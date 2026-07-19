/**
 * @file battle_grid_exact_801C3108.c
 * @brief Exact battle-grid group-table accessor.
 */
#include "dolphin/types.h"

extern u8 lbl_80466DE8[];

void* battleGridGetPtr(void)
{
    return lbl_80466DE8;
}
