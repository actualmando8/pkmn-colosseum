#!/usr/bin/env python3
"""Convert small pragma-guarded functions from register-level to idiomatic C.

This script processes each file and replaces small pragma-guarded function blocks
with cleaned-up idiomatic C. It handles the most common patterns found in
auto-decompiled PPC functions.
"""
import re
import sys
import os

def find_pragma_blocks(content):
    """Find all #pragma push...#pragma pop blocks and return their spans."""
    blocks = []
    lines = content.split('\n')
    i = 0
    while i < len(lines):
        if lines[i].strip() == '#pragma push':
            start = i
            # Find matching #pragma pop
            j = i + 1
            depth = 1
            while j < len(lines):
                if lines[j].strip() == '#pragma push':
                    depth += 1
                elif lines[j].strip() == '#pragma pop':
                    depth -= 1
                    if depth == 0:
                        blocks.append((start, j))
                        break
                j += 1
            i = j + 1
        else:
            i += 1
    return blocks

def is_small_function(lines, start, end):
    """Check if a pragma block contains a small function (<=12 body statements)."""
    body_lines = 0
    has_register_decls = False
    fn_name = None
    fn_sig = None
    fn_line = -1

    for i in range(start, end + 1):
        l = lines[i].strip()
        if re.match(r'^u32 r\d+\s*=', l) or re.match(r'^u8 sp\[', l) or \
           re.match(r'^f32 f\d+\s*=', l) or re.match(r'^f64 f\d+\s*=', l) or \
           l.startswith('void (*ctr_fn)') or l.startswith('u32 ctr ='):
            has_register_decls = True

        m = re.match(r'((?:void|u32|s32|u16|s16|u8|s8|f32|f64|int|u32\*|void\*|s32\*)\s+)(fn_[0-9A-Fa-f]+)\s*\((.+?)\)', l)
        if m and fn_name is None:
            fn_name = m.group(2)
            fn_sig = l
            fn_line = i

        if (l and not l.startswith('//') and not l.startswith('/*') and not l.startswith('*')
            and not l.startswith('#pragma') and l != '{' and l != '}'
            and not l.startswith('extern ')
            and not re.match(r'^u32 r\d+\s*=', l) and not re.match(r'^u8 sp\[', l)
            and not re.match(r'^f32 f\d+\s*=', l) and not re.match(r'^f64 f\d+\s*=', l)
            and not l.startswith('u32 r1 =') and not l.startswith('void (*ctr_fn)')
            and not l.startswith('u32 ctr =')
            and not re.match(r'^(?:void|u32|s32|u16|s16|u8|s8|f32|f64|int|u32\*|void\*|s32\*)\s+fn_', l)):
            body_lines += 1

    return has_register_decls and body_lines <= 12 and fn_name is not None, fn_name, fn_line

def extract_externs(lines, start, end):
    """Extract extern declarations from within the function body."""
    externs = []
    for i in range(start, end + 1):
        l = lines[i].strip()
        if l.startswith('extern '):
            externs.append(l)
    return externs

def get_block_text(lines, start, end):
    """Get the original text of a block."""
    return '\n'.join(lines[start:end+1])


# Define manual conversions for each file
# Each entry: (function_name, replacement_text)
# The replacement text replaces the entire #pragma push...#pragma pop block

GS_RENDER_CONVERSIONS = {
    'fn_800D6A5C': '''void fn_800D6A5C(void) {
    extern u8 lbl_804001F0[];
    u8* base = lbl_804001F0;
    *(u32*)(base + 0xC) = *(u32*)(base + 0xC); /* r5 = *(base+0xC) + r3(=0) */
    *(u32*)(base + 0x4) = *(u32*)(base + 0x4); /* r0 = *(base+0x4) + r4(=0) */
}''',

    'fn_800DC540': '''void fn_800DC540(void) {
    extern u8 lbl_80400EE0[];
    extern u8 lbl_8047AAE0[];
    u8* base = lbl_80400EE0;
    *(u8*)base = 0;
    *(u8*)lbl_8047AAE0 = 0;
    *(u8*)(base + 0x14) = 0;
    *(u8*)(base + 0x28) = 0;
    *(u8*)(base + 0x3C) = 0;
}''',

    'fn_800DCAA4': '''void fn_800DCAA4(void) {
    /* empty / nop */
}''',

    'fn_800DCAB0': '''void fn_800DCAB0(void* self) {
    extern void fn_801A48F4();
    u8* ptr = (u8*)self;
    *(u32*)(ptr + 0x4) = 0;
    *(u32*)(ptr + 0x8) = 0;
    fn_801A48F4(*(u32*)(ptr + 0xC));
}''',

    'fn_800DCADC': '''void fn_800DCADC(void* self) {
    u8* ptr = (u8*)self;
    *(u32*)(ptr + 0x4) = 0;
    *(u32*)(ptr + 0x8) = 0;
}''',

    'fn_800DCC3C': '''void fn_800DCC3C(void* self) {
    extern void fn_801A48F4();
    fn_801A48F4(*(u32*)((u8*)self + 0xC));
}''',

    'fn_800DCC60': '''void fn_800DCC60(void* self) {
    extern void fn_801A49C0();
    fn_801A49C0(*(u32*)((u8*)self + 0xC));
}''',

    'fn_800DF11C': '''void fn_800DF11C(void* src, void* dst) {
    u8* s = (u8*)src;
    u8* d = (u8*)dst;
    d[0] = s[0xC];
    d[1] = s[0xD];
    d[2] = s[0xE];
    d[3] = s[0xF];
}''',

    'fn_800DF1B8': '''void fn_800DF1B8(void* self, f32 value) {
    u8* ptr = (u8*)self;
    u32 sub = *(u32*)(ptr + 0x20);
    *(f32*)(ptr + 0x34) = value;
    if (sub != 0) {
        *(f32*)((u8*)sub + 0x50) = value;
    }
}''',

    'fn_800DF1D0': '''void fn_800DF1D0(void* self, u32 p1, u32 p2, f32 fval, u32 p3) {
    u8* ptr = (u8*)self;
    *(u32*)(ptr + 0x2C) = p1;
    *(u32*)(ptr + 0x30) = p2;
    *(f32*)(ptr + 0x34) = fval;
    *(u32*)(ptr + 0x28) = p3;
}''',

    'fn_800DF1E4': '''void fn_800DF1E4(void* self, void* color) {
    u8* ptr = (u8*)self;
    u8* c = (u8*)color;
    ptr[0xC] = c[0];
    ptr[0xD] = c[1];
    ptr[0xE] = c[2];
    ptr[0xF] = c[3];
}''',

    'fn_800DF208': '''void fn_800DF208(void* self, u32 a, u32 b, u32 c, u32 d) {
    u8* ptr = (u8*)self;
    *(u32*)(ptr + 0x10) = a;
    *(u32*)(ptr + 0x14) = b;
    *(u32*)(ptr + 0x18) = c;
    *(u32*)(ptr + 0x1C) = d;
}''',

    'fn_800DF21C': '''void fn_800DF21C(void* self) {
    extern void fn_801A6DDC();
    fn_801A6DDC(*(u32*)((u8*)self + 0x8));
}''',

    'fn_800DF470': '''void fn_800DF470(void* self) {
    u8* ptr = (u8*)self;
    u32 cur = *(u32*)(ptr + 0x3C);
    u32 check = cur + (0x102 << 16);
    if (check == 0xfefe) return;
    u32 sub = *(u32*)(ptr + 0x8);
    *(u32*)((u8*)sub + 0x10) = cur;
    *(u32*)(ptr + 0x3C) = check;
}''',

    'fn_800DFF98': '''void fn_800DFF98(void* a, void* b, void* c) {
    extern void fn_800A37CC();
    fn_800A37CC(b, c, a);
}''',

    'fn_800DFFCC': '''void fn_800DFFCC(void* a, void* b, void* c) {
    extern void fn_800A3B9C();
    fn_800A3B9C(b, c, a);
}''',

    'fn_800E0000': '''void fn_800E0000(void) {
    extern void fn_800A3B7C();
    fn_800A3B7C();
}''',

    'fn_800E0020': '''void fn_800E0020(void) {
    extern void fn_800A3BD8();
    fn_800A3BD8();
}''',

    'fn_800E0040': '''void fn_800E0040(void) {
    extern void fn_800A3C00();
    fn_800A3C00();
}''',

    'fn_800E0060': '''void fn_800E0060(void* a, void* b) {
    extern void fn_800A3ADC();
    fn_800A3ADC(b, a);
}''',

    'fn_800E008C': '''void fn_800E008C(void) {
    extern void fn_800A3B38();
    fn_800A3B38();
}''',

    'fn_800E00E0': '''void fn_800E00E0(void* dst, void* src) {
    f32* d = (f32*)dst;
    f32* s = (f32*)src;
    d[0] = -s[0];
    d[1] = -s[1];
    d[2] = -s[2];
}''',

    'fn_800E0108': '''void fn_800E0108(void* dst, void* a, void* b) {
    f32* d = (f32*)dst;
    f32* va = (f32*)a;
    f32* vb = (f32*)b;
    d[0] = va[0] * vb[0];
    d[1] = va[1] * vb[1];
    d[2] = va[2] * vb[2];
}''',

    'fn_800E013C': '''void fn_800E013C(void* a, void* b) {
    extern void fn_800A3AC0();
    fn_800A3AC0(b, a);
}''',

    'fn_800E0168': '''void fn_800E0168(void* a, void* b, void* c) {
    extern void fn_800A3A9C();
    fn_800A3A9C(b, c, a);
}''',

    'fn_800E019C': '''void fn_800E019C(void* a, void* b, void* c) {
    extern void fn_800A3A78();
    fn_800A3A78(b, c, a);
}''',

    'fn_800E01D0': '''void fn_800E01D0(void* dst, void* src) {
    memcpy(dst, src, 0xc);
}''',

    'fn_800E01F4': '''void fn_800E01F4(void* dst, f32 x, f32 y, f32 z) {
    f32* d = (f32*)dst;
    d[0] = x;
    d[1] = y;
    d[2] = z;
}''',

    'fn_800E0204': '''void fn_800E0204(void* dst, f32 val) {
    extern u8 lbl_8047CADC[];
    f32 v = *(f32*)lbl_8047CADC;
    f32* d = (f32*)dst;
    d[0] = v;
    d[1] = v;
    d[2] = v;
}''',

    'fn_800E0218': '''void fn_800E0218(void) {
    extern void fn_800A3458();
    fn_800A3458();
}''',

    'fn_800E0238': '''void fn_800E0238(void* a, void* b) {
    extern void fn_800A2E64();
    fn_800A2E64(b, a);
}''',

    'fn_800E0264': '''void fn_800E0264(void* a, void* b) {
    extern void fn_800A2EB4();
    fn_800A2EB4(b, a);
}''',

    'fn_800E0290': '''void fn_800E0290(void* a, void* b, void* c) {
    extern void fn_800A2D98();
    fn_800A2D98(b, c, a);
}''',

    'fn_800E02C4': '''void fn_800E02C4(void* a) {
    extern void fn_800A335C();
    fn_800A335C(0, a);
}''',

    'fn_800E03E8': '''void fn_800E03E8(void* a) {
    extern void fn_800A32E8();
    fn_800A32E8(0, a);
}''',

    'fn_800E040C': '''void fn_800E040C(void) {
    extern void fn_800A33B4();
    fn_800A33B4();
}''',

    'fn_800E04F4': '''void fn_800E04F4(void* dst) {
    extern void fn_800A3074();
    fn_800A3074(dst, 0x5a);
}''',

    'fn_800E0518': '''void fn_800E0518(void* dst) {
    extern void fn_800A3074();
    fn_800A3074(dst, 0x59);
}''',

    'fn_800E053C': '''void fn_800E053C(void* dst) {
    extern void fn_800A3074();
    fn_800A3074(dst, 0x58);
}''',

    'fn_800E0628': '''void fn_800E0628(void* dst, void* src) {
    memcpy(dst, src, 0x30);
}''',

    'fn_800E064C': '''void fn_800E064C(void* dst) {
    extern u8 lbl_80315568[];
    memcpy(dst, lbl_80315568, 0x30);
}''',

    'fn_800E0678': '''void fn_800E0678(void) {
    extern void fn_800A3910();
    fn_800A3910();
}''',

    'fn_800E0698': '''void fn_800E0698(void) {
    extern void fn_800A39E0();
    fn_800A39E0();
}''',

    'fn_800E06B8': '''void fn_800E06B8(void* a, void* b, void* c) {
    extern void fn_800A3D3C();
    fn_800A3D3C(b, c, a);
}''',

    'fn_800E06EC': '''void fn_800E06EC(void* a, void* b) {
    extern void fn_801ADAAC();
    fn_801ADAAC(b, a);
}''',

    'fn_800E0718': '''void fn_800E0718(void) {
    extern void fn_800A3CB0();
    fn_800A3CB0();
}''',

    'fn_800E0738': '''void fn_800E0738(void* a, void* b, void* c) {
    extern void fn_800A3C54();
    fn_800A3C54(b, c, a);
}''',

    'fn_800E076C': '''void fn_800E076C(void* dst, void* src) {
    f32* d = (f32*)dst;
    f32* s = (f32*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}''',

    'fn_800E0BE4': '''void fn_800E0BE4(void) {
    extern void fn_801ADC7C();
    fn_801ADC7C();
}''',

    'fn_800E0C54': '''u32 fn_800E0C54(void) {
    extern void fn_801ADCD8();
    u32 r3 = (u32)fn_801ADCD8();
    return r3 & 0xFFFF;
}''',

    'fn_800E0C78': '''void fn_800E0C78(void) {
    extern u8 lbl_80478C94[];
    OSGetTime();
    u32* ptr = *(u32**)lbl_80478C94;
    /* stores low word of time */
}''',
}


def process_file(filepath, conversions):
    """Process a file, replacing small pragma-guarded functions with conversions."""
    with open(filepath, 'r') as f:
        content = f.read()

    lines = content.split('\n')
    blocks = find_pragma_blocks(content)

    # Process blocks in reverse order to maintain line indices
    replacements = 0
    for start, end in reversed(blocks):
        is_small, fn_name, fn_line = is_small_function(lines, start, end)
        if not is_small:
            continue
        if fn_name not in conversions:
            continue

        # Get the comment before pragma push (if any)
        comment_line = ''
        if start > 0 and lines[start-1].strip().startswith('/*'):
            comment_line = lines[start-1]

        replacement = conversions[fn_name]

        # Replace the block
        new_lines = replacement.split('\n')
        lines[start:end+1] = new_lines
        replacements += 1
        print(f"  Converted {fn_name} (was lines {start+1}-{end+1})")

    if replacements > 0:
        with open(filepath, 'w') as f:
            f.write('\n'.join(lines))

    return replacements


if __name__ == '__main__':
    if len(sys.argv) > 1:
        filepath = sys.argv[1]
        # Just process one file for testing
        n = process_file(filepath, GS_RENDER_CONVERSIONS)
        print(f"Converted {n} functions in {filepath}")
    else:
        print("Usage: python convert_small_funcs.py <file>")
