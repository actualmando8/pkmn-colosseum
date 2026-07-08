/**
 * @file GBARead.c
 * @brief gba/GBARead.c -- split from colosseum_battle.c (the
 *        Colosseum battle-flow/AI bucket, 0x802405C0-0x80265EC4),
 *        address range 0x8025F524-0x8025F618, 2 fns.
 *
 * XD source unit: gba/GBARead.c
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

#pragma scheduling on

/* Address: 0x8025F524 | Size: 0x60 | Ghidra import */
void ReadProc(int r3)

{
  u8 *entry;

  entry = lbl_804783E0 + r3 * 0x100;
  if (*(s32 *)(entry + 0x20) == 0) {
    memcpy(*(void **)(entry + 0x18), entry + 0x5, 4);
    **(u8 **)(entry + 0x14) = *(u8 *)(entry + 0x9) & 0x3a;
	  }
	}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GBARead(void) {
#include "src/game/colosseum_battle_fn_8025F584.inc"
}
#endif
#pragma pop
#pragma push
#pragma peephole off
u32 GBARead(int r3, u32 r4, u32 r5) {
  int idx;
  u8 *entry;
  s32 result;
  idx = r3;
  entry = lbl_804783E0 + idx * 0x100;
  if (*(u32 *)(entry + 0x1C) != 0) {
    result = 2;
  } else {
    *(u8 *)(entry + 0x0) = 0x14;
    *(u32 *)(entry + 0x18) = r4;
    *(u32 *)(entry + 0x14) = r5;
    *(u32 *)(entry + 0x1C) = (u32)__GBASyncCallback;
    result = __GBATransfer(idx, 1, 5, (u32)ReadProc);
  }
  if (result == 0) {
    result = __GBASync(idx);
  }
  return result;
}
#pragma pop
