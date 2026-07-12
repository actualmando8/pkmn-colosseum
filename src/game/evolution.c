/**
 * @file evolution.c
 * @brief game/pxdvs/app/evolution/evolution.cpp -- split from colosseum_battle.c (the
 *        Colosseum battle-flow/AI bucket, 0x802405C0-0x80265EC4),
 *        address range 0x802600E4-0x80261388, 7 fns.
 *
 * XD source unit: game/pxdvs/app/evolution/evolution.cpp
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

/* Address: 0x8026045C | Size: 0x27C | Ghidra import */
int cbWazaForget(u32 r3,u32 r4,int r5)

{
    extern u8 lbl_8027A488[];
    extern int fn_80097A38();
    extern int GScameraGetPerspective();
    extern u32 GScameraGetActiveCamera();
    extern int GSmodelSetAnimType();
    extern int fn_801766A8();
    extern int GSscene_GetCameraRotationVector();
    extern int GSscene_SetCameraRotationVector();
    extern int GSscene_GetCameraDirectionVector();
    extern int GSscene_SetCameraDirectionVector();
    extern int GSscene_GetCameraPositionVector();
    extern int GSscene_SetCameraPositionVector();
    extern int GSscene_GetCameraViewVector();
    extern int GSscene_SetCameraViewVector();
    extern int GSscene_SetMode();
    extern int wazaSequenceSysRelease();
    extern int fn_801DADC0();
    extern f32 lbl_8047E6C0;

  u32 uVar1;
  u32 *savedRotationPtr;
  u32 *savedPositionPtr;
  u32 *savedViewPtr;
  int iVar2;
  u16 sVar4;
  u32 uVar3;
  u8 cVar5;
  u16 *puVar6;
  u32 iVar7;
  int iVar8;
  u32 savedView[4];
  u32 savedPosition[3];
  u32 savedRotation[3];
  u32 savedDirection[3];
  u32 view[3];
  u32 position[3];
  u32 rotation[3];
  u32 direction[3];
  u8 auStack_8c [4];
  u8 auStack_90 [4];
  u8 auStack_94 [4];
  float local_98;
  
  fadeSet((double)lbl_8047E6C0,3);
  fadeCheck(1);
  GSscene_GetCameraDirectionVector(direction);
  GSscene_GetCameraRotationVector(rotation);
  GSscene_GetCameraPositionVector(position);
  GSscene_GetCameraViewVector(view);
  uVar1 = GScameraGetActiveCamera();
  GScameraGetPerspective(uVar1,&local_98,auStack_94,auStack_90,auStack_8c);
  savedDirection[0] = direction[0];
  savedDirection[1] = direction[1];
  savedDirection[2] = direction[2];
  savedRotationPtr = savedRotation;
  savedPositionPtr = savedPosition;
  savedViewPtr = savedView;
  savedRotation[0] = rotation[0];
  savedRotation[1] = rotation[1];
  savedRotation[2] = rotation[2];
  savedPosition[0] = position[0];
  savedPosition[1] = position[1];
  savedPosition[2] = position[2];
  savedView[0] = view[0];
  savedView[1] = view[1];
  savedView[2] = view[2];
  ((volatile float *)savedView)[3] = local_98;
  wazaSequenceSysRelease();
  iVar2 = fn_80097A38(r3,r4);
  if (iVar2 >= 4) {
    iVar2 = -1;
  }
  fn_801DADC0(2);
  sVar4 = (int)pokemonGetStatus(r3,0,0x6e,0);
  if (sVar4 == 0) {
    sVar4 = 0xffff;
  }
  else {
    sVar4 = (int)pokemonGetStatus(0,sVar4,0x66,0);
    if (sVar4 == 0) {
      sVar4 = 0xffff;
    }
  }
  if (sVar4 == 0xffff) {
    iVar7 = 0;
  }
  else {
    uVar1 = pokemonBiosGetRnd(r3);
    uVar3 = (int)pokemonGetStatus(r3,0,0xc1,0);
    uVar3 = (-uVar3 | uVar3) >> 0x1f;
    iVar7 = fn_801DE190(sVar4,uVar1,uVar3);
    if (iVar7 == 0) {
      iVar7 = 0;
    }
    else {
      iVar8 = 0;
      puVar6 = (u16 *)lbl_8027A488;
      do {
        if ((*(int *)(puVar6 + 2) == 1) && (cVar5 = fn_801DDD28(iVar7,*puVar6,4,0), cVar5 == '\0'))
        break;
        iVar8 = iVar8 + 1;
        puVar6 = puVar6 + 4;
      } while (iVar8 < 5);
      if (iVar8 < 5) {
        iVar7 = 0;
      }
    }
  }
  if (iVar7 != 0) {
    *(int *)(r5 + 4) = iVar7;
  }
  uVar1 = fn_801DAC3C(*(u32 *)(r5 + 4));
  GSmodelSetAnimType(uVar1,1);
  fn_801DA4E8(*(u32 *)(r5 + 4),1);
  GSscene_SetMode(2);
  GSscene_SetCameraDirectionVector(savedDirection);
  GSscene_SetCameraRotationVector(savedRotationPtr);
  GSscene_SetCameraPositionVector(savedPositionPtr);
  GSscene_SetCameraViewVector(savedViewPtr);
  cameraSetFov((double)((volatile float *)savedView)[3]);
  fadeSet((double)lbl_8047E6C0,2);
  fadeCheck(1);
  return (int)(signed char)iVar2;
}

/* Address: 0x802606D8 | Size: 0x238 | Ghidra import */
int doWazaSequence(u32 *r3,int r4,int r5,u32 r6)

{
    extern u8 lbl_8027A488[];
    extern int fn_800D3088();
    extern u32 fn_800F7AF0();
    extern u32 fn_800F7BC4();
    extern f32 lbl_8047E6C0;
  u16 uVar1;
  BOOL bVar2;

  u8 cVar7;
  int iVar3;
  u32 uVar4;
  u32 uVar5;
  u32 uVar6;
  int iVar8;
  u32 uVar9;
  u32 *savedRotationPtr;
  u32 *savedPositionPtr;
  u32 *savedViewPtr;
  u32 savedView[4];
  u32 savedPosition[3];
  u32 savedRotation[3];
  u32 savedDirection[3];
  u32 view[3];
  u32 position[3];
  u32 rotation[3];
  u32 direction[3];
  u8 auStack_9c [4];
  u8 auStack_a0 [4];
  u8 auStack_a4 [4];
  float local_a8;
  
  iVar8 = 0;
  bVar2 = 0;
  if ((r4 < 0) || (r4 >= 5)) {
    iVar8 = 0;
  }
  else {
    uVar1 = *(u16 *)(lbl_8027A488 + r4 * 8);
    if (*(int *)(lbl_8027A488 + r4 * 8 + 4) != 0) {
      uVar9 = r3[1];
    }
    else {
      uVar9 = *r3;
    }
    fn_801DA9E8(uVar9,uVar1,4);
    savedRotationPtr = savedRotation;
    savedPositionPtr = savedPosition;
    savedViewPtr = savedView;
    while (!bVar2) {
      fn_801DB088();
      if ((r6 & 10) != 0) {
        uVar6 = 4;
        if ((r6 & 2) != 0) {
          uVar6 = 2;
        }
        fadeSet((double)lbl_8047E6C0,uVar6);
        r6 = r6 & 0xfffffff5;
      }
      cVar7 = fn_801DA94C(uVar9,uVar1,4);
      if (cVar7 == '\0') break;
      _threadSwitch();
      GSscene_GetCameraDirectionVector(direction);
      GSscene_GetCameraRotationVector(rotation);
      GSscene_GetCameraPositionVector(position);
      GSscene_GetCameraViewVector(view);
      uVar6 = GScameraGetActiveCamera();
      GScameraGetPerspective(uVar6,&local_a8,auStack_a4,auStack_a0,auStack_9c);
      savedDirection[0] = direction[0];
      savedDirection[1] = direction[1];
      savedDirection[2] = direction[2];
      savedRotation[0] = rotation[0];
      savedRotation[1] = rotation[1];
      savedRotation[2] = rotation[2];
      savedPosition[0] = position[0];
      savedPosition[1] = position[1];
      savedPosition[2] = position[2];
      savedView[0] = view[0];
      savedView[1] = view[1];
      savedView[2] = view[2];
      ((volatile float *)savedView)[3] = local_a8;
      iVar3 = fn_800D3088();
      iVar8 = iVar8 + iVar3;
      if (r5 != 0) {
        uVar4 = fn_800F7AF0(1);
        uVar5 = fn_800F7BC4(1);
        if ((uVar5 & uVar4 & 0x200) != 0) {
          bVar2 = 1;
        }
      }
    }
    GSscene_SetMode(2);
    GSscene_SetCameraDirectionVector(savedDirection);
    GSscene_SetCameraRotationVector(savedRotationPtr);
    GSscene_SetCameraPositionVector(savedPositionPtr);
    GSscene_SetCameraViewVector(savedViewPtr);
    cameraSetFov((double)((volatile float *)savedView)[3]);
    if ((r6 & 0x14) != 0) {
      uVar6 = 5;
      if ((r6 & 4) != 0) {
        uVar6 = 3;
      }
      fadeSet((double)lbl_8047E6C0,uVar6);
      fadeCheck(1);
    }
    fn_801DA8C4(uVar9,uVar1,4);
    if (bVar2 != 0) {
      return iVar8;
    }
    iVar8 = -1;
  }
  return iVar8;
}

/* Address: 0x80260EBC | Size: 0x414 | Ghidra import */
u32
evolutionStart(u32 r3,u32 r4,u32 r5,u16 *r6,
            int r7,u8 *r8)

{
    extern u8 lbl_8027A488[];
    extern int pokemonBiosCopy();
    extern int scriptSoundStop();
    extern int evolutionDemo();
    extern f32 lbl_8047E6C0;
  u16 uVar1;
  BOOL bVar2;
  u8 candidateValid;

  u32 sVar7;
  u8 cVar8;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  u32 evolutionResult;
  int iVar6;
  int iVar9;
  int *resultPtr;
  u16 *puVar10;
  u8 local_188 [4];
  int local_184;
  int local_180;
  volatile int local_178;
  volatile int local_174;
  volatile u32 local_170;
  volatile u32 local_16c;
  u8 auStack_168 [320];
  
  fn_801DADC0(2);
  local_184 = 0;
  local_180 = 0;
  sVar7 = (int)pokemonGetStatus(r3,0,0x6e,0);
  if ((sVar7 & 0xffff) == 0) {
    sVar7 = 0xffff;
  }
  else {
    sVar7 = (int)pokemonGetStatus(0,sVar7,0x66,0);
    if ((sVar7 & 0xffff) == 0) {
      sVar7 = 0xffff;
    }
  }
  if ((sVar7 & 0xffff) == 0xffff) {
    evolutionResult = 0;
  }
  else {
    uVar3 = pokemonBiosGetRnd(r3);
    uVar4 = (int)pokemonGetStatus(r3,0,0xc1,0);
    uVar4 = (-uVar4 | uVar4) >> 0x1f;
    evolutionResult = fn_801DE190(sVar7,uVar3,uVar4);
    if (evolutionResult == 0) {
      evolutionResult = 0;
    }
    else {
      iVar6 = 0;
      puVar10 = (u16 *)lbl_8027A488;
      do {
        if ((*(int *)(puVar10 + 2) == 0) &&
           (cVar8 = fn_801DDD28(evolutionResult,*puVar10,4,0), cVar8 == '\0')) break;
        iVar6 = iVar6 + 1;
        puVar10 = puVar10 + 4;
      } while (iVar6 < 5);
      if (iVar6 < 5) {
        evolutionResult = 0;
      }
    }
  }
  if (evolutionResult == 0) {
    candidateValid = 0;
  }
  else {
    local_184 = evolutionResult;
    candidateValid = 1;
  }
  if (candidateValid) {
    sVar7 = (int)pokemonGetStatus(r4,0,0x6e,0);
    if ((sVar7 & 0xffff) == 0) {
      sVar7 = 0xffff;
    }
    else {
      sVar7 = (int)pokemonGetStatus(0,sVar7,0x66,0);
      if ((sVar7 & 0xffff) == 0) {
        sVar7 = 0xffff;
      }
    }
    if ((sVar7 & 0xffff) == 0xffff) {
      evolutionResult = 0;
    }
    else {
      uVar3 = pokemonBiosGetRnd(r4);
      uVar4 = (int)pokemonGetStatus(r4,0,0xc1,0);
      uVar4 = (-uVar4 | uVar4) >> 0x1f;
      evolutionResult = fn_801DE190(sVar7,uVar3,uVar4);
      if (evolutionResult == 0) {
        evolutionResult = 0;
      }
      else {
        iVar6 = 0;
        puVar10 = (u16 *)lbl_8027A488;
        do {
          if ((*(int *)(puVar10 + 2) == 1) &&
             (cVar8 = fn_801DDD28(evolutionResult,*puVar10,4,0), cVar8 == '\0')) break;
          iVar6 = iVar6 + 1;
          puVar10 = puVar10 + 4;
        } while (iVar6 < 5);
        if (iVar6 < 5) {
          evolutionResult = 0;
        }
      }
    }
    if (evolutionResult == 0) {
      candidateValid = 0;
    }
    else {
      local_180 = evolutionResult;
      candidateValid = 1;
    }
  }
  if (candidateValid) {
    bVar2 = 1;
  }
  else {
    wazaSequenceSysRelease();
    bVar2 = 0;
  }
  if (bVar2) {
    iVar9 = fn_801653C4();
    if (iVar9 != 0) {
      uVar4 = fn_801656D8();
      fn_80165A20(1,0x32,0xff);
    }
    else {
      uVar4 = 0;
    }
    iVar6 = fn_801653BC();
    if (iVar6 != 0) {
      uVar5 = fn_801656D8();
      scriptSoundStop(0x32);
    }
    else {
      uVar5 = 0;
    }
    local_178 = iVar9;
    local_174 = iVar6;
    local_170 = uVar4;
    local_16c = uVar5;
    iVar9 = evolutionDemo(&local_184,r5,r3,r4);
    if (local_178 != 0) {
      fn_80165A20(local_178,0x32,local_170 & 0xff);
    }
    if (local_174 != 0) {
      fn_801659FC(local_174,0x32,local_16c & 0xff);
    }
    if (iVar9 == 0) {
      bVar2 = 0;
    }
    else {
      pokemonBiosCopy((int*)auStack_168,r4);
      resultPtr = &local_184;
      for (iVar9 = 0; iVar9 < r7; iVar9 = iVar9 + 1) {
        uVar1 = *r6;
        iVar6 = evolutionWazaLearn((int*)auStack_168,uVar1,local_188,0,cbWazaForget,resultPtr);
        if (iVar6 != 0) {
          pokemonWazaCreate((int*)auStack_168,local_188[0],uVar1);
        }
        else {
          local_188[0] = 0xff;
        }
        r6 = r6 + 1;
        *r8 = local_188[0];
        r8 = r8 + 1;
      }
      fadeSet((double)lbl_8047E6C0,3);
      fadeCheck(1);
      bVar2 = 1;
    }
    wazaSequenceSysRelease();
    if (bVar2) {
      uVar3 = 0;
    }
    else {
      uVar3 = 1;
    }
  }
  else {
    uVar3 = 2;
  }
  return uVar3;
}

/* Address: 0x8026132C | Size: 0x5C | Ghidra import */
u32 evolutionOpen(u32 r3, u32 r4, u32 r5, u16 *r6, u32 r7, u8 *r8)
{
    extern u32 lbl_804787E0[];
    extern void fn_800FF730(u32);
    extern void floorSetFadeScript(u32, u32);
    extern void _threadSwitch(void);
    u32 *base;

    base = lbl_804787E0;
    base[0] = r3;
    base[1] = r4;
    base[2] = r5;
    base[3] = r7;
    base[4] = (u32)r6;
    base[5] = (u32)r8;
    fn_800FF730(0x386);
    floorSetFadeScript(0, 0);
    _threadSwitch();
    return base[6];
}

/* Address: 0x802612D0 | Size: 0x5C | Ghidra import */

void evolution(void)
{
    extern u32 lbl_804787E0[];
    extern u32 evolutionStart();
    extern void fn_800FF660();
    extern void floorSetFadeScript();
    u32 *base = lbl_804787E0;
    base[6] = evolutionStart(base[0], base[1], base[2], (u16*)base[4], base[3], (u8*)base[5]);
    fn_800FF660();
    floorSetFadeScript(0, 0);
}

/* Address: 0x802600E4 | Size: 0x378 | Ghidra import */
u32
evolutionWazaLearn(u32 r3,u32 r4,u8 *r5,int r6,void *r7,
            u32 r8)

{
    extern s8 fn_8001E074();
    extern int winMsgClose();
    extern int winMsgOpen();
  int iVar1;
  short sVar3;
  u32 uVar2;
  s8 cVar5;
  u16 uVar4;
  u32 uVar6;
  
  uVar6 = 0;
  do {
    sVar3 = pokemonBiosGetPokemonWazaDataId(r3,uVar6 & 0xffff);
    if (sVar3 == 0) break;
    uVar6 = uVar6 + 1;
  } while ((int)uVar6 < 4);
  if ((int)uVar6 < 4) {
LAB_0025d3c4:
    fn_80165668(0x4ca,0,0xff);
    uVar2 = pokemonBiosGetNicknamePtr(r3);
    msgctrlSetValue(0x32,uVar2);
    msgctrlSetValue(0x39,r4 & 0xffff);
    if (r6 == 0) {
      winMsgOpenField(0x423d,1,0);
      winMsgCloseField(1);
    }
    else {
      winMsgOpen(2,0x423d,1,0);
      winMsgClose(1);
    }
    *r5 = (char)uVar6;
    uVar2 = 1;
  }
  else {
    uVar2 = pokemonBiosGetNicknamePtr(r3);
    msgctrlSetValue(0x32,uVar2);
    msgctrlSetValue(0x39,r4 & 0xffff);
    do {
      if (r6 == 0) {
        winMsgOpenField(0x4243,1,0);
        cVar5 = fn_8001E184();
        winMsgCloseField(1);
      }
      else {
        winMsgOpen(2,0x4243,1,0);
        cVar5 = fn_8001E074(0,0xffffffff,0xffffffff,0);
        winMsgClose(1);
      }
      if (cVar5 == '\x01') {
        iVar1 = 1;
      }
      else if ((cVar5 < '\x01') && (-1 < cVar5)) {
        iVar1 = 0;
      }
      else {
        iVar1 = 2;
      }
      if (iVar1 == 0) {
        if (r7 == (void *)0x0) {
          uVar6 = 0;
        }
        else {
          cVar5 = ((s8 (*)())r7)(r3,r4,r8);
          uVar6 = (u32)cVar5;
        }
        if (-1 < (int)uVar6) {
          uVar2 = pokemonBiosGetNicknamePtr(r3);
          msgctrlSetValue(0x32,uVar2);
          msgctrlSetValue(0x5d,0x468);
          uVar4 = pokemonBiosGetPokemonWazaDataId(r3,uVar6 & 0xffff);
          msgctrlSetValue(0x39,uVar4);
          if (r6 == 0) {
            winMsgOpenField(0x4248,1,0);
          }
          else {
            winMsgOpen(2,0x4248,1,0);
          }
          goto LAB_0025d3c4;
        }
      }
      msgctrlSetValue(0x32,uVar2);
      msgctrlSetValue(0x39,r4 & 0xffff);
      if (r6 == 0) {
        winMsgOpenField(0x4242,1,0);
        cVar5 = fn_8001E184();
        winMsgCloseField(1);
      }
      else {
        winMsgOpen(2,0x4242,1,0);
        cVar5 = fn_8001E074(0,0xffffffff,0xffffffff,0);
        winMsgClose(1);
      }
      if (cVar5 == '\x01') {
        iVar1 = 1;
      }
      else if ((cVar5 < '\x01') && (-1 < cVar5)) {
        iVar1 = 0;
      }
      else {
        iVar1 = 2;
      }
    } while (iVar1 != 0);
    if (r6 == 0) {
      winMsgOpenField(0x4241,1,0);
      winMsgCloseField(1);
    }
    else {
      winMsgOpen(2,0x4241,1,0);
      winMsgClose(1);
    }
    uVar2 = 0;
  }
  return uVar2;
}

/* Address: 0x80260910 | Size: 0x5AC | Ghidra import (PSQ removed) */


u32 evolutionDemo(u32 *r3,int r4,u32 r5,u32 r6)

{
    extern u8 lbl_8027A488[];
    extern u8 lbl_8027A4B0[];
    extern f32 lbl_8047E6B0;
    extern f32 lbl_8047E6B4;
    extern f64 lbl_8047E6B8;
    extern f32 lbl_8047E6C0;
    extern f32 lbl_8047E6C4;
    int saved_r29;
  float fVar1;
  u16 uVar2;
  BOOL bVar3;
  BOOL bVar4;
  BOOL bVar5;

  u32 uVar5;
  int iVar6;
  u32 uVar7;
  u32 uVar8;
  u8 cVar10;
  s8 cVar13;
  u16 uVar9;
  int uVar11;
  u32 uVar12;

  double dVar13;
  double dVar14;
  double dVar15;
  volatile u32 local_8;
  volatile u32 local_c;

  fn_801DA4E8(*r3,1);
  uVar5 = pokemonBiosGetNicknamePtr(r5);
  msgctrlSetValue(0x32,uVar5);
  winMsgOpenField(0x4401,1,0);
  if (r5 == 0) {
    iVar6 = 0;
  }
  else {
    cVar10 = pokemonCheckValid(r5);
    if (cVar10 == '\0') {
      iVar6 = 0;
    }
    else {
      pokemonBiosGetPokemonDataId(r5);
      iVar6 = pokemonDataBiosGetPtr();
    }
  }
  if (iVar6 == 0) {
    uVar9 = 0;
  }
  else {
    uVar9 = pokemonDataBiosGetVoice();
    fn_80166A28(uVar9);
  }
  if (*(u32 *)(lbl_8027A488 + 4) == 0) {
    uVar5 = *r3;
  }
  else {
    uVar5 = r3[1];
  }
  fn_801DA9E8(uVar5,*(u16 *)lbl_8027A488,4);
  fn_801DB088();
  r3[2] = 0;
  bVar3 = 1;
  bVar4 = 0;
  uVar11 = 0;
  uVar12 = 0;
  goto LAB_0025db58;
  while (1) {
    if (((r4 != 0) && (0x77 < uVar12)) && (1 < uVar11)) {
      uVar7 = fn_800F7AF0(1);
      uVar8 = fn_800F7BC4(1);
      if ((uVar8 & uVar7 & 0x200) != 0) {
        bVar4 = 1;
        goto LAB_0025db60;
      }
    }
    if (*(int *)(lbl_8027A488 + r3[2] * 8 + 4) == 0) {
      uVar5 = *r3;
    }
    else {
      uVar5 = r3[1];
    }
    uVar2 = *(u16 *)(lbl_8027A488 + r3[2] * 8);
    fn_801DB088();
    uVar7 = fn_801DA94C(uVar5,uVar2,4);
    bVar5 = (uVar7 & 0xff) != 0;
    if (!bVar5) goto LAB_0025db60;
    if (uVar11 == 1) {
      iVar6 = fn_801666BC(0x3d3);
      if (iVar6 != 2) {
        iVar6 = fn_800D3088();
        saved_r29 = saved_r29 + iVar6;
        if (0x1d < saved_r29) {
          fn_80165A20(0x3d4,0,0xff);
          uVar11 = 2;
        }
      }
    }
    else if ((uVar11 == 0) && (iVar6 = fn_801666BC(uVar9), iVar6 != 2)) {
      fn_80165A20(0x3d3,0,0xff);
      saved_r29 = 0;
      uVar11 = 1;
    }
    if (bVar3) {
      fadeSet((double)lbl_8047E6C0,2);
      bVar3 = 0;
    }
    _threadSwitch();
    iVar6 = fn_800D3088();
    uVar12 = uVar12 + iVar6;
LAB_0025db58:
    if (uVar12 >= 0x23a) break;
  }

LAB_0025db60:
  fadeSet((double)lbl_8047E6C0,5);
  while (cVar13 = fadeCheck(0), cVar13 != '\0') {
    if (*(int *)(lbl_8027A488 + r3[2] * 8 + 4) == 0) {
      uVar5 = *r3;
    }
    else {
      uVar5 = r3[1];
    }
    uVar9 = *(u16 *)(lbl_8027A488 + r3[2] * 8);
    fn_801DB088();
    fn_801DA94C(uVar5,uVar9,4);
    _threadSwitch();
  }
  if (*(int *)(lbl_8027A488 + r3[2] * 8 + 4) == 0) {
    uVar5 = *r3;
  }
  else {
    uVar5 = r3[1];
  }
  fn_801DA8C4(uVar5,*(u16 *)(lbl_8027A488 + r3[2] * 8),4);
  dVar15 = lbl_8047E6B8;
  dVar14 = (double)lbl_8047E6B4;
  fVar1 = lbl_8047E6B0;
  while (dVar13 = (double)fVar1, dVar13 < dVar14) {
    _threadSwitch();
    uVar5 = fn_800D3088();
    local_8 = 0x43300000;
    local_c = uVar5;
    fVar1 = (float)(dVar13 + (double)(float)(*(double *)&local_8 - dVar15));
  }
  if (bVar4) {
    winMsgCloseField(1);
    soundStop(0x3d4,0x32);
      iVar6 = 0;
    if ((*(u32 *)lbl_8027A4B0 < uVar12) && (iVar6 = 1, *(u32 *)(lbl_8027A4B0 + 8) < uVar12)) {
      iVar6 = 2;
    }
    doWazaSequence(r3,*(u32 *)(lbl_8027A4B0 + iVar6 * 8 + 4),0,8);
    if (r5 == 0) {
      iVar6 = 0;
    }
    else {
      cVar10 = pokemonCheckValid(r5);
      if (cVar10 == '\0') {
        iVar6 = 0;
      }
      else {
        pokemonBiosGetPokemonDataId(r5);
        iVar6 = pokemonDataBiosGetPtr();
      }
    }
    if (iVar6 == 0) {
      uVar9 = 0;
    }
    else {
      uVar9 = pokemonDataBiosGetVoice();
      fn_80166A28(uVar9);
    }
    while (iVar6 = fn_801666BC(uVar9), iVar6 == 2) {
      _threadSwitch();
    }
    uVar5 = pokemonBiosGetNicknamePtr(r5);
    msgctrlSetValue(0x32,uVar5);
    winMsgOpenField(0x43ff,1,0);
    winMsgCloseField(1);
    fadeSet((double)lbl_8047E6C0,3);
    fadeCheck(1);
    uVar5 = 0;
  }
  else {
    fn_801DA4E8(*r3,0);
    fn_801DA4E8(r3[1],1);
    doWazaSequence(r3,1,0,8);
    winMsgCloseField(1);
    soundStop(0x3d4,0x32);
    if (r6 == 0) {
      iVar6 = 0;
    }
    else {
      cVar10 = pokemonCheckValid(r6);
      if (cVar10 == '\0') {
        iVar6 = 0;
      }
      else {
        pokemonBiosGetPokemonDataId(r6);
        iVar6 = pokemonDataBiosGetPtr();
      }
    }
    if (iVar6 == 0) {
      uVar9 = 0;
    }
    else {
      uVar9 = pokemonDataBiosGetVoice();
      fn_80166A28(uVar9);
    }
    while (iVar6 = fn_801666BC(uVar9), iVar6 == 2) {
      _threadSwitch();
    }
    uVar5 = pokemonBiosGetNicknamePtr(r5);
    msgctrlSetValue(0x32,uVar5);
    uVar9 = pokemonBiosGetPokemonDataId(r6);
    msgctrlSetValue(0x4e,uVar9);
    msgctrlSetValue(0x5d,0x3d2);
    winMsgOpenField(0x4400,1,0);
    winMsgCloseField(1);
    dVar14 = lbl_8047E6B8;
    dVar15 = (double)lbl_8047E6C4;
    fVar1 = lbl_8047E6B0;
    while (dVar13 = (double)fVar1, dVar13 < dVar15) {
      _threadSwitch();
      uVar5 = fn_800D3088();
      local_8 = 0x43300000;
      local_c = uVar5;
      fVar1 = (float)(dVar13 + (double)(float)(*(double *)&local_8 - dVar14));
    }
    uVar5 = 1;
  }

  return uVar5;
}
