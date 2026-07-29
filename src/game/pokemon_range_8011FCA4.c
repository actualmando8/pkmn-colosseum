/**
 * @file pokemon_range_8011FCA4.c
 * @brief Residual Pokemon candidate, 0x8011FCA4 - 0x80120B00 (shared bank).
 */
#define POKEMON_RANGE_SPLIT
#define POKEMON_RANGE_RESIDUAL_8011FCA4
#include "src/game/pokemon_range_8011F5FC.c"

s32 pokemonCheckSetMonohiroi(u8* pokemon)
{
    u32 table[10];
    u16 ability;
    u16 item;
    u16 i;
    u32 roll;

    table[0] = ((u32*)lbl_8027296C)[0];
    table[1] = ((u32*)lbl_8027296C)[1];
    table[2] = ((u32*)lbl_8027296C)[2];
    table[3] = ((u32*)lbl_8027296C)[3];
    table[4] = ((u32*)lbl_8027296C)[4];
    table[5] = ((u32*)lbl_8027296C)[5];
    table[6] = ((u32*)lbl_8027296C)[6];
    table[7] = ((u32*)lbl_8027296C)[7];
    table[8] = ((u32*)lbl_8027296C)[8];
    table[9] = ((u32*)lbl_8027296C)[9];

    if (pokemon == NULL) {
        ability = 0;
    } else {
        u16 species = (u16)pokemonGetStatus(NULL, 0x6E, 0, 0);
        if ((u16)pokemonGetStatus(NULL, species, 0x17, 1) == 0) {
            ability = (u16)pokemonGetStatus(NULL, species, 0x17, 0);
        } else {
            ability = (u16)pokemonGetStatus(
                NULL, species, 0x17,
                (u8)pokemonGetStatus(pokemon, 0, 0xB7, 0));
        }
    }

    if (ability != 0x35 ||
        (u16)pokemonGetStatus(pokemon, 0, 0x82, 0) != 0 ||
        fn_800E0C54() % 10 != 0) {
        return 0;
    }

    item = 0;
    roll = fn_800E0C54() % 100;
    for (i = 0; i < 20; i += 2) {
        if (((u16*)table)[i + 1] > roll) {
            item = ((u16*)table)[i];
            break;
        }
    }
    if (item == 0) {
        return 0;
    }
    if (pokemon != NULL) {
        pokemonSetStatus(pokemon, 0, 0x82, 0, item);
    }
    return 1;
}

void pokemonAllKaihuku(u8* pokemon)
{
    u8 slot;
    u8 valid;
    u8 pp;
    u16 move;
    u16 moveId;

    if (pokemon == NULL) {
        return;
    }

    pokemonSetStatus(pokemon, 0, 0x83, 0,
                     pokemonGetStatus(pokemon, 0, 0x87, 0));
    for (slot = 0; slot < 4; slot++) {
        move = (u16)pokemonGetStatus(pokemon, 0, 0x7F, slot);
        valid = move != 0 && move != 0x163;
        if (valid == 1) {
            moveId = (u16)pokemonGetStatus(pokemon, 0, 0x7F, slot + 4);
            pp = wazaGetMaxPP(
                moveId,
                (u8)pokemonGetStatus(pokemon, 0, 0x81, slot + 4));
            pokemonSetStatus(pokemon, 0, 0x80, slot, pp);
        }
    }

    if (fn_80119ED0(3) == 0x7C || fn_80119ED0(3) == 0xC8) {
        fn_8011B788(pokemon, 3);
    }
    if (fn_80119ED0(4) == 0x7C || fn_80119ED0(4) == 0xC8) {
        fn_8011B788(pokemon, 4);
    }
    if (fn_80119ED0(5) == 0x7C || fn_80119ED0(5) == 0xC8) {
        fn_8011B788(pokemon, 5);
    }
    if (fn_80119ED0(6) == 0x7C || fn_80119ED0(6) == 0xC8) {
        fn_8011B788(pokemon, 6);
    }
    if (fn_80119ED0(7) == 0x7C || fn_80119ED0(7) == 0xC8) {
        fn_8011B788(pokemon, 7);
    }
    if (fn_80119ED0(8) == 0x7C || fn_80119ED0(8) == 0xC8) {
        fn_8011B788(pokemon, 8);
    }
}
