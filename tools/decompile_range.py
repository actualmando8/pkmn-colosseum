#!/usr/bin/env python3
"""
Generate C function stubs for all uncovered functions in range 0x800F8268-0x80138000.
Applies pattern matching for accessors and generates asm includes for complex functions.
"""

import re
import os
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).resolve().parent.parent
ASM_FILE = PROJECT_ROOT / "build" / "GC6E01" / "asm" / "auto_01_800055E0_text.s"
SYMBOLS_FILE = PROJECT_ROOT / "config" / "GC6E01" / "symbols.txt"
LINK_ORDER = PROJECT_ROOT / "config" / "GC6E01" / "link_order.txt"
SRC_DIR = PROJECT_ROOT / "src"

RANGE_LO = 0x800F8268
RANGE_HI = 0x80138000


def parse_asm():
    functions = {}
    current_fn = None
    current_instrs = []
    current_addr = 0
    current_size = 0
    current_scope = "global"

    fn_start_re = re.compile(r'^\.fn\s+(\w+)(?:,\s*(\w+))?')
    fn_end_re = re.compile(r'^\.endfn\s+(\w+)')
    comment_re = re.compile(r'^# \.text:.*\|\s*(0x[0-9A-Fa-f]+)\s*\|\s*size:\s*(0x[0-9A-Fa-f]+)')
    instr_re = re.compile(r'/\*\s*([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f ]+\*/\s+(.+)')

    pending_addr = 0
    pending_size = 0

    with open(ASM_FILE, 'r') as f:
        for line in f:
            line = line.rstrip()
            m = comment_re.match(line)
            if m:
                pending_addr = int(m.group(1), 16)
                pending_size = int(m.group(2), 16)
                continue
            m = fn_start_re.match(line)
            if m:
                if current_fn:
                    functions[current_fn] = (current_addr, current_size, current_scope, current_instrs)
                current_fn = m.group(1)
                current_scope = m.group(2) or "global"
                current_addr = pending_addr
                current_size = pending_size
                current_instrs = []
                continue
            m = fn_end_re.match(line)
            if m:
                if current_fn:
                    functions[current_fn] = (current_addr, current_size, current_scope, current_instrs)
                current_fn = None
                current_instrs = []
                continue
            if current_fn:
                m = instr_re.match(line.strip())
                if m:
                    rest = m.group(2).strip()
                    parts = rest.split(None, 1)
                    mnem = parts[0]
                    ops = parts[1] if len(parts) > 1 else ''
                    current_instrs.append((mnem, ops))
    return functions


def parse_symbols():
    funcs = []
    with open(SYMBOLS_FILE) as f:
        for line in f:
            m = re.match(r'(\w+)\s*=\s*\.\w+:0x([0-9A-Fa-f]+);\s*//\s*type:function\s+size:0x([0-9A-Fa-f]+)', line.strip())
            if m:
                addr = int(m.group(2), 16)
                if RANGE_LO <= addr < RANGE_HI:
                    funcs.append((m.group(1), addr, int(m.group(3), 16)))
    return funcs


def parse_link_order():
    ranges = []
    with open(LINK_ORDER, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('#') or not line:
                continue
            m = re.match(r'(src/\S+\.c)\s+(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)', line)
            if m:
                ranges.append((m.group(1), int(m.group(2), 16), int(m.group(3), 16)))
    return sorted(ranges, key=lambda x: x[1])


def find_tu(addr, ranges):
    for src, lo, hi in ranges:
        if lo <= addr < hi:
            return src
    return None


def find_existing():
    defined = set()
    fn_def_re = re.compile(
        r'(?:^|\n)\s*(?:void|u8|u16|u32|s8|s16|s32|f32|f64|int|BOOL|char\*?|'
        r'static\s+\w+\*?|struct\s+\w+\*?|[A-Z]\w+\*?|asm\s+void)\s+'
        r'(fn_([0-9A-Fa-f]{8}))\s*\('
    )
    for root, dirs, files in os.walk(SRC_DIR):
        for fname in files:
            if fname.endswith('.c'):
                fpath = os.path.join(root, fname)
                with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
                    content = f.read()
                for m in fn_def_re.finditer(content):
                    addr = int(m.group(2), 16)
                    if RANGE_LO <= addr < RANGE_HI:
                        defined.add(m.group(1))
    return defined


# ============================================================================
# Pattern matching helpers
# ============================================================================

def parse_load_offset(ops):
    m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))\(r(\d+)\)', ops)
    if m:
        val_str = m.group(2)
        if val_str.startswith('-0x'):
            val = -int(val_str[1:], 16)
        elif val_str.startswith('0x'):
            val = int(val_str, 16)
        elif val_str.startswith('-'):
            val = int(val_str)
        else:
            val = int(val_str)
        return (int(m.group(1)), val, int(m.group(3)))
    return None

def parse_float_offset(ops):
    m = re.match(r'f(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))\(r(\d+)\)', ops)
    if m:
        val_str = m.group(2)
        if val_str.startswith('-0x'):
            val = -int(val_str[1:], 16)
        elif val_str.startswith('0x'):
            val = int(val_str, 16)
        elif val_str.startswith('-'):
            val = int(val_str)
        else:
            val = int(val_str)
        return (int(m.group(1)), val, int(m.group(3)))
    return None

def parse_sda(ops):
    m = re.match(r'(r|f)(\d+),\s*(\w+)@sda21\(r(\d+)\)', ops)
    if m:
        return (m.group(1), int(m.group(2)), m.group(3), int(m.group(4)))
    return None

def load_ctype(mnem):
    return {'lwz':'u32','lhz':'u16','lbz':'u8','lha':'s16','lfs':'f32','lfd':'f64'}.get(mnem)

def store_ctype(mnem):
    return {'stw':'u32','sth':'u16','stb':'u8','stfs':'f32','stfd':'f64'}.get(mnem)

def is_load(m):
    return m in ('lwz','lhz','lbz','lha','lfs','lfd')

def is_store(m):
    return m in ('stw','sth','stb','stfs','stfd')

def is_float_load(m):
    return m in ('lfs','lfd')

def is_float_store(m):
    return m in ('stfs','stfd')

def fmt_off(v):
    if v < 0:
        return f"-0x{abs(v):X}"
    return f"0x{v:X}"


# ============================================================================
# Pattern matching
# ============================================================================

def try_match(name, instrs, size):
    n = len(instrs)
    if n < 1:
        return None
    if instrs[-1][0] != 'blr':
        return None

    # void stub
    if n == 1 and size == 0x4:
        return ("void_stub", f"void {name}(void) {{\n}}\n")

    # return constant
    if n == 2 and size == 0x8 and instrs[0][0] == 'li':
        m = re.match(r'r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[0][1])
        if m:
            val_str = m.group(1)
            if val_str.startswith('-0x'):
                val = -int(val_str[1:], 16)
            elif val_str.startswith('0x'):
                val = int(val_str, 16)
            elif val_str.startswith('-'):
                val = int(val_str)
            else:
                val = int(val_str)
            if val < 0:
                return ("return_const", f"s32 {name}(void) {{ return {val}; }}\n")
            else:
                return ("return_const", f"u32 {name}(void) {{ return {val}; }}\n")

    # addi return pointer
    if n == 2 and size == 0x8 and instrs[0][0] == 'addi':
        m = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[0][1])
        if m:
            val_str = m.group(1)
            if val_str.startswith('-0x'):
                off = -int(val_str[1:], 16)
            elif val_str.startswith('0x'):
                off = int(val_str, 16)
            else:
                off = int(val_str)
            return ("addi_ptr", f"void* {name}(void* obj) {{ return (u8*)obj + {fmt_off(off)}; }}\n")

    # mr r3, r4; blr (pass-through)
    if n == 2 and size == 0x8 and instrs[0][0] == 'mr':
        m = re.match(r'r3,\s*r(\d+)', instrs[0][1])
        if m:
            src_reg = int(m.group(1))
            if src_reg == 4:
                return ("mr_passthrough", f"u32 {name}(u32 a, u32 b) {{ return b; }}\n")

    # simple getter
    if n == 2 and size == 0x8:
        mnem = instrs[0][0]
        if is_load(mnem) and not is_float_load(mnem):
            p = parse_load_offset(instrs[0][1])
            if p and p[0] == 3 and p[2] == 3:
                ct = load_ctype(mnem)
                return ("simple_getter", f"{ct} {name}(void* obj) {{ return *({ct}*)((u8*)obj + {fmt_off(p[1])}); }}\n")
        if is_float_load(mnem):
            p = parse_float_offset(instrs[0][1])
            if p and p[0] == 1 and p[2] == 3:
                ct = load_ctype(mnem)
                return ("simple_getter_f", f"{ct} {name}(void* obj) {{ return *({ct}*)((u8*)obj + {fmt_off(p[1])}); }}\n")

    # simple setter
    if n == 2 and size == 0x8:
        mnem = instrs[0][0]
        if is_store(mnem) and not is_float_store(mnem):
            p = parse_load_offset(instrs[0][1])
            if p and p[0] == 4 and p[2] == 3:
                ct = store_ctype(mnem)
                return ("simple_setter", f"void {name}(void* obj, {ct} val) {{ *({ct}*)((u8*)obj + {fmt_off(p[1])}) = val; }}\n")
        if is_float_store(mnem):
            p = parse_float_offset(instrs[0][1])
            if p and p[0] == 1 and p[2] == 3:
                ct = store_ctype(mnem)
                return ("simple_setter_f", f"void {name}(void* obj, {ct} val) {{ *({ct}*)((u8*)obj + {fmt_off(p[1])}) = val; }}\n")

    # SDA getter
    if n == 2 and size == 0x8:
        mnem = instrs[0][0]
        if is_load(mnem) or is_float_load(mnem):
            p = parse_sda(instrs[0][1])
            if p:
                rf, rn, sym, base = p
                if (rf == 'r' and rn == 3) or (rf == 'f' and rn == 1):
                    ct = load_ctype(mnem)
                    return ("sda_getter", f"{ct} {name}(void) {{ return {sym}; }}\n")

    # SDA setter
    if n == 2 and size == 0x8:
        mnem = instrs[0][0]
        if is_store(mnem) or is_float_store(mnem):
            p = parse_sda(instrs[0][1])
            if p:
                rf, rn, sym, base = p
                if (rf == 'r' and rn == 3) or (rf == 'f' and rn == 1):
                    ct = store_ctype(mnem)
                    return ("sda_setter", f"void {name}({ct} val) {{ {sym} = val; }}\n")

    # addis getter (3 instrs, 0xC)
    if n == 3 and size == 0xC and instrs[0][0] == 'addis':
        m = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[0][1])
        if m:
            hi = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
            hi <<= 16
            mnem = instrs[1][0]
            if is_load(mnem) and not is_float_load(mnem):
                p = parse_load_offset(instrs[1][1])
                if p and p[0] == 3 and p[2] == 3:
                    actual = hi + p[1]
                    ct = load_ctype(mnem)
                    return ("addis_getter", f"{ct} {name}(void* obj) {{ return *({ct}*)((u8*)obj + {fmt_off(actual)}); }}\n")

    # addis setter (3 instrs, 0xC)
    if n == 3 and size == 0xC and instrs[0][0] == 'addis':
        m = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[0][1])
        if m:
            hi = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
            hi <<= 16
            mnem = instrs[1][0]
            if is_store(mnem) and not is_float_store(mnem):
                p = parse_load_offset(instrs[1][1])
                if p and p[0] == 4 and p[2] == 3:
                    actual = hi + p[1]
                    ct = store_ctype(mnem)
                    return ("addis_setter", f"void {name}(void* obj, {ct} val) {{ *({ct}*)((u8*)obj + {fmt_off(actual)}) = val; }}\n")

    # null-check bne getter (cmplwi r3,0; bne; li r3,0; blr; load r3,OFF(r3); blr)
    if n >= 5 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if instrs[1][0] == 'bne' and instrs[2][0] == 'li' and instrs[3][0] == 'blr':
            m_li = re.match(r'r3,\s*0', instrs[2][1])
            if m_li:
                mnem4 = instrs[4][0]
                if is_load(mnem4) and not is_float_load(mnem4):
                    p = parse_load_offset(instrs[4][1])
                    if p and p[0] == 3 and p[2] == 3:
                        ct = load_ctype(mnem4)
                        return ("nc_getter", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(p[1])});\n}}\n")
                if is_float_load(mnem4):
                    p = parse_float_offset(instrs[4][1])
                    if p and p[0] == 1 and p[2] == 3:
                        ct = load_ctype(mnem4)
                        return ("nc_getter_f", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(p[1])});\n}}\n")
                # lbz r0, OFF(r3); extsb r3, r0
                if mnem4 == 'lbz' and n >= 7:
                    p = parse_load_offset(instrs[4][1])
                    if p and p[0] == 0 and p[2] == 3 and instrs[5][0] == 'extsb':
                        return ("nc_getter_s8", f"s32 {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return (s8)*((u8*)ptr + {fmt_off(p[1])});\n}}\n")
                # lwz r3, OFF(r3); lwz r3, OFF2(r3); blr (double-deref)
                if n >= 6 and mnem4 == 'lwz':
                    p = parse_load_offset(instrs[4][1])
                    if p and p[0] == 3 and p[2] == 3:
                        mnem5 = instrs[5][0]
                        if is_load(mnem5) and not is_float_load(mnem5):
                            p2 = parse_load_offset(instrs[5][1])
                            if p2 and p2[0] == 3 and p2[2] == 3:
                                ct = load_ctype(mnem5)
                                return ("nc_double_deref", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    void* tmp = *(void**)((u8*)ptr + {fmt_off(p[1])});\n    return *({ct}*)((u8*)tmp + {fmt_off(p2[1])});\n}}\n")

    # null-check beqlr getter (cmplwi r3,0; beqlr; load r3, OFF(r3); blr)
    if n == 4 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if instrs[1][0] == 'beqlr':
            mnem = instrs[2][0]
            if is_load(mnem) and not is_float_load(mnem):
                p = parse_load_offset(instrs[2][1])
                if p and p[0] == 3 and p[2] == 3:
                    ct = load_ctype(mnem)
                    return ("nc_beqlr_getter", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(p[1])});\n}}\n")

    # null-check setter (cmplwi r3,0; beqlr; store r4, OFF(r3); blr)
    if n == 4 and size == 0x10:
        if instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
            if instrs[1][0] == 'beqlr':
                mnem = instrs[2][0]
                if is_store(mnem) and not is_float_store(mnem):
                    p = parse_load_offset(instrs[2][1])
                    if p and p[0] == 4 and p[2] == 3:
                        ct = store_ctype(mnem)
                        return ("nc_setter", f"void {name}(void* ptr, {ct} val) {{\n    if (ptr == NULL) {{ return; }}\n    *({ct}*)((u8*)ptr + {fmt_off(p[1])}) = val;\n}}\n")
                if is_float_store(mnem):
                    p = parse_float_offset(instrs[2][1])
                    if p and p[0] == 1 and p[2] == 3:
                        ct = store_ctype(mnem)
                        return ("nc_setter_f", f"void {name}(void* ptr, {ct} val) {{\n    if (ptr == NULL) {{ return; }}\n    *({ct}*)((u8*)ptr + {fmt_off(p[1])}) = val;\n}}\n")

    # null-check addis getter (cmplwi r3,0; beqlr; addis r3,r3; load r3,OFF(r3); blr)
    if n == 5 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if instrs[1][0] == 'beqlr' and instrs[2][0] == 'addis':
            m = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[2][1])
            if m:
                hi = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
                hi <<= 16
                mnem = instrs[3][0]
                if is_load(mnem) and not is_float_load(mnem):
                    p = parse_load_offset(instrs[3][1])
                    if p and p[0] == 3 and p[2] == 3:
                        actual = hi + p[1]
                        ct = load_ctype(mnem)
                        return ("nc_addis_getter", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(actual)});\n}}\n")

    # null-check addis setter
    if n == 5 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if instrs[1][0] == 'beqlr' and instrs[2][0] == 'addis':
            m = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[2][1])
            if m:
                hi = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
                hi <<= 16
                mnem = instrs[3][0]
                if is_store(mnem) and not is_float_store(mnem):
                    p = parse_load_offset(instrs[3][1])
                    if p and p[0] == 4 and p[2] == 3:
                        actual = hi + p[1]
                        ct = store_ctype(mnem)
                        return ("nc_addis_setter", f"void {name}(void* ptr, {ct} val) {{\n    if (ptr == NULL) {{ return; }}\n    *({ct}*)((u8*)ptr + {fmt_off(actual)}) = val;\n}}\n")

    # null-check bne + addis getter (7 instrs)
    if n == 7 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if (instrs[1][0] == 'bne' and instrs[2][0] == 'li' and
            instrs[3][0] == 'blr' and instrs[4][0] == 'addis'):
            m = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[4][1])
            if m:
                hi = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
                hi <<= 16
                mnem = instrs[5][0]
                if is_load(mnem) and not is_float_load(mnem):
                    p = parse_load_offset(instrs[5][1])
                    if p and p[0] == 3 and p[2] == 3:
                        actual = hi + p[1]
                        ct = load_ctype(mnem)
                        return ("nc_bne_addis", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(actual)});\n}}\n")

    # SDA load + deref pattern (3 instrs: lwz r3, sym@sda21(r13); load r3, OFF(r3); blr)
    if n == 3 and size == 0xC:
        mnem0 = instrs[0][0]
        if mnem0 == 'lwz':
            p0 = parse_sda(instrs[0][1])
            if p0 and p0[0] == 'r' and p0[1] == 3:
                sym = p0[2]
                mnem1 = instrs[1][0]
                if is_load(mnem1) and not is_float_load(mnem1):
                    p1 = parse_load_offset(instrs[1][1])
                    if p1 and p1[0] == 3 and p1[2] == 3:
                        ct = load_ctype(mnem1)
                        return ("sda_deref_getter", f"{ct} {name}(void) {{\n    return *({ct}*)((u8*){sym} + {fmt_off(p1[1])});\n}}\n")

    # SDA store pattern (3 instrs: lwz rX, sym@sda21(r13); store r3, OFF(rX); blr)
    if n == 3 and size == 0xC:
        mnem0 = instrs[0][0]
        if mnem0 == 'lwz':
            p0 = parse_sda(instrs[0][1])
            if p0 and p0[0] == 'r':
                base_reg = p0[1]
                sym = p0[2]
                mnem1 = instrs[1][0]
                if is_store(mnem1) and not is_float_store(mnem1):
                    p1 = parse_load_offset(instrs[1][1])
                    if p1 and p1[2] == base_reg:
                        ct = store_ctype(mnem1)
                        src_reg = p1[0]
                        if src_reg == 3:
                            return ("sda_deref_setter", f"void {name}({ct} val) {{\n    *({ct}*)((u8*){sym} + {fmt_off(p1[1])}) = val;\n}}\n")

    return None


def analyze_function_calls(name, instrs):
    """Extract function calls from instructions."""
    calls = []
    for mnem, ops in instrs:
        if mnem == 'bl':
            target = ops.strip()
            calls.append(target)
    return calls


def analyze_params(instrs):
    """Analyze how many parameters a function likely takes."""
    # Look at register usage in early instructions
    reads_r4 = False
    reads_r5 = False
    reads_r6 = False
    reads_f1 = False

    for mnem, ops in instrs[:20]:
        if 'r4' in ops and mnem not in ('stw', 'sth', 'stb', 'mr', 'li', 'lis'):
            reads_r4 = True
        if 'r5' in ops and mnem not in ('stw', 'sth', 'stb', 'mr', 'li', 'lis'):
            reads_r5 = True
        if 'r6' in ops and mnem not in ('stw', 'sth', 'stb', 'mr', 'li', 'lis'):
            reads_r6 = True
        if 'f1' in ops:
            reads_f1 = True

    return reads_r4, reads_r5, reads_r6, reads_f1


def gen_asm_stub(name, addr, size, instrs):
    """Generate an asm-include stub for a complex function."""
    lines = []
    lines.append(f"/* {name} - 0x{addr:08X} | 0x{size:X} bytes */")

    # Determine if function returns void or value
    # Check last few instructions before blr
    returns_value = False
    for mnem, ops in instrs[-5:]:
        if mnem in ('li', 'lwz', 'lhz', 'lbz', 'lha', 'mr') and 'r3' in ops:
            returns_value = True
        if mnem in ('lfs', 'lfd') and 'f1' in ops:
            returns_value = True

    ret_type = "void"

    lines.append(f"#pragma push")
    lines.append(f"#pragma optimization_level 0")
    lines.append(f"#pragma optimizewithasm off")
    lines.append(f"asm {ret_type} {name}(void) {{")
    lines.append(f"    nofralloc")
    lines.append(f"    #include \"asm/{name}.s\"")
    lines.append(f"}}")
    lines.append(f"#pragma pop")
    lines.append(f"")
    return "\n".join(lines)


def main():
    print("Parsing assembly...")
    asm_funcs = parse_asm()
    print(f"  {len(asm_funcs)} functions parsed from assembly")

    print("Parsing symbols...")
    symbols = parse_symbols()
    print(f"  {len(symbols)} functions in range 0x{RANGE_LO:08X}-0x{RANGE_HI:08X}")

    print("Parsing link order...")
    link_ranges = parse_link_order()

    print("Finding existing definitions...")
    existing = find_existing()
    print(f"  {len(existing)} already defined")

    uncovered = [(name, addr, size) for name, addr, size in symbols if name not in existing]
    print(f"  {len(uncovered)} uncovered functions to generate")

    # Pattern match
    matched = {}
    unmatched = []
    pattern_counts = defaultdict(int)
    sda_externs = defaultdict(set)  # sym -> set of ctypes used

    for name, addr, size in uncovered:
        if name in asm_funcs:
            _, _, _, instrs = asm_funcs[name]
            result = try_match(name, instrs, size)
            if result:
                pattern, code = result
                matched[name] = (addr, size, pattern, code)
                pattern_counts[pattern] += 1
                # Track SDA externs
                if 'sda' in pattern:
                    for sda_m in re.finditer(r'return (\w+);|(\w+) = val;', code):
                        sym = sda_m.group(1) or sda_m.group(2)
                        if sym and not sym.startswith('fn_'):
                            # Get type from code
                            type_m = re.match(r'(\w+)\s+' + re.escape(name), code)
                            if type_m:
                                sda_externs[sym].add(type_m.group(1))
            else:
                unmatched.append((name, addr, size))
        else:
            unmatched.append((name, addr, size))

    print(f"\nPattern matches: {len(matched)}")
    for p, c in sorted(pattern_counts.items(), key=lambda x: -x[1]):
        print(f"  {p:25s}: {c}")
    print(f"Unmatched (asm stubs): {len(unmatched)}")

    # Group by TU
    tu_groups = defaultdict(list)
    for name, addr, size in uncovered:
        tu = find_tu(addr, link_ranges)
        if tu is None:
            if 0x800FEBA0 <= addr < 0x800FF0A0:
                tu = "src/game/gs_task_util.c"
            elif 0x8013151C <= addr < 0x80137114:
                tu = "src/game/effect/effect_util.c"
            elif 0x801380D4 <= addr < 0x80138000:
                tu = "src/game/effect/tracefx.c"
            else:
                tu = "src/game/gs_misc_stubs.c"
        tu_groups[tu].append((name, addr, size))

    # Process each TU
    total_written = 0
    for tu, funcs in sorted(tu_groups.items(), key=lambda x: x[1][0][1]):
        funcs.sort(key=lambda x: x[1])
        src_path = PROJECT_ROOT / tu

        print(f"\n{'='*60}")
        print(f"{tu}: {len(funcs)} functions")
        print(f"{'='*60}")

        # Check existing content
        existing_content = ""
        file_exists = src_path.exists()
        if file_exists:
            with open(src_path, 'r', encoding='utf-8', errors='replace') as f:
                existing_content = f.read()

        already_in_file = set()
        for fm in re.finditer(r'(?:void|u8|u16|u32|s8|s16|s32|f32|f64|int|BOOL|char\*?|asm\s+void|asm\s+u32|asm\s+s32)\s+\*?\s*(fn_[0-9A-Fa-f]{8})\s*\(', existing_content):
            already_in_file.add(fm.group(1))

        new_code_parts = []
        needed_externs = set()
        new_count = 0
        matched_count = 0
        stub_count = 0

        for name, addr, size in funcs:
            if name in already_in_file:
                continue

            if name in matched:
                _, _, pattern, code = matched[name]
                # Collect SDA externs from code
                for ext_m in re.finditer(r'return (\w+@?\w*);|(\w+@?\w*) = val;', code):
                    sym = ext_m.group(1) or ext_m.group(2)
                    if sym and not sym.startswith('fn_') and not sym.startswith('0') and sym not in ('val', 'NULL', 'obj', 'ptr', 'tmp'):
                        # Extract type from the function return type
                        rt_m = re.match(r'(\w+)\s+' + re.escape(name), code)
                        if rt_m and rt_m.group(1) != 'void':
                            needed_externs.add((sym, rt_m.group(1)))
                        else:
                            # For setters, get param type
                            pt_m = re.search(r'\((\w+)\s+val\)', code)
                            if pt_m:
                                needed_externs.add((sym, pt_m.group(1)))

                new_code_parts.append(f"/* 0x{addr:08X} | 0x{size:X} | {pattern} */")
                new_code_parts.append(code)
                matched_count += 1
            else:
                if name in asm_funcs:
                    _, _, _, instrs = asm_funcs[name]
                    stub = gen_asm_stub(name, addr, size, instrs)
                    new_code_parts.append(stub)
                    stub_count += 1
                else:
                    new_code_parts.append(f"/* 0x{addr:08X} | 0x{size:X} - no assembly */")
                    new_code_parts.append(f"void {name}(void) {{ /* TODO */ }}\n")
                    stub_count += 1
            new_count += 1

        if new_count == 0:
            print(f"  All functions already present, skipping")
            continue

        print(f"  {matched_count} pattern-matched, {stub_count} asm stubs, {len(funcs) - new_count} already present")

        # Build code block
        block_lines = []
        block_lines.append("")
        block_lines.append("/* ===================================================================")
        block_lines.append(f" * Generated functions ({new_count} total: {matched_count} matched, {stub_count} stubs)")
        block_lines.append(f" * Range: 0x{funcs[0][1]:08X} - 0x{funcs[-1][1]+funcs[-1][2]:08X}")
        block_lines.append(" * =================================================================== */")
        block_lines.append("")

        # Add extern declarations for SDA symbols
        if needed_externs:
            for sym, ct in sorted(needed_externs):
                block_lines.append(f"extern {ct} {sym};")
            block_lines.append("")

        block_lines.extend(new_code_parts)

        block = "\n".join(block_lines)

        if file_exists:
            with open(src_path, 'a', encoding='utf-8') as f:
                f.write(block)
            print(f"  Appended to {src_path.name}")
        else:
            src_path.parent.mkdir(parents=True, exist_ok=True)
            header = (
                f"/**\n"
                f" * @file {src_path.name}\n"
                f" * @brief Decompiled functions.\n"
                f" *\n"
                f" * Address range: 0x{funcs[0][1]:08X} - 0x{funcs[-1][1]+funcs[-1][2]:08X}\n"
                f" */\n"
                f"\n"
                f"#include \"dolphin/types.h\"\n"
            )
            with open(src_path, 'w', encoding='utf-8') as f:
                f.write(header)
                f.write(block)
            print(f"  Created {src_path.name}")

        total_written += new_count

    print(f"\n{'='*60}")
    print(f"TOTAL: {total_written} functions written ({len(matched)} matched, {len(unmatched)} stubs)")
    print(f"{'='*60}")


if __name__ == "__main__":
    main()
