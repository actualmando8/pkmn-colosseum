/**
 * @file fight.c
 * @brief fightKouka + fightDataRewrite + fight core section -- split from colosseum_event.c (the fight
 *        engine bucket, 0x80202810-0x80211A00), address range
 *        0x8020D968-0x8020E4E8, 66 fns.
 *
 * fightKouka effect/target/condition data lookups, fightDataRewrite
 * (post-fight hero/pokedoru/mail/fade rewrite), and the FIGHT_WORK
 * fightSet / fightGet accessor farm. Corresponds to XD's fight.cpp
 * tail (0x8020C018-0x8020D858, capped by __sinit_fight_cpp).
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

/* SDA table pointers for event data arrays */
extern u32 lbl_80478D38;   /* Event table count */
extern ColosseumEventRow6 lbl_80478D30[]; /* Event table base (6 bytes per entry) */
extern u32 lbl_80478D28; /* Pair-row table count */
extern ColosseumEventPairRow lbl_80375A08[]; /* 0x18-byte pair rows */

/* 0x8020D968 | size: 0x38 | small */
void fn_8020D968(void* dst, void* src) {
    struct CopyBlk8020D968 { u32 data[12]; };
    if (dst == 0) return;
    if (src == 0) return;
    *(struct CopyBlk8020D968*)dst = *(struct CopyBlk8020D968*)src;
}

/* Address: 0x8020D9A0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightKoukaDataBiosGetKoukaDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x8020D9B8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightKoukaDataBiosGetFightTargetDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x8020D9D0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightKoukaDataBiosGetFightJoukenDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* fightKoukaDataBiosGetPtr | Size: 0x2C | Look up entry in 6-byte table (u16 index) */
void* fightKoukaDataBiosGetPtr(u16 index) {
    extern u8 lbl_80375CB8[];
    extern u32 lbl_80478D50;
    if (index >= lbl_80478D50) {
        return NULL;
    }
    return &lbl_80375CB8[index * 6];
}

/* Address: 0x8020DA14 | Size: 0xbc | Ghidra import */
u32 fightKoukaDoFightKoukaJoukenAndKouka(void)

{
    u32 r3;
    u32 r4;

    extern void fn_80136078();
    extern u32 fightTargetGetPtr();
    extern u32 fightFloorGetStatus();
    extern int fn_8020A8E0();
    extern u16 fightKoukaDataBiosGetKoukaDataId();
    extern u16 fightKoukaDataBiosGetFightTargetDataId();
    extern u16 fightKoukaDataBiosGetFightJoukenDataId();
    extern void* fightKoukaDataBiosGetPtr();
  u32 uVar1;
  u32 uVar2;
  u16 uVar3;
  u16 uVar4;
  s8 cVar5;
  
  fightKoukaDataBiosGetPtr(r4);
  uVar1 = fightKoukaDataBiosGetFightJoukenDataId();
  fightKoukaDataBiosGetPtr(r4);
  uVar2 = fightKoukaDataBiosGetFightTargetDataId();
  fightKoukaDataBiosGetPtr(r4);
  uVar3 = fightKoukaDataBiosGetKoukaDataId();
  uVar4 = fightFloorGetStatus(0,0,0x14,0);
  uVar2 = fightTargetGetPtr(uVar2,r3,uVar4);
  cVar5 = fn_8020A8E0(uVar1,r3);
  if (cVar5 == 1) {
    fn_80136078(uVar3,uVar2,r3,0);
  }
  return cVar5 == 1;
}

/* Address: 0x8020DAD0 | Size: 0x274 | Ghidra import */
#pragma push
#pragma peephole on
u32 fn_8020DAD0(u32 p1) {
    extern void _threadSwitch();
    extern u32 fn_800FF56C();
    extern void fn_800FF730();
    extern void fn_80112700();
    extern void floorSetFadeScript();
    extern void floorSetPrevFloorID();
    extern void fn_80113FE8();
    extern void fn_801140C8();
    extern void heroDecPokedoru();
    extern u32 heroGetStatus();
    extern void msgctrlSetValue();
    extern void scriptSoundStop();
    extern void fn_80165A20();
    extern void fn_80166AB8();
    extern void fn_8018DA88();
    extern u8 fn_801902E0();
    extern void fn_801903B0();
    extern void fn_80190528();
    extern void fadeCheck();
    extern void fadeSetEX(f32, f32, u32, u32, u32);
    extern void fadeSet();
    extern void fn_801D0AFC();
    extern void mailMainReceiveTerminate();
    extern void fn_801EF61C();
    extern void fn_801EF62C();
    extern u32 fn_801EF634();
    extern void fn_801EF7B4();
    extern u8 fightFloorIsGcHeroWin();
    extern void fightFloorSetStatus();
    extern u32 fightFloorGetStatus();
    extern u32 fn_801FCC7C();
    extern u32 fightTrainerDataBiosGetPtr();
    extern u16 fightEncountDataBiosGetWipeEffectSndID();
    extern f32 fightEncountDataBiosGetWipeEffectTime();
    extern u32 fightEncountDataBiosGetWipeSnapshotUse();
    extern u32 fightEncountDataBiosGetWipeFunction();
    extern u32 fightEncountWipeDataBiosGetPtr();
    extern u8 fightEncountDataBiosGetZenmetuFlag();
    extern void fightEncountDataBiosGetWipeId();
    extern u16 fightEncountDataBiosGetFightTrainerDataId();
    extern u32 fightEncountDataBiosGetFightFloorDataId();
    extern u8 fightEncountDataBiosGetFightKind();
    extern u32 fightEncountDataBiosGetPtr();
    extern u8 fightKindDataBiosGetPokemonStatusMenuSubbarFlag();
    extern u32 fightKindDataBiosGetPtr();
    extern u16 charNameBiosSearchIndex();
    extern u16 charNameBiosGetHearFlag();
    extern f32 lbl_8047E528;
    extern f32 lbl_8047E52C;

    u32 uVar1;
    u32 uVar2;
    u16 uVar7;
    u8 uVar9;
    u32 iVar3;
    u8 cVar10;
    u16 sVar8;
    u32 uVar4;
    u32 uVar5;
    u32 uVar6;

    if ((p1 & 0xffff) == 0) {
        uVar1 = 0;
    } else {
        uVar1 = fightEncountDataBiosGetPtr();
        fn_801EF62C(0);
        fn_801903B0(0x9b0);
        fn_801EF61C(p1);
        uVar2 = fn_800FF56C();
        fightFloorSetStatus(0, 0, 0x4a, 0, uVar2);
        uVar2 = fightEncountDataBiosGetFightFloorDataId(uVar1);
        uVar7 = fightFloorGetStatus(0, uVar2, 2, 0);
        mailMainReceiveTerminate();
        uVar9 = fightEncountDataBiosGetFightKind(uVar1);
        iVar3 = fightKindDataBiosGetPtr(uVar9);
        if ((iVar3 != 0) && (cVar10 = fightKindDataBiosGetPokemonStatusMenuSubbarFlag(), cVar10 != 0)) {
            fightEncountDataBiosGetFightTrainerDataId(uVar1, 1);
            iVar3 = fightTrainerDataBiosGetPtr();
            if (iVar3 != 0) {
                uVar2 = fn_801FCC7C();
                sVar8 = charNameBiosSearchIndex();
                if ((sVar8 != 0) && (sVar8 = charNameBiosGetHearFlag(), sVar8 != 0)) {
                    fn_80190528();
                }
                msgctrlSetValue(0x59, uVar2);
            }
        }
        fn_80165A20(1, 1000, 0xff);
        scriptSoundStop(1000);
        fightEncountDataBiosGetWipeId(uVar1);
        uVar2 = fightEncountWipeDataBiosGetPtr();
        uVar4 = fightEncountDataBiosGetWipeSnapshotUse();
        uVar5 = fightEncountDataBiosGetWipeFunction(uVar2);
        fadeSetEX(lbl_8047E528, fightEncountDataBiosGetWipeEffectTime(uVar2), 9, uVar5, uVar4);
        sVar8 = fightEncountDataBiosGetWipeEffectSndID(uVar2);
        if (sVar8 != 0) {
            fn_80166AB8(sVar8, 0, 0);
        }
        fn_801EF7B4();
        fn_800FF730(uVar7);
        floorSetFadeScript(0, 0);
        _threadSwitch();
        floorSetPrevFloorID(uVar7);
        cVar10 = fightEncountDataBiosGetZenmetuFlag(uVar1);
        if (cVar10 != 0) {
            uVar1 = fn_801EF634();
            cVar10 = fightFloorIsGcHeroWin(0, uVar1);
            if (cVar10 == 0) {
                fn_801EF61C(0);
                fn_801903B0(0xe05);
                uVar6 = heroGetStatus(0, 0xc, 0);
                heroDecPokedoru(0, ((s32)uVar6 >> 1) + (((s32)uVar6 < 0) & (uVar6 & 1)));
                fn_801D0AFC(1);
                fn_8018DA88();
                fn_80113FE8();
                floorSetFadeScript(0, 0x5960008);
                _threadSwitch();
                uVar1 = fn_801EF634();
                return uVar1;
            }
        }
        fn_80190528(0x9b0);
        fn_80112700();
        fn_801140C8();
        cVar10 = fn_801902E0(0xe05);
        if (cVar10 == 0) {
            fadeSet(lbl_8047E52C, 2);
            fadeCheck(1);
        }
        fn_801EF61C(0);
        uVar1 = fn_801EF634();
    }
    return uVar1;
}
#pragma pop

/* 0x8020DD44 | size: 0x3C | small */
void fightEncountGetEnvSndDataId(void) {
    extern void fightFloorGetStatus();
    extern u32 fightEncountDataBiosGetFightFloorDataId();
    extern void fightEncountDataBiosGetPtr();
    u32 val;
    fightEncountDataBiosGetPtr();
    val = fightEncountDataBiosGetFightFloorDataId();
    fightFloorGetStatus(0, val, 0x7, 0);
}

/* Address: 0x8020DD80 | Size: 0xd0 | Ghidra import */
int fightEncountGetBgmSndDataId(void)

{
    u32 r3;

    extern int fightFloorGetStatus();
    extern short fightTrainerGetStatus();
    extern int fightTrainerKindDataBiosGetBgmSndId();
    extern void fightTrainerKindDataBiosGetPtr();
    extern u32 fightEncountDataBiosGetBgmSndId();
    extern u16 fightEncountDataBiosGetFightTrainerDataId();
    extern u16 fightEncountDataBiosGetFightFloorDataId();
    extern u32 fightEncountDataBiosGetPtr();
  u32 uVar1;
  int iVar2;
  u32 uVar3;
  u32 uVar4;
  short sVar5;
  u16 uVar6;
  
  uVar1 = fightEncountDataBiosGetPtr();
  iVar2 = fightEncountDataBiosGetBgmSndId();
  if (iVar2 == 0) {
    fightEncountDataBiosGetPtr(r3);
    uVar3 = fightEncountDataBiosGetFightFloorDataId();
    iVar2 = fightFloorGetStatus(0,uVar3,6,0);
    if (iVar2 == 0) {
      for (uVar6 = 0; uVar6 < 4; uVar6 = uVar6 + 1) {
        uVar4 = fightEncountDataBiosGetFightTrainerDataId(uVar1,uVar6 & 0xff);
        if (((uVar4 & 0xffff) != 0) && (sVar5 = fightTrainerGetStatus(0,uVar4,4,0), sVar5 != 0)) {
          fightTrainerKindDataBiosGetPtr();
          iVar2 = fightTrainerKindDataBiosGetBgmSndId();
          if (iVar2 != 0) {
            return iVar2;
          }
        }
      }
      iVar2 = 1;
    }
  }
  return iVar2;
}

/* Address: 0x8020DE50 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightEncountDataBiosGetWipeEffectSndID(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* fightEncountDataBiosGetWipeEffectTime | Size: 0x18 | Get float from ptr+4, or default if NULL */
f32 fightEncountDataBiosGetWipeEffectTime(u8* ptr) {
    extern f32 lbl_8047E530;
    if (ptr == NULL) {
        return lbl_8047E530;
    }
    return *(f32*)(ptr + 0x4);
}

/* Address: 0x8020DE80 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightEncountDataBiosGetWipeSnapshotUse(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020DE98 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightEncountDataBiosGetWipeFunction(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* fightEncountWipeDataBiosGetPtr | Size: 0x28 | Look up entry in 12-byte table */
void* fightEncountWipeDataBiosGetPtr(u32 index) {
    extern u8 fight_encount_wipe_data[];
    extern u32 lbl_80478D20;
    if (index >= lbl_80478D20) {
        return NULL;
    }
    return &fight_encount_wipe_data[index * 12];
}

/* Address: 0x8020DED8 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightEncountDataBiosGetFightName(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020DEF0 | Size: 0x10 | Pattern: nullcheck_setter */
void fightEncountDataBiosSetSyoukaiWzxDataId(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}

/* Address: 0x8020DF00 | Size: 0x10 | Pattern: nullcheck_setter */
void fightEncountDataBiosSetBgmSndId(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* fightEncountDataBiosSetGSInputDevice | Size: 0x40 | Write u32 to slot in 8-byte array at offset 0x18 */
#pragma push
#pragma peephole on
void fightEncountDataBiosSetGSInputDevice(u8* base, u8 slot, u32 value) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return;
    }
    *(u32*)(entry + 0x4) = value;
}
#pragma pop

/* fightEncountDataBiosSetFightTrainerDataId | Size: 0x40 | Write u16 to slot in 8-byte array at offset 0x18 */
#pragma push
#pragma peephole on
void fightEncountDataBiosSetFightTrainerDataId(u8* base, u8 slot, u16 value) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return;
    }
    *(u16*)(entry) = value;
}
#pragma pop

/* Address: 0x8020DF90 | Size: 0x10 | Pattern: nullcheck_setter */
void fightEncountDataBiosSetFightFloorDataId(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x4]) = val;
}

/* Address: 0x8020DFA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fightEncountDataBiosSetTrainer(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}

/* Address: 0x8020DFB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fightEncountDataBiosSetFightKind(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x8020DFC0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightEncountDataBiosGetSyoukaiWzxDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}

/* Address: 0x8020DFD8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightEncountDataBiosGetZenmetuFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}

/* Address: 0x8020DFF0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightEncountDataBiosGetWipeId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x8020E008 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightEncountDataBiosGetBgmSndId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* fightEncountDataBiosGetGSInputDevice | Size: 0x48 | Read u32 from slot in 8-byte array at offset 0x18 */
#pragma push
#pragma peephole on
u32 fightEncountDataBiosGetGSInputDevice(u8* base, u8 slot) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return 0;
    }
    return *(u32*)(entry + 0x4);
}
#pragma pop

/* fightEncountDataBiosGetFightTrainerDataId | Size: 0x48 | Read u16 from slot in 8-byte array at offset 0x18 */
#pragma push
#pragma peephole on
u16 fightEncountDataBiosGetFightTrainerDataId(u8* base, u8 slot) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return 0;
    }
    return *(u16*)(entry);
}
#pragma pop

/* Address: 0x8020E0B0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightEncountDataBiosGetFightFloorDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x8020E0C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightEncountDataBiosGetTrainer(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* Address: 0x8020E0E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightEncountDataBiosGetFightKind(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* fightEncountDataBiosGetPtr | Size: 0x2C | Look up entry in 0x38-byte table (indirect) */
void* fightEncountDataBiosGetPtr(u16 index) {
    extern u32* lbl_80478F50;
    extern u8* lbl_80478F54;
    if (index >= *lbl_80478F50) {
        return NULL;
    }
    return lbl_80478F54 + index * 0x38;
}

/* 0x8020E124 | size: 0x80 | small */
void fightTypeGetFightSideFightOutPokemonMax(void) {
    extern u32 lbl_80478F00;
    extern u32 lbl_80478F04;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r4 = lbl_80478F00;
    r7 = r3 & 0xFFFF;
    r6 = *(u32*)((u8*)r4 + 0x0);
    if (r7 > r6) {
        r4 = 0x0;
    } else {

        r4 = lbl_80478F04;
        /* clrlslwi r0, r3, 16, 3 */;
        r4 = r4 + r0;
    }
    if (r4 == (u32)0x0) {
        r5 = 0x0;
    } else {

        r5 = *(u8*)((u8*)r4 + 0x0);
    }
    if (r7 > r6) {
        r4 = 0x0;
    } else {

        r4 = lbl_80478F04;
        /* clrlslwi r0, r3, 16, 3 */;
        r4 = r4 + r0;
    }
    r3 = r5 & 0xFF;
    if (r4 == (u32)0x0) {
        r0 = 0x0;
    } else {

        r0 = *(u8*)((u8*)r4 + 0x2);
    }
    r0 = r0 & 0xFF;
    r0 = r3 * r0;
    r3 = r0 & 0xFFFF;
    return;
}

/* Address: 0x8020E1A4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightTypeDataBiosGetFightoutPokemonNum(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}

/* Address: 0x8020E1BC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightTypeDataBiosGetEntryPokemonNum(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* Address: 0x8020E1D4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightTypeDataBiosGetTrainerNum(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020E1EC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightTypeDataBiosGetName(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* fightTypeDataBiosGetPtr | Size: 0x2C | Look up entry in 8-byte table (indirect) */
void* fightTypeDataBiosGetPtr(u16 index) {
    extern u32 lbl_80478F00;
    extern u32 lbl_80478F04;
    u32* countPtr = (u32*)lbl_80478F00;
    if (index > *countPtr) {
        return NULL;
    }
    return (u8*)lbl_80478F04 + (u32)index * 8;
}

/* Address: 0x8020E230 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetHostEnemyMsgFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x18]);
}

/* Address: 0x8020E248 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightKindDataBiosGetName(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x1C]);
}

/* Address: 0x8020E260 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetPokemonStatusMenuSubbarFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x17]);
}

/* Address: 0x8020E278 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDarkpokemonHypermodeFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x16]);
}

/* Address: 0x8020E290 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetMonohiroiFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x15]);
}

/* Address: 0x8020E2A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDorobouFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x14]);
}

/* Address: 0x8020E2C0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetBossFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x13]);
}

/* Address: 0x8020E2D8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetKeikentihueruFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x12]);
}

/* Address: 0x8020E2F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDoItemSoubiTokukoutokubouupFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}

/* Address: 0x8020E308 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDoHizukiMiyaburiFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}

/* Address: 0x8020E320 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDoHizukiAiFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}

/* Address: 0x8020E338 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDoCriticalAttackFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}

/* Address: 0x8020E350 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetGetInfectPokerusFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}

/* Address: 0x8020E368 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetGetFriendFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}

/* Address: 0x8020E380 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetGetNekoniKobanFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}

/* Address: 0x8020E398 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetGetOkaneFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}

/* Address: 0x8020E3B0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetOkanePoolFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA]);
}

/* Address: 0x8020E3C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetGetExpFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}

/* Address: 0x8020E3E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDrawFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}

/* Address: 0x8020E3F8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetNigeruFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}

/* Address: 0x8020E410 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetCallFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}

/* Address: 0x8020E428 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetUseItemFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}

/* Address: 0x8020E440 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDoZukanTukamaetaFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}

/* Address: 0x8020E458 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDoZukanMitaFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}

/* Address: 0x8020E470 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetDoBadgeCheckFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* fightKindDataBiosGetPtr | Size: 0x2C | Look up entry in 32-byte table (indirect) */
void* fightKindDataBiosGetPtr(u16 index) {
    extern u32 lbl_80478F40;
    extern u32 lbl_80478F44;
    u32* countPtr = (u32*)lbl_80478F40;
    if (index > *countPtr) {
        return NULL;
    }
    return (u8*)lbl_80478F44 + (u32)index * 32;
}

/* Address: 0x8020E4B4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightKindDataBiosGetBackSaveDataFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* 0x8020E4CC | size: 0x1C | tiny */
/* fightAbicntFitMinMax | Size: 0x1C | Clamp value to [0, 12] */
s32 fightAbicntFitMinMax(s32 value) {
    if (value < 0) {
        value = 0;
    }
    if (value > 12) {
        value = 12;
    }
    return value;
}
