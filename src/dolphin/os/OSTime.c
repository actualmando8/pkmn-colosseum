#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSInterrupt.h"

/*
 * OSTime.c - Time base access functions.
 *
 * OSGetTime reads the PowerPC time base register pair (TBU/TBL).
 * OSGetTick reads just the lower 32 bits (TBL).
 * __OSGetSystemTime adds the system time bias stored at 0x800030D8.
 *
 * Matches: 0x800A2778 - 0x800A27F8
 */

#define OS_TIME_BASE_HI (*(volatile u32*)0x800030D8)
#define OS_TIME_BASE_LO (*(volatile u32*)0x800030DC)

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm s64 OSGetTime(void) {
    nofralloc
    mftbu r3
    mftb  r4
    mftbu r5
    cmpw  r3, r5
    bne   OSGetTime
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm u32 OSGetTick(void) {
    nofralloc
    mftb r3
    blr
}
#pragma pop

s64 __OSGetSystemTime(void) {
    BOOL enabled;
    s64 time;
    s64 bias;

    enabled = OSDisableInterrupts();
    time = OSGetTime();
    bias = *(s64*)&OS_TIME_BASE_HI;
    time += bias;
    OSRestoreInterrupts(enabled);
    return time;
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800A27FC - 0x800A27FC | size: 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A27FC(void) {
    extern u8 lbl_80311878[];
    extern u8 lbl_803118A8[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;

    r5 = (0x9249 << 16);
    r0 = r5 + 0x2493;
    r7 = r3 + 0x6;
    r6 = (s32)((s64)r0 * (s64)r7 >> 32);
    r5 = (0xb38d << 16);
    /* subi r0, r5, 0x64f */;
    r0 = (s32)((s64)r0 * (s64)r3 >> 32);
    r5 = r6 + r7;
    r5 = (s32)r5 >> 2;
    r6 = (u32)r5 >> 31;
    r5 = r5 + r6;
    r0 = r0 + r3;
    r6 = r5 * 0x7;
    r0 = (s32)r0 >> 8;
    r5 = (u32)r0 >> 31;
    r5 = r0 + r5;
    r0 = r7 - r6;
    r11 = r5 * 0x16d;
    *(u32*)((u8*)r4 + 0x18) = r0;
    goto L_800A284C;
L_800A284C: ;
    r6 = (0x51ec << 16);
    /* subi r10, r6, 0x7ae1 */;
    goto L_800A2858;
L_800A2858: ;
    goto L_800A2864;
L_800A285C: ;
    /* subi r11, r11, 0x16d */;
    /* subi r5, r5, 0x1 */;
L_800A2864: ;
    if ((s32)r5 >= (s32)0x1) goto L_800A2874;
    r0 = 0x0;
    goto L_800A28A8;
L_800A2874: ;
    /* subi r0, r5, 0x1 */;
    r0 = (s32)((s64)r10 * (s64)r0 >> 32);
    r8 = (s32)r0 >> 7;
    r6 = (s32)r0 >> 5;
    r0 = r5 + 0x3;
    r7 = (u32)r6 >> 31;
    r0 = (s32)r0 >> 2;
    r9 = (u32)r8 >> 31;
    r6 = r6 + r7;
    /* addze r0, r0 */;
    r7 = r8 + r9;
    r0 = r0 - r6;
    r0 = r7 + r0;
L_800A28A8: ;
    r0 = r11 + r0;
    if ((s32)r3 < (s32)r0) goto L_800A285C;
    r6 = (s32)r5 >> 2;
    *(u32*)((u8*)r4 + 0x14) = r5;
    /* addze r6, r6 */;
    r6 = r6 << 2;
    r6 = r5 - r6;
    r0 = r3 - r0;
    *(u32*)((u8*)r4 + 0x1C) = r0;
    r7 = 0x1;
    r8 = 0x0;
    if ((s32)r6 != (s32)0x0) goto L_800A290C;
    r3 = (0x51ec << 16);
    /* subi r3, r3, 0x7ae1 */;
    r3 = (s32)((s64)r3 * (s64)r5 >> 32);
    r3 = (s32)r3 >> 5;
    r6 = (u32)r3 >> 31;
    r3 = r3 + r6;
    r3 = r3 * 0x64;
    r3 = r5 - r3;
    if ((s32)r3 == (s32)0x0) goto L_800A290C;
    r8 = r7;
L_800A290C: ;
    if ((s32)r8 != (s32)0x0) goto L_800A2940;
    r3 = (0x51ec << 16);
    /* subi r3, r3, 0x7ae1 */;
    r3 = (s32)((s64)r3 * (s64)r5 >> 32);
    r3 = (s32)r3 >> 7;
    r6 = (u32)r3 >> 31;
    r3 = r3 + r6;
    r3 = r3 * 0x190;
    r3 = r5 - r3;
    if ((s32)r3 == (s32)0x0) goto L_800A2940;
    r7 = 0x0;
L_800A2940: ;
    if ((s32)r7 == (s32)0x0) goto L_800A2954;
    r3 = (u32)lbl_803118A8;
    r6 = (u32)lbl_803118A8;
    goto L_800A295C;
L_800A2954: ;
    r3 = (u32)lbl_80311878;
    r6 = (u32)lbl_80311878;
L_800A295C: ;
    r7 = 0xc;
    r3 = 0x30;
    goto L_800A2968;
L_800A2968: ;
    goto L_800A296C;
L_800A296C: ;
    /* subi r3, r3, 0x4 */;
    r5 = *(u32*)(r6 + r3);
    /* subi r7, r7, 0x1 */;
    if ((s32)r0 < (s32)r5) goto L_800A296C;
    *(u32*)((u8*)r4 + 0x10) = r7;
    r3 = *(u32*)(r6 + r3);
    r3 = r0 - r3;
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0xC) = r0;
    return;
}
#pragma pop

/* fn_800A2998 - 0x800A2998 | size: 0x204 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800A2998(void) {
    extern void fn_800A27FC();
    extern void fn_800C4928();
    extern void fn_800C4B44();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    /* stmw r25, 0x1c(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r27 = (0x8000 << 16);
    r0 = *(u32*)((u8*)r27 + 0xF8);
    r3 = r29;
    r4 = r30;
    r6 = (u32)r0 >> 2;
    r5 = 0x0;
    fn_800C4B44();
    r26 = r3;
    r5 = 0x0;
    r25 = r4;
    /* xoris r4, r26, 0x8000 */;
    /* xoris r3, r5, 0x8000 */;
    r0 = r25 - r5;
    r3 = r4 - r3; /* -borrow */;
    r3 = r4 - r4; /* -borrow */;
    r3 = -r3;
    if ((s32)r3 == (s32)0x0) goto L_800A2A0C;
    r0 = *(u32*)((u8*)r27 + 0xF8);
    r0 = (u32)r0 >> 2;
    r25 = r25 + r0;
    r26 = r26 + r5; /* +carry */;
L_800A2A0C: ;
    r4 = 0x8;
    r3 = r26 * r4;
    r0 = (u32)((u64)r25 * (u64)r4 >> 32);
    r27 = (0x8000 << 16);
    r6 = *(u32*)((u8*)r27 + 0xF8);
    r5 = (0x431c << 16);
    /* subi r5, r5, 0x217d */;
    r6 = (u32)r6 >> 2;
    r5 = (u32)((u64)r5 * (u64)r6 >> 32);
    r6 = (u32)r5 >> 15;
    r28 = 0x0;
    r3 = r3 + r0;
    r0 = r25 * r28;
    r4 = r25 * r4;
    r3 = r3 + r0;
    r5 = 0x0;
    fn_800C4928();
    r5 = 0x0;
    r6 = 0x3e8;
    fn_800C4B44();
    *(u32*)((u8*)r31 + 0x24) = r4;
    r3 = (0x1062 << 16);
    r5 = r3 + 0x4dd3;
    r0 = *(u32*)((u8*)r27 + 0xF8);
    r3 = r26;
    r4 = r25;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r5 * (u64)r0 >> 32);
    r6 = (u32)r0 >> 6;
    r5 = 0x0;
    fn_800C4928();
    r5 = 0x0;
    r6 = 0x3e8;
    fn_800C4B44();
    *(u32*)((u8*)r31 + 0x20) = r4;
    r30 = r30 - r25;
    r29 = r29 - r26; /* -borrow */;
    r0 = *(u32*)((u8*)r27 + 0xF8);
    r5 = (0x1 << 16);
    r25 = r5 + 0x5180;
    r3 = r29;
    r6 = (u32)r0 >> 2;
    r4 = r30;
    r5 = 0x0;
    fn_800C4928();
    r6 = r25;
    r5 = 0x0;
    fn_800C4928();
    r5 = (0xb << 16);
    r0 = *(u32*)((u8*)r27 + 0xF8);
    r5 = r5 + 0x2575;
    r26 = r4 + r5;
    r6 = (u32)r0 >> 2;
    r0 = r3 + r28; /* +carry */;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    fn_800C4928();
    r6 = r25;
    r5 = 0x0;
    fn_800C4B44();
    r27 = r4;
    if ((s32)r27 >= (s32)0x0) goto L_800A2B18;
    r27 = r27 + (0x1 << 16);
    /* subi r26, r26, 0x1 */;
    r27 = r27 + 0x5180;
L_800A2B18: ;
    r3 = r26;
    r4 = r31;
    fn_800A27FC();
    r3 = (0x8889 << 16);
    /* subi r5, r3, 0x7777 */;
    r0 = (s32)((s64)r5 * (s64)r27 >> 32);
    r4 = r0 + r27;
    r0 = (s32)r4 >> 5;
    r3 = (u32)r0 >> 31;
    r7 = r0 + r3;
    r0 = (s32)((s64)r5 * (s64)r7 >> 32);
    r0 = r0 + r7;
    r5 = (s32)r0 >> 5;
    r0 = (s32)r0 >> 5;
    r3 = (u32)r0 >> 31;
    r3 = r0 + r3;
    r0 = (s32)r4 >> 5;
    r6 = (u32)r5 >> 31;
    r4 = r3 * 0x3c;
    r3 = (u32)r0 >> 31;
    r5 = r5 + r6;
    r0 = r0 + r3;
    *(u32*)((u8*)r31 + 0x8) = r5;
    r0 = r0 * 0x3c;
    r3 = r7 - r4;
    *(u32*)((u8*)r31 + 0x4) = r3;
    r0 = r27 - r0;
    *(u32*)((u8*)r31 + 0x0) = r0;
    /* lmw r25, 0x1c(r1) */;
    return;
}
#pragma pop

