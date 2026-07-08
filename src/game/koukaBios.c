/**
 * @file koukaBios.c
 * @brief Decompiled functions.
 *
 * Address range: 0x80135F58 - 0x80136078
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"


/* 0x80135F58 | 0x38 */
#if 0
asm void koukaLinkDataBiosGetKouka(void) {
#include "src/game/effect/effect_util_fn_80135F58.inc"
}
#else
#pragma scheduling on
u32 koukaLinkDataBiosGetKouka(u32 index, u32 sub) {
    if (index > lbl_80478B90 || sub > 8) {
        return 0;
    }
    return lbl_80363B78[index].links[sub];
}
#pragma scheduling off
#endif


/* 0x80135F90 | 0x2C */
/* Get the linked effect index from the effect status table. */
#pragma scheduling on
u32 koukaDataBiosGetLink(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].linkedIndex;
}
#pragma scheduling off


/* 0x80135FBC | 0x3C
 * Get a signed status parameter from the effect status table.
 */
#pragma scheduling on
#pragma push
#pragma scheduling on
#pragma fp_contract on
s32 koukaDataBiosGetValue(u32 index, u32 subIndex) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;
    if (index > lbl_80478B88 || subIndex > 2) {
        return 0;
    }
    /* subIndex 2 is valid in the original table and reads the next signed halfword. */
    return (&lbl_80363B18[index].amount)[subIndex];
}
#pragma pop
#pragma scheduling off


/* 0x80135FF8 | 0x2C */
/* Get the status update mode from the effect status table. */
#pragma scheduling on
u32 koukaDataBiosGetVar(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].mode;
}
#pragma scheduling off


/* 0x80136024 | 0x2C */
/* Get the status sub-type from the effect status table. */
#pragma scheduling on
u32 koukaDataBiosGetStatus(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].statusSub;
}
#pragma scheduling off


/* 0x80136050 | 0x28 */
/* Get the status kind from the effect status table. */
#pragma scheduling on
u32 koukaDataBiosGetStatusKind(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].statusKind;
}
#pragma scheduling off
