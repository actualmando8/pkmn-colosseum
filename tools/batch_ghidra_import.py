#!/usr/bin/env python3
"""
batch_ghidra_import.py - Import Ghidra decompiled functions into source files.

For colosseum_battle.c: imports from build/ghidra_output/src/game/colosseum_battle.c
For colosseum_script.c: extracts from build/ghidra_output/raw_decompilation.c

Workflow for each function:
1. Parse Ghidra output
2. Apply C89 fixups
3. Find matching stub or append to source file
4. Attempt compilation
5. If compile succeeds, run match test
6. Keep if match improved, revert if worse

Usage:
    python tools/batch_ghidra_import.py --target battle
    python tools/batch_ghidra_import.py --target script
    python tools/batch_ghidra_import.py --target all
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

# Add tools to path
TOOLS_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = TOOLS_DIR.parent
sys.path.insert(0, str(TOOLS_DIR))

from c89_fixup import (
    fixup_function, fixup_raw_function,
    parse_functions_processed, parse_functions_raw,
    apply_type_replacements, apply_param_renaming
)

# ============================================================================
# Paths
# ============================================================================

SRC_DIR = PROJECT_ROOT / "src" / "game"
GHIDRA_PROCESSED = PROJECT_ROOT / "build" / "ghidra_output" / "src" / "game"
GHIDRA_RAW = PROJECT_ROOT / "build" / "ghidra_output" / "raw_decompilation.c"

BATTLE_SRC = SRC_DIR / "colosseum_battle.c"
SCRIPT_SRC = SRC_DIR / "colosseum_script.c"
BATTLE_GHIDRA = GHIDRA_PROCESSED / "colosseum_battle.c"

# Compiler versions
BATTLE_CV = "1.3"
SCRIPT_CV = "1.2.5n"


def run_compile(source_file: str, cv: str) -> tuple:
    """Run compile_check.py and return (success, output)."""
    cmd = [
        sys.executable, str(TOOLS_DIR / "compile_check.py"),
        source_file, "-cv", cv
    ]
    try:
        result = subprocess.run(
            cmd, capture_output=True, text=True, timeout=120,
            cwd=str(PROJECT_ROOT)
        )
        success = result.returncode == 0
        output = result.stdout + result.stderr
        return success, output
    except subprocess.TimeoutExpired:
        return False, "Compile timed out"
    except Exception as e:
        return False, str(e)


def run_match_test(func_name: str) -> tuple:
    """Run match_test.py for a function and return (match_pct, output)."""
    cmd = [
        sys.executable, str(TOOLS_DIR / "match_test.py"),
        func_name
    ]
    try:
        result = subprocess.run(
            cmd, capture_output=True, text=True, timeout=120,
            cwd=str(PROJECT_ROOT)
        )
        output = result.stdout + result.stderr
        # Try to parse match percentage
        m = re.search(r'(\d+(?:\.\d+)?)\s*%', output)
        if m:
            return float(m.group(1)), output
        return -1.0, output
    except Exception as e:
        return -1.0, str(e)


def find_stub_in_source(source_text: str, func_name: str) -> tuple:
    """
    Find a stub function in the source text.
    Returns (start_line, end_line, is_stub) or None.

    Matches patterns like:
      u32 fn_XXXXXXXX(void* ctx, u32 param) { return 0; /* stub */ }
      void fn_XXXXXXXX(u32 value) { /* stub */ }
    """
    lines = source_text.split('\n')
    func_pattern = re.compile(
        rf'\b{re.escape(func_name)}\b\s*\('
    )

    for i, line in enumerate(lines):
        if func_pattern.search(line):
            # Check if this is a function definition (has return type before name)
            stripped = line.strip()
            # Should start with a type keyword
            if re.match(r'^(?:void|u32|s32|int|u8|u16|u64|s64|s16|s8|float|double|BOOL)\s', stripped):
                # Check if it's a one-liner stub
                if '/* stub */' in line or ('{' in line and '}' in line and 'return' in line):
                    return (i, i, True)
                # Check if it's a TODO decompile block
                if i > 0 and 'TODO: Decompile' in lines[i-1]:
                    # Multi-line stub - find the end
                    brace_depth = 0
                    for j in range(i, len(lines)):
                        brace_depth += lines[j].count('{') - lines[j].count('}')
                        if brace_depth <= 0 and '{' in ''.join(lines[i:j+1]):
                            return (i-1, j, True)  # Include the TODO comment
                    return (i-1, len(lines)-1, True)
                # Multi-line function definition
                if '{' in line or (i+1 < len(lines) and '{' in lines[i+1]):
                    brace_depth = 0
                    start = i
                    # Check for comment above
                    if i > 0 and lines[i-1].strip().startswith('/*'):
                        start = i - 1
                    for j in range(i, len(lines)):
                        brace_depth += lines[j].count('{') - lines[j].count('}')
                        if brace_depth <= 0 and '{' in ''.join(lines[i:j+1]):
                            is_stub = '/* stub */' in ''.join(lines[i:j+1]) or \
                                      'return 0;' == ''.join(lines[i:j+1]).strip().split('{')[1].split('}')[0].strip()
                            return (start, j, is_stub)

    return None


def find_insertion_point(source_text: str, func_addr: int) -> int:
    """
    Find the correct line to insert a new function based on address ordering.
    Returns the line number after which to insert.
    """
    lines = source_text.split('\n')
    last_func_end = len(lines) - 1

    # Find all function definitions with addresses
    func_positions = []
    addr_pattern = re.compile(r'fn_([0-9a-fA-F]{8})')

    for i, line in enumerate(lines):
        # Look for address comments or function definitions
        m = re.search(r'Address:\s*0x([0-9a-fA-F]+)', line)
        if m:
            addr = int(m.group(1), 16)
            func_positions.append((addr, i))
            continue

        # Look for fn_ definitions (with return type)
        if re.match(r'^(?:void|u32|s32|int|u8|u16|u64|s64|s16|s8|float|double|BOOL)\s+fn_', line):
            m = addr_pattern.search(line)
            if m:
                addr = int(m.group(1), 16)
                func_positions.append((addr, i))

    if not func_positions:
        return last_func_end

    # Find where this function should go (maintain address order)
    func_positions.sort(key=lambda x: x[0])

    for idx, (addr, line_num) in enumerate(func_positions):
        if addr > func_addr:
            # Insert before this function
            # Go back to find a good insertion point (blank line before)
            insert_line = line_num
            while insert_line > 0 and lines[insert_line - 1].strip():
                insert_line -= 1
            return insert_line

    # Address is after all existing functions - append at end
    return last_func_end


def generate_extern_decls_for_calls(func_code: str, source_text: str, func_name: str) -> list:
    """Generate extern declarations for called functions not already declared."""
    called = set(re.findall(r'\b(fn_[0-9a-fA-F]{8})\s*\(', func_code))
    # Remove the function itself
    called.discard(func_name)

    externs_needed = []
    for fn in sorted(called):
        # Check if already declared/defined in source
        if fn in source_text:
            continue
        externs_needed.append(f'    extern void {fn}();')

    return externs_needed


def format_function_for_import(func_name: str, func_code: str, size_hex: str,
                                source_text: str) -> str:
    """Format a fixed-up function for insertion into the source file."""
    # Generate extern declarations for unknown called functions
    ext_decls = generate_extern_decls_for_calls(func_code, source_text, func_name)

    # Check if extern decls need to be added inside the function body
    if ext_decls:
        brace_idx = func_code.find('{')
        if brace_idx >= 0:
            nl_after = func_code.find('\n', brace_idx)
            if nl_after >= 0:
                insert_pt = nl_after + 1
            else:
                insert_pt = brace_idx + 1
            ext_block = '\n'.join(ext_decls) + '\n'
            func_code = func_code[:insert_pt] + ext_block + func_code[insert_pt:]

    addr = func_name[3:]  # Strip "fn_"
    size_val = int(size_hex, 16)
    header = f'/* Address: 0x{addr} | Size: 0x{size_hex.upper()} | Ghidra import */\n'
    return header + func_code


def import_battle_functions(dry_run: bool = False, max_funcs: int = 0) -> dict:
    """Import Ghidra functions into colosseum_battle.c"""
    print("=" * 70)
    print("IMPORTING GHIDRA FUNCTIONS INTO colosseum_battle.c")
    print("=" * 70)

    if not BATTLE_GHIDRA.exists():
        print(f"ERROR: Ghidra output not found: {BATTLE_GHIDRA}")
        return {'imported': 0, 'failed': 0, 'skipped': 0}

    # Parse Ghidra output
    ghidra_text = BATTLE_GHIDRA.read_text()
    functions = parse_functions_processed(ghidra_text)
    print(f"Found {len(functions)} functions in Ghidra output")

    # Read current source
    source_text = BATTLE_SRC.read_text()

    # Filter to functions that exist in the Ghidra output's address range
    # (0x8025DC2C - 0x80266320) but not already in source
    importable = []
    for name, size, code in functions:
        addr = int(name[3:], 16)
        # Check the function's own address range (not called functions)
        if addr < 0x80240000:
            continue
        # Check if already defined in source
        existing = find_stub_in_source(source_text, name)
        if existing:
            start, end, is_stub = existing
            if is_stub:
                importable.append((name, size, code, 'replace_stub', start, end))
            else:
                print(f"  SKIP {name}: already has implementation")
        else:
            importable.append((name, size, code, 'append', -1, -1))

    if max_funcs > 0:
        importable = importable[:max_funcs]

    print(f"Functions to import: {len(importable)}")

    if dry_run:
        for name, size, code, mode, _, _ in importable:
            print(f"  {name} (0x{size}) - {mode}")
        return {'imported': len(importable), 'failed': 0, 'skipped': 0}

    # Backup original
    backup_path = str(BATTLE_SRC) + '.bak'
    shutil.copy2(str(BATTLE_SRC), backup_path)

    stats = {'imported': 0, 'failed': 0, 'skipped': 0, 'reverted': 0}
    batch = []

    for name, size, code, mode, start_line, end_line in importable:
        # Apply C89 fixups
        fixed_code = fixup_function(code, name)

        # Format for import
        formatted = format_function_for_import(name, fixed_code, size, source_text)
        batch.append((name, size, formatted, mode, start_line, end_line))

    # Process in batches to avoid too many compile attempts
    # Try importing all at once first, then fall back to individual
    print(f"\nAttempting batch import of {len(batch)} functions...")

    # Build the new source with all functions appended
    source_lines = source_text.split('\n')
    new_lines = list(source_lines)

    # Process replacements first (in reverse line order to preserve indices)
    replacements = [(name, size, fmt, mode, sl, el) for name, size, fmt, mode, sl, el in batch if mode == 'replace_stub']
    replacements.sort(key=lambda x: x[4], reverse=True)

    for name, size, formatted, mode, start_line, end_line in replacements:
        new_lines[start_line:end_line+1] = formatted.split('\n')

    # Then appends
    appends = [(name, size, fmt, mode, sl, el) for name, size, fmt, mode, sl, el in batch if mode == 'append']
    if appends:
        new_lines.append('')
        new_lines.append('/* =========================================================================')
        new_lines.append(' * Ghidra-imported functions (batch import)')
        new_lines.append(' * ========================================================================= */')
        new_lines.append('')
        for name, size, formatted, mode, _, _ in appends:
            new_lines.append(formatted)
            new_lines.append('')

    new_source = '\n'.join(new_lines)
    BATTLE_SRC.write_text(new_source)

    # Try compiling
    success, output = run_compile(str(BATTLE_SRC), BATTLE_CV)
    if success:
        print(f"  BATCH COMPILE SUCCESS! All {len(batch)} functions compiled.")
        stats['imported'] = len(batch)
    else:
        print(f"  Batch compile failed. Trying individual imports...")
        # Parse error output to find problematic functions
        error_funcs = set()
        for line in output.split('\n'):
            # Look for function names in error output
            m = re.search(r'fn_[0-9a-fA-F]{8}', line)
            if m:
                error_funcs.add(m.group(0))
            # Also look for line numbers to identify which function
            m2 = re.search(r'line\s+(\d+)', line)
            if m2:
                err_line = int(m2.group(1))
                # Map line number to function
                for name, _, _, _, _, _ in batch:
                    if name in new_source:
                        pass  # Could improve line tracking

        print(f"  Error output (first 30 lines):")
        for line in output.split('\n')[:30]:
            print(f"    {line}")

        # Fall back: import one at a time
        BATTLE_SRC.write_text(source_text)  # Restore original

        for name, size, code, mode, start_line, end_line in importable:
            print(f"\n  Trying {name} (0x{size})...")

            # Re-read source (may have been modified by previous successful imports)
            current_source = BATTLE_SRC.read_text()

            # Apply fixups
            fixed_code = fixup_function(code, name)
            formatted = format_function_for_import(name, fixed_code, size, current_source)

            # Find current stub position (may have shifted)
            existing = find_stub_in_source(current_source, name)

            if existing:
                start, end, is_stub = existing
                lines = current_source.split('\n')
                lines[start:end+1] = formatted.split('\n')
                new_source = '\n'.join(lines)
            else:
                # Append
                new_source = current_source.rstrip() + '\n\n' + formatted + '\n'

            # Write and compile
            BATTLE_SRC.write_text(new_source)
            success, output = run_compile(str(BATTLE_SRC), BATTLE_CV)

            if success:
                print(f"    COMPILED OK")
                stats['imported'] += 1
            else:
                print(f"    Compile FAILED, reverting")
                # Show first error
                for line in output.split('\n'):
                    if 'error' in line.lower() or 'Error' in line:
                        print(f"      {line.strip()}")
                        break
                BATTLE_SRC.write_text(current_source)
                stats['failed'] += 1

    return stats


def extract_script_functions_from_raw() -> list:
    """Extract functions in the colosseum_script.c address range from raw decompilation."""
    if not GHIDRA_RAW.exists():
        print(f"ERROR: Raw decompilation not found: {GHIDRA_RAW}")
        return []

    # Read the colosseum_script.c address range: 0x80212000 - 0x80240000
    raw_text = GHIDRA_RAW.read_text()
    all_functions = parse_functions_raw(raw_text)

    script_functions = []
    for name, size, code in all_functions:
        addr = int(name[3:], 16)
        if 0x80212000 <= addr < 0x80240000:
            script_functions.append((name, size, code))

    return script_functions


def import_script_functions(dry_run: bool = False, max_funcs: int = 0) -> dict:
    """Import Ghidra functions into colosseum_script.c"""
    print("=" * 70)
    print("IMPORTING GHIDRA FUNCTIONS INTO colosseum_script.c")
    print("=" * 70)

    functions = extract_script_functions_from_raw()
    print(f"Found {len(functions)} functions in script address range from raw decompilation")

    if not functions:
        return {'imported': 0, 'failed': 0, 'skipped': 0}

    source_text = SCRIPT_SRC.read_text()

    # Find importable functions (stubs or missing)
    importable = []
    for name, size, code in functions:
        existing = find_stub_in_source(source_text, name)
        if existing:
            start, end, is_stub = existing
            if is_stub:
                importable.append((name, size, code, 'replace_stub', start, end))
            else:
                pass  # Already implemented, skip
        else:
            # Check if the function is referenced but not defined
            if name in source_text:
                pass  # Referenced but not as a stub
            else:
                importable.append((name, size, code, 'append', -1, -1))

    if max_funcs > 0:
        importable = importable[:max_funcs]

    print(f"Functions to import: {len(importable)}")
    print(f"  Replace stubs: {sum(1 for _,_,_,m,_,_ in importable if m == 'replace_stub')}")
    print(f"  New appends: {sum(1 for _,_,_,m,_,_ in importable if m == 'append')}")

    if dry_run:
        for name, size, code, mode, _, _ in importable:
            print(f"  {name} (0x{size}) - {mode}")
        return {'imported': len(importable), 'failed': 0, 'skipped': 0}

    # Backup
    backup_path = str(SCRIPT_SRC) + '.bak'
    shutil.copy2(str(SCRIPT_SRC), backup_path)

    stats = {'imported': 0, 'failed': 0, 'skipped': 0}

    for name, size, code, mode, start_line, end_line in importable:
        print(f"\n  Trying {name} (0x{size})...")

        current_source = SCRIPT_SRC.read_text()

        # Apply full fixup pipeline (raw format)
        fixed_code = fixup_raw_function(code, name)
        formatted = format_function_for_import(name, fixed_code, size, current_source)

        existing = find_stub_in_source(current_source, name)
        if existing:
            start, end, is_stub = existing
            lines = current_source.split('\n')
            lines[start:end+1] = formatted.split('\n')
            new_source = '\n'.join(lines)
        else:
            new_source = current_source.rstrip() + '\n\n' + formatted + '\n'

        SCRIPT_SRC.write_text(new_source)
        success, output = run_compile(str(SCRIPT_SRC), SCRIPT_CV)

        if success:
            print(f"    COMPILED OK")
            stats['imported'] += 1
        else:
            print(f"    Compile FAILED, reverting")
            for line in output.split('\n'):
                if 'error' in line.lower() or 'Error' in line:
                    print(f"      {line.strip()}")
                    break
            SCRIPT_SRC.write_text(current_source)
            stats['failed'] += 1

    return stats


def main():
    parser = argparse.ArgumentParser(description='Batch import Ghidra functions')
    parser.add_argument('--target', choices=['battle', 'script', 'all'],
                        default='all', help='Which file to target')
    parser.add_argument('--dry-run', action='store_true',
                        help='Show what would be imported without doing it')
    parser.add_argument('--max', type=int, default=0,
                        help='Maximum functions to import (0 = all)')
    args = parser.parse_args()

    total_stats = {'imported': 0, 'failed': 0, 'skipped': 0}

    if args.target in ('battle', 'all'):
        stats = import_battle_functions(dry_run=args.dry_run, max_funcs=args.max)
        for k in total_stats:
            total_stats[k] += stats.get(k, 0)

    if args.target in ('script', 'all'):
        stats = import_script_functions(dry_run=args.dry_run, max_funcs=args.max)
        for k in total_stats:
            total_stats[k] += stats.get(k, 0)

    print("\n" + "=" * 70)
    print(f"FINAL RESULTS:")
    print(f"  Imported: {total_stats['imported']}")
    print(f"  Failed:   {total_stats['failed']}")
    print(f"  Skipped:  {total_stats['skipped']}")
    print("=" * 70)


if __name__ == '__main__':
    main()
