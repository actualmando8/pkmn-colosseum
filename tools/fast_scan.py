#!/usr/bin/env python3
"""Fast scan - only diffs functions within the file's address range."""

import sys
import json
import subprocess
import re
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from match_test import parse_symbols, extract_match_info, OBJDIFF_CLI, PROJECT_ROOT
from compile_check import compile_source, find_target_obj


def fast_scan(src_rel):
    src_path = (PROJECT_ROOT / src_rel).resolve()
    symbols = parse_symbols()

    with open(src_path, "r", errors="replace") as f:
        content = f.read()

    # Get address range from file header
    addr_range = re.search(
        r"Address range:\s*0x([0-9A-Fa-f]+)\s*-\s*0x([0-9A-Fa-f]+)", content
    )
    if not addr_range:
        print(f"ERROR: No address range found in {src_rel}")
        return

    range_start = int(addr_range.group(1), 16)
    range_end = int(addr_range.group(2), 16)

    # Filter to functions in range
    file_symbols = [
        s for s in symbols if s.is_function and range_start <= s.address < range_end
    ]
    file_symbols.sort(key=lambda s: s.address)

    print(f"Source: {src_rel}")
    print(f"Range:  0x{range_start:08X} - 0x{range_end:08X}")
    print(f"Functions in range: {len(file_symbols)}")
    sys.stdout.flush()

    # Compile once
    base_obj = compile_source(src_path)
    target_obj = find_target_obj(src_path)

    if not target_obj.exists() or not OBJDIFF_CLI.exists():
        print("Cannot diff (missing target .o or objdiff-cli)")
        return

    # Test each function
    results = []
    for i, sym in enumerate(file_symbols):
        if (i + 1) % 25 == 0:
            sys.stderr.write(f"  Progress: {i+1}/{len(file_symbols)}...\n")
            sys.stderr.flush()

        cmd = [
            str(OBJDIFF_CLI), "diff",
            "-1", str(target_obj),
            "-2", str(base_obj),
            "-o", "-",
            "--format", "json",
            sym.name,
        ]
        res = subprocess.run(cmd, capture_output=True, text=True, cwd=str(PROJECT_ROOT))

        info = {"name": sym.name, "address": sym.address, "size": sym.size}
        if res.returncode == 0 and res.stdout.strip():
            try:
                diff_json = json.loads(res.stdout)
                match = extract_match_info(diff_json, sym.name)
                info.update(match)
            except json.JSONDecodeError:
                info["match_percent"] = -1
        else:
            info["match_percent"] = -1
        results.append(info)

    # Summary table
    print()
    print("=" * 70)
    hdr = f"{'Function':<35} {'Address':<12} {'Size':<8} {'Match':>8}"
    print(hdr)
    print(f"{'-'*35} {'-'*12} {'-'*8} {'-'*8}")

    total_matched = 0
    total_tested = 0
    near_matches = []

    for r in results:
        name = r["name"][:34]
        addr = f"0x{r['address']:08X}"
        size = f"0x{r['size']:X}" if r.get("size") else "?"
        if r["match_percent"] < 0:
            match_str = "N/A"
        else:
            match_str = f"{r['match_percent']:.1f}%"
            total_tested += 1
            if r["match_percent"] == 100.0:
                total_matched += 1
            elif r["match_percent"] >= 80.0:
                near_matches.append(r)

        print(f"  {name:<34} {addr:<12} {size:<8} {match_str:>8}")

    print()
    print(f"  Total: {total_matched}/{total_tested} functions matching "
          f"({len(results)} in file)")
    if near_matches:
        print(f"  Near matches (>=80%): {len(near_matches)}")
        for r in near_matches:
            print(f"    {r['name']} @ 0x{r['address']:08X}: {r['match_percent']:.1f}%")

    sys.stdout.flush()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python tools/fast_scan.py src/game/FILE.c")
        sys.exit(1)
    fast_scan(sys.argv[1])
