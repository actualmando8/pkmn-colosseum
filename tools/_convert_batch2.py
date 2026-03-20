#!/usr/bin/env python3
"""Convert 24-byte and 28-byte stubs."""
import re

with open('src/game/gs_field_world.c', 'r') as f:
    content = f.read()

count_before = len(re.findall(r'TODO: match', content))

# ============================================================
# 24-byte functions: null-check getter pattern (in accessor block)
# ============================================================
# These are inside the #pragma push block for GSfield_GetObj* functions
# fn_8011E4D8 through fn_8011E538

accessors = [
    ('8011E4D8', '0x8', 'u16'),   # GSfield_GetObjType
    ('8011E4F0', '0x2', 'u8'),    # GSfield_GetObjSubtype
    ('8011E508', '0x1', 'u8'),    # GSfield_GetObjFlags
    ('8011E520', '0x0', 'u8'),    # GSfield_GetObjState
    ('8011E538', '0x6', 'u16'),   # GSfield_GetObjGroupId
]

for addr, offset, rtype in accessors:
    old_str = "    /* TODO: match -- 24 bytes at 0x" + addr + " */"
    new_str = "    if (obj == NULL) { return 0; }\n    return *(" + rtype + "*)((u8*)obj + " + offset + ");"
    if old_str in content:
        content = content.replace(old_str, new_str)
        print("Converted fn_" + addr)
    else:
        print("NOT FOUND: fn_" + addr)

# fn_80116E6C (24 bytes): rlwimi bit insert
# Sets bit 7 of byte[0] based on val bit 0
old = """/* 0x80116E6C | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116E6C(void) {
    /* TODO: match -- 24 bytes at 0x80116E6C */
}
#pragma pop"""
new = """/* 0x80116E6C | 0x18 */
void fn_80116E6C(u8* ptr, u8 val) {
    u8 tmp;
    if (ptr == NULL) { return; }
    tmp = ptr[0];
    tmp = (u8)((tmp & ~0x80) | ((val & 1) << 7));
    ptr[0] = tmp;
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_80116E6C")
else:
    print("NOT FOUND: fn_80116E6C")

# fn_8012B184 (24 bytes): cmpwi r3,0; bltlr; store to global+0x188
old = """/* 0x8012B184 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8012B184(void) {
    /* TODO: match -- 24 bytes at 0x8012B184 */
}
#pragma pop"""
new = """/* 0x8012B184 | 0x18 */
extern u8 lbl_80426BD0[];
void fn_8012B184(s32 val) {
    if (val < 0) { return; }
    *(u32*)(lbl_80426BD0 + 0x188) = (u32)val;
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_8012B184")
else:
    print("NOT FOUND: fn_8012B184")

# ============================================================
# 28-byte functions: bit extract patterns
# ============================================================

# fn_80116EF8: extrwi r3, r0, 1, 27 -> (r0 >> 4) & 1
old = """/* 0x80116EF8 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116EF8(void) {
    /* TODO: match -- 28 bytes at 0x80116EF8 */
}
#pragma pop"""
new = """/* 0x80116EF8 | 0x1C */
u32 fn_80116EF8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[0] >> 4) & 1);
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_80116EF8")
else:
    print("NOT FOUND: fn_80116EF8")

# fn_80116F14: extrwi r3, r0, 2, 26 -> (ptr[1] >> 4) & 3
old = """/* 0x80116F14 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116F14(void) {
    /* TODO: match -- 28 bytes at 0x80116F14 */
}
#pragma pop"""
new = """/* 0x80116F14 | 0x1C */
u32 fn_80116F14(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[1] >> 4) & 3);
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_80116F14")
else:
    print("NOT FOUND: fn_80116F14")

# fn_80116F30: extrwi r3, r0, 2, 24 -> (ptr[1] >> 6) & 3
old = """/* 0x80116F30 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116F30(void) {
    /* TODO: match -- 28 bytes at 0x80116F30 */
}
#pragma pop"""
new = """/* 0x80116F30 | 0x1C */
u32 fn_80116F30(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[1] >> 6) & 3);
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_80116F30")
else:
    print("NOT FOUND: fn_80116F30")

# fn_80116F4C: clrlwi r3, r0, 29 -> ptr[0] & 7
old = """/* 0x80116F4C | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80116F4C(void) {
    /* TODO: match -- 28 bytes at 0x80116F4C */
}
#pragma pop"""
new = """/* 0x80116F4C | 0x1C */
u32 fn_80116F4C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)(ptr[0] & 7);
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_80116F4C")
else:
    print("NOT FOUND: fn_80116F4C")

# fn_80117038: extrwi r3, r0, 1, 25 -> (ptr[0] >> 6) & 1
old = """/* 0x80117038 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117038(void) {
    /* TODO: match -- 28 bytes at 0x80117038 */
}
#pragma pop"""
new = """/* 0x80117038 | 0x1C */
u32 fn_80117038(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[0] >> 6) & 1);
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_80117038")
else:
    print("NOT FOUND: fn_80117038")

# fn_80117054: extrwi r3, r0, 1, 24 -> (ptr[0] >> 7) & 1
old = """/* 0x80117054 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80117054(void) {
    /* TODO: match -- 28 bytes at 0x80117054 */
}
#pragma pop"""
new = """/* 0x80117054 | 0x1C */
u32 fn_80117054(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u32)((ptr[0] >> 7) & 1);
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_80117054")
else:
    print("NOT FOUND: fn_80117054")

# fn_8011D8D8: clamp val to 0x639C, store to offset 0xDC
old = """/* 0x8011D8D8 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011D8D8(void) {
    /* TODO: match -- 28 bytes at 0x8011D8D8 */
}
#pragma pop"""
new = """/* 0x8011D8D8 | 0x1C */
void fn_8011D8D8(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val >= 0x639C) { val = 0x639C; }
    *(u32*)(&ptr[0xDC]) = (u32)val;
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_8011D8D8")
else:
    print("NOT FOUND: fn_8011D8D8")

# fn_8011E7A4: lwz + clrlwi 24 -> load u32, return low byte
old = """/* 0x8011E7A4 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011E7A4(void) {
    /* TODO: match -- 28 bytes at 0x8011E7A4 */
}
#pragma pop"""
new = """/* 0x8011E7A4 | 0x1C */
u8 fn_8011E7A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u8)(*(u32*)(&ptr[0xF8]));
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_8011E7A4")
else:
    print("NOT FOUND: fn_8011E7A4")

# fn_8011F5E0: copy u32 from src to dst
old = """/* 0x8011F5E0 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011F5E0(void) {
    /* TODO: match -- 28 bytes at 0x8011F5E0 */
}
#pragma pop"""
new = """/* 0x8011F5E0 | 0x1C */
void fn_8011F5E0(u32* dst, u32* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    *dst = *src;
}"""
if old in content:
    content = content.replace(old, new)
    print("Converted fn_8011F5E0")
else:
    print("NOT FOUND: fn_8011F5E0")

# ============================================================
# 28-byte functions: pointer offset patterns
# ============================================================

offset_funcs = [
    ('80128CC0', 0x1C45C),
    ('80128CDC', 0x1B014),
    ('80128CF8', 0xE3E8),
    ('80128D14', 0x9A1C),
    ('80128D30', 0x82A8),
    ('80128D4C', 0x8168),
    ('80128D80', 0x1BE5C),
    ('80128D9C', 0x1BDDC),
    ('80128DB8', 0x1BDBC),
]

for addr, offset in offset_funcs:
    old = "/* 0x" + addr + " | 0x1C */\n"
    old += "#pragma push\n"
    old += "#pragma optimization_level 0\n"
    old += "#pragma optimizewithasm off\n"
    old += "void fn_" + addr + "(void) {\n"
    old += "    /* TODO: match -- 28 bytes at 0x" + addr + " */\n"
    old += "}\n"
    old += "#pragma pop"

    new = "/* 0x" + addr + " | 0x1C */\n"
    new += "void* fn_" + addr + "(void* ptr) {\n"
    new += "    if (ptr == NULL) { return NULL; }\n"
    new += "    return (u8*)ptr + 0x" + format(offset, 'X') + ";\n"
    new += "}"

    if old in content:
        content = content.replace(old, new)
        print("Converted fn_" + addr)
    else:
        print("NOT FOUND: fn_" + addr)

count_after = len(re.findall(r'TODO: match', content))
print("\nStubs after: " + str(count_after) + " (converted " + str(count_before - count_after) + ")")

with open('src/game/gs_field_world.c', 'w') as f:
    f.write(content)
