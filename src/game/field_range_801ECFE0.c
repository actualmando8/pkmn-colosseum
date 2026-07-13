/**
 * @file field_range_801ECFE0.c
 * @brief field/hero, 0x801ECFE0 - 0x801ED640.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

u8 fn_801ED218(void* arg0) {
    u8* status;
    u8 result;

    extern u8* savedataGetStatus(u8* arg0, u16 arg1);

    if (arg0 == NULL) {
        status = savedataGetStatus(NULL, 0xb);
    } else {
        status = (u8*)arg0;
    }

    result = status[0];
    return result;
}

u8* sodateyaGetPokemonPtr(void* arg0) {
    extern u8* savedataGetStatus(u8* arg0, u16 arg1);

    if (arg0 == NULL) {
        arg0 = (u8*)savedataGetStatus(NULL, 0xb);
    }

    return (u8*)arg0 + 8;
}

void fn_801ED388(void) {
    extern void* fn_801ED3B8(void);
    extern u32 heroMoveAddStepCallback(void* callback, s32 arg);
    extern u32 lbl_8047B5B8;

    lbl_8047B5B8 = heroMoveAddStepCallback(fn_801ED3B8, 0);
}
