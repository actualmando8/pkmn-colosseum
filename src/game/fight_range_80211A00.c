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

/* Cross-TU accessors (bodies live in other units). */
extern u32 fightPokemonGetPokemonPtr(u32 arg);
extern u32 pokemonGetTokuseiDataId(u32 poke);

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

/*
 * FightSeq bytecode opcode handlers (0x80215300 block and siblings).
 *
 * lbl_8047B610 is the FightSeq script program counter (see
 * src/game/trainer.c comment at top of file: "trainer system program
 * counter / script position"). Its value is treated as a byte address
 * into the script stream: on a "skip" outcome the PC is advanced past
 * a fixed-size instruction (opcode byte + operand bytes); on a "jump"
 * outcome the PC is replaced by the 4-byte operand embedded right
 * after the opcode byte (*(u32*)(pc + 1)).
 *
 * fightTargetGetPtrAsNowFightType(slot, base) resolves a FightSeq context/ctx pointer.
 * fn_802025B8/fn_8020248C/fn_802026E4 are the GetEventState/
 * SetEventState/CheckEventFlag dispatchers already matched 100% in
 * colosseum_event.c; despite being declared void there, real callers
 * (including that file's own fightSeqGetNromalWazaDamage) consume their r3 return
 * value, so they are re-declared here with the return type each call
 * site actually uses -- same K&R untyped-extern idiom already used
 * throughout colosseum_event.c for cross-TU calls.
 */
extern u32 fightTargetGetPtrAsNowFightType();
extern u8  fn_802025B8();
extern void fn_8020248C();
extern u8  fn_802026E4();
extern u32 lbl_8047B610;
extern u32 lbl_8047B618;
extern u16 lbl_8047B60C;

/*
 * fn_80214F10 (0x80214F10)
 *
 * FightSeq opcode handler: picks a random target index from the ally
 * pokemon-ptr list (fn_80215008 fills a u16[12] buffer via the ctx's
 * fightFloorGetFightOutPokemonPtrToFightTrainerPtr trainer object)
 * and forwards it through fn_8022B2CC to compute a value stashed via
 * fightFloorSetStatus(...,0x43,...). The picked u16 is also recorded
 * in lbl_8047B60C and flag bit 0x400 of lbl_8047B618 is cleared. If
 * the buffer came back empty the script-embedded jump is taken.
 */
extern u32 fightFloorGetFightOutPokemonPtrToFightTrainerPtr();
extern s32 fn_80215008();
extern u32 fn_8022B2CC();
extern u32 fightFloorGetStatus();
extern void fightFloorSetStatus();
#pragma optimize_for_size on
void fn_80214F10(void) {
    u16 buf[22];
    u32 ctx;
    u16 floorVal;
    u32 trainer;
    s32 count;
    u32 idx;
    u16 pick;
    u32 tmp;

    floorVal = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
    ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    trainer = fightFloorGetFightOutPokemonPtrToFightTrainerPtr(0, ctx);
    count = fn_80215008(trainer, buf, 0x18, ctx);

    if (count != 0) {
        idx = (u16)fn_800E0C54() % count;
        lbl_8047B618 &= ~0x400u;
        pick = buf[idx];
        lbl_8047B60C = pick;
        tmp = fn_8022B2CC(ctx, pick, floorVal, 0, 1, 1, -1);
        fightFloorSetStatus(0, 0, 0x43, 0, tmp);
        lbl_8047B610 = lbl_8047B610 + 5;
    } else {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    }
}
#pragma optimize_for_size reset

/*
 * WS_ONNEN (0x80215300)
 *
 * FightSeq opcode handler: if event-state 0x28 on the ctx from slot
 * 0x11 reads 2, clears it back to 0 and advances the PC past this
 * instruction (+5); otherwise takes the script-embedded jump.
 */
void WS_ONNEN(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);

    if (fn_802025B8(ctx, 0x28) == 2) {
        fn_8020248C(ctx, 0x28, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    } else {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    }
}

/* WS_CHOUHATSU (0x802161F0): same FightSeq opcode-handler shape as
 * WS_ONNEN, slot 0x12 / event-state id 0x30. */
void WS_CHOUHATSU(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x12, 0);

    if (fn_802025B8(ctx, 0x30) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx, 0x30, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}

/* WS_ICHAMON (0x802162F0): same shape, slot 0x12 / event-state id 0x1b. */
void WS_ICHAMON(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x12, 0);

    if (fn_802025B8(ctx, 0x1b) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx, 0x1b, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}

/* WS_NEWOHARU (0x80215A78): same shape, slot 0x11 / event-state id 0x25. */
void WS_NEWOHARU(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);

    if (fn_802025B8(ctx, 0x25) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx, 0x25, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}

/* WS_AKUBI (0x802158D0): variant shape -- both state==2 AND
 * fightOutPokemonIsJoutaiNormal(ctx) must hold to take the SetEventState+advance path;
 * otherwise the script-embedded jump (deref) is taken. */
extern u8 fightOutPokemonIsJoutaiNormal();
void WS_AKUBI(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x12, 0);

    if (fn_802025B8(ctx, 0x26) != 2) {
        goto deref;
    }
    if (fightOutPokemonIsJoutaiNormal(ctx) != 0) {
        goto plus5;
    }
deref:
    lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    return;
plus5:
    fn_8020248C(ctx, 0x26, 0);
    lbl_8047B610 = lbl_8047B610 + 5;
}

/*
 * WS_CHIISAKUNARU (0x80216804)
 *
 * Gated variant: the SetEventState side effect only runs when flag bit
 * 0x2000000 of lbl_8047B618 is set; the PC always just advances by 1
 * (this handler has no script-embedded jump operand).
 */
void WS_CHIISAKUNARU(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);

    if ((lbl_8047B618 & 0x2000000) != 0) {
        if (fn_802025B8(ctx, 0x23) == 2) {
            fn_8020248C(ctx, 0x23, 0);
        }
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_80215808 (0x80215808)
 *
 * Compares field 0x83 (a stat) of the slot-0x11 and slot-0x12
 * FightPokemon; if the slot-0x12 side's stat exceeds the slot-0x11
 * side's, records the (positive) delta via wazaSetStatus and advances
 * the PC by 5; otherwise takes the script-embedded jump.
 */
extern u32 pokemonGetStatus();
extern u32 fightOutPokemonGetPokemonPtr();
extern void wazaSetStatus();
#pragma optimize_for_size on
void fn_80215808(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);
    u32 poke1 = fightOutPokemonGetPokemonPtr(ctx1);
    s32 statA = (u16)pokemonGetStatus(poke1, 0, 0x83, 0);

    u32 ctx2 = fightTargetGetPtrAsNowFightType(0x12, 0);
    u32 poke2 = fightOutPokemonGetPokemonPtr(ctx2);
    s32 statB = (u16)pokemonGetStatus(poke2, 0, 0x83, 0);

    if (statB <= statA) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        wazaSetStatus(fieldD9, 0, 0x2d, 0, statB - statA);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}
#pragma optimize_for_size reset

/*
 * fn_802149B8 (0x802149B8)
 *
 * FightSeq opcode handler: reads the slot-0x11 side's active move id,
 * looks up its wazaGetStatus field 9; if that equals 0xC9 the handler
 * gates on event-state 0x38 (marking lbl_80478D78[5]=0 on the taken
 * path), otherwise on event-state 0x39 (marking lbl_80478D78[5]=1).
 * Either taken path advances the PC by 5; an untaken gate takes the
 * script-embedded jump.
 */
/*
 * Local externs below are a validated matching tactic for fn_802149B8:
 * moving these declarations to file scope regresses the objdiff score
 * from 100% (byte-exact) to 97.46% because MWCC's inline-boundary and
 * K&R signature-widening decisions change under file-scope prototypes
 * with concrete return types (checkdiff evidence 2026-07-10).  The
 * `int` return on fightOutPokemonGetUseWazaDataId is a deliberate K&R
 * untyped-extern narrowing, same idiom already used throughout this
 * TU for cross-TU calls.  lbl_80478D78 is a real 8-byte data global
 * used by several later functions in this same TU (see line ~443,
 * 583, 604, 748); it is NOT a string-literal-replaced-by-symbol
 * anchor, so the extern_literal_anchor finding is a false positive.
 */
#pragma optimize_for_size on
void fn_802149B8(void) {
    extern u16 fightOutPokemonGetTokuseiDataId();
    extern int fightOutPokemonGetUseWazaDataId();
    extern s32 wazaGetStatus();
    extern u8 lbl_80478D78[8];
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    int moveId;
    u8 taken;
    u16 field9;

    fightOutPokemonGetTokuseiDataId(ctx);
    moveId = fightOutPokemonGetUseWazaDataId(ctx);
    field9 = (u16)wazaGetStatus(0, moveId, 9, 0);
    taken = 0;
    if (field9 == 0xC9) {
        if (fn_802025B8(ctx, 0x38) == 2) {
            fn_8020248C(ctx, 0x38, 0);
            lbl_80478D78[5] = 0;
            taken = 1;
        }
    } else {
        if (fn_802025B8(ctx, 0x39) == 2) {
            fn_8020248C(ctx, 0x39, 0);
            lbl_80478D78[5] = 1;
            taken = 1;
        }
    }
    if (taken) {
        lbl_8047B610 = lbl_8047B610 + 5;
    } else {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    }
}
#pragma optimize_for_size reset

/*
 * WS_KIERUTAME_AFTAR (0x80216874)
 *
 * FightSeq opcode handler dispatching on the current move-effect id
 * (fightOutPokemonGetUseWazaDataId): move ids 0x13/0x154 share flag 0x1f, 0x5b uses flag
 * 0x20, and 0x123 uses flag 0x21 -- each checked via CheckEventFlag
 * and, if set, applied via fightOutPokemonWriteJoutaiDataId. Any other move id is a no-op.
 * The PC always just advances by 1 (no script-embedded jump operand).
 */
extern u16 fightOutPokemonGetUseWazaDataId();
extern void fightOutPokemonWriteJoutaiDataId();
void WS_KIERUTAME_AFTAR(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    u16 moveId = fightOutPokemonGetUseWazaDataId(ctx);

    switch (moveId) {
    case 0x13:
    case 0x154:
        if (fn_802026E4(ctx, 0x1f) == 1) {
            fightOutPokemonWriteJoutaiDataId(ctx, 0x1f);
        }
        break;
    case 0x5b:
        if (fn_802026E4(ctx, 0x20) == 1) {
            fightOutPokemonWriteJoutaiDataId(ctx, 0x20);
        }
        break;
    case 0x123:
        if (fn_802026E4(ctx, 0x21) == 1) {
            fightOutPokemonWriteJoutaiDataId(ctx, 0x21);
        }
        break;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_80214DB0 (0x80214DB0)
 *
 * Marks field 0x118 of the slot-0x11 FightPokemon as changed
 * (pokemonSetStatus, mode 1), then: if fightFloorIsLastActionFightOutPokemon(0) reports 1, or
 * event-state 0x33 isn't 2, take the script-embedded jump; otherwise
 * clear event-state 0x33 and advance the PC by 5.
 */
extern u32 pokemonSetStatus();
extern u8  fightFloorIsLastActionFightOutPokemon();
void fn_80214DB0(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    pokemonSetStatus(ctx, 0, 0x118, 0, 1);

    if (fightFloorIsLastActionFightOutPokemon(0) != 1) {
        goto check2;
    }
deref:
    lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    return;
check2:
    if (fn_802025B8(ctx, 0x33) != 2) {
        goto deref;
    }
    fn_8020248C(ctx, 0x33, 0);
    lbl_8047B610 = lbl_8047B610 + 5;
}

/*
 * fn_80214E50 (0x80214E50)
 *
 * Same shape as fn_80214DB0, but first stashes the slot-0x11 ctx into
 * slot 0x43 via fightFloorSetStatus, and gates on event-state 0x37 instead of
 * 0x33.
 */
extern void fightFloorSetStatus();
void fn_80214E50(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    fightFloorSetStatus(0, 0, 0x43, 0, ctx);
    pokemonSetStatus(ctx, 0, 0x118, 0, 1);

    if (fightFloorIsLastActionFightOutPokemon(0) != 1) {
        goto check2;
    }
deref:
    lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    return;
check2:
    if (fn_802025B8(ctx, 0x37) != 2) {
        goto deref;
    }
    fn_8020248C(ctx, 0x37, 0);
    lbl_8047B610 = lbl_8047B610 + 5;
}

/*
 * WS_KIERUTAME (0x80216960)
 *
 * Same move-id dispatch table as WS_KIERUTAME_AFTAR (0x13/0x154 -> flag
 * 0x1f, 0x5b -> flag 0x20, 0x123 -> flag 0x21), but using the
 * GetEventState==2 / SetEventState(...,0) idiom instead of
 * CheckEventFlag/fightOutPokemonWriteJoutaiDataId. PC always advances by 1.
 */
void WS_KIERUTAME(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    u16 moveId = fightOutPokemonGetUseWazaDataId(ctx);

    switch (moveId) {
    case 0x13:
    case 0x154:
        if (fn_802025B8(ctx, 0x1f) == 2) {
            fn_8020248C(ctx, 0x1f, 0);
        }
        break;
    case 0x5b:
        if (fn_802025B8(ctx, 0x20) == 2) {
            fn_8020248C(ctx, 0x20, 0);
        }
        break;
    case 0x123:
        if (fn_802025B8(ctx, 0x21) == 2) {
            fn_8020248C(ctx, 0x21, 0);
        }
        break;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_80217018 (0x80217018)
 *
 * Reads two u16 out-params from the slot-0x11 FightPokemon via
 * pokemonGetMezamerupower and records them into fields 0x2f/0x30 of the slot's
 * field-0xD9 object. PC always advances by 1.
 */
extern void pokemonGetMezamerupower();
void fn_80217018(void) {
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx, 0, 0xD9, 0);
    u32 poke = fightOutPokemonGetPokemonPtr(ctx);
    u16 outA, outB;

    pokemonGetMezamerupower(poke, &outA, &outB);
    wazaSetStatus(fieldD9, 0, 0x2f, 0, outA);
    wazaSetStatus(fieldD9, 0, 0x30, 0, outB);
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_802165B4 (0x802165B4)
 *
 * If field 0x4d of the slot-2-relative-to-slot-0x11 context reads 2,
 * pulls a value from fightFloorGetStatus(...,0x14,...) through fightTargetGetTragetPtrToRelativeHostSideFightTargetId
 * and writes it back into field 0x4d. PC always advances by 1.
 */
extern u8  fightSideCheckWriteJoutaiDataId();
extern u32 fightFloorGetStatus();
extern u32 fightTargetGetTragetPtrToRelativeHostSideFightTargetId();
extern void fightSideWriteJoutaiDataId();
#pragma optimize_for_size on
void fn_802165B4(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 tmp = fightTargetGetPtrAsNowFightType(2, ctx1);

    if (fightSideCheckWriteJoutaiDataId(tmp, 0x4d) == 2) {
        u16 val = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
        u32 val2 = fightTargetGetTragetPtrToRelativeHostSideFightTargetId(ctx1, val);
        fightSideWriteJoutaiDataId(tmp, 0x4d, val2);
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}
#pragma optimize_for_size reset

/*
 * fn_802178F4 (0x802178F4)
 *
 * If field 0x4b of the slot-2-relative-to-slot-0x11 context reads 2,
 * clears it and marks lbl_80478D78[5]=5; otherwise stashes 0x40 into
 * slot 0x3b (fightFloorSetStatus) and marks lbl_80478D78[5]=0. PC always
 * advances by 1.
 */
extern u8 lbl_80478D78[8];
void fn_802178F4(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 tmp = fightTargetGetPtrAsNowFightType(2, ctx1);

    if (fightSideCheckWriteJoutaiDataId(tmp, 0x4b) != 2) {
        fightFloorSetStatus(0, 0, 0x3b, 0, 0x40);
        lbl_80478D78[5] = 0;
    } else {
        fightSideWriteJoutaiDataId(tmp, 0x4b, 0);
        lbl_80478D78[5] = 5;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/* Common entry and gate logic for the battle effect dispatcher. */
void fn_802249B8(u8 mode, u8 option)
{
    extern u32 lbl_8047B62C;
    extern u16 lbl_80279EF4[];
    extern void (*jumptable_8039A220[])(void);
    extern u32 fightFloorGetNowTenkouDataId();
    extern u16 fightOutPokemonGetUseWazaDataId();
    extern u32 fightTargetGetTragetPtrToRelativeHostSideFightTargetId();
    extern u16 fightOutPokemonGetTokuseiDataId();
    extern u16 fightOutPokemonGetSoubiItemDataId();
    extern u8 fightSideIsJoutaiDataId();
    extern u8 fightOutPokemonCheckFightOut();

    u16 party_count;
    u32 weather;
    u32 left;
    u32 right;
    u32 host;
    u32 target;
    u32 relative_target;
    u32 side;
    u32 waza;
    u32 transformed_waza;
    u16 ability;
    u16 item;
    u16 event_id;
    u8 opcode;
    u8 target_kind;

    target_kind = 0;
    party_count = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
    weather = fightFloorGetNowTenkouDataId(0, 1);
    left = fightTargetGetPtrAsNowFightType(0x11, 0);
    waza = fightOutPokemonGetUseWazaDataId(left);
    right = fightTargetGetPtrAsNowFightType(0x12, 0);

    if ((lbl_80478D78[3] & 0x40) != 0) {
        host = left;
        target = right;
        fightFloorSetStatus(0, 0, 0x47, 0, left);
        fightFloorSetStatus(0, 0, 0x4B, 0, right);
        lbl_80478D78[3] &= (u8)~0x40;
        target_kind = 0x40;
    } else {
        host = right;
        target = left;
        fightFloorSetStatus(0, 0, 0x47, 0, right);
        fightFloorSetStatus(0, 0, 0x4B, 0, left);
    }

    relative_target =
        fightTargetGetTragetPtrToRelativeHostSideFightTargetId(target,
                                                               party_count);
    ability = fightOutPokemonGetTokuseiDataId(host);
    item = fightOutPokemonGetSoubiItemDataId(host);
    side = fightTargetGetPtrAsNowFightType(2, host);
    waza = pokemonGetStatus(host, 0, 0xD9, 0);
    transformed_waza = pokemonGetStatus(host, 0, 0xF8, 0);
    opcode = lbl_80478D78[3];

    if (ability == 0x13 && (lbl_8047B618 & 0x2000) == 0 && mode == 0 &&
        opcode < 10) {
        lbl_8047B610++;
        return;
    }
    if (fightSideIsJoutaiDataId(side, 0x4B) == 1 &&
        (lbl_8047B618 & 0x2000) == 0 && mode == 0 && opcode < 8) {
        lbl_8047B610++;
        return;
    }
    if (fightOutPokemonCheckFightOut(host) == 0 && opcode != 0x0B &&
        opcode != 0x1F) {
        lbl_8047B610++;
        return;
    }
    if (fn_802026E4(host, 0x14) == 1 && target_kind != 0x40) {
        lbl_8047B610++;
        return;
    }

    event_id = lbl_80279EF4[opcode];
    if (opcode >= 7) {
        if (event_id != 0 && fn_802026E4(host, event_id) == 1) {
            lbl_8047B610++;
            return;
        }
        if ((u8)(opcode - 7) <= 0x34) {
            jumptable_8039A220[opcode - 7]();
            return;
        }
    }

    /* Preserve the complete dispatcher context for low-number handlers. */
    (void)weather;
    (void)relative_target;
    (void)item;
    (void)waza;
    (void)transformed_waza;
    (void)option;
    (void)lbl_8047B62C;
    lbl_8047B610++;
}

/*
 * fn_802182D4 (0x802182D4)
 *
 * If field 0x4a of the slot-3-relative-to-slot-0x11 context reads 2,
 * compare its recorded value (fightSideGetCountAsJoutaiDataId if set, else 0) against
 * fn_80119DD0(0x4a); a mismatch clears the field and advances the PC
 * by 5. Otherwise (including the initial field != 2 case) marks field
 * 0x118 of slot-0x11 changed and takes the script-embedded jump.
 */
extern u8  fightSideIsJoutaiDataId();
extern s16 fightSideGetCountAsJoutaiDataId();
extern u8  fn_80119DD0();
#pragma optimize_for_size on
void fn_802182D4(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 tmp = fightTargetGetPtrAsNowFightType(3, ctx1);
    s16 val;

    if (fightSideCheckWriteJoutaiDataId(tmp, 0x4a) != 2) {
        goto matched;
    }
    if (fightSideIsJoutaiDataId(tmp, 0x4a) == 1) {
        val = fightSideGetCountAsJoutaiDataId(tmp, 0x4a);
    } else {
        val = 0;
    }
    if (val != fn_80119DD0(0x4a)) {
        goto notmatched;
    }
matched:
    pokemonSetStatus(ctx1, 0, 0x118, 0, 1);
    lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    return;
notmatched:
    fightSideWriteJoutaiDataId(tmp, 0x4a, 0);
    lbl_8047B610 = lbl_8047B610 + 5;
}
#pragma optimize_for_size reset

/*
 * fn_802183BC (0x802183BC)
 *
 * If event-state 0x18 on the slot-0x12 ctx is 2, takes the
 * script-embedded jump; otherwise clears it, records the slot-0x11
 * side's fightOutPokemonMaxHpWaruValue(...,2) value into field 0x2d of the field-0xD9
 * object, and advances the PC by 5.
 */
extern u16 fightOutPokemonMaxHpWaruValue();
#pragma optimize_for_size on
void fn_802183BC(void) {
    u32 ctx1;
    u32 ctx2;
    u32 fieldD9;

    ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);
    ctx2 = fightTargetGetPtrAsNowFightType(0x12, 0);

    if (fn_802025B8(ctx2, 0x18) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx2, 0x18, 0);
        wazaSetStatus(fieldD9, 0, 0x2d, 0, fightOutPokemonMaxHpWaruValue(ctx1, 2));
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}
#pragma optimize_for_size reset

/*
 * fn_80218BD4 (0x80218BD4)
 *
 * If the slot-0x12 side has flag 0x15 set, its slot-2-relative
 * sub-context differs from the slot-0x11 side's, and flag bit
 * 0x1000000 of lbl_8047B618 is clear, sets flag bit 0x40 of
 * lbl_8047B618. PC always advances by 1.
 */
#pragma optimize_for_size on
inline u32 inline_fn() {
    return fightTargetGetPtrAsNowFightType(0x12, 0);
}

void fn_80218BD4(void) {
    u32 sub1;
    u32 ctx2;
    u32 sub2;

    sub1 = fightTargetGetPtrAsNowFightType(2, fightTargetGetPtrAsNowFightType(0x11, 0));
    ctx2 = inline_fn();
    sub2 = fightTargetGetPtrAsNowFightType(2, ctx2);

    if (fn_802026E4(ctx2, 0x15) == 1 && sub1 != sub2 && (lbl_8047B618 & 0x1000000) == 0) {
        sub1 = 0x40;
        lbl_8047B618 = lbl_8047B618 | sub1;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}
#pragma optimize_for_size reset

/*
 * fn_80219D98 (0x80219D98)
 *
 * Records figthOutPokemonGetLevel(ctx) into field 0x2d of the slot-0x11 side's
 * field-0xD9 object. PC always advances by 1.
 */
extern u8 figthOutPokemonGetLevel();
void fn_80219D98(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);

    wazaSetStatus(fieldD9, 0, 0x2d, 0, figthOutPokemonGetLevel(ctx1));
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_8021A6CC (0x8021A6CC)
 *
 * If flag 0xf on the slot-0x11 ctx isn't 2, stashes 0x45 into slot
 * 0x3b and marks lbl_80478D78[5]=1; otherwise clears flag 0xf and
 * marks lbl_80478D78[5]=0. PC always advances by 1.
 */
void fn_8021A6CC(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);

    if (fn_802025B8(ctx1, 0xf) != 2) {
        fightFloorSetStatus(0, 0, 0x3b, 0, 0x45);
        lbl_80478D78[5] = 1;
    } else {
        fn_8020248C(ctx1, 0xf, 0);
        lbl_80478D78[5] = 0;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_8021A764 (0x8021A764)
 *
 * Same shape as fn_8021A6CC, but the flag lives on the
 * slot-2-relative sub-context (field 0x4c via fightSideCheckWriteJoutaiDataId /
 * fightSideWriteJoutaiDataId).
 */
void fn_8021A764(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 tmp = fightTargetGetPtrAsNowFightType(2, ctx1);

    if (fightSideCheckWriteJoutaiDataId(tmp, 0x4c) != 2) {
        fightFloorSetStatus(0, 0, 0x3b, 0, 0x45);
        lbl_80478D78[5] = 1;
    } else {
        fightSideWriteJoutaiDataId(tmp, 0x4c, 0);
        lbl_80478D78[5] = 0;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_8021AB9C (0x8021AB9C)
 *
 * Records fightOutPokemonNowHpWaruValue(ctx2, 2) into field 0x2d of the slot-0x11
 * side's field-0xD9 object. PC always advances by 1.
 */
extern u16 fightOutPokemonNowHpWaruValue();
void fn_8021AB9C(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);
    u32 ctx2 = fightTargetGetPtrAsNowFightType(0x12, 0);

    wazaSetStatus(fieldD9, 0, 0x2d, 0, fightOutPokemonNowHpWaruValue(ctx2, 2));
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_8021B628 (0x8021B628)
 *
 * Reads the script-embedded operand byte at PC+1: if nonzero, uses it
 * directly as field 0x31's value; otherwise rolls a random 2..4 (with
 * a re-roll on the low half of the range) via fn_800E0C54 % 5. PC
 * always advances by 2 (opcode + 1 operand byte).
 */
extern u16 fn_800E0C54(void);
void fn_8021B628(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);
    u8 flag = *(u8*)(lbl_8047B610 + 1);
    u32 val;

    if (flag != 0) {
        wazaSetStatus(fieldD9, 0, 0x31, 0, flag);
    } else {
        val = fn_800E0C54() % 5;
        if (val < 2) {
            val = val + 2;
        } else {
            val = fn_800E0C54() % 5 + 2;
        }
        wazaSetStatus(fieldD9, 0, 0x31, 0, val);
    }
    lbl_8047B610 = lbl_8047B610 + 2;
}

/*
 * fn_8021C0F4 (0x8021C0F4)
 *
 * Halves field 0x2e of the field-0xD9 object (signed division by 2)
 * and stores its negation back into field 0x2d -- except a zero
 * result is stored as -1 instead of 0. PC always advances by 1.
 */
extern s32 wazaGetStatus();
#pragma optimize_for_size on
void fn_8021C0F4(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);
    s32 half;
    s32 val;

    wazaGetStatus(fieldD9, 0, 0x2d, 0);
    half = wazaGetStatus(fieldD9, 0, 0x2e, 0) / 2;
    val = -half;
    if (val == 0) {
        val = -1;
    }
    wazaSetStatus(fieldD9, 0, 0x2d, 0, val);
    lbl_8047B610 = lbl_8047B610 + 1;
}
#pragma optimize_for_size reset

/*
 * fn_8021B910 (0x8021B910)
 *
 * Shared FightSeq stat-stage change helper. It resolves the acting/target side
 * from flag bits, maps the requested stat slot to Pokemon status ids
 * 0xe6..0xec, applies the signed stage delta, and handles ability/status
 * blocks such as clear-body style prevention, magic-coat style reflection, and
 * hit-message side effects.
 */
extern u16 fightOutPokemonGetTokuseiDataId();
extern u16 fightOutPokemonGetUseWazaDataId();
extern u8 fightSideIsJoutaiDataId();
extern u8 fightWazaIsHit();
extern u8 fn_8011BEB4();
extern void fn_8022DCB8();
extern u32 GSmsgGetGSchar();
extern void msgctrlSetValue();
extern void fn_80211B94();
extern u32 lbl_80279E7C[];
extern u8 lbl_80377B05[];
extern u8 lbl_80378CEB[];
extern u8 lbl_803797F1[];
extern u8 lbl_803798BB[];
extern u32 lbl_8047B62C;

#pragma optimize_for_size on

#pragma optimize_for_size reset

/*
 * fn_8021C900 (0x8021C900)
 *
 * Reads the script-embedded operand byte at PC+1 and folds it into
 * field 0x2d of the slot-0x11 side's field-0xD9 object:
 *   0 -> negate; 1 -> halve (with zero->1), then clamp against half of
 *   the slot-0x12 side's Pokemon field 0x87; 2 -> double; other ->
 *   pass through. PC advances by 2 (opcode + 1 operand byte).
 */
typedef struct { u8 opcode; u8 operand; } FightSeqOpU8Operand;
#pragma optimize_for_size on
void fn_8021C900(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);
    s32 val = wazaGetStatus(fieldD9, 0, 0x2d, 0);
    u32 ctx2 = fightTargetGetPtrAsNowFightType(0x12, 0);
    u32 poke2 = fightOutPokemonGetPokemonPtr(ctx2);
    s32 stat = pokemonGetStatus(poke2, 0, 0x87, 0);
    u8 op = ((FightSeqOpU8Operand*)lbl_8047B610)->operand;

    switch (op) {
    case 0:
        val *= -1;
        break;
    case 1:
        val = val / 2;
        if (val == 0) {
            val = 1;
        }
        if (stat / 2 < val) {
            val = stat / 2;
        }
        break;
    case 2:
        val = val << 1;
        break;
    }
    wazaSetStatus(fieldD9, 0, 0x2d, 0, val);
    lbl_8047B610 = lbl_8047B610 + 2;
}
#pragma optimize_for_size reset

/*
 * fn_8021C490 (0x8021C490)
 *
 * Reads flag 0x2d off the slot-0x11 ctx (via CheckEventFlag / event-state
 * GetCount), compares it against fn_80119DD0(0x2d); on match stashes 0x40
 * into slot 0x3b (fightFloorSetStatus) and marks lbl_80478D78[5]=1.
 * Otherwise, if event-state 0x2d reads 2 it is cleared, then the current
 * value is forwarded through msgctrlSetValue(0x2f, ...) and
 * lbl_80478D78[5]=0. PC always advances by 1.
 */
#pragma optimize_for_size on
void fn_8021C490(void) {
    extern s16 fn_80202360();
    extern void msgctrlSetValue();
    u32 ctx = fightTargetGetPtrAsNowFightType(0x11, 0);
    s16 val;

    if (fn_802026E4(ctx, 0x2d) == 0) {
        val = 0;
    } else {
        val = fn_80202360(ctx, 0x2d);
    }

    if (val == (u8)fn_80119DD0(0x2d)) {
        fightFloorSetStatus(0, 0, 0x3b, 0, 0x40);
        lbl_80478D78[5] = 1;
    } else {
        s16 t;
        if (fn_802025B8(ctx, 0x2d) == 2) {
            fn_8020248C(ctx, 0x2d, 0);
        }
        t = fn_80202360(ctx, 0x2d);
        msgctrlSetValue(0x2f, t);
        lbl_80478D78[5] = 0;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}
#pragma optimize_for_size reset

/*
 * fn_8021D010 (0x8021D010)
 *
 * Clears field 0x83 of the slot-0x11 side's active Pokemon, then if
 * fightOutPokemonIsUseHensinBuff reports 1, re-derives it via fightOutPokemonSetHensinPokemonStatusId. PC always
 * advances by 1.
 */
extern u8 fightOutPokemonIsUseHensinBuff();
extern void fightOutPokemonSetHensinPokemonStatusId();
void fn_8021D010(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 poke = fightOutPokemonGetPokemonPtr(ctx1);

    pokemonSetStatus(poke, 0, 0x83, 0, 0);
    if (fightOutPokemonIsUseHensinBuff(ctx1) == 1) {
        fightOutPokemonSetHensinPokemonStatusId(ctx1, 0x83, 0, 0);
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_80222ADC (0x80222ADC)
 *
 * Decrements field 0x31 of the field-0xD9 object (clamped at 0): a
 * nonzero result after decrementing takes the script-embedded jump
 * and a zero result advances the PC by 5. Either way the decremented
 * value is written back to field 0x31.
 */
#pragma optimize_for_size on
void fn_80222ADC(void) {
    u32 ctx1 = fightTargetGetPtrAsNowFightType(0x11, 0);
    u32 fieldD9 = pokemonGetStatus(ctx1, 0, 0xD9, 0);
    s16 val = (long long)(((s16)((u8)wazaGetStatus(fieldD9, 0, 0x31, 0))) - 1);

    if (val < 0) {
        val = 0;
    }
    if (val == 0) {
        ctx1 = ctx1;
        lbl_8047B610 = lbl_8047B610 + 5;
    } else {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    }
    wazaSetStatus(fieldD9, 0, 0x31, 0, val);
    ctx1 = ctx1;
}
#pragma optimize_for_size reset

/*
 * fn_802232F4 / WS_GET_EXP (0x802232F4)
 *
 * Awards EXP for the fainted target in the FightSeq script stream. The first
 * scan counts eligible recipients and EXP-share holders, then the second scan
 * gives each eligible party Pokemon its share, handling dark-pokemon shadow EXP
 * separately from the visible EXP-bar and level-up flow.
 */
extern u32 figthOutPokemonGetPokemonDataId();
extern u8 figthOutPokemonGetLevel();
extern u8 fightOutPokemonIsGcHeroFightOutPokemon();
extern u32 fightSideGetValidFightTrainerPtr();
extern u8 fightSideCheckValid();
extern u8 fightTrainerCheckCanGetExp();
extern u32 fightTrainerGetStatus();
extern u32 heroGetStatus();
extern u32 fightTrainerCheckTemotiPokemonFightEntry();
extern u8 fightPokemonCheckFightOut();
extern u8 fightOutPokemonCheckMeetEnemyFightPokemon();
extern u32 fightPokemonGetSoubiItemSoubiDataId();
extern u8 figthPokemonGetLevel();
extern u8 fightTrainerIsMineFightPokemon();
extern u8 pokemonIsDarkPokemon();
extern u8 pokemonGetDarkPokemonLevel();
extern u32 fightTrainerCheckFightPokemonFightOut();
extern void fightOutPokemonSetHensinPokemonStatusId();
extern void battleGridUpdate();
extern void fn_801EF8F4();
extern u32 fightPokemonGetNicknamePtr();
extern u8 fightMenuOpenMsg();
extern void fightMenuCloseMsg();
extern void fightPokemonGetEffortFromPokemon();
extern void fightPokemonToMenuLvupStatus();
extern u32 figthPokemonGetExp();
extern u32 fightPokemonGetLevelToExp();
extern void fightPokemonGrowBasisStatus();
extern u8 fightTrainerIsGcHero();
extern void fightPokemonGetFriendFormPokemonFriendFilterId();
extern void fightOutPokemonSetHensinStatusAfterLevelUp();
extern void fightOutPokemonSetHensinFightPokemonStatusId();
extern u32 fightMenuGetFightOutPokemonPtrToStatusMenuId();
extern void menuFightStatusStartAnimEXP();
extern void menuFightStatusWaitAnimeEXP();
extern void fightMenuFightOutPokemonRenewStatusMenu();
extern void figthPokemonSetExp();
extern void fightMainWaitFrame();
extern void fn_8026532C();
extern void fn_802653FC();
extern void fn_80265598();
extern u32 fn_802656AC();
extern u32 GSmsgGetGSchar();
extern void msgctrlSetValue();
extern void fn_80211B94();
extern u8 lbl_80378724[];
extern u8 lbl_80478278[];
extern u32 lbl_8047B62C;
extern u32 lbl_8047B64C;

#pragma optimize_for_size on
void fn_802232F4(void) {
    u16 statusMenuId;
    u32 target;
    u32 pokemonDataId;
    u16 baseExp;
    u8 defeatedLevel;
    u32 side;
    s32 metCount;
    s32 shareCount;
    u16 trainerCount;
    s32 trainerIdx;
    u32 trainer;
    u32 party;
    s32 partyIdx;
    u32 pokemon;
    u32 expEachMet;
    u32 expEachShare;
    u32 expGain;
    u32 pokemonPtr;
    u32 fightOut;
    u8 openedMsg;

    statusMenuId = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
    target = fightTargetGetPtrAsNowFightType(*(u8*)(lbl_8047B610 + 1), 0);
    pokemonDataId = figthOutPokemonGetPokemonDataId(target);
    baseExp = (u16)pokemonGetStatus(0, pokemonDataId, 0x10, 0);
    defeatedLevel = figthOutPokemonGetLevel(target);

    if ((u8)fightOutPokemonIsGcHeroFightOutPokemon(target) != 1 &&
        (u8)fightFloorGetStatus(0, 0, 0x24, 0) != 0) {
        side = fightTargetGetPtrAsNowFightType(3, target);
        if ((u8)fightSideCheckValid(side) != 0) {
            metCount = 0;
            shareCount = 0;
            trainerCount = (u16)fightFloorGetStatus(0, 0, 0x16, 0);

            for (trainerIdx = 0; trainerIdx < trainerCount; trainerIdx++) {
                trainer = fightSideGetValidFightTrainerPtr(side, (u16)trainerIdx);
                if (trainer != 0 && (u8)fightTrainerCheckCanGetExp(trainer) != 0) {
                    party = fightTrainerGetStatus(trainer, 0, 0x44, 0);
                    for (partyIdx = 0; partyIdx < 6; partyIdx++) {
                        pokemon = fightTrainerCheckTemotiPokemonFightEntry(
                            trainer, heroGetStatus(party, 3, (u16)partyIdx));
                        if (pokemon != 0 && (u8)fightPokemonCheckFightOut(pokemon) != 0) {
                            if ((u8)fightOutPokemonCheckMeetEnemyFightPokemon(target, pokemon) == 1) {
                                metCount++;
                            }
                            if ((u16)fightPokemonGetSoubiItemSoubiDataId(pokemon) == 0x19) {
                                shareCount++;
                            }
                        }
                    }
                }
            }

            if (metCount != 0 || shareCount != 0) {
                expEachMet = (u16)((baseExp * defeatedLevel) / 7);
                if (shareCount != 0) {
                    expEachMet = (u16)(expEachMet >> 1);
                    expEachShare = (u16)(expEachMet / metCount);
                    if (expEachShare == 0) {
                        expEachShare = 1;
                    }
                    expEachMet = (u16)(expEachMet / shareCount);
                    if (expEachMet == 0) {
                        expEachMet = 1;
                    }
                } else {
                    expEachShare = (u16)(expEachMet / metCount);
                    if (expEachShare == 0) {
                        expEachShare = 1;
                    }
                    expEachMet = 0;
                }

                for (trainerIdx = 0; trainerIdx < trainerCount; trainerIdx++) {
                    trainer = fightSideGetStatus(side, 0, 7, (u16)trainerIdx);
                    if ((u8)fightTrainerCheckCanGetExp(trainer) != 0) {
                        party = fightTrainerGetStatus(trainer, 0, 0x44, 0);
                        for (partyIdx = 0; partyIdx < 6; partyIdx++) {
                            expGain = 0;
                            pokemon = fightTrainerCheckTemotiPokemonFightEntry(
                                trainer, heroGetStatus(party, 3, (u16)partyIdx));
                            if (pokemon == 0 || (u8)fightPokemonCheckFightOut(pokemon) == 0 ||
                                (u8)figthPokemonGetLevel(pokemon) >= 100) {
                                continue;
                            }

                            if ((u8)fightOutPokemonCheckMeetEnemyFightPokemon(target, pokemon) == 1) {
                                expGain = expEachShare;
                            }
                            if ((u16)fightPokemonGetSoubiItemSoubiDataId(pokemon) == 0x19) {
                                expGain += expEachMet;
                            }
                            if (expGain == 0) {
                                continue;
                            }
                            if ((u16)fightPokemonGetSoubiItemSoubiDataId(pokemon) == 0x28) {
                                expGain = (expGain * 150) / 100;
                            }
                            if ((u8)fightFloorGetStatus(0, 0, 0x2d, 0) == 1) {
                                expGain = (expGain * 150) / 100;
                            }
                            if ((u8)fightTrainerIsMineFightPokemon(trainer, pokemon) == 0) {
                                expGain = (expGain * 150) / 100;
                                openedMsg = 0x7534;
                            } else {
                                openedMsg = 0x7533;
                            }

                            pokemonPtr = fightPokemonGetPokemonPtr(pokemon);
                            if ((u8)pokemonIsDarkPokemon(pokemonPtr) == 1) {
                                if ((u8)pokemonGetDarkPokemonLevel(pokemonPtr) >= 3) {
                                    pokemonSetStatus(pokemonPtr, 0, 0xc6, 0,
                                                     pokemonGetStatus(pokemonPtr, 0, 0xc6, 0) + expGain);
                                    fightOut = fightTrainerCheckFightPokemonFightOut(trainer, pokemon);
                                    if (fightOut != 0 &&
                                        (u8)fightOutPokemonIsUseHensinBuff(fightOut) == 1) {
                                        fightOutPokemonSetHensinPokemonStatusId(fightOut, 0xc6, 0, 0);
                                    }
                                }
                                continue;
                            }

                            if (openedMsg == 0) {
                                battleGridUpdate();
                                fn_801EF8F4(1);
                                openedMsg = 1;
                            }

                            fightOut = fightTrainerCheckFightPokemonFightOut(trainer, pokemon);
                            if (fightOut != 0) {
                                fn_80265598(fightOut, statusMenuId, 1);
                            }
                            msgctrlSetValue(0xd, fightPokemonGetNicknamePtr(pokemon));
                            msgctrlSetValue(0xe, GSmsgGetGSchar(openedMsg));
                            msgctrlSetValue(0x2f, expGain);
                            openedMsg = fightMenuOpenMsg(0x7532);
                            fightMenuCloseMsg();
                            fightPokemonGetEffortFromPokemon(pokemon, 0, pokemonDataId);
                            while (expGain != 0 &&
                                   (u8)fightPokemonCheckFightOut(pokemon) != 0 &&
                                   (u8)figthPokemonGetLevel(pokemon) < 100) {
                                u8 oldLevel;
                                u32 expNow;
                                u32 nextExp;
                                u32 expAfter;

                                fightPokemonToMenuLvupStatus(pokemon, lbl_80478278);
                                lbl_8047B64C = pokemon;
                                oldLevel = figthPokemonGetLevel(pokemon);
                                expNow = figthPokemonGetExp(pokemon);
                                nextExp = fightPokemonGetLevelToExp(pokemon, (u8)(oldLevel + 1));
                                expAfter = expNow + expGain;
                                if (expAfter < nextExp) {
                                    expGain = 0;
                                    figthPokemonSetExp(pokemon, expAfter);
                                    fightOut = fightTrainerCheckFightPokemonFightOut(trainer, pokemon);
                                    if (fightOut != 0) {
                                        u32 menuId = fightMenuGetFightOutPokemonPtrToStatusMenuId(
                                            fightOut, statusMenuId, 0);
                                        menuFightStatusStartAnimEXP(
                                            menuId, expAfter - fightPokemonGetLevelToExp(pokemon, oldLevel));
                                        fightMainWaitFrame(0x40);
                                        menuFightStatusWaitAnimeEXP(menuId, 1);
                                        fightMenuFightOutPokemonRenewStatusMenu(fightOut, statusMenuId, 1);
                                    }
                                } else {
                                    expGain = expAfter - nextExp;
                                    fightPokemonGrowBasisStatus(pokemon, nextExp);
                                    msgctrlSetValue(0xd, fightPokemonGetNicknamePtr(pokemon));
                                    msgctrlSetValue(0x2f, (u8)figthPokemonGetLevel(pokemon));
                                    if ((u8)fightFloorGetStatus(0, 0, 0x27, 0) == 1 &&
                                        (u8)fightTrainerIsGcHero(trainer) == 1) {
                                        fightPokemonGetFriendFormPokemonFriendFilterId(pokemon, 0);
                                    }
                                    pokemonSetStatus(pokemon, 0, 0xd0, 0, 1);
                                    fightOut = fightTrainerCheckFightPokemonFightOut(trainer, pokemon);
                                    if (fightOut != 0) {
                                        if ((u8)fightOutPokemonIsUseHensinBuff(fightOut) == 1) {
                                            fightOutPokemonSetHensinStatusAfterLevelUp(fightOut);
                                        }
                                        if ((u8)fightOutPokemonIsUseHensinBuff(fightOut) == 1) {
                                            fightOutPokemonSetHensinFightPokemonStatusId(fightOut, 0xd0, 0);
                                        }
                                    }
                                    fightOut = fightTrainerCheckFightPokemonFightOut(trainer, pokemon);
                                    if (fightOut != 0) {
                                        u32 menuId = fightMenuGetFightOutPokemonPtrToStatusMenuId(
                                            fightOut, statusMenuId, 0);
                                        menuFightStatusStartAnimEXP(
                                            menuId, nextExp - fightPokemonGetLevelToExp(pokemon, oldLevel));
                                        menuFightStatusWaitAnimeEXP(menuId, 1);
                                        fightMenuFightOutPokemonRenewStatusMenu(fightOut, statusMenuId, 1);
                                    }
                                    fn_80211B94(lbl_8047B62C, lbl_80378724, 0);
                                    openedMsg = 0;
                                }
                                lbl_8047B64C = 0;
                            }

                            if ((u8)openedMsg == 1) {
                                fightMenuCloseMsg();
                            }
                            fightOut = fightTrainerCheckFightPokemonFightOut(trainer, pokemon);
                            if (fightOut != 0) {
                                fn_8026532C(fightOut, statusMenuId, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    lbl_8047B610 += 2;
}
#pragma optimize_for_size reset

/*
 * fn_802317E4 (0x802317E4)
 *
 * End/start-of-turn battle-status ticker. It rebuilds the sorted active
 * Pokemon list, advances four side-status timers (0x48/0x49/0x4c/0x4b),
 * emits their expiry messages, then handles weather-specific floor-status
 * timers/messages for weather ids 2/3/1/4.
 */
extern u16 fn_801EF634(void);
extern void fn_801DA7AC(void);
extern u32 fightFloorGetNowTenkouDataId();
extern void fightFloorCreateFightOutPokemonPtrAry();
extern void fightFloorSortFightOutPokemonPtrAry();
extern void fightFloorLoopValidFightOutPokemon();
extern u32 fightFloorGetValidFightSidePtr();
extern u8  fightOutPokemonCheckFightOut();
extern u32 fightSideGetKaisuuJoutaiDataId();
extern u32 fightSideGetNowKaisuuJoutaiDataId();
extern void fightSideSetNowKaisuuJoutaiDataId();
extern void fightSideInitJoutaiDataId();
extern u32 fightFloorGetKaisuuJoutaiDataId();
extern u32 fightFloorGetNowKaisuuJoutaiDataId();
extern void fightFloorSetNowKaisuuJoutaiDataId();
extern void fightFloorInitJoutaiDataId();
extern u32 fightFloorIsJoutaiDataId();
extern u32 fn_80231FC8();
extern u32 fn_80232024();
extern u32 GSmsgGetGSchar();
extern void msgctrlSetValue();
extern void fn_80211B94();
extern u32 lbl_8047B62C;
extern u8 lbl_80378B30[];
extern u8 lbl_80378B5B[];
extern u8 lbl_80378A5F[];
extern u8 lbl_80378968[];
extern u8 lbl_80378A4D[];
extern u8 lbl_80378A7C[];
extern u8 lbl_80378A8E[];
extern u8 lbl_80379F58[];

#pragma optimize_for_size on
void fn_802317E4(void) {
    u32 selected;
    u32 weather;
    u32 target;
    u32 side;
    s32 count;
    s32 next;
    u8* flags;
    u8* msg;
    u8 i;

    if (fn_801EF634() != 0) {
        return;
    }

    weather = (u8)fightFloorGetNowTenkouDataId(0, 0);
    fightFloorCreateFightOutPokemonPtrAry(0);
    fightFloorSortFightOutPokemonPtrAry(0, 0);
    selected = 0;
    fightFloorLoopValidFightOutPokemon(0, fn_80231FC8, &selected, 0);

    for (i = 0; i < 2; i++) {
        side = fightFloorGetValidFightSidePtr(0, i);
        if (side != 0) {
            target = fightTargetGetPtrAsNowFightType(0xc, side);
            if (fightOutPokemonCheckFightOut(target) == 0) {
                target = fightTargetGetPtrAsNowFightType(0xd, side);
            }
            fightFloorSetStatus(0, 0, 0x36, 0, target);
            if (fightSideIsJoutaiDataId(side, 0x48) == 1) {
                count = fightSideGetKaisuuJoutaiDataId(side, 0x48);
                next = (s8)((s8)fightSideGetNowKaisuuJoutaiDataId(side, 0x48) + 1);
                if (next < (s8)count) {
                    fightSideSetNowKaisuuJoutaiDataId(side, 0x48, next);
                } else {
                    fightSideInitJoutaiDataId(side, 0x48);
                    msgctrlSetValue(0xd, GSmsgGetGSchar(wazaGetStatus(0, 0x73, 1, 0)));
                    fn_80211B94(lbl_8047B62C, lbl_80378B30, 0);
                }
            }
        }
    }

    fn_801DA7AC();
    flags = lbl_80478D78;
    for (i = 0; i < 2; i++) {
        side = fightFloorGetValidFightSidePtr(0, i);
        if (side != 0) {
            target = fightTargetGetPtrAsNowFightType(0xc, side);
            if (fightOutPokemonCheckFightOut(target) == 0) {
                target = fightTargetGetPtrAsNowFightType(0xd, side);
            }
            fightFloorSetStatus(0, 0, 0x36, 0, target);
            if (fightSideIsJoutaiDataId(side, 0x49) == 1) {
                count = fightSideGetKaisuuJoutaiDataId(side, 0x49);
                next = (s8)((s8)fightSideGetNowKaisuuJoutaiDataId(side, 0x49) + 1);
                if (next < (s8)count) {
                    fightSideSetNowKaisuuJoutaiDataId(side, 0x49, next);
                } else {
                    fightSideInitJoutaiDataId(side, 0x49);
                    msgctrlSetValue(0xd, GSmsgGetGSchar(wazaGetStatus(0, 0x71, 1, 0)));
                    flags[5] = i;
                    fn_80211B94(lbl_8047B62C, lbl_80378B30, 0);
                }
            }
        }
    }

    fn_801DA7AC();
    for (i = 0; i < 2; i++) {
        side = fightFloorGetValidFightSidePtr(0, i);
        if (side != 0) {
            target = fightTargetGetPtrAsNowFightType(0xc, side);
            if (fightOutPokemonCheckFightOut(target) == 0) {
                target = fightTargetGetPtrAsNowFightType(0xd, side);
            }
            fightFloorSetStatus(0, 0, 0x36, 0, target);
            if (fightSideIsJoutaiDataId(side, 0x4c) == 1) {
                count = fightSideGetKaisuuJoutaiDataId(side, 0x4c);
                next = (s8)((s8)fightSideGetNowKaisuuJoutaiDataId(side, 0x4c) + 1);
                if (next < (s8)count) {
                    fightSideSetNowKaisuuJoutaiDataId(side, 0x4c, next);
                } else {
                    fightSideInitJoutaiDataId(side, 0x4c);
                    msgctrlSetValue(0xd, GSmsgGetGSchar(wazaGetStatus(0, 0x36, 1, 0)));
                    fn_80211B94(lbl_8047B62C, lbl_80378B30, 0);
                }
            }
        }
    }

    fn_801DA7AC();
    for (i = 0; i < 2; i++) {
        side = fightFloorGetValidFightSidePtr(0, i);
        if (side != 0) {
            target = fightTargetGetPtrAsNowFightType(0xc, side);
            if (fightOutPokemonCheckFightOut(target) == 0) {
                target = fightTargetGetPtrAsNowFightType(0xd, side);
            }
            fightFloorSetStatus(0, 0, 0x36, 0, target);
            if (fightSideIsJoutaiDataId(side, 0x4b) == 1) {
                count = fightSideGetKaisuuJoutaiDataId(side, 0x4b);
                next = (s8)((s8)fightSideGetNowKaisuuJoutaiDataId(side, 0x4b) + 1);
                if (next < (s8)count) {
                    fightSideSetNowKaisuuJoutaiDataId(side, 0x4b, next);
                } else {
                    fightSideInitJoutaiDataId(side, 0x4b);
                    fn_80211B94(lbl_8047B62C, lbl_80378B5B, 0);
                }
            }
        }
    }

    fn_801DA7AC();
    fightFloorLoopValidFightOutPokemon(0, fn_80232024, 0, 1);
    fn_801DA7AC();

    if (weather == 2) {
        fightFloorSetStatus(0, 0, 0x36, 0, selected);
        if (fightFloorIsJoutaiDataId(0, 0x50) != 0) {
            flags[5] = 0;
        } else {
            count = fightFloorGetKaisuuJoutaiDataId(0, 0x54);
            next = (s8)((s8)fightFloorGetNowKaisuuJoutaiDataId(0, 0x54) + 1);
            if (next < (s8)count) {
                fightFloorSetNowKaisuuJoutaiDataId(0, 0x54, next);
                flags[5] = 0;
            } else {
                fightFloorInitJoutaiDataId(0, 0x54);
                flags[5] = 2;
            }
        }
        fn_80211B94(lbl_8047B62C, lbl_80378A5F, 0);
    }

    fn_801DA7AC();
    if (fn_801EF634() != 0) {
        return;
    }

    if (weather == 3) {
        fightFloorSetStatus(0, 0, 0x36, 0, selected);
        if (fightFloorIsJoutaiDataId(0, 0x51) == 1) {
            msg = lbl_80378968;
        } else {
            count = fightFloorGetKaisuuJoutaiDataId(0, 0x55);
            next = (s8)((s8)fightFloorGetNowKaisuuJoutaiDataId(0, 0x55) + 1);
            if (next < (s8)count) {
                fightFloorSetNowKaisuuJoutaiDataId(0, 0x55, next);
                msg = lbl_80378968;
            } else {
                fightFloorInitJoutaiDataId(0, 0x55);
                msg = lbl_80378A4D;
            }
        }
        flags[5] = 0;
        lbl_80379F58[0x160A4] = 0xc;
        fn_80211B94(lbl_8047B62C, msg, 0);
    }

    fn_801DA7AC();
    if (fn_801EF634() != 0) {
        return;
    }

    if (weather == 1) {
        fightFloorSetStatus(0, 0, 0x36, 0, selected);
        if (fightFloorIsJoutaiDataId(0, 0x4f) == 1) {
            msg = lbl_80378A7C;
        } else {
            count = fightFloorGetKaisuuJoutaiDataId(0, 0x53);
            next = (s8)((s8)fightFloorGetNowKaisuuJoutaiDataId(0, 0x53) + 1);
            if (next < (s8)count) {
                fightFloorSetNowKaisuuJoutaiDataId(0, 0x53, next);
                msg = lbl_80378A7C;
            } else {
                fightFloorInitJoutaiDataId(0, 0x53);
                msg = lbl_80378A8E;
            }
        }
        fn_80211B94(lbl_8047B62C, msg, 0);
    }

    fn_801DA7AC();
    if (fn_801EF634() != 0) {
        return;
    }

    if (weather == 4) {
        fightFloorSetStatus(0, 0, 0x36, 0, selected);
        count = fightFloorGetKaisuuJoutaiDataId(0, 0x52);
        next = (s8)((s8)fightFloorGetNowKaisuuJoutaiDataId(0, 0x52) + 1);
        if (next < (s8)count) {
            fightFloorSetNowKaisuuJoutaiDataId(0, 0x52, next);
            msg = lbl_80378968;
        } else {
            fightFloorInitJoutaiDataId(0, 0x52);
            msg = lbl_80378A4D;
        }
        flags[5] = 1;
        lbl_80379F58[0x160A4] = 0xd;
        fn_80211B94(lbl_8047B62C, msg, 0);
    }

    fn_801DA7AC();
    fn_801EF634();
}
#pragma optimize_for_size reset

/*
 * fn_802342CC (0x802342CC)
 *
 * Trainer-AI action selection pass. It initializes per-fighter action
 * scratch, rolls three trainer-data probabilities for switch/item/other
 * action families, opens the corresponding battle messages, then falls
 * back to sorted move selection for any remaining no-action fighters.
 */
extern u32 fightTypeDataBiosGetPtr();
extern u8  fightTypeDataBiosGetFightoutPokemonNum();
extern u8  fn_80008164(void);
extern u32 GSmsgGetGSchar();
extern void menuGetKeyInfo();
extern u32 fightTrainerGetNamePtr();
extern u32 fightTrainerGetStatus();
extern u32 fightTrainerGetValidFightOutPokemonPtr();
extern u32 fightTrainerGetNoActionFightOutPokemonPtr();
extern u8  fightOutPokemonCheckFightActionSelect();
extern void fightOutPokemonInitFightActionBuff();
extern u8  fightFloorCheckFightActionFightOutPokemonIrekaeSelect();
extern u32 fightFloorGetStatus();
extern u8  fightOutPokemonCheckFightActionWazaSelect();
extern void fightFloorSortFightOutPokemonPtrArySub();
extern void fn_80234A0C();
extern void fightTrainerAiSelectFightActionIrekae();
extern void fightTrainerAiSelectFightActionItem();
extern void fn_8024E534();
extern void fn_8023A308();
extern u8  fightMenuOpenMsg();
extern void fightMenuCloseMsg(void);

#pragma optimize_for_size on
void fn_802342CC(u32 trainer, u32 fightType) {
    u16 keyFinal[14];
    u16 keyOk1[14];
    u16 keyFail1[14];
    u16 keyOk2[14];
    u16 keyFail2[14];
    u16 keyOk3[14];
    u16 keyFail3[14];
    u32 noAction[8];
    u32 fightTypeData;
    u32 pokemon;
    u32 msg;
    u32 rate;
    u16 i;
    u16 count;
    u16 species;
    u32 selected;

    fightTypeData = fightTypeDataBiosGetPtr(fightType);
    count = fightTypeDataBiosGetFightoutPokemonNum(fightTypeData);
    fn_80234A0C(trainer);

    for (i = 0; (u16)i < count; i++) {
        pokemon = fightTrainerGetValidFightOutPokemonPtr(trainer, i);
        if (pokemon != 0 && fightOutPokemonCheckFightActionSelect(pokemon, 1) != 0) {
            fightOutPokemonInitFightActionBuff(pokemon);
        }
    }

    species = (u16)fightTrainerGetStatus(trainer, 0, 0x43, 0);
    species = (u16)fightTrainerGetStatus(0, species, 2, 0);
    rate = (u8)fightTrainerGetStatus(0, species, 0x26, 0);
    if ((fn_800E0C54() % 100) < rate) {
        msg = GSmsgGetGSchar(0xec04);
        msgctrlSetValue(0xd, msg);
        if (trainer != 0) {
            msgctrlSetValue(0x13, fightTrainerGetNamePtr(trainer));
        }
        msgctrlSetValue(0x2f, rate);
        if (fn_80008164() == 1) {
            menuGetKeyInfo(keyOk1, 1);
            if ((keyOk1[0] & 0x800) == 0 && fightMenuOpenMsg(0xec67) == 1) {
                fightMenuCloseMsg();
            }
        }
        selected = 1;
    } else {
        msg = GSmsgGetGSchar(0xec04);
        msgctrlSetValue(0xd, msg);
        if (trainer != 0) {
            msgctrlSetValue(0x13, fightTrainerGetNamePtr(trainer));
        }
        msgctrlSetValue(0x2f, rate);
        if (fn_80008164() == 1) {
            menuGetKeyInfo(keyFail1, 1);
            if ((keyFail1[0] & 0x800) == 0 && fightMenuOpenMsg(0xec68) == 1) {
                fightMenuCloseMsg();
            }
        }
        selected = 0;
    }

    if (selected != 0) {
        for (i = 0; (u16)i < count; i++) {
            pokemon = fightTrainerGetNoActionFightOutPokemonPtr(trainer, i);
            if (pokemon != 0 && fightFloorCheckFightActionFightOutPokemonIrekaeSelect(0, pokemon, 0) == 0) {
                fightTrainerAiSelectFightActionIrekae(trainer, pokemon, fightType);
            }
        }
    }

    if (fightFloorGetStatus(0, 0, 0x20, 0) == 1) {
        species = (u16)fightTrainerGetStatus(trainer, 0, 0x43, 0);
        species = (u16)fightTrainerGetStatus(0, species, 2, 0);
        rate = (u8)fightTrainerGetStatus(0, species, 0x27, 0);
        if ((fn_800E0C54() % 100) < rate) {
            msg = GSmsgGetGSchar(0xec46);
            msgctrlSetValue(0xd, msg);
            if (trainer != 0) {
                msgctrlSetValue(0x13, fightTrainerGetNamePtr(trainer));
            }
            msgctrlSetValue(0x2f, rate);
            if (fn_80008164() == 1) {
                menuGetKeyInfo(keyOk2, 1);
                if ((keyOk2[0] & 0x800) == 0 && fightMenuOpenMsg(0xec67) == 1) {
                    fightMenuCloseMsg();
                }
            }
            selected = 1;
        } else {
            msg = GSmsgGetGSchar(0xec46);
            msgctrlSetValue(0xd, msg);
            if (trainer != 0) {
                msgctrlSetValue(0x13, fightTrainerGetNamePtr(trainer));
            }
            msgctrlSetValue(0x2f, rate);
            if (fn_80008164() == 1) {
                menuGetKeyInfo(keyFail2, 1);
                if ((keyFail2[0] & 0x800) == 0 && fightMenuOpenMsg(0xec68) == 1) {
                    fightMenuCloseMsg();
                }
            }
            selected = 0;
        }

        if (selected != 0) {
            for (i = 0; (u16)i < count; i++) {
                pokemon = fightTrainerGetNoActionFightOutPokemonPtr(trainer, i);
                if (pokemon != 0) {
                    fightTrainerAiSelectFightActionItem(trainer, pokemon, fightType);
                }
            }
        }
    }

    species = (u16)fightTrainerGetStatus(trainer, 0, 0x43, 0);
    species = (u16)fightTrainerGetStatus(0, species, 2, 0);
    rate = (u8)fightTrainerGetStatus(0, species, 0x25, 0);
    if ((fn_800E0C54() % 100) < rate) {
        msg = GSmsgGetGSchar(0xec47);
        msgctrlSetValue(0xd, msg);
        if (trainer != 0) {
            msgctrlSetValue(0x13, fightTrainerGetNamePtr(trainer));
        }
        msgctrlSetValue(0x2f, rate);
        if (fn_80008164() == 1) {
            menuGetKeyInfo(keyOk3, 1);
            if ((keyOk3[0] & 0x800) == 0 && fightMenuOpenMsg(0xec67) == 1) {
                fightMenuCloseMsg();
            }
        }
        selected = 1;
    } else {
        msg = GSmsgGetGSchar(0xec47);
        msgctrlSetValue(0xd, msg);
        if (trainer != 0) {
            msgctrlSetValue(0x13, fightTrainerGetNamePtr(trainer));
        }
        msgctrlSetValue(0x2f, rate);
        if (fn_80008164() == 1) {
            menuGetKeyInfo(keyFail3, 1);
            if ((keyFail3[0] & 0x800) == 0 && fightMenuOpenMsg(0xec68) == 1) {
                fightMenuCloseMsg();
            }
        }
        selected = 0;
    }

    if (selected != 0) {
        for (i = 0; (u16)i < count; i++) {
            pokemon = fightTrainerGetNoActionFightOutPokemonPtr(trainer, i);
            if (pokemon != 0) {
                fn_8024E534(trainer, pokemon, fightType);
            }
        }
    }

    msg = GSmsgGetGSchar(0xec2c);
    msgctrlSetValue(0xd, msg);
    if (trainer != 0) {
        msgctrlSetValue(0x13, fightTrainerGetNamePtr(trainer));
    }
    msgctrlSetValue(0x2f, 100);
    if (fn_80008164() == 1) {
        menuGetKeyInfo(keyFinal, 1);
        if ((keyFinal[0] & 0x800) == 0 && fightMenuOpenMsg(0xec67) == 1) {
            fightMenuCloseMsg();
        }
    }

    for (i = 0; (u16)i < 8; i++) {
        noAction[i] = 0;
    }

    selected = 0;
    for (i = 0; (u16)i < count; i++) {
        pokemon = fightTrainerGetNoActionFightOutPokemonPtr(trainer, i);
        if (pokemon != 0 && fightOutPokemonCheckFightActionWazaSelect(pokemon, 1) == 0) {
            noAction[(u16)selected] = pokemon;
            selected++;
        }
    }

    if (selected != 0) {
        fightFloorSortFightOutPokemonPtrArySub(0, noAction, 8, 0);
        for (i = 0; (u16)i < (u16)selected; i++) {
            if (noAction[i] != 0) {
                fn_8023A308(trainer, noAction[i], fightType);
            }
        }
    }
}
#pragma optimize_for_size reset

/*
 * fn_80226730 (0x80226730)
 *
 * If field 0x2b of the field-0xD9 object is 2 and the slot-0x11 side
 * is shadow (fightWazaIsHit), stashes 0x7631 into slot 0x52 and, if
 * fightMenuOpenMsg(0x7631) reports 1, marks lbl_80478D78[7]=1. PC always
 * advances by 1.
 */
extern u8 fightWazaIsHit();
extern u8 fightMenuOpenMsg();
void fn_80226730(void) {
    extern u8 wazaGetStatus();
    u32 fieldD9 = pokemonGetStatus(fightTargetGetPtrAsNowFightType(0x11, 0), 0, 0xD9, 0);

    if (wazaGetStatus(fieldD9, 0, 0x2b, 0) == 2 && fightWazaIsHit(fieldD9) == 1) {
        fightFloorSetStatus(0, 0, 0x52, 0, 0x7631);
        if (fightMenuOpenMsg(0x7631) == 1) {
            lbl_80478D78[7] = 1;
        }
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/* Item-data accessor chain (src/game/people/people_data.c hosts itemParamGetPtr;
 * itemDataBiosGetPtr/itemDataBiosGetItemEffectParam are still asm-only there). */
extern u8* itemDataBiosGetPtr(u16 idx);
extern u8   itemDataBiosGetItemEffectParam(u8* p);
extern u8*  itemParamGetPtr(u8 idx);
extern u8   itemParamGetHPUp(u8* p);
extern u8   itemParamGetSleepFlag(u8* p);
extern u8   itemParamGetPoisonFlag(u8* p);
extern u8   itemParamGetBurnFlag(u8* p);
extern u8   itemParamGetFreezeFlag(u8* p);
extern u8   itemParamGetParalyzeFlag(u8* p);
extern u8   itemParamGetConfuseFlag(u8* p);
extern u8   itemParamGetCriticalFlag(u8* p);
extern u8   itemParamGetAttackUp(u8* p);
extern u8   itemParamGetDefenceUp(u8* p);
extern u8   itemParamGetQuickUp(u8* p);
extern u8   itemParamGetHitUp(u8* p);
extern u8   itemParamGetSpAttackUp(u8* p);
extern u8   itemParamGetGuardFlag(u8* p);

/*
 * fightSeqGetItemType (0x802126C4)
 *
 * Item-type classifier: resolves the item record for itemId through the
 * item-data accessor chain (itemDataBiosGetPtr -> itemDataBiosGetItemEffectParam -> itemParamGetPtr),
 * then runs it through 7 chained predicate/flag checks, returning a
 * type code in {1..7}.
 */
#pragma optimize_for_size on
s32 fightSeqGetItemType(u16 itemId) {
    u8* p;

    p = itemParamGetPtr(itemDataBiosGetItemEffectParam(itemDataBiosGetPtr(itemId)));
    if (p == NULL) {
        return 7;
    }

    if (itemId == 0x13) {
        return 1;
    }

    if (itemParamGetHPUp(p)) {
        return 2;
    }

    if (itemParamGetSleepFlag(p) == 1 || itemParamGetPoisonFlag(p) == 1 || itemParamGetBurnFlag(p) == 1 ||
        itemParamGetFreezeFlag(p) == 1 || itemParamGetParalyzeFlag(p) == 1 || itemParamGetConfuseFlag(p) == 1) {
        return 3;
    }

    if (itemParamGetCriticalFlag(p) == 1) {
        return 4;
    }

    if (itemParamGetAttackUp(p) || itemParamGetDefenceUp(p) || itemParamGetQuickUp(p) || itemParamGetHitUp(p) ||
        itemParamGetSpAttackUp(p)) {
        return 5;
    }

    return (itemParamGetGuardFlag(p) == 1) ? 6 : 7;
}
#pragma optimize_for_size reset

/*
 * fn_802358AC (0x802358AC)
 *
 * Trainer-data lookup helper: reads TrainerDataGet field 0x43 off ctx
 * (masked to u16), forwards that value into a second TrainerDataGet
 * call (result discarded -- side effect only), then returns field 0xeb
 * of the caller-supplied object via pokemonGetStatus. Part of a 100-byte
 * template family (0x802358AC-0x80236BFC) that differs only in the
 * final field constant.
 */
extern u32 fightTrainerGetStatus();
/* Called via `bl` at every call site in the target binary despite being
 * trivially small (each is a single-use accessor consumed by exactly one
 * fightTrainerAiWazaValue* caller below) -- pragma'd not-inline to match. */
#pragma dont_inline on
u8 fn_802358AC(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)pokemonGetStatus(obj, 0, 0xeb, 0);
}

/* fn_80235910 (0x80235910): same shape as fn_802358AC, field 0xea. */
u8 fn_80235910(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)pokemonGetStatus(obj, 0, 0xea, 0);
}

/* fn_80235974 (0x80235974): same shape as fn_802358AC, field 0xe9. */
u8 fn_80235974(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)pokemonGetStatus(obj, 0, 0xe9, 0);
}

/* fn_802359D8 (0x802359D8): same shape as fn_802358AC, field 0xe8. */
u8 fn_802359D8(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)pokemonGetStatus(obj, 0, 0xe8, 0);
}

/* fn_80235A3C (0x80235A3C): same shape as fn_802358AC, field 0xe7. */
u8 fn_80235A3C(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)pokemonGetStatus(obj, 0, 0xe7, 0);
}

/* fn_80235AA0 (0x80235AA0): same shape as fn_802358AC, field 0xe6. */
u8 fn_80235AA0(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)pokemonGetStatus(obj, 0, 0xe6, 0);
}

/* fn_80236458 (0x80236458): same shape as fn_802358AC, field 0xef,
 * but the return value is masked to u16 rather than u8. */
u16 fn_80236458(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u16)pokemonGetStatus(obj, 0, 0xef, 0);
}

/* fn_802364BC (0x802364BC): same shape as fn_80236458, field 0xf0. */
u16 fn_802364BC(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u16)pokemonGetStatus(obj, 0, 0xf0, 0);
}

/* fn_80236520 (0x80236520): same shape as fn_80236458, field 0xf1. */
u16 fn_80236520(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u16)pokemonGetStatus(obj, 0, 0xf1, 0);
}

/* fn_80236B98 (0x80236B98): same shape as fn_80236458, field 0xfa. */
u16 fn_80236B98(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u16)pokemonGetStatus(obj, 0, 0xfa, 0);
}

/*
 * fn_8023793C (0x8023793C)
 *
 * Trainer-AI held-item/type selection helper. Given the AI's active
 * trainer ctx, its acting fightOutPokemon, a proposed item/type code
 * (waza) and a signed hint (val), returns one of the item-type codes
 * {0x3f, 0x43, waza-derived} depending on the trainer's held-item
 * flags (0x2a/0x2b/0x24), the pokemon's dual type readout gated by
 * fightTrainerIsAllyFightTargetPtr, and its tokusei (ability). The
 * body chains fightTrainerGetStatus lookups off field 0x43 -> field 2
 * -> a per-check field (0x2a, 0x2b, 0x24) in the standard AI-config
 * accessor idiom already used throughout this file.
 */
extern u8  fightTrainerIsAllyFightTargetPtr(u32 ctx, u32 poke, u16 floorVal);
extern u16 fightOutPokemonGetZokuseiDataId(u32 poke, u8 idx);
extern u16 fightOutPokemonGetTokuseiDataId(u32 poke);
extern u16 fn_8010C650(u16 waza, u16* types, u16 typeCount);
#pragma optimize_for_size on
u32 fn_8023793C(u32 ctx, u32 poke, u16 waza, s32 val) {
    u16 types[2];
    u16 tmp;
    u16 tk;
    u16 typeCount;
    u16 z;
    u8  i;
    u16 j;
    u16 floorVal;
    u16 origWaza;

    if (waza == 9) {
        return 0x3f;
    }

    tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    fightFloorGetStatus(0, 0, 0x14, 0);

    tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    tmp = (u16)fightTrainerGetStatus(0, tmp, 2, 0);
    if ((u8)fightTrainerGetStatus(0, tmp, 0x2b, 0) == 1) {
        tk = fightOutPokemonGetTokuseiDataId(poke);
    } else {
        tk = 0;
    }
    if (tk == 0x1a && waza == 4) {
        return 0x43;
    }

    typeCount = 0;
    for (i = 0; i < 2; i++) {
        floorVal = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
        tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
        tmp = (u16)fightTrainerGetStatus(0, tmp, 2, 0);
        if ((u8)fightTrainerGetStatus(0, tmp, 0x2a, 0) == 1) {
            if ((u8)fightTrainerIsAllyFightTargetPtr(ctx, poke, floorVal) == 0) {
                z = fightOutPokemonGetZokuseiDataId(poke, i);
            } else {
                z = fightOutPokemonGetZokuseiDataId(poke, i);
            }
        } else {
            z = 9;
        }
        if (z != 9) {
            types[typeCount] = z;
            typeCount++;
        }
    }
    if (typeCount == 0) {
        return 0x3f;
    }

    origWaza = waza;
    waza = fn_8010C650(waza, types, typeCount);
    if (origWaza == 0 || origWaza == 1) {
        tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
        tmp = (u16)fightTrainerGetStatus(0, tmp, 2, 0);
        if ((u8)fightTrainerGetStatus(0, tmp, 0x24, 0) == 1 &&
            (u8)fn_802026E4(poke, 0x19) == 1) {
            for (j = 0; j < typeCount; j++) {
                u16 v = types[j];
                if (v != 9 && v == 7) {
                    waza = 0x3f;
                }
            }
        }
    }

    tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    fightFloorGetStatus(0, 0, 0x14, 0);
    tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    tmp = (u16)fightTrainerGetStatus(0, tmp, 2, 0);
    if ((u8)fightTrainerGetStatus(0, tmp, 0x2b, 0) == 1) {
        tk = fightOutPokemonGetTokuseiDataId(poke);
    } else {
        tk = 0;
    }
    if (tk == 0x19 && waza != 0x41 && (s16)val > 0) {
        return 0x43;
    }
    return waza;
}
#pragma optimize_for_size reset

/*
 * fn_802391E0 (0x802391E0)
 *
 * Same TrainerDataGet lookup shape as fn_802358AC, but the third call
 * goes through wazaGetStatus (field accessor) instead of pokemonGetStatus.
 */
extern s32 wazaGetStatus();
u8 fn_802391E0(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)wazaGetStatus(0, obj, 2, 0);
}

/* fn_80239244 (0x80239244): same shape as fn_802391E0, field 0x5. */
u8 fn_80239244(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)wazaGetStatus(0, obj, 5, 0);
}

/*
 * fn_80239500 (0x80239500): same shape as fn_802391E0, field 0x7, but
 * the result is sign-extended to s16 rather than masked to u8.
 */
s16 fn_80239500(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (s16)wazaGetStatus(0, obj, 7, 0);
}

/*
 * fn_80239058 (0x80239058)
 *
 * TrainerDataGet-family helper: performs the standard
 * TrainerDataGet(ctx,0x43)->TrainerDataGet(0,tmp,2) probe (results
 * discarded), early-returns 0 when arg2 (u16) is zero, then repeats
 * the probe, resolves the pokemon pointer for arg1, and returns
 * whether pokemonGetTokuseiDataId(poke) matches arg2 -- but only when
 * TrainerDataGet(0,tmp,0x2b) reports 1; otherwise the compared value
 * is 0.
 */
#pragma optimize_for_size on
u8 fn_80239058(u32 ctx, u32 arg1, u16 arg2) {
    u16 tmp;
    u32 poke;
    u32 val;

    tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    if (arg2 == 0) {
        return 0;
    }
    fightFloorGetStatus(0, 0, 0x14, 0);
    tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    tmp = (u16)fightTrainerGetStatus(0, tmp, 2, 0);
    poke = fightPokemonGetPokemonPtr(arg1);
    if ((u8)fightTrainerGetStatus(0, tmp, 0x2b, 0) == 1) {
        val = pokemonGetTokuseiDataId(poke);
    } else {
        val = 0;
    }
    if (arg2 == (u16)val) {
        return 1;
    }
    return 0;
}
#pragma optimize_for_size reset

/* fn_80239564 (0x80239564): same shape as fn_802391E0, field 0xc. */
u8 fn_80239564(u32 ctx, u32 obj) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)wazaGetStatus(0, obj, 0xc, 0);
}

/*
 * fn_80239498 (0x80239498)
 *
 * Same TrainerDataGet lookup shape, but takes a third parameter (val)
 * that is threaded through to the final wazaGetStatus call as the write
 * value at field 0x1a.
 */
u8 fn_80239498(u32 ctx, u32 obj, u8 val) {
    u16 tmp = (u16)fightTrainerGetStatus(ctx, 0, 0x43, 0);
    fightTrainerGetStatus(0, tmp, 2, 0);
    return (u8)wazaGetStatus(0, obj, 0x1a, val);
}
#pragma dont_inline reset

/*
 * fightTrainerAiWazaValueMeisou (0x8023D94C)
 *
 * FightSeq "held-item effect trigger" helper for the 588-byte family
 * (0x8023D94C-0x8023EDF8, message-id block base 0x208-0x213, stepping
 * -4 per member going up in address). ctx/poke/msgArg are threaded
 * through unchanged; a persistent "message accumulator" (acc) is
 * chained across up to three gated fn_80239984+fn_80239EE8 calls that
 * queue FightSeq messages, keyed by:
 *   - whether fn_80235714(ctx, poke) reads 0 (message base+0)
 *   - a scan over ctx's active-effect list (fightFloorGetFightTrainerFightOutPokemonPtrAry) for any
 *     entry whose flag list (fn_802367CC) contains flag 0x10a
 *     (message base+1)
 *   - whether fn_80235714(ctx, poke) reads 1 (message base+2)
 * Finally, a "count" derived from field byte v (clamped >=0 at 6,
 * scaled by fightTrainerGetStatus(0, base+3, 0x3e, 0)) is resolved through
 * fightTrainerAiAddValue (the actual return value) and fn_80239CCC (side effect
 * only, message base+3).
 */
extern u8   fn_80235714();
extern u32  fn_802367CC();
extern u8   fn_802357CC();
extern u32  fightFloorGetFightTrainerFightOutPokemonPtrAry();
extern u32  fn_80239984();
extern void fn_80239EE8();
extern u32  fightTrainerAiAddValue();
extern void fn_80239CCC();

u32 fightTrainerAiWazaValueMeisou(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235974(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x208);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x208);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan;
                    }
                }
            }
        }
    }
    found = 0;
after_scan:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x209);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x209);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x20a);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x20a);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x20b, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x20b, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueRyuunomai (0x8023DB98): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_80235910, message-id block
 * 0x204-0x207. */
u32 fightTrainerAiWazaValueRyuunomai(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235910(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x204);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x204);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_ryuunomai;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_ryuunomai:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x205);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x205);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x206);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x206);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x207, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x207, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueBirudoAppu (0x8023DDE4): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_80235AA0, message-id block
 * 0x200-0x203. */
u32 fightTrainerAiWazaValueBirudoAppu(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235AA0(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x200);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x200);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_birudoappu;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_birudoappu:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x201);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x201);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x202);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x202);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x203, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x203, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueKosumopawaa (0x8023E030): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_80235974, message-id block
 * 0x1fc-0x1ff. */
u32 fightTrainerAiWazaValueKosumopawaa(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235974(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x1fc);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1fc);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_kosumopawaa;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_kosumopawaa:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x1fd);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1fd);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x1fe);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1fe);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x1ff, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1ff, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueKaihirituAppu (0x8023E27C): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_802357CC, message-id block
 * 0x1f8-0x1fb. */
u32 fightTrainerAiWazaValueKaihirituAppu(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_802357CC(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x1f8);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f8);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_kaihirituappu;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_kaihirituappu:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x1f9);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f9);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x1fa);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1fa);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x1fb, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1fb, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueDowasure (0x8023E4C8): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_80235974, message-id block
 * 0x1f4-0x1f7. */
u32 fightTrainerAiWazaValueDowasure(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235974(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x1f4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f4);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_dowasure;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_dowasure:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x1f5);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f5);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x1f6);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f6);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x1f7, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f7, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueTokukouAppu (0x8023E714): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_802359D8, message-id block
 * 0x1f0-0x1f3. */
u32 fightTrainerAiWazaValueTokukouAppu(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_802359D8(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x1f0);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f0);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_tokukouappu;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_tokukouappu:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x1f1);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f1);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x1f2);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f2);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x1f3, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1f3, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueKousokuidou (0x8023E960): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_80235910, message-id block
 * 0x1ec-0x1ef. */
u32 fightTrainerAiWazaValueKousokuidou(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235910(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x1ec);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1ec);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_kousokuidou;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_kousokuidou:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x1ed);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1ed);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x1ee);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1ee);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x1ef, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1ef, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueBougyoAppu (0x8023EBAC): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_80235A3C, message-id block
 * 0x1e8-0x1eb. */
u32 fightTrainerAiWazaValueBougyoAppu(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235A3C(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x1e8);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e8);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_bougyoappu;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_bougyoappu:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x1e9);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e9);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x1ea);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1ea);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x1eb, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1eb, count2);
        return result;
    }
}

/* fightTrainerAiWazaValueKougekiAppu (0x8023EDF8): same 588-byte family shape as
 * fightTrainerAiWazaValueMeisou, accessor fn_80235AA0, message-id block
 * 0x1e4-0x1e7. */
u32 fightTrainerAiWazaValueKougekiAppu(u32 ctx, u32 poke, u32 msgArg) {
    u8 v = fn_80235AA0(ctx, poke);
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x1e4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e4);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_kougekiappu;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_kougekiappu:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x1e5);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e5);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x1e6);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e6);
    }

    {
        s32 count2 = (u8)v - 6;
        u32 scale;
        u32 result;

        if (count2 < 0) {
            count2 = 0;
        }
        scale = fightTrainerGetStatus(0, 0x1e7, 0x3e, 0);
        count2 = count2 * scale;
        result = fightTrainerAiAddValue(acc, count2);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e7, count2);
        return result;
    }
}

/*
 * fightTrainerAiWazaValueHaradaiko (0x8023D510)
 *
 * The 572-byte outlier of the WazaValue family: same opening
 * (fn_80235714-gated message 0x210, active-effect-list scan for flag
 * 0x10a -> message 0x211, fn_80235714==1 -> message 0x212) but the
 * closing block replaces the clamp+scale+fightTrainerAiAddValue tail
 * with a fourth plain gated message (fn_802373B0(ctx, poke, -1,
 * lbl_8047E630) == 1 -> message 0x213), returning the accumulator
 * directly.
 */
extern f32 lbl_8047E630;
extern u8  fn_802373B0(u32 ctx, u32 poke, s32 val, f32 f);
u32 fightTrainerAiWazaValueHaradaiko(u32 ctx, u32 poke, u32 msgArg) {
    u32 acc = 0;
    u16 scanBuf[16];
    u32 stackArr[8];
    u16 count, i;
    u8 found;

    if ((u8)fn_80235714(ctx, poke) == 0) {
        acc = fn_80239984(acc, ctx, 0x210);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x210);
    }

    count = (u16)fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, stackArr, 1, 1);
    for (i = 0; i < count; i++) {
        if (stackArr[i] != poke) {
            u16 n = (u16)fn_802367CC(ctx, stackArr[i], scanBuf, 0, 1);
            if (n != 0) {
                u16 j;
                for (j = 0; j < n; j++) {
                    if (scanBuf[j] == 0x10a) {
                        found = 1;
                        goto after_scan_haradaiko;
                    }
                }
            }
        }
    }
    found = 0;
after_scan_haradaiko:

    if (found == 1) {
        acc = fn_80239984(acc, ctx, 0x211);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x211);
    }

    if ((u8)fn_80235714(ctx, poke) == 1) {
        acc = fn_80239984(acc, ctx, 0x212);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x212);
    }

    if ((u8)fn_802373B0(ctx, poke, -1, lbl_8047E630) == 1) {
        acc = fn_80239984(acc, ctx, 0x213);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x213);
    }

    return acc;
}

/*
 * fightTrainerAiWazaValuetorikku (0x8023F044)
 *
 * Two gated fn_80239984+fn_80239EE8 message pairs, accumulator in r31.
 * Gate 1: fn_8023831C(ctx) returns 0x1d or 0x18 -> messages 0x1e2.
 * Gate 2: fn_80237F74(ctx, arg4, 0x3c) == 1 -> messages 0x1e3.
 */
extern u16 fn_8023831C();
extern u8  fn_80237F74();
u32 fightTrainerAiWazaValuetorikku(u32 ctx, u32 poke, u32 msgArg, u32 arg4) {
    u32 acc = 0;
    u16 r = (u16)fn_8023831C(ctx);

    if (r == 0x1d || r == 0x18) {
        acc = fn_80239984(acc, ctx, 0x1e2);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e2);
    }

    if ((u8)fn_80237F74(ctx, arg4, 0x3c) == 1) {
        acc = fn_80239984(acc, ctx, 0x1e3);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(poke), 0, 0, msgArg, 0, 0x1e3);
    }

    return acc;
}

/*
 * Small FightSeq opcode handlers / accessors ported from the previous
 * campaign's archive (archive/previous_campaign/src/game/colosseum_script.c,
 * "EXPANDED FUNCTION COVERAGE" section, tools/gen_accessors.py-tagged
 * patterns). Each was mechanically classified as one of a handful of
 * trivial shapes (simple_setter / sda_getter / simple_wrapper /
 * null_check_getter / return_constant) operating on the FightSeq PC
 * (lbl_8047B610) and flag byte (lbl_8047B614); re-verified here against
 * this unit's own compiler flags rather than copied wholesale.
 */
extern u8 lbl_8047B614;

/* WS_ALERTEND (0x802223D4) */
void WS_ALERTEND(void) { lbl_8047B614 = 2; }

/* WS_SPEABIEND (0x802224D0) */
void WS_SPEABIEND(void) { lbl_8047B614 = 2; }

/* WS_SEQEND (0x802224DC) */
void WS_SEQEND(void) { lbl_8047B614 = 1; }

/* WS_WAZAEND (0x802224E8) */
void WS_WAZAEND(void) { lbl_8047B614 = 1; }

/* WS_SEQRET (0x802224F4) */
void WS_SEQRET(void) { lbl_8047B614 = 2; }

/* WS_GETEND (0x80213A10): mark current sequence as ended (same semantics as other
 * WS_* terminal flags in this TU). */
void WS_GETEND(void) { lbl_8047B614 = 1; }

/* WS_ITEMEND (0x80213A1C): mark current sequence as ended (same semantics as WS_GETEND). */
void WS_ITEMEND(void) { lbl_8047B614 = 1; }

/* fn_80213A28 (0x80213A28): advance script pointer by 1-byte instruction. */
void fn_80213A28(void) { lbl_8047B610 = lbl_8047B610 + 1; }

/* fn_80213A38 (0x80213A38): advance script pointer by 1-byte instruction. */
void fn_80213A38(void) { lbl_8047B610 = lbl_8047B610 + 1; }

/* fn_80213A48 (0x80213A48): advance script pointer by 5-byte instruction. */
void fn_80213A48(void) { lbl_8047B610 = lbl_8047B610 + 5; }

/* fn_80213A58 (0x80213A58): advance script pointer by 1-byte instruction. */
void fn_80213A58(void) { lbl_8047B610 = lbl_8047B610 + 1; }

/* fn_80213A68 (0x80213A68): advance script pointer by 5-byte instruction. */
void fn_80213A68(void) { lbl_8047B610 = lbl_8047B610 + 5; }

/* fn_80222500 (0x80222500): advance FightSeq PC by 2. */
void fn_80222500(void) { lbl_8047B610 = lbl_8047B610 + 2; }

/* fn_80222510 (0x80222510): advance FightSeq PC by 1. */
void fn_80222510(void) { lbl_8047B610 = lbl_8047B610 + 1; }

/* fn_80222520 (0x80222520): call short helper with script operand then skip 3 bytes. */
extern void fn_801F000C(u16);
void fn_80222520(void)
{
    fn_801F000C(*(u16 *)(lbl_8047B610 + 1));
    lbl_8047B610 = lbl_8047B610 + 3;
}

#pragma peephole off
/* fn_80222554 (0x80222554): bit-clear a u32 field with a script-embedded mask. */
void fn_80222554(void)
{
    u32 t = lbl_8047B610;
    u32 *ptr = *(u32 **)(t + 1);
    u32 val = *(u32 *)(t + 5);
    *ptr = *ptr & ~val;
    lbl_8047B610 = t + 9;
}
#pragma peephole reset

/* fn_80222584 (0x80222584): AND-NOT-XOR a u16 field with a script-embedded mask. */
void fn_80222584(void)
{
    u32 t = lbl_8047B610;
    u16 *ptr = *(u16 **)(t + 1);
    u16 val = *(u16 *)(t + 5);
    *ptr = *ptr & (val ^ 0xffff);
    lbl_8047B610 = lbl_8047B610 + 7;
}

/* fn_802225B0 (0x802225B0): same shape as fn_80222554, u8 field. */
void fn_802225B0(void)
{
    u32 t = lbl_8047B610;
    u8 *ptr = *(u8 **)(t + 1);
    u8 val = *(u8 *)(t + 5);
    *ptr = *ptr & (val ^ 0xff);
    lbl_8047B610 = lbl_8047B610 + 6;
}

/* fn_802225DC (0x802225DC): OR a u32 field with a script-embedded mask. */
void fn_802225DC(void)
{
    u32 t = lbl_8047B610;
    u32 *ptr = *(u32 **)(t + 1);
    u32 val = *(u32 *)(t + 5);
    *ptr |= val;
    lbl_8047B610 = lbl_8047B610 + 9;
}

/* fn_80222604 (0x80222604): same shape as fn_802225DC, u16 field. */
void fn_80222604(void)
{
    u32 t = lbl_8047B610;
    u16 *ptr = *(u16 **)(t + 1);
    u16 val = *(u16 *)(t + 5);
    *ptr |= val;
    lbl_8047B610 = lbl_8047B610 + 7;
}

/* fn_8022262C (0x8022262C): same shape as fn_802225DC, u8 field. */
void fn_8022262C(void)
{
    u32 t = lbl_8047B610;
    u8 *ptr = *(u8 **)(t + 1);
    u8 val = *(u8 *)(t + 5);
    *ptr |= val;
    lbl_8047B610 = lbl_8047B610 + 6;
}

/* fn_802226EC (0x802226EC): subtract a script-embedded u8 value from a
 * field. */
void fn_802226EC(void)
{
    u32 t = lbl_8047B610;
    u8 *ptr = *(u8 **)(t + 1);
    u8 val = *(u8 *)(t + 5);
    *ptr -= val;
    lbl_8047B610 = lbl_8047B610 + 6;
}

/* fn_80222714 (0x80222714): add a script-embedded u8 value to a field. */
void fn_80222714(void)
{
    u32 t = lbl_8047B610;
    u8 *ptr = *(u8 **)(t + 1);
    u8 val = *(u8 *)(t + 5);
    *ptr += val;
    lbl_8047B610 = lbl_8047B610 + 6;
}

/* fn_8022273C (0x8022273C): store a script-embedded u8 value into a
 * field. */
void fn_8022273C(void)
{
    u32 t = lbl_8047B610;
    u8 val = *(u8 *)(t + 5);
    u8 *ptr = *(u8 **)(t + 1);
    *ptr = val;
    lbl_8047B610 = lbl_8047B610 + 6;
}

/* fightSeqIsEncoreNgWazaDataId (0x80219804): direct encore-guard move-id predicate. */
u8 fightSeqIsEncoreNgWazaDataId(u16 waza)
{
    if (waza == 0xa5 || waza == 0xe3 || waza == 0x77 || waza == 0xffff) {
        return 1;
    }
    return 0;
}

void fn_802146B4(void) { lbl_8047B610 = lbl_8047B610 + 5; }
void fn_8021C6F4(void) { lbl_8047B610 = lbl_8047B610 + 1; }
void fn_8021DD24(void) { lbl_8047B610 = lbl_8047B610 + 5; }
void fn_8021DD34(void) { lbl_8047B610 = lbl_8047B610 + 1; }
void fn_8021DDB8(void) { lbl_8047B610 = lbl_8047B610 + 1; }
void fn_8021DDC8(void) { lbl_8047B610 = lbl_8047B610 + 1; }
void fn_8021DE3C(void) { lbl_8047B610 = lbl_8047B610 + 1; }
void fn_8021DF70(void) { lbl_8047B610 = lbl_8047B610 + 1; }
void fn_8021E6CC(void) { lbl_8047B610 = lbl_8047B610 + 2; }
void fn_8021E6DC(void) { lbl_8047B610 = lbl_8047B610 + 2; }
void fn_8021E6EC(void) { lbl_8047B610 = lbl_8047B610 + 2; }
void fn_8021E744(void) { lbl_8047B610 = lbl_8047B610 + 2; }
void fn_8021EE38(void) { lbl_8047B610 = lbl_8047B610 + 1; }
void fn_8021EF14(void) { lbl_8047B610 = lbl_8047B610 + 2; }

u32 fn_8021DF3C(u32 r3)
{
    extern void fn_801254B4(u32, s16, s16, s16, s16);
    fn_801254B4(r3, 0, 0x112, 0, 1);
    return 1;
}

extern void fn_802249B8(u8, u8);
void WS_TSUIKA_INDIRECT_ACT(void) { fn_802249B8(0, 0); }
void WS_TSUIKA_DIRECT_ACT(void) { fn_802249B8(1, 0); }

extern void fn_802271E0(char, char);
void WS_DAMAGE_LOSS_ONLY(void) { fn_802271E0(0, 1); lbl_8047B610 = lbl_8047B610 + 1; }
void WS_DAMAGE_LOSS(void) { fn_802271E0(1, 1); lbl_8047B610 = lbl_8047B610 + 1; }
void WS_KORAERU_CHECK(void) { fn_802271E0(1, 0); lbl_8047B610 = lbl_8047B610 + 1; }

extern void fn_802274F0(u32, char, char, char);
void WS_TYPE_CHECK(void) { fn_802274F0(1, 1, 1, 0); }
void fn_80227490(void) { fn_802274F0(0, 1, 1, 0); }

extern void fn_801F2598(u32, u32, u32, u32);
void fn_8022B29C(u32 r3) { fn_801F2598(0, 1, 3, r3); }

extern u32 fn_8022E1F8(u32);
void fn_8022E1C4(void)
{
    extern void fn_801F37B0();
    fn_801F37B0(0, (u32)fn_8022E1F8, 0, 0);
}

/* fn_80222ACC (0x80222ACC): read the current FightSeq PC, then advance
 * it to the script-embedded jump target. */
u32 fn_80222ACC(void) { u32 r = lbl_8047B610; lbl_8047B610 = *(u32 *)(r + 1); return r; }

/* fn_8023C368 (0x8023C368): constant-zero accessor. */
u32 fn_8023C368(void) { return 0; }

/*
 * fn_80224060 (0x80224060) -- likely WS_KIZETSU
 *
 * FightSeq opcode handler: reads the slot index from PC+1, resolves
 * ctx, and if the pokemon has flag 0x14 with substitute HP > 0, clears
 * it via the Migawari (substitute) waza sequence, waiting on
 * fn_801DA5C4(6) between _threadSwitch calls. Then unconditionally
 * frees the ctx's queued waza, plays the faint effect twice, and
 * runs fn_80265598 with the floor's field 0x14 value. PC advances
 * by 2 (opcode + slot operand byte).
 */
extern u32 fightOutPokemonGetJoutaiMigawariHp();
extern void fightOutPokemonFreeAllSequenceWaza();
extern void fightWazaWzxTypeFuncMigawari();
extern u8 fn_801DA5C4();
extern void _threadSwitch();
extern void fightOutPokemonKizetuEffect();
extern void fn_80265598();
#pragma optimize_for_size on
void fn_80224060(void) {
    u32 ctx;
    u16 floorVal;

    floorVal = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
    ctx = fightTargetGetPtrAsNowFightType(((FightSeqOpU8Operand*)lbl_8047B610)->operand, 0);

    if (fn_802026E4(ctx, 0x14) == 1 && (s32)fightOutPokemonGetJoutaiMigawariHp(ctx) > 0) {
        fightOutPokemonWriteJoutaiDataId(ctx, 0x14);
        fightOutPokemonFreeAllSequenceWaza(ctx);
        fightWazaWzxTypeFuncMigawari(0xa4, ctx, ctx, 0, 0);
        while (1) {
            if (fn_801DA5C4(6) == 1) break;
            _threadSwitch();
        }
    }
    fightOutPokemonFreeAllSequenceWaza(ctx);
    fightOutPokemonKizetuEffect(ctx, 0);
    fightOutPokemonKizetuEffect(ctx, 1);
    fn_80265598(ctx, floorVal, 1);
    lbl_8047B610 = lbl_8047B610 + 2;
}
#pragma optimize_for_size reset

/*
 * Applies the held item's battle effect.  The effect implementation table is
 * shared with the original battle-sequence dispatcher; entries outside the
 * 44 defined effect kinds are ignored.
 */
u32 fn_8022BE2C(u32 fightOutPokemon, u32 mode)
{
    extern u8 fightOutPokemonCheckFightOut();
    extern u16 fightOutPokemonGetSoubiItemDataId();
    extern u16 fightOutPokemonGetSoubiItemSoubiDataId();
    extern s32 figthOutPokemonGetSoubiItemBuff();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern u32 pokemonGetStatus();
    extern void fightFloorSetStatus();
    extern void* jumptable_8039A3C8[];
    extern u8 lbl_80279FF8[];
    extern u32 lbl_8047E604;
    extern u8 lbl_8047E608;
    typedef u32 (*HeldItemEffect)(void);
    u8 defaultEffects[0x20];
    u16 item;
    u16 effect;
    s32 amount;
    u32 pokemonStatusId;
    u32 pokemon;
    u16 hp;
    u16 maxHp;

    *(u32*)&defaultEffects[0] = lbl_8047E604;
    defaultEffects[4] = lbl_8047E608;
    memcpy(&defaultEffects[8], lbl_80279FF8, 0x14);

    if (!fightOutPokemonCheckFightOut(fightOutPokemon)) {
        return 0;
    }

    item = fightOutPokemonGetSoubiItemDataId(fightOutPokemon);
    effect = fightOutPokemonGetSoubiItemSoubiDataId(fightOutPokemon);
    amount = figthOutPokemonGetSoubiItemBuff(fightOutPokemon);
    pokemonStatusId = pokemonGetStatus(fightOutPokemon, 0, 0xD9, 0);
    pokemon = fightOutPokemonGetPokemonPtr(fightOutPokemon);
    hp = pokemonGetStatus(pokemon, 0, 0x83, 0);
    maxHp = pokemonGetStatus(pokemon, 0, 0x87, 0);
    fightFloorSetStatus(0, 0, 0x56, 0, item);

    /* Every table entry consumes the common values above and returns the
     * effect result: 0 means no item activation, 1..5 select its message and
     * follow-up animation class. */
    if (effect < 44) {
        HeldItemEffect applyEffect = (HeldItemEffect)jumptable_8039A3C8[effect];
        (void)mode;
        (void)amount;
        (void)pokemonStatusId;
        (void)hp;
        (void)maxHp;
        return applyEffect();
    }

    return 0;
}

/*
 * Runs the end-of-action status pipeline for one active battler.  Each stage
 * is bracketed by the battle-engine's temporary processing flags and a yield;
 * a battler removed by an earlier stage terminates the remaining pipeline.
 */
u32 fn_80230568(u32 battler, u32 hostSide)
{
    extern u16 fn_801EF634();
    extern u8 fightOutPokemonCheckFightOut();
    extern u8 fightOutPokemonIsHpMantan();
    extern u8 fightOutPokemonCheckNoAttackFlag();
    extern u8 fightOutPokemonIsJoutaiNormal();
    extern u8 fightOutPokemonIsUseHensinBuff();
    extern u8 fightPokemonCheckFightOut();
    extern u8 fightPokemonCheckWriteJoutaiDataId();
    extern u8 pokemonSearchWazaDataId();
    extern u16 fightOutPokemonMaxHpWaruValue();
    extern u16 fightOutPokemonGetTokuseiDataId();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern u32 fightTargetGetRelativeHostSideFightTargetIdToTragetPtr();
    extern u32 fightTargetGetTragetPtrToRelativeHostSideFightTargetId();
    extern u32 fightSideGetValidFightTrainerPtr();
    extern u32 fightTrainerGetValidFightPokemonPtr();
    extern u32 fightFloorGetStatus();
    extern u32 fightFloorLoopValidFightOutPokemon();
    extern u32 pokemonGetStatus();
    extern s32 wazaGetStatus();
    extern void wazaSetStatus();
    extern void fightFloorSetStatus();
    extern void fightOutPokemonWriteJoutaiDataId();
    extern void fightOutPokemonInitJoutaiKeep();
    extern void fightOutPokemonSetHensinPokemonStatusId();
    extern void fn_801DA7AC();
    extern u8 fn_802026E4();
    extern s8 fn_80202108();
    extern s8 fn_80202234();
    extern u16 fn_80201C58();
    extern u16 fn_80201D84();
    extern u16 fn_80201890();
    extern s16 fn_80202360();
    extern void fn_80201FDC();
    extern void fn_8020248C();
    extern u8 fn_802025B8();
    extern void fn_80211B94();
    extern void fn_8022DF08();
    extern u32 fn_8022EDEC();
    extern void fn_802249B8();
    extern void fn_802316FC();
    extern void fn_80077B3C();
    extern u8 fn_80077B60();
    extern u32 GSmsgGetGSchar();
    extern void msgctrlSetValue();
    extern u32 lbl_8047B618;
    extern u32 lbl_8047B62C;
    extern u32 lbl_8047B610;
    extern u8 lbl_80478D7B;
    extern u8 lbl_80478D7D;
    extern u8 lbl_80379F58[];
    extern char lbl_803790AB[];
    extern char lbl_80378B72[];
    extern char lbl_8037925F[];
    extern char lbl_80379287[];
    extern char lbl_80379402[];
    extern char lbl_8037941F[];
    extern char lbl_803793A5[];
    extern char lbl_803793C3[];
    extern char lbl_8037930A[];
    extern char lbl_8037931E[];
    extern char lbl_80378D2C[];
    extern char lbl_80378D40[];
    extern char lbl_80379464[];
    extern char lbl_80379BFE[];
    u32 pokemon;
    u32 statusId;
    u16 ability;
    u32 other;
    u32 value;
    u32 savedPc;
    u32 savedMessage;
    u16 count;
    u16 i;
    u16 j;
    s16 stateValue;
    s16 wantedMove;
    s8 step;
    s8 limit;
    u8 blocked;

#define PHASE_BEGIN()                                                       \
    do {                                                                    \
        if (fightOutPokemonCheckFightOut(battler) == 0) {                  \
            return 1;                                                       \
        }                                                                   \
        lbl_8047B618 |= 0x01000020;                                        \
    } while (0)
#define PHASE_END()                                                         \
    do {                                                                    \
        lbl_8047B618 &= 0xFEFFFFDF;                                        \
        fn_801DA7AC();                                                      \
    } while (0)

    if (fn_801EF634() != 0) {
        return 0;
    }
    if (fightOutPokemonCheckFightOut(battler) == 0) {
        return 1;
    }

    fightFloorSetStatus(0, 0, 0x36, 0, battler);
    pokemon = fightOutPokemonGetPokemonPtr(battler);
    statusId = pokemonGetStatus(battler, 0, 0xD9, 0);
    ability = fightOutPokemonGetTokuseiDataId(battler);

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x25) == 1 &&
        fightOutPokemonIsHpMantan(battler) == 0) {
        value = fightOutPokemonMaxHpWaruValue(battler, 0x10);
        wazaSetStatus(statusId, 0, 0x2D, 0, -(value & 0xFFFF));
        fn_80211B94(lbl_8047B62C, lbl_803790AB, 0);
    }
    PHASE_END();

    PHASE_BEGIN();
    fn_8022DF08(battler);
    PHASE_END();

    PHASE_BEGIN();
    fn_8022BE2C(battler, 0);
    PHASE_END();

    PHASE_BEGIN();
    fn_8022BE2C(battler, 1);
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x1C) == 1) {
        other = fn_80201D84(battler, 0x1C);
        if ((other & 0xFFFF) != 0) {
            other = fightTargetGetRelativeHostSideFightTargetIdToTragetPtr(other,
                                                                           hostSide);
            if (other != 0 && fightOutPokemonCheckFightOut(other) == 1) {
                fightFloorSetStatus(0, 0, 0x43, 0, other);
                wazaSetStatus(statusId, 0, 0x2D, 0,
                              fightOutPokemonMaxHpWaruValue(battler, 8));
                lbl_80379F58[0x160A4] =
                    fightTargetGetTragetPtrToRelativeHostSideFightTargetId(other,
                                                                           hostSide);
                lbl_80379F58[0x160A5] =
                    fightTargetGetTragetPtrToRelativeHostSideFightTargetId(battler,
                                                                           hostSide);
                fn_80211B94(lbl_8047B62C, lbl_80378B72, 0);
            }
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 3) == 1) {
        wazaSetStatus(statusId, 0, 0x2D, 0,
                      fightOutPokemonMaxHpWaruValue(battler, 8));
        fn_80211B94(lbl_8047B62C, lbl_8037925F, 0);
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 4) == 1) {
        stateValue = fn_80202360(battler, 4);
        value = fightOutPokemonMaxHpWaruValue(battler, 0x10);
        wazaSetStatus(statusId, 0, 0x2D, 0,
                      (value & 0xFFFF) * (s32)stateValue);
        fn_8020248C(battler, 4, 0);
        if (fightOutPokemonIsUseHensinBuff(battler) == 1) {
            fightOutPokemonSetHensinPokemonStatusId(battler, 0x7C, 0, 0);
        }
        fn_80211B94(lbl_8047B62C, lbl_8037925F, 0);
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 6) == 1) {
        wazaSetStatus(statusId, 0, 0x2D, 0,
                      fightOutPokemonMaxHpWaruValue(battler, 8));
        fn_80211B94(lbl_8047B62C, lbl_80379287, 0);
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x17) == 1) {
        if (fn_802026E4(battler, 8) == 1) {
            wazaSetStatus(statusId, 0, 0x2D, 0,
                          fightOutPokemonMaxHpWaruValue(battler, 4));
            fn_80211B94(lbl_8047B62C, lbl_80379402, 0);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0x17);
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x18) == 1) {
        wazaSetStatus(statusId, 0, 0x2D, 0,
                      fightOutPokemonMaxHpWaruValue(battler, 4));
        fn_80211B94(lbl_8047B62C, lbl_8037941F, 0);
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0xE) == 1) {
        step = fn_80202108(battler, 0xE);
        value = fn_80201C58(battler, 0xE);
        wazaGetStatus(0, value, 1, 0);
        msgctrlSetValue(0xD, GSmsgGetGSchar());
        limit = fn_80202234(battler, 0xE);
        if (step < limit) {
            lbl_80379F58[0x160A4] = value;
            lbl_80379F58[0x160A5] = value >> 8;
            wazaSetStatus(statusId, 0, 0x2D, 0,
                          fightOutPokemonMaxHpWaruValue(battler, 0x10));
            fn_80201FDC(battler, 0xE, (s8)(step + 1));
            fn_80211B94(lbl_8047B62C, lbl_803793A5, 0);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0xE);
            fn_80211B94(lbl_8047B62C, lbl_803793C3, 0);
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0xB) == 1) {
        fightFloorLoopValidFightOutPokemon(0, fn_802316FC, 0, 0);
        fightFloorSetStatus(0, 0, 0x36, 0, battler);
        step = fn_80202108(battler, 0xB);
        limit = fn_80202234(battler, 0xB);
        if ((s8)(step + 1) < limit) {
            fn_80201FDC(battler, 0xB);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0xB);
        }
        if (fightOutPokemonCheckNoAttackFlag(battler) == 1 ||
            fn_802026E4(battler, 0xB) != 1) {
            fightOutPokemonInitJoutaiKeep(battler);
            lbl_80478D7D = 1;
        } else {
            fn_8020248C(battler, 0x22, 0);
            lbl_80478D7D = 0;
        }
        fn_80211B94(lbl_8047B62C, lbl_8037930A, 0);
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0xD) == 1) {
        step = fn_80202108(battler, 0xD);
        limit = fn_80202234(battler, 0xD);
        if ((s8)(step + 1) < limit) {
            fn_80201FDC(battler, 0xD);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0xD);
        }
        if (fightOutPokemonCheckNoAttackFlag(battler) == 1) {
            fightOutPokemonInitJoutaiKeep(battler);
        } else if (fn_802026E4(battler, 0xD) == 0 &&
                   fn_802026E4(battler, 0x22) == 1) {
            fightOutPokemonWriteJoutaiDataId(battler, 0x22);
            if (fn_802026E4(battler, 9) == 0) {
                savedPc = lbl_8047B610;
                savedMessage = *(u32 *)&lbl_80379F58[0x2001C];
                lbl_80478D7B = 0x47;
                *(u32 *)&lbl_80379F58[0x2001C] = (u32)lbl_80379BFE;
                fn_802249B8(1, 0);
                lbl_8047B610 = savedPc;
                *(u32 *)&lbl_80379F58[0x2001C] = savedMessage;
                if (fn_802026E4(battler, 9) == 1) {
                    fn_80211B94(lbl_8047B62C, lbl_8037931E, 0);
                }
            }
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x29) == 1) {
        step = fn_80202108(battler, 0x29);
        value = fn_80201C58(battler, 0x29);
        fn_80201FDC(battler, 0x29, (s8)(step + 1));
        if ((s8)pokemonSearchWazaDataId(pokemon, value) < 0) {
            fightOutPokemonWriteJoutaiDataId(battler, 0x29);
        } else if (fn_80202234(battler, 0x29) <= (s8)(step + 1)) {
            fightOutPokemonWriteJoutaiDataId(battler, 0x29);
            fn_80211B94(lbl_8047B62C, lbl_80378D2C, 0);
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x2A) == 1) {
        step = fn_80202108(battler, 0x2A);
        wantedMove = fn_80201C58(battler, 0x2A);
        i = fn_80201890(battler, 0x2A);
        stateValue = pokemonGetStatus(pokemon, 0, 0x7F, i);
        fn_80201FDC(battler, 0x2A, (s8)(step + 1));
        if (stateValue != wantedMove) {
            fightOutPokemonWriteJoutaiDataId(battler, 0x2A);
        } else if (fn_80202234(battler, 0x2A) <= (s8)(step + 1) ||
                   pokemonGetStatus(pokemon, 0, 0x80, i) == 0) {
            fightOutPokemonWriteJoutaiDataId(battler, 0x2A);
            fn_80211B94(lbl_8047B62C, lbl_80378D40, 0);
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x1D) == 1) {
        step = fn_80202108(battler, 0x1D);
        if ((s8)(step + 1) < fn_80202234(battler, 0x1D)) {
            fn_80201FDC(battler, 0x1D);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0x1D);
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x24) == 1) {
        step = fn_80202108(battler, 0x24);
        if ((s8)(step + 1) < fn_80202234(battler, 0x24)) {
            fn_80201FDC(battler, 0x24);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0x24);
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x30) == 1) {
        step = fn_80202108(battler, 0x30);
        if ((s8)(step + 1) < fn_80202234(battler, 0x30)) {
            fn_80201FDC(battler, 0x30);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0x30);
        }
    }
    PHASE_END();

    PHASE_BEGIN();
    if (fn_802026E4(battler, 0x26) == 1) {
        step = fn_80202108(battler, 0x26);
        if ((s8)(step + 1) < fn_80202234(battler, 0x26)) {
            fn_80201FDC(battler, 0x26);
        } else {
            fightOutPokemonWriteJoutaiDataId(battler, 0x26);
            if (fightOutPokemonIsJoutaiNormal(battler) == 1 && ability != 0x48 &&
                ability != 0x0F &&
                fightFloorLoopValidFightOutPokemon(0, 0x8021C638, battler, 1) == 0) {
                blocked = 0;
                value = fightFloorGetStatus(0, 0, 0x34, 0);
                limit = fn_80077B60();
                fn_80077B3C();
                if (value == 1 && limit != 1) {
                    u32 trainer = fightSideGetValidFightTrainerPtr(2, battler);
                    count = fightFloorGetStatus(0, 0, 0x16, 0);
                    fightFloorGetStatus(0, 0, 0x17, 0);
                    for (i = 0; i < count; i++) {
                        u32 fightPokemon = fightTrainerGetValidFightPokemonPtr(trainer, i);
                        if (fightPokemon != 0) {
                            for (j = 0; j < 6; j++) {
                                other = fightPokemonCheckFightOut(fightPokemon, j);
                                if (other != 0 && fightPokemonCheckFightOut(other) != 0 &&
                                    fightPokemonCheckWriteJoutaiDataId(other, 8) == 1) {
                                    blocked++;
                                }
                            }
                        }
                    }
                }
                if (blocked == 0 && fn_802025B8(battler, 8) == 2) {
                    fn_8020248C(battler, 8, 0);
                    fightOutPokemonInitJoutaiKeep(battler);
                    if (fightOutPokemonIsUseHensinBuff(battler) == 1) {
                        fightOutPokemonSetHensinPokemonStatusId(battler, 0x7C, 0, 0);
                    }
                    fightFloorSetStatus(0, 0, 0x47, 0, battler);
                    fn_80211B94(lbl_8047B62C, lbl_80379464, 0);
                }
            }
        }
    }
    PHASE_END();

    if (fightOutPokemonCheckFightOut(battler) != 0) {
        lbl_8047B618 |= 0x01000020;
        fn_8022EDEC(battler, 1);
        lbl_8047B618 &= 0xFEFFFFDF;
        fn_801DA7AC();
    }

#undef PHASE_BEGIN
#undef PHASE_END
    return 1;
}

void fn_80221104(u8 r3, u32 r4)

{
    extern u8 lbl_80379F58[];
    extern u8 lbl_80378964[];
    extern void _threadSwitch();
    extern void battleGridUpdate();
    extern void battleGridReplacePokemon();
    extern void fn_801DA8C4();
    extern u8 fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB100();
    extern u8 fn_801DDD28();
    extern u32 fightTargetDataBiosGetBuff();
    extern void fightTargetDataBiosGetPtr();
    extern u32 fightTargetGetPtrAsNowFightType();
    extern u32 fightFloorGetStatus();
    extern u8 fightFloorGetNowTenkouDataId();
    extern u16 fightOutPokemonGetUseWazaDataId();
    extern u32 fightOutPokemonCreateSequence();
    extern void fightTargetDataBiosGetPtr();
    extern u32 fightTargetDataBiosGetBuff();
    extern void fightOutPokemonLoadWazaEffect();
    extern void fightOutPokemonWazaEffect();
    extern void fightWazaWzxTypeFuncMigawari();
    extern void battleGridUpdate();
    extern void pokemonSetStatus();
    extern u32 fn_80201C58();
    extern u32 fightOutPokemonCreateSequence();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    extern void fightWazaWzxTypeFuncMigawari();
    extern void fn_80211B94();
    extern u32 fn_80222110();
    extern void fn_80265598();
    extern u8 lbl_80478D78[8];
    extern u32 lbl_8047B618;
    extern u32 lbl_8047B62C;
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u16 uVar6;
  u32 uVar5;
  u8 cVar9;
  u16 uVar7;
  u16 sVar8;
  s32 bVar10;

  u32 uVar11;

  uVar6 = fightFloorGetStatus(0,0,0x14,0);
  uVar2 = fightTargetGetPtrAsNowFightType(r3,0);
  uVar3 = fightTargetGetPtrAsNowFightType(0x11,0);
  uVar4 = fightTargetGetPtrAsNowFightType(0x12,0);
  cVar9 = lbl_80379F58[0x160a4] - ((lbl_80379F58[0x1601e] & 0xf) - 1);
  if (((r4 == 0x11) || (r4 - 1U < 2)) || (r4 == 0x1a)) {
    if (r4 == 1) {
      if (lbl_80478D78 != 0) {
        if (lbl_80478D78[3] != 0) {
          switch (lbl_80478D78[3]) {
        case 0x0F:
        case 0x12:
          uVar4 = 0x26;
          break;
        case 0x10:
        case 0x13:
        case 0x15:
          uVar4 = 0x28;
          break;
        case 0x11:
          uVar4 = 0x2A;
          break;
        case 0x14:
          uVar4 = 0x2C;
          break;
        case 0x16:
        case 0x19:
          uVar4 = 0x27;
          break;
        case 0x17:
        case 0x1A:
        case 0x1C:
          uVar4 = 0x29;
          break;
        case 0x18:
          uVar4 = 0x2B;
          break;
        case 0x1B:
          uVar4 = 0x3C;
          break;
          default:
            uVar4 = fn_80222110(cVar9);
            break;
          }
        } else {
          uVar4 = fn_80222110(cVar9);
        }
      } else {
        uVar4 = fn_80222110(cVar9);
      }
      uVar6 = fightFloorGetStatus(0,0,0x14,0);
      uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
      if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,uVar4,4,0), cVar9 != 0)) {
        fn_801DA9E8(uVar5,uVar4,4);
        fn_80265598(uVar2,uVar6,1);
      }
    }
    if (r4 == 0x1a) {
      uVar6 = fightFloorGetStatus(0,0,0x14,0);
      uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
      if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x2d,4,0), cVar9 != 0)) {
        fn_801DA9E8(uVar5,0x2d,4);
        fn_80265598(uVar2,uVar6,1);
      }
    }
    if (r4 == 0x11) {
      fightOutPokemonLoadWazaEffect(uVar2,0x121,1,0);
      fightOutPokemonLoadWazaEffect(uVar3,0x121,2,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar4 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0x121,1,1,uVar4);
      fightTargetDataBiosGetPtr(0x12);
      uVar4 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar3,0x121,2,0,uVar4);
    }
    if (r4 != 2) {
      return;
    }
    fightWazaWzxTypeFuncMigawari(0xa4,uVar2,uVar2,0,0);
    return;
  }
  if ((lbl_8047B618 & 0x80) != 0) {
    fn_80211B94(lbl_8047B62C,(u32)lbl_80378964,0);
    return;
  }
  if (((r4 - 10U < 4) || (r4 - 0x20U < 3)) || (r4 == 0x1e)) {
    if (r4 == 10) {
      fightOutPokemonLoadWazaEffect(uVar2,0xf0,1,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0xf0,1,0,uVar3);
    }
    if (r4 == 0xb) {
      fightOutPokemonLoadWazaEffect(uVar2,0xf1,1,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0xf1,1,0,uVar3);
    }
    if (r4 == 0xc) {
      fightOutPokemonLoadWazaEffect(uVar2,0xc9,2,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0xc9,2,0,uVar3);
    }
    if (r4 == 0xd) {
      fightOutPokemonLoadWazaEffect(uVar2,0x102,2,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0x102,2,0,uVar3);
    }
    if (r4 == 0x20) {
      fightOutPokemonLoadWazaEffect(uVar2,0xf0,1,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0xf0,1,0,uVar3);
    }
    if (r4 == 0x22) {
      fightOutPokemonLoadWazaEffect(uVar2,0xc9,1,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0xc9,1,0,uVar3);
    }
    if (r4 == 0x21) {
      fightOutPokemonLoadWazaEffect(uVar2,0xf1,1,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0xf1,1,0,uVar3);
    }
    if (r4 != 0x1e) {
      return;
    }
    cVar9 = (int)fightFloorGetNowTenkouDataId(0,0);
    if (cVar9 == 2) {
      fightFloorGetStatus(0,0,0x14,0);
      uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
      if (uVar5 == 0) {
        return;
      }
      cVar9 = fn_801DDD28(uVar5,0x80,4,0);
      if (cVar9 == 0) {
        return;
      }
      fn_801DA9E8(uVar5,0x80,4);
      return;
    }
    cVar9 = (int)fightFloorGetNowTenkouDataId(0,0);
    if (cVar9 == 1) {
      fightFloorGetStatus(0,0,0x14,0);
      uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
      if (uVar5 == 0) {
        return;
      }
      cVar9 = fn_801DDD28(uVar5,0x7f,4,0);
      if (cVar9 == 0) {
        return;
      }
      fn_801DA9E8(uVar5,0x7f,4);
      return;
    }
    cVar9 = (int)fightFloorGetNowTenkouDataId(0,0);
    if (cVar9 == 3) {
      fightFloorGetStatus(0,0,0x14,0);
      uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
      if (uVar5 == 0) {
        return;
      }
      cVar9 = fn_801DDD28(uVar5,0x7e,4,0);
      if (cVar9 == 0) {
        return;
      }
      fn_801DA9E8(uVar5,0x7e,4);
      return;
    }
    cVar9 = (int)fightFloorGetNowTenkouDataId(0,0);
    if (cVar9 != 4) {
      fightFloorGetStatus(0,0,0x14,0);
      uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
      if (uVar5 == 0) {
        return;
      }
      cVar9 = fn_801DDD28(uVar5,0x9d,4,0);
      if (cVar9 == 0) {
        return;
      }
      fn_801DA9E8(uVar5,0x9d,4);
      return;
    }
    fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if (uVar5 == 0) {
      return;
    }
    cVar9 = fn_801DDD28(uVar5,0x7d,4,0);
    if (cVar9 == 0) {
      return;
    }
    fn_801DA9E8(uVar5,0x7d,4);
    return;
  }
  if (r4 == 0x17) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x57,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x57,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 0x1f) {
    fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x9d,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x9d,4);
    }
  }
  if (r4 == 0x18) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x58,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x58,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 0x23) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0xa3,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0xa3,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 0x24) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0xd8,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0xd8,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 0x19) {
    fightOutPokemonLoadWazaEffect(uVar2,1,2,0);
    fightTargetDataBiosGetPtr(0x12);
    uVar3 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar2,1,2,0,uVar3);
  }
  if (r4 == 0xe) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x38,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x38,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 0x16) {
    fightOutPokemonLoadWazaEffect(uVar2,0x111,3,0);
    fightTargetDataBiosGetPtr(0x11);
    uVar3 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar2,0x111,3,0,uVar3);
  }
  if (r4 == 0x12) {
    fightOutPokemonLoadWazaEffect(uVar2,0xf8,2,0);
    fightTargetDataBiosGetPtr(0x12);
    uVar3 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar2,0xf8,2,0,uVar3);
  }
  if (r4 == 0x13) {
    fightOutPokemonLoadWazaEffect(uVar4,0x161,2,0);
    fightTargetDataBiosGetPtr(0x12);
    uVar3 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar4,0x161,2,0,uVar3);
  }
  if (r4 == 0x15) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x3b,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x3b,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 7) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x39,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x39,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 0x1b) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x32,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x32,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 0x1c) {
    fightOutPokemonLoadWazaEffect(uVar2,0x36,3,0);
    fightTargetDataBiosGetPtr(0x11);
    uVar3 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar2,0x36,3,0,uVar3);
  }
  if (r4 == 0x1d) {
    fightOutPokemonLoadWazaEffect(uVar2,0x74,3,0);
    fightTargetDataBiosGetPtr(0x11);
    uVar3 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar2,0x74,3,0,uVar3);
  }
  if (r4 == 9) {
    uVar7 = fightFloorGetStatus(0,0,0x14,0);
    uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
    if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x86,4,0), cVar9 != 0)) {
      fn_801DA9E8(uVar5,0x86,4);
      fn_80265598(uVar2,uVar7,1);
    }
  }
  if (r4 == 6) {
    uVar3 = fn_80201C58(uVar2,0xe);
    fightOutPokemonLoadWazaEffect(uVar2,uVar3,2,0);
    fightTargetDataBiosGetPtr(0x12);
    uVar4 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar2,uVar3,2,0,uVar4);
  }
  if (r4 == 0x10) {
    sVar8 = fightOutPokemonGetUseWazaDataId(uVar2);
    if (sVar8 == 0x157) {
      fightOutPokemonLoadWazaEffect(uVar2,0x157,3,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0x157,3,0,uVar3);
    }
    sVar8 = fightOutPokemonGetUseWazaDataId(uVar2);
    if (sVar8 == 0xa8) {
      fightOutPokemonLoadWazaEffect(uVar2,0xa8,3,0);
      fightTargetDataBiosGetPtr(0x11);
      uVar3 = fightTargetDataBiosGetBuff();
      fightOutPokemonWazaEffect(uVar2,0xa8,3,0,uVar3);
    }
  }
  if (r4 != 0) goto LAB_0021f0bc;
  bVar10 = (int)fightFloorGetNowTenkouDataId(0,1);
  uVar3 = (int)pokemonGetStatus(uVar2,0,0xee,0);
  uVar4 = fightOutPokemonCreateSequence(uVar2,bVar10);
  switch (bVar10) {
  case 1:
    uVar11 = 0x84;
    break;
  case 2:
    uVar11 = 0x85;
    break;
  case 4:
    uVar11 = 0x82;
    break;
  default:
    uVar11 = 0x83;
    break;
  }
  fn_801DDD28(uVar3,0x81,4,0);
  fn_801DDD28(uVar4,uVar11,4,0);
  fn_80265598(uVar2,uVar6,1);
  fightFloorGetStatus(0,0,0x14,0);
  uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
  if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,0x81,4,0), cVar9 != 0)) {
    fn_801DA9E8(uVar5,0x81,4);
    for (;;) {
      if (fn_801DA94C(uVar5,0x81,4) == 0) {
        break;
      }
      _threadSwitch();
    }
    fn_801DA8C4(uVar5,0x81,4);
  }
  battleGridReplacePokemon(uVar3,uVar4);
  battleGridUpdate();
  pokemonSetStatus(uVar2,0,0xee,0,uVar4);
  fightFloorGetStatus(0,0,0x14,0);
  uVar5 = (int)pokemonGetStatus(uVar2,0,0xee,0);
  if ((uVar5 != 0) && (cVar9 = fn_801DDD28(uVar5,uVar11,4,0), cVar9 != 0)) {
    fn_801DA9E8(uVar5,uVar11,4);
  }
  fn_801DB100(uVar3);
LAB_0021f0bc:
  if (r4 == 0x14) {
    fightOutPokemonLoadWazaEffect(uVar2,0x75,3,0);
    fightTargetDataBiosGetPtr(0x11);
    uVar3 = fightTargetDataBiosGetBuff();
    fightOutPokemonWazaEffect(uVar2,0x75,3,0,uVar3);
  }
  return;
}
