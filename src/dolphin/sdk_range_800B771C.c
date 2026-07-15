/**
 * @file sdk_range_800B771C.c
 * @brief dolphin-sdk code, 0x800B771C - 0x800B856C (9 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

extern u32* gx;

#pragma optimize_for_size on
#pragma peephole off
void fn_800B7D3C(void) {
    volatile u32* gx32;
    volatile u8* gx8;

    gx32 = (u32*)gx;
    gx8 = (u8*)gx;

    gx32[0x5] = 0;
    gx32[0x5] = (gx32[0x5] & 0xFF9FFFFF) | 0x200;
    gx32[0x6] = 0;
    gx8[0x41C] = 0;
    gx8[0x41D] = 0;
    gx32[0x4F4 >> 2] |= 8;
}
#pragma peephole reset
#pragma optimize_for_size reset
