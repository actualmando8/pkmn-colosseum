#!/usr/bin/env python3
"""
convert_pragma.py - Convert pragma-guarded functions in colosseum_battle.c
to idiomatic C89.

The existing pseudo-C in pragma blocks uses register names as variables and
relies on optimizewithasm to compile directly to the register layout.
We convert by:
1. Removing pragma push/pop/optimization_level/optimizewithasm
2. Keeping all register-named variables as properly typed locals
3. Keeping the sp[] stack buffer when used
4. Removing epilogue/prologue artifacts (stmw/lmw comments, sp restores)
5. Casting void* params to u32 where used as integer
"""

import re
import sys
from pathlib import Path

SRC = Path("src/game/colosseum_battle.c")


def parse_pragma_blocks(src_lines):
    """Find all #pragma push/pop blocks."""
    blocks = []
    i = 0
    while i < len(src_lines):
        if src_lines[i].strip() == '#pragma push':
            start = i
            depth = 1
            j = i + 1
            while j < len(src_lines) and depth > 0:
                if src_lines[j].strip() == '#pragma push':
                    depth += 1
                elif src_lines[j].strip() == '#pragma pop':
                    depth -= 1
                j += 1
            end = j - 1

            # Find function signature
            for k in range(start, min(start + 10, end + 1)):
                m = re.match(r'^(\w[\w\s\*]*?)\s+(fn_[0-9A-Fa-f]+)\s*\(([^)]*)\)\s*\{', src_lines[k])
                if m:
                    blocks.append({
                        'start': start,
                        'end': end,
                        'ret_type': m.group(1).strip(),
                        'name': m.group(2),
                        'params': m.group(3).strip(),
                        'sig_idx': k,
                    })
                    break
            i = end + 1
        else:
            i += 1
    return blocks


def get_param_names(params_str):
    """Extract parameter names and their register mappings."""
    names = {}
    parts = [p.strip() for p in params_str.split(',')]
    reg_idx = 3
    for p in parts:
        toks = p.rsplit(None, 1)
        if len(toks) == 2:
            ptype = toks[0].strip()
            pname = toks[1].strip().lstrip('*')
            names[f'r{reg_idx}'] = (pname, ptype)
            reg_idx += 1
    return names


def transform_block(block, src_lines):
    """Transform a pragma block into idiomatic C89."""
    start = block['start']
    end = block['end']
    name = block['name']
    ret_type = block['ret_type']
    params = block['params']
    sig_idx = block['sig_idx']

    # Get body between { and }
    body_start = sig_idx + 1
    # Find closing brace
    brace_depth = 1
    close_idx = body_start
    while close_idx <= end:
        s = src_lines[close_idx].strip()
        brace_depth += s.count('{') - s.count('}')
        if brace_depth <= 0:
            break
        close_idx += 1

    body_lines = [src_lines[i] for i in range(body_start, close_idx)]

    # Parse: separate extern decls, register decls, sp decl, and code
    extern_decls = []
    used_regs = set()       # Integer registers used in code
    used_fregs = set()      # Float registers
    has_sp = False
    sp_size = '0x10'
    code_lines = []
    in_decls = True

    param_info = get_param_names(params)
    param_reg_names = set(param_info.keys())  # r3, r4, r5, r6

    for line in body_lines:
        s = line.strip()
        if not s:
            if not in_decls:
                code_lines.append('')
            continue

        if s.startswith('extern '):
            extern_decls.append(s)
            continue

        # u8 sp[0xNN];
        m = re.match(r'u8\s+sp\[(0x[0-9A-Fa-f]+|\d+)\]\s*;', s)
        if m and in_decls:
            sp_size = m.group(1)
            has_sp = True
            continue

        # Register declarations
        m = re.match(r'(u32|s32)\s+(r\d+)\s*=\s*(0|0x0|\(u32\)sp)\s*;', s)
        if m and in_decls:
            reg = m.group(2)
            if reg == 'r1':
                continue  # skip r1 (stack pointer)
            used_regs.add(reg)
            continue

        m = re.match(r'(f32|f64)\s+(f\d+)\s*=\s*(0|0\.0f?)\s*;', s)
        if m and in_decls:
            used_fregs.add(m.group(2))
            continue

        in_decls = False
        code_lines.append(s)

    # Also scan code_lines for any registers mentioned
    all_code = ' '.join(code_lines)
    for m in re.finditer(r'\br(\d+)\b', all_code):
        rn = int(m.group(1))
        if rn != 1:  # skip r1
            used_regs.add(f'r{rn}')
    for m in re.finditer(r'\bf(\d+)\b', all_code):
        used_fregs.add(f'f{m.group(1)}')

    # Check if sp is referenced in code
    if 'sp' in all_code and not has_sp:
        has_sp = True

    # Determine which regs need local declarations
    # Parameters (r3-r6) are passed as function args; if code uses r3 as integer
    # we need to create a local shadow. The original code uses r3 = param and
    # treats it as u32 throughout.
    #
    # Strategy: declare ALL used registers as u32 locals, and initialize
    # parameter registers from the function params.
    regs_to_declare = set()
    for reg in used_regs:
        if reg not in param_reg_names:
            regs_to_declare.add(reg)
        # If reg is a param register but used as u32 (and param is void*),
        # we still need a local u32 copy
        elif reg in param_info:
            pname, ptype = param_info[reg]
            if '*' in ptype:
                regs_to_declare.add(reg)

    # Build the function
    out = []
    out.append(f'{ret_type} {name}({params}) {{')

    # Extern declarations
    for ext in extern_decls:
        out.append(f'    {ext}')

    # Stack buffer
    if has_sp:
        out.append(f'    u8 sp[{sp_size}];')

    # Register variable declarations
    sorted_regs = sorted(regs_to_declare, key=lambda r: int(r[1:]))
    for reg in sorted_regs:
        if reg in param_info:
            pname, ptype = param_info[reg]
            out.append(f'    u32 {reg} = (u32){pname};')
        else:
            out.append(f'    u32 {reg} = 0;')

    # Float register declarations
    for freg in sorted(used_fregs, key=lambda f: int(f[1:])):
        out.append(f'    f32 {freg} = 0.0f;')

    if extern_decls or has_sp or regs_to_declare or used_fregs:
        out.append('')

    # Transform code lines
    for line in code_lines:
        t = transform_line(line, param_info, ret_type)
        if t is not None:
            out.append(t)

    out.append('}')
    return out


def transform_line(line, param_info, ret_type):
    """Transform a single code line."""
    s = line.strip()

    if not s:
        return ''

    # Skip asm-artifact comments
    if s.startswith('/*') and s.endswith('*/;'):
        return None
    if s.startswith('/*') and any(x in s for x in ['stmw', 'lmw', 'subi r',
                                                      'clrlslwi', 'xoris',
                                                      'subic', 'crclr',
                                                      'crset']):
        return None

    # r1 = (u32)sp -- already handled
    if re.match(r'r1\s*=\s*\(u32\)sp\s*;', s):
        return None

    # Labels
    m = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
    if m:
        return f'{m.group(1)}:'

    # Return
    if s == 'return;':
        return '    return;'

    # All other lines: keep as-is with indentation
    return f'    {s}'


def main():
    print("Loading source...")
    with open(SRC, 'r') as f:
        src_lines = [l.rstrip('\n') for l in f.readlines()]

    print("Parsing pragma blocks...")
    blocks = parse_pragma_blocks(src_lines)
    print(f"  Found {len(blocks)} blocks")

    # Sort descending by start line for bottom-up replacement
    blocks.sort(key=lambda b: b['start'], reverse=True)

    converted = 0
    failed = 0
    errors = []

    for block in blocks:
        try:
            new_lines = transform_block(block, src_lines)
            # Replace from #pragma push to #pragma pop
            src_lines[block['start']:block['end'] + 1] = new_lines
            converted += 1
        except Exception as e:
            errors.append(f"{block['name']}: {e}")
            failed += 1

    print(f"Converted: {converted}")
    print(f"Failed: {failed}")
    for e in errors[:20]:
        print(f"  ERROR: {e}")

    print(f"Writing {SRC}...")
    with open(SRC, 'w', newline='\n') as f:
        for line in src_lines:
            f.write(line + '\n')

    # Verify no pragma push/pop remain
    remain = sum(1 for l in src_lines if l.strip() == '#pragma push')
    print(f"Remaining #pragma push: {remain}")
    print("Done!")
    return 0 if failed == 0 else 1


if __name__ == '__main__':
    sys.exit(main())
