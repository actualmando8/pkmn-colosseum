#!/usr/bin/env python3
"""Vary CW flags (holding version 1.3) on a big 0-progress file and count
matches per flag variant. Closes the 'is there any config unlock' question."""
import json, re, subprocess, sys, os
from pathlib import Path
ROOT = Path(r"C:\Users\douglaswhittingham\pkmn-colosseum")
sys.path.insert(0, str(ROOT/"tools"))
import compile_check as cc
from headless_subprocess import run as run_tool

SRC = ROOT/"src/game/colosseum_battle.c"
TARGET = cc.PROJECT_ROOT/"build"/"GC6E01"/"obj"/"auto_01_800055E0_text.o"

VARIANTS = [
    ("baseline (-O4,p)", None),
    ("-O3,p", ["-O3,p"]),
    ("-O2,p", ["-O2,p"]),
    ("-O1,p", ["-O1,p"]),
    ("-O0", ["-O0"]),
    ("-inline off", ["-inline", "off"]),
    ("-inline all", ["-inline", "all"]),
    ("-inline auto", ["-inline", "auto"]),
    ("-func_align 32", ["-func_align", "32"]),
    ("-ipa off", ["-ipa", "off"]),
    ("-sym off", ["-sym", "off"]),
    ("-O4,p -inline off", ["-inline", "off"]),  # combos handled by override note below
]

def base_obj():
    try:
        return cc.source_to_base_obj(SRC.resolve())
    except Exception:
        return cc.PROJECT_ROOT/"build"/"GC6E01"/"base"/"game"/"colosseum_battle.o"

def count(flags):
    try:
        cc.compile_source(SRC, compiler_version="1.3", extra_flags=flags)
    except SystemExit:
        return None, None, "compile failed"
    except Exception as e:
        return None, None, f"compile err {e}"
    bo = base_obj()
    if not bo.exists():
        return None, None, "no base obj"
    r = run_tool([str(cc.OBJDIFF_CLI),"diff","-1",str(TARGET),"-2",str(bo),"-o","-",
                  "--format","json","-c","ppc.calculatePoolRelocations=false"],
                 capture_output=True, text=True)
    if r.returncode != 0:
        return None, None, "objdiff fail"
    try:
        j = json.loads(r.stdout)
    except Exception:
        return None, None, "json fail"
    syms = [s for s in j.get("right",{}).get("symbols",[])
            if s.get("kind")=="SYMBOL_FUNCTION" and s.get("name","").startswith("fn_")]
    total = len(syms)
    matched = sum(1 for s in syms if (s.get("match_percent") or 0) >= 100.0)
    return matched, total, ""

print(f"==== {SRC.name} : matches per FLAG variant (version 1.3) ====")
best = (-1, None)
for label, flags in VARIANTS:
    m, t, err = count(flags)
    if m is None:
        print(f"  {label:22s}  -- ({err})")
    else:
        print(f"  {label:22s}  {m:>4}/{t} matched")
        if m > best[0]: best = (m, label)
print(f"  BEST: {best[1]} ({best[0]} matches)")
# restore base obj to default flags so the committed measurement path is clean
cc.compile_source(SRC, compiler_version="1.3")
print("  (restored default-flag base obj)")
