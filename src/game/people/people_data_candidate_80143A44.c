/**
 * @file people_data_candidate_80143A44.c
 * @brief Item critical-hit and attraction flag accessors.
 */
#include "game/people/people_data.h"

s32 itemParamGetCriticalFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->criticalFlag != 0;
}

s32 itemParamGetMeromeroFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->meromeroFlag != 0;
}
