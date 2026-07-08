/**
 * @file fight_trainer_ai_waza_hit.c
 * @brief game/pxdvs/app/fight/fightTrainerAiWazaHit.cpp -- split from colosseum_battle.c (the
 *        Colosseum battle-flow/AI bucket, 0x802405C0-0x80265EC4),
 *        address range 0x80253950-0x8025C5A4, 200 fns.
 *
 * XD source unit: game/pxdvs/app/fight/fightTrainerAiWazaHit.cpp
 * Physically split out of the pre/post-battle mega-file by address
 * (functions located and bucketed by name via config/GC6E01/symbols.txt,
 * since this TU uses plain named C bodies with no address-comment
 * markers).
 */

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

/* =========================================================================
 * Duplicated declarations (verbatim from the original colosseum_battle.c
 * preamble, present in every split segment so each TU keeps the same
 * external visibility it had before the split)
 * ========================================================================= */
extern void* pokemonGetStatus();
extern u32   pokemonSetStatus();

/* Battle system functions */
extern void fn_801EF8F4();

/* Sound functions */
extern void soundStop();     /* Stop sound */
extern void fn_80165A20();     /* Fade out music */
extern void fn_801659FC();     /* Start BGM */

/* SDA2 float constants used by asm wrappers */
extern f32 lbl_8047E678;
extern f32 lbl_8047E67C;

/* SDA1 globals used by asm wrappers */
extern u32 lbl_8047B668;
extern u32 lbl_8047B66C;
extern u32 lbl_8047B670;

/* Data labels used by asm wrappers */
extern u8  lbl_8039A6B8[];
extern u8  lbl_8039A6A8[];
extern int lbl_804782BC[];
extern u8  lbl_804782E0[];
extern u8  lbl_804783E0[];

/* Forward declarations for functions used as addresses in asm wrappers */
void ShortCommandProc(int r3);
void ReadProc(int r3);
void WriteProc(int r3);
void __GBASyncCallback(int r3);
u32  __GBASync(int r3);
u32  __GBATransfer(int r3, u32 r4, u32 r5, u32 r6);

/* Forward declarations for asm wrapper bl targets (use () form for compat) */
extern void DSPInit();
extern void fn_800E01F4();
extern int  _fadeEffectGetRandom__FUl();
extern u32  fn_8011F520();
extern u32  fn_8011F5B0();
extern u16  fn_8011F5C8();
extern u32  savedataGetStatus();
extern int  fadeCheck();
extern int  fadeSet();
extern int  fn_801DAC90();
extern int  fn_801DADC0();
extern void OSRegisterResetFunction();
extern void OSInitAlarm();
extern void OSInitThreadQueue();
extern void* memcpy();

/* Forward declarations for converted functions */
u32 evolutionWazaLearn();
u32 evolutionWazaLearn();
int fightTrainerAiWazaValueKuroikiri(void* ctx, u32 param1, u32 param2, u32 param3);
void fightTrainerAiWazaValueHimitunotikara(void* ctx, u32 param1, u32 param2, u32 param3);
s32 fightTrainerAiSelectIrekaeDasuFightPokemon(void* ctx, u32 param1, u32 param2, u32 param3);
u32 fightTrainerAiWazaHit045(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType);
u32 fightMenuFightTrainerGcHeroOpenMenu(void* ctx, u32 param1, u32 param2);

/* Note: fightTrainerAiWazaHit046 (Shadow Pokemon check) and fightTrainerAiWazaHit047 (battle
 * condition/flag check via fn_80236BFC) are implemented and matched at
 * 100% further below in this file (Ghidra-import block), not here. */

/* =========================================================================
 * fightTrainerAiWazaHit045 - ProcessBattleResult
 *
 * Medium helper (0xB0 bytes) that processes the result of a battle.
 * Calls BattleSequenceCheck and BattleResultCheck to determine
 * the outcome, then calls fn_80236C80 to update state.
 *
 * @param trainerCtx    Trainer context
 * @param trainerSlot   Slot index
 * @param resultSlot    Result query slot
 * @param resultType    Type of result to process
 * ========================================================================= */
/* fightTrainerAiWazaHit045 | size: 0xB0 */
u32 fightTrainerAiWazaHit045(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType) {
    extern u16 fn_8023793C();
    extern u32 fn_80239500();
    extern u32 fn_802395C8();
    extern u32 _fightTrainerAiWazaHitCheck();
    s32 resultVal;
    u32 statusVal;
    statusVal = fn_802395C8(trainerCtx, resultSlot, trainerSlot);
    resultVal = _fightTrainerAiWazaHitCheck(trainerCtx, trainerSlot, resultSlot, resultType, 0);
    if (fn_8023793C(trainerCtx, resultType, statusVal, fn_80239500(trainerCtx, resultSlot)) == 0x43) {
        resultVal = 0;
    }
    if (resultVal == 0) {
        return 0;
    }
    if (resultVal == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80254678 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit195(void) { return 1; }

/* Address: 0x80254E2C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit183(void) { return 1; }

/* Address: 0x80255218 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit174(void) { return 1; }

/* Address: 0x802552C8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit172(void) { return 1; }

/* Address: 0x80255EE4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit153(void) { return 0; }

/* Address: 0x802564C0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit143(void) { return 1; }

/* Address: 0x8025746C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit116(void) { return 1; }

/* Address: 0x802575C0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit111(void) { return 1; }

/* Address: 0x80258134 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit093(void) { return 1; }

/* Address: 0x802586F4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit085(void) { return 1; }

/* Address: 0x802587B8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit083(void) { return 1; }

/* Address: 0x8025B11C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit025(void) { return 1; }

/* Address: 0x8025C25C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHitNull(void) { return 1; }

/* Address: 0x80253950 | Size: 0x6C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit213(void* ctx, u32 slot, u32 param) {
    extern u32 fn_80136428(u32);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u8 fn_80237DBC(void*, u32, u32);
    u32 value;
    u32 result;

    value = fn_80136428(fn_801F54A4(0, 0, 0xf, 0) & 0xFFFF);
    result = fn_80237DBC(ctx, slot, value & 0xFF);
    result = result != 1;
    return result;
}

/* Address: 0x802539BC | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit212(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235910(void*, u32);
    extern u32 fn_80235AA0(void);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_80235AA0();
    secondStatus = fn_80235910(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 1, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 3, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253A80 | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit211(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235974(void*, u32);
    extern u32 fn_802359D8(void*, u32);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_802359D8(ctx, param1);
    secondStatus = fn_80235974(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 4, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 5, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253B44 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit210(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x39);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x80253B78 | Size: 0xB4 */
s32 fightTrainerAiWazaHit209(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80253C2C | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit208(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235A3C(void*, u32);
    extern u32 fn_80235AA0(void*, u32);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_80235AA0(ctx, param1);
    secondStatus = fn_80235A3C(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 1, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 2, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253CF0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit207(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80253DA4 | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit206(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235974(void*, u32);
    extern u32 fn_80235A3C(void*, u32);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_80235A3C(ctx, param1);
    secondStatus = fn_80235974(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 2, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 5, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253E68 | Size: 0xC0 */
s32 fightTrainerAiWazaHit205(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235AA0(void* ctx, u32 elem);
    extern u8 fn_80235A3C(void* ctx, u32 elem);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u8 a = fn_80235AA0(ctx, param3);
    u8 b = fn_80235A3C(ctx, param3);

    if (a == 0 && b == 0) {
        return 0;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 1, 1) != 0) {
        goto ret1;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 2, 1) != 0) {
        goto ret1;
    }
    return 0;
ret1:
    return 1;
}

/* Address: 0x80253F28 | Size: 0xB4 */
s32 fightTrainerAiWazaHit204(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80253FDC | Size: 0xF0 (240 bytes) */
s32 fightTrainerAiWazaHit203(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235B04(void* ctx, u32 zero, u32 one);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u8 state = fn_80235B04(ctx, 0, 1);
    s32 gate;
    u32 v1;

    if (state == 2) {
        v1 = 0xb;
    } else if (state == 3) {
        v1 = 0x5;
    } else if (state == 1) {
        v1 = 0xa;
    } else if (state == 4) {
        v1 = 0xf;
    } else {
        v1 = 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* -------------------------------------------------------------------
 * Item Rewards & Poke Coupon (0x80254000-0x80258000)
 * 95 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802540CC | Size: 0xB4 */
s32 fightTrainerAiWazaHit202(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254180 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit201(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x38);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit200(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254268 | Size: 0x1F8 (504 bytes) */
void fightTrainerAiWazaHit199(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r31 = r6;
    r28 = r4;
    r29 = r5;
    r3 = 0x2;
    r4 = r31;
    fn_801F025C();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_802543A0;
        }
        r3 = r27;
        r4 = r31;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_802543A0:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r30;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254460 | Size: 0xB4 */
s32 fightTrainerAiWazaHit198(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254514 | Size: 0xB4 */
s32 fightTrainerAiWazaHit197(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802545C8 | Size: 0xB0 */
s32 fightTrainerAiWazaHit196(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80254680 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit194(u32 r3, u32 r4, u32 r5, u32 r6)

{
  extern u32 fn_80236BFC();
  u32 uVar1;
  
  uVar1 = fn_80236BFC(r3,r6,0x28);
  return (uVar1 & 0xff) != 1;
}
/* Address: 0x802546B8 | Size: 0x30 | Ghidra import */
u32 fightTrainerAiWazaHit193(void)

{
    extern u32 fn_8023720C();
  u32 uVar1;
  
  uVar1 = fn_8023720C();
  uVar1 = __cntlzw(1 - (uVar1 & 0xff));
  return uVar1 >> 5 & 0xff;
}
/* Address: 0x802546E8 | Size: 0x128 (296 bytes) */
void fightTrainerAiWazaHit192(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r6 = 0x0;
    r7 = 0x0;
    r5 = (u32)sp + 0x1c;
    r29 = r3;
    r27 = r4;
    fn_802367CC();
    r31 = r3;
    r4 = r29;
    r5 = (u32)sp + 0x30;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r28 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x27;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r27 = (u32)sp + 0x30;
    r28 = r28 & 0xFFFF;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)r28) break;
        r4 = *(u32*)(r27 + r0);
        if (r4 != (u32)0x0) {
            r3 = r29;
            r5 = (u32)sp + 0x8;
            r6 = 0x0;
            r7 = 0x0;
            fn_802367CC();
            r5 = (u32)sp + 0x1c;
            r4 = r31 & 0xFFFF;
            r8 = (u32)sp + 0x8;
            r0 = r3 & 0xFFFF;
            r10 = 0x0;
            while (1) {
                r3 = r10 & 0xFFFF;
                if (r3 >= (u32)r0) break;
                r9 = 0x0;
                while (1) {
                    r3 = r9 & 0xFFFF;
                    if (r3 >= (u32)r4) break;
                    r6 = *(u16*)(r8 + r7);
                    r3 = *(u16*)(r5 + r3);
                    if (r6 == (u32)r3) {
                        r3 = 0x1;
                        return;
                    }
                    r9 = r9 + 0x1;

                }
                r10 = r10 + 0x1;

            }
        }
        r30 = r30 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* Address: 0x80254810 | Size: 0xC8 (200 bytes) */
s32 fightTrainerAiWazaHit191(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u8 fn_80229934(u32 param2, u32 param1, u32 param3);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);

    if (fn_80229934(param2, param1, param3) == 1) {
        return 0;
    }
    if (fn_80237F74(ctx, param1, 0x19) == 1) {
        return 0;
    }
    if (fn_80237F74(ctx, param3, 0x19) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802548D8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit190(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025498C | Size: 0xE4 (228 bytes) */
s32 fightTrainerAiWazaHit189(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u16 fn_802376EC(void* ctx, u32 elem);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    u16 a = fn_802376EC(ctx, param1);
    u16 b = fn_802376EC(ctx, param3);
    s32 gate;

    if (b <= a) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80254A70 | Size: 0xB4 */
s32 fightTrainerAiWazaHit188(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254B24 | Size: 0x178 (376 bytes) */
void fightTrainerAiWazaHit187(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80229704();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckSawagu();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r5;
    r30 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x48;
    r4 = r30;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r29;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    fightTrainerAiCheckSawagu();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r4 = r30;
    r3 = 0x8;
    fn_80229704();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254C9C | Size: 0xB0 */
s32 fightTrainerAiWazaHit186(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80254D4C | Size: 0xB4 */
s32 fightTrainerAiWazaHit185(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254E00 | Size: 0x2c | Ghidra import */
u32 fightTrainerAiWazaHit184(void)

{
    extern u16 fn_80236B98();
  u16 sVar1;
  
  sVar1 = fn_80236B98();
  return sVar1 != 0;
}
/* Address: 0x80254E34 | Size: 0xB4 */
s32 fightTrainerAiWazaHit182(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254EE8 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit181(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x25);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x80254F1C | Size: 0x38 | Ghidra import */
/* Address: 0x80254F1C | Size: 0x38 | Ghidra import */
u32 fightTrainerAiWazaHit180(u32 r3, u32 r4)
{
    extern u32 fn_80215008();
    u8 auStack_38[0x38];

    u32 uVar1 = fn_80215008(r3, auStack_38, 0x18, r4);
    return (-uVar1 & ~uVar1) >> 0x1f;
}
/* Address: 0x80254F54 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit179(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x35);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit178(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80237F74(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFF);
    if (fn_80237F74(ctx, extra, 0x19) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255000 | Size: 0xF0 (240 bytes) */
s32 fightTrainerAiWazaHit177(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802383A4(void* ctx, u32 elem);
    extern u8 fn_80216048(u32 elem);
    extern u8 fn_80142984(u32 elem);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    u16 A = fn_802383A4(ctx, param1);
    u16 B = fn_802383A4(ctx, param3);
    u8 c = fn_80216048(param1);

    if (!c) {
        return 0;
    }
    if (A == 0 && B == 0) {
        goto ret0;
    }
    if (A == 0xaf) {
        goto ret0;
    }
    if (B == 0xaf) {
        goto ret0;
    }
    if (A != 0 && fn_80142984(A) == 0) {
        goto ret0;
    }
    if (B != 0 && fn_80142984(B) == 0) {
        return 0;
    }
    if (fn_80237F74(ctx, param3, 0x3c) == 1) {
        goto ret0;
    }
    return 1;
ret0:
    return 0;
}

/* Address: 0x802550F0 | Size: 0xB4 */
u32 fightTrainerAiWazaHit176(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F025C(u32);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u8 fightOutPokemonCheckFightOut(u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    u32 target;

    target = fn_801F025C(0xe);
    if (target == 0) {
        return 0;
    }
    if ((fn_801F54A4(0, 0, 0x19, 0) & 0xFFFF) < 2) {
        goto fail;
    }
    if (fightOutPokemonCheckFightOut(target) != 1) {
        goto fail;
    }
    if (fn_80236BFC(ctx, slot, 0x32) != 0) {
        goto fail;
    }
    if (fn_80236BFC(ctx, target, 0x32) == 0) {
        goto success;
    }
fail:
    return 0;
success:
    return 1;
}

/* Address: 0x802551A4 | Size: 0x74 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit175(void* ctx, u32 slot, u32 param, u32 param3) {
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    u32 result;

    result = _fightTrainerAiWazaHitCheck(ctx, slot, param, param3, 0);
    if ((fn_80236BFC(ctx, param3, 0x30) & 0xff) == 1) {
        return 0;
    }
    if ((s32)result == 0) {
        return 0;
    }
    if ((s32)result == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255220 | Size: 0xA8 */
u32 fightTrainerAiWazaHit173(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 fn_801363E8(u32);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u32 fn_8023C530(void*, u32, u32, u32);
    u32 paramType;
    u32 other;
    u32 otherType;

    other = fn_801363E8(fn_801F54A4(0, 0, 0xf, 0) & 0xFFFF);
    paramType = wazaGetStatus(0, param, 9, 0) & 0xFFFF;
    param = other;
    otherType = wazaGetStatus(0, param, 9, 0) & 0xFFFF;
    if (otherType != paramType) {
        return fn_8023C530(ctx, slot, param, extra);
    }
    return 1;
}

/* Address: 0x802552D0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit171(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80255384 | Size: 0xB4 */
s32 fightTrainerAiWazaHit170(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80255438 | Size: 0xB4 */
s32 fightTrainerAiWazaHit169(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802554EC | Size: 0x10C (268 bytes) */
s32 fightTrainerAiWazaHit168(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235AA0(void* ctx, u32 elem);
    extern u8 fn_802359D8(void* ctx, u32 elem);
    extern u8 fightTrainerAiCheckGuard(void* ctx, u32 param3, u32 param2);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u8 q1 = fn_80235AA0(ctx, param3);
    u8 q2 = fn_802359D8(ctx, param3);

    if (fightTrainerAiCheckGuard(ctx, param3, param2) == 1) {
        return 0;
    }
    if (q1 == 0 && q2 == 0) {
        return 0;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 1, 1) == 0 &&
        fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 4, 1) == 0) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    return 1;
}

/* Address: 0x802555F8 | Size: 0x228 (552 bytes) */
u32 fightTrainerAiWazaHit167(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80236BFC();
    extern u8 fn_80237DBC();
    extern u8 fn_80237F74();
    extern u8 fn_80237310();
    extern s32 _fightTrainerAiWazaHitCheck();
    extern s32 fn_801F025C();
    extern u8 fightSideIsJoutaiDataId();
    s32 handle;
    u8 flag;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) return 0;
    if (fn_80236BFC(ctx, param3, 0x6) == 1) return 0;
    if (fn_80237DBC(ctx, param3, 0xA) == 1) return 0;
    if (fn_80237F74(ctx, param3, 0x29) == 1) return 0;

    if (fn_80237F74(ctx, param3, 0x11) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x14) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x7) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0xF) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x48) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x29) == 1) {
        flag = 0;
        goto check;
    }
    if (fn_80237F74(ctx, param3, 0x28) != 1) {
        fn_80237F74(ctx, param3, 0xC);
    }
flag1:
    flag = 1;
check:
    if (flag == 0) return 0;

    if (fn_80237310(ctx, param3) == 0) return 0;

    handle = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4B) == 1) return 0;
    if (handle == 0) return 0;
    if (handle == -1) return 1;
    return 1;
}

/* Address: 0x80255820 | Size: 0x218 (536 bytes) */
void fightTrainerAiWazaHit166(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80235910();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckAbiCnt();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r3;
    r27 = r6;
    r25 = r4;
    r26 = r5;
    r3 = 0x2;
    r4 = r27;
    fn_801F025C();
    r0 = r3;
    r3 = r31;
    r28 = r0;
    r4 = r27;
    fn_80235910();
    r29 = r3;
    r3 = r31;
    r4 = r25;
    r5 = r26;
    r6 = r27;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r30 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r29 & 0xFF;
        if (r0 >= (u32)0xc) {
            r3 = r31;
            r4 = r25;
            r5 = r27;
            r6 = r26;
            r7 = 0x10;
            r8 = 0x4;
            r9 = 0x1;
            fightTrainerAiCheckAbiCnt();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0xc) {
                r3 = 0x0;
                return;
    }
    }
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_802559DC;
        }
        r3 = r31;
        r4 = r27;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_802559DC:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80255A38 | Size: 0x74 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit165(void* ctx, u32 slot, u32 param, u32 param3) {
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    u32 result;

    result = _fightTrainerAiWazaHitCheck(ctx, slot, param, param3, 0);
    if ((fn_80236BFC(ctx, param3, 0x1b) & 0xff) == 1) {
        return 0;
    }
    if ((s32)result == 0) {
        return 0;
    }
    if ((s32)result == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255AAC | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit164(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 4;
}
/* Address: 0x80255AE4 | Size: 0x68 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit162(void* ctx, u32 slot, u32 param) {
    extern f32 lbl_8047E648;
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);

    if (fn_80236BFC(ctx, slot, 0x2d) == 0) {
        return 0;
    }
    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}

/* Address: 0x80255B4C | Size: 0xCC (204 bytes) */
s32 fightTrainerAiWazaHit161(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u8 fn_80236BFC(void* ctx, u32 param1, u32 flag);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_80236BFC(ctx, param1, 0x2d) == 0) {
        gate = 0;
    }
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255C18 | Size: 0x74 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit160(void* ctx, u32 slot, u32 param) {
    extern u8 fn_80236BFC(void* ctx, u32 slot, u32 param);
    extern u32 fn_80202360(u32 slot, u32 param);
    extern u8 fn_80119DD0(u32 param);
    u32 result;

    result = slot;
    if (fn_80236BFC(ctx, slot, 0x2d) == 0) {
        result = 0;
    } else {
        result = fn_80202360(result, 0x2d);
    }
    if ((s16)result >= (s32)fn_80119DD0(0x2d)) {
        return 0;
    }
    return 1;
}

/* Address: 0x80255C8C | Size: 0xB0 */
s32 fightTrainerAiWazaHit159(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255D3C | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaHit158(u32 r3_in, u32 r4)
{
    extern u32 pokemonGetStatus();
  u32 uVar1;
  u8 result;
  uVar1 = pokemonGetStatus(r4, 0, 0xed, 0) & 0xFFFF;
  result = uVar1 != 0;
  return result;
}
/* Address: 0x80255D7C | Size: 0x38 */
u32 fightTrainerAiWazaHit157(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x80255DB4 | Size: 0x44 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit156(void* ctx, u32 slot, u32 param, u32 arg3) {
    extern u32 fightTrainerAiCheckAbiCnt();
    u32 result = fightTrainerAiCheckAbiCnt(ctx, slot, arg3, param, 0x10, 0x2, 0x41) & 0xFF;
    return (result != 0) ? 1 : 0;
}

/* Address: 0x80255DF8 | Size: 0xB0 */
s32 fightTrainerAiWazaHit155(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255EA8 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit154(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern int _fightTrainerAiWazaHitCheck();
  int iVar1;
  iVar1 = _fightTrainerAiWazaHitCheck(r3, r4, r5, r6, 0);
  if (iVar1 == 0) {
    return 0;
  }
  if (iVar1 == -1) {
    return 1;
  }
  return 1;
}
/* Address: 0x80255EEC | Size: 0xB4 */
s32 fightTrainerAiWazaHit152(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80255FA0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit151(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256054 | Size: 0xB4 */
s32 fightTrainerAiWazaHit150(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256108 | Size: 0xB4 */
s32 fightTrainerAiWazaHit149(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802561BC | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit148(u32 r3, u32 r4, u32 r5, u32 r6)

{
  extern u32 fn_80236BFC();
  u32 uVar1;
  
  uVar1 = fn_80236BFC(r3,r6,0x34);
  return (uVar1 & 0xff) != 1;
}
/* Address: 0x802561F4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit147(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802562A8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit146(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025635C | Size: 0xB4 */
s32 fightTrainerAiWazaHit145(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256410 | Size: 0xB0 */
s32 fightTrainerAiWazaHit144(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802564C8 | Size: 0x64 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit142(void* ctx, u32 slot, u32 param) {
    extern f32 lbl_8047E64C;
    extern u32 fn_80235AA0(void);
    extern u32 fn_802373B0(void*, u32, s32, f32);

    if ((fn_80235AA0() & 0xFF) >= 0xC) {
        goto ret0;
    }
    if ((fn_802373B0(ctx, slot, -1, lbl_8047E64C) & 0xFF) == 0) {
        goto ret1;
    }
ret0:
    return 0;
ret1:
    return 1;
}

/* Address: 0x8025652C | Size: 0xB4 */
s32 fightTrainerAiWazaHit140(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802565E0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit139(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256694 | Size: 0xB4 */
s32 fightTrainerAiWazaHit138(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256748 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit137(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 1;
}
/* Address: 0x80256780 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit136(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 2;
}
/* Address: 0x802567B8 | Size: 0xA4 */
s32 fightTrainerAiWazaHit135(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32 v);
    extern void pokemonGetMezamerupower(u32, void*, void*);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, s16 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u16 hi;
    u16 lo;
    s32 gate;

    pokemonGetMezamerupower(fightOutPokemonGetPokemonPtr(param1), &hi, &lo);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, lo, (s16)hi) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025685C | Size: 0x38 */
u32 fightTrainerAiWazaHit134(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x80256894 | Size: 0x38 */
u32 fightTrainerAiWazaHit133(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x802568CC | Size: 0x38 */
u32 fightTrainerAiWazaHit132(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x80256904 | Size: 0xB0 */
s32 fightTrainerAiWazaHit130(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802569B4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit129(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256A68 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit128(void* ctx, u32 a, u32 b, u32 c) {
    extern s32 fn_802395C8();
    extern s32 fn_80239500();
    extern s32 fn_8023793C();
    s32 res1;

    res1 = fn_802395C8(ctx, b, a);
    return !((u16)fn_8023793C(ctx, c, res1, fn_80239500(ctx, b)) == 0x43);
}

/* Address: 0x80256AE0 | Size: 0x38 | Ghidra import */
u32 fightTrainerAiWazaHit127(void)
{
    extern int fn_801F8A18();
  u32 r3;
  int iVar1;
  u16 local_8[4];

  local_8[0] = 0;
  iVar1 = fn_801F8A18(r3, local_8);
  return (-iVar1 != 0) ? 1 : 0;
}
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
s32 fightTrainerAiWazaHit126(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256BCC | Size: 0xB4 */
s32 fightTrainerAiWazaHit125(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256C80 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit124(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x4b);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x80256CBC | Size: 0xB4 */
s32 fightTrainerAiWazaHit123(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256D70 | Size: 0xB0 */
s32 fightTrainerAiWazaHit122(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80256E20 | Size: 0xB0 */
s32 fightTrainerAiWazaHit121(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80256ED0 | Size: 0x200 (512 bytes) */
void fightTrainerAiWazaHit120(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightOutPokemonGetSex();
    extern void fn_80236BFC();
    extern void fn_80237288();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r26 = r4;
    r27 = r3;
    r29 = r5;
    r28 = r6;
    r3 = r26;
    fightOutPokemonGetSex();
    r30 = r3;
    r3 = r28;
    fightOutPokemonGetSex();
    r31 = r3;
    r3 = r27;
    r4 = r26;
    r5 = r29;
    r6 = r28;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    r4 = r28;
    r5 = 0xc;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r29 = 0x0;
    }
    r3 = r27;
    r4 = r28;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r27;
        r4 = r28;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r27;
            r4 = r28;
            r5 = 0x7;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r3 = r27;
                r4 = r28;
                r5 = 0xf;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                    r3 = r27;
                    r4 = r28;
                    r5 = 0x48;
                    fn_80237F74();
                    r0 = r3 & 0xFF;
                    if (r0 != (u32)0x1) {
                        r3 = r27;
                        r4 = r28;
                        r5 = 0x29;
                        fn_80237F74();
                        r0 = r3 & 0xFF;
                        if (r0 != (u32)0x1) {
                            r3 = r27;
                            r4 = r28;
                            r5 = 0x28;
                            fn_80237F74();
                            r0 = r3 & 0xFF;
                            if (r0 != (u32)0x1) {
                                r3 = r27;
                                r4 = r28;
                                r5 = 0xc;
                                fn_80237F74();
                                r0 = r3 & 0xFF;
                                if (r0 == (u32)0x1) {
                                    r0 = 0x0;
                                    goto L_80257034;
    }
    }
    }
    }
    }
    }
    }
    }
    r0 = 0x1;
L_80257034:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30 & 0xFF;
    r0 = r31 & 0xFF;
    if (r3 != (u32)r0) {
        r3 = r27;
        r4 = r28;
        fn_80237288();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r27;
            r4 = r28;
            r5 = 0xa;
            fn_80236BFC();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r0 = r30 & 0xFF;
                if (r0 != (u32)0x2) {
                    r0 = r31 & 0xFF;
                    if (r0 == (u32)0x2) {
        }
        }
        }
        }
        r29 = 0x0;
                    }
    if ((s32)r29 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802570D0 | Size: 0xB0 */
s32 fightTrainerAiWazaHit119(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257180 | Size: 0x23C (572 bytes) */
void fightTrainerAiWazaHit118(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80235AA0();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckAbiCnt();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r3;
    r27 = r6;
    r25 = r4;
    r26 = r5;
    r3 = 0x2;
    r4 = r27;
    fn_801F025C();
    r0 = r3;
    r3 = r31;
    r28 = r0;
    r4 = r27;
    fn_80235AA0();
    r29 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = r25;
    r5 = r26;
    r6 = r27;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r30 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r29 & 0xFF;
        if (r0 >= (u32)0xc) {
            r3 = r31;
            r4 = r25;
            r5 = r27;
            r6 = r26;
            r7 = 0x20;
            r8 = 0x1;
            r9 = 0x1;
            fightTrainerAiCheckAbiCnt();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0xc) {
                r3 = 0x0;
                return;
    }
    }
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_80257360;
        }
        r3 = r31;
        r4 = r27;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_80257360:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802573BC | Size: 0xB0 */
s32 fightTrainerAiWazaHit117(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257474 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit115(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 3;
}
/* Address: 0x802574AC | Size: 0x5C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit114(void* ctx, u32 slot, u32 param, u32 arg3) {
    extern u32 fn_80229934();
    extern u32 fightTrainerAiCheckHorobinouta();

    if ((u8)fn_80229934(param, slot, arg3) == 1) {
        return 0;
    }
    return (u16)fightTrainerAiCheckHorobinouta(ctx) != 0;
}

/* Address: 0x80257508 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit113(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern int _fightTrainerAiWazaHitCheck();
  int iVar1;
  iVar1 = _fightTrainerAiWazaHitCheck(r3, r4, r5, r6, 0);
  if (iVar1 == 0) {
    return 0;
  }
  if (iVar1 == -1) {
    return 1;
  }
  return 1;
}
/* Address: 0x80257544 | Size: 0x7C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit112(void* ctx, u32 slot, u32 param) {
    extern u8 fn_80119DD0(u32);
    extern u32 fn_801F025C(u32);
    extern u32 fightSideGetCountAsJoutaiDataId(u32, u32);
    extern u8 fightSideIsJoutaiDataId(u32, u32);
    u32 target;
    u32 count;

    target = fn_801F025C(3);
    count = 0;
    if (fightSideIsJoutaiDataId(target, 0x4a) == 1) {
        count = fightSideGetCountAsJoutaiDataId(target, 0x4a);
    }
    if ((s16)count >= (s32)fn_80119DD0(0x4a)) {
        return 0;
    }
    return 1;
}

/* Address: 0x802575C8 | Size: 0x188 (392 bytes) */
s32 fightTrainerAiWazaHit109(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235AA0(void* ctx, u32 elem);
    extern u8 fn_80235A3C(void* ctx, u32 elem);
    extern u8 fn_80235910(void* ctx, u32 elem);
    extern u8 fn_80237DBC(void* ctx, u32 a, u32 type);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fn_80237288(void* ctx, u32 elem);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u8 a = fn_80235AA0(ctx, param3);
    u8 b = fn_80235A3C(ctx, param3);
    u8 c = fn_80235910(ctx, param3);

    if (fn_80237DBC(ctx, param1, 7) == 1) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        if (fn_80236BFC(ctx, param3, 0x18) != 1) {
            if (fn_80237288(ctx, param3) != 1) {
                goto ret1;
            }
        }
        return 0;
    }

    if (((c == 0) && (a >= 0xc)) && (b >= 0xc)) {
        return 0;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 3, 0x41) != 0) {
        goto ret1;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 1, 0x41) != 0) {
        goto ret1;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 2, 0x41) != 0) {
        goto ret1;
    }
ret0:
    return 0, 0;

    return 0;
ret1:
    return 1;
}

/* Address: 0x80257750 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit108(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 7, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80257838 | Size: 0x8C */
u32 fightTrainerAiWazaHit107(void* ctx, u32 param1, u32 param2, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);

    if (fn_80236BFC(ctx, extra, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, extra, 0x17) == 1) {
        return 0;
    }
    return fn_80236BFC(ctx, extra, 8) != 0;
}

/* Address: 0x802578C4 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit106(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFE);
    if (fn_80236BFC(ctx, extra, 0x16) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025793C | Size: 0xB4 */
s32 fightTrainerAiWazaHit105(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802579F0 | Size: 0xB0 */
s32 fightTrainerAiWazaHit104(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257AA0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit103(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80257B54 | Size: 0xCC (204 bytes) */
s32 fightTrainerAiWazaHit102(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1A6C(u32 zero, void* ctx, void* buf, u32 one1, u32 one2);
    extern u8 fn_80239058(void* ctx, u32 elem, u32 type);
    extern u8 fn_80238748(void* ctx, u32 elem);
    u32 buf[0x17];
    u32 count = fn_801F1A6C(0, ctx, buf, 1, 1);
    u16 target;
    u16 count16;
    u32 *bufPtr;
    u16 i;
    u8 flag;

    bufPtr = buf;
    count16 = (u16)count;
    target = (u16)param2;
    flag = 0;
    for (i = 0; i < count16; i++) {
        if (bufPtr[i] == 0) {
            continue;
        }
        if (target == 0xd7 && fn_80239058(ctx, bufPtr[i], 0x2b) == 1) {
            continue;
        }
        if (fn_80238748(ctx, bufPtr[i]) != 0) {
            continue;
        }
        flag = 1;
        break;
    }
    if (flag == 0) {
        return 0;
    }
    return 1;
}

/* Address: 0x80257C20 | Size: 0xB4 */
s32 fightTrainerAiWazaHit101(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80257CD4 | Size: 0x124 (292 bytes) */
s32 fightTrainerAiWazaHit100(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u16 fn_802367CC(void* ctx, u32 param3, void* arrayA, void* arrayB, u32 flag);
    extern u32 fn_802364BC(void* ctx, u32 param3);
    extern u8 fn_80237288(void* ctx, u32 elem);
    extern u8 fn_802381C4(void* ctx, u32 param3, u8 v);
    u16 arrayA[0xe];
    s16 arrayB[0xa];
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);
    u16 count = fn_802367CC(ctx, param3, arrayA, arrayB, 0);
    u32 a = fn_802364BC(ctx, param3);
    u8 flag;
    u8 i;

    if ((u16)a == 0 || (u16)a == 0x165 || (u16)a == 0xffff || fn_80237288(ctx, param3) == 1) {
        return 0;
    }
    flag = 0;
    for (i = 0; i < count; i++) {
        if (arrayB[i] < 0) {
            continue;
        }
        if ((u16)a != arrayA[i]) {
            continue;
        }
        flag = fn_802381C4(ctx, param3, (u8)arrayB[i]);
        break;
    }
    if (flag == 0) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257DF8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit099(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80257EAC | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit098(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    extern u32 fn_80229934();

    return (u8)fn_80229934(arg2, arg1, arg3) != 1;
}
/* Address: 0x80257EE4 | Size: 0xE4 (228 bytes) */
s32 fightTrainerAiWazaHit097(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 param1, void* buf, u32 zero, u32 one);
    extern u8 fn_80236BFC(void* ctx, u32 param1, u32 flag);
    extern u8 fightSeqRendouWazaCheck(u32 elem);
    extern u8 fn_8021901C(int elem);
    u16 buf[0xa];
    u16 count = fn_802367CC(ctx, param1, buf, 0, 1);
    u16 i;

    if (!fn_80236BFC(ctx, param1, 8)) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        u16 elem = buf[i];
        if (elem == 0x165 || elem == 0x163) {
            continue;
        }
        if (fightSeqRendouWazaCheck(elem) != 0) {
            continue;
        }
        if (elem == 0x108 || elem == 0xfd) {
            continue;
        }
        if (fn_8021901C(elem) == 0) {
            break;
        }
    }
    if (i >= count) {
        return 0;
    }
    return 1;
}

/* Address: 0x80257FC8 | Size: 0xF4 (244 bytes) */
s32 fightTrainerAiWazaHit095(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802367CC(void* ctx, u32 param1, void* buf, u32 zero1, u32 zero2);
    extern u32 fn_80236458(void* ctx, u32 param3);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fn_80219270(u32 elem);
    u16 buf[0xa];
    u32 count = fn_802367CC(ctx, param1, buf, 0, 0);
    u32 target = fn_80236458(ctx, param3);
    u16 i;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param1, 0x10) == 1) {
        goto ret0;
    }
    if (fn_80219270(target) != 1) {
        goto scan;
    }
ret0:
    return 0;
scan:
    for (i = 0; i < (u16)count; i++) {
        if (buf[i] == (u16)target) {
            return 0;
        }
    }
    return 1;
}

/* -------------------------------------------------------------------
 * Team State Updates (0x80258000-0x8025C000)
 * 78 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802580BC | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit094(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFF);
    if (fn_80236BFC(ctx, extra, 0x14) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025813C | Size: 0xD0 (208 bytes) */
s32 fightTrainerAiWazaHit092(void* ctx, u32 param1, u32 param2, u32 param3) {
    u32 *new_var;
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u8 fn_80236BFC(void* ctx, u32 param1, u32 flag);
    extern u32 fn_80239500(void* ctx, u32 param2);
    s32 gate;
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);

    if (!fn_80236BFC(ctx, param1, 0x8)) {
        return 0;
    }
    new_var = &v1;
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, *new_var, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (((0, gate)) == (-1)) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025820C | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit091(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFF);
    if (fn_80236BFC(ctx, extra, 0x14) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258284 | Size: 0x140 (320 bytes) */
s32 fightTrainerAiWazaHit090(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 param3, void* arrayA, void* arrayB, u32 flag);
    extern u32 fn_802364BC(void* ctx, u32 param3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightSeqIsEncoreNgWazaDataId(u32 elem);
    extern u8 fn_802381C4(void* ctx, u32 param3, u8 v);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    u16 arrayA[0xe];
    s16 arrayB[0xa];
    u16 count = fn_802367CC(ctx, param3, arrayA, arrayB, 0);
    u32 targetVal = fn_802364BC(ctx, param3);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    u8 flag;
    u16 i;

    if (fightSeqIsEncoreNgWazaDataId(targetVal) == 1) {
        return 0;
    }
    flag = 0;
    for (i = 0; i < count; i++) {
        if (arrayB[i] < 0) {
            continue;
        }
        if ((u16)targetVal != arrayA[i]) {
            continue;
        }
        flag = fn_802381C4(ctx, param3, (u8)arrayB[i]);
        break;
    }
    if (flag == 0) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x2a) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;

    return;
}

/* Address: 0x802583C4 | Size: 0xB0 */
s32 fightTrainerAiWazaHit089(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258474 | Size: 0xB0 */
s32 fightTrainerAiWazaHit088(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258524 | Size: 0xB0 */
s32 fightTrainerAiWazaHit087(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802585D4 | Size: 0x120 (288 bytes) */
s32 fightTrainerAiWazaHit086(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 param3, void* arrayA, void* arrayB, u32 flag);
    extern u16 fn_802364BC(void* ctx, u32 param3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fn_802381C4(void* ctx, u32 param3, u8 v);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    u16 arrayA[0xe];
    s16 arrayB[0xa];
    u16 count = fn_802367CC(ctx, param3, arrayA, arrayB, 0);
    u16 target = fn_802364BC(ctx, param3);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    u8 flag = 0;
    u16 i;

    for (i = 0; i < count; i++) {
        if (arrayB[i] < 0) {
            continue;
        }
        if (target != arrayA[i]) {
            continue;
        }
        flag = fn_802381C4(ctx, param3, (u8)arrayB[i]);
        break;
    }
    if (flag == 0) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x29) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;

    return;
}

/* Address: 0x802586FC | Size: 0xBC */
s32 fightTrainerAiWazaHit084(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fn_80237DBC(void* ctx, u32 a, u32 type);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_80236BFC(ctx, param3, 0x1c) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0xc) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802587C0 | Size: 0x144 (324 bytes) */
s32 fightTrainerAiWazaHit082(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 elem, void* arrA, void* arrB, u32 flag);
    extern u32 fn_802364BC(void* ctx, u32 param3);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u8 fightSeqMonomaneNGCheck(u32 elem);
    u16 buf[0x10];
    u16 count = fn_802367CC(ctx, param1, buf, 0, 0);
    u32 targetVal = fn_802364BC(ctx, param3);
    s32 gate;
    u16 i;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);
    if (fn_80236BFC(ctx, param1, 0x10) == 1 ||
        fightSeqMonomaneNGCheck(targetVal) != 0 ||
        (u16)targetVal == 0 || (u16)targetVal == 0xffff || (u16)targetVal == 0x165 || (u16)targetVal == 0x163) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        if ((u16)targetVal == buf[i]) {
            return 0;
        }
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258904 | Size: 0xB0 */
s32 fightTrainerAiWazaHit081(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802589B4 | Size: 0xB0 */
s32 fightTrainerAiWazaHit080(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258A64 | Size: 0x6C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit079(void* ctx, u32 slot, u32 param) {
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_802373B0(void*, u32, s32, f32);
    extern f32 lbl_8047E650;
    u32 result;

    if ((fn_80236BFC(ctx, slot, 0x14) & 0xff) == 1) {
        return 0;
    }

    result = fn_802373B0(ctx, slot, -1, lbl_8047E650) & 0xff;
    return result != 1;
}

/* Address: 0x80258AD0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit078(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258B84 | Size: 0xB4 */
s32 fightTrainerAiWazaHit077(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258C38 | Size: 0xB4 */
s32 fightTrainerAiWazaHit076(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258CEC | Size: 0xB4 */
s32 fightTrainerAiWazaHit075(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258DA0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit073(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258E54 | Size: 0xB4 */
s32 fightTrainerAiWazaHit072(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258F08 | Size: 0xB4 */
s32 fightTrainerAiWazaHit071(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258FBC | Size: 0xB4 */
s32 fightTrainerAiWazaHit070(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80259070 | Size: 0xB4 */
s32 fightTrainerAiWazaHit069(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80259124 | Size: 0xB4 */
s32 fightTrainerAiWazaHit068(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802591D8 | Size: 0x260 (608 bytes) */
void fightTrainerAiWazaHit067(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_8023793C();
    extern void fn_80237F74();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r30 = r6;
    r28 = r4;
    r29 = r5;
    r3 = 0x2;
    r4 = r30;
    fn_801F025C();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r29;
    r5 = r28;
    fn_802395C8();
    r0 = r3;
    r3 = r27;
    r26 = r0;
    r4 = r30;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_80259328;
        }
        r3 = r27;
        r4 = r30;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_80259328:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r29;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r26;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r27 = r3;
    r3 = r31;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r27 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259438 | Size: 0x270 (624 bytes) */
u8 fightTrainerAiWazaHit066(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80237F74(void*, u32, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_80237DBC(void*, u32, u32);
    extern u8 fn_80237310(void*, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    extern u32 fn_801F025C(u32, u32);
    extern u8 fightSideIsJoutaiDataId(u32, u32);
    s32 r;
    u8 flag;

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x4) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x8) == 1) {
        return 0;
    }
    if (fn_80237310(ctx, param3) == 0) {
        return 0;
    }

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        flag = 0;
    } else {
        if (fn_80237F74(ctx, param3, 0x14) != 1) {
            if (fn_80237F74(ctx, param3, 0x7) != 1) {
                if (fn_80237F74(ctx, param3, 0xf) != 1) {
                    if (fn_80237F74(ctx, param3, 0x48) != 1) {
                        if (fn_80237F74(ctx, param3, 0x29) != 1) {
                            if (fn_80237F74(ctx, param3, 0x28) != 1) {
                                fn_80237F74(ctx, param3, 0xc);
                            }
                        }
                    }
                }
            }
        }
        flag = 1;
    }
    if (flag == 0) {
        return 0;
    }

    r = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4b) == 1) {
        return 0;
    }
    if (r == 0) {
        return 0;
    }
    if (r == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802596A8 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit065(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x48);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x802596E4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit062(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x5, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x802597CC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit060(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x3, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x802598B4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit059(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x2, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025999C | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit058(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x1, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259A84 | Size: 0x68 | Pattern: field_accessor */
int fightTrainerAiWazaHit057(void* ctx, u32 slot, u32 param, u32 arg3) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_80237288(void*, u32);

    if ((u8)fn_80236BFC(ctx, arg3, 0x10) == 1) {
        goto fail;
    }
    if ((u8)fn_80237288(ctx, arg3) != 1) {
        goto success;
    }
fail:
    return 0;
success:
    return 1;
}

/* Address: 0x80259AEC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit054(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x5, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259BD4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit053(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x4, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259CBC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit052(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x3, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259DA4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit051(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x2, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259E8C | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit050(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x1, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259F74 | Size: 0x1F8 (504 bytes) */
void fightTrainerAiWazaHit049(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r31 = r6;
    r28 = r4;
    r29 = r5;
    r3 = 0x2;
    r4 = r31;
    fn_801F025C();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_8025A0F4;
        }
        r3 = r27;
        r4 = r31;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_8025A0F4:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r30;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A16C | Size: 0xB4 */
s32 fightTrainerAiWazaHit048(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A220 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit047(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  return (fn_80236BFC(r3, r4, 0xf) & 0xFF) != 1;
}
/* Address: 0x8025A254 | Size: 0x3c | Ghidra import */
/* Address: 0x8025A254 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit046(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x4c);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x8025A340 | Size: 0xB4 */
s32 fightTrainerAiWazaHit044(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A3F4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit043(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A4A8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit042(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A55C | Size: 0xB0 */
s32 fightTrainerAiWazaHit041(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025A60C | Size: 0xB0 */
s32 fightTrainerAiWazaHit040(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025A6BC | Size: 0xB4 */
s32 fightTrainerAiWazaHit039(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A770 | Size: 0x100 (256 bytes) */
s32 fightTrainerAiWazaHit038(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80237774(void* ctx, u32 elem);
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    s32 gate;
    u8 a = fn_80237774(ctx, param1);
    u8 b = fn_80237774(ctx, param3);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (fn_80237F74(ctx, param3, 5) == 1) {
        gate = 0;
    }
    if (a < b) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025A870 | Size: 0x8C */
u32 fightTrainerAiWazaHit037(void* ctx, u32 param1, u32 param2) {
    extern f32 lbl_8047E648;
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);
    extern u8 fightTrainerAiCheckSawagu(void*, u32);

    if (fn_80236BFC(ctx, param1, 8) == 1) {
        return 0;
    }
    if (fightTrainerAiCheckSawagu(ctx, param1) == 1) {
        return 0;
    }
    return fn_802373B0(ctx, param1, 0, lbl_8047E648) != 1;
}

/* Address: 0x8025A8FC | Size: 0xB4 */
s32 fightTrainerAiWazaHit036(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A9B0 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit035(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x49);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x8025A9EC | Size: 0xB4 */
s32 fightTrainerAiWazaHit034(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025AAA0 | Size: 0x270 (624 bytes) */
u8 fightTrainerAiWazaHit033(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80237F74(void*, u32, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_80237DBC(void*, u32, u32);
    extern u8 fn_80237310(void*, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    extern u32 fn_801F025C(u32, u32);
    extern u8 fightSideIsJoutaiDataId(u32, u32);
    s32 r;
    u8 flag;

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x4) == 1) {
        return 0;
    }
    if (fn_80237310(ctx, param3) == 0) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x8) == 1) {
        return 0;
    }

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        flag = 0;
    } else {
        if (fn_80237F74(ctx, param3, 0x14) != 1) {
            if (fn_80237F74(ctx, param3, 0x7) != 1) {
                if (fn_80237F74(ctx, param3, 0xf) != 1) {
                    if (fn_80237F74(ctx, param3, 0x48) != 1) {
                        if (fn_80237F74(ctx, param3, 0x29) != 1) {
                            if (fn_80237F74(ctx, param3, 0x28) != 1) {
                                fn_80237F74(ctx, param3, 0xc);
                            }
                        }
                    }
                }
            }
        }
        flag = 1;
    }
    if (flag == 0) {
        return 0;
    }

    r = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4b) == 1) {
        return 0;
    }
    if (r == 0) {
        return 0;
    }
    if (r == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025AD10 | Size: 0x38 */
u32 fightTrainerAiWazaHit032(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x8025AD48 | Size: 0xB4 */
s32 fightTrainerAiWazaHit031(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025ADFC | Size: 0x2c | Ghidra import */
u32 fightTrainerAiWazaHit030(void)

{
    extern int fightTrainerAiCheckTextureZokusei();
  u8 cVar1;
  
  cVar1 = fightTrainerAiCheckTextureZokusei();
  return cVar1 != 0;
}
/* Address: 0x8025AE28 | Size: 0xB0 */
s32 fightTrainerAiWazaHit029(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025AED8 | Size: 0xE0 (224 bytes) */
s32 fightTrainerAiWazaHit028(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F4354(u32 zero, u32 param3);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u16 fn_801F87CC(u32 v, void* buf);
    u8 buf[0x14];
    u32 prev = fn_801F4354(0, param3);
    s32 gate;

    if (fn_80237F74(ctx, param3, 0x15) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x25) == 1) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_801F87CC(prev, buf) == 0) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025AFB8 | Size: 0xB0 */
s32 fightTrainerAiWazaHit027(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025B068 | Size: 0xB4 */
s32 fightTrainerAiWazaHit026(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025B124 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit024(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x7, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B20C | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit023(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x6, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B2F4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit020(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x3, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B3DC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit019(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x2, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B4C4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit018(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x1, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B5AC | Size: 0xB4 */
s32 fightTrainerAiWazaHit017(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025B660 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit016(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x7, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B748 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit013(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x4, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B830 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit011(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x2, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B918 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit010(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x1, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025BA00 | Size: 0x2c | Ghidra import */
u32 fightTrainerAiWazaHit009(void)

{
    extern u16 fightTrainerAiCheckOumu();
  u16 sVar1;
  
  sVar1 = fightTrainerAiCheckOumu();
  return sVar1 != 0;
}
/* Address: 0x8025BA2C | Size: 0xF4 (244 bytes) */
void fn_8025BA2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r31;
    r4 = r28;
    fn_802395C8();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r29;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r29;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/*
 * fightTrainerAiWazaHit008 (0x8025BA2C)
 *
 * Same WazaHitNNN veto template as fightTrainerAiWazaHit007, minus the
 * fn_801F1A6C flinch-count check: instead gates on two fn_80236BFC flag
 * checks (0x14, 0x8), then the shared _fightTrainerAiWazaHitCheck tri-state gate / 0x43
 * type-immunity check.
 */
s32 fightTrainerAiWazaHit008(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (!fn_80236BFC(ctx, param3, 0x8)) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025BB20 | Size: 0x108 (264 bytes) */
void fn_8025BB20(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_8022967C();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckSimerike();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = r6;
    r5 = r27;
    r4 = r28;
    fn_802395C8();
    r30 = r3;
    r4 = r26;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r31 = r3;
    r3 = r28;
    fn_8022967C();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r31 & 0xFFFF;
        if (r0 <= (u32)0x1) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r26;
    fightTrainerAiCheckSimerike();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r26;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r26;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/*
 * fightTrainerAiWazaHit007 (0x8025BB20)
 *
 * Per-move AI evaluator gate: vetoes the move (returns 0) if any of 4
 * checks trip -- (1) already-flinched-ish tri-state via fn_8022967C +
 * fn_801F1A6C's flag-count buffer, (2) fightTrainerAiCheckSimerike(ctx), (3) fn_8023793C
 * type/immunity check == 0x43, (4) _fightTrainerAiWazaHitCheck's tri-state gate == 0.
 * Otherwise returns 1. Shares its template with the WazaHitNNN family
 * (see fn_8025BA2C aka fightTrainerAiWazaHit008, same shape minus the
 * fn_801F1A6C flinch-count check).
 */
s32 fightTrainerAiWazaHit007(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_801F1A6C(u32 zero, void* ctx, void* buf, u32 one1, u32 one2);
    extern u8 fn_8022967C(u32 param2);
    extern u8 fightTrainerAiCheckSimerike(void* ctx);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u8 buf[0x60];
    u32 v1 = fn_802395C8(ctx, param2, param1);
    u32 flinchCount = fn_801F1A6C(0, ctx, buf, 1, 1);
    s32 gate;

    if (fn_8022967C(param2) == 1 && (u16)flinchCount <= 1) {
        return 0;
    }
    if (fightTrainerAiCheckSimerike(ctx) == 1) {
        return 0;
    }
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025BC28 | Size: 0xB4 */
s32 fightTrainerAiWazaHit006(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BCDC | Size: 0xB4 */
s32 fightTrainerAiWazaHit005(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BD90 | Size: 0xB4 */
s32 fightTrainerAiWazaHit004(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BE44 | Size: 0xB4 */
s32 fightTrainerAiWazaHit003(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BEF8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit002(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BFAC | Size: 0x200 (512 bytes) */
u32 fightTrainerAiWazaHit001(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fightTrainerAiCheckSawagu(void* ctx, u32 a);
    extern u8 fn_80237310(void* ctx, u32 a);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 p1, u32 p2, u32 p3, u32 z);
    extern u32 fn_801F025C(s32 a, u32 b);
    extern u8 fightSideIsJoutaiDataId(u32 a, u32 b);
    s32 result;
    u8 ok;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) return 0;
    if (fn_80236BFC(ctx, param3, 0x8) == 1) return 0;
    if (fightTrainerAiCheckSawagu(ctx, param3) == 1) return 0;
    if (fn_80237310(ctx, param3) == 0) return 0;
    if (fn_80237F74(ctx, param3, 0x11) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0x14) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0x7) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0xf) == 1 || fn_80237F74(ctx, param3, 0x48) == 1) {
        ok = 0;
        goto check;
    }
    if (fn_80237F74(ctx, param3, 0x29) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0x28) == 1) goto ok1;
    fn_80237F74(ctx, param3, 0xc);
ok1:
    ok = 1;
check:;
    if (ok == 0) return 0;
    result = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4b) == 1) return 0;
    if (result == 0) return 0;
    if (result == -1) return 1;
    return 1;
}

/* -------------------------------------------------------------------
 * Shadow Pokemon & Purification (0x8025C000-0x80260000)
 * 89 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8025C1AC | Size: 0xB0 */
s32 fightTrainerAiWazaHit000(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025C264 | Size: 0x340 (832 bytes) */
void _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void wazaGetStatus();
    extern void fn_801F0134();
    extern void fn_801F1D5C();
    extern void fn_801F3BB4();
    extern void fn_801F54A4();
    extern void fn_80201D84();
    extern void fn_80229934();
    extern void fn_80229B70();
    extern void fn_80229BD8();
    extern void fn_80235B04();
    extern void fn_80236BFC();
    extern void fn_80237288();
    extern void fightTrainerAiCheckGuard();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r26;
    fn_801F0134();
    r31 = r3;
    r3 = r25;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r4 = r27;
    r3 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    wazaGetStatus();
    r30 = r3 & 0xFFFF;
    if (r28 == (u32)0x0) {
        r3 = 0x1;
        return;
    }
    r7 = (u32)sp + 0x8;
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F1D5C();
    r24 = r3;
    r4 = (u32)sp + 0x8;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x0;
    fn_801F3BB4();
    r4 = (u32)sp + 0x8;
    r0 = r24 & 0xFFFF;
    r6 = 0x0;
    r7 = 0x0;
    r5 = 0x0;
    while (1) {
        r3 = r5 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        r3 = *(u32*)(r4 + r3);
        if (r3 != (u32)0x0) {
            if (r26 == (u32)r3) {
                r6 = r5;
            }
            if (r28 == (u32)r3) {
                r7 = r5;
        }
        }
        r5 = r5 + 0x1;

    }
    r3 = r6 & 0xFFFF;
    r0 = r7 & 0xFFFF;
    r0 = r3 - r0;
    r24 = (u32)r0 >> 31;

    if (r29 == (u32)0xffff || r29 == (u32)0xfffe) {

        if (r29 == (u32)0xffff) {
            r3 = r25;
            r4 = r28;
            r5 = 0x1d;
            fn_80236BFC();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r28;
                r4 = 0x1d;
                fn_80201D84();
                r3 = r3 & 0xFFFF;
                r0 = r31 & 0xFFFF;
                if (r0 == (u32)r3) {
                    r3 = -0x1;
                    return;
        }
        }
        }
        r3 = r25;
        r4 = r26;
        fn_80237288();
        r0 = r3 & 0xFF;
        if ((r0 == (u32)0x1) && (r24 == (u32)0x1)) {

            r3 = 0x0;
            return;
        }
        r3 = r25;
        r4 = r28;
        r5 = r27;
        fightTrainerAiCheckGuard();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = 0x1;
        return;
    }
    r3 = r27;
    r4 = r26;
    r5 = r28;
    fn_80229934();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = r27;
    fightTrainerAiCheckGuard();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r28;
        r4 = 0x1d;
        fn_80201D84();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if (r3 == (u32)r0) {
            r3 = -0x1;
            return;
    }
    }
    if (r24 == (u32)0x1 && r0 == (u32)0x1 && r0 != (u32)0x39 && r0 != (u32)0xfa) {
        r3 = r25;
        r4 = r28;
        r5 = 0x1f;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            if (r30 != (u32)0x92) {
                if (r30 != (u32)0x95) {
                    if (r30 != (u32)0x98) {
                        if (r30 != (u32)0xcf) {
                            r3 = 0x0;
                            return;
        }
        }
        }
        }
        }
        r3 = r25;
        r4 = r28;
        r5 = 0x20;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((r0 == (u32)0x1) && (r30 != (u32)0x93)) {

            r3 = 0x0;
            return;
        }
        r3 = r25;
        r4 = r28;
        r5 = 0x21;
        fn_80236BFC();
        r0 = r3 & 0xFF;

        r0 = r27 & 0xFFFF;

        r3 = 0x0;
        return;
    }
    r3 = r27;
    fn_80229B70();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = -0x1;
        return;
    }
    r3 = r27;
    fn_80229BD8();
    r0 = r3 & 0xFF;
    r3 = 0x1;
    if (r0 != (u32)0x1) return;
    r3 = -0x1;

    return;
}
