/**
 * @file gba_comm_ext.c
 * @brief GBA communication transfers (0x80092C90-0x800937F4)
 *
 * Address range: 0x80092C90 - 0x800937F4
 * Total functions: 9
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_800716C8();
extern void fn_800716E8();
extern void fn_80089048();
extern void fn_8009F77C();
extern void fn_8009F7B4();
extern void fn_8009F890();
extern void fn_8009F9C8();
extern void fn_8009FABC();
extern void fn_800A19CC();
extern void fn_800A1E54();
extern void fn_800A1F94();
extern void fn_800A257C();
extern void fn_800CA968();
extern void fn_800E202C();
extern void fn_800E209C();
extern void fn_800E24B0();
extern void fn_800E27B0();
extern void fn_800E2C04();
extern void fn_800F0308();
extern void fn_80196E10();
extern void strlen();
extern void* memset(void* dst, int val, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_8047C1E8;

/* ===== Rodata / data labels ===== */
extern u8 lbl_8026F5A8[];
extern u8 lbl_803FB328[];

/* ===== Forward declarations ===== */
s32 fn_80092C90(void);
s32 fn_80092E38(void);
s32 fn_80092FC8(void);
s32 fn_80093160(void);
s32 fn_800932F0(void);
s32 fn_800934E4(void);
s32 fn_80093574(void);
s32 fn_80093610(void);
s32 fn_80093698(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80092C90 | size: 0x1A8 */
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80092C90(void) {
    extern void fn_800937F4();
    extern void fn_80093B04();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    if ((s32)r29 < (s32)0x0) goto L_80092CBC;
    if ((s32)r29 <= (s32)0x3) goto L_80092CC4;
L_80092CBC: ;
    r0 = 0x0;
    goto L_80092D94;
L_80092CC4: ;
    r3 = (u32)&lbl_803FB328;
    r26 = r29 << 2;
    r27 = (u32)&lbl_803FB328;
    r0 = *(u32*)(r27 + r26);
    if ((u32)r0 == (u32)0x0) goto L_80092CE4;
    r0 = 0x1;
    goto L_80092D94;
L_80092CE4: ;
    r3 = 0x44a0;
    r4 = 0x20;
    ((void(*)(void))fn_800E2C04)();
    r28 = r3;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_80092D14;
    r3 = (u32)&lbl_8026F5A8;
    r4 = 0x1dd;
    r3 = (u32)&lbl_8026F5A8;
    r5 = (u32)&lbl_8047C1E8;
    ((void(*)(void))fn_80196E10)();
L_80092D14: ;
    r3 = r28;
    ((void(*)(void))fn_800E27B0)();
    r28 = r3;
    r4 = 0x0;
    r5 = 0x4490;
    memset((void*)r3, (int)r4, (u32)r5);
    *(u32*)(r27 + r26) = r28;
    r3 = (u32)fn_80093B04;
    r5 = (u32)fn_80093B04;
    r0 = 0x0;
    r26 = *(u32*)(r27 + r26);
    r3 = r29;
    *(u32*)((u8*)r26 + 0x4340) = r0;
    r4 = r26 + 0x20;
    *(u32*)((u8*)r26 + 0x4338) = r29;
    ((void(*)(void))fn_800716C8)();
    r3 = r26;
    ((void(*)(void))fn_8009F77C)();
    r3 = r26 + 0x18;
    ((void(*)(void))fn_8009F9C8)();
    r3 = (u32)fn_800937F4;
    r5 = r26;
    r4 = (u32)fn_800937F4;
    r6 = r26 + 0x4338;
    r3 = r26 + 0x20;
    r7 = 0x4000;
    r8 = 0x8;
    r9 = 0x0;
    ((void(*)(void))fn_800A19CC)();
    r3 = r26 + 0x20;
    ((void(*)(void))fn_800A1F94)();
    r0 = 0x1;
L_80092D94: ;
    if ((s32)r0 != (s32)0x0) goto L_80092DA4;
    r3 = 0x0;
    goto L_80092E24;
L_80092DA4: ;
    r3 = (u32)&lbl_803FB328;
    r0 = r29 << 2;
    r3 = (u32)&lbl_803FB328;
    r26 = 0x0;
    r27 = *(u32*)(r3 + r0);
    r3 = r27;
    ((void(*)(void))fn_8009F7B4)();
    r0 = *(u32*)((u8*)r27 + 0x4340);
    if ((s32)r0 != (s32)0x0) goto L_80092DFC;
    r4 = r30;
    r5 = r31;
    r3 = r27 + 0x4344;
    ((void(*)(void))fn_80089048)();
    r26 = r3;
    if ((s32)r26 == (s32)0x0) goto L_80092DFC;
    r0 = 0xc;
    r3 = (0x3 << 16);
    *(u32*)((u8*)r27 + 0x4340) = r0;
    r0 = r3 + 0xc;
    *(u32*)((u8*)r27 + 0x433C) = r0;
L_80092DFC: ;
    r3 = r27;
    ((void(*)(void))fn_8009F890)();
    r3 = r27 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    if ((s32)r26 == (s32)0x0) goto L_80092E20;
    r3 = r27 + 0x18;
    ((void(*)(void))fn_8009FABC)();
L_80092E20: ;
    r3 = r26;
L_80092E24: ;
    /* lmw r26, 0x8(r1) */;
    return;
}

/* 0x80092E38 | size: 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80092E38(void) {
    extern void fn_800937F4();
    extern void fn_80093B04();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r3;
    r31 = r4;
    if ((s32)r30 < (s32)0x0) goto L_80092E60;
    if ((s32)r30 <= (s32)0x3) goto L_80092E68;
L_80092E60: ;
    r0 = 0x0;
    goto L_80092F38;
L_80092E68: ;
    r3 = (u32)&lbl_803FB328;
    r27 = r30 << 2;
    r28 = (u32)&lbl_803FB328;
    r0 = *(u32*)(r28 + r27);
    if ((u32)r0 == (u32)0x0) goto L_80092E88;
    r0 = 0x1;
    goto L_80092F38;
L_80092E88: ;
    r3 = 0x44a0;
    r4 = 0x20;
    ((void(*)(void))fn_800E2C04)();
    r29 = r3;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_80092EB8;
    r3 = (u32)&lbl_8026F5A8;
    r4 = 0x1dd;
    r3 = (u32)&lbl_8026F5A8;
    r5 = (u32)&lbl_8047C1E8;
    ((void(*)(void))fn_80196E10)();
L_80092EB8: ;
    r3 = r29;
    ((void(*)(void))fn_800E27B0)();
    r29 = r3;
    r4 = 0x0;
    r5 = 0x4490;
    memset((void*)r3, (int)r4, (u32)r5);
    *(u32*)(r28 + r27) = r29;
    r3 = (u32)fn_80093B04;
    r5 = (u32)fn_80093B04;
    r0 = 0x0;
    r27 = *(u32*)(r28 + r27);
    r3 = r30;
    *(u32*)((u8*)r27 + 0x4340) = r0;
    r4 = r27 + 0x20;
    *(u32*)((u8*)r27 + 0x4338) = r30;
    ((void(*)(void))fn_800716C8)();
    r3 = r27;
    ((void(*)(void))fn_8009F77C)();
    r3 = r27 + 0x18;
    ((void(*)(void))fn_8009F9C8)();
    r3 = (u32)fn_800937F4;
    r5 = r27;
    r4 = (u32)fn_800937F4;
    r6 = r27 + 0x4338;
    r3 = r27 + 0x20;
    r7 = 0x4000;
    r8 = 0x8;
    r9 = 0x0;
    ((void(*)(void))fn_800A19CC)();
    r3 = r27 + 0x20;
    ((void(*)(void))fn_800A1F94)();
    r0 = 0x1;
L_80092F38: ;
    if ((s32)r0 != (s32)0x0) goto L_80092F48;
    r3 = 0x0;
    goto L_80092FB4;
L_80092F48: ;
    r3 = (u32)&lbl_803FB328;
    r0 = r30 << 2;
    r3 = (u32)&lbl_803FB328;
    r27 = 0x0;
    r28 = *(u32*)(r3 + r0);
    r3 = r28;
    ((void(*)(void))fn_8009F7B4)();
    r0 = *(u32*)((u8*)r28 + 0x4340);
    if ((s32)r0 != (s32)0x0) goto L_80092F8C;
    r0 = 0xb;
    r3 = (0x3 << 16);
    *(u32*)((u8*)r28 + 0x4340) = r0;
    r0 = r3 + 0xb;
    r27 = 0x1;
    *(u32*)((u8*)r28 + 0x433C) = r0;
    *(u32*)((u8*)r28 + 0x4344) = r31;
L_80092F8C: ;
    r3 = r28;
    ((void(*)(void))fn_8009F890)();
    r3 = r28 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    if ((s32)r27 == (s32)0x0) goto L_80092FB0;
    r3 = r28 + 0x18;
    ((void(*)(void))fn_8009FABC)();
L_80092FB0: ;
    r3 = r27;
L_80092FB4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80092FC8 | size: 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80092FC8(void) {
    extern void fn_800937F4();
    extern void fn_80093B04();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    if ((s32)r29 < (s32)0x0) goto L_80092FF4;
    if ((s32)r29 <= (s32)0x3) goto L_80092FFC;
L_80092FF4: ;
    r0 = 0x0;
    goto L_800930CC;
L_80092FFC: ;
    r3 = (u32)&lbl_803FB328;
    r26 = r29 << 2;
    r27 = (u32)&lbl_803FB328;
    r0 = *(u32*)(r27 + r26);
    if ((u32)r0 == (u32)0x0) goto L_8009301C;
    r0 = 0x1;
    goto L_800930CC;
L_8009301C: ;
    r3 = 0x44a0;
    r4 = 0x20;
    ((void(*)(void))fn_800E2C04)();
    r28 = r3;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_8009304C;
    r3 = (u32)&lbl_8026F5A8;
    r4 = 0x1dd;
    r3 = (u32)&lbl_8026F5A8;
    r5 = (u32)&lbl_8047C1E8;
    ((void(*)(void))fn_80196E10)();
L_8009304C: ;
    r3 = r28;
    ((void(*)(void))fn_800E27B0)();
    r28 = r3;
    r4 = 0x0;
    r5 = 0x4490;
    memset((void*)r3, (int)r4, (u32)r5);
    *(u32*)(r27 + r26) = r28;
    r3 = (u32)fn_80093B04;
    r5 = (u32)fn_80093B04;
    r0 = 0x0;
    r26 = *(u32*)(r27 + r26);
    r3 = r29;
    *(u32*)((u8*)r26 + 0x4340) = r0;
    r4 = r26 + 0x20;
    *(u32*)((u8*)r26 + 0x4338) = r29;
    ((void(*)(void))fn_800716C8)();
    r3 = r26;
    ((void(*)(void))fn_8009F77C)();
    r3 = r26 + 0x18;
    ((void(*)(void))fn_8009F9C8)();
    r3 = (u32)fn_800937F4;
    r5 = r26;
    r4 = (u32)fn_800937F4;
    r6 = r26 + 0x4338;
    r3 = r26 + 0x20;
    r7 = 0x4000;
    r8 = 0x8;
    r9 = 0x0;
    ((void(*)(void))fn_800A19CC)();
    r3 = r26 + 0x20;
    ((void(*)(void))fn_800A1F94)();
    r0 = 0x1;
L_800930CC: ;
    if ((s32)r0 != (s32)0x0) goto L_800930DC;
    r3 = 0x0;
    goto L_8009314C;
L_800930DC: ;
    r3 = (u32)&lbl_803FB328;
    r0 = r29 << 2;
    r3 = (u32)&lbl_803FB328;
    r26 = 0x0;
    r27 = *(u32*)(r3 + r0);
    r3 = r27;
    ((void(*)(void))fn_8009F7B4)();
    r0 = *(u32*)((u8*)r27 + 0x4340);
    if ((s32)r0 != (s32)0x0) goto L_80093124;
    r0 = 0x4;
    r3 = (0x3 << 16);
    *(u32*)((u8*)r27 + 0x4340) = r0;
    r0 = r3 + 0x4;
    r26 = 0x1;
    *(u32*)((u8*)r27 + 0x433C) = r0;
    *(u32*)((u8*)r27 + 0x4344) = r30;
    *(u32*)((u8*)r27 + 0x4348) = r31;
L_80093124: ;
    r3 = r27;
    ((void(*)(void))fn_8009F890)();
    r3 = r27 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    if ((s32)r26 == (s32)0x0) goto L_80093148;
    r3 = r27 + 0x18;
    ((void(*)(void))fn_8009FABC)();
L_80093148: ;
    r3 = r26;
L_8009314C: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80093160 | size: 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80093160(void) {
    extern void fn_800937F4();
    extern void fn_80093B04();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r3;
    r31 = r4;
    if ((s32)r30 < (s32)0x0) goto L_80093188;
    if ((s32)r30 <= (s32)0x3) goto L_80093190;
L_80093188: ;
    r0 = 0x0;
    goto L_80093260;
L_80093190: ;
    r3 = (u32)&lbl_803FB328;
    r27 = r30 << 2;
    r28 = (u32)&lbl_803FB328;
    r0 = *(u32*)(r28 + r27);
    if ((u32)r0 == (u32)0x0) goto L_800931B0;
    r0 = 0x1;
    goto L_80093260;
L_800931B0: ;
    r3 = 0x44a0;
    r4 = 0x20;
    ((void(*)(void))fn_800E2C04)();
    r29 = r3;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_800931E0;
    r3 = (u32)&lbl_8026F5A8;
    r4 = 0x1dd;
    r3 = (u32)&lbl_8026F5A8;
    r5 = (u32)&lbl_8047C1E8;
    ((void(*)(void))fn_80196E10)();
L_800931E0: ;
    r3 = r29;
    ((void(*)(void))fn_800E27B0)();
    r29 = r3;
    r4 = 0x0;
    r5 = 0x4490;
    memset((void*)r3, (int)r4, (u32)r5);
    *(u32*)(r28 + r27) = r29;
    r3 = (u32)fn_80093B04;
    r5 = (u32)fn_80093B04;
    r0 = 0x0;
    r27 = *(u32*)(r28 + r27);
    r3 = r30;
    *(u32*)((u8*)r27 + 0x4340) = r0;
    r4 = r27 + 0x20;
    *(u32*)((u8*)r27 + 0x4338) = r30;
    ((void(*)(void))fn_800716C8)();
    r3 = r27;
    ((void(*)(void))fn_8009F77C)();
    r3 = r27 + 0x18;
    ((void(*)(void))fn_8009F9C8)();
    r3 = (u32)fn_800937F4;
    r5 = r27;
    r4 = (u32)fn_800937F4;
    r6 = r27 + 0x4338;
    r3 = r27 + 0x20;
    r7 = 0x4000;
    r8 = 0x8;
    r9 = 0x0;
    ((void(*)(void))fn_800A19CC)();
    r3 = r27 + 0x20;
    ((void(*)(void))fn_800A1F94)();
    r0 = 0x1;
L_80093260: ;
    if ((s32)r0 != (s32)0x0) goto L_80093270;
    r3 = 0x0;
    goto L_800932DC;
L_80093270: ;
    r3 = (u32)&lbl_803FB328;
    r0 = r30 << 2;
    r3 = (u32)&lbl_803FB328;
    r27 = 0x0;
    r28 = *(u32*)(r3 + r0);
    r3 = r28;
    ((void(*)(void))fn_8009F7B4)();
    r0 = *(u32*)((u8*)r28 + 0x4340);
    if ((s32)r0 != (s32)0x0) goto L_800932B4;
    r0 = 0x2;
    r3 = (0x3 << 16);
    *(u32*)((u8*)r28 + 0x4340) = r0;
    r0 = r3 + 0x2;
    r27 = 0x1;
    *(u32*)((u8*)r28 + 0x433C) = r0;
    *(u32*)((u8*)r28 + 0x4344) = r31;
L_800932B4: ;
    r3 = r28;
    ((void(*)(void))fn_8009F890)();
    r3 = r28 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    if ((s32)r27 == (s32)0x0) goto L_800932D8;
    r3 = r28 + 0x18;
    ((void(*)(void))fn_8009FABC)();
L_800932D8: ;
    r3 = r27;
L_800932DC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x800932F0 | size: 0x1F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_800932F0(void) {
    extern void fn_800937F4();
    extern void fn_80093B04();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    if ((s32)r29 < (s32)0x0) goto L_8009331C;
    if ((s32)r29 <= (s32)0x3) goto L_80093324;
L_8009331C: ;
    r0 = 0x0;
    goto L_800933F4;
L_80093324: ;
    r3 = (u32)&lbl_803FB328;
    r26 = r29 << 2;
    r27 = (u32)&lbl_803FB328;
    r0 = *(u32*)(r27 + r26);
    if ((u32)r0 == (u32)0x0) goto L_80093344;
    r0 = 0x1;
    goto L_800933F4;
L_80093344: ;
    r3 = 0x44a0;
    r4 = 0x20;
    ((void(*)(void))fn_800E2C04)();
    r28 = r3;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_80093374;
    r3 = (u32)&lbl_8026F5A8;
    r4 = 0x1dd;
    r3 = (u32)&lbl_8026F5A8;
    r5 = (u32)&lbl_8047C1E8;
    ((void(*)(void))fn_80196E10)();
L_80093374: ;
    r3 = r28;
    ((void(*)(void))fn_800E27B0)();
    r28 = r3;
    r4 = 0x0;
    r5 = 0x4490;
    memset((void*)r3, (int)r4, (u32)r5);
    *(u32*)(r27 + r26) = r28;
    r3 = (u32)fn_80093B04;
    r5 = (u32)fn_80093B04;
    r0 = 0x0;
    r26 = *(u32*)(r27 + r26);
    r3 = r29;
    *(u32*)((u8*)r26 + 0x4340) = r0;
    r4 = r26 + 0x20;
    *(u32*)((u8*)r26 + 0x4338) = r29;
    ((void(*)(void))fn_800716C8)();
    r3 = r26;
    ((void(*)(void))fn_8009F77C)();
    r3 = r26 + 0x18;
    ((void(*)(void))fn_8009F9C8)();
    r3 = (u32)fn_800937F4;
    r5 = r26;
    r4 = (u32)fn_800937F4;
    r6 = r26 + 0x4338;
    r3 = r26 + 0x20;
    r7 = 0x4000;
    r8 = 0x8;
    r9 = 0x0;
    ((void(*)(void))fn_800A19CC)();
    r3 = r26 + 0x20;
    ((void(*)(void))fn_800A1F94)();
    r0 = 0x1;
L_800933F4: ;
    if ((s32)r0 != (s32)0x0) goto L_80093404;
    r3 = 0x0;
    goto L_800934D0;
L_80093404: ;
    r3 = (u32)&lbl_803FB328;
    r0 = r29 << 2;
    r4 = (u32)&lbl_803FB328;
    r3 = r30;
    r27 = *(u32*)(r4 + r0);
    r26 = 0x0;
    r3 = (u32)strlen((const char*)r3);
    r29 = r3;
    if ((u32)r31 == (u32)0x0) goto L_80093438;
    r3 = r31;
    r3 = (u32)strlen((const char*)r3);
    goto L_8009343C;
L_80093438: ;
    r3 = 0x0;
L_8009343C: ;
    if ((u32)r29 >= (u32)0x7f) goto L_8009344C;
    if ((u32)r3 < (u32)0x7f) goto L_80093454;
L_8009344C: ;
    r26 = 0x0;
    goto L_800934CC;
L_80093454: ;
    r3 = r27;
    ((void(*)(void))fn_8009F7B4)();
    r0 = *(u32*)((u8*)r27 + 0x4340);
    if ((s32)r0 != (s32)0x0) goto L_800934A8;
    r26 = 0x1;
    r3 = (0x3 << 16);
    *(u32*)((u8*)r27 + 0x4340) = r26;
    r0 = r3 + 0x1;
    r4 = r30;
    r3 = r27 + 0x4344;
    *(u32*)((u8*)r27 + 0x433C) = r0;
    ((void(*)(void))fn_800CA968)();
    if ((u32)r31 == (u32)0x0) goto L_800934A0;
    r4 = r31;
    r3 = r27 + 0x43c4;
    ((void(*)(void))fn_800CA968)();
    goto L_800934A8;
L_800934A0: ;
    r0 = 0x0;
    *(u8*)((u8*)r27 + 0x43C4) = r0;
L_800934A8: ;
    r3 = r27;
    ((void(*)(void))fn_8009F890)();
    r3 = r27 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    if ((s32)r26 == (s32)0x0) goto L_800934CC;
    r3 = r27 + 0x18;
    ((void(*)(void))fn_8009FABC)();
L_800934CC: ;
    r3 = r26;
L_800934D0: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x800934E4 | size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_800934E4(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if ((s32)r3 < (s32)0x0) goto L_80093508;
    if ((s32)r3 <= (s32)0x3) goto L_80093510;
L_80093508: ;
    r3 = 0x0;
    goto L_8009355C;
L_80093510: ;
    r4 = (u32)&lbl_803FB328;
    r0 = r3 << 2;
    r3 = (u32)&lbl_803FB328;
    r30 = *(u32*)(r3 + r0);
    if ((u32)r30 == (u32)0x0) goto L_80093554;
    r3 = r30;
    ((void(*)(void))fn_8009F7B4)();
    r0 = *(u32*)((u8*)r30 + 0x4340);
    r3 = r30;
    r0 = __cntlzw(r0);
    r31 = (u32)r0 >> 5;
    ((void(*)(void))fn_8009F890)();
    r3 = r30 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    goto L_80093558;
L_80093554: ;
    r31 = 0x1;
L_80093558: ;
    r3 = r31;
L_8009355C: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80093574 | size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80093574(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if ((s32)r3 < (s32)0x0) goto L_80093598;
    if ((s32)r3 <= (s32)0x3) goto L_800935A0;
L_80093598: ;
    r3 = (0x1 << 16);
    goto L_800935F8;
L_800935A0: ;
    r4 = (u32)&lbl_803FB328;
    r0 = r3 << 2;
    r3 = (u32)&lbl_803FB328;
    r30 = *(u32*)(r3 + r0);
    if ((u32)r30 != (u32)0x0) goto L_800935C0;
    r3 = 0x0;
    goto L_800935F8;
L_800935C0: ;
    r3 = r30;
    ((void(*)(void))fn_8009F7B4)();
    r31 = *(u32*)((u8*)r30 + 0x433C);
    r3 = r30;
    ((void(*)(void))fn_8009F890)();
    r3 = r30 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    r0 = (u32)r31 >> 16;
    if ((s32)r0 != (s32)0x3) goto L_800935F4;
    ((void(*)(void))fn_800F0308)();
    goto L_800935C0;
L_800935F4: ;
    r3 = r31;
L_800935F8: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80093610 | size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80093610(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if ((s32)r3 < (s32)0x0) goto L_80093634;
    if ((s32)r3 <= (s32)0x3) goto L_8009363C;
L_80093634: ;
    r3 = (0x1 << 16);
    goto L_80093680;
L_8009363C: ;
    r4 = (u32)&lbl_803FB328;
    r0 = r3 << 2;
    r3 = (u32)&lbl_803FB328;
    r30 = *(u32*)(r3 + r0);
    if ((u32)r30 != (u32)0x0) goto L_8009365C;
    r3 = 0x0;
    goto L_80093680;
L_8009365C: ;
    r3 = r30;
    ((void(*)(void))fn_8009F7B4)();
    r31 = *(u32*)((u8*)r30 + 0x433C);
    r3 = r30;
    ((void(*)(void))fn_8009F890)();
    r3 = r30 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    r3 = r31;
L_80093680: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80093698 | size: 0x15C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80093698(void) {
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if ((s32)r3 < (s32)0x0) goto L_800936C4;
    if ((s32)r3 <= (s32)0x3) goto L_800936CC;
L_800936C4: ;
    r3 = 0x0;
    goto L_800937D4;
L_800936CC: ;
    r4 = (u32)&lbl_803FB328;
    r30 = r3 << 2;
    r31 = (u32)&lbl_803FB328;
    r28 = *(u32*)(r31 + r30);
    if ((u32)r28 != (u32)0x0) goto L_800936EC;
    r3 = 0x1;
    goto L_800937D4;
L_800936EC: ;
    r3 = *(u32*)((u8*)r28 + 0x4338);
    r4 = 0x1;
    ((void(*)(void))fn_800716E8)();
L_800936F8: ;
    r3 = r28;
    ((void(*)(void))fn_8009F7B4)();
    r29 = *(u32*)((u8*)r28 + 0x433C);
    r3 = r28;
    ((void(*)(void))fn_8009F890)();
    r3 = r28 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    r0 = (u32)r29 >> 16;
    if ((s32)r0 != (s32)0x3) goto L_8009372C;
    ((void(*)(void))fn_800F0308)();
    goto L_800936F8;
L_8009372C: ;
    r3 = r28;
    ((void(*)(void))fn_8009F7B4)();
    r0 = 0xd;
    r3 = (0x3 << 16);
    *(u32*)((u8*)r28 + 0x4340) = r0;
    r0 = r3 + 0xd;
    r3 = r28;
    *(u32*)((u8*)r28 + 0x433C) = r0;
    ((void(*)(void))fn_8009F890)();
    r3 = r28 + 0x20;
    r4 = 0x8;
    ((void(*)(void))fn_800A257C)();
    r3 = r28 + 0x18;
    ((void(*)(void))fn_8009FABC)();
    r3 = r28 + 0x20;
    r4 = 0x0;
    ((void(*)(void))fn_800A1E54)();
    r3 = *(u32*)((u8*)r28 + 0x4338);
    r4 = 0x0;
    r5 = 0x0;
    ((void(*)(void))fn_800716C8)();
    r3 = *(u32*)((u8*)r28 + 0x4338);
    r4 = 0x0;
    ((void(*)(void))fn_800716E8)();
    r3 = *(u32*)(r31 + r30);
    ((void(*)(void))fn_800E202C)();
    r29 = r3;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_800937B8;
    r3 = (u32)&lbl_8026F5A8;
    r4 = 0x1e6;
    r3 = (u32)&lbl_8026F5A8;
    r5 = (u32)&lbl_8047C1E8;
    ((void(*)(void))fn_80196E10)();
L_800937B8: ;
    r3 = r29;
    ((void(*)(void))fn_800E24B0)();
    r3 = r29;
    ((void(*)(void))fn_800E209C)();
    r0 = 0x0;
    r3 = 0x1;
    *(u32*)(r31 + r30) = r0;
L_800937D4: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

#pragma pop
