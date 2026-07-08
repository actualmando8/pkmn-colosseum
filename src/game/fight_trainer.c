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

/* Address: 0x8020E4E8 | Size: 0x94 | Ghidra import */

u32 fightAbicntDoKakeWaru(u32 r3,int r4)

{
    extern u32 lbl_80478D68;
  u32 uVar1;
  u8 *pbVar2;
  int iVar3;
  u32 uVar4;

  pbVar2 = (u8 *)((r3 & 0xffff) * 2 + -0x7fc8a2f0);
  if (lbl_80478D68 <= (r3 & 0xffff)) {
    pbVar2 = (u8 *)0x0;
  }
  if (pbVar2 == (void *)0) {
    uVar4 = 0;
  }
  else {
    uVar4 = (u32)*pbVar2;
  }
  iVar3 = (r3 & 0xffff) * 2 + -0x7fc8a2f0;
  if (lbl_80478D68 <= (r3 & 0xffff)) {
    iVar3 = 0;
  }
  if (iVar3 == 0) {
    uVar1 = 1;
  }
  else {
    uVar1 = (u32)*(u8 *)(iVar3 + 1);
  }
  return (r4 * uVar4) / uVar1;
}

/* Address: 0x8020E57C | Size: 0x98 | Ghidra import */
int fightOutPokemonEnemySearchAry(void)

{
    int r3;
    u16 r4;
    int r5;

    extern int fn_801FD104();
  int iVar1;
  u16 uVar2;
  int iVar3;

  if (r3 != 0) {
    for (uVar2 = 0; uVar2 < r4; uVar2 = uVar2 + 1) {
      iVar3 = r3 + (u32)uVar2 * 0xc;
      iVar1 = fn_801FD104(iVar3);
      if ((iVar1 != 0) && (iVar1 = fn_801FD104(iVar3), iVar1 == r5)) {
        return iVar3;
      }
    }
  }
  return 0;
}

/* fightOutPokemonEnemyCheckValid | Size: 0x2C | Check if fn_801FD104 returns non-zero */
BOOL fightOutPokemonEnemyCheckValid(void) {
    extern s32 fn_801FD104(void);
    return -fn_801FD104() != 0;
}

/* Address: 0x8020E640 | Size: 0x94 | Ghidra import */
void fightOutPokemonEnemyCreate(void)

{
    int r3;
    int r4;

    extern void fn_801FD07C();
    extern void fn_801FD08C();
    extern void fn_801FD09C();
    extern void fn_801FD0AC();
    extern u32 fightOutPokemonGetPokemonPtr();
  u32 uVar1;
  u16 uVar2;
  
  if ((r3 != 0) && (r4 != 0)) {
    fn_801FD0AC(r3,0);
    fn_801FD09C(r3,0);
    fn_801FD08C(r3,0);
    fn_801FD07C(r3,0);
    fn_801FD0AC(r3,r4);
    uVar1 = fightOutPokemonGetPokemonPtr(r4);
    uVar2 = (int)pokemonGetStatus(uVar1,0,0x83,0);
    fn_801FD08C(r3,uVar2);
  }
  return;
}

/* Address: 0x8020E6D4 | Size: 0x84 | Ghidra import */
void fightOutPokemonEnemyInitAry(void)

{
    int r3;
    u16 r4;

    extern void fn_801FD07C();
    extern void fn_801FD08C();
    extern void fn_801FD09C();
    extern void fn_801FD0AC();
  u16 uVar1;
  int iVar2;

  if (r3 != 0) {
    for (uVar1 = 0; uVar1 < r4; uVar1 = uVar1 + 1) {
      iVar2 = r3 + (u32)uVar1 * 0xc;
      fn_801FD0AC(iVar2,0);
      fn_801FD09C(iVar2,0);
      fn_801FD08C(iVar2,0);
      fn_801FD07C(iVar2,0);
    }
  }
  return;
}

/* Address: 0x8020E758 | Size: 0x54 | Ghidra import */
#pragma push
#pragma peephole on
void fightOutPokemonEnemyInit(u32 r3)
{
    extern void fn_801FD07C();
    extern void fn_801FD08C();
    extern void fn_801FD09C();
    extern void fn_801FD0AC();
    fn_801FD0AC(r3, 0);
    fn_801FD09C(r3, 0);
    fn_801FD08C(r3, 0);
    fn_801FD07C(r3, 0);
}
#pragma pop

/* Address: 0x8020E7AC | Size: 0x1b0 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightTrainerEnemyPokemonEraseAry(void* ctx, u16 count, short matchVal)
{
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    extern void fn_801FBDD4();
    extern void fn_801FBDE4();
    extern void fn_801FBDF4();
    extern void fn_801FBE18();
    extern short fn_801FBF04();
  u32 bVar1;
  u32 uVar2;
  short sVar3;
  u16 uVar6;
  void* iVar4;
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
            sVar3 = fn_801FBF04(iVar4);
            if (sVar3 < 0) {
              bVar1 = 0;
            }
            else {
              bVar1 = 1;
            }
          }
          if (bVar1 == 0) goto LAB_0020b8ac;
        }
        else {
          if (iVar4 == NULL) {
            bVar1 = 0;
          }
          else {
            sVar3 = fn_801FBF04(iVar4);
            if (sVar3 < 0) {
              bVar1 = 0;
            }
            else {
              bVar1 = 1;
            }
          }
          if ((bVar1) && (sVar3 = fn_801FBF04(iVar4), matchVal == sVar3)) goto LAB_0020b8ac;
        }
      }
      iVar4 = NULL;
    }
LAB_0020b8ac:
    if (iVar4 == NULL) {
      uVar2 = 0;
    }
    else {
      fn_801FBE18(iVar4, (void*)0xffffffff);
      for (uVar5 = 0; (uVar5 & 0xff) < 4; uVar5 = uVar5 + 1) {
        fn_801FBDF4(iVar4,uVar5,0);
      }
      fn_801FBDE4(iVar4,0);
      fn_801FBDD4(iVar4,0);
      fn_801FBDC4(iVar4,0);
      fn_801FBDB4(iVar4,0);
      fn_801FBDA4(iVar4,0);
      fn_801FBD94(iVar4,0);
      fn_801FBD84(iVar4,0);
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
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    extern void fn_801FBDD4();
    extern void fn_801FBDE4();
    extern void fn_801FBDF4();
    extern void fn_801FBE18();
    extern short fn_801FBF04();
  u32 bVar1;
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
              sVar3 = fn_801FBF04(iVar4);
              if (sVar3 < 0) {
                bVar1 = 0;
              }
              else {
                bVar1 = 1;
              }
            }
            if (bVar1 == 0) goto LAB_0020ba60;
          }
          else {
            if (iVar4 == NULL) {
              bVar1 = 0;
            }
            else {
              sVar3 = fn_801FBF04(iVar4);
              if (sVar3 < 0) {
                bVar1 = 0;
              }
              else {
                bVar1 = 1;
              }
            }
            if ((bVar1) && (sVar3 = fn_801FBF04(iVar4), (short)matchVal == sVar3)) goto LAB_0020ba60;
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
              sVar3 = fn_801FBF04(iVar4);
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
            fn_801FBE18(iVar4, (void*)0xffffffff);
            for (uVar5 = 0; (uVar5 & 0xff) < 4; uVar5 = uVar5 + 1) {
              fn_801FBDF4(iVar4,uVar5,0);
            }
            fn_801FBDE4(iVar4,0);
            fn_801FBDD4(iVar4,0);
            fn_801FBDC4(iVar4,0);
            fn_801FBDB4(iVar4,0);
            fn_801FBDA4(iVar4,0);
            fn_801FBD94(iVar4,0);
            fn_801FBD84(iVar4,0);
            fn_801FBE18(iVar4,matchVal);
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
    extern s16 fn_801FBF04();
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
                sVar2 = fn_801FBF04(iVar4);
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
                sVar2 = fn_801FBF04(iVar4);
                if (sVar2 < 0) {
                    bVar1 = 0;
                } else {
                    bVar1 = 1;
                }
            }
            if (bVar1 && p3 == (sVar2 = fn_801FBF04(iVar4))) {
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
    extern s16 fn_801FBF04();
    s16 val;
    if (obj == 0) return 0;
    val = fn_801FBF04(obj);
    return (val >= 0) ? 1 : 0;
}
#pragma pop

/* Address: 0x8020ECE0 | Size: 0xdc | Ghidra import */
#pragma push
#pragma peephole on
void fightTrainerEnemyPokemonInitAry(void* ctx, u16 count) {
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    extern void fn_801FBDD4();
    extern void fn_801FBDE4();
    extern void fn_801FBDF4(void* entry, u8 idx, u32 zero);
    extern void fn_801FBE18();
    u8 j;
    void* entry;
    u16 i;
    if (ctx == NULL) { return; }
    for (i = 0; i < count; i++) {
        entry = (void*)((u32)ctx + (u32)i * 0x14);
        fn_801FBE18(entry, (void*)0xffffffff);
        for (j = 0; j < 4; j++) {
            fn_801FBDF4(entry, j, 0);
        }
        fn_801FBDE4(entry, 0);
        fn_801FBDD4(entry, 0);
        fn_801FBDC4(entry, 0);
        fn_801FBDB4(entry, 0);
        fn_801FBDA4(entry, 0);
        fn_801FBD94(entry, 0);
        fn_801FBD84(entry, 0);
    }
}
#pragma pop

/* 0x8020EDBC | size: 0x60 */
#pragma push
#pragma peephole on
void fightTrainerEnemyPokemonInitFightOutStatus(void* ctx) {
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    fn_801FBDC4(ctx, 0);
    fn_801FBDB4(ctx, 0);
    fn_801FBDA4(ctx, 0);
    fn_801FBD94(ctx, 0);
    fn_801FBD84(ctx, 0);
}
#pragma pop
