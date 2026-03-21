#!/usr/bin/env python3
"""
fix_compile_errors.py - Automatically fix compile errors in decompiled source files.

Handles:
1. Missing extern declarations for lbl_XXXXXXXX globals and fn_XXXXXXXX functions
2. Missing 'sp' array declarations in #pragma optimizewithasm functions
3. Missing 'r0' register variable declarations
4. Type conflicts (redeclared identifiers)
"""

import re
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"


def get_compile_errors(src_file):
    """Run compile_check.py and return the error output."""
    result = subprocess.run(
        [sys.executable, str(PROJECT_ROOT / "tools" / "compile_check.py"), str(src_file)],
        capture_output=True, text=True, cwd=str(PROJECT_ROOT)
    )
    return result.stdout + result.stderr


def extract_undefined_identifiers(error_output):
    """Extract all undefined identifier names from compiler output."""
    pattern = r"undefined identifier '([^']+)'"
    return sorted(set(re.findall(pattern, error_output)))


def extract_undefined_with_lines(error_output):
    """Extract undefined identifiers with their line numbers."""
    # Pattern: #    LINE:      code
    # followed by: undefined identifier 'NAME'
    results = []
    lines = error_output.split('\n')
    current_line = None
    for line in lines:
        m = re.match(r'#\s+(\d+):', line)
        if m:
            current_line = int(m.group(1))
        m = re.match(r"#\s+undefined identifier '([^']+)'", line)
        if m and current_line:
            results.append((current_line, m.group(1)))
    return results


def find_function_for_line(source_lines, target_line):
    """Find the function that contains the given line number (1-indexed)."""
    # Walk backward from target_line to find the function opening
    for i in range(target_line - 1, -1, -1):
        line = source_lines[i]
        # Match function definition patterns
        if re.match(r'(void|u32|s32|f32|u8|u16|s16|s8|int|BOOL)\s+fn_[0-9A-Fa-f]+\s*\(', line):
            return i  # 0-indexed line of function start
        if re.match(r'(void|u32|s32|f32|u8|u16|s16|s8|int|BOOL)\s+\w+\s*\([^)]*\)\s*\{', line):
            return i
    return None


def fix_undefined_in_function(source_lines, func_start_line, identifier):
    """Add an extern declaration for the identifier inside the function body.

    Returns the modified source_lines and True if modified.
    """
    # Find the opening brace of the function
    brace_line = None
    for i in range(func_start_line, min(func_start_line + 5, len(source_lines))):
        if '{' in source_lines[i]:
            brace_line = i
            break

    if brace_line is None:
        return source_lines, False

    # Find the insertion point: after existing extern declarations
    insert_line = brace_line + 1
    for i in range(brace_line + 1, min(brace_line + 100, len(source_lines))):
        line = source_lines[i].strip()
        if line.startswith('extern '):
            insert_line = i + 1
        elif line.startswith('//') or line.startswith('/*') or line == '':
            continue
        else:
            break

    # Determine the type of declaration needed
    if identifier.startswith('fn_'):
        decl = f"    extern void {identifier}();\n"
    elif identifier.startswith('lbl_'):
        decl = f"    extern u8 {identifier};\n"
    elif identifier == 'sp':
        # Need u8 sp[0x100] array -- find where to insert (after externs, before register vars)
        decl = f"    u8 sp[0x100];\n"
    elif identifier in ('r0', 'r2', 'r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'r10',
                        'r11', 'r12', 'r13', 'r14', 'r15', 'r16', 'r17', 'r18', 'r19',
                        'r20', 'r21', 'r22', 'r23', 'r24', 'r25', 'r26', 'r27', 'r28',
                        'r29', 'r30', 'r31'):
        decl = f"    u32 {identifier} = 0;\n"
    else:
        # Generic extern
        decl = f"    extern u32 {identifier};\n"

    # Check if already declared in this function scope
    for i in range(func_start_line, min(func_start_line + 100, len(source_lines))):
        if source_lines[i].strip() == decl.strip():
            return source_lines, False
        # Stop looking at the end of the variable declarations section
        line = source_lines[i].strip()
        if line.startswith('goto ') or line.startswith('if ') or (line.startswith('r') and '=' in line and 'lbl_' in line):
            break

    source_lines.insert(insert_line, decl)
    return source_lines, True


def fix_file_undefined_identifiers(src_file):
    """Fix all undefined identifier errors in a source file."""
    error_output = get_compile_errors(src_file)
    undef_with_lines = extract_undefined_with_lines(error_output)

    if not undef_with_lines:
        return False

    with open(src_file, 'r') as f:
        source_lines = f.readlines()

    # Group identifiers by function
    # For each unique identifier, find which function it belongs to
    # Process in reverse order of line numbers to avoid offset issues

    # Deduplicate: for each identifier, keep the first occurrence
    seen = {}
    for line_no, ident in undef_with_lines:
        if ident not in seen:
            seen[ident] = line_no

    # For file-level identifiers (lbl_, fn_), add them at file scope
    # For sp/r0 etc, add them in the function scope

    modified = False

    # Group by function
    func_idents = {}  # func_start_line -> list of identifiers
    file_level_idents = []

    for ident, line_no in seen.items():
        if ident in ('sp',) or ident.startswith('r') and ident[1:].isdigit():
            # These need to be in function scope
            func_start = find_function_for_line(source_lines, line_no)
            if func_start is not None:
                if func_start not in func_idents:
                    func_idents[func_start] = []
                func_idents[func_start].append(ident)
            else:
                file_level_idents.append(ident)
        else:
            # Check if this identifier is used inside a #pragma optimizewithasm function
            func_start = find_function_for_line(source_lines, line_no)
            if func_start is not None:
                # Check if the function uses #pragma optimizewithasm
                is_pragma_func = False
                for i in range(max(0, func_start - 3), func_start + 1):
                    if 'optimizewithasm' in source_lines[i]:
                        is_pragma_func = True
                        break

                if is_pragma_func:
                    if func_start not in func_idents:
                        func_idents[func_start] = []
                    func_idents[func_start].append(ident)
                else:
                    file_level_idents.append(ident)
            else:
                file_level_idents.append(ident)

    # Process function-level insertions (in reverse order to maintain line numbers)
    for func_start in sorted(func_idents.keys(), reverse=True):
        for ident in func_idents[func_start]:
            source_lines, was_modified = fix_undefined_in_function(source_lines, func_start, ident)
            if was_modified:
                modified = True

    # Process file-level insertions
    if file_level_idents:
        # Find the right place to insert: after existing #include and extern declarations
        insert_pos = 0
        for i, line in enumerate(source_lines):
            stripped = line.strip()
            if stripped.startswith('#include') or stripped.startswith('extern ') or stripped.startswith('/*') or stripped.startswith('*') or stripped.startswith('//') or stripped == '' or stripped.startswith('/**'):
                insert_pos = i + 1
            elif stripped.startswith('#') or stripped.startswith('typedef'):
                insert_pos = i + 1
            else:
                break

        for ident in sorted(file_level_idents):
            if ident.startswith('fn_'):
                decl = f"extern void {ident}();\n"
            elif ident.startswith('lbl_'):
                decl = f"extern u8 {ident};\n"
            else:
                decl = f"extern u32 {ident};\n"

            # Check if already declared
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


if __name__ == "__main__":
    if len(sys.argv) > 1:
        src_file = Path(sys.argv[1])
        if not src_file.is_absolute():
            src_file = PROJECT_ROOT / src_file
        fix_file_undefined_identifiers(src_file)
    else:
        print("Usage: python tools/fix_compile_errors.py <source_file>")
