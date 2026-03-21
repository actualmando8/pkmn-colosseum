#!/usr/bin/env python3
"""
autofix.py - Multi-pass auto-fixer for compile errors in decompiled source.

Iteratively compiles each file, parses errors, applies fixes, and re-compiles
until either zero errors remain or no more fixable errors are found.
"""

import re
import subprocess
import sys
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"
COMPILE_CMD = [sys.executable, str(PROJECT_ROOT / "tools" / "compile_check.py")]


def compile_file(src_file):
    """Return (success, stdout+stderr)."""
    r = subprocess.run(
        COMPILE_CMD + [str(src_file)],
        capture_output=True, text=True, cwd=str(PROJECT_ROOT)
    )
    output = (r.stdout or '') + '\n' + (r.stderr or '')
    return r.returncode == 0, output


def parse_undefined_errors(output):
    """Parse undefined identifier errors. Returns list of (line_no, identifier)."""
    results = []
    lines = output.split('\n')
    current_line = None
    for raw_line in lines:
        m = re.match(r'#\s+(\d+):', raw_line)
        if m:
            current_line = int(m.group(1))
        m = re.match(r"#\s+undefined identifier '([^']+)'", raw_line)
        if m and current_line:
            results.append((current_line, m.group(1)))
    return results


def find_function_range(source_lines, target_line_0idx):
    """Find the start and end of the function containing target_line.

    Returns (func_start_0idx, func_end_0idx) or (None, None).
    func_start is the line with the function signature.
    func_end is the line with the closing brace.
    """
    # Walk backward to find function start
    func_start = None
    for i in range(target_line_0idx, -1, -1):
        line = source_lines[i].strip()
        if re.match(r'(?:static\s+)?(?:void|u32|s32|f32|u8|u16|s16|s8|int|BOOL|char|long|short)\s+\w+\s*\(', line):
            func_start = i
            break

    if func_start is None:
        return None, None

    # Walk forward to find function end (matching brace)
    depth = 0
    func_end = None
    for i in range(func_start, len(source_lines)):
        for ch in source_lines[i]:
            if ch == '{':
                depth += 1
            elif ch == '}':
                depth -= 1
                if depth == 0:
                    func_end = i
                    break
        if func_end is not None:
            break

    return func_start, func_end


def is_declared_in_range(source_lines, start, end, identifier):
    """Check if identifier is declared (extern or local var) in given line range."""
    for i in range(start, min(end + 1, len(source_lines))):
        line = source_lines[i]
        # Check for extern declaration
        if f'extern' in line and identifier in line:
            # Make sure it's actually declaring this identifier (not just mentioning it)
            if re.search(rf'\bextern\b.*\b{re.escape(identifier)}\b', line):
                return True
        # Check for local variable declaration: u8 sp[...], u32 r0 = ...
        if re.match(rf'\s+(?:u8|u16|u32|s8|s16|s32|f32|f64)\s+{re.escape(identifier)}\b', line):
            return True
    return False


def find_extern_insert_point(source_lines, func_start):
    """Find the insertion point for extern declarations inside a function.

    Returns 0-indexed line right after the opening brace or after existing externs.
    """
    brace_line = None
    for i in range(func_start, min(func_start + 5, len(source_lines))):
        if '{' in source_lines[i]:
            brace_line = i
            break
    if brace_line is None:
        return func_start + 1

    insert = brace_line + 1
    for i in range(brace_line + 1, min(brace_line + 300, len(source_lines))):
        stripped = source_lines[i].strip()
        if stripped.startswith('extern '):
            insert = i + 1
        elif stripped == '' or stripped.startswith('//') or stripped.startswith('/*'):
            continue
        else:
            break
    return insert


def find_local_insert_point(source_lines, func_start):
    """Find insertion point for local variable declarations.

    Returns 0-indexed line after all externs and existing local var declarations.
    """
    brace_line = None
    for i in range(func_start, min(func_start + 5, len(source_lines))):
        if '{' in source_lines[i]:
            brace_line = i
            break
    if brace_line is None:
        return func_start + 1

    last_decl = brace_line
    for i in range(brace_line + 1, min(brace_line + 300, len(source_lines))):
        stripped = source_lines[i].strip()
        if stripped.startswith('extern '):
            last_decl = i
        elif stripped == '':
            continue
        elif re.match(r'(?:u8|u16|u32|s8|s16|s32|f32|f64|void)\s', stripped):
            last_decl = i
        elif stripped.startswith('//') or stripped.startswith('/*'):
            continue
        else:
            break
    return last_decl + 1


def fix_file(src_file):
    """Fix undefined identifier errors. Returns True if file was modified."""
    success, output = compile_file(src_file)
    if success:
        return False

    undef_errors = parse_undefined_errors(output)
    if not undef_errors:
        return False

    with open(src_file, 'r') as f:
        source_lines = f.readlines()

    # Group errors by enclosing function
    # (func_start_0idx, func_end_0idx) -> set of identifiers
    func_idents = defaultdict(set)
    unfixed = []

    for line_no, ident in undef_errors:
        func_start, func_end = find_function_range(source_lines, line_no - 1)
        if func_start is not None:
            func_idents[(func_start, func_end)].add(ident)
        else:
            unfixed.append((line_no, ident))

    if not func_idents and not unfixed:
        return False

    modified = False

    # Process functions in reverse order (so line insertions don't affect later functions)
    for (func_start, func_end) in sorted(func_idents.keys(), reverse=True):
        idents = func_idents[(func_start, func_end)]

        extern_insert = find_extern_insert_point(source_lines, func_start)
        local_insert = find_local_insert_point(source_lines, func_start)

        externs_to_add = []
        locals_to_add = []

        for ident in sorted(idents):
            # Check if already declared in this function's range
            if is_declared_in_range(source_lines, func_start, func_end, ident):
                continue

            if ident == 'sp':
                locals_to_add.append(f"    u8 sp[0x100];\n")
            elif ident.startswith('r') and ident[1:].isdigit() and int(ident[1:]) < 32:
                locals_to_add.append(f"    u32 {ident} = 0;\n")
            elif ident.startswith('f') and ident[1:].isdigit() and int(ident[1:]) < 32:
                locals_to_add.append(f"    f32 {ident} = 0.0f;\n")
            elif ident.startswith('fn_'):
                externs_to_add.append(f"    extern void {ident}();\n")
            elif ident.startswith('lbl_'):
                externs_to_add.append(f"    extern u8 {ident};\n")
            else:
                externs_to_add.append(f"    extern u32 {ident};\n")

        # Insert locals (at local_insert, which >= extern_insert)
        # We need to insert locals first since they go at a higher line number
        for decl in reversed(locals_to_add):
            source_lines.insert(local_insert, decl)
            modified = True

        # Insert externs
        for decl in reversed(externs_to_add):
            source_lines.insert(extern_insert, decl)
            modified = True

    # Handle file-level unfixed identifiers
    if unfixed:
        # Find file-level insert point
        insert_pos = 0
        for i, line in enumerate(source_lines):
            stripped = line.strip()
            if (stripped.startswith('#include') or
                stripped.startswith('extern ') or
                stripped.startswith('/*') or stripped.startswith('*') or
                stripped.startswith('//') or stripped == '' or
                stripped.startswith('/**') or stripped.startswith('#')):
                insert_pos = i + 1
            else:
                break

        for line_no, ident in unfixed:
            if ident.startswith('fn_'):
                decl = f"extern void {ident}();\n"
            elif ident.startswith('lbl_'):
                decl = f"extern u8 {ident};\n"
            else:
                decl = f"extern u32 {ident};\n"

            # Check not already at file level
            already = False
            for line in source_lines:
                if ident in line and 'extern' in line:
                    already = True
                    break
            if not already:
                source_lines.insert(insert_pos, decl)
                insert_pos += 1
                modified = True

    if modified:
        with open(src_file, 'w') as f:
            f.writelines(source_lines)

    return modified


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/autofix.py <source_file> [--max-passes N]")
        return

    src_file = Path(sys.argv[1])
    if not src_file.is_absolute():
        src_file = PROJECT_ROOT / src_file

    max_passes = 5
    if '--max-passes' in sys.argv:
        idx = sys.argv.index('--max-passes')
        max_passes = int(sys.argv[idx + 1])

    for pass_num in range(1, max_passes + 1):
        print(f"\n=== Pass {pass_num} for {src_file.name} ===")
        changed = fix_file(src_file)
        if not changed:
            # Check if it compiles now
            success, output = compile_file(src_file)
            if success:
                print(f"  OK: {src_file.name} compiles successfully!")
            else:
                print(f"  Cannot auto-fix remaining errors:")
                # Show first few errors
                for line in output.split('\n'):
                    if 'Error' in line or 'error' in line:
                        print(f"    {line.strip()[:100]}")
            return

        print(f"  Applied fixes in pass {pass_num}")

    # Final check
    success, output = compile_file(src_file)
    if success:
        print(f"  OK: {src_file.name} compiles successfully!")
    else:
        print(f"  Still failing after {max_passes} passes")


if __name__ == "__main__":
    main()
