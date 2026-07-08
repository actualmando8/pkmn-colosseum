/**
 * @file fight_waza.c
 * @brief fightWazaWzx module -- split from colosseum_event.c (the fight
 *        engine bucket, 0x80202810-0x80211A00), address range
 *        0x8020EE1C-0x80211170, 68 fns.
 *
 * fightWazaWzxTypeFunc / fightWazaWzxVariationFunc move-effect dispatch
 * table implementations (status/weather/multi-hit/variable-power moves)
 * plus their Bios data-table accessors. Own cluster between fightSeq and
 * fightTrainerAi in XD (0x8022B1F4-0x8022C628).
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

/* Address: 0x8020EE1C | Size: 0xa4 | Ghidra import */
void fn_8020EE1C(void)

{
    u32 r3;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;

    extern u32 fn_801F0204();
    extern void fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
  u32 uVar1;
  
  fightOutPokemonLoadWazaEffect(r4,r3,1,0);
  fightOutPokemonLoadWazaEffect(r5,r3,2,r7);
  fn_801F0234(0x11);
  uVar1 = fn_801F0204();
  fightOutPokemonWazaEffect(r4,r3,1,1,uVar1);
  fn_801F0234(0x12);
  uVar1 = fn_801F0204();
  fightOutPokemonWazaEffect(r5,r3,2,0,uVar1);
  return;
}

/* 0x8020EEC0 | size: 0x14 | tiny */
#pragma push
#pragma peephole on
extern u8 lbl_8047B600;
u8 fightWazaWzxVariationFuncOiuchi(void) { return (u8)(lbl_8047B600 == 1); }
#pragma pop

/* Address: 0x8020EED4 | Size: 0x22c | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncHuuin(void* ctx1, void* ctx2, u32 p5, u32 p6, void* p7)
{
    extern u32 fn_801F0204();
    extern void fn_801F0234();
    extern u32 fn_801F02AC();
    extern u32 fn_801F54A4();
    extern u8 fightOutPokemonCheckFightOut();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u16 limit;
    void* uVar1;
    void* uVar2;
    u8 cVar5;

    uVar1 = (void*)fn_801F02AC(0xf, ctx2, limit = fn_801F54A4(0, 0, 0x14, 0));
    uVar2 = (void*)fn_801F02AC(0x10, ctx2, limit);
    fightOutPokemonLoadWazaEffect(ctx2, ctx1, 1, p7);
    fightOutPokemonLoadWazaEffect(uVar1, ctx1, 2, p7);
    fightOutPokemonLoadWazaEffect(uVar2, ctx1, 2, p7);
    cVar5 = fightOutPokemonCheckFightOut(uVar1);
    if ((cVar5 == 1) && (cVar5 = fightOutPokemonCheckFightOut(uVar2), cVar5 == 1)) {
        fn_801F0234(0x11);
        fightOutPokemonWazaEffect(ctx2, ctx1, 1, 1, fn_801F0204());
        fn_801F0234(0x12);
        fightOutPokemonWazaEffect(uVar1, ctx1, 2, 1, fn_801F0204());
        fn_801F0234(0x12);
        fightOutPokemonWazaEffect(uVar2, ctx1, 2, 0, fn_801F0204());
    } else {
        cVar5 = fightOutPokemonCheckFightOut(uVar2);
        if (cVar5 == 1) {
            fn_801F0234(0x11);
            fightOutPokemonWazaEffect(ctx2, ctx1, 1, 1, fn_801F0204());
            fn_801F0234(0x12);
            fightOutPokemonWazaEffect(uVar2, ctx1, 2, 0, fn_801F0204());
        } else {
            cVar5 = fightOutPokemonCheckFightOut(uVar1);
            if (cVar5 == 1) {
                fn_801F0234(0x11);
                fightOutPokemonWazaEffect(ctx2, ctx1, 1, 1, fn_801F0204());
                fn_801F0234(0x12);
                fightOutPokemonWazaEffect(uVar1, ctx1, 2, 0, fn_801F0204());
            } else {
                fn_801F0234(0x11);
                fightOutPokemonWazaEffect(ctx2, ctx1, 1, 1, fn_801F0204());
            }
        }
    }
}
#pragma pop

/* Address: 0x8020F100 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F100(void) { return 0; }

/* 0x8020F108 | size: 0x128 */
void fightWazaWzxTypeFuncMigawari(void* battleCtx, void* ctx) {
    extern void wazaGetStatus();
    extern void battleGridUpdate();
    extern void battleGridReplacePokemon();
    extern void fn_801DB100();
    extern u8 fn_801F453C();
    extern void fn_801FCEC4();
    extern void* fightOutPokemonCreateSequence();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u8 localBuf[0x6E8];
    void* eeData;
    void* resolved;
    u8 partySlot;

    partySlot = (u8)fn_801F453C(0, 1);
    wazaGetStatus(0, battleCtx, 0x1F, 0);
    eeData = pokemonGetStatus(ctx, 0, 0xEE, 0);
    resolved = fightOutPokemonCreateSequence(ctx, partySlot);
    fn_801FCEC4(localBuf, ctx);
    pokemonSetStatus(localBuf, 0, 0xEE, 0, (u32)resolved);
    fightOutPokemonLoadWazaEffect(ctx, battleCtx, 1, 0);
    fightOutPokemonLoadWazaEffect(localBuf, battleCtx, 3, 0);
    fightOutPokemonWazaEffect(ctx, battleCtx, 1, 1, 0);
    battleGridReplacePokemon(eeData, resolved);
    battleGridUpdate();
    pokemonSetStatus(ctx, 0, 0xEE, 0, (u32)resolved);
    fightOutPokemonWazaEffect(ctx, battleCtx, 3, 0, 0);
    fn_801DB100(eeData);
}

/* Address: 0x8020F230 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F230(void) { return 0; }

/* 0x8020F238 | size: 0x128 */
void fightWazaWzxTypeFuncHensin(void* battleCtx, void* ctx) {
    extern void wazaGetStatus();
    extern void battleGridUpdate();
    extern void battleGridReplacePokemon();
    extern void fn_801DB100();
    extern u8 fn_801F453C();
    extern void fn_801FCEC4();
    extern void* fightOutPokemonCreateSequence();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u8 localBuf[0x6E8];
    void* eeData;
    void* resolved;
    u8 partySlot;

    partySlot = (u8)fn_801F453C(0, 1);
    wazaGetStatus(0, battleCtx, 0x1F, 0);
    eeData = pokemonGetStatus(ctx, 0, 0xEE, 0);
    resolved = fightOutPokemonCreateSequence(ctx, partySlot);
    fn_801FCEC4(localBuf, ctx);
    pokemonSetStatus(localBuf, 0, 0xEE, 0, (u32)resolved);
    fightOutPokemonLoadWazaEffect(ctx, battleCtx, 3, 0);
    fightOutPokemonLoadWazaEffect(localBuf, battleCtx, 3, 1);
    fightOutPokemonWazaEffect(ctx, battleCtx, 3, 1, 0);
    battleGridReplacePokemon(eeData, resolved);
    battleGridUpdate();
    pokemonSetStatus(ctx, 0, 0xEE, 0, (u32)resolved);
    fightOutPokemonWazaEffect(ctx, battleCtx, 3, 0, 0);
    fn_801DB100(eeData);
}

/* Address: 0x8020F360 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F360(void) { return 0; }

/* Address: 0x8020F368 | Size: 0x80 | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncMagiccort(void* ctx1, void* target1, void* sideCtx, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern int fn_802026E4();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
  u8 cVar2;

  fightOutPokemonLoadWazaEffect(target1,ctx1,1,p7);
  cVar2 = fn_802026E4(sideCtx,0x37);
  if (cVar2 == 1) {
    fightOutPokemonWazaEffect(target1,ctx1,1,0,fn_801F0204(fn_801F0234(0x11)));
  }
  return;
}
#pragma pop

/* Address: 0x8020F3E8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F3E8(void) { return 0; }

/* Address: 0x8020F3F0 | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8020F3F0(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,p7);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* Address: 0x8020F494 | Size: 0x84 | Ghidra import */

u32 fightWazaWzxVariationFuncWeatherBall(void)

{
    extern int fn_801F453C();
  u8 bVar2;
  u32 uVar1;
  
  bVar2 = fn_801F453C(0,1);
  if (bVar2 == 2) {
    uVar1 = 1;
  }
  else if (bVar2 < 2) {
    if (bVar2 == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = 3;
    }
  }
  else if (bVar2 == 4) {
    uVar1 = 2;
  }
  else if (bVar2 < 4) {
    uVar1 = 4;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

/* Address: 0x8020F518 | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8020F518(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,p7);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* Address: 0x8020F5BC | Size: 0x8c | Ghidra import */
#pragma push
#pragma peephole on
u32 fightWazaWzxVariationFuncIceball(u32 unused, void* typeObj)

{
    extern int fn_80202360();
  short sVar1;

  sVar1 = fn_80202360(typeObj,0x2f);
  switch (sVar1) {
  case 1:
    return 0;
  case 2:
    return 1;
  case 3:
    return 2;
  case 4:
    return 3;
  case -1:
    return 4;
  }
  return 0;
}
#pragma pop

/* Address: 0x8020F648 | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncKawarawari(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,0);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* 0x8020F6EC | size: 0x60 | small */
s32 fightWazaWzxVariationFuncKawarawari(void) {
    extern u8 lbl_80379F58[];
    u32 val;
    val = *(u8*)((u8*)lbl_80379F58 + (0x1 << 16) + 0x6002);
    switch ((s32)val) {
    case 0: return 3;
    case 1: return 0;
    case 2: return 1;
    case 3: return 2;
    default: return 0;
    }
}

/* 0x8020F74C | size: 0x64 */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncNegaigoto(void* p1, void* p2, u32 unused1, u32 unused2, u32 p3) {
    extern u32 fn_801F0204();
    extern void fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    fightOutPokemonLoadWazaEffect(p2, p1, 1, p3);
    fn_801F0234(0x11);
    fightOutPokemonWazaEffect(p2, p1, 1, 0, fn_801F0204());
}
#pragma pop

/* Address: 0x8020F7B0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F7B0(void) { return 0; }

/* 0x8020F7B8 | size: 0x114 */
void fightWazaWzxTypeFuncTedasuke(void* p1, void* p2, u32 p3, u32 p4) {
    extern u16 wazaGetStatus();
    extern void fightTargetGetRelativeHostSideFightTargetIdToTragetPtr();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void* fn_801F02AC();
    extern u16 fn_801F54A4();
    extern u8 fightOutPokemonCheckFightOut();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u16 partyCount;
    void* d9Data;
    u16 field29;
    void* resolved;
    void* tablePtr;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    d9Data = pokemonGetStatus(p2, 0, 0xD9, 0);
    field29 = wazaGetStatus(d9Data, 0, 0x29, 0);
    fightTargetGetRelativeHostSideFightTargetIdToTragetPtr(field29, partyCount);
    resolved = fn_801F02AC(0xE, p2, partyCount);
    fightOutPokemonLoadWazaEffect(p2, p1, 1, p4);
    fightOutPokemonLoadWazaEffect(resolved, p1, 3, p4);
    tablePtr = fn_801F0204(fn_801F0234(0x11));
    fightOutPokemonWazaEffect(p2, p1, 1, 1, tablePtr);
    if ((u8)fightOutPokemonCheckFightOut(resolved) == 1) {
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fightOutPokemonWazaEffect(resolved, p1, 3, 0, tablePtr);
    }
}

/* Address: 0x8020F8CC | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F8CC(void) { return 0; }

/* Address: 0x8020F8D4 | Size: 0x64 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8020F8D4(void* ctx1, void* target1, u32 unused1, u32 unused2, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,3,p7);
    fightOutPokemonWazaEffect(target1,ctx1,3,0,fn_801F0204(fn_801F0234(0x11)));
}
#pragma pop

/* Address: 0x8020F938 | Size: 0x64 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8020F938(void* ctx1, void* target1, u32 unused1, u32 unused2, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,3,p7);
    fightOutPokemonWazaEffect(target1,ctx1,3,0,fn_801F0204(fn_801F0234(0x11)));
}
#pragma pop

/* fightWazaWzxVariationFuncNomikomu | Size: 0x40 | Check if byte at fixed address equals 3 */
BOOL fightWazaWzxVariationFuncNomikomu(void) {
    extern u8 lbl_80379F58[];
    u8 val = *(u8*)((u8*)lbl_80379F58 + 0x16002);
    switch (val) {
        case 1:
        case 2:
            return FALSE;
        case 3:
            return TRUE;
        default:
            return FALSE;
    }
}

/* Address: 0x8020F9DC | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8020F9DC(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,p7);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* fightWazaWzxVariationFuncHakidasu | Size: 0x40 | Check if byte at fixed address equals 3 */
BOOL fightWazaWzxVariationFuncHakidasu(void) {
    extern u8 lbl_80379F58[];
    u8 val = *(u8*)((u8*)lbl_80379F58 + 0x16002);
    switch (val) {
        case 1:
        case 2:
            return FALSE;
        case 3:
            return TRUE;
        default:
            return FALSE;
    }
}

/* 0x8020FAC0 | size: 0x70 */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncAtt(void* p1, void* p2, u32 unused1, u16 flag, u32 p4) {
    extern u32 fn_801F0204();
    extern void fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    fightOutPokemonLoadWazaEffect(p2, p1, 1, p4);
    if (flag == 0) {
        fn_801F0234(0x11);
        fightOutPokemonWazaEffect(p2, p1, 1, 0, fn_801F0204());
    }
}
#pragma pop

/* Address: 0x8020FB30 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020FB30(void) { return 0; }

/* Address: 0x8020FB38 | Size: 0xc4 | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncItamiwake(void* ctx1, void* target1, void* target2, u16 flag, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    if (flag != 0) {
        fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
        fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
    }
    else {
        fightOutPokemonLoadWazaEffect(target1,ctx1,1,p7);
        fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
        fightOutPokemonWazaEffect(target1,ctx1,1,0,fn_801F0204(fn_801F0234(0x11)));
    }
}
#pragma pop

/* Address: 0x8020FBFC | Size: 0x8 | Pattern: return_constant */
u32 fn_8020FBFC(void) { return 0; }

/* Address: 0x8020FC04 | Size: 0x6c | Ghidra import */

u32 fightWazaWzxVariationFuncWeatherHP(void)

{
    extern int fn_801F453C();
  u8 bVar2;
  u32 uVar1;
  
  bVar2 = fn_801F453C(0,1);
  if (bVar2 == 1) {
    uVar1 = 0;
  }
  else if (bVar2 == 0) {
    uVar1 = 1;
  }
  else if (bVar2 < 5) {
    uVar1 = 2;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

/* 0x8020FC70 | size: 0x11C */
#pragma push
#pragma peephole on
void fn_8020FC70(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern u8 wazaGetStatus();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u8 battleType;

    battleType = (u8)wazaGetStatus(0, p1, 5, 0);
    if ((battleType == 4 || battleType == 6 || battleType == 1) && mode != 0) {
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    } else {
        fightOutPokemonLoadWazaEffect(p2, p1, 1, 0);
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    }
}
#pragma pop

/* Address: 0x8020FD8C | Size: 0xb4 | Ghidra import */
u32 fightWazaWzxVariationFuncMagnitude(void)

{
    u32 r3;
    u32 r4;

    extern u32 wazaGetStatus();
  u32 uVar1;
  u16 uVar2;
  
  uVar1 = (int)pokemonGetStatus(r4,0,0xd9,0);
  uVar2 = wazaGetStatus(uVar1,0,0x2f,0);
  if (uVar2 == 0x46) {
    return 1;
  }
  if (uVar2 < 0x46) {
    if (uVar2 == 0x1e) {
      return 0;
    }
    if (uVar2 < 0x1e) {
      if (uVar2 == 10) {
        return 0;
      }
    }
    else if (uVar2 == 0x32) {
      return 1;
    }
  }
  else {
    if (uVar2 == 0x6e) {
      return 2;
    }
    if (uVar2 < 0x6e) {
      if (uVar2 == 0x5a) {
        return 1;
      }
    }
    else if (uVar2 == 0x96) {
      return 2;
    }
  }
  return 0;
}

/* Address: 0x8020FE40 | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8020FE40(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,0);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* Address: 0x8020FEE4 | Size: 0x78 | Ghidra import */
char fightWazaWzxVariationFuncYatuatari()

{
    u32 r3;
    u32 r4;

    extern u32 wazaGetStatus();
  s8 cVar1;
  u32 uVar2;
  u16 uVar3;
  
  uVar2 = (int)pokemonGetStatus(r4,0,0xd9,0);
  uVar3 = wazaGetStatus(uVar2,0,0x2f,0);
  if (uVar3 < 0x5a) {
    if (uVar3 < 0x3e) {
      cVar1 = -((uVar3 < 0x16) + -1);
    }
    else {
      cVar1 = 2;
    }
  }
  else {
    cVar1 = 3;
  }
  return cVar1;
}

/* Address: 0x8020FF5C | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8020FF5C(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,0);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* Address: 0x80210000 | Size: 0x74 | Ghidra import */
u32 fightWazaWzxVariationFuncPresent(void)

{
    u32 r3;
    u32 r4;

    extern u32 wazaGetStatus();
  u32 uVar1;
  u16 uVar2;
  
  uVar1 = (int)pokemonGetStatus(r4,0,0xd9,0);
  uVar2 = wazaGetStatus(uVar1,0,0x2f,0);
  if (uVar2 == 0x50) {
LAB_0020d058:
    uVar1 = 0;
  }
  else {
    if (uVar2 < 0x50) {
      if (uVar2 == 0x28) goto LAB_0020d058;
    }
    else if (uVar2 == 0x78) goto LAB_0020d058;
    uVar1 = 1;
  }
  return uVar1;
}

/* Address: 0x80210074 | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_80210074(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,0);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* Address: 0x80210118 | Size: 0x78 | Ghidra import */
int fightWazaWzxVariationFuncOngaesi(void)

{
    u32 r3;
    u32 r4;

    extern u32 wazaGetStatus();
  u32 uVar1;
  u16 uVar3;
  int iVar2;
  
  uVar1 = (int)pokemonGetStatus(r4,0,0xd9,0);
  uVar3 = wazaGetStatus(uVar1,0,0x2f,0);
  if (uVar3 <= 0x18) {
    iVar2 = 0;
  }
  else if (uVar3 <= 0x24) {
    iVar2 = 1;
  }
  else {
    iVar2 = 3 - (u32)(uVar3 < 0x51);
  }
  return iVar2;
}

/* 0x80210190 | size: 0xA4 */
#pragma push
#pragma peephole on
void fn_80210190(void* p1, void* p2, void* p3, u32 unused, void* p4) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    fightOutPokemonLoadWazaEffect(p2, p1, 1, p4);
    fightOutPokemonLoadWazaEffect(p3, p1, 2, p4);
    fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* 0x80210234 | size: 0x84 */
#pragma push
#pragma peephole on
u32 fightWazaWzxVariationFuncRenzokugiri(u32 unused, void* typeObj) {
    extern s16 fn_80202360();
    s16 val;
    val = fn_80202360(typeObj, 0x2E);
    switch (val) {
        case 1: return 0;
        case 2: return 1;
        case 3: return 2;
        case 4: return 3;
        case 5: return 4;
        default: return 0;
    }
}
#pragma pop

/* Address: 0x802102B8 | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_802102B8(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,0);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* Address: 0x8021035C | Size: 0x8c | Ghidra import */
#pragma push
#pragma peephole on
u32 fightWazaWzxVariationFuncKorogaru(u32 unused, void* typeObj)

{
    extern int fn_80202360();
  short sVar1;

  sVar1 = fn_80202360(typeObj,0x2f);
  switch (sVar1) {
  case 1:
    return 0;
  case 2:
    return 1;
  case 3:
    return 2;
  case 4:
    return 3;
  case -1:
    return 4;
  }
  return 0;
}
#pragma pop

/* 0x802103E8 | size: 0x100 */
#pragma push
#pragma peephole on
void fn_802103E8(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern u8 wazaGetStatus();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    wazaGetStatus(0, p1, 5, 0);
    if (mode != 0) {
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    } else {
        fightOutPokemonLoadWazaEffect(p2, p1, 1, p5);
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    }
}
#pragma pop

/* Address: 0x802104E8 | Size: 0x84 | Ghidra import */
u32 fightWazaWzxVariationFuncTripleKick(void)

{
    u32 r3;
    u32 r4;

    extern u32 wazaGetStatus();
  u32 uVar1;
  u16 uVar2;
  
  uVar1 = (int)pokemonGetStatus(r4,0,0xd9,0);
  uVar2 = wazaGetStatus(uVar1,0,0x2f,0);
  if (uVar2 == 0x14) {
    uVar1 = 1;
  }
  else {
    if (uVar2 < 0x14) {
      if (uVar2 == 10) {
        return 0;
      }
    }
    else if (uVar2 == 0x1e) {
      return 2;
    }
    uVar1 = 0;
  }
  return uVar1;
}

/* Address: 0x8021056C | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_8021056C(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,0);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* fightWazaWzxVariationFuncTikyuunage | Size: 0x48 | Get level category from figthOutPokemonGetLevel result */
u32 fightWazaWzxVariationFuncTikyuunage(void* unused, void* param) {
    extern u8 figthOutPokemonGetLevel(void* param);
    u8 val = figthOutPokemonGetLevel(param);
    if (val < 0x21) {
        return 0;
    }
    if (val >= 0x42) {
        return 2;
    }
    return 1;
}

/* Address: 0x80210658 | Size: 0xa4 | Ghidra import */
#pragma push
#pragma peephole on
void fn_80210658(void* ctx1, void* target1, void* target2, u32 unused, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,1,p7);
    fightOutPokemonLoadWazaEffect(target2,ctx1,2,p7);
    fightOutPokemonWazaEffect(target1,ctx1,1,1,fn_801F0204(fn_801F0234(0x11)));
    fightOutPokemonWazaEffect(target2,ctx1,2,0,fn_801F0204(fn_801F0234(0x12)));
}
#pragma pop

/* Address: 0x802106FC | Size: 0xc0 | Ghidra import */
u32 fightWazaWzxVariationFuncKetaguri(void)

{
    u32 r3;
    u32 r4;

    extern u32 wazaGetStatus();
  u32 uVar1;
  u16 uVar2;
  
  uVar1 = (int)pokemonGetStatus(r4,0,0xd9,0);
  uVar2 = wazaGetStatus(uVar1,0,0x2f,0);
  if (uVar2 == 0x50) {
    return 3;
  }
  if (uVar2 < 0x50) {
    if (uVar2 == 0x28) {
      return 1;
    }
    if (uVar2 < 0x28) {
      if (uVar2 == 0x14) {
        return 0;
      }
    }
    else if (uVar2 == 0x3c) {
      return 2;
    }
  }
  else {
    if (uVar2 == 0x78) {
      return 5;
    }
    if ((uVar2 < 0x78) && (uVar2 == 100)) {
      return 4;
    }
  }
  return 0;
}

/* 0x802107BC | size: 0xC4 */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncAttDef(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    if (mode == 0) {
        fightOutPokemonLoadWazaEffect(p2, p1, 1, p5);
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p2, p1, 1, 0, fn_801F0204(fn_801F0234(0x11)));
    } else {
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    }
}
#pragma pop

/* Address: 0x80210880 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210880(void) { return 0; }

/* 0x80210888 | size: 0x108 */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncAllAlly(void* p1, void* p2, u32 p3, u32 unused, u32 p4) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void* fn_801F02AC();
    extern u16 fn_801F54A4();
    extern u8 fightOutPokemonCheckFightOut();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u16 partyCount;
    void* resolved;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    resolved = fn_801F02AC(0xE, p2, partyCount);
    fightOutPokemonLoadWazaEffect(p2, p1, 3, p4);
    fightOutPokemonLoadWazaEffect(resolved, p1, 3, p4);
    if ((u8)fightOutPokemonCheckFightOut(resolved) == 1) {
        fightOutPokemonWazaEffect(p2, p1, 3, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(resolved, p1, 3, 0, fn_801F0204(fn_801F0234(0x11)));
    } else {
        fightOutPokemonWazaEffect(p2, p1, 3, 0, fn_801F0204(fn_801F0234(0x11)));
    }
}
#pragma pop

/* Address: 0x80210990 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210990(void) { return 0; }

/* 0x80210998 | size: 0x168 */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncAllEnemy(void* p1, void* p2, void* p3, u32 unused, u32 p4) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void* fn_801F02AC();
    extern u16 fn_801F54A4();
    extern u8 fightOutPokemonCheckFightOut();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u16 partyCount;
    void* resolved;
    void* tablePtr;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    resolved = fn_801F02AC(0x10, p2, partyCount);
    fightOutPokemonLoadWazaEffect(p2, p1, 1, p4);
    fightOutPokemonLoadWazaEffect(p3, p1, 2, p4);
    fightOutPokemonLoadWazaEffect(resolved, p1, 2, p4);
    if ((u8)fightOutPokemonCheckFightOut(resolved) == 1) {
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 1, fn_801F0204(fn_801F0234(0x12)));
        fightOutPokemonWazaEffect(resolved, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    } else {
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    }
}
#pragma pop

/* Address: 0x80210B00 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210B00(void) { return 0; }

/* 0x80210B08 | size: 0xE8 */
#pragma push
#pragma peephole on
void fn_80210B08(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    void* tablePtr;

    if (mode != 0) {
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    } else {
        fightOutPokemonLoadWazaEffect(p2, p1, 1, p5);
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    }
}
#pragma pop

/* Address: 0x80210BF0 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210BF0(void) { return 0; }

/* 0x80210BF8 | size: 0x104 */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncAttDefSpc(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    if (mode == 0) {
        fightOutPokemonLoadWazaEffect(p2, p1, 1, p5);
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonLoadWazaEffect(p2, p1, 3, p5);
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    } else if (mode == 1) {
        fightOutPokemonLoadWazaEffect(p2, p1, 3, p5);
        fightOutPokemonWazaEffect(p2, p1, 3, 0, fn_801F0204(fn_801F0234(0x11)));
    }
}
#pragma pop

/* Address: 0x80210CFC | Size: 0x8 | Pattern: return_constant */
u32 fn_80210CFC(void) { return 0; }

/* 0x80210D04 | size: 0x150 */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncVampire(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern s32 wazaGetStatus();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    extern void fn_802221EC();
    void* d9Data;
    s32 field2D;

    d9Data = pokemonGetStatus(p2, 0, 0xD9, 0);
    field2D = wazaGetStatus(d9Data, 0, 0x2D, 0);
    if (mode == 0) {
        fightOutPokemonLoadWazaEffect(p2, p1, 1, p5);
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonLoadWazaEffect(p2, p1, 3, p5);
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    } else if (mode == 1) {
        fightOutPokemonLoadWazaEffect(p2, p1, 3, p5);
        if (field2D > 0) {
            fn_802221EC(0x32, p2, 0, 1);
        } else {
            fightOutPokemonWazaEffect(p2, p1, 3, 0, fn_801F0204(fn_801F0234(0x11)));
        }
    }
}
#pragma pop

/* Address: 0x80210E54 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210E54(void) { return 0; }

/* Address: 0x80210E5C | Size: 0x64 | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncSpc(void* ctx1, void* target1, u32 unused1, u32 unused2, void* p7)
{
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();

    fightOutPokemonLoadWazaEffect(target1,ctx1,3,p7);
    fightOutPokemonWazaEffect(target1,ctx1,3,0,fn_801F0204(fn_801F0234(0x11)));
}
#pragma pop

/* Address: 0x80210EC0 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210EC0(void) { return 0; }

/* Address: 0x80210EC8 | Size: 0x170 | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncTame(void* p1, void* p2, void* p3, u16 mode, u32 p5)
{
    extern u8 lbl_80379F58[];
    extern u8 wazaGetStatus();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u8 battleType;

    if (*(u8*)((u8*)lbl_80379F58 + (0x1 << 16) + 0x6002) == 1) {
        battleType = (u8)wazaGetStatus(0, p1, 5, 0);
        if ((battleType == 4 || battleType == 6 || battleType == 1) && mode != 0) {
            fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
            fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
        } else {
            fightOutPokemonLoadWazaEffect(p2, p1, 1, p5);
            fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
            fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
            fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
        }
    } else {
        fightOutPokemonLoadWazaEffect(p2, p1, 3, p5);
        fightOutPokemonWazaEffect(p2, p1, 3, 0, fn_801F0204(fn_801F0234(0x11)));
    }
}
#pragma pop

/* Address: 0x80211038 | Size: 0x8 | Pattern: return_constant */
u32 fn_80211038(void) { return 0; }

/* 0x80211040 | size: 0x11C */
#pragma push
#pragma peephole on
void fightWazaWzxTypeFuncNormal(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern u8 wazaGetStatus();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fightOutPokemonWazaEffect();
    extern void fightOutPokemonLoadWazaEffect();
    u8 battleType;

    battleType = (u8)wazaGetStatus(0, p1, 5, 0);
    if ((battleType == 4 || battleType == 6 || battleType == 1) && mode != 0) {
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    } else {
        fightOutPokemonLoadWazaEffect(p2, p1, 1, p5);
        fightOutPokemonLoadWazaEffect(p3, p1, 2, p5);
        fightOutPokemonWazaEffect(p2, p1, 1, 1, fn_801F0204(fn_801F0234(0x11)));
        fightOutPokemonWazaEffect(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)));
    }
}
#pragma pop

/* Address: 0x8021115C | Size: 0x8 | Pattern: return_constant */
u32 fightWazaWzxVariationFuncNormal(void) { return 0; }

/* 0x80211164 | size: 0x4 | trivial */
void fightWazaWzxTypeFuncNull(void) { return; }

/* Address: 0x80211168 | Size: 0x8 | Pattern: return_constant */
u32 fightWazaWzxVariationFuncNull(void) { return 0; }
