/**
 * @file people_item_battle_boost_exact_801439B8.c
 * @brief Strict item battle-stat accessors, 0x801439B8 - 0x80143A44.
 */
#include "dolphin/types.h"

u32 itemParamGetSpAttackUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0x2] & 0xF;
}

u32 itemParamGetHitUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return (p[0x2] >> 4) & 0xF;
}

u32 itemParamGetQuickUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return p[0x1] & 0xF;
}

u32 itemParamGetDefenceUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return (p[0x1] >> 4) & 0xF;
}

u32 itemParamGetAttackUp(u8* p)
{
    if (p == NULL) {
        return 0;
    }
    return (p[0x0] >> 1) & 0xF;
}
