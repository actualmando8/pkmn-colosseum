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
#pragma peephole off
void fn_80240BD0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* BattleOrchestrator - 2704 bytes */
}
#pragma peephole reset

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
#pragma peephole off
void fn_80245FC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* MultiBattleSetup - 4228 bytes */
}
#pragma peephole reset

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
#pragma peephole off
void fn_8024E690(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* PostBattleProcessing - 4644 bytes */
}
#pragma peephole reset

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
#pragma peephole off
void fn_8025A290(void* trainerCtx, u32 trainerSlot, u32 resultSlot, u32 resultType) {
    /* ProcessBattleResult - 176 bytes */
}
#pragma peephole reset

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
#pragma peephole off
void fn_8026316C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* FinalCleanup - 2652 bytes */
}
#pragma peephole reset

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
#pragma peephole off
u32 fn_802400D8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80240144 | Size: 0xAC */
#pragma peephole off
void fn_80240144(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80240144 (172 bytes) */
}
#pragma peephole reset

/* Address: 0x802401F0 | Size: 0x264 (612 bytes) */
#pragma peephole off
void fn_802401F0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802401F0 (612 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80240454 | Size: 0x16C (364 bytes) */
#pragma peephole off
void fn_80240454(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80240454 (364 bytes) */
}
#pragma peephole reset

/* Address: 0x802405C0 | Size: 0x8C */
#pragma peephole off
void fn_802405C0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802405C0 (140 bytes) */
}
#pragma peephole reset

/* Address: 0x8024064C | Size: 0x13C (316 bytes) */
#pragma peephole off
void fn_8024064C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024064C (316 bytes) */
}
#pragma peephole reset

/* Address: 0x80240788 | Size: 0x448 (1096 bytes) */
#pragma peephole off
void fn_80240788(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80240788 (1096 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80241660 | Size: 0x510 (1296 bytes) */
#pragma peephole off
void fn_80241660(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80241660 (1296 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80241B70 | Size: 0x278 (632 bytes) */
#pragma peephole off
void fn_80241B70(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80241B70 (632 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80241DE8 | Size: 0x1FC (508 bytes) */
#pragma peephole off
void fn_80241DE8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80241DE8 (508 bytes) */
}
#pragma peephole reset

/* Address: 0x80241FE4 | Size: 0x2A8 (680 bytes) */
#pragma peephole off
void fn_80241FE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80241FE4 (680 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024228C | Size: 0x3BC (956 bytes) */
#pragma peephole off
void fn_8024228C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024228C (956 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80242648 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_80242648(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80242648 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80242730 | Size: 0x170 (368 bytes) */
#pragma peephole off
void fn_80242730(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80242730 (368 bytes) */
}
#pragma peephole reset

/* Address: 0x802428A0 | Size: 0x134 (308 bytes) */
#pragma peephole off
void fn_802428A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802428A0 (308 bytes) */
}
#pragma peephole reset

/* Address: 0x802429D4 | Size: 0x17C (380 bytes) */
#pragma peephole off
void fn_802429D4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802429D4 (380 bytes) */
}
#pragma peephole reset

/* Address: 0x80242B50 | Size: 0x17C (380 bytes) */
#pragma peephole off
void fn_80242B50(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80242B50 (380 bytes) */
}
#pragma peephole reset

/* Address: 0x80242CCC | Size: 0xE4 (228 bytes) */
#pragma peephole off
void fn_80242CCC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80242CCC (228 bytes) */
}
#pragma peephole reset

/* Address: 0x80242DB0 | Size: 0x9C */
#pragma peephole off
void fn_80242DB0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80242DB0 (156 bytes) */
}
#pragma peephole reset

/* Address: 0x80242E4C | Size: 0x1A0 (416 bytes) */
#pragma peephole off
void fn_80242E4C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80242E4C (416 bytes) */
}
#pragma peephole reset

/* Address: 0x80242FEC | Size: 0xF8 (248 bytes) */
#pragma peephole off
void fn_80242FEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80242FEC (248 bytes) */
}
#pragma peephole reset

/* Address: 0x802430E4 | Size: 0x94 */
#pragma peephole off
void fn_802430E4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802430E4 (148 bytes) */
}
#pragma peephole reset

/* Address: 0x80243178 | Size: 0x10C (268 bytes) */
#pragma peephole off
void fn_80243178(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80243178 (268 bytes) */
}
#pragma peephole reset

/* Address: 0x80243284 | Size: 0x10C (268 bytes) */
#pragma peephole off
void fn_80243284(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80243284 (268 bytes) */
}
#pragma peephole reset

/* Address: 0x80243390 | Size: 0x10C (268 bytes) */
#pragma peephole off
void fn_80243390(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80243390 (268 bytes) */
}
#pragma peephole reset

/* Address: 0x8024349C | Size: 0x138 (312 bytes) */
#pragma peephole off
void fn_8024349C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024349C (312 bytes) */
}
#pragma peephole reset

/* Address: 0x802435D4 | Size: 0xB8 */
#pragma peephole off
void fn_802435D4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802435D4 (184 bytes) */
}
#pragma peephole reset

/* Address: 0x8024368C | Size: 0x1AC (428 bytes) */
#pragma peephole off
void fn_8024368C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024368C (428 bytes) */
}
#pragma peephole reset

/* Address: 0x80243838 | Size: 0x94 */
#pragma peephole off
void fn_80243838(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80243838 (148 bytes) */
}
#pragma peephole reset

/* Address: 0x802438CC | Size: 0x140 (320 bytes) */
#pragma peephole off
void fn_802438CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802438CC (320 bytes) */
}
#pragma peephole reset

/* Address: 0x80243A0C | Size: 0x250 (592 bytes) */
#pragma peephole off
void fn_80243A0C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80243A0C (592 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80243C5C | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80243C5C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80243CD8 | Size: 0x640 (1600 bytes) */
#pragma peephole off
void fn_80243CD8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80243CD8 (1600 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80244318 | Size: 0x160 (352 bytes) */
#pragma peephole off
void fn_80244318(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80244318 (352 bytes) */
}
#pragma peephole reset

/* Address: 0x80244478 | Size: 0x9C */
#pragma peephole off
void fn_80244478(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80244478 (156 bytes) */
}
#pragma peephole reset

/* Address: 0x80244514 | Size: 0x18C (396 bytes) */
#pragma peephole off
void fn_80244514(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80244514 (396 bytes) */
}
#pragma peephole reset

/* Address: 0x802446A0 | Size: 0x18C (396 bytes) */
#pragma peephole off
void fn_802446A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802446A0 (396 bytes) */
}
#pragma peephole reset

/* Address: 0x8024482C | Size: 0xD4 (212 bytes) */
#pragma peephole off
void fn_8024482C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024482C (212 bytes) */
}
#pragma peephole reset

/* Address: 0x80244900 | Size: 0x8C */
#pragma peephole off
void fn_80244900(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80244900 (140 bytes) */
}
#pragma peephole reset

/* Address: 0x8024498C | Size: 0x318 (792 bytes) */
#pragma peephole off
void fn_8024498C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024498C (792 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80244CA4 | Size: 0x2C4 (708 bytes) */
#pragma peephole off
void fn_80244CA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80244CA4 (708 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80244F68 | Size: 0x258 (600 bytes) */
#pragma peephole off
void fn_80244F68(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80244F68 (600 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802451C0 | Size: 0x258 (600 bytes) */
#pragma peephole off
void fn_802451C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802451C0 (600 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80245418 | Size: 0x160 (352 bytes) */
#pragma peephole off
void fn_80245418(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80245418 (352 bytes) */
}
#pragma peephole reset

/* Address: 0x80245578 | Size: 0x1A0 (416 bytes) */
#pragma peephole off
void fn_80245578(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80245578 (416 bytes) */
}
#pragma peephole reset

/* Address: 0x80245718 | Size: 0x98 */
#pragma peephole off
void fn_80245718(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80245718 (152 bytes) */
}
#pragma peephole reset

/* Address: 0x802457B0 | Size: 0x168 (360 bytes) */
#pragma peephole off
void fn_802457B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802457B0 (360 bytes) */
}
#pragma peephole reset

/* Address: 0x80245918 | Size: 0x98 */
#pragma peephole off
void fn_80245918(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80245918 (152 bytes) */
}
#pragma peephole reset

/* Address: 0x802459B0 | Size: 0x44C (1100 bytes) */
#pragma peephole off
void fn_802459B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802459B0 (1100 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80245DFC | Size: 0x14C (332 bytes) */
#pragma peephole off
void fn_80245DFC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80245DFC (332 bytes) */
}
#pragma peephole reset

/* Address: 0x80245F48 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80245F48(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80247048 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80247048(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802470C4 | Size: 0xEC (236 bytes) */
#pragma peephole off
void fn_802470C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802470C4 (236 bytes) */
}
#pragma peephole reset

/* Address: 0x802471B0 | Size: 0x128 (296 bytes) */
#pragma peephole off
void fn_802471B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802471B0 (296 bytes) */
}
#pragma peephole reset

/* Address: 0x802472D8 | Size: 0xDC (220 bytes) */
#pragma peephole off
void fn_802472D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802472D8 (220 bytes) */
}
#pragma peephole reset

/* Address: 0x802473B4 | Size: 0x144 (324 bytes) */
#pragma peephole off
void fn_802473B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802473B4 (324 bytes) */
}
#pragma peephole reset

/* Address: 0x802474F8 | Size: 0x1A8 (424 bytes) */
#pragma peephole off
void fn_802474F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802474F8 (424 bytes) */
}
#pragma peephole reset

/* Address: 0x802476A0 | Size: 0x110 (272 bytes) */
#pragma peephole off
void fn_802476A0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802476A0 (272 bytes) */
}
#pragma peephole reset

/* Address: 0x802477B0 | Size: 0x158 (344 bytes) */
#pragma peephole off
void fn_802477B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802477B0 (344 bytes) */
}
#pragma peephole reset

/* Address: 0x80247908 | Size: 0x1C0 (448 bytes) */
#pragma peephole off
void fn_80247908(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80247908 (448 bytes) */
}
#pragma peephole reset

/* Address: 0x80247AC8 | Size: 0x194 (404 bytes) */
#pragma peephole off
void fn_80247AC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80247AC8 (404 bytes) */
}
#pragma peephole reset

/* Address: 0x80247C5C | Size: 0x184 (388 bytes) */
#pragma peephole off
void fn_80247C5C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80247C5C (388 bytes) */
}
#pragma peephole reset

/* Address: 0x80247DE0 | Size: 0x1C0 (448 bytes) */
#pragma peephole off
void fn_80247DE0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80247DE0 (448 bytes) */
}
#pragma peephole reset

/* Address: 0x80247FA0 | Size: 0x1D0 (464 bytes) */
#pragma peephole off
void fn_80247FA0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80247FA0 (464 bytes) */
}
#pragma peephole reset

/* Address: 0x80248170 | Size: 0x150 (336 bytes) */
#pragma peephole off
void fn_80248170(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80248170 (336 bytes) */
}
#pragma peephole reset

/* Address: 0x802482C0 | Size: 0x1A0 (416 bytes) */
#pragma peephole off
void fn_802482C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802482C0 (416 bytes) */
}
#pragma peephole reset

/* Address: 0x80248460 | Size: 0x22C (556 bytes) */
#pragma peephole off
void fn_80248460(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80248460 (556 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024868C | Size: 0x1D0 (464 bytes) */
#pragma peephole off
void fn_8024868C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024868C (464 bytes) */
}
#pragma peephole reset

/* Address: 0x8024885C | Size: 0x2C0 (704 bytes) */
#pragma peephole off
void fn_8024885C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024885C (704 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80248B1C | Size: 0x220 (544 bytes) */
#pragma peephole off
void fn_80248B1C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80248B1C (544 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80248D3C | Size: 0x288 (648 bytes) */
#pragma peephole off
void fn_80248D3C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80248D3C (648 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80248FC4 | Size: 0x49C (1180 bytes) */
#pragma peephole off
void fn_80248FC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80248FC4 (1180 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80249460 | Size: 0x218 (536 bytes) */
#pragma peephole off
void fn_80249460(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80249460 (536 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80249678 | Size: 0x248 (584 bytes) */
#pragma peephole off
void fn_80249678(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80249678 (584 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802498C0 | Size: 0x1F4 (500 bytes) */
#pragma peephole off
void fn_802498C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802498C0 (500 bytes) */
}
#pragma peephole reset

/* Address: 0x80249AB4 | Size: 0x278 (632 bytes) */
#pragma peephole off
void fn_80249AB4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80249AB4 (632 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80249D2C | Size: 0x25C (604 bytes) */
#pragma peephole off
void fn_80249D2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80249D2C (604 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80249F88 | Size: 0x1E8 (488 bytes) */
#pragma peephole off
void fn_80249F88(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80249F88 (488 bytes) */
}
#pragma peephole reset

/* Address: 0x8024A170 | Size: 0x2B8 (696 bytes) */
#pragma peephole off
void fn_8024A170(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024A170 (696 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024A428 | Size: 0x23C (572 bytes) */
#pragma peephole off
void fn_8024A428(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024A428 (572 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024A664 | Size: 0x2C0 (704 bytes) */
#pragma peephole off
void fn_8024A664(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024A664 (704 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024A924 | Size: 0x25C (604 bytes) */
#pragma peephole off
void fn_8024A924(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024A924 (604 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024AB80 | Size: 0x204 (516 bytes) */
#pragma peephole off
void fn_8024AB80(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024AB80 (516 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024AD84 | Size: 0x16C (364 bytes) */
#pragma peephole off
void fn_8024AD84(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024AD84 (364 bytes) */
}
#pragma peephole reset

/* Address: 0x8024AEF0 | Size: 0xD4 (212 bytes) */
#pragma peephole off
void fn_8024AEF0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024AEF0 (212 bytes) */
}
#pragma peephole reset

/* Address: 0x8024AFC4 | Size: 0x4B0 (1200 bytes) */
#pragma peephole off
void fn_8024AFC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024AFC4 (1200 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024B474 | Size: 0x5D0 (1488 bytes) */
#pragma peephole off
void fn_8024B474(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024B474 (1488 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024BA44 | Size: 0x438 (1080 bytes) */
#pragma peephole off
void fn_8024BA44(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024BA44 (1080 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024BE7C | Size: 0x144 (324 bytes) */
#pragma peephole off
void fn_8024BE7C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024BE7C (324 bytes) */
}
#pragma peephole reset

/* Address: 0x8024BFC0 | Size: 0x5FC (1532 bytes) */
#pragma peephole off
void fn_8024BFC0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024BFC0 (1532 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024C5BC | Size: 0x91C (2332 bytes) */
#pragma peephole off
void fn_8024C5BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024C5BC (2332 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024CED8 | Size: 0x940 (2368 bytes) */
#pragma peephole off
void fn_8024CED8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024CED8 (2368 bytes) - complex function */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Experience & Level Processing (0x8024D000-0x80254000)
 * 136 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8024D818 | Size: 0x140 (320 bytes) */
#pragma peephole off
void fn_8024D818(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024D818 (320 bytes) */
}
#pragma peephole reset

/* Address: 0x8024D958 | Size: 0x1A4 (420 bytes) */
#pragma peephole off
void fn_8024D958(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024D958 (420 bytes) */
}
#pragma peephole reset

/* Address: 0x8024DAFC | Size: 0xC0 */
#pragma peephole off
void fn_8024DAFC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8024DAFC (192 bytes) */
}
#pragma peephole reset

/* Address: 0x8024DBBC | Size: 0xC0 */
#pragma peephole off
void fn_8024DBBC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8024DBBC (192 bytes) */
}
#pragma peephole reset

/* Address: 0x8024DC7C | Size: 0x210 (528 bytes) */
#pragma peephole off
void fn_8024DC7C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024DC7C (528 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024DE8C | Size: 0x138 (312 bytes) */
#pragma peephole off
void fn_8024DE8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024DE8C (312 bytes) */
}
#pragma peephole reset

/* Address: 0x8024DFC4 | Size: 0x108 (264 bytes) */
#pragma peephole off
void fn_8024DFC4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024DFC4 (264 bytes) */
}
#pragma peephole reset

/* Address: 0x8024E0CC | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8024E0CC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8024E148 | Size: 0xEC (236 bytes) */
#pragma peephole off
void fn_8024E148(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024E148 (236 bytes) */
}
#pragma peephole reset

/* Address: 0x8024E234 | Size: 0xEC (236 bytes) */
#pragma peephole off
void fn_8024E234(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024E234 (236 bytes) */
}
#pragma peephole reset

/* Address: 0x8024E320 | Size: 0x164 (356 bytes) */
#pragma peephole off
void fn_8024E320(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024E320 (356 bytes) */
}
#pragma peephole reset

/* Address: 0x8024E484 | Size: 0xA8 */
#pragma peephole off
void fn_8024E484(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8024E484 (168 bytes) */
}
#pragma peephole reset

/* Address: 0x8024E534 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8024E534(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8024E578 | Size: 0x118 (280 bytes) */
#pragma peephole off
void fn_8024E578(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024E578 (280 bytes) */
}
#pragma peephole reset

/* Address: 0x8024F8B4 | Size: 0x5CC (1484 bytes) */
#pragma peephole off
void fn_8024F8B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024F8B4 (1484 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8024FE80 | Size: 0x1F0 (496 bytes) */
#pragma peephole off
void fn_8024FE80(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8024FE80 (496 bytes) */
}
#pragma peephole reset

/* Address: 0x80250070 | Size: 0x27C (636 bytes) */
#pragma peephole off
void fn_80250070(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80250070 (636 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802502EC | Size: 0x694 (1684 bytes) */
#pragma peephole off
void fn_802502EC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802502EC (1684 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802509A0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802509A0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250A2C | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80250A2C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250AC0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80250AC0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250B44 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80250B44(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250BBC | Size: 0xA8 */
#pragma peephole off
void fn_80250BBC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80250BBC (168 bytes) */
}
#pragma peephole reset

/* Address: 0x80250C64 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80250C64(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250CF0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80250CF0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250D7C | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80250D7C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250E00 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80250E00(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80250E84 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80250E84(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80250EC4 | Size: 0x90 */
#pragma peephole off
void fn_80250EC4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80250EC4 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x80250F7C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80250F7C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80250FBC | Size: 0xB4 */
#pragma peephole off
void fn_80250FBC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80250FBC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80251070 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251070(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802510CC | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802510CC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251158 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251158(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251194 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251194(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802511E0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802511E0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802512A4 | Size: 0xAC */
#pragma peephole off
void fn_802512A4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802512A4 (172 bytes) */
}
#pragma peephole reset

/* Address: 0x80251358 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251358(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802513D0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802513D0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251454 | Size: 0x70 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251454(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802514EC | Size: 0x98 */
#pragma peephole off
void fn_802514EC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802514EC (152 bytes) */
}
#pragma peephole reset

/* Address: 0x80251584 | Size: 0x88 */
#pragma peephole off
void fn_80251584(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80251584 (136 bytes) */
}
#pragma peephole reset

/* Address: 0x80251614 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251614(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251658 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80251658(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251688 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251688(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802516C4 | Size: 0xD4 (212 bytes) */
#pragma peephole off
void fn_802516C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802516C4 (212 bytes) */
}
#pragma peephole reset

/* Address: 0x802517A0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802517A0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251824 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251824(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251860 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251860(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802518D8 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802518D8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251950 | Size: 0xBC */
#pragma peephole off
void fn_80251950(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80251950 (188 bytes) */
}
#pragma peephole reset

/* Address: 0x80251A0C | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251A0C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251A84 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251A84(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251AFC | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80251AFC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251B50 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251B50(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251BD4 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251BD4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251C58 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251C58(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251CEC | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80251CEC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80251D2C | Size: 0x88 */
#pragma peephole off
void fn_80251D2C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80251D2C (136 bytes) */
}
#pragma peephole reset

/* Address: 0x80251DB4 | Size: 0x90 */
#pragma peephole off
void fn_80251DB4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80251DB4 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x80251E44 | Size: 0x90 */
#pragma peephole off
void fn_80251E44(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80251E44 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x80251ED4 | Size: 0x90 */
#pragma peephole off
void fn_80251ED4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80251ED4 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x80251F6C | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80251F6C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80251FF0 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80251FF0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252038 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252038(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252078 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252078(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802520BC | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802520BC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252148 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252148(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252188 | Size: 0x80 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252188(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252208 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252208(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252248 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252248(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025228C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_8025228C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802522CC | Size: 0x80 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802522CC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252354 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80252354(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252398 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252398(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802523D8 | Size: 0x90 */
#pragma peephole off
void fn_802523D8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802523D8 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x802524B8 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802524B8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025253C | Size: 0xB4 */
#pragma peephole off
void fn_8025253C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025253C (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802525F0 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802525F0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252634 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252634(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252678 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252678(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802526BC | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802526BC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252748 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252748(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802527C4 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_802527C4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252804 | Size: 0x90 */
#pragma peephole off
void fn_80252804(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80252804 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x80252894 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80252894(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802528DC | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802528DC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252918 | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252918(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025297C | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025297C(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x802529A0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_802529A0(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x802529F4 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802529F4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252A80 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252A80(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252B04 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80252B04(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252B44 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252B44(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252BC8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80252BC8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80252C04 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252C04(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252C88 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252C88(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252D0C | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252D0C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252D90 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252D90(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252E14 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252E14(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252E98 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252E98(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80252F8C | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80252F8C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80253020 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80253020(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802530A4 | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_802530A4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802530E4 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802530E4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80253168 | Size: 0x88 */
#pragma peephole off
void fn_80253168(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80253168 (136 bytes) */
}
#pragma peephole reset

/* Address: 0x802531F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802531F8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253234 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80253234(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253270 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80253270(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253298 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80253298(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802532C0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802532C0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025334C | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025334C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802533D8 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802533D8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253400 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80253400(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025348C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_8025348C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802534D4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802534D4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253548 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80253548(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802535F4 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802535F4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253630 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80253630(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025366C | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025366C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802536F0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802536F0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80253774 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80253774(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802537F8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802537F8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253834 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80253834(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802538C0 | Size: 0x88 */
#pragma peephole off
void fn_802538C0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802538C0 (136 bytes) */
}
#pragma peephole reset

/* Address: 0x80253950 | Size: 0x6C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80253950(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802539BC | Size: 0xC4 (196 bytes) */
#pragma peephole off
void fn_802539BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802539BC (196 bytes) */
}
#pragma peephole reset

/* Address: 0x80253A80 | Size: 0xC4 (196 bytes) */
#pragma peephole off
void fn_80253A80(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80253A80 (196 bytes) */
}
#pragma peephole reset

/* Address: 0x80253B44 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80253B44(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80253B78 | Size: 0xB4 */
#pragma peephole off
void fn_80253B78(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80253B78 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80253C2C | Size: 0xC4 (196 bytes) */
#pragma peephole off
void fn_80253C2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80253C2C (196 bytes) */
}
#pragma peephole reset

/* Address: 0x80253CF0 | Size: 0xB4 */
#pragma peephole off
void fn_80253CF0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80253CF0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80253DA4 | Size: 0xC4 (196 bytes) */
#pragma peephole off
void fn_80253DA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80253DA4 (196 bytes) */
}
#pragma peephole reset

/* Address: 0x80253E68 | Size: 0xC0 */
#pragma peephole off
void fn_80253E68(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80253E68 (192 bytes) */
}
#pragma peephole reset

/* Address: 0x80253F28 | Size: 0xB4 */
#pragma peephole off
void fn_80253F28(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80253F28 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80253FDC | Size: 0xF0 (240 bytes) */
#pragma peephole off
void fn_80253FDC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80253FDC (240 bytes) */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Item Rewards & Poke Coupon (0x80254000-0x80258000)
 * 95 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802540CC | Size: 0xB4 */
#pragma peephole off
void fn_802540CC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802540CC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80254180 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254180(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802541B4 | Size: 0xB4 */
#pragma peephole off
void fn_802541B4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802541B4 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80254268 | Size: 0x1F8 (504 bytes) */
#pragma peephole off
void fn_80254268(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80254268 (504 bytes) */
}
#pragma peephole reset

/* Address: 0x80254460 | Size: 0xB4 */
#pragma peephole off
void fn_80254460(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80254460 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80254514 | Size: 0xB4 */
#pragma peephole off
void fn_80254514(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80254514 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802545C8 | Size: 0xB0 */
#pragma peephole off
void fn_802545C8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802545C8 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80254680 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80254680(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802546B8 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_802546B8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802546E8 | Size: 0x128 (296 bytes) */
#pragma peephole off
void fn_802546E8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802546E8 (296 bytes) */
}
#pragma peephole reset

/* Address: 0x80254810 | Size: 0xC8 (200 bytes) */
#pragma peephole off
void fn_80254810(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80254810 (200 bytes) */
}
#pragma peephole reset

/* Address: 0x802548D8 | Size: 0xB4 */
#pragma peephole off
void fn_802548D8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802548D8 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025498C | Size: 0xE4 (228 bytes) */
#pragma peephole off
void fn_8025498C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025498C (228 bytes) */
}
#pragma peephole reset

/* Address: 0x80254A70 | Size: 0xB4 */
#pragma peephole off
void fn_80254A70(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80254A70 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80254B24 | Size: 0x178 (376 bytes) */
#pragma peephole off
void fn_80254B24(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80254B24 (376 bytes) */
}
#pragma peephole reset

/* Address: 0x80254C9C | Size: 0xB0 */
#pragma peephole off
void fn_80254C9C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80254C9C (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80254D4C | Size: 0xB4 */
#pragma peephole off
void fn_80254D4C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80254D4C (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80254E00 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_80254E00(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254E34 | Size: 0xB4 */
#pragma peephole off
void fn_80254E34(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80254E34 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80254EE8 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254EE8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F1C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80254F1C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F54 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80254F54(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80254F88 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80254F88(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80255000 | Size: 0xF0 (240 bytes) */
#pragma peephole off
void fn_80255000(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80255000 (240 bytes) */
}
#pragma peephole reset

/* Address: 0x802550F0 | Size: 0xB4 */
#pragma peephole off
void fn_802550F0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802550F0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802551A4 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802551A4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80255220 | Size: 0xA8 */
#pragma peephole off
void fn_80255220(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80255220 (168 bytes) */
}
#pragma peephole reset

/* Address: 0x802552D0 | Size: 0xB4 */
#pragma peephole off
void fn_802552D0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802552D0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80255384 | Size: 0xB4 */
#pragma peephole off
void fn_80255384(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80255384 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80255438 | Size: 0xB4 */
#pragma peephole off
void fn_80255438(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80255438 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802554EC | Size: 0x10C (268 bytes) */
#pragma peephole off
void fn_802554EC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802554EC (268 bytes) */
}
#pragma peephole reset

/* Address: 0x802555F8 | Size: 0x228 (552 bytes) */
#pragma peephole off
void fn_802555F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802555F8 (552 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80255820 | Size: 0x218 (536 bytes) */
#pragma peephole off
void fn_80255820(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80255820 (536 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80255A38 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80255A38(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80255AAC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80255AAC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255AE4 | Size: 0x68 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80255AE4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80255B4C | Size: 0xCC (204 bytes) */
#pragma peephole off
void fn_80255B4C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80255B4C (204 bytes) */
}
#pragma peephole reset

/* Address: 0x80255C18 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80255C18(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80255C8C | Size: 0xB0 */
#pragma peephole off
void fn_80255C8C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80255C8C (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80255D3C | Size: 0x40 | Pattern: simple_wrapper */
u32 fn_80255D3C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255D7C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80255D7C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255DB4 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80255DB4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80255DF8 | Size: 0xB0 */
#pragma peephole off
void fn_80255DF8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80255DF8 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80255EA8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80255EA8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80255EEC | Size: 0xB4 */
#pragma peephole off
void fn_80255EEC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80255EEC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80255FA0 | Size: 0xB4 */
#pragma peephole off
void fn_80255FA0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80255FA0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256054 | Size: 0xB4 */
#pragma peephole off
void fn_80256054(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256054 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256108 | Size: 0xB4 */
#pragma peephole off
void fn_80256108(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256108 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802561BC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_802561BC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802561F4 | Size: 0xB4 */
#pragma peephole off
void fn_802561F4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802561F4 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802562A8 | Size: 0xB4 */
#pragma peephole off
void fn_802562A8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802562A8 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025635C | Size: 0xB4 */
#pragma peephole off
void fn_8025635C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025635C (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256410 | Size: 0xB0 */
#pragma peephole off
void fn_80256410(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256410 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x802564C8 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802564C8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025652C | Size: 0xB4 */
#pragma peephole off
void fn_8025652C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025652C (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802565E0 | Size: 0xB4 */
#pragma peephole off
void fn_802565E0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802565E0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256694 | Size: 0xB4 */
#pragma peephole off
void fn_80256694(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256694 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256748 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256748(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256780 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256780(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802567B8 | Size: 0xA4 */
#pragma peephole off
void fn_802567B8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802567B8 (164 bytes) */
}
#pragma peephole reset

/* Address: 0x8025685C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025685C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256894 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256894(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802568CC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_802568CC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256904 | Size: 0xB0 */
#pragma peephole off
void fn_80256904(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256904 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x802569B4 | Size: 0xB4 */
#pragma peephole off
void fn_802569B4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802569B4 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256A68 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80256A68(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80256AE0 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80256AE0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256B18 | Size: 0xB4 */
#pragma peephole off
void fn_80256B18(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256B18 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256BCC | Size: 0xB4 */
#pragma peephole off
void fn_80256BCC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256BCC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256C80 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80256C80(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80256CBC | Size: 0xB4 */
#pragma peephole off
void fn_80256CBC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256CBC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80256D70 | Size: 0xB0 */
#pragma peephole off
void fn_80256D70(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256D70 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80256E20 | Size: 0xB0 */
#pragma peephole off
void fn_80256E20(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80256E20 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80256ED0 | Size: 0x200 (512 bytes) */
#pragma peephole off
void fn_80256ED0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80256ED0 (512 bytes) */
}
#pragma peephole reset

/* Address: 0x802570D0 | Size: 0xB0 */
#pragma peephole off
void fn_802570D0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802570D0 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80257180 | Size: 0x23C (572 bytes) */
#pragma peephole off
void fn_80257180(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80257180 (572 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802573BC | Size: 0xB0 */
#pragma peephole off
void fn_802573BC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802573BC (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80257474 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80257474(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802574AC | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_802574AC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80257508 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80257508(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80257544 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80257544(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802575C8 | Size: 0x188 (392 bytes) */
#pragma peephole off
void fn_802575C8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802575C8 (392 bytes) */
}
#pragma peephole reset

/* Address: 0x80257750 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_80257750(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80257750 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80257838 | Size: 0x8C */
#pragma peephole off
void fn_80257838(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80257838 (140 bytes) */
}
#pragma peephole reset

/* Address: 0x802578C4 | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802578C4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025793C | Size: 0xB4 */
#pragma peephole off
void fn_8025793C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025793C (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802579F0 | Size: 0xB0 */
#pragma peephole off
void fn_802579F0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802579F0 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80257AA0 | Size: 0xB4 */
#pragma peephole off
void fn_80257AA0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80257AA0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80257B54 | Size: 0xCC (204 bytes) */
#pragma peephole off
void fn_80257B54(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80257B54 (204 bytes) */
}
#pragma peephole reset

/* Address: 0x80257C20 | Size: 0xB4 */
#pragma peephole off
void fn_80257C20(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80257C20 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80257CD4 | Size: 0x124 (292 bytes) */
#pragma peephole off
void fn_80257CD4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80257CD4 (292 bytes) */
}
#pragma peephole reset

/* Address: 0x80257DF8 | Size: 0xB4 */
#pragma peephole off
void fn_80257DF8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80257DF8 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80257EAC | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80257EAC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80257EE4 | Size: 0xE4 (228 bytes) */
#pragma peephole off
void fn_80257EE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80257EE4 (228 bytes) */
}
#pragma peephole reset

/* Address: 0x80257FC8 | Size: 0xF4 (244 bytes) */
#pragma peephole off
void fn_80257FC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80257FC8 (244 bytes) */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Team State Updates (0x80258000-0x8025C000)
 * 78 functions
 * ------------------------------------------------------------------- */

/* Address: 0x802580BC | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802580BC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025813C | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_8025813C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025813C (208 bytes) */
}
#pragma peephole reset

/* Address: 0x8025820C | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025820C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80258284 | Size: 0x140 (320 bytes) */
#pragma peephole off
void fn_80258284(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80258284 (320 bytes) */
}
#pragma peephole reset

/* Address: 0x802583C4 | Size: 0xB0 */
#pragma peephole off
void fn_802583C4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802583C4 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80258474 | Size: 0xB0 */
#pragma peephole off
void fn_80258474(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258474 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80258524 | Size: 0xB0 */
#pragma peephole off
void fn_80258524(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258524 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x802585D4 | Size: 0x120 (288 bytes) */
#pragma peephole off
void fn_802585D4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802585D4 (288 bytes) */
}
#pragma peephole reset

/* Address: 0x802586FC | Size: 0xBC */
#pragma peephole off
void fn_802586FC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802586FC (188 bytes) */
}
#pragma peephole reset

/* Address: 0x802587C0 | Size: 0x144 (324 bytes) */
#pragma peephole off
void fn_802587C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802587C0 (324 bytes) */
}
#pragma peephole reset

/* Address: 0x80258904 | Size: 0xB0 */
#pragma peephole off
void fn_80258904(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258904 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x802589B4 | Size: 0xB0 */
#pragma peephole off
void fn_802589B4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802589B4 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80258A64 | Size: 0x6C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80258A64(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80258AD0 | Size: 0xB4 */
#pragma peephole off
void fn_80258AD0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258AD0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80258B84 | Size: 0xB4 */
#pragma peephole off
void fn_80258B84(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258B84 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80258C38 | Size: 0xB4 */
#pragma peephole off
void fn_80258C38(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258C38 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80258CEC | Size: 0xB4 */
#pragma peephole off
void fn_80258CEC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258CEC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80258DA0 | Size: 0xB4 */
#pragma peephole off
void fn_80258DA0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258DA0 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80258E54 | Size: 0xB4 */
#pragma peephole off
void fn_80258E54(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258E54 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80258F08 | Size: 0xB4 */
#pragma peephole off
void fn_80258F08(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258F08 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80258FBC | Size: 0xB4 */
#pragma peephole off
void fn_80258FBC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80258FBC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80259070 | Size: 0xB4 */
#pragma peephole off
void fn_80259070(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80259070 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x80259124 | Size: 0xB4 */
#pragma peephole off
void fn_80259124(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80259124 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x802591D8 | Size: 0x260 (608 bytes) */
#pragma peephole off
void fn_802591D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802591D8 (608 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80259438 | Size: 0x270 (624 bytes) */
#pragma peephole off
void fn_80259438(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80259438 (624 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802596A8 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802596A8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802596E4 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_802596E4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802596E4 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x802597CC | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_802597CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802597CC (232 bytes) */
}
#pragma peephole reset

/* Address: 0x802598B4 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_802598B4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802598B4 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025999C | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025999C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025999C (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80259A84 | Size: 0x68 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80259A84(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80259AEC | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_80259AEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80259AEC (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80259BD4 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_80259BD4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80259BD4 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80259CBC | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_80259CBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80259CBC (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80259DA4 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_80259DA4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80259DA4 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80259E8C | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_80259E8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80259E8C (232 bytes) */
}
#pragma peephole reset

/* Address: 0x80259F74 | Size: 0x1F8 (504 bytes) */
#pragma peephole off
void fn_80259F74(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80259F74 (504 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A16C | Size: 0xB4 */
#pragma peephole off
void fn_8025A16C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A16C (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A220 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8025A220(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A254 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025A254(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A340 | Size: 0xB4 */
#pragma peephole off
void fn_8025A340(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A340 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A3F4 | Size: 0xB4 */
#pragma peephole off
void fn_8025A3F4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A3F4 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A4A8 | Size: 0xB4 */
#pragma peephole off
void fn_8025A4A8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A4A8 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A55C | Size: 0xB0 */
#pragma peephole off
void fn_8025A55C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A55C (176 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A60C | Size: 0xB0 */
#pragma peephole off
void fn_8025A60C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A60C (176 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A6BC | Size: 0xB4 */
#pragma peephole off
void fn_8025A6BC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A6BC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A770 | Size: 0x100 (256 bytes) */
#pragma peephole off
void fn_8025A770(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025A770 (256 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A870 | Size: 0x8C */
#pragma peephole off
void fn_8025A870(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A870 (140 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A8FC | Size: 0xB4 */
#pragma peephole off
void fn_8025A8FC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A8FC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025A9B0 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025A9B0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025A9EC | Size: 0xB4 */
#pragma peephole off
void fn_8025A9EC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025A9EC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025AAA0 | Size: 0x270 (624 bytes) */
#pragma peephole off
void fn_8025AAA0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025AAA0 (624 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025AD10 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025AD10(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025AD48 | Size: 0xB4 */
#pragma peephole off
void fn_8025AD48(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025AD48 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025ADFC | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_8025ADFC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025AE28 | Size: 0xB0 */
#pragma peephole off
void fn_8025AE28(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025AE28 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x8025AED8 | Size: 0xE0 (224 bytes) */
#pragma peephole off
void fn_8025AED8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025AED8 (224 bytes) */
}
#pragma peephole reset

/* Address: 0x8025AFB8 | Size: 0xB0 */
#pragma peephole off
void fn_8025AFB8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025AFB8 (176 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B068 | Size: 0xB4 */
#pragma peephole off
void fn_8025B068(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025B068 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B124 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B124(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B124 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B20C | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B20C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B20C (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B2F4 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B2F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B2F4 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B3DC | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B3DC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B3DC (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B4C4 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B4C4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B4C4 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B5AC | Size: 0xB4 */
#pragma peephole off
void fn_8025B5AC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025B5AC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B660 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B660(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B660 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B748 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B748(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B748 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B830 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B830(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B830 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025B918 | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_8025B918(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025B918 (232 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BA00 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_8025BA00(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025BA2C | Size: 0xF4 (244 bytes) */
#pragma peephole off
void fn_8025BA2C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025BA2C (244 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BB20 | Size: 0x108 (264 bytes) */
#pragma peephole off
void fn_8025BB20(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025BB20 (264 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BC28 | Size: 0xB4 */
#pragma peephole off
void fn_8025BC28(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025BC28 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BCDC | Size: 0xB4 */
#pragma peephole off
void fn_8025BCDC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025BCDC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BD90 | Size: 0xB4 */
#pragma peephole off
void fn_8025BD90(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025BD90 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BE44 | Size: 0xB4 */
#pragma peephole off
void fn_8025BE44(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025BE44 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BEF8 | Size: 0xB4 */
#pragma peephole off
void fn_8025BEF8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025BEF8 (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025BFAC | Size: 0x200 (512 bytes) */
#pragma peephole off
void fn_8025BFAC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025BFAC (512 bytes) */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Shadow Pokemon & Purification (0x8025C000-0x80260000)
 * 89 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8025C1AC | Size: 0xB0 */
#pragma peephole off
void fn_8025C1AC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025C1AC (176 bytes) */
}
#pragma peephole reset

/* Address: 0x8025C264 | Size: 0x340 (832 bytes) */
#pragma peephole off
void fn_8025C264(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025C264 (832 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025C5A4 | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_8025C5A4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025C5A4 (208 bytes) */
}
#pragma peephole reset

/* Address: 0x8025C674 | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025C674(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025C6BC | Size: 0xB4 */
#pragma peephole off
void fn_8025C6BC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025C6BC (180 bytes) */
}
#pragma peephole reset

/* Address: 0x8025C770 | Size: 0x98 */
#pragma peephole off
void fn_8025C770(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025C770 (152 bytes) */
}
#pragma peephole reset

/* Address: 0x8025C808 | Size: 0x2A0 (672 bytes) */
#pragma peephole off
void fn_8025C808(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025C808 (672 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025CAA8 | Size: 0x94 */
#pragma peephole off
void fn_8025CAA8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025CAA8 (148 bytes) */
}
#pragma peephole reset

/* Address: 0x8025CB3C | Size: 0xAC */
#pragma peephole off
void fn_8025CB3C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025CB3C (172 bytes) */
}
#pragma peephole reset

/* Address: 0x8025CBE8 | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025CBE8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025CC30 | Size: 0x60 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025CC30(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025CC90 | Size: 0x50 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025CC90(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025CCE0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025CCE0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025CD64 | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025CD64(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025CDB8 | Size: 0x2B4 (692 bytes) */
#pragma peephole off
void fn_8025CDB8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025CDB8 (692 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025D06C | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025D06C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025D0A8 | Size: 0xBC */
#pragma peephole off
void fn_8025D0A8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025D0A8 (188 bytes) */
}
#pragma peephole reset

/* Address: 0x8025D164 | Size: 0x128 (296 bytes) */
#pragma peephole off
void fn_8025D164(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025D164 (296 bytes) */
}
#pragma peephole reset

/* Address: 0x8025D28C | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D28C(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025D2B0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D2B0(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025D2D4 | Size: 0x90 */
#pragma peephole off
void fn_8025D2D4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025D2D4 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x8025D364 | Size: 0x90 */
#pragma peephole off
void fn_8025D364(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025D364 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x8025D3F4 | Size: 0x16C (364 bytes) */
#pragma peephole off
void fn_8025D3F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025D3F4 (364 bytes) */
}
#pragma peephole reset

/* Address: 0x8025D560 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D560(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025D584 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025D584(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025D5E0 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025D5E0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025D644 | Size: 0x100 (256 bytes) */
#pragma peephole off
void fn_8025D644(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025D644 (256 bytes) */
}
#pragma peephole reset

/* Address: 0x8025D744 | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025D744(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025D788 | Size: 0x80 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025D788(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025D808 | Size: 0x94 */
#pragma peephole off
void fn_8025D808(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025D808 (148 bytes) */
}
#pragma peephole reset

/* Address: 0x8025D89C | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025D89C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025D914 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D914(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025D938 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025D938(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025D970 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025D970(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025D9A8 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D9A8(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025D9CC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025D9CC(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025D9F0 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_8025D9F0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DA18 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DA18(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DA3C | Size: 0x4C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025DA3C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025DA88 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DA88(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DAAC | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAAC(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DAD0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DAD0(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DAF4 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8025DAF4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DB2C | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_8025DB2C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DB5C | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DB5C(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DB80 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_8025DB80(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025DBB0 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8025DBB0(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DBD4 | Size: 0x58 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025DBD4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025DC2C | Size: 0x90 */
#pragma peephole off
void fn_8025DC2C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025DC2C (144 bytes) */
}
#pragma peephole reset

/* Address: 0x8025DCBC | Size: 0x58 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025DCBC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025DD14 | Size: 0x98 */
#pragma peephole off
void fn_8025DD14(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025DD14 (152 bytes) */
}
#pragma peephole reset

/* Address: 0x8025DDAC | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025DDAC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025DDF4 | Size: 0x18 | Pattern: null_check_getter */
u32 fn_8025DDF4(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025DE0C | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025DE0C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025DE54 | Size: 0xE4 (228 bytes) */
#pragma peephole off
void fn_8025DE54(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025DE54 (228 bytes) */
}
#pragma peephole reset

/* Address: 0x8025DF38 | Size: 0x178 (376 bytes) */
#pragma peephole off
void fn_8025DF38(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025DF38 (376 bytes) */
}
#pragma peephole reset

/* Address: 0x8025E0B0 | Size: 0x10C (268 bytes) */
#pragma peephole off
void fn_8025E0B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025E0B0 (268 bytes) */
}
#pragma peephole reset

/* Address: 0x8025E1BC | Size: 0x1F4 (500 bytes) */
#pragma peephole off
void fn_8025E1BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025E1BC (500 bytes) */
}
#pragma peephole reset

/* Address: 0x8025E3B0 | Size: 0x184 (388 bytes) */
#pragma peephole off
void fn_8025E3B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025E3B0 (388 bytes) */
}
#pragma peephole reset

/* Address: 0x8025E534 | Size: 0xC0 */
#pragma peephole off
void fn_8025E534(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025E534 (192 bytes) */
}
#pragma peephole reset

/* Address: 0x8025E5F4 | Size: 0x4C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025E5F4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025E640 | Size: 0x37C (892 bytes) */
#pragma peephole off
void fn_8025E640(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025E640 (892 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025E9BC | Size: 0x390 (912 bytes) */
#pragma peephole off
void fn_8025E9BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025E9BC (912 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025ED4C | Size: 0x20C (524 bytes) */
#pragma peephole off
void fn_8025ED4C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025ED4C (524 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025EF58 | Size: 0x354 (852 bytes) */
#pragma peephole off
void fn_8025EF58(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025EF58 (852 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8025F2AC | Size: 0x14 | Pattern: null_check_getter */
u32 fn_8025F2AC(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8025F2C0 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_8025F2C0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025F2FC | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025F2FC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025F350 | Size: 0xA4 */
#pragma peephole off
void fn_8025F350(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025F350 (164 bytes) */
}
#pragma peephole reset

/* Address: 0x8025F3F4 | Size: 0x90 */
#pragma peephole off
void fn_8025F3F4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025F3F4 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x8025F484 | Size: 0x90 */
#pragma peephole off
void fn_8025F484(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025F484 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x8025F514 | Size: 0x10 | Pattern: sda_getter */
u32 fn_8025F514(void) { return 0; /* stub */ }

/* Address: 0x8025F524 | Size: 0x60 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025F524(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025F584 | Size: 0x94 */
#pragma peephole off
void fn_8025F584(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025F584 (148 bytes) */
}
#pragma peephole reset

/* Address: 0x8025F618 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_8025F618(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025F648 | Size: 0xC4 (196 bytes) */
#pragma peephole off
void fn_8025F648(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025F648 (196 bytes) */
}
#pragma peephole reset

/* Address: 0x8025F70C | Size: 0xDC (220 bytes) */
#pragma peephole off
void fn_8025F70C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025F70C (220 bytes) */
}
#pragma peephole reset

/* Address: 0x8025F7E8 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8025F7E8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025F81C | Size: 0x6C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025F81C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025F888 | Size: 0x124 (292 bytes) */
#pragma peephole off
void fn_8025F888(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025F888 (292 bytes) */
}
#pragma peephole reset

/* Address: 0x8025F9AC | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025F9AC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025FA20 | Size: 0x1AC (428 bytes) */
#pragma peephole off
void fn_8025FA20(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025FA20 (428 bytes) */
}
#pragma peephole reset

/* Address: 0x8025FBCC | Size: 0x168 (360 bytes) */
#pragma peephole off
void fn_8025FBCC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025FBCC (360 bytes) */
}
#pragma peephole reset

/* Address: 0x8025FD34 | Size: 0xA8 */
#pragma peephole off
void fn_8025FD34(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025FD34 (168 bytes) */
}
#pragma peephole reset

/* Address: 0x8025FDDC | Size: 0xA8 */
#pragma peephole off
void fn_8025FDDC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8025FDDC (168 bytes) */
}
#pragma peephole reset

/* Address: 0x8025FE84 | Size: 0x60 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025FE84(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025FEE4 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8025FEE4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8025FF18 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8025FF18(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8025FF9C | Size: 0xD4 (212 bytes) */
#pragma peephole off
void fn_8025FF9C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8025FF9C (212 bytes) */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Utility & Cleanup (0x80260000-0x80266360)
 * 70 functions
 * ------------------------------------------------------------------- */

/* Address: 0x80260070 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80260070(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802600E4 | Size: 0x378 (888 bytes) */
#pragma peephole off
void fn_802600E4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802600E4 (888 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8026045C | Size: 0x27C (636 bytes) */
#pragma peephole off
void fn_8026045C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8026045C (636 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802606D8 | Size: 0x238 (568 bytes) */
#pragma peephole off
void fn_802606D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802606D8 (568 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80260910 | Size: 0x5AC (1452 bytes) */
#pragma peephole off
void fn_80260910(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80260910 (1452 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80260EBC | Size: 0x414 (1044 bytes) */
#pragma peephole off
void fn_80260EBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80260EBC (1044 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802612D0 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_802612D0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8026132C | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8026132C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80261388 | Size: 0x4C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80261388(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802613D4 | Size: 0x70 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802613D4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80261444 | Size: 0x70 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80261444(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802614B4 | Size: 0x88 */
#pragma peephole off
void fn_802614B4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802614B4 (136 bytes) */
}
#pragma peephole reset

/* Address: 0x8026153C | Size: 0xB8 */
#pragma peephole off
void fn_8026153C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8026153C (184 bytes) */
}
#pragma peephole reset

/* Address: 0x802615F4 | Size: 0x114 (276 bytes) */
#pragma peephole off
void fn_802615F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802615F4 (276 bytes) */
}
#pragma peephole reset

/* Address: 0x80261708 | Size: 0x144 (324 bytes) */
#pragma peephole off
void fn_80261708(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80261708 (324 bytes) */
}
#pragma peephole reset

/* Address: 0x8026184C | Size: 0x108 (264 bytes) */
#pragma peephole off
void fn_8026184C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8026184C (264 bytes) */
}
#pragma peephole reset

/* Address: 0x80261954 | Size: 0x17C (380 bytes) */
#pragma peephole off
void fn_80261954(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80261954 (380 bytes) */
}
#pragma peephole reset

/* Address: 0x80261AD0 | Size: 0x98 */
#pragma peephole off
void fn_80261AD0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80261AD0 (152 bytes) */
}
#pragma peephole reset

/* Address: 0x80261B68 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80261B68(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80261BEC | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_80261BEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80261BEC (208 bytes) */
}
#pragma peephole reset

/* Address: 0x80261CBC | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_80261CBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80261CBC (208 bytes) */
}
#pragma peephole reset

/* Address: 0x80261D8C | Size: 0xF0 (240 bytes) */
#pragma peephole off
void fn_80261D8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80261D8C (240 bytes) */
}
#pragma peephole reset

/* Address: 0x80261E7C | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80261E7C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80261EF8 | Size: 0xBC */
#pragma peephole off
void fn_80261EF8(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80261EF8 (188 bytes) */
}
#pragma peephole reset

/* Address: 0x80261FB4 | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_80261FB4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80261FB4 (208 bytes) */
}
#pragma peephole reset

/* Address: 0x80262084 | Size: 0x140 (320 bytes) */
#pragma peephole off
void fn_80262084(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80262084 (320 bytes) */
}
#pragma peephole reset

/* Address: 0x802621C4 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_802621C4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802621F4 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_802621F4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80262270 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80262270(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802622E4 | Size: 0x24 | Pattern: null_check_getter */
u32 fn_802622E4(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x80262308 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_80262308(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80262334 | Size: 0x80 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80262334(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802623B4 | Size: 0xB8 */
#pragma peephole off
void fn_802623B4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802623B4 (184 bytes) */
}
#pragma peephole reset

/* Address: 0x8026246C | Size: 0x24 | Pattern: null_check_getter */
u32 fn_8026246C(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x80262490 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80262490(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802624CC | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_802624CC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80262508 | Size: 0x82C (2092 bytes) */
#pragma peephole off
void fn_80262508(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80262508 (2092 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80262D3C | Size: 0x430 (1072 bytes) */
#pragma peephole off
void fn_80262D3C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80262D3C (1072 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80263BC8 | Size: 0x21C (540 bytes) */
#pragma peephole off
void fn_80263BC8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80263BC8 (540 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80263DE4 | Size: 0x6A4 (1700 bytes) */
#pragma peephole off
void fn_80263DE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80263DE4 (1700 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80264488 | Size: 0x654 (1620 bytes) */
#pragma peephole off
void fn_80264488(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80264488 (1620 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80264ADC | Size: 0x27C (636 bytes) */
#pragma peephole off
void fn_80264ADC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80264ADC (636 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80264D58 | Size: 0x2E4 (740 bytes) */
#pragma peephole off
void fn_80264D58(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80264D58 (740 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8026503C | Size: 0x2F0 (752 bytes) */
#pragma peephole off
void fn_8026503C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8026503C (752 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8026532C | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_8026532C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8026532C (208 bytes) */
}
#pragma peephole reset

/* Address: 0x802653FC | Size: 0x19C (412 bytes) */
#pragma peephole off
void fn_802653FC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802653FC (412 bytes) */
}
#pragma peephole reset

/* Address: 0x80265598 | Size: 0x114 (276 bytes) */
#pragma peephole off
void fn_80265598(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80265598 (276 bytes) */
}
#pragma peephole reset

/* Address: 0x802656AC | Size: 0xA8 */
#pragma peephole off
void fn_802656AC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802656AC (168 bytes) */
}
#pragma peephole reset

/* Address: 0x80265754 | Size: 0x174 (372 bytes) */
#pragma peephole off
void fn_80265754(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80265754 (372 bytes) */
}
#pragma peephole reset

/* Address: 0x802658C8 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_802658C8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265924 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80265924(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8026595C | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8026595C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802659A4 | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802659A4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802659F8 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802659F8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265A6C | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_80265A6C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80265A6C (208 bytes) */
}
#pragma peephole reset

/* Address: 0x80265B3C | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80265B3C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80265B74 | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265B74(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265BBC | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265BBC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265C10 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265C10(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265C84 | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_80265C84(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80265C84 (208 bytes) */
}
#pragma peephole reset

/* Address: 0x80265D54 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265D54(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265DB0 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265DB0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265E34 | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265E34(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265E7C | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265E7C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265EC4 | Size: 0x50 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265EC4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265F14 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_80265F14(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80265F4C | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80265F4C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80265F94 | Size: 0x2BC (700 bytes) */
#pragma peephole off
void fn_80265F94(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80265F94 (700 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80266250 | Size: 0xD0 (208 bytes) */
#pragma peephole off
void fn_80266250(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80266250 (208 bytes) */
}
#pragma peephole reset

/* Address: 0x80266320 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80266320(void* ctx, u32 param) { return 0; /* stub */ }
