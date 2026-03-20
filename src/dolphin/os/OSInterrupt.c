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
    /* TODO: decompile -- 20 bytes at 0x8009DFA4 */
}
#pragma pop

/* fn_8009E414 - 0x8009E414 | size: 0x344 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8009E414(void) {
    /* TODO: decompile -- 836 bytes at 0x8009E414 */
}
#pragma pop

