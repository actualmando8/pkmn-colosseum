/**
 * @file fight_out_pokemon_exact_8020355C.c
 * @brief Strict natural-C fightOutPokemon accessor island, address range
 *        0x8020355C-0x802038A4, 8 functions.
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
extern u32 pokemonSetStatus();
extern void pokemonGrowBasisStatus();

/* 0x8020355C | size: 0x60 */
u32 fightPokemonGetLevelToExp(u32 obj, u32 param) {
    extern u32 pokemonGetLevelToExp();
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
    return pokemonGetLevelToExp(result, param);
}

/* 0x802035BC | size: 0x64 */
void figthPokemonSetExp(void* obj, u32 value) {
    void* intermediate;
    if (obj == NULL) {
        intermediate = NULL;
    } else {
        intermediate = pokemonGetStatus(obj, 0, 0xCC, 0);
    }
    if (intermediate != NULL) {
        pokemonSetStatus(intermediate, 0, 0x79, 0, value);
    }
}

/* =========================================================================
 * figthPokemonGetExp
 *
 * Navigate from a trainer context through two data table hops to reach
 * extended Pokemon/trainer data. Same pokemonGetStatus(..., 0xCC/0x79, ...)
 * hop pattern as fightPokemonGetLevelToExp/figthPokemonSetExp above.
 *
 * Hop 1: pokemonGetStatus(ctx, 0, 0xCC, 0) -> intermediate pointer
 * Hop 2: pokemonGetStatus(intermediate, 0, 0x79, 0) -> extended data
 *
 * If either hop returns NULL, the function returns NULL.
 *
 * @param context  Trainer/party context
 * @return         Extended data pointer, or NULL
 * ========================================================================= */
void* figthPokemonGetExp(void* context) {
    void* intermediate;
    if (context == NULL) {
        intermediate = NULL;
    } else {
        intermediate = pokemonGetStatus(context, 0, 0xCC, 0);
    }
    if (intermediate == NULL) {
        return NULL;
    }

    return pokemonGetStatus(intermediate, 0, 0x79, 0);
}

/* =========================================================================
 * fightPokemonGrowBasisStatus
 *
 * Similar two-hop navigation, but the second call writes data via
 * pokemonGrowBasisStatus instead of reading it.
 *
 * @param context  Trainer/party context
 * @param value    Value to write
 * ========================================================================= */
void fightPokemonGrowBasisStatus(void* context, u32 value) {
    void* intermediate;
    if (context == NULL) {
        intermediate = NULL;
    } else {
        intermediate = pokemonGetStatus(context, 0, 0xCC, 0);
    }
    if (intermediate != NULL) {
        pokemonGrowBasisStatus(intermediate, value);
    }
}

/* 0x802036D4 | size: 0x84 */
u32 fightOutPokemonGetVoiceSndId(void* ctx) {
    void* resolved;
    u16 species;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        resolved = resolved == NULL ? NULL
                                    : pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    species = (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
    return (u16)(u32)pokemonGetStatus(NULL, species, 0x61, 0);
}

/* 0x80203758 | size: 0x84 */
u32 fightOutPokemonGetNamePtr(void* ctx) {
    extern u32 GSmsgGetGSchar();
    void* resolved;
    u16 species;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    resolved = resolved == NULL ? NULL
                                : pokemonGetStatus(resolved, 0, 0xCC, 0);
    if (resolved == NULL) {
        return 0;
    }
    species = (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
    resolved = pokemonGetStatus(NULL, species, 0x01, 0);
    return GSmsgGetGSchar(resolved);
}

/* 0x802037DC | size: 0x6C */
void* fightOutPokemonGetNicknamePtr(void* ctx) {
    void* resolved;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    resolved = resolved == NULL ? NULL
                                : pokemonGetStatus(resolved, 0, 0xCC, 0);
    if (resolved == NULL) {
        return NULL;
    }
    return pokemonGetStatus(resolved, 0, 0x77, 0);
}

/* 0x80203848 | size: 0x5C | small */
u32 fightPokemonGetNicknamePtr(void* ctx) {
    void* resolved;
    u32 result;

    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        result = 0;
    } else {
        result = (u32)pokemonGetStatus(resolved, 0, 0x77, 0);
    }
    return result;
}
