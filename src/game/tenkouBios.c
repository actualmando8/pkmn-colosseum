/**
 * @file tenkouBios.c
 * @brief Decompiled functions.
 *
 * Address range: 0x80136368 - 0x801363E8
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"

#if 0
asm void tenkouDataBiosGetFightInitMsgId(void) {
#include "src/game/effect/effect_util_fn_80136368.inc"
}
#else
#pragma scheduling on
u32 tenkouDataBiosGetFightInitMsgId(u16 index) {
    EffectTraceFxEntry* entry;
    if ((u32)index > lbl_80478B98) {
        entry = NULL;
    } else {
        entry = &lbl_80363B88[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->effectId;
}
#pragma scheduling off
#endif

#if 0
asm void tenkouDataBiosGetSolarFlag(void) {
#include "src/game/effect/effect_util_fn_801363A8.inc"
}
#else
#pragma scheduling on
u32 tenkouDataBiosGetSolarFlag(u16 index) {
    EffectTraceFxEntry* entry;
    if ((u32)index > lbl_80478B98) {
        entry = NULL;
    } else {
        entry = &lbl_80363B88[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->kind;
}
#pragma scheduling off
#endif
