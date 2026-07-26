#!/usr/bin/env python3
"""Build a self-contained tarball that runs the match loop inside ChatGPT's
code-interpreter sandbox (Linux x86-64, no network).

The sandbox gets a real compile+measure loop, not just a text prompt:

    wibo            Linux ELF loader for Win32 PE  -> runs mwcceppc.exe
    mwcceppc.exe    the actual CodeWarrior compiler
    objdiff-cli     Linux ELF, measures match % against the target object
    target .o       the original DOL's object for this unit
    headers         include/ tree needed to compile
    try.sh          one-shot: edit unit.c, run ./try.sh, get a match %

So the model can iterate against ground truth instead of guessing, which is
the whole point -- a wrong guess costs it one turn, not one of your reviews.

Usage:
    python tools/decomp_work/handoff/make_sandbox.py --source src/game/main_retrace.c
    python tools/decomp_work/handoff/make_sandbox.py --source src/... --symbol fn_X --out kit.tar.gz

IMPORTANT: upload and drive this manually in the ChatGPT UI. Do not script the
browser against your account -- that is what OpenAI's terms prohibit. Manual use
of file upload + code interpreter is a supported product feature.

The sandbox result is ADVISORY. A 100% there still has to come home and pass
`verify.py`, because the real gate is main.dol matching config/GC6E01/build.sha1.
"""
import argparse
import json
import os
import re
import shutil
import stat
import sys
import tarfile
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OBJDIFF_JSON = ROOT / "objdiff.json"
NINJA = ROOT / "build.ninja"


def unit_for(source: str) -> dict:
    if not OBJDIFF_JSON.exists():
        sys.exit("objdiff.json missing -- run: python configure.py --no-progress")
    want = source.replace("\\", "/")
    cfg = json.loads(OBJDIFF_JSON.read_text())
    for u in cfg["units"]:
        bp = (u.get("base_path") or "").replace("\\", "/")
        # base_path is build/GC6E01/<source-with-.o>
        if bp.endswith(Path(want).with_suffix(".o").as_posix()):
            return u
    sys.exit(f"no objdiff unit whose base_path corresponds to {want}")


def ninja_flags(source: str) -> dict:
    obj = str(Path(source).with_suffix(".o")).replace("/", "\\")
    lines = NINJA.read_text(errors="replace").splitlines()
    for i, ln in enumerate(lines):
        if ln.startswith("build ") and obj in ln.replace("/", "\\"):
            blob = "\n".join(lines[i + 1 : i + 25])
            blob = blob.split("\nbuild ")[0]
            mv = re.search(r"mw_version\s*=\s*(\S+)", blob)
            cf = re.search(r"cflags\s*=\s*((?:.|\n)*?)(?:\n  \w+ =|\Z)", blob)
            return {
                "mw_version": mv.group(1).replace("\\", "/") if mv else "GC/1.3",
                "cflags": " ".join(cf.group(1).replace("$\n", " ").split()) if cf else "",
            }
    return {"mw_version": "GC/1.3", "cflags": ""}


def collect_includes(cflags: str) -> list:
    dirs = []
    for m in re.finditer(r"-i\s+(\S+)", cflags):
        d = ROOT / m.group(1)
        if d.is_dir():
            dirs.append(m.group(1))
    if "include" not in dirs and (ROOT / "include").is_dir():
        dirs.append("include")
    return dirs


TRY_SH = r"""#!/bin/sh
# One-shot compile + measure. Edit unit.c, then: ./try.sh
cd "$(dirname "$0")"
SYMBOL='__SYMBOL__'
chmod +x wibo objdiff-cli 2>/dev/null || true
rm -f out.o result.json

./wibo compiler/mwcceppc.exe __CFLAGS__ -c -o out.o unit.c 2>&1 | \
    grep -viE 'license|evaluation' || true

if [ ! -f out.o ]; then
    echo "COMPILE FAILED -- fix the C above before measuring"
    exit 1
fi

./objdiff-cli diff -1 target.o -2 out.o -o result.json --format json "$SYMBOL" \
    >/dev/null 2>&1 || true

python3 - "$SYMBOL" <<'PY'
import json,sys
sym=sys.argv[1]
try:
    d=json.load(open('result.json'))
except Exception:
    print("objdiff produced no result"); sys.exit(1)
def side(k):
    return {s['name']:s for s in d.get(k,{}).get('symbols',[])}
L,R=side('left'),side('right')
s=L.get(sym)
if not s:
    print("symbol %r not found in target; present: %s"%(sym,list(L)[:8])); sys.exit(1)
print("MATCH %.2f%%  %s"%(s.get('match_percent',-1), sym))
li=s.get('instructions') or []
ri=(R.get(sym) or {}).get('instructions') or []
f=lambda i:(i.get('instruction',{}).get('formatted','') if i else '')
n=0
for k in range(max(len(li),len(ri))):
    a,b=f(li[k] if k<len(li) else None), f(ri[k] if k<len(ri) else None)
    if a!=b:
        print("  >>%3d  %-36s | %s"%(k,a,b)); n+=1
print("diffs:",n)
print("PERFECT" if n==0 else "not yet")
PY
"""

README = """# Decompilation sandbox — {symbol} ({source})

Everything needed to compile and score this function offline. No network.

## Run it

    ./try.sh

Prints `MATCH xx.xx%` plus every differing instruction as
`>>  idx   TARGET | OURS`. Your job: get it to 100.00% with 0 diffs.

Edit **unit.c** only. Then re-run `./try.sh`. Iterate.

## Hard rules (a violation makes the result worthless)

- **C89**: all declarations before statements in each block.
- **No inline asm. No `.inc` includes. No asm wrappers.** The project rejects
  any match built from assembly; it will not be merged.
- Types: `u8/s8/u16/s16/u32/s32/f32/f64`. `cmpwi` = signed, `cmplwi` = unsigned.
- Don't touch headers. Change only `unit.c`.
- Float constants come from named externs (`extern f32 lbl_XXXX;`), never literals.

## Compiler

    {mw_version}
    {cflags}

If you believe the flags themselves are wrong, say so explicitly in your answer
rather than silently working around it — flag changes are sometimes the fix.
For example a target prologue using `stmw`/`lmw` where we emit paired
`stw`/`lwz` means the unit needs `-O4,s` (`-use_lmw_stmw on` alone is not
enough at 2-3 registers).

## Deliverable

Return the complete contents of `unit.c`. Report the exact MATCH % you reached
and paste the final `try.sh` output. Do not claim a match you did not observe.

Note: 100% here is necessary but not sufficient. The authoritative gate is the
full link back home (`main.dol` vs `build.sha1`), which this sandbox cannot run.
"""


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", required=True)
    ap.add_argument("--symbol")
    ap.add_argument("--out")
    a = ap.parse_args()

    src_rel = a.source.replace("\\", "/")
    src_abs = ROOT / src_rel
    if not src_abs.exists():
        sys.exit(f"{src_rel} not found")

    unit = unit_for(src_rel)
    target = ROOT / unit["target_path"]
    if not target.exists():
        sys.exit(f"target object {unit['target_path']} not built -- run `ninja`")

    flags = ninja_flags(src_rel)
    mw = flags["mw_version"]
    compiler_dir = ROOT / "build" / "compilers" / mw
    if not compiler_dir.exists():
        sys.exit(f"compiler {mw} not found at {compiler_dir}")

    wibo = ROOT / "build" / "tools" / "wibo"
    objdiff = ROOT / "build" / "tools" / "objdiff-cli"
    for p in (wibo, objdiff):
        if not p.exists():
            sys.exit(f"{p} missing (need the Linux ELF build for the sandbox)")

    symbol = a.symbol or "<SYMBOL>"
    out = Path(a.out) if a.out else ROOT / f"sandbox_{Path(src_rel).stem}.tar.gz"

    with tempfile.TemporaryDirectory() as td:
        kit = Path(td) / "kit"
        kit.mkdir()
        shutil.copy2(src_abs, kit / "unit.c")
        shutil.copy2(target, kit / "target.o")
        shutil.copy2(wibo, kit / "wibo")
        shutil.copy2(objdiff, kit / "objdiff-cli")
        (kit / "compiler").mkdir()
        for f in compiler_dir.iterdir():
            if f.is_file():
                shutil.copy2(f, kit / "compiler" / f.name)

        inc_dirs = collect_includes(flags["cflags"])
        sandbox_cflags = flags["cflags"]
        for d in inc_dirs:
            dst = kit / d
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copytree(ROOT / d, dst, dirs_exist_ok=True)

        (kit / "try.sh").write_text(
            TRY_SH.replace("__CFLAGS__", sandbox_cflags).replace("__SYMBOL__", symbol),
            encoding="utf-8", newline="\n")
        os.chmod(kit / "try.sh", 0o755)
        (kit / "README.md").write_text(
            README.format(symbol=symbol, source=src_rel,
                          mw_version=mw, cflags=sandbox_cflags),
            encoding="utf-8", newline="\n")

        with tarfile.open(out, "w:gz") as tf:
            for f in sorted(kit.rglob("*")):
                ti = tf.gettarinfo(f, arcname=str(f.relative_to(kit)))
                if f.name in ("wibo", "objdiff-cli", "try.sh") or f.suffix == ".exe":
                    ti.mode = 0o755
                if f.is_file():
                    with open(f, "rb") as fh:
                        tf.addfile(ti, fh)
                else:
                    tf.addfile(ti)

    mb = out.stat().st_size / 1e6
    print(f"wrote {out}  ({mb:.1f} MB)")
    print(f"  unit    {src_rel}")
    print(f"  symbol  {symbol}")
    print(f"  target  {unit['target_path']}")
    print(f"  cc      {mw}")
    if mb > 100:
        print("  WARNING: large upload; consider trimming include dirs")


if __name__ == "__main__":
    main()
