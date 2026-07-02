/**
 * @file fight_range_80211A00.c
 * @brief Fight/battle-AI support layer, 0x80211A00 - 0x802405C0 (470 fns).
 *
 * Split out of the previously-unassigned auto block (audit 2026-07-01,
 * docs/fable5_audit_pass3_fight_engine.md). Contains fightSeqGetItemType,
 * fightTrainerAiGetValueAryMaxBanme, and the callee layer used by
 * colosseum_battle.c's WazaHit family. Range name kept honest until the
 * module's internal structure is proven.
 */
#include "dolphin/types.h"

/* fn_800E0C54: RNG (see src/game/gs_render.c, gs_model.c) */
extern u16 fn_800E0C54(void);

/*
 * fightTrainerAiGetValueAryMaxBanme (0x802397B8)
 *
 * Argmax over a 16-slot AI value array: scans valueAry[0..count) for the
 * maximum value (starting from the AI's "unset" sentinel -200), collects
 * every index that ties for that maximum into a 16-slot local array, then
 * returns one of the tied indices -- the first if there's no tie or
 * useRandom is not set, otherwise an RNG-selected tie via fn_800E0C54().
 */
#pragma optimize_for_size on
s32 fightTrainerAiGetValueAryMaxBanme(s32* valueAry, u16 count, u8 useRandom) {
    s32 banmeAry[16];
    s32 max = -0xC8;
    u16 tieCount = 0;
    u16 i;
    u16 idx;

    for (i = 0; i < 16; i++) {
        banmeAry[i] = -1;
    }

    for (i = 0; i < count; i++) {
        if (max < valueAry[i]) {
            max = valueAry[i];
        }
    }

    for (i = 0; i < count; i++) {
        if (max <= valueAry[i]) {
            banmeAry[tieCount] = i;
            tieCount++;
        }
        if (tieCount >= 16) {
            break;
        }
    }

    if (tieCount == 0) {
        return -1;
    }

    if (tieCount == 1) {
        idx = 0;
    } else if (useRandom == 1) {
        idx = fn_800E0C54() % tieCount;
    } else {
        idx = 0;
    }

    return banmeAry[idx];
}
#pragma optimize_for_size reset

/* Item-data accessor chain (src/game/people/people_data.c hosts fn_80143A94;
 * fn_801440A0/fn_80143DFC are still asm-only there). */
extern u8* fn_801440A0(u16 idx);
extern u8   fn_80143DFC(u8* p);
extern u8*  fn_80143A94(u8 idx);
extern u8   fn_801437E0(u8* p);
extern u8   fn_80143940(u8* p);
extern u8   fn_80143918(u8* p);
extern u8   fn_801438F0(u8* p);
extern u8   fn_801438C8(u8* p);
extern u8   fn_801438A0(u8* p);
extern u8   fn_80143878(u8* p);
extern u8   fn_80143A44(u8* p);
extern u8   fn_80143A28(u8* p);
extern u8   fn_80143A0C(u8* p);
extern u8   fn_801439F0(u8* p);
extern u8   fn_801439D4(u8* p);
extern u8   fn_801439B8(u8* p);
extern u8   fn_80143990(u8* p);

/*
 * fightSeqGetItemType (0x802126C4)
 *
 * Item-type classifier: resolves the item record for itemId through the
 * item-data accessor chain (fn_801440A0 -> fn_80143DFC -> fn_80143A94),
 * then runs it through 7 chained predicate/flag checks, returning a
 * type code in {1..7}.
 */
#pragma optimize_for_size on
s32 fightSeqGetItemType(u16 itemId) {
    u8* p;

    p = fn_80143A94(fn_80143DFC(fn_801440A0(itemId)));
    if (p == NULL) {
        return 7;
    }

    if (itemId == 0x13) {
        return 1;
    }

    if (fn_801437E0(p)) {
        return 2;
    }

    if (fn_80143940(p) == 1 || fn_80143918(p) == 1 || fn_801438F0(p) == 1 ||
        fn_801438C8(p) == 1 || fn_801438A0(p) == 1 || fn_80143878(p) == 1) {
        return 3;
    }

    if (fn_80143A44(p) == 1) {
        return 4;
    }

    if (fn_80143A28(p) || fn_80143A0C(p) || fn_801439F0(p) || fn_801439D4(p) ||
        fn_801439B8(p)) {
        return 5;
    }

    return (fn_80143990(p) == 1) ? 6 : 7;
}
#pragma optimize_for_size reset
