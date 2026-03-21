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
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8007109C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    ((void(*)(void))fn_800E202C)();
    r31 = r3;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_800710D4;
    r3 = (u32)&lbl_80268708;
    r4 = 0xde;
    r3 = (u32)&lbl_80268708;
    r5 = (u32)&lbl_8047C090;
    ((void(*)(void))fn_80196E10)();
L_800710D4: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) goto L_800710F0;
    r3 = r31;
    ((void(*)(void))fn_800E24B0)();
    r3 = r31;
    ((void(*)(void))fn_800E209C)();
L_800710F0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x80071104 | size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80071104(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = r3 + 0x1f;
    r4 = 0x20;
    /* clrrwi r3, r0, 5 */;
    ((void(*)(void))fn_800E2C04)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) goto L_80071134;
    ((void(*)(void))fn_800E27B0)();
    goto L_80071150;
L_80071134: ;
    if ((u32)r0 != (u32)0x0) goto L_8007114C;
    r3 = (u32)&lbl_80268708;
    r4 = 0xd5;
    r3 = (u32)&lbl_80268708;
    r5 = (u32)&lbl_8047C090;
    ((void(*)(void))fn_80196E10)();
L_8007114C: ;
    r3 = 0x0;
L_80071150: ;
    return;
}
#pragma pop

/* 0x80071160 | size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80071160(void) {
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = 0x0;
    r31 = 0x0;
L_80071180: ;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    r0 = r31 + 0x59cc;
    r29 = *(u32*)(r3 + r0);
    if ((s32)r29 == (s32)0x0) goto L_800711D8;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    r0 = r31 + 0x59a8;
    r3 = r3 + r0;
    ((void(*)(void))fn_8006A7E8)();
    if ((s32)r3 == (s32)0x0) goto L_800711D8;
    r3 = r29;
    ((void(*)(void))fn_8008ABA0)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_800711D8;
    r3 = r29;
    goto L_800711EC;
L_800711D8: ;
    r31 = r31 + 0x1660;
    r30 = r30 + 0x1;
    if ((u32)r30 < (u32)0x4) goto L_80071180;
    r3 = 0x0;
L_800711EC: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80071208 | size: 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80071208(void) {
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    ((void(*)(void))fn_800F7AF0)();
    r31 = r3;
    r3 = r30;
    ((void(*)(void))fn_800F7BC4)();
    r31 = r3 & r31;
    if ((u32)r31 != (u32)0x0) goto L_800712FC;
    r3 = r30;
    ((void(*)(void))fn_8008ABA0)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800712FC;
    r3 = r30;
    ((void(*)(void))fn_8006B154)();
    r4 = r1 + 0x8;
    ((void(*)(void))fn_80073A44)();
    if ((s32)r3 != (s32)0x0) goto L_800712FC;
    r3 = *(u16*)(sp + 0x8);
    r0 = r3 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_8007127C;
    r31 = r31 | 0x100;
L_8007127C: ;
    r0 = r3 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_8007128C;
    r31 = r31 | 0x200;
L_8007128C: ;
    r0 = r3 & 0x00000008;
    if ((s32)r0 == (s32)0x0) goto L_8007129C;
    r31 = r31 | 0x1000;
L_8007129C: ;
    r0 = r3 & 0x00000010;
    if ((s32)r0 == (s32)0x0) goto L_800712AC;
    r31 = r31 | 0x2;
L_800712AC: ;
    r0 = r3 & 0x00000020;
    if ((s32)r0 == (s32)0x0) goto L_800712BC;
    r31 = r31 | 0x1;
L_800712BC: ;
    r0 = r3 & 0x00000040;
    if ((s32)r0 == (s32)0x0) goto L_800712CC;
    r31 = r31 | 0x8;
L_800712CC: ;
    r0 = r3 & 0x00000080;
    if ((s32)r0 == (s32)0x0) goto L_800712DC;
    r31 = r31 | 0x4;
L_800712DC: ;
    r0 = r3 & 0x00000100;
    if ((s32)r0 == (s32)0x0) goto L_800712EC;
    r31 = r31 | 0x20;
L_800712EC: ;
    r0 = r3 & 0x00000200;
    if ((s32)r0 == (s32)0x0) goto L_800712FC;
    r31 = r31 | 0x40;
L_800712FC: ;
    r3 = r31;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* 0x80071318 | size: 0x2C */
void fn_80071318(void) {
}

/* 0x80071344 | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80071344(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    r3 = (u32)&lbl_803B6D88;
    r6 = 0x10;
    r0 = *(u32*)((u8*)r3 + 0x40);
    r7 = 0x1;
    r8 = 0x0;
    r9 = 0x0;
    r0 = r0 << 3;
    r5 = r3 + r0;
    r3 = *(u32*)((u8*)r5 + 0x0);
    r5 = r5 + 0x4;
    /* crclr cr1eq */;
    ((void(*)(void))fn_801026A4)();
    return;
}
#pragma pop

/* 0x80071398 | size: 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80071398(void) {
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r3 = (u32)&lbl_803B6D88;
    r31 = (u32)&lbl_803B6D88;
    r0 = *(u32*)((u8*)r31 + 0x40);
    r0 = r0 << 3;
    r29 = *(u32*)(r31 + r0);
    ((void(*)(void))fn_801046B8)();
    if ((s32)r3 != (s32)r29) goto L_800713F4;
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    r0 = r0 << 3;
    r5 = 0x0;
    r3 = *(u32*)(r3 + r0);
    ((void(*)(void))fn_80102568)();
L_800713F4: ;
    r3 = 0xbe;
    ((void(*)(void))fn_80104704)();
    if ((u32)r3 == (u32)0x0) goto L_80071414;
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    ((void(*)(void))fn_80102568)();
L_80071414: ;
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    r0 = r0 << 3;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x4) = r4;
    r0 = *(u32*)((u8*)r31 + 0x40);
    if ((s32)r0 == (s32)0x0) goto L_80071464;
    if ((s32)r0 > (s32)0x0) goto L_80071458;
    r3 = (u32)&lbl_80268708;
    r5 = (u32)&lbl_80268718;
    r3 = (u32)&lbl_80268708;
    r4 = 0x5c;
    r5 = (u32)&lbl_80268718;
    ((void(*)(void))fn_80196E10)();
L_80071458: ;
    r3 = *(u32*)((u8*)r31 + 0x40);
    /* subi r0, r3, 0x1 */;
    *(u32*)((u8*)r31 + 0x40) = r0;
L_80071464: ;
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    goto L_80071484;
L_80071470: ;
    if ((s32)r4 == (s32)0x0) goto L_80071498;
    r4 = *(u32*)((u8*)r31 + 0x40);
    /* subi r0, r4, 0x1 */;
    *(u32*)((u8*)r31 + 0x40) = r0;
L_80071484: ;
    r4 = *(u32*)((u8*)r31 + 0x40);
    r0 = r4 << 3;
    r0 = *(u32*)(r3 + r0);
    if ((s32)r30 != (s32)r0) goto L_80071470;
L_80071498: ;
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    r0 = r0 << 3;
    r3 = *(u32*)(r3 + r0);
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x800714C8 | size: 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_800714C8(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)&lbl_803B6D88;
    r31 = (u32)&lbl_803B6D88;
    r0 = *(u32*)((u8*)r31 + 0x40);
    r0 = r0 << 3;
    r30 = *(u32*)(r31 + r0);
    ((void(*)(void))fn_801046B8)();
    if ((s32)r3 != (s32)r30) goto L_8007151C;
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    r0 = r0 << 3;
    r5 = 0x0;
    r3 = *(u32*)(r3 + r0);
    ((void(*)(void))fn_80102568)();
L_8007151C: ;
    r3 = 0xbe;
    ((void(*)(void))fn_80104704)();
    if ((u32)r3 == (u32)0x0) goto L_8007153C;
    r3 = 0xbe;
    r4 = 0x0;
    r5 = 0x1;
    ((void(*)(void))fn_80102568)();
L_8007153C: ;
    r0 = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    r4 = 0x0;
    r0 = r0 << 3;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x4) = r4;
    r0 = *(u32*)((u8*)r31 + 0x40);
    if ((s32)r0 != (s32)0x0) goto L_8007156C;
    r3 = -0x1;
    goto L_800715A4;
L_8007156C: ;
    if ((s32)r0 > (s32)0x0) goto L_80071588;
    r3 = (u32)&lbl_80268708;
    r5 = (u32)&lbl_80268718;
    r3 = (u32)&lbl_80268708;
    r4 = 0x5c;
    r5 = (u32)&lbl_80268718;
    ((void(*)(void))fn_80196E10)();
L_80071588: ;
    r4 = *(u32*)((u8*)r31 + 0x40);
    r3 = (u32)&lbl_803B6D88;
    r3 = (u32)&lbl_803B6D88;
    /* subi r0, r4, 0x1 */;
    *(u32*)((u8*)r31 + 0x40) = r0;
    r0 = r0 << 3;
    r3 = *(u32*)(r3 + r0);
L_800715A4: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x800715BC | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800715BC(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = (u32)&lbl_803B6D88;
    r5 = (u32)&lbl_803B6D88;
    r4 = *(u32*)((u8*)r5 + 0x40);
    if ((u32)r4 < (u32)0x8) goto L_800715F8;
    r3 = (u32)&lbl_80268708;
    r5 = (u32)&lbl_80268750;
    r3 = (u32)&lbl_80268708;
    r4 = 0x41;
    r5 = (u32)&lbl_80268750;
    ((void(*)(void))fn_80196E10)();
    goto L_8007161C;
L_800715F8: ;
    r0 = r4 + 0x1;
    r4 = 0x0;
    *(u32*)((u8*)r5 + 0x40) = r0;
    r0 = r0 << 3;
    *(u32*)(r5 + r0) = r3;
    r0 = *(u32*)((u8*)r5 + 0x40);
    r0 = r0 << 3;
    r3 = r5 + r0;
    *(u32*)((u8*)r3 + 0x4) = r4;
L_8007161C: ;
    return;
}
#pragma pop

/* 0x8007162C | size: 0x18 */
void fn_8007162C(void) {
}

/* 0x80071644 | size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80071644(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = (u32)&lbl_803B6D88;
    r0 = 0x0;
    r4 = (u32)&lbl_803B6D88;
    *(u32*)((u8*)r4 + 0x0) = r0;
    *(u32*)((u8*)r4 + 0x4) = r0;
    *(u32*)((u8*)r4 + 0x8) = r0;
    *(u32*)((u8*)r4 + 0xC) = r0;
    *(u32*)((u8*)r4 + 0x10) = r0;
    *(u32*)((u8*)r4 + 0x14) = r0;
    *(u32*)((u8*)r4 + 0x18) = r0;
    *(u32*)((u8*)r4 + 0x1C) = r0;
    *(u32*)((u8*)r4 + 0x20) = r0;
    *(u32*)((u8*)r4 + 0x24) = r0;
    *(u32*)((u8*)r4 + 0x28) = r0;
    *(u32*)((u8*)r4 + 0x2C) = r0;
    *(u32*)((u8*)r4 + 0x30) = r0;
    *(u32*)((u8*)r4 + 0x34) = r0;
    *(u32*)((u8*)r4 + 0x38) = r0;
    *(u32*)((u8*)r4 + 0x3C) = r0;
    *(u32*)((u8*)r4 + 0x0) = r3;
    *(u32*)((u8*)r4 + 0x40) = r0;
    return;
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80071700(void) {
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r24, 0x20(r1) */;
    r30 = r3;
    r31 = r30 + 0x1;
    r4 = 0x2;
    r3 = r31;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r30;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 == (s32)0x0) goto L_8007173C;
    r27 = r3;
    goto L_80071984;
L_8007173C: ;
    r0 = 0x44;
    r3 = r30;
    *(u32*)(sp + 0xC) = r0;
    r4 = r1 + 0xc;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == (s32)0x0) goto L_80071764;
    r27 = 0xb;
    goto L_80071858;
L_80071764: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r26 = r0 * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r29 = r3;
    r0 = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = r0 + r6;
    r27 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_800717AC: ;
    OSGetTick();
    r0 = r3 - r29;
    if ((u32)r0 <= (u32)r26) goto L_800717C4;
    r3 = 0x1;
    goto L_80071844;
L_800717C4: ;
    r3 = r30;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_800717E0;
    r3 = 0x2;
    goto L_80071844;
L_800717E0: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_80071820;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_8007180C;
    r3 = r30;
    r4 = *(u32*)((u8*)r25 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8007180C: ;
    r0 = *(u32*)(r27 + r28);
    if ((s32)r0 == (s32)0x0) goto L_800717AC;
    r3 = 0x3e8;
    goto L_80071844;
L_80071820: ;
    r3 = r30;
    r4 = r1 + 0x10;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80071840;
    r3 = 0x3;
    goto L_80071844;
L_80071840: ;
    r3 = 0x0;
L_80071844: ;
    if ((s32)r3 == (s32)0x0) goto L_80071854;
    r27 = r3 + 0xb;
    goto L_80071858;
L_80071854: ;
    r27 = 0x0;
L_80071858: ;
    if ((s32)r27 == (s32)0x0) goto L_80071864;
    goto L_80071984;
L_80071864: ;
    r0 = *(u32*)(sp + 0x10);
    r0 = (u32)r0 >> 24;
    if ((u32)r0 == (u32)0x44) goto L_8007187C;
    r27 = 0xf;
    goto L_80071984;
L_8007187C: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r26 = r0 * 0x7530;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r27 = r3;
    r0 = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = r0 + r6;
    r29 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_800718C4: ;
    OSGetTick();
    r0 = r3 - r27;
    if ((u32)r0 <= (u32)r26) goto L_800718DC;
    r3 = 0x1;
    goto L_8007195C;
L_800718DC: ;
    r3 = r30;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_800718F8;
    r3 = 0x2;
    goto L_8007195C;
L_800718F8: ;
    r0 = *(u8*)(sp + 0x9);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_80071938;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_80071924;
    r3 = r30;
    r4 = *(u32*)((u8*)r25 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80071924: ;
    r0 = *(u32*)(r29 + r28);
    if ((s32)r0 == (s32)0x0) goto L_800718C4;
    r3 = 0x3e8;
    goto L_8007195C;
L_80071938: ;
    r3 = r30;
    r4 = r1 + 0x10;
    r5 = r1 + 0xb;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80071958;
    r3 = 0x3;
    goto L_8007195C;
L_80071958: ;
    r3 = 0x0;
L_8007195C: ;
    if ((s32)r3 == (s32)0x0) goto L_8007196C;
    r27 = r3 + 0xf;
    goto L_80071984;
L_8007196C: ;
    r0 = *(u32*)(sp + 0x10);
    if ((u32)r0 == (u32)0x0) goto L_80071980;
    r27 = 0x13;
    goto L_80071984;
L_80071980: ;
    r27 = 0x0;
L_80071984: ;
    r3 = r31;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r27;
    /* lmw r24, 0x20(r1) */;
    return;
}
#pragma pop

/* 0x800719A8 | size: 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800719A8(void) {
    extern void fn_80072684();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r31 = r3;
    ((void(*)(void))fn_800A13F8)();
    r4 = (u32)&lbl_803B6DE0;
    *(u32*)&lbl_8047A600 = r3;
    r3 = (u32)&lbl_803B6DE0;
    OSCreateAlarm();
    OSDisableInterrupts();
    r4 = (0x8000 << 16);
    r5 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r4 = (u32)fn_80072684;
    r5 = r5 + 0x4dd3;
    r6 = (u32)&lbl_803B6DE0;
    r0 = (u32)r0 >> 2;
    r7 = (u32)fn_80072684;
    r0 = (u32)((u64)r5 * (u64)r0 >> 32);
    r5 = (u32)&lbl_803B6DE0;
    r30 = r3;
    r3 = r5;
    r5 = 0x0;
    r6 = (u32)r0 >> 6;
    OSSetAlarm();
    r3 = *(u32*)&lbl_8047A600;
    ((void(*)(void))fn_800A221C)();
    r3 = r30;
    OSRestoreInterrupts();
    r3 = r31;
    ((void(*)(void))fn_800D0F44)();
    /* subis r0, r3, 0x4 */;
    if ((u32)r0 == (u32)0x0) goto L_80071A40;
    r30 = 0x1;
    goto L_80071AAC;
L_80071A40: ;
    r3 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80071A5C;
    r30 = 0x2;
    goto L_80071AAC;
L_80071A5C: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0x00000008;
    if ((s32)r0 != (s32)0x0) goto L_80071A74;
    r30 = -0x1;
    goto L_80071AAC;
L_80071A74: ;
    r3 = r31;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80071A94;
    r30 = 0x3;
    goto L_80071AAC;
L_80071A94: ;
    r0 = *(u32*)(sp + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_80071AA8;
    r30 = 0x4;
    goto L_80071AAC;
L_80071AA8: ;
    r30 = 0x0;
L_80071AAC: ;
    if ((s32)r30 == (s32)0x0) goto L_80071ABC;
    if ((s32)r30 < (s32)0x3) goto L_80071AC8;
L_80071ABC: ;
    r3 = r31 + 0x1;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
L_80071AC8: ;
    r3 = r30;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* 0x80071AE4 | size: 0x350 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80071AE4(void) {
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r14, 0x28(r1) */;
    r15 = r3;
    r16 = r4;
    r0 = r15 + 0x1;
    r4 = 0x2;
    *(u32*)(sp + 0x18) = r0;
    r3 = r0;
    ((void(*)(void))fn_8008ABE4)();
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r18 = r0 * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r15 << 3;
    r25 = r3;
    r0 = (u32)&lbl_803B6E18;
    r26 = r15 << 2;
    r19 = r0 + r6;
    r27 = (u32)&lbl_803B6E08;
    r20 = r19 + 0x4;
    r5 = *(u32*)((u8*)r16 + 0xC);
    r3 = (u32)r5 >> 24;
    r0 = ((r5 << 24) | ((u32)r5 >> 8)) & 0x0000FF00;
    r4 = ((r5 << 8) | ((u32)r5 >> 24)) & 0x00FF0000;
    r5 = r5 << 24;
    r0 = r3 | r0;
    r0 = r4 | r0;
    r0 = r5 | r0;
    r14 = (u32)r0 >> 16;
    r23 = r0 & 0xFFFF;
L_80071B80: ;
    OSGetTick();
    r4 = r3 - r25;
    r3 = r15;
    r0 = r4 ^ r18;
    r0 = __cntlzw(r0);
    r0 = r4 << r0;
    r17 = (u32)r0 >> 31;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 == (s32)0x0) goto L_80071BB0;
    r21 = r3;
    goto L_80071DF0;
L_80071BB0: ;
    r0 = 0x22;
    r3 = r15;
    *(u32*)(sp + 0x10) = r0;
    r4 = r1 + 0x10;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == (s32)0x0) goto L_80071BD8;
    r21 = 0xb;
    goto L_80071CAC;
L_80071BD8: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r22 = r0 * 0x64;
    OSGetTick();
    r21 = r3;
L_80071C00: ;
    OSGetTick();
    r0 = r3 - r21;
    if ((u32)r0 <= (u32)r22) goto L_80071C18;
    r3 = 0x1;
    goto L_80071C98;
L_80071C18: ;
    r3 = r15;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80071C34;
    r3 = 0x2;
    goto L_80071C98;
L_80071C34: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_80071C74;
    r12 = *(u32*)((u8*)r19 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_80071C60;
    r3 = r15;
    r4 = *(u32*)((u8*)r20 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80071C60: ;
    r0 = *(u32*)(r27 + r26);
    if ((s32)r0 == (s32)0x0) goto L_80071C00;
    r3 = 0x3e8;
    goto L_80071C98;
L_80071C74: ;
    r3 = r15;
    r4 = r1 + 0x14;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80071C94;
    r3 = 0x3;
    goto L_80071C98;
L_80071C94: ;
    r3 = 0x0;
L_80071C98: ;
    if ((s32)r3 == (s32)0x0) goto L_80071CA8;
    r21 = r3 + 0xb;
    goto L_80071CAC;
L_80071CA8: ;
    r21 = 0x0;
L_80071CAC: ;
    if ((s32)r21 == (s32)0x0) goto L_80071CB8;
    goto L_80071DF0;
L_80071CB8: ;
    r0 = *(u32*)(sp + 0x14);
    r0 = (u32)r0 >> 24;
    if ((u32)r0 == (u32)0x22) goto L_80071CD0;
    r21 = 0xf;
    goto L_80071DF0;
L_80071CD0: ;
    r0 = r14 & 0xFFFF;
    r3 = r23 << 2;
    r24 = r3 + 0x10;
    if ((u32)r0 == (u32)0x0) goto L_80071CE8;
    r24 = r24 + 0x70;
L_80071CE8: ;
    r22 = 0x0;
    r3 = (0x1062 << 16);
    r31 = r16;
    r28 = r3 + 0x4dd3;
    r29 = (0x8000 << 16);
    goto L_80071DD4;
L_80071D00: ;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r3 = r15;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    *(u32*)(sp + 0xC) = r0;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == (s32)0x0) goto L_80071D28;
    r21 = 0x10;
    goto L_80071DE0;
L_80071D28: ;
    r0 = *(u32*)((u8*)r29 + 0xF8);
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r28 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r21 = r0 * 0x64;
    OSGetTick();
    r30 = r3;
L_80071D44: ;
    OSGetTick();
    r0 = r3 - r30;
    if ((u32)r0 <= (u32)r21) goto L_80071D5C;
    r21 = 0x11;
    goto L_80071DE0;
L_80071D5C: ;
    r3 = r15;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80071D78;
    r21 = 0x12;
    goto L_80071DE0;
L_80071D78: ;
    r0 = *(u8*)(sp + 0x9);
    r0 = r0 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_80071DB8;
    r12 = *(u32*)((u8*)r19 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_80071DA4;
    r3 = r15;
    r4 = *(u32*)((u8*)r20 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80071DA4: ;
    r0 = *(u32*)(r27 + r26);
    if ((s32)r0 == (s32)0x0) goto L_80071D44;
    r21 = 0x3e8;
    goto L_80071DE0;
L_80071DB8: ;
    r0 = *(u32*)(r27 + r26);
    if ((s32)r0 == (s32)0x0) goto L_80071DCC;
    r21 = 0x3e8;
    goto L_80071DE0;
L_80071DCC: ;
    r22 = r22 + 0x4;
    r31 = r31 + 0x4;
L_80071DD4: ;
    if ((s32)r22 < (s32)r24) goto L_80071D00;
    r21 = 0x0;
L_80071DE0: ;
    if ((s32)r21 == (s32)0x0) goto L_80071DEC;
    goto L_80071DF0;
L_80071DEC: ;
    r21 = 0x0;
L_80071DF0: ;
    if ((s32)r21 != (s32)0x1) goto L_80071E00;
    if ((s32)r17 == (s32)0x0) goto L_80071B80;
L_80071E00: ;
    r3 = *(u32*)(sp + 0x18);
    if ((s32)r21 == (s32)0x0) goto L_80071E14;
    r4 = 0x1;
    goto L_80071E18;
L_80071E14: ;
    r4 = 0x3;
L_80071E18: ;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r21;
    /* lmw r14, 0x28(r1) */;
    return;
}
#pragma pop

/* 0x80071E34 | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80071E34(void) {
    extern void fn_80071EA4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    r31 = r29 + 0x1;
    r4 = 0x2;
    r3 = r31;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r29;
    r4 = r30;
    fn_80071EA4();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r31;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80071EA4 | size: 0x3FC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80071EA4(void) {
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r20, 0x20(r1) */;
    r28 = r3;
    r29 = r4;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 == (s32)0x0) goto L_80071ECC;
    goto L_8007228C;
L_80071ECC: ;
    r0 = 0x33;
    r3 = r28;
    *(u32*)(sp + 0x18) = r0;
    r4 = r1 + 0x18;
    r5 = r1 + 0xd;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == (s32)0x0) goto L_80071EF4;
    r3 = 0xb;
    goto L_80071FE8;
L_80071EF4: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r24 = r0 * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r28 << 3;
    r23 = r3;
    r0 = (u32)&lbl_803B6E18;
    r22 = r28 << 2;
    r25 = r0 + r6;
    r21 = (u32)&lbl_803B6E08;
    r20 = r25 + 0x4;
L_80071F3C: ;
    OSGetTick();
    r0 = r3 - r23;
    if ((u32)r0 <= (u32)r24) goto L_80071F54;
    r3 = 0x1;
    goto L_80071FD4;
L_80071F54: ;
    r3 = r28;
    r4 = r1 + 0xa;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80071F70;
    r3 = 0x2;
    goto L_80071FD4;
L_80071F70: ;
    r0 = *(u8*)(sp + 0xA);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_80071FB0;
    r12 = *(u32*)((u8*)r25 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_80071F9C;
    r3 = r28;
    r4 = *(u32*)((u8*)r20 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80071F9C: ;
    r0 = *(u32*)(r21 + r22);
    if ((s32)r0 == (s32)0x0) goto L_80071F3C;
    r3 = 0x3e8;
    goto L_80071FD4;
L_80071FB0: ;
    r3 = r28;
    r4 = r1 + 0x1c;
    r5 = r1 + 0xd;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80071FD0;
    r3 = 0x3;
    goto L_80071FD4;
L_80071FD0: ;
    r3 = 0x0;
L_80071FD4: ;
    if ((s32)r3 == (s32)0x0) goto L_80071FE4;
    r3 = r3 + 0xb;
    goto L_80071FE8;
L_80071FE4: ;
    r3 = 0x0;
L_80071FE8: ;
    if ((s32)r3 == (s32)0x0) goto L_80071FF4;
    goto L_8007228C;
L_80071FF4: ;
    r0 = *(u32*)(sp + 0x1C);
    r0 = (u32)r0 >> 24;
    if ((u32)r0 == (u32)0x33) goto L_8007200C;
    r3 = 0xf;
    goto L_8007228C;
L_8007200C: ;
    r4 = (u32)&lbl_803B6E18;
    r3 = (u32)&lbl_803B6E08;
    r5 = r28 << 3;
    r27 = r28 << 2;
    r0 = (u32)&lbl_803B6E18;
    r26 = (u32)&lbl_803B6E08;
    r31 = r0 + r5;
    r23 = 0x0;
    r30 = r31 + 0x4;
    r3 = (0x1062 << 16);
    r22 = r29;
    r25 = r3 + 0x4dd3;
    r24 = (0x8000 << 16);
L_80072040: ;
    r0 = *(u32*)((u8*)r24 + 0xF8);
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r25 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r20 = r0 * 0x64;
    OSGetTick();
    r21 = r3;
L_8007205C: ;
    OSGetTick();
    r0 = r3 - r21;
    if ((u32)r0 <= (u32)r20) goto L_80072074;
    r3 = 0x1;
    goto L_800720F4;
L_80072074: ;
    r3 = r28;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80072090;
    r3 = 0x2;
    goto L_800720F4;
L_80072090: ;
    r0 = *(u8*)(sp + 0x9);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_800720D0;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_800720BC;
    r3 = r28;
    r4 = *(u32*)((u8*)r30 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800720BC: ;
    r0 = *(u32*)(r26 + r27);
    if ((s32)r0 == (s32)0x0) goto L_8007205C;
    r3 = 0x3e8;
    goto L_800720F4;
L_800720D0: ;
    r3 = r28;
    r4 = r1 + 0x14;
    r5 = r1 + 0xc;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_800720F0;
    r3 = 0x3;
    goto L_800720F4;
L_800720F0: ;
    r3 = 0x0;
L_800720F4: ;
    if ((s32)r3 == (s32)0x0) goto L_80072100;
    goto L_80072130;
L_80072100: ;
    r0 = *(u32*)(sp + 0x14);
    *(u32*)((u8*)r22 + 0x0) = r0;
    r0 = *(u32*)(r26 + r27);
    if ((s32)r0 == (s32)0x0) goto L_8007211C;
    r3 = 0x3e8;
    goto L_80072130;
L_8007211C: ;
    r23 = r23 + 0x4;
    r22 = r22 + 0x4;
    if ((s32)r23 < (s32)0x10) goto L_80072040;
    r3 = 0x0;
L_80072130: ;
    if ((s32)r3 == (s32)0x0) goto L_80072140;
    r3 = r3 + 0xf;
    goto L_8007228C;
L_80072140: ;
    r5 = *(u32*)((u8*)r29 + 0xC);
    r6 = (u32)&lbl_803B6E08;
    r24 = r28 << 2;
    r23 = 0x0;
    r3 = (u32)r5 >> 24;
    r0 = ((r5 << 24) | ((u32)r5 >> 8)) & 0x0000FF00;
    r4 = ((r5 << 8) | ((u32)r5 >> 24)) & 0x00FF0000;
    r5 = r5 << 24;
    r0 = r3 | r0;
    r25 = (u32)&lbl_803B6E08;
    r0 = r4 | r0;
    r0 = r5 | r0;
    r0 = r0 & 0xFFFF;
    r22 = r0 << 2;
    r3 = (0x1062 << 16);
    r27 = (0x8000 << 16);
    r26 = r3 + 0x4dd3;
    goto L_8007226C;
L_80072188: ;
    r0 = *(u32*)((u8*)r27 + 0xF8);
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r26 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r20 = r0 * 0x64;
    OSGetTick();
    r21 = r3;
L_800721A4: ;
    OSGetTick();
    r0 = r3 - r21;
    if ((u32)r0 <= (u32)r20) goto L_800721BC;
    r3 = 0x1;
    goto L_8007223C;
L_800721BC: ;
    r3 = r28;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_800721D8;
    r3 = 0x2;
    goto L_8007223C;
L_800721D8: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_80072218;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_80072204;
    r3 = r28;
    r4 = *(u32*)((u8*)r30 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80072204: ;
    r0 = *(u32*)(r25 + r24);
    if ((s32)r0 == (s32)0x0) goto L_800721A4;
    r3 = 0x3e8;
    goto L_8007223C;
L_80072218: ;
    r3 = r28;
    r4 = r1 + 0x10;
    r5 = r1 + 0xb;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80072238;
    r3 = 0x3;
    goto L_8007223C;
L_80072238: ;
    r3 = 0x0;
L_8007223C: ;
    if ((s32)r3 == (s32)0x0) goto L_80072248;
    goto L_80072278;
L_80072248: ;
    r0 = *(u32*)(sp + 0x10);
    r3 = r29 + r23;
    *(u32*)((u8*)r3 + 0x10) = r0;
    r0 = *(u32*)(r25 + r24);
    if ((s32)r0 == (s32)0x0) goto L_80072268;
    r3 = 0x3e8;
    goto L_80072278;
L_80072268: ;
    r23 = r23 + 0x4;
L_8007226C: ;
    if ((s32)r23 < (s32)r22) goto L_80072188;
    r3 = 0x0;
L_80072278: ;
    if ((s32)r3 == (s32)0x0) goto L_80072288;
    r3 = r3 + 0x12;
    goto L_8007228C;
L_80072288: ;
    r3 = 0x0;
L_8007228C: ;
    /* lmw r20, 0x20(r1) */;
    return;
}
#pragma pop

/* 0x800722A0 | size: 0x2A8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_800722A0(void) {
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r24, 0x20(r1) */;
    r30 = r3;
    r31 = r30 + 0x1;
    r4 = 0x2;
    r3 = r31;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r30;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 == (s32)0x0) goto L_800722DC;
    r27 = r3;
    goto L_80072524;
L_800722DC: ;
    r0 = 0x44;
    r3 = r30;
    *(u32*)(sp + 0xC) = r0;
    r4 = r1 + 0xc;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == (s32)0x0) goto L_80072304;
    r27 = 0xb;
    goto L_800723F8;
L_80072304: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r26 = r0 * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r29 = r3;
    r0 = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = r0 + r6;
    r27 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_8007234C: ;
    OSGetTick();
    r0 = r3 - r29;
    if ((u32)r0 <= (u32)r26) goto L_80072364;
    r3 = 0x1;
    goto L_800723E4;
L_80072364: ;
    r3 = r30;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80072380;
    r3 = 0x2;
    goto L_800723E4;
L_80072380: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_800723C0;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_800723AC;
    r3 = r30;
    r4 = *(u32*)((u8*)r25 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800723AC: ;
    r0 = *(u32*)(r27 + r28);
    if ((s32)r0 == (s32)0x0) goto L_8007234C;
    r3 = 0x3e8;
    goto L_800723E4;
L_800723C0: ;
    r3 = r30;
    r4 = r1 + 0x10;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_800723E0;
    r3 = 0x3;
    goto L_800723E4;
L_800723E0: ;
    r3 = 0x0;
L_800723E4: ;
    if ((s32)r3 == (s32)0x0) goto L_800723F4;
    r27 = r3 + 0xb;
    goto L_800723F8;
L_800723F4: ;
    r27 = 0x0;
L_800723F8: ;
    if ((s32)r27 == (s32)0x0) goto L_80072404;
    goto L_80072524;
L_80072404: ;
    r0 = *(u32*)(sp + 0x10);
    r0 = (u32)r0 >> 24;
    if ((u32)r0 == (u32)0x44) goto L_8007241C;
    r27 = 0xf;
    goto L_80072524;
L_8007241C: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r26 = r0 * 0x7530;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r27 = r3;
    r0 = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = r0 + r6;
    r29 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_80072464: ;
    OSGetTick();
    r0 = r3 - r27;
    if ((u32)r0 <= (u32)r26) goto L_8007247C;
    r3 = 0x1;
    goto L_800724FC;
L_8007247C: ;
    r3 = r30;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80072498;
    r3 = 0x2;
    goto L_800724FC;
L_80072498: ;
    r0 = *(u8*)(sp + 0x9);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_800724D8;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_800724C4;
    r3 = r30;
    r4 = *(u32*)((u8*)r25 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800724C4: ;
    r0 = *(u32*)(r29 + r28);
    if ((s32)r0 == (s32)0x0) goto L_80072464;
    r3 = 0x3e8;
    goto L_800724FC;
L_800724D8: ;
    r3 = r30;
    r4 = r1 + 0x10;
    r5 = r1 + 0xb;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_800724F8;
    r3 = 0x3;
    goto L_800724FC;
L_800724F8: ;
    r3 = 0x0;
L_800724FC: ;
    if ((s32)r3 == (s32)0x0) goto L_8007250C;
    r27 = r3 + 0xf;
    goto L_80072524;
L_8007250C: ;
    r0 = *(u32*)(sp + 0x10);
    if ((u32)r0 == (u32)0x0) goto L_80072520;
    r27 = 0x13;
    goto L_80072524;
L_80072520: ;
    r27 = 0x0;
L_80072524: ;
    r3 = r31;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r27;
    /* lmw r24, 0x20(r1) */;
    return;
}
#pragma pop

/* 0x80072548 | size: 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80072548(void) {
    extern void fn_80072684();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r31 = r3;
    ((void(*)(void))fn_800A13F8)();
    r4 = (u32)&lbl_803B6DE0;
    *(u32*)&lbl_8047A600 = r3;
    r3 = (u32)&lbl_803B6DE0;
    OSCreateAlarm();
    OSDisableInterrupts();
    r4 = (0x8000 << 16);
    r5 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r4 = (u32)fn_80072684;
    r5 = r5 + 0x4dd3;
    r6 = (u32)&lbl_803B6DE0;
    r0 = (u32)r0 >> 2;
    r7 = (u32)fn_80072684;
    r0 = (u32)((u64)r5 * (u64)r0 >> 32);
    r5 = (u32)&lbl_803B6DE0;
    r30 = r3;
    r3 = r5;
    r5 = 0x0;
    r6 = (u32)r0 >> 6;
    OSSetAlarm();
    r3 = *(u32*)&lbl_8047A600;
    ((void(*)(void))fn_800A221C)();
    r3 = r30;
    OSRestoreInterrupts();
    r3 = r31;
    ((void(*)(void))fn_800D0F44)();
    /* subis r0, r3, 0x4 */;
    if ((u32)r0 == (u32)0x0) goto L_800725E0;
    r30 = 0x1;
    goto L_8007264C;
L_800725E0: ;
    r3 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_800725FC;
    r30 = 0x2;
    goto L_8007264C;
L_800725FC: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0x00000008;
    if ((s32)r0 != (s32)0x0) goto L_80072614;
    r30 = -0x1;
    goto L_8007264C;
L_80072614: ;
    r3 = r31;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80072634;
    r30 = 0x3;
    goto L_8007264C;
L_80072634: ;
    r0 = *(u32*)(sp + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_80072648;
    r30 = 0x4;
    goto L_8007264C;
L_80072648: ;
    r30 = 0x0;
L_8007264C: ;
    if ((s32)r30 == (s32)0x0) goto L_8007265C;
    if ((s32)r30 < (s32)0x3) goto L_80072668;
L_8007265C: ;
    r3 = r31 + 0x1;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
L_80072668: ;
    r3 = r30;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* 0x80072684 | size: 0x24 */
void fn_80072684(void) {
    fn_800A1F94();
}

/* 0x800726A8 | size: 0x358 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_800726A8(void) {
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r14, 0x18(r1) */;
    r22 = r3;
    r23 = r4;
    r14 = r22 + 0x1;
    r4 = 0x2;
    r3 = r14;
    ((void(*)(void))fn_8008ABE4)();
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r25 = r0 * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r22 << 3;
    r28 = r3;
    r0 = (u32)&lbl_803B6E18;
    r29 = r22 << 2;
    r26 = r0 + r6;
    r30 = (u32)&lbl_803B6E08;
    r27 = r26 + 0x4;
    r3 = (0x1062 << 16);
    r15 = (0x8000 << 16);
    r31 = r3 + 0x4dd3;
L_80072724: ;
    OSGetTick();
    r0 = *(u32*)((u8*)r15 + 0xF8);
    r4 = r3 - r28;
    r3 = r4 ^ r25;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r31 * (u64)r0 >> 32);
    r3 = __cntlzw(r3);
    r3 = r4 << r3;
    r24 = (u32)r3 >> 31;
    r0 = (u32)r0 >> 6;
    r17 = r0 * 0x64;
    OSGetTick();
    r16 = r3;
L_80072758: ;
    OSGetTick();
    r0 = r3 - r16;
    if ((u32)r0 <= (u32)r17) goto L_80072770;
    r16 = 0x1;
    goto L_800729BC;
L_80072770: ;
    r3 = r22;
    ((void(*)(void))fn_800D0F44)();
    /* subis r0, r3, 0x4 */;
    if ((u32)r0 == (u32)0x0) goto L_800727B4;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_800727A0;
    r3 = r22;
    r4 = *(u32*)((u8*)r27 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800727A0: ;
    r0 = *(u32*)(r30 + r29);
    if ((s32)r0 == (s32)0x0) goto L_80072758;
    r16 = 0x3e8;
    goto L_800729BC;
L_800727B4: ;
    r3 = r22;
    ((void(*)(void))fn_80073C38)();
    if ((s32)r3 == (s32)0x0) goto L_800727CC;
    r16 = r3;
    goto L_800729BC;
L_800727CC: ;
    r0 = 0x55;
    r3 = r22;
    *(u32*)(sp + 0x10) = r0;
    r4 = r1 + 0x10;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == (s32)0x0) goto L_800727F4;
    r16 = 0xb;
    goto L_800728C8;
L_800727F4: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r17 = r0 * 0x64;
    OSGetTick();
    r16 = r3;
L_8007281C: ;
    OSGetTick();
    r0 = r3 - r16;
    if ((u32)r0 <= (u32)r17) goto L_80072834;
    r3 = 0x1;
    goto L_800728B4;
L_80072834: ;
    r3 = r22;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80072850;
    r3 = 0x2;
    goto L_800728B4;
L_80072850: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_80072890;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_8007287C;
    r3 = r22;
    r4 = *(u32*)((u8*)r27 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8007287C: ;
    r0 = *(u32*)(r30 + r29);
    if ((s32)r0 == (s32)0x0) goto L_8007281C;
    r3 = 0x3e8;
    goto L_800728B4;
L_80072890: ;
    r3 = r22;
    r4 = r1 + 0x14;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_800728B0;
    r3 = 0x3;
    goto L_800728B4;
L_800728B0: ;
    r3 = 0x0;
L_800728B4: ;
    if ((s32)r3 == (s32)0x0) goto L_800728C4;
    r16 = r3 + 0xb;
    goto L_800728C8;
L_800728C4: ;
    r16 = 0x0;
L_800728C8: ;
    if ((s32)r16 == (s32)0x0) goto L_800728D4;
    goto L_800729BC;
L_800728D4: ;
    r0 = *(u32*)(sp + 0x14);
    r0 = (u32)r0 >> 24;
    if ((u32)r0 == (u32)0x55) goto L_800728EC;
    r16 = 0xf;
    goto L_800729BC;
L_800728EC: ;
    r16 = r23;
    r18 = 0x0;
    r3 = (0x1062 << 16);
    r20 = (0x8000 << 16);
    r19 = r3 + 0x4dd3;
L_80072900: ;
    r0 = *(u32*)((u8*)r16 + 0x0);
    r3 = r22;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    *(u32*)(sp + 0xC) = r0;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 != (s32)0x0) goto L_800729B8;
    r0 = *(u32*)((u8*)r20 + 0xF8);
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r19 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r17 = r0 * 0x64;
    OSGetTick();
    r21 = r3;
L_8007293C: ;
    OSGetTick();
    r0 = r3 - r21;
    if ((u32)r0 > (u32)r17) goto L_800729B8;
    r3 = r22;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 != (s32)0x0) goto L_800729B8;
    r0 = *(u8*)(sp + 0x9);
    r0 = r0 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_8007299C;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_8007298C;
    r3 = r22;
    r4 = *(u32*)((u8*)r27 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8007298C: ;
    r0 = *(u32*)(r30 + r29);
    if ((s32)r0 == (s32)0x0) goto L_8007293C;
    goto L_800729B8;
L_8007299C: ;
    r0 = *(u32*)(r30 + r29);
    if ((s32)r0 != (s32)0x0) goto L_800729B8;
    r16 = r16 + 0x4;
    r18 = r18 + 0x4;
    if ((s32)r18 < (s32)0x78) goto L_80072900;
L_800729B8: ;
    r16 = 0x0;
L_800729BC: ;
    if ((s32)r16 != (s32)0x1) goto L_800729CC;
    if ((s32)r24 == (s32)0x0) goto L_80072724;
L_800729CC: ;
    r3 = r14;
    if ((s32)r16 == (s32)0x0) goto L_800729E0;
    r4 = 0x1;
    goto L_800729E4;
L_800729E0: ;
    r4 = 0x3;
L_800729E4: ;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r16;
    /* lmw r14, 0x18(r1) */;
    return;
}
#pragma pop

#pragma pop
