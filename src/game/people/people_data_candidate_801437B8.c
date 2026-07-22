/**
 * @file people_data_candidate_801437B8.c
 * @brief Item PP-selection flag accessor.
 */
#include "game/people/people_data.h"

s32 itemParamGetPPSelectFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->ppSelectFlag != 0;
}
