/**
 * @file gs_range_80033278.c
 * @brief gs-engine code, 0x80033278 - 0x80035E04 (20 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

/* fn_80034708 - 0x80034708 | size: 0xB0 */
void fn_80034708(void* unused, u8* sprite) {
    extern u32 lbl_8047A458;
    extern u32 lbl_80266FA0[];
    extern void winSpriteSetDisp(void*, u32);
    extern u8* windowSearchID(s32);
    u32* table = NULL;
    u8* window;

    switch (lbl_8047A458) {
    case 1:
        table = lbl_80266FA0;
        break;
    }

    if (table != NULL) {
        winSpriteSetDisp(sprite, 1);
        switch (*(s16*)(sprite + 6)) {
        case 0x7CC:
            window = windowSearchID(0xA4);
            if (window != NULL) {
                *(u32*)(sprite + 0x4C) = table[(s8)window[0x95]];
            }
            break;
        }
    } else {
        *(u32*)(sprite + 0x4C) = 0;
        winSpriteSetDisp(sprite, 0);
    }
}

/* fn_800347B8 - 0x800347B8 | size: 0xC */
void fn_800347B8(void) {
    extern u8 lbl_8047A440;

    lbl_8047A440 = 1;
}

/* fn_800347C4 - 0x800347C4 | size: 0x24 */
#pragma push
#pragma scheduling off
void fn_800347C4(void) {
    extern void fn_80166A28(u32);

    fn_80166A28(0x26);
}
#pragma pop

/* fn_800347E8 - 0x800347E8 | size: 0x24 */
#pragma push
#pragma scheduling off
void fn_800347E8(void) {
    extern void fn_80166A28(u32);

    fn_80166A28(0x26);
}
#pragma pop

/* fn_8003480C - 0x8003480C | size: 0x24 */
#pragma push
#pragma scheduling off
void fn_8003480C(void) {
    extern void fn_80166A28(u32);

    fn_80166A28(0x26);
}
#pragma pop

/* fn_80034DC0 - 0x80034DC0 | size: 0x78 */
#pragma push
#pragma peephole off
void fn_80034DC0(u8* arg0, u8* arg1) {
    extern u32 lbl_8047A44C;
    extern void fn_800FB680(s32, s32, u32, u16);
    extern void msgctrlSetValue(s32, s32);
    extern void fn_800FBB34(s32, s32, s32, s32, u32, u16);
    u8 byte;
    s32 mask;
    s32 combined;

    byte = arg0[0x8B];
    mask = -0x100;
    combined = byte | mask;
    fn_800FB680(0, 0, combined, 0x3CC8);
    msgctrlSetValue(0x34, lbl_8047A44C);
    fn_800FBB34(0, 0, *(s16*)(arg1 + 0x54), *(s16*)(arg1 + 0x56), combined, 0x3CC9);
}
#pragma pop

/* fn_80034E38 - 0x80034E38 | size: 0xB8 */
#pragma push
#pragma peephole off
void fn_80034E38(u8* arg0, u8* arg1) {
    extern u32 lbl_8047A44C;
    extern u32 lbl_8047A450;
    extern void fn_800FB680(s32, s32, u32, u16);
    extern void msgctrlSetValue(s32, u32);
    extern void fn_800FBB34(s32, s32, s32, s32, u32, u16);
    u32 count;
    u32 value;
    s32 mask;

    count = lbl_8047A450;
    if (count > lbl_8047A44C) {
        value = arg0[0x8B] | 0xFFA08000;
    } else {
        mask = -0x100;
        value = arg0[0x8B] | mask;
    }
    fn_800FB680(0, 0, value, 0x3CC7);
    if (lbl_8047A450 <= 0x98967F) {
        msgctrlSetValue(0x34, lbl_8047A450);
    } else {
        msgctrlSetValue(0x34, 0x98967F);
    }
    fn_800FBB34(0, 0, *(s16*)(arg1 + 0x54), *(s16*)(arg1 + 0x56), value, 0x3CC9);
}
#pragma pop

/* fn_80034EF0 - 0x80034EF0 | size: 0x94 */
#pragma push
#pragma peephole off
#pragma optimization_level 1
void fn_80034EF0(u8* arg0, u8* arg1) {
    extern void fn_800FB680(s32, s32, u32, u16);
    extern s32 heroGetStatus(s32, s32, s32);
    extern void msgctrlSetValue(s32, s32);
    extern void fn_800FBB34(s32, s32, s32, s32, u32, u16);
    s32 mask;
    s32 value;
    s32 status;
    s16 position;

    value = arg0[0x8B];
    mask = -0x100;
    value |= mask;
    fn_800FB680(0, 0, value, 0x3CC6);
    status = heroGetStatus(0, 0xD, 0);
    msgctrlSetValue(0x34, status);
    position = *(s16*)(arg1 + 0x54) + 2;
    fn_800FBB34(0, 0, position, *(s16*)(arg1 + 0x56), value, 0x3CD3);
}
#pragma pop

/* fn_80034F84 - 0x80034F84 | size: 0x2C */
#pragma push
#pragma scheduling off
#pragma optimize_for_size on
void fn_80034F84(void) {
    extern void msgctrlSetValue();
    extern u8 lbl_803A3288[];
#pragma peephole off
    msgctrlSetValue(0x37, lbl_803A3288);
}
#pragma peephole on
#pragma optimize_for_size reset
#pragma pop

/* fn_80034FB0 - 0x80034FB0 | size: 0x4 */
void fn_80034FB0(void) {
}

/* fn_80035D70 - 0x80035D70 | size: 0x30 */
#pragma push
#pragma scheduling off
#pragma optimize_for_size on
void fn_80035D70(void) {
    extern void fn_800FF730(u32);
    extern void _threadSwitch(void);
    extern volatile u8 lbl_8047A439;

    fn_800FF730((lbl_8047A439 = 1, 0x393));
    _threadSwitch();
}
#pragma optimize_for_size reset
#pragma pop

/* fn_80035DA0 - 0x80035DA0 | size: 0x34 */
#pragma push
#pragma scheduling off
#pragma optimize_for_size on
void fn_80035DA0(void) {
    extern void floorLink(s32, s32);
    extern void _threadSwitch(void);
    extern volatile u8 lbl_8047A439;

    floorLink((lbl_8047A439 = 0, 0x393), 0);
    _threadSwitch();
}
#pragma optimize_for_size reset
#pragma pop

/* fn_80035DD4 - 0x80035DD4 | size: 0x30 */
#pragma push
#pragma scheduling off
#pragma optimize_for_size on
void fn_80035DD4(void) {
    extern void fadeSet(f32, u32);
    extern void fadeCheck(u32);
    extern f32 lbl_8047BA30;

    fadeSet(lbl_8047BA30, 3);
    fadeCheck(1);
}
#pragma optimize_for_size reset
#pragma pop
