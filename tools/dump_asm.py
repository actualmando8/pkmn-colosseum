#!/usr/bin/env python3
"""Dump target assembly for a list of symbols."""
import json, subprocess, sys, os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGET = os.path.join(ROOT, "build", "GC6E01", "obj", "auto_01_800055E0_text.o")
BASE = os.path.join(ROOT, "build", "GC6E01", "base", "game", "people", "people_data.o")
CLI = os.path.join(ROOT, "tools", "objdiff-cli.exe")

SYMS = [
    "fn_8014369C", "fn_801436B8", "fn_801436D4", "fn_801436F0",
    "fn_80143718", "fn_80143730", "fn_80143748", "fn_80143760",
    "fn_80143778", "fn_801437A0", "fn_801437B8", "fn_801437E0",
    "fn_801437F8", "fn_80143820", "fn_80143838",
    "fn_80143850", "fn_80143878", "fn_801438A0", "fn_801438C8",
    "fn_801438F0", "fn_80143918", "fn_80143940", "fn_80143968",
    "fn_80143990", "fn_801439B8", "fn_801439D4", "fn_801439F0",
    "fn_80143A0C", "fn_80143A28", "fn_80143A44",
    "fn_80143A6C", "fn_80143A94", "fn_80143ABC", "fn_80143AF0",
    "fn_80143B08", "fn_80143B30", "fn_80143B48", "fn_80143B70",
    "fn_80143B80", "fn_80143B90", "fn_80143BA0", "fn_80143BB0",
    "fn_80143BD0", "fn_80143BE0", "fn_80143BF0", "fn_80143C00",
    "fn_80143C10", "fn_80143C20", "fn_80143C30", "fn_80143C40",
    "fn_80143C50", "fn_80143C68", "fn_80143C80", "fn_80143C98",
    "fn_80143CB0", "fn_80143CC8", "fn_80143CE0", "fn_80143CF8",
    "fn_80144088",
]

for sym in SYMS:
    cmd = [CLI, "diff", "-1", TARGET, "-2", BASE, "-o", "-", "--format", "json", sym]
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT)
    if r.returncode != 0:
        print(f"--- {sym} FAILED ---")
        continue
    try:
        data = json.loads(r.stdout)
    except json.JSONDecodeError:
        print(f"--- {sym} JSON FAIL ---")
        continue

    left = data.get("left", {})
    for ls in left.get("symbols", []):
        if ls.get("name") == sym:
            instrs = ls.get("instructions", [])
            print(f"--- {sym} ({len(instrs)} insns) ---")
            for ins in instrs:
                i = ins.get("instruction", {})
                fmt = i.get("formatted", "")
                print(f"  {fmt}")
            break
