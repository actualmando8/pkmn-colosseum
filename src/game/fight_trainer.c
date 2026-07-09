/**
 * @file fight_trainer.c
 * @brief fightTrainer section (fightTrainerEnemyPokemon cluster tail) -- split from colosseum_event.c (the fight
 *        engine bucket, 0x80202810-0x80211A00), address range
 *        0x8020E4E8-0x8020EE1C, 12 fns.
 *
 * fightTrainerEnemyPokemon Create/InitAry/Init/Erase/Regist/SearchAry/
 * CheckValid family plus the trainer action-buff/DB accessor head.
 * Corresponds to XD's fightTrainer section tail
 * (0x801FC350-0x802004EC, anchors at 0x80200288-0x80200424).
 */

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

typedef struct ColosseumEventRow6 {
    u8 mode;
    u8 field_01;
    u16 eventIndex;
    u16 nextIndex;
} ColosseumEventRow6;

typedef struct ColosseumEventSubRow {
    u8 valueMode;
    u8 scaleMode;
    s16 scaleNumerator;
    s16 scaleDenominator;
    u16 minValue;
    u16 maxValue;
} ColosseumEventSubRow;

typedef struct ColosseumEventPairRow {
    u8 resultFuncId;
    u8 field_01;
    u16 firstLinkIndex;
    ColosseumEventSubRow slots[2];
} ColosseumEventPairRow;

typedef struct FightAbicntRatio {
    u8 numerator;
    u8 denominator;
} FightAbicntRatio;

/* fightOutPokemonEnemy DB row (0xC bytes); fields accessed via the
 * fightOutPokemonEnemyBios* getter/setter family in fight_trainer_db_range_801FBD10.c */
typedef struct FightOutPokemonEnemyEntry {
    u32 targetFightOutPokemonPtr; /* 0x0 */
    u16 oumuWazaDataId;           /* 0x4 */
    u16 initHp;                   /* 0x6 */
    u16 damage;                   /* 0x8 */
} FightOutPokemonEnemyEntry;

/* =========================================================================
 * External declarations
 * ========================================================================= */

extern void* pokemonGetStatus();
extern u32   pokemonSetStatus();
extern void  pokemonGrowBasisStatus();
extern u32   itemGetStatus();
extern void  fn_80119ED0(void);
extern void  fn_80121ADC(void);
extern void  fn_8011B67C(void);
extern void  pokemonGetSoubiItemDataId(void);
extern void* fightActionGetPri(void* p);
extern void  wazaGetStatus(void);

/* Ability-count numerator/denominator table (16 entries) */
extern FightAbicntRatio lbl_80375D10[];
extern u32 lbl_80478D68; /* table entry count */

/* Address: 0x8020E4E8 | Size: 0x94 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightAbicntDoKakeWaru(u16 id, u32 val)
{
    FightAbicntRatio *p = &lbl_80375D10[id];
    u8 num;
    u8 den;

    if (id >= lbl_80478D68) {
        p = NULL;
    }
    num = (p == NULL) ? 0 : p->numerator;
    den = (p == NULL) ? 1 : p->denominator;

    return (val * num) / den;
}
#pragma pop

/* Address: 0x8020E57C | Size: 0x98 | Ghidra import */
#pragma push
#pragma peephole on
FightOutPokemonEnemyEntry* fightOutPokemonEnemySearchAry(FightOutPokemonEnemyEntry* ctx, u16 count, u32 matchVal)
{
    extern u32 fightOutPokemonEnemyBiosGetTargetFightOutPokemonPtr(FightOutPokemonEnemyEntry* ptr);
    FightOutPokemonEnemyEntry* entry;
    u16 i;

    if (ctx == NULL) {
        return NULL;
    }
    for (i = 0; i < count; i++) {
        entry = &ctx[i];
        {
            u8 valid = -fightOutPokemonEnemyBiosGetTargetFightOutPokemonPtr(entry) != 0;
            if (valid && fightOutPokemonEnemyBiosGetTargetFightOutPokemonPtr(entry) == matchVal) {
                return entry;
            }
        }
    }
    return NULL;
}
#pragma pop

/* fightOutPokemonEnemyCheckValid | Size: 0x2C | Check if fightOutPokemonEnemyBiosGetTargetFightOutPokemonPtr returns non-zero */
BOOL fightOutPokemonEnemyCheckValid(void) {
    extern s32 fightOutPokemonEnemyBiosGetTargetFightOutPokemonPtr(void);
    return -fightOutPokemonEnemyBiosGetTargetFightOutPokemonPtr() != 0;
}

/* Address: 0x8020E640 | Size: 0x94 | Ghidra import */
#pragma push
#pragma peephole on
void fightOutPokemonEnemyCreate(FightOutPokemonEnemyEntry* ctx, u32 pokemonSlot)
{
    extern void fightOutPokemonEnemyBiosSetDamage(FightOutPokemonEnemyEntry* ptr, u16 val);
    extern void fightOutPokemonEnemyBiosSetInitHp(FightOutPokemonEnemyEntry* ptr, u16 val);
    extern void fightOutPokemonEnemyBiosSetOumuWazaDataId(FightOutPokemonEnemyEntry* ptr, u16 val);
    extern void fightOutPokemonEnemyBiosSetTargetFightOutPokemonPtr(FightOutPokemonEnemyEntry* ptr, u32 val);
    extern u32 fightOutPokemonGetPokemonPtr(u32 slot);
    u32 pokePtr;
    u16 hp;

    if ((ctx != NULL) && (pokemonSlot != 0)) {
        fightOutPokemonEnemyBiosSetTargetFightOutPokemonPtr(ctx, 0);
        fightOutPokemonEnemyBiosSetOumuWazaDataId(ctx, 0);
        fightOutPokemonEnemyBiosSetInitHp(ctx, 0);
        fightOutPokemonEnemyBiosSetDamage(ctx, 0);
        fightOutPokemonEnemyBiosSetTargetFightOutPokemonPtr(ctx, pokemonSlot);
        pokePtr = fightOutPokemonGetPokemonPtr(pokemonSlot);
        hp = (u16)pokemonGetStatus(pokePtr, 0, 0x83, 0);
        fightOutPokemonEnemyBiosSetInitHp(ctx, hp);
    }
    return;
}
#pragma pop

/* Address: 0x8020E6D4 | Size: 0x84 | Ghidra import */
#pragma push
#pragma peephole on
void fightOutPokemonEnemyInitAry(FightOutPokemonEnemyEntry* ctx, u16 count)
{
    extern void fightOutPokemonEnemyBiosSetDamage(FightOutPokemonEnemyEntry* ptr, u16 val);
    extern void fightOutPokemonEnemyBiosSetInitHp(FightOutPokemonEnemyEntry* ptr, u16 val);
    extern void fightOutPokemonEnemyBiosSetOumuWazaDataId(FightOutPokemonEnemyEntry* ptr, u16 val);
    extern void fightOutPokemonEnemyBiosSetTargetFightOutPokemonPtr(FightOutPokemonEnemyEntry* ptr, u32 val);
    FightOutPokemonEnemyEntry* entry;
    u16 i;

    if (ctx != NULL) {
        for (i = 0; i < count; i++) {
            entry = &ctx[i];
            fightOutPokemonEnemyBiosSetTargetFightOutPokemonPtr(entry, 0);
            fightOutPokemonEnemyBiosSetOumuWazaDataId(entry, 0);
            fightOutPokemonEnemyBiosSetInitHp(entry, 0);
            fightOutPokemonEnemyBiosSetDamage(entry, 0);
        }
    }
    return;
}
#pragma pop

/* Address: 0x8020E758 | Size: 0x54 | Ghidra import */
#pragma push
#pragma peephole on
void fightOutPokemonEnemyInit(u32 r3)
{
    extern void fightOutPokemonEnemyBiosSetDamage();
    extern void fightOutPokemonEnemyBiosSetInitHp();
    extern void fightOutPokemonEnemyBiosSetOumuWazaDataId();
    extern void fightOutPokemonEnemyBiosSetTargetFightOutPokemonPtr();
    fightOutPokemonEnemyBiosSetTargetFightOutPokemonPtr(r3, 0);
    fightOutPokemonEnemyBiosSetOumuWazaDataId(r3, 0);
    fightOutPokemonEnemyBiosSetInitHp(r3, 0);
    fightOutPokemonEnemyBiosSetDamage(r3, 0);
}
#pragma pop

/* Address: 0x8020E7AC | Size: 0x1b0 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightTrainerEnemyPokemonEraseAry(void* ctx, u16 count, short matchVal)
{
    extern void fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag();
    extern void fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag();
    extern void fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetStoreTokuseiData();
    extern void fightTrainerEnemyPokemonBiosSetTokuseiFlag();
    extern void fn_801FBDF4();
    extern void fightTrainerEnemyPokemonBiosSetFightEntryeId();
    extern short fightTrainerEnemyPokemonBiosGetFightEntryeId();
    u8 bVar1;
    u32 uVar2;
    short sVar3;
    void* iVar4;
    u16 uVar6;
    u32 uVar5;

    if (ctx == NULL) {
        uVar2 = 0;
    }
    else if (matchVal < 0) {
        uVar2 = 0;
    }
    else {
        if (ctx == NULL) {
            iVar4 = NULL;
        }
        else {
            for (uVar6 = 0; uVar6 < count; uVar6 = uVar6 + 1) {
                iVar4 = (void*)((u32)ctx + (u32)uVar6 * 0x14);
                if (matchVal < 0) {
                    if (iVar4 == NULL) {
                        bVar1 = 0;
                    }
                    else {
                        sVar3 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4);
                        if (sVar3 < 0) {
                            bVar1 = 0;
                        }
                        else {
                            bVar1 = 1;
                        }
                    }
                    if (bVar1 != 0) {
                        continue;
                    }
                    goto LAB_0020b8ac;
                }
                else {
                    if (iVar4 == NULL) {
                        bVar1 = 0;
                    }
                    else {
                        sVar3 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4);
                        if (sVar3 < 0) {
                            bVar1 = 0;
                        }
                        else {
                            bVar1 = 1;
                        }
                    }
                    if ((bVar1) && (sVar3 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4), matchVal == sVar3)) goto LAB_0020b8ac;
                }
            }
            iVar4 = NULL;
        }
LAB_0020b8ac:
        if (iVar4 == NULL) {
            uVar2 = 0;
        }
        else {
            fightTrainerEnemyPokemonBiosSetFightEntryeId(iVar4, (void*)0xffffffff);
            for (uVar5 = 0; (uVar5 & 0xff) < 4; uVar5 = uVar5 + 1) {
                fn_801FBDF4(iVar4, uVar5, 0);
            }
            fightTrainerEnemyPokemonBiosSetTokuseiFlag(iVar4, 0);
            fightTrainerEnemyPokemonBiosSetStoreTokuseiData(iVar4, 0);
            fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag(iVar4, 0);
            fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag(iVar4, 0);
            fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag(iVar4, 0);
            fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag(iVar4, 0);
            fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag(iVar4, 0);
            uVar2 = 1;
        }
    }
    return uVar2;
}
#pragma pop

/* Address: 0x8020E95C | Size: 0x24c | Ghidra import */
#pragma push
#pragma peephole on
u32 fightTrainerEnemyPokemonRegistAry(void* ctx, u16 count, u32 matchVal)
{
    extern void fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag();
    extern void fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag();
    extern void fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetStoreTokuseiData();
    extern void fightTrainerEnemyPokemonBiosSetTokuseiFlag();
    extern void fn_801FBDF4();
    extern void fightTrainerEnemyPokemonBiosSetFightEntryeId();
    extern short fightTrainerEnemyPokemonBiosGetFightEntryeId();
    u8 bVar1;
    u32 uVar2;
    short sVar3;
    void* iVar4;
    u16 uVar6;
    u32 uVar5;

    if (ctx == NULL) {
        uVar2 = 0;
    }
    else {
        if ((short)matchVal < 0) {
            uVar2 = 0;
        }
        else {
            if (ctx == NULL) {
                iVar4 = NULL;
            }
            else {
                for (uVar6 = 0; uVar6 < count; uVar6 = uVar6 + 1) {
                    iVar4 = (void*)((u32)ctx + (u32)uVar6 * 0x14);
                    if ((short)matchVal < 0) {
                        if (iVar4 == NULL) {
                            bVar1 = 0;
                        }
                        else {
                            sVar3 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4);
                            if (sVar3 < 0) {
                            bVar1 = 0;
                        }
                        else {
                            bVar1 = 1;
                        }
                        }
                        if (bVar1 != 0) {
                            continue;
                        }
                        goto LAB_0020ba60;
                    }
                    else {
                        if (iVar4 == NULL) {
                            bVar1 = 0;
                        }
                        else {
                            sVar3 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4);
                            if (sVar3 < 0) {
                            bVar1 = 0;
                        }
                        else {
                            bVar1 = 1;
                        }
                        }
                        if ((bVar1) && (sVar3 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4), (short)matchVal == sVar3)) goto LAB_0020ba60;
                    }
                }
                iVar4 = NULL;
            }
LAB_0020ba60:
            if (iVar4 == NULL) {
                if (ctx == NULL) {
                    iVar4 = NULL;
                }
                else {
                    for (uVar6 = 0; uVar6 < count; uVar6 = uVar6 + 1) {
                        iVar4 = (void*)((u32)ctx + (u32)uVar6 * 0x14);
                        if (iVar4 == NULL) {
                            bVar1 = 0;
                        }
                        else {
                            sVar3 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4);
                            if (sVar3 < 0) {
                            bVar1 = 0;
                        }
                        else {
                            bVar1 = 1;
                        }
                        }
                        if (bVar1 == 0) goto LAB_0020bae0;
                    }
                    iVar4 = NULL;
                }
LAB_0020bae0:
                if (iVar4 == NULL) {
                    uVar2 = 0;
                }
                else {
                    if (-1 < (short)matchVal) {
                        fightTrainerEnemyPokemonBiosSetFightEntryeId(iVar4, (void*)0xffffffff);
                        for (uVar5 = 0; (uVar5 & 0xff) < 4; uVar5 = uVar5 + 1) {
                            fn_801FBDF4(iVar4, uVar5, 0);
                        }
                        fightTrainerEnemyPokemonBiosSetTokuseiFlag(iVar4, 0);
                        fightTrainerEnemyPokemonBiosSetStoreTokuseiData(iVar4, 0);
                        fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag(iVar4, 0);
                        fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag(iVar4, 0);
                        fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag(iVar4, 0);
                        fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag(iVar4, 0);
                        fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag(iVar4, 0);
                        fightTrainerEnemyPokemonBiosSetFightEntryeId(iVar4, matchVal);
                    }
                    uVar2 = 1;
                }
            }
            else {
                uVar2 = 0;
            }
        }
    }
    return uVar2;
}
#pragma pop

/* Address: 0x8020EBA8 | Size: 0xfc | Ghidra import */
#pragma push
#pragma peephole on
int fightTrainerEnemyPokemonSearchAry(void* p1, u16 p2, s16 p3) {
    extern s16 fightTrainerEnemyPokemonBiosGetFightEntryeId();
    u8 bVar1;
    s16 sVar2;
    void* iVar4;
    u16 uVar3;

    if (p1 == 0) return 0;
    for (uVar3 = 0; uVar3 < p2; uVar3++) {
        iVar4 = (void*)((u32)p1 + (u32)uVar3 * 0x14);
        if (p3 < 0) {
            if (iVar4 == 0) {
                bVar1 = 0;
            } else {
                sVar2 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4);
                if (sVar2 < 0) {
                    bVar1 = 0;
                } else {
                    bVar1 = 1;
                }
            }
            if (bVar1 == 0) {
                return (s32)iVar4;
            }
        } else {
            if (iVar4 == 0) {
                bVar1 = 0;
            } else {
                sVar2 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4);
                if (sVar2 < 0) {
                    bVar1 = 0;
                } else {
                    bVar1 = 1;
                }
            }
            if (bVar1 && p3 == (sVar2 = fightTrainerEnemyPokemonBiosGetFightEntryeId(iVar4))) {
                return (s32)iVar4;
            }
        }
    }
    return 0;
}
#pragma pop

/* 0x8020ECA4 | size: 0x3C | small */
#pragma push
#pragma peephole on
u32 fightTrainerEnemyPokemonCheckValid(void* obj) {
    extern s16 fightTrainerEnemyPokemonBiosGetFightEntryeId();
    s16 val;
    if (obj == 0) return 0;
    val = fightTrainerEnemyPokemonBiosGetFightEntryeId(obj);
    return (val >= 0) ? 1 : 0;
}
#pragma pop

/* Address: 0x8020ECE0 | Size: 0xdc | Ghidra import */
#pragma push
#pragma peephole on
void fightTrainerEnemyPokemonInitAry(void* ctx, u16 count) {
    extern void fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag();
    extern void fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag();
    extern void fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetStoreTokuseiData();
    extern void fightTrainerEnemyPokemonBiosSetTokuseiFlag();
    extern void fn_801FBDF4(void* entry, u8 idx, u32 zero);
    extern void fightTrainerEnemyPokemonBiosSetFightEntryeId();
    u8 j;
    void* entry;
    u16 i;
    if (ctx == NULL) { return; }
    for (i = 0; i < count; i++) {
        entry = (void*)((u32)ctx + (u32)i * 0x14);
        fightTrainerEnemyPokemonBiosSetFightEntryeId(entry, (void*)0xffffffff);
        for (j = 0; j < 4; j++) {
            fn_801FBDF4(entry, j, 0);
        }
        fightTrainerEnemyPokemonBiosSetTokuseiFlag(entry, 0);
        fightTrainerEnemyPokemonBiosSetStoreTokuseiData(entry, 0);
        fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag(entry, 0);
        fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag(entry, 0);
        fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag(entry, 0);
        fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag(entry, 0);
        fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag(entry, 0);
    }
}
#pragma pop

/* 0x8020EDBC | size: 0x60 */
#pragma push
#pragma peephole on
void fightTrainerEnemyPokemonInitFightOutStatus(void* ctx) {
    extern void fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag();
    extern void fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag();
    extern void fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag();
    extern void fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag();
    fightTrainerEnemyPokemonBiosSetNowhp1banhikuiFlag(ctx, 0);
    fightTrainerEnemyPokemonBiosSetLv1banhikuiFlag(ctx, 0);
    fightTrainerEnemyPokemonBiosSetDefense1banhikuiFlag(ctx, 0);
    fightTrainerEnemyPokemonBiosSetBadwazaHaveFlag(ctx, 0);
    fightTrainerEnemyPokemonBiosSetParam1bantakaiFlag(ctx, 0);
}
#pragma pop
