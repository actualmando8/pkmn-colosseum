/**
 * @file colosseum_battle.c
 * @brief Pre/post battle flow, rewards, and experience for Colosseum mode.
 *
 * =========================================================================
 * SUBSYSTEM ANALYSIS
 * =========================================================================
 *
 * Address range: 0x80240000 - 0x80266360
 * Total functions: ~520
 * Total code size: ~156KB
 *
 * This module handles everything that happens around battles in the
 * Colosseum mode -- the setup before battles, processing after battles,
 * experience/reward distribution, and progression tracking.
 *
 * The code in this range heavily uses the CheckTrainerPokemonFlag
 * (fn_80236BFC, 272 calls) and BattleSequenceStart (fn_80239EE8, 491 calls)
 * functions, confirming its role as the battle orchestration layer.
 *
 * BATTLE ORCHESTRATION (0x80240000-0x8024D000):
 *
 *   fightTrainerAiWazaValueKuroikiri (0xA90 bytes): Large battle orchestration function.
 *     Manages the high-level flow of a battle encounter including
 *     pre-battle dialogue, party selection, and battle initiation.
 *
 *   fightTrainerAiWazaValueHimitunotikara (0x1084 bytes): Second-largest in this region.
 *     Complex battle setup that handles multi-round encounters.
 *     The code shows repeated patterns of:
 *       CheckTrainerPokemonFlag -> PreBattleSetup -> BattleSequenceStart
 *     with different sequence IDs (0xF1, 0xF2, 0xF3, 0xF4).
 *
 *   fightTrainerAiSelectIrekaeDasuFightPokemon (0x1224 bytes): Third-largest. Appears to handle
 *     post-battle processing including experience distribution and
 *     potential Shadow Pokemon purification progress.
 *
 * BATTLE SEQUENCE PATTERN:
 *   The calling convention throughout this module follows a strict pattern
 *   visible at 0x80249000:
 *
 *     1. GetTrainerPokemonPtr(ctx) -> pokemonPtr
 *     2. PreBattleSetup(ctx, trainerSlot, seqId) -> setupHandle
 *     3. CheckTrainerPokemonFlag(trainerSlot, partySlot, flagId)
 *     4. If flag set:
 *        BattleSequenceStart(0xEC64, trainerSlot, pokemonPtr,
 *                            0, 0, contextHandle, 0, seqId)
 *
 *   The constant 0xEC64 is constructed as `lis r6, 1; subi r3, r6, 0x139C`
 *   and appears to be a standard battle configuration code.
 *
 *   Sequence IDs observed:
 *     0xF1 = First round / initial encounter
 *     0xF2 = Second round / continuation
 *     0xF3 = Third round / continuation
 *     0xF4 = Final round
 *
 *   After each sequence:
 *     CheckTrainerPokemonFlag with flagId 0x18, 0x1E, 0x07 to determine
 *     if additional rounds are needed.
 *
 * REWARD/EXPERIENCE PROCESSING (0x8024D000-0x80260000):
 *
 *   This is the densest code region, with many functions in the
 *   0x80250000-0x80260000 range (500+ functions in 64KB). The
 *   function density suggests these are individual reward/stat/check
 *   handlers, each dealing with a specific aspect of post-battle
 *   processing.
 *
 *   Key address clusters:
 *     0x80250000-0x80254000: ~170 functions, likely experience calculation
 *       and level-up processing for each Pokemon
 *     0x80254000-0x80258000: ~100 functions, likely item rewards and
 *       Poke Coupon processing
 *     0x80258000-0x8025C000: ~80 functions, likely team state updates
 *       (PP restoration, status healing)
 *     0x8025C000-0x80260000: ~80 functions, likely shadow Pokemon
 *       purification gauge updates
 *
 * UTILITY REGION (0x80260000-0x80266360):
 *
 *   fightMenuFightTrainerGcHeroOpenMenu (0xA5C bytes): Large utility function near the end.
 *     This is one of the last significant functions before the .ctors
 *     section at 0x80266360.
 *
 *   The remaining functions (0x80260000-0x80266360) appear to be
 *   miscellaneous helpers: string formatting, data validation,
 *   debug helpers, and cleanup functions.
 *
 * lbl_8047B610 USAGE (script PC):
 *   Throughout this module, lbl_8047B610 is incremented heavily:
 *     lwz r3, lbl_8047B610@sda21(r0)
 *     addi r0, r3, 1
 *     stw r0, lbl_8047B610@sda21(r0)
 *
 *   or with skip:
 *     lwz r3, lbl_8047B610@sda21(r0)
 *     addi r0, r3, 5
 *     stw r0, lbl_8047B610@sda21(r0)
 *
 *   The +5 skips happen after successful condition checks, jumping
 *   over what would be a 4-byte operand + 1-byte opcode.
 *
 *   Sometimes the PC is set to a loaded value:
 *     lwz r3, lbl_8047B610@sda21(r0)
 *     lwz r0, 0x1(r3)    ; read next 4 bytes as target
 *     stw r0, lbl_8047B610@sda21(r0)  ; absolute jump
 *
 *   This confirms a bytecode format where:
 *     Byte 0: opcode
 *     Bytes 1-4: optional 32-bit operand (jump target, value, etc.)
 *
 * Note: this file previously carried two unreferenced fictional
 * definitions -- ShadowPokemonCheck and CheckBattleCondition -- claiming
 * addresses fightTrainerAiWazaHit046 and fightTrainerAiWazaHit047. Both addresses are actually
 * implemented and matched at 100% elsewhere in this file (Ghidra-import
 * block); the fictional duplicates have been removed. CheckTrainerPokemonFlag
 * (fn_80236BFC) remains undecompiled -- its only caller was the removed
 * CheckBattleCondition fiction.
 *
 * =========================================================================
 */

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

/* =========================================================================
 * External declarations
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

/* =========================================================================
 * fightTrainerAiWazaValueKuroikiri - BattleOrchestrator
 *
 * Large battle orchestration function (0xA90 = 2704 bytes).
 *
 * Manages the complete flow of a battle encounter:
 *   1. Pre-battle: Load opponent data, validate teams
 *   2. Initiation: Call BattleSequenceStart with appropriate IDs
 *   3. Monitoring: Check battle status and handle interrupts
 *   4. Completion: Process results and update state
 *
 * This function acts as the bridge between the script system
 * (colosseum_script.c) and the battle engine (battle_main.c).
 * ========================================================================= */
int fightTrainerAiWazaValueKuroikiri(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235AA0(void* ctx, u32 elem);
    extern u8 fn_80235A3C(void* ctx, u32 elem);
    extern u8 fn_802359D8(void* ctx, u32 elem);
    extern u8 fn_80235974(void* ctx, u32 elem);
    extern u8 fn_80235910(void* ctx, u32 elem);
    extern u8 fn_802358AC(void* ctx, u32 elem);
    extern u8 fn_802357CC(void* ctx, u32 elem);
    extern u16 fightFloorGetFightTrainerFightOutPokemonPtrAry(int a, void* ctx, u32* buf, int b, int c);
    extern int fn_80239984(int handle, void* ctx, int seq);
    extern u32 fightOutPokemonGetPokemonPtr(u32 v);
    extern void fn_80239EE8(int a, void* ctx, u32 v, int b, int c, u32 d, int e, int seq);
    u32 array1[10];
    u32 array2[8];
    u8 bufA[8];
    u8 bufB[8];
    u8 bufC[8];
    u8 bufD[8];
    u8 bufE[8];
    u8 bufF[8];
    u8 bufG[8];
    u8 bufH[8];
    u16 count1;
    u16 count2;
    int handle;
    u32 elem;
    u16 i;
    u8 j;

    handle = 0;
    count1 = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, array1, 1, 1);
    count2 = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, array2, 0, 1);

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count2; i++) {
            elem = array2[i];
            bufA[0] = fn_80235AA0(ctx, elem);
            bufA[1] = fn_80235A3C(ctx, elem);
            bufA[2] = fn_802359D8(ctx, elem);
            bufA[3] = fn_80235974(ctx, elem);
            bufA[4] = fn_80235910(ctx, elem);
            bufA[5] = fn_802358AC(ctx, elem);
            bufA[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if (bufA[j] >= 8 && bufA[j] <= 9) { matched = 1; goto M0; }
            }
            matched = 0;
        M0:
            if (matched == 1) { found = 1; goto L0; }
        }
        found = 0;
    L0:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1b6);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1b6);
        }
    }

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count1; i++) {
            elem = array1[i];
            bufB[0] = fn_80235AA0(ctx, elem);
            bufB[1] = fn_80235A3C(ctx, elem);
            bufB[2] = fn_802359D8(ctx, elem);
            bufB[3] = fn_80235974(ctx, elem);
            bufB[4] = fn_80235910(ctx, elem);
            bufB[5] = fn_802358AC(ctx, elem);
            bufB[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if (bufB[j] >= 3 && bufB[j] <= 4) { matched = 1; goto M1; }
            }
            matched = 0;
        M1:
            if (matched == 1) { found = 1; goto L1; }
        }
        found = 0;
    L1:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1b7);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1b7);
        }
    }

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count2; i++) {
            elem = array2[i];
            bufC[0] = fn_80235AA0(ctx, elem);
            bufC[1] = fn_80235A3C(ctx, elem);
            bufC[2] = fn_802359D8(ctx, elem);
            bufC[3] = fn_80235974(ctx, elem);
            bufC[4] = fn_80235910(ctx, elem);
            bufC[5] = fn_802358AC(ctx, elem);
            bufC[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if (bufC[j] >= 10 && bufC[j] <= 12) { matched = 1; goto M2; }
            }
            matched = 0;
        M2:
            if (matched == 1) { found = 1; goto L2; }
        }
        found = 0;
    L2:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1b8);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1b8);
        }
    }

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count1; i++) {
            elem = array1[i];
            bufD[0] = fn_80235AA0(ctx, elem);
            bufD[1] = fn_80235A3C(ctx, elem);
            bufD[2] = fn_802359D8(ctx, elem);
            bufD[3] = fn_80235974(ctx, elem);
            bufD[4] = fn_80235910(ctx, elem);
            bufD[5] = fn_802358AC(ctx, elem);
            bufD[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if ((s32)(u8)bufD[j] >= 0 && (s32)(u8)bufD[j] <= 2) { matched = 1; goto M3; }
            }
            matched = 0;
        M3:
            if (matched == 1) { found = 1; goto L3; }
        }
        found = 0;
    L3:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1b9);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1b9);
        }
    }

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count1; i++) {
            elem = array1[i];
            bufE[0] = fn_80235AA0(ctx, elem);
            bufE[1] = fn_80235A3C(ctx, elem);
            bufE[2] = fn_802359D8(ctx, elem);
            bufE[3] = fn_80235974(ctx, elem);
            bufE[4] = fn_80235910(ctx, elem);
            bufE[5] = fn_802358AC(ctx, elem);
            bufE[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if (bufE[j] >= 8 && bufE[j] <= 9) { matched = 1; goto M4; }
            }
            matched = 0;
        M4:
            if (matched == 1) { found = 1; goto L4; }
        }
        found = 0;
    L4:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1ba);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1ba);
        }
    }

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count2; i++) {
            elem = array2[i];
            bufF[0] = fn_80235AA0(ctx, elem);
            bufF[1] = fn_80235A3C(ctx, elem);
            bufF[2] = fn_802359D8(ctx, elem);
            bufF[3] = fn_80235974(ctx, elem);
            bufF[4] = fn_80235910(ctx, elem);
            bufF[5] = fn_802358AC(ctx, elem);
            bufF[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if (bufF[j] >= 3 && bufF[j] <= 4) { matched = 1; goto M5; }
            }
            matched = 0;
        M5:
            if (matched == 1) { found = 1; goto L5; }
        }
        found = 0;
    L5:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1bb);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1bb);
        }
    }

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count1; i++) {
            elem = array1[i];
            bufG[0] = fn_80235AA0(ctx, elem);
            bufG[1] = fn_80235A3C(ctx, elem);
            bufG[2] = fn_802359D8(ctx, elem);
            bufG[3] = fn_80235974(ctx, elem);
            bufG[4] = fn_80235910(ctx, elem);
            bufG[5] = fn_802358AC(ctx, elem);
            bufG[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if (bufG[j] >= 10 && bufG[j] <= 12) { matched = 1; goto M6; }
            }
            matched = 0;
        M6:
            if (matched == 1) { found = 1; goto L6; }
        }
        found = 0;
    L6:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1bc);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1bc);
        }
    }

    {
        u8 found;
        u8 matched;
        for (i = 0; i < count2; i++) {
            elem = array2[i];
            bufH[0] = fn_80235AA0(ctx, elem);
            bufH[1] = fn_80235A3C(ctx, elem);
            bufH[2] = fn_802359D8(ctx, elem);
            bufH[3] = fn_80235974(ctx, elem);
            bufH[4] = fn_80235910(ctx, elem);
            bufH[5] = fn_802358AC(ctx, elem);
            bufH[6] = fn_802357CC(ctx, elem);
            for (j = 0; j < 7; j++) {
                if ((s32)(u8)bufH[j] >= 0 && (s32)(u8)bufH[j] <= 2) { matched = 1; goto M7; }
            }
            matched = 0;
        M7:
            if (matched == 1) { found = 1; goto L7; }
        }
        found = 0;
    L7:
        if (found == 1) {
            handle = fn_80239984(handle, ctx, 0x1bd);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1bd);
        }
    }
    return handle;
}

/* =========================================================================
 * fightTrainerAiWazaValueHimitunotikara - MultiBattleSetup
 *
 * Handles multi-round battle encounters (0x1084 = 4228 bytes).
 *
 * The function shows a clear pattern of setting up multiple battle
 * rounds in sequence, checking after each round whether additional
 * rounds are needed:
 *
 *   for each potential round:
 *     if (CheckTrainerPokemonFlag(slot, slot2, roundCheckFlag)):
 *       PreBattleSetup(ctx, slot, roundSeqId)
 *       GetTrainerPokemonPtr(pokemonCtx) -> ptr
 *       BattleSequenceStart(0xEC64, slot, ptr, 0, 0, ctxHandle, 0, seqId)
 *
 * Round check flags: 0x18 (round 1), 0x1E (round 2), 0x07 (round 3)
 * ========================================================================= */
/* TODO: Decompile fightTrainerAiWazaValueHimitunotikara (4228 bytes) */
void fightTrainerAiWazaValueHimitunotikara(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80136468();
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fn_801F54A4();
    extern void fn_801FB1C0();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_802358AC();
    extern void fn_80235910();
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_802384B4();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fightTrainerAiAddValue();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    extern void fightTrainerAiWazaValueTuikaHirumi();
    extern u8 jumptable_8039A5D8[];
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r29 = r4;
    r30 = r5;
    r28 = r6;
    r24 = 0x0;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0xf;
    r6 = 0x0;
    fn_801F54A4();
    r3 = r3 & 0xFFFF;
    fn_80136468();
    r0 = r3 & 0xFF;
    if (r0 > (u32)0x1b) { r3 = r24; return; }
    r3 = (u32)jumptable_8039A5D8;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8039A5D8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    r3 = r31;
    r4 = r30;
    fn_80239564();
    r25 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x113;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r25 = (s32)r25 / (s32)r3;
    r3 = 0x0;
    r4 = r25;
    fightTrainerAiAddValue();
    r26 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x113;
    fn_80239CCC();
    r4 = r31;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x114;
        fn_80239984();
        r26 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x114;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x11;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r31;
            r4 = r28;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
        }
        }
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0x115;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r26;
        r4 = r24;
        fightTrainerAiAddValue();
        r26 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x115;
        fn_80239CCC();
            }
    r24 = r26;
    r3 = r24;
    return;
    r4 = r31;
    r5 = (u32)sp + 0x10;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1A6C();
    r26 = r3;
    r3 = r31;
    r4 = r30;
    fn_80239564();
    r24 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x10d;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r24 = (s32)r24 / (s32)r3;
    r3 = 0x0;
    r4 = r24;
    fightTrainerAiAddValue();
    r27 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10d;
    fn_80239CCC();
    r25 = (u32)sp + 0x10;
    r26 = r26 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r26) break;
        r3 = r28;
        r4 = 0x0;
        r5 = 0xd5;
        r6 = 0x0;
        ((void(*)(void))pokemonGetStatus)();
        r4 = *(u32*)(r25 + r0);
        if (r3 != (u32)r4) {
            r3 = r31;
            r5 = 0x8;
            fn_802384B4();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r31;
                r4 = r30;
                fn_80239564();
                r24 = r3 & 0xFF;
                r3 = 0x0;
                r4 = 0x10e;
                r5 = 0x3e;
                r6 = 0x0;
                fn_801FB1C0();
                r24 = (s32)r24 / (s32)r3;
                r3 = r27;
                r4 = r24;
                fightTrainerAiAddValue();
                r27 = r3;
                r3 = r29;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r31;
                r8 = r30;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x10e;
                fn_80239CCC();
                break;
        }
        }
        r24 = r24 + 0x1;

    }

    r3 = r31;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 != (u32)r26) {
        r3 = r31;
        r4 = r28;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r31;
            r4 = r28;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
        }
        }
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0x10f;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x10f;
        fn_80239CCC();
            }
    r24 = r27;
    r3 = r24;
    return;
    r3 = r31;
    r4 = r30;
    fn_80239564();
    r24 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xdb;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r24 = (s32)r24 / (s32)r3;
    r3 = 0x0;
    r4 = r24;
    fightTrainerAiAddValue();
    r27 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xdb;
    fn_80239CCC();
    r4 = r31;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xdc;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xdc;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xdd;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xdd;
        fn_80239CCC();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r31;
            r4 = r28;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r3 = r31;
                r4 = r28;
                r5 = 0x33;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
        }
        }
        }
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xde;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xde;
        fn_80239CCC();
                }
    r24 = r27;
    r3 = r24;
    return;
    r3 = r31;
    r4 = r30;
    fn_80239564();
    r24 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xc3;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r24 = (s32)r24 / (s32)r3;
    r3 = 0x0;
    r4 = r24;
    fightTrainerAiAddValue();
    r27 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc3;
    fn_80239CCC();
    r4 = r31;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc4;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc4;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xc5;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc5;
        fn_80239CCC();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r31;
            r4 = r28;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
        }
        }
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xc6;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc6;
        fn_80239CCC();
            }
    r24 = r27;
    r3 = r24;
    return;
    r4 = r31;
    r3 = 0x0;
    r5 = 0xbf;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbf;
    fn_80239EE8();
    r4 = r31;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc0;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc0;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc1;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc1;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r31;
            r4 = r28;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r3 = r31;
                r4 = r28;
                r5 = 0x34;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
        }
        }
        }
        r3 = r27;
        r4 = r31;
        r5 = 0xc2;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc2;
        fn_80239EE8();
                }
    r24 = r27;
    r3 = r24;
    return;
    r3 = r31;
    r4 = r30;
    fn_80239564();
    r24 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xc7;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r24 = (s32)r24 / (s32)r3;
    r3 = 0x0;
    r4 = r24;
    fightTrainerAiAddValue();
    r27 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc7;
    fn_80239CCC();
    r4 = r31;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc8;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc8;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc9;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc9;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    fn_80235910();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xca;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xca;
        fn_80239CCC();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r31;
            r4 = r28;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
        }
        }
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xcb;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xcb;
        fn_80239CCC();
            }
    r24 = r27;
    r3 = r24;
    return;
    r3 = r31;
    r4 = r30;
    fn_80239564();
    r24 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xe3;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r24 = (s32)r24 / (s32)r3;
    r3 = 0x0;
    r4 = r24;
    fightTrainerAiAddValue();
    r27 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe3;
    fn_80239CCC();
    r4 = r31;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xe4;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe4;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xe5;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe5;
        fn_80239CCC();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
        }
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0xe6;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r27;
        r4 = r24;
        fightTrainerAiAddValue();
        r27 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe6;
        fn_80239CCC();
        }
    r24 = r27;
    r3 = r24;
    return;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    r6 = r28;
    fightTrainerAiWazaValueTuikaHirumi();
    r24 = r3;
    r3 = r24;
    return;
    r3 = r31;
    r4 = r30;
    fn_80239564();
    r24 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x104;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r24 = (s32)r24 / (s32)r3;
    r3 = 0x0;
    r4 = r24;
    fightTrainerAiAddValue();
    r25 = r3;
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x104;
    fn_80239CCC();
    r4 = r31;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r25;
        r4 = r31;
        r5 = 0x105;
        fn_80239984();
        r25 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x105;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r31;
            r4 = r28;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
        }
        }
        r3 = r31;
        r4 = r30;
        fn_80239564();
        r24 = r3 & 0xFF;
        r3 = 0x0;
        r4 = 0x106;
        r5 = 0x3e;
        r6 = 0x0;
        fn_801FB1C0();
        r24 = (s32)r24 / (s32)r3;
        r3 = r25;
        r4 = r24;
        fightTrainerAiAddValue();
        r25 = r3;
        r3 = r29;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x106;
        fn_80239CCC();
            }
    r24 = r25;

    r3 = r24;
    return;
}

/* =========================================================================
 * fightTrainerAiSelectIrekaeDasuFightPokemon - PostBattleProcessing
 *
 * Large post-battle handler (0x1224 = 4644 bytes).
 *
 * Processes the results of a completed battle:
 *   - Experience calculation and distribution
 *   - Shadow Pokemon purification progress
 *   - Prize money / Poke Coupon rewards
 *   - Story flag updates
 *   - Team state restoration (PP, status)
 * ========================================================================= */
/* TODO: Decompile fightTrainerAiSelectIrekaeDasuFightPokemon (4644 bytes) */
s32 fightTrainerAiSelectIrekaeDasuFightPokemon(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A434[];
    extern u32 lbl_80478B38;
    extern void fn_8000815C();
    extern void fn_800E0C54();
    extern void fn_801F1A6C();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_801F4460();
    extern void fn_801F87CC();
    extern void fn_801F8C00();
    extern void fn_801FB1C0();
    extern void fn_801FCEC4();
    extern void fightOutPokemonIsFightActionAttackWazaOut();
    extern void fightPokemonGetPokemonPtr();
    extern void fightOutPokemonCheckFightOut();
    extern void fightOutPokemonCreate();
    extern void fn_80235B04();
    extern void fn_802367CC();
    extern void fn_802369B8();
    extern void fn_8023793C();
    extern void fn_80238538();
    extern void fn_80238600();
    extern void fn_802386C8();
    extern void fn_8023881C();
    extern void fn_802389D4();
    extern void fn_80238B0C();
    extern void fn_80238E30();
    extern void fn_80239058();
    extern void fn_8023943C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_802397B8();
    extern void fn_802398E4();
    extern void fn_80239984();
    extern void fightTrainerAiAddValue();
    extern void fn_80239A40();
    extern void fn_80239EE8();
    extern void fn_8023A118();
    extern void fn_8023C530();
    extern void fn_8024FE80();
    u8 sp[0x860];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;
    f32 f8 = 0.0f;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = 0x0;
    r5 = 0x43;
    r16 = r6;
    r15 = r3;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r21 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = r21;
    r5 = 0x38;
    r6 = 0x0;
    fn_801FB1C0();
    r5 = (u32)lbl_8027A434;
    r0 = r3 & 0xFF;
    r5 = (u32)lbl_8027A434;
    r4 = 0x3;
    *(u8*)(sp + 0x7F0) = r0;
    r6 = (u32)sp + 0x14;
    r14 = 0x0;
    ctr_fn = (void(*)(void))r4;
    do {
        r3 = *(u32*)((u8*)r5 + 0x4);
        r0 = *(u32*)((u8*)r5 + 0x8);
        *(u32*)((u8*)r6 + 0x4) = r3;
        r6 += 8; *(u32*)r6 = r0;
    } while (--ctr != 0);
    r0 = *(u32*)((u8*)r5 + 0x4);
    r3 = r15;
    r4 = 0x0;
    r5 = 0x1;
    *(u32*)((u8*)r6 + 0x4) = r0;
    fn_80235B04();
    r3 = (u32)sp + 0x68;
    r5 = 0x0;
    r4 = 0x0;
    while (1) {
        r0 = r5 & 0xFFFF;
        if (r0 >= (u32)0x6) break;
        r5 = r5 + 0x1;
        *(u32*)(r3 + r0) = r4;

    }
    r4 = r16;
    r3 = (u32)sp + 0x110;
    fn_801FCEC4();
    r3 = r16;
    fightOutPokemonCheckFightOut();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r16;
        r4 = 0xe2;
        r5 = 0x0;
        fightOutPokemonIsFightActionAttackWazaOut();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r14 = 0x1;
    }
    }
    r3 = r15;
    r4 = (u32)sp + 0x98;
    fn_801F87CC();
    r20 = r3;
    r4 = r15;
    r5 = (u32)sp + 0x34;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r4 = r15;
    r5 = (u32)sp + 0xb0;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r0 = r20 & 0xFFFF;
    if (r0 == (u32)0x1) {
        r3 = -0x1;
        return;
    }
    r4 = r21;
    r3 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        r5 = (0x1 << 16);
        *(u32*)(sp + 0x8) = r0;
        r6 = r15;
        *(u32*)(sp + 0xC) = r0;
        r7 = 0x0;
        r8 = 0x0;
        *(u32*)(sp + 0x10) = r0;
        r9 = 0x0;
        r10 = 0x0;
        fn_8023A118();
        fn_800E0C54();
        r5 = r3 & 0xFFFF;
        r4 = r20 & 0xFFFF;
        r0 = (s32)r5 / (s32)r4;
        r3 = (u32)sp + 0x98;
        r0 = r0 * r4;
        r0 = r5 - r0;
        r17 = *(u32*)(r3 + r0);
        if (r17 != (u32)0x0) {
            r4 = r17;
            r3 = 0x0;
            fn_801F4460();
            r0 = r3;
            r3 = r17;
            r14 = r0;
            fightPokemonGetPokemonPtr();
            r8 = 0x0;
            r5 = (0x1 << 16);
            r0 = 0x228;
            r7 = r3;
            r6 = r14;
            *(u32*)(sp + 0xC) = r0;
            r8 = 0x0;
            r9 = 0x0;
            r10 = 0x0;
            fn_8023A118();
            r3 = r17;
            r4 = 0x0;
            r5 = 0xce;
            r6 = 0x0;
            ((void(*)(void))pokemonGetStatus)();
            r3 = (s16)r3;
            return;
    }
    }
    r0 = r14 & 0xFF;
    if (r0 == (u32)0x1) {
        r14 = (u32)sp + 0x98;
        r17 = (u32)sp + 0x80;
        r18 = (u32)sp + 0x18;
        r19 = 0x0;
        r26 = 0x0;
        while (1) {
            r0 = r26 & 0xFFFF;
            if (r0 >= (u32)0xe) break;
            r4 = 0x0;
            r0 = r3 + 0x2;
            r23 = *(s16*)(r18 + r3);
            r24 = *(s16*)(r18 + r0);
            r3 = r4;
            while (1) {
                r0 = r4 & 0xFFFF;
                if (r0 >= (u32)0x6) break;
                r4 = r4 + 0x1;
                *(u32*)(r17 + r0) = r3;

            }
            r22 = 0x0;
            r25 = r22;
            while (1) {
                r0 = r25 & 0xFFFF;
                if (r0 >= (u32)0x6) break;
                r19 = *(u32*)(r14 + r0);
                if (r19 != (u32)0x0 || (s32)r23 != (s32)r0 || (s32)r24 != (s32)r0) {
                    r0 = (s16)r23;
                    if (r19 >= (u32)0x0) {
                        r3 = r15;
                        r4 = r19;
                        fn_80238600();
                        r0 = r3 & 0xFF;

                    }
                    r0 = (s16)r24;
                    if ((s32)r23 >= (s32)r0) {
                        r3 = r15;
                        r4 = r19;
                        fn_80238538();
                        r0 = r3 & 0xFF;

                    }
                    r3 = (u32)sp + 0x80;
                    *(u32*)(r3 + r0) = r19;
                    r22 = r22 + 0x1;
                }
                r25 = r25 + 0x1;

            }
            r0 = r22 & 0xFFFF;
            r19 = r22;
            if (r0 != (u32)0x6) break;
            r26 = r26 + 0x2;

        }

        r0 = r19 & 0xFFFF;
        if (r0 != (u32)0xe) {
            fn_800E0C54();
            r5 = r3 & 0xFFFF;
            r4 = r19 & 0xFFFF;
            r0 = (s32)r5 / (s32)r4;
            r3 = (u32)sp + 0x80;
            r0 = r0 * r4;
            r0 = r5 - r0;
            r14 = *(u32*)(r3 + r0);
            if (r14 != (u32)0x0) {
                r4 = r14;
                r3 = 0x0;
                fn_801F4460();
                r15 = r3;
                r3 = r14;
                fightPokemonGetPokemonPtr();
                r8 = 0x0;
                r5 = (0x1 << 16);
                r0 = 0x228;
                r7 = r3;
                r6 = r15;
                *(u32*)(sp + 0xC) = r0;
                r8 = 0x0;
                r9 = 0x0;
                r10 = 0x0;
                fn_8023A118();
                r3 = r14;
                r4 = 0x0;
                r5 = 0xce;
                r6 = 0x0;
                ((void(*)(void))pokemonGetStatus)();
                r3 = (s16)r3;
                return;
    }
    }
    }
    r4 = (0xffff << 16);
    r3 = (0x1 << 16);
    r18 = r4 + 0x1;
    r22 = (u32)sp + 0x98;
    r17 = r20 & 0xFFFF;
    r19 = 0x0;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r17) break;
        r24 = *(u32*)(r22 + r0);
        if (r24 != (u32)0x0) {
            r3 = r24;
            r4 = 0x0;
            r5 = 0xce;
            r6 = 0x0;
            ((void(*)(void))pokemonGetStatus)();
            r0 = (s16)r3;
            if (r24 >= (u32)0x0) {
                r3 = r15;
                r4 = r24;
                fn_802386C8();
                r0 = r3;
                r3 = r15;
                r25 = r0;
                r4 = r24;
                fn_802389D4();
                r0 = r3;
                r3 = r24;
                r24 = r0;
                fightPokemonGetPokemonPtr();
                r4 = 0x0;
                r5 = 0xc9;
                r6 = 0x0;
                ((void(*)(void))pokemonGetStatus)();
                r3 = r3 & 0xFFFF;
                if (r19 < r25) {
                    r19 = r25;
                }
                if ((s32)r18 < (s32)r24) {
                    r18 = r24;
                }
                r0 = r14 & 0xFFFF;
                if (r0 > r3) {
                    r14 = r3;
        }
        }
        }
        r23 = r23 + 0x1;

    }
    r4 = r21;
    r3 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r22 = (u32)sp + 0x98;
        r17 = r20 & 0xFFFF;
        r23 = 0x0;
        while (1) {
            r0 = r23 & 0xFFFF;
            if (r0 >= (u32)r17) break;
            r24 = *(u32*)(r22 + r0);
            if (r24 != (u32)0x0) {
                r3 = r24;
                r4 = 0x0;
                r5 = 0xce;
                r6 = 0x0;
                ((void(*)(void))pokemonGetStatus)();
                r0 = (s16)r3;
                if (r24 >= (u32)0x0) {
                    r4 = r24;
                    r3 = 0x0;
                    fn_801F4460();
                    r3 = r24;
                    fightPokemonGetPokemonPtr();
                    r4 = 0x0;
                    r5 = 0xc9;
                    r6 = 0x0;
                    ((void(*)(void))pokemonGetStatus)();
                    r3 = r3 & 0xFFFF;
                    r0 = r14 & 0xFFFF;
                    if (r0 >= (u32)r3) {
                        r4 = r24;
                        r3 = 0x0;
                        fn_801F4460();
                        r14 = r3;
                        r3 = r24;
                        fightPokemonGetPokemonPtr();
                        r8 = 0x0;
                        r5 = (0x1 << 16);
                        r0 = 0x228;
                        r7 = r3;
                        r6 = r14;
                        *(u32*)(sp + 0xC) = r0;
                        r8 = 0x0;
                        r9 = 0x0;
                        r10 = 0x0;
                        fn_8023A118();
                        r3 = r24;
                        r4 = 0x0;
                        r5 = 0xce;
                        r6 = 0x0;
                        ((void(*)(void))pokemonGetStatus)();
                        r3 = (s16)r3;
                        return;
            }
            }
            }
            r23 = r23 + 0x1;

        }
    }
    r0 = *(u8*)(sp + 0x7F0);
    r28 = 0x0;
    r0 = r3 + 0x1;
    *(u32*)(sp + 0x7F4) = r0;
    r0 = r20 & 0xFFFF;
    *(u32*)(sp + 0x808) = r0;
    while (1) {
        r0 = *(u32*)(sp + 0x808);
        r3 = r28 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        r3 = (u32)sp + 0x98;
        r27 = *(u32*)(r3 + r30);
        if (r27 != (u32)0x0) {
            r3 = r27;
            r4 = 0x0;
            r5 = 0xce;
            r6 = 0x0;
            ((void(*)(void))pokemonGetStatus)();
            r0 = (s16)r3;
            if (r27 >= (u32)0x0) {
                r4 = r27;
                r3 = 0x0;
                fn_801F4460();
                r26 = r3;
                r3 = r16;
                r4 = r27;
                r5 = 0x0;
                fightOutPokemonCreate();
                r3 = r15;
                r4 = r16;
                r5 = (u32)sp + 0x54;
                r6 = 0x0;
                r7 = 0x1;
                fn_802367CC();
                r0 = *(u32*)(sp + 0x7FC);
                r23 = r3;
                r31 = (u32)sp + 0x34;
                r14 = 0x0;
                r17 = r0 & 0xFFFF;
                r25 = 0x0;
                while (1) {
                    r0 = r25 & 0xFFFF;
                    if (r0 >= (u32)r17) break;
                    r29 = *(u32*)(r31 + r0);
                    if (r29 != (u32)0x0) {
                        r22 = r23 & 0xFFFF;
                        r24 = 0x0;
                        while (1) {
                            r0 = r24 & 0xFFFF;
                            if (r0 >= (u32)r22) break;
                            r3 = (u32)sp + 0x54;
                            r5 = *(u16*)(r3 + r0);
                            if (r5 == (u32)0x0 || r5 == (u32)0x165 || r0 == (u32)0x1) {

                                r3 = r15;
                                r4 = r16;
                                r6 = r29;
                                fn_8023C530();
                                r0 = r3 & 0xFF;

                                r14 = 0x1;
                                break;
                            }
                            r24 = r24 + 0x1;

                        }

                        r0 = r14 & 0xFF;
                        if (r0 == (u32)0x1) break;
                    }
                    r25 = r25 + 0x1;

                }

                r3 = r16;
                r4 = (u32)sp + 0x110;
                fn_801FCEC4();
                r3 = r15;
                r4 = r27;
                r5 = (u32)sp + 0x54;
                r6 = 0x0;
                r7 = 0x1;
                fn_802369B8();
                r3 = r27;
                fightPokemonGetPokemonPtr();
                r0 = 0x0;
                r5 = (0x1 << 16);
                *(u32*)(sp + 0x8) = r0;
                r0 = 0x227;
                r7 = r3;
                r29 = (u32)sp + 0x68;
                *(u32*)(sp + 0xC) = r0;
                r6 = r26;
                r0 = *(u32*)(r29 + r30);
                r8 = 0x0;
                r9 = 0x0;
                *(u32*)(sp + 0x10) = r0;
                r10 = 0x0;
                fn_8023A118();
                r3 = r15;
                r4 = r27;
                fn_80238600();
                r17 = r3;
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x1a;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x1a;
                    fn_80239EE8();
                }
                r0 = r17 & 0xFF;
                if (r0 == (u32)0x2) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x1b;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x1b;
                    fn_80239EE8();
                }
                r0 = r17 & 0xFF;
                if (r0 == (u32)0x3) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x1c;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x1c;
                    fn_80239EE8();
                }
                r17 = 0x0;
                while (1) {
                    r0 = r17 & 0xFFFF;
                    if (r0 >= (u32)0x2) break;
                    r4 = r21;
                    r6 = r17;
                    r3 = 0x0;
                    r5 = 0x39;
                    fn_801FB1C0();
                    r5 = r3 & 0xFFFF;
                    if (r5 != (u32)0x9) {
                        r3 = r15;
                        r4 = r27;
                        fn_80238E30();
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r4 = r21;
                            r6 = r17;
                            r3 = 0x0;
                            r5 = 0x3a;
                            fn_801FB1C0();
                            r22 = r3 & 0xFF;
                            r3 = *(u32*)(r29 + r30);
                            r4 = r22;
                            r5 = r15;
                            r6 = 0x1d;
                            fn_802398E4();
                            *(u32*)(r29 + r30) = r3;
                            r3 = r27;
                            fightPokemonGetPokemonPtr();
                            r6 = (0x1 << 16);
                            r5 = r3;
                            r4 = r15;
                            r6 = 0x0;
                            r7 = 0x0;
                            r8 = 0x0;
                            r9 = 0x0;
                            r10 = 0x1d;
                            fn_80239A40();
                    }
                    }
                    r17 = r17 + 0x1;

                }
                r3 = r15;
                r4 = r27;
                fn_8023881C();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x1e;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x1e;
                    fn_80239EE8();
                }
                r0 = r14 & 0xFF;
                if (r0 == (u32)0x1) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x1f;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x1f;
                    fn_80239EE8();
                }
                r3 = r15;
                r4 = r27;
                fn_802386C8();
                if (r19 <= (u32)r3) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x21;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x21;
                    fn_80239EE8();
                }
                r3 = r15;
                r4 = r27;
                fn_802389D4();
                if ((s32)r18 <= (s32)r3) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x20;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x20;
                    fn_80239EE8();
                }
                r0 = *(u32*)(sp + 0x7FC);
                r24 = 0x0;
                r14 = r0 & 0xFFFF;
                while (1) {
                    r0 = r24 & 0xFFFF;
                    if (r0 >= (u32)r14) break;
                    r3 = (u32)sp + 0x34;
                    r23 = *(u32*)(r3 + r0);
                    if (r23 != (u32)0x0) {
                        r0 = *(u32*)(sp + 0x804);
                        r25 = 0x0;
                        r31 = r0 & 0xFFFF;
                        while (1) {
                            r0 = r25 & 0xFFFF;
                            if (r0 >= (u32)r31) break;
                            r3 = (u32)sp + 0x54;
                            r22 = *(u16*)(r3 + r0);
                            if (r22 != (u32)0x0) {
                                r3 = r15;
                                r4 = r22;
                                r5 = r16;
                                fn_802395C8();
                                r0 = r3 & 0xFFFF;
                                r17 = r3;
                                if (r0 != (u32)0x9) {
                                    r3 = r15;
                                    r4 = r22;
                                    r5 = 0x1;
                                    fn_8023943C();
                                    r0 = r3 & 0xFF;
                                    if (r0 != (u32)0x9) {
                                        r3 = r15;
                                        r4 = r22;
                                        fn_80239500();
                                        r6 = r3;
                                        r3 = r15;
                                        r4 = r23;
                                        r5 = r17;
                                        fn_8023793C();
                                        r0 = r3 & 0xFFFF;
                                        if (r0 == (u32)0x41) {
                                            r3 = *(u32*)(r29 + r30);
                                            r4 = r15;
                                            r5 = 0x22;
                                            fn_80239984();
                                            *(u32*)(r29 + r30) = r3;
                                            r3 = r27;
                                            fightPokemonGetPokemonPtr();
                                            r7 = (0x1 << 16);
                                            r5 = r3;
                                            r4 = r26;
                                            r6 = 0x0;
                                            r7 = 0x0;
                                            r8 = 0x0;
                                            r9 = 0x0;
                                            r10 = 0x22;
                                            fn_80239EE8();
                            }
                            }
                            }
                            }
                            r25 = r25 + 0x1;

                        }
                    }
                    r24 = r24 + 0x1;

                }
                r0 = *(u32*)(sp + 0x7FC);
                r22 = 0x0;
                r31 = r0 & 0xFFFF;
                while (1) {
                    r0 = r22 & 0xFFFF;
                    if (r0 >= (u32)r31) break;
                    r3 = (u32)sp + 0x34;
                    r23 = *(u32*)(r3 + r0);
                    if (r23 != (u32)0x0) {
                        r3 = r15;
                        r4 = r23;
                        r5 = (u32)sp + 0x54;
                        r6 = 0x0;
                        r7 = 0x0;
                        fn_802367CC();
                        r14 = r3 & 0xFFFF;
                        r17 = 0x0;
                        while (1) {
                            r0 = r17 & 0xFFFF;
                            if (r0 >= (u32)r14) break;
                            r3 = (u32)sp + 0x54;
                            r24 = *(u16*)(r3 + r0);
                            if (r24 != (u32)0x0) {
                                r3 = r15;
                                r4 = r24;
                                r5 = r23;
                                fn_802395C8();
                                r0 = r3 & 0xFFFF;
                                r25 = r3;
                                if (r0 != (u32)0x9) {
                                    r3 = r15;
                                    r4 = r24;
                                    r5 = 0x1;
                                    fn_8023943C();
                                    r0 = r3 & 0xFF;
                                    if (r0 != (u32)0x9) {
                                        r3 = r15;
                                        r4 = r24;
                                        fn_80239500();
                                        r6 = r3;
                                        r3 = r15;
                                        r4 = r27;
                                        r5 = r25;
                                        fn_80238B0C();
                                        r0 = r3 & 0xFFFF;
                                        if (r0 == (u32)0x41) {
                                            r3 = *(u32*)(r29 + r30);
                                            r4 = r15;
                                            r5 = 0x23;
                                            fn_80239984();
                                            *(u32*)(r29 + r30) = r3;
                                            r3 = r27;
                                            fightPokemonGetPokemonPtr();
                                            r7 = (0x1 << 16);
                                            r5 = r3;
                                            r4 = r26;
                                            r6 = 0x0;
                                            r7 = 0x0;
                                            r8 = 0x0;
                                            r9 = 0x0;
                                            r10 = 0x23;
                                            fn_80239EE8();
                            }
                            }
                            }
                            }
                            r17 = r17 + 0x1;

                        }
                    }
                    r22 = r22 + 0x1;

                }
                r4 = r21;
                r3 = 0x0;
                r5 = 0x20;
                r6 = 0x0;
                fn_801FB1C0();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r17 = 0x0;
                    while (1) {
                        r0 = lbl_80478B38;
                        r3 = r17 & 0xFFFF;
                        if (r3 >= (u32)r0) break;
                        r0 = r17 & 0xFFFF;
                        if (r0 != (u32)0x9) {
                            r3 = r15;
                            r4 = r27;
                            r5 = r17;
                            r6 = 0x1;
                            fn_80238B0C();
                            r0 = r3 & 0xFFFF;
                            if (r0 == (u32)0x41) {
                                r0 = *(u32*)(sp + 0x800);
                                r14 = (u32)sp + 0xb0;
                                r22 = 0x0;
                                r23 = r0 & 0xFFFF;
                                while (1) {
                                    r0 = r22 & 0xFFFF;
                                    if (r0 >= (u32)r23) break;
                                    r24 = *(u32*)(r14 + r0);
                                    if (r24 != (u32)0x0) {
                                        r4 = r24;
                                        r3 = 0x0;
                                        fn_801F4460();
                                        if (r3 != (u32)0x0) {
                                            r4 = r24;
                                            fn_801F8C00();
                                            r0 = r3 & 0xFF;
                                    }
                                    }
                                    if (r0 != (u32)0x1 || r0 != (u32)0x2 && r0 != (u32)0x3 || r0 != (u32)0x2 && r0 != (u32)0x3) {

                                        if (r0 == (u32)0x2 || r0 == (u32)0x3) {

                                            r3 = r15;
                                            r4 = r24;
                                            r5 = r17;
                                            r6 = 0x1;
                                            fn_80238B0C();
                                            r0 = r3 & 0xFFFF;
                                            if (r0 == (u32)0x41) {
                                                r3 = *(u32*)(r29 + r30);
                                                r4 = r15;
                                                r5 = 0x24;
                                                fn_80239984();
                                                *(u32*)(r29 + r30) = r3;
                                                r3 = r27;
                                                fightPokemonGetPokemonPtr();
                                                r7 = (0x1 << 16);
                                                r5 = r3;
                                                r4 = r26;
                                                r6 = 0x0;
                                                r7 = 0x0;
                                                r8 = 0x0;
                                                r9 = 0x0;
                                                r10 = 0x24;
                                                fn_80239EE8();
                            }
                                        }
                                    }
                                    r22 = r22 + 0x1;

                                }
                    }
                        }
                        r17 = r17 + 0x1;

                    }
                }
                r3 = r15;
                r4 = r27;
                fn_8024FE80();
                r0 = r3 & 0xFFFF;
                r14 = r3;
                if (r3 != (u32)r0) {
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = r14;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r6 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r10 = r14;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    fn_80239EE8();
                }
                r0 = *(u32*)(sp + 0x7F8);
                r0 = r0 & 0xFF;
                if (r0 == (u32)0x2) {
                    r3 = r15;
                    r4 = r27;
                    r5 = 0x21;
                    fn_80239058();
                    r0 = r3 & 0xFF;
                    if (r0 != (u32)0x1) {
                        r3 = r15;
                        r4 = r27;
                        r5 = 0x2c;
                        fn_80239058();
                        r0 = r3 & 0xFF;
                        if (r0 != (u32)0x1) goto L_8024F710;
                    }
                    r3 = *(u32*)(r29 + r30);
                    r4 = r15;
                    r5 = 0x29;
                    fn_80239984();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x29;
                    fn_80239EE8();

                } else {
                    if (r0 == (u32)0x1) {
                        r3 = r15;
                        r4 = r27;
                        r5 = 0x22;
                        fn_80239058();
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r3 = *(u32*)(r29 + r30);
                            r4 = r15;
                            r5 = 0x2a;
                            fn_80239984();
                            *(u32*)(r29 + r30) = r3;
                            r3 = r27;
                            fightPokemonGetPokemonPtr();
                            r7 = (0x1 << 16);
                            r5 = r3;
                            r4 = r26;
                            r6 = 0x0;
                            r7 = 0x0;
                            r8 = 0x0;
                            r9 = 0x0;
                            r10 = 0x2a;
                            fn_80239EE8();
                        }
                    } else {
                    if (r0 == (u32)0x3) {
                        r3 = r15;
                        r4 = r27;
                        r14 = 0x0;
                        r5 = 0x8;
                        fn_80239058();
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r14 = 0x1;
                        }
                        r3 = r15;
                        r4 = r27;
                        r5 = 0x8;
                        fn_80238E30();
                        r0 = r3 & 0xFF;
                        if (r0 != (u32)0x1) {
                            r3 = r15;
                            r4 = r27;
                            r5 = 0x5;
                            fn_80238E30();
                            r0 = r3 & 0xFF;
                            if (r0 != (u32)0x1) {
                                r3 = r15;
                                r4 = r27;
                                r5 = 0x4;
                                fn_80238E30();
                                r0 = r3 & 0xFF;
                                if (r0 == (u32)0x1) {
                        }
                            }
                            r14 = 0x1;
                                }
                        r0 = r14 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r3 = *(u32*)(r29 + r30);
                            r4 = r15;
                            r5 = 0x2b;
                            fn_80239984();
                            *(u32*)(r29 + r30) = r3;
                            r3 = r27;
                            fightPokemonGetPokemonPtr();
                            r7 = (0x1 << 16);
                            r5 = r3;
                            r4 = r26;
                            r6 = 0x0;
                            r7 = 0x0;
                            r8 = 0x0;
                            r9 = 0x0;
                            r10 = 0x2b;
                            fn_80239EE8();
                        }
                        goto L_8024F710;
                    }
                    }
                    if (r0 == (u32)0x4) {
                        r3 = r15;
                        r4 = r27;
                        r5 = 0xf;
                        fn_80238E30();
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r3 = *(u32*)(r29 + r30);
                            r4 = r15;
                            r5 = 0x2c;
                            fn_80239984();
                            *(u32*)(r29 + r30) = r3;
                            r3 = r27;
                            fightPokemonGetPokemonPtr();
                            r7 = (0x1 << 16);
                            r5 = r3;
                            r4 = r26;
                            r6 = 0x0;
                            r7 = 0x0;
                            r8 = 0x0;
                            r9 = 0x0;
                            r10 = 0x2c;
                            fn_80239EE8();
                }
                    }
                }
            L_8024F710:
                fn_8000815C();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    fn_800E0C54();
                    r0 = *(u32*)(sp + 0x7F4);
                    r5 = r3 & 0xFFFF;
                    r3 = *(u32*)(r29 + r30);
                    r4 = (s32)r5 / (s32)r0;
                    r0 = r4 * r0;
                    r4 = r5 - r0;
                    r0 = *(u8*)(sp + 0x7F0);
                    r14 = r4 - r0;
                    r4 = r14;
                    fightTrainerAiAddValue();
                    *(u32*)(r29 + r30) = r3;
                    r3 = r27;
                    fightPokemonGetPokemonPtr();
                    r0 = 0x0;
                    r5 = (0x1 << 16);
                    *(u32*)(sp + 0x8) = r0;
                    r0 = 0x225;
                    r7 = r3;
                    r6 = r26;
                    *(u32*)(sp + 0xC) = r0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x0;
                    fn_8023A118();
                }
                r3 = r27;
                fightPokemonGetPokemonPtr();
                r0 = 0x0;
                r5 = (0x1 << 16);
                *(u32*)(sp + 0x8) = r0;
                r0 = 0x226;
                r7 = r3;
                r6 = r26;
                *(u32*)(sp + 0xC) = r0;
                r0 = *(u32*)(r29 + r30);
                r8 = 0x0;
                r9 = 0x0;
                r10 = 0x0;
                *(u32*)(sp + 0x10) = r0;
                fn_8023A118();
            }
        }
        r28 = r28 + 0x1;

    }
    r4 = r20;
    r3 = (u32)sp + 0x68;
    r5 = 0x1;
    fn_802397B8();
    if ((s32)r3 < (s32)0x0) {
        r3 = -0x1;
        return;
    }
    r14 = r3 << 2;
    r3 = (u32)sp + 0x98;
    r15 = *(u32*)(r3 + r14);
    if (r15 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r4 = r15;
    r3 = 0x0;
    fn_801F4460();
    r16 = r3;
    r3 = r15;
    fightPokemonGetPokemonPtr();
    r0 = 0x0;
    r4 = (u32)sp + 0x68;
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x228;
    r5 = (0x1 << 16);
    r7 = r3;
    *(u32*)(sp + 0xC) = r0;
    r6 = r16;
    r8 = 0x0;
    r0 = *(u32*)(r4 + r14);
    r9 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r15;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))pokemonGetStatus)();
    r3 = (s16)r3;

    return;
}

/* Note: fightTrainerAiWazaHit046 (Shadow Pokemon check) and fightTrainerAiWazaHit047 (battle
 * condition/flag check via fn_80236BFC) are implemented and matched at
 * 100% further below in this file (Ghidra-import block), not here. */

/* =========================================================================
 * fightTrainerAiWazaHit045 - ProcessBattleResult
 *
 * Medium helper (0xB0 bytes) that processes the result of a battle.
 * Calls BattleSequenceCheck and BattleResultCheck to determine
 * the outcome, then calls fn_80236C80 to update state.
 *
 * @param trainerCtx    Trainer context
 * @param trainerSlot   Slot index
 * @param resultSlot    Result query slot
 * @param resultType    Type of result to process
 * ========================================================================= */
/* fightTrainerAiWazaHit045 | size: 0xB0 */
u32 fightTrainerAiWazaHit045(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType) {
    extern u16 fn_8023793C();
    extern u32 fn_80239500();
    extern u32 fn_802395C8();
    extern u32 _fightTrainerAiWazaHitCheck();
    s32 resultVal;
    u32 statusVal;
    statusVal = fn_802395C8(trainerCtx, resultSlot, trainerSlot);
    resultVal = _fightTrainerAiWazaHitCheck(trainerCtx, trainerSlot, resultSlot, resultType, 0);
    if (fn_8023793C(trainerCtx, resultType, statusVal, fn_80239500(trainerCtx, resultSlot)) == 0x43) {
        resultVal = 0;
    }
    if (resultVal == 0) {
        return 0;
    }
    if (resultVal == -1) {
        return 1;
    }
    return 1;
}

/* =========================================================================
 * fightMenuFightTrainerGcHeroOpenMenu - FinalCleanup
 *
 * Large function near the end of the range (0xA5C = 2652 bytes).
 * One of the last significant functions before .ctors at 0x80266360.
 *
 * Likely performs comprehensive cleanup after a Colosseum session:
 *   - Reset temporary state
 *   - Save progress
 *   - Release loaded resources
 *   - Restore overworld state
 * ========================================================================= */
/* TODO: Decompile fightMenuFightTrainerGcHeroOpenMenu (2652 bytes) */
u32 fightMenuFightTrainerGcHeroOpenMenu(void* ctx, u32 param1, u32 param2) {
    extern void fn_800119A8();
    extern void fn_80011A1C();
    extern void fn_80011D9C();
    extern void menuCloseCustom();
    extern void menuIsCheck();
    extern void menuOpenCustom();
    extern void fn_801EF634();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F1700();
    extern void fn_801F18DC();
    extern void fn_801F1918();
    extern void fightSideGetStatus();
    extern void fn_801F7E60();
    extern void fn_801F9130();
    extern void fn_801F9790();
    extern void fn_801F981C();
    extern void fightOutPokemonCheckFightActionSelect();
    extern void fightOutPokemonInitFightActionBuff();
    extern void fightTypeDataBiosGetFightoutPokemonNum();
    extern void fightTypeDataBiosGetPtr();
    extern u32 _fightMenuFightTrainerGcHeroOpenMenuSubMain__FP13FIGHT_TRAINERP15FightOutPokemonUsl();
    extern void fightTimerCommandIsOver();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f3 = 0.0f;
    f32 f5 = 0.0f;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = 0;

    r31 = r4;
    r29 = r3;
    r25 = r5;
    r3 = r31;
    fightTypeDataBiosGetPtr();
    fightTypeDataBiosGetFightoutPokemonNum();
    r27 = r3 & 0xFF;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) {
    do {
        r4 = r29;
        r5 = r31;
        r24 = 0x100;
        r3 = 0x2;
        fn_801F02AC();
        if (r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fightSideGetStatus();
        r26 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r31;
        fn_801F0134();
        r0 = r3 & 0xFFFF;
        if (r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        fn_801F0234();
        fn_801F0204();
        if ((s32)r3 < (s32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = r26;
        r6 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x2;
        fightSideGetStatus();
    } while (0);
    do {
        if ((s32)r3 != (s32)0xf3) {
            if ((s32)r3 < (s32)0xf3) {
                if ((s32)r3 != (s32)0xf1) {
                    if ((s32)r3 < (s32)0xf1) {
                        break;
                    }
                    if ((s32)r3 >= (s32)0xf5) break;
                    continue;
                    }
                r24 = 0x100;
                break;
                    }
            r24 = 0x101;
            break;
        }
        r24 = 0x102;
        break;

        r24 = 0x103;
    } while (0);
        r3 = r24;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        menuOpenCustom();
    }
    r30 = 0x0;
    r26 = 0x0;
while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= r27) break;
    do {
        r3 = r29;
        r4 = r30;
        fn_801F981C();
        if ((s32)r3 == (s32)0xf5) {
            r26 = r30;
            break;
        }
        r4 = 0x1;
        fightOutPokemonCheckFightActionSelect();
        r0 = r3 & 0xFF;
        if ((s32)r3 == (s32)0xf5) {
            r26 = r30;
            break;
        }
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x1) {
    while (1) {
        r3 = r29;
        fn_801F9790();
        break;
        }
        r3 = 0x0;
        fn_801F1700();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            fightTimerCommandIsOver();
            r0 = r3 & 0xFF;
        }
        if (r0 == (u32)0x1) {
    while (1) {
        r3 = r28;
        fightOutPokemonInitFightActionBuff();
        r3 = r29;
        r4 = r28;
        r5 = r31;
        fn_801F9130();
        r26 = r30;
        break;
        }
        r3 = 0x0;
        fn_801F18DC();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
        do {
            r4 = r28;
            r5 = r31;
            r3 = 0x2;
            fn_801F02AC();
            if (r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = 0x0;
            r5 = 0x5;
            r6 = 0x0;
            fightSideGetStatus();
            r24 = r3 & 0xFFFF;
            r3 = r28;
            r4 = r31;
            fn_801F0134();
            r0 = r3 & 0xFFFF;
            if (r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            fn_801F0234();
            fn_801F0204();
            if ((s32)r3 < (s32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = r24;
            r6 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x3;
            fightSideGetStatus();
        } while (0);
            r4 = 0x1;
            fn_80011D9C();
        }
        r0 = 0x0;
        *(u32*)(sp + 0x8) = r0;
    while (1) {
            r3 = r28;
            fightOutPokemonInitFightActionBuff();
            r4 = r28;
            r5 = (u32)sp + 0xc;
            r3 = 0x0;
            fn_801F1918();
            r3 = 0x0;
            fn_801F18DC();
            *(u8*)(sp + 0x23) = r3;
            r3 = (u32)sp + 0xc;
            r4 = (u32)sp + 0x8;
            r5 = 0x1;
            fn_80011A1C();
            r24 = r3;
            fn_801EF634();
            r0 = r3 & 0xFFFF;
            if (r0 == (u32)0x1) {
                r3 = 0x0;
                fn_801F18DC();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                do {
                    r4 = r28;
                    r5 = r31;
                    r3 = 0x2;
                    fn_801F02AC();
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = 0x0;
                    r5 = 0x5;
                    r6 = 0x0;
                    fightSideGetStatus();
                    r24 = r3 & 0xFFFF;
                    r3 = r28;
                    r4 = r31;
                    fn_801F0134();
                    r0 = r3 & 0xFFFF;
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    fn_801F0234();
                    fn_801F0204();
                    if ((s32)r3 < (s32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = r24;
                    r6 = r3 & 0xFFFF;
                    r3 = 0x0;
                    r5 = 0x3;
                    fightSideGetStatus();
                } while (0);
                    r4 = 0x0;
                    fn_80011D9C();
                }
                r3 = 0x1;
                fn_800119A8();
                continue;
            }
            r3 = 0x0;
            fn_801F1700();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                fightTimerCommandIsOver();
                r0 = r3 & 0xFF;
            }
            if (r0 == (u32)0x1 || (s32)r24 >= (s32)0x0) {

                r3 = 0x0;
                fn_801F18DC();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                do {
                    r4 = r28;
                    r5 = r31;
                    r3 = 0x2;
                    fn_801F02AC();
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = 0x0;
                    r5 = 0x5;
                    r6 = 0x0;
                    fightSideGetStatus();
                    r24 = r3 & 0xFFFF;
                    r3 = r28;
                    r4 = r31;
                    fn_801F0134();
                    r0 = r3 & 0xFFFF;
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    fn_801F0234();
                    fn_801F0204();
                    if ((s32)r3 < (s32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = r24;
                    r6 = r3 & 0xFFFF;
                    r3 = 0x0;
                    r5 = 0x3;
                    fightSideGetStatus();
                } while (0);
                    r4 = 0x0;
                    fn_80011D9C();
                }
                r3 = 0x1;
                fn_800119A8();
                continue;
            }
            if ((s32)r24 >= (s32)0x0) {
                r3 = 0x1;
                fn_800119A8();
            }
            r3 = r29;
            r4 = r28;
            r5 = r31;
            r6 = r24;
            _fightMenuFightTrainerGcHeroOpenMenuSubMain__FP13FIGHT_TRAINERP15FightOutPokemonUsl();
            r24 = r3;
            fn_801EF634();
            r0 = r3 & 0xFFFF;
            if (r0 == (u32)0x1) {
                r3 = 0x0;
                fn_801F18DC();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                do {
                    r4 = r28;
                    r5 = r31;
                    r3 = 0x2;
                    fn_801F02AC();
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = 0x0;
                    r5 = 0x5;
                    r6 = 0x0;
                    fightSideGetStatus();
                    r24 = r3 & 0xFFFF;
                    r3 = r28;
                    r4 = r31;
                    fn_801F0134();
                    r0 = r3 & 0xFFFF;
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    fn_801F0234();
                    fn_801F0204();
                    if ((s32)r3 < (s32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = r24;
                    r6 = r3 & 0xFFFF;
                    r3 = 0x0;
                    r5 = 0x3;
                    fightSideGetStatus();
                } while (0);
                    r4 = 0x0;
                    fn_80011D9C();
                }
                r3 = 0x1;
                fn_800119A8();
    }
            }
            r3 = 0x0;
            fn_801F1700();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                fightTimerCommandIsOver();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r0 = r24 & 0xFF;
            }
            }
            if (r0 != (u32)0x1) {
                r3 = 0x0;
                fn_801F18DC();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                do {
                    r4 = r28;
                    r5 = r31;
                    r3 = 0x2;
                    fn_801F02AC();
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = 0x0;
                    r5 = 0x5;
                    r6 = 0x0;
                    fightSideGetStatus();
                    r24 = r3 & 0xFFFF;
                    r3 = r28;
                    r4 = r31;
                    fn_801F0134();
                    r0 = r3 & 0xFFFF;
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    fn_801F0234();
                    fn_801F0204();
                    if ((s32)r3 < (s32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = r24;
                    r6 = r3 & 0xFFFF;
                    r3 = 0x0;
                    r5 = 0x3;
                    fightSideGetStatus();
                } while (0);
                    r4 = 0x0;
                    fn_80011D9C();
                }
                r3 = 0x1;
                fn_800119A8();
    }
            }
            r0 = r24 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r29;
                fn_801F7E60();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) continue;
                r3 = r30 & 0xFFFF;
                if (r0 != (u32)0x1) {
                    r30 = r26;
                    r3 = 0x0;
                    fn_801F18DC();
                    r0 = r3 & 0xFF;
                    if (r0 != (u32)0x1) {
                    do {
                        r4 = r28;
                        r5 = r31;
                        r3 = 0x2;
                        fn_801F02AC();
                        if (r3 == (u32)0x0) {
                            r3 = 0x0;
                            break;
                        }
                        r4 = 0x0;
                        r5 = 0x5;
                        r6 = 0x0;
                        fightSideGetStatus();
                        r24 = r3 & 0xFFFF;
                        r3 = r28;
                        r4 = r31;
                        fn_801F0134();
                        r0 = r3 & 0xFFFF;
                        if (r3 == (u32)0x0) {
                            r3 = 0x0;
                            break;
                        }
                        fn_801F0234();
                        fn_801F0204();
                        if ((s32)r3 < (s32)0x0) {
                            r3 = 0x0;
                            break;
                        }
                        r4 = r24;
                        r6 = r3 & 0xFFFF;
                        r3 = 0x0;
                        r5 = 0x3;
                        fightSideGetStatus();
                    } while (0);
                        r4 = 0x0;
                        fn_80011D9C();
                    }
                    r3 = 0x1;
                    fn_800119A8();
                    continue;
                }
                r0 = r25 & 0xFF;
                if (r0 != (u32)0x1 || r3 != (u32)0x0) continue;

                r3 = 0x0;
                fn_801F18DC();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                do {
                    r4 = r28;
                    r5 = r31;
                    r3 = 0x2;
                    fn_801F02AC();
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = 0x0;
                    r5 = 0x5;
                    r6 = 0x0;
                    fightSideGetStatus();
                    r24 = r3 & 0xFFFF;
                    r3 = r28;
                    r4 = r31;
                    fn_801F0134();
                    r0 = r3 & 0xFFFF;
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    fn_801F0234();
                    fn_801F0204();
                    if ((s32)r3 < (s32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = r24;
                    r6 = r3 & 0xFFFF;
                    r3 = 0x0;
                    r5 = 0x3;
                    fightSideGetStatus();
                } while (0);
                    r4 = 0x0;
                    fn_80011D9C();
                }
                r3 = 0x1;
                fn_800119A8();
                r24 = 0x0;
                while (1) {
                    r0 = r24 & 0xFFFF;
                    if (r0 >= (u32)r27) break;
                    r3 = r29;
                    r4 = r24;
                    fn_801F981C();
                    if (r3 != (u32)0x0) {
                        r4 = 0x0;
                        r5 = 0x120;
                        r6 = 0x0;
                        r7 = 0x0;
                        ((void(*)(void))pokemonSetStatus)();
                    }
                    r24 = r24 + 0x1;

                }
                r3 = 0x0;
                return r3;
            }
            if (r0 == (u32)0x2) continue;
        break;
    }
        r3 = 0x0;
        fn_801F18DC();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
        do {
            r4 = r28;
            r5 = r31;
            r3 = 0x2;
            fn_801F02AC();
            if (r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = 0x0;
            r5 = 0x5;
            r6 = 0x0;
            fightSideGetStatus();
            r24 = r3 & 0xFFFF;
            r3 = r28;
            r4 = r31;
            fn_801F0134();
            r0 = r3 & 0xFFFF;
            if (r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            fn_801F0234();
            fn_801F0204();
            if ((s32)r3 < (s32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = r24;
            r6 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x3;
            fightSideGetStatus();
        } while (0);
            r4 = 0x0;
            fn_80011D9C();
        }
        r26 = r30;
    } while (0);
        r30 = r30 + 0x1;
        r0 = r30 & 0xFFFF;
        if (r0 < r27) continue;
    break;
}

    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if (r0 != (u32)r27) {
    do {
        r4 = r29;
        r5 = r31;
        r24 = 0x100;
        r3 = 0x2;
        fn_801F02AC();
        if (r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fightSideGetStatus();
        r25 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r31;
        fn_801F0134();
        r0 = r3 & 0xFFFF;
        if (r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        fn_801F0234();
        fn_801F0204();
        if ((s32)r3 < (s32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = r25;
        r6 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x2;
        fightSideGetStatus();
    } while (0);
    do {
        if ((s32)r3 != (s32)0xf3) {
            if ((s32)r3 < (s32)0xf3) {
                if ((s32)r3 != (s32)0xf1) {
                    if ((s32)r3 < (s32)0xf1) {
                        break;
                    }
                    if ((s32)r3 >= (s32)0xf5) break;
                    continue;
                    }
                r24 = 0x100;
                break;
                    }
            r24 = 0x101;
            break;
        }
        r24 = 0x102;
        break;

        r24 = 0x103;
    } while (0);
        r3 = r24;
        menuIsCheck();
        r0 = r3 & 0xFF;
        if ((s32)r3 != (s32)0xf5) {
        do {
            r4 = r29;
            r5 = r31;
            r24 = 0x100;
            r3 = 0x2;
            fn_801F02AC();
            if (r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = 0x0;
            r5 = 0x5;
            r6 = 0x0;
            fightSideGetStatus();
            r25 = r3 & 0xFFFF;
            r3 = r29;
            r4 = r31;
            fn_801F0134();
            r0 = r3 & 0xFFFF;
            if (r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            fn_801F0234();
            fn_801F0204();
            if ((s32)r3 < (s32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = r25;
            r6 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x2;
            fightSideGetStatus();
        } while (0);
        do {
            if ((s32)r3 != (s32)0xf3) {
                if ((s32)r3 < (s32)0xf3) {
                    if ((s32)r3 != (s32)0xf1) {
                        if ((s32)r3 < (s32)0xf1) {
                            break;
                        }
                        if ((s32)r3 >= (s32)0xf5) break;
                        continue;
                        }
                    r24 = 0x100;
                    break;
                        }
                r24 = 0x101;
                break;
            }
            r24 = 0x102;
            break;

            r24 = 0x103;
        } while (0);
            r3 = r24;
            r4 = 0x0;
            r5 = 0x1;
            menuCloseCustom();
    }
    }
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r27) break;
        r3 = r29;
        r4 = r24;
        fn_801F981C();
        if (r3 != (u32)0x0) {
            r4 = 0x0;
            r5 = 0x120;
            r6 = 0x0;
            r7 = 0x0;
            ((void(*)(void))pokemonSetStatus)();
        }
        r24 = r24 + 0x1;

    }
    r3 = 0x1;

    return r3;
}

/* Address: 0x80263BC8 | Size: 0x21C | Ghidra import */
u32 _fightMenuFightTrainerGcHeroOpenMenuSubMain__FP13FIGHT_TRAINERP15FightOutPokemonUsl(u32 r3, u32 r4, u32 r5, s32 mode)
{
    extern u8 fn_801FF1BC(u32, u32);
    extern u8 fn_801F54A4(u32, u32, u32, u32);
    extern void fn_80132A38(u32, u32);
    extern void winMsgOpenFight(u32, u32, u32);
    extern void fn_801F000C(u32);
    extern void winMsgCloseFight(u32);
    extern u8 _fightMenuFightTrainerGcHeroOpenMenuSubWaza__FP13FIGHT_TRAINERP15FightOutPokemonUs(u32, u32, u32);
    extern u8 _fightMenuFightTrainerGcHeroOpenMenuSubItem__FP13FIGHT_TRAINERP15FightOutPokemonUs(u32, u32, u32);
    extern s32 fightMenuFightTrainerGcHeroSelectIrekaeFightPokemon(u32, u32, u32, u32, u32);
    extern void fightOutPokemonCreateFightAction(u32, u32, u32, u32, void *, s32);
    extern char lbl_80375D30[];
    u8 flag;
    u32 result;

    result = 0;
    switch (mode) {
    case 0:
        flag = fn_801FF1BC(r4, 1);
        if (flag == 1) {
            fn_80132A38(0x11, r4);
            winMsgOpenFight(0x75fc, 1, 1);
            fn_801F000C(0x40);
            winMsgCloseFight(0);
        }
        if (flag == 0) {
            if (_fightMenuFightTrainerGcHeroOpenMenuSubWaza__FP13FIGHT_TRAINERP15FightOutPokemonUs(r3, r4, r5) == 0) {
                result = 2;
            }
        }
        break;
    case 1:
        if (fn_801F54A4(0, 0, 0x20, 0) == 0) {
            winMsgOpenFight(0x75f5, 1, 1);
            fn_801F000C(0x40);
            winMsgCloseFight(0);
            result = 2;
        }
        else if (_fightMenuFightTrainerGcHeroOpenMenuSubItem__FP13FIGHT_TRAINERP15FightOutPokemonUs(r3, r4, r5) == 0) {
            result = 2;
        }
        break;
    case 2:
        r5 = fightMenuFightTrainerGcHeroSelectIrekaeFightPokemon(r3, r4, r5, 1, 1);
        if ((s16)r5 < 0) {
            flag = 0;
        }
        else {
            fightOutPokemonCreateFightAction(r4, 0, 9, 0, lbl_80375D30, (s16)(r5 & 0xFFFFFFFFFFFFFFFFu));
            flag = 1;
        }
        if (flag == 0) {
            result = 2;
        }
        break;
    case 3:
        if (fn_801F54A4(0, 0, 0x22, 0) == 1) {
            fightOutPokemonCreateFightAction(r4, 0, 8, 0, lbl_80375D30, 0);
        }
        else if (fn_801F54A4(0, 0, 0x21, 0) == 1) {
            fightOutPokemonCreateFightAction(r4, 0, 0xa, 0, lbl_80375D30, 0);
        }
        else {
            result = 2;
        }
        break;
    default:
        result = 1;
        break;
    }
    return result;
}

/* Address: 0x80263DE4 | Size: 0x6A4 | Ghidra import */
s32 fightMenuFightTrainerGcHeroSelectIrekaeFightPokemon(u32 r3, u32 r4, u32 r5, u32 retry, u32 reopen)
{
    extern u16 fn_801EF634(void);
    extern u8 fn_801F1700(u32);
    extern u8 fn_801F1758(u32);
    extern u8 fightTimerCommandIsOver(void);
    extern u8 fn_801F18DC(u32);
    extern u32 fn_801F02AC(u32, u32, u32);
    extern u32 fightSideGetStatus(u32, u32, u32, u32);
    extern u16 fn_801F0134(u32, u32);
    extern void fn_801F0234(void);
    extern s32 fn_801F0204(void);
    extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
    extern u8 menuIsCheck(u32);
    extern void menuCloseCustom(u32, u32, u32);
    extern void fightMenuCloseInfoMenu(u32);
    extern void fightMenuOpenInfoMenu(s8);
    extern s32 fn_800114A4(u32, u32, u32, u32, u32);
    extern void fn_801EFFC4(u32);
    extern void fn_80011D9C(u32, u32);
    extern s32 fn_801F93F8(u32, u32, u32);
    extern u32 fn_801FB1C0(u32, u32, u32, u32);
    extern u8 fightPokemonCheckValid(u32);
    extern s16 pokemonGetStatus(u32, u32, u32, u32);
    u32 ctx;
    u32 actor;
    u32 param;
    u32 choice;
    u32 msg;
    u32 found;
    u32 status;
    u32 side;
    s32 index;
    u32 item;

    ctx = r3;
    actor = r4;
    param = r5;
    while (1) {
        if ((u16)fn_801EF634() == 1) {
            return -2;
        }
        if ((fn_801F1700(0) == 1) && (fightTimerCommandIsOver() == 1)) {
            if ((u8)reopen == 0) {
                return fn_801F93F8(ctx, actor, param);
            }
            if ((u8)retry != 0) {
                return -1;
            }
            continue;
        }
        if ((fn_801F18DC(0) != 0) && ((u8)reopen == 0)) {
            msg = 0x100;
            found = fn_801F02AC(2, ctx, param);
            if (found == 0) {
                status = 0;
            }
            else {
                side = (u16)fightSideGetStatus(found, 0, 5, 0);
                if (fn_801F0134(ctx, param) == 0) {
                    status = 0;
                }
                else {
                    fn_801F0234();
                    index = fn_801F0204();
                    if (index < 0) {
                        status = 0;
                    }
                    else {
                        status = fightSideGetStatus(0, side, 2, index & 0xffff);
                    }
                }
            }
            switch (status) {
            case 0xf1:
                msg = 0x100;
                break;
            case 0xf2:
                msg = 0x101;
                break;
            case 0xf3:
                msg = 0x102;
                break;
            case 0xf4:
                msg = 0x103;
                break;
            }
            menuOpenCustom(msg, 0, 0, 0, 0, 0);
        }
        if (fn_801F18DC(0) == 0) {
            fightMenuCloseInfoMenu(1);
        }
        choice = fn_800114A4(ctx, actor, param, retry, fn_801F18DC(0));
        fn_801EFFC4(0xa);
        if ((fn_801F18DC(0) == 0) && ((u8)reopen == 1)) {
            fightMenuOpenInfoMenu(1);
            if ((actor != 0) && (fn_801F18DC(0) != 1)) {
                found = fn_801F02AC(2, actor, param);
                if (found == 0) {
                    status = 0;
                }
                else {
                    side = (u16)fightSideGetStatus(found, 0, 5, 0);
                    if (fn_801F0134(actor, param) == 0) {
                        status = 0;
                    }
                    else {
                        fn_801F0234();
                        index = fn_801F0204();
                        if (index < 0) {
                            status = 0;
                        }
                        else {
                            status = fightSideGetStatus(0, side, 3, index & 0xffff);
                        }
                    }
                }
                fn_80011D9C(status, 1);
            }
        }
        if ((fn_801F18DC(0) != 0) && ((u8)reopen == 0)) {
            msg = 0x100;
            found = fn_801F02AC(2, ctx, param);
            if (found == 0) {
                status = 0;
            }
            else {
                side = (u16)fightSideGetStatus(found, 0, 5, 0);
                if (fn_801F0134(ctx, param) == 0) {
                    status = 0;
                }
                else {
                    fn_801F0234();
                    index = fn_801F0204();
                    if (index < 0) {
                        status = 0;
                    }
                    else {
                        status = fightSideGetStatus(0, side, 2, index & 0xffff);
                    }
                }
            }
            switch (status) {
            case 0xf1:
                msg = 0x100;
                break;
            case 0xf2:
                msg = 0x101;
                break;
            case 0xf3:
                msg = 0x102;
                break;
            case 0xf4:
                msg = 0x103;
                break;
            }
            if (menuIsCheck(msg) != 0) {
                msg = 0x100;
                found = fn_801F02AC(2, ctx, param);
                if (found == 0) {
                    status = 0;
                }
                else {
                    side = (u16)fightSideGetStatus(found, 0, 5, 0);
                    if (fn_801F0134(ctx, param) == 0) {
                        status = 0;
                    }
                    else {
                        fn_801F0234();
                        index = fn_801F0204();
                        if (index < 0) {
                            status = 0;
                        }
                        else {
                            status = fightSideGetStatus(0, side, 2, index & 0xffff);
                        }
                    }
                }
                switch (status) {
                case 0xf1:
                    msg = 0x100;
                    break;
                case 0xf2:
                    msg = 0x101;
                    break;
                case 0xf3:
                    msg = 0x102;
                    break;
                case 0xf4:
                    msg = 0x103;
                    break;
                }
                menuCloseCustom(msg, 0, 1);
            }
        }
        if ((u16)fn_801EF634() == 1) {
            return -2;
        }
        if ((fn_801F1700(0) == 1) && (fightTimerCommandIsOver() == 1) && (choice < 0)) {
            if ((u8)reopen == 0) {
                return fn_801F93F8(ctx, actor, param);
            }
            if ((u8)retry != 0) {
                return -1;
            }
            continue;
        }
        if (choice < 0) {
            if ((u8)retry == 0) {
                continue;
            }
            return -1;
        }
        item = fn_801FB1C0(ctx, 0, 0x45, choice & 0xffff);
        if (fightPokemonCheckValid(item) == 0) {
            continue;
        }
        if (((u8)pokemonGetStatus(item, 0, 0xd2, 0)) == 1) {
            continue;
        }
        return pokemonGetStatus(item, 0, 0xce, 0);
    }
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 120 functions matched
 * =================================================================== */

/* Address: 0x8024E52C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaValueNull(void) { return 0; }

/* Address: 0x80250980 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage213(void) { return 0; }

/* Address: 0x80250988 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage212(void) { return 0; }

/* Address: 0x80250990 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage211(void) { return 0; }

/* Address: 0x80250998 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage210(void) { return 0; }

/* Address: 0x80250A24 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage208(void) { return 0; }

/* Address: 0x80250AB0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage206(void) { return 0; }

/* Address: 0x80250AB8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage205(void) { return 0; }

/* Address: 0x80250CE8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage201(void) { return 0; }

/* Address: 0x80250D74 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage199(void) { return 0; }

/* Address: 0x80250F54 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage195(void) { return 0; }

/* Address: 0x80250F5C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage194(void) { return 0; }

/* Address: 0x80250F64 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage193(void) { return 0; }

/* Address: 0x80250F6C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage192(void) { return 0; }

/* Address: 0x80250F74 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage191(void) { return 0; }

/* Address: 0x80251150 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage187(void) { return 0; }

/* Address: 0x802511D0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage184(void) { return 0; }

/* Address: 0x802511D8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage183(void) { return 0; }

/* Address: 0x80251264 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage181(void) { return 0; }

/* Address: 0x8025126C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage180(void) { return 0; }

/* Address: 0x80251274 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage179(void) { return 0; }

/* Address: 0x8025127C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage178(void) { return 0; }

/* Address: 0x80251284 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage177(void) { return 0; }

/* Address: 0x8025128C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage176(void) { return 0; }

/* Address: 0x80251294 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage175(void) { return 0; }

/* Address: 0x8025129C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage174(void) { return 0; }

/* Address: 0x80251350 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage172(void) { return 0; }

/* Address: 0x802514C4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage168(void) { return 0; }

/* Address: 0x802514CC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage167(void) { return 0; }

/* Address: 0x802514D4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage166(void) { return 0; }

/* Address: 0x802514DC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage165(void) { return 0; }

/* Address: 0x802514E4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage164(void) { return 0; }

/* Address: 0x8025160C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage160(void) { return 0; }

/* Address: 0x80251650 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage158(void) { return 0; }

/* Address: 0x80251680 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage156(void) { return 0; }

/* Address: 0x80251798 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage153(void) { return 0; }

/* Address: 0x80251B38 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage144(void) { return 0; }

/* Address: 0x80251B40 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage143(void) { return 0; }

/* Address: 0x80251B48 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage142(void) { return 0; }

/* Address: 0x80251CDC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage137(void) { return 0; }

/* Address: 0x80251CE4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage136(void) { return 0; }

/* Address: 0x80251F64 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage130(void) { return 20; }

/* Address: 0x80252030 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage127(void) { return 0; }

/* Address: 0x80252140 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage124(void) { return 0; }

/* Address: 0x8025234C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage120(void) { return 0; }

/* Address: 0x80252390 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage118(void) { return 0; }

/* Address: 0x80252468 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage116(void) { return 0; }

/* Address: 0x80252470 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage115(void) { return 0; }

/* Address: 0x80252478 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage114(void) { return 0; }

/* Address: 0x80252480 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage113(void) { return 0; }

/* Address: 0x80252488 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage112(void) { return 0; }

/* Address: 0x80252490 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage111(void) { return 0; }

/* Address: 0x80252498 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage109(void) { return 0; }

/* Address: 0x802524A0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage108(void) { return 0; }

/* Address: 0x802524A8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage107(void) { return 0; }

/* Address: 0x802524B0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage106(void) { return 0; }

/* Address: 0x80252740 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage102(void) { return 0; }

/* Address: 0x802527BC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage100(void) { return 5; }

/* Address: 0x802528BC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage097(void) { return 0; }

/* Address: 0x802528C4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage095(void) { return 0; }

/* Address: 0x802528CC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage094(void) { return 0; }

/* Address: 0x802528D4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage093(void) { return 0; }

/* Address: 0x8025296C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage090(void) { return 0; }

/* Address: 0x80252974 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage089(void) { return 0; }

/* Address: 0x802529C4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage086(void) { return 0; }

/* Address: 0x802529CC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage085(void) { return 0; }

/* Address: 0x802529D4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage084(void) { return 0; }

/* Address: 0x802529DC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage083(void) { return 0; }

/* Address: 0x802529E4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage082(void) { return 0; }

/* Address: 0x802529EC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage081(void) { return 0; }

/* Address: 0x80252A78 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage079(void) { return 0; }

/* Address: 0x80252F1C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage067(void) { return 0; }

/* Address: 0x80252F24 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage066(void) { return 0; }

/* Address: 0x80252F2C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage065(void) { return 0; }

/* Address: 0x80252F34 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage062(void) { return 0; }

/* Address: 0x80252F3C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage060(void) { return 0; }

/* Address: 0x80252F44 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage059(void) { return 0; }

/* Address: 0x80252F4C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage058(void) { return 0; }

/* Address: 0x80252F54 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage057(void) { return 0; }

/* Address: 0x80252F5C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage054(void) { return 0; }

/* Address: 0x80252F64 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage053(void) { return 0; }

/* Address: 0x80252F6C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage052(void) { return 0; }

/* Address: 0x80252F74 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage051(void) { return 0; }

/* Address: 0x80252F7C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage050(void) { return 0; }

/* Address: 0x80252F84 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage049(void) { return 0; }

/* Address: 0x80253010 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage047(void) { return 0; }

/* Address: 0x80253018 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage046(void) { return 0; }

/* Address: 0x802531F0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage041(void) { return 40; }

/* Address: 0x80253344 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage035(void) { return 0; }

/* Address: 0x802533D0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage033(void) { return 0; }

/* Address: 0x80253484 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage030(void) { return 0; }

/* Address: 0x802534CC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage028(void) { return 0; }

/* Address: 0x80253510 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage026(void) { return 0; }

/* Address: 0x80253518 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage025(void) { return 0; }

/* Address: 0x80253520 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage024(void) { return 0; }

/* Address: 0x80253528 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage023(void) { return 0; }

/* Address: 0x80253530 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage020(void) { return 0; }

/* Address: 0x80253538 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage019(void) { return 0; }

/* Address: 0x80253540 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage018(void) { return 0; }

/* Address: 0x802535CC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage016(void) { return 0; }

/* Address: 0x802535D4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage013(void) { return 0; }

/* Address: 0x802535DC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage011(void) { return 0; }

/* Address: 0x802535E4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage010(void) { return 0; }

/* Address: 0x802535EC | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage009(void) { return 0; }

/* Address: 0x802538B8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamage001(void) { return 0; }

/* Address: 0x80253948 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaDamageNull(void) { return 0; }

/* Address: 0x80254678 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit195(void) { return 1; }

/* Address: 0x80254E2C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit183(void) { return 1; }

/* Address: 0x80255218 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit174(void) { return 1; }

/* Address: 0x802552C8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit172(void) { return 1; }

/* Address: 0x80255EE4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit153(void) { return 0; }

/* Address: 0x802564C0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit143(void) { return 1; }

/* Address: 0x8025746C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit116(void) { return 1; }

/* Address: 0x802575C0 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit111(void) { return 1; }

/* Address: 0x80258134 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit093(void) { return 1; }

/* Address: 0x802586F4 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit085(void) { return 1; }

/* Address: 0x802587B8 | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit083(void) { return 1; }

/* Address: 0x8025B11C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHit025(void) { return 1; }

/* Address: 0x8025C25C | Size: 0x8 | Pattern: return_constant */
u32 fightTrainerAiWazaHitNull(void) { return 1; }

/* Address: 0x80262D34 | Size: 0x8 | Pattern: return_constant */
u32 _fightMenuFightTrainerAgbHeroSelectDefensePokemon__FP15FightOutPokemonUsUs(void) { return 0; }

/* ===================================================================
 * EXPANDED FUNCTION COVERAGE
 * 560 additional functions for 0x80240000-0x80266360
 * =================================================================== */

/* -------------------------------------------------------------------
 * Battle Orchestration (0x80240000-0x8024D000)
 * 92 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802400D8 | Size: 0x6C | Pattern: field_accessor */
u32 fightTrainerAiWazaValueOomugaesi(void* ctx, u32 slot, u16 species, u32 extra) {
    extern u32 fn_8023CA9C();
    extern u16 fightTrainerAiCheckOumu();
    u16 currentSpecies;
    currentSpecies = fightTrainerAiCheckOumu(ctx);
    if (currentSpecies == species || currentSpecies == 0) {
        return 0;
    }
    return fn_8023CA9C(ctx, slot, currentSpecies, extra);
}

/* Address: 0x80240144 | Size: 0xAC */
void fightTrainerAiWazaValueRandamuSentaku(void* ctx, u32 param1, u32 param2) {
    extern void fn_800E0C54();
    extern void fn_801FB1C0();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fightTrainerAiAddValue();
    extern void fn_80239CCC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r6 = 0x0;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r3 = 0x0;
    r4 = 0x1cb;
    r5 = 0x3e;
    fn_801FB1C0();
    r31 = r3;
    fn_800E0C54();
    r5 = r3 & 0xFFFF;
    r4 = r31 + 0x1;
    r0 = (s32)r5 / (s32)r4;
    r3 = 0x0;
    r0 = r0 * r4;
    r30 = r5 - r0;
    r4 = r30;
    fightTrainerAiAddValue();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1cb;
    fn_80239CCC();
    r3 = r31;
    return;
}

/* Address: 0x802401F0 | Size: 0x264 (612 bytes) */
void fightTrainerAiWazaValueHurahuradansu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_801F54A4();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80237F74();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r6 = 0x1;
    r7 = 0x1;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r5 = (u32)sp + 0x8;
    r4 = r27;
    r30 = 0x0;
    r3 = 0x0;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r31 = r3;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    if (r0 >= (u32)0x2) {
        r4 = r27;
        r3 = 0x0;
        r5 = 0x1c7;
        fn_80239984();
        r0 = r3;
        r3 = r28;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c7;
        fn_80239EE8();
    }
    r26 = (u32)sp + 0x8;
    r25 = r31 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r25) break;
        r4 = *(u32*)(r26 + r0);
        if (r28 != (u32)r4) {
            r3 = r27;
            fn_8023831C();
            r0 = r3 & 0xFFFF;

            if (r0 == (u32)0x8 || r0 == (u32)0x9) {

                r3 = r30;
                r4 = r27;
                r5 = 0x1c8;
                fn_80239984();
                r0 = r3;
                r3 = r28;
                r30 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r27;
                r8 = r29;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x1c8;
                fn_80239EE8();
                break;
        }
        }
        r24 = r24 + 0x1;

    }

    r25 = (u32)sp + 0x8;
    r26 = r31 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r26) break;
        r4 = *(u32*)(r25 + r0);
        if (r28 != (u32)r4) {
            r3 = r27;
            r5 = 0x14;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r30;
                r4 = r27;
                r5 = 0x1c9;
                fn_80239984();
                r0 = r3;
                r3 = r28;
                r30 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r27;
                r8 = r29;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x1c9;
                fn_80239EE8();
                break;
        }
        }
        r24 = r24 + 0x1;

    }

    r4 = (u32)sp + 0x8;
    r0 = r31 & 0xFFFF;
    r5 = 0x0;
    while (1) {
        r3 = r5 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        r3 = *(u32*)(r4 + r3);
        if (r28 != (u32)r3) {
            r3 = r30;
            r4 = r27;
            r5 = 0x1ca;
            fn_80239984();
            r0 = r3;
            r3 = r28;
            r30 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x1ca;
            fn_80239EE8();
            r3 = r30;
            return;
        }
        r5 = r5 + 0x1;

    }

    r3 = r30;
    return;
}

/* Address: 0x80240454 | Size: 0x16C (364 bytes) */
void fightTrainerAiWazaValueMakibisi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideGetCountAsJoutaiDataId();
    extern void fightSideIsJoutaiDataId();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r3;
    r27 = r4;
    r31 = r5;
    r4 = r6;
    r29 = 0x0;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4a;
    r28 = r3;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r28;
        r4 = 0x4a;
        fightSideGetCountAsJoutaiDataId();
    } else {

        r3 = 0x0;
    }
    r0 = (s16)r3;
    if (r0 == (u32)0x1) {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x1c4;
        fn_80239984();
        r0 = r3;
        r3 = r27;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r30;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c4;
        fn_80239EE8();
        r3 = r29;
        return;
    }
    r0 = (s16)r3;
    if ((s32)r0 == (s32)0x1) {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x1c5;
        fn_80239984();
        r0 = r3;
        r3 = r27;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r30;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c5;
        fn_80239EE8();
        r3 = r29;
        return;
    }
    if ((s32)r0 != (s32)0x2) { r3 = r29; return; }
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1c6;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c6;
    fn_80239EE8();

    r3 = r29;
    return;
}

/* Address: 0x802405C0 | Size: 0x8C */
u32 fightTrainerAiWazaValueRisaikuru(void* ctx, u32 param1, u32 param2) {
#pragma optimize_for_size on
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u16 fn_80236B98(void* ctx);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle = 0;

    if (fn_80236B98(ctx) != 0) {
        u32 tmp = fn_80239984(0, ctx, 0x1c3);
        handle = tmp;
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1c3);
    }
    return handle;
}

/* Address: 0x8024064C | Size: 0x13C (316 bytes) */
u32 fightTrainerAiWazaValueSiroikiri(void* ctx, u32 param1, u32 param2, u32 param3) {
#pragma optimize_for_size on
    typedef void (*BattleScriptCallback)();
    extern BattleScriptCallback wazaGetStatus(u32, u16, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u16 fn_80236520(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    extern void fightTrainerAiWazaValueKusuguruDaun();
    extern void fightTrainerAiWazaValueKaihiDaun();
    extern void fightTrainerAiWazaValueMeityuuDaun();
    extern void fightTrainerAiWazaValueTokubouDaun();
    extern void fightTrainerAiWazaValueBougyoDaun();
    extern void fightTrainerAiWazaValueKougekiDaun();
    extern void fightTrainerAiWazaValueSubayasaDaun();
    extern void fightTrainerAiWazaValueNull();
    BattleScriptCallback callback;
    u32 setup;
    u16 species;

    setup = 0;
    species = fn_80236520(ctx, param3);
    if ((species != 0) && (species != 0xffff) && (species != 0x165) && (species != 0x163)) {
        callback = wazaGetStatus(0, species, 0x1c, 0);
        if (callback == NULL) {
            callback = fightTrainerAiWazaValueNull;
        }
        if ((callback == fightTrainerAiWazaValueSubayasaDaun) || (callback == fightTrainerAiWazaValueKougekiDaun) || (callback == fightTrainerAiWazaValueBougyoDaun)
            || (callback == fightTrainerAiWazaValueTokubouDaun) || (callback == fightTrainerAiWazaValueMeityuuDaun) || (callback == fightTrainerAiWazaValueKaihiDaun)
            || (callback == fightTrainerAiWazaValueKusuguruDaun)) {
            setup = fn_80239984(0, ctx, 0x1c2);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1c2);
        }
    }
    return setup;
}

/* Address: 0x80240788 | Size: 0x448 (1096 bytes) */
void fightTrainerAiWazaValueJikoanji(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_802357CC();
    extern void fn_802358AC();
    extern void fn_80235910();
    extern void fn_80235974();
    extern void fn_802359D8();
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r6;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r4 = r30;
    r31 = 0x0;
    fn_80235AA0();
    *(u8*)(sp + 0x20) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    *(u8*)(sp + 0x21) = r3;
    r3 = r27;
    r4 = r30;
    fn_802359D8();
    *(u8*)(sp + 0x22) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235974();
    *(u8*)(sp + 0x23) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235910();
    *(u8*)(sp + 0x24) = r3;
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    *(u8*)(sp + 0x25) = r3;
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    *(u8*)(sp + 0x26) = r3;
    r3 = (u32)sp + 0x20;
    r4 = 0x0;
    while (1) {
        r0 = r4 & 0xFF;
        if (r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if (r0 >= (u32)0x8 || r0 > (u32)0x9) {

            r0 = 0x1;
            break;
        }
        r4 = r4 + 0x1;

    }
    r0 = 0x0;

    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r4 = r27;
        r3 = 0x0;
        r5 = 0x1be;
        fn_80239984();
        r0 = r3;
        r3 = r28;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1be;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235AA0();
    *(u8*)(sp + 0x18) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    *(u8*)(sp + 0x19) = r3;
    r3 = r27;
    r4 = r30;
    fn_802359D8();
    *(u8*)(sp + 0x1A) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235974();
    *(u8*)(sp + 0x1B) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235910();
    *(u8*)(sp + 0x1C) = r3;
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    *(u8*)(sp + 0x1D) = r3;
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    *(u8*)(sp + 0x1E) = r3;
    r3 = (u32)sp + 0x18;
    r4 = 0x0;
    while (1) {
        r0 = r4 & 0xFF;
        if (r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if (r0 >= (u32)0xa || r0 > (u32)0xc) {

            r0 = 0x1;
            break;
        }
        r4 = r4 + 0x1;

    }
    r0 = 0x0;

    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x1bf;
        fn_80239984();
        r0 = r3;
        r3 = r28;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1bf;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235AA0();
    *(u8*)(sp + 0x10) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    *(u8*)(sp + 0x11) = r3;
    r3 = r27;
    r4 = r30;
    fn_802359D8();
    *(u8*)(sp + 0x12) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235974();
    *(u8*)(sp + 0x13) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235910();
    *(u8*)(sp + 0x14) = r3;
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    *(u8*)(sp + 0x15) = r3;
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    *(u8*)(sp + 0x16) = r3;
    r3 = (u32)sp + 0x10;
    r4 = 0x0;
    while (1) {
        r0 = r4 & 0xFF;
        if (r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if (r0 >= (u32)0x3 || r0 > (u32)0x4) {

            r0 = 0x1;
            break;
        }
        r4 = r4 + 0x1;

    }
    r0 = 0x0;

    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x1c0;
        fn_80239984();
        r0 = r3;
        r3 = r28;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c0;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235AA0();
    *(u8*)(sp + 0x8) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    *(u8*)(sp + 0x9) = r3;
    r3 = r27;
    r4 = r30;
    fn_802359D8();
    *(u8*)(sp + 0xA) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235974();
    *(u8*)(sp + 0xB) = r3;
    r3 = r27;
    r4 = r30;
    fn_80235910();
    *(u8*)(sp + 0xC) = r3;
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    *(u8*)(sp + 0xD) = r3;
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    *(u8*)(sp + 0xE) = r3;
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    while (1) {
        r0 = r4 & 0xFF;
        if (r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if (r0 >= (u32)0x0 || r0 > (u32)0x2) {

            r0 = 0x1;
            break;
        }
        r4 = r4 + 0x1;

    }
    r0 = 0x0;

    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x1c1;
        fn_80239984();
        r0 = r3;
        r3 = r28;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c1;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80241660 | Size: 0x510 (1296 bytes) */
void fightTrainerAiWazaValueTedasuke(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 lbl_80478DF8;
    extern void fn_801F0898();
    extern void fn_801F1170();
    extern void fn_801F1990();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_80202108();
    extern void fn_80202234();
    extern void fightOutPokemonGetUseWazaDataId();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f9 = 0.0f;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r6 = 0x1;
    r7 = 0x1;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r5 = (u32)sp + 0x1c;
    r4 = r28;
    r31 = 0x0;
    r3 = 0x0;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r26 = r3;
    r21 = 0x0;
    r22 = 0x0;
    while (1) {
        r3 = lbl_80478DF8;
        r4 = r22 & 0xFFFF;
        r0 = *(u32*)((u8*)r3 + 0x0);
        if (r4 >= (u32)r0) break;
        r0 = r22 & 0xFFFF;
        if ((s32)r0 != (s32)0) {
            if (r0 != (u32)0x165) {
                if (r0 != (u32)0x163) {
                    r3 = r28;
                    r4 = r22;
                    r5 = 0x1;
                    fn_8023943C();
                    r0 = r3 & 0xFF;
                    if (r0 != (u32)0x163) {
                        r4 = r28;
                        r7 = r22;
                        r3 = 0x0;
                        r5 = 0x1;
                        r6 = 0x1;
                        r8 = 0x0;
                        fn_801F1990();
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r21 = 0x1;
        }
        }
        }
        }
        }
        r22 = r22 + 0x1;

    }
    r0 = r21 & 0xFF;
    if (r0 == (u32)0x1) {
        r4 = r28;
        r3 = 0x0;
        r5 = 0x1b0;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b0;
        fn_80239EE8();
    }
    r27 = (u32)sp + 0x1c;
    r22 = r26 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r22) break;
        r4 = *(u32*)(r27 + r0);
        if (r29 != (u32)r4) {
            r3 = r28;
            r5 = (u32)sp + 0x8;
            r6 = 0x0;
            r7 = 0x1;
            fn_802367CC();
            r25 = r3 & 0xFFFF;
            if (r29 != (u32)r4) {
                r23 = (u32)sp + 0x8;
                r21 = 0x0;
                while (1) {
                    r0 = r21 & 0xFFFF;
                    if (r0 >= (u32)r25) break;
                    r3 = r28;
                    r4 = *(u16*)(r23 + r0);
                    r5 = 0x1;
                    fn_8023943C();
                    r0 = r3 & 0xFF;
                    if (r29 != (u32)r4) {
                        r3 = r31;
                        r4 = r28;
                        r5 = 0x1b1;
                        fn_80239984();
                        r0 = r3;
                        r3 = r29;
                        r31 = r0;
                        fightOutPokemonGetPokemonPtr();
                        r6 = (0x1 << 16);
                        r5 = r3;
                        r4 = r28;
                        r8 = r30;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0x1b1;
                        fn_80239EE8();
                        break;
                    }
                    r21 = r21 + 0x1;

                }
        }
        }
        r24 = r24 + 0x1;

    }
    r27 = (u32)sp + 0x1c;
    r23 = r26 & 0xFFFF;
    r25 = 0x1;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r23) break;
        r3 = *(u32*)(r27 + r22);
        if (r29 == (u32)r3 || r29 == (u32)r3 || r29 == (u32)r3 || r0 == (u32)0x13) {
            r4 = 0x0;
            r5 = 0xfe;
            r6 = 0x0;
            ((void(*)(void))pokemonGetStatus)();

            fn_801F1170();
            r0 = r3 & 0xFF;

            r3 = r21;
            fn_801F0898();
            r0 = r3 & 0xFFFF;
            if (r0 != (u32)0x13) {
                r25 = 0x0;
                break;
            }
            r3 = *(u32*)(r27 + r22);
            fightOutPokemonGetUseWazaDataId();
            r0 = r3;
            r3 = r28;
            r4 = r0;
            r5 = 0x1;
            fn_8023943C();
            r0 = r3 & 0xFF;

            r25 = 0x0;
            break;
        }
        r24 = r24 + 0x1;

    }

    r0 = r25 & 0xFF;
    if (r0 == (u32)r23) {
        r3 = r31;
        r4 = r28;
        r5 = 0x1b2;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b2;
        fn_80239EE8();
    }
    r25 = (u32)sp + 0x1c;
    r24 = r26 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r24) break;
        r4 = *(u32*)(r25 + r0);
        if (r29 != (u32)r4) {
            r3 = r28;
            r5 = 0x12;
            fn_80236BFC();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r31;
                r4 = r28;
                r5 = 0x1b3;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r28;
                r8 = r30;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x1b3;
                fn_80239EE8();
                break;
        }
        }
        r22 = r22 + 0x1;

    }

    r25 = (u32)sp + 0x1c;
    r24 = r26 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r24) break;
        r3 = *(u32*)(r25 + r0);
        if (r29 != (u32)r3) {
            r4 = 0x0;
            r5 = 0xf9;
            r6 = 0x0;
            ((void(*)(void))pokemonGetStatus)();
            r0 = r3 & 0xFF;
            if (r29 != (u32)r3) {
                r3 = r31;
                r4 = r28;
                r5 = 0x1b4;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r28;
                r8 = r30;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x1b4;
                fn_80239EE8();
                break;
        }
        }
        r22 = r22 + 0x1;

    }

    r27 = (u32)sp + 0x1c;
    r24 = r26 & 0xFFFF;
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if (r0 >= (u32)r24) break;
        r23 = *(u32*)(r27 + r0);
        if (r29 != (u32)r23) {
            r3 = r28;
            r4 = r23;
            r5 = 0x8;
            fn_80236BFC();
            r0 = r3 & 0xFF;
            if (r29 == (u32)r23) {
                r0 = -0x1;

            } else {
                r3 = r28;
                r4 = r23;
                r5 = 0x30;
                fn_80237F74();
                r0 = r3 & 0xFF;
                r3 = r23;
                r0 = 0x1 - r0;
                r4 = 0x8;
                r0 = __cntlzw(r0);
                r5 = (u32)r0 >> 5;
                r26 = r5 + 0x1;
                fn_80202108();
                r22 = r3 + r26;
                r3 = r23;
                r4 = 0x8;
                fn_80202234();
                r3 = (s8)r3;
                r0 = (s8)r22;
                if ((s32)r0 >= (s32)r3) {
                    r0 = 0x1;
                    goto L_80241AF4;
                }
                r0 = 0x0;
            }
        L_80241AF4:
            r0 = (s8)r0;
            if ((s32)r0 == (s32)r3) {
                r3 = r31;
                r4 = r28;
                r5 = 0x1b5;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r28;
                r8 = r30;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x1b5;
                fn_80239EE8();
                r3 = r31;
                return;
        }
        }
        r25 = r25 + 0x1;

    }

    r3 = r31;
    return;
}

/* Address: 0x80241B70 | Size: 0x278 (632 bytes) */
u32 fightTrainerAiWazaValueKyouseikoutai(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F025C(u32, u32);
    extern u32 fightSideGetCountAsJoutaiDataId(u32, u32);
    extern u8 fightSideIsJoutaiDataId(u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_802357CC(void*, u32);
    extern u8 fn_802358AC(void*, u32);
    extern u8 fn_80235910(void*, u32);
    extern u8 fn_80235974(void*, u32);
    extern u8 fn_802359D8(void*, u32);
    extern u8 fn_80235A3C(void*, u32);
    extern u8 fn_80235AA0(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u8 stats[7];
    u32 handle;
    u32 mode;
    u8 found;
    u8 i;

    handle = 0;
    mode = fn_801F025C(2, param3);
    if (fightSideIsJoutaiDataId(mode, 0x4a) == 1) {
        mode = fightSideGetCountAsJoutaiDataId(mode, 0x4a);
    } else {
        mode = 0;
    }
    stats[0] = fn_80235AA0(ctx, param3);
    stats[1] = fn_80235A3C(ctx, param3);
    stats[2] = fn_802359D8(ctx, param3);
    stats[3] = fn_80235974(ctx, param3);
    stats[4] = fn_80235910(ctx, param3);
    stats[5] = fn_802358AC(ctx, param3);
    stats[6] = fn_802357CC(ctx, param3);

    i = 0;
    goto check_stats;
check_stat_value:
    if (stats[i] >= 8 && stats[i] <= 0xc) {
        found = 1;
        goto done_stats;
    }
    i++;
check_stats:
    if (i < 7) {
        goto check_stat_value;
    }
    found = 0;
done_stats:
    if (found == 1) {
        handle = fn_80239984(0, ctx, 0x1ac);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1ac);
    }
    if ((s16)mode == 1) {
        handle = fn_80239984(handle, ctx, 0x1ad);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1ad);
    } else if ((s16)mode == 2) {
        handle = fn_80239984(handle, ctx, 0x1ae);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1ae);
    } else if ((s16)mode == 3) {
        handle = fn_80239984(handle, ctx, 0x1af);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1af);
    }
    return handle;
}

/* Address: 0x80241DE8 | Size: 0x1FC (508 bytes) */
s32 fightTrainerAiWazaValueYokodori(void* ctx, void* param1, u32 param2, u32 param3) {
    extern u16 fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void* fightOutPokemonGetPokemonPtr();
    extern u16 fn_802364BC();
    extern u32 fn_80236520();
    extern u16 fn_802377E8();
    extern u8 fn_8023943C();
    extern void* fn_80239984();
    extern void fn_80239EE8();
    u32 buf[8];
    s32 handle = 0;
    u32 r1v;
    u16 r2v;
    u16 count;
    u16 i;
    u32 r3v;

    r1v = fn_80236520(ctx, param1);
    r2v = fn_802364BC(ctx, param1);
    r3v = fn_80236520(ctx, param3);
    count = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, buf, 0, 1);

    if ((u16)r3v != 0 && (u16)r3v != 0xffff && (u16)r3v != 0x165 && (u16)r3v != 0x163 &&
        fn_8023943C(ctx, r3v, 4) == 1) {
        handle = (s32)fn_80239984(0, ctx, 0x1a9);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1a9);
    }

    for (i = 0; i < count; i++) {
        u16 v = fn_802377E8(ctx, buf[i]);
        if (v == 0x12e || v == 0xd4 || v == 0x177) {
            handle = (s32)fn_80239984(handle, ctx, 0x1aa);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1aa);
            break;
        }
    }

    if ((u16)r1v == 0x121 || r2v == 0x121) {
        handle = (s32)fn_80239984(handle, ctx, 0x1ab);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1ab);
    }
    return handle;
}

/* Address: 0x80241FE4 | Size: 0x2A8 (680 bytes) */
s32 fightTrainerAiWazaValueKonoyubitomare(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void* fightOutPokemonGetPokemonPtr();
    extern u16 fn_802377E8();
    extern u8 fn_8023785C();
    extern u16 fn_8023793C();
    extern u16 fn_80237CB8();
    extern s32 fn_80239500();
    extern void* fn_80239984();
    extern void fn_80239EE8();
    u32 listA[8];
    u32 listB[8];
    u16 listC[2];
    s32 handle = 0;
    u16 countA;
    u16 countB;
    u16 countC;
    u8 found;
    u16 i;
    u16 j;
    u16 k;

    countA = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, listA, 1, 1);
    countB = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, listB, 0, 1);

    found = 0;
    for (i = 0; i < countB; i++) {
        countC = fn_80237CB8(ctx, listB[i], listC);
        for (j = 0; j < countC; j++) {
            for (k = 0; k < countA; k++) {
                if (param1 != listA[k] &&
                    fn_8023793C(ctx, listA[k], listC[j], fn_80239500(ctx, param2)) == 0x41) {
                    found = 1;
                }
            }
        }
    }

    if (found == 1) {
        handle = (s32)fn_80239984(0, ctx, 0x1A6);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1A6);
    }

    for (i = 0; i < countA; i++) {
        if (param1 != listA[i] && fn_8023785C(ctx, listA[i]) == 2) {
            handle = (s32)fn_80239984(handle, ctx, 0x1A7);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1A7);
            break;
        }
    }

    for (i = 0; i < countB; i++) {
        u16 v = fn_802377E8(ctx, listB[i]);
        if (v == 0x12E || v == 0xD4 || v == 0x177) {
            handle = (s32)fn_80239984(handle, ctx, 0x1A8);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1A8);
            break;
        }
    }

    return handle;
}

/* Address: 0x8024228C | Size: 0x3BC (956 bytes) */
u32 fightTrainerAiWazaValueKoukanKinsi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if ((u8)fn_80236BFC(ctx, param3, 3) == 1) {
        handle = fn_80239984(0, ctx, 0x19d);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x19d);
    }
    if ((u8)fn_80236BFC(ctx, param3, 4) == 1) {
        handle = fn_80239984(handle, ctx, 0x19e);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x19e);
    }
    if ((u8)fn_80236BFC(ctx, param3, 6) == 1) {
        handle = fn_80239984(handle, ctx, 0x19f);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x19f);
    }
    if ((u8)fn_80236BFC(ctx, param3, 5) == 1) {
        handle = fn_80239984(handle, ctx, 0x1a0);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1a0);
    }
    if ((u8)fn_80236BFC(ctx, param3, 9) == 1) {
        handle = fn_80239984(handle, ctx, 0x1a1);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1a1);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0xa) == 1) {
        handle = fn_80239984(handle, ctx, 0x1a2);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1a2);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x18) == 1) {
        handle = fn_80239984(handle, ctx, 0x1a3);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1a3);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1e) == 1) {
        handle = fn_80239984(handle, ctx, 0x1a4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1a4);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1c) == 1) {
        handle = fn_80239984(handle, ctx, 0x1a5);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x1a5);
    }
    return handle;
}

/* Address: 0x80242648 | Size: 0xE8 (232 bytes) */
u32 fightTrainerAiWazaValueKanasibari(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236520(void*, u32);
    extern s32 fn_80236D60(void*, u32, u32);
    extern u8 fn_8023943C(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;
    u32 move;

    handle = 0;
    move = fn_80236520(ctx, param3);
    if (fn_80236D60(ctx, param1, param3) > 0) {
        if ((move & 0xFFFF) != 0 && (move & 0xFFFF) != 0xFFFF && (move & 0xFFFF) != 0x165 &&
            (move & 0xFFFF) != 0x163) {
            if (fn_8023943C(ctx, move, 1) == 1) {
                handle = fn_80239984(0, ctx, 0x19c);
                fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x19c);
            }
        }
    }
    return handle;
}

/* Address: 0x80242730 | Size: 0x170 (368 bytes) */
u32 fightTrainerAiWazaValueMitizure(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern f32 lbl_8047E630;
    extern f32 lbl_8047E638;
    extern f32 lbl_8047E63C;
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if (fn_802373B0(ctx, param1, -1, lbl_8047E638) == 1) {
        handle = fn_80239984(0, ctx, 0x19b);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x19b);
    } else if (fn_802373B0(ctx, param1, -1, lbl_8047E63C) == 1) {
        handle = fn_80239984(0, ctx, 0x19a);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x19a);
    } else if (fn_802373B0(ctx, param1, -1, lbl_8047E630) == 1) {
        handle = fn_80239984(0, ctx, 0x199);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x199);
    }
    return handle;
}

/* Address: 0x802428A0 | Size: 0x134 (308 bytes) */
u32 fightTrainerAiWazaValueUrami(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236520(void*, u32);
    extern s32 fn_80236D60(void*, u32, u32);
    extern u8 fn_802391E0(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;
    u32 move;
    u32 value;

    handle = 0;
    move = fn_80236520(ctx, param3);
    if ((move & 0xFFFF) != 0 && (move & 0xFFFF) != 0xFFFF && (move & 0xFFFF) != 0x165 &&
        (move & 0xFFFF) != 0x163 && fn_80236D60(ctx, param1, param3) > 0) {
        value = fn_802391E0(ctx, move);
        if ((value & 0xFF) <= 5) {
            handle = fn_80239984(0, ctx, 0x197);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x197);
        } else if ((value & 0xFF) <= 0xa) {
            handle = fn_80239984(0, ctx, 0x198);
            fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x198);
        }
    }
    return handle;
}

/* Address: 0x802429D4 | Size: 0x17C (380 bytes) */
u32 fightTrainerAiWazaValueOdareru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236FFC(void*, u32);
    extern u32 fn_802370AC(void*, u32);
    extern u32 fn_8023715C(void*, u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 statLimit;
    u32 statCurrent;
    u32 statNext;
    u32 handle;

    handle = 0;
    statLimit = fn_80236FFC(ctx, param3);
    statCurrent = fn_8023715C(ctx, param3);
    statNext = fn_802370AC(ctx, param3);
    if ((u16)statCurrent > (u16)statLimit) {
        handle = fn_80239984(0, ctx, 0x194);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x194);
    }
    if ((u16)statCurrent > (u16)statNext) {
        handle = fn_80239984(handle, ctx, 0x195);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x195);
    }
    if (fn_80237F74(ctx, param3, 0x14) == 1) {
        handle = fn_80239984(handle, ctx, 0x196);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x196);
    }
    return handle;
}

/* Address: 0x80242B50 | Size: 0x17C (380 bytes) */
u32 fightTrainerAiWazaValueIbaru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236FFC(void*, u32);
    extern u32 fn_802370AC(void*, u32);
    extern u32 fn_8023715C(void*, u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 statLimit;
    u32 statCurrent;
    u32 statNext;
    u32 handle;

    handle = 0;
    statLimit = fn_80236FFC(ctx, param3);
    statCurrent = fn_8023715C(ctx, param3);
    statNext = fn_802370AC(ctx, param3);
    if ((u16)statLimit > (u16)statCurrent) {
        handle = fn_80239984(0, ctx, 0x191);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x191);
    }
    if ((u16)statCurrent > (u16)statNext) {
        handle = fn_80239984(handle, ctx, 0x192);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x192);
    }
    if (fn_80237F74(ctx, param3, 0x14) == 1) {
        handle = fn_80239984(handle, ctx, 0x193);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x193);
    }
    return handle;
}

/* Address: 0x80242CCC | Size: 0xE4 (228 bytes) */
u32 fightTrainerAiWazaValueAnkooru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_802364BC(void*, u32);
    extern s32 fn_80236D60(void*, u32, u32);
    extern u8 fn_8023943C(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;
    u32 move;

    handle = 0;
    move = fn_802364BC(ctx, param3);
    if (fn_80236D60(ctx, param1, param3) > 0) {
        if ((move & 0xFFFF) != 0 && (move & 0xFFFF) != 0xFFFF && (move & 0xFFFF) != 0x165 &&
            (move & 0xFFFF) != 0x163) {
            if (fn_8023943C(ctx, move, 1) == 0) {
                handle = fn_80239984(0, ctx, 0x190);
                fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x190);
            }
        }
    }
    return handle;
}

/* Address: 0x80242DB0 | Size: 0x9C */
u32 fightTrainerAiWazaValueIkarinomaeba(void* ctx, u32 param1, u32 param2, u32 extra) {
    extern f32 lbl_8047E630;
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if (fn_802373B0(ctx, extra, 1, lbl_8047E630) == 1) {
        handle = fn_80239984(0, ctx, 0x18f);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x18f);
    }
    return handle;
}

/* Address: 0x80242E4C | Size: 0x1A0 (416 bytes) */
u32 fightTrainerAiWazaValueKusuguruDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80235A3C(void*, u32);
    extern u8 fn_80235AA0(void*, u32);
    extern u32 fn_80236F4C(void*, u32);
    extern u32 fn_80236FFC(void*, u32);
    extern u32 fn_802370AC(void*, u32);
    extern u32 fn_8023715C(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 currentAtk;
    u32 limitAtk;
    u32 currentDef;
    u32 limitDef;
    u32 handle;

    handle = 0;
    currentAtk = fn_8023715C(ctx, param3);
    limitAtk = fn_80236FFC(ctx, param3);
    currentDef = fn_802370AC(ctx, param3);
    limitDef = fn_80236F4C(ctx, param3);
    if ((u16)currentAtk > (u16)limitAtk) {
        handle = fn_80239984(0, ctx, 0x18c);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x18c);
    }
    if ((u16)currentDef > (u16)limitDef) {
        handle = fn_80239984(handle, ctx, 0x18d);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x18d);
    }
    if (fn_80235AA0(ctx, param3) <= 4 && fn_80235A3C(ctx, param3) <= 4) {
        handle = fn_80239984(handle, ctx, 0x18e);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x18e);
    }
    return handle;
}

/* Address: 0x80242FEC | Size: 0xF8 (248 bytes) */
u32 fightTrainerAiWazaValueKaihiDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_802357CC();
    extern s32 fn_80239984();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    s32 handle = 0;

    if ((u8)fn_802357CC(ctx, param3) >= 7) {
        handle = fn_80239984(0, ctx, 0x18A);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x18A);
    }
    if ((u8)fn_802357CC(ctx, param3) <= 4) {
        handle = fn_80239984(handle, ctx, 0x18B);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x18B);
    }
    return handle;
}

/* Address: 0x802430E4 | Size: 0x94 */
u32 fightTrainerAiWazaValueMeityuuDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_802358AC(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 setup;

    setup = 0;
    if (fn_802358AC(ctx, param3) <= 4U) {
        setup = fn_80239984(0, ctx, 0x189);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x189);
    }
    return setup;
}

/* Address: 0x80243178 | Size: 0x10C (268 bytes) */
u32 fightTrainerAiWazaValueTokubouDaun(void* arg0, void* arg1, u32 arg2, void* arg3) {
    extern u32 fn_802370AC();
    extern u32 fn_80236F4C();
    extern u32 fn_80235974();
    extern s32 fn_80239984();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    s32 handle;
    s32 count;
    s32 limit;

    handle = 0;
    count = fn_802370AC(arg0, arg3);
    limit = fn_80236F4C(arg0, arg3);
    if ((u16)count < (u16)limit) {
        handle = fn_80239984(0, arg0, 0x187);
        fn_80239EE8(0xEC64, arg0, fightOutPokemonGetPokemonPtr(arg1), 0, 0, arg2, 0, 0x187);
    }
    if ((u8)fn_80235974(arg0, arg3) <= 4) {
        handle = fn_80239984(handle, arg0, 0x188);
        fn_80239EE8(0xEC64, arg0, fightOutPokemonGetPokemonPtr(arg1), 0, 0, arg2, 0, 0x188);
    }
    return handle;
}

/* Address: 0x80243284 | Size: 0x10C (268 bytes) */
u32 fightTrainerAiWazaValueBougyoDaun(void* arg0, void* arg1, u32 arg2, void* arg3) {
    extern u32 fn_802370AC();
    extern u32 fn_80236F4C();
    extern u32 fn_80235A3C();
    extern s32 fn_80239984();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    s32 handle;
    s32 count;
    s32 limit;

    handle = 0;
    count = fn_802370AC(arg0, arg3);
    limit = fn_80236F4C(arg0, arg3);
    if ((u16)count > (u16)limit) {
        handle = fn_80239984(0, arg0, 0x185);
        fn_80239EE8(0xEC64, arg0, fightOutPokemonGetPokemonPtr(arg1), 0, 0, arg2, 0, 0x185);
    }
    if ((u8)fn_80235A3C(arg0, arg3) <= 4) {
        handle = fn_80239984(handle, arg0, 0x186);
        fn_80239EE8(0xEC64, arg0, fightOutPokemonGetPokemonPtr(arg1), 0, 0, arg2, 0, 0x186);
    }
    return handle;
}

/* Address: 0x80243390 | Size: 0x10C (268 bytes) */
u32 fightTrainerAiWazaValueKougekiDaun(void* arg0, void* arg1, u32 arg2, void* arg3) {
    extern u32 fn_8023715C();
    extern u32 fn_80236FFC();
    extern u32 fn_80235AA0();
    extern s32 fn_80239984();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    s32 handle;
    s32 count;
    s32 limit;

    handle = 0;
    count = fn_8023715C(arg0, arg3);
    limit = fn_80236FFC(arg0, arg3);
    if ((u16)count > (u16)limit) {
        handle = fn_80239984(0, arg0, 0x183);
        fn_80239EE8(0xEC64, arg0, fightOutPokemonGetPokemonPtr(arg1), 0, 0, arg2, 0, 0x183);
    }
    if ((u8)fn_80235AA0(arg0, arg3) <= 4) {
        handle = fn_80239984(handle, arg0, 0x184);
        fn_80239EE8(0xEC64, arg0, fightOutPokemonGetPokemonPtr(arg1), 0, 0, arg2, 0, 0x184);
    }
    return handle;
}

/* Address: 0x8024349C | Size: 0x138 (312 bytes) */
u32 fightTrainerAiWazaValueSubayasaDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fightFloorGetFightTrainerFightOutPokemonPtrAry(u32, void*, u32*, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern s32 fn_80236D60(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 entries[10];
    u32 setup;
    u16 count;
    u8 found;
    u16 index;

    found = 0;
    count = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, entries, 1, 1);
    index = 0;
    while (index < count) {
        if (fn_80236D60(ctx, param3, entries[index]) > 0) {
            found = 1;
            break;
        }
        index++;
    }

    if (found == 1) {
        setup = fn_80239984(0, ctx, 0x181);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x181);
    } else {
        setup = fn_80239984(0, ctx, 0x182);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x182);
    }
    return setup;
}

/* Address: 0x802435D4 | Size: 0xB8 */
u32 fightTrainerAiWazaValueGamusyara(void* ctx, u32 param1, u32 param2, u32 extra) {
    extern u32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_802376EC(void*, u32);
    extern u32 fightTrainerAiAddValue(u32, u32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, u32);
    u32 baseHp;
    u32 handle;

    baseHp = fn_802376EC(ctx, param1);
    extra = (s32)(fn_802376EC(ctx, extra) & 0xFFFF) / (s32)(baseHp & 0xFFFF);
    extra *= fn_801FB1C0(0, 0x180, 0x3e, 0);
    handle = fightTrainerAiAddValue(0, extra);
    fn_80239CCC(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x180, extra);
    return handle;
}

/* Address: 0x8024368C | Size: 0x1AC (428 bytes) */
u32 fightTrainerAiWazaValueItamiwake(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern s32 fn_802387C8(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 ownHp;
    s32 targetHp;
    u32 handle;

    handle = 0;
    ownHp = fn_802387C8(ctx, param1);
    targetHp = fn_802387C8(ctx, param3);
    if (ownHp * 3 <= targetHp) {
        handle = fn_80239984(0, ctx, 0x17d);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x17d);
    } else if (ownHp * 2 <= targetHp) {
        handle = fn_80239984(0, ctx, 0x17c);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x17c);
    } else if (ownHp >= targetHp * 3) {
        handle = fn_80239984(0, ctx, 0x17f);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x17f);
    } else if (ownHp >= targetHp * 2) {
        handle = fn_80239984(0, ctx, 0x17e);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x17e);
    }
    return handle;
}

/* Address: 0x80243838 | Size: 0x94 */
s32 fightTrainerAiWazaValueAkumu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80236BFC();
    extern void* fightOutPokemonGetPokemonPtr();
    extern void* fn_80239984();
    extern void fn_80239EE8();
    s32 ret = 0;

    if (fn_80236BFC(ctx, param3, 0x17) == 0) {
        ret = (s32)fn_80239984(0, ctx, 0x17b);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x17b);
    }
    return ret;
}

/* Address: 0x802438CC | Size: 0x140 (320 bytes) */
u32 fightTrainerAiWazaValueRokkuon(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_801F0134(u32, u16);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u16 fn_80201D84(u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 setup;
    u16 current;

    setup = 0;
    current = fn_801F0134(param1, (u16)fn_801F54A4(0, 0, 0x14, 0));
    if (fn_80236BFC(ctx, param3, 0x1d) == 0) {
        setup = fn_80239984(0, ctx, 0x179);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x179);
    }
    if (fn_80236BFC(ctx, param3, 0x1d) == 1) {
        if (current == fn_80201D84(param3, 0x1d)) {
            setup = fn_80239984(setup, ctx, 0x17a);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x17a);
        }
    }
    return setup;
}

/* Address: 0x80243A0C | Size: 0x250 (592 bytes) */
u32 fightTrainerAiWazaValueNoroi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern f32 lbl_8047E630;
    extern f32 lbl_8047E640;
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235714(void*, u32);
    extern u32 fn_802373B0(void*, u32, s32, f32);
    extern u32 fn_80237DBC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;
    u32 state;

    handle = 0;
    state = fn_80237DBC(ctx, param1, 7);
    if ((u8)state == 1 && (u8)fn_802373B0(ctx, param1, 1, lbl_8047E630) == 1) {
        handle = fn_80239984(0, ctx, 0x174);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x174);
    } else if ((u8)state == 1 && (u8)fn_802373B0(ctx, param1, -1, lbl_8047E640) == 1) {
        handle = fn_80239984(0, ctx, 0x175);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x175);
    }
    if ((u8)state == 0 && (u8)fn_80235714(ctx, param1) == 0) {
        handle = fn_80239984(handle, ctx, 0x176);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x176);
    }
    if ((u8)state == 1) {
        handle = fn_80239984(handle, ctx, 0x177);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x177);
    }
    if ((u8)state == 0 && (u8)fn_80235714(ctx, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0x178);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x178);
    }
    return handle;
}

/* Address: 0x80243C5C | Size: 0x7C | Pattern: field_accessor */
s32 fightTrainerAiWazaValueToriaezutukae(void* ctx, u32 slot, u32 param) {
    extern s32 fightOutPokemonGetPokemonPtr();
    extern s32 fn_80239984();
    extern void fn_80239EE8();
    s32 handle = fn_80239984(0, ctx, 0x173);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x173);
    return handle;
}

/* Address: 0x80243CD8 | Size: 0x640 (1600 bytes) */
void fightTrainerAiWazaValueKoraeru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 lbl_80478DF8;
    extern f32 lbl_8047E630;
    extern void wazaGetStatus();
    extern void fn_801F1990();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80235B04();
    extern void fn_80236520();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_802373B0();
    extern void fn_802376EC();
    extern void fn_8023793C();
    extern void fn_80237CB8();
    extern void fn_8023831C();
    extern void fn_80239500();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fightTrainerAiWazaValueJisin();
    extern void fightTrainerAiWazaValueJibaku();
    extern void fightTrainerAiWazaValueNull();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r4;
    r28 = r5;
    r31 = r3;
    r16 = r6;
    r29 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r25 = r3;
    r4 = r31;
    r5 = (u32)sp + 0x20;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r15 = r3;
    r3 = r31;
    r4 = r16;
    r5 = (u32)sp + 0x8;
    fn_80237CB8();
    r16 = r3;
    r3 = r31;
    r4 = r30;
    fn_80236520();
    r27 = r3;
    r3 = r31;
    r4 = r30;
    fn_8023831C();
    r26 = r3;
    r17 = (u32)sp + 0x8;
    r16 = r16 & 0xFFFF;
    r18 = 0x0;
    r19 = 0x0;
    while (1) {
        r0 = r19 & 0xFFFF;
        if (r0 >= (u32)r16) break;
        r3 = r31;
        r4 = r28;
        fn_80239500();
        r6 = r3;
        r5 = *(u16*)(r17 + r0);
        r3 = r31;
        r4 = r30;
        fn_8023793C();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x41) {
            r18 = 0x1;
            break;
        }
        r19 = r19 + 0x1;

    }

    r0 = r18 & 0xFF;
    if (r0 == (u32)0x1) {
        r4 = r31;
        r3 = 0x0;
        r5 = 0x16a;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x16a;
        fn_80239EE8();
    }
    f1 = lbl_8047E630;
    r3 = r31;
    r4 = r30;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r29;
        r4 = r31;
        r5 = 0x16b;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x16b;
        fn_80239EE8();
    }
    r16 = (u32)sp + 0x20;
    r21 = r15 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r21) break;
        r4 = *(u32*)(r16 + r0);
        if (r30 != (u32)r4) {
            r3 = r31;
            r5 = (u32)sp + 0xc;
            r6 = 0x0;
            r7 = 0x1;
            fn_802367CC();
            r17 = r3 & 0xFFFF;
            r24 = r3;
            if (r30 != (u32)r4) {
                r4 = (u32)fightTrainerAiWazaValueJisin;
                r3 = (u32)fightTrainerAiWazaValueJibaku;
                r5 = (u32)fightTrainerAiWazaValueNull;
                r15 = (u32)sp + 0xc;
                r19 = (u32)fightTrainerAiWazaValueJisin;
                r20 = (u32)fightTrainerAiWazaValueJibaku;
                r18 = (u32)fightTrainerAiWazaValueNull;
                r22 = 0x0;
                while (1) {
                    r0 = r22 & 0xFFFF;
                    if (r0 >= (u32)r17) break;
                    r3 = 0x0;
                    r4 = *(u16*)(r15 + r0);
                    r5 = 0x1c;
                    r6 = 0x0;
                    wazaGetStatus();
                    if (r3 == (u32)0x0) {
                        r3 = r18;
                    }

                    if (r3 == (u32)r19 || r3 == (u32)r20) {

                        r3 = r29;
                        r4 = r31;
                        r5 = 0x16c;
                        fn_80239984();
                        r0 = r3;
                        r3 = r30;
                        r29 = r0;
                        fightOutPokemonGetPokemonPtr();
                        r6 = (0x1 << 16);
                        r5 = r3;
                        r4 = r31;
                        r8 = r28;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0x16c;
                        fn_80239EE8();
                        break;
                    }
                    r22 = r22 + 0x1;

                }

                r3 = r22 & 0xFFFF;
                r0 = r24 & 0xFFFF;
                if (r3 < r0) break;
        }
        }
        r23 = r23 + 0x1;

    }

    r3 = (u32)fightTrainerAiWazaValueJisin;
    r15 = 0x0;
    r16 = (u32)fightTrainerAiWazaValueJisin;
    while (1) {
        r3 = lbl_80478DF8;
        r4 = r15 & 0xFFFF;
        r0 = *(u32*)((u8*)r3 + 0x0);
        if (r4 >= (u32)r0) break;
        r4 = r15;
        r3 = 0x0;
        r5 = 0x1c;
        r6 = 0x0;
        wazaGetStatus();
        if (r3 == (u32)0x0) {
            r3 = (u32)fightTrainerAiWazaValueNull;
            r3 = (u32)fightTrainerAiWazaValueNull;
        }
        if (r3 != (u32)r16) {
            r4 = (u32)fightTrainerAiWazaValueJibaku;
            r0 = (u32)fightTrainerAiWazaValueJibaku;
            if (r3 == (u32)r0) {
            }
            r4 = r31;
            r7 = r15;
            r3 = 0x0;
            r5 = 0x1;
            r6 = 0x1;
            r8 = 0x0;
            fn_801F1990();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r29;
                r4 = r31;
                r5 = 0x16d;
                fn_80239984();
                r0 = r3;
                r3 = r30;
                r29 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r31;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x16d;
                fn_80239EE8();
                break;
            }
            }
        r15 = r15 + 0x1;

    }

    r3 = r31;
    r4 = r30;
    r15 = 0x0;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r0 = r15 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r29;
        r4 = r31;
        r5 = 0x16e;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x16e;
        fn_80239EE8();
    }
    r0 = r27 & 0xFFFF;
    if (r0 == (u32)0xcb) {
        r3 = r30;
        r4 = 0x0;
        r5 = 0xfc;
        r6 = 0x0;
        ((void(*)(void))pokemonGetStatus)();
        if ((s32)r3 != (s32)0x0) {
            r3 = r29;
            r4 = r31;
            r5 = 0x16f;
            fn_80239984();
            r0 = r3;
            r3 = r30;
            r29 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r31;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x16f;
            fn_80239EE8();
    }
    }
    r0 = r26 & 0xFFFF;
    if ((r0 != (u32)0x11) && (r0 != (u32)0xf)) {

        r3 = r29;
        r4 = r31;
        r5 = 0x170;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x170;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    fn_802376EC();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x1) {
        r3 = r29;
        r4 = r31;
        r5 = 0x171;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x171;
        fn_80239EE8();
    }
    r0 = r25 & 0xFF;
    if (r0 != (u32)0x4) {
        if (r0 != (u32)0x3) { r3 = r29; return; }
    }
    r3 = r29;
    r4 = r31;
    r5 = 0x172;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x172;
    fn_80239EE8();

    r3 = r29;
    return;
}

/* Address: 0x80244318 | Size: 0x160 (352 bytes) */
u32 fightTrainerAiWazaValueMiyaburu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_802357CC(void*, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_80237DBC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if (fn_80237DBC(ctx, param3, 7) == 1) {
        handle = fn_80239984(0, ctx, 0x167);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x167);
    }
    if ((fn_802357CC(ctx, param3) & 0xff) >= 8) {
        handle = fn_80239984(handle, ctx, 0x168);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x168);
    }
    if (fn_80236BFC(ctx, param3, 0x19) == 1) {
        handle = fn_80239984(handle, ctx, 0x169);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x169);
    }
    return handle;
}

/* Address: 0x80244478 | Size: 0x9C */
u32 fightTrainerAiWazaValueMajikkukooto(void* ctx, u32 param1, u32 param2) {
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if ((pokemonGetStatus(param1, 0, 0xed, 0) & 0xFFFF) != 0) {
        handle = fn_80239984(0, ctx, 0x166);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x166);
    }
    return handle;
}

/* Address: 0x80244514 | Size: 0x18C (396 bytes) */
u32 fightTrainerAiWazaValueMiraakooto(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_8010C4A0(u32);
    extern u32 fightFloorGetFightTrainerFightOutPokemonPtrAry(u32, void*, u32*, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236520(void*, u32);
    extern u32 fn_80236FFC(void*, u32);
    extern u32 fn_8023715C(void*, u32);
    extern u32 fn_802395C8(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 entries[8];
    u32 rawCount;
    u32 count;
    u32* entriesPtr;
    u32 move;
    u32 index;
    u32 current;
    u32 handle;

    handle = 0;
    rawCount = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, entries, 0, 1);
    move = fn_80236520(ctx, param3);
    entriesPtr = entries;
    count = rawCount & 0xffff;
    index = 0;
    while ((u16)index < count) {
        current = fn_8023715C(ctx, entriesPtr[(u16)index]);
        if ((u16)current < (u16)fn_80236FFC(ctx, entriesPtr[(u16)index])) {
            handle = fn_80239984(0, ctx, 0x164);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x164);
            break;
        }
        index++;
    }
    if ((u16)move != 0 && (u16)move != 0xffff && (u16)move != 0x165 && (u16)move != 0x163) {
        if ((u8)fn_8010C4A0(fn_802395C8(ctx, move, param3)) == 2) {
            handle = fn_80239984(handle, ctx, 0x165);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x165);
        }
    }
    return handle;
}

/* Address: 0x802446A0 | Size: 0x18C (396 bytes) */
u32 fightTrainerAiWazaValueKauntaa(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_8010C4A0(u32);
    extern u32 fightFloorGetFightTrainerFightOutPokemonPtrAry(u32, void*, u32*, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236520(void*, u32);
    extern u32 fn_80236FFC(void*, u32);
    extern u32 fn_8023715C(void*, u32);
    extern u32 fn_802395C8(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 entries[8];
    u32 rawCount;
    u32 count;
    u32* entriesPtr;
    u32 move;
    u32 index;
    u32 current;
    u32 handle;

    handle = 0;
    rawCount = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, entries, 0, 1);
    move = fn_80236520(ctx, param3);
    entriesPtr = entries;
    count = rawCount & 0xffff;
    index = 0;
    while ((u16)index < count) {
        current = fn_8023715C(ctx, entriesPtr[(u16)index]);
        if ((u16)current > (u16)fn_80236FFC(ctx, entriesPtr[(u16)index])) {
            handle = fn_80239984(0, ctx, 0x162);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x162);
            break;
        }
        index++;
    }
    if ((u16)move != 0 && (u16)move != 0xffff && (u16)move != 0x165 && (u16)move != 0x163) {
        if ((u8)fn_8010C4A0(fn_802395C8(ctx, move, param3)) == 1) {
            handle = fn_80239984(handle, ctx, 0x163);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x163);
        }
    }
    return handle;
}

/* Address: 0x8024482C | Size: 0xD4 (212 bytes) */
u32 fightTrainerAiWazaValueAroma(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1A6C(u32, void*, u32*, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80238748(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 list[23];
    void* battleCtx = ctx;
    u32 trainer = param1;
    u32 sequenceArg = param2;
    u32* listPtr;
    u32 handle = 0;
    u32 i;
    u32 count;

    count = fn_801F1A6C(0, battleCtx, list, 1, 1);
    listPtr = list;
    count &= 0xFFFF;
    for (i = 0; (u16)i < count; i++) {
        if (fn_80238748(battleCtx, listPtr[(u16)i]) == 0) {
            handle = fn_80239984(0, battleCtx, 0x161);
            fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x161);
            break;
        }
    }
    return handle;
}

/* Address: 0x80244900 | Size: 0x8C */
u32 fightTrainerAiWazaValueRihuressyu(void* ctx, u32 param1, u32 param2) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void* ctx);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle = 0;

    if (fn_80237310(ctx) == 0) {
        u32 tmp = fn_80239984(0, ctx, 0x160);
        handle = tmp;
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x160);
    }
    return handle;
}

/* Address: 0x8024498C | Size: 0x318 (792 bytes) */
void fightTrainerAiWazaValueSunaarasi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80235B04();
    extern void fn_802377E8();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239058();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r26 = r4;
    r27 = r5;
    r25 = r3;
    r28 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r29 = r3;
    r4 = r25;
    r5 = (u32)sp + 0x28;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r30 = r3;
    r4 = r25;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r0 = r29 & 0xFF;
    r31 = r3;
    if (r0 != (u32)0x3) {
        r4 = r25;
        r3 = 0x0;
        r5 = 0x15b;
        fn_80239984();
        r0 = r3;
        r3 = r26;
        r28 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r25;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x15b;
        fn_80239EE8();
    }
    r22 = (u32)sp + 0x28;
    r24 = r30 & 0xFFFF;
    r21 = 0x0;
    while (1) {
        r0 = r21 & 0xFFFF;
        if (r0 >= (u32)r24) break;
        r3 = r25;
        r4 = *(u32*)(r22 + r23);
        r5 = 0x5;
        fn_80238E30();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r4 = *(u32*)(r22 + r23);
            r3 = r25;
            r5 = 0x4;
            fn_80238E30();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r4 = *(u32*)(r22 + r23);
                r3 = r25;
                r5 = 0x8;
                fn_80238E30();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                    r4 = *(u32*)(r22 + r23);
                    r3 = r25;
                    r5 = 0x8;
                    fn_80239058();
                    r0 = r3 & 0xFF;
                    if (r0 == (u32)0x1) {
            }
            }
            }
            r3 = r28;
            r4 = r25;
            r5 = 0x15c;
            fn_80239984();
            r0 = r3;
            r3 = r26;
            r28 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r25;
            r8 = r27;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x15c;
            fn_80239EE8();
            break;
                    }
        r21 = r21 + 0x1;

    }

    r24 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r31) break;
        r3 = r25;
        r4 = *(u32*)(r24 + r0);
        fn_802377E8();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x12f) {
            r3 = r28;
            r4 = r25;
            r5 = 0x15d;
            fn_80239984();
            r0 = r3;
            r3 = r26;
            r28 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r25;
            r8 = r27;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x15d;
            fn_80239EE8();
            break;
        }
        r22 = r22 + 0x1;

    }

    r31 = (u32)sp + 0x28;
    r30 = r30 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r30) break;
        r3 = r25;
        r4 = *(u32*)(r31 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x181) {
            r3 = r28;
            r4 = r25;
            r5 = 0x15e;
            fn_80239984();
            r0 = r3;
            r3 = r26;
            r28 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r25;
            r8 = r27;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x15e;
            fn_80239EE8();
            break;
        }
        r22 = r22 + 0x1;

    }

    r0 = r29 & 0xFF;
    if (r0 == (u32)0x3) {
        r3 = r28;
        r4 = r25;
        r5 = 0x15f;
        fn_80239984();
        r0 = r3;
        r3 = r26;
        r28 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r25;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x15f;
        fn_80239EE8();
    }
    r3 = r28;
    return;
}

/* Address: 0x80244CA4 | Size: 0x2C4 (708 bytes) */
void fightTrainerAiWazaValueArare(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80235B04();
    extern void fn_802377E8();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r26 = r4;
    r27 = r5;
    r25 = r3;
    r28 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r29 = r3;
    r4 = r25;
    r5 = (u32)sp + 0x28;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r30 = r3;
    r4 = r25;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r0 = r29 & 0xFF;
    r31 = r3;
    if (r0 != (u32)0x4) {
        r4 = r25;
        r3 = 0x0;
        r5 = 0x156;
        fn_80239984();
        r0 = r3;
        r3 = r26;
        r28 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r25;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x156;
        fn_80239EE8();
    }
    r23 = (u32)sp + 0x28;
    r24 = r30 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r24) break;
        r3 = r25;
        r4 = *(u32*)(r23 + r0);
        r5 = 0xf;
        fn_80238E30();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = r28;
            r4 = r25;
            r5 = 0x157;
            fn_80239984();
            r0 = r3;
            r3 = r26;
            r28 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r25;
            r8 = r27;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x157;
            fn_80239EE8();
            break;
        }
        r22 = r22 + 0x1;

    }

    r24 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r31) break;
        r3 = r25;
        r4 = *(u32*)(r24 + r0);
        fn_802377E8();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x12f) {
            r3 = r28;
            r4 = r25;
            r5 = 0x158;
            fn_80239984();
            r0 = r3;
            r3 = r26;
            r28 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r25;
            r8 = r27;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x158;
            fn_80239EE8();
            break;
        }
        r23 = r23 + 0x1;

    }

    r31 = (u32)sp + 0x28;
    r30 = r30 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r30) break;
        r3 = r25;
        r4 = *(u32*)(r31 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x181) {
            r3 = r28;
            r4 = r25;
            r5 = 0x159;
            fn_80239984();
            r0 = r3;
            r3 = r26;
            r28 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r25;
            r8 = r27;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x159;
            fn_80239EE8();
            break;
        }
        r23 = r23 + 0x1;

    }

    r0 = r29 & 0xFF;
    if (r0 == (u32)0x4) {
        r3 = r28;
        r4 = r25;
        r5 = 0x15a;
        fn_80239984();
        r0 = r3;
        r3 = r26;
        r28 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r25;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x15a;
        fn_80239EE8();
    }
    r3 = r28;
    return;
}

/* Address: 0x80244F68 | Size: 0x258 (600 bytes) */
void fightTrainerAiWazaValueNihonbare(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80235B04();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239058();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r30 = r3;
    r4 = r26;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r0 = r30 & 0xFF;
    r31 = r3;
    if (r0 != (u32)0x1) {
        r4 = r26;
        r3 = 0x0;
        r5 = 0x152;
        fn_80239984();
        r0 = r3;
        r3 = r27;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x152;
        fn_80239EE8();
    }
    r23 = (u32)sp + 0x8;
    r25 = r31 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r25) break;
        r3 = r26;
        r4 = *(u32*)(r23 + r24);
        r5 = 0xa;
        fn_80238E30();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r4 = *(u32*)(r23 + r24);
            r3 = r26;
            r5 = 0xc;
            fn_80238E30();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r4 = *(u32*)(r23 + r24);
                r3 = r26;
                r5 = 0x22;
                fn_80239058();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
            }
            }
            r3 = r29;
            r4 = r26;
            r5 = 0x153;
            fn_80239984();
            r0 = r3;
            r3 = r27;
            r29 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r26;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x153;
            fn_80239EE8();
            break;
                }
        r22 = r22 + 0x1;

    }

    r25 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r31) break;
        r3 = r26;
        r4 = *(u32*)(r25 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x181) {
            r3 = r29;
            r4 = r26;
            r5 = 0x154;
            fn_80239984();
            r0 = r3;
            r3 = r27;
            r29 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r26;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x154;
            fn_80239EE8();
            break;
        }
        r23 = r23 + 0x1;

    }

    r0 = r30 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r29;
        r4 = r26;
        r5 = 0x155;
        fn_80239984();
        r0 = r3;
        r3 = r27;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x155;
        fn_80239EE8();
    }
    r3 = r29;
    return;
}

/* Address: 0x802451C0 | Size: 0x258 (600 bytes) */
void fightTrainerAiWazaValueAmagoi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80235B04();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239058();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r30 = r3;
    r4 = r26;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r0 = r30 & 0xFF;
    r31 = r3;
    if (r0 != (u32)0x2) {
        r4 = r26;
        r3 = 0x0;
        r5 = 0x14e;
        fn_80239984();
        r0 = r3;
        r3 = r27;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x14e;
        fn_80239EE8();
    }
    r23 = (u32)sp + 0x8;
    r25 = r31 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r25) break;
        r3 = r26;
        r4 = *(u32*)(r23 + r24);
        r5 = 0xb;
        fn_80238E30();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r4 = *(u32*)(r23 + r24);
            r3 = r26;
            r5 = 0x21;
            fn_80239058();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r4 = *(u32*)(r23 + r24);
                r3 = r26;
                r5 = 0x2c;
                fn_80239058();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
            }
            }
            r3 = r29;
            r4 = r26;
            r5 = 0x14f;
            fn_80239984();
            r0 = r3;
            r3 = r27;
            r29 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r26;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x14f;
            fn_80239EE8();
            break;
                }
        r22 = r22 + 0x1;

    }

    r25 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r31) break;
        r3 = r26;
        r4 = *(u32*)(r25 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x181) {
            r3 = r29;
            r4 = r26;
            r5 = 0x150;
            fn_80239984();
            r0 = r3;
            r3 = r27;
            r29 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r26;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x150;
            fn_80239EE8();
            break;
        }
        r23 = r23 + 0x1;

    }

    r0 = r30 & 0xFF;
    if (r0 == (u32)0x2) {
        r3 = r29;
        r4 = r26;
        r5 = 0x151;
        fn_80239984();
        r0 = r3;
        r3 = r27;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x151;
        fn_80239EE8();
    }
    r3 = r29;
    return;
}

/* Address: 0x80245418 | Size: 0x160 (352 bytes) */
u32 fightTrainerAiWazaValueNemurare(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u16 fn_801F1A6C(u32, void*, u32*, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void*, u32);
    extern u8 fn_802384B4(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 entries[26];
    u32 setup;
    u32 compareValue;
    u16 count;
    u16 index;

    setup = 0;
    count = fn_801F1A6C(0, ctx, entries, 0, 1);
    if (fn_80237310(ctx, param3) == 1) {
        setup = fn_80239984(0, ctx, 0x14c);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x14c);
    }

    index = 0;
    while (index < count) {
        compareValue = pokemonGetStatus(param3, 0, 0xd5, 0);
        if (compareValue == entries[index]) {
            ;
        } else {
            if (fn_802384B4(ctx, entries[index], 8) == 1) {
                setup = fn_80239984(setup, ctx, 0x14d);
                fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x14d);
                break;
            }
        }
        index++;
    }

    return setup;
}

/* Address: 0x80245578 | Size: 0x1A0 (416 bytes) */
u32 fightTrainerAiWazaValueNemuru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern f32 lbl_8047E630;
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void*, u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);
    extern u32 fn_8023831C(void*);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;
    u32 state;

    handle = 0;
    state = fn_8023831C(ctx);
    if ((state & 0xffff) == 3 || (state & 0xffff) == 9) {
        handle = fn_80239984(0, ctx, 0x148);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x148);
    }
    if (fn_802373B0(ctx, param1, -1, lbl_8047E630) == 1) {
        handle = fn_80239984(handle, ctx, 0x149);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x149);
    }
    if (fn_80237310(ctx, param1) == 0) {
        handle = fn_80239984(handle, ctx, 0x14a);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x14a);
    }
    handle = fn_80239984(handle, ctx, 0x14b);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x14b);
    return handle;
}

/* Address: 0x80245718 | Size: 0x98 */
u32 fightTrainerAiWazaValueKaihuku2(void* ctx, u32 param1, u32 param2) {
    extern f32 lbl_8047E630;
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if (fn_802373B0(ctx, param1, -1, lbl_8047E630) == 1) {
        handle = fn_80239984(0, ctx, 0x147);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x147);
    }
    return handle;
}

/* Address: 0x802457B0 | Size: 0x168 (360 bytes) */
u32 fightTrainerAiWazaValueKaihuku1(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern f32 lbl_8047E630;
    extern u32 fn_80235B04();
    extern u8 fn_802373B0(void* a, u32 b, s32 c, f32 d);
    extern s32 fn_80239984();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    s32 handle = 0;
    u32 state;

    state = fn_80235B04(ctx, 0, 1);
    if ((u8)state == 1) {
        handle = fn_80239984(0, ctx, 0x144);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x144);
    }
    if ((u8)fn_802373B0(ctx, param1, -1, lbl_8047E630) == 1) {
        handle = fn_80239984(handle, ctx, 0x145);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x145);
    }
    if ((u8)state == 2 || (u8)state == 4 || (u8)state == 3) {
        handle = fn_80239984(handle, ctx, 0x146);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x146);
    }
    return handle;
}

/* Address: 0x80245918 | Size: 0x98 */
u32 fightTrainerAiWazaValueKituke(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle = 0;

    if (fn_80236BFC(ctx, param3, 5) == 1) {
        u32 tmp = fn_80239984(0, ctx, 0x143);
        handle = tmp;
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x143);
    }
    return handle;
}

/* Address: 0x802459B0 | Size: 0x44C (1100 bytes) */
u32 fightTrainerAiWazaValueOiuti(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8A18(u32, u16*);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235BE4(void*, u32, u32, u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u16 tmp;
    u32 found;
    u32 handle;
    u32 pokemon;
    u32 status;

    handle = 0;
    status = fn_80235BE4(ctx, 0, param3, 0);
    pokemon = fn_801F4354(0, param3);
    tmp = 0;
    found = fn_801F8A18(pokemon, &tmp);
    if (found == 0) {
        status = 1;
    }
    if ((u8)fn_80236BFC(ctx, param3, 3) == 1) {
        handle = fn_80239984(0, ctx, 0x139);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x139);
    }
    if ((u8)fn_80236BFC(ctx, param3, 4) == 1) {
        handle = fn_80239984(handle, ctx, 0x13a);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x13a);
    }
    if ((u8)fn_80236BFC(ctx, param3, 6) == 1) {
        handle = fn_80239984(handle, ctx, 0x13b);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x13b);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1c) == 1) {
        handle = fn_80239984(handle, ctx, 0x13c);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x13c);
    }
    if ((u8)fn_80236BFC(ctx, param3, 9) == 1) {
        handle = fn_80239984(handle, ctx, 0x13d);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x13d);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0xa) == 1) {
        handle = fn_80239984(handle, ctx, 0x13e);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x13e);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x18) == 1) {
        handle = fn_80239984(handle, ctx, 0x13f);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x13f);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1e) == 1) {
        handle = fn_80239984(handle, ctx, 0x140);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x140);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x26) == 1) {
        handle = fn_80239984(handle, ctx, 0x141);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x141);
    }
    if ((u8)status != 0) {
        handle = fn_80239984(handle, ctx, 0x142);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x142);
    }
    return handle;
}

/* Address: 0x80245DFC | Size: 0x14C (332 bytes) */
u32 fightTrainerAiWazaValueUezaabooru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235B04(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;
    u32 state;

    handle = 0;
    state = fn_80235B04(ctx, 0, 1);
    if ((u8)state == 1 || (u8)state == 2) {
        handle = fn_80239984(0, ctx, 0x136);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x136);
    }
    if ((u8)state == 4) {
        handle = fn_80239984(handle, ctx, 0x137);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x137);
    }
    if ((u8)state == 3) {
        handle = fn_80239984(handle, ctx, 0x138);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x138);
    }
    return handle;
}

/* Address: 0x80245F48 | Size: 0x7C | Pattern: field_accessor */
u32 fightTrainerAiWazaValueSizennotikara(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u16 fn_801363E8(u16);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u32 fn_8023CA9C(void*, u32, u16, u32);
    u16 value;

    value = fn_801363E8((u16)fn_801F54A4(0, 0, 0xf, 0));
    if (value != (u16)param) {
        return fn_8023CA9C(ctx, slot, value, extra);
    }
    return 0;
}

/* Address: 0x80247048 | Size: 0x7C | Pattern: field_accessor */
s32 fightTrainerAiWazaValueItigekihissatu(void* ctx, u32 slot, u32 param) {
    extern s32 fightOutPokemonGetPokemonPtr();
    extern s32 fn_80239984();
    extern void fn_80239EE8();
    s32 handle = fn_80239984(0, ctx, 0x135);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x135);
    return handle;
}

/* Address: 0x802470C4 | Size: 0xEC (236 bytes) */
u32 fightTrainerAiWazaValueZennouryokuappu(void* ctx, u32 slot, u32 param, u32 unused) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x133);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x133);
    if (fn_801F1990(0, ctx, 1, 1, 0x10e, slot) == 1) {
        handle = fn_80239984(handle, ctx, 0x134);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x134);
    }
    return handle;
}

/* Address: 0x802471B0 | Size: 0x128 (296 bytes) */
u32 fightTrainerAiWazaValueTuikaKougekiAppu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80235AA0(void*, u32);
    extern u8 fn_80239564(void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    u32 battleParam = param2;
    u32 trainerParam = param1;
    void* battleCtx = ctx;
    s32 quotient;
    u32 setupHandle;
    u32 pokemonPtr;
    u32 statusValue;

    statusValue = fn_80239564(battleCtx, battleParam);
    quotient = (s32)statusValue / fn_801FB1C0(0, 0x131, 0x3e, 0);
    setupHandle = fightTrainerAiAddValue(0, quotient);
    pokemonPtr = fightOutPokemonGetPokemonPtr(trainerParam);
    fn_80239CCC(0xEC64, battleCtx, pokemonPtr, 0, 0, battleParam, 0, 0x131, quotient);

    if (fn_80235AA0(battleCtx, trainerParam) >= 0xcU) {
        statusValue = fn_80239564(battleCtx, battleParam);
        quotient = (s32)statusValue / fn_801FB1C0(0, 0x132, 0x3e, 0);
        setupHandle = fightTrainerAiAddValue(setupHandle, quotient);
        pokemonPtr = fightOutPokemonGetPokemonPtr(trainerParam);
        fn_80239CCC(0xEC64, battleCtx, pokemonPtr, 0, 0, battleParam, 0, 0x132, quotient);
    }

    return setupHandle;
}

/* Address: 0x802472D8 | Size: 0xDC (220 bytes) */
u32 fightTrainerAiWazaValueHaganenotubasa(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80235A3C(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x12f);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x12f);
    if (fn_80235A3C(ctx, param1) >= 0xc) {
        handle = fn_80239984(handle, ctx, 0x130);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x130);
    }
    return handle;
}

/* Address: 0x802473B4 | Size: 0x144 (324 bytes) */
u32 fn_802473B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_8023831C(void*);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    void* battleCtx = ctx;
    u32 trainer = param1;
    u32 sequenceArg = param2;
    u32 handle = 0;

    if ((fn_8023831C(battleCtx) & 0xFFFF) == 0x17) {
        handle = fn_80239984(0, battleCtx, 0x12c);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x12c);
    }
    if (fn_801F1990(0, battleCtx, 1, 1, 0x10e, trainer) == 1) {
        handle = fn_80239984(handle, battleCtx, 0x12d);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x12d);
    }
    handle = fn_80239984(handle, battleCtx, 0x12e);
    fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x12e);
    return handle;
}

/* Address: 0x802474F8 | Size: 0x1A8 (424 bytes) */
u32 fightTrainerAiWazaValueOobaahiito(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u32 fn_8023831C(void*);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if ((fn_8023831C(ctx) & 0xffff) == 0x17) {
        handle = fn_80239984(0, ctx, 0x128);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x128);
    }
    if (fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0x129);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x129);
    }
    handle = fn_80239984(handle, ctx, 0x12a);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x12a);
    if (fn_80236BFC(ctx, param3, 7) == 1) {
        handle = fn_80239984(handle, ctx, 0x12b);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x12b);
    }
    return handle;
}

/* Address: 0x802476A0 | Size: 0x110 (272 bytes) */
s32 fightTrainerAiWazaValueHatakiotosu(void* ctx, void* param1, u32 param2, u32 param3) {
    extern void* fightOutPokemonGetPokemonPtr();
    extern u8 fn_80236BFC();
    extern u8 fn_80237F74();
    extern u16 fn_802383A4();
    extern void* fn_80239984();
    extern void fn_80239EE8();
    s32 handle = 0;

    if (fn_80236BFC(ctx, param3, 0x3d) == 0 && fn_802383A4(ctx, param3) != 0) {
        handle = (s32)fn_80239984(0, ctx, 0x126);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x126);
    }
    if (fn_80237F74(ctx, param3, 0x3c) == 1) {
        handle = (s32)fn_80239984(handle, ctx, 0x127);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x127);
    }
    return handle;
}

/* Address: 0x802477B0 | Size: 0x158 (344 bytes) */
u32 fightTrainerAiWazaValueDouguUbau(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80142984(u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80216048(u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u32 fn_802383A4();
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    void* battleCtx = ctx;
    u32 trainer = param1;
    u32 sequenceArg = param2;
    u32 target = param3;
    u32 handle = 0;
    u8 canSteal;
    u32 targetItem;
    u32 ownItem;

    ownItem = fn_802383A4(battleCtx);
    targetItem = fn_802383A4(battleCtx, target);
    canSteal = 1;
    if (fn_80216048(trainer) == 0) {
        canSteal = 0;
    }
    if ((ownItem & 0xFFFF) != 0 || (targetItem & 0xFFFF) == 0xAF || (targetItem & 0xFFFF) == 0 ||
        fn_80142984(targetItem) == 0) {
        canSteal = 0;
    }
    if (canSteal == 1) {
        handle = fn_80239984(0, battleCtx, 0x124);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x124);
    }
    if (fn_80237F74(battleCtx, target, 0x3c) == 1) {
        handle = fn_80239984(handle, battleCtx, 0x125);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x125);
    }
    return handle;
}

/* Address: 0x80247908 | Size: 0x1C0 (448 bytes) */
u32 fightTrainerAiWazaValueSooraabiimu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235B04(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 trainer;
    u32 sequenceArg;
    void* battleCtx;
    u32 handle;
    u32 state;

    trainer = param1;
    sequenceArg = param2;
    battleCtx = ctx;
    handle = 0;
    state = fn_80235B04(battleCtx, 0, 1);
    if ((u8)state == 1) {
        handle = fn_80239984(0, battleCtx, 0x120);
        fn_80239EE8(0xec64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x120);
    }
    if (fn_801F1990(0, battleCtx, 1, 1, 0x10e, trainer) == 1) {
        handle = fn_80239984(handle, battleCtx, 0x121);
        fn_80239EE8(0xec64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x121);
    }
    if ((u8)state == 2 || (u8)state == 4 || (u8)state == 3) {
        handle = fn_80239984(handle, battleCtx, 0x122);
        fn_80239EE8(0xec64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x122);
    }
    if ((u8)state == 0) {
        handle = fn_80239984(handle, battleCtx, 0x123);
        fn_80239EE8(0xec64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x123);
    }
    return handle;
}

/* Address: 0x80247AC8 | Size: 0x194 (404 bytes) */
u32 fightTrainerAiWazaValueToraiatakku(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void*, u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x11d);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x11d);
    if (fn_80237310(ctx, param3) == 0) {
        handle = fn_80239984(handle, ctx, 0x11e);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x11e);
    }
    if (fn_80237F74(ctx, param3, 0x13) == 1 || fn_80237F74(ctx, param3, 7) == 1 ||
        fn_80237F74(ctx, param3, 0x29) == 1 || fn_80237F74(ctx, param3, 0x28) == 1) {
        handle = fn_80239984(handle, ctx, 0x11f);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x11f);
    }
    return handle;
}

/* Address: 0x80247C5C | Size: 0x184 (388 bytes) */
u32 fightTrainerAiWazaValueDokudokunokiba(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void*, u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x11a);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x11a);
    if (fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0x11b);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x11b);
    }
    if (fn_80237310(ctx, param3) == 0 || fn_80237F74(ctx, param3, 0x11) == 1 ||
        fn_80237F74(ctx, param3, 0x13) == 1) {
        handle = fn_80239984(handle, ctx, 0x11c);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x11c);
    }
    return handle;
}

/* Address: 0x80247DE0 | Size: 0x1C0 (448 bytes) */
u32 fightTrainerAiWazaValuePoizunteeru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void*, u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x116);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x116);
    handle = fn_80239984(handle, ctx, 0x117);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x117);
    if (fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0x118);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x118);
    }
    if (fn_80237310(ctx, param3) == 0 || fn_80237F74(ctx, param3, 0x11) == 1 ||
        fn_80237F74(ctx, param3, 0x13) == 1) {
        handle = fn_80239984(handle, ctx, 0x119);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x119);
    }
    return handle;
}

/* Address: 0x80247FA0 | Size: 0x1D0 (464 bytes) */
u32 fightTrainerAiWazaValueTuikaDoku(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void*, u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u8 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 battleParam = param2;
    u32 trainerParam = param1;
    void* battleCtx = ctx;
    u32 target = param3;
    s32 quotient;
    u32 handle;
    u32 pokemonPtr;
    u32 statusValue;

    statusValue = fn_80239564(battleCtx, battleParam);
    quotient = (s32)statusValue / fn_801FB1C0(0, 0x113, 0x3e, 0);
    handle = fightTrainerAiAddValue(0, quotient);
    pokemonPtr = fightOutPokemonGetPokemonPtr(trainerParam);
    fn_80239CCC(0xec64, battleCtx, pokemonPtr, 0, 0, battleParam, 0, 0x113, quotient);
    if (fn_801F1990(0, battleCtx, 1, 1, 0x10e, trainerParam) == 1) {
        handle = fn_80239984(handle, battleCtx, 0x114);
        fn_80239EE8(0xec64, battleCtx, fightOutPokemonGetPokemonPtr(trainerParam), 0, 0, battleParam, 0, 0x114);
    }
    if (fn_80237310(battleCtx, target) == 0 || fn_80237F74(battleCtx, target, 0x11) == 1 ||
        fn_80237F74(battleCtx, target, 0x13) == 1) {
        statusValue = fn_80239564(battleCtx, battleParam);
        quotient = (s32)statusValue / fn_801FB1C0(0, 0x115, 0x3e, 0);
        handle = fightTrainerAiAddValue(handle, quotient);
        pokemonPtr = fightOutPokemonGetPokemonPtr(trainerParam);
        fn_80239CCC(0xec64, battleCtx, pokemonPtr, 0, 0, battleParam, 0, 0x115, quotient);
    }
    return handle;
}

/* Address: 0x80248170 | Size: 0x150 (336 bytes) */
u32 fightTrainerAiWazaValueHandou(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle = 0;

    if ((fn_801F1990(0, ctx, 1, 1, 0x10e, param1) & 0xFF) == 1) {
        {
            u32 nextHandle = fn_80239984(0, ctx, 0x110);
            handle = nextHandle;
        }
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x110);
    }
    if ((fn_80237F74(ctx, param1, 0x36) & 0xFF) == 1) {
        {
            u32 nextHandle = fn_80239984(handle, ctx, 0x111);
            handle = nextHandle;
        }
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x111);
    }
    handle = fn_80239984(handle, ctx, 0x112);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x112);
    return handle;
}

/* Address: 0x802482C0 | Size: 0x1A0 (416 bytes) */
s32 fightTrainerAiWazaValueBakuretuPanti(void* ctx, void* param1, u32 param2, u32 param3) {
    extern u16 fn_801F54A4();
    extern u32 fn_801F0134();
    extern u8 fn_80236BFC();
    extern u16 fn_80201D84();
    extern u8 fn_80237F74();
    extern u32 fn_80239984();
    extern void* fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    s32 h = 0;
    u32 r28v;

    r28v = fn_801F0134(param1, fn_801F54A4(0, 0, 0x14, 0));
    if (fn_80236BFC(ctx, param3, 0x1d) == 1 &&
        (u16)r28v == (u16)fn_80201D84(param3, 0x1d)) {
        h = fn_80239984(0, ctx, 0x10a);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x10a);
    }
    r28v = fn_80239984(h, ctx, 0x10b);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x10b);
    if (fn_80236BFC(ctx, param3, 9) == 1 || fn_80237F74(ctx, param3, 0x14) == 1) {
        r28v = fn_80239984(r28v, ctx, 0x10c);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x10c);
    }
    return r28v;
}

/* Address: 0x80248460 | Size: 0x22C (556 bytes) */
s32 fightTrainerAiWazaValueDenjihou(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_801F0134();
    extern u16 fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern u16 fn_801F54A4();
    extern u16 fn_80201D84();
    extern void* fightOutPokemonGetPokemonPtr();
    extern u8 fn_80236BFC();
    extern s32 fn_80236D60();
    extern u8 fn_80237310();
    extern u8 fn_80237F74();
    extern void* fn_80239984();
    extern void fn_80239EE8();
    u32 buf[8];
    s32 handle = 0;
    u16 someVal;
    u16 count;
    u16 i;

    someVal = fn_801F0134(param1, fn_801F54A4(0, 0, 0x14, 0));
    count = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, buf, 1, 1);

    for (i = 0; i < count; i++) {
        if (buf[i] != 0 && fn_80236D60(ctx, param3, buf[i]) > 0) {
            break;
        }
    }

    if (i < count) {
        handle = (s32)fn_80239984(0, ctx, 0x107);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x107);
    }

    if (fn_80236BFC(ctx, param3, 0x1D) == 1 && someVal == fn_80201D84(param3, 0x1D)) {
        handle = (s32)fn_80239984(handle, ctx, 0x108);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x108);
    }

    if (fn_80237310(ctx, param3) == 0 || fn_80237F74(ctx, param3, 7) == 1 ||
        fn_80237F74(ctx, param3, 0x13) == 1) {
        handle = (s32)fn_80239984(handle, ctx, 0x109);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x109);
    }

    return handle;
}

/* Address: 0x8024868C | Size: 0x1D0 (464 bytes) */
u32 fightTrainerAiWazaValueTuikaMahi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237310(void*, u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u8 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 battleParam = param2;
    u32 trainerParam = param1;
    void* battleCtx = ctx;
    u32 target = param3;
    s32 quotient;
    u32 handle;
    u32 pokemonPtr;
    u32 statusValue;

    statusValue = fn_80239564(battleCtx, battleParam);
    quotient = (s32)statusValue / fn_801FB1C0(0, 0x104, 0x3e, 0);
    handle = fightTrainerAiAddValue(0, quotient);
    pokemonPtr = fightOutPokemonGetPokemonPtr(trainerParam);
    fn_80239CCC(0xec64, battleCtx, pokemonPtr, 0, 0, battleParam, 0, 0x104, quotient);
    if (fn_801F1990(0, battleCtx, 1, 1, 0x10e, trainerParam) == 1) {
        handle = fn_80239984(handle, battleCtx, 0x105);
        fn_80239EE8(0xec64, battleCtx, fightOutPokemonGetPokemonPtr(trainerParam), 0, 0, battleParam, 0, 0x105);
    }
    if (fn_80237310(battleCtx, target) == 0 || fn_80237F74(battleCtx, target, 7) == 1 ||
        fn_80237F74(battleCtx, target, 0x13) == 1) {
        statusValue = fn_80239564(battleCtx, battleParam);
        quotient = (s32)statusValue / fn_801FB1C0(0, 0x106, 0x3e, 0);
        handle = fightTrainerAiAddValue(handle, quotient);
        pokemonPtr = fightOutPokemonGetPokemonPtr(trainerParam);
        fn_80239CCC(0xec64, battleCtx, pokemonPtr, 0, 0, battleParam, 0, 0x106, quotient);
    }
    return handle;
}

/* Address: 0x8024885C | Size: 0x2C0 (704 bytes) */
void fightTrainerAiWazaValueTuikaKoori(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fn_801FB1C0();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_802384B4();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fightTrainerAiAddValue();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x1;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r5 = (u32)sp + 0x10;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1A6C();
    r26 = r3;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r25 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x100;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r25 = (s32)r25 / (s32)r3;
    r3 = 0x0;
    r4 = r25;
    fightTrainerAiAddValue();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x100;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x101;
        fn_80239984();
        r0 = r3;
        r3 = r28;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x101;
        fn_80239EE8();
    }
    r25 = (u32)sp + 0x10;
    r26 = r26 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r26) break;
        r3 = r30;
        r4 = 0x0;
        r5 = 0xd5;
        r6 = 0x0;
        ((void(*)(void))pokemonGetStatus)();
        r4 = *(u32*)(r25 + r0);
        if (r3 != (u32)r4) {
            r3 = r27;
            r5 = 0x7;
            fn_802384B4();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r27;
                r4 = r29;
                fn_80239564();
                r25 = r3 & 0xFF;
                r3 = 0x0;
                r4 = 0x102;
                r5 = 0x3e;
                r6 = 0x0;
                fn_801FB1C0();
                r25 = (s32)r25 / (s32)r3;
                r3 = r31;
                r4 = r25;
                fightTrainerAiAddValue();
                r0 = r3;
                r3 = r28;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r27;
                r8 = r29;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x102;
                fn_80239CCC();
                break;
        }
        }
        r24 = r24 + 0x1;

    }

    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 != (u32)r26) {
        r3 = r27;
        r4 = r30;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r25 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x103;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r25 = (s32)r25 / (s32)r3;
    r3 = r31;
    r4 = r25;
    fightTrainerAiAddValue();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x103;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x80248B1C | Size: 0x220 (544 bytes) */
u32 fightTrainerAiWazaValueBureizukikku(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80237310(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0xfb);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xfb);
    handle = fn_80239984(handle, ctx, 0xfc);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xfc);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xfd);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xfd);
    }
    if ((u8)fn_80236BFC(ctx, param3, 7) == 1) {
        handle = fn_80239984(handle, ctx, 0xfe);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xfe);
    }
    if ((u8)fn_80237310(ctx, param3) == 0 || (u8)fn_80237F74(ctx, param3, 0x29) == 1 ||
        (u8)fn_80237F74(ctx, param3, 0x13) == 1) {
        handle = fn_80239984(handle, ctx, 0xff);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xff);
    }
    return handle;
}

/* Address: 0x80248D3C | Size: 0x288 (648 bytes) */
u32 fightTrainerAiWazaValueJikokaitou(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80237310(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    u8 numerator;
    s32 quotient;
    u32 handle;

    handle = 0;
    if ((u8)fn_80236BFC(ctx, param1, 7) == 1) {
        handle = fn_80239984(0, ctx, 0xf6);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf6);
    }
    quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xf7, 0x3e, 0);
    quotient = quotient / denom;
    handle = fightTrainerAiAddValue(handle, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf7, quotient);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xf8);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf8);
    }
    if ((u8)fn_80236BFC(ctx, param3, 7) == 1) {
        handle = fn_80239984(handle, ctx, 0xf9);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf9);
    }
    if ((u8)fn_80237310(ctx, param3) != 0) {
        if ((u8)fn_80237F74(ctx, param3, 0x29) != 1) {
            if ((u8)fn_80237F74(ctx, param3, 0x13) != 1) {
                goto done;
            }
        }
    }
    numerator = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xfa, 0x3e, 0);
    quotient = numerator / denom;
    handle = fightTrainerAiAddValue(handle, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xfa, quotient);
done:
    return handle;
}

/* Address: 0x80248FC4 | Size: 0x49C (1180 bytes) */
u32 fightTrainerAiWazaValueHonoonouzu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8A18(u32, u16*);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235BE4(void*, u32, u32, u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u16 tmp;
    u32 found;
    u32 handle;
    u32 pokemon;
    u32 status;

    handle = 0;
    status = fn_80235BE4(ctx, 0, param3, 0);
    pokemon = fn_801F4354(0, param3);
    tmp = 0;
    found = fn_801F8A18(pokemon, &tmp);
    if (found == 0) {
        status = 1;
    }
    if ((u8)status == 0) {
        handle = fn_80239984(0, ctx, 0xeb);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xeb);
    }
    if ((u8)fn_80236BFC(ctx, param3, 3) == 1) {
        handle = fn_80239984(handle, ctx, 0xec);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xec);
    }
    if ((u8)fn_80236BFC(ctx, param3, 4) == 1) {
        handle = fn_80239984(handle, ctx, 0xed);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xed);
    }
    if ((u8)fn_80236BFC(ctx, param3, 6) == 1) {
        handle = fn_80239984(handle, ctx, 0xee);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xee);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1c) == 1) {
        handle = fn_80239984(handle, ctx, 0xef);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xef);
    }
    if ((u8)fn_80236BFC(ctx, param3, 9) == 1) {
        handle = fn_80239984(handle, ctx, 0xf0);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf0);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0xa) == 1) {
        handle = fn_80239984(handle, ctx, 0xf1);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf1);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x18) == 1) {
        handle = fn_80239984(handle, ctx, 0xf2);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf2);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1e) == 1) {
        handle = fn_80239984(handle, ctx, 0xf3);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf3);
    }
    if ((u8)fn_80236BFC(ctx, param3, 7) == 1) {
        handle = fn_80239984(handle, ctx, 0xf4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf4);
    }
    if ((u8)status != 0) {
        handle = fn_80239984(handle, ctx, 0xf5);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xf5);
    }
    return handle;
}

/* Address: 0x80249460 | Size: 0x218 (536 bytes) */
u32 fightTrainerAiWazaValueIryokuHonoo(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80237310(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    s32 quotient;
    u32 handle;

    quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xe7, 0x3e, 0);
    quotient = quotient / denom;
    handle = fightTrainerAiAddValue(0, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe7, quotient);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xe8);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe8);
    }
    if ((u8)fn_80236BFC(ctx, param3, 7) == 1) {
        handle = fn_80239984(handle, ctx, 0xe9);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe9);
    }
    if ((u8)fn_80237310(ctx, param3) == 0 || (u8)fn_80237F74(ctx, param3, 0x29) == 1) {
        quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
        denom = fn_801FB1C0(0, 0xea, 0x3e, 0);
        quotient = quotient / denom;
        handle = fightTrainerAiAddValue(handle, quotient);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xea, quotient);
    }
    return handle;
}

/* Address: 0x80249678 | Size: 0x248 (584 bytes) */
u32 fightTrainerAiWazaValueTuikouKonran(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    u8 numerator;
    s32 quotient;
    s32 finalQuotient;
    u32 handle;

    quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xe3, 0x3e, 0);
    quotient = quotient / denom;
    handle = fightTrainerAiAddValue(0, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe3, quotient);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xe4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe4);
    }
    if ((u8)fn_80236BFC(ctx, param3, 9) == 1) {
        quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
        denom = fn_801FB1C0(0, 0xe5, 0x3e, 0);
        quotient = quotient / denom;
        handle = fightTrainerAiAddValue(handle, quotient);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe5, quotient);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x13) != 1) {
        if ((u8)fn_80237F74(ctx, param3, 0x14) != 1) {
            goto done;
        }
    }
    numerator = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xe6, 0x3e, 0);
    finalQuotient = numerator / denom;
    handle = fightTrainerAiAddValue(handle, finalQuotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe6, finalQuotient);
done:
    return handle;
}

/* Address: 0x802498C0 | Size: 0x1F4 (500 bytes) */
u32 fightTrainerAiWazaValueDorokake(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_802358AC(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0xdf);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xdf);
    if ((u8)fn_802358AC(ctx, param3) == 0) {
        handle = fn_80239984(handle, ctx, 0xe0);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe0);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x1d) == 1 || (u8)fn_80237F74(ctx, param3, 0x13) == 1 ||
        (u8)fn_80237F74(ctx, param3, 0x49) == 1 || (u8)fn_80237F74(ctx, param3, 0x33) == 1) {
        handle = fn_80239984(handle, ctx, 0xe1);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe1);
    }
    if ((u8)fn_802358AC(ctx, param3) <= 4) {
        handle = fn_80239984(handle, ctx, 0xe2);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xe2);
    }
    return handle;
}

/* Address: 0x80249AB4 | Size: 0x278 (632 bytes) */
u32 fightTrainerAiWazaValueTuikouMeityuuDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_802358AC(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    u8 numerator;
    s32 quotient;
    s32 finalQuotient;
    u32 handle;

    quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xdb, 0x3e, 0);
    quotient = quotient / denom;
    handle = fightTrainerAiAddValue(0, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xdb, quotient);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xdc);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xdc);
    }
    if ((u8)fn_802358AC(ctx, param3) == 0) {
        quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
        denom = fn_801FB1C0(0, 0xdd, 0x3e, 0);
        quotient = quotient / denom;
        handle = fightTrainerAiAddValue(handle, quotient);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xdd, quotient);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x1d) != 1) {
        if ((u8)fn_80237F74(ctx, param3, 0x13) != 1) {
            if ((u8)fn_80237F74(ctx, param3, 0x49) != 1) {
                if ((u8)fn_80237F74(ctx, param3, 0x33) != 1) {
                    goto done;
                }
            }
        }
    }
    numerator = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xde, 0x3e, 0);
    finalQuotient = numerator / denom;
    handle = fightTrainerAiAddValue(handle, finalQuotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xde, finalQuotient);
done:
    return handle;
}

/* Address: 0x80249D2C | Size: 0x25C (604 bytes) */
u32 fightTrainerAiWazaValueTuikouTokubouDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235974(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    u8 numerator;
    s32 quotient;
    s32 finalQuotient;
    u32 handle;

    quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xd7, 0x3e, 0);
    quotient = quotient / denom;
    handle = fightTrainerAiAddValue(0, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xd7, quotient);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xd8);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xd8);
    }
    if ((u8)fn_80235974(ctx, param3) == 0) {
        quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
        denom = fn_801FB1C0(0, 0xd9, 0x3e, 0);
        quotient = quotient / denom;
        handle = fightTrainerAiAddValue(handle, quotient);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xd9, quotient);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x1d) != 1) {
        if ((u8)fn_80237F74(ctx, param3, 0x13) != 1) {
            if ((u8)fn_80237F74(ctx, param3, 0x49) != 1) {
                goto done;
            }
        }
    }
    numerator = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xda, 0x3e, 0);
    finalQuotient = numerator / denom;
    handle = fightTrainerAiAddValue(handle, finalQuotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xda, finalQuotient);
done:
    return handle;
}

/* Address: 0x80249F88 | Size: 0x1E8 (488 bytes) */
u32 fightTrainerAiWazaValueTuikouTokukouDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_802359D8(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0xd3);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xd3);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xd4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xd4);
    }
    if ((u8)fn_802359D8(ctx, param3) == 0) {
        handle = fn_80239984(handle, ctx, 0xd5);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xd5);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x1d) != 1) {
        if ((u8)fn_80237F74(ctx, param3, 0x13) != 1) {
            if ((u8)fn_80237F74(ctx, param3, 0x49) != 1) {
                goto done;
            }
        }
    }
    handle = fn_80239984(handle, ctx, 0xd6);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xd6);
done:
    return handle;
}

/* Address: 0x8024A170 | Size: 0x2B8 (696 bytes) */
void fightTrainerAiWazaValueKogoerukaze(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80236D60();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x1;
    r24 = r3;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r4 = r24;
    r5 = (u32)sp + 0x28;
    r29 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r30 = r3;
    r4 = r24;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r31 = (u32)sp + 0x8;
    r23 = r3 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if (r0 >= (u32)r23) break;
        r19 = *(u32*)(r31 + r0);
        if (r19 != (u32)0x0) {
            r21 = (u32)sp + 0x28;
            r22 = r30 & 0xFFFF;
            r20 = 0x0;
            while (1) {
                r0 = r20 & 0xFFFF;
            if (r0 >= (u32)r22) break;
                r5 = *(u32*)(r21 + r0);
                if (r5 != (u32)0x0) {
                    r3 = r24;
                    r4 = r19;
                    fn_80236D60();
                    if ((s32)r3 > (s32)0x0) {
                        r3 = r29;
                        r4 = r24;
                        r5 = 0xcf;
                        fn_80239984();
                        r0 = r3;
                        r3 = r25;
                        r29 = r0;
                        fightOutPokemonGetPokemonPtr();
                        r6 = (0x1 << 16);
                        r5 = r3;
                        r4 = r24;
                        r8 = r26;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0xcf;
                        fn_80239EE8();
                        break;
                }
                }
                r20 = r20 + 0x1;

            }
        }
        r28 = r28 + 0x1;

    }
    if (r23 >= (u32)0x2) {
        r3 = r29;
        r4 = r24;
        r5 = 0xd0;
        fn_80239984();
        r0 = r3;
        r3 = r25;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r24;
        r8 = r26;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xd0;
        fn_80239EE8();
    }
    r31 = (u32)sp + 0x28;
    r28 = r30 & 0xFFFF;
    r21 = 0x0;
    while (1) {
        r0 = r21 & 0xFFFF;
        if (r0 >= (u32)r28) break;
        r5 = *(u32*)(r31 + r0);
        if (r5 != (u32)0x0) {
            r3 = r24;
            r4 = r27;
            fn_80236D60();
            if ((s32)r3 < (s32)0x0) break;
        }
        r21 = r21 + 0x1;

    }

    r3 = r21 & 0xFFFF;
    r0 = r30 & 0xFFFF;
    if (r3 < r0) {
        r3 = r29;
        r4 = r24;
        r5 = 0xd1;
        fn_80239984();
        r0 = r3;
        r3 = r25;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r24;
        r8 = r26;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xd1;
        fn_80239EE8();
    }
    r3 = r24;
    r4 = r27;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r24;
        r4 = r27;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r24;
            r4 = r27;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) { r3 = r29; return; }
    }
    }
    r3 = r29;
    r4 = r24;
    r5 = 0xd2;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd2;
    fn_80239EE8();

    r3 = r29;
    return;
}

/* Address: 0x8024A428 | Size: 0x23C (572 bytes) */
void fightTrainerAiWazaValueKanarazuSubayasaDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80235910();
    extern void fn_80236D60();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x1;
    r24 = r3;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r4 = r24;
    r5 = (u32)sp + 0x28;
    r29 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r30 = r3;
    r4 = r24;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r31 = (u32)sp + 0x8;
    r23 = r3 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if (r0 >= (u32)r23) break;
        r19 = *(u32*)(r31 + r0);
        if (r19 != (u32)0x0) {
            r21 = (u32)sp + 0x28;
            r22 = r30 & 0xFFFF;
            r18 = 0x0;
            r20 = 0x0;
            while (1) {
                r0 = r20 & 0xFFFF;
                if (r0 >= (u32)r22) break;
                r5 = *(u32*)(r21 + r0);
                if (r5 != (u32)0x0) {
                    r3 = r24;
                    r4 = r19;
                    fn_80236D60();
                    if ((s32)r3 > (s32)0x0) {
                        r3 = r29;
                        r4 = r24;
                        r5 = 0xcc;
                        fn_80239984();
                        r0 = r3;
                        r3 = r25;
                        r29 = r0;
                        fightOutPokemonGetPokemonPtr();
                        r6 = (0x1 << 16);
                        r5 = r3;
                        r4 = r24;
                        r8 = r26;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0xcc;
                        fn_80239EE8();
                        r18 = 0x1;
                        break;
                }
                }
                r20 = r20 + 0x1;

            }

            r0 = r18 & 0xFF;
            if (r0 == (u32)0x1) break;
        }
        r28 = r28 + 0x1;

    }

    r3 = r24;
    r4 = r27;
    fn_80235910();
    r0 = r3 & 0xFF;
    if (r0 == (u32)r23) {
        r3 = r29;
        r4 = r24;
        r5 = 0xcd;
        fn_80239984();
        r0 = r3;
        r3 = r25;
        r29 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r24;
        r8 = r26;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xcd;
        fn_80239EE8();
    }
    r3 = r24;
    r4 = r27;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r24;
        r4 = r27;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r24;
            r4 = r27;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) { r3 = r29; return; }
    }
    }
    r3 = r29;
    r4 = r24;
    r5 = 0xce;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xce;
    fn_80239EE8();

    r3 = r29;
    return;
}

/* Address: 0x8024A664 | Size: 0x2C0 (704 bytes) */
u32 fightTrainerAiWazaValueTuikouSubayasaDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235910(void*, u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    u8 numerator;
    s32 quotient;
    s32 finalQuotient;
    u32 handle;

    quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xc7, 0x3e, 0);
    quotient = quotient / denom;
    handle = fightTrainerAiAddValue(0, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc7, quotient);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xc8);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc8);
    }
    if ((u8)fn_80236BFC(ctx, param3, 5) == 1) {
        handle = fn_80239984(handle, ctx, 0xc9);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc9);
    }
    if ((u8)fn_80235910(ctx, param3) == 0) {
        quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
        denom = fn_801FB1C0(0, 0xca, 0x3e, 0);
        quotient = quotient / denom;
        handle = fightTrainerAiAddValue(handle, quotient);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xca, quotient);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x1d) != 1) {
        if ((u8)fn_80237F74(ctx, param3, 0x13) != 1) {
            if ((u8)fn_80237F74(ctx, param3, 0x49) != 1) {
                goto done;
            }
        }
    }
    numerator = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xcb, 0x3e, 0);
    finalQuotient = numerator / denom;
    handle = fightTrainerAiAddValue(handle, finalQuotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xcb, finalQuotient);
done:
    return handle;
}

/* Address: 0x8024A924 | Size: 0x25C (604 bytes) */
u32 fightTrainerAiWazaValueTuikouBougyoDaun(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235A3C(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    u8 numerator;
    s32 quotient;
    s32 finalQuotient;
    u32 handle;

    quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xc3, 0x3e, 0);
    quotient = quotient / denom;
    handle = fightTrainerAiAddValue(0, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc3, quotient);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xc4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc4);
    }
    if ((u8)fn_80235A3C(ctx, param3) == 0) {
        quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
        denom = fn_801FB1C0(0, 0xc5, 0x3e, 0);
        quotient = quotient / denom;
        handle = fightTrainerAiAddValue(handle, quotient);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc5, quotient);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x1d) != 1) {
        if ((u8)fn_80237F74(ctx, param3, 0x13) != 1) {
            if ((u8)fn_80237F74(ctx, param3, 0x49) != 1) {
                goto done;
            }
        }
    }
    numerator = (s32)(fn_80239564(ctx, param2) & 0xff);
    denom = fn_801FB1C0(0, 0xc6, 0x3e, 0);
    finalQuotient = numerator / denom;
    handle = fightTrainerAiAddValue(handle, finalQuotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc6, finalQuotient);
done:
    return handle;
}

/* Address: 0x8024AB80 | Size: 0x204 (516 bytes) */
u32 fightTrainerAiWazaValueOororabiimu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235AA0(void*, u32);
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0xbf);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xbf);
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xc0);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc0);
    }
    if ((u8)fn_80235AA0(ctx, param3) == 0) {
        handle = fn_80239984(handle, ctx, 0xc1);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc1);
    }
    if ((u8)fn_80237F74(ctx, param3, 0x1d) == 1 || (u8)fn_80237F74(ctx, param3, 0x13) == 1 ||
        (u8)fn_80237F74(ctx, param3, 0x49) == 1 || (u8)fn_80237F74(ctx, param3, 0x34) == 1) {
        handle = fn_80239984(handle, ctx, 0xc2);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xc2);
    }
    return handle;
}

/* Address: 0x8024AD84 | Size: 0x16C (364 bytes) */
u32 fightTrainerAiWazaValueNekodamasi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if ((pokemonGetStatus(param1, 0, 0xed, 0) & 0xffff) != 0) {
        handle = fn_80239984(0, ctx, 0xbd);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xbd);
    }
    if (fn_80237F74(ctx, param3, 0x27) == 1) {
        handle = fn_80239984(handle, ctx, 0xbe);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xbe);
        goto done;
    }
    if (fn_80237F74(ctx, param3, 0x13) == 1) {
        handle = fn_80239984(handle, ctx, 0xbe);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xbe);
    }
done:
    return handle;
}

/* Address: 0x8024AEF0 | Size: 0xD4 (212 bytes) */
u32 fightTrainerAiWazaValueGoddobaado(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern s32 fn_80236D60(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if (fn_80236D60(ctx, param1, param3) > 0) {
        handle = fn_80239984(0, ctx, 0xbb);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xbb);
    }
    handle = fn_80239984(handle, ctx, 0xbc);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xbc);
    return handle;
}

/* Address: 0x8024AFC4 | Size: 0x4B0 (1200 bytes) */
u32 fightTrainerAiWazaValueTuikaHirumi(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern s32 fn_80236D60(void*, u32, u32);
    extern u32 fn_80239564(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    s32 denom;
    s32 quotient;
    u32 handle;

    handle = 0;
    if (fn_80236D60(ctx, param1, param3) > 0) {
        quotient = (s32)(fn_80239564(ctx, param2) & 0xff);
        denom = fn_801FB1C0(0, 0xb0, 0x3e, 0);
        quotient = quotient / denom;
        handle = fightTrainerAiAddValue(0, quotient);
        fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb0, quotient);
        if ((u8)fn_80236BFC(ctx, param3, 3) == 1) {
            handle = fn_80239984(handle, ctx, 0xb1);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb1);
        }
        if ((u8)fn_80236BFC(ctx, param3, 4) == 1) {
            handle = fn_80239984(handle, ctx, 0xb2);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb2);
        }
        if ((u8)fn_80236BFC(ctx, param3, 6) == 1) {
            handle = fn_80239984(handle, ctx, 0xb3);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb3);
        }
        if ((u8)fn_80236BFC(ctx, param3, 0x1c) == 1) {
            handle = fn_80239984(handle, ctx, 0xb4);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb4);
        }
        if ((u8)fn_80236BFC(ctx, param3, 0x18) == 1) {
            handle = fn_80239984(handle, ctx, 0xb5);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb5);
        }
        if ((u8)fn_80236BFC(ctx, param3, 5) == 1) {
            handle = fn_80239984(handle, ctx, 0xb6);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb6);
        }
        if ((u8)fn_80236BFC(ctx, param3, 9) == 1) {
            handle = fn_80239984(handle, ctx, 0xb7);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb7);
        }
        if ((u8)fn_80236BFC(ctx, param3, 0xa) == 1) {
            handle = fn_80239984(handle, ctx, 0xb8);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb8);
        }
        if ((u8)fn_80236BFC(ctx, param3, 0x1e) == 1) {
            handle = fn_80239984(handle, ctx, 0xb9);
            fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xb9);
        }
    }
    if ((u8)fn_801F1990(0, ctx, 1, 1, 0x10e, param1) == 1) {
        handle = fn_80239984(handle, ctx, 0xba);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xba);
    }
    return handle;
}

/* Address: 0x8024B474 | Size: 0x5D0 (1488 bytes) */
void fightTrainerAiWazaValueJisin(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80236520();
    extern void fn_802367CC();
    extern void fn_802376EC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fn_8023C370();
    u8 sp[0xF0];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r4;
    r28 = r5;
    r30 = r3;
    r31 = 0x0;
    r5 = r29;
    r4 = r28;
    fn_802395C8();
    r24 = r3;
    r4 = r30;
    r5 = (u32)sp + 0x5c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r4 = r30;
    r5 = (u32)sp + 0x3c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r27 = r3;
    r4 = r30;
    r5 = (u32)sp + 0x1c;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r25 = (u32)sp + 0x3c;
    r22 = r27 & 0xFFFF;
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFFFF;
        if (r0 >= (u32)r22) break;
        r0 = *(u32*)(r25 + r23);
        if (r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r25 + r23);
            r6 = r3;
            r3 = r30;
            r5 = r24;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if (r0 == (u32)0x43) {
                r3 = r31;
                r4 = r30;
                r5 = 0xa9;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0xa9;
                fn_80239EE8();
        }
        }
        r26 = r26 + 0x1;

    }
    r25 = (u32)sp + 0x3c;
    r22 = r27 & 0xFFFF;
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFFFF;
        if (r0 >= (u32)r22) break;
        r0 = *(u32*)(r25 + r23);
        if (r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r25 + r23);
            r6 = r3;
            r3 = r30;
            r5 = r24;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if (r0 == (u32)0x42) {
                r3 = r31;
                r4 = r30;
                r5 = 0xaa;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0xaa;
                fn_80239EE8();
        }
        }
        r26 = r26 + 0x1;

    }
    r25 = (u32)sp + 0x3c;
    r22 = r27 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r22) break;
        r4 = *(u32*)(r25 + r0);
        if (r29 != (u32)r4) {
            r3 = r30;
            r5 = (u32)sp + 0x8;
            r6 = 0x0;
            r7 = 0x1;
            fn_802367CC();
            r4 = r3 & 0xFFFF;
            r26 = r3;
            if (r29 != (u32)r4) {
                r3 = (u32)sp + 0x8;
                r23 = 0x0;
                while (1) {
                    r0 = r23 & 0xFFFF;
                    if (r0 >= (u32)r4) break;
                    r0 = *(u16*)(r3 + r0);
                    if (r0 != (u32)0xb6 && r0 != (u32)0xc5) {

                        if (r0 == (u32)0xcb) {
                        }
                        r3 = r31;
                        r4 = r30;
                        r5 = 0xab;
                        fn_80239984();
                        r0 = r3;
                        r3 = r29;
                        r31 = r0;
                        fightOutPokemonGetPokemonPtr();
                        r6 = (0x1 << 16);
                        r5 = r3;
                        r4 = r30;
                        r8 = r28;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0xab;
                        fn_80239EE8();
                        break;
                        }
                    r23 = r23 + 0x1;

                }

                r3 = r23 & 0xFFFF;
                r0 = r26 & 0xFFFF;
                if (r3 < r0) break;
        }
        }
        r24 = r24 + 0x1;

    }

    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xb6;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        r5 = 0xac;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r30;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xac;
        fn_80239EE8();

    } else {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x1;
        r6 = 0x1;
        r7 = 0xc5;
        r8 = 0x0;
        fn_801F1990();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = r31;
            r4 = r30;
            r5 = 0xac;
            fn_80239984();
            r0 = r3;
            r3 = r29;
            r31 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r30;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xac;
            fn_80239EE8();
            goto L_8024B878;
        }
        r4 = r30;
        r3 = 0x0;
        r5 = 0x1;
        r6 = 0x1;
        r7 = 0xcb;
        r8 = 0x0;
        fn_801F1990();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = r31;
            r4 = r30;
            r5 = 0xac;
            fn_80239984();
            r0 = r3;
            r3 = r29;
            r31 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r30;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xac;
            fn_80239EE8();
        }
    }
L_8024B878:
    r4 = r30;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        r5 = 0xad;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r30;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xad;
        fn_80239EE8();
    }
    r24 = (u32)sp + 0x3c;
    r23 = r27 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)r23) break;
        r4 = *(u32*)(r24 + r0);
        if (r29 != (u32)r4) {
            r3 = r30;
            fn_80236520();
            r0 = r3 & 0xFFFF;
            if (r0 != (u32)0xb6 && r0 != (u32)0xc5) {

                if (r0 == (u32)0xcb) {
                }
                r3 = r31;
                r4 = r30;
                r5 = 0xae;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0xae;
                fn_80239EE8();
            }
                }
        r22 = r22 + 0x1;

    }
    r22 = (u32)sp + 0x3c;
    r23 = r27 & 0xFFFF;
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if (r0 >= (u32)r23) break;
        r4 = *(u32*)(r22 + r24);
        if (r29 != (u32)r4) {
            r3 = r30;
            fn_802376EC();
            r6 = *(u32*)(r22 + r24);
            r24 = r3;
            r3 = r30;
            r4 = r29;
            r5 = r28;
            r7 = 0x0;
            fn_8023C370();
            r0 = r24 & 0xFFFF;
            if ((s32)r0 <= (s32)r3) {
                r3 = r31;
                r4 = r30;
                r5 = 0xaf;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0xaf;
                fn_80239EE8();
        }
        }
        r25 = r25 + 0x1;

    }
    r3 = r31;
    return;
}

/* Address: 0x8024BA44 | Size: 0x438 (1080 bytes) */
u32 fightTrainerAiWazaValueSokubaku(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F8A18(u32, u16*);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80235BE4(void*, u32, u32, u32);
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u16 tmp;
    u32 found;
    u32 handle;
    u32 pokemon;
    u32 status;

    handle = 0;
    status = fn_80235BE4(ctx, 0, param3, 0);
    pokemon = fn_801F4354(0, param3);
    tmp = 0;
    found = fn_801F8A18(pokemon, &tmp);
    if (found == 0) {
        status = 1;
    }
    if ((u8)status == 0) {
        handle = fn_80239984(0, ctx, 0x9f);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x9f);
    }
    if ((u8)fn_80236BFC(ctx, param3, 3) == 1) {
        handle = fn_80239984(handle, ctx, 0xa0);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa0);
    }
    if ((u8)fn_80236BFC(ctx, param3, 4) == 1) {
        handle = fn_80239984(handle, ctx, 0xa1);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa1);
    }
    if ((u8)fn_80236BFC(ctx, param3, 6) == 1) {
        handle = fn_80239984(handle, ctx, 0xa2);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa2);
    }
    if ((u8)fn_80236BFC(ctx, param3, 9) == 1) {
        handle = fn_80239984(handle, ctx, 0xa4);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa4);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0xa) == 1) {
        handle = fn_80239984(handle, ctx, 0xa5);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa5);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x18) == 1) {
        handle = fn_80239984(handle, ctx, 0xa6);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa6);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1e) == 1) {
        handle = fn_80239984(handle, ctx, 0xa7);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa7);
    }
    if ((u8)fn_80236BFC(ctx, param3, 0x1c) == 1) {
        handle = fn_80239984(handle, ctx, 0xa3);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa3);
    }
    if ((u8)status != 0) {
        handle = fn_80239984(handle, ctx, 0xa8);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0xa8);
    }
    return handle;
}

/* Address: 0x8024BE7C | Size: 0x144 (324 bytes) */
u32 fightTrainerAiWazaValueKorogaru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern s32 fn_802387C8(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;
    s32 value;
    s32 quotient;

    handle = 0;
    if (fn_80236BFC(ctx, param1, 0x1a) == 1) {
        handle = fn_80239984(0, ctx, 0x9c);
        fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x9c);
    }
    value = fn_802387C8(ctx, param1);
    quotient = value / fn_801FB1C0(0, 0x9d, 0x3e, 0);
    handle = fightTrainerAiAddValue(handle, quotient);
    fn_80239CCC(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x9d, quotient);
    handle = fn_80239984(handle, ctx, 0x9e);
    fn_80239EE8(0xec64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x9e);
    return handle;
}

/* Address: 0x8024BFC0 | Size: 0x5FC (1532 bytes) */
void fightTrainerAiWazaValueJibaku(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_80236520();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_802376EC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fn_8023C370();
    u8 sp[0xF0];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r4;
    r28 = r5;
    r30 = r3;
    r31 = 0x0;
    r5 = r29;
    r4 = r28;
    fn_802395C8();
    r25 = r3;
    r4 = r30;
    r5 = (u32)sp + 0x5c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r4 = r30;
    r5 = (u32)sp + 0x3c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r27 = r3;
    r4 = r30;
    r5 = (u32)sp + 0x1c;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r26 = r3;
    r24 = (u32)sp + 0x3c;
    r21 = r27 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r21) break;
        r0 = *(u32*)(r24 + r22);
        if (r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r24 + r22);
            r6 = r3;
            r3 = r30;
            r5 = r25;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if (r0 == (u32)0x43) {
                r3 = r31;
                r4 = r30;
                r5 = 0x94;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x94;
                fn_80239EE8();
        }
        }
        r23 = r23 + 0x1;

    }
    r24 = (u32)sp + 0x3c;
    r21 = r27 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r21) break;
        r0 = *(u32*)(r24 + r22);
        if (r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r24 + r22);
            r6 = r3;
            r3 = r30;
            r5 = r25;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if (r0 == (u32)0x42) {
                r3 = r31;
                r4 = r30;
                r5 = 0x95;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x95;
                fn_80239EE8();
        }
        }
        r23 = r23 + 0x1;

    }
    r24 = (u32)sp + 0x3c;
    r21 = r27 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r21) break;
        r4 = *(u32*)(r24 + r0);
        if (r29 != (u32)r4) {
            r3 = r30;
            r5 = (u32)sp + 0x8;
            r6 = 0x0;
            r7 = 0x1;
            fn_802367CC();
            r4 = r3 & 0xFFFF;
            r25 = r3;
            if (r29 != (u32)r4) {
                r3 = (u32)sp + 0x8;
                r22 = 0x0;
                while (1) {
                    r0 = r22 & 0xFFFF;
                    if (r0 >= (u32)r4) break;
                    r0 = *(u16*)(r3 + r0);
                    if (r0 != (u32)0xb6 && r0 != (u32)0xc5) {

                        if (r0 == (u32)0xcb) {
                        }
                        r3 = r31;
                        r4 = r30;
                        r5 = 0x96;
                        fn_80239984();
                        r0 = r3;
                        r3 = r29;
                        r31 = r0;
                        fightOutPokemonGetPokemonPtr();
                        r6 = (0x1 << 16);
                        r5 = r3;
                        r4 = r30;
                        r8 = r28;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0x96;
                        fn_80239EE8();
                        break;
                        }
                    r22 = r22 + 0x1;

                }

                r3 = r22 & 0xFFFF;
                r0 = r25 & 0xFFFF;
                if (r3 < r0) break;
        }
        }
        r23 = r23 + 0x1;

    }

    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xb6;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        r5 = 0x97;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r31 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r30;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x97;
        fn_80239EE8();

    } else {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x1;
        r6 = 0x1;
        r7 = 0xc5;
        r8 = 0x0;
        fn_801F1990();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = r31;
            r4 = r30;
            r5 = 0x97;
            fn_80239984();
            r0 = r3;
            r3 = r29;
            r31 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r30;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x97;
            fn_80239EE8();
            goto L_8024C3C8;
        }
        r4 = r30;
        r3 = 0x0;
        r5 = 0x1;
        r6 = 0x1;
        r7 = 0xcb;
        r8 = 0x0;
        fn_801F1990();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = r31;
            r4 = r30;
            r5 = 0x97;
            fn_80239984();
            r0 = r3;
            r3 = r29;
            r31 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r30;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x97;
            fn_80239EE8();
        }
    }
L_8024C3C8:
    r25 = (u32)sp + 0x1c;
    r22 = r26 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r22) break;
        r3 = r30;
        r4 = *(u32*)(r25 + r0);
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r24 & 0xFFFF;
            r3 = r31;
            r4 = r30;
            r5 = r0 - r0; /* -borrow */;
            r23 = r5 + 0x99;
            r5 = r23;
            fn_80239984();
            r0 = r3;
            r3 = r29;
            r31 = r0;
            fightOutPokemonGetPokemonPtr();
            r6 = (0x1 << 16);
            r5 = r3;
            r4 = r30;
            r8 = r28;
            r10 = r23;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            fn_80239EE8();
        }
        r24 = r24 + 0x1;

    }
    r23 = (u32)sp + 0x3c;
    r22 = r27 & 0xFFFF;
    r21 = 0x0;
    while (1) {
        r0 = r21 & 0xFFFF;
        if (r0 >= (u32)r22) break;
        r4 = *(u32*)(r23 + r0);
        if (r29 != (u32)r4) {
            r3 = r30;
            fn_80236520();
            r0 = r3 & 0xFFFF;
            if (r0 != (u32)0xb6 && r0 != (u32)0xc5) {

                if (r0 == (u32)0xcb) {
                }
                r3 = r31;
                r4 = r30;
                r5 = 0x9a;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x9a;
                fn_80239EE8();
            }
                }
        r21 = r21 + 0x1;

    }
    r21 = (u32)sp + 0x3c;
    r22 = r27 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r22) break;
        r4 = *(u32*)(r21 + r23);
        if (r29 != (u32)r4) {
            r3 = r30;
            fn_802376EC();
            r6 = *(u32*)(r21 + r23);
            r23 = r3;
            r3 = r30;
            r4 = r29;
            r5 = r28;
            r7 = 0x0;
            fn_8023C370();
            r0 = r23 & 0xFFFF;
            if ((s32)r0 <= (s32)r3) {
                r3 = r31;
                r4 = r30;
                r5 = 0x9b;
                fn_80239984();
                r0 = r3;
                r3 = r29;
                r31 = r0;
                fightOutPokemonGetPokemonPtr();
                r6 = (0x1 << 16);
                r5 = r3;
                r4 = r30;
                r8 = r28;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x9b;
                fn_80239EE8();
        }
        }
        r24 = r24 + 0x1;

    }
    r3 = r31;
    return;
}

/* Address: 0x8024C5BC | Size: 0x91C (2332 bytes) */
void fightTrainerAiWazaValueTobihaneru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_80236D60();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern s32 fightTrainerAiCheckJoutaiKieWazaHitWazaDataId();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x1;
    r31 = r3;
    r30 = r4;
    r29 = r5;
    r27 = r6;
    r4 = r31;
    r5 = (u32)sp + 0x1c;
    r26 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r28 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r4 = r31;
        r3 = 0x0;
        r5 = 0x7e;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x7e;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x7f;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x7f;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x80;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x80;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x81;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x81;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x82;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x82;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x83;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x83;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x84;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x84;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x85;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x85;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x86;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x86;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = r27;
    fn_80236D60();
    if ((s32)r3 > (s32)0x0) {
        r3 = r26;
        r4 = r31;
        r5 = 0x87;
        fn_80239984();
        r0 = r3;
        r3 = r30;
        r26 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x87;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r31;
    r5 = 0x88;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fightOutPokemonGetPokemonPtr();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x88;
    fn_80239EE8();
    r3 = r31;
    r4 = r30;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x89;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x89;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8a;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x8a;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8b;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x8b;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8c;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x8c;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8d;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x8d;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8e;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x8e;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8f;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x8f;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x90;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x90;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x91;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x91;
        fn_80239EE8();
    }
    r26 = (u32)sp + 0x1c;
    r25 = r28 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if (r0 >= (u32)r25) break;
        r3 = r31;
        r4 = *(u32*)(r26 + r0);
        r5 = (u32)sp + 0x8;
        r6 = 0x0;
        r7 = 0x0;
        fn_802367CC();
        r23 = r3 & 0xFFFF;
        if (r0 != (u32)0x1) {
            r24 = (u32)sp + 0x8;
            r21 = 0x0;
            r22 = 0x0;
            while (1) {
                r0 = r22 & 0xFFFF;
                if (r0 >= (u32)r23) break;
                r3 = r31;
                r5 = *(u16*)(r24 + r0);
                r4 = 0x1f;
                fightTrainerAiCheckJoutaiKieWazaHitWazaDataId();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r3 = r27;
                    r4 = r31;
                    r5 = 0x92;
                    fn_80239984();
                    r27 = r3;
                    r3 = r30;
                    fightOutPokemonGetPokemonPtr();
                    r6 = (0x1 << 16);
                    r5 = r3;
                    r4 = r31;
                    r8 = r29;
                    r6 = 0x0;
                    r7 = 0x0;
                    r9 = 0x0;
                    r10 = 0x92;
                    fn_80239EE8();
                    r21 = 0x1;
                    break;
                }
                r22 = r22 + 0x1;

            }

            r0 = r21 & 0xFF;
            if (r0 == (u32)0x1) break;
        }
        r28 = r28 + 0x1;

    }

    r3 = r31;
    r4 = r30;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x93;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x93;
        fn_80239EE8();
    }
    r3 = r27;
    return;
}

/* Address: 0x8024CED8 | Size: 0x940 (2368 bytes) */
void fightTrainerAiWazaValueSorawotobu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_80236D60();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern s32 fightTrainerAiCheckJoutaiKieWazaHitWazaDataId();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x1;
    r31 = r3;
    r29 = r4;
    r28 = r5;
    r25 = r6;
    r4 = r31;
    r5 = (u32)sp + 0x1c;
    r30 = 0x0;
    r27 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r26 = r3;
    r3 = r31;
    r4 = r25;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r4 = r31;
        r3 = 0x0;
        r5 = 0x69;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x69;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x6a;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x6a;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x6b;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x6b;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x6c;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x6c;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x6d;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x6d;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x6e;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x6e;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x6f;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x6f;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x70;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x70;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r25;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x71;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x71;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = r25;
    fn_80236D60();
    if ((s32)r3 > (s32)0x0) {
        r3 = r30;
        r4 = r31;
        r5 = 0x72;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x72;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x73;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x73;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x74;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x74;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x75;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x75;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x76;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x76;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x77;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x77;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x78;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x78;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x79;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x79;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x7a;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x7a;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x7b;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x7b;
        fn_80239EE8();
    }
    r0 = r28 & 0xFFFF;

    if (r0 == (u32)0x13 || r0 == (u32)0x154) {

        r27 = 0x1f;

    } else {
        if (r0 == (u32)0x5b) {
            r27 = 0x20;
            goto L_8024D6BC;
        }
        if (r0 == (u32)0x123) {
            r27 = 0x21;
        }
    }
L_8024D6BC:
    r25 = (u32)sp + 0x1c;
    r24 = r26 & 0xFFFF;
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFFFF;
        if (r0 >= (u32)r24) break;
        r3 = r31;
        r4 = *(u32*)(r25 + r0);
        r5 = (u32)sp + 0x8;
        r6 = 0x0;
        r7 = 0x0;
        fn_802367CC();
        r22 = r3 & 0xFFFF;
        if (r0 != (u32)0x123) {
            r23 = (u32)sp + 0x8;
            r20 = 0x0;
            r21 = 0x0;
            while (1) {
                r0 = r21 & 0xFFFF;
                if (r0 >= (u32)r22) break;
                r3 = r31;
                r5 = *(u16*)(r23 + r0);
                r4 = r27;
                fightTrainerAiCheckJoutaiKieWazaHitWazaDataId();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r3 = r30;
                    r4 = r31;
                    r5 = 0x7c;
                    fn_80239984();
                    r0 = r3;
                    r3 = r29;
                    r30 = r0;
                    fightOutPokemonGetPokemonPtr();
                    r6 = (0x1 << 16);
                    r5 = r3;
                    r4 = r31;
                    r8 = r28;
                    r6 = 0x0;
                    r7 = 0x0;
                    r9 = 0x0;
                    r10 = 0x7c;
                    fn_80239EE8();
                    r20 = 0x1;
                    break;
                }
                r21 = r21 + 0x1;

            }

            r0 = r20 & 0xFF;
            if (r0 == (u32)0x1) break;
        }
        r26 = r26 + 0x1;

    }

    r3 = r31;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x7d;
        fn_80239984();
        r0 = r3;
        r3 = r29;
        r30 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x7d;
        fn_80239EE8();
    }
    r3 = r30;
    return;
}

/* -------------------------------------------------------------------
 * Experience & Level Processing (0x8024D000-0x80254000)
 * 136 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8024D818 | Size: 0x140 (320 bytes) */
u32 fightTrainerAiWazaValueAbareru(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80237F74();
    extern u16 fn_8023831C();
    extern s32 fn_80239984();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    s32 handle = 0;
    u16 v;

    if (fn_80237F74(ctx, param1, 0x14) == 1) {
        handle = fn_80239984(0, ctx, 0x66);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x66);
    }
    v = fn_8023831C(ctx, param1);
    if (v == 8 || v == 9) {
        handle = fn_80239984(handle, ctx, 0x67);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x67);
    }
    handle = fn_80239984(handle, ctx, 0x68);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x68);
    return handle;
}

/* Address: 0x8024D958 | Size: 0x1A4 (420 bytes) */
u32 fightTrainerAiWazaValueKyuusyuu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern s32 fn_801FB1C0(u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_802376EC();
    extern u32 fn_80237F74(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern u32 fightTrainerAiAddValue(u32, s32);
    extern void fn_80239CCC(u32, void*, u32, u32, u32, u32, u32, u32, s32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    extern s32 fn_8023C370(void*, u32, u32, u32, u32);
    u32 handle;
    s32 score;
    s32 scale;

    handle = fn_802376EC();
    score = fn_8023C370(ctx, param1, param2, param3, 1);
    scale = fn_801FB1C0(0, 0x63, 0x3e, 0);
    score = (((score / 2) * 100) / (u16)handle) / scale;
    {
        u32 nextHandle = fightTrainerAiAddValue(0, score);
        handle = nextHandle;
    }
    fn_80239CCC(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x63, score);
    if ((fn_801F1990(0, ctx, 1, 1, 0x10e, param1) & 0xFF) == 1) {
        {
            u32 nextHandle = fn_80239984(handle, ctx, 0x64);
            handle = nextHandle;
        }
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x64);
    }
    if ((fn_80237F74(ctx, param3, 0x40) & 0xFF) == 1) {
        {
            u32 nextHandle = fn_80239984(handle, ctx, 0x65);
            handle = nextHandle;
        }
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(param1), 0, 0, param2, 0, 0x65);
    }
    return handle;
}

/* Address: 0x8024DAFC | Size: 0xC0 */
u32 fightTrainerAiWazaValueRokettoZutuki(void* ctx, u32 slot, u32 param) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x61);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x61);
    handle = fn_80239984(handle, ctx, 0x62);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x62);
    return handle;
}

/* Address: 0x8024DBBC | Size: 0xC0 */
u32 fightTrainerAiWazaValueKamaitati(void* ctx, u32 slot, u32 param) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x5f);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x5f);
    handle = fn_80239984(handle, ctx, 0x60);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x60);
    return handle;
}

/* Address: 0x8024DC7C | Size: 0x210 (528 bytes) */
u32 fightTrainerAiWazaValueKiaipanti(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightFloorGetFightTrainerFightOutPokemonPtrAry(u32, void*, u32*, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236520(void*, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 entries[8];
    void* battleCtx;
    u32 trainer;
    u32 sequenceArg;
    u32 handle;
    u32 rawCount;
    u32* entriesPtr;
    u16 count;
    u32 index;

    battleCtx = ctx;
    trainer = param1;
    sequenceArg = param2;
    handle = 0;
    rawCount = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, battleCtx, entries, 0, 1);
    if ((fn_80236520(battleCtx, trainer) & 0xFFFF) == 0x117) {
        handle = fn_80239984(0, battleCtx, 0x5B);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x5B);
    }

    entriesPtr = entries;
    count = rawCount;
    index = 0;
    while ((u16)index < count) {
        if (fn_80236BFC(battleCtx, entriesPtr[(u16)index], 8) == 1) {
            handle = fn_80239984(handle, battleCtx, 0x5C);
            fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x5C);
            break;
        }
        index++;
    }

    entriesPtr = entries;
    rawCount = rawCount & 0xFFFF;
    index = 0;
    while ((u16)index < rawCount) {
        if (fn_80236BFC(battleCtx, entriesPtr[(u16)index], 7) == 1) {
            handle = fn_80239984(handle, battleCtx, 0x5D);
            fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x5D);
            break;
        }
        index++;
    }

    handle = fn_80239984(handle, battleCtx, 0x5E);
    fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x5E);
    return handle;
}

/* Address: 0x8024DE8C | Size: 0x138 (312 bytes) */
u32 fightTrainerAiWazaValueAteminage(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_802357CC(void*, u32);
    extern u8 fn_802358AC(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    void* battleCtx = ctx;
    u32 trainer = param1;
    u32 sequenceArg = param2;
    u32 handle = 0;

    if (fn_802357CC(battleCtx, param3) > 6U) {
        handle = fn_80239984(0, battleCtx, 0x58);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x58);
    }
    if (fn_802358AC(battleCtx, trainer) < 6U) {
        handle = fn_80239984(handle, battleCtx, 0x59);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x59);
    }
    handle = fn_80239984(handle, battleCtx, 0x5a);
    fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x5a);
    return handle;
}

/* Address: 0x8024DFC4 | Size: 0x108 (264 bytes) */
u32 fightTrainerAiWazaValueRibenji(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80236520(void*, u32);
    extern u8 fn_8023943C(void*, u32, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    void* battleCtx = ctx;
    u32 trainer = param1;
    u32 sequenceArg = param2;
    u32 handle = 0;
    u32 move = fn_80236520(battleCtx, param3);

    if ((move & 0xFFFF) != 0 && (move & 0xFFFF) != 0xFFFF && (move & 0xFFFF) != 0x165 &&
        (move & 0xFFFF) != 0x163) {
        if (fn_8023943C(battleCtx, move, 1) == 1) {
            handle = fn_80239984(0, battleCtx, 0x56);
            fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x56);
        }
    }
    handle = fn_80239984(handle, battleCtx, 0x57);
    fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x57);
    return handle;
}

/* Address: 0x8024E0CC | Size: 0x7C | Pattern: field_accessor */
u32 fightTrainerAiWazaValueTokubetuYuusen(void* ctx, u32 slot, u32 param) {
    extern s32 fn_80239984();
    extern u32 fightOutPokemonGetPokemonPtr();
    extern void fn_80239EE8();
    u32 handle;

    handle = fn_80239984(0, ctx, 0x55);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x55);
    return handle;
}

/* Address: 0x8024E148 | Size: 0xEC (236 bytes) */
u32 fightTrainerAiWazaValueKouPuraioritii(void* ctx, u32 slot, u32 param, u32 unused) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x53);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x53);
    if (fn_801F1990(0, ctx, 1, 1, 0x10e, slot) == 1) {
        handle = fn_80239984(handle, ctx, 0x54);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x54);
    }
    return handle;
}

/* Address: 0x8024E234 | Size: 0xEC (236 bytes) */
u32 fn_8024E234(void* ctx, u32 slot, u32 param, u32 unused) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = fn_80239984(0, ctx, 0x51);
    fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x51);
    if (fn_801F1990(0, ctx, 1, 1, 0x10e, slot) == 1) {
        handle = fn_80239984(handle, ctx, 0x52);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x52);
    }
    return handle;
}

/* Address: 0x8024E320 | Size: 0x164 (356 bytes) */
u32 fightTrainerAiWazaValueHittyuu(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u8 fn_802357CC(void*, u32);
    extern u8 fn_802358AC(void*, u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    void* battleCtx = ctx;
    u32 trainer = param1;
    u32 sequenceArg = param2;
    u32 handle = 0;

    if (fn_802357CC(battleCtx, param3) > 6U) {
        handle = fn_80239984(0, battleCtx, 0x4E);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x4E);
    }
    if (fn_802358AC(battleCtx, trainer) < 6U) {
        handle = fn_80239984(handle, battleCtx, 0x4F);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x4F);
    }
    if (fn_801F1990(0, battleCtx, 1, 1, 0x10E, trainer) == 1) {
        handle = fn_80239984(handle, battleCtx, 0x50);
        fn_80239EE8(0xEC64, battleCtx, fightOutPokemonGetPokemonPtr(trainer), 0, 0, sequenceArg, 0, 0x50);
    }
    return handle;
}

/* Address: 0x8024E484 | Size: 0xA8 */
u32 fightTrainerAiWazaValueTuujouIryoku(void* ctx, u32 slot, u32 param) {
    extern u8 fn_801F1990(u32, void*, u32, u32, u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern u32 fn_80239984(u32, void*, u32);
    extern void fn_80239EE8(u32, void*, u32, u32, u32, u32, u32, u32);
    u32 handle;

    handle = 0;
    if (fn_801F1990(0, ctx, 1, 1, 0x10e, slot) == 1) {
        handle = fn_80239984(0, ctx, 0x4d);
        fn_80239EE8(0xEC64, ctx, fightOutPokemonGetPokemonPtr(slot), 0, 0, param, 0, 0x4d);
    }
    return handle;
}

/* Address: 0x8024E534 | Size: 0x44 | Pattern: field_accessor */
u32 fn_8024E534(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801FB1C0();
    fn_801FB1C0(0, fn_801FB1C0(ctx, 0, 0x43, 0) & 0xFFFF, 0x2, 0);
    return 0;
}

/* Address: 0x8024E578 | Size: 0x118 (280 bytes) */
u32 fightTrainerAiSelectFightActionIrekae(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375D30[];
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801FB1C0(u32, u32, u32, u32);
    extern void fightOutPokemonCreateFightAction(u32, u32, u32, u32, void*, s32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern void fn_8023A118(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, s32);
    extern s32 fightTrainerAiSelectIrekaeDasuFightPokemon(void*, u32, u32, u32);
    extern s32 fightTrainerAiGetFightPokemonIrekaeModosuValue();
    u32 choice;
    s32 score;
    u32 field;

    fn_801FB1C0(0, fn_801FB1C0((u32)ctx, 0, 0x43, 0) & 0xffff, 2, 0);
    field = fn_801F4354(0, param1);
    score = fightTrainerAiGetFightPokemonIrekaeModosuValue(ctx, param1, param2);
    if (score <= 0) {
        return 0;
    }
    fn_8023A118(0xec63, 0xec04, 0xec05, field, fightOutPokemonGetPokemonPtr(param1), 0, 0, 0, 0, 0x228, score);
    choice = fightTrainerAiSelectIrekaeDasuFightPokemon(ctx, param2, 1, param1);
    if ((s16)choice < 0) {
        return 0;
    }
    fightOutPokemonCreateFightAction(param1, 0, 9, 0, lbl_80375D30, (s16)choice);
    return 1;
}

/* Address: 0x8024F8B4 | Size: 0x5CC (1484 bytes) */
s32 fightTrainerAiGetFightPokemonIrekaeModosuValue(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_801F4354();
    extern void fn_801FB1C0();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fn_8023565C();
    extern void fn_80235714();
    extern void fn_802367CC();
    extern void fn_80236E9C();
    extern void fn_80237288();
    extern void fn_8023753C();
    extern void fn_802376EC();
    extern void fn_8023785C();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fn_8023A118();
    extern void fn_8023C370();
    extern void fn_8023C530();
    extern s32 fightTrainerAiGetFightOutPokemonIrekaeJoutaiBadJoutaiAddsbuDataId();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r14 = 0;
    u32 r15 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r5 = 0x43;
    r6 = 0x0;
    r16 = r4;
    r15 = r3;
    r4 = 0x0;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r16;
    r24 = 0x0;
    r3 = 0x0;
    fn_801F4354();
    r0 = r3;
    r3 = r15;
    r20 = r0;
    r4 = r16;
    fn_80236E9C();
    r26 = r3;
    r3 = r15;
    r4 = r16;
    r5 = (u32)sp + 0x38;
    r6 = 0x0;
    r7 = 0x1;
    fn_802367CC();
    r27 = r3;
    r4 = r15;
    r5 = (u32)sp + 0x18;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r28 = r3;
    r14 = (u32)sp + 0x18;
    r31 = r3 & 0xFFFF;
    r18 = 0x0;
    r17 = 0x0;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)r31) break;
        r19 = *(u32*)(r14 + r0);
        if (r19 != (u32)0x0) {
            r3 = r15;
            r4 = r19;
            fn_802376EC();
            r25 = r3 & 0xFFFF;
            r30 = r27 & 0xFFFF;
            r21 = 0x0;
            while (1) {
                r0 = r21 & 0xFFFF;
                if (r0 >= (u32)r30) break;
                r3 = (u32)sp + 0x38;
                r22 = *(u16*)(r3 + r0);
                if (r22 != (u32)0x0) {
                    if (r22 != (u32)0x165) {
                        r3 = r15;
                        r4 = r16;
                        r5 = r22;
                        r6 = r19;
                        fn_8023C530();
                        r29 = r3;
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r17 = 0x1;
                        }
                        r3 = r15;
                        r4 = r19;
                        fn_80237288();
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) {
                            r17 = 0x1;
                        }
                        r3 = r15;
                        r4 = r22;
                        r5 = 0x1;
                        fn_8023943C();
                        r0 = r3 & 0xFF;
                        if (r0 != (u32)0x1) {
                            r3 = r15;
                            r4 = r16;
                            r5 = r22;
                            r6 = r19;
                            r7 = 0x0;
                            fn_8023C370();
                            if ((s32)r25 < (s32)r3) {
                                r0 = r29 & 0xFF;
                                if (r0 == (u32)0x1) {
                                    r18 = 0x1;
                }
                }
                }
                }
                }
                r21 = r21 + 0x1;

            }
        }
        r23 = r23 + 0x1;

    }
    r3 = r16;
    fightOutPokemonGetPokemonPtr();
    r8 = 0x0;
    r5 = (0x1 << 16);
    r0 = 0x227;
    r7 = r3;
    r6 = r20;
    *(u32*)(sp + 0xC) = r0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r15;
    r4 = r16;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if (r0 != (u32)r31) {
        r4 = r15;
        r3 = 0x0;
        r5 = 0x1;
        fn_80239984();
        r0 = r3;
        r3 = r16;
        r24 = r0;
        fightOutPokemonGetPokemonPtr();
        r7 = (0x1 << 16);
        r5 = r3;
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x1;
        fn_80239EE8();
    }
    r3 = r15;
    r4 = r16;
    fn_8023753C();
    r0 = r3 & 0xFF;
    if (r0 == (u32)r31) {
        r3 = r24;
        r4 = r15;
        r5 = 0x2;
        fn_80239984();
        r0 = r3;
        r3 = r16;
        r24 = r0;
        fightOutPokemonGetPokemonPtr();
        r7 = (0x1 << 16);
        r5 = r3;
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x2;
        fn_80239EE8();

    } else {
        r3 = r15;
        r4 = r16;
        fn_8023785C();
        r0 = r3 & 0xFF;
        if (r0 != (u32)r31) {
            r3 = r24;
            r4 = r15;
            r5 = 0x3;
            fn_80239984();
            r0 = r3;
            r3 = r16;
            r24 = r0;
            fightOutPokemonGetPokemonPtr();
            r7 = (0x1 << 16);
            r5 = r3;
            r4 = r20;
            r6 = 0x0;
            r7 = 0x0;
            r8 = 0x0;
            r9 = 0x0;
            r10 = 0x3;
            fn_80239EE8();
        }
    }
    r3 = r15;
    r4 = r16;
    fn_80235714();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r24;
        r4 = r15;
        r5 = 0x4;
        fn_80239984();
        r0 = r3;
        r3 = r16;
        r24 = r0;
        fightOutPokemonGetPokemonPtr();
        r7 = (0x1 << 16);
        r5 = r3;
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x4;
        fn_80239EE8();
    }
    r3 = r15;
    r4 = r16;
    fn_8023565C();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r24;
        r4 = r15;
        r5 = 0x5;
        fn_80239984();
        r0 = r3;
        r3 = r16;
        r24 = r0;
        fightOutPokemonGetPokemonPtr();
        r7 = (0x1 << 16);
        r5 = r3;
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x5;
        fn_80239EE8();
    }
    r0 = r17 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r24;
        r4 = r15;
        r5 = 0x6;
        fn_80239984();
        r0 = r3;
        r3 = r16;
        r24 = r0;
        fightOutPokemonGetPokemonPtr();
        r7 = (0x1 << 16);
        r5 = r3;
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x6;
        fn_80239EE8();
    }
    r0 = r18 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r24;
        r4 = r15;
        r5 = 0x7;
        fn_80239984();
        r0 = r3;
        r3 = r16;
        r24 = r0;
        fightOutPokemonGetPokemonPtr();
        r7 = (0x1 << 16);
        r5 = r3;
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x7;
        fn_80239EE8();
    }
    r3 = r15;
    r4 = r16;
    fn_8023753C();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r17 = (u32)sp + 0x18;
        r14 = r28 & 0xFFFF;
        r18 = 0x0;
        while (1) {
            r0 = r18 & 0xFFFF;
            if (r0 >= (u32)r14) break;
            r4 = *(u32*)(r17 + r0);
            if (r4 != (u32)0x0) {
                r3 = r15;
                fn_80236E9C();
                r3 = r3 & 0xFFFF;
                r0 = r26 & 0xFFFF;
                if (r3 > r0) {
                    r3 = r24;
                    r4 = r15;
                    r5 = 0x8;
                    fn_80239984();
                    r0 = r3;
                    r3 = r16;
                    r24 = r0;
                    fightOutPokemonGetPokemonPtr();
                    r7 = (0x1 << 16);
                    r5 = r3;
                    r4 = r20;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x8;
                    fn_80239EE8();
            }
            }
            r18 = r18 + 0x1;

        }
    }
    r3 = r15;
    r4 = r16;
    fightTrainerAiGetFightOutPokemonIrekaeJoutaiBadJoutaiAddsbuDataId();
    r0 = r3 & 0xFFFF;
    r14 = r3;
    if (r0 != (u32)r14) {
        r3 = r24;
        r4 = r15;
        r5 = r14;
        fn_80239984();
        r0 = r3;
        r3 = r16;
        r24 = r0;
        fightOutPokemonGetPokemonPtr();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r20;
        r10 = r14;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        fn_80239EE8();
    }
    r3 = r16;
    fightOutPokemonGetPokemonPtr();
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x226;
    r7 = r3;
    r6 = r20;
    *(u32*)(sp + 0xC) = r0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r24;
    return r3;
}

/* Address: 0x8024FE80 | Size: 0x1F0 (496 bytes) */
void fightTrainerAiGetFightPokemonIrekaeDasuTokuseiAddsubDataId(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A420[];
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_80235B04();
    extern void fn_80236FFC();
    extern void fn_8023715C();
    extern void fn_80237F74();
    extern void fn_80239058();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r5 = 0x1;
    r30 = r4;
    r29 = r3;
    r4 = 0x0;
    fn_80235B04();
    r4 = (u32)lbl_8027A420;
    r0 = 0x2;
    r4 = (u32)lbl_8027A420;
    r31 = r3;
    r6 = (u32)sp + 0x4;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u32*)((u8*)r4 + 0x4);
        r0 = *(u32*)((u8*)r4 + 0x8);
        *(u32*)((u8*)r6 + 0x4) = r3;
        r6 += 8; *(u32*)r6 = r0;
    } while (--ctr != 0);
    r0 = *(u32*)((u8*)r4 + 0x4);
    r4 = r29;
    r5 = (u32)sp + 0x1c;
    r3 = 0x0;
    *(u32*)((u8*)r6 + 0x4) = r0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r28 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x16;
    fn_80239058();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r25 = (u32)sp + 0x1c;
        r26 = r28 & 0xFFFF;
        r24 = 0x0;
        while (1) {
            r0 = r24 & 0xFFFF;
            if (r0 >= (u32)r26) break;
            r3 = r29;
            r4 = *(u32*)(r25 + r27);
            fn_8023715C();
            r4 = *(u32*)(r25 + r27);
            r27 = r3;
            r3 = r29;
            fn_80236FFC();
            r4 = r27 & 0xFFFF;
            r0 = r3 & 0xFFFF;
            if (r4 >= (u32)r0) {
                r3 = 0x25;
                return;
            }
            r24 = r24 + 0x1;

        }
    }
    r3 = r29;
    r4 = r30;
    r5 = 0x24;
    fn_80239058();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r27 = (u32)sp + 0x8;
        r25 = (u32)sp + 0x1c;
        r28 = r28 & 0xFFFF;
        r24 = 0x0;
        while (1) {
            r0 = r24 & 0xFFFF;
            if (r0 >= (u32)r28) break;
            r22 = 0x0;
            r23 = 0x0;
            while (1) {
                r0 = r23 & 0xFFFF;
                if (r0 >= (u32)0xa) break;
                r4 = *(u32*)(r25 + r26);
                r5 = *(u16*)(r27 + r0);
                r3 = r29;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r22 = 0x1;
                    break;
                }
                r23 = r23 + 0x1;

            }

            r0 = r22 & 0xFF;
            if (r0 != (u32)0x1) {
                r3 = 0x26;
                return;
            }
            r24 = r24 + 0x1;

        }
    }
    r0 = r31 & 0xFF;
    if (r0 != (u32)r28) {
        r3 = r29;
        r4 = r30;
        r5 = 0x4d;
        fn_80239058();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x27;
            return;
        }
        r3 = r29;
        r4 = r30;
        r5 = 0xd;
        fn_80239058();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x28;
            return;
    }
    }
    r3 = 0x0;

    return;
}

/* Address: 0x80250070 | Size: 0x27C (636 bytes) */
s32 fightTrainerAiGetFightOutPokemonIrekaeJoutaiBadJoutaiAddsbuDataId(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80236BFC();
    extern u8 fn_8023753C();

    if (fn_80236BFC(ctx, param1, 0x9) == 1) {
        return 0x9;
    }
    if (fn_80236BFC(ctx, param1, 0xA) == 1) {
        return 0xA;
    }
    if (fn_80236BFC(ctx, param1, 0x1E) == 1 && fn_8023753C(ctx, param1) == 0) {
        return 0xB;
    }
    if (fn_80236BFC(ctx, param1, 0xE) == 1) {
        return 0xC;
    }
    if (fn_80236BFC(ctx, param1, 0x17) == 1) {
        return 0xD;
    }
    if (fn_80236BFC(ctx, param1, 0x18) == 1) {
        return 0xE;
    }
    if (fn_80236BFC(ctx, param1, 0x19) == 1) {
        return 0xF;
    }
    if (fn_80236BFC(ctx, param1, 0x1B) == 1) {
        return 0x10;
    }
    if (fn_80236BFC(ctx, param1, 0x1C) == 1) {
        return 0x11;
    }
    if (fn_80236BFC(ctx, param1, 0x1D) == 1) {
        return 0x12;
    }
    if (fn_80236BFC(ctx, param1, 0x26) == 1) {
        return 0x13;
    }
    if (fn_80236BFC(ctx, param1, 0x27) == 1) {
        return 0x14;
    }
    if (fn_80236BFC(ctx, param1, 0x28) == 1) {
        return 0x15;
    }
    if (fn_80236BFC(ctx, param1, 0x29) == 1) {
        return 0x16;
    }
    if (fn_80236BFC(ctx, param1, 0x2A) == 1) {
        return 0x17;
    }
    return fn_80236BFC(ctx, param1, 0x30) == 1 ? 0x18 : 0;
}

/* Address: 0x802502EC | Size: 0x694 (1684 bytes) */
void fightTrainerAiSelectFightActionItem(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375D70[];
    extern void pokemonIsDarkPokemon();
    extern void fn_80142984();
    extern void itemUse2PokemonSimulation();
    extern void fn_801F0134();
    extern void fn_801F1A6C();
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_801F7C54();
    extern void fn_801FB1C0();
    extern void fightOutPokemonCreateFightActionUseItem();
    extern void fightOutPokemonGetPokemonPtr();
    extern void fightOutPokemonCheckFightOut();
    extern void fightPokemonCheckFightOut();
    extern void fn_802126C4();
    extern void fn_80235714();
    extern void fn_80236C80();
    extern void fn_80237310();
    extern void fn_8023753C();
    extern void fn_8023785C();
    extern void fn_802397B8();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fn_8023A118();
    u8 sp[0x250];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r6 = 0x0;
    r29 = r4;
    r22 = r5;
    r28 = r3;
    r4 = 0x0;
    r5 = 0x43;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r3 = r28;
    r4 = (u32)sp + 0x38;
    r30 = 0x0;
    r5 = 0x14;
    r6 = 0x1;
    fn_801F7C54();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = r28;
    r5 = (u32)sp + 0x18;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r17 = r3;
    r4 = r28;
    r5 = (u32)sp + 0x60;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r26 = r3;
    r18 = (u32)sp + 0x18;
    r17 = r17 & 0xFFFF;
    r20 = 0x0;
    while (1) {
        r0 = r20 & 0xFFFF;
        if (r0 >= (u32)r17) break;
        r19 = *(u32*)(r18 + r0);
        if (r19 != (u32)0x0) {
            r3 = r19;
            fightOutPokemonCheckFightOut();
            r0 = r3 & 0xFF;
            if (r19 != (u32)0x0) {
                r3 = r28;
                r4 = r19;
                fn_80235714();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r30 = 0x0;
        }
        }
        }
        r20 = r20 + 0x1;

    }
    r3 = (u32)sp + 0xc0;
    r5 = 0x0;
    r4 = 0x0;
    while (1) {
        r0 = r5 & 0xFFFF;
        if (r0 >= (u32)0x14) break;
        r5 = r5 + 0x1;
        *(u32*)(r3 + r0) = r4;

    }
    r20 = (u32)sp + 0x38;
    r27 = r31 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if (r0 >= (u32)r27) break;
        r25 = r24 & 0xFFFF;
        r23 = *(u16*)(r20 + r0);
        if (r23 != (u32)0x0) {
            r3 = r23;
            fn_80142984();
            r0 = r3 & 0xFF;
            if (r23 != (u32)0x0) {
                r3 = r23;
                r4 = r29;
                fn_802126C4();
                r21 = r3;
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x7) {
                    r3 = r29;
                    r4 = 0x0;
                    r5 = 0xd5;
                    r6 = 0x0;
                    ((void(*)(void))pokemonGetStatus)();
                    r5 = r3;
                    r6 = r23;
                    r3 = (u32)sp + 0x110;
                    r4 = 0x0;
                    r7 = 0x0;
                    itemUse2PokemonSimulation();
                    r3 = (s16)r3;
                    r0 = -r3;
                    r0 = r0 & ~r3;
                    if (r0 != (u32)0x7) {
                        r0 = r21 & 0xFF;

                        if (r0 == (u32)0x2 || r0 == (u32)0x1) {

                            r3 = r28;
                            r4 = r29;
                            fn_8023753C();
                            r0 = r3 & 0xFF;
                            if (r0 == (u32)0x1) {
                                r3 = r29;
                                fightOutPokemonGetPokemonPtr();
                                pokemonIsDarkPokemon();
                                r0 = r3 & 0xFF;
                                if (r0 == (u32)0x1) {
                                    r17 = r25 << 2;
                                    r18 = (u32)sp + 0xc0;
                                    r3 = *(u32*)(r18 + r17);
                                    r4 = r28;
                                    r5 = 0x2e;
                                    fn_80239984();
                                    *(u32*)(r18 + r17) = r3;
                                    r3 = r29;
                                    fightOutPokemonGetPokemonPtr();
                                    r6 = (0x1 << 16);
                                    r5 = r3;
                                    r4 = r28;
                                    r9 = r23;
                                    r6 = 0x0;
                                    r7 = 0x0;
                                    r8 = 0x0;
                                    r10 = 0x2e;
                                    fn_80239EE8();
        }
            }
                        }
                        r0 = r21 & 0xFF;

                        if (r0 == (u32)0x3 || r0 == (u32)0x1) {

                            r3 = r28;
                            r4 = r29;
                            fn_80237310();
                            r0 = r3 & 0xFF;
                            if (r0 == (u32)0x1) {
                                r3 = r29;
                                fightOutPokemonGetPokemonPtr();
                                pokemonIsDarkPokemon();
                                r0 = r3 & 0xFF;
                                if (r0 == (u32)0x1) {
                                    r17 = r25 << 2;
                                    r18 = (u32)sp + 0xc0;
                                    r3 = *(u32*)(r18 + r17);
                                    r4 = r28;
                                    r5 = 0x2f;
                                    fn_80239984();
                                    *(u32*)(r18 + r17) = r3;
                                    r3 = r29;
                                    fightOutPokemonGetPokemonPtr();
                                    r6 = (0x1 << 16);
                                    r5 = r3;
                                    r4 = r28;
                                    r9 = r23;
                                    r6 = 0x0;
                                    r7 = 0x0;
                                    r8 = 0x0;
                                    r10 = 0x2f;
                                    fn_80239EE8();
        }
            }
                        }
                        r0 = r21 & 0xFF;
                        if (r0 == (u32)0x5) {
                            r3 = r28;
                            r4 = r29;
                            fn_80235714();
                            r0 = r3 & 0xFF;
                            if (r0 == (u32)0x5) {
                                r17 = r25 << 2;
                                r18 = (u32)sp + 0xc0;
                                r3 = *(u32*)(r18 + r17);
                                r4 = r28;
                                r5 = 0x30;
                                fn_80239984();
                                *(u32*)(r18 + r17) = r3;
                                r3 = r29;
                                fightOutPokemonGetPokemonPtr();
                                r6 = (0x1 << 16);
                                r5 = r3;
                                r4 = r28;
                                r9 = r23;
                                r6 = 0x0;
                                r7 = 0x0;
                                r8 = 0x0;
                                r10 = 0x30;
                                fn_80239EE8();
                        }
                        }
                        r0 = r21 & 0xFF;
                        if (r0 == (u32)0x4) {
                            r3 = r28;
                            r4 = r29;
                            fn_80236C80();
                            r0 = r3 & 0xFF;
                            if (r0 == (u32)0x2) {
                                r17 = r25 << 2;
                                r18 = (u32)sp + 0xc0;
                                r3 = *(u32*)(r18 + r17);
                                r4 = r28;
                                r5 = 0x31;
                                fn_80239984();
                                *(u32*)(r18 + r17) = r3;
                                r3 = r29;
                                fightOutPokemonGetPokemonPtr();
                                r6 = (0x1 << 16);
                                r5 = r3;
                                r4 = r28;
                                r9 = r23;
                                r6 = 0x0;
                                r7 = 0x0;
                                r8 = 0x0;
                                r10 = 0x31;
                                fn_80239EE8();
                        }
                        }
                        r0 = r21 & 0xFF;
                        if (r0 == (u32)0x6) {
                            r0 = r30 & 0xFF;
                            if (r0 == (u32)0x1) {
                                r17 = r25 << 2;
                                r18 = (u32)sp + 0xc0;
                                r3 = *(u32*)(r18 + r17);
                                r4 = r28;
                                r5 = 0x32;
                                fn_80239984();
                                *(u32*)(r18 + r17) = r3;
                                r3 = r29;
                                fightOutPokemonGetPokemonPtr();
                                r6 = (0x1 << 16);
                                r5 = r3;
                                r4 = r28;
                                r9 = r23;
                                r6 = 0x0;
                                r7 = 0x0;
                                r8 = 0x0;
                                r10 = 0x32;
                                fn_80239EE8();
                        }
                        }
                        r3 = r28;
                        r4 = r29;
                        fn_8023785C();
                        r0 = r3 & 0xFF;
                        if (r0 != (u32)0x2) {
                            r3 = r28;
                            r4 = r29;
                            fn_8023785C();
                            r0 = r3 & 0xFF;
                            if (r0 != (u32)0x3) {
                                r19 = (u32)sp + 0x60;
                                r17 = r26 & 0xFFFF;
                                r18 = 0x0;
                                while (1) {
                                    r0 = r18 & 0xFFFF;
                                    if (r0 >= (u32)r17) break;
                                    r3 = r29;
                                    r4 = 0x0;
                                    r5 = 0xd5;
                                    r6 = 0x0;
                                    ((void(*)(void))pokemonGetStatus)();
                                    r0 = *(u32*)(r19 + r21);
                                    if (r3 != (u32)r0) {
                                        r3 = r28;
                                        r4 = r29;
                                        fn_8023785C();
                                        r0 = r3 & 0xFF;
                                        if (r0 != (u32)0x2) {
                                            r3 = r28;
                                            r4 = r29;
                                            fn_8023785C();
                                            r0 = r3 & 0xFF;
                                            if (r0 == (u32)0x3) {
                                            }
                                            r3 = *(u32*)(r19 + r21);
                                            fightPokemonCheckFightOut();
                                            r0 = r3 & 0xFF;
                                            if (r0 == (u32)0x1) {
                                                r17 = r25 << 2;
                                                r18 = (u32)sp + 0xc0;
                                                r3 = *(u32*)(r18 + r17);
                                                r4 = r28;
                                                r5 = 0x33;
                                                fn_80239984();
                                                *(u32*)(r18 + r17) = r3;
                                                r3 = r29;
                                                fightOutPokemonGetPokemonPtr();
                                                r6 = (0x1 << 16);
                                                r5 = r3;
                                                r4 = r28;
                                                r9 = r23;
                                                r6 = 0x0;
                                                r7 = 0x0;
                                                r8 = 0x0;
                                                r10 = 0x33;
                                                fn_80239EE8();
                                                break;
                    }
                                        }
                                            }
                                    r18 = r18 + 0x1;

                                }
        }
                        }
                        r3 = r29;
                        fightOutPokemonGetPokemonPtr();
                        r0 = 0x226;
                        r4 = (u32)sp + 0xc0;
                        r5 = (0x1 << 16);
                        *(u32*)(sp + 0xC) = r0;
                        r0 = r25 << 2;
                        r7 = r3;
                        r6 = r28;
                        r0 = *(u32*)(r4 + r0);
                        *(u32*)(sp + 0x10) = r0;
                        r8 = 0x0;
                        r9 = 0x0;
                        r10 = 0x0;
                        fn_8023A118();
        }
        }
        }
        }
        r24 = r24 + 0x1;

    }
    r4 = (u32)sp + 0xc0;
    r0 = r31 & 0xFFFF;
    r17 = 0x0;
    while (1) {
        r3 = r17 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        r3 = *(u32*)(r4 + r3);
        if ((s32)r3 > (s32)0x0) break;
        r17 = r17 + 0x1;

    }

    r3 = r17 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if (r3 >= (u32)r0) {
        r3 = 0x0;
        return;
    }
    r4 = r31;
    r3 = (u32)sp + 0xc0;
    r5 = 0x1;
    fn_802397B8();
    if (r3 < r0) {
        r3 = 0x0;
        return;
    }
    r3 = (u32)sp + 0x38;
    r17 = *(u16*)(r3 + r0);
    if (r17 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r29;
    fightOutPokemonGetPokemonPtr();
    r0 = 0x228;
    r4 = (u32)sp + 0xc0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0xC) = r0;
    r0 = r18 << 2;
    r7 = r3;
    r6 = r28;
    r0 = *(u32*)(r4 + r0);
    *(u32*)(sp + 0x10) = r0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r29;
    r4 = r22;
    fn_801F0134();
    r0 = 0x0;
    r4 = (u32)lbl_80375D70;
    *(u32*)(sp + 0x8) = r0;
    r9 = r3;
    r7 = (u32)lbl_80375D70;
    r3 = r29;
    r8 = r17;
    r4 = 0x0;
    r5 = 0x12;
    r6 = 0x0;
    r10 = -0x1;
    fightOutPokemonCreateFightActionUseItem();
    r3 = 0x1;

    return;
}

/* Address: 0x802509A0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage209(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80250A2C | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage207(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80250AC0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage204(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80250B44 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage203(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u32 fightSeqGetNromalWazaDamage(void*, u32, u32, u32, u32, u32, void*, u32);
    extern u8 fn_80235B04(void*, u32, u32);
    extern void _fightTrainerAiWazaDamage203SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void*, u32, u32);
    u32 damage;
    u8 boost;

    boost = fn_80235B04(ctx, 0, 1);
    damage = fightSeqGetNromalWazaDamage(ctx, param, slot, extra, 0, 0, _fightTrainerAiWazaDamage203SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon, 0);
    if (boost != 0) {
        damage <<= 1;
    }
    return damage;
}

/* Address: 0x80250BBC | Size: 0xA8 */
u32 _fightTrainerAiWazaDamage203SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 param1, u32 param2) {
    extern u32 wazaSetStatus(u32, u32, u32, u32, u32);
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u32 fn_80235B04(void*, u32, u32);
    u32 value;
    u32 status;
    u32 mapped;

    status = fn_80235B04(ctx, 0, 1);
    value = pokemonGetStatus(param2, 0, 0xd9, 0);
    status = status & 0xFF;
    if (status == 2) {
        mapped = 0xb;
    } else if (status == 3) {
        mapped = 5;
    } else if (status == 1) {
        mapped = 0xa;
    } else if (status == 4) {
        mapped = 0xf;
    } else {
        mapped = 0;
    }
    return wazaSetStatus(value, 0, 0x30, 0, mapped & 0xFFFF);
}

/* Address: 0x80250C64 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage202(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80250CF0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage200(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80250D7C | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage198(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80250E00 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage197(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80250E84 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage196(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage196SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage196SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x80250EC4 | Size: 0x90 */
void _fightTrainerAiWazaDamage196SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 param1, u32 param2) {
    extern void wazaSetStatus(u32, u32, u32, u32, u32);
    extern u32 fightSeqGetKetaguriIryoku(u32);
    extern u32 fn_802377E8(void*, u32);
    u32 sourceValue;
    u32 convertedValue;
    u32 scaledValue;

    sourceValue = (u32)pokemonGetStatus(param2, 0, 0xd9, 0);
    convertedValue = fn_802377E8(ctx, param2);
    scaledValue = fightSeqGetKetaguriIryoku((u32)pokemonGetStatus(0, convertedValue, 0x5f, 0) & 0xffff);
    wazaSetStatus(sourceValue, 0, 0x2f, 0, scaledValue & 0xffff);
}

/* Address: 0x80250F7C | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage190(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage190SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage190SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x80250FBC | Size: 0xB4 */
void _fightTrainerAiWazaDamage190SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 param1, u32 param2) {
    extern void wazaSetStatus(u32, u32, u32, u32, u32);
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u32 fn_80237664(void*, u32);
    extern u32 fn_802376EC(void*, u32);
    u32 value;
    u32 current;
    u32 maximum;
    u16 amount;

    value = pokemonGetStatus(param2, 0, 0xd9, 0);
    current = fn_802376EC(ctx, param2);
    maximum = fn_80237664(ctx, param2);
    amount = wazaGetStatus(value, 0, 0x2f, 0);
    amount = (s32)(amount * (current & 0xFFFF)) / (s32)(maximum & 0xFFFF);
    if (amount == 0) {
        amount = 1;
    }
    wazaSetStatus(value, 0, 0x2f, 0, amount);
}

/* Address: 0x80251070 | Size: 0x5C | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage189(void* ctx, u32 slot, u32 param, u32 arg3) {
    extern u32 fn_802376EC(void*, u32);
    u32 first;
    u32 second;
    u32 diff;
    u32 sumBase;

    first = fn_802376EC(ctx, slot) & 0xffff;
    second = fn_802376EC(ctx, arg3) & 0xffff;
    diff = second - first;
    sumBase = first - second;
    return diff & -((sumBase + (second ^ 0x80000000)) < sumBase);
}

/* Address: 0x802510CC | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage188(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80251158 | Size: 0x3C | Pattern: simple_wrapper */
extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
u32 fightTrainerAiWazaDamage186(void* ctx, u32 param1, u32 param2, u32 param3) {
    return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);
}

/* Address: 0x80251194 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage185(void* ctx, u32 param1, u32 param2, u32 param3) {
    return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);
}

/* Address: 0x802511E0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage182(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802512A4 | Size: 0xAC */
u32 fightTrainerAiWazaDamage173(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 fn_801363E8(u32);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u32 fn_8023C370(void*, u32, u32, u32, u32);
    u32 paramType;
    u32 other;
    u32 otherType;

    other = fn_801363E8(fn_801F54A4(0, 0, 0xf, 0) & 0xFFFF);
    paramType = wazaGetStatus(0, param, 9, 0) & 0xFFFF;
    param = other;
    otherType = wazaGetStatus(0, param, 9, 0) & 0xFFFF;
    if (otherType != paramType) {
        return fn_8023C370(ctx, slot, param, extra, 1);
    }
    return 0;
}

/* Address: 0x80251358 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage171(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x5) == 1) {
        v1 = v1 << 1;
    }
    return v1;
}

/* Address: 0x802513D0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage170(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80251454 | Size: 0x70 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage169(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u32 fightSeqGetNromalWazaDamage(void*, u32, u32, u32, u32, u32, u32, u32);
    extern u8 fn_8023720C(void*, u32);
    u32 savedSlot;
    void* savedCtx;
    u32 damage;

    savedSlot = slot;
    savedCtx = ctx;
    damage = fightSeqGetNromalWazaDamage(ctx, param, savedSlot, extra, 0, 0, 0, 0);
    if (fn_8023720C(savedCtx, savedSlot) == 1) {
        damage <<= 1;
    }
    return damage;
}

/* Address: 0x802514EC | Size: 0x98 */
s32 fightTrainerAiWazaDamage162(void* ctx, u32 param1, u32 param2) {
    extern u8 fn_80119DD0(u32);
    extern s16 fn_80202360(u32, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u32 fn_80237664(void*, u32);
    s16 multiplier;
    s16 shift;
    u32 hp;

    multiplier = 1;
    if (fn_80236BFC(ctx, param1, 0x2d) == 1) {
        multiplier = fn_80202360(param1, 0x2d);
    }
    shift = fn_80119DD0(0x2d) - multiplier;
    if (shift < 0) {
        shift = 0;
    }
    multiplier = 1 << shift;
    hp = fn_80237664(ctx, param1) & 0xFFFF;
    return -((s32)hp / multiplier);
}

/* Address: 0x80251584 | Size: 0x88 */
s32 fightTrainerAiWazaDamage161(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s16 fn_80202360(u32, u32);
    extern s32 fightSeqGetNromalWazaDamage(void*, u32, u32, u32, u32, u32, u32, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    s16 multiplier;

    multiplier = 1;
    if (fn_80236BFC(ctx, param1, 0x2d) == 1) {
        multiplier = fn_80202360(param1, 0x2d);
    }
    return multiplier * fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);
}

/* Address: 0x80251614 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage159(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251658 | Size: 0x28 | Ghidra import */
int fightTrainerAiWazaDamage157(void)

{
    extern u32 fn_80237664();
  u32 uVar1;
  
  uVar1 = fn_80237664();
  return -(uVar1 >> 1 & 0x7fff);
}
/* Address: 0x80251688 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage155(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x802516C4 | Size: 0xD4 (212 bytes) */
u32 fightTrainerAiWazaDamage154(void* ctx, u32 unused, u32 param2, u32 param3) {
    extern u16 fn_801F1A6C(u32, void*, u32*, u32, u32);
    extern u32 fn_80216CF8(u32, u32, u8, u32, u8);
    extern u32 fn_80237774();
    extern u32 fn_802377E8();
    extern u32 fn_8023892C();
    extern u32 fn_80238980();
    u32 entries[24];
    u32 total;
    u16 count;
    u16 index;
    u32 mappedEntry;
    u32 entryType;
    u32 convertedParam;
    u32 convertedState;

    total = 0;
    count = fn_801F1A6C(0, ctx, entries, 1, 1);
    index = 0;
    while (index < count) {
        mappedEntry = fn_80238980(ctx, entries[index]);
        entryType = fn_8023892C(ctx, entries[index]);
        convertedParam = fn_802377E8(ctx, param3);
        convertedState = fn_80237774(ctx, param3);
        total += fn_80216CF8(param2, mappedEntry, (u8)entryType, convertedParam, (u8)convertedState);
        index++;
    }
    return total;
}

/* Address: 0x802517A0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage152(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80251824 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage151(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251860 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage150(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x23) == 1) {
        v1 = v1 << 1;
    }
    return v1;
}

/* Address: 0x802518D8 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage149(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x1f) == 1) {
        v1 = v1 << 1;
    }
    return v1;
}

/* Address: 0x80251950 | Size: 0xBC */
u32 fightTrainerAiWazaDamage148(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 fn_801F025C(u32, u32);
    extern u8 fn_802026E4(u32, u32);
    extern u32 fn_80232110(u32, u32, u32, u32, u32, u32);
    u32 damage;
    u32 stat7;
    u32 stat3;
    u32 target;

    stat7 = wazaGetStatus(0, param, 7, 0) & 0xFFFF;
    stat3 = wazaGetStatus(0, param, 3, 0) & 0xFFFF;
    target = fn_801F025C(2, extra);
    damage = fn_80232110(slot, extra, target, param, stat7, stat3);
    if (fn_802026E4(slot, 0x32) == 1) {
        damage = (s32)(damage * 0xf) / 0xa;
    }
    return damage;
}

/* Address: 0x80251A0C | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage147(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x20) == 1) {
        v1 = v1 << 1;
    }
    return v1;
}

/* Address: 0x80251A84 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage146(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x1f) == 1) {
        v1 = v1 << 1;
    }
    return v1;
}

/* Address: 0x80251AFC | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage145(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251B50 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage140(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80251BD4 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage139(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80251C58 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage138(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80251CEC | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage135(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage135SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage135SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x80251D2C | Size: 0x88 */
void _fightTrainerAiWazaDamage135SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 param1, u32 param2) {
    extern void* fightOutPokemonGetPokemonPtr();
    extern void pokemonGetMezamerupower();
    extern void wazaSetStatus();
    void* handle;
    u16 var_a;
    u16 var_8;

    handle = pokemonGetStatus(param2, 0, 0xd9, 0);
    pokemonGetMezamerupower(fightOutPokemonGetPokemonPtr(param2), &var_a, &var_8);
    wazaSetStatus(handle, 0, 0x2f, 0, var_a);
    wazaSetStatus(handle, 0, 0x30, 0, var_8);
}

/* Address: 0x80251DB4 | Size: 0x90 */
int fightTrainerAiWazaDamage134(void* ctx, u32 param1, u32 param2) {
    extern u8 fn_80235B04(void*, u32, u32);
    extern u32 fn_80237664(void*, u32);
    u32 status;
    s32 value;

    status = fn_80235B04(ctx, 0, 1);
    if ((u8)status == 0) {
        value = (fn_80237664(ctx, param1) >> 1) & 0x7FFF;
    } else if ((u8)status == 1) {
        value = ((s32)(fn_80237664(ctx, param1) & 0xFFFF) * 0x14) / 0x1E;
    } else {
        value = (fn_80237664(ctx, param1) >> 2) & 0x3FFF;
    }
    return -value;
}

/* Address: 0x80251E44 | Size: 0x90 */
int fightTrainerAiWazaDamage133(void* ctx, u32 param1, u32 param2) {
    extern u8 fn_80235B04(void*, u32, u32);
    extern u32 fn_80237664(void*, u32);
    u32 status;
    s32 value;

    status = fn_80235B04(ctx, 0, 1);
    if ((u8)status == 0) {
        value = (fn_80237664(ctx, param1) >> 1) & 0x7FFF;
    } else if ((u8)status == 1) {
        value = ((s32)(fn_80237664(ctx, param1) & 0xFFFF) * 0x14) / 0x1E;
    } else {
        value = (fn_80237664(ctx, param1) >> 2) & 0x3FFF;
    }
    return -value;
}

/* Address: 0x80251ED4 | Size: 0x90 */
int fightTrainerAiWazaDamage132(void* ctx, u32 param1, u32 param2) {
    extern u8 fn_80235B04(void*, u32, u32);
    extern u32 fn_80237664(void*, u32);
    u32 status;
    s32 value;

    status = fn_80235B04(ctx, 0, 1);
    if ((u8)status == 0) {
        value = (fn_80237664(ctx, param1) >> 1) & 0x7FFF;
    } else if ((u8)status == 1) {
        value = ((s32)(fn_80237664(ctx, param1) & 0xFFFF) * 0x14) / 0x1E;
    } else {
        value = (fn_80237664(ctx, param1) >> 2) & 0x3FFF;
    }
    return -value;
}

/* Address: 0x80251F6C | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage129(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80251FF0 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage128(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    int iVar1;
  iVar1 = fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,0,0);
  return iVar1 << 1;
}


/* Address: 0x80252038 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage126(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage126SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage126SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x80252078 | Size: 0x44 | Pattern: field_accessor */
u32 _fightTrainerAiWazaDamage126SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 slot, u32 param) {
    extern u32 wazaSetStatus();
    u32 val = (u32)pokemonGetStatus((void*)param, 0, 0xd9, 0);
    return (u32)wazaSetStatus(val, 0, 0x2f, 0, 0x46);
}

/* Address: 0x802520BC | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage125(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252148 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage123(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage123SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage123SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x80252188 | Size: 0x80 | Pattern: field_accessor */
void _fightTrainerAiWazaDamage123SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 slot, u32 param) {
    extern void wazaSetStatus(u32, u32, u32, u32, u32);
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u32 fn_80217BD0(u32);
    extern u32 fn_8023842C(void*, u32);
    u32 value;
    u32 amount;

    value = pokemonGetStatus(param, 0, 0xd9, 0);
    wazaGetStatus(value, 0, 0x2f, 0);
    amount = fn_80217BD0(fn_8023842C(ctx, param));
    wazaSetStatus(value, 0, 0x2f, 0, amount & 0xFFFF);
}

/* Address: 0x80252208 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage122(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage122SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage122SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x80252248 | Size: 0x44 | Pattern: field_accessor */
u32 _fightTrainerAiWazaDamage122SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 slot, u32 param) {
    extern u32 wazaSetStatus();
    u32 val = (u32)pokemonGetStatus((void*)param, 0, 0xd9, 0);
    return (u32)wazaSetStatus(val, 0, 0x2f, 0, 0x28);
}

/* Address: 0x8025228C | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage121(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage121SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage121SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x802522CC | Size: 0x80 | Pattern: field_accessor */
void _fightTrainerAiWazaDamage121SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 slot, u32 param) {
    extern void wazaSetStatus(u32, u32, u32, u32, u32);
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u32 fn_80217BEC(u32);
    extern u32 fn_8023842C(void*, u32);
    u32 value;
    u32 amount;

    value = pokemonGetStatus(param, 0, 0xd9, 0);
    wazaGetStatus(value, 0, 0x2f, 0);
    amount = fn_80217BEC(fn_8023842C(ctx, param));
    wazaSetStatus(value, 0, 0x2f, 0, amount & 0xFFFF);
}

/* Address: 0x80252354 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage119(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252398 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage117(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage117SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage117SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x802523D8 | Size: 0x90 */
void _fightTrainerAiWazaDamage117SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 param1, u32 param2) {
    extern void wazaSetStatus(u32, u32, u32, u32, u32);
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    u32 value;
    u32 result;
    u32 amount;

    value = pokemonGetStatus(param2, 0, 0xd9, 0);
    amount = wazaGetStatus(value, 0, 0x2f, 0) & 0xFFFF;
    if (fn_80236BFC(ctx, param2, 0x1a) == 1) {
        amount = (amount << 1) & 0xFFFF;
    }
    wazaSetStatus(value, 0, 0x2f, 0, amount & 0xFFFF);
}

/* Address: 0x802524B8 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage105(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x8025253C | Size: 0xB4 */
u32 fightTrainerAiWazaDamage104(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void*, u32, u32, u32, u32, u32, u32, u32);
    extern u32 _fightTrainerAiWazaDamage104SubPre3__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void);
    extern u32 _fightTrainerAiWazaDamage104SubPre2__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void);
    extern u32 _fightTrainerAiWazaDamage104SubPre1__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void);
    u32 result;

    result = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, (u32)_fightTrainerAiWazaDamage104SubPre1__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon, 0);
    result += fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, (u32)_fightTrainerAiWazaDamage104SubPre2__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon, 0);
    result += fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, (u32)_fightTrainerAiWazaDamage104SubPre3__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon, 0);
    return result;
}

/* Address: 0x802525F0 | Size: 0x44 | Pattern: field_accessor */
u32 _fightTrainerAiWazaDamage104SubPre3__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 slot, u32 param) {
    extern u32 wazaSetStatus();
    u32 val = (u32)pokemonGetStatus((void*)param, 0, 0xd9, 0);
    return (u32)wazaSetStatus(val, 0, 0x2f, 0, 0x1e);
}

/* Address: 0x80252634 | Size: 0x44 | Pattern: field_accessor */
u32 _fightTrainerAiWazaDamage104SubPre2__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 slot, u32 param) {
    extern u32 wazaSetStatus();
    u32 val = (u32)pokemonGetStatus((void*)param, 0, 0xd9, 0);
    return (u32)wazaSetStatus(val, 0, 0x2f, 0, 0x14);
}

/* Address: 0x80252678 | Size: 0x44 | Pattern: field_accessor */
u32 _fightTrainerAiWazaDamage104SubPre1__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 slot, u32 param) {
    extern u32 wazaSetStatus();
    u32 val = (u32)pokemonGetStatus((void*)param, 0, 0xd9, 0);
    return (u32)wazaSetStatus(val, 0, 0x2f, 0, 0xa);
}

/* Address: 0x802526BC | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage103(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252748 | Size: 0x74 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage101(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u32 fightSeqGetNromalWazaDamage(void*, u32, u32, u32, u32, u32, u32, u32);
    extern u32 fn_802376EC(void*, u32);
    u32 cap;
    u32 damage;

    cap = fn_802376EC(ctx, extra);
    damage = fightSeqGetNromalWazaDamage(ctx, param, slot, extra, 0, 0, 0, 0);
    if ((s32)(cap & 0xFFFF) <= (s32)damage) {
        return (cap & 0xFFFF) - 1;
    }
    return damage;
}

/* Address: 0x802527C4 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage099(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    extern void _fightTrainerAiWazaDamage099SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon();
  return fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,(u32)_fightTrainerAiWazaDamage099SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon,0);
}


/* Address: 0x80252804 | Size: 0x90 */
void _fightTrainerAiWazaDamage099SubPre__FP13FIGHT_TRAINERUsP15FightOutPokemonP15FightOutPokemon(void* ctx, u32 param1, u32 param2) {
    extern void* pokemonGetStatus();
    extern u32 fn_802376EC();
    extern s32 fn_80237664();
    extern u8 fn_80218B6C();
    extern void wazaSetStatus();
    void* a;
    u32 b;
    s32 t;

    a = pokemonGetStatus(param2, 0, 0xD9, 0);
    b = fn_802376EC(ctx, param2);
    t = fn_80237664(ctx, param2);
    wazaSetStatus(a, 0, 0x2F, 0, fn_80218B6C(b, t));
}

/* Address: 0x80252894 | Size: 0x28 | Pattern: call_return_u16 */
extern u32 fn_802376EC(void*, u32, u32);
u16 fightTrainerAiWazaDamage098(void* ctx, u32 p1, u32 p2, u32 p3) { return (u16)fn_802376EC(ctx, p3, p2); }

/* Address: 0x802528DC | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage092(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252918 | Size: 0x54 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage091(void* ctx, u32 slot, u32 arg2, u32 param) {
    extern u32 fn_802376EC();
    u32 val1, val2, avg;
    val1 = fn_802376EC(ctx, slot) & 0xFFFF;
    val2 = fn_802376EC(ctx, param) & 0xFFFF;
    avg = (s32)(val1 + val2) / 2;
    return val2 - avg;
}

/* Address: 0x8025297C | Size: 0x24 | Pattern: call_return_u8 */
extern u32 fn_80237774(void*);
u8 fightTrainerAiWazaDamage088(void* ctx) { return (u8)fn_80237774(ctx); }

/* Address: 0x802529A0 | Size: 0x24 | Pattern: call_return_u8 */
u8 fightTrainerAiWazaDamage087(void* ctx) { return (u8)fn_80237774(ctx); }

/* Address: 0x802529F4 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage080(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252A80 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage078(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252B04 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage077(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    int iVar1;
  iVar1 = fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,0,0);
  return iVar1 << 1;
}


/* Address: 0x80252B44 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage076(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252BC8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage075(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252C04 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage073(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252C88 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage072(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252D0C | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage071(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252D90 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage070(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252E14 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage069(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252E98 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage068(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80252F8C | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage048(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80253020 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage045(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802530A4 | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage044(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    int iVar1;
  iVar1 = fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,0,0);
  return iVar1 << 1;
}


/* Address: 0x802530E4 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage043(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80253168 | Size: 0x88 */
u32 fightTrainerAiWazaDamage042(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0xfa) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802531F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage040(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253234 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage039(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253270 | Size: 0x28 | Pattern: call_return_u16 */
u16 fightTrainerAiWazaDamage038(void* ctx, u32 p1, u32 p2, u32 p3) { return (u16)fn_802376EC(ctx, p3, p2); }

/* Address: 0x80253298 | Size: 0x28 | Ghidra import */
int fightTrainerAiWazaDamage037(void)

{
    extern u32 fn_80237664();
  u32 uVar1;
  
  uVar1 = fn_80237664();
  return -(uVar1 & 0xffff);
}
/* Address: 0x802532C0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage036(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x8025334C | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage034(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802533D8 | Size: 0x28 | Ghidra import */
int fightTrainerAiWazaDamage032(void)

{
    extern u32 fn_80237664();
  u32 uVar1;
  
  uVar1 = fn_80237664();
  return -(uVar1 >> 1 & 0x7fff);
}
/* Address: 0x80253400 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage031(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x8025348C | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaDamage029(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern u32 fightSeqGetNromalWazaDamage();
    int iVar1;
  iVar1 = fightSeqGetNromalWazaDamage(r3,r5,r4,r6,0,0,0,0);
  return iVar1 * 3;
}


/* Address: 0x802534D4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage027(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253548 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage017(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802535F4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage008(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253630 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage007(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x8025366C | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage006(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802536F0 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage005(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80253774 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage004(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802537F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fightTrainerAiWazaDamage003(void* ctx, u32 param1, u32 param2, u32 param3) { return fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253834 | Size: 0x84 | Pattern: field_accessor */
u32 fightTrainerAiWazaDamage002(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);
    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x802538C0 | Size: 0x88 */
u32 fightTrainerAiWazaDamage000(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightSeqGetNromalWazaDamage(void* ctx, u32 param2, u32 param1, u32 param3, u32 zero1, u32 zero2, u32 zero3, u32 zero4);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u32 v1 = fightSeqGetNromalWazaDamage(ctx, param2, param1, param3, 0, 0, 0, 0);

    if (fn_80236BFC(ctx, param3, 0x21) == 1) {
        if ((u16)param2 == 0x39) {
            v1 = v1 << 1;
        }
    }
    return v1;
}

/* Address: 0x80253950 | Size: 0x6C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit213(void* ctx, u32 slot, u32 param) {
    extern u32 fn_80136428(u32);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u8 fn_80237DBC(void*, u32, u32);
    u32 value;
    u32 result;

    value = fn_80136428(fn_801F54A4(0, 0, 0xf, 0) & 0xFFFF);
    result = fn_80237DBC(ctx, slot, value & 0xFF);
    result = result != 1;
    return result;
}

/* Address: 0x802539BC | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit212(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235910(void*, u32);
    extern u32 fn_80235AA0(void);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_80235AA0();
    secondStatus = fn_80235910(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 1, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 3, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253A80 | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit211(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235974(void*, u32);
    extern u32 fn_802359D8(void*, u32);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_802359D8(ctx, param1);
    secondStatus = fn_80235974(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 4, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 5, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253B44 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit210(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x39);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x80253B78 | Size: 0xB4 */
s32 fightTrainerAiWazaHit209(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80253C2C | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit208(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235A3C(void*, u32);
    extern u32 fn_80235AA0(void*, u32);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_80235AA0(ctx, param1);
    secondStatus = fn_80235A3C(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 1, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 2, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253CF0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit207(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80253DA4 | Size: 0xC4 (196 bytes) */
u32 fightTrainerAiWazaHit206(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_80235974(void*, u32);
    extern u32 fn_80235A3C(void*, u32);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u32 firstStatus;
    u32 secondStatus;

    firstStatus = fn_80235A3C(ctx, param1);
    secondStatus = fn_80235974(ctx, param1);
    if ((firstStatus & 0xff) >= 0xc) {
        if ((secondStatus & 0xff) >= 0xc) {
            return 0;
        }
    }
    if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 2, 0x41) & 0xff) == 0) {
        if ((fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 5, 0x41) & 0xff) == 0) {
            return 0;
        }
    }
    return 1;
}

/* Address: 0x80253E68 | Size: 0xC0 */
s32 fightTrainerAiWazaHit205(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235AA0(void* ctx, u32 elem);
    extern u8 fn_80235A3C(void* ctx, u32 elem);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u8 a = fn_80235AA0(ctx, param3);
    u8 b = fn_80235A3C(ctx, param3);

    if (a == 0 && b == 0) {
        return 0;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 1, 1) != 0) {
        goto ret1;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 2, 1) != 0) {
        goto ret1;
    }
    return 0;
ret1:
    return 1;
}

/* Address: 0x80253F28 | Size: 0xB4 */
s32 fightTrainerAiWazaHit204(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80253FDC | Size: 0xF0 (240 bytes) */
s32 fightTrainerAiWazaHit203(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235B04(void* ctx, u32 zero, u32 one);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u8 state = fn_80235B04(ctx, 0, 1);
    s32 gate;
    u32 v1;

    if (state == 2) {
        v1 = 0xb;
    } else if (state == 3) {
        v1 = 0x5;
    } else if (state == 1) {
        v1 = 0xa;
    } else if (state == 4) {
        v1 = 0xf;
    } else {
        v1 = 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* -------------------------------------------------------------------
 * Item Rewards & Poke Coupon (0x80254000-0x80258000)
 * 95 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802540CC | Size: 0xB4 */
s32 fightTrainerAiWazaHit202(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254180 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit201(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x38);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
/* Address: 0x802541B4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit200(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254268 | Size: 0x1F8 (504 bytes) */
void fightTrainerAiWazaHit199(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r31 = r6;
    r28 = r4;
    r29 = r5;
    r3 = 0x2;
    r4 = r31;
    fn_801F025C();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_802543A0;
        }
        r3 = r27;
        r4 = r31;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_802543A0:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r30;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254460 | Size: 0xB4 */
s32 fightTrainerAiWazaHit198(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254514 | Size: 0xB4 */
s32 fightTrainerAiWazaHit197(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802545C8 | Size: 0xB0 */
s32 fightTrainerAiWazaHit196(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80254680 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit194(u32 r3, u32 r4, u32 r5, u32 r6)

{
  extern u32 fn_80236BFC();
  u32 uVar1;
  
  uVar1 = fn_80236BFC(r3,r6,0x28);
  return (uVar1 & 0xff) != 1;
}
/* Address: 0x802546B8 | Size: 0x30 | Ghidra import */
u32 fightTrainerAiWazaHit193(void)

{
    extern u32 fn_8023720C();
  u32 uVar1;
  
  uVar1 = fn_8023720C();
  uVar1 = __cntlzw(1 - (uVar1 & 0xff));
  return uVar1 >> 5 & 0xff;
}
/* Address: 0x802546E8 | Size: 0x128 (296 bytes) */
void fightTrainerAiWazaHit192(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fightFloorGetFightTrainerFightOutPokemonPtrAry();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r6 = 0x0;
    r7 = 0x0;
    r5 = (u32)sp + 0x1c;
    r29 = r3;
    r27 = r4;
    fn_802367CC();
    r31 = r3;
    r4 = r29;
    r5 = (u32)sp + 0x30;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fightFloorGetFightTrainerFightOutPokemonPtrAry();
    r28 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x27;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r27 = (u32)sp + 0x30;
    r28 = r28 & 0xFFFF;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)r28) break;
        r4 = *(u32*)(r27 + r0);
        if (r4 != (u32)0x0) {
            r3 = r29;
            r5 = (u32)sp + 0x8;
            r6 = 0x0;
            r7 = 0x0;
            fn_802367CC();
            r5 = (u32)sp + 0x1c;
            r4 = r31 & 0xFFFF;
            r8 = (u32)sp + 0x8;
            r0 = r3 & 0xFFFF;
            r10 = 0x0;
            while (1) {
                r3 = r10 & 0xFFFF;
                if (r3 >= (u32)r0) break;
                r9 = 0x0;
                while (1) {
                    r3 = r9 & 0xFFFF;
                    if (r3 >= (u32)r4) break;
                    r6 = *(u16*)(r8 + r7);
                    r3 = *(u16*)(r5 + r3);
                    if (r6 == (u32)r3) {
                        r3 = 0x1;
                        return;
                    }
                    r9 = r9 + 0x1;

                }
                r10 = r10 + 0x1;

            }
        }
        r30 = r30 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* Address: 0x80254810 | Size: 0xC8 (200 bytes) */
s32 fightTrainerAiWazaHit191(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u8 fn_80229934(u32 param2, u32 param1, u32 param3);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);

    if (fn_80229934(param2, param1, param3) == 1) {
        return 0;
    }
    if (fn_80237F74(ctx, param1, 0x19) == 1) {
        return 0;
    }
    if (fn_80237F74(ctx, param3, 0x19) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802548D8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit190(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025498C | Size: 0xE4 (228 bytes) */
s32 fightTrainerAiWazaHit189(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u16 fn_802376EC(void* ctx, u32 elem);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    u16 a = fn_802376EC(ctx, param1);
    u16 b = fn_802376EC(ctx, param3);
    s32 gate;

    if (b <= a) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80254A70 | Size: 0xB4 */
s32 fightTrainerAiWazaHit188(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254B24 | Size: 0x178 (376 bytes) */
void fightTrainerAiWazaHit187(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80229704();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckSawagu();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r5;
    r30 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x48;
    r4 = r30;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r29;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    fightTrainerAiCheckSawagu();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r4 = r30;
    r3 = 0x8;
    fn_80229704();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254C9C | Size: 0xB0 */
s32 fightTrainerAiWazaHit186(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80254D4C | Size: 0xB4 */
s32 fightTrainerAiWazaHit185(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254E00 | Size: 0x2c | Ghidra import */
u32 fightTrainerAiWazaHit184(void)

{
    extern u16 fn_80236B98();
  u16 sVar1;
  
  sVar1 = fn_80236B98();
  return sVar1 != 0;
}
/* Address: 0x80254E34 | Size: 0xB4 */
s32 fightTrainerAiWazaHit182(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80254EE8 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit181(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x25);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x80254F1C | Size: 0x38 | Ghidra import */
/* Address: 0x80254F1C | Size: 0x38 | Ghidra import */
u32 fightTrainerAiWazaHit180(u32 r3, u32 r4)
{
    extern u32 fn_80215008();
    u8 auStack_38[0x38];

    u32 uVar1 = fn_80215008(r3, auStack_38, 0x18, r4);
    return (-uVar1 & ~uVar1) >> 0x1f;
}
/* Address: 0x80254F54 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit179(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  u32 uVar1;
  uVar1 = fn_80236BFC(r3, r4, 0x35);
  return (uVar1 & 0xFF) != 1;
}
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit178(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80237F74(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFF);
    if (fn_80237F74(ctx, extra, 0x19) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255000 | Size: 0xF0 (240 bytes) */
s32 fightTrainerAiWazaHit177(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802383A4(void* ctx, u32 elem);
    extern u8 fn_80216048(u32 elem);
    extern u8 fn_80142984(u32 elem);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    u16 A = fn_802383A4(ctx, param1);
    u16 B = fn_802383A4(ctx, param3);
    u8 c = fn_80216048(param1);

    if (!c) {
        return 0;
    }
    if (A == 0 && B == 0) {
        goto ret0;
    }
    if (A == 0xaf) {
        goto ret0;
    }
    if (B == 0xaf) {
        goto ret0;
    }
    if (A != 0 && fn_80142984(A) == 0) {
        goto ret0;
    }
    if (B != 0 && fn_80142984(B) == 0) {
        return 0;
    }
    if (fn_80237F74(ctx, param3, 0x3c) == 1) {
        goto ret0;
    }
    return 1;
ret0:
    return 0;
}

/* Address: 0x802550F0 | Size: 0xB4 */
u32 fightTrainerAiWazaHit176(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F025C(u32);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u8 fightOutPokemonCheckFightOut(u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    u32 target;

    target = fn_801F025C(0xe);
    if (target == 0) {
        return 0;
    }
    if ((fn_801F54A4(0, 0, 0x19, 0) & 0xFFFF) < 2) {
        goto fail;
    }
    if (fightOutPokemonCheckFightOut(target) != 1) {
        goto fail;
    }
    if (fn_80236BFC(ctx, slot, 0x32) != 0) {
        goto fail;
    }
    if (fn_80236BFC(ctx, target, 0x32) == 0) {
        goto success;
    }
fail:
    return 0;
success:
    return 1;
}

/* Address: 0x802551A4 | Size: 0x74 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit175(void* ctx, u32 slot, u32 param, u32 param3) {
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    u32 result;

    result = _fightTrainerAiWazaHitCheck(ctx, slot, param, param3, 0);
    if ((fn_80236BFC(ctx, param3, 0x30) & 0xff) == 1) {
        return 0;
    }
    if ((s32)result == 0) {
        return 0;
    }
    if ((s32)result == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255220 | Size: 0xA8 */
u32 fightTrainerAiWazaHit173(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 fn_801363E8(u32);
    extern u32 fn_801F54A4(u32, u32, u32, u32);
    extern u32 fn_8023C530(void*, u32, u32, u32);
    u32 paramType;
    u32 other;
    u32 otherType;

    other = fn_801363E8(fn_801F54A4(0, 0, 0xf, 0) & 0xFFFF);
    paramType = wazaGetStatus(0, param, 9, 0) & 0xFFFF;
    param = other;
    otherType = wazaGetStatus(0, param, 9, 0) & 0xFFFF;
    if (otherType != paramType) {
        return fn_8023C530(ctx, slot, param, extra);
    }
    return 1;
}

/* Address: 0x802552D0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit171(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80255384 | Size: 0xB4 */
s32 fightTrainerAiWazaHit170(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80255438 | Size: 0xB4 */
s32 fightTrainerAiWazaHit169(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802554EC | Size: 0x10C (268 bytes) */
s32 fightTrainerAiWazaHit168(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235AA0(void* ctx, u32 elem);
    extern u8 fn_802359D8(void* ctx, u32 elem);
    extern u8 fightTrainerAiCheckGuard(void* ctx, u32 param3, u32 param2);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    u8 q1 = fn_80235AA0(ctx, param3);
    u8 q2 = fn_802359D8(ctx, param3);

    if (fightTrainerAiCheckGuard(ctx, param3, param2) == 1) {
        return 0;
    }
    if (q1 == 0 && q2 == 0) {
        return 0;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 1, 1) == 0 &&
        fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 4, 1) == 0) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    return 1;
}

/* Address: 0x802555F8 | Size: 0x228 (552 bytes) */
u32 fightTrainerAiWazaHit167(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80236BFC();
    extern u8 fn_80237DBC();
    extern u8 fn_80237F74();
    extern u8 fn_80237310();
    extern s32 _fightTrainerAiWazaHitCheck();
    extern s32 fn_801F025C();
    extern u8 fightSideIsJoutaiDataId();
    s32 handle;
    u8 flag;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) return 0;
    if (fn_80236BFC(ctx, param3, 0x6) == 1) return 0;
    if (fn_80237DBC(ctx, param3, 0xA) == 1) return 0;
    if (fn_80237F74(ctx, param3, 0x29) == 1) return 0;

    if (fn_80237F74(ctx, param3, 0x11) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x14) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x7) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0xF) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x48) == 1) goto flag1;
    if (fn_80237F74(ctx, param3, 0x29) == 1) {
        flag = 0;
        goto check;
    }
    if (fn_80237F74(ctx, param3, 0x28) != 1) {
        fn_80237F74(ctx, param3, 0xC);
    }
flag1:
    flag = 1;
check:
    if (flag == 0) return 0;

    if (fn_80237310(ctx, param3) == 0) return 0;

    handle = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4B) == 1) return 0;
    if (handle == 0) return 0;
    if (handle == -1) return 1;
    return 1;
}

/* Address: 0x80255820 | Size: 0x218 (536 bytes) */
void fightTrainerAiWazaHit166(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80235910();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckAbiCnt();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r3;
    r27 = r6;
    r25 = r4;
    r26 = r5;
    r3 = 0x2;
    r4 = r27;
    fn_801F025C();
    r0 = r3;
    r3 = r31;
    r28 = r0;
    r4 = r27;
    fn_80235910();
    r29 = r3;
    r3 = r31;
    r4 = r25;
    r5 = r26;
    r6 = r27;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r30 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r29 & 0xFF;
        if (r0 >= (u32)0xc) {
            r3 = r31;
            r4 = r25;
            r5 = r27;
            r6 = r26;
            r7 = 0x10;
            r8 = 0x4;
            r9 = 0x1;
            fightTrainerAiCheckAbiCnt();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0xc) {
                r3 = 0x0;
                return;
    }
    }
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_802559DC;
        }
        r3 = r31;
        r4 = r27;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_802559DC:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80255A38 | Size: 0x74 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit165(void* ctx, u32 slot, u32 param, u32 param3) {
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    u32 result;

    result = _fightTrainerAiWazaHitCheck(ctx, slot, param, param3, 0);
    if ((fn_80236BFC(ctx, param3, 0x1b) & 0xff) == 1) {
        return 0;
    }
    if ((s32)result == 0) {
        return 0;
    }
    if ((s32)result == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255AAC | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit164(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 4;
}
/* Address: 0x80255AE4 | Size: 0x68 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit162(void* ctx, u32 slot, u32 param) {
    extern f32 lbl_8047E648;
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);

    if (fn_80236BFC(ctx, slot, 0x2d) == 0) {
        return 0;
    }
    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}

/* Address: 0x80255B4C | Size: 0xCC (204 bytes) */
s32 fightTrainerAiWazaHit161(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u8 fn_80236BFC(void* ctx, u32 param1, u32 flag);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_80236BFC(ctx, param1, 0x2d) == 0) {
        gate = 0;
    }
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255C18 | Size: 0x74 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit160(void* ctx, u32 slot, u32 param) {
    extern u8 fn_80236BFC(void* ctx, u32 slot, u32 param);
    extern u32 fn_80202360(u32 slot, u32 param);
    extern u8 fn_80119DD0(u32 param);
    u32 result;

    result = slot;
    if (fn_80236BFC(ctx, slot, 0x2d) == 0) {
        result = 0;
    } else {
        result = fn_80202360(result, 0x2d);
    }
    if ((s16)result >= (s32)fn_80119DD0(0x2d)) {
        return 0;
    }
    return 1;
}

/* Address: 0x80255C8C | Size: 0xB0 */
s32 fightTrainerAiWazaHit159(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255D3C | Size: 0x40 | Ghidra import */
u32 fightTrainerAiWazaHit158(u32 r3_in, u32 r4)
{
    extern u32 pokemonGetStatus();
  u32 uVar1;
  u8 result;
  uVar1 = pokemonGetStatus(r4, 0, 0xed, 0) & 0xFFFF;
  result = uVar1 != 0;
  return result;
}
/* Address: 0x80255D7C | Size: 0x38 */
u32 fightTrainerAiWazaHit157(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x80255DB4 | Size: 0x44 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit156(void* ctx, u32 slot, u32 param, u32 arg3) {
    extern u32 fightTrainerAiCheckAbiCnt();
    u32 result = fightTrainerAiCheckAbiCnt(ctx, slot, arg3, param, 0x10, 0x2, 0x41) & 0xFF;
    return (result != 0) ? 1 : 0;
}

/* Address: 0x80255DF8 | Size: 0xB0 */
s32 fightTrainerAiWazaHit155(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80255EA8 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit154(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern int _fightTrainerAiWazaHitCheck();
  int iVar1;
  iVar1 = _fightTrainerAiWazaHitCheck(r3, r4, r5, r6, 0);
  if (iVar1 == 0) {
    return 0;
  }
  if (iVar1 == -1) {
    return 1;
  }
  return 1;
}
/* Address: 0x80255EEC | Size: 0xB4 */
s32 fightTrainerAiWazaHit152(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80255FA0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit151(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256054 | Size: 0xB4 */
s32 fightTrainerAiWazaHit150(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256108 | Size: 0xB4 */
s32 fightTrainerAiWazaHit149(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802561BC | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit148(u32 r3, u32 r4, u32 r5, u32 r6)

{
  extern u32 fn_80236BFC();
  u32 uVar1;
  
  uVar1 = fn_80236BFC(r3,r6,0x34);
  return (uVar1 & 0xff) != 1;
}
/* Address: 0x802561F4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit147(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802562A8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit146(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025635C | Size: 0xB4 */
s32 fightTrainerAiWazaHit145(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256410 | Size: 0xB0 */
s32 fightTrainerAiWazaHit144(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802564C8 | Size: 0x64 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit142(void* ctx, u32 slot, u32 param) {
    extern f32 lbl_8047E64C;
    extern u32 fn_80235AA0(void);
    extern u32 fn_802373B0(void*, u32, s32, f32);

    if ((fn_80235AA0() & 0xFF) >= 0xC) {
        goto ret0;
    }
    if ((fn_802373B0(ctx, slot, -1, lbl_8047E64C) & 0xFF) == 0) {
        goto ret1;
    }
ret0:
    return 0;
ret1:
    return 1;
}

/* Address: 0x8025652C | Size: 0xB4 */
s32 fightTrainerAiWazaHit140(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802565E0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit139(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256694 | Size: 0xB4 */
s32 fightTrainerAiWazaHit138(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256748 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit137(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 1;
}
/* Address: 0x80256780 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit136(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 2;
}
/* Address: 0x802567B8 | Size: 0xA4 */
s32 fightTrainerAiWazaHit135(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fightOutPokemonGetPokemonPtr(u32 v);
    extern void pokemonGetMezamerupower(u32, void*, void*);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, s16 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u16 hi;
    u16 lo;
    s32 gate;

    pokemonGetMezamerupower(fightOutPokemonGetPokemonPtr(param1), &hi, &lo);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, lo, (s16)hi) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025685C | Size: 0x38 */
u32 fightTrainerAiWazaHit134(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x80256894 | Size: 0x38 */
u32 fightTrainerAiWazaHit133(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x802568CC | Size: 0x38 */
u32 fightTrainerAiWazaHit132(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x80256904 | Size: 0xB0 */
s32 fightTrainerAiWazaHit130(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802569B4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit129(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256A68 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit128(void* ctx, u32 a, u32 b, u32 c) {
    extern s32 fn_802395C8();
    extern s32 fn_80239500();
    extern s32 fn_8023793C();
    s32 res1;

    res1 = fn_802395C8(ctx, b, a);
    return !((u16)fn_8023793C(ctx, c, res1, fn_80239500(ctx, b)) == 0x43);
}

/* Address: 0x80256AE0 | Size: 0x38 | Ghidra import */
u32 fightTrainerAiWazaHit127(void)
{
    extern int fn_801F8A18();
  u32 r3;
  int iVar1;
  u16 local_8[4];

  local_8[0] = 0;
  iVar1 = fn_801F8A18(r3, local_8);
  return (-iVar1 != 0) ? 1 : 0;
}
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
/* Address: 0x80256B18 | Size: 0xB4 */
s32 fightTrainerAiWazaHit126(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256BCC | Size: 0xB4 */
s32 fightTrainerAiWazaHit125(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256C80 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit124(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x4b);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x80256CBC | Size: 0xB4 */
s32 fightTrainerAiWazaHit123(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80256D70 | Size: 0xB0 */
s32 fightTrainerAiWazaHit122(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80256E20 | Size: 0xB0 */
s32 fightTrainerAiWazaHit121(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80256ED0 | Size: 0x200 (512 bytes) */
void fightTrainerAiWazaHit120(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802016A4();
    extern void fn_80236BFC();
    extern void fn_80237288();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r26 = r4;
    r27 = r3;
    r29 = r5;
    r28 = r6;
    r3 = r26;
    fn_802016A4();
    r30 = r3;
    r3 = r28;
    fn_802016A4();
    r31 = r3;
    r3 = r27;
    r4 = r26;
    r5 = r29;
    r6 = r28;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    r4 = r28;
    r5 = 0xc;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r29 = 0x0;
    }
    r3 = r27;
    r4 = r28;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r27;
        r4 = r28;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r27;
            r4 = r28;
            r5 = 0x7;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r3 = r27;
                r4 = r28;
                r5 = 0xf;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                    r3 = r27;
                    r4 = r28;
                    r5 = 0x48;
                    fn_80237F74();
                    r0 = r3 & 0xFF;
                    if (r0 != (u32)0x1) {
                        r3 = r27;
                        r4 = r28;
                        r5 = 0x29;
                        fn_80237F74();
                        r0 = r3 & 0xFF;
                        if (r0 != (u32)0x1) {
                            r3 = r27;
                            r4 = r28;
                            r5 = 0x28;
                            fn_80237F74();
                            r0 = r3 & 0xFF;
                            if (r0 != (u32)0x1) {
                                r3 = r27;
                                r4 = r28;
                                r5 = 0xc;
                                fn_80237F74();
                                r0 = r3 & 0xFF;
                                if (r0 == (u32)0x1) {
                                    r0 = 0x0;
                                    goto L_80257034;
    }
    }
    }
    }
    }
    }
    }
    }
    r0 = 0x1;
L_80257034:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30 & 0xFF;
    r0 = r31 & 0xFF;
    if (r3 != (u32)r0) {
        r3 = r27;
        r4 = r28;
        fn_80237288();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r27;
            r4 = r28;
            r5 = 0xa;
            fn_80236BFC();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1) {
                r0 = r30 & 0xFF;
                if (r0 != (u32)0x2) {
                    r0 = r31 & 0xFF;
                    if (r0 == (u32)0x2) {
        }
        }
        }
        }
        r29 = 0x0;
                    }
    if ((s32)r29 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802570D0 | Size: 0xB0 */
s32 fightTrainerAiWazaHit119(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257180 | Size: 0x23C (572 bytes) */
void fightTrainerAiWazaHit118(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80235AA0();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckAbiCnt();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r3;
    r27 = r6;
    r25 = r4;
    r26 = r5;
    r3 = 0x2;
    r4 = r27;
    fn_801F025C();
    r0 = r3;
    r3 = r31;
    r28 = r0;
    r4 = r27;
    fn_80235AA0();
    r29 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = r25;
    r5 = r26;
    r6 = r27;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r30 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r29 & 0xFF;
        if (r0 >= (u32)0xc) {
            r3 = r31;
            r4 = r25;
            r5 = r27;
            r6 = r26;
            r7 = 0x20;
            r8 = 0x1;
            r9 = 0x1;
            fightTrainerAiCheckAbiCnt();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0xc) {
                r3 = 0x0;
                return;
    }
    }
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = r27;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_80257360;
        }
        r3 = r31;
        r4 = r27;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r31;
        r4 = r27;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_80257360:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802573BC | Size: 0xB0 */
s32 fightTrainerAiWazaHit117(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257474 | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit115(void)

{
    u32 r3;

    extern u32 fn_80235B04();
  u32 uVar1;
  
  uVar1 = fn_80235B04(r3,0,0);
  return (uVar1 & 0xff) != 3;
}
/* Address: 0x802574AC | Size: 0x5C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit114(void* ctx, u32 slot, u32 param, u32 arg3) {
    extern u32 fn_80229934();
    extern u32 fightTrainerAiCheckHorobinouta();

    if ((u8)fn_80229934(param, slot, arg3) == 1) {
        return 0;
    }
    return (u16)fightTrainerAiCheckHorobinouta(ctx) != 0;
}

/* Address: 0x80257508 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit113(u32 r3, u32 r4, u32 r5, u32 r6)
{
    extern int _fightTrainerAiWazaHitCheck();
  int iVar1;
  iVar1 = _fightTrainerAiWazaHitCheck(r3, r4, r5, r6, 0);
  if (iVar1 == 0) {
    return 0;
  }
  if (iVar1 == -1) {
    return 1;
  }
  return 1;
}
/* Address: 0x80257544 | Size: 0x7C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit112(void* ctx, u32 slot, u32 param) {
    extern u8 fn_80119DD0(u32);
    extern u32 fn_801F025C(u32);
    extern u32 fightSideGetCountAsJoutaiDataId(u32, u32);
    extern u8 fightSideIsJoutaiDataId(u32, u32);
    u32 target;
    u32 count;

    target = fn_801F025C(3);
    count = 0;
    if (fightSideIsJoutaiDataId(target, 0x4a) == 1) {
        count = fightSideGetCountAsJoutaiDataId(target, 0x4a);
    }
    if ((s16)count >= (s32)fn_80119DD0(0x4a)) {
        return 0;
    }
    return 1;
}

/* Address: 0x802575C8 | Size: 0x188 (392 bytes) */
s32 fightTrainerAiWazaHit109(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80235AA0(void* ctx, u32 elem);
    extern u8 fn_80235A3C(void* ctx, u32 elem);
    extern u8 fn_80235910(void* ctx, u32 elem);
    extern u8 fn_80237DBC(void* ctx, u32 a, u32 type);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fn_80237288(void* ctx, u32 elem);
    extern u8 fightTrainerAiCheckAbiCnt(void*, u32, u32, u32, u32, u32, u32);
    u8 a = fn_80235AA0(ctx, param3);
    u8 b = fn_80235A3C(ctx, param3);
    u8 c = fn_80235910(ctx, param3);

    if (fn_80237DBC(ctx, param1, 7) == 1) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        if (fn_80236BFC(ctx, param3, 0x18) != 1) {
            if (fn_80237288(ctx, param3) != 1) {
                goto ret1;
            }
        }
        return 0;
    }

    if (((c == 0) && (a >= 0xc)) && (b >= 0xc)) {
        return 0;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 3, 0x41) != 0) {
        goto ret1;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 1, 0x41) != 0) {
        goto ret1;
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 2, 0x41) != 0) {
        goto ret1;
    }
ret0:
    return 0, 0;

    return 0;
ret1:
    return 1;
}

/* Address: 0x80257750 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit108(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 7, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80257838 | Size: 0x8C */
u32 fightTrainerAiWazaHit107(void* ctx, u32 param1, u32 param2, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);

    if (fn_80236BFC(ctx, extra, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, extra, 0x17) == 1) {
        return 0;
    }
    return fn_80236BFC(ctx, extra, 8) != 0;
}

/* Address: 0x802578C4 | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit106(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFE);
    if (fn_80236BFC(ctx, extra, 0x16) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025793C | Size: 0xB4 */
s32 fightTrainerAiWazaHit105(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802579F0 | Size: 0xB0 */
s32 fightTrainerAiWazaHit104(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257AA0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit103(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80257B54 | Size: 0xCC (204 bytes) */
s32 fightTrainerAiWazaHit102(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F1A6C(u32 zero, void* ctx, void* buf, u32 one1, u32 one2);
    extern u8 fn_80239058(void* ctx, u32 elem, u32 type);
    extern u8 fn_80238748(void* ctx, u32 elem);
    u32 buf[0x17];
    u32 count = fn_801F1A6C(0, ctx, buf, 1, 1);
    u16 target;
    u16 count16;
    u32 *bufPtr;
    u16 i;
    u8 flag;

    bufPtr = buf;
    count16 = (u16)count;
    target = (u16)param2;
    flag = 0;
    for (i = 0; i < count16; i++) {
        if (bufPtr[i] == 0) {
            continue;
        }
        if (target == 0xd7 && fn_80239058(ctx, bufPtr[i], 0x2b) == 1) {
            continue;
        }
        if (fn_80238748(ctx, bufPtr[i]) != 0) {
            continue;
        }
        flag = 1;
        break;
    }
    if (flag == 0) {
        return 0;
    }
    return 1;
}

/* Address: 0x80257C20 | Size: 0xB4 */
s32 fightTrainerAiWazaHit101(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80257CD4 | Size: 0x124 (292 bytes) */
s32 fightTrainerAiWazaHit100(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u16 fn_802367CC(void* ctx, u32 param3, void* arrayA, void* arrayB, u32 flag);
    extern u32 fn_802364BC(void* ctx, u32 param3);
    extern u8 fn_80237288(void* ctx, u32 elem);
    extern u8 fn_802381C4(void* ctx, u32 param3, u8 v);
    u16 arrayA[0xe];
    s16 arrayB[0xa];
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);
    u16 count = fn_802367CC(ctx, param3, arrayA, arrayB, 0);
    u32 a = fn_802364BC(ctx, param3);
    u8 flag;
    u8 i;

    if ((u16)a == 0 || (u16)a == 0x165 || (u16)a == 0xffff || fn_80237288(ctx, param3) == 1) {
        return 0;
    }
    flag = 0;
    for (i = 0; i < count; i++) {
        if (arrayB[i] < 0) {
            continue;
        }
        if ((u16)a != arrayA[i]) {
            continue;
        }
        flag = fn_802381C4(ctx, param3, (u8)arrayB[i]);
        break;
    }
    if (flag == 0) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80257DF8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit099(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80257EAC | Size: 0x38 | Ghidra import */
int fightTrainerAiWazaHit098(u32 arg0, u32 arg1, u32 arg2, u32 arg3)
{
    extern u32 fn_80229934();

    return (u8)fn_80229934(arg2, arg1, arg3) != 1;
}
/* Address: 0x80257EE4 | Size: 0xE4 (228 bytes) */
s32 fightTrainerAiWazaHit097(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 param1, void* buf, u32 zero, u32 one);
    extern u8 fn_80236BFC(void* ctx, u32 param1, u32 flag);
    extern u8 fightSeqRendouWazaCheck(u32 elem);
    extern u8 fn_8021901C(int elem);
    u16 buf[0xa];
    u16 count = fn_802367CC(ctx, param1, buf, 0, 1);
    u16 i;

    if (!fn_80236BFC(ctx, param1, 8)) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        u16 elem = buf[i];
        if (elem == 0x165 || elem == 0x163) {
            continue;
        }
        if (fightSeqRendouWazaCheck(elem) != 0) {
            continue;
        }
        if (elem == 0x108 || elem == 0xfd) {
            continue;
        }
        if (fn_8021901C(elem) == 0) {
            break;
        }
    }
    if (i >= count) {
        return 0;
    }
    return 1;
}

/* Address: 0x80257FC8 | Size: 0xF4 (244 bytes) */
s32 fightTrainerAiWazaHit095(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802367CC(void* ctx, u32 param1, void* buf, u32 zero1, u32 zero2);
    extern u32 fn_80236458(void* ctx, u32 param3);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fn_80219270(u32 elem);
    u16 buf[0xa];
    u32 count = fn_802367CC(ctx, param1, buf, 0, 0);
    u32 target = fn_80236458(ctx, param3);
    u16 i;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param1, 0x10) == 1) {
        goto ret0;
    }
    if (fn_80219270(target) != 1) {
        goto scan;
    }
ret0:
    return 0;
scan:
    for (i = 0; i < (u16)count; i++) {
        if (buf[i] == (u16)target) {
            return 0;
        }
    }
    return 1;
}

/* -------------------------------------------------------------------
 * Team State Updates (0x80258000-0x8025C000)
 * 78 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802580BC | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit094(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFF);
    if (fn_80236BFC(ctx, extra, 0x14) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025813C | Size: 0xD0 (208 bytes) */
s32 fightTrainerAiWazaHit092(void* ctx, u32 param1, u32 param2, u32 param3) {
    u32 *new_var;
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u8 fn_80236BFC(void* ctx, u32 param1, u32 flag);
    extern u32 fn_80239500(void* ctx, u32 param2);
    s32 gate;
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);

    if (!fn_80236BFC(ctx, param1, 0x8)) {
        return 0;
    }
    new_var = &v1;
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, *new_var, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (((0, gate)) == (-1)) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025820C | Size: 0x78 | Pattern: field_accessor */
u32 fightTrainerAiWazaHit091(void* ctx, u32 slot, u32 param, u32 extra) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    s32 gate;

    gate = _fightTrainerAiWazaHitCheck(ctx, slot, param, extra, 0xFFFF);
    if (fn_80236BFC(ctx, extra, 0x14) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258284 | Size: 0x140 (320 bytes) */
s32 fightTrainerAiWazaHit090(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 param3, void* arrayA, void* arrayB, u32 flag);
    extern u32 fn_802364BC(void* ctx, u32 param3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightSeqIsEncoreNgWazaDataId(u32 elem);
    extern u8 fn_802381C4(void* ctx, u32 param3, u8 v);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    u16 arrayA[0xe];
    s16 arrayB[0xa];
    u16 count = fn_802367CC(ctx, param3, arrayA, arrayB, 0);
    u32 targetVal = fn_802364BC(ctx, param3);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    u8 flag;
    u16 i;

    if (fightSeqIsEncoreNgWazaDataId(targetVal) == 1) {
        return 0;
    }
    flag = 0;
    for (i = 0; i < count; i++) {
        if (arrayB[i] < 0) {
            continue;
        }
        if ((u16)targetVal != arrayA[i]) {
            continue;
        }
        flag = fn_802381C4(ctx, param3, (u8)arrayB[i]);
        break;
    }
    if (flag == 0) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x2a) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;

    return;
}

/* Address: 0x802583C4 | Size: 0xB0 */
s32 fightTrainerAiWazaHit089(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258474 | Size: 0xB0 */
s32 fightTrainerAiWazaHit088(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258524 | Size: 0xB0 */
s32 fightTrainerAiWazaHit087(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802585D4 | Size: 0x120 (288 bytes) */
s32 fightTrainerAiWazaHit086(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 param3, void* arrayA, void* arrayB, u32 flag);
    extern u16 fn_802364BC(void* ctx, u32 param3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fn_802381C4(void* ctx, u32 param3, u8 v);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    u16 arrayA[0xe];
    s16 arrayB[0xa];
    u16 count = fn_802367CC(ctx, param3, arrayA, arrayB, 0);
    u16 target = fn_802364BC(ctx, param3);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    u8 flag = 0;
    u16 i;

    for (i = 0; i < count; i++) {
        if (arrayB[i] < 0) {
            continue;
        }
        if (target != arrayA[i]) {
            continue;
        }
        flag = fn_802381C4(ctx, param3, (u8)arrayB[i]);
        break;
    }
    if (flag == 0) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x29) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;

    return;
}

/* Address: 0x802586FC | Size: 0xBC */
s32 fightTrainerAiWazaHit084(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fn_80237DBC(void* ctx, u32 a, u32 type);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_80236BFC(ctx, param3, 0x1c) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0xc) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802587C0 | Size: 0x144 (324 bytes) */
s32 fightTrainerAiWazaHit082(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 fn_802367CC(void* ctx, u32 elem, void* arrA, void* arrB, u32 flag);
    extern u32 fn_802364BC(void* ctx, u32 param3);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u8 fightSeqMonomaneNGCheck(u32 elem);
    u16 buf[0x10];
    u16 count = fn_802367CC(ctx, param1, buf, 0, 0);
    u32 targetVal = fn_802364BC(ctx, param3);
    s32 gate;
    u16 i;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);
    if (fn_80236BFC(ctx, param1, 0x10) == 1 ||
        fightSeqMonomaneNGCheck(targetVal) != 0 ||
        (u16)targetVal == 0 || (u16)targetVal == 0xffff || (u16)targetVal == 0x165 || (u16)targetVal == 0x163) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        if ((u16)targetVal == buf[i]) {
            return 0;
        }
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258904 | Size: 0xB0 */
s32 fightTrainerAiWazaHit081(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802589B4 | Size: 0xB0 */
s32 fightTrainerAiWazaHit080(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x80258A64 | Size: 0x6C | Pattern: field_accessor */
u32 fightTrainerAiWazaHit079(void* ctx, u32 slot, u32 param) {
    extern u32 fn_80236BFC(void*, u32, u32);
    extern u32 fn_802373B0(void*, u32, s32, f32);
    extern f32 lbl_8047E650;
    u32 result;

    if ((fn_80236BFC(ctx, slot, 0x14) & 0xff) == 1) {
        return 0;
    }

    result = fn_802373B0(ctx, slot, -1, lbl_8047E650) & 0xff;
    return result != 1;
}

/* Address: 0x80258AD0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit078(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258B84 | Size: 0xB4 */
s32 fightTrainerAiWazaHit077(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258C38 | Size: 0xB4 */
s32 fightTrainerAiWazaHit076(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258CEC | Size: 0xB4 */
s32 fightTrainerAiWazaHit075(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258DA0 | Size: 0xB4 */
s32 fightTrainerAiWazaHit073(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258E54 | Size: 0xB4 */
s32 fightTrainerAiWazaHit072(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258F08 | Size: 0xB4 */
s32 fightTrainerAiWazaHit071(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80258FBC | Size: 0xB4 */
s32 fightTrainerAiWazaHit070(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80259070 | Size: 0xB4 */
s32 fightTrainerAiWazaHit069(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x80259124 | Size: 0xB4 */
s32 fightTrainerAiWazaHit068(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x802591D8 | Size: 0x260 (608 bytes) */
void fightTrainerAiWazaHit067(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_8023793C();
    extern void fn_80237F74();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r30 = r6;
    r28 = r4;
    r29 = r5;
    r3 = 0x2;
    r4 = r30;
    fn_801F025C();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r29;
    r5 = r28;
    fn_802395C8();
    r0 = r3;
    r3 = r27;
    r26 = r0;
    r4 = r30;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_80259328;
        }
        r3 = r27;
        r4 = r30;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r30;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_80259328:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r29;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r26;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r27 = r3;
    r3 = r31;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r27 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259438 | Size: 0x270 (624 bytes) */
u8 fightTrainerAiWazaHit066(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80237F74(void*, u32, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_80237DBC(void*, u32, u32);
    extern u8 fn_80237310(void*, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    extern u32 fn_801F025C(u32, u32);
    extern u8 fightSideIsJoutaiDataId(u32, u32);
    s32 r;
    u8 flag;

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x4) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x8) == 1) {
        return 0;
    }
    if (fn_80237310(ctx, param3) == 0) {
        return 0;
    }

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        flag = 0;
    } else {
        if (fn_80237F74(ctx, param3, 0x14) != 1) {
            if (fn_80237F74(ctx, param3, 0x7) != 1) {
                if (fn_80237F74(ctx, param3, 0xf) != 1) {
                    if (fn_80237F74(ctx, param3, 0x48) != 1) {
                        if (fn_80237F74(ctx, param3, 0x29) != 1) {
                            if (fn_80237F74(ctx, param3, 0x28) != 1) {
                                fn_80237F74(ctx, param3, 0xc);
                            }
                        }
                    }
                }
            }
        }
        flag = 1;
    }
    if (flag == 0) {
        return 0;
    }

    r = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4b) == 1) {
        return 0;
    }
    if (r == 0) {
        return 0;
    }
    if (r == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x802596A8 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit065(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x48);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x802596E4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit062(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x5, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x802597CC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit060(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x3, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x802598B4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit059(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x2, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025999C | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit058(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0xa0, 0x1, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0xa0) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259A84 | Size: 0x68 | Pattern: field_accessor */
int fightTrainerAiWazaHit057(void* ctx, u32 slot, u32 param, u32 arg3) {
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_80237288(void*, u32);

    if ((u8)fn_80236BFC(ctx, arg3, 0x10) == 1) {
        goto fail;
    }
    if ((u8)fn_80237288(ctx, arg3) != 1) {
        goto success;
    }
fail:
    return 0;
success:
    return 1;
}

/* Address: 0x80259AEC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit054(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x5, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259BD4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit053(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x4, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259CBC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit052(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x3, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259DA4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit051(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x2, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259E8C | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit050(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x20, 0x1, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x20) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x80259F74 | Size: 0x1F8 (504 bytes) */
void fightTrainerAiWazaHit049(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r31 = r6;
    r28 = r4;
    r29 = r5;
    r3 = 0x2;
    r4 = r31;
    fn_801F025C();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1 && r0 != (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x0;
            goto L_8025A0F4;
        }
        r3 = r27;
        r4 = r31;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xf;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;

        r3 = r27;
        r4 = r31;
        r5 = 0xc;
        fn_80237F74();
    }
    r0 = 0x1;
L_8025A0F4:
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r30;
    r4 = 0x4b;
    fightSideIsJoutaiDataId();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A16C | Size: 0xB4 */
s32 fightTrainerAiWazaHit048(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A220 | Size: 0x34 | Ghidra import */
int fightTrainerAiWazaHit047(void)
{
    u32 r3;
    u32 r4;
    extern u32 fn_80236BFC();
  return (fn_80236BFC(r3, r4, 0xf) & 0xFF) != 1;
}
/* Address: 0x8025A254 | Size: 0x3c | Ghidra import */
/* Address: 0x8025A254 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit046(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x4c);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x8025A340 | Size: 0xB4 */
s32 fightTrainerAiWazaHit044(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A3F4 | Size: 0xB4 */
s32 fightTrainerAiWazaHit043(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A4A8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit042(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A55C | Size: 0xB0 */
s32 fightTrainerAiWazaHit041(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025A60C | Size: 0xB0 */
s32 fightTrainerAiWazaHit040(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025A6BC | Size: 0xB4 */
s32 fightTrainerAiWazaHit039(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A770 | Size: 0x100 (256 bytes) */
s32 fightTrainerAiWazaHit038(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80237774(void* ctx, u32 elem);
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 one);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    s32 gate;
    u8 a = fn_80237774(ctx, param1);
    u8 b = fn_80237774(ctx, param3);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0xFFFF);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (fn_80237F74(ctx, param3, 5) == 1) {
        gate = 0;
    }
    if (a < b) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025A870 | Size: 0x8C */
u32 fightTrainerAiWazaHit037(void* ctx, u32 param1, u32 param2) {
    extern f32 lbl_8047E648;
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_802373B0(void*, u32, s32, f32);
    extern u8 fightTrainerAiCheckSawagu(void*, u32);

    if (fn_80236BFC(ctx, param1, 8) == 1) {
        return 0;
    }
    if (fightTrainerAiCheckSawagu(ctx, param1) == 1) {
        return 0;
    }
    return fn_802373B0(ctx, param1, 0, lbl_8047E648) != 1;
}

/* Address: 0x8025A8FC | Size: 0xB4 */
s32 fightTrainerAiWazaHit036(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025A9B0 | Size: 0x3c | Ghidra import */
u32 fightTrainerAiWazaHit035(void)

{
    extern u32 fn_801F025C();
    extern u32 fightSideCheckWriteJoutaiDataId();
  u32 uVar1;
  u32 uVar2;
  
  uVar1 = fn_801F025C(2);
  uVar2 = fightSideCheckWriteJoutaiDataId(uVar1,0x49);
  uVar2 = __cntlzw(2 - (uVar2 & 0xff));
  return uVar2 >> 5;
}
/* Address: 0x8025A9EC | Size: 0xB4 */
s32 fightTrainerAiWazaHit034(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025AAA0 | Size: 0x270 (624 bytes) */
u8 fightTrainerAiWazaHit033(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80237F74(void*, u32, u32);
    extern u8 fn_80236BFC(void*, u32, u32);
    extern u8 fn_80237DBC(void*, u32, u32);
    extern u8 fn_80237310(void*, u32);
    extern s32 _fightTrainerAiWazaHitCheck(void*, u32, u32, u32, u32);
    extern u32 fn_801F025C(u32, u32);
    extern u8 fightSideIsJoutaiDataId(u32, u32);
    s32 r;
    u8 flag;

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x4) == 1) {
        return 0;
    }
    if (fn_80237310(ctx, param3) == 0) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x3) == 1) {
        return 0;
    }
    if (fn_80237DBC(ctx, param3, 0x8) == 1) {
        return 0;
    }

    if (fn_80237F74(ctx, param3, 0x11) == 1) {
        flag = 0;
    } else {
        if (fn_80237F74(ctx, param3, 0x14) != 1) {
            if (fn_80237F74(ctx, param3, 0x7) != 1) {
                if (fn_80237F74(ctx, param3, 0xf) != 1) {
                    if (fn_80237F74(ctx, param3, 0x48) != 1) {
                        if (fn_80237F74(ctx, param3, 0x29) != 1) {
                            if (fn_80237F74(ctx, param3, 0x28) != 1) {
                                fn_80237F74(ctx, param3, 0xc);
                            }
                        }
                    }
                }
            }
        }
        flag = 1;
    }
    if (flag == 0) {
        return 0;
    }

    r = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4b) == 1) {
        return 0;
    }
    if (r == 0) {
        return 0;
    }
    if (r == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025AD10 | Size: 0x38 */
u32 fightTrainerAiWazaHit032(void* ctx, u32 slot, u32 param2, u32 param3) {
    extern f32 lbl_8047E648;
    extern u8 fn_802373B0(void*, u32, s32, f32);

    return fn_802373B0(ctx, slot, 0, lbl_8047E648) != 1;
}
/* Address: 0x8025AD48 | Size: 0xB4 */
s32 fightTrainerAiWazaHit031(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025ADFC | Size: 0x2c | Ghidra import */
u32 fightTrainerAiWazaHit030(void)

{
    extern int fightTrainerAiCheckTextureZokusei();
  u8 cVar1;
  
  cVar1 = fightTrainerAiCheckTextureZokusei();
  return cVar1 != 0;
}
/* Address: 0x8025AE28 | Size: 0xB0 */
s32 fightTrainerAiWazaHit029(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025AED8 | Size: 0xE0 (224 bytes) */
s32 fightTrainerAiWazaHit028(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_801F4354(u32 zero, u32 param3);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u16 fn_801F87CC(u32 v, void* buf);
    u8 buf[0x14];
    u32 prev = fn_801F4354(0, param3);
    s32 gate;

    if (fn_80237F74(ctx, param3, 0x15) == 1) {
        return 0;
    }
    if (fn_80236BFC(ctx, param3, 0x25) == 1) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_801F87CC(prev, buf) == 0) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025AFB8 | Size: 0xB0 */
s32 fightTrainerAiWazaHit027(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025B068 | Size: 0xB4 */
s32 fightTrainerAiWazaHit026(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025B124 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit024(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x7, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B20C | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit023(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x6, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B2F4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit020(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x3, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B3DC | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit019(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x2, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B4C4 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit018(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x90, 0x1, 0x1) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x90) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B5AC | Size: 0xB4 */
s32 fightTrainerAiWazaHit017(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025B660 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit016(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x7, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B748 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit013(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x4, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B830 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit011(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x2, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025B918 | Size: 0xE8 (232 bytes) */
s32 fightTrainerAiWazaHit010(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern s8 fightSeqCondChgActParaIdToValue(u32 id);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    extern u8 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param3, u32 param2, u32 a, u32 b, u32 c);
    s32 gate;

    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (fn_80236BFC(ctx, param3, 0x14) == 1) {
            return 0;
        }
        gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    }
    if (fightTrainerAiCheckAbiCnt(ctx, param1, param3, param2, 0x10, 0x1, 0x41) == 0) {
        return 0;
    }
    if (fightSeqCondChgActParaIdToValue(0x10) < 0) {
        if (gate == 0) {
            return 0;
        }
        if (gate == -1) {
            return 1;
        }
    }
    return 1;
}

/* Address: 0x8025BA00 | Size: 0x2c | Ghidra import */
u32 fightTrainerAiWazaHit009(void)

{
    extern u16 fightTrainerAiCheckOumu();
  u16 sVar1;
  
  sVar1 = fightTrainerAiCheckOumu();
  return sVar1 != 0;
}
/* Address: 0x8025BA2C | Size: 0xF4 (244 bytes) */
void fn_8025BA2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void _fightTrainerAiWazaHitCheck();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r31 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r31;
    r4 = r28;
    fn_802395C8();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r29;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r29;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/*
 * fightTrainerAiWazaHit008 (0x8025BA2C)
 *
 * Same WazaHitNNN veto template as fightTrainerAiWazaHit007, minus the
 * fn_801F1A6C flinch-count check: instead gates on two fn_80236BFC flag
 * checks (0x14, 0x8), then the shared _fightTrainerAiWazaHitCheck tri-state gate / 0x43
 * type-immunity check.
 */
s32 fightTrainerAiWazaHit008(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u8 fn_80236BFC(void* ctx, u32 param3, u32 flag);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) {
        return 0;
    }
    if (!fn_80236BFC(ctx, param3, 0x8)) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025BB20 | Size: 0x108 (264 bytes) */
void fn_8025BB20(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_8022967C();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void _fightTrainerAiWazaHitCheck();
    extern void fightTrainerAiCheckSimerike();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = r6;
    r5 = r27;
    r4 = r28;
    fn_802395C8();
    r30 = r3;
    r4 = r26;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r31 = r3;
    r3 = r28;
    fn_8022967C();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r31 & 0xFFFF;
        if (r0 <= (u32)0x1) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r26;
    fightTrainerAiCheckSimerike();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r26;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r26;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    _fightTrainerAiWazaHitCheck();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/*
 * fightTrainerAiWazaHit007 (0x8025BB20)
 *
 * Per-move AI evaluator gate: vetoes the move (returns 0) if any of 4
 * checks trip -- (1) already-flinched-ish tri-state via fn_8022967C +
 * fn_801F1A6C's flag-count buffer, (2) fightTrainerAiCheckSimerike(ctx), (3) fn_8023793C
 * type/immunity check == 0x43, (4) _fightTrainerAiWazaHitCheck's tri-state gate == 0.
 * Otherwise returns 1. Shares its template with the WazaHitNNN family
 * (see fn_8025BA2C aka fightTrainerAiWazaHit008, same shape minus the
 * fn_801F1A6C flinch-count check).
 */
s32 fightTrainerAiWazaHit007(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_801F1A6C(u32 zero, void* ctx, void* buf, u32 one1, u32 one2);
    extern u8 fn_8022967C(u32 param2);
    extern u8 fightTrainerAiCheckSimerike(void* ctx);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u8 buf[0x60];
    u32 v1 = fn_802395C8(ctx, param2, param1);
    u32 flinchCount = fn_801F1A6C(0, ctx, buf, 1, 1);
    s32 gate;

    if (fn_8022967C(param2) == 1 && (u16)flinchCount <= 1) {
        return 0;
    }
    if (fightTrainerAiCheckSimerike(ctx) == 1) {
        return 0;
    }
    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        return 0;
    }
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025BC28 | Size: 0xB4 */
s32 fightTrainerAiWazaHit006(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BCDC | Size: 0xB4 */
s32 fightTrainerAiWazaHit005(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BD90 | Size: 0xB4 */
s32 fightTrainerAiWazaHit004(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BE44 | Size: 0xB4 */
s32 fightTrainerAiWazaHit003(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BEF8 | Size: 0xB4 */
s32 fightTrainerAiWazaHit002(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    u32 v1 = fn_802395C8(ctx, param2, param1);
    s32 gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return gate != 0;
    }
    return 1;
}

/* Address: 0x8025BFAC | Size: 0x200 (512 bytes) */
u32 fightTrainerAiWazaHit001(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 fn_80236BFC(void* ctx, u32 a, u32 type);
    extern u8 fightTrainerAiCheckSawagu(void* ctx, u32 a);
    extern u8 fn_80237310(void* ctx, u32 a);
    extern u8 fn_80237F74(void* ctx, u32 a, u32 type);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 p1, u32 p2, u32 p3, u32 z);
    extern u32 fn_801F025C(s32 a, u32 b);
    extern u8 fightSideIsJoutaiDataId(u32 a, u32 b);
    s32 result;
    u8 ok;

    if (fn_80236BFC(ctx, param3, 0x14) == 1) return 0;
    if (fn_80236BFC(ctx, param3, 0x8) == 1) return 0;
    if (fightTrainerAiCheckSawagu(ctx, param3) == 1) return 0;
    if (fn_80237310(ctx, param3) == 0) return 0;
    if (fn_80237F74(ctx, param3, 0x11) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0x14) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0x7) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0xf) == 1 || fn_80237F74(ctx, param3, 0x48) == 1) {
        ok = 0;
        goto check;
    }
    if (fn_80237F74(ctx, param3, 0x29) == 1) goto ok1;
    if (fn_80237F74(ctx, param3, 0x28) == 1) goto ok1;
    fn_80237F74(ctx, param3, 0xc);
ok1:
    ok = 1;
check:;
    if (ok == 0) return 0;
    result = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);
    if (fightSideIsJoutaiDataId(fn_801F025C(2, param3), 0x4b) == 1) return 0;
    if (result == 0) return 0;
    if (result == -1) return 1;
    return 1;
}

/* -------------------------------------------------------------------
 * Shadow Pokemon & Purification (0x8025C000-0x80260000)
 * 89 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8025C1AC | Size: 0xB0 */
s32 fightTrainerAiWazaHit000(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 fn_802395C8(void* ctx, u32 param2, u32 param1);
    extern u32 fn_80239500(void* ctx, u32 param2);
    extern u16 fn_8023793C(void* ctx, u32 param3, u32 v1, u32 v3);
    extern s32 _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3, u32 zero);
    s32 gate;
    u32 v1 = fn_802395C8(ctx, param2, param1);
    gate = _fightTrainerAiWazaHitCheck(ctx, param1, param2, param3, 0);

    if (fn_8023793C(ctx, param3, v1, fn_80239500(ctx, param2)) == 0x43) {
        gate = 0;
    }
    if (gate == 0) {
        return 0;
    }
    if (gate == -1) {
        return 1;
    }
    return 1;
}

/* Address: 0x8025C264 | Size: 0x340 (832 bytes) */
void _fightTrainerAiWazaHitCheck(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void wazaGetStatus();
    extern void fn_801F0134();
    extern void fn_801F1D5C();
    extern void fn_801F3BB4();
    extern void fn_801F54A4();
    extern void fn_80201D84();
    extern void fn_80229934();
    extern void fn_80229B70();
    extern void fn_80229BD8();
    extern void fn_80235B04();
    extern void fn_80236BFC();
    extern void fn_80237288();
    extern void fightTrainerAiCheckGuard();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r26;
    fn_801F0134();
    r31 = r3;
    r3 = r25;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r4 = r27;
    r3 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    wazaGetStatus();
    r30 = r3 & 0xFFFF;
    if (r28 == (u32)0x0) {
        r3 = 0x1;
        return;
    }
    r7 = (u32)sp + 0x8;
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F1D5C();
    r24 = r3;
    r4 = (u32)sp + 0x8;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x0;
    fn_801F3BB4();
    r4 = (u32)sp + 0x8;
    r0 = r24 & 0xFFFF;
    r6 = 0x0;
    r7 = 0x0;
    r5 = 0x0;
    while (1) {
        r3 = r5 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        r3 = *(u32*)(r4 + r3);
        if (r3 != (u32)0x0) {
            if (r26 == (u32)r3) {
                r6 = r5;
            }
            if (r28 == (u32)r3) {
                r7 = r5;
        }
        }
        r5 = r5 + 0x1;

    }
    r3 = r6 & 0xFFFF;
    r0 = r7 & 0xFFFF;
    r0 = r3 - r0;
    r24 = (u32)r0 >> 31;

    if (r29 == (u32)0xffff || r29 == (u32)0xfffe) {

        if (r29 == (u32)0xffff) {
            r3 = r25;
            r4 = r28;
            r5 = 0x1d;
            fn_80236BFC();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r28;
                r4 = 0x1d;
                fn_80201D84();
                r3 = r3 & 0xFFFF;
                r0 = r31 & 0xFFFF;
                if (r0 == (u32)r3) {
                    r3 = -0x1;
                    return;
        }
        }
        }
        r3 = r25;
        r4 = r26;
        fn_80237288();
        r0 = r3 & 0xFF;
        if ((r0 == (u32)0x1) && (r24 == (u32)0x1)) {

            r3 = 0x0;
            return;
        }
        r3 = r25;
        r4 = r28;
        r5 = r27;
        fightTrainerAiCheckGuard();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = 0x1;
        return;
    }
    r3 = r27;
    r4 = r26;
    r5 = r28;
    fn_80229934();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = r27;
    fightTrainerAiCheckGuard();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r28;
        r4 = 0x1d;
        fn_80201D84();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if (r3 == (u32)r0) {
            r3 = -0x1;
            return;
    }
    }
    if (r24 == (u32)0x1 && r0 == (u32)0x1 && r0 != (u32)0x39 && r0 != (u32)0xfa) {
        r3 = r25;
        r4 = r28;
        r5 = 0x1f;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            if (r30 != (u32)0x92) {
                if (r30 != (u32)0x95) {
                    if (r30 != (u32)0x98) {
                        if (r30 != (u32)0xcf) {
                            r3 = 0x0;
                            return;
        }
        }
        }
        }
        }
        r3 = r25;
        r4 = r28;
        r5 = 0x20;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((r0 == (u32)0x1) && (r30 != (u32)0x93)) {

            r3 = 0x0;
            return;
        }
        r3 = r25;
        r4 = r28;
        r5 = 0x21;
        fn_80236BFC();
        r0 = r3 & 0xFF;

        r0 = r27 & 0xFFFF;

        r3 = 0x0;
        return;
    }
    r3 = r27;
    fn_80229B70();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = -0x1;
        return;
    }
    r3 = r27;
    fn_80229BD8();
    r0 = r3 & 0xFF;
    r3 = 0x1;
    if (r0 != (u32)0x1) return;
    r3 = -0x1;

    return;
}

/* Address: 0x8025C5A4 | Size: 0xD0 (208 bytes) */
s32 fightTrainerAiCheckJoutaiKieWazaHitWazaDataId(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u16 wazaGetStatus();
    u32 val = wazaGetStatus(0, param2, 9, 0) & 0xFFFF;

    if ((u16)param1 == 0x1F &&
        (val == 0x92 || val == 0x95 || val == 0x98 || val == 0xCF)) {
        return 1;
    }
    if ((u16)param1 == 0x20 && val == 0x93) {
        return 1;
    }
    if ((u16)param1 == 0x21 && ((u16)param2 == 0x39 || (u16)param2 == 0xFA)) {
        return 1;
    }
    if (val == 0x5E) {
        return 1;
    }
    return 0;
}

/* Address: 0x8025C674 | Size: 0x48 | Pattern: field_accessor */
u32 fightTrainerAiCheckHorobinouta(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F37B0();
    extern void _fightTrainerAiCheckHorobinoutaSub();
    u32 result[2];
    result[0] = (u32)ctx;
    result[1] = 0;
    fn_801F37B0(0, (u32)_fightTrainerAiCheckHorobinoutaSub, (u32)result, 0);
    return result[1] & 0xFFFF;
}

/* Address: 0x8025C6BC | Size: 0xB4 */
void fn_8025C6BC(void* ctx, u32 param1, u32 param2) {
    extern void fightTrainerIsAllyFightTargetPtr();
    extern void fightOutPokemonCheckFightOut();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r29 = r5;
    r28 = r4;
    r31 = r3;
    r30 = *(u32*)((u8*)r5 + 0x0);
    fightOutPokemonCheckFightOut();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = r28;
    fightTrainerIsAllyFightTargetPtr();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x2b;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = *(u32*)((u8*)r29 + 0x4);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r29 + 0x4) = r0;
    }
    }
    r3 = 0x1;

    return;
}

/* fightTrainerAiCheckHorobinoutaSub's counter context: field0 (id passed to the
 * 3 gate checks below) at 0x0, running counter at 0x4. */
typedef struct HorobinoutaCtx {
    u32 field0;
    u32 counter;
} HorobinoutaCtx;

/*
 * _fightTrainerAiCheckHorobinoutaSub (0x8025C6BC)
 *
 * Perish Song (Horobinouta) counter callback, invoked per-floor-slot via
 * fn_801F37B0 (see fightTrainerAiCheckHorobinouta). Runs 3 chained gate checks against
 * ctx->field0/floor/idx; only when ALL 3 fail does it increment
 * ctx->counter. Always returns 1 (the enumeration driver doesn't use the
 * return value -- the counter in ctx is the real output).
 */
#pragma optimize_for_size on
s32 _fightTrainerAiCheckHorobinoutaSub(void* floor, u32 idx, HorobinoutaCtx* ctx) {
    extern u8 fightOutPokemonCheckFightOut(void* floor, u32 idx, HorobinoutaCtx* ctx);
    extern u8 fightTrainerIsAllyFightTargetPtr(u32 fieldA, void* floor, u32 idx);
    extern u8 fn_80236BFC(u32 fieldA, void* floor, u32 flag);
    extern u8 fn_80237F74(u32 fieldA, void* floor, u32 flag);
    u32 r31;
    u32 r30;
    u32 r28;
    u32 r29;

    r29 = (u32)ctx;
    r28 = idx;
    r31 = (u32)floor;
    r30 = *(u32*)r29;
    if (!fightOutPokemonCheckFightOut((void*)r31, r28, (HorobinoutaCtx*)r29)) {
        return 1;
    }
    if (fightTrainerIsAllyFightTargetPtr(r30, (void*)r31, r28) == 1) {
        return 1;
    }
    if (fn_80236BFC(r30, (void*)r31, 0x1e) != 1 && fn_80237F74(r30, (void*)r31, 0x2b) != 1) {
        *(u32*)(r29 + 4) = *(u32*)(r29 + 4) + 1;
    }
    return 1;
}
#pragma optimize_for_size reset

/* Address: 0x8025C770 | Size: 0x98 */
u32 fightTrainerAiCheckTextureZokusei(void* ctx, u32 param1, u32 param2) {
    extern u32 fn_801FB1C0(void*, u32, u32, u32);
    extern u8 fn_8021B364(u32, void*);
    u8 out[0x10];
    u32 value;
    u32 result;

    value = fn_801FB1C0(ctx, 0, 0x43, 0) & 0xFFFF;
    value = fn_801FB1C0(0, value, 2, 0) & 0xFFFF;
    if ((u8)fn_801FB1C0(0, value, 0x2a, 0) == 1) {
        value = fn_8021B364(param1, out) & 0xFF;
        result = value >= 1;
        return result & 0xFF;
    }
    return 1;
}

/* Address: 0x8025C808 | Size: 0x2A0 (672 bytes) */
u32 fightTrainerAiCheckAbiCnt(void* ctx, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6) {
    extern void fn_801F025C();
    extern void fightSideIsJoutaiDataId();
    extern void fn_801FB1C0();
    extern void fightSeqCondChgActParaIdToValue();
    extern void fightSeqCondChgActTypeToPokemonStatusId();
    extern void fn_80229C28();
    extern void fn_80237F74();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = param4;
    u32 r8 = param5;
    u32 r9 = param6;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r0 = r9 & 0x00000040;
    r21 = r9;
    r22 = r3;
    r23 = r5;
    r24 = r6;
    r25 = r7;
    r26 = r8;
    r28 = 0x0;
    r27 = 0x0;
    if ((s32)r0 != (s32)0) {
        r30 = r4;
    } else {

        r30 = r23;
    }
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r31 = r21 & 0xFF;
    r21 = r3;
    r0 = r31 & 0xbf;
    r0 = r0 & 0x00000080;
    if ((s32)r0 != (s32)0) {
        r28 = 0x1;
    }
    r0 = r31 & 0x00000020;
    if ((s32)r0 != (s32)0) {
        r27 = 0x1;
    }
    r3 = r26;
    fightSeqCondChgActTypeToPokemonStatusId();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    r4 = 0x0;
    r5 = r29;
    r6 = 0x0;
    ((void(*)(void))pokemonGetStatus)();
    r26 = (s8)r3;
    r3 = r25;
    fightSeqCondChgActParaIdToValue();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r21;
        r4 = 0x4c;
        fightSideIsJoutaiDataId();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if (r0 == (u32)0x1) {
                r0 = r24 & 0xFFFF;
                if (r0 != (u32)0xae) {
                    r3 = 0x0;
                    return r3;
        }
        }
        }
        r0 = r24 & 0xFFFF;
        if (r0 == (u32)0xae || r0 == (u32)0x1 || r0 == (u32)0x1) {
            r0 = r27 & 0xFF;

            r3 = r22;
            r4 = 0x0;
            r5 = 0x43;
            r6 = 0x0;
            fn_801FB1C0();
            r4 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x2;
            r6 = 0x0;
            fn_801FB1C0();
            r4 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x24;
            r6 = 0x0;
            fn_801FB1C0();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = r23;
                r4 = r24;
                fn_80229C28();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x1) {
                    r0 = 0x1;
                    goto L_8025C96C;
            }
            }
            r0 = 0x0;
        L_8025C96C:
            r0 = r0 & 0xFF;

            r3 = 0x0;
            return r3;
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x1d;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x1) {
            r3 = r22;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x1 || r0 != (u32)0x1 || r0 != (u32)0xae) {
            }
            r0 = r28 & 0xFF;

            r0 = r24 & 0xFFFF;

            r3 = 0x0;
            return r3;
            }
        r3 = r22;
        r4 = r30;
        r5 = 0x33;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if (r0 == (u32)0x1) {
                r0 = r29 & 0xFFFF;
                if (r0 == (u32)0xeb) {
                    r3 = 0x0;
                    return r3;
        }
        }
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x34;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if (r0 == (u32)0x1) {
                r0 = r29 & 0xFFFF;
                if (r0 == (u32)0xe6) {
                    r3 = 0x0;
                    return r3;
        }
        }
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = r31 & 0x1F;
            if (r0 == (u32)0x1) {
                r3 = 0x0;
                return r3;
        }
        }
        r0 = (s8)r26;
        if (r0 > (u32)0x1) { r3 = 0x1; return r3; }
        r3 = 0x0;
        return r3;
    }
    if ((s32)r26 < (s32)0xc) { r3 = 0x1; return r3; }
    r3 = 0x0;
    return r3;

    r3 = 0x1;

    return r3;
}

/* Address: 0x8025CAA8 | Size: 0x94 */
u32 fightTrainerAiCheckGuard(void* ctx, u32 param1, u32 param2) {
    extern u32 fn_801FB1C0(void*, u32, u32, u32);
    extern u8 fn_80229C28(u32, u32);
    u32 value;

    value = fn_801FB1C0(ctx, 0, 0x43, 0) & 0xFFFF;
    value = fn_801FB1C0(0, value, 2, 0) & 0xFFFF;
    if ((u8)fn_801FB1C0(0, value, 0x24, 0) == 1) {
        if (fn_80229C28(param1, param2) == 1) {
            return 1;
        }
    }
    return 0;
}

/* Address: 0x8025CB3C | Size: 0xAC */
u16 fightTrainerAiCheckOumu(void* ctx, u32 param1, u32 param2) {
    extern u32 fn_800E0C54(void);
    extern u32 pokemonGetStatus(u32, u32, u32, u32);
    extern u8 fn_80201248(u32, u16*);
    u16 choices[4];
    u32 species;
    s32 random;
    s32 index;
    u8 count;

    species = pokemonGetStatus(param1, 0, 0xF7, 0) & 0xFFFF;
    if ((species != 0) && (species != 0x165) && (species != 0xFFFF)) {
        return species;
    }
    count = fn_80201248(param1, choices);
    if (count != 0) {
        random = fn_800E0C54() & 0xFFFF;
        index = random % (s32)(u8)count;
        species = choices[(u8)index];
        if ((species != 0) && (species != 0x165)) {
            return species;
        }
    }
    return 0;
}

/* Address: 0x8025CBE8 | Size: 0x48 | Pattern: field_accessor */
u32 fightTrainerAiCheckSimerike(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F37B0();
    extern void _fightTrainerAiSimerikeCheckSub();
    u32 buf[2];
    u32 r;
    u32 s;
    buf[0] = (u32)ctx;
    r = fn_801F37B0(0, (u32)_fightTrainerAiSimerikeCheckSub, (u32)buf, 0) & 0xFF;
    s = 1 - r;
    return (s != 0) ? 1 : 0;
}

/* Address: 0x8025CC30 | Size: 0x60 | Pattern: field_accessor */
u32 _fightTrainerAiSimerikeCheckSub(void* ctx, u32 slot, u32* param) {
    extern u32 fightOutPokemonCheckFightOut(void);
    extern u32 fn_80237F74(u32, void*, u32);
    u32 r31;
    u32 r30;
    u32 result;

    r30 = (u32)ctx;
    r31 = *param;
    if ((fightOutPokemonCheckFightOut() & 0xFF) == 0) {
        return 1;
    }
    result = fn_80237F74(r31, (void*)r30, 6) & 0xFF;
    return result != 1;
}

/* Address: 0x8025CC90 | Size: 0x50 | Pattern: field_accessor */
u32 fightTrainerAiCheckSawagu(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F37B0();
    extern u32 _fightTrainerAiSawaguCheckSub();
    u32 buf[2];
    u32 r;
    buf[0] = (u32)ctx;
    buf[1] = slot;
    r = fn_801F37B0(0, (u32)_fightTrainerAiSawaguCheckSub, (u32)buf, 0) & 0xFF;
    return r != 1;
}

/* Address: 0x8025CCE0 | Size: 0x84 | Pattern: field_accessor */
u32 _fightTrainerAiSawaguCheckSub(void* ctx, u32 slot, u32 param) {
    extern u32 fightOutPokemonCheckFightOut(void);
    extern u32 fn_80236BFC(u32, void*, u32);
    extern u32 fn_80237F74(u32, u32, u32);
    u32* args;
    u32 a;
    u32 b;

    args = (u32*)param;
    a = args[0];
    b = args[1];
    if ((fightOutPokemonCheckFightOut() & 0xFF) == 0) {
        return 1;
    }
    if (((fn_80236BFC(a, ctx, 0xB) & 0xFF) == 1) &&
        ((fn_80237F74(a, b, 0x2B) & 0xFF) == 0)) {
        return 0;
    }
    return 1;
}

/* Address: 0x8025CD64 | Size: 0x54 | Pattern: field_accessor */
void toolentryTaisenFreePokemonData(void* ctx, u32 slot, u32 param) {
    extern u32 lbl_8047B650;
    extern u32 fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    u32 handle;
    u32 result;
    handle = lbl_8047B650;
    if (handle != 0) {
        result = fn_800E202C(handle);
        if ((result & 0xFFFF) != 0) {
            fn_800E24B0(result);
            fn_800E209C(result);
        }
        lbl_8047B650 = 0;
    }
}

/* Address: 0x8025CDB8 | Size: 0x2B4 (692 bytes) */
void toolentryDebugPokemonCreate(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u32 lbl_80478D98;
    extern u32 lbl_8047B650;
    extern u32 lbl_8047B654;
    extern void fn_8006B09C();
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void fn_800FA280();
    extern void pokemonAllKaihuku();
    extern void pokemonSetCatchStatus();
    extern void pokemonCreate();
    extern void pokemonInit();
    extern void savedataGetStatus();
    extern void heroInit();
    extern void heroBiosGetPokemonPtr();
    extern void gamedataGetStatus();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = 0x0;
    r4 = 0x20;
    r0 = lbl_80478D98;
    lbl_8047B654 = r3;
    r3 = r0 * 0x138;
    r0 = r3 + 0x1f;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {
        fn_800E27B0();
    } else {

        r3 = 0x0;
    }
    lbl_8047B654 = r3;
    r30 = r3;
    r31 = 0x0;
    while (1) {
        r0 = lbl_80478D98;
        if ((s32)r31 >= (s32)r0) break;
        r3 = 0x0;
        r4 = 0x1;
        gamedataGetStatus();
        r0 = r31 + 0x1;
        r6 = r3;
        r3 = r30;
        r5 = 0xa;
        r4 = r0 & 0xFFFF;
        pokemonCreate();
        r3 = r31 + 0x1004;
        fn_800FA280();
        r9 = r3;
        r3 = r30;
        r4 = 0x0;
        r5 = 0x8;
        r6 = 0x1;
        r7 = 0x0;
        r8 = 0x0;
        pokemonSetCatchStatus();
        r3 = r30;
        pokemonAllKaihuku();
        r30 = r30 + 0x138;
        r31 = r31 + 0x1;

    }
    r3 = lbl_8047B650;
    if (r3 != (u32)0x0) {
        fn_800E202C();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if (r3 != (u32)0x0) {
            fn_800E24B0();
            r3 = r30;
            fn_800E209C();
        }
        r0 = 0x0;
        lbl_8047B650 = r0;
    }
    r3 = 0x80;
    r4 = 0x20;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if (r3 != (u32)0x0) {
        fn_800E27B0();
    } else {

        r3 = 0x0;
    }
    lbl_8047B650 = r3;
    r31 = 0x0;
    do {
        r3 = r31;
        fn_8006B09C();
        r3 = r3 + 0xb44;
        heroInit();
        r30 = 0x0;
        do {
            r3 = r31;
            fn_8006B09C();
            r4 = r30 & 0xFFFF;
            r3 = r3 + 0xb44;
            heroBiosGetPokemonPtr();
            pokemonInit();
            r30 = r30 + 0x1;
        } while ((s32)r30 < (s32)0x6);
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x4);
    r30 = 0x0;
    do {
        r3 = r30;
        fn_8006B09C();
        r3 = r3 + 0x2c;
        heroInit();
        r31 = 0x0;
        do {
            r3 = r30;
            fn_8006B09C();
            r4 = r31 & 0xFFFF;
            r3 = r3 + 0x2c;
            heroBiosGetPokemonPtr();
            pokemonInit();
            r31 = r31 + 0x1;
        } while ((s32)r31 < (s32)0x6);
        r30 = r30 + 0x1;
    } while ((s32)r30 < (s32)0x4);
    r0 = 0x6;
    ctr_fn = (void(*)(void))r0;
    do {
    } while (--ctr != 0);
    r3 = 0x0;
    r4 = 0x2;
    savedataGetStatus();
    r30 = r3;
    r3 = 0x0;
    fn_8006B09C();
    r3 = r3 + 0xb44;
    if (r30 != (u32)0x0) {
        r4 = r30;
        r5 = 0xb18;
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r29 = 0x0;
    r31 = 0x0;
    do {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            r4 = 0x2;
            savedataGetStatus();
            r30 = r3;
            r3 = r31;
            fn_8006B09C();
            r3 = r3 + 0xb44;
            if (r30 != (u32)0x0) {
                r4 = r30;
                r5 = 0xb18;
                memcpy((void*)r3, (const void*)r4, (u32)r5);
            }

        } else {
            r28 = 0x0;
            do {
                r0 = lbl_8047B654;
                r3 = r31;
                r30 = r0 + r29;
                fn_8006B09C();
                r4 = r28 & 0xFFFF;
                r3 = r3 + 0xb44;
                heroBiosGetPokemonPtr();
                if (r3 != (u32)0x0) {
                    r4 = r30;
                    r5 = 0x138;
                    memcpy((void*)r3, (const void*)r4, (u32)r5);
                }
                r28 = r28 + 0x1;
                r29 = r29 + 0x138;
            } while ((s32)r28 < (s32)0x6);
        }
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x4);
    r3 = lbl_8047B654;
    if (r3 != (u32)0x0) {
        fn_800E202C();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if (r3 != (u32)0x0) {
            fn_800E24B0();
            r3 = r30;
            fn_800E209C();
        }
        r0 = 0x0;
        lbl_8047B654 = r0;
    }
    return;
}

/* Address: 0x8025D06C | Size: 0x3c | Ghidra import */
u32 fn_8025D06C(void)
{
    extern u32 fn_8006ADEC();
    extern void fn_8006AFC4();
    extern void* fn_8006B5A8();
    extern void heroAddPokecoupon();
    u32 uVar1;
  fn_8006B5A8();
  fn_8006AFC4();
  uVar1 = fn_8006ADEC();
  heroAddPokecoupon(0,uVar1);
  return 0;
}


/* Address: 0x8025D0A8 | Size: 0xBC */
f32 fn_8025D0A8(void* ctx, u32 param1, u32 param2) {
    extern u32 lbl_80478EAC;
    extern f32 lbl_8047E658;
    extern f32 lbl_8047E65C;
    extern u16 fn_8011F5C8(void*);
    extern u8 pokemonCheckValid(void*);
    extern void* savedataGetStatus(u32, u32);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    u32 i;
    u32 count;
    void* member;
    void* party;
    u32 offset;
    u32 masked;
    u16 idx;
    u16 species;
    u16 entry;
    f32 scale;
    f32 factor;

    count = 0;
    if ((party = ctx) == 0) {
        party = savedataGetStatus(0, 2);
    }
    for (i = 0; (s32)i < 6; i++) {
        member = heroBiosGetPokemonPtr(party, i & 0xFFFF);
        if (pokemonCheckValid(member) != 0) {
            species = fn_8011F5C8(member);
            offset = 0;
            while (1) {
                entry = *(u16*)(lbl_80478EAC + offset);
                if (entry == 0) {
                    break;
                }
                if (species == entry) {
                    count++;
                }
                offset += 2;
            }
        }
    }
    scale = lbl_8047E658;
    factor = lbl_8047E65C;
    while ((s32)count > 0) {
        scale *= factor;
        count--;
    }
    return scale;
}

/* Address: 0x8025D164 | Size: 0x128 (296 bytes) */
void fn_8025D164(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8039A648[];
    extern u8 lbl_8039A664[];
    extern u32 lbl_80478EAC;
    extern f32 lbl_8047E658;
    extern f32 lbl_8047E65C;
    extern void fn_8006B09C();
    extern void fn_8006B5A8();
    extern void fn_8011F5C8();
    extern void pokemonCheckValid();
    extern void heroBiosGetPokemonPtr();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    u32 r4 = param1;
    u32 r5 = param2;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = 0x0;
    fn_8006B5A8();
    r27 = *(u32*)((u8*)r3 + 0xC);
    fn_8006B5A8();
    r26 = *(u32*)((u8*)r3 + 0x0);
    fn_8006B5A8();
    r30 = *(u32*)((u8*)r3 + 0x14);
    r29 = 0x0;
    do {
        r3 = 0x0;
        fn_8006B09C();
        r4 = r29 & 0xFFFF;
        r3 = r3 + 0xb44;
        heroBiosGetPokemonPtr();
        r31 = r3;
        pokemonCheckValid();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r31;
            fn_8011F5C8();
            r4 = lbl_80478EAC;
            r0 = r3 & 0xFFFF;
            r3 = 0x0;
            do {
                r5 = *(u16*)(r4 + r3);
                if (r5 == (u32)0x0) break;
                if (r0 == (u32)r5) {
                    r28 = r28 + 0x1;
                }
                r3 = r3 + 0x2;
            } while (1);
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    f1 = lbl_8047E658;
    f0 = lbl_8047E65C;
    ctr_fn = (void(*)(void))r28;
    if ((s32)r28 > (s32)0x0) {
        do {
            f1 = f1 * f0;
        } while (--ctr != 0);
    }
    if ((s32)r26 == (s32)0x1) {
        r3 = r30 + 0x1;
        r0 = 0xa;
        r0 = (s32)r3 / (s32)r0;
        if ((s32)r0 > (s32)0xa) {
            r0 = 0xa;
        }
        r3 = (u32)lbl_8039A664;
        r0 = r0 << 2;
        r3 = (u32)lbl_8039A664;
        f0 = *(f32*)(void*)(r3 + r0);
        f1 = f1 * f0;
    } else {

        if ((s32)r27 >= (s32)0x6) {
            r27 = 0x6;
        }
        r3 = (u32)lbl_8039A648;
        r0 = r27 << 2;
        r3 = (u32)lbl_8039A648;
        f0 = *(f32*)(void*)(r3 + r0);
        f1 = f1 * f0;
    }
    f0 = (f64)(s32)f1;
    *(f64*)(void*)(sp + 0x8) = f0;
    r3 = *(u32*)(sp + 0xC);
    return;
}

/* Address: 0x8025D28C | Size: 0x24 | Pattern: null_check_getter */
extern void* fn_8006B09C(void*);
u16 toolentryTaisenGetTrainerDataID(void* ctx) { return *(u16*)fn_8006B09C(ctx); }

/* Address: 0x8025D2B0 | Size: 0x24 | Pattern: null_check_getter */
u32 toolentryTaisenGetControlerType(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x24); }

/* Address: 0x8025D2D4 | Size: 0x90 */
u32 toolentryGetTrainerSamllFaceResID(void* ctx, u32 param1, u32 param2) {
    extern u32 lbl_80478E04;
    extern void* fn_8006B09C(void*);
    extern u32 fn_801FCBA4(void);
    extern void fn_801FCCC4(u32);
    u32 id;
    u32 base;
    u32 offset;
    u32* entry;
    u32 ret;

    id = *(u16*)fn_8006B09C(ctx);
    if (id == 0) {
        return 0;
    }
    fn_801FCCC4(id);
    offset = fn_801FCBA4();
    offset *= 0x14;
    base = lbl_80478E04;
    entry = (u32*)(base + offset);
    if ((s32)param1 == 0) {
        ret = entry[3];
        if (ret == 0) {
            return 0xf941200;
        }
        return ret;
    }
    ret = entry[4];
    if (ret == 0) {
        return 0xf8f1200;
    }
    return ret;
}

/* Address: 0x8025D364 | Size: 0x90 */
u32 toolentryGetTrainerBicFaceResID(void* ctx, u32 param1, u32 param2) {
    extern u32 lbl_80478E04;
    extern void* fn_8006B09C(void*);
    extern u32 fn_801FCBA4(void);
    extern void fn_801FCCC4(u32);
    u16 id16;
    u32 id;
    u32 base;
    u32 offset;
    u32* entry;
    u32 ret;

    if ((s32)ctx != 0) {
        id16 = *(u16*)fn_8006B09C(ctx);
    } else {
        id16 = *(u16*)fn_8006B09C(ctx);
    }
    id = id16;
    if (id == 0) {
        return 0;
    }
    fn_801FCCC4(id);
    offset = fn_801FCBA4();
    offset *= 0x14;
    base = lbl_80478E04;
    entry = (u32*)(base + offset);
    if ((s32)param1 == 0) {
        return entry[1];
    }
    ret = entry[2];
    if (ret == 0) {
        return 0xf991200;
    }
    return ret;
}

/* Address: 0x8025D3F4 | Size: 0x16C (364 bytes) */
void toolentryTaisenEntryPokemon(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void pokemonCheckValid();
    extern void heroBiosGetPokemonPtr();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r27 = r3;
    fn_8006B1D4();
    r30 = 0x0;
    r31 = r27;
    r29 = r30;
    r28 = r3 & 0xFFFF;
    do {
        r3 = r27;
        fn_8006B09C();
        r4 = r29 & 0xFFFF;
        r3 = r3 + 0xb44;
        heroBiosGetPokemonPtr();
        pokemonCheckValid();
        r3 = r3 & 0xFF;
        r0 = r3 - r0; /* -borrow */;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30 & 0xFFFF;
            r0 = r3 + 0x1;
            r30 = r0 & 0xFFFF;
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    r0 = r30 & 0xFFFF;
    if (r0 < r28) {
        r28 = r30;
    }
    r3 = r31;
    r30 = r28 & 0xFFFF;
    fn_8006B09C();
    r28 = r3;
    r3 = r31;
    fn_8006B09C();
    r0 = 0x163;
    r5 = r28 + 0x28;
    r4 = r3 + 0xb40;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u32*)((u8*)r4 + 0x4);
        r0 = *(u32*)((u8*)r4 + 0x8);
        *(u32*)((u8*)r5 + 0x4) = r3;
        r5 += 8; *(u32*)r5 = r0;
    } while (--ctr != 0);
    r27 = 0x0;
    r29 = 0x0;
    while ((s32)r27 < (s32)r30) {

        r3 = r31;
        fn_8006B09C();
        r0 = r29 + 0x8;
        r28 = *(u32*)(r3 + r0);
        r3 = r31;
        fn_8006B09C();
        r4 = r28 & 0xFFFF;
        r3 = r3 + 0xb44;
        heroBiosGetPokemonPtr();
        r28 = r3;
        r3 = r31;
        fn_8006B09C();
        r4 = r27 & 0xFFFF;
        r3 = r3 + 0x2c;
        heroBiosGetPokemonPtr();
        r3 = r31;
        fn_8006B09C();
        r4 = r27 & 0xFFFF;
        r3 = r3 + 0x2c;
        heroBiosGetPokemonPtr();
        if ((r3 != (u32)0x0) && (r28 != (u32)0x0)) {

            r0 = 0x27;
            ctr_fn = (void(*)(void))r0;
            do {
                r3 = *(u32*)((u8*)r4 + 0x4);
                r0 = *(u32*)((u8*)r4 + 0x8);
                *(u32*)((u8*)r5 + 0x4) = r3;
                r5 += 8; *(u32*)r5 = r0;
            } while (--ctr != 0);
        }
        r27 = r27 + 0x1;
        r29 = r29 + 0x4;

    }
    return;
}

/* Address: 0x8025D560 | Size: 0x24 | Pattern: null_check_getter */
u32 toolentryTaisengetEtnryPokemonOrderNum(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x20); }

/* Address: 0x8025D584 | Size: 0x5C | Pattern: field_accessor */
u32 toolentryTaisenDeleteEtnryPokemonOrder(void* ctx, u32 slot, u32 param) {
    typedef struct BattleFieldAccessor {
        u8 unk_00[8];
        u32 values[6];
        u32 count;
    } BattleFieldAccessor;
    extern BattleFieldAccessor* fn_8006B09C(void*);
    BattleFieldAccessor* entry;
    s32 index;

    entry = fn_8006B09C(ctx);
    index = entry->count - 1;
    if ((index < 0) || (index > 6)) {
        return 0;
    }
    entry->values[index] = (u32)-1;
    entry->count--;
    return entry->count;
}

/* Address: 0x8025D5E0 | Size: 0x64 | Pattern: field_accessor */
u32 toolentryTaisenSetEtnryPokemonOrderGBA(void* ctx, s32 count, u32* src) {
    typedef struct BattleFieldAccessor {
        u8 unk_00[8];
        u32 values[6];
        u32 count;
    } BattleFieldAccessor;
    extern BattleFieldAccessor* fn_8006B09C(void*);
    BattleFieldAccessor* dst;
    s32 i;

    dst = fn_8006B09C(ctx);
    for (i = 0; i < count; i++) {
        dst->values[i] = src[i];
    }
    dst->count = i;
    return i;
}

/* Address: 0x8025D644 | Size: 0x100 (256 bytes) */
s32 toolentryTaisenSetEtnryPokemonOrder(void* ctx, u32 order, u32 param2, u32 param3) {
    typedef struct BattleFieldAccessor {
        u8 unk_00[8];
        u32 values[6];
        u32 count;
    } BattleFieldAccessor;
    extern BattleFieldAccessor* fn_8006B09C(void*);
    extern u32 fn_8006B1D4(void*);
    extern u32 pokemonCheckValid(void*);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    BattleFieldAccessor* entry;
    u32 entryCount;
    u32 liveCount;
    u32 limit;
    u32 rawLimit;
    s32 i;
    u32 r0;
    u32 r3;

    entry = fn_8006B09C(ctx);
    entryCount = entry->count;
    rawLimit = fn_8006B1D4(entry);
    liveCount = 0;
    limit = rawLimit & 0xFFFF;
    i = liveCount;
    do {
        r3 = (u32)fn_8006B09C(ctx);
        r3 = (u32)heroBiosGetPokemonPtr((void*)(r3 + 0xb44), i & 0xFFFF);
        r3 = pokemonCheckValid((void*)r3);
        r3 = r3 & 0xFF;
        r0 = r3 != 0;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = liveCount & 0xFFFF;
            r0 = r3 + 1;
            liveCount = r0 & 0xFFFF;
        }
        i++;
    } while (i < 6);
    if ((liveCount & 0xFFFF) < limit) {
        limit = liveCount;
    }
    if ((s32)entry->count >= (s32)(limit & 0xFFFF)) {
        return -1;
    }
    for (i = 0; i < (s32)entryCount; i++) {
        if ((s32)entry->values[i] == (s32)order) {
            return -1;
        }
    }
    entry->values[entryCount] = order;
    entry->count++;
    return entryCount;
}

/* Address: 0x8025D744 | Size: 0x44 | Pattern: field_accessor */
u32 toolentryTaisenInitPokemonOrder(void* ctx, u32 slot, u32 param) {
    extern void* fn_8006B09C();
    u8* base;
    u32 i;
    base = (u8*)fn_8006B09C(ctx);
    *(u32*)(base + 0x20) = 0;
    for (i = 0; i < 6; i++) {
        *(u32*)(base + 0x8 + i * 4) = (u32)-1;
    }
    return (u32)base;
}

/* Address: 0x8025D788 | Size: 0x80 | Pattern: field_accessor */
void toolentryCopyHero(void* ctx, u32 slot, u32 param) {
    typedef struct {
        u32 words[0x2C6];
    } BattleCopyBlock;
    BattleCopyBlock* src;
    s32 i;
    BattleCopyBlock* dst;

    i = 0;
    do {
        src = (BattleCopyBlock*)((u8*)fn_8006B09C((void*)i) + 0xb44);
        dst = (BattleCopyBlock*)((u8*)fn_8006B09C((void*)i) + 0x2c);
        if ((src != NULL) && (dst != NULL)) {
            *dst = *src;
        }
        i++;
    } while ((s32)i < 4);
}

/* Address: 0x8025D808 | Size: 0x94 */
u32 toolentryTaisenGetEntryPokemonNum(void* ctx, u32 param1, u32 param2) {
    extern void* fn_8006B09C(void*);
    extern u32 fn_8006B1D4(void*);
    extern u32 pokemonCheckValid(void*);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    u32 r0;
    u32 r3;
    u32 r29;
    u32 r30;
    u32 r31;
    void* saved;

    saved = ctx;
    r3 = fn_8006B1D4(saved);
    r31 = r3 & 0xFFFF;
    r30 = 0;
    r29 = 0;
    do {
        r3 = (u32)fn_8006B09C(saved);
        r3 = (u32)heroBiosGetPokemonPtr((void *)(r3 + 0xb44), r29 & 0xFFFF);
        r3 = pokemonCheckValid((void *)r3);
        r3 = r3 & 0xFF;
        r0 = r3 != 0;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30 & 0xFFFF;
            r0 = r3 + 0x1;
            r30 = r0 & 0xFFFF;
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    r0 = r30 & 0xFFFF;
    r3 = r31;
    if (r0 < r31) {
        r3 = r30;
    }
    return r3;
}

/* Address: 0x8025D89C | Size: 0x78 | Pattern: field_accessor */
u32 toolentryTaisenGetPokemonNum(void* ctx, u32 slot, u32 param) {
    extern void* fn_8006B09C(void*);
    extern u32 pokemonCheckValid(void*);
    extern void* heroBiosGetPokemonPtr(void*, u32);
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r31 = 0;
    u32 r30 = 0;
    u32 r29 = 0;
    u32 r4 = slot;

    r30 = 0x0;
    r31 = r3;
    r29 = 0x0;
    do {
        r3 = (u32)fn_8006B09C((void *)r31);
        r4 = r29 & 0xFFFF;
        r3 = (u32)heroBiosGetPokemonPtr((void *)(r3 + 0xb44), r4);
        r3 = pokemonCheckValid((void *)r3);
        r3 = r3 & 0xFF;
        r0 = r3 != 0;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30 & 0xFFFF;
            r0 = r3 + 0x1;
            r30 = r0 & 0xFFFF;
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    return r30;
}

/* Address: 0x8025D914 | Size: 0x24 | Pattern: null_check_getter */
void* toolentryTaisenGetHeroPtr(void* ctx) { return (u8*)fn_8006B09C(ctx) + 0xb44; }

/* Address: 0x8025D938 | Size: 0x38 | Ghidra import */
u32 toolentryTaisenGetEntryPokemonPtr(u32 r3, u16 r4)
{
    extern void* fn_8006B09C();
    extern void heroBiosGetPokemonPtr();
    int iVar1;
  iVar1 = (int)fn_8006B09C();
  heroBiosGetPokemonPtr(iVar1 + 0x2c, r4);
}


/* Address: 0x8025D970 | Size: 0x38 | Ghidra import */
u32 toolentryTaisenGetPokemonPtr(u32 r3, u16 r4)
{
    extern void* fn_8006B09C();
    extern void heroBiosGetPokemonPtr();
    int iVar1;
  iVar1 = (int)fn_8006B09C();
  heroBiosGetPokemonPtr(iVar1 + 0xb44, r4);
}


/* Address: 0x8025D9A8 | Size: 0x24 | Pattern: null_check_getter */
extern void* fn_8006B5A8(void*);
u32 fn_8025D9A8(void* ctx) { return *(u32*)fn_8006B5A8(ctx); }

/* Address: 0x8025D9CC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D9CC(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x10); }

/* Address: 0x8025D9F0 | Size: 0x28 | Ghidra import */
u32 toolentryTaisenGetHomePlace(void)
{
    extern u32 fn_8006A7E8();
    extern void* fn_8006B09C();
    u16 uVar1;
  fn_8006B09C();
  uVar1 = fn_8006A7E8();
  return uVar1;
}


/* Address: 0x8025DA18 | Size: 0x24 | Pattern: accessor */
u32 toolentryTaisenGetBattlePlayerID(void* ctx) {
    extern void* fn_8006B09C(void*);
    extern u32 fn_8006A7D8(void);

    fn_8006B09C(ctx);
    return fn_8006A7D8();
}

/* toolentryTaisenGetEntryPlayerNum | Size: 0x4C | Get battle party size based on mode */
u32 toolentryTaisenGetEntryPlayerNum(void) {
    s32 mode;
    u32 res;
    extern void* fn_8006B5A8(void);
    void* result = fn_8006B5A8();
    res = 2;
    mode = *(s32*)((u8*)result + 0x4);
    switch (mode) {
        case 0:
        case 1:
            res = 2;
            break;
        case 2:
            res = 4;
            break;
    }
    return res;
}

/* Address: 0x8025DA88 | Size: 0x24 | Pattern: null_check_getter */
u32 toolentryTaisenGetBattleType(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x4); }

/* Address: 0x8025DAAC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAAC(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0xc); }

/* Address: 0x8025DAD0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAD0(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x8); }

/* Address: 0x8025DAF4 | Size: 0x38 | Ghidra import */
u32 fn_8025DAF4(void)
{
    extern void* fn_8006B5A8();
    u32 uVar1;
    uVar1 = (u32)fn_8006B5A8();
    if (*(u32 *)(uVar1 + 0x18) != 0) {
        *(u32 *)(uVar1 + 0x18) = *(u32 *)(uVar1 + 0x18) - 1;
    }
    return *(u32 *)(uVar1 + 0x18);
}


/* Address: 0x8025DB2C | Size: 0x30 | Ghidra import */
u32 fn_8025DB2C(void)
{
    extern void* fn_8006B5A8();
    int iVar1;
  iVar1 = (int)fn_8006B5A8();
  *(int *)(iVar1 + 0x18) = *(int *)(iVar1 + 0x18) + 1;
  return *(u32 *)(iVar1 + 0x18);
}


/* Address: 0x8025DB5C | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DB5C(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x18); }

/* Address: 0x8025DB80 | Size: 0x30 | Ghidra import */
u32 fn_8025DB80(void)
{
    extern void* fn_8006B5A8();
    int iVar1;
  iVar1 = (int)fn_8006B5A8();
  *(int *)(iVar1 + 0x14) = *(int *)(iVar1 + 0x14) + 1;
  return *(u32 *)(iVar1 + 0x14);
}


/* Address: 0x8025DBB0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DBB0(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x14); }

/* Address: 0x8025DBD4 | Size: 0x58 | Pattern: field_accessor */
u32 tableResBiosGetResPtr(u32 idx) {
    extern u8 lbl_8027A450[];
    extern u8 lbl_8039A690[];
    extern u32 *lbl_80478E08;
    extern u32 *lbl_80478E0C;
    extern void GSlogWrite(const char* fmt, ...);

    u32 count;
    u32* table;
    count = *lbl_80478E08;
    if (idx >= count) {
        GSlogWrite((const char*)lbl_8027A450, lbl_8039A690);
        return 0;
    }
    table = lbl_80478E0C;
    return table[idx];
}

/* Address: 0x8025DCBC | Size: 0x58 | Ghidra import */
void fn_8025DCBC(int *param)
{
    u32 *r3 = (u32 *)param;
    if (*r3 != 0) {
        fn_80165A20(*r3, 0x32, 0xff);
    }
    if (r3[1] != 0) {
        fn_801659FC(r3[1], 0x32, 0xff);
    }
}

/* Address: 0x8025DD14 | Size: 0x98 | Ghidra import */
#pragma push
#pragma use_lmw_stmw off
#pragma optimize_for_size off
void fn_8025DD14(int *r3)
{
    extern int fn_801653BC();
    extern int fn_801653C4();
    extern int fn_801656D8();
    extern void fn_80165A20();
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar1 = fn_801653C4();
    if (iVar1 != 0) {
        iVar2 = fn_801656D8();
        fn_80165A20(1, 0x32, 0xff);
    } else {
        iVar2 = 0;
    }
    iVar3 = fn_801653BC();
    if (iVar3 != 0) {
        iVar4 = fn_801656D8();
    } else {
        iVar4 = 0;
    }
    *r3 = iVar1;
    r3[1] = iVar3;
    r3[2] = iVar2;
    r3[3] = iVar4;
}
#pragma pop

/* Address: 0x8025DDAC | Size: 0x48 | Ghidra import */
void fn_8025DDAC(u32 *r3,u32 r4)
{
    extern u32 GSmodelSetRotation();
    extern u32 fn_801DAC3C(u32);
  u32 iVar1;
  r3 = (u32*)*r3;
  if (r3 == 0) return;
  iVar1 = fn_801DAC3C((u32)r3);
  if (iVar1 == 0) return;
  GSmodelSetRotation(iVar1,r4);
}

/* Address: 0x8025DDF4 | Size: 0x18 | Ghidra import */
void fn_8025DDF4(u32 *r3)

{
  if (*r3 == 0) {
    return;
  }
  *r3 = 0;
  return;
}

/* Address: 0x8025DE0C | Size: 0x48 | Ghidra import */
void fn_8025DE0C(u32 *r3,u32 r4)
{
    extern u32 GSmodelSetPosition();
    extern u32 fn_801DAC3C(u32);
  u32 iVar1;
  r3 = (u32*)*r3;
  if (r3 == 0) return;
  iVar1 = fn_801DAC3C((u32)r3);
  if (iVar1 == 0) return;
  GSmodelSetPosition(iVar1,r4);
}

/* Address: 0x8025DE54 | Size: 0xE4 | Ghidra import */
void fn_8025DE54(u32 *r3,u16 *r4,int r5,int r6,int r7,
                 int r8)

{
    extern int _threadSwitch();
    extern int fadeSet();
    extern int fn_801DA4E8();
    extern int fn_801DA8C4();
    extern int fn_801DA914();
    extern u8 fn_801DA94C();
    extern int fn_801DA9E8();
    extern int fn_801DB088();
    extern f32 lbl_8047E670;

  u16 *p;
  int iVar2;

  if (r7 == 1) {
    fadeSet((double)lbl_8047E670,2);
  }
  fn_801DA4E8(*r3,1);
  p = r4;
  for (iVar2 = 0; iVar2 < r5; iVar2 = iVar2 + 1) {
    if (r6 == 0) {
      fn_801DA914(*r3,*p,p[1]);
    }
    fn_801DA9E8(*r3,*p,p[1]);
    while (fn_801DA94C(*r3,*p,p[1]) != 0) {
      fn_801DB088();
      _threadSwitch();
    }
    if (r8 == 1) {
      fn_801DA4E8(*r3,0);
    }
    fn_801DA8C4(*r3,*p,p[1]);
    p = p + 2;
  }
  return;
}

/* Address: 0x8025DF38 | Size: 0x178 | Ghidra import */
u32 fn_8025DF38(int *r3,u32 r4,u16 *r5,int r6)

{
    extern u32 _fadeEffectGetRandom__FUl();
    extern int GSmodelSetBoundCheck();
    extern int GSmodelSetShadowLight();
    extern int GSmodelSetShadowSurface();
    extern int GSmodelClearShadowFlags();
    extern int GSmodelSetShadowFlags();
    extern u32 fn_800FF56C();
    extern int floorGetResource();
    extern int floorDataBiosGetShadowReciveNum();
    extern u32 floorDataBiosGetShadowReciveID();
    extern int floorDataBiosGetCurrentPtr();
    extern u32 fn_8018F470();
    extern u8 fn_801DDD28();
    extern int fn_801DE190();
  u32 uVar2;
  int iVar3;
  u8 cVar8;
  int iVar4;
  u32 iVar5;
  u32 uVar6;
	  u32 uVar7;
	  int iVar9;
	  int local_68 [13];
  
	  uVar2 = _fadeEffectGetRandom__FUl(0xffffffff);
	  iVar3 = fn_801DE190(r4,uVar2,0);
  *r3 = iVar3;
  if ((u32)iVar3 == 0) {
    uVar2 = 0;
  }
  else {
    for (iVar3 = 0; iVar3 < r6; iVar3 = iVar3 + 1) {
      cVar8 = fn_801DDD28(*r3,*r5,r5[1],0);
      if (cVar8 == '\0') {
        return 0;
      }
      r5 = r5 + 2;
    }
    if (((r3 != (int *)0x0) && (*(u32 *)r3 != 0)) && (iVar3 = fn_801DAC3C(), iVar3 != 0)) {
      GSmodelClearShadowFlags(iVar3,1);
      iVar4 = floorDataBiosGetCurrentPtr();
      if (iVar4 != 0) {
        uVar2 = fn_8018F470(1);
        iVar5 = floorDataBiosGetShadowReciveNum(iVar4);
	        iVar9 = 0;
	        if (iVar5 == 1) {
	          for (uVar7 = 0; uVar7 < iVar5; uVar7 = uVar7 + 1) {
	            uVar6 = floorDataBiosGetShadowReciveID(iVar4,uVar7);
	            uVar6 = floorGetResource(fn_800FF56C(),uVar6);
	            if (uVar6 != 0) {
	              local_68[iVar9] = uVar6;
              iVar9 = iVar9 + 1;
            }
          }
          GSmodelSetShadowFlags(iVar3,1);
          GSmodelSetShadowLight(iVar3,uVar2);
          GSmodelSetShadowSurface(iVar3,iVar9,local_68);
          GSmodelSetBoundCheck(iVar3,1);
        }
      }
    }
    uVar2 = 1;
  }
  return uVar2;
}

/* Address: 0x8025E0B0 | Size: 0x10C | Ghidra import */
u32 loadSequence(int *r3,u32 r4,u16 *r5,int r6)

{
    extern u32 fn_8011F5B0();
  u32 uVar1;
  u32 uVar2;
  u16 sVar4;
  u16 sVar6;
  u32 iResult;
  int iVar3;
  u8 cVar5;

  sVar4 = (int)pokemonGetStatus(r4,0,0x6e,0);
  if (sVar4 == 0) {
    uVar1 = 0;
  }
  else {
    sVar6 = (int)pokemonGetStatus(0,sVar4,0x66,0);
    if (sVar6 == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = fn_8011F5B0(r4);
      uVar2 = (int)pokemonGetStatus(r4,0,0xc1,0);
      uVar2 = (-uVar2 | uVar2) >> 0x1f;
      iResult = fn_801DE190(sVar6,uVar1,uVar2);
      *r3 = iResult;
      if (iResult == 0) {
        uVar1 = 0;
      }
      else {
        u16 *p = r5;
        for (iVar3 = 0; iVar3 < r6; iVar3 = iVar3 + 1) {
          cVar5 = fn_801DDD28(*r3,*p,p[1],0);
          if (cVar5 == '\0') {
            return 0;
          }
          p = p + 2;
        }
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8025E1BC(void) {
#include "src/game/colosseum_battle_fn_8025E1BC.inc"
}
#endif
#pragma pop
static inline u32 *validptr_8025E1BC(u32 *p) {

  u32 *q = p;

  if (q != (u32 *)0 && *q != 0) return q;

  return (u32 *)0;

}

#pragma push
#pragma optimize_for_size off
void fn_8025E1BC(float f1, float f2, float f3, float f4, float f5, float f6) {
  extern u32 lbl_8047B658;
  extern u16 lbl_80478DA0;
  extern f32 lbl_8047E678;
  extern f32 lbl_8047E67C;
  extern void fn_8025DDF4();
  extern u8 fn_8025DF38();
  extern void fn_8025DE0C();
  extern void fn_8025DDAC();
  extern void fn_8025DE54();
  extern void fn_800E01F4();
  u32 *r31;
  u32 *r30;
  float out_buf[3];
  float scale_buf[3];
  float sv1, sv2, sv3, sv4, sv5, sv6;
  float fscale;
  sv1 = f1; sv2 = f2; sv3 = f3;
  sv4 = f4; sv5 = f5; sv6 = f6;
  fn_801DADC0(1);
  fn_8025DDF4(&lbl_8047B658);
  if ((u8)(fn_8025DF38(&lbl_8047B658, 0x3f, &lbl_80478DA0, 1) == 1) == 1) {
    r31 = validptr_8025E1BC(&lbl_8047B658);
    r30 = validptr_8025E1BC(&lbl_8047B658);
    if (r30 != (u32 *)0 && *r30 != 0) {
      fn_800E01F4(sv1, out_buf, sv2, sv3);
      fn_8025DE0C(r30, (u32)out_buf);
    }
    {
      u32 *r3;
      r3 = validptr_8025E1BC(&lbl_8047B658);
      if (r3 != (u32 *)0 && *r3 != 0) {
        fscale = lbl_8047E678;
        scale_buf[0] = fscale * sv4;
        scale_buf[1] = fscale * sv5;
        scale_buf[2] = fscale * sv6;
        fn_8025DDAC(r3, (u32)scale_buf);
      }
    }
    if (r31 != (u32 *)0 && *r31 != 0) {
      fn_8025DE54(r31, &lbl_80478DA0, 1, 0, 0, 1);
      fadeSet(5, lbl_8047E67C);
      fadeCheck(1);
    }
  }
  fn_801DAC90();
}
#pragma pop

/* Address: 0x8025E534 | Size: 0xC0 | Ghidra import */
void _preReliveDisplayHokoraParticleThreadFunc__Fv(void)
{
    extern u32 lbl_8027A478[];
    extern u32 GSresGetResource();
    extern u32 fn_80113F48();
    extern int fn_80118A68();
    extern int fn_80118F04();
    extern u32 fn_801190DC();
    extern int waitTime__Ff();
    extern u32 lbl_8047B660;
    extern u32 lbl_8047B664;
    extern f32 lbl_8047E684;
    float fVar1;
    u32 uVar2;
    u32 state;
    u32 local_buf[3];

    local_buf[0] = lbl_8027A478[0];
    local_buf[1] = lbl_8027A478[1];
    local_buf[2] = lbl_8027A478[2];
    state = lbl_8047B664;
    if ((s32)state != 1) goto skip_cleanup;
    if ((s32)state != 1) goto skip_cleanup;
    fn_80118A68(lbl_8047B660, 1);
    lbl_8047B660 = 0;
    lbl_8047B664 = 0;
skip_cleanup:
    uVar2 = fn_80113F48();
    uVar2 = GSresGetResource(uVar2, 0x108a1400);
    uVar2 = fn_801190DC(uVar2, 0, 0);
    lbl_8047B660 = uVar2;
    fn_80118F04(uVar2, local_buf);
    fVar1 = lbl_8047E684;
    lbl_8047B664 = 1;
    waitTime__Ff(fVar1);
    if ((s32)lbl_8047B664 == 1) {
        fn_80118A68(lbl_8047B660, 1);
        lbl_8047B660 = 0;
        lbl_8047B664 = 0;
    }
}

/* Address: 0x8025E5F4 | Size: 0x4C | Ghidra import */
void preReliveDisplayHokoraParticle(void)
{
    extern int GSthreadCreate();
    extern u32 fn_800FF560();
    extern void _preReliveDisplayHokoraParticleThreadFunc__Fv();
    extern u32 scriptGetDarkPointZeroPokemonNum();
  u32 uVar1;
  u32 uVar2;

  uVar1 = scriptGetDarkPointZeroPokemonNum();
  if ((uVar1 & 0xFFFF) != 0) {
    uVar2 = fn_800FF560();
    GSthreadCreate(1, uVar2, 0x4000, 1, 1, (u32)_preReliveDisplayHokoraParticleThreadFunc__Fv);
  }
}

/* Address: 0x8025E640 | Size: 0x37C | Ghidra import */
u32 _expRecover__FP7PokemonUl(u32 r3,int r4)

{
    extern int winMsgCloseLevelUpStatus();
    extern int winMsgOpenLevelUpStatus();
    extern int winMsgOpenField();
    extern int fn_8011DE98();
    extern u32 fn_8011F4A8();
    extern int pokemonGetFriendFormPokemonFriendFilterId();
    extern u32 pokemonGetLevelToExp();
    extern u32 pokemonGetSoubiItemSoubiDataId();
    extern u32 pokemonGetOboeWazaDataId();
    extern s8 pokemonSearchWazaDataId();
    extern int pokemonWazaCreate();
    extern int pokemonResetBasisStatus();
    extern int fn_80132A38();
    extern int fn_80165668();
    /* evolutionWazaLearn forward-declared at file scope */
  u32 bVar11;
  int iVar1;
  u32 uVar2;
  u32 uVar3;
  s8 cVar12;
  u32 uVar4;
  u32 uVar13;
  u8 local_57[3];
  u8 local_54[4];
  u8 local_44[4];
  u8 local_58[4];
  
  uVar13 = 0;
  local_58[0] = '\0';
  pokemonGetStatus(r3,0,0x7a,0);
  while (1) {
    if (r4 == 0) {
      pokemonSetStatus(r3,0,0xc6,0,0);
      return uVar13;
    }
    bVar11 = (u8)pokemonGetStatus(r3,0,0x7a,0);
    if (bVar11 > 99) break;
    iVar1 = (int)pokemonGetStatus(r3,0,0x79,0);
    uVar2 = pokemonGetLevelToExp(r3,bVar11 + 1);
    pokemonGetStatus(r3,0,0x87,0);
    pokemonGetStatus(r3,0,0x88,0);
    pokemonGetStatus(r3,0,0x89,0);
    pokemonGetStatus(r3,0,0x8a,0);
    pokemonGetStatus(r3,0,0x8b,0);
    pokemonGetStatus(r3,0,0x8c,0);
    if ((u32)(iVar1 + r4) >= uVar2) {
      r4 = (iVar1 + r4) - uVar2;
      uVar13 = 1;
      fn_8011DE98(r3,uVar2);
      pokemonResetBasisStatus(r3);
      uVar3 = pokemonGetSoubiItemSoubiDataId(r3);
      pokemonGetFriendFormPokemonFriendFilterId(r3,uVar3,0);
      uVar2 = fn_8011F4A8(r3);
      fn_80165668(0x4ca,0,0xff);
      fn_80132A38(0x2f,uVar2 & 0xff);
      winMsgOpenField(0x44ce,1,0);
      pokemonGetStatus(r3,0,0x87,0);
      pokemonGetStatus(r3,0,0x88,0);
      pokemonGetStatus(r3,0,0x89,0);
      pokemonGetStatus(r3,0,0x8a,0);
      pokemonGetStatus(r3,0,0x8b,0);
      pokemonGetStatus(r3,0,0x8c,0);
      local_44[0] = 1;
      winMsgOpenLevelUpStatus(local_44,1);
      local_54[0] = 0;
      winMsgOpenLevelUpStatus(local_54,1);
      winMsgCloseLevelUpStatus(1);
      local_58[0] = '\0';
      while (uVar4 = pokemonGetOboeWazaDataId(r3,uVar2,local_58), (uVar4 & 0xffff) != 0) {
        cVar12 = pokemonSearchWazaDataId(r3,uVar4);
        if ((cVar12 == -1) &&
           (iVar1 = evolutionWazaLearn(r3,uVar4,local_57,0,0x8025e3b0,0), iVar1 != 0)) {
          pokemonWazaCreate(r3,local_57[0],uVar4);
        }
        local_58[0] = local_58[0] + 1;
      }
    }
    else {
      r4 = 0;
      pokemonSetStatus(r3,0,0x79,0);
    }
  }
  return uVar13;
}

#pragma push
#pragma optimize_for_size off
/* Address: 0x8025EF58 | Size: 0x354 | Ghidra import */

void preReliveMain(void)

{
    extern u32 _DAT_804782bc;
    extern u32 _DAT_804782c0;
    extern u32 _DAT_804782c4;
    extern u32 menuPokemonOpen();
    extern int fn_800FF730();
    extern int winMsgCloseField();
    extern int fn_8011288C();
    extern int fn_8011EE40();
    extern s8 heroItemCheckHaveItemDataId();
    extern u32 heroGetStatus();
    extern int fadeCheck();
    extern s8 fn_801EEC74();
    extern u32 lbl_8047B660;
    extern u32 lbl_8047B664;
    extern u32 lbl_8047B668;
    extern u32 lbl_8047B66C;
    extern f32 lbl_8047E680;
    extern f32 lbl_8047E68C;
  int iVar1;

  u32 uVar2;
  u32 uVar3;
  short sVar5;
  s8 cVar7;
  short sVar6;
  u32 uVar4;

  u16 uVar8;
  u8 auStack_28 [24];
  
  if ((int)lbl_8047B668 != -1) {
    sVar6 = 0;
    savedataGetStatus(0,0);
    uVar2 = savedataGetStatus(0,2);
    uVar8 = 0;
    while (1) {
      if (5 < uVar8) break;
      uVar3 = heroBiosGetPokemonPtr(uVar2,uVar8);
      cVar7 = pokemonCheckValid();
      if (cVar7 != '\0') {
        cVar7 = pokemonIsDarkPokemon(uVar3);
        if (cVar7 == '\x01') {
          fn_8011EE40(uVar3);
          cVar7 = fn_801EEC74();
          if (cVar7 == '\0') {
            sVar6 = sVar6 + 1;
          }
        }
      }
      uVar8 = uVar8 + 1;
    }
    sVar5 = scriptGetDarkPointZeroPokemonNum();
    savedataGetStatus(0,0);
    savedataGetStatus(0,2);
    cVar7 = heroItemCheckHaveItemDataId(0,0x219);
    if (cVar7 == '\x01') {
      if (sVar6 == 0) {
        iVar1 = 3;
      }
      else if (sVar5 == 0) {
        iVar1 = 1;
      }
      else {
        iVar1 = 2;
      }
    }
    else if (sVar5 == 0) {
      iVar1 = 5;
    }
    else {
      iVar1 = 4;
    }
    if (lbl_8047B668 == 0) {
      if (iVar1 == 1) {
        iVar1 = 5;
      }
      else if (iVar1 == 2) {
        iVar1 = 4;
      }
      else if (iVar1 == 3) {
        iVar1 = 5;
      }
    }
    else if (lbl_8047B668 == 1) {
      iVar1 = 2;
    }
    if (iVar1 == 2) {
      fn_8025DD14((int*)auStack_28);
      fn_80165668(0x3c8,0,0xff);
      uVar2 = heroGetStatus(0,3,lbl_8047B66C & 0xffff);
      _DAT_804782c0 = (int)pokemonGetStatus(uVar2,0,0x6e,0);
      _DAT_804782bc = 1;
      _DAT_804782c4 = lbl_8047B66C;
      fadeSet((double)lbl_8047E680,3);
      fadeCheck(1);
      fn_800FF730(0x385);
      fn_8011288C(0,0);
      _threadSwitch();
      fadeSet((double)lbl_8047E68C,2);
      fadeCheck(1);
      fn_8025DCBC((int*)auStack_28);
    }
    else if (iVar1 == 4) {
      if (lbl_8047B664 == 1) {
        fn_80118A68(lbl_8047B660,1);
        lbl_8047B660 = 0;
        lbl_8047B664 = 0;
      }
      sVar6 = scriptGetDarkPointZeroPokemonNum();
      if (sVar6 == 0) {
        lbl_8047B668 = 0xffffffff;
        ((u32*)&lbl_8047B668)[1] = 0xffffffff;
      }
      else {
        winMsgOpenField(0x3b0f,1,0);
        winMsgCloseField(1);
        uVar4 = menuPokemonOpen(7,0,0);
        if (uVar4 != 0xffffffff) {
          uVar2 = heroGetStatus(0,3,uVar4 & 0xffff);
          _DAT_804782c0 = (int)pokemonGetStatus(uVar2,0,0x6e,0);
          _DAT_804782bc = 0;
          _DAT_804782c4 = uVar4;
          fadeSet((double)lbl_8047E680,3);
          fadeCheck(1);
          fn_8025DD14((int*)auStack_28);
          fn_800FF730(0x385);
          fn_8011288C(0,0);
          _threadSwitch();
          fadeSet((double)lbl_8047E68C,2);
          fadeCheck(1);
          fn_8025DCBC((int*)auStack_28);
        }
      }
    }
    else {
      lbl_8047B668 = 0xffffffff;
      ((u32*)&lbl_8047B668)[1] = 0xffffffff;
    }
  }
  return;
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void preReliveInit(void) {
#include "src/game/colosseum_battle_fn_8025F2AC.inc"
}
#else
#pragma optimization_level 4
void preReliveInit(void) {
  u32 *p = &lbl_8047B668;
  p[0] = 0xFFFFFFFF;
  p[1] = 0xFFFFFFFF;
}
#endif
#pragma pop

/* Address: 0x8025F2C0 | Size: 0x3C | Ghidra import */
void preReliveSetParameter(int r3,u32 r4)

{
    extern u32 lbl_8047B668;
    u32 *p = &lbl_8047B668;
    p[0] = r3;
    p[1] = r4;
  if ((r3 == 0) || (r3 == 1)) {
    preReliveMain();
  }
  return;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void ShortCommandProc(void) {
#include "src/game/colosseum_battle_fn_8025F2FC.inc"
}
#endif
#pragma pop
void ShortCommandProc(int r3) {
  u8 *entry;
  u8 b7;
  entry = lbl_804783E0 + (r3 * 0x100);
  if (*(s32 *)(entry + 0x20) != 0) return;
  if ((*(u8 *)(entry + 0x5) != 0) || (*(u8 *)(entry + 0x6) != 4)) {
    *(s32 *)(entry + 0x20) = 1;
    return;
  }
  b7 = *(u8 *)(entry + 0x7);
  *(u8 *)(*(u32 *)(entry + 0x14)) = b7 & 0x3a;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8025F350(void) {
#include "src/game/colosseum_battle_fn_8025F350.inc"
}
#endif
#pragma pop
void GBAInit(void) {
  u32 r30;
  u8 *r29;
  u8 *r28;
  int i;
  u32 r0;
  r0 = *(u32 *)0x800000F8;
  r0 = r0 >> 2;
  r0 = __mulhwu(0x431BDE83u, r0);
  r0 = r0 >> 15;
  r0 = r0 * 60;
  r30 = r0 >> 3;
  r29 = lbl_804783E0;
  r28 = lbl_804782E0;
  for (i = 0; i < 4; i++) {
    *(u32 *)(r29 + 0x34) = r30;
    *(u32 *)(r29 + 0x30) = 0;
    OSInitThreadQueue((void *)(r29 + 0x24));
    *(u32 *)(r29 + 0xF8) = (u32)r28;
    r29 += 0x100;
    r28 += 0x40;
  }
  OSInitAlarm();
  DSPInit();
  lbl_8047B670 = 0;
  OSRegisterResetFunction(lbl_8039A6B8);
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GBAGetStatus(void) {
#include "src/game/colosseum_battle_fn_8025F3F4.inc"
}
#endif
#pragma pop
#pragma push
#pragma peephole off
u32 GBAGetStatus(int r3, u32 r4) {
  int idx;
  u8 *entry;
  s32 result;
	  idx = r3;
	  entry = lbl_804783E0 + idx * 0x100;
  if (*(u32 *)(entry + 0x1C) != 0) {
    result = 2;
  } else {
    *(u8 *)(entry + 0x0) = 0;
    *(u32 *)(entry + 0x14) = r4;
    *(u32 *)(entry + 0x1C) = (u32)__GBASyncCallback;
    result = __GBATransfer(idx, 1, 3, (u32)ShortCommandProc);
  }
  if (result == 0) {
    result = __GBASync(idx);
  }
  return result;
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GBAReset(void) {
#include "src/game/colosseum_battle_fn_8025F484.inc"
}
#endif
#pragma pop
#pragma push
#pragma peephole off
u32 GBAReset(int r3, u32 r4) {
  int idx;
  u8 *entry;
  s32 result;
  idx = r3;
  entry = lbl_804783E0 + idx * 0x100;
  if (*(u32 *)(entry + 0x1C) != 0) {
    result = 2;
  } else {
    *(u8 *)(entry + 0x0) = 0xFF;
    *(u32 *)(entry + 0x14) = r4;
    *(u32 *)(entry + 0x1C) = (u32)__GBASyncCallback;
    result = __GBATransfer(idx, 1, 3, (u32)ShortCommandProc);
  }
  if (result == 0) {
    result = __GBASync(idx);
  }
  return result;
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void OnReset(void) {
#include "src/game/colosseum_battle_fn_8025F514.inc"
}
#endif
#pragma pop
#pragma scheduling off
u32 OnReset(void) {
  lbl_8047B670 = 1;
  return 1;
}
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

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void WriteProc(void) {
#include "src/game/colosseum_battle_fn_8025F618.inc"
}
#endif
#pragma pop
void WriteProc(int r3) {
  u8 *entry;
  entry = lbl_804783E0 + r3 * 0x100;
  if (*(s32 *)(entry + 0x20) != 0) return;
  *(u8 *)(*(u32 *)(entry + 0x14)) = *(u8 *)(entry + 0x5) & 0x3A;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8025F648(void) {
#include "src/game/colosseum_battle_fn_8025F648.inc"
}
#endif
#pragma pop
#pragma push
#pragma peephole off
#pragma optimize_for_size off
u32 GBAWrite(int r3, u32 r4, u32 r5) {
  int idx;
  u32 r29;
  u32 r30;
  u8 *entry;
  s32 result;
  idx = r3;
  r29 = r4;
  r30 = r5;
  entry = lbl_804783E0 + idx * 0x100;
  if (*(u32 *)(entry + 0x1C) != 0) {
    result = 2;
  } else {
    *(u8 *)(entry + 0x0) = 0x15;
    memcpy(entry + 1, (void *)r29, 4);
    *(u32 *)(entry + 0x18) = r29;
    *(u32 *)(entry + 0x14) = r30;
    *(u32 *)(entry + 0x1C) = (u32)__GBASyncCallback;
    result = __GBATransfer(idx, 5, 1, (u32)WriteProc);
  }
  if (result == 0) {
    result = __GBASync(idx);
  }
  return result;
}
#pragma pop

#pragma push
#pragma peephole off
/* Address: 0x8025F70C | Size: 0xDC | Ghidra import */
void __GBAHandler(int chan, u32 status, void* currentContext)

{
    extern void OSClearContext(void*);
    extern void OSSetCurrentContext(void*);
  u8* entry;
  void (*callback1)(int, u32);
  u8 context[0x2d0];

  entry = lbl_804783E0 + chan * 0x100;
  if (lbl_8047B670 != 0) {
    return;
  }
  if ((status & 0xf) != 0) {
    *(u32 *)(entry + 0x20) = 1;
  }
  else {
    *(u32 *)(entry + 0x20) = 0;
  }
  if (*(u32 *)(entry + 0x38) != 0) {
    void (*callback0)(int) = *(void (**)(int))(entry + 0x38);
    *(u32 *)(entry + 0x38) = 0;
    callback0(chan);
  }
  if (*(u32 *)(entry + 0x1c) != 0) {
    OSClearContext(context);
    OSSetCurrentContext(context);
    callback1 = *(void (**)(int, u32))(entry + 0x1c);
    *(u32 *)(entry + 0x1c) = 0;
    callback1(chan, *(u32 *)(entry + 0x20));
    OSClearContext(context);
    OSSetCurrentContext(currentContext);
  }
}
#pragma pop

/* Address: 0x8025F7E8 | Size: 0x34 | Ghidra import */
#pragma peephole off
void __GBASyncCallback(int r3)

{
    extern void OSWakeupThread(void *);
  OSWakeupThread(lbl_804783E0 + r3 * 0x100 + 0x24);
}
#pragma peephole on

/* Address: 0x8025F81C | Size: 0x6C | Ghidra import */
#pragma push
#pragma optimize_for_size off
u32 __GBASync(int r3)

{
  extern u32 OSDisableInterrupts(void);
  extern void OSRestoreInterrupts(u32);
  extern void OSSleepThread(void *);
  u8 *entry;
  u32 interrupts;
  u32 status;

  entry = lbl_804783E0 + r3 * 0x100;
  interrupts = OSDisableInterrupts();
  while (*(u32 *)(entry + 0x1c) != 0) {
    OSSleepThread(entry + 0x24);
  }
  status = *(u32 *)(entry + 0x20);
  OSRestoreInterrupts(interrupts);
  return status;
}
#pragma pop

/* Address: 0x8025F888 | Size: 0x124 | Ghidra import */
void TypeAndStatusCallback(int r3,u32 r4)

{
  extern void *OSGetCurrentContext(void);
  extern void OSClearContext(void *);
  extern void OSSetCurrentContext(void *);
  extern void __OSReschedule(void);
  extern int SITransfer(int, void *, u32, void *, u32, u32, u32, u32);
  u8 *entry;
  void (*callback1)(int, u32);
  void *currentContext;
  u8 context[0x2d0];

  entry = lbl_804783E0 + r3 * 0x100;
  if (lbl_8047B670 == 0) {
    if (((r4 & 0xff) != 0) || ((r4 & 0xffff0000) != 0x40000)) {
      *(u32 *)(entry + 0x20) = 1;
    }
    else {
      if (SITransfer(r3, entry, *(u32 *)(entry + 0xc),
                     entry + 0x5, *(u32 *)(entry + 0x10),
                     (u32)__GBAHandler, *(u32 *)(entry + 0x30),
                     *(u32 *)(entry + 0x34)) != 0) {
        return;
      }
      *(u32 *)(entry + 0x20) = 2;
    }
    if (*(u32 *)(entry + 0x38) != 0) {
      void (*callback0)(int) = *(void (**)(int))(entry + 0x38);
      *(u32 *)(entry + 0x38) = 0;
      callback0(r3);
    }
    if (*(u32 *)(entry + 0x1c) != 0) {
      currentContext = OSGetCurrentContext();
      OSClearContext(context);
      OSSetCurrentContext(context);
      callback1 = *(void (**)(int, u32))(entry + 0x1c);
      *(u32 *)(entry + 0x1c) = 0;
      callback1(r3, *(u32 *)(entry + 0x20));
      OSClearContext(context);
      OSSetCurrentContext(currentContext);
      __OSReschedule();
    }
  }
}

/* Address: 0x8025F9AC | Size: 0x74 | Ghidra import */
u32 __GBATransfer(int r3,u32 r4,u32 r5,u32 r6)

{
  extern u32 OSDisableInterrupts(void);
  extern void OSRestoreInterrupts(u32);
  extern int SIGetTypeAsync();
  u32 chan;
  u32 sendBytes;
  u32 recvBytes;
  u32 callback;
  u8 *entry;
  u32 interrupts;

  chan = r3;
  sendBytes = r4;
  recvBytes = r5;
  callback = r6;
  entry = lbl_804783E0 + chan * 0x100;
  interrupts = OSDisableInterrupts();
  *(u32 *)(entry + 0x38) = callback;
  *(u32 *)(entry + 0xc) = sendBytes;
  *(u32 *)(entry + 0x10) = recvBytes;
  SIGetTypeAsync(chan,(u32)TypeAndStatusCallback);
  OSRestoreInterrupts(interrupts);
  return 0;
}

/* Address: 0x8025FA20 | Size: 0x1AC | Ghidra import */
void memoGetScaleAngle(u16 r3, f32 *r4, f32 *r5)

{
    extern f32 lbl_8047E690;
    extern f32 lbl_8047E694;
    extern f32 lbl_8047E698;
    extern f32 lbl_8047E69C;
    extern f32 lbl_8047E6A0;
    extern f32 lbl_8047E6A4;
    extern f32 lbl_8047E6A8;
    extern f32 lbl_8047E6AC;
  s32 id;
  f32 fVar1;

  id = (u16)r3;
  fVar1 = lbl_8047E690;
  if (id != 0x92) {
    if (id < 0x92) {
      if (id != 0x4a) {
        if (id < 0x4a) {
          if (id != 0x26) {
            if (id < 0x26) {
              if (id != 0x1a) {
                if ((id < 0x1a) && (id == 6)) {
                  fVar1 = lbl_8047E6A0;
                }
              }
              else {
                fVar1 = lbl_8047E6A0;
              }
            }
            else if (id == 0x44) {
              fVar1 = lbl_8047E6A8;
            }
          }
          else {
            fVar1 = lbl_8047E694;
          }
        }
        else if (id != 0x8e) {
          if (id < 0x8e) {
            if (id != 0x85) {
              if ((id < 0x85) && (id < 0x4c)) {
                fVar1 = lbl_8047E698;
              }
            }
            else {
              fVar1 = lbl_8047E698;
            }
          }
          else if (id == 0x90) {
            fVar1 = lbl_8047E6A4;
          }
          else if (0x8f < id) {
            fVar1 = lbl_8047E69C;
          }
        }
        else {
          fVar1 = lbl_8047E69C;
        }
      }
      else {
        fVar1 = lbl_8047E698;
      }
    }
    else if (id == 0x136) {
      fVar1 = lbl_8047E69C;
    }
    else if (id < 0x136) {
      if (id == 0xfa) {
        fVar1 = lbl_8047E698;
      }
      else if (id < 0xfa) {
        if (id == 0xe2) {
          fVar1 = lbl_8047E69C;
        }
        else if ((id < 0xe2) && (id == 0xd9)) {
          fVar1 = lbl_8047E69C;
        }
      }
      else if (id == 300) {
        fVar1 = lbl_8047E6A0;
      }
    }
    else if (id == 0x16b) {
      fVar1 = lbl_8047E698;
    }
    else if (id < 0x16b) {
      if (id == 0x14b) {
        fVar1 = lbl_8047E6A8;
      }
    }
    else if (id == 0x198) {
      fVar1 = lbl_8047E6A4;
    }
    else if ((id < 0x198) && (0x196 < id)) {
      fVar1 = lbl_8047E6A4;
    }
  }
  else {
    fVar1 = lbl_8047E6A4;
  }
  if (r5 != (f32 *)0x0) {
    *r5 = lbl_8047E6AC;
  }
  if (r4 != (f32 *)0x0) {
    *r4 = fVar1;
    return;
  }
  return;
}

/* Address: 0x8025FBCC | Size: 0x168 | Ghidra import */
void memoInitDebug(int r3)

{
  u16 *puVar1;
  u32 uVar2;
  u32 uVar3;
  u16 uVar4;
  
  if (r3 == 0) {
    savedataGetStatus(0,0xc);
  }
  for (uVar4 = 1; uVar4 < 0xfc; uVar4 = uVar4 + 1) {
    puVar1 = (u16 *)savedataGetStatus(0,0xc);
    for (uVar3 = 0; (uVar3 & 0xffff) < (u32)*puVar1; uVar3 = uVar3 + 1) {
      if ((puVar1[(uVar3 & 0xffff) * 6 + 2] & 0x3fff) != uVar4) {
        }
        puVar1[(u32)*puVar1 * 6 + 2] = uVar4 | 0x8000;
        uVar2 = _fadeEffectGetRandom__FUl(0xffffffff);
        *(u32 *)(puVar1 + (u32)*puVar1 * 6 + 6) = uVar2;
        *puVar1 = *puVar1 + 1;
      }
  }
  uVar4 = 0x115;
  do {
    if (0x19b < uVar4) {
      return;
    }
    puVar1 = (u16 *)savedataGetStatus(0,0xc);
    for (uVar3 = 0; (uVar3 & 0xffff) < (u32)*puVar1; uVar3 = uVar3 + 1) {
      if ((puVar1[(uVar3 & 0xffff) * 6 + 2] & 0x3fff) != uVar4) {
        }
        puVar1[(u32)*puVar1 * 6 + 2] = uVar4 | 0x8000;
        uVar2 = _fadeEffectGetRandom__FUl(0xffffffff);
        *(u32 *)(puVar1 + (u32)*puVar1 * 6 + 6) = uVar2;
        *puVar1 = *puVar1 + 1;
      }
    uVar4 = uVar4 + 1;
  } while (1);
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void memoDataGetPokemonTrainerRndFromID(void) {
#include "src/game/colosseum_battle_fn_8025FD34.inc"
}
#endif
#pragma pop
u32 memoDataGetPokemonTrainerRndFromID(u16 *r3, u16 r4) {
	  u16 *queue;
	  u16 *countQueue;
	  u16 count;
	  u32 r5;
	  u32 i;
	  s32 r0;
  queue = r3;
  if (queue == (u16 *)0) {
    queue = (u16 *)savedataGetStatus(0, 0xC);
  }
  countQueue = queue;
	  if (queue == (u16 *)0) {
	    countQueue = (u16 *)savedataGetStatus(0, 0xC);
	  }
	  count = (u32)*countQueue;
	  for (i = 0; (u32)(u16)i < (u32)count; i = i + 1) {
    r5 = (i & 0xffff) * 12;
    r0 = r5 + 4;
    r0 = (u32)*(u16 *)((u8 *)queue + r0) & 0x3FFF;
			    if (r0 == (u16)r4) {
		      r0 = r5 + 8;
		      return *(u32 *)((u8 *)queue + r0);
		    }
  }
  return 0;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void memoDataGetPokemonRndFromID(void) {
#include "src/game/colosseum_battle_fn_8025FDDC.inc"
}
#endif
#pragma pop
u32 memoDataGetPokemonRndFromID(u16 *r3, u16 r4) {
  u16 *queue;
  u16 *countQueue;
  u32 r5;
  u32 i;
  u16 count;
  s32 r0;
  queue = r3;
  if (queue == (u16 *)0) {
    queue = (u16 *)savedataGetStatus(0, 0xC);
  }
  countQueue = queue;
  if (queue == (u16 *)0) {
    countQueue = (u16 *)savedataGetStatus(0, 0xC);
  }
  count = (u32)*countQueue;
	  for (i = 0; (u32)(u16)i < (u32)count; i = i + 1) {
	    r5 = (i & 0xffff) * 12;
	    r0 = r5 + 4;
	    r0 = (u32)*(u16 *)((u8 *)queue + r0) & 0x3FFF;
		    if (r0 == (u16)r4) {
		      r0 = r5 + 0xC;
		      return *(u32 *)((u8 *)queue + r0);
		    }
  }
  return 0;
}

/* Address: 0x8025FE84 | Size: 0x60 | Ghidra import */
u16 memoDataGetPokemonID(u16 *r3, u32 r4)
{
    u16 res;
    extern void *savedataGetStatus();
    if (r3 == (u16 *)0) {
        r3 = (u16 *)savedataGetStatus(0, 0xc);
    }
    if (*r3 != 0) {
        r3 = (u16 *)((u8 *)r3 + (r4 & 0xFFFF) * 12);
        res = *(u16 *)((u8 *)r3 + 4);
    } else {
        res = 0;
    }
    return res;
}

/* Address: 0x8025FEE4 | Size: 0x34 | Ghidra import */
u16 memoDataGetCount(u16 *r3)

{
  if (r3 == (u16 *)0x0) {
    r3 = (u16 *)savedataGetStatus(0,0xc);
  }
  return *r3;
}

/* Address: 0x8025FF18 | Size: 0x84 | Ghidra import */
u32 memoDataSetMemoFlag(u16 *r3)

{
  u32 uVar2;
  u32 uVar3;
  u32 uVar1;
  
  uVar3 = 0;
  if (r3 == (u16 *)0x0) {
    r3 = (u16 *)savedataGetStatus(0,0xc);
  }
  uVar2 = 0;
  while ((u32)(u16)uVar2 < (u32)*r3) {
    uVar1 = (uVar2 & 0xffff) * 12;
    if ((*(volatile u16 *)((u8 *)r3 + uVar1 + 4) & 0x8000) != 0) {
      uVar3 = 1;
    }
    uVar2 = uVar2 + 1;
    *(volatile u16 *)((u8 *)r3 + uVar1 + 4) = *(volatile u16 *)((u8 *)r3 + uVar1 + 4) & 0x3fff;
  }
  return uVar3;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8025FF9C(void) {
#include "src/game/colosseum_battle_fn_8025FF9C.inc"
}
#endif
#pragma pop
void memoDataSet(u16 *r3, u32 r4) {
  extern u32 fn_8011F5C8();
  u16 *queue;
  u32 r30;
		  u32 count;
		  u32 rawLookup;
		  u16 lookup;
		  u32 r5;
		  u32 i;
		  u32 r0;

  queue = r3;
  r30 = r4;
  if (queue == (u16 *)0) {
    queue = (u16 *)savedataGetStatus(0, 0xC);
  }
  rawLookup = fn_8011F5C8(r30);
	  lookup = rawLookup;
	  count = *queue;
	  for (i = 0; (u32)(u16)i < (u32)count; i = i + 1) {
	    r5 = (i & 0xffff) * 12;
	    r0 = (u32)*(u16 *)((u8 *)queue + r5 + 4) & 0x3FFF;
	    if (r0 == lookup) {
	      return;
	    }
	  }
		  *(u16 *)((u8 *)queue + ((u32)count * 12 + 4)) = (u16)(rawLookup | 0x8000);
	  r5 = fn_8011F5B0(r30);
	  *(u32 *)((u8 *)queue + ((u32)*queue * 12 + 0xC)) = r5;
	  r5 = fn_8011F520(r30);
	  *(u32 *)((u8 *)queue + ((u32)*queue * 12 + 0x8)) = r5;
  *queue = (u16)(*queue + 1);
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void memoInit(void) {
#include "src/game/colosseum_battle_fn_80260070.inc"
}
#endif
#pragma pop
void memoInit(u16 *r3) {
  u32 r31;
  u32 r30;
  u16 *r29;
  u8 *r28;
  s32 r27;
  r29 = r3;
  if (r29 == (u16 *)0) {
    r29 = (u16 *)savedataGetStatus(0, 0xC);
  }
  r30 = 0;
  r27 = 0;
  *r29 = r30;
  r31 = r30;
  while (r27 < 0x1F4) {
    r28 = (u8 *)r29 + r31;
    *(u16 *)(r28 + 4) = r30;
    *(u32 *)(r28 + 0xC) = _fadeEffectGetRandom__FUl(-1);
    r27++;
    r31 += 12;
  }
}

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
    extern int fn_801DAC90();
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
  fn_801DAC90();
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
    uVar1 = fn_8011F5B0(r3);
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

  u16 sVar7;
  u8 cVar8;
  u32 uVar3;
  u32 uVar4;
  u32 uVar5;
  int iVar6;
  int iVar9;
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
  if (sVar7 == 0) {
    sVar7 = 0xffff;
  }
  else {
    sVar7 = (int)pokemonGetStatus(0,sVar7,0x66,0);
    if (sVar7 == 0) {
      sVar7 = 0xffff;
    }
  }
  if (sVar7 == 0xffff) {
    iVar9 = 0;
  }
  else {
    uVar3 = fn_8011F5B0(r3);
    uVar4 = (int)pokemonGetStatus(r3,0,0xc1,0);
    uVar4 = (-uVar4 | uVar4) >> 0x1f;
    iVar9 = fn_801DE190(sVar7,uVar3,uVar4);
    if (iVar9 == 0) {
      iVar9 = 0;
    }
    else {
      iVar6 = 0;
      puVar10 = (u16 *)lbl_8027A488;
      do {
        if ((*(int *)(puVar10 + 2) == 0) &&
           (cVar8 = fn_801DDD28(iVar9,*puVar10,4,0), cVar8 == '\0')) break;
        iVar6 = iVar6 + 1;
        puVar10 = puVar10 + 4;
      } while (iVar6 < 5);
      if (iVar6 < 5) {
        iVar9 = 0;
      }
    }
  }
  if (iVar9 != 0) {
    local_184 = iVar9;
    sVar7 = (int)pokemonGetStatus(r4,0,0x6e,0);
    if (sVar7 == 0) {
      sVar7 = 0xffff;
    }
    else {
      sVar7 = (int)pokemonGetStatus(0,sVar7,0x66,0);
      if (sVar7 == 0) {
        sVar7 = 0xffff;
      }
    }
    if (sVar7 == 0xffff) {
      iVar9 = 0;
    }
    else {
      uVar3 = fn_8011F5B0(r4);
      uVar4 = (int)pokemonGetStatus(r4,0,0xc1,0);
      uVar4 = (-uVar4 | uVar4) >> 0x1f;
      iVar9 = fn_801DE190(sVar7,uVar3,uVar4);
      if (iVar9 == 0) {
        iVar9 = 0;
      }
      else {
        iVar6 = 0;
        puVar10 = (u16 *)lbl_8027A488;
        do {
          if ((*(int *)(puVar10 + 2) == 1) &&
             (cVar8 = fn_801DDD28(iVar9,*puVar10,4,0), cVar8 == '\0')) break;
          iVar6 = iVar6 + 1;
          puVar10 = puVar10 + 4;
        } while (iVar6 < 5);
        if (iVar6 < 5) {
          iVar9 = 0;
        }
      }
    }
    if (iVar9 != 0) {
      bVar2 = 1;
      local_180 = iVar9;
      goto LAB_0025e130;
    }
  }
  fn_801DAC90();
  bVar2 = 0;
LAB_0025e130:
  if (bVar2) {
    iVar9 = fn_801653C4();
    if (iVar9 == 0) {
      uVar4 = 0;
    }
    else {
      uVar4 = fn_801656D8();
      fn_80165A20(1,0x32,0xff);
    }
    local_174 = fn_801653BC();
    if (local_174 == 0) {
      uVar5 = 0;
    }
    else {
      uVar5 = fn_801656D8();
      scriptSoundStop(0x32);
    }
    local_178 = iVar9;
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
      for (iVar9 = 0; iVar9 < r7; iVar9 = iVar9 + 1) {
        uVar1 = *r6;
        iVar6 = evolutionWazaLearn((int*)auStack_168,uVar1,local_188,0,0x8026045c,&local_184);
        if (iVar6 == 0) {
          local_188[0] = 0xff;
        }
        else {
          pokemonWazaCreate((int*)auStack_168,local_188[0],uVar1);
        }
        r6 = r6 + 1;
        *r8 = local_188[0];
        r8 = r8 + 1;
      }
      fadeSet((double)lbl_8047E6C0,3);
      fadeCheck(1);
      bVar2 = 1;
    }
    fn_801DAC90();
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
    extern void fn_8011288C(u32, u32);
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
    fn_8011288C(0, 0);
    _threadSwitch();
    return base[6];
}

#pragma peephole off
#pragma opt_common_subs off
/* Address: 0x80261388 | Size: 0x4C | Ghidra import */
u32 charNameBiosSearchIndex(u32 value)
{
    extern u32 *lbl_80478F80;
    extern u32 *lbl_80478F84;
    u32 *table;
    u32 count;
    u32 i;
    u32 offset;

    table = lbl_80478F84;
    count = *lbl_80478F80;
    for (i = 0; (u16)i < count; i++) {
        offset = ((u16)i) << 3;
        if (*(u32 *)((u8 *)table + offset + 4) == value) {
            return i;
        }
    }
    return 0;
}

/* Address: 0x802613D4 | Size: 0x70 | Ghidra import */
#pragma opt_common_subs on
u32 charNameBiosGetNameID(u32 idx) {
    extern void* lbl_80478F80;
    extern void* lbl_80478F84;
    extern void GSlogWrite(char*, char*, ...);
    extern char lbl_8027A4C8[];
    extern char lbl_8039A6C8[];
    u8* entry;
    u16 i = (u16)idx;

    if (i >= *(u32*)lbl_80478F80) {
        GSlogWrite(lbl_8027A4C8, lbl_8039A6C8);
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F84 + i * 8;
    }
    if (entry == NULL) {
        return 0;
    }
    return *(u32*)(entry + 4);
}
#pragma peephole on

/* Address: 0x80261444 | Size: 0x70 | Ghidra import */
#pragma peephole off
u32 charNameBiosGetHearFlag(u32 idx)
{
    extern void* lbl_80478F80;
    extern void* lbl_80478F84;
    extern void GSlogWrite(char*, char*, ...);
    extern char lbl_8027A4C8[];
    extern char lbl_8039A6C8[];
    u8* entry;
    u16 i = (u16)idx;

    if (i >= *(u32*)lbl_80478F80) {
        GSlogWrite(lbl_8027A4C8, lbl_8039A6C8);
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F84 + i * 8;
    }
    if (entry == NULL) {
        return 0;
    }
    return *(u16*)entry;
}
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
    extern void fn_801F2B5C(u32, BattleScanCallback, void *, u32);
    extern void fn_801F37B0(u32, BattleScanCallback, void *, u32);
    extern u32 _fightGSfloorPokemonCB__FPvUsPv(u32, u32, char *);
    extern u32 _fightGSfloorTrainerCB__FPvUsPv(u32, u32, char *);
    BattleScanContext scan;
    u16 firstCount;
    int total;

    scan.collectEntries = 0;
    scan.consumeEntries = 0;
    scan.count = 0;
    scan.nextEntry = 0;
    fn_801F2B5C(0, _fightGSfloorTrainerCB__FPvUsPv, &scan, 0);
    firstCount = scan.count;
    scan.count = 0;
    fn_801F37B0(0, _fightGSfloorPokemonCB__FPvUsPv, &scan, 0);
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
    extern void *fn_801C3108(void);
    extern void fn_801F2B5C(u32, BattleScanCallback, void *, u32);
    extern void fn_801F37B0(u32, BattleScanCallback, void *, u32);
    extern u32 _fightGSfloorPokemonCB__FPvUsPv(u32, u32, char *);
    extern u32 _fightGSfloorTrainerCB__FPvUsPv(u32, u32, char *);
    BattleScanOutput *out;
    BattleScanContext scan;
    u16 firstCount;
    u16 secondCount;
    u8 *entries;

    out = rawOut;
    entries = out->entries;
    memcpy(entries, fn_801C3108(), 0x44);
    scan.collectEntries = 1;
    scan.consumeEntries = 1;
    scan.count = 0;
    scan.entries = entries;
    scan.nextEntry = entries + 0x44;
    fn_801F2B5C(0, _fightGSfloorTrainerCB__FPvUsPv, &scan, 0);
    firstCount = scan.count;
    scan.count = 0;
    fn_801F37B0(0, _fightGSfloorPokemonCB__FPvUsPv, &scan, 0);
    secondCount = scan.count;
    out->firstCount = firstCount;
    out->secondCount = secondCount;
    fn_801EF8F4(1);
    fn_801C3114();
    fn_801DAC90();
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
    extern int fn_800E9B2C();
    extern int fn_801C3430();
    extern int fn_801DAEF8(int);
    extern u32 fn_801DE418(u16);
    extern int fn_801F198C();
    u8 *entries;
    u16 firstCount;
    BattleReplayFirstEntry *firstEntry;
    u16 secondCount;
    BattleReplaySecondEntry *secondEntry;
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
        state = fn_801DAC3C();
        fn_800E9B2C(state, firstEntry->state);
        fn_801DA224(model, firstEntry->attr);
        firstEntry++;
    }
    secondEntry = (BattleReplaySecondEntry *)firstEntry;
    while (secondCount-- != 0) {
        model = fn_801DE190(secondEntry->modelId, secondEntry->arg, secondEntry->variant);
        *(u32 *)(secondEntry->target + 0x600) = model;
        *secondEntry->modelOut = model;
        state = fn_801DAC3C();
        fn_800E9B2C(state, secondEntry->state);
        fn_801DA224(model, secondEntry->attr);
        state = GSmodelGetVisibility(state);
        fn_801DA4E8(model, state);
        secondEntry++;
    }
    memcpy(fn_801C3108(), entries, 0x44);
    fn_801EF8F4(1);
    fn_801C3430();
    fn_801F198C();
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
  
  iVar1 = fn_801FB1C0(r3,0,0x4c,0);
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
	                    if (*piVar6 == iVar1) goto LAB_0025e8fc;
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
    extern void fn_801F2B5C();
    extern u32 fn_801F37B0();
    extern u8 fn_801F1700(u32);
    extern u8 fn_801F1758(u32);
    extern void fn_8000DDBC(void);
    extern void fn_8000DD30(void);
    extern u8 fn_8000DD98(void);
    extern u8 fn_8000DD0C(void);
    extern u32 _fightMenuAllFightTrainerCloseStatusMenuSubCloseCheck__FPvUsPv();
    extern u32 _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv();
    extern u32 _fightMenuAllFightOutPokemonCloseStatusMenuSubCloseCheck__FPvUsPv();
    extern u32 _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv();
    u8 done;

    fn_801F2B5C(0, _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv, 0, 0);
    fn_801F37B0(0, _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv, 0, 0);
    if (fn_801F1700(0) == 1) {
        fn_8000DDBC();
    }
    if (fn_801F1758(0) == 1) {
        fn_8000DD30();
    }
    if ((u8)wait == 1) {
        fn_801F2B5C(0, _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv, 0, 0);
        do {
            done = 1;
            fn_801F2B5C(0, _fightMenuAllFightTrainerCloseStatusMenuSubCloseCheck__FPvUsPv, &done, 0);
            if (done == 1) {
                break;
            }
            _threadSwitch();
        } while (1);
        fn_801F37B0(0, _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv, 0, 0);
        do {
            if ((u8)fn_801F37B0(0, _fightMenuAllFightOutPokemonCloseStatusMenuSubCloseCheck__FPvUsPv, 0, 0) == 1) {
                break;
            }
            _threadSwitch();
        } while (1);
        if (fn_801F1700(0) == 1) {
            do {
                if (fn_8000DD98() == 0) {
                    break;
                }
                _threadSwitch();
            } while (1);
        }
        if (fn_801F1758(0) == 1) {
            do {
                if (fn_8000DD0C() == 0) {
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
    extern void fn_801F2B5C();
    extern u32 fn_801F37B0();
    extern u8 fn_801F1700(u32);
    extern u8 fn_801F1758(u32);
    extern void fn_8000DDE8(void);
    extern void fn_8000DD5C(void);
    extern u32 _fightMenuAllFightTrainerOpenStatusMenuSub__FPvUsPv();
    extern u32 _fightMenuAllFightOutPokemonOpenStatusMenuSub__FPvUsPv();
    u8 openStatus;

    fn_801F2B5C(0, _fightMenuAllFightTrainerOpenStatusMenuSub__FPvUsPv, 0, 0);
    openStatus = 1;
    fn_801F37B0(0, _fightMenuAllFightOutPokemonOpenStatusMenuSub__FPvUsPv, &openStatus, 0);
    if (timerMode < 0) {
        if (fn_801F1700(0) == 1) {
            fn_8000DDE8();
        }
    }
    if (fn_801F1758(0) == 1) {
        fn_8000DD5C();
    }
}

/* Address: 0x80261B68 | Size: 0x84 | Ghidra import */
void fightMenuAllFightTrainerCloseStatusMenu(u32 wait)
{
    extern void fn_801F2B5C();
    extern void _threadSwitch(void);
    extern u32 _fightMenuAllFightTrainerCloseStatusMenuSubCloseCheck__FPvUsPv();
    extern u32 _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv();
    u32 r30;
    u8 done;

    fn_801F2B5C(0, _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv, 0, 0);
    if ((u8)wait == 1) {
        r30 = 1;
        do {
            done = r30;
            fn_801F2B5C(0, _fightMenuAllFightTrainerCloseStatusMenuSubCloseCheck__FPvUsPv, &done, 0);
            if (done == 1) {
                break;
            }
            _threadSwitch();
        } while (1);
    }
}

/* Address: 0x80261BEC | Size: 0xD0 | Ghidra import */
u32 _fightMenuAllFightTrainerCloseStatusMenuSubCloseCheck__FPvUsPv(u32 r3,u32 r4,u8 *r5)

{
  u32 iVar1;
  u32 uVar2;
  u16 uVar4;
  u16 sVar5;
  u32 uVar3;
  u8 result;
  u32 found;
  u8 cVar6;

  iVar1 = fn_801F02AC(2,r3,r4);
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    uVar4 = fightSideGetStatus(iVar1,0,5,0);
    sVar5 = fn_801F0134(r3,r4);
    if (sVar5 == 0) {
      uVar2 = 0;
    }
    else {
      fn_801F0234();
      uVar3 = fn_801F0204();
      if ((int)uVar3 < 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = fightSideGetStatus(0,uVar4,2,uVar3 & 0xffff);
      }
    }
  }
  cVar6 = menuIsCheck(uVar2);
  if ((cVar6 == '\x01') && (r5 != (u8 *)0x0)) {
    *r5 = 0;
  }
  return 1;
}

/* Address: 0x80261CBC | Size: 0xD0 | Ghidra import */
u32 _fightMenuAllFightTrainerCloseStatusMenuSub__FPvUsPv(u32 r3,u32 r4)

{
  u32 iVar1;
  u16 uVar3;
  u16 sVar4;
  u32 uVar2;
  u8 cVar5;
  u32 uVar6;

  iVar1 = fn_801F02AC(2,r3,r4);
  if (iVar1 == 0) {
    uVar6 = 0;
  }
  else {
    uVar3 = fightSideGetStatus(iVar1,0,5,0);
      sVar4 = fn_801F0134(r3,r4);
    if (sVar4 == 0) {
      uVar6 = 0;
    }
    else {
      fn_801F0234();
      uVar2 = fn_801F0204();
      if ((int)uVar2 < 0) {
        uVar6 = 0;
      }
      else {
        uVar6 = fightSideGetStatus(0,uVar3,2,uVar2 & 0xffff);
      }
    }
  }
  cVar5 = menuIsCheck(uVar6);
  if (cVar5 != '\0') {
    menuCloseCustom(uVar6,0,0);
  }
  return 1;
}

/* Address: 0x80261D8C | Size: 0xF0 | Ghidra import */
u32 _fightMenuAllFightTrainerOpenStatusMenuSub__FPvUsPv(u32 r3, u32 r4)
{
    extern int fn_801F7954();
    extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
    u32 iVar1;
    u16 uVar3;
    u16 sVar4;
    u32 uVar2;
    u32 uVar5;
    u32 local_20[2];
    u32 local_28[2];

    iVar1 = fn_801F02AC(2, r3, r4);
    if (iVar1 == 0) {
        uVar5 = 0;
    } else {
        uVar3 = fightSideGetStatus(iVar1, 0, 5, 0);
        sVar4 = fn_801F0134(r3, r4);
        if (sVar4 == 0) {
            uVar5 = 0;
        } else {
            fn_801F0234();
            uVar2 = fn_801F0204();
            if ((int)uVar2 < 0) {
                uVar5 = 0;
            } else {
                uVar5 = fightSideGetStatus(0, uVar3, 2, uVar2 & 0xffff);
            }
        }
    }
    fn_801F7954(r3, local_28);
    local_20[0] = local_28[0];
    *(u16 *)&local_20[1] = *(u16 *)&local_28[1];
    menuOpenCustom(uVar5, 0, 0, 0, 0, 1, local_20);
  return 1;
}

/* Address: 0x80261E7C | Size: 0x7C | Ghidra import */
void fightMenuAllFightOutPokemonCloseStatusMenu(u32 wait)
{
    extern u32 fn_801F37B0();
    extern void _threadSwitch(void);
    extern u32 _fightMenuAllFightOutPokemonCloseStatusMenuSubCloseCheck__FPvUsPv();
    extern u32 _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv();
    fn_801F37B0(0, _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv, 0, 0);
    if ((u8)wait == 1) {
        do {
            if ((u8)fn_801F37B0(0, _fightMenuAllFightOutPokemonCloseStatusMenuSubCloseCheck__FPvUsPv, 0, 0) == 1) {
                break;
            }
            _threadSwitch();
        } while (1);
    }
}

/* Address: 0x80261EF8 | Size: 0xBC | Ghidra import */
int _fightMenuAllFightOutPokemonCloseStatusMenuSubCloseCheck__FPvUsPv(u32 r3,u32 r4)

{
  u32 iVar1;
  u32 uVar2;
  u16 uVar4;
  u16 sVar5;
  u32 uVar3;
  u8 result;
  
  iVar1 = fn_801F02AC(2,r3,r4);
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    uVar4 = fightSideGetStatus(iVar1,0,5,0);
    sVar5 = fn_801F0134(r3,r4);
    if (sVar5 == 0) {
      uVar2 = 0;
    }
    else {
      fn_801F0234();
      uVar3 = fn_801F0204();
      if ((int)uVar3 < 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = fightSideGetStatus(0,uVar4,3,uVar3 & 0xffff);
      }
    }
  }
  uVar3 = menuIsCheck(uVar2);
  result = (uVar3 & 0xff) != 1;
  return result;
}

/* Address: 0x80261FB4 | Size: 0xD0 | Ghidra import */
u32 _fightMenuAllFightOutPokemonCloseStatusMenuSub__FPvUsPv(u32 r3,u32 r4)

{
  u32 iVar1;
  u16 uVar3;
  u16 sVar4;
  u32 uVar2;
  u8 cVar5;
  u32 uVar6;

  iVar1 = fn_801F02AC(2,r3,r4);
  if (iVar1 == 0) {
    uVar6 = 0;
  }
  else {
    uVar3 = fightSideGetStatus(iVar1,0,5,0);
    sVar4 = fn_801F0134(r3,r4);
    if (sVar4 == 0) {
      uVar6 = 0;
    }
    else {
      fn_801F0234();
      uVar2 = fn_801F0204();
      if ((int)uVar2 < 0) {
        uVar6 = 0;
      }
      else {
        uVar6 = fightSideGetStatus(0,uVar3,3,uVar2 & 0xffff);
      }
    }
  }
  cVar5 = menuIsCheck(uVar6);
  if (cVar5 != '\0') {
    menuCloseCustom(uVar6,0,0);
  }
  return 1;
}

/* Address: 0x80262084 | Size: 0x140 | Ghidra import */
u32 _fightMenuAllFightOutPokemonOpenStatusMenuSub__FPvUsPv(u32 r3,u32 r4,char *r5)

{
    extern int fn_801FE168();
  extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
  u32 *puVar1;
  u32 uVar2;
  u8 cVar7;
  u32 iVar3;
  u16 uVar5;
  u16 sVar6;
  u32 uVar4;
  u32 *puVar8;
  u32 *puVar9;
  u8 cVar10;
		  u32 uVar11;
		  u32 local_48[12];
		  u32 local_78[12];

	  if (r5 == (char *)0x0) {
	    cVar10 = '\x01';
	  }
	  else {
	    cVar10 = *r5;
	  }
	  cVar7 = fightOutPokemonCheckFightOut(r3);
	  if (cVar7 == '\0') {
	    return 1;
	  }
	    iVar3 = fn_801F02AC(2,r3,r4);
    if (iVar3 == 0) {
      uVar11 = 0;
    }
    else {
      uVar5 = fightSideGetStatus(iVar3,0,5,0);
		      sVar6 = fn_801F0134(r3,r4);
      if (sVar6 == 0) {
        uVar11 = 0;
      }
      else {
        fn_801F0234();
        uVar4 = fn_801F0204();
        if ((int)uVar4 < 0) {
          uVar11 = 0;
        }
        else {
          uVar11 = fightSideGetStatus(0,uVar5,3,uVar4 & 0xffff);
        }
      }
    }
		    fn_801FE168(r3,local_78);
    if (cVar10 == '\0') {
      ((u8 *)local_78)[0x29] = 0;
    }
    puVar9 = local_48 - 1;
    puVar8 = local_78 - 1;
    iVar3 = 6;
    while (iVar3--) {
      puVar1 = puVar8 + 1;
      puVar8 = puVar8 + 2;
      uVar2 = *puVar1;
      puVar9[1] = uVar2;
      puVar9 = puVar9 + 2;
      *puVar9 = *puVar8;
    }
    menuOpenCustom(uVar11,0xffffffff,0,0,0,1,local_48);
  return 1;
}

/* Address: 0x802621C4 | Size: 0x30 | Ghidra import */
void fightMenuOpenLevelUpStatusMenu(u8 *dst, u8 value)
{
    extern void winMsgOpenLevelUpStatus(u8 *, u32);

    if (dst != NULL) {
        *dst = value;
        winMsgOpenLevelUpStatus(dst, 1);
    }
}

/* Address: 0x802621F4 | Size: 0x7C | Ghidra import */
void fightMenuSubMenuLvupStatus(s16 *current, s16 *previous, s16 *out)
{
    if (current == NULL) {
        return;
    }
    if (previous == NULL) {
        return;
    }
    if (out == NULL) {
        return;
    }
    out[1] = current[1] - previous[1];
    out[2] = current[2] - previous[2];
    out[3] = current[3] - previous[3];
    out[5] = current[5] - previous[5];
    out[6] = current[6] - previous[6];
    out[4] = current[4] - previous[4];
}

/* Address: 0x80262270 | Size: 0x74 | Ghidra import */
s32 fightMenuWazaWasure(u32 r3, u32 r4)
{
    extern f32 lbl_8047E6C8;
    extern s32 fn_80097A38(u32, u32);
    s32 result;

    fadeSet((double)lbl_8047E6C8, 3);
    fadeCheck(1);
    result = fn_80097A38(r3, r4);
    if (result == 4) {
        result = -1;
    }
    fadeSet((double)lbl_8047E6C8, 2);
    fadeCheck(1);
    return result;
}

/* Address: 0x802622E4 | Size: 0x24 | Ghidra import */
void fightMenuCloseLevelUpStatusMenu(void)
{
    extern void winMsgCloseLevelUpStatus(u32);

    winMsgCloseLevelUpStatus(1);
}

/* Address: 0x80262308 | Size: 0x2C | Ghidra import */
u32 fightMenuYesNo(void)
{
    extern s8 fn_8001E184(void);

    return (__cntlzw((s8)fn_8001E184()) >> 5) & 0xff;
}

/* Address: 0x80262334 | Size: 0x80 | Ghidra import */
u32 fightMenuWazaKoukaMsg(u32 msgId, u32 unused, u32 itemId)
{
    extern u32 itemGetStatus(u32, u32, u32, u32);
    extern u32 fn_800FA280(u32);
    u32 itemName;

    fn_80132A38(0x10);
    itemName = fn_800FA280(itemGetStatus(0, itemId, 1, 0));
    fn_80132A38(0x29, itemName);
    if (msgId != 0) {
        winMsgOpenFight(msgId, 1, 1);
        return 1;
    }
    return 0;
}

/* Address: 0x802623B4 | Size: 0xB8 | Ghidra import */
u32 fightMenuWazaOutMsg(u32 msgId, u32 pokemon)
{
    extern u32 wazaGetStatus(u32, u32, u32, u32);
    extern u32 fn_800FA280(u32);
    u32 name;

    fn_80132A38(0xf, msgId);
    name = fn_800FA280(wazaGetStatus(0, pokemon, 0xa, 0));
    fn_80132A38(0xd, name);
    fn_80132A38(0x28, fn_800FA280(wazaGetStatus(0, pokemon, 1, 0)));
    fn_80132A38(0xe, fn_800FA280(wazaGetStatus(0, pokemon, 0xb, 0)));
    winMsgOpenFight(0x768d, 1, 1);
    return 1;
}

/* Address: 0x8026246C | Size: 0x24 | Ghidra import */
void fightMenuCloseMsg(void)
{
    winMsgCloseFight(0);
}

/* Address: 0x80262490 | Size: 0x3C | Ghidra import */
u32 fightMenuOpenTrainerMsg(u32 msgId)
{
    if (msgId != 0) {
        winMsgOpenFightNoWait(msgId, 1, 1);
        return 1;
    }
    return 0;
}

/* Address: 0x802624CC | Size: 0x3C | Ghidra import */
u32 fightMenuOpenMsg(u32 msgId)
{
    if (msgId != 0) {
        winMsgOpenFight(msgId, 1, 1);
        return 1;
    }
    return 0;
}

/* Address: 0x80262508 | Size: 0x82C | Ghidra import */
u32 fightMenuFightTrainerAgbHeroOpenMenu(u32 r3, u32 r4)
{
    extern u32 fn_801FB1C0(u32, u32, u32, u32);
    extern u8 fn_801F18DC(u32);
    extern u32 fn_801F02AC(u32, u32, u32);
    extern u16 fightSideGetStatus(u32, u32, u32, u32);
    extern u16 fn_801F0134(u32, u32);
    extern void fn_801F0234(void);
    extern s32 fn_801F0204(void);
    extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
    extern u16 fn_801F54A4(u32, u32, u32, u32);
    extern void fightTypeDataBiosGetPtr(u32);
    extern u32 fightTypeDataBiosGetFightoutPokemonNum(void);
    extern u32 fn_801F981C(u32, u32);
    extern u8 fightOutPokemonCheckFightActionSelect(u32, u32);
    extern void fightOutPokemonInitFightActionBuff(u32);
    extern u16 fn_801EF634(void);
    extern u8 fn_801F1700(u32);
    extern u8 fightTimerCommandIsOver(void);
    extern u32 fn_800111C4(u32, u32, u32, u32);
    extern s32 fn_80089F70(u32);
    extern s8 fn_80089F68(u32);
    extern u16 fn_80089F58(u32);
    extern u16 fn_80089F60(u32);
    extern u8 fn_801FF1BC(u32, u32);
    extern u32 fightOutPokemonGetPokemonPtr(u32);
    extern s16 pokemonGetStatus(u32, u32, u32, u32);
    extern u32 lbl_80478DF8;
    extern u32 fn_8022B2CC(u32, u32, u32, u32, u32, u32, s32);
    extern u32 fn_801F47B4(u32, u32);
    extern u32 fn_801F7258(u32, u32);
    extern void fightOutPokemonCreateFightActionAttackWaza(u32, u32, u32, u32, void *, u32, u32, s32, u32);
    extern void fightOutPokemonCreateFightAction(u32, u32, u32, u32, void *, s32);
    extern void fn_801F9130(u32, u32, u32);
    extern void fn_801F9790(u32);
    extern u8 menuIsCheck(u32);
    extern void menuCloseCustom(u32, u32, u32);
    extern char lbl_80375D30[];
    extern char lbl_80375CA8[];
    u32 ctx;
    u32 param;
    u32 battle;
    u32 found;
    u32 status;
    u32 side;
    s32 index;
    u32 msg;
    u16 optionCount;
    u32 count;
    u32 lastGood;
    u32 i;
    u32 slot;
    u32 entry;
    s32 kind;
    s32 moveSlot;
    u32 moveId;
    u32 selected;
    u32 targetIndex;
    u32 sideIndex;
    u32 optionIndex;
    u32 scanIndex;
    u32 sideObj;
    u32 optionObj;
    u32 done;

    ctx = r3;
    param = r4;
    battle = fn_801FB1C0(ctx, 0, 0x4b, 0);
    if (fn_801F18DC(0) != 0) {
        msg = 0x100;
        found = fn_801F02AC(2, ctx, param);
        if (found == 0) {
            status = 0;
        }
        else {
            side = fightSideGetStatus(found, 0, 5, 0);
            if (fn_801F0134(ctx, param) == 0) {
                status = 0;
            }
            else {
                fn_801F0234();
                index = fn_801F0204();
                if (index < 0) {
                    status = 0;
                }
                else {
                    status = fightSideGetStatus(0, side, 2, index & 0xffff);
                }
            }
        }
        switch (status) {
        case 0xf1:
            msg = 0x100;
            break;
        case 0xf2:
            msg = 0x101;
            break;
        case 0xf3:
            msg = 0x102;
            break;
        case 0xf4:
            msg = 0x103;
            break;
        }
        menuOpenCustom(msg, 0, 0, 0, 0, 0);
    }
    optionCount = fn_801F54A4(0, 0, 0x16, 0);
    fightTypeDataBiosGetPtr(param);
    count = fightTypeDataBiosGetFightoutPokemonNum() & 0xff;
    lastGood = 0;
    i = 0;
    while ((u32)(u16)i < count) {
        slot = fn_801F981C(ctx, i);
        if (slot == 0) {
            lastGood = i;
            i++;
            continue;
        }
        if (fightOutPokemonCheckFightActionSelect(slot, 1) == 0) {
            lastGood = i;
            i++;
            continue;
        }
        while (1) {
            fightOutPokemonInitFightActionBuff(slot);
            if ((u16)fn_801EF634() == 1) {
                fn_801F9790(ctx);
                if (fn_801F18DC(0) != 0) {
                    msg = 0x100;
                    found = fn_801F02AC(2, ctx, param);
                    if (found == 0) {
                        status = 0;
                    }
                    else {
                        side = fightSideGetStatus(found, 0, 5, 0);
                        if (fn_801F0134(ctx, param) == 0) {
                            status = 0;
                        }
                        else {
                            fn_801F0234();
                            index = fn_801F0204();
                            status = (index < 0) ? 0 : fightSideGetStatus(0, side, 2, index & 0xffff);
                        }
                    }
                    switch (status) {
                    case 0xf1:
                        msg = 0x100;
                        break;
                    case 0xf2:
                        msg = 0x101;
                        break;
                    case 0xf3:
                        msg = 0x102;
                        break;
                    case 0xf4:
                        msg = 0x103;
                        break;
                    }
                    if (menuIsCheck(msg) != 0) {
                        menuCloseCustom(msg, 0, 1);
                    }
                }
                return 0;
            }
            if ((fn_801F1700(0) == 1) && (fightTimerCommandIsOver() == 1)) {
                fn_801F9130(ctx, slot, param);
                lastGood = i;
                break;
            }
            entry = fn_800111C4(battle, ctx, (u16)i, param);
            kind = fn_80089F70(entry);
            if ((u16)fn_801EF634() == 1) {
                fn_801F9790(ctx);
                return 0;
            }
            if (kind == 3) {
                fightOutPokemonCreateFightAction(slot, 0, 8, 0, lbl_80375D30, 0);
                lastGood = i;
                break;
            }
            if (kind == 1) {
                if (fn_801FF1BC(slot, 1) != 0) {
                    lastGood = i;
                    break;
                }
                moveSlot = fn_80089F68(entry);
                if (moveSlot < 0) {
                    continue;
                }
                moveId = pokemonGetStatus(fightOutPokemonGetPokemonPtr(slot), 0, 0x7f, moveSlot);
                moveId &= 0xffff;
                if ((moveId == 0) || (moveId >= lbl_80478DF8) || (moveId == 0x165)) {
                    continue;
                }
                selected = fn_8022B2CC(slot, moveId, param, (u32)_fightMenuFightTrainerAgbHeroSelectDefensePokemon__FP15FightOutPokemonUsUs, 1, 0, -1);
                if (selected == 0) {
                    targetIndex = fn_80089F60(entry);
                    scanIndex = 0;
                    done = 0;
                    sideIndex = 0;
                    while ((sideIndex & 0xffff) < 2) {
                        sideObj = fn_801F47B4(0, sideIndex);
                        if (sideObj != 0) {
                            optionIndex = 0;
                            while ((optionIndex & 0xffff) < optionCount) {
                                optionObj = fn_801F7258(sideObj, optionIndex);
                                if (optionObj != 0) {
                                    selected = fn_801FB1C0(optionObj, 0, 0x46, scanIndex);
                                    if ((u16)scanIndex == targetIndex) {
                                        done = 1;
                                        break;
                                    }
                                    scanIndex++;
                                }
                                optionIndex++;
                            }
                            if (done == 1) {
                                break;
                            }
                        }
                        sideIndex++;
                    }
                }
                if (selected == 0) {
                    continue;
                }
                fightOutPokemonCreateFightActionAttackWaza(slot, 0, 0x13, 0, lbl_80375CA8, moveId,
                            fn_801F0134(selected, param), moveSlot, 0);
                lastGood = i;
                break;
            }
            if (kind == 2) {
                moveId = fn_801FB1C0(ctx, 0, 0x45, fn_80089F58(entry));
                index = pokemonGetStatus(moveId, 0, 0xce, 0);
                if (index < 0) {
                    continue;
                }
                fightOutPokemonCreateFightAction(slot, 0, 9, 0, lbl_80375D30, index);
                lastGood = i;
                break;
            }
            if (kind == 4) {
                fn_801F9130(ctx, slot, param);
                lastGood = i;
                break;
            }
            if (kind == 5) {
                fn_801F9790(ctx);
                return 0;
            }
            if (kind != 0) {
                fn_801F9130(ctx, slot, param);
                lastGood = i;
                break;
            }
            if ((u16)i == 0) {
                break;
            }
            i = lastGood;
        }
        i++;
    }
    if (fn_801F18DC(0) != 0) {
        msg = 0x100;
        found = fn_801F02AC(2, ctx, param);
        if (found == 0) {
            status = 0;
        }
        else {
            side = fightSideGetStatus(found, 0, 5, 0);
            if (fn_801F0134(ctx, param) == 0) {
                status = 0;
            }
            else {
                fn_801F0234();
                index = fn_801F0204();
                status = (index < 0) ? 0 : fightSideGetStatus(0, side, 2, index & 0xffff);
            }
        }
        switch (status) {
        case 0xf1:
            msg = 0x100;
            break;
        case 0xf2:
            msg = 0x101;
            break;
        case 0xf3:
            msg = 0x102;
            break;
        case 0xf4:
            msg = 0x103;
            break;
        }
        if (menuIsCheck(msg) != 0) {
            msg = 0x100;
            found = fn_801F02AC(2, ctx, param);
            if (found == 0) {
                status = 0;
            }
            else {
                side = fightSideGetStatus(found, 0, 5, 0);
                if (fn_801F0134(ctx, param) == 0) {
                    status = 0;
                }
                else {
                    fn_801F0234();
                    index = fn_801F0204();
                    status = (index < 0) ? 0 : fightSideGetStatus(0, side, 2, index & 0xffff);
                }
            }
            switch (status) {
            case 0xf1:
                msg = 0x100;
                break;
            case 0xf2:
                msg = 0x101;
                break;
            case 0xf3:
                msg = 0x102;
                break;
            case 0xf4:
                msg = 0x103;
                break;
            }
            menuCloseCustom(msg, 0, 1);
        }
    }
    return 1;
}

/* Address: 0x80262D3C | Size: 0x430 | Ghidra import */
s32 fightMenuFightTrainerAgbHeroSelectIrekaeFightPokemon(u32 r3, u32 r4, u32 r5, s32 r6)
{
    extern u32 fn_801FB1C0(u32, u32, u32, u32);
    extern void fightTypeDataBiosGetPtr(u32);
    extern u32 fightTypeDataBiosGetFightoutPokemonNum(void);
    extern u16 fn_801EF634(void);
    extern u8 fn_801F1700(u32);
    extern u8 fightTimerCommandIsOver(void);
    extern u8 fn_801F18DC(u32);
    extern u32 fn_801F02AC(u32, u32, u32);
    extern u32 fightSideGetStatus(u32, u32, u32, u32);
    extern u16 fn_801F0134(u32, u32);
    extern void fn_801F0234(void);
    extern s32 fn_801F0204(void);
    extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
    extern u32 fn_800111E8(u32, u32, u32, u32);
    extern s32 fn_80089F70(u32);
    extern u16 fn_80089F58(u32);
    extern s16 pokemonGetStatus(u32, u32, u32, u32);
    extern s32 fn_801F93F8(u32, s32, u32);
    extern u8 menuIsCheck(u32);
    extern void menuCloseCustom(u32, u32, u32);
    u32 ctx;
    u32 param1;
    u32 param2;
    s32 target;
    u32 battle;
    u32 count;
    u32 i;
    u32 found;
    u32 status;
    u32 side;
    s32 index;
    u32 msg;
    u32 entry;
    s32 kind;

    ctx = r3;
    param1 = r4;
    param2 = r5;
    target = r6;
    battle = fn_801FB1C0(ctx, 0, 0x4b, 0);
    fightTypeDataBiosGetPtr(param1);
    count = fightTypeDataBiosGetFightoutPokemonNum() & 0xff;
    i = 0;
    while ((u32)(u16)i < count) {
        found = fn_801FB1C0(ctx, 0, 0x46, i);
        if (found == (u32)target) {
            break;
        }
        i++;
    }
    if ((u32)(u16)i >= count) {
        return fightTrainerAiSelectIrekaeDasuFightPokemon((void *)ctx, param1, param2, target);
    }

    if ((u16)fn_801EF634() == 1) {
        goto set_cancel;
    }
    if ((fn_801F1700(0) == 1) && (fightTimerCommandIsOver() == 1)) {
        goto set_select;
    }
    {
        if (fn_801F18DC(0) != 0) {
            msg = 0x100;
            found = fn_801F02AC(2, ctx, param1);
            if (found == 0) {
                status = 0;
            }
            else {
                side = (u16)fightSideGetStatus(found, 0, 5, 0);
                if (fn_801F0134(ctx, param1) == 0) {
                    status = 0;
                }
                else {
                    fn_801F0234();
                    index = fn_801F0204();
                    if (index < 0) {
                        status = 0;
                    }
                    else {
                        status = fightSideGetStatus(0, side, 2, index & 0xffff);
                    }
                }
            }
            switch (status) {
            case 0xf1:
                msg = 0x100;
                break;
            case 0xf2:
                msg = 0x101;
                break;
            case 0xf3:
                msg = 0x102;
                break;
            case 0xf4:
                msg = 0x103;
                break;
            }
            menuOpenCustom(msg, 0, 0, 0, 0, 0);
        }
        entry = fn_800111E8(battle, ctx, (u16)i, param1);
        kind = fn_80089F70(entry);
        if ((u16)fn_801EF634() == 1) {
            goto set_cancel;
        }
        if (kind == 2) {
            found = fn_801FB1C0(ctx, 0, 0x45, fn_80089F58(entry));
            target = pokemonGetStatus(found, 0, 0xce, 0);
            goto after_select;
        }
	        if (kind == 4) {
	            goto set_select;
	        }
	        if (kind == 5) {
	            goto set_cancel;
	        }
	    }
set_select:
    target = fn_801F93F8(ctx, target, param1);
    goto after_select;
set_cancel:
    target = -2;
after_select:

    if (fn_801F18DC(0) != 0) {
        msg = 0x100;
        found = fn_801F02AC(2, ctx, param1);
        if (found == 0) {
            status = 0;
        }
        else {
            side = (u16)fightSideGetStatus(found, 0, 5, 0);
            if (fn_801F0134(ctx, param1) == 0) {
                status = 0;
            }
            else {
                fn_801F0234();
                index = fn_801F0204();
                if (index < 0) {
                    status = 0;
                }
                else {
                    status = fightSideGetStatus(0, side, 2, index & 0xffff);
                }
            }
        }
        switch (status) {
        case 0xf1:
            msg = 0x100;
            break;
        case 0xf2:
            msg = 0x101;
            break;
        case 0xf3:
            msg = 0x102;
            break;
        case 0xf4:
            msg = 0x103;
            break;
        }
        if (menuIsCheck(msg) != 0) {
            msg = 0x100;
            found = fn_801F02AC(2, ctx, param1);
            if (found == 0) {
                status = 0;
            }
            else {
                side = (u16)fightSideGetStatus(found, 0, 5, 0);
                if (fn_801F0134(ctx, param1) == 0) {
                    status = 0;
                }
                else {
                    fn_801F0234();
                    index = fn_801F0204();
                    if (index < 0) {
                        status = 0;
                    }
                    else {
                        status = fightSideGetStatus(0, side, 2, index & 0xffff);
                    }
                }
            }
            switch (status) {
            case 0xf1:
                msg = 0x100;
                break;
            case 0xf2:
                msg = 0x101;
                break;
            case 0xf3:
                msg = 0x102;
                break;
            case 0xf4:
                msg = 0x103;
                break;
            }
            menuCloseCustom(msg, 0, 1);
        }
    }
    return target;
}

/* Address: 0x80264ADC | Size: 0x27C | Ghidra import */
u32 _fightMenuFightTrainerGcHeroOpenMenuSubBallSelectTargetPokemon__FP15FightOutPokemonUsUs(u32 r3,u32 r4,u32 r5)

{
    extern int fn_8001120C();
    extern int fn_80011288();
    extern int winMsgCloseFight();
    extern int winMsgOpenFight();
    extern int fn_801906A0();
    extern int fn_801F000C();
  u16 uVar4;
  u32 uVar1;
  u8 cVar6;
  int iVar2;
  u16 sVar5;
  u32 uVar3;
  u32 found;
  u8 auStack_38 [0x24];
  u32 local_34;
  u32 local_2c;
  u32 local_24;
  u32 local_1c;
  u8 local_18;
  u8 local_17;
  
LAB_00261af4:
  uVar1 = fn_801F02AC(0xf,r3,r5);
    cVar6 = fightOutPokemonCheckFightOut();
    if (cVar6 == '\x01') {
    found = fn_801F02AC(2,uVar1,r5);
    if (found == 0) {
      uVar3 = 0;
    }
    else {
      uVar4 = fightSideGetStatus(found,0,5,0);
      sVar5 = fn_801F0134(uVar1,r5);
      if (sVar5 == 0) {
        uVar3 = 0;
      }
      else {
        fn_801F0234();
        uVar3 = fn_801F0204();
        if ((int)uVar3 < 0) {
          uVar3 = 0;
        }
        else {
          uVar3 = fightSideGetStatus(0,uVar4,3,uVar3 & 0xffff);
        }
      }
    }
  }
  else {
    uVar3 = 0;
  }
  *(u32 *)(auStack_38 + 4) = uVar3;
  {
    u32 uVar1b;
    u16 uVar4b;

    uVar1b = fn_801F02AC(0x10,r3,r5);
    cVar6 = fightOutPokemonCheckFightOut();
    if (cVar6 == '\x01') {
      found = fn_801F02AC(2,uVar1b,r5);
      if (found == 0) {
        uVar3 = 0;
      }
      else {
        uVar4b = fightSideGetStatus(found,0,5,0);
        sVar5 = fn_801F0134(uVar1b,r5);
        if (sVar5 == 0) {
          uVar3 = 0;
        }
        else {
          fn_801F0234();
          uVar3 = fn_801F0204();
          if ((int)uVar3 < 0) {
            uVar3 = 0;
          }
          else {
            uVar3 = fightSideGetStatus(0,uVar4b,3,uVar3 & 0xffff);
          }
        }
      }
    }
    else {
      uVar3 = 0;
    }
  }
  *(u32 *)(auStack_38 + 0xc) = uVar3;
  *(u32 *)(auStack_38 + 0x14) = 0;
  *(u32 *)(auStack_38 + 0x1c) = 0;
  auStack_38[0x20] = 2;
  auStack_38[0x21] = fn_801F18DC(0);
  iVar2 = fn_80011288((int*)auStack_38,0,1);
  if (iVar2 < 0) {
    fn_8001120C(1);
    return 0;
  }
  if (iVar2 != 0) goto LAB_00261cb0;
  uVar1 = fn_801F02AC(0xf,r3,r5);
  goto LAB_00261ccc;
LAB_00261cb0:
  if (iVar2 == 1) {
    uVar1 = fn_801F02AC(0x10,r3,r5);
LAB_00261ccc:
    cVar6 = fightOutPokemonCheckFightOut(uVar1);
    if (cVar6 != '\0') {
      fightOutPokemonGetPokemonPtr(uVar1);
      cVar6 = pokemonIsDarkPokemon();
      if (cVar6 == '\0') {
        uVar3 = fn_801906A0(0x99f);
        if (uVar3 == 0) {
          winMsgOpenFight(0x7716,1,1);
        }
        else {
          winMsgOpenFight(0x7702,1,1);
        }
        fn_801F000C(0x40);
        winMsgCloseFight(0);
        goto LAB_00261af4;
      }
      fn_8001120C(1);
      return uVar1;
    }
  }
  goto LAB_00261af4;
}

/* Address: 0x8026503C | Size: 0x2F0 | Ghidra import */
u32 _fightMenuFightTrainerGcHeroOpenMenuSubWazaSelectDefensePokemon__FP15FightOutPokemonUsUs(u32 r3,u32 r4,u32 r5)

{
    extern u8 lbl_8047B678;
  u16 uVar4;
  u32 uVar1;
  u8 cVar6;
  int iVar2;
  u16 sVar5;
  u32 uVar3;
  u32 found;

  u8 auStack_38 [0x24];
  u32 local_34;
  u32 local_2c;
  u32 local_24;
  u32 local_1c;
  u8 local_18;
  u8 local_17;
  
LAB_00262054:
  do {
    uVar1 = fn_801F02AC(0xf,r3,r5);
    cVar6 = fightOutPokemonCheckFightOut();
    if (cVar6 == '\x01') {
      found = fn_801F02AC(2,uVar1,r5);
      if (found == 0) {
        uVar3 = 0;
      }
      else {
        uVar4 = fightSideGetStatus(found,0,5,0);
        sVar5 = fn_801F0134(uVar1,r5);
        if (sVar5 == 0) {
          uVar3 = 0;
        }
        else {
          fn_801F0234();
          uVar3 = fn_801F0204();
          if ((int)uVar3 < 0) {
            uVar3 = 0;
          }
          else {
            uVar3 = fightSideGetStatus(0,uVar4,3,uVar3 & 0xffff);
          }
        }
      }
    }
    else {
      uVar3 = 0;
    }
    *(u32 *)(auStack_38 + 4) = uVar3;
    {
      u32 uVar1b;
      u16 uVar4b;

      uVar1b = fn_801F02AC(0x10,r3,r5);
      cVar6 = fightOutPokemonCheckFightOut();
      if (cVar6 == '\x01') {
        found = fn_801F02AC(2,uVar1b,r5);
        if (found == 0) {
          uVar3 = 0;
        }
        else {
          uVar4b = fightSideGetStatus(found,0,5,0);
          sVar5 = fn_801F0134(uVar1b,r5);
          if (sVar5 == 0) {
            uVar3 = 0;
          }
          else {
            fn_801F0234();
            uVar3 = fn_801F0204();
            if ((int)uVar3 < 0) {
              uVar3 = 0;
            }
            else {
              uVar3 = fightSideGetStatus(0,uVar4b,3,uVar3 & 0xffff);
            }
          }
        }
      }
      else {
        uVar3 = 0;
      }
    }
    *(u32 *)(auStack_38 + 0xc) = uVar3;
    {
      u32 uVar1c;
      u16 uVar4c;

      uVar1c = fn_801F02AC(0xe,r3,r5);
      cVar6 = fightOutPokemonCheckFightOut();
      if (cVar6 == '\x01') {
        found = fn_801F02AC(2,uVar1c,r5);
        if (found == 0) {
          uVar3 = 0;
        }
        else {
          uVar4c = fightSideGetStatus(found,0,5,0);
          sVar5 = fn_801F0134(uVar1c,r5);
          if (sVar5 == 0) {
            uVar3 = 0;
          }
          else {
            fn_801F0234();
            uVar3 = fn_801F0204();
            if ((int)uVar3 < 0) {
              uVar3 = 0;
            }
            else {
              uVar3 = fightSideGetStatus(0,uVar4c,3,uVar3 & 0xffff);
            }
          }
        }
      }
      else {
        uVar3 = 0;
      }
    }
    *(u32 *)(auStack_38 + 0x14) = uVar3;
    *(u32 *)(auStack_38 + 0x1c) = 0;
    auStack_38[0x20] = 3;
    auStack_38[0x21] = fn_801F18DC(0);
    iVar2 = fn_80011288((int*)auStack_38,0,1);
    lbl_8047B678 = 1;
    if (iVar2 < 0) {
      fn_8001120C(1);
      return 0;
    }
    if (iVar2 == 0) {
      uVar1 = fn_801F02AC(0xf,r3,r5);
    }
    else if (iVar2 == 1) {
      uVar1 = fn_801F02AC(0x10,r3,r5);
    }
    else {
      if (iVar2 != 2) goto LAB_00262054;
      uVar1 = fn_801F02AC(0xe,r3,r5);
    }
    cVar6 = fightOutPokemonCheckFightOut(uVar1);
    if (cVar6 != '\0') {
      fn_8001120C(1);
      return uVar1;
    }
  } while (1);
}

/* Address: 0x8026532C | Size: 0xD0 | Ghidra import */
void fn_8026532C(u32 r3, u32 r4, u32 r5)
{
  u32 iVar1;
  u16 uVar3;
  u16 sVar4;
  u32 uVar2;
  u32 uVar6;

  iVar1 = fn_801F02AC(2, r3, r4);
  if (iVar1 == 0) {
    uVar6 = 0;
  }
  else {
    uVar3 = fightSideGetStatus(iVar1, 0, 5, 0);
    sVar4 = fn_801F0134(r3, r4);
    if (sVar4 == 0) {
      uVar6 = 0;
    }
    else {
      fn_801F0234();
      uVar2 = fn_801F0204();
      if ((int)uVar2 < 0) {
        uVar6 = 0;
      }
      else {
        uVar6 = fightSideGetStatus(0, uVar3, 3, uVar2 & 0xffff);
      }
    }
  }
  if ((u8)menuIsCheck(uVar6) != 0) {
    menuCloseCustom(uVar6, 0, r5);
  }
}

/* Address: 0x802653FC | Size: 0x19C | Ghidra import */
void fightMenuFightOutPokemonRenewStatusMenu(u32 r3, u32 r4, u32 r5)
{
  typedef struct StatusMenuCopy {
    u32 word[12];
  } StatusMenuCopy;
  typedef struct StatusMenuOut {
    StatusMenuCopy copy;
    u32 pad[2];
  } StatusMenuOut;
  extern int fn_801FE168();
  extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
  u32 r28;
  u32 r29;
  u32 r30;
  u32 iVar3;
  u16 uVar5;
  u16 sVar6;
  u32 uVar4;
  u32 checkStatus;
  u32 uVar11;
  StatusMenuOut local_48;
  StatusMenuCopy local_78;

  r28 = r3;
  r29 = r4;
  r30 = r5;
  iVar3 = fn_801F02AC(2, r28, r29);
  if (iVar3 == 0) {
    checkStatus = 0;
  }
  else {
    uVar5 = fightSideGetStatus(iVar3, 0, 5, 0);
    sVar6 = fn_801F0134(r28, r29);
    if (sVar6 == 0) {
      checkStatus = 0;
    }
    else {
      fn_801F0234();
      uVar4 = fn_801F0204();
      if ((int)uVar4 < 0) {
        checkStatus = 0;
      }
      else {
        checkStatus = fightSideGetStatus(0, uVar5, 3, uVar4 & 0xffff);
      }
    }
  }
  if ((u8)menuIsCheck(checkStatus) != 0) {
    iVar3 = fn_801F02AC(2, r28, r29);
    if (iVar3 == 0) {
      uVar11 = 0;
    }
    else {
      uVar5 = fightSideGetStatus(iVar3, 0, 5, 0);
      sVar6 = fn_801F0134(r28, r29);
      if (sVar6 == 0) {
        uVar11 = 0;
      }
      else {
        fn_801F0234();
        uVar4 = fn_801F0204();
        if ((int)uVar4 < 0) {
          uVar11 = 0;
        }
        else {
          uVar11 = fightSideGetStatus(0, uVar5, 3, uVar4 & 0xffff);
        }
      }
    }
    fn_801FE168(r28, &local_78);
    if ((u8)r30 == 0) {
      *(u8 *)((u8 *)&local_78 + 0x29) = 0;
    }
    local_48.copy = local_78;
    menuOpenCustom(uVar11, -1, 0, 0, 0, 1, &local_48.copy);
  }
}

/* Address: 0x80265598 | Size: 0x114 | Ghidra import */
void fn_80265598(u32 r3, u32 r4, u32 r5)
{
  typedef struct StatusMenuCopy {
    u32 word[12];
  } StatusMenuCopy;
  typedef struct StatusMenuOut {
    StatusMenuCopy copy;
    u32 pad[2];
  } StatusMenuOut;
  extern int fn_801FE168();
  extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
  u32 r28;
  u32 r29;
  u32 r30;
  u32 iVar3;
  u16 uVar5;
  u16 sVar6;
  u32 uVar4;
  u32 uVar11;
  StatusMenuOut local_48;
  StatusMenuCopy local_78;

  r28 = r3;
  r29 = r4;
  r30 = r5;
  iVar3 = fn_801F02AC(2, r28, r29);
  if (iVar3 == 0) {
    uVar11 = 0;
  }
  else {
    uVar5 = fightSideGetStatus(iVar3, 0, 5, 0);
    sVar6 = fn_801F0134(r28, r29);
    if (sVar6 == 0) {
      uVar11 = 0;
    }
    else {
      fn_801F0234();
      uVar4 = fn_801F0204();
      if ((int)uVar4 < 0) {
        uVar11 = 0;
      }
      else {
        uVar11 = fightSideGetStatus(0, uVar5, 3, uVar4 & 0xffff);
      }
    }
  }
  fn_801FE168(r28, &local_78);
  if ((u8)r30 == 0) {
    *(u8 *)((u8 *)&local_78 + 0x29) = 0;
  }
  local_48.copy = local_78;
  menuOpenCustom(uVar11, -1, 0, 0, 0, 1, &local_48.copy);
}

/* Address: 0x802656AC | Size: 0xA8 | Ghidra import */
u32 fightMenuGetFightOutPokemonPtrToStatusMenuId(u32 r3, u32 r4)
{
  u32 iVar1;
  u16 uVar3;
  u16 sVar4;
  u32 uVar2;
  u32 uVar6;

  iVar1 = fn_801F02AC(2, r3, r4);
  if (iVar1 == 0) {
    uVar6 = 0;
  }
  else {
    uVar3 = fightSideGetStatus(iVar1, 0, 5, 0);
    sVar4 = fn_801F0134(r3, r4);
    if (sVar4 == 0) {
      uVar6 = 0;
    }
    else {
      fn_801F0234();
      uVar2 = fn_801F0204();
      if ((int)uVar2 < 0) {
        uVar6 = 0;
      }
      else {
        uVar6 = fightSideGetStatus(0, uVar3, 3, uVar2 & 0xffff);
      }
    }
  }
  return uVar6;
}

/* Address: 0x80265754 | Size: 0x174 | Ghidra import */
void fightMenuFightTrainerRenewStatusMenu(u32 r3,u32 r4)

{
  typedef struct BattleStatusPair {
      u32 unk0;
      u16 unk4;
  } BattleStatusPair;
  extern void menuOpenCustom(u32, u32, u32, u32, u32, u32, ...);
  u32 iVar1;
  u32 uVar2;
  u8 cVar6;
  u16 uVar4;
  u16 sVar5;
  u32 uVar3;
  BattleStatusPair local_20;
  BattleStatusPair local_28;
  
  iVar1 = fn_801F02AC(2,r3,r4);
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    uVar4 = fightSideGetStatus(iVar1,0,5,0);
    sVar5 = fn_801F0134(r3,r4);
    if (sVar5 == 0) {
      uVar2 = 0;
    }
    else {
      fn_801F0234();
      uVar3 = fn_801F0204();
      if ((int)uVar3 < 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = fightSideGetStatus(0,uVar4,2,uVar3 & 0xffff);
      }
    }
  }
  cVar6 = menuIsCheck(uVar2);
  if (cVar6 != '\0') {
    iVar1 = fn_801F02AC(2,r3,r4);
    if (iVar1 == 0) {
      uVar2 = 0;
    }
    else {
      uVar4 = fightSideGetStatus(iVar1,0,5,0);
      sVar5 = fn_801F0134(r3,r4);
      if (sVar5 == 0) {
        uVar2 = 0;
      }
      else {
        fn_801F0234();
        uVar3 = fn_801F0204();
        if ((int)uVar3 < 0) {
          uVar2 = 0;
        }
        else {
          uVar2 = fightSideGetStatus(0,uVar4,2,uVar3 & 0xffff);
        }
      }
    }
    fn_801F7954(r3,&local_28);
    local_20.unk0 = local_28.unk0;
    local_20.unk4 = local_28.unk4;
    menuOpenCustom(uVar2,0,0,0,0,1,&local_20);
  }
  return;
}

typedef struct ColosseumBattleTimerState {
  u8 done;
  u8 forceDone;
  u8 pad[2];
  f32 elapsed;
  f32 limit;
  u32 thread;
} ColosseumBattleTimerState;

/* Address: 0x802658C8 | Size: 0x5C | Ghidra import */
void fightTimerCommandTerminate(void)
{
    extern ColosseumBattleTimerState lbl_80478800;
    extern f32 lbl_8047E6D8;
    extern void fn_800F05A0(u32);
    ColosseumBattleTimerState *state;

    state = &lbl_80478800;
    if (state->thread != 0) {
        fn_800F05A0(state->thread);
    }
    lbl_80478800.done = 0;
    state->thread = 0;
    lbl_80478800.elapsed = lbl_8047E6D8;
    lbl_80478800.limit = lbl_8047E6D8;
    lbl_80478800.forceDone = 0;
}

/* Address: 0x80265924 | Size: 0x38 | Ghidra import */
u32 fightTimerCommandIsOver(void)
{
    extern ColosseumBattleTimerState lbl_80478800;
    ColosseumBattleTimerState *state;

    state = &lbl_80478800;
    if (state->forceDone == 1) {
        return 0;
    }
    if (state->thread == 0) {
        return 1;
    }
    return state->done;
}

/* Address: 0x8026595C | Size: 0x48 | Ghidra import */
f32 fightTimerCommandGetNokoriTime(void)
{
    extern ColosseumBattleTimerState lbl_80478800;
    extern f32 lbl_8047E6D8;
    extern f32 lbl_8047E6DC;
    extern f32 lbl_8047E6E8;
    ColosseumBattleTimerState *state;

    state = &lbl_80478800;
    if (state->forceDone == 1) {
        return lbl_8047E6E8;
    }
    if (state->thread == 0) {
        return lbl_8047E6D8;
    }
    return state->limit - state->elapsed / lbl_8047E6DC;
}

/* Address: 0x802659A4 | Size: 0x54 | Ghidra import */
u32 fightTimerCommandBlock(void)
{
    extern ColosseumBattleTimerState lbl_80478800;
    extern void fn_800F0438(u32);
    ColosseumBattleTimerState *state;

    state = &lbl_80478800;
    if (state->forceDone == 1) {
        return 0;
    }
    if (state->thread == 0) {
        return 0;
    }
    fn_800F0438(state->thread);
    return 1;
}

/* Address: 0x802659F8 | Size: 0x74 | Ghidra import */
void fightTimerCommandStart(void)
{
    extern ColosseumBattleTimerState lbl_80478800;
    extern u32 fn_800FF560(void);
    extern u32 GSthreadCreate(u32, u32, u32, u32, u32, u32);
    extern void fn_800F0654(u32, u32, ...);
    extern void fightTimerThreadFunc(u8 *);
    u32 thread;

    if (lbl_80478800.forceDone != 1) {
        thread = GSthreadCreate(1, fn_800FF560(), 0x4000, 1, 0, (u32)fightTimerThreadFunc);
        if (thread != 0) {
            lbl_80478800.thread = thread;
            fn_800F0654(thread, 1);
        }
    }
}

/* Address: 0x80265A6C | Size: 0xD0 | Ghidra import */
void fightTimerCommandInit(void)
{
    extern ColosseumBattleTimerState lbl_80478800;
    extern f32 lbl_8047E6D8;
    extern f32 lbl_8047E6E8;
    extern s32 fn_80077B84(void);
    extern void fn_800F05A0(u32);
    ColosseumBattleTimerState *state;
    f32 limit;

    limit = (f32)fn_80077B84();
    state = &lbl_80478800;
    if (state->thread != 0) {
        if (state->thread != 0) {
            fn_800F05A0(state->thread);
        }
        lbl_80478800.done = 0;
        state->thread = 0;
        lbl_80478800.elapsed = lbl_8047E6D8;
        lbl_80478800.limit = lbl_8047E6D8;
        lbl_80478800.forceDone = 0;
    }
    lbl_80478800.done = 0;
    lbl_80478800.elapsed = lbl_8047E6D8;
    if (limit < lbl_8047E6D8) {
        lbl_80478800.forceDone = 1;
        lbl_80478800.limit = lbl_8047E6E8;
    }
    else {
        lbl_80478800.forceDone = 0;
        lbl_80478800.limit = limit;
    }
    state->thread = 0;
}

/* Address: 0x80265B3C | Size: 0x38 | Ghidra import */
u32 fightTimerAllIsOver(void)
{
    extern ColosseumBattleTimerState lbl_80478810;
    ColosseumBattleTimerState *state;

    state = &lbl_80478810;
    if (state->forceDone == 1) {
        return 0;
    }
    if (state->thread == 0) {
        return 1;
    }
    return state->done;
}

/* Address: 0x80265B74 | Size: 0x48 | Ghidra import */
f32 fightTimerAllGetNokoriTime(void)
{
    extern ColosseumBattleTimerState lbl_80478810;
    extern f32 lbl_8047E6D8;
    extern f32 lbl_8047E6DC;
    extern f32 lbl_8047E6E8;
    ColosseumBattleTimerState *state;

    state = &lbl_80478810;
    if (state->forceDone == 1) {
        return lbl_8047E6E8;
    }
    if (state->thread == 0) {
        return lbl_8047E6D8;
    }
    return state->limit - state->elapsed / lbl_8047E6DC;
}

/* Address: 0x80265BBC | Size: 0x54 | Ghidra import */
u32 fightTimerAllBlock(void)
{
    extern ColosseumBattleTimerState lbl_80478810;
    extern void fn_800F0438(u32);
    ColosseumBattleTimerState *state;

    state = &lbl_80478810;
    if (state->forceDone == 1) {
        return 0;
    }
    if (state->thread == 0) {
        return 0;
    }
    fn_800F0438(state->thread);
    return 1;
}

/* Address: 0x80265C10 | Size: 0x74 | Ghidra import */
void fightTimerAllStart(void)
{
    extern ColosseumBattleTimerState lbl_80478810;
    extern u32 fn_800FF560(void);
    extern u32 GSthreadCreate(u32, u32, u32, u32, u32, u32);
    extern void fn_800F0654(u32, u32, ...);
    extern void fightTimerThreadFunc(u8 *);
    u32 thread;

    if (lbl_80478810.forceDone != 1) {
        thread = GSthreadCreate(1, fn_800FF560(), 0x4000, 1, 0, (u32)fightTimerThreadFunc);
        if (thread != 0) {
            lbl_80478810.thread = thread;
            fn_800F0654(thread, 1);
        }
    }
}

/* Address: 0x80265C84 | Size: 0xD0 | Ghidra import */
void fightTimerAllInit(void)
{
    extern ColosseumBattleTimerState lbl_80478810;
    extern f32 lbl_8047E6D8;
    extern f32 lbl_8047E6E8;
    extern s32 menuCBRule_GetBattleTimeLimit(void);
    extern void fn_800F05A0(u32);
    ColosseumBattleTimerState *state;
    f32 limit;

    limit = (f32)menuCBRule_GetBattleTimeLimit();
    state = &lbl_80478810;
    if (state->thread != 0) {
        if (state->thread != 0) {
            fn_800F05A0(state->thread);
        }
        lbl_80478810.done = 0;
        state->thread = 0;
        lbl_80478810.elapsed = lbl_8047E6D8;
        lbl_80478810.limit = lbl_8047E6D8;
        lbl_80478810.forceDone = 0;
    }
    lbl_80478810.done = 0;
    lbl_80478810.elapsed = lbl_8047E6D8;
    if (limit < lbl_8047E6D8) {
        lbl_80478810.forceDone = 1;
        lbl_80478810.limit = lbl_8047E6E8;
    }
    else {
        lbl_80478810.forceDone = 0;
        lbl_80478810.limit = limit;
    }
    state->thread = 0;
}

/* Address: 0x80265D54 | Size: 0x5C | Ghidra import */
void fightTimerAllTerminate(void)
{
    extern ColosseumBattleTimerState lbl_80478810;
    extern f32 lbl_8047E6D8;
    extern void fn_800F05A0(u32);
    ColosseumBattleTimerState *state;

    state = &lbl_80478810;
    if (state->thread != 0) {
        fn_800F05A0(state->thread);
    }
    lbl_80478810.done = 0;
    state->thread = 0;
    lbl_80478810.elapsed = lbl_8047E6D8;
    lbl_80478810.limit = lbl_8047E6D8;
    lbl_80478810.forceDone = 0;
}

/* Address: 0x80265E34 | Size: 0x48 | Ghidra import */
u32 exribbonSetEarthRibbon(u32 r3)

{
    extern int fn_8011D5D4();
  int iVar1;
  
  fn_8011D5D4(r3,1);
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
    extern int fn_8011D5F8();
  int iVar1;
  
  fn_8011D5F8(r3,1);
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
    extern u16 fn_8011E15C();
    extern int fn_8011E778();
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
  iVar4 = fn_8011E778(0xfa);
  if (iVar4 == 0) {
    uVar8 = 0;
  }
  else {
    uVar8 = fn_8011E15C();
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
    fn_80132A38(0x5d,0x3d2);
    uVar5 = 0x44bb;
  }
  else if ((r3 < 1) && (-1 < r3)) {
    fn_80132A38(0x5d,0x3d2);
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
    extern void fn_801DAC90();
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
    fn_801DAC90();
    if (iVar1 != 0) {
        fn_80165A20(iVar1, 0x32, uVar3);
    }
    if (iVar2 != 0) {
        fn_801659FC(iVar2, 0x32, uVar4);
    }
    fn_800FF660();
    fn_8011288C(0, 0x5960008);
}
#pragma push
#pragma optimize_for_size off
/* Address: 0x8025E3B0 | Size: 0x184 | Ghidra import */

int _cbWazaForget__FP7PokemonUsl(u32 r3,u32 r4)

{
    extern u8 lbl_80478288[];
    extern u32 lbl_80478DB0;
    extern f32 lbl_8047E680;

  u32 uVar1;
  int iVar2;
  int iVar3;
  u32 *snapshot;

  u32 view[3];
  u32 position[3];
  u32 rotation[3];
  u32 direction[3];
  u8 auStack_48 [4];
  u8 auStack_4c [4];
  u8 auStack_50 [4];
  f32 local_54;
  u8 auStack_58 [4];
  
  fadeSet((double)lbl_8047E680,3);
  fadeCheck(1);
  GSscene_GetCameraDirectionVector(direction);
  GSscene_GetCameraRotationVector(rotation);
  GSscene_GetCameraPositionVector(position);
  GSscene_GetCameraViewVector(view);
  uVar1 = GScameraGetActiveCamera();
  GScameraGetPerspective(uVar1,&local_54,auStack_50,auStack_4c,auStack_48);
  snapshot = (u32 *)lbl_80478288;
  snapshot[0] = direction[0];
  snapshot[1] = direction[1];
  snapshot[2] = direction[2];
  snapshot[3] = rotation[0];
  snapshot[4] = rotation[1];
  snapshot[5] = rotation[2];
  snapshot[6] = position[0];
  snapshot[7] = position[1];
  snapshot[8] = position[2];
  snapshot[9] = view[0];
  snapshot[10] = view[1];
  snapshot[11] = view[2];
  ((f32 *)snapshot)[12] = local_54;
  fn_801DAC90();
  iVar2 = fn_80097A38(r3,r4);
  if (iVar2 >= 4) {
    iVar2 = -1;
  }
  pokemonGetStatus(r3,0,0x6e,0);
  fn_801DADC0(1);
  iVar3 = loadSequence((int*)auStack_58,r3,(u16*)&lbl_80478DB0,1);
  if (iVar3 == 1) {
    fadeSet((double)lbl_8047E680,2);
    fn_8025DE54((u32*)auStack_58,(u16*)&lbl_80478DB0,1,1,1,0);
  }
  return (int)(signed char)iVar2;
}
#pragma pop
#pragma push
#pragma optimize_for_size off
/* Address: 0x8025E9BC | Size: 0x390 | Ghidra import */
void reliveCeremonyAll(u32 r3)

{
    extern s8 fn_8001E184();
    extern int fn_80029660();
    extern int fn_8011D904();
    extern int fn_8011DE68();
    extern u16 fn_8011EDF8();
    extern u32 fn_8011EE10();
    extern u16 fn_8011EE58();
    extern u16 fn_8011F228();
    extern u32 fn_8011F4F0();
    extern int pokemonSetDp();
    extern u8 fn_80121ADC();
    extern int fn_80121B4C();
    extern int pokemonEvolutionAll();
    extern u32 pokemonEvolutionCheck();
    extern u8 pokemonCheckValid();
    extern int fn_801EECD8();
    extern u32 lbl_80478DB0;
    extern f32 lbl_8047E680;
    extern f32 lbl_8047E688;

  u32 uVar1;
  s8 cVar7;
  u8 uVar8;
  u32 uVar2;
  u16 sVar5;
  u16 sVar6;
  u16 sVar7;
  int iVar3;
  u32 uVar4;

  u8 auStack_18 [8];
  u8 auStack_1c [4];
  u8 auStack_20 [4];
  u8 auStack_24 [4];
  u16 local_28 [2];

  local_28[0] = 0;
  uVar1 = heroGetStatus(0,3,r3 & 0xffff);
  uVar8 = pokemonCheckValid();
  if (uVar8 == 1) {
    pokemonGetStatus(uVar1,0,0x6e,0);
    uVar2 = fn_8011EE40(uVar1);
    fn_801EECD8(uVar2,1);
    uVar8 = fn_80121ADC(uVar1,0x3e);
    if (uVar8 != 0) {
      fn_80121B4C(uVar1,0x3e);
    }
    pokemonSetDp((double)lbl_8047E688,uVar1);
    fn_80165668(0x3f7,0,0xff);
    sVar5 = fn_8011F228(uVar1,0);
    if (sVar5 != 0) {
      uVar2 = fn_8011F4F0(uVar1);
      fn_80132A38(0x32,uVar2);
      fn_80132A38(0x39,sVar5);
      winMsgOpenField(0x3b10,1,0);
    }
    uVar4 = fn_8011EE10(uVar1);
    uVar2 = uVar4;
    fn_80132A38(0x2f,uVar2);
    winMsgOpenField(0x3b0b,1,0);
    winMsgCloseField(1);
    sVar7 = fn_8011EE58(uVar1);
    sVar6 = fn_8011EDF8(uVar1);
    fn_8011D904(uVar1,(u16)((sVar6 + sVar7) + 0x46));
    fn_8011DE68(uVar1,0);
    iVar3 = _expRecover__FP7PokemonUl(uVar1,uVar2);
    if (iVar3 == 1) {
      uVar4 = pokemonEvolutionCheck(uVar1,0,0,local_28,auStack_18);
      if (((uVar4 & 0xffff) != 0) && ((uVar4 & 0xffff) != 0xffff)) {
        fadeSet((double)lbl_8047E680,3);
        fadeCheck(1);
        fn_801DAC90();
        iVar3 = pokemonEvolutionAll(uVar1,uVar4,local_28[0],auStack_18,0,1,1,0);
        if (iVar3 == 0) {
          fn_801DADC0(1);
          iVar3 = loadSequence((int*)auStack_1c,uVar1,(u16*)&lbl_80478DB0,1);
          if (iVar3 == 1) {
            fadeSet((double)lbl_8047E680,2);
            fn_8025DE54((u32*)auStack_1c,(u16*)&lbl_80478DB0,1,1,1,0);
          }
          uVar2 = fn_8011F4F0(uVar1);
          fn_80132A38(0x32,uVar2);
        }
        else {
          uVar2 = fn_8011F4F0(uVar1);
          fn_80132A38(0x32,uVar2);
          fn_801DADC0(1);
          iVar3 = loadSequence((int*)auStack_20,uVar1,(u16*)&lbl_80478DB0,1);
          if (iVar3 == 1) {
            fadeSet((double)lbl_8047E680,2);
            fn_8025DE54((u32*)auStack_20,(u16*)&lbl_80478DB0,1,1,1,0);
          }
        }
      }
    }
    fn_80165668(0x3ca,0,0xff);
    exribbonSetNarionalRibbon(uVar1);
    winMsgOpenField(0x3b0c,1,0);
    winMsgOpenField(0x3b0d,1,1);
    cVar7 = fn_8001E184();
    if (cVar7 != '\0') {
      winMsgCloseField(1);
    }
    else {
      winMsgCloseField(1);
      fadeSet((double)lbl_8047E680,3);
      fadeCheck(1);
      fn_801DAC90();
      fn_80029660(2,r3);
      fn_801DADC0(1);
      iVar3 = loadSequence((int*)auStack_24,uVar1,(u16*)&lbl_80478DB0,1);
      if (iVar3 == 1) {
        fadeSet((double)lbl_8047E680,2);
        fn_8025DE54((u32*)auStack_24,(u16*)&lbl_80478DB0,1,1,1,0);
      }
    }
  }
  return;
}
#pragma pop

#pragma push
#pragma optimize_for_size off
/* Address: 0x8025ED4C | Ghidra import */

void reliveMain(void)

{
    extern u32 lbl_80478DA8;
    extern u32 lbl_8047B668;
    extern u32 lbl_8047B66C;
    extern f32 lbl_8047E680;
  BOOL bVar1;

		  u32 uVar2;
		  u8 cVar4;
		  int iVar3;

  u8 auStack_18 [16];
  
  if ((lbl_804782BC[0] == 0) || (lbl_804782BC[0] == 1)) {
    fn_801DADC0(1);
    if (lbl_804782BC[0] == 0) {
      uVar2 = heroGetStatus(0,3,lbl_804782BC[2] & 0xffff);
      cVar4 = pokemonCheckValid();
      if (cVar4 != '\x01') {
        bVar1 = 0;
      }
      else {
        pokemonGetStatus(uVar2,0,0x6e,0);
        iVar3 = loadSequence((int*)auStack_18,uVar2,(u16*)&lbl_80478DA8,2);
        if (iVar3 == 1) {
          fn_8025DE54((u32*)auStack_18,(u16*)&lbl_80478DA8,2,1,1,0);
          bVar1 = 1;
        }
        else {
          bVar1 = 0;
        }
      }
      if (bVar1 == 1) {
        reliveCeremonyAll(lbl_804782BC[2]);
      }
    }
    else if (lbl_804782BC[0] == 1) {
      uVar2 = heroGetStatus(0,3,lbl_804782BC[2] & 0xffff);
      cVar4 = pokemonCheckValid();
      if (cVar4 != '\x01') {
        bVar1 = 0;
      }
      else {
        pokemonSetStatus(uVar2,0,0xc5,0,0);
        pokemonGetStatus(uVar2,0,0x6e,0);
        iVar3 = loadSequence((int*)auStack_18,uVar2,(u16*)lbl_8039A6A8,4);
        if (iVar3 == 1) {
          fn_8025DE54((u32*)auStack_18,(u16*)lbl_8039A6A8,4,1,1,0);
          bVar1 = 1;
        }
        else {
          bVar1 = 0;
        }
      }
      if (bVar1 == 1) {
        reliveCeremonyAll(lbl_804782BC[2]);
      }
    }
    fadeSet((double)lbl_8047E680,3);
    fadeCheck(1);
				    fn_801DAC90();
						    lbl_804782BC[0] = -1;
						    lbl_8047B668 = 0xffffffff;
						    lbl_8047B66C = 0xffffffff;
				    *(u16 *)((u8 *)lbl_804782BC + 4) = 0;
    lbl_804782BC[2] = 0;
    fn_800FF660();
    fn_8011288C(0,0);
    _threadSwitch();
  }
  return;
}
#pragma pop

/* Address: 0x80264D58 | Size: 0x2E4 | Ghidra import */
u32 _fightMenuFightTrainerGcHeroOpenMenuSubWaza__FP13FIGHT_TRAINERP15FightOutPokemonUs(u32 r3,u32 r4,u32 r5)

{
    extern int menuFightCloseWaza();
    extern u32 fn_800117BC();
    extern int fn_801F4C14();
    extern int fn_801FE3F8();
    extern u8 fn_801FFEC8();
    extern u16 fightOutPokemonGetSoubiItemDataId();
    extern int fightOutPokemonCreateFightActionAttackWaza();
    extern int fn_8022B2CC();
    extern u32 _fightMenuFightTrainerGcHeroOpenMenuSubWazaSelectDefensePokemon__FP15FightOutPokemonUsUs();
    extern char lbl_80375CA8[];
    extern u32 *lbl_80478DF8;
    extern u8 lbl_8047B678;
    u32 saved_r27;
  u8 bVar8;
  u32 uVar1;
  u32 uVar2;
  u32 uVar3;
  u8 cVar9;
  u16 uVar7;
  u32 uVar4;
  u32 iVar5;
  u32 iVar6;

  u16 local_70 [2];
  u8 auStack_6c [69];
  
  fightTypeDataBiosGetPtr(r5);
  bVar8 = fightTypeDataBiosGetFightoutPokemonNum();
  fn_801FE3F8(r4,auStack_6c);
  auStack_6c[68] = fn_801F18DC(0);
  uVar1 = fightOutPokemonGetPokemonPtr(r4);
  do {
    do {
      while (1) {
        uVar2 = (int)pokemonGetStatus(r4,0,0x101,0);
        uVar3 = fn_800117BC((int*)auStack_6c,uVar2,1);
        if ((int)uVar3 < 0) {
          menuFightCloseWaza(1);
          return 0;
        }
        cVar9 = fn_801FFEC8(r4,(u16)uVar3,1,local_70);
        uVar7 = (int)pokemonGetStatus(uVar1,0,0x7f,uVar3 & 0xffff);
        if (cVar9 != '\0') {
          fn_80132A38(0x11,r4);
          wazaGetStatus(0,uVar7,1,0);
          uVar2 = fn_800FA280();
          fn_80132A38(0x28,uVar2);
          uVar7 = fightOutPokemonGetSoubiItemDataId(r4);
          fn_801F4C14(0,0,0x56,0,uVar7);
        }
        if (cVar9 == '\x06') {
          saved_r27 = 0x7661;
        }
        else if (cVar9 == '\x05') {
          wazaGetStatus(0,local_70[0],1,0);
          uVar2 = fn_800FA280();
          fn_80132A38(0x28,uVar2);
          saved_r27 = 0x76bb;
        }
        else if (cVar9 == '\x04') {
          saved_r27 = 0x7600;
        }
        else if (cVar9 == '\x03') {
          saved_r27 = 0x75ff;
        }
        else if (cVar9 == '\x02') {
          saved_r27 = 0x75fe;
        }
        else if (cVar9 == '\x01') {
          saved_r27 = 0x75fd;
        }
        if (cVar9 == '\0') break;
        if (saved_r27 != 0) {
          winMsgOpenFight(saved_r27,1,1);
        }
        fn_801F000C(0x40);
        winMsgCloseFight(0);
      }
      uVar4 = (int)pokemonGetStatus(uVar1,0,0x7f,(u16)uVar3);
      uVar4 = uVar4 & 0xffff;
    } while (((uVar4 == 0) || (uVar4 >= *lbl_80478DF8)) || (uVar4 == 0x165));
    lbl_8047B678 = 0;
    iVar5 = fn_8022B2CC(r4,uVar4,r5,_fightMenuFightTrainerGcHeroOpenMenuSubWazaSelectDefensePokemon__FP15FightOutPokemonUsUs,1,0,0xffffffff);
    cVar9 = fn_801F18DC(0);
  } while ((((cVar9 == '\x01') && (lbl_8047B678 == '\0')) &&
           ((bVar8 >= 2 && (iVar6 = _fightMenuFightTrainerGcHeroOpenMenuSubWazaSelectDefensePokemon__FP15FightOutPokemonUsUs(r4,uVar4,r5), iVar6 == 0)))) ||
          (iVar5 == 0));
  uVar1 = fn_801F0134(iVar5,r5);
  menuFightCloseWaza(1);
  fightOutPokemonCreateFightActionAttackWaza(r4,0,0x13,0,lbl_80375CA8,uVar4,uVar1,(int)(s8)uVar3,0);
  return 1;
}

/* Address: 0x802612D0 | Size: 0x5C | Ghidra import */

void evolution(void)
{
    extern u32 lbl_804787E0[];
    extern u32 evolutionStart();
    extern void fn_800FF660();
    extern void fn_8011288C();
    u32 *base = lbl_804787E0;
    base[6] = evolutionStart(base[0], base[1], base[2], (u16*)base[4], base[3], (u8*)base[5]);
    fn_800FF660();
    fn_8011288C(0, 0);
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
    sVar3 = fn_8011F228(r3,uVar6 & 0xffff);
    if (sVar3 == 0) break;
    uVar6 = uVar6 + 1;
  } while ((int)uVar6 < 4);
  if ((int)uVar6 < 4) {
LAB_0025d3c4:
    fn_80165668(0x4ca,0,0xff);
    uVar2 = fn_8011F4F0(r3);
    fn_80132A38(0x32,uVar2);
    fn_80132A38(0x39,r4 & 0xffff);
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
    uVar2 = fn_8011F4F0(r3);
    fn_80132A38(0x32,uVar2);
    fn_80132A38(0x39,r4 & 0xffff);
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
          uVar2 = fn_8011F4F0(r3);
          fn_80132A38(0x32,uVar2);
          fn_80132A38(0x5d,0x468);
          uVar4 = fn_8011F228(r3,uVar6 & 0xffff);
          fn_80132A38(0x39,uVar4);
          if (r6 == 0) {
            winMsgOpenField(0x4248,1,0);
          }
          else {
            winMsgOpen(2,0x4248,1,0);
          }
          goto LAB_0025d3c4;
        }
      }
      fn_80132A38(0x32,uVar2);
      fn_80132A38(0x39,r4 & 0xffff);
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

/* Address: 0x8025DC2C | Size: 0x90 | Ghidra import (PSQ removed) */


void waitTime__Ff(float r3)

{
    extern f32 lbl_8047E660;
    extern f32 lbl_8047E664;
    extern f64 lbl_8047E668;
  u32 uVar2;

  double dVar4;
  float fVar1;

  dVar4 = (double)(float)(lbl_8047E664 * r3);
  fVar1 = lbl_8047E660;
  while (fVar1 < dVar4) {
    _threadSwitch();
    uVar2 = fn_800D3088();
    fVar1 += (float)uVar2;
  }

  return;
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
  uVar5 = fn_8011F4F0(r5);
  fn_80132A38(0x32,uVar5);
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
      fn_8011F5C8(r5);
      iVar6 = fn_8011E778();
    }
  }
  if (iVar6 == 0) {
    uVar9 = 0;
  }
  else {
    uVar9 = fn_8011E15C();
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
        fn_8011F5C8(r5);
        iVar6 = fn_8011E778();
      }
    }
    if (iVar6 == 0) {
      uVar9 = 0;
    }
    else {
      uVar9 = fn_8011E15C();
      fn_80166A28(uVar9);
    }
    while (iVar6 = fn_801666BC(uVar9), iVar6 == 2) {
      _threadSwitch();
    }
    uVar5 = fn_8011F4F0(r5);
    fn_80132A38(0x32,uVar5);
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
        fn_8011F5C8(r6);
        iVar6 = fn_8011E778();
      }
    }
    if (iVar6 == 0) {
      uVar9 = 0;
    }
    else {
      uVar9 = fn_8011E15C();
      fn_80166A28(uVar9);
    }
    while (iVar6 = fn_801666BC(uVar9), iVar6 == 2) {
      _threadSwitch();
    }
    uVar5 = fn_8011F4F0(r5);
    fn_80132A38(0x32,uVar5);
    uVar9 = fn_8011F5C8(r6);
    fn_80132A38(0x4e,uVar9);
    fn_80132A38(0x5d,0x3d2);
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

/* Address: 0x80264488 | Size: 0x654 | Ghidra import (PSQ removed) */


u32 _fightMenuFightTrainerGcHeroOpenMenuSubItem__FP13FIGHT_TRAINERP15FightOutPokemonUs(u32 r3,int r4,u32 r5)

{
    extern s8 fn_8000DD0C();
    extern int fn_8000DD30();
    extern int fn_8000DD5C();
    extern s8 fn_8000DD98();
    extern int fn_8000DDBC();
    extern int fn_8000DDE8();
    extern u32 fn_80018F88();
    extern u8 fn_80019064();
    extern u32 fn_800D37CC();
    extern int menuReleaseOffScreen();
    extern int menuCreateOffScreen();
    extern s8 fn_80142CF4();
    extern int fn_801DA36C();
    extern int fn_801EFFC4();
    extern s8 fn_801F1758();
    extern s8 fn_801F7EF0();
    extern u16 fn_801F85B0();
    extern short fn_801F8638();
    extern int fn_8022FF90();
    extern f32 lbl_8047E6CC;
    extern f64 lbl_8047E6D0;

  int iVar1;
  u32 uVar2;
  short sVar4;
  s8 cVar6;
  u8 uVar7;
  int iVar3;
  u16 uVar5;
  u16 uVar8;
  u32 uVar9;
  u32 uVar10;

  u64 f30;
  double dVar11;
  u64 f31;
  double dVar12;
  u8 local_80;
  char local_7f [3];
  char local_7c [4];
  char local_78 [4];
  char local_74 [4];
  u32 local_70 [2];
  u32 local_68;
  u32 uStack_64;
  u8 auStack_18 [16];
  u8 auStack_8 [8];

  local_70[0] = 0;
  for (uVar8 = 0; uVar8 < 2; uVar8 = uVar8 + 1) {
    local_74[uVar8] = '\0';
    local_78[uVar8] = '\0';
    local_7c[uVar8] = '\0';
  }
  for (uVar9 = 0; (uVar9 & 0xffff) < 2; uVar9 = uVar9 + 1) {
    iVar1 = fn_801FB1C0(r3,0,0x46,uVar9);
    if (iVar1 != 0) {
      uVar5 = fn_801F85B0(r3,iVar1);
      fn_801FB1C0(r3,0,0x45,uVar5);
      cVar6 = fightPokemonCheckFightOut();
      uVar10 = uVar9 & 0xffff;
      local_74[uVar10] = cVar6;
      cVar6 = fn_802026E4(iVar1,8);
      local_78[uVar10] = cVar6;
      cVar6 = fn_802026E4(iVar1,7);
      local_7c[uVar10] = cVar6;
    }
  }
  iVar1 = fn_801FB1C0(r3,0,0x44,0);
  if (iVar1 != 0) {
    uVar2 = (int)pokemonGetStatus(r4,0,0xd6,0);
    sVar4 = fn_801F8638(r3,uVar2);
    local_70[0] = (u32)sVar4;
    dVar11 = lbl_8047E6D0;
    dVar12 = (double)lbl_8047E6CC;
LAB_0026160c:
    fn_801F2B5C(0,0x80261cbc,0,0);
    fn_801F37B0(0,0x80261fb4,0,0);
    cVar6 = fn_801F1700(0);
    if (cVar6 == '\x01') {
      fn_8000DDBC();
    }
    cVar6 = fn_801F1758(0);
    if (cVar6 == '\x01') {
      fn_8000DD30();
    }
    fn_801F2B5C(0,0x80261cbc,0,0);
    while (1) {
      local_7f[0] = '\x01';
      fn_801F2B5C(0,0x80261bec,local_7f,0);
      if (local_7f[0] == '\x01') break;
      _threadSwitch();
    }
    fn_801F37B0(0,0x80261fb4,0,0);
    while (cVar6 = fn_801F37B0(0,0x80261ef8,0,0), cVar6 != '\x01') {
      _threadSwitch();
    }
    cVar6 = fn_801F1700(0);
    if (cVar6 == '\x01') {
      while (cVar6 = fn_8000DD98(), cVar6 != '\0') {
        _threadSwitch();
      }
    }
    cVar6 = fn_801F1758(0);
    if (cVar6 == '\x01') {
      while (cVar6 = fn_8000DD0C(), cVar6 != '\0') {
        _threadSwitch();
      }
    }
    uStack_64 = fn_800D37CC();
    uStack_64 = uStack_64 ^ 0x80000000;
    local_68 = 0x43300000;
    menuCreateOffScreen((double)(float)(dVar12 / (double)(float)((double)(((u64)(0x43300000) << 32) | (u32)(uStack_64)) -
                                                         dVar11)));
    uVar9 = fn_80018F88(1,local_70,r3);
    fn_801EFFC4(10);
    uVar7 = fn_80019064();
    if (((uVar9 & 0xffff) != 0) && (cVar6 = itemGetStatus(0,uVar9,2,0), cVar6 == '\x02')) {
      for (uVar10 = 0; (uVar10 & 0xffff) < 2; uVar10 = uVar10 + 1) {
        iVar1 = fn_801FB1C0(r3,0,0x46,uVar10);
        if ((iVar1 != 0) && (iVar3 = (int)pokemonGetStatus(iVar1,0,0xee,0), iVar3 != 0)) {
          if ((local_78[uVar10 & 0xffff] == '\x01') &&
             (cVar6 = fn_802026E4(iVar1,8), cVar6 == '\0')) {
            fn_801DA36C(iVar3,1);
          }
          if ((local_7c[uVar10 & 0xffff] == '\x01') &&
             (cVar6 = fn_802026E4(iVar1,7), cVar6 == '\0')) {
            fn_801DA36C(iVar3,2);
          }
        }
      }
    }
    fn_801F2B5C(0,0x80261d8c,0,0);
    local_80 = 1;
    fn_801F37B0(0,0x80262084,&local_80,0);
    cVar6 = fn_801F1700(0);
    if (cVar6 == '\x01') {
      fn_8000DDE8();
    }
    cVar6 = fn_801F1758(0);
    if (cVar6 == '\x01') {
      fn_8000DD5C();
    }
    if ((r4 != 0) && (cVar6 = fn_801F18DC(0), cVar6 != '\x01')) {
      iVar1 = fn_801F02AC(2,r4,r5);
      if (iVar1 == 0) {
        uVar2 = 0;
      }
      else {
        uVar5 = fightSideGetStatus(iVar1,0,5,0);
        sVar4 = fn_801F0134(r4,r5);
        if (sVar4 == 0) {
          uVar2 = 0;
        }
        else {
          fn_801F0234();
          uVar10 = fn_801F0204();
          if ((int)uVar10 < 0) {
            uVar2 = 0;
          }
          else {
            uVar2 = fightSideGetStatus(0,uVar5,3,uVar10 & 0xffff);
          }
        }
      }
      fn_80011D9C(uVar2,1);
    }
    uStack_64 = fn_800D37CC();
    uStack_64 = uStack_64 ^ 0x80000000;
    local_68 = 0x43300000;
    menuReleaseOffScreen((double)(float)(dVar12 / (double)(float)((double)(((u64)(0x43300000) << 32) | (u32)(uStack_64)) -
                                                         dVar11)));
    fn_801EFFC4(10);
    if ((uVar9 & 0xffff) != 0) {
      cVar6 = itemGetStatus(0,uVar9,2,0);
      if (cVar6 == '\x01') goto code_r0x002619ac;
      cVar6 = itemGetStatus(0,uVar9,2,0);
      if (cVar6 == '\x02') {
        fn_8022FF90();
        for (uVar10 = 0; (uVar10 & 0xffff) < 2; uVar10 = uVar10 + 1) {
          iVar1 = fn_801FB1C0(r3,0,0x46,uVar10);
          if (((iVar1 != 0) && (local_74[uVar10 & 0xffff] == '\0')) &&
             (cVar6 = fightOutPokemonCheckFightOut(), cVar6 == '\x01')) {
            pokemonSetStatus(iVar1,0,0x120,0,1);
          }
        }
      }
      uVar2 = 1;
      uVar10 = local_70[0] & 0xffff;
      goto LAB_00261a8c;
    }
    uVar2 = 0;
    goto LAB_00261ab8;
  }
  uVar2 = 0;
LAB_00261ab8:

  return uVar2;
code_r0x002619ac:
  cVar6 = fn_801F7EF0(r3);
  if ((cVar6 == '\x01') || (iVar1 = _fightMenuFightTrainerGcHeroOpenMenuSubBallSelectTargetPokemon__FP15FightOutPokemonUsUs(r4,uVar9,r5), iVar1 == 0))
  goto LAB_0026160c;
  uVar10 = fn_801F0134(iVar1,r5);
  uVar2 = 0;
LAB_00261a8c:
  fightOutPokemonCreateFightActionUseItem(r4,0,0x12,0,0x80375d70,uVar9 & 0xffff,uVar10,uVar7,uVar2);
  uVar2 = 1;
  goto LAB_00261ab8;
}

/* Address: 0x80265DB0 | Size: 0x84 | Ghidra import (PSQ removed) */
void fightTimerThreadFunc(ColosseumBattleTimerState *r3)
{
    extern f32 lbl_8047E6D8;
    extern f32 lbl_8047E6DC;
    extern u32 fn_800D3088(void);
    extern void _threadSwitch(void);
  u32 ticks;
  f32 scale;

  r3->elapsed = lbl_8047E6D8;
  scale = lbl_8047E6DC;
  while (r3->elapsed < scale * r3->limit) {
    _threadSwitch();
    ticks = fn_800D3088();
    r3->elapsed = (f32)ticks + r3->elapsed;
  }
  r3->done = 1;
  do {
    _threadSwitch();
  } while (1);
}
