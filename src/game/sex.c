/**
 * @file sex.c
 * @brief Decompiled functions.
 *
 * Address range: 0x80131574 - 0x80131588
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"


/* 0x80131574 | 20 bytes | indexed_lookup */
u8 sexGetPokemonSexRaitoKotei(u32 idx) {
    return lbl_803635D8[(u16)idx].value;
}
