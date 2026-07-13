/**
 * @file field_range_801CA7EC.c
 * @brief field/hero, 0x801CA7EC - 0x801CB180.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) -- mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

void scriptSetPokemonNakigoe(void) {
    extern void etctoolSetPokemonNakigoe(void);
    etctoolSetPokemonNakigoe();
}

#pragma push
#pragma scheduling off
s32 scriptFloorCharSetDisp(u32 a0, u32 a1) {
    extern u8* fn_8011711C();
    extern void floorCharacterBiosSetVisibility();

    floorCharacterBiosSetVisibility((u8*)fn_8011711C(a0), (u8)a1);
    return 1;
}
#pragma pop
