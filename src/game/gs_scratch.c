/**
 * @file gs_scratch.c
 * @brief GSscratch (scratch/ARAM-backed allocator)
 *
 * Split from gs_range_800E202C.c (0x800EE928-0x800EEF48) — one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"
