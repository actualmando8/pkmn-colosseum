#include "dolphin/types.h"

/*
 * mwtrace.c - CRT library functions.
 *
 * Stub implementations for function coverage.
 */

/* fn_800C4548 - 0x800C4548 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4548(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = *(u32*)((u8*)r3 + 0x0);
    OSRestoreInterrupts();
    return;
}
#pragma pop

/* fn_800C456C - 0x800C456C | size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C456C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    OSDisableInterrupts();
    *(u32*)((u8*)r31 + 0x0) = r3;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* fn_800C459C - 0x800C459C | size: 0x4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800C459C(void) {
    nofralloc
    blr
}
#pragma pop

