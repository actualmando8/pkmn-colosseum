/**
 * @file menu_common_ext.c
 * @brief Menu common extensions - helpers, draw, input (0x8007109C-0x80072A00)
 *
 * Address range: 0x8007109C - 0x80072A00
 * Total functions: 23
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8006A7E8();
extern void fn_8006B154();
extern void fn_80073A44();
extern void fn_80073C38();
extern void fn_8008ABA0();
extern void fn_8008ABE4();
extern void fn_800A13F8();
extern void fn_800A1F94();
extern void fn_800A221C();
extern void fn_800D0F44();
extern void fn_800E202C();
extern void fn_800E209C();
extern void fn_800E24B0();
extern void fn_800E27B0();
extern void fn_800E2C04();
extern void fn_800F7AF0();
extern void fn_800F7BC4();
extern void fn_80102568();
extern void fn_801026A4();
extern void fn_801046B8();
extern void fn_80104704();
extern void fn_80113828();
extern void fn_80129280();
extern void fn_80196E10();
extern void fn_8025F3F4();
extern void fn_8025F584();
extern void fn_8025F648();
extern void OSCreateAlarm();
extern void OSDisableInterrupts();
extern void OSGetTick();
extern void OSRestoreInterrupts();
extern void OSSetAlarm();

/* ===== SDA globals ===== */
extern u8 lbl_8047A600;
extern u8 lbl_8047C090;

/* ===== Rodata / data labels ===== */
extern u8 lbl_80268708[];
extern u8 lbl_80268718[];
extern u8 lbl_80268750[];
extern u8 lbl_803B6D88[];
extern u8 lbl_803B6DE0[];
extern u8 lbl_803B6E08[];
extern u8 lbl_803B6E18[];

/* ===== Forward declarations ===== */
void fn_8007109C(void);
s32 fn_80071104(void);
s32 fn_80071160(void);
void fn_80071208(void);
void fn_80071318(void);
void fn_80071344(void);
s32 fn_80071398(void);
s32 fn_800714C8(void);
void fn_800715BC(void);
void fn_8007162C(void);
void fn_80071644(void);
s32 fn_8007169C(void);
s32 fn_800716C8(void);
s32 fn_800716E8(void);
s32 fn_80071700(void);
void fn_800719A8(void);
s32 fn_80071AE4(void);
void fn_80071E34(void);
s32 fn_80071EA4(void);
s32 fn_800722A0(void);
void fn_80072548(void);
void fn_80072684(void);
s32 fn_800726A8(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x8007109C | size: 0x68 */
void fn_8007109C(void) {
    /* TODO: decompile (0x68 bytes) */
    fn_800E202C();
    fn_80196E10();
    fn_800E24B0();
    fn_800E209C();
}

/* 0x80071104 | size: 0x5C */
s32 fn_80071104(void) {
    /* TODO: decompile (0x5C bytes) */
    fn_800E2C04();
    fn_800E27B0();
    fn_80196E10();
    return 0;
}

/* 0x80071160 | size: 0xA8 */
s32 fn_80071160(void) {
    /* TODO: decompile (0xA8 bytes) */
    fn_80129280();
    fn_8006A7E8();
    fn_8008ABA0();
    return 0;
}

/* 0x80071208 | size: 0x110 */
void fn_80071208(void) {
    /* TODO: decompile (0x110 bytes) */
    fn_800F7AF0();
    fn_800F7BC4();
    fn_8008ABA0();
    fn_8006B154();
    fn_80073A44();
}

/* 0x80071318 | size: 0x2C */
void fn_80071318(void) {
}

/* 0x80071344 | size: 0x54 */
void fn_80071344(void) {
    /* TODO: decompile (0x54 bytes) */
    fn_801026A4();
}

/* 0x80071398 | size: 0x130 */
s32 fn_80071398(void) {
    /* TODO: decompile (0x130 bytes) */
    fn_801046B8();
    fn_80102568();
    fn_80104704();
    fn_80196E10();
    return 0;
}

/* 0x800714C8 | size: 0xF4 */
s32 fn_800714C8(void) {
    /* TODO: decompile (0xF4 bytes) */
    fn_801046B8();
    fn_80102568();
    fn_80104704();
    fn_80196E10();
    return -1;
}

/* 0x800715BC | size: 0x70 */
void fn_800715BC(void) {
    /* TODO: decompile (0x70 bytes) */
    fn_80196E10();
}

/* 0x8007162C | size: 0x18 */
void fn_8007162C(void) {
}

/* 0x80071644 | size: 0x58 */
void fn_80071644(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x8007169C | size: 0x2C */
s32 fn_8007169C(void) {
    fn_80113828();
    return 0;
}

/* 0x800716C8 | size: 0x20 */
s32 fn_800716C8(void) {
    return 0;
}

/* 0x800716E8 | size: 0x18 */
s32 fn_800716E8(void) {
    return 0;
}

/* 0x80071700 | size: 0x2A8 */
s32 fn_80071700(void) {
    /* TODO: decompile (0x2A8 bytes, ~170 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    fn_80073C38();
    fn_8025F648();
    OSGetTick();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

/* 0x800719A8 | size: 0x13C */
void fn_800719A8(void) {
    /* TODO: decompile (0x13C bytes) */
    fn_800A13F8();
    OSCreateAlarm();
    OSDisableInterrupts();
    OSSetAlarm();
    fn_800A221C();
    OSRestoreInterrupts();
    fn_800D0F44();
    fn_8025F3F4();
}

/* 0x80071AE4 | size: 0x350 */
s32 fn_80071AE4(void) {
    /* TODO: decompile (0x350 bytes, ~212 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    OSGetTick();
    fn_80073C38();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

/* 0x80071E34 | size: 0x70 */
void fn_80071E34(void) {
    /* TODO: decompile (0x70 bytes) */
    fn_8008ABE4();
    fn_80071EA4();
}

/* 0x80071EA4 | size: 0x3FC */
s32 fn_80071EA4(void) {
    /* TODO: decompile (0x3FC bytes, ~255 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_80073C38();
    fn_8025F648();
    OSGetTick();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

/* 0x800722A0 | size: 0x2A8 */
s32 fn_800722A0(void) {
    /* TODO: decompile (0x2A8 bytes, ~170 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    fn_80073C38();
    fn_8025F648();
    OSGetTick();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

/* 0x80072548 | size: 0x13C */
void fn_80072548(void) {
    /* TODO: decompile (0x13C bytes) */
    fn_800A13F8();
    OSCreateAlarm();
    OSDisableInterrupts();
    OSSetAlarm();
    fn_800A221C();
    OSRestoreInterrupts();
    fn_800D0F44();
    fn_8025F3F4();
}

/* 0x80072684 | size: 0x24 */
void fn_80072684(void) {
    fn_800A1F94();
}

/* 0x800726A8 | size: 0x358 */
s32 fn_800726A8(void) {
    /* TODO: decompile (0x358 bytes, ~214 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    OSGetTick();
    fn_800D0F44();
    fn_80073C38();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

#pragma pop
