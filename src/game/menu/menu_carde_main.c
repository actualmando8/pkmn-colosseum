/**
 * @file menu_carde_main.c
 * @brief Card-E main handlers (0x8007FD64-0x80082650)
 *
 * Address range: 0x8007FD64 - 0x80082650
 * Total functions: 5
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_80083AF4();
extern void fn_80083BF8();
extern void fn_800CA620();
extern void fn_800E202C();
extern void fn_800E209C();
extern void fn_800E24B0();
extern void fn_800E27B0();
extern void fn_800E2C04();
extern void fn_800F9D24();
extern void fn_800F9E70();
extern void fn_801046C8();
extern void fn_80196E10();
extern void* memset(void* dst, int val, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478948;
extern u8 lbl_8047C140;
extern u8 lbl_8047C178;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EE890[];
extern u8 jumptable_802EE924[];
extern u8 jumptable_802EE9B8[];
extern u8 jumptable_802EEA4C[];
extern u8 lbl_80268B88[];
extern u8 lbl_80268DC0[];
extern u8 lbl_80269B68[];
extern u8 lbl_8026C7F8[];

/* ===== Forward declarations ===== */
s32 fn_8007FD64(void);
s32 fn_8007FDBC(void);
s32 fn_80080310(void);
s32 fn_80080ED8(void);
s32 fn_8008102C(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x8007FD64 | size: 0x58 */
s32 fn_8007FD64(void) {
    /* TODO: decompile (0x58 bytes) */
    return -1;
}

/* 0x8007FDBC | size: 0x554 */
s32 fn_8007FDBC(void) {
    /* TODO: decompile (0x554 bytes, ~341 instructions) */
    /* Uses many saved registers */
    fn_800E2C04();
    fn_80196E10();
    fn_800E27B0();
    fn_800F9D24();
    fn_800E202C();
    fn_800E24B0();
    fn_800E209C();
    fn_80083BF8();
    fn_80083AF4();
    fn_800CA620();
    fn_801046C8();
    return 0;
}

/* 0x80080310 | size: 0xBC8 */
s32 fn_80080310(void) {
    /* TODO: decompile (0xBC8 bytes, ~754 instructions) */
    /* Uses many saved registers */
    fn_8008102C();
    return 0;
}

/* 0x80080ED8 | size: 0x154 */
s32 fn_80080ED8(void) {
    /* TODO: decompile (0x154 bytes) */
    return 0;
}

/* 0x8008102C | size: 0x1624 */
s32 fn_8008102C(void) {
    /* TODO: decompile (0x1624 bytes, ~1417 instructions) */
    /* Contains switch/jump table */
    fn_800F9E70();
    fn_80196E10();
    return 0;
}

#pragma pop
