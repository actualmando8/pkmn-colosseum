#!/usr/bin/env python3
"""Convert remaining 52-60 byte stubs."""
import re

with open('src/game/gs_field_world.c', 'r') as f:
    content = f.read()

count_before = len(re.findall(r'TODO: match', content))

def replace_stub(addr, size_hex, old_sig, new_code):
    global content
    size_dec = int(size_hex, 16)
    old = "/* 0x" + addr + " | 0x" + size_hex + " */\n"
    old += "#pragma push\n"
    old += "#pragma optimization_level 0\n"
    old += "#pragma optimizewithasm off\n"
    old += old_sig + " {\n"
    old += "    /* TODO: match -- " + str(size_dec) + " bytes at 0x" + addr + " */\n"
    old += "}\n"
    old += "#pragma pop"
    new = "/* 0x" + addr + " | 0x" + size_hex + " */\n" + new_code
    if old in content:
        content = content.replace(old, new)
        print("Converted fn_" + addr)
        return True
    else:
        print("NOT FOUND: fn_" + addr)
        return False

# ============================================================
# Remaining 52-byte: indexed sub-struct access, stride 4, with offset
# Pattern: mr. r5, r3; null check; clrlwi idx; bounds check; clrlslwi *4; addi offset; add base
# ============================================================

indexed_sub4 = [
    ('8012AA9C', '0xA', '0xA9A'),
    ('8012AAD0', '0x3', '0xA74'),
    ('8012AB04', '0x2E', '0x9BC'),
    ('8012AB38', '0x40', '0x8BC'),
    ('8012AB6C', '0x10', '0x87C'),
    ('8012ABA0', '0x2B', '0x7D0'),
    ('8012ABD4', '0x14', '0x780'),
]

for addr, max_val, base_off in indexed_sub4:
    replace_stub(addr, '34', 'void fn_' + addr + '(void)',
        'void* fn_' + addr + '(u8* ptr, u16 idx) {\n'
        '    if (ptr == NULL) { return NULL; }\n'
        '    if ((u16)idx >= ' + max_val + ') { return NULL; }\n'
        '    return ptr + (u32)(u16)idx * 4 + ' + base_off + ';\n'
        '}')

# fn_8012AC08: idx < 6, stride 0x138 (mulli), offset 0x30
replace_stub('8012AC08', '34', 'void fn_8012AC08(void)',
    'void* fn_8012AC08(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return NULL; }\n'
    '    if ((u16)idx >= 6) { return NULL; }\n'
    '    return ptr + (u32)(u16)idx * 0x138 + 0x30;\n'
    '}')

# fn_8012F11C: range check [0,2), index into global, stride 32, read u16 bit 0
replace_stub('8012F11C', '34', 'void fn_8012F11C(void)',
    'u32 fn_8012F11C(s32 idx) {\n'
    '    u16 val;\n'
    '    if (idx < 0 || idx >= 2) { return 0; }\n'
    '    val = *(u16*)(lbl_80426BD0 + (u32)idx * 32 + 4);\n'
    '    return (u32)(val & 1);\n'
    '}')

# ============================================================
# 56-byte functions
# ============================================================

# fn_8011711C: call chain fn_800FF56C -> fn_80115C48 -> fn_80115684
replace_stub('8011711C', '38', 'void fn_8011711C(void)',
    'extern void fn_80115C48(void);\nextern void fn_80115684(void*, u32);\n'
    'void fn_8011711C(u32 arg) {\n'
    '    fn_800FF56C();\n'
    '    fn_80115C48();\n'
    '    fn_80115684(NULL, arg);\n'
    '}')

# fn_80118DA8: check byte at ptr[1], if 1 return -1, else call fn_801694E0(ptr[0x10])
replace_stub('80118DA8', '38', 'void fn_80118DA8(void)',
    'extern s32 fn_801694E0(u32);\n'
    's32 fn_80118DA8(u8* ptr) {\n'
    '    if (ptr[1] == 1) { return -1; }\n'
    '    return fn_801694E0(*(u32*)(&ptr[0x10]));\n'
    '}')

# fn_8011F1B8, F1F0, F228: call fn_8011F260 then read field - depend on fn_8011F260 signature
# These call fn_8011F260(r3, r4, 1) then read a byte/halfword from result
# Since fn_8011F260 is declared void(void), we skip these for now
# Actually, let me just leave them as stubs since they depend on fn_8011F260

# fn_8011F5FC: copy loop, 0x27 iterations of 8 bytes = 312 bytes total
# It's essentially memcpy of 0x27*8 = 0x138 bytes
replace_stub('8011F5FC', '38', 'void fn_8011F5FC(void)',
    'void fn_8011F5FC(u32* dst, u32* src) {\n'
    '    s32 i;\n'
    '    if (dst == NULL) { return; }\n'
    '    if (src == NULL) { return; }\n'
    '    for (i = 0; i < 0x27; i++) {\n'
    '        dst[i * 2] = src[i * 2];\n'
    '        dst[i * 2 + 1] = src[i * 2 + 1];\n'
    '    }\n'
    '}')

# fn_8012A89C: null-check both args, call fn_800F9D24(ptr+0xAC2, src, 0xB)
replace_stub('8012A89C', '38', 'void fn_8012A89C(void)',
    'extern void fn_800F9D24(void*, void*, u32);\n'
    'void fn_8012A89C(u8* ptr, void* src) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if (src == NULL) { return; }\n'
    '    fn_800F9D24(ptr + 0xAC2, src, 0xB);\n'
    '}')

# fn_8012AA64: null-check both, call fn_800F9D24(r3, r4, 0xB)
replace_stub('8012AA64', '38', 'void fn_8012AA64(void)',
    'void fn_8012AA64(void* dst, void* src) {\n'
    '    if (dst == NULL) { return; }\n'
    '    if (src == NULL) { return; }\n'
    '    fn_800F9D24(dst, src, 0xB);\n'
    '}')

# fn_8012AC64: copy loop, 0x163 iterations of 8 bytes = 0xB18 bytes
replace_stub('8012AC64', '38', 'void fn_8012AC64(void)',
    'void fn_8012AC64(u32* dst, u32* src) {\n'
    '    s32 i;\n'
    '    if (dst == NULL) { return; }\n'
    '    if (src == NULL) { return; }\n'
    '    for (i = 0; i < 0x163; i++) {\n'
    '        dst[i * 2] = src[i * 2];\n'
    '        dst[i * 2 + 1] = src[i * 2 + 1];\n'
    '    }\n'
    '}')

# ============================================================
# 60-byte functions - read the rest from assembly output
# ============================================================

# fn_8011CA60: null-check, u16 idx < 3, return sub-struct
# Let me check the assembly for these

count_after = len(re.findall(r'TODO: match', content))
print("\nStubs after: " + str(count_after) + " (converted " + str(count_before - count_after) + ")")

with open('src/game/gs_field_world.c', 'w') as f:
    f.write(content)
