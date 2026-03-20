#!/usr/bin/env python3
"""
Second pass: convert remaining pragma stubs to real C code for additional patterns.
"""

import re
import os
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).resolve().parent.parent
ASM_FILE = PROJECT_ROOT / "build" / "GC6E01" / "asm" / "auto_01_800055E0_text.s"
SYMBOLS_FILE = PROJECT_ROOT / "config" / "GC6E01" / "symbols.txt"
SRC_DIR = PROJECT_ROOT / "src"

RANGE_LO = 0x800F8268
RANGE_HI = 0x80138000


def parse_asm():
    functions = {}
    current_fn = None
    current_instrs = []
    pending_addr = 0
    pending_size = 0

    fn_start_re = re.compile(r'^\.fn\s+(\w+)(?:,\s*(\w+))?')
    fn_end_re = re.compile(r'^\.endfn\s+(\w+)')
    comment_re = re.compile(r'^# \.text:.*\|\s*(0x[0-9A-Fa-f]+)\s*\|\s*size:\s*(0x[0-9A-Fa-f]+)')
    instr_re = re.compile(r'/\*\s*([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f ]+\*/\s+(.+)')

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
                    functions[current_fn] = (pending_addr, pending_size, current_instrs)
                current_fn = m.group(1)
                current_instrs = []
                continue
            m = fn_end_re.match(line)
            if m:
                if current_fn:
                    functions[current_fn] = (pending_addr, pending_size, current_instrs)
                current_fn = None
                current_instrs = []
                continue
            if current_fn:
                m = instr_re.match(line.strip())
                if m:
                    rest = m.group(2).strip()
                    parts = rest.split(None, 1)
                    current_instrs.append((parts[0], parts[1] if len(parts) > 1 else ''))
    return functions


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

def fmt_off(v):
    if v < 0:
        return f"-0x{abs(v):X}"
    return f"0x{v:X}"


def try_match_extended(name, instrs, size):
    """Extended pattern matching for patterns the first pass missed."""
    n = len(instrs)
    if n < 2:
        return None
    if instrs[-1][0] != 'blr':
        return None

    # Pattern: null-check addi ptr return
    # cmplwi r3,0; bne; li r3,0; blr; addi r3,r3,OFF; blr
    if n == 6 and size == 0x18:
        if (instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1] and
            instrs[1][0] == 'bne' and instrs[2][0] == 'li' and
            instrs[3][0] == 'blr' and instrs[4][0] == 'addi'):
            m = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[4][1])
            if m:
                off = parse_int(m.group(1))
                return ("nc_addi_ptr",
                    f"void* {name}(void* ptr) {{\n"
                    f"    if (ptr == NULL) {{ return NULL; }}\n"
                    f"    return (u8*)ptr + {fmt_off(off)};\n"
                    f"}}\n")

    # Pattern: beq return-default getter (reverse null-check)
    # cmplwi r3,0; beq; load r3,OFF(r3); blr; li r3,DEFAULT; blr
    if n == 6 and size == 0x18:
        if (instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1] and
            instrs[1][0] == 'beq' and instrs[3][0] == 'blr'):
            mnem2 = instrs[2][0]
            if is_load(mnem2):
                p = parse_load_offset(instrs[2][1])
                if p and p[0] == 3 and p[2] == 3:
                    ct = load_ctype(mnem2)
                    # Check what the default return is
                    if instrs[4][0] == 'li':
                        m_val = re.match(r'r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[4][1])
                        if m_val:
                            default = parse_int(m_val.group(1))
                            return ("beq_default_getter",
                                f"{ct} {name}(void* ptr) {{\n"
                                f"    if (ptr == NULL) {{ return {default}; }}\n"
                                f"    return *({ct}*)((u8*)ptr + {fmt_off(p[1])});\n"
                                f"}}\n")
            # addi variant
            if instrs[2][0] == 'addi':
                m_addi = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[2][1])
                if m_addi:
                    off = parse_int(m_addi.group(1))
                    return ("beq_addi_ptr",
                        f"void* {name}(void* ptr) {{\n"
                        f"    if (ptr == NULL) {{ return NULL; }}\n"
                        f"    return (u8*)ptr + {fmt_off(off)};\n"
                        f"}}\n")

    # Pattern: compound setter (0x24 = 9 instrs)
    # cmplwi r3,0; bne L1; li r3,0; b L2; L1: addi r3,r3,OFF; L2: cmplwi r3,0; beqlr; store r4,OFF2(r3); blr
    if n == 9 and size == 0x24:
        if (instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1] and
            instrs[1][0] == 'bne' and instrs[2][0] == 'li' and
            instrs[3][0] == 'b' and instrs[4][0] == 'addi' and
            instrs[5][0] == 'cmplwi' and instrs[6][0] == 'beqlr'):
            m_addi = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[4][1])
            if m_addi:
                off1 = parse_int(m_addi.group(1))
                mnem7 = instrs[7][0]
                if is_store(mnem7):
                    p = parse_load_offset(instrs[7][1])
                    if p and p[0] == 4 and p[2] == 3:
                        ct = store_ctype(mnem7)
                        return ("compound_setter",
                            f"void {name}(void* ptr, {ct} val) {{\n"
                            f"    void* sub;\n"
                            f"    if (ptr == NULL) {{ return; }}\n"
                            f"    sub = (u8*)ptr + {fmt_off(off1)};\n"
                            f"    if (sub == NULL) {{ return; }}\n"
                            f"    *({ct}*)((u8*)sub + {fmt_off(p[1])}) = val;\n"
                            f"}}\n")

    # Pattern: compound getter (0x2C = 11 instrs)
    # cmplwi r3,0; bne; li r3,0; b; addi r3,r3,OFF; cmplwi r3,0; bne; li r3,0; blr; load r3,OFF2(r3); blr
    if n == 11 and size == 0x2C:
        if (instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1] and
            instrs[1][0] == 'bne' and instrs[2][0] == 'li' and
            instrs[3][0] == 'b' and instrs[4][0] == 'addi' and
            instrs[5][0] == 'cmplwi' and instrs[6][0] == 'bne' and
            instrs[7][0] == 'li' and instrs[8][0] == 'blr'):
            m_addi = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[4][1])
            if m_addi:
                off1 = parse_int(m_addi.group(1))
                mnem9 = instrs[9][0]
                if is_load(mnem9):
                    p = parse_load_offset(instrs[9][1])
                    if p and p[0] == 3 and p[2] == 3:
                        ct = load_ctype(mnem9)
                        return ("compound_getter",
                            f"{ct} {name}(void* ptr) {{\n"
                            f"    void* sub;\n"
                            f"    if (ptr == NULL) {{ return 0; }}\n"
                            f"    sub = (u8*)ptr + {fmt_off(off1)};\n"
                            f"    if (sub == NULL) {{ return 0; }}\n"
                            f"    return *({ct}*)((u8*)sub + {fmt_off(p[1])});\n"
                            f"}}\n")

    # Pattern: simple wrapper (call a function and return)
    # stwu r1,-0x10(r1); mflr r0; stw r0,0x14(r1); bl TARGET; li r3,VAL; lwz r0,0x14(r1); mtlr r0; addi r1,r1,0x10; blr
    if n == 9 and size == 0x24:
        if (instrs[0][0] == 'stwu' and instrs[1][0] == 'mflr' and
            instrs[2][0] == 'stw' and instrs[3][0] == 'bl' and
            instrs[4][0] == 'li' and instrs[5][0] == 'lwz' and
            instrs[6][0] == 'mtlr' and instrs[7][0] == 'addi'):
            target = instrs[3][1].strip()
            m_li = re.match(r'r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[4][1])
            if m_li:
                val = parse_int(m_li.group(1))
                return ("call_return_const",
                    f"u32 {name}(void) {{\n"
                    f"    {target}();\n"
                    f"    return {val};\n"
                    f"}}\n")

    # Pattern: wrapper that loads field then calls
    # stwu r1,-0x10(r1); mflr r0; stw r0,0x14(r1); lwz r3,OFF(r3); bl TARGET; lwz r0,0x14(r1); mtlr r0; addi r1,r1,0x10; blr
    if n == 9 and size == 0x24:
        if (instrs[0][0] == 'stwu' and instrs[1][0] == 'mflr' and
            instrs[2][0] == 'stw' and instrs[3][0] in ('lwz','lhz','lbz') and
            instrs[4][0] == 'bl' and instrs[5][0] == 'lwz' and
            instrs[6][0] == 'mtlr' and instrs[7][0] == 'addi'):
            p = parse_load_offset(instrs[3][1])
            if p and p[0] == 3 and p[2] == 3:
                target = instrs[4][1].strip()
                return ("load_then_call",
                    f"void {name}(void* obj) {{\n"
                    f"    {target}(*(void**)((u8*)obj + {fmt_off(p[1])}));\n"
                    f"}}\n")

    # Pattern: call then return const
    # stwu; mflr; stw; bl; lwz; li r3,VAL; mtlr; addi r1; blr
    if n == 9 and size == 0x24:
        if (instrs[0][0] == 'stwu' and instrs[1][0] == 'mflr' and
            instrs[2][0] == 'stw' and instrs[3][0] == 'bl' and
            instrs[4][0] == 'lwz' and instrs[5][0] == 'li' and
            instrs[6][0] == 'mtlr' and instrs[7][0] == 'addi'):
            target = instrs[3][1].strip()
            m_li = re.match(r'r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[5][1])
            if m_li:
                val = parse_int(m_li.group(1))
                return ("call_return_const2",
                    f"u32 {name}(void) {{\n"
                    f"    {target}();\n"
                    f"    return {val};\n"
                    f"}}\n")

    # Pattern: set field + return const
    # li r0, VAL; stb/sth/stw r0, OFF(r3); li r3, RET; blr
    if n == 4 and size == 0x10:
        if instrs[0][0] == 'li' and is_store(instrs[1][0]) and instrs[2][0] == 'li':
            m0 = re.match(r'r0,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[0][1])
            p1 = parse_load_offset(instrs[1][1])
            m2 = re.match(r'r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[2][1])
            if m0 and p1 and m2 and p1[0] == 0 and p1[2] == 3:
                val = parse_int(m0.group(1))
                ct = store_ctype(instrs[1][0])
                ret = parse_int(m2.group(1))
                return ("set_field_return",
                    f"u32 {name}(void* obj) {{\n"
                    f"    *({ct}*)((u8*)obj + {fmt_off(p1[1])}) = {val};\n"
                    f"    return {ret};\n"
                    f"}}\n")

    # Pattern: cmplwi r3, 0; bnelr; li r3, 0; blr (4 instrs, 0x10)
    # Returns 0 only if r3 is NULL, otherwise returns r3 unchanged
    if n == 4 and size == 0x10:
        if (instrs[0][0] == 'cmplwi' and 'r3' in instrs[0][1] and
            instrs[1][0] == 'bnelr' and instrs[2][0] == 'li'):
            m_val = re.match(r'r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[2][1])
            if m_val:
                val = parse_int(m_val.group(1))
                return ("nc_bnelr",
                    f"u32 {name}(void* ptr) {{\n"
                    f"    if (ptr != NULL) {{ return (u32)ptr; }}\n"
                    f"    return {val};\n"
                    f"}}\n")

    # Pattern: lis+addi+load (global variable getter)
    # lis r3, SYM@ha; addi r3,r3,SYM@l; load r3, OFF(r3); blr
    if n == 4 and size == 0x10:
        if instrs[0][0] == 'lis' and instrs[1][0] == 'addi':
            m_lis = re.match(r'r(\d+),\s*(\w+)@ha', instrs[0][1])
            m_addi = re.match(r'r3,\s*r\d+,\s*(\w+)@l', instrs[1][1])
            if m_lis and m_addi:
                sym = m_addi.group(1)
                mnem2 = instrs[2][0]
                if is_load(mnem2):
                    p = parse_load_offset(instrs[2][1])
                    if p and p[0] == 3 and p[2] == 3:
                        ct = load_ctype(mnem2)
                        return ("global_getter",
                            f"{ct} {name}(void) {{\n"
                            f"    return *({ct}*)((u8*){sym} + {fmt_off(p[1])});\n"
                            f"}}\n")

    # Pattern: lis+addi+store (global variable setter)
    if n == 4 and size == 0x10:
        if instrs[0][0] == 'lis' and instrs[1][0] == 'addi':
            m_addi = re.match(r'r(\d+),\s*r\d+,\s*(\w+)@l', instrs[1][1])
            if m_addi:
                sym = m_addi.group(2)
                dst_reg = int(m_addi.group(1))
                mnem2 = instrs[2][0]
                if is_store(mnem2):
                    p = parse_load_offset(instrs[2][1])
                    if p and p[2] == dst_reg:
                        ct = store_ctype(mnem2)
                        return ("global_setter",
                            f"void {name}({ct} val) {{\n"
                            f"    *({ct}*)((u8*){sym} + {fmt_off(p[1])}) = val;\n"
                            f"}}\n")

    # Pattern: lis+addi+addi (global address return)
    if n == 4 and size == 0x10:
        if instrs[0][0] == 'lis' and instrs[1][0] == 'addi' and instrs[2][0] == 'addi':
            m_addi1 = re.match(r'r3,\s*r\d+,\s*(\w+)@l', instrs[1][1])
            m_addi2 = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[2][1])
            if m_addi1 and m_addi2:
                sym = m_addi1.group(1)
                off = parse_int(m_addi2.group(1))
                return ("global_addr",
                    f"void* {name}(void) {{\n"
                    f"    return (u8*){sym} + {fmt_off(off)};\n"
                    f"}}\n")

    # Pattern: SDA swap (read SDA, write arg, return old)
    # lbz r0, SYM@sda21(r0); stb r3, SYM@sda21(r0); mr r3, r0; blr
    if n == 4 and size == 0x10:
        if (instrs[0][0] in ('lbz','lhz','lwz') and instrs[2][0] == 'mr'):
            p0 = parse_sda(instrs[0][1])
            p1 = parse_sda(instrs[1][1])
            if p0 and p1 and p0[2] == p1[2]:
                ct = load_ctype(instrs[0][0])
                sym = p0[2]
                return ("sda_swap",
                    f"{ct} {name}({ct} val) {{\n"
                    f"    {ct} old = {sym};\n"
                    f"    {sym} = val;\n"
                    f"    return old;\n"
                    f"}}\n")

    # Pattern: clrlslwi indexed lookup
    # lis r4, SYM@ha; clrlslwi r0, r3, 16, 3; addi r3, r4, SYM@l; lbzx r3, r3, r0; blr
    if n == 5 and size == 0x14:
        if (instrs[0][0] == 'lis' and instrs[1][0] == 'clrlslwi' and
            instrs[2][0] == 'addi' and instrs[3][0] == 'lbzx'):
            m_addi = re.match(r'r3,\s*r\d+,\s*(\w+)@l', instrs[2][1])
            if m_addi:
                sym = m_addi.group(1)
                return ("indexed_lookup",
                    f"u8 {name}(u32 idx) {{\n"
                    f"    return ((u8*){sym})[idx];\n"
                    f"}}\n")

    # Pattern: multi-SDA store (zeroing multiple globals)
    # li r0, VAL; stb/stw r0, SYM1@sda21; stb/stw r0, SYM2@sda21; ...; blr
    all_stores = True
    if n >= 3 and instrs[0][0] == 'li':
        m_li = re.match(r'r0,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[0][1])
        if m_li:
            val = parse_int(m_li.group(1))
            stores = []
            for i in range(1, n-1):
                if is_store(instrs[i][0]):
                    p = parse_sda(instrs[i][1])
                    if p and p[0] == 'r' and p[1] == 0:
                        stores.append((p[2], store_ctype(instrs[i][0])))
                    else:
                        all_stores = False
                        break
                else:
                    all_stores = False
                    break
            if all_stores and len(stores) >= 2:
                lines = [f"void {name}(void) {{"]
                for sym, ct in stores:
                    lines.append(f"    {sym} = {val};")
                lines.append("}\n")
                return ("multi_sda_store", "\n".join(lines))

    return None


def main():
    print("Parsing assembly...")
    asm_funcs = parse_asm()
    print(f"  {len(asm_funcs)} functions parsed")

    # Find all stubs in source files
    stubs_found = []
    stub_re = re.compile(
        r'(/\* 0x([0-9A-Fa-f]{8}) \| (0x[0-9A-Fa-f]+) \*/\n'
        r'#pragma push\n'
        r'#pragma optimization_level 0\n'
        r'#pragma optimizewithasm off\n'
        r'void (fn_[0-9A-Fa-f]{8})\(void\) \{\n'
        r'    /\* TODO: match -- (\d+) bytes at 0x[0-9A-Fa-f]+ \*/\n'
        r'\}\n'
        r'#pragma pop\n)'
    )

    files_to_update = {}
    for root, dirs, files in os.walk(SRC_DIR):
        for fname in files:
            if fname.endswith('.c'):
                fpath = os.path.join(root, fname)
                with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
                    content = f.read()
                for m in stub_re.finditer(content):
                    full_match = m.group(0)
                    addr = int(m.group(2), 16)
                    size_hex = m.group(3)
                    fn_name = m.group(4)
                    size = int(m.group(5))

                    if RANGE_LO <= addr < RANGE_HI:
                        if fpath not in files_to_update:
                            files_to_update[fpath] = []
                        files_to_update[fpath].append((full_match, fn_name, addr, size))

    total_stubs = sum(len(v) for v in files_to_update.values())
    print(f"Found {total_stubs} stubs across {len(files_to_update)} files")

    # Try to match each stub
    total_improved = 0
    total_failed = 0
    pattern_counts = defaultdict(int)
    needed_externs = defaultdict(set)  # file -> set of (sym, type)

    for fpath, stubs in files_to_update.items():
        improved = 0
        for full_match, fn_name, addr, size in stubs:
            if fn_name not in asm_funcs:
                total_failed += 1
                continue

            _, asm_size, instrs = asm_funcs[fn_name]
            result = try_match_extended(fn_name, instrs, asm_size)
            if result:
                pattern, new_code = result
                pattern_counts[pattern] += 1

                # Build replacement
                replacement = f"/* 0x{addr:08X} | {size} bytes | {pattern} */\n{new_code}"

                # Read current content and replace
                with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
                    content = f.read()
                content = content.replace(full_match, replacement)
                with open(fpath, 'w', encoding='utf-8') as f:
                    f.write(content)

                # Track needed externs
                for ext_m in re.finditer(r'(\w+)\(\)', new_code):
                    sym = ext_m.group(1)
                    if sym.startswith('fn_') or sym.startswith('lbl_'):
                        needed_externs[fpath].add(sym)
                for ext_m in re.finditer(r'\(u8\*\)(\w+)', new_code):
                    sym = ext_m.group(1)
                    if sym.startswith('lbl_'):
                        needed_externs[fpath].add(sym)

                improved += 1
                total_improved += 1
            else:
                total_failed += 1

        if improved > 0:
            rel = os.path.relpath(fpath, PROJECT_ROOT)
            print(f"  {rel}: {improved} stubs improved")

    print(f"\nPattern matches: {total_improved}")
    for p, c in sorted(pattern_counts.items(), key=lambda x: -x[1]):
        print(f"  {p:25s}: {c}")
    print(f"Stubs remaining: {total_failed}")

    # Add extern declarations for lbl_ symbols used
    for fpath, syms in needed_externs.items():
        lbl_syms = [s for s in syms if s.startswith('lbl_')]
        if not lbl_syms:
            continue
        with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        # Check which symbols are already declared
        new_externs = []
        for sym in sorted(lbl_syms):
            if f"extern" in content and sym in content.split("extern")[0:]:
                continue
            if f"{sym}" not in content.split("extern")[0]:
                # Only add if not already declared
                if f"extern u32 {sym};" not in content and f"extern void* {sym};" not in content:
                    new_externs.append(f"extern u32 {sym};")

        if new_externs:
            # Find the Generated block header and add externs after it
            insert_point = content.find("/* ===================================================================\n * Generated:")
            if insert_point >= 0:
                end_of_header = content.find(" */\n", insert_point)
                if end_of_header >= 0:
                    insert_pos = end_of_header + 4
                    extern_block = "\n" + "\n".join(new_externs) + "\n"
                    content = content[:insert_pos] + extern_block + content[insert_pos:]
                    with open(fpath, 'w', encoding='utf-8') as f:
                        f.write(content)


if __name__ == "__main__":
    main()
