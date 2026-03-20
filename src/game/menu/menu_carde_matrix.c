/**
 * @file menu_carde_matrix.c
 * @brief Card-E matrix display (0x8007C300-0x8007FD64)
 *
 * Address range: 0x8007C300 - 0x8007FD64
 * Total functions: 15
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8005D858();
extern void fn_8007FDBC();
extern void fn_80082A88();
extern void fn_80082EA4();
extern void fn_80082FE4();
extern void fn_80083AF4();
extern void fn_80083BF8();
extern void fn_800CA620();
extern void fn_800E202C();
extern void fn_800E209C();
extern void fn_800E24B0();
extern void fn_800E27B0();
extern void fn_800E2C04();
extern void fn_800F0308();
extern void fn_800F9D24();
extern void fn_801040A0();
extern void fn_801040D0();
extern void fn_801046B8();
extern void fn_80104704();
extern void fn_80105624();
extern void fn_801081F8();
extern void fn_801091F4();
extern void fn_80109220();
extern void fn_80132A38();
extern void fn_80166A28();
extern void fn_80196E10();
extern void* memset(void* dst, int val, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_8047A658;
extern u8 lbl_8047C130;
extern u8 lbl_8047C138;
extern u8 lbl_8047C140;
extern u8 lbl_8047C148;
extern u8 lbl_8047C14C;
extern u8 lbl_8047C150;
extern u8 lbl_8047C154;
extern u8 lbl_8047C158;
extern u8 lbl_8047C15C;
extern u8 lbl_8047C160;
extern u8 lbl_8047C168;
extern u8 lbl_8047C170;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EE868[];
extern u8 lbl_80268B88[];
extern u8 lbl_80268D78[];
extern u8 lbl_80268D8C[];
extern u8 lbl_80268DA0[];
extern u8 lbl_80268DB4[];

/* ===== Forward declarations ===== */
s32 fn_8007C300(void);
s32 fn_8007C414(void);
s32 fn_8007C450(void);
s32 fn_8007C634(void);
s32 fn_8007C764(void);
s32 fn_8007C7A8(void);
s32 fn_8007C7EC(void);
s32 fn_8007CAB0(void);
s32 fn_8007CB54(void);
s32 fn_8007CBB4(void);
s32 fn_8007D4FC(void);
s32 fn_8007D564(void);
s32 fn_8007D79C(void);
s32 fn_8007D89C(void);
s32 fn_8007D978(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x8007C300 | size: 0x114 */
s32 fn_8007C300(void) {
    /* TODO: decompile (0x114 bytes) */
    fn_80104704();
    fn_801040A0();
    fn_800F0308();
    fn_80196E10();
    return 0;
}

/* 0x8007C414 | size: 0x3C */
s32 fn_8007C414(void) {
    /* TODO: decompile (0x3C bytes) */
    fn_80104704();
    fn_801040A0();
    return 0;
}

/* 0x8007C450 | size: 0x1E4 */
s32 fn_8007C450(void) {
    /* TODO: decompile (0x1E4 bytes) */
    fn_80104704();
    fn_801040A0();
    fn_800F0308();
    fn_80196E10();
    return 0;
}

/* 0x8007C634 | size: 0x130 */
s32 fn_8007C634(void) {
    /* TODO: decompile (0x130 bytes) */
    fn_80104704();
    fn_801040A0();
    fn_800F0308();
    return 0;
}

/* 0x8007C764 | size: 0x44 */
s32 fn_8007C764(void) {
    /* TODO: decompile (0x44 bytes) */
    fn_80104704();
    fn_801040A0();
    return 0;
}

/* 0x8007C7A8 | size: 0x44 */
s32 fn_8007C7A8(void) {
    /* TODO: decompile (0x44 bytes) */
    fn_80104704();
    fn_801040A0();
    return 0;
}

/* 0x8007C7EC | size: 0x2C4 */
s32 fn_8007C7EC(void) {
    /* TODO: decompile (0x2C4 bytes, ~177 instructions) */
    /* Uses many saved registers */
    fn_80104704();
    fn_801040A0();
    fn_80196E10();
    fn_800E202C();
    fn_800E24B0();
    fn_800E209C();
    fn_80083BF8();
    fn_800E2C04();
    fn_800E27B0();
    memset();
    fn_80083AF4();
    fn_800CA620();
    return 0;
}

/* 0x8007CAB0 | size: 0xA4 */
s32 fn_8007CAB0(void) {
    /* TODO: decompile (0xA4 bytes) */
    fn_80104704();
    fn_801040A0();
    return 0;
}

/* 0x8007CB54 | size: 0x60 */
s32 fn_8007CB54(void) {
    /* TODO: decompile (0x60 bytes) */
    fn_80104704();
    fn_801040A0();
    fn_800F9D24();
    return 0;
}

/* 0x8007CBB4 | size: 0x948 */
s32 fn_8007CBB4(void) {
    /* TODO: decompile (0x948 bytes, ~594 instructions) */
    /* Uses many saved registers */
    fn_80104704();
    fn_801040A0();
    fn_801091F4();
    fn_80082FE4();
    fn_8005D858();
    fn_80132A38();
    fn_80082EA4();
    return 0;
}

/* 0x8007D4FC | size: 0x68 */
s32 fn_8007D4FC(void) {
    /* TODO: decompile (0x68 bytes) */
    fn_80104704();
    fn_801040A0();
    fn_80132A38();
    return 0;
}

/* 0x8007D564 | size: 0x238 */
s32 fn_8007D564(void) {
    /* TODO: decompile (0x238 bytes, ~142 instructions) */
    fn_80104704();
    fn_801040A0();
    fn_80082FE4();
    fn_80132A38();
    return 0;
}

/* 0x8007D79C | size: 0x100 */
s32 fn_8007D79C(void) {
    /* TODO: decompile (0x100 bytes) */
    fn_80104704();
    fn_801040A0();
    fn_80082FE4();
    fn_80132A38();
    return 0;
}

/* 0x8007D89C | size: 0xDC */
s32 fn_8007D89C(void) {
    /* TODO: decompile (0xDC bytes) */
    fn_80104704();
    fn_801040A0();
    fn_80132A38();
    return 0;
}

/* 0x8007D978 | size: 0x23EC */
s32 fn_8007D978(void) {
    /* TODO: decompile (0x23EC bytes, ~2299 instructions) */
    /* Contains switch/jump table */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_80104704();
    fn_801040A0();
    fn_801040D0();
    fn_8007FDBC();
    fn_801081F8();
    fn_800E202C();
    fn_80196E10();
    fn_800E24B0();
    fn_800E209C();
    fn_80109220();
    fn_801046B8();
    fn_80105624();
    return -1;
}

#pragma pop
