/**
 * @file fight_gsfloor.c
 * @brief game/pxdvs/app/fight/fightGSfloor.cpp -- split from colosseum_battle.c (the
 *        Colosseum battle-flow/AI bucket, 0x802405C0-0x80265EC4),
 *        address range 0x802614B4-0x80261B68, 7 fns.
 *
 * XD source unit: game/pxdvs/app/fight/fightGSfloor.cpp
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

#pragma peephole on

/* Address: 0x802614B4 | Size: 0x88 | Ghidra import */
int fightGSfloorGetPushDataSize(void)

{
    typedef struct BattleScanContext {
        u8 collectEntries;
        u8 consumeEntries;
        u16 count;
        u8 *entries;
        u8 *nextEntry;
    } BattleScanContext;
    typedef u32 (*BattleScanCallback)(u32, u32, char *);
    extern void fightFloorLoopValidFightTrainer(u32, BattleScanCallback, void *, u32);
    extern void fightFloorLoopValidFightOutPokemon(u32, BattleScanCallback, void *, u32);
    extern u32 _fightGSfloorPokemonCB__FPvUsPv(u32, u32, char *);
    extern u32 _fightGSfloorTrainerCB__FPvUsPv(u32, u32, char *);
    BattleScanContext scan;
    u16 firstCount;
    int total;

    scan.collectEntries = 0;
    scan.consumeEntries = 0;
    scan.count = 0;
    scan.nextEntry = 0;
    fightFloorLoopValidFightTrainer(0, _fightGSfloorTrainerCB__FPvUsPv, &scan, 0);
    firstCount = scan.count;
    scan.count = 0;
    fightFloorLoopValidFightOutPokemon(0, _fightGSfloorPokemonCB__FPvUsPv, &scan, 0);
    total = firstCount * 0x78;
    total += scan.count * 0x7C;
    return total + 0x48;
}

/* Address: 0x8026153C | Size: 0xB8 | Ghidra import */
void fightGSfloorPushData(void *rawOut)
{
#pragma optimize_for_size on
    typedef struct BattleScanOutput {
        u16 firstCount;
        u16 secondCount;
        u8 entries[0x44];
    } BattleScanOutput;
    typedef struct BattleScanContext {
        u8 collectEntries;
        u8 consumeEntries;
        u16 count;
        u8 *entries;
        u8 *nextEntry;
    } BattleScanContext;
    typedef u32 (*BattleScanCallback)(u32, u32, char *);
    extern void *battleGridGetPtr(void);
    extern void fightFloorLoopValidFightTrainer(u32, BattleScanCallback, void *, u32);
    extern void fightFloorLoopValidFightOutPokemon(u32, BattleScanCallback, void *, u32);
    extern u32 _fightGSfloorPokemonCB__FPvUsPv(u32, u32, char *);
    extern u32 _fightGSfloorTrainerCB__FPvUsPv(u32, u32, char *);
    BattleScanOutput *out;
    BattleScanContext scan;
    u16 firstCount;
    u16 secondCount;
    u8 *entries;

    out = rawOut;
    entries = out->entries;
    memcpy(entries, battleGridGetPtr(), 0x44);
    scan.collectEntries = 1;
    scan.consumeEntries = 1;
    scan.count = 0;
    scan.entries = entries;
    scan.nextEntry = entries + 0x44;
    fightFloorLoopValidFightTrainer(0, _fightGSfloorTrainerCB__FPvUsPv, &scan, 0);
    firstCount = scan.count;
    scan.count = 0;
    fightFloorLoopValidFightOutPokemon(0, _fightGSfloorPokemonCB__FPvUsPv, &scan, 0);
    secondCount = scan.count;
    out->firstCount = firstCount;
    out->secondCount = secondCount;
    fn_801EF8F4(1);
    fn_801C3114();
    wazaSequenceSysRelease();
}

typedef struct BattleReplayHeader {
    u16 firstCount;
    u16 secondCount;
    u8 entries[0x44];
} BattleReplayHeader;

typedef struct BattleReplayFirstEntry {
    u32 target;
    u16 modelId;
    u8 attr;
    u8 pad7;
    u32 *modelOut;
    u8 state[0x6C];
} BattleReplayFirstEntry;

typedef struct BattleReplaySecondEntry {
    u32 target;
    u16 modelId;
    u8 attr;
    u8 variant;
    u32 arg;
    u32 *modelOut;
    u8 state[0x6C];
} BattleReplaySecondEntry;

/* Address: 0x802615F4 | Size: 0x114 | Ghidra import */
void fightGSfloorPopData(BattleReplayHeader *header)
{
    extern u32 GSmodelGetVisibility();
    extern void GSmodelPopState();
    extern int battleGridUpdate();
    BattleReplaySecondEntry *secondEntry;
    extern int fn_801DAEF8(int);
    extern u32 fn_801DE418(u16);
    u16 secondCount;
    extern int fightFloorSetShadow();
    u8 *entries;
    u16 firstCount;
    BattleReplayFirstEntry *firstEntry;
    u32 model;
    u32 state;

    entries = header->entries;
    fn_801DAEF8(10);
    firstCount = header->firstCount;
    firstEntry = (BattleReplayFirstEntry *)(entries + 0x44);
    secondCount = header->secondCount;
    while (firstCount-- != 0) {
        model = fn_801DE418(firstEntry->modelId);
        *(u32 *)(firstEntry->target + 0x27C0) = model;
        *firstEntry->modelOut = model;
        state = fn_801DAC3C(model);
        GSmodelPopState(state, firstEntry->state);
        fn_801DA224(model, firstEntry->attr);
        firstEntry++;
    }
    secondEntry = (BattleReplaySecondEntry *)firstEntry;
    while (secondCount-- != 0) {
        model = fn_801DE190(secondEntry->modelId, secondEntry->arg, secondEntry->variant);
        *(u32 *)(secondEntry->target + 0x600) = model;
        *secondEntry->modelOut = model;
        state = fn_801DAC3C(model);
        GSmodelPopState(state, secondEntry->state);
        fn_801DA224(model, secondEntry->attr);
        state = GSmodelGetVisibility(state);
        fn_801DA4E8(model, state);
        secondEntry++;
    }
    memcpy(battleGridGetPtr(), entries, 0x44);
    fn_801EF8F4(1);
    battleGridUpdate();
    fightFloorSetShadow();
}

/* Address: 0x80261708 | Size: 0x144 | Ghidra import */
u32 _fightGSfloorPokemonCB__FPvUsPv(u32 r3,u32 r4,char *r5)

{
    extern int GSmodelPushState();
    extern u8 fn_801D9E1C();
    extern u8 fn_801DA354();
    extern u16 fn_801DAC78();
    extern int fn_801DB100();
    extern u32 fn_801DE164();
  int iVar1;
  u32 *puVar9;
  u32 iVar2;
  u16 sVar5;
  u8 uVar6;
	  u32 uVar3;
	  int iVar4;
	  int iVar8;
	  int iVar7;
	  int iVar10;
	  int base;
  
  iVar2 = (u32)pokemonGetStatus(r3,0,0xee,0);
  if (iVar2 == 0) {
    return 1;
  }
  sVar5 = fn_801DAC78();
  if (sVar5 == 0) {
    return 1;
  }
  if (*r5 != '\0') {
    puVar9 = *(u32 **)(r5 + 8);
    *puVar9 = r3;
    *(short *)(puVar9 + 1) = sVar5;
    uVar6 = fn_801DA354(iVar2);
    *(u8 *)((int)puVar9 + 6) = uVar6;
    uVar6 = fn_801D9E1C(iVar2);
    *(u8 *)((int)puVar9 + 7) = uVar6;
    uVar3 = fn_801DE164(iVar2);
    puVar9[2] = uVar3;
    base = *(int *)(r5 + 4);
    if (iVar2 == 0) {
      iVar1 = 0;
    }
    else {
			      iVar8 = 0;
			      iVar4 = 0;
			      do {
			        iVar7 = 0;
			        for (iVar10 = 2; iVar10 != 0; iVar10--) {
			          iVar1 = base + iVar4 + iVar7;
		          if (*(int *)(iVar1 + 4) == iVar2) {
		            iVar1 = iVar1 + 4;
		            goto LAB_0025e7f4;
	          }
	          iVar7 = iVar7 + 4;
	        }
	        iVar8 = iVar8 + 1;
	        iVar4 = iVar4 + 0x10;
	      } while (iVar8 < 4);
      iVar1 = 0;
    }
LAB_0025e7f4:
    puVar9[3] = iVar1;
    uVar3 = fn_801DAC3C(iVar2);
    GSmodelPushState(uVar3,puVar9 + 4);
    *(int *)(r5 + 8) = *(int *)(r5 + 8) + 0x7c;
  }
  if (r5[1] != '\0') {
    fn_801DB100(iVar2);
  }
  *(u16 *)(r5 + 2) = *(u16 *)(r5 + 2) + 1;
  return 1;
}

/* Address: 0x8026184C | Size: 0x108 | Ghidra import */
u32 _fightGSfloorTrainerCB__FPvUsPv(u32 r3,u32 r4,char *r5)

{
  u32 *puVar7;
  u32 iVar1;
  int sVar4;
  u8 uVar5;
  int iVar2;
  u32 uVar3;
  int *piVar6;
  int base;
  int iVar8;
  
  iVar1 = fightTrainerGetStatus(r3,0,0x4c,0);
  if (iVar1 == 0) {
    return 1;
  }
  sVar4 = fn_801DAC78();
  if ((u16)sVar4 == 0) {
    return 1;
  }
  if (*r5 != '\0') {
    puVar7 = *(u32 **)(r5 + 8);
    *puVar7 = r3;
    *(short *)(puVar7 + 1) = sVar4;
    uVar5 = fn_801DA354(iVar1);
    *(u8 *)((int)puVar7 + 6) = uVar5;
    base = *(int *)(r5 + 4);
    if (iVar1 == 0) {
      piVar6 = (int *)0x0;
    }
	    else {
		      iVar2 = 0;
				      for (iVar8 = 4; iVar8 != 0; iVar8--) {
	                piVar6 = (int *)(base + iVar2);
	                    if (*piVar6 == iVar1) {
	                        if (((!iVar2) && (!iVar2)) && (!iVar2)) {
	                        }
	                        goto LAB_0025e8fc;
	                    }
	                iVar2 = iVar2 + 0x10;
				      }
	      piVar6 = (int *)0x0;
	    }
LAB_0025e8fc:
    puVar7[2] = (u32)piVar6;
    uVar3 = fn_801DAC3C(iVar1);
    GSmodelPushState(uVar3,puVar7 + 3);
    *(int *)(r5 + 8) = *(int *)(r5 + 8) + 0x78;
  }
  if (r5[1] != '\0') {
    fn_801DB100(iVar1);
  }
  *(u16 *)(r5 + 2) = *(u16 *)(r5 + 2) + 1;
  return 1;
}

/* Address: 0x80261954 | Size: 0x17C | Ghidra import */
void fightMenuCloseInfoMenu(u32 wait)
{
    extern void fightFloorLoopValidFightTrainer();
    extern u32 fightFloorLoopValidFightOutPokemon();
    extern u8 fightFloorIsUseFightTimerCommand(u32);
    extern u8 fightFloorIsUseFightTimerAll(u32);
    extern void menuFightCloseCountDown(void);
    extern void menuFightCloseTotalTimer(void);
    extern u8 menuFightCloseCheckCountDown(void);
    extern u8 menuFightCloseCheckTotalTimer(void);
    extern u32 _fightMenuAllFightTrainerCloseStatusMenuSubCloseCheck__FPvUsPv();
    extern u32 _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv();
    extern u32 _fightMenuAllFightOutPokemonCloseStatusMenuSubCloseCheck__FPvUsPv();
    extern u32 _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv();
    u8 done;

    fightFloorLoopValidFightTrainer(0, _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv, 0, 0);
    fightFloorLoopValidFightOutPokemon(0, _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv, 0, 0);
    if (fightFloorIsUseFightTimerCommand(0) == 1) {
        menuFightCloseCountDown();
    }
    if (fightFloorIsUseFightTimerAll(0) == 1) {
        menuFightCloseTotalTimer();
    }
    if ((u8)wait == 1) {
        fightFloorLoopValidFightTrainer(0, _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv, 0, 0);
        do {
            done = 1;
            fightFloorLoopValidFightTrainer(0, _fightMenuAllFightTrainerCloseStatusMenuSubCloseCheck__FPvUsPv, &done, 0);
            if (done == 1) {
                break;
            }
            _threadSwitch();
        } while (1);
        fightFloorLoopValidFightOutPokemon(0, _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv, 0, 0);
        do {
            if ((u8)fightFloorLoopValidFightOutPokemon(0, _fightMenuAllFightOutPokemonCloseStatusMenuSubCloseCheck__FPvUsPv, 0, 0) == 1) {
                break;
            }
            _threadSwitch();
        } while (1);
        if (fightFloorIsUseFightTimerCommand(0) == 1) {
            do {
                if (menuFightCloseCheckCountDown() == 0) {
                    break;
                }
                _threadSwitch();
            } while (1);
        }
        if (fightFloorIsUseFightTimerAll(0) == 1) {
            do {
                if (menuFightCloseCheckTotalTimer() == 0) {
                    break;
                }
                _threadSwitch();
            } while (1);
        }
    }
}

/* Address: 0x80261AD0 | Size: 0x98 | Ghidra import */
void fightMenuOpenInfoMenu(s8 timerMode)
{
    extern void fightFloorLoopValidFightTrainer();
    extern u32 fightFloorLoopValidFightOutPokemon();
    extern u8 fightFloorIsUseFightTimerCommand(u32);
    extern u8 fightFloorIsUseFightTimerAll(u32);
    extern void menuFightOpenCountDown(void);
    extern void menuFightOpenTotalTimer(void);
    extern u32 _fightMenuAllFightTrainerOpenStatusMenuSub__FPvUsPv();
    extern u32 _fightMenuAllFightOutPokemonOpenStatusMenuSub__FPvUsPv();
    u8 openStatus;

    fightFloorLoopValidFightTrainer(0, _fightMenuAllFightTrainerOpenStatusMenuSub__FPvUsPv, 0, 0);
    openStatus = 1;
    fightFloorLoopValidFightOutPokemon(0, _fightMenuAllFightOutPokemonOpenStatusMenuSub__FPvUsPv, &openStatus, 0);
    if (timerMode < 0) {
        if (fightFloorIsUseFightTimerCommand(0) == 1) {
            menuFightOpenCountDown();
        }
    }
    if (fightFloorIsUseFightTimerAll(0) == 1) {
        menuFightOpenTotalTimer();
    }
}
