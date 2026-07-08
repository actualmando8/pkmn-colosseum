/**
 * @file tikeiBios.c
 * @brief Decompiled functions.
 *
 * Address range: 0x801363E8 - 0x801364A8
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"

#if 0
asm void tikeiDataBiosGetWazaId(void) {
#include "src/game/effect/effect_util_fn_801363E8.inc"
}
#else
#pragma scheduling on
u32 tikeiDataBiosGetWazaId(u16 index) {
    EffectTraceEntry* entry;
    if ((u32)index > lbl_80478BA0) {
        entry = NULL;
    } else {
        entry = &lbl_80363C00[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->value1;
}
#pragma scheduling off
#endif

#if 0
asm void tikeiDataBiosGetZokuseiDataId(void) {
#include "src/game/effect/effect_util_fn_80136428.inc"
}
#else
#pragma scheduling on
u32 tikeiDataBiosGetZokuseiDataId(u16 index) {
    EffectTraceEntry* entry;
    if ((u32)index > lbl_80478BA0) {
        entry = NULL;
    } else {
        entry = &lbl_80363C00[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->kind;
}
#pragma scheduling off
#endif

#if 0
asm void tikeiDataBiosGetFightKoukaId(void) {
#include "src/game/effect/effect_util_fn_80136468.inc"
}
#else
#pragma scheduling on
u32 tikeiDataBiosGetFightKoukaId(u16 index) {
    EffectTraceEntry* entry;
    if ((u32)index > lbl_80478BA0) {
        entry = NULL;
    } else {
        entry = &lbl_80363C00[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->value0;
}
#pragma scheduling off
#endif
