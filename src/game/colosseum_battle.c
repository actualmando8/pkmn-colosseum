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
u32 fn_8025A290(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType);
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

    r6 = 0x1;
    r7 = 0x1;
    r26 = r3;
    r27 = r4;
    r28 = r5;
    r5 = (u32)sp + 0x68;
    r4 = r26;
    r29 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r30 = r3;
    r4 = r26;
    r5 = (u32)sp + 0x48;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r31 = r3;
    r24 = (u32)sp + 0x40;
    r25 = (u32)sp + 0x48;
    r22 = 0x0;
    r23 = r3 & 0xFFFF;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r24 + r0);
            if ((u32)r0 >= (u32)0x8 || (u32)r0 > (u32)0x9) {

                r0 = 0x1;
                goto L_80240CF0;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_80240CF0:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_80240D18;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_80240D18:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b6;
        fn_80239EE8();
    }
    r24 = (u32)sp + 0x38;
    r25 = (u32)sp + 0x68;
    r23 = r30 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r24 + r0);
            if ((u32)r0 >= (u32)0x3 || (u32)r0 > (u32)0x4) {

                r0 = 0x1;
                goto L_80240E34;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_80240E34:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_80240E5C;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_80240E5C:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b7;
        fn_80239EE8();
    }
    r24 = (u32)sp + 0x30;
    r25 = (u32)sp + 0x48;
    r23 = r31 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r24 + r0);
            if ((u32)r0 >= (u32)0xa || (u32)r0 > (u32)0xc) {

                r0 = 0x1;
                goto L_80240F78;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_80240F78:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_80240FA0;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_80240FA0:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b8;
        fn_80239EE8();
    }
    r24 = (u32)sp + 0x28;
    r25 = (u32)sp + 0x68;
    r23 = r30 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r24 + r0);
            if ((u32)r0 >= (u32)0x0 || (u32)r0 > (u32)0x2) {

                r0 = 0x1;
                goto L_802410BC;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_802410BC:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_802410E4;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_802410E4:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b9;
        fn_80239EE8();
    }
    r24 = (u32)sp + 0x20;
    r25 = (u32)sp + 0x68;
    r23 = r30 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r24 + r0);
            if ((u32)r0 >= (u32)0x8 || (u32)r0 > (u32)0x9) {

                r0 = 0x1;
                goto L_80241200;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_80241200:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_80241228;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_80241228:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1ba;
        fn_80239EE8();
    }
    r24 = (u32)sp + 0x18;
    r25 = (u32)sp + 0x48;
    r23 = r31 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r24 + r0);
            if ((u32)r0 >= (u32)0x3 || (u32)r0 > (u32)0x4) {

                r0 = 0x1;
                goto L_80241344;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_80241344:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_8024136C;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_8024136C:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1bb;
        fn_80239EE8();
    }
    r25 = (u32)sp + 0x10;
    r23 = (u32)sp + 0x68;
    r24 = r30 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r24) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r25 + r0);
            if ((u32)r0 >= (u32)0xa || (u32)r0 > (u32)0xc) {

                r0 = 0x1;
                goto L_80241488;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_80241488:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_802414B0;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_802414B0:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1bc;
        fn_80239EE8();
    }
    r25 = (u32)sp + 0x8;
    r24 = (u32)sp + 0x48;
    r30 = r31 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r30) break;
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
        while (1) {
            r0 = r3 & 0xFF;
            if ((u32)r0 >= (u32)0x7) break;
            r0 = r3 & 0xFF;
            r0 = *(u8*)(r25 + r0);
            if ((u32)r0 >= (u32)0x0 || (u32)r0 > (u32)0x2) {

                r0 = 0x1;
                goto L_802415CC;
            }
            r3 = r3 + 0x1;


        }
        r0 = 0x0;
    L_802415CC:
        r0 = r0 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x1;
            goto L_802415F4;
        }
        r22 = r22 + 0x1;


    }
    r0 = 0x0;
L_802415F4:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1bd;
        fn_80239EE8();
    }
    r3 = r29;
    return;
}

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
    if ((u32)r0 > (u32)0x1b) { r3 = r24; return; }
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
    fn_802399FC();
    r26 = r3;
    r3 = r29;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r26;
        r4 = r31;
        r5 = 0x114;
        fn_80239984();
        r26 = r3;
        r3 = r29;
        fn_80205B8C();
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
L_8024614C:
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
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x115;
    fn_80239CCC();
L_802461B8:
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
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
        if ((u32)r0 >= (u32)r26) break;
        r3 = r28;
        r4 = 0x0;
        r5 = 0xd5;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
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
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x10e;
        fn_80239CCC();
        break;
    L_80246304:
        r24 = r24 + 0x1;


    }

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
L_80246360:
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
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10f;
    fn_80239CCC();
L_802463CC:
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xdc;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
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
L_8024659C:
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
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xde;
    fn_80239CCC();
L_80246608:
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc4;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
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
L_802467BC:
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
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc6;
    fn_80239CCC();
L_80246828:
    r24 = r27;
    r3 = r24;
    return;
    r4 = r31;
    r3 = 0x0;
    r5 = 0xbf;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc0;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc1;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
L_802469A8:
    r3 = r27;
    r4 = r31;
    r5 = 0xc2;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r31;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc2;
    fn_80239EE8();
L_802469EC:
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc8;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xc9;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
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
L_80246C00:
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
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xcb;
    fn_80239CCC();
L_80246C6C:
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
    fn_802399FC();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0xe4;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 != (u32)0x1) {
        r3 = r31;
        r4 = r28;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
        fn_802399FC();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
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
    fn_8024AFC4();
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
    fn_802399FC();
    r25 = r3;
    r3 = r29;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r25;
        r4 = r31;
        r5 = 0x105;
        fn_80239984();
        r25 = r3;
        r3 = r29;
        fn_80205B8C();
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
L_80246FC0:
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
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x106;
    fn_80239CCC();
L_8024702C:
    r24 = r25;

    r3 = r24;
    return;
}

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
        if ((u32)r0 >= (u32)0x6) break;
        r5 = r5 + 0x1;
        *(u32*)(r3 + r0) = r4;


    }
    r4 = r16;
    r3 = (u32)sp + 0x110;
    fn_801FCEC4();
    r3 = r16;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r16;
        r4 = 0xe2;
        r5 = 0x0;
        fn_80204DE4();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
    fn_801F1C18();
    r4 = r15;
    r5 = (u32)sp + 0xb0;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1A6C();
    r0 = r20 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = -0x1;
        return;
    }
    r4 = r21;
    r3 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r17 != (u32)0x0) {
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
            return;
    }
    }
    r0 = r14 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8024EA84;
    r14 = (u32)sp + 0x98;
    r17 = (u32)sp + 0x80;
    r18 = (u32)sp + 0x18;
    r19 = 0x0;
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFFFF;
        if ((u32)r0 >= (u32)0xe) break;
        r4 = 0x0;
        r0 = r3 + 0x2;
        r23 = *(s16*)(r18 + r3);
        r24 = *(s16*)(r18 + r0);
        r3 = r4;
        while (1) {
            r0 = r4 & 0xFFFF;
            if ((u32)r0 >= (u32)0x6) break;
            r4 = r4 + 0x1;
            *(u32*)(r17 + r0) = r3;


        }
        r22 = 0x0;
        r25 = r22;
        while (1) {
            r0 = r25 & 0xFFFF;
            if ((u32)r0 >= (u32)0x6) break;
            r19 = *(u32*)(r14 + r0);
            if ((u32)r19 == (u32)0x0) goto L_8024E9B4;
            r0 = (s16)r23;
            if ((u32)r19 >= (u32)0x0) {
                r3 = r15;
                r4 = r19;
                fn_80238600();
                r0 = r3 & 0xFF;
                if ((s32)r23 != (s32)r0) goto L_8024E9B4;
            }
            r0 = (s16)r24;
            if ((s32)r23 >= (s32)r0) {
                r3 = r15;
                r4 = r19;
                fn_80238538();
                r0 = r3 & 0xFF;
                if ((s32)r24 != (s32)r0) goto L_8024E9B4;
            }
            r3 = (u32)sp + 0x80;
            *(u32*)(r3 + r0) = r19;
            r22 = r22 + 0x1;
        L_8024E9B4:
            r25 = r25 + 0x1;


        }
        r0 = r22 & 0xFFFF;
        r19 = r22;
        if ((u32)r0 != (u32)0x6) break;
        r26 = r26 + 0x2;


    }

    r0 = r19 & 0xFFFF;
    if ((u32)r0 == (u32)0xe) goto L_8024EA84;
    fn_800E0C54();
    r5 = r3 & 0xFFFF;
    r4 = r19 & 0xFFFF;
    r0 = (s32)r5 / (s32)r4;
    r3 = (u32)sp + 0x80;
    r0 = r0 * r4;
    r0 = r5 - r0;
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
    return;
L_8024EA84:
    r4 = (0xffff << 16);
    r3 = (0x1 << 16);
    r18 = r4 + 0x1;
    r22 = (u32)sp + 0x98;
    r17 = r20 & 0xFFFF;
    r19 = 0x0;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if ((u32)r0 >= (u32)r17) break;
        r24 = *(u32*)(r22 + r0);
        if ((u32)r24 != (u32)0x0) {
            r3 = r24;
            r4 = 0x0;
            r5 = 0xce;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            r0 = (s16)r3;
            if ((u32)r24 >= (u32)0x0) {
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
                if ((u32)r19 < (u32)r25) {
                    r19 = r25;
                }
                if ((s32)r18 < (s32)r24) {
                    r18 = r24;
                }
                r0 = r14 & 0xFFFF;
                if ((u32)r0 > (u32)r3) {
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
    if ((u32)r0 == (u32)0x1) {
        r22 = (u32)sp + 0x98;
        r17 = r20 & 0xFFFF;
        r23 = 0x0;
        while (1) {
            r0 = r23 & 0xFFFF;
            if ((u32)r0 >= (u32)r17) break;
            r24 = *(u32*)(r22 + r0);
            if ((u32)r24 != (u32)0x0) {
                r3 = r24;
                r4 = 0x0;
                r5 = 0xce;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                r0 = (s16)r3;
                if ((u32)r24 >= (u32)0x0) {
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
                    if ((u32)r0 >= (u32)r3) {
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
        if ((u32)r3 >= (u32)r0) break;
        r3 = (u32)sp + 0x98;
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
            if ((u32)r0 >= (u32)r17) break;
            r29 = *(u32*)(r31 + r0);
            if ((u32)r29 != (u32)0x0) {
                r22 = r23 & 0xFFFF;
                r24 = 0x0;
                while (1) {
                    r0 = r24 & 0xFFFF;
                    if ((u32)r0 >= (u32)r22) break;
                    r3 = (u32)sp + 0x54;
                    r5 = *(u16*)(r3 + r0);
                    if ((u32)r5 == (u32)0x0 || (u32)r5 == (u32)0x165) goto L_8024ED60;

                    r3 = r15;
                    r4 = r16;
                    r6 = r29;
                    fn_8023C530();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x1) goto L_8024ED60;
                    r14 = 0x1;
                    break;
                L_8024ED60:
                    r24 = r24 + 0x1;


                }

                r0 = r14 & 0xFF;
                if ((u32)r0 == (u32)0x1) break;
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
        fn_80205BE8();
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
        if ((u32)r0 == (u32)0x1) {
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = 0x1a;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
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
        if ((u32)r0 == (u32)0x2) {
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = 0x1b;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
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
        if ((u32)r0 == (u32)0x3) {
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = 0x1c;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
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
            if ((u32)r0 >= (u32)0x2) break;
            r4 = r21;
            r6 = r17;
            r3 = 0x0;
            r5 = 0x39;
            fn_801FB1C0();
            r5 = r3 & 0xFFFF;
            if ((u32)r5 != (u32)0x9) {
                r3 = r15;
                r4 = r27;
                fn_80238E30();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 == (u32)0x1) {
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = 0x1e;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
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
        if ((u32)r0 == (u32)0x1) {
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = 0x1f;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
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
        if ((u32)r19 <= (u32)r3) {
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = 0x21;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
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
            fn_80205BE8();
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
            if ((u32)r0 >= (u32)r14) break;
            r3 = (u32)sp + 0x34;
            r23 = *(u32*)(r3 + r0);
            if ((u32)r23 != (u32)0x0) {
                r0 = *(u32*)(sp + 0x804);
                r25 = 0x0;
                r31 = r0 & 0xFFFF;
                while (1) {
                    r0 = r25 & 0xFFFF;
                    if ((u32)r0 >= (u32)r31) break;
                    r3 = (u32)sp + 0x54;
                    r22 = *(u16*)(r3 + r0);
                    if ((u32)r22 != (u32)0x0) {
                        r3 = r15;
                        r4 = r22;
                        r5 = r16;
                        fn_802395C8();
                        r0 = r3 & 0xFFFF;
                        r17 = r3;
                        if ((u32)r0 != (u32)0x9) {
                            r3 = r15;
                            r4 = r22;
                            r5 = 0x1;
                            fn_8023943C();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 != (u32)0x9) {
                                r3 = r15;
                                r4 = r22;
                                fn_80239500();
                                r6 = r3;
                                r3 = r15;
                                r4 = r23;
                                r5 = r17;
                                fn_8023793C();
                                r0 = r3 & 0xFFFF;
                                if ((u32)r0 == (u32)0x41) {
                                    r3 = *(u32*)(r29 + r30);
                                    r4 = r15;
                                    r5 = 0x22;
                                    fn_80239984();
                                    *(u32*)(r29 + r30) = r3;
                                    r3 = r27;
                                    fn_80205BE8();
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
            if ((u32)r0 >= (u32)r31) break;
            r3 = (u32)sp + 0x34;
            r23 = *(u32*)(r3 + r0);
            if ((u32)r23 != (u32)0x0) {
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
                    if ((u32)r0 >= (u32)r14) break;
                    r3 = (u32)sp + 0x54;
                    r24 = *(u16*)(r3 + r0);
                    if ((u32)r24 != (u32)0x0) {
                        r3 = r15;
                        r4 = r24;
                        r5 = r23;
                        fn_802395C8();
                        r0 = r3 & 0xFFFF;
                        r25 = r3;
                        if ((u32)r0 != (u32)0x9) {
                            r3 = r15;
                            r4 = r24;
                            r5 = 0x1;
                            fn_8023943C();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 != (u32)0x9) {
                                r3 = r15;
                                r4 = r24;
                                fn_80239500();
                                r6 = r3;
                                r3 = r15;
                                r4 = r27;
                                r5 = r25;
                                fn_80238B0C();
                                r0 = r3 & 0xFFFF;
                                if ((u32)r0 == (u32)0x41) {
                                    r3 = *(u32*)(r29 + r30);
                                    r4 = r15;
                                    r5 = 0x23;
                                    fn_80239984();
                                    *(u32*)(r29 + r30) = r3;
                                    r3 = r27;
                                    fn_80205BE8();
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
        if ((u32)r0 == (u32)0x1) {
            r17 = 0x0;
            while (1) {
                r0 = *(u32*)lbl_80478B38;
                r3 = r17 & 0xFFFF;
                if ((u32)r3 >= (u32)r0) break;
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
                r14 = (u32)sp + 0xb0;
                r22 = 0x0;
                r23 = r0 & 0xFFFF;
                while (1) {
                    r0 = r22 & 0xFFFF;
                    if ((u32)r0 >= (u32)r23) break;
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

                    if ((u32)r0 != (u32)0x2 && (u32)r0 != (u32)0x3) goto L_8024F454;

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
                    r4 = r26;
                    r6 = 0x0;
                    r7 = 0x0;
                    r8 = 0x0;
                    r9 = 0x0;
                    r10 = 0x24;
                    fn_80239EE8();
                L_8024F454:
                    r22 = r22 + 0x1;


                }
            L_8024F464:
                r17 = r17 + 0x1;



            }
        }
        r3 = r15;
        r4 = r27;
        fn_8024FE80();
        r0 = r3 & 0xFFFF;
        r14 = r3;
        if ((u32)r3 != (u32)r0) {
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = r14;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
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
        if ((u32)r0 == (u32)0x2) {
            r3 = r15;
            r4 = r27;
            r5 = 0x21;
            fn_80239058();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r15;
                r4 = r27;
                r5 = 0x2c;
                fn_80239058();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x1) goto L_8024F710;
            }
            r3 = *(u32*)(r29 + r30);
            r4 = r15;
            r5 = 0x29;
            fn_80239984();
            *(u32*)(r29 + r30) = r3;
            r3 = r27;
            fn_80205BE8();
            r7 = (0x1 << 16);
            r5 = r3;
            r4 = r26;
            r6 = 0x0;
            r7 = 0x0;
            r8 = 0x0;
            r9 = 0x0;
            r10 = 0x29;
            fn_80239EE8();
            goto L_8024F710;
        }
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r26;
            r6 = 0x0;
            r7 = 0x0;
            r8 = 0x0;
            r9 = 0x0;
            r10 = 0x2a;
            fn_80239EE8();
            goto L_8024F710;
        }
        if ((u32)r0 == (u32)0x3) {
            r3 = r15;
            r4 = r27;
            r14 = 0x0;
            r5 = 0x8;
            fn_80239058();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r14 = 0x1;
            }
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
        L_8024F650:
            r14 = 0x1;
        L_8024F654:
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
            r4 = r26;
            r6 = 0x0;
            r7 = 0x0;
            r8 = 0x0;
            r9 = 0x0;
            r10 = 0x2b;
            fn_80239EE8();
            goto L_8024F710;
        }
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
        r4 = r26;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x2c;
        fn_80239EE8();
    L_8024F710:
        fn_8000815C();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r8 = 0x0;
            r9 = 0x0;
            r10 = 0x0;
            fn_8023A118();
        }
        r3 = r27;
        fn_80205BE8();
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
    L_8024F7DC:
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
    if ((u32)r15 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r4 = r15;
    r3 = 0x0;
    fn_801F4460();
    r16 = r3;
    r3 = r15;
    fn_80205BE8();
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
    ((void(*)(void))fn_8012640C)();
    r3 = (s16)r3;

    return;
}

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
/* fn_8025A290 | size: 0xB0 */
u32 fn_8025A290(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType) {
    extern u16 fn_8023793C();
    extern u32 fn_80239500();
    extern u32 fn_802395C8();
    extern u32 fn_8025C264();
    u32 statusVal;
    u32 resultVal;
    u32 slotData;
    statusVal = fn_802395C8(trainerCtx, resultSlot, trainerSlot);
    resultVal = fn_8025C264(trainerCtx, trainerSlot, resultSlot, resultType, 0);
    slotData = fn_80239500(trainerCtx, resultSlot);
    if (fn_8023793C(trainerCtx, resultType, statusVal, slotData) == 0x43) {
        resultVal = 0;
    }
    return (resultVal != 0) ? 1 : 0;
}

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
    u32 r6 = param3;

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
    if ((s32)r0 != (s32)0) {
        r4 = r29;
        r5 = r31;
        r24 = 0x100;
        r3 = 0x2;
        fn_801F02AC();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_80263228;
        }
        r4 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fn_801F76B8();
        r26 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r31;
        fn_801F0134();
        r0 = r3 & 0xFFFF;
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_80263228;
        }
        fn_801F0234();
        fn_801F0204();
        if ((s32)r3 < (s32)0x0) {
            r3 = 0x0;
            goto L_80263228;
        }
        r4 = r26;
        r6 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x2;
        fn_801F76B8();
    L_80263228:
        if ((s32)r3 != (s32)0xf3) {
            if ((s32)r3 < (s32)0xf3) {
                if ((s32)r3 != (s32)0xf1) {
                    if ((s32)r3 < (s32)0xf1) {
                        goto L_8026326C;
                    }
                    if ((s32)r3 >= (s32)0xf5) goto L_8026326C;
                    goto L_80263268;
                    }
                r24 = 0x100;
                goto L_8026326C;
                    }
            r24 = 0x101;
            goto L_8026326C;
        }
        r24 = 0x102;
        goto L_8026326C;
    L_80263268:
        r24 = 0x103;
    L_8026326C:
        r3 = r24;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        fn_801026A4();
    }
    r30 = 0x0;
    r26 = 0x0;
    goto L_802639AC;
L_8026329C:
    r3 = r29;
    r4 = r30;
    fn_801F981C();
    if ((s32)r3 == (s32)0xf5) {
        r26 = r30;
        goto L_802639A8;
    }
    r4 = 0x1;
    fn_80205C24();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0xf5) {
        r26 = r30;
        goto L_802639A8;
    }
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802632EC;
L_802632E0:
    r3 = r29;
    fn_801F9790();
    goto L_802639B8;
L_802632EC:
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263330;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80263330;
L_80263310:
    r3 = r28;
    fn_80207760();
    r3 = r29;
    r4 = r28;
    r5 = r31;
    fn_801F9130();
    r26 = r30;
    goto L_802639A8;
L_80263330:
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r4 = r28;
        r5 = r31;
        r3 = 0x2;
        fn_801F02AC();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_802633C0;
        }
        r4 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fn_801F76B8();
        r24 = r3 & 0xFFFF;
        r3 = r28;
        r4 = r31;
        fn_801F0134();
        r0 = r3 & 0xFFFF;
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_802633C0;
        }
        fn_801F0234();
        fn_801F0204();
        if ((s32)r3 < (s32)0x0) {
            r3 = 0x0;
            goto L_802633C0;
        }
        r4 = r24;
        r6 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x3;
        fn_801F76B8();
    L_802633C0:
        r4 = 0x1;
        fn_80011D9C();
    }
    r0 = 0x0;
    *(u32*)(sp + 0x8) = r0;
L_802633D0:
    r3 = r28;
    fn_80207760();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        fn_801F18DC();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r4 = r28;
            r5 = r31;
            r3 = 0x2;
            fn_801F02AC();
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                goto L_802634A8;
            }
            r4 = 0x0;
            r5 = 0x5;
            r6 = 0x0;
            fn_801F76B8();
            r24 = r3 & 0xFFFF;
            r3 = r28;
            r4 = r31;
            fn_801F0134();
            r0 = r3 & 0xFFFF;
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                goto L_802634A8;
            }
            fn_801F0234();
            fn_801F0204();
            if ((s32)r3 < (s32)0x0) {
                r3 = 0x0;
                goto L_802634A8;
            }
            r4 = r24;
            r6 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x3;
            fn_801F76B8();
        L_802634A8:
            r4 = 0x0;
            fn_80011D9C();
        }
        r3 = 0x1;
        fn_800119A8();
        goto L_802632E0;
    }
    r3 = 0x0;
    fn_801F1700();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8026358C;
    fn_80265924();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1 || (s32)r24 >= (s32)0x0) goto L_8026358C;

    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r4 = r28;
        r5 = r31;
        r3 = 0x2;
        fn_801F02AC();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_80263578;
        }
        r4 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fn_801F76B8();
        r24 = r3 & 0xFFFF;
        r3 = r28;
        r4 = r31;
        fn_801F0134();
        r0 = r3 & 0xFFFF;
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_80263578;
        }
        fn_801F0234();
        fn_801F0204();
        if ((s32)r3 < (s32)0x0) {
            r3 = 0x0;
            goto L_80263578;
        }
        r4 = r24;
        r6 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x3;
        fn_801F76B8();
    L_80263578:
        r4 = 0x0;
        fn_80011D9C();
    }
    r3 = 0x1;
    fn_800119A8();
    goto L_80263310;
L_8026358C:
    if ((s32)r24 >= (s32)0x0) {
        r3 = 0x1;
        fn_800119A8();
    }
    r3 = r29;
    r4 = r28;
    r5 = r31;
    r6 = r24;
    fn_80263BC8();
    r24 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        fn_801F18DC();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r4 = r28;
            r5 = r31;
            r3 = 0x2;
            fn_801F02AC();
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                goto L_80263654;
            }
            r4 = 0x0;
            r5 = 0x5;
            r6 = 0x0;
            fn_801F76B8();
            r24 = r3 & 0xFFFF;
            r3 = r28;
            r4 = r31;
            fn_801F0134();
            r0 = r3 & 0xFFFF;
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                goto L_80263654;
            }
            fn_801F0234();
            fn_801F0204();
            if ((s32)r3 < (s32)0x0) {
                r3 = 0x0;
                goto L_80263654;
            }
            r4 = r24;
            r6 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x3;
            fn_801F76B8();
        L_80263654:
            r4 = 0x0;
            fn_80011D9C();
        }
        r3 = 0x1;
        fn_800119A8();
        goto L_802632E0;
    }
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
    if ((u32)r0 != (u32)0x1) {
        r4 = r28;
        r5 = r31;
        r3 = 0x2;
        fn_801F02AC();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_80263724;
        }
        r4 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fn_801F76B8();
        r24 = r3 & 0xFFFF;
        r3 = r28;
        r4 = r31;
        fn_801F0134();
        r0 = r3 & 0xFFFF;
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_80263724;
        }
        fn_801F0234();
        fn_801F0204();
        if ((s32)r3 < (s32)0x0) {
            r3 = 0x0;
            goto L_80263724;
        }
        r4 = r24;
        r6 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x3;
        fn_801F76B8();
    L_80263724:
        r4 = 0x0;
        fn_80011D9C();
    }
    r3 = 0x1;
    fn_800119A8();
    goto L_80263310;
L_80263738:
    r0 = r24 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r29;
        fn_801F7E60();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) goto L_802633D0;
        r3 = r30 & 0xFFFF;
        if ((u32)r0 != (u32)0x1) {
            r30 = r26;
            r3 = 0x0;
            fn_801F18DC();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r4 = r28;
                r5 = r31;
                r3 = 0x2;
                fn_801F02AC();
                if ((u32)r3 == (u32)0x0) {
                    r3 = 0x0;
                    goto L_802637F4;
                }
                r4 = 0x0;
                r5 = 0x5;
                r6 = 0x0;
                fn_801F76B8();
                r24 = r3 & 0xFFFF;
                r3 = r28;
                r4 = r31;
                fn_801F0134();
                r0 = r3 & 0xFFFF;
                if ((u32)r3 == (u32)0x0) {
                    r3 = 0x0;
                    goto L_802637F4;
                }
                fn_801F0234();
                fn_801F0204();
                if ((s32)r3 < (s32)0x0) {
                    r3 = 0x0;
                    goto L_802637F4;
                }
                r4 = r24;
                r6 = r3 & 0xFFFF;
                r3 = 0x0;
                r5 = 0x3;
                fn_801F76B8();
            L_802637F4:
                r4 = 0x0;
                fn_80011D9C();
            }
            r3 = 0x1;
            fn_800119A8();
            goto L_8026329C;
        }
        r0 = r25 & 0xFF;
        if ((u32)r0 != (u32)0x1 || (u32)r3 != (u32)0x0) goto L_802633D0;

        r3 = 0x0;
        fn_801F18DC();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r4 = r28;
            r5 = r31;
            r3 = 0x2;
            fn_801F02AC();
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                goto L_802638AC;
            }
            r4 = 0x0;
            r5 = 0x5;
            r6 = 0x0;
            fn_801F76B8();
            r24 = r3 & 0xFFFF;
            r3 = r28;
            r4 = r31;
            fn_801F0134();
            r0 = r3 & 0xFFFF;
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                goto L_802638AC;
            }
            fn_801F0234();
            fn_801F0204();
            if ((s32)r3 < (s32)0x0) {
                r3 = 0x0;
                goto L_802638AC;
            }
            r4 = r24;
            r6 = r3 & 0xFFFF;
            r3 = 0x0;
            r5 = 0x3;
            fn_801F76B8();
        L_802638AC:
            r4 = 0x0;
            fn_80011D9C();
        }
        r3 = 0x1;
        fn_800119A8();
        r24 = 0x0;
        while (1) {
            r0 = r24 & 0xFFFF;
            if ((u32)r0 >= (u32)r27) break;
            r3 = r29;
            r4 = r24;
            fn_801F981C();
            if ((u32)r3 != (u32)0x0) {
                r4 = 0x0;
                r5 = 0x120;
                r6 = 0x0;
                r7 = 0x0;
                ((void(*)(void))fn_801254B4)();
            }
            r24 = r24 + 0x1;


        }
        r3 = 0x0;
        return;
    }
    if ((u32)r0 == (u32)0x2) goto L_802633D0;
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r4 = r28;
        r5 = r31;
        r3 = 0x2;
        fn_801F02AC();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_8026399C;
        }
        r4 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fn_801F76B8();
        r24 = r3 & 0xFFFF;
        r3 = r28;
        r4 = r31;
        fn_801F0134();
        r0 = r3 & 0xFFFF;
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            goto L_8026399C;
        }
        fn_801F0234();
        fn_801F0204();
        if ((s32)r3 < (s32)0x0) {
            r3 = 0x0;
            goto L_8026399C;
        }
        r4 = r24;
        r6 = r3 & 0xFFFF;
        r3 = 0x0;
        r5 = 0x3;
        fn_801F76B8();
    L_8026399C:
        r4 = 0x0;
        fn_80011D9C();
    }
    r26 = r30;
L_802639A8:
    r30 = r30 + 0x1;
L_802639AC:
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_8026329C;
L_802639B8:
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r27) goto L_80263B70;
    r4 = r29;
    r5 = r31;
    r24 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_80263A48;
    }
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r25 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_80263A48;
    }
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 < (s32)0x0) {
        r3 = 0x0;
        goto L_80263A48;
    }
    r4 = r25;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80263A48:
    if ((s32)r3 != (s32)0xf3) {
        if ((s32)r3 < (s32)0xf3) {
            if ((s32)r3 != (s32)0xf1) {
                if ((s32)r3 < (s32)0xf1) {
                    goto L_80263A8C;
                }
                if ((s32)r3 >= (s32)0xf5) goto L_80263A8C;
                goto L_80263A88;
                }
            r24 = 0x100;
            goto L_80263A8C;
                }
        r24 = 0x101;
        goto L_80263A8C;
    }
    r24 = 0x102;
    goto L_80263A8C;
L_80263A88:
    r24 = 0x103;
L_80263A8C:
    r3 = r24;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((s32)r3 == (s32)0xf5) goto L_80263B70;
    r4 = r29;
    r5 = r31;
    r24 = 0x100;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_80263B1C;
    }
    r4 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r25 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r31;
    fn_801F0134();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        goto L_80263B1C;
    }
    fn_801F0234();
    fn_801F0204();
    if ((s32)r3 < (s32)0x0) {
        r3 = 0x0;
        goto L_80263B1C;
    }
    r4 = r25;
    r6 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    fn_801F76B8();
L_80263B1C:
    if ((s32)r3 != (s32)0xf3) {
        if ((s32)r3 < (s32)0xf3) {
            if ((s32)r3 != (s32)0xf1) {
                if ((s32)r3 < (s32)0xf1) {
                    goto L_80263B60;
                }
                if ((s32)r3 >= (s32)0xf5) goto L_80263B60;
                goto L_80263B5C;
                }
            r24 = 0x100;
            goto L_80263B60;
                }
        r24 = 0x101;
        goto L_80263B60;
    }
    r24 = 0x102;
    goto L_80263B60;
L_80263B5C:
    r24 = 0x103;
L_80263B60:
    r3 = r24;
    r4 = 0x0;
    r5 = 0x1;
    fn_80102568();
L_80263B70:
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r27) break;
        r3 = r29;
        r4 = r24;
        fn_801F981C();
        if ((u32)r3 != (u32)0x0) {
            r4 = 0x0;
            r5 = 0x120;
            r6 = 0x0;
            r7 = 0x0;
            ((void(*)(void))fn_801254B4)();
        }
        r24 = r24 + 0x1;


    }
    r3 = 0x1;

    return;
}

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
u32 fn_802400D8(void* ctx, u32 slot, u16 species, u32 extra) {
    extern u32 fn_8023CA9C();
    extern u16 fn_8025CB3C();
    u16 currentSpecies;
    currentSpecies = fn_8025CB3C(ctx);
    if (currentSpecies == species || currentSpecies == 0) {
        return 0;
    }
    return fn_8023CA9C(ctx, slot, currentSpecies, extra);
}

/* Address: 0x80240144 | Size: 0xAC */
void fn_80240144(void* ctx, u32 param1, u32 param2) {
    extern void fn_800E0C54();
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_802399FC();
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
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
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
    fn_801F1C18();
    r31 = r3;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 >= (u32)0x2) {
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
        if ((u32)r0 >= (u32)r25) break;
        r4 = *(u32*)(r26 + r0);
        if ((u32)r28 == (u32)r4) goto L_8024031C;
        r3 = r27;
        fn_8023831C();
        r0 = r3 & 0xFFFF;

        if ((u32)r0 != (u32)0x8 && (u32)r0 != (u32)0x9) goto L_8024031C;

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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c8;
        fn_80239EE8();
        break;
    L_8024031C:
        r24 = r24 + 0x1;


    }

    r25 = (u32)sp + 0x8;
    r26 = r31 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r26) break;
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c9;
        fn_80239EE8();
        break;
    L_802403B0:
        r24 = r24 + 0x1;


    }

    r4 = (u32)sp + 0x8;
    r0 = r31 & 0xFFFF;
    r5 = 0x0;
    while (1) {
        r3 = r5 & 0xFFFF;
        if ((u32)r3 >= (u32)r0) break;
        r3 = *(u32*)(r4 + r3);
        if ((u32)r28 != (u32)r3) {
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
void fn_80240454(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6D9C();
    extern void fn_801F6E98();
    extern void fn_80205B8C();
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
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        r4 = 0x4a;
        fn_801F6D9C();
    } else {

        r3 = 0x0;
    }
    r0 = (s16)r3;
    if ((u32)r0 == (u32)0x1) {
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
        fn_80205B8C();
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
    fn_80205B8C();
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
void fn_802405C0(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80236B98();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    fn_80236B98();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1c3;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024064C | Size: 0x13C (316 bytes) */
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
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) { r3 = r31; return; }
    if ((u32)r0 == (u32)0xffff) { r3 = r31; return; }
    if ((u32)r0 == (u32)0x165) { r3 = r31; return; }
    if ((u32)r0 == (u32)0x163) { r3 = r31; return; }
    r4 = r3;
    r3 = 0x0;
    r5 = 0x1c;
    r6 = 0x0;
    fn_8011BEB4();
    if ((u32)r3 == (u32)0x0) {
        r3 = (u32)fn_8024E52C;
        r3 = (u32)fn_8024E52C;
    }
    r4 = (u32)fn_8024349C;
    r0 = (u32)fn_8024349C;
    if ((u32)r3 != (u32)r0) {
        r4 = (u32)fn_80243390;
        r0 = (u32)fn_80243390;
        if ((u32)r3 != (u32)r0) {
            r4 = (u32)fn_80243284;
            r0 = (u32)fn_80243284;
            if ((u32)r3 != (u32)r0) {
                r4 = (u32)fn_80243178;
                r0 = (u32)fn_80243178;
                if ((u32)r3 != (u32)r0) {
                    r4 = (u32)fn_802430E4;
                    r0 = (u32)fn_802430E4;
                    if ((u32)r3 != (u32)r0) {
                        r4 = (u32)fn_80242FEC;
                        r0 = (u32)fn_80242FEC;
                        if ((u32)r3 != (u32)r0) {
                            r4 = (u32)fn_80242E4C;
                            r0 = (u32)fn_80242E4C;
                            if ((u32)r3 != (u32)r0) { r3 = r31; return; }
    }
    }
    }
    }
    }
    }
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
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1c2;
    fn_80239EE8();

    r3 = r31;
    return;
}

/* Address: 0x80240788 | Size: 0x448 (1096 bytes) */
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
        if ((u32)r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if ((u32)r0 >= (u32)0x8 || (u32)r0 > (u32)0x9) {

            r0 = 0x1;
            goto L_80240858;
        }
        r4 = r4 + 0x1;


    }
    r0 = 0x0;
L_80240858:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if ((u32)r0 >= (u32)0xa || (u32)r0 > (u32)0xc) {

            r0 = 0x1;
            goto L_8024095C;
        }
        r4 = r4 + 0x1;


    }
    r0 = 0x0;
L_8024095C:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if ((u32)r0 >= (u32)0x3 || (u32)r0 > (u32)0x4) {

            r0 = 0x1;
            goto L_80240A60;
        }
        r4 = r4 + 0x1;


    }
    r0 = 0x0;
L_80240A60:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if ((u32)r0 >= (u32)0x0 || (u32)r0 > (u32)0x2) {

            r0 = 0x1;
            goto L_80240B64;
        }
        r4 = r4 + 0x1;


    }
    r0 = 0x0;
L_80240B64:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    fn_801F1C18();
    r26 = r3;
    r21 = 0x0;
    r22 = 0x0;
    while (1) {
        r3 = *(u32*)lbl_80478DF8;
        r4 = r22 & 0xFFFF;
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((u32)r4 >= (u32)r0) break;
        r0 = r22 & 0xFFFF;
        if ((s32)r0 != (s32)0) {
            if ((u32)r0 != (u32)0x165) {
                if ((u32)r0 != (u32)0x163) {
                    r3 = r28;
                    r4 = r22;
                    r5 = 0x1;
                    fn_8023943C();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x163) {
                        r4 = r28;
                        r7 = r22;
                        r3 = 0x0;
                        r5 = 0x1;
                        r6 = 0x1;
                        r8 = 0x0;
                        fn_801F1990();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x1) {
                            r21 = 0x1;
        }
        }
        }
        }
        }
        r22 = r22 + 0x1;




    }
    r0 = r21 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)r22) break;
        r4 = *(u32*)(r27 + r0);
        if ((u32)r29 == (u32)r4) goto L_80241830;
        r3 = r28;
        r5 = (u32)sp + 0x8;
        r6 = 0x0;
        r7 = 0x1;
        fn_802367CC();
        r25 = r3 & 0xFFFF;
        if ((u32)r29 == (u32)r4) goto L_80241830;
        r23 = (u32)sp + 0x8;
        r21 = 0x0;
        while (1) {
            r0 = r21 & 0xFFFF;
            if ((u32)r0 >= (u32)r25) break;
            r3 = r28;
            r4 = *(u16*)(r23 + r0);
            r5 = 0x1;
            fn_8023943C();
            r0 = r3 & 0xFF;
            if ((u32)r29 != (u32)r4) {
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
    L_80241830:
        r24 = r24 + 0x1;


    }
    r27 = (u32)sp + 0x1c;
    r23 = r26 & 0xFFFF;
    r25 = 0x1;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
        r3 = *(u32*)(r27 + r22);
        if ((u32)r29 == (u32)r3) goto L_802418D0;
        r4 = 0x0;
        r5 = 0xfe;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r29 == (u32)r3) goto L_802418D0;
        fn_801F1170();
        r0 = r3 & 0xFF;
        if ((u32)r29 == (u32)r3) goto L_802418D0;
        r3 = r21;
        fn_801F0898();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x13) {
            r25 = 0x0;
            break;
        }
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
        break;
    L_802418D0:
        r24 = r24 + 0x1;


    }

    r0 = r25 & 0xFF;
    if ((u32)r0 == (u32)r23) {
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
        if ((u32)r0 >= (u32)r24) break;
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b3;
        fn_80239EE8();
        break;
    L_802419B4:
        r22 = r22 + 0x1;


    }

    r25 = (u32)sp + 0x1c;
    r24 = r26 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r24) break;
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b4;
        fn_80239EE8();
        break;
    L_80241A48:
        r22 = r22 + 0x1;


    }

    r27 = (u32)sp + 0x1c;
    r24 = r26 & 0xFFFF;
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if ((u32)r0 >= (u32)r24) break;
        r23 = *(u32*)(r27 + r0);
        if ((u32)r29 == (u32)r23) goto L_80241B48;
        r3 = r28;
        r4 = r23;
        r5 = 0x8;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r29 == (u32)r23) {
            r0 = -0x1;
            goto L_80241AF4;
        }
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
    L_80241AF4:
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1b5;
        fn_80239EE8();
        r3 = r31;
        return;
    L_80241B48:
        r25 = r25 + 0x1;


    }

    r3 = r31;
    return;
}

/* Address: 0x80241B70 | Size: 0x278 (632 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = 0x4a;
        fn_801F6D9C();
        r27 = r3;
    } else {

        r27 = 0x0;
    }
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
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    while (1) {
        r0 = r4 & 0xFF;
        if ((u32)r0 >= (u32)0x7) break;
        r0 = r4 & 0xFF;
        r0 = *(u8*)(r3 + r0);
        if ((u32)r0 >= (u32)0x8 || (u32)r0 > (u32)0xc) {

            r0 = 0x1;
            goto L_80241C80;
        }
        r4 = r4 + 0x1;


    }
    r0 = 0x0;
L_80241C80:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1ac;
        fn_80239EE8();
    }
    r0 = (s16)r27;
    if ((s32)r0 == (s32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1ad;
        fn_80239EE8();
        r3 = r31;
        return;
    }
    if ((s32)r0 == (s32)0x2) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1ae;
        fn_80239EE8();
        r3 = r31;
        return;
    }
    if ((s32)r0 != (s32)0x3) { r3 = r31; return; }
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
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1af;
    fn_80239EE8();

    r3 = r31;
    return;
}

/* Address: 0x80241DE8 | Size: 0x1FC (508 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r0 = r23 & 0xFFFF;
    r25 = r3;
    if ((s32)r0 != (s32)0) {
        if ((u32)r0 != (u32)0xffff) {
            if ((u32)r0 != (u32)0x165) {
                if ((u32)r0 != (u32)0x163) {
                    r3 = r26;
                    r4 = r23;
                    r5 = 0x4;
                    fn_8023943C();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) {
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
                        r4 = r26;
                        r8 = r28;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0x1a9;
                        fn_80239EE8();
    }
    }
    }
    }
    }
    r24 = (u32)sp + 0x8;
    r25 = r25 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if ((u32)r0 >= (u32)r25) break;
        r3 = r26;
        r4 = *(u32*)(r24 + r0);
        fn_802377E8();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x12e || (u32)r0 == (u32)0xd4) goto L_80241F10;

        if ((u32)r0 != (u32)0x177) goto L_80241F5C;
    L_80241F10:
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1aa;
        fn_80239EE8();
        break;
    L_80241F5C:
        r23 = r23 + 0x1;


    }

    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x121) {
        r0 = r31 & 0xFFFF;
        if ((u32)r0 != (u32)0x121) { r3 = r29; return; }
    }
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
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x1ab;
    fn_80239EE8();

    r3 = r29;
    return;
}

/* Address: 0x80241FE4 | Size: 0x2A8 (680 bytes) */
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

    r6 = 0x1;
    r7 = 0x1;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r5 = (u32)sp + 0x2c;
    r4 = r29;
    r20 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r22 = r3;
    r4 = r29;
    r5 = (u32)sp + 0xc;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r23 = r3;
    r21 = (u32)sp + 0x2c;
    r26 = r22 & 0xFFFF;
    r15 = (u32)sp + 0x8;
    r14 = (u32)sp + 0xc;
    r28 = r3 & 0xFFFF;
    r19 = 0x0;
    r18 = 0x0;
    while (1) {
        r0 = r18 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r3 = r29;
        r4 = *(u32*)(r14 + r0);
        r5 = (u32)sp + 0x8;
        fn_80237CB8();
        r27 = r3 & 0xFFFF;
        r17 = 0x0;
        while (1) {
            r0 = r17 & 0xFFFF;
            if ((u32)r0 >= (u32)r27) break;
            r16 = 0x0;
            while (1) {
                r0 = r16 & 0xFFFF;
                if ((u32)r0 >= (u32)r26) break;
                r0 = *(u32*)(r21 + r25);
                if ((u32)r30 != (u32)r0) {
                    r3 = r29;
                    r4 = r31;
                    fn_80239500();
                    r4 = *(u32*)(r21 + r25);
                    r6 = r3;
                    r5 = *(u16*)(r15 + r24);
                    r3 = r29;
                    fn_8023793C();
                    r0 = r3 & 0xFFFF;
                    if ((u32)r0 == (u32)0x41) {
                        r19 = 0x1;
                }
                }
                r16 = r16 + 0x1;


            }
            r17 = r17 + 0x1;


        }
        r18 = r18 + 0x1;


    }
    r0 = r19 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a6;
        fn_80239EE8();
    }
    r15 = (u32)sp + 0x2c;
    r14 = r22 & 0xFFFF;
    r16 = 0x0;
    while (1) {
        r0 = r16 & 0xFFFF;
        if ((u32)r0 >= (u32)r14) break;
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a7;
        fn_80239EE8();
        break;
    L_802421CC:
        r16 = r16 + 0x1;


    }

    r15 = (u32)sp + 0xc;
    r14 = r23 & 0xFFFF;
    r16 = 0x0;
    while (1) {
        r0 = r16 & 0xFFFF;
        if ((u32)r0 >= (u32)r14) break;
        r3 = r29;
        r4 = *(u32*)(r15 + r0);
        fn_802377E8();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x12e || (u32)r0 == (u32)0xd4) goto L_80242218;

        if ((u32)r0 != (u32)0x177) goto L_80242264;
    L_80242218:
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a8;
        fn_80239EE8();
        r3 = r20;
        return;
    L_80242264:
        r16 = r16 + 0x1;


    }

    r3 = r20;
    return;
}

/* Address: 0x8024228C | Size: 0x3BC (956 bytes) */
void fn_8024228C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
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

    r29 = r5;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r31 = 0x0;
    r4 = r30;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x19d;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x19e;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x19f;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a0;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a1;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a2;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a3;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a4;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x1a5;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80242648 | Size: 0xE8 (232 bytes) */
void fn_80242648(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236D60();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((s32)r3 > (s32)0x0) {
        r0 = r30 & 0xFFFF;
        if ((s32)r3 != (s32)0x0) {
            if ((u32)r0 != (u32)0xffff) {
                if ((u32)r0 != (u32)0x165) {
                    if ((u32)r0 != (u32)0x163) {
                        r3 = r26;
                        r4 = r30;
                        r5 = 0x1;
                        fn_8023943C();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x1) {
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
                            r4 = r26;
                            r8 = r28;
                            r6 = 0x0;
                            r7 = 0x0;
                            r9 = 0x0;
                            r10 = 0x19c;
                            fn_80239EE8();
    }
    }
    }
    }
    }
    }
    r3 = r31;
    return;
}

/* Address: 0x80242730 | Size: 0x170 (368 bytes) */
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
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    f1 = *(f32*)(u32)lbl_8047E638;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = 0x0;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x19b;
        fn_80239EE8();
        r3 = r31;
        return;
    }
    f1 = *(f32*)(u32)lbl_8047E63C;
    r3 = r28;
    r4 = r29;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x19a;
        fn_80239EE8();
        r3 = r31;
        return;
    }
    f1 = *(f32*)(u32)lbl_8047E630;
    r3 = r28;
    r4 = r29;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
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
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x199;
    fn_80239EE8();

    r3 = r31;
    return;
}

/* Address: 0x802428A0 | Size: 0x134 (308 bytes) */
void fn_802428A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236D60();
    extern void fn_802391E0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r6;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r4 = r29;
    r31 = 0x0;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    r30 = r3;
    if ((s32)r0 == (s32)0) { r3 = r31; return; }
    if ((u32)r0 == (u32)0xffff) { r3 = r31; return; }
    if ((u32)r0 == (u32)0x165) { r3 = r31; return; }
    if ((u32)r0 == (u32)0x163) { r3 = r31; return; }
    r3 = r26;
    r4 = r27;
    r5 = r29;
    fn_80236D60();
    if ((s32)r3 <= (s32)0x0) { r3 = r31; return; }
    r3 = r26;
    r4 = r30;
    fn_802391E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x5) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x197;
        fn_80239EE8();
        r3 = r31;
        return;
    }
    if ((u32)r0 > (u32)0xa) { r3 = r31; return; }
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
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x198;
    fn_80239EE8();

    r3 = r31;
    return;
}

/* Address: 0x802429D4 | Size: 0x17C (380 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r4 > (u32)r0) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x194;
        fn_80239EE8();
    }
    r3 = r27 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    if ((u32)r3 > (u32)r0) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x195;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r25;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x196;
        fn_80239EE8();
    }
    r3 = r26;
    return;
}

/* Address: 0x80242B50 | Size: 0x17C (380 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r4 > (u32)r0) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x191;
        fn_80239EE8();
    }
    r3 = r27 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    if ((u32)r3 > (u32)r0) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x192;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r25;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x193;
        fn_80239EE8();
    }
    r3 = r26;
    return;
}

/* Address: 0x80242CCC | Size: 0xE4 (228 bytes) */
void fn_80242CCC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802364BC();
    extern void fn_80236D60();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((s32)r3 > (s32)0x0) {
        r0 = r30 & 0xFFFF;
        if ((s32)r3 != (s32)0x0) {
            if ((u32)r0 != (u32)0xffff) {
                if ((u32)r0 != (u32)0x165) {
                    if ((u32)r0 != (u32)0x163) {
                        r3 = r26;
                        r4 = r30;
                        r5 = 0x1;
                        fn_8023943C();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x163) {
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
                            r4 = r26;
                            r8 = r28;
                            r6 = 0x0;
                            r7 = 0x0;
                            r9 = 0x0;
                            r10 = 0x190;
                            fn_80239EE8();
    }
    }
    }
    }
    }
    }
    r3 = r31;
    return;
}

/* Address: 0x80242DB0 | Size: 0x9C */
void fn_80242DB0(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8047E630[];
    extern void fn_80205B8C();
    extern void fn_802373B0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;

    f1 = *(f32*)(u32)lbl_8047E630;
    r29 = r4;
    r30 = r5;
    r28 = r3;
    r4 = r6;
    r31 = 0x0;
    r5 = 0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x18f;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80242E4C | Size: 0x1A0 (416 bytes) */
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r4 > (u32)r0) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x18c;
        fn_80239EE8();
    }
    r3 = r25 & 0xFFFF;
    r0 = r26 & 0xFFFF;
    if ((u32)r3 > (u32)r0) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x18d;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x4) {
        r3 = r27;
        r4 = r30;
        fn_80235A3C();
        r0 = r3 & 0xFF;
        if ((u32)r0 <= (u32)0x4) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x18e;
            fn_80239EE8();
    }
    }
    r3 = r31;
    return;
}

/* Address: 0x80242FEC | Size: 0xF8 (248 bytes) */
void fn_80242FEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802357CC();
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

    r30 = r6;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r4 = r30;
    r31 = 0x0;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 >= (u32)0x7) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x18a;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x4) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x18b;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x802430E4 | Size: 0x94 */
void fn_802430E4(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_802358AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x4) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x189;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80243178 | Size: 0x10C (268 bytes) */
void fn_80243178(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235974();
    extern void fn_80236F4C();
    extern void fn_802370AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r4 < (u32)r0) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x187;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    fn_80235974();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x4) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x188;
        fn_80239EE8();
    }
    r3 = r30;
    return;
}

/* Address: 0x80243284 | Size: 0x10C (268 bytes) */
void fn_80243284(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235A3C();
    extern void fn_80236F4C();
    extern void fn_802370AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r4 > (u32)r0) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x185;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x4) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x186;
        fn_80239EE8();
    }
    r3 = r30;
    return;
}

/* Address: 0x80243390 | Size: 0x10C (268 bytes) */
void fn_80243390(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235AA0();
    extern void fn_80236FFC();
    extern void fn_8023715C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r4 > (u32)r0) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x183;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x4) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x184;
        fn_80239EE8();
    }
    r3 = r30;
    return;
}

/* Address: 0x8024349C | Size: 0x138 (312 bytes) */
void fn_8024349C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80236D60();
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

    r7 = 0x1;
    r31 = r3;
    r24 = r4;
    r25 = r5;
    r26 = r6;
    r4 = r31;
    r5 = (u32)sp + 0x8;
    r28 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fn_801F1C18();
    r29 = (u32)sp + 0x8;
    r30 = r3 & 0xFFFF;
    r27 = 0x0;
    while (1) {
        r0 = r27 & 0xFFFF;
        if ((u32)r0 >= (u32)r30) break;
        r3 = r31;
        r5 = *(u32*)(r29 + r0);
        r4 = r26;
        fn_80236D60();
        if ((s32)r3 > (s32)0x0) {
            r28 = 0x1;
            break;
        }
        r27 = r27 + 0x1;


    }

    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r25;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x181;
        fn_80239EE8();
    } else {

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
        r4 = r31;
        r8 = r25;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x182;
        fn_80239EE8();
    }
    r3 = r29;
    return;
}

/* Address: 0x802435D4 | Size: 0xB8 */
void fn_802435D4(void* ctx, u32 param1, u32 param2) {
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_802376EC();
    extern void fn_802399FC();
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x180;
    fn_80239CCC();
    r3 = r31;
    return;
}

/* Address: 0x8024368C | Size: 0x1AC (428 bytes) */
void fn_8024368C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802387C8();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((s32)r0 <= (s32)r3) {
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
        r4 = r30;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x17d;
        fn_80239EE8();
        r3 = r28;
        return;
    }
    r0 = r29 << 1;
    if ((s32)r0 <= (s32)r3) {
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
        r4 = r30;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x17c;
        fn_80239EE8();
        r3 = r28;
        return;
    }
    r0 = r3 * 0x3;
    if ((s32)r29 >= (s32)r0) {
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
        r4 = r30;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x17f;
        fn_80239EE8();
        r3 = r28;
        return;
    }
    r0 = r3 << 1;
    if ((s32)r29 < (s32)r0) { r3 = r28; return; }
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
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x17e;
    fn_80239EE8();

    r3 = r28;
    return;
}

/* Address: 0x80243838 | Size: 0x94 */
void fn_80243838(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r29 = r4;
    r30 = r5;
    r28 = r3;
    r4 = r6;
    r31 = 0x0;
    r5 = 0x17;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x17b;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x802438CC | Size: 0x140 (320 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((s32)r0 == (s32)0) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x179;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r29;
        r4 = 0x1d;
        fn_80201D84();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
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
            r4 = r26;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x17a;
            fn_80239EE8();
    }
    }
    r3 = r30;
    return;
}

/* Address: 0x80243A0C | Size: 0x250 (592 bytes) */
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
    f32 f1 = 0.0f;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r5;
    r27 = r3;
    r28 = r4;
    r30 = 0x0;
    r5 = 0x7;
    fn_80237DBC();
    r31 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243AB0;
    f1 = *(f32*)(u32)lbl_8047E630;
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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x174;
    fn_80239EE8();
    goto L_80243B24;
L_80243AB0:
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80243B24;
    f1 = *(f32*)(u32)lbl_8047E640;
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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x175;
    fn_80239EE8();
L_80243B24:
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r28;
        fn_80235714();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x176;
            fn_80239EE8();
    }
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x177;
        fn_80239EE8();
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r28;
        fn_80235714();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x178;
            fn_80239EE8();
    }
    }
    r3 = r30;
    return;
}

/* Address: 0x80243C5C | Size: 0x7C | Pattern: field_accessor */
u32 fn_80243C5C(void* ctx, u32 slot, u32 param) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

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
    r4 = r29;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x173;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x80243CD8 | Size: 0x640 (1600 bytes) */
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
    fn_801F1C18();
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
        if ((u32)r0 >= (u32)r16) break;
        r3 = r31;
        r4 = r28;
        fn_80239500();
        r6 = r3;
        r5 = *(u16*)(r17 + r0);
        r3 = r31;
        r4 = r30;
        fn_8023793C();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x41) {
            r18 = 0x1;
            break;
        }
        r19 = r19 + 0x1;


    }

    r0 = r18 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x16a;
        fn_80239EE8();
    }
    f1 = *(f32*)(u32)lbl_8047E630;
    r3 = r31;
    r4 = r30;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)r21) break;
        r4 = *(u32*)(r16 + r0);
        if ((u32)r30 == (u32)r4) goto L_80243F78;
        r3 = r31;
        r5 = (u32)sp + 0xc;
        r6 = 0x0;
        r7 = 0x1;
        fn_802367CC();
        r17 = r3 & 0xFFFF;
        r24 = r3;
        if ((u32)r30 == (u32)r4) goto L_80243F78;
        r4 = (u32)fn_8024B474;
        r3 = (u32)fn_8024BFC0;
        r5 = (u32)fn_8024E52C;
        r15 = (u32)sp + 0xc;
        r19 = (u32)fn_8024B474;
        r20 = (u32)fn_8024BFC0;
        r18 = (u32)fn_8024E52C;
        r22 = 0x0;
        while (1) {
            r0 = r22 & 0xFFFF;
            if ((u32)r0 >= (u32)r17) break;
            r3 = 0x0;
            r4 = *(u16*)(r15 + r0);
            r5 = 0x1c;
            r6 = 0x0;
            fn_8011BEB4();
            if ((u32)r3 == (u32)0x0) {
                r3 = r18;
            }

            if ((u32)r3 != (u32)r19 && (u32)r3 != (u32)r20) goto L_80243F58;

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
            r4 = r31;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x16c;
            fn_80239EE8();
            break;
        L_80243F58:
            r22 = r22 + 0x1;


        }

        r3 = r22 & 0xFFFF;
        r0 = r24 & 0xFFFF;
        if ((u32)r3 < (u32)r0) break;
    L_80243F78:
        r23 = r23 + 0x1;


    }

    r3 = (u32)fn_8024B474;
    r15 = 0x0;
    r16 = (u32)fn_8024B474;
    while (1) {
        r3 = *(u32*)lbl_80478DF8;
        r4 = r15 & 0xFFFF;
        r0 = *(u32*)((u8*)r3 + 0x0);
        if ((u32)r4 >= (u32)r0) break;
        r4 = r15;
        r3 = 0x0;
        r5 = 0x1c;
        r6 = 0x0;
        fn_8011BEB4();
        if ((u32)r3 == (u32)0x0) {
            r3 = (u32)fn_8024E52C;
            r3 = (u32)fn_8024E52C;
        }
        if ((u32)r3 != (u32)r16) {
            r4 = (u32)fn_8024BFC0;
            r0 = (u32)fn_8024BFC0;
            if ((u32)r3 != (u32)r0) goto L_80244048;
        }
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
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x16d;
        fn_80239EE8();
        break;
    L_80244048:
        r15 = r15 + 0x1;




    }

    r3 = r31;
    r4 = r30;
    r15 = 0x0;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r3 = r31;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r15 = 0x1;
    }
    r0 = r15 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x16e;
        fn_80239EE8();
    }
    r0 = r27 & 0xFFFF;
    if ((u32)r0 == (u32)0xcb) {
        r3 = r30;
        r4 = 0x0;
        r5 = 0xfc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((s32)r3 != (s32)0x0) {
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
    if (((u32)r0 != (u32)0x11) && ((u32)r0 != (u32)0xf)) {

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x171;
        fn_80239EE8();
    }
    r0 = r25 & 0xFF;
    if ((u32)r0 != (u32)0x4) {
        if ((u32)r0 != (u32)0x3) { r3 = r29; return; }
    }
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
void fn_80244318(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_80236BFC();
    extern void fn_80237DBC();
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

    r29 = r5;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r31 = 0x0;
    r4 = r30;
    r5 = 0x7;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x167;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 >= (u32)0x8) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x168;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x19;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x169;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80244478 | Size: 0x9C */
void fn_80244478(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r6 = 0x0;
    r29 = r4;
    r28 = r3;
    r30 = r5;
    r31 = 0x0;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xed;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x166;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80244514 | Size: 0x18C (396 bytes) */
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

    r7 = 0x1;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r5 = (u32)sp + 0x8;
    r31 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1C18();
    r25 = r3;
    r3 = r27;
    r4 = r30;
    fn_80236520();
    r24 = (u32)sp + 0x8;
    r23 = r3;
    r26 = r25 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r26) break;
        r3 = r27;
        r4 = *(u32*)(r24 + r25);
        fn_8023715C();
        r4 = *(u32*)(r24 + r25);
        r25 = r3;
        r3 = r27;
        fn_80236FFC();
        r4 = r25 & 0xFFFF;
        r0 = r3 & 0xFFFF;
        if ((u32)r4 < (u32)r0) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x164;
            fn_80239EE8();
            break;
        }
        r22 = r22 + 0x1;


    }

    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)r26) {
        if ((u32)r0 != (u32)0xffff) {
            if ((u32)r0 != (u32)0x165) {
                if ((u32)r0 != (u32)0x163) {
                    r3 = r27;
                    r4 = r23;
                    r5 = r30;
                    fn_802395C8();
                    fn_8010C4A0();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x2) {
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
                        r4 = r27;
                        r8 = r29;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0x165;
                        fn_80239EE8();
    }
    }
    }
    }
    }
    r3 = r31;
    return;
}

/* Address: 0x802446A0 | Size: 0x18C (396 bytes) */
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

    r7 = 0x1;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r5 = (u32)sp + 0x8;
    r31 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1C18();
    r25 = r3;
    r3 = r27;
    r4 = r30;
    fn_80236520();
    r24 = (u32)sp + 0x8;
    r23 = r3;
    r26 = r25 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r26) break;
        r3 = r27;
        r4 = *(u32*)(r24 + r25);
        fn_8023715C();
        r4 = *(u32*)(r24 + r25);
        r25 = r3;
        r3 = r27;
        fn_80236FFC();
        r4 = r25 & 0xFFFF;
        r0 = r3 & 0xFFFF;
        if ((u32)r4 > (u32)r0) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x162;
            fn_80239EE8();
            break;
        }
        r22 = r22 + 0x1;


    }

    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)r26) {
        if ((u32)r0 != (u32)0xffff) {
            if ((u32)r0 != (u32)0x165) {
                if ((u32)r0 != (u32)0x163) {
                    r3 = r27;
                    r4 = r23;
                    r5 = r30;
                    fn_802395C8();
                    fn_8010C4A0();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) {
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
                        r4 = r27;
                        r8 = r29;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0x163;
                        fn_80239EE8();
    }
    }
    }
    }
    }
    r3 = r31;
    return;
}

/* Address: 0x8024482C | Size: 0xD4 (212 bytes) */
void fn_8024482C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80205B8C();
    extern void fn_80238748();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r6 = 0x1;
    r7 = 0x1;
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r5 = (u32)sp + 0x8;
    r4 = r25;
    r29 = 0x0;
    r3 = 0x0;
    fn_801F1A6C();
    r30 = (u32)sp + 0x8;
    r31 = r3 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r3 = r25;
        r4 = *(u32*)(r30 + r0);
        fn_80238748();
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0) {
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
            r4 = r25;
            r8 = r27;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x161;
            fn_80239EE8();
            r3 = r29;
            return;
        }
        r28 = r28 + 0x1;


    }

    r3 = r29;
    return;
}

/* Address: 0x80244900 | Size: 0x8C */
void fn_80244900(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x160;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024498C | Size: 0x318 (792 bytes) */
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
    fn_801F1C18();
    r0 = r29 & 0xFF;
    r31 = r3;
    if ((u32)r0 != (u32)0x3) {
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
        if ((u32)r0 >= (u32)r24) break;
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
    L_80244ACC:
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
        r4 = r25;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x15c;
        fn_80239EE8();
        break;
    L_80244B18:
        r21 = r21 + 0x1;


    }

    r24 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r3 = r25;
        r4 = *(u32*)(r24 + r0);
        fn_802377E8();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x12f) {
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
        if ((u32)r0 >= (u32)r30) break;
        r3 = r25;
        r4 = *(u32*)(r31 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x181) {
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
    if ((u32)r0 == (u32)0x3) {
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
    fn_801F1C18();
    r0 = r29 & 0xFF;
    r31 = r3;
    if ((u32)r0 != (u32)0x4) {
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
        if ((u32)r0 >= (u32)r24) break;
        r3 = r25;
        r4 = *(u32*)(r23 + r0);
        r5 = 0xf;
        fn_80238E30();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)r31) break;
        r3 = r25;
        r4 = *(u32*)(r24 + r0);
        fn_802377E8();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x12f) {
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
        if ((u32)r0 >= (u32)r30) break;
        r3 = r25;
        r4 = *(u32*)(r31 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x181) {
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
    if ((u32)r0 == (u32)0x4) {
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
    if ((u32)r0 != (u32)0x1) {
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
        if ((u32)r0 >= (u32)r25) break;
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
    L_80245070:
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x153;
        fn_80239EE8();
        break;
    L_802450BC:
        r22 = r22 + 0x1;


    }

    r25 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r3 = r26;
        r4 = *(u32*)(r25 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x181) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 != (u32)0x2) {
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
        if ((u32)r0 >= (u32)r25) break;
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
    L_802452C8:
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x14f;
        fn_80239EE8();
        break;
    L_80245314:
        r22 = r22 + 0x1;


    }

    r25 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r3 = r26;
        r4 = *(u32*)(r25 + r0);
        fn_80238980();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x181) {
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
    if ((u32)r0 == (u32)0x2) {
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
void fn_80245418(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_802384B4();
    extern void fn_80239984();
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
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r24 = r6;
    r4 = r29;
    r5 = (u32)sp + 0x8;
    r26 = 0x0;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F1A6C();
    r28 = r3;
    r3 = r29;
    r4 = r24;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x14c;
        fn_80239EE8();
    }
    r27 = (u32)sp + 0x8;
    r28 = r28 & 0xFFFF;
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r3 = r24;
        r4 = 0x0;
        r5 = 0xd5;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r4 = *(u32*)(r27 + r0);
        if ((u32)r3 != (u32)r4) {
            r3 = r29;
            r5 = 0x8;
            fn_802384B4();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
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
                r4 = r29;
                r8 = r31;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0x14d;
                fn_80239EE8();
                r3 = r26;
                return;
        }
        }
        r25 = r25 + 0x1;


    }

    r3 = r26;
    return;
}

/* Address: 0x80245578 | Size: 0x1A0 (416 bytes) */
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
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r3;
    r28 = r4;
    r31 = r5;
    r29 = 0x0;
    fn_8023831C();
    r0 = r3 & 0xFFFF;

    if ((u32)r0 != (u32)0x3 && (u32)r0 != (u32)0x9) goto L_802455F8;

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
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x148;
    fn_80239EE8();
L_802455F8:
    f1 = *(f32*)(u32)lbl_8047E630;
    r3 = r30;
    r4 = r28;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r30;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x149;
        fn_80239EE8();
    }
    r3 = r30;
    r4 = r28;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r30;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x14a;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r30;
    r5 = 0x14b;
    fn_80239984();
    r29 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r30;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x14b;
    fn_80239EE8();
    r3 = r29;
    return;
}

/* Address: 0x80245718 | Size: 0x98 */
void fn_80245718(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8047E630[];
    extern void fn_80205B8C();
    extern void fn_802373B0();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;

    f1 = *(f32*)(u32)lbl_8047E630;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = 0x0;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x147;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x802457B0 | Size: 0x168 (360 bytes) */
void fn_802457B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern u8 lbl_8047E630[];
    extern void fn_80205B8C();
    extern void fn_80235B04();
    extern void fn_802373B0();
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
    f32 f1 = 0.0f;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r28 = r4;
    r29 = r5;
    r27 = r3;
    r30 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r31 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x144;
        fn_80239EE8();
    }
    f1 = *(f32*)(u32)lbl_8047E630;
    r3 = r27;
    r4 = r28;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x145;
        fn_80239EE8();
    }
    r0 = r31 & 0xFF;
    if (((u32)r0 != (u32)0x2) && ((u32)r0 != (u32)0x4)) {

        if ((u32)r0 != (u32)0x3) { r3 = r30; return; }
    }
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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x146;
    fn_80239EE8();

    r3 = r30;
    return;
}

/* Address: 0x80245918 | Size: 0x98 */
void fn_80245918(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r29 = r4;
    r30 = r5;
    r28 = r3;
    r4 = r6;
    r31 = 0x0;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x143;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x802459B0 | Size: 0x44C (1100 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    r4 = (u32)sp + 0x8;
    *(u16*)(sp + 0x8) = r0;
    fn_801F8A18();
    if ((u32)r3 == (u32)0x0) {
        r30 = 0x1;
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x139;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x13a;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x13b;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x13c;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x13d;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x13e;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x13f;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x140;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x141;
        fn_80239EE8();
    }
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x142;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80245DFC | Size: 0x14C (332 bytes) */
void fn_80245DFC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235B04();
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

    r28 = r4;
    r29 = r5;
    r27 = r3;
    r30 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r31 = r3;
    r0 = r3 & 0xFF;

    if ((u32)r0 != (u32)0x1 && (u32)r0 != (u32)0x2) goto L_80245E88;

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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x136;
    fn_80239EE8();
L_80245E88:
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x4) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x137;
        fn_80239EE8();
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x3) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x138;
        fn_80239EE8();
    }
    r3 = r30;
    return;
}

/* Address: 0x80245F48 | Size: 0x7C | Pattern: field_accessor */
u32 fn_80245F48(void* ctx, u32 slot, u32 param) {
    extern void fn_801363E8();
    extern void fn_801F54A4();
    extern void fn_8023CA9C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

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
    if ((u32)r3 != (u32)r0) {
        r3 = r28;
        r4 = r29;
        r6 = r31;
        fn_8023CA9C();
    } else {

        r3 = 0x0;
    }
    return;
}

/* Address: 0x80247048 | Size: 0x7C | Pattern: field_accessor */
u32 fn_80247048(void* ctx, u32 slot, u32 param) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

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
    r4 = r29;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x135;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x802470C4 | Size: 0xEC (236 bytes) */
void fn_802470C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x134;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x802471B0 | Size: 0x128 (296 bytes) */
void fn_802471B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801FB1C0();
    extern void fn_80205B8C();
    extern void fn_80235AA0();
    extern void fn_80239564();
    extern void fn_802399FC();
    extern void fn_80239CCC();
    u8 sp[0x30];
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
    if ((u32)r0 >= (u32)0xc) {
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
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x132;
        fn_80239CCC();
    }
    r3 = r30;
    return;
}

/* Address: 0x802472D8 | Size: 0xDC (220 bytes) */
void fn_802472D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80235A3C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 >= (u32)0xc) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x130;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x802473B4 | Size: 0x144 (324 bytes) */
void fn_802473B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    fn_8023831C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x17) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x12c;
        fn_80239EE8();
    }
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x12d;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x12e;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x12e;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x802474F8 | Size: 0x1A8 (424 bytes) */
void fn_802474F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_8023831C();
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

    r29 = r3;
    r30 = r4;
    r31 = r5;
    r27 = r6;
    r28 = 0x0;
    fn_8023831C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x17) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x128;
        fn_80239EE8();
    }
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x129;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r29;
    r5 = 0x12a;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        r4 = r29;
        r5 = 0x12b;
        fn_80239984();
        r28 = r3;
        r3 = r30;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x12b;
        fn_80239EE8();
    }
    r3 = r28;
    return;
}

/* Address: 0x802476A0 | Size: 0x110 (272 bytes) */
void fn_802476A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_802383A4();
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

    r29 = r5;
    r30 = r6;
    r28 = r4;
    r27 = r3;
    r31 = 0x0;
    r4 = r30;
    r5 = 0x3d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = r27;
        r4 = r30;
        fn_802383A4();
        r0 = r3 & 0xFFFF;
        if ((s32)r0 != (s32)0) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x126;
            fn_80239EE8();
    }
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x3c;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x127;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x802477B0 | Size: 0x158 (344 bytes) */
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((s32)r0 == (s32)0) {
        r26 = 0x0;
    }
    r0 = r28 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_80247834;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 == (u32)0xaf || (u32)r0 == (u32)0x0) goto L_80247834;

    r3 = r25;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80247838;
L_80247834:
    r26 = 0x0;
L_80247838:
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x124;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r24;
    r5 = 0x3c;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x125;
        fn_80239EE8();
    }
    r3 = r27;
    return;
}

/* Address: 0x80247908 | Size: 0x1C0 (448 bytes) */
void fn_80247908(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80235B04();
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

    r30 = r4;
    r31 = r5;
    r29 = r3;
    r27 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r28 = r3;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x120;
        fn_80239EE8();
    }
    r4 = r29;
    r8 = r30;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x121;
        fn_80239EE8();
    }
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x2 || (u32)r0 == (u32)0x4) goto L_80247A18;

    if ((u32)r0 != (u32)0x3) goto L_80247A60;
L_80247A18:
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
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x122;
    fn_80239EE8();
L_80247A60:
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x3) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x123;
        fn_80239EE8();
    }
    r3 = r27;
    return;
}

/* Address: 0x80247AC8 | Size: 0x194 (404 bytes) */
void fn_80247AC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
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
    if ((s32)r0 == (s32)0) {
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
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x11e;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r29;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r31;
        r4 = r29;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r31;
            r4 = r29;
            r5 = 0x29;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r31;
                r4 = r29;
                r5 = 0x28;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x1) { r3 = r30; return; }
    }
    }
    }
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
    r4 = r31;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11f;
    fn_80239EE8();

    r3 = r30;
    return;
}

/* Address: 0x80247C5C | Size: 0x184 (388 bytes) */
void fn_80247C5C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x11b;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x11;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x11c;
    fn_80239EE8();

    r3 = r31;
    return;
}

/* Address: 0x80247DE0 | Size: 0x1C0 (448 bytes) */
void fn_80247DE0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80237310();
    extern void fn_80237F74();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        r4 = r29;
        r5 = 0x118;
        fn_80239984();
        r28 = r3;
        r3 = r30;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x118;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r27;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r29;
        r4 = r27;
        r5 = 0x11;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r29;
            r4 = r27;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r28; return; }
    }
    }
    r3 = r28;
    r4 = r29;
    r5 = 0x119;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x119;
    fn_80239EE8();

    r3 = r28;
    return;
}

/* Address: 0x80247FA0 | Size: 0x1D0 (464 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x114;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r26;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r29;
        r4 = r26;
        r5 = 0x11;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r29;
            r4 = r26;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r27; return; }
    }
    }
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
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x115;
    fn_80239CCC();

    r3 = r27;
    return;
}

/* Address: 0x80248170 | Size: 0x150 (336 bytes) */
void fn_80248170(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80237F74();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r6 = 0x1;
    r7 = 0x10e;
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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x110;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r29;
    r5 = 0x36;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x111;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x112;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x112;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x802482C0 | Size: 0x1A0 (416 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
        r3 = r26;
        r4 = 0x1d;
        fn_80201D84();
        r3 = r3 & 0xFFFF;
        r0 = r28 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
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
            r4 = r29;
            r8 = r31;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x10a;
            fn_80239EE8();
    }
    }
    r3 = r27;
    r4 = r29;
    r5 = 0x10b;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
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
    if ((u32)r0 != (u32)0x1) {
        r3 = r29;
        r4 = r26;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) { r3 = r28; return; }
    }
    r3 = r28;
    r4 = r29;
    r5 = 0x10c;
    fn_80239984();
    r28 = r3;
    r3 = r30;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x10c;
    fn_80239EE8();

    r3 = r28;
    return;
}

/* Address: 0x80248460 | Size: 0x22C (556 bytes) */
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
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801F1C18();
    r23 = r3;
    r24 = (u32)sp + 0x8;
    r25 = r3 & 0xFFFF;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if ((u32)r0 >= (u32)r25) break;
        r5 = *(u32*)(r24 + r0);
        if ((u32)r5 != (u32)0x0) {
            r3 = r26;
            r4 = r29;
            fn_80236D60();
            if ((s32)r3 > (s32)0x0) break;
        }
        r22 = r22 + 0x1;


    }

    r3 = r22 & 0xFFFF;
    r0 = r23 & 0xFFFF;
    if ((u32)r3 < (u32)r0) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x107;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r29;
        r4 = 0x1d;
        fn_80201D84();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r0 == (u32)r3) {
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
            r4 = r26;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x108;
            fn_80239EE8();
    }
    }
    r3 = r26;
    r4 = r29;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)r3) {
        r3 = r26;
        r4 = r29;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r26;
            r4 = r29;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r30; return; }
    }
    }
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
    r4 = r26;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x109;
    fn_80239EE8();

    r3 = r30;
    return;
}

/* Address: 0x8024868C | Size: 0x1D0 (464 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x105;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r26;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r29;
        r4 = r26;
        r5 = 0x7;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r29;
            r4 = r26;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r27; return; }
    }
    }
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
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x106;
    fn_80239CCC();

    r3 = r27;
    return;
}

/* Address: 0x8024885C | Size: 0x2C0 (704 bytes) */
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
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)r26) break;
        r3 = r30;
        r4 = 0x0;
        r5 = 0xd5;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
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
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x102;
        fn_80239CCC();
        break;
    L_80248A38:
        r24 = r24 + 0x1;


    }

    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)r26) {
        r3 = r27;
        r4 = r30;
        r5 = 0x28;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
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
    fn_802399FC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r28;
        r5 = 0xfd;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xfd;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r28;
        r5 = 0xfe;
        fn_80239984();
        r27 = r3;
        r3 = r29;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xfe;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r31;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r28;
        r4 = r31;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r28;
            r4 = r31;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r27; return; }
    }
    }
    r3 = r27;
    r4 = r28;
    r5 = 0xff;
    fn_80239984();
    r27 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xff;
    fn_80239EE8();

    r3 = r27;
    return;
}

/* Address: 0x80248D3C | Size: 0x288 (648 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r5;
    r27 = r3;
    r28 = r4;
    r30 = r6;
    r31 = 0x0;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf6;
        fn_80239EE8();
    }
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0xf8;
        fn_80239984();
        r31 = r3;
        r3 = r28;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf8;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = r27;
        r5 = 0xf9;
        fn_80239984();
        r31 = r3;
        r3 = r28;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf9;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x13;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xfa;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x80248FC4 | Size: 0x49C (1180 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    r4 = (u32)sp + 0x8;
    *(u16*)(sp + 0x8) = r0;
    fn_801F8A18();
    if ((u32)r3 == (u32)0x0) {
        r29 = 0x1;
    }
    r0 = r29 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xeb;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xec;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xed;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xee;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xef;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf0;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf1;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf2;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf3;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf4;
        fn_80239EE8();
    }
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xf5;
        fn_80239EE8();
    }
    r3 = r30;
    return;
}

/* Address: 0x80249460 | Size: 0x218 (536 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe8;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x7;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe9;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x29;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xea;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x80249678 | Size: 0x248 (584 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe4;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe5;
        fn_80239CCC();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe6;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x802498C0 | Size: 0x1F4 (500 bytes) */
void fn_802498C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802358AC();
    extern void fn_80237F74();
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
    if ((s32)r0 == (s32)0) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe0;
        fn_80239EE8();
    }
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
L_802499F4:
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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xe1;
    fn_80239EE8();
L_80249A3C:
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x4) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xe2;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x80249AB4 | Size: 0x278 (632 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xdc;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xdd;
        fn_80239CCC();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r27;
                r4 = r30;
                r5 = 0x33;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
    }
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xde;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x80249D2C | Size: 0x25C (604 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xd8;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235974();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xd9;
        fn_80239CCC();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xda;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x80249F88 | Size: 0x1E8 (488 bytes) */
void fn_80249F88(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_802359D8();
    extern void fn_80237F74();
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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xd4;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r27;
    fn_802359D8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r29;
        r8 = r31;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xd5;
        fn_80239EE8();
    }
    r3 = r29;
    r4 = r27;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r29;
        r4 = r27;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r29;
            r4 = r27;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r28; return; }
    }
    }
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
    r4 = r29;
    r8 = r31;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xd6;
    fn_80239EE8();

    r3 = r28;
    return;
}

/* Address: 0x8024A170 | Size: 0x2B8 (696 bytes) */
void fn_8024A170(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
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
    fn_801F1C18();
    r30 = r3;
    r4 = r24;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r31 = (u32)sp + 0x8;
    r23 = r3 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
        r19 = *(u32*)(r31 + r0);
        if ((u32)r19 != (u32)0x0) {
            r21 = (u32)sp + 0x28;
            r22 = r30 & 0xFFFF;
            r20 = 0x0;
            while (1) {
                r0 = r20 & 0xFFFF;
            if ((u32)r0 >= (u32)r22) break;
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
                r4 = r24;
                r8 = r26;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0xcf;
                fn_80239EE8();
                break;
            L_8024A268:
                r20 = r20 + 0x1;


            }
        }
        r28 = r28 + 0x1;


    }
    if ((u32)r23 >= (u32)0x2) {
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
        if ((u32)r0 >= (u32)r28) break;
        r5 = *(u32*)(r31 + r0);
        if ((u32)r5 != (u32)0x0) {
            r3 = r24;
            r4 = r27;
            fn_80236D60();
            if ((s32)r3 < (s32)0x0) break;
        }
        r21 = r21 + 0x1;


    }

    r3 = r21 & 0xFFFF;
    r0 = r30 & 0xFFFF;
    if ((u32)r3 < (u32)r0) {
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
    if ((u32)r0 != (u32)0x1) {
        r3 = r24;
        r4 = r27;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r24;
            r4 = r27;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r29; return; }
    }
    }
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
    fn_801F1C18();
    r30 = r3;
    r4 = r24;
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r31 = (u32)sp + 0x8;
    r23 = r3 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
        r19 = *(u32*)(r31 + r0);
        if ((u32)r19 != (u32)0x0) {
            r21 = (u32)sp + 0x28;
            r22 = r30 & 0xFFFF;
            r18 = 0x0;
            r20 = 0x0;
            while (1) {
                r0 = r20 & 0xFFFF;
                if ((u32)r0 >= (u32)r22) break;
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
                r4 = r24;
                r8 = r26;
                r6 = 0x0;
                r7 = 0x0;
                r9 = 0x0;
                r10 = 0xcc;
                fn_80239EE8();
                r18 = 0x1;
                break;
            L_8024A528:
                r20 = r20 + 0x1;


            }

            r0 = r18 & 0xFF;
            if ((u32)r0 == (u32)0x1) break;
        }
        r28 = r28 + 0x1;


    }

    r3 = r24;
    r4 = r27;
    fn_80235910();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)r23) {
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
    if ((u32)r0 != (u32)0x1) {
        r3 = r24;
        r4 = r27;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r24;
            r4 = r27;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r29; return; }
    }
    }
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc8;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc9;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235910();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xca;
        fn_80239CCC();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xcb;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x8024A924 | Size: 0x25C (604 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc4;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235A3C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc5;
        fn_80239CCC();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
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
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc6;
    fn_80239CCC();

    r3 = r31;
    return;
}

/* Address: 0x8024AB80 | Size: 0x204 (516 bytes) */
void fn_8024AB80(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80235AA0();
    extern void fn_80237F74();
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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc0;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xc1;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x1d;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r27;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r27;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r27;
                r4 = r30;
                r5 = 0x34;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
    }
    }
    }
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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xc2;
    fn_80239EE8();

    r3 = r31;
    return;
}

/* Address: 0x8024AD84 | Size: 0x16C (364 bytes) */
void fn_8024AD84(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80237F74();
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
    if ((s32)r0 != (s32)0) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xbd;
        fn_80239EE8();
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x27;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xbe;
        fn_80239EE8();
        r3 = r31;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x13;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) { r3 = r31; return; }
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
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbe;
    fn_80239EE8();

    r3 = r31;
    return;
}

/* Address: 0x8024AEF0 | Size: 0xD4 (212 bytes) */
void fn_8024AEF0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236D60();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r5;
    r28 = r3;
    r29 = r4;
    r5 = r6;
    r31 = 0x0;
    fn_80236D60();
    if ((s32)r3 > (s32)0x0) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xbb;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0xbc;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xbc;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x8024AFC4 | Size: 0x4B0 (1200 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r6;
    r29 = r5;
    r27 = r3;
    r28 = r4;
    r5 = r30;
    r31 = 0x0;
    fn_80236D60();
    if ((s32)r3 > (s32)0x0) {
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
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb1;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x4;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb2;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x6;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb3;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x1c;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb4;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x18;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb5;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x5;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb6;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x9;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb7;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0xa;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb8;
            fn_80239EE8();
        }
        r3 = r27;
        r4 = r30;
        r5 = 0x1e;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xb9;
            fn_80239EE8();
    }
    }
    r4 = r27;
    r8 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xba;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024B474 | Size: 0x5D0 (1488 bytes) */
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
    fn_801F1C18();
    r27 = r3;
    r4 = r30;
    r5 = (u32)sp + 0x1c;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r25 = (u32)sp + 0x3c;
    r22 = r27 & 0xFFFF;
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFFFF;
        if ((u32)r0 >= (u32)r22) break;
        r0 = *(u32*)(r25 + r23);
        if ((u32)r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r25 + r23);
            r6 = r3;
            r3 = r30;
            r5 = r24;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x43) {
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
        if ((u32)r0 >= (u32)r22) break;
        r0 = *(u32*)(r25 + r23);
        if ((u32)r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r25 + r23);
            r6 = r3;
            r3 = r30;
            r5 = r24;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x42) {
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
        if ((u32)r0 >= (u32)r22) break;
        r4 = *(u32*)(r25 + r0);
        if ((u32)r29 == (u32)r4) goto L_8024B710;
        r3 = r30;
        r5 = (u32)sp + 0x8;
        r6 = 0x0;
        r7 = 0x1;
        fn_802367CC();
        r4 = r3 & 0xFFFF;
        r26 = r3;
        if ((u32)r29 == (u32)r4) goto L_8024B710;
        r3 = (u32)sp + 0x8;
        r23 = 0x0;
        while (1) {
            r0 = r23 & 0xFFFF;
            if ((u32)r0 >= (u32)r4) break;
            r0 = *(u16*)(r3 + r0);
            if ((u32)r0 == (u32)0xb6 || (u32)r0 == (u32)0xc5) goto L_8024B6A4;

            if ((u32)r0 != (u32)0xcb) goto L_8024B6F0;
        L_8024B6A4:
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
            r4 = r30;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0xab;
            fn_80239EE8();
            break;
        L_8024B6F0:
            r23 = r23 + 0x1;


        }

        r3 = r23 & 0xFFFF;
        r0 = r26 & 0xFFFF;
        if ((u32)r3 < (u32)r0) break;
    L_8024B710:
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
    if ((u32)r0 == (u32)0x1) {
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
    r7 = 0xc5;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0xac;
    fn_80239EE8();
L_8024B878:
    r4 = r30;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)r23) break;
        r4 = *(u32*)(r24 + r0);
        if ((u32)r29 == (u32)r4) goto L_8024B974;
        r3 = r30;
        fn_80236520();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0xb6 || (u32)r0 == (u32)0xc5) goto L_8024B92C;

        if ((u32)r0 != (u32)0xcb) goto L_8024B974;
    L_8024B92C:
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
        r4 = r30;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xae;
        fn_80239EE8();
    L_8024B974:
        r22 = r22 + 0x1;


    }
    r22 = (u32)sp + 0x3c;
    r23 = r27 & 0xFFFF;
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if ((u32)r0 >= (u32)r23) break;
        r4 = *(u32*)(r22 + r24);
        if ((u32)r29 != (u32)r4) {
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
                fn_80205B8C();
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
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    r4 = (u32)sp + 0x8;
    *(u16*)(sp + 0x8) = r0;
    fn_801F8A18();
    if ((u32)r3 == (u32)0x0) {
        r30 = 0x1;
    }
    r0 = r30 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x9f;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa0;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa1;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa2;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa4;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa5;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa6;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa7;
        fn_80239EE8();
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa3;
        fn_80239EE8();
    }
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
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
        r4 = r26;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0xa8;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024BE7C | Size: 0x144 (324 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r5;
    r31 = r3;
    r26 = r4;
    r29 = 0x0;
    r5 = 0x1a;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r27;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x9c;
        fn_80239EE8();
    }
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
    r4 = r31;
    r8 = r27;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x9e;
    fn_80239EE8();
    r3 = r30;
    return;
}

/* Address: 0x8024BFC0 | Size: 0x5FC (1532 bytes) */
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
    fn_801F1C18();
    r27 = r3;
    r4 = r30;
    r5 = (u32)sp + 0x1c;
    r3 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F1C18();
    r26 = r3;
    r24 = (u32)sp + 0x3c;
    r21 = r27 & 0xFFFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if ((u32)r0 >= (u32)r21) break;
        r0 = *(u32*)(r24 + r22);
        if ((u32)r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r24 + r22);
            r6 = r3;
            r3 = r30;
            r5 = r25;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x43) {
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
        if ((u32)r0 >= (u32)r21) break;
        r0 = *(u32*)(r24 + r22);
        if ((u32)r29 != (u32)r0) {
            r3 = r30;
            r4 = r28;
            fn_80239500();
            r4 = *(u32*)(r24 + r22);
            r6 = r3;
            r3 = r30;
            r5 = r25;
            fn_8023793C();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x42) {
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
        if ((u32)r0 >= (u32)r21) break;
        r4 = *(u32*)(r24 + r0);
        if ((u32)r29 == (u32)r4) goto L_8024C260;
        r3 = r30;
        r5 = (u32)sp + 0x8;
        r6 = 0x0;
        r7 = 0x1;
        fn_802367CC();
        r4 = r3 & 0xFFFF;
        r25 = r3;
        if ((u32)r29 == (u32)r4) goto L_8024C260;
        r3 = (u32)sp + 0x8;
        r22 = 0x0;
        while (1) {
            r0 = r22 & 0xFFFF;
            if ((u32)r0 >= (u32)r4) break;
            r0 = *(u16*)(r3 + r0);
            if ((u32)r0 == (u32)0xb6 || (u32)r0 == (u32)0xc5) goto L_8024C1F4;

            if ((u32)r0 != (u32)0xcb) goto L_8024C240;
        L_8024C1F4:
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
            r4 = r30;
            r8 = r28;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x96;
            fn_80239EE8();
            break;
        L_8024C240:
            r22 = r22 + 0x1;


        }

        r3 = r22 & 0xFFFF;
        r0 = r25 & 0xFFFF;
        if ((u32)r3 < (u32)r0) break;
    L_8024C260:
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
    if ((u32)r0 == (u32)0x1) {
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
    r7 = 0xc5;
    r8 = 0x0;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    r4 = r30;
    r8 = r28;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x97;
    fn_80239EE8();
L_8024C3C8:
    r25 = (u32)sp + 0x1c;
    r22 = r26 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r22) break;
        r3 = r30;
        r4 = *(u32*)(r25 + r0);
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            fn_80205B8C();
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
        if ((u32)r0 >= (u32)r22) break;
        r4 = *(u32*)(r23 + r0);
        if ((u32)r29 == (u32)r4) goto L_8024C4EC;
        r3 = r30;
        fn_80236520();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0xb6 || (u32)r0 == (u32)0xc5) goto L_8024C4A4;

        if ((u32)r0 != (u32)0xcb) goto L_8024C4EC;
    L_8024C4A4:
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
        r4 = r30;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x9a;
        fn_80239EE8();
    L_8024C4EC:
        r21 = r21 + 0x1;


    }
    r21 = (u32)sp + 0x3c;
    r22 = r27 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r22) break;
        r4 = *(u32*)(r21 + r23);
        if ((u32)r29 != (u32)r4) {
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
                fn_80205B8C();
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
    fn_801F1C18();
    r28 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
        fn_80205B8C();
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
    fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x89;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8a;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8b;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8c;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8d;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8e;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x8f;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x90;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x91;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
        if ((u32)r0 >= (u32)r25) break;
        r3 = r31;
        r4 = *(u32*)(r26 + r0);
        r5 = (u32)sp + 0x8;
        r6 = 0x0;
        r7 = 0x0;
        fn_802367CC();
        r23 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x1) {
            r24 = (u32)sp + 0x8;
            r21 = 0x0;
            r22 = 0x0;
            while (1) {
                r0 = r22 & 0xFFFF;
                if ((u32)r0 >= (u32)r23) break;
                r3 = r31;
                r5 = *(u16*)(r24 + r0);
                r4 = 0x1f;
                fn_8025C5A4();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r3 = r27;
                    r4 = r31;
                    r5 = 0x92;
                    fn_80239984();
                    r27 = r3;
                    r3 = r30;
                    fn_80205B8C();
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
            if ((u32)r0 == (u32)0x1) break;
        }
        r28 = r28 + 0x1;


    }

    r3 = r31;
    r4 = r30;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r27;
        r4 = r31;
        r5 = 0x93;
        fn_80239984();
        r27 = r3;
        r3 = r30;
        fn_80205B8C();
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
    fn_801F1C18();
    r26 = r3;
    r3 = r31;
    r4 = r25;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
        fn_80205B8C();
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r31;
        r8 = r28;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x7b;
        fn_80239EE8();
    }
    r0 = r28 & 0xFFFF;

    if ((u32)r0 != (u32)0x13 && (u32)r0 != (u32)0x154) goto L_8024D6A0;

    r27 = 0x1f;
    goto L_8024D6BC;
L_8024D6A0:
    if ((u32)r0 == (u32)0x5b) {
        r27 = 0x20;
        goto L_8024D6BC;
    }
    if ((u32)r0 != (u32)0x123) goto L_8024D6BC;
    r27 = 0x21;
L_8024D6BC:
    r25 = (u32)sp + 0x1c;
    r24 = r26 & 0xFFFF;
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFFFF;
        if ((u32)r0 >= (u32)r24) break;
        r3 = r31;
        r4 = *(u32*)(r25 + r0);
        r5 = (u32)sp + 0x8;
        r6 = 0x0;
        r7 = 0x0;
        fn_802367CC();
        r22 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x123) {
            r23 = (u32)sp + 0x8;
            r20 = 0x0;
            r21 = 0x0;
            while (1) {
                r0 = r21 & 0xFFFF;
                if ((u32)r0 >= (u32)r22) break;
                r3 = r31;
                r5 = *(u16*)(r23 + r0);
                r4 = r27;
                fn_8025C5A4();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
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
            if ((u32)r0 == (u32)0x1) break;
        }
        r26 = r26 + 0x1;


    }

    r3 = r31;
    r4 = r29;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
void fn_8024D818(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80237F74();
    extern void fn_8023831C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = 0x0;
    r5 = 0x14;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x66;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r29;
    fn_8023831C();
    r0 = r3 & 0xFFFF;

    if ((u32)r0 != (u32)0x8 && (u32)r0 != (u32)0x9) goto L_8024D8FC;

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
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x67;
    fn_80239EE8();
L_8024D8FC:
    r3 = r31;
    r4 = r28;
    r5 = 0x68;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x68;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x8024D958 | Size: 0x1A4 (420 bytes) */
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
    u32 r3 = (u32)ctx;
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
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x64;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x40;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x65;
        fn_80239EE8();
    }
    r3 = r26;
    return;
}

/* Address: 0x8024DAFC | Size: 0xC0 */
void fn_8024DAFC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x62;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x8024DBBC | Size: 0xC0 */
void fn_8024DBBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x60;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x8024DC7C | Size: 0x210 (528 bytes) */
void fn_8024DC7C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_80236BFC();
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

    r6 = 0x0;
    r7 = 0x1;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r5 = (u32)sp + 0x8;
    r4 = r27;
    r30 = 0x0;
    r3 = 0x0;
    fn_801F1C18();
    r31 = r3;
    r3 = r27;
    r4 = r28;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x117) {
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
        r4 = r27;
        r8 = r29;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x5b;
        fn_80239EE8();
    }
    r25 = (u32)sp + 0x8;
    r26 = r31 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r26) break;
        r3 = r27;
        r4 = *(u32*)(r25 + r0);
        r5 = 0x8;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x5c;
            fn_80239EE8();
            break;
        }
        r24 = r24 + 0x1;


    }

    r26 = (u32)sp + 0x8;
    r31 = r31 & 0xFFFF;
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r3 = r27;
        r4 = *(u32*)(r26 + r0);
        r5 = 0x7;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            r4 = r27;
            r8 = r29;
            r6 = 0x0;
            r7 = 0x0;
            r9 = 0x0;
            r10 = 0x5d;
            fn_80239EE8();
            break;
        }
        r25 = r25 + 0x1;


    }

    r3 = r30;
    r4 = r27;
    r5 = 0x5e;
    fn_80239984();
    r25 = r3;
    r3 = r28;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r27;
    r8 = r29;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5e;
    fn_80239EE8();
    r3 = r25;
    return;
}

/* Address: 0x8024DE8C | Size: 0x138 (312 bytes) */
void fn_8024DE8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_802358AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x6) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x58;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r29;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x6) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x59;
        fn_80239EE8();
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x5a;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x5a;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x8024DFC4 | Size: 0x108 (264 bytes) */
void fn_8024DFC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80205B8C();
    extern void fn_80236520();
    extern void fn_8023943C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_80236520();
    r0 = r3 & 0xFFFF;
    r4 = r3;
    if ((s32)r0 != (s32)0) {
        if ((u32)r0 != (u32)0xffff) {
            if ((u32)r0 != (u32)0x165) {
                if ((u32)r0 != (u32)0x163) {
                    r3 = r28;
                    r5 = 0x1;
                    fn_8023943C();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) {
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
                        r4 = r28;
                        r8 = r30;
                        r6 = 0x0;
                        r7 = 0x0;
                        r9 = 0x0;
                        r10 = 0x56;
                        fn_80239EE8();
    }
    }
    }
    }
    }
    r3 = r31;
    r4 = r28;
    r5 = 0x57;
    fn_80239984();
    r31 = r3;
    r3 = r29;
    fn_80205B8C();
    r6 = (0x1 << 16);
    r5 = r3;
    r4 = r28;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x57;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x8024E0CC | Size: 0x7C | Pattern: field_accessor */
u32 fn_8024E0CC(void* ctx, u32 slot, u32 param) {
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

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
    r4 = r29;
    r8 = r30;
    r6 = 0x0;
    r7 = 0x0;
    r9 = 0x0;
    r10 = 0x55;
    fn_80239EE8();
    r3 = r31;
    return;
}

/* Address: 0x8024E148 | Size: 0xEC (236 bytes) */
void fn_8024E148(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x54;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024E234 | Size: 0xEC (236 bytes) */
void fn_8024E234(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x52;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024E320 | Size: 0x164 (356 bytes) */
void fn_8024E320(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_802357CC();
    extern void fn_802358AC();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r29 = r4;
    r28 = r3;
    r30 = r5;
    r4 = r6;
    r31 = 0x0;
    fn_802357CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x6) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x4e;
        fn_80239EE8();
    }
    r3 = r28;
    r4 = r29;
    fn_802358AC();
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x6) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x4f;
        fn_80239EE8();
    }
    r4 = r28;
    r8 = r29;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x10e;
    fn_801F1990();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x50;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024E484 | Size: 0xA8 */
void fn_8024E484(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F1990();
    extern void fn_80205B8C();
    extern void fn_80239984();
    extern void fn_80239EE8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r6 = 0x1;
    r7 = 0x10e;
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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r28;
        r8 = r30;
        r6 = 0x0;
        r7 = 0x0;
        r9 = 0x0;
        r10 = 0x4d;
        fn_80239EE8();
    }
    r3 = r31;
    return;
}

/* Address: 0x8024E534 | Size: 0x44 | Pattern: field_accessor */
u32 fn_8024E534(void* ctx, u32 slot, u32 param) {
    extern u32 fn_801FB1C0();
    u32 val;
    val = fn_801FB1C0(ctx, 0, 0x43, 0);
    fn_801FB1C0(0, val & 0xFFFF, 0x2, 0);
    return 0;
}

/* Address: 0x8024E578 | Size: 0x118 (280 bytes) */
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

    r6 = 0x0;
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
    if ((s32)r0 <= (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    fn_80205B8C();
    r0 = 0x0;
    r5 = (0x1 << 16);
    *(u32*)(sp + 0x8) = r0;
    r0 = 0x228;
    r7 = r3;
    r6 = r30;
    *(u32*)(sp + 0xC) = r0;
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
    if ((s32)r0 < (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = (u32)lbl_80375D30;
    r8 = (s16)r5;
    r7 = (u32)lbl_80375D30;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_8020505C();
    r3 = 0x1;

    return;
}

/* Address: 0x8024F8B4 | Size: 0x5CC (1484 bytes) */
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
    fn_801F1C18();
    r28 = r3;
    r14 = (u32)sp + 0x18;
    r31 = r3 & 0xFFFF;
    r18 = 0x0;
    r17 = 0x0;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r19 = *(u32*)(r14 + r0);
        if ((u32)r19 != (u32)0x0) {
            r3 = r15;
            r4 = r19;
            fn_802376EC();
            r25 = r3 & 0xFFFF;
            r30 = r27 & 0xFFFF;
            r21 = 0x0;
            while (1) {
                r0 = r21 & 0xFFFF;
                if ((u32)r0 >= (u32)r30) break;
                r3 = (u32)sp + 0x38;
                r22 = *(u16*)(r3 + r0);
                if ((u32)r22 != (u32)0x0) {
                    if ((u32)r22 != (u32)0x165) {
                        r3 = r15;
                        r4 = r16;
                        r5 = r22;
                        r6 = r19;
                        fn_8023C530();
                        r29 = r3;
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x1) {
                            r17 = 0x1;
                        }
                        r3 = r15;
                        r4 = r19;
                        fn_80237288();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x1) {
                            r17 = 0x1;
                        }
                        r3 = r15;
                        r4 = r22;
                        r5 = 0x1;
                        fn_8023943C();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 != (u32)0x1) {
                            r3 = r15;
                            r4 = r16;
                            r5 = r22;
                            r6 = r19;
                            r7 = 0x0;
                            fn_8023C370();
                            if ((s32)r25 < (s32)r3) {
                                r0 = r29 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
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
    fn_80205B8C();
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
    if ((u32)r0 != (u32)r31) {
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
    if ((u32)r0 == (u32)r31) {
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
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x2;
        fn_80239EE8();
        goto L_8024FBB8;
    }
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
    r4 = r20;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x3;
    fn_80239EE8();
L_8024FBB8:
    r3 = r15;
    r4 = r16;
    fn_80235714();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x5;
        fn_80239EE8();
    }
    r0 = r17 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        r4 = r20;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        r10 = 0x6;
        fn_80239EE8();
    }
    r0 = r18 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x1) {
        r17 = (u32)sp + 0x18;
        r14 = r28 & 0xFFFF;
        r18 = 0x0;
        while (1) {
            r0 = r18 & 0xFFFF;
            if ((u32)r0 >= (u32)r14) break;
            r4 = *(u32*)(r17 + r0);
            if ((u32)r4 != (u32)0x0) {
                r3 = r15;
                fn_80236E9C();
                r3 = r3 & 0xFFFF;
                r0 = r26 & 0xFFFF;
                if ((u32)r3 > (u32)r0) {
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
    fn_80250070();
    r0 = r3 & 0xFFFF;
    r14 = r3;
    if ((u32)r0 != (u32)r14) {
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
        r4 = r20;
        r10 = r14;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r9 = 0x0;
        fn_80239EE8();
    }
    r3 = r16;
    fn_80205B8C();
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
    return;
}

/* Address: 0x8024FE80 | Size: 0x1F0 (496 bytes) */
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
    fn_801F1C18();
    r28 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x16;
    fn_80239058();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r25 = (u32)sp + 0x1c;
        r26 = r28 & 0xFFFF;
        r24 = 0x0;
        while (1) {
            r0 = r24 & 0xFFFF;
            if ((u32)r0 >= (u32)r26) break;
            r3 = r29;
            r4 = *(u32*)(r25 + r27);
            fn_8023715C();
            r4 = *(u32*)(r25 + r27);
            r27 = r3;
            r3 = r29;
            fn_80236FFC();
            r4 = r27 & 0xFFFF;
            r0 = r3 & 0xFFFF;
            if ((u32)r4 >= (u32)r0) {
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
    if ((u32)r0 == (u32)0x1) {
        r27 = (u32)sp + 0x8;
        r25 = (u32)sp + 0x1c;
        r28 = r28 & 0xFFFF;
        r24 = 0x0;
        while (1) {
            r0 = r24 & 0xFFFF;
            if ((u32)r0 >= (u32)r28) break;
            r22 = 0x0;
            r23 = 0x0;
            while (1) {
                r0 = r23 & 0xFFFF;
                if ((u32)r0 >= (u32)0xa) break;
                r4 = *(u32*)(r25 + r26);
                r5 = *(u16*)(r27 + r0);
                r3 = r29;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r22 = 0x1;
                    break;
                }
                r23 = r23 + 0x1;


            }

            r0 = r22 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = 0x26;
                return;
            }
            r24 = r24 + 0x1;


        }
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)r28) {
        r3 = r29;
        r4 = r30;
        r5 = 0x4d;
        fn_80239058();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x27;
            return;
        }
        r3 = r29;
        r4 = r30;
        r5 = 0xd;
        fn_80239058();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x28;
            return;
    }
    }
    r3 = 0x0;

    return;
}

/* Address: 0x80250070 | Size: 0x27C (636 bytes) */
void fn_80250070(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023753C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0x9;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x9;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0xa;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0xa;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        fn_8023753C();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0xb;
            return;
    }
    }
    r3 = r30;
    r4 = r31;
    r5 = 0xe;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0xc;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x17;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0xd;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x18;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0xe;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x19;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0xf;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x1b;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x10;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x11;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x12;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x13;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x27;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x14;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x28;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x15;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x29;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x16;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x2a;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x17;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x30;
    fn_80236BFC();
    r3 = r3 & 0xFF;
    r0 = 0x18;
    r3 = r3 - r3; /* -borrow */;
    r3 = r0 & r3;

    return;
}

/* Address: 0x802502EC | Size: 0x694 (1684 bytes) */
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
    fn_801F1C18();
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
        if ((u32)r0 >= (u32)r17) break;
        r19 = *(u32*)(r18 + r0);
        if ((u32)r19 != (u32)0x0) {
            r3 = r19;
            fn_802062FC();
            r0 = r3 & 0xFF;
            if ((u32)r19 != (u32)0x0) {
                r3 = r28;
                r4 = r19;
                fn_80235714();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 >= (u32)0x14) break;
        r5 = r5 + 0x1;
        *(u32*)(r3 + r0) = r4;


    }
    r20 = (u32)sp + 0x38;
    r27 = r31 & 0xFFFF;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r27) break;
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
        r3 = (u32)sp + 0x110;
        r4 = 0x0;
        r7 = 0x0;
        fn_801440F0();
        r3 = (s16)r3;
        r0 = -r3;
        r0 = r0 & ~r3;
        if ((u32)r0 == (u32)0x7) goto L_8025084C;
        r0 = r21 & 0xFF;

        if ((u32)r0 != (u32)0x2 && (u32)r0 != (u32)0x1) goto L_80250530;

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
        r18 = (u32)sp + 0xc0;
        r3 = *(u32*)(r18 + r17);
        r4 = r28;
        r5 = 0x2e;
        fn_80239984();
        *(u32*)(r18 + r17) = r3;
        r3 = r29;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r28;
        r9 = r23;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r10 = 0x2e;
        fn_80239EE8();
    L_80250530:
        r0 = r21 & 0xFF;

        if ((u32)r0 != (u32)0x3 && (u32)r0 != (u32)0x1) goto L_802505B8;

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
        r18 = (u32)sp + 0xc0;
        r3 = *(u32*)(r18 + r17);
        r4 = r28;
        r5 = 0x2f;
        fn_80239984();
        *(u32*)(r18 + r17) = r3;
        r3 = r29;
        fn_80205B8C();
        r6 = (0x1 << 16);
        r5 = r3;
        r4 = r28;
        r9 = r23;
        r6 = 0x0;
        r7 = 0x0;
        r8 = 0x0;
        r10 = 0x2f;
        fn_80239EE8();
    L_802505B8:
        r0 = r21 & 0xFF;
        if ((u32)r0 == (u32)0x5) {
            r3 = r28;
            r4 = r29;
            fn_80235714();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x5) {
                r17 = r25 << 2;
                r18 = (u32)sp + 0xc0;
                r3 = *(u32*)(r18 + r17);
                r4 = r28;
                r5 = 0x30;
                fn_80239984();
                *(u32*)(r18 + r17) = r3;
                r3 = r29;
                fn_80205B8C();
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
        if ((u32)r0 == (u32)0x4) {
            r3 = r28;
            r4 = r29;
            fn_80236C80();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x2) {
                r17 = r25 << 2;
                r18 = (u32)sp + 0xc0;
                r3 = *(u32*)(r18 + r17);
                r4 = r28;
                r5 = 0x31;
                fn_80239984();
                *(u32*)(r18 + r17) = r3;
                r3 = r29;
                fn_80205B8C();
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
        if ((u32)r0 == (u32)0x6) {
            r0 = r30 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r17 = r25 << 2;
                r18 = (u32)sp + 0xc0;
                r3 = *(u32*)(r18 + r17);
                r4 = r28;
                r5 = 0x32;
                fn_80239984();
                *(u32*)(r18 + r17) = r3;
                r3 = r29;
                fn_80205B8C();
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
        if ((u32)r0 == (u32)0x2) goto L_80250800;
        r3 = r28;
        r4 = r29;
        fn_8023785C();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x3) goto L_80250800;
        r19 = (u32)sp + 0x60;
        r17 = r26 & 0xFFFF;
        r18 = 0x0;
        while (1) {
            r0 = r18 & 0xFFFF;
            if ((u32)r0 >= (u32)r17) break;
            r3 = r29;
            r4 = 0x0;
            r5 = 0xd5;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            r0 = *(u32*)(r19 + r21);
            if ((u32)r3 == (u32)r0) goto L_802507F0;
            r3 = r28;
            r4 = r29;
            fn_8023785C();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x2) {
                r3 = r28;
                r4 = r29;
                fn_8023785C();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x3) goto L_802507F0;
            }
            r3 = *(u32*)(r19 + r21);
            fn_80206608();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) goto L_802507F0;
            r17 = r25 << 2;
            r18 = (u32)sp + 0xc0;
            r3 = *(u32*)(r18 + r17);
            r4 = r28;
            r5 = 0x33;
            fn_80239984();
            *(u32*)(r18 + r17) = r3;
            r3 = r29;
            fn_80205B8C();
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
        L_802507F0:
            r18 = r18 + 0x1;


        }
    L_80250800:
        r3 = r29;
        fn_80205B8C();
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
    L_8025084C:
        r24 = r24 + 0x1;


    }
    r4 = (u32)sp + 0xc0;
    r0 = r31 & 0xFFFF;
    r17 = 0x0;
    while (1) {
        r3 = r17 & 0xFFFF;
        if ((u32)r3 >= (u32)r0) break;
        r3 = *(u32*)(r4 + r3);
        if ((s32)r3 > (s32)0x0) break;
        r17 = r17 + 0x1;


    }

    r3 = r17 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x0;
        return;
    }
    r4 = r31;
    r3 = (u32)sp + 0xc0;
    r5 = 0x1;
    fn_802397B8();
    if ((u32)r3 < (u32)r0) {
        r3 = 0x0;
        return;
    }
    r3 = (u32)sp + 0x38;
    r17 = *(u16*)(r3 + r0);
    if ((u32)r17 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r29;
    fn_80205B8C();
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
    fn_80204CE0();
    r3 = 0x1;

    return;
}

/* Address: 0x802509A0 | Size: 0x84 | Pattern: field_accessor */
u32 fn_802509A0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80250A2C | Size: 0x84 | Pattern: field_accessor */
u32 fn_80250A2C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80250AC0 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80250AC0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80250B44 | Size: 0x78 | Pattern: field_accessor */
u32 fn_80250B44(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80235B04();
    extern void fn_80250BBC();
    u8 sp[0x20];
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
    u32 r4 = slot;
    u32 r5 = param;

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
    if ((s32)r0 != (s32)0) {
        r3 = r3 << 1;
    }
    return;
}

/* Address: 0x80250BBC | Size: 0xA8 */
void fn_80250BBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_80235B04();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r4 = 0x0;
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
    if ((u32)r0 == (u32)0x2) {
        r0 = 0xb;

    } else if ((u32)r0 == (u32)0x3) {
        r0 = 0x5;

    } else if ((u32)r0 == (u32)0x1) {
        r0 = 0xa;

    } else if ((u32)r0 == (u32)0x4) {
        r0 = 0xf;

    } else {
        r0 = 0x0;
    }
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_8011BBD8();
    return;
}

/* Address: 0x80250C64 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80250C64(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80250CF0 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80250CF0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80250D7C | Size: 0x84 | Pattern: field_accessor */
u32 fn_80250D7C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80250E00 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80250E00(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80250E84 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80250E84(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80250EC4 | Size: 0x90 */
void fn_80250EC4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_802152A8();
    extern void fn_802377E8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r4 = 0x0;
    r6 = 0x0;
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
    return;
}

/* Address: 0x80250F7C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80250F7C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80250FBC | Size: 0xB4 */
void fn_80250FBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80237664();
    extern void fn_802376EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r4 = 0x0;
    r6 = 0x0;
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
    if ((s32)r0 == (s32)0) {
        r0 = 0x1;
    }
    r3 = r29;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    return;
}

/* Address: 0x80251070 | Size: 0x5C | Pattern: field_accessor */
u32 fn_80251070(void* ctx, u32 slot, u32 param) {
    extern void fn_802376EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;

    r29 = r3;
    r30 = r6;
    fn_802376EC();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = r30;
    fn_802376EC();
    r0 = r3 & 0xFFFF;
    r3 = r31 - r0;
    r0 = r0 - r31;
    r3 = r3 + r4;
    r3 = r3 - r3; /* -borrow */;
    r3 = r0 & ~r3;
    return;
}

/* Address: 0x802510CC | Size: 0x84 | Pattern: field_accessor */
u32 fn_802510CC(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

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
u32 fn_802511E0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802512A4 | Size: 0xAC */
void fn_802512A4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BEB4();
    extern void fn_801363E8();
    extern void fn_801F54A4();
    extern void fn_8023C370();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 != (u32)r31) {
        r3 = r27;
        r4 = r28;
        r5 = r30;
        r6 = r29;
        r7 = 0x1;
        fn_8023C370();
    } else {

        r3 = 0x0;
    }
    return;
}

/* Address: 0x80251358 | Size: 0x78 | Pattern: field_accessor */
u32 fn_80251358(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r31 = r31 << 1;
    }
    r3 = r31;
    return;
}

/* Address: 0x802513D0 | Size: 0x84 | Pattern: field_accessor */
u32 fn_802513D0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80251454 | Size: 0x70 | Pattern: field_accessor */
u32 fn_80251454(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_8023720C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r31 = r31 << 1;
    }
    r3 = r31;
    return;
}

/* Address: 0x802514EC | Size: 0x98 */
void fn_802514EC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80119DD0();
    extern void fn_80202360();
    extern void fn_80236BFC();
    extern void fn_80237664();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0x2d;
    r29 = r3;
    r30 = r4;
    r31 = 0x1;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = 0x2d;
        fn_80202360();
        r31 = r3;
    }
    r3 = 0x2d;
    fn_80119DD0();
    r0 = r3 & 0xFF;
    r0 = r0 - r31;
    r0 = (s16)r0;
    if ((u32)r0 < (u32)0x1) {
        r0 = 0x0;
    }
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
    return;
}

/* Address: 0x80251584 | Size: 0x88 */
void fn_80251584(void* ctx, u32 param1, u32 param2) {
    extern void fn_80202360();
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
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

    r29 = r5;
    r27 = r3;
    r28 = r4;
    r30 = r6;
    r31 = 0x1;
    r5 = 0x2d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        r4 = 0x2d;
        fn_80202360();
        r31 = r3;
    }
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
    r3 = r0 * r3;
    return;
}

/* Address: 0x80251614 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251614(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251658 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80251658(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251688 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251688(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x802516C4 | Size: 0xD4 (212 bytes) */
void fn_802516C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80216CF8();
    extern void fn_80237774();
    extern void fn_802377E8();
    extern void fn_8023892C();
    extern void fn_80238980();
    u8 sp[0x90];
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

    r7 = 0x1;
    r22 = r3;
    r23 = r5;
    r24 = r6;
    r5 = (u32)sp + 0x8;
    r4 = r22;
    r28 = 0x0;
    r3 = 0x0;
    r6 = 0x1;
    fn_801F1A6C();
    r29 = (u32)sp + 0x8;
    r31 = r3 & 0xFFFF;
    r27 = 0x0;
    while (1) {
        r0 = r27 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
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


    }
    r3 = r28;
    return;
}

/* Address: 0x802517A0 | Size: 0x84 | Pattern: field_accessor */
u32 fn_802517A0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80251824 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251824(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251860 | Size: 0x78 | Pattern: field_accessor */
u32 fn_80251860(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r31 = r31 << 1;
    }
    r3 = r31;
    return;
}

/* Address: 0x802518D8 | Size: 0x78 | Pattern: field_accessor */
u32 fn_802518D8(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r31 = r31 << 1;
    }
    r3 = r31;
    return;
}

/* Address: 0x80251950 | Size: 0xBC */
void fn_80251950(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BEB4();
    extern void fn_801F025C();
    extern void fn_802026E4();
    extern void fn_80232110();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r3 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r31 * 0xf;
        r0 = 0xa;
        r31 = (s32)r3 / (s32)r0;
    }
    r3 = r31;
    return;
}

/* Address: 0x80251A0C | Size: 0x78 | Pattern: field_accessor */
u32 fn_80251A0C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r31 = r31 << 1;
    }
    r3 = r31;
    return;
}

/* Address: 0x80251A84 | Size: 0x78 | Pattern: field_accessor */
u32 fn_80251A84(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r4 = r5;
    r9 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r31 = r31 << 1;
    }
    r3 = r31;
    return;
}

/* Address: 0x80251AFC | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251AFC(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80251B50 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80251B50(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80251BD4 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80251BD4(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80251C58 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80251C58(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80251CEC | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80251CEC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251D2C | Size: 0x88 */
void fn_80251D2C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_80120B00();
    extern void fn_80205B8C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;

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
    r4 = (u32)sp + 0xa;
    r5 = (u32)sp + 0x8;
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

/* Address: 0x80251DB4 | Size: 0x90 */
void fn_80251DB4(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235B04();
    extern void fn_80237664();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0x1;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = r30;
        r4 = r31;
        fn_80237664();
        r3 = -r0;
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        fn_80237664();
        r3 = r3 & 0xFFFF;
        r0 = 0x1e;
        r3 = r3 * 0x14;
        r0 = (s32)r3 / (s32)r0;
        r3 = -r0;
        return;
    }
    r3 = r30;
    r4 = r31;
    fn_80237664();

    r3 = -r0;
    return;
}

/* Address: 0x80251E44 | Size: 0x90 */
void fn_80251E44(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235B04();
    extern void fn_80237664();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0x1;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = r30;
        r4 = r31;
        fn_80237664();
        r3 = -r0;
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        fn_80237664();
        r3 = r3 & 0xFFFF;
        r0 = 0x1e;
        r3 = r3 * 0x14;
        r0 = (s32)r3 / (s32)r0;
        r3 = -r0;
        return;
    }
    r3 = r30;
    r4 = r31;
    fn_80237664();

    r3 = -r0;
    return;
}

/* Address: 0x80251ED4 | Size: 0x90 */
void fn_80251ED4(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235B04();
    extern void fn_80237664();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0x1;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = r30;
        r4 = r31;
        fn_80237664();
        r3 = -r0;
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        fn_80237664();
        r3 = r3 & 0xFFFF;
        r0 = 0x1e;
        r3 = r3 * 0x14;
        r0 = (s32)r3 / (s32)r0;
        r3 = -r0;
        return;
    }
    r3 = r30;
    r4 = r31;
    fn_80237664();

    r3 = -r0;
    return;
}

/* Address: 0x80251F6C | Size: 0x84 | Pattern: field_accessor */
u32 fn_80251F6C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80251FF0 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80251FF0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252038 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252038(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252078 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252078(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C((void*)param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x46);
    return 0;
}

/* Address: 0x802520BC | Size: 0x84 | Pattern: field_accessor */
u32 fn_802520BC(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252148 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252148(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252188 | Size: 0x80 | Pattern: field_accessor */
u32 fn_80252188(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80217BD0();
    extern void fn_8023842C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r4 = 0x0;
    r6 = 0x0;
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
    return;
}

/* Address: 0x80252208 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252208(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252248 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252248(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C((void*)param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x28);
    return 0;
}

/* Address: 0x8025228C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_8025228C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802522CC | Size: 0x80 | Pattern: field_accessor */
u32 fn_802522CC(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80217BEC();
    extern void fn_8023842C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r4 = 0x0;
    r6 = 0x0;
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
    return;
}

/* Address: 0x80252354 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80252354(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252398 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252398(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802523D8 | Size: 0x90 */
void fn_802523D8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r4 = 0x0;
    r6 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
    }
    r3 = r31;
    r7 = r30 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    return;
}

/* Address: 0x802524B8 | Size: 0x84 | Pattern: field_accessor */
u32 fn_802524B8(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x8025253C | Size: 0xB4 */
void fn_8025253C(void* ctx, u32 param1, u32 param2) {
    extern void fn_80211170();
    extern void fn_802525F0();
    extern void fn_80252634();
    extern void fn_80252678();
    u8 sp[0x20];
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

    r7 = (u32)fn_80252678;
    r8 = 0x0;
    r9 = (u32)fn_80252678;
    r7 = 0x0;
    r10 = 0x0;
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
    return;
}

/* Address: 0x802525F0 | Size: 0x44 | Pattern: field_accessor */
u32 fn_802525F0(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C((void*)param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x1e);
    return 0;
}

/* Address: 0x80252634 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252634(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C((void*)param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0x14);
    return 0;
}

/* Address: 0x80252678 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80252678(void* ctx, u32 slot, u32 param) {
    extern void fn_8011BBD8();
    u32 val = (u32)fn_8012640C((void*)param, 0, 0xd9, 0);
    fn_8011BBD8(val, 0, 0x2f, 0, 0xa);
    return 0;
}

/* Address: 0x802526BC | Size: 0x84 | Pattern: field_accessor */
u32 fn_802526BC(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252748 | Size: 0x74 | Pattern: field_accessor */
u32 fn_80252748(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_802376EC();
    u8 sp[0x20];
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
    u32 r4 = slot;
    u32 r5 = param;

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
    if ((s32)r4 <= (s32)r3) {
    }
    return;
}

/* Address: 0x802527C4 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_802527C4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252804 | Size: 0x90 */
void fn_80252804(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BBD8();
    extern void fn_80218B6C();
    extern void fn_80237664();
    extern void fn_802376EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r4 = 0x0;
    r6 = 0x0;
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
    return;
}

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
u32 fn_802529F4(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252A80 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252A80(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252B04 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252B04(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252B44 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252B44(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252BC8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80252BC8(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80252C04 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252C04(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252C88 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252C88(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252D0C | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252D0C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252D90 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252D90(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252E14 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252E14(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252E98 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252E98(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80252F8C | Size: 0x84 | Pattern: field_accessor */
u32 fn_80252F8C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80253020 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80253020(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802530A4 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_802530A4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802530E4 | Size: 0x84 | Pattern: field_accessor */
u32 fn_802530E4(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80253168 | Size: 0x88 */
void fn_80253168(void* ctx, u32 param1, u32 param2) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r30 & 0xFFFF;
        if ((u32)r0 == (u32)0xfa) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802531F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802531F8(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253234 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80253234(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253270 | Size: 0x28 | Pattern: call_return_u16 */
u16 fn_80253270(void* ctx, u32 p1, u32 p2, u32 p3) { return (u16)fn_802376EC(ctx, p3, p2); }

/* Address: 0x80253298 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80253298(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802532C0 | Size: 0x84 | Pattern: field_accessor */
u32 fn_802532C0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x8025334C | Size: 0x84 | Pattern: field_accessor */
u32 fn_8025334C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802533D8 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802533D8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253400 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80253400(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x8025348C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_8025348C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802534D4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802534D4(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253548 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80253548(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802535F4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802535F4(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253630 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80253630(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x8025366C | Size: 0x84 | Pattern: field_accessor */
u32 fn_8025366C(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802536F0 | Size: 0x84 | Pattern: field_accessor */
u32 fn_802536F0(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80253774 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80253774(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802537F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802537F8(void* ctx, u32 param1, u32 param2, u32 param3) { return fn_80211170(ctx, param2, param1, param3, 0, 0, 0, 0); }

/* Address: 0x80253834 | Size: 0x84 | Pattern: field_accessor */
u32 fn_80253834(void* ctx, u32 slot, u32 param) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x802538C0 | Size: 0x88 */
void fn_802538C0(void* ctx, u32 param1, u32 param2) {
    extern void fn_80211170();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r7 = 0x0;
    r8 = 0x0;
    r0 = r4;
    r9 = 0x0;
    r10 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r30 & 0xFFFF;
        if ((u32)r0 == (u32)0x39) {
            r31 = r31 << 1;
        }
    }
    r3 = r31;
    return;
}

/* Address: 0x80253950 | Size: 0x6C | Pattern: field_accessor */
u32 fn_80253950(void* ctx, u32 slot, u32 param) {
    extern void fn_80136428();
    extern void fn_801F54A4();
    extern void fn_80237DBC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r5 = 0xf;
    r6 = 0x0;
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
    r3 = 0x1 - r0;
    r3 = r3 - r0; /* -borrow */;
    return;
}

/* Address: 0x802539BC | Size: 0xC4 (196 bytes) */
void fn_802539BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235910();
    extern void fn_80235AA0();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 >= (u32)0xc) {
        r0 = r3 & 0xFF;
        if ((u32)r0 >= (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xc) {
        r3 = r27;
        r4 = r28;
        r5 = r30;
        r6 = r29;
        r7 = 0x10;
        r8 = 0x3;
        r9 = 0x41;
        fn_8025C808();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253A80 | Size: 0xC4 (196 bytes) */
void fn_80253A80(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235974();
    extern void fn_802359D8();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 >= (u32)0xc) {
        r0 = r3 & 0xFF;
        if ((u32)r0 >= (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x4;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xc) {
        r3 = r27;
        r4 = r28;
        r5 = r30;
        r6 = r29;
        r7 = 0x10;
        r8 = 0x5;
        r9 = 0x41;
        fn_8025C808();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253B44 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80253B44(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253B78 | Size: 0xB4 */
void fn_80253B78(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253C2C | Size: 0xC4 (196 bytes) */
void fn_80253C2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 >= (u32)0xc) {
        r0 = r3 & 0xFF;
        if ((u32)r0 >= (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xc) {
        r3 = r27;
        r4 = r28;
        r5 = r30;
        r6 = r29;
        r7 = 0x10;
        r8 = 0x2;
        r9 = 0x41;
        fn_8025C808();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253CF0 | Size: 0xB4 */
void fn_80253CF0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253DA4 | Size: 0xC4 (196 bytes) */
void fn_80253DA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235974();
    extern void fn_80235A3C();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 >= (u32)0xc) {
        r0 = r3 & 0xFF;
        if ((u32)r0 >= (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xc) {
        r3 = r27;
        r4 = r28;
        r5 = r30;
        r6 = r29;
        r7 = 0x10;
        r8 = 0x5;
        r9 = 0x41;
        fn_8025C808();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0xc) {
            r3 = 0x0;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253E68 | Size: 0xC0 */
void fn_80253E68(void* ctx, u32 param1, u32 param2) {
    extern void fn_80235A3C();
    extern void fn_80235AA0();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((s32)r0 == (s32)0) {
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = r27;
        r4 = r28;
        r5 = r30;
        r6 = r29;
        r7 = 0x90;
        r8 = 0x2;
        r9 = 0x1;
        fn_8025C808();
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0) {
            r3 = 0x0;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253F28 | Size: 0xB4 */
void fn_80253F28(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80253FDC | Size: 0xF0 (240 bytes) */
void fn_80253FDC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80235B04();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_8025C264();
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
    r4 = 0x0;
    r5 = 0x1;
    fn_80235B04();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) {
        r30 = 0xb;

    } else if ((u32)r0 == (u32)0x3) {
        r30 = 0x5;

    } else if ((u32)r0 == (u32)0x1) {
        r30 = 0xa;

    } else if ((u32)r0 == (u32)0x4) {
        r30 = 0xf;

    } else {
        r30 = 0x0;
    }
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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}


/* -------------------------------------------------------------------
 * Item Rewards & Poke Coupon (0x80254000-0x80258000)
 * 95 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802540CC | Size: 0xB4 */
void fn_802540CC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254180 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254180(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802541B4 | Size: 0xB4 */
void fn_802541B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254268 | Size: 0x1F8 (504 bytes) */
void fn_80254268(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_802543A0;
    }
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
L_8025439C:
    r0 = 0x1;
L_802543A0:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
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
void fn_80254460(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254514 | Size: 0xB4 */
void fn_80254514(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802545C8 | Size: 0xB0 */
void fn_802545C8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80254680 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80254680(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802546B8 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_802546B8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802546E8 | Size: 0x128 (296 bytes) */
void fn_802546E8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1C18();
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
    fn_801F1C18();
    r28 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x27;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r27 = (u32)sp + 0x30;
    r28 = r28 & 0xFFFF;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r4 = *(u32*)(r27 + r0);
        if ((u32)r4 != (u32)0x0) {
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
                if ((u32)r3 >= (u32)r0) break;
                r9 = 0x0;
                while (1) {
                    r3 = r9 & 0xFFFF;
                    if ((u32)r3 >= (u32)r4) break;
                    r6 = *(u16*)(r8 + r7);
                    r3 = *(u16*)(r5 + r3);
                    if ((u32)r6 == (u32)r3) {
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
void fn_80254810(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80229934();
    extern void fn_80237F74();
    extern void fn_8025C264();
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

    r7 = (0x1 << 16);
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = 0x19;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x19;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x802548D8 | Size: 0xB4 */
void fn_802548D8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025498C | Size: 0xE4 (228 bytes) */
void fn_8025498C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802376EC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
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
    if ((u32)r3 <= (u32)r0) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254A70 | Size: 0xB4 */
void fn_80254A70(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254B24 | Size: 0x178 (376 bytes) */
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0xf;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x26;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r4 = r30;
    r3 = 0x8;
    fn_80229704();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
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
void fn_80254C9C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80254D4C | Size: 0xB4 */
void fn_80254D4C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254E00 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_80254E00(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254E34 | Size: 0xB4 */
void fn_80254E34(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80254EE8 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254EE8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F1C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80254F1C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F54 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254F54(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
u32 fn_80254F88(void* ctx, u32 slot, u32 param) {
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = (0x1 << 16);
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x19;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x80255000 | Size: 0xF0 (240 bytes) */
void fn_80255000(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80142984();
    extern void fn_80216048();
    extern void fn_80237F74();
    extern void fn_802383A4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r0 = r31 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r0 = r30 & 0xFFFF;
        if ((s32)r0 == (s32)0) { r3 = 0x0; return; }
    }
    r3 = r31 & 0xFFFF;
    if ((u32)r3 == (u32)0xaf) { r3 = 0x0; return; }
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0xaf) { r3 = 0x0; return; }
    if ((u32)r3 != (u32)0x0) {
        r3 = r31;
        fn_80142984();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) { r3 = 0x0; return; }
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_802550B4;
    r3 = r30;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802550B4;

    r3 = 0x0;
    return;
L_802550B4:
    r3 = r28;
    r4 = r29;
    r5 = 0x3c;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802550F0 | Size: 0xB4 */
void fn_802550F0(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F025C();
    extern void fn_801F54A4();
    extern void fn_802062FC();
    extern void fn_80236BFC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r29 = r3;
    r30 = r4;
    r3 = 0xe;
    fn_801F025C();
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x19;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) { r3 = 0x0; return; }
    r3 = r31;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) { r3 = 0x0; return; }
    r3 = r29;
    r4 = r30;
    r5 = 0x32;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) { r3 = 0x0; return; }
    r3 = r29;
    r4 = r31;
    r5 = 0x32;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) { r3 = 0x1; return; }

    r3 = 0x0;
    return;

    r3 = 0x1;

    return;
}

/* Address: 0x802551A4 | Size: 0x74 | Pattern: field_accessor */
u32 fn_802551A4(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x30;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x80255220 | Size: 0xA8 */
void fn_80255220(void* ctx, u32 param1, u32 param2) {
    extern void fn_8011BEB4();
    extern void fn_801363E8();
    extern void fn_801F54A4();
    extern void fn_8023C530();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 != (u32)r31) {
        r3 = r27;
        r4 = r28;
        r5 = r30;
        r6 = r29;
        fn_8023C530();
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x802552D0 | Size: 0xB4 */
void fn_802552D0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80255384 | Size: 0xB4 */
void fn_80255384(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80255438 | Size: 0xB4 */
void fn_80255438(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802554EC | Size: 0x10C (268 bytes) */
void fn_802554EC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802359D8();
    extern void fn_80235AA0();
    extern void fn_80236BFC();
    extern void fn_8025C808();
    extern void fn_8025CAA8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = r31 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r26;
    r4 = r27;
    r5 = r29;
    r6 = r28;
    r7 = 0xa0;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r26;
        r4 = r27;
        r5 = r29;
        r6 = r28;
        r7 = 0xa0;
        r8 = 0x4;
        r9 = 0x1;
        fn_8025C808();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r26;
    r4 = r29;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802555F8 | Size: 0x228 (552 bytes) */
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
    r5 = 0x14;
    r4 = r30;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x6;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0xa;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x29;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80255784;
    }
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
L_80255780:
    r0 = 0x1;
L_80255784:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x80255820 | Size: 0x218 (536 bytes) */
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
    fn_8025C264();
    r30 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFF;
        if ((u32)r0 >= (u32)0xc) {
            r3 = r31;
            r4 = r25;
            r5 = r27;
            r6 = r26;
            r7 = 0x10;
            r8 = 0x4;
            r9 = 0x1;
            fn_8025C808();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0xc) {
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_802559DC;
    }
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
L_802559D8:
    r0 = 0x1;
L_802559DC:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
u32 fn_80255A38(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = 0x0;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x1b;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x80255AAC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80255AAC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255AE4 | Size: 0x68 | Pattern: field_accessor */
u32 fn_80255AE4(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E648[];
    extern void fn_80236BFC();
    extern void fn_802373B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    u32 r4 = slot;
    u32 r5 = param;

    r5 = 0x2d;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {

        f1 = *(f32*)(u32)lbl_8047E648;
        r3 = r30;
        r4 = r31;
        r5 = 0x0;
        fn_802373B0();
        r0 = r3 & 0xFF;
        r3 = 0x1 - r0;
        r3 = r3 - r0; /* -borrow */;
    }
    return;
}

/* Address: 0x80255B4C | Size: 0xCC (204 bytes) */
void fn_80255B4C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
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
    if ((s32)r0 == (s32)0) {
        r31 = 0x0;
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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80255C18 | Size: 0x74 | Pattern: field_accessor */
u32 fn_80255C18(void* ctx, u32 slot, u32 param) {
    extern void fn_80119DD0();
    extern void fn_80202360();
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r5 = 0x2d;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r31 = 0x0;
    } else {

        r3 = r31;
        r4 = 0x2d;
        fn_80202360();
        r31 = r3;
    }
    r3 = 0x2d;
    fn_80119DD0();
    r3 = r3 & 0xFF;
    r0 = (s16)r31;
    if ((s32)r0 >= (s32)r3) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* Address: 0x80255C8C | Size: 0xB0 */
void fn_80255C8C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80255D3C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80255D3C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255D7C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80255D7C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255DB4 | Size: 0x44 | Pattern: field_accessor */
u32 fn_80255DB4(void* ctx, u32 slot, u32 param) {
    extern u32 fn_8025C808();
    u32 r3 = (u32)ctx;
    u32 r4 = slot;
    u32 r5 = param;
    u32 r6 = 0;
    /* r3=ctx, r4=slot, r5=param, r6=? -> call(ctx, slot, ?, param, 0x10, 0x2, 0x41) */
    u32 result = fn_8025C808(ctx, slot, 0, param, 0x10, 0x2, 0x41) & 0xFF;
    return (result != 0) ? 1 : 0;
}

/* Address: 0x80255DF8 | Size: 0xB0 */
void fn_80255DF8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80255EA8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80255EA8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255EEC | Size: 0xB4 */
void fn_80255EEC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80255FA0 | Size: 0xB4 */
void fn_80255FA0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256054 | Size: 0xB4 */
void fn_80256054(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256108 | Size: 0xB4 */
void fn_80256108(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802561BC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_802561BC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802561F4 | Size: 0xB4 */
void fn_802561F4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802562A8 | Size: 0xB4 */
void fn_802562A8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025635C | Size: 0xB4 */
void fn_8025635C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256410 | Size: 0xB0 */
void fn_80256410(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x802564C8 | Size: 0x64 | Pattern: field_accessor */
u32 fn_802564C8(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E64C[];
    extern void fn_80235AA0();
    extern void fn_802373B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    u32 r4 = slot;
    u32 r5 = param;

    r30 = r3;
    r31 = r4;
    fn_80235AA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 >= (u32)0xc) { r3 = 0x0; return; }
    f1 = *(f32*)(u32)lbl_8047E64C;
    r3 = r30;
    r4 = r31;
    r5 = -0x1;
    fn_802373B0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0xc) { r3 = 0x1; return; }

    r3 = 0x0;
    return;

    r3 = 0x1;

    return;
}

/* Address: 0x8025652C | Size: 0xB4 */
void fn_8025652C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802565E0 | Size: 0xB4 */
void fn_802565E0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256694 | Size: 0xB4 */
void fn_80256694(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256748 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256748(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256780 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256780(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802567B8 | Size: 0xA4 */
void fn_802567B8(void* ctx, u32 param1, u32 param2) {
    extern void fn_80120B00();
    extern void fn_80205B8C();
    extern void fn_8023793C();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;

    r29 = r4;
    r28 = r3;
    r31 = r5;
    r30 = r6;
    r3 = r29;
    fn_80205B8C();
    r4 = (u32)sp + 0xa;
    r5 = (u32)sp + 0x8;
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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x8025685C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025685C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256894 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256894(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802568CC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_802568CC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256904 | Size: 0xB0 */
void fn_80256904(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x802569B4 | Size: 0xB4 */
void fn_802569B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256A68 | Size: 0x78 | Pattern: field_accessor */
u32 fn_80256A68(void* ctx, u32 slot, u32 param) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r0 = r4;
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
    r3 = 0x43 - r0;
    r3 = r3 - r0; /* -borrow */;
    return;
}

/* Address: 0x80256AE0 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256AE0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256B18 | Size: 0xB4 */
void fn_80256B18(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256BCC | Size: 0xB4 */
void fn_80256BCC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256C80 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80256C80(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256CBC | Size: 0xB4 */
void fn_80256CBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80256D70 | Size: 0xB0 */
void fn_80256D70(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80256E20 | Size: 0xB0 */
void fn_80256E20(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80256ED0 | Size: 0x200 (512 bytes) */
void fn_80256ED0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802016A4();
    extern void fn_80236BFC();
    extern void fn_80237288();
    extern void fn_80237F74();
    extern void fn_8025C264();
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
    fn_8025C264();
    r0 = r3;
    r3 = r27;
    r29 = r0;
    r4 = r28;
    r5 = 0xc;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r29 = 0x0;
    }
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
L_80257030:
    r0 = 0x1;
L_80257034:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
L_802570A0:
    r29 = 0x0;
L_802570A4:
    if ((s32)r29 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802570D0 | Size: 0xB0 */
void fn_802570D0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80257180 | Size: 0x23C (572 bytes) */
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFF;
        if ((u32)r0 >= (u32)0xc) {
            r3 = r31;
            r4 = r25;
            r5 = r27;
            r6 = r26;
            r7 = 0x20;
            r8 = 0x1;
            r9 = 0x1;
            fn_8025C808();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0xc) {
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80257360;
    }
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
L_8025735C:
    r0 = 0x1;
L_80257360:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = 0x4b;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
void fn_802573BC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80257474 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80257474(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802574AC | Size: 0x5C | Pattern: field_accessor */
u32 fn_802574AC(void* ctx, u32 slot, u32 param) {
    extern void fn_80229934();
    extern void fn_8025C674();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r31 = 0;
    u32 r5 = param;

    r31 = r3;
    r3 = r5;
    r5 = r6;
    fn_80229934();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
    } else {

        r3 = r31;
        fn_8025C674();
        r3 = r3 & 0xFFFF;
        r3 = r3 - r0; /* -borrow */;
    }
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* Address: 0x80257508 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80257508(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80257544 | Size: 0x7C | Pattern: field_accessor */
u32 fn_80257544(void* ctx, u32 slot, u32 param) {
    extern void fn_80119DD0();
    extern void fn_801F025C();
    extern void fn_801F6D9C();
    extern void fn_801F6E98();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;

    r3 = 0x3;
    fn_801F025C();
    r30 = 0x0;
    r31 = r3;
    r4 = 0x4a;
    fn_801F6E98();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = 0x4a;
        fn_801F6D9C();
        r30 = r3;
    }
    r3 = 0x4a;
    fn_80119DD0();
    r3 = r3 & 0xFF;
    r0 = (s16)r30;
    if ((s32)r0 >= (s32)r3) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x802575C8 | Size: 0x188 (392 bytes) */
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r25;
        r4 = r28;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r25;
        r4 = r28;
        r5 = 0x18;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) { r3 = 0x0; return; }
        r3 = r25;
        r4 = r28;
        fn_80237288();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) { r3 = 0x1; return; }

        r3 = 0x0;
        return;
    }
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = r29 & 0xFF;
        if ((u32)r0 >= (u32)0xc) {
            r0 = r30 & 0xFF;
            if ((u32)r0 >= (u32)0xc) {
                r3 = 0x0;
                return;
    }
    }
    }
    r3 = r25;
    r4 = r26;
    r5 = r28;
    r6 = r27;
    r7 = 0x90;
    r8 = 0x3;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) { r3 = 0x1; return; }
    r3 = r25;
    r4 = r26;
    r5 = r28;
    r6 = r27;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) { r3 = 0x1; return; }
    r3 = r25;
    r4 = r26;
    r5 = r28;
    r6 = r27;
    r7 = 0x10;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0xc) { r3 = 0x1; return; }
    r3 = 0x0;
    return;

    r3 = 0x1;

    return;
}

/* Address: 0x80257750 | Size: 0xE8 (232 bytes) */
void fn_80257750(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x7;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80257838 | Size: 0x8C */
void fn_80257838(void* ctx, u32 param1, u32 param2) {
    extern void fn_80236BFC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0x14;
    r31 = r6;
    r30 = r3;
    r4 = r31;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x17;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x8;
    fn_80236BFC();
    r3 = r3 & 0xFF;
    r3 = r3 - r0; /* -borrow */;

    return;
}

/* Address: 0x802578C4 | Size: 0x78 | Pattern: field_accessor */
u32 fn_802578C4(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = (0x1 << 16);
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x16;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x8025793C | Size: 0xB4 */
void fn_8025793C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802579F0 | Size: 0xB0 */
void fn_802579F0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80257AA0 | Size: 0xB4 */
void fn_80257AA0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80257B54 | Size: 0xCC (204 bytes) */
void fn_80257B54(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F1A6C();
    extern void fn_80238748();
    extern void fn_80239058();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
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
    r25 = r3;
    r26 = r5;
    r5 = (u32)sp + 0x8;
    r4 = r25;
    r3 = 0x0;
    fn_801F1A6C();
    r28 = (u32)sp + 0x8;
    r31 = r3 & 0xFFFF;
    r30 = r26 & 0xFFFF;
    r26 = 0x0;
    r27 = 0x0;
    while (1) {
        r0 = r27 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r4 = *(u32*)(r28 + r29);
        if ((u32)r4 == (u32)0x0) goto L_80257BE8;
        if ((u32)r30 == (u32)0xd7) {
            r3 = r25;
            r5 = 0x2b;
            fn_80239058();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) goto L_80257BE8;
        }
        r4 = *(u32*)(r28 + r29);
        r3 = r25;
        fn_80238748();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) goto L_80257BE8;
        r26 = 0x1;
        break;
    L_80257BE8:
        r27 = r27 + 0x1;


    }

    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)r31) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80257C20 | Size: 0xB4 */
void fn_80257C20(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80257CD4 | Size: 0x124 (292 bytes) */
void fn_80257CD4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80237288();
    extern void fn_802381C4();
    extern void fn_8025C264();
    u8 sp[0x50];
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

    r7 = (0x1 << 16);
    r30 = r3;
    r31 = r6;
    fn_8025C264();
    r27 = r3;
    r3 = r30;
    r4 = r31;
    r5 = (u32)sp + 0x1c;
    r6 = (u32)sp + 0x8;
    r7 = 0x0;
    fn_802367CC();
    r28 = r3;
    r3 = r30;
    r4 = r31;
    fn_802364BC();
    r0 = r3 & 0xFFFF;
    r29 = r3;
    if ((s32)r0 == (s32)0) { r3 = 0x0; return; }
    if ((u32)r0 == (u32)0x165) { r3 = 0x0; return; }
    if ((u32)r0 == (u32)0xffff) { r3 = 0x0; return; }
    r3 = r30;
    r4 = r31;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {

        r3 = 0x0;
        return;
    }
    r7 = r28 & 0xFFFF;
    r6 = (u32)sp + 0x8;
    r5 = r29 & 0xFFFF;
    r4 = (u32)sp + 0x1c;
    r3 = 0x0;
    r10 = 0x0;
    while (1) {
        r0 = r10 & 0xFF;
        if ((s32)r0 >= (s32)r7) break;
        r9 = *(s16*)(r6 + r8);
        r0 = (s16)r9;
        if ((u32)r0 < (u32)0x1) goto L_80257DAC;
        r0 = *(u16*)(r4 + r8);
        if ((u32)r5 != (u32)r0) goto L_80257DAC;
        r3 = r30;
        r4 = r31;
        r5 = r9 & 0xFF;
        fn_802381C4();
        break;
    L_80257DAC:
        r10 = r10 + 0x1;


    }

    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)r7) {
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

/* Address: 0x80257DF8 | Size: 0xB4 */
void fn_80257DF8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80257EAC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80257EAC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80257EE4 | Size: 0xE4 (228 bytes) */
void fn_80257EE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80218FDC();
    extern void fn_8021901C();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
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
    r7 = 0x1;
    r5 = (u32)sp + 0x8;
    r28 = r3;
    r27 = r4;
    fn_802367CC();
    r29 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r30 = (u32)sp + 0x8;
    r31 = r29 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r27 = *(u16*)(r30 + r0);
        if ((u32)r27 == (u32)0x165 || (u32)r27 == (u32)0x163) goto L_80257F88;

        r3 = r27;
        fn_80218FDC();
        r0 = r3 & 0xFF;
        if ((u32)r27 != (u32)0x163 || (u32)r27 == (u32)0x108 || (u32)r27 == (u32)0xfd) goto L_80257F88;


        r3 = r27;
        fn_8021901C();
        r0 = r3 & 0xFF;
        if ((u32)r27 == (u32)0xfd) break;
    L_80257F88:
        r28 = r28 + 0x1;


    }

    r3 = r28 & 0xFFFF;
    r0 = r29 & 0xFFFF;
    if ((u32)r3 >= (u32)r0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80257FC8 | Size: 0xF4 (244 bytes) */
void fn_80257FC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80219270();
    extern void fn_80236458();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x0;
    r5 = (u32)sp + 0x8;
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = 0x10;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) { r3 = 0x0; return; }
    r3 = r30;
    fn_80219270();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {

        r3 = 0x0;
        return;
    }
    r5 = r30 & 0xFFFF;
    r4 = (u32)sp + 0x8;
    r0 = r31 & 0xFFFF;
    r6 = 0x0;
    while (1) {
        r3 = r6 & 0xFFFF;
        if ((u32)r3 >= (u32)r0) break;
        r3 = *(u16*)(r4 + r3);
        if ((u32)r5 == (u32)r3) {
            r3 = 0x0;
            return;
        }
        r6 = r6 + 0x1;


    }
    r3 = 0x1;

    return;
}


/* -------------------------------------------------------------------
 * Team State Updates (0x80258000-0x8025C000)
 * 78 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802580BC | Size: 0x78 | Pattern: field_accessor */
u32 fn_802580BC(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = (0x1 << 16);
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x8025813C | Size: 0xD0 (208 bytes) */
void fn_8025813C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
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
    r4 = r31;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025820C | Size: 0x78 | Pattern: field_accessor */
u32 fn_8025820C(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r7 = (0x1 << 16);
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x80258284 | Size: 0x140 (320 bytes) */
void fn_80258284(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80219804();
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_802381C4();
    extern void fn_8025C264();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x0;
    r31 = r6;
    r26 = r4;
    r29 = r5;
    r30 = r3;
    r4 = r31;
    r5 = (u32)sp + 0x1c;
    r6 = (u32)sp + 0x8;
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r7 = (u32)sp + 0x8;
    r6 = r27 & 0xFFFF;
    r5 = (u32)sp + 0x1c;
    r0 = r28 & 0xFFFF;
    r3 = 0x0;
    r10 = 0x0;
    while (1) {
        r4 = r10 & 0xFFFF;
        if ((u32)r4 >= (u32)r0) break;
        r9 = *(s16*)(r7 + r8);
        r4 = (s16)r9;
        if ((u32)r0 < (u32)0x1) goto L_80258354;
        r4 = *(u16*)(r5 + r8);
        if ((u32)r6 != (u32)r4) goto L_80258354;
        r3 = r30;
        r4 = r31;
        r5 = r9 & 0xFF;
        fn_802381C4();
        break;
    L_80258354:
        r10 = r10 + 0x1;


    }

    r0 = r3 & 0xFF;
    if ((u32)r4 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x2a;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r29 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802583C4 | Size: 0xB0 */
void fn_802583C4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80258474 | Size: 0xB0 */
void fn_80258474(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80258524 | Size: 0xB0 */
void fn_80258524(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x802585D4 | Size: 0x120 (288 bytes) */
void fn_802585D4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_802381C4();
    extern void fn_8025C264();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
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
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r7 = 0x0;
    r31 = r6;
    r26 = r4;
    r29 = r5;
    r30 = r3;
    r4 = r31;
    r5 = (u32)sp + 0x1c;
    r6 = (u32)sp + 0x8;
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
    r7 = (u32)sp + 0x8;
    r6 = r28 & 0xFFFF;
    r5 = (u32)sp + 0x1c;
    r0 = r27 & 0xFFFF;
    r3 = 0x0;
    r10 = 0x0;
    while (1) {
        r4 = r10 & 0xFFFF;
        if ((u32)r4 >= (u32)r0) break;
        r9 = *(s16*)(r7 + r8);
        r4 = (s16)r9;
        if ((s32)r0 < (s32)0) goto L_80258684;
        r4 = *(u16*)(r5 + r8);
        if ((u32)r6 != (u32)r4) goto L_80258684;
        r3 = r30;
        r4 = r31;
        r5 = r9 & 0xFF;
        fn_802381C4();
        break;
    L_80258684:
        r10 = r10 + 0x1;


    }

    r0 = r3 & 0xFF;
    if ((u32)r4 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x29;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r29 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802586FC | Size: 0xBC */
void fn_802586FC(void* ctx, u32 param1, u32 param2) {
    extern void fn_80236BFC();
    extern void fn_80237DBC();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r7 = 0x0;
    r29 = r3;
    r30 = r6;
    fn_8025C264();
    r31 = r3;
    r3 = r29;
    r4 = r30;
    r5 = 0x1c;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r29;
    r4 = r30;
    r5 = 0xc;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r29;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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

/* Address: 0x802587C0 | Size: 0x144 (324 bytes) */
void fn_802587C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021A2C0();
    extern void fn_802364BC();
    extern void fn_802367CC();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    u8 sp[0x40];
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

    r7 = 0x0;
    r28 = r5;
    r31 = r6;
    r26 = r3;
    r27 = r4;
    r5 = (u32)sp + 0x8;
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r7 = (0x1 << 16);
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = r31;
    fn_8025C264();
    r31 = r3;
    r3 = r26;
    r4 = r27;
    r5 = 0x10;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) { r3 = 0x0; return; }
    r3 = r29;
    fn_8021A2C0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) { r3 = 0x0; return; }
    r5 = r29 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) { r3 = 0x0; return; }
    if ((u32)r5 == (u32)0xffff) { r3 = 0x0; return; }
    if ((u32)r5 == (u32)0x165) { r3 = 0x0; return; }
    if ((u32)r5 == (u32)0x163) {

        r3 = 0x0;
        return;
    }
    r4 = (u32)sp + 0x8;
    r0 = r30 & 0xFFFF;
    r6 = 0x0;
    while (1) {
        r3 = r6 & 0xFFFF;
        if ((u32)r3 >= (u32)r0) break;
        r3 = *(u16*)(r4 + r3);
        if ((u32)r5 == (u32)r3) {
            r3 = 0x0;
            return;
        }
        r6 = r6 + 0x1;


    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258904 | Size: 0xB0 */
void fn_80258904(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x802589B4 | Size: 0xB0 */
void fn_802589B4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x80258A64 | Size: 0x6C | Pattern: field_accessor */
u32 fn_80258A64(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047E650[];
    extern void fn_80236BFC();
    extern void fn_802373B0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    u32 r4 = slot;
    u32 r5 = param;

    r5 = 0x14;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
    } else {

        f1 = *(f32*)(u32)lbl_8047E650;
        r3 = r30;
        r4 = r31;
        r5 = -0x1;
        fn_802373B0();
        r0 = r3 & 0xFF;
        r3 = 0x1 - r0;
        r3 = r3 - r0; /* -borrow */;
    }
    return;
}

/* Address: 0x80258AD0 | Size: 0xB4 */
void fn_80258AD0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258B84 | Size: 0xB4 */
void fn_80258B84(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258C38 | Size: 0xB4 */
void fn_80258C38(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258CEC | Size: 0xB4 */
void fn_80258CEC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258DA0 | Size: 0xB4 */
void fn_80258DA0(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258E54 | Size: 0xB4 */
void fn_80258E54(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258F08 | Size: 0xB4 */
void fn_80258F08(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80258FBC | Size: 0xB4 */
void fn_80258FBC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259070 | Size: 0xB4 */
void fn_80259070(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259124 | Size: 0xB4 */
void fn_80259124(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802591D8 | Size: 0x260 (608 bytes) */
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_80259328;
    }
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
L_80259324:
    r0 = 0x1;
L_80259328:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x5;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
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
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r5;
    r31 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x11;
    r4 = r31;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x8;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
    } else {

        r3 = r28;
        r4 = r31;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r28;
            r4 = r31;
            r5 = 0x7;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r28;
                r4 = r31;
                r5 = 0xf;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x1) {
                    r3 = r28;
                    r4 = r31;
                    r5 = 0x48;
                    fn_80237F74();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x1) {
                        r3 = r28;
                        r4 = r31;
                        r5 = 0x29;
                        fn_80237F74();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 != (u32)0x1) {
                            r3 = r28;
                            r4 = r31;
                            r5 = 0x28;
                            fn_80237F74();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 != (u32)0x1) {
                                r3 = r28;
                                r4 = r31;
                                r5 = 0xc;
                                fn_80237F74();
        }
        }
        }
        }
        }
        }
        r0 = 0x1;
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r28 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802596A8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802596A8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802596E4 | Size: 0xE8 (232 bytes) */
void fn_802596E4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x5;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802597CC | Size: 0xE8 (232 bytes) */
void fn_802597CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x3;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x802598B4 | Size: 0xE8 (232 bytes) */
void fn_802598B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x2;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025999C | Size: 0xE8 (232 bytes) */
void fn_8025999C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0xa0;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0xa0;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259A84 | Size: 0x68 | Pattern: field_accessor */
u32 fn_80259A84(void* ctx, u32 slot, u32 param) {
    extern void fn_80236BFC();
    extern void fn_80237288();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r5 = 0x10;
    r31 = r6;
    r30 = r3;
    r4 = r31;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) { r3 = 0x0; return; }
    r3 = r30;
    r4 = r31;
    fn_80237288();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) { r3 = 0x1; return; }

    r3 = 0x0;
    return;

    r3 = 0x1;

    return;
}

/* Address: 0x80259AEC | Size: 0xE8 (232 bytes) */
void fn_80259AEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x5;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259BD4 | Size: 0xE8 (232 bytes) */
void fn_80259BD4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x4;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259CBC | Size: 0xE8 (232 bytes) */
void fn_80259CBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x3;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259DA4 | Size: 0xE8 (232 bytes) */
void fn_80259DA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259E8C | Size: 0xE8 (232 bytes) */
void fn_80259E8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x20;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x20;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x80259F74 | Size: 0x1F8 (504 bytes) */
void fn_80259F74(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F025C();
    extern void fn_801F6E98();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r31;
    r5 = 0x9;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
        goto L_8025A0F4;
    }
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
L_8025A0F0:
    r0 = 0x1;
L_8025A0F4:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
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
void fn_8025A16C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A220 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8025A220(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A254 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025A254(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A340 | Size: 0xB4 */
void fn_8025A340(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A3F4 | Size: 0xB4 */
void fn_8025A3F4(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A4A8 | Size: 0xB4 */
void fn_8025A4A8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A55C | Size: 0xB0 */
void fn_8025A55C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x8025A60C | Size: 0xB0 */
void fn_8025A60C(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x8025A6BC | Size: 0xB4 */
void fn_8025A6BC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A770 | Size: 0x100 (256 bytes) */
void fn_8025A770(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80237774();
    extern void fn_8023793C();
    extern void fn_80237F74();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
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
    if ((u32)r0 == (u32)0x43) {
        r29 = 0x0;
    }
    r3 = r25;
    r4 = r27;
    r5 = 0x5;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r29 = 0x0;
    }
    r3 = r30 & 0xFF;
    r0 = r31 & 0xFF;
    if ((u32)r3 < (u32)r0) {
        r29 = 0x0;
    }
    if ((s32)r29 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x8025A870 | Size: 0x8C */
void fn_8025A870(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_8047E648[];
    extern void fn_80236BFC();
    extern void fn_802373B0();
    extern void fn_8025CC90();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0x8;
    r30 = r3;
    r31 = r4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = r31;
    fn_8025CC90();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    f1 = *(f32*)(u32)lbl_8047E648;
    r3 = r30;
    r4 = r31;
    r5 = 0x0;
    fn_802373B0();
    r0 = r3 & 0xFF;
    r3 = 0x1 - r0;
    r3 = r3 - r0; /* -borrow */;

    return;
}

/* Address: 0x8025A8FC | Size: 0xB4 */
void fn_8025A8FC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025A9B0 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025A9B0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A9EC | Size: 0xB4 */
void fn_8025A9EC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025AAA0 | Size: 0x270 (624 bytes) */
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
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r30 = r5;
    r31 = r6;
    r29 = r4;
    r28 = r3;
    r5 = 0x11;
    r4 = r31;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x14;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x4;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x3;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x8;
    fn_80237DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r31;
    r5 = 0x11;
    fn_80237F74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r0 = 0x0;
    } else {

        r3 = r28;
        r4 = r31;
        r5 = 0x14;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r28;
            r4 = r31;
            r5 = 0x7;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r28;
                r4 = r31;
                r5 = 0xf;
                fn_80237F74();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x1) {
                    r3 = r28;
                    r4 = r31;
                    r5 = 0x48;
                    fn_80237F74();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x1) {
                        r3 = r28;
                        r4 = r31;
                        r5 = 0x29;
                        fn_80237F74();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 != (u32)0x1) {
                            r3 = r28;
                            r4 = r31;
                            r5 = 0x28;
                            fn_80237F74();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 != (u32)0x1) {
                                r3 = r28;
                                r4 = r31;
                                r5 = 0xc;
                                fn_80237F74();
        }
        }
        }
        }
        }
        }
        r0 = 0x1;
    }
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((s32)r28 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025AD10 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025AD10(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025AD48 | Size: 0xB4 */
void fn_8025AD48(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025ADFC | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_8025ADFC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025AE28 | Size: 0xB0 */
void fn_8025AE28(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x8025AED8 | Size: 0xE0 (224 bytes) */
void fn_8025AED8(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_801F4354();
    extern void fn_801F87CC();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    extern void fn_8025C264();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r30;
    r5 = 0x25;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = 0x0;
    fn_8025C264();
    r0 = r3;
    r3 = r31;
    r31 = r0;
    r4 = (u32)sp + 0x8;
    fn_801F87CC();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025AFB8 | Size: 0xB0 */
void fn_8025AFB8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x8025B068 | Size: 0xB4 */
void fn_8025B068(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B124 | Size: 0xE8 (232 bytes) */
void fn_8025B124(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x7;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B20C | Size: 0xE8 (232 bytes) */
void fn_8025B20C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x6;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B2F4 | Size: 0xE8 (232 bytes) */
void fn_8025B2F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x3;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B3DC | Size: 0xE8 (232 bytes) */
void fn_8025B3DC(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x2;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B4C4 | Size: 0xE8 (232 bytes) */
void fn_8025B4C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x90;
    r8 = 0x1;
    r9 = 0x1;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x90;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B5AC | Size: 0xB4 */
void fn_8025B5AC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B660 | Size: 0xE8 (232 bytes) */
void fn_8025B660(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x7;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B748 | Size: 0xE8 (232 bytes) */
void fn_8025B748(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x4;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B830 | Size: 0xE8 (232 bytes) */
void fn_8025B830(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x2;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025B918 | Size: 0xE8 (232 bytes) */
void fn_8025B918(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8021C034();
    extern void fn_80236BFC();
    extern void fn_8025C264();
    extern void fn_8025C808();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0) {
        r3 = r27;
        r4 = r30;
        r5 = 0x14;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
        }
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        r7 = 0x0;
        fn_8025C264();
        r31 = r3;
    }
    r3 = r27;
    r4 = r28;
    r5 = r30;
    r6 = r29;
    r7 = 0x10;
    r8 = 0x1;
    r9 = 0x41;
    fn_8025C808();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x10;
    fn_8021C034();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) {
        if ((s32)r31 == (s32)0x0) {
            r3 = 0x0;
            return;
        }
        if ((s32)r31 == (s32)-0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025BA00 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_8025BA00(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025BA2C | Size: 0xF4 (244 bytes) */
void fn_8025BA2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_80236BFC();
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r27;
    r4 = r29;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x43) {
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

/* Address: 0x8025BB20 | Size: 0x108 (264 bytes) */
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
    if ((u32)r0 == (u32)0x1) {
        r0 = r31 & 0xFFFF;
        if ((u32)r0 <= (u32)0x1) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r26;
    fn_8025CBE8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
    if ((u32)r0 == (u32)0x43) {
        r3 = 0x0;
        return;
    }
    r3 = r26;
    r4 = r27;
    r5 = r28;
    r6 = r29;
    r7 = 0x0;
    fn_8025C264();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025BC28 | Size: 0xB4 */
void fn_8025BC28(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025BCDC | Size: 0xB4 */
void fn_8025BCDC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025BD90 | Size: 0xB4 */
void fn_8025BD90(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025BE44 | Size: 0xB4 */
void fn_8025BE44(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025BEF8 | Size: 0xB4 */
void fn_8025BEF8(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r30 = 0x0;
    }
    if ((s32)r30 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((s32)r30 == (s32)-0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025BFAC | Size: 0x200 (512 bytes) */
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
    r5 = 0x14;
    r4 = r30;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    r5 = 0x8;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    fn_8025CC90();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r28;
    r4 = r30;
    fn_80237310();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 != (u32)0x1) {
        r3 = r28;
        r4 = r30;
        r5 = 0x48;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
        }
        r0 = 0x0;
        goto L_8025C12C;
        }
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
L_8025C128:
    r0 = 0x1;
L_8025C12C:
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r0 == (u32)0x1) {
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


/* -------------------------------------------------------------------
 * Shadow Pokemon & Purification (0x8025C000-0x80260000)
 * 89 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8025C1AC | Size: 0xB0 */
void fn_8025C1AC(void* ctx, u32 param1, u32 param2) {
    extern void fn_8023793C();
    extern void fn_80239500();
    extern void fn_802395C8();
    extern void fn_8025C264();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x43) {
        r31 = 0x0;
    }
    if ((s32)r31 == (s32)0x0) {
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* Address: 0x8025C264 | Size: 0x340 (832 bytes) */
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
    fn_8011BEB4();
    r30 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) {
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
        if ((u32)r3 >= (u32)r0) break;
        r3 = *(u32*)(r4 + r3);
        if ((u32)r3 != (u32)0x0) {
            if ((u32)r26 == (u32)r3) {
                r6 = r5;
            }
            if ((u32)r28 == (u32)r3) {
                r7 = r5;
        }
        }
        r5 = r5 + 0x1;


    }
    r3 = r6 & 0xFFFF;
    r0 = r7 & 0xFFFF;
    r0 = r3 - r0;
    r24 = (u32)r0 >> 31;

    if ((u32)r29 != (u32)0xffff && (u32)r29 != (u32)0xfffe) goto L_8025C420;

    if ((u32)r29 == (u32)0xffff) {
        r3 = r25;
        r4 = r28;
        r5 = 0x1d;
        fn_80236BFC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = r28;
            r4 = 0x1d;
            fn_80201D84();
            r3 = r3 & 0xFFFF;
            r0 = r31 & 0xFFFF;
            if ((u32)r0 == (u32)r3) {
                r3 = -0x1;
                return;
    }
    }
    }
    r3 = r25;
    r4 = r26;
    fn_80237288();
    r0 = r3 & 0xFF;
    if (((u32)r0 == (u32)0x1) && ((u32)r24 == (u32)0x1)) {

        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = r27;
    fn_8025CAA8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;
    return;
L_8025C420:
    r3 = r27;
    r4 = r26;
    r5 = r28;
    fn_80229934();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = r27;
    fn_8025CAA8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = 0x1d;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r28;
        r4 = 0x1d;
        fn_80201D84();
        r3 = r3 & 0xFFFF;
        r0 = r31 & 0xFFFF;
        if ((u32)r3 == (u32)r0) {
            r3 = -0x1;
            return;
    }
    }
    if ((u32)r24 != (u32)0x1) goto L_8025C558;
    r3 = r25;
    r4 = r28;
    r5 = 0x1f;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        if ((u32)r30 != (u32)0x92) {
            if ((u32)r30 != (u32)0x95) {
                if ((u32)r30 != (u32)0x98) {
                    if ((u32)r30 != (u32)0xcf) {
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
    if (((u32)r0 == (u32)0x1) && ((u32)r30 != (u32)0x93)) {

        r3 = 0x0;
        return;
    }
    r3 = r25;
    r4 = r28;
    r5 = 0x21;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8025C558;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 == (u32)0x39 || (u32)r0 == (u32)0xfa) goto L_8025C558;

    r3 = 0x0;
    return;
L_8025C558:
    r3 = r27;
    fn_80229B70();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = -0x1;
        return;
    }
    r3 = r27;
    fn_80229BD8();
    r0 = r3 & 0xFF;
    r3 = 0x1;
    if ((u32)r0 != (u32)0x1) return;
    r3 = -0x1;

    return;
}

/* Address: 0x8025C5A4 | Size: 0xD0 (208 bytes) */
void fn_8025C5A4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;
    u32 r6 = param3;

    r3 = 0x0;
    r6 = 0x0;
    r31 = r5;
    r30 = r4;
    r5 = 0x9;
    r4 = r31;
    fn_8011BEB4();
    r0 = r30 & 0xFFFF;
    r3 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1f) goto L_8025C608;
    if ((u32)r3 == (u32)0x92) { r3 = 0x1; return; }
    if ((u32)r3 == (u32)0x95) { r3 = 0x1; return; }
    if ((u32)r3 == (u32)0x98) { r3 = 0x1; return; }
    if ((u32)r3 != (u32)0xcf) goto L_8025C608;

    r3 = 0x1;
    return;
L_8025C608:
    r0 = r30 & 0xFFFF;
    if (((u32)r0 == (u32)0x20) && ((u32)r3 == (u32)0x93)) {

        r3 = 0x1;
        return;
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x21) goto L_8025C64C;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x39) { r3 = 0x1; return; }
    if ((u32)r0 != (u32)0xfa) goto L_8025C64C;

    r3 = 0x1;
    return;
L_8025C64C:
    if ((u32)r3 == (u32)0x5e) {
        r3 = 0x1;
        return;
    }
    r3 = 0x0;

    return;
}

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
void fn_8025C6BC(void* ctx, u32 param1, u32 param2) {
    extern void fn_801F8424();
    extern void fn_802062FC();
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
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = r28;
    fn_801F8424();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = r30;
    r4 = r31;
    r5 = 0x1e;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r30;
        r4 = r31;
        r5 = 0x2b;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = *(u32*)((u8*)r29 + 0x4);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r29 + 0x4) = r0;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025C770 | Size: 0x98 */
void fn_8025C770(void* ctx, u32 param1, u32 param2) {
    extern void fn_801FB1C0();
    extern void fn_8021B364();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r31 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;

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
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = (u32)sp + 0x8;
        fn_8021B364();
        r3 = r3 & 0xFF;
        r0 = 0x1;
        r0 = r3 - r0;
        r0 = -0x1;
        r3 = r0 & 0xFF;
    } else {

        r3 = 0x1;
    }
    r31 = *(u32*)(sp + 0x1C);
    return;
}

/* Address: 0x8025C808 | Size: 0x2A0 (672 bytes) */
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
    u32 r3 = (u32)ctx;
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
    if ((s32)r0 < (s32)0) {
        r3 = r21;
        r4 = 0x4c;
        fn_801F6E98();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = r24 & 0xFFFF;
                if ((u32)r0 != (u32)0xae) {
                    r3 = 0x0;
                    return;
        }
        }
        }
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
    L_8025C968:
        r0 = 0x0;
    L_8025C96C:
        r0 = r0 & 0xFF;
        if ((u32)r0 != (u32)0x1) goto L_8025C980;
        r3 = 0x0;
        return;
    L_8025C980:
        r3 = r22;
        r4 = r30;
        r5 = 0x1d;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            r3 = r22;
            r4 = r30;
            r5 = 0x49;
            fn_80237F74();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) goto L_8025C9D4;
        }
        r0 = r28 & 0xFF;
        if ((u32)r0 != (u32)0x1) goto L_8025C9D4;
        r0 = r24 & 0xFFFF;
        if ((u32)r0 == (u32)0xae) goto L_8025C9D4;
        r3 = 0x0;
        return;
    L_8025C9D4:
        r3 = r22;
        r4 = r30;
        r5 = 0x33;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = r29 & 0xFFFF;
                if ((u32)r0 == (u32)0xeb) {
                    r3 = 0x0;
                    return;
        }
        }
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x34;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = r29 & 0xFFFF;
                if ((u32)r0 == (u32)0xe6) {
                    r3 = 0x0;
                    return;
        }
        }
        }
        r3 = r22;
        r4 = r30;
        r5 = 0x13;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r31 & 0x1F;
            if ((u32)r0 == (u32)0x1) {
                r3 = 0x0;
                return;
        }
        }
        r0 = (s8)r26;
        if ((u32)r0 > (u32)0x1) { r3 = 0x1; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r26 < (s32)0xc) { r3 = 0x1; return; }
    r3 = 0x0;
    return;

    r3 = 0x1;

    return;
}

/* Address: 0x8025CAA8 | Size: 0x94 */
void fn_8025CAA8(void* ctx, u32 param1, u32 param2) {
    extern void fn_801FB1C0();
    extern void fn_80229C28();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;
    u32 r5 = param2;

    r6 = 0x0;
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        r4 = r31;
        fn_80229C28();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x0;

    return;
}

/* Address: 0x8025CB3C | Size: 0xAC */
void fn_8025CB3C(void* ctx, u32 param1, u32 param2) {
    extern void fn_800E0C54();
    extern void fn_80201248();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r31 = 0;
    f32 f7 = 0.0f;
    u32 r1 = (u32)sp;
    u32 r4 = param1;
    u32 r5 = param2;

    r5 = 0xf7;
    r6 = 0x0;
    r31 = r4;
    r4 = 0x0;
    r3 = r31;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {
        if ((u32)r3 != (u32)0x165) {
            if ((u32)r3 != (u32)0xffff) {
                r31 = *(u32*)(sp + 0x1C);
                return;
    }
    }
    }
    r3 = r31;
    r4 = (u32)sp + 0x8;
    fn_80201248();
    r0 = r3 & 0xFF;
    r31 = r3;
    if ((u32)r3 != (u32)0xffff) {
        fn_800E0C54();
        r5 = r3 & 0xFFFF;
        r4 = r31 & 0xFF;
        r0 = (s32)r5 / (s32)r4;
        r3 = (u32)sp + 0x8;
        r0 = r0 * r4;
        r0 = r5 - r0;
        r3 = *(u16*)(r3 + r0);
        if ((u32)r3 != (u32)0x0) {
            if ((u32)r3 != (u32)0x165) {
                r31 = *(u32*)(sp + 0x1C);
                return;
    }
    }
    }
    r3 = 0x0;

    r31 = *(u32*)(sp + 0x1C);
    return;
}

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
u32 fn_8025CC30(void* ctx, u32 slot, u32 param) {
    extern void fn_802062FC();
    extern void fn_80237F74();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r30 = r3;
    r31 = *(u32*)((u8*)r5 + 0x0);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
    } else {

        r3 = r31;
        r4 = r30;
        r5 = 0x6;
        fn_80237F74();
        r0 = r3 & 0xFF;
        r3 = 0x1 - r0;
        r3 = r3 - r0; /* -borrow */;
    }
    return;
}

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
u32 fn_8025CCE0(void* ctx, u32 slot, u32 param) {
    extern void fn_802062FC();
    extern void fn_80236BFC();
    extern void fn_80237F74();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    r29 = r3;
    r31 = *(u32*)((u8*)r5 + 0x0);
    r30 = *(u32*)((u8*)r5 + 0x4);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r3 = r31;
    r4 = r29;
    r5 = 0xb;
    fn_80236BFC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        r5 = 0x2b;
        fn_80237F74();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            return;
    }
    }
    r3 = 0x1;

    return;
}

/* Address: 0x8025CD64 | Size: 0x54 | Pattern: field_accessor */
u32 fn_8025CD64(void* ctx, u32 slot, u32 param) {
    extern u8 lbl_8047B650[];
    extern u32 fn_800E202C();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    u32 handle;
    u32 id;
    handle = *(u32*)lbl_8047B650;
    if (handle != 0) {
        id = fn_800E202C(handle) & 0xFFFF;
        if (id != 0) {
            fn_800E24B0(id);
            fn_800E209C(id);
        }
        *(u32*)lbl_8047B650 = 0;
    }
    return 0;
}

/* Address: 0x8025CDB8 | Size: 0x2B4 (692 bytes) */
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
    r0 = *(u32*)lbl_80478D98;
    *(u32*)lbl_8047B654 = r3;
    r3 = r0 * 0x138;
    r0 = r3 + 0x1f;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {
        fn_800E27B0();
    } else {

        r3 = 0x0;
    }
    *(u32*)lbl_8047B654 = r3;
    r30 = r3;
    r31 = 0x0;
    while (1) {
        r0 = *(u32*)lbl_80478D98;
        if ((s32)r31 >= (s32)r0) break;
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


    }
    r3 = *(u32*)lbl_8047B650;
    if ((u32)r3 != (u32)0x0) {
        fn_800E202C();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if ((u32)r3 != (u32)0x0) {
            fn_800E24B0();
            r3 = r30;
            fn_800E209C();
        }
        r0 = 0x0;
        *(u32*)lbl_8047B650 = r0;
    }
    r3 = 0x80;
    r4 = 0x20;
    fn_800E2C04();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) {
        fn_800E27B0();
    } else {

        r3 = 0x0;
    }
    *(u32*)lbl_8047B650 = r3;
    r31 = 0x0;
    do {
        r3 = r31;
        fn_8006B09C();
        r3 = r3 + 0xb44;
        fn_8012A248();
        r30 = 0x0;
        do {
            r3 = r31;
            fn_8006B09C();
            r4 = r30 & 0xFFFF;
            r3 = r3 + 0xb44;
            fn_8012AC08();
            fn_80124A60();
            r30 = r30 + 0x1;
        } while ((s32)r30 < (s32)0x6);
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x4);
    r30 = 0x0;
    do {
        r3 = r30;
        fn_8006B09C();
        r3 = r3 + 0x2c;
        fn_8012A248();
        r31 = 0x0;
        do {
            r3 = r30;
            fn_8006B09C();
            r4 = r31 & 0xFFFF;
            r3 = r3 + 0x2c;
            fn_8012AC08();
            fn_80124A60();
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
    fn_80129280();
    r30 = r3;
    r3 = 0x0;
    fn_8006B09C();
    r3 = r3 + 0xb44;
    if ((u32)r30 != (u32)0x0) {
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
        }
        r28 = 0x0;
        do {
            r0 = *(u32*)lbl_8047B654;
            r3 = r31;
            r30 = r0 + r29;
            fn_8006B09C();
            r4 = r28 & 0xFFFF;
            r3 = r3 + 0xb44;
            fn_8012AC08();
            if ((u32)r3 != (u32)0x0) {
                r4 = r30;
                r5 = 0x138;
                memcpy((void*)r3, (const void*)r4, (u32)r5);
            }
            r28 = r28 + 0x1;
            r29 = r29 + 0x138;
        } while ((s32)r28 < (s32)0x6);
    L_8025D01C:
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x4);
    r3 = *(u32*)lbl_8047B654;
    if ((u32)r3 != (u32)0x0) {
        fn_800E202C();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if ((u32)r3 != (u32)0x0) {
            fn_800E24B0();
            r3 = r30;
            fn_800E209C();
        }
        r0 = 0x0;
        *(u32*)lbl_8047B654 = r0;
    }
    return;
}

/* Address: 0x8025D06C | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025D06C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025D0A8 | Size: 0xBC */
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
    u32 r3 = (u32)ctx;
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

    r30 = 0x0;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        r4 = 0x2;
        fn_80129280();
        r28 = r3;
    }
    r31 = 0x0;
    do {
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
    L_8025D108:
        r5 = *(u16*)(r4 + r3);
        if ((u32)r5 == (u32)0x0) goto L_8025D128;
        if ((u32)r0 == (u32)r5) {
            r30 = r30 + 0x1;
        }
        r3 = r3 + 0x2;
        goto L_8025D108;
    L_8025D128:
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x6);
    f1 = *(f32*)(u32)lbl_8047E658;
    f0 = *(f32*)(u32)lbl_8047E65C;
    ctr_fn = (void(*)(void))r30;
    if ((s32)r30 > (s32)0x0) {
        do {
            f1 = f1 * f0;
        } while (--ctr != 0);
    }
    return;
}

/* Address: 0x8025D164 | Size: 0x128 (296 bytes) */
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
    L_8025D1CC:
        r5 = *(u16*)(r4 + r3);
        if ((u32)r5 == (u32)0x0) goto L_8025D1EC;
        if ((u32)r0 == (u32)r5) {
            r28 = r28 + 0x1;
        }
        r3 = r3 + 0x2;
        goto L_8025D1CC;
    L_8025D1EC:
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x6);
    f1 = *(f32*)(u32)lbl_8047E658;
    f0 = *(f32*)(u32)lbl_8047E65C;
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
u16 fn_8025D28C(void* ctx) { return *(u16*)fn_8006B09C(ctx); }

/* Address: 0x8025D2B0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D2B0(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x24); }

/* Address: 0x8025D2D4 | Size: 0x90 */
void fn_8025D2D4(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_80478E04[];
    extern void fn_8006B09C();
    extern void fn_801FCBA4();
    extern void fn_801FCCC4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    u32 r4 = param1;

    r31 = r4;
    fn_8006B09C();
    r3 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    fn_801FCCC4();
    fn_801FCBA4();
    r0 = r3 * 0x14;
    r3 = *(u32*)lbl_80478E04;
    r3 = r3 + r0;
    if ((s32)r31 == (s32)0x0) {
        r0 = *(u32*)((u8*)r3 + 0xC);
        r3 = (0xf94 << 16);
        r3 = r3 + 0x1200;
        if ((u32)r0 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
        r3 = r0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    r0 = *(u32*)((u8*)r3 + 0x10);
    r3 = (0xf8f << 16);
    r3 = r3 + 0x1200;
    if ((u32)r0 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
    r3 = r0;

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* Address: 0x8025D364 | Size: 0x90 */
void fn_8025D364(void* ctx, u32 param1, u32 param2) {
    extern u8 lbl_80478E04[];
    extern void fn_8006B09C();
    extern void fn_801FCBA4();
    extern void fn_801FCCC4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r31 = 0;
    u32 r4 = param1;

    r31 = r4;
    if ((s32)r3 != (s32)0x0) {
        fn_8006B09C();
        r0 = *(u16*)((u8*)r3 + 0x0);
    } else {

        fn_8006B09C();
        r0 = *(u16*)((u8*)r3 + 0x0);
    }
    r3 = r0 & 0xFFFF;
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    fn_801FCCC4();
    fn_801FCBA4();
    r0 = r3 * 0x14;
    r3 = *(u32*)lbl_80478E04;
    r3 = r3 + r0;
    if ((s32)r31 == (s32)0x0) {
        r3 = *(u32*)((u8*)r3 + 0x4);
        r31 = *(u32*)(sp + 0xC);
        return;
    }
    r0 = *(u32*)((u8*)r3 + 0x8);
    r3 = (0xf99 << 16);
    r3 = r3 + 0x1200;
    if ((u32)r0 == (u32)0x0) { r31 = *(u32*)(sp + 0xC); return; }
    r3 = r0;

    r31 = *(u32*)(sp + 0xC);
    return;
}

/* Address: 0x8025D3F4 | Size: 0x16C (364 bytes) */
void fn_8025D3F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
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
        fn_8012AC08();
        fn_80123FBC();
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
    if ((u32)r0 < (u32)r28) {
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
        if (((u32)r3 != (u32)0x0) && ((u32)r28 != (u32)0x0)) {

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
u32 fn_8025D560(void* ctx) { return *(u32*)((u8*)fn_8006B09C(ctx) + 0x20); }

/* Address: 0x8025D584 | Size: 0x5C | Pattern: field_accessor */
u32 fn_8025D584(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r4 = slot;
    u32 r5 = param;

    fn_8006B09C();
    r4 = *(u32*)((u8*)r3 + 0x20);
    if ((s32)r0 < (s32)0) { r3 = 0x0; return; }
    if ((s32)r0 > (s32)0x6) {

        r3 = 0x0;
        return;
    }
    r0 = r0 << 2;
    r5 = -0x1;
    r4 = r3 + r0;
    *(u32*)((u8*)r4 + 0x8) = r5;
    r4 = *(u32*)((u8*)r3 + 0x20);
    *(u32*)((u8*)r3 + 0x20) = r0;
    r3 = *(u32*)((u8*)r3 + 0x20);

    return;
}

/* Address: 0x8025D5E0 | Size: 0x64 | Pattern: field_accessor */
u32 fn_8025D5E0(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r4;
    r31 = r5;
    fn_8006B09C();
    r6 = 0x0;
    r4 = 0x0;
    ctr_fn = (void(*)(void))r30;
    if ((s32)r30 > (s32)0x0) {
        do {
            r5 = *(u32*)(r31 + r4);
            r0 = r4 + 0x8;
            r6 = r6 + 0x1;
            r4 = r4 + 0x4;
            *(u32*)(r3 + r0) = r5;
        } while (--ctr != 0);
    }
    *(u32*)((u8*)r3 + 0x20) = r6;
    r3 = r6;
    return;
}

/* Address: 0x8025D644 | Size: 0x100 (256 bytes) */
void fn_8025D644(void* ctx, u32 param1, u32 param2, u32 param3) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r29 = r4;
    r25 = r3;
    fn_8006B09C();
    r31 = r3;
    r30 = *(u32*)((u8*)r3 + 0x20);
    fn_8006B1D4();
    r26 = 0x0;
    r28 = r3 & 0xFFFF;
    r27 = r26;
    do {
        r3 = r25;
        fn_8006B09C();
        r4 = r27 & 0xFFFF;
        r3 = r3 + 0xb44;
        fn_8012AC08();
        fn_80123FBC();
        r3 = r3 & 0xFF;
        r0 = r3 - r0; /* -borrow */;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r26 & 0xFFFF;
            r0 = r3 + 0x1;
            r26 = r0 & 0xFFFF;
        }
        r27 = r27 + 0x1;
    } while ((s32)r27 < (s32)0x6);
    r0 = r26 & 0xFFFF;
    r3 = *(u32*)((u8*)r31 + 0x20);
    if ((u32)r0 < (u32)r28) {
        r28 = r26;
    }
    r0 = r28 & 0xFFFF;
    if ((s32)r3 >= (s32)r0) {
        r3 = -0x1;
        return;
    }
    r3 = 0x0;
    ctr_fn = (void(*)(void))r30;
    if ((s32)r30 > (s32)0x0) {
        do {
            r0 = r3 + 0x8;
            r0 = *(u32*)(r31 + r0);
            if ((s32)r0 == (s32)r29) {
                r3 = -0x1;
                return;
            }
            r3 = r3 + 0x4;
        } while (--ctr != 0);
    }
    r0 = r30 << 2;
    r3 = r30;
    r4 = r31 + r0;
    *(u32*)((u8*)r4 + 0x8) = r29;
    r4 = *(u32*)((u8*)r31 + 0x20);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r31 + 0x20) = r0;

    return;
}

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
u32 fn_8025D788(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;
    u32 r5 = param;

    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r29 = 0x0;
    r31 = 0x163;
    do {
        r3 = r29;
        fn_8006B09C();
        r30 = r3 + 0xb44;
        r3 = r29;
        fn_8006B09C();
        r3 = r3 + 0x2c;
        if (((u32)r30 != (u32)0x0) && ((u32)r3 != (u32)0x0)) {

            ctr_fn = (void(*)(void))r31;
            do {
                r3 = *(u32*)((u8*)r4 + 0x4);
                r0 = *(u32*)((u8*)r4 + 0x8);
                *(u32*)((u8*)r5 + 0x4) = r3;
                r5 += 8; *(u32*)r5 = r0;
            } while (--ctr != 0);
        }
        r29 = r29 + 0x1;
    } while ((s32)r29 < (s32)0x4);
    return;
}

/* Address: 0x8025D808 | Size: 0x94 */
void fn_8025D808(void* ctx, u32 param1, u32 param2) {
    extern void fn_8006B09C();
    extern void fn_8006B1D4();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = param1;

    r28 = r3;
    fn_8006B1D4();
    r30 = 0x0;
    r29 = r28;
    r31 = r30;
    r28 = r3 & 0xFFFF;
    do {
        r3 = r29;
        fn_8006B09C();
        r4 = r31 & 0xFFFF;
        r3 = r3 + 0xb44;
        fn_8012AC08();
        fn_80123FBC();
        r3 = r3 & 0xFF;
        r0 = r3 - r0; /* -borrow */;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30 & 0xFFFF;
            r0 = r3 + 0x1;
            r30 = r0 & 0xFFFF;
        }
        r31 = r31 + 0x1;
    } while ((s32)r31 < (s32)0x6);
    r0 = r30 & 0xFFFF;
    r3 = r28;
    if ((u32)r0 < (u32)r28) {
        r3 = r30;
    }
    return;
}

/* Address: 0x8025D89C | Size: 0x78 | Pattern: field_accessor */
u32 fn_8025D89C(void* ctx, u32 slot, u32 param) {
    extern void fn_8006B09C();
    extern void fn_80123FBC();
    extern void fn_8012AC08();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = (u32)ctx;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    u32 r4 = slot;

    r30 = 0x0;
    r31 = r3;
    r29 = 0x0;
    do {
        r3 = r31;
        fn_8006B09C();
        r4 = r29 & 0xFFFF;
        r3 = r3 + 0xb44;
        fn_8012AC08();
        fn_80123FBC();
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
    r3 = r30;
    return;
}

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

/* fn_8025DA3C | Size: 0x4C | Get battle party size based on mode */
u32 fn_8025DA3C(void) {
    extern void* fn_8006B5A8(void);
    void* result = fn_8006B5A8();
    s32 mode = *(s32*)((u8*)result + 0x4);
    switch (mode) {
        case 0:
        case 1:
            return 2;
        case 2:
            return 4;
        default:
            return 2;
    }
}

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
u32 fn_8025DBD4(u32 idx) {
    extern u8 lbl_8027A450[];
    extern u8 lbl_8039A690[];
    extern u8 lbl_80478E08[];
    extern u8 lbl_80478E0C[];
    extern void fn_800DD970();

    u32 count;
    u32* table;
    count = *(u32*)*(u32*)lbl_80478E08;
    if (idx >= count) {
    fn_800DD970(lbl_8027A450, lbl_8039A690);
    return 0;
    }
    table = (u32*)*(u32*)lbl_80478E0C;
    return table[idx];
}
