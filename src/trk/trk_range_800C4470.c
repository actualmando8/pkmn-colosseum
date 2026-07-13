/**
 * @file trk_range_800C4470.c
 * @brief trk code, 0x800C4470 - 0x800C459C (4 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

extern s32 OSDisableInterrupts(void);
extern s32 OSRestoreInterrupts(u32 state);

/* fn_800C4548 - 0x800C4548 | size: 0x24 | scope global */
void fn_800C4548(u32* state) {
    OSRestoreInterrupts(*state);
}

/* fn_800C456C - 0x800C456C | size: 0x30 | scope global */
void fn_800C456C(u32* state) {
    *state = OSDisableInterrupts();
}
