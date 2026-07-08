/**
 * @file fight_trainer_ai2.c
 * @brief game/pxdvs/app/fight/fightTrainerAi2.cpp -- split from colosseum_battle.c (the
 *        Colosseum battle-flow/AI bucket, 0x802405C0-0x80265EC4),
 *        address range 0x8025C5A4-0x8025CD64, 11 fns.
 *
 * XD source unit: game/pxdvs/app/fight/fightTrainerAi2.cpp
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
extern void set__5GSvecFfff();
extern int  _fadeEffectGetRandom__FUl();
extern u32  pokemonBiosGetCatchTrainerRnd();
extern u32  pokemonBiosGetRnd();
extern u16  pokemonBiosGetPokemonDataId();
extern u32  savedataGetStatus();
extern int  fadeCheck();
extern int  fadeSet();
extern int  wazaSequenceSysRelease();
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

/* Address: 0x8025C5A4 | Size: 0xD0 (208 bytes) */
s32 fightTrainerAiCheckJoutaiKieWazaHitWazaDataId(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 wazaGetStatus();
    u32 val = wazaGetStatus(0, param2, 9, 0) & 0xFFFF;

    if ((u16)param1 == 0x1F &&
        (val == 0x92 || val == 0x95 || val == 0x98 || val == 0xCF)) {
        return 1;
    }
    if ((u16)param1 == 0x20 && val == 0x93) {
        return 1;
    }
    if ((u16)param1 == 0x21 && ((u16)param2 == 0x39 || (u16)param2 == 0xFA)) {
        return 1;
    }
    if (val == 0x5E) {
        return 1;
    }
    return 0;
}

/* Address: 0x8025C674 | Size: 0x48 | Pattern: field_accessor */
u32 fightTrainerAiCheckHorobinouta(void* ctx, u32 slot, u32 param) {
    extern u32 fightFloorLoopValidFightOutPokemon();
    extern void _fightTrainerAiCheckHorobinoutaSub();
    u32 result[2];
    result[0] = (u32)ctx;
    result[1] = 0;
    fightFloorLoopValidFightOutPokemon(0, (u32)_fightTrainerAiCheckHorobinoutaSub, (u32)result, 0);
    return result[1] & 0xFFFF;
}

/* Address: 0x8025C6BC | Size: 0xB4 */
void fn_8025C6BC(void* ctx, u32 param1, u32 param2) {
    extern void fightTrainerIsAllyFightTargetPtr();
    extern void fightOutPokemonCheckFightOut();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r29 = r5;
    r28 = r4;
    r31 = r3;
    r30 = *(u32*)((u8*)r5 + 0x0);
    fightOutPokemonCheckFightOut();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = r28;
    fightTrainerIsAllyFightTargetPtr();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x2b;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = *(u32*)((u8*)r29 + 0x4);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r29 + 0x4) = r0;
    }
    }
    r3 = 0x1;

    return;
}

/* fightTrainerAiCheckHorobinoutaSub's counter context: field0 (id passed to the
 * 3 gate checks below) at 0x0, running counter at 0x4. */
typedef struct HorobinoutaCtx {
    u32 field0;
    u32 counter;
} HorobinoutaCtx;

/*
 * _fightTrainerAiCheckHorobinoutaSub (0x8025C6BC)
 *
 * Perish Song (Horobinouta) counter callback, invoked per-floor-slot via
 * fightFloorLoopValidFightOutPokemon (see fightTrainerAiCheckHorobinouta). Runs 3 chained gate checks against
 * ctx->field0/floor/idx; only when ALL 3 fail does it increment
 * ctx->counter. Always returns 1 (the enumeration driver doesn't use the
 * return value -- the counter in ctx is the real output).
 */
#pragma optimize_for_size on
s32 _fightTrainerAiCheckHorobinoutaSub(void* floor, u32 idx, HorobinoutaCtx* ctx) {
    extern u8 fightOutPokemonCheckFightOut(void* floor, u32 idx, HorobinoutaCtx* ctx);
    extern u8 fightTrainerIsAllyFightTargetPtr(u32 fieldA, void* floor, u32 idx);
    extern u8 fn_80236BFC(u32 fieldA, void* floor, u32 flag);
    extern u8 fn_80237F74(u32 fieldA, void* floor, u32 flag);
    u32 r31;
    u32 r30;
    u32 r28;
    u32 r29;

    r29 = (u32)ctx;
    r28 = idx;
    r31 = (u32)floor;
    r30 = *(u32*)r29;
    if (!fightOutPokemonCheckFightOut((void*)r31, r28, (HorobinoutaCtx*)r29)) {
        return 1;
    }
    if (fightTrainerIsAllyFightTargetPtr(r30, (void*)r31, r28) == 1) {
        return 1;
    }
    if (fn_80236BFC(r30, (void*)r31, 0x1e) != 1 && fn_80237F74(r30, (void*)r31, 0x2b) != 1) {
        *(u32*)(r29 + 4) = *(u32*)(r29 + 4) + 1;
    }
    return 1;
}
#pragma optimize_for_size reset

/* Address: 0x8025C770 | Size: 0x98 */
u32 fightTrainerAiCheckTextureZokusei(void* ctx, u32 param1, u32 param2) {
    extern u32 fightTrainerGetStatus(void*, u32, u32, u32);
    extern u8 fn_8021B364(u32, void*);
    u8 out[0x10];
    u32 value;
    u32 result;

    value = fightTrainerGetStatus(ctx, 0, 0x43, 0) & 0xFFFF;
    value = fightTrainerGetStatus(0, value, 2, 0) & 0xFFFF;
    if ((u8)fightTrainerGetStatus(0, value, 0x2a, 0) == 1) {
        value = fn_8021B364(param1, out) & 0xFF;
        result = value >= 1;
        return result & 0xFF;
    }
    return 1;
}

/* Address: 0x8025C808 | Size: 0x2A0 (672 bytes) */
u32 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6) {
    extern void fightTargetGetPtrAsNowFightType();
    extern void fightSideIsJoutaiDataId();
    extern void fightTrainerGetStatus();
    extern void fightSeqCondChgActParaIdToValue();
    extern void fightSeqCondChgActTypeToPokemonStatusId();
    extern void fn_80229C28();
    extern void fn_80237F74();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = param4;
    u32 r8 = param5;
    u32 r9 = param6;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
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

    r0 = r9 & 0x00000040;
    r21 = r9;
    r22 = r3;
    r23 = r5;
    r24 = r6;
    r25 = r7;
    r26 = r8;
    r28 = 0x0;
    r27 = 0x0;
    if ((s32)r0 != (s32)0) {
        r30 = r4;
    } else {

        r30 = r23;
    }
    r4 = r30;
    r3 = 0x2;
    fightTargetGetPtrAsNowFightType();
    r31 = r21 & 0xFF;
    r21 = r3;
    r0 = r31 & 0xbf;
    r0 = r0 & 0x00000080;
    if ((s32)r0 != (s32)0) {
        r28 = 0x1;
    }
    r0 = r31 & 0x00000020;
    if ((s32)r0 != (s32)0) {
        r27 = 0x1;
    }
    r3 = r26;
    fightSeqCondChgActTypeToPokemonStatusId();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    r4 = 0x0;
    r5 = r29;
    r6 = 0x0;
    ((void(*)(void))pokemonGetStatus)();
    r26 = (s8)r3;
    r3 = r25;
    fightSeqCondChgActParaIdToValue();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r21;
        r4 = 0x4c;
        fightSideIsJoutaiDataId();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if (r0 == (u32)0x1) {
                r0 = r24 & 0xFFFF;
                if (r0 != (u32)0xae) {
                    r3 = 0x0;
                    return r3;
        }
        }
        }
        r0 = r24 & 0xFFFF;
        if (r0 == (u32)0xae || r0 == (u32)0x1 || r0 == (u32)0x1) {
            r0 = r27 & 0xFF;

            r3 = r22;
            r4 = 0x0;
            r5 = 0x43;
            r6 = 0x0;
            fightTrainerGetStatus();
            r4 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x2;
            r6 = 0x0;
            fightTrainerGetStatus();
            r4 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x24;
            r6 = 0x0;
            fightTrainerGetStatus();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r23;
                r4 = r24;
                fn_80229C28();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r0 = 0x1;
                    goto L_8025C96C;
            }
            }
            r0 = 0x0;
        L_8025C96C:
            r0 = r0 & 0xFF;

            r3 = 0x0;
            return r3;
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x1d;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r22;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1 || r0 != (u32)0x1 || r0 != (u32)0xae) {
            }
            r0 = r28 & 0xFF;

            r0 = r24 & 0xFFFF;

            r3 = 0x0;
            return r3;
            }
        r3 = r22;
        r4 = r30;
        r5 = 0x33;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if (r0 == (u32)0x1) {
                r0 = r29 & 0xFFFF;
                if (r0 == (u32)0xeb) {
                    r3 = 0x0;
                    return r3;
        }
        }
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x34;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if (r0 == (u32)0x1) {
                r0 = r29 & 0xFFFF;
                if (r0 == (u32)0xe6) {
                    r3 = 0x0;
                    return r3;
        }
        }
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r31 & 0x1F;
            if (r0 == (u32)0x1) {
                r3 = 0x0;
                return r3;
        }
        }
        r0 = (s8)r26;
        if (r0 > (u32)0x1) { r3 = 0x1; return r3; }
        r3 = 0x0;
        return r3;
    }
    if ((s32)r26 < (s32)0xc) { r3 = 0x1; return r3; }
    r3 = 0x0;
    return r3;

    r3 = 0x1;

    return r3;
}

/* Address: 0x8025CAA8 | Size: 0x94 */
u32 fightTrainerAiCheckGuard(void* ctx, u32 param1, u32 param2) {
    extern u32 fightTrainerGetStatus(void*, u32, u32, u32);
    extern u8 fn_80229C28(u32, u32);
    u32 value;

    value = fightTrainerGetStatus(ctx, 0, 0x43, 0) & 0xFFFF;
    value = fightTrainerGetStatus(0, value, 2, 0) & 0xFFFF;
    if ((u8)fightTrainerGetStatus(0, value, 0x24, 0) == 1) {
        if (fn_80229C28(param1, param2) == 1) {
            return 1;
        }
    }
    return 0;
}

/* Address: 0x8025CB3C | Size: 0xAC */
u16 fightTrainerAiCheckOumu(void* ctx, u32 param1, u32 param2) {
    extern u32 fn_800E0C54(void);
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u8 fightOutPokemonGetFightOutPokemonEnemyOumuWazaDataIdAry(u32, u16*);
    u16 choices[4];
    u32 species;
    s32 random;
    s32 index;
    u8 count;

    species = pokemonGetStatus(param1, 0, 0xF7, 0) & 0xFFFF;
    if ((species != 0) && (species != 0x165) && (species != 0xFFFF)) {
        return species;
    }
    count = fightOutPokemonGetFightOutPokemonEnemyOumuWazaDataIdAry(param1, choices);
    if (count != 0) {
        random = fn_800E0C54() & 0xFFFF;
        index = random % (s32)(u8)count;
        species = choices[(u8)index];
        if ((species != 0) && (species != 0x165)) {
            return species;
        }
    }
    return 0;
}

/* Address: 0x8025CBE8 | Size: 0x48 | Pattern: field_accessor */
u32 fightTrainerAiCheckSimerike(void* ctx, u32 slot, u32 param) {
    extern u32 fightFloorLoopValidFightOutPokemon();
    extern void _fightTrainerAiSimerikeCheckSub();
    u32 buf[2];
    u32 r;
    u32 s;
    buf[0] = (u32)ctx;
    r = fightFloorLoopValidFightOutPokemon(0, (u32)_fightTrainerAiSimerikeCheckSub, (u32)buf, 0) & 0xFF;
    s = 1 - r;
    return (s != 0) ? 1 : 0;
}

/* Address: 0x8025CC30 | Size: 0x60 | Pattern: field_accessor */
u32 _fightTrainerAiSimerikeCheckSub(void* ctx, u32 slot, u32* param) {
    extern u32 fightOutPokemonCheckFightOut(void);
    extern u32 fn_80237F74(u32, void*, u32);
    u32 r31;
    u32 r30;
    u32 result;

    r30 = (u32)ctx;
    r31 = *param;
    if ((fightOutPokemonCheckFightOut() & 0xFF) == 0) {
        return 1;
    }
    result = fn_80237F74(r31, (void*)r30, 6) & 0xFF;
    return result != 1;
}

/* Address: 0x8025CC90 | Size: 0x50 | Pattern: field_accessor */
u32 fightTrainerAiCheckSawagu(void* ctx, u32 slot, u32 param) {
    extern u32 fightFloorLoopValidFightOutPokemon();
    extern u32 _fightTrainerAiSawaguCheckSub();
    u32 buf[2];
    u32 r;
    buf[0] = (u32)ctx;
    buf[1] = slot;
    r = fightFloorLoopValidFightOutPokemon(0, (u32)_fightTrainerAiSawaguCheckSub, (u32)buf, 0) & 0xFF;
    return r != 1;
}

/* Address: 0x8025CCE0 | Size: 0x84 | Pattern: field_accessor */
u32 _fightTrainerAiSawaguCheckSub(void* ctx, u32 slot, u32 param) {
    extern u32 fightOutPokemonCheckFightOut(void);
    extern u32 fn_80236BFC(u32, void*, u32);
    extern u32 fn_80237F74(u32, u32, u32);
    u32* args;
    u32 a;
    u32 b;

    args = (u32*)param;
    a = args[0];
    b = args[1];
    if ((fightOutPokemonCheckFightOut() & 0xFF) == 0) {
        return 1;
    }
    if (((fn_80236BFC(a, ctx, 0xB) & 0xFF) == 1) &&
        ((fn_80237F74(a, b, 0x2B) & 0xFF) == 0)) {
        return 0;
    }
    return 1;
}
