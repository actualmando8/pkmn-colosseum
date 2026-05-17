#!/usr/bin/env python3
"""verify_commit.py - anti-fraud gate for decomp commits.

Every wave this session produced the same recurring fraud/error modes that
had to be caught by hand before cherry-picking:

  - editing `*_fn_*.inc` (the ROM-truth bytes objdiff measures against)
  - editing other truth files (symbols.txt / splits*.txt / target .o)
  - flipping `#if 0` -> `#if 1` to re-activate an asm wrapper and forge 100%
  - claiming a match% that does not re-measure on a clean build

This makes that vigilance a one-shot tool. Run it before cherry-picking a
subagent commit, and in CI.

Usage:
    # check the last commit on the current branch (vs its parent)
    python tools/verify_commit.py

    # check a range / specific commit
    python tools/verify_commit.py --range master..HEAD
    python tools/verify_commit.py --commit <sha>

    # also re-measure claimed results (compile + objdiff)
    python tools/verify_commit.py --measure src/game/gs_render.c:fn_800D56C0:100

Exit code 0 = clean, 1 = violation (prints what + why).
"""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TARGET_O = ROOT / "build" / "GC6E01" / "obj" / "auto_01_800055E0_text.o"
OBJDIFF = ROOT / "tools" / "objdiff-cli.exe"
sys.path.insert(0, str(ROOT / "tools"))

# Files a match-improvement commit may NEVER modify.
TRUTH_DENY = [
    re.compile(r"_fn_[0-9A-Fa-f]+\.inc$"),
    re.compile(r"\.inc$"),
    re.compile(r"config/GC6E01/symbols\.txt$"),
    re.compile(r"config/GC6E01/splits.*\.txt$"),
    re.compile(r"config/GC6E01/link_order\.txt$"),
    re.compile(r"build/GC6E01/obj/.*\.o$"),
    re.compile(r"\.dol$"),
]


def git(*args):
    return subprocess.run(["git", "-C", str(ROOT), *args],
                           capture_output=True, text=True).stdout


def changed_files(rng):
    out = git("diff", "--name-only", rng)
    return [f.strip() for f in out.splitlines() if f.strip()]


def diff_text(rng):
    return git("diff", rng)


def check_truth_files(files):
    bad = []
    for f in files:
        for rx in TRUTH_DENY:
            if rx.search(f):
                bad.append(f)
                break
    return bad


def check_asm_wrapper_flip(diff):
    """Detect `#if 0` -> `#if 1` re-activation of an asm wrapper.

    Fraud signature: an added `#if 1` (or `#if 0` removed and `#if 1`
    added) within a few lines of an `asm ` wrapper or a `_fn_*.inc`
    include.
    """
    lines = diff.splitlines()
    violations = []
    window = []
    for i, ln in enumerate(lines):
        window = lines[max(0, i - 4):i + 5]
        if re.match(r"^\+\s*#if\s+1\b", ln):
            ctx = "\n".join(window)
            if ("asm " in ctx or re.search(r"_fn_[0-9A-Fa-f]+\.inc", ctx)
                    or re.search(r"^\+.*asm\s+\w+\s+fn_", ctx, re.M)):
                violations.append(ln.strip())
        # also: removed `#if 0` paired with added `#if 1`
        if re.match(r"^-\s*#if\s+0\b", ln):
            nxt = "\n".join(lines[i:i + 6])
            if re.search(r"^\+\s*#if\s+1\b", nxt, re.M):
                violations.append(ln.strip() + "  -> #if 1")
    return violations


def remeasure(spec):
    """spec = 'src/path.c:fn_NAME:CLAIMED' -> (ok, msg)."""
    import compile_check
    src, fn, claimed = spec.rsplit(":", 2)
    claimed = float(claimed)
    src_path = ROOT / src
    try:
        base_o = compile_check.compile_source(src_path)
    except SystemExit:
        return (False, f"{fn}: compile FAILED (claimed {claimed}%)")
    r = subprocess.run(
        [str(OBJDIFF), "diff", "-1", str(TARGET_O), "-2", str(base_o),
         "-o", "-", "--format", "json",
         "-c", "ppc.calculatePoolRelocations=false"],
        capture_output=True, text=True, cwd=str(ROOT))
    if r.returncode != 0:
        return (False, f"{fn}: objdiff failed")
    j = json.loads(r.stdout)
    actual = None
    for s in j.get("right", {}).get("symbols", []):
        if s.get("name") == fn:
            actual = s.get("match_percent", 0.0)
            break
    if actual is None:
        return (False, f"{fn}: not found in object")
    if actual + 0.01 < claimed:
        return (False, f"{fn}: claimed {claimed}% but measured "
                        f"{actual:.2f}% (HALLUCINATION)")
    return (True, f"{fn}: {actual:.2f}% >= claimed {claimed}% OK")


def main():
    ap = argparse.ArgumentParser()
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--range", help="git diff range (e.g. master..HEAD)")
    g.add_argument("--commit", help="single commit sha")
    ap.add_argument("--measure", action="append", default=[],
                    help="src.c:fn:CLAIMED% — re-measure claim (repeatable)")
    args = ap.parse_args()

    if args.commit:
        rng = f"{args.commit}~1..{args.commit}"
    elif args.range:
        rng = args.range
    else:
        rng = "HEAD~1..HEAD"

    files = changed_files(rng)
    diff = diff_text(rng)
    violations = []

    bad_truth = check_truth_files(files)
    if bad_truth:
        violations.append("TRUTH-FILE EDIT (forbidden): "
                          + ", ".join(bad_truth))

    flips = check_asm_wrapper_flip(diff)
    if flips:
        violations.append("ASM-WRAPPER #if 0->#if 1 FLIP: "
                          + " | ".join(flips[:5]))

    for spec in args.measure:
        ok, msg = remeasure(spec)
        print(("  OK   " if ok else "  FAIL ") + msg)
        if not ok:
            violations.append("MEASURE: " + msg)

    print(f"\n[verify] range {rng}: {len(files)} files changed")
    if violations:
        print("[verify] REJECTED:")
        for v in violations:
            print("  - " + v)
        return 1
    print("[verify] clean — no truth edits, no asm-flip, claims hold")
    return 0


if __name__ == "__main__":
    sys.exit(main())
