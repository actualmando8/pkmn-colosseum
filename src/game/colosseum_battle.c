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
 *   fn_80240BD0 (0xA90 bytes): Large battle orchestration function.
 *     Manages the high-level flow of a battle encounter including
 *     pre-battle dialogue, party selection, and battle initiation.
 *
 *   fn_80245FC4 (0x1084 bytes): Second-largest in this region.
 *     Complex battle setup that handles multi-round encounters.
 *     The code shows repeated patterns of:
 *       CheckTrainerPokemonFlag -> PreBattleSetup -> BattleSequenceStart
 *     with different sequence IDs (0xF1, 0xF2, 0xF3, 0xF4).
 *
 *   fn_8024E690 (0x1224 bytes): Third-largest. Appears to handle
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
 *   fn_8026316C (0xA5C bytes): Large utility function near the end.
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
 * =========================================================================
 */

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

extern void* fn_8012640C(void* context, u32 slot, u16 tableId, u32 flags);
extern u32   fn_801254B4(void* context, u32 slot, u16 tableId, u32 flags, u32 value);

/* Battle system functions */
extern void fn_801EF8F4(u32 param);

/* Sound functions */
extern void fn_801657F8(void);     /* Stop sound */
extern void fn_80165A20(u32 param); /* Fade out music */
extern void fn_801659FC(u32 bgmId); /* Start BGM */

/* Forward declarations for converted functions */
void fn_80240BD0(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_80245FC4(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_8024E690(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_8025A290(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType);
void fn_8026316C(void* ctx, u32 param1, u32 param2, u32 param3);


/* =========================================================================
 * fn_80240BD0 - BattleOrchestrator
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
/* TODO: Decompile fn_80240BD0 (2704 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80240BD0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_802358AC();
    extern void fn_80235910();
    extern void fn_80235974();
    extern void fn_802359D8();
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r6 = 0x1;
    r7 = 0x1;
    /* stmw r21, 0x94(r1) */;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    r5 = r1 + 0x68;
    r4 = r26;
    r29 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r30 = r3;
    r4 = r26;
    r5 = r1 + 0x48;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r31 = r3;
    r24 = r1 + 0x40;
    r25 = r1 + 0x48;
    r22 = 0x0;
    r23 = r3 & 0xFFFF;
    goto L_80240D08;
L_80240C3C: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r25 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x40) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x41) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0x42) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0x43) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0x44) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0x45) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0x46) = r3;
    r3 = 0x0;
    goto L_80240CE0;
L_80240CBC: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r24 + r0);
    if ((u32)r0 < (u32)0x8) goto L_80240CDC;
    if ((u32)r0 > (u32)0x9) goto L_80240CDC;
    r0 = 0x1;
    goto L_80240CF0;
L_80240CDC: ;
    r3 = r3 + 0x1;
L_80240CE0: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80240CBC;
    r0 = 0x0;
L_80240CF0: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240D04;
    r0 = 0x1;
    goto L_80240D18;
L_80240D04: ;
    r22 = r22 + 0x1;
L_80240D08: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_80240C3C;
    r0 = 0x0;
L_80240D18: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240D6C;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x1b6;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b6;
    fn_80239EE8();
L_80240D6C: ;
    r24 = r1 + 0x38;
    r25 = r1 + 0x68;
    r23 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_80240E4C;
L_80240D80: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r25 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x38) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x39) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0x3A) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0x3B) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0x3C) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0x3D) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0x3E) = r3;
    r3 = 0x0;
    goto L_80240E24;
L_80240E00: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r24 + r0);
    if ((u32)r0 < (u32)0x3) goto L_80240E20;
    if ((u32)r0 > (u32)0x4) goto L_80240E20;
    r0 = 0x1;
    goto L_80240E34;
L_80240E20: ;
    r3 = r3 + 0x1;
L_80240E24: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80240E00;
    r0 = 0x0;
L_80240E34: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240E48;
    r0 = 0x1;
    goto L_80240E5C;
L_80240E48: ;
    r22 = r22 + 0x1;
L_80240E4C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_80240D80;
    r0 = 0x0;
L_80240E5C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240EB0;
    r3 = r29;
    r4 = r26;
    r5 = 0x1b7;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b7;
    fn_80239EE8();
L_80240EB0: ;
    r24 = r1 + 0x30;
    r25 = r1 + 0x48;
    r23 = r31 & 0xFFFF;
    r22 = 0x0;
    goto L_80240F90;
L_80240EC4: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r25 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x30) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x31) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0x32) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0x33) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0x34) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0x35) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0x36) = r3;
    r3 = 0x0;
    goto L_80240F68;
L_80240F44: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r24 + r0);
    if ((u32)r0 < (u32)0xa) goto L_80240F64;
    if ((u32)r0 > (u32)0xc) goto L_80240F64;
    r0 = 0x1;
    goto L_80240F78;
L_80240F64: ;
    r3 = r3 + 0x1;
L_80240F68: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80240F44;
    r0 = 0x0;
L_80240F78: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240F8C;
    r0 = 0x1;
    goto L_80240FA0;
L_80240F8C: ;
    r22 = r22 + 0x1;
L_80240F90: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_80240EC4;
    r0 = 0x0;
L_80240FA0: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240FF4;
    r3 = r29;
    r4 = r26;
    r5 = 0x1b8;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b8;
    fn_80239EE8();
L_80240FF4: ;
    r24 = r1 + 0x28;
    r25 = r1 + 0x68;
    r23 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_802410D4;
L_80241008: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r25 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x28) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x29) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0x2A) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0x2B) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0x2C) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0x2D) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0x2E) = r3;
    r3 = 0x0;
    goto L_802410AC;
L_80241088: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r24 + r0);
    if ((u32)r0 < (u32)0x0) goto L_802410A8;
    if ((u32)r0 > (u32)0x2) goto L_802410A8;
    r0 = 0x1;
    goto L_802410BC;
L_802410A8: ;
    r3 = r3 + 0x1;
L_802410AC: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80241088;
    r0 = 0x0;
L_802410BC: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802410D0;
    r0 = 0x1;
    goto L_802410E4;
L_802410D0: ;
    r22 = r22 + 0x1;
L_802410D4: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_80241008;
    r0 = 0x0;
L_802410E4: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241138;
    r3 = r29;
    r4 = r26;
    r5 = 0x1b9;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b9;
    fn_80239EE8();
L_80241138: ;
    r24 = r1 + 0x20;
    r25 = r1 + 0x68;
    r23 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_80241218;
L_8024114C: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r25 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x20) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x21) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0x22) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0x23) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0x24) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0x25) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0x26) = r3;
    r3 = 0x0;
    goto L_802411F0;
L_802411CC: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r24 + r0);
    if ((u32)r0 < (u32)0x8) goto L_802411EC;
    if ((u32)r0 > (u32)0x9) goto L_802411EC;
    r0 = 0x1;
    goto L_80241200;
L_802411EC: ;
    r3 = r3 + 0x1;
L_802411F0: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_802411CC;
    r0 = 0x0;
L_80241200: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241214;
    r0 = 0x1;
    goto L_80241228;
L_80241214: ;
    r22 = r22 + 0x1;
L_80241218: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_8024114C;
    r0 = 0x0;
L_80241228: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024127C;
    r3 = r29;
    r4 = r26;
    r5 = 0x1ba;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1ba;
    fn_80239EE8();
L_8024127C: ;
    r24 = r1 + 0x18;
    r25 = r1 + 0x48;
    r23 = r31 & 0xFFFF;
    r22 = 0x0;
    goto L_8024135C;
L_80241290: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r25 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x18) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x19) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0x1A) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0x1B) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0x1C) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0x1D) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0x1E) = r3;
    r3 = 0x0;
    goto L_80241334;
L_80241310: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r24 + r0);
    if ((u32)r0 < (u32)0x3) goto L_80241330;
    if ((u32)r0 > (u32)0x4) goto L_80241330;
    r0 = 0x1;
    goto L_80241344;
L_80241330: ;
    r3 = r3 + 0x1;
L_80241334: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80241310;
    r0 = 0x0;
L_80241344: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241358;
    r0 = 0x1;
    goto L_8024136C;
L_80241358: ;
    r22 = r22 + 0x1;
L_8024135C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_80241290;
    r0 = 0x0;
L_8024136C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802413C0;
    r3 = r29;
    r4 = r26;
    r5 = 0x1bb;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1bb;
    fn_80239EE8();
L_802413C0: ;
    r25 = r1 + 0x10;
    r23 = r1 + 0x68;
    r24 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_802414A0;
L_802413D4: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r23 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x10) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x11) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0x12) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0x13) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0x14) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0x15) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0x16) = r3;
    r3 = 0x0;
    goto L_80241478;
L_80241454: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r25 + r0);
    if ((u32)r0 < (u32)0xa) goto L_80241474;
    if ((u32)r0 > (u32)0xc) goto L_80241474;
    r0 = 0x1;
    goto L_80241488;
L_80241474: ;
    r3 = r3 + 0x1;
L_80241478: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80241454;
    r0 = 0x0;
L_80241488: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024149C;
    r0 = 0x1;
    goto L_802414B0;
L_8024149C: ;
    r22 = r22 + 0x1;
L_802414A0: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_802413D4;
    r0 = 0x0;
L_802414B0: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241504;
    r3 = r29;
    r4 = r26;
    r5 = 0x1bc;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1bc;
    fn_80239EE8();
L_80241504: ;
    r25 = r1 + 0x8;
    r24 = r1 + 0x48;
    r30 = r31 & 0xFFFF;
    r22 = 0x0;
    goto L_802415E4;
L_80241518: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r26;
    r21 = *(u32*)(r24 + r0);
    r4 = r21;
    fn_80235AA0();
    *(u8*)(sp + 0x8) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235A3C();
    *(u8*)(sp + 0x9) = r3;
    r3 = r26;
    r4 = r21;
    fn_802359D8();
    *(u8*)(sp + 0xA) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235974();
    *(u8*)(sp + 0xB) = r3;
    r3 = r26;
    r4 = r21;
    fn_80235910();
    *(u8*)(sp + 0xC) = r3;
    r3 = r26;
    r4 = r21;
    fn_802358AC();
    *(u8*)(sp + 0xD) = r3;
    r3 = r26;
    r4 = r21;
    fn_802357CC();
    *(u8*)(sp + 0xE) = r3;
    r3 = 0x0;
    goto L_802415BC;
L_80241598: ;
    r0 = r3 & 0xFF;
    r0 = *(u8*)(r25 + r0);
    if ((u32)r0 < (u32)0x0) goto L_802415B8;
    if ((u32)r0 > (u32)0x2) goto L_802415B8;
    r0 = 0x1;
    goto L_802415CC;
L_802415B8: ;
    r3 = r3 + 0x1;
L_802415BC: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80241598;
    r0 = 0x0;
L_802415CC: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802415E0;
    r0 = 0x1;
    goto L_802415F4;
L_802415E0: ;
    r22 = r22 + 0x1;
L_802415E4: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_80241518;
    r0 = 0x0;
L_802415F4: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241648;
    r3 = r29;
    r4 = r26;
    r5 = 0x1bd;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1bd;
    fn_80239EE8();
L_80241648: ;
    r3 = r29;
    /* lmw r21, 0x94(r1) */;
    return;
}
#pragma pop

/* =========================================================================
 * fn_80245FC4 - MultiBattleSetup
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
/* TODO: Decompile fn_80245FC4 (4228 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80245FC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80136468();
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fn_801F54A4();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
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
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    extern void fn_8024AFC4();
    extern u8 jumptable_8039A5D8[];
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    void (*ctr_fn)(void) = 0;

    /* stmw r24, 0x70(r1) */;
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
    if ((u32)r0 > (u32)0x1b) goto L_80247030;
    r3 = (u32)jumptable_8039A5D8;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8039A5D8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
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
    fn_802399FC();
    r26 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_80246100;
    r3 = r26;
    r4 = r31;
    r5 = 0x114;
    fn_80239984();
    r26 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x114;
    fn_80239EE8();
L_80246100: ;
    r3 = r31;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024614C;
    r3 = r31;
    r4 = r28;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024614C;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802461B8;
L_8024614C: ;
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
    fn_802399FC();
    r26 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x115;
    fn_80239CCC();
L_802461B8: ;
    r24 = r26;
    goto L_80247030;
    r4 = r31;
    r5 = r1 + 0x10;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10d;
    fn_80239CCC();
    r25 = r1 + 0x10;
    r26 = r26 & 0xFFFF;
    r24 = 0x0;
    goto L_80246308;
L_80246258: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* clrlslwi r0, r24, 16, 2 */;
    r4 = *(u32*)(r25 + r0);
    if ((u32)r3 == (u32)r4) goto L_80246304;
    r3 = r31;
    r5 = 0x8;
    fn_802384B4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246304;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10e;
    fn_80239CCC();
    goto L_80246314;
L_80246304: ;
    r24 = r24 + 0x1;
L_80246308: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_80246258;
L_80246314: ;
    r3 = r31;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r26) goto L_80246360;
    r3 = r31;
    r4 = r28;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80246360;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802463CC;
L_80246360: ;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10f;
    fn_80239CCC();
L_802463CC: ;
    r24 = r27;
    goto L_80247030;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_802464AC;
    r3 = r27;
    r4 = r31;
    r5 = 0xdc;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xdc;
    fn_80239EE8();
L_802464AC: ;
    r3 = r31;
    r4 = r28;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024652C;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xdd;
    fn_80239CCC();
L_8024652C: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024659C;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024659C;
    r3 = r31;
    r4 = r28;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024659C;
    r3 = r31;
    r4 = r28;
    r5 = 0x33;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246608;
L_8024659C: ;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xde;
    fn_80239CCC();
L_80246608: ;
    r24 = r27;
    goto L_80247030;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_802466E8;
    r3 = r27;
    r4 = r31;
    r5 = 0xc4;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc4;
    fn_80239EE8();
L_802466E8: ;
    r3 = r31;
    r4 = r28;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246768;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc5;
    fn_80239CCC();
L_80246768: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802467BC;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802467BC;
    r3 = r31;
    r4 = r28;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246828;
L_802467BC: ;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc6;
    fn_80239CCC();
L_80246828: ;
    r24 = r27;
    goto L_80247030;
    r4 = r31;
    r3 = 0x0;
    r5 = 0xbf;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_802468E0;
    r3 = r27;
    r4 = r31;
    r5 = 0xc0;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc0;
    fn_80239EE8();
L_802468E0: ;
    r3 = r31;
    r4 = r28;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246938;
    r3 = r27;
    r4 = r31;
    r5 = 0xc1;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc1;
    fn_80239EE8();
L_80246938: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802469A8;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802469A8;
    r3 = r31;
    r4 = r28;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802469A8;
    r3 = r31;
    r4 = r28;
    r5 = 0x34;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802469EC;
L_802469A8: ;
    r3 = r27;
    r4 = r31;
    r5 = 0xc2;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc2;
    fn_80239EE8();
L_802469EC: ;
    r24 = r27;
    goto L_80247030;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_80246ACC;
    r3 = r27;
    r4 = r31;
    r5 = 0xc8;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc8;
    fn_80239EE8();
L_80246ACC: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246B2C;
    r3 = r27;
    r4 = r31;
    r5 = 0xc9;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc9;
    fn_80239EE8();
L_80246B2C: ;
    r3 = r31;
    r4 = r28;
    fn_80235910();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246BAC;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xca;
    fn_80239CCC();
L_80246BAC: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80246C00;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80246C00;
    r3 = r31;
    r4 = r28;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246C6C;
L_80246C00: ;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xcb;
    fn_80239CCC();
L_80246C6C: ;
    r24 = r27;
    goto L_80247030;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_80246D4C;
    r3 = r27;
    r4 = r31;
    r5 = 0xe4;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe4;
    fn_80239EE8();
L_80246D4C: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246DD4;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe5;
    fn_80239CCC();
L_80246DD4: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80246E0C;
    r3 = r31;
    r4 = r28;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80246E78;
L_80246E0C: ;
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe6;
    fn_80239CCC();
L_80246E78: ;
    r24 = r27;
    goto L_80247030;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    r6 = r28;
    fn_8024AFC4();
    r24 = r3;
    goto L_80247030;
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
    fn_802399FC();
    r25 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_80246F74;
    r3 = r25;
    r4 = r31;
    r5 = 0x105;
    fn_80239984();
    r25 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x105;
    fn_80239EE8();
L_80246F74: ;
    r3 = r31;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80246FC0;
    r3 = r31;
    r4 = r28;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80246FC0;
    r3 = r31;
    r4 = r28;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024702C;
L_80246FC0: ;
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
    fn_802399FC();
    r25 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x106;
    fn_80239CCC();
L_8024702C: ;
    r24 = r25;
L_80247030: ;
    r3 = r24;
    /* lmw r24, 0x70(r1) */;
    return;
}
#pragma pop

/* =========================================================================
 * fn_8024E690 - PostBattleProcessing
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
/* TODO: Decompile fn_8024E690 (4644 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024E690(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A434[];
    extern u8 lbl_80478B38[];
    extern void fn_8000815C();
    extern void fn_800E0C54();
    extern void fn_801F1A6C();
    extern void fn_801F1C18();
    extern void fn_801F4460();
    extern void fn_801F87CC();
    extern void fn_801F8C00();
    extern void fn_801FB1C0();
    extern void fn_801FCEC4();
    extern void fn_80204DE4();
    extern void fn_80205BE8();
    extern void fn_802062FC();
    extern void fn_802068C8();
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
    extern void fn_802399FC();
    extern void fn_80239A40();
    extern void fn_80239EE8();
    extern void fn_8023A118();
    extern void fn_8023C530();
    extern void fn_8024FE80();
    u8 sp[0x860];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = 0x0;
    r5 = 0x43;
    /* stmw r14, 0x818(r1) */;
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
    r6 = r1 + 0x14;
    r14 = 0x0;
    /* subi r5, r5, 0x4 */;
    ctr_fn = (void(*)(void))r4;
L_8024E708: ;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)((u8*)r6 + 0x4) = r3;
    r6 += 8; *(u32*)r6 = r0;
    if (--ctr != 0) goto L_8024E708;
    r0 = *(u32*)((u8*)r5 + 0x4);
    r3 = r15;
    r4 = 0x0;
    r5 = 0x1;
    *(u32*)((u8*)r6 + 0x4) = r0;
    fn_80235B04();
    r3 = r1 + 0x68;
    r5 = 0x0;
    r4 = 0x0;
    goto L_8024E754;
L_8024E748: ;
    /* clrlslwi r0, r5, 16, 2 */;
    r5 = r5 + 0x1;
    *(u32*)(r3 + r0) = r4;
L_8024E754: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8024E748;
    r4 = r16;
    r3 = r1 + 0x110;
    fn_801FCEC4();
    r3 = r16;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E7A0;
    r3 = r16;
    r4 = 0xe2;
    r5 = 0x0;
    fn_80204DE4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E7A0;
    r14 = 0x1;
L_8024E7A0: ;
    r3 = r15;
    r4 = r1 + 0x98;
    fn_801F87CC();
    r20 = r3;
    r4 = r15;
    r5 = r1 + 0x34;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r4 = r15;
    r5 = r1 + 0xb0;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r0 = r20 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E7F8;
    r3 = -0x1;
    goto L_8024F8A0;
L_8024E7F8: ;
    r4 = r21;
    r3 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E8F0;
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r6 = r15;
    /* subi r3, r5, 0x139e */;
    /* subi r4, r5, 0x13fc */;
    *(u32*)(sp + 0xC) = r0;
    /* subi r5, r5, 0x13e3 */;
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
    r3 = r1 + 0x98;
    r0 = r0 * r4;
    r0 = r5 - r0;
    /* clrlslwi r0, r0, 16, 2 */;
    r17 = *(u32*)(r3 + r0);
    if ((u32)r17 == (u32)0x0) goto L_8024E8F0;
    r4 = r17;
    r3 = 0x0;
    fn_801F4460();
    r0 = r3;
    r3 = r17;
    r14 = r0;
    fn_80205BE8();
    r8 = 0x0;
    r5 = (0x1 << 16);
    r0 = 0x228;
    r7 = r3;
    r6 = r14;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13e3 */;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r17;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = (s16)r3;
    goto L_8024F8A0;
L_8024E8F0: ;
    r0 = r14 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024EA84;
    r14 = r1 + 0x98;
    r17 = r1 + 0x80;
    r18 = r1 + 0x18;
    r19 = 0x0;
    r26 = 0x0;
    goto L_8024E9D4;
L_8024E914: ;
    /* clrlslwi r3, r26, 16, 1 */;
    r4 = 0x0;
    r0 = r3 + 0x2;
    r23 = *(s16*)(r18 + r3);
    r24 = *(s16*)(r18 + r0);
    r3 = r4;
    goto L_8024E93C;
L_8024E930: ;
    /* clrlslwi r0, r4, 16, 2 */;
    r4 = r4 + 0x1;
    *(u32*)(r17 + r0) = r3;
L_8024E93C: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8024E930;
    r22 = 0x0;
    r25 = r22;
    goto L_8024E9B8;
L_8024E954: ;
    /* clrlslwi r0, r25, 16, 2 */;
    r19 = *(u32*)(r14 + r0);
    if ((u32)r19 == (u32)0x0) goto L_8024E9B4;
    r0 = (s16)r23;
    if ((u32)r19 < (u32)0x0) goto L_8024E984;
    r3 = r15;
    r4 = r19;
    fn_80238600();
    r0 = r3 & 0xFF;
    if ((s32)r23 != (s32)r0) goto L_8024E9B4;
L_8024E984: ;
    r0 = (s16)r24;
    if ((s32)r23 < (s32)r0) goto L_8024E9A4;
    r3 = r15;
    r4 = r19;
    fn_80238538();
    r0 = r3 & 0xFF;
    if ((s32)r24 != (s32)r0) goto L_8024E9B4;
L_8024E9A4: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r1 + 0x80;
    *(u32*)(r3 + r0) = r19;
    r22 = r22 + 0x1;
L_8024E9B4: ;
    r25 = r25 + 0x1;
L_8024E9B8: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8024E954;
    r0 = r22 & 0xFFFF;
    r19 = r22;
    if ((u32)r0 != (u32)0x6) goto L_8024E9E0;
    r26 = r26 + 0x2;
L_8024E9D4: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)0xe) goto L_8024E914;
L_8024E9E0: ;
    r0 = r19 & 0xFFFF;
    if ((u32)r0 == (u32)0xe) goto L_8024EA84;
    fn_800E0C54();
    r5 = r3 & 0xFFFF;
    r4 = r19 & 0xFFFF;
    r0 = (s32)r5 / (s32)r4;
    r3 = r1 + 0x80;
    r0 = r0 * r4;
    r0 = r5 - r0;
    /* clrlslwi r0, r0, 16, 2 */;
    r14 = *(u32*)(r3 + r0);
    if ((u32)r14 == (u32)0x0) goto L_8024EA84;
    r4 = r14;
    r3 = 0x0;
    fn_801F4460();
    r15 = r3;
    r3 = r14;
    fn_80205BE8();
    r8 = 0x0;
    r5 = (0x1 << 16);
    r0 = 0x228;
    r7 = r3;
    r6 = r15;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13e3 */;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r14;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = (s16)r3;
    goto L_8024F8A0;
L_8024EA84: ;
    r4 = (0xffff << 16);
    r3 = (0x1 << 16);
    r18 = r4 + 0x1;
    r22 = r1 + 0x98;
    /* subi r14, r3, 0x1 */;
    r17 = r20 & 0xFFFF;
    r19 = 0x0;
    r23 = 0x0;
    goto L_8024EB44;
L_8024EAA8: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r24 = *(u32*)(r22 + r0);
    if ((u32)r24 == (u32)0x0) goto L_8024EB40;
    r3 = r24;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = (s16)r3;
    if ((u32)r24 < (u32)0x0) goto L_8024EB40;
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
    fn_80205BE8();
    r4 = 0x0;
    r5 = 0xc9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    if ((u32)r19 >= (u32)r25) goto L_8024EB24;
    r19 = r25;
L_8024EB24: ;
    if ((s32)r18 >= (s32)r24) goto L_8024EB30;
    r18 = r24;
L_8024EB30: ;
    r0 = r14 & 0xFFFF;
    if ((u32)r0 <= (u32)r3) goto L_8024EB40;
    r14 = r3;
L_8024EB40: ;
    r23 = r23 + 0x1;
L_8024EB44: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r17) goto L_8024EAA8;
    r4 = r21;
    r3 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024EC60;
    r22 = r1 + 0x98;
    r17 = r20 & 0xFFFF;
    r23 = 0x0;
    goto L_8024EC54;
L_8024EB80: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r24 = *(u32*)(r22 + r0);
    if ((u32)r24 == (u32)0x0) goto L_8024EC50;
    r3 = r24;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = (s16)r3;
    if ((u32)r24 < (u32)0x0) goto L_8024EC50;
    r4 = r24;
    r3 = 0x0;
    fn_801F4460();
    r3 = r24;
    fn_80205BE8();
    r4 = 0x0;
    r5 = 0xc9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r14 & 0xFFFF;
    if ((u32)r0 < (u32)r3) goto L_8024EC50;
    r4 = r24;
    r3 = 0x0;
    fn_801F4460();
    r14 = r3;
    r3 = r24;
    fn_80205BE8();
    r8 = 0x0;
    r5 = (0x1 << 16);
    r0 = 0x228;
    r7 = r3;
    r6 = r14;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13e3 */;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r24;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = (s16)r3;
    goto L_8024F8A0;
L_8024EC50: ;
    r23 = r23 + 0x1;
L_8024EC54: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r17) goto L_8024EB80;
L_8024EC60: ;
    r0 = *(u8*)(sp + 0x7F0);
    r28 = 0x0;
    /* clrlslwi r3, r0, 24, 1 */;
    r0 = r3 + 0x1;
    *(u32*)(sp + 0x7F4) = r0;
    r0 = r20 & 0xFFFF;
    *(u32*)(sp + 0x808) = r0;
    goto L_8024F7E0;
L_8024EC80: ;
    /* clrlslwi r30, r28, 16, 2 */;
    r3 = r1 + 0x98;
    r27 = *(u32*)(r3 + r30);
    if ((u32)r27 == (u32)0x0) goto L_8024F7DC;
    r3 = r27;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = (s16)r3;
    if ((u32)r27 < (u32)0x0) goto L_8024F7DC;
    r4 = r27;
    r3 = 0x0;
    fn_801F4460();
    r26 = r3;
    r3 = r16;
    r4 = r27;
    r5 = 0x0;
    fn_802068C8();
    r3 = r15;
    r4 = r16;
    r5 = r1 + 0x54;
    r6 = 0x0;
    r7 = 0x1;
    fn_802367CC();
    r0 = *(u32*)(sp + 0x7FC);
    r23 = r3;
    r31 = r1 + 0x34;
    r14 = 0x0;
    r17 = r0 & 0xFFFF;
    r25 = 0x0;
    goto L_8024ED80;
L_8024ED04: ;
    /* clrlslwi r0, r25, 16, 2 */;
    r29 = *(u32*)(r31 + r0);
    if ((u32)r29 == (u32)0x0) goto L_8024ED7C;
    r22 = r23 & 0xFFFF;
    r24 = 0x0;
    goto L_8024ED64;
L_8024ED20: ;
    /* clrlslwi r0, r24, 16, 1 */;
    r3 = r1 + 0x54;
    r5 = *(u16*)(r3 + r0);
    if ((u32)r5 == (u32)0x0) goto L_8024ED60;
    if ((u32)r5 == (u32)0x165) goto L_8024ED60;
    r3 = r15;
    r4 = r16;
    r6 = r29;
    fn_8023C530();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024ED60;
    r14 = 0x1;
    goto L_8024ED70;
L_8024ED60: ;
    r24 = r24 + 0x1;
L_8024ED64: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024ED20;
L_8024ED70: ;
    r0 = r14 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024ED8C;
L_8024ED7C: ;
    r25 = r25 + 0x1;
L_8024ED80: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r17) goto L_8024ED04;
L_8024ED8C: ;
    r3 = r16;
    r4 = r1 + 0x110;
    fn_801FCEC4();
    r3 = r15;
    r4 = r27;
    r5 = r1 + 0x54;
    r6 = 0x0;
    r7 = 0x1;
    fn_802369B8();
    r3 = r27;
    fn_80205BE8();
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x227;
    r7 = r3;
    r29 = r1 + 0x68;
    *(u32*)(sp + 0xC) = r0;
    r6 = r26;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    r0 = *(u32*)(r29 + r30);
    /* subi r5, r5, 0x13e3 */;
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
    if ((u32)r0 != (u32)0x1) goto L_8024EE60;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x1a;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x1a;
    fn_80239EE8();
L_8024EE60: ;
    r0 = r17 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_8024EEB0;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x1b;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x1b;
    fn_80239EE8();
L_8024EEB0: ;
    r0 = r17 & 0xFF;
    if ((u32)r0 != (u32)0x3) goto L_8024EF00;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x1c;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x1c;
    fn_80239EE8();
L_8024EF00: ;
    r17 = 0x0;
    goto L_8024EFA8;
L_8024EF08: ;
    r4 = r21;
    r6 = r17;
    r3 = 0x0;
    r5 = 0x39;
    fn_801FB1C0();
    r5 = r3 & 0xFFFF;
    if ((u32)r5 == (u32)0x9) goto L_8024EFA4;
    r3 = r15;
    r4 = r27;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024EFA4;
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
    fn_80205BE8();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r15;
    /* subi r3, r6, 0x139d */;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x1d;
    fn_80239A40();
L_8024EFA4: ;
    r17 = r17 + 0x1;
L_8024EFA8: ;
    r0 = r17 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8024EF08;
    r3 = r15;
    r4 = r27;
    fn_8023881C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F010;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x1e;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x1e;
    fn_80239EE8();
L_8024F010: ;
    r0 = r14 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F05C;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x1f;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x1f;
    fn_80239EE8();
L_8024F05C: ;
    r3 = r15;
    r4 = r27;
    fn_802386C8();
    if ((u32)r19 > (u32)r3) goto L_8024F0B4;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x21;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x21;
    fn_80239EE8();
L_8024F0B4: ;
    r3 = r15;
    r4 = r27;
    fn_802389D4();
    if ((s32)r18 > (s32)r3) goto L_8024F10C;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x20;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x20;
    fn_80239EE8();
L_8024F10C: ;
    r0 = *(u32*)(sp + 0x7FC);
    r24 = 0x0;
    r14 = r0 & 0xFFFF;
    goto L_8024F210;
L_8024F11C: ;
    /* clrlslwi r0, r24, 16, 2 */;
    r3 = r1 + 0x34;
    r23 = *(u32*)(r3 + r0);
    if ((u32)r23 == (u32)0x0) goto L_8024F20C;
    r0 = *(u32*)(sp + 0x804);
    r25 = 0x0;
    r31 = r0 & 0xFFFF;
    goto L_8024F200;
L_8024F140: ;
    /* clrlslwi r0, r25, 16, 1 */;
    r3 = r1 + 0x54;
    r22 = *(u16*)(r3 + r0);
    if ((u32)r22 == (u32)0x0) goto L_8024F1FC;
    r3 = r15;
    r4 = r22;
    r5 = r16;
    fn_802395C8();
    r0 = r3 & 0xFFFF;
    r17 = r3;
    if ((u32)r0 == (u32)0x9) goto L_8024F1FC;
    r3 = r15;
    r4 = r22;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x9) goto L_8024F1FC;
    r3 = r15;
    r4 = r22;
    fn_80239500();
    r6 = r3;
    r3 = r15;
    r4 = r23;
    r5 = r17;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x41) goto L_8024F1FC;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x22;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x22;
    fn_80239EE8();
L_8024F1FC: ;
    r25 = r25 + 0x1;
L_8024F200: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8024F140;
L_8024F20C: ;
    r24 = r24 + 0x1;
L_8024F210: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r14) goto L_8024F11C;
    r0 = *(u32*)(sp + 0x7FC);
    r22 = 0x0;
    r31 = r0 & 0xFFFF;
    goto L_8024F334;
L_8024F22C: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r1 + 0x34;
    r23 = *(u32*)(r3 + r0);
    if ((u32)r23 == (u32)0x0) goto L_8024F330;
    r3 = r15;
    r4 = r23;
    r5 = r1 + 0x54;
    r6 = 0x0;
    r7 = 0x0;
    fn_802367CC();
    r14 = r3 & 0xFFFF;
    r17 = 0x0;
    goto L_8024F324;
L_8024F264: ;
    /* clrlslwi r0, r17, 16, 1 */;
    r3 = r1 + 0x54;
    r24 = *(u16*)(r3 + r0);
    if ((u32)r24 == (u32)0x0) goto L_8024F320;
    r3 = r15;
    r4 = r24;
    r5 = r23;
    fn_802395C8();
    r0 = r3 & 0xFFFF;
    r25 = r3;
    if ((u32)r0 == (u32)0x9) goto L_8024F320;
    r3 = r15;
    r4 = r24;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x9) goto L_8024F320;
    r3 = r15;
    r4 = r24;
    fn_80239500();
    r6 = r3;
    r3 = r15;
    r4 = r27;
    r5 = r25;
    fn_80238B0C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x41) goto L_8024F320;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x23;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x23;
    fn_80239EE8();
L_8024F320: ;
    r17 = r17 + 0x1;
L_8024F324: ;
    r0 = r17 & 0xFFFF;
    if ((u32)r0 < (u32)r14) goto L_8024F264;
L_8024F330: ;
    r22 = r22 + 0x1;
L_8024F334: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8024F22C;
    r4 = r21;
    r3 = 0x0;
    r5 = 0x20;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F478;
    r17 = 0x0;
    goto L_8024F468;
L_8024F368: ;
    r0 = r17 & 0xFFFF;
    if ((u32)r0 == (u32)0x9) goto L_8024F464;
    r3 = r15;
    r4 = r27;
    r5 = r17;
    r6 = 0x1;
    fn_80238B0C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x41) goto L_8024F464;
    r0 = *(u32*)(sp + 0x800);
    r14 = r1 + 0xb0;
    r22 = 0x0;
    r23 = r0 & 0xFFFF;
    goto L_8024F458;
L_8024F3A8: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r24 = *(u32*)(r14 + r0);
    if ((u32)r24 == (u32)0x0) goto L_8024F454;
    r4 = r24;
    r3 = 0x0;
    fn_801F4460();
    if ((u32)r3 == (u32)0x0) goto L_8024F454;
    r4 = r24;
    fn_801F8C00();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024F454;
    if ((u32)r0 == (u32)0x2) goto L_8024F3F0;
    if ((u32)r0 != (u32)0x3) goto L_8024F454;
L_8024F3F0: ;
    r3 = r15;
    r4 = r24;
    r5 = r17;
    r6 = 0x1;
    fn_80238B0C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x41) goto L_8024F454;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x24;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x24;
    fn_80239EE8();
L_8024F454: ;
    r22 = r22 + 0x1;
L_8024F458: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_8024F3A8;
L_8024F464: ;
    r17 = r17 + 0x1;
L_8024F468: ;
    r0 = *(u32*)lbl_80478B38;
    r3 = r17 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8024F368;
L_8024F478: ;
    r3 = r15;
    r4 = r27;
    fn_8024FE80();
    r0 = r3 & 0xFFFF;
    r14 = r3;
    if ((u32)r3 == (u32)r0) goto L_8024F4D4;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = r14;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139d */;
    r4 = r26;
    r10 = r14;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    fn_80239EE8();
L_8024F4D4: ;
    r0 = *(u32*)(sp + 0x7F8);
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_8024F564;
    r3 = r15;
    r4 = r27;
    r5 = 0x21;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024F51C;
    r3 = r15;
    r4 = r27;
    r5 = 0x2c;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F710;
L_8024F51C: ;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x29;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x29;
    fn_80239EE8();
    goto L_8024F710;
L_8024F564: ;
    if ((u32)r0 != (u32)0x1) goto L_8024F5D0;
    r3 = r15;
    r4 = r27;
    r5 = 0x22;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F710;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x2a;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x2a;
    fn_80239EE8();
    goto L_8024F710;
L_8024F5D0: ;
    if ((u32)r0 != (u32)0x3) goto L_8024F6A8;
    r3 = r15;
    r4 = r27;
    r14 = 0x0;
    r5 = 0x8;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F5FC;
    r14 = 0x1;
L_8024F5FC: ;
    r3 = r15;
    r4 = r27;
    r5 = 0x8;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024F650;
    r3 = r15;
    r4 = r27;
    r5 = 0x5;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024F650;
    r3 = r15;
    r4 = r27;
    r5 = 0x4;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F654;
L_8024F650: ;
    r14 = 0x1;
L_8024F654: ;
    r0 = r14 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F710;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x2b;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x2b;
    fn_80239EE8();
    goto L_8024F710;
L_8024F6A8: ;
    if ((u32)r0 != (u32)0x4) goto L_8024F710;
    r3 = r15;
    r4 = r27;
    r5 = 0xf;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F710;
    r3 = *(u32*)(r29 + r30);
    r4 = r15;
    r5 = 0x2c;
    fn_80239984();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x2c;
    fn_80239EE8();
L_8024F710: ;
    fn_8000815C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F794;
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
    fn_802399FC();
    *(u32*)(r29 + r30) = r3;
    r3 = r27;
    fn_80205BE8();
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x225;
    r7 = r3;
    r6 = r26;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13e3 */;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
L_8024F794: ;
    r3 = r27;
    fn_80205BE8();
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x226;
    r7 = r3;
    r6 = r26;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13e3 */;
    r0 = *(u32*)(r29 + r30);
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_8023A118();
L_8024F7DC: ;
    r28 = r28 + 0x1;
L_8024F7E0: ;
    r0 = *(u32*)(sp + 0x808);
    r3 = r28 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8024EC80;
    r4 = r20;
    r3 = r1 + 0x68;
    r5 = 0x1;
    fn_802397B8();
    if ((s32)r3 >= (s32)0x0) goto L_8024F810;
    r3 = -0x1;
    goto L_8024F8A0;
L_8024F810: ;
    r14 = r3 << 2;
    r3 = r1 + 0x98;
    r15 = *(u32*)(r3 + r14);
    if ((u32)r15 != (u32)0x0) goto L_8024F82C;
    r3 = -0x1;
    goto L_8024F8A0;
L_8024F82C: ;
    r4 = r15;
    r3 = 0x0;
    fn_801F4460();
    r16 = r3;
    r3 = r15;
    fn_80205BE8();
    r0 = 0x0;
    r4 = r1 + 0x68;
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x228;
    r5 = (0x1 << 16);
    r7 = r3;
    *(u32*)(sp + 0xC) = r0;
    r6 = r16;
    /* subi r3, r5, 0x139d */;
    r8 = 0x0;
    r0 = *(u32*)(r4 + r14);
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13e3 */;
    r9 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r15;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = (s16)r3;
L_8024F8A0: ;
    /* lmw r14, 0x818(r1) */;
    return;
}
#pragma pop

/* =========================================================================
 * fn_8025A254 - ShadowPokemonCheck
 *
 * Small helper (0x3C bytes) that checks if a Pokemon at a given
 * party index is a Shadow Pokemon. Demonstrates the data access pattern:
 *
 *   PokemonSlotLookupDefault(0x02) -> Pokemon pointer
 *   fn_801F6E44(pointer, 0x4C) -> shadow status
 *   return (status != 2)
 *
 * This is one of the few functions small enough to fully reconstruct:
 * ========================================================================= */
BOOL ShadowPokemonCheck(void) {
    void* pokemon;
    u8 status;

    pokemon = (void*)PokemonSlotLookupDefault(0x02, 0);
    /* fn_801F6E44 reads a field at offset 0x4C, returning shadow status */
    /* status = fn_801F6E44(pokemon, 0x4C); */
    /* return (status != 2); */
    return FALSE; /* Placeholder */
}

/* =========================================================================
 * fn_8025A220 - CheckBattleCondition
 *
 * Small helper (0x34 bytes) that checks a specific battle condition
 * via CheckTrainerPokemonFlag with flagId 0x0F.
 *
 * return (!CheckTrainerPokemonFlag(r3, r4, 0x0F));
 * ========================================================================= */
BOOL CheckBattleCondition(void* context, u32 slot) {
    BOOL flagSet = CheckTrainerPokemonFlag(context, slot, 0x0F);
    return !flagSet;
}

/* =========================================================================
 * fn_8025A290 - ProcessBattleResult
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
/* TODO: Decompile fn_8025A290 (0xB0 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A290(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A314;
    r31 = 0x0;
L_8025A314: ;
    if ((s32)r31 != (s32)0x0) goto L_8025A324;
    r3 = 0x0;
    goto L_8025A32C;
L_8025A324: ;
    r3 = 0x1;
L_8025A32C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* =========================================================================
 * fn_8026316C - FinalCleanup
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
/* TODO: Decompile fn_8026316C (2652 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8026316C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_800119A8();
    extern void fn_80011A1C();
    extern void fn_80011D9C();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801EF634();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F1700();
    extern void fn_801F18DC();
    extern void fn_801F1918();
    extern void fn_801F76B8();
    extern void fn_801F7E60();
    extern void fn_801F9130();
    extern void fn_801F9790();
    extern void fn_801F981C();
    extern void fn_80205C24();
    extern void fn_80207760();
    extern void fn_8020E1A4();
    extern void fn_8020E204();
    extern void fn_80263BC8();
    extern void fn_80265924();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r24, 0x30(r1) */;
    r31 = r4;
    r29 = r3;
    r25 = r5;
    r3 = r31;
    fn_8020E204();
    fn_8020E1A4();
    r27 = r3 & 0xFF;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8026328C;
    r4 = r29;
    r5 = r31;
    r24 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802631CC;
    r3 = 0x0;
    goto L_80263228;
L_802631CC: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r26 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802631FC;
    r3 = 0x0;
    goto L_80263228;
L_802631FC: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263214;
    r3 = 0x0;
    goto L_80263228;
L_80263214: ;
    r4 = r26;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80263228: ;
    if ((s32)r3 == (s32)0xf3) goto L_80263260;
    if ((s32)r3 >= (s32)0xf3) goto L_80263244;
    if ((s32)r3 == (s32)0xf1) goto L_80263250;
    if ((s32)r3 >= (s32)0xf1) goto L_80263258;
    goto L_8026326C;
L_80263244: ;
    if ((s32)r3 >= (s32)0xf5) goto L_8026326C;
    goto L_80263268;
L_80263250: ;
    r24 = 0x100;
    goto L_8026326C;
L_80263258: ;
    r24 = 0x101;
    goto L_8026326C;
L_80263260: ;
    r24 = 0x102;
    goto L_8026326C;
L_80263268: ;
    r24 = 0x103;
L_8026326C: ;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
L_8026328C: ;
    r30 = 0x0;
    r26 = 0x0;
    goto L_802639AC;
L_8026329C: ;
    r3 = r29;
    r4 = r30;
    fn_801F981C();
    /* mr. r28, r3 */;
    if ((s32)r3 != (s32)0xf5) goto L_802632B8;
    r26 = r30;
    goto L_802639A8;
L_802632B8: ;
    r4 = 0x1;
    fn_80205C24();
    r0 = r3 & 0xFF;
    if ((s32)r3 != (s32)0xf5) goto L_802632D0;
    r26 = r30;
    goto L_802639A8;
L_802632D0: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802632EC;
L_802632E0: ;
    r3 = r29;
    fn_801F9790();
    goto L_802639B8;
L_802632EC: ;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263330;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263330;
L_80263310: ;
    r3 = r28;
    fn_80207760();
    r3 = r29;
    r4 = r28;
    r5 = r31;
    fn_801F9130();
    r26 = r30;
    goto L_802639A8;
L_80263330: ;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802633C8;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80263364;
    r3 = 0x0;
    goto L_802633C0;
L_80263364: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263394;
    r3 = 0x0;
    goto L_802633C0;
L_80263394: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802633AC;
    r3 = 0x0;
    goto L_802633C0;
L_802633AC: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_802633C0: ;
    r4 = 0x1;
    fn_80011D9C();
L_802633C8: ;
    r0 = 0x0;
    *(u32*)(sp + 0x8) = r0;
L_802633D0: ;
    r3 = r28;
    fn_80207760();
    r4 = r28;
    r5 = r1 + 0xc;
    r3 = 0x0;
    fn_801F1918();
    r3 = 0x0;
    fn_801F18DC();
    *(u8*)(sp + 0x23) = r3;
    r3 = r1 + 0xc;
    r4 = r1 + 0x8;
    r5 = 0x1;
    fn_80011A1C();
    r24 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802634BC;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802634B0;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_8026344C;
    r3 = 0x0;
    goto L_802634A8;
L_8026344C: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_8026347C;
    r3 = 0x0;
    goto L_802634A8;
L_8026347C: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263494;
    r3 = 0x0;
    goto L_802634A8;
L_80263494: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_802634A8: ;
    r4 = 0x0;
    fn_80011D9C();
L_802634B0: ;
    r3 = 0x1;
    fn_800119A8();
    goto L_802632E0;
L_802634BC: ;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8026358C;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8026358C;
    if ((s32)r24 >= (s32)0x0) goto L_8026358C;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80263580;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_8026351C;
    r3 = 0x0;
    goto L_80263578;
L_8026351C: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_8026354C;
    r3 = 0x0;
    goto L_80263578;
L_8026354C: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263564;
    r3 = 0x0;
    goto L_80263578;
L_80263564: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_80263578: ;
    r4 = 0x0;
    fn_80011D9C();
L_80263580: ;
    r3 = 0x1;
    fn_800119A8();
    goto L_80263310;
L_8026358C: ;
    if ((s32)r24 < (s32)0x0) goto L_8026359C;
    r3 = 0x1;
    fn_800119A8();
L_8026359C: ;
    r3 = r29;
    r4 = r28;
    r5 = r31;
    r6 = r24;
    fn_80263BC8();
    r24 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80263668;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8026365C;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802635F8;
    r3 = 0x0;
    goto L_80263654;
L_802635F8: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263628;
    r3 = 0x0;
    goto L_80263654;
L_80263628: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263640;
    r3 = 0x0;
    goto L_80263654;
L_80263640: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_80263654: ;
    r4 = 0x0;
    fn_80011D9C();
L_8026365C: ;
    r3 = 0x1;
    fn_800119A8();
    goto L_802632E0;
L_80263668: ;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263738;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263738;
    r0 = r24 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80263738;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8026372C;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802636C8;
    r3 = 0x0;
    goto L_80263724;
L_802636C8: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802636F8;
    r3 = 0x0;
    goto L_80263724;
L_802636F8: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263710;
    r3 = 0x0;
    goto L_80263724;
L_80263710: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_80263724: ;
    r4 = 0x0;
    fn_80011D9C();
L_8026372C: ;
    r3 = 0x1;
    fn_800119A8();
    goto L_80263310;
L_80263738: ;
    r0 = r24 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263904;
    r3 = r29;
    fn_801F7E60();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802633D0;
    r3 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80263808;
    r30 = r26;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802637FC;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80263798;
    r3 = 0x0;
    goto L_802637F4;
L_80263798: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802637C8;
    r3 = 0x0;
    goto L_802637F4;
L_802637C8: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802637E0;
    r3 = 0x0;
    goto L_802637F4;
L_802637E0: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_802637F4: ;
    r4 = 0x0;
    fn_80011D9C();
L_802637FC: ;
    r3 = 0x1;
    fn_800119A8();
    goto L_8026329C;
L_80263808: ;
    r0 = r25 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802633D0;
    if ((u32)r3 != (u32)0x0) goto L_802633D0;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802638B4;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80263850;
    r3 = 0x0;
    goto L_802638AC;
L_80263850: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263880;
    r3 = 0x0;
    goto L_802638AC;
L_80263880: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263898;
    r3 = 0x0;
    goto L_802638AC;
L_80263898: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_802638AC: ;
    r4 = 0x0;
    fn_80011D9C();
L_802638B4: ;
    r3 = 0x1;
    fn_800119A8();
    r24 = 0x0;
    goto L_802638F0;
L_802638C4: ;
    r3 = r29;
    r4 = r24;
    fn_801F981C();
    if ((u32)r3 == (u32)0x0) goto L_802638EC;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_802638EC: ;
    r24 = r24 + 0x1;
L_802638F0: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_802638C4;
    r3 = 0x0;
    goto L_80263BB4;
L_80263904: ;
    if ((u32)r0 == (u32)0x2) goto L_802633D0;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802639A4;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80263940;
    r3 = 0x0;
    goto L_8026399C;
L_80263940: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263970;
    r3 = 0x0;
    goto L_8026399C;
L_80263970: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263988;
    r3 = 0x0;
    goto L_8026399C;
L_80263988: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_8026399C: ;
    r4 = 0x0;
    fn_80011D9C();
L_802639A4: ;
    r26 = r30;
L_802639A8: ;
    r30 = r30 + 0x1;
L_802639AC: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_8026329C;
L_802639B8: ;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r27) goto L_80263B70;
    r4 = r29;
    r5 = r31;
    r24 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802639EC;
    r3 = 0x0;
    goto L_80263A48;
L_802639EC: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r25 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263A1C;
    r3 = 0x0;
    goto L_80263A48;
L_80263A1C: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263A34;
    r3 = 0x0;
    goto L_80263A48;
L_80263A34: ;
    r4 = r25;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80263A48: ;
    if ((s32)r3 == (s32)0xf3) goto L_80263A80;
    if ((s32)r3 >= (s32)0xf3) goto L_80263A64;
    if ((s32)r3 == (s32)0xf1) goto L_80263A70;
    if ((s32)r3 >= (s32)0xf1) goto L_80263A78;
    goto L_80263A8C;
L_80263A64: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80263A8C;
    goto L_80263A88;
L_80263A70: ;
    r24 = 0x100;
    goto L_80263A8C;
L_80263A78: ;
    r24 = 0x101;
    goto L_80263A8C;
L_80263A80: ;
    r24 = 0x102;
    goto L_80263A8C;
L_80263A88: ;
    r24 = 0x103;
L_80263A8C: ;
    r3 = r24;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0xf5) goto L_80263B70;
    r4 = r29;
    r5 = r31;
    r24 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80263AC0;
    r3 = 0x0;
    goto L_80263B1C;
L_80263AC0: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r25 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263AF0;
    r3 = 0x0;
    goto L_80263B1C;
L_80263AF0: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263B08;
    r3 = 0x0;
    goto L_80263B1C;
L_80263B08: ;
    r4 = r25;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80263B1C: ;
    if ((s32)r3 == (s32)0xf3) goto L_80263B54;
    if ((s32)r3 >= (s32)0xf3) goto L_80263B38;
    if ((s32)r3 == (s32)0xf1) goto L_80263B44;
    if ((s32)r3 >= (s32)0xf1) goto L_80263B4C;
    goto L_80263B60;
L_80263B38: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80263B60;
    goto L_80263B5C;
L_80263B44: ;
    r24 = 0x100;
    goto L_80263B60;
L_80263B4C: ;
    r24 = 0x101;
    goto L_80263B60;
L_80263B54: ;
    r24 = 0x102;
    goto L_80263B60;
L_80263B5C: ;
    r24 = 0x103;
L_80263B60: ;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80263B70: ;
    r24 = 0x0;
    goto L_80263BA4;
L_80263B78: ;
    r3 = r29;
    r4 = r24;
    fn_801F981C();
    if ((u32)r3 == (u32)0x0) goto L_80263BA0;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_80263BA0: ;
    r24 = r24 + 0x1;
L_80263BA4: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_80263B78;
    r3 = 0x1;
L_80263BB4: ;
    /* lmw r24, 0x30(r1) */;
    return;
}
#pragma pop

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 120 functions matched
 * =================================================================== */

/* Address: 0x8024E52C | Size: 0x8 | Pattern: return_constant */
u32 fn_8024E52C(void) { return 0; }

/* Address: 0x80250980 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250980(void) { return 0; }

/* Address: 0x80250988 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250988(void) { return 0; }

/* Address: 0x80250990 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250990(void) { return 0; }

/* Address: 0x80250998 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250998(void) { return 0; }

/* Address: 0x80250A24 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250A24(void) { return 0; }

/* Address: 0x80250AB0 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250AB0(void) { return 0; }

/* Address: 0x80250AB8 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250AB8(void) { return 0; }

/* Address: 0x80250CE8 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250CE8(void) { return 0; }

/* Address: 0x80250D74 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250D74(void) { return 0; }

/* Address: 0x80250F54 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250F54(void) { return 0; }

/* Address: 0x80250F5C | Size: 0x8 | Pattern: return_constant */
u32 fn_80250F5C(void) { return 0; }

/* Address: 0x80250F64 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250F64(void) { return 0; }

/* Address: 0x80250F6C | Size: 0x8 | Pattern: return_constant */
u32 fn_80250F6C(void) { return 0; }

/* Address: 0x80250F74 | Size: 0x8 | Pattern: return_constant */
u32 fn_80250F74(void) { return 0; }

/* Address: 0x80251150 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251150(void) { return 0; }

/* Address: 0x802511D0 | Size: 0x8 | Pattern: return_constant */
u32 fn_802511D0(void) { return 0; }

/* Address: 0x802511D8 | Size: 0x8 | Pattern: return_constant */
u32 fn_802511D8(void) { return 0; }

/* Address: 0x80251264 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251264(void) { return 0; }

/* Address: 0x8025126C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025126C(void) { return 0; }

/* Address: 0x80251274 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251274(void) { return 0; }

/* Address: 0x8025127C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025127C(void) { return 0; }

/* Address: 0x80251284 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251284(void) { return 0; }

/* Address: 0x8025128C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025128C(void) { return 0; }

/* Address: 0x80251294 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251294(void) { return 0; }

/* Address: 0x8025129C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025129C(void) { return 0; }

/* Address: 0x80251350 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251350(void) { return 0; }

/* Address: 0x802514C4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802514C4(void) { return 0; }

/* Address: 0x802514CC | Size: 0x8 | Pattern: return_constant */
u32 fn_802514CC(void) { return 0; }

/* Address: 0x802514D4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802514D4(void) { return 0; }

/* Address: 0x802514DC | Size: 0x8 | Pattern: return_constant */
u32 fn_802514DC(void) { return 0; }

/* Address: 0x802514E4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802514E4(void) { return 0; }

/* Address: 0x8025160C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025160C(void) { return 0; }

/* Address: 0x80251650 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251650(void) { return 0; }

/* Address: 0x80251680 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251680(void) { return 0; }

/* Address: 0x80251798 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251798(void) { return 0; }

/* Address: 0x80251B38 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251B38(void) { return 0; }

/* Address: 0x80251B40 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251B40(void) { return 0; }

/* Address: 0x80251B48 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251B48(void) { return 0; }

/* Address: 0x80251CDC | Size: 0x8 | Pattern: return_constant */
u32 fn_80251CDC(void) { return 0; }

/* Address: 0x80251CE4 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251CE4(void) { return 0; }

/* Address: 0x80251F64 | Size: 0x8 | Pattern: return_constant */
u32 fn_80251F64(void) { return 20; }

/* Address: 0x80252030 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252030(void) { return 0; }

/* Address: 0x80252140 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252140(void) { return 0; }

/* Address: 0x8025234C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025234C(void) { return 0; }

/* Address: 0x80252390 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252390(void) { return 0; }

/* Address: 0x80252468 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252468(void) { return 0; }

/* Address: 0x80252470 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252470(void) { return 0; }

/* Address: 0x80252478 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252478(void) { return 0; }

/* Address: 0x80252480 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252480(void) { return 0; }

/* Address: 0x80252488 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252488(void) { return 0; }

/* Address: 0x80252490 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252490(void) { return 0; }

/* Address: 0x80252498 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252498(void) { return 0; }

/* Address: 0x802524A0 | Size: 0x8 | Pattern: return_constant */
u32 fn_802524A0(void) { return 0; }

/* Address: 0x802524A8 | Size: 0x8 | Pattern: return_constant */
u32 fn_802524A8(void) { return 0; }

/* Address: 0x802524B0 | Size: 0x8 | Pattern: return_constant */
u32 fn_802524B0(void) { return 0; }

/* Address: 0x80252740 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252740(void) { return 0; }

/* Address: 0x802527BC | Size: 0x8 | Pattern: return_constant */
u32 fn_802527BC(void) { return 5; }

/* Address: 0x802528BC | Size: 0x8 | Pattern: return_constant */
u32 fn_802528BC(void) { return 0; }

/* Address: 0x802528C4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802528C4(void) { return 0; }

/* Address: 0x802528CC | Size: 0x8 | Pattern: return_constant */
u32 fn_802528CC(void) { return 0; }

/* Address: 0x802528D4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802528D4(void) { return 0; }

/* Address: 0x8025296C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025296C(void) { return 0; }

/* Address: 0x80252974 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252974(void) { return 0; }

/* Address: 0x802529C4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802529C4(void) { return 0; }

/* Address: 0x802529CC | Size: 0x8 | Pattern: return_constant */
u32 fn_802529CC(void) { return 0; }

/* Address: 0x802529D4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802529D4(void) { return 0; }

/* Address: 0x802529DC | Size: 0x8 | Pattern: return_constant */
u32 fn_802529DC(void) { return 0; }

/* Address: 0x802529E4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802529E4(void) { return 0; }

/* Address: 0x802529EC | Size: 0x8 | Pattern: return_constant */
u32 fn_802529EC(void) { return 0; }

/* Address: 0x80252A78 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252A78(void) { return 0; }

/* Address: 0x80252F1C | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F1C(void) { return 0; }

/* Address: 0x80252F24 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F24(void) { return 0; }

/* Address: 0x80252F2C | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F2C(void) { return 0; }

/* Address: 0x80252F34 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F34(void) { return 0; }

/* Address: 0x80252F3C | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F3C(void) { return 0; }

/* Address: 0x80252F44 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F44(void) { return 0; }

/* Address: 0x80252F4C | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F4C(void) { return 0; }

/* Address: 0x80252F54 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F54(void) { return 0; }

/* Address: 0x80252F5C | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F5C(void) { return 0; }

/* Address: 0x80252F64 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F64(void) { return 0; }

/* Address: 0x80252F6C | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F6C(void) { return 0; }

/* Address: 0x80252F74 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F74(void) { return 0; }

/* Address: 0x80252F7C | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F7C(void) { return 0; }

/* Address: 0x80252F84 | Size: 0x8 | Pattern: return_constant */
u32 fn_80252F84(void) { return 0; }

/* Address: 0x80253010 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253010(void) { return 0; }

/* Address: 0x80253018 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253018(void) { return 0; }

/* Address: 0x802531F0 | Size: 0x8 | Pattern: return_constant */
u32 fn_802531F0(void) { return 40; }

/* Address: 0x80253344 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253344(void) { return 0; }

/* Address: 0x802533D0 | Size: 0x8 | Pattern: return_constant */
u32 fn_802533D0(void) { return 0; }

/* Address: 0x80253484 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253484(void) { return 0; }

/* Address: 0x802534CC | Size: 0x8 | Pattern: return_constant */
u32 fn_802534CC(void) { return 0; }

/* Address: 0x80253510 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253510(void) { return 0; }

/* Address: 0x80253518 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253518(void) { return 0; }

/* Address: 0x80253520 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253520(void) { return 0; }

/* Address: 0x80253528 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253528(void) { return 0; }

/* Address: 0x80253530 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253530(void) { return 0; }

/* Address: 0x80253538 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253538(void) { return 0; }

/* Address: 0x80253540 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253540(void) { return 0; }

/* Address: 0x802535CC | Size: 0x8 | Pattern: return_constant */
u32 fn_802535CC(void) { return 0; }

/* Address: 0x802535D4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802535D4(void) { return 0; }

/* Address: 0x802535DC | Size: 0x8 | Pattern: return_constant */
u32 fn_802535DC(void) { return 0; }

/* Address: 0x802535E4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802535E4(void) { return 0; }

/* Address: 0x802535EC | Size: 0x8 | Pattern: return_constant */
u32 fn_802535EC(void) { return 0; }

/* Address: 0x802538B8 | Size: 0x8 | Pattern: return_constant */
u32 fn_802538B8(void) { return 0; }

/* Address: 0x80253948 | Size: 0x8 | Pattern: return_constant */
u32 fn_80253948(void) { return 0; }

/* Address: 0x80254678 | Size: 0x8 | Pattern: return_constant */
u32 fn_80254678(void) { return 1; }

/* Address: 0x80254E2C | Size: 0x8 | Pattern: return_constant */
u32 fn_80254E2C(void) { return 1; }

/* Address: 0x80255218 | Size: 0x8 | Pattern: return_constant */
u32 fn_80255218(void) { return 1; }

/* Address: 0x802552C8 | Size: 0x8 | Pattern: return_constant */
u32 fn_802552C8(void) { return 1; }

/* Address: 0x80255EE4 | Size: 0x8 | Pattern: return_constant */
u32 fn_80255EE4(void) { return 0; }

/* Address: 0x802564C0 | Size: 0x8 | Pattern: return_constant */
u32 fn_802564C0(void) { return 1; }

/* Address: 0x8025746C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025746C(void) { return 1; }

/* Address: 0x802575C0 | Size: 0x8 | Pattern: return_constant */
u32 fn_802575C0(void) { return 1; }

/* Address: 0x80258134 | Size: 0x8 | Pattern: return_constant */
u32 fn_80258134(void) { return 1; }

/* Address: 0x802586F4 | Size: 0x8 | Pattern: return_constant */
u32 fn_802586F4(void) { return 1; }

/* Address: 0x802587B8 | Size: 0x8 | Pattern: return_constant */
u32 fn_802587B8(void) { return 1; }

/* Address: 0x8025B11C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025B11C(void) { return 1; }

/* Address: 0x8025C25C | Size: 0x8 | Pattern: return_constant */
u32 fn_8025C25C(void) { return 1; }

/* Address: 0x80262D34 | Size: 0x8 | Pattern: return_constant */
u32 fn_80262D34(void) { return 0; }

/* ===================================================================
 * EXPANDED FUNCTION COVERAGE
 * 560 additional functions for 0x80240000-0x80266360
 * =================================================================== */


/* -------------------------------------------------------------------
 * Battle Orchestration (0x80240000-0x8024D000)
 * 92 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802400D8 | Size: 0x6C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802400D8(void* ctx, u32 slot, u32 param) {
    extern void fn_8023CA9C();
    extern void fn_8025CB3C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    fn_8025CB3C();
    r5 = r3;
    r0 = r30 & 0xFFFF;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)r0) goto L_8024012C;
    if ((u32)r3 == (u32)0x0) goto L_8024012C;
    r3 = r28;
    r4 = r29;
    r6 = r31;
    fn_8023CA9C();
    goto L_80240130;
L_8024012C: ;
    r3 = 0x0;
L_80240130: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80240144 | Size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80240144(void* ctx, u32 param1, u32 param2) {
    extern void fn_800E0C54();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    r6 = 0x0;
    /* stmw r27, 0x1c(r1) */;
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
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1cb;
    fn_80239CCC();
    r3 = r31;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* Address: 0x802401F0 | Size: 0x264 (612 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802401F0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_801F54A4();
    extern void fn_80205B8C();
    extern void fn_80237F74();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r6 = 0x1;
    r7 = 0x1;
    /* stmw r24, 0x30(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r5 = r1 + 0x8;
    r4 = r27;
    r30 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r31 = r3;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_80240294;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x1c7;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c7;
    fn_80239EE8();
L_80240294: ;
    r26 = r1 + 0x8;
    r25 = r31 & 0xFFFF;
    r24 = 0x0;
    goto L_80240320;
L_802402A4: ;
    /* clrlslwi r0, r24, 16, 2 */;
    r4 = *(u32*)(r26 + r0);
    if ((u32)r28 == (u32)r4) goto L_8024031C;
    r3 = r27;
    fn_8023831C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x8) goto L_802402D0;
    if ((u32)r0 != (u32)0x9) goto L_8024031C;
L_802402D0: ;
    r3 = r30;
    r4 = r27;
    r5 = 0x1c8;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c8;
    fn_80239EE8();
    goto L_8024032C;
L_8024031C: ;
    r24 = r24 + 0x1;
L_80240320: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_802402A4;
L_8024032C: ;
    r25 = r1 + 0x8;
    r26 = r31 & 0xFFFF;
    r24 = 0x0;
    goto L_802403B4;
L_8024033C: ;
    /* clrlslwi r0, r24, 16, 2 */;
    r4 = *(u32*)(r25 + r0);
    if ((u32)r28 == (u32)r4) goto L_802403B0;
    r3 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802403B0;
    r3 = r30;
    r4 = r27;
    r5 = 0x1c9;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c9;
    fn_80239EE8();
    goto L_802403C0;
L_802403B0: ;
    r24 = r24 + 0x1;
L_802403B4: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_8024033C;
L_802403C0: ;
    r4 = r1 + 0x8;
    r0 = r31 & 0xFFFF;
    r5 = 0x0;
    goto L_80240430;
L_802403D0: ;
    /* clrlslwi r3, r5, 16, 2 */;
    r3 = *(u32*)(r4 + r3);
    if ((u32)r28 == (u32)r3) goto L_8024042C;
    r3 = r30;
    r4 = r27;
    r5 = 0x1ca;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1ca;
    fn_80239EE8();
    goto L_8024043C;
L_8024042C: ;
    r5 = r5 + 0x1;
L_80240430: ;
    r3 = r5 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_802403D0;
L_8024043C: ;
    r3 = r30;
    /* lmw r24, 0x30(r1) */;
    return;
}
#pragma pop

/* Address: 0x80240454 | Size: 0x16C (364 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80240454(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6D9C();
    extern void fn_801F6E98();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r30 = r3;
    r27 = r4;
    r31 = r5;
    r4 = r6;
    r29 = 0x0;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4a;
    r28 = r3;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802404A8;
    r3 = r28;
    r4 = 0x4a;
    fn_801F6D9C();
    goto L_802404AC;
L_802404A8: ;
    r3 = 0x0;
L_802404AC: ;
    r0 = (s16)r3;
    if ((u32)r0 != (u32)0x1) goto L_80240500;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1c4;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c4;
    fn_80239EE8();
    goto L_802405A8;
L_80240500: ;
    r0 = (s16)r3;
    if ((s32)r0 != (s32)0x1) goto L_80240558;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1c5;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c5;
    fn_80239EE8();
    goto L_802405A8;
L_80240558: ;
    if ((s32)r0 != (s32)0x2) goto L_802405A8;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1c6;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c6;
    fn_80239EE8();
L_802405A8: ;
    r3 = r29;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802405C0 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802405C0(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80236B98();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    fn_80236B98();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80240634;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1c3;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c3;
    fn_80239EE8();
L_80240634: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024064C | Size: 0x13C (316 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024064C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8011BEB4();
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fn_80242E4C();
    extern void fn_80242FEC();
    extern void fn_802430E4();
    extern void fn_80243178();
    extern void fn_80243284();
    extern void fn_80243390();
    extern void fn_8024349C();
    extern void fn_8024E52C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80240770;
    if ((u32)r0 == (u32)0xffff) goto L_80240770;
    if ((u32)r0 == (u32)0x165) goto L_80240770;
    if ((u32)r0 == (u32)0x163) goto L_80240770;
    r4 = r3;
    r3 = 0x0;
    r5 = 0x1c;
    r6 = 0x0;
    fn_8011BEB4();
    if ((u32)r3 != (u32)0x0) goto L_802406B8;
    r3 = (u32)fn_8024E52C;
    r3 = (u32)fn_8024E52C;
L_802406B8: ;
    r4 = (u32)fn_8024349C;
    r0 = (u32)fn_8024349C;
    if ((u32)r3 == (u32)r0) goto L_80240728;
    r4 = (u32)fn_80243390;
    r0 = (u32)fn_80243390;
    if ((u32)r3 == (u32)r0) goto L_80240728;
    r4 = (u32)fn_80243284;
    r0 = (u32)fn_80243284;
    if ((u32)r3 == (u32)r0) goto L_80240728;
    r4 = (u32)fn_80243178;
    r0 = (u32)fn_80243178;
    if ((u32)r3 == (u32)r0) goto L_80240728;
    r4 = (u32)fn_802430E4;
    r0 = (u32)fn_802430E4;
    if ((u32)r3 == (u32)r0) goto L_80240728;
    r4 = (u32)fn_80242FEC;
    r0 = (u32)fn_80242FEC;
    if ((u32)r3 == (u32)r0) goto L_80240728;
    r4 = (u32)fn_80242E4C;
    r0 = (u32)fn_80242E4C;
    if ((u32)r3 != (u32)r0) goto L_80240770;
L_80240728: ;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1c2;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c2;
    fn_80239EE8();
L_80240770: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80240788 | Size: 0x448 (1096 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80240788(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
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
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0x2c(r1) */;
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
    r3 = r1 + 0x20;
    r4 = 0x0;
    goto L_80240848;
L_80240824: ;
    r0 = r4 & 0xFF;
    r0 = *(u8*)(r3 + r0);
    if ((u32)r0 < (u32)0x8) goto L_80240844;
    if ((u32)r0 > (u32)0x9) goto L_80240844;
    r0 = 0x1;
    goto L_80240858;
L_80240844: ;
    r4 = r4 + 0x1;
L_80240848: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80240824;
    r0 = 0x0;
L_80240858: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802408AC;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x1be;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1be;
    fn_80239EE8();
L_802408AC: ;
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
    r3 = r1 + 0x18;
    r4 = 0x0;
    goto L_8024094C;
L_80240928: ;
    r0 = r4 & 0xFF;
    r0 = *(u8*)(r3 + r0);
    if ((u32)r0 < (u32)0xa) goto L_80240948;
    if ((u32)r0 > (u32)0xc) goto L_80240948;
    r0 = 0x1;
    goto L_8024095C;
L_80240948: ;
    r4 = r4 + 0x1;
L_8024094C: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80240928;
    r0 = 0x0;
L_8024095C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802409B0;
    r3 = r31;
    r4 = r27;
    r5 = 0x1bf;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1bf;
    fn_80239EE8();
L_802409B0: ;
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
    r3 = r1 + 0x10;
    r4 = 0x0;
    goto L_80240A50;
L_80240A2C: ;
    r0 = r4 & 0xFF;
    r0 = *(u8*)(r3 + r0);
    if ((u32)r0 < (u32)0x3) goto L_80240A4C;
    if ((u32)r0 > (u32)0x4) goto L_80240A4C;
    r0 = 0x1;
    goto L_80240A60;
L_80240A4C: ;
    r4 = r4 + 0x1;
L_80240A50: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80240A2C;
    r0 = 0x0;
L_80240A60: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240AB4;
    r3 = r31;
    r4 = r27;
    r5 = 0x1c0;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c0;
    fn_80239EE8();
L_80240AB4: ;
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
    r3 = r1 + 0x8;
    r4 = 0x0;
    goto L_80240B54;
L_80240B30: ;
    r0 = r4 & 0xFF;
    r0 = *(u8*)(r3 + r0);
    if ((u32)r0 < (u32)0x0) goto L_80240B50;
    if ((u32)r0 > (u32)0x2) goto L_80240B50;
    r0 = 0x1;
    goto L_80240B64;
L_80240B50: ;
    r4 = r4 + 0x1;
L_80240B54: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80240B30;
    r0 = 0x0;
L_80240B64: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80240BB8;
    r3 = r31;
    r4 = r27;
    r5 = 0x1c1;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c1;
    fn_80239EE8();
L_80240BB8: ;
    r3 = r31;
    /* lmw r27, 0x2c(r1) */;
    return;
}
#pragma pop

/* Address: 0x80241660 | Size: 0x510 (1296 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80241660(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478DF8[];
    extern void fn_801F0898();
    extern void fn_801F1170();
    extern void fn_801F1990();
    extern void fn_801F1C18();
    extern void fn_80202108();
    extern void fn_80202234();
    extern void fn_80205184();
    extern void fn_80205B8C();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r6 = 0x1;
    r7 = 0x1;
    /* stmw r21, 0x44(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r5 = r1 + 0x1c;
    r4 = r28;
    r31 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r26 = r3;
    r21 = 0x0;
    r22 = 0x0;
    goto L_80241708;
L_802416A8: ;
    r0 = r22 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80241704;
    if ((u32)r0 == (u32)0x165) goto L_80241704;
    if ((u32)r0 == (u32)0x163) goto L_80241704;
    r3 = r28;
    r4 = r22;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x163) goto L_80241704;
    r4 = r28;
    r7 = r22;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241704;
    r21 = 0x1;
L_80241704: ;
    r22 = r22 + 0x1;
L_80241708: ;
    r3 = *(u32*)lbl_80478DF8;
    r4 = r22 & 0xFFFF;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r4 < (u32)r0) goto L_802416A8;
    r0 = r21 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241770;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1b0;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b0;
    fn_80239EE8();
L_80241770: ;
    r27 = r1 + 0x1c;
    r22 = r26 & 0xFFFF;
    r24 = 0x0;
    goto L_80241834;
L_80241780: ;
    /* clrlslwi r0, r24, 16, 2 */;
    r4 = *(u32*)(r27 + r0);
    if ((u32)r29 == (u32)r4) goto L_80241830;
    r3 = r28;
    r5 = r1 + 0x8;
    r6 = 0x0;
    r7 = 0x1;
    fn_802367CC();
    r25 = r3 & 0xFFFF;
    if ((u32)r29 == (u32)r4) goto L_80241830;
    r23 = r1 + 0x8;
    r21 = 0x0;
    goto L_80241824;
L_802417B8: ;
    /* clrlslwi r0, r21, 16, 1 */;
    r3 = r28;
    r4 = *(u16*)(r23 + r0);
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)r4) goto L_80241820;
    r3 = r31;
    r4 = r28;
    r5 = 0x1b1;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b1;
    fn_80239EE8();
    goto L_80241830;
L_80241820: ;
    r21 = r21 + 0x1;
L_80241824: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_802417B8;
L_80241830: ;
    r24 = r24 + 0x1;
L_80241834: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_80241780;
    r27 = r1 + 0x1c;
    r23 = r26 & 0xFFFF;
    r25 = 0x1;
    r24 = 0x0;
    goto L_802418D4;
L_80241854: ;
    /* clrlslwi r22, r24, 16, 2 */;
    r3 = *(u32*)(r27 + r22);
    if ((u32)r29 == (u32)r3) goto L_802418D0;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r21, r3 */;
    if ((u32)r29 == (u32)r3) goto L_802418D0;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)r3) goto L_802418D0;
    r3 = r21;
    fn_801F0898();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x13) goto L_802418A4;
    r25 = 0x0;
    goto L_802418E0;
L_802418A4: ;
    r3 = *(u32*)(r27 + r22);
    fn_80205184();
    r0 = r3;
    r3 = r28;
    r4 = r0;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x13) goto L_802418D0;
    r25 = 0x0;
    goto L_802418E0;
L_802418D0: ;
    r24 = r24 + 0x1;
L_802418D4: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_80241854;
L_802418E0: ;
    r0 = r25 & 0xFF;
    if ((u32)r0 != (u32)r23) goto L_80241930;
    r3 = r31;
    r4 = r28;
    r5 = 0x1b2;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b2;
    fn_80239EE8();
L_80241930: ;
    r25 = r1 + 0x1c;
    r24 = r26 & 0xFFFF;
    r22 = 0x0;
    goto L_802419B8;
L_80241940: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r4 = *(u32*)(r25 + r0);
    if ((u32)r29 == (u32)r4) goto L_802419B4;
    r3 = r28;
    r5 = 0x12;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802419B4;
    r3 = r31;
    r4 = r28;
    r5 = 0x1b3;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b3;
    fn_80239EE8();
    goto L_802419C4;
L_802419B4: ;
    r22 = r22 + 0x1;
L_802419B8: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_80241940;
L_802419C4: ;
    r25 = r1 + 0x1c;
    r24 = r26 & 0xFFFF;
    r22 = 0x0;
    goto L_80241A4C;
L_802419D4: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = *(u32*)(r25 + r0);
    if ((u32)r29 == (u32)r3) goto L_80241A48;
    r4 = 0x0;
    r5 = 0xf9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)r3) goto L_80241A48;
    r3 = r31;
    r4 = r28;
    r5 = 0x1b4;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b4;
    fn_80239EE8();
    goto L_80241A58;
L_80241A48: ;
    r22 = r22 + 0x1;
L_80241A4C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_802419D4;
L_80241A58: ;
    r27 = r1 + 0x1c;
    r24 = r26 & 0xFFFF;
    r25 = 0x0;
    goto L_80241B4C;
L_80241A68: ;
    /* clrlslwi r0, r25, 16, 2 */;
    r23 = *(u32*)(r27 + r0);
    if ((u32)r29 == (u32)r23) goto L_80241B48;
    r3 = r28;
    r4 = r23;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r29 != (u32)r23) goto L_80241A98;
    r0 = -0x1;
    goto L_80241AF4;
L_80241A98: ;
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
    if ((s32)r0 < (s32)r3) goto L_80241AF0;
    r0 = 0x1;
    goto L_80241AF4;
L_80241AF0: ;
    r0 = 0x0;
L_80241AF4: ;
    r0 = (s8)r0;
    if ((s32)r0 != (s32)r3) goto L_80241B48;
    r3 = r31;
    r4 = r28;
    r5 = 0x1b5;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1b5;
    fn_80239EE8();
    goto L_80241B58;
L_80241B48: ;
    r25 = r25 + 0x1;
L_80241B4C: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_80241A68;
L_80241B58: ;
    r3 = r31;
    /* lmw r21, 0x44(r1) */;
    return;
}
#pragma pop

/* Address: 0x80241B70 | Size: 0x278 (632 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80241B70(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6D9C();
    extern void fn_801F6E98();
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_802358AC();
    extern void fn_80235910();
    extern void fn_80235974();
    extern void fn_802359D8();
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r28 = r3;
    r26 = r6;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    r4 = r26;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4a;
    r27 = r3;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241BCC;
    r3 = r27;
    r4 = 0x4a;
    fn_801F6D9C();
    r27 = r3;
    goto L_80241BD0;
L_80241BCC: ;
    r27 = 0x0;
L_80241BD0: ;
    r3 = r28;
    r4 = r26;
    fn_80235AA0();
    *(u8*)(sp + 0x8) = r3;
    r3 = r28;
    r4 = r26;
    fn_80235A3C();
    *(u8*)(sp + 0x9) = r3;
    r3 = r28;
    r4 = r26;
    fn_802359D8();
    *(u8*)(sp + 0xA) = r3;
    r3 = r28;
    r4 = r26;
    fn_80235974();
    *(u8*)(sp + 0xB) = r3;
    r3 = r28;
    r4 = r26;
    fn_80235910();
    *(u8*)(sp + 0xC) = r3;
    r3 = r28;
    r4 = r26;
    fn_802358AC();
    *(u8*)(sp + 0xD) = r3;
    r3 = r28;
    r4 = r26;
    fn_802357CC();
    *(u8*)(sp + 0xE) = r3;
    r3 = r1 + 0x8;
    r4 = 0x0;
    goto L_80241C70;
L_80241C4C: ;
    r0 = r4 & 0xFF;
    r0 = *(u8*)(r3 + r0);
    if ((u32)r0 < (u32)0x8) goto L_80241C6C;
    if ((u32)r0 > (u32)0xc) goto L_80241C6C;
    r0 = 0x1;
    goto L_80241C80;
L_80241C6C: ;
    r4 = r4 + 0x1;
L_80241C70: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80241C4C;
    r0 = 0x0;
L_80241C80: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241CD4;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1ac;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1ac;
    fn_80239EE8();
L_80241CD4: ;
    r0 = (s16)r27;
    if ((s32)r0 != (s32)0x1) goto L_80241D2C;
    r3 = r31;
    r4 = r28;
    r5 = 0x1ad;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1ad;
    fn_80239EE8();
    goto L_80241DD0;
L_80241D2C: ;
    if ((s32)r0 != (s32)0x2) goto L_80241D80;
    r3 = r31;
    r4 = r28;
    r5 = 0x1ae;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1ae;
    fn_80239EE8();
    goto L_80241DD0;
L_80241D80: ;
    if ((s32)r0 != (s32)0x3) goto L_80241DD0;
    r3 = r31;
    r4 = r28;
    r5 = 0x1af;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1af;
    fn_80239EE8();
L_80241DD0: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80241DE8 | Size: 0x1FC (508 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80241DE8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_802364BC();
    extern void fn_80236520();
    extern void fn_802377E8();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r23, 0x2c(r1) */;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    r23 = r6;
    r29 = 0x0;
    fn_80236520();
    r30 = r3;
    r3 = r26;
    r4 = r27;
    fn_802364BC();
    r31 = r3;
    r3 = r26;
    r4 = r23;
    fn_80236520();
    r23 = r3;
    r4 = r26;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r0 = r23 & 0xFFFF;
    r25 = r3;
    if ((s32)r0 == (s32)0) goto L_80241ED4;
    if ((u32)r0 == (u32)0xffff) goto L_80241ED4;
    if ((u32)r0 == (u32)0x165) goto L_80241ED4;
    if ((u32)r0 == (u32)0x163) goto L_80241ED4;
    r3 = r26;
    r4 = r23;
    r5 = 0x4;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80241ED4;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x1a9;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a9;
    fn_80239EE8();
L_80241ED4: ;
    r24 = r1 + 0x8;
    r25 = r25 & 0xFFFF;
    r23 = 0x0;
    goto L_80241F60;
L_80241EE4: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r3 = r26;
    r4 = *(u32*)(r24 + r0);
    fn_802377E8();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x12e) goto L_80241F10;
    if ((u32)r0 == (u32)0xd4) goto L_80241F10;
    if ((u32)r0 != (u32)0x177) goto L_80241F5C;
L_80241F10: ;
    r3 = r29;
    r4 = r26;
    r5 = 0x1aa;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1aa;
    fn_80239EE8();
    goto L_80241F6C;
L_80241F5C: ;
    r23 = r23 + 0x1;
L_80241F60: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_80241EE4;
L_80241F6C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x121) goto L_80241F84;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x121) goto L_80241FCC;
L_80241F84: ;
    r3 = r29;
    r4 = r26;
    r5 = 0x1ab;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1ab;
    fn_80239EE8();
L_80241FCC: ;
    r3 = r29;
    /* lmw r23, 0x2c(r1) */;
    return;
}
#pragma pop

/* Address: 0x80241FE4 | Size: 0x2A8 (680 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80241FE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_802377E8();
    extern void fn_8023785C();
    extern void fn_8023793C();
    extern void fn_80237CB8();
    extern void fn_80239500();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r6 = 0x1;
    r7 = 0x1;
    /* stmw r14, 0x58(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r5 = r1 + 0x2c;
    r4 = r29;
    r20 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r22 = r3;
    r4 = r29;
    r5 = r1 + 0xc;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r23 = r3;
    r21 = r1 + 0x2c;
    r26 = r22 & 0xFFFF;
    r15 = r1 + 0x8;
    r14 = r1 + 0xc;
    r28 = r3 & 0xFFFF;
    r19 = 0x0;
    r18 = 0x0;
    goto L_802420EC;
L_8024205C: ;
    /* clrlslwi r0, r18, 16, 2 */;
    r3 = r29;
    r4 = *(u32*)(r14 + r0);
    r5 = r1 + 0x8;
    fn_80237CB8();
    r27 = r3 & 0xFFFF;
    r17 = 0x0;
    goto L_802420DC;
L_8024207C: ;
    /* clrlslwi r24, r17, 16, 1 */;
    r16 = 0x0;
    goto L_802420CC;
L_80242088: ;
    /* clrlslwi r25, r16, 16, 2 */;
    r0 = *(u32*)(r21 + r25);
    if ((u32)r30 == (u32)r0) goto L_802420C8;
    r3 = r29;
    r4 = r31;
    fn_80239500();
    r4 = *(u32*)(r21 + r25);
    r6 = r3;
    r5 = *(u16*)(r15 + r24);
    r3 = r29;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x41) goto L_802420C8;
    r19 = 0x1;
L_802420C8: ;
    r16 = r16 + 0x1;
L_802420CC: ;
    r0 = r16 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_80242088;
    r17 = r17 + 0x1;
L_802420DC: ;
    r0 = r17 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_8024207C;
    r18 = r18 + 0x1;
L_802420EC: ;
    r0 = r18 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8024205C;
    r0 = r19 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024214C;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x1a6;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r20 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a6;
    fn_80239EE8();
L_8024214C: ;
    r15 = r1 + 0x2c;
    r14 = r22 & 0xFFFF;
    r16 = 0x0;
    goto L_802421D0;
L_8024215C: ;
    /* clrlslwi r0, r16, 16, 2 */;
    r4 = *(u32*)(r15 + r0);
    if ((u32)r30 == (u32)r4) goto L_802421CC;
    r3 = r29;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_802421CC;
    r3 = r20;
    r4 = r29;
    r5 = 0x1a7;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r20 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a7;
    fn_80239EE8();
    goto L_802421DC;
L_802421CC: ;
    r16 = r16 + 0x1;
L_802421D0: ;
    r0 = r16 & 0xFFFF;
    if ((u32)r0 < (u32)r14) goto L_8024215C;
L_802421DC: ;
    r15 = r1 + 0xc;
    r14 = r23 & 0xFFFF;
    r16 = 0x0;
    goto L_80242268;
L_802421EC: ;
    /* clrlslwi r0, r16, 16, 2 */;
    r3 = r29;
    r4 = *(u32*)(r15 + r0);
    fn_802377E8();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x12e) goto L_80242218;
    if ((u32)r0 == (u32)0xd4) goto L_80242218;
    if ((u32)r0 != (u32)0x177) goto L_80242264;
L_80242218: ;
    r3 = r20;
    r4 = r29;
    r5 = 0x1a8;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r20 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a8;
    fn_80239EE8();
    goto L_80242274;
L_80242264: ;
    r16 = r16 + 0x1;
L_80242268: ;
    r0 = r16 & 0xFFFF;
    if ((u32)r0 < (u32)r14) goto L_802421EC;
L_80242274: ;
    r3 = r20;
    /* lmw r14, 0x58(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024228C | Size: 0x3BC (956 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024228C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r29 = r5;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r31 = 0x0;
    r4 = r30;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242310;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x19d;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x19d;
    fn_80239EE8();
L_80242310: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242374;
    r3 = r31;
    r4 = r27;
    r5 = 0x19e;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x19e;
    fn_80239EE8();
L_80242374: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802423D8;
    r3 = r31;
    r4 = r27;
    r5 = 0x19f;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x19f;
    fn_80239EE8();
L_802423D8: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024243C;
    r3 = r31;
    r4 = r27;
    r5 = 0x1a0;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a0;
    fn_80239EE8();
L_8024243C: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802424A0;
    r3 = r31;
    r4 = r27;
    r5 = 0x1a1;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a1;
    fn_80239EE8();
L_802424A0: ;
    r3 = r27;
    r4 = r30;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242504;
    r3 = r31;
    r4 = r27;
    r5 = 0x1a2;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a2;
    fn_80239EE8();
L_80242504: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242568;
    r3 = r31;
    r4 = r27;
    r5 = 0x1a3;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a3;
    fn_80239EE8();
L_80242568: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802425CC;
    r3 = r31;
    r4 = r27;
    r5 = 0x1a4;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a4;
    fn_80239EE8();
L_802425CC: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242630;
    r3 = r31;
    r4 = r27;
    r5 = 0x1a5;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1a5;
    fn_80239EE8();
L_80242630: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80242648 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80242648(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236D60();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    r31 = 0x0;
    fn_80236520();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    r4 = r27;
    r5 = r29;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_80242718;
    r0 = r30 & 0xFFFF;
    if ((s32)r3 == (s32)0x0) goto L_80242718;
    if ((u32)r0 == (u32)0xffff) goto L_80242718;
    if ((u32)r0 == (u32)0x165) goto L_80242718;
    if ((u32)r0 == (u32)0x163) goto L_80242718;
    r3 = r26;
    r4 = r30;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242718;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x19c;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x19c;
    fn_80239EE8();
L_80242718: ;
    r3 = r31;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80242730 | Size: 0x170 (368 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80242730(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E630[];
    extern u8 lbl_8047E638[];
    extern u8 lbl_8047E63C[];
    extern void fn_80205B8C();
    extern void fn_802373B0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    f1 = *(f32*)lbl_8047E638;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = 0x0;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802427B4;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x19b;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x19b;
    fn_80239EE8();
    goto L_80242888;
L_802427B4: ;
    f1 = *(f32*)lbl_8047E63C;
    r3 = r28;
    r4 = r29;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242820;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x19a;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x19a;
    fn_80239EE8();
    goto L_80242888;
L_80242820: ;
    f1 = *(f32*)lbl_8047E630;
    r3 = r28;
    r4 = r29;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242888;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x199;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x199;
    fn_80239EE8();
L_80242888: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802428A0 | Size: 0x134 (308 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802428A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236D60();
    extern void fn_802391E0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    r31 = 0x0;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    r30 = r3;
    if ((s32)r0 == (s32)0) goto L_802429BC;
    if ((u32)r0 == (u32)0xffff) goto L_802429BC;
    if ((u32)r0 == (u32)0x165) goto L_802429BC;
    if ((u32)r0 == (u32)0x163) goto L_802429BC;
    r3 = r26;
    r4 = r27;
    r5 = r29;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_802429BC;
    r3 = r26;
    r4 = r30;
    fn_802391E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x5) goto L_8024296C;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x197;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x197;
    fn_80239EE8();
    goto L_802429BC;
L_8024296C: ;
    if ((u32)r0 > (u32)0xa) goto L_802429BC;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x198;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x198;
    fn_80239EE8();
L_802429BC: ;
    r3 = r31;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802429D4 | Size: 0x17C (380 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802429D4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236FFC();
    extern void fn_802370AC();
    extern void fn_8023715C();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r6;
    r30 = r4;
    r29 = r3;
    r31 = r5;
    r4 = r25;
    r26 = 0x0;
    fn_80236FFC();
    r28 = r3;
    r3 = r29;
    r4 = r25;
    fn_8023715C();
    r27 = r3;
    r3 = r29;
    r4 = r25;
    fn_802370AC();
    r4 = r27 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    r28 = r3;
    if ((u32)r4 <= (u32)r0) goto L_80242A7C;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x194;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x194;
    fn_80239EE8();
L_80242A7C: ;
    r3 = r27 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    if ((u32)r3 <= (u32)r0) goto L_80242AD4;
    r3 = r26;
    r4 = r29;
    r5 = 0x195;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x195;
    fn_80239EE8();
L_80242AD4: ;
    r3 = r29;
    r4 = r25;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242B38;
    r3 = r26;
    r4 = r29;
    r5 = 0x196;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x196;
    fn_80239EE8();
L_80242B38: ;
    r3 = r26;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80242B50 | Size: 0x17C (380 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80242B50(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236FFC();
    extern void fn_802370AC();
    extern void fn_8023715C();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r6;
    r30 = r4;
    r29 = r3;
    r31 = r5;
    r4 = r25;
    r26 = 0x0;
    fn_80236FFC();
    r28 = r3;
    r3 = r29;
    r4 = r25;
    fn_8023715C();
    r27 = r3;
    r3 = r29;
    r4 = r25;
    fn_802370AC();
    r4 = r28 & 0xFFFF;
    r0 = r27 & 0xFFFF;
    r28 = r3;
    if ((u32)r4 <= (u32)r0) goto L_80242BF8;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x191;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x191;
    fn_80239EE8();
L_80242BF8: ;
    r3 = r27 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    if ((u32)r3 <= (u32)r0) goto L_80242C50;
    r3 = r26;
    r4 = r29;
    r5 = 0x192;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x192;
    fn_80239EE8();
L_80242C50: ;
    r3 = r29;
    r4 = r25;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242CB4;
    r3 = r26;
    r4 = r29;
    r5 = 0x193;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x193;
    fn_80239EE8();
L_80242CB4: ;
    r3 = r26;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80242CCC | Size: 0xE4 (228 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80242CCC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802364BC();
    extern void fn_80236D60();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    r31 = 0x0;
    fn_802364BC();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    r4 = r27;
    r5 = r29;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_80242D98;
    r0 = r30 & 0xFFFF;
    if ((s32)r3 == (s32)0x0) goto L_80242D98;
    if ((u32)r0 == (u32)0xffff) goto L_80242D98;
    if ((u32)r0 == (u32)0x165) goto L_80242D98;
    if ((u32)r0 == (u32)0x163) goto L_80242D98;
    r3 = r26;
    r4 = r30;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x163) goto L_80242D98;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x190;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x190;
    fn_80239EE8();
L_80242D98: ;
    r3 = r31;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80242DB0 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80242DB0(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8047E630[];
    extern void fn_80205B8C();
    extern void fn_802373B0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    f1 = *(f32*)lbl_8047E630;
    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r30 = r5;
    r28 = r3;
    r4 = r6;
    r31 = 0x0;
    r5 = 0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80242E34;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x18f;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x18f;
    fn_80239EE8();
L_80242E34: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80242E4C | Size: 0x1A0 (416 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80242E4C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_80236F4C();
    extern void fn_80236FFC();
    extern void fn_802370AC();
    extern void fn_8023715C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r24, 0x10(r1) */;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r4 = r30;
    r31 = 0x0;
    fn_8023715C();
    r24 = r3;
    r3 = r27;
    r4 = r30;
    fn_80236FFC();
    r26 = r3;
    r3 = r27;
    r4 = r30;
    fn_802370AC();
    r25 = r3;
    r3 = r27;
    r4 = r30;
    fn_80236F4C();
    r4 = r24 & 0xFFFF;
    r0 = r26 & 0xFFFF;
    r26 = r3;
    if ((u32)r4 <= (u32)r0) goto L_80242F04;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x18c;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x18c;
    fn_80239EE8();
L_80242F04: ;
    r3 = r25 & 0xFFFF;
    r0 = r26 & 0xFFFF;
    if ((u32)r3 <= (u32)r0) goto L_80242F5C;
    r3 = r31;
    r4 = r27;
    r5 = 0x18d;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x18d;
    fn_80239EE8();
L_80242F5C: ;
    r3 = r27;
    r4 = r30;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_80242FD4;
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_80242FD4;
    r3 = r31;
    r4 = r27;
    r5 = 0x18e;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x18e;
    fn_80239EE8();
L_80242FD4: ;
    r3 = r31;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80242FEC | Size: 0xF8 (248 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80242FEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r4 = r30;
    r31 = 0x0;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_8024306C;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x18a;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x18a;
    fn_80239EE8();
L_8024306C: ;
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_802430CC;
    r3 = r31;
    r4 = r27;
    r5 = 0x18b;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x18b;
    fn_80239EE8();
L_802430CC: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802430E4 | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802430E4(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_802358AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_80243160;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x189;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x189;
    fn_80239EE8();
L_80243160: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80243178 | Size: 0x10C (268 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80243178(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235974();
    extern void fn_80236F4C();
    extern void fn_802370AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    r30 = 0x0;
    fn_802370AC();
    r31 = r3;
    r3 = r26;
    r4 = r29;
    fn_80236F4C();
    r4 = r31 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    if ((u32)r4 >= (u32)r0) goto L_8024320C;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x187;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x187;
    fn_80239EE8();
L_8024320C: ;
    r3 = r26;
    r4 = r29;
    fn_80235974();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_8024326C;
    r3 = r30;
    r4 = r26;
    r5 = 0x188;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x188;
    fn_80239EE8();
L_8024326C: ;
    r3 = r30;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80243284 | Size: 0x10C (268 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80243284(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235A3C();
    extern void fn_80236F4C();
    extern void fn_802370AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    r30 = 0x0;
    fn_802370AC();
    r31 = r3;
    r3 = r26;
    r4 = r29;
    fn_80236F4C();
    r4 = r31 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    if ((u32)r4 <= (u32)r0) goto L_80243318;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x185;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x185;
    fn_80239EE8();
L_80243318: ;
    r3 = r26;
    r4 = r29;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_80243378;
    r3 = r30;
    r4 = r26;
    r5 = 0x186;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x186;
    fn_80239EE8();
L_80243378: ;
    r3 = r30;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80243390 | Size: 0x10C (268 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80243390(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235AA0();
    extern void fn_80236FFC();
    extern void fn_8023715C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    r30 = 0x0;
    fn_8023715C();
    r31 = r3;
    r3 = r26;
    r4 = r29;
    fn_80236FFC();
    r4 = r31 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    if ((u32)r4 <= (u32)r0) goto L_80243424;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x183;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x183;
    fn_80239EE8();
L_80243424: ;
    r3 = r26;
    r4 = r29;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_80243484;
    r3 = r30;
    r4 = r26;
    r5 = 0x184;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x184;
    fn_80239EE8();
L_80243484: ;
    r3 = r30;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024349C | Size: 0x138 (312 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024349C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80236D60();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r24, 0x30(r1) */;
    r31 = r3;
    r24 = r4;
    r25 = r5;
    r26 = r6;
    r4 = r31;
    r5 = r1 + 0x8;
    r28 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fn_801F1C18();
    r29 = r1 + 0x8;
    r30 = r3 & 0xFFFF;
    r27 = 0x0;
    goto L_80243510;
L_802434E8: ;
    /* clrlslwi r0, r27, 16, 2 */;
    r3 = r31;
    r5 = *(u32*)(r29 + r0);
    r4 = r26;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_8024350C;
    r28 = 0x1;
    goto L_8024351C;
L_8024350C: ;
    r27 = r27 + 0x1;
L_80243510: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_802434E8;
L_8024351C: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243574;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x181;
    fn_80239984();
    r0 = r3;
    r3 = r24;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r25;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x181;
    fn_80239EE8();
    goto L_802435BC;
L_80243574: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x182;
    fn_80239984();
    r0 = r3;
    r3 = r24;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r25;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x182;
    fn_80239EE8();
L_802435BC: ;
    r3 = r29;
    /* lmw r24, 0x30(r1) */;
    return;
}
#pragma pop

/* Address: 0x802435D4 | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802435D4(void* ctx, u32 param1, u32 param2) {
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_802376EC();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0x1c(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    fn_802376EC();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    fn_802376EC();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    r30 = (s32)r3 / (s32)r0;
    r3 = 0x0;
    r4 = 0x180;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r30 = r30 * r3;
    r3 = 0x0;
    r4 = r30;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x180;
    fn_80239CCC();
    r3 = r31;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024368C | Size: 0x1AC (428 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024368C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802387C8();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r30 = r3;
    r26 = r4;
    r31 = r5;
    r27 = r6;
    r28 = 0x0;
    fn_802387C8();
    r29 = r3;
    r3 = r30;
    r4 = r27;
    fn_802387C8();
    r0 = r29 * 0x3;
    if ((s32)r0 > (s32)r3) goto L_8024371C;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x17d;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x17d;
    fn_80239EE8();
    goto L_80243820;
L_8024371C: ;
    r0 = r29 << 1;
    if ((s32)r0 > (s32)r3) goto L_80243774;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x17c;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x17c;
    fn_80239EE8();
    goto L_80243820;
L_80243774: ;
    r0 = r3 * 0x3;
    if ((s32)r29 < (s32)r0) goto L_802437CC;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x17f;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x17f;
    fn_80239EE8();
    goto L_80243820;
L_802437CC: ;
    r0 = r3 << 1;
    if ((s32)r29 < (s32)r0) goto L_80243820;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x17e;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x17e;
    fn_80239EE8();
L_80243820: ;
    r3 = r28;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80243838 | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80243838(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r30 = r5;
    r28 = r3;
    r4 = r6;
    r31 = 0x0;
    r5 = 0x17;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_802438B4;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x17b;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x17b;
    fn_80239EE8();
L_802438B4: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802438CC | Size: 0x140 (320 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802438CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F0134();
    extern void fn_801F54A4();
    extern void fn_80201D84();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    r29 = r6;
    r30 = 0x0;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r27;
    fn_801F0134();
    r31 = r3;
    r3 = r26;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80243974;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x179;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x179;
    fn_80239EE8();
L_80243974: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802439F4;
    r3 = r29;
    r4 = 0x1d;
    fn_80201D84();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_802439F4;
    r3 = r30;
    r4 = r26;
    r5 = 0x17a;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x17a;
    fn_80239EE8();
L_802439F4: ;
    r3 = r30;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80243A0C | Size: 0x250 (592 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80243A0C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E630[];
    extern u8 lbl_8047E640[];
    extern void fn_80205B8C();
    extern void fn_80235714();
    extern void fn_802373B0();
    extern void fn_80237DBC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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
    f32 f1 = 0.0f;

    /* stmw r27, 0xc(r1) */;
    r29 = r5;
    r27 = r3;
    r28 = r4;
    r30 = 0x0;
    r5 = 0x7;
    fn_80237DBC();
    r31 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243AB0;
    f1 = *(f32*)lbl_8047E630;
    r3 = r27;
    r4 = r28;
    r5 = 0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243AB0;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x174;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x174;
    fn_80239EE8();
    goto L_80243B24;
L_80243AB0: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243B24;
    f1 = *(f32*)lbl_8047E640;
    r3 = r27;
    r4 = r28;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243B24;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x175;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x175;
    fn_80239EE8();
L_80243B24: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243B88;
    r3 = r27;
    r4 = r28;
    fn_80235714();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243B88;
    r3 = r30;
    r4 = r27;
    r5 = 0x176;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x176;
    fn_80239EE8();
L_80243B88: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243BDC;
    r3 = r30;
    r4 = r27;
    r5 = 0x177;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x177;
    fn_80239EE8();
L_80243BDC: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243C44;
    r3 = r27;
    r4 = r28;
    fn_80235714();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243C44;
    r3 = r30;
    r4 = r27;
    r5 = 0x178;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x178;
    fn_80239EE8();
L_80243C44: ;
    r3 = r30;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80243C5C | Size: 0x7C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80243C5C(void* ctx, u32 slot, u32 param) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r31 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r29;
    r5 = 0x173;
    fn_80239984();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x173;
    fn_80239EE8();
    r3 = r31;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80243CD8 | Size: 0x640 (1600 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80243CD8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478DF8[];
    extern u8 lbl_8047E630[];
    extern void fn_8011BEB4();
    extern void fn_801F1990();
    extern void fn_801F1C18();
    extern void fn_80205B8C();
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
    extern void fn_8024B474();
    extern void fn_8024BFC0();
    extern void fn_8024E52C();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r15, 0x4c(r1) */;
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
    r5 = r1 + 0x20;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1C18();
    r15 = r3;
    r3 = r31;
    r4 = r16;
    r5 = r1 + 0x8;
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
    r17 = r1 + 0x8;
    r16 = r16 & 0xFFFF;
    r18 = 0x0;
    r19 = 0x0;
    goto L_80243DAC;
L_80243D70: ;
    r3 = r31;
    r4 = r28;
    fn_80239500();
    /* clrlslwi r0, r19, 16, 1 */;
    r6 = r3;
    r5 = *(u16*)(r17 + r0);
    r3 = r31;
    r4 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x41) goto L_80243DA8;
    r18 = 0x1;
    goto L_80243DB8;
L_80243DA8: ;
    r19 = r19 + 0x1;
L_80243DAC: ;
    r0 = r19 & 0xFFFF;
    if ((u32)r0 < (u32)r16) goto L_80243D70;
L_80243DB8: ;
    r0 = r18 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243E0C;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x16a;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x16a;
    fn_80239EE8();
L_80243E0C: ;
    f1 = *(f32*)lbl_8047E630;
    r3 = r31;
    r4 = r30;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243E74;
    r3 = r29;
    r4 = r31;
    r5 = 0x16b;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x16b;
    fn_80239EE8();
L_80243E74: ;
    r16 = r1 + 0x20;
    r21 = r15 & 0xFFFF;
    r23 = 0x0;
    goto L_80243F7C;
L_80243E84: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r4 = *(u32*)(r16 + r0);
    if ((u32)r30 == (u32)r4) goto L_80243F78;
    r3 = r31;
    r5 = r1 + 0xc;
    r6 = 0x0;
    r7 = 0x1;
    fn_802367CC();
    r17 = r3 & 0xFFFF;
    r24 = r3;
    if ((u32)r30 == (u32)r4) goto L_80243F78;
    r4 = (u32)fn_8024B474;
    r3 = (u32)fn_8024BFC0;
    r5 = (u32)fn_8024E52C;
    r15 = r1 + 0xc;
    r19 = (u32)fn_8024B474;
    r20 = (u32)fn_8024BFC0;
    r18 = (u32)fn_8024E52C;
    r22 = 0x0;
    goto L_80243F5C;
L_80243ED8: ;
    /* clrlslwi r0, r22, 16, 1 */;
    r3 = 0x0;
    r4 = *(u16*)(r15 + r0);
    r5 = 0x1c;
    r6 = 0x0;
    fn_8011BEB4();
    if ((u32)r3 != (u32)0x0) goto L_80243EFC;
    r3 = r18;
L_80243EFC: ;
    if ((u32)r3 == (u32)r19) goto L_80243F0C;
    if ((u32)r3 != (u32)r20) goto L_80243F58;
L_80243F0C: ;
    r3 = r29;
    r4 = r31;
    r5 = 0x16c;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x16c;
    fn_80239EE8();
    goto L_80243F68;
L_80243F58: ;
    r22 = r22 + 0x1;
L_80243F5C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r17) goto L_80243ED8;
L_80243F68: ;
    r3 = r22 & 0xFFFF;
    r0 = r24 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_80243F88;
L_80243F78: ;
    r23 = r23 + 0x1;
L_80243F7C: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r21) goto L_80243E84;
L_80243F88: ;
    r3 = (u32)fn_8024B474;
    r15 = 0x0;
    r16 = (u32)fn_8024B474;
    goto L_8024404C;
L_80243F98: ;
    r4 = r15;
    r3 = 0x0;
    r5 = 0x1c;
    r6 = 0x0;
    fn_8011BEB4();
    if ((u32)r3 != (u32)0x0) goto L_80243FBC;
    r3 = (u32)fn_8024E52C;
    r3 = (u32)fn_8024E52C;
L_80243FBC: ;
    if ((u32)r3 == (u32)r16) goto L_80243FD4;
    r4 = (u32)fn_8024BFC0;
    r0 = (u32)fn_8024BFC0;
    if ((u32)r3 != (u32)r0) goto L_80244048;
L_80243FD4: ;
    r4 = r31;
    r7 = r15;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244048;
    r3 = r29;
    r4 = r31;
    r5 = 0x16d;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x16d;
    fn_80239EE8();
    goto L_80244060;
L_80244048: ;
    r15 = r15 + 0x1;
L_8024404C: ;
    r3 = *(u32*)lbl_80478DF8;
    r4 = r15 & 0xFFFF;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r4 < (u32)r0) goto L_80243F98;
L_80244060: ;
    r3 = r31;
    r4 = r30;
    r15 = 0x0;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244084;
    r15 = 0x1;
L_80244084: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802440A4;
    r15 = 0x1;
L_802440A4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802440C4;
    r15 = 0x1;
L_802440C4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802440E4;
    r15 = 0x1;
L_802440E4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244104;
    r15 = 0x1;
L_80244104: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244124;
    r15 = 0x1;
L_80244124: ;
    r0 = r15 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244178;
    r3 = r29;
    r4 = r31;
    r5 = 0x16e;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x16e;
    fn_80239EE8();
L_80244178: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 != (u32)0xcb) goto L_802441E8;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xfc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 == (s32)0x0) goto L_802441E8;
    r3 = r29;
    r4 = r31;
    r5 = 0x16f;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x16f;
    fn_80239EE8();
L_802441E8: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 == (u32)0x11) goto L_80244244;
    if ((u32)r0 == (u32)0xf) goto L_80244244;
    r3 = r29;
    r4 = r31;
    r5 = 0x170;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x170;
    fn_80239EE8();
L_80244244: ;
    r3 = r31;
    r4 = r30;
    fn_802376EC();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802442A4;
    r3 = r29;
    r4 = r31;
    r5 = 0x171;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x171;
    fn_80239EE8();
L_802442A4: ;
    r0 = r25 & 0xFF;
    if ((u32)r0 == (u32)0x4) goto L_802442B8;
    if ((u32)r0 != (u32)0x3) goto L_80244300;
L_802442B8: ;
    r3 = r29;
    r4 = r31;
    r5 = 0x172;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x172;
    fn_80239EE8();
L_80244300: ;
    r3 = r29;
    /* lmw r15, 0x4c(r1) */;
    return;
}
#pragma pop

/* Address: 0x80244318 | Size: 0x160 (352 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80244318(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_80236BFC();
    extern void fn_80237DBC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r29 = r5;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r31 = 0x0;
    r4 = r30;
    r5 = 0x7;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024439C;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x167;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x167;
    fn_80239EE8();
L_8024439C: ;
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x8) goto L_802443FC;
    r3 = r31;
    r4 = r27;
    r5 = 0x168;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x168;
    fn_80239EE8();
L_802443FC: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x19;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244460;
    r3 = r31;
    r4 = r27;
    r5 = 0x169;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x169;
    fn_80239EE8();
L_80244460: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80244478 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80244478(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r28 = r3;
    r30 = r5;
    r31 = 0x0;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xed;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_802444FC;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x166;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x166;
    fn_80239EE8();
L_802444FC: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80244514 | Size: 0x18C (396 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80244514(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8010C4A0();
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236FFC();
    extern void fn_8023715C();
    extern void fn_802395C8();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r22, 0x28(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r5 = r1 + 0x8;
    r31 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1C18();
    r25 = r3;
    r3 = r27;
    r4 = r30;
    fn_80236520();
    r24 = r1 + 0x8;
    r23 = r3;
    r26 = r25 & 0xFFFF;
    r22 = 0x0;
    goto L_802445F4;
L_80244574: ;
    /* clrlslwi r25, r22, 16, 2 */;
    r3 = r27;
    r4 = *(u32*)(r24 + r25);
    fn_8023715C();
    r4 = *(u32*)(r24 + r25);
    r25 = r3;
    r3 = r27;
    fn_80236FFC();
    r4 = r25 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    if ((u32)r4 >= (u32)r0) goto L_802445F0;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x164;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x164;
    fn_80239EE8();
    goto L_80244600;
L_802445F0: ;
    r22 = r22 + 0x1;
L_802445F4: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_80244574;
L_80244600: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 == (u32)r26) goto L_80244688;
    if ((u32)r0 == (u32)0xffff) goto L_80244688;
    if ((u32)r0 == (u32)0x165) goto L_80244688;
    if ((u32)r0 == (u32)0x163) goto L_80244688;
    r3 = r27;
    r4 = r23;
    r5 = r30;
    fn_802395C8();
    fn_8010C4A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_80244688;
    r3 = r31;
    r4 = r27;
    r5 = 0x165;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x165;
    fn_80239EE8();
L_80244688: ;
    r3 = r31;
    /* lmw r22, 0x28(r1) */;
    return;
}
#pragma pop

/* Address: 0x802446A0 | Size: 0x18C (396 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802446A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8010C4A0();
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236FFC();
    extern void fn_8023715C();
    extern void fn_802395C8();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r22, 0x28(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r5 = r1 + 0x8;
    r31 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1C18();
    r25 = r3;
    r3 = r27;
    r4 = r30;
    fn_80236520();
    r24 = r1 + 0x8;
    r23 = r3;
    r26 = r25 & 0xFFFF;
    r22 = 0x0;
    goto L_80244780;
L_80244700: ;
    /* clrlslwi r25, r22, 16, 2 */;
    r3 = r27;
    r4 = *(u32*)(r24 + r25);
    fn_8023715C();
    r4 = *(u32*)(r24 + r25);
    r25 = r3;
    r3 = r27;
    fn_80236FFC();
    r4 = r25 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    if ((u32)r4 <= (u32)r0) goto L_8024477C;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x162;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x162;
    fn_80239EE8();
    goto L_8024478C;
L_8024477C: ;
    r22 = r22 + 0x1;
L_80244780: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_80244700;
L_8024478C: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 == (u32)r26) goto L_80244814;
    if ((u32)r0 == (u32)0xffff) goto L_80244814;
    if ((u32)r0 == (u32)0x165) goto L_80244814;
    if ((u32)r0 == (u32)0x163) goto L_80244814;
    r3 = r27;
    r4 = r23;
    r5 = r30;
    fn_802395C8();
    fn_8010C4A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244814;
    r3 = r31;
    r4 = r27;
    r5 = 0x163;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x163;
    fn_80239EE8();
L_80244814: ;
    r3 = r31;
    /* lmw r22, 0x28(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024482C | Size: 0xD4 (212 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024482C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80205B8C();
    extern void fn_80238748();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x1;
    r7 = 0x1;
    /* stmw r25, 0x74(r1) */;
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r5 = r1 + 0x8;
    r4 = r25;
    r29 = 0x0;
    r3 = 0x0;
    fn_801F1A6C();
    r30 = r1 + 0x8;
    r31 = r3 & 0xFFFF;
    r28 = 0x0;
    goto L_802448DC;
L_80244874: ;
    /* clrlslwi r0, r28, 16, 2 */;
    r3 = r25;
    r4 = *(u32*)(r30 + r0);
    fn_80238748();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_802448D8;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x161;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x161;
    fn_80239EE8();
    goto L_802448E8;
L_802448D8: ;
    r28 = r28 + 0x1;
L_802448DC: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_80244874;
L_802448E8: ;
    r3 = r29;
    /* lmw r25, 0x74(r1) */;
    return;
}
#pragma pop

/* Address: 0x80244900 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80244900(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80244974;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x160;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x160;
    fn_80239EE8();
L_80244974: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024498C | Size: 0x318 (792 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024498C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_802377E8();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239058();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r21, 0x94(r1) */;
    r26 = r4;
    r27 = r5;
    r25 = r3;
    r28 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r29 = r3;
    r4 = r25;
    r5 = r1 + 0x28;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r30 = r3;
    r4 = r25;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r0 = r29 & 0xFF;
    r31 = r3;
    if ((u32)r0 == (u32)0x3) goto L_80244A48;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x15b;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x15b;
    fn_80239EE8();
L_80244A48: ;
    r22 = r1 + 0x28;
    r24 = r30 & 0xFFFF;
    r21 = 0x0;
    goto L_80244B1C;
L_80244A58: ;
    /* clrlslwi r23, r21, 16, 2 */;
    r3 = r25;
    r4 = *(u32*)(r22 + r23);
    r5 = 0x5;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80244ACC;
    r4 = *(u32*)(r22 + r23);
    r3 = r25;
    r5 = 0x4;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80244ACC;
    r4 = *(u32*)(r22 + r23);
    r3 = r25;
    r5 = 0x8;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80244ACC;
    r4 = *(u32*)(r22 + r23);
    r3 = r25;
    r5 = 0x8;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244B18;
L_80244ACC: ;
    r3 = r28;
    r4 = r25;
    r5 = 0x15c;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x15c;
    fn_80239EE8();
    goto L_80244B28;
L_80244B18: ;
    r21 = r21 + 0x1;
L_80244B1C: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_80244A58;
L_80244B28: ;
    r24 = r1 + 0x8;
    r31 = r31 & 0xFFFF;
    r22 = 0x0;
    goto L_80244BA4;
L_80244B38: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r25;
    r4 = *(u32*)(r24 + r0);
    fn_802377E8();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x12f) goto L_80244BA0;
    r3 = r28;
    r4 = r25;
    r5 = 0x15d;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x15d;
    fn_80239EE8();
    goto L_80244BB0;
L_80244BA0: ;
    r22 = r22 + 0x1;
L_80244BA4: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_80244B38;
L_80244BB0: ;
    r31 = r1 + 0x28;
    r30 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_80244C2C;
L_80244BC0: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r25;
    r4 = *(u32*)(r31 + r0);
    fn_80238980();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x181) goto L_80244C28;
    r3 = r28;
    r4 = r25;
    r5 = 0x15e;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x15e;
    fn_80239EE8();
    goto L_80244C38;
L_80244C28: ;
    r22 = r22 + 0x1;
L_80244C2C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_80244BC0;
L_80244C38: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x3) goto L_80244C8C;
    r3 = r28;
    r4 = r25;
    r5 = 0x15f;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x15f;
    fn_80239EE8();
L_80244C8C: ;
    r3 = r28;
    /* lmw r21, 0x94(r1) */;
    return;
}
#pragma pop

/* Address: 0x80244CA4 | Size: 0x2C4 (708 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80244CA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_802377E8();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0x88(r1) */;
    r26 = r4;
    r27 = r5;
    r25 = r3;
    r28 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r29 = r3;
    r4 = r25;
    r5 = r1 + 0x28;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r30 = r3;
    r4 = r25;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r0 = r29 & 0xFF;
    r31 = r3;
    if ((u32)r0 == (u32)0x4) goto L_80244D60;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x156;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x156;
    fn_80239EE8();
L_80244D60: ;
    r23 = r1 + 0x28;
    r24 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_80244DE0;
L_80244D70: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = r25;
    r4 = *(u32*)(r23 + r0);
    r5 = 0xf;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80244DDC;
    r3 = r28;
    r4 = r25;
    r5 = 0x157;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x157;
    fn_80239EE8();
    goto L_80244DEC;
L_80244DDC: ;
    r22 = r22 + 0x1;
L_80244DE0: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_80244D70;
L_80244DEC: ;
    r24 = r1 + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    goto L_80244E68;
L_80244DFC: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r3 = r25;
    r4 = *(u32*)(r24 + r0);
    fn_802377E8();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x12f) goto L_80244E64;
    r3 = r28;
    r4 = r25;
    r5 = 0x158;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x158;
    fn_80239EE8();
    goto L_80244E74;
L_80244E64: ;
    r23 = r23 + 0x1;
L_80244E68: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_80244DFC;
L_80244E74: ;
    r31 = r1 + 0x28;
    r30 = r30 & 0xFFFF;
    r23 = 0x0;
    goto L_80244EF0;
L_80244E84: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r3 = r25;
    r4 = *(u32*)(r31 + r0);
    fn_80238980();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x181) goto L_80244EEC;
    r3 = r28;
    r4 = r25;
    r5 = 0x159;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x159;
    fn_80239EE8();
    goto L_80244EFC;
L_80244EEC: ;
    r23 = r23 + 0x1;
L_80244EF0: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_80244E84;
L_80244EFC: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x4) goto L_80244F50;
    r3 = r28;
    r4 = r25;
    r5 = 0x15a;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r25;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x15a;
    fn_80239EE8();
L_80244F50: ;
    r3 = r28;
    /* lmw r22, 0x88(r1) */;
    return;
}
#pragma pop

/* Address: 0x80244F68 | Size: 0x258 (600 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80244F68(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239058();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0x68(r1) */;
    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r30 = r3;
    r4 = r26;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r0 = r30 & 0xFF;
    r31 = r3;
    if ((u32)r0 == (u32)0x1) goto L_80245008;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x152;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x152;
    fn_80239EE8();
L_80245008: ;
    r23 = r1 + 0x8;
    r25 = r31 & 0xFFFF;
    r22 = 0x0;
    goto L_802450C0;
L_80245018: ;
    /* clrlslwi r24, r22, 16, 2 */;
    r3 = r26;
    r4 = *(u32*)(r23 + r24);
    r5 = 0xa;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80245070;
    r4 = *(u32*)(r23 + r24);
    r3 = r26;
    r5 = 0xc;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80245070;
    r4 = *(u32*)(r23 + r24);
    r3 = r26;
    r5 = 0x22;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802450BC;
L_80245070: ;
    r3 = r29;
    r4 = r26;
    r5 = 0x153;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x153;
    fn_80239EE8();
    goto L_802450CC;
L_802450BC: ;
    r22 = r22 + 0x1;
L_802450C0: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_80245018;
L_802450CC: ;
    r25 = r1 + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    goto L_80245148;
L_802450DC: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r3 = r26;
    r4 = *(u32*)(r25 + r0);
    fn_80238980();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x181) goto L_80245144;
    r3 = r29;
    r4 = r26;
    r5 = 0x154;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x154;
    fn_80239EE8();
    goto L_80245154;
L_80245144: ;
    r23 = r23 + 0x1;
L_80245148: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_802450DC;
L_80245154: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802451A8;
    r3 = r29;
    r4 = r26;
    r5 = 0x155;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x155;
    fn_80239EE8();
L_802451A8: ;
    r3 = r29;
    /* lmw r22, 0x68(r1) */;
    return;
}
#pragma pop

/* Address: 0x802451C0 | Size: 0x258 (600 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802451C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_80238980();
    extern void fn_80238E30();
    extern void fn_80239058();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0x68(r1) */;
    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    fn_80235B04();
    r30 = r3;
    r4 = r26;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r0 = r30 & 0xFF;
    r31 = r3;
    if ((u32)r0 == (u32)0x2) goto L_80245260;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x14e;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x14e;
    fn_80239EE8();
L_80245260: ;
    r23 = r1 + 0x8;
    r25 = r31 & 0xFFFF;
    r22 = 0x0;
    goto L_80245318;
L_80245270: ;
    /* clrlslwi r24, r22, 16, 2 */;
    r3 = r26;
    r4 = *(u32*)(r23 + r24);
    r5 = 0xb;
    fn_80238E30();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802452C8;
    r4 = *(u32*)(r23 + r24);
    r3 = r26;
    r5 = 0x21;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802452C8;
    r4 = *(u32*)(r23 + r24);
    r3 = r26;
    r5 = 0x2c;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245314;
L_802452C8: ;
    r3 = r29;
    r4 = r26;
    r5 = 0x14f;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x14f;
    fn_80239EE8();
    goto L_80245324;
L_80245314: ;
    r22 = r22 + 0x1;
L_80245318: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_80245270;
L_80245324: ;
    r25 = r1 + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    goto L_802453A0;
L_80245334: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r3 = r26;
    r4 = *(u32*)(r25 + r0);
    fn_80238980();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x181) goto L_8024539C;
    r3 = r29;
    r4 = r26;
    r5 = 0x150;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x150;
    fn_80239EE8();
    goto L_802453AC;
L_8024539C: ;
    r23 = r23 + 0x1;
L_802453A0: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_80245334;
L_802453AC: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_80245400;
    r3 = r29;
    r4 = r26;
    r5 = 0x151;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x151;
    fn_80239EE8();
L_80245400: ;
    r3 = r29;
    /* lmw r22, 0x68(r1) */;
    return;
}
#pragma pop

/* Address: 0x80245418 | Size: 0x160 (352 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80245418(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_802384B4();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r24, 0x70(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r24 = r6;
    r4 = r29;
    r5 = r1 + 0x8;
    r26 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1A6C();
    r28 = r3;
    r3 = r29;
    r4 = r24;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802454B8;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x14c;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x14c;
    fn_80239EE8();
L_802454B8: ;
    r27 = r1 + 0x8;
    r28 = r28 & 0xFFFF;
    r25 = 0x0;
    goto L_80245554;
L_802454C8: ;
    r3 = r24;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* clrlslwi r0, r25, 16, 2 */;
    r4 = *(u32*)(r27 + r0);
    if ((u32)r3 == (u32)r4) goto L_80245550;
    r3 = r29;
    r5 = 0x8;
    fn_802384B4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245550;
    r3 = r26;
    r4 = r29;
    r5 = 0x14d;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x14d;
    fn_80239EE8();
    goto L_80245560;
L_80245550: ;
    r25 = r25 + 0x1;
L_80245554: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_802454C8;
L_80245560: ;
    r3 = r26;
    /* lmw r24, 0x70(r1) */;
    return;
}
#pragma pop

/* Address: 0x80245578 | Size: 0x1A0 (416 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80245578(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E630[];
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_802373B0();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    /* stmw r28, 0x10(r1) */;
    r30 = r3;
    r28 = r4;
    r31 = r5;
    r29 = 0x0;
    fn_8023831C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x3) goto L_802455B0;
    if ((u32)r0 != (u32)0x9) goto L_802455F8;
L_802455B0: ;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x148;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x148;
    fn_80239EE8();
L_802455F8: ;
    f1 = *(f32*)lbl_8047E630;
    r3 = r30;
    r4 = r28;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245660;
    r3 = r29;
    r4 = r30;
    r5 = 0x149;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x149;
    fn_80239EE8();
L_80245660: ;
    r3 = r30;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802456BC;
    r3 = r29;
    r4 = r30;
    r5 = 0x14a;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x14a;
    fn_80239EE8();
L_802456BC: ;
    r3 = r29;
    r4 = r30;
    r5 = 0x14b;
    fn_80239984();
    r29 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x14b;
    fn_80239EE8();
    r3 = r29;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80245718 | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80245718(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8047E630[];
    extern void fn_80205B8C();
    extern void fn_802373B0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    f1 = *(f32*)lbl_8047E630;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = 0x0;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245798;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x147;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x147;
    fn_80239EE8();
L_80245798: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802457B0 | Size: 0x168 (360 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802457B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E630[];
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_802373B0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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
    f32 f1 = 0.0f;

    /* stmw r27, 0xc(r1) */;
    r28 = r4;
    r29 = r5;
    r27 = r3;
    r30 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r31 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245834;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x144;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x144;
    fn_80239EE8();
L_80245834: ;
    f1 = *(f32*)lbl_8047E630;
    r3 = r27;
    r4 = r28;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024589C;
    r3 = r30;
    r4 = r27;
    r5 = 0x145;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x145;
    fn_80239EE8();
L_8024589C: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_802458B8;
    if ((u32)r0 == (u32)0x4) goto L_802458B8;
    if ((u32)r0 != (u32)0x3) goto L_80245900;
L_802458B8: ;
    r3 = r30;
    r4 = r27;
    r5 = 0x146;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x146;
    fn_80239EE8();
L_80245900: ;
    r3 = r30;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80245918 | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80245918(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r30 = r5;
    r28 = r3;
    r4 = r6;
    r31 = 0x0;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245998;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x143;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x143;
    fn_80239EE8();
L_80245998: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802459B0 | Size: 0x44C (1100 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802459B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F4354();
    extern void fn_801F8A18();
    extern void fn_80205B8C();
    extern void fn_80235BE4();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r6;
    r27 = r4;
    r28 = r5;
    r26 = r3;
    r5 = r29;
    r31 = 0x0;
    r4 = 0x0;
    r6 = 0x0;
    fn_80235BE4();
    r30 = r3;
    r4 = r29;
    r3 = 0x0;
    fn_801F4354();
    r0 = 0x0;
    r4 = r1 + 0x8;
    *(u16*)(sp + 0x8) = r0;
    fn_801F8A18();
    if ((u32)r3 != (u32)0x0) goto L_80245A10;
    r30 = 0x1;
L_80245A10: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245A74;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x139;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x139;
    fn_80239EE8();
L_80245A74: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245AD8;
    r3 = r31;
    r4 = r26;
    r5 = 0x13a;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x13a;
    fn_80239EE8();
L_80245AD8: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245B3C;
    r3 = r31;
    r4 = r26;
    r5 = 0x13b;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x13b;
    fn_80239EE8();
L_80245B3C: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245BA0;
    r3 = r31;
    r4 = r26;
    r5 = 0x13c;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x13c;
    fn_80239EE8();
L_80245BA0: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245C04;
    r3 = r31;
    r4 = r26;
    r5 = 0x13d;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x13d;
    fn_80239EE8();
L_80245C04: ;
    r3 = r26;
    r4 = r29;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245C68;
    r3 = r31;
    r4 = r26;
    r5 = 0x13e;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x13e;
    fn_80239EE8();
L_80245C68: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245CCC;
    r3 = r31;
    r4 = r26;
    r5 = 0x13f;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x13f;
    fn_80239EE8();
L_80245CCC: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245D30;
    r3 = r31;
    r4 = r26;
    r5 = 0x140;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x140;
    fn_80239EE8();
L_80245D30: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80245D94;
    r3 = r31;
    r4 = r26;
    r5 = 0x141;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x141;
    fn_80239EE8();
L_80245D94: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80245DE4;
    r3 = r31;
    r4 = r26;
    r5 = 0x142;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x142;
    fn_80239EE8();
L_80245DE4: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80245DFC | Size: 0x14C (332 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80245DFC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r28 = r4;
    r29 = r5;
    r27 = r3;
    r30 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r31 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80245E40;
    if ((u32)r0 != (u32)0x2) goto L_80245E88;
L_80245E40: ;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x136;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x136;
    fn_80239EE8();
L_80245E88: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x4) goto L_80245EDC;
    r3 = r30;
    r4 = r27;
    r5 = 0x137;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x137;
    fn_80239EE8();
L_80245EDC: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x3) goto L_80245F30;
    r3 = r30;
    r4 = r27;
    r5 = 0x138;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x138;
    fn_80239EE8();
L_80245F30: ;
    r3 = r30;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80245F48 | Size: 0x7C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80245F48(void* ctx, u32 slot, u32 param) {
    extern void fn_801363E8();
    extern void fn_801F54A4();
    extern void fn_8023CA9C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0xf;
    r6 = 0x0;
    fn_801F54A4();
    r3 = r3 & 0xFFFF;
    fn_801363E8();
    r5 = r3;
    r0 = r30 & 0xFFFF;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)r0) goto L_80245FAC;
    r3 = r28;
    r4 = r29;
    r6 = r31;
    fn_8023CA9C();
    goto L_80245FB0;
L_80245FAC: ;
    r3 = 0x0;
L_80245FB0: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80247048 | Size: 0x7C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80247048(void* ctx, u32 slot, u32 param) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r31 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r29;
    r5 = 0x135;
    fn_80239984();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x135;
    fn_80239EE8();
    r3 = r31;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x802470C4 | Size: 0xEC (236 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802470C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r28;
    r5 = 0x133;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x133;
    fn_80239EE8();
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247198;
    r3 = r31;
    r4 = r28;
    r5 = 0x134;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x134;
    fn_80239EE8();
L_80247198: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802471B0 | Size: 0x128 (296 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802471B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80235AA0();
    extern void fn_80239564();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0x1c(r1) */;
    r29 = r5;
    r28 = r4;
    r27 = r3;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x131;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r31 = (s32)r30 / (s32)r3;
    r3 = 0x0;
    r4 = r31;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x131;
    fn_80239CCC();
    r3 = r27;
    r4 = r28;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_802472C0;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r31 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x132;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r31 = (s32)r31 / (s32)r3;
    r3 = r30;
    r4 = r31;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x132;
    fn_80239CCC();
L_802472C0: ;
    r3 = r30;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* Address: 0x802472D8 | Size: 0xDC (220 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802472D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235A3C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r28;
    r5 = 0x12f;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x12f;
    fn_80239EE8();
    r3 = r28;
    r4 = r29;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_8024739C;
    r3 = r31;
    r4 = r28;
    r5 = 0x130;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x130;
    fn_80239EE8();
L_8024739C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802473B4 | Size: 0x144 (324 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802473B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    fn_8023831C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x17) goto L_8024742C;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x12c;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x12c;
    fn_80239EE8();
L_8024742C: ;
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024749C;
    r3 = r31;
    r4 = r28;
    r5 = 0x12d;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x12d;
    fn_80239EE8();
L_8024749C: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x12e;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x12e;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802474F8 | Size: 0x1A8 (424 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802474F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r27 = r6;
    r28 = 0x0;
    fn_8023831C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x17) goto L_80247574;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x128;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x128;
    fn_80239EE8();
L_80247574: ;
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802475E4;
    r3 = r28;
    r4 = r29;
    r5 = 0x129;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x129;
    fn_80239EE8();
L_802475E4: ;
    r3 = r28;
    r4 = r29;
    r5 = 0x12a;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x12a;
    fn_80239EE8();
    r3 = r29;
    r4 = r27;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247688;
    r3 = r28;
    r4 = r29;
    r5 = 0x12b;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x12b;
    fn_80239EE8();
L_80247688: ;
    r3 = r28;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802476A0 | Size: 0x110 (272 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802476A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_802383A4();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r29 = r5;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r31 = 0x0;
    r4 = r30;
    r5 = 0x3d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80247734;
    r3 = r27;
    r4 = r30;
    fn_802383A4();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80247734;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x126;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x126;
    fn_80239EE8();
L_80247734: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x3c;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247798;
    r3 = r31;
    r4 = r27;
    r5 = 0x127;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x127;
    fn_80239EE8();
L_80247798: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802477B0 | Size: 0x158 (344 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802477B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80142984();
    extern void fn_80205B8C();
    extern void fn_80216048();
    extern void fn_80237F74();
    extern void fn_802383A4();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r24, 0x10(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r24 = r6;
    r27 = 0x0;
    fn_802383A4();
    r28 = r3;
    r3 = r29;
    r4 = r24;
    fn_802383A4();
    r0 = r3;
    r3 = r30;
    r25 = r0;
    r26 = 0x1;
    fn_80216048();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80247808;
    r26 = 0x0;
L_80247808: ;
    r0 = r28 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80247834;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 == (u32)0xaf) goto L_80247834;
    if ((u32)r0 == (u32)0x0) goto L_80247834;
    r3 = r25;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80247838;
L_80247834: ;
    r26 = 0x0;
L_80247838: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024788C;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x124;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x124;
    fn_80239EE8();
L_8024788C: ;
    r3 = r29;
    r4 = r24;
    r5 = 0x3c;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802478F0;
    r3 = r27;
    r4 = r29;
    r5 = 0x125;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x125;
    fn_80239EE8();
L_802478F0: ;
    r3 = r27;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80247908 | Size: 0x1C0 (448 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80247908(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r31 = r5;
    r29 = r3;
    r27 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r28 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024798C;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x120;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x120;
    fn_80239EE8();
L_8024798C: ;
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802479FC;
    r3 = r27;
    r4 = r29;
    r5 = 0x121;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x121;
    fn_80239EE8();
L_802479FC: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_80247A18;
    if ((u32)r0 == (u32)0x4) goto L_80247A18;
    if ((u32)r0 != (u32)0x3) goto L_80247A60;
L_80247A18: ;
    r3 = r27;
    r4 = r29;
    r5 = 0x122;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x122;
    fn_80239EE8();
L_80247A60: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x3) goto L_80247AB0;
    r3 = r27;
    r4 = r29;
    r5 = 0x123;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x123;
    fn_80239EE8();
L_80247AB0: ;
    r3 = r27;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80247AC8 | Size: 0x194 (404 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80247AC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r31 = r3;
    r27 = r4;
    r28 = r5;
    r29 = r6;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x11d;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11d;
    fn_80239EE8();
    r3 = r31;
    r4 = r29;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80247B8C;
    r3 = r30;
    r4 = r31;
    r5 = 0x11e;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11e;
    fn_80239EE8();
L_80247B8C: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80247BFC;
    r3 = r31;
    r4 = r29;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80247BFC;
    r3 = r31;
    r4 = r29;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80247BFC;
    r3 = r31;
    r4 = r29;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247C44;
L_80247BFC: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x11f;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11f;
    fn_80239EE8();
L_80247C44: ;
    r3 = r30;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80247C5C | Size: 0x184 (388 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80247C5C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x11a;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11a;
    fn_80239EE8();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247D34;
    r3 = r31;
    r4 = r27;
    r5 = 0x11b;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11b;
    fn_80239EE8();
L_80247D34: ;
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80247D80;
    r3 = r27;
    r4 = r30;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80247D80;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247DC8;
L_80247D80: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x11c;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11c;
    fn_80239EE8();
L_80247DC8: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80247DE0 | Size: 0x1C0 (448 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80247DE0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r27 = r6;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x116;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x116;
    fn_80239EE8();
    r3 = r28;
    r4 = r29;
    r5 = 0x117;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x117;
    fn_80239EE8();
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247EF8;
    r3 = r28;
    r4 = r29;
    r5 = 0x118;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x118;
    fn_80239EE8();
L_80247EF8: ;
    r3 = r29;
    r4 = r27;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80247F44;
    r3 = r29;
    r4 = r27;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80247F44;
    r3 = r29;
    r4 = r27;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80247F88;
L_80247F44: ;
    r3 = r28;
    r4 = r29;
    r5 = 0x119;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x119;
    fn_80239EE8();
L_80247F88: ;
    r3 = r28;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80247FA0 | Size: 0x1D0 (464 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80247FA0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r26 = r6;
    r4 = r31;
    fn_80239564();
    r27 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x113;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r28 = (s32)r27 / (s32)r3;
    r3 = 0x0;
    r4 = r28;
    fn_802399FC();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r29;
    /* subi r3, r6, 0x139c */;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x113;
    fn_80239CCC();
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024809C;
    r3 = r27;
    r4 = r29;
    r5 = 0x114;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x114;
    fn_80239EE8();
L_8024809C: ;
    r3 = r29;
    r4 = r26;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802480E8;
    r3 = r29;
    r4 = r26;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802480E8;
    r3 = r29;
    r4 = r26;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248158;
L_802480E8: ;
    r3 = r29;
    r4 = r31;
    fn_80239564();
    r28 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x115;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r28 = (s32)r28 / (s32)r3;
    r3 = r27;
    r4 = r28;
    fn_802399FC();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r29;
    /* subi r3, r6, 0x139c */;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x115;
    fn_80239CCC();
L_80248158: ;
    r3 = r27;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80248170 | Size: 0x150 (336 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80248170(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x1;
    r7 = 0x10e;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248200;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x110;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x110;
    fn_80239EE8();
L_80248200: ;
    r3 = r28;
    r4 = r29;
    r5 = 0x36;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248264;
    r3 = r31;
    r4 = r28;
    r5 = 0x111;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x111;
    fn_80239EE8();
L_80248264: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x112;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x112;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802482C0 | Size: 0x1A0 (416 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802482C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F0134();
    extern void fn_801F54A4();
    extern void fn_80201D84();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r26 = r6;
    r27 = 0x0;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r30;
    fn_801F0134();
    r28 = r3;
    r3 = r29;
    r4 = r26;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248388;
    r3 = r26;
    r4 = 0x1d;
    fn_80201D84();
    r3 = r3 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_80248388;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x10a;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10a;
    fn_80239EE8();
L_80248388: ;
    r3 = r27;
    r4 = r29;
    r5 = 0x10b;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10b;
    fn_80239EE8();
    r3 = r29;
    r4 = r26;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80248404;
    r3 = r29;
    r4 = r26;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248448;
L_80248404: ;
    r3 = r28;
    r4 = r29;
    r5 = 0x10c;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10c;
    fn_80239EE8();
L_80248448: ;
    r3 = r28;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80248460 | Size: 0x22C (556 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80248460(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F0134();
    extern void fn_801F1C18();
    extern void fn_801F54A4();
    extern void fn_80201D84();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80236D60();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0x28(r1) */;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    r29 = r6;
    r30 = 0x0;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r27;
    fn_801F0134();
    r31 = r3;
    r4 = r26;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1C18();
    r23 = r3;
    r24 = r1 + 0x8;
    r25 = r3 & 0xFFFF;
    r22 = 0x0;
    goto L_802484FC;
L_802484D4: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r5 = *(u32*)(r24 + r0);
    if ((u32)r5 == (u32)0x0) goto L_802484F8;
    r3 = r26;
    r4 = r29;
    fn_80236D60();
    if ((s32)r3 > (s32)0x0) goto L_80248508;
L_802484F8: ;
    r22 = r22 + 0x1;
L_802484FC: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_802484D4;
L_80248508: ;
    r3 = r22 & 0xFFFF;
    r0 = r23 & 0xFFFF;
    if ((u32)r3 >= (u32)r0) goto L_80248560;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x107;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x107;
    fn_80239EE8();
L_80248560: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802485E0;
    r3 = r29;
    r4 = 0x1d;
    fn_80201D84();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_802485E0;
    r3 = r30;
    r4 = r26;
    r5 = 0x108;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x108;
    fn_80239EE8();
L_802485E0: ;
    r3 = r26;
    r4 = r29;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r3) goto L_8024862C;
    r3 = r26;
    r4 = r29;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024862C;
    r3 = r26;
    r4 = r29;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248674;
L_8024862C: ;
    r3 = r30;
    r4 = r26;
    r5 = 0x109;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x109;
    fn_80239EE8();
L_80248674: ;
    r3 = r30;
    /* lmw r22, 0x28(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024868C | Size: 0x1D0 (464 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024868C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r26 = r6;
    r4 = r31;
    fn_80239564();
    r27 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x104;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r28 = (s32)r27 / (s32)r3;
    r3 = 0x0;
    r4 = r28;
    fn_802399FC();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r29;
    /* subi r3, r6, 0x139c */;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x104;
    fn_80239CCC();
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248788;
    r3 = r27;
    r4 = r29;
    r5 = 0x105;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x105;
    fn_80239EE8();
L_80248788: ;
    r3 = r29;
    r4 = r26;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802487D4;
    r3 = r29;
    r4 = r26;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802487D4;
    r3 = r29;
    r4 = r26;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248844;
L_802487D4: ;
    r3 = r29;
    r4 = r31;
    fn_80239564();
    r28 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0x106;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r28 = (s32)r28 / (s32)r3;
    r3 = r27;
    r4 = r28;
    fn_802399FC();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r29;
    /* subi r3, r6, 0x139c */;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x106;
    fn_80239CCC();
L_80248844: ;
    r3 = r27;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024885C | Size: 0x2C0 (704 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024885C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_802384B4();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r24, 0x70(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r5 = r1 + 0x10;
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
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_80248978;
    r3 = r31;
    r4 = r27;
    r5 = 0x101;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x101;
    fn_80239EE8();
L_80248978: ;
    r25 = r1 + 0x10;
    r26 = r26 & 0xFFFF;
    r24 = 0x0;
    goto L_80248A3C;
L_80248988: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* clrlslwi r0, r24, 16, 2 */;
    r4 = *(u32*)(r25 + r0);
    if ((u32)r3 == (u32)r4) goto L_80248A38;
    r3 = r27;
    r5 = 0x7;
    fn_802384B4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248A38;
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
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x102;
    fn_80239CCC();
    goto L_80248A48;
L_80248A38: ;
    r24 = r24 + 0x1;
L_80248A3C: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_80248988;
L_80248A48: ;
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r26) goto L_80248A94;
    r3 = r27;
    r4 = r30;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80248A94;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248B04;
L_80248A94: ;
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
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x103;
    fn_80239CCC();
L_80248B04: ;
    r3 = r31;
    /* lmw r24, 0x70(r1) */;
    return;
}
#pragma pop

/* Address: 0x80248B1C | Size: 0x220 (544 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80248B1C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    r4 = r28;
    r3 = 0x0;
    r5 = 0xfb;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r27 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xfb;
    fn_80239EE8();
    r3 = r27;
    r4 = r28;
    r5 = 0xfc;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xfc;
    fn_80239EE8();
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248C34;
    r3 = r27;
    r4 = r28;
    r5 = 0xfd;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xfd;
    fn_80239EE8();
L_80248C34: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248C94;
    r3 = r27;
    r4 = r28;
    r5 = 0xfe;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xfe;
    fn_80239EE8();
L_80248C94: ;
    r3 = r28;
    r4 = r31;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80248CE0;
    r3 = r28;
    r4 = r31;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80248CE0;
    r3 = r28;
    r4 = r31;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248D24;
L_80248CE0: ;
    r3 = r27;
    r4 = r28;
    r5 = 0xff;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xff;
    fn_80239EE8();
L_80248D24: ;
    r3 = r27;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80248D3C | Size: 0x288 (648 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80248D3C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;

    /* stmw r26, 0x18(r1) */;
    r29 = r5;
    r27 = r3;
    r28 = r4;
    r30 = r6;
    r31 = 0x0;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248DBC;
    r4 = r27;
    r3 = 0x0;
    r5 = 0xf6;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf6;
    fn_80239EE8();
L_80248DBC: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xf7;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r31 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf7;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248E94;
    r3 = r31;
    r4 = r27;
    r5 = 0xf8;
    fn_80239984();
    r31 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf8;
    fn_80239EE8();
L_80248E94: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248EF4;
    r3 = r31;
    r4 = r27;
    r5 = 0xf9;
    fn_80239984();
    r31 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf9;
    fn_80239EE8();
L_80248EF4: ;
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80248F40;
    r3 = r27;
    r4 = r30;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80248F40;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80248FAC;
L_80248F40: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xfa;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r30 = (s32)r30 / (s32)r3;
    r3 = r31;
    r4 = r30;
    fn_802399FC();
    r31 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xfa;
    fn_80239CCC();
L_80248FAC: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80248FC4 | Size: 0x49C (1180 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80248FC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F4354();
    extern void fn_801F8A18();
    extern void fn_80205B8C();
    extern void fn_80235BE4();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;

    /* stmw r26, 0x18(r1) */;
    r28 = r6;
    r26 = r4;
    r27 = r5;
    r31 = r3;
    r5 = r28;
    r30 = 0x0;
    r4 = 0x0;
    r6 = 0x0;
    fn_80235BE4();
    r29 = r3;
    r4 = r28;
    r3 = 0x0;
    fn_801F4354();
    r0 = 0x0;
    r4 = r1 + 0x8;
    *(u16*)(sp + 0x8) = r0;
    fn_801F8A18();
    if ((u32)r3 != (u32)0x0) goto L_80249024;
    r29 = 0x1;
L_80249024: ;
    r0 = r29 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80249074;
    r4 = r31;
    r3 = 0x0;
    r5 = 0xeb;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xeb;
    fn_80239EE8();
L_80249074: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802490D8;
    r3 = r30;
    r4 = r31;
    r5 = 0xec;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xec;
    fn_80239EE8();
L_802490D8: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024913C;
    r3 = r30;
    r4 = r31;
    r5 = 0xed;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xed;
    fn_80239EE8();
L_8024913C: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802491A0;
    r3 = r30;
    r4 = r31;
    r5 = 0xee;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xee;
    fn_80239EE8();
L_802491A0: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249204;
    r3 = r30;
    r4 = r31;
    r5 = 0xef;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xef;
    fn_80239EE8();
L_80249204: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249268;
    r3 = r30;
    r4 = r31;
    r5 = 0xf0;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf0;
    fn_80239EE8();
L_80249268: ;
    r3 = r31;
    r4 = r28;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802492CC;
    r3 = r30;
    r4 = r31;
    r5 = 0xf1;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf1;
    fn_80239EE8();
L_802492CC: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249330;
    r3 = r30;
    r4 = r31;
    r5 = 0xf2;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf2;
    fn_80239EE8();
L_80249330: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249394;
    r3 = r30;
    r4 = r31;
    r5 = 0xf3;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf3;
    fn_80239EE8();
L_80249394: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802493F8;
    r3 = r30;
    r4 = r31;
    r5 = 0xf4;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf4;
    fn_80239EE8();
L_802493F8: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80249448;
    r3 = r30;
    r4 = r31;
    r5 = 0xf5;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xf5;
    fn_80239EE8();
L_80249448: ;
    r3 = r30;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80249460 | Size: 0x218 (536 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80249460(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r5;
    r28 = r4;
    r27 = r3;
    r30 = r6;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xe7;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = 0x0;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe7;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024955C;
    r3 = r31;
    r4 = r27;
    r5 = 0xe8;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe8;
    fn_80239EE8();
L_8024955C: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802495C0;
    r3 = r31;
    r4 = r27;
    r5 = 0xe9;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe9;
    fn_80239EE8();
L_802495C0: ;
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802495F0;
    r3 = r27;
    r4 = r30;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249660;
L_802495F0: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xea;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r30 = (s32)r30 / (s32)r3;
    r3 = r31;
    r4 = r30;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xea;
    fn_80239CCC();
L_80249660: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80249678 | Size: 0x248 (584 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80249678(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r5;
    r28 = r4;
    r27 = r3;
    r30 = r6;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xe3;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = 0x0;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe3;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249774;
    r3 = r31;
    r4 = r27;
    r5 = 0xe4;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe4;
    fn_80239EE8();
L_80249774: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249800;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xe5;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe5;
    fn_80239CCC();
L_80249800: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80249838;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802498A8;
L_80249838: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xe6;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r30 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe6;
    fn_80239CCC();
L_802498A8: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x802498C0 | Size: 0x1F4 (500 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802498C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802358AC();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r3 = 0x0;
    r5 = 0xdf;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xdf;
    fn_80239EE8();
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80249984;
    r3 = r31;
    r4 = r27;
    r5 = 0xe0;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe0;
    fn_80239EE8();
L_80249984: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802499F4;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802499F4;
    r3 = r27;
    r4 = r30;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802499F4;
    r3 = r27;
    r4 = r30;
    r5 = 0x33;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249A3C;
L_802499F4: ;
    r3 = r31;
    r4 = r27;
    r5 = 0xe1;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe1;
    fn_80239EE8();
L_80249A3C: ;
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x4) goto L_80249A9C;
    r3 = r31;
    r4 = r27;
    r5 = 0xe2;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe2;
    fn_80239EE8();
L_80249A9C: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80249AB4 | Size: 0x278 (632 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80249AB4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_802358AC();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r5;
    r28 = r4;
    r27 = r3;
    r30 = r6;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xdb;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = 0x0;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xdb;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249BB0;
    r3 = r31;
    r4 = r27;
    r5 = 0xdc;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xdc;
    fn_80239EE8();
L_80249BB0: ;
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249C34;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xdd;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xdd;
    fn_80239CCC();
L_80249C34: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80249CA4;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80249CA4;
    r3 = r27;
    r4 = r30;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80249CA4;
    r3 = r27;
    r4 = r30;
    r5 = 0x33;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249D14;
L_80249CA4: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xde;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r30 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xde;
    fn_80239CCC();
L_80249D14: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80249D2C | Size: 0x25C (604 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80249D2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80235974();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r5;
    r28 = r4;
    r27 = r3;
    r30 = r6;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xd7;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = 0x0;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd7;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249E28;
    r3 = r31;
    r4 = r27;
    r5 = 0xd8;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd8;
    fn_80239EE8();
L_80249E28: ;
    r3 = r27;
    r4 = r30;
    fn_80235974();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249EAC;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xd9;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd9;
    fn_80239CCC();
L_80249EAC: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80249F00;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80249F00;
    r3 = r27;
    r4 = r30;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80249F70;
L_80249F00: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xda;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r30 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xda;
    fn_80239CCC();
L_80249F70: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80249F88 | Size: 0x1E8 (488 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80249F88(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_802359D8();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r27 = r6;
    r4 = r29;
    r3 = 0x0;
    r5 = 0xd3;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd3;
    fn_80239EE8();
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A060;
    r3 = r28;
    r4 = r29;
    r5 = 0xd4;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd4;
    fn_80239EE8();
L_8024A060: ;
    r3 = r29;
    r4 = r27;
    fn_802359D8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A0BC;
    r3 = r28;
    r4 = r29;
    r5 = 0xd5;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd5;
    fn_80239EE8();
L_8024A0BC: ;
    r3 = r29;
    r4 = r27;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A110;
    r3 = r29;
    r4 = r27;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A110;
    r3 = r29;
    r4 = r27;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A158;
L_8024A110: ;
    r3 = r28;
    r4 = r29;
    r5 = 0xd6;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r28 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd6;
    fn_80239EE8();
L_8024A158: ;
    r3 = r28;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024A170 | Size: 0x2B8 (696 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024A170(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80236D60();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r19, 0x4c(r1) */;
    r24 = r3;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r4 = r24;
    r5 = r1 + 0x28;
    r29 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fn_801F1C18();
    r30 = r3;
    r4 = r24;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r31 = r1 + 0x8;
    r23 = r3 & 0xFFFF;
    r28 = 0x0;
    goto L_8024A27C;
L_8024A1D8: ;
    /* clrlslwi r0, r28, 16, 2 */;
    r19 = *(u32*)(r31 + r0);
    if ((u32)r19 == (u32)0x0) goto L_8024A278;
    r21 = r1 + 0x28;
    r22 = r30 & 0xFFFF;
    r20 = 0x0;
    goto L_8024A26C;
L_8024A1F8: ;
    /* clrlslwi r0, r20, 16, 2 */;
    r5 = *(u32*)(r21 + r0);
    if ((u32)r5 == (u32)0x0) goto L_8024A268;
    r3 = r24;
    r4 = r19;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_8024A268;
    r3 = r29;
    r4 = r24;
    r5 = 0xcf;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xcf;
    fn_80239EE8();
    goto L_8024A278;
L_8024A268: ;
    r20 = r20 + 0x1;
L_8024A26C: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024A1F8;
L_8024A278: ;
    r28 = r28 + 0x1;
L_8024A27C: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_8024A1D8;
    if ((u32)r23 < (u32)0x2) goto L_8024A2D8;
    r3 = r29;
    r4 = r24;
    r5 = 0xd0;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd0;
    fn_80239EE8();
L_8024A2D8: ;
    r31 = r1 + 0x28;
    r28 = r30 & 0xFFFF;
    r21 = 0x0;
    goto L_8024A310;
L_8024A2E8: ;
    /* clrlslwi r0, r21, 16, 2 */;
    r5 = *(u32*)(r31 + r0);
    if ((u32)r5 == (u32)0x0) goto L_8024A30C;
    r3 = r24;
    r4 = r27;
    fn_80236D60();
    if ((s32)r3 < (s32)0x0) goto L_8024A31C;
L_8024A30C: ;
    r21 = r21 + 0x1;
L_8024A310: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8024A2E8;
L_8024A31C: ;
    r3 = r21 & 0xFFFF;
    r0 = r30 & 0xFFFF;
    if ((u32)r3 >= (u32)r0) goto L_8024A374;
    r3 = r29;
    r4 = r24;
    r5 = 0xd1;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd1;
    fn_80239EE8();
L_8024A374: ;
    r3 = r24;
    r4 = r27;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A3C8;
    r3 = r24;
    r4 = r27;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A3C8;
    r3 = r24;
    r4 = r27;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A410;
L_8024A3C8: ;
    r3 = r29;
    r4 = r24;
    r5 = 0xd2;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd2;
    fn_80239EE8();
L_8024A410: ;
    r3 = r29;
    /* lmw r19, 0x4c(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024A428 | Size: 0x23C (572 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024A428(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80235910();
    extern void fn_80236D60();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r18, 0x48(r1) */;
    r24 = r3;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r4 = r24;
    r5 = r1 + 0x28;
    r29 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fn_801F1C18();
    r30 = r3;
    r4 = r24;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r31 = r1 + 0x8;
    r23 = r3 & 0xFFFF;
    r28 = 0x0;
    goto L_8024A548;
L_8024A490: ;
    /* clrlslwi r0, r28, 16, 2 */;
    r19 = *(u32*)(r31 + r0);
    if ((u32)r19 == (u32)0x0) goto L_8024A544;
    r21 = r1 + 0x28;
    r22 = r30 & 0xFFFF;
    r18 = 0x0;
    r20 = 0x0;
    goto L_8024A52C;
L_8024A4B4: ;
    /* clrlslwi r0, r20, 16, 2 */;
    r5 = *(u32*)(r21 + r0);
    if ((u32)r5 == (u32)0x0) goto L_8024A528;
    r3 = r24;
    r4 = r19;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_8024A528;
    r3 = r29;
    r4 = r24;
    r5 = 0xcc;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xcc;
    fn_80239EE8();
    r18 = 0x1;
    goto L_8024A538;
L_8024A528: ;
    r20 = r20 + 0x1;
L_8024A52C: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024A4B4;
L_8024A538: ;
    r0 = r18 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A554;
L_8024A544: ;
    r28 = r28 + 0x1;
L_8024A548: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_8024A490;
L_8024A554: ;
    r3 = r24;
    r4 = r27;
    fn_80235910();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)r23) goto L_8024A5B0;
    r3 = r29;
    r4 = r24;
    r5 = 0xcd;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xcd;
    fn_80239EE8();
L_8024A5B0: ;
    r3 = r24;
    r4 = r27;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A604;
    r3 = r24;
    r4 = r27;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A604;
    r3 = r24;
    r4 = r27;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A64C;
L_8024A604: ;
    r3 = r29;
    r4 = r24;
    r5 = 0xce;
    fn_80239984();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r24;
    r8 = r26;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xce;
    fn_80239EE8();
L_8024A64C: ;
    r3 = r29;
    /* lmw r18, 0x48(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024A664 | Size: 0x2C0 (704 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024A664(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80235910();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r5;
    r28 = r4;
    r27 = r3;
    r30 = r6;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xc7;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = 0x0;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc7;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A760;
    r3 = r31;
    r4 = r27;
    r5 = 0xc8;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc8;
    fn_80239EE8();
L_8024A760: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A7C4;
    r3 = r31;
    r4 = r27;
    r5 = 0xc9;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc9;
    fn_80239EE8();
L_8024A7C4: ;
    r3 = r27;
    r4 = r30;
    fn_80235910();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A848;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xca;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xca;
    fn_80239CCC();
L_8024A848: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A89C;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024A89C;
    r3 = r27;
    r4 = r30;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024A90C;
L_8024A89C: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xcb;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r30 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xcb;
    fn_80239CCC();
L_8024A90C: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024A924 | Size: 0x25C (604 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024A924(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80235A3C();
    extern void fn_80237F74();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r5;
    r28 = r4;
    r27 = r3;
    r30 = r6;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xc3;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = 0x0;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc3;
    fn_80239CCC();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024AA20;
    r3 = r31;
    r4 = r27;
    r5 = 0xc4;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc4;
    fn_80239EE8();
L_8024AA20: ;
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024AAA4;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xc5;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc5;
    fn_80239CCC();
L_8024AAA4: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024AAF8;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024AAF8;
    r3 = r27;
    r4 = r30;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024AB68;
L_8024AAF8: ;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r30 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xc6;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r30 / (s32)r3;
    r3 = r31;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc6;
    fn_80239CCC();
L_8024AB68: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024AB80 | Size: 0x204 (516 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024AB80(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80235AA0();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r3 = 0x0;
    r5 = 0xbf;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbf;
    fn_80239EE8();
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024AC58;
    r3 = r31;
    r4 = r27;
    r5 = 0xc0;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc0;
    fn_80239EE8();
L_8024AC58: ;
    r3 = r27;
    r4 = r30;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024ACB4;
    r3 = r31;
    r4 = r27;
    r5 = 0xc1;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc1;
    fn_80239EE8();
L_8024ACB4: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024AD24;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024AD24;
    r3 = r27;
    r4 = r30;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024AD24;
    r3 = r27;
    r4 = r30;
    r5 = 0x34;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024AD6C;
L_8024AD24: ;
    r3 = r31;
    r4 = r27;
    r5 = 0xc2;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc2;
    fn_80239EE8();
L_8024AD6C: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024AD84 | Size: 0x16C (364 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024AD84(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r30 = r6;
    r3 = r28;
    r31 = 0x0;
    r4 = 0x0;
    r5 = 0xed;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_8024AE0C;
    r4 = r27;
    r3 = 0x0;
    r5 = 0xbd;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbd;
    fn_80239EE8();
L_8024AE0C: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x27;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024AE74;
    r3 = r31;
    r4 = r27;
    r5 = 0xbe;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbe;
    fn_80239EE8();
    goto L_8024AED8;
L_8024AE74: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024AED8;
    r3 = r31;
    r4 = r27;
    r5 = 0xbe;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbe;
    fn_80239EE8();
L_8024AED8: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024AEF0 | Size: 0xD4 (212 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024AEF0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236D60();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r5 = r6;
    r31 = 0x0;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_8024AF68;
    r4 = r28;
    r3 = 0x0;
    r5 = 0xbb;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbb;
    fn_80239EE8();
L_8024AF68: ;
    r3 = r31;
    r4 = r28;
    r5 = 0xbc;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbc;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024AFC4 | Size: 0x4B0 (1200 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024AFC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80236D60();
    extern void fn_80239564();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r30 = r6;
    r29 = r5;
    r27 = r3;
    r28 = r4;
    r5 = r30;
    r31 = 0x0;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_8024B3EC;
    r3 = r27;
    r4 = r29;
    fn_80239564();
    r26 = r3 & 0xFF;
    r3 = 0x0;
    r4 = 0xb0;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = (s32)r26 / (s32)r3;
    r3 = 0x0;
    r4 = r26;
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    /* subi r3, r6, 0x139c */;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb0;
    fn_80239CCC();
    r3 = r27;
    r4 = r30;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B0CC;
    r3 = r31;
    r4 = r27;
    r5 = 0xb1;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb1;
    fn_80239EE8();
L_8024B0CC: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B130;
    r3 = r31;
    r4 = r27;
    r5 = 0xb2;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb2;
    fn_80239EE8();
L_8024B130: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B194;
    r3 = r31;
    r4 = r27;
    r5 = 0xb3;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb3;
    fn_80239EE8();
L_8024B194: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B1F8;
    r3 = r31;
    r4 = r27;
    r5 = 0xb4;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb4;
    fn_80239EE8();
L_8024B1F8: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B25C;
    r3 = r31;
    r4 = r27;
    r5 = 0xb5;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb5;
    fn_80239EE8();
L_8024B25C: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B2C0;
    r3 = r31;
    r4 = r27;
    r5 = 0xb6;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb6;
    fn_80239EE8();
L_8024B2C0: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B324;
    r3 = r31;
    r4 = r27;
    r5 = 0xb7;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb7;
    fn_80239EE8();
L_8024B324: ;
    r3 = r27;
    r4 = r30;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B388;
    r3 = r31;
    r4 = r27;
    r5 = 0xb8;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb8;
    fn_80239EE8();
L_8024B388: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B3EC;
    r3 = r31;
    r4 = r27;
    r5 = 0xb9;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xb9;
    fn_80239EE8();
L_8024B3EC: ;
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B45C;
    r3 = r31;
    r4 = r27;
    r5 = 0xba;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xba;
    fn_80239EE8();
L_8024B45C: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024B474 | Size: 0x5D0 (1488 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024B474(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fn_801F1C18();
    extern void fn_80205B8C();
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
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0xc8(r1) */;
    r29 = r4;
    r28 = r5;
    r30 = r3;
    r31 = 0x0;
    r5 = r29;
    r4 = r28;
    fn_802395C8();
    r24 = r3;
    r4 = r30;
    r5 = r1 + 0x5c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r4 = r30;
    r5 = r1 + 0x3c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1C18();
    r27 = r3;
    r4 = r30;
    r5 = r1 + 0x1c;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r25 = r1 + 0x3c;
    r22 = r27 & 0xFFFF;
    r26 = 0x0;
    goto L_8024B588;
L_8024B500: ;
    /* clrlslwi r23, r26, 16, 2 */;
    r0 = *(u32*)(r25 + r23);
    if ((u32)r29 == (u32)r0) goto L_8024B584;
    r3 = r30;
    r4 = r28;
    fn_80239500();
    r4 = *(u32*)(r25 + r23);
    r6 = r3;
    r3 = r30;
    r5 = r24;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8024B584;
    r3 = r31;
    r4 = r30;
    r5 = 0xa9;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa9;
    fn_80239EE8();
L_8024B584: ;
    r26 = r26 + 0x1;
L_8024B588: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024B500;
    r25 = r1 + 0x3c;
    r22 = r27 & 0xFFFF;
    r26 = 0x0;
    goto L_8024B62C;
L_8024B5A4: ;
    /* clrlslwi r23, r26, 16, 2 */;
    r0 = *(u32*)(r25 + r23);
    if ((u32)r29 == (u32)r0) goto L_8024B628;
    r3 = r30;
    r4 = r28;
    fn_80239500();
    r4 = *(u32*)(r25 + r23);
    r6 = r3;
    r3 = r30;
    r5 = r24;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x42) goto L_8024B628;
    r3 = r31;
    r4 = r30;
    r5 = 0xaa;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xaa;
    fn_80239EE8();
L_8024B628: ;
    r26 = r26 + 0x1;
L_8024B62C: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024B5A4;
    r25 = r1 + 0x3c;
    r22 = r27 & 0xFFFF;
    r24 = 0x0;
    goto L_8024B714;
L_8024B648: ;
    /* clrlslwi r0, r24, 16, 2 */;
    r4 = *(u32*)(r25 + r0);
    if ((u32)r29 == (u32)r4) goto L_8024B710;
    r3 = r30;
    r5 = r1 + 0x8;
    r6 = 0x0;
    r7 = 0x1;
    fn_802367CC();
    r4 = r3 & 0xFFFF;
    r26 = r3;
    if ((u32)r29 == (u32)r4) goto L_8024B710;
    r3 = r1 + 0x8;
    r23 = 0x0;
    goto L_8024B6F4;
L_8024B684: ;
    /* clrlslwi r0, r23, 16, 1 */;
    r0 = *(u16*)(r3 + r0);
    if ((u32)r0 == (u32)0xb6) goto L_8024B6A4;
    if ((u32)r0 == (u32)0xc5) goto L_8024B6A4;
    if ((u32)r0 != (u32)0xcb) goto L_8024B6F0;
L_8024B6A4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0xab;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xab;
    fn_80239EE8();
    goto L_8024B700;
L_8024B6F0: ;
    r23 = r23 + 0x1;
L_8024B6F4: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r4) goto L_8024B684;
L_8024B700: ;
    r3 = r23 & 0xFFFF;
    r0 = r26 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8024B720;
L_8024B710: ;
    r24 = r24 + 0x1;
L_8024B714: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024B648;
L_8024B720: ;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xb6;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B794;
    r3 = r31;
    r4 = r30;
    r5 = 0xac;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xac;
    fn_80239EE8();
    goto L_8024B878;
L_8024B794: ;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xc5;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B808;
    r3 = r31;
    r4 = r30;
    r5 = 0xac;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xac;
    fn_80239EE8();
    goto L_8024B878;
L_8024B808: ;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xcb;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B878;
    r3 = r31;
    r4 = r30;
    r5 = 0xac;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xac;
    fn_80239EE8();
L_8024B878: ;
    r4 = r30;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024B8E8;
    r3 = r31;
    r4 = r30;
    r5 = 0xad;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xad;
    fn_80239EE8();
L_8024B8E8: ;
    r24 = r1 + 0x3c;
    r23 = r27 & 0xFFFF;
    r22 = 0x0;
    goto L_8024B978;
L_8024B8F8: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r4 = *(u32*)(r24 + r0);
    if ((u32)r29 == (u32)r4) goto L_8024B974;
    r3 = r30;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xb6) goto L_8024B92C;
    if ((u32)r0 == (u32)0xc5) goto L_8024B92C;
    if ((u32)r0 != (u32)0xcb) goto L_8024B974;
L_8024B92C: ;
    r3 = r31;
    r4 = r30;
    r5 = 0xae;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xae;
    fn_80239EE8();
L_8024B974: ;
    r22 = r22 + 0x1;
L_8024B978: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_8024B8F8;
    r22 = r1 + 0x3c;
    r23 = r27 & 0xFFFF;
    r25 = 0x0;
    goto L_8024BA20;
L_8024B994: ;
    /* clrlslwi r24, r25, 16, 2 */;
    r4 = *(u32*)(r22 + r24);
    if ((u32)r29 == (u32)r4) goto L_8024BA1C;
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
    if ((s32)r0 > (s32)r3) goto L_8024BA1C;
    r3 = r31;
    r4 = r30;
    r5 = 0xaf;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xaf;
    fn_80239EE8();
L_8024BA1C: ;
    r25 = r25 + 0x1;
L_8024BA20: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_8024B994;
    r3 = r31;
    /* lmw r22, 0xc8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024BA44 | Size: 0x438 (1080 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024BA44(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F4354();
    extern void fn_801F8A18();
    extern void fn_80205B8C();
    extern void fn_80235BE4();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r6;
    r27 = r4;
    r28 = r5;
    r26 = r3;
    r5 = r29;
    r31 = 0x0;
    r4 = 0x0;
    r6 = 0x0;
    fn_80235BE4();
    r30 = r3;
    r4 = r29;
    r3 = 0x0;
    fn_801F4354();
    r0 = 0x0;
    r4 = r1 + 0x8;
    *(u16*)(sp + 0x8) = r0;
    fn_801F8A18();
    if ((u32)r3 != (u32)0x0) goto L_8024BAA4;
    r30 = 0x1;
L_8024BAA4: ;
    r0 = r30 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8024BAF4;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x9f;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x9f;
    fn_80239EE8();
L_8024BAF4: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BB58;
    r3 = r31;
    r4 = r26;
    r5 = 0xa0;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa0;
    fn_80239EE8();
L_8024BB58: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BBBC;
    r3 = r31;
    r4 = r26;
    r5 = 0xa1;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa1;
    fn_80239EE8();
L_8024BBBC: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BC20;
    r3 = r31;
    r4 = r26;
    r5 = 0xa2;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa2;
    fn_80239EE8();
L_8024BC20: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BC84;
    r3 = r31;
    r4 = r26;
    r5 = 0xa4;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa4;
    fn_80239EE8();
L_8024BC84: ;
    r3 = r26;
    r4 = r29;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BCE8;
    r3 = r31;
    r4 = r26;
    r5 = 0xa5;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa5;
    fn_80239EE8();
L_8024BCE8: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BD4C;
    r3 = r31;
    r4 = r26;
    r5 = 0xa6;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa6;
    fn_80239EE8();
L_8024BD4C: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BDB0;
    r3 = r31;
    r4 = r26;
    r5 = 0xa7;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa7;
    fn_80239EE8();
L_8024BDB0: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BE14;
    r3 = r31;
    r4 = r26;
    r5 = 0xa3;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa3;
    fn_80239EE8();
L_8024BE14: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024BE64;
    r3 = r31;
    r4 = r26;
    r5 = 0xa8;
    fn_80239984();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xa8;
    fn_80239EE8();
L_8024BE64: ;
    r3 = r31;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024BE7C | Size: 0x144 (324 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024BE7C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_802387C8();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r27 = r5;
    r31 = r3;
    r26 = r4;
    r29 = 0x0;
    r5 = 0x1a;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024BEF8;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x9c;
    fn_80239984();
    r0 = r3;
    r3 = r26;
    r29 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x9c;
    fn_80239EE8();
L_8024BEF8: ;
    r3 = r31;
    r4 = r26;
    fn_802387C8();
    r30 = r3;
    r3 = 0x0;
    r4 = 0x9d;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r28 = (s32)r30 / (s32)r3;
    r3 = r29;
    r4 = r28;
    fn_802399FC();
    r30 = r3;
    r3 = r26;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    /* subi r3, r6, 0x139c */;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x9d;
    fn_80239CCC();
    r3 = r30;
    r4 = r31;
    r5 = 0x9e;
    fn_80239984();
    r30 = r3;
    r3 = r26;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x9e;
    fn_80239EE8();
    r3 = r30;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024BFC0 | Size: 0x5FC (1532 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024BFC0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801F1A6C();
    extern void fn_801F1C18();
    extern void fn_80205B8C();
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
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r21, 0xc4(r1) */;
    r29 = r4;
    r28 = r5;
    r30 = r3;
    r31 = 0x0;
    r5 = r29;
    r4 = r28;
    fn_802395C8();
    r25 = r3;
    r4 = r30;
    r5 = r1 + 0x5c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r4 = r30;
    r5 = r1 + 0x3c;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1C18();
    r27 = r3;
    r4 = r30;
    r5 = r1 + 0x1c;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r26 = r3;
    r24 = r1 + 0x3c;
    r21 = r27 & 0xFFFF;
    r23 = 0x0;
    goto L_8024C0D8;
L_8024C050: ;
    /* clrlslwi r22, r23, 16, 2 */;
    r0 = *(u32*)(r24 + r22);
    if ((u32)r29 == (u32)r0) goto L_8024C0D4;
    r3 = r30;
    r4 = r28;
    fn_80239500();
    r4 = *(u32*)(r24 + r22);
    r6 = r3;
    r3 = r30;
    r5 = r25;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8024C0D4;
    r3 = r31;
    r4 = r30;
    r5 = 0x94;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x94;
    fn_80239EE8();
L_8024C0D4: ;
    r23 = r23 + 0x1;
L_8024C0D8: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r21) goto L_8024C050;
    r24 = r1 + 0x3c;
    r21 = r27 & 0xFFFF;
    r23 = 0x0;
    goto L_8024C17C;
L_8024C0F4: ;
    /* clrlslwi r22, r23, 16, 2 */;
    r0 = *(u32*)(r24 + r22);
    if ((u32)r29 == (u32)r0) goto L_8024C178;
    r3 = r30;
    r4 = r28;
    fn_80239500();
    r4 = *(u32*)(r24 + r22);
    r6 = r3;
    r3 = r30;
    r5 = r25;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x42) goto L_8024C178;
    r3 = r31;
    r4 = r30;
    r5 = 0x95;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x95;
    fn_80239EE8();
L_8024C178: ;
    r23 = r23 + 0x1;
L_8024C17C: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r21) goto L_8024C0F4;
    r24 = r1 + 0x3c;
    r21 = r27 & 0xFFFF;
    r23 = 0x0;
    goto L_8024C264;
L_8024C198: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r4 = *(u32*)(r24 + r0);
    if ((u32)r29 == (u32)r4) goto L_8024C260;
    r3 = r30;
    r5 = r1 + 0x8;
    r6 = 0x0;
    r7 = 0x1;
    fn_802367CC();
    r4 = r3 & 0xFFFF;
    r25 = r3;
    if ((u32)r29 == (u32)r4) goto L_8024C260;
    r3 = r1 + 0x8;
    r22 = 0x0;
    goto L_8024C244;
L_8024C1D4: ;
    /* clrlslwi r0, r22, 16, 1 */;
    r0 = *(u16*)(r3 + r0);
    if ((u32)r0 == (u32)0xb6) goto L_8024C1F4;
    if ((u32)r0 == (u32)0xc5) goto L_8024C1F4;
    if ((u32)r0 != (u32)0xcb) goto L_8024C240;
L_8024C1F4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x96;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x96;
    fn_80239EE8();
    goto L_8024C250;
L_8024C240: ;
    r22 = r22 + 0x1;
L_8024C244: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r4) goto L_8024C1D4;
L_8024C250: ;
    r3 = r22 & 0xFFFF;
    r0 = r25 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8024C270;
L_8024C260: ;
    r23 = r23 + 0x1;
L_8024C264: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r21) goto L_8024C198;
L_8024C270: ;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xb6;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C2E4;
    r3 = r31;
    r4 = r30;
    r5 = 0x97;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x97;
    fn_80239EE8();
    goto L_8024C3C8;
L_8024C2E4: ;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xc5;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C358;
    r3 = r31;
    r4 = r30;
    r5 = 0x97;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x97;
    fn_80239EE8();
    goto L_8024C3C8;
L_8024C358: ;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0xcb;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C3C8;
    r3 = r31;
    r4 = r30;
    r5 = 0x97;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x97;
    fn_80239EE8();
L_8024C3C8: ;
    r25 = r1 + 0x1c;
    r22 = r26 & 0xFFFF;
    r24 = 0x0;
    goto L_8024C454;
L_8024C3D8: ;
    /* clrlslwi r0, r24, 16, 2 */;
    r3 = r30;
    r4 = *(u32*)(r25 + r0);
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C450;
    r0 = r24 & 0xFFFF;
    r3 = r31;
    /* subic r0, r0, 0x1 */;
    r4 = r30;
    r5 = r0 - r0; /* -borrow */;
    r23 = r5 + 0x99;
    r5 = r23;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r10 = r23;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    fn_80239EE8();
L_8024C450: ;
    r24 = r24 + 0x1;
L_8024C454: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024C3D8;
    r23 = r1 + 0x3c;
    r22 = r27 & 0xFFFF;
    r21 = 0x0;
    goto L_8024C4F0;
L_8024C470: ;
    /* clrlslwi r0, r21, 16, 2 */;
    r4 = *(u32*)(r23 + r0);
    if ((u32)r29 == (u32)r4) goto L_8024C4EC;
    r3 = r30;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xb6) goto L_8024C4A4;
    if ((u32)r0 == (u32)0xc5) goto L_8024C4A4;
    if ((u32)r0 != (u32)0xcb) goto L_8024C4EC;
L_8024C4A4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x9a;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x9a;
    fn_80239EE8();
L_8024C4EC: ;
    r21 = r21 + 0x1;
L_8024C4F0: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024C470;
    r21 = r1 + 0x3c;
    r22 = r27 & 0xFFFF;
    r24 = 0x0;
    goto L_8024C598;
L_8024C50C: ;
    /* clrlslwi r23, r24, 16, 2 */;
    r4 = *(u32*)(r21 + r23);
    if ((u32)r29 == (u32)r4) goto L_8024C594;
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
    if ((s32)r0 > (s32)r3) goto L_8024C594;
    r3 = r31;
    r4 = r30;
    r5 = 0x9b;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x9b;
    fn_80239EE8();
L_8024C594: ;
    r24 = r24 + 0x1;
L_8024C598: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024C50C;
    r3 = r31;
    /* lmw r21, 0xc4(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024C5BC | Size: 0x91C (2332 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024C5BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_80236D60();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fn_8025C5A4();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r21, 0x44(r1) */;
    r31 = r3;
    r30 = r4;
    r29 = r5;
    r27 = r6;
    r4 = r31;
    r5 = r1 + 0x1c;
    r26 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1C18();
    r28 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C660;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x7e;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x7e;
    fn_80239EE8();
L_8024C660: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C6C4;
    r3 = r26;
    r4 = r31;
    r5 = 0x7f;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x7f;
    fn_80239EE8();
L_8024C6C4: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C728;
    r3 = r26;
    r4 = r31;
    r5 = 0x80;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x80;
    fn_80239EE8();
L_8024C728: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C78C;
    r3 = r26;
    r4 = r31;
    r5 = 0x81;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x81;
    fn_80239EE8();
L_8024C78C: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C7F0;
    r3 = r26;
    r4 = r31;
    r5 = 0x82;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x82;
    fn_80239EE8();
L_8024C7F0: ;
    r3 = r31;
    r4 = r27;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C854;
    r3 = r26;
    r4 = r31;
    r5 = 0x83;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x83;
    fn_80239EE8();
L_8024C854: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C8B8;
    r3 = r26;
    r4 = r31;
    r5 = 0x84;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x84;
    fn_80239EE8();
L_8024C8B8: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C91C;
    r3 = r26;
    r4 = r31;
    r5 = 0x85;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x85;
    fn_80239EE8();
L_8024C91C: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024C980;
    r3 = r26;
    r4 = r31;
    r5 = 0x86;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x86;
    fn_80239EE8();
L_8024C980: ;
    r3 = r31;
    r4 = r30;
    r5 = r27;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_8024C9E0;
    r3 = r26;
    r4 = r31;
    r5 = 0x87;
    fn_80239984();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x87;
    fn_80239EE8();
L_8024C9E0: ;
    r3 = r26;
    r4 = r31;
    r5 = 0x88;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
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
    if ((u32)r0 != (u32)0x1) goto L_8024CA84;
    r3 = r27;
    r4 = r31;
    r5 = 0x89;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x89;
    fn_80239EE8();
L_8024CA84: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CAE4;
    r3 = r27;
    r4 = r31;
    r5 = 0x8a;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x8a;
    fn_80239EE8();
L_8024CAE4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CB44;
    r3 = r27;
    r4 = r31;
    r5 = 0x8b;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x8b;
    fn_80239EE8();
L_8024CB44: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CBA4;
    r3 = r27;
    r4 = r31;
    r5 = 0x8c;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x8c;
    fn_80239EE8();
L_8024CBA4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CC04;
    r3 = r27;
    r4 = r31;
    r5 = 0x8d;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x8d;
    fn_80239EE8();
L_8024CC04: ;
    r3 = r31;
    r4 = r30;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CC64;
    r3 = r27;
    r4 = r31;
    r5 = 0x8e;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x8e;
    fn_80239EE8();
L_8024CC64: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CCC4;
    r3 = r27;
    r4 = r31;
    r5 = 0x8f;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x8f;
    fn_80239EE8();
L_8024CCC4: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CD24;
    r3 = r27;
    r4 = r31;
    r5 = 0x90;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x90;
    fn_80239EE8();
L_8024CD24: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CD84;
    r3 = r27;
    r4 = r31;
    r5 = 0x91;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x91;
    fn_80239EE8();
L_8024CD84: ;
    r26 = r1 + 0x1c;
    r25 = r28 & 0xFFFF;
    r28 = 0x0;
    goto L_8024CE54;
L_8024CD94: ;
    /* clrlslwi r0, r28, 16, 2 */;
    r3 = r31;
    r4 = *(u32*)(r26 + r0);
    r5 = r1 + 0x8;
    r6 = 0x0;
    r7 = 0x0;
    fn_802367CC();
    r23 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_8024CE50;
    r24 = r1 + 0x8;
    r21 = 0x0;
    r22 = 0x0;
    goto L_8024CE38;
L_8024CDC8: ;
    /* clrlslwi r0, r22, 16, 1 */;
    r3 = r31;
    r5 = *(u16*)(r24 + r0);
    r4 = 0x1f;
    fn_8025C5A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CE34;
    r3 = r27;
    r4 = r31;
    r5 = 0x92;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x92;
    fn_80239EE8();
    r21 = 0x1;
    goto L_8024CE44;
L_8024CE34: ;
    r22 = r22 + 0x1;
L_8024CE38: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r23) goto L_8024CDC8;
L_8024CE44: ;
    r0 = r21 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024CE60;
L_8024CE50: ;
    r28 = r28 + 0x1;
L_8024CE54: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_8024CD94;
L_8024CE60: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CEC0;
    r3 = r27;
    r4 = r31;
    r5 = 0x93;
    fn_80239984();
    r27 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x93;
    fn_80239EE8();
L_8024CEC0: ;
    r3 = r27;
    /* lmw r21, 0x44(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024CED8 | Size: 0x940 (2368 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024CED8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_80236D60();
    extern void fn_80239984();
    extern void fn_80239EE8();
    extern void fn_8025C5A4();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r20, 0x40(r1) */;
    r31 = r3;
    r29 = r4;
    r28 = r5;
    r25 = r6;
    r4 = r31;
    r5 = r1 + 0x1c;
    r30 = 0x0;
    r27 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1C18();
    r26 = r3;
    r3 = r31;
    r4 = r25;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CF80;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x69;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x69;
    fn_80239EE8();
L_8024CF80: ;
    r3 = r31;
    r4 = r25;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024CFE4;
    r3 = r30;
    r4 = r31;
    r5 = 0x6a;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x6a;
    fn_80239EE8();
L_8024CFE4: ;
    r3 = r31;
    r4 = r25;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D048;
    r3 = r30;
    r4 = r31;
    r5 = 0x6b;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x6b;
    fn_80239EE8();
L_8024D048: ;
    r3 = r31;
    r4 = r25;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D0AC;
    r3 = r30;
    r4 = r31;
    r5 = 0x6c;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x6c;
    fn_80239EE8();
L_8024D0AC: ;
    r3 = r31;
    r4 = r25;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D110;
    r3 = r30;
    r4 = r31;
    r5 = 0x6d;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x6d;
    fn_80239EE8();
L_8024D110: ;
    r3 = r31;
    r4 = r25;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D174;
    r3 = r30;
    r4 = r31;
    r5 = 0x6e;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x6e;
    fn_80239EE8();
L_8024D174: ;
    r3 = r31;
    r4 = r25;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D1D8;
    r3 = r30;
    r4 = r31;
    r5 = 0x6f;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x6f;
    fn_80239EE8();
L_8024D1D8: ;
    r3 = r31;
    r4 = r25;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D23C;
    r3 = r30;
    r4 = r31;
    r5 = 0x70;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x70;
    fn_80239EE8();
L_8024D23C: ;
    r3 = r31;
    r4 = r25;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D2A0;
    r3 = r30;
    r4 = r31;
    r5 = 0x71;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x71;
    fn_80239EE8();
L_8024D2A0: ;
    r3 = r31;
    r4 = r29;
    r5 = r25;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) goto L_8024D300;
    r3 = r30;
    r4 = r31;
    r5 = 0x72;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x72;
    fn_80239EE8();
L_8024D300: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D364;
    r3 = r30;
    r4 = r31;
    r5 = 0x73;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x73;
    fn_80239EE8();
L_8024D364: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D3C8;
    r3 = r30;
    r4 = r31;
    r5 = 0x74;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x74;
    fn_80239EE8();
L_8024D3C8: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D42C;
    r3 = r30;
    r4 = r31;
    r5 = 0x75;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x75;
    fn_80239EE8();
L_8024D42C: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D490;
    r3 = r30;
    r4 = r31;
    r5 = 0x76;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x76;
    fn_80239EE8();
L_8024D490: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D4F4;
    r3 = r30;
    r4 = r31;
    r5 = 0x77;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x77;
    fn_80239EE8();
L_8024D4F4: ;
    r3 = r31;
    r4 = r29;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D558;
    r3 = r30;
    r4 = r31;
    r5 = 0x78;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x78;
    fn_80239EE8();
L_8024D558: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D5BC;
    r3 = r30;
    r4 = r31;
    r5 = 0x79;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x79;
    fn_80239EE8();
L_8024D5BC: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D620;
    r3 = r30;
    r4 = r31;
    r5 = 0x7a;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x7a;
    fn_80239EE8();
L_8024D620: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D684;
    r3 = r30;
    r4 = r31;
    r5 = 0x7b;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x7b;
    fn_80239EE8();
L_8024D684: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)0x13) goto L_8024D698;
    if ((u32)r0 != (u32)0x154) goto L_8024D6A0;
L_8024D698: ;
    r27 = 0x1f;
    goto L_8024D6BC;
L_8024D6A0: ;
    if ((u32)r0 != (u32)0x5b) goto L_8024D6B0;
    r27 = 0x20;
    goto L_8024D6BC;
L_8024D6B0: ;
    if ((u32)r0 != (u32)0x123) goto L_8024D6BC;
    r27 = 0x21;
L_8024D6BC: ;
    r25 = r1 + 0x1c;
    r24 = r26 & 0xFFFF;
    r26 = 0x0;
    goto L_8024D790;
L_8024D6CC: ;
    /* clrlslwi r0, r26, 16, 2 */;
    r3 = r31;
    r4 = *(u32*)(r25 + r0);
    r5 = r1 + 0x8;
    r6 = 0x0;
    r7 = 0x0;
    fn_802367CC();
    r22 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x123) goto L_8024D78C;
    r23 = r1 + 0x8;
    r20 = 0x0;
    r21 = 0x0;
    goto L_8024D774;
L_8024D700: ;
    /* clrlslwi r0, r21, 16, 1 */;
    r3 = r31;
    r5 = *(u16*)(r23 + r0);
    r4 = r27;
    fn_8025C5A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D770;
    r3 = r30;
    r4 = r31;
    r5 = 0x7c;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x7c;
    fn_80239EE8();
    r20 = 0x1;
    goto L_8024D780;
L_8024D770: ;
    r21 = r21 + 0x1;
L_8024D774: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r22) goto L_8024D700;
L_8024D780: ;
    r0 = r20 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024D79C;
L_8024D78C: ;
    r26 = r26 + 0x1;
L_8024D790: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_8024D6CC;
L_8024D79C: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D800;
    r3 = r30;
    r4 = r31;
    r5 = 0x7d;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x7d;
    fn_80239EE8();
L_8024D800: ;
    r3 = r30;
    /* lmw r20, 0x40(r1) */;
    return;
}
#pragma pop


/* -------------------------------------------------------------------
 * Experience & Level Processing (0x8024D000-0x80254000)
 * 136 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8024D818 | Size: 0x140 (320 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024D818(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80237F74();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = 0x0;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024D894;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x66;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x66;
    fn_80239EE8();
L_8024D894: ;
    r3 = r28;
    r4 = r29;
    fn_8023831C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x8) goto L_8024D8B4;
    if ((u32)r0 != (u32)0x9) goto L_8024D8FC;
L_8024D8B4: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x67;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x67;
    fn_80239EE8();
L_8024D8FC: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x68;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x68;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024D958 | Size: 0x1A4 (420 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024D958(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_802376EC();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    extern void fn_80239EE8();
    extern void fn_8023C370();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x18(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    fn_802376EC();
    r26 = r3;
    r3 = r28;
    r4 = r29;
    r5 = r30;
    r6 = r31;
    r7 = 0x1;
    fn_8023C370();
    r27 = r3;
    r3 = 0x0;
    r4 = 0x63;
    r5 = 0x3e;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = (s32)r27 >> 1;
    r0 = r26 & 0xFFFF;
    /* addze r4, r4 */;
    r4 = r4 * 0x64;
    r0 = (s32)r4 / (s32)r0;
    r27 = (s32)r0 / (s32)r3;
    r3 = 0x0;
    r4 = r27;
    fn_802399FC();
    r0 = r3;
    r3 = r29;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    /* subi r3, r6, 0x139c */;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x63;
    fn_80239CCC();
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024DA80;
    r3 = r26;
    r4 = r28;
    r5 = 0x64;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x64;
    fn_80239EE8();
L_8024DA80: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x40;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024DAE4;
    r3 = r26;
    r4 = r28;
    r5 = 0x65;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r26 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x65;
    fn_80239EE8();
L_8024DAE4: ;
    r3 = r26;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024DAFC | Size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024DAFC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r28;
    r5 = 0x61;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x61;
    fn_80239EE8();
    r3 = r31;
    r4 = r28;
    r5 = 0x62;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x62;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024DBBC | Size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024DBBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r28;
    r5 = 0x5f;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5f;
    fn_80239EE8();
    r3 = r31;
    r4 = r28;
    r5 = 0x60;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x60;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024DC7C | Size: 0x210 (528 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024DC7C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r6 = 0x0;
    r7 = 0x1;
    /* stmw r24, 0x30(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r5 = r1 + 0x8;
    r4 = r27;
    r30 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x117) goto L_8024DD18;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x5b;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5b;
    fn_80239EE8();
L_8024DD18: ;
    r25 = r1 + 0x8;
    r26 = r31 & 0xFFFF;
    r24 = 0x0;
    goto L_8024DD98;
L_8024DD28: ;
    /* clrlslwi r0, r24, 16, 2 */;
    r3 = r27;
    r4 = *(u32*)(r25 + r0);
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024DD94;
    r3 = r30;
    r4 = r27;
    r5 = 0x5c;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5c;
    fn_80239EE8();
    goto L_8024DDA4;
L_8024DD94: ;
    r24 = r24 + 0x1;
L_8024DD98: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_8024DD28;
L_8024DDA4: ;
    r26 = r1 + 0x8;
    r31 = r31 & 0xFFFF;
    r25 = 0x0;
    goto L_8024DE24;
L_8024DDB4: ;
    /* clrlslwi r0, r25, 16, 2 */;
    r3 = r27;
    r4 = *(u32*)(r26 + r0);
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024DE20;
    r3 = r30;
    r4 = r27;
    r5 = 0x5d;
    fn_80239984();
    r0 = r3;
    r3 = r28;
    r30 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5d;
    fn_80239EE8();
    goto L_8024DE30;
L_8024DE20: ;
    r25 = r25 + 0x1;
L_8024DE24: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8024DDB4;
L_8024DE30: ;
    r3 = r30;
    r4 = r27;
    r5 = 0x5e;
    fn_80239984();
    r25 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5e;
    fn_80239EE8();
    r3 = r25;
    /* lmw r24, 0x30(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024DE8C | Size: 0x138 (312 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024DE8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_802358AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x6) goto L_8024DF08;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x58;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x58;
    fn_80239EE8();
L_8024DF08: ;
    r3 = r28;
    r4 = r29;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 >= (u32)0x6) goto L_8024DF68;
    r3 = r31;
    r4 = r28;
    r5 = 0x59;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x59;
    fn_80239EE8();
L_8024DF68: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x5a;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5a;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024DFC4 | Size: 0x108 (264 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024DFC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    r4 = r3;
    if ((s32)r0 == (s32)0) goto L_8024E070;
    if ((u32)r0 == (u32)0xffff) goto L_8024E070;
    if ((u32)r0 == (u32)0x165) goto L_8024E070;
    if ((u32)r0 == (u32)0x163) goto L_8024E070;
    r3 = r28;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E070;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x56;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x56;
    fn_80239EE8();
L_8024E070: ;
    r3 = r31;
    r4 = r28;
    r5 = 0x57;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x57;
    fn_80239EE8();
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024E0CC | Size: 0x7C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8024E0CC(void* ctx, u32 slot, u32 param) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r31 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r29;
    r5 = 0x55;
    fn_80239984();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r29;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x55;
    fn_80239EE8();
    r3 = r31;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024E148 | Size: 0xEC (236 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024E148(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r28;
    r5 = 0x53;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x53;
    fn_80239EE8();
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E21C;
    r3 = r31;
    r4 = r28;
    r5 = 0x54;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x54;
    fn_80239EE8();
L_8024E21C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024E234 | Size: 0xEC (236 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024E234(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x0;
    r4 = r28;
    r5 = 0x51;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x51;
    fn_80239EE8();
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E308;
    r3 = r31;
    r4 = r28;
    r5 = 0x52;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x52;
    fn_80239EE8();
L_8024E308: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024E320 | Size: 0x164 (356 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024E320(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_802358AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x6) goto L_8024E39C;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x4e;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x4e;
    fn_80239EE8();
L_8024E39C: ;
    r3 = r28;
    r4 = r29;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 >= (u32)0x6) goto L_8024E3FC;
    r3 = r31;
    r4 = r28;
    r5 = 0x4f;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x4f;
    fn_80239EE8();
L_8024E3FC: ;
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E46C;
    r3 = r31;
    r4 = r28;
    r5 = 0x50;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x50;
    fn_80239EE8();
L_8024E46C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024E484 | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024E484(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x1;
    r7 = 0x10e;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024E514;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x4d;
    fn_80239984();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139c */;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x4d;
    fn_80239EE8();
L_8024E514: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024E534 | Size: 0x44 | Pattern: field_accessor */
u32 fn_8024E534(void* ctx, u32 slot, u32 param) {
    extern void fn_801FB1C0();
    u32 val;
    val = (u32)fn_801FB1C0(ctx, 0, 0x43, 0);
    fn_801FB1C0(0, val & 0xFFFF, 0x2, 0);
    return 0;
}

/* Address: 0x8024E578 | Size: 0x118 (280 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024E578(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375D30[];
    extern void fn_801F4354();
    extern void fn_801FB1C0();
    extern void fn_8020505C();
    extern void fn_80205B8C();
    extern void fn_8023A118();
    extern void fn_8024E690();
    extern void fn_8024F8B4();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    r6 = 0x0;
    /* stmw r27, 0x1c(r1) */;
    r28 = r4;
    r29 = r5;
    r27 = r3;
    r4 = 0x0;
    r5 = 0x43;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r28;
    r3 = 0x0;
    fn_801F4354();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r28;
    r5 = r29;
    fn_8024F8B4();
    /* mr. r31, r3 */;
    if ((s32)r0 > (s32)0) goto L_8024E5EC;
    r3 = 0x0;
    goto L_8024E67C;
L_8024E5EC: ;
    r3 = r28;
    fn_80205B8C();
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x228;
    r7 = r3;
    r6 = r30;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13fb */;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r27;
    r4 = r29;
    r6 = r28;
    r5 = 0x1;
    fn_8024E690();
    r5 = r3;
    r0 = (s16)r5;
    if ((s32)r0 >= (s32)0) goto L_8024E658;
    r3 = 0x0;
    goto L_8024E67C;
L_8024E658: ;
    r4 = (u32)lbl_80375D30;
    r8 = (s16)r5;
    r7 = (u32)lbl_80375D30;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_8020505C();
    r3 = 0x1;
L_8024E67C: ;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024F8B4 | Size: 0x5CC (1484 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024F8B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_801F4354();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
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
    extern void fn_80250070();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r5 = 0x43;
    r6 = 0x0;
    /* stmw r14, 0x58(r1) */;
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
    r5 = r1 + 0x38;
    r6 = 0x0;
    r7 = 0x1;
    fn_802367CC();
    r27 = r3;
    r4 = r15;
    r5 = r1 + 0x18;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r28 = r3;
    r14 = r1 + 0x18;
    r31 = r3 & 0xFFFF;
    r18 = 0x0;
    r17 = 0x0;
    r23 = 0x0;
    goto L_8024FA50;
L_8024F968: ;
    /* clrlslwi r0, r23, 16, 2 */;
    r19 = *(u32*)(r14 + r0);
    if ((u32)r19 == (u32)0x0) goto L_8024FA4C;
    r3 = r15;
    r4 = r19;
    fn_802376EC();
    r25 = r3 & 0xFFFF;
    r30 = r27 & 0xFFFF;
    r21 = 0x0;
    goto L_8024FA40;
L_8024F994: ;
    /* clrlslwi r0, r21, 16, 1 */;
    r3 = r1 + 0x38;
    r22 = *(u16*)(r3 + r0);
    if ((u32)r22 == (u32)0x0) goto L_8024FA3C;
    if ((u32)r22 == (u32)0x165) goto L_8024FA3C;
    r3 = r15;
    r4 = r16;
    r5 = r22;
    r6 = r19;
    fn_8023C530();
    r29 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F9D8;
    r17 = 0x1;
L_8024F9D8: ;
    r3 = r15;
    r4 = r19;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024F9F4;
    r17 = 0x1;
L_8024F9F4: ;
    r3 = r15;
    r4 = r22;
    r5 = 0x1;
    fn_8023943C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024FA3C;
    r3 = r15;
    r4 = r16;
    r5 = r22;
    r6 = r19;
    r7 = 0x0;
    fn_8023C370();
    if ((s32)r25 >= (s32)r3) goto L_8024FA3C;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FA3C;
    r18 = 0x1;
L_8024FA3C: ;
    r21 = r21 + 0x1;
L_8024FA40: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_8024F994;
L_8024FA4C: ;
    r23 = r23 + 0x1;
L_8024FA50: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8024F968;
    r3 = r16;
    fn_80205B8C();
    r8 = 0x0;
    r5 = (0x1 << 16);
    r0 = 0x227;
    r7 = r3;
    r6 = r20;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13fb */;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r15;
    r4 = r16;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r31) goto L_8024FAFC;
    r4 = r15;
    r3 = 0x0;
    r5 = 0x1;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x1;
    fn_80239EE8();
L_8024FAFC: ;
    r3 = r15;
    r4 = r16;
    fn_8023753C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)r31) goto L_8024FB5C;
    r3 = r24;
    r4 = r15;
    r5 = 0x2;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x2;
    fn_80239EE8();
    goto L_8024FBB8;
L_8024FB5C: ;
    r3 = r15;
    r4 = r16;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r31) goto L_8024FBB8;
    r3 = r24;
    r4 = r15;
    r5 = 0x3;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x3;
    fn_80239EE8();
L_8024FBB8: ;
    r3 = r15;
    r4 = r16;
    fn_80235714();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FC18;
    r3 = r24;
    r4 = r15;
    r5 = 0x4;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x4;
    fn_80239EE8();
L_8024FC18: ;
    r3 = r15;
    r4 = r16;
    fn_8023565C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FC78;
    r3 = r24;
    r4 = r15;
    r5 = 0x5;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x5;
    fn_80239EE8();
L_8024FC78: ;
    r0 = r17 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FCC8;
    r3 = r24;
    r4 = r15;
    r5 = 0x6;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x6;
    fn_80239EE8();
L_8024FCC8: ;
    r0 = r18 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FD1C;
    r3 = r24;
    r4 = r15;
    r5 = 0x7;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x7;
    fn_80239EE8();
L_8024FD1C: ;
    r3 = r15;
    r4 = r16;
    fn_8023753C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FDC4;
    r17 = r1 + 0x18;
    r14 = r28 & 0xFFFF;
    r18 = 0x0;
    goto L_8024FDB8;
L_8024FD44: ;
    /* clrlslwi r0, r18, 16, 2 */;
    r4 = *(u32*)(r17 + r0);
    if ((u32)r4 == (u32)0x0) goto L_8024FDB4;
    r3 = r15;
    fn_80236E9C();
    r3 = r3 & 0xFFFF;
    r0 = r26 & 0xFFFF;
    if ((u32)r3 <= (u32)r0) goto L_8024FDB4;
    r3 = r24;
    r4 = r15;
    r5 = 0x8;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r7 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r7, 0x139d */;
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x8;
    fn_80239EE8();
L_8024FDB4: ;
    r18 = r18 + 0x1;
L_8024FDB8: ;
    r0 = r18 & 0xFFFF;
    if ((u32)r0 < (u32)r14) goto L_8024FD44;
L_8024FDC4: ;
    r3 = r15;
    r4 = r16;
    fn_80250070();
    r0 = r3 & 0xFFFF;
    r14 = r3;
    if ((u32)r0 == (u32)r14) goto L_8024FE24;
    r3 = r24;
    r4 = r15;
    r5 = r14;
    fn_80239984();
    r0 = r3;
    r3 = r16;
    r24 = r0;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139d */;
    r4 = r20;
    r10 = r14;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    fn_80239EE8();
L_8024FE24: ;
    r3 = r16;
    fn_80205B8C();
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x226;
    r7 = r3;
    r6 = r20;
    *(u32*)(sp + 0xC) = r0;
    /* subi r3, r5, 0x139d */;
    /* subi r4, r5, 0x13fc */;
    /* subi r5, r5, 0x13fb */;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
    r3 = r24;
    /* lmw r14, 0x58(r1) */;
    return;
}
#pragma pop

/* Address: 0x8024FE80 | Size: 0x1F0 (496 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8024FE80(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A420[];
    extern void fn_801F1C18();
    extern void fn_80235B04();
    extern void fn_80236FFC();
    extern void fn_8023715C();
    extern void fn_80237F74();
    extern void fn_80239058();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r5 = 0x1;
    /* stmw r22, 0x48(r1) */;
    r30 = r4;
    r29 = r3;
    r4 = 0x0;
    fn_80235B04();
    r4 = (u32)lbl_8027A420;
    r0 = 0x2;
    r4 = (u32)lbl_8027A420;
    r31 = r3;
    r6 = r1 + 0x4;
    /* subi r4, r4, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_8024FEC0: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r6 + 0x4) = r3;
    r6 += 8; *(u32*)r6 = r0;
    if (--ctr != 0) goto L_8024FEC0;
    r0 = *(u32*)((u8*)r4 + 0x4);
    r4 = r29;
    r5 = r1 + 0x1c;
    r3 = 0x0;
    *(u32*)((u8*)r6 + 0x4) = r0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r28 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x16;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FF6C;
    r25 = r1 + 0x1c;
    r26 = r28 & 0xFFFF;
    r24 = 0x0;
    goto L_8024FF60;
L_8024FF24: ;
    /* clrlslwi r27, r24, 16, 2 */;
    r3 = r29;
    r4 = *(u32*)(r25 + r27);
    fn_8023715C();
    r4 = *(u32*)(r25 + r27);
    r27 = r3;
    r3 = r29;
    fn_80236FFC();
    r4 = r27 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    if ((u32)r4 < (u32)r0) goto L_8024FF5C;
    r3 = 0x25;
    goto L_8025005C;
L_8024FF5C: ;
    r24 = r24 + 0x1;
L_8024FF60: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_8024FF24;
L_8024FF6C: ;
    r3 = r29;
    r4 = r30;
    r5 = 0x24;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250008;
    r27 = r1 + 0x8;
    r25 = r1 + 0x1c;
    r28 = r28 & 0xFFFF;
    r24 = 0x0;
    goto L_8024FFFC;
L_8024FF9C: ;
    /* clrlslwi r26, r24, 16, 2 */;
    r22 = 0x0;
    r23 = 0x0;
    goto L_8024FFD8;
L_8024FFAC: ;
    /* clrlslwi r0, r23, 16, 1 */;
    r4 = *(u32*)(r25 + r26);
    r5 = *(u16*)(r27 + r0);
    r3 = r29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024FFD4;
    r22 = 0x1;
    goto L_8024FFE4;
L_8024FFD4: ;
    r23 = r23 + 0x1;
L_8024FFD8: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)0xa) goto L_8024FFAC;
L_8024FFE4: ;
    r0 = r22 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8024FFF8;
    r3 = 0x26;
    goto L_8025005C;
L_8024FFF8: ;
    r24 = r24 + 0x1;
L_8024FFFC: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8024FF9C;
L_80250008: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)r28) goto L_80250058;
    r3 = r29;
    r4 = r30;
    r5 = 0x4d;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250034;
    r3 = 0x27;
    goto L_8025005C;
L_80250034: ;
    r3 = r29;
    r4 = r30;
    r5 = 0xd;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250058;
    r3 = 0x28;
    goto L_8025005C;
L_80250058: ;
    r3 = 0x0;
L_8025005C: ;
    /* lmw r22, 0x48(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250070 | Size: 0x27C (636 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80250070(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023753C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x9;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802500A4;
    r3 = 0x9;
    goto L_802502D8;
L_802500A4: ;
    r3 = r30;
    r4 = r31;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802500C8;
    r3 = 0xa;
    goto L_802502D8;
L_802500C8: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250100;
    r3 = r30;
    r4 = r31;
    fn_8023753C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250100;
    r3 = 0xb;
    goto L_802502D8;
L_80250100: ;
    r3 = r30;
    r4 = r31;
    r5 = 0xe;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250124;
    r3 = 0xc;
    goto L_802502D8;
L_80250124: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x17;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250148;
    r3 = 0xd;
    goto L_802502D8;
L_80250148: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025016C;
    r3 = 0xe;
    goto L_802502D8;
L_8025016C: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x19;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250190;
    r3 = 0xf;
    goto L_802502D8;
L_80250190: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x1b;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802501B4;
    r3 = 0x10;
    goto L_802502D8;
L_802501B4: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802501D8;
    r3 = 0x11;
    goto L_802502D8;
L_802501D8: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802501FC;
    r3 = 0x12;
    goto L_802502D8;
L_802501FC: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250220;
    r3 = 0x13;
    goto L_802502D8;
L_80250220: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x27;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250244;
    r3 = 0x14;
    goto L_802502D8;
L_80250244: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x28;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250268;
    r3 = 0x15;
    goto L_802502D8;
L_80250268: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x29;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025028C;
    r3 = 0x16;
    goto L_802502D8;
L_8025028C: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x2a;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802502B0;
    r3 = 0x17;
    goto L_802502D8;
L_802502B0: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x30;
    fn_80236BFC();
    r3 = r3 & 0xFF;
    r0 = 0x18;
    /* subi r3, r3, 0x1 */;
    /* subic r3, r3, 0x1 */;
    r3 = r3 - r3; /* -borrow */;
    r3 = r0 & r3;
L_802502D8: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802502EC | Size: 0x694 (1684 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802502EC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375D70[];
    extern void fn_8011FC74();
    extern void fn_80142984();
    extern void fn_801440F0();
    extern void fn_801F0134();
    extern void fn_801F1A6C();
    extern void fn_801F1C18();
    extern void fn_801F7C54();
    extern void fn_801FB1C0();
    extern void fn_80204CE0();
    extern void fn_80205B8C();
    extern void fn_802062FC();
    extern void fn_80206608();
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
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r6 = 0x0;
    /* stmw r17, 0x214(r1) */;
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
    r4 = r1 + 0x38;
    r30 = 0x0;
    r5 = 0x14;
    r6 = 0x1;
    fn_801F7C54();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if ((s32)r0 != (s32)0) goto L_80250358;
    r3 = 0x0;
    goto L_8025096C;
L_80250358: ;
    r4 = r28;
    r5 = r1 + 0x18;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1C18();
    r17 = r3;
    r4 = r28;
    r5 = r1 + 0x60;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r26 = r3;
    r18 = r1 + 0x18;
    r17 = r17 & 0xFFFF;
    r20 = 0x0;
    goto L_802503E0;
L_802503A0: ;
    /* clrlslwi r0, r20, 16, 2 */;
    r19 = *(u32*)(r18 + r0);
    if ((u32)r19 == (u32)0x0) goto L_802503DC;
    r3 = r19;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r19 == (u32)0x0) goto L_802503DC;
    r3 = r28;
    r4 = r19;
    fn_80235714();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802503DC;
    r30 = 0x0;
L_802503DC: ;
    r20 = r20 + 0x1;
L_802503E0: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)r17) goto L_802503A0;
    r3 = r1 + 0xc0;
    r5 = 0x0;
    r4 = 0x0;
    goto L_80250408;
L_802503FC: ;
    /* clrlslwi r0, r5, 16, 2 */;
    r5 = r5 + 0x1;
    *(u32*)(r3 + r0) = r4;
L_80250408: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)0x14) goto L_802503FC;
    r20 = r1 + 0x38;
    r27 = r31 & 0xFFFF;
    r24 = 0x0;
    goto L_80250850;
L_80250424: ;
    /* clrlslwi r0, r24, 16, 1 */;
    r25 = r24 & 0xFFFF;
    r23 = *(u16*)(r20 + r0);
    if ((u32)r23 == (u32)0x0) goto L_8025084C;
    r3 = r23;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((u32)r23 == (u32)0x0) goto L_8025084C;
    r3 = r23;
    r4 = r29;
    fn_802126C4();
    r21 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x7) goto L_8025084C;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r5 = r3;
    r6 = r23;
    r3 = r1 + 0x110;
    r4 = 0x0;
    r7 = 0x0;
    fn_801440F0();
    r3 = (s16)r3;
    r0 = -r3;
    r0 = r0 & ~r3;
    /* srwi. r0, r0, 31 */;
    if ((u32)r0 == (u32)0x7) goto L_8025084C;
    r0 = r21 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_802504B8;
    if ((u32)r0 != (u32)0x1) goto L_80250530;
L_802504B8: ;
    r3 = r28;
    r4 = r29;
    fn_8023753C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250530;
    r3 = r29;
    fn_80205B8C();
    fn_8011FC74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250530;
    r17 = r25 << 2;
    r18 = r1 + 0xc0;
    r3 = *(u32*)(r18 + r17);
    r4 = r28;
    r5 = 0x2e;
    fn_80239984();
    *(u32*)(r18 + r17) = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139b */;
    r4 = r28;
    r9 = r23;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x2e;
    fn_80239EE8();
L_80250530: ;
    r0 = r21 & 0xFF;
    if ((u32)r0 == (u32)0x3) goto L_80250544;
    if ((u32)r0 != (u32)0x1) goto L_802505B8;
L_80250544: ;
    r3 = r28;
    r4 = r29;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802505B8;
    r3 = r29;
    fn_80205B8C();
    fn_8011FC74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802505B8;
    r17 = r25 << 2;
    r18 = r1 + 0xc0;
    r3 = *(u32*)(r18 + r17);
    r4 = r28;
    r5 = 0x2f;
    fn_80239984();
    *(u32*)(r18 + r17) = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139b */;
    r4 = r28;
    r9 = r23;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x2f;
    fn_80239EE8();
L_802505B8: ;
    r0 = r21 & 0xFF;
    if ((u32)r0 != (u32)0x5) goto L_80250624;
    r3 = r28;
    r4 = r29;
    fn_80235714();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x5) goto L_80250624;
    r17 = r25 << 2;
    r18 = r1 + 0xc0;
    r3 = *(u32*)(r18 + r17);
    r4 = r28;
    r5 = 0x30;
    fn_80239984();
    *(u32*)(r18 + r17) = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139b */;
    r4 = r28;
    r9 = r23;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x30;
    fn_80239EE8();
L_80250624: ;
    r0 = r21 & 0xFF;
    if ((u32)r0 != (u32)0x4) goto L_80250694;
    r3 = r28;
    r4 = r29;
    fn_80236C80();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_80250694;
    r17 = r25 << 2;
    r18 = r1 + 0xc0;
    r3 = *(u32*)(r18 + r17);
    r4 = r28;
    r5 = 0x31;
    fn_80239984();
    *(u32*)(r18 + r17) = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139b */;
    r4 = r28;
    r9 = r23;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x31;
    fn_80239EE8();
L_80250694: ;
    r0 = r21 & 0xFF;
    if ((u32)r0 != (u32)0x6) goto L_802506F8;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802506F8;
    r17 = r25 << 2;
    r18 = r1 + 0xc0;
    r3 = *(u32*)(r18 + r17);
    r4 = r28;
    r5 = 0x32;
    fn_80239984();
    *(u32*)(r18 + r17) = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139b */;
    r4 = r28;
    r9 = r23;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x32;
    fn_80239EE8();
L_802506F8: ;
    r3 = r28;
    r4 = r29;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_80250800;
    r3 = r28;
    r4 = r29;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x3) goto L_80250800;
    r19 = r1 + 0x60;
    r17 = r26 & 0xFFFF;
    r18 = 0x0;
    goto L_802507F4;
L_80250738: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* clrlslwi r21, r18, 16, 2 */;
    r0 = *(u32*)(r19 + r21);
    if ((u32)r3 == (u32)r0) goto L_802507F0;
    r3 = r28;
    r4 = r29;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_8025078C;
    r3 = r28;
    r4 = r29;
    fn_8023785C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x3) goto L_802507F0;
L_8025078C: ;
    r3 = *(u32*)(r19 + r21);
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802507F0;
    r17 = r25 << 2;
    r18 = r1 + 0xc0;
    r3 = *(u32*)(r18 + r17);
    r4 = r28;
    r5 = 0x33;
    fn_80239984();
    *(u32*)(r18 + r17) = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    /* subi r3, r6, 0x139b */;
    r4 = r28;
    r9 = r23;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x33;
    fn_80239EE8();
    goto L_80250800;
L_802507F0: ;
    r18 = r18 + 0x1;
L_802507F4: ;
    r0 = r18 & 0xFFFF;
    if ((u32)r0 < (u32)r17) goto L_80250738;
L_80250800: ;
    r3 = r29;
    fn_80205B8C();
    r0 = 0x226;
    r4 = r1 + 0xc0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0xC) = r0;
    r0 = r25 << 2;
    r7 = r3;
    r6 = r28;
    r0 = *(u32*)(r4 + r0);
    /* subi r3, r5, 0x139b */;
    /* subi r4, r5, 0x13ba */;
    /* subi r5, r5, 0xdc8 */;
    *(u32*)(sp + 0x10) = r0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_8023A118();
L_8025084C: ;
    r24 = r24 + 0x1;
L_80250850: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_80250424;
    r4 = r1 + 0xc0;
    r0 = r31 & 0xFFFF;
    r17 = 0x0;
    goto L_80250880;
L_8025086C: ;
    /* clrlslwi r3, r17, 16, 2 */;
    r3 = *(u32*)(r4 + r3);
    if ((s32)r3 > (s32)0x0) goto L_8025088C;
    r17 = r17 + 0x1;
L_80250880: ;
    r3 = r17 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8025086C;
L_8025088C: ;
    r3 = r17 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_802508A4;
    r3 = 0x0;
    goto L_8025096C;
L_802508A4: ;
    r4 = r31;
    r3 = r1 + 0xc0;
    r5 = 0x1;
    fn_802397B8();
    /* mr. r18, r3 */;
    if ((u32)r3 >= (u32)r0) goto L_802508C4;
    r3 = 0x0;
    goto L_8025096C;
L_802508C4: ;
    /* clrlslwi r0, r17, 16, 1 */;
    r3 = r1 + 0x38;
    r17 = *(u16*)(r3 + r0);
    if ((u32)r17 != (u32)0x0) goto L_802508E0;
    r3 = 0x0;
    goto L_8025096C;
L_802508E0: ;
    r3 = r29;
    fn_80205B8C();
    r0 = 0x228;
    r4 = r1 + 0xc0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0xC) = r0;
    r0 = r18 << 2;
    r7 = r3;
    r6 = r28;
    r0 = *(u32*)(r4 + r0);
    /* subi r3, r5, 0x139b */;
    /* subi r4, r5, 0x13ba */;
    /* subi r5, r5, 0xdc8 */;
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
    fn_80204CE0();
    r3 = 0x1;
L_8025096C: ;
    /* lmw r17, 0x214(r1) */;
    return;
}
#pragma pop

/* Address: 0x802509A0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802509A0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250A0C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80250A0C;
    r31 = r31 << 1;
L_80250A0C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250A2C | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80250A2C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250A98;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80250A98;
    r31 = r31 << 1;
L_80250A98: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250AC0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80250AC0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250B2C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80250B2C;
    r31 = r31 << 1;
L_80250B2C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250B44 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80250B44(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80235B04();
    extern void fn_80250BBC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r28 = r4;
    r29 = r5;
    r27 = r3;
    r30 = r6;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r4 = (u32)fn_80250BBC;
    r31 = r3;
    r9 = (u32)fn_80250BBC;
    r3 = r27;
    r4 = r29;
    r5 = r28;
    r6 = r30;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x0;
    fn_80211170();
    r0 = r31 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_80250BA8;
    r3 = r3 << 1;
L_80250BA8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250BBC | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80250BBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_80235B04();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r5;
    r5 = 0x1;
    fn_80235B04();
    r31 = r3;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_80250C08;
    r0 = 0xb;
    goto L_80250C3C;
L_80250C08: ;
    if ((u32)r0 != (u32)0x3) goto L_80250C18;
    r0 = 0x5;
    goto L_80250C3C;
L_80250C18: ;
    if ((u32)r0 != (u32)0x1) goto L_80250C28;
    r0 = 0xa;
    goto L_80250C3C;
L_80250C28: ;
    if ((u32)r0 != (u32)0x4) goto L_80250C38;
    r0 = 0xf;
    goto L_80250C3C;
L_80250C38: ;
    r0 = 0x0;
L_80250C3C: ;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250C64 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80250C64(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250CD0;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80250CD0;
    r31 = r31 << 1;
L_80250CD0: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250CF0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80250CF0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250D5C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80250D5C;
    r31 = r31 << 1;
L_80250D5C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250D7C | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80250D7C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250DE8;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80250DE8;
    r31 = r31 << 1;
L_80250DE8: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250E00 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80250E00(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80250E6C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80250E6C;
    r31 = r31 << 1;
L_80250E6C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250E84 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80250E84(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80250EC4 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80250EC4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_802152A8();
    extern void fn_802377E8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r5;
    r31 = r3;
    r5 = 0xd9;
    r3 = r30;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    r4 = r30;
    fn_802377E8();
    r0 = r3;
    r3 = 0x0;
    r4 = r0;
    r5 = 0x5f;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    fn_802152A8();
    r0 = r3;
    r3 = r31;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80250F7C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80250F7C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80250FBC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80250FBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80237664();
    extern void fn_802376EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r31 = r5;
    r28 = r3;
    r5 = 0xd9;
    r3 = r31;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r28;
    r29 = r0;
    r4 = r31;
    fn_802376EC();
    r30 = r3;
    r3 = r28;
    r4 = r31;
    fn_80237664();
    r31 = r3;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    r0 = r30 & 0xFFFF;
    r3 = r3 * r0;
    r0 = r31 & 0xFFFF;
    r0 = (s32)r3 / (s32)r0;
    r0 = r0 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80251044;
    r0 = 0x1;
L_80251044: ;
    r3 = r29;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251070 | Size: 0x5C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251070(void* ctx, u32 slot, u32 param) {
    extern void fn_802376EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_802376EC();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_802376EC();
    r0 = r3 & 0xFFFF;
    /* xoris r4, r0, 0x8000 */;
    r3 = r31 - r0;
    r0 = r0 - r31;
    r3 = r3 + r4;
    r3 = r3 - r3; /* -borrow */;
    r3 = r0 & ~r3;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x802510CC | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802510CC(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251138;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80251138;
    r31 = r31 << 1;
L_80251138: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251158 | Size: 0x3C | Pattern: simple_wrapper */
extern u32 fn_80211170(void* ctx, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
u32 fn_80251158(void* ctx, u32 param1, u32 param2, u32 param3) {
    return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0);
}

/* Address: 0x80251194 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251194(void* ctx, u32 param1, u32 param2, u32 param3) {
    return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0);
}

/* Address: 0x802511E0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802511E0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025124C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_8025124C;
    r31 = r31 << 1;
L_8025124C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802512A4 | Size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802512A4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BEB4();
    extern void fn_801363E8();
    extern void fn_801F54A4();
    extern void fn_8023C370();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r30 = r5;
    r29 = r6;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0xf;
    r6 = 0x0;
    fn_801F54A4();
    r3 = r3 & 0xFFFF;
    fn_801363E8();
    r4 = r30;
    r30 = r3;
    r3 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_8011BEB4();
    r31 = r3 & 0xFFFF;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)r31) goto L_80251338;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x1;
    fn_8023C370();
    goto L_8025133C;
L_80251338: ;
    r3 = 0x0;
L_8025133C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251358 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251358(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r6;
    r5 = r0;
    r10 = 0x0;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r30;
    r5 = 0x5;
    r31 = r0;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802513B8;
    r31 = r31 << 1;
L_802513B8: ;
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802513D0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802513D0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025143C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_8025143C;
    r31 = r31 << 1;
L_8025143C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251454 | Size: 0x70 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251454(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_8023720C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r4 = r5;
    r30 = r3;
    r5 = r31;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r30;
    r31 = r0;
    fn_8023720C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802514AC;
    r31 = r31 << 1;
L_802514AC: ;
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802514EC | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802514EC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80119DD0();
    extern void fn_80202360();
    extern void fn_80236BFC();
    extern void fn_80237664();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x2d;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = 0x1;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025152C;
    r3 = r30;
    r4 = 0x2d;
    fn_80202360();
    r31 = r3;
L_8025152C: ;
    r3 = 0x2d;
    fn_80119DD0();
    r0 = r3 & 0xFF;
    r0 = r0 - r31;
    r0 = (s16)r0;
    if ((u32)r0 >= (u32)0x1) goto L_80251548;
    r0 = 0x0;
L_80251548: ;
    r0 = (s16)r0;
    r3 = 0x1;
    r0 = r3 << r0;
    r3 = r29;
    r4 = r30;
    r31 = (s16)r0;
    fn_80237664();
    r0 = r3 & 0xFFFF;
    r0 = (s32)r0 / (s32)r31;
    r3 = -r0;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251584 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80251584(void* ctx, u32 param1, u32 param2) {
    extern void fn_80202360();
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r29 = r5;
    r27 = r3;
    r28 = r4;
    r30 = r6;
    r31 = 0x1;
    r5 = 0x2d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802515CC;
    r3 = r28;
    r4 = 0x2d;
    fn_80202360();
    r31 = r3;
L_802515CC: ;
    r3 = r27;
    r4 = r29;
    r5 = r28;
    r6 = r30;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_80211170();
    r0 = (s16)r31;
    /* lmw r27, 0xc(r1) */;
    r3 = r0 * r3;
    return;
}
#pragma pop

/* Address: 0x80251614 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251614(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251658 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80251658(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251688 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251688(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x802516C4 | Size: 0xD4 (212 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802516C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80216CF8();
    extern void fn_80237774();
    extern void fn_802377E8();
    extern void fn_8023892C();
    extern void fn_80238980();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r7 = 0x1;
    /* stmw r22, 0x68(r1) */;
    r22 = r3;
    r23 = r5;
    r24 = r6;
    r5 = r1 + 0x8;
    r4 = r22;
    r28 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fn_801F1A6C();
    r29 = r1 + 0x8;
    r31 = r3 & 0xFFFF;
    r27 = 0x0;
    goto L_80251774;
L_8025170C: ;
    /* clrlslwi r30, r27, 16, 2 */;
    r3 = r22;
    r4 = *(u32*)(r29 + r30);
    fn_80238980();
    r4 = *(u32*)(r29 + r30);
    r26 = r3;
    r3 = r22;
    fn_8023892C();
    r30 = r3;
    r3 = r22;
    r4 = r24;
    fn_802377E8();
    r0 = r3;
    r3 = r22;
    r25 = r0;
    r4 = r24;
    fn_80237774();
    r0 = r3;
    r3 = r23;
    r4 = r26;
    r6 = r25;
    r5 = r30 & 0xFF;
    r7 = r0 & 0xFF;
    fn_80216CF8();
    r28 = r28 + r3;
    r27 = r27 + 0x1;
L_80251774: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8025170C;
    r3 = r28;
    /* lmw r22, 0x68(r1) */;
    return;
}
#pragma pop

/* Address: 0x802517A0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802517A0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025180C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_8025180C;
    r31 = r31 << 1;
L_8025180C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251824 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251824(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251860 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251860(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r6;
    r5 = r0;
    r10 = 0x0;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r30;
    r5 = 0x23;
    r31 = r0;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802518C0;
    r31 = r31 << 1;
L_802518C0: ;
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802518D8 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802518D8(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r6;
    r5 = r0;
    r10 = 0x0;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r30;
    r5 = 0x1f;
    r31 = r0;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251938;
    r31 = r31 << 1;
L_80251938: ;
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251950 | Size: 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80251950(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BEB4();
    extern void fn_801F025C();
    extern void fn_802026E4();
    extern void fn_80232110();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = 0x0;
    /* stmw r27, 0xc(r1) */;
    r28 = r5;
    r27 = r4;
    r29 = r6;
    r4 = r28;
    r5 = 0x7;
    r6 = 0x0;
    fn_8011BEB4();
    r31 = r3 & 0xFFFF;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x3;
    r6 = 0x0;
    fn_8011BEB4();
    r30 = r3 & 0xFFFF;
    r4 = r29;
    r3 = 0x2;
    fn_801F025C();
    r0 = r3;
    r3 = r27;
    r5 = r0;
    r4 = r29;
    r6 = r28;
    r7 = r31;
    r8 = r30;
    fn_80232110();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = 0x32;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802519F4;
    r3 = r31 * 0xf;
    r0 = 0xa;
    r31 = (s32)r3 / (s32)r0;
L_802519F4: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251A0C | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251A0C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r6;
    r5 = r0;
    r10 = 0x0;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r30;
    r5 = 0x20;
    r31 = r0;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251A6C;
    r31 = r31 << 1;
L_80251A6C: ;
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251A84 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251A84(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r6;
    r5 = r0;
    r10 = 0x0;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r30;
    r5 = 0x1f;
    r31 = r0;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251AE4;
    r31 = r31 << 1;
L_80251AE4: ;
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251AFC | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251AFC(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251B50 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251B50(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251BBC;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80251BBC;
    r31 = r31 << 1;
L_80251BBC: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251BD4 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251BD4(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251C40;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80251C40;
    r31 = r31 << 1;
L_80251C40: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251C58 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251C58(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251CC4;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80251CC4;
    r31 = r31 << 1;
L_80251CC4: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251CEC | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80251CEC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251D2C | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80251D2C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_80120B00();
    extern void fn_80205B8C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    r31 = r5;
    r3 = r31;
    r5 = 0xd9;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    fn_80205B8C();
    r4 = r1 + 0xa;
    r5 = r1 + 0x8;
    fn_80120B00();
    r7 = *(u16*)(sp + 0xA);
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    r7 = *(u16*)(sp + 0x8);
    r3 = r31;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_8011BBD8();
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x80251DB4 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80251DB4(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235B04();
    extern void fn_80237664();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x1;
    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80251DF4;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    /* extrwi r0, r3, 15, 16 */;
    goto L_80251E2C;
L_80251DF4: ;
    if ((u32)r0 != (u32)0x1) goto L_80251E1C;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    r3 = r3 & 0xFFFF;
    r0 = 0x1e;
    r3 = r3 * 0x14;
    r0 = (s32)r3 / (s32)r0;
    goto L_80251E2C;
L_80251E1C: ;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    /* extrwi r0, r3, 14, 16 */;
L_80251E2C: ;
    r3 = -r0;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251E44 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80251E44(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235B04();
    extern void fn_80237664();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x1;
    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80251E84;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    /* extrwi r0, r3, 15, 16 */;
    goto L_80251EBC;
L_80251E84: ;
    if ((u32)r0 != (u32)0x1) goto L_80251EAC;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    r3 = r3 & 0xFFFF;
    r0 = 0x1e;
    r3 = r3 * 0x14;
    r0 = (s32)r3 / (s32)r0;
    goto L_80251EBC;
L_80251EAC: ;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    /* extrwi r0, r3, 14, 16 */;
L_80251EBC: ;
    r3 = -r0;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251ED4 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80251ED4(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235B04();
    extern void fn_80237664();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x1;
    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80251F14;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    /* extrwi r0, r3, 15, 16 */;
    goto L_80251F4C;
L_80251F14: ;
    if ((u32)r0 != (u32)0x1) goto L_80251F3C;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    r3 = r3 & 0xFFFF;
    r0 = 0x1e;
    r3 = r3 * 0x14;
    r0 = (s32)r3 / (s32)r0;
    goto L_80251F4C;
L_80251F3C: ;
    r3 = r30;
    r4 = r31;
    fn_80237664();
    /* extrwi r0, r3, 14, 16 */;
L_80251F4C: ;
    r3 = -r0;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251F6C | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80251F6C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80251FD8;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80251FD8;
    r31 = r31 << 1;
L_80251FD8: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80251FF0 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80251FF0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252038 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252038(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252078 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252078(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C(param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x46);
    return 0;
}

/* Address: 0x802520BC | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802520BC(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252128;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252128;
    r31 = r31 << 1;
L_80252128: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252148 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252148(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252188 | Size: 0x80 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252188(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80217BD0();
    extern void fn_8023842C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r30 = r5;
    r29 = r3;
    r5 = 0xd9;
    r3 = r30;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r29;
    r4 = r30;
    fn_8023842C();
    fn_80217BD0();
    r0 = r3;
    r3 = r31;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252208 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252208(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252248 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252248(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C(param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x28);
    return 0;
}

/* Address: 0x8025228C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_8025228C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802522CC | Size: 0x80 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802522CC(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80217BEC();
    extern void fn_8023842C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r30 = r5;
    r29 = r3;
    r5 = 0xd9;
    r3 = r30;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r29;
    r4 = r30;
    fn_8023842C();
    fn_80217BEC();
    r0 = r3;
    r3 = r31;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252354 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80252354(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252398 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252398(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802523D8 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802523D8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r5 = 0xd9;
    r3 = r29;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r30 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r29;
    r5 = 0x1a;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025243C;
    /* clrlslwi r30, r30, 17, 1 */;
L_8025243C: ;
    r3 = r31;
    r7 = r30 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802524B8 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802524B8(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252524;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252524;
    r31 = r31 << 1;
L_80252524: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025253C | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025253C(void* ctx, u32 param1, u32 param2) {
    extern void fn_80211170();
    extern void fn_802525F0();
    extern void fn_80252634();
    extern void fn_80252678();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    r7 = (u32)fn_80252678;
    r8 = 0x0;
    r9 = (u32)fn_80252678;
    r7 = 0x0;
    r10 = 0x0;
    /* stmw r27, 0xc(r1) */;
    r28 = r4;
    r29 = r5;
    r27 = r3;
    r30 = r6;
    r5 = r28;
    r4 = r29;
    fn_80211170();
    r4 = (u32)fn_80252634;
    r31 = r3;
    r9 = (u32)fn_80252634;
    r3 = r27;
    r4 = r29;
    r5 = r28;
    r6 = r30;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x0;
    fn_80211170();
    r4 = (u32)fn_802525F0;
    r31 = r31 + r3;
    r9 = (u32)fn_802525F0;
    r3 = r27;
    r4 = r29;
    r5 = r28;
    r6 = r30;
    r7 = 0x0;
    r8 = 0x0;
    r10 = 0x0;
    fn_80211170();
    r31 = r31 + r3;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802525F0 | Size: 0x44 | Pattern: field_accessor */
u32 fn_802525F0(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C(param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x1e);
    return 0;
}

/* Address: 0x80252634 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252634(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C(param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x14);
    return 0;
}

/* Address: 0x80252678 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252678(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C(param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0xa);
    return 0;
}

/* Address: 0x802526BC | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802526BC(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252728;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252728;
    r31 = r31 << 1;
L_80252728: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252748 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252748(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_802376EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    /* stmw r27, 0xc(r1) */;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r4 = r30;
    fn_802376EC();
    r31 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r28;
    r6 = r30;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_80211170();
    r4 = r31 & 0xFFFF;
    if ((s32)r4 > (s32)r3) goto L_802527A8;
    /* subi r3, r4, 0x1 */;
L_802527A8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802527C4 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_802527C4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252804 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80252804(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_80218B6C();
    extern void fn_80237664();
    extern void fn_802376EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r29 = r5;
    r30 = r3;
    r5 = 0xd9;
    r3 = r29;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r30;
    r31 = r0;
    r4 = r29;
    fn_802376EC();
    r0 = r3;
    r3 = r30;
    r30 = r0;
    r4 = r29;
    fn_80237664();
    r0 = r3;
    r3 = r30;
    r4 = r0;
    fn_80218B6C();
    r7 = r3 & 0xFF;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252894 | Size: 0x28 | Pattern: call_return_u16 */
extern u32 fn_802376EC(void*, u32, u32);
u16 fn_80252894(void* ctx, u32 p1, u32 p2, u32 p3) { return (u16)fn_802376EC(ctx, p3, p2); }

/* Address: 0x802528DC | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802528DC(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252918 | Size: 0x54 | Pattern: field_accessor */
u32 fn_80252918(void* ctx, u32 slot, u32 param) {
    extern u32 fn_802376EC();
    u32 val1, val2, avg;
    val1 = fn_802376EC(ctx, slot) & 0xFFFF;
    val2 = fn_802376EC(ctx, slot) & 0xFFFF;
    avg = (s32)(val1 + val2) >> 1;
    return val2 - avg;
}

/* Address: 0x8025297C | Size: 0x24 | Pattern: call_return_u8 */
extern u32 fn_80237774(void*);
u8 fn_8025297C(void* ctx) { return (u8)fn_80237774(ctx); }

/* Address: 0x802529A0 | Size: 0x24 | Pattern: call_return_u8 */
u8 fn_802529A0(void* ctx) { return (u8)fn_80237774(ctx); }

/* Address: 0x802529F4 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802529F4(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252A60;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252A60;
    r31 = r31 << 1;
L_80252A60: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252A80 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252A80(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252AEC;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252AEC;
    r31 = r31 << 1;
L_80252AEC: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252B04 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252B04(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252B44 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252B44(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252BB0;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252BB0;
    r31 = r31 << 1;
L_80252BB0: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252BC8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80252BC8(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252C04 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252C04(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252C70;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252C70;
    r31 = r31 << 1;
L_80252C70: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252C88 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252C88(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252CF4;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252CF4;
    r31 = r31 << 1;
L_80252CF4: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252D0C | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252D0C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252D78;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252D78;
    r31 = r31 << 1;
L_80252D78: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252D90 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252D90(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252DFC;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252DFC;
    r31 = r31 << 1;
L_80252DFC: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252E14 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252E14(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252E80;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252E80;
    r31 = r31 << 1;
L_80252E80: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252E98 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252E98(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252F04;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252F04;
    r31 = r31 << 1;
L_80252F04: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80252F8C | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80252F8C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80252FF8;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80252FF8;
    r31 = r31 << 1;
L_80252FF8: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253020 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80253020(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025308C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_8025308C;
    r31 = r31 << 1;
L_8025308C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802530A4 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_802530A4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802530E4 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802530E4(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80253150;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80253150;
    r31 = r31 << 1;
L_80253150: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253168 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253168(void* ctx, u32 param1, u32 param2) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r30 = r5;
    r29 = r3;
    r31 = r6;
    r4 = r30;
    r5 = r0;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r29;
    r5 = 0x21;
    r31 = r0;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802531D8;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0xfa) goto L_802531D8;
    r31 = r31 << 1;
L_802531D8: ;
    r3 = r31;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x802531F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802531F8(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253234 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80253234(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253270 | Size: 0x28 | Pattern: call_return_u16 */
u16 fn_80253270(void* ctx, u32 p1, u32 p2, u32 p3) { return (u16)fn_802376EC(ctx, p3, p2); }

/* Address: 0x80253298 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80253298(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802532C0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802532C0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025332C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_8025332C;
    r31 = r31 << 1;
L_8025332C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025334C | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025334C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802533B8;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_802533B8;
    r31 = r31 << 1;
L_802533B8: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802533D8 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802533D8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253400 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80253400(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025346C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_8025346C;
    r31 = r31 << 1;
L_8025346C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025348C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_8025348C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802534D4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802534D4(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253548 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80253548(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802535B4;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_802535B4;
    r31 = r31 << 1;
L_802535B4: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802535F4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802535F4(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253630 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80253630(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x8025366C | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025366C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802536D8;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_802536D8;
    r31 = r31 << 1;
L_802536D8: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802536F0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802536F0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025375C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_8025375C;
    r31 = r31 << 1;
L_8025375C: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253774 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80253774(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802537E0;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_802537E0;
    r31 = r31 << 1;
L_802537E0: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802537F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802537F8(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253834 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80253834(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_80211170();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802538A0;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_802538A0;
    r31 = r31 << 1;
L_802538A0: ;
    r3 = r31;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802538C0 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802538C0(void* ctx, u32 param1, u32 param2) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r30 = r5;
    r29 = r3;
    r31 = r6;
    r4 = r30;
    r5 = r0;
    fn_80211170();
    r0 = r3;
    r4 = r31;
    r3 = r29;
    r5 = 0x21;
    r31 = r0;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80253930;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x39) goto L_80253930;
    r31 = r31 << 1;
L_80253930: ;
    r3 = r31;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253950 | Size: 0x6C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80253950(void* ctx, u32 slot, u32 param) {
    extern void fn_80136428();
    extern void fn_801F54A4();
    extern void fn_80237DBC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0xf;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r3 = r3 & 0xFFFF;
    fn_80136428();
    r0 = r3;
    r3 = r30;
    r4 = r31;
    r5 = r0 & 0xFF;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    /* lmw r30, 0x8(r1) */;
    r3 = 0x1 - r0;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
    return;
}
#pragma pop

/* Address: 0x802539BC | Size: 0xC4 (196 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802539BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235910();
    extern void fn_80235AA0();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    fn_80235AA0();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80235910();
    r0 = r31 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253A10;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253A10;
    r3 = 0x0;
    goto L_80253A6C;
L_80253A10: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253A68;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x3;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253A68;
    r3 = 0x0;
    goto L_80253A6C;
L_80253A68: ;
    r3 = 0x1;
L_80253A6C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253A80 | Size: 0xC4 (196 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253A80(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235974();
    extern void fn_802359D8();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    fn_802359D8();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80235974();
    r0 = r31 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253AD4;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253AD4;
    r3 = 0x0;
    goto L_80253B30;
L_80253AD4: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x4;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253B2C;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x5;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253B2C;
    r3 = 0x0;
    goto L_80253B30;
L_80253B2C: ;
    r3 = 0x1;
L_80253B30: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253B44 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80253B44(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253B78 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253B78(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80253BF4;
    r30 = 0x0;
L_80253BF4: ;
    if ((s32)r30 != (s32)0x0) goto L_80253C04;
    r3 = 0x0;
    goto L_80253C18;
L_80253C04: ;
    if ((s32)r30 != (s32)-0x1) goto L_80253C14;
    r3 = 0x1;
    goto L_80253C18;
L_80253C14: ;
    r3 = 0x1;
L_80253C18: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253C2C | Size: 0xC4 (196 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253C2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    fn_80235AA0();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80235A3C();
    r0 = r31 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253C80;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253C80;
    r3 = 0x0;
    goto L_80253CDC;
L_80253C80: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253CD8;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253CD8;
    r3 = 0x0;
    goto L_80253CDC;
L_80253CD8: ;
    r3 = 0x1;
L_80253CDC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253CF0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253CF0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80253D6C;
    r30 = 0x0;
L_80253D6C: ;
    if ((s32)r30 != (s32)0x0) goto L_80253D7C;
    r3 = 0x0;
    goto L_80253D90;
L_80253D7C: ;
    if ((s32)r30 != (s32)-0x1) goto L_80253D8C;
    r3 = 0x1;
    goto L_80253D90;
L_80253D8C: ;
    r3 = 0x1;
L_80253D90: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253DA4 | Size: 0xC4 (196 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253DA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235974();
    extern void fn_80235A3C();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    fn_80235A3C();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80235974();
    r0 = r31 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253DF8;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80253DF8;
    r3 = 0x0;
    goto L_80253E54;
L_80253DF8: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253E50;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x5;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80253E50;
    r3 = 0x0;
    goto L_80253E54;
L_80253E50: ;
    r3 = 0x1;
L_80253E54: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253E68 | Size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253E68(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r4 = r30;
    fn_80235AA0();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    r0 = r31 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80253EB8;
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80253EB8;
    r3 = 0x0;
    goto L_80253F14;
L_80253EB8: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80253F10;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x2;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80253F10;
    r3 = 0x0;
    goto L_80253F14;
L_80253F10: ;
    r3 = 0x1;
L_80253F14: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253F28 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253F28(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80253FA4;
    r30 = 0x0;
L_80253FA4: ;
    if ((s32)r30 != (s32)0x0) goto L_80253FB4;
    r3 = 0x0;
    goto L_80253FC8;
L_80253FB4: ;
    if ((s32)r30 != (s32)-0x1) goto L_80253FC4;
    r3 = 0x1;
    goto L_80253FC8;
L_80253FC4: ;
    r3 = 0x1;
L_80253FC8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80253FDC | Size: 0xF0 (240 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80253FDC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235B04();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r31 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_8025401C;
    r30 = 0xb;
    goto L_80254050;
L_8025401C: ;
    if ((u32)r0 != (u32)0x3) goto L_8025402C;
    r30 = 0x5;
    goto L_80254050;
L_8025402C: ;
    if ((u32)r0 != (u32)0x1) goto L_8025403C;
    r30 = 0xa;
    goto L_80254050;
L_8025403C: ;
    if ((u32)r0 != (u32)0x4) goto L_8025404C;
    r30 = 0xf;
    goto L_80254050;
L_8025404C: ;
    r30 = 0x0;
L_80254050: ;
    r3 = r27;
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802540A0;
    r31 = 0x0;
L_802540A0: ;
    if ((s32)r31 != (s32)0x0) goto L_802540B0;
    r3 = 0x0;
    goto L_802540B8;
L_802540B0: ;
    r3 = 0x1;
L_802540B8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop


/* -------------------------------------------------------------------
 * Item Rewards & Poke Coupon (0x80254000-0x80258000)
 * 95 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802540CC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802540CC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254148;
    r30 = 0x0;
L_80254148: ;
    if ((s32)r30 != (s32)0x0) goto L_80254158;
    r3 = 0x0;
    goto L_8025416C;
L_80254158: ;
    if ((s32)r30 != (s32)-0x1) goto L_80254168;
    r3 = 0x1;
    goto L_8025416C;
L_80254168: ;
    r3 = 0x1;
L_8025416C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254180 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254180(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802541B4 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802541B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254230;
    r30 = 0x0;
L_80254230: ;
    if ((s32)r30 != (s32)0x0) goto L_80254240;
    r3 = 0x0;
    goto L_80254254;
L_80254240: ;
    if ((s32)r30 != (s32)-0x1) goto L_80254250;
    r3 = 0x1;
    goto L_80254254;
L_80254250: ;
    r3 = 0x1;
L_80254254: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254268 | Size: 0x1F8 (504 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254268(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    if ((u32)r0 != (u32)0x1) goto L_802542C0;
    r3 = 0x0;
    goto L_8025444C;
L_802542C0: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025439C;
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254300;
    r0 = 0x0;
    goto L_802543A0;
L_80254300: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025439C;
    r3 = r27;
    r4 = r31;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025439C;
    r3 = r27;
    r4 = r31;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025439C;
    r3 = r27;
    r4 = r31;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025439C;
    r3 = r27;
    r4 = r31;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025439C;
    r3 = r27;
    r4 = r31;
    r5 = 0xc;
    fn_80237F74();
L_8025439C: ;
    r0 = 0x1;
L_802543A0: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802543B0;
    r3 = 0x0;
    goto L_8025444C;
L_802543B0: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802543D4;
    r3 = 0x0;
    goto L_8025444C;
L_802543D4: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802543F8;
    r3 = 0x0;
    goto L_8025444C;
L_802543F8: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
    r3 = r30;
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254434;
    r3 = 0x0;
    goto L_8025444C;
L_80254434: ;
    if ((s32)r31 != (s32)0x0) goto L_80254444;
    r3 = 0x0;
    goto L_8025444C;
L_80254444: ;
    r3 = 0x1;
L_8025444C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254460 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254460(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802544DC;
    r30 = 0x0;
L_802544DC: ;
    if ((s32)r30 != (s32)0x0) goto L_802544EC;
    r3 = 0x0;
    goto L_80254500;
L_802544EC: ;
    if ((s32)r30 != (s32)-0x1) goto L_802544FC;
    r3 = 0x1;
    goto L_80254500;
L_802544FC: ;
    r3 = 0x1;
L_80254500: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254514 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254514(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254590;
    r30 = 0x0;
L_80254590: ;
    if ((s32)r30 != (s32)0x0) goto L_802545A0;
    r3 = 0x0;
    goto L_802545B4;
L_802545A0: ;
    if ((s32)r30 != (s32)-0x1) goto L_802545B0;
    r3 = 0x1;
    goto L_802545B4;
L_802545B0: ;
    r3 = 0x1;
L_802545B4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802545C8 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802545C8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025464C;
    r31 = 0x0;
L_8025464C: ;
    if ((s32)r31 != (s32)0x0) goto L_8025465C;
    r3 = 0x0;
    goto L_80254664;
L_8025465C: ;
    r3 = 0x1;
L_80254664: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254680 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80254680(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802546B8 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_802546B8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802546E8 | Size: 0x128 (296 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802546E8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    r6 = 0x0;
    r7 = 0x0;
    r5 = r1 + 0x1c;
    /* stmw r27, 0x5c(r1) */;
    r29 = r3;
    r27 = r4;
    fn_802367CC();
    r31 = r3;
    r4 = r29;
    r5 = r1 + 0x30;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r28 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x27;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254754;
    r3 = 0x0;
    goto L_802547FC;
L_80254754: ;
    r27 = r1 + 0x30;
    r28 = r28 & 0xFFFF;
    r30 = 0x0;
    goto L_802547EC;
L_80254764: ;
    /* clrlslwi r0, r30, 16, 2 */;
    r4 = *(u32*)(r27 + r0);
    if ((u32)r4 == (u32)0x0) goto L_802547E8;
    r3 = r29;
    r5 = r1 + 0x8;
    r6 = 0x0;
    r7 = 0x0;
    fn_802367CC();
    r5 = r1 + 0x1c;
    r4 = r31 & 0xFFFF;
    r8 = r1 + 0x8;
    r0 = r3 & 0xFFFF;
    r10 = 0x0;
    goto L_802547DC;
L_802547A0: ;
    /* clrlslwi r7, r10, 16, 1 */;
    r9 = 0x0;
    goto L_802547CC;
L_802547AC: ;
    /* clrlslwi r3, r9, 16, 1 */;
    r6 = *(u16*)(r8 + r7);
    r3 = *(u16*)(r5 + r3);
    if ((u32)r6 != (u32)r3) goto L_802547C8;
    r3 = 0x1;
    goto L_802547FC;
L_802547C8: ;
    r9 = r9 + 0x1;
L_802547CC: ;
    r3 = r9 & 0xFFFF;
    if ((u32)r3 < (u32)r4) goto L_802547AC;
    r10 = r10 + 0x1;
L_802547DC: ;
    r3 = r10 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_802547A0;
L_802547E8: ;
    r30 = r30 + 0x1;
L_802547EC: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_80254764;
    r3 = 0x0;
L_802547FC: ;
    /* lmw r27, 0x5c(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254810 | Size: 0xC8 (200 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254810(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80229934();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = (0x1 << 16);
    /* subi r7, r7, 0x1 */;
    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r28;
    r5 = r30;
    fn_80229934();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254864;
    r3 = 0x0;
    goto L_802548C4;
L_80254864: ;
    r3 = r27;
    r4 = r28;
    r5 = 0x19;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254888;
    r3 = 0x0;
    goto L_802548C4;
L_80254888: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x19;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802548AC;
    r3 = 0x0;
    goto L_802548C4;
L_802548AC: ;
    if ((s32)r31 != (s32)0x0) goto L_802548BC;
    r3 = 0x0;
    goto L_802548C4;
L_802548BC: ;
    r3 = 0x1;
L_802548C4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802548D8 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802548D8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254954;
    r30 = 0x0;
L_80254954: ;
    if ((s32)r30 != (s32)0x0) goto L_80254964;
    r3 = 0x0;
    goto L_80254978;
L_80254964: ;
    if ((s32)r30 != (s32)-0x1) goto L_80254974;
    r3 = 0x1;
    goto L_80254978;
L_80254974: ;
    r3 = 0x1;
L_80254978: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025498C | Size: 0xE4 (228 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025498C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802376EC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = r6;
    r5 = r27;
    r4 = r28;
    fn_802395C8();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    r4 = r27;
    fn_802376EC();
    r31 = r3;
    r3 = r26;
    r4 = r29;
    fn_802376EC();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r3 > (u32)r0) goto L_802549F4;
    r3 = 0x0;
    goto L_80254A5C;
L_802549F4: ;
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r26;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r26;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254A44;
    r31 = 0x0;
L_80254A44: ;
    if ((s32)r31 != (s32)0x0) goto L_80254A54;
    r3 = 0x0;
    goto L_80254A5C;
L_80254A54: ;
    r3 = 0x1;
L_80254A5C: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254A70 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254A70(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254AEC;
    r30 = 0x0;
L_80254AEC: ;
    if ((s32)r30 != (s32)0x0) goto L_80254AFC;
    r3 = 0x0;
    goto L_80254B10;
L_80254AFC: ;
    if ((s32)r30 != (s32)-0x1) goto L_80254B0C;
    r3 = 0x1;
    goto L_80254B10;
L_80254B0C: ;
    r3 = 0x1;
L_80254B10: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254B24 | Size: 0x178 (376 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254B24(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80229704();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_8025C264();
    extern void fn_8025CC90();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r31 = r5;
    r30 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x48;
    r4 = r30;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254B64;
    r3 = 0x0;
    goto L_80254C88;
L_80254B64: ;
    r3 = r28;
    r4 = r30;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254B88;
    r3 = 0x0;
    goto L_80254C88;
L_80254B88: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254BAC;
    r3 = 0x0;
    goto L_80254C88;
L_80254BAC: ;
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254BD4;
    r3 = 0x0;
    goto L_80254C88;
L_80254BD4: ;
    r3 = r28;
    r4 = r29;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
    r3 = r28;
    r4 = r30;
    fn_8025CC90();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254C10;
    r3 = 0x0;
    goto L_80254C88;
L_80254C10: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254C34;
    r3 = 0x0;
    goto L_80254C88;
L_80254C34: ;
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254C50;
    r3 = 0x0;
    goto L_80254C88;
L_80254C50: ;
    r4 = r30;
    r3 = 0x8;
    fn_80229704();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80254C70;
    r3 = 0x0;
    goto L_80254C88;
L_80254C70: ;
    if ((s32)r31 != (s32)0x0) goto L_80254C80;
    r3 = 0x0;
    goto L_80254C88;
L_80254C80: ;
    r3 = 0x1;
L_80254C88: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254C9C | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254C9C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254D20;
    r31 = 0x0;
L_80254D20: ;
    if ((s32)r31 != (s32)0x0) goto L_80254D30;
    r3 = 0x0;
    goto L_80254D38;
L_80254D30: ;
    r3 = 0x1;
L_80254D38: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254D4C | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254D4C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254DC8;
    r30 = 0x0;
L_80254DC8: ;
    if ((s32)r30 != (s32)0x0) goto L_80254DD8;
    r3 = 0x0;
    goto L_80254DEC;
L_80254DD8: ;
    if ((s32)r30 != (s32)-0x1) goto L_80254DE8;
    r3 = 0x1;
    goto L_80254DEC;
L_80254DE8: ;
    r3 = 0x1;
L_80254DEC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254E00 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_80254E00(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254E34 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80254E34(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80254EB0;
    r30 = 0x0;
L_80254EB0: ;
    if ((s32)r30 != (s32)0x0) goto L_80254EC0;
    r3 = 0x0;
    goto L_80254ED4;
L_80254EC0: ;
    if ((s32)r30 != (s32)-0x1) goto L_80254ED0;
    r3 = 0x1;
    goto L_80254ED4;
L_80254ED0: ;
    r3 = 0x1;
L_80254ED4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80254EE8 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254EE8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F1C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80254F1C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F54 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254F54(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80254F88(void* ctx, u32 slot, u32 param) {
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = (0x1 << 16);
    /* subi r7, r7, 0x1 */;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x19;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80254FD4;
    r3 = 0x0;
    goto L_80254FEC;
L_80254FD4: ;
    if ((s32)r31 != (s32)0x0) goto L_80254FE4;
    r3 = 0x0;
    goto L_80254FEC;
L_80254FE4: ;
    r3 = 0x1;
L_80254FEC: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255000 | Size: 0xF0 (240 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255000(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80142984();
    extern void fn_80216048();
    extern void fn_80237F74();
    extern void fn_802383A4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r30 = r4;
    r29 = r6;
    fn_802383A4();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    r4 = r29;
    fn_802383A4();
    r0 = r3;
    r3 = r30;
    r30 = r0;
    fn_80216048();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80255054;
    r3 = 0x0;
    goto L_802550DC;
L_80255054: ;
    r0 = r31 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80255064;
    r0 = r30 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_802550AC;
L_80255064: ;
    r3 = r31 & 0xFFFF;
    if ((u32)r3 == (u32)0xaf) goto L_802550AC;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0xaf) goto L_802550AC;
    if ((u32)r3 == (u32)0x0) goto L_80255094;
    r3 = r31;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_802550AC;
L_80255094: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_802550B4;
    r3 = r30;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802550B4;
L_802550AC: ;
    r3 = 0x0;
    goto L_802550DC;
L_802550B4: ;
    r3 = r28;
    r4 = r29;
    r5 = 0x3c;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802550D8;
    r3 = 0x0;
    goto L_802550DC;
L_802550D8: ;
    r3 = 0x1;
L_802550DC: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802550F0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802550F0(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F025C();
    extern void fn_801F54A4();
    extern void fn_802062FC();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r4;
    r3 = 0xe;
    fn_801F025C();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_80255120;
    r3 = 0x0;
    goto L_80255190;
L_80255120: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x19;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_80255184;
    r3 = r31;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255184;
    r3 = r29;
    r4 = r30;
    r5 = 0x32;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255184;
    r3 = r29;
    r4 = r31;
    r5 = 0x32;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025518C;
L_80255184: ;
    r3 = 0x0;
    goto L_80255190;
L_8025518C: ;
    r3 = 0x1;
L_80255190: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x802551A4 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802551A4(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x30;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802551EC;
    r3 = 0x0;
    goto L_80255204;
L_802551EC: ;
    if ((s32)r31 != (s32)0x0) goto L_802551FC;
    r3 = 0x0;
    goto L_80255204;
L_802551FC: ;
    r3 = 0x1;
L_80255204: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255220 | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255220(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BEB4();
    extern void fn_801363E8();
    extern void fn_801F54A4();
    extern void fn_8023C530();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r30 = r5;
    r29 = r6;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0xf;
    r6 = 0x0;
    fn_801F54A4();
    r3 = r3 & 0xFFFF;
    fn_801363E8();
    r4 = r30;
    r30 = r3;
    r3 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_8011BEB4();
    r31 = r3 & 0xFFFF;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)r31) goto L_802552B0;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    fn_8023C530();
    goto L_802552B4;
L_802552B0: ;
    r3 = 0x1;
L_802552B4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802552D0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802552D0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025534C;
    r30 = 0x0;
L_8025534C: ;
    if ((s32)r30 != (s32)0x0) goto L_8025535C;
    r3 = 0x0;
    goto L_80255370;
L_8025535C: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025536C;
    r3 = 0x1;
    goto L_80255370;
L_8025536C: ;
    r3 = 0x1;
L_80255370: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255384 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255384(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80255400;
    r30 = 0x0;
L_80255400: ;
    if ((s32)r30 != (s32)0x0) goto L_80255410;
    r3 = 0x0;
    goto L_80255424;
L_80255410: ;
    if ((s32)r30 != (s32)-0x1) goto L_80255420;
    r3 = 0x1;
    goto L_80255424;
L_80255420: ;
    r3 = 0x1;
L_80255424: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255438 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255438(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802554B4;
    r30 = 0x0;
L_802554B4: ;
    if ((s32)r30 != (s32)0x0) goto L_802554C4;
    r3 = 0x0;
    goto L_802554D8;
L_802554C4: ;
    if ((s32)r30 != (s32)-0x1) goto L_802554D4;
    r3 = 0x1;
    goto L_802554D8;
L_802554D4: ;
    r3 = 0x1;
L_802554D8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802554EC | Size: 0x10C (268 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802554EC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802359D8();
    extern void fn_80235AA0();
    extern void fn_80236BFC();
    extern void fn_8025C808();
    extern void fn_8025CAA8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    fn_80235AA0();
    r30 = r3;
    r3 = r26;
    r4 = r29;
    fn_802359D8();
    r31 = r3;
    r3 = r26;
    r4 = r29;
    r5 = r28;
    fn_8025CAA8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025554C;
    r3 = 0x0;
    goto L_802555E4;
L_8025554C: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255564;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255564;
    r3 = 0x0;
    goto L_802555E4;
L_80255564: ;
    r3 = r26;
    r4 = r27;
    r5 = r29;
    r6 = r28;
    r7 = 0xa0;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802555BC;
    r3 = r26;
    r4 = r27;
    r5 = r29;
    r6 = r28;
    r7 = 0xa0;
    r8 = 0x4;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802555BC;
    r3 = 0x0;
    goto L_802555E4;
L_802555BC: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802555E0;
    r3 = 0x0;
    goto L_802555E4;
L_802555E0: ;
    r3 = 0x1;
L_802555E4: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802555F8 | Size: 0x228 (552 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802555F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237DBC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r31 = r5;
    r30 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x14;
    r4 = r30;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255638;
    r3 = 0x0;
    goto L_8025580C;
L_80255638: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025565C;
    r3 = 0x0;
    goto L_8025580C;
L_8025565C: ;
    r3 = r28;
    r4 = r30;
    r5 = 0xa;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255680;
    r3 = 0x0;
    goto L_8025580C;
L_80255680: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802556A4;
    r3 = 0x0;
    goto L_8025580C;
L_802556A4: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80255780;
    r3 = r28;
    r4 = r30;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80255780;
    r3 = r28;
    r4 = r30;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80255780;
    r3 = r28;
    r4 = r30;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80255780;
    r3 = r28;
    r4 = r30;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80255780;
    r3 = r28;
    r4 = r30;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255754;
    r0 = 0x0;
    goto L_80255784;
L_80255754: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80255780;
    r3 = r28;
    r4 = r30;
    r5 = 0xc;
    fn_80237F74();
L_80255780: ;
    r0 = 0x1;
L_80255784: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255794;
    r3 = 0x0;
    goto L_8025580C;
L_80255794: ;
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802557B0;
    r3 = 0x0;
    goto L_8025580C;
L_802557B0: ;
    r3 = r28;
    r4 = r29;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802557F4;
    r3 = 0x0;
    goto L_8025580C;
L_802557F4: ;
    if ((s32)r31 != (s32)0x0) goto L_80255804;
    r3 = 0x0;
    goto L_8025580C;
L_80255804: ;
    r3 = 0x1;
L_8025580C: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255820 | Size: 0x218 (536 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255820(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80235910();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r25, 0x14(r1) */;
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
    fn_8025C264();
    r30 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802558D8;
    r0 = r29 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_802558D8;
    r3 = r31;
    r4 = r25;
    r5 = r27;
    r6 = r26;
    r7 = 0x10;
    r8 = 0x4;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_802558D8;
    r3 = 0x0;
    goto L_80255A24;
L_802558D8: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802558FC;
    r3 = 0x0;
    goto L_80255A24;
L_802558FC: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802559D8;
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025593C;
    r0 = 0x0;
    goto L_802559DC;
L_8025593C: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802559D8;
    r3 = r31;
    r4 = r27;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802559D8;
    r3 = r31;
    r4 = r27;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802559D8;
    r3 = r31;
    r4 = r27;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802559D8;
    r3 = r31;
    r4 = r27;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802559D8;
    r3 = r31;
    r4 = r27;
    r5 = 0xc;
    fn_80237F74();
L_802559D8: ;
    r0 = 0x1;
L_802559DC: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802559EC;
    r3 = 0x0;
    goto L_80255A24;
L_802559EC: ;
    r3 = r28;
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255A0C;
    r3 = 0x0;
    goto L_80255A24;
L_80255A0C: ;
    if ((s32)r30 != (s32)0x0) goto L_80255A1C;
    r3 = 0x0;
    goto L_80255A24;
L_80255A1C: ;
    r3 = 0x1;
L_80255A24: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255A38 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80255A38(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x1b;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80255A80;
    r3 = 0x0;
    goto L_80255A98;
L_80255A80: ;
    if ((s32)r31 != (s32)0x0) goto L_80255A90;
    r3 = 0x0;
    goto L_80255A98;
L_80255A90: ;
    r3 = 0x1;
L_80255A98: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255AAC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80255AAC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255AE4 | Size: 0x68 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80255AE4(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E648[];
    extern void fn_80236BFC();
    extern void fn_802373B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r5 = 0x2d;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80255B14;
    r3 = 0x0;
    goto L_80255B38;
L_80255B14: ;
    f1 = *(f32*)lbl_8047E648;
    r3 = r30;
    r4 = r31;
    r5 = 0x0;
    fn_802373B0();
    r0 = r3 & 0xFF;
    r3 = 0x1 - r0;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_80255B38: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255B4C | Size: 0xCC (204 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255B4C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = r6;
    r5 = r27;
    r4 = r28;
    fn_802395C8();
    r0 = r3;
    r3 = r26;
    r30 = r0;
    r4 = r27;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r26;
    r31 = r0;
    r4 = r27;
    r5 = 0x2d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80255BBC;
    r31 = 0x0;
L_80255BBC: ;
    r3 = r26;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r26;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80255BEC;
    r31 = 0x0;
L_80255BEC: ;
    if ((s32)r31 != (s32)0x0) goto L_80255BFC;
    r3 = 0x0;
    goto L_80255C04;
L_80255BFC: ;
    r3 = 0x1;
L_80255C04: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255C18 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80255C18(void* ctx, u32 slot, u32 param) {
    extern void fn_80119DD0();
    extern void fn_80202360();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r5 = 0x2d;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80255C44;
    r31 = 0x0;
    goto L_80255C54;
L_80255C44: ;
    r3 = r31;
    r4 = 0x2d;
    fn_80202360();
    r31 = r3;
L_80255C54: ;
    r3 = 0x2d;
    fn_80119DD0();
    r3 = r3 & 0xFF;
    r0 = (s16)r31;
    if ((s32)r0 < (s32)r3) goto L_80255C74;
    r3 = 0x0;
    goto L_80255C78;
L_80255C74: ;
    r3 = 0x1;
L_80255C78: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x80255C8C | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255C8C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80255D10;
    r31 = 0x0;
L_80255D10: ;
    if ((s32)r31 != (s32)0x0) goto L_80255D20;
    r3 = 0x0;
    goto L_80255D28;
L_80255D20: ;
    r3 = 0x1;
L_80255D28: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255D3C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80255D3C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255D7C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80255D7C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255DB4 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80255DB4(void* ctx, u32 slot, u32 param) {
    extern u32 fn_8025C808();
    /* r3=ctx, r4=slot, r5=param, r6=? -> call(ctx, slot, ?, param, 0x10, 0x2, 0x41) */
    u32 result = fn_8025C808(ctx, slot, 0, param, 0x10, 0x2, 0x41) & 0xFF;
    return (result != 0) ? 1 : 0;
}

/* Address: 0x80255DF8 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255DF8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80255E7C;
    r31 = 0x0;
L_80255E7C: ;
    if ((s32)r31 != (s32)0x0) goto L_80255E8C;
    r3 = 0x0;
    goto L_80255E94;
L_80255E8C: ;
    r3 = 0x1;
L_80255E94: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255EA8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80255EA8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255EEC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255EEC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80255F68;
    r30 = 0x0;
L_80255F68: ;
    if ((s32)r30 != (s32)0x0) goto L_80255F78;
    r3 = 0x0;
    goto L_80255F8C;
L_80255F78: ;
    if ((s32)r30 != (s32)-0x1) goto L_80255F88;
    r3 = 0x1;
    goto L_80255F8C;
L_80255F88: ;
    r3 = 0x1;
L_80255F8C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80255FA0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80255FA0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025601C;
    r30 = 0x0;
L_8025601C: ;
    if ((s32)r30 != (s32)0x0) goto L_8025602C;
    r3 = 0x0;
    goto L_80256040;
L_8025602C: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025603C;
    r3 = 0x1;
    goto L_80256040;
L_8025603C: ;
    r3 = 0x1;
L_80256040: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256054 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256054(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802560D0;
    r30 = 0x0;
L_802560D0: ;
    if ((s32)r30 != (s32)0x0) goto L_802560E0;
    r3 = 0x0;
    goto L_802560F4;
L_802560E0: ;
    if ((s32)r30 != (s32)-0x1) goto L_802560F0;
    r3 = 0x1;
    goto L_802560F4;
L_802560F0: ;
    r3 = 0x1;
L_802560F4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256108 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256108(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256184;
    r30 = 0x0;
L_80256184: ;
    if ((s32)r30 != (s32)0x0) goto L_80256194;
    r3 = 0x0;
    goto L_802561A8;
L_80256194: ;
    if ((s32)r30 != (s32)-0x1) goto L_802561A4;
    r3 = 0x1;
    goto L_802561A8;
L_802561A4: ;
    r3 = 0x1;
L_802561A8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802561BC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_802561BC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802561F4 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802561F4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256270;
    r30 = 0x0;
L_80256270: ;
    if ((s32)r30 != (s32)0x0) goto L_80256280;
    r3 = 0x0;
    goto L_80256294;
L_80256280: ;
    if ((s32)r30 != (s32)-0x1) goto L_80256290;
    r3 = 0x1;
    goto L_80256294;
L_80256290: ;
    r3 = 0x1;
L_80256294: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802562A8 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802562A8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256324;
    r30 = 0x0;
L_80256324: ;
    if ((s32)r30 != (s32)0x0) goto L_80256334;
    r3 = 0x0;
    goto L_80256348;
L_80256334: ;
    if ((s32)r30 != (s32)-0x1) goto L_80256344;
    r3 = 0x1;
    goto L_80256348;
L_80256344: ;
    r3 = 0x1;
L_80256348: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025635C | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025635C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802563D8;
    r30 = 0x0;
L_802563D8: ;
    if ((s32)r30 != (s32)0x0) goto L_802563E8;
    r3 = 0x0;
    goto L_802563FC;
L_802563E8: ;
    if ((s32)r30 != (s32)-0x1) goto L_802563F8;
    r3 = 0x1;
    goto L_802563FC;
L_802563F8: ;
    r3 = 0x1;
L_802563FC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256410 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256410(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256494;
    r31 = 0x0;
L_80256494: ;
    if ((s32)r31 != (s32)0x0) goto L_802564A4;
    r3 = 0x0;
    goto L_802564AC;
L_802564A4: ;
    r3 = 0x1;
L_802564AC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802564C8 | Size: 0x64 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802564C8(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E64C[];
    extern void fn_80235AA0();
    extern void fn_802373B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 >= (u32)0xc) goto L_8025650C;
    f1 = *(f32*)lbl_8047E64C;
    r3 = r30;
    r4 = r31;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xc) goto L_80256514;
L_8025650C: ;
    r3 = 0x0;
    goto L_80256518;
L_80256514: ;
    r3 = 0x1;
L_80256518: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025652C | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025652C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802565A8;
    r30 = 0x0;
L_802565A8: ;
    if ((s32)r30 != (s32)0x0) goto L_802565B8;
    r3 = 0x0;
    goto L_802565CC;
L_802565B8: ;
    if ((s32)r30 != (s32)-0x1) goto L_802565C8;
    r3 = 0x1;
    goto L_802565CC;
L_802565C8: ;
    r3 = 0x1;
L_802565CC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802565E0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802565E0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025665C;
    r30 = 0x0;
L_8025665C: ;
    if ((s32)r30 != (s32)0x0) goto L_8025666C;
    r3 = 0x0;
    goto L_80256680;
L_8025666C: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025667C;
    r3 = 0x1;
    goto L_80256680;
L_8025667C: ;
    r3 = 0x1;
L_80256680: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256694 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256694(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256710;
    r30 = 0x0;
L_80256710: ;
    if ((s32)r30 != (s32)0x0) goto L_80256720;
    r3 = 0x0;
    goto L_80256734;
L_80256720: ;
    if ((s32)r30 != (s32)-0x1) goto L_80256730;
    r3 = 0x1;
    goto L_80256734;
L_80256730: ;
    r3 = 0x1;
L_80256734: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256748 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256748(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256780 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256780(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802567B8 | Size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802567B8(void* ctx, u32 param1, u32 param2) {
    extern void fn_80120B00();
    extern void fn_80205B8C();
    extern void fn_8023793C();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r4;
    r28 = r3;
    r31 = r5;
    r30 = r6;
    r3 = r29;
    fn_80205B8C();
    r4 = r1 + 0xa;
    r5 = r1 + 0x8;
    fn_80120B00();
    r3 = r28;
    r4 = r29;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r0 = *(u16*)(sp + 0xA);
    r31 = r3;
    r5 = *(u16*)(sp + 0x8);
    r3 = r28;
    r4 = r30;
    r6 = (s16)r0;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256830;
    r31 = 0x0;
L_80256830: ;
    if ((s32)r31 != (s32)0x0) goto L_80256840;
    r3 = 0x0;
    goto L_80256848;
L_80256840: ;
    r3 = 0x1;
L_80256848: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025685C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025685C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256894 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256894(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802568CC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_802568CC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256904 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256904(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256988;
    r31 = 0x0;
L_80256988: ;
    if ((s32)r31 != (s32)0x0) goto L_80256998;
    r3 = 0x0;
    goto L_802569A0;
L_80256998: ;
    r3 = 0x1;
L_802569A0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802569B4 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802569B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256A30;
    r30 = 0x0;
L_80256A30: ;
    if ((s32)r30 != (s32)0x0) goto L_80256A40;
    r3 = 0x0;
    goto L_80256A54;
L_80256A40: ;
    if ((s32)r30 != (s32)-0x1) goto L_80256A50;
    r3 = 0x1;
    goto L_80256A54;
L_80256A50: ;
    r3 = 0x1;
L_80256A54: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256A68 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80256A68(void* ctx, u32 slot, u32 param) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r0 = r4;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r30 = r6;
    r4 = r29;
    r5 = r0;
    fn_802395C8();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    r4 = r29;
    fn_80239500();
    r6 = r3;
    r3 = r28;
    r4 = r30;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    /* lmw r28, 0x10(r1) */;
    r3 = 0x43 - r0;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
    return;
}
#pragma pop

/* Address: 0x80256AE0 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256AE0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256B18 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256B18(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256B94;
    r30 = 0x0;
L_80256B94: ;
    if ((s32)r30 != (s32)0x0) goto L_80256BA4;
    r3 = 0x0;
    goto L_80256BB8;
L_80256BA4: ;
    if ((s32)r30 != (s32)-0x1) goto L_80256BB4;
    r3 = 0x1;
    goto L_80256BB8;
L_80256BB4: ;
    r3 = 0x1;
L_80256BB8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256BCC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256BCC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256C48;
    r30 = 0x0;
L_80256C48: ;
    if ((s32)r30 != (s32)0x0) goto L_80256C58;
    r3 = 0x0;
    goto L_80256C6C;
L_80256C58: ;
    if ((s32)r30 != (s32)-0x1) goto L_80256C68;
    r3 = 0x1;
    goto L_80256C6C;
L_80256C68: ;
    r3 = 0x1;
L_80256C6C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256C80 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80256C80(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256CBC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256CBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256D38;
    r30 = 0x0;
L_80256D38: ;
    if ((s32)r30 != (s32)0x0) goto L_80256D48;
    r3 = 0x0;
    goto L_80256D5C;
L_80256D48: ;
    if ((s32)r30 != (s32)-0x1) goto L_80256D58;
    r3 = 0x1;
    goto L_80256D5C;
L_80256D58: ;
    r3 = 0x1;
L_80256D5C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256D70 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256D70(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256DF4;
    r31 = 0x0;
L_80256DF4: ;
    if ((s32)r31 != (s32)0x0) goto L_80256E04;
    r3 = 0x0;
    goto L_80256E0C;
L_80256E04: ;
    r3 = 0x1;
L_80256E0C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256E20 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256E20(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80256EA4;
    r31 = 0x0;
L_80256EA4: ;
    if ((s32)r31 != (s32)0x0) goto L_80256EB4;
    r3 = 0x0;
    goto L_80256EBC;
L_80256EB4: ;
    r3 = 0x1;
L_80256EBC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80256ED0 | Size: 0x200 (512 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80256ED0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802016A4();
    extern void fn_80236BFC();
    extern void fn_80237288();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
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
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    r4 = r28;
    r5 = 0xc;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80256F48;
    r29 = 0x0;
L_80256F48: ;
    r3 = r27;
    r4 = r28;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257030;
    r3 = r27;
    r4 = r28;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257030;
    r3 = r27;
    r4 = r28;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257030;
    r3 = r27;
    r4 = r28;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257030;
    r3 = r27;
    r4 = r28;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257030;
    r3 = r27;
    r4 = r28;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257030;
    r3 = r27;
    r4 = r28;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257030;
    r3 = r27;
    r4 = r28;
    r5 = 0xc;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257030;
    r0 = 0x0;
    goto L_80257034;
L_80257030: ;
    r0 = 0x1;
L_80257034: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257044;
    r3 = 0x0;
    goto L_802570BC;
L_80257044: ;
    r3 = r30 & 0xFF;
    r0 = r31 & 0xFF;
    if ((u32)r3 == (u32)r0) goto L_802570A0;
    r3 = r27;
    r4 = r28;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802570A0;
    r3 = r27;
    r4 = r28;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802570A0;
    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_802570A0;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_802570A4;
L_802570A0: ;
    r29 = 0x0;
L_802570A4: ;
    if ((s32)r29 != (s32)0x0) goto L_802570B4;
    r3 = 0x0;
    goto L_802570BC;
L_802570B4: ;
    r3 = 0x1;
L_802570BC: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802570D0 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802570D0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80257154;
    r31 = 0x0;
L_80257154: ;
    if ((s32)r31 != (s32)0x0) goto L_80257164;
    r3 = 0x0;
    goto L_8025716C;
L_80257164: ;
    r3 = 0x1;
L_8025716C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257180 | Size: 0x23C (572 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257180(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80235AA0();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r25, 0x14(r1) */;
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
    if ((u32)r0 != (u32)0x1) goto L_802571E8;
    r3 = 0x0;
    goto L_802573A8;
L_802571E8: ;
    r3 = r31;
    r4 = r25;
    r5 = r26;
    r6 = r27;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025725C;
    r0 = r29 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_8025725C;
    r3 = r31;
    r4 = r25;
    r5 = r27;
    r6 = r26;
    r7 = 0x20;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_8025725C;
    r3 = 0x0;
    goto L_802573A8;
L_8025725C: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257280;
    r3 = 0x0;
    goto L_802573A8;
L_80257280: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025735C;
    r3 = r31;
    r4 = r27;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802572C0;
    r0 = 0x0;
    goto L_80257360;
L_802572C0: ;
    r3 = r31;
    r4 = r27;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025735C;
    r3 = r31;
    r4 = r27;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025735C;
    r3 = r31;
    r4 = r27;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025735C;
    r3 = r31;
    r4 = r27;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025735C;
    r3 = r31;
    r4 = r27;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025735C;
    r3 = r31;
    r4 = r27;
    r5 = 0xc;
    fn_80237F74();
L_8025735C: ;
    r0 = 0x1;
L_80257360: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257370;
    r3 = 0x0;
    goto L_802573A8;
L_80257370: ;
    r3 = r28;
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257390;
    r3 = 0x0;
    goto L_802573A8;
L_80257390: ;
    if ((s32)r30 != (s32)0x0) goto L_802573A0;
    r3 = 0x0;
    goto L_802573A8;
L_802573A0: ;
    r3 = 0x1;
L_802573A8: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x802573BC | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802573BC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80257440;
    r31 = 0x0;
L_80257440: ;
    if ((s32)r31 != (s32)0x0) goto L_80257450;
    r3 = 0x0;
    goto L_80257458;
L_80257450: ;
    r3 = 0x1;
L_80257458: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257474 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80257474(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802574AC | Size: 0x5C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802574AC(void* ctx, u32 slot, u32 param) {
    extern void fn_80229934();
    extern void fn_8025C674();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = r5;
    r5 = r6;
    fn_80229934();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802574E0;
    r3 = 0x0;
    goto L_802574F4;
L_802574E0: ;
    r3 = r31;
    fn_8025C674();
    r3 = r3 & 0xFFFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_802574F4: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x80257508 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80257508(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80257544 | Size: 0x7C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80257544(void* ctx, u32 slot, u32 param) {
    extern void fn_80119DD0();
    extern void fn_801F025C();
    extern void fn_801F6D9C();
    extern void fn_801F6E98();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = 0x3;
    /* stmw r30, 0x8(r1) */;
    fn_801F025C();
    r30 = 0x0;
    r31 = r3;
    r4 = 0x4a;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257588;
    r3 = r31;
    r4 = 0x4a;
    fn_801F6D9C();
    r30 = r3;
L_80257588: ;
    r3 = 0x4a;
    fn_80119DD0();
    r3 = r3 & 0xFF;
    r0 = (s16)r30;
    if ((s32)r0 < (s32)r3) goto L_802575A8;
    r3 = 0x0;
    goto L_802575AC;
L_802575A8: ;
    r3 = 0x1;
L_802575AC: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802575C8 | Size: 0x188 (392 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802575C8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235910();
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_80236BFC();
    extern void fn_80237288();
    extern void fn_80237DBC();
    extern void fn_8025C808();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r25, 0x14(r1) */;
    r28 = r6;
    r26 = r4;
    r25 = r3;
    r27 = r5;
    r4 = r28;
    fn_80235AA0();
    r29 = r3;
    r3 = r25;
    r4 = r28;
    fn_80235A3C();
    r30 = r3;
    r3 = r25;
    r4 = r28;
    fn_80235910();
    r31 = r3;
    r3 = r25;
    r4 = r26;
    r5 = 0x7;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257690;
    r3 = r25;
    r4 = r28;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257654;
    r3 = 0x0;
    goto L_8025773C;
L_80257654: ;
    r3 = r25;
    r4 = r28;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257688;
    r3 = r25;
    r4 = r28;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257738;
L_80257688: ;
    r3 = 0x0;
    goto L_8025773C;
L_80257690: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802576B8;
    r0 = r29 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_802576B8;
    r0 = r30 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_802576B8;
    r3 = 0x0;
    goto L_8025773C;
L_802576B8: ;
    r3 = r25;
    r4 = r26;
    r5 = r28;
    r6 = r27;
    r7 = 0x90;
    r8 = 0x3;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80257738;
    r3 = r25;
    r4 = r26;
    r5 = r28;
    r6 = r27;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80257738;
    r3 = r25;
    r4 = r26;
    r5 = r28;
    r6 = r27;
    r7 = 0x10;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) goto L_80257738;
    r3 = 0x0;
    goto L_8025773C;
L_80257738: ;
    r3 = 0x1;
L_8025773C: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257750 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257750(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_802577C0;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802577A4;
    r3 = 0x0;
    goto L_80257824;
L_802577A4: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_802577C0: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x7;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802577F0;
    r3 = 0x0;
    goto L_80257824;
L_802577F0: ;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80257820;
    if ((s32)r31 != (s32)0x0) goto L_80257810;
    r3 = 0x0;
    goto L_80257824;
L_80257810: ;
    if ((s32)r31 != (s32)-0x1) goto L_80257820;
    r3 = 0x1;
    goto L_80257824;
L_80257820: ;
    r3 = 0x1;
L_80257824: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257838 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257838(void* ctx, u32 param1, u32 param2) {
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x14;
    /* stmw r30, 0x8(r1) */;
    r31 = r6;
    r30 = r3;
    r4 = r31;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257870;
    r3 = 0x0;
    goto L_802578B0;
L_80257870: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x17;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257894;
    r3 = 0x0;
    goto L_802578B0;
L_80257894: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x8;
    fn_80236BFC();
    r3 = r3 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_802578B0: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802578C4 | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802578C4(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = (0x1 << 16);
    /* subi r7, r7, 0x2 */;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x16;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257910;
    r3 = 0x0;
    goto L_80257928;
L_80257910: ;
    if ((s32)r31 != (s32)0x0) goto L_80257920;
    r3 = 0x0;
    goto L_80257928;
L_80257920: ;
    r3 = 0x1;
L_80257928: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025793C | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025793C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802579B8;
    r30 = 0x0;
L_802579B8: ;
    if ((s32)r30 != (s32)0x0) goto L_802579C8;
    r3 = 0x0;
    goto L_802579DC;
L_802579C8: ;
    if ((s32)r30 != (s32)-0x1) goto L_802579D8;
    r3 = 0x1;
    goto L_802579DC;
L_802579D8: ;
    r3 = 0x1;
L_802579DC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802579F0 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802579F0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80257A74;
    r31 = 0x0;
L_80257A74: ;
    if ((s32)r31 != (s32)0x0) goto L_80257A84;
    r3 = 0x0;
    goto L_80257A8C;
L_80257A84: ;
    r3 = 0x1;
L_80257A8C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257AA0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257AA0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80257B1C;
    r30 = 0x0;
L_80257B1C: ;
    if ((s32)r30 != (s32)0x0) goto L_80257B2C;
    r3 = 0x0;
    goto L_80257B40;
L_80257B2C: ;
    if ((s32)r30 != (s32)-0x1) goto L_80257B3C;
    r3 = 0x1;
    goto L_80257B40;
L_80257B3C: ;
    r3 = 0x1;
L_80257B40: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257B54 | Size: 0xCC (204 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257B54(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80238748();
    extern void fn_80239058();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x1;
    r7 = 0x1;
    /* stmw r25, 0x74(r1) */;
    r25 = r3;
    r26 = r5;
    r5 = r1 + 0x8;
    r4 = r25;
    r3 = 0x0;
    fn_801F1A6C();
    r28 = r1 + 0x8;
    r31 = r3 & 0xFFFF;
    r30 = r26 & 0xFFFF;
    r26 = 0x0;
    r27 = 0x0;
    goto L_80257BEC;
L_80257B9C: ;
    /* clrlslwi r29, r27, 16, 2 */;
    r4 = *(u32*)(r28 + r29);
    if ((u32)r4 == (u32)0x0) goto L_80257BE8;
    if ((u32)r30 != (u32)0xd7) goto L_80257BCC;
    r3 = r25;
    r5 = 0x2b;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80257BE8;
L_80257BCC: ;
    r4 = *(u32*)(r28 + r29);
    r3 = r25;
    fn_80238748();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257BE8;
    r26 = 0x1;
    goto L_80257BF8;
L_80257BE8: ;
    r27 = r27 + 0x1;
L_80257BEC: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_80257B9C;
L_80257BF8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 != (u32)r31) goto L_80257C08;
    r3 = 0x0;
    goto L_80257C0C;
L_80257C08: ;
    r3 = 0x1;
L_80257C0C: ;
    /* lmw r25, 0x74(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257C20 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257C20(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80257C9C;
    r30 = 0x0;
L_80257C9C: ;
    if ((s32)r30 != (s32)0x0) goto L_80257CAC;
    r3 = 0x0;
    goto L_80257CC0;
L_80257CAC: ;
    if ((s32)r30 != (s32)-0x1) goto L_80257CBC;
    r3 = 0x1;
    goto L_80257CC0;
L_80257CBC: ;
    r3 = 0x1;
L_80257CC0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257CD4 | Size: 0x124 (292 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257CD4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80237288();
    extern void fn_802381C4();
    extern void fn_8025C264();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
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

    r7 = (0x1 << 16);
    /* subi r7, r7, 0x1 */;
    /* stmw r27, 0x3c(r1) */;
    r30 = r3;
    r31 = r6;
    fn_8025C264();
    r27 = r3;
    r3 = r30;
    r4 = r31;
    r5 = r1 + 0x1c;
    r6 = r1 + 0x8;
    r7 = 0x0;
    fn_802367CC();
    r28 = r3;
    r3 = r30;
    r4 = r31;
    fn_802364BC();
    r0 = r3 & 0xFFFF;
    r29 = r3;
    if ((s32)r0 == (s32)0) goto L_80257D58;
    if ((u32)r0 == (u32)0x165) goto L_80257D58;
    if ((u32)r0 == (u32)0xffff) goto L_80257D58;
    r3 = r30;
    r4 = r31;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80257D60;
L_80257D58: ;
    r3 = 0x0;
    goto L_80257DE4;
L_80257D60: ;
    r7 = r28 & 0xFFFF;
    r6 = r1 + 0x8;
    r5 = r29 & 0xFFFF;
    r4 = r1 + 0x1c;
    r3 = 0x0;
    r10 = 0x0;
    goto L_80257DB0;
L_80257D7C: ;
    /* clrlslwi r8, r10, 24, 1 */;
    r9 = *(s16*)(r6 + r8);
    r0 = (s16)r9;
    if ((u32)r0 < (u32)0x1) goto L_80257DAC;
    r0 = *(u16*)(r4 + r8);
    if ((u32)r5 != (u32)r0) goto L_80257DAC;
    r3 = r30;
    r4 = r31;
    r5 = r9 & 0xFF;
    fn_802381C4();
    goto L_80257DBC;
L_80257DAC: ;
    r10 = r10 + 0x1;
L_80257DB0: ;
    r0 = r10 & 0xFF;
    if ((s32)r0 < (s32)r7) goto L_80257D7C;
L_80257DBC: ;
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)r7) goto L_80257DCC;
    r3 = 0x0;
    goto L_80257DE4;
L_80257DCC: ;
    if ((s32)r27 != (s32)0x0) goto L_80257DDC;
    r3 = 0x0;
    goto L_80257DE4;
L_80257DDC: ;
    r3 = 0x1;
L_80257DE4: ;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257DF8 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257DF8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80257E74;
    r30 = 0x0;
L_80257E74: ;
    if ((s32)r30 != (s32)0x0) goto L_80257E84;
    r3 = 0x0;
    goto L_80257E98;
L_80257E84: ;
    if ((s32)r30 != (s32)-0x1) goto L_80257E94;
    r3 = 0x1;
    goto L_80257E98;
L_80257E94: ;
    r3 = 0x1;
L_80257E98: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257EAC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80257EAC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80257EE4 | Size: 0xE4 (228 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257EE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80218FDC();
    extern void fn_8021901C();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x0;
    r7 = 0x1;
    r5 = r1 + 0x8;
    /* stmw r27, 0x2c(r1) */;
    r28 = r3;
    r27 = r4;
    fn_802367CC();
    r29 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80257F30;
    r3 = 0x0;
    goto L_80257FB4;
L_80257F30: ;
    r30 = r1 + 0x8;
    r31 = r29 & 0xFFFF;
    r28 = 0x0;
    goto L_80257F8C;
L_80257F40: ;
    /* clrlslwi r0, r28, 16, 1 */;
    r27 = *(u16*)(r30 + r0);
    if ((u32)r27 == (u32)0x165) goto L_80257F88;
    if ((u32)r27 == (u32)0x163) goto L_80257F88;
    r3 = r27;
    fn_80218FDC();
    r0 = r3 & 0xFF;
    if ((u32)r27 != (u32)0x163) goto L_80257F88;
    if ((u32)r27 == (u32)0x108) goto L_80257F88;
    if ((u32)r27 == (u32)0xfd) goto L_80257F88;
    r3 = r27;
    fn_8021901C();
    r0 = r3 & 0xFF;
    if ((u32)r27 == (u32)0xfd) goto L_80257F98;
L_80257F88: ;
    r28 = r28 + 0x1;
L_80257F8C: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_80257F40;
L_80257F98: ;
    r3 = r28 & 0xFFFF;
    r0 = r29 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_80257FB0;
    r3 = 0x0;
    goto L_80257FB4;
L_80257FB0: ;
    r3 = 0x1;
L_80257FB4: ;
    /* lmw r27, 0x2c(r1) */;
    return;
}
#pragma pop

/* Address: 0x80257FC8 | Size: 0xF4 (244 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80257FC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80219270();
    extern void fn_80236458();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    r5 = r1 + 0x8;
    /* stmw r27, 0x2c(r1) */;
    r29 = r6;
    r27 = r3;
    r28 = r4;
    r6 = 0x0;
    fn_802367CC();
    r31 = r3;
    r3 = r27;
    r4 = r29;
    fn_80236458();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r29;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258030;
    r3 = 0x0;
    goto L_802580A8;
L_80258030: ;
    r3 = r27;
    r4 = r28;
    r5 = 0x10;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80258060;
    r3 = r30;
    fn_80219270();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258068;
L_80258060: ;
    r3 = 0x0;
    goto L_802580A8;
L_80258068: ;
    r5 = r30 & 0xFFFF;
    r4 = r1 + 0x8;
    r0 = r31 & 0xFFFF;
    r6 = 0x0;
    goto L_80258098;
L_8025807C: ;
    /* clrlslwi r3, r6, 16, 1 */;
    r3 = *(u16*)(r4 + r3);
    if ((u32)r5 != (u32)r3) goto L_80258094;
    r3 = 0x0;
    goto L_802580A8;
L_80258094: ;
    r6 = r6 + 0x1;
L_80258098: ;
    r3 = r6 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8025807C;
    r3 = 0x1;
L_802580A8: ;
    /* lmw r27, 0x2c(r1) */;
    return;
}
#pragma pop


/* -------------------------------------------------------------------
 * Team State Updates (0x80258000-0x8025C000)
 * 78 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802580BC | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802580BC(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = (0x1 << 16);
    /* subi r7, r7, 0x1 */;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258108;
    r3 = 0x0;
    goto L_80258120;
L_80258108: ;
    if ((s32)r31 != (s32)0x0) goto L_80258118;
    r3 = 0x0;
    goto L_80258120;
L_80258118: ;
    r3 = 0x1;
L_80258120: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025813C | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025813C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80258190;
    r3 = 0x0;
    goto L_802581F8;
L_80258190: ;
    r3 = r27;
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802581E0;
    r31 = 0x0;
L_802581E0: ;
    if ((s32)r31 != (s32)0x0) goto L_802581F0;
    r3 = 0x0;
    goto L_802581F8;
L_802581F0: ;
    r3 = 0x1;
L_802581F8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025820C | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025820C(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = (0x1 << 16);
    /* subi r7, r7, 0x1 */;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258258;
    r3 = 0x0;
    goto L_80258270;
L_80258258: ;
    if ((s32)r31 != (s32)0x0) goto L_80258268;
    r3 = 0x0;
    goto L_80258270;
L_80258268: ;
    r3 = 0x1;
L_80258270: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258284 | Size: 0x140 (320 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258284(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80219804();
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_802381C4();
    extern void fn_8025C264();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    /* stmw r26, 0x38(r1) */;
    r31 = r6;
    r26 = r4;
    r29 = r5;
    r30 = r3;
    r4 = r31;
    r5 = r1 + 0x1c;
    r6 = r1 + 0x8;
    fn_802367CC();
    r28 = r3;
    r3 = r30;
    r4 = r31;
    fn_802364BC();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    r4 = r26;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    fn_8025C264();
    r29 = r3;
    r3 = r27;
    fn_80219804();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258308;
    r3 = 0x0;
    goto L_802583B0;
L_80258308: ;
    r7 = r1 + 0x8;
    r6 = r27 & 0xFFFF;
    r5 = r1 + 0x1c;
    r0 = r28 & 0xFFFF;
    r3 = 0x0;
    r10 = 0x0;
    goto L_80258358;
L_80258324: ;
    /* clrlslwi r8, r10, 16, 1 */;
    r9 = *(s16*)(r7 + r8);
    r4 = (s16)r9;
    if ((u32)r0 < (u32)0x1) goto L_80258354;
    r4 = *(u16*)(r5 + r8);
    if ((u32)r6 != (u32)r4) goto L_80258354;
    r3 = r30;
    r4 = r31;
    r5 = r9 & 0xFF;
    fn_802381C4();
    goto L_80258364;
L_80258354: ;
    r10 = r10 + 0x1;
L_80258358: ;
    r4 = r10 & 0xFFFF;
    if ((u32)r4 < (u32)r0) goto L_80258324;
L_80258364: ;
    r0 = r3 & 0xFF;
    if ((u32)r4 != (u32)r0) goto L_80258374;
    r3 = 0x0;
    goto L_802583B0;
L_80258374: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x2a;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258398;
    r3 = 0x0;
    goto L_802583B0;
L_80258398: ;
    if ((s32)r29 != (s32)0x0) goto L_802583A8;
    r3 = 0x0;
    goto L_802583B0;
L_802583A8: ;
    r3 = 0x1;
L_802583B0: ;
    /* lmw r26, 0x38(r1) */;
    return;
}
#pragma pop

/* Address: 0x802583C4 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802583C4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258448;
    r31 = 0x0;
L_80258448: ;
    if ((s32)r31 != (s32)0x0) goto L_80258458;
    r3 = 0x0;
    goto L_80258460;
L_80258458: ;
    r3 = 0x1;
L_80258460: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258474 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258474(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802584F8;
    r31 = 0x0;
L_802584F8: ;
    if ((s32)r31 != (s32)0x0) goto L_80258508;
    r3 = 0x0;
    goto L_80258510;
L_80258508: ;
    r3 = 0x1;
L_80258510: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258524 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258524(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802585A8;
    r31 = 0x0;
L_802585A8: ;
    if ((s32)r31 != (s32)0x0) goto L_802585B8;
    r3 = 0x0;
    goto L_802585C0;
L_802585B8: ;
    r3 = 0x1;
L_802585C0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802585D4 | Size: 0x120 (288 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802585D4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_802381C4();
    extern void fn_8025C264();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    /* stmw r26, 0x38(r1) */;
    r31 = r6;
    r26 = r4;
    r29 = r5;
    r30 = r3;
    r4 = r31;
    r5 = r1 + 0x1c;
    r6 = r1 + 0x8;
    fn_802367CC();
    r27 = r3;
    r3 = r30;
    r4 = r31;
    fn_802364BC();
    r28 = r3;
    r3 = r30;
    r4 = r26;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    fn_8025C264();
    r29 = r3;
    r7 = r1 + 0x8;
    r6 = r28 & 0xFFFF;
    r5 = r1 + 0x1c;
    r0 = r27 & 0xFFFF;
    r3 = 0x0;
    r10 = 0x0;
    goto L_80258688;
L_80258654: ;
    /* clrlslwi r8, r10, 16, 1 */;
    r9 = *(s16*)(r7 + r8);
    r4 = (s16)r9;
    if ((s32)r0 < (s32)0) goto L_80258684;
    r4 = *(u16*)(r5 + r8);
    if ((u32)r6 != (u32)r4) goto L_80258684;
    r3 = r30;
    r4 = r31;
    r5 = r9 & 0xFF;
    fn_802381C4();
    goto L_80258694;
L_80258684: ;
    r10 = r10 + 0x1;
L_80258688: ;
    r4 = r10 & 0xFFFF;
    if ((u32)r4 < (u32)r0) goto L_80258654;
L_80258694: ;
    r0 = r3 & 0xFF;
    if ((u32)r4 != (u32)r0) goto L_802586A4;
    r3 = 0x0;
    goto L_802586E0;
L_802586A4: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x29;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802586C8;
    r3 = 0x0;
    goto L_802586E0;
L_802586C8: ;
    if ((s32)r29 != (s32)0x0) goto L_802586D8;
    r3 = 0x0;
    goto L_802586E0;
L_802586D8: ;
    r3 = 0x1;
L_802586E0: ;
    /* lmw r26, 0x38(r1) */;
    return;
}
#pragma pop

/* Address: 0x802586FC | Size: 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802586FC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80236BFC();
    extern void fn_80237DBC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258744;
    r3 = 0x0;
    goto L_802587A4;
L_80258744: ;
    r3 = r29;
    r4 = r30;
    r5 = 0xc;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258768;
    r3 = 0x0;
    goto L_802587A4;
L_80258768: ;
    r3 = r29;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025878C;
    r3 = 0x0;
    goto L_802587A4;
L_8025878C: ;
    if ((s32)r31 != (s32)0x0) goto L_8025879C;
    r3 = 0x0;
    goto L_802587A4;
L_8025879C: ;
    r3 = 0x1;
L_802587A4: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x802587C0 | Size: 0x144 (324 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802587C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021A2C0();
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r7 = 0x0;
    /* stmw r26, 0x28(r1) */;
    r28 = r5;
    r31 = r6;
    r26 = r3;
    r27 = r4;
    r5 = r1 + 0x8;
    r6 = 0x0;
    fn_802367CC();
    r30 = r3;
    r3 = r26;
    r4 = r31;
    fn_802364BC();
    r0 = r3;
    r3 = r26;
    r29 = r0;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025882C;
    r3 = 0x0;
    goto L_802588F0;
L_8025882C: ;
    r7 = (0x1 << 16);
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = r31;
    /* subi r7, r7, 0x1 */;
    fn_8025C264();
    r31 = r3;
    r3 = r26;
    r4 = r27;
    r5 = 0x10;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80258898;
    r3 = r29;
    fn_8021A2C0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258898;
    r5 = r29 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80258898;
    if ((u32)r5 == (u32)0xffff) goto L_80258898;
    if ((u32)r5 == (u32)0x165) goto L_80258898;
    if ((u32)r5 != (u32)0x163) goto L_802588A0;
L_80258898: ;
    r3 = 0x0;
    goto L_802588F0;
L_802588A0: ;
    r4 = r1 + 0x8;
    r0 = r30 & 0xFFFF;
    r6 = 0x0;
    goto L_802588CC;
L_802588B0: ;
    /* clrlslwi r3, r6, 16, 1 */;
    r3 = *(u16*)(r4 + r3);
    if ((u32)r5 != (u32)r3) goto L_802588C8;
    r3 = 0x0;
    goto L_802588F0;
L_802588C8: ;
    r6 = r6 + 0x1;
L_802588CC: ;
    r3 = r6 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_802588B0;
    if ((s32)r31 != (s32)0x0) goto L_802588E8;
    r3 = 0x0;
    goto L_802588F0;
L_802588E8: ;
    r3 = 0x1;
L_802588F0: ;
    /* lmw r26, 0x28(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258904 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258904(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258988;
    r31 = 0x0;
L_80258988: ;
    if ((s32)r31 != (s32)0x0) goto L_80258998;
    r3 = 0x0;
    goto L_802589A0;
L_80258998: ;
    r3 = 0x1;
L_802589A0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802589B4 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802589B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258A38;
    r31 = 0x0;
L_80258A38: ;
    if ((s32)r31 != (s32)0x0) goto L_80258A48;
    r3 = 0x0;
    goto L_80258A50;
L_80258A48: ;
    r3 = 0x1;
L_80258A50: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258A64 | Size: 0x6C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80258A64(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E650[];
    extern void fn_80236BFC();
    extern void fn_802373B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r5 = 0x14;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80258A98;
    r3 = 0x0;
    goto L_80258ABC;
L_80258A98: ;
    f1 = *(f32*)lbl_8047E650;
    r3 = r30;
    r4 = r31;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    r3 = 0x1 - r0;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_80258ABC: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258AD0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258AD0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258B4C;
    r30 = 0x0;
L_80258B4C: ;
    if ((s32)r30 != (s32)0x0) goto L_80258B5C;
    r3 = 0x0;
    goto L_80258B70;
L_80258B5C: ;
    if ((s32)r30 != (s32)-0x1) goto L_80258B6C;
    r3 = 0x1;
    goto L_80258B70;
L_80258B6C: ;
    r3 = 0x1;
L_80258B70: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258B84 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258B84(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258C00;
    r30 = 0x0;
L_80258C00: ;
    if ((s32)r30 != (s32)0x0) goto L_80258C10;
    r3 = 0x0;
    goto L_80258C24;
L_80258C10: ;
    if ((s32)r30 != (s32)-0x1) goto L_80258C20;
    r3 = 0x1;
    goto L_80258C24;
L_80258C20: ;
    r3 = 0x1;
L_80258C24: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258C38 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258C38(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258CB4;
    r30 = 0x0;
L_80258CB4: ;
    if ((s32)r30 != (s32)0x0) goto L_80258CC4;
    r3 = 0x0;
    goto L_80258CD8;
L_80258CC4: ;
    if ((s32)r30 != (s32)-0x1) goto L_80258CD4;
    r3 = 0x1;
    goto L_80258CD8;
L_80258CD4: ;
    r3 = 0x1;
L_80258CD8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258CEC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258CEC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258D68;
    r30 = 0x0;
L_80258D68: ;
    if ((s32)r30 != (s32)0x0) goto L_80258D78;
    r3 = 0x0;
    goto L_80258D8C;
L_80258D78: ;
    if ((s32)r30 != (s32)-0x1) goto L_80258D88;
    r3 = 0x1;
    goto L_80258D8C;
L_80258D88: ;
    r3 = 0x1;
L_80258D8C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258DA0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258DA0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258E1C;
    r30 = 0x0;
L_80258E1C: ;
    if ((s32)r30 != (s32)0x0) goto L_80258E2C;
    r3 = 0x0;
    goto L_80258E40;
L_80258E2C: ;
    if ((s32)r30 != (s32)-0x1) goto L_80258E3C;
    r3 = 0x1;
    goto L_80258E40;
L_80258E3C: ;
    r3 = 0x1;
L_80258E40: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258E54 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258E54(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258ED0;
    r30 = 0x0;
L_80258ED0: ;
    if ((s32)r30 != (s32)0x0) goto L_80258EE0;
    r3 = 0x0;
    goto L_80258EF4;
L_80258EE0: ;
    if ((s32)r30 != (s32)-0x1) goto L_80258EF0;
    r3 = 0x1;
    goto L_80258EF4;
L_80258EF0: ;
    r3 = 0x1;
L_80258EF4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258F08 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258F08(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80258F84;
    r30 = 0x0;
L_80258F84: ;
    if ((s32)r30 != (s32)0x0) goto L_80258F94;
    r3 = 0x0;
    goto L_80258FA8;
L_80258F94: ;
    if ((s32)r30 != (s32)-0x1) goto L_80258FA4;
    r3 = 0x1;
    goto L_80258FA8;
L_80258FA4: ;
    r3 = 0x1;
L_80258FA8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80258FBC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80258FBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80259038;
    r30 = 0x0;
L_80259038: ;
    if ((s32)r30 != (s32)0x0) goto L_80259048;
    r3 = 0x0;
    goto L_8025905C;
L_80259048: ;
    if ((s32)r30 != (s32)-0x1) goto L_80259058;
    r3 = 0x1;
    goto L_8025905C;
L_80259058: ;
    r3 = 0x1;
L_8025905C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259070 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259070(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802590EC;
    r30 = 0x0;
L_802590EC: ;
    if ((s32)r30 != (s32)0x0) goto L_802590FC;
    r3 = 0x0;
    goto L_80259110;
L_802590FC: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025910C;
    r3 = 0x1;
    goto L_80259110;
L_8025910C: ;
    r3 = 0x1;
L_80259110: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259124 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259124(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_802591A0;
    r30 = 0x0;
L_802591A0: ;
    if ((s32)r30 != (s32)0x0) goto L_802591B0;
    r3 = 0x0;
    goto L_802591C4;
L_802591B0: ;
    if ((s32)r30 != (s32)-0x1) goto L_802591C0;
    r3 = 0x1;
    goto L_802591C4;
L_802591C0: ;
    r3 = 0x1;
L_802591C4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802591D8 | Size: 0x260 (608 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802591D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_8023793C();
    extern void fn_80237F74();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
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
    if ((u32)r0 != (u32)0x1) goto L_80259248;
    r3 = 0x0;
    goto L_80259424;
L_80259248: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259324;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259324;
    r3 = r27;
    r4 = r30;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802592A4;
    r0 = 0x0;
    goto L_80259328;
L_802592A4: ;
    r3 = r27;
    r4 = r30;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259324;
    r3 = r27;
    r4 = r30;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259324;
    r3 = r27;
    r4 = r30;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259324;
    r3 = r27;
    r4 = r30;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259324;
    r3 = r27;
    r4 = r30;
    r5 = 0xc;
    fn_80237F74();
L_80259324: ;
    r0 = 0x1;
L_80259328: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259338;
    r3 = 0x0;
    goto L_80259424;
L_80259338: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025935C;
    r3 = 0x0;
    goto L_80259424;
L_8025935C: ;
    r3 = r27;
    r4 = r29;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r26;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_80259390;
    r3 = 0x0;
    goto L_80259424;
L_80259390: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802593B4;
    r3 = 0x0;
    goto L_80259424;
L_802593B4: ;
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802593D0;
    r3 = 0x0;
    goto L_80259424;
L_802593D0: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r27 = r3;
    r3 = r31;
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025940C;
    r3 = 0x0;
    goto L_80259424;
L_8025940C: ;
    if ((s32)r27 != (s32)0x0) goto L_8025941C;
    r3 = 0x0;
    goto L_80259424;
L_8025941C: ;
    r3 = 0x1;
L_80259424: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259438 | Size: 0x270 (624 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259438(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237DBC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r31 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x11;
    r4 = r31;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259478;
    r3 = 0x0;
    goto L_80259694;
L_80259478: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025949C;
    r3 = 0x0;
    goto L_80259694;
L_8025949C: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802594C0;
    r3 = 0x0;
    goto L_80259694;
L_802594C0: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802594E4;
    r3 = 0x0;
    goto L_80259694;
L_802594E4: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259508;
    r3 = 0x0;
    goto L_80259694;
L_80259508: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x8;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025952C;
    r3 = 0x0;
    goto L_80259694;
L_8025952C: ;
    r3 = r28;
    r4 = r31;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259548;
    r3 = 0x0;
    goto L_80259694;
L_80259548: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025956C;
    r0 = 0x0;
    goto L_80259628;
L_8025956C: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259624;
    r3 = r28;
    r4 = r31;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259624;
    r3 = r28;
    r4 = r31;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259624;
    r3 = r28;
    r4 = r31;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259624;
    r3 = r28;
    r4 = r31;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259624;
    r3 = r28;
    r4 = r31;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259624;
    r3 = r28;
    r4 = r31;
    r5 = 0xc;
    fn_80237F74();
L_80259624: ;
    r0 = 0x1;
L_80259628: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259638;
    r3 = 0x0;
    goto L_80259694;
L_80259638: ;
    r3 = r28;
    r4 = r29;
    r5 = r30;
    r6 = r31;
    r7 = 0x0;
    fn_8025C264();
    r28 = r3;
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025967C;
    r3 = 0x0;
    goto L_80259694;
L_8025967C: ;
    if ((s32)r28 != (s32)0x0) goto L_8025968C;
    r3 = 0x0;
    goto L_80259694;
L_8025968C: ;
    r3 = 0x1;
L_80259694: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802596A8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802596A8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802596E4 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802596E4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259754;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259738;
    r3 = 0x0;
    goto L_802597B8;
L_80259738: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259754: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x5;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259784;
    r3 = 0x0;
    goto L_802597B8;
L_80259784: ;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_802597B4;
    if ((s32)r31 != (s32)0x0) goto L_802597A4;
    r3 = 0x0;
    goto L_802597B8;
L_802597A4: ;
    if ((s32)r31 != (s32)-0x1) goto L_802597B4;
    r3 = 0x1;
    goto L_802597B8;
L_802597B4: ;
    r3 = 0x1;
L_802597B8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802597CC | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802597CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025983C;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259820;
    r3 = 0x0;
    goto L_802598A0;
L_80259820: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025983C: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x3;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025986C;
    r3 = 0x0;
    goto L_802598A0;
L_8025986C: ;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025989C;
    if ((s32)r31 != (s32)0x0) goto L_8025988C;
    r3 = 0x0;
    goto L_802598A0;
L_8025988C: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025989C;
    r3 = 0x1;
    goto L_802598A0;
L_8025989C: ;
    r3 = 0x1;
L_802598A0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802598B4 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802598B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259924;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259908;
    r3 = 0x0;
    goto L_80259988;
L_80259908: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259924: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x2;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259954;
    r3 = 0x0;
    goto L_80259988;
L_80259954: ;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80259984;
    if ((s32)r31 != (s32)0x0) goto L_80259974;
    r3 = 0x0;
    goto L_80259988;
L_80259974: ;
    if ((s32)r31 != (s32)-0x1) goto L_80259984;
    r3 = 0x1;
    goto L_80259988;
L_80259984: ;
    r3 = 0x1;
L_80259988: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025999C | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025999C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259A0C;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802599F0;
    r3 = 0x0;
    goto L_80259A70;
L_802599F0: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259A0C: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259A3C;
    r3 = 0x0;
    goto L_80259A70;
L_80259A3C: ;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80259A6C;
    if ((s32)r31 != (s32)0x0) goto L_80259A5C;
    r3 = 0x0;
    goto L_80259A70;
L_80259A5C: ;
    if ((s32)r31 != (s32)-0x1) goto L_80259A6C;
    r3 = 0x1;
    goto L_80259A70;
L_80259A6C: ;
    r3 = 0x1;
L_80259A70: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259A84 | Size: 0x68 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80259A84(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_80237288();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x10;
    /* stmw r30, 0x8(r1) */;
    r31 = r6;
    r30 = r3;
    r4 = r31;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80259ACC;
    r3 = r30;
    r4 = r31;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259AD4;
L_80259ACC: ;
    r3 = 0x0;
    goto L_80259AD8;
L_80259AD4: ;
    r3 = 0x1;
L_80259AD8: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259AEC | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259AEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259B5C;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259B40;
    r3 = 0x0;
    goto L_80259BC0;
L_80259B40: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259B5C: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x5;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259B8C;
    r3 = 0x0;
    goto L_80259BC0;
L_80259B8C: ;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80259BBC;
    if ((s32)r31 != (s32)0x0) goto L_80259BAC;
    r3 = 0x0;
    goto L_80259BC0;
L_80259BAC: ;
    if ((s32)r31 != (s32)-0x1) goto L_80259BBC;
    r3 = 0x1;
    goto L_80259BC0;
L_80259BBC: ;
    r3 = 0x1;
L_80259BC0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259BD4 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259BD4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259C44;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259C28;
    r3 = 0x0;
    goto L_80259CA8;
L_80259C28: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259C44: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x4;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259C74;
    r3 = 0x0;
    goto L_80259CA8;
L_80259C74: ;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80259CA4;
    if ((s32)r31 != (s32)0x0) goto L_80259C94;
    r3 = 0x0;
    goto L_80259CA8;
L_80259C94: ;
    if ((s32)r31 != (s32)-0x1) goto L_80259CA4;
    r3 = 0x1;
    goto L_80259CA8;
L_80259CA4: ;
    r3 = 0x1;
L_80259CA8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259CBC | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259CBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259D2C;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259D10;
    r3 = 0x0;
    goto L_80259D90;
L_80259D10: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259D2C: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x3;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259D5C;
    r3 = 0x0;
    goto L_80259D90;
L_80259D5C: ;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80259D8C;
    if ((s32)r31 != (s32)0x0) goto L_80259D7C;
    r3 = 0x0;
    goto L_80259D90;
L_80259D7C: ;
    if ((s32)r31 != (s32)-0x1) goto L_80259D8C;
    r3 = 0x1;
    goto L_80259D90;
L_80259D8C: ;
    r3 = 0x1;
L_80259D90: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259DA4 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259DA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259E14;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259DF8;
    r3 = 0x0;
    goto L_80259E78;
L_80259DF8: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259E14: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259E44;
    r3 = 0x0;
    goto L_80259E78;
L_80259E44: ;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80259E74;
    if ((s32)r31 != (s32)0x0) goto L_80259E64;
    r3 = 0x0;
    goto L_80259E78;
L_80259E64: ;
    if ((s32)r31 != (s32)-0x1) goto L_80259E74;
    r3 = 0x1;
    goto L_80259E78;
L_80259E74: ;
    r3 = 0x1;
L_80259E78: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259E8C | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259E8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_80259EFC;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259EE0;
    r3 = 0x0;
    goto L_80259F60;
L_80259EE0: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_80259EFC: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259F2C;
    r3 = 0x0;
    goto L_80259F60;
L_80259F2C: ;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_80259F5C;
    if ((s32)r31 != (s32)0x0) goto L_80259F4C;
    r3 = 0x0;
    goto L_80259F60;
L_80259F4C: ;
    if ((s32)r31 != (s32)-0x1) goto L_80259F5C;
    r3 = 0x1;
    goto L_80259F60;
L_80259F5C: ;
    r3 = 0x1;
L_80259F60: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80259F74 | Size: 0x1F8 (504 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80259F74(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    if ((u32)r0 != (u32)0x1) goto L_80259FCC;
    r3 = 0x0;
    goto L_8025A158;
L_80259FCC: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80259FF0;
    r3 = 0x0;
    goto L_8025A158;
L_80259FF0: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025A014;
    r3 = 0x0;
    goto L_8025A158;
L_8025A014: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025A0F0;
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025A054;
    r0 = 0x0;
    goto L_8025A0F4;
L_8025A054: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025A0F0;
    r3 = r27;
    r4 = r31;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025A0F0;
    r3 = r27;
    r4 = r31;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025A0F0;
    r3 = r27;
    r4 = r31;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025A0F0;
    r3 = r27;
    r4 = r31;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025A0F0;
    r3 = r27;
    r4 = r31;
    r5 = 0xc;
    fn_80237F74();
L_8025A0F0: ;
    r0 = 0x1;
L_8025A0F4: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025A104;
    r3 = 0x0;
    goto L_8025A158;
L_8025A104: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r31;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
    r3 = r30;
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025A140;
    r3 = 0x0;
    goto L_8025A158;
L_8025A140: ;
    if ((s32)r31 != (s32)0x0) goto L_8025A150;
    r3 = 0x0;
    goto L_8025A158;
L_8025A150: ;
    r3 = 0x1;
L_8025A158: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A16C | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A16C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A1E8;
    r30 = 0x0;
L_8025A1E8: ;
    if ((s32)r30 != (s32)0x0) goto L_8025A1F8;
    r3 = 0x0;
    goto L_8025A20C;
L_8025A1F8: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025A208;
    r3 = 0x1;
    goto L_8025A20C;
L_8025A208: ;
    r3 = 0x1;
L_8025A20C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A220 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8025A220(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A254 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025A254(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A340 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A340(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A3BC;
    r30 = 0x0;
L_8025A3BC: ;
    if ((s32)r30 != (s32)0x0) goto L_8025A3CC;
    r3 = 0x0;
    goto L_8025A3E0;
L_8025A3CC: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025A3DC;
    r3 = 0x1;
    goto L_8025A3E0;
L_8025A3DC: ;
    r3 = 0x1;
L_8025A3E0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A3F4 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A3F4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A470;
    r30 = 0x0;
L_8025A470: ;
    if ((s32)r30 != (s32)0x0) goto L_8025A480;
    r3 = 0x0;
    goto L_8025A494;
L_8025A480: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025A490;
    r3 = 0x1;
    goto L_8025A494;
L_8025A490: ;
    r3 = 0x1;
L_8025A494: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A4A8 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A4A8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A524;
    r30 = 0x0;
L_8025A524: ;
    if ((s32)r30 != (s32)0x0) goto L_8025A534;
    r3 = 0x0;
    goto L_8025A548;
L_8025A534: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025A544;
    r3 = 0x1;
    goto L_8025A548;
L_8025A544: ;
    r3 = 0x1;
L_8025A548: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A55C | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A55C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A5E0;
    r31 = 0x0;
L_8025A5E0: ;
    if ((s32)r31 != (s32)0x0) goto L_8025A5F0;
    r3 = 0x0;
    goto L_8025A5F8;
L_8025A5F0: ;
    r3 = 0x1;
L_8025A5F8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A60C | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A60C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A690;
    r31 = 0x0;
L_8025A690: ;
    if ((s32)r31 != (s32)0x0) goto L_8025A6A0;
    r3 = 0x0;
    goto L_8025A6A8;
L_8025A6A0: ;
    r3 = 0x1;
L_8025A6A8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A6BC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A6BC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A738;
    r30 = 0x0;
L_8025A738: ;
    if ((s32)r30 != (s32)0x0) goto L_8025A748;
    r3 = 0x0;
    goto L_8025A75C;
L_8025A748: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025A758;
    r3 = 0x1;
    goto L_8025A75C;
L_8025A758: ;
    r3 = 0x1;
L_8025A75C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A770 | Size: 0x100 (256 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A770(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80237774();
    extern void fn_8023793C();
    extern void fn_80237F74();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r29 = r4;
    r26 = r5;
    r27 = r6;
    fn_80237774();
    r30 = r3;
    r3 = r25;
    r4 = r27;
    fn_80237774();
    r31 = r3;
    r3 = r25;
    r4 = r26;
    r5 = r29;
    fn_802395C8();
    r7 = (0x1 << 16);
    r28 = r3;
    r3 = r25;
    r4 = r29;
    r5 = r26;
    r6 = r27;
    /* subi r7, r7, 0x1 */;
    fn_8025C264();
    r0 = r3;
    r3 = r25;
    r29 = r0;
    r4 = r26;
    fn_80239500();
    r6 = r3;
    r3 = r25;
    r4 = r27;
    r5 = r28;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A810;
    r29 = 0x0;
L_8025A810: ;
    r3 = r25;
    r4 = r27;
    r5 = 0x5;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025A830;
    r29 = 0x0;
L_8025A830: ;
    r3 = r30 & 0xFF;
    r0 = r31 & 0xFF;
    if ((u32)r3 >= (u32)r0) goto L_8025A844;
    r29 = 0x0;
L_8025A844: ;
    if ((s32)r29 != (s32)0x0) goto L_8025A854;
    r3 = 0x0;
    goto L_8025A85C;
L_8025A854: ;
    r3 = 0x1;
L_8025A85C: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A870 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A870(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8047E648[];
    extern void fn_80236BFC();
    extern void fn_802373B0();
    extern void fn_8025CC90();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r5 = 0x8;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025A8A4;
    r3 = 0x0;
    goto L_8025A8E8;
L_8025A8A4: ;
    r3 = r30;
    r4 = r31;
    fn_8025CC90();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025A8C4;
    r3 = 0x0;
    goto L_8025A8E8;
L_8025A8C4: ;
    f1 = *(f32*)lbl_8047E648;
    r3 = r30;
    r4 = r31;
    r5 = 0x0;
    fn_802373B0();
    r0 = r3 & 0xFF;
    r3 = 0x1 - r0;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_8025A8E8: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A8FC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A8FC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025A978;
    r30 = 0x0;
L_8025A978: ;
    if ((s32)r30 != (s32)0x0) goto L_8025A988;
    r3 = 0x0;
    goto L_8025A99C;
L_8025A988: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025A998;
    r3 = 0x1;
    goto L_8025A99C;
L_8025A998: ;
    r3 = 0x1;
L_8025A99C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025A9B0 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025A9B0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A9EC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025A9EC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025AA68;
    r30 = 0x0;
L_8025AA68: ;
    if ((s32)r30 != (s32)0x0) goto L_8025AA78;
    r3 = 0x0;
    goto L_8025AA8C;
L_8025AA78: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025AA88;
    r3 = 0x1;
    goto L_8025AA8C;
L_8025AA88: ;
    r3 = 0x1;
L_8025AA8C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025AAA0 | Size: 0x270 (624 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025AAA0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237DBC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r31 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x11;
    r4 = r31;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AAE0;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025AAE0: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AB04;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025AB04: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AB28;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025AB28: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AB4C;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025AB4C: ;
    r3 = r28;
    r4 = r31;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AB68;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025AB68: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AB8C;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025AB8C: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x8;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025ABB0;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025ABB0: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025ABD4;
    r0 = 0x0;
    goto L_8025AC90;
L_8025ABD4: ;
    r3 = r28;
    r4 = r31;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025AC8C;
    r3 = r28;
    r4 = r31;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025AC8C;
    r3 = r28;
    r4 = r31;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025AC8C;
    r3 = r28;
    r4 = r31;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025AC8C;
    r3 = r28;
    r4 = r31;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025AC8C;
    r3 = r28;
    r4 = r31;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025AC8C;
    r3 = r28;
    r4 = r31;
    r5 = 0xc;
    fn_80237F74();
L_8025AC8C: ;
    r0 = 0x1;
L_8025AC90: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025ACA0;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025ACA0: ;
    r3 = r28;
    r4 = r29;
    r5 = r30;
    r6 = r31;
    r7 = 0x0;
    fn_8025C264();
    r28 = r3;
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025ACE4;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025ACE4: ;
    if ((s32)r28 != (s32)0x0) goto L_8025ACF4;
    r3 = 0x0;
    goto L_8025ACFC;
L_8025ACF4: ;
    r3 = 0x1;
L_8025ACFC: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025AD10 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025AD10(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025AD48 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025AD48(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025ADC4;
    r30 = 0x0;
L_8025ADC4: ;
    if ((s32)r30 != (s32)0x0) goto L_8025ADD4;
    r3 = 0x0;
    goto L_8025ADE8;
L_8025ADD4: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025ADE4;
    r3 = 0x1;
    goto L_8025ADE8;
L_8025ADE4: ;
    r3 = 0x1;
L_8025ADE8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025ADFC | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_8025ADFC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025AE28 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025AE28(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025AEAC;
    r31 = 0x0;
L_8025AEAC: ;
    if ((s32)r31 != (s32)0x0) goto L_8025AEBC;
    r3 = 0x0;
    goto L_8025AEC4;
L_8025AEBC: ;
    r3 = 0x1;
L_8025AEC4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025AED8 | Size: 0xE0 (224 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025AED8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F4354();
    extern void fn_801F87CC();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0x2c(r1) */;
    r27 = r3;
    r30 = r6;
    r28 = r4;
    r29 = r5;
    r3 = 0x0;
    r4 = r30;
    fn_801F4354();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r30;
    r5 = 0x15;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AF30;
    r3 = 0x0;
    goto L_8025AFA4;
L_8025AF30: ;
    r3 = r27;
    r4 = r30;
    r5 = 0x25;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AF54;
    r3 = 0x0;
    goto L_8025AFA4;
L_8025AF54: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    r4 = r1 + 0x8;
    fn_801F87CC();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8025AF8C;
    r31 = 0x0;
L_8025AF8C: ;
    if ((s32)r31 != (s32)0x0) goto L_8025AF9C;
    r3 = 0x0;
    goto L_8025AFA4;
L_8025AF9C: ;
    r3 = 0x1;
L_8025AFA4: ;
    /* lmw r27, 0x2c(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025AFB8 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025AFB8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025B03C;
    r31 = 0x0;
L_8025B03C: ;
    if ((s32)r31 != (s32)0x0) goto L_8025B04C;
    r3 = 0x0;
    goto L_8025B054;
L_8025B04C: ;
    r3 = 0x1;
L_8025B054: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B068 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B068(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025B0E4;
    r30 = 0x0;
L_8025B0E4: ;
    if ((s32)r30 != (s32)0x0) goto L_8025B0F4;
    r3 = 0x0;
    goto L_8025B108;
L_8025B0F4: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025B104;
    r3 = 0x1;
    goto L_8025B108;
L_8025B104: ;
    r3 = 0x1;
L_8025B108: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B124 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B124(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B194;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B178;
    r3 = 0x0;
    goto L_8025B1F8;
L_8025B178: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B194: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x7;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B1C4;
    r3 = 0x0;
    goto L_8025B1F8;
L_8025B1C4: ;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B1F4;
    if ((s32)r31 != (s32)0x0) goto L_8025B1E4;
    r3 = 0x0;
    goto L_8025B1F8;
L_8025B1E4: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B1F4;
    r3 = 0x1;
    goto L_8025B1F8;
L_8025B1F4: ;
    r3 = 0x1;
L_8025B1F8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B20C | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B20C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B27C;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B260;
    r3 = 0x0;
    goto L_8025B2E0;
L_8025B260: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B27C: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x6;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B2AC;
    r3 = 0x0;
    goto L_8025B2E0;
L_8025B2AC: ;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B2DC;
    if ((s32)r31 != (s32)0x0) goto L_8025B2CC;
    r3 = 0x0;
    goto L_8025B2E0;
L_8025B2CC: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B2DC;
    r3 = 0x1;
    goto L_8025B2E0;
L_8025B2DC: ;
    r3 = 0x1;
L_8025B2E0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B2F4 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B2F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B364;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B348;
    r3 = 0x0;
    goto L_8025B3C8;
L_8025B348: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B364: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x3;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B394;
    r3 = 0x0;
    goto L_8025B3C8;
L_8025B394: ;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B3C4;
    if ((s32)r31 != (s32)0x0) goto L_8025B3B4;
    r3 = 0x0;
    goto L_8025B3C8;
L_8025B3B4: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B3C4;
    r3 = 0x1;
    goto L_8025B3C8;
L_8025B3C4: ;
    r3 = 0x1;
L_8025B3C8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B3DC | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B3DC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B44C;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B430;
    r3 = 0x0;
    goto L_8025B4B0;
L_8025B430: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B44C: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x2;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B47C;
    r3 = 0x0;
    goto L_8025B4B0;
L_8025B47C: ;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B4AC;
    if ((s32)r31 != (s32)0x0) goto L_8025B49C;
    r3 = 0x0;
    goto L_8025B4B0;
L_8025B49C: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B4AC;
    r3 = 0x1;
    goto L_8025B4B0;
L_8025B4AC: ;
    r3 = 0x1;
L_8025B4B0: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B4C4 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B4C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B534;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B518;
    r3 = 0x0;
    goto L_8025B598;
L_8025B518: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B534: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B564;
    r3 = 0x0;
    goto L_8025B598;
L_8025B564: ;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B594;
    if ((s32)r31 != (s32)0x0) goto L_8025B584;
    r3 = 0x0;
    goto L_8025B598;
L_8025B584: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B594;
    r3 = 0x1;
    goto L_8025B598;
L_8025B594: ;
    r3 = 0x1;
L_8025B598: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B5AC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B5AC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025B628;
    r30 = 0x0;
L_8025B628: ;
    if ((s32)r30 != (s32)0x0) goto L_8025B638;
    r3 = 0x0;
    goto L_8025B64C;
L_8025B638: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025B648;
    r3 = 0x1;
    goto L_8025B64C;
L_8025B648: ;
    r3 = 0x1;
L_8025B64C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B660 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B660(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B6D0;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B6B4;
    r3 = 0x0;
    goto L_8025B734;
L_8025B6B4: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B6D0: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x7;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B700;
    r3 = 0x0;
    goto L_8025B734;
L_8025B700: ;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B730;
    if ((s32)r31 != (s32)0x0) goto L_8025B720;
    r3 = 0x0;
    goto L_8025B734;
L_8025B720: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B730;
    r3 = 0x1;
    goto L_8025B734;
L_8025B730: ;
    r3 = 0x1;
L_8025B734: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B748 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B748(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B7B8;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B79C;
    r3 = 0x0;
    goto L_8025B81C;
L_8025B79C: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B7B8: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x4;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B7E8;
    r3 = 0x0;
    goto L_8025B81C;
L_8025B7E8: ;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B818;
    if ((s32)r31 != (s32)0x0) goto L_8025B808;
    r3 = 0x0;
    goto L_8025B81C;
L_8025B808: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B818;
    r3 = 0x1;
    goto L_8025B81C;
L_8025B818: ;
    r3 = 0x1;
L_8025B81C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B830 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B830(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B8A0;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B884;
    r3 = 0x0;
    goto L_8025B904;
L_8025B884: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B8A0: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B8D0;
    r3 = 0x0;
    goto L_8025B904;
L_8025B8D0: ;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B900;
    if ((s32)r31 != (s32)0x0) goto L_8025B8F0;
    r3 = 0x0;
    goto L_8025B904;
L_8025B8F0: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B900;
    r3 = 0x1;
    goto L_8025B904;
L_8025B900: ;
    r3 = 0x1;
L_8025B904: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025B918 | Size: 0xE8 (232 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025B918(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025B988;
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B96C;
    r3 = 0x0;
    goto L_8025B9EC;
L_8025B96C: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
L_8025B988: ;
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025B9B8;
    r3 = 0x0;
    goto L_8025B9EC;
L_8025B9B8: ;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 >= (u32)0x1) goto L_8025B9E8;
    if ((s32)r31 != (s32)0x0) goto L_8025B9D8;
    r3 = 0x0;
    goto L_8025B9EC;
L_8025B9D8: ;
    if ((s32)r31 != (s32)-0x1) goto L_8025B9E8;
    r3 = 0x1;
    goto L_8025B9EC;
L_8025B9E8: ;
    r3 = 0x1;
L_8025B9EC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BA00 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_8025BA00(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025BA2C | Size: 0xF4 (244 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BA2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    if ((u32)r0 != (u32)0x1) goto L_8025BA84;
    r3 = 0x0;
    goto L_8025BB0C;
L_8025BA84: ;
    r3 = r27;
    r4 = r29;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025BAA4;
    r3 = 0x0;
    goto L_8025BB0C;
L_8025BAA4: ;
    r3 = r27;
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
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
    if ((u32)r0 != (u32)0x43) goto L_8025BAF4;
    r3 = 0x0;
    goto L_8025BB0C;
L_8025BAF4: ;
    if ((s32)r31 != (s32)0x0) goto L_8025BB04;
    r3 = 0x0;
    goto L_8025BB0C;
L_8025BB04: ;
    r3 = 0x1;
L_8025BB0C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BB20 | Size: 0x108 (264 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BB20(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_8022967C();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    extern void fn_8025CBE8();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x68(r1) */;
    r27 = r4;
    r28 = r5;
    r26 = r3;
    r29 = r6;
    r5 = r27;
    r4 = r28;
    fn_802395C8();
    r30 = r3;
    r4 = r26;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r31 = r3;
    r3 = r28;
    fn_8022967C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025BB94;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 > (u32)0x1) goto L_8025BB94;
    r3 = 0x0;
    goto L_8025BC14;
L_8025BB94: ;
    r3 = r26;
    fn_8025CBE8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025BBB0;
    r3 = 0x0;
    goto L_8025BC14;
L_8025BBB0: ;
    r3 = r26;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r26;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025BBE4;
    r3 = 0x0;
    goto L_8025BC14;
L_8025BBE4: ;
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    if ((s32)r3 != (s32)0x0) goto L_8025BC0C;
    r3 = 0x0;
    goto L_8025BC14;
L_8025BC0C: ;
    r3 = 0x1;
L_8025BC14: ;
    /* lmw r26, 0x68(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BC28 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BC28(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025BCA4;
    r30 = 0x0;
L_8025BCA4: ;
    if ((s32)r30 != (s32)0x0) goto L_8025BCB4;
    r3 = 0x0;
    goto L_8025BCC8;
L_8025BCB4: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025BCC4;
    r3 = 0x1;
    goto L_8025BCC8;
L_8025BCC4: ;
    r3 = 0x1;
L_8025BCC8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BCDC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BCDC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025BD58;
    r30 = 0x0;
L_8025BD58: ;
    if ((s32)r30 != (s32)0x0) goto L_8025BD68;
    r3 = 0x0;
    goto L_8025BD7C;
L_8025BD68: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025BD78;
    r3 = 0x1;
    goto L_8025BD7C;
L_8025BD78: ;
    r3 = 0x1;
L_8025BD7C: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BD90 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BD90(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025BE0C;
    r30 = 0x0;
L_8025BE0C: ;
    if ((s32)r30 != (s32)0x0) goto L_8025BE1C;
    r3 = 0x0;
    goto L_8025BE30;
L_8025BE1C: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025BE2C;
    r3 = 0x1;
    goto L_8025BE30;
L_8025BE2C: ;
    r3 = 0x1;
L_8025BE30: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BE44 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BE44(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025BEC0;
    r30 = 0x0;
L_8025BEC0: ;
    if ((s32)r30 != (s32)0x0) goto L_8025BED0;
    r3 = 0x0;
    goto L_8025BEE4;
L_8025BED0: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025BEE0;
    r3 = 0x1;
    goto L_8025BEE4;
L_8025BEE0: ;
    r3 = 0x1;
L_8025BEE4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BEF8 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BEF8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r4;
    r28 = r5;
    r27 = r3;
    r29 = r6;
    r5 = r30;
    r4 = r28;
    fn_802395C8();
    r31 = r3;
    r3 = r27;
    r4 = r30;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r30 = r3;
    r3 = r27;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r31;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025BF74;
    r30 = 0x0;
L_8025BF74: ;
    if ((s32)r30 != (s32)0x0) goto L_8025BF84;
    r3 = 0x0;
    goto L_8025BF98;
L_8025BF84: ;
    if ((s32)r30 != (s32)-0x1) goto L_8025BF94;
    r3 = 0x1;
    goto L_8025BF98;
L_8025BF94: ;
    r3 = 0x1;
L_8025BF98: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025BFAC | Size: 0x200 (512 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025BFAC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237310();
    extern void fn_80237F74();
    extern void fn_8025C264();
    extern void fn_8025CC90();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r31 = r5;
    r30 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x14;
    r4 = r30;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025BFEC;
    r3 = 0x0;
    goto L_8025C198;
L_8025BFEC: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C010;
    r3 = 0x0;
    goto L_8025C198;
L_8025C010: ;
    r3 = r28;
    r4 = r30;
    fn_8025CC90();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C030;
    r3 = 0x0;
    goto L_8025C198;
L_8025C030: ;
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C04C;
    r3 = 0x0;
    goto L_8025C198;
L_8025C04C: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C128;
    r3 = r28;
    r4 = r30;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C128;
    r3 = r28;
    r4 = r30;
    r5 = 0x7;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C128;
    r3 = r28;
    r4 = r30;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C0D8;
    r3 = r28;
    r4 = r30;
    r5 = 0x48;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C0E0;
L_8025C0D8: ;
    r0 = 0x0;
    goto L_8025C12C;
L_8025C0E0: ;
    r3 = r28;
    r4 = r30;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C128;
    r3 = r28;
    r4 = r30;
    r5 = 0x28;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C128;
    r3 = r28;
    r4 = r30;
    r5 = 0xc;
    fn_80237F74();
L_8025C128: ;
    r0 = 0x1;
L_8025C12C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C13C;
    r3 = 0x0;
    goto L_8025C198;
L_8025C13C: ;
    r3 = r28;
    r4 = r29;
    r5 = r31;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r31 = r3;
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C180;
    r3 = 0x0;
    goto L_8025C198;
L_8025C180: ;
    if ((s32)r31 != (s32)0x0) goto L_8025C190;
    r3 = 0x0;
    goto L_8025C198;
L_8025C190: ;
    r3 = 0x1;
L_8025C198: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop


/* -------------------------------------------------------------------
 * Shadow Pokemon & Purification (0x8025C000-0x80260000)
 * 89 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8025C1AC | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025C1AC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
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
    r4 = r31;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    r4 = r28;
    fn_80239500();
    r6 = r3;
    r3 = r27;
    r4 = r29;
    r5 = r30;
    fn_8023793C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x43) goto L_8025C230;
    r31 = 0x0;
L_8025C230: ;
    if ((s32)r31 != (s32)0x0) goto L_8025C240;
    r3 = 0x0;
    goto L_8025C248;
L_8025C240: ;
    r3 = 0x1;
L_8025C248: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025C264 | Size: 0x340 (832 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025C264(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8011BEB4();
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
    extern void fn_8025CAA8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r24, 0x30(r1) */;
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
    fn_8011BEB4();
    r30 = r3 & 0xFFFF;
    if ((u32)r28 != (u32)0x0) goto L_8025C2E4;
    r3 = 0x1;
    goto L_8025C590;
L_8025C2E4: ;
    r7 = r1 + 0x8;
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F1D5C();
    r24 = r3;
    r4 = r1 + 0x8;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x0;
    fn_801F3BB4();
    r4 = r1 + 0x8;
    r0 = r24 & 0xFFFF;
    r6 = 0x0;
    r7 = 0x0;
    r5 = 0x0;
    goto L_8025C358;
L_8025C32C: ;
    /* clrlslwi r3, r5, 16, 2 */;
    r3 = *(u32*)(r4 + r3);
    if ((u32)r3 == (u32)0x0) goto L_8025C354;
    if ((u32)r26 != (u32)r3) goto L_8025C348;
    r6 = r5;
L_8025C348: ;
    if ((u32)r28 != (u32)r3) goto L_8025C354;
    r7 = r5;
L_8025C354: ;
    r5 = r5 + 0x1;
L_8025C358: ;
    r3 = r5 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8025C32C;
    r3 = r6 & 0xFFFF;
    r0 = r7 & 0xFFFF;
    r0 = r3 - r0;
    r24 = (u32)r0 >> 31;
    if ((u32)r29 == (u32)0xffff) goto L_8025C384;
    if ((u32)r29 != (u32)0xfffe) goto L_8025C420;
L_8025C384: ;
    if ((u32)r29 != (u32)0xffff) goto L_8025C3CC;
    r3 = r25;
    r4 = r28;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C3CC;
    r3 = r28;
    r4 = 0x1d;
    fn_80201D84();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_8025C3CC;
    r3 = -0x1;
    goto L_8025C590;
L_8025C3CC: ;
    r3 = r25;
    r4 = r26;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C3F4;
    if ((u32)r24 != (u32)0x1) goto L_8025C3F4;
    r3 = 0x0;
    goto L_8025C590;
L_8025C3F4: ;
    r3 = r25;
    r4 = r28;
    r5 = r27;
    fn_8025CAA8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C418;
    r3 = 0x0;
    goto L_8025C590;
L_8025C418: ;
    r3 = 0x1;
    goto L_8025C590;
L_8025C420: ;
    r3 = r27;
    r4 = r26;
    r5 = r28;
    fn_80229934();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C444;
    r3 = 0x0;
    goto L_8025C590;
L_8025C444: ;
    r3 = r25;
    r4 = r28;
    r5 = r27;
    fn_8025CAA8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C468;
    r3 = 0x0;
    goto L_8025C590;
L_8025C468: ;
    r3 = r25;
    r4 = r28;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C4A8;
    r3 = r28;
    r4 = 0x1d;
    fn_80201D84();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r3 != (u32)r0) goto L_8025C4A8;
    r3 = -0x1;
    goto L_8025C590;
L_8025C4A8: ;
    if ((u32)r24 != (u32)0x1) goto L_8025C558;
    r3 = r25;
    r4 = r28;
    r5 = 0x1f;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C4F4;
    if ((u32)r30 == (u32)0x92) goto L_8025C4F4;
    if ((u32)r30 == (u32)0x95) goto L_8025C4F4;
    if ((u32)r30 == (u32)0x98) goto L_8025C4F4;
    if ((u32)r30 == (u32)0xcf) goto L_8025C4F4;
    r3 = 0x0;
    goto L_8025C590;
L_8025C4F4: ;
    r3 = r25;
    r4 = r28;
    r5 = 0x20;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C520;
    if ((u32)r30 == (u32)0x93) goto L_8025C520;
    r3 = 0x0;
    goto L_8025C590;
L_8025C520: ;
    r3 = r25;
    r4 = r28;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C558;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 == (u32)0x39) goto L_8025C558;
    if ((u32)r0 == (u32)0xfa) goto L_8025C558;
    r3 = 0x0;
    goto L_8025C590;
L_8025C558: ;
    r3 = r27;
    fn_80229B70();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C574;
    r3 = -0x1;
    goto L_8025C590;
L_8025C574: ;
    r3 = r27;
    fn_80229BD8();
    r0 = r3 & 0xFF;
    r3 = 0x1;
    if ((u32)r0 != (u32)0x1) goto L_8025C590;
    r3 = -0x1;
L_8025C590: ;
    /* lmw r24, 0x30(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025C5A4 | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025C5A4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = 0x0;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r31 = r5;
    r30 = r4;
    r5 = 0x9;
    r4 = r31;
    fn_8011BEB4();
    r0 = r30 & 0xFFFF;
    r3 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1f) goto L_8025C608;
    if ((u32)r3 == (u32)0x92) goto L_8025C600;
    if ((u32)r3 == (u32)0x95) goto L_8025C600;
    if ((u32)r3 == (u32)0x98) goto L_8025C600;
    if ((u32)r3 != (u32)0xcf) goto L_8025C608;
L_8025C600: ;
    r3 = 0x1;
    goto L_8025C660;
L_8025C608: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x20) goto L_8025C624;
    if ((u32)r3 != (u32)0x93) goto L_8025C624;
    r3 = 0x1;
    goto L_8025C660;
L_8025C624: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x21) goto L_8025C64C;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x39) goto L_8025C644;
    if ((u32)r0 != (u32)0xfa) goto L_8025C64C;
L_8025C644: ;
    r3 = 0x1;
    goto L_8025C660;
L_8025C64C: ;
    if ((u32)r3 != (u32)0x5e) goto L_8025C65C;
    r3 = 0x1;
    goto L_8025C660;
L_8025C65C: ;
    r3 = 0x0;
L_8025C660: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025C674 | Size: 0x48 | Pattern: field_accessor */
u32 fn_8025C674(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F37B0();
    extern void fn_8025C6BC();
    u32 result[2];
    result[0] = 0;
    fn_801F37B0(0, (u32)fn_8025C6BC, (u32)result, 0);
    return result[1] & 0xFFFF;
}

/* Address: 0x8025C6BC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025C6BC(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F8424();
    extern void fn_802062FC();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r4;
    r31 = r3;
    r30 = *(u32*)((u8*)r5 + 0x0);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8025C6F0;
    r3 = 0x1;
    goto L_8025C75C;
L_8025C6F0: ;
    r3 = r30;
    r4 = r31;
    r5 = r28;
    fn_801F8424();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C714;
    r3 = 0x1;
    goto L_8025C75C;
L_8025C714: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C758;
    r3 = r30;
    r4 = r31;
    r5 = 0x2b;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C758;
    r3 = *(u32*)((u8*)r29 + 0x4);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r29 + 0x4) = r0;
L_8025C758: ;
    r3 = 0x1;
L_8025C75C: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025C770 | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025C770(void* ctx, u32 param1, u32 param2) {
    extern void fn_801FB1C0();
    extern void fn_8021B364();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r5 = 0x43;
    r6 = 0x0;
    r31 = r4;
    r4 = 0x0;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2a;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C7F0;
    r3 = r31;
    r4 = r1 + 0x8;
    fn_8021B364();
    r3 = r3 & 0xFF;
    r0 = 0x1;
    r0 = r3 - r0;
    r0 = -0x1;
    /* subfze r0, r0 */;
    r3 = r0 & 0xFF;
    goto L_8025C7F4;
L_8025C7F0: ;
    r3 = 0x1;
L_8025C7F4: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x8025C808 | Size: 0x2A0 (672 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025C808(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_801FB1C0();
    extern void fn_8021C034();
    extern void fn_8021C090();
    extern void fn_80229C28();
    extern void fn_80237F74();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
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

    r0 = r9 & 0x00000040;
    /* stmw r21, 0x14(r1) */;
    r21 = r9;
    r22 = r3;
    r23 = r5;
    r24 = r6;
    r25 = r7;
    r26 = r8;
    r28 = 0x0;
    r27 = 0x0;
    if ((s32)r0 == (s32)0) goto L_8025C848;
    r30 = r4;
    goto L_8025C84C;
L_8025C848: ;
    r30 = r23;
L_8025C84C: ;
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r31 = r21 & 0xFF;
    r21 = r3;
    r0 = r31 & 0xbf;
    r0 = r0 & 0x00000080;
    if ((s32)r0 == (s32)0) goto L_8025C870;
    r28 = 0x1;
L_8025C870: ;
    r0 = r31 & 0x00000020;
    if ((s32)r0 == (s32)0) goto L_8025C87C;
    r27 = 0x1;
L_8025C87C: ;
    r3 = r26;
    fn_8021C090();
    r0 = r3;
    r3 = r30;
    r29 = r0;
    r4 = 0x0;
    r5 = r29;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r26 = (s8)r3;
    r3 = r25;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0) goto L_8025CA80;
    r3 = r21;
    r4 = 0x4c;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C8E8;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C8E8;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 == (u32)0xae) goto L_8025C8E8;
    r3 = 0x0;
    goto L_8025CA94;
L_8025C8E8: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 == (u32)0xae) goto L_8025C980;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C980;
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
    if ((u32)r0 != (u32)0x1) goto L_8025C968;
    r3 = r23;
    r4 = r24;
    fn_80229C28();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C968;
    r0 = 0x1;
    goto L_8025C96C;
L_8025C968: ;
    r0 = 0x0;
L_8025C96C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C980;
    r3 = 0x0;
    goto L_8025CA94;
L_8025C980: ;
    r3 = r22;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025C9B8;
    r3 = r22;
    r4 = r30;
    r5 = 0x49;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C9D4;
L_8025C9B8: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C9D4;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 == (u32)0xae) goto L_8025C9D4;
    r3 = 0x0;
    goto L_8025CA94;
L_8025C9D4: ;
    r3 = r22;
    r4 = r30;
    r5 = 0x33;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CA0C;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CA0C;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0xeb) goto L_8025CA0C;
    r3 = 0x0;
    goto L_8025CA94;
L_8025CA0C: ;
    r3 = r22;
    r4 = r30;
    r5 = 0x34;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CA44;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CA44;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0xe6) goto L_8025CA44;
    r3 = 0x0;
    goto L_8025CA94;
L_8025CA44: ;
    r3 = r22;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CA70;
    r0 = r31 & 0x1F;
    if ((u32)r0 != (u32)0x1) goto L_8025CA70;
    r3 = 0x0;
    goto L_8025CA94;
L_8025CA70: ;
    r0 = (s8)r26;
    if ((u32)r0 > (u32)0x1) goto L_8025CA90;
    r3 = 0x0;
    goto L_8025CA94;
L_8025CA80: ;
    if ((s32)r26 < (s32)0xc) goto L_8025CA90;
    r3 = 0x0;
    goto L_8025CA94;
L_8025CA90: ;
    r3 = 0x1;
L_8025CA94: ;
    /* lmw r21, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025CAA8 | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025CAA8(void* ctx, u32 param1, u32 param2) {
    extern void fn_801FB1C0();
    extern void fn_80229C28();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r4;
    r31 = r5;
    r4 = 0x0;
    r5 = 0x43;
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
    if ((u32)r0 != (u32)0x1) goto L_8025CB24;
    r3 = r30;
    r4 = r31;
    fn_80229C28();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CB24;
    r3 = 0x1;
    goto L_8025CB28;
L_8025CB24: ;
    r3 = 0x0;
L_8025CB28: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025CB3C | Size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025CB3C(void* ctx, u32 param1, u32 param2) {
    extern void fn_800E0C54();
    extern void fn_80201248();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    f32 f7 = 0.0f;

    r5 = 0xf7;
    r6 = 0x0;
    r31 = r4;
    r4 = 0x0;
    r3 = r31;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_8025CB80;
    if ((u32)r3 == (u32)0x165) goto L_8025CB80;
    if ((u32)r3 == (u32)0xffff) goto L_8025CB80;
    goto L_8025CBD4;
L_8025CB80: ;
    r3 = r31;
    r4 = r1 + 0x8;
    fn_80201248();
    r0 = r3 & 0xFF;
    r31 = r3;
    if ((u32)r3 == (u32)0xffff) goto L_8025CBD0;
    fn_800E0C54();
    r5 = r3 & 0xFFFF;
    r4 = r31 & 0xFF;
    r0 = (s32)r5 / (s32)r4;
    r3 = r1 + 0x8;
    r0 = r0 * r4;
    r0 = r5 - r0;
    /* clrlslwi r0, r0, 24, 1 */;
    r3 = *(u16*)(r3 + r0);
    if ((u32)r3 == (u32)0x0) goto L_8025CBD0;
    if ((u32)r3 == (u32)0x165) goto L_8025CBD0;
    goto L_8025CBD4;
L_8025CBD0: ;
    r3 = 0x0;
L_8025CBD4: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x8025CBE8 | Size: 0x48 | Pattern: field_accessor */
u32 fn_8025CBE8(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F37B0();
    extern void fn_8025CC30();
    u8 buf[8];
    u32 r;
    r = fn_801F37B0(0, (u32)fn_8025CC30, (u32)buf, 0) & 0xFF;
    r = 1 - r;
    return (r != 0) ? 1 : 0;
}

/* Address: 0x8025CC30 | Size: 0x60 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025CC30(void* ctx, u32 slot, u32 param) {
    extern void fn_802062FC();
    extern void fn_80237F74();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = *(u32*)((u8*)r5 + 0x0);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8025CC5C;
    r3 = 0x1;
    goto L_8025CC7C;
L_8025CC5C: ;
    r3 = r31;
    r4 = r30;
    r5 = 0x6;
    fn_80237F74();
    r0 = r3 & 0xFF;
    r3 = 0x1 - r0;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_8025CC7C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025CC90 | Size: 0x50 | Pattern: field_accessor */
u32 fn_8025CC90(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801F37B0();
    extern void fn_8025CCE0();
    u8 buf[8];
    u32 r;
    r = fn_801F37B0(0, (u32)fn_8025CCE0, (u32)buf, 0) & 0xFF;
    r = 1 - r;
    return (r != 0) ? 1 : 0;
}

/* Address: 0x8025CCE0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025CCE0(void* ctx, u32 slot, u32 param) {
    extern void fn_802062FC();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r31 = *(u32*)((u8*)r5 + 0x0);
    r30 = *(u32*)((u8*)r5 + 0x4);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8025CD10;
    r3 = 0x1;
    goto L_8025CD50;
L_8025CD10: ;
    r3 = r31;
    r4 = r29;
    r5 = 0xb;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CD4C;
    r3 = r31;
    r4 = r30;
    r5 = 0x2b;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025CD4C;
    r3 = 0x0;
    goto L_8025CD50;
L_8025CD4C: ;
    r3 = 0x1;
L_8025CD50: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025CD64 | Size: 0x54 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025CD64(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047B650[];
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047B650;
    if ((u32)r3 == (u32)0x0) goto L_8025CDA4;
    fn_800E202C();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if ((u32)r3 == (u32)0x0) goto L_8025CD9C;
    fn_800E24B0();
    r3 = r31;
    fn_800E209C();
L_8025CD9C: ;
    r0 = 0x0;
    *(u32*)lbl_8047B650 = r0;
L_8025CDA4: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x8025CDB8 | Size: 0x2B4 (692 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025CDB8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478D98[];
    extern u8 lbl_8047B650[];
    extern u8 lbl_8047B654[];
    extern void fn_8006B09C();
    extern void fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void fn_800FA280();
    extern void fn_8012086C();
    extern void fn_80123EF0();
    extern void fn_801240C4();
    extern void fn_80124A60();
    extern void fn_80129280();
    extern void fn_8012A248();
    extern void fn_8012AC08();
    extern void fn_80135938();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = 0x0;
    r4 = 0x20;
    /* stmw r28, 0x10(r1) */;
    r0 = *(u32*)lbl_80478D98;
    *(u32*)lbl_8047B654 = r3;
    r3 = r0 * 0x138;
    r0 = r3 + 0x1f;
    /* clrrwi r3, r0, 5 */;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_8025CDF8;
    fn_800E27B0();
    goto L_8025CDFC;
L_8025CDF8: ;
    r3 = 0x0;
L_8025CDFC: ;
    *(u32*)lbl_8047B654 = r3;
    r30 = r3;
    r31 = 0x0;
    goto L_8025CE68;
L_8025CE0C: ;
    r3 = 0x0;
    r4 = 0x1;
    fn_80135938();
    r0 = r31 + 0x1;
    r6 = r3;
    r3 = r30;
    r5 = 0xa;
    r4 = r0 & 0xFFFF;
    fn_801240C4();
    r3 = r31 + 0x1004;
    fn_800FA280();
    r9 = r3;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x8;
    r6 = 0x1;
    r7 = 0x0;
    r8 = 0x0;
    fn_80123EF0();
    r3 = r30;
    fn_8012086C();
    r30 = r30 + 0x138;
    r31 = r31 + 0x1;
L_8025CE68: ;
    r0 = *(u32*)lbl_80478D98;
    if ((s32)r31 < (s32)r0) goto L_8025CE0C;
    r3 = *(u32*)lbl_8047B650;
    if ((u32)r3 == (u32)0x0) goto L_8025CEA4;
    fn_800E202C();
    r0 = r3 & 0xFFFF;
    r30 = r3;
    if ((u32)r3 == (u32)0x0) goto L_8025CE9C;
    fn_800E24B0();
    r3 = r30;
    fn_800E209C();
L_8025CE9C: ;
    r0 = 0x0;
    *(u32*)lbl_8047B650 = r0;
L_8025CEA4: ;
    r3 = 0x80;
    r4 = 0x20;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_8025CEC0;
    fn_800E27B0();
    goto L_8025CEC4;
L_8025CEC0: ;
    r3 = 0x0;
L_8025CEC4: ;
    *(u32*)lbl_8047B650 = r3;
    r31 = 0x0;
L_8025CECC: ;
    r3 = r31;
    fn_8006B09C();
    r3 = r3 + 0xb44;
    fn_8012A248();
    r30 = 0x0;
L_8025CEE0: ;
    r3 = r31;
    fn_8006B09C();
    r4 = r30 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    fn_80124A60();
    r30 = r30 + 0x1;
    if ((s32)r30 < (s32)0x6) goto L_8025CEE0;
    r31 = r31 + 0x1;
    if ((s32)r31 < (s32)0x4) goto L_8025CECC;
    r30 = 0x0;
L_8025CF14: ;
    r3 = r30;
    fn_8006B09C();
    r3 = r3 + 0x2c;
    fn_8012A248();
    r31 = 0x0;
L_8025CF28: ;
    r3 = r30;
    fn_8006B09C();
    r4 = r31 & 0xFFFF;
    r3 = r3 + 0x2c;
    fn_8012AC08();
    fn_80124A60();
    r31 = r31 + 0x1;
    if ((s32)r31 < (s32)0x6) goto L_8025CF28;
    r30 = r30 + 0x1;
    if ((s32)r30 < (s32)0x4) goto L_8025CF14;
    r0 = 0x6;
    ctr_fn = (void(*)(void))r0;
L_8025CF60: ;
    if (--ctr != 0) goto L_8025CF60;
    r3 = 0x0;
    r4 = 0x2;
    fn_80129280();
    r30 = r3;
    r3 = 0x0;
    fn_8006B09C();
    r3 = r3 + 0xb44;
    if ((u32)r30 == (u32)0x0) goto L_8025CF94;
    r4 = r30;
    r5 = 0xb18;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
L_8025CF94: ;
    r29 = 0x0;
    r31 = 0x0;
L_8025CF9C: ;
    if ((s32)r31 != (s32)0x0) goto L_8025CFD8;
    r3 = 0x0;
    r4 = 0x2;
    fn_80129280();
    r30 = r3;
    r3 = r31;
    fn_8006B09C();
    r3 = r3 + 0xb44;
    if ((u32)r30 == (u32)0x0) goto L_8025D01C;
    r4 = r30;
    r5 = 0xb18;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    goto L_8025D01C;
L_8025CFD8: ;
    r28 = 0x0;
L_8025CFDC: ;
    r0 = *(u32*)lbl_8047B654;
    r3 = r31;
    r30 = r0 + r29;
    fn_8006B09C();
    r4 = r28 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    if ((u32)r3 == (u32)0x0) goto L_8025D00C;
    r4 = r30;
    r5 = 0x138;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
L_8025D00C: ;
    r28 = r28 + 0x1;
    r29 = r29 + 0x138;
    if ((s32)r28 < (s32)0x6) goto L_8025CFDC;
L_8025D01C: ;
    r31 = r31 + 0x1;
    if ((s32)r31 < (s32)0x4) goto L_8025CF9C;
    r3 = *(u32*)lbl_8047B654;
    if ((u32)r3 == (u32)0x0) goto L_8025D058;
    fn_800E202C();
    r0 = r3 & 0xFFFF;
    r30 = r3;
    if ((u32)r3 == (u32)0x0) goto L_8025D050;
    fn_800E24B0();
    r3 = r30;
    fn_800E209C();
L_8025D050: ;
    r0 = 0x0;
    *(u32*)lbl_8047B654 = r0;
L_8025D058: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D06C | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025D06C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025D0A8 | Size: 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025D0A8(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_80478EAC[];
    extern u8 lbl_8047E658[];
    extern u8 lbl_8047E65C[];
    extern void fn_8011F5C8();
    extern void fn_80123FBC();
    extern void fn_80129280();
    extern void fn_8012AC08();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r28, 0x10(r1) */;
    /* mr. r28, r3 */;
    r30 = 0x0;
    if ((s32)r0 != (s32)0) goto L_8025D0D4;
    r3 = 0x0;
    r4 = 0x2;
    fn_80129280();
    r28 = r3;
L_8025D0D4: ;
    r31 = 0x0;
L_8025D0D8: ;
    r3 = r28;
    r4 = r31 & 0xFFFF;
    fn_8012AC08();
    r29 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8025D128;
    r3 = r29;
    fn_8011F5C8();
    r4 = *(u32*)lbl_80478EAC;
    r0 = r3 & 0xFFFF;
    r3 = 0x0;
L_8025D108: ;
    r5 = *(u16*)(r4 + r3);
    if ((u32)r5 == (u32)0x0) goto L_8025D128;
    if ((u32)r0 != (u32)r5) goto L_8025D120;
    r30 = r30 + 0x1;
L_8025D120: ;
    r3 = r3 + 0x2;
    goto L_8025D108;
L_8025D128: ;
    r31 = r31 + 0x1;
    if ((s32)r31 < (s32)0x6) goto L_8025D0D8;
    f1 = *(f32*)lbl_8047E658;
    f0 = *(f32*)lbl_8047E65C;
    ctr_fn = (void(*)(void))r30;
    if ((s32)r30 <= (s32)0x0) goto L_8025D150;
L_8025D148: ;
    f1 = f1 * f0;
    if (--ctr != 0) goto L_8025D148;
L_8025D150: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D164 | Size: 0x128 (296 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025D164(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8039A648[];
    extern u8 lbl_8039A664[];
    extern u8 lbl_80478EAC[];
    extern u8 lbl_8047E658[];
    extern u8 lbl_8047E65C[];
    extern void fn_8006B09C();
    extern void fn_8006B5A8();
    extern void fn_8011F5C8();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r26, 0x18(r1) */;
    r28 = 0x0;
    fn_8006B5A8();
    r27 = *(u32*)((u8*)r3 + 0xC);
    fn_8006B5A8();
    r26 = *(u32*)((u8*)r3 + 0x0);
    fn_8006B5A8();
    r30 = *(u32*)((u8*)r3 + 0x14);
    r29 = 0x0;
L_8025D194: ;
    r3 = 0x0;
    fn_8006B09C();
    r4 = r29 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    r31 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8025D1EC;
    r3 = r31;
    fn_8011F5C8();
    r4 = *(u32*)lbl_80478EAC;
    r0 = r3 & 0xFFFF;
    r3 = 0x0;
L_8025D1CC: ;
    r5 = *(u16*)(r4 + r3);
    if ((u32)r5 == (u32)0x0) goto L_8025D1EC;
    if ((u32)r0 != (u32)r5) goto L_8025D1E4;
    r28 = r28 + 0x1;
L_8025D1E4: ;
    r3 = r3 + 0x2;
    goto L_8025D1CC;
L_8025D1EC: ;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x6) goto L_8025D194;
    f1 = *(f32*)lbl_8047E658;
    f0 = *(f32*)lbl_8047E65C;
    ctr_fn = (void(*)(void))r28;
    if ((s32)r28 <= (s32)0x0) goto L_8025D214;
L_8025D20C: ;
    f1 = f1 * f0;
    if (--ctr != 0) goto L_8025D20C;
L_8025D214: ;
    if ((s32)r26 != (s32)0x1) goto L_8025D24C;
    r3 = r30 + 0x1;
    r0 = 0xa;
    r0 = (s32)r3 / (s32)r0;
    if ((s32)r0 <= (s32)0xa) goto L_8025D234;
    r0 = 0xa;
L_8025D234: ;
    r3 = (u32)lbl_8039A664;
    r0 = r0 << 2;
    r3 = (u32)lbl_8039A664;
    f0 = *(f32*)(r3 + r0);
    f1 = f1 * f0;
    goto L_8025D26C;
L_8025D24C: ;
    if ((s32)r27 < (s32)0x6) goto L_8025D258;
    r27 = 0x6;
L_8025D258: ;
    r3 = (u32)lbl_8039A648;
    r0 = r27 << 2;
    r3 = (u32)lbl_8039A648;
    f0 = *(f32*)(r3 + r0);
    f1 = f1 * f0;
L_8025D26C: ;
    f0 = (f64)(s32)f1;
    *(f64*)(sp + 0x8) = f0;
    r3 = *(u32*)(sp + 0xC);
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D28C | Size: 0x24 | Pattern: null_check_getter */
extern void* fn_8006B09C(void*);
u16 fn_8025D28C(void* ctx) { return *(u16*)fn_8006B09C(ctx); }

/* Address: 0x8025D2B0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D2B0(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x24); }

/* Address: 0x8025D2D4 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025D2D4(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_80478E04[];
    extern void fn_8006B09C();
    extern void fn_801FCBA4();
    extern void fn_801FCCC4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r31 = r4;
    fn_8006B09C();
    r3 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r3 != (u32)0x0) goto L_8025D300;
    r3 = 0x0;
    goto L_8025D350;
L_8025D300: ;
    fn_801FCCC4();
    fn_801FCBA4();
    r0 = r3 * 0x14;
    r3 = *(u32*)lbl_80478E04;
    r3 = r3 + r0;
    if ((s32)r31 != (s32)0x0) goto L_8025D338;
    r0 = *(u32*)((u8*)r3 + 0xC);
    r3 = (0xf94 << 16);
    r3 = r3 + 0x1200;
    if ((u32)r0 == (u32)0x0) goto L_8025D350;
    r3 = r0;
    goto L_8025D350;
L_8025D338: ;
    r0 = *(u32*)((u8*)r3 + 0x10);
    r3 = (0xf8f << 16);
    r3 = r3 + 0x1200;
    if ((u32)r0 == (u32)0x0) goto L_8025D350;
    r3 = r0;
L_8025D350: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x8025D364 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025D364(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_80478E04[];
    extern void fn_8006B09C();
    extern void fn_801FCBA4();
    extern void fn_801FCCC4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((s32)r3 == (s32)0x0) goto L_8025D38C;
    fn_8006B09C();
    r0 = *(u16*)((u8*)r3 + 0x0);
    goto L_8025D394;
L_8025D38C: ;
    fn_8006B09C();
    r0 = *(u16*)((u8*)r3 + 0x0);
L_8025D394: ;
    r3 = r0 & 0xFFFF;
    if ((s32)r3 != (s32)0x0) goto L_8025D3A4;
    r3 = 0x0;
    goto L_8025D3E0;
L_8025D3A4: ;
    fn_801FCCC4();
    fn_801FCBA4();
    r0 = r3 * 0x14;
    r3 = *(u32*)lbl_80478E04;
    r3 = r3 + r0;
    if ((s32)r31 != (s32)0x0) goto L_8025D3C8;
    r3 = *(u32*)((u8*)r3 + 0x4);
    goto L_8025D3E0;
L_8025D3C8: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    r3 = (0xf99 << 16);
    r3 = r3 + 0x1200;
    if ((u32)r0 == (u32)0x0) goto L_8025D3E0;
    r3 = r0;
L_8025D3E0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x8025D3F4 | Size: 0x16C (364 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025D3F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    fn_8006B1D4();
    r30 = 0x0;
    r31 = r27;
    r29 = r30;
    r28 = r3 & 0xFFFF;
L_8025D41C: ;
    r3 = r27;
    fn_8006B09C();
    r4 = r29 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    fn_80123FBC();
    r3 = r3 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r0 = r3 - r0; /* -borrow */;
    r0 = r0 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8025D454;
    r3 = r30 & 0xFFFF;
    r0 = r3 + 0x1;
    r30 = r0 & 0xFFFF;
L_8025D454: ;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x6) goto L_8025D41C;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 >= (u32)r28) goto L_8025D470;
    r28 = r30;
L_8025D470: ;
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
L_8025D498: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8025D498;
    r27 = 0x0;
    r29 = 0x0;
    goto L_8025D544;
L_8025D4B8: ;
    r3 = r31;
    fn_8006B09C();
    r0 = r29 + 0x8;
    r28 = *(u32*)(r3 + r0);
    r3 = r31;
    fn_8006B09C();
    r4 = r28 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    r28 = r3;
    r3 = r31;
    fn_8006B09C();
    r4 = r27 & 0xFFFF;
    r3 = r3 + 0x2c;
    fn_8012AC08();
    r3 = r31;
    fn_8006B09C();
    r4 = r27 & 0xFFFF;
    r3 = r3 + 0x2c;
    fn_8012AC08();
    if ((u32)r3 == (u32)0x0) goto L_8025D53C;
    if ((u32)r28 == (u32)0x0) goto L_8025D53C;
    r0 = 0x27;
    /* subi r5, r3, 0x4 */;
    /* subi r4, r28, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_8025D528: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8025D528;
L_8025D53C: ;
    r27 = r27 + 0x1;
    r29 = r29 + 0x4;
L_8025D544: ;
    if ((s32)r27 < (s32)r30) goto L_8025D4B8;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D560 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D560(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x20); }

/* Address: 0x8025D584 | Size: 0x5C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025D584(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    fn_8006B09C();
    r4 = *(u32*)((u8*)r3 + 0x20);
    /* subic. r0, r4, 0x1 */;
    if ((s32)r0 < (s32)0) goto L_8025D5A8;
    if ((s32)r0 <= (s32)0x6) goto L_8025D5B0;
L_8025D5A8: ;
    r3 = 0x0;
    goto L_8025D5D0;
L_8025D5B0: ;
    r0 = r0 << 2;
    r5 = -0x1;
    r4 = r3 + r0;
    *(u32*)((u8*)r4 + 0x8) = r5;
    r4 = *(u32*)((u8*)r3 + 0x20);
    /* subi r0, r4, 0x1 */;
    *(u32*)((u8*)r3 + 0x20) = r0;
    r3 = *(u32*)((u8*)r3 + 0x20);
L_8025D5D0: ;
    return;
}
#pragma pop

/* Address: 0x8025D5E0 | Size: 0x64 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025D5E0(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r4;
    r31 = r5;
    fn_8006B09C();
    r6 = 0x0;
    r4 = 0x0;
    ctr_fn = (void(*)(void))r30;
    if ((s32)r30 <= (s32)0x0) goto L_8025D628;
L_8025D610: ;
    r5 = *(u32*)(r31 + r4);
    r0 = r4 + 0x8;
    r6 = r6 + 0x1;
    r4 = r4 + 0x4;
    *(u32*)(r3 + r0) = r5;
    if (--ctr != 0) goto L_8025D610;
L_8025D628: ;
    *(u32*)((u8*)r3 + 0x20) = r6;
    r3 = r6;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D644 | Size: 0x100 (256 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025D644(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r25, 0x14(r1) */;
    r29 = r4;
    r25 = r3;
    fn_8006B09C();
    r31 = r3;
    r30 = *(u32*)((u8*)r3 + 0x20);
    fn_8006B1D4();
    r26 = 0x0;
    r28 = r3 & 0xFFFF;
    r27 = r26;
L_8025D678: ;
    r3 = r25;
    fn_8006B09C();
    r4 = r27 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    fn_80123FBC();
    r3 = r3 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r0 = r3 - r0; /* -borrow */;
    r0 = r0 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8025D6B0;
    r3 = r26 & 0xFFFF;
    r0 = r3 + 0x1;
    r26 = r0 & 0xFFFF;
L_8025D6B0: ;
    r27 = r27 + 0x1;
    if ((s32)r27 < (s32)0x6) goto L_8025D678;
    r0 = r26 & 0xFFFF;
    r3 = *(u32*)((u8*)r31 + 0x20);
    if ((u32)r0 >= (u32)r28) goto L_8025D6D0;
    r28 = r26;
L_8025D6D0: ;
    r0 = r28 & 0xFFFF;
    if ((s32)r3 < (s32)r0) goto L_8025D6E4;
    r3 = -0x1;
    goto L_8025D730;
L_8025D6E4: ;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r30;
    if ((s32)r30 <= (s32)0x0) goto L_8025D714;
L_8025D6F4: ;
    r0 = r3 + 0x8;
    r0 = *(u32*)(r31 + r0);
    if ((s32)r0 != (s32)r29) goto L_8025D70C;
    r3 = -0x1;
    goto L_8025D730;
L_8025D70C: ;
    r3 = r3 + 0x4;
    if (--ctr != 0) goto L_8025D6F4;
L_8025D714: ;
    r0 = r30 << 2;
    r3 = r30;
    r4 = r31 + r0;
    *(u32*)((u8*)r4 + 0x8) = r29;
    r4 = *(u32*)((u8*)r31 + 0x20);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r31 + 0x20) = r0;
L_8025D730: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D744 | Size: 0x44 | Pattern: field_accessor */
u32 fn_8025D744(void* ctx, u32 slot, u32 param) {
    extern void* fn_8006B09C();
    u8* base;
    u32 i;
    base = (u8*)fn_8006B09C(ctx);
    *(u32*)(base + 0x20) = 0;
    for (i = 0; i < 6; i++) {
        *(u32*)(base + 0x8 + i * 4) = (u32)-1;
    }
    return 0;
}

/* Address: 0x8025D788 | Size: 0x80 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025D788(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = 0x0;
    r31 = 0x163;
L_8025D7A0: ;
    r3 = r29;
    fn_8006B09C();
    r30 = r3 + 0xb44;
    r3 = r29;
    fn_8006B09C();
    r3 = r3 + 0x2c;
    if ((u32)r30 == (u32)0x0) goto L_8025D7E8;
    if ((u32)r3 == (u32)0x0) goto L_8025D7E8;
    /* subi r5, r3, 0x4 */;
    /* subi r4, r30, 0x4 */;
    ctr_fn = (void(*)(void))r31;
L_8025D7D4: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8025D7D4;
L_8025D7E8: ;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x4) goto L_8025D7A0;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D808 | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025D808(void* ctx, u32 param1, u32 param2) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    fn_8006B1D4();
    r30 = 0x0;
    r29 = r28;
    r31 = r30;
    r28 = r3 & 0xFFFF;
L_8025D830: ;
    r3 = r29;
    fn_8006B09C();
    r4 = r31 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    fn_80123FBC();
    r3 = r3 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r0 = r3 - r0; /* -borrow */;
    r0 = r0 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8025D868;
    r3 = r30 & 0xFFFF;
    r0 = r3 + 0x1;
    r30 = r0 & 0xFFFF;
L_8025D868: ;
    r31 = r31 + 0x1;
    if ((s32)r31 < (s32)0x6) goto L_8025D830;
    r0 = r30 & 0xFFFF;
    r3 = r28;
    if ((u32)r0 >= (u32)r28) goto L_8025D888;
    r3 = r30;
L_8025D888: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D89C | Size: 0x78 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025D89C(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r30 = 0x0;
    r31 = r3;
    r29 = 0x0;
L_8025D8B8: ;
    r3 = r31;
    fn_8006B09C();
    r4 = r29 & 0xFFFF;
    r3 = r3 + 0xb44;
    fn_8012AC08();
    fn_80123FBC();
    r3 = r3 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r0 = r3 - r0; /* -borrow */;
    r0 = r0 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8025D8F0;
    r3 = r30 & 0xFFFF;
    r0 = r3 + 0x1;
    r30 = r0 & 0xFFFF;
L_8025D8F0: ;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x6) goto L_8025D8B8;
    r3 = r30;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025D914 | Size: 0x24 | Pattern: null_check_getter */
void* fn_8025D914(void* ctx) { return (u8*)fn_8006B09C(ctx) + 0xb44; }

/* Address: 0x8025D938 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025D938(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025D970 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025D970(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025D9A8 | Size: 0x24 | Pattern: null_check_getter */
extern void* fn_8006B5A8(void*);
u32 fn_8025D9A8(void* ctx) { return *(u32*)fn_8006B5A8(ctx); }

/* Address: 0x8025D9CC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D9CC(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x10); }

/* Address: 0x8025D9F0 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_8025D9F0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DA18 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DA18(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DA3C | Size: 0x4C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025DA3C(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B5A8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    fn_8006B5A8();
    r0 = *(u32*)((u8*)r3 + 0x4);
    r3 = 0x2;
    if ((s32)r0 == (s32)0x2) goto L_8025DA74;
    if ((s32)r0 >= (s32)0x2) goto L_8025DA78;
    if ((s32)r0 >= (s32)0x0) goto L_8025DA6C;
    goto L_8025DA78;
L_8025DA6C: ;
    r3 = 0x2;
    goto L_8025DA78;
L_8025DA74: ;
    r3 = 0x4;
L_8025DA78: ;
    return;
}
#pragma pop

/* Address: 0x8025DA88 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DA88(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x4); }

/* Address: 0x8025DAAC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAAC(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0xc); }

/* Address: 0x8025DAD0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAD0(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x8); }

/* Address: 0x8025DAF4 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025DAF4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DB2C | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_8025DB2C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DB5C | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DB5C(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x18); }

/* Address: 0x8025DB80 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_8025DB80(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DBB0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DBB0(void* ctx) { return *(u32*)((u8*)fn_8006B5A8(ctx) + 0x14); }

/* Address: 0x8025DBD4 | Size: 0x58 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025DBD4(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8027A450[];
    extern u8 lbl_8039A690[];
    extern u8 lbl_80478E08[];
    extern u8 lbl_80478E0C[];
    extern void fn_800DD970();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = *(u32*)lbl_80478E08;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r3 < (u32)r0) goto L_8025DC10;
    r3 = (u32)lbl_8027A450;
    r4 = (u32)lbl_8039A690;
    r3 = (u32)lbl_8027A450;
    r4 = (u32)lbl_8039A690;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto L_8025DC1C;
L_8025DC10: ;
    r4 = *(u32*)lbl_80478E0C;
    r0 = r3 << 2;
    r3 = *(u32*)(r4 + r0);
L_8025DC1C: ;
    return;
}
#pragma pop

/* Address: 0x8025DC2C | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025DC2C(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8047E660[];
    extern u8 lbl_8047E664[];
    extern u8 lbl_8047E668[];
    extern void fn_800D3088();
    extern void fn_800F0308();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x40) = f31;
    /* psq_st f31, 0x48(r1), 0, qr0 */;
    *(f64*)(sp + 0x30) = f30;
    /* psq_st f30, 0x38(r1), 0, qr0 */;
    *(f64*)(sp + 0x20) = f29;
    /* psq_st f29, 0x28(r1), 0, qr0 */;
    f0 = *(f32*)lbl_8047E664;
    r31 = (0x4330 << 16);
    f29 = *(f32*)lbl_8047E660;
    f30 = f0 * f1;
    f31 = *(f64*)lbl_8047E668;
    goto L_8025DC88;
L_8025DC6C: ;
    fn_800F0308();
    fn_800D3088();
    f0 = *(f64*)(sp + 0x8);
    f0 = f0 - f31;
    f29 = f29 + f0;
L_8025DC88: ;
    if (f29 < f30) goto L_8025DC6C;
    /* psq_l f31, 0x48(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x40);
    /* psq_l f30, 0x38(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x30);
    /* psq_l f29, 0x28(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x8025DCBC | Size: 0x58 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025DCBC(void* ctx, u32 slot, u32 param) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_8025DCE8;
    r4 = 0x32;
    r5 = 0xff;
    ((void(*)(void))fn_80165A20)();
L_8025DCE8: ;
    r3 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r3 == (u32)0x0) goto L_8025DD00;
    r4 = 0x32;
    r5 = 0xff;
    ((void(*)(void))fn_801659FC)();
L_8025DD00: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x8025DD14 | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025DD14(void* ctx, u32 param1, u32 param2) {
    extern void fn_801653BC();
    extern void fn_801653C4();
    extern void fn_801656D8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    fn_801653C4();
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_8025DD60;
    fn_801656D8();
    r0 = r3;
    r3 = 0x1;
    r29 = r0;
    r4 = 0x32;
    r5 = 0xff;
    ((void(*)(void))fn_80165A20)();
    goto L_8025DD64;
L_8025DD60: ;
    r29 = 0x0;
L_8025DD64: ;
    fn_801653BC();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_8025DD78;
    fn_801656D8();
    goto L_8025DD7C;
L_8025DD78: ;
    r3 = 0x0;
L_8025DD7C: ;
    *(u32*)((u8*)r28 + 0x0) = r30;
    *(u32*)((u8*)r28 + 0x4) = r31;
    *(u32*)((u8*)r28 + 0x8) = r29;
    *(u32*)((u8*)r28 + 0xC) = r3;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* Address: 0x8025DDAC | Size: 0x48 | Pattern: field_accessor */
u32 fn_8025DDAC(void* ctx, u32 slot, u32 param) {
    extern void fn_800E4170();
    extern void* fn_801DAC3C();
    void* ptr = *(void**)ctx;
    if (ptr != 0) {
        ptr = fn_801DAC3C(ptr);
        if (ptr != 0) {
            fn_800E4170(ptr, slot);
        }
    }
    return 0;
}

/* Address: 0x8025DDF4 | Size: 0x18 */
void fn_8025DDF4(u32* ptr) { if (*ptr != 0) { *ptr = 0; } }

/* Address: 0x8025DE0C | Size: 0x48 | Pattern: field_accessor */
u32 fn_8025DE0C(void* ctx, u32 slot, u32 param) {
    extern void fn_800E43A4();
    extern void* fn_801DAC3C();
    void* ptr = *(void**)ctx;
    if (ptr != 0) {
        ptr = fn_801DAC3C(ptr);
        if (ptr != 0) {
            fn_800E43A4(ptr, slot);
        }
    }
    return 0;
}

/* Address: 0x8025DE54 | Size: 0xE4 (228 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025DE54(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E670[];
    extern void fn_800F0308();
    extern void fn_801C41C8();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern void fn_801DA914();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB088();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    /* stmw r26, 0x8(r1) */;
    r26 = r3;
    r30 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r8;
    if ((s32)r7 != (s32)0x1) goto L_8025DE8C;
    f1 = *(f32*)lbl_8047E670;
    r3 = 0x2;
    fn_801C41C8();
L_8025DE8C: ;
    r3 = *(u32*)((u8*)r26 + 0x0);
    r4 = 0x1;
    fn_801DA4E8();
    r31 = r30;
    r30 = 0x0;
    goto L_8025DF1C;
L_8025DEA4: ;
    if ((s32)r28 != (s32)0x0) goto L_8025DEBC;
    r3 = *(u32*)((u8*)r26 + 0x0);
    r4 = *(u16*)((u8*)r31 + 0x0);
    r5 = *(u16*)((u8*)r31 + 0x2);
    fn_801DA914();
L_8025DEBC: ;
    r3 = *(u32*)((u8*)r26 + 0x0);
    r4 = *(u16*)((u8*)r31 + 0x0);
    r5 = *(u16*)((u8*)r31 + 0x2);
    fn_801DA9E8();
    goto L_8025DED8;
L_8025DED0: ;
    fn_801DB088();
    fn_800F0308();
L_8025DED8: ;
    r3 = *(u32*)((u8*)r26 + 0x0);
    r4 = *(u16*)((u8*)r31 + 0x0);
    r5 = *(u16*)((u8*)r31 + 0x2);
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((s32)r28 != (s32)0x0) goto L_8025DED0;
    if ((s32)r29 != (s32)0x1) goto L_8025DF04;
    r3 = *(u32*)((u8*)r26 + 0x0);
    r4 = 0x0;
    fn_801DA4E8();
L_8025DF04: ;
    r3 = *(u32*)((u8*)r26 + 0x0);
    r4 = *(u16*)((u8*)r31 + 0x0);
    r5 = *(u16*)((u8*)r31 + 0x2);
    fn_801DA8C4();
    r31 = r31 + 0x4;
    r30 = r30 + 0x1;
L_8025DF1C: ;
    if ((s32)r30 < (s32)r27) goto L_8025DEA4;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025DF38 | Size: 0x178 (376 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025DF38(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_800E0C04();
    extern void fn_800E3C94();
    extern void fn_800E8FE8();
    extern void fn_800E900C();
    extern void fn_800E90C8();
    extern void fn_800E9108();
    extern void fn_800FF56C();
    extern void fn_80113F6C();
    extern void fn_80115280();
    extern void fn_8011538C();
    extern void fn_80115BD8();
    extern void fn_8018F470();
    extern void fn_801DAC3C();
    extern void fn_801DDD28();
    extern void fn_801DE190();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r23, 0x4c(r1) */;
    r26 = r3;
    r24 = r4;
    r23 = r5;
    r25 = r6;
    r3 = -0x1;
    fn_800E0C04();
    r4 = r3;
    r3 = r24;
    r5 = 0x0;
    fn_801DE190();
    *(u32*)((u8*)r26 + 0x0) = r3;
    if ((u32)r3 != (u32)0x0) goto L_8025DF84;
    r3 = 0x0;
    goto L_8025E09C;
L_8025DF84: ;
    r24 = 0x0;
    goto L_8025DFB8;
L_8025DF8C: ;
    r3 = *(u32*)((u8*)r26 + 0x0);
    r6 = 0x0;
    r4 = *(u16*)((u8*)r23 + 0x0);
    r5 = *(u16*)((u8*)r23 + 0x2);
    fn_801DDD28();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8025DFB0;
    r3 = 0x0;
    goto L_8025E09C;
L_8025DFB0: ;
    r23 = r23 + 0x4;
    r24 = r24 + 0x1;
L_8025DFB8: ;
    if ((s32)r24 < (s32)r25) goto L_8025DF8C;
    if ((u32)r26 == (u32)0x0) goto L_8025E098;
    r3 = *(u32*)((u8*)r26 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_8025E098;
    fn_801DAC3C();
    /* mr. r28, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_8025E098;
    r4 = 0x1;
    fn_800E90C8();
    fn_80115BD8();
    /* mr. r26, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_8025E098;
    r3 = 0x1;
    fn_8018F470();
    r23 = r3;
    r3 = r26;
    fn_80115280();
    r29 = r3;
    r27 = 0x0;
    r31 = r27;
    if ((u32)r29 != (u32)0x1) goto L_8025E098;
    r25 = r27;
    r30 = r1 + 0x8;
    goto L_8025E05C;
L_8025E028: ;
    r3 = r26;
    r4 = r25;
    fn_8011538C();
    r24 = r3;
    fn_800FF56C();
    r4 = r24;
    fn_80113F6C();
    if ((u32)r3 == (u32)0x0) goto L_8025E058;
    *(u32*)(r30 + r31) = r3;
    r27 = r27 + 0x1;
    r31 = r31 + 0x4;
L_8025E058: ;
    r25 = r25 + 0x1;
L_8025E05C: ;
    if ((u32)r25 < (u32)r29) goto L_8025E028;
    r3 = r28;
    r4 = 0x1;
    fn_800E9108();
    r3 = r28;
    r4 = r23;
    fn_800E8FE8();
    r3 = r28;
    r4 = r27;
    r5 = r1 + 0x8;
    fn_800E900C();
    r3 = r28;
    r4 = 0x1;
    fn_800E3C94();
L_8025E098: ;
    r3 = 0x1;
L_8025E09C: ;
    /* lmw r23, 0x4c(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025E0B0 | Size: 0x10C (268 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025E0B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8011F5B0();
    extern void fn_801DDD28();
    extern void fn_801DE190();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r31 = r4;
    r27 = r3;
    r28 = r5;
    r29 = r6;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_8025E0F4;
    r3 = 0x0;
    goto L_8025E1A8;
L_8025E0F4: ;
    r3 = 0x0;
    r5 = 0x66;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_8025E114;
    r3 = 0x0;
    goto L_8025E1A8;
L_8025E114: ;
    r3 = r31;
    fn_8011F5B0();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    r4 = 0x0;
    r5 = 0xc1;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = -r3;
    r4 = r31;
    r0 = r0 | r3;
    r3 = r30;
    r5 = (u32)r0 >> 31;
    fn_801DE190();
    *(u32*)((u8*)r27 + 0x0) = r3;
    if ((u32)r3 != (u32)0x0) goto L_8025E164;
    r3 = 0x0;
    goto L_8025E1A8;
L_8025E164: ;
    r31 = r28;
    r30 = 0x0;
    goto L_8025E19C;
L_8025E170: ;
    r3 = *(u32*)((u8*)r27 + 0x0);
    r6 = 0x0;
    r4 = *(u16*)((u8*)r31 + 0x0);
    r5 = *(u16*)((u8*)r31 + 0x2);
    fn_801DDD28();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8025E194;
    r3 = 0x0;
    goto L_8025E1A8;
L_8025E194: ;
    r31 = r31 + 0x4;
    r30 = r30 + 0x1;
L_8025E19C: ;
    if ((s32)r30 < (s32)r29) goto L_8025E170;
    r3 = 0x1;
L_8025E1A8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025E1BC | Size: 0x1F4 (500 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025E1BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478DA0[];
    extern u8 lbl_8047B658[];
    extern u8 lbl_8047E678[];
    extern u8 lbl_8047E67C[];
    extern void fn_800E01F4();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DAC90();
    extern void fn_801DADC0();
    extern void fn_8025DDAC();
    extern void fn_8025DDF4();
    extern void fn_8025DE0C();
    extern void fn_8025DE54();
    extern void fn_8025DF38();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x80) = f31;
    /* psq_st f31, 0x88(r1), 0, qr0 */;
    *(f64*)(sp + 0x70) = f30;
    /* psq_st f30, 0x78(r1), 0, qr0 */;
    *(f64*)(sp + 0x60) = f29;
    /* psq_st f29, 0x68(r1), 0, qr0 */;
    *(f64*)(sp + 0x50) = f28;
    /* psq_st f28, 0x58(r1), 0, qr0 */;
    *(f64*)(sp + 0x40) = f27;
    /* psq_st f27, 0x48(r1), 0, qr0 */;
    *(f64*)(sp + 0x30) = f26;
    /* psq_st f26, 0x38(r1), 0, qr0 */;
    f26 = f1;
    r3 = 0x1;
    f27 = f2;
    f28 = f3;
    f29 = f4;
    f30 = f5;
    f31 = f6;
    fn_801DADC0();
    r3 = (u32)lbl_8047B658;
    fn_8025DDF4();
    r3 = (u32)lbl_8047B658;
    r4 = 0x3f;
    r5 = (u32)lbl_80478DA0;
    r6 = 0x1;
    fn_8025DF38();
    r0 = r3 & 0xFF;
    r0 = 0x1 - r0;
    r0 = __cntlzw(r0);
    /* extrwi r0, r0, 8, 19 */;
    if ((u32)r0 != (u32)0x1) goto L_8025E364;
    r31 = (u32)lbl_8047B658;
    if ((u32)r31 == (u32)0x0) goto L_8025E270;
    r0 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8025E270;
    goto L_8025E274;
L_8025E270: ;
    r31 = 0x0;
L_8025E274: ;
    r30 = (u32)lbl_8047B658;
    if ((u32)r30 == (u32)0x0) goto L_8025E290;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8025E290;
    goto L_8025E294;
L_8025E290: ;
    r30 = 0x0;
L_8025E294: ;
    if ((u32)r30 == (u32)0x0) goto L_8025E2C8;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8025E2C8;
    f1 = f26;
    r3 = r1 + 0x14;
    f2 = f27;
    f3 = f28;
    fn_800E01F4();
    r3 = r30;
    r4 = r1 + 0x14;
    fn_8025DE0C();
L_8025E2C8: ;
    r3 = (u32)lbl_8047B658;
    if ((u32)r3 == (u32)0x0) goto L_8025E2E4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8025E2E4;
    goto L_8025E2E8;
L_8025E2E4: ;
    r3 = 0x0;
L_8025E2E8: ;
    if ((u32)r3 == (u32)0x0) goto L_8025E320;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8025E320;
    f0 = *(f32*)lbl_8047E678;
    r4 = r1 + 0x8;
    f2 = f0 * f29;
    f1 = f0 * f30;
    f0 = f0 * f31;
    *(f32*)(sp + 0x8) = f2;
    *(f32*)(sp + 0xC) = f1;
    *(f32*)(sp + 0x10) = f0;
    fn_8025DDAC();
L_8025E320: ;
    if ((u32)r31 == (u32)0x0) goto L_8025E364;
    r0 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8025E364;
    r3 = r31;
    r4 = (u32)lbl_80478DA0;
    r5 = 0x1;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x1;
    fn_8025DE54();
    f1 = *(f32*)lbl_8047E67C;
    r3 = 0x5;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
L_8025E364: ;
    fn_801DAC90();
    /* psq_l f31, 0x88(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x80);
    /* psq_l f30, 0x78(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x70);
    /* psq_l f29, 0x68(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x60);
    /* psq_l f28, 0x58(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0x50);
    /* psq_l f27, 0x48(r1), 0, qr0 */;
    f27 = *(f64*)(sp + 0x40);
    /* psq_l f26, 0x38(r1), 0, qr0 */;
    f26 = *(f64*)(sp + 0x30);
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    return;
}
#pragma pop

/* Address: 0x8025E3B0 | Size: 0x184 (388 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025E3B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478288[];
    extern u8 lbl_80478DB0[];
    extern u8 lbl_8047E680[];
    extern void fn_80097A38();
    extern void fn_800D1FDC();
    extern void fn_800D2584();
    extern void fn_80177830();
    extern void fn_801778B4();
    extern void fn_80177908();
    extern void fn_8017795C();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DAC90();
    extern void fn_801DADC0();
    extern void fn_8025DE54();
    extern void fn_8025E0B0();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    f1 = *(f32*)lbl_8047E680;
    r31 = r3;
    r3 = 0x3;
    r28 = r4;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r1 + 0x1c;
    fn_801778B4();
    r3 = r1 + 0x28;
    fn_80177830();
    r3 = r1 + 0x34;
    fn_80177908();
    r3 = r1 + 0x40;
    fn_8017795C();
    fn_800D2584();
    r4 = r1 + 0xc;
    r5 = r1 + 0x10;
    r6 = r1 + 0x14;
    r7 = r1 + 0x18;
    fn_800D1FDC();
    r3 = (u32)lbl_80478288;
    r30 = *(u32*)(sp + 0x1C);
    r29 = (u32)lbl_80478288;
    r12 = *(u32*)(sp + 0x20);
    r11 = *(u32*)(sp + 0x24);
    r10 = *(u32*)(sp + 0x28);
    r9 = *(u32*)(sp + 0x2C);
    r8 = *(u32*)(sp + 0x30);
    r7 = *(u32*)(sp + 0x34);
    r6 = *(u32*)(sp + 0x38);
    r5 = *(u32*)(sp + 0x3C);
    r4 = *(u32*)(sp + 0x40);
    r3 = *(u32*)(sp + 0x44);
    r0 = *(u32*)(sp + 0x48);
    f0 = *(f32*)(sp + 0xC);
    *(u32*)((u8*)r29 + 0x0) = r30;
    *(u32*)((u8*)r29 + 0x4) = r12;
    *(u32*)((u8*)r29 + 0x8) = r11;
    *(u32*)((u8*)r29 + 0xC) = r10;
    *(u32*)((u8*)r29 + 0x10) = r9;
    *(u32*)((u8*)r29 + 0x14) = r8;
    *(u32*)((u8*)r29 + 0x18) = r7;
    *(u32*)((u8*)r29 + 0x1C) = r6;
    *(u32*)((u8*)r29 + 0x20) = r5;
    *(u32*)((u8*)r29 + 0x24) = r4;
    *(u32*)((u8*)r29 + 0x28) = r3;
    *(u32*)((u8*)r29 + 0x2C) = r0;
    *(f32*)((u8*)r29 + 0x30) = f0;
    fn_801DAC90();
    r3 = r31;
    r4 = r28;
    fn_80097A38();
    r29 = r3;
    if ((s32)r29 < (s32)0x4) goto L_8025E4B0;
    r29 = -0x1;
L_8025E4B0: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = 0x1;
    fn_801DADC0();
    r4 = r31;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_80478DB0;
    r6 = 0x1;
    fn_8025E0B0();
    if ((s32)r3 != (s32)0x1) goto L_8025E510;
    f1 = *(f32*)lbl_8047E680;
    r3 = 0x2;
    fn_801C41C8();
    r3 = r1 + 0x8;
    r4 = (u32)lbl_80478DB0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x1;
    r8 = 0x0;
    fn_8025DE54();
L_8025E510: ;
    r3 = (s8)r29;
    r31 = *(u32*)(sp + 0x5C);
    r30 = *(u32*)(sp + 0x58);
    r29 = *(u32*)(sp + 0x54);
    r28 = *(u32*)(sp + 0x50);
    return;
}
#pragma pop

/* Address: 0x8025E534 | Size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025E534(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8027A478[];
    extern u8 lbl_8047B660[];
    extern u8 lbl_8047B664[];
    extern u8 lbl_8047E684[];
    extern void fn_800F9318();
    extern void fn_80113F48();
    extern void fn_80118A68();
    extern void fn_80118F04();
    extern void fn_801190DC();
    extern void fn_8025DC2C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f1 = 0.0f;

    r3 = (u32)lbl_8027A478;
    r5 = (u32)lbl_8027A478;
    r0 = *(u32*)lbl_8047B664;
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x10) = r0;
    if ((s32)r0 != (s32)0x1) goto L_8025E588;
    if ((s32)r0 != (s32)0x1) goto L_8025E588;
    r3 = *(u32*)lbl_8047B660;
    r4 = 0x1;
    fn_80118A68();
    r0 = 0x0;
    *(u32*)lbl_8047B660 = r0;
    *(u32*)lbl_8047B664 = r0;
L_8025E588: ;
    fn_80113F48();
    r4 = (0x108a << 16);
    r4 = r4 + 0x1400;
    fn_800F9318();
    r4 = 0x0;
    r5 = 0x0;
    fn_801190DC();
    *(u32*)lbl_8047B660 = r3;
    r4 = r1 + 0x8;
    fn_80118F04();
    r0 = 0x1;
    f1 = *(f32*)lbl_8047E684;
    *(u32*)lbl_8047B664 = r0;
    fn_8025DC2C();
    r0 = *(u32*)lbl_8047B664;
    if ((s32)r0 != (s32)0x1) goto L_8025E5E4;
    r3 = *(u32*)lbl_8047B660;
    r4 = 0x1;
    fn_80118A68();
    r0 = 0x0;
    *(u32*)lbl_8047B660 = r0;
    *(u32*)lbl_8047B664 = r0;
L_8025E5E4: ;
    return;
}
#pragma pop

/* Address: 0x8025E5F4 | Size: 0x4C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025E5F4(void* ctx, u32 slot, u32 param) {
    extern void fn_800F07A8();
    extern void fn_800FF560();
    extern void fn_801CAF0C();
    extern void fn_8025E534();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    fn_801CAF0C();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_8025E630;
    fn_800FF560();
    r5 = (u32)fn_8025E534;
    r4 = r3;
    r8 = (u32)fn_8025E534;
    r3 = 0x1;
    r5 = 0x4000;
    r6 = 0x1;
    r7 = 0x1;
    fn_800F07A8();
L_8025E630: ;
    return;
}
#pragma pop

/* Address: 0x8025E640 | Size: 0x37C (892 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025E640(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80105C68();
    extern void fn_80105E7C();
    extern void fn_801067E8();
    extern void fn_8011DE98();
    extern void fn_8011F4A8();
    extern void fn_80122370();
    extern void fn_801229F4();
    extern void fn_80123090();
    extern void fn_801236F8();
    extern void fn_80123B5C();
    extern void fn_80123D58();
    extern void fn_8012546C();
    extern void fn_80132A38();
    extern void fn_80165668();
    extern void fn_802600E4();
    extern void fn_8025E3B0();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r5 = 0x7a;
    r6 = 0x0;
    r0 = 0x0;
    /* stmw r21, 0x34(r1) */;
    r26 = r4;
    r23 = r3;
    r24 = 0x0;
    r4 = 0x0;
    *(u8*)(sp + 0x8) = r0;
    ((void(*)(void))fn_8012640C)();
L_8025E674: ;
    if ((u32)r26 != (u32)0x0) goto L_8025E698;
    r3 = r23;
    r7 = r26;
    r4 = 0x0;
    r5 = 0xc6;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_8025E9A4;
L_8025E698: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r21 = r3 & 0xFF;
    if ((u32)r21 >= (u32)0x64) goto L_8025E9A4;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r21 + 0x1;
    r25 = r3;
    r3 = r23;
    r4 = r0 & 0xFF;
    fn_801229F4();
    r0 = r3;
    r3 = r23;
    r21 = r0;
    r4 = 0x0;
    r5 = 0x87;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = (s16)r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x88;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = (s16)r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x89;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r29 = (s16)r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x8a;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r28 = (s16)r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x8b;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r27 = (s16)r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x8c;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r7 = r25 + r26;
    r22 = (s16)r3;
    if ((u32)r7 < (u32)r21) goto L_8025E988;
    r3 = r23;
    r4 = r21;
    r26 = r7 - r21;
    r24 = 0x1;
    fn_8011DE98();
    r3 = r23;
    fn_8012546C();
    r3 = r23;
    fn_80123090();
    r0 = r3;
    r3 = r23;
    r4 = r0;
    r5 = 0x0;
    fn_80122370();
    r3 = r23;
    fn_8011F4A8();
    r0 = r3;
    r3 = 0x4ca;
    r25 = r0;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    r4 = r25 & 0xFF;
    r3 = 0x2f;
    fn_80132A38();
    r3 = 0x44ce;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    r3 = r23;
    r4 = 0x0;
    r5 = 0x87;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    *(u16*)(sp + 0xE) = r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x88;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    *(u16*)(sp + 0x10) = r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x89;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    *(u16*)(sp + 0x12) = r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x8a;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    *(u16*)(sp + 0x16) = r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x8b;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    *(u16*)(sp + 0x18) = r3;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x8c;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = *(s16*)((u8*)r1 + 0xE);
    r5 = (s16)r3;
    r0 = *(s16*)((u8*)r1 + 0x10);
    r5 = r5 - r22;
    r10 = r4 - r31;
    r4 = *(s16*)((u8*)r1 + 0x12);
    r9 = r0 - r30;
    r0 = *(s16*)((u8*)r1 + 0x16);
    r8 = r4 - r29;
    r4 = *(s16*)((u8*)r1 + 0x18);
    r7 = r0 - r28;
    r0 = 0x1;
    r6 = r4 - r27;
    *(u16*)(sp + 0x14) = r3;
    r3 = r1 + 0x1c;
    r4 = 0x1;
    *(u16*)(sp + 0x1E) = r10;
    *(u16*)(sp + 0x20) = r9;
    *(u16*)(sp + 0x22) = r8;
    *(u16*)(sp + 0x26) = r7;
    *(u16*)(sp + 0x28) = r6;
    *(u16*)(sp + 0x24) = r5;
    *(u8*)(sp + 0x1C) = r0;
    fn_80105E7C();
    r0 = 0x0;
    r3 = r1 + 0xc;
    *(u8*)(sp + 0xC) = r0;
    r4 = 0x1;
    fn_80105E7C();
    r3 = 0x1;
    fn_80105C68();
    r0 = 0x0;
    *(u8*)(sp + 0x8) = r0;
    goto L_8025E968;
L_8025E90C: ;
    r3 = r23;
    r4 = r21;
    fn_80123B5C();
    r0 = (s8)r3;
    if ((s32)r0 != (s32)-0x1) goto L_8025E95C;
    r4 = (u32)fn_8025E3B0;
    r3 = r23;
    r7 = (u32)fn_8025E3B0;
    r5 = r1 + 0x9;
    r4 = r21;
    r6 = 0x0;
    r8 = 0x0;
    fn_802600E4();
    if ((s32)r3 == (s32)0x0) goto L_8025E95C;
    r4 = *(u8*)(sp + 0x9);
    r3 = r23;
    r5 = r21;
    fn_80123D58();
L_8025E95C: ;
    r3 = *(u8*)(sp + 0x8);
    r0 = r3 + 0x1;
    *(u8*)(sp + 0x8) = r0;
L_8025E968: ;
    r3 = r23;
    r4 = r25;
    r5 = r1 + 0x8;
    fn_801236F8();
    r0 = r3 & 0xFFFF;
    r21 = r3;
    if ((s32)r3 != (s32)0x0) goto L_8025E90C;
    goto L_8025E674;
L_8025E988: ;
    r3 = r23;
    r26 = 0x0;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_8025E674;
L_8025E9A4: ;
    r3 = r24;
    /* lmw r21, 0x34(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025E9BC | Size: 0x390 (912 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025E9BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478DB0[];
    extern u8 lbl_8047E680[];
    extern u8 lbl_8047E688[];
    extern void fn_8001E184();
    extern void fn_80029660();
    extern void fn_801065B8();
    extern void fn_801067E8();
    extern void fn_8011D904();
    extern void fn_8011DE68();
    extern void fn_8011EDF8();
    extern void fn_8011EE10();
    extern void fn_8011EE40();
    extern void fn_8011EE58();
    extern void fn_8011F228();
    extern void fn_8011F4F0();
    extern void fn_8011FBCC();
    extern void fn_80121ADC();
    extern void fn_80121B4C();
    extern void fn_80123FBC();
    extern void fn_8012805C();
    extern void fn_80128A64();
    extern void fn_8012A5B0();
    extern void fn_80132A38();
    extern void fn_80165668();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DAC90();
    extern void fn_801DADC0();
    extern void fn_801EECD8();
    extern void fn_8025DE54();
    extern void fn_8025E0B0();
    extern void fn_8025E640();
    extern void fn_80265E7C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f7 = 0.0f;

    r5 = r3 & 0xFFFF;
    r4 = 0x3;
    r0 = 0x0;
    r30 = r3;
    r3 = 0x0;
    *(u16*)(sp + 0x8) = r0;
    fn_8012A5B0();
    r31 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025ED2C;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r31;
    fn_8011EE40();
    r4 = 0x1;
    fn_801EECD8();
    r3 = r31;
    r4 = 0x3e;
    fn_80121ADC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025EA4C;
    r3 = r31;
    r4 = 0x3e;
    fn_80121B4C();
L_8025EA4C: ;
    f1 = *(f32*)lbl_8047E688;
    r3 = r31;
    fn_8011FBCC();
    r3 = 0x3f7;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    r3 = r31;
    r4 = 0x0;
    fn_8011F228();
    r0 = r3 & 0xFFFF;
    r29 = r3;
    if ((u32)r0 == (u32)0x1) goto L_8025EAB4;
    r3 = r31;
    fn_8011F4F0();
    r0 = r3;
    r3 = 0x32;
    r4 = r0;
    fn_80132A38();
    r4 = r29 & 0xFFFF;
    r3 = 0x39;
    fn_80132A38();
    r3 = 0x3b10;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
L_8025EAB4: ;
    r3 = r31;
    fn_8011EE10();
    r0 = r3;
    r3 = 0x2f;
    r28 = r0;
    r4 = r28;
    fn_80132A38();
    r3 = 0x3b0b;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    r3 = 0x1;
    fn_801065B8();
    r3 = r31;
    fn_8011EE58();
    r29 = r3;
    r3 = r31;
    fn_8011EDF8();
    r0 = r3 + 0x46;
    r3 = r31;
    r0 = r29 + r0;
    r4 = r0 & 0xFFFF;
    fn_8011D904();
    r3 = r31;
    r4 = 0x0;
    fn_8011DE68();
    r3 = r31;
    r4 = r28;
    fn_8025E640();
    if ((s32)r3 != (s32)0x1) goto L_8025EC64;
    r3 = r31;
    r6 = r1 + 0x8;
    r7 = r1 + 0x18;
    r4 = 0x0;
    r5 = 0x0;
    fn_80128A64();
    r0 = r3 & 0xFFFF;
    r28 = r3;
    if ((s32)r3 == (s32)0x1) goto L_8025EC64;
    if ((u32)r0 == (u32)0xffff) goto L_8025EC64;
    f1 = *(f32*)lbl_8047E680;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    fn_801DAC90();
    r5 = *(u16*)(sp + 0x8);
    r3 = r31;
    r4 = r28;
    r6 = r1 + 0x18;
    r7 = 0x0;
    r8 = 0x1;
    r9 = 0x1;
    r10 = 0x0;
    fn_8012805C();
    if ((s32)r3 != (s32)0x0) goto L_8025EC04;
    r3 = 0x1;
    fn_801DADC0();
    r4 = r31;
    r3 = r1 + 0x14;
    r5 = (u32)lbl_80478DB0;
    r6 = 0x1;
    fn_8025E0B0();
    if ((s32)r3 != (s32)0x1) goto L_8025EBEC;
    f1 = *(f32*)lbl_8047E680;
    r3 = 0x2;
    fn_801C41C8();
    r3 = r1 + 0x14;
    r4 = (u32)lbl_80478DB0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x1;
    r8 = 0x0;
    fn_8025DE54();
L_8025EBEC: ;
    r3 = r31;
    fn_8011F4F0();
    r4 = r3;
    r3 = 0x32;
    fn_80132A38();
    goto L_8025EC64;
L_8025EC04: ;
    r3 = r31;
    fn_8011F4F0();
    r4 = r3;
    r3 = 0x32;
    fn_80132A38();
    r3 = 0x1;
    fn_801DADC0();
    r4 = r31;
    r3 = r1 + 0x10;
    r5 = (u32)lbl_80478DB0;
    r6 = 0x1;
    fn_8025E0B0();
    if ((s32)r3 != (s32)0x1) goto L_8025EC64;
    f1 = *(f32*)lbl_8047E680;
    r3 = 0x2;
    fn_801C41C8();
    r3 = r1 + 0x10;
    r4 = (u32)lbl_80478DB0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x1;
    r8 = 0x0;
    fn_8025DE54();
L_8025EC64: ;
    r3 = 0x3ca;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    r3 = r31;
    fn_80265E7C();
    r3 = 0x3b0c;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    r3 = 0x3b0d;
    r4 = 0x1;
    r5 = 0x1;
    fn_801067E8();
    fn_8001E184();
    r0 = (s8)r3;
    if ((s32)r3 == (s32)0x1) goto L_8025ECB4;
    r3 = 0x1;
    fn_801065B8();
    goto L_8025ED2C;
L_8025ECB4: ;
    r3 = 0x1;
    fn_801065B8();
    f1 = *(f32*)lbl_8047E680;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    fn_801DAC90();
    r4 = r30;
    r3 = 0x2;
    fn_80029660();
    r3 = 0x1;
    fn_801DADC0();
    r4 = r31;
    r3 = r1 + 0xc;
    r5 = (u32)lbl_80478DB0;
    r6 = 0x1;
    fn_8025E0B0();
    if ((s32)r3 != (s32)0x1) goto L_8025ED2C;
    f1 = *(f32*)lbl_8047E680;
    r3 = 0x2;
    fn_801C41C8();
    r3 = r1 + 0xc;
    r4 = (u32)lbl_80478DB0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x1;
    r8 = 0x0;
    fn_8025DE54();
L_8025ED2C: ;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    r28 = *(u32*)(sp + 0x20);
    return;
}
#pragma pop

/* Address: 0x8025ED4C | Size: 0x20C (524 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025ED4C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8039A6A8[];
    extern u8 lbl_804782BC[];
    extern u8 lbl_80478DA8[];
    extern u8 lbl_8047B668[];
    extern u8 lbl_8047E680[];
    extern void fn_800F0308();
    extern void fn_800FF660();
    extern void fn_8011288C();
    extern void fn_80123FBC();
    extern void fn_8012A5B0();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DAC90();
    extern void fn_801DADC0();
    extern void fn_8025DE54();
    extern void fn_8025E0B0();
    extern void fn_8025E9BC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r3 = (u32)lbl_804782BC;
    r0 = *(u32*)lbl_804782BC;
    if ((s32)r0 == (s32)0x0) goto L_8025ED78;
    if ((s32)r0 != (s32)0x1) goto L_8025EF40;
L_8025ED78: ;
    r3 = 0x1;
    fn_801DADC0();
    r3 = (u32)lbl_804782BC;
    r31 = (u32)lbl_804782BC;
    r0 = *(u32*)((u8*)r31 + 0x0);
    if ((s32)r0 != (s32)0x0) goto L_8025EE30;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = 0x0;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    fn_8012A5B0();
    r30 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025EDC4;
    r0 = 0x0;
    goto L_8025EE1C;
L_8025EDC4: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r30;
    r3 = r1 + 0x8;
    r5 = (u32)lbl_80478DA8;
    r6 = 0x2;
    fn_8025E0B0();
    if ((s32)r3 != (s32)0x1) goto L_8025EE18;
    r3 = r1 + 0x8;
    r4 = (u32)lbl_80478DA8;
    r5 = 0x2;
    r6 = 0x1;
    r7 = 0x1;
    r8 = 0x0;
    fn_8025DE54();
    r0 = 0x1;
    goto L_8025EE1C;
L_8025EE18: ;
    r0 = 0x0;
L_8025EE1C: ;
    if ((s32)r0 != (s32)0x1) goto L_8025EEF0;
    r3 = *(u32*)((u8*)r31 + 0x8);
    fn_8025E9BC();
    goto L_8025EEF0;
L_8025EE30: ;
    if ((s32)r0 != (s32)0x1) goto L_8025EEF0;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = 0x0;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    fn_8012A5B0();
    r30 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8025EE68;
    r0 = 0x0;
    goto L_8025EEE0;
L_8025EE68: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xc5;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = (u32)lbl_8039A6A8;
    r4 = r30;
    r5 = (u32)lbl_8039A6A8;
    r6 = 0x4;
    r3 = r1 + 0x8;
    fn_8025E0B0();
    if ((s32)r3 != (s32)0x1) goto L_8025EEDC;
    r4 = (u32)lbl_8039A6A8;
    r3 = r1 + 0x8;
    r4 = (u32)lbl_8039A6A8;
    r5 = 0x4;
    r6 = 0x1;
    r7 = 0x1;
    r8 = 0x0;
    fn_8025DE54();
    r0 = 0x1;
    goto L_8025EEE0;
L_8025EEDC: ;
    r0 = 0x0;
L_8025EEE0: ;
    if ((s32)r0 != (s32)0x1) goto L_8025EEF0;
    r3 = *(u32*)((u8*)r31 + 0x8);
    fn_8025E9BC();
L_8025EEF0: ;
    f1 = *(f32*)lbl_8047E680;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    fn_801DAC90();
    r5 = -0x1;
    r3 = (u32)lbl_804782BC;
    /* stwu r5, lbl_804782BC@l(r3) */;
    r0 = 0x0;
    r4 = (u32)lbl_8047B668;
    *(u32*)lbl_8047B668 = r5;
    *(u32*)((u8*)r4 + 0x4) = r5;
    *(u16*)((u8*)r3 + 0x4) = r0;
    *(u32*)((u8*)r3 + 0x8) = r0;
    fn_800FF660();
    r3 = 0x0;
    r4 = 0x0;
    fn_8011288C();
    fn_800F0308();
L_8025EF40: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* Address: 0x8025EF58 | Size: 0x354 (852 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025EF58(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_804782BC[];
    extern u8 lbl_8047B660[];
    extern u8 lbl_8047B664[];
    extern u8 lbl_8047B668[];
    extern u8 lbl_8047E680[];
    extern u8 lbl_8047E68C[];
    extern void fn_8001BDF4();
    extern void fn_800F0308();
    extern void fn_800FF730();
    extern void fn_801065B8();
    extern void fn_801067E8();
    extern void fn_8011288C();
    extern void fn_80118A68();
    extern void fn_8011EE40();
    extern void fn_8011FC74();
    extern void fn_80123FBC();
    extern void fn_80129280();
    extern void fn_80129B2C();
    extern void fn_8012A5B0();
    extern void fn_8012AC08();
    extern void fn_80165668();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801CAF0C();
    extern void fn_801EEC74();
    extern void fn_8025DCBC();
    extern void fn_8025DD14();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r0 = *(u32*)lbl_8047B668;
    if ((s32)r0 == (s32)-0x1) goto L_8025F28C;
    r29 = 0x0;
    r3 = 0x0;
    r4 = 0x0;
    fn_80129280();
    r3 = 0x0;
    r4 = 0x2;
    fn_80129280();
    r28 = 0x0;
    r31 = r3;
    goto L_8025EFF4;
L_8025EFA8: ;
    r3 = r31;
    r4 = r28 & 0xFFFF;
    fn_8012AC08();
    r30 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)-0x1) goto L_8025EFF0;
    r3 = r30;
    fn_8011FC74();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0x1) goto L_8025EFF0;
    r3 = r30;
    fn_8011EE40();
    fn_801EEC74();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0x1) goto L_8025EFF0;
    r29 = r29 + 0x1;
L_8025EFF0: ;
    r28 = r28 + 0x1;
L_8025EFF4: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8025EFA8;
    fn_801CAF0C();
    r31 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    fn_80129280();
    r3 = 0x0;
    r4 = 0x2;
    fn_80129280();
    r3 = 0x0;
    r4 = 0x219;
    fn_80129B2C();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0x1) goto L_8025F060;
    r0 = r29 & 0xFFFF;
    if ((s32)r0 == (s32)0x1) goto L_8025F058;
    if ((u32)r31 == (u32)0x0) goto L_8025F050;
    r0 = 0x2;
    goto L_8025F074;
L_8025F050: ;
    r0 = 0x1;
    goto L_8025F074;
L_8025F058: ;
    r0 = 0x3;
    goto L_8025F074;
L_8025F060: ;
    if ((u32)r31 == (u32)0x0) goto L_8025F070;
    r0 = 0x4;
    goto L_8025F074;
L_8025F070: ;
    r0 = 0x5;
L_8025F074: ;
    r3 = *(u32*)lbl_8047B668;
    r4 = r0;
    if ((s32)r3 != (s32)0x0) goto L_8025F0B8;
    r0 = r0 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8025F098;
    r4 = 0x5;
    goto L_8025F0C4;
L_8025F098: ;
    if ((u32)r0 != (u32)0x2) goto L_8025F0A8;
    r4 = 0x4;
    goto L_8025F0C4;
L_8025F0A8: ;
    if ((u32)r0 != (u32)0x3) goto L_8025F0C4;
    r4 = 0x5;
    goto L_8025F0C4;
L_8025F0B8: ;
    if ((s32)r3 != (s32)0x1) goto L_8025F0C4;
    r4 = 0x2;
L_8025F0C4: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 != (u32)0x2) goto L_8025F174;
    r3 = r1 + 0x8;
    fn_8025DD14();
    r3 = 0x3c8;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    r28 = (u32)lbl_8047B668;
    r3 = 0x0;
    r0 = *(u32*)((u8*)r28 + 0x4);
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    fn_8012A5B0();
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r5 = 0x1;
    r4 = (u32)lbl_804782BC;
    /* stwu r5, lbl_804782BC@l(r4) */;
    r0 = *(u32*)((u8*)r28 + 0x4);
    *(u16*)((u8*)r4 + 0x4) = r3;
    r3 = 0x3;
    f1 = *(f32*)lbl_8047E680;
    *(u32*)((u8*)r4 + 0x8) = r0;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x385;
    fn_800FF730();
    r3 = 0x0;
    r4 = 0x0;
    fn_8011288C();
    fn_800F0308();
    f1 = *(f32*)lbl_8047E68C;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r1 + 0x8;
    fn_8025DCBC();
    goto L_8025F28C;
L_8025F174: ;
    if ((u32)r0 != (u32)0x4) goto L_8025F27C;
    r0 = *(u32*)lbl_8047B664;
    if ((s32)r0 != (s32)0x1) goto L_8025F1A0;
    r3 = *(u32*)lbl_8047B660;
    r4 = 0x1;
    fn_80118A68();
    r0 = 0x0;
    *(u32*)lbl_8047B660 = r0;
    *(u32*)lbl_8047B664 = r0;
L_8025F1A0: ;
    fn_801CAF0C();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0x1) goto L_8025F1C0;
    r0 = -0x1;
    r3 = (u32)lbl_8047B668;
    *(u32*)lbl_8047B668 = r0;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8025F28C;
L_8025F1C0: ;
    r3 = 0x3b0f;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    r3 = 0x1;
    fn_801065B8();
    r3 = 0x7;
    r4 = 0x0;
    r5 = 0x0;
    fn_8001BDF4();
    r31 = r3;
    if ((s32)r31 == (s32)-0x1) goto L_8025F28C;
    r5 = r31 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x3;
    fn_8012A5B0();
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = 0x0;
    r4 = (u32)lbl_804782BC;
    /* stwu r0, lbl_804782BC@l(r4) */;
    f1 = *(f32*)lbl_8047E680;
    *(u16*)((u8*)r4 + 0x4) = r3;
    r3 = 0x3;
    *(u32*)((u8*)r4 + 0x8) = r31;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r1 + 0x8;
    fn_8025DD14();
    r3 = 0x385;
    fn_800FF730();
    r3 = 0x0;
    r4 = 0x0;
    fn_8011288C();
    fn_800F0308();
    f1 = *(f32*)lbl_8047E68C;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r1 + 0x8;
    fn_8025DCBC();
    goto L_8025F28C;
L_8025F27C: ;
    r0 = -0x1;
    r3 = (u32)lbl_8047B668;
    *(u32*)lbl_8047B668 = r0;
    *(u32*)((u8*)r3 + 0x4) = r0;
L_8025F28C: ;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    r28 = *(u32*)(sp + 0x20);
    return;
}
#pragma pop

/* Address: 0x8025F2AC | Size: 0x14 | Pattern: null_check_getter */
u32 fn_8025F2AC(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025F2C0 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025F2C0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025F2FC | Size: 0x54 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025F2FC(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_804783E0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = (u32)lbl_804783E0;
    r3 = r3 << 8;
    r0 = (u32)lbl_804783E0;
    r3 = r0 + r3;
    r0 = *(u32*)((u8*)r3 + 0x20);
    if ((s32)r0 != (s32)0x0) return;
    r0 = *(u8*)((u8*)r3 + 0x5);
    if ((u32)r0 != (u32)0x0) goto L_8025F330;
    r0 = *(u8*)((u8*)r3 + 0x6);
    if ((u32)r0 == (u32)0x4) goto L_8025F33C;
L_8025F330: ;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x20) = r0;
    return;
L_8025F33C: ;
    r0 = *(u8*)((u8*)r3 + 0x7);
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r0 & 0x3a;
    *(u8*)((u8*)r3 + 0x0) = r0;
    return;
}
#pragma pop

/* Address: 0x8025F350 | Size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025F350(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8039A6B8[];
    extern u8 lbl_804782E0[];
    extern u8 lbl_804783E0[];
    extern u8 lbl_8047B670[];
    extern void fn_800AE7E0();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r3 = (0x8000 << 16);
    r4 = (u32)lbl_804783E0;
    /* stmw r27, 0xc(r1) */;
    r29 = (u32)lbl_804783E0;
    r27 = 0x0;
    r31 = 0x0;
    r0 = *(u32*)((u8*)r3 + 0xF8);
    r3 = (0x431c << 16);
    /* subi r3, r3, 0x217d */;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 15;
    r0 = r0 * 0x3c;
    r3 = (u32)lbl_804782E0;
    r28 = (u32)lbl_804782E0;
    r30 = (u32)r0 >> 3;
L_8025F39C: ;
    *(u32*)((u8*)r29 + 0x34) = r30;
    r3 = r29 + 0x24;
    *(u32*)((u8*)r29 + 0x30) = r31;
    OSInitThreadQueue();
    r27 = r27 + 0x1;
    *(u32*)((u8*)r29 + 0xF8) = r28;
    r29 = r29 + 0x100;
    r28 = r28 + 0x40;
    if ((s32)r27 < (s32)0x4) goto L_8025F39C;
    OSInitAlarm();
    fn_800AE7E0();
    r0 = 0x0;
    r3 = (u32)lbl_8039A6B8;
    *(u32*)lbl_8047B670 = r0;
    r3 = (u32)lbl_8039A6B8;
    OSRegisterResetFunction();
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025F3F4 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025F3F4(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_804783E0[];
    extern void fn_8025F81C();
    extern void fn_8025F9AC();
    extern void fn_8025F2FC();
    extern void fn_8025F7E8();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r31 = r3 + 0x0;
    r3 = (u32)lbl_804783E0;
    r5 = r31 << 8;
    r0 = (u32)lbl_804783E0;
    r7 = r0 + r5;
    r0 = *(u32*)((u8*)r7 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_8025F42C;
    r3 = 0x2;
    goto L_8025F45C;
L_8025F42C: ;
    r0 = 0x0;
    *(u8*)((u8*)r7 + 0x0) = r0;
    r5 = (u32)fn_8025F7E8;
    r0 = (u32)fn_8025F7E8;
    *(u32*)((u8*)r7 + 0x14) = r4;
    r3 = (u32)fn_8025F2FC;
    r6 = (u32)fn_8025F2FC;
    *(u32*)((u8*)r7 + 0x1C) = r0;
    r3 = r31 + 0x0;
    r4 = 0x1;
    r5 = 0x3;
    fn_8025F9AC();
L_8025F45C: ;
    if ((s32)r3 == (s32)0x0) goto L_8025F468;
    goto L_8025F470;
L_8025F468: ;
    r3 = r31;
    fn_8025F81C();
L_8025F470: ;
    return;
}
#pragma pop

/* Address: 0x8025F484 | Size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025F484(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_804783E0[];
    extern void fn_8025F81C();
    extern void fn_8025F9AC();
    extern void fn_8025F2FC();
    extern void fn_8025F7E8();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r31 = r3 + 0x0;
    r3 = (u32)lbl_804783E0;
    r5 = r31 << 8;
    r0 = (u32)lbl_804783E0;
    r7 = r0 + r5;
    r0 = *(u32*)((u8*)r7 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_8025F4BC;
    r3 = 0x2;
    goto L_8025F4EC;
L_8025F4BC: ;
    r0 = 0xff;
    *(u8*)((u8*)r7 + 0x0) = r0;
    r5 = (u32)fn_8025F7E8;
    r0 = (u32)fn_8025F7E8;
    *(u32*)((u8*)r7 + 0x14) = r4;
    r3 = (u32)fn_8025F2FC;
    r6 = (u32)fn_8025F2FC;
    *(u32*)((u8*)r7 + 0x1C) = r0;
    r3 = r31 + 0x0;
    r4 = 0x1;
    r5 = 0x3;
    fn_8025F9AC();
L_8025F4EC: ;
    if ((s32)r3 == (s32)0x0) goto L_8025F4F8;
    goto L_8025F500;
L_8025F4F8: ;
    r3 = r31;
    fn_8025F81C();
L_8025F500: ;
    return;
}
#pragma pop

/* Address: 0x8025F514 | Size: 0x10 | Pattern: sda_getter */
u32 fn_8025F514(void) { return 0; /* stub */ }

/* Address: 0x8025F524 | Size: 0x60 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025F524(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_804783E0[];
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_804783E0;
    r3 = r3 << 8;
    r0 = (u32)lbl_804783E0;
    r31 = r0 + r3;
    r0 = *(u32*)((u8*)r31 + 0x20);
    if ((s32)r0 != (s32)0x0) goto L_8025F570;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r4 = r31 + 0x5;
    r5 = 0x4;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u8*)((u8*)r31 + 0x9);
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x3a;
    *(u8*)((u8*)r3 + 0x0) = r0;
L_8025F570: ;
    return;
}
#pragma pop

/* Address: 0x8025F584 | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025F584(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_804783E0[];
    extern void fn_8025F81C();
    extern void fn_8025F9AC();
    extern void fn_8025F524();
    extern void fn_8025F7E8();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;

    r31 = r3 + 0x0;
    r3 = (u32)lbl_804783E0;
    r6 = r31 << 8;
    r0 = (u32)lbl_804783E0;
    r8 = r0 + r6;
    r0 = *(u32*)((u8*)r8 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_8025F5BC;
    r3 = 0x2;
    goto L_8025F5F0;
L_8025F5BC: ;
    r0 = 0x14;
    *(u8*)((u8*)r8 + 0x0) = r0;
    r7 = (u32)fn_8025F7E8;
    r3 = (u32)fn_8025F524;
    *(u32*)((u8*)r8 + 0x18) = r4;
    r6 = (u32)fn_8025F524;
    r0 = (u32)fn_8025F7E8;
    *(u32*)((u8*)r8 + 0x14) = r5;
    r3 = r31;
    r4 = 0x1;
    *(u32*)((u8*)r8 + 0x1C) = r0;
    r5 = 0x5;
    fn_8025F9AC();
L_8025F5F0: ;
    if ((s32)r3 == (s32)0x0) goto L_8025F5FC;
    goto L_8025F604;
L_8025F5FC: ;
    r3 = r31;
    fn_8025F81C();
L_8025F604: ;
    return;
}
#pragma pop

/* Address: 0x8025F618 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_8025F618(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025F648 | Size: 0xC4 (196 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025F648(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_804783E0[];
    extern void fn_8025F81C();
    extern void fn_8025F9AC();
    extern void fn_8025F618();
    extern void fn_8025F7E8();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r5 + 0x0;
    r29 = r4 + 0x0;
    r28 = r3 + 0x0;
    r3 = (u32)lbl_804783E0;
    r6 = r28 << 8;
    r0 = (u32)lbl_804783E0;
    r31 = r0 + r6;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_8025F694;
    r3 = 0x2;
    goto L_8025F6D8;
L_8025F694: ;
    r0 = 0x15;
    *(u8*)((u8*)r31 + 0x0) = r0;
    r4 = r29 + 0x0;
    r3 = r31 + 0x1;
    r5 = 0x4;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    *(u32*)((u8*)r31 + 0x18) = r29;
    r4 = (u32)fn_8025F7E8;
    r0 = (u32)fn_8025F7E8;
    *(u32*)((u8*)r31 + 0x14) = r30;
    r3 = (u32)fn_8025F618;
    r6 = (u32)fn_8025F618;
    *(u32*)((u8*)r31 + 0x1C) = r0;
    r3 = r28 + 0x0;
    r4 = 0x5;
    r5 = 0x1;
    fn_8025F9AC();
L_8025F6D8: ;
    if ((s32)r3 == (s32)0x0) goto L_8025F6E4;
    goto L_8025F6EC;
L_8025F6E4: ;
    r3 = r28;
    fn_8025F81C();
L_8025F6EC: ;
    return;
}
#pragma pop

/* Address: 0x8025F70C | Size: 0xDC (220 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025F70C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_804783E0[];
    extern u8 lbl_8047B670[];
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;
    f32 f8 = 0.0f;

    r30 = r5 + 0x0;
    r29 = r3 + 0x0;
    r3 = (u32)lbl_804783E0;
    r0 = *(u32*)lbl_8047B670;
    r6 = r29 << 8;
    r0 = (u32)lbl_804783E0;
    r31 = r0 + r6;
    if ((s32)r0 != (s32)0x0) goto L_8025F7CC;
    r0 = r4 & 0xF;
    if ((s32)r0 == (s32)0x0) goto L_8025F75C;
    r0 = 0x1;
    *(u32*)((u8*)r31 + 0x20) = r0;
    goto L_8025F764;
L_8025F75C: ;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
L_8025F764: ;
    r12 = *(u32*)((u8*)r31 + 0x38);
    if ((u32)r12 == (u32)0x0) goto L_8025F784;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x38) = r0;
    r3 = r29;
    /* blrl  */;
L_8025F784: ;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_8025F7CC;
    r3 = r1 + 0x18;
    OSClearContext();
    r3 = r1 + 0x18;
    OSSetCurrentContext();
    r12 = *(u32*)((u8*)r31 + 0x1C);
    r0 = 0x0;
    r3 = r29 + 0x0;
    *(u32*)((u8*)r31 + 0x1C) = r0;
    r4 = *(u32*)((u8*)r31 + 0x20);
    /* blrl  */;
    r3 = r1 + 0x18;
    OSClearContext();
    r3 = r30;
    OSSetCurrentContext();
L_8025F7CC: ;
    return;
}
#pragma pop

/* Address: 0x8025F7E8 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8025F7E8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025F81C | Size: 0x6C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025F81C(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_804783E0[];
    extern void fn_800A238C();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_804783E0;
    r3 = r3 << 8;
    r0 = (u32)lbl_804783E0;
    r31 = r0 + r3;
    OSDisableInterrupts();
    r30 = r3;
    goto L_8025F854;
L_8025F84C: ;
    r3 = r31 + 0x24;
    fn_800A238C();
L_8025F854: ;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    if ((u32)r0 != (u32)0x0) goto L_8025F84C;
    r31 = *(u32*)((u8*)r31 + 0x20);
    r3 = r30;
    OSRestoreInterrupts();
    r3 = r31;
    return;
}
#pragma pop

/* Address: 0x8025F888 | Size: 0x124 (292 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025F888(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_804783E0[];
    extern u8 lbl_8047B670[];
    extern void fn_8009BBC4();
    extern void fn_8025F70C();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;

    r31 = r3 + 0x0;
    r3 = (u32)lbl_804783E0;
    r5 = r31 << 8;
    r0 = *(u32*)lbl_8047B670;
    r0 = (u32)lbl_804783E0;
    r30 = r0 + r5;
    if ((s32)r0 != (s32)0x0) goto L_8025F990;
    r0 = r4 & 0xFF;
    if ((s32)r0 != (s32)0x0) goto L_8025F8D8;
    /* clrrwi r3, r4, 16 */;
    /* subis r0, r3, 0x4 */;
    if ((u32)r0 == (u32)0x0) goto L_8025F8E4;
L_8025F8D8: ;
    r0 = 0x1;
    *(u32*)((u8*)r30 + 0x20) = r0;
    goto L_8025F91C;
L_8025F8E4: ;
    r3 = (u32)fn_8025F70C;
    r5 = *(u32*)((u8*)r30 + 0xC);
    r8 = (u32)fn_8025F70C;
    r7 = *(u32*)((u8*)r30 + 0x10);
    r9 = *(u32*)((u8*)r30 + 0x30);
    r3 = r31;
    r10 = *(u32*)((u8*)r30 + 0x34);
    r4 = r30 + 0x0;
    r6 = r30 + 0x5;
    SITransfer();
    if ((s32)r3 != (s32)0x0) goto L_8025F990;
    r0 = 0x2;
    *(u32*)((u8*)r30 + 0x20) = r0;
L_8025F91C: ;
    r12 = *(u32*)((u8*)r30 + 0x38);
    if ((u32)r12 == (u32)0x0) goto L_8025F93C;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x38) = r0;
    r3 = r31;
    /* blrl  */;
L_8025F93C: ;
    r0 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_8025F990;
    fn_8009BBC4();
    r29 = r3 + 0x0;
    r3 = r1 + 0x10;
    OSClearContext();
    r3 = r1 + 0x10;
    OSSetCurrentContext();
    r12 = *(u32*)((u8*)r30 + 0x1C);
    r0 = 0x0;
    r3 = r31 + 0x0;
    *(u32*)((u8*)r30 + 0x1C) = r0;
    r4 = *(u32*)((u8*)r30 + 0x20);
    /* blrl  */;
    r3 = r1 + 0x10;
    OSClearContext();
    r3 = r29;
    OSSetCurrentContext();
    __OSReschedule();
L_8025F990: ;
    return;
}
#pragma pop

/* Address: 0x8025F9AC | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025F9AC(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_804783E0[];
    extern void fn_800D0CBC();
    extern void fn_8025F888();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0x1c(r1) */;
    r27 = r3 + 0x0;
    r3 = (u32)lbl_804783E0;
    r7 = r27 << 8;
    r0 = (u32)lbl_804783E0;
    r28 = r4 + 0x0;
    r29 = r5 + 0x0;
    r31 = r6 + 0x0;
    r30 = r0 + r7;
    OSDisableInterrupts();
    *(u32*)((u8*)r30 + 0x38) = r31;
    r4 = (u32)fn_8025F888;
    r31 = r3 + 0x0;
    *(u32*)((u8*)r30 + 0xC) = r28;
    r4 = (u32)fn_8025F888;
    r3 = r27;
    *(u32*)((u8*)r30 + 0x10) = r29;
    fn_800D0CBC();
    r3 = r31;
    OSRestoreInterrupts();
    /* lmw r27, 0x1c(r1) */;
    r3 = 0x0;
    return;
}
#pragma pop

/* Address: 0x8025FA20 | Size: 0x1AC (428 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025FA20(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E690[];
    extern u8 lbl_8047E694[];
    extern u8 lbl_8047E698[];
    extern u8 lbl_8047E69C[];
    extern u8 lbl_8047E6A0[];
    extern u8 lbl_8047E6A4[];
    extern u8 lbl_8047E6A8[];
    extern u8 lbl_8047E6AC[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r0 = r3 & 0xFFFF;
    f1 = *(f32*)lbl_8047E690;
    if ((s32)r0 == (s32)0x92) goto L_8025FB38;
    if ((s32)r0 >= (s32)0x92) goto L_8025FAA4;
    if ((s32)r0 == (s32)0x4a) goto L_8025FB98;
    if ((s32)r0 >= (s32)0x4a) goto L_8025FA70;
    if ((s32)r0 == (s32)0x26) goto L_8025FB10;
    if ((s32)r0 >= (s32)0x26) goto L_8025FA64;
    if ((s32)r0 == (s32)0x1a) goto L_8025FB60;
    if ((s32)r0 >= (s32)0x1a) goto L_8025FBAC;
    if ((s32)r0 == (s32)0x6) goto L_8025FB68;
    goto L_8025FBAC;
L_8025FA64: ;
    if ((s32)r0 == (s32)0x44) goto L_8025FB88;
    goto L_8025FBAC;
L_8025FA70: ;
    if ((s32)r0 == (s32)0x8e) goto L_8025FB30;
    if ((s32)r0 >= (s32)0x8e) goto L_8025FA94;
    if ((s32)r0 == (s32)0x85) goto L_8025FBA8;
    if ((s32)r0 >= (s32)0x85) goto L_8025FBAC;
    if ((s32)r0 >= (s32)0x4c) goto L_8025FBAC;
    goto L_8025FB18;
L_8025FA94: ;
    if ((s32)r0 == (s32)0x90) goto L_8025FB40;
    if ((s32)r0 >= (s32)0x90) goto L_8025FB20;
    goto L_8025FBAC;
L_8025FAA4: ;
    if ((s32)r0 == (s32)0x136) goto L_8025FB90;
    if ((s32)r0 >= (s32)0x136) goto L_8025FAE0;
    if ((s32)r0 == (s32)0xfa) goto L_8025FB48;
    if ((s32)r0 >= (s32)0xfa) goto L_8025FAD4;
    if ((s32)r0 == (s32)0xe2) goto L_8025FB70;
    if ((s32)r0 >= (s32)0xe2) goto L_8025FBAC;
    if ((s32)r0 == (s32)0xd9) goto L_8025FB78;
    goto L_8025FBAC;
L_8025FAD4: ;
    if ((s32)r0 == (s32)0x12c) goto L_8025FB28;
    goto L_8025FBAC;
L_8025FAE0: ;
    if ((s32)r0 == (s32)0x16b) goto L_8025FBA0;
    if ((s32)r0 >= (s32)0x16b) goto L_8025FAF8;
    if ((s32)r0 == (s32)0x14b) goto L_8025FB80;
    goto L_8025FBAC;
L_8025FAF8: ;
    if ((s32)r0 == (s32)0x198) goto L_8025FB58;
    if ((s32)r0 >= (s32)0x198) goto L_8025FBAC;
    if ((s32)r0 >= (s32)0x197) goto L_8025FB50;
    goto L_8025FBAC;
L_8025FB10: ;
    f1 = *(f32*)lbl_8047E694;
    goto L_8025FBAC;
L_8025FB18: ;
    f1 = *(f32*)lbl_8047E698;
    goto L_8025FBAC;
L_8025FB20: ;
    f1 = *(f32*)lbl_8047E69C;
    goto L_8025FBAC;
L_8025FB28: ;
    f1 = *(f32*)lbl_8047E6A0;
    goto L_8025FBAC;
L_8025FB30: ;
    f1 = *(f32*)lbl_8047E69C;
    goto L_8025FBAC;
L_8025FB38: ;
    f1 = *(f32*)lbl_8047E6A4;
    goto L_8025FBAC;
L_8025FB40: ;
    f1 = *(f32*)lbl_8047E6A4;
    goto L_8025FBAC;
L_8025FB48: ;
    f1 = *(f32*)lbl_8047E698;
    goto L_8025FBAC;
L_8025FB50: ;
    f1 = *(f32*)lbl_8047E6A4;
    goto L_8025FBAC;
L_8025FB58: ;
    f1 = *(f32*)lbl_8047E6A4;
    goto L_8025FBAC;
L_8025FB60: ;
    f1 = *(f32*)lbl_8047E6A0;
    goto L_8025FBAC;
L_8025FB68: ;
    f1 = *(f32*)lbl_8047E6A0;
    goto L_8025FBAC;
L_8025FB70: ;
    f1 = *(f32*)lbl_8047E69C;
    goto L_8025FBAC;
L_8025FB78: ;
    f1 = *(f32*)lbl_8047E69C;
    goto L_8025FBAC;
L_8025FB80: ;
    f1 = *(f32*)lbl_8047E6A8;
    goto L_8025FBAC;
L_8025FB88: ;
    f1 = *(f32*)lbl_8047E6A8;
    goto L_8025FBAC;
L_8025FB90: ;
    f1 = *(f32*)lbl_8047E69C;
    goto L_8025FBAC;
L_8025FB98: ;
    f1 = *(f32*)lbl_8047E698;
    goto L_8025FBAC;
L_8025FBA0: ;
    f1 = *(f32*)lbl_8047E698;
    goto L_8025FBAC;
L_8025FBA8: ;
    f1 = *(f32*)lbl_8047E698;
L_8025FBAC: ;
    if ((u32)r5 == (u32)0x0) goto L_8025FBBC;
    f0 = *(f32*)lbl_8047E6AC;
    *(f32*)((u8*)r5 + 0x0) = f0;
L_8025FBBC: ;
    if ((u32)r4 == (u32)0x0) return;
    *(f32*)((u8*)r4 + 0x0) = f1;
    return;
}
#pragma pop

/* Address: 0x8025FBCC | Size: 0x168 (360 bytes) */
extern u8* fn_80129280(u32, u32);
extern u32 fn_800E0C04(s32);
void fn_8025FBCC(u32 flag) {
    u16 i;
    u8* data;

    if (flag == 0) {
        fn_80129280(0, 0xc);
    }

    for (i = 1; i <= 0xfbu; i++) {
        u16 count, j;
        data = fn_80129280(0, 0xc);
        count = *(u16*)data;
        for (j = 0; j < count; j++) {
            if ((*(u16*)(data + j * 0xc + 4) & 0x3FFF) == i) {
                goto next1;
            }
        }
        *(u16*)(data + count * 0xc + 4) = (u16)(i | 0x8000);
        {
            u32 val = fn_800E0C04(-1);
            *(u32*)(data + *(u16*)data * 0xc + 0xc) = val;
        }
        *(u16*)data = (u16)(*(u16*)data + 1);
    next1:;
    }

    for (i = 0x115; i <= 0x19bu; i++) {
        u16 count, j;
        data = fn_80129280(0, 0xc);
        count = *(u16*)data;
        for (j = 0; j < count; j++) {
            if ((*(u16*)(data + j * 0xc + 4) & 0x3FFF) == i) {
                goto next2;
            }
        }
        *(u16*)(data + count * 0xc + 4) = (u16)(i | 0x8000);
        {
            u32 val = fn_800E0C04(-1);
            *(u32*)(data + *(u16*)data * 0xc + 0xc) = val;
        }
        *(u16*)data = (u16)(*(u16*)data + 1);
    next2:;
    }
}

/* Address: 0x8025FD34 | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025FD34(void* ctx, u32 param1, u32 param2) {
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r4;
    r31 = r3;
    if ((u32)r3 != (u32)0x0) goto L_8025FD64;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
    r31 = r3;
L_8025FD64: ;
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_8025FD7C;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
L_8025FD7C: ;
    r3 = *(u16*)((u8*)r3 + 0x0);
    r4 = r30 & 0xFFFF;
    r6 = 0x0;
    goto L_8025FDB8;
L_8025FD8C: ;
    r0 = r6 & 0xFFFF;
    r5 = r0 * 0xc;
    r0 = r5 + 0x4;
    r0 = *(u16*)(r31 + r0);
    r0 = r0 & 0x3FFF;
    if ((s32)r0 != (s32)r4) goto L_8025FDB4;
    r3 = r31 + r5;
    r3 = *(u32*)((u8*)r3 + 0x8);
    goto L_8025FDC8;
L_8025FDB4: ;
    r6 = r6 + 0x1;
L_8025FDB8: ;
    r0 = r6 & 0xFFFF;
    if ((u32)r0 < (u32)r3) goto L_8025FD8C;
    r3 = 0x0;
L_8025FDC8: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025FDDC | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025FDDC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r4;
    r31 = r3;
    if ((u32)r3 != (u32)0x0) goto L_8025FE0C;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
    r31 = r3;
L_8025FE0C: ;
    r3 = r31;
    if ((u32)r31 != (u32)0x0) goto L_8025FE24;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
L_8025FE24: ;
    r3 = *(u16*)((u8*)r3 + 0x0);
    r4 = r30 & 0xFFFF;
    r6 = 0x0;
    goto L_8025FE60;
L_8025FE34: ;
    r0 = r6 & 0xFFFF;
    r5 = r0 * 0xc;
    r0 = r5 + 0x4;
    r0 = *(u16*)(r31 + r0);
    r0 = r0 & 0x3FFF;
    if ((s32)r0 != (s32)r4) goto L_8025FE5C;
    r3 = r31 + r5;
    r3 = *(u32*)((u8*)r3 + 0xC);
    goto L_8025FE70;
L_8025FE5C: ;
    r6 = r6 + 0x1;
L_8025FE60: ;
    r0 = r6 & 0xFFFF;
    if ((u32)r0 < (u32)r3) goto L_8025FE34;
    r3 = 0x0;
L_8025FE70: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x8025FE84 | Size: 0x60 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025FE84(void* ctx, u32 slot, u32 param) {
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 != (u32)0x0) goto L_8025FEAC;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
L_8025FEAC: ;
    r0 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8025FECC;
    r0 = r31 & 0xFFFF;
    r0 = r0 * 0xc;
    r3 = r3 + r0;
    r3 = *(u16*)((u8*)r3 + 0x4);
    goto L_8025FED0;
L_8025FECC: ;
    r3 = 0x0;
L_8025FED0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x8025FEE4 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8025FEE4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025FF18 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8025FF18(void* ctx, u32 slot, u32 param) {
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = 0x0;
    if ((u32)r3 != (u32)0x0) goto L_8025FF40;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
L_8025FF40: ;
    r5 = 0x0;
    goto L_8025FF74;
L_8025FF48: ;
    r0 = r5 & 0xFFFF;
    r4 = r0 * 0xc;
    r4 = r4 + 0x4;
    r0 = *(u16*)(r3 + r4);
    r0 = r0 & 0x00008000;
    if ((u32)r3 == (u32)0x0) goto L_8025FF64;
    r31 = 0x1;
L_8025FF64: ;
    r0 = *(u16*)(r3 + r4);
    r5 = r5 + 0x1;
    r0 = r0 & 0x3FFF;
    *(u16*)(r3 + r4) = r0;
L_8025FF74: ;
    r0 = *(u16*)((u8*)r3 + 0x0);
    r4 = r5 & 0xFFFF;
    if ((u32)r4 < (u32)r0) goto L_8025FF48;
    r3 = r31;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x8025FF9C | Size: 0xD4 (212 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8025FF9C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8011F520();
    extern void fn_8011F5B0();
    extern void fn_8011F5C8();
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r4;
    r31 = r3;
    if ((u32)r3 != (u32)0x0) goto L_8025FFCC;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
    r31 = r3;
L_8025FFCC: ;
    r3 = r30;
    fn_8011F5C8();
    r6 = *(u16*)((u8*)r31 + 0x0);
    r4 = r3 & 0xFFFF;
    r7 = 0x0;
    goto L_80260004;
L_8025FFE4: ;
    r0 = r7 & 0xFFFF;
    r5 = r0 * 0xc;
    r0 = r5 + 0x4;
    r0 = *(u16*)(r31 + r0);
    r0 = r0 & 0x3FFF;
    if ((u32)r0 == (u32)r4) goto L_8026005C;
    r7 = r7 + 0x1;
L_80260004: ;
    r0 = r7 & 0xFFFF;
    if ((u32)r0 < (u32)r6) goto L_8025FFE4;
    r0 = r6 * 0xc;
    r5 = r3 | 0x8000;
    r3 = r30;
    r4 = r31 + r0;
    *(u16*)((u8*)r4 + 0x4) = r5;
    fn_8011F5B0();
    r0 = *(u16*)((u8*)r31 + 0x0);
    r0 = r0 * 0xc;
    r4 = r31 + r0;
    *(u32*)((u8*)r4 + 0xC) = r3;
    r3 = r30;
    fn_8011F520();
    r0 = *(u16*)((u8*)r31 + 0x0);
    r0 = r0 * 0xc;
    r4 = r31 + r0;
    *(u32*)((u8*)r4 + 0x8) = r3;
    r3 = *(u16*)((u8*)r31 + 0x0);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r31 + 0x0) = r0;
L_8026005C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop


/* -------------------------------------------------------------------
 * Utility & Cleanup (0x80260000-0x80266360)
 * 70 functions
 * ------------------------------------------------------------------- */

/* Address: 0x80260070 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80260070(void* ctx, u32 slot, u32 param) {
    extern void fn_800E0C04();
    extern void fn_80129280();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f4 = 0.0f;

    /* stmw r27, 0xc(r1) */;
    r29 = r3;
    if ((u32)r3 != (u32)0x0) goto L_8026009C;
    r3 = 0x0;
    r4 = 0xc;
    fn_80129280();
    r29 = r3;
L_8026009C: ;
    r30 = 0x0;
    r27 = 0x0;
    *(u16*)((u8*)r29 + 0x0) = r30;
    r31 = r30;
L_802600AC: ;
    r28 = r29 + r31;
    r3 = -0x1;
    *(u16*)((u8*)r28 + 0x4) = r30;
    fn_800E0C04();
    r27 = r27 + 0x1;
    *(u32*)((u8*)r28 + 0xC) = r3;
    r31 = r31 + 0xc;
    if ((s32)r27 < (s32)0x1f4) goto L_802600AC;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x802600E4 | Size: 0x378 (888 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802600E4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8001E074();
    extern void fn_8001E184();
    extern void fn_801065B8();
    extern void fn_801067E8();
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_8011F228();
    extern void fn_8011F4F0();
    extern void fn_80132A38();
    extern void fn_80165668();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r23, 0xc(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    r23 = r7;
    r24 = r8;
    r27 = 0x0;
L_80260110: ;
    r3 = r28;
    r4 = r27 & 0xFFFF;
    fn_8011F228();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80260130;
    r27 = r27 + 0x1;
    if ((s32)r27 < (s32)0x4) goto L_80260110;
L_80260130: ;
    if ((s32)r27 < (s32)0x4) goto L_802603C4;
    r3 = r28;
    fn_8011F4F0();
    r0 = r3;
    r3 = 0x32;
    r26 = r0;
    r4 = r26;
    fn_80132A38();
    r25 = r29 & 0xFFFF;
    r3 = 0x39;
    r4 = r25;
    fn_80132A38();
L_80260164: ;
    if ((s32)r31 != (s32)0x0) goto L_80260180;
    r3 = 0x4243;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    goto L_80260194;
L_80260180: ;
    r3 = 0x2;
    r4 = 0x4243;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
L_80260194: ;
    if ((s32)r31 != (s32)0x0) goto L_802601A8;
    fn_8001E184();
    r27 = (s8)r3;
    goto L_802601C0;
L_802601A8: ;
    r3 = 0x0;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    fn_8001E074();
    r27 = (s8)r3;
L_802601C0: ;
    if ((s32)r31 != (s32)0x0) goto L_802601D4;
    r3 = 0x1;
    fn_801065B8();
    goto L_802601DC;
L_802601D4: ;
    r3 = 0x1;
    fn_801069FC();
L_802601DC: ;
    if ((s32)r27 == (s32)0x1) goto L_802601FC;
    if ((s32)r27 >= (s32)0x1) goto L_80260204;
    if ((s32)r27 >= (s32)0x0) goto L_802601F4;
    goto L_80260204;
L_802601F4: ;
    r0 = 0x0;
    goto L_80260208;
L_802601FC: ;
    r0 = 0x1;
    goto L_80260208;
L_80260204: ;
    r0 = 0x2;
L_80260208: ;
    if ((s32)r0 != (s32)0x0) goto L_80260244;
    if ((u32)r23 == (u32)0x0) goto L_80260238;
    r12 = r23;
    r3 = r28;
    r4 = r29;
    r5 = r24;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r27 = (s8)r3;
    goto L_8026023C;
L_80260238: ;
    r27 = 0x0;
L_8026023C: ;
    if ((s32)r27 >= (s32)0x0) goto L_8026035C;
L_80260244: ;
    r4 = r26;
    r3 = 0x32;
    fn_80132A38();
    r4 = r25;
    r3 = 0x39;
    fn_80132A38();
    if ((s32)r31 != (s32)0x0) goto L_80260278;
    r3 = 0x4242;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    goto L_8026028C;
L_80260278: ;
    r3 = 0x2;
    r4 = 0x4242;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
L_8026028C: ;
    if ((s32)r31 != (s32)0x0) goto L_802602A0;
    fn_8001E184();
    r27 = (s8)r3;
    goto L_802602B8;
L_802602A0: ;
    r3 = 0x0;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    fn_8001E074();
    r27 = (s8)r3;
L_802602B8: ;
    if ((s32)r31 != (s32)0x0) goto L_802602CC;
    r3 = 0x1;
    fn_801065B8();
    goto L_802602D4;
L_802602CC: ;
    r3 = 0x1;
    fn_801069FC();
L_802602D4: ;
    if ((s32)r27 == (s32)0x1) goto L_802602F4;
    if ((s32)r27 >= (s32)0x1) goto L_802602FC;
    if ((s32)r27 >= (s32)0x0) goto L_802602EC;
    goto L_802602FC;
L_802602EC: ;
    r0 = 0x0;
    goto L_80260300;
L_802602F4: ;
    r0 = 0x1;
    goto L_80260300;
L_802602FC: ;
    r0 = 0x2;
L_80260300: ;
    if ((s32)r0 != (s32)0x0) goto L_80260164;
    if ((s32)r31 != (s32)0x0) goto L_80260324;
    r3 = 0x4241;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    goto L_80260338;
L_80260324: ;
    r3 = 0x2;
    r4 = 0x4241;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
L_80260338: ;
    if ((s32)r31 != (s32)0x0) goto L_8026034C;
    r3 = 0x1;
    fn_801065B8();
    goto L_80260354;
L_8026034C: ;
    r3 = 0x1;
    fn_801069FC();
L_80260354: ;
    r3 = 0x0;
    goto L_80260448;
L_8026035C: ;
    r3 = r28;
    fn_8011F4F0();
    r4 = r3;
    r3 = 0x32;
    fn_80132A38();
    r3 = 0x5d;
    r4 = 0x468;
    fn_80132A38();
    r3 = r28;
    r4 = r27 & 0xFFFF;
    fn_8011F228();
    r4 = r3 & 0xFFFF;
    r3 = 0x39;
    fn_80132A38();
    if ((s32)r31 != (s32)0x0) goto L_802603B0;
    r3 = 0x4248;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    goto L_802603C4;
L_802603B0: ;
    r3 = 0x2;
    r4 = 0x4248;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
L_802603C4: ;
    r3 = 0x4ca;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    r3 = r28;
    fn_8011F4F0();
    r4 = r3;
    r3 = 0x32;
    fn_80132A38();
    r4 = r29 & 0xFFFF;
    r3 = 0x39;
    fn_80132A38();
    if ((s32)r31 != (s32)0x0) goto L_80260410;
    r3 = 0x423d;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    goto L_80260424;
L_80260410: ;
    r3 = 0x2;
    r4 = 0x423d;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
L_80260424: ;
    if ((s32)r31 != (s32)0x0) goto L_80260438;
    r3 = 0x1;
    fn_801065B8();
    goto L_80260440;
L_80260438: ;
    r3 = 0x1;
    fn_801069FC();
L_80260440: ;
    *(u8*)((u8*)r30 + 0x0) = r27;
    r3 = 0x1;
L_80260448: ;
    /* lmw r23, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x8026045C | Size: 0x27C (636 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8026045C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A488[];
    extern u8 lbl_8047E6C0[];
    extern void fn_80097A38();
    extern void fn_800D1FDC();
    extern void fn_800D2584();
    extern void fn_800ECB74();
    extern void fn_8011F5B0();
    extern void fn_801766A8();
    extern void fn_80177830();
    extern void fn_80177858();
    extern void fn_801778B4();
    extern void fn_801778DC();
    extern void fn_80177908();
    extern void fn_80177930();
    extern void fn_8017795C();
    extern void fn_80177984();
    extern void fn_80177A44();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DA4E8();
    extern void fn_801DAC3C();
    extern void fn_801DAC90();
    extern void fn_801DADC0();
    extern void fn_801DDD28();
    extern void fn_801DE190();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    f1 = *(f32*)lbl_8047E6C0;
    /* stmw r24, 0x80(r1) */;
    r26 = r3;
    r28 = r4;
    r27 = r5;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r1 + 0x18;
    fn_801778B4();
    r3 = r1 + 0x24;
    fn_80177830();
    r3 = r1 + 0x30;
    fn_80177908();
    r3 = r1 + 0x3c;
    fn_8017795C();
    fn_800D2584();
    r4 = r1 + 0x8;
    r5 = r1 + 0xc;
    r6 = r1 + 0x10;
    r7 = r1 + 0x14;
    fn_800D1FDC();
    r25 = *(u32*)(sp + 0x18);
    r31 = r1 + 0x54;
    r12 = *(u32*)(sp + 0x1C);
    r30 = r1 + 0x60;
    r11 = *(u32*)(sp + 0x20);
    r29 = r1 + 0x6c;
    r10 = *(u32*)(sp + 0x24);
    r9 = *(u32*)(sp + 0x28);
    r8 = *(u32*)(sp + 0x2C);
    r7 = *(u32*)(sp + 0x30);
    r6 = *(u32*)(sp + 0x34);
    r5 = *(u32*)(sp + 0x38);
    r4 = *(u32*)(sp + 0x3C);
    r3 = *(u32*)(sp + 0x40);
    r0 = *(u32*)(sp + 0x44);
    f0 = *(f32*)(sp + 0x8);
    *(u32*)(sp + 0x74) = r0;
    *(f32*)(sp + 0x78) = f0;
    fn_801DAC90();
    r3 = r26;
    r4 = r28;
    fn_80097A38();
    r28 = r3;
    if ((s32)r28 < (s32)0x4) goto L_80260558;
    r28 = -0x1;
L_80260558: ;
    r3 = 0x2;
    fn_801DADC0();
    r3 = r26;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    if ((s32)r28 != (s32)0x4) goto L_80260588;
    r3 = (0x1 << 16);
    /* subi r24, r3, 0x1 */;
    goto L_802605A8;
L_80260588: ;
    r3 = 0x0;
    r5 = 0x66;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r24 = r3 & 0xFFFF;
    if ((s32)r28 != (s32)0x4) goto L_802605A8;
    r3 = (0x1 << 16);
    /* subi r24, r3, 0x1 */;
L_802605A8: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 != (u32)0xffff) goto L_802605BC;
    r25 = 0x0;
    goto L_80260654;
L_802605BC: ;
    r3 = r26;
    fn_8011F5B0();
    r25 = r3;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xc1;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = -r3;
    r4 = r25;
    r0 = r0 | r3;
    r3 = r24;
    r5 = (u32)r0 >> 31;
    fn_801DE190();
    /* mr. r25, r3 */;
    if ((u32)r0 != (u32)0xffff) goto L_80260604;
    r25 = 0x0;
    goto L_80260654;
L_80260604: ;
    r3 = (u32)lbl_8027A488;
    r26 = 0x0;
    r24 = (u32)lbl_8027A488;
L_80260610: ;
    r0 = *(u32*)((u8*)r24 + 0x4);
    if ((s32)r0 != (s32)0x1) goto L_80260638;
    r4 = *(u16*)((u8*)r24 + 0x0);
    r3 = r25;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_80260648;
L_80260638: ;
    r26 = r26 + 0x1;
    r24 = r24 + 0x8;
    if ((s32)r26 < (s32)0x5) goto L_80260610;
L_80260648: ;
    if ((s32)r26 >= (s32)0x5) goto L_80260654;
    r25 = 0x0;
L_80260654: ;
    if ((u32)r25 == (u32)0x0) goto L_80260660;
    *(u32*)((u8*)r27 + 0x4) = r25;
L_80260660: ;
    r3 = *(u32*)((u8*)r27 + 0x4);
    fn_801DAC3C();
    r4 = 0x1;
    fn_800ECB74();
    r3 = *(u32*)((u8*)r27 + 0x4);
    r4 = 0x1;
    fn_801DA4E8();
    r3 = 0x2;
    fn_80177A44();
    r3 = r1 + 0x48;
    fn_801778DC();
    r3 = r31;
    fn_80177858();
    r3 = r30;
    fn_80177930();
    r3 = r29;
    fn_80177984();
    f1 = *(f32*)(sp + 0x78);
    fn_801766A8();
    f1 = *(f32*)lbl_8047E6C0;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = (s8)r28;
    /* lmw r24, 0x80(r1) */;
    return;
}
#pragma pop

/* Address: 0x802606D8 | Size: 0x238 (568 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802606D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A488[];
    extern u8 lbl_8047E6C0[];
    extern void fn_800D1FDC();
    extern void fn_800D2584();
    extern void fn_800D3088();
    extern void fn_800F0308();
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_801766A8();
    extern void fn_80177830();
    extern void fn_80177858();
    extern void fn_801778B4();
    extern void fn_801778DC();
    extern void fn_80177908();
    extern void fn_80177930();
    extern void fn_8017795C();
    extern void fn_80177984();
    extern void fn_80177A44();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB088();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
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
    f32 f1 = 0.0f;

    /* stmw r22, 0x88(r1) */;
    r23 = r5;
    r24 = r6;
    r26 = 0x0;
    r25 = 0x0;
    if ((s32)r4 < (s32)0x0) goto L_80260708;
    if ((s32)r4 < (s32)0x5) goto L_80260710;
L_80260708: ;
    r3 = 0x0;
    goto L_802608FC;
L_80260710: ;
    r5 = (u32)lbl_8027A488;
    r0 = r4 << 3;
    r5 = (u32)lbl_8027A488;
    r4 = r5 + r0;
    r28 = *(u16*)(r5 + r0);
    r0 = *(u32*)((u8*)r4 + 0x4);
    if ((s32)r0 == (s32)0x0) goto L_80260738;
    r27 = *(u32*)((u8*)r3 + 0x4);
    goto L_8026073C;
L_80260738: ;
    r27 = *(u32*)((u8*)r3 + 0x0);
L_8026073C: ;
    r3 = r27;
    r4 = r28;
    r5 = 0x4;
    fn_801DA9E8();
    r31 = r1 + 0x54;
    r30 = r1 + 0x60;
    r29 = r1 + 0x6c;
    goto L_80260878;
L_8026075C: ;
    fn_801DB088();
    r0 = r24 & 0xa;
    if ((s32)r0 == (s32)0x0) goto L_80260788;
    r0 = r24 & 0x00000002;
    r3 = 0x4;
    if ((s32)r0 == (s32)0x0) goto L_80260778;
    r3 = 0x2;
L_80260778: ;
    f1 = *(f32*)lbl_8047E6C0;
    fn_801C41C8();
    r0 = -0xb;
    r24 = r24 & r0;
L_80260788: ;
    r3 = r27;
    r4 = r28;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x0) goto L_80260880;
    fn_800F0308();
    r3 = r1 + 0x18;
    fn_801778B4();
    r3 = r1 + 0x24;
    fn_80177830();
    r3 = r1 + 0x30;
    fn_80177908();
    r3 = r1 + 0x3c;
    fn_8017795C();
    fn_800D2584();
    r4 = r1 + 0x8;
    r5 = r1 + 0xc;
    r6 = r1 + 0x10;
    r7 = r1 + 0x14;
    fn_800D1FDC();
    r22 = *(u32*)(sp + 0x18);
    r12 = *(u32*)(sp + 0x1C);
    r11 = *(u32*)(sp + 0x20);
    r10 = *(u32*)(sp + 0x24);
    r9 = *(u32*)(sp + 0x28);
    r8 = *(u32*)(sp + 0x2C);
    r7 = *(u32*)(sp + 0x30);
    r6 = *(u32*)(sp + 0x34);
    r5 = *(u32*)(sp + 0x38);
    r4 = *(u32*)(sp + 0x3C);
    r3 = *(u32*)(sp + 0x40);
    r0 = *(u32*)(sp + 0x44);
    f0 = *(f32*)(sp + 0x8);
    *(u32*)(sp + 0x74) = r0;
    *(f32*)(sp + 0x78) = f0;
    fn_800D3088();
    r26 = r26 + r3;
    if ((s32)r23 == (s32)0x0) goto L_80260878;
    r3 = 0x1;
    fn_800F7AF0();
    r22 = r3;
    r3 = 0x1;
    fn_800F7BC4();
    r0 = r3 & r22;
    r0 = r0 & 0x00000200;
    if ((s32)r23 == (s32)0x0) goto L_80260878;
    r25 = 0x1;
L_80260878: ;
    if ((s32)r25 == (s32)0x0) goto L_8026075C;
L_80260880: ;
    r3 = 0x2;
    fn_80177A44();
    r3 = r1 + 0x48;
    fn_801778DC();
    r3 = r31;
    fn_80177858();
    r3 = r30;
    fn_80177930();
    r3 = r29;
    fn_80177984();
    f1 = *(f32*)(sp + 0x78);
    fn_801766A8();
    r0 = r24 & 0x14;
    if ((s32)r25 == (s32)0x0) goto L_802608D8;
    r0 = r24 & 0x00000004;
    r3 = 0x5;
    if ((s32)r25 == (s32)0x0) goto L_802608C8;
    r3 = 0x3;
L_802608C8: ;
    f1 = *(f32*)lbl_8047E6C0;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
L_802608D8: ;
    r3 = r27;
    r4 = r28;
    r5 = 0x4;
    fn_801DA8C4();
    if ((s32)r25 == (s32)0x0) goto L_802608F8;
    r3 = r26;
    goto L_802608FC;
L_802608F8: ;
    r3 = -0x1;
L_802608FC: ;
    /* lmw r22, 0x88(r1) */;
    return;
}
#pragma pop

/* Address: 0x80260910 | Size: 0x5AC (1452 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80260910(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A488[];
    extern u8 lbl_8027A4B0[];
    extern u8 lbl_8047E6B0[];
    extern u8 lbl_8047E6B4[];
    extern u8 lbl_8047E6B8[];
    extern u8 lbl_8047E6C0[];
    extern u8 lbl_8047E6C4[];
    extern void fn_800D3088();
    extern void fn_800F0308();
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_801065B8();
    extern void fn_801067E8();
    extern void fn_8011E15C();
    extern void fn_8011E778();
    extern void fn_8011F4F0();
    extern void fn_8011F5C8();
    extern void fn_80123FBC();
    extern void fn_80132A38();
    extern void fn_801666BC();
    extern void fn_80166A28();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB088();
    extern void fn_802606D8();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    f32 f1 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x70) = f31;
    /* psq_st f31, 0x78(r1), 0, qr0 */;
    *(f64*)(sp + 0x60) = f30;
    /* psq_st f30, 0x68(r1), 0, qr0 */;
    *(f64*)(sp + 0x50) = f29;
    /* psq_st f29, 0x58(r1), 0, qr0 */;
    /* stmw r18, 0x18(r1) */;
    r27 = r3;
    r28 = r4;
    r3 = *(u32*)((u8*)r3 + 0x0);
    r31 = r5;
    r30 = r6;
    r4 = 0x1;
    fn_801DA4E8();
    r3 = r31;
    fn_8011F4F0();
    r0 = r3;
    r3 = 0x32;
    r4 = r0;
    fn_80132A38();
    r3 = 0x4401;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    if ((u32)r31 != (u32)0x0) goto L_8026098C;
    r3 = 0x0;
    goto L_802609B0;
L_8026098C: ;
    r3 = r31;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r31 != (u32)0x0) goto L_802609A4;
    r3 = 0x0;
    goto L_802609B0;
L_802609A4: ;
    r3 = r31;
    fn_8011F5C8();
    fn_8011E778();
L_802609B0: ;
    if ((u32)r3 != (u32)0x0) goto L_802609C0;
    r25 = 0x0;
    goto L_802609D0;
L_802609C0: ;
    fn_8011E15C();
    r25 = r3 & 0xFFFF;
    r3 = r25;
    fn_80166A28();
L_802609D0: ;
    r3 = (u32)lbl_8027A488;
    r3 = (u32)lbl_8027A488;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3 + 0x4;
    r4 = *(u16*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x0) goto L_802609F4;
    r3 = *(u32*)((u8*)r27 + 0x4);
    goto L_802609F8;
L_802609F4: ;
    r3 = *(u32*)((u8*)r27 + 0x0);
L_802609F8: ;
    r5 = 0x4;
    fn_801DA9E8();
    fn_801DB088();
    r0 = 0x0;
    r3 = (u32)lbl_8027A488;
    *(u32*)((u8*)r27 + 0x8) = r0;
    r26 = (u32)lbl_8027A488;
    r22 = 0x1;
    r20 = 0x0;
    r19 = 0x0;
    r21 = 0x0;
    goto L_80260B58;
L_80260A28: ;
    if ((s32)r28 == (s32)0x0) goto L_80260A68;
    if ((u32)r21 < (u32)0x78) goto L_80260A68;
    if ((s32)r19 < (s32)0x2) goto L_80260A68;
    r3 = 0x1;
    fn_800F7AF0();
    r24 = r3;
    r3 = 0x1;
    fn_800F7BC4();
    r0 = r3 & r24;
    r0 = r0 & 0x00000200;
    if ((s32)r19 == (s32)0x2) goto L_80260A68;
    r20 = 0x1;
    goto L_80260B60;
L_80260A68: ;
    r4 = *(u32*)((u8*)r27 + 0x8);
    r0 = r4 << 3;
    r3 = r26 + r0;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x0) goto L_80260A88;
    r24 = *(u32*)((u8*)r27 + 0x4);
    goto L_80260A8C;
L_80260A88: ;
    r24 = *(u32*)((u8*)r27 + 0x0);
L_80260A8C: ;
    r0 = r4 << 3;
    r18 = *(u16*)(r26 + r0);
    fn_801DB088();
    r3 = r24;
    r4 = r18;
    r5 = 0x4;
    fn_801DA94C();
    r3 = r3 & 0xFF;
    r0 = -r3;
    r0 = r0 | r3;
    /* srwi. r0, r0, 31 */;
    if ((s32)r0 == (s32)0x0) goto L_80260B60;
    if ((s32)r19 == (s32)0x1) goto L_80260B00;
    if ((s32)r19 >= (s32)0x1) goto L_80260B34;
    if ((s32)r19 >= (s32)0x0) goto L_80260AD4;
    goto L_80260B34;
L_80260AD4: ;
    r3 = r25;
    fn_801666BC();
    if ((s32)r3 == (s32)0x2) goto L_80260B34;
    r3 = 0x3d3;
    r4 = 0x0;
    r5 = 0xff;
    ((void(*)(void))fn_80165A20)();
    r29 = 0x0;
    r19 = 0x1;
    goto L_80260B34;
L_80260B00: ;
    r3 = 0x3d3;
    fn_801666BC();
    if ((s32)r3 == (s32)0x2) goto L_80260B34;
    fn_800D3088();
    r29 = r29 + r3;
    if ((s32)r29 < (s32)0x1e) goto L_80260B34;
    r3 = 0x3d4;
    r4 = 0x0;
    r5 = 0xff;
    ((void(*)(void))fn_80165A20)();
    r19 = 0x2;
L_80260B34: ;
    if ((s32)r22 == (s32)0x0) goto L_80260B4C;
    f1 = *(f32*)lbl_8047E6C0;
    r3 = 0x2;
    fn_801C41C8();
    r22 = 0x0;
L_80260B4C: ;
    fn_800F0308();
    fn_800D3088();
    r21 = r21 + r3;
L_80260B58: ;
    if ((u32)r21 < (u32)0x23a) goto L_80260A28;
L_80260B60: ;
    f1 = *(f32*)lbl_8047E6C0;
    r3 = 0x5;
    fn_801C41C8();
    r3 = (u32)lbl_8027A488;
    r19 = (u32)lbl_8027A488;
    goto L_80260BBC;
L_80260B78: ;
    r4 = *(u32*)((u8*)r27 + 0x8);
    r0 = r4 << 3;
    r3 = r19 + r0;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x0) goto L_80260B98;
    r22 = *(u32*)((u8*)r27 + 0x4);
    goto L_80260B9C;
L_80260B98: ;
    r22 = *(u32*)((u8*)r27 + 0x0);
L_80260B9C: ;
    r0 = r4 << 3;
    r18 = *(u16*)(r19 + r0);
    fn_801DB088();
    r3 = r22;
    r4 = r18;
    r5 = 0x4;
    fn_801DA94C();
    fn_800F0308();
L_80260BBC: ;
    r3 = 0x0;
    fn_801C40F0();
    r0 = (s8)r3;
    if ((s32)r0 != (s32)0x0) goto L_80260B78;
    r5 = *(u32*)((u8*)r27 + 0x8);
    r0 = r5 << 3;
    r0 = *(u32*)(r23 + r0);
    if ((s32)r0 == (s32)0x0) goto L_80260BE8;
    r3 = *(u32*)((u8*)r27 + 0x4);
    goto L_80260BEC;
L_80260BE8: ;
    r3 = *(u32*)((u8*)r27 + 0x0);
L_80260BEC: ;
    r4 = (u32)lbl_8027A488;
    r0 = r5 << 3;
    r4 = (u32)lbl_8027A488;
    r5 = 0x4;
    r4 = *(u16*)(r4 + r0);
    fn_801DA8C4();
    f29 = *(f32*)lbl_8047E6B0;
    r19 = (0x4330 << 16);
    f31 = *(f64*)lbl_8047E6B8;
    f30 = *(f32*)lbl_8047E6B4;
    goto L_80260C34;
L_80260C18: ;
    fn_800F0308();
    fn_800D3088();
    f0 = *(f64*)(sp + 0x8);
    f0 = f0 - f31;
    f29 = f29 + f0;
L_80260C34: ;
    if (f29 < f30) goto L_80260C18;
    if ((s32)r20 == (s32)0x0) goto L_80260D58;
    r3 = 0x1;
    fn_801065B8();
    r3 = 0x3d4;
    r4 = 0x32;
    ((void(*)(void))fn_801657F8)();
    r3 = (u32)lbl_8027A4B0;
    r0 = *(u32*)lbl_8027A4B0;
    r4 = 0x0;
    if ((u32)r0 >= (u32)r21) goto L_80260C80;
    r0 = *(u32*)((u8*)r3 + 0x8);
    r4 = 0x1;
    if ((u32)r0 >= (u32)r21) goto L_80260C80;
    r4 = 0x2;
L_80260C80: ;
    r3 = (u32)lbl_8027A4B0;
    r0 = r4 << 3;
    r4 = (u32)lbl_8027A4B0;
    r3 = r27;
    r4 = r4 + r0;
    r5 = 0x0;
    r4 = *(u32*)((u8*)r4 + 0x4);
    r6 = 0x8;
    fn_802606D8();
    if ((u32)r31 != (u32)0x0) goto L_80260CB4;
    r3 = 0x0;
    goto L_80260CD8;
L_80260CB4: ;
    r3 = r31;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r31 != (u32)0x0) goto L_80260CCC;
    r3 = 0x0;
    goto L_80260CD8;
L_80260CCC: ;
    r3 = r31;
    fn_8011F5C8();
    fn_8011E778();
L_80260CD8: ;
    if ((u32)r3 != (u32)0x0) goto L_80260CE8;
    r18 = 0x0;
    goto L_80260D00;
L_80260CE8: ;
    fn_8011E15C();
    r18 = r3 & 0xFFFF;
    r3 = r18;
    fn_80166A28();
    goto L_80260D00;
L_80260CFC: ;
    fn_800F0308();
L_80260D00: ;
    r3 = r18;
    fn_801666BC();
    if ((s32)r3 == (s32)0x2) goto L_80260CFC;
    r3 = r31;
    fn_8011F4F0();
    r4 = r3;
    r3 = 0x32;
    fn_80132A38();
    r3 = 0x43ff;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    r3 = 0x1;
    fn_801065B8();
    f1 = *(f32*)lbl_8047E6C0;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x0;
    goto L_80260E90;
L_80260D58: ;
    r3 = *(u32*)((u8*)r27 + 0x0);
    r4 = 0x0;
    fn_801DA4E8();
    r3 = *(u32*)((u8*)r27 + 0x4);
    r4 = 0x1;
    fn_801DA4E8();
    r3 = r27;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x8;
    fn_802606D8();
    r3 = 0x1;
    fn_801065B8();
    r3 = 0x3d4;
    r4 = 0x32;
    ((void(*)(void))fn_801657F8)();
    if ((u32)r30 != (u32)0x0) goto L_80260DA8;
    r3 = 0x0;
    goto L_80260DCC;
L_80260DA8: ;
    r3 = r30;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r30 != (u32)0x0) goto L_80260DC0;
    r3 = 0x0;
    goto L_80260DCC;
L_80260DC0: ;
    r3 = r30;
    fn_8011F5C8();
    fn_8011E778();
L_80260DCC: ;
    if ((u32)r3 != (u32)0x0) goto L_80260DDC;
    r18 = 0x0;
    goto L_80260DF4;
L_80260DDC: ;
    fn_8011E15C();
    r18 = r3 & 0xFFFF;
    r3 = r18;
    fn_80166A28();
    goto L_80260DF4;
L_80260DF0: ;
    fn_800F0308();
L_80260DF4: ;
    r3 = r18;
    fn_801666BC();
    if ((s32)r3 == (s32)0x2) goto L_80260DF0;
    r3 = r31;
    fn_8011F4F0();
    r4 = r3;
    r3 = 0x32;
    fn_80132A38();
    r3 = r30;
    fn_8011F5C8();
    r0 = r3;
    r3 = 0x4e;
    r4 = r0 & 0xFFFF;
    fn_80132A38();
    r3 = 0x5d;
    r4 = 0x3d2;
    fn_80132A38();
    r3 = 0x4400;
    r4 = 0x1;
    r5 = 0x0;
    fn_801067E8();
    r3 = 0x1;
    fn_801065B8();
    f29 = *(f32*)lbl_8047E6B0;
    r19 = (0x4330 << 16);
    f30 = *(f64*)lbl_8047E6B8;
    f31 = *(f32*)lbl_8047E6C4;
    goto L_80260E84;
L_80260E68: ;
    fn_800F0308();
    fn_800D3088();
    f0 = *(f64*)(sp + 0x8);
    f0 = f0 - f30;
    f29 = f29 + f0;
L_80260E84: ;
    if (f29 < f31) goto L_80260E68;
    r3 = 0x1;
L_80260E90: ;
    /* psq_l f31, 0x78(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x70);
    /* psq_l f30, 0x68(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x60);
    /* psq_l f29, 0x58(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x50);
    /* lmw r18, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80260EBC | Size: 0x414 (1044 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80260EBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8027A488[];
    extern u8 lbl_8047E6C0[];
    extern void fn_8011F5B0();
    extern void fn_8011F5FC();
    extern void fn_80123D58();
    extern void fn_801653BC();
    extern void fn_801653C4();
    extern void fn_801656D8();
    extern void fn_801657D0();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DAC90();
    extern void fn_801DADC0();
    extern void fn_801DDD28();
    extern void fn_801DE190();
    extern void fn_802600E4();
    extern void fn_80260910();
    extern void fn_8026045C();
    u8 sp[0x190];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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

    /* stmw r22, 0x168(r1) */;
    r29 = r3;
    r30 = r4;
    r26 = r5;
    r28 = r6;
    r31 = r7;
    r27 = r8;
    r3 = 0x2;
    fn_801DADC0();
    r0 = 0x0;
    r3 = r29;
    *(u32*)(sp + 0xC) = r0;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80260F20;
    r3 = (0x1 << 16);
    /* subi r23, r3, 0x1 */;
    goto L_80260F40;
L_80260F20: ;
    r3 = 0x0;
    r5 = 0x66;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r23 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80260F40;
    r3 = (0x1 << 16);
    /* subi r23, r3, 0x1 */;
L_80260F40: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)0xffff) goto L_80260F54;
    r22 = 0x0;
    goto L_80260FEC;
L_80260F54: ;
    r3 = r29;
    fn_8011F5B0();
    r22 = r3;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xc1;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = -r3;
    r4 = r22;
    r0 = r0 | r3;
    r3 = r23;
    r5 = (u32)r0 >> 31;
    fn_801DE190();
    /* mr. r22, r3 */;
    if ((u32)r0 != (u32)0xffff) goto L_80260F9C;
    r22 = 0x0;
    goto L_80260FEC;
L_80260F9C: ;
    r3 = (u32)lbl_8027A488;
    r23 = 0x0;
    r24 = (u32)lbl_8027A488;
L_80260FA8: ;
    r0 = *(u32*)((u8*)r24 + 0x4);
    if ((s32)r0 != (s32)0x0) goto L_80260FD0;
    r4 = *(u16*)((u8*)r24 + 0x0);
    r3 = r22;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x0) goto L_80260FE0;
L_80260FD0: ;
    r23 = r23 + 0x1;
    r24 = r24 + 0x8;
    if ((s32)r23 < (s32)0x5) goto L_80260FA8;
L_80260FE0: ;
    if ((s32)r23 >= (s32)0x5) goto L_80260FEC;
    r22 = 0x0;
L_80260FEC: ;
    if ((u32)r22 != (u32)0x0) goto L_80260FFC;
    r0 = 0x0;
    goto L_80261004;
L_80260FFC: ;
    r0 = 0x1;
L_80261004: ;
    r0 = r0 & 0xFF;
    if ((u32)r22 == (u32)0x0) goto L_80261128;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    if ((u32)r22 != (u32)0x0) goto L_80261034;
    r3 = (0x1 << 16);
    /* subi r23, r3, 0x1 */;
    goto L_80261054;
L_80261034: ;
    r3 = 0x0;
    r5 = 0x66;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r23 = r3 & 0xFFFF;
    if ((u32)r22 != (u32)0x0) goto L_80261054;
    r3 = (0x1 << 16);
    /* subi r23, r3, 0x1 */;
L_80261054: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)0xffff) goto L_80261068;
    r22 = 0x0;
    goto L_80261100;
L_80261068: ;
    r3 = r30;
    fn_8011F5B0();
    r22 = r3;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xc1;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = -r3;
    r4 = r22;
    r0 = r0 | r3;
    r3 = r23;
    r5 = (u32)r0 >> 31;
    fn_801DE190();
    /* mr. r22, r3 */;
    if ((u32)r0 != (u32)0xffff) goto L_802610B0;
    r22 = 0x0;
    goto L_80261100;
L_802610B0: ;
    r3 = (u32)lbl_8027A488;
    r23 = 0x0;
    r24 = (u32)lbl_8027A488;
L_802610BC: ;
    r0 = *(u32*)((u8*)r24 + 0x4);
    if ((s32)r0 != (s32)0x1) goto L_802610E4;
    r4 = *(u16*)((u8*)r24 + 0x0);
    r3 = r22;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_802610F4;
L_802610E4: ;
    r23 = r23 + 0x1;
    r24 = r24 + 0x8;
    if ((s32)r23 < (s32)0x5) goto L_802610BC;
L_802610F4: ;
    if ((s32)r23 >= (s32)0x5) goto L_80261100;
    r22 = 0x0;
L_80261100: ;
    if ((u32)r22 != (u32)0x0) goto L_80261110;
    r0 = 0x0;
    goto L_80261118;
L_80261110: ;
    r0 = 0x1;
L_80261118: ;
    r0 = r0 & 0xFF;
    if ((u32)r22 == (u32)0x0) goto L_80261128;
    r0 = 0x1;
    goto L_80261130;
L_80261128: ;
    fn_801DAC90();
    r0 = 0x0;
L_80261130: ;
    if ((s32)r0 != (s32)0x0) goto L_80261140;
    r3 = 0x2;
    goto L_802612BC;
L_80261140: ;
    r25 = r27;
    r27 = r28;
    fn_801653C4();
    /* mr. r24, r3 */;
    if ((s32)r0 == (s32)0x0) goto L_80261170;
    fn_801656D8();
    r22 = r3;
    r3 = 0x1;
    r4 = 0x32;
    r5 = 0xff;
    ((void(*)(void))fn_80165A20)();
    goto L_80261174;
L_80261170: ;
    r22 = 0x0;
L_80261174: ;
    fn_801653BC();
    /* mr. r28, r3 */;
    if ((s32)r0 == (s32)0x0) goto L_80261194;
    fn_801656D8();
    r23 = r3;
    r3 = 0x32;
    fn_801657D0();
    goto L_80261198;
L_80261194: ;
    r23 = 0x0;
L_80261198: ;
    r4 = r26;
    r5 = r29;
    r6 = r30;
    r3 = r1 + 0xc;
    fn_80260910();
    r4 = *(u32*)(sp + 0x18);
    r26 = r3;
    if ((u32)r4 == (u32)0x0) goto L_802611E0;
    r0 = *(u32*)(sp + 0x20);
    r3 = r4;
    r4 = 0x32;
    r5 = r0 & 0xFF;
    ((void(*)(void))fn_80165A20)();
L_802611E0: ;
    r3 = *(u32*)(sp + 0x1C);
    if ((u32)r3 == (u32)0x0) goto L_802611FC;
    r0 = *(u32*)(sp + 0x24);
    r4 = 0x32;
    r5 = r0 & 0xFF;
    ((void(*)(void))fn_801659FC)();
L_802611FC: ;
    if ((s32)r26 != (s32)0x0) goto L_8026120C;
    r24 = 0x0;
    goto L_802612A4;
L_8026120C: ;
    r4 = r30;
    r3 = r1 + 0x28;
    fn_8011F5FC();
    r3 = (u32)fn_8026045C;
    r22 = r1 + 0xc;
    r28 = (u32)fn_8026045C;
    r29 = 0x0;
    r26 = 0xff;
    goto L_80261284;
L_80261230: ;
    r24 = *(u16*)((u8*)r27 + 0x0);
    r7 = r28;
    r8 = r22;
    r3 = r1 + 0x28;
    r4 = r24;
    r5 = r1 + 0x8;
    r6 = 0x0;
    fn_802600E4();
    if ((s32)r3 == (s32)0x0) goto L_8026126C;
    r4 = *(u8*)(sp + 0x8);
    r5 = r24;
    r3 = r1 + 0x28;
    fn_80123D58();
    goto L_80261270;
L_8026126C: ;
    *(u8*)(sp + 0x8) = r26;
L_80261270: ;
    r0 = *(u8*)(sp + 0x8);
    r29 = r29 + 0x1;
    r27 = r27 + 0x2;
    *(u8*)((u8*)r25 + 0x0) = r0;
    r25 = r25 + 0x1;
L_80261284: ;
    if ((s32)r29 < (s32)r31) goto L_80261230;
    f1 = *(f32*)lbl_8047E6C0;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r24 = 0x1;
L_802612A4: ;
    fn_801DAC90();
    if ((s32)r24 == (s32)0x0) goto L_802612B8;
    r3 = 0x0;
    goto L_802612BC;
L_802612B8: ;
    r3 = 0x1;
L_802612BC: ;
    /* lmw r22, 0x168(r1) */;
    return;
}
#pragma pop

/* Address: 0x802612D0 | Size: 0x5C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802612D0(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_804787E0[];
    extern void fn_800FF660();
    extern void fn_8011288C();
    extern void fn_80260EBC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r3 = (u32)lbl_804787E0;
    r8 = (u32)lbl_804787E0;
    r3 = *(u32*)((u8*)r8 + 0x0);
    r4 = *(u32*)((u8*)r8 + 0x4);
    r5 = *(u32*)((u8*)r8 + 0x8);
    r6 = *(u32*)((u8*)r8 + 0x10);
    r7 = *(u32*)((u8*)r8 + 0xC);
    r8 = *(u32*)((u8*)r8 + 0x14);
    fn_80260EBC();
    r4 = (u32)lbl_804787E0;
    r4 = (u32)lbl_804787E0;
    *(u32*)((u8*)r4 + 0x18) = r3;
    fn_800FF660();
    r3 = 0x0;
    r4 = 0x0;
    fn_8011288C();
    return;
}
#pragma pop

/* Address: 0x8026132C | Size: 0x5C | Pattern: field_accessor */
u32 fn_8026132C(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_804787E0[];
    extern void fn_800F0308();
    extern void fn_800FF730();
    extern void fn_8011288C();
    u8* base = lbl_804787E0;
    *(u32*)(base + 0x0) = (u32)ctx;
    *(u32*)(base + 0x4) = slot;
    *(u32*)(base + 0x8) = param;
    fn_800FF730(0x386);
    fn_8011288C(0, 0);
    fn_800F0308();
    return *(u32*)(base + 0x18);
}

/* Address: 0x80261388 | Size: 0x4C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80261388(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478F80[];
    extern u8 lbl_80478F84[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r4 = *(u32*)lbl_80478F80;
    r7 = 0x0;
    r5 = *(u32*)lbl_80478F84;
    r6 = *(u32*)((u8*)r4 + 0x0);
    goto L_802613C0;
L_8026139C: ;
    r0 = r7 & 0xFFFF;
    r4 = r0 << 3;
    r0 = r4 + 0x4;
    r0 = *(u32*)(r5 + r0);
    if ((u32)r3 != (u32)r0) goto L_802613BC;
    r3 = r7;
    return;
L_802613BC: ;
    r7 = r7 + 0x1;
L_802613C0: ;
    r0 = r7 & 0xFFFF;
    if ((u32)r0 < (u32)r6) goto L_8026139C;
    r3 = 0x0;
    return;
}
#pragma pop

/* Address: 0x802613D4 | Size: 0x70 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802613D4(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8027A4C8[];
    extern u8 lbl_8039A6C8[];
    extern u8 lbl_80478F80[];
    extern u8 lbl_80478F84[];
    extern void fn_800DD970();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = *(u32*)lbl_80478F80;
    r5 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r5 < (u32)r0) goto L_80261414;
    r3 = (u32)lbl_8027A4C8;
    r4 = (u32)lbl_8039A6C8;
    r3 = (u32)lbl_8027A4C8;
    r4 = (u32)lbl_8039A6C8;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto L_80261420;
L_80261414: ;
    r3 = *(u32*)lbl_80478F84;
    r0 = r5 << 3;
    r3 = r3 + r0;
L_80261420: ;
    if ((u32)r3 != (u32)0x0) goto L_80261430;
    r3 = 0x0;
    goto L_80261434;
L_80261430: ;
    r3 = *(u32*)((u8*)r3 + 0x4);
L_80261434: ;
    return;
}
#pragma pop

/* Address: 0x80261444 | Size: 0x70 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80261444(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8027A4C8[];
    extern u8 lbl_8039A6C8[];
    extern u8 lbl_80478F80[];
    extern u8 lbl_80478F84[];
    extern void fn_800DD970();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = *(u32*)lbl_80478F80;
    r5 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r5 < (u32)r0) goto L_80261484;
    r3 = (u32)lbl_8027A4C8;
    r4 = (u32)lbl_8039A6C8;
    r3 = (u32)lbl_8027A4C8;
    r4 = (u32)lbl_8039A6C8;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto L_80261490;
L_80261484: ;
    r3 = *(u32*)lbl_80478F84;
    r0 = r5 << 3;
    r3 = r3 + r0;
L_80261490: ;
    if ((u32)r3 != (u32)0x0) goto L_802614A0;
    r3 = 0x0;
    goto L_802614A4;
L_802614A0: ;
    r3 = *(u16*)((u8*)r3 + 0x0);
L_802614A4: ;
    return;
}
#pragma pop

/* Address: 0x802614B4 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802614B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F2B5C();
    extern void fn_801F37B0();
    extern void fn_80261708();
    extern void fn_8026184C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r3 = (u32)fn_8026184C;
    r6 = 0x0;
    r4 = (u32)fn_8026184C;
    r0 = 0x0;
    r5 = r1 + 0x8;
    r3 = 0x0;
    *(u8*)(sp + 0x8) = r0;
    *(u8*)(sp + 0x9) = r0;
    *(u16*)(sp + 0xA) = r0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F2B5C();
    r31 = *(u16*)(sp + 0xA);
    r0 = 0x0;
    r3 = (u32)fn_80261708;
    r5 = r1 + 0x8;
    *(u16*)(sp + 0xA) = r0;
    r4 = (u32)fn_80261708;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = *(u16*)(sp + 0xA);
    r3 = r31 * 0x78;
    r0 = r0 * 0x7c;
    r3 = r3 + r0;
    r3 = r3 + 0x48;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x8026153C | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8026153C(void* ctx, u32 param1, u32 param2) {
    extern void fn_801C3108();
    extern void fn_801C3114();
    extern void fn_801DAC90();
    extern void fn_801F2B5C();
    extern void fn_801F37B0();
    extern void fn_80261708();
    extern void fn_8026184C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x18(r1) */;
    r30 = r3;
    r31 = r30 + 0x4;
    fn_801C3108();
    r4 = r3;
    r3 = r31;
    r5 = 0x44;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r6 = 0x1;
    r0 = r31 + 0x44;
    r7 = 0x0;
    r3 = (u32)fn_8026184C;
    r4 = (u32)fn_8026184C;
    *(u8*)(sp + 0x8) = r6;
    r5 = r1 + 0x8;
    r3 = 0x0;
    *(u8*)(sp + 0x9) = r6;
    r6 = 0x0;
    *(u16*)(sp + 0xA) = r7;
    *(u32*)(sp + 0x10) = r0;
    fn_801F2B5C();
    r31 = *(u16*)(sp + 0xA);
    r0 = 0x0;
    r3 = (u32)fn_80261708;
    r5 = r1 + 0x8;
    *(u16*)(sp + 0xA) = r0;
    r4 = (u32)fn_80261708;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = *(u16*)(sp + 0xA);
    r3 = 0x1;
    *(u16*)((u8*)r30 + 0x0) = r31;
    *(u16*)((u8*)r30 + 0x2) = r0;
    ((void(*)(void))fn_801EF8F4)();
    fn_801C3114();
    fn_801DAC90();
    /* lmw r30, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x802615F4 | Size: 0x114 (276 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802615F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_800E3D08();
    extern void fn_800E9B2C();
    extern void fn_801C3108();
    extern void fn_801C3430();
    extern void fn_801DA224();
    extern void fn_801DA4E8();
    extern void fn_801DAC3C();
    extern void fn_801DAEF8();
    extern void fn_801DE190();
    extern void fn_801DE418();
    extern void fn_801F198C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r28 = r3;
    r3 = 0xa;
    r27 = r28 + 0x4;
    fn_801DAEF8();
    r29 = *(u16*)((u8*)r28 + 0x0);
    r30 = r27 + 0x44;
    r28 = *(u16*)((u8*)r28 + 0x2);
    goto L_8026165C;
L_80261624: ;
    r3 = *(u16*)((u8*)r30 + 0x4);
    fn_801DE418();
    r4 = *(u32*)((u8*)r30 + 0x0);
    r31 = r3;
    *(u32*)((u8*)r4 + 0x27C0) = r31;
    r4 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r4 + 0x0) = r31;
    fn_801DAC3C();
    r4 = r30 + 0xc;
    fn_800E9B2C();
    r4 = *(u8*)((u8*)r30 + 0x6);
    r3 = r31;
    fn_801DA224();
    r30 = r30 + 0x78;
L_8026165C: ;
    r0 = r29 & 0xFFFF;
    /* subi r29, r29, 0x1 */;
    if ((s32)r0 != (s32)0) goto L_80261624;
    r29 = r30;
    goto L_802616C8;
L_80261670: ;
    r3 = *(u16*)((u8*)r29 + 0x4);
    r4 = *(u32*)((u8*)r29 + 0x8);
    r5 = *(u8*)((u8*)r29 + 0x7);
    fn_801DE190();
    r4 = *(u32*)((u8*)r29 + 0x0);
    r31 = r3;
    *(u32*)((u8*)r4 + 0x600) = r31;
    r4 = *(u32*)((u8*)r29 + 0xC);
    *(u32*)((u8*)r4 + 0x0) = r31;
    fn_801DAC3C();
    r4 = r29 + 0x10;
    r30 = r3;
    fn_800E9B2C();
    r4 = *(u8*)((u8*)r29 + 0x6);
    r3 = r31;
    fn_801DA224();
    r3 = r30;
    fn_800E3D08();
    r4 = r3;
    r3 = r31;
    fn_801DA4E8();
    r29 = r29 + 0x7c;
L_802616C8: ;
    r0 = r28 & 0xFFFF;
    /* subi r28, r28, 0x1 */;
    if ((s32)r0 != (s32)0) goto L_80261670;
    fn_801C3108();
    r4 = r27;
    r5 = 0x44;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = 0x1;
    ((void(*)(void))fn_801EF8F4)();
    fn_801C3430();
    fn_801F198C();
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80261708 | Size: 0x144 (324 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261708(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_800E9C6C();
    extern void fn_801D9E1C();
    extern void fn_801DA354();
    extern void fn_801DAC3C();
    extern void fn_801DAC78();
    extern void fn_801DB100();
    extern void fn_801DE164();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r31 = r5;
    r28 = r3;
    r5 = 0xee;
    ((void(*)(void))fn_8012640C)();
    /* mr. r29, r3 */;
    if ((s32)r0 != (s32)0) goto L_80261740;
    r3 = 0x1;
    goto L_80261838;
L_80261740: ;
    fn_801DAC78();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80261754;
    r3 = 0x1;
    goto L_80261838;
L_80261754: ;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_80261814;
    r30 = *(u32*)((u8*)r31 + 0x8);
    *(u32*)((u8*)r30 + 0x0) = r28;
    *(u16*)((u8*)r30 + 0x4) = r3;
    r3 = r29;
    fn_801DA354();
    *(u8*)((u8*)r30 + 0x6) = r3;
    r3 = r29;
    fn_801D9E1C();
    *(u8*)((u8*)r30 + 0x7) = r3;
    r3 = r29;
    fn_801DE164();
    *(u32*)((u8*)r30 + 0x8) = r3;
    r9 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r29 != (u32)0x0) goto L_802617A4;
    r0 = 0x0;
    goto L_802617F4;
L_802617A4: ;
    r7 = 0x0;
    r3 = 0x0;
    r6 = r7;
    r0 = 0x2;
L_802617B4: ;
    r4 = r6;
    r8 = r9 + r3;
    ctr_fn = (void(*)(void))r0;
L_802617C0: ;
    r10 = r8 + r4;
    r5 = *(u32*)((u8*)r10 + 0x4);
    if ((u32)r5 != (u32)r29) goto L_802617D8;
    r0 = r10 + 0x4;
    goto L_802617F4;
L_802617D8: ;
    r4 = r4 + 0x4;
    if (--ctr != 0) goto L_802617C0;
    r7 = r7 + 0x1;
    r3 = r3 + 0x10;
    if ((s32)r7 < (s32)0x4) goto L_802617B4;
    r0 = 0x0;
L_802617F4: ;
    *(u32*)((u8*)r30 + 0xC) = r0;
    r3 = r29;
    fn_801DAC3C();
    r4 = r30 + 0x10;
    fn_800E9C6C();
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = r3 + 0x7c;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80261814: ;
    r0 = *(u8*)((u8*)r31 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_80261828;
    r3 = r29;
    fn_801DB100();
L_80261828: ;
    r4 = *(u16*)((u8*)r31 + 0x2);
    r3 = 0x1;
    r0 = r4 + 0x1;
    *(u16*)((u8*)r31 + 0x2) = r0;
L_80261838: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x8026184C | Size: 0x108 (264 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8026184C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_800E9C6C();
    extern void fn_801DA354();
    extern void fn_801DAC3C();
    extern void fn_801DAC78();
    extern void fn_801DB100();
    extern void fn_801FB1C0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r29 = r5;
    r28 = r3;
    r5 = 0x4c;
    fn_801FB1C0();
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_80261884;
    r3 = 0x1;
    goto L_80261940;
L_80261884: ;
    fn_801DAC78();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80261898;
    r3 = 0x1;
    goto L_80261940;
L_80261898: ;
    r0 = *(u8*)((u8*)r29 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8026191C;
    r31 = *(u32*)((u8*)r29 + 0x8);
    *(u32*)((u8*)r31 + 0x0) = r28;
    *(u16*)((u8*)r31 + 0x4) = r3;
    r3 = r30;
    fn_801DA354();
    *(u8*)((u8*)r31 + 0x6) = r3;
    r5 = *(u32*)((u8*)r29 + 0x4);
    if ((u32)r30 != (u32)0x0) goto L_802618D0;
    r4 = 0x0;
    goto L_802618FC;
L_802618D0: ;
    r0 = 0x4;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r0;
L_802618DC: ;
    r4 = r5 + r3;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r0 != (u32)r30) goto L_802618F0;
    goto L_802618FC;
L_802618F0: ;
    r3 = r3 + 0x10;
    if (--ctr != 0) goto L_802618DC;
    r4 = 0x0;
L_802618FC: ;
    *(u32*)((u8*)r31 + 0x8) = r4;
    r3 = r30;
    fn_801DAC3C();
    r4 = r31 + 0xc;
    fn_800E9C6C();
    r3 = *(u32*)((u8*)r29 + 0x8);
    r0 = r3 + 0x78;
    *(u32*)((u8*)r29 + 0x8) = r0;
L_8026191C: ;
    r0 = *(u8*)((u8*)r29 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_80261930;
    r3 = r30;
    fn_801DB100();
L_80261930: ;
    r4 = *(u16*)((u8*)r29 + 0x2);
    r3 = 0x1;
    r0 = r4 + 0x1;
    *(u16*)((u8*)r29 + 0x2) = r0;
L_80261940: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80261954 | Size: 0x17C (380 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261954(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8000DD0C();
    extern void fn_8000DD30();
    extern void fn_8000DD98();
    extern void fn_8000DDBC();
    extern void fn_800F0308();
    extern void fn_801F1700();
    extern void fn_801F1758();
    extern void fn_801F2B5C();
    extern void fn_801F37B0();
    extern void fn_80261BEC();
    extern void fn_80261CBC();
    extern void fn_80261EF8();
    extern void fn_80261FB4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)fn_80261CBC;
    r5 = 0x0;
    r4 = (u32)fn_80261CBC;
    r6 = 0x0;
    /* stmw r30, 0x18(r1) */;
    r30 = r3;
    r3 = 0x0;
    fn_801F2B5C();
    r4 = (u32)fn_80261FB4;
    r3 = 0x0;
    r4 = (u32)fn_80261FB4;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802619B0;
    fn_8000DDBC();
L_802619B0: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802619C8;
    fn_8000DD30();
L_802619C8: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261ABC;
    r4 = (u32)fn_80261CBC;
    r3 = 0x0;
    r4 = (u32)fn_80261CBC;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r3 = (u32)fn_80261BEC;
    r30 = 0x1;
    r31 = (u32)fn_80261BEC;
L_802619F8: ;
    *(u8*)(sp + 0x8) = r30;
    r4 = r31;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r0 == (u32)0x1) goto L_80261A24;
    fn_800F0308();
    goto L_802619F8;
L_80261A24: ;
    r4 = (u32)fn_80261FB4;
    r3 = 0x0;
    r4 = (u32)fn_80261FB4;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = (u32)fn_80261EF8;
    r31 = (u32)fn_80261EF8;
L_80261A44: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80261A6C;
    fn_800F0308();
    goto L_80261A44;
L_80261A6C: ;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261A94;
L_80261A80: ;
    fn_8000DD98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80261A94;
    fn_800F0308();
    goto L_80261A80;
L_80261A94: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261ABC;
L_80261AA8: ;
    fn_8000DD0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80261ABC;
    fn_800F0308();
    goto L_80261AA8;
L_80261ABC: ;
    /* lmw r30, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80261AD0 | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261AD0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8000DD5C();
    extern void fn_8000DDE8();
    extern void fn_801F1700();
    extern void fn_801F1758();
    extern void fn_801F2B5C();
    extern void fn_801F37B0();
    extern void fn_80261D8C();
    extern void fn_80262084();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r4 = (u32)fn_80261D8C;
    r5 = 0x0;
    r4 = (u32)fn_80261D8C;
    r6 = 0x0;
    r31 = r3;
    r3 = 0x0;
    fn_801F2B5C();
    r0 = 0x1;
    r3 = (u32)fn_80262084;
    *(u8*)(sp + 0x8) = r0;
    r4 = (u32)fn_80262084;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = (s8)r31;
    if ((s32)r0 >= (s32)0) goto L_80261B3C;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261B3C;
    fn_8000DDE8();
L_80261B3C: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261B54;
    fn_8000DD5C();
L_80261B54: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x80261B68 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80261B68(void* ctx, u32 slot, u32 param) {
    extern void fn_800F0308();
    extern void fn_801F2B5C();
    extern void fn_80261BEC();
    extern void fn_80261CBC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)fn_80261CBC;
    r5 = 0x0;
    r4 = (u32)fn_80261CBC;
    r6 = 0x0;
    /* stmw r30, 0x18(r1) */;
    r30 = r3;
    r3 = 0x0;
    fn_801F2B5C();
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261BD8;
    r3 = (u32)fn_80261BEC;
    r30 = 0x1;
    r31 = (u32)fn_80261BEC;
L_80261BAC: ;
    *(u8*)(sp + 0x8) = r30;
    r4 = r31;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r0 == (u32)0x1) goto L_80261BD8;
    fn_800F0308();
    goto L_80261BAC;
L_80261BD8: ;
    /* lmw r30, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80261BEC | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261BEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80102620();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r30 = r3;
    r28 = r4;
    r29 = r5;
    r4 = r30;
    r3 = 0x2;
    r5 = r28;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80261C28;
    r3 = 0x0;
    goto L_80261C84;
L_80261C28: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r28;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80261C58;
    r3 = 0x0;
    goto L_80261C84;
L_80261C58: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80261C70;
    r3 = 0x0;
    goto L_80261C84;
L_80261C70: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80261C84: ;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261CA4;
    if ((u32)r29 == (u32)0x0) goto L_80261CA4;
    r0 = 0x0;
    *(u8*)((u8*)r29 + 0x0) = r0;
L_80261CA4: ;
    /* lmw r28, 0x10(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* Address: 0x80261CBC | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261CBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r4;
    r30 = r3;
    r3 = 0x2;
    r4 = r30;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80261CF4;
    r31 = 0x0;
    goto L_80261D54;
L_80261CF4: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80261D24;
    r31 = 0x0;
    goto L_80261D54;
L_80261D24: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80261D3C;
    r31 = 0x0;
    goto L_80261D54;
L_80261D3C: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
    r31 = r3;
L_80261D54: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_80261D74;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
L_80261D74: ;
    /* lmw r29, 0x14(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* Address: 0x80261D8C | Size: 0xF0 (240 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261D8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801026A4();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    extern void fn_801F7954();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x24(r1) */;
    r29 = r4;
    r30 = r3;
    r3 = 0x2;
    r4 = r30;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80261DC4;
    r31 = 0x0;
    goto L_80261E24;
L_80261DC4: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80261DF4;
    r31 = 0x0;
    goto L_80261E24;
L_80261DF4: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80261E0C;
    r31 = 0x0;
    goto L_80261E24;
L_80261E0C: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
    r31 = r3;
L_80261E24: ;
    r3 = r30;
    r4 = r1 + 0x8;
    fn_801F7954();
    r4 = *(u32*)(sp + 0x8);
    r3 = r31;
    r0 = *(u16*)(sp + 0xC);
    r9 = r1 + 0x10;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    *(u16*)(sp + 0x14) = r0;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    /* lmw r29, 0x24(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* Address: 0x80261E7C | Size: 0x7C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80261E7C(void* ctx, u32 slot, u32 param) {
    extern void fn_800F0308();
    extern void fn_801F37B0();
    extern void fn_80261EF8();
    extern void fn_80261FB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r4 = (u32)fn_80261FB4;
    r5 = 0x0;
    r4 = (u32)fn_80261FB4;
    r6 = 0x0;
    r31 = r3;
    r3 = 0x0;
    fn_801F37B0();
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80261EE4;
    r3 = (u32)fn_80261EF8;
    r31 = (u32)fn_80261EF8;
L_80261EBC: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80261EE4;
    fn_800F0308();
    goto L_80261EBC;
L_80261EE4: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x80261EF8 | Size: 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261EF8(void* ctx, u32 param1, u32 param2) {
    extern void fn_80102620();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r4;
    r30 = r3;
    r3 = 0x2;
    r4 = r30;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80261F30;
    r3 = 0x0;
    goto L_80261F8C;
L_80261F30: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80261F60;
    r3 = 0x0;
    goto L_80261F8C;
L_80261F60: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80261F78;
    r3 = 0x0;
    goto L_80261F8C;
L_80261F78: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_80261F8C: ;
    fn_80102620();
    r0 = r3 & 0xFF;
    /* lmw r29, 0x14(r1) */;
    r3 = 0x1 - r0;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
    return;
}
#pragma pop

/* Address: 0x80261FB4 | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80261FB4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r4;
    r30 = r3;
    r3 = 0x2;
    r4 = r30;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80261FEC;
    r31 = 0x0;
    goto L_8026204C;
L_80261FEC: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_8026201C;
    r31 = 0x0;
    goto L_8026204C;
L_8026201C: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80262034;
    r31 = 0x0;
    goto L_8026204C;
L_80262034: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    r31 = r3;
L_8026204C: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_8026206C;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
L_8026206C: ;
    /* lmw r29, 0x14(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* Address: 0x80262084 | Size: 0x140 (320 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80262084(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801026A4();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    extern void fn_801FE168();
    extern void fn_802062FC();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r28, 0x70(r1) */;
    r28 = r4;
    r30 = r3;
    if ((u32)r5 != (u32)0x0) goto L_802620AC;
    r29 = 0x1;
    goto L_802620B0;
L_802620AC: ;
    r29 = *(u8*)((u8*)r5 + 0x0);
L_802620B0: ;
    r3 = r30;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r5 != (u32)0x0) goto L_802620C8;
    r3 = 0x1;
    goto L_802621B0;
L_802620C8: ;
    r4 = r30;
    r5 = r28;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802620E8;
    r31 = 0x0;
    goto L_80262148;
L_802620E8: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r28;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80262118;
    r31 = 0x0;
    goto L_80262148;
L_80262118: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80262130;
    r31 = 0x0;
    goto L_80262148;
L_80262130: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    r31 = r3;
L_80262148: ;
    r3 = r30;
    r4 = r1 + 0x8;
    fn_801FE168();
    r0 = r29 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80262164;
    r0 = 0x0;
    *(u8*)(sp + 0x31) = r0;
L_80262164: ;
    r0 = 0x6;
    r5 = r1 + 0x34;
    r4 = r1 + 0x4;
    ctr_fn = (void(*)(void))r0;
L_80262174: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_80262174;
    r3 = r31;
    r9 = r1 + 0x38;
    r4 = -0x1;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0x1;
L_802621B0: ;
    /* lmw r28, 0x70(r1) */;
    return;
}
#pragma pop

/* Address: 0x802621C4 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_802621C4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802621F4 | Size: 0x7C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802621F4(void* ctx, u32 slot, u32 param) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 == (u32)0x0) return;
    if ((u32)r4 == (u32)0x0) return;
    if ((u32)r5 == (u32)0x0) return;
    r6 = *(s16*)((u8*)r4 + 0x2);
    r0 = *(s16*)((u8*)r3 + 0x2);
    r0 = r0 - r6;
    *(u16*)((u8*)r5 + 0x2) = r0;
    r6 = *(s16*)((u8*)r4 + 0x4);
    r0 = *(s16*)((u8*)r3 + 0x4);
    r0 = r0 - r6;
    *(u16*)((u8*)r5 + 0x4) = r0;
    r6 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(s16*)((u8*)r3 + 0x6);
    r0 = r0 - r6;
    *(u16*)((u8*)r5 + 0x6) = r0;
    r6 = *(s16*)((u8*)r4 + 0xA);
    r0 = *(s16*)((u8*)r3 + 0xA);
    r0 = r0 - r6;
    *(u16*)((u8*)r5 + 0xA) = r0;
    r6 = *(s16*)((u8*)r4 + 0xC);
    r0 = *(s16*)((u8*)r3 + 0xC);
    r0 = r0 - r6;
    *(u16*)((u8*)r5 + 0xC) = r0;
    r4 = *(s16*)((u8*)r4 + 0x8);
    r0 = *(s16*)((u8*)r3 + 0x8);
    r0 = r0 - r4;
    *(u16*)((u8*)r5 + 0x8) = r0;
    return;
}
#pragma pop

/* Address: 0x80262270 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80262270(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E6C8[];
    extern void fn_80097A38();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    f1 = *(f32*)lbl_8047E6C8;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r30;
    r4 = r31;
    fn_80097A38();
    r31 = r3;
    if ((s32)r31 != (s32)0x4) goto L_802622B8;
    r31 = -0x1;
L_802622B8: ;
    f1 = *(f32*)lbl_8047E6C8;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802622E4 | Size: 0x24 | Pattern: null_check_getter */
extern void fn_80105C68(u32);
void fn_802622E4(void) { fn_80105C68(1); }

/* Address: 0x80262308 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_80262308(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80262334 | Size: 0x80 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80262334(void* ctx, u32 slot, u32 param) {
    extern void fn_800FA280();
    extern void fn_80106394();
    extern void fn_80132A38();
    extern void fn_80142CF4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r5;
    r3 = 0x10;
    fn_80132A38();
    r4 = r31;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_80142CF4();
    fn_800FA280();
    r0 = r3;
    r3 = 0x29;
    r4 = r0;
    fn_80132A38();
    if ((u32)r30 == (u32)0x0) goto L_8026239C;
    r3 = r30;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
    r3 = 0x1;
    goto L_802623A0;
L_8026239C: ;
    r3 = 0x0;
L_802623A0: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x802623B4 | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802623B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_800FA280();
    extern void fn_80106394();
    extern void fn_8011BEB4();
    extern void fn_80132A38();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    r4 = r3;
    r3 = 0xf;
    fn_80132A38();
    r4 = r31;
    r3 = 0x0;
    r5 = 0xa;
    r6 = 0x0;
    fn_8011BEB4();
    fn_800FA280();
    r0 = r3;
    r3 = 0xd;
    r4 = r0;
    fn_80132A38();
    r4 = r31;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8011BEB4();
    fn_800FA280();
    r4 = r3;
    r3 = 0x28;
    fn_80132A38();
    r4 = r31;
    r3 = 0x0;
    r5 = 0xb;
    r6 = 0x0;
    fn_8011BEB4();
    fn_800FA280();
    r4 = r3;
    r3 = 0xe;
    fn_80132A38();
    r3 = 0x768d;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
    r3 = 0x1;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x8026246C | Size: 0x24 | Pattern: null_check_getter */
extern void fn_80106080(u32);
void fn_8026246C(void) { fn_80106080(0); }

/* Address: 0x80262490 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80262490(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802624CC | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802624CC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80262508 | Size: 0x82C (2092 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80262508(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375CA8[];
    extern u8 lbl_80375D30[];
    extern u8 lbl_80478DF8[];
    extern void fn_800111C4();
    extern void fn_80089F58();
    extern void fn_80089F60();
    extern void fn_80089F68();
    extern void fn_80089F70();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801EF634();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F1700();
    extern void fn_801F18DC();
    extern void fn_801F47B4();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F76B8();
    extern void fn_801F9130();
    extern void fn_801F9790();
    extern void fn_801F981C();
    extern void fn_801FB1C0();
    extern void fn_801FF1BC();
    extern void fn_80204F6C();
    extern void fn_8020505C();
    extern void fn_80205B8C();
    extern void fn_80205C24();
    extern void fn_80207760();
    extern void fn_8020E1A4();
    extern void fn_8020E204();
    extern void fn_8022B2CC();
    extern void fn_80265924();
    extern void fn_80262D34();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    f32 f1 = 0.0f;
    f32 f3 = 0.0f;
    f32 f5 = 0.0f;

    r5 = 0x4b;
    r6 = 0x0;
    /* stmw r14, 0x18(r1) */;
    r16 = r4;
    r15 = r3;
    r4 = 0x0;
    fn_801FB1C0();
    r0 = r3;
    r3 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8026262C;
    r4 = r15;
    r5 = r16;
    r14 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_8026256C;
    r3 = 0x0;
    goto L_802625C8;
L_8026256C: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r17 = r3 & 0xFFFF;
    r3 = r15;
    r4 = r16;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_8026259C;
    r3 = 0x0;
    goto L_802625C8;
L_8026259C: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802625B4;
    r3 = 0x0;
    goto L_802625C8;
L_802625B4: ;
    r4 = r17;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_802625C8: ;
    if ((s32)r3 == (s32)0xf3) goto L_80262600;
    if ((s32)r3 >= (s32)0xf3) goto L_802625E4;
    if ((s32)r3 == (s32)0xf1) goto L_802625F0;
    if ((s32)r3 >= (s32)0xf1) goto L_802625F8;
    goto L_8026260C;
L_802625E4: ;
    if ((s32)r3 >= (s32)0xf5) goto L_8026260C;
    goto L_80262608;
L_802625F0: ;
    r14 = 0x100;
    goto L_8026260C;
L_802625F8: ;
    r14 = 0x101;
    goto L_8026260C;
L_80262600: ;
    r14 = 0x102;
    goto L_8026260C;
L_80262608: ;
    r14 = 0x103;
L_8026260C: ;
    r3 = r14;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
L_8026262C: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    r3 = r16;
    *(u16*)(sp + 0x14) = r0;
    fn_8020E204();
    fn_8020E1A4();
    r28 = r3 & 0xFF;
    r25 = 0x0;
    r29 = 0x0;
    goto L_80262B58;
L_80262664: ;
    r3 = r15;
    r4 = r29;
    fn_801F981C();
    /* mr. r31, r3 */;
    if ((s32)r3 != (s32)0xf5) goto L_80262680;
    r25 = r29;
    goto L_80262B54;
L_80262680: ;
    r4 = 0x1;
    fn_80205C24();
    r0 = r3 & 0xFF;
    if ((s32)r3 != (s32)0xf5) goto L_80262698;
    r25 = r29;
    goto L_80262B54;
L_80262698: ;
    r14 = r29 & 0xFFFF;
L_8026269C: ;
    r3 = r31;
    fn_80207760();
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80262970;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802626D8;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80262954;
L_802626D8: ;
    r3 = *(u32*)(sp + 0x10);
    r4 = r15;
    r5 = r14;
    r6 = r16;
    fn_800111C4();
    r18 = r3;
    fn_80089F70();
    r17 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80262970;
    if ((s32)r17 != (s32)0x3) goto L_80262734;
    r4 = (u32)lbl_80375D30;
    r3 = r31;
    r7 = (u32)lbl_80375D30;
    r5 = 0x8;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x0;
    fn_8020505C();
    goto L_80262B50;
L_80262734: ;
    if ((s32)r17 != (s32)0x1) goto L_802628F0;
    r3 = r31;
    r4 = 0x1;
    fn_801FF1BC();
    r0 = r3 & 0xFF;
    if ((s32)r17 == (s32)0x1) goto L_80262758;
    r25 = r29;
    goto L_80262B54;
L_80262758: ;
    r3 = r18;
    fn_80089F68();
    r20 = (s8)r3;
    if ((s32)r17 < (s32)0x1) goto L_8026269C;
    r3 = r31;
    fn_80205B8C();
    r6 = r20;
    r4 = 0x0;
    r5 = 0x7f;
    ((void(*)(void))fn_8012640C)();
    r26 = r3 & 0xFFFF;
    if ((s32)r17 == (s32)0x1) goto L_8026269C;
    r3 = *(u32*)lbl_80478DF8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r26 >= (u32)r0) goto L_8026269C;
    if ((u32)r26 == (u32)0x165) goto L_8026269C;
    r4 = (u32)fn_80262D34;
    r3 = r31;
    r6 = (u32)fn_80262D34;
    r5 = r16;
    r4 = r26;
    r30 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    r9 = -0x1;
    fn_8022B2CC();
    if ((u32)r3 == (u32)0x0) goto L_802627D8;
    r30 = r3;
    goto L_802628A8;
L_802627D8: ;
    r3 = r18;
    fn_80089F60();
    r27 = r3 & 0xFFFF;
    r22 = 0x0;
    r17 = 0x0;
    r24 = 0x0;
    goto L_8026289C;
L_802627F4: ;
    r4 = r24;
    r3 = 0x0;
    fn_801F47B4();
    /* mr. r18, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_80262898;
    r23 = 0x0;
    goto L_8026287C;
L_80262810: ;
    r3 = r18;
    r4 = r23;
    fn_801F7258();
    /* mr. r19, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_80262878;
    r21 = 0x0;
    goto L_80262860;
L_8026282C: ;
    r3 = r19;
    r6 = r21;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    r0 = r22 & 0xFFFF;
    r30 = r3;
    if ((u32)r0 != (u32)r27) goto L_80262858;
    r17 = 0x1;
    goto L_8026286C;
L_80262858: ;
    r22 = r22 + 0x1;
    r21 = r21 + 0x1;
L_80262860: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8026282C;
L_8026286C: ;
    r0 = r17 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8026288C;
L_80262878: ;
    r23 = r23 + 0x1;
L_8026287C: ;
    r0 = *(u16*)(sp + 0x14);
    r3 = r23 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_80262810;
L_8026288C: ;
    r0 = r17 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802628A8;
L_80262898: ;
    r24 = r24 + 0x1;
L_8026289C: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_802627F4;
L_802628A8: ;
    if ((u32)r30 == (u32)0x0) goto L_8026269C;
    r3 = r30;
    r4 = r16;
    fn_801F0134();
    r0 = 0x0;
    r4 = (u32)lbl_80375CA8;
    *(u32*)(sp + 0x8) = r0;
    r9 = r3;
    r7 = (u32)lbl_80375CA8;
    r3 = r31;
    r8 = r26;
    r10 = r20;
    r4 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_80204F6C();
    goto L_80262B50;
L_802628F0: ;
    if ((s32)r17 != (s32)0x2) goto L_8026294C;
    r3 = r18;
    fn_80089F58();
    r6 = r3 & 0xFFFF;
    r3 = r15;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r8 = (s16)r3;
    if ((s32)r17 < (s32)0x2) goto L_8026269C;
    r4 = (u32)lbl_80375D30;
    r3 = r31;
    r7 = (u32)lbl_80375D30;
    r5 = 0x9;
    r4 = 0x0;
    r6 = 0x0;
    fn_8020505C();
    goto L_80262B50;
L_8026294C: ;
    if ((s32)r17 != (s32)0x4) goto L_80262968;
L_80262954: ;
    r3 = r15;
    r4 = r31;
    r5 = r16;
    fn_801F9130();
    goto L_80262B50;
L_80262968: ;
    if ((s32)r17 != (s32)0x5) goto L_80262B38;
L_80262970: ;
    r3 = r15;
    fn_801F9790();
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((s32)r17 == (s32)0x5) goto L_80262B30;
    r4 = r15;
    r5 = r16;
    r14 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802629AC;
    r3 = 0x0;
    goto L_80262A08;
L_802629AC: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r17 = r3 & 0xFFFF;
    r3 = r15;
    r4 = r16;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802629DC;
    r3 = 0x0;
    goto L_80262A08;
L_802629DC: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802629F4;
    r3 = 0x0;
    goto L_80262A08;
L_802629F4: ;
    r4 = r17;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80262A08: ;
    if ((s32)r3 == (s32)0xf3) goto L_80262A40;
    if ((s32)r3 >= (s32)0xf3) goto L_80262A24;
    if ((s32)r3 == (s32)0xf1) goto L_80262A30;
    if ((s32)r3 >= (s32)0xf1) goto L_80262A38;
    goto L_80262A4C;
L_80262A24: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80262A4C;
    goto L_80262A48;
L_80262A30: ;
    r14 = 0x100;
    goto L_80262A4C;
L_80262A38: ;
    r14 = 0x101;
    goto L_80262A4C;
L_80262A40: ;
    r14 = 0x102;
    goto L_80262A4C;
L_80262A48: ;
    r14 = 0x103;
L_80262A4C: ;
    r3 = r14;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0xf5) goto L_80262B30;
    r4 = r15;
    r5 = r16;
    r14 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80262A80;
    r3 = 0x0;
    goto L_80262ADC;
L_80262A80: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r17 = r3 & 0xFFFF;
    r3 = r15;
    r4 = r16;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80262AB0;
    r3 = 0x0;
    goto L_80262ADC;
L_80262AB0: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80262AC8;
    r3 = 0x0;
    goto L_80262ADC;
L_80262AC8: ;
    r4 = r17;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80262ADC: ;
    if ((s32)r3 == (s32)0xf3) goto L_80262B14;
    if ((s32)r3 >= (s32)0xf3) goto L_80262AF8;
    if ((s32)r3 == (s32)0xf1) goto L_80262B04;
    if ((s32)r3 >= (s32)0xf1) goto L_80262B0C;
    goto L_80262B20;
L_80262AF8: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80262B20;
    goto L_80262B1C;
L_80262B04: ;
    r14 = 0x100;
    goto L_80262B20;
L_80262B0C: ;
    r14 = 0x101;
    goto L_80262B20;
L_80262B14: ;
    r14 = 0x102;
    goto L_80262B20;
L_80262B1C: ;
    r14 = 0x103;
L_80262B20: ;
    r3 = r14;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80262B30: ;
    r3 = 0x0;
    goto L_80262D20;
L_80262B38: ;
    if ((s32)r17 != (s32)0x0) goto L_80262954;
    r0 = r29 & 0xFFFF;
    if ((s32)r17 == (s32)0x0) goto L_80262664;
    r29 = r25;
    goto L_80262664;
L_80262B50: ;
    r25 = r29;
L_80262B54: ;
    r29 = r29 + 0x1;
L_80262B58: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_80262664;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r28) goto L_80262D1C;
    r4 = r15;
    r5 = r16;
    r14 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80262B98;
    r3 = 0x0;
    goto L_80262BF4;
L_80262B98: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r17 = r3 & 0xFFFF;
    r3 = r15;
    r4 = r16;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80262BC8;
    r3 = 0x0;
    goto L_80262BF4;
L_80262BC8: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80262BE0;
    r3 = 0x0;
    goto L_80262BF4;
L_80262BE0: ;
    r4 = r17;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80262BF4: ;
    if ((s32)r3 == (s32)0xf3) goto L_80262C2C;
    if ((s32)r3 >= (s32)0xf3) goto L_80262C10;
    if ((s32)r3 == (s32)0xf1) goto L_80262C1C;
    if ((s32)r3 >= (s32)0xf1) goto L_80262C24;
    goto L_80262C38;
L_80262C10: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80262C38;
    goto L_80262C34;
L_80262C1C: ;
    r14 = 0x100;
    goto L_80262C38;
L_80262C24: ;
    r14 = 0x101;
    goto L_80262C38;
L_80262C2C: ;
    r14 = 0x102;
    goto L_80262C38;
L_80262C34: ;
    r14 = 0x103;
L_80262C38: ;
    r3 = r14;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0xf5) goto L_80262D1C;
    r4 = r15;
    r5 = r16;
    r14 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80262C6C;
    r3 = 0x0;
    goto L_80262CC8;
L_80262C6C: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r17 = r3 & 0xFFFF;
    r3 = r15;
    r4 = r16;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80262C9C;
    r3 = 0x0;
    goto L_80262CC8;
L_80262C9C: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80262CB4;
    r3 = 0x0;
    goto L_80262CC8;
L_80262CB4: ;
    r4 = r17;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80262CC8: ;
    if ((s32)r3 == (s32)0xf3) goto L_80262D00;
    if ((s32)r3 >= (s32)0xf3) goto L_80262CE4;
    if ((s32)r3 == (s32)0xf1) goto L_80262CF0;
    if ((s32)r3 >= (s32)0xf1) goto L_80262CF8;
    goto L_80262D0C;
L_80262CE4: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80262D0C;
    goto L_80262D08;
L_80262CF0: ;
    r14 = 0x100;
    goto L_80262D0C;
L_80262CF8: ;
    r14 = 0x101;
    goto L_80262D0C;
L_80262D00: ;
    r14 = 0x102;
    goto L_80262D0C;
L_80262D08: ;
    r14 = 0x103;
L_80262D0C: ;
    r3 = r14;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80262D1C: ;
    r3 = 0x1;
L_80262D20: ;
    /* lmw r14, 0x18(r1) */;
    return;
}
#pragma pop

/* Address: 0x80262D3C | Size: 0x430 (1072 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80262D3C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_800111E8();
    extern void fn_80089F58();
    extern void fn_80089F70();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801EF634();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F1700();
    extern void fn_801F18DC();
    extern void fn_801F76B8();
    extern void fn_801F93F8();
    extern void fn_801FB1C0();
    extern void fn_8020E1A4();
    extern void fn_8020E204();
    extern void fn_8024E690();
    extern void fn_80265924();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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

    /* stmw r25, 0x14(r1) */;
    r30 = r4;
    r28 = r5;
    r31 = r6;
    r29 = r3;
    r4 = 0x0;
    r5 = 0x4b;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3;
    r3 = r30;
    r26 = r0;
    fn_8020E204();
    fn_8020E1A4();
    r27 = r3 & 0xFF;
    r25 = 0x0;
    goto L_80262DAC;
L_80262D8C: ;
    r3 = r29;
    r6 = r25;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    if ((u32)r3 == (u32)r31) goto L_80262DB8;
    r25 = r25 + 0x1;
L_80262DAC: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_80262D8C;
L_80262DB8: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_80262DDC;
    r3 = r29;
    r4 = r30;
    r5 = r28;
    r6 = r31;
    fn_8024E690();
    goto L_80263158;
L_80262DDC: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80262F98;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80262E10;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80262F78;
L_80262E10: ;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80262F04;
    r4 = r29;
    r5 = r30;
    r28 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80262E44;
    r3 = 0x0;
    goto L_80262EA0;
L_80262E44: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r27 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80262E74;
    r3 = 0x0;
    goto L_80262EA0;
L_80262E74: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80262E8C;
    r3 = 0x0;
    goto L_80262EA0;
L_80262E8C: ;
    r4 = r27;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80262EA0: ;
    if ((s32)r3 == (s32)0xf3) goto L_80262ED8;
    if ((s32)r3 >= (s32)0xf3) goto L_80262EBC;
    if ((s32)r3 == (s32)0xf1) goto L_80262EC8;
    if ((s32)r3 >= (s32)0xf1) goto L_80262ED0;
    goto L_80262EE4;
L_80262EBC: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80262EE4;
    goto L_80262EE0;
L_80262EC8: ;
    r28 = 0x100;
    goto L_80262EE4;
L_80262ED0: ;
    r28 = 0x101;
    goto L_80262EE4;
L_80262ED8: ;
    r28 = 0x102;
    goto L_80262EE4;
L_80262EE0: ;
    r28 = 0x103;
L_80262EE4: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
L_80262F04: ;
    r3 = r26;
    r4 = r29;
    r6 = r30;
    r5 = r25 & 0xFFFF;
    fn_800111E8();
    r27 = r3;
    fn_80089F70();
    r28 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80262F98;
    if ((s32)r28 != (s32)0x2) goto L_80262F70;
    r3 = r27;
    fn_80089F58();
    r6 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = (s16)r3;
    goto L_80262F9C;
L_80262F70: ;
    if ((s32)r28 != (s32)0x4) goto L_80262F90;
L_80262F78: ;
    r3 = r29;
    r4 = r31;
    r5 = r30;
    fn_801F93F8();
    r31 = r3;
    goto L_80262F9C;
L_80262F90: ;
    if ((s32)r28 != (s32)0x5) goto L_80262F78;
L_80262F98: ;
    r31 = -0x2;
L_80262F9C: ;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((s32)r28 == (s32)0x5) goto L_80263154;
    r4 = r29;
    r5 = r30;
    r28 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80262FD0;
    r3 = 0x0;
    goto L_8026302C;
L_80262FD0: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r27 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263000;
    r3 = 0x0;
    goto L_8026302C;
L_80263000: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263018;
    r3 = 0x0;
    goto L_8026302C;
L_80263018: ;
    r4 = r27;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_8026302C: ;
    if ((s32)r3 == (s32)0xf3) goto L_80263064;
    if ((s32)r3 >= (s32)0xf3) goto L_80263048;
    if ((s32)r3 == (s32)0xf1) goto L_80263054;
    if ((s32)r3 >= (s32)0xf1) goto L_8026305C;
    goto L_80263070;
L_80263048: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80263070;
    goto L_8026306C;
L_80263054: ;
    r28 = 0x100;
    goto L_80263070;
L_8026305C: ;
    r28 = 0x101;
    goto L_80263070;
L_80263064: ;
    r28 = 0x102;
    goto L_80263070;
L_8026306C: ;
    r28 = 0x103;
L_80263070: ;
    r3 = r28;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0xf5) goto L_80263154;
    r4 = r29;
    r5 = r30;
    r28 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802630A4;
    r3 = 0x0;
    goto L_80263100;
L_802630A4: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r27 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802630D4;
    r3 = 0x0;
    goto L_80263100;
L_802630D4: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802630EC;
    r3 = 0x0;
    goto L_80263100;
L_802630EC: ;
    r4 = r27;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80263100: ;
    if ((s32)r3 == (s32)0xf3) goto L_80263138;
    if ((s32)r3 >= (s32)0xf3) goto L_8026311C;
    if ((s32)r3 == (s32)0xf1) goto L_80263128;
    if ((s32)r3 >= (s32)0xf1) goto L_80263130;
    goto L_80263144;
L_8026311C: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80263144;
    goto L_80263140;
L_80263128: ;
    r28 = 0x100;
    goto L_80263144;
L_80263130: ;
    r28 = 0x101;
    goto L_80263144;
L_80263138: ;
    r28 = 0x102;
    goto L_80263144;
L_80263140: ;
    r28 = 0x103;
L_80263144: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80263154: ;
    r3 = r31;
L_80263158: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80263BC8 | Size: 0x21C (540 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80263BC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375D30[];
    extern void fn_80106080();
    extern void fn_80106394();
    extern void fn_80132A38();
    extern void fn_801F000C();
    extern void fn_801F54A4();
    extern void fn_801FF1BC();
    extern void fn_8020505C();
    extern void fn_80263DE4();
    extern void fn_80264488();
    extern void fn_80264D58();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f5 = 0.0f;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r31 = 0x0;
    if ((s32)r6 == (s32)0x2) goto L_80263CE4;
    if ((s32)r6 >= (s32)0x2) goto L_80263C04;
    if ((s32)r6 == (s32)0x0) goto L_80263C10;
    if ((s32)r6 >= (s32)0x0) goto L_80263C80;
    goto L_80263DC8;
L_80263C04: ;
    if ((s32)r6 >= (s32)0x4) goto L_80263DC8;
    goto L_80263D38;
L_80263C10: ;
    r3 = r28;
    r4 = 0x1;
    fn_801FF1BC();
    r30 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263C58;
    r4 = r28;
    r3 = 0x11;
    fn_80132A38();
    r3 = 0x75fc;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
    r3 = 0x40;
    fn_801F000C();
    r3 = 0x0;
    fn_80106080();
L_80263C58: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263DCC;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    fn_80264D58();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263DCC;
    r31 = 0x2;
    goto L_80263DCC;
L_80263C80: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x20;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263CC4;
    r3 = 0x75f5;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
    r3 = 0x40;
    fn_801F000C();
    r3 = 0x0;
    fn_80106080();
    r31 = 0x2;
    goto L_80263DCC;
L_80263CC4: ;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    fn_80264488();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263DCC;
    r31 = 0x2;
    goto L_80263DCC;
L_80263CE4: ;
    r6 = 0x1;
    r7 = 0x1;
    fn_80263DE4();
    r5 = r3;
    r0 = (s16)r5;
    if ((u32)r0 >= (u32)0x1) goto L_80263D04;
    r0 = 0x0;
    goto L_80263D28;
L_80263D04: ;
    r4 = (u32)lbl_80375D30;
    r8 = (s16)r5;
    r7 = (u32)lbl_80375D30;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_8020505C();
    r0 = 0x1;
L_80263D28: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263DCC;
    r31 = 0x2;
    goto L_80263DCC;
L_80263D38: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x22;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263D7C;
    r4 = (u32)lbl_80375D30;
    r3 = r28;
    r7 = (u32)lbl_80375D30;
    r5 = 0x8;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x0;
    fn_8020505C();
    goto L_80263DCC;
L_80263D7C: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263DC0;
    r4 = (u32)lbl_80375D30;
    r3 = r28;
    r7 = (u32)lbl_80375D30;
    r5 = 0xa;
    r4 = 0x0;
    r6 = 0x0;
    r8 = 0x0;
    fn_8020505C();
    goto L_80263DCC;
L_80263DC0: ;
    r31 = 0x2;
    goto L_80263DCC;
L_80263DC8: ;
    r31 = 0x1;
L_80263DCC: ;
    r3 = r31;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80263DE4 | Size: 0x6A4 (1700 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80263DE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8000DD0C();
    extern void fn_8000DD30();
    extern void fn_8000DD5C();
    extern void fn_8000DD98();
    extern void fn_8000DDBC();
    extern void fn_8000DDE8();
    extern void fn_800114A4();
    extern void fn_80011D9C();
    extern void fn_800F0308();
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801EF634();
    extern void fn_801EFFC4();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F1700();
    extern void fn_801F1758();
    extern void fn_801F18DC();
    extern void fn_801F2B5C();
    extern void fn_801F37B0();
    extern void fn_801F76B8();
    extern void fn_801F93F8();
    extern void fn_801FB1C0();
    extern void fn_80206A04();
    extern void fn_80265924();
    extern void fn_80261BEC();
    extern void fn_80261CBC();
    extern void fn_80261D8C();
    extern void fn_80261EF8();
    extern void fn_80261FB4();
    extern void fn_80262084();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r24, 0x10(r1) */;
    r30 = r3;
    r29 = r4;
    r31 = r5;
    r26 = r6;
    r27 = r7;
L_80263E08: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80263E20;
L_80263E18: ;
    r3 = -0x2;
    goto L_80264474;
L_80263E20: ;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263E44;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802643E4;
L_80263E44: ;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80263F40;
    r0 = r27 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263F40;
    r4 = r30;
    r5 = r31;
    r25 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80263E80;
    r3 = 0x0;
    goto L_80263EDC;
L_80263E80: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r28 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80263EB0;
    r3 = 0x0;
    goto L_80263EDC;
L_80263EB0: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80263EC8;
    r3 = 0x0;
    goto L_80263EDC;
L_80263EC8: ;
    r4 = r28;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80263EDC: ;
    if ((s32)r3 == (s32)0xf3) goto L_80263F14;
    if ((s32)r3 >= (s32)0xf3) goto L_80263EF8;
    if ((s32)r3 == (s32)0xf1) goto L_80263F04;
    if ((s32)r3 >= (s32)0xf1) goto L_80263F0C;
    goto L_80263F20;
L_80263EF8: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80263F20;
    goto L_80263F1C;
L_80263F04: ;
    r25 = 0x100;
    goto L_80263F20;
L_80263F0C: ;
    r25 = 0x101;
    goto L_80263F20;
L_80263F14: ;
    r25 = 0x102;
    goto L_80263F20;
L_80263F1C: ;
    r25 = 0x103;
L_80263F20: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
L_80263F40: ;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((s32)r3 != (s32)0xf5) goto L_80264098;
    r4 = (u32)fn_80261CBC;
    r3 = 0x0;
    r4 = (u32)fn_80261CBC;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r4 = (u32)fn_80261FB4;
    r3 = 0x0;
    r4 = (u32)fn_80261FB4;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263F98;
    fn_8000DDBC();
L_80263F98: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263FB0;
    fn_8000DD30();
L_80263FB0: ;
    r4 = (u32)fn_80261CBC;
    r3 = 0x0;
    r4 = (u32)fn_80261CBC;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r3 = (u32)fn_80261BEC;
    r25 = 0x1;
    r28 = (u32)fn_80261BEC;
L_80263FD4: ;
    *(u8*)(sp + 0x9) = r25;
    r4 = r28;
    r5 = r1 + 0x9;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r0 = *(u8*)(sp + 0x9);
    if ((u32)r0 == (u32)0x1) goto L_80264000;
    fn_800F0308();
    goto L_80263FD4;
L_80264000: ;
    r4 = (u32)fn_80261FB4;
    r3 = 0x0;
    r4 = (u32)fn_80261FB4;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = (u32)fn_80261EF8;
    r28 = (u32)fn_80261EF8;
L_80264020: ;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80264048;
    fn_800F0308();
    goto L_80264020;
L_80264048: ;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264070;
L_8026405C: ;
    fn_8000DD98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80264070;
    fn_800F0308();
    goto L_8026405C;
L_80264070: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264098;
L_80264084: ;
    fn_8000DD0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80264098;
    fn_800F0308();
    goto L_80264084;
L_80264098: ;
    r3 = 0x0;
    fn_801F18DC();
    r7 = r3;
    r3 = r30;
    r4 = r29;
    r5 = r31;
    r6 = r26;
    fn_800114A4();
    r28 = r3;
    r3 = 0xa;
    fn_801EFFC4();
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802641E8;
    r0 = r27 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802641E8;
    r4 = (u32)fn_80261D8C;
    r3 = 0x0;
    r4 = (u32)fn_80261D8C;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r0 = 0x1;
    r3 = (u32)fn_80262084;
    *(u8*)(sp + 0x8) = r0;
    r4 = (u32)fn_80262084;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264130;
    fn_8000DDE8();
L_80264130: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264148;
    fn_8000DD5C();
L_80264148: ;
    if ((u32)r29 == (u32)0x0) goto L_802641E8;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802641E8;
    r4 = r29;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80264184;
    r3 = 0x0;
    goto L_802641E0;
L_80264184: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r25 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802641B4;
    r3 = 0x0;
    goto L_802641E0;
L_802641B4: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802641CC;
    r3 = 0x0;
    goto L_802641E0;
L_802641CC: ;
    r4 = r25;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_802641E0: ;
    r4 = 0x1;
    fn_80011D9C();
L_802641E8: ;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_802643A8;
    r0 = r27 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_802643A8;
    r4 = r30;
    r5 = r31;
    r25 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80264224;
    r3 = 0x0;
    goto L_80264280;
L_80264224: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80264254;
    r3 = 0x0;
    goto L_80264280;
L_80264254: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_8026426C;
    r3 = 0x0;
    goto L_80264280;
L_8026426C: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80264280: ;
    if ((s32)r3 == (s32)0xf3) goto L_802642B8;
    if ((s32)r3 >= (s32)0xf3) goto L_8026429C;
    if ((s32)r3 == (s32)0xf1) goto L_802642A8;
    if ((s32)r3 >= (s32)0xf1) goto L_802642B0;
    goto L_802642C4;
L_8026429C: ;
    if ((s32)r3 >= (s32)0xf5) goto L_802642C4;
    goto L_802642C0;
L_802642A8: ;
    r25 = 0x100;
    goto L_802642C4;
L_802642B0: ;
    r25 = 0x101;
    goto L_802642C4;
L_802642B8: ;
    r25 = 0x102;
    goto L_802642C4;
L_802642C0: ;
    r25 = 0x103;
L_802642C4: ;
    r3 = r25;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0xf5) goto L_802643A8;
    r4 = r30;
    r5 = r31;
    r25 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802642F8;
    r3 = 0x0;
    goto L_80264354;
L_802642F8: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r24 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80264328;
    r3 = 0x0;
    goto L_80264354;
L_80264328: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80264340;
    r3 = 0x0;
    goto L_80264354;
L_80264340: ;
    r4 = r24;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80264354: ;
    if ((s32)r3 == (s32)0xf3) goto L_8026438C;
    if ((s32)r3 >= (s32)0xf3) goto L_80264370;
    if ((s32)r3 == (s32)0xf1) goto L_8026437C;
    if ((s32)r3 >= (s32)0xf1) goto L_80264384;
    goto L_80264398;
L_80264370: ;
    if ((s32)r3 >= (s32)0xf5) goto L_80264398;
    goto L_80264394;
L_8026437C: ;
    r25 = 0x100;
    goto L_80264398;
L_80264384: ;
    r25 = 0x101;
    goto L_80264398;
L_8026438C: ;
    r25 = 0x102;
    goto L_80264398;
L_80264394: ;
    r25 = 0x103;
L_80264398: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_802643A8: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80263E18;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264400;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264400;
    if ((s32)r28 >= (s32)0x0) goto L_80264400;
L_802643E4: ;
    r0 = r27 & 0xFF;
    if ((s32)r28 != (s32)0x0) goto L_80264408;
    r3 = r30;
    r4 = r29;
    r5 = r31;
    fn_801F93F8();
    goto L_80264474;
L_80264400: ;
    if ((s32)r28 >= (s32)0x0) goto L_80264418;
L_80264408: ;
    r0 = r26 & 0xFF;
    if ((s32)r28 == (s32)0x0) goto L_80263E08;
    r3 = -0x1;
    goto L_80264474;
L_80264418: ;
    r3 = r30;
    r6 = r28 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    r24 = r3;
    fn_80206A04();
    r0 = r3 & 0xFF;
    if ((s32)r28 == (s32)0x0) goto L_80263E08;
    r3 = r24;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80263E08;
    r3 = r24;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = (s16)r3;
L_80264474: ;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x80264488 | Size: 0x654 (1620 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80264488(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375D70[];
    extern u8 lbl_8047E6CC[];
    extern u8 lbl_8047E6D0[];
    extern void fn_8000DD0C();
    extern void fn_8000DD30();
    extern void fn_8000DD5C();
    extern void fn_8000DD98();
    extern void fn_8000DDBC();
    extern void fn_8000DDE8();
    extern void fn_80011D9C();
    extern void fn_80018F88();
    extern void fn_80019064();
    extern void fn_800D37CC();
    extern void fn_800F0308();
    extern void fn_80102038();
    extern void fn_8010206C();
    extern void fn_80142CF4();
    extern void fn_801DA36C();
    extern void fn_801EFFC4();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F1700();
    extern void fn_801F1758();
    extern void fn_801F18DC();
    extern void fn_801F2B5C();
    extern void fn_801F37B0();
    extern void fn_801F76B8();
    extern void fn_801F7EF0();
    extern void fn_801F85B0();
    extern void fn_801F8638();
    extern void fn_801FB1C0();
    extern void fn_802026E4();
    extern void fn_80204CE0();
    extern void fn_802062FC();
    extern void fn_80206608();
    extern void fn_8022FF90();
    extern void fn_80264ADC();
    extern void fn_80261BEC();
    extern void fn_80261CBC();
    extern void fn_80261D8C();
    extern void fn_80261EF8();
    extern void fn_80261FB4();
    extern void fn_80262084();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
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
    f32 f1 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x80) = f31;
    /* psq_st f31, 0x88(r1), 0, qr0 */;
    *(f64*)(sp + 0x70) = f30;
    /* psq_st f30, 0x78(r1), 0, qr0 */;
    /* stmw r16, 0x30(r1) */;
    r6 = 0x0;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r5 = r1 + 0x1c;
    r4 = r1 + 0x18;
    r3 = r1 + 0x14;
    r7 = 0x0;
    goto L_802644E4;
L_802644D0: ;
    r0 = r7 & 0xFFFF;
    r7 = r7 + 0x1;
    *(u8*)(r5 + r0) = r6;
    *(u8*)(r4 + r0) = r6;
    *(u8*)(r3 + r0) = r6;
L_802644E4: ;
    r0 = r7 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_802644D0;
    r17 = 0x0;
    goto L_80264574;
L_802644F8: ;
    r3 = r28;
    r6 = r17;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    /* mr. r18, r3 */;
    if ((u32)r0 == (u32)0x2) goto L_80264570;
    r3 = r28;
    r4 = r18;
    fn_801F85B0();
    r0 = r3;
    r3 = r28;
    r6 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    fn_80206608();
    r16 = r17 & 0xFFFF;
    r4 = r1 + 0x1c;
    *(u8*)(r4 + r16) = r3;
    r3 = r18;
    r4 = 0x8;
    fn_802026E4();
    r5 = r1 + 0x18;
    r4 = 0x7;
    *(u8*)(r5 + r16) = r3;
    r3 = r18;
    fn_802026E4();
    r4 = r1 + 0x14;
    *(u8*)(r4 + r16) = r3;
L_80264570: ;
    r17 = r17 + 0x1;
L_80264574: ;
    r0 = r17 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_802644F8;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r3 != (u32)0x0) goto L_802645A4;
    r3 = 0x0;
    goto L_80264AB8;
L_802645A4: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r28;
    r4 = r0;
    fn_801F8638();
    r0 = (s16)r3;
    r4 = (u32)fn_80261FB4;
    r3 = (u32)fn_80261CBC;
    r6 = (u32)fn_80261BEC;
    r23 = (u32)fn_80261FB4;
    r5 = (u32)fn_80261EF8;
    r22 = (u32)fn_80261CBC;
    r4 = (u32)fn_80261D8C;
    r3 = (u32)fn_80262084;
    *(u32*)(sp + 0x20) = r0;
    f30 = *(f64*)lbl_8047E6D0;
    r24 = (u32)fn_80261BEC;
    f31 = *(f32*)lbl_8047E6CC;
    r25 = (u32)fn_80261EF8;
    r26 = (u32)fn_80261D8C;
    r27 = (u32)fn_80262084;
    r16 = (0x4330 << 16);
L_8026460C: ;
    r4 = r22;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r4 = r23;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8026464C;
    fn_8000DDBC();
L_8026464C: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264664;
    fn_8000DD30();
L_80264664: ;
    r4 = r22;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r17 = 0x1;
L_8026467C: ;
    *(u8*)(sp + 0x11) = r17;
    r4 = r24;
    r5 = r1 + 0x11;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r0 = *(u8*)(sp + 0x11);
    if ((u32)r0 == (u32)0x1) goto L_802646A8;
    fn_800F0308();
    goto L_8026467C;
L_802646A8: ;
    r4 = r23;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
L_802646BC: ;
    r4 = r25;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802646E4;
    fn_800F0308();
    goto L_802646BC;
L_802646E4: ;
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8026470C;
L_802646F8: ;
    fn_8000DD98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8026470C;
    fn_800F0308();
    goto L_802646F8;
L_8026470C: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264734;
L_80264720: ;
    fn_8000DD0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80264734;
    fn_800F0308();
    goto L_80264720;
L_80264734: ;
    fn_800D37CC();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x2C) = r0;
    f0 = *(f64*)(sp + 0x28);
    f0 = f0 - f30;
    f1 = f31 / f0;
    fn_8010206C();
    r5 = r28;
    r4 = r1 + 0x20;
    r3 = 0x1;
    fn_80018F88();
    r0 = r3;
    r3 = 0xa;
    r31 = r0;
    fn_801EFFC4();
    fn_80019064();
    r0 = r31 & 0xFFFF;
    r21 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80264854;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_80142CF4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_80264854;
    r19 = 0x0;
    goto L_80264848;
L_802647AC: ;
    r3 = r28;
    r6 = r19;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    /* mr. r18, r3 */;
    if ((u32)r0 == (u32)0x2) goto L_80264844;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r17, r3 */;
    if ((u32)r0 == (u32)0x2) goto L_80264844;
    r20 = r19 & 0xFFFF;
    r3 = r1 + 0x18;
    r0 = *(u8*)(r3 + r20);
    if ((u32)r0 != (u32)0x1) goto L_80264814;
    r3 = r18;
    r4 = 0x8;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264814;
    r3 = r17;
    r4 = 0x1;
    fn_801DA36C();
L_80264814: ;
    r3 = r1 + 0x14;
    r0 = *(u8*)(r3 + r20);
    if ((u32)r0 != (u32)0x1) goto L_80264844;
    r3 = r18;
    r4 = 0x7;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264844;
    r3 = r17;
    r4 = 0x2;
    fn_801DA36C();
L_80264844: ;
    r19 = r19 + 0x1;
L_80264848: ;
    r0 = r19 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_802647AC;
L_80264854: ;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F2B5C();
    r0 = 0x1;
    r4 = r27;
    *(u8*)(sp + 0x10) = r0;
    r5 = r1 + 0x10;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8026489C;
    fn_8000DDE8();
L_8026489C: ;
    r3 = 0x0;
    fn_801F1758();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802648B4;
    fn_8000DD5C();
L_802648B4: ;
    if ((u32)r29 == (u32)0x0) goto L_80264954;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80264954;
    r4 = r29;
    r5 = r30;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802648F0;
    r3 = 0x0;
    goto L_8026494C;
L_802648F0: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r17 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80264920;
    r3 = 0x0;
    goto L_8026494C;
L_80264920: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80264938;
    r3 = 0x0;
    goto L_8026494C;
L_80264938: ;
    r4 = r17;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_8026494C: ;
    r4 = 0x1;
    fn_80011D9C();
L_80264954: ;
    fn_800D37CC();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x2C) = r0;
    f0 = *(f64*)(sp + 0x28);
    f0 = f0 - f30;
    f1 = f31 / f0;
    fn_80102038();
    r3 = 0xa;
    fn_801EFFC4();
    r0 = r31 & 0xFFFF;
    if ((s32)r3 != (s32)0x0) goto L_8026498C;
    r3 = 0x0;
    goto L_80264AB8;
L_8026498C: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_80142CF4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802649EC;
    r3 = r28;
    fn_801F7EF0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8026460C;
    r3 = r29;
    r4 = r31;
    r5 = r30;
    fn_80264ADC();
    if ((u32)r3 == (u32)0x0) goto L_8026460C;
    r4 = r30;
    fn_801F0134();
    r4 = 0x0;
    r9 = r3;
    goto L_80264A8C;
L_802649EC: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_80142CF4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_80264A80;
    fn_8022FF90();
    r16 = 0x0;
    goto L_80264A74;
L_80264A18: ;
    r3 = r28;
    r6 = r16;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    /* mr. r17, r3 */;
    if ((u32)r0 == (u32)0x2) goto L_80264A70;
    r0 = r16 & 0xFFFF;
    r4 = r1 + 0x1c;
    r0 = *(u8*)(r4 + r0);
    if ((u32)r0 != (u32)0x0) goto L_80264A70;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264A70;
    r3 = r17;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    r7 = 0x1;
    ((void(*)(void))fn_801254B4)();
L_80264A70: ;
    r16 = r16 + 0x1;
L_80264A74: ;
    r0 = r16 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_80264A18;
L_80264A80: ;
    r0 = *(u32*)(sp + 0x20);
    r4 = 0x1;
    r9 = r0 & 0xFFFF;
L_80264A8C: ;
    r3 = (u32)lbl_80375D70;
    r7 = (u32)lbl_80375D70;
    r10 = r21;
    r3 = r29;
    r8 = r31 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x12;
    r6 = 0x0;
    fn_80204CE0();
    r3 = 0x1;
L_80264AB8: ;
    /* psq_l f31, 0x88(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x80);
    /* psq_l f30, 0x78(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x70);
    /* lmw r16, 0x30(r1) */;
    return;
}
#pragma pop

/* Address: 0x80264ADC | Size: 0x27C (636 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80264ADC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8001120C();
    extern void fn_80011288();
    extern void fn_80106080();
    extern void fn_80106394();
    extern void fn_8011FC74();
    extern void fn_801906A0();
    extern void fn_801F000C();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F18DC();
    extern void fn_801F76B8();
    extern void fn_80205B8C();
    extern void fn_802062FC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x30(r1) */;
    r30 = r3;
    r31 = r5;
L_80264AF4: ;
    r4 = r30;
    r5 = r31;
    r3 = 0xf;
    fn_801F02AC();
    r28 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264B98;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80264B38;
    r3 = 0x0;
    goto L_80264B9C;
L_80264B38: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r29 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80264B68;
    r3 = 0x0;
    goto L_80264B9C;
L_80264B68: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80264B80;
    r3 = 0x0;
    goto L_80264B9C;
L_80264B80: ;
    r4 = r29;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    goto L_80264B9C;
L_80264B98: ;
    r3 = 0x0;
L_80264B9C: ;
    r4 = r30;
    r5 = r31;
    r3 = 0x10;
    fn_801F02AC();
    r28 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264C44;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80264BE4;
    r3 = 0x0;
    goto L_80264C48;
L_80264BE4: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r29 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80264C14;
    r3 = 0x0;
    goto L_80264C48;
L_80264C14: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80264C2C;
    r3 = 0x0;
    goto L_80264C48;
L_80264C2C: ;
    r4 = r29;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    goto L_80264C48;
L_80264C44: ;
    r3 = 0x0;
L_80264C48: ;
    r4 = 0x0;
    r0 = 0x2;
    r3 = 0x0;
    *(u8*)(sp + 0x28) = r0;
    fn_801F18DC();
    *(u8*)(sp + 0x29) = r3;
    r3 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0x1;
    fn_80011288();
    if ((s32)r3 >= (s32)0x0) goto L_80264C94;
    r3 = 0x1;
    fn_8001120C();
    r3 = 0x0;
    goto L_80264D44;
L_80264C94: ;
    if ((s32)r3 != (s32)0x0) goto L_80264CB0;
    r4 = r30;
    r5 = r31;
    r3 = 0xf;
    fn_801F02AC();
    r29 = r3;
    goto L_80264CCC;
L_80264CB0: ;
    if ((s32)r3 != (s32)0x1) goto L_80264AF4;
    r4 = r30;
    r5 = r31;
    r3 = 0x10;
    fn_801F02AC();
    r29 = r3;
L_80264CCC: ;
    r3 = r29;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x1) goto L_80264AF4;
    r3 = r29;
    fn_80205B8C();
    fn_8011FC74();
    r0 = r3 & 0xFF;
    if ((s32)r3 != (s32)0x1) goto L_80264D38;
    r3 = 0x99f;
    fn_801906A0();
    if ((u32)r3 != (u32)0x0) goto L_80264D14;
    r3 = 0x7716;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
    goto L_80264D24;
L_80264D14: ;
    r3 = 0x7702;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
L_80264D24: ;
    r3 = 0x40;
    fn_801F000C();
    r3 = 0x0;
    fn_80106080();
    goto L_80264AF4;
L_80264D38: ;
    r3 = 0x1;
    fn_8001120C();
    r3 = r29;
L_80264D44: ;
    /* lmw r28, 0x30(r1) */;
    return;
}
#pragma pop

/* Address: 0x80264D58 | Size: 0x2E4 (740 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80264D58(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80375CA8[];
    extern u8 lbl_80478DF8[];
    extern u8 lbl_8047B678[];
    extern void fn_80011700();
    extern void fn_800117BC();
    extern void fn_800FA280();
    extern void fn_80106080();
    extern void fn_80106394();
    extern void fn_8011BEB4();
    extern void fn_80132A38();
    extern void fn_801F000C();
    extern void fn_801F0134();
    extern void fn_801F18DC();
    extern void fn_801F4C14();
    extern void fn_801FE3F8();
    extern void fn_801FFEC8();
    extern void fn_802040E8();
    extern void fn_80204F6C();
    extern void fn_80205B8C();
    extern void fn_8020E1A4();
    extern void fn_8020E204();
    extern void fn_8022B2CC();
    extern void fn_8026503C();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r24, 0x60(r1) */;
    r25 = r5;
    r30 = r4;
    r3 = r25;
    fn_8020E204();
    fn_8020E1A4();
    r26 = r3 & 0xFF;
    r3 = r30;
    r4 = r1 + 0x14;
    fn_801FE3F8();
    r3 = 0x0;
    fn_801F18DC();
    *(u8*)(sp + 0x58) = r3;
    r3 = r30;
    fn_80205B8C();
    r28 = r3;
L_80264DA4: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x101;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r1 + 0x14;
    r4 = r0;
    r5 = 0x1;
    fn_800117BC();
    /* mr. r31, r3 */;
    if ((s32)r0 >= (s32)0) goto L_80264DE4;
    r3 = 0x1;
    fn_80011700();
    r3 = 0x0;
    goto L_80265028;
L_80264DE4: ;
    r3 = r30;
    r4 = r31 & 0xFFFF;
    r6 = r1 + 0x10;
    r5 = 0x1;
    fn_801FFEC8();
    r29 = r3;
    r3 = r28;
    r6 = r31 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x7f;
    ((void(*)(void))fn_8012640C)();
    r0 = r29 & 0xFF;
    r24 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80264E70;
    r4 = r30;
    r3 = 0x11;
    fn_80132A38();
    r4 = r24;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8011BEB4();
    fn_800FA280();
    r4 = r3;
    r3 = 0x28;
    fn_80132A38();
    r3 = r30;
    fn_802040E8();
    r0 = r3;
    r3 = 0x0;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x56;
    r6 = 0x0;
    fn_801F4C14();
L_80264E70: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x6) goto L_80264E84;
    r27 = 0x7661;
    goto L_80264EF4;
L_80264E84: ;
    if ((u32)r0 != (u32)0x5) goto L_80264EB8;
    r4 = *(u16*)(sp + 0x10);
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8011BEB4();
    fn_800FA280();
    r4 = r3;
    r3 = 0x28;
    fn_80132A38();
    r27 = 0x76bb;
    goto L_80264EF4;
L_80264EB8: ;
    if ((u32)r0 != (u32)0x4) goto L_80264EC8;
    r27 = 0x7600;
    goto L_80264EF4;
L_80264EC8: ;
    if ((u32)r0 != (u32)0x3) goto L_80264ED8;
    r27 = 0x75ff;
    goto L_80264EF4;
L_80264ED8: ;
    if ((u32)r0 != (u32)0x2) goto L_80264EE8;
    r27 = 0x75fe;
    goto L_80264EF4;
L_80264EE8: ;
    if ((u32)r0 != (u32)0x1) goto L_80264EF4;
    r27 = 0x75fd;
L_80264EF4: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80264F28;
    if ((u32)r27 == (u32)0x0) goto L_80264F14;
    r3 = r27;
    r4 = 0x1;
    r5 = 0x1;
    fn_80106394();
L_80264F14: ;
    r3 = 0x40;
    fn_801F000C();
    r3 = 0x0;
    fn_80106080();
    goto L_80264DA4;
L_80264F28: ;
    r3 = r28;
    r6 = r31 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x7f;
    ((void(*)(void))fn_8012640C)();
    r29 = r3 & 0xFFFF;
    if ((u32)r27 == (u32)0x0) goto L_80264DA4;
    r3 = *(u32*)lbl_80478DF8;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r29 >= (u32)r0) goto L_80264DA4;
    if ((u32)r29 == (u32)0x165) goto L_80264DA4;
    r0 = 0x0;
    r3 = (u32)fn_8026503C;
    *(u8*)lbl_8047B678 = r0;
    r6 = (u32)fn_8026503C;
    r3 = r30;
    r4 = r29;
    r5 = r25;
    r7 = 0x1;
    r8 = 0x0;
    r9 = -0x1;
    fn_8022B2CC();
    r0 = r3;
    r3 = 0x0;
    r24 = r0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80264FD0;
    r0 = *(u8*)lbl_8047B678;
    if ((u32)r0 != (u32)0x0) goto L_80264FD0;
    if ((u32)r26 < (u32)0x2) goto L_80264FD0;
    r3 = r30;
    r4 = r29;
    r5 = r25;
    fn_8026503C();
    if ((u32)r3 == (u32)0x0) goto L_80264DA4;
L_80264FD0: ;
    if ((u32)r24 == (u32)0x0) goto L_80264DA4;
    r3 = r24;
    r4 = r25;
    fn_801F0134();
    r0 = r3;
    r3 = 0x1;
    r24 = r0;
    fn_80011700();
    r0 = 0x0;
    r3 = (u32)lbl_80375CA8;
    *(u32*)(sp + 0x8) = r0;
    r7 = (u32)lbl_80375CA8;
    r3 = r30;
    r8 = r29;
    r9 = r24;
    r10 = (s8)r31;
    r4 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    fn_80204F6C();
    r3 = 0x1;
L_80265028: ;
    /* lmw r24, 0x60(r1) */;
    return;
}
#pragma pop

/* Address: 0x8026503C | Size: 0x2F0 (752 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8026503C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047B678[];
    extern void fn_8001120C();
    extern void fn_80011288();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F18DC();
    extern void fn_801F76B8();
    extern void fn_802062FC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x30(r1) */;
    r30 = r3;
    r31 = r5;
L_80265054: ;
    r4 = r30;
    r5 = r31;
    r3 = 0xf;
    fn_801F02AC();
    r28 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802650F8;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80265098;
    r3 = 0x0;
    goto L_802650FC;
L_80265098: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r29 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802650C8;
    r3 = 0x0;
    goto L_802650FC;
L_802650C8: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802650E0;
    r3 = 0x0;
    goto L_802650FC;
L_802650E0: ;
    r4 = r29;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    goto L_802650FC;
L_802650F8: ;
    r3 = 0x0;
L_802650FC: ;
    r4 = r30;
    r5 = r31;
    r3 = 0x10;
    fn_801F02AC();
    r28 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802651A4;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80265144;
    r3 = 0x0;
    goto L_802651A8;
L_80265144: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r29 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80265174;
    r3 = 0x0;
    goto L_802651A8;
L_80265174: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_8026518C;
    r3 = 0x0;
    goto L_802651A8;
L_8026518C: ;
    r4 = r29;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    goto L_802651A8;
L_802651A4: ;
    r3 = 0x0;
L_802651A8: ;
    r4 = r30;
    r5 = r31;
    r3 = 0xe;
    fn_801F02AC();
    r28 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80265250;
    r4 = r28;
    r5 = r31;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802651F0;
    r3 = 0x0;
    goto L_80265254;
L_802651F0: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r29 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80265220;
    r3 = 0x0;
    goto L_80265254;
L_80265220: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80265238;
    r3 = 0x0;
    goto L_80265254;
L_80265238: ;
    r4 = r29;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    goto L_80265254;
L_80265250: ;
    r3 = 0x0;
L_80265254: ;
    r4 = 0x0;
    r0 = 0x3;
    r3 = 0x0;
    *(u8*)(sp + 0x28) = r0;
    fn_801F18DC();
    *(u8*)(sp + 0x29) = r3;
    r3 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0x1;
    fn_80011288();
    r0 = 0x1;
    *(u8*)lbl_8047B678 = r0;
    if ((s32)r3 >= (s32)0x0) goto L_802652A4;
    r3 = 0x1;
    fn_8001120C();
    r3 = 0x0;
    goto L_80265318;
L_802652A4: ;
    if ((s32)r3 != (s32)0x0) goto L_802652C0;
    r4 = r30;
    r5 = r31;
    r3 = 0xf;
    fn_801F02AC();
    r29 = r3;
    goto L_802652FC;
L_802652C0: ;
    if ((s32)r3 != (s32)0x1) goto L_802652E0;
    r4 = r30;
    r5 = r31;
    r3 = 0x10;
    fn_801F02AC();
    r29 = r3;
    goto L_802652FC;
L_802652E0: ;
    if ((s32)r3 != (s32)0x2) goto L_80265054;
    r4 = r30;
    r5 = r31;
    r3 = 0xe;
    fn_801F02AC();
    r29 = r3;
L_802652FC: ;
    r3 = r29;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x2) goto L_80265054;
    r3 = 0x1;
    fn_8001120C();
    r3 = r29;
L_80265318: ;
    /* lmw r28, 0x30(r1) */;
    return;
}
#pragma pop

/* Address: 0x8026532C | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8026532C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x2;
    r4 = r28;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80265368;
    r31 = 0x0;
    goto L_802653C8;
L_80265368: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80265398;
    r31 = 0x0;
    goto L_802653C8;
L_80265398: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802653B0;
    r31 = 0x0;
    goto L_802653C8;
L_802653B0: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    r31 = r3;
L_802653C8: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_802653E8;
    r3 = r31;
    r5 = r30;
    r4 = 0x0;
    fn_80102568();
L_802653E8: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* Address: 0x802653FC | Size: 0x19C (412 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802653FC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    extern void fn_801FE168();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r28, 0x70(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x2;
    r4 = r28;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80265438;
    r3 = 0x0;
    goto L_80265494;
L_80265438: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80265468;
    r3 = 0x0;
    goto L_80265494;
L_80265468: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80265480;
    r3 = 0x0;
    goto L_80265494;
L_80265480: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_80265494: ;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_80265584;
    r4 = r28;
    r5 = r29;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802654C0;
    r31 = 0x0;
    goto L_80265520;
L_802654C0: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802654F0;
    r31 = 0x0;
    goto L_80265520;
L_802654F0: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_80265508;
    r31 = 0x0;
    goto L_80265520;
L_80265508: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    r31 = r3;
L_80265520: ;
    r3 = r28;
    r4 = r1 + 0x8;
    fn_801FE168();
    r0 = r30 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_8026553C;
    r0 = 0x0;
    *(u8*)(sp + 0x31) = r0;
L_8026553C: ;
    r0 = 0x6;
    r5 = r1 + 0x34;
    r4 = r1 + 0x4;
    ctr_fn = (void(*)(void))r0;
L_8026554C: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8026554C;
    r3 = r31;
    r9 = r1 + 0x38;
    r4 = -0x1;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
L_80265584: ;
    /* lmw r28, 0x70(r1) */;
    return;
}
#pragma pop

/* Address: 0x80265598 | Size: 0x114 (276 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80265598(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801026A4();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    extern void fn_801FE168();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r28, 0x70(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = 0x2;
    r4 = r28;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802655D4;
    r31 = 0x0;
    goto L_80265634;
L_802655D4: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r29;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80265604;
    r31 = 0x0;
    goto L_80265634;
L_80265604: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_8026561C;
    r31 = 0x0;
    goto L_80265634;
L_8026561C: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
    r31 = r3;
L_80265634: ;
    r3 = r28;
    r4 = r1 + 0x8;
    fn_801FE168();
    r0 = r30 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80265650;
    r0 = 0x0;
    *(u8*)(sp + 0x31) = r0;
L_80265650: ;
    r0 = 0x6;
    r5 = r1 + 0x34;
    r4 = r1 + 0x4;
    ctr_fn = (void(*)(void))r0;
L_80265660: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_80265660;
    r3 = r31;
    r9 = r1 + 0x38;
    r4 = -0x1;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
    /* lmw r28, 0x70(r1) */;
    return;
}
#pragma pop

/* Address: 0x802656AC | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802656AC(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r4;
    r3 = 0x2;
    r4 = r29;
    r5 = r30;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_802656E4;
    r3 = 0x0;
    goto L_80265740;
L_802656E4: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80265714;
    r3 = 0x0;
    goto L_80265740;
L_80265714: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_8026572C;
    r3 = 0x0;
    goto L_80265740;
L_8026572C: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x3;
    fn_801F76B8();
L_80265740: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* Address: 0x80265754 | Size: 0x174 (372 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80265754(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801F0134();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F76B8();
    extern void fn_801F7954();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x24(r1) */;
    r29 = r3;
    r30 = r4;
    r3 = 0x2;
    r4 = r29;
    r5 = r30;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_8026578C;
    r3 = 0x0;
    goto L_802657E8;
L_8026578C: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_802657BC;
    r3 = 0x0;
    goto L_802657E8;
L_802657BC: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_802657D4;
    r3 = 0x0;
    goto L_802657E8;
L_802657D4: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_802657E8: ;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_802658B4;
    r4 = r29;
    r5 = r30;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_80265814;
    r31 = 0x0;
    goto L_80265874;
L_80265814: ;
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_80265844;
    r31 = 0x0;
    goto L_80265874;
L_80265844: ;
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 >= (s32)0x0) goto L_8026585C;
    r31 = 0x0;
    goto L_80265874;
L_8026585C: ;
    r4 = r31;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
    r31 = r3;
L_80265874: ;
    r3 = r29;
    r4 = r1 + 0x8;
    fn_801F7954();
    r4 = *(u32*)(sp + 0x8);
    r3 = r31;
    r0 = *(u16*)(sp + 0xC);
    r9 = r1 + 0x10;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    *(u16*)(sp + 0x14) = r0;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
L_802658B4: ;
    /* lmw r29, 0x24(r1) */;
    return;
}
#pragma pop

/* Address: 0x802658C8 | Size: 0x5C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802658C8(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478800[];
    extern u8 lbl_8047E6D8[];
    extern void fn_800F05A0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r3 = (u32)lbl_80478800;
    r31 = (u32)lbl_80478800;
    r3 = *(u32*)((u8*)r31 + 0xC);
    if ((u32)r3 == (u32)0x0) goto L_802658F0;
    fn_800F05A0();
L_802658F0: ;
    r0 = 0x0;
    r3 = (u32)lbl_80478800;
    r3 = (u32)lbl_80478800; *(u8*)r3 = r0;
    f0 = *(f32*)lbl_8047E6D8;
    *(u32*)((u8*)r31 + 0xC) = r0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x1) = r0;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x80265924 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80265924(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8026595C | Size: 0x48 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8026595C(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478800[];
    extern u8 lbl_8047E6D8[];
    extern u8 lbl_8047E6DC[];
    extern u8 lbl_8047E6E8[];
    u32 r0 = 0;
    u32 r3 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r3 = (u32)lbl_80478800;
    r3 = (u32)lbl_80478800;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 != (u32)0x1) goto L_80265978;
    f1 = *(f32*)lbl_8047E6E8;
    return;
L_80265978: ;
    r0 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_8026598C;
    f1 = *(f32*)lbl_8047E6D8;
    return;
L_8026598C: ;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = *(f32*)lbl_8047E6DC;
    f2 = *(f32*)((u8*)r3 + 0x8);
    f0 = f1 / f0;
    f1 = f2 - f0;
    return;
}
#pragma pop

/* Address: 0x802659A4 | Size: 0x54 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802659A4(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478800[];
    extern void fn_800F0438();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r3 = (u32)lbl_80478800;
    r3 = (u32)lbl_80478800;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 != (u32)0x1) goto L_802659CC;
    r3 = 0x0;
    goto L_802659E8;
L_802659CC: ;
    r3 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r3 != (u32)0x0) goto L_802659E0;
    r3 = 0x0;
    goto L_802659E8;
L_802659E0: ;
    fn_800F0438();
    r3 = 0x1;
L_802659E8: ;
    return;
}
#pragma pop

/* Address: 0x802659F8 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_802659F8(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478800[];
    extern void fn_800F0654();
    extern void fn_800F07A8();
    extern void fn_800FF560();
    extern void fn_80265DB0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r3 = (u32)lbl_80478800;
    r3 = (u32)lbl_80478800;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 == (u32)0x1) goto L_80265A5C;
    fn_800FF560();
    r5 = (u32)fn_80265DB0;
    r4 = r3;
    r8 = (u32)fn_80265DB0;
    r3 = 0x1;
    r5 = 0x4000;
    r6 = 0x1;
    r7 = 0x0;
    fn_800F07A8();
    if ((u32)r3 == (u32)0x0) goto L_80265A5C;
    r5 = (u32)lbl_80478800;
    r4 = 0x1;
    r5 = (u32)lbl_80478800;
    *(u32*)((u8*)r5 + 0xC) = r3;
    /* crclr cr1eq */;
    fn_800F0654();
L_80265A5C: ;
    return;
}
#pragma pop

/* Address: 0x80265A6C | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80265A6C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478800[];
    extern u8 lbl_8047E6D8[];
    extern u8 lbl_8047E6E8[];
    extern u8 lbl_8047E6F0[];
    extern void fn_80077B84();
    extern void fn_800F05A0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28(r1), 0, qr0 */;
    fn_80077B84();
    /* xoris r4, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    r3 = (u32)lbl_80478800;
    r31 = (u32)lbl_80478800;
    f1 = *(f64*)lbl_8047E6F0;
    *(u32*)(sp + 0x8) = r0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    if ((u32)r3 == (u32)0x0) goto L_80265AE0;
    if ((u32)r3 == (u32)0x0) goto L_80265AC0;
    fn_800F05A0();
L_80265AC0: ;
    r0 = 0x0;
    r3 = (u32)lbl_80478800;
    r3 = (u32)lbl_80478800; *(u8*)r3 = r0;
    f0 = *(f32*)lbl_8047E6D8;
    *(u32*)((u8*)r31 + 0xC) = r0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x1) = r0;
L_80265AE0: ;
    f0 = *(f32*)lbl_8047E6D8;
    r0 = 0x0;
    r3 = (u32)lbl_80478800;
    r3 = (u32)lbl_80478800; *(u8*)r3 = r0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    if (f31 >= f0) goto L_80265B10;
    f0 = *(f32*)lbl_8047E6E8;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x1) = r0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    goto L_80265B18;
L_80265B10: ;
    *(u8*)((u8*)r3 + 0x1) = r0;
    *(f32*)((u8*)r3 + 0x8) = f31;
L_80265B18: ;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0xC) = r0;
    /* psq_l f31, 0x28(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x80265B3C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80265B3C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80265B74 | Size: 0x48 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265B74(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478810[];
    extern u8 lbl_8047E6D8[];
    extern u8 lbl_8047E6DC[];
    extern u8 lbl_8047E6E8[];
    u32 r0 = 0;
    u32 r3 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r3 = (u32)lbl_80478810;
    r3 = (u32)lbl_80478810;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 != (u32)0x1) goto L_80265B90;
    f1 = *(f32*)lbl_8047E6E8;
    return;
L_80265B90: ;
    r0 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_80265BA4;
    f1 = *(f32*)lbl_8047E6D8;
    return;
L_80265BA4: ;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = *(f32*)lbl_8047E6DC;
    f2 = *(f32*)((u8*)r3 + 0x8);
    f0 = f1 / f0;
    f1 = f2 - f0;
    return;
}
#pragma pop

/* Address: 0x80265BBC | Size: 0x54 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265BBC(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478810[];
    extern void fn_800F0438();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r3 = (u32)lbl_80478810;
    r3 = (u32)lbl_80478810;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 != (u32)0x1) goto L_80265BE4;
    r3 = 0x0;
    goto L_80265C00;
L_80265BE4: ;
    r3 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r3 != (u32)0x0) goto L_80265BF8;
    r3 = 0x0;
    goto L_80265C00;
L_80265BF8: ;
    fn_800F0438();
    r3 = 0x1;
L_80265C00: ;
    return;
}
#pragma pop

/* Address: 0x80265C10 | Size: 0x74 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265C10(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478810[];
    extern void fn_800F0654();
    extern void fn_800F07A8();
    extern void fn_800FF560();
    extern void fn_80265DB0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r3 = (u32)lbl_80478810;
    r3 = (u32)lbl_80478810;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 == (u32)0x1) goto L_80265C74;
    fn_800FF560();
    r5 = (u32)fn_80265DB0;
    r4 = r3;
    r8 = (u32)fn_80265DB0;
    r3 = 0x1;
    r5 = 0x4000;
    r6 = 0x1;
    r7 = 0x0;
    fn_800F07A8();
    if ((u32)r3 == (u32)0x0) goto L_80265C74;
    r5 = (u32)lbl_80478810;
    r4 = 0x1;
    r5 = (u32)lbl_80478810;
    *(u32*)((u8*)r5 + 0xC) = r3;
    /* crclr cr1eq */;
    fn_800F0654();
L_80265C74: ;
    return;
}
#pragma pop

/* Address: 0x80265C84 | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80265C84(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_80478810[];
    extern u8 lbl_8047E6D8[];
    extern u8 lbl_8047E6E8[];
    extern u8 lbl_8047E6F0[];
    extern void fn_80077BA8();
    extern void fn_800F05A0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28(r1), 0, qr0 */;
    fn_80077BA8();
    /* xoris r4, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    r3 = (u32)lbl_80478810;
    r31 = (u32)lbl_80478810;
    f1 = *(f64*)lbl_8047E6F0;
    *(u32*)(sp + 0x8) = r0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    if ((u32)r3 == (u32)0x0) goto L_80265CF8;
    if ((u32)r3 == (u32)0x0) goto L_80265CD8;
    fn_800F05A0();
L_80265CD8: ;
    r0 = 0x0;
    r3 = (u32)lbl_80478810;
    r3 = (u32)lbl_80478810; *(u8*)r3 = r0;
    f0 = *(f32*)lbl_8047E6D8;
    *(u32*)((u8*)r31 + 0xC) = r0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x1) = r0;
L_80265CF8: ;
    f0 = *(f32*)lbl_8047E6D8;
    r0 = 0x0;
    r3 = (u32)lbl_80478810;
    r3 = (u32)lbl_80478810; *(u8*)r3 = r0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    if (f31 >= f0) goto L_80265D28;
    f0 = *(f32*)lbl_8047E6E8;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x1) = r0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    goto L_80265D30;
L_80265D28: ;
    *(u8*)((u8*)r3 + 0x1) = r0;
    *(f32*)((u8*)r3 + 0x8) = f31;
L_80265D30: ;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0xC) = r0;
    /* psq_l f31, 0x28(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* Address: 0x80265D54 | Size: 0x5C | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265D54(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_80478810[];
    extern u8 lbl_8047E6D8[];
    extern void fn_800F05A0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r3 = (u32)lbl_80478810;
    r31 = (u32)lbl_80478810;
    r3 = *(u32*)((u8*)r31 + 0xC);
    if ((u32)r3 == (u32)0x0) goto L_80265D7C;
    fn_800F05A0();
L_80265D7C: ;
    r0 = 0x0;
    r3 = (u32)lbl_80478810;
    r3 = (u32)lbl_80478810; *(u8*)r3 = r0;
    f0 = *(f32*)lbl_8047E6D8;
    *(u32*)((u8*)r31 + 0xC) = r0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x1) = r0;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* Address: 0x80265DB0 | Size: 0x84 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265DB0(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E6D8[];
    extern u8 lbl_8047E6DC[];
    extern u8 lbl_8047E6E0[];
    extern void fn_800D3088();
    extern void fn_800F0308();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x30) = f31;
    /* psq_st f31, 0x38(r1), 0, qr0 */;
    *(f64*)(sp + 0x20) = f30;
    /* psq_st f30, 0x28(r1), 0, qr0 */;
    /* stmw r30, 0x18(r1) */;
    f0 = *(f32*)lbl_8047E6D8;
    r30 = r3;
    f30 = *(f64*)lbl_8047E6E0;
    r31 = (0x4330 << 16);
    *(f32*)((u8*)r3 + 0x4) = f0;
    f31 = *(f32*)lbl_8047E6DC;
    goto L_80265E10;
L_80265DEC: ;
    fn_800F0308();
    fn_800D3088();
    f0 = *(f32*)((u8*)r30 + 0x4);
    f1 = *(f64*)(sp + 0x8);
    f1 = f1 - f30;
    f0 = f0 + f1;
    *(f32*)((u8*)r30 + 0x4) = f0;
L_80265E10: ;
    f0 = *(f32*)((u8*)r30 + 0x8);
    f1 = *(f32*)((u8*)r30 + 0x4);
    f0 = f31 * f0;
    if (f1 < f0) goto L_80265DEC;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x0) = r0;
L_80265E2C: ;
    fn_800F0308();
    goto L_80265E2C;
}
#pragma pop

/* Address: 0x80265E34 | Size: 0x48 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265E34(void* ctx, u32 slot, u32 param) {
    extern void fn_8011D5D4();
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = 0x1;
    fn_8011D5D4();
    r3 = 0x0;
    r4 = 0x10;
    fn_80129280();
    r0 = *(u8*)((u8*)r3 + 0x5);
    if ((u32)r0 != (u32)0x0) goto L_80265E68;
    r0 = 0x2d;
    *(u8*)((u8*)r3 + 0x5) = r0;
L_80265E68: ;
    r3 = 0x1;
    return;
}
#pragma pop

/* Address: 0x80265E7C | Size: 0x48 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265E7C(void* ctx, u32 slot, u32 param) {
    extern void fn_8011D5F8();
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = 0x1;
    fn_8011D5F8();
    r3 = 0x0;
    r4 = 0x10;
    fn_80129280();
    r0 = *(u8*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)0x0) goto L_80265EB0;
    r0 = 0x2c;
    *(u8*)((u8*)r3 + 0x4) = r0;
L_80265EB0: ;
    r3 = 0x1;
    return;
}
#pragma pop

/* Address: 0x80265EC4 | Size: 0x50 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265EC4(void* ctx, u32 slot, u32 param) {
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = 0x0;
    r4 = 0x10;
    fn_80129280();
    r0 = *(u8*)(r3 + r30);
    if ((u32)r0 != (u32)0x0) goto L_80265F00;
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80265F00;
    *(u8*)(r3 + r30) = r31;
L_80265F00: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* Address: 0x80265F14 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80265F14(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80265F4C | Size: 0x48 | Pattern: field_accessor */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80265F4C(void* ctx, u32 slot, u32 param) {
    extern void fn_80129280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((u32)r3 != (u32)0x0) goto L_80265F6C;
    r3 = 0x0;
    r4 = 0x10;
    fn_80129280();
L_80265F6C: ;
    r0 = 0xb;
    r4 = 0x0;
    ctr_fn = (void(*)(void))r0;
L_80265F78: ;
    *(u8*)((u8*)r3 + 0x0) = r4;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_80265F78;
    return;
}
#pragma pop

/* Address: 0x80265F94 | Size: 0x2BC (700 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80265F94(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E6F8[];
    extern u8 lbl_8047E6FC[];
    extern void fn_800F0308();
    extern void fn_801065B8();
    extern void fn_801067E8();
    extern void fn_80106934();
    extern void fn_8011E15C();
    extern void fn_8011E778();
    extern void fn_80132A38();
    extern void fn_801666BC();
    extern void fn_80166A28();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DA4E8();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB088();
    extern void fn_801DDD28();
    extern void fn_801DE190();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r4 = 0xfa;
    r5 = 0x66;
    r6 = 0x0;
    r29 = r3;
    r3 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFFFF;
    r3 = (0x1 << 16);
    /* subi r3, r3, 0x1 */;
    if ((s32)r0 == (s32)0) goto L_80265FDC;
    r3 = r0;
L_80265FDC: ;
    r4 = 0x0;
    r5 = 0x0;
    fn_801DE190();
    r4 = *(u16*)lbl_8047E6F8;
    r31 = r3;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r3 = r31;
    r4 = 0x1;
    fn_801DA4E8();
    r4 = *(u16*)lbl_8047E6F8;
    r3 = r31;
    r5 = 0x4;
    r30 = r4;
    fn_801DA9E8();
    f1 = *(f32*)lbl_8047E6FC;
    r3 = 0x2;
    fn_801C41C8();
    goto L_8026605C;
L_8026602C: ;
    fn_801DB088();
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80266058;
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA9E8();
L_80266058: ;
    fn_800F0308();
L_8026605C: ;
    r3 = 0x0;
    fn_801C40F0();
    r0 = (s8)r3;
    if ((s32)r0 != (s32)0) goto L_8026602C;
    r3 = 0xfa;
    fn_8011E778();
    if ((u32)r3 != (u32)0x0) goto L_80266084;
    r28 = 0x0;
    goto L_802660C8;
L_80266084: ;
    fn_8011E15C();
    r28 = r3 & 0xFFFF;
    r3 = r28;
    fn_80166A28();
    goto L_802660C8;
L_80266098: ;
    fn_801DB088();
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802660C4;
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA9E8();
L_802660C4: ;
    fn_800F0308();
L_802660C8: ;
    r3 = r28;
    fn_801666BC();
    if ((s32)r3 == (s32)0x2) goto L_80266098;
    r3 = 0x44ba;
    r4 = 0x0;
    r5 = 0x0;
    fn_801067E8();
    goto L_8026611C;
L_802660EC: ;
    fn_801DB088();
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((s32)r3 != (s32)0x2) goto L_80266118;
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA9E8();
L_80266118: ;
    fn_800F0308();
L_8026611C: ;
    fn_80106934();
    r0 = (s8)r3;
    r0 = 0x1 - r0;
    r0 = __cntlzw(r0);
    /* extrwi. r0, r0, 8, 19 */;
    if ((s32)r3 != (s32)0x2) goto L_802660EC;
    r3 = 0x1;
    fn_801065B8();
    if ((s32)r29 == (s32)0x1) goto L_80266168;
    if ((s32)r29 >= (s32)0x1) goto L_8026617C;
    if ((s32)r29 >= (s32)0x0) goto L_80266154;
    goto L_8026617C;
L_80266154: ;
    r3 = 0x5d;
    r4 = 0x3d2;
    fn_80132A38();
    r3 = 0x44bc;
    goto L_80266180;
L_80266168: ;
    r3 = 0x5d;
    r4 = 0x3d2;
    fn_80132A38();
    r3 = 0x44bb;
    goto L_80266180;
L_8026617C: ;
    r3 = 0x44b9;
L_80266180: ;
    r4 = 0x0;
    r5 = 0x0;
    fn_801067E8();
    goto L_802661C0;
L_80266190: ;
    fn_801DB088();
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((s32)r29 != (s32)0x0) goto L_802661BC;
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA9E8();
L_802661BC: ;
    fn_800F0308();
L_802661C0: ;
    fn_80106934();
    r0 = (s8)r3;
    r0 = 0x1 - r0;
    r0 = __cntlzw(r0);
    /* extrwi. r0, r0, 8, 19 */;
    if ((s32)r29 != (s32)0x0) goto L_80266190;
    r3 = 0x1;
    fn_801065B8();
    f1 = *(f32*)lbl_8047E6FC;
    r3 = 0x3;
    fn_801C41C8();
    goto L_80266220;
L_802661F0: ;
    fn_801DB088();
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((s32)r29 != (s32)0x0) goto L_8026621C;
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_801DA9E8();
L_8026621C: ;
    fn_800F0308();
L_80266220: ;
    r3 = 0x0;
    fn_801C40F0();
    r0 = (s8)r3;
    if ((s32)r29 != (s32)0x0) goto L_802661F0;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* Address: 0x80266250 | Size: 0xD0 (208 bytes) */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80266250(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047B680[];
    extern void fn_800FF660();
    extern void fn_8011288C();
    extern void fn_801653BC();
    extern void fn_801653C4();
    extern void fn_801656D8();
    extern void fn_801657D0();
    extern void fn_801DAC90();
    extern void fn_801DADC0();
    extern void fn_80265F94();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r31 = *(u32*)lbl_8047B680;
    fn_801653C4();
    /* mr. r27, r3 */;
    if ((s32)r0 == (s32)0) goto L_8026628C;
    fn_801656D8();
    r29 = r3;
    r3 = 0x1;
    r4 = 0x32;
    r5 = 0xff;
    ((void(*)(void))fn_80165A20)();
    goto L_80266290;
L_8026628C: ;
    r29 = 0x0;
L_80266290: ;
    fn_801653BC();
    /* mr. r28, r3 */;
    if ((s32)r0 == (s32)0) goto L_802662B0;
    fn_801656D8();
    r30 = r3;
    r3 = 0x32;
    fn_801657D0();
    goto L_802662B4;
L_802662B0: ;
    r30 = 0x0;
L_802662B4: ;
    r3 = 0x1;
    fn_801DADC0();
    r3 = r31;
    fn_80265F94();
    fn_801DAC90();
    if ((u32)r27 == (u32)0x0) goto L_802662E0;
    r3 = r27;
    r5 = r29 & 0xFF;
    r4 = 0x32;
    ((void(*)(void))fn_80165A20)();
L_802662E0: ;
    if ((u32)r28 == (u32)0x0) goto L_802662F8;
    r3 = r28;
    r5 = r30 & 0xFF;
    r4 = 0x32;
    ((void(*)(void))fn_801659FC)();
L_802662F8: ;
    fn_800FF660();
    r4 = (0x596 << 16);
    r3 = 0x0;
    r4 = r4 + 0x8;
    fn_8011288C();
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* Address: 0x80266320 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80266320(void* ctx, u32 param) { return 0; /* stub */ }
