#!/usr/bin/env python3
"""
batch_scan.py - Batch scan multiple source files for match percentages.

Does ONE full-file objdiff for each source file and extracts per-function
match percentages from the JSON output. Much faster than per-symbol diffs.
"""

import sys
import json
import subprocess
import re
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from match_test import parse_symbols, OBJDIFF_CLI, PROJECT_ROOT
from compile_check import compile_source, find_target_obj


TEMP_DIR = Path("C:/Users/DOUGLA~1/AppData/Local/Temp")


def extract_all_matches(diff_json, target_symbols):
    """Extract match percentages for all target symbols from a full diff."""
    left_symbols = diff_json.get("left", {}).get("symbols", [])

    # Build lookup by name
    left_by_name = {}
    for lsym in left_symbols:
        name = lsym.get("name", "")
        left_by_name[name] = lsym

    results = []
    for sym in target_symbols:
        info = {"name": sym.name, "address": sym.address, "size": sym.size}
        lsym = left_by_name.get(sym.name)

        if lsym and lsym.get("instructions"):
            instructions = lsym["instructions"]
            total = len(instructions)
            matched = 0
            for instr in instructions:
                dk = instr.get("diff_kind")
                if dk is None or dk == "DIFF_NONE":
                    matched += 1
            info["match_percent"] = 100.0 * matched / total if total > 0 else 0.0
            info["total_instructions"] = total
            info["matched_instructions"] = matched
        else:
            info["match_percent"] = -1
            info["total_instructions"] = 0
            info["matched_instructions"] = 0

        results.append(info)

    return results


def scan_file(src_rel):
    """Scan a source file and print match results."""
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
        return None

    range_start = int(addr_range.group(1), 16)
    range_end = int(addr_range.group(2), 16)

    # Filter to functions in range
    file_symbols = [
        s for s in symbols if s.is_function and range_start <= s.address < range_end
    ]
    file_symbols.sort(key=lambda s: s.address)

    print(f"\nSource: {src_rel}")
    print(f"Range:  0x{range_start:08X} - 0x{range_end:08X}")
    print(f"Functions in range: {len(file_symbols)}")
    sys.stdout.flush()

    # Compile once
    base_obj = compile_source(src_path)
    target_obj = find_target_obj(src_path)

    if not target_obj.exists() or not OBJDIFF_CLI.exists():
        print("Cannot diff (missing target .o or objdiff-cli)")
        return None

    # Do ONE full-file diff
    diff_output = TEMP_DIR / f"diff_{src_path.stem}.json"
    cmd = [
        str(OBJDIFF_CLI), "diff",
        "-1", str(target_obj),
        "-2", str(base_obj),
        "-o", str(diff_output),
        "--format", "json",
    ]
    print(f"Running full-file diff...")
    sys.stdout.flush()
    res = subprocess.run(cmd, capture_output=True, text=True, cwd=str(PROJECT_ROOT))

    if res.returncode != 0:
        print(f"objdiff failed: {res.stderr[:200]}")
        return None

    # Parse the JSON
    print(f"Parsing diff output ({diff_output.stat().st_size / 1024 / 1024:.1f} MB)...")
    sys.stdout.flush()
    with open(diff_output, "r") as f:
        diff_json = json.load(f)

    # Extract matches
    results = extract_all_matches(diff_json, file_symbols)

    # Print summary table
    print()
    print("=" * 70)
    print(f"{'Function':<35} {'Address':<12} {'Size':<8} {'Match':>8}")
    print(f"{'-'*35} {'-'*12} {'-'*8} {'-'*8}")

    total_matched = 0
    total_tested = 0
    near_matches = []
    partial_matches = []

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
            elif r["match_percent"] >= 50.0:
                partial_matches.append(r)

        print(f"  {name:<34} {addr:<12} {size:<8} {match_str:>8}")

    print()
    print(f"  TOTAL: {total_matched}/{total_tested} functions byte-identical "
          f"({len(results)} in file)")
    print(f"  Near matches (>=80%):    {len(near_matches)}")
    print(f"  Partial matches (>=50%): {len(partial_matches)}")

    if near_matches:
        print(f"\n  === Near matches (>=80%) ===")
        for r in sorted(near_matches, key=lambda x: -x["match_percent"]):
            print(f"    {r['name']:<30} 0x{r['address']:08X}  "
                  f"{r['match_percent']:.1f}%  "
                  f"({r.get('matched_instructions', '?')}/{r.get('total_instructions', '?')})")

    sys.stdout.flush()

    return {
        "file": src_rel,
        "total_functions": len(results),
        "tested": total_tested,
        "matched": total_matched,
        "near_matches": len(near_matches),
        "partial_matches": len(partial_matches),
        "near_match_list": near_matches,
    }


if __name__ == "__main__":
    files = sys.argv[1:] if len(sys.argv) > 1 else [
        "src/game/effect/effect_util.c",
        "src/game/ui/ui_core.c",
        "src/game/gs_thread.c",
        "src/game/gs_model.c",
        "src/game/gs_field_world.c",
    ]

    all_results = []
    for f in files:
        r = scan_file(f)
        if r:
            all_results.append(r)

    # Grand summary
    if len(all_results) > 1:
        print("\n" + "=" * 70)
        print("GRAND SUMMARY")
        print("=" * 70)
        total_funcs = sum(r["total_functions"] for r in all_results)
        total_tested = sum(r["tested"] for r in all_results)
        total_matched = sum(r["matched"] for r in all_results)
        total_near = sum(r["near_matches"] for r in all_results)

        for r in all_results:
            pct = 100.0 * r["matched"] / r["tested"] if r["tested"] > 0 else 0
            print(f"  {r['file']:<40} {r['matched']:>4}/{r['tested']:<4} "
                  f"({pct:.1f}%)  near:{r['near_matches']}")

        pct = 100.0 * total_matched / total_tested if total_tested > 0 else 0
        print(f"  {'TOTAL':<40} {total_matched:>4}/{total_tested:<4} "
              f"({pct:.1f}%)  near:{total_near}")
