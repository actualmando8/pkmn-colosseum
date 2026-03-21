#!/usr/bin/env python3
"""
idiomatize_functions.py - Convert register-level C to idiomatic C89 (Phase 2).

Transforms register variables (r0-r31) to meaningful names, converts
function call patterns to proper C, and simplifies control flow.

Operates on the already-cleaned output of convert_colosseum_script.py.

Target: CW GC/1.2.5n with -O4,p (functions keep #pragma optimization_level 0).
"""

import re
import sys
import os


def process_file(filepath):
    """Process all functions in the file."""
    with open(filepath, 'r') as f:
        content = f.read()

    lines = content.split('\n')

    # Find all function bodies (between #pragma optimization_level 0 + sig ... })
    functions = find_functions(lines)
    print(f"Found {len(functions)} functions to process")

    # Process each function
    converted = 0
    for func in functions:
        result = idiomatize_function(lines, func)
        if result is not None:
            # Replace lines in-place
            start, end = func['body_start'], func['body_end']
            old_len = end - start + 1
            new_content = result
            lines[start:end + 1] = new_content
            # Adjust subsequent function positions
            delta = len(new_content) - old_len
            for other in functions:
                if other['body_start'] > end:
                    other['body_start'] += delta
                    other['body_end'] += delta
                    other['sig_line'] += delta
            converted += 1

    with open(filepath, 'w') as f:
        f.write('\n'.join(lines))

    print(f"Idiomatized {converted} functions")
    return converted


def find_functions(lines):
    """Find all #pragma optimization_level 0 function blocks."""
    functions = []
    i = 0
    while i < len(lines):
        if lines[i].strip() == '#pragma optimization_level 0':
            # Next line should be function signature
            for j in range(i + 1, min(i + 3, len(lines))):
                m = re.match(
                    r'^((?:static\s+)?(?:void|u32|s32|u16|u8|int|f32|f64)\s*\**\s*'
                    r'(fn_[0-9A-Fa-f]+)\s*\(([^)]*)\))\s*\{',
                    lines[j].strip()
                )
                if m:
                    sig = m.group(1)
                    name = m.group(2)
                    params_str = m.group(3)

                    # Find closing brace
                    depth = 1
                    for k in range(j + 1, len(lines)):
                        s = lines[k].strip()
                        depth += s.count('{') - s.count('}')
                        if depth == 0:
                            functions.append({
                                'pragma_line': i,
                                'sig_line': j,
                                'body_start': j,
                                'body_end': k,
                                'name': name,
                                'sig': sig,
                                'params': parse_params(params_str),
                            })
                            i = k + 1
                            break
                    else:
                        i = j + 1
                    break
            else:
                i += 1
        else:
            i += 1
    return functions


def parse_params(params_str):
    """Parse function parameter string into list of (type, name) tuples."""
    if not params_str or params_str.strip() == 'void':
        return []
    params = []
    for p in params_str.split(','):
        p = p.strip()
        if not p:
            continue
        parts = p.split()
        if len(parts) >= 2:
            ptype = ' '.join(parts[:-1])
            pname = parts[-1].lstrip('*')
            params.append((ptype, pname))
        else:
            params.append((p, 'arg'))
    return params


def idiomatize_function(lines, func):
    """Convert a single function body to idiomatic C.

    Returns list of replacement lines, or None if no changes needed.
    """
    start = func['body_start']
    end = func['body_end']
    body = lines[start:end + 1]

    # Parse declarations and code
    sig_line = body[0]
    externs = []
    decls = []
    code_lines = []
    code_start_idx = 1

    for i in range(1, len(body)):
        s = body[i].strip()
        if not s or s == '{':
            continue
        if s.startswith('extern '):
            externs.append(body[i])
            continue
        # Register declarations
        if re.match(r'^(u32|s32|u16|u8|f32|f64)\s+(r\d+|f\d+)\s*=\s*', s):
            decls.append(body[i])
            continue
        if re.match(r'^void\s+\(\*ctr_fn\)', s):
            decls.append(body[i])
            continue
        if re.match(r'^u32\s+ctr\s*=', s):
            decls.append(body[i])
            continue
        if re.match(r'^u8\s+sp\[', s):
            decls.append(body[i])
            continue
        if re.match(r'^u32\s+r1\s*=', s):
            decls.append(body[i])
            continue
        # Empty line after declarations
        if s == '' and not code_lines:
            continue
        code_lines.append(body[i])

    if not code_lines:
        return None

    # Build rename map based on parameter-to-register bindings
    rename_map = build_rename_map(func['params'], code_lines)

    if not rename_map:
        return None  # No renames to do

    # Apply renames to code lines and declarations
    new_code = []
    for line in code_lines:
        new_code.append(apply_renames(line, rename_map))

    new_decls = []
    for line in decls:
        new_decls.append(apply_renames(line, rename_map))

    # Rebuild function
    result = [sig_line]
    result.extend(externs)
    result.extend(new_decls)
    if externs or new_decls:
        result.append('')
    result.extend(new_code)

    return result


def build_rename_map(params, code_lines):
    """Build register -> meaningful name mapping.

    Analyzes the first few lines of code to find register-parameter bindings.
    PPC calling convention: r3, r4, r5, r6, r7, r8, r9, r10 for params.
    """
    param_regs = ['r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'r10']
    rename = {}

    # Look for early saves: rXX = r3; (save param to callee-saved reg)
    code_text = '\n'.join(l.strip() for l in code_lines[:30])

    for i, (ptype, pname) in enumerate(params):
        if i >= len(param_regs):
            break
        preg = param_regs[i]

        # Check for: rXX = r3; pattern (save to callee-saved)
        for line in code_lines[:30]:
            s = line.strip()
            m = re.match(r'^(r\d+)\s*=\s*' + re.escape(preg) + r'\s*;$', s)
            if m:
                save_reg = m.group(1)
                reg_num = int(save_reg[1:])
                # Only rename callee-saved registers (r14-r31)
                if 14 <= reg_num <= 31:
                    rename[save_reg] = pname
                break

    return rename


def apply_renames(line, rename_map):
    """Apply register renames to a line."""
    result = line
    # Sort by length descending to avoid partial matches (r31 before r3)
    for old in sorted(rename_map.keys(), key=lambda x: -len(x)):
        new = rename_map[old]
        if old != new:
            result = re.sub(r'\b' + re.escape(old) + r'\b', new, result)
    return result


def main():
    if len(sys.argv) < 2:
        print("Usage: python tools/idiomatize_functions.py <source_file>")
        sys.exit(1)

    filepath = sys.argv[1]
    if not os.path.exists(filepath):
        print(f"ERROR: File not found: {filepath}")
        sys.exit(1)

    count = process_file(filepath)
    print(f"\nDone. Idiomatized {count} functions.")


if __name__ == '__main__':
    main()
