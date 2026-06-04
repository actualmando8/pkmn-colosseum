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
void fn_800A27FC(void) {
    extern u8 lbl_80311878[];
    extern u8 lbl_803118A8[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;

    r5 = 0x92490000;
    tmp = r5 + 0x2493;
    r7 = r3 + 0x6;
    r6 = (s32)((s64)tmp * (s64)r7 >> 32);
    r5 = 0xB38D0000;
    tmp = (s32)((s64)tmp * (s64)r3 >> 32);
    r5 = r6 + r7;
    r5 = (s32)r5 >> 2;
    r6 = (u32)r5 >> 31;
    r5 = r5 + r6;
    tmp = tmp + r3;
    r6 = r5 * 0x7;
    tmp = (s32)tmp >> 8;
    r5 = (u32)tmp >> 31;
    r5 = tmp + r5;
    tmp = r7 - r6;
    r11 = r5 * 0x16d;
    *(u32*)((u8*)r4 + 0x18) = tmp;


    r6 = 0x51EC0000;

    do {
        if ((s32)r5 < 1) {
            tmp = 0x0;
        } else {
            tmp = (s32)((s64)r10 * (s64)tmp >> 32);
            r8 = (s32)tmp >> 7;
            r6 = (s32)tmp >> 5;
            tmp = r5 + 0x3;
            r7 = (u32)r6 >> 31;
            tmp = (s32)tmp >> 2;
            r9 = (u32)r8 >> 31;
            r6 = r6 + r7;
            r7 = r8 + r9;
            tmp = tmp - r6;
            tmp = r7 + tmp;
        }
        tmp = r11 + tmp;
    } while ((s32)r3 < (s32)tmp);
    r6 = (s32)r5 >> 2;
    *(u32*)((u8*)r4 + 0x14) = r5;
    r6 = r6 << 2;
    r6 = r5 - r6;
    tmp = r3 - tmp;
    *(u32*)((u8*)r4 + 0x1C) = tmp;
    r7 = 0x1;
    r8 = 0x0;
    if ((s32)r6 == 0) {
        r3 = 0x51EC0000;
        r3 = (s32)((s64)r3 * (s64)r5 >> 32);
        r3 = (s32)r3 >> 5;
        r6 = (u32)r3 >> 31;
        r3 = r3 + r6;
        r3 = r3 * 0x64;
        r3 = r5 - r3;
        if ((s32)r3 != 0) {
            r8 = r7;
    }
    }
    if ((s32)r8 == 0) {
        r3 = 0x51EC0000;
        r3 = (s32)((s64)r3 * (s64)r5 >> 32);
        r3 = (s32)r3 >> 7;
        r6 = (u32)r3 >> 31;
        r3 = r3 + r6;
        r3 = r3 * 0x190;
        r3 = r5 - r3;
        if ((s32)r3 != 0) {
            r7 = 0x0;
    }
    }
    if ((s32)r7 != 0) {
        r3 = (u32)lbl_803118A8;
        r6 = (u32)lbl_803118A8;
    } else {

        r3 = (u32)lbl_80311878;
        r6 = (u32)lbl_80311878;
    }
    r7 = 0xc;
    r3 = 0x30;



    do {
        r5 = *(u32*)(r6 + r3);
    } while ((s32)tmp < (s32)r5);
    *(u32*)((u8*)r4 + 0x10) = r7;
    r3 = *(u32*)(r6 + r3);
    r3 = tmp - r3;
    tmp = r3 + 0x1;
    *(u32*)((u8*)r4 + 0xC) = tmp;
    return;
}

/* fn_800A2998 - 0x800A2998 | size: 0x204 */
void fn_800A2998(void) {
    extern void fn_800A27FC();
    extern void __div2i();
    extern void __mod2i();
    u32 tmp = 0;
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

    r29 = r3;
    r30 = r4;
    r31 = r5;
    r27 = 0x80000000;
    tmp = *(u32*)((u8*)r27 + 0xF8);
    r3 = r29;
    r4 = r30;
    r6 = (u32)tmp >> 2;
    r5 = 0x0;
    __mod2i();
    r26 = r3;
    r5 = 0x0;
    r25 = r4;
    tmp = r25 - r5;
    r3 = r4 - r3; /* -borrow */;
    r3 = r4 - r4; /* -borrow */;
    r3 = -r3;
    if ((s32)r3 != 0) {
        tmp = *(u32*)((u8*)r27 + 0xF8);
        tmp = (u32)tmp >> 2;
        r25 = r25 + tmp;
        r26 = r26 + r5; /* +carry */;
    }
    r4 = 0x8;
    r3 = r26 * r4;
    tmp = (u32)((u64)r25 * (u64)r4 >> 32);
    r27 = 0x80000000;
    r6 = *(u32*)((u8*)r27 + 0xF8);
    r5 = 0x431C0000;
    r6 = (u32)r6 >> 2;
    r5 = (u32)((u64)r5 * (u64)r6 >> 32);
    r6 = (u32)r5 >> 15;
    r28 = 0x0;
    r3 = r3 + tmp;
    tmp = r25 * r28;
    r4 = r25 * r4;
    r3 = r3 + tmp;
    r5 = 0x0;
    __div2i();
    r5 = 0x0;
    r6 = 0x3e8;
    __mod2i();
    *(u32*)((u8*)r31 + 0x24) = r4;
    r3 = 0x10620000;
    r5 = r3 + 0x4dd3;
    tmp = *(u32*)((u8*)r27 + 0xF8);
    r3 = r26;
    r4 = r25;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r5 * (u64)tmp >> 32);
    r6 = (u32)tmp >> 6;
    r5 = 0x0;
    __div2i();
    r5 = 0x0;
    r6 = 0x3e8;
    __mod2i();
    *(u32*)((u8*)r31 + 0x20) = r4;
    r30 = r30 - r25;
    r29 = r29 - r26; /* -borrow */;
    tmp = *(u32*)((u8*)r27 + 0xF8);
    r5 = 0x10000;
    r25 = r5 + 0x5180;
    r3 = r29;
    r6 = (u32)tmp >> 2;
    r4 = r30;
    r5 = 0x0;
    __div2i();
    r6 = r25;
    r5 = 0x0;
    __div2i();
    r5 = 0xB0000;
    tmp = *(u32*)((u8*)r27 + 0xF8);
    r5 = r5 + 0x2575;
    r26 = r4 + r5;
    r6 = (u32)tmp >> 2;
    tmp = r3 + r28; /* +carry */;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    __div2i();
    r6 = r25;
    r5 = 0x0;
    __mod2i();
    r27 = r4;
    if ((s32)r27 < 0) {
        r27 = r27 + (0x1 << 16);
        r27 = r27 + 0x5180;
    }
    r3 = r26;
    r4 = r31;
    fn_800A27FC();
    r3 = 0x88890000;
    tmp = (s32)((s64)r5 * (s64)r27 >> 32);
    r4 = tmp + r27;
    tmp = (s32)r4 >> 5;
    r3 = (u32)tmp >> 31;
    r7 = tmp + r3;
    tmp = (s32)((s64)r5 * (s64)r7 >> 32);
    tmp = tmp + r7;
    r5 = (s32)tmp >> 5;
    tmp = (s32)tmp >> 5;
    r3 = (u32)tmp >> 31;
    r3 = tmp + r3;
    tmp = (s32)r4 >> 5;
    r6 = (u32)r5 >> 31;
    r4 = r3 * 0x3c;
    r3 = (u32)tmp >> 31;
    r5 = r5 + r6;
    tmp = tmp + r3;
    *(u32*)((u8*)r31 + 0x8) = r5;
    tmp = tmp * 0x3c;
    r3 = r7 - r4;
    *(u32*)((u8*)r31 + 0x4) = r3;
    tmp = r27 - tmp;
    *(u32*)((u8*)r31 + 0x0) = tmp;
    return;
}

