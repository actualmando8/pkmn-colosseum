#!/usr/bin/env python3
"""
Generate C function stubs for all uncovered functions in range 0x800F8268-0x80138000.
Pattern-matched accessors get real C code; complex functions get pragma stubs.
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
        r'static\s+\w+\*?|struct\s+\w+\*?|[A-Z]\w+\*?|asm\s+void|asm\s+u32|asm\s+s32)\s+'
        r'\*?\s*(fn_([0-9A-Fa-f]{8}))\s*\('
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
                # Also catch named functions via address comments
                for m in re.finditer(r'/\*.*?0x([0-9A-Fa-f]{8}).*?\*/', content):
                    # Check if this is preceded by a function definition
                    pass
    return defined


# ============================================================================
# Pattern matching helpers
# ============================================================================

def parse_int(s):
    s = s.strip()
    if s.startswith('-0x') or s.startswith('-0X'):
        return -int(s[1:], 16)
    elif s.startswith('0x') or s.startswith('0X'):
        return int(s, 16)
    elif s.startswith('-'):
        return int(s)
    else:
        return int(s)


def parse_load_offset(ops):
    m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))\(r(\d+)\)', ops)
    if m:
        return (int(m.group(1)), parse_int(m.group(2)), int(m.group(3)))
    return None

def parse_float_offset(ops):
    m = re.match(r'f(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))\(r(\d+)\)', ops)
    if m:
        return (int(m.group(1)), parse_int(m.group(2)), int(m.group(3)))
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

    # void stub (blr only)
    if n == 1 and size == 0x4:
        return ("void_stub", f"void {name}(void) {{\n}}\n")

    # return constant (li r3, IMM; blr)
    if n == 2 and size == 0x8 and instrs[0][0] == 'li':
        m = re.match(r'r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[0][1])
        if m:
            val = parse_int(m.group(1))
            if val < 0:
                return ("return_const", f"s32 {name}(void) {{ return {val}; }}\n")
            else:
                return ("return_const", f"u32 {name}(void) {{ return {val}; }}\n")

    # addi return pointer (addi r3, r3, OFF; blr)
    if n == 2 and size == 0x8 and instrs[0][0] == 'addi':
        m = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[0][1])
        if m:
            off = parse_int(m.group(1))
            return ("addi_ptr", f"void* {name}(void* obj) {{ return (u8*)obj + {fmt_off(off)}; }}\n")

    # mr r3, r4; blr
    if n == 2 and size == 0x8 and instrs[0][0] == 'mr':
        m = re.match(r'r3,\s*r(\d+)', instrs[0][1])
        if m:
            src_reg = int(m.group(1))
            if src_reg == 4:
                return ("mr_passthrough", f"u32 {name}(u32 a, u32 b) {{ return b; }}\n")

    # simple getter (load r3, OFF(r3); blr)
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
                return ("simple_getter", f"{ct} {name}(void* obj) {{ return *({ct}*)((u8*)obj + {fmt_off(p[1])}); }}\n")

    # simple setter (store r4, OFF(r3); blr)
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
                return ("simple_setter", f"void {name}(void* obj, {ct} val) {{ *({ct}*)((u8*)obj + {fmt_off(p[1])}) = val; }}\n")

    # SDA getter (load r3/f1, sym@sda21(rX); blr)
    if n == 2 and size == 0x8:
        mnem = instrs[0][0]
        if is_load(mnem) or is_float_load(mnem):
            p = parse_sda(instrs[0][1])
            if p:
                rf, rn, sym, base = p
                if (rf == 'r' and rn == 3) or (rf == 'f' and rn == 1):
                    ct = load_ctype(mnem)
                    return ("sda_getter", f"{ct} {name}(void) {{ return {sym}; }}\n")

    # SDA setter (store r3/f1, sym@sda21(rX); blr)
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
            hi = parse_int(m.group(1)) << 16
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
            hi = parse_int(m.group(1)) << 16
            mnem = instrs[1][0]
            if is_store(mnem) and not is_float_store(mnem):
                p = parse_load_offset(instrs[1][1])
                if p and p[0] == 4 and p[2] == 3:
                    actual = hi + p[1]
                    ct = store_ctype(mnem)
                    return ("addis_setter", f"void {name}(void* obj, {ct} val) {{ *({ct}*)((u8*)obj + {fmt_off(actual)}) = val; }}\n")

    # SDA deref getter (3 instrs: lwz r3, sym@sda21(r13); load r3, OFF(r3); blr)
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

    # SDA deref setter (3 instrs: lwz rX, sym@sda21(r13); store r3, OFF(rX); blr)
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
                    if p1 and p1[2] == base_reg and p1[0] in (3, 4):
                        ct = store_ctype(mnem1)
                        if p1[0] == 3:
                            return ("sda_deref_setter", f"void {name}({ct} val) {{\n    *({ct}*)((u8*){sym} + {fmt_off(p1[1])}) = val;\n}}\n")
                        elif p1[0] == 4:
                            return ("sda_deref_setter2", f"void {name}(void* unused, {ct} val) {{\n    *({ct}*)((u8*){sym} + {fmt_off(p1[1])}) = val;\n}}\n")

    # null-check bne getter (cmplwi r3,0; bne; li r3,0; blr; load r3,OFF(r3); blr)
    if n >= 5 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if instrs[1][0] == 'bne' and instrs[2][0] == 'li' and instrs[3][0] == 'blr':
            m_li = re.match(r'r3,\s*0', instrs[2][1])
            if m_li:
                mnem4 = instrs[4][0]
                if n == 6 and is_load(mnem4) and not is_float_load(mnem4):
                    p = parse_load_offset(instrs[4][1])
                    if p and p[0] == 3 and p[2] == 3:
                        ct = load_ctype(mnem4)
                        return ("nc_getter", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(p[1])});\n}}\n")
                if n == 6 and is_float_load(mnem4):
                    p = parse_float_offset(instrs[4][1])
                    if p and p[0] == 1 and p[2] == 3:
                        ct = load_ctype(mnem4)
                        return ("nc_getter_f", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(p[1])});\n}}\n")
                # lbz r0, OFF(r3); extsb r3, r0
                if n >= 7 and mnem4 == 'lbz':
                    p = parse_load_offset(instrs[4][1])
                    if p and p[0] == 0 and p[2] == 3 and instrs[5][0] == 'extsb':
                        return ("nc_getter_s8", f"s32 {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return (s8)*((u8*)ptr + {fmt_off(p[1])});\n}}\n")
                # Double deref: lwz r3, OFF(r3); load r3, OFF2(r3)
                if n >= 7 and mnem4 == 'lwz':
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
            mnem2 = instrs[2][0]
            if is_load(mnem2) and not is_float_load(mnem2):
                p = parse_load_offset(instrs[2][1])
                if p and p[0] == 3 and p[2] == 3:
                    ct = load_ctype(mnem2)
                    return ("nc_beqlr_getter", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(p[1])});\n}}\n")

    # null-check setter (cmplwi r3,0; beqlr; store r4/f1, OFF(r3); blr)
    if n == 4 and size == 0x10:
        if instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
            if instrs[1][0] == 'beqlr':
                mnem2 = instrs[2][0]
                if is_store(mnem2) and not is_float_store(mnem2):
                    p = parse_load_offset(instrs[2][1])
                    if p and p[0] == 4 and p[2] == 3:
                        ct = store_ctype(mnem2)
                        return ("nc_setter", f"void {name}(void* ptr, {ct} val) {{\n    if (ptr == NULL) {{ return; }}\n    *({ct}*)((u8*)ptr + {fmt_off(p[1])}) = val;\n}}\n")
                if is_float_store(mnem2):
                    p = parse_float_offset(instrs[2][1])
                    if p and p[0] == 1 and p[2] == 3:
                        ct = store_ctype(mnem2)
                        return ("nc_setter_f", f"void {name}(void* ptr, {ct} val) {{\n    if (ptr == NULL) {{ return; }}\n    *({ct}*)((u8*)ptr + {fmt_off(p[1])}) = val;\n}}\n")

    # null-check addis getter (cmplwi r3,0; beqlr; addis r3,r3; load r3,OFF(r3); blr)
    if n == 5 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if instrs[1][0] == 'beqlr' and instrs[2][0] == 'addis':
            m = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[2][1])
            if m:
                hi = parse_int(m.group(1)) << 16
                mnem3 = instrs[3][0]
                if is_load(mnem3) and not is_float_load(mnem3):
                    p = parse_load_offset(instrs[3][1])
                    if p and p[0] == 3 and p[2] == 3:
                        actual = hi + p[1]
                        ct = load_ctype(mnem3)
                        return ("nc_addis_getter", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(actual)});\n}}\n")

    # null-check addis setter
    if n == 5 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if instrs[1][0] == 'beqlr' and instrs[2][0] == 'addis':
            m = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[2][1])
            if m:
                hi = parse_int(m.group(1)) << 16
                mnem3 = instrs[3][0]
                if is_store(mnem3) and not is_float_store(mnem3):
                    p = parse_load_offset(instrs[3][1])
                    if p and p[0] == 4 and p[2] == 3:
                        actual = hi + p[1]
                        ct = store_ctype(mnem3)
                        return ("nc_addis_setter", f"void {name}(void* ptr, {ct} val) {{\n    if (ptr == NULL) {{ return; }}\n    *({ct}*)((u8*)ptr + {fmt_off(actual)}) = val;\n}}\n")

    # null-check bne + addis getter (7 instrs)
    if n == 7 and instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1]:
        if (instrs[1][0] == 'bne' and instrs[2][0] == 'li' and
            instrs[3][0] == 'blr' and instrs[4][0] == 'addis'):
            m_addis = re.match(r'r3,\s*r3,\s*(0x[0-9a-fA-F]+|\d+)', instrs[4][1])
            if m_addis:
                hi = parse_int(m_addis.group(1)) << 16
                mnem5 = instrs[5][0]
                if is_load(mnem5) and not is_float_load(mnem5):
                    p = parse_load_offset(instrs[5][1])
                    if p and p[0] == 3 and p[2] == 3:
                        actual = hi + p[1]
                        ct = load_ctype(mnem5)
                        return ("nc_bne_addis", f"{ct} {name}(void* ptr) {{\n    if (ptr == NULL) {{ return 0; }}\n    return *({ct}*)((u8*)ptr + {fmt_off(actual)});\n}}\n")

    # SDA load + load chain: lwz r3, sym@sda21(r13); lwz r3, OFF(r3); load r3, OFF2(r3); blr
    if n == 4 and size == 0x10:
        if instrs[0][0] == 'lwz':
            p0 = parse_sda(instrs[0][1])
            if p0 and p0[0] == 'r' and p0[1] == 3:
                if instrs[1][0] == 'lwz':
                    p1 = parse_load_offset(instrs[1][1])
                    if p1 and p1[0] == 3 and p1[2] == 3:
                        mnem2 = instrs[2][0]
                        if is_load(mnem2) and not is_float_load(mnem2):
                            p2 = parse_load_offset(instrs[2][1])
                            if p2 and p2[0] == 3 and p2[2] == 3:
                                ct = load_ctype(mnem2)
                                sym = p0[2]
                                return ("sda_chain_getter", f"{ct} {name}(void) {{\n    void* tmp = *(void**)((u8*){sym} + {fmt_off(p1[1])});\n    return *({ct}*)((u8*)tmp + {fmt_off(p2[1])});\n}}\n")

    # Tail call: b target (not blr)
    if n == 1 and size == 0x4 and instrs[0][0] == 'b' and instrs[0][1].strip().startswith('fn_'):
        target = instrs[0][1].strip()
        return ("tail_call", f"/* tail call to {target} */\nvoid {name}(void) {{ {target}(); }}\n")

    return None


def gen_pragma_stub(name, addr, size):
    """Generate a #pragma stub for a complex function."""
    lines = []
    lines.append(f"#pragma push")
    lines.append(f"#pragma optimization_level 0")
    lines.append(f"#pragma optimizewithasm off")
    lines.append(f"void {name}(void) {{")
    lines.append(f"    /* TODO: match -- {size} bytes at 0x{addr:08X} */")
    lines.append(f"}}")
    lines.append(f"#pragma pop")
    lines.append(f"")
    return "\n".join(lines)


def main():
    print("Parsing assembly...")
    asm_funcs = parse_asm()
    print(f"  {len(asm_funcs)} functions parsed")

    print("Parsing symbols...")
    symbols = parse_symbols()
    print(f"  {len(symbols)} functions in range")

    print("Parsing link order...")
    link_ranges = parse_link_order()

    print("Finding existing definitions...")
    existing = find_existing()
    print(f"  {len(existing)} already defined")

    uncovered = [(name, addr, size) for name, addr, size in symbols if name not in existing]
    print(f"  {len(uncovered)} uncovered functions")

    # Pattern match
    matched = {}
    unmatched = []
    pattern_counts = defaultdict(int)

    for name, addr, size in uncovered:
        if name in asm_funcs:
            _, _, _, instrs = asm_funcs[name]
            result = try_match(name, instrs, size)
            if result:
                pattern, code = result
                matched[name] = (addr, size, pattern, code)
                pattern_counts[pattern] += 1
            else:
                unmatched.append((name, addr, size))
        else:
            unmatched.append((name, addr, size))

    print(f"\nPattern matches: {len(matched)}")
    for p, c in sorted(pattern_counts.items(), key=lambda x: -x[1]):
        print(f"  {p:25s}: {c}")
    print(f"Stubs needed: {len(unmatched)}")

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

        existing_content = ""
        file_exists = src_path.exists()
        if file_exists:
            with open(src_path, 'r', encoding='utf-8', errors='replace') as f:
                existing_content = f.read()

        # Find already-defined functions in file
        already_in_file = set()
        for fm in re.finditer(
            r'(?:void|u8|u16|u32|s8|s16|s32|f32|f64|int|BOOL|char\*?|asm\s+void|asm\s+u32|asm\s+s32|s32|u8\*|void\*)\s+'
            r'\*?\s*(fn_[0-9A-Fa-f]{8})\s*\(',
            existing_content
        ):
            already_in_file.add(fm.group(1))
        # Also check address comments for named functions
        for fm in re.finditer(r'at\s+0x([0-9A-Fa-f]{8})', existing_content):
            fn_name = f"fn_{fm.group(1)}"
            already_in_file.add(fn_name)

        new_code_parts = []
        needed_sda_externs = {}  # sym -> ctype
        new_count = 0
        match_count = 0
        stub_count = 0

        for name, addr, size in funcs:
            if name in already_in_file:
                continue

            if name in matched:
                _, _, pattern, code = matched[name]
                # Collect SDA externs
                if 'sda' in pattern:
                    for ext_m in re.finditer(r'return (\w+);|(\w+) = val;', code):
                        sym = ext_m.group(1) or ext_m.group(2)
                        if sym and sym.startswith('lbl_'):
                            # Determine type
                            rt_m = re.match(r'(\w+)\s+' + re.escape(name), code)
                            if rt_m and rt_m.group(1) != 'void':
                                needed_sda_externs[sym] = rt_m.group(1)
                            else:
                                pt_m = re.search(r'\((\w+)\s+val\)', code)
                                if pt_m:
                                    needed_sda_externs[sym] = pt_m.group(1)

                new_code_parts.append(f"/* 0x{addr:08X} | 0x{size:X} | {pattern} */")
                new_code_parts.append(code)
                match_count += 1
            else:
                stub = gen_pragma_stub(name, addr, size)
                new_code_parts.append(f"/* 0x{addr:08X} | 0x{size:X} */")
                new_code_parts.append(stub)
                stub_count += 1
            new_count += 1

        if new_count == 0:
            continue

        print(f"{tu}: +{new_count} ({match_count} matched, {stub_count} stubs)")

        # Build block
        block_lines = []
        block_lines.append("")
        block_lines.append("/* ===================================================================")
        block_lines.append(f" * Generated: {match_count} pattern-matched + {stub_count} stubs")
        block_lines.append(f" * Range: 0x{funcs[0][1]:08X} - 0x{funcs[-1][1]+funcs[-1][2]:08X}")
        block_lines.append(" * =================================================================== */")
        block_lines.append("")

        # Add SDA externs
        if needed_sda_externs:
            for sym in sorted(needed_sda_externs.keys()):
                ct = needed_sda_externs[sym]
                block_lines.append(f"extern {ct} {sym};")
            block_lines.append("")

        block_lines.extend(new_code_parts)
        block = "\n".join(block_lines)

        if file_exists:
            with open(src_path, 'a', encoding='utf-8') as f:
                f.write(block)
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

        total_written += new_count

    print(f"\nTOTAL: {total_written} functions written ({len(matched)} matched, {len(unmatched)} stubs)")


if __name__ == "__main__":
    main()
