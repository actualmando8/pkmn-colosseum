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
 * fn_801F025C(slot, base) resolves a FightSeq context/ctx pointer.
 * fn_802025B8/fn_8020248C/fn_802026E4 are the GetEventState/
 * SetEventState/CheckEventFlag dispatchers already matched 100% in
 * colosseum_event.c; despite being declared void there, real callers
 * (including that file's own fn_80211170) consume their r3 return
 * value, so they are re-declared here with the return type each call
 * site actually uses -- same K&R untyped-extern idiom already used
 * throughout colosseum_event.c for cross-TU calls.
 */
extern u32 fn_801F025C();
extern u8  fn_802025B8();
extern void fn_8020248C();
extern u8  fn_802026E4();
extern u32 lbl_8047B610;
extern u32 lbl_8047B618;

/*
 * fn_80215300 (0x80215300)
 *
 * FightSeq opcode handler: if event-state 0x28 on the ctx from slot
 * 0x11 reads 2, clears it back to 0 and advances the PC past this
 * instruction (+5); otherwise takes the script-embedded jump.
 */
void fn_80215300(void) {
    u32 ctx = fn_801F025C(0x11, 0);

    if (fn_802025B8(ctx, 0x28) == 2) {
        fn_8020248C(ctx, 0x28, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    } else {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    }
}

/* fn_802161F0 (0x802161F0): same FightSeq opcode-handler shape as
 * fn_80215300, slot 0x12 / event-state id 0x30. */
void fn_802161F0(void) {
    u32 ctx = fn_801F025C(0x12, 0);

    if (fn_802025B8(ctx, 0x30) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx, 0x30, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}

/* fn_802162F0 (0x802162F0): same shape, slot 0x12 / event-state id 0x1b. */
void fn_802162F0(void) {
    u32 ctx = fn_801F025C(0x12, 0);

    if (fn_802025B8(ctx, 0x1b) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx, 0x1b, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}

/* fn_80215A78 (0x80215A78): same shape, slot 0x11 / event-state id 0x25. */
void fn_80215A78(void) {
    u32 ctx = fn_801F025C(0x11, 0);

    if (fn_802025B8(ctx, 0x25) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx, 0x25, 0);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}

/* fn_802158D0 (0x802158D0): variant shape -- both state==2 AND
 * fn_80203CCC(ctx) must hold to take the SetEventState+advance path;
 * otherwise the script-embedded jump (deref) is taken. */
extern u8 fn_80203CCC();
void fn_802158D0(void) {
    u32 ctx = fn_801F025C(0x12, 0);

    if (fn_802025B8(ctx, 0x26) != 2) {
        goto deref;
    }
    if (fn_80203CCC(ctx) != 0) {
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
 * fn_80216804 (0x80216804)
 *
 * Gated variant: the SetEventState side effect only runs when flag bit
 * 0x2000000 of lbl_8047B618 is set; the PC always just advances by 1
 * (this handler has no script-embedded jump operand).
 */
void fn_80216804(void) {
    u32 ctx = fn_801F025C(0x11, 0);

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
 * side's, records the (positive) delta via fn_8011BBD8 and advances
 * the PC by 5; otherwise takes the script-embedded jump.
 */
extern u32 fn_8012640C();
extern u32 fn_80205B8C();
extern void fn_8011BBD8();
#pragma optimize_for_size on
void fn_80215808(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 fieldD9 = fn_8012640C(ctx1, 0, 0xD9, 0);
    u32 poke1 = fn_80205B8C(ctx1);
    s32 statA = (u16)fn_8012640C(poke1, 0, 0x83, 0);

    u32 ctx2 = fn_801F025C(0x12, 0);
    u32 poke2 = fn_80205B8C(ctx2);
    s32 statB = (u16)fn_8012640C(poke2, 0, 0x83, 0);

    if (statB <= statA) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8011BBD8(fieldD9, 0, 0x2d, 0, statB - statA);
        lbl_8047B610 = lbl_8047B610 + 5;
    }
}
#pragma optimize_for_size reset

/*
 * fn_80216874 (0x80216874)
 *
 * FightSeq opcode handler dispatching on the current move-effect id
 * (fn_80205184): move ids 0x13/0x154 share flag 0x1f, 0x5b uses flag
 * 0x20, and 0x123 uses flag 0x21 -- each checked via CheckEventFlag
 * and, if set, applied via fn_80202810. Any other move id is a no-op.
 * The PC always just advances by 1 (no script-embedded jump operand).
 */
extern u16 fn_80205184();
extern void fn_80202810();
void fn_80216874(void) {
    u32 ctx = fn_801F025C(0x11, 0);
    u16 moveId = fn_80205184(ctx);

    switch (moveId) {
    case 0x13:
    case 0x154:
        if (fn_802026E4(ctx, 0x1f) == 1) {
            fn_80202810(ctx, 0x1f);
        }
        break;
    case 0x5b:
        if (fn_802026E4(ctx, 0x20) == 1) {
            fn_80202810(ctx, 0x20);
        }
        break;
    case 0x123:
        if (fn_802026E4(ctx, 0x21) == 1) {
            fn_80202810(ctx, 0x21);
        }
        break;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_80214DB0 (0x80214DB0)
 *
 * Marks field 0x118 of the slot-0x11 FightPokemon as changed
 * (fn_801254B4, mode 1), then: if fn_801F221C(0) reports 1, or
 * event-state 0x33 isn't 2, take the script-embedded jump; otherwise
 * clear event-state 0x33 and advance the PC by 5.
 */
extern u32 fn_801254B4();
extern u8  fn_801F221C();
void fn_80214DB0(void) {
    u32 ctx = fn_801F025C(0x11, 0);
    fn_801254B4(ctx, 0, 0x118, 0, 1);

    if (fn_801F221C(0) != 1) {
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
 * slot 0x43 via fn_801F4C14, and gates on event-state 0x37 instead of
 * 0x33.
 */
extern void fn_801F4C14();
void fn_80214E50(void) {
    u32 ctx = fn_801F025C(0x11, 0);
    fn_801F4C14(0, 0, 0x43, 0, ctx);
    fn_801254B4(ctx, 0, 0x118, 0, 1);

    if (fn_801F221C(0) != 1) {
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
 * fn_80216960 (0x80216960)
 *
 * Same move-id dispatch table as fn_80216874 (0x13/0x154 -> flag
 * 0x1f, 0x5b -> flag 0x20, 0x123 -> flag 0x21), but using the
 * GetEventState==2 / SetEventState(...,0) idiom instead of
 * CheckEventFlag/fn_80202810. PC always advances by 1.
 */
void fn_80216960(void) {
    u32 ctx = fn_801F025C(0x11, 0);
    u16 moveId = fn_80205184(ctx);

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
 * fn_80120B00 and records them into fields 0x2f/0x30 of the slot's
 * field-0xD9 object. PC always advances by 1.
 */
extern void fn_80120B00();
void fn_80217018(void) {
    u32 ctx = fn_801F025C(0x11, 0);
    u32 fieldD9 = fn_8012640C(ctx, 0, 0xD9, 0);
    u32 poke = fn_80205B8C(ctx);
    u16 outA, outB;

    fn_80120B00(poke, &outA, &outB);
    fn_8011BBD8(fieldD9, 0, 0x2f, 0, outA);
    fn_8011BBD8(fieldD9, 0, 0x30, 0, outB);
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_802165B4 (0x802165B4)
 *
 * If field 0x4d of the slot-2-relative-to-slot-0x11 context reads 2,
 * pulls a value from fn_801F54A4(...,0x14,...) through fn_801F0134
 * and writes it back into field 0x4d. PC always advances by 1.
 */
extern u8  fn_801F6E44();
extern u32 fn_801F54A4();
extern u32 fn_801F0134();
extern void fn_801F6DF0();
#pragma optimize_for_size on
void fn_802165B4(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 tmp = fn_801F025C(2, ctx1);

    if (fn_801F6E44(tmp, 0x4d) == 2) {
        u16 val = (u16)fn_801F54A4(0, 0, 0x14, 0);
        u32 val2 = fn_801F0134(ctx1, val);
        fn_801F6DF0(tmp, 0x4d, val2);
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}
#pragma optimize_for_size reset

/*
 * fn_802178F4 (0x802178F4)
 *
 * If field 0x4b of the slot-2-relative-to-slot-0x11 context reads 2,
 * clears it and marks lbl_80478D78[5]=5; otherwise stashes 0x40 into
 * slot 0x3b (fn_801F4C14) and marks lbl_80478D78[5]=0. PC always
 * advances by 1.
 */
extern u8 lbl_80478D78[8];
void fn_802178F4(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 tmp = fn_801F025C(2, ctx1);

    if (fn_801F6E44(tmp, 0x4b) != 2) {
        fn_801F4C14(0, 0, 0x3b, 0, 0x40);
        lbl_80478D78[5] = 0;
    } else {
        fn_801F6DF0(tmp, 0x4b, 0);
        lbl_80478D78[5] = 5;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_802182D4 (0x802182D4)
 *
 * If field 0x4a of the slot-3-relative-to-slot-0x11 context reads 2,
 * compare its recorded value (fn_801F6D9C if set, else 0) against
 * fn_80119DD0(0x4a); a mismatch clears the field and advances the PC
 * by 5. Otherwise (including the initial field != 2 case) marks field
 * 0x118 of slot-0x11 changed and takes the script-embedded jump.
 */
extern u8  fn_801F6E98();
extern s16 fn_801F6D9C();
extern u8  fn_80119DD0();
#pragma optimize_for_size on
void fn_802182D4(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 tmp = fn_801F025C(3, ctx1);
    s16 val;

    if (fn_801F6E44(tmp, 0x4a) != 2) {
        goto matched;
    }
    if (fn_801F6E98(tmp, 0x4a) == 1) {
        val = fn_801F6D9C(tmp, 0x4a);
    } else {
        val = 0;
    }
    if (val != fn_80119DD0(0x4a)) {
        goto notmatched;
    }
matched:
    fn_801254B4(ctx1, 0, 0x118, 0, 1);
    lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    return;
notmatched:
    fn_801F6DF0(tmp, 0x4a, 0);
    lbl_8047B610 = lbl_8047B610 + 5;
}
#pragma optimize_for_size reset

/*
 * fn_802183BC (0x802183BC)
 *
 * If event-state 0x18 on the slot-0x12 ctx is 2, takes the
 * script-embedded jump; otherwise clears it, records the slot-0x11
 * side's fn_80203B5C(...,2) value into field 0x2d of the field-0xD9
 * object, and advances the PC by 5.
 */
extern u16 fn_80203B5C();
#pragma optimize_for_size on
void fn_802183BC(void) {
    u32 ctx1;
    u32 ctx2;
    u32 fieldD9;

    ctx1 = fn_801F025C(0x11, 0);
    fieldD9 = fn_8012640C(ctx1, 0, 0xD9, 0);
    ctx2 = fn_801F025C(0x12, 0);

    if (fn_802025B8(ctx2, 0x18) != 2) {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    } else {
        fn_8020248C(ctx2, 0x18, 0);
        fn_8011BBD8(fieldD9, 0, 0x2d, 0, fn_80203B5C(ctx1, 2));
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
void fn_80218BD4(void) {
    u32 sub1;
    u32 ctx2;
    u32 sub2;

    sub1 = fn_801F025C(2, fn_801F025C(0x11, 0));
    ctx2 = fn_801F025C(0x12, 0);
    sub2 = fn_801F025C(2, ctx2);

    if (fn_802026E4(ctx2, 0x15) == 1 && sub1 != sub2 && (lbl_8047B618 & 0x1000000) == 0) {
        lbl_8047B618 = lbl_8047B618 | 0x40;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}
#pragma optimize_for_size reset

/*
 * fn_80219D98 (0x80219D98)
 *
 * Records fn_80203E0C(ctx) into field 0x2d of the slot-0x11 side's
 * field-0xD9 object. PC always advances by 1.
 */
extern u8 fn_80203E0C();
void fn_80219D98(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 fieldD9 = fn_8012640C(ctx1, 0, 0xD9, 0);

    fn_8011BBD8(fieldD9, 0, 0x2d, 0, fn_80203E0C(ctx1));
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
    u32 ctx1 = fn_801F025C(0x11, 0);

    if (fn_802025B8(ctx1, 0xf) != 2) {
        fn_801F4C14(0, 0, 0x3b, 0, 0x45);
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
 * slot-2-relative sub-context (field 0x4c via fn_801F6E44 /
 * fn_801F6DF0).
 */
void fn_8021A764(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 tmp = fn_801F025C(2, ctx1);

    if (fn_801F6E44(tmp, 0x4c) != 2) {
        fn_801F4C14(0, 0, 0x3b, 0, 0x45);
        lbl_80478D78[5] = 1;
    } else {
        fn_801F6DF0(tmp, 0x4c, 0);
        lbl_80478D78[5] = 0;
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

/*
 * fn_8021AB9C (0x8021AB9C)
 *
 * Records fn_80203ADC(ctx2, 2) into field 0x2d of the slot-0x11
 * side's field-0xD9 object. PC always advances by 1.
 */
extern u16 fn_80203ADC();
void fn_8021AB9C(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 fieldD9 = fn_8012640C(ctx1, 0, 0xD9, 0);
    u32 ctx2 = fn_801F025C(0x12, 0);

    fn_8011BBD8(fieldD9, 0, 0x2d, 0, fn_80203ADC(ctx2, 2));
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
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 fieldD9 = fn_8012640C(ctx1, 0, 0xD9, 0);
    u8 flag = *(u8*)(lbl_8047B610 + 1);
    u32 val;

    if (flag != 0) {
        fn_8011BBD8(fieldD9, 0, 0x31, 0, flag);
    } else {
        val = fn_800E0C54() % 5;
        if (val < 2) {
            val = val + 2;
        } else {
            val = fn_800E0C54() % 5 + 2;
        }
        fn_8011BBD8(fieldD9, 0, 0x31, 0, val);
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
extern s32 fn_8011BEB4();
#pragma optimize_for_size on
void fn_8021C0F4(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 fieldD9 = fn_8012640C(ctx1, 0, 0xD9, 0);
    s32 half;
    s32 val;

    fn_8011BEB4(fieldD9, 0, 0x2d, 0);
    half = fn_8011BEB4(fieldD9, 0, 0x2e, 0) / 2;
    val = -half;
    if (val == 0) {
        val = -1;
    }
    fn_8011BBD8(fieldD9, 0, 0x2d, 0, val);
    lbl_8047B610 = lbl_8047B610 + 1;
}
#pragma optimize_for_size reset

/*
 * fn_8021D010 (0x8021D010)
 *
 * Clears field 0x83 of the slot-0x11 side's active Pokemon, then if
 * fn_801FECD4 reports 1, re-derives it via fn_801FE7EC. PC always
 * advances by 1.
 */
extern u8 fn_801FECD4();
extern void fn_801FE7EC();
void fn_8021D010(void) {
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 poke = fn_80205B8C(ctx1);

    fn_801254B4(poke, 0, 0x83, 0, 0);
    if (fn_801FECD4(ctx1) == 1) {
        fn_801FE7EC(ctx1, 0x83, 0, 0);
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
    u32 ctx1 = fn_801F025C(0x11, 0);
    u32 fieldD9 = fn_8012640C(ctx1, 0, 0xD9, 0);
    s16 val = (long long)(((s16)((u8)fn_8011BEB4(fieldD9, 0, 0x31, 0))) - 1);

    if (val < 0) {
        val = 0;
    }
    if (val == 0) {
        ctx1 = ctx1;
        lbl_8047B610 = lbl_8047B610 + 5;
    } else {
        lbl_8047B610 = *(u32*)(lbl_8047B610 + 1);
    }
    fn_8011BBD8(fieldD9, 0, 0x31, 0, val);
    ctx1 = ctx1;
}
#pragma optimize_for_size reset

/*
 * fn_80226730 (0x80226730)
 *
 * If field 0x2b of the field-0xD9 object is 2 and the slot-0x11 side
 * is shadow (fn_802096E8), stashes 0x7631 into slot 0x52 and, if
 * fn_802624CC(0x7631) reports 1, marks lbl_80478D78[7]=1. PC always
 * advances by 1.
 */
extern u8 fn_802096E8();
extern u8 fn_802624CC();
void fn_80226730(void) {
    extern u8 fn_8011BEB4();
    u32 fieldD9 = fn_8012640C(fn_801F025C(0x11, 0), 0, 0xD9, 0);

    if (fn_8011BEB4(fieldD9, 0, 0x2b, 0) == 2 && fn_802096E8(fieldD9) == 1) {
        fn_801F4C14(0, 0, 0x52, 0, 0x7631);
        if (fn_802624CC(0x7631) == 1) {
            lbl_80478D78[7] = 1;
        }
    }
    lbl_8047B610 = lbl_8047B610 + 1;
}

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
