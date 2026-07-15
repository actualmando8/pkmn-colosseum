/**
 * @file field_range_801ECFE0.c
 * @brief field/hero, 0x801ECFE0 - 0x801ED640.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

typedef struct PokemonCopy {
    u32 words[0x4e];
} PokemonCopy;

#pragma push
#pragma optimize_for_size on
s32 fn_801ECFE0(void* arg0, void* arg1) {
    extern u8* savedataGetStatus(u8* arg0, u16 arg1);
    extern u8 pokemonBiosGetDarkFlag(u8* pokemon);
    extern void pokemonBiosGetDp(u8* pokemon);
    extern void pokemonInit(u8* pokemon);
    u8* source;
    void* status;
    void* resolved;

    status = arg0;
    if (arg0 == NULL) {
        status = savedataGetStatus(NULL, 0xb);
    }
    if (arg1 == NULL) {
        return 0;
    }

    resolved = status;
    if (status == NULL) {
        resolved = savedataGetStatus(NULL, 0xb);
    }
    source = (u8*)resolved + 8;
    if (source == NULL) {
        return 0;
    }

    *(PokemonCopy*)arg1 = *(PokemonCopy*)source;
    if (pokemonBiosGetDarkFlag(source)) {
        pokemonBiosGetDp(source);
    }

    if (status == NULL) {
        status = savedataGetStatus(NULL, 0xb);
    }
    if (status != NULL) {
        *(u8*)status = 0;
    }

    pokemonInit(source);
    return 1;
}
#pragma pop

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

s32 fn_801ED24C(void* arg0) {
    extern u8* savedataGetStatus(u8* arg0, u16 arg1);
    u8* status;

    if (arg0 == NULL) {
        status = savedataGetStatus(NULL, 0xb);
    } else {
        status = (u8*)arg0;
    }

    if (status[0] != 0) {
        return *(s32*)(status + 4);
    }
    return -1;
}

u8 fn_801ED294(void* arg0) {
    extern u8* savedataGetStatus(u8* arg0, u16 arg1);
    u8* status;

    if (arg0 == NULL) {
        status = savedataGetStatus(NULL, 0xb);
    } else {
        status = (u8*)arg0;
    }

    if (status[0] != 0) {
        return status[1];
    }
    return 0xff;
}

u8* sodateyaGetPokemonPtr(void* arg0) {
    extern u8* savedataGetStatus(u8* arg0, u16 arg1);

    if (arg0 == NULL) {
        arg0 = (u8*)savedataGetStatus(NULL, 0xb);
    }

    return (u8*)arg0 + 8;
}

void sodateyaInit(u8* data) {
    extern u8* savedataGetStatus(u8* arg0, u16 arg1);
    u8* status;

    status = data;
    if (status == NULL) {
        status = savedataGetStatus(NULL, 0xb);
    }
    if (status != NULL) {
        data = status;
        if (status == NULL) {
            data = savedataGetStatus(NULL, 0xb);
        }
        if (data != NULL) {
            *(u16*)(data + 2) = 0;
        }
        status[1] = 0;
        status[0] = 0;
    }
}

void fn_801ED388(void) {
    extern void* fn_801ED3B8(void);
    extern u32 heroMoveAddStepCallback(void* callback, s32 arg);
    extern u32 lbl_8047B5B8;

    lbl_8047B5B8 = heroMoveAddStepCallback(fn_801ED3B8, 0);
}
