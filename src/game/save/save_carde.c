/**
 * @file save_carde.c
 * @brief Card-E save data validation (0x80082650-0x80083AF4)
 *
 * Address range: 0x80082650 - 0x80083AF4
 * Total functions: 11
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_800CAA3C();
extern void fn_80129280();
extern void fn_80196E10();
extern void fn_801EE10C();
extern void fn_801EE1E0();
extern void fn_801EE2B4();
extern void* memset(void* dst, int val, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_8047C180;
extern u8 lbl_8047C188;

/* ===== Rodata / data labels ===== */
extern u8 lbl_8026F1C8[];
extern u8 lbl_8026F1D8[];

/* ===== Forward declarations ===== */
void fn_80082650(void);
s32 fn_80082738(void);
s32 fn_80082960(void);
s32 fn_80082A88(void);
s32 fn_80082BA4(void);
s32 fn_80082CF0(void);
void fn_80082EA4(void);
void fn_80082FE4(void);
s32 fn_800830A4(void);
s32 fn_800832C8(void);
s32 fn_800836AC(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80082650 | size: 0xE8 */
void fn_80082650(void) {
    /* TODO: decompile (0xE8 bytes) */
    fn_80196E10();
}

/* 0x80082738 | size: 0x228 */
s32 fn_80082738(void) {
    /* TODO: decompile (0x228 bytes, ~138 instructions) */
    /* Uses many saved registers */
    fn_80196E10();
    return 0;
}

/* 0x80082960 | size: 0x128 */
s32 fn_80082960(void) {
    /* TODO: decompile (0x128 bytes) */
    fn_80196E10();
    return 0;
}

/* 0x80082A88 | size: 0x11C */
s32 fn_80082A88(void) {
    /* TODO: decompile (0x11C bytes) */
    fn_80196E10();
    return 0;
}

/* 0x80082BA4 | size: 0x14C */
s32 fn_80082BA4(void) {
    /* TODO: decompile (0x14C bytes) */
    fn_80196E10();
    fn_800CAA3C();
    return 0;
}

/* 0x80082CF0 | size: 0x1B4 */
s32 fn_80082CF0(void) {
    /* TODO: decompile (0x1B4 bytes) */
    fn_80196E10();
    fn_800CAA3C();
    return 0;
}

/* 0x80082EA4 | size: 0x140 */
void fn_80082EA4(void) {
    /* TODO: decompile (0x140 bytes) */
    fn_80196E10();
}

/* 0x80082FE4 | size: 0xC0 */
void fn_80082FE4(void) {
    /* TODO: decompile (0xC0 bytes) */
    fn_80196E10();
}

/* 0x800830A4 | size: 0x224 */
s32 fn_800830A4(void) {
    /* TODO: decompile (0x224 bytes, ~137 instructions) */
    /* Uses many saved registers */
    fn_80129280();
    fn_80196E10();
    return 0;
}

/* 0x800832C8 | size: 0x3E4 */
s32 fn_800832C8(void) {
    /* TODO: decompile (0x3E4 bytes, ~249 instructions) */
    /* Uses many saved registers */
    fn_80129280();
    fn_80196E10();
    fn_800CAA3C();
    fn_801EE1E0();
    fn_801EE2B4();
    fn_801EE10C();
    return -1;
}

/* 0x800836AC | size: 0x448 */
s32 fn_800836AC(void) {
    /* TODO: decompile (0x448 bytes, ~274 instructions) */
    /* Uses many saved registers */
    fn_80129280();
    fn_800CAA3C();
    fn_80196E10();
    return 0;
}

#pragma pop
