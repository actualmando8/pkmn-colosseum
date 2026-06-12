#!/usr/bin/env python3
"""Live dashboard for symbolmap renaming targets."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import time
from collections import Counter
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse


ROOT = Path(__file__).resolve().parents[2]
SM_DIR = ROOT / "config" / "GC6E01" / "symbolmap"
SYMBOLS = ROOT / "config" / "GC6E01" / "symbols.txt"
FUNC_TU_MAP = ROOT / "config" / "GC6E01" / "func_tu_map.json"
DECOMP_REPORT = ROOT / "report.json"
HISTORY_FILE = ROOT / ".omx" / "state" / "renaming_dashboard_history.json"
HISTORY_INTERVAL_SECONDS = 60
DASHBOARD_VERSION = 2

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


def recent_commits(limit: int = 10) -> list[dict[str, str]]:
    text = git_value(
        [
            "log",
            f"-n{limit}",
            "--date=format:%b %d, %H:%M",
            "--pretty=format:%h%x09%cd%x09%s",
        ]
    )
    rows: list[dict[str, str]] = []
    for line in text.splitlines():
        parts = line.split("\t", 2)
        if len(parts) != 3:
            continue
        sha, when, subject = parts
        rows.append({"sha": sha, "when": when, "subject": subject})
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
    }


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
    tmp.write_text(json.dumps(history[-500:], indent=2), encoding="utf-8")
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
    return history[-500:]


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
    return {
        "version": DASHBOARD_VERSION,
        "generated_at": time.strftime("%Y-%m-%d %H:%M:%S"),
        "repo": str(ROOT),
        "head": git_value(["rev-parse", "--short", "HEAD"]),
        "branch": git_value(["branch", "--show-current"]),
        "recent_commits": recent_commits(),
        "decomp": decomp,
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
        },
    }


HTML = r"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>GC6E01 Renaming Control</title>
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
    @media (max-width: 1180px) {
      .metric-grid { grid-template-columns: repeat(3, minmax(130px, 1fr)); }
      .overview, .chart-grid, .ops-grid, .workbench { grid-template-columns: 1fr; }
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
      .feed-row, .commit-row { grid-template-columns: 1fr; }
    }
  </style>
</head>
<body>
  <header class="topbar">
    <div>
      <h1>GC6E01 Symbolmap Control</h1>
      <div class="subtitle">
        <span id="repo"></span>
        <span id="head"></span>
        <span id="updated"></span>
      </div>
    </div>
    <div class="actions">
      <button class="btn ghost" id="attention" type="button">Needs Wiring</button>
      <button class="btn primary" id="refresh" type="button">Refresh</button>
    </div>
  </header>
  <main>
    <section class="hud-strip">
      <div class="hud-stats">
        <span class="hud-project">GC6E01/SYMBOLMAP</span>
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

    <section class="metric-grid" id="metrics"></section>

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

    <section class="chart-grid">
      <div class="panel chart-card">
        <div class="panel-title">
          <h2>Status Distribution</h2>
          <span class="panel-note" id="status-total"></span>
        </div>
        <canvas id="status-chart" height="205"></canvas>
      </div>
      <div class="panel chart-card">
        <div class="panel-title">
          <h2>Completion Timeline</h2>
          <span class="panel-note" id="timeline-range"></span>
        </div>
        <canvas id="history-chart" height="205"></canvas>
      </div>
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
        <h2>Decomp Match Units</h2>
        <span class="panel-note" id="decomp-note"></span>
      </div>
      <div class="tu-toolbar">
        <input id="decomp-query" type="search" placeholder="Filter decomp unit or source">
        <button class="btn" id="decomp-near" type="button">Near Match</button>
        <button class="btn" id="decomp-clear" type="button">Clear</button>
      </div>
      <div class="tu-map" id="decomp-map"></div>
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
      refreshTimer: null
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
    function drawHistory(canvas, history) {
      const { ctx, w, h } = fitCanvas(canvas);
      ctx.clearRect(0, 0, w, h);
      if (!history || history.length < 2) {
        return drawEmpty(ctx, w, h, "Timeline starts with the next snapshot");
      }
      const pad = { l: 42, r: 16, t: 18, b: 28 };
      const xs = history.map(row => Number(row.unix || 0));
      const series = [
        { key: "completion_pct", label: "symbols", color: "#38b995" },
        { key: "decomp_functions_pct", label: "fns", color: "#f0b35a" },
        { key: "decomp_code_pct", label: "code", color: "#5c91df" },
        { key: "decomp_fuzzy_pct", label: "fuzzy", color: "#a98ee6" }
      ].filter(item => history.some(row => Number(row[item.key] || 0) > 0));
      const ys = history.flatMap(row => series.map(item => Number(row[item.key] || 0)));
      const minX = Math.min(...xs);
      const maxX = Math.max(...xs);
      const minY = 0;
      const maxY = Math.max(100, Math.ceil(Math.max(...ys) / 10) * 10);
      const x = value => pad.l + (w - pad.l - pad.r) * (value - minX) / Math.max(1, maxX - minX);
      const y = value => h - pad.b - (h - pad.t - pad.b) * (value - minY) / Math.max(1, maxY - minY);
      ctx.strokeStyle = "#2d3a4b";
      ctx.lineWidth = 1;
      for (let i = 0; i <= 4; i++) {
        const gy = pad.t + (h - pad.t - pad.b) * i / 4;
        ctx.beginPath();
        ctx.moveTo(pad.l, gy);
        ctx.lineTo(w - pad.r, gy);
        ctx.stroke();
      }
      series.forEach((item, sidx) => {
        ctx.strokeStyle = item.color;
        ctx.lineWidth = sidx === 0 ? 2.5 : 2;
        ctx.beginPath();
        history.forEach((row, idx) => {
          const px = x(Number(row.unix || 0));
          const py = y(Number(row[item.key] || 0));
          if (idx === 0) ctx.moveTo(px, py);
          else ctx.lineTo(px, py);
        });
        ctx.stroke();
        ctx.fillStyle = item.color;
        for (const row of history) {
          ctx.beginPath();
          ctx.arc(x(Number(row.unix || 0)), y(Number(row[item.key] || 0)), 2.5, 0, Math.PI * 2);
          ctx.fill();
        }
      });
      let legendX = pad.l;
      for (const item of series) {
        ctx.fillStyle = item.color;
        ctx.fillRect(legendX, pad.t - 12, 9, 9);
        ctx.fillStyle = "#cbd5e3";
        ctx.textAlign = "left";
        ctx.font = "12px Segoe UI, Arial";
        ctx.fillText(item.label, legendX + 13, pad.t - 3);
        legendX += 72;
      }
      ctx.fillStyle = "#a8b4c4";
      ctx.font = "12px Segoe UI, Arial";
      ctx.textAlign = "right";
      ctx.fillText("100%", pad.l - 8, y(100) + 4);
      ctx.fillText("0%", pad.l - 8, y(0) + 4);
      ctx.textAlign = "left";
      ctx.fillText(history[0].timestamp || "", pad.l, h - 8);
      ctx.textAlign = "right";
      ctx.fillText(history[history.length - 1].timestamp || "", w - pad.r, h - 8);
    }
    function renderMetrics(data) {
      const decomp = data.decomp || {};
      const metrics = $("metrics");
      metrics.replaceChildren(
        metric("Completion", `${data.metrics.completion_pct}%`, `${data.metrics.wired_targets}/${data.counts.targets} recorded or renamed`, "#38b995"),
        metric("Decomp Fns", pctText(decomp.functions_pct), `${decomp.matched_functions || 0}/${decomp.total_functions || 0} functions at 100%`, "#f0b35a"),
        metric("Decomp Code", pctText(decomp.code_pct), `${(decomp.matched_code || 0).toLocaleString()}/${(decomp.total_code || 0).toLocaleString()} bytes`, "#5c91df"),
        metric("Fuzzy Match", pctText(decomp.fuzzy_pct), "weighted instruction similarity", "#a98ee6"),
        metric("Complete Units", `${decomp.complete_units || 0}/${decomp.total_units || 0}`, "report.json decomp units", "#38b995"),
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
          time: row.timestamp || "",
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
        setText(when, commit.when);
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
    function renderTreemap(data) {
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
          $("attention").classList.remove("primary");
          renderRows();
          document.querySelector(".workbench").scrollIntoView({ behavior: "smooth", block: "start" });
        });
        map.append(tile);
      }
    }
    function filteredDecompUnits(data) {
      const q = $("decomp-query").value.trim().toLowerCase();
      return ((data.decomp || {}).units || []).filter(row => {
        const haystack = `${row.name} ${row.source}`.toLowerCase();
        if (q && !haystack.includes(q)) return false;
        if (store.decompNearOnly && !(Number(row.functions_pct || 0) >= 90 && Number(row.functions_pct || 0) < 100)) return false;
        return true;
      });
    }
    function renderDecompMap(data) {
      const map = $("decomp-map");
      map.replaceChildren();
      const units = filteredDecompUnits(data);
      const decomp = data.decomp || {};
      setText($("decomp-note"), decomp.available ? `${units.length}/${(decomp.units || []).length} units | ${pctText(decomp.functions_pct)} fns` : "report.json not available");
      if (!decomp.available || !units.length) {
        const empty = document.createElement("div");
        empty.className = "empty-state";
        setText(empty, decomp.available ? "No decomp units match the current filter" : "No report.json decomp metrics found");
        map.append(empty);
        return;
      }
      const maxFns = Math.max(...units.map(row => Number(row.total_functions || 0)), 1);
      for (const row of units) {
        const totalFns = Number(row.total_functions || 0);
        const pct = Number(row.functions_pct || 0);
        const span = Math.max(1, Math.min(5, Math.ceil(Math.sqrt(totalFns / maxFns) * 5)));
        const tile = document.createElement("button");
        tile.type = "button";
        tile.className = "tu-tile";
        tile.style.background = tileColor(pct);
        tile.style.gridColumnEnd = `span ${span}`;
        tile.style.gridRowEnd = `span ${Math.max(1, Math.min(3, Math.ceil(span * .72)))}`;
        tile.title = `${row.name} | ${row.matched_functions}/${row.total_functions} functions | ${pctText(row.code_pct)} code`;
        const name = document.createElement("div");
        name.className = "tu-name";
        setText(name, row.name || shortSource(row.source));
        const pctNode = document.createElement("div");
        pctNode.className = "tu-pct";
        setText(pctNode, pctText(pct));
        const meta = document.createElement("div");
        meta.className = "tu-meta";
        setText(meta, `${row.matched_functions || 0}/${row.total_functions || 0} fns | ${pctText(row.code_pct)} code`);
        tile.append(name, pctNode, meta);
        tile.addEventListener("click", () => {
          $("query").value = shortSource(row.source || row.name);
          store.attention = false;
          $("attention").classList.remove("primary");
          renderRows();
          document.querySelector(".workbench").scrollIntoView({ behavior: "smooth", block: "start" });
        });
        map.append(tile);
      }
    }
    function renderCharts(data) {
      drawDonut($("status-chart"), data.charts.status || []);
      drawBars($("provenance-chart"), data.charts.provenance || []);
      drawHistory($("history-chart"), data.history || []);
      renderSourceBars(data);
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
      renderDecompMap(data);
      renderTreemap(data);
      renderRows();
    }
    $("query").addEventListener("input", renderRows);
    $("status-filter").addEventListener("change", () => {
      store.attention = false;
      $("attention").classList.remove("primary");
      renderRows();
    });
    $("source-filter").addEventListener("change", renderRows);
    $("clear").addEventListener("click", () => {
      $("query").value = "";
      $("status-filter").value = "";
      $("source-filter").value = "";
      store.attention = false;
      $("attention").classList.remove("primary");
      renderRows();
    });
    $("attention").addEventListener("click", () => {
      store.attention = !store.attention;
      $("attention").classList.toggle("primary", store.attention);
      if (store.attention) $("status-filter").value = "";
      renderRows();
    });
    $("refresh").addEventListener("click", refresh);
    $("auto-refresh").addEventListener("change", scheduleRefresh);
    $("refresh-rate").addEventListener("change", scheduleRefresh);
    $("tu-query").addEventListener("input", () => {
      if (store.data) renderTreemap(store.data);
    });
    $("tu-needs").addEventListener("click", () => {
      store.tuNeedsOnly = !store.tuNeedsOnly;
      $("tu-needs").classList.toggle("primary", store.tuNeedsOnly);
      if (store.data) renderTreemap(store.data);
    });
    $("tu-clear").addEventListener("click", () => {
      $("tu-query").value = "";
      store.tuNeedsOnly = false;
      $("tu-needs").classList.remove("primary");
      if (store.data) renderTreemap(store.data);
    });
    $("decomp-query").addEventListener("input", () => {
      if (store.data) renderDecompMap(store.data);
    });
    $("decomp-near").addEventListener("click", () => {
      store.decompNearOnly = !store.decompNearOnly;
      $("decomp-near").classList.toggle("primary", store.decompNearOnly);
      if (store.data) renderDecompMap(store.data);
    });
    $("decomp-clear").addEventListener("click", () => {
      $("decomp-query").value = "";
      store.decompNearOnly = false;
      $("decomp-near").classList.remove("primary");
      if (store.data) renderDecompMap(store.data);
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
      if (store.data) renderCharts(store.data);
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
        path = urlparse(self.path).path
        if path == "/api/state":
            state = build_state()
            state["history"] = update_history(state)
            self.send_json(state)
            return
        if path == "/api/history":
            self.send_json(load_history())
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


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8788)
    parser.add_argument("--once", action="store_true", help="print JSON state and exit")
    args = parser.parse_args()

    if args.once:
        state = build_state()
        state["history"] = update_history(state)
        print(json.dumps(state, indent=2))
        return 0

    server = ThreadingHTTPServer((args.host, args.port), Handler)
    print(f"Renaming dashboard: http://{args.host}:{args.port}/")
    server.serve_forever()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
