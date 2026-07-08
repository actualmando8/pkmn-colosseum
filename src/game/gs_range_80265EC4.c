/**
 * @file gs_range_80265EC4.c
 * @brief gs-engine code, 0x80265EC4 - 0x8026635C (6 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). The range name stays honest until internal
 * TU structure is proven.
 */
#include "dolphin/types.h"

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

extern void* pokemonGetStatus();
extern u32 savedataGetStatus();
extern int fadeCheck();
extern int fadeSet();
extern void _threadSwitch();
extern void msgctrlSetValue();
extern void winMsgOpenField();
extern void winMsgCloseField();
extern void fn_801DB088();
extern u32 fn_801DE190();
extern void fn_801DDD28();
extern void fn_801DA4E8();
extern void fn_801DA9E8();
extern s8 fn_801DA94C();
extern void floorSetFadeScript();

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
    floorSetFadeScript(0, 0x5960008);
}
