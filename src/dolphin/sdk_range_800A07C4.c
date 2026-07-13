/**
 * @file sdk_range_800A07C4.c
 * @brief dolphin-sdk code, 0x800A07C4 - 0x800A13E8 (19 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

void __OSUnlockSram(BOOL commit) {
    extern void fn_800A09B0(BOOL commit, u32 arg);

    fn_800A09B0(commit, 0);
}

BOOL __OSSyncSram(void) {
    extern u32 Scb_803FB840[0x54 / sizeof(u32)];

    return ((u32*)Scb_803FB840)[0x13];
}

BOOL __OSUnlockSramEx(BOOL commit) {
    extern BOOL fn_800A09B0(BOOL commit, u32 arg);

    return fn_800A09B0(commit, 0x14);
}

void fn_800A128C(void) {
}
