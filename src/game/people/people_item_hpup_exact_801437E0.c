/**
 * @file people_item_hpup_exact_801437E0.c
 * @brief Strict HP-up value accessor, 0x801437E0 - 0x801437F8.
 */
#include "dolphin/types.h"

u8 itemParamGetHPUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0xA];
}
