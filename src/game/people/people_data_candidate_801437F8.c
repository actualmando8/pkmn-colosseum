/**
 * @file people_data_candidate_801437F8.c
 * @brief Item recovery, effort, and battle-status accessors.
 */
#include "game/people/people_data.h"

s32 itemParamGetReviveFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->reviveFlag != 0;
}

u8 itemParamGetAttackEffortUp(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->attackEffortUp;
}

u8 itemParamGetHPEffortUp(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->hpEffortUp;
}

s32 itemParamGetPPMaxUpFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->ppMaxUpFlag != 0;
}

s32 itemParamGetConfuseFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->confuseFlag != 0;
}

s32 itemParamGetParalyzeFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->paralyzeFlag != 0;
}

s32 itemParamGetFreezeFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->freezeFlag != 0;
}

s32 itemParamGetBurnFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->burnFlag != 0;
}

s32 itemParamGetPoisonFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->poisonFlag != 0;
}

s32 itemParamGetSleepFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->sleepFlag != 0;
}

s32 itemParamGetLevelUpFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->levelUpFlag != 0;
}

s32 itemParamGetGuardFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->guardFlag != 0;
}
