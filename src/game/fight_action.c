/**
 * @file fight_action.c
 * @brief fightAction section -- split from colosseum_event.c (the fight
 *        engine bucket, 0x80202810-0x80211A00), address range
 *        0x8020AE30-0x8020D968, 68 fns.
 *
 * Per-turn action dispatch: fightActionDisp / fightActionFlow state
 * machine driving one battle turn (kaisi/heijou/syuuryou/kaijou phases,
 * trainer call/item/nigeru/irekae sub-flows) plus the fightActionBios*
 * accessor farm they read/write. Corresponds to XD's fight.cpp
 * fightAction section (0x80208288-0x8020C018).
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
extern void* fn_801F0928(void* p);
extern void  wazaGetStatus(void);

/* SDA table pointers for event data arrays */
extern u32 lbl_80478D38;   /* Event table count */
extern ColosseumEventRow6 lbl_80478D30[]; /* Event table base (6 bytes per entry) */
extern u32 lbl_80478D28; /* Pair-row table count */
extern ColosseumEventPairRow lbl_80375A08[]; /* 0x18-byte pair rows */

/* Address: 0x8020AE30 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispWazaKiaipantiPre(void) { return 1; }

/* Address: 0x8020AE38 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispTenkouInit(void) { return 1; }

/* Address: 0x8020AE40 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispHeijou(void) { return 1; }

/* Address: 0x8020AE48 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispSyuuryouPost(void) { return 1; }

/* Address: 0x8020AE50 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispSyuuryou(void) { return 1; }

/* Address: 0x8020AE58 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispSyuuryouPre(void) { return 1; }

/* Address: 0x8020AE60 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispFightOutPokemonOutWaza(void) { return 1; }

/* Address: 0x8020AE68 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispFightTrainerUseItem(void) { return 1; }

/* Address: 0x8020AE70 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispFightTrainerCall(void) { return 1; }

/* Address: 0x8020AE78 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispFightOutPokemonIrekae(void) { return 1; }

/* Address: 0x8020AE80 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispFightNigeru(void) { return 1; }

/* Address: 0x8020AE88 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispOneTurnPost(void) { return 1; }

/* Address: 0x8020AE90 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispAllFightOutPokemonDoFightAction(void) { return 1; }

/* Address: 0x8020AE98 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispAllFightTrainerSelectFightAction(void) { return 1; }

/* Address: 0x8020AEA0 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispKaisiPost(void) { return 1; }

/* Address: 0x8020AEA8 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispKaisiPre(void) { return 1; }

/* Address: 0x8020AEB0 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispKaisiNyuujouPokemon(void) { return 1; }

/* Address: 0x8020AEB8 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispKaisiNyuujouTrainer(void) { return 1; }

/* Address: 0x8020AEC0 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispKaijou(void) { return 1; }

/* Address: 0x8020AEC8 | Size: 0x8 | Pattern: return_constant */
u32 fightActionDispNullFunc(void) { return 1; }

/* 0x8020AED0 | size: 0x60 */
#pragma push
#pragma peephole on
u32 fightActionFlowWazaKiaipantiPre(void* ctx) {
    extern void fn_801F4C14();
    extern u32 fightActionBiosGetBuffDataPtr();
    extern u32 fightActionBiosGetActorFightTargetPtr();
    extern void fn_80211B94();
    fn_801F4C14(0, 0, 0x36, 0, fightActionBiosGetActorFightTargetPtr(ctx));
    fn_80211B94(ctx, fightActionBiosGetBuffDataPtr(ctx), 0);
    return 1;
}
#pragma pop

/* Address: 0x8020AF30 | Size: 0xc4 | Ghidra import */
u32 fightActionFlowTenkouInit(void)

{
    u32 r3;

    extern u32 tenkouDataBiosGetFightInitMsgId();
    extern void fn_801F37B0();
    extern s8 fn_801F453C();
    extern void fn_801F4C14();
    extern u32 fightActionBiosGetBuffDataPtr();
    extern void fightKoukaDoFightKoukaJoukenAndKouka();
    extern void fn_80211B94();
  s8 cVar2;
  u32 uVar1;
  u32 local_18 [4];
  
  fightKoukaDoFightKoukaJoukenAndKouka(0,1);
  cVar2 = fn_801F453C(0,0);
  local_18[0] = 0;
  fn_801F37B0(0,0x8020aff4,local_18,0);
  if (cVar2 != 0) {
    fn_801F4C14(0,0,0x36,0,local_18[0]);
    uVar1 = tenkouDataBiosGetFightInitMsgId(cVar2);
    fn_801F4C14(0,0,0x50,0,uVar1);
    uVar1 = fightActionBiosGetBuffDataPtr(r3);
    fn_80211B94(r3,uVar1,0);
  }
  return 1;
}

/* Address: 0x8020AFF4 | Size: 0x5c | Ghidra import */
u32 _fightActionFlowTenkouInitSubGetSeqFightOutPokemonPtr__FPvUsPv(void)

{
    u32 r3;
    u32 r4;
    u32 *r5;

  int iVar1;
  u32 uVar2;
  
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 == 0) {
    uVar2 = 1;
  }
  else {
    if (r5 != (void *)0) {
      *r5 = r3;
    }
    uVar2 = 0;
  }
  return uVar2;
}

/* Address: 0x8020B050 | Size: 0x8 | Pattern: return_constant */
u32 fightActionFlowHeijou(void) { return 1; }

/* Address: 0x8020B058 | Size: 0x2d8 | Ghidra import */
u32 fightActionFlowSyuuryouPost(void)

{
    extern int fn_8006B0F8();
    extern s8 fn_8006B57C();
    extern s8 pokemonIsDarkPokemon();
    extern s8 pokemonCheckFightOut();
    extern void pokemonEvolutionAll();
    extern u32 pokemonEvolutionCheck();
    extern int savedataGetStatus();
    extern void heroCheckSetMonohiroiAllTemotiPokemon();
    extern u32 heroGetStatus();
    extern void heroBiosCopy();
    extern short fn_801EF634();
    extern void fn_801EFFC4();
    extern s8 fn_801F1DBC();
    extern int fn_801F2A7C();
    extern int fn_801F47B4();
    extern u32 fn_801F54A4();
    extern int fn_801F7258();
    extern void fightTrainerBackFightPokemonToTemotiPokemon();
    extern s8 fightTrainerCheckCanGetExp();
    extern int fightTrainerCheckTemotiPokemonFightEntry();
    extern int fightTrainerGetStatus();
    extern u32 fightPokemonCheckFightOut();
  u32 uVar1;
  short sVar6;
  int iVar2;
  int iVar3;
  u32 uVar4;
  s8 cVar7;
  int iVar5;
  u16 uVar9;
  u32 uVar8;
  u32 uVar10;
  u16 local_28 [2];
  u8 auStack_24 [8];
  
  uVar1 = fn_801F54A4(0,0,0x16,0);
  sVar6 = fn_801EF634();
  if (sVar6 != 1) {
    iVar2 = fn_801F2A7C(0);
    if ((iVar2 != 0) && (iVar3 = fightTrainerGetStatus(iVar2,0,0x44,0), iVar3 != 0)) {
      fightTrainerBackFightPokemonToTemotiPokemon(iVar2,0);
      uVar4 = fn_801EF634();
      cVar7 = fn_801F1DBC(0,uVar4);
      if (cVar7 == 1) {
        cVar7 = fn_801F54A4(0,0,0x24,0);
        if ((cVar7 == 1) && (cVar7 = fightTrainerCheckCanGetExp(iVar2), cVar7 == 1)) {
          for (uVar9 = 0; uVar9 < 6; uVar9 = uVar9 + 1) {
            uVar4 = heroGetStatus(iVar3,3,uVar9);
            cVar7 = pokemonCheckFightOut();
            if (((((cVar7 != 0) && (cVar7 = pokemonIsDarkPokemon(uVar4), cVar7 != 1)) &&
                 (iVar5 = fightTrainerCheckTemotiPokemonFightEntry(iVar2,uVar4), iVar5 != 0)) &&
                ((cVar7 = fightPokemonCheckFightOut(), cVar7 != 0 &&
                 (cVar7 = (int)pokemonGetStatus(iVar5,0,0xd0,0), cVar7 != 0)))) &&
               (uVar8 = pokemonEvolutionCheck(uVar4,0,0,local_28,auStack_24), (uVar8 & 0xffff) != 0)) {
              pokemonEvolutionAll(uVar4,uVar8,local_28[0],auStack_24,iVar3,1,1,0);
              fn_801EFFC4(10);
            }
          }
        }
        cVar7 = fn_801F54A4(0,0,0x30,0);
        if (cVar7 == 1) {
          heroCheckSetMonohiroiAllTemotiPokemon(iVar3);
        }
        fn_801F54A4(0,0,0x28,0);
      }
      cVar7 = fn_801F54A4(0,0,0x1c,0);
      if ((cVar7 == 1) && (iVar2 = savedataGetStatus(0,2), iVar2 != 0)) {
        heroBiosCopy(iVar2,iVar3);
      }
    }
    cVar7 = fn_8006B57C();
    if (cVar7 == 1) {
      for (uVar8 = 0; (uVar8 & 0xffff) < 2; uVar8 = uVar8 + 1) {
        iVar2 = fn_801F47B4(0,uVar8);
        if (iVar2 != 0) {
          for (uVar10 = 0; (uVar10 & 0xffff) < (uVar1 & 0xffff); uVar10 = uVar10 + 1) {
            iVar3 = fn_801F7258(iVar2,uVar10);
            if (iVar3 != 0) {
              fightTrainerBackFightPokemonToTemotiPokemon(iVar3,0);
              iVar5 = fn_8006B0F8(uVar10 + (uVar8 & 0xffff) * (uVar1 & 0xffff) & 0xff);
              if ((iVar5 != 0) && (iVar3 = fightTrainerGetStatus(iVar3,0,0x44,0), iVar3 != 0)) {
                heroBiosCopy(iVar5);
              }
            }
          }
        }
      }
    }
  }
  return 1;
}

/* Address: 0x8020B330 | Size: 0x3a4 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightActionFlowSyuuryou(void* ctx)
{
    extern u32 fn_800896B8();
    extern u32 fn_800896C0();
    extern void _threadSwitch();
    extern void fn_80132A38();
    extern void fn_80165668();
    extern u32 battleCameraIsSimple();
    extern void fn_801DA8C4();
    extern u8 fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
    extern void fn_801EF2D4();
    extern u32 fn_801EF634();
    extern void fn_801EF8F4();
    extern void fn_801F000C();
    extern u32 fn_801F025C();
    extern u8 fn_801F1DBC();
    extern u8 fn_801F54A4();
    extern u32 fn_801F8000();
    extern u32 fightTrainerGetNamePtr();
    extern u32 fightTrainerGetStatus();
    extern s16 fightActionDataBiosGetBuff();
    extern void fightActionBiosGetFightActionDataPtr();
    extern void fn_80211B94();
    extern u8 lbl_80378801[];
    extern u8 lbl_8037880F[];
    extern void fightMenuCloseMsg();
    extern void fightMenuOpenTrainerMsg();
    extern void fightMenuOpenMsg();
    u16 sVar9;
    u32 uVar1;
    u32 uVar2;
    u32 uVar4;
    u32 iVar5;
    u16 uVar3;
    u8 cVar10;
    u32 saved_r27;
    u32 iVar7;

    fightActionBiosGetFightActionDataPtr();
    sVar9 = fightActionDataBiosGetBuff();
    uVar1 = fn_801F025C(0xb, 0);
    uVar2 = fn_801F025C(9, uVar1);
    uVar3 = fightTrainerGetStatus(uVar2, 0, 0x43, 0);
    uVar4 = fightTrainerGetStatus(uVar2, 0, 0x4c, 0);
    iVar5 = fightTrainerGetStatus(0, uVar3, 8, 1);
    if (uVar3 == fn_800896B8()) {
        iVar7 = fn_800896C0();
        if (iVar7 == 0) {
            iVar7 = 0;
        } else {
            fn_80132A38(0x24, iVar7);
            iVar7 = 0x7531;
        }
    } else {
        if ((s32)fightTrainerGetStatus(uVar2, 0, 0x4a, 0) == 0) {
            iVar7 = fightTrainerGetStatus(0, uVar3, 8, 2);
        } else {
            iVar7 = fightTrainerGetStatus(0, uVar3, 8, 3);
            if (iVar7 == 0) {
                iVar7 = fightTrainerGetStatus(0, uVar3, 8, 2);
            }
        }
    }
    fn_80132A38(0x22, fn_801F8000(uVar2));
    fn_80132A38(0x23, fightTrainerGetNamePtr(uVar2));
    fn_80132A38(0x13, fightTrainerGetNamePtr(uVar1));
    fn_80132A38(0x25, fightTrainerGetNamePtr(uVar2));
    cVar10 = fn_801F54A4(0, 0, 0x33, 0);
    if (cVar10 == 1) {
        if (sVar9 == 2) {
            if (iVar7 != 0) {
                fn_801DDD28(uVar4, 0x5a, 4, 0);
                saved_r27 = battleCameraIsSimple();
            }
            fn_80165668(0x3f5, 0, 0xff);
            fn_80132A38(0x5d, 0);
            fightMenuOpenMsg(0x766c);
            fightMenuCloseMsg();
            if (iVar7 != 0) {
                fn_801DA9E8(uVar4, 0x5a, 4);
                fightMenuOpenTrainerMsg(iVar7);
                while (1) {
                    cVar10 = fn_801DA94C(uVar4, 0x5a, 4);
                    if (cVar10 == 0) break;
                    _threadSwitch();
                }
                fn_801EF8F4(saved_r27);
                fightMenuCloseMsg();
                fn_801DA8C4(uVar4, 0x5a, 4);
            }
        } else if (sVar9 == 3) {
            if (iVar5 != 0) {
                fn_801DDD28(uVar4, 0x59, 4, 0);
                saved_r27 = battleCameraIsSimple();
            }
            fightMenuOpenMsg(0x7547);
            fightMenuCloseMsg();
            if (iVar5 != 0) {
                fn_801DA9E8(uVar4, 0x59, 4);
                fightMenuOpenTrainerMsg(iVar5);
                while (1) {
                    cVar10 = fn_801DA94C(uVar4, 0x59, 4);
                    if (cVar10 == 0) break;
                    _threadSwitch();
                }
                fn_801EF8F4(saved_r27);
                fightMenuCloseMsg();
                fn_801DA8C4(uVar4, 0x59, 4);
            }
            fightMenuOpenMsg(0x7548);
            fightMenuCloseMsg();
        } else if ((1 < (u16)(sVar9 - 4U)) && ((sVar9 == 7 || (sVar9 == 6)))) {
            fightMenuOpenMsg(0x7640);
            fn_801F000C(0x40);
            fightMenuCloseMsg();
        }
    }
    uVar1 = fn_801EF634();
    cVar10 = fn_801F1DBC(0, uVar1);
    if ((cVar10 == 1) && (cVar10 = fn_801F54A4(0, 0, 0x25, 0), cVar10 == 1)) {
        fn_80211B94(ctx, (u32)lbl_80378801, 0);
        fn_80211B94(ctx, (u32)lbl_8037880F, 0);
    }
    fn_801EF2D4();
    return 1;
}
#pragma pop

/* Address: 0x8020B6D4 | Size: 0x58 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightActionFlowSyuuryouPre(void)
{
    extern void fn_8016597C();
    extern void fn_801F000C();
    extern u32 fn_801F54A4();
    u32 uVar1;

    uVar1 = fn_801F54A4(0, 0, 0x12, 0);
    if (uVar1 != 0) {
        fn_8016597C(1, 1000, 1000, 0xff);
        fn_801F000C(0x3c);
    }
    return 1;
}
#pragma pop

/* Address: 0x8020B72C | Size: 0x1e4 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightActionFlowFightOutPokemonOutWaza(void* ctx)
{
    extern void wazaSetStatus();
    extern u32 wazaGetStatus();
    extern u32 fightTargetGetRelativeHostSideFightTargetIdToTragetPtr();
    extern void fn_801F4C14();
    extern u32 fn_801F54A4();
    extern u16 fightOutPokemonGetMotoWazaDataId();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fightWazaSetUseWazaStatus();
    extern u8 fightWazaCheckValid();
    extern u32 fightActionBiosGetActorFightTargetPtr();
    extern void fn_802128D0();
    extern u32 fn_8022B2CC();
    u16 uVar6;
    u32 uVar1;
    u32 uVar3;
    u32 uVar2;
    u8 cVar8;
    u16 uVar7;
    u16 uVar4;
    s8 uVar9;
    u32 uVar5;

    uVar6 = fn_801F54A4(0, 0, 0x14, 0);
    uVar1 = fightActionBiosGetActorFightTargetPtr(ctx);
    uVar2 = (u32)pokemonGetStatus((void*)uVar1, 0, 0xd9, 0);
    cVar8 = fightWazaCheckValid();
    if (cVar8 == 0) {
        uVar1 = 0;
    } else {
        uVar7 = wazaGetStatus((void*)uVar2, 0, 0x29, 0);
        uVar2 = fightTargetGetRelativeHostSideFightTargetIdToTragetPtr(uVar7, uVar6);
        fn_801F4C14(0, 0, 0x36, 0, uVar1);
        fn_801F4C14(0, 0, 0x42, 0, uVar2);
        uVar2 = fightOutPokemonGetPokemonPtr(uVar1);
        uVar3 = (u32)pokemonGetStatus((void*)uVar1, 0, 0xd9, 0);
        uVar4 = fightOutPokemonGetMotoWazaDataId(uVar1);
        uVar9 = wazaGetStatus((void*)uVar3, 0, 0x26, 0);
        cVar8 = wazaGetStatus((void*)uVar3, 0, 0x32, 0);
        if (cVar8 == 0) {
            uVar5 = (u32)pokemonGetStatus((void*)uVar2, 0, 0x7f, (u8)uVar9);
            if ((uVar5 & 0xffff) != (uVar4 & 0xffff)) {
                uVar4 = (u16)(u32)pokemonGetStatus((void*)uVar2, 0, 0x7f, (u8)uVar9);
                wazaSetStatus((void*)uVar3, 0, 0x27, 0, uVar4);
                fightWazaSetUseWazaStatus((void*)uVar3, uVar4);
                uVar1 = fn_8022B2CC(uVar1, uVar4, uVar6, 0, 1, 0, (void*)0xffffffff);
                fn_801F4C14(0, 0, 0x43, 0, uVar1);
            }
        }
        fn_802128D0(ctx, uVar4);
        uVar1 = 1;
    }
    return uVar1;
}
#pragma pop

/* 0x8020B910 | size: 0x104 */
#pragma push
#pragma peephole on
u32 fightActionFlowFightTrainerUseItem(void* ctx) {
    extern u32 fightTargetGetRelativeHostSideFightTargetIdToTragetPtr();
    extern void fn_801F4C14();
    extern u16 fn_801F54A4();
    extern u32 fightActionBiosGetActorFightTargetPtr();
    extern void fn_80211E18();
    u32 d908val;
    void* e5Data;
    u16 field1E;
    u32 partyCount;
    u8 slotType;
    u32 finalVal;
    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    d908val = fightActionBiosGetActorFightTargetPtr(ctx);
    fn_801F4C14(0, 0, 0x36, 0, d908val);
    e5Data = pokemonGetStatus((void*)d908val, 0, 0xE5, 0);
    field1E = (u16)itemGetStatus((u32)e5Data, 0, 0x1E, 0);
    slotType = (u8)itemGetStatus(0, field1E, 0x2, 0);
    if (slotType == 1) {
        finalVal = (u32)fightTargetGetRelativeHostSideFightTargetIdToTragetPtr((u16)itemGetStatus((u32)e5Data, 0, 0x1F, 0), partyCount);
    } else {
        finalVal = d908val;
    }
    fn_801F4C14(0, 0, 0x42, 0, finalVal);
    fn_80211E18(ctx, field1E);
    return 1;
}
#pragma pop

/* 0x8020BA14 | size: 0x6c */
#pragma push
#pragma peephole on
u32 fightActionFlowFightTrainerCall(void* ctx) {
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern u32 fightActionBiosGetActorFightTargetPtr();
    extern void fn_80212D6C();
    u32 d908val;
    fn_801F54A4(0, 0, 0x14, 0);
    d908val = fightActionBiosGetActorFightTargetPtr(ctx);
    fn_801F4C14(0, 0, 0x36, 0, d908val);
    fn_80212D6C(ctx);
    return 1;
}
#pragma pop

/* Address: 0x8020BA80 | Size: 0x78 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightActionFlowFightOutPokemonIrekae(void* ctx)
{
    extern void fn_801F4C14();
    extern u32 fightActionBiosGetBuffDataId();
    extern u32 fightActionBiosGetActorFightTargetPtr();
    extern void fn_80213158();
  u32 uVar1;
  short sVar2;

  uVar1 = fightActionBiosGetActorFightTargetPtr();
  fn_801F4C14(0,0,0x45,0,uVar1);
  sVar2 = fightActionBiosGetBuffDataId(ctx);
  pokemonSetStatus(uVar1,0,0x121,0,(int)sVar2);
  fn_80213158(ctx);
  return 1;
}
#pragma pop

/* 0x8020BAF8 | size: 0xAC */
void fightActionFlowFightNigeru(void* ctx) {
    extern u8 fightTargetIsHostSide();
    extern u8 fn_801F3984();
    extern void fn_801F4C14();
    extern u16 fn_801F54A4();
    extern u32 fightActionBiosGetActorFightTargetPtr();
    extern void fightSeqSpecificationActionCounterInit();
    u16 tableId;
    u32 obj;
    u8 result;
    tableId = fn_801F54A4(NULL, 0, 0x14, 0);
    obj = fightActionBiosGetActorFightTargetPtr(ctx);
    fightSeqSpecificationActionCounterInit(obj);
    if (fightTargetIsHostSide(obj, tableId) == 1) {
        result = fn_801F3984(0, 4);
    } else {
        result = fn_801F3984(0, 5);
    }
    if (result == 1) {
        fn_801F4C14(0, 0, 0x44, 0, obj);
    }
}

/* Address: 0x8020BBA4 | Size: 0x58 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightActionFlowOneTurnPost(void* ctx) {
    extern u16 fn_801EF634();
    extern void fn_801F000C();
    extern void fn_801F4AC0();
    extern void fightSeqPost();
    u16 sVar1;
    sVar1 = fn_801EF634();
    if (sVar1 != 0) { return 1; }
    fightSeqPost(ctx);
    fn_801F000C(5);
    fn_801F4AC0(0);
    return 1;
}
#pragma pop

/* 0x8020BBFC | size: 0x98 */
#pragma push
#pragma peephole on
u32 fightActionFlowAllFightOutPokemonDoFightAction(void* ctx) {
    extern u16 fn_801EF634();
    extern void fn_801F3B24();
    extern void fn_801F4718();
    extern u32 _fightActionFlowAllFightOutPokemonDoFightActionOneLoop__FP11FIGHT_FLOORUc();
    extern void fn_80211A00();
    u32 uVar1;
    u16 sVar3;
    u32 uVar2;
    fn_801F4718(0);
    fn_801F3B24(0, 1);
    uVar1 = _fightActionFlowAllFightOutPokemonDoFightActionOneLoop__FP11FIGHT_FLOORUc(0, 0);
    if ((u8)uVar1 != 1) { return uVar1; }
    sVar3 = fn_801EF634();
    if (sVar3 != 0) { return 1; }
    fn_80211A00(ctx);
    uVar2 = _fightActionFlowAllFightOutPokemonDoFightActionOneLoop__FP11FIGHT_FLOORUc(0, 1);
    uVar1 = 1;
    if ((u8)uVar2 != 1) {
        uVar1 = uVar2;
    }
    return uVar1;
}
#pragma pop

/* Address: 0x8020BC94 | Size: 0x1a4 | Ghidra import */
u32 _fightActionFlowAllFightOutPokemonDoFightActionOneLoop__FP11FIGHT_FLOORUc(void)

{
    u32 r3;
    char r4;

    extern short fn_801EF634();
    extern short fn_801F0898();
    extern void fn_801F0F04();
    extern s8 fn_801F1170();
    extern void fn_801F4AC0();
    extern int fn_801F54A4();
    extern u8 fightOutPokemonCheckFightOut();
  u32 *puVar1;
  u32 uVar2;
  int iVar3;
  s8 cVar7;
  int iVar4;
  short sVar6;
  int iVar5;
  u32 *puVar8;
  u32 *puVar9;
  u32 uVar10;
  u32 uStack_4c;
  u32 local_48 [13];
  
  uVar10 = 0;
  do {
    if (7 < (uVar10 & 0xffff)) {
      return 1;
    }
    iVar3 = fn_801F54A4(r3,0,0x59,uVar10);
    if (iVar3 != 0) {
      cVar7 = fightOutPokemonCheckFightOut();
      if (cVar7 == 0) {
        pokemonSetStatus(iVar3,0,0x112,0,1);
      }
      else {
        iVar4 = (int)pokemonGetStatus(iVar3,0,0xfe,0);
        if (iVar4 == 0) {
          pokemonSetStatus(iVar3,0,0x112,0,1);
        }
        else {
          cVar7 = fn_801F1170();
          if (cVar7 == 0) {
            pokemonSetStatus(iVar3,0,0x112,0,1);
          }
          else if (r4 == 0) {
            sVar6 = fn_801F0898(iVar4);
            if (sVar6 == 8) {
LAB_00208d8c:
              iVar5 = (int)pokemonGetStatus(iVar3,0,0x112,0);
              if (iVar5 != 1) {
                pokemonSetStatus(iVar3,0,0x112,0,1);
                puVar9 = &uStack_4c;
                puVar8 = (u32 *)(iVar4 + -4);
                iVar3 = 6;
                do {
                  puVar1 = puVar8 + 1;
                  puVar8 = puVar8 + 2;
                  uVar2 = *puVar8;
                  puVar9[1] = *puVar1;
                  puVar9 = puVar9 + 2;
                  *puVar9 = uVar2;
                  iVar3 = iVar3 + -1;
                } while (iVar3 != 0);
                fn_801F0F04(local_48);
                if (r4 != 0) {
                  fn_801F4AC0(0);
                  sVar6 = fn_801EF634();
                  if (sVar6 != 0) {
                    return 1;
                  }
                }
              }
            }
          }
          else {
            sVar6 = fn_801F0898(iVar4);
            if (sVar6 != 8) goto LAB_00208d8c;
          }
        }
      }
    }
    uVar10 = uVar10 + 1;
  } while (1);
}

/* 0x8020BE38 | size: 0x108 */
u32 fightActionFlowAllFightTrainerSelectFightAction(void) {
    extern u8 fn_80008174();
    extern void fn_801F2B5C();
    extern void* fn_801F47B4();
    extern u16 fn_801F54A4();
    extern void* fn_801F7258();
    extern u8 fightMenuFightTrainerGcHeroOpenMenu();
    extern u32 _fightActionFlowFightTrainerSelectFightAction__FPvUsPv();
    u8 checkResult;
    u16 partyCount;
    u16 slotCount;
    u16 i;
    u16 j;
    void* slotData;
    void* entry;

    checkResult = fn_80008174();
    if (checkResult != 1) {
        fn_801F2B5C(0, (u32)_fightActionFlowFightTrainerSelectFightAction__FPvUsPv, 0, 1);
    } else {
        partyCount = fn_801F54A4(0, 0, 0x14, 0);
        slotCount = fn_801F54A4(0, 0, 0x16, 0);
        for (i = 0; i < 2; i++) {
            slotData = fn_801F47B4(0, i);
            if (slotData == NULL) { continue; }
            for (j = 0; j < slotCount; j++) {
                entry = fn_801F7258(slotData, j);
                if (entry == NULL) { continue; }
                if ((u8)fightMenuFightTrainerGcHeroOpenMenu(entry, partyCount, checkResult) != 0) { continue; }
                if (i == 0) { continue; }
                i--;
                break;
            }
        }
    }
    return 1;
}

/* 0x8020BF40 | size: 0x60 */
#pragma push
#pragma peephole on
u32 _fightActionFlowFightTrainerSelectFightAction__FPvUsPv(void* ctx, u32 param) {
    extern u16 fn_801EF634();
    extern void fightFloorSetTuusinErrorFightResult();
    extern u8 fightTrainerSelectFightAction();
    if (fn_801EF634() != 0) {
        return 1;
    }
    if (fightTrainerSelectFightAction(ctx, param) == 0) {
        fightFloorSetTuusinErrorFightResult(0);
    }
    return 1;
}
#pragma pop

/* 0x8020BFA0 | size: 0x120 */
#pragma push
#pragma peephole on
#pragma optimization_level 1
u32 fightActionFlowKaisiPost(void* ctx) {
    extern u8 lbl_80375CC8[];
    extern u8 lbl_80378AA0[];
    extern u16 fn_800E0C54();
    extern void fn_801DA7AC();
    extern void fn_801F2F3C();
    extern void fn_801F3074();
    extern void fn_801F3178();
    extern void fn_801F37B0();
    extern void fn_801F3B24();
    extern void fn_801F4718();
    extern void fn_801F4C14();
    extern u32 fightActionBiosGetFightActionDataPtr();
    extern void fightSeqInit();
    extern void fightSeqFightActionCreateAndFlowFifo();
    extern void fn_8022E1C4();
    extern void fn_8022E314();
    extern s32 _fightActionFlowKaisiPostSubFightOutPokemonSoubiItemCheckAppear__FPvUsPv();
    extern s32 _fightActionFlowKaisiPostSubFightOutPokemonTokuseiCheckAppear__FPvUsPv();
    extern s32 _fightActionFlowKaisiPostSubFightOutPokemonDarkCheckAppear__FPvUsPv();
    u8 localBuf[0x10];

    fn_801F4718(0);
    fn_801F3B24(0, 0);
    fightSeqInit();
    localBuf[0] = 0;
    fn_801F37B0(0, (u32)_fightActionFlowKaisiPostSubFightOutPokemonDarkCheckAppear__FPvUsPv, &localBuf[0], 0);
    fightSeqFightActionCreateAndFlowFifo(fightActionBiosGetFightActionDataPtr(ctx), 0, 6, 0, lbl_80375CC8, lbl_80378AA0);
    fn_801F37B0(0, (u32)_fightActionFlowKaisiPostSubFightOutPokemonTokuseiCheckAppear__FPvUsPv, 0, 1);
    fn_8022E314(1);
    fn_8022E1C4();
    fn_801F37B0(0, (u32)_fightActionFlowKaisiPostSubFightOutPokemonSoubiItemCheckAppear__FPvUsPv, 0, 1);
    localBuf[0] = 1;
    fn_801F37B0(0, (u32)_fightActionFlowKaisiPostSubFightOutPokemonDarkCheckAppear__FPvUsPv, &localBuf[0], 0);
    fn_801F3178(0);
    fn_801F3074(0);
    fn_801F2F3C(0);
    fn_801F4C14(0, 0, 0x5B, 0, (u32)fn_800E0C54());
    fn_801DA7AC();
    return 1;
}
#pragma pop

/* 0x8020C0C0 | size: 0x24 | small */
/* _fightActionFlowKaisiPostSubFightOutPokemonSoubiItemCheckAppear__FPvUsPv | Size: 0x24 | Call fn_8022D084 and return 1 */
#pragma push
#pragma peephole on
s32 _fightActionFlowKaisiPostSubFightOutPokemonSoubiItemCheckAppear__FPvUsPv(void) {
    extern void fn_8022D084(void);
    fn_8022D084();
    return 1;
}
#pragma pop

/* 0x8020C0E4 | size: 0x24 | small */
/* _fightActionFlowKaisiPostSubFightOutPokemonTokuseiCheckAppear__FPvUsPv | Size: 0x24 | Call fn_8022E410 and return 1 */
#pragma push
#pragma peephole on
s32 _fightActionFlowKaisiPostSubFightOutPokemonTokuseiCheckAppear__FPvUsPv(void) {
    extern void fn_8022E410(void);
    fn_8022E410();
    return 1;
}
#pragma pop

/* _fightActionFlowKaisiPostSubFightOutPokemonDarkCheckAppear__FPvUsPv | Size: 0x54 | Apply effect with optional data parameter */
s32 _fightActionFlowKaisiPostSubFightOutPokemonDarkCheckAppear__FPvUsPv(void* ctx, u32 unused, u8* data) {
    extern void fn_8022E6F0(void* ctx, u32 value);
    if (data != NULL) {
        fn_8022E6F0(ctx, data[0]);
    } else {
        fn_8022E6F0(ctx, 0);
        fn_8022E6F0(ctx, 1);
    }
    return 1;
}

/* Address: 0x8020C15C | Size: 0x6e4 | Ghidra import */
u32 fightActionFlowKaisiPre(void)

{
    extern void _threadSwitch();
    extern void menuGetKeyInfo();
    extern void fn_80132A38();
    extern void fn_80165A20();
    extern s8 fadeCheck();
    extern void fadeSet();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern s8 fn_801DA94C();
    extern void fn_801DA9B4();
    extern void fn_801DA9E8();
    extern void fn_801EF7C4();
    extern u32 fn_801F025C();
    extern s8 fn_801F1888();
    extern u32 fn_801F54A4();
    extern u32 fn_801F8000();
    extern u32 fightTrainerGetNamePtr();
    extern u32 fightTrainerGetStatus();
    extern u32 fightEncountDataBiosGetSyoukaiWzxDataId();
    extern void* fightEncountDataBiosGetPtr();
    extern void fightMenuCloseMsg();
    extern void fightMenuOpenTrainerMsg();
    extern u16 lbl_8047B5F8;
    extern f32 lbl_8047E520;
  u32 bVar1;
  u32 bVar2;

  u16 uVar12;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u16 uVar13;
  u32 uVar6;
  u32 uVar7;
  u32 uVar8;
  int iVar9;
  u8 cVar14;
  int iVar10;
  u32 uVar11;

  u16 local_74 [14];
  u16 local_58 [14];
  u16 local_3c [14];
  
  bVar2 = 0;
  uVar12 = fn_801F54A4(0,0,0xe,0);
  uVar3 = fn_801F025C(0xb,0);
  uVar4 = fightTrainerGetStatus(uVar3,0,0x4c,0);
  uVar3 = fn_801F025C(9,uVar3);
  uVar5 = fightTrainerGetStatus(uVar3,0,0x4c,0);
  uVar13 = fightTrainerGetStatus(uVar3,0,0x43,0);
  uVar6 = fn_801F54A4(0,0,0x10,0);
  uVar12 = fn_801F54A4(0,uVar12,0xd,0);
  fightEncountDataBiosGetPtr(uVar12);
  uVar7 = fightEncountDataBiosGetSyoukaiWzxDataId();
  uVar8 = fightTrainerGetStatus(uVar3,uVar13,7,0);
  if (uVar8 == 0) {
    uVar8 = 0x5f;
  }
  iVar9 = fightTrainerGetStatus(0,uVar13,8,0);
  cVar14 = fn_801F1888(0);
  if (cVar14 == 0) {
    if (uVar6 != 0) {
      if (uVar7 != 0) {
        fn_801DA9E8(uVar5,uVar7 & 0xffff,4);
      }
      fn_801DA9E8(uVar5,uVar6 & 0xffff,4);
      while (1) {
        if (0) {
          bVar1 = 0;
        }
        else {
          menuGetKeyInfo(local_3c,1);
          cVar14 = fadeCheck(0);
          bVar1 = bVar2;
          if ((cVar14 == 0) && ((local_3c[0] & 0x20) != 0)) {
            bVar1 = 1;
            bVar2 = bVar1;
          }
        }
        if (bVar1) goto LAB_00209430;
        cVar14 = fn_801DA94C(uVar5,uVar6 & 0xffff,4);
        if (cVar14 == 0) break;
        _threadSwitch();
      }
      cVar14 = fn_801F54A4(0,0,0x33,0);
      if (cVar14 == 1) {
        fn_801DA9E8(uVar5,lbl_8047B5F8,4);
        while (1) {
          if (0) {
            bVar1 = 0;
          }
          else {
            menuGetKeyInfo(local_58,1);
            cVar14 = fadeCheck(0);
            bVar1 = bVar2;
            if ((cVar14 == 0) && ((local_58[0] & 0x20) != 0)) {
              bVar1 = 1;
              bVar2 = bVar1;
            }
          }
          if (bVar1) goto LAB_00209430;
          cVar14 = fn_801DA94C(uVar5,lbl_8047B5F8,4);
          if (cVar14 == 0) break;
          _threadSwitch();
        }
      }
      if (uVar7 != 0) {
        while (1) {
          if (0) {
            bVar1 = 0;
          }
          else {
            menuGetKeyInfo(local_74,1);
            cVar14 = fadeCheck(0);
            bVar1 = bVar2;
            if ((cVar14 == 0) && ((local_74[0] & 0x20) != 0)) {
              bVar1 = 1;
              bVar2 = bVar1;
            }
          }
          if ((bVar1) || (cVar14 = fn_801DA94C(uVar5,uVar7 & 0xffff,4), cVar14 == 0)) break;
          _threadSwitch();
        }
      }
    }
LAB_00209430:
    if (bVar2) {
      fadeSet((double)lbl_8047E520,3);
      fadeCheck(1);
      if (uVar7 != 0) {
        fn_801DA9B4(uVar5,uVar7 & 0xffff,4);
      }
      fn_801DA9B4(uVar5,uVar6 & 0xffff,4);
      cVar14 = fn_801F54A4(0,0,0x33,0);
      if (cVar14 == 1) {
        fn_801DA9B4(uVar5,lbl_8047B5F8,4);
      }
    }
    if (iVar9 != 0) {
      fn_801DA9E8(uVar5,0x5f,4);
      if (bVar2) {
        fadeSet((double)lbl_8047E520,2);
        bVar2 = 0;
      }
      fightMenuOpenTrainerMsg(iVar9);
      while (cVar14 = fn_801DA94C(uVar5,0x5f,4), cVar14 != 0) {
        _threadSwitch();
      }
      fightMenuCloseMsg();
    }
    iVar10 = fn_801F54A4(0,0,0x11,0);
    if (iVar10 != 0) {
      fn_80165A20(iVar10,0,0xff);
    }
    cVar14 = fn_801F54A4(0,0,0x33,0);
    if (cVar14 == 1) {
      uVar4 = fn_801F8000(uVar3);
      fn_80132A38(0x22,uVar4);
      uVar3 = fightTrainerGetNamePtr(uVar3);
      fn_80132A38(0x23,uVar3);
      fn_801DA9E8(uVar5,uVar8 & 0xffff,4);
      if (bVar2) {
        fadeSet((double)lbl_8047E520,2);
        bVar2 = 0;
      }
      fightMenuOpenTrainerMsg(0x766d);
      while (cVar14 = fn_801DA94C(uVar5,uVar8 & 0xffff,4), cVar14 != 0) {
        _threadSwitch();
      }
      fightMenuCloseMsg();
    }
    if (bVar2) {
      fadeSet((double)lbl_8047E520,2);
    }
    if (uVar6 != 0) {
      if (uVar7 != 0) {
        fn_801DA8C4(uVar5,uVar7 & 0xffff,4);
      }
      fn_801DA8C4(uVar5,uVar6 & 0xffff,4);
      cVar14 = fn_801F54A4(0,0,0x33,0);
      if (cVar14 == 1) {
        fn_801DA8C4(uVar5,lbl_8047B5F8,4);
      }
    }
    if (iVar9 != 0) {
      fn_801DA8C4(uVar5,0x5f,4);
    }
    cVar14 = fn_801F54A4(0,0,0x33,0);
    if (cVar14 == 1) {
      fn_801DA8C4(uVar5,uVar8 & 0xffff,4);
    }
  }
  else {
    fn_801EF7C4(0);
    fn_801DA4E8(uVar4,1);
    fn_801DA9E8(uVar4,0x54,4);
    while (cVar14 = fn_801DA94C(uVar4,0x54,4), cVar14 != 0) {
      _threadSwitch();
    }
    fn_801EF7C4(0);
    fn_801DA4E8(uVar5,1);
    fn_801DA9E8(uVar5,0x55,4);
    cVar14 = fn_801F54A4(0,0,0x33,0);
    if (cVar14 == 1) {
      uVar11 = fn_801F8000(uVar3);
      fn_80132A38(0x22,uVar11);
      uVar3 = fightTrainerGetNamePtr(uVar3);
      fn_80132A38(0x23,uVar3);
      fightMenuOpenTrainerMsg(0x766d);
    }
    while (cVar14 = fn_801DA94C(uVar5,0x55,4), cVar14 != 0) {
      _threadSwitch();
    }
    cVar14 = fn_801F54A4(0,0,0x33,0);
    if (cVar14 == 1) {
      fightMenuCloseMsg();
    }
    fn_801EF7C4(1);
    fn_801DA9E8(uVar4,0x56,4);
    while (cVar14 = fn_801DA94C(uVar4,0x56,4), cVar14 != 0) {
      _threadSwitch();
    }
    fn_801DA8C4(uVar4,0x54,4);
    fn_801DA8C4(uVar5,0x55,4);
    fn_801DA8C4(uVar4,0x56,4);
    iVar9 = fn_801F54A4(0,0,0x11,0);
    if (iVar9 != 0) {
      fn_80165A20(iVar9,0,0xff);
    }
  }
  return 1;
}

/* Address: 0x8020CA98 | Size: 0x548 | Ghidra import */
u32 fightActionFlowKaisiNyuujouPokemon(void)

{
    extern void fn_8010AE2C();
    extern u32 pokemonCreateSequence();
    extern void fn_80132A38();
    extern void battleGridUpdate();
    extern void battleGridAddPokemon();
    extern u32 fn_801F02AC();
    extern void fn_801F4C14();
    extern u32 fn_801F54A4();
    extern int fn_801F7258();
    extern u32 fn_801F7388();
    extern s8 fn_801F7404();
    extern u32 fn_801F8000();
    extern u32 fightTrainerGetNamePtr();
    extern int fightTrainerGetValidFightOutPokemonPtr();
    extern u32 fightTrainerGetDoFightOutFightOutPokemonCount();
    extern int fightTrainerGetStatus();
    extern void fightTrainerBallThrowEffect();
    extern s8 fightOutPokemonIsGcHeroFightOutPokemon();
    extern void fightOutPokemonSetOnDarkPokemonFlag();
    extern void fightOutPokemonSetOnZukanFlag();
    extern void* fightPokemonGetPokemonPtr();
    extern u32 fightPokemonCheckFightOut();
    extern void fightOutPokemonCreate();
    extern void fightOutPokemonRegWzxLoad();
    extern void fightOutPokemonDasuEffect();
    extern void _fightActionFlowKaisiNyuujouPokemonSubAppearMsg__FP13FIGHT_TRAINERP15FightOutPokemonUsUsUsUsUc();
    extern u32 fightActionDataBiosGetBuff();
    extern u32 fightActionBiosGetFightActionDataPtr();
    extern void fightMenuCloseMsg();
    extern void fn_8026532C();
    extern void fn_80265598();
    u32 saved_r26 = 0;
  u16 uVar8;
  u16 uVar9;
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  s8 cVar10;
  u8 uVar11;
  int iVar4;
  u32 uVar5;
  u8 uVar12;
  u32 uVar6;
  int iVar7;
  u32 uVar13;
  u32 uVar14;
  u32 uVar15;

  fightActionBiosGetFightActionDataPtr();
  uVar8 = fightActionDataBiosGetBuff();
  uVar9 = fn_801F54A4(0,0,0x14,0);
  uVar1 = fn_801F54A4(0,0,0x18,0);
  uVar1 = uVar1 & 0xffff;
  uVar2 = fn_801F54A4(0,0,0x16,0);
  uVar2 = uVar2 & 0xffff;
  uVar3 = fn_801F02AC(uVar8,0,uVar9);
  cVar10 = fn_801F7404();
  if (cVar10 == 0) {
    uVar3 = 0;
  }
  else {
    uVar11 = fn_801F7388(uVar3);
    uVar13 = 0;
    while (1) {
      if (uVar2 <= (uVar13 & 0xffff)) break;
      iVar7 = fn_801F7258(uVar3,uVar13);
      if ((iVar7 != 0) && (iVar4 = fightTrainerGetStatus(iVar7,0,0x4c,0), iVar4 != 0)) {
        fightTrainerGetDoFightOutFightOutPokemonCount(iVar7);
        uVar15 = 0;
        uVar14 = 0;
        while ((((uVar14 & 0xffff) < 6 && ((uVar15 & 0xffff) < uVar1)) && ((uVar15 & 0xffff) < 2)))
        {
          uVar6 = fightTrainerGetStatus(iVar7,0,0x45,uVar14);
          cVar10 = fightPokemonCheckFightOut();
          if (cVar10 != 0) {
            fn_8010AE2C(uVar6,0,0);
            fightPokemonGetPokemonPtr(uVar6);
            uVar5 = pokemonCreateSequence();
            saved_r26 = fightTrainerGetStatus(iVar7,0,0x46,uVar14);
            fightOutPokemonCreate(saved_r26,uVar6,uVar5);
            uVar15 = uVar15 + 1;
            fightOutPokemonRegWzxLoad(saved_r26);
            cVar10 = fn_801F54A4(0,0,0x1e,0);
            if ((cVar10 == 1) && (cVar10 = fightOutPokemonIsGcHeroFightOutPokemon(saved_r26), cVar10 == 0)) {
              fightOutPokemonSetOnZukanFlag(saved_r26,0);
              fightOutPokemonSetOnDarkPokemonFlag(saved_r26,0);
            }
            battleGridAddPokemon(iVar4,uVar5);
          }
          uVar14 = uVar14 + 1;
        }
      }
      uVar13 = uVar13 + 1;
    }
    uVar13 = 0;
    while (1) {
      if (uVar2 <= (uVar13 & 0xffff)) break;
      iVar7 = fn_801F7258(uVar3,uVar13);
      if (iVar7 != 0) {
        uVar14 = 0;
        while (1) {
          if ((uVar1 <= (uVar14 & 0xffff)) ||
             (saved_r26 = fightTrainerGetValidFightOutPokemonPtr(iVar7,uVar14), saved_r26 != 0)) break;
          uVar14 = uVar14 + 1;
        }
        fightTrainerBallThrowEffect(iVar7,saved_r26,0);
        uVar14 = 0;
        while (1) {
          if (uVar1 <= (uVar14 & 0xffff)) break;
          saved_r26 = fightTrainerGetValidFightOutPokemonPtr(iVar7,uVar14);
          if (saved_r26 != 0) {
            fightOutPokemonDasuEffect(saved_r26,0);
          }
          uVar14 = uVar14 + 1;
        }
      }
      uVar13 = uVar13 + 1;
    }
    uVar13 = 0;
    while (1) {
      if (uVar2 <= (uVar13 & 0xffff)) break;
      iVar7 = fn_801F7258(uVar3,uVar13);
      if (iVar7 != 0) {
        uVar12 = fightTrainerGetDoFightOutFightOutPokemonCount();
        uVar14 = 0;
        while (1) {
          if (uVar1 <= (uVar14 & 0xffff)) break;
          saved_r26 = fightTrainerGetValidFightOutPokemonPtr(iVar7,uVar14);
          if (saved_r26 != 0) {
            _fightActionFlowKaisiNyuujouPokemonSubAppearMsg__FP13FIGHT_TRAINERP15FightOutPokemonUsUsUsUsUc(iVar7,saved_r26,uVar11,uVar12,uVar13,uVar14,0);
          }
          uVar14 = uVar14 + 1;
        }
        uVar14 = 0;
        while (1) {
          if ((uVar1 <= (uVar14 & 0xffff)) ||
             (saved_r26 = fightTrainerGetValidFightOutPokemonPtr(iVar7,uVar14), saved_r26 != 0)) break;
          uVar14 = uVar14 + 1;
        }
        battleGridUpdate();
        fightTrainerBallThrowEffect(iVar7,saved_r26,1);
        uVar6 = fn_801F8000(iVar7);
        fn_80132A38(0x22,uVar6);
        uVar6 = fightTrainerGetNamePtr(iVar7);
        fn_80132A38(0x23,uVar6);
        uVar6 = fightTrainerGetNamePtr(iVar7);
        fn_80132A38(0x25,uVar6);
        _fightActionFlowKaisiNyuujouPokemonSubAppearMsg__FP13FIGHT_TRAINERP15FightOutPokemonUsUsUsUsUc(iVar7,saved_r26,uVar11,uVar12,uVar13,uVar14,1);
        fightTrainerBallThrowEffect(iVar7,saved_r26,2);
        uVar14 = 0;
        while (1) {
          if (uVar1 <= (uVar14 & 0xffff)) break;
          saved_r26 = fightTrainerGetValidFightOutPokemonPtr(iVar7,uVar14);
          if (saved_r26 != 0) {
            uVar6 = fn_801F54A4(0,0,0x36,0);
            fn_801F4C14(0,0,0x36,0,saved_r26);
            fightOutPokemonDasuEffect(saved_r26,1);
            cVar10 = fightOutPokemonIsGcHeroFightOutPokemon(saved_r26);
            if (cVar10 == 0) {
              fn_80265598(saved_r26,uVar9,0);
            }
            else {
              fn_80265598(saved_r26,uVar9,1);
            }
            fightOutPokemonDasuEffect(saved_r26,2);
            fightOutPokemonDasuEffect(saved_r26,3);
            fightOutPokemonDasuEffect(saved_r26,4);
            fn_8026532C(saved_r26,uVar9,0);
            fn_801F4C14(0,0,0x36,0,uVar6);
          }
          uVar14 = uVar14 + 1;
        }
      }
      uVar13 = uVar13 + 1;
    }
    fightMenuCloseMsg();
    uVar13 = 0;
    while (1) {
      if (uVar2 <= (uVar13 & 0xffff)) break;
      iVar7 = fn_801F7258(uVar3,uVar13);
      if (iVar7 != 0) {
        uVar14 = 0;
        while (1) {
          if ((uVar1 <= (uVar14 & 0xffff)) ||
             (saved_r26 = fightTrainerGetValidFightOutPokemonPtr(iVar7,uVar14), saved_r26 != 0)) break;
          uVar14 = uVar14 + 1;
        }
        fightTrainerBallThrowEffect(iVar7,saved_r26,3);
        uVar14 = 0;
        while (1) {
          if (uVar1 <= (uVar14 & 0xffff)) break;
          saved_r26 = fightTrainerGetValidFightOutPokemonPtr(iVar7,uVar14);
          if (saved_r26 != 0) {
            fightOutPokemonDasuEffect(saved_r26,5);
          }
          uVar14 = uVar14 + 1;
        }
      }
      uVar13 = uVar13 + 1;
    }
    uVar3 = 1;
  }
  return uVar3;
}

/* Address: 0x8020CFE0 | Size: 0x21c | Ghidra import */
void _fightActionFlowKaisiNyuujouPokemonSubAppearMsg__FP13FIGHT_TRAINERP15FightOutPokemonUsUsUsUsUc(void)

{
    u32 r3;
    u32 r4;
    u16 r5;
    u16 r6;
    u32 r7;
    short r8;
    char r9;

    extern void fn_80132A38();
    extern u32 fn_801F18DC();
    extern int fn_801F8000();
    extern u32 fightOutPokemonIsGcHeroFightOutPokemon();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fightMenuOpenMsg();
  u32 uVar1;
  u32 uVar2;
  int iVar3;
  u32 uVar4;
  
  uVar1 = fightOutPokemonIsGcHeroFightOutPokemon(r4);
  uVar1 = __cntlzw(1 - (uVar1 & 0xff));
  uVar1 = uVar1 >> 5;
  uVar2 = fn_801F18DC(0);
  uVar2 = __cntlzw(1 - (uVar2 & 0xff));
  uVar2 = uVar2 >> 5;
  iVar3 = fn_801F8000(r3);
  if ((iVar3 == 0) && ((uVar1 & 0xff) == 0)) {
    uVar2 = 1;
  }
  uVar4 = fightOutPokemonGetPokemonPtr(r4);
  uVar4 = (int)pokemonGetStatus(uVar4,0,0x77,0);
  if (r9 == 0) {
    if ((uVar2 & 0xff) == 1) {
      if (r8 == 0) {
        fn_80132A38(0x14,uVar4);
        fn_80132A38(0x16,uVar4);
      }
      else {
        fn_80132A38(0x15,uVar4);
        fn_80132A38(0x17,uVar4);
      }
    }
    else if ((uVar1 & 0xff) == 1) {
      if (r8 == 0) {
        fn_80132A38(0x15,uVar4);
        fn_80132A38(0x17,uVar4);
      }
      else {
        fn_80132A38(0x14,uVar4);
        fn_80132A38(0x16,uVar4);
      }
    }
    else if (r8 == 0) {
      fn_80132A38(0x14,uVar4);
      fn_80132A38(0x16,uVar4);
    }
    else {
      fn_80132A38(0x15,uVar4);
      fn_80132A38(0x17,uVar4);
    }
  }
  else if (r9 == 1) {
    if ((r5 < 2) && (1 < r6)) {
      if ((uVar2 & 0xff) == 1) {
        uVar4 = 0x7674;
      }
      else if ((uVar1 & 0xff) == 1) {
        uVar4 = 0x7679;
      }
      else {
        uVar4 = 0x7671;
      }
    }
    else {
      fn_80132A38(0x14,uVar4);
      fn_80132A38(0x16,uVar4);
      if ((uVar2 & 0xff) == 1) {
        uVar4 = 0x7673;
      }
      else if ((uVar1 & 0xff) == 1) {
        uVar4 = 0x7678;
      }
      else {
        uVar4 = 0x7670;
      }
    }
    fightMenuOpenMsg(uVar4);
  }
  return;
}

/* Address: 0x8020D1FC | Size: 0x49c | Ghidra import */
u32 fightActionFlowKaisiNyuujouTrainer(void)

{
    extern u32 fn_8006B0F8();
    extern s8 fn_8006B57C();
    extern s8 pokemonCheckFightOut();
    extern s8 pokemonCheckValid();
    extern u32 heroGetStatus();
    extern void heroBiosCopy();
    extern void battleGridUpdate();
    extern void battleGridAddTrainer();
    extern void fn_801DA4E8();
    extern u32 fn_801F02AC();
    extern u32 fn_801F4804();
    extern u32 fn_801F54A4();
    extern int fn_801F7258();
    extern void fn_801F72B0();
    extern u32 fn_801F7388();
    extern s8 fn_801F7404();
    extern u32 fightSideGetStatus();
    extern u32 fightTrainerCreateSequence();
    extern int fightTrainerCheckTemotiPokemonFightEntry();
    extern void fightTrainerSortFightTrainerDataIdToHeroTemotiPokemon();
    extern void fightTrainerCreateFightTrainerDataIdToHero();
    extern s8 fightTrainerCheckTrainerDataIdValid();
    extern s8 fightTrainerCheckValid();
    extern void fightTrainerCreate();
    extern u32 fightTrainerGetStatus();
    extern s8 fightTrainerIsGcHero();
    extern void fightPokemonGetFriendFormPokemonFriendFilterId();
    extern void fightPokemonCreate();
    extern u32 fightActionDataBiosGetBuff();
    extern u32 fightActionBiosGetFightActionDataPtr();
    extern u32 fightEncountDataBiosGetGSInputDevice();
    extern u16 fightEncountDataBiosGetFightTrainerDataId();
    extern u32 fightEncountDataBiosGetPtr();
  u32 uVar1;
  u16 uVar14;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  s8 cVar15;
  u32 uVar7;
  u32 uVar8;
  u32 uVar9;
  u32 uVar10;
  s8 cVar16;
  int iVar11;
  u32 uVar12;
  u8 uVar17;
  int iVar13;
  int iVar18;
  u32 uVar19;
  u8 local_b58;
  u8 local_b57 [3];
  u8 auStack_b54 [2844];
  
  fightActionBiosGetFightActionDataPtr();
  uVar1 = fightActionDataBiosGetBuff();
  uVar14 = fn_801F54A4(0,0,0xd,0);
  uVar2 = fightEncountDataBiosGetPtr(uVar14);
  uVar14 = fn_801F54A4(0,0,0x14,0);
  uVar3 = fn_801F54A4(0,0,0x16,0);
  uVar3 = uVar3 & 0xffff;
  uVar4 = fn_801F54A4(0,0,0x17,0);
  uVar4 = uVar4 & 0xffff;
  uVar5 = fn_801F54A4(0,0,0x18,0);
  uVar6 = fn_801F02AC(uVar1 & 0xffff,0,uVar14);
  cVar15 = fn_801F7404();
  if (cVar15 == 0) {
    uVar2 = 0;
  }
  else {
    iVar13 = -(uVar1 & 0xffff);
    iVar18 = iVar13 + 4;
    uVar14 = fightSideGetStatus(uVar6,0,5,0);
    uVar1 = 0;
    while (1) {
      if (uVar3 <= (uVar1 & 0xffff)) break;
      uVar7 = fightSideGetStatus(uVar6,0,7,uVar1);
      uVar19 = uVar1 + (iVar18 - ((u32)(iVar18 == 0) + iVar13 + 3) & 0xffff) * uVar3 & 0xff;
      uVar8 = fightEncountDataBiosGetFightTrainerDataId(uVar2,uVar19);
      uVar9 = fightEncountDataBiosGetGSInputDevice(uVar2,uVar19);
      cVar15 = fightTrainerCheckTrainerDataIdValid(uVar8,uVar9);
      if (cVar15 != 0) {
        cVar15 = fn_8006B57C();
        if (cVar15 == 1) {
          uVar10 = fn_8006B0F8(uVar19);
          heroBiosCopy(auStack_b54,uVar10);
        }
        else {
          fightTrainerCreateFightTrainerDataIdToHero(uVar8,uVar9,auStack_b54);
        }
        uVar10 = fightTrainerCreateSequence(uVar8);
        fightTrainerCreate(uVar7,auStack_b54,uVar8,uVar9,uVar10);
        uVar8 = fightTrainerGetStatus(uVar7,0,0x44,0);
        cVar15 = fightTrainerCheckValid(uVar7);
        if (cVar15 != 0) {
          fightTrainerSortFightTrainerDataIdToHeroTemotiPokemon(uVar7,uVar4,uVar5 & 0xffff);
          cVar15 = 0;
          uVar19 = 0;
          while (((((uVar19 & 0xffff) < 6 && (iVar11 = (int)cVar15, iVar11 < (int)(uVar5 & 0xffff)))
                  && (iVar11 < (int)uVar4)) && (iVar11 < 6))) {
            uVar9 = heroGetStatus(uVar8,3,uVar19);
            cVar16 = pokemonCheckFightOut();
            if ((cVar16 != 0) && (iVar11 = fightTrainerCheckTemotiPokemonFightEntry(uVar7,uVar9), iVar11 == 0)) {
              uVar10 = fightTrainerGetStatus(uVar7,0,0x45,(int)cVar15);
              uVar12 = fn_801F4804(0);
              fightPokemonCreate(uVar10,uVar9,uVar12);
              cVar16 = fn_801F54A4(0,0,0x27,0);
              if ((cVar16 == 1) &&
                 ((cVar16 = fn_801F54A4(0,0,0x2e,0), cVar16 == 1 &&
                  (cVar16 = fightTrainerIsGcHero(uVar7), cVar16 == 1)))) {
                fightPokemonGetFriendFormPokemonFriendFilterId(uVar10,3);
              }
              cVar15 = cVar15 + 1;
            }
            uVar19 = uVar19 + 1;
          }
          uVar19 = 0;
          while ((((uVar19 & 0xffff) < 6 && ((int)cVar15 < (int)uVar4)) && (cVar15 < 6))) {
            uVar9 = heroGetStatus(uVar8,3,uVar19);
            cVar16 = pokemonCheckValid();
            if ((cVar16 != 0) && (iVar11 = fightTrainerCheckTemotiPokemonFightEntry(uVar7,uVar9), iVar11 == 0)) {
              uVar10 = fightTrainerGetStatus(uVar7,0,0x45,(int)cVar15);
              uVar12 = fn_801F4804(0);
              fightPokemonCreate(uVar10,uVar9,uVar12);
              cVar16 = fn_801F54A4(0,0,0x27,0);
              if ((cVar16 == 1) &&
                 ((cVar16 = fn_801F54A4(0,0,0x2e,0), cVar16 == 1 &&
                  (cVar16 = fightTrainerIsGcHero(uVar7), cVar16 == 1)))) {
                fightPokemonGetFriendFormPokemonFriendFilterId(uVar10,3);
              }
              cVar15 = cVar15 + 1;
            }
            uVar19 = uVar19 + 1;
          }
        }
      }
      uVar1 = uVar1 + 1;
    }
    uVar17 = fn_801F7388(uVar6);
    for (uVar4 = 0; (uVar4 & 0xffff) < uVar3; uVar4 = uVar4 + 1) {
      iVar13 = fn_801F7258(uVar6,uVar4);
      if ((iVar13 != 0) && (iVar13 = fightTrainerGetStatus(iVar13,0,0x4c,0), iVar13 != 0)) {
        fn_801F72B0(uVar14,uVar17,uVar4,local_b57,&local_b58);
        battleGridAddTrainer(iVar13,local_b57[0],local_b58);
        battleGridUpdate();
        fn_801DA4E8(iVar13,1);
      }
    }
    uVar2 = 1;
  }
  return uVar2;
}

/* Address: 0x8020D698 | Size: 0xec | Ghidra import */
u32 fightActionFlowKaijou(void)

{
    extern u32 fn_801EF624();
    extern u32 fn_801F02AC();
    extern void fn_801F17B0();
    extern void fn_801F4860();
    extern u32 fn_801F54A4();
    extern void fn_801F7480();
    extern u16 fightEncountDataBiosGetFightFloorDataId();
    extern u32 fightEncountDataBiosGetPtr();
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  u16 uVar4;
  u16 uVar5;
  
  uVar1 = fn_801EF624();
  uVar2 = fightEncountDataBiosGetPtr();
  uVar3 = fn_801F54A4(0,0,0,0);
  fn_801F4860(uVar3,uVar1);
  fn_801F17B0(0);
  uVar4 = fn_801F54A4(0,0,0x14,0);
  uVar1 = fightEncountDataBiosGetFightFloorDataId(uVar2);
  uVar2 = fn_801F02AC(4,0,uVar4);
  uVar5 = fn_801F54A4(0,uVar1,3,0);
  fn_801F7480(uVar2,uVar5);
  uVar2 = fn_801F02AC(5,0,uVar4);
  uVar4 = fn_801F54A4(0,uVar1,3,1);
  fn_801F7480(uVar2,uVar4);
  return 1;
}

/* Address: 0x8020D784 | Size: 0x8 | Pattern: return_constant */
u32 fightActionFlowNullFunc(void) { return 1; }

/* Address: 0x8020D78C | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetFifoBanme(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x1C]) = val;
}

/* Address: 0x8020D79C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionKindDataBiosGetDispFuncPtr(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020D7B4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionKindDataBiosGetFlowFuncPtr(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* fightActionKindDataBiosGetPri | Size: 0x1C | Read signed byte, return -128 if NULL */
s32 fightActionKindDataBiosGetPri(u8* ptr) {
    if (ptr == NULL) {
        return -128;
    }
    return (s8)ptr[0];
}

/* fightActionKindDataBiosGetPtr | Size: 0x2C | Look up entry in 12-byte table (u16 index) */
void* fightActionKindDataBiosGetPtr(u16 index) {
    extern u8 lbl_80375BB8[];
    extern u32 lbl_80478D48;
    if (index >= lbl_80478D48) {
        return NULL;
    }
    return &lbl_80375BB8[index * 12];
}

/* Address: 0x8020D814 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionDataBiosGetBuff(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020D82C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightActionDataBiosGetKind(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* fightActionBiosSetDispBuff | Size: 0x24 | Store value at indexed slot (max 4) */
#pragma push
#pragma peephole on
void fightActionBiosSetDispBuff(u8* ptr, u16 index, u32 value) {
    if (ptr == NULL) {
        return;
    }
    if (index >= 4) {
        return;
    }
    ptr += index * 4;
    *(u32*)(ptr + 0x20) = value;
}
#pragma pop

/* Address: 0x8020D868 | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetMotoFightActionDataPtr(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x18]) = val;
}

/* Address: 0x8020D878 | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetBuffDataId(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x10]) = val;
}

/* Address: 0x8020D888 | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetBuffDataPtr(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* Address: 0x8020D898 | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetActorFightTargetPtr(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}

/* Address: 0x8020D8A8 | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetFightActionDataPtr(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x8020D8B8 | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetBuff(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x8020D8C8 | Size: 0x10 | Pattern: nullcheck_setter */
void fightActionBiosSetKind(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x8020D8D8 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionBiosGetBuffDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x8020D8F0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionBiosGetBuffDataPtr(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x8020D908 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionBiosGetActorFightTargetPtr(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}

/* Address: 0x8020D920 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionBiosGetFightActionDataPtr(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020D938 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightActionBiosGetBuff(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020D950 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightActionBiosGetKind(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}
