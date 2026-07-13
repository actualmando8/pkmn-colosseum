/**
 * @file sdk_range_800BA44C.c
 * @brief dolphin-sdk code, 0x800BA44C - 0x800BAE5C (11 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

void fn_800BAE34(u32 *arg0, u32 arg1, u32 arg2) {
    *arg0 = (*arg0 & 0xFFFFFFFCu) | arg1;
    *arg0 = (*arg0 & 0xFFFFFFF3u) | (arg2 << 2);
}
