/**
 * @file gba_conv2.c
 * @brief Pokemon conversion GBA-GCN (0x80089048-0x800895A4)
 *
 * Address range: 0x80089048 - 0x800895A4
 * Total functions: 2
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8008AE18();
extern void fn_8011F5C8();
extern void fn_80123FBC();
extern void fn_80196E10();
extern void fn_80265F14();
extern void* memset(void* dst, int val, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_8047A660;
extern u8 lbl_8047A664;
extern u8 lbl_8047A668;
extern u8 lbl_8047A66C;

/* ===== Rodata / data labels ===== */
extern u8 lbl_8026F568[];
extern u8 lbl_8026F574[];

/* ===== Forward declarations ===== */
s32 fn_80089048(void);
s32 fn_80089380(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80089048 | size: 0x338 */
s32 fn_80089048(void) {
    /* TODO: decompile (0x338 bytes, ~206 instructions) */
    fn_80123FBC();
    fn_8011F5C8();
    fn_80196E10();
    fn_8008AE18();
    fn_80265F14();
    return 0;
}

/* 0x80089380 | size: 0x224 */
s32 fn_80089380(void) {
    /* TODO: decompile (0x224 bytes, ~137 instructions) */
    fn_80196E10();
    return 0;
}

#pragma pop
