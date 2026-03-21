#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSContext.h"

extern void* memset(void* dest, int val, u32 n);

/*
 * OSInterrupt.c - Interrupt management.
 *
 * Manages the interrupt handler table, interrupt masking, and provides
 * the external interrupt exception handler.
 *
 * Matches: 0x8009DF3C - 0x8009E7A4
 */

static __OSInterruptHandler* InterruptHandlerTable;

/* SDA symbol aliases used by stub functions */
extern u32 InterruptHandlerTable_8047A710;
extern u16 __OSLastInterrupt;
extern u32 __OSLastInterruptTime;
extern u32 __OSLastInterruptSrr0;

/* Hardware registers */
#define PI_INTMR    (*(volatile u32*)0xCC003004)

extern void __OSDispatchInterrupt(u8 exception, OSContext* context);

/* __OSSetInterruptMask - translates OS interrupt bits to hardware register writes.
 * This is fn_8009E02C in the disassembly, a large function that maps OS-level
 * interrupt mask bits to the appropriate PI/DSP/AI/SI/EXI hardware registers.
 * Takes the changed mask bits and the combined global mask, returns remaining
 * unprocessed bits.
 */
extern u32 __OSSetInterruptMask(u32 mask, u32 globalMask);

/* Forward declaration */
static void ExternalInterruptHandler(u8 exception, OSContext* context);

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm BOOL OSDisableInterrupts(void) {
    nofralloc
    mfmsr   r3
    rlwinm  r4, r3, 0, 17, 15  /* clear MSR[EE] */
    mtmsr   r4
    extrwi  r3, r3, 1, 16      /* extract MSR[EE] bit */
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm BOOL OSEnableInterrupts(void) {
    nofralloc
    mfmsr   r3
    ori     r4, r3, 0x8000      /* set MSR[EE] */
    mtmsr   r4
    extrwi  r3, r3, 1, 16       /* extract old MSR[EE] bit */
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm BOOL OSRestoreInterrupts(register BOOL level) {
    nofralloc
    cmpwi   r3, 0
    mfmsr   r4
    beq     _disable
    ori     r5, r4, 0x8000       /* set MSR[EE] */
    b       _set
_disable:
    rlwinm  r5, r4, 0, 17, 15   /* clear MSR[EE] */
_set:
    mtmsr   r5
    extrwi  r3, r4, 1, 16       /* return old MSR[EE] bit */
    blr
}
#pragma pop

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler) {
    __OSInterruptHandler old;

    old = InterruptHandlerTable[interrupt];
    InterruptHandlerTable[interrupt] = handler;
    return old;
}

__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt) {
    return InterruptHandlerTable[interrupt];
}

void __OSInterruptInit(void) {
    InterruptHandlerTable = (__OSInterruptHandler*)0x80003040;
    memset(InterruptHandlerTable, 0, 0x80);

    *(volatile u32*)0x800000C4 = 0;
    *(volatile u32*)0x800000C8 = 0;
    PI_INTMR = 0xF0;

    __OSMaskInterrupts(0xFFFFFFE0);
    __OSSetExceptionHandler(OS_EXCEPTION_EXTERNAL_INTERRUPT,
                            (__OSExceptionHandler)ExternalInterruptHandler);
}

u32 __OSMaskInterrupts(u32 mask) {
    BOOL enabled;
    u32 prev;
    u32 local;
    u32 global;

    enabled = OSDisableInterrupts();

    prev  = *(volatile u32*)0x800000C4;
    local = *(volatile u32*)0x800000C8;
    mask &= ~(prev | local);
    prev |= mask;
    *(volatile u32*)0x800000C4 = prev;
    global = prev | local;

    while (mask != 0) {
        mask = __OSSetInterruptMask(mask, global);
    }

    OSRestoreInterrupts(enabled);
    return prev;
}

u32 __OSUnmaskInterrupts(u32 mask) {
    BOOL enabled;
    u32 prev;
    u32 local;
    u32 global;

    enabled = OSDisableInterrupts();

    prev  = *(volatile u32*)0x800000C4;
    local = *(volatile u32*)0x800000C8;
    mask &= (prev | local);
    prev &= ~mask;
    *(volatile u32*)0x800000C4 = prev;
    global = prev | local;

    while (mask != 0) {
        mask = __OSSetInterruptMask(mask, global);
    }

    OSRestoreInterrupts(enabled);
    return prev;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm static void ExternalInterruptHandler(register u8 exception, register OSContext* context) {
    nofralloc
    stw     r0,   0x0000(r4)
    stw     r1,   0x0004(r4)
    stw     r2,   0x0008(r4)
    stmw    r6,   0x0018(r4)
    mfspr   r0, GQR1
    stw     r0, 0x01A8(r4)
    mfspr   r0, GQR2
    stw     r0, 0x01AC(r4)
    mfspr   r0, GQR3
    stw     r0, 0x01B0(r4)
    mfspr   r0, GQR4
    stw     r0, 0x01B4(r4)
    mfspr   r0, GQR5
    stw     r0, 0x01B8(r4)
    mfspr   r0, GQR6
    stw     r0, 0x01BC(r4)
    mfspr   r0, GQR7
    stw     r0, 0x01C0(r4)
    stwu    r1, -0x0008(r1)
    b       __OSDispatchInterrupt
}
#pragma pop

/* ===================================================================
 * Stub functions for coverage -- TODO: decompile
 * 2 function(s)
 * =================================================================== */

/* fn_8009DFA4 - 0x8009DFA4 | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8009DFA4(void) {
    u32 r0 = 0;
    u32 r3 = 0;

    r0 = (s16)r3;
    r3 = *(u32*)InterruptHandlerTable_8047A710;
    r0 = r0 << 2;
    r3 = *(u32*)(r3 + r0);
    return;
}
#pragma pop

/* fn_8009E414 - 0x8009E414 | size: 0x344 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8009E414(void) {
    extern u8 lbl_803117E8[];
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r4;
    r3 = (0xcc00 << 16);
    r31 = *(u32*)((u8*)r3 + 0x3000);
    r31 = r31 & 0xFFFEFFFF;
    if ((u32)r31 == (u32)0x0) goto L_8009E458;
    r3 = r3 + 0x3000;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = r31 & r0;
    if ((u32)r0 != (u32)0x0) goto L_8009E460;
L_8009E458: ;
    r3 = r30;
    OSLoadContext((OSContext*)r3);
L_8009E460: ;
    r0 = r31 & 0x00000080;
    r0 = 0x0;
    if ((u32)r0 == (u32)0x0) goto L_8009E4CC;
    r3 = (0xcc00 << 16);
    r3 = r3 + 0x4000;
    r4 = *(u16*)((u8*)r3 + 0x1E);
    r3 = r4 & 0x1;
    if ((u32)r3 == (u32)0x0) goto L_8009E48C;
    r0 = r0 | (0x8000 << 16);
L_8009E48C: ;
    r3 = r4 & 0x00000002;
    if ((u32)r3 == (u32)0x0) goto L_8009E49C;
    r0 = r0 | (0x4000 << 16);
L_8009E49C: ;
    r3 = r4 & 0x00000004;
    if ((u32)r3 == (u32)0x0) goto L_8009E4AC;
    r0 = r0 | (0x2000 << 16);
L_8009E4AC: ;
    r3 = r4 & 0x00000008;
    if ((u32)r3 == (u32)0x0) goto L_8009E4BC;
    r0 = r0 | (0x1000 << 16);
L_8009E4BC: ;
    r3 = r4 & 0x00000010;
    if ((u32)r3 == (u32)0x0) goto L_8009E4CC;
    r0 = r0 | (0x800 << 16);
L_8009E4CC: ;
    r3 = r31 & 0x00000040;
    if ((u32)r3 == (u32)0x0) goto L_8009E514;
    r3 = (0xcc00 << 16);
    r3 = r3 + 0x5000;
    r4 = *(u16*)((u8*)r3 + 0xA);
    r3 = r4 & 0x00000008;
    if ((u32)r3 == (u32)0x0) goto L_8009E4F4;
    r0 = r0 | (0x400 << 16);
L_8009E4F4: ;
    r3 = r4 & 0x00000020;
    if ((u32)r3 == (u32)0x0) goto L_8009E504;
    r0 = r0 | (0x200 << 16);
L_8009E504: ;
    r3 = r4 & 0x00000080;
    if ((u32)r3 == (u32)0x0) goto L_8009E514;
    r0 = r0 | (0x100 << 16);
L_8009E514: ;
    r3 = r31 & 0x00000020;
    if ((u32)r3 == (u32)0x0) goto L_8009E538;
    r3 = (0xcc00 << 16);
    r3 = *(u32*)((u8*)r3 + 0x6C00);
    r3 = r3 & 0x00000008;
    if ((u32)r3 == (u32)0x0) goto L_8009E538;
    r0 = r0 | (0x80 << 16);
L_8009E538: ;
    r3 = r31 & 0x00000010;
    if ((u32)r3 == (u32)0x0) goto L_8009E5E4;
    r3 = (0xcc00 << 16);
    r4 = *(u32*)((u8*)r3 + 0x6800);
    r3 = r4 & 0x00000002;
    if ((u32)r3 == (u32)0x0) goto L_8009E55C;
    r0 = r0 | (0x40 << 16);
L_8009E55C: ;
    r3 = r4 & 0x00000008;
    if ((u32)r3 == (u32)0x0) goto L_8009E56C;
    r0 = r0 | (0x20 << 16);
L_8009E56C: ;
    r3 = r4 & 0x00000800;
    if ((u32)r3 == (u32)0x0) goto L_8009E57C;
    r0 = r0 | (0x10 << 16);
L_8009E57C: ;
    r3 = (0xcc00 << 16);
    r3 = r3 + 0x6800;
    r4 = *(u32*)((u8*)r3 + 0x14);
    r3 = r4 & 0x00000002;
    if ((u32)r3 == (u32)0x0) goto L_8009E598;
    r0 = r0 | (0x8 << 16);
L_8009E598: ;
    r3 = r4 & 0x00000008;
    if ((u32)r3 == (u32)0x0) goto L_8009E5A8;
    r0 = r0 | (0x4 << 16);
L_8009E5A8: ;
    r3 = r4 & 0x00000800;
    if ((u32)r3 == (u32)0x0) goto L_8009E5B8;
    r0 = r0 | (0x2 << 16);
L_8009E5B8: ;
    r3 = (0xcc00 << 16);
    r3 = r3 + 0x6800;
    r4 = *(u32*)((u8*)r3 + 0x28);
    r3 = r4 & 0x00000002;
    if ((u32)r3 == (u32)0x0) goto L_8009E5D4;
    r0 = r0 | (0x1 << 16);
L_8009E5D4: ;
    r3 = r4 & 0x00000008;
    if ((u32)r3 == (u32)0x0) goto L_8009E5E4;
    r0 = r0 | 0x8000;
L_8009E5E4: ;
    r3 = r31 & 0x00002000;
    if ((u32)r3 == (u32)0x0) goto L_8009E5F4;
    r0 = r0 | 0x20;
L_8009E5F4: ;
    r3 = r31 & 0x00001000;
    if ((u32)r3 == (u32)0x0) goto L_8009E604;
    r0 = r0 | 0x40;
L_8009E604: ;
    r3 = r31 & 0x00000400;
    if ((u32)r3 == (u32)0x0) goto L_8009E614;
    r0 = r0 | 0x1000;
L_8009E614: ;
    r3 = r31 & 0x00000200;
    if ((u32)r3 == (u32)0x0) goto L_8009E624;
    r0 = r0 | 0x2000;
L_8009E624: ;
    r3 = r31 & 0x00000100;
    if ((u32)r3 == (u32)0x0) goto L_8009E634;
    r0 = r0 | 0x80;
L_8009E634: ;
    r3 = r31 & 0x00000008;
    if ((u32)r3 == (u32)0x0) goto L_8009E644;
    r0 = r0 | 0x800;
L_8009E644: ;
    r3 = r31 & 0x00000004;
    if ((u32)r3 == (u32)0x0) goto L_8009E654;
    r0 = r0 | 0x400;
L_8009E654: ;
    r3 = r31 & 0x00000002;
    if ((u32)r3 == (u32)0x0) goto L_8009E664;
    r0 = r0 | 0x200;
L_8009E664: ;
    r3 = r31 & 0x00000800;
    if ((u32)r3 == (u32)0x0) goto L_8009E674;
    r0 = r0 | 0x4000;
L_8009E674: ;
    r3 = r31 & 0x1;
    if ((u32)r3 == (u32)0x0) goto L_8009E684;
    r0 = r0 | 0x100;
L_8009E684: ;
    r3 = (0x8000 << 16);
    r4 = *(u32*)((u8*)r3 + 0xC4);
    r3 = *(u32*)((u8*)r3 + 0xC8);
    r3 = r4 | r3;
    r4 = r0 & ~r3;
    if ((u32)r4 == (u32)0x0) goto L_8009E734;
    r3 = (u32)lbl_803117E8;
    r0 = (u32)lbl_803117E8;
    r3 = r0;
    goto L_8009E6B0;
L_8009E6B0: ;
    goto L_8009E6B4;
L_8009E6B4: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    r0 = r4 & r0;
    if ((u32)r0 == (u32)0x0) goto L_8009E6D0;
    r0 = __cntlzw(r0);
    r29 = (s16)r0;
    goto L_8009E6D8;
L_8009E6D0: ;
    r3 = r3 + 0x4;
    goto L_8009E6B4;
L_8009E6D8: ;
    r3 = *(u32*)InterruptHandlerTable_8047A710;
    r0 = r29 << 2;
    r31 = *(u32*)(r3 + r0);
    if ((u32)r31 == (u32)0x0) goto L_8009E734;
    if ((s32)r29 <= (s32)0x4) goto L_8009E70C;
    *(u16*)__OSLastInterrupt = r29;
    OSGetTime();
    *((u32*)&__OSLastInterruptTime + 1) = r4;
    *(u32*)__OSLastInterruptTime = r3;
    r0 = *(u32*)((u8*)r30 + 0x198);
    *(u32*)__OSLastInterruptSrr0 = r0;
L_8009E70C: ;
    OSDisableScheduler();
    r3 = r29;
    r4 = r30;
    r12 = r31;
    /* blrl  */;
    OSEnableScheduler();
    __OSReschedule();
    r3 = r30;
    OSLoadContext((OSContext*)r3);
L_8009E734: ;
    r3 = r30;
    OSLoadContext((OSContext*)r3);
    return;
}
#pragma pop

