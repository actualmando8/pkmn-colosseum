#!/usr/bin/env python3
"""Rank unmatched functions by whether their diff shape is WINNABLE or a WALL.

Target selection is the scarce resource in this campaign, not compute. Most
near-misses in the report are unwinnable for structural reasons -- a shared-
master shim, an anonymous conv-literal, a register permutation -- and a model
pointed at one of those burns turns discovering that for itself.

    python tools/decomp_work/handoff/triage.py
    python tools/decomp_work/handoff/triage.py --min 95 --actionable

Output per unmatched symbol: match %, delta count, and lever tags.
Tags marked [ACTIONABLE] have a documented source-level lever (LEVERS.md).
Tags marked [WALL] have been measured as not C-steerable -- skip them.
"""
import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CLI = ROOT / "build" / "tools" / ("objdiff-cli.exe" if os.name == "nt" else "objdiff-cli")
DTK = ROOT / "build" / "tools" / ("dtk.exe" if os.name == "nt" else "dtk")


def classify(deltas):
    """deltas: list of (target_instr, our_instr)."""
    tags = []
    j = " | ".join(f"{a} >> {b}" for a, b in deltas)
    if re.search(r"\b(stmw|lmw)\b", j) and re.search(r"\b(stw|lwz)\b", j):
        tags.append("stmw/lmw prologue -> add -O4,s [ACTIONABLE]")
    if re.search(r"\bclrlwi\b", j):
        tags.append("clrlwi mask width [ACTIONABLE]")
    if any(re.match(r"b(eq|ne|lt|ge|gt|le)", a or "")
           and (a or "").split()[:1] != (b or "").split()[:1] for a, b in deltas):
        tags.append("branch polarity [ACTIONABLE]")
    if any(re.search(r"\bmr\.\s", a or "") and re.search(r"\bmr\s", b or "")
           for a, b in deltas):
        tags.append("record-form mr. [UNSOLVED: peephole-on and assign-in-cond "
                    "both fail on fn_80165FDC]")
    if re.search(r"@\d+@sda2", j):
        tags.append("anonymous conv-literal [WALL]")
    if any(re.search(r"addi\s+r\d+,\s*r\d+,\s*0x0", a or "") and "mr " in (b or "")
           for a, b in deltas):
        tags.append("addi rD,rS,0 vs mr [WALL]")
    ops = [((a.split() or [""])[0], (b.split() or [""])[0]) for a, b in deltas]
    if deltas and all(x == y for x, y in ops if x and y):
        tags.append("same opcodes, registers differ [reg-alloc: try decl order]")
    return tags or ["unclassified"]


def symbols_of(obj: Path):
    r = subprocess.run([str(DTK), "elf", "info", str(obj)],
                       capture_output=True, text=True)
    out = []
    for line in r.stdout.splitlines():
        if "|" in line:
            n = line.split("|")[-1].strip()
            if n and not n.startswith(".") and not n[0].isdigit():
                out.append(n)
    return out


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--min", type=float, default=90.0,
                    help="minimum unit fuzzy %% to consider (default 90)")
    ap.add_argument("--actionable", action="store_true",
                    help="only show symbols with an [ACTIONABLE] tag")
    a = ap.parse_args()

    rep = json.loads((ROOT / "build" / "GC6E01" / "report.json").read_text())
    rows = []
    for u in rep["units"]:
        md, m = u["metadata"], u["measures"]
        sp = md.get("source_path")
        if not sp or md.get("auto_generated") or md.get("complete"):
            continue
        tf, mf = m.get("total_functions", 0), m.get("matched_functions", 0)
        if not tf or mf >= tf or m.get("fuzzy_match_percent", 0) < a.min:
            continue
        p = ROOT / sp
        if not p.exists():
            continue
        # shim over a shared master: cannot be linked independently
        if re.search(r'#include\s+"[^"]+\.c"', p.read_text(errors="replace")):
            continue
        rows.append((m["fuzzy_match_percent"], u["name"], sp))
    rows.sort(key=lambda r: -r[0])

    print(f"scanning {len(rows)} standalone near-miss units (>= {a.min}%)\n")
    for _fz, unit, sp in rows:
        obj = ROOT / "build" / "GC6E01" / Path(sp).with_suffix(".o")
        if not obj.exists():
            continue
        names = symbols_of(obj)
        if not names:
            continue
        with tempfile.TemporaryDirectory() as td:
            j = Path(td) / "d.json"
            subprocess.run([str(CLI), "diff", "-p", str(ROOT), "-u", unit,
                            "-o", str(j), "--format", "json", names[0]],
                           capture_output=True)
            if not j.exists():
                continue
            d = json.loads(j.read_text())
        for s in d["left"].get("symbols", []):
            mp = s.get("match_percent")
            if mp is None or mp >= 100.0:
                continue
            L = s.get("instructions") or []
            R = next(((x.get("instructions") or []) for x in d["right"]["symbols"]
                      if x["name"] == s["name"]), [])
            f = lambda i: (i.get("instruction") or {}).get("formatted", "") if i else ""
            dl = [(f(L[k] if k < len(L) else None), f(R[k] if k < len(R) else None))
                  for k in range(max(len(L), len(R)))]
            dl = [x for x in dl if x[0] != x[1]]
            tags = classify(dl)
            if a.actionable and not any("[ACTIONABLE]" in t for t in tags):
                continue
            print(f"{mp:6.2f}% {len(dl):>4}d  {s['name']:<38} {sp}")
            for t in tags:
                print(f"           - {t}")


if __name__ == "__main__":
    main()
