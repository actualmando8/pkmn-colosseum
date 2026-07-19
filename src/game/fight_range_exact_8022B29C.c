/**
 * @file fight_range_exact_8022B29C.c
 * @brief Exact pure-C fight target dispatcher at 0x8022B29C.
 */
#include "dolphin/types.h"

extern void fightFloorGetFightOutPokemonPtrRandom();

void fn_8022B29C(s32 arg)
{
    fightFloorGetFightOutPokemonPtrRandom(0, 1, 3, arg);
}
