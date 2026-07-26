#!/usr/bin/env python3
"""Build a batch of decomp.me-style scratches for the offline gVisor tooling.

Produces one self-contained task per unmatched function, in the layout the
`decomp offline tooling (mwcc_242_81, gvisor x86_64)` bundle expects:

    code.cpp  ctx.cpp  target.o  metadata.json

Batch layout:

    colosseum_batchNN/
      PROMPT.md          paste-in instructions for the model
      TASKS.md           the task table, ranked, with lever hints
      setup.sh           `./setup.sh 07` stages task 07 into the tooling dir
      include/           shared headers
      build/GC6E01/...   generated headers referenced by -i
      tasks/NN_fn_.../   code.cpp ctx.cpp metadata.json target.o NOTES.md

Usage:
    python tools/decomp_work/handoff/make_batch.py --count 30 --out batch01

Targets are ranked by triage: actionable lever shapes first, known walls
excluded, shared-master shims excluded (they cannot be linked independently),
and units owned by in-flight branches skipped via --exclude.
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CLI = ROOT / "build" / "tools" / ("objdiff-cli.exe" if os.name == "nt" else "objdiff-cli")
DTK = ROOT / "build" / "tools" / ("dtk.exe" if os.name == "nt" else "dtk")
NINJA = ROOT / "build.ninja"

# Compiler in the user's bundle. Our GC/1.3 emits .comment 2.4.2.1, which is
# mwcc build 2.4.2 #81 -- the bundled mwcc_242_81. Units declared with other
# mw_versions are skipped rather than silently compiled with the wrong one.
BUNDLED = {"GC/1.3": "mwcc_242_81"}


def sh(cmd):
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)


def unit_flags(source: str) -> dict:
    obj = str(Path(source).with_suffix(".o")).replace("/", "\\")
    lines = NINJA.read_text(errors="replace").splitlines()
    for i, ln in enumerate(lines):
        if ln.startswith("build ") and obj in ln.replace("/", "\\"):
            blob = "\n".join(lines[i + 1: i + 30]).split("\nbuild ")[0]
            mv = re.search(r"mw_version\s*=\s*(\S+)", blob)
            cf = re.search(r"cflags\s*=\s*((?:.|\n)*?)(?:\n  \w+ =|\Z)", blob)
            return {
                "mw_version": mv.group(1).replace("\\", "/") if mv else "",
                "cflags": " ".join(cf.group(1).replace("$\n", " ").split()) if cf else "",
            }
    return {}


def classify(deltas):
    tags, j = [], " | ".join(f"{a} >> {b}" for a, b in deltas)
    if re.search(r"\b(stmw|lmw)\b", j) and re.search(r"\b(stw|lwz)\b", j):
        tags.append(("ACTIONABLE", "stmw/lmw prologue -- unit likely needs -O4,s"))
    if re.search(r"\bclrlwi\b", j):
        tags.append(("ACTIONABLE", "clrlwi mask width -- try `x & 0xFF` on s32, or a "
                                   "local `extern fn(...,u8)` prototype"))
    if any(re.match(r"b(eq|ne|lt|ge|gt|le)", a or "")
           and (a or "").split()[:1] != (b or "").split()[:1] for a, b in deltas):
        tags.append(("ACTIONABLE", "branch polarity -- invert the C test / early-return"))
    if any(re.search(r"\bmr\.\s", a or "") for a, b in deltas):
        tags.append(("HARD", "record-form mr. -- peephole-on and assign-in-condition "
                             "both measured WORSE; unsolved"))
    if re.search(r"@\d+@sda2", j):
        tags.append(("WALL", "anonymous conv-literal -- typically caps just under 100%"))
    if any(re.search(r"addi\s+r\d+,\s*r\d+,\s*0x0", a or "") and "mr " in (b or "")
           for a, b in deltas):
        tags.append(("WALL", "addi rD,rS,0 vs mr -- every C form measured folds"))
    ops = [((a.split() or [""])[0], (b.split() or [""])[0]) for a, b in deltas]
    if deltas and all(x == y for x, y in ops if x and y):
        tags.append(("REGALLOC", "same opcodes, registers differ -- try declaration order"))
    return tags or [("UNKNOWN", "unclassified diff shape")]


def score(tags, pct, ndelta):
    """Lower is better. Prefer actionable, high match, few deltas."""
    kinds = {k for k, _ in tags}
    base = 0
    if "ACTIONABLE" in kinds:
        base -= 100
    if "REGALLOC" in kinds:
        base -= 20
    if "WALL" in kinds:
        base += 200
    if "HARD" in kinds:
        base += 60
    return base + ndelta * 0.5 - pct


def collect(min_pct, exclude):
    rep = json.loads((ROOT / "build" / "GC6E01" / "report.json").read_text())
    out = []
    for u in rep["units"]:
        md, m = u["metadata"], u["measures"]
        sp = md.get("source_path")
        if not sp or md.get("auto_generated") or md.get("complete"):
            continue
        tf, mf = m.get("total_functions", 0), m.get("matched_functions", 0)
        if not tf or mf >= tf or m.get("fuzzy_match_percent", 0) < min_pct:
            continue
        if any(x in sp for x in exclude):
            continue
        p = ROOT / sp
        if not p.exists():
            continue
        text = p.read_text(errors="replace")
        if re.search(r'#include\s+"[^"]+\.c"', text):
            continue                      # shared-master shim: not independently linkable
        fl = unit_flags(sp)
        if fl.get("mw_version") not in BUNDLED:
            continue                      # wrong compiler for this bundle
        obj = ROOT / "build" / "GC6E01" / Path(sp).with_suffix(".o")
        tgt = ROOT / "build" / "GC6E01" / "obj" / Path(sp).relative_to("src").with_suffix(".o")
        if not obj.exists() or not tgt.exists():
            continue
        r = sh([str(DTK), "elf", "info", str(obj)])
        names = [l.split("|")[-1].strip() for l in r.stdout.splitlines() if "|" in l]
        names = [n for n in names if n and not n.startswith(".") and not n[0].isdigit()]
        if not names:
            continue
        with tempfile.TemporaryDirectory() as td:
            j = Path(td) / "d.json"
            sh([str(CLI), "diff", "-p", str(ROOT), "-u", u["name"],
                "-o", str(j), "--format", "json", names[0]])
            if not j.exists():
                continue
            d = json.loads(j.read_text())
        for s in d["left"].get("symbols", []):
            pct = s.get("match_percent")
            if pct is None or pct >= 100.0:
                continue
            L = s.get("instructions") or []
            R = next(((x.get("instructions") or []) for x in d["right"]["symbols"]
                      if x["name"] == s["name"]), [])
            f = lambda i: (i.get("instruction") or {}).get("formatted", "") if i else ""
            rows = [(k, f(L[k] if k < len(L) else None), f(R[k] if k < len(R) else None))
                    for k in range(max(len(L), len(R)))]
            dl = [(t, o) for _k, t, o in rows if t != o]
            tags = classify(dl)
            out.append({
                "symbol": s["name"], "source": sp, "unit": u["name"],
                "pct": pct, "rows": rows, "deltas": dl, "tags": tags,
                "flags": fl, "target": tgt,
                "score": score(tags, pct, len(dl)),
            })
    out.sort(key=lambda r: r["score"])
    return out


PROMPT = """# Pokemon Colosseum (GC6E01) -- batch of {n} decompilation tasks

You have the offline mwcc_242_81 / gVisor tooling bundle extracted here, plus a
`tasks/` directory of independent jobs. Work them one at a time.

## Loop

```bash
./setup.sh 01          # stage task 01 into this directory
WIBO=./wibo-qemu-i386 ./build.sh
./diff.sh --no-pager --format plain
```

Then edit `code.cpp` and repeat. The asm-differ score beside CURRENT is lower=better;
zero with no differing rows is a full match. `tasks/01_*/NOTES.md` has that task's
exact instruction diff and the levers that fit its shape -- read it first.

When a task is done, save `code.cpp` as `tasks/NN_*/SOLVED_code.cpp` and move on.
Report the final score for every task, including the ones you could not finish.

## Rules -- a violation makes the result unusable

- **C89.** All declarations before any statement in a block.
- **No inline asm, no `.inc` include, no asm wrapper.** These are rejected on
  return by an automated gate. Dead `#if 0` reference asm already in the file is
  fine; adding live asm is not.
- Do not edit `ctx.cpp`, `target.o`, `metadata.json`, or the compiler flags to
  manufacture a match. If you believe the *flags* are genuinely wrong, say so in
  your report instead of changing them -- for a target prologue using `stmw`/`lmw`
  where the build emits paired `stw`/`lwz`, the fix really is adding `-O4,s`, and
  that is worth flagging.
- Types are `u8/s8/u16/s16/u32/s32/f32/f64`. `cmpwi` signed, `cmplwi` unsigned.
- Float constants come from named `extern f32 lbl_XXXX;`, never literals.
- Preserve behavior. A match that changes semantics is worthless.

## What is realistic

These are near-misses, mostly 92-99%. Several are known-hard and are labelled as
such in NOTES.md -- if a task is tagged WALL, spend a couple of attempts at most
and move on; do not burn the batch on it. Tasks tagged ACTIONABLE have a specific
documented lever and are where the wins are.

Honesty matters more than score here: report the number you actually observed
from `./diff.sh`. Do not claim a match you did not see.

## Note on the whole-file layout

`code.cpp` is the complete translation unit, not an isolated function -- that is
deliberate, because register allocation and scheduling depend on the surrounding
code. `diff_label` in `metadata.json` selects the one function being scored.
Other functions in the file already match; if you change them and their score
drops, you have broken more than you fixed.
"""

SETUP = """#!/bin/sh
# ./setup.sh NN -- stage task NN into the tooling directory.
set -e
cd "$(dirname "$0")"
n="$1"
[ -z "$n" ] && { echo "usage: ./setup.sh NN   (see TASKS.md)"; exit 1; }
d=$(ls -d tasks/${n}_* 2>/dev/null | head -1)
[ -z "$d" ] && { echo "no task $n"; exit 1; }
cp "$d/code.cpp" "$d/ctx.cpp" "$d/metadata.json" "$d/target.o" .
echo "staged $d"
sed -n '1,12p' "$d/NOTES.md"
"""


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--count", type=int, default=30)
    ap.add_argument("--min", type=float, default=88.0)
    ap.add_argument("--out", default="colosseum_batch01")
    ap.add_argument("--exclude", default="fight_range_80211A00",
                    help="comma-separated substrings of source paths to skip "
                         "(default skips the unit owned by in-flight sol/ branches)")
    a = ap.parse_args()

    excl = [x for x in a.exclude.split(",") if x]
    print(f"scanning report (min {a.min}%, excluding {excl}) ...", file=sys.stderr)
    cands = collect(a.min, excl)
    print(f"  {len(cands)} unmatched functions available", file=sys.stderr)
    picked = cands[: a.count]
    if not picked:
        sys.exit("nothing to package")

    out = Path(a.out)
    if out.exists():
        shutil.rmtree(out)
    (out / "tasks").mkdir(parents=True)

    for d in ("include", "build/GC6E01/include"):
        s = ROOT / d
        if s.is_dir():
            shutil.copytree(s, out / d, dirs_exist_ok=True)

    table = []
    for i, c in enumerate(picked, 1):
        name = f"{i:02d}_{c['symbol'][:38]}"
        td = out / "tasks" / name
        td.mkdir()
        src = (ROOT / c["source"]).read_text(errors="replace")
        (td / "code.cpp").write_text(src, encoding="utf-8", newline="\n")
        (td / "ctx.cpp").write_text(
            f"/* Context for {c['symbol']} ({c['source']}).\n"
            " * code.cpp is the complete translation unit; it is appended here\n"
            " * by build.sh, so this file intentionally declares nothing.\n"
            " */\n", encoding="utf-8", newline="\n")
        shutil.copy2(c["target"], td / "target.o")
        (td / "metadata.json").write_text(json.dumps({
            "platform": "gc_wii",
            "compiler": BUNDLED[c["flags"]["mw_version"]],
            "compiler_flags": c["flags"]["cflags"],
            "diff_label": c["symbol"],
            "name": f"{c['symbol']} ({c['source']})",
        }, indent=2), encoding="utf-8", newline="\n")

        lines = [f"# {c['symbol']}  --  {c['pct']:.2f}%, {len(c['deltas'])} deltas\n",
                 f"- source: `{c['source']}`", f"- unit: `{c['unit']}`",
                 f"- compiler: `{c['flags']['mw_version']}` -> `{BUNDLED[c['flags']['mw_version']]}`",
                 "", "## Lever hints", ""]
        for k, why in c["tags"]:
            lines.append(f"- **[{k}]** {why}")
        lines += ["", "## Instruction diff (`>>` = mismatch)", "", "```",
                  f"{'#':>4}  {'TARGET (want)':<38} {'OURS (have)':<38}"]
        for k, t, o in c["rows"]:
            lines.append(f"{'>>' if t != o else '  '}{k:>3}  {t:<38} {o:<38}".rstrip())
        lines += ["```", ""]
        (td / "NOTES.md").write_text("\n".join(lines), encoding="utf-8", newline="\n")

        kinds = "/".join(sorted({k for k, _ in c["tags"]}))
        table.append(f"| {i:02d} | `{c['symbol']}` | {c['pct']:.2f}% | "
                     f"{len(c['deltas'])} | {kinds} | `{Path(c['source']).name}` |")

    (out / "PROMPT.md").write_text(PROMPT.format(n=len(picked)),
                                   encoding="utf-8", newline="\n")
    (out / "TASKS.md").write_text(
        "# Tasks (ranked: actionable first, walls last)\n\n"
        "| # | function | match | deltas | shape | file |\n"
        "|---|---|---|---|---|---|\n" + "\n".join(table) + "\n",
        encoding="utf-8", newline="\n")
    sp = out / "setup.sh"
    sp.write_text(SETUP, encoding="utf-8", newline="\n")
    os.chmod(sp, 0o755)

    zp = Path(str(out) + ".zip")
    with zipfile.ZipFile(zp, "w", zipfile.ZIP_DEFLATED) as z:
        for f in sorted(out.rglob("*")):
            if f.is_file():
                zi = zipfile.ZipInfo(str(f.relative_to(out)).replace("\\", "/"))
                zi.external_attr = (0o755 if f.name.endswith(".sh") else 0o644) << 16
                zi.compress_type = zipfile.ZIP_DEFLATED
                z.writestr(zi, f.read_bytes())

    print(f"\nwrote {zp}  ({zp.stat().st_size/1e6:.1f} MB)  {len(picked)} tasks")
    for line in table:
        print("  " + line.replace("|", " ").strip())


if __name__ == "__main__":
    main()
