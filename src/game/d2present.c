/**
 * @file d2present.c
 * @brief game/pxdvs/app/d2present/d2present.cpp -- split from colosseum_battle.c (the
 *        Colosseum battle-flow/AI bucket, 0x802405C0-0x80265EC4),
 *        address range 0x80265E34-0x80265EC4, 2 fns.
 *
 * XD source unit: game/pxdvs/app/d2present/d2present.cpp
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

/* Address: 0x80265E34 | Size: 0x48 | Ghidra import */
u32 exribbonSetEarthRibbon(u32 r3)

{
    extern int pokemonBiosSetEarthRibbon();
  int iVar1;
  
  pokemonBiosSetEarthRibbon(r3,1);
  iVar1 = savedataGetStatus(0,0x10);
  if (*(char *)(iVar1 + 5) == '\0') {
    *(u8 *)(iVar1 + 5) = 0x2d;
  }
  return 1;
}

/* Address: 0x80265E7C | Size: 0x48 | Ghidra import */
#pragma dont_inline on
u32 exribbonSetNarionalRibbon(u32 r3)

{
    extern int pokemonBiosSetNationalRibbon();
  int iVar1;
  
  pokemonBiosSetNationalRibbon(r3,1);
  iVar1 = savedataGetStatus(0,0x10);
  if (*(char *)(iVar1 + 4) == '\0') {
    *(u8 *)(iVar1 + 4) = 0x2c;
  }
  return 1;
}
#pragma dont_inline reset

/* Address: 0x80265F94 | Size: 0x2BC | Ghidra import */
void fn_80265F94(int r3)

{
    extern s8 winMsgCheck();
    extern u16 pokemonDataBiosGetVoice();
    extern int pokemonDataBiosGetPtr();
    extern int fn_801666BC();
    extern int fn_80166A28();
    extern u16 lbl_8047E6F8;
    extern f32 lbl_8047E6FC;
  u16 uVar1;
  u32 uVar2;

  short sVar6;
  short sVar7;
  u32 uVar3;
  s8 cVar9;
  int iVar4;
  u16 uVar8;
  u32 uVar5;
  
  sVar6 = (int)pokemonGetStatus(0,0xfa,0x66,0);
  sVar7 = -1;
  if (sVar6 != 0) {
    sVar7 = sVar6;
  }
  uVar3 = fn_801DE190(sVar7,0,0);
  fn_801DDD28(uVar3,lbl_8047E6F8,4,0);
  fn_801DA4E8(uVar3,1);
  uVar1 = lbl_8047E6F8;
  fn_801DA9E8(uVar3,uVar1,4);
  fadeSet((double)lbl_8047E6FC,2);
  while (cVar9 = fadeCheck(0), cVar9 != '\0') {
    fn_801DB088();
    cVar9 = fn_801DA94C(uVar3,uVar1,4);
    if (cVar9 == '\0') {
      fn_801DA9E8(uVar3,uVar1,4);
    }
    _threadSwitch();
  }
  iVar4 = pokemonDataBiosGetPtr(0xfa);
  if (iVar4 == 0) {
    uVar8 = 0;
  }
  else {
    uVar8 = pokemonDataBiosGetVoice();
    fn_80166A28(uVar8);
  }
  while (iVar4 = fn_801666BC(uVar8), iVar4 == 2) {
    fn_801DB088();
    cVar9 = fn_801DA94C(uVar3,uVar1,4);
    if (cVar9 == '\0') {
      fn_801DA9E8(uVar3,uVar1,4);
    }
    _threadSwitch();
  }
  winMsgOpenField(0x44ba,0,0);
  while (1) {
    cVar9 = winMsgCheck();
    uVar2 = __cntlzw(1 - cVar9);
    if ((uVar2 >> 5 & 0xff) == 0) break;
    fn_801DB088();
    cVar9 = fn_801DA94C(uVar3,uVar1,4);
    if (cVar9 == '\0') {
      fn_801DA9E8(uVar3,uVar1,4);
    }
    _threadSwitch();
  }
  winMsgCloseField(1);
  if (r3 == 1) {
    msgctrlSetValue(0x5d,0x3d2);
    uVar5 = 0x44bb;
  }
  else if ((r3 < 1) && (-1 < r3)) {
    msgctrlSetValue(0x5d,0x3d2);
    uVar5 = 0x44bc;
  }
  else {
    uVar5 = 0x44b9;
  }
  winMsgOpenField(uVar5,0,0);
  while (1) {
    cVar9 = winMsgCheck();
    uVar2 = __cntlzw(1 - cVar9);
    if ((uVar2 >> 5 & 0xff) == 0) break;
    fn_801DB088();
    cVar9 = fn_801DA94C(uVar3,uVar1,4);
    if (cVar9 == '\0') {
      fn_801DA9E8(uVar3,uVar1,4);
    }
    _threadSwitch();
  }
  winMsgCloseField(1);
  fadeSet((double)lbl_8047E6FC,3);
  while (cVar9 = fadeCheck(0), cVar9 != '\0') {
    fn_801DB088();
    cVar9 = fn_801DA94C(uVar3,uVar1,4);
    if (cVar9 == '\0') {
      fn_801DA9E8(uVar3,uVar1,4);
    }
    _threadSwitch();
  }
  return;
}

/* Address: 0x80266250 | Size: 0xD0 | Ghidra import */
void fn_80266250(void)
{
    extern int fn_800FF660();
    extern u32 lbl_8047B680;
    extern int fn_801653BC();
    extern int fn_801653C4();
    extern u32 fn_801656D8();
    extern void fn_80165A20();
    extern void scriptSoundStop();
    extern void fn_801659FC();
    extern void fn_801DADC0();
    extern void wazaSequenceSysRelease();
    extern void fn_80265F94();
    u32 iVar1;
    u32 uVar3;
    u32 iVar2;
    u32 uVar4;
    u32 uVar5;

    uVar5 = lbl_8047B680;
    iVar1 = fn_801653C4();
    if (iVar1 != 0) {
        uVar3 = fn_801656D8();
        fn_80165A20(1, 0x32, 0xff);
    } else {
        uVar3 = 0;
    }
    iVar2 = fn_801653BC();
    if (iVar2 != 0) {
        uVar4 = fn_801656D8();
        scriptSoundStop(0x32);
    } else {
        uVar4 = 0;
    }
    fn_801DADC0(1);
    fn_80265F94(uVar5);
    wazaSequenceSysRelease();
    if (iVar1 != 0) {
        fn_80165A20(iVar1, 0x32, uVar3);
    }
    if (iVar2 != 0) {
        fn_801659FC(iVar2, 0x32, uVar4);
    }
    fn_800FF660();
    fn_8011288C(0, 0x5960008);
}
