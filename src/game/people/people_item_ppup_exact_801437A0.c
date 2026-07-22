/**
 * @file people_item_ppup_exact_801437A0.c
 * @brief Strict PP-up value accessor, 0x801437A0 - 0x801437B8.
 */
#include "dolphin/types.h"

u8 itemParamGetPPUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0xB];
}
