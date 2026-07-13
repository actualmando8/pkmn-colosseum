/**
 * @file gs_model_bound.c
 * @brief GSmodel bound-recalc + anim-blend interpolation (XD anim.c sub-range)
 *
 * Split from gs_range_800E202C.c (0x800EB268-0x800EBEEC) — one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"

void _modelBoundBeginSurface__F13GSgfxPrimTypeUsUlPv(s32 prim, u16 count, u32 attr, void *ctx) {
    (void)prim;
    (void)count;
    *(u32 *)((u8 *)ctx + 0xC) = attr;
}
