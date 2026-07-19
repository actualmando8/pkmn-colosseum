#include "dolphin/types.h"

extern u32 pokemonGetStatus(
    u32 object, u32 data_id, u32 status, u32 index);

u8 pokemonIsDarkPokemon(u32 pokemon)
{
    return (u8)pokemonGetStatus(pokemon, 0, 0xc2, 0);
}
