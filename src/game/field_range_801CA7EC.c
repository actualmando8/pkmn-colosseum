/**
 * @file field_range_801CA7EC.c
 * @brief field/hero, 0x801CA7EC - 0x801CB180.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) -- mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

#pragma push
#pragma peephole off
u32 scriptAddPokecoupon(s32 offset) {
    extern u32 heroGetStatus(u8* ptr, u32 selector, u32 index);
    extern void heroAddPokecoupon(u8* ptr, s32 offset);
    extern void heroDecPokecoupon(u8* ptr, s32 offset);

    heroGetStatus(0, 0xd, 0);
    if (offset >= 0) {
        heroAddPokecoupon(0, offset);
    } else {
        heroDecPokecoupon(0, -offset);
    }
    return heroGetStatus(0, 0xd, 0);
}
#pragma pop

#pragma push
#pragma peephole off
u32 scriptAddPremium(s32 offset) {
    extern u32 fn_801906A0(u32 flag);
    extern void _flagSet(u32 flag, u32 value);
    u32 current;
    u32 premium;

    current = fn_801906A0(0xa9e);
    premium = current + offset;
    _flagSet(0xa9e, premium);
    return premium;
}
#pragma pop

#pragma push
#pragma peephole off
u32 scriptAddPokedoru(s32 offset) {
    extern void heroAddPokedoru(u8* ptr, s32 offset);
    extern void heroDecPokedoru(u8* ptr, s32 offset);
    extern u32 heroGetStatus(u8* ptr, u32 selector, u32 index);

    if (offset >= 0) {
        heroAddPokedoru(0, offset);
    } else {
        heroDecPokedoru(0, -offset);
    }
    return heroGetStatus(0, 0xc, 0);
}
#pragma pop

#pragma push
#pragma peephole off
s32 scriptAddItem(u32 item, s32 count) {
    extern s32 heroItemAddItemDataId(u8* ptr, u16 item, u16 count, s32 slot);
    extern s32 heroItemDecItemDataId(u8* ptr, u16 item, u16 count, s32 slot);
    s32 result;

    if (count > 0) {
        result = heroItemAddItemDataId(0, item, count, -1);
    } else if (count < 0) {
        result = heroItemDecItemDataId(0, item, -count, -1);
    }
    return result;
}
#pragma pop

#pragma push
#pragma peephole off
void scriptStoreTemochiPokemon(u8* hero) {
    extern u32 heroGetStatus(u8* hero, u32 selector, u32 index);
    extern u8 pokemonCheckValid(u32 pokemon);
    extern u32 pokemonGetStatus(u32 pokemon, u32 index, u32 selector, u32 subindex);
    extern u32 lbl_804670B4[][2];
    u32* entry = lbl_804670B4[0];
    s32 i = 0;
    u16 species;
    u32 pokemon;
    u32 status;

    for (; i < 6; entry += 2, i++) {
        entry[1] = 0;
        pokemon = heroGetStatus(hero, 3, (u16)i);
        if (pokemon != 0 && pokemonCheckValid(pokemon)) {
            species = pokemonGetStatus(pokemon, 0, 0x6e, 0);
            status = pokemonGetStatus(pokemon, 0, 0x6f, 0);
            entry[1] = species;
            entry[0] = status;
        }
    }
}
#pragma pop

#pragma push
#pragma peephole off
s32 scriptGetEarthRibbon(void) {
    extern void* savedataGetStatus(u32 side, u32 slotType);
    extern void* heroBiosGetPokemonPtr(void* party, u16 index);
    extern u8 pokemonCheckValid(void* pokemon);
    extern void exribbonSetEarthRibbon(void* pokemon);
    void* party;
    void* pokemon;
    u16 i;

    party = savedataGetStatus(0, 2);
    i = 0;
    while (i < 6) {
        pokemon = heroBiosGetPokemonPtr(party, i);
        if (pokemonCheckValid(pokemon)) {
            exribbonSetEarthRibbon(pokemon);
        }
        i++;
    }
    return 0;
}
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_801CADA8(u8 selector) {
    extern u32 savedataGetStatus(u32 side, u32 slotType);
    extern s32 heroPokemonGetBlacky(u32 hero, u32 ball);
    extern s32 heroPokemonGetEifie(u32 hero, u32 ball);
    extern s32 heroPokemonGetPrasle(u32 hero, u32 ball);
    extern s32 heroPokemonGetHouou(u32 hero, u32 ball);
    extern s32 heroPokemonGetPikachu(u32 hero, u32 ball);
    extern s32 heroPokemonGetCelebi(u32 hero, u32 ball);
    u32 hero;
    s32 result = 0;

    hero = savedataGetStatus(0, 2);
    switch (selector) {
    case 1:
        result = heroPokemonGetBlacky(hero, 0xfe);
        break;
    case 2:
        result = heroPokemonGetEifie(hero, 0xfe);
        break;
    case 3:
        result = heroPokemonGetPrasle(hero, 0xfe);
        break;
    case 4:
        result = heroPokemonGetHouou(hero, 0xff);
        break;
    case 5:
        result = heroPokemonGetPikachu(hero, 0xff);
        break;
    case 6:
        result = heroPokemonGetCelebi(hero, 0xff);
        break;
    }
    return result;
}
#pragma pop

void scriptSetPokemonNakigoe(void) {
    extern void etctoolSetPokemonNakigoe(void);
    etctoolSetPokemonNakigoe();
}

u32 scriptGetPokemonNickName(s32 index) {
    extern u32 savedataGetStatus(u32 side, u32 slotType);
    extern u32 heroBiosGetPokemonPtr(u32 party, u16 index);
    extern u32 pokemonGetStatus(u32 pokemon, u32 index, u32 selector, u32 subindex);
    u32 pokemon;

    if (index >= 6) {
        return 0;
    }
    pokemon = heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)index);
    if (pokemon == 0) {
        return 0;
    }
    return pokemonGetStatus(pokemon, 0, 0x77, 0);
}

#pragma push
#pragma peephole off
s32 scriptGetDarkPointZeroPokemonNum(void) {
    extern void* savedataGetStatus(u32 side, u32 slotType);
    extern void* heroBiosGetPokemonPtr(void* party, u16 index);
    extern u8 pokemonCheckValid(void* pokemon);
    extern u8 pokemonBiosGetDarkFlag(void* pokemon);
    extern f32 pokemonGetDp(void* pokemon);
    void* party;
    void* pokemon;
    u16 i;
    u16 count;

    count = 0;
    party = savedataGetStatus(0, 2);
    i = 0;
    while (i < 6) {
        pokemon = heroBiosGetPokemonPtr(party, i);
        if (pokemonCheckValid(pokemon)) {
            if (pokemonBiosGetDarkFlag(pokemon)) {
                if (pokemonGetDp(pokemon) == 0.0f) {
                    count++;
                }
            }
        }
        i++;
    }
    return count;
}
#pragma pop

#pragma push
#pragma peephole off
s32 scriptGetDarkPokemonNum(void) {
    extern void* savedataGetStatus(u32 side, u32 slotType);
    extern void* heroBiosGetPokemonPtr(void* party, u16 index);
    extern u8 pokemonCheckValid(void* pokemon);
    extern u8 pokemonBiosGetDarkFlag(void* pokemon);
    void* party;
    void* pokemon;
    u16 i;
    u16 count;

    count = 0;
    party = savedataGetStatus(0, 2);
    i = 0;
    while (i < 6) {
        pokemon = heroBiosGetPokemonPtr(party, i);
        if (pokemonCheckValid(pokemon)) {
            if (pokemonBiosGetDarkFlag(pokemon)) {
                count++;
            }
        }
        i++;
    }
    return count;
}
#pragma pop

#pragma push
#pragma peephole off
s32 scriptGetPokemonNum(void) {
    extern void* savedataGetStatus(u32 side, u32 slotType);
    extern void* heroBiosGetPokemonPtr(void* party, u16 index);
    extern u8 pokemonCheckValid(void* pokemon);
    void* party;
    u16 i;
    u16 count;

    count = 0;
    party = savedataGetStatus(0, 2);
    i = 0;
    while (i < 6) {
        if (pokemonCheckValid(heroBiosGetPokemonPtr(party, i))) {
            count++;
        }
        i++;
    }
    return count;
}
#pragma pop

#pragma push
#pragma peephole off
s32 floorCharacterSetPos(u32 character, f32 x, f32 y, f32 z) {
    extern void set__5GSvecFfff(f32* position, f32 x, f32 y, f32 z);
    extern void* fn_8011711C(u32 character);
    extern void floorCharacterBiosSetPos(void* character, f32* position);
    f32 position[3];

    set__5GSvecFfff(position, x, y, z);
    floorCharacterBiosSetPos(fn_8011711C(character), position);
    return 1;
}
#pragma pop

#pragma push
#pragma scheduling off
s32 scriptFloorCharSetDisp(u32 a0, u32 a1) {
    extern u8* fn_8011711C();
    extern void floorCharacterBiosSetVisibility();

    floorCharacterBiosSetVisibility((u8*)fn_8011711C(a0), (u8)a1);
    return 1;
}
#pragma pop
