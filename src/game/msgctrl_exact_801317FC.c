/**
 * @file msgctrl_exact_801317FC.c
 * @brief Strict message-control getters, 0x801317FC - 0x8013182C.
 */
#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"

void msgctrlPokemonID(void)
{
    pokemonDataBiosGetPtr(lbl_8047AE90);
    pokemonDataBiosGetName();
}

u32 msgctrlString(void)
{
    return lbl_8047AE88;
}
