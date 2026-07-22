/**
 * @file people_item_effort_exact_80143718.c
 * @brief Strict item effort-value accessors, 0x80143718 - 0x80143778.
 */
#include "dolphin/types.h"

u8 itemParamGetSpAttackEffortUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0xF];
}

u8 itemParamGetSpDefenceEffortUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0xE];
}

u8 itemParamGetQuickEffortUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0xD];
}

u8 itemParamGetDefenceEffortUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0xC];
}
