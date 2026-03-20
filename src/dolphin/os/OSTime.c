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
asm void fn_800A27FC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A2998 - 0x800A2998 | size: 0x204 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A2998(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

