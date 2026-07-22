/* Canonical Dolphin SRAM unlock entry points. */
#include "dolphin/types.h"

void __OSUnlockSram(BOOL commit)
{
    extern void fn_800A09B0(BOOL commit, u32 arg);

    fn_800A09B0(commit, 0);
}

BOOL __OSUnlockSramEx(BOOL commit)
{
    extern BOOL fn_800A09B0(BOOL commit, u32 arg);

    return fn_800A09B0(commit, 0x14);
}
