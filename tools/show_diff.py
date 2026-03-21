#!/usr/bin/env python3
"""Show instruction-level diff for a specific function."""

import json
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent


def format_instr(instr_obj):
    """Format an instruction from objdiff JSON."""
    instr = instr_obj.get("instruction", instr_obj)
    return instr.get("formatted", "???")


def show_diff(sym_name, target_obj, base_obj):
    cmd = [
        str(PROJECT_ROOT / "tools" / "objdiff-cli.exe"), "diff",
        "-1", str(target_obj),
        "-2", str(base_obj),
        "-o", "-",
        "--format", "json",
        sym_name,
    ]
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=str(PROJECT_ROOT))
    if r.returncode != 0:
        print(f"objdiff failed: {r.stderr[:200]}")
        return

    data = json.loads(r.stdout)

    left_instrs = []
    right_instrs = []
    left_diffs = []
    right_diffs = []

    for lsym in data["left"]["symbols"]:
        if lsym.get("name") == sym_name:
            mp = lsym.get("match_percent", 0)
            print(f"Match: {mp:.1f}%")
            for instr in lsym["instructions"]:
                left_instrs.append(format_instr(instr))
                left_diffs.append(instr.get("diff_kind", "DIFF_NONE"))
            break

    for rsym in data["right"]["symbols"]:
        if rsym.get("name") == sym_name:
            for instr in rsym["instructions"]:
                right_instrs.append(format_instr(instr))
                right_diffs.append(instr.get("diff_kind", "DIFF_NONE"))
            break

    # Print side by side
    max_left = max((len(s) for s in left_instrs), default=20)
    max_left = min(max_left, 40)

    print(f"\n{'TARGET (original)':<{max_left+4}}  {'BASE (ours)'}")
    print(f"{'-'*(max_left+4)}  {'-'*40}")

    max_len = max(len(left_instrs), len(right_instrs))
    for i in range(max_len):
        l_text = left_instrs[i] if i < len(left_instrs) else ""
        r_text = right_instrs[i] if i < len(right_instrs) else ""
        l_dk = left_diffs[i] if i < len(left_diffs) else "DIFF_NONE"
        r_dk = right_diffs[i] if i < len(right_diffs) else "DIFF_NONE"

        if l_dk in (None, "DIFF_NONE") and r_dk in (None, "DIFF_NONE"):
            marker = "  "
        else:
            marker = ">>"
        print(f"{marker} {l_text:<{max_left+2}}  {r_text}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python tools/show_diff.py SYMBOL [src_file]")
        sys.exit(1)

    sym_name = sys.argv[1]
    if len(sys.argv) >= 3:
        src_rel = sys.argv[2]
    else:
        # Auto-detect
        sys.path.insert(0, str(PROJECT_ROOT / "tools"))
        from match_test import parse_symbols, find_source_for_symbol
        from compile_check import compile_source, find_target_obj
        symbols = parse_symbols()
        sym = None
        for s in symbols:
            if s.name == sym_name:
                sym = s
                break
        if not sym:
            print(f"Symbol {sym_name} not found")
            sys.exit(1)
        src_path = find_source_for_symbol(sym)
        if not src_path:
            print(f"No source for {sym_name}")
            sys.exit(1)
        base_obj = compile_source(src_path)
        target_obj = find_target_obj(src_path)
        show_diff(sym_name, target_obj, base_obj)
