#!/usr/bin/env python3
"""Convert 32-60 byte stubs based on assembly analysis."""
import re

with open('src/game/gs_field_world.c', 'r') as f:
    content = f.read()

count_before = len(re.findall(r'TODO: match', content))

def replace_stub(addr, size_hex, old_sig, new_code):
    """Replace a pragma stub with real code."""
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
# 32-byte: indexed array setters with bounds check
# Pattern: null-check ptr; clrlwi idx; cmplwi idx, MAX; bgelr; add ptr+idx; stb val
# ============================================================

# fn_8011C430: ptr[idx + 0x34] = val, idx < 3
replace_stub('8011C430', '20', 'void fn_8011C430(void)',
    'void fn_8011C430(u8* ptr, u16 idx, u8 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)idx >= 3) { return; }\n'
    '    ptr[(u16)idx + 0x34] = val;\n'
    '}')

# fn_8011CED0: ptr[idx + 0x6E] = val, idx < 2
replace_stub('8011CED0', '20', 'void fn_8011CED0(void)',
    'void fn_8011CED0(u8* ptr, u16 idx, u8 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)idx >= 2) { return; }\n'
    '    ptr[(u16)idx + 0x6E] = val;\n'
    '}')

# fn_8011D0AC: ptr[idx + 0x34] = val, idx < 0x3A
replace_stub('8011D0AC', '20', 'void fn_8011D0AC(void)',
    'void fn_8011D0AC(u8* ptr, u16 idx, u8 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)idx >= 0x3A) { return; }\n'
    '    ptr[(u16)idx + 0x34] = val;\n'
    '}')

# fn_8011D20C: ptr[idx + 0x32] = val, idx < 2
replace_stub('8011D20C', '20', 'void fn_8011D20C(void)',
    'void fn_8011D20C(u8* ptr, u16 idx, u8 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)idx >= 2) { return; }\n'
    '    ptr[(u16)idx + 0x32] = val;\n'
    '}')

# fn_8011D22C: ptr[idx + 0x30] = val, idx < 2
replace_stub('8011D22C', '20', 'void fn_8011D22C(void)',
    'void fn_8011D22C(u8* ptr, u16 idx, u8 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)idx >= 2) { return; }\n'
    '    ptr[(u16)idx + 0x30] = val;\n'
    '}')

# fn_8011D904: clamp u16 to 0xFF, store as u16 at 0xB0
replace_stub('8011D904', '20', 'void fn_8011D904(void)',
    'void fn_8011D904(u8* ptr, u16 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)val > 0xFF) { val = 0xFF; }\n'
    '    *(u16*)(&ptr[0xB0]) = val;\n'
    '}')

# fn_8011DE48: clamp u8 to 0x64, store as u8 at 0x60
replace_stub('8011DE48', '20', 'void fn_8011DE48(void)',
    'void fn_8011DE48(u8* ptr, u8 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u8)val > 0x64) { val = 0x64; }\n'
    '    ptr[0x60] = val;\n'
    '}')

# fn_8011DE68: clamp u16 to 0xFF, store as u16 at 0xE4
replace_stub('8011DE68', '20', 'void fn_8011DE68(void)',
    'void fn_8011DE68(u8* ptr, u16 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)val > 0xFF) { val = 0xFF; }\n'
    '    *(u16*)(&ptr[0xE4]) = val;\n'
    '}')

# fn_8012BAD0: store 5 u32s to global struct
replace_stub('8012BAD0', '20', 'void fn_8012BAD0(void)',
    'void fn_8012BAD0(u32 a, u32 b, u32 c, u32 d, u32 e) {\n'
    '    u8* base = lbl_80426BD0;\n'
    '    *(u32*)(base + 0x18C) = a;\n'
    '    *(u32*)(base + 0x190) = b;\n'
    '    *(u32*)(base + 0x194) = c;\n'
    '    *(u32*)(base + 0x198) = d;\n'
    '    *(u32*)(base + 0x19C) = e;\n'
    '}')

# ============================================================
# 36-byte: indexed array setters with shift (store u16)
# ============================================================

# fn_8011CEF0: idx < 8, store u16 at ptr + idx*2 + 0x74
replace_stub('8011CEF0', '24', 'void fn_8011CEF0(void)',
    'void fn_8011CEF0(u8* ptr, u16 idx, u16 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)idx >= 8) { return; }\n'
    '    *(u16*)(&ptr[(u16)idx * 2 + 0x74]) = val;\n'
    '}')

# fn_8011D24C: idx < 2, store u16 at ptr + idx*2 + 0x70
replace_stub('8011D24C', '24', 'void fn_8011D24C(void)',
    'void fn_8011D24C(u8* ptr, u16 idx, u16 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if ((u16)idx >= 2) { return; }\n'
    '    *(u16*)(&ptr[(u16)idx * 2 + 0x70]) = val;\n'
    '}')

# ============================================================
# 40-byte functions
# ============================================================

# fn_801174C4: check two globals, return 1 if both non-zero
replace_stub('801174C4', '28', 'void fn_801174C4(void)',
    'u8 fn_801174C4(void) {\n'
    '    u8 result = 0;\n'
    '    if (lbl_8047AD68 != 0 && lbl_8047AD6C != 0) {\n'
    '        result = 1;\n'
    '    }\n'
    '    return result;\n'
    '}')

# fn_8011CAB8: index into array via SDA globals
replace_stub('8011CAB8', '28', 'void fn_8011CAB8(void)',
    'extern u32 lbl_80478E68;\nextern u32 lbl_80478E6C;\n'
    'void* fn_8011CAB8(u16 idx) {\n'
    '    u32* hdr = (u32*)lbl_80478E68;\n'
    '    if ((u16)idx >= hdr[0]) { return NULL; }\n'
    '    return (u8*)lbl_80478E6C + (u16)idx;\n'
    '}')

# ============================================================
# 44-byte functions
# ============================================================

# fn_80116E84: copy Vec3f from src to dst+0x18
replace_stub('80116E84', '2C', 'void fn_80116E84(void)',
    'void fn_80116E84(u8* dst, f32* src) {\n'
    '    if (dst == NULL) { return; }\n'
    '    if (src == NULL) { return; }\n'
    '    *(f32*)(&dst[0x18]) = src[0];\n'
    '    *(f32*)(&dst[0x1C]) = src[1];\n'
    '    *(f32*)(&dst[0x20]) = src[2];\n'
    '}')

# fn_8011CA34: index into array via SDA globals, stride 0x38
replace_stub('8011CA34', '2C', 'void fn_8011CA34(void)',
    'extern u32 lbl_80478DF8;\nextern u32 lbl_80478DFC;\n'
    'void* fn_8011CA34(u16 idx) {\n'
    '    u32* hdr = (u32*)lbl_80478DF8;\n'
    '    if ((u16)idx >= hdr[0]) {\n'
    '        return (void*)lbl_80478DFC;\n'
    '    }\n'
    '    return (u8*)lbl_80478DFC + (u32)(u16)idx * 0x38;\n'
    '}')

# fn_8011CB10: index into BSS array, stride 3
replace_stub('8011CB10', '2C', 'void fn_8011CB10(void)',
    'extern u32 lbl_80478B70;\nextern u8 lbl_8035F988[];\n'
    'void* fn_8011CB10(u16 idx) {\n'
    '    if ((u16)idx >= lbl_80478B70) { return NULL; }\n'
    '    return (u8*)lbl_8035F988 + (u32)(u16)idx * 3;\n'
    '}')

# fn_8011CB6C: index into BSS array, stride 0xC
replace_stub('8011CB6C', '2C', 'void fn_8011CB6C(void)',
    'extern u32 lbl_80478B68;\nextern u8 lbl_8035F5E0[];\n'
    'void* fn_8011CB6C(u16 idx) {\n'
    '    if ((u16)idx >= lbl_80478B68) { return NULL; }\n'
    '    return (u8*)lbl_8035F5E0 + (u32)(u16)idx * 0xC;\n'
    '}')

# fn_8011CBC8: index via SDA globals, u8 idx, stride 2
replace_stub('8011CBC8', '2C', 'void fn_8011CBC8(void)',
    'extern u32 lbl_80478E58;\nextern u32 lbl_80478E5C;\n'
    'void* fn_8011CBC8(u8 idx) {\n'
    '    u32* hdr = (u32*)lbl_80478E58;\n'
    '    if ((u8)idx >= hdr[0]) { return NULL; }\n'
    '    return (u8*)lbl_80478E5C + (u32)(u8)idx * 2;\n'
    '}')

# fn_8011CE18: index via SDA globals, u8 idx, stride 0x28
replace_stub('8011CE18', '2C', 'void fn_8011CE18(void)',
    'extern u32 lbl_80478E60;\nextern u32 lbl_80478E64;\n'
    'void* fn_8011CE18(u8 idx) {\n'
    '    u32* hdr = (u32*)lbl_80478E60;\n'
    '    if ((u8)idx >= hdr[0]) { return NULL; }\n'
    '    return (u8*)lbl_80478E64 + (u32)(u8)idx * 0x28;\n'
    '}')

# fn_8011CE74: index into BSS array, u8 idx, stride 0x194
replace_stub('8011CE74', '2C', 'void fn_8011CE74(void)',
    'extern u32 lbl_80478B60;\nextern u8 lbl_8035E940[];\n'
    'void* fn_8011CE74(u8 idx) {\n'
    '    if ((u8)idx >= lbl_80478B60) { return NULL; }\n'
    '    return (u8*)lbl_8035E940 + (u32)(u8)idx * 0x194;\n'
    '}')

# fn_8011CF44: null-check then call fn_8019075C with ptr[0x28]
replace_stub('8011CF44', '2C', 'void fn_8011CF44(void)',
    'extern void fn_8019075C(u32);\n'
    'void fn_8011CF44(u8* ptr) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    fn_8019075C(*(u32*)(&ptr[0x28]));\n'
    '}')

# fn_8011CF70: null-check then call fn_8019075C with ptr[0x24]
replace_stub('8011CF70', '2C', 'void fn_8011CF70(void)',
    'void fn_8011CF70(u8* ptr) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    fn_8019075C(*(u32*)(&ptr[0x24]));\n'
    '}')

# fn_8011E550: double null-check, read u16 from sub-struct
# null-check ptr; ptr += 0x90; null-check again; return *(u16*)(ptr+0xA)
old_str = "    /* TODO: match -- 44 bytes at 0x8011E550 */"
new_str = ('    u8* sub;\n'
           '    if (obj == NULL) { return 0; }\n'
           '    sub = (u8*)obj + 0x90;\n'
           '    if (sub == NULL) { return 0; }\n'
           '    return *(u16*)(sub + 0xA);')
if old_str in content:
    content = content.replace(old_str, new_str)
    print("Converted fn_8011E550")

# fn_8011E778: index via SDA globals, u16 idx, stride 0x11C
replace_stub('8011E778', '2C', 'void fn_8011E778(void)',
    'extern u32 lbl_80478F90;\nextern u32 lbl_80478F94;\n'
    'void* fn_8011E778(u16 idx) {\n'
    '    u32* hdr = (u32*)lbl_80478F90;\n'
    '    if ((u16)idx >= hdr[0]) { return NULL; }\n'
    '    return (u8*)lbl_80478F94 + (u32)(u16)idx * 0x11C;\n'
    '}')

# ============================================================
# 48-byte functions
# ============================================================

# fn_8011C588: indexed array getter, idx < 3, return byte at ptr[idx + 0x34]
replace_stub('8011C588', '30', 'void fn_8011C588(void)',
    'u8 fn_8011C588(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u16)idx >= 3) { return 0; }\n'
    '    return ptr[(u16)idx + 0x34];\n'
    '}')

# fn_8011CAE0: null-check, u8 idx < 3, lbzx + extsb -> signed byte load
replace_stub('8011CAE0', '30', 'void fn_8011CAE0(void)',
    's8 fn_8011CAE0(u8* ptr, u8 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u8)idx >= 3) { return 0; }\n'
    '    return (s8)ptr[(u8)idx];\n'
    '}')

# fn_8011CBF4: null-check, u8 idx < 7, return byte at ptr[idx + 0x1F]
replace_stub('8011CBF4', '30', 'void fn_8011CBF4(void)',
    'u8 fn_8011CBF4(u8* ptr, u8 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u8)idx >= 7) { return 0; }\n'
    '    return ptr[(u8)idx + 0x1F];\n'
    '}')

# fn_8011CC24: null-check, u8 idx < 7, return byte at ptr[idx + 0x18]
replace_stub('8011CC24', '30', 'void fn_8011CC24(void)',
    'u8 fn_8011CC24(u8* ptr, u8 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u8)idx >= 7) { return 0; }\n'
    '    return ptr[(u8)idx + 0x18];\n'
    '}')

# fn_8011CE44: null-check, u8 idx < 0x65, return u32 at ptr[idx*4]
replace_stub('8011CE44', '30', 'void fn_8011CE44(void)',
    'u32 fn_8011CE44(u8* ptr, u8 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u8)idx >= 0x65) { return 0; }\n'
    '    return *(u32*)(&ptr[(u32)(u8)idx * 4]);\n'
    '}')

# fn_8011E048: indexed getter, u16 idx < 2, byte at ptr[idx + 0x6E]
replace_stub('8011E048', '30', 'void fn_8011E048(void)',
    'u8 fn_8011E048(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u16)idx >= 2) { return 0; }\n'
    '    return ptr[(u16)idx + 0x6E];\n'
    '}')

# fn_8011E2AC: indexed getter, u16 idx < 0x3A, byte at ptr[idx + 0x34]
replace_stub('8011E2AC', '30', 'void fn_8011E2AC(void)',
    'u8 fn_8011E2AC(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u16)idx >= 0x3A) { return 0; }\n'
    '    return ptr[(u16)idx + 0x34];\n'
    '}')

# fn_8011E444: indexed getter, u16 idx < 2, byte at ptr[idx + 0x32]
replace_stub('8011E444', '30', 'void fn_8011E444(void)',
    'u8 fn_8011E444(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u16)idx >= 2) { return 0; }\n'
    '    return ptr[(u16)idx + 0x32];\n'
    '}')

# fn_8011E474: indexed getter, u16 idx < 2, byte at ptr[idx + 0x30]
replace_stub('8011E474', '30', 'void fn_8011E474(void)',
    'u8 fn_8011E474(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u16)idx >= 2) { return 0; }\n'
    '    return ptr[(u16)idx + 0x30];\n'
    '}')

# fn_8011FC74: call GSfield_TransitionStateMachine with constants, return u8 result
replace_stub('8011FC74', '30', 'void fn_8011FC74(void)',
    'extern void GSfield_TransitionStateMachine(void);\n'
    'u8 fn_8011FC74(u32 arg) {\n'
    '    return (u8)fn_8012640C(arg, 0, 0xC2, 0);\n'
    '}')

# fn_801230E0: similar call pattern
replace_stub('801230E0', '30', 'void fn_801230E0(void)',
    'u16 fn_801230E0(u32 arg) {\n'
    '    return (u16)fn_8012640C(arg, 0, 0x82, 0);\n'
    '}')

# fn_8012A7DC: clamp s32 to [0, 0x9896FF], store at ptr+0xA8C
replace_stub('8012A7DC', '30', 'void fn_8012A7DC(void)',
    'void fn_8012A7DC(u8* ptr, s32 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if (val < 0) { val = 0; }\n'
    '    if (val > 0x9896FF) { val = 0x9896FF; }\n'
    '    *(s32*)(&ptr[0xA8C]) = val;\n'
    '}')

# fn_8012A824: same clamp, store at ptr+0xA88
replace_stub('8012A824', '30', 'void fn_8012A824(void)',
    'void fn_8012A824(u8* ptr, s32 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if (val < 0) { val = 0; }\n'
    '    if (val > 0x9896FF) { val = 0x9896FF; }\n'
    '    *(s32*)(&ptr[0xA88]) = val;\n'
    '}')

# fn_8012A86C: same clamp, store at ptr+0xA84
replace_stub('8012A86C', '30', 'void fn_8012A86C(void)',
    'void fn_8012A86C(u8* ptr, s32 val) {\n'
    '    if (ptr == NULL) { return; }\n'
    '    if (val < 0) { val = 0; }\n'
    '    if (val > 0x9896FF) { val = 0x9896FF; }\n'
    '    *(s32*)(&ptr[0xA84]) = val;\n'
    '}')

# ============================================================
# 52-byte functions: compound setter with sub-struct + clamp
# Pattern: null-check; ptr += offset; null-check; clamp val; store u16
# ============================================================

# fn_8011D924-DB60: these are compound setters with sub-struct at +0xA4 or +0x98
compound_a4 = [
    ('8011D924', '0xA4', '0x1F', '0xA'),
    ('8011D958', '0xA4', '0x1F', '0x8'),
    ('8011D98C', '0xA4', '0x1F', '0x6'),
    ('8011D9C0', '0xA4', '0x1F', '0x4'),
    ('8011D9F4', '0xA4', '0x1F', '0x2'),
    ('8011DA28', '0xA4', '0x1F', '0x0'),
]

compound_98 = [
    ('8011DA5C', '0x98', '0xFF', '0xA'),
    ('8011DA90', '0x98', '0xFF', '0x8'),
    ('8011DAC4', '0x98', '0xFF', '0x6'),
    ('8011DAF8', '0x98', '0xFF', '0x4'),
    ('8011DB2C', '0x98', '0xFF', '0x2'),
    ('8011DB60', '0x98', '0xFF', '0x0'),
]

for addr, sub_off, max_val, store_off in compound_a4 + compound_98:
    replace_stub(addr, '34', 'void fn_' + addr + '(void)',
        'void fn_' + addr + '(u8* ptr, u16 val) {\n'
        '    u8* sub;\n'
        '    if (ptr == NULL) { return; }\n'
        '    sub = ptr + ' + sub_off + ';\n'
        '    if (sub == NULL) { return; }\n'
        '    if ((u16)val > ' + max_val + ') { val = ' + max_val + '; }\n'
        '    *(u16*)(sub + ' + store_off + ') = val;\n'
        '}')

# fn_80117070: null-check, load u16 from ptr[0x6], call fn_8018F6CC
replace_stub('80117070', '34', 'void fn_80117070(void)',
    'extern void* fn_8018F6CC(u16);\n'
    'void* fn_80117070(u8* ptr) {\n'
    '    if (ptr == NULL) { return NULL; }\n'
    '    return fn_8018F6CC(*(u16*)(&ptr[0x6]));\n'
    '}')

# fn_80118F7C: load 3 floats from ptr[0x38,0x3C,0x40], call fn_800E01F4
replace_stub('80118F7C', '34', 'void fn_80118F7C(void)',
    'extern void fn_800E01F4(void*, f32, f32, f32);\n'
    'void fn_80118F7C(u8* obj, void* arg) {\n'
    '    f32 f1 = *(f32*)(&obj[0x38]);\n'
    '    f32 f2 = *(f32*)(&obj[0x3C]);\n'
    '    f32 f3 = *(f32*)(&obj[0x40]);\n'
    '    fn_800E01F4(arg, f1, f2, f3);\n'
    '}')

# fn_8011E078: indexed getter, u16 idx < 8, return u16 at ptr[idx*2 + 0x74]
replace_stub('8011E078', '34', 'void fn_8011E078(void)',
    'u16 fn_8011E078(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return 0; }\n'
    '    if ((u16)idx >= 8) { return 0; }\n'
    '    return *(u16*)(&ptr[(u16)idx * 2 + 0x74]);\n'
    '}')

# fn_8011E0F4: null-check, load ptr[0x28], call fn_801906A0
replace_stub('8011E0F4', '34', 'void fn_8011E0F4(void)',
    'extern void* fn_801906A0(u32);\n'
    'void* fn_8011E0F4(u8* ptr) {\n'
    '    if (ptr == NULL) { return NULL; }\n'
    '    return fn_801906A0(*(u32*)(&ptr[0x28]));\n'
    '}')

# fn_8011E128: null-check, load ptr[0x24], call fn_801906A0
replace_stub('8011E128', '34', 'void fn_8011E128(void)',
    'void* fn_8011E128(u8* ptr) {\n'
    '    if (ptr == NULL) { return NULL; }\n'
    '    return fn_801906A0(*(u32*)(&ptr[0x24]));\n'
    '}')

# fn_8011E4A4: indexed getter, u16 idx < 2, return u16 at ptr[idx*2 + 0x70]
old_str = "    /* TODO: match -- 52 bytes at 0x8011E4A4 */"
new_str = ('    if (obj == NULL) { return 0; }\n'
           '    if ((u16)slot >= 2) { return 0; }\n'
           '    return *(u16*)((u8*)obj + (u16)slot * 2 + 0x70);')
if old_str in content:
    content = content.replace(old_str, new_str)
    print("Converted fn_8011E4A4")

# fn_8011EDC4: mr. r5, r3; indexed sub-struct, stride 16, offset 0xE8
replace_stub('8011EDC4', '34', 'void fn_8011EDC4(void)',
    'void* fn_8011EDC4(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return NULL; }\n'
    '    if ((u16)idx >= 1) { return NULL; }\n'
    '    return ptr + (u32)(u16)idx * 16 + 0xE8;\n'
    '}')

# fn_8011F474: same pattern, offset 0x64
replace_stub('8011F474', '34', 'void fn_8011F474(void)',
    'void* fn_8011F474(u8* ptr, u16 idx) {\n'
    '    if (ptr == NULL) { return NULL; }\n'
    '    if ((u16)idx >= 1) { return NULL; }\n'
    '    return ptr + (u32)(u16)idx * 16 + 0x64;\n'
    '}')

# fn_801252E0: call transition + fn_8011B950
replace_stub('801252E0', '34', 'void fn_801252E0(void)',
    'extern void fn_8011B950(void);\n'
    'void fn_801252E0(u32 arg) {\n'
    '    fn_8012640C(arg, 0, 0x7C, 0);\n'
    '    fn_8011B950(arg, 1);\n'
    '}')

# fn_8011CA60, fn_80122334, and remaining 52/56/60 byte functions
# will be handled later

count_after = len(re.findall(r'TODO: match', content))
print("\nStubs after: " + str(count_after) + " (converted " + str(count_before - count_after) + ")")

with open('src/game/gs_field_world.c', 'w') as f:
    f.write(content)
