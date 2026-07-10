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

/* fn_80222500 (0x80222500): advance FightSeq PC by 2. */
void fn_80222500(void) { lbl_8047B610 = lbl_8047B610 + 2; }

/* fn_80222510 (0x80222510): advance FightSeq PC by 1. */
void fn_80222510(void) { lbl_8047B610 = lbl_8047B610 + 1; }

/* fn_80222554 (0x80222554): AND-NOT a u32 field with a script-embedded
 * mask -- REJECTED, could not reach 100% (best attempt 76.25%, MWCC
 * emits xor+and instead of andc for this shape under -O4). Left
 * unimplemented (still asm-only) rather than landed at <100%. */

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

/* fn_80222ACC (0x80222ACC): read the current FightSeq PC, then advance
 * it to the script-embedded jump target. */
u32 fn_80222ACC(void) { u32 r = lbl_8047B610; lbl_8047B610 = *(u32 *)(r + 1); return r; }

/* fn_8023C368 (0x8023C368): constant-zero accessor. */
u32 fn_8023C368(void) { return 0; }
