/**
 * @file sdk_range_800C470C.c
 * @brief dolphin-sdk code, 0x800C470C - 0x800C4E44 (13 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

/*
 * Left shift for a 64-bit value represented as (r3:r4) by r5.
 */
void fn_800C4C50(u32 r3, u32 r4, u32 r5) {
    u32 r8 = 0x20 - r5;
    u32 r9 = r5 - 0x20;
    u32 r10 = 0;

    r3 = (r3 << r5) | (r4 >> r8);
    r10 = r4 << r9;
    r3 = r3 | r10;
    r4 = r4 << r5;
}

void fn_800C4C74(u32 r3, u32 r4, u32 r5) {
    u32 r8 = 0x20 - r5;
    u32 r9 = r5 - 0x20;
    u32 r10 = 0;

    r4 = (r4 >> r5) | (r3 << r8);
    r10 = (u32)(r3 >> r9);
    r4 = r4 | r10;
    r3 = r3 >> r5;
}

void __shr2i(u32 r3, u32 r4, u32 r5) {
    s32 r9;
    u32 r8 = 0x20 - r5;

    r4 = (r4 >> r5) | (r3 << r8);
    r9 = (s32)r5 - 0x20;

    if (r9 > 0) {
        r4 |= (u32)((s32)r3 >> r9);
    }
    r3 = (s32)r3 >> r5;
}
