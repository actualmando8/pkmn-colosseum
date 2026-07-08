/**
 * @file kouka.c
 * @brief Decompiled functions.
 *
 * Address range: 0x80136078 - 0x80136368
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"


/* 0x80136078 | 0xC4 */
#if 0
asm void fn_80136078(void) {
#include "src/game/effect/effect_util_fn_80136078.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80136078(u32 index, void* arg1, void* arg2, s32* out) {
    u32 linkedIndex;
    u32 sub;

    if (out != NULL) {
        _koukaOneExec__FUlPvPvPl(index, arg1, arg2, out);
    } else {
        _koukaOneExec__FUlPvPvPl(index, arg1, arg2, NULL);
    }

    linkedIndex = koukaDataBiosGetLink(index) & 0xFFFF;
    if (linkedIndex == 0) {
        return;
    }

    for (sub = 0; sub < 8; sub++) {
        if ((koukaLinkDataBiosGetKouka(linkedIndex, sub & 0xFFFF) & 0xFFFF) != 0) {
            if (out != NULL) {
                _koukaOneExec__FUlPvPvPl(index, arg1, arg2, out + ((sub & 0xFFFF) + 1));
            } else {
                _koukaOneExec__FUlPvPvPl(index, arg1, arg2, NULL);
            }
        }
    }
}
#pragma pop
#endif


/* 0x8013613C | 0x22C */
#if 0
asm void _koukaOneExec__FUlPvPvPl(void) {
#include "src/game/effect/effect_util_fn_8013613C.inc"
}
#else
void _koukaOneExec__FUlPvPvPl(u32 index, void* arg1, void* arg2, s32* out) {
    u32 statusKind;
    u32 statusSub;
    s32 amount;
    s32 divisor;
    u32 mode;
    s32 current;
    u32 extra;
    u16 handle;

    if (index == 0) {
        return;
    }

    statusKind = koukaDataBiosGetStatusKind(index);
    statusSub = koukaDataBiosGetStatus(index);
    amount = (s16)koukaDataBiosGetValue(index, 0);
    divisor = (s16)koukaDataBiosGetValue(index, 1);
    mode = koukaDataBiosGetVar(index) & 0xFF;

    current = fn_80135E44(statusKind, (u32)arg1, 0, statusSub, amount & 0xFFFF);
    if (mode == 0 || mode == 2 || mode == 3) {
        if (amount == -1) {
            amount = fn_80135E44(statusKind, (u32)arg1, 0, divisor & 0xFFFF, 0) / 2;
        } else if (amount == -2) {
            amount = (s16)fn_80135E44(statusKind, (u32)arg1, 0, divisor & 0xFFFF, 0);
        } else if (amount < -2 || divisor < -2) {
            return;
        }
    }

    switch (mode) {
    case 0:
        break;
    case 1:
        amount = (current * amount) / divisor;
        break;
    case 2:
        amount = current + amount;
        break;
    case 3:
        amount = current - amount;
        break;
    case 4:
        amount = current + ((current * amount) / divisor);
        break;
    case 5:
        amount = current - ((current * amount) / divisor);
        break;
    default:
        return;
    }

    if (arg2 != NULL) {
        handle = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
        extra = fightTargetGetTragetPtrToRelativeHostSideFightTargetId(arg2, handle);
    } else {
        extra = 0;
    }

    fn_80135D10(statusKind, (u32)arg1, 0, statusSub, extra, amount);
    if (out != NULL) {
        *out = amount;
    }
}
#endif
