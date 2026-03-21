#!/usr/bin/env python3
"""
batch_match_scan.py - Batch scan all game source files for match percentages.

Scans every .c file under src/game/, compiles each once, then tests all
functions in each file via objdiff-cli. Outputs a CSV summary.

Usage:
    python tools/batch_match_scan.py
"""

import csv
import json
import re
import subprocess
import sys
import time
from pathlib import Path

# ============================================================================
# Project layout
# ============================================================================

PROJECT_ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = PROJECT_ROOT / "build" / "GC6E01"
BASE_DIR = BUILD_DIR / "base"
OBJ_DIR = BUILD_DIR / "obj"
SRC_DIR = PROJECT_ROOT / "src"
CONFIG_DIR = PROJECT_ROOT / "config" / "GC6E01"
TOOLS_DIR = PROJECT_ROOT / "tools"

SYMBOLS_TXT = CONFIG_DIR / "symbols.txt"
OBJDIFF_CLI = TOOLS_DIR / "objdiff-cli.exe"
OBJDIFF_JSON = PROJECT_ROOT / "objdiff.json"

# Import from existing tools
sys.path.insert(0, str(TOOLS_DIR))
from compile_check import (
    compile_source, source_to_base_obj, find_target_obj,
    run_diff_json, DEFAULT_COMPILER_VERSION, get_file_compiler_version,
    PROJECT_ROOT as _
)
from match_test import (
    parse_symbols, extract_match_info, Symbol
)


def find_file_functions(src_path: Path, symbols: list) -> list:
    """Find all function symbols associated with a source file."""
    try:
        with open(src_path, "r", errors="replace") as f:
            content = f.read()
    except OSError:
        return []

    # Extract addresses mentioned in source
    addr_pattern = re.compile(r'Address:\s*0x([0-9A-Fa-f]{8})')
    mentioned_addrs = set()
    for m in addr_pattern.finditer(content):
        mentioned_addrs.add(int(m.group(1), 16))

    # Also look for fn_XXXXXXXX patterns
    fn_pattern = re.compile(r'\bfn_([0-9A-Fa-f]{8})\b')
    for m in fn_pattern.finditer(content):
        mentioned_addrs.add(int(m.group(1), 16))

    # Find matching symbols by address
    file_symbols = []
    for sym in symbols:
        if sym.is_function and sym.address in mentioned_addrs:
            file_symbols.append(sym)

    # Also add symbols whose names appear as function definitions
    func_def_pattern = re.compile(
        r'^(?:\w[\w\s\*]+)\s+(\w+)\s*\([^)]*\)\s*\{', re.MULTILINE
    )
    defined_names = set()
    for m in func_def_pattern.finditer(content):
        defined_names.add(m.group(1))

    for sym in symbols:
        if sym.is_function and sym.name in defined_names:
            if sym not in file_symbols:
                file_symbols.append(sym)

    file_symbols.sort(key=lambda s: s.address)
    return file_symbols


def scan_one_file(src_path: Path, symbols: list) -> dict:
    """Scan a single source file. Returns summary dict."""
    rel = str(src_path.relative_to(PROJECT_ROOT))
    result = {
        "filename": rel,
        "functions_tested": 0,
        "functions_100pct": 0,
        "total_match_sum": 0.0,
        "match_rate": 0.0,
        "error": None,
    }

    file_symbols = find_file_functions(src_path, symbols)
    if not file_symbols:
        result["error"] = "no_functions"
        return result

    # Compile once
    try:
        base_obj = compile_source(src_path, verbose=False)
    except Exception as e:
        result["error"] = f"compile_error: {e}"
        return result

    target_obj = find_target_obj(src_path)
    if not target_obj.exists():
        result["error"] = "no_target_obj"
        return result

    if not OBJDIFF_CLI.exists():
        result["error"] = "no_objdiff"
        return result

    tested = 0
    matched_100 = 0
    match_sum = 0.0

    for sym in file_symbols:
        cmd = [
            str(OBJDIFF_CLI), "diff",
            "-1", str(target_obj),
            "-2", str(base_obj),
            "-o", "-",
            "--format", "json",
            sym.name,
        ]
        try:
            res = subprocess.run(
                cmd, capture_output=True, text=True,
                cwd=str(PROJECT_ROOT), timeout=60
            )
        except subprocess.TimeoutExpired:
            continue

        if res.returncode == 0 and res.stdout.strip():
            try:
                diff_json = json.loads(res.stdout)
                info = extract_match_info(diff_json, sym.name)
                if info["match_percent"] >= 0:
                    tested += 1
                    match_sum += info["match_percent"]
                    if info["match_percent"] == 100.0:
                        matched_100 += 1
            except json.JSONDecodeError:
                pass

    result["functions_tested"] = tested
    result["functions_100pct"] = matched_100
    result["total_match_sum"] = match_sum
    if tested > 0:
        result["match_rate"] = match_sum / tested
    else:
        result["match_rate"] = 0.0

    return result


def main():
    print("Parsing symbols...", flush=True)
    symbols = parse_symbols()
    func_count = sum(1 for s in symbols if s.is_function)
    print(f"  {len(symbols)} symbols total, {func_count} functions")

    # Find all game source files
    game_dir = SRC_DIR / "game"
    src_files = sorted(game_dir.rglob("*.c"))
    print(f"\nFound {len(src_files)} source files in src/game/")
    print(f"{'='*80}\n", flush=True)

    results = []
    start_total = time.time()

    for i, src_file in enumerate(src_files, 1):
        rel = str(src_file.relative_to(PROJECT_ROOT))
        print(f"[{i:3d}/{len(src_files)}] Scanning {rel}...", end="", flush=True)
        start = time.time()

        r = scan_one_file(src_file, symbols)
        elapsed = time.time() - start

        if r["error"]:
            print(f" {r['error']} ({elapsed:.1f}s)")
        else:
            print(f" {r['functions_tested']} funcs, "
                  f"{r['functions_100pct']} at 100%, "
                  f"avg {r['match_rate']:.1f}% ({elapsed:.1f}s)")

        results.append(r)

    total_elapsed = time.time() - start_total
    print(f"\n{'='*80}")
    print(f"Completed in {total_elapsed:.1f}s\n")

    # Write CSV
    csv_path = PROJECT_ROOT / "build" / "match_results.csv"
    csv_path.parent.mkdir(parents=True, exist_ok=True)
    with open(csv_path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["filename", "functions_tested", "functions_100pct",
                          "match_rate", "error"])
        for r in results:
            writer.writerow([
                r["filename"],
                r["functions_tested"],
                r["functions_100pct"],
                f"{r['match_rate']:.1f}",
                r["error"] or ""
            ])
    print(f"CSV written to: {csv_path}")

    # Print summary table sorted by match rate descending
    print(f"\n{'='*80}")
    print(f"{'FILENAME':<55} {'TESTED':>7} {'100%':>5} {'RATE':>8}")
    print(f"{'-'*55} {'-'*7} {'-'*5} {'-'*8}")

    # Sort: files with functions first (by match rate desc), then no-function files
    with_funcs = [r for r in results if r["functions_tested"] > 0]
    no_funcs = [r for r in results if r["functions_tested"] == 0]

    with_funcs.sort(key=lambda r: r["match_rate"], reverse=True)

    grand_tested = 0
    grand_100 = 0
    grand_match_sum = 0.0

    for r in with_funcs:
        fn = r["filename"]
        if len(fn) > 54:
            fn = "..." + fn[-51:]
        print(f"  {fn:<54} {r['functions_tested']:>6} {r['functions_100pct']:>5} "
              f"{r['match_rate']:>7.1f}%")
        grand_tested += r["functions_tested"]
        grand_100 += r["functions_100pct"]
        grand_match_sum += r["total_match_sum"]

    if no_funcs:
        print(f"\n  --- Files with no testable functions ---")
        for r in no_funcs:
            fn = r["filename"]
            err = r["error"] or ""
            print(f"  {fn:<54} {err}")

    print(f"\n{'='*80}")
    if grand_tested > 0:
        overall = grand_match_sum / grand_tested
        print(f"TOTALS: {grand_tested} functions tested, "
              f"{grand_100} at 100% ({100.0*grand_100/grand_tested:.1f}%), "
              f"overall avg match: {overall:.1f}%")
    else:
        print("No functions tested.")

    return 0


if __name__ == "__main__":
    sys.exit(main() or 0)
