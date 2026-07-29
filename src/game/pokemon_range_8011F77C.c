/**
 * @file pokemon_range_8011F77C.c
 * @brief Residual Pokemon candidate, 0x8011F77C - 0x8011F910.
 */
#define POKEMON_RANGE_SPLIT
#define POKEMON_RANGE_RESIDUAL_8011F77C
#include "src/game/pokemon_range_8011F5FC.c"

s32 pokemonGetDarkPokemonLevel(u8* pokemon)
{
    u16 divisor;
    f32 progress;
    f32 level;

    if (pokemon == NULL) {
        return 7;
    }
    if ((u8)pokemonGetStatus(pokemon, 0, 0xC2, 0) != 1) {
        return 7;
    }

    divisor = (u16)pokemonGetStatus(pokemon, 0, 0xC4, 0);
    if (divisor == 0) {
        divisor = 1;
    }
    if (pokemon == NULL) {
        progress = lbl_8047CFF0;
    } else {
        progress =
            (f32)pokemonGetStatus(pokemon, 0, 0xC5, 0) / lbl_8047CFF4;
    }

    if (progress < lbl_8047CFF0) {
        level = lbl_8047CFF0;
    } else {
        level = (lbl_8047CFF4 * progress) / (f32)divisor;
    }
    if (level >= lbl_8047CFF4) {
        return 0;
    }
    if (level >= lbl_8047CFF8) {
        return 1;
    }
    if (level >= lbl_8047CFFC) {
        return 2;
    }
    if (level >= lbl_8047D000) {
        return 3;
    }
    if (level >= lbl_8047D004) {
        return 4;
    }
    if (level > lbl_8047CFF0) {
        return 5;
    }
    return 6;
}
