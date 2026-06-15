#!/usr/bin/env python3
"""Live dashboard for symbolmap renaming targets."""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import threading
import time
from collections import Counter
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse


ROOT = Path(__file__).resolve().parents[2]
SM_DIR = ROOT / "config" / "GC6E01" / "symbolmap"
SYMBOLS = ROOT / "config" / "GC6E01" / "symbols.txt"
FUNC_TU_MAP = ROOT / "config" / "GC6E01" / "func_tu_map.json"
DECOMP_REPORT = ROOT / "report.json"
DECOMP_STATUS_LOG = ROOT / "tools" / "decomp_work" / "coordination" / "status.md"
# --- v9: reader / wall / agent / token / lockout data sources --------------------
DECOMP_WORK = ROOT / "tools" / "decomp_work"
COORD_DIR = DECOMP_WORK / "coordination"
WALLS_MD = ROOT / "WALLS.md"
EQUIVALENT_TXT = DECOMP_WORK / "equivalent.txt"
CS_WALLS_JSON = ROOT / "build" / "cs_walls.json"
AGENT_STATUS_TXT = COORD_DIR / "agent_status.txt"
CLAIMS_JSON = COORD_DIR / "claims.json"
TASKS_JSON = COORD_DIR / "tasks.json"
AGENT_TOKENS_JSON = ROOT / ".omc" / "agent_tokens.json"
AGENT_LIMITS_JSON = DECOMP_WORK / "agent_limits.json"
OPENCODE_STORAGE = Path(
    os.environ.get(
        "OPENCODE_STORAGE",
        str(Path.home() / ".local" / "share" / "opencode" / "storage"),
    )
)
HISTORY_FILE = ROOT / ".omx" / "state" / "renaming_dashboard_history.json"
UNIT_HISTORY_FILE = ROOT / ".omx" / "state" / "renaming_dashboard_unit_history.json"
FN_HISTORY_FILE = ROOT / ".omx" / "state" / "renaming_dashboard_fn_history.json"
HISTORY_INTERVAL_SECONDS = 60
# Ring cap for the global match-progress history. Raised from 500 -> 2000 so the
# months-long git backfill (one row per report.json commit) is not evicted.
HISTORY_CAP = 2000
UNIT_HISTORY_CAP = 300
FN_HISTORY_CAP = 200
STATE_CACHE_TTL_SECONDS = 1.8
DASHBOARD_VERSION = 9

# In-process TTL cache for build_state(). The full rebuild runs git x4, reparses
# the ~646KB report.json, and shells out to rg per Proposed/Needs-wiring row, so
# it can take ~19s. The 5s front-end auto-refresh would otherwise stack rebuilds.
_STATE_CACHE: dict[str, object] = {"value": None, "expires": 0.0}

MAP_RE = re.compile(
    r"^(fn_[0-9A-Fa-f]{8})\s*->\s*([A-Za-z_.$][\w.$:@?]*)\s*//\s*(.*?)\s*$"
)
LEAD_RE = re.compile(
    r"^\|\s*`?(fn_[0-9A-Fa-f]{8})`?\s*\|\s*`?([^|`]+?)`?\s*\|\s*([^|]+?)\s*\|\s*([^|]+?)\s*\|"
)
SYMBOL_RE = re.compile(
    r"^([A-Za-z_.$][\w.$:@?]*)\s*=\s*(\.\w+):(0x[0-9A-Fa-f]+);"
    r"\s*//\s*type:(\w+)\s*size:(0x[0-9A-Fa-f]+)(.*)$"
)


def read_text(path: Path) -> str:
    if not path.exists():
        return ""
    return path.read_text(encoding="utf-8", errors="replace")


def load_simple_map(path: Path) -> dict[str, dict[str, str]]:
    rows: dict[str, dict[str, str]] = {}
    for line in read_text(path).splitlines():
        match = MAP_RE.match(line.strip())
        if not match:
            continue
        fn, name, provenance = match.groups()
        rows[fn] = {"fn": fn, "name": name, "provenance": provenance.strip()}
    return rows


def load_leads(path: Path) -> dict[str, dict[str, str]]:
    rows: dict[str, dict[str, str]] = {}
    for line in read_text(path).splitlines():
        match = LEAD_RE.match(line.strip())
        if not match:
            continue
        fn, name, provenance, header = match.groups()
        if fn == "addr (fn_)":
            continue
        rows[fn] = {
            "fn": fn,
            "name": name.strip(),
            "provenance": provenance.strip(),
            "header": header.strip(),
        }
    return rows


def load_symbols(path: Path) -> tuple[dict[str, dict[str, str]], dict[str, dict[str, str]]]:
    by_name: dict[str, dict[str, str]] = {}
    by_addr: dict[str, dict[str, str]] = {}
    for line in read_text(path).splitlines():
        match = SYMBOL_RE.match(line.strip())
        if not match:
            continue
        name, section, addr, kind, size, comment = match.groups()
        row = {
            "name": name,
            "section": section,
            "addr": addr.lower(),
            "kind": kind,
            "size": size,
            "comment": comment.strip(),
        }
        by_name[name] = row
        by_addr[addr.lower()] = row
    return by_name, by_addr


def load_tu_map(path: Path) -> dict[str, dict[str, str]]:
    if not path.exists():
        return {}
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError:
        return {}
    return data if isinstance(data, dict) else {}


def fn_to_addr(fn: str) -> str:
    return "0x" + fn.removeprefix("fn_").lower()


def git_value(args: list[str]) -> str:
    try:
        proc = subprocess.run(
            ["git", *args],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=3,
        )
    except (OSError, subprocess.TimeoutExpired):
        return ""
    if proc.returncode != 0:
        return ""
    return proc.stdout.strip()


def recent_commits(limit: int = 10) -> list[dict[str, object]]:
    text = git_value(
        [
            "log",
            f"-n{limit}",
            "--date=format:%b %d, %H:%M",
            "--pretty=format:%h%x09%cd%x09%ct%x09%s",
        ]
    )
    rows: list[dict[str, object]] = []
    for line in text.splitlines():
        parts = line.split("\t", 3)
        if len(parts) != 4:
            continue
        sha, when, ct, subject = parts
        try:
            unix = int(ct)
        except ValueError:
            unix = 0
        # `when` retained for backwards-compat; `unix` lets the front-end format
        # the commit time in HST via hstTime().
        rows.append({"sha": sha, "when": when, "unix": unix, "subject": subject})
    return rows


def recent_commit_attempts(limit: int = 40) -> list[dict[str, object]]:
    """Derive attempt-log entries from recent git commits that touched src/*.c.

    Codex commits its per-file decomp work to git (not status.md), so without
    this its progress (e.g. "Advance menu_middle matching" on menu_middle.c)
    never shows up in the Attempt Log. We emit one entry per changed .c file
    (capped per commit) in the same shape load_attempt_log() produces so the
    front-end's reverse-sort and relatedAttempts(unit) filtering both work.
    """
    text = git_value(
        [
            "log",
            f"-n{limit}",
            "--name-only",
            "--pretty=format:%x01%H%x09%ct%x09%s",
            "--",
            "src",
        ]
    )
    rows: list[dict[str, object]] = []
    # Records are separated by the \x01 we injected at the start of each header.
    for record in text.split("\x01"):
        record = record.strip("\n")
        if not record:
            continue
        lines = record.split("\n")
        header = lines[0].split("\t", 2)
        if len(header) != 3:
            continue
        _sha, ct, subject = header
        try:
            unix = int(ct)
        except ValueError:
            continue
        iso = time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime(unix))
        emitted = 0
        for path in lines[1:]:
            path = path.strip()
            if not path or not path.endswith(".c"):
                continue
            if emitted >= 3:
                break
            emitted += 1
            base = path.replace("\\", "/").rsplit("/", 1)[-1]
            rows.append(
                {
                    "timestamp": iso,
                    "unix": unix,
                    "agent": "git",
                    "kind": "commit",
                    "function": "",
                    "file": base,
                    "percent": None,
                    "message": subject,
                }
            )
    return rows


def source_label(source: str) -> str:
    if not source:
        return "unknown"
    path = source.replace("\\", "/")
    name = path.rsplit("/", 1)[-1]
    return name.removesuffix(".c")


def source_refs(symbol: str) -> int:
    try:
        proc = subprocess.run(
            [
                "rg",
                "-n",
                "--fixed-strings",
                symbol,
                "src",
                "include",
                "config/GC6E01/symbols.txt",
                "config/GC6E01/symbols.build.txt",
                "config/GC6E01/func_tu_map.json",
                "config/GC6E01/symdb.json",
                "--glob",
                "!*.inc",
            ],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=3,
        )
    except (OSError, subprocess.TimeoutExpired):
        return -1
    if proc.returncode not in (0, 1):
        return -1
    return len([line for line in proc.stdout.splitlines() if line.strip()])


def int_value(value: object) -> int:
    try:
        return int(str(value), 0)
    except (TypeError, ValueError):
        return 0


def pct(done: int, total: int) -> float:
    if total <= 0:
        return 0.0
    return round(done * 100.0 / total, 1)


def float_pct(value: object) -> float:
    try:
        return round(float(value), 2)
    except (TypeError, ValueError):
        return 0.0


def match_status(value: object) -> str:
    score = float_pct(value)
    if score >= 99.95:
        return "matched"
    if score >= 90.0:
        return "near"
    if score > 0:
        return "partial"
    return "missing"


def classify_provenance(provenance: str) -> str:
    text = provenance.lower()
    if "xd" in text:
        return "XD port"
    if "string" in text:
        return "String evidence"
    if "structural" in text:
        return "Structural"
    return "Other"


def load_decomp_report(path: Path) -> dict[str, object]:
    empty = {
        "available": False,
        "fuzzy_pct": 0.0,
        "code_pct": 0.0,
        "functions_pct": 0.0,
        "matched_functions": 0,
        "total_functions": 0,
        "matched_code": 0,
        "total_code": 0,
        "complete_units": 0,
        "total_units": 0,
        "units": [],
        "source": str(path),
    }
    if not path.exists():
        return empty
    try:
        report = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return empty
    measures = report.get("measures", {})
    if not isinstance(measures, dict):
        return empty

    units = []
    for unit in report.get("units", []):
        if not isinstance(unit, dict):
            continue
        unit_measures = unit.get("measures", {})
        if not isinstance(unit_measures, dict):
            unit_measures = {}
        metadata = unit.get("metadata", {})
        if not isinstance(metadata, dict):
            metadata = {}
        total_functions = int_value(unit_measures.get("total_functions", 0))
        matched_functions = int_value(unit_measures.get("matched_functions", 0))
        functions = []
        status_counts: Counter[str] = Counter()
        for index, function in enumerate(unit.get("functions", [])):
            if not isinstance(function, dict):
                continue
            fn_pct = float_pct(function.get("fuzzy_match_percent", 0))
            status = match_status(fn_pct)
            status_counts[status] += 1
            functions.append(
                {
                    "index": index,
                    "name": function.get("name", ""),
                    "size": int_value(function.get("size", 0)),
                    "fuzzy_pct": fn_pct,
                    "status": status,
                }
            )
        functions.sort(
            key=lambda row: (
                float(row.get("fuzzy_pct", 0)) >= 99.95,
                -int(row.get("size", 0)),
                str(row.get("name", "")),
            )
        )
        units.append(
            {
                "name": unit.get("name", ""),
                "source": metadata.get("source_path", ""),
                "fuzzy_pct": float_pct(unit_measures.get("fuzzy_match_percent", 0)),
                "code_pct": float_pct(unit_measures.get("matched_code_percent", 0)),
                "functions_pct": float_pct(
                    unit_measures.get("matched_functions_percent", 0)
                ),
                "matched_functions": matched_functions,
                "total_functions": total_functions,
                "matched_code": int_value(unit_measures.get("matched_code", 0)),
                "total_code": int_value(unit_measures.get("total_code", 0)),
                "complete": bool(metadata.get("complete"))
                or float_pct(unit_measures.get("matched_functions_percent", 0)) >= 100.0,
                "functions": functions,
                "function_status": dict(status_counts),
            }
        )
    units.sort(
        key=lambda row: (
            -int(row.get("total_functions", 0)),
            str(row.get("name", "")),
        )
    )

    return {
        "available": True,
        "fuzzy_pct": float_pct(measures.get("fuzzy_match_percent", 0)),
        "code_pct": float_pct(measures.get("matched_code_percent", 0)),
        "functions_pct": float_pct(measures.get("matched_functions_percent", 0)),
        "matched_functions": int_value(measures.get("matched_functions", 0)),
        "total_functions": int_value(measures.get("total_functions", 0)),
        "matched_code": int_value(measures.get("matched_code", 0)),
        "total_code": int_value(measures.get("total_code", 0)),
        "complete_units": int_value(measures.get("complete_units", 0)),
        "total_units": int_value(measures.get("total_units", 0)),
        "units": units,
        "source": str(path),
        "updated_at": time.strftime(
            "%Y-%m-%d %H:%M:%S", time.localtime(path.stat().st_mtime)
        ),
    }


def load_attempt_log(path: Path, limit: int = 400) -> list[dict[str, object]]:
    rows: list[dict[str, object]] = []
    line_re = re.compile(
        r"^- \*\*(?P<timestamp>[^*]+)\*\*\s+`(?P<agent>[^`]+)`\s+(?P<message>.*)$"
    )
    fn_re = re.compile(r"fn_[0-9A-Fa-f]{8}")
    pct_re = re.compile(r"([0-9]+(?:\.[0-9]+)?)%")
    file_re = re.compile(r"\bin\s+([^()\s]+\.c|\?\.c)")
    markers = (
        "MATCH!",
        "COMMIT",
        "REGRESSION",
        "Claimed",
        "Completed",
        "Decision",
        "Enqueued",
    )
    for line in read_text(path).splitlines():
        match = line_re.match(line.strip())
        if not match:
            continue
        raw_message = match.group("message").strip()
        message = raw_message
        for marker in markers:
            index = raw_message.find(marker)
            if index >= 0:
                message = raw_message[index:].strip()
                break
        kind = "note"
        upper = message.upper()
        if "REGRESSION" in upper:
            kind = "regression"
        elif "MATCH!" in upper:
            kind = "match"
        elif "COMMIT" in upper:
            kind = "commit"
        elif "CLAIMED" in upper:
            kind = "claim"
        fn_match = fn_re.search(message)
        pct_match = pct_re.search(message)
        file_match = file_re.search(message)
        rows.append(
            {
                "timestamp": match.group("timestamp"),
                "agent": match.group("agent"),
                "kind": kind,
                "function": fn_match.group(0) if fn_match else "",
                "file": file_match.group(1) if file_match else "",
                "percent": float_pct(pct_match.group(1)) if pct_match else None,
                "message": message,
            }
        )
    return rows[-limit:]


def load_history() -> list[dict[str, object]]:
    if not HISTORY_FILE.exists():
        return []
    try:
        data = json.loads(HISTORY_FILE.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return []
    if not isinstance(data, list):
        return []
    return [row for row in data if isinstance(row, dict)]


def write_history(history: list[dict[str, object]]) -> None:
    HISTORY_FILE.parent.mkdir(parents=True, exist_ok=True)
    tmp = HISTORY_FILE.with_suffix(".json.tmp")
    tmp.write_text(json.dumps(history[-HISTORY_CAP:], indent=2), encoding="utf-8")
    tmp.replace(HISTORY_FILE)


def snapshot_for_history(state: dict[str, object]) -> dict[str, object]:
    counts = state.get("counts", {})
    if not isinstance(counts, dict):
        counts = {}
    metrics = state.get("metrics", {})
    if not isinstance(metrics, dict):
        metrics = {}
    decomp = state.get("decomp", {})
    if not isinstance(decomp, dict):
        decomp = {}
    now = time.time()
    return {
        "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
        "unix": int(now),
        "head": state.get("head", ""),
        "branch": state.get("branch", ""),
        "targets": counts.get("targets", 0),
        "needs_wiring": counts.get("by_status", {}).get("Needs wiring", 0)
        if isinstance(counts.get("by_status"), dict)
        else 0,
        "proposed": counts.get("by_status", {}).get("Proposed", 0)
        if isinstance(counts.get("by_status"), dict)
        else 0,
        "recorded": counts.get("by_status", {}).get("Recorded", 0)
        if isinstance(counts.get("by_status"), dict)
        else 0,
        "renamed": counts.get("by_status", {}).get("Renamed", 0)
        if isinstance(counts.get("by_status"), dict)
        else 0,
        "completion_pct": metrics.get("completion_pct", 0),
        "decomp_fuzzy_pct": decomp.get("fuzzy_pct", 0),
        "decomp_code_pct": decomp.get("code_pct", 0),
        "decomp_functions_pct": decomp.get("functions_pct", 0),
        "decomp_matched_functions": decomp.get("matched_functions", 0),
        "decomp_total_functions": decomp.get("total_functions", 0),
        "old_ref_total": metrics.get("old_ref_total", 0),
    }


def should_record_history(history: list[dict[str, object]], snapshot: dict[str, object]) -> bool:
    if not history:
        return True
    last = history[-1]
    tracked = [
        "targets",
        "needs_wiring",
        "proposed",
        "recorded",
        "renamed",
        "completion_pct",
        "decomp_fuzzy_pct",
        "decomp_code_pct",
        "decomp_functions_pct",
        "decomp_matched_functions",
        "old_ref_total",
        "head",
    ]
    if any(last.get(key) != snapshot.get(key) for key in tracked):
        return True
    return int(snapshot["unix"]) - int(last.get("unix", 0)) >= HISTORY_INTERVAL_SECONDS


def update_history(state: dict[str, object]) -> list[dict[str, object]]:
    history = load_history()
    snapshot = snapshot_for_history(state)
    if should_record_history(history, snapshot):
        history.append(snapshot)
        write_history(history)
    return history[-HISTORY_CAP:]


def _load_json_obj(path: Path) -> dict[str, object]:
    if not path.exists():
        return {}
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return {}
    return data if isinstance(data, dict) else {}


def _write_json_obj(path: Path, data: dict[str, object]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(json.dumps(data), encoding="utf-8")
    tmp.replace(path)


def load_unit_history(source: str) -> list[dict[str, object]]:
    data = _load_json_obj(UNIT_HISTORY_FILE)
    rows = data.get(source)
    if not isinstance(rows, list):
        return []
    return [row for row in rows if isinstance(row, dict)]


def load_fn_history(name: str) -> list[dict[str, object]]:
    data = _load_json_obj(FN_HISTORY_FILE)
    rows = data.get(name)
    if not isinstance(rows, list):
        return []
    return [row for row in rows if isinstance(row, dict)]


def update_unit_history(decomp: dict[str, object]) -> None:
    """Append a per-unit sample only when that unit's functions_pct or code_pct
    changed (change-gated, like should_record_history but per unit)."""
    units = decomp.get("units")
    if not isinstance(units, list) or not units:
        return
    store = _load_json_obj(UNIT_HISTORY_FILE)
    now = int(time.time())
    dirty = False
    for unit in units:
        if not isinstance(unit, dict):
            continue
        source = str(unit.get("source") or unit.get("name") or "")
        if not source:
            continue
        fp = float_pct(unit.get("functions_pct", 0))
        cp = float_pct(unit.get("code_pct", 0))
        rows = store.get(source)
        if not isinstance(rows, list):
            rows = []
        last = rows[-1] if rows else None
        if (
            last is not None
            and float_pct(last.get("fp", 0)) == fp
            and float_pct(last.get("cp", 0)) == cp
        ):
            continue
        rows.append(
            {
                "unix": now,
                "fp": fp,
                "cp": cp,
                "mc": int_value(unit.get("matched_code", 0)),
                "mf": int_value(unit.get("matched_functions", 0)),
                "tf": int_value(unit.get("total_functions", 0)),
            }
        )
        store[source] = rows[-UNIT_HISTORY_CAP:]
        dirty = True
    if dirty:
        _write_json_obj(UNIT_HISTORY_FILE, store)


def update_fn_history(decomp: dict[str, object]) -> None:
    """Append a per-function sample for unmatched (<100%) functions only, when
    that function's fuzzy_pct changed. Keyed by function name. Capped per fn."""
    units = decomp.get("units")
    if not isinstance(units, list) or not units:
        return
    store = _load_json_obj(FN_HISTORY_FILE)
    now = int(time.time())
    dirty = False
    seen: set[str] = set()
    for unit in units:
        if not isinstance(unit, dict):
            continue
        for fn in unit.get("functions", []):
            if not isinstance(fn, dict):
                continue
            name = str(fn.get("name") or "")
            if not name or name in seen:
                continue
            seen.add(name)
            fp = float_pct(fn.get("fuzzy_pct", 0))
            if fp >= 99.95:
                # Drop history for now-matched fns so the file stays small.
                if name in store:
                    del store[name]
                    dirty = True
                continue
            rows = store.get(name)
            if not isinstance(rows, list):
                rows = []
            last = rows[-1] if rows else None
            if last is not None and float_pct(last.get("fuzzy_pct", 0)) == fp:
                continue
            rows.append({"unix": now, "fuzzy_pct": fp})
            store[name] = rows[-FN_HISTORY_CAP:]
            dirty = True
    if dirty:
        _write_json_obj(FN_HISTORY_FILE, store)


def load_unit_functions(source: str) -> dict[str, object]:
    """Lazy endpoint backing: return one unit's functions[] without shipping all
    units. Matches on metadata.source_path (preferred) or unit name."""
    if not DECOMP_REPORT.exists() or not source:
        return {"available": False, "source": source, "functions": []}
    try:
        report = json.loads(DECOMP_REPORT.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return {"available": False, "source": source, "functions": []}
    norm = source.replace("\\", "/")
    for unit in report.get("units", []):
        if not isinstance(unit, dict):
            continue
        metadata = unit.get("metadata", {})
        if not isinstance(metadata, dict):
            metadata = {}
        unit_source = str(metadata.get("source_path", "")).replace("\\", "/")
        if unit_source != norm and str(unit.get("name", "")) != source:
            continue
        functions = []
        for index, function in enumerate(unit.get("functions", [])):
            if not isinstance(function, dict):
                continue
            fn_pct = float_pct(function.get("fuzzy_match_percent", 0))
            functions.append(
                {
                    "index": index,
                    "name": function.get("name", ""),
                    "size": int_value(function.get("size", 0)),
                    "fuzzy_pct": fn_pct,
                    "status": match_status(fn_pct),
                }
            )
        functions.sort(
            key=lambda row: (
                float(row.get("fuzzy_pct", 0)) >= 99.95,
                -int(row.get("size", 0)),
                str(row.get("name", "")),
            )
        )
        return {
            "available": True,
            "source": unit_source or source,
            "name": unit.get("name", ""),
            "functions": functions,
        }
    return {"available": False, "source": source, "functions": []}


def get_state(force: bool = False) -> dict[str, object]:
    """Return build_state() through a short TTL cache so the 5s auto-refresh and
    the slow rebuild stop colliding."""
    now = time.monotonic()
    cached = _STATE_CACHE.get("value")
    if not force and cached is not None and now < float(_STATE_CACHE.get("expires", 0)):
        return cached  # type: ignore[return-value]
    state = build_state()
    _STATE_CACHE["value"] = state
    _STATE_CACHE["expires"] = now + STATE_CACHE_TTL_SECONDS
    return state


def build_state() -> dict[str, object]:
    proposed = load_simple_map(SM_DIR / "proposed_symbols.txt")
    applied = load_simple_map(SM_DIR / "applied_symbols.txt")
    structural = load_simple_map(SM_DIR / "structural_applied.txt")
    leads = load_leads(SM_DIR / "leads_needs_wiring.md")
    lead_order = {fn: idx for idx, fn in enumerate(leads)}
    symbols_by_name, symbols_by_addr = load_symbols(SYMBOLS)
    tu_map = load_tu_map(FUNC_TU_MAP)

    all_fns = set(proposed) | set(applied) | set(structural) | set(leads)
    rows = []
    for fn in sorted(all_fns, key=lambda item: int(item.removeprefix("fn_"), 16)):
        base = proposed.get(fn) or applied.get(fn) or structural.get(fn) or leads.get(fn)
        if not base:
            continue

        name = leads.get(fn, {}).get("name", base["name"])
        addr = fn_to_addr(fn)
        current = symbols_by_addr.get(addr)
        tu = tu_map.get(name) or tu_map.get(fn) or {}

        status = "Proposed"
        if fn in leads:
            status = "Needs wiring"
        if current and current["name"] == name:
            status = "Renamed"
        elif fn in applied or fn in structural:
            status = "Recorded"

        old_refs = source_refs(fn) if status in ("Needs wiring", "Proposed") else 0
        if old_refs == -1:
            old_refs_label = "unknown"
        else:
            old_refs_label = str(old_refs)

        rows.append(
            {
                "fn": fn,
                "name": name,
                "status": status,
                "addr": addr,
                "size": (current or {}).get("size") or tu.get("size", ""),
                "source": tu.get("src") or "",
                "provenance": leads.get(fn, {}).get(
                    "provenance", base.get("provenance", "")
                ),
                "header": leads.get(fn, {}).get("header", ""),
                "current_symbol": (current or {}).get("name", ""),
                "old_refs": old_refs_label,
            }
        )

    rank = {"Needs wiring": 0, "Proposed": 1, "Recorded": 2, "Renamed": 3}
    rows.sort(
        key=lambda row: (
            rank.get(str(row["status"]), 9),
            lead_order.get(str(row["fn"]), int(str(row["fn"])[3:], 16)),
        )
    )
    next_target = next((row for row in rows if row["status"] == "Needs wiring"), None)

    status_counts: dict[str, int] = {}
    source_counts: Counter[str] = Counter()
    provenance_counts: Counter[str] = Counter()
    source_summary: dict[str, dict[str, object]] = {}
    old_ref_total = 0
    known_size_total = 0
    for row in rows:
        key = str(row["status"])
        status_counts[key] = status_counts.get(key, 0) + 1
        source = str(row["source"] or "unknown")
        source_counts[source] += 1
        provenance_counts[classify_provenance(str(row["provenance"]))] += 1
        if str(row["old_refs"]).isdigit():
            old_ref_total += int(str(row["old_refs"]))
        size = int_value(row["size"])
        known_size_total += size
        summary = source_summary.setdefault(
            source,
            {
                "source": source,
                "label": source_label(source),
                "targets": 0,
                "wired": 0,
                "renamed": 0,
                "recorded": 0,
                "needs_wiring": 0,
                "proposed": 0,
                "size_bytes": 0,
            },
        )
        summary["targets"] = int(summary["targets"]) + 1
        summary["size_bytes"] = int(summary["size_bytes"]) + size
        if key in ("Recorded", "Renamed"):
            summary["wired"] = int(summary["wired"]) + 1
        if key == "Renamed":
            summary["renamed"] = int(summary["renamed"]) + 1
        elif key == "Recorded":
            summary["recorded"] = int(summary["recorded"]) + 1
        elif key == "Needs wiring":
            summary["needs_wiring"] = int(summary["needs_wiring"]) + 1
        elif key == "Proposed":
            summary["proposed"] = int(summary["proposed"]) + 1

    total_targets = len(rows)
    wired_targets = status_counts.get("Recorded", 0) + status_counts.get("Renamed", 0)
    active_targets = status_counts.get("Needs wiring", 0) + status_counts.get("Proposed", 0)
    tu_tiles = []
    for summary in source_summary.values():
        targets = int(summary["targets"])
        wired = int(summary["wired"])
        summary["completion_pct"] = pct(wired, targets)
        tu_tiles.append(summary)
    tu_tiles.sort(
        key=lambda row: (
            -int(row.get("targets", 0)),
            str(row.get("source", "")),
        )
    )
    decomp = load_decomp_report(DECOMP_REPORT)
    # Merge status.md attempt log with git-commit-derived entries so codex's
    # per-file work (committed to git, not status.md) appears in the log and in
    # per-unit drill-downs. The front-end reverse-sorts and filters by `file`.
    attempt_log = load_attempt_log(DECOMP_STATUS_LOG) + recent_commit_attempts()
    return {
        "version": DASHBOARD_VERSION,
        "generated_at": time.strftime("%Y-%m-%d %H:%M:%S"),
        "repo": str(ROOT),
        "head": git_value(["rev-parse", "--short", "HEAD"]),
        "branch": git_value(["branch", "--show-current"]),
        "recent_commits": recent_commits(),
        "decomp": decomp,
        "attempt_log": attempt_log,
        "next_target": next_target,
        "counts": {
            "leads": len(leads),
            "proposed": len(proposed),
            "applied": len(applied),
            "structural_recorded": len(structural),
            "targets": len(rows),
            "by_status": status_counts,
        },
        "metrics": {
            "completion_pct": pct(wired_targets, total_targets),
            "rename_pct": pct(status_counts.get("Renamed", 0), total_targets),
            "active_pct": pct(active_targets, total_targets),
            "wired_targets": wired_targets,
            "active_targets": active_targets,
            "old_ref_total": old_ref_total,
            "known_size_bytes": known_size_total,
        },
        "charts": {
            "status": [
                {"label": key, "value": status_counts.get(key, 0)}
                for key in ("Needs wiring", "Proposed", "Recorded", "Renamed")
            ],
            "sources": [
                {"label": key, "value": value}
                for key, value in source_counts.most_common(8)
            ],
            "provenance": [
                {"label": key, "value": value}
                for key, value in provenance_counts.most_common()
            ],
        },
        "targets": rows,
        "tu_tiles": tu_tiles,
        "files": {
            "leads": str(SM_DIR / "leads_needs_wiring.md"),
            "proposed": str(SM_DIR / "proposed_symbols.txt"),
            "applied": str(SM_DIR / "applied_symbols.txt"),
            "structural": str(SM_DIR / "structural_applied.txt"),
            "symbols": str(SYMBOLS),
            "tu_map": str(FUNC_TU_MAP),
            "decomp_report": str(DECOMP_REPORT),
            "decomp_status_log": str(DECOMP_STATUS_LOG),
        },
    }


# =========================================================================== #
# v9: decomp.me-style function reader (compile + objdiff, per-instruction)     #
# =========================================================================== #
# band.py already encodes the exact compile+objdiff invocation against the
# immutable target object. We reuse compile_check (per-file flags/version/target)
# the same way band.py does, then run objdiff-cli in JSON mode and slice out the
# requested fn's left(TARGET)/right(CURRENT) instruction rows.
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "decomp_work"))
try:  # imported lazily-safe: the reader endpoint degrades to an error payload
    import compile_check as _compile_check  # type: ignore
except Exception:  # noqa: BLE001
    _compile_check = None
_OBJDIFF_CLI = ROOT / "tools" / "objdiff-cli.exe"

# TTL cache keyed by (source, fn). The compile is slow (~seconds), so a hit
# within the TTL or while the source file is unchanged returns instantly.
_ASM_CACHE: dict[str, dict[str, object]] = {}
_ASM_CACHE_TTL_SECONDS = 90.0
_ASM_LOCK = threading.Lock()


def _resolve_source_path(name: str) -> Path | None:
    """Resolve a stem or repo-relative path to a tracked src/**.c (band.py-style)."""
    if not name:
        return None
    norm = name.replace("\\", "/")
    cand = ROOT / norm
    if cand.exists():
        return cand.resolve()
    stem = Path(norm).stem
    matches = sorted((ROOT / "src").rglob(f"{stem}.c"))
    return matches[0].resolve() if matches else None


def _instr_text(ins: object) -> str:
    if not isinstance(ins, dict):
        return "---"
    inner = ins.get("instruction")
    if isinstance(inner, dict):
        return str(inner.get("formatted") or inner.get("mnemonic") or "?")
    return "---"


def _row_state(lk: str, rk: str) -> str:
    """Map objdiff diff_kind pair -> a coarse row colour class."""
    if lk in ("DIFF_NONE", "") and rk in ("DIFF_NONE", ""):
        return "same"
    if lk in ("DIFF_DELETE",) or rk in ("DIFF_INSERT",):
        return "addrm"
    return "diff"


def compute_asm_diff(source: str, fn: str) -> dict[str, object]:
    """Compile `source` to its base .o and objdiff vs the target; return the
    aligned per-instruction rows for `fn`. Shape mirrors band.py cmd_diff."""
    if _compile_check is None:
        return {"available": False, "error": "compile_check import failed", "fn": fn, "source": source}
    src_path = _resolve_source_path(source)
    if src_path is None:
        return {"available": False, "error": f"source not found: {source}", "fn": fn, "source": source}
    if not _OBJDIFF_CLI.exists():
        return {"available": False, "error": "objdiff-cli not found", "fn": fn, "source": source}

    cache_key = f"{src_path}|{fn}"
    try:
        mtime = src_path.stat().st_mtime
    except OSError:
        mtime = 0.0
    now = time.monotonic()
    with _ASM_LOCK:
        hit = _ASM_CACHE.get(cache_key)
        if hit is not None and hit.get("_mtime") == mtime and now < float(hit.get("_expires", 0)):
            return hit["payload"]  # type: ignore[return-value]

    try:
        target_o = _compile_check.find_target_obj(src_path)
        if not Path(target_o).exists():
            return {"available": False, "error": f"target object missing: {target_o}", "fn": fn, "source": source}
        base_o = _compile_check.compile_source(src_path, verbose=False)
    except SystemExit as exc:  # compile_source exits on failure
        return {"available": False, "error": f"compile failed: {exc}", "fn": fn, "source": source}
    except Exception as exc:  # noqa: BLE001
        return {"available": False, "error": f"compile error: {exc}", "fn": fn, "source": source}

    try:
        proc = subprocess.run(
            [str(_OBJDIFF_CLI), "diff", "-1", str(target_o), "-2", str(base_o),
             "-o", "-", "--format", "json",
             "-c", "ppc.calculatePoolRelocations=false"],
            cwd=str(ROOT), capture_output=True, text=True, timeout=180,
        )
    except (OSError, subprocess.TimeoutExpired) as exc:
        return {"available": False, "error": f"objdiff failed: {exc}", "fn": fn, "source": source}
    if proc.returncode != 0:
        return {"available": False, "error": "objdiff failed: " + proc.stderr[:300], "fn": fn, "source": source}
    try:
        diff = json.loads(proc.stdout)
    except json.JSONDecodeError:
        return {"available": False, "error": "objdiff returned non-JSON", "fn": fn, "source": source}

    def _side(side: str) -> list:
        for sym in diff.get(side, {}).get("symbols", []):
            if sym.get("name") == fn:
                return sym.get("instructions", []) or []
        return []

    left, right = _side("left"), _side("right")
    rows = []
    matched = 0
    total = 0
    for idx in range(max(len(left), len(right))):
        li = left[idx] if idx < len(left) else None
        ri = right[idx] if idx < len(right) else None
        lk = (li.get("diff_kind") if isinstance(li, dict) else None) or ("X" if li is None else "DIFF_NONE")
        rk = (ri.get("diff_kind") if isinstance(ri, dict) else None) or ("X" if ri is None else "DIFF_NONE")
        state = _row_state(lk, rk)
        if li is not None and ri is not None:
            total += 1
            if state == "same":
                matched += 1
        rows.append({"l": _instr_text(li), "r": _instr_text(ri), "state": state})
    # fuzzy_pct from objdiff symbol match_percent (right side), fallback to ratio
    fuzzy = 0.0
    for sym in diff.get("right", {}).get("symbols", []):
        if sym.get("name") == fn:
            fuzzy = float(sym.get("match_percent") or 0.0)
            break
    if fuzzy == 0.0 and total:
        fuzzy = round(100.0 * matched / total, 2)

    payload = {
        "available": True,
        "fn": fn,
        "source": str(src_path.relative_to(ROOT)).replace("\\", "/"),
        "target_obj": Path(target_o).name,
        "fuzzy_pct": round(fuzzy, 2),
        "rows": rows,
        "row_count": len(rows),
        "matched": matched,
        "total": total,
        "generated_at": time.strftime("%Y-%m-%d %H:%M:%S"),
    }
    with _ASM_LOCK:
        _ASM_CACHE[cache_key] = {"_mtime": mtime, "_expires": now + _ASM_CACHE_TTL_SECONDS, "payload": payload}
    return payload


# =========================================================================== #
# v9: per-function wall / equivalent / attempt info                           #
# =========================================================================== #
_WALL_CLASS_RE = re.compile(r"\b(W-[A-Za-z0-9-]+|W[0-9])\b")


def _parse_equivalent(fn: str) -> tuple[bool, str]:
    """Return (is_equivalent, note) by scanning equivalent.txt for `fn`."""
    text = read_text(EQUIVALENT_TXT)
    for line in text.splitlines():
        stripped = line.strip()
        if not stripped or stripped.startswith("#"):
            continue
        # `fn_XXXX   # reason`  (whitespace or tab separated)
        head = re.split(r"[\s#]", stripped, 1)[0]
        if head == fn:
            note = ""
            if "#" in stripped:
                note = stripped.split("#", 1)[1].strip()
            return True, note
    return False, ""


def _parse_walls_md(fn: str) -> tuple[str, str]:
    """Return (wall_class, note) for `fn` from WALLS.md table/bullet entries."""
    text = read_text(WALLS_MD)
    for raw in text.splitlines():
        # match the fn name wrapped in backticks or bare, anywhere on the line
        if fn not in raw:
            continue
        # Skip lines that are pure cross-references in prose (keep table rows + bullets)
        stripped = raw.strip()
        if not (stripped.startswith("|") or stripped.startswith("- ")):
            continue
        cls_match = _WALL_CLASS_RE.search(raw)
        wall_class = cls_match.group(1) if cls_match else ""
        # Trim markdown table pipes / leading bullet for a readable note.
        note = stripped.strip("|").strip()
        note = re.sub(r"^[-*]\s*", "", note)
        # Collapse the leading "`fn_XXXX`" token out of the note for brevity.
        note = note.replace(f"`{fn}`", "").replace(fn, "").strip(" |-")
        if len(note) > 360:
            note = note[:357] + "..."
        return wall_class, note
    return "", ""


def _parse_cs_walls(fn: str) -> bool:
    """True if `fn` is listed in build/cs_walls.json (a flat list of fn names)."""
    if not CS_WALLS_JSON.exists():
        return False
    try:
        data = json.loads(CS_WALLS_JSON.read_text(encoding="utf-8", errors="replace"))
    except (OSError, json.JSONDecodeError):
        return False
    if isinstance(data, list):
        return fn in data
    if isinstance(data, dict):
        return fn in data
    return False


def _fn_attempts(fn: str) -> list[dict[str, object]]:
    """Recent status.md attempt-log lines that name `fn`."""
    out = []
    for row in load_attempt_log(DECOMP_STATUS_LOG):
        if row.get("function") == fn or (fn and fn in str(row.get("message", ""))):
            out.append({
                "timestamp": row.get("timestamp"),
                "kind": row.get("kind"),
                "percent": row.get("percent"),
                "message": row.get("message"),
            })
    return out[-25:]


def load_fn_info(fn: str) -> dict[str, object]:
    is_equiv, equiv_note = _parse_equivalent(fn)
    wall_class, wall_note = _parse_walls_md(fn)
    in_cs_walls = _parse_cs_walls(fn)
    attempts = _fn_attempts(fn)
    return {
        "fn": fn,
        "wall_class": wall_class,
        "note": wall_note or equiv_note,
        "is_equivalent": is_equiv,
        "in_cs_walls": in_cs_walls,
        "attempts": attempts,
        "attempt_count": len(attempts),
    }


# =========================================================================== #
# v9: agent-activity panel (merge shell dashboards into the web)              #
# =========================================================================== #
def load_agents() -> dict[str, object]:
    """Parse coordination/{agent_status.txt, claims.json, tasks.json} into a
    who-is-working-on-what view."""
    agents: dict[str, dict[str, object]] = {}

    # agent_status.txt: e.g. "20:22:58 codex=false opencode=true" (latest line)
    status_line = ""
    flags: dict[str, bool] = {}
    for line in read_text(AGENT_STATUS_TXT).splitlines():
        line = line.strip()
        if line:
            status_line = line
    if status_line:
        for tok in status_line.split():
            if "=" in tok:
                key, _, val = tok.partition("=")
                flags[key] = val.strip().lower() in ("true", "1", "yes", "busy", "active")

    # In-flight task lookup: latest 'claimed' status task per function.
    inflight: dict[str, dict[str, object]] = {}
    try:
        tasks = json.loads(TASKS_JSON.read_text(encoding="utf-8", errors="replace"))
    except (OSError, json.JSONDecodeError):
        tasks = []
    if isinstance(tasks, list):
        for t in tasks:
            if not isinstance(t, dict):
                continue
            if t.get("status") == "claimed":
                by = str(t.get("claimed_by") or "")
                if by:
                    inflight.setdefault(by, t)

    # claims.json: [{agent, function, claimed_at, task_id}]
    try:
        claims = json.loads(CLAIMS_JSON.read_text(encoding="utf-8", errors="replace"))
    except (OSError, json.JSONDecodeError):
        claims = []
    rows: list[dict[str, object]] = []
    if isinstance(claims, list):
        # newest claim per agent wins for the live table
        latest: dict[str, dict[str, object]] = {}
        for c in claims:
            if not isinstance(c, dict):
                continue
            agent = str(c.get("agent") or "")
            if not agent:
                continue
            prev = latest.get(agent)
            if prev is None or str(c.get("claimed_at", "")) >= str(prev.get("claimed_at", "")):
                latest[agent] = c
        for agent, c in latest.items():
            task = inflight.get(agent) or {}
            meta = task.get("meta", {}) if isinstance(task.get("meta"), dict) else {}
            # status flag: prefer the agent_status busy flags, else infer from inflight
            busy = flags.get(agent.split("-")[0], flags.get(agent, bool(task)))
            rows.append({
                "agent": agent,
                "function": str(c.get("function") or task.get("function") or ""),
                "file": str(meta.get("file") or ""),
                "claimed_at": c.get("claimed_at"),
                "task_status": str(task.get("status") or ""),
                "busy": bool(busy),
            })
    rows.sort(key=lambda r: str(r.get("claimed_at") or ""), reverse=True)

    # token usage (.omc/agent_tokens.json) merged in per agent when names match
    tokens = _load_json_obj(AGENT_TOKENS_JSON).get("agents", {})
    if isinstance(tokens, dict):
        for r in rows:
            tk = tokens.get(r["agent"])
            if isinstance(tk, dict):
                r["tokens_used"] = int_value(tk.get("tokens_used", 0))
                r["token_limit"] = int_value(tk.get("limit", 0))
                r["token_status"] = tk.get("status", "")

    return {
        "available": bool(rows) or bool(flags),
        "status_line": status_line,
        "flags": flags,
        "agents": rows,
        "queued": sum(1 for t in tasks if isinstance(t, dict) and t.get("status") == "queued") if isinstance(tasks, list) else 0,
        "claimed": sum(1 for t in tasks if isinstance(t, dict) and t.get("status") == "claimed") if isinstance(tasks, list) else 0,
        "generated_at": time.strftime("%Y-%m-%d %H:%M:%S"),
    }


# =========================================================================== #
# v9: token-expense-over-time (OpenCode session storage) + lockout schedule   #
# =========================================================================== #
_TOKENS_CACHE: dict[str, object] = {"value": None, "expires": 0.0}
_TOKENS_CACHE_TTL = 120.0


def load_tokens(hours: int = 72) -> dict[str, object]:
    """Bucket OpenCode assistant-message token usage into hourly samples.

    Source: <opencode>/storage/message/**/<msg>.json — each assistant message
    carries time.created (unix ms) + tokens.{input,output,reasoning,cache}. We
    sum input+output per UTC hour over the trailing `hours` window. This is a
    REAL data source (no collector required); see DASHBOARD UX notes for the
    per-model breakdown extension."""
    now = time.monotonic()
    cached = _TOKENS_CACHE.get("value")
    if cached is not None and now < float(_TOKENS_CACHE.get("expires", 0)):
        return cached  # type: ignore[return-value]

    msg_dir = OPENCODE_STORAGE / "message"
    if not msg_dir.exists():
        payload = {"available": False, "reason": f"opencode storage not found: {msg_dir}", "buckets": []}
        _TOKENS_CACHE["value"] = payload
        _TOKENS_CACHE["expires"] = now + _TOKENS_CACHE_TTL
        return payload

    cutoff_ms = (time.time() - hours * 3600) * 1000.0
    buckets: dict[int, dict[str, float]] = {}
    files = 0
    for path in msg_dir.rglob("*.json"):
        files += 1
        try:
            d = json.loads(path.read_text(encoding="utf-8", errors="replace"))
        except (OSError, json.JSONDecodeError):
            continue
        if d.get("role") != "assistant":
            continue
        tk = d.get("tokens")
        if not isinstance(tk, dict):
            continue
        created = ((d.get("time") or {}).get("created")) if isinstance(d.get("time"), dict) else None
        if not isinstance(created, (int, float)) or created < cutoff_ms:
            continue
        hour = int(created // 3600000) * 3600  # unix seconds, hour-aligned
        b = buckets.setdefault(hour, {"in": 0.0, "out": 0.0, "total": 0.0, "n": 0.0})
        b["in"] += float(tk.get("input") or 0)
        b["out"] += float(tk.get("output") or 0)
        b["total"] += float(tk.get("total") or (float(tk.get("input") or 0) + float(tk.get("output") or 0)))
        b["n"] += 1
    series = [
        {"unix": hour, "input": int(v["in"]), "output": int(v["out"]),
         "total": int(v["total"]), "messages": int(v["n"])}
        for hour, v in sorted(buckets.items())
    ]
    grand = int(sum(s["total"] for s in series))
    payload = {
        "available": bool(series),
        "buckets": series,
        "window_hours": hours,
        "grand_total": grand,
        "scanned_files": files,
        "source": str(msg_dir),
        "generated_at": time.strftime("%Y-%m-%d %H:%M:%S"),
    }
    _TOKENS_CACHE["value"] = payload
    _TOKENS_CACHE["expires"] = now + _TOKENS_CACHE_TTL
    return payload


def _parse_iso(ts: object) -> float:
    """Parse an ISO-8601 UTC string (trailing Z optional) -> unix seconds, or 0."""
    if not isinstance(ts, str) or not ts:
        return 0.0
    try:
        return time.mktime(time.strptime(ts.replace("Z", "").split(".")[0], "%Y-%m-%dT%H:%M:%S")) - time.timezone
    except (ValueError, OverflowError):
        return 0.0


def load_limits() -> dict[str, object]:
    """Read the user-maintained agent_limits.json and compute next-reset unix
    timestamps for the countdown panel."""
    data = _load_json_obj(AGENT_LIMITS_JSON)
    agents_in = data.get("agents", []) if isinstance(data.get("agents"), list) else []
    now = time.time()
    out = []
    for a in agents_in:
        if not isinstance(a, dict):
            continue
        next_unix = _parse_iso(a.get("next_reset"))
        if not next_unix:
            interval = a.get("reset_interval_hours")
            last = _parse_iso(a.get("last_reset"))
            if isinstance(interval, (int, float)) and interval and last:
                step = interval * 3600
                # roll forward from last_reset to the first reset strictly in the future
                k = max(0, int((now - last) // step) + 1)
                next_unix = last + k * step
        out.append({
            "name": a.get("name", ""),
            "label": a.get("label", a.get("name", "")),
            "kind": a.get("kind", ""),
            "reset_interval_hours": a.get("reset_interval_hours"),
            "next_reset_unix": int(next_unix) if next_unix else 0,
            "seconds_until": int(next_unix - now) if next_unix else 0,
            "note": a.get("note", ""),
        })
    return {
        "available": bool(out),
        "source": str(AGENT_LIMITS_JSON),
        "agents": out,
        "now_unix": int(now),
    }


HTML = r"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>GC6E01 Progress Control</title>
  <style>
    :root {
      color-scheme: dark;
      --bg: #0d1118;
      --band: #151c28;
      --panel: #182231;
      --panel-2: #111925;
      --ink: #eef4fb;
      --muted: #a8b4c4;
      --quiet: #728095;
      --line: #2d3a4b;
      --line-strong: #46556a;
      --amber: #f0b35a;
      --teal: #38b995;
      --cobalt: #5c91df;
      --red: #e07171;
      --violet: #a98ee6;
      --steel: #8da0b8;
      --shadow: 0 18px 42px rgba(0, 0, 0, .28);
      --radius: 8px;
    }
    * { box-sizing: border-box; }
    body {
      margin: 0;
      font-family: "Segoe UI", Arial, sans-serif;
      font-size: 14px;
      line-height: 1.35;
      letter-spacing: 0;
      background:
        linear-gradient(180deg, rgba(240, 179, 90, .08), transparent 220px),
        radial-gradient(circle at 12% -20%, rgba(56, 185, 149, .14), transparent 260px),
        var(--bg);
      color: var(--ink);
    }
    button, input, select {
      font: inherit;
      letter-spacing: 0;
    }
    button {
      cursor: pointer;
    }
    .topbar {
      display: grid;
      grid-template-columns: minmax(280px, 1fr) auto;
      gap: 18px;
      align-items: center;
      padding: 18px 24px;
      background: rgba(13, 17, 24, .9);
      border-bottom: 1px solid var(--line);
      position: sticky;
      top: 0;
      z-index: 20;
      backdrop-filter: blur(12px);
    }
    .hud-strip {
      display: grid;
      grid-template-columns: minmax(0, 1fr) auto;
      gap: 12px;
      align-items: center;
      margin-bottom: 12px;
      padding: 9px 12px;
      border: 1px solid var(--line);
      border-radius: var(--radius);
      background: #0f1722;
      box-shadow: var(--shadow);
    }
    .hud-stats {
      display: flex;
      flex-wrap: wrap;
      gap: 14px;
      align-items: baseline;
      color: var(--muted);
      font-family: Consolas, "Courier New", monospace;
      font-weight: 700;
      min-width: 0;
    }
    .hud-project {
      color: #f3f7fb;
      font-size: 15px;
    }
    .hud-value {
      color: #dfe8f4;
      font-size: 18px;
    }
    .hud-good { color: #58d889; }
    .hud-warn { color: #f0b35a; }
    .hud-bad { color: #e07171; }
    .hud-controls {
      display: flex;
      flex-wrap: wrap;
      gap: 8px;
      align-items: center;
      justify-content: flex-end;
      color: var(--muted);
      font-size: 12px;
    }
    .hud-controls label {
      display: inline-flex;
      gap: 6px;
      align-items: center;
      white-space: nowrap;
    }
    .hud-controls input[type="checkbox"] {
      width: 14px;
      height: 14px;
      min-width: 14px;
      padding: 0;
      accent-color: #38b995;
    }
    .hud-controls select {
      height: 28px;
      width: 74px;
      padding: 0 7px;
    }
    h1 {
      margin: 0;
      font-size: 22px;
      font-weight: 760;
    }
    .subtitle {
      margin-top: 4px;
      color: var(--muted);
      display: flex;
      flex-wrap: wrap;
      gap: 10px;
      font-size: 12px;
    }
    .actions {
      display: flex;
      flex-wrap: wrap;
      gap: 8px;
      justify-content: flex-end;
    }
    .btn {
      height: 36px;
      border-radius: 7px;
      border: 1px solid var(--line-strong);
      color: var(--ink);
      background: #1c2939;
      padding: 0 12px;
      font-weight: 700;
    }
    .btn.primary {
      background: #23684f;
      border-color: #2f9874;
    }
    .btn.ghost {
      background: transparent;
    }
    main {
      padding: 18px 24px 30px;
      max-width: 1680px;
      margin: 0 auto;
    }
    .metric-grid {
      display: grid;
      grid-template-columns: repeat(6, minmax(130px, 1fr));
      gap: 10px;
      margin-bottom: 12px;
    }
    .metric, .panel, .detail-panel {
      background: linear-gradient(180deg, rgba(255, 255, 255, .03), transparent), var(--panel);
      border: 1px solid var(--line);
      border-radius: var(--radius);
      box-shadow: var(--shadow);
    }
    .metric {
      min-height: 92px;
      padding: 12px;
      position: relative;
      overflow: hidden;
    }
    .metric::after {
      content: "";
      position: absolute;
      inset: auto 0 0 0;
      height: 3px;
      background: var(--accent, var(--steel));
    }
    .metric-label {
      color: var(--muted);
      font-size: 11px;
      font-weight: 760;
      text-transform: uppercase;
    }
    .metric-value {
      margin-top: 8px;
      font-size: 28px;
      font-weight: 780;
    }
    .metric-note {
      margin-top: 3px;
      color: var(--quiet);
      font-size: 12px;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }
    .overview {
      display: grid;
      grid-template-columns: minmax(300px, 1.15fr) minmax(300px, .85fr);
      gap: 12px;
      align-items: stretch;
      margin-bottom: 12px;
    }
    .panel {
      padding: 14px;
      min-width: 0;
    }
    .panel-title {
      display: flex;
      align-items: baseline;
      justify-content: space-between;
      gap: 10px;
      margin-bottom: 10px;
    }
    h2 {
      margin: 0;
      font-size: 14px;
      font-weight: 760;
      text-transform: uppercase;
      color: #dfe8f4;
    }
    .panel-note {
      color: var(--quiet);
      font-size: 12px;
    }
    .target-line {
      display: grid;
      grid-template-columns: auto 1fr auto;
      gap: 12px;
      align-items: center;
      padding: 12px;
      border-radius: var(--radius);
      background: #101824;
      border: 1px solid var(--line);
      margin-bottom: 12px;
    }
    .target-badge {
      display: grid;
      place-items: center;
      width: 50px;
      height: 50px;
      border-radius: 7px;
      color: #1a1204;
      background: var(--amber);
      font-weight: 800;
    }
    .target-name {
      font-size: 18px;
      font-weight: 780;
      overflow-wrap: anywhere;
    }
    .target-meta {
      margin-top: 3px;
      color: var(--muted);
      overflow-wrap: anywhere;
    }
    .progress-shell {
      height: 12px;
      border-radius: 999px;
      background: #0d131d;
      border: 1px solid #253243;
      overflow: hidden;
    }
    .progress-fill {
      height: 100%;
      width: 0;
      background: linear-gradient(90deg, var(--teal), var(--amber));
      transition: width .25s ease;
    }
    .chart-grid {
      display: grid;
      grid-template-columns: 1fr 1.2fr;
      gap: 12px;
      margin-bottom: 12px;
    }
    .ops-grid {
      display: grid;
      grid-template-columns: minmax(0, 1fr) minmax(320px, .85fr);
      gap: 12px;
      margin-bottom: 12px;
      align-items: stretch;
    }
    .feed {
      display: grid;
      gap: 8px;
      max-height: 265px;
      overflow: auto;
      padding-right: 4px;
    }
    .feed-row {
      display: grid;
      grid-template-columns: 92px 1fr auto;
      gap: 10px;
      align-items: start;
      padding: 8px 9px;
      border: 1px solid #263244;
      background: #101824;
      border-radius: 7px;
    }
    .feed-time {
      color: var(--quiet);
      font-size: 12px;
      font-family: Consolas, "Courier New", monospace;
      white-space: nowrap;
    }
    .feed-main {
      color: #dce7f3;
      overflow-wrap: anywhere;
    }
    .feed-tag {
      border-radius: 999px;
      padding: 2px 7px;
      background: rgba(56, 185, 149, .14);
      color: #93f0d2;
      font-size: 11px;
      font-weight: 780;
      text-transform: uppercase;
      white-space: nowrap;
    }
    .commit-list {
      display: grid;
      gap: 7px;
      max-height: 265px;
      overflow: auto;
      padding-right: 4px;
    }
    .commit-row {
      display: grid;
      grid-template-columns: 72px 92px 1fr;
      gap: 10px;
      align-items: start;
      padding: 8px 9px;
      border: 1px solid #263244;
      background: #101824;
      border-radius: 7px;
    }
    .commit-sha {
      color: #76a9ff;
      font-family: Consolas, "Courier New", monospace;
      font-weight: 780;
    }
    .commit-when {
      color: var(--quiet);
      font-family: Consolas, "Courier New", monospace;
      font-size: 12px;
      white-space: nowrap;
    }
    .commit-subject {
      color: #cdd8e6;
      overflow-wrap: anywhere;
    }
    .tu-panel {
      margin-bottom: 12px;
    }
    .tu-toolbar {
      display: grid;
      grid-template-columns: minmax(180px, 1fr) auto auto;
      gap: 8px;
      margin-bottom: 10px;
      align-items: center;
    }
    .tu-map {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(92px, 1fr));
      grid-auto-rows: 42px;
      grid-auto-flow: dense;
      gap: 5px;
      min-height: 240px;
      max-height: 430px;
      overflow: auto;
      border: 1px solid #263244;
      border-radius: var(--radius);
      background: #0f1722;
      padding: 8px;
    }
    .tu-tile {
      display: grid;
      align-content: start;
      gap: 3px;
      min-width: 0;
      border: 1px solid rgba(255, 255, 255, .14);
      border-radius: 4px;
      padding: 6px;
      color: #06100b;
      overflow: hidden;
      cursor: pointer;
      box-shadow: inset 0 0 0 1px rgba(0, 0, 0, .16);
    }
    .tu-name {
      font-family: Consolas, "Courier New", monospace;
      font-size: 12px;
      font-weight: 800;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }
    .tu-pct {
      font-size: 12px;
      font-weight: 780;
    }
    .tu-meta {
      color: rgba(6, 16, 11, .72);
      font-size: 11px;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }
    .chart-card {
      min-height: 260px;
    }
    canvas {
      width: 100%;
      height: 205px;
      display: block;
    }
    .legend {
      display: flex;
      flex-wrap: wrap;
      gap: 8px 14px;
      margin-top: 10px;
      color: var(--muted);
      font-size: 12px;
    }
    .legend-item {
      display: inline-flex;
      align-items: center;
      gap: 6px;
    }
    .swatch {
      width: 10px;
      height: 10px;
      border-radius: 2px;
      background: var(--swatch);
    }
    .workbench {
      display: grid;
      grid-template-columns: minmax(0, 1fr) 360px;
      gap: 12px;
      align-items: start;
    }
    .toolbar {
      display: grid;
      grid-template-columns: minmax(180px, 1fr) 170px minmax(140px, 220px) 132px;
      gap: 8px;
      margin-bottom: 10px;
    }
    input, select {
      height: 36px;
      min-width: 0;
      color: var(--ink);
      background: #101824;
      border: 1px solid var(--line);
      border-radius: 7px;
      padding: 0 10px;
    }
    input::placeholder {
      color: #78869a;
    }
    .table-wrap {
      overflow: auto;
      border: 1px solid var(--line);
      border-radius: var(--radius);
      background: var(--panel-2);
    }
    table {
      width: 100%;
      min-width: 1040px;
      border-collapse: collapse;
    }
    th, td {
      padding: 9px 10px;
      border-bottom: 1px solid #263244;
      text-align: left;
      vertical-align: middle;
      white-space: nowrap;
    }
    th {
      position: sticky;
      top: 0;
      z-index: 4;
      background: #202b3b;
      color: #c4cfdd;
      font-size: 11px;
      font-weight: 760;
      text-transform: uppercase;
      user-select: none;
    }
    th[data-sort] {
      cursor: pointer;
    }
    td.wrap {
      white-space: normal;
      min-width: 210px;
      overflow-wrap: anywhere;
    }
    tr {
      transition: background .12s ease;
    }
    tbody tr:hover {
      background: #1d2939;
    }
    tbody tr.selected {
      background: #26364a;
      outline: 1px solid #547298;
      outline-offset: -1px;
    }
    .mono {
      font-family: Consolas, "Courier New", monospace;
    }
    .pill {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      min-width: 104px;
      border-radius: 999px;
      padding: 3px 9px;
      font-size: 12px;
      font-weight: 780;
      border: 1px solid transparent;
    }
    .needs { background: rgba(240, 179, 90, .16); color: #ffd28a; border-color: rgba(240, 179, 90, .45); }
    .proposed { background: rgba(92, 145, 223, .16); color: #a9caff; border-color: rgba(92, 145, 223, .45); }
    .recorded { background: rgba(141, 160, 184, .16); color: #d1d9e4; border-color: rgba(141, 160, 184, .35); }
    .renamed { background: rgba(56, 185, 149, .16); color: #93f0d2; border-color: rgba(56, 185, 149, .48); }
    .bad {
      color: #ff9a9a;
      font-weight: 780;
    }
    .detail-panel {
      padding: 14px;
      position: sticky;
      top: 82px;
    }
    .detail-title {
      font-size: 16px;
      font-weight: 780;
      overflow-wrap: anywhere;
    }
    .detail-subtitle {
      margin: 4px 0 12px;
      color: var(--muted);
      overflow-wrap: anywhere;
    }
    .kv {
      display: grid;
      grid-template-columns: 112px 1fr;
      gap: 8px;
      padding: 7px 0;
      border-top: 1px solid #263244;
    }
    .kv:first-of-type {
      border-top: 0;
    }
    .kv .k {
      color: var(--quiet);
      font-size: 12px;
      text-transform: uppercase;
      font-weight: 760;
    }
    .kv .v {
      overflow-wrap: anywhere;
    }
    code {
      color: #dce8f5;
      background: #0c131d;
      border: 1px solid #253143;
      border-radius: 5px;
      padding: 2px 5px;
      font-family: Consolas, "Courier New", monospace;
      font-size: 12px;
    }
    .command-list {
      display: grid;
      gap: 6px;
      margin-top: 8px;
    }
    .source-bars {
      display: grid;
      gap: 8px;
      margin-top: 8px;
    }
    .bar-row {
      display: grid;
      grid-template-columns: minmax(90px, 1fr) 3fr 34px;
      gap: 8px;
      align-items: center;
      color: var(--muted);
      font-size: 12px;
    }
    .bar-name {
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }
    .bar-track {
      height: 8px;
      background: #0d131d;
      border-radius: 999px;
      overflow: hidden;
    }
    .bar-fill {
      height: 100%;
      background: var(--cobalt);
      border-radius: 999px;
    }
    .empty-state {
      color: var(--quiet);
      padding: 16px;
    }
    .tabs {
      display: inline-flex;
      gap: 6px;
      padding: 4px;
      margin-bottom: 12px;
      border: 1px solid var(--line);
      border-radius: var(--radius);
      background: #0f1722;
    }
    .tab-btn {
      height: 34px;
      min-width: 124px;
      padding: 0 14px;
      border: 0;
      border-radius: 6px;
      color: var(--muted);
      background: transparent;
      font-weight: 780;
    }
    .tab-btn.active {
      color: #06100b;
      background: linear-gradient(90deg, #39c95e, #f0b35a);
    }
    .view {
      display: none;
    }
    .view.active {
      display: block;
    }
    .decomp-workspace {
      display: grid;
      grid-template-columns: minmax(0, 1fr) minmax(360px, .42fr);
      gap: 12px;
      align-items: start;
      margin-bottom: 12px;
    }
    .decomp-map {
      max-height: 620px;
      grid-auto-rows: 48px;
    }
    .tu-tile.selected {
      outline: 2px solid #eef4fb;
      outline-offset: 1px;
    }
    .decomp-detail {
      max-height: 620px;
      overflow: auto;
    }
    .treemap-toolbar {
      display: grid;
      gap: 8px;
      margin-bottom: 10px;
    }
    .treemap-controls {
      display: grid;
      grid-template-columns: minmax(160px, 1fr) auto auto auto;
      gap: 8px;
      align-items: center;
    }
    .area-toggle {
      display: inline-flex;
      gap: 6px;
      align-items: center;
      color: var(--muted);
      font-size: 12px;
      white-space: nowrap;
    }
    .area-toggle input {
      width: 14px;
      height: 14px;
      min-width: 14px;
      accent-color: #38b995;
    }
    .crumbs {
      display: flex;
      flex-wrap: wrap;
      align-items: center;
      gap: 4px;
      color: var(--muted);
      font-size: 13px;
      min-height: 22px;
    }
    .crumbs button {
      border: 0;
      background: transparent;
      color: #76a9ff;
      font-weight: 760;
      padding: 0;
    }
    .crumbs button:disabled {
      color: #cdd8e6;
      cursor: default;
    }
    .crumbs .sep {
      color: var(--quiet);
    }
    .treemap-shell {
      position: relative;
      border: 1px solid #263244;
      border-radius: var(--radius);
      background: #0f1722;
      overflow: hidden;
    }
    #decomp-treemap {
      width: 100%;
      height: 560px;
      display: block;
      cursor: pointer;
    }
    .treemap-tip {
      position: absolute;
      z-index: 9;
      pointer-events: none;
      max-width: 280px;
      padding: 7px 9px;
      border: 1px solid var(--line-strong);
      border-radius: 6px;
      background: rgba(10, 15, 22, .96);
      color: var(--ink);
      font-size: 12px;
      box-shadow: var(--shadow);
    }
    .treemap-tip b {
      color: #fff;
      font-family: Consolas, "Courier New", monospace;
    }
    .files-toolbar {
      display: grid;
      grid-template-columns: minmax(180px, 1fr) auto auto;
      gap: 8px;
      margin-bottom: 10px;
      align-items: center;
    }
    th[data-fsort] { cursor: pointer; }
    th.num, td.num { text-align: right; font-family: Consolas, "Courier New", monospace; }
    tfoot td {
      font-weight: 800;
      background: #1a2433;
      border-top: 2px solid var(--line-strong);
      position: sticky;
      bottom: 0;
    }
    .mini-bar {
      display: inline-block;
      vertical-align: middle;
      width: 46px;
      height: 7px;
      margin-left: 6px;
      border-radius: 999px;
      background: #0d131d;
      overflow: hidden;
    }
    .mini-bar > span {
      display: block;
      height: 100%;
      background: var(--teal);
    }
    .decomp-title {
      font-size: 17px;
      font-weight: 800;
      overflow-wrap: anywhere;
    }
    .decomp-subtitle {
      margin: 4px 0 12px;
      color: var(--muted);
      overflow-wrap: anywhere;
    }
    .mini-stats {
      display: grid;
      grid-template-columns: repeat(4, minmax(0, 1fr));
      gap: 7px;
      margin-bottom: 12px;
    }
    .mini-stat {
      padding: 8px;
      border: 1px solid #263244;
      border-radius: 6px;
      background: #101824;
    }
    .mini-label {
      color: var(--quiet);
      font-size: 11px;
      font-weight: 760;
      text-transform: uppercase;
    }
    .mini-value {
      margin-top: 3px;
      font-weight: 800;
      font-family: Consolas, "Courier New", monospace;
    }
    .function-list,
    .attempt-list {
      display: grid;
      gap: 6px;
      max-height: 300px;
      overflow: auto;
      padding-right: 4px;
    }
    .function-row {
      display: grid;
      grid-template-columns: minmax(150px, 1fr) 72px 72px 82px;
      gap: 8px;
      align-items: center;
      padding: 7px 8px;
      border: 1px solid #263244;
      border-radius: 6px;
      background: #101824;
    }
    .function-name {
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
      font-family: Consolas, "Courier New", monospace;
      font-weight: 760;
    }
    .function-pct,
    .function-size {
      font-family: Consolas, "Courier New", monospace;
      color: #dce7f3;
      text-align: right;
    }
    .status-chip {
      justify-self: end;
      border-radius: 999px;
      padding: 2px 7px;
      font-size: 11px;
      font-weight: 780;
      text-transform: uppercase;
      border: 1px solid rgba(255, 255, 255, .16);
    }
    .status-chip.matched { background: rgba(56, 185, 149, .18); color: #93f0d2; }
    .status-chip.near { background: rgba(240, 179, 90, .18); color: #ffd28a; }
    .status-chip.partial { background: rgba(92, 145, 223, .18); color: #a9caff; }
    .status-chip.missing { background: rgba(224, 113, 113, .18); color: #ffaaaa; }
    .attempt-row {
      display: grid;
      grid-template-columns: 162px 78px 1fr;
      gap: 9px;
      align-items: start;
      padding: 8px 9px;
      border: 1px solid #263244;
      background: #101824;
      border-radius: 7px;
    }
    .attempt-time,
    .attempt-kind {
      color: var(--quiet);
      font-family: Consolas, "Courier New", monospace;
      font-size: 12px;
      white-space: nowrap;
    }
    .attempt-message {
      color: #dce7f3;
      overflow-wrap: anywhere;
    }
    .history-layout {
      display: grid;
      grid-template-columns: minmax(0, 1fr) minmax(320px, .72fr);
      gap: 12px;
      margin-bottom: 12px;
    }
    /* ---- v9: decomp.me-style function reader ------------------------------ */
    .reader-overlay {
      display: none;
      margin-bottom: 12px;
    }
    .reader-overlay.active { display: block; }
    .reader-head {
      display: flex;
      flex-wrap: wrap;
      align-items: center;
      gap: 12px;
      margin-bottom: 12px;
    }
    .reader-fn {
      font-family: Consolas, "Courier New", monospace;
      font-size: 18px;
      font-weight: 800;
      color: #eef4fb;
    }
    .reader-pct {
      font-family: Consolas, "Courier New", monospace;
      font-size: 22px;
      font-weight: 800;
    }
    .reader-pct.good { color: #58d889; }
    .reader-pct.near { color: #f0b35a; }
    .reader-pct.bad { color: #e07171; }
    .reader-meta {
      color: var(--muted);
      font-size: 12px;
      font-family: Consolas, "Courier New", monospace;
    }
    .reader-grid {
      display: grid;
      grid-template-columns: minmax(0, 1.6fr) minmax(260px, .6fr);
      gap: 12px;
      align-items: start;
    }
    .asm-pane {
      border: 1px solid var(--line);
      border-radius: var(--radius);
      background: #0b1018;
      overflow: hidden;
    }
    .asm-colhead {
      display: grid;
      grid-template-columns: 36px 1fr 1fr;
      gap: 0;
      position: sticky;
      top: 0;
      z-index: 2;
      background: #1b2535;
      border-bottom: 1px solid var(--line-strong);
    }
    .asm-colhead span {
      padding: 7px 10px;
      font-size: 11px;
      font-weight: 800;
      text-transform: uppercase;
      color: #c4cfdd;
      letter-spacing: .04em;
    }
    .asm-colhead .target { border-right: 1px solid var(--line); }
    .asm-body {
      max-height: 560px;
      overflow: auto;
      font-family: Consolas, "Courier New", monospace;
      font-size: 12.5px;
      line-height: 1.5;
    }
    .asm-line {
      display: grid;
      grid-template-columns: 36px 1fr 1fr;
      border-bottom: 1px solid rgba(38, 50, 68, .5);
    }
    .asm-line:hover { background: rgba(92, 145, 223, .07); }
    .asm-num {
      color: #4d5b70;
      text-align: right;
      padding: 1px 6px;
      user-select: none;
      background: rgba(0, 0, 0, .18);
    }
    .asm-cell {
      padding: 1px 10px;
      white-space: pre;
      overflow: hidden;
      text-overflow: ellipsis;
    }
    .asm-cell.target { border-right: 1px solid rgba(45, 58, 75, .8); }
    .asm-line.same .asm-cell { color: #b9c6d6; }
    .asm-line.diff { background: rgba(224, 113, 113, .10); }
    .asm-line.diff .asm-cell.current { color: #ff9a9a; }
    .asm-line.diff .asm-cell.target { color: #ffd28a; }
    .asm-line.addrm { background: rgba(240, 179, 90, .09); }
    .asm-line.addrm .asm-cell { color: #c69152; }
    .asm-legend {
      display: flex;
      flex-wrap: wrap;
      gap: 8px 14px;
      margin-top: 10px;
      color: var(--muted);
      font-size: 12px;
    }
    .reader-side { display: grid; gap: 12px; }
    .wall-card {
      border: 1px solid var(--line);
      border-radius: var(--radius);
      background: var(--panel-2);
      padding: 12px;
    }
    .wall-banner {
      display: inline-flex;
      align-items: center;
      gap: 7px;
      padding: 5px 10px;
      border-radius: 999px;
      font-size: 12px;
      font-weight: 800;
      margin-bottom: 8px;
    }
    .wall-banner.wall { background: rgba(224, 113, 113, .16); color: #ffaaaa; border: 1px solid rgba(224, 113, 113, .4); }
    .wall-banner.equiv { background: rgba(169, 142, 230, .16); color: #d6c6ff; border: 1px solid rgba(169, 142, 230, .4); }
    .wall-banner.clear { background: rgba(56, 185, 149, .14); color: #93f0d2; border: 1px solid rgba(56, 185, 149, .35); }
    .wall-note {
      color: #cdd8e6;
      font-size: 12.5px;
      line-height: 1.5;
      overflow-wrap: anywhere;
    }
    .agent-table {
      width: 100%;
      border-collapse: collapse;
      font-size: 12.5px;
    }
    .agent-table th, .agent-table td {
      padding: 7px 9px;
      border-bottom: 1px solid #263244;
      text-align: left;
      white-space: nowrap;
    }
    .agent-table th { color: #c4cfdd; font-size: 11px; text-transform: uppercase; }
    .agent-dot {
      display: inline-block;
      width: 8px; height: 8px;
      border-radius: 999px;
      margin-right: 6px;
      vertical-align: middle;
    }
    .agent-dot.busy { background: #58d889; box-shadow: 0 0 6px #58d889; }
    .agent-dot.idle { background: #5b6a80; }
    .limit-grid {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(160px, 1fr));
      gap: 10px;
    }
    .limit-card {
      border: 1px solid #263244;
      border-radius: 7px;
      background: #101824;
      padding: 11px;
    }
    .limit-label { color: var(--muted); font-size: 11px; font-weight: 760; text-transform: uppercase; }
    .limit-countdown {
      margin-top: 6px;
      font-family: Consolas, "Courier New", monospace;
      font-size: 22px;
      font-weight: 800;
      color: #f0b35a;
    }
    .limit-countdown.soon { color: #e07171; }
    .limit-note { margin-top: 4px; color: var(--quiet); font-size: 11px; }
    .ops3-grid {
      display: grid;
      grid-template-columns: minmax(0, 1.3fr) minmax(280px, .7fr);
      gap: 12px;
      margin-bottom: 12px;
      align-items: start;
    }
    @media (max-width: 1180px) {
      .metric-grid { grid-template-columns: repeat(3, minmax(130px, 1fr)); }
      .overview, .chart-grid, .ops-grid, .ops3-grid, .workbench, .decomp-workspace, .history-layout, .reader-grid { grid-template-columns: 1fr; }
      .detail-panel { position: static; }
    }
    @media (max-width: 760px) {
      .topbar { grid-template-columns: 1fr; padding: 14px 12px; }
      .hud-strip { grid-template-columns: 1fr; }
      .actions { justify-content: stretch; }
      .actions .btn { flex: 1 1 auto; }
      main { padding: 12px; }
      .metric-grid { grid-template-columns: repeat(2, minmax(120px, 1fr)); }
      .toolbar { grid-template-columns: 1fr; }
      .tu-toolbar { grid-template-columns: 1fr; }
      .target-line { grid-template-columns: 1fr; }
      .feed-row, .commit-row, .attempt-row, .function-row { grid-template-columns: 1fr; }
      .mini-stats { grid-template-columns: repeat(2, minmax(0, 1fr)); }
    }
  </style>
</head>
<body>
  <header class="topbar">
    <div>
      <h1>GC6E01 Progress Control</h1>
      <div class="subtitle">
        <span id="repo"></span>
        <span id="head"></span>
        <span id="updated"></span>
      </div>
    </div>
    <div class="actions">
      <button class="btn primary" id="refresh" type="button">Refresh</button>
    </div>
  </header>
  <main>
    <section class="hud-strip">
      <div class="hud-stats">
        <span class="hud-project" id="hud-project">GC6E01/DECOMP</span>
        <span><span class="hud-value" id="hud-completion">0%</span> complete</span>
        <span><span class="hud-value hud-good" id="hud-fuzzy">0%</span> fuzzy</span>
        <span><span class="hud-value" id="hud-code">0%</span> code</span>
        <span><span class="hud-value hud-good" id="hud-fns">0%</span> fns</span>
        <span><span class="hud-value" id="hud-units">0/0</span> units</span>
        <span><span class="hud-value hud-good" id="hud-renamed">0</span> renamed</span>
        <span><span class="hud-value hud-warn" id="hud-active">0</span> active</span>
        <span><span class="hud-value hud-bad" id="hud-oldrefs">0</span> old refs</span>
        <span id="hud-next">next: -</span>
      </div>
      <div class="hud-controls">
        <label><input id="auto-refresh" type="checkbox" checked> live</label>
        <span>every</span>
        <select id="refresh-rate">
          <option value="5000">5 sec</option>
          <option value="15000">15 sec</option>
          <option value="30000">30 sec</option>
        </select>
      </div>
    </section>

    <nav class="tabs" aria-label="Dashboard views">
      <button class="tab-btn active" id="tab-decomp" type="button" data-view="decomp">Decomp</button>
      <button class="tab-btn" id="tab-files" type="button" data-view="files">Files</button>
      <button class="tab-btn" id="tab-symbols" type="button" data-view="symbols">Symbol Map</button>
    </nav>

    <section class="view active" id="view-decomp">
      <section class="metric-grid" id="decomp-metrics"></section>

      <section class="history-layout">
        <div class="panel chart-card">
          <div class="panel-title">
            <h2>Match Progress Over Time</h2>
            <span class="panel-note" id="timeline-range"></span>
          </div>
          <canvas id="history-chart" height="205"></canvas>
        </div>
        <div class="panel chart-card">
          <div class="panel-title">
            <h2>Selected File Progress</h2>
            <span class="panel-note" id="file-history-note"></span>
          </div>
          <canvas id="file-history-chart" height="205"></canvas>
        </div>
      </section>

      <section class="decomp-workspace">
        <div class="panel tu-panel">
          <div class="panel-title">
            <h2>Treemap</h2>
            <span class="panel-note" id="decomp-note"></span>
          </div>
          <div class="treemap-toolbar">
            <nav class="crumbs" id="decomp-crumbs" aria-label="Treemap breadcrumb"></nav>
            <div class="treemap-controls">
              <input id="decomp-query" type="search" placeholder="Filter file or source">
              <label class="area-toggle"><input id="decomp-area-fns" type="checkbox"> area by fn count</label>
              <button class="btn" id="decomp-near" type="button">Near Match</button>
              <button class="btn" id="decomp-clear" type="button">Clear</button>
            </div>
          </div>
          <div class="treemap-shell">
            <canvas id="decomp-treemap"></canvas>
            <div class="treemap-tip" id="decomp-tip" hidden></div>
          </div>
        </div>

        <aside class="detail-panel decomp-detail" id="decomp-details"></aside>
      </section>

      <section class="panel reader-overlay" id="reader-overlay">
        <div class="panel-title">
          <h2>Function Reader</h2>
          <button class="btn ghost" id="reader-back" type="button">&#8592; Back to treemap</button>
        </div>
        <div class="reader-head">
          <span class="reader-fn" id="reader-fn">fn_</span>
          <span class="reader-pct" id="reader-pct">--%</span>
          <span class="reader-meta" id="reader-meta"></span>
        </div>
        <div class="reader-grid">
          <div>
            <div class="asm-pane">
              <div class="asm-colhead">
                <span></span>
                <span class="target">Target (aim for)</span>
                <span class="current">Current build</span>
              </div>
              <div class="asm-body" id="asm-body"></div>
            </div>
            <div class="asm-legend">
              <span class="legend-item"><span class="swatch" style="--swatch:#5c91df"></span>match</span>
              <span class="legend-item"><span class="swatch" style="--swatch:#e07171"></span>differs</span>
              <span class="legend-item"><span class="swatch" style="--swatch:#f0b35a"></span>insert / delete</span>
            </div>
          </div>
          <aside class="reader-side" id="reader-side"></aside>
        </div>
      </section>

      <section class="panel">
        <div class="panel-title">
          <h2>Decomp Attempt Log</h2>
          <span class="panel-note" id="decomp-log-note"></span>
        </div>
        <div class="attempt-list" id="decomp-log"></div>
      </section>

      <section class="ops3-grid">
        <div class="panel">
          <div class="panel-title">
            <h2>Agent Activity</h2>
            <span class="panel-note" id="agents-note"></span>
          </div>
          <div class="table-wrap">
            <table class="agent-table">
              <thead>
                <tr><th>Agent</th><th>Function</th><th>File</th><th>Claimed</th><th>State</th></tr>
              </thead>
              <tbody id="agents-body"></tbody>
            </table>
          </div>
        </div>
        <div class="panel">
          <div class="panel-title">
            <h2>Lockout Resets</h2>
            <span class="panel-note" id="limits-note"></span>
          </div>
          <div class="limit-grid" id="limits-grid"></div>
        </div>
      </section>

      <section class="panel chart-card">
        <div class="panel-title">
          <h2>Token Expense Over Time</h2>
          <span class="panel-note" id="tokens-note"></span>
        </div>
        <canvas id="tokens-chart" height="205"></canvas>
      </section>
    </section>

    <section class="view" id="view-files">
      <section class="panel">
        <div class="panel-title">
          <h2>Translation Units &amp; File Sizes</h2>
          <span class="panel-note" id="files-note"></span>
        </div>
        <div class="files-toolbar">
          <input id="files-query" type="search" placeholder="Filter source path">
          <label class="area-toggle"><input id="files-incomplete" type="checkbox"> only incomplete</label>
          <button class="btn" id="files-clear" type="button">Clear</button>
        </div>
        <div class="table-wrap">
          <table id="files-table">
            <thead>
              <tr>
                <th data-fsort="source">Source Path</th>
                <th data-fsort="total_code" class="num">Bytes</th>
                <th data-fsort="matched_code" class="num">Matched</th>
                <th data-fsort="code_pct" class="num">Code %</th>
                <th data-fsort="total_functions" class="num">Fns</th>
                <th data-fsort="matched_functions" class="num">Matched Fns</th>
                <th data-fsort="functions_pct" class="num">Fns %</th>
                <th data-fsort="fuzzy_pct" class="num">Fuzzy %</th>
                <th data-fsort="complete">Complete</th>
              </tr>
            </thead>
            <tbody id="files-body"></tbody>
            <tfoot><tr id="files-foot"></tr></tfoot>
          </table>
        </div>
      </section>
    </section>

    <section class="view" id="view-symbols">
      <section class="metric-grid" id="symbol-metrics"></section>

      <section class="overview">
        <div class="panel">
          <div class="panel-title">
            <h2>Current Target</h2>
            <span class="panel-note" id="progress-label"></span>
          </div>
          <div class="target-line">
            <div class="target-badge" id="next-rank">NEXT</div>
            <div>
              <div class="target-name mono" id="next-name"></div>
              <div class="target-meta" id="next-detail"></div>
            </div>
            <span class="pill needs" id="next-status">Needs wiring</span>
          </div>
          <div class="progress-shell"><div class="progress-fill" id="progress-fill"></div></div>
          <div class="legend" id="status-legend"></div>
        </div>
        <div class="panel">
          <div class="panel-title">
            <h2>Evidence Mix</h2>
            <span class="panel-note" id="history-points"></span>
          </div>
          <canvas id="provenance-chart" height="205"></canvas>
          <div class="source-bars" id="source-bars"></div>
        </div>
      </section>

      <section class="panel chart-card">
        <div class="panel-title">
          <h2>Status Distribution</h2>
          <span class="panel-note" id="status-total"></span>
        </div>
        <canvas id="status-chart" height="205"></canvas>
      </section>

      <section class="ops-grid">
        <div class="panel">
          <div class="panel-title">
            <h2>Live Activity</h2>
            <span class="panel-note" id="activity-note"></span>
          </div>
          <div class="feed" id="activity-feed"></div>
        </div>
        <div class="panel">
          <div class="panel-title">
            <h2>Recent Commits</h2>
            <span class="panel-note" id="commit-note"></span>
          </div>
          <div class="commit-list" id="commit-list"></div>
        </div>
      </section>

      <section class="panel tu-panel">
        <div class="panel-title">
          <h2>Translation Units</h2>
          <span class="panel-note" id="tu-note"></span>
        </div>
        <div class="tu-toolbar">
          <input id="tu-query" type="search" placeholder="Filter TU name or source">
          <button class="btn" id="tu-needs" type="button">Needs Wiring</button>
          <button class="btn" id="tu-clear" type="button">Clear</button>
        </div>
        <div class="tu-map" id="tu-map"></div>
      </section>

      <section class="workbench">
        <div class="panel">
          <div class="panel-title">
            <h2>Targets</h2>
            <span class="panel-note" id="row-count"></span>
          </div>
          <div class="toolbar">
            <input id="query" type="search" placeholder="Filter symbol, source, evidence">
            <select id="status-filter">
              <option value="">All statuses</option>
              <option value="Needs wiring">Needs wiring</option>
              <option value="Proposed">Proposed</option>
              <option value="Recorded">Recorded</option>
              <option value="Renamed">Renamed</option>
            </select>
            <select id="source-filter">
              <option value="">All sources</option>
            </select>
            <button class="btn" id="clear" type="button">Clear</button>
          </div>
          <div class="table-wrap">
            <table>
              <thead>
                <tr>
                  <th data-sort="status">Status</th>
                  <th data-sort="fn">Function</th>
                  <th data-sort="name">Name</th>
                  <th data-sort="size">Size</th>
                  <th data-sort="source">Source</th>
                  <th data-sort="current_symbol">Current</th>
                  <th data-sort="old_refs">Old Refs</th>
                  <th data-sort="provenance">Evidence</th>
                </tr>
              </thead>
              <tbody id="targets"></tbody>
            </table>
          </div>
        </div>

        <aside class="detail-panel" id="details"></aside>
      </section>
    </section>
  </main>
  <script>
    const store = {
      data: null,
      rows: [],
      filtered: [],
      selectedFn: null,
      sortKey: "",
      sortDir: 1,
      attention: false,
      tuNeedsOnly: false,
      decompNearOnly: false,
      selectedUnitKey: "",
      activeView: "decomp",
      refreshTimer: null,
      // Treemap drill state. level: "files" -> "unit" -> "fn".
      tm: {
        level: "files",
        rects: [],
        unitSource: "",
        unitName: "",
        unitFns: null,
        unitFnsSource: "",
        selectedFn: "",
        areaByFns: false
      },
      filesSort: "total_code",
      filesDir: -1,
      fnHistoryCache: {},
      // v9: function reader + agent/lockout/token panels
      reader: { open: false, fn: "", source: "" },
      limits: [],
      agentsTimer: null,
      limitsTimer: null
    };
    const statusClass = {
      "Needs wiring": "needs",
      "Proposed": "proposed",
      "Recorded": "recorded",
      "Renamed": "renamed"
    };
    const palette = {
      "Needs wiring": "#f0b35a",
      "Proposed": "#5c91df",
      "Recorded": "#8da0b8",
      "Renamed": "#38b995",
      "XD port": "#38b995",
      "String evidence": "#f0b35a",
      "Structural": "#5c91df",
      "Other": "#a98ee6"
    };
    function $(id) {
      return document.getElementById(id);
    }
    function setText(node, value) {
      node.textContent = value == null || value === "" ? "-" : String(value);
    }
    function number(value) {
      const text = String(value || "0");
      const parsed = text.toLowerCase().startsWith("0x") ? parseInt(text, 16) : Number(text);
      return Number.isFinite(parsed) ? parsed : 0;
    }
    function shortSource(source) {
      if (!source) return "unknown";
      const normalized = String(source).replaceAll("\\", "/");
      return normalized.split("/").pop().replace(/\.c$/, "");
    }
    function tileColor(percent) {
      const pct = Number(percent || 0);
      if (pct >= 99.95) return "#39c95e";
      if (pct >= 90) return "#2f9b48";
      if (pct >= 70) return "#78901b";
      if (pct > 0) return "#c68b25";
      return "#8a2525";
    }
    function pctText(value) {
      return `${Number(value || 0).toFixed(1)}%`;
    }
    function unitKey(row) {
      return `${row.name || ""}|${row.source || ""}`;
    }
    function fileName(source) {
      if (!source) return "unknown";
      return String(source).replaceAll("\\", "/").split("/").pop();
    }
    function unitDisplayName(row) {
      return fileName(row.source) !== "unknown" ? fileName(row.source) : (row.name || "unknown");
    }
    function functionStatusLabel(status) {
      if (status === "matched") return "100";
      if (status === "near") return "near";
      if (status === "partial") return "partial";
      return "missing";
    }
    function statusChip(status) {
      const chip = document.createElement("span");
      chip.className = `status-chip ${status || "missing"}`;
      setText(chip, functionStatusLabel(status));
      return chip;
    }
    function metric(label, value, note, color) {
      const box = document.createElement("div");
      box.className = "metric";
      box.style.setProperty("--accent", color);
      const labelNode = document.createElement("div");
      labelNode.className = "metric-label";
      setText(labelNode, label);
      const valueNode = document.createElement("div");
      valueNode.className = "metric-value";
      setText(valueNode, value);
      const noteNode = document.createElement("div");
      noteNode.className = "metric-note";
      setText(noteNode, note);
      box.append(labelNode, valueNode, noteNode);
      return box;
    }
    function pill(status) {
      const el = document.createElement("span");
      el.className = `pill ${statusClass[status] || "recorded"}`;
      setText(el, status);
      return el;
    }
    function td(value, className) {
      const cell = document.createElement("td");
      if (className) cell.className = className;
      setText(cell, value);
      return cell;
    }
    function code(value) {
      const node = document.createElement("code");
      setText(node, value);
      return node;
    }
    function fitCanvas(canvas) {
      const rect = canvas.getBoundingClientRect();
      const ratio = window.devicePixelRatio || 1;
      const width = Math.max(240, Math.floor(rect.width * ratio));
      const height = Math.max(180, Math.floor(rect.height * ratio));
      if (canvas.width !== width || canvas.height !== height) {
        canvas.width = width;
        canvas.height = height;
      }
      const ctx = canvas.getContext("2d");
      ctx.setTransform(ratio, 0, 0, ratio, 0, 0);
      return { ctx, w: width / ratio, h: height / ratio };
    }
    function drawEmpty(ctx, w, h, label) {
      ctx.clearRect(0, 0, w, h);
      ctx.fillStyle = "#728095";
      ctx.font = "12px Segoe UI, Arial";
      ctx.textAlign = "center";
      ctx.fillText(label, w / 2, h / 2);
    }
    function drawDonut(canvas, items) {
      const { ctx, w, h } = fitCanvas(canvas);
      ctx.clearRect(0, 0, w, h);
      const total = items.reduce((sum, row) => sum + Number(row.value || 0), 0);
      if (!total) return drawEmpty(ctx, w, h, "No target data");
      const cx = w * .38;
      const cy = h * .52;
      const radius = Math.min(w, h) * .34;
      const inner = radius * .58;
      let angle = -Math.PI / 2;
      for (const item of items) {
        const value = Number(item.value || 0);
        if (!value) continue;
        const next = angle + (Math.PI * 2 * value / total);
        ctx.beginPath();
        ctx.moveTo(cx, cy);
        ctx.arc(cx, cy, radius, angle, next);
        ctx.closePath();
        ctx.fillStyle = palette[item.label] || "#a98ee6";
        ctx.fill();
        angle = next;
      }
      ctx.globalCompositeOperation = "destination-out";
      ctx.beginPath();
      ctx.arc(cx, cy, inner, 0, Math.PI * 2);
      ctx.fill();
      ctx.globalCompositeOperation = "source-over";
      ctx.fillStyle = "#eef4fb";
      ctx.font = "700 24px Segoe UI, Arial";
      ctx.textAlign = "center";
      ctx.fillText(String(total), cx, cy + 5);
      ctx.fillStyle = "#a8b4c4";
      ctx.font = "12px Segoe UI, Arial";
      ctx.fillText("targets", cx, cy + 25);
      let y = 26;
      ctx.textAlign = "left";
      for (const item of items) {
        ctx.fillStyle = palette[item.label] || "#a98ee6";
        ctx.fillRect(w * .68, y - 9, 10, 10);
        ctx.fillStyle = "#cbd5e3";
        ctx.font = "12px Segoe UI, Arial";
        ctx.fillText(`${item.label}: ${item.value}`, w * .68 + 16, y);
        y += 22;
      }
    }
    function drawBars(canvas, items) {
      const { ctx, w, h } = fitCanvas(canvas);
      ctx.clearRect(0, 0, w, h);
      const max = Math.max(...items.map(row => Number(row.value || 0)), 0);
      if (!max) return drawEmpty(ctx, w, h, "No evidence data");
      const left = 108;
      const right = 20;
      const top = 18;
      const barH = 18;
      const gap = 12;
      ctx.font = "12px Segoe UI, Arial";
      items.forEach((item, idx) => {
        const y = top + idx * (barH + gap);
        const width = (w - left - right) * Number(item.value || 0) / max;
        ctx.fillStyle = "#a8b4c4";
        ctx.textAlign = "right";
        ctx.fillText(item.label, left - 10, y + 13);
        ctx.fillStyle = "#0d131d";
        ctx.fillRect(left, y, w - left - right, barH);
        ctx.fillStyle = palette[item.label] || "#a98ee6";
        ctx.fillRect(left, y, width, barH);
        ctx.fillStyle = "#eef4fb";
        ctx.textAlign = "left";
        ctx.fillText(String(item.value), left + width + 6, y + 13);
      });
    }
    // ---- Unified time-series chart: fixed elapsed-hours x-axis (anchored at 0,
    //      does not scroll), % gridline labels on the Y axis, and a hover
    //      crosshair + tooltip. Drives BOTH the global match-progress chart and
    //      the per-file/per-fn selected-file chart so they read the same. ------
    function _chartTip() {
      let t = document.getElementById("chart-tip");
      if (!t) {
        t = document.createElement("div");
        t.id = "chart-tip";
        t.style.cssText = "position:fixed;z-index:60;pointer-events:none;display:none;" +
          "background:#0d131d;border:1px solid #2d3a4b;border-radius:6px;padding:6px 9px;" +
          "font:12px Segoe UI,Arial;color:#eef4fb;box-shadow:0 4px 14px rgba(0,0,0,.5)";
        document.body.appendChild(t);
      }
      return t;
    }
    function _bindChartHover(canvas) {
      if (canvas._hoverBound) return;
      canvas._hoverBound = true;
      canvas.addEventListener("mousemove", evt => {
        const c = canvas._chart;
        const tip = _chartTip();
        if (!c || !c.rows.length) { tip.style.display = "none"; return; }
        const rect = canvas.getBoundingClientRect();
        const mx = evt.clientX - rect.left;
        if (mx < c.pad.l - 6 || mx > c.w - c.pad.r + 6) { tip.style.display = "none"; return; }
        let bi = 0, bd = 1e9;
        c.rows.forEach((r, i) => { const d = Math.abs(c.x(Number(r.unix)) - mx); if (d < bd) { bd = d; bi = i; } });
        const r = c.rows[bi];
        const lines = c.active.map(it => {
          const v = _seriesVal(r, it.key);
          return `<span style="color:${it.color}">&#9632;</span> ${it.label}: ${Number.isFinite(v) ? v.toFixed(2) + "%" : "-"}`;
        }).join("<br>");
        tip.innerHTML = `<b>${c.fmtH(r.unix)}</b> &middot; ${fmtTime(r.unix)}<br>${lines}`;
        tip.style.display = "block";
        tip.style.left = (evt.clientX + 14) + "px";
        tip.style.top = (evt.clientY + 14) + "px";
      });
      canvas.addEventListener("mouseleave", () => { _chartTip().style.display = "none"; });
    }
    // A row's value for a series is finite-or-GAP: backfilled history rows only
    // carry decomp_* keys, so completion_pct/symbols are missing there. Treat a
    // missing/non-finite raw value as a gap (NaN) -> skip the point, break the
    // line -- never plot it as 0 (which would drag the line down to the axis).
    function _seriesVal(row, key) {
      const raw = row == null ? undefined : row[key];
      const n = Number(raw);
      return (raw === null || raw === undefined || !Number.isFinite(n)) ? NaN : n;
    }
    function _drawTimeChart(canvas, rows, series, emptyLabel) {
      const { ctx, w, h } = fitCanvas(canvas);
      ctx.clearRect(0, 0, w, h);
      rows = (rows || []).filter(r => r && Number.isFinite(Number(r.unix)));
      // Keep a series only if it has at least one finite point somewhere.
      const active = series.filter(it => rows.some(r => Number.isFinite(_seriesVal(r, it.key))));
      if (rows.length < 2 || !active.length) { canvas._chart = null; return drawEmpty(ctx, w, h, emptyLabel || "No history yet"); }
      const pad = { l: 46, r: 16, t: 18, b: 30 };
      const xs = rows.map(r => Number(r.unix || 0));
      const ys = rows.flatMap(r => active.map(it => _seriesVal(r, it.key))).filter(Number.isFinite);
      const minX = Math.min(...xs), maxX = Math.max(...xs);
      const maxY = Math.max(100, Math.ceil((ys.length ? Math.max(...ys) : 0) / 10) * 10);
      // FIXED scale: min = first row unix, max = last row unix (does not scroll).
      const x = v => pad.l + (w - pad.l - pad.r) * (v - minX) / Math.max(1, maxX - minX);
      const y = v => h - pad.b - (h - pad.t - pad.b) * v / Math.max(1, maxY);
      // Y gridlines + percent labels at every step (read the % off the axis)
      const ystep = maxY <= 100 ? 20 : (maxY <= 200 ? 25 : 50);
      ctx.lineWidth = 1;
      for (let v = 0; v <= maxY + 0.01; v += ystep) {
        const gy = y(v);
        ctx.strokeStyle = v === 0 ? "#3a4a5e" : "#2d3a4b";
        ctx.beginPath(); ctx.moveTo(pad.l, gy); ctx.lineTo(w - pad.r, gy); ctx.stroke();
        ctx.fillStyle = "#8da0b8"; ctx.font = "11px Segoe UI, Arial"; ctx.textAlign = "right";
        ctx.fillText(v + "%", pad.l - 6, gy + 4);
      }
      // series lines + points -- break the line at gaps (NaN), don't plot 0.
      active.forEach((it, si) => {
        ctx.strokeStyle = it.color; ctx.lineWidth = si === 0 ? 2.5 : 2; ctx.beginPath();
        let pen = false;
        rows.forEach(r => {
          const val = _seriesVal(r, it.key);
          if (!Number.isFinite(val)) { pen = false; return; }
          const px = x(Number(r.unix)), py = y(val);
          if (pen) ctx.lineTo(px, py); else { ctx.moveTo(px, py); pen = true; }
        });
        ctx.stroke(); ctx.fillStyle = it.color;
        rows.forEach(r => {
          const val = _seriesVal(r, it.key);
          if (!Number.isFinite(val)) return;
          ctx.beginPath(); ctx.arc(x(Number(r.unix)), y(val), 2.2, 0, 6.2832); ctx.fill();
        });
      });
      // legend
      let lx = pad.l; ctx.textAlign = "left"; ctx.font = "12px Segoe UI, Arial";
      active.forEach(it => { ctx.fillStyle = it.color; ctx.fillRect(lx, pad.t - 12, 9, 9); ctx.fillStyle = "#cbd5e3"; ctx.fillText(it.label, lx + 13, pad.t - 3); lx += 64; });
      // x-axis: 6-hour-aligned ticks. <=48h span -> elapsed-hours labels; longer
      // (days/months) -> HST date labels at a readable cadence (~8-12 max).
      const fmtH = ux => { const hr = (Number(ux) - minX) / 3600; return hr < 1 ? Math.round(hr * 60) + "m" : (hr < 10 ? hr.toFixed(1) : String(Math.round(hr))) + "h"; };
      const SIXH = 6 * 3600;
      const spanH = (maxX - minX) / 3600;
      ctx.fillStyle = "#7c8aa0"; ctx.font = "11px Segoe UI, Arial"; ctx.textAlign = "center";
      if (spanH <= 48) {
        // Elapsed hours, ticks every 6h from the origin.
        for (let ux = minX; ux <= maxX + 1; ux += SIXH) ctx.fillText(fmtH(ux), x(ux), h - 8);
        ctx.fillStyle = "#6b7686"; ctx.font = "10px Segoe UI, Arial"; ctx.textAlign = "right";
        ctx.fillText("hours since " + fmtTime(rows[0].unix), w - pad.r, pad.t - 3);
      } else {
        // Date labels in HST. Choose a 6h-multiple step so we get <=12 ticks.
        const slots = Math.ceil((maxX - minX) / SIXH);
        const stepSlots = Math.max(1, Math.ceil(slots / 11));
        const step = stepSlots * SIXH;
        // Snap the first tick up to the next 6h UTC boundary so ticks align.
        const first = Math.ceil(minX / SIXH) * SIXH;
        const dateLbl = ux => new Date(Number(ux) * 1000).toLocaleString("en-US", { timeZone: "Pacific/Honolulu", month: "numeric", day: "numeric" });
        for (let ux = first; ux <= maxX + 1; ux += step) ctx.fillText(dateLbl(ux), x(ux), h - 8);
        ctx.fillStyle = "#6b7686"; ctx.font = "10px Segoe UI, Arial"; ctx.textAlign = "right";
        ctx.fillText(dateLbl(minX) + " -> " + dateLbl(maxX) + " HST", w - pad.r, pad.t - 3);
      }
      canvas._chart = { rows, active, x, y, pad, w, h, fmtH };
      _bindChartHover(canvas);
    }
    function drawHistory(canvas, history) {
      _drawTimeChart(canvas, history, [
        { key: "completion_pct", label: "symbols", color: "#38b995" },
        { key: "decomp_functions_pct", label: "fns", color: "#f0b35a" },
        { key: "decomp_code_pct", label: "code", color: "#5c91df" },
        { key: "decomp_fuzzy_pct", label: "fuzzy", color: "#a98ee6" }
      ], "Timeline starts with the next snapshot");
    }
    function relatedAttempts(data, unit) {
      if (!unit) return data.attempt_log || [];
      const sourceFile = fileName(unit.source);
      const shortName = shortSource(unit.source || unit.name);
      const fnSet = new Set((unit.functions || []).map(row => row.name).filter(Boolean));
      return (data.attempt_log || []).filter(row => {
        const rowFile = fileName(row.file || "");
        if (rowFile !== "unknown" && sourceFile !== "unknown" && rowFile === sourceFile) return true;
        if (rowFile !== "unknown" && rowFile.replace(/\.c$/, "") === shortName) return true;
        if (row.function && fnSet.has(row.function)) return true;
        return false;
      });
    }
    // HST formatting (Pacific/Honolulu, UTC-10). Accepts a unix-seconds NUMBER
    // or an ISO-UTC STRING ("2026-06-14T11:00:06Z"). Returns "" for empty input.
    function hstTime(v) {
      let ms = NaN;
      if (typeof v === "number") {
        ms = v * 1000;
      } else if (typeof v === "string" && v) {
        const n = Number(v);
        ms = Number.isFinite(n) && /^\d+$/.test(v.trim()) ? n * 1000 : Date.parse(v);
      }
      if (!Number.isFinite(ms) || !ms) return typeof v === "string" ? v : "";
      return new Date(ms).toLocaleString("en-US", {
        timeZone: "Pacific/Honolulu", month: "numeric", day: "numeric",
        hour: "2-digit", minute: "2-digit", hour12: false
      });
    }
    function fmtTime(unix) {
      const ms = Number(unix || 0) * 1000;
      if (!ms) return "";
      return new Date(ms).toLocaleString("en-US", {
        timeZone: "Pacific/Honolulu", month: "numeric", day: "numeric",
        hour: "2-digit", minute: "2-digit", hour12: false
      });
    }
    // Real time-series over unix time. series = [{key,label,color}], rows from
    // the new /api/history/unit or /api/history/fn endpoints.
    function drawTimeSeries(canvas, rows, series, emptyLabel) {
      _drawTimeChart(canvas, rows, series, emptyLabel || "No history recorded yet");
    }
    function renderMetrics(data) {
      const decomp = data.decomp || {};
      $("decomp-metrics").replaceChildren(
        metric("Decomp Fns", pctText(decomp.functions_pct), `${decomp.matched_functions || 0}/${decomp.total_functions || 0} functions at 100%`, "#f0b35a"),
        metric("Decomp Code", pctText(decomp.code_pct), `${(decomp.matched_code || 0).toLocaleString()}/${(decomp.total_code || 0).toLocaleString()} bytes`, "#5c91df"),
        metric("Fuzzy Match", pctText(decomp.fuzzy_pct), "weighted instruction similarity", "#a98ee6"),
        metric("Complete Units", `${decomp.complete_units || 0}/${decomp.total_units || 0}`, "report.json decomp units", "#38b995"),
        metric("Attempt Log", `${(data.attempt_log || []).length}`, "coordination status entries retained", "#8da0b8"),
        metric("Report Updated", decomp.updated_at || "unknown", "mtime for report.json", "#38b995")
      );
      $("symbol-metrics").replaceChildren(
        metric("Completion", `${data.metrics.completion_pct}%`, `${data.metrics.wired_targets}/${data.counts.targets} recorded or renamed`, "#38b995"),
        metric("Needs Wiring", data.counts.by_status["Needs wiring"] || 0, "confirmed leads blocking clean rename", "#f0b35a"),
        metric("Renamed", data.counts.by_status.Renamed || 0, `${data.metrics.rename_pct}% fully wired`, "#5c91df"),
        metric("Recorded", data.counts.by_status.Recorded || 0, "evidence captured", "#8da0b8"),
        metric("Old Fn Refs", data.metrics.old_ref_total, "live references to unresolved fn_ names", "#e07171"),
        metric("Known Bytes", data.metrics.known_size_bytes.toLocaleString(), "tracked symbol sizes", "#a98ee6")
      );
    }
    function renderHud(data) {
      const next = data.next_target;
      const decomp = data.decomp || {};
      setText($("hud-completion"), pctText(data.metrics.completion_pct));
      setText($("hud-fuzzy"), pctText(decomp.fuzzy_pct));
      setText($("hud-code"), pctText(decomp.code_pct));
      setText($("hud-fns"), pctText(decomp.functions_pct));
      setText($("hud-units"), `${decomp.complete_units || 0}/${decomp.total_units || 0}`);
      setText($("hud-renamed"), data.counts.by_status.Renamed || 0);
      setText($("hud-active"), data.metrics.active_targets || 0);
      setText($("hud-oldrefs"), data.metrics.old_ref_total || 0);
      setText($("hud-next"), next ? `next: ${next.fn} -> ${next.name}` : "next: none");
    }
    function renderTop(data) {
      const row = data.next_target;
      setText($("repo"), data.repo);
      setText($("head"), `${data.branch || "detached"} @ ${data.head || "unknown"}`);
      setText($("updated"), `Updated ${data.generated_at}`);
      setText($("next-name"), row ? `${row.fn} -> ${row.name}` : "No pending wiring leads");
      setText($("next-detail"), row ? `${row.source || "unknown source"} | ${row.size || "unknown size"} | ${row.provenance || "no provenance"}` : "All confirmed leads have been wired");
      setText($("progress-label"), `${data.metrics.completion_pct}% complete`);
      $("progress-fill").style.width = `${Math.max(0, Math.min(100, Number(data.metrics.completion_pct || 0)))}%`;
      $("next-status").replaceWith(pill(row ? row.status : "Renamed"));
      const nextStatus = document.querySelector(".target-line .pill");
      nextStatus.id = "next-status";
    }
    function renderLegend(data) {
      const legend = $("status-legend");
      legend.replaceChildren();
      for (const item of data.charts.status) {
        const entry = document.createElement("span");
        entry.className = "legend-item";
        const swatch = document.createElement("span");
        swatch.className = "swatch";
        swatch.style.setProperty("--swatch", palette[item.label] || "#a98ee6");
        entry.append(swatch, document.createTextNode(`${item.label}: ${item.value}`));
        legend.append(entry);
      }
      setText($("status-total"), `${data.counts.targets} tracked rows`);
      setText($("history-points"), `${(data.history || []).length} snapshots`);
      const history = data.history || [];
      const range = history.length ? `${history[0].timestamp} to ${history[history.length - 1].timestamp}` : "no snapshots yet";
      setText($("timeline-range"), range);
    }
    function renderSourceFilters(rows) {
      const select = $("source-filter");
      const previous = select.value;
      const sources = [...new Set(rows.map(row => row.source || "unknown"))].sort();
      select.replaceChildren(new Option("All sources", ""));
      for (const source of sources) {
        select.append(new Option(source, source));
      }
      if (sources.includes(previous)) {
        select.value = previous;
      }
    }
    function filteredRows() {
      const q = $("query").value.trim().toLowerCase();
      const status = store.attention ? "Needs wiring" : $("status-filter").value;
      const source = $("source-filter").value;
      let rows = store.rows.filter(row => {
        const haystack = `${row.fn} ${row.name} ${row.source} ${row.provenance} ${row.header} ${row.current_symbol}`.toLowerCase();
        if (q && !haystack.includes(q)) return false;
        if (status && row.status !== status) return false;
        if (source && (row.source || "unknown") !== source) return false;
        return true;
      });
      if (store.sortKey) {
        const key = store.sortKey;
        const dir = store.sortDir;
        rows = rows.slice().sort((a, b) => {
          const av = key === "size" || key === "old_refs" ? number(a[key]) : String(a[key] || "").toLowerCase();
          const bv = key === "size" || key === "old_refs" ? number(b[key]) : String(b[key] || "").toLowerCase();
          if (av < bv) return -1 * dir;
          if (av > bv) return 1 * dir;
          return 0;
        });
      }
      return rows;
    }
    function renderRows() {
      const body = $("targets");
      const rows = filteredRows();
      store.filtered = rows;
      setText($("row-count"), `${rows.length}/${store.rows.length} visible`);
      body.replaceChildren();
      if (!rows.length) {
        const tr = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 8;
        cell.className = "empty-state";
        setText(cell, "No targets match the current filters");
        tr.append(cell);
        body.append(tr);
        return;
      }
      if (!store.selectedFn || !store.rows.some(row => row.fn === store.selectedFn)) {
        store.selectedFn = rows[0].fn;
      }
      for (const row of rows) {
        const tr = document.createElement("tr");
        tr.tabIndex = 0;
        tr.dataset.fn = row.fn;
        if (row.fn === store.selectedFn) tr.className = "selected";
        const statusCell = document.createElement("td");
        statusCell.append(pill(row.status));
        tr.append(
          statusCell,
          td(row.fn, "mono"),
          td(row.name, "mono"),
          td(row.size, "mono"),
          td(row.source, "wrap"),
          td(row.current_symbol, "mono"),
          td(row.old_refs, row.old_refs !== "0" && row.status !== "Renamed" ? "bad mono" : "mono"),
          td(row.provenance, "wrap")
        );
        tr.addEventListener("click", () => {
          store.selectedFn = row.fn;
          renderRows();
          renderDetails(row);
        });
        tr.addEventListener("keydown", event => {
          if (event.key === "Enter" || event.key === " ") {
            event.preventDefault();
            store.selectedFn = row.fn;
            renderRows();
            renderDetails(row);
          }
        });
        body.append(tr);
      }
      renderDetails(rows.find(row => row.fn === store.selectedFn) || rows[0]);
    }
    function kv(label, value) {
      const row = document.createElement("div");
      row.className = "kv";
      const k = document.createElement("div");
      k.className = "k";
      setText(k, label);
      const v = document.createElement("div");
      v.className = "v";
      if (value instanceof Node) v.append(value);
      else setText(v, value);
      row.append(k, v);
      return row;
    }
    function renderDetails(row) {
      const panel = $("details");
      panel.replaceChildren();
      if (!row) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "No selected target");
        panel.append(empty);
        return;
      }
      const title = document.createElement("div");
      title.className = "detail-title mono";
      setText(title, `${row.fn} -> ${row.name}`);
      const subtitle = document.createElement("div");
      subtitle.className = "detail-subtitle";
      setText(subtitle, row.source || "unknown source");
      const status = document.createElement("div");
      status.append(pill(row.status));
      const commands = document.createElement("div");
      commands.className = "command-list";
      commands.append(
        code(`python tools\\match_test.py ${row.fn} --verbose`),
        code(`rg -n "${row.fn}|${row.name}" src include config\\GC6E01`)
      );
      panel.append(
        title,
        subtitle,
        kv("Status", status),
        kv("Address", row.addr),
        kv("Size", row.size || "unknown"),
        kv("Current", row.current_symbol || "not in symbols.txt"),
        kv("Old refs", row.old_refs),
        kv("Evidence", row.provenance),
        kv("Header", row.header),
        kv("Commands", commands)
      );
    }
    function renderSourceBars(data) {
      const bars = $("source-bars");
      bars.replaceChildren();
      const sources = data.charts.sources || [];
      const max = Math.max(...sources.map(row => Number(row.value || 0)), 1);
      for (const row of sources) {
        const line = document.createElement("div");
        line.className = "bar-row";
        const name = document.createElement("div");
        name.className = "bar-name";
        setText(name, row.label);
        const track = document.createElement("div");
        track.className = "bar-track";
        const fill = document.createElement("div");
        fill.className = "bar-fill";
        fill.style.width = `${Number(row.value || 0) * 100 / max}%`;
        track.append(fill);
        const value = document.createElement("div");
        setText(value, row.value);
        line.append(name, track, value);
        bars.append(line);
      }
    }
    function renderActivity(data) {
      const feed = $("activity-feed");
      feed.replaceChildren();
      const history = data.history || [];
      const rows = [];
      const next = data.next_target;
      if (next) {
        rows.push({
          time: data.generated_at,
          tag: "focus",
          text: `${next.fn} -> ${next.name} | ${next.source || "unknown source"}`
        });
      } else {
        rows.push({ time: data.generated_at, tag: "clear", text: "No confirmed-name leads are currently waiting for wiring." });
      }
      for (let idx = history.length - 1; idx >= 0 && rows.length < 8; idx--) {
        const row = history[idx];
        const prev = idx > 0 ? history[idx - 1] : null;
        const delta = prev ? Number(row.completion_pct || 0) - Number(prev.completion_pct || 0) : 0;
        const tag = delta > 0 ? "gain" : "tick";
        rows.push({
          time: Number.isFinite(Number(row.unix)) ? hstTime(Number(row.unix)) : (row.timestamp || ""),
          tag,
          text: `${pctText(row.completion_pct)} complete, ${row.renamed || 0} renamed, ${row.needs_wiring || 0} needs wiring${delta > 0 ? `, +${delta.toFixed(1)}%` : ""}`
        });
      }
      for (const row of rows) {
        const item = document.createElement("div");
        item.className = "feed-row";
        const time = document.createElement("div");
        time.className = "feed-time";
        setText(time, row.time);
        const main = document.createElement("div");
        main.className = "feed-main";
        setText(main, row.text);
        const tag = document.createElement("div");
        tag.className = "feed-tag";
        setText(tag, row.tag);
        item.append(time, main, tag);
        feed.append(item);
      }
      setText($("activity-note"), history.length ? `${history.length} retained snapshots` : "waiting for snapshots");
    }
    function renderCommits(data) {
      const list = $("commit-list");
      list.replaceChildren();
      const commits = data.recent_commits || [];
      if (!commits.length) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "No recent commits available");
        list.append(empty);
      }
      for (const commit of commits) {
        const row = document.createElement("div");
        row.className = "commit-row";
        const sha = document.createElement("div");
        sha.className = "commit-sha";
        setText(sha, commit.sha);
        const when = document.createElement("div");
        when.className = "commit-when";
        setText(when, Number.isFinite(Number(commit.unix)) && Number(commit.unix) > 0 ? hstTime(Number(commit.unix)) : commit.when);
        const subject = document.createElement("div");
        subject.className = "commit-subject";
        setText(subject, commit.subject);
        row.append(sha, when, subject);
        list.append(row);
      }
      setText($("commit-note"), `${commits.length} commits`);
    }
    function filteredTuTiles(data) {
      const q = $("tu-query").value.trim().toLowerCase();
      return (data.tu_tiles || []).filter(row => {
        const haystack = `${row.label} ${row.source}`.toLowerCase();
        if (q && !haystack.includes(q)) return false;
        if (store.tuNeedsOnly && Number(row.needs_wiring || 0) <= 0) return false;
        return true;
      });
    }
    function renderSymbolTreemap(data) {
      const map = $("tu-map");
      map.replaceChildren();
      const tiles = filteredTuTiles(data);
      setText($("tu-note"), `${tiles.length}/${(data.tu_tiles || []).length} source groups`);
      if (!tiles.length) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "No translation units match the current filter");
        map.append(empty);
        return;
      }
      const maxTargets = Math.max(...tiles.map(row => Number(row.targets || 0)), 1);
      for (const row of tiles) {
        const targets = Number(row.targets || 0);
        const pct = Number(row.completion_pct || 0);
        const span = Math.max(1, Math.min(5, Math.ceil(Math.sqrt(targets / maxTargets) * 5)));
        const tile = document.createElement("button");
        tile.type = "button";
        tile.className = "tu-tile";
        tile.style.background = tileColor(pct);
        tile.style.gridColumnEnd = `span ${span}`;
        tile.style.gridRowEnd = `span ${Math.max(1, Math.min(3, Math.ceil(span * .72)))}`;
        tile.title = `${row.source} | ${row.wired}/${row.targets} wired`;
        const name = document.createElement("div");
        name.className = "tu-name";
        setText(name, row.label || shortSource(row.source));
        const pctNode = document.createElement("div");
        pctNode.className = "tu-pct";
        setText(pctNode, pctText(pct));
        const meta = document.createElement("div");
        meta.className = "tu-meta";
        setText(meta, `${row.wired}/${row.targets} wired | ${row.needs_wiring || 0} needs`);
        tile.append(name, pctNode, meta);
        tile.addEventListener("click", () => {
          $("source-filter").value = row.source || "unknown";
          store.attention = false;
          $("attention")?.classList.remove("primary");
          renderRows();
          document.querySelector(".workbench").scrollIntoView({ behavior: "smooth", block: "start" });
        });
        map.append(tile);
      }
    }
    // ---- Squarified treemap (decomp.dev-style, single canvas) ----------------
    function lerp(a, b, t) { return a + (b - a) * t; }
    // Green = matched fraction, blue = unmatched. Continuous interpolation keyed
    // on match%, matching the user's decomp.dev screenshot palette.
    function tmColor(pct) {
      const t = Math.max(0, Math.min(1, Number(pct || 0) / 100));
      // unmatched (blue #3a5fa8) -> matched (green #2f9b48)
      const r = Math.round(lerp(0x3a, 0x2f, t));
      const g = Math.round(lerp(0x5f, 0x9b, t));
      const b = Math.round(lerp(0xa8, 0x48, t));
      return `rgb(${r},${g},${b})`;
    }
    // Squarified treemap layout. items: [{value, ...}] -> sets item._rect.
    function squarify(items, x, y, w, h) {
      items = items.filter(it => Number(it.value || 0) > 0);
      const total = items.reduce((s, it) => s + Number(it.value || 0), 0);
      if (total <= 0 || w <= 0 || h <= 0) return;
      const scale = (w * h) / total;
      const nodes = items.map(it => ({ ref: it, area: Number(it.value || 0) * scale }))
        .sort((a, b) => b.area - a.area);
      let rx = x, ry = y, rw = w, rh = h;
      let idx = 0;
      const worst = (row, side) => {
        const sum = row.reduce((s, n) => s + n.area, 0);
        const maxA = Math.max(...row.map(n => n.area));
        const minA = Math.min(...row.map(n => n.area));
        const s2 = side * side;
        const sum2 = sum * sum;
        return Math.max((s2 * maxA) / sum2, sum2 / (s2 * minA));
      };
      while (idx < nodes.length) {
        const vertical = rw >= rh;
        const side = vertical ? rh : rw;
        const row = [nodes[idx]];
        let j = idx + 1;
        while (j < nodes.length) {
          const test = row.concat([nodes[j]]);
          if (worst(test, side) > worst(row, side)) break;
          row.push(nodes[j]);
          j += 1;
        }
        const rowArea = row.reduce((s, n) => s + n.area, 0);
        const thick = rowArea / side;
        let off = vertical ? ry : rx;
        for (const n of row) {
          const len = n.area / thick;
          if (vertical) {
            n.ref._rect = { x: rx, y: off, w: thick, h: len };
            off += len;
          } else {
            n.ref._rect = { x: off, y: ry, w: len, h: thick };
            off += len;
          }
        }
        if (vertical) { rx += thick; rw -= thick; } else { ry += thick; rh -= thick; }
        idx = j;
      }
    }
    function treemapItems() {
      const tm = store.tm;
      const data = store.data || {};
      const q = $("decomp-query").value.trim().toLowerCase();
      const areaFns = tm.areaByFns;
      if (tm.level === "files") {
        let units = ((data.decomp || {}).units || []).filter(row => {
          const haystack = `${row.name} ${row.source}`.toLowerCase();
          if (q && !haystack.includes(q)) return false;
          if (store.decompNearOnly && !(Number(row.functions_pct || 0) >= 90 && Number(row.functions_pct || 0) < 100)) return false;
          return true;
        });
        return units.map(u => ({
          kind: "unit",
          ref: u,
          label: unitDisplayName(u),
          value: areaFns ? Number(u.total_functions || 0) : Number(u.total_code || 0),
          pct: Number(u.code_pct || 0),
          tip: `<b>${unitDisplayName(u)}</b><br>${pctText(u.code_pct)} code | ${u.matched_functions || 0}/${u.total_functions || 0} fns<br>${Number(u.total_code || 0).toLocaleString()} bytes`
        }));
      }
      // unit level -> functions
      const fns = (tm.unitFns || []).filter(fn => {
        if (q && !String(fn.name || "").toLowerCase().includes(q)) return false;
        if (store.decompNearOnly && !(Number(fn.fuzzy_pct || 0) >= 90 && Number(fn.fuzzy_pct || 0) < 100)) return false;
        return true;
      });
      return fns.map(fn => ({
        kind: "fn",
        ref: fn,
        label: fn.name || "",
        value: Math.max(1, Number(fn.size || 0)),
        pct: Number(fn.fuzzy_pct || 0),
        tip: `<b>${fn.name || ""}</b><br>${pctText(fn.fuzzy_pct)} fuzzy | ${Number(fn.size || 0).toLocaleString()} bytes`
      }));
    }
    function renderTreemap() {
      const canvas = $("decomp-treemap");
      const decomp = (store.data || {}).decomp || {};
      const tm = store.tm;
      const items = treemapItems();
      if (tm.level === "files") {
        setText($("decomp-note"), decomp.available ? `${items.length}/${((decomp.units) || []).length} files | ${pctText(decomp.code_pct)} code` : "report.json not available");
      } else {
        const matched = items.filter(it => it.pct >= 99.95).length;
        setText($("decomp-note"), `${items.length} fns | ${matched} at 100%`);
      }
      const { ctx, w, h } = fitCanvas(canvas);
      ctx.clearRect(0, 0, w, h);
      tm.rects = [];
      if (!items.length) {
        return drawEmpty(ctx, w, h, decomp.available ? "No items match the current filter" : "report.json not available");
      }
      squarify(items, 1, 1, w - 2, h - 2);
      ctx.font = "11px Consolas, 'Courier New', monospace";
      ctx.textBaseline = "top";
      for (const it of items) {
        const r = it._rect;
        if (!r || r.w < 0.5 || r.h < 0.5) continue;
        tm.rects.push({ x: r.x, y: r.y, w: r.w, h: r.h, item: it });
        ctx.fillStyle = tmColor(it.pct);
        ctx.fillRect(r.x, r.y, r.w, r.h);
        ctx.strokeStyle = "rgba(13,17,24,.75)";
        ctx.lineWidth = 1;
        ctx.strokeRect(r.x + 0.5, r.y + 0.5, Math.max(0, r.w - 1), Math.max(0, r.h - 1));
        if (r.w > 46 && r.h > 18) {
          ctx.save();
          ctx.beginPath();
          ctx.rect(r.x + 3, r.y + 2, r.w - 6, r.h - 4);
          ctx.clip();
          // Light label on the bluer (low-pct) end, dark on the greener end.
          ctx.fillStyle = it.pct >= 55 ? "rgba(8,14,11,.92)" : "rgba(238,244,251,.96)";
          ctx.fillText(it.label, r.x + 4, r.y + 3);
          if (r.h > 32) ctx.fillText(pctText(it.pct), r.x + 4, r.y + 16);
          ctx.restore();
        }
      }
    }
    function treemapHit(evt) {
      const canvas = $("decomp-treemap");
      const rect = canvas.getBoundingClientRect();
      const px = (evt.clientX - rect.left);
      const py = (evt.clientY - rect.top);
      for (const r of store.tm.rects) {
        if (px >= r.x && px <= r.x + r.w && py >= r.y && py <= r.y + r.h) return r;
      }
      return null;
    }
    function onTreemapMove(evt) {
      const tip = $("decomp-tip");
      const hit = treemapHit(evt);
      if (!hit) { tip.hidden = true; return; }
      tip.hidden = false;
      tip.innerHTML = hit.item.tip;
      const shell = $("decomp-treemap").parentElement.getBoundingClientRect();
      // Measure the tip now that its content is set, then flip it above/left of
      // the cursor when it would overflow the (overflow:hidden) shell — without
      // this, tooltips on bottom/right tiles render off-canvas and stay invisible.
      const tw = tip.offsetWidth || 280;
      const th = tip.offsetHeight || 60;
      let left = evt.clientX - shell.left + 14;
      let top = evt.clientY - shell.top + 14;
      if (left + tw > shell.width) left = evt.clientX - shell.left - tw - 14;
      if (top + th > shell.height) top = evt.clientY - shell.top - th - 14;
      tip.style.left = `${Math.max(0, Math.min(left, shell.width - tw))}px`;
      tip.style.top = `${Math.max(0, Math.min(top, shell.height - th))}px`;
    }
    function onTreemapClick(evt) {
      const hit = treemapHit(evt);
      if (!hit) return;
      const it = hit.item;
      if (it.kind === "unit") {
        enterUnit(it.ref);
      } else if (it.kind === "fn") {
        enterFn(it.ref);
      }
    }
    function enterUnit(unit) {
      const tm = store.tm;
      tm.level = "unit";
      tm.unitSource = unit.source || "";
      tm.unitName = unitDisplayName(unit);
      tm.selectedFn = "";
      tm.unitFns = null;
      const wantSource = unit.source || unit.name || "";
      const url = `/api/unit?source=${encodeURIComponent(wantSource)}`;
      fetch(url, { cache: "no-store" }).then(r => r.json()).then(payload => {
        if (store.tm.level !== "unit" || store.tm.unitSource !== (unit.source || "")) return;
        store.tm.unitFns = (payload.functions || []);
        store.tm.unitFnsSource = wantSource;
        renderTreemap();
        renderDecompDetail(store.data);
      }).catch(() => {
        store.tm.unitFns = (unit.functions || []);
        renderTreemap();
        renderDecompDetail(store.data);
      });
      renderCrumbs();
      renderDecompDetail(store.data);
      renderTreemap();
    }
    function enterFn(fn) {
      store.tm.selectedFn = fn.name || "";
      renderCrumbs();
      renderDecompDetail(store.data);
      openReader(fn.name || "");
    }
    // ---- v9: decomp.me-style function reader --------------------------------
    function closeReader() {
      store.reader.open = false;
      $("reader-overlay").classList.remove("active");
    }
    function openReader(fnName) {
      if (!fnName) return;
      const source = store.tm.unitFnsSource || store.tm.unitSource || store.tm.unitName || "";
      store.reader = { open: true, fn: fnName, source };
      const overlay = $("reader-overlay");
      overlay.classList.add("active");
      overlay.scrollIntoView({ behavior: "smooth", block: "start" });
      setText($("reader-fn"), fnName);
      setText($("reader-pct"), "...");
      $("reader-pct").className = "reader-pct";
      setText($("reader-meta"), "compiling + diffing (first load ~5-8s)...");
      const body = $("asm-body");
      body.replaceChildren();
      const wait = document.createElement("div");
      wait.className = "empty-state";
      setText(wait, "Compiling " + shortSource(source) + ".c and running objdiff...");
      body.append(wait);
      $("reader-side").replaceChildren();
      // Fire wall-info and asm-diff in parallel.
      loadFnInfo(fnName);
      const url = `/api/asm?source=${encodeURIComponent(source)}&fn=${encodeURIComponent(fnName)}`;
      fetch(url, { cache: "no-store" }).then(r => r.json()).then(payload => {
        if (!store.reader.open || store.reader.fn !== fnName) return;
        renderReaderAsm(payload);
      }).catch(() => {
        if (!store.reader.open || store.reader.fn !== fnName) return;
        renderReaderAsm({ available: false, error: "request failed" });
      });
    }
    function renderReaderAsm(payload) {
      const body = $("asm-body");
      body.replaceChildren();
      if (!payload || !payload.available) {
        const err = document.createElement("div");
        err.className = "empty-state";
        setText(err, (payload && payload.error) ? `Reader unavailable: ${payload.error}` : "No diff data available.");
        body.append(err);
        setText($("reader-pct"), "--%");
        $("reader-pct").className = "reader-pct bad";
        setText($("reader-meta"), payload && payload.source ? payload.source : "");
        return;
      }
      const pct = Number(payload.fuzzy_pct || 0);
      setText($("reader-pct"), pct.toFixed(2) + "%");
      $("reader-pct").className = "reader-pct " + (pct >= 99.95 ? "good" : pct >= 90 ? "near" : "bad");
      setText($("reader-meta"), `${payload.source} | ${payload.target_obj || ""} | ${payload.matched || 0}/${payload.total || 0} instr match`);
      const rows = payload.rows || [];
      if (!rows.length) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "objdiff returned no instructions for this symbol.");
        body.append(empty);
        return;
      }
      const frag = document.createDocumentFragment();
      rows.forEach((row, idx) => {
        const line = document.createElement("div");
        line.className = `asm-line ${row.state || "same"}`;
        const num = document.createElement("div");
        num.className = "asm-num";
        num.textContent = String(idx + 1);
        const target = document.createElement("div");
        target.className = "asm-cell target";
        target.textContent = row.l || "";
        const current = document.createElement("div");
        current.className = "asm-cell current";
        current.textContent = row.r || "";
        line.append(num, target, current);
        frag.append(line);
      });
      body.append(frag);
    }
    function loadFnInfo(fnName) {
      fetch(`/api/fninfo?fn=${encodeURIComponent(fnName)}`, { cache: "no-store" })
        .then(r => r.json())
        .then(info => {
          if (!store.reader.open || store.reader.fn !== fnName) return;
          renderFnInfo(info);
        })
        .catch(() => {});
    }
    function renderFnInfo(info) {
      const side = $("reader-side");
      side.replaceChildren();
      const card = document.createElement("div");
      card.className = "wall-card";
      const banner = document.createElement("div");
      const hasWall = info.wall_class || info.in_cs_walls;
      if (info.is_equivalent) {
        banner.className = "wall-banner equiv";
        setText(banner, "Logged Equivalent");
      } else if (hasWall) {
        banner.className = "wall-banner wall";
        setText(banner, `Known wall${info.wall_class ? ": " + info.wall_class : ""}`);
      } else {
        banner.className = "wall-banner clear";
        setText(banner, "No wall logged");
      }
      card.append(banner);
      if (info.wall_class || info.in_cs_walls) {
        const tags = document.createElement("div");
        tags.className = "reader-meta";
        const bits = [];
        if (info.wall_class) bits.push(info.wall_class);
        if (info.in_cs_walls) bits.push("cs_walls.json");
        if (info.is_equivalent) bits.push("equivalent.txt");
        setText(tags, bits.join(" | "));
        tags.style.marginBottom = "8px";
        card.append(tags);
      }
      if (info.note) {
        const note = document.createElement("div");
        note.className = "wall-note";
        setText(note, info.note);
        card.append(note);
      } else if (!hasWall && !info.is_equivalent) {
        const note = document.createElement("div");
        note.className = "wall-note";
        setText(note, "Not in WALLS.md, equivalent.txt, or cs_walls.json. If this fn is below 100%, it is an open target.");
        card.append(note);
      }
      side.append(card);
      // history chart for the fn (reuse existing per-fn endpoint via canvas)
      const histCard = document.createElement("div");
      histCard.className = "wall-card";
      const histTitle = document.createElement("div");
      histTitle.className = "limit-label";
      setText(histTitle, `${info.attempt_count || 0} logged attempts`);
      histCard.append(histTitle);
      if ((info.attempts || []).length) {
        const list = document.createElement("div");
        list.className = "function-list";
        list.style.maxHeight = "180px";
        list.style.marginTop = "8px";
        for (const a of info.attempts.slice().reverse().slice(0, 12)) {
          const row = document.createElement("div");
          row.className = "attempt-row";
          row.style.gridTemplateColumns = "120px 1fr";
          const t = document.createElement("div");
          t.className = "attempt-time";
          setText(t, hstTime(a.timestamp));
          const m = document.createElement("div");
          m.className = "attempt-message";
          setText(m, a.message || a.kind || "");
          row.append(t, m);
          list.append(row);
        }
        histCard.append(list);
      }
      side.append(histCard);
    }
    function gotoFiles() {
      const tm = store.tm;
      tm.level = "files";
      tm.unitSource = "";
      tm.unitName = "";
      tm.unitFns = null;
      tm.selectedFn = "";
      renderCrumbs();
      renderTreemap();
      renderDecompDetail(store.data);
    }
    function gotoUnit() {
      store.tm.selectedFn = "";
      renderCrumbs();
      renderTreemap();
      renderDecompDetail(store.data);
    }
    function renderCrumbs() {
      const nav = $("decomp-crumbs");
      nav.replaceChildren();
      const tm = store.tm;
      const mk = (label, handler, active) => {
        const b = document.createElement("button");
        b.type = "button";
        setText(b, label);
        if (active) b.disabled = true;
        else b.addEventListener("click", handler);
        return b;
      };
      const sep = () => {
        const s = document.createElement("span");
        s.className = "sep";
        s.textContent = "›";
        return s;
      };
      nav.append(mk("All files", gotoFiles, tm.level === "files"));
      if (tm.level !== "files") {
        nav.append(sep(), mk(tm.unitName || "unit", gotoUnit, !tm.selectedFn));
      }
      if (tm.selectedFn) {
        nav.append(sep(), mk(tm.selectedFn, () => {}, true));
      }
    }
    function renderDecompDetail(data) {
      const panel = $("decomp-details");
      panel.replaceChildren();
      const decomp = (data || {}).decomp || {};
      const tm = store.tm;
      if (!decomp.available) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "No report.json decomp metrics found");
        panel.append(empty);
        renderDecompAttemptLog(data, null);
        drawTimeSeries($("file-history-chart"), [], [], "No report.json decomp metrics");
        setText($("file-history-note"), "no report.json");
        return;
      }
      if (tm.level === "files") {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "Click a file in the treemap to drill into its functions.");
        panel.append(empty);
        renderDecompAttemptLog(data, null);
        drawTimeSeries($("file-history-chart"), [], [], "Select a file for its progress history");
        setText($("file-history-note"), "no file selected");
        return;
      }
      const units = decomp.units || [];
      const unit = units.find(u => (u.source || "") === tm.unitSource) || units.find(u => unitDisplayName(u) === tm.unitName);
      if (!unit) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "Unit not found in current report.");
        panel.append(empty);
        return;
      }
      const title = document.createElement("div");
      title.className = "decomp-title mono";
      setText(title, unitDisplayName(unit));
      const subtitle = document.createElement("div");
      subtitle.className = "decomp-subtitle";
      setText(subtitle, unit.source || unit.name || "unknown source");
      const stats = document.createElement("div");
      stats.className = "mini-stats";
      const matched = (unit.function_status || {}).matched || 0;
      const near = (unit.function_status || {}).near || 0;
      const partial = (unit.function_status || {}).partial || 0;
      const missing = (unit.function_status || {}).missing || 0;
      stats.append(
        miniStat("Functions", `${unit.matched_functions || 0}/${unit.total_functions || 0}`),
        miniStat("Fuzzy", pctText(unit.fuzzy_pct)),
        miniStat("Code", pctText(unit.code_pct)),
        miniStat("Bytes", Number(unit.total_code || 0).toLocaleString())
      );
      const mix = document.createElement("div");
      mix.className = "legend";
      for (const [label, value, color] of [
        ["100", matched, "#38b995"],
        ["near", near, "#f0b35a"],
        ["partial", partial, "#5c91df"],
        ["missing", missing, "#e07171"]
      ]) {
        const entry = document.createElement("span");
        entry.className = "legend-item";
        const swatch = document.createElement("span");
        swatch.className = "swatch";
        swatch.style.setProperty("--swatch", color);
        entry.append(swatch, document.createTextNode(`${label}: ${value}`));
        mix.append(entry);
      }
      const listTitle = document.createElement("div");
      listTitle.className = "panel-title";
      const h = document.createElement("h2");
      setText(h, "Functions");
      const note = document.createElement("span");
      note.className = "panel-note";
      const fnSource = tm.unitFns || (unit.functions || []);
      setText(note, tm.unitFns ? `${fnSource.length} rows (lazy /api/unit)` : `${fnSource.length} rows`);
      listTitle.append(h, note);
      const list = document.createElement("div");
      list.className = "function-list";
      const functions = fnSource.slice().sort((a, b) => {
        const ap = Number(a.fuzzy_pct || 0);
        const bp = Number(b.fuzzy_pct || 0);
        if (ap !== bp) return ap - bp;
        return Number(b.size || 0) - Number(a.size || 0);
      });
      if (!functions.length) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, tm.unitFns ? "No function rows recorded for this file" : "Loading functions...");
        list.append(empty);
      }
      for (const fn of functions) {
        const row = document.createElement("div");
        row.className = "function-row";
        if (fn.name === tm.selectedFn) row.style.outline = "1px solid #547298";
        const name = document.createElement("div");
        name.className = "function-name";
        setText(name, fn.name || "unknown");
        const pctNode = document.createElement("div");
        pctNode.className = "function-pct";
        setText(pctNode, pctText(fn.fuzzy_pct));
        const size = document.createElement("div");
        size.className = "function-size";
        setText(size, `${Number(fn.size || 0).toLocaleString()}b`);
        row.append(name, pctNode, size, statusChip(fn.status));
        row.style.cursor = "pointer";
        row.addEventListener("click", () => enterFn(fn));
        list.append(row);
      }
      panel.append(title, subtitle, stats, mix, listTitle, list);
      renderDecompAttemptLog(data, unit);
      // Real time series. If a fn is drilled, prefer /api/history/fn.
      if (tm.selectedFn) {
        loadFnHistory(tm.selectedFn);
      } else {
        loadUnitHistory(unit.source || unit.name || "");
      }
    }
    function loadUnitHistory(source) {
      if (!source) {
        drawTimeSeries($("file-history-chart"), [], [], "No history yet for this file");
        setText($("file-history-note"), "no source path");
        return;
      }
      fetch(`/api/history/unit?source=${encodeURIComponent(source)}`, { cache: "no-store" })
        .then(r => r.json())
        .then(rows => {
          if (store.tm.level !== "unit" || store.tm.selectedFn) return;
          const series = [
            { key: "fp", label: "fns %", color: "#f0b35a" },
            { key: "cp", label: "code %", color: "#5c91df" }
          ];
          drawTimeSeries($("file-history-chart"), rows, series, "No history recorded for this file yet");
          setText($("file-history-note"), rows.length ? `${rows.length} recorded changes` : "no changes recorded yet");
        })
        .catch(() => {
          drawTimeSeries($("file-history-chart"), [], [], "History unavailable");
          setText($("file-history-note"), "history unavailable");
        });
    }
    function loadFnHistory(name) {
      fetch(`/api/history/fn?name=${encodeURIComponent(name)}`, { cache: "no-store" })
        .then(r => r.json())
        .then(rows => {
          if (store.tm.selectedFn !== name) return;
          const series = [{ key: "fuzzy_pct", label: "fuzzy %", color: "#a98ee6" }];
          drawTimeSeries($("file-history-chart"), rows, series, "No fuzzy-match history for this fn yet");
          setText($("file-history-note"), rows.length ? `${rows.length} recorded changes for ${name}` : `no history yet for ${name}`);
        })
        .catch(() => {
          drawTimeSeries($("file-history-chart"), [], [], "History unavailable");
          setText($("file-history-note"), "history unavailable");
        });
    }
    function miniStat(label, value) {
      const box = document.createElement("div");
      box.className = "mini-stat";
      const k = document.createElement("div");
      k.className = "mini-label";
      setText(k, label);
      const v = document.createElement("div");
      v.className = "mini-value";
      setText(v, value);
      box.append(k, v);
      return box;
    }
    function renderDecompAttemptLog(data, unit) {
      const list = $("decomp-log");
      list.replaceChildren();
      const attempts = (unit ? relatedAttempts(data, unit) : (data.attempt_log || [])).slice().reverse();
      setText($("decomp-log-note"), unit ? `${attempts.length} entries for ${unitDisplayName(unit)}` : `${attempts.length} retained entries`);
      if (!attempts.length) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "No attempt history found for this selection");
        list.append(empty);
        return;
      }
      for (const attempt of attempts.slice(0, 80)) {
        const row = document.createElement("div");
        row.className = "attempt-row";
        const time = document.createElement("div");
        time.className = "attempt-time";
        setText(time, hstTime(Number.isFinite(Number(attempt.unix)) ? Number(attempt.unix) : attempt.timestamp));
        const kind = document.createElement("div");
        kind.className = "attempt-kind";
        setText(kind, attempt.kind);
        const message = document.createElement("div");
        message.className = "attempt-message";
        setText(message, attempt.message);
        row.append(time, kind, message);
        list.append(row);
      }
    }
    function renderCharts(data) {
      drawDonut($("status-chart"), data.charts.status || []);
      drawBars($("provenance-chart"), data.charts.provenance || []);
      drawHistory($("history-chart"), data.history || []);
      renderSourceBars(data);
    }
    // ---- v9: agent activity ------------------------------------------------
    function renderAgents(payload) {
      const body = $("agents-body");
      body.replaceChildren();
      const rows = (payload && payload.agents) || [];
      setText($("agents-note"), payload && payload.available
        ? `${rows.length} agent(s) | ${payload.claimed || 0} claimed, ${payload.queued || 0} queued`
        : "coordination data unavailable");
      if (!rows.length) {
        const tr = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 5;
        cell.className = "empty-state";
        setText(cell, "No active claims in coordination/claims.json");
        tr.append(cell);
        body.append(tr);
        return;
      }
      for (const a of rows) {
        const tr = document.createElement("tr");
        const agent = document.createElement("td");
        agent.className = "mono";
        setText(agent, a.agent);
        const fn = document.createElement("td");
        fn.className = "mono";
        setText(fn, a.function || "-");
        const file = document.createElement("td");
        file.className = "mono";
        setText(file, a.file ? fileName(a.file) : "-");
        const claimed = document.createElement("td");
        claimed.className = "mono";
        setText(claimed, a.claimed_at ? hstTime(a.claimed_at) : "-");
        const state = document.createElement("td");
        const dot = document.createElement("span");
        dot.className = `agent-dot ${a.busy ? "busy" : "idle"}`;
        state.append(dot, document.createTextNode(a.busy ? "busy" : (a.task_status || "idle")));
        tr.append(agent, fn, file, claimed, state);
        body.append(tr);
      }
    }
    function pollAgents() {
      fetch("/api/agents", { cache: "no-store" })
        .then(r => r.json()).then(renderAgents).catch(() => {});
    }
    // ---- v9: lockout reset countdowns --------------------------------------
    function fmtCountdown(secs) {
      secs = Math.max(0, Math.floor(secs));
      const h = Math.floor(secs / 3600);
      const m = Math.floor((secs % 3600) / 60);
      const s = secs % 60;
      if (h > 0) return `${h}h ${String(m).padStart(2, "0")}m`;
      if (m > 0) return `${m}m ${String(s).padStart(2, "0")}s`;
      return `${s}s`;
    }
    function renderLimits(payload) {
      store.limits = (payload && payload.agents) || [];
      setText($("limits-note"), payload && payload.available ? `${store.limits.length} tracked` : "agent_limits.json TODO");
      tickLimits();
    }
    function tickLimits() {
      const grid = $("limits-grid");
      grid.replaceChildren();
      if (!store.limits.length) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, "No agents in tools/decomp_work/agent_limits.json");
        grid.append(empty);
        return;
      }
      const now = Date.now() / 1000;
      for (const a of store.limits) {
        const card = document.createElement("div");
        card.className = "limit-card";
        const label = document.createElement("div");
        label.className = "limit-label";
        setText(label, a.label || a.name);
        const cd = document.createElement("div");
        const remaining = a.next_reset_unix ? a.next_reset_unix - now : 0;
        cd.className = "limit-countdown" + (remaining > 0 && remaining < 1800 ? " soon" : "");
        setText(cd, a.next_reset_unix ? fmtCountdown(remaining) : "n/a");
        const note = document.createElement("div");
        note.className = "limit-note";
        setText(note, a.next_reset_unix ? `resets ${hstTime(a.next_reset_unix)} HST` : (a.note || "set last_reset"));
        card.append(label, cd, note);
        grid.append(card);
      }
    }
    function pollLimits() {
      fetch("/api/limits", { cache: "no-store" })
        .then(r => r.json()).then(renderLimits).catch(() => {});
    }
    // ---- v9: token-expense bar/area chart over hourly buckets --------------
    function drawTokens(canvas, payload) {
      store._tokensPayload = payload || {};
      const { ctx, w, h } = fitCanvas(canvas);
      ctx.clearRect(0, 0, w, h);
      const buckets = (payload && payload.buckets) || [];
      if (!buckets.length) {
        return drawEmpty(ctx, w, h, payload && payload.reason ? payload.reason : "No token usage in window");
      }
      const pad = { l: 54, r: 16, t: 18, b: 30 };
      const xs = buckets.map(b => Number(b.unix || 0));
      const maxTotal = Math.max(...buckets.map(b => Number(b.total || 0)), 1);
      const minX = Math.min(...xs), maxX = Math.max(...xs);
      const x = v => pad.l + (w - pad.l - pad.r) * (v - minX) / Math.max(1, maxX - minX);
      const y = v => h - pad.b - (h - pad.t - pad.b) * v / maxTotal;
      // y gridlines with k-formatted token labels
      const fmtK = n => n >= 1e6 ? (n / 1e6).toFixed(1) + "M" : n >= 1e3 ? Math.round(n / 1e3) + "k" : String(n);
      ctx.lineWidth = 1;
      for (let i = 0; i <= 4; i++) {
        const v = maxTotal * i / 4;
        const gy = y(v);
        ctx.strokeStyle = i === 0 ? "#3a4a5e" : "#2d3a4b";
        ctx.beginPath(); ctx.moveTo(pad.l, gy); ctx.lineTo(w - pad.r, gy); ctx.stroke();
        ctx.fillStyle = "#8da0b8"; ctx.font = "11px Segoe UI, Arial"; ctx.textAlign = "right";
        ctx.fillText(fmtK(v), pad.l - 6, gy + 4);
      }
      // bars (input stacked under output)
      const n = buckets.length;
      const slot = (w - pad.l - pad.r) / Math.max(1, n);
      const bw = Math.max(1, Math.min(22, slot * 0.7));
      buckets.forEach(b => {
        const cx = x(Number(b.unix));
        const inH = (h - pad.t - pad.b) * Number(b.input || 0) / maxTotal;
        const outH = (h - pad.t - pad.b) * Number(b.output || 0) / maxTotal;
        const base = h - pad.b;
        ctx.fillStyle = "#5c91df";
        ctx.fillRect(cx - bw / 2, base - inH, bw, inH);
        ctx.fillStyle = "#f0b35a";
        ctx.fillRect(cx - bw / 2, base - inH - outH, bw, outH);
      });
      // legend + x labels
      ctx.textAlign = "left"; ctx.font = "12px Segoe UI, Arial";
      ctx.fillStyle = "#5c91df"; ctx.fillRect(pad.l, pad.t - 12, 9, 9);
      ctx.fillStyle = "#cbd5e3"; ctx.fillText("input", pad.l + 13, pad.t - 3);
      ctx.fillStyle = "#f0b35a"; ctx.fillRect(pad.l + 64, pad.t - 12, 9, 9);
      ctx.fillStyle = "#cbd5e3"; ctx.fillText("output", pad.l + 77, pad.t - 3);
      ctx.fillStyle = "#7c8aa0"; ctx.font = "11px Segoe UI, Arial"; ctx.textAlign = "center";
      const dateLbl = ux => new Date(Number(ux) * 1000).toLocaleString("en-US", { timeZone: "Pacific/Honolulu", month: "numeric", day: "numeric", hour: "2-digit", hour12: false });
      const step = Math.max(1, Math.ceil(n / 8));
      for (let i = 0; i < n; i += step) ctx.fillText(dateLbl(buckets[i].unix), x(Number(buckets[i].unix)), h - 8);
    }
    function pollTokens() {
      // Default window is 96h; if empty (the stored OpenCode sessions may be
      // weeks/months old), widen progressively so the chart is never silently
      // blank when data exists further back.
      const tryHours = [96, 720, 4320, 8760];
      let i = 0;
      const attempt = () => {
        fetch(`/api/tokens?hours=${tryHours[i]}`, { cache: "no-store" })
          .then(r => r.json())
          .then(payload => {
            if (payload && payload.available) {
              drawTokens($("tokens-chart"), payload);
              setText($("tokens-note"), `${(payload.grand_total || 0).toLocaleString()} tokens / ${payload.buckets.length} hr-buckets (${tryHours[i]}h window)`);
            } else if (i < tryHours.length - 1) {
              i++; attempt();
            } else {
              drawTokens($("tokens-chart"), payload);
              setText($("tokens-note"), (payload && payload.reason) || "no OpenCode token data found");
            }
          })
          .catch(() => { setText($("tokens-note"), "token source unavailable"); });
      };
      attempt();
    }
    function miniBar(pct) {
      const wrap = document.createElement("span");
      wrap.className = "mini-bar";
      const fill = document.createElement("span");
      fill.style.width = `${Math.max(0, Math.min(100, Number(pct || 0)))}%`;
      wrap.append(fill);
      return wrap;
    }
    function numCell(value, extra) {
      const cell = document.createElement("td");
      cell.className = "num";
      setText(cell, value);
      if (extra) cell.append(extra);
      return cell;
    }
    function renderFilesTable(data) {
      const body = $("files-body");
      const foot = $("files-foot");
      body.replaceChildren();
      foot.replaceChildren();
      const decomp = data.decomp || {};
      let units = (decomp.units || []).slice();
      const q = $("files-query").value.trim().toLowerCase();
      const incompleteOnly = $("files-incomplete").checked;
      units = units.filter(u => {
        if (q && !`${u.source} ${u.name}`.toLowerCase().includes(q)) return false;
        if (incompleteOnly && u.complete) return false;
        return true;
      });
      const key = store.filesSort;
      const dir = store.filesDir;
      const numKeys = new Set(["total_code", "matched_code", "code_pct", "total_functions", "matched_functions", "functions_pct", "fuzzy_pct"]);
      units.sort((a, b) => {
        let av, bv;
        if (key === "source") { av = String(a.source || ""); bv = String(b.source || ""); }
        else if (key === "complete") { av = a.complete ? 1 : 0; bv = b.complete ? 1 : 0; }
        else { av = Number(a[key] || 0); bv = Number(b[key] || 0); }
        if (av < bv) return -1 * dir;
        if (av > bv) return 1 * dir;
        return 0;
      });
      setText($("files-note"), decomp.available ? `${units.length}/${(decomp.units || []).length} units` : "report.json not available");
      if (!units.length) {
        const tr = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 9;
        cell.className = "empty-state";
        setText(cell, decomp.available ? "No units match the current filter" : "No report.json decomp metrics found");
        tr.append(cell);
        body.append(tr);
        return;
      }
      let sumCode = 0, sumMatchedCode = 0, sumFns = 0, sumMatchedFns = 0;
      for (const u of units) {
        sumCode += Number(u.total_code || 0);
        sumMatchedCode += Number(u.matched_code || 0);
        sumFns += Number(u.total_functions || 0);
        sumMatchedFns += Number(u.matched_functions || 0);
        const tr = document.createElement("tr");
        tr.style.cursor = "pointer";
        const src = document.createElement("td");
        src.className = "wrap mono";
        setText(src, u.source || u.name || "unknown");
        tr.append(
          src,
          numCell(Number(u.total_code || 0).toLocaleString()),
          numCell(Number(u.matched_code || 0).toLocaleString()),
          numCell(pctText(u.code_pct), miniBar(u.code_pct)),
          numCell(Number(u.total_functions || 0).toLocaleString()),
          numCell(Number(u.matched_functions || 0).toLocaleString()),
          numCell(pctText(u.functions_pct), miniBar(u.functions_pct)),
          numCell(pctText(u.fuzzy_pct)),
          td(u.complete ? "yes" : "-")
        );
        tr.addEventListener("click", () => {
          switchView("decomp");
          gotoFiles();
          enterUnit(u);
          document.querySelector(".decomp-workspace").scrollIntoView({ behavior: "smooth", block: "start" });
        });
        body.append(tr);
      }
      const codePct = sumCode ? (sumMatchedCode * 100 / sumCode) : 0;
      const fnPct = sumFns ? (sumMatchedFns * 100 / sumFns) : 0;
      foot.append(
        td(`TOTAL (${units.length})`),
        numCell(sumCode.toLocaleString()),
        numCell(sumMatchedCode.toLocaleString()),
        numCell(pctText(codePct)),
        numCell(sumFns.toLocaleString()),
        numCell(sumMatchedFns.toLocaleString()),
        numCell(pctText(fnPct)),
        numCell(pctText(decomp.fuzzy_pct)),
        td("")
      );
    }
    function switchView(view) {
      store.activeView = view;
      document.querySelectorAll(".tab-btn").forEach(button => {
        button.classList.toggle("active", button.dataset.view === view);
      });
      document.querySelectorAll(".view").forEach(section => {
        section.classList.toggle("active", section.id === `view-${view}`);
      });
      const labels = { symbols: "GC6E01/SYMBOLMAP", files: "GC6E01/FILES", decomp: "GC6E01/DECOMP" };
      setText($("hud-project"), labels[view] || "GC6E01/DECOMP");
      if (location.hash !== `#${view}`) {
        history.replaceState(null, "", `#${view}`);
      }
      if (store.data) {
        renderCharts(store.data);
        if (view === "decomp") {
          // Canvas had zero size while hidden; re-layout now that it is visible.
          renderTreemap();
          renderDecompDetail(store.data);
          // The token canvas also had zero size while hidden; redraw on show.
          pollTokens();
          tickLimits();
        } else if (view === "files") {
          renderFilesTable(store.data);
        }
      }
    }
    function scheduleRefresh() {
      if (store.refreshTimer) {
        clearInterval(store.refreshTimer);
        store.refreshTimer = null;
      }
      if ($("auto-refresh").checked) {
        store.refreshTimer = setInterval(refresh, Number($("refresh-rate").value || 5000));
      }
    }
    async function refresh() {
      const response = await fetch("/api/state", { cache: "no-store" });
      const data = await response.json();
      store.data = data;
      store.rows = data.targets || [];
      renderHud(data);
      renderMetrics(data);
      renderTop(data);
      renderLegend(data);
      renderSourceFilters(store.rows);
      renderCharts(data);
      renderActivity(data);
      renderCommits(data);
      renderCrumbs();
      renderSymbolTreemap(data);
      renderRows();
      // switchView re-renders the active view's treemap/detail/files table with
      // correct canvas dimensions.
      switchView(store.activeView);
    }
    $("query").addEventListener("input", renderRows);
    $("status-filter").addEventListener("change", () => {
      store.attention = false;
      $("attention")?.classList.remove("primary");
      renderRows();
    });
    $("source-filter").addEventListener("change", renderRows);
    $("clear").addEventListener("click", () => {
      $("query").value = "";
      $("status-filter").value = "";
      $("source-filter").value = "";
      store.attention = false;
      $("attention")?.classList.remove("primary");
      renderRows();
    });
    // The "Needs Wiring" pill was removed from the top bar; guard the handler so
    // it is a no-op when the button is absent (status filter still works via the
    // Status Map / filter controls).
    $("attention")?.addEventListener("click", () => {
      store.attention = !store.attention;
      $("attention")?.classList.toggle("primary", store.attention);
      if (store.attention) $("status-filter").value = "";
      renderRows();
    });
    $("refresh").addEventListener("click", refresh);
    $("auto-refresh").addEventListener("change", scheduleRefresh);
    $("refresh-rate").addEventListener("change", scheduleRefresh);
    $("tu-query").addEventListener("input", () => {
      if (store.data) renderSymbolTreemap(store.data);
    });
    $("tu-needs").addEventListener("click", () => {
      store.tuNeedsOnly = !store.tuNeedsOnly;
      $("tu-needs").classList.toggle("primary", store.tuNeedsOnly);
      if (store.data) renderSymbolTreemap(store.data);
    });
    $("tu-clear").addEventListener("click", () => {
      $("tu-query").value = "";
      store.tuNeedsOnly = false;
      $("tu-needs").classList.remove("primary");
      if (store.data) renderSymbolTreemap(store.data);
    });
    $("decomp-query").addEventListener("input", () => {
      if (store.data) renderTreemap();
    });
    $("decomp-area-fns").addEventListener("change", () => {
      store.tm.areaByFns = $("decomp-area-fns").checked;
      if (store.data) renderTreemap();
    });
    $("decomp-near").addEventListener("click", () => {
      store.decompNearOnly = !store.decompNearOnly;
      $("decomp-near").classList.toggle("primary", store.decompNearOnly);
      if (store.data) renderTreemap();
    });
    $("decomp-clear").addEventListener("click", () => {
      $("decomp-query").value = "";
      store.decompNearOnly = false;
      $("decomp-near").classList.remove("primary");
      gotoFiles();
    });
    $("decomp-treemap").addEventListener("mousemove", onTreemapMove);
    $("decomp-treemap").addEventListener("mouseleave", () => { $("decomp-tip").hidden = true; });
    $("decomp-treemap").addEventListener("click", onTreemapClick);
    $("reader-back").addEventListener("click", () => {
      closeReader();
      document.querySelector(".decomp-workspace").scrollIntoView({ behavior: "smooth", block: "start" });
    });
    $("files-query").addEventListener("input", () => { if (store.data) renderFilesTable(store.data); });
    $("files-incomplete").addEventListener("change", () => { if (store.data) renderFilesTable(store.data); });
    $("files-clear").addEventListener("click", () => {
      $("files-query").value = "";
      $("files-incomplete").checked = false;
      if (store.data) renderFilesTable(store.data);
    });
    document.querySelectorAll("th[data-fsort]").forEach(th => {
      th.addEventListener("click", () => {
        const key = th.dataset.fsort;
        if (store.filesSort === key) store.filesDir *= -1;
        else { store.filesSort = key; store.filesDir = (key === "source" || key === "complete") ? 1 : -1; }
        if (store.data) renderFilesTable(store.data);
      });
    });
    document.querySelectorAll(".tab-btn").forEach(button => {
      button.addEventListener("click", () => switchView(button.dataset.view || "decomp"));
    });
    document.querySelectorAll("th[data-sort]").forEach(th => {
      th.addEventListener("click", () => {
        const key = th.dataset.sort;
        if (store.sortKey === key) store.sortDir *= -1;
        else {
          store.sortKey = key;
          store.sortDir = 1;
        }
        renderRows();
      });
    });
    window.addEventListener("resize", () => {
      if (store.data) {
        renderCharts(store.data);
        if (store.activeView === "decomp") renderTreemap();
      }
    });
    if (location.hash === "#symbols") {
      store.activeView = "symbols";
    } else if (location.hash === "#files") {
      store.activeView = "files";
    }
    // v9: agent activity + lockout panels poll independently of the main refresh
    // (lighter endpoints, want a faster cadence). Token chart redraws on the
    // main refresh cadence and on view switch.
    pollAgents();
    pollLimits();
    pollTokens();
    store.agentsTimer = setInterval(pollAgents, 10000);
    store.limitsTimer = setInterval(pollLimits, 60000);
    setInterval(tickLimits, 1000);   // smooth 1s countdown without refetching
    window.addEventListener("resize", () => {
      if (store.activeView === "decomp") drawTokens($("tokens-chart"), store._tokensPayload || {});
    });
    refresh();
    scheduleRefresh();
  </script>
</body>
</html>
"""


class Handler(BaseHTTPRequestHandler):
    def send_json(self, payload: object) -> None:
        body = json.dumps(payload, indent=2).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Cache-Control", "no-store")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self) -> None:
        parsed = urlparse(self.path)
        path = parsed.path
        query = parse_qs(parsed.query)
        if path == "/api/state":
            state = get_state()
            state["history"] = update_history(state)
            decomp = state.get("decomp", {})
            if isinstance(decomp, dict):
                update_unit_history(decomp)
                update_fn_history(decomp)
            self.send_json(state)
            return
        if path == "/api/unit":
            source = (query.get("source") or query.get("name") or [""])[0]
            self.send_json(load_unit_functions(source))
            return
        if path == "/api/history":
            self.send_json(load_history())
            return
        if path == "/api/history/unit":
            source = (query.get("source") or query.get("name") or [""])[0]
            self.send_json(load_unit_history(source))
            return
        if path == "/api/history/fn":
            name = (query.get("name") or [""])[0]
            self.send_json(load_fn_history(name))
            return
        if path == "/api/asm":
            source = (query.get("source") or [""])[0]
            fn = (query.get("fn") or [""])[0]
            if not source or not fn:
                self.send_json({"available": False, "error": "source and fn are required"})
                return
            self.send_json(compute_asm_diff(source, fn))
            return
        if path == "/api/fninfo":
            fn = (query.get("fn") or [""])[0]
            self.send_json(load_fn_info(fn))
            return
        if path == "/api/agents":
            self.send_json(load_agents())
            return
        if path == "/api/tokens":
            try:
                hours = int((query.get("hours") or ["72"])[0])
            except (TypeError, ValueError):
                hours = 72
            self.send_json(load_tokens(max(1, min(hours, 24 * 30))))
            return
        if path == "/api/limits":
            self.send_json(load_limits())
            return
        if path == "/api/health":
            self.send_json({"ok": True, "version": DASHBOARD_VERSION})
            return
        if path == "/favicon.ico":
            self.send_response(204)
            self.end_headers()
            return
        body = HTML.encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Cache-Control", "no-store")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, fmt: str, *args: object) -> None:
        return


# ---- auto-refresh: periodically regenerate report.json so progress-over-time is
# captured automatically (the dashboard runs the "progress check" itself). --------
REPORT_REFRESH_SECONDS = int(os.environ.get("DASH_REPORT_REFRESH", "600"))  # 0 = off
GEN_REPORT = ROOT / "tools" / "gen_decomp_report.py"
STATUS_LOG = ROOT / "tools" / "decomp_work" / "coordination" / "status.md"
_auto_state = {"last_matched": None}


def _report_matched():
    try:
        d = json.loads(DECOMP_REPORT.read_text(encoding="utf-8", errors="replace"))
        return int(d.get("measures", d).get("matched_functions"))
    except Exception:
        return None


def _report_units():
    """{unit_name: (matched, total)} from report.json -> per-file attempt-log attribution."""
    out = {}
    try:
        d = json.loads(DECOMP_REPORT.read_text(encoding="utf-8", errors="replace"))
        for u in d.get("units", []):
            m = u.get("measures", {})
            out[u.get("name", "")] = (int(m.get("matched_functions", 0)),
                                      int(m.get("total_functions", 0)))
    except Exception:
        pass
    return out


def _refresh_report_once() -> bool:
    """Regenerate report.json atomically via gen_decomp_report.py."""
    if not GEN_REPORT.exists():
        return False
    tmp = DECOMP_REPORT.with_name(DECOMP_REPORT.name + ".auto.tmp")
    try:
        proc = subprocess.run(
            [sys.executable, str(GEN_REPORT), "-o", str(tmp)],
            cwd=str(ROOT), capture_output=True, text=True, timeout=1200,
        )
        if proc.returncode == 0 and tmp.exists() and tmp.stat().st_size > 100:
            os.replace(str(tmp), str(DECOMP_REPORT))
            return True
    except Exception:
        pass
    try:
        if tmp.exists():
            tmp.unlink()
    except Exception:
        pass
    return False


def _auto_report_loop(interval: int) -> None:
    _auto_state["last_matched"] = _report_matched()
    _auto_state["units"] = _report_units()
    while True:
        time.sleep(interval)
        if not _refresh_report_once():
            continue
        # sample the fresh numbers into the time-series history
        try:
            state = get_state(force=True)
            update_history(state)
            decomp = state.get("decomp", {})
            if isinstance(decomp, dict):
                update_unit_history(decomp)
                update_fn_history(decomp)
        except Exception:
            pass
        # Append live attempt-log lines so the activity feed updates without any
        # agent writing to status.md: a per-FILE line for every unit whose matched
        # count moved (this is how codex's work — e.g. menu_middle.c — shows up,
        # since codex commits to git, not status.md), plus an aggregate line.
        new = _report_matched()
        old = _auto_state["last_matched"]
        new_units = _report_units()
        old_units = _auto_state.get("units") or {}
        ts = time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())
        try:
            lines = []
            for name, (mn, tn) in new_units.items():
                prev = old_units.get(name)
                if prev is not None and mn != prev[0]:
                    sign = "+" if mn > prev[0] else ""
                    lines.append(f"- **{ts}** `report` - {name} {prev[0]}->{mn}/{tn} "
                                 f"({sign}{mn - prev[0]} fns)\n")
            if new is not None and old is not None and new != old:
                tot = int(json.loads(DECOMP_REPORT.read_text(encoding="utf-8", errors="replace"))
                          .get("measures", {}).get("total_functions", 0)) or 1
                lines.append(f"- **{ts}** `auto-report` - report.json {old}->{new} / "
                             f"{tot} ({100.0 * new / tot:.2f}% fns)\n")
            if lines:
                with open(STATUS_LOG, "a", encoding="utf-8") as fh:
                    fh.writelines(lines)
        except Exception:
            pass
        if new is not None:
            _auto_state["last_matched"] = new
        _auto_state["units"] = new_units


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8788)
    parser.add_argument("--once", action="store_true", help="print JSON state and exit")
    args = parser.parse_args()

    if args.once:
        state = get_state(force=True)
        state["history"] = update_history(state)
        decomp = state.get("decomp", {})
        if isinstance(decomp, dict):
            update_unit_history(decomp)
            update_fn_history(decomp)
        print(json.dumps(state, indent=2))
        return 0

    if REPORT_REFRESH_SECONDS > 0 and GEN_REPORT.exists():
        threading.Thread(
            target=_auto_report_loop, args=(REPORT_REFRESH_SECONDS,), daemon=True
        ).start()
        print(f"  auto-refresh: regenerating report.json every {REPORT_REFRESH_SECONDS}s "
              f"(set DASH_REPORT_REFRESH=0 to disable)")

    server = ThreadingHTTPServer((args.host, args.port), Handler)
    print(f"Renaming dashboard: http://{args.host}:{args.port}/")
    server.serve_forever()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
