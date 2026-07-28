/**
 * @file toolentry.c
 * @brief game/pxdvs/app/toolentry/toolentry.cpp -- split from colosseum_battle.c (the
 *        Colosseum battle-flow/AI bucket, 0x802405C0-0x80265EC4),
 *        address range 0x8025CD64-0x8025DBD4, 34 fns.
 *
 * XD source unit: game/pxdvs/app/toolentry/toolentry.cpp
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
int fightTrainerAiWazaValueKuroikiri(void* ctx, u32 param1, u32 param2, u32 param3);
void fightTrainerAiWazaValueHimitunotikara(void* ctx, u32 param1, u32 param2, u32 param3);
s32 fightTrainerAiSelectIrekaeDasuFightPokemon(void* ctx, u32 param1, u32 param2, u32 param3);
u32 fightTrainerAiWazaHit045(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType);
u32 fightMenuFightTrainerGcHeroOpenMenu(void* ctx, u32 param1, u32 param2);

typedef struct ToolentrySpeciesList {
    u16 speciesId[1];
} ToolentrySpeciesList;

/* Address: 0x8025CD64 | Size: 0x54 | Pattern: field_accessor */
void toolentryTaisenFreePokemonData(void* ctx, u32 slot, u32 param) {
    extern u32 lbl_8047B650;
    extern u32 fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    u32 handle;
    u32 result;
    handle = lbl_8047B650;
    if (handle != 0) {
        result = fn_800E202C(handle);
        if ((result & 0xFFFF) != 0) {
            fn_800E24B0(result);
            fn_800E209C(result);
        }
        lbl_8047B650 = 0;
    }
}

/* Address: 0x8025CDB8 | Size: 0x2B4 (692 bytes) */
void toolentryDebugPokemonCreate(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 lbl_80478D98;
    extern u32 lbl_8047B650;
    extern u32 lbl_8047B654;
    extern void fn_8006B09C();
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void GSmsgGetGSchar();
    extern void pokemonAllKaihuku();
    extern void pokemonSetCatchStatus();
    extern void pokemonCreate();
    extern void pokemonInit();
    extern void savedataGetStatus();
    extern void heroInit();
    extern void heroBiosGetPokemonPtr();
    extern void gamedataGetStatus();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = 0x0;
    r4 = 0x20;
    r0 = lbl_80478D98;
    lbl_8047B654 = r3;
    r3 = r0 * 0x138;
    r0 = r3 + 0x1f;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {
        fn_800E27B0();
    } else {

        r3 = 0x0;
    }
    lbl_8047B654 = r3;
    r30 = r3;
    r31 = 0x0;
    while (1) {
        r0 = lbl_80478D98;
        if ((s32)r31 >= (s32)r0) break;
        r3 = 0x0;
        r4 = 0x1;
        gamedataGetStatus();
        r0 = r31 + 0x1;
        r6 = r3;
        r3 = r30;
        r5 = 0xa;
        r4 = r0 & 0xFFFF;
        pokemonCreate();
        r3 = r31 + 0x1004;
        GSmsgGetGSchar();
        r9 = r3;
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8;
        r6 = 0x1;
        r7 = 0x0;
        r8 = 0x0;
        pokemonSetCatchStatus();
        r3 = r30;
        pokemonAllKaihuku();
        r30 = r30 + 0x138;
        r31 = r31 + 0x1;

    }
    r3 = lbl_8047B650;
    if (r3 != (u32)0x0) {
        fn_800E202C();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if (r3 != (u32)0x0) {
            fn_800E24B0();
            r3 = r30;
            fn_800E209C();
        }
        r0 = 0x0;
        lbl_8047B650 = r0;
    }
    r3 = 0x80;
    r4 = 0x20;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if (r3 != (u32)0x0) {
        fn_800E27B0();
    } else {

        r3 = 0x0;
    }
    lbl_8047B650 = r3;
    r31 = 0x0;
    do {
        r3 = r31;
        fn_8006B09C();
        r3 = r3 + 0xb44;
        heroInit();
        r30 = 0x0;
        do {
            r3 = r31;
            fn_8006B09C();
            r4 = r30 & 0xFFFF;
            r3 = r3 + 0xb44;
            heroBiosGetPokemonPtr();
            pokemonInit();
            r30 = r30 + 0x1;
        } while ((s32)r30 < (s32)0x6);
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x4);
    r30 = 0x0;
    do {
        r3 = r30;
        fn_8006B09C();
        r3 = r3 + 0x2c;
        heroInit();
        r31 = 0x0;
        do {
            r3 = r30;
            fn_8006B09C();
            r4 = r31 & 0xFFFF;
            r3 = r3 + 0x2c;
            heroBiosGetPokemonPtr();
            pokemonInit();
            r31 = r31 + 0x1;
        } while ((s32)r31 < (s32)0x6);
        r30 = r30 + 0x1;
    } while ((s32)r30 < (s32)0x4);
    r0 = 0x6;
    ctr_fn = (void(*)(void))r0;
    do {
    } while (--ctr != 0);
    r3 = 0x0;
    r4 = 0x2;
    savedataGetStatus();
    r30 = r3;
    r3 = 0x0;
    fn_8006B09C();
    r3 = r3 + 0xb44;
    if (r30 != (u32)0x0) {
        r4 = r30;
        r5 = 0xb18;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r29 = 0x0;
    r31 = 0x0;
    do {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            r4 = 0x2;
            savedataGetStatus();
            r30 = r3;
            r3 = r31;
            fn_8006B09C();
            r3 = r3 + 0xb44;
            if (r30 != (u32)0x0) {
                r4 = r30;
                r5 = 0xb18;
                memcpy((void*)r3, (const void*)r4, (u32)r5);
            }

        } else {
            r28 = 0x0;
            do {
                r0 = lbl_8047B654;
                r3 = r31;
                r30 = r0 + r29;
                fn_8006B09C();
                r4 = r28 & 0xFFFF;
                r3 = r3 + 0xb44;
                heroBiosGetPokemonPtr();
                if (r3 != (u32)0x0) {
                    r4 = r30;
                    r5 = 0x138;
                    memcpy((void*)r3, (const void*)r4, (u32)r5);
                }
                r28 = r28 + 0x1;
                r29 = r29 + 0x138;
            } while ((s32)r28 < (s32)0x6);
        }
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x4);
    r3 = lbl_8047B654;
    if (r3 != (u32)0x0) {
        fn_800E202C();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if (r3 != (u32)0x0) {
            fn_800E24B0();
            r3 = r30;
            fn_800E209C();
        }
        r0 = 0x0;
        lbl_8047B654 = r0;
    }
    return;
}

/* Address: 0x8025D06C | Size: 0x3c | Ghidra import */
u32 fn_8025D06C(void)
{
    extern u32 fn_8006ADEC();
    extern void fn_8006AFC4();
    extern void* fn_8006B5A8();
    extern void heroAddPokecoupon();
    u32 uVar1;
  fn_8006B5A8();
  fn_8006AFC4();
  uVar1 = fn_8006ADEC();
  heroAddPokecoupon(0,uVar1);
  return 0;
}


/* Address: 0x8025D0A8 | Size: 0xBC */
f32 fn_8025D0A8(void* ctx, u32 param1, u32 param2) {
    extern ToolentrySpeciesList* lbl_80478EAC;
    extern f32 lbl_8047E658;
    extern f32 lbl_8047E65C;
    extern u16 pokemonBiosGetPokemonDataId(void*);
    extern u8 pokemonCheckValid(void*);
    extern void* savedataGetStatus(u32, u32);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    u32 i;
    u32 count;
    void* member;
    void* party;
    ToolentrySpeciesList* speciesList;
    u32 offset;
    u16 idx;
    u16 species;
    u16 entry;
    f32 scale;
    f32 factor;

    count = 0;
    if ((party = ctx) == 0) {
        party = savedataGetStatus(0, 2);
    }
    for (i = 0; (s32)i < 6; i++) {
        member = heroBiosGetPokemonPtr(party, i & 0xFFFF);
        if (pokemonCheckValid(member) != 0) {
            species = pokemonBiosGetPokemonDataId(member);
            speciesList = lbl_80478EAC;
            offset = 0;
            while (1) {
                entry = speciesList->speciesId[offset];
                if (entry == 0) {
                    break;
                }
                if (species == entry) {
                    count++;
                }
                offset++;
            }
        }
    }
    scale = lbl_8047E658;
    factor = lbl_8047E65C;
    while ((s32)count > 0) {
        scale *= factor;
        count--;
    }
    return scale;
}

/* Address: 0x8025D164 | Size: 0x128 (296 bytes) */
void fn_8025D164(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8039A648[];
    extern u8 lbl_8039A664[];
    extern u32 lbl_80478EAC;
    extern f32 lbl_8047E658;
    extern f32 lbl_8047E65C;
    extern void fn_8006B09C();
    extern void fn_8006B5A8();
    extern void pokemonBiosGetPokemonDataId();
    extern void pokemonCheckValid();
    extern void heroBiosGetPokemonPtr();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    u32 r4 = param1;
    u32 r5 = param2;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = 0x0;
    fn_8006B5A8();
    r27 = *(u32*)((u8*)r3 + 0xC);
    fn_8006B5A8();
    r26 = *(u32*)((u8*)r3 + 0x0);
    fn_8006B5A8();
    r30 = *(u32*)((u8*)r3 + 0x14);
    r29 = 0x0;
    do {
        r3 = 0x0;
        fn_8006B09C();
        r4 = r29 & 0xFFFF;
        r3 = r3 + 0xb44;
        heroBiosGetPokemonPtr();
        r31 = r3;
        pokemonCheckValid();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r31;
            pokemonBiosGetPokemonDataId();
            r4 = lbl_80478EAC;
            r0 = r3 & 0xFFFF;
            r3 = 0x0;
            do {
                r5 = *(u16*)(r4 + r3);
                if (r5 == (u32)0x0) break;
                if (r0 == (u32)r5) {
                    r28 = r28 + 0x1;
                }
                r3 = r3 + 0x2;
            } while (1);
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    f1 = lbl_8047E658;
    f0 = lbl_8047E65C;
    ctr_fn = (void(*)(void))r28;
    if ((s32)r28 > (s32)0x0) {
        do {
            f1 = f1 * f0;
        } while (--ctr != 0);
    }
    if ((s32)r26 == (s32)0x1) {
        r3 = r30 + 0x1;
        r0 = 0xa;
        r0 = (s32)r3 / (s32)r0;
        if ((s32)r0 > (s32)0xa) {
            r0 = 0xa;
        }
        r3 = (u32)lbl_8039A664;
        r0 = r0 << 2;
        r3 = (u32)lbl_8039A664;
        f0 = *(f32*)(void*)(r3 + r0);
        f1 = f1 * f0;
    } else {

        if ((s32)r27 >= (s32)0x6) {
            r27 = 0x6;
        }
        r3 = (u32)lbl_8039A648;
        r0 = r27 << 2;
        r3 = (u32)lbl_8039A648;
        f0 = *(f32*)(void*)(r3 + r0);
        f1 = f1 * f0;
    }
    f0 = (f64)(s32)f1;
    *(f64*)(void*)(sp + 0x8) = f0;
    r3 = *(u32*)(sp + 0xC);
    return;
}

/* Address: 0x8025D28C | Size: 0x24 | Pattern: null_check_getter */
extern void* fn_8006B09C(void*);
u16 toolentryTaisenGetTrainerDataID(void* ctx) { return *(u16*)fn_8006B09C(ctx); }

/* Address: 0x8025D2B0 | Size: 0x24 | Pattern: null_check_getter */
u32 toolentryTaisenGetControlerType(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x24); }

/* Address: 0x8025D2D4 | Size: 0x90 */
u32 toolentryGetTrainerSamllFaceResID(void* ctx, u32 param1, u32 param2) {
    extern u32 lbl_80478E04;
    extern void* fn_8006B09C(void*);
    extern u32 fn_801FCBA4(void);
    extern void fightTrainerDataBiosGetPtr(u32);
    u32 id;
    u32 base;
    u32 offset;
    u32* entry;
    u32 ret;

    id = *(u16*)fn_8006B09C(ctx);
    if (id == 0) {
        return 0;
    }
    fightTrainerDataBiosGetPtr(id);
    offset = fn_801FCBA4();
    offset *= 0x14;
    base = lbl_80478E04;
    entry = (u32*)(base + offset);
    if ((s32)param1 == 0) {
        ret = entry[3];
        if (ret == 0) {
            return 0xf941200;
        }
        return ret;
    }
    ret = entry[4];
    if (ret == 0) {
        return 0xf8f1200;
    }
    return ret;
}

/* Address: 0x8025D364 | Size: 0x90 */
u32 toolentryGetTrainerBicFaceResID(void* ctx, u32 param1, u32 param2) {
    extern u32 lbl_80478E04;
    extern void* fn_8006B09C(void*);
    extern u32 fn_801FCBA4(void);
    extern void fightTrainerDataBiosGetPtr(u32);
    u16 id16;
    u32 id;
    u32 base;
    u32 offset;
    u32* entry;
    u32 ret;

    if ((s32)ctx != 0) {
        id16 = *(u16*)fn_8006B09C(ctx);
    } else {
        id16 = *(u16*)fn_8006B09C(ctx);
    }
    id = id16;
    if (id == 0) {
        return 0;
    }
    fightTrainerDataBiosGetPtr(id);
    offset = fn_801FCBA4();
    offset *= 0x14;
    base = lbl_80478E04;
    entry = (u32*)(base + offset);
    if ((s32)param1 == 0) {
        return entry[1];
    }
    ret = entry[2];
    if (ret == 0) {
        return 0xf991200;
    }
    return ret;
}

/* Address: 0x8025D3F4 | Size: 0x16C (364 bytes) */
void toolentryTaisenEntryPokemon(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void pokemonCheckValid();
    extern void heroBiosGetPokemonPtr();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r27 = r3;
    fn_8006B1D4();
    r30 = 0x0;
    r31 = r27;
    r29 = r30;
    r28 = r3 & 0xFFFF;
    do {
        r3 = r27;
        fn_8006B09C();
        r4 = r29 & 0xFFFF;
        r3 = r3 + 0xb44;
        heroBiosGetPokemonPtr();
        pokemonCheckValid();
        r3 = r3 & 0xFF;
        r0 = r3 - r0; /* -borrow */;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30 & 0xFFFF;
            r0 = r3 + 0x1;
            r30 = r0 & 0xFFFF;
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    r0 = r30 & 0xFFFF;
    if (r0 < r28) {
        r28 = r30;
    }
    r3 = r31;
    r30 = r28 & 0xFFFF;
    fn_8006B09C();
    r28 = r3;
    r3 = r31;
    fn_8006B09C();
    r0 = 0x163;
    r5 = r28 + 0x28;
    r4 = r3 + 0xb40;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u32*)((u8*)r4 + 0x4);
        r0 = *(u32*)((u8*)r4 + 0x8);
        *(u32*)((u8*)r5 + 0x4) = r3;
        r5 += 8; *(u32*)r5 = r0;
    } while (--ctr != 0);
    r27 = 0x0;
    r29 = 0x0;
    while ((s32)r27 < (s32)r30) {

        r3 = r31;
        fn_8006B09C();
        r0 = r29 + 0x8;
        r28 = *(u32*)(r3 + r0);
        r3 = r31;
        fn_8006B09C();
        r4 = r28 & 0xFFFF;
        r3 = r3 + 0xb44;
        heroBiosGetPokemonPtr();
        r28 = r3;
        r3 = r31;
        fn_8006B09C();
        r4 = r27 & 0xFFFF;
        r3 = r3 + 0x2c;
        heroBiosGetPokemonPtr();
        r3 = r31;
        fn_8006B09C();
        r4 = r27 & 0xFFFF;
        r3 = r3 + 0x2c;
        heroBiosGetPokemonPtr();
        if ((r3 != (u32)0x0) && (r28 != (u32)0x0)) {

            r0 = 0x27;
            ctr_fn = (void(*)(void))r0;
            do {
                r3 = *(u32*)((u8*)r4 + 0x4);
                r0 = *(u32*)((u8*)r4 + 0x8);
                *(u32*)((u8*)r5 + 0x4) = r3;
                r5 += 8; *(u32*)r5 = r0;
            } while (--ctr != 0);
        }
        r27 = r27 + 0x1;
        r29 = r29 + 0x4;

    }
    return;
}

/* Address: 0x8025D560 | Size: 0x24 | Pattern: null_check_getter */
u32 toolentryTaisengetEtnryPokemonOrderNum(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x20); }

/* Address: 0x8025D584 | Size: 0x5C | Pattern: field_accessor */
u32 toolentryTaisenDeleteEtnryPokemonOrder(void* ctx, u32 slot, u32 param) {
    typedef struct BattleFieldAccessor {
        u8 unk_00[8];
        u32 values[6];
        u32 count;
    } BattleFieldAccessor;
    extern BattleFieldAccessor* fn_8006B09C(void*);
    BattleFieldAccessor* entry;
    s32 index;

    entry = fn_8006B09C(ctx);
    index = entry->count - 1;
    if ((index < 0) || (index > 6)) {
        return 0;
    }
    entry->values[index] = (u32)-1;
    entry->count--;
    return entry->count;
}

/* Address: 0x8025D5E0 | Size: 0x64 | Pattern: field_accessor */
u32 toolentryTaisenSetEtnryPokemonOrderGBA(void* ctx, s32 count, u32* src) {
    typedef struct BattleFieldAccessor {
        u8 unk_00[8];
        u32 values[6];
        u32 count;
    } BattleFieldAccessor;
    extern BattleFieldAccessor* fn_8006B09C(void*);
    BattleFieldAccessor* dst;
    s32 i;

    dst = fn_8006B09C(ctx);
    for (i = 0; i < count; i++) {
        dst->values[i] = src[i];
    }
    dst->count = i;
    return i;
}

/* Address: 0x8025D644 | Size: 0x100 (256 bytes) */
s32 toolentryTaisenSetEtnryPokemonOrder(void* ctx, u32 order, u32 param2, u32 param3) {
    typedef struct BattleFieldAccessor {
        u8 unk_00[8];
        u32 values[6];
        u32 count;
    } BattleFieldAccessor;
    extern BattleFieldAccessor* fn_8006B09C(void*);
    extern u32 fn_8006B1D4(void*);
    extern u32 pokemonCheckValid(void*);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    BattleFieldAccessor* entry;
    u32 entryCount;
    u32 liveCount;
    u32 limit;
    u32 rawLimit;
    s32 i;
    u32 r0;
    u32 r3;

    entry = fn_8006B09C(ctx);
    entryCount = entry->count;
    rawLimit = fn_8006B1D4(entry);
    liveCount = 0;
    limit = rawLimit & 0xFFFF;
    i = liveCount;
    do {
        r3 = (u32)fn_8006B09C(ctx);
        r3 = (u32)heroBiosGetPokemonPtr((void*)(r3 + 0xb44), i & 0xFFFF);
        r3 = pokemonCheckValid((void*)r3);
        r3 = r3 & 0xFF;
        r0 = r3 != 0;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = liveCount & 0xFFFF;
            r0 = r3 + 1;
            liveCount = r0 & 0xFFFF;
        }
        i++;
    } while (i < 6);
    if ((liveCount & 0xFFFF) < limit) {
        limit = liveCount;
    }
    if ((s32)entry->count >= (s32)(limit & 0xFFFF)) {
        return -1;
    }
    for (i = 0; i < (s32)entryCount; i++) {
        if ((s32)entry->values[i] == (s32)order) {
            return -1;
        }
    }
    entry->values[entryCount] = order;
    entry->count++;
    return entryCount;
}

/* Address: 0x8025D744 | Size: 0x44 | Pattern: field_accessor */
u32 toolentryTaisenInitPokemonOrder(void* ctx, u32 slot, u32 param) {
    extern void* fn_8006B09C();
    u8* base;
    u32 i;
    base = (u8*)fn_8006B09C(ctx);
    *(u32*)(base + 0x20) = 0;
    for (i = 0; i < 6; i++) {
        *(u32*)(base + 0x8 + i * 4) = (u32)-1;
    }
    return (u32)base;
}

/* Address: 0x8025D788 | Size: 0x80 | Pattern: field_accessor */
void toolentryCopyHero(void* ctx, u32 slot, u32 param) {
    typedef struct {
        u32 words[0x2C6];
    } BattleCopyBlock;
    BattleCopyBlock* src;
    s32 i;
    BattleCopyBlock* dst;

    i = 0;
    do {
        src = (BattleCopyBlock*)((u8*)fn_8006B09C((void*)i) + 0xb44);
        dst = (BattleCopyBlock*)((u8*)fn_8006B09C((void*)i) + 0x2c);
        if ((src != NULL) && (dst != NULL)) {
            *dst = *src;
        }
        i++;
    } while ((s32)i < 4);
}

/* Address: 0x8025D808 | Size: 0x94 */
u32 toolentryTaisenGetEntryPokemonNum(void* ctx, u32 param1, u32 param2) {
    extern void* fn_8006B09C(void*);
    extern u32 fn_8006B1D4(void*);
    extern u32 pokemonCheckValid(void*);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    u32 r0;
    u32 r3;
    u32 r29;
    u32 r30;
    u32 r31;
    void* saved;

    saved = ctx;
    r3 = fn_8006B1D4(saved);
    r31 = r3 & 0xFFFF;
    r30 = 0;
    r29 = 0;
    do {
        r3 = (u32)fn_8006B09C(saved);
        r3 = (u32)heroBiosGetPokemonPtr((void *)(r3 + 0xb44), r29 & 0xFFFF);
        r3 = pokemonCheckValid((void *)r3);
        r3 = r3 & 0xFF;
        r0 = r3 != 0;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30 & 0xFFFF;
            r0 = r3 + 0x1;
            r30 = r0 & 0xFFFF;
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    r0 = r30 & 0xFFFF;
    r3 = r31;
    if (r0 < r31) {
        r3 = r30;
    }
    return r3;
}

/* Address: 0x8025D89C | Size: 0x78 | Pattern: field_accessor */
u32 toolentryTaisenGetPokemonNum(void* ctx, u32 slot, u32 param) {
    extern void* fn_8006B09C(void*);
    extern u32 pokemonCheckValid(void*);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r31 = 0;
    u32 r30 = 0;
    u32 r29 = 0;
    u32 r4 = slot;

    r30 = 0x0;
    r31 = r3;
    r29 = 0x0;
    do {
        r3 = (u32)fn_8006B09C((void *)r31);
        r4 = r29 & 0xFFFF;
        r3 = (u32)heroBiosGetPokemonPtr((void *)(r3 + 0xb44), r4);
        r3 = pokemonCheckValid((void *)r3);
        r3 = r3 & 0xFF;
        r0 = r3 != 0;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30 & 0xFFFF;
            r0 = r3 + 0x1;
            r30 = r0 & 0xFFFF;
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    return r30;
}

/* Address: 0x8025D914 | Size: 0x24 | Pattern: null_check_getter */
void* toolentryTaisenGetHeroPtr(void* ctx) { return (u8*)fn_8006B09C(ctx) + 0xb44; }

/* Address: 0x8025D938 | Size: 0x38 | Ghidra import */
u32 toolentryTaisenGetEntryPokemonPtr(u32 r3, u16 r4)
{
    extern void* fn_8006B09C();
    extern void heroBiosGetPokemonPtr();
    int iVar1;
  iVar1 = (int)fn_8006B09C();
  heroBiosGetPokemonPtr(iVar1 + 0x2c, r4);
}


/* Address: 0x8025D970 | Size: 0x38 | Ghidra import */
u32 toolentryTaisenGetPokemonPtr(u32 r3, u16 r4)
{
    extern void* fn_8006B09C();
    extern void heroBiosGetPokemonPtr();
    int iVar1;
  iVar1 = (int)fn_8006B09C();
  heroBiosGetPokemonPtr(iVar1 + 0xb44, r4);
}


/* Address: 0x8025D9A8 | Size: 0x24 | Pattern: null_check_getter */
extern void* fn_8006B5A8(void*);
u32 fn_8025D9A8(void* ctx) { return *(u32*)fn_8006B5A8(ctx); }

/* Address: 0x8025D9CC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D9CC(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x10); }

/* Address: 0x8025D9F0 | Size: 0x28 | Ghidra import */
u32 toolentryTaisenGetHomePlace(void)
{
    extern u32 fn_8006A7E8();
    extern void* fn_8006B09C();
    u16 uVar1;
  fn_8006B09C();
  uVar1 = fn_8006A7E8();
  return uVar1;
}


/* Address: 0x8025DA18 | Size: 0x24 | Pattern: accessor */
u32 toolentryTaisenGetBattlePlayerID(void* ctx) {
    extern void* fn_8006B09C(void*);
    extern u32 fn_8006A7D8(void);

    fn_8006B09C(ctx);
    return fn_8006A7D8();
}

/* toolentryTaisenGetEntryPlayerNum | Size: 0x4C | Get battle party size based on mode */
u32 toolentryTaisenGetEntryPlayerNum(void) {
    s32 mode;
    u32 res;
    extern void* fn_8006B5A8(void);
    void* result = fn_8006B5A8();
    res = 2;
    mode = *(s32*)((u8*)result + 0x4);
    switch (mode) {
        case 0:
        case 1:
            res = 2;
            break;
        case 2:
            res = 4;
            break;
    }
    return res;
}

/* Address: 0x8025DA88 | Size: 0x24 | Pattern: null_check_getter */
u32 toolentryTaisenGetBattleType(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x4); }

/* Address: 0x8025DAAC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAAC(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0xc); }

/* Address: 0x8025DAD0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAD0(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x8); }

/* Address: 0x8025DAF4 | Size: 0x38 | Ghidra import */
u32 fn_8025DAF4(void)
{
    extern void* fn_8006B5A8();
    u32 uVar1;
    uVar1 = (u32)fn_8006B5A8();
    if (*(u32 *)(uVar1 + 0x18) != 0) {
        *(u32 *)(uVar1 + 0x18) = *(u32 *)(uVar1 + 0x18) - 1;
    }
    return *(u32 *)(uVar1 + 0x18);
}


/* Address: 0x8025DB2C | Size: 0x30 | Ghidra import */
u32 fn_8025DB2C(void)
{
    extern void* fn_8006B5A8();
    int iVar1;
  iVar1 = (int)fn_8006B5A8();
  *(int *)(iVar1 + 0x18) = *(int *)(iVar1 + 0x18) + 1;
  return *(u32 *)(iVar1 + 0x18);
}


/* Address: 0x8025DB5C | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DB5C(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x18); }

/* Address: 0x8025DB80 | Size: 0x30 | Ghidra import */
u32 fn_8025DB80(void)
{
    extern void* fn_8006B5A8();
    int iVar1;
  iVar1 = (int)fn_8006B5A8();
  *(int *)(iVar1 + 0x14) = *(int *)(iVar1 + 0x14) + 1;
  return *(u32 *)(iVar1 + 0x14);
}


/* Address: 0x8025DBB0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DBB0(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x14); }
