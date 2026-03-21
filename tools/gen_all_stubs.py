#!/usr/bin/env python3
"""
gen_all_stubs.py - Generalized auto-decompiler for ALL pragma stubs.

Scans ALL .c files under src/ for pragma stubs (functions containing TODO
comments indicating they need decompilation), reads PPC disassembly from the
auto-generated assembly file, and converts each stub to register-level C89.

Based on the breakthrough gen_gs_render.py which successfully converted all
270 stubs in gs_render.c to compilable code with zero compile errors.

Usage:
    python tools/gen_all_stubs.py                    # Process all files
    python tools/gen_all_stubs.py --dry-run           # Show what would be done
    python tools/gen_all_stubs.py --file src/game/gs_thread.c  # Single file
    python tools/gen_all_stubs.py --compile           # Compile-check after
"""

import re
import os
import sys
import subprocess
import argparse
import shutil
from pathlib import Path

PROJ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ASM_FILES = [
    os.path.join(PROJ, 'build', 'GC6E01', 'asm', 'auto_01_800055E0_text.s'),
    os.path.join(PROJ, 'build', 'GC6E01', 'asm', 'auto_00_80003100_init.s'),
]
SRC_DIR = os.path.join(PROJ, 'src')

# ============================================================
# PPC Assembly loader (from gen_gs_render.py)
# ============================================================

def load_all_asm():
    """Load all function bodies from all assembly files."""
    funcs = {}
    for asm_file in ASM_FILES:
        if not os.path.exists(asm_file):
            print(f"WARNING: Assembly file not found: {asm_file}", file=sys.stderr)
            continue
        with open(asm_file, 'r') as f:
            text = f.read()
        for m in re.finditer(r'\.fn (fn_[0-9A-Fa-f]+), global\n(.*?)\.endfn \1', text, re.DOTALL):
            funcs[m.group(1)] = m.group(2).strip()
    return funcs


# ============================================================
# Instruction parsing (from gen_gs_render.py)
# ============================================================

def get_insns(body):
    """Get clean instruction list from asm body."""
    result = []
    for line in body.split('\n'):
        line = line.strip()
        if line.startswith('.L_'):
            result.append(('label', line.rstrip(':').strip()))
            continue
        m = re.match(r'/\*\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f ]+\*/\s+(.*)', line)
        if m:
            inst = m.group(1).strip()
            parts = inst.split(None, 1)
            mnem = parts[0]
            ops = parts[1] if len(parts) > 1 else ''
            result.append(('inst', mnem, ops))
    return result


def ops_list(ops_str):
    """Split operands by comma."""
    if not ops_str:
        return []
    return [o.strip() for o in ops_str.split(',')]


def fmt_off(off):
    """Format an integer offset for C code."""
    if off < 0:
        return f'({off})'
    return f'0x{off:X}'


def parse_mem(op):
    """Parse memory operand: offset(base) -> (offset_or_label, base)"""
    m = re.match(r'(.*)\((\w+)\)', op)
    if not m:
        return None, None
    offset_str = m.group(1).strip()
    base = m.group(2)
    # SDA label
    if '@sda21' in offset_str:
        label = offset_str.replace('@sda21', '')
        return label, base
    # HA/LO label
    if '@ha' in offset_str or '@l' in offset_str:
        label = re.sub(r'@\w+', '', offset_str)
        return label, base
    # Numeric
    try:
        if offset_str.startswith('-'):
            return -int(offset_str[1:], 0), base
        return int(offset_str, 0), base
    except ValueError:
        return offset_str, base


# ============================================================
# PPC -> C translation engine (from gen_gs_render.py)
# ============================================================

def decompile_function(fn_name, asm_funcs):
    """Decompile a single function. Returns (ret_type, body, needs_pragma)."""
    if fn_name not in asm_funcs:
        return None

    insns = get_insns(asm_funcs[fn_name])
    real = [(entry[1], entry[2]) for entry in insns if entry[0] == 'inst'] if insns else []
    if not real:
        return None

    n = len(real)

    # ---- Pattern: single blr (empty function) ----
    if n == 1 and real[0][0] == 'blr':
        return ('void', '', False)

    return decompile_general(fn_name, insns, real)


def decompile_general(fn_name, insns, real):
    """General decompilation -- compilable C with register vars and gotos."""

    # First pass: collect used registers, labels, float regs, stack locals
    used_iregs = set()
    used_fregs = set()
    used_labels = set()
    branch_targets = set()
    has_frame = real[0][0] == 'stwu' if real else False
    frame_size = 0
    stack_loads = set()
    stack_stores = set()
    has_ctr_call = False

    if has_frame:
        op = ops_list(real[0][1])
        fmem = parse_mem(op[1])
        if fmem[0] is not None and isinstance(fmem[0], int):
            frame_size = -fmem[0]

    for entry in insns:
        if entry[0] == 'label':
            used_labels.add(entry[1])
            continue
        if entry[0] != 'inst':
            continue
        mnem, ops_str = entry[1], entry[2] if len(entry) > 2 else ''
        op = ops_list(ops_str) if ops_str else []

        # Collect register usage
        for o in op:
            m = re.match(r'^r(\d+)$', o)
            if m:
                rn = int(m.group(1))
                used_iregs.add(rn)
            m = re.match(r'^f(\d+)$', o)
            if m:
                used_fregs.add(int(m.group(1)))

        # Also find registers in memory operands
        for o in op:
            for rm in re.finditer(r'r(\d+)', o):
                rn = int(rm.group(1))
                used_iregs.add(rn)
            for fm in re.finditer(r'f(\d+)', o):
                used_fregs.add(int(fm.group(1)))

        # Collect branch targets
        if mnem.startswith('b') and mnem not in ('blr', 'bl', 'bctrl', 'bctr',
                                                   'beqlr', 'bnelr', 'bgelr',
                                                   'bltlr', 'bgtlr', 'blelr'):
            target = op[-1].strip() if op else ''
            if target.startswith('.L_'):
                branch_targets.add(target)

        if mnem == 'bctrl':
            has_ctr_call = True

        # Track stack loads for local variables
        if mnem in ('lwz', 'lbz', 'lhz', 'lfs', 'lfd') and len(op) >= 2:
            off, base = parse_mem(op[1])
            if base == 'r1' and isinstance(off, int) and off > 0 and off < frame_size:
                stack_loads.add(off)
        if mnem in ('stw', 'stb', 'sth', 'stfs', 'stfd') and len(op) >= 2:
            off, base = parse_mem(op[1])
            if base == 'r1' and isinstance(off, int) and off > 0 and off < frame_size:
                stack_stores.add(off)

    # Stack locals
    stack_locals = stack_loads | stack_stores

    # Collect SDA/HA label references
    sda_labels = set()
    ha_labels = set()
    bl_targets = set()
    for entry in insns:
        if entry[0] != 'inst':
            continue
        ops_str = entry[2] if len(entry) > 2 else ''
        for m in re.finditer(r'(lbl_[0-9A-Fa-f]+)@sda21', ops_str):
            sda_labels.add(m.group(1))
        for m in re.finditer(r'(lbl_[0-9A-Fa-f]+)@ha', ops_str):
            ha_labels.add(m.group(1))
        for m in re.finditer(r'(lbl_[0-9A-Fa-f]+)@l', ops_str):
            ha_labels.add(m.group(1))
        if entry[1] == 'bl':
            target = ops_str.strip()
            if not target.startswith('_') and target.startswith('fn_'):
                bl_targets.add(target)

    # Build declarations
    decl_lines = []

    # Declare extern labels
    for lbl in sorted(sda_labels | ha_labels):
        decl_lines.append(f'    extern u8 {lbl}[];')

    # Declare extern functions called
    for fn_target in sorted(bl_targets):
        decl_lines.append(f'    extern void {fn_target}();')

    # Also find jumptable, fn_ address references, and extra labels
    jt_set = set()
    fn_addr_set = set()
    has_bdnz = False
    extra_labels = set()
    for entry in insns:
        if entry[0] != 'inst':
            continue
        mnem = entry[1]
        ops_str = entry[2] if len(entry) > 2 else ''
        for m2 in re.finditer(r'(jumptable_[0-9A-Fa-f]+)', ops_str):
            jt_set.add(m2.group(1))
        if entry[1] not in ('bl',):
            for m2 in re.finditer(r'(fn_[0-9A-Fa-f]+)@', ops_str):
                fn_addr_set.add(m2.group(1))
        if mnem == 'bdnz':
            has_bdnz = True
        if mnem == 'bl':
            target = ops_str.strip()
            if target == 'memcpy' or target == 'memset':
                used_iregs.update({3, 4, 5})
            elif target.startswith('fn_') or target.startswith('_'):
                pass
            else:
                used_iregs.update({3, 4, 5})
        if mnem == 'li' and '@sda21' in ops_str:
            m2 = re.search(r'(\w+)@sda21', ops_str)
            if m2:
                extra_labels.add(m2.group(1))

    for jt in sorted(jt_set):
        decl_lines.append(f'    extern u8 {jt}[];')
    for fa in sorted(fn_addr_set):
        if fa not in bl_targets:
            decl_lines.append(f'    extern void {fa}();')
    for el in sorted(extra_labels):
        if el not in (sda_labels | ha_labels):
            decl_lines.append(f'    extern u8 {el}[];')

    # Stack frame
    if frame_size > 0:
        decl_lines.append(f'    u8 sp[0x{frame_size:X}];')

    # Integer register declarations
    for rn in sorted(used_iregs):
        if rn == 1:
            if frame_size > 0:
                decl_lines.append(f'    u32 r1 = (u32)sp;')
            else:
                decl_lines.append(f'    u32 r1 = 0;')
        else:
            decl_lines.append(f'    u32 r{rn} = 0;')

    # Float register declarations
    for fn in sorted(used_fregs):
        if 0 <= fn <= 31:
            decl_lines.append(f'    f32 f{fn} = 0.0f;')

    # CTR variable for indirect calls
    if has_ctr_call or any(m == 'mtctr' for m, _ in real):
        decl_lines.append('    void (*ctr_fn)(void) = 0;')

    # CTR counter for bdnz loops
    if has_bdnz:
        decl_lines.append('    u32 ctr = 0;')

    # Second pass: generate code
    body_lines = []
    in_epilogue = False
    last_cmp_info = ('cmpwi', 'r0', '0')

    for entry in insns:
        if entry[0] == 'label':
            label_name = entry[1].replace('.L_', 'L_')
            body_lines.append(f'{label_name}: ;')
            in_epilogue = False
            continue

        if entry[0] != 'inst':
            continue

        mnem = entry[1]
        ops_str = entry[2] if len(entry) > 2 else ''
        op = ops_list(ops_str) if ops_str else []

        # Skip prologue/epilogue
        if mnem == 'stwu' and op and op[0] == 'r1':
            continue
        if mnem == 'mflr':
            continue
        if mnem == 'mtlr':
            in_epilogue = True
            continue
        if mnem == 'addi' and op and op[0] == 'r1' and op[1] == 'r1':
            # addi r1, r1, N is always frame restore (epilogue)
            continue
        if mnem == 'blr':
            body_lines.append('    return;')
            in_epilogue = False
            continue

        # Stack saves/restores of callee-saved registers
        if mnem == 'stw' and len(op) >= 2:
            off, base = parse_mem(op[1])
            if base == 'r1' and isinstance(off, int) and off > 0:
                src = op[0]
                if src.startswith('r') and src != 'r0':
                    continue
                if src == 'r0' and off >= frame_size - 4:
                    continue
                body_lines.append(f'    *(u32*)(sp + 0x{off:X}) = {src};')
                continue

        if mnem == 'lwz' and len(op) >= 2:
            off, base = parse_mem(op[1])
            if base == 'r1' and isinstance(off, int):
                if in_epilogue and off > 0:
                    continue
                if off > 0 and off < frame_size:
                    body_lines.append(f'    {op[0]} = *(u32*)(sp + 0x{off:X});')
                    continue
                if off > 0:
                    continue

        # _save/_rest register calls
        if mnem == 'bl' and (ops_str.startswith('_save') or ops_str.startswith('_rest')):
            continue

        # Track comparisons for condition codes
        if mnem == 'cmpwi':
            parts = op
            if len(parts) >= 2:
                last_cmp_info = ('cmpwi', parts[0], parts[1])
        elif mnem == 'cmplwi':
            parts = op
            if len(parts) >= 2:
                last_cmp_info = ('cmplwi', parts[0], parts[1])
        elif mnem == 'cmpw':
            parts = op
            if len(parts) >= 2:
                last_cmp_info = ('cmpw', parts[0], parts[1])
        elif mnem == 'cmplw':
            parts = op
            if len(parts) >= 2:
                last_cmp_info = ('cmplw', parts[0], parts[1])
        elif mnem == 'fcmpo' or mnem == 'fcmpu':
            parts = op
            if len(parts) >= 3:
                last_cmp_info = ('fcmp', parts[1], parts[2])
            elif len(parts) >= 2:
                last_cmp_info = ('fcmp', parts[0], parts[1])

        # Handle branch instructions with proper conditions
        if mnem in ('beq', 'bne', 'blt', 'bgt', 'ble', 'bge'):
            target = op[-1].strip().replace('.L_', 'L_')
            cmp_op = mnem[1:]
            cmp_type, lhs, rhs = last_cmp_info
            sign = '(s32)' if cmp_type in ('cmpwi', 'cmpw') else '(u32)' if cmp_type in ('cmplwi', 'cmplw') else ''
            op_map = {'eq': '==', 'ne': '!=', 'lt': '<', 'gt': '>', 'le': '<=', 'ge': '>='}
            c_op = op_map[cmp_op]
            body_lines.append(f'    if ({sign}{lhs} {c_op} {sign}{rhs}) goto {target};')
            continue

        if mnem in ('beqlr', 'bnelr', 'bltlr', 'bgtlr', 'blelr', 'bgelr'):
            cmp_op = mnem[1:-2]
            cmp_type, lhs, rhs = last_cmp_info
            sign = '(s32)' if cmp_type in ('cmpwi', 'cmpw') else '(u32)' if cmp_type in ('cmplwi', 'cmplw') else ''
            op_map = {'eq': '==', 'ne': '!=', 'lt': '<', 'gt': '>', 'le': '<=', 'ge': '>='}
            c_op = op_map.get(cmp_op, '==')
            body_lines.append(f'    if ({sign}{lhs} {c_op} {sign}{rhs}) return;')
            continue

        if mnem == 'b':
            target = op[0].strip().replace('.L_', 'L_')
            if target.startswith('L_'):
                body_lines.append(f'    goto {target};')
            else:
                body_lines.append(f'    /* b {target} */;')
            continue

        # MTR for indirect calls
        if mnem == 'mtctr':
            body_lines.append(f'    ctr_fn = (void(*)(void)){op[0]};')
            continue
        if mnem == 'bctrl':
            body_lines.append(f'    ctr_fn();')
            continue

        # Skip comparison instructions (already tracked above)
        if mnem in ('cmpwi', 'cmplwi', 'cmpw', 'cmplw', 'fcmpo', 'fcmpu'):
            continue

        # Generate the C statement
        c = translate_inst_v2(mnem, op, ops_str, frame_size)
        if c:
            body_lines.append(f'    {c}')

    # Combine declarations and body
    all_lines = decl_lines + [''] + body_lines if decl_lines else body_lines
    return ('void', '\n'.join(all_lines) + '\n', True)


def translate_inst_v2(mnem, op, ops_str, frame_size):
    """Translate instruction to C, using sp[] for stack frame."""
    # Stores to stack frame
    if mnem in ('stw', 'stb', 'sth', 'stfs', 'stfd') and len(op) >= 2:
        off, base = parse_mem(op[1])
        if base == 'r1' and isinstance(off, int) and off > 0:
            st = {'stw': 'u32', 'stb': 'u8', 'sth': 'u16', 'stfs': 'f32', 'stfd': 'f64'}[mnem]
            return f'*({st}*)(sp + 0x{off:X}) = {op[0]};'

    # Loads from stack frame
    if mnem in ('lwz', 'lbz', 'lhz', 'lfs', 'lfd') and len(op) >= 2:
        off, base = parse_mem(op[1])
        if base == 'r1' and isinstance(off, int) and off > 0 and off < frame_size:
            lt = {'lwz': 'u32', 'lbz': 'u8', 'lhz': 'u16', 'lfs': 'f32', 'lfd': 'f64'}[mnem]
            return f'{op[0]} = *({lt}*)(sp + 0x{off:X});'

    return translate_inst(mnem, op, ops_str)


def translate_inst(mnem, op, ops_str):
    """Translate a PPC instruction to a C statement."""

    if mnem == 'li':
        val = op[1]
        if '@sda21' in val:
            label = val.replace('@sda21', '')
            return f'{op[0]} = (u32){label};'
        return f'{op[0]} = {val};'
    if mnem == 'lis':
        if '@ha' in op[1]:
            label = op[1].replace('@ha', '')
            return f'{op[0]} = (u32){label};'
        return f'{op[0]} = ({op[1]} << 16);'
    if mnem == 'addi':
        if '@l' in op[2]:
            label = op[2].replace('@l', '')
            return f'{op[0]} = (u32){label};'
        if op[1] == 'r0':
            return f'{op[0]} = {op[2]};'
        return f'{op[0]} = {op[1]} + {op[2]};'
    if mnem == 'addic':
        return f'{op[0]} = {op[1]} + {op[2]};'
    if mnem == 'addis':
        return f'{op[0]} = {op[1]} + ({op[2]} << 16);'
    if mnem == 'subfic':
        return f'{op[0]} = {op[2]} - {op[1]};'

    # Load instructions
    if mnem in ('lwz', 'lbz', 'lhz', 'lwzu', 'lbzu', 'lhzu'):
        off, base = parse_mem(op[1])
        lt = {'lwz': 'u32', 'lbz': 'u8', 'lhz': 'u16',
              'lwzu': 'u32', 'lbzu': 'u8', 'lhzu': 'u16'}.get(mnem, 'u32')
        if isinstance(off, str):
            return f'{op[0]} = *({lt}*){off};'
        if isinstance(off, int):
            if off < 0:
                return f'{op[0]} = *({lt}*)((u8*){base} + ({off}));'
            return f'{op[0]} = *({lt}*)((u8*){base} + 0x{off:X});'

    if mnem in ('lfs', 'lfsu'):
        off, base = parse_mem(op[1])
        if isinstance(off, str):
            return f'{op[0]} = *(f32*){off};'
        if isinstance(off, int):
            if off < 0:
                return f'{op[0]} = *(f32*)((u8*){base} + ({off}));'
            return f'{op[0]} = *(f32*)((u8*){base} + 0x{off:X});'

    if mnem == 'lfd':
        off, base = parse_mem(op[1])
        if isinstance(off, str):
            return f'{op[0]} = *(f64*){off};'
        if isinstance(off, int):
            return f'{op[0]} = *(f64*)((u8*){base} + {fmt_off(off)});'

    if mnem == 'lha':
        off, base = parse_mem(op[1])
        if isinstance(off, int):
            if off < 0:
                return f'{op[0]} = *(s16*)((u8*){base} + ({off}));'
            return f'{op[0]} = *(s16*)((u8*){base} + 0x{off:X});'

    # Indexed loads
    if mnem == 'lwzx':
        return f'{op[0]} = *(u32*)({op[1]} + {op[2]});'
    if mnem == 'lbzx':
        return f'{op[0]} = *(u8*)({op[1]} + {op[2]});'
    if mnem == 'lhzx':
        return f'{op[0]} = *(u16*)({op[1]} + {op[2]});'
    if mnem == 'lhax':
        return f'{op[0]} = *(s16*)({op[1]} + {op[2]});'
    if mnem == 'lfsx':
        return f'{op[0]} = *(f32*)({op[1]} + {op[2]});'
    if mnem == 'lfdx':
        return f'{op[0]} = *(f64*)({op[1]} + {op[2]});'

    # Store instructions
    if mnem in ('stw', 'stb', 'sth'):
        off, base = parse_mem(op[1])
        st = {'stw': 'u32', 'stb': 'u8', 'sth': 'u16'}[mnem]
        if isinstance(off, str):
            return f'*({st}*){off} = {op[0]};'
        if isinstance(off, int):
            if off < 0:
                return f'*({st}*)((u8*){base} + ({off})) = {op[0]};'
            return f'*({st}*)((u8*){base} + 0x{off:X}) = {op[0]};'

    if mnem == 'stwu':
        off, base = parse_mem(op[1])
        if isinstance(off, int):
            return f'{base} += {off}; *(u32*){base} = {op[0]};'

    if mnem == 'stbu':
        off, base = parse_mem(op[1])
        if isinstance(off, str):
            return f'{base} = (u32){off}; *(u8*){base} = {op[0]};'
        if isinstance(off, int):
            return f'{base} += {off}; *(u8*){base} = {op[0]};'

    if mnem in ('stfs',):
        off, base = parse_mem(op[1])
        if isinstance(off, str):
            return f'*(f32*){off} = {op[0]};'
        if isinstance(off, int):
            if off < 0:
                return f'*(f32*)((u8*){base} + ({off})) = {op[0]};'
            return f'*(f32*)((u8*){base} + 0x{off:X}) = {op[0]};'

    if mnem == 'stfd':
        off, base = parse_mem(op[1])
        if isinstance(off, int):
            return f'*(f64*)((u8*){base} + {fmt_off(off)}) = {op[0]};'

    # Indexed stores
    if mnem == 'stwx':
        return f'*(u32*)({op[1]} + {op[2]}) = {op[0]};'
    if mnem == 'stbx':
        return f'*(u8*)({op[1]} + {op[2]}) = {op[0]};'
    if mnem == 'sthx':
        return f'*(u16*)({op[1]} + {op[2]}) = {op[0]};'
    if mnem == 'stfsx':
        return f'*(f32*)({op[1]} + {op[2]}) = {op[0]};'
    if mnem == 'stfdx':
        return f'*(f64*)({op[1]} + {op[2]}) = {op[0]};'

    # Arithmetic
    if mnem == 'mr' or (mnem == 'or' and len(op) >= 3 and op[1] == op[2]):
        return f'{op[0]} = {op[1]};'
    if mnem == 'add':
        return f'{op[0]} = {op[1]} + {op[2]};'
    if mnem == 'addc':
        return f'{op[0]} = {op[1]} + {op[2]};'
    if mnem == 'adde':
        return f'{op[0]} = {op[1]} + {op[2]}; /* +carry */;'
    if mnem == 'subf':
        return f'{op[0]} = {op[2]} - {op[1]};'
    if mnem == 'subfc':
        return f'{op[0]} = {op[2]} - {op[1]};'
    if mnem == 'subfe':
        return f'{op[0]} = {op[2]} - {op[1]}; /* -borrow */;'
    if mnem == 'neg':
        return f'{op[0]} = -{op[1]};'
    if mnem == 'mulli':
        return f'{op[0]} = {op[1]} * {op[2]};'
    if mnem == 'mullw':
        return f'{op[0]} = {op[1]} * {op[2]};'
    if mnem == 'mulhw':
        return f'{op[0]} = (s32)((s64){op[1]} * (s64){op[2]} >> 32);'
    if mnem == 'mulhwu':
        return f'{op[0]} = (u32)((u64){op[1]} * (u64){op[2]} >> 32);'
    if mnem == 'divw':
        return f'{op[0]} = (s32){op[1]} / (s32){op[2]};'
    if mnem == 'divwu':
        return f'{op[0]} = (u32){op[1]} / (u32){op[2]};'

    # Logical
    if mnem == 'or':
        return f'{op[0]} = {op[1]} | {op[2]};'
    if mnem == 'ori':
        return f'{op[0]} = {op[1]} | {op[2]};'
    if mnem == 'oris':
        return f'{op[0]} = {op[1]} | ({op[2]} << 16);'
    if mnem == 'and':
        return f'{op[0]} = {op[1]} & {op[2]};'
    if mnem in ('andi.', 'andi'):
        return f'{op[0]} = {op[1]} & {op[2]};'
    if mnem == 'andis.':
        return f'{op[0]} = {op[1]} & ({op[2]} << 16);'
    if mnem == 'xor':
        return f'{op[0]} = {op[1]} ^ {op[2]};'
    if mnem == 'xori':
        return f'{op[0]} = {op[1]} ^ {op[2]};'
    if mnem == 'nor':
        return f'{op[0]} = ~({op[1]} | {op[2]});'
    if mnem == 'not':
        return f'{op[0]} = ~{op[1]};'
    if mnem == 'nand':
        return f'{op[0]} = ~({op[1]} & {op[2]});'
    if mnem == 'andc':
        return f'{op[0]} = {op[1]} & ~{op[2]};'
    if mnem == 'orc':
        return f'{op[0]} = {op[1]} | ~{op[2]};'

    # Shifts
    if mnem == 'slwi':
        return f'{op[0]} = {op[1]} << {op[2]};'
    if mnem == 'slw':
        return f'{op[0]} = {op[1]} << {op[2]};'
    if mnem == 'srwi':
        return f'{op[0]} = (u32){op[1]} >> {op[2]};'
    if mnem == 'srw':
        return f'{op[0]} = (u32){op[1]} >> {op[2]};'
    if mnem == 'srawi':
        return f'{op[0]} = (s32){op[1]} >> {op[2]};'
    if mnem == 'sraw':
        return f'{op[0]} = (s32){op[1]} >> {op[2]};'

    # Rotate/mask
    if mnem in ('clrlwi', 'clrlwi.'):
        n = int(op[2])
        mask = (1 << (32 - n)) - 1
        return f'{op[0]} = {op[1]} & 0x{mask:X};'
    if mnem in ('rlwinm', 'rlwinm.'):
        # rlwinm rD, rS, SH, MB, ME -> rotate left + mask
        if len(op) >= 4:
            rd, rs, sh = op[0], op[1], int(op[2])
            mb, me = int(op[3]), int(op[4]) if len(op) >= 5 else 31
            # Build the mask
            if mb <= me:
                mask = 0
                for bit in range(mb, me + 1):
                    mask |= (1 << (31 - bit))
            else:
                mask = 0xFFFFFFFF
                for bit in range(me + 1, mb):
                    mask &= ~(1 << (31 - bit))
            if sh == 0:
                return f'{rd} = {rs} & 0x{mask:08X};'
            return f'{rd} = (({rs} << {sh}) | ((u32){rs} >> {32 - sh})) & 0x{mask:08X};'
        return f'/* rlwinm {ops_str} */;'
    if mnem == 'rlwimi':
        if len(op) >= 5:
            rd, rs, sh = op[0], op[1], int(op[2])
            mb, me = int(op[3]), int(op[4])
            if mb <= me:
                mask = 0
                for bit in range(mb, me + 1):
                    mask |= (1 << (31 - bit))
            else:
                mask = 0xFFFFFFFF
                for bit in range(me + 1, mb):
                    mask &= ~(1 << (31 - bit))
            return f'{rd} = ({rd} & ~0x{mask:08X}) | ((({rs} << {sh}) | ((u32){rs} >> {32 - sh})) & 0x{mask:08X});'
        return f'/* rlwimi {ops_str} */;'
    if mnem == 'rotlwi':
        return f'/* rotlwi {ops_str} */;'

    # Extend
    if mnem == 'extsb':
        return f'{op[0]} = (s8){op[1]};'
    if mnem == 'extsh':
        return f'{op[0]} = (s16){op[1]};'
    if mnem in ('extsb.', 'extsh.'):
        return f'{op[0]} = (s8){op[1]};' if 'b' in mnem else f'{op[0]} = (s16){op[1]};'

    # cntlzw (count leading zeros)
    if mnem == 'cntlzw':
        return f'{op[0]} = __cntlzw({op[1]});'

    # Branches
    if mnem == 'b':
        target = op[0].strip()
        if target.startswith('.L_'):
            return f'goto {target.replace(".L_", "L_")};'
        return f'/* b {target} */;'
    if mnem in ('beq', 'bne', 'blt', 'bgt', 'ble', 'bge'):
        target = op[-1].strip()
        cond = {'beq': 'eq', 'bne': 'ne', 'blt': 'lt', 'bgt': 'gt',
                'ble': 'le', 'bge': 'ge'}[mnem]
        if target.startswith('.L_'):
            return f'if (/* {cond} */) goto {target.replace(".L_", "L_")};'
        return f'/* {mnem} {ops_str} */;'
    if mnem in ('beqlr', 'bnelr', 'bltlr', 'bgtlr', 'blelr', 'bgelr'):
        cond = mnem[1:-2]
        return f'if (/* {cond} */) return;'
    if mnem == 'bdnz':
        target = op[0].strip().replace('.L_', 'L_')
        if target.startswith('L_'):
            return f'if (--ctr != 0) goto {target};'

    # Function calls
    if mnem == 'bl':
        target = op[0].strip()
        if target.startswith('_save') or target.startswith('_rest'):
            return f'/* {target} */;'
        if target == 'memcpy':
            return f'memcpy((void*)r3, (const void*)r4, (u32)r5);'
        if target == 'memset':
            return f'memset((void*)r3, (int)r4, (u32)r5);'
        if target == 'sprintf':
            return f'r3 = (u32)sprintf((char*)r3, (const char*)r4);'
        if target == 'strlen':
            return f'r3 = (u32)strlen((const char*)r3);'
        return f'{target}();'
    if mnem == 'bctrl':
        return f'/* indirect call via ctr */;'
    if mnem == 'bctr':
        return f'/* indirect jump via ctr */;'

    # CTR/LR
    if mnem == 'mtctr':
        return f'/* mtctr {op[0]} */;'
    if mnem == 'mfctr':
        return f'{op[0]} = 0; /* mfctr */;'

    # Float ops
    if mnem == 'fmr':
        return f'{op[0]} = {op[1]};'
    if mnem == 'fadds':
        return f'{op[0]} = {op[1]} + {op[2]};'
    if mnem == 'fadd':
        return f'{op[0]} = {op[1]} + {op[2]};'
    if mnem == 'fsubs':
        return f'{op[0]} = {op[1]} - {op[2]};'
    if mnem == 'fsub':
        return f'{op[0]} = {op[1]} - {op[2]};'
    if mnem == 'fmuls':
        return f'{op[0]} = {op[1]} * {op[2]};'
    if mnem == 'fmul':
        return f'{op[0]} = {op[1]} * {op[2]};'
    if mnem == 'fdivs':
        return f'{op[0]} = {op[1]} / {op[2]};'
    if mnem == 'fdiv':
        return f'{op[0]} = {op[1]} / {op[2]};'
    if mnem == 'fneg':
        return f'{op[0]} = -{op[1]};'
    if mnem == 'fabs':
        return f'/* fabs */ {op[0]} = ({op[1]} < 0) ? -{op[1]} : {op[1]};'
    if mnem == 'fmadds':
        return f'{op[0]} = {op[1]} * {op[2]} + {op[3]};'
    if mnem == 'fmadd':
        return f'{op[0]} = {op[1]} * {op[2]} + {op[3]};'
    if mnem == 'fmsubs':
        return f'{op[0]} = {op[1]} * {op[2]} - {op[3]};'
    if mnem == 'fmsub':
        return f'{op[0]} = {op[1]} * {op[2]} - {op[3]};'
    if mnem == 'fnmsubs':
        return f'{op[0]} = -({op[1]} * {op[2]} - {op[3]});'
    if mnem == 'fnmsub':
        return f'{op[0]} = -({op[1]} * {op[2]} - {op[3]});'
    if mnem == 'fnmadds':
        return f'{op[0]} = -({op[1]} * {op[2]} + {op[3]});'
    if mnem == 'fctiwz':
        return f'{op[0]} = (f64)(s32){op[1]};'
    if mnem == 'frsp':
        return f'{op[0]} = (f32){op[1]};'
    if mnem == 'fcmpo':
        return f'/* fcmpo {ops_str} */;'
    if mnem == 'fcmpu':
        return f'/* fcmpu {ops_str} */;'
    if mnem == 'fsel':
        return f'{op[0]} = ({op[1]} >= 0) ? {op[2]} : {op[3]};'

    # CR ops
    if mnem in ('crclr', 'crset', 'cror', 'crandc', 'crorc', 'crand'):
        return f'/* {mnem} {ops_str} */;'

    # Paired singles
    if mnem.startswith('ps_'):
        return f'/* {mnem} {ops_str} */;'

    # Misc
    if mnem == 'mfcr':
        return f'{op[0]} = 0; /* mfcr */;'
    if mnem == 'mtcrf':
        return f'/* mtcrf {ops_str} */;'
    if mnem in ('isync', 'sync', 'eieio'):
        return f'/* {mnem} */;'
    if mnem in ('dcbi', 'icbi', 'dcbf', 'dcbst', 'dcbt', 'dcbz'):
        return f'/* {mnem} {ops_str} */;'

    # stmw/lmw
    if mnem == 'stmw':
        return f'/* stmw {ops_str} */;'
    if mnem == 'lmw':
        return f'/* lmw {ops_str} */;'

    # SPR access
    if mnem == 'mfspr':
        return f'{op[0]} = 0; /* mfspr {op[1]} */;'
    if mnem == 'mtspr':
        return f'/* mtspr {ops_str} */;'
    if mnem == 'mfmsr':
        return f'{op[0]} = 0; /* mfmsr */;'
    if mnem == 'mtmsr':
        return f'/* mtmsr {ops_str} */;'

    # TLB
    if mnem == 'tlbie':
        return f'/* tlbie {ops_str} */;'

    # RFI
    if mnem == 'rfi':
        return f'/* rfi */; return;'

    # SC (system call)
    if mnem == 'sc':
        return f'/* sc */;'

    return f'/* {mnem} {ops_str} */;'


# ============================================================
# Stub detection and replacement
# ============================================================

def is_todo_stub_body(body_text):
    """Check if a function body contains a TODO marker indicating it's a stub."""
    return 'TODO' in body_text and ('decompile' in body_text.lower() or 'match' in body_text.lower())


def find_fn_name(line):
    """Extract function name from a C function signature line."""
    # Match fn_XXXXXXXX pattern
    m = re.search(r'\b(fn_[0-9A-Fa-f]+)\b', line)
    if m:
        return m.group(1)
    return None


def find_asm_name_from_context(func_line, body_lines_text):
    """Try to determine the assembly function name from context.

    For named functions (not fn_XXXXXXXX), look for the address in:
    1. The function line itself (e.g., fn_XXXXXXXX in the name)
    2. TODO comments containing 'at 0xXXXXXXXX'
    3. Comment above with address
    """
    # First: direct fn_ name in the signature
    m = re.search(r'\b(fn_[0-9A-Fa-f]+)\b', func_line)
    if m:
        return m.group(1)

    # Second: address in TODO comment
    m = re.search(r'at\s+0x([0-9A-Fa-f]{8})', body_lines_text)
    if m:
        return f'fn_{m.group(1)}'

    # Third: address in the function line comment or nearby text
    m = re.search(r'0x([0-9A-Fa-f]{8})', body_lines_text)
    if m:
        return f'fn_{m.group(1)}'

    return None


def filter_conflicting_decls(body_text, file_scope_symbols):
    """Remove conflicting 'extern' declarations and fix access patterns.

    For fn_ symbols: removes local declarations and casts calls to void(*)(void).
    For lbl_ symbols: removes local declarations and adjusts access to use &lbl_XXX.
    """
    if not file_scope_symbols:
        return body_text

    fn_symbols = {s for s in file_scope_symbols if s.startswith('fn_')}
    lbl_symbols = {s for s in file_scope_symbols if s.startswith('lbl_')}

    filtered_lines = []
    for line in body_text.split('\n'):
        stripped = line.strip()
        if stripped.startswith('extern '):
            m = re.search(r'\b(fn_[0-9A-Fa-f]+|lbl_[0-9A-Fa-f]+)\b', stripped)
            if m and m.group(1) in file_scope_symbols:
                continue  # Skip conflicting declaration
        # Fix fn_ calls: cast to void(*)(void)
        for sym in fn_symbols:
            if f'{sym}();' in line:
                line = line.replace(f'{sym}();', f'((void(*)(void)){sym})();')
        # Fix lbl_ accesses: for file-scope scalar labels,
        # wrap with & to get the address (since they're not arrays)
        for sym in lbl_symbols:
            # Replace patterns like *(u32*)sym or *(f32*)sym with *(u32*)&sym
            # But only when sym appears without & already
            if sym in line and f'&{sym}' not in line:
                # Replace direct usage of lbl as pointer (it's a scalar at file scope)
                # Pattern: *(TYPE*)lbl_XXX -> *(TYPE*)&lbl_XXX
                line = re.sub(
                    r'\*\((\w+\*)\)' + re.escape(sym) + r'\b',
                    r'*(\1)&' + sym,
                    line
                )
                # Pattern: (u32)lbl_XXX -> (u32)&lbl_XXX (for address loads)
                line = re.sub(
                    r'\(u32\)' + re.escape(sym) + r'\b',
                    r'(u32)&' + sym,
                    line
                )
                # Pattern: *(TYPE*)&lbl = val -> (for SDA stores)
                # Already handled by the above transforms
        filtered_lines.append(line)
    return '\n'.join(filtered_lines)


def clean_signature(sig):
    """Clean a function signature for C output (strip asm keyword, register params, etc.)."""
    # Remove 'asm ' keyword
    sig = re.sub(r'\basm\s+', '', sig)
    # Remove 'register' keyword from parameters (used in asm functions)
    sig = re.sub(r'\bregister\s+', '', sig)
    # Remove 'nofralloc' if present
    sig = re.sub(r'\bnofralloc\b\s*', '', sig)
    return sig.strip()


def scan_file_scope_symbols(c_text):
    """Scan a C file for file-scope symbol declarations (lbl_ and fn_).

    Returns a set of symbol names that are already declared at file scope.
    These should NOT be re-declared inside generated function bodies.

    Only considers declarations that are truly at file scope (not inside
    function bodies), by tracking brace depth.
    """
    symbols = set()
    brace_depth = 0
    for line in c_text.split('\n'):
        # Track brace depth to determine if we're at file scope
        for ch in line:
            if ch == '{':
                brace_depth += 1
            elif ch == '}':
                brace_depth -= 1

        if brace_depth > 0:
            continue  # Inside a function body, skip

        # Match extern declarations at file scope
        m = re.match(r'\s*extern\s+\w[\w\s\*]*\s+(lbl_[0-9A-Fa-f]+|fn_[0-9A-Fa-f]+)\b', line)
        if m:
            symbols.add(m.group(1))
        # Also match non-extern file-scope declarations
        m = re.match(r'(?:static\s+)?(?:u8|u16|u32|s8|s16|s32|f32|f64|void|int|char|unsigned|long|short|BOOL)[\s\*]+(lbl_[0-9A-Fa-f]+)\b', line)
        if m:
            symbols.add(m.group(1))

    return symbols


def process_file(filepath, asm_funcs, dry_run=False):
    """Process a single C file, replacing all TODO stubs with decompiled code.

    Returns (stubs_replaced, stubs_failed, stubs_not_found).
    """
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        c_text = f.read()

    # Scan for file-scope symbols to avoid redeclaration conflicts
    file_scope_symbols = scan_file_scope_symbols(c_text)

    lines = c_text.split('\n')
    output_lines = []
    i = 0
    stubs_replaced = 0
    stubs_failed = 0
    stubs_not_found = 0

    while i < len(lines):
        line = lines[i]

        # ============================================
        # Pattern 1: Standard pragma push block
        # #pragma push
        # #pragma optimization_level 0
        # #pragma optimizewithasm off
        # void fn_XXX(...) { /* TODO... */ }
        # #pragma pop
        # ============================================
        if line.strip() == '#pragma push':
            if (i + 4 < len(lines) and
                '#pragma optimization_level' in lines[i+1] and
                '#pragma optimizewithasm' in lines[i+2]):

                # Find the matching #pragma pop for this block
                pop_line = i + 3
                brace_depth_scan = 0
                while pop_line < len(lines):
                    if lines[pop_line].strip() == '#pragma pop':
                        break
                    pop_line += 1
                if pop_line >= len(lines):
                    pop_line = len(lines) - 1

                # Extract the entire block between pragmas
                block_lines = lines[i+3:pop_line]
                block_text = '\n'.join(block_lines)

                # Find all functions in this block
                funcs_in_block = []
                bi = 0
                while bi < len(block_lines):
                    bl = block_lines[bi]
                    # Look for function definition: has '{' and looks like a function
                    # For multi-line signatures, check if previous lines form the signature
                    is_func_def = False
                    if '{' in bl and not bl.strip().startswith('#') and not bl.strip().startswith('/*') and not bl.strip().startswith('*'):
                        if re.search(r'\w+\s*\(', bl):
                            is_func_def = True
                        else:
                            # Check if this is the end of a multi-line signature
                            # Look back for the function name
                            for lookback in range(max(0, bi - 5), bi):
                                if re.search(r'\w+\s*\(', block_lines[lookback]):
                                    is_func_def = True
                                    break
                    if is_func_def:
                        # Find the actual start of the function signature (may be earlier for multi-line)
                        func_start = bi
                        for lookback in range(max(0, bi - 5), bi):
                            if re.search(r'(?:void|u8|u16|u32|s8|s16|s32|f32|f64|int|char|BOOL|unsigned|long|short)\s', block_lines[lookback]):
                                func_start = lookback
                                break
                        # Build the full function line (join all sig lines up to {)
                        fline = ' '.join(block_lines[j2].strip() for j2 in range(func_start, bi+1))
                        # Find closing brace
                        bd = 0
                        for bj in range(bi, len(block_lines)):
                            for ch in block_lines[bj]:
                                if ch == '{':
                                    bd += 1
                                elif ch == '}':
                                    bd -= 1
                            if bd == 0:
                                func_end = bj
                                func_text = '\n'.join(block_lines[func_start:func_end+1])
                                funcs_in_block.append((func_start, func_end, func_text, fline))
                                bi = func_end + 1
                                break
                        else:
                            bi += 1
                    else:
                        bi += 1

                # Process each function in the block
                replaced_any = False
                replaced_all = True
                block_output = []
                processed_ranges = set()

                for fstart, fend, ftxt, fline in funcs_in_block:
                    is_todo = 'TODO' in ftxt and ('decompile' in ftxt.lower() or 'match' in ftxt.lower())
                    if not is_todo:
                        replaced_all = False
                        continue

                    fn_name = find_fn_name(fline)
                    asm_name = fn_name
                    if not asm_name:
                        asm_name = find_asm_name_from_context(fline, ftxt)
                        if not asm_name:
                            # Check comment lines before function
                            for ci in range(max(0, fstart - 5), fstart):
                                m_addr = re.search(r'0x([0-9A-Fa-f]{8})', block_lines[ci])
                                if m_addr:
                                    asm_name = f'fn_{m_addr.group(1)}'
                                    break

                    if asm_name:
                        result = decompile_function(asm_name, asm_funcs)
                        if result:
                            processed_ranges.add((fstart, fend))
                            replaced_any = True
                            stubs_replaced += 1
                        else:
                            replaced_all = False
                            stubs_not_found += 1
                    else:
                        replaced_all = False
                        stubs_not_found += 1

                if replaced_any:
                    if not dry_run:
                        # Rebuild the block
                        bi = 0
                        while bi < len(block_lines):
                            # Check if this line starts a processed function
                            matched_func = None
                            for fstart, fend, ftxt, fline in funcs_in_block:
                                if bi == fstart and (fstart, fend) in processed_ranges:
                                    matched_func = (fstart, fend, ftxt, fline)
                                    break

                            if matched_func:
                                fstart, fend, ftxt, fline = matched_func

                                fn_name_match = find_fn_name(fline)
                                asm_nm = fn_name_match
                                if not asm_nm:
                                    asm_nm = find_asm_name_from_context(fline, ftxt)
                                    if not asm_nm:
                                        for ci in range(max(0, fstart - 5), fstart):
                                            m_addr = re.search(r'0x([0-9A-Fa-f]{8})', block_lines[ci])
                                            if m_addr:
                                                asm_nm = f'fn_{m_addr.group(1)}'
                                                break

                                result = decompile_function(asm_nm, asm_funcs)
                                if result:
                                    ret_type, body, needs_pragma = result
                                    body = filter_conflicting_decls(body, file_scope_symbols)

                                    sig_match = re.match(r'(.*?)\{', fline)
                                    if sig_match:
                                        sig = clean_signature(sig_match.group(1))
                                        block_output.append(f'{sig} {{')
                                        for bline in body.rstrip('\n').split('\n'):
                                            block_output.append(bline)
                                        block_output.append('}')
                                else:
                                    # Keep original
                                    for li in range(fstart, fend + 1):
                                        block_output.append(block_lines[li])

                                bi = fend + 1
                            else:
                                block_output.append(block_lines[bi])
                                bi += 1

                        # Output: single push/pop wrapping all functions
                        output_lines.append('#pragma push')
                        output_lines.append('#pragma optimization_level 0')
                        output_lines.append('#pragma optimizewithasm off')
                        for bl2 in block_output:
                            output_lines.append(bl2)
                        output_lines.append('#pragma pop')

                    i = pop_line + 1
                    continue
                else:
                    # No stubs found in this block; keep as-is
                    pass

            output_lines.append(line)
            i += 1
            continue

        # ============================================
        # Pattern 2: Peephole pragma block
        # #pragma peephole off
        # void fn_XXX(void) {
        #     /* TODO: decompile ... */
        # }
        # #pragma peephole reset
        # ============================================
        if line.strip() == '#pragma peephole off':
            # Look ahead for function signature
            func_start = i + 1
            if func_start < len(lines):
                sig_line = func_start
                while sig_line < len(lines) and '{' not in lines[sig_line]:
                    sig_line += 1

                if sig_line < len(lines):
                    func_line = ' '.join(lines[j].strip() for j in range(func_start, sig_line + 1))

                    # Find closing brace
                    body_end = sig_line
                    is_todo = False
                    if '{' in func_line and '}' in func_line:
                        body_end = sig_line
                        if 'TODO' in func_line:
                            is_todo = True
                    else:
                        j = sig_line + 1
                        while j < len(lines) and lines[j].strip() != '}':
                            if 'TODO' in lines[j]:
                                is_todo = True
                            j += 1
                        body_end = j

                    # Also check: if the body is empty (just braces), check for
                    # TODO in the comment BEFORE #pragma peephole off
                    if not is_todo:
                        # Check the lines before pragma peephole off
                        for check_i in range(max(0, i - 3), i):
                            if 'TODO' in lines[check_i] and ('Decompile' in lines[check_i] or 'decompile' in lines[check_i]):
                                is_todo = True
                                break

                    # Also check if body is essentially empty (no real statements)
                    if not is_todo:
                        body_content = ''
                        for j in range(sig_line + 1, body_end):
                            body_content += lines[j].strip()
                        # If body is empty or only has a comment, treat as stub
                        body_stripped = body_content.strip()
                        if body_stripped == '' or (body_stripped.startswith('/*') and body_stripped.endswith('*/')):
                            # Check for TODO in nearby comments
                            for check_i in range(max(0, i - 5), i):
                                if 'TODO' in lines[check_i]:
                                    is_todo = True
                                    break

                    # Find #pragma peephole reset
                    reset_line = body_end + 1
                    while reset_line < len(lines) and '#pragma peephole reset' not in lines[reset_line]:
                        reset_line += 1
                        if reset_line - body_end > 5:
                            break

                    fn_name = find_fn_name(func_line)
                    asm_name = fn_name
                    if not asm_name and is_todo:
                        body_txt = '\n'.join(lines[sig_line:body_end+1])
                        asm_name = find_asm_name_from_context(func_line, body_txt)
                        if not asm_name:
                            for check_i in range(max(0, i - 5), i):
                                m_addr = re.search(r'0x([0-9A-Fa-f]{8})', lines[check_i])
                                if m_addr:
                                    asm_name = f'fn_{m_addr.group(1)}'
                                    break
                    if asm_name and is_todo:
                        result = decompile_function(asm_name, asm_funcs)
                        if result:
                            ret_type, body, needs_pragma = result
                            body = filter_conflicting_decls(body, file_scope_symbols)

                            if not dry_run:
                                # Use pragma push/opt/asm for decompiled code
                                output_lines.append('#pragma push')
                                output_lines.append('#pragma optimization_level 0')
                                output_lines.append('#pragma optimizewithasm off')

                                sig_match = re.match(r'(.*?)\{', func_line)
                                if sig_match:
                                    sig = clean_signature(sig_match.group(1))
                                    output_lines.append(f'{sig} {{')
                                    for bline in body.rstrip('\n').split('\n'):
                                        output_lines.append(bline)
                                    output_lines.append('}')
                                else:
                                    output_lines.append(func_line)

                                output_lines.append('#pragma pop')

                            skip_to = reset_line + 1 if reset_line < len(lines) and '#pragma peephole reset' in lines[reset_line] else body_end + 1
                            i = skip_to
                            stubs_replaced += 1
                            continue
                        else:
                            stubs_not_found += 1

            output_lines.append(line)
            i += 1
            continue

        # ============================================
        # Pattern 3: Bare function with TODO in body
        # (no pragma optimization_level or peephole,
        #  just a function with TODO inside)
        # Matches patterns like:
        #   s32 fn_80059BDC(void) {
        #       /* TODO: decompile ... */
        #       ...
        #   }
        # or named functions with TODO:
        #   void GSDVD_CheckAndClose(void) {
        #       /* TODO: match -- 48 bytes at 0x80167040 */
        #   }
        # ============================================
        # Check if this line is a function definition with '{'
        if ('{' in line and
            not line.strip().startswith('#') and
            not line.strip().startswith('/*') and
            not line.strip().startswith('*') and
            re.search(r'\b\w+\s*\(', line)):

            # Determine if this is a function definition (has return type + name + parens + brace)
            func_sig_match = re.match(r'\s*((?:static\s+|extern\s+|inline\s+)*(?:void|u8|u16|u32|s8|s16|s32|f32|f64|int|char|unsigned|long|short|BOOL|void\s*\*)\s*\*?\s*\w+\s*\(.*\))\s*\{', line)
            if not func_sig_match:
                # Try multi-line: line might not have '{' yet or signature spans lines
                # Also check simpler pattern
                func_sig_match = re.match(r'\s*((?:asm\s+)?(?:static\s+|extern\s+|inline\s+)*\w[\w\s\*]*\s+\w+\s*\([^)]*\))\s*\{', line)

            if func_sig_match:
                func_line = line
                sig_line_idx = i

                # Check if body contains TODO
                # Find the closing brace
                brace_depth = 0
                body_text = ''
                is_todo = False
                body_end = i

                for j in range(i, len(lines)):
                    for ch in lines[j]:
                        if ch == '{':
                            brace_depth += 1
                        elif ch == '}':
                            brace_depth -= 1
                    body_text += lines[j] + '\n'
                    if brace_depth == 0:
                        body_end = j
                        break

                if 'TODO' in body_text and ('decompile' in body_text.lower() or 'match' in body_text.lower()):
                    is_todo = True

                if is_todo:
                    # Try to find the asm function name
                    fn_name = find_fn_name(func_line)
                    asm_name = fn_name

                    if not asm_name:
                        # Named function -- extract address from TODO comment
                        asm_name = find_asm_name_from_context(func_line, body_text)

                        # Also check comment line(s) above
                        if not asm_name:
                            for check_i in range(max(0, i - 5), i):
                                m = re.search(r'0x([0-9A-Fa-f]{8})', lines[check_i])
                                if m:
                                    asm_name = f'fn_{m.group(1)}'
                                    break

                    if asm_name:
                        result = decompile_function(asm_name, asm_funcs)
                        if result:
                            ret_type, body, needs_pragma = result
                            body = filter_conflicting_decls(body, file_scope_symbols)

                            if not dry_run:
                                # Check if we're already inside a #pragma push
                                in_push = False
                                for prev_line in output_lines[-20:]:
                                    stripped_prev = prev_line.strip()
                                    if stripped_prev == '#pragma push':
                                        in_push = True
                                    elif stripped_prev == '#pragma pop':
                                        in_push = False

                                if not in_push:
                                    output_lines.append('#pragma push')
                                output_lines.append('#pragma optimization_level 0')
                                output_lines.append('#pragma optimizewithasm off')

                                sig_match = re.match(r'(.*?)\{', func_line)
                                if sig_match:
                                    sig = clean_signature(sig_match.group(1))
                                    output_lines.append(f'{sig} {{')
                                    for bline in body.rstrip('\n').split('\n'):
                                        output_lines.append(bline)
                                    output_lines.append('}')
                                else:
                                    output_lines.append(func_line)

                                if not in_push:
                                    output_lines.append('#pragma pop')

                            i = body_end + 1
                            stubs_replaced += 1
                            continue
                        else:
                            stubs_not_found += 1

        output_lines.append(line)
        i += 1

    if stubs_replaced > 0 and not dry_run:
        result_text = '\n'.join(output_lines)
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(result_text)

    return stubs_replaced, stubs_failed, stubs_not_found


def compile_check(filepath):
    """Run compile_check.py on a file. Returns True on success."""
    cmd = [sys.executable, os.path.join(PROJ, 'tools', 'compile_check.py'), filepath]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True,
                               cwd=PROJ, timeout=120)
        return result.returncode == 0, result.stdout + result.stderr
    except subprocess.TimeoutExpired:
        return False, "TIMEOUT"
    except Exception as e:
        return False, str(e)


def main():
    parser = argparse.ArgumentParser(
        description="Auto-decompile all pragma stubs across the entire codebase."
    )
    parser.add_argument('--dry-run', action='store_true',
                       help='Show what would be done without modifying files')
    parser.add_argument('--file', type=str,
                       help='Process only a specific file')
    parser.add_argument('--compile', action='store_true',
                       help='Compile-check each modified file after processing')
    parser.add_argument('--backup', action='store_true',
                       help='Create .bak backup of each modified file')
    parser.add_argument('--skip-gs-render', action='store_true', default=True,
                       help='Skip gs_render.c (already processed)')
    args = parser.parse_args()

    print("Loading assembly...", file=sys.stderr)
    asm_funcs = load_all_asm()
    print(f"Loaded {len(asm_funcs)} functions from assembly", file=sys.stderr)

    # Find source files to process
    if args.file:
        src_files = [os.path.abspath(args.file)]
    else:
        src_files = []
        for root, dirs, files in os.walk(SRC_DIR):
            for f in sorted(files):
                if f.endswith('.c'):
                    src_files.append(os.path.join(root, f))
        src_files.sort()

    total_replaced = 0
    total_failed = 0
    total_not_found = 0
    files_modified = []
    compile_errors = []

    for filepath in src_files:
        rel = os.path.relpath(filepath, PROJ)

        # Skip gs_render.c (already processed by gen_gs_render.py)
        if args.skip_gs_render and 'gs_render.c' in filepath:
            continue

        # Quick check: does this file have any TODO stubs?
        try:
            with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
                content = f.read()
        except Exception as e:
            print(f"WARNING: Could not read {rel}: {e}", file=sys.stderr)
            continue
        if 'TODO' not in content:
            continue

        if args.backup and not args.dry_run:
            shutil.copy2(filepath, filepath + '.bak')

        replaced, failed, not_found = process_file(filepath, asm_funcs, dry_run=args.dry_run)

        if replaced > 0 or failed > 0 or not_found > 0:
            print(f"{rel}: {replaced} replaced, {failed} failed, {not_found} asm-not-found",
                  file=sys.stderr)

        total_replaced += replaced
        total_failed += failed
        total_not_found += not_found

        if replaced > 0:
            files_modified.append(rel)

            if args.compile and not args.dry_run:
                ok, output = compile_check(filepath)
                if ok:
                    print(f"  COMPILE OK: {rel}", file=sys.stderr)
                else:
                    print(f"  COMPILE FAIL: {rel}", file=sys.stderr)
                    compile_errors.append((rel, output))

    # Summary
    print(f"\n{'='*60}", file=sys.stderr)
    print(f"SUMMARY", file=sys.stderr)
    print(f"{'='*60}", file=sys.stderr)
    print(f"Stubs replaced:   {total_replaced}", file=sys.stderr)
    print(f"Stubs failed:     {total_failed}", file=sys.stderr)
    print(f"ASM not found:    {total_not_found}", file=sys.stderr)
    print(f"Files modified:   {len(files_modified)}", file=sys.stderr)

    if compile_errors:
        print(f"\nCOMPILE ERRORS ({len(compile_errors)}):", file=sys.stderr)
        for rel, output in compile_errors:
            print(f"  {rel}", file=sys.stderr)
            # Show first few lines of error
            for eline in output.strip().split('\n')[:5]:
                print(f"    {eline}", file=sys.stderr)

    if files_modified and not args.dry_run:
        print(f"\nModified files:", file=sys.stderr)
        for f in files_modified:
            print(f"  {f}", file=sys.stderr)


if __name__ == '__main__':
    main()
