/**
 * @file people_data_candidate_80143778.c
 * @brief Item evolution-flag accessor.
 */
#include "game/people/people_data.h"

s32 itemParamGetEvolutionFlag(const ItemParamData* item)
{
    if (item == NULL) {
        return 0;
    }
    return item->evolutionFlag != 0;
}
