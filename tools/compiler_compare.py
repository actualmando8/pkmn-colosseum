#!/usr/bin/env python3
"""
compiler_compare.py - Compare CW GC/1.2.5n vs GC/1.3 for each source file.

For each file, compiles with both compiler versions, then runs objdiff-cli
on a sample of functions to determine which compiler produces better matches.
"""

import json
import os
import re
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
TOOLS_DIR = PROJECT_ROOT / "tools"
MWCC_BASE = TOOLS_DIR / "mwcc_compiler" / "GC"
OBJDIFF_CLI = TOOLS_DIR / "objdiff-cli.exe"
INCLUDE_DIR = PROJECT_ROOT / "include"
SRC_DIR = PROJECT_ROOT / "src"
BUILD_DIR = PROJECT_ROOT / "build" / "GC6E01"
OBJ_DIR = BUILD_DIR / "obj"
CONFIG_DIR = PROJECT_ROOT / "config" / "GC6E01"
SYMBOLS_TXT = CONFIG_DIR / "symbols.txt"

# Standard flags (same for both compilers)
CFLAGS = "-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off"

# Files to test
FILES = [
    "src/game/colosseum_script.c",
    "src/game/colosseum_event.c",
    "src/game/pokemon.c",
    "src/game/trainer.c",
    "src/game/gs_render.c",
    "src/game/gs_field_world.c",
    "src/game/gs_model.c",
    "src/game/gs_thread.c",
    "src/game/battle/battle_waza.c",
    "src/game/battle/battle_scene.c",
    "src/game/battle/battle_logic.c",
    "src/game/ui/ui_core.c",
    "src/game/sound/sound.c",
]

# Number of functions to sample per file
SAMPLE_SIZE = 8


def parse_symbols():
    """Parse symbols.txt into a dict of name -> (address, size)."""
    sym_re = re.compile(
        r'^(\S+)\s*=\s*(\.\w+):0x([0-9A-Fa-f]+)\s*;'
        r'(?:\s*//\s*(.*))?$'
    )
    symbols = {}
    with open(SYMBOLS_TXT, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("//") or line.startswith("#"):
                continue
            m = sym_re.match(line)
            if not m:
                continue
            name = m.group(1)
            section = m.group(2)
            address = int(m.group(3), 16)
            comment = m.group(4) or ""

            sym_type = None
            size = None
            for part in comment.split():
                if part.startswith("type:"):
                    sym_type = part.split(":", 1)[1]
                elif part.startswith("size:"):
                    try:
                        size = int(part.split(":", 1)[1], 0)
                    except ValueError:
                        pass

            if sym_type == "function" and size and size > 0:
                symbols[name] = (address, size)

    return symbols


def get_file_functions(src_path, all_symbols):
    """Get function symbols that belong to a source file."""
    with open(src_path, "r", errors="replace") as f:
        content = f.read()

    # Extract fn_XXXXXXXX addresses mentioned in the file
    fn_pattern = re.compile(r'\b(fn_([0-9A-Fa-f]{8}))\b')
    file_addrs = {}
    for m in fn_pattern.finditer(content):
        name = m.group(1)
        addr = int(m.group(2), 16)
        file_addrs[addr] = name

    # Also look for named function definitions
    func_def_pattern = re.compile(
        r'^(?:\w[\w\s\*]+)\s+(\w+)\s*\([^)]*\)\s*\{', re.MULTILINE
    )
    defined_names = set()
    for m in func_def_pattern.finditer(content):
        name = m.group(1)
        if name not in {'if', 'while', 'for', 'switch', 'return', 'else'}:
            defined_names.add(name)

    # Match to symbols
    functions = []
    seen_addrs = set()
    for sym_name, (addr, size) in all_symbols.items():
        if addr in file_addrs and addr not in seen_addrs:
            functions.append((sym_name, addr, size))
            seen_addrs.add(addr)
        elif sym_name in defined_names and addr not in seen_addrs:
            functions.append((sym_name, addr, size))
            seen_addrs.add(addr)

    # Sort by address
    functions.sort(key=lambda x: x[1])
    return functions


def compile_file(src_path, compiler_version, output_path):
    """Compile a file with specified compiler version. Returns True on success."""
    compiler = MWCC_BASE / compiler_version / "mwcceppc.exe"
    if not compiler.exists():
        print(f"  ERROR: Compiler not found: {compiler}")
        return False

    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    cmd = [
        str(compiler), "-c",
    ] + CFLAGS.split() + [
        "-i", str(INCLUDE_DIR),
        "-o", str(output_path),
        str(src_path),
    ]

    result = subprocess.run(cmd, capture_output=True, text=True,
                            cwd=str(PROJECT_ROOT))

    if result.returncode != 0:
        print(f"  COMPILE FAIL ({compiler_version}): {result.stderr.strip()[:200]}")
        return False

    return True


def diff_function(target_obj, base_obj, symbol_name):
    """Run objdiff-cli for a specific function. Returns match percentage or None."""
    cmd = [
        str(OBJDIFF_CLI), "diff",
        "-1", str(target_obj),
        "-2", str(base_obj),
        "-o", "-",
        "--format", "json",
        symbol_name,
    ]

    result = subprocess.run(cmd, capture_output=True, text=True,
                            cwd=str(PROJECT_ROOT), timeout=30)

    if result.returncode != 0 or not result.stdout.strip():
        return None

    try:
        diff_json = json.loads(result.stdout)
    except json.JSONDecodeError:
        return None

    # Extract match info from the JSON
    left = diff_json.get("left", {})
    left_symbols = left.get("symbols", [])

    for lsym in left_symbols:
        name = lsym.get("name", "")
        if name == symbol_name:
            instructions = lsym.get("instructions", [])
            if instructions:
                total = len(instructions)
                matched = 0
                for instr in instructions:
                    diff_kind = instr.get("diff_kind")
                    if diff_kind is None or diff_kind == "DIFF_NONE":
                        matched += 1
                return (matched / total * 100.0) if total > 0 else 0.0

    # If we didn't find the symbol by name, try to get section-level match
    for section in left.get("sections", []):
        mp = section.get("match_percent")
        if mp is not None:
            return mp

    return None


def find_target_obj(src_path):
    """Find the target (original) object file for comparison."""
    stem = Path(src_path).stem

    # Check for dedicated object file
    for obj_file in OBJ_DIR.glob("*.o"):
        if stem in obj_file.stem and "auto_" not in obj_file.stem:
            return obj_file

    # Default: monolithic text object
    return OBJ_DIR / "auto_01_800055E0_text.o"


def select_sample_functions(functions, sample_size):
    """Select a representative sample of functions.

    Prefer medium-sized functions (not too small, not too huge) for
    meaningful comparison. Sample from across the address range.
    """
    if len(functions) <= sample_size:
        return functions

    # Filter to medium-sized functions (32-2048 bytes, i.e. 8-512 instructions)
    medium = [f for f in functions if 32 <= f[2] <= 2048]
    if len(medium) < sample_size:
        medium = [f for f in functions if 16 <= f[2] <= 4096]
    if len(medium) < sample_size:
        medium = functions

    # Sample evenly across the range
    step = max(1, len(medium) // sample_size)
    sample = medium[::step][:sample_size]

    # If we still don't have enough, pad from the beginning
    if len(sample) < sample_size:
        for f in medium:
            if f not in sample:
                sample.append(f)
            if len(sample) >= sample_size:
                break

    return sample[:sample_size]


def test_file(src_rel, all_symbols):
    """Test a file with both compiler versions and return results."""
    src_path = PROJECT_ROOT / src_rel
    if not src_path.exists():
        print(f"\n{'='*70}")
        print(f"SKIP: {src_rel} (file not found)")
        return None

    print(f"\n{'='*70}")
    print(f"TESTING: {src_rel}")
    print(f"{'='*70}")

    # Get functions from this file
    functions = get_file_functions(src_path, all_symbols)
    if not functions:
        print(f"  No function symbols found in {src_rel}")
        return None

    print(f"  Total functions in file: {len(functions)}")

    # Select sample
    sample = select_sample_functions(functions, SAMPLE_SIZE)
    print(f"  Testing {len(sample)} sample functions")

    # Find target object
    target_obj = find_target_obj(src_path)
    if not target_obj.exists():
        print(f"  ERROR: Target object not found: {target_obj}")
        return None

    print(f"  Target: {target_obj.relative_to(PROJECT_ROOT)}")

    # Compile with 1.2.5n
    out_125n = PROJECT_ROOT / "build" / "tmp" / "test_125n.o"
    out_13 = PROJECT_ROOT / "build" / "tmp" / "test_13.o"

    ok_125n = compile_file(src_path, "1.2.5n", out_125n)
    ok_13 = compile_file(src_path, "1.3", out_13)

    if not ok_125n and not ok_13:
        print(f"  Both compilers failed!")
        return None

    # Test each sample function with both compilers
    results = []
    for sym_name, addr, size in sample:
        result = {
            "name": sym_name,
            "address": f"0x{addr:08X}",
            "size": size,
            "match_125n": None,
            "match_13": None,
        }

        if ok_125n:
            try:
                pct = diff_function(target_obj, out_125n, sym_name)
                result["match_125n"] = pct
            except subprocess.TimeoutExpired:
                pass

        if ok_13:
            try:
                pct = diff_function(target_obj, out_13, sym_name)
                result["match_13"] = pct
            except subprocess.TimeoutExpired:
                pass

        results.append(result)

        # Print inline
        m125 = f"{result['match_125n']:.1f}%" if result['match_125n'] is not None else "N/A"
        m13 = f"{result['match_13']:.1f}%" if result['match_13'] is not None else "N/A"
        better = ""
        if result['match_125n'] is not None and result['match_13'] is not None:
            if result['match_13'] > result['match_125n']:
                better = " << 1.3 BETTER"
            elif result['match_125n'] > result['match_13']:
                better = " << 1.2.5n BETTER"
            else:
                better = " (same)"
        print(f"    {sym_name:40s}  1.2.5n={m125:>8s}  1.3={m13:>8s}{better}")

    # Summarize
    count_125n_better = 0
    count_13_better = 0
    count_same = 0
    sum_125n = 0.0
    sum_13 = 0.0
    compared = 0

    for r in results:
        if r["match_125n"] is not None and r["match_13"] is not None:
            compared += 1
            sum_125n += r["match_125n"]
            sum_13 += r["match_13"]
            if r["match_13"] > r["match_125n"] + 0.5:
                count_13_better += 1
            elif r["match_125n"] > r["match_13"] + 0.5:
                count_125n_better += 1
            else:
                count_same += 1

    avg_125n = sum_125n / compared if compared else 0
    avg_13 = sum_13 / compared if compared else 0

    print(f"\n  SUMMARY for {src_rel}:")
    print(f"    Compared {compared} functions")
    print(f"    1.2.5n better: {count_125n_better}  |  1.3 better: {count_13_better}  |  same: {count_same}")
    print(f"    Avg match 1.2.5n: {avg_125n:.1f}%  |  Avg match 1.3: {avg_13:.1f}%")

    recommendation = "1.2.5n"  # default
    if count_13_better > count_125n_better and avg_13 > avg_125n:
        recommendation = "1.3"
    elif count_13_better > count_125n_better * 2:
        recommendation = "1.3"
    elif avg_13 > avg_125n + 5.0:
        recommendation = "1.3"

    print(f"    RECOMMENDATION: GC/{recommendation}")

    return {
        "file": src_rel,
        "compiled_125n": ok_125n,
        "compiled_13": ok_13,
        "compared": compared,
        "count_125n_better": count_125n_better,
        "count_13_better": count_13_better,
        "count_same": count_same,
        "avg_125n": avg_125n,
        "avg_13": avg_13,
        "recommendation": recommendation,
        "details": results,
    }


def main():
    print("=" * 70)
    print("COMPILER VERSION COMPARISON: CW GC/1.2.5n vs GC/1.3")
    print("Pokemon Colosseum (GPXE01/GC6E01) Decompilation")
    print("=" * 70)

    # Parse all symbols
    print("\nLoading symbols...")
    all_symbols = parse_symbols()
    print(f"  {len(all_symbols)} function symbols loaded")

    # Create temp build dir
    tmp_dir = PROJECT_ROOT / "build" / "tmp"
    os.makedirs(tmp_dir, exist_ok=True)

    # Test each file
    all_results = []
    for src_rel in FILES:
        result = test_file(src_rel, all_symbols)
        if result:
            all_results.append(result)

    # Final summary
    print("\n" + "=" * 70)
    print("FINAL SUMMARY")
    print("=" * 70)
    print(f"\n{'File':<45s} {'Recommendation':<15s} {'1.2.5n Avg':>10s} {'1.3 Avg':>10s} {'Winner':>10s}")
    print("-" * 95)

    files_for_13 = []
    files_for_125n = []

    for r in all_results:
        short_name = r["file"].replace("src/game/", "")
        rec = r["recommendation"]
        winner = "1.3" if r["avg_13"] > r["avg_125n"] else "1.2.5n"
        if abs(r["avg_13"] - r["avg_125n"]) < 0.5:
            winner = "tie"
        print(f"  {short_name:<43s} {rec:<15s} {r['avg_125n']:>9.1f}% {r['avg_13']:>9.1f}% {winner:>10s}")

        if rec == "1.3":
            files_for_13.append(r["file"])
        else:
            files_for_125n.append(r["file"])

    print(f"\nFiles that should use GC/1.3 ({len(files_for_13)}):")
    for f in files_for_13:
        print(f"  {f}")

    print(f"\nFiles that should stay with GC/1.2.5n ({len(files_for_125n)}):")
    for f in files_for_125n:
        print(f"  {f}")

    # Save results to JSON
    results_path = PROJECT_ROOT / "compiler_comparison_results.json"
    with open(results_path, "w") as f:
        json.dump(all_results, f, indent=2)
    print(f"\nDetailed results saved to: {results_path}")

    return files_for_13


if __name__ == "__main__":
    files_for_13 = main()
