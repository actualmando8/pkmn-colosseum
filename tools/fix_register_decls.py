#!/usr/bin/env python3
"""
fix_register_decls.py - Fix missing register variable declarations.

Scans EVERY multi-line function body in colosseum_battle.c for references
to register variables (r0-r31, f0-f31) that are not declared, and adds
declarations. Also handles missing 'sp' and 'r1' references.
"""

import re
import sys
from pathlib import Path

SRC = Path("src/game/colosseum_battle.c")


def find_multiline_functions(lines):
    """Find all multi-line function definitions (opening { at end of line)."""
    functions = []
    i = 0
    while i < len(lines):
        s = lines[i].rstrip()
        # Match: rettype fn_XXXX(...) {
        m = re.match(r'^(\w[\w\s\*]*?)\s+(fn_[0-9A-Fa-f]+)\s*\(([^)]*)\)\s*\{', s)
        if m:
            # Check it's not a forward declaration or single-line function
            if s.rstrip().endswith('{') and not s.rstrip().endswith('};'):
                func_start = i

                # Find matching closing brace
                depth = 1
                j = i + 1
                while j < len(lines) and depth > 0:
                    ln = lines[j]
                    depth += ln.count('{') - ln.count('}')
                    j += 1
                func_end = j - 1

                functions.append({
                    'start': func_start,
                    'end': func_end,
                    'ret_type': m.group(1).strip(),
                    'name': m.group(2),
                    'params': m.group(3).strip(),
                })
                i = func_end + 1
                continue
        i += 1
    return functions


def get_param_regs(params_str):
    """Map register names to param info."""
    mapping = {}
    parts = [p.strip() for p in params_str.split(',')]
    reg_idx = 3
    for p in parts:
        toks = p.rsplit(None, 1)
        if len(toks) == 2:
            ptype = toks[0].strip()
            pname = toks[1].strip().lstrip('*')
            mapping[f'r{reg_idx}'] = (pname, ptype)
            reg_idx += 1
    return mapping


def fix_function(lines, func):
    """Add missing register declarations to a function."""
    start = func['start']
    end = func['end']
    params = func['params']

    body_start = start + 1
    body_end = end

    # Get full body text for scanning
    body_text = '\n'.join(lines[body_start:body_end])

    # Find ALL register references
    all_regs = set()
    for m in re.finditer(r'\br(\d+)\b', body_text):
        all_regs.add(f'r{m.group(1)}')
    all_fregs = set()
    for m in re.finditer(r'\bf(\d+)\b', body_text):
        all_fregs.add(f'f{m.group(1)}')

    # Find already-declared variables
    declared = set()
    has_sp = False
    last_decl_idx = body_start - 1

    for i in range(body_start, body_end):
        s = lines[i].strip()

        # Empty line after declarations = end of decl section
        if s == '' and declared:
            last_decl_idx = i - 1
            break

        # extern declaration
        if s.startswith('extern '):
            last_decl_idx = i
            continue

        # u8 sp[...];
        if re.match(r'u8\s+sp\[', s):
            has_sp = True
            last_decl_idx = i
            continue

        # u32 rN = ...;
        m = re.match(r'u32\s+(r\d+)\s*=', s)
        if m:
            declared.add(m.group(1))
            last_decl_idx = i
            continue

        # f32 fN = ...;
        m = re.match(r'(f32|f64)\s+(f\d+)\s*=', s)
        if m:
            declared.add(m.group(2))
            last_decl_idx = i
            continue

        # Any other type declaration (u16, etc.)
        if re.match(r'(u8|u16|u32|s8|s16|s32|f32|f64|BOOL|void)\s+\w+', s):
            last_decl_idx = i
            continue

        # Hit actual code - stop looking for declarations
        break

    # Determine missing declarations
    param_map = get_param_regs(params)

    missing = []

    # Check for sp usage
    if not has_sp and re.search(r'\bsp\b', body_text):
        # Find max sp offset
        max_off = 0x10
        for m in re.finditer(r'sp\s*[\+\]]\s*(0x[0-9A-Fa-f]+)', body_text):
            off = int(m.group(1), 16)
            if off + 8 > max_off:
                max_off = off + 8
        max_off = ((max_off + 0xF) // 0x10) * 0x10
        missing.append(f'    u8 sp[0x{max_off:X}];')

    # Check for r1 usage (stack pointer - often used as sp arithmetic)
    if 'r1' in all_regs and 'r1' not in declared:
        # r1 is the stack pointer - declare as u32 and set to (u32)sp if sp exists
        if has_sp or re.search(r'\bsp\b', body_text):
            missing.append('    u32 r1 = (u32)sp;')
        else:
            missing.append('    u32 r1 = 0;')

    # Missing integer registers
    for reg in sorted(all_regs, key=lambda r: int(r[1:])):
        if reg in declared or reg == 'r1':
            continue
        rn = int(reg[1:])
        if reg in param_map:
            pname, ptype = param_map[reg]
            if '*' in ptype:
                missing.append(f'    u32 {reg} = (u32){pname};')
            else:
                missing.append(f'    u32 {reg} = {pname};')
        else:
            missing.append(f'    u32 {reg} = 0;')

    # Missing float registers
    for freg in sorted(all_fregs, key=lambda f: int(f[1:])):
        if freg in declared:
            continue
        missing.append(f'    f32 {freg} = 0.0f;')

    if not missing:
        return 0

    # Insert after the last declaration line
    insert_idx = last_decl_idx + 1

    for decl in reversed(missing):
        lines.insert(insert_idx, decl)

    return len(missing)


def main():
    print("Loading source...")
    with open(SRC, 'r') as f:
        lines = [l.rstrip('\n') for l in f.readlines()]

    print("Finding functions...")
    functions = find_multiline_functions(lines)
    print(f"  Found {len(functions)} multi-line functions")

    # Process from bottom to top
    functions.sort(key=lambda f: f['start'], reverse=True)

    total_added = 0
    funcs_fixed = 0

    for func in functions:
        added = fix_function(lines, func)
        if added > 0:
            total_added += added
            funcs_fixed += 1

    print(f"Fixed {funcs_fixed} functions, added {total_added} declarations")

    print(f"Writing {SRC}...")
    with open(SRC, 'w', newline='\n') as f:
        for line in lines:
            f.write(line + '\n')

    print("Done!")


if __name__ == '__main__':
    main()
