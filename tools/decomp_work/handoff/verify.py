#!/usr/bin/env python3
"""Verify a candidate .c returned by an external model, against the real gate.

The sandbox (and objdiff generally) can only tell you a function's .text
matches. That is necessary but NOT sufficient: a unit can score 100% and still
fail to link, or link and produce a DOL that differs from the original. The
authoritative gate is main.dol vs config/GC6E01/build.sha1.

    # measure only (safe, always restores)
    python tools/decomp_work/handoff/verify.py \
        --source src/game/main_retrace.c --symbol fn_80005E00 --candidate new.c

    # measure, then promote + link + check the DOL SHA
    python tools/decomp_work/handoff/verify.py --source ... --symbol ... \
        --candidate new.c --link

    # keep the change in the tree if (and only if) everything passed
    ... --link --keep

Exit code 0 only when every requested check passed.
"""
import argparse
import json
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CONFIGURE = ROOT / "configure.py"


def read_raw(p: Path) -> str:
    """Read preserving line endings (Path.read_text(newline=) is 3.13+)."""
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as fh:
        return fh.read()


def sh(cmd, **kw):
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, **kw)


def objdiff_cli() -> Path:
    for c in ("objdiff-cli.exe", "objdiff-cli"):
        p = ROOT / "build" / "tools" / c
        if p.exists():
            return p
    sys.exit("objdiff-cli not found under build/tools/")


def unit_name(source: str) -> str:
    cfg = json.loads((ROOT / "objdiff.json").read_text())
    tail = Path(source.replace("\\", "/")).with_suffix(".o").as_posix()
    for u in cfg["units"]:
        if (u.get("base_path") or "").replace("\\", "/").endswith(tail):
            return u["name"]
    sys.exit(f"no objdiff unit for {source}")


def strip_comments(text: str) -> str:
    """Blank out /*...*/ and //... so keyword scans can't hit prose.

    Prose in this codebase genuinely contains "asm{} blocks", "asm incs",
    "asm wrappers" -- scanning raw text produces false rejections.
    Newlines are preserved so line-based #if tracking still lines up.
    """
    out, i, n = [], 0, len(text)
    state = None  # None | 'block' | 'line' | 'str' | 'chr'
    while i < n:
        c, nxt = text[i], text[i + 1] if i + 1 < n else ""
        if state is None:
            if c == "/" and nxt == "*":
                state, i = "block", i + 2; continue
            if c == "/" and nxt == "/":
                state, i = "line", i + 2; continue
            if c == '"':
                state = "str"
            elif c == "'":
                state = "chr"
            out.append(c)
        elif state == "block":
            if c == "*" and nxt == "/":
                state, i = None, i + 2; continue
            out.append("\n" if c == "\n" else " ")
        elif state == "line":
            if c == "\n":
                state = None; out.append("\n")
            else:
                out.append(" ")
        else:  # inside a string/char literal
            out.append(c)
            if c == "\\":
                if i + 1 < n:
                    out.append(text[i + 1]); i += 2; continue
            elif (state == "str" and c == '"') or (state == "chr" and c == "'"):
                state = None
        i += 1
    return "".join(out)


def reject_asm(text: str, path: str) -> None:
    """A match built from assembly is not a decompilation. Refuse it."""
    bad = []
    stack = []
    for ln in strip_comments(text).splitlines():
        s = ln.strip()
        if re.match(r"#\s*if\b", s):
            stack.append(re.sub(r"^#\s*if\s*", "", s).strip() != "0"); continue
        if re.match(r"#\s*ifdef\b|#\s*ifndef\b", s):
            stack.append(True); continue
        if re.match(r"#\s*else\b", s):
            if stack: stack[-1] = not stack[-1]
            continue
        if re.match(r"#\s*elif\b", s):
            if stack: stack[-1] = True
            continue
        if re.match(r"#\s*endif\b", s):
            if stack: stack.pop()
            continue
        if not all(stack):
            continue  # dead #if 0 reference asm is fine
        if re.search(r'#\s*include\s+"[^"]+\.inc"', s) or re.search(
                r"\basm\s+\w+\s*\(|\basm\s*\{|__asm\b|\bnofralloc\b", s):
            bad.append(s[:70])
    if bad:
        print(f"REJECTED: {path} contains LIVE assembly -- not a valid decomp:")
        for b in bad[:6]:
            print(f"    {b}")
        sys.exit(2)


def measure(unit: str, symbol: str):
    with tempfile.TemporaryDirectory() as td:
        out = Path(td) / "m.json"
        sh([str(objdiff_cli()), "diff", "-p", str(ROOT), "-u", unit,
            "-o", str(out), "--format", "json", symbol])
        if not out.exists():
            return None, None
        d = json.loads(out.read_text())
        sym = next((s for s in d["left"].get("symbols", [])
                    if s["name"] == symbol), None)
        if not sym:
            return None, None
        L = sym.get("instructions") or []
        R = next(((s.get("instructions") or []) for s in d["right"].get("symbols", [])
                  if s["name"] == symbol), [])
        f = lambda i: (i.get("instruction") or {}).get("formatted", "") if i else ""
        deltas = [(k, f(L[k] if k < len(L) else None), f(R[k] if k < len(R) else None))
                  for k in range(max(len(L), len(R)))]
        deltas = [d_ for d_ in deltas if d_[1] != d_[2]]
        return sym.get("match_percent"), deltas


def promote(source: str) -> bool:
    """CodeCandidate -> Matching for this unit. Returns True if changed."""
    rel = source.replace("\\", "/")
    rel = rel[4:] if rel.startswith("src/") else rel
    text = read_raw(CONFIGURE)
    pat = re.compile(r'(Object\(\s*|\(\s*)CodeCandidate(\s*,\s*"'
                     + re.escape(rel) + r'")')
    new, n = pat.subn(lambda m: m.group(1) + "Matching" + m.group(2), text)
    if n:
        CONFIGURE.write_text(new, encoding="utf-8", newline="")
    return bool(n)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", required=True)
    ap.add_argument("--symbol", required=True)
    ap.add_argument("--candidate", required=True)
    ap.add_argument("--link", action="store_true",
                    help="also promote to Matching, link, and check the DOL SHA")
    ap.add_argument("--keep", action="store_true",
                    help="keep changes in the tree if all checks pass")
    a = ap.parse_args()

    src = ROOT / a.source
    cand = Path(a.candidate)
    if not src.exists():
        sys.exit(f"{a.source} not found")
    if not cand.exists():
        sys.exit(f"{a.candidate} not found")

    new_text = read_raw(cand)
    reject_asm(new_text, a.candidate)

    unit = unit_name(a.source)
    obj = ROOT / "build" / "GC6E01" / Path(a.source).with_suffix(".o")

    backup_c = read_raw(src)
    backup_cfg = read_raw(CONFIGURE)
    ok = False
    try:
        before, _ = measure(unit, a.symbol)
        src.write_text(new_text, encoding="utf-8", newline="")
        if obj.exists():
            obj.unlink()
        r = sh(["ninja", str(obj.relative_to(ROOT)).replace("\\", "/")])
        if not obj.exists():
            print("COMPILE FAILED")
            print(r.stdout[-1500:] or r.stderr[-1500:])
            return 1

        pct, deltas = measure(unit, a.symbol)
        if pct is None:
            print(f"could not measure {a.symbol} in {unit}")
            return 1
        arrow = "" if before is None else f"  (was {before:.2f}%)"
        print(f"{a.symbol}: {pct:.2f}%{arrow}   deltas={len(deltas)}")
        for k, t, o in deltas[:12]:
            print(f"  >>{k:>4}  {t:<36} | {o}")

        if pct < 100.0:
            print("NOT byte-exact -- nothing to link.")
            return 1

        if not a.link:
            print("100% on .text. Re-run with --link to check the DOL gate.")
            ok = a.keep
            return 0

        changed = promote(a.source)
        print(f"promoted to Matching: {changed}")
        sh([sys.executable, "configure.py", "--no-progress"])
        r = sh(["ninja"])
        blob = r.stdout + r.stderr
        if "multiply-defined" in blob:
            names = sorted(set(re.findall(r"multiply-defined: '([^']+)'", blob)))
            print(f"LINK FAILED -- multiply-defined: {names[:6]}")
            print("  (unit probably shares a master .c with an already-linked sibling)")
            return 1
        if "main.dol: OK" not in blob:
            tail = [x for x in blob.splitlines()
                    if "FAILED" in x or "main.dol" in x][:6]
            print("DOL SHA GATE FAILED:")
            for t in tail:
                print(f"    {t}")
            return 1

        print("main.dol: OK  -- byte-exact and linked.")
        ok = True
        return 0
    finally:
        if ok and a.keep:
            print("kept changes in tree (--keep).")
        else:
            src.write_text(backup_c, encoding="utf-8", newline="")
            CONFIGURE.write_text(backup_cfg, encoding="utf-8", newline="")
            sh([sys.executable, "configure.py", "--no-progress"])
            sh(["ninja"])
            print("restored tree to pre-verification state.")


if __name__ == "__main__":
    sys.exit(main())
