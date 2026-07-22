/**
 * @file fight_out_pokemon_exact_80203A6C.c
 * @brief Strict natural-C fightOutPokemon accessor island, address range
 *        0x80203A6C-0x80203EDC, 10 functions.
 *
 * OutPokemon/Pokemon field accessors, sequence/status writers, and
 * damage-calc support the seq/waza layers call into (statusGetStatus,
 * fadeEffectGetRandom callers, etc). Corresponds to XD's
 * fight.cpp fightOutPokemon+fightPokemon cluster (0x80200644-0x80208288).
 */

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

extern void* pokemonGetStatus();

/* 0x80203A6C | size: 0x70 */
u32 fightOutPokemonGetNowHpPercentage(void* ctx) {
    extern u32 pokemonGetNowHpPercentage();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        resolved = resolved == NULL ? NULL
                                    : pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonGetNowHpPercentage(resolved);
}

/* 0x80203ADC | size: 0x80 */
u32 fightOutPokemonNowHpWaruValue(void* ctx, u32 param) {
    extern u32 pokemonGetNowHpWaruValue();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        resolved = resolved == NULL ? NULL
                                    : pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonGetNowHpWaruValue(resolved, param);
}

/* 0x80203B5C | size: 0x80 */
u32 fightOutPokemonMaxHpWaruValue(void* ctx, u32 param) {
    extern u32 pokemonGetMaxHpWaruValue();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        resolved = resolved == NULL ? NULL
                                    : pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonGetMaxHpWaruValue(resolved, param);
}

/* 0x80203BDC | size: 0x80 */
u32 fightOutPokemonIsNokoriHpFollowing(void* ctx, u32 param) {
    extern u32 pokemonIsNokoriHpFollowing();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        resolved = resolved == NULL ? NULL
                                    : pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonIsNokoriHpFollowing(resolved, param);
}

/* 0x80203C5C | size: 0x70 */
u32 fightOutPokemonIsJoutaiKaragenki(void* ctx) {
    extern u32 pokemonIsJoutaiKaragenki();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        resolved = resolved == NULL ? NULL
                                    : pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonIsJoutaiKaragenki(resolved);
}

/* 0x80203CCC | size: 0x70 */
u32 fightOutPokemonIsJoutaiNormal(void* ctx) {
    extern u32 pokemonIsJoutaiNormal();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        resolved = resolved == NULL ? NULL
                                    : pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonIsJoutaiNormal(resolved);
}

/* 0x80203D3C | size: 0x70 */
u16 figthOutPokemonGetPokemonDataId(void* ctx) {
    void* resolved;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    resolved = resolved == NULL ? NULL
                                : pokemonGetStatus(resolved, 0, 0xCC, 0);
    if (resolved == NULL) {
        return 0;
    }
    return (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
}

/* 0x80203DAC | size: 0x60 */
u16 figthPokemonGetPokemonDataId(void* ctx) {
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
}

/* 0x80203E0C | size: 0x70 */
    u8 figthOutPokemonGetLevel(void* ctx) {
    void* resolved;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    resolved = resolved == NULL ? NULL
                                : pokemonGetStatus(resolved, 0, 0xCC, 0);
    if (resolved == NULL) {
        return 0;
    }
    return (u8)(u32)pokemonGetStatus(resolved, 0, 0x7A, 0);
}

/* 0x80203E7C | size: 0x60 */
u32 figthPokemonGetLevel(u32 obj) {
    extern u32 pokemonGetStatus();
    u32 result;
    if (obj == 0) {
        result = 0;
    } else {
        result = pokemonGetStatus(obj, 0, 0xCC, 0);
    }
    if (result == 0) {
        return 0;
    }
    return pokemonGetStatus(result, 0, 0x7A, 0) & 0xFF;
}
