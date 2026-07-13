/**
 * @file gs_range_801E09E0.c
 * @brief gs-engine, 0x801E09E0 - 0x801E1B54.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

void etctoolSetPokemonNakigoe(void)
{
    void *pokemonData;
    u16 voice;

    extern void *pokemonDataBiosGetPtr(void);
    extern u16 pokemonDataBiosGetVoice(void *);
    extern void fn_80166AB8(u32, u32, u32);

    pokemonData = pokemonDataBiosGetPtr();
    if (pokemonData != NULL) {
        voice = pokemonDataBiosGetVoice(pokemonData);
        fn_80166AB8((u32)voice, 0, 0);
    }
}

void fn_801E1170(void)
{
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;
    extern u32 lbl_8047B430;

    lbl_8047B424 = 4;
    lbl_8047B428 = 3;
    lbl_8047B430 = 0;
}

void fn_801E118C(void)
{
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;

    lbl_8047B424 = 3;
    lbl_8047B428 = 3;
}

void fn_801E119C(void)
{
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;

    lbl_8047B424 = 2;
    lbl_8047B428 = 3;
}

void fn_801E11B0(void)
{
    extern s32 lbl_8047B424;
    extern s32 lbl_8047B428;

    lbl_8047B424 = 1;
    if (lbl_8047B428 == 2) {
        return;
    }

    lbl_8047B428 = 1;
}

u32 fn_801E11CC(void)
{
    extern u8 lbl_8047B434;

    return lbl_8047B434;
}

void fn_801E11D4(u32 a, u8 b)
{
    extern u8 lbl_8047B434;
    extern u8 lbl_8047B435;

    lbl_8047B434 = a;
    lbl_8047B435 = b;
}

s32 fn_801E11E0(void)
{
    extern s32 lbl_8047B424;

    return lbl_8047B424;
}

u8 fn_801E11E8(void)
{
    extern u8 lbl_8047B420;

    return lbl_8047B420;
}

void fn_801E1258(void)
{
    extern u8 lbl_8047B420;
    extern u32 lbl_8047B424;
    extern u32 lbl_8047B428;

    lbl_8047B420 = 1;
    lbl_8047B424 = 2;
    lbl_8047B428 = 3;
}

void GSvtrLoadTexture(void)
{
    extern u32 lbl_8047B438;
    extern u32 fn_800F92D4(u32);

    lbl_8047B438 = fn_800F92D4(0x0b521200);
}

s32 fn_801E16D0(void)
{
    extern s32 fn_801E25C8(void);

    return fn_801E25C8();
}

s32 fn_801E1874(void)
{
    extern u8 lbl_8047B440;
    extern u8 lbl_8047B441;

    if (lbl_8047B440 == 0) {
        return 0;
    }
    if (lbl_8047B441 == 0) {
        return 0;
    }
    return 1;
}

void fn_801E1B2C(void)
{
    extern void fn_801E4A6C(void);
    extern u8 lbl_8047B440;

    fn_801E4A6C();
    lbl_8047B440 = 1;
}
