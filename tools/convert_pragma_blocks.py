#!/usr/bin/env python3
"""
Convert pragma-guarded register-level C to idiomatic C89.
Pokemon Colosseum decomp -- gs_render.c and gs_field_world.c

Full register data-flow analysis + semantic conversion.
"""

import re
import sys
import os


def extract_blocks(filepath):
    """Extract pragma blocks with their surrounding context."""
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    blocks = []
    i = 0
    while i < len(lines):
        if lines[i].strip() == '#pragma push':
            start = i
            depth = 1
            j = i + 1
            while j < len(lines) and depth > 0:
                if lines[j].strip() == '#pragma push':
                    depth += 1
                elif lines[j].strip() == '#pragma pop':
                    depth -= 1
                j += 1
            blocks.append((start, j, lines[start:j]))
            i = j
        else:
            i += 1
    return lines, blocks


def parse_block(block_lines):
    """Parse a pragma block into structured info."""
    text = ''.join(block_lines)
    lines_stripped = [l.rstrip('\n') for l in block_lines]

    # Find function signature
    sig = None
    sig_idx = None
    for i, l in enumerate(lines_stripped):
        s = l.strip()
        if s.startswith('#pragma'):
            continue
        if s.startswith('/*'):
            continue
        if '(' in s and '{' in s:
            for prefix in ['void ', 'u32 ', 'u8 ', 's32 ', 'u16 ', 's16 ',
                          'f32 ', 'f64 ', 'BOOL ', 'int ', 'u8* ',
                          'void* ', 'u32* ', 's32* ', 'u16* ']:
                if s.startswith(prefix):
                    sig = s.rstrip(' {').rstrip('{').rstrip() + ' {'
                    sig_idx = i
                    break
            if sig:
                break

    if sig is None:
        return None

    # Extract function name
    m = re.search(r'(\w+)\s*\(', sig)
    func_name = m.group(1) if m else '???'

    # Extract declared params from signature
    m = re.search(r'\(([^)]*)\)', sig)
    params_str = m.group(1).strip() if m else ''
    if params_str == 'void' or params_str == '':
        declared_params = []
    else:
        declared_params = [p.strip() for p in params_str.split(',')]

    # Extract externs and body
    externs = []
    typedefs = []
    body = []
    in_body = False
    brace_depth = 0

    for i, l in enumerate(lines_stripped):
        s = l.strip()
        if s.startswith('#pragma'):
            continue
        if i == sig_idx:
            in_body = True
            brace_depth = s.count('{') - s.count('}')
            continue
        if not in_body:
            continue
        if s.startswith('extern '):
            externs.append(s)
            continue
        if s.startswith('typedef '):
            typedefs.append(s)
            continue
        body.append(s)

    # Remove trailing } and empty lines
    while body and body[-1] in ('}', '', '#pragma pop'):
        body.pop()

    # Extract register declarations and their initial values
    reg_decls = {}
    clean_body = []
    for l in body:
        m = re.match(r'^(u32) (r\d+) = (.+);$', l)
        if m:
            reg_decls[m.group(2)] = m.group(1)
            continue
        m = re.match(r'^(f32|f64) (f\d+) = (.+);$', l)
        if m:
            reg_decls[m.group(2)] = m.group(1)
            continue
        if re.match(r'^u8 sp\[', l):
            continue
        if re.match(r'^void \(\*ctr_fn\)', l):
            reg_decls['ctr_fn'] = 'void(*)(void)'
            continue
        if re.match(r'^u32 ctr = 0;', l):
            reg_decls['ctr'] = 'u32'
            continue
        clean_body.append(l)

    return {
        'name': func_name,
        'sig': sig,
        'declared_params': declared_params,
        'externs': externs,
        'typedefs': typedefs,
        'reg_decls': reg_decls,
        'body': clean_body,
    }


def infer_input_regs(body_lines):
    """
    Infer which registers are used as input params.
    In PPC calling convention: r3=param1, r4=param2, r5=param3, etc.
    f1=float1, f2=float2, etc.

    We detect input regs by finding registers that are READ before being WRITTEN.
    """
    written = set()
    inputs = set()

    for l in body_lines:
        # Skip noise
        if l.startswith('/*') or l.startswith('L_') or l == 'return;':
            continue
        if re.match(r'^r\d+ = \*\(u32\*\)\(sp \+ 0x', l):
            continue
        if l.startswith('/* stmw') or l.startswith('/* lmw'):
            continue
        if l.startswith('/* psq_'):
            continue
        if re.match(r'^\*\(f64\*\)\(sp', l):
            continue
        if re.match(r'^f\d+ = \*\(f64\*\)\(sp', l):
            continue

        # Find all register references in the line
        # Right side (reads)
        reads = set()
        writes = set()

        # Assignment: LHS = RHS;
        m = re.match(r'^(r\d+|f\d+) = (.+);$', l)
        if m:
            lhs = m.group(1)
            rhs = m.group(2)
            writes.add(lhs)
            # Find register refs in RHS
            for r in re.findall(r'\b(r\d+|f\d+)\b', rhs):
                if r != lhs:
                    reads.add(r)
        else:
            # Other lines (function calls, stores, etc.)
            for r in re.findall(r'\b(r\d+|f\d+)\b', l):
                reads.add(r)

        # Check if any reads are of unwritten registers
        for r in reads:
            if r not in written:
                inputs.add(r)
        written.update(writes)

    return inputs


def infer_param_type(body_lines, reg, declared_params):
    """Infer the type of an input parameter register based on usage."""
    body_text = '\n'.join(body_lines)

    # Check for casts applied to this register
    # r5 = r3 & 0xFF -> u8
    if re.search(r'\b' + reg + r' & 0xFF\b', body_text):
        return 'u8'
    # r5 = r3 & 0xFFFF -> u16
    if re.search(r'\b' + reg + r' & 0xFFFF\b', body_text):
        return 'u16'
    # r5 = (s8)r3 -> s8
    if re.search(r'\(s8\)' + reg + r'\b', body_text):
        return 's8'
    # r5 = (s16)r3 -> s16
    if re.search(r'\(s16\)' + reg + r'\b', body_text):
        return 's16'

    # Check for float usage
    if reg.startswith('f'):
        return 'f32'

    # Check how it's stored
    # *(u8*)(...) = rN -> u8
    if re.search(r'\*\(u8\*\).*= ' + reg + r'\b', body_text):
        return 'u8'
    if re.search(r'\*\(u16\*\).*= ' + reg + r'\b', body_text):
        return 'u16'
    if re.search(r'\*\(f32\*\).*= ' + reg + r'\b', body_text):
        return 'f32'

    return 'u32'


def convert_function(parsed):
    """
    Convert a parsed pragma block to idiomatic C89.
    Returns list of lines, or None if too complex.
    """
    name = parsed['name']
    body = parsed['body']
    externs = parsed['externs']
    typedefs = parsed['typedefs']
    reg_decls = parsed['reg_decls']
    declared_params = parsed['declared_params']

    body_text = '\n'.join(body)

    # Check complexity
    goto_count = len(re.findall(r'\bgoto\b', body_text))
    label_count = len(re.findall(r'^L_[0-9a-fA-F]+\s*:', body_text, re.MULTILINE))
    has_jt = 'jumptable_' in body_text
    has_gslog = 'fn_800D4F98' in body_text
    has_47e = '0x47E' in body_text or '0x47e' in body_text
    has_6b00 = 'fn_800D6B00' in body_text
    has_loop = 'ctr != 0' in body_text or '--ctr' in body_text
    line_count = len(body)

    # --- Pattern: STATE_47E_DISPATCH ---
    if has_gslog and has_47e and goto_count <= 3 and label_count <= 3:
        return convert_state_dispatch(parsed, check_47e=True)

    # --- Pattern: STATE_SIMPLE_DISPATCH ---
    if has_gslog and not has_47e and goto_count <= 3 and label_count <= 3:
        return convert_state_dispatch(parsed, check_47e=False)

    # --- Pattern: STATE_MULTI (more branches, still with gslog) ---
    if has_gslog and goto_count <= 6 and label_count <= 6:
        return convert_state_dispatch(parsed, check_47e=has_47e)

    # --- Everything else: too complex for auto, keep pragmas ---
    return None


def convert_state_dispatch(parsed, check_47e=True):
    """
    Convert a state-check + fn_800D4F98 dispatch function.
    Handles both 0x47E and non-0x47E variants.
    """
    body = parsed['body']
    externs = parsed['externs']
    typedefs = parsed['typedefs']
    reg_decls = parsed['reg_decls']
    name = parsed['name']

    # Clean body: remove prologue/epilogue noise
    clean = []
    for l in body:
        if l.startswith('/* stmw') or l.startswith('/* lmw'):
            continue
        if l.startswith('/* psq_st') or l.startswith('/* psq_l'):
            continue
        if re.match(r'^\*\(f64\*\)\(sp \+ 0x[0-9a-fA-F]+\) = f\d+;$', l):
            continue
        if re.match(r'^f\d+ = \*\(f64\*\)\(sp \+ 0x[0-9a-fA-F]+\);$', l):
            continue
        if re.match(r'^r\d+ = \*\(u32\*\)\(sp \+ 0x[0-9a-fA-F]+\);$', l):
            continue
        if l in ('/* crclr cr1eq */;', '/* crset cr1eq */;'):
            continue
        if re.match(r'^/\* (lwzx|stbx|subi|clrlslwi|lbzx|rlwinm) .* \*/;?$', l):
            continue
        if l == '/* indirect jump via ctr */;':
            continue
        clean.append(l)

    # Infer actual input registers
    inputs = infer_input_regs(clean)

    # Only consider r3..r10, f1..f8 as potential params
    param_regs = sorted([r for r in inputs if re.match(r'^r[3-9]$|^r10$|^f[1-8]$', r)],
                       key=lambda r: int(r[1:]) if r[0] == 'r' else 100 + int(r[1:]))

    # Find param saves (r31=r4, r30=r3, etc.)
    param_saves = {}  # saved_reg -> input_reg
    for l in clean[:20]:
        m = re.match(r'^(r\d+) = (r[3-9]|r10);$', l)
        if m:
            dst = m.group(1)
            src = m.group(2)
            if int(dst[1:]) >= 20:  # saved regs are r20-r31
                param_saves[dst] = src

    # Infer param types
    param_types = {}
    for reg in param_regs:
        param_types[reg] = infer_param_type(clean, reg, parsed['declared_params'])

    # Create param name mapping
    param_names = {}
    param_idx = 0
    int_param_idx = 0
    float_param_idx = 0
    for reg in param_regs:
        if reg.startswith('f'):
            float_param_idx += 1
            param_names[reg] = f'fp{float_param_idx}'
        else:
            int_param_idx += 1
            ptype = param_types[reg]
            if ptype in ('u8', 's8'):
                param_names[reg] = f'param{int_param_idx}'
            elif ptype in ('u16', 's16'):
                param_names[reg] = f'param{int_param_idx}'
            else:
                param_names[reg] = f'param{int_param_idx}'

    # Map saved regs to param names
    for saved, inp in param_saves.items():
        if inp in param_names:
            param_names[saved] = param_names[inp]

    # --- Find fn_800D4F98 call and extract CMD, ARGC, extra args ---
    gslog_idx = None
    for i, l in enumerate(clean):
        if 'fn_800D4F98()' in l or 'fn_800D4F98(' in l:
            gslog_idx = i
            break

    if gslog_idx is None:
        return None

    # Find CMD and ARGC
    cmd = argc = None
    for i in range(gslog_idx - 1, max(gslog_idx - 10, -1), -1):
        l = clean[i]
        if cmd is None:
            m = re.match(r'^r3 = (0x[0-9a-fA-F]+);$', l)
            if m: cmd = m.group(1)
        if argc is None:
            m = re.match(r'^r4 = (0x[0-9a-fA-F]+);$', l)
            if m: argc = m.group(1)

    if cmd is None or argc is None:
        return None

    # Extract extra args using full register simulation up to gslog call
    pre_call_state = {}
    for reg, pname in param_names.items():
        pre_call_state[reg] = pname
    for l in clean[:gslog_idx]:
        s = l.strip()
        if s.startswith('if (') or s.startswith('goto ') or s.startswith('L_'):
            continue
        if s in ('/* crclr cr1eq */;', '/* crset cr1eq */;'):
            continue
        m = re.match(r'^(r\d+) = \*\(u32\*\)lbl_8047AA80;$', s)
        if m: pre_call_state[m.group(1)] = 'state'; continue
        m = re.match(r'^(r\d+) = (r\d+);$', s)
        if m: pre_call_state[m.group(1)] = pre_call_state.get(m.group(2), m.group(2)); continue
        m = re.match(r'^(r\d+) = (0x[0-9a-fA-F]+|-?\d+);$', s)
        if m: pre_call_state[m.group(1)] = m.group(2); continue
        m = re.match(r'^(r\d+) = (r\d+) & 0xFF;$', s)
        if m: pre_call_state[m.group(1)] = f'(u8){pre_call_state.get(m.group(2), m.group(2))}'; continue
        m = re.match(r'^(r\d+) = (r\d+) & 0xFFFF;$', s)
        if m: pre_call_state[m.group(1)] = f'(u16){pre_call_state.get(m.group(2), m.group(2))}'; continue
        m = re.match(r'^(r\d+) = \(s8\)(r\d+);$', s)
        if m: pre_call_state[m.group(1)] = f'(s8){pre_call_state.get(m.group(2), m.group(2))}'; continue
        m = re.match(r'^(r\d+) = \(s16\)(r\d+);$', s)
        if m: pre_call_state[m.group(1)] = f'(s16){pre_call_state.get(m.group(2), m.group(2))}'; continue
        m = re.match(r'^(r\d+) = (r\d+) << (\d+);$', s)
        if m: pre_call_state[m.group(1)] = f'({pre_call_state.get(m.group(2), m.group(2))} << {m.group(3)})'; continue
        m = re.match(r'^(r\d+) = (r\d+) \* (0x[0-9a-fA-F]+|\d+);$', s)
        if m: pre_call_state[m.group(1)] = f'({pre_call_state.get(m.group(2), m.group(2))} * {m.group(3)})'; continue
        m = re.match(r'^(r\d+) = (r\d+) \+ (r\d+);$', s)
        if m: pre_call_state[m.group(1)] = f'({pre_call_state.get(m.group(2), m.group(2))} + {pre_call_state.get(m.group(3), m.group(3))})'; continue
        m = re.match(r'^(r\d+) = (r\d+) \+ (0x[0-9a-fA-F]+|\d+);$', s)
        if m: pre_call_state[m.group(1)] = f'({pre_call_state.get(m.group(2), m.group(2))} + {m.group(3)})'; continue
        m = re.match(r'^(r\d+) = \*\((u32|u16|u8|s32)\*\)\(\(u8\*\)(r\d+) \+ (0x[0-9a-fA-F]+)\);$', s)
        if m: pre_call_state[m.group(1)] = f'*({m.group(2)}*)((u8*){pre_call_state.get(m.group(3), m.group(3))} + {m.group(4)})'; continue

    # Extra args from r5..r10
    extra_args = []
    for reg in ['r5', 'r6', 'r7', 'r8', 'r9', 'r10']:
        val = pre_call_state.get(reg, None)
        if val is not None and val != reg and '<clobbered>' not in str(val):
            extra_args.append(str(val))
        else:
            break

    args_str = ', '.join([cmd, argc] + extra_args)

    # --- Find labels and extract else branch ---
    labels = []
    for i, l in enumerate(clean):
        m = re.match(r'^(L_[0-9a-fA-F]+)\s*:?\s*;?$', l)
        if m:
            labels.append((i, m.group(1)))

    if not labels:
        return None

    else_start = labels[0][0] + 1
    if len(labels) > 1:
        else_end = labels[-1][0]
    else:
        else_end = len(clean)

    else_body = []
    for i in range(else_start, else_end):
        l = clean[i]
        if l == 'return;':
            break
        if l.startswith('goto '):
            continue
        if l.startswith('L_') and ':' in l:
            continue
        else_body.append(l)

    # --- Simulate register state from function start to else entry ---
    # Build initial register state from param mapping
    entry_state = {}
    for reg, pname in param_names.items():
        entry_state[reg] = pname

    # Simulate lines from start up to else label to get register state at else entry
    for i in range(else_start):
        l = clean[i]
        if l.startswith('if (') or l.startswith('goto ') or l.startswith('L_'):
            continue
        if l in ('/* crclr cr1eq */;', '/* crset cr1eq */;'):
            continue

        # Track assignments using the same logic as simulate_else_branch
        m = re.match(r'^(r\d+) = \*\(u32\*\)lbl_8047AA80;$', l)
        if m:
            entry_state[m.group(1)] = 'state'
            continue
        m = re.match(r'^(r\d+) = \(u32\)(fn_[0-9a-fA-F]+);$', l)
        if m:
            entry_state[m.group(1)] = f'(u32){m.group(2)}'
            continue
        m = re.match(r'^(r\d+) = \(u32\)(&?lbl_[0-9a-fA-F]+(?:\[\])?);$', l)
        if m:
            entry_state[m.group(1)] = f'(u32){m.group(2)}'
            continue
        m = re.match(r'^(r\d+) = (0x[0-9a-fA-F]+|-?\d+);$', l)
        if m:
            entry_state[m.group(1)] = m.group(2)
            continue
        m = re.match(r'^(r\d+) = (r\d+);$', l)
        if m:
            entry_state[m.group(1)] = entry_state.get(m.group(2), m.group(2))
            continue
        m = re.match(r'^(r\d+) = (r\d+) << (\d+);$', l)
        if m:
            src = entry_state.get(m.group(2), m.group(2))
            entry_state[m.group(1)] = f'({src} << {m.group(3)})'
            continue
        m = re.match(r'^(r\d+) = (r\d+) \* (0x[0-9a-fA-F]+|\d+);$', l)
        if m:
            src = entry_state.get(m.group(2), m.group(2))
            entry_state[m.group(1)] = f'({src} * {m.group(3)})'
            continue
        m = re.match(r'^(r\d+) = (r\d+) \+ (r\d+);$', l)
        if m:
            lhs = entry_state.get(m.group(2), m.group(2))
            rhs = entry_state.get(m.group(3), m.group(3))
            entry_state[m.group(1)] = f'({lhs} + {rhs})'
            continue
        m = re.match(r'^(r\d+) = (r\d+) \+ (0x[0-9a-fA-F]+|\d+);$', l)
        if m:
            src = entry_state.get(m.group(2), m.group(2))
            entry_state[m.group(1)] = f'({src} + {m.group(3)})'
            continue
        m = re.match(r'^(r\d+) = (r\d+) & (0x[0-9a-fA-F]+|\d+);$', l)
        if m:
            src = entry_state.get(m.group(2), m.group(2))
            mask = m.group(3)
            if mask == '0xFF':
                entry_state[m.group(1)] = f'(u8){src}'
            elif mask == '0xFFFF':
                entry_state[m.group(1)] = f'(u16){src}'
            else:
                entry_state[m.group(1)] = f'({src} & {mask})'
            continue
        m = re.match(r'^(r\d+) = \(s8\)(r\d+);$', l)
        if m:
            src = entry_state.get(m.group(2), m.group(2))
            entry_state[m.group(1)] = f'(s8){src}'
            continue
        m = re.match(r'^(r\d+) = \(s16\)(r\d+);$', l)
        if m:
            src = entry_state.get(m.group(2), m.group(2))
            entry_state[m.group(1)] = f'(s16){src}'
            continue
        m = re.match(r'^(r\d+) = \*\((u32|u16|u8|s32)\*\)\(\(u8\*\)(r\d+) \+ (0x[0-9a-fA-F]+)\);$', l)
        if m:
            base = entry_state.get(m.group(3), m.group(3))
            entry_state[m.group(1)] = f'*({m.group(2)}*)((u8*){base} + {m.group(4)})'
            continue

    # --- Convert else branch with full register state context ---
    converted_else = simulate_else_branch_with_state(else_body, entry_state)

    if converted_else is None:
        return None

    # --- Build output function ---
    # Reconstruct function signature with inferred params
    ret_type = 'void'
    m = re.match(r'(\w+(?:\s*\*)?)\s+' + re.escape(name) + r'\s*\(', parsed['sig'])
    if m:
        ret_type = m.group(1)

    if param_regs:
        params_list = []
        for reg in param_regs:
            ptype = param_types[reg]
            pname = param_names[reg]
            params_list.append(f'{ptype} {pname}')
        new_sig = f'{ret_type} {name}({", ".join(params_list)})'
    else:
        new_sig = f'{ret_type} {name}(void)'

    out = []
    out.append(new_sig + ' {')

    for ext in externs:
        out.append('    ' + ext)
    for td in typedefs:
        out.append('    ' + td)

    out.append('    u8* state = (u8*)lbl_8047AA80;')

    if check_47e:
        out.append('    if (state[0x47E] == 0 && *(s32*)state == 1) {')
    else:
        out.append('    if (*(s32*)state == 1) {')

    out.append(f'        fn_800D4F98({args_str});')

    if converted_else:
        out.append('    } else {')
        for l in converted_else:
            out.append('        ' + l)
        out.append('    }')
    else:
        out.append('    }')

    out.append('}')

    return out


def simplify_expr(expr, state_name='state'):
    """Simplify a symbolic expression."""
    if expr is None:
        return None
    expr = str(expr)
    if '<clobbered>' in expr:
        return None
    expr = expr.replace('(u8*)lbl_8047AA80', state_name)
    return expr


def convert_line_with_state(line, reg_state):
    """Convert a single register-level line to C using current register state."""
    m = re.match(r'^(r\d+) = \*\(u32\*\)lbl_8047AA80;$', line)
    if m:
        reg_state[m.group(1)] = 'state'
        return []
    m = re.match(r'^(r\d+) = \(u32\)(fn_[0-9a-fA-F]+);$', line)
    if m:
        reg_state[m.group(1)] = f'(u32){m.group(2)}'
        return []
    m = re.match(r'^(r\d+) = \(u32\)(&?lbl_[0-9a-fA-F]+(?:\[\])?);$', line)
    if m:
        reg_state[m.group(1)] = f'(u32){m.group(2)}'
        return []
    m = re.match(r'^(r\d+) = (0x[0-9a-fA-F]+|-?\d+);$', line)
    if m:
        reg_state[m.group(1)] = m.group(2)
        return []
    m = re.match(r'^(r\d+) = (r\d+);$', line)
    if m:
        reg_state[m.group(1)] = reg_state.get(m.group(2), m.group(2))
        return []
    m = re.match(r'^(r\d+) = (r\d+) << (\d+);$', line)
    if m:
        src = reg_state.get(m.group(2), m.group(2))
        reg_state[m.group(1)] = f'({src} << {m.group(3)})'
        return []
    m = re.match(r'^(r\d+) = (r\d+) \* (0x[0-9a-fA-F]+|\d+);$', line)
    if m:
        src = reg_state.get(m.group(2), m.group(2))
        reg_state[m.group(1)] = f'({src} * {m.group(3)})'
        return []
    m = re.match(r'^(r\d+) = (r\d+) \+ (r\d+);$', line)
    if m:
        lhs = reg_state.get(m.group(2), m.group(2))
        rhs = reg_state.get(m.group(3), m.group(3))
        reg_state[m.group(1)] = f'({lhs} + {rhs})'
        return []
    m = re.match(r'^(r\d+) = (r\d+) \+ (0x[0-9a-fA-F]+|\d+);$', line)
    if m:
        src = reg_state.get(m.group(2), m.group(2))
        reg_state[m.group(1)] = f'({src} + {m.group(3)})'
        return []
    m = re.match(r'^(r\d+) = (r\d+) - (0x[0-9a-fA-F]+|\d+);$', line)
    if m:
        src = reg_state.get(m.group(2), m.group(2))
        reg_state[m.group(1)] = f'({src} - {m.group(3)})'
        return []
    m = re.match(r'^(r\d+) = (r\d+) - (r\d+);$', line)
    if m:
        lhs = reg_state.get(m.group(2), m.group(2))
        rhs = reg_state.get(m.group(3), m.group(3))
        reg_state[m.group(1)] = f'({lhs} - {rhs})'
        return []
    m = re.match(r'^(r\d+) = (r\d+) & (0x[0-9a-fA-F]+|\d+);$', line)
    if m:
        src = reg_state.get(m.group(2), m.group(2))
        mask = m.group(3)
        if mask == '0xFF': reg_state[m.group(1)] = f'(u8){src}'
        elif mask == '0xFFFF': reg_state[m.group(1)] = f'(u16){src}'
        else: reg_state[m.group(1)] = f'({src} & {mask})'
        return []
    m = re.match(r'^(r\d+) = \(s8\)(r\d+);$', line)
    if m:
        src = reg_state.get(m.group(2), m.group(2))
        reg_state[m.group(1)] = f'(s8){src}'
        return []
    m = re.match(r'^(r\d+) = \(s16\)(r\d+);$', line)
    if m:
        src = reg_state.get(m.group(2), m.group(2))
        reg_state[m.group(1)] = f'(s16){src}'
        return []
    # LOAD
    m = re.match(r'^(r\d+) = \*\((u32|u16|u8|s32)\*\)\(\(u8\*\)(r\d+) \+ (0x[0-9a-fA-F]+)\);$', line)
    if m:
        base = reg_state.get(m.group(3), m.group(3))
        bs = simplify_expr(base)
        if bs is None: return None
        reg_state[m.group(1)] = f'*({m.group(2)}*)({bs} + {m.group(4)})'
        return []
    m = re.match(r'^(f\d+) = \*\((f32|f64)\*\)(lbl_[0-9a-fA-F]+);$', line)
    if m:
        reg_state[m.group(1)] = f'*({m.group(2)}*){m.group(3)}'
        return []
    m = re.match(r'^(r\d+) = \*\(u32\*\)&(lbl_[0-9a-fA-F]+);$', line)
    if m:
        reg_state[m.group(1)] = f'*(u32*)&{m.group(2)}'
        return []
    # STORE
    m = re.match(r'^\*\((u32|u16|u8|f32)\*\)\(\(u8\*\)(r\d+) \+ (0x[0-9a-fA-F]+)\) = (r\d+|f\d+);$', line)
    if m:
        base = reg_state.get(m.group(2), m.group(2))
        val = reg_state.get(m.group(4), m.group(4))
        bs = simplify_expr(base)
        vs = simplify_expr(val)
        if bs is None or vs is None: return None
        return [f'*({m.group(1)}*)({bs} + {m.group(3)}) = {vs};']
    m = re.match(r'^\*\((u32|u16|u8)\*\)(lbl_[0-9a-fA-F]+) = (r\d+);$', line)
    if m:
        val = reg_state.get(m.group(3), m.group(3))
        vs = simplify_expr(val)
        if vs is None: return None
        return [f'*({m.group(1)}*){m.group(2)} = {vs};']
    m = re.match(r'^\*\(u32\*\)lbl_8047AA80 = (r\d+);$', line)
    if m:
        val = reg_state.get(m.group(1), m.group(1))
        vs = simplify_expr(val)
        if vs is None: return None
        return [f'*(u32*)lbl_8047AA80 = {vs};']
    # FUNCTION CALLS
    m = re.match(r'^(fn_[0-9a-fA-F]+)\(\);$', line)
    if m:
        for vreg in ['r0','r3','r4','r5','r6','r7','r8','r9','r10','r11','r12']:
            reg_state.pop(vreg, None)
            reg_state[vreg] = '<clobbered>'
        return [line]
    m = re.match(r'^(fn_[0-9a-fA-F]+)\((.+)\);$', line)
    if m:
        for vreg in ['r0','r3','r4','r5','r6','r7','r8','r9','r10','r11','r12']:
            reg_state[vreg] = '<clobbered>'
        return [line]
    if line.startswith('memcpy(') or line.startswith('memset('):
        resolved = line
        for reg in sorted(reg_state.keys(), key=lambda r: -len(r)):
            val = simplify_expr(reg_state[reg])
            if val is None: return None
            resolved = resolved.replace(f'(void*){reg}', f'(void*){val}')
            resolved = resolved.replace(f'(const void*){reg}', f'(const void*){val}')
            resolved = resolved.replace(f'(u32){reg})', f'(u32){val})')
        for vreg in ['r0','r3','r4','r5','r6','r7','r8','r9','r10','r11','r12']:
            reg_state[vreg] = '<clobbered>'
        return [resolved]
    m = re.match(r'^(\w+)\(\);$', line)
    if m:
        for vreg in ['r0','r3','r4','r5','r6','r7','r8','r9','r10','r11','r12']:
            reg_state[vreg] = '<clobbered>'
        return [line]
    # Unrecognized with register refs
    if re.search(r'\br\d+\b', line) or re.search(r'\bf\d+\b', line):
        return None
    return [line]


def simulate_else_branch_with_state(else_lines, entry_state):
    """Simulate register dataflow in else branch with pre-computed entry state."""
    if not else_lines:
        return []
    reg_state = dict(entry_state)
    output = []
    for l in else_lines:
        result = convert_line_with_state(l, reg_state)
        if result is None:
            return None
        output.extend(result)
    return output


def simulate_else_branch(else_lines, param_saves, param_names, reg_decls):
    """
    Simulate register dataflow in else branch and convert to C statements.

    Tracks register values as symbolic expressions and converts
    store operations to equivalent C.
    """
    if not else_lines:
        return []

    # Track register state: reg -> symbolic expression
    reg_state = {}

    # Initialize with param names
    for reg, pname in param_names.items():
        reg_state[reg] = pname

    # Also set state pointer
    # Find which register holds the state pointer
    for l in else_lines:
        m = re.match(r'^(r\d+) = \*\(u32\*\)lbl_8047AA80;$', l)
        if m:
            reg_state[m.group(1)] = 'state'

    output = []

    for l in else_lines:
        # r3 = *(u32*)lbl_8047AA80;
        m = re.match(r'^(r\d+) = \*\(u32\*\)lbl_8047AA80;$', l)
        if m:
            reg_state[m.group(1)] = 'state'
            continue

        # r4 = (u32)fn_XXXXXXXX;
        m = re.match(r'^(r\d+) = \(u32\)(fn_[0-9a-fA-F]+);$', l)
        if m:
            reg_state[m.group(1)] = f'(u32){m.group(2)}'
            continue

        # r4 = (u32)lbl_XXXXXXXX;
        m = re.match(r'^(r\d+) = \(u32\)(lbl_[0-9a-fA-F]+(?:\[\])?);$', l)
        if m:
            reg_state[m.group(1)] = f'(u32){m.group(2)}'
            continue

        # r0 = 0x1; (or other constant)
        m = re.match(r'^(r\d+) = (0x[0-9a-fA-F]+|0|-?\d+);$', l)
        if m:
            reg_state[m.group(1)] = m.group(2)
            continue

        # r5 = r3 << 2;
        m = re.match(r'^(r\d+) = (r\d+) << (\d+);$', l)
        if m:
            src_val = reg_state.get(m.group(2), m.group(2))
            reg_state[m.group(1)] = f'({src_val} << {m.group(3)})'
            continue

        # r3 = r3 * 0x1c;
        m = re.match(r'^(r\d+) = (r\d+) \* (0x[0-9a-fA-F]+|\d+);$', l)
        if m:
            src_val = reg_state.get(m.group(2), m.group(2))
            reg_state[m.group(1)] = f'({src_val} * {m.group(3)})'
            continue

        # r3 = r7 + r5;
        m = re.match(r'^(r\d+) = (r\d+) \+ (r\d+);$', l)
        if m:
            lhs_val = reg_state.get(m.group(2), m.group(2))
            rhs_val = reg_state.get(m.group(3), m.group(3))
            reg_state[m.group(1)] = f'({lhs_val} + {rhs_val})'
            continue

        # r3 = r3 + 0x360;
        m = re.match(r'^(r\d+) = (r\d+) \+ (0x[0-9a-fA-F]+|\d+);$', l)
        if m:
            src_val = reg_state.get(m.group(2), m.group(2))
            reg_state[m.group(1)] = f'({src_val} + {m.group(3)})'
            continue

        # r3 = r3 - 0x1;
        m = re.match(r'^(r\d+) = (r\d+) - (0x[0-9a-fA-F]+|\d+);$', l)
        if m:
            src_val = reg_state.get(m.group(2), m.group(2))
            reg_state[m.group(1)] = f'({src_val} - {m.group(3)})'
            continue

        # r0 = r3 & 0xFF;
        m = re.match(r'^(r\d+) = (r\d+) & (0x[0-9a-fA-F]+|\d+);$', l)
        if m:
            src_val = reg_state.get(m.group(2), m.group(2))
            reg_state[m.group(1)] = f'({src_val} & {m.group(3)})'
            continue

        # r3 = r31; (simple copy)
        m = re.match(r'^(r\d+) = (r\d+);$', l)
        if m:
            reg_state[m.group(1)] = reg_state.get(m.group(2), m.group(2))
            continue

        # *(u32*)((u8*)r3 + 0x4A8) = r4;
        m = re.match(r'^\*\((u32|u16|u8|f32)\*\)\(\(u8\*\)(r\d+) \+ (0x[0-9a-fA-F]+)\) = (r\d+|f\d+);$', l)
        if m:
            cast_type = m.group(1)
            base = reg_state.get(m.group(2), m.group(2))
            offset = m.group(3)
            val = reg_state.get(m.group(4), m.group(4))

            # Simplify: if base is 'state', emit clean
            if base == 'state' or base == '(u32)state':
                output.append(f'*({cast_type}*)(state + {offset}) = {val};')
            else:
                output.append(f'*({cast_type}*)((u8*){base} + {offset}) = {val};')
            continue

        # *(u32*)lbl_8047AA80 = rN;
        m = re.match(r'^\*\(u32\*\)lbl_8047AA80 = (r\d+);$', l)
        if m:
            val = reg_state.get(m.group(1), m.group(1))
            output.append(f'*(u32*)lbl_8047AA80 = {val};')
            continue

        # *(u16*)lbl_XXXXXXXX = rN;
        m = re.match(r'^\*\((u32|u16|u8)\*\)(lbl_[0-9a-fA-F]+) = (r\d+);$', l)
        if m:
            cast = m.group(1)
            label = m.group(2)
            val = reg_state.get(m.group(3), m.group(3))
            output.append(f'*({cast}*){label} = {val};')
            continue

        # *(f32*)((u8*)r26 + 0x0) = f0;
        m = re.match(r'^\*\((f32|f64)\*\)\(\(u8\*\)(r\d+) \+ (0x[0-9a-fA-F]+)\) = (f\d+);$', l)
        if m:
            cast_type = m.group(1)
            base = reg_state.get(m.group(2), m.group(2))
            offset = m.group(3)
            val = reg_state.get(m.group(4), m.group(4))
            output.append(f'*({cast_type}*)((u8*){base} + {offset}) = {val};')
            continue

        # Function calls: fn_XXXXXXXX();
        m = re.match(r'^(fn_[0-9a-fA-F]+)\(\);$', l)
        if m:
            output.append(l)
            continue

        # Function calls with args
        m = re.match(r'^(fn_[0-9a-fA-F]+)\((.+)\);$', l)
        if m:
            output.append(l)
            continue

        # memcpy/memset
        if l.startswith('memcpy(') or l.startswith('memset('):
            # Resolve register refs in args
            resolved = l
            for reg in sorted(reg_state.keys(), key=lambda r: -len(r)):
                resolved = resolved.replace(f'(void*){reg}', f'(void*){reg_state[reg]}')
                resolved = resolved.replace(f'(const void*){reg}', f'(const void*){reg_state[reg]}')
                resolved = resolved.replace(f'(u32){reg}', f'(u32){reg_state[reg]}')
            output.append(resolved)
            continue

        # r3 += NNN; *(u8*)r3 = rM;
        m = re.match(r'^(r\d+) \+= (\d+); \*\(u8\*\)(r\d+) = (r\d+);$', l)
        if m:
            # Complex combined operation - bail
            return None

        # OSGetTick(), other void calls
        m = re.match(r'^(\w+)\(\);$', l)
        if m:
            output.append(l)
            continue

        # If we get here with an unhandled line that uses registers, bail
        if re.search(r'\br\d+\b', l) or re.search(r'\bf\d+\b', l):
            # Try to resolve register references
            resolved = l
            has_unresolved = False
            for reg in re.findall(r'\b(r\d+|f\d+)\b', l):
                if reg in reg_state:
                    resolved = re.sub(r'\b' + reg + r'\b', str(reg_state[reg]), resolved, count=1)
                else:
                    has_unresolved = True
            if has_unresolved:
                return None  # Can't resolve all register refs
            output.append(resolved)
            continue

        # Non-register lines
        output.append(l)

    return output


def process_file(filepath):
    """Process a C file, converting all pragma blocks."""
    lines, blocks = extract_blocks(filepath)

    print(f"Found {len(blocks)} pragma blocks in {os.path.basename(filepath)}")

    # Process blocks from end to start
    converted = 0
    kept = 0
    stats = {}

    for start, end, block_lines in reversed(blocks):
        parsed = parse_block(block_lines)
        if parsed is None:
            kept += 1
            continue

        result = convert_function(parsed)

        body_text = '\n'.join(parsed['body'])
        has_47e = '0x47E' in body_text
        has_gslog = 'fn_800D4F98' in body_text
        has_jt = 'jumptable_' in body_text
        gotos = len(re.findall(r'\bgoto\b', body_text))

        if has_jt: cat = 'JUMPTABLE'
        elif has_47e and has_gslog and gotos <= 3: cat = 'STATE_47E'
        elif has_gslog and gotos <= 3: cat = 'STATE_SIMPLE'
        elif has_gslog and gotos <= 6: cat = 'STATE_MULTI'
        else: cat = 'OTHER'

        if result is not None:
            # Find the comment line(s) before pragma push
            # Look for /* fn_XXXXXXXX | Size: 0xNN */ pattern
            comment_start = start
            while comment_start > 0 and lines[comment_start - 1].strip().startswith('/*'):
                comment_start -= 1

            # Preserve comments
            comments = lines[comment_start:start]

            new_lines = []
            for c in comments:
                new_lines.append(c)
            for l in result:
                new_lines.append(l + '\n')
            new_lines.append('\n')

            lines[comment_start:end] = new_lines
            converted += 1
            stats[cat + '_OK'] = stats.get(cat + '_OK', 0) + 1
        else:
            kept += 1
            stats[cat + '_FAIL'] = stats.get(cat + '_FAIL', 0) + 1

    # Write result
    with open(filepath, 'w', encoding='utf-8', newline='') as f:
        f.writelines(lines)

    print(f"Converted: {converted}")
    print(f"Kept as pragma: {kept}")
    for k, v in sorted(stats.items()):
        print(f"  {k}: {v}")

    return converted


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python convert_pragma_blocks.py <file.c> [file2.c ...]")
        sys.exit(1)

    total = 0
    for filepath in sys.argv[1:]:
        total += process_file(filepath)
        print()
    print(f"Total converted: {total}")
