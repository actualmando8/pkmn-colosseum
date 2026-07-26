#!/usr/bin/env python3
"""Generate a self-contained decompilation brief for one function.

The brief is everything an external model (Codex CLI, a ChatGPT Project, any
agent) needs to attempt a match WITHOUT access to this repo or the toolchain:
the target disassembly, our current output, the exact instruction deltas, the
compile flags in force, and the levers that apply to the observed diff shape.

Usage:
    python tools/decomp_work/handoff/gen_brief.py --source src/game/main_retrace.c --symbol fn_8000B0C4
    python tools/decomp_work/handoff/gen_brief.py --source src/game/main_retrace.c --list
    python tools/decomp_work/handoff/gen_brief.py --worst          # brief for the closest near-miss

Writes markdown to stdout, or to --out.

Nothing here mutates the tree. Pair with verify.py, which is the actual gate.
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
REPORT = ROOT / "build" / "GC6E01" / "report.json"
NINJA = ROOT / "build.ninja"


def objdiff_cli() -> Path:
    for c in ("objdiff-cli.exe", "objdiff-cli"):
        p = ROOT / "build" / "tools" / c
        if p.exists():
            return p
    sys.exit("objdiff-cli not found under build/tools/ -- run `ninja` first")


def load_units() -> list:
    if not REPORT.exists():
        sys.exit(f"{REPORT} missing -- run: ninja all_source build/GC6E01/report.json")
    return json.loads(REPORT.read_text())["units"]


def find_unit(units: list, source: str) -> dict:
    want = source.replace("\\", "/")
    for u in units:
        if (u["metadata"].get("source_path") or "").replace("\\", "/") == want:
            return u
    sys.exit(f"no unit in report.json with source_path == {want}")


def obj_path(source_path: str) -> Path:
    """src/game/foo.c -> build/GC6E01/src/game/foo.o (OUR compiled object).

    Layout, per objdiff.json:
      build/GC6E01/obj/  = TARGET objects, split from the original DOL
      build/GC6E01/src/  = what we compile (objdiff calls this "base")
      build/GC6E01/base/ = whole-TU extractions (e.g. trk/gdev_cc.o)

    Note the `.comment` compiler version on TARGET objects is uniform dtk
    output (everything reads 2.3.0.1) and does NOT indicate the original
    per-TU compiler -- fn_801EEE6C matches byte-exactly against a 2.3.0.1
    target using a compiler that emits 2.4.2.1. Never pick mw_version from it.
    """
    rel = Path(source_path).with_suffix(".o")
    return ROOT / "build" / "GC6E01" / rel


def unit_flags(source_path: str) -> dict:
    """Recover mw_version and cflags for this object from build.ninja."""
    if not NINJA.exists():
        return {}
    obj = str(Path(source_path).with_suffix(".o")).replace("/", "\\")
    text = NINJA.read_text(errors="replace").splitlines()
    for i, line in enumerate(text):
        if line.startswith("build ") and obj in line.replace("/", "\\"):
            out, block = {}, []
            for line2 in text[i + 1 : i + 25]:
                if line2.startswith("build "):
                    break
                block.append(line2)
            blob = "\n".join(block)
            mv = re.search(r"mw_version\s*=\s*(\S+)", blob)
            if mv:
                out["mw_version"] = mv.group(1).replace("\\", "/")
            cf = re.search(r"cflags\s*=\s*((?:.|\n)*?)(?:\n  \w+ =|\Z)", blob)
            if cf:
                out["cflags"] = " ".join(
                    cf.group(1).replace("$\n", " ").split()
                )
            return out
    return {}


def run_diff(unit_name: str, symbol: str) -> dict:
    cli = objdiff_cli()
    with tempfile.TemporaryDirectory() as td:
        out = Path(td) / "d.json"
        subprocess.run(
            [str(cli), "diff", "-p", str(ROOT), "-u", unit_name,
             "-o", str(out), "--format", "json", symbol],
            cwd=ROOT, capture_output=True,
        )
        if not out.exists():
            sys.exit(f"objdiff produced no output for {unit_name}:{symbol}")
        return json.loads(out.read_text())


def instrs(side: dict, symbol: str) -> list:
    for s in side.get("symbols", []):
        if s["name"] == symbol:
            return s.get("instructions") or []
    return []


def fmt(i) -> str:
    if not i:
        return ""
    return (i.get("instruction") or {}).get("formatted", "")


# Diff-shape -> lever mapping. Each entry: (name, predicate over the delta rows).
# Derived from measured results in this repo; see LEVERS.md for provenance.
LEVERS = [
    ("stmw/lmw prologue",
     lambda d: any(("stmw" in a or "lmw" in a) and ("stw" in b or "lwz" in b)
                   for a, b in d),
     "Target uses stmw/lmw, we emit paired stw/lwz. Add `-O4,s` to the unit's "
     "extra_cflags -- `-use_lmw_stmw on` alone is NOT enough at 2-3 registers. "
     "This is exactly what fixed fn_801EEE6C (74.74% -> 100%)."),
    ("addi rD,rS,0 vs mr",
     lambda d: any(re.search(r"addi\s+r\d+,\s*r\d+,\s*0x0", a) and "mr " in b
                   for a, b in d),
     "Zero-offset pointer arithmetic folded to a register move. KNOWN WALL on "
     "gdev_cc_write: &p[0], (char*)((u32)p+0), struct &p->m0, peephole off, and "
     "every opt level all still fold. Do not burn time here."),
    ("clrlwi / mask",
     lambda d: any("clrlwi" in a or "clrlwi" in b for a, b in d),
     "Mask-width mismatch. Try `x & 0xFF` on an s32 rather than a (u8) cast; a "
     "local `extern fn(..., u8)` prototype forces clrlwi at the callsite."),
    ("branch inversion",
     lambda d: any(re.match(r"b(eq|ne|lt|ge|gt|le)\b", a) and
                   re.match(r"b(eq|ne|lt|ge|gt|le)\b", b) and a.split()[0] != b.split()[0]
                   for a, b in d),
     "Condition polarity. Invert the C test (early-return the negated case) -- "
     "e.g. `if (x != K) return 0; call();` instead of `if (x == K) { call(); } return 0;`"),
    ("float const / @sda21",
     lambda d: any("@sda21" in a or "@sda2" in a or "lfs" in a for a, b in d),
     "Float constant addressing. Use a named `extern f32 lbl_XXXX;` and reference "
     "it, never a float literal. An anonymous @NNN conv-literal usually caps <100%."),
    ("scheduling / reorder only",
     lambda d: all(sorted(x for x in (a, b) if x) and a.split()[:1] == b.split()[:1]
                   for a, b in d) if d else False,
     "Same opcodes in a different order = instruction scheduling. Try reordering "
     "declarations; `#pragma scheduling off` occasionally helps. Often not steerable."),
]


def build_brief(unit: dict, symbol: str) -> str:
    name, src = unit["name"], unit["metadata"]["source_path"]
    d = run_diff(name, symbol)
    L, R = instrs(d["left"], symbol), instrs(d["right"], symbol)
    pct = next((s.get("match_percent") for s in d["left"].get("symbols", [])
                if s["name"] == symbol), None)

    rows, deltas = [], []
    for k in range(max(len(L), len(R))):
        a, b = fmt(L[k] if k < len(L) else None), fmt(R[k] if k < len(R) else None)
        rows.append((k, a, b, a != b))
        if a != b:
            deltas.append((a, b))

    flags = unit_flags(src)
    hits = [(n, why) for n, pred, why in LEVERS
            if _safe(pred, deltas)]

    out = []
    out.append(f"# Decompilation task: `{symbol}`\n")
    out.append(f"- **Source file**: `{src}`")
    out.append(f"- **objdiff unit**: `{name}`")
    out.append(f"- **Current match**: {pct:.2f}%" if pct is not None else "- Current match: n/a")
    out.append(f"- **Instruction deltas**: {len(deltas)} of {max(len(L), len(R))}")
    if flags.get("mw_version"):
        out.append(f"- **Compiler**: `{flags['mw_version']}`")
    if flags.get("cflags"):
        out.append(f"- **cflags**: `{flags['cflags']}`")
    out.append("")

    out.append("## Goal\n")
    out.append("Rewrite the C body so it compiles byte-identically to the target.")
    out.append("Constraints that are NOT negotiable:\n")
    out.append("- **C89**: every declaration precedes every statement in its block.")
    out.append("- **No inline asm, no `.inc` includes, no asm wrappers.** A match that")
    out.append("  uses assembly does not count and will be rejected.")
    out.append("- Types are `u8/s8/u16/s16/u32/s32/f32/f64`. `cmpwi`=signed, `cmplwi`=unsigned.")
    out.append("- Do not edit headers or other units; change only this .c (and, if")
    out.append("  justified, this unit's `extra_cflags` in configure.py).\n")

    out.append("## Instruction diff (`>>` = mismatch)\n")
    out.append("```")
    out.append(f"{'#':>4}  {'TARGET (want)':<38} {'OURS (have)':<38}")
    for k, a, b, bad in rows:
        out.append(f"{'>>' if bad else '  '}{k:>3}  {a:<38} {b:<38}".rstrip())
    out.append("```\n")

    if hits:
        out.append("## Levers matching this diff shape\n")
        for n, why in hits:
            out.append(f"**{n}** — {why}\n")
    else:
        out.append("## Levers\n\nNo lever pattern matched automatically; see LEVERS.md.\n")

    p = ROOT / src
    if p.exists():
        out.append("## Current C source\n")
        out.append(f"```c\n{p.read_text(errors='replace').rstrip()}\n```\n")

    out.append("## How your answer is verified\n")
    out.append("Return the complete replacement `.c` file, nothing else. It is checked by:\n")
    out.append("```bash")
    out.append(f"python tools/decomp_work/handoff/verify.py --source {src} \\")
    out.append(f"    --symbol {symbol} --candidate <your-file.c>")
    out.append("```")
    out.append("which rebuilds the object, re-measures, and -- if the unit is promoted --")
    out.append("relinks and checks `main.dol` against `config/GC6E01/build.sha1`.")
    out.append("**objdiff reporting 100% is not sufficient; the DOL SHA is the gate.**")
    return "\n".join(out)


def _safe(pred, deltas) -> bool:
    try:
        return bool(pred(deltas))
    except Exception:
        return False


def list_symbols(unit: dict) -> None:
    src = unit["metadata"]["source_path"]
    o = obj_path(src)
    dtk = ROOT / "build" / "tools" / ("dtk.exe" if os.name == "nt" else "dtk")
    if not o.exists():
        sys.exit(f"{o} not built yet -- run: ninja {o.relative_to(ROOT)}")
    r = subprocess.run([str(dtk), "elf", "info", str(o)],
                       capture_output=True, text=True)
    print(f"# symbols in {src}\n")
    for line in r.stdout.splitlines():
        if "|" in line and ".text" in line:
            print(line.strip())


def pick_worst(units: list):
    """Closest source-backed, standalone (non-shim, non-asm) near-miss."""
    best = None
    for u in units:
        md, m = u["metadata"], u["measures"]
        sp = md.get("source_path")
        if not sp or md.get("auto_generated") or md.get("complete"):
            continue
        tf, mf = m.get("total_functions", 0), m.get("matched_functions", 0)
        if not tf or mf >= tf:
            continue
        p = ROOT / sp
        if not p.exists():
            continue
        t = p.read_text(errors="replace")
        if re.search(r'#include\s+"[^"]+\.c"', t):   # shim over a shared master
            continue
        fz = m.get("fuzzy_match_percent", 0)
        if best is None or fz > best[0]:
            best = (fz, u)
    return best[1] if best else None


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--source")
    ap.add_argument("--symbol")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--worst", action="store_true")
    ap.add_argument("--out")
    a = ap.parse_args()

    units = load_units()
    if a.worst:
        u = pick_worst(units)
        if not u:
            sys.exit("no standalone near-miss units found")
        print(f"# closest standalone near-miss: {u['metadata']['source_path']}",
              file=sys.stderr)
        if not a.symbol:
            list_symbols(u)
            return
    else:
        if not a.source:
            sys.exit("--source is required (or use --worst)")
        u = find_unit(units, a.source)

    if a.list:
        list_symbols(u)
        return
    if not a.symbol:
        sys.exit("--symbol is required (use --list to see candidates)")

    text = build_brief(u, a.symbol)
    if a.out:
        Path(a.out).write_text(text, encoding="utf-8")
        print(f"wrote {a.out} ({len(text)} bytes)", file=sys.stderr)
    else:
        print(text)


if __name__ == "__main__":
    main()
