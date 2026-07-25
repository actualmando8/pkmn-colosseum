/**
 * @file gs_mem.c
 * @brief GSmem (heap allocator)
 *
 * Split from gs_range_800E202C.c (0x800E202C-0x800E3604) — one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"

extern u32 lbl_8047AB2C;

s32 fn_800E2AF8(void)
{
    return 1;
}

void fn_800E3560(u32 value)
{
    lbl_8047AB2C = value;
}
