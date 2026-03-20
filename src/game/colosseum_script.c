/**
 * @file colosseum_script.c
 * @brief Story script interpreter and Colosseum tournament logic.
 *
 * =========================================================================
 * SUBSYSTEM ANALYSIS
 * =========================================================================
 *
 * Address range: 0x80212000 - 0x80240000
 * Total functions: ~320
 * Total code size: ~180KB
 *
 * This is the largest subsystem in the uncovered gap. It contains the
 * story script interpreter, the Colosseum tournament flow controller,
 * team management/validation, and extensive logic for the game's unique
 * stadium-based battle progression.
 *
 * THE SCRIPT INTERPRETER
 * ----------------------
 *
 * fn_802249B8 (StoryScriptExecute) is the crown jewel at 6012 bytes --
 * the single largest function in the entire uncovered range. It implements
 * a bytecode interpreter for the game's story/event scripting system.
 *
 * The interpreter reads commands from a data stream pointed to by
 * lbl_80478D78 (SDA21-relative) and dispatches them through
 * jumptable_8039A220 (52 entries). The command byte at offset +3 in the
 * stream is read with `lbz r0, 0x3(r31)`, then range-checked and used
 * as the jumptable index after subtracting 7 (commands 0x07-0x3B).
 *
 * Interpreter setup (first ~200 instructions):
 *   1. PokemonGet(0, 0, 0x14, 0) -> party count (stored in r24)
 *   2. fn_801F453C(0, 1) -> player party reference (stored in r19)
 *   3. PokemonSlotLookupDefault(0x11, 0) -> player active slot (r30)
 *   4. fn_80205184() -> some context (stored in r28)
 *   5. PokemonSlotLookupDefault(0x12, 0) -> enemy active slot (r18)
 *   6. Load flags from lbl_80478D78+3
 *   7. If bit 6 is set: player=r30, enemy=r18 (normal perspective)
 *      If bit 6 is clear: player=r18, enemy=r30 (swapped perspective)
 *   8. Set up active Pokemon via PokemonSet(0, 0, 0x47, 0, playerSlot)
 *      and PokemonSet(0, 0, 0x4B, 0, enemySlot)
 *
 * Script program counter:
 *   lbl_8047B610 is the script PC. It's incremented by +1 for most
 *   commands and by +5 for commands that skip the next instruction
 *   (like successful conditional branches). The pattern:
 *     lwz r3, lbl_8047B610@sda21(r0)
 *     addi r0, r3, 1  (or 5)
 *     stw r0, lbl_8047B610@sda21(r0)
 *   appears hundreds of times throughout this code.
 *
 * Data tables referenced from the interpreter:
 *   jumptable_8039A220 : Command dispatch table (52 entries)
 *   lbl_80399F58       : Command parameter table (indexed by command byte)
 *   lbl_80279EF4       : Half-word constant table (indexed by status values)
 *
 * COLOSSEUM TOURNAMENT FUNCTIONS
 * ------------------------------
 *
 *   fn_80221104 (ColosseumMatchSetup, 0x100C bytes):
 *     Sets up a complete Colosseum match. Resolves opponents from the
 *     bracket data, validates teams, initializes battle parameters.
 *     Uses TrainerDataGet extensively to configure both sides.
 *
 *   fn_8021FAD4 (TeamValidation, 0xCA4 bytes):
 *     Validates a team against Colosseum rules. Checks for banned
 *     species, duplicate items, level caps, and other restrictions.
 *     Called before each round to verify team legality.
 *
 *   fn_8022BE2C (ColosseumRoundExecute, 0x1258 bytes):
 *     Manages a full round: pre-battle setup, the battle itself,
 *     and post-battle processing (experience, rewards, healing).
 *
 *   fn_8022A6C8 (ColosseumBracketAdvance, 0xBD4 bytes):
 *     Advances the tournament bracket after a round completes.
 *     Handles win/loss paths and bracket reshuffling.
 *
 *   fn_8022F2F8 (RewardProcessing, 0xB28 bytes):
 *     Post-battle reward processing: prize money calculation,
 *     item rewards, experience distribution.
 *
 * TEAM MANAGEMENT (0x80226000-0x80240000)
 * ----------------------------------------
 *
 *   fn_80230568 (TeamRegistration, 0x1194 bytes):
 *     Full team registration flow for a Colosseum. Validates team,
 *     saves team composition, sets entry flags.
 *
 *   fn_80232110 (TeamCompositionCheck, 0xC18 bytes):
 *     Detailed team composition checking (species clause, item clause).
 *
 *   fn_802331F4 (OpponentTeamGeneration, 0xBBC bytes):
 *     Generates an opponent team for a Colosseum round. Selects from
 *     predefined trainer rosters based on difficulty level.
 *
 *   fn_80234A0C (TeamPokemonSetup, 0xC50 bytes):
 *     Initializes each Pokemon on a team for battle: calculates stats,
 *     applies level adjustments, sets up movesets.
 *
 * BATTLE FLOW HELPERS
 * -------------------
 *
 *   fn_80239984 (PreBattleSetup) - 491 calls
 *     One of the most-called functions. Sets up the pre-battle state:
 *     resolves the opponent, configures battle rules, starts the
 *     transition sequence.
 *
 *   fn_80239EE8 (BattleSequenceStart) - 491 calls
 *     Launches a battle sequence. Takes a full set of parameters:
 *     battle ID, trainer slot, Pokemon pointer, flags, and a sequence
 *     type ID (0xF1-0xF4).
 *
 *     The function at 0x80249000 region shows the pattern clearly:
 *       fn_80205B8C(ctx)          -> get Pokemon ptr
 *       fn_80239984(ctx, slot, seqId) -> pre-battle setup
 *       fn_80236BFC(slot, slot2, flag) -> check trainer Pokemon flag
 *       fn_80239EE8(battleId, slot, ptr, 0,0, r8, 0, 0xF1) -> start
 *
 *     Sequence type IDs:
 *       0xF1 : Initial battle setup
 *       0xF2 : Mid-battle (round 2 of multi-battle?)
 *       0xF3 : Battle continuation
 *       0xF4 : Final round
 *
 *   fn_802395C8 (BattleSequenceCheck) - 98 calls
 *   fn_8023793C (BattleResultCheck) - 98 calls
 *
 * MASSIVE FUNCTIONS SUMMARY (by size):
 *   fn_802249B8 : 6012 bytes (StoryScriptExecute)
 *   fn_8022BE2C : 4696 bytes (ColosseumRoundExecute)
 *   fn_8024E690 : 4644 bytes (unknown, in reward/post-battle region)
 *   fn_80245FC4 : 4228 bytes (unknown, in team setup region)
 *   fn_80221104 : 4108 bytes (ColosseumMatchSetup)
 *   fn_8023A740 : 3416 bytes (unknown, near BattleSequenceStart)
 *   fn_8023B498 : 3792 bytes (unknown, near BattleSequenceStart)
 *   fn_80230568 : 4500 bytes (TeamRegistration)
 *   fn_80232110 : 3096 bytes (TeamCompositionCheck)
 *   fn_802331F4 : 3004 bytes (OpponentTeamGeneration)
 *   fn_80234A0C : 3152 bytes (TeamPokemonSetup)
 *   fn_8021FAD4 : 3236 bytes (TeamValidation)
 *   fn_8022A6C8 : 2964 bytes (ColosseumBracketAdvance)
 *   fn_8022F2F8 : 2856 bytes (RewardProcessing)
 *   fn_80240BD0 : 2704 bytes (unknown)
 *   fn_801FFEC8 : 2920 bytes (unknown, in trainer/party region)
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
extern u32   fn_80142CF4(u32 context, u32 param, u16 field, u32 flags);

/* fn_801F453C: Get player party reference */
extern void* fn_801F453C(u32 param1, u32 param2);

/* fn_80205184: Get current context (unknown purpose) */
extern u32 fn_80205184(void);

/* fn_802040E8: Another context query */
extern u32 fn_802040E8(void* pokemon);

/* fn_80207BF4: Yet another context query */
extern u32 fn_80207BF4(void* pokemon);

/* fn_801EF8F4: Battle system entry point (in battle_main.c range) */
extern void fn_801EF8F4(u32 param);

/* SDA variables */
extern u32 lbl_8047B610;
extern u8 lbl_8047B626;

/* Forward declarations for converted functions */
u32 fn_80236BFC(void* ctx, u32 slot, u32 param);
u32 fn_80239984(void* context, u32 trainerSlot, u16 sequenceId);
void fn_8021FAD4(void);
void fn_80221104(u32 param1, u32 param2);
void fn_802249B8(u32 param1, u32 param2);
void fn_8022A6C8(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_8022BE2C(u32 context, u32 param);
void fn_8022F2F8(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_80230568(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_80232110(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_802331F4(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_80234A0C(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_8023793C(void* ctx, u32 param1, u32 param2, u32 param3);
void fn_802395C8(void* ctx, u32 param1, u32 param2, u32 param3);


/* =========================================================================
 * fn_802249B8 - StoryScriptExecute
 *
 * THE MAIN STORY SCRIPT INTERPRETER.
 *
 * At 6012 bytes, this is the largest function in the uncovered range and
 * arguably one of the most important functions in the entire game. It
 * processes the bytecode-like command stream that drives all story events,
 * Colosseum round progressions, and scripted interactions.
 *
 * Decompilation outline (from disassembly analysis):
 *
 * void StoryScriptExecute(u32 param1, u32 param2) {
 *     u16 partyCount;
 *     void* playerParty;
 *     void* playerSlot;
 *     void* enemySlot;
 *     u32 contextA, contextB;
 *     u8* scriptPtr;
 *     u32 activePlayer, activeEnemy;
 *     u32 flags;
 *
 *     // Setup
 *     partyCount = PokemonGet(NULL, 0, 0x14, 0);
 *     playerParty = fn_801F453C(0, 1);
 *     playerSlot = PokemonSlotLookupDefault(0x11, 0);
 *     contextA = fn_80205184();
 *     enemySlot = PokemonSlotLookupDefault(0x12, 0);
 *
 *     scriptPtr = (u8*)lbl_80478D78;  // SDA21
 *     flags = scriptPtr[3];
 *
 *     if (flags & 0x40) {
 *         // Normal perspective
 *         activePlayer = playerSlot;
 *         PokemonSet(NULL, 0, 0x47, 0, playerSlot);
 *         PokemonSet(NULL, 0, 0x4B, 0, enemySlot);
 *         scriptPtr[3] &= ~0x40;
 *     } else {
 *         // Swapped perspective
 *         activePlayer = enemySlot;
 *         PokemonSet(NULL, 0, 0x47, 0, enemySlot);
 *         PokemonSet(NULL, 0, 0x4B, 0, playerSlot);
 *     }
 *
 *     // Resolve initial Pokemon
 *     PokemonGet(activePlayer, partyCount, ...);
 *     contextB = fn_80207BF4(activePlayer);
 *     ...fn_802040E8(activePlayer)...
 *
 *     // Main command loop
 *     while (1) {
 *         u8 cmd = scriptPtr[3];
 *         if (cmd < 7 || cmd > 0x3B) {
 *             // Invalid command or end
 *             break;
 *         }
 *
 *         // Dispatch through jumptable_8039A220
 *         switch (cmd - 7) {
 *             case 0x00: // cmd 0x07
 *                 // ... handler ...
 *                 lbl_8047B610++;
 *                 break;
 *             case 0x02: // cmd 0x09 - Battle command
 *                 if (CheckEventFlag(context, 0x09)) {
 *                     SetEventState(context, 0x09, 0);
 *                 }
 *                 // ... battle setup ...
 *                 break;
 *             // ... 50 more cases ...
 *         }
 *     }
 * }
 *
 * [Full decompilation requires analysis of all 52 command handlers
 *  within the jumptable_8039A220 dispatch.]
 * ========================================================================= */
/* TODO: Decompile fn_802249B8 (6012 bytes) */
#pragma peephole off
void fn_802249B8(u32 param1, u32 param2) {
    /* StoryScriptExecute - 6012 bytes - main bytecode interpreter
     * Uses jumptable_8039A220 (52 entries) for command dispatch */
}
#pragma peephole reset

/* =========================================================================
 * fn_80221104 - ColosseumMatchSetup
 *
 * 4108 bytes. Initializes a Colosseum match by:
 *   1. Querying the bracket data for the current round's opponent
 *   2. Loading the opponent's trainer data via TrainerDataGet
 *   3. Setting up battle rules (level caps, species bans)
 *   4. Configuring the battle sequence
 *
 * Heavily references lbl_80279E7C (constant 0x7693) and uses
 * repeated calls to TrainerDataGet with field 0x43 (Pokemon pointer).
 * ========================================================================= */
/* TODO: Decompile fn_80221104 (4108 bytes) */
#pragma peephole off
void fn_80221104(u32 param1, u32 param2) {
    /* ColosseumMatchSetup - 4108 bytes */
}
#pragma peephole reset

/* =========================================================================
 * fn_8021FAD4 - TeamValidation
 *
 * 3236 bytes. Validates a team against Colosseum entry rules.
 * References lbl_80279FE0 (constants 0x000F, 0x0016) which may be
 * minimum/maximum level boundaries.
 * ========================================================================= */
/* TODO: Decompile fn_8021FAD4 (3236 bytes) */

/* =========================================================================
 * fn_8022BE2C - ColosseumRoundExecute
 *
 * 4696 bytes. Second largest function. Manages a complete battle round:
 *   1. Pre-battle: team validation, opponent setup
 *   2. Battle: calls into the battle engine
 *   3. Post-battle: reward processing, bracket advancement
 * ========================================================================= */
/* TODO: Decompile fn_8022BE2C (4696 bytes) */
#pragma peephole off
void fn_8022BE2C(u32 context, u32 param) {
    /* ColosseumRoundExecute - 4696 bytes */
}
#pragma peephole reset

/* =========================================================================
 * fn_80230568 - TeamRegistration
 *
 * 4500 bytes. Full team registration flow. Called when the player
 * enters a Colosseum and registers their team.
 * ========================================================================= */
/* TODO: Decompile fn_80230568 (4500 bytes) */
#pragma peephole off
void fn_80230568(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TeamRegistration - 4500 bytes */
}
#pragma peephole reset

/* =========================================================================
 * fn_80239984 - PreBattleSetup
 *
 * 491 call sites. One of the most important battle flow functions.
 * Sets up the pre-battle state for a trainer encounter.
 *
 * @param context     Battle/scene context
 * @param trainerSlot Trainer slot to fight
 * @param sequenceId  Sequence type (0xF1-0xF4)
 * @return            Battle setup handle
 * ========================================================================= */
/* TODO: Decompile fn_80239984 */
#pragma peephole off
u32 fn_80239984(void* context, u32 trainerSlot, u16 sequenceId) {
    /* PreBattleSetup - 120 bytes - 491 call sites */
    return 0;
}
#pragma peephole reset

/* =========================================================================
 * fn_80239EE8 - BattleSequenceStart
 *
 * 491 call sites. Launches a battle with full configuration.
 *
 * Parameters visible from calling patterns:
 *   r3 = battle ID (e.g., 0xEC64 = large constant)
 *   r4 = trainer slot
 *   r5 = Pokemon pointer (from fn_80205B8C)
 *   r6 = 0 (flags)
 *   r7 = 0 (flags)
 *   r8 = context (from caller)
 *   r9 = 0 (reserved)
 *   r10 = sequence ID (0xF1, 0xF2, 0xF3, 0xF4)
 *
 * The constant 0xEC64 (decimal 60516) appears to be a standard battle
 * configuration ID. It's loaded via:
 *   lis r6, 0x1
 *   subi r3, r6, 0x139c  -> 0x10000 - 0x139C = 0xEC64
 * ========================================================================= */
/* TODO: Decompile fn_80239EE8 */
#pragma peephole off
void fn_80239EE8(u32 battleId, u32 trainerSlot, void* pokemonPtr,
                 u32 param4, u32 param5, u32 param6,
                 u32 param7, u16 seqId) {
    /* BattleSequenceStart - 560 bytes - 491 call sites */
}
#pragma peephole reset

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

/* Address: 0x8023C368 | Size: 0x8 | Pattern: return_constant */
u32 fn_8023C368(void) { return 0; }

/* ===================================================================
 * EXPANDED FUNCTION COVERAGE
 * 243 additional functions for 0x80220000-0x80240000
 * =================================================================== */


/* -------------------------------------------------------------------
 * Colosseum Setup (0x80220000-0x80222000)
 * 5 functions
 * ------------------------------------------------------------------- */

/* Address: 0x80220778 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80220778(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802207D4 | Size: 0x94 */
#pragma peephole off
void fn_802207D4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802207D4 (148 bytes) */
}
#pragma peephole reset

/* Address: 0x80220868 | Size: 0x324 (804 bytes) */
#pragma peephole off
void fn_80220868(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80220868 (804 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80220B8C | Size: 0x4E0 (1248 bytes) */
#pragma peephole off
void fn_80220B8C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80220B8C (1248 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022106C | Size: 0x98 */
#pragma peephole off
void fn_8022106C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8022106C (152 bytes) */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Script Interpreter Helpers (0x80222000-0x80226000)
 * 53 functions
 * ------------------------------------------------------------------- */

/* Address: 0x80222110 | Size: 0xDC (220 bytes) */
#pragma peephole off
void fn_80222110(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80222110 (220 bytes) */
}
#pragma peephole reset

/* Address: 0x802221EC | Size: 0x108 (264 bytes) */
#pragma peephole off
void fn_802221EC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802221EC (264 bytes) */
}
#pragma peephole reset

/* Address: 0x802222F4 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_802222F4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222370 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80222370(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802223D4 | Size: 0xC | Pattern: simple_setter */
void fn_802223D4(u32 value) { /* stub */ }

/* Address: 0x802223E0 | Size: 0x58 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802223E0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return;
    return; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222438 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80222438(void* ctx, u32 slot, u32 param) {
    if (!ctx) return;
    return; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222494 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80222494(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x802224D0 | Size: 0xC | Pattern: simple_setter */
void fn_802224D0(u32 value) { /* stub */ }

/* Address: 0x802224DC | Size: 0xC | Pattern: simple_setter */
void fn_802224DC(u32 value) { /* stub */ }

/* Address: 0x802224E8 | Size: 0xC | Pattern: simple_setter */
void fn_802224E8(u32 value) { /* stub */ }

/* Address: 0x802224F4 | Size: 0xC | Pattern: simple_setter */
void fn_802224F4(u32 value) { /* stub */ }

/* Address: 0x80222500 | Size: 0x10 | Pattern: sda_getter */
u32 fn_80222500(void) { return; /* stub */ }

/* Address: 0x80222510 | Size: 0x10 | Pattern: sda_getter */
u32 fn_80222510(void) { return; /* stub */ }

/* Address: 0x80222520 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80222520(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x80222554 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_80222554(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x80222584 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_80222584(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x802225B0 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_802225B0(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x802225DC | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802225DC(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x80222604 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80222604(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x8022262C | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_8022262C(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x80222654 | Size: 0x50 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80222654(void* ctx, u32 slot, u32 param) {
    if (!ctx) return;
    return; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802226A4 | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802226A4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return;
    return; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802226EC | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802226EC(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x80222714 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80222714(void* ctx, u32 param) { return; /* stub */ }

/* Address: 0x8022273C | Size: 0x20 | Pattern: null_check_getter */
u32 fn_8022273C(void* ctx) { if (!ctx) return; return; /* stub */ }

/* Address: 0x8022275C | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8022275C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return;
    return; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802227D4 | Size: 0x70 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802227D4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return;
    return; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222844 | Size: 0xC8 (200 bytes) */
#pragma peephole off
void fn_80222844(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80222844 (200 bytes) */
}
#pragma peephole reset

/* Address: 0x8022290C | Size: 0xE0 (224 bytes) */
#pragma peephole off
void fn_8022290C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022290C (224 bytes) */
}
#pragma peephole reset

/* Address: 0x802229EC | Size: 0xE0 (224 bytes) */
#pragma peephole off
void fn_802229EC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802229EC (224 bytes) */
}
#pragma peephole reset

/* Address: 0x80222ACC | Size: 0x10 | Pattern: sda_getter */
u32 fn_80222ACC(void) { return 0; /* stub */ }

/* Address: 0x80222ADC | Size: 0xA0 */
#pragma peephole off
void fn_80222ADC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80222ADC (160 bytes) */
}
#pragma peephole reset

/* Address: 0x80222B7C | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80222B7C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222BD8 | Size: 0x6C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80222BD8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222C44 | Size: 0x2AC (684 bytes) */
#pragma peephole off
void fn_80222C44(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80222C44 (684 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80222EF0 | Size: 0x1CC (460 bytes) */
#pragma peephole off
void fn_80222EF0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80222EF0 (460 bytes) */
}
#pragma peephole reset

/* Address: 0x802230BC | Size: 0x238 (568 bytes) */
#pragma peephole off
void fn_802230BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802230BC (568 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802232F4 | Size: 0x730 (1840 bytes) */
#pragma peephole off
void fn_802232F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802232F4 (1840 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80223A24 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80223A24(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80223A88 | Size: 0x6C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80223A88(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80223AF4 | Size: 0x180 (384 bytes) */
#pragma peephole off
void fn_80223AF4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80223AF4 (384 bytes) */
}
#pragma peephole reset

/* Address: 0x80223C74 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80223C74(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80223CE8 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80223CE8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80223D64 | Size: 0xDC (220 bytes) */
#pragma peephole off
void fn_80223D64(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80223D64 (220 bytes) */
}
#pragma peephole reset

/* Address: 0x80223E40 | Size: 0xDC (220 bytes) */
#pragma peephole off
void fn_80223E40(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80223E40 (220 bytes) */
}
#pragma peephole reset

/* Address: 0x80223F1C | Size: 0x144 (324 bytes) */
#pragma peephole off
void fn_80223F1C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80223F1C (324 bytes) */
}
#pragma peephole reset

/* Address: 0x80224060 | Size: 0xF8 (248 bytes) */
#pragma peephole off
void fn_80224060(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80224060 (248 bytes) */
}
#pragma peephole reset

/* Address: 0x80224158 | Size: 0x5E8 (1512 bytes) */
#pragma peephole off
void fn_80224158(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80224158 (1512 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80224740 | Size: 0x90 */
#pragma peephole off
void fn_80224740(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80224740 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x802247D0 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802247D0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802247F8 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802247F8(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80224820 | Size: 0x198 (408 bytes) */
#pragma peephole off
void fn_80224820(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80224820 (408 bytes) */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Team Management & Validation (0x80226000-0x80230000)
 * 59 functions
 * ------------------------------------------------------------------- */

/* Address: 0x80226134 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80226134(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802261B0 | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_802261B0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8022622C | Size: 0x58 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8022622C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80226284 | Size: 0x4C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80226284(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802262D0 | Size: 0x4C | Pattern: field_accessor */
#pragma peephole off
u32 fn_802262D0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8022631C | Size: 0x3D0 (976 bytes) */
#pragma peephole off
void fn_8022631C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022631C (976 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802266EC | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802266EC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80226730 | Size: 0xB8 */
#pragma peephole off
void fn_80226730(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80226730 (184 bytes) */
}
#pragma peephole reset

/* Address: 0x802267E8 | Size: 0x12C (300 bytes) */
#pragma peephole off
void fn_802267E8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802267E8 (300 bytes) */
}
#pragma peephole reset

/* Address: 0x80226914 | Size: 0x5F8 (1528 bytes) */
#pragma peephole off
void fn_80226914(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80226914 (1528 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80226F0C | Size: 0xC8 (200 bytes) */
#pragma peephole off
void fn_80226F0C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80226F0C (200 bytes) */
}
#pragma peephole reset

/* Address: 0x80226FD4 | Size: 0x1A4 (420 bytes) */
#pragma peephole off
void fn_80226FD4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80226FD4 (420 bytes) */
}
#pragma peephole reset

/* Address: 0x80227178 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80227178(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802271AC | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_802271AC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802271E0 | Size: 0x2B0 (688 bytes) */
#pragma peephole off
void fn_802271E0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802271E0 (688 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80227490 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_80227490(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802274C0 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_802274C0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802274F0 | Size: 0x508 (1288 bytes) */
#pragma peephole off
void fn_802274F0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802274F0 (1288 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802279F8 | Size: 0x248 (584 bytes) */
#pragma peephole off
void fn_802279F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802279F8 (584 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80227C40 | Size: 0x178 (376 bytes) */
#pragma peephole off
void fn_80227C40(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80227C40 (376 bytes) */
}
#pragma peephole reset

/* Address: 0x80227DB8 | Size: 0x2D4 (724 bytes) */
#pragma peephole off
void fn_80227DB8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80227DB8 (724 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022808C | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8022808C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022808C (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802282D8 | Size: 0x1D8 (472 bytes) */
#pragma peephole off
void fn_802282D8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802282D8 (472 bytes) */
}
#pragma peephole reset

/* Address: 0x802284B0 | Size: 0x8FC (2300 bytes) */
#pragma peephole off
void fn_802284B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802284B0 (2300 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80228DAC | Size: 0x8D0 (2256 bytes) */
#pragma peephole off
void fn_80228DAC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80228DAC (2256 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022967C | Size: 0x88 */
#pragma peephole off
void fn_8022967C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8022967C (136 bytes) */
}
#pragma peephole reset

/* Address: 0x80229704 | Size: 0x230 (560 bytes) */
#pragma peephole off
void fn_80229704(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80229704 (560 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80229934 | Size: 0x23C (572 bytes) */
#pragma peephole off
void fn_80229934(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80229934 (572 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80229B70 | Size: 0x68 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80229B70(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80229BD8 | Size: 0x50 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80229BD8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80229C28 | Size: 0x68 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80229C28(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80229C90 | Size: 0x1C4 (452 bytes) */
#pragma peephole off
void fn_80229C90(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80229C90 (452 bytes) */
}
#pragma peephole reset

/* Address: 0x80229E54 | Size: 0x6B0 (1712 bytes) */
#pragma peephole off
void fn_80229E54(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80229E54 (1712 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022A504 | Size: 0x1C4 (452 bytes) */
#pragma peephole off
void fn_8022A504(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022A504 (452 bytes) */
}
#pragma peephole reset

/* Address: 0x8022A6C8 | Size: 0xBD4 (3028 bytes) */
#pragma peephole off
void fn_8022A6C8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022A6C8 (3028 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022B29C | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_8022B29C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8022B2CC | Size: 0x2FC (764 bytes) */
#pragma peephole off
void fn_8022B2CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022B2CC (764 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022B5C8 | Size: 0x5BC (1468 bytes) */
#pragma peephole off
void fn_8022B5C8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022B5C8 (1468 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022BB84 | Size: 0x2A8 (680 bytes) */
#pragma peephole off
void fn_8022BB84(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022BB84 (680 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022D084 | Size: 0x188 (392 bytes) */
#pragma peephole off
void fn_8022D084(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022D084 (392 bytes) */
}
#pragma peephole reset

/* Address: 0x8022D20C | Size: 0xC0 */
#pragma peephole off
void fn_8022D20C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8022D20C (192 bytes) */
}
#pragma peephole reset

/* Address: 0x8022D2CC | Size: 0xC8 (200 bytes) */
#pragma peephole off
void fn_8022D2CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022D2CC (200 bytes) */
}
#pragma peephole reset

/* Address: 0x8022D394 | Size: 0x328 (808 bytes) */
#pragma peephole off
void fn_8022D394(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022D394 (808 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022D6BC | Size: 0x5FC (1532 bytes) */
#pragma peephole off
void fn_8022D6BC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022D6BC (1532 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022DCB8 | Size: 0x250 (592 bytes) */
#pragma peephole off
void fn_8022DCB8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022DCB8 (592 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022DF08 | Size: 0x2BC (700 bytes) */
#pragma peephole off
void fn_8022DF08(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022DF08 (700 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022E1C4 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_8022E1C4(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8022E1F8 | Size: 0x11C (284 bytes) */
#pragma peephole off
void fn_8022E1F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022E1F8 (284 bytes) */
}
#pragma peephole reset

/* Address: 0x8022E314 | Size: 0x38 | Pattern: simple_wrapper */
u32 fn_8022E314(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8022E34C | Size: 0xC4 (196 bytes) */
#pragma peephole off
void fn_8022E34C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022E34C (196 bytes) */
}
#pragma peephole reset

/* Address: 0x8022E410 | Size: 0x2E0 (736 bytes) */
#pragma peephole off
void fn_8022E410(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022E410 (736 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022E6F0 | Size: 0x4AC (1196 bytes) */
#pragma peephole off
void fn_8022E6F0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022E6F0 (1196 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022EB9C | Size: 0xA4 */
#pragma peephole off
void fn_8022EB9C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8022EB9C (164 bytes) */
}
#pragma peephole reset

/* Address: 0x8022EC40 | Size: 0x1AC (428 bytes) */
#pragma peephole off
void fn_8022EC40(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022EC40 (428 bytes) */
}
#pragma peephole reset

/* Address: 0x8022EDEC | Size: 0x50C (1292 bytes) */
#pragma peephole off
void fn_8022EDEC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022EDEC (1292 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022F2F8 | Size: 0xB28 (2856 bytes) */
#pragma peephole off
void fn_8022F2F8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022F2F8 (2856 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8022FE20 | Size: 0x60 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8022FE20(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8022FE80 | Size: 0x110 (272 bytes) */
#pragma peephole off
void fn_8022FE80(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022FE80 (272 bytes) */
}
#pragma peephole reset

/* Address: 0x8022FF90 | Size: 0xF8 (248 bytes) */
#pragma peephole off
void fn_8022FF90(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8022FF90 (248 bytes) */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Tournament Flow (0x80230000-0x80236000)
 * 27 functions
 * ------------------------------------------------------------------- */

/* Address: 0x80230088 | Size: 0x94 */
#pragma peephole off
void fn_80230088(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80230088 (148 bytes) */
}
#pragma peephole reset

/* Address: 0x8023011C | Size: 0x8C */
#pragma peephole off
void fn_8023011C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023011C (140 bytes) */
}
#pragma peephole reset

/* Address: 0x802301A8 | Size: 0x170 (368 bytes) */
#pragma peephole off
void fn_802301A8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802301A8 (368 bytes) */
}
#pragma peephole reset

/* Address: 0x80230318 | Size: 0x250 (592 bytes) */
#pragma peephole off
void fn_80230318(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80230318 (592 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802316FC | Size: 0xE8 (232 bytes) */
#pragma peephole off
void fn_802316FC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802316FC (232 bytes) */
}
#pragma peephole reset

/* Address: 0x802317E4 | Size: 0x7E4 (2020 bytes) */
#pragma peephole off
void fn_802317E4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802317E4 (2020 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80231FC8 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80231FC8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80232024 | Size: 0xEC (236 bytes) */
#pragma peephole off
void fn_80232024(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80232024 (236 bytes) */
}
#pragma peephole reset

/* Address: 0x80232110 | Size: 0xC18 (3096 bytes) */
#pragma peephole off
void fn_80232110(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80232110 (3096 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80232D28 | Size: 0x2BC (700 bytes) */
#pragma peephole off
void fn_80232D28(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80232D28 (700 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80232FE4 | Size: 0x1C0 (448 bytes) */
#pragma peephole off
void fn_80232FE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80232FE4 (448 bytes) */
}
#pragma peephole reset

/* Address: 0x802331A4 | Size: 0x50 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802331A4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802331F4 | Size: 0xBBC (3004 bytes) */
#pragma peephole off
void fn_802331F4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802331F4 (3004 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80233DB0 | Size: 0x51C (1308 bytes) */
#pragma peephole off
void fn_80233DB0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80233DB0 (1308 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802342CC | Size: 0x740 (1856 bytes) */
#pragma peephole off
void fn_802342CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802342CC (1856 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80234A0C | Size: 0xC50 (3152 bytes) */
#pragma peephole off
void fn_80234A0C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80234A0C (3152 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023565C | Size: 0xB8 */
#pragma peephole off
void fn_8023565C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023565C (184 bytes) */
}
#pragma peephole reset

/* Address: 0x80235714 | Size: 0xB8 */
#pragma peephole off
void fn_80235714(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80235714 (184 bytes) */
}
#pragma peephole reset

/* Address: 0x802357CC | Size: 0xE0 (224 bytes) */
#pragma peephole off
void fn_802357CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802357CC (224 bytes) */
}
#pragma peephole reset

/* Address: 0x802358AC | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802358AC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80235910 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80235910(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80235974 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80235974(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802359D8 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802359D8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80235A3C | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80235A3C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80235AA0 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80235AA0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80235B04 | Size: 0xE0 (224 bytes) */
#pragma peephole off
void fn_80235B04(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80235B04 (224 bytes) */
}
#pragma peephole reset

/* Address: 0x80235BE4 | Size: 0x684 (1668 bytes) */
#pragma peephole off
void fn_80235BE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80235BE4 (1668 bytes) - complex function */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Battle Flow Helpers (0x80236000-0x8023A000)
 * 64 functions
 * ------------------------------------------------------------------- */

/* Address: 0x80236268 | Size: 0x1F0 (496 bytes) */
#pragma peephole off
void fn_80236268(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80236268 (496 bytes) */
}
#pragma peephole reset

/* Address: 0x80236458 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80236458(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802364BC | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802364BC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80236520 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80236520(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80236584 | Size: 0x248 (584 bytes) */
#pragma peephole off
void fn_80236584(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80236584 (584 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x802367CC | Size: 0x1EC (492 bytes) */
#pragma peephole off
void fn_802367CC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802367CC (492 bytes) */
}
#pragma peephole reset

/* Address: 0x802369B8 | Size: 0x1E0 (480 bytes) */
#pragma peephole off
void fn_802369B8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802369B8 (480 bytes) */
}
#pragma peephole reset

/* Address: 0x80236B98 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80236B98(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80236BFC | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80236BFC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80236C80 | Size: 0xE0 (224 bytes) */
#pragma peephole off
void fn_80236C80(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80236C80 (224 bytes) */
}
#pragma peephole reset

/* Address: 0x80236D60 | Size: 0x13C (316 bytes) */
#pragma peephole off
void fn_80236D60(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80236D60 (316 bytes) */
}
#pragma peephole reset

/* Address: 0x80236E9C | Size: 0xB0 */
#pragma peephole off
void fn_80236E9C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80236E9C (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80236F4C | Size: 0xB0 */
#pragma peephole off
void fn_80236F4C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80236F4C (176 bytes) */
}
#pragma peephole reset

/* Address: 0x80236FFC | Size: 0xB0 */
#pragma peephole off
void fn_80236FFC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80236FFC (176 bytes) */
}
#pragma peephole reset

/* Address: 0x802370AC | Size: 0xB0 */
#pragma peephole off
void fn_802370AC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802370AC (176 bytes) */
}
#pragma peephole reset

/* Address: 0x8023715C | Size: 0xB0 */
#pragma peephole off
void fn_8023715C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023715C (176 bytes) */
}
#pragma peephole reset

/* Address: 0x8023720C | Size: 0x7C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8023720C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80237288 | Size: 0x88 */
#pragma peephole off
void fn_80237288(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80237288 (136 bytes) */
}
#pragma peephole reset

/* Address: 0x80237310 | Size: 0xA0 */
#pragma peephole off
void fn_80237310(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80237310 (160 bytes) */
}
#pragma peephole reset

/* Address: 0x802373B0 | Size: 0x18C (396 bytes) */
#pragma peephole off
void fn_802373B0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802373B0 (396 bytes) */
}
#pragma peephole reset

/* Address: 0x8023753C | Size: 0x128 (296 bytes) */
#pragma peephole off
void fn_8023753C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023753C (296 bytes) */
}
#pragma peephole reset

/* Address: 0x80237664 | Size: 0x88 */
#pragma peephole off
void fn_80237664(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80237664 (136 bytes) */
}
#pragma peephole reset

/* Address: 0x802376EC | Size: 0x88 */
#pragma peephole off
void fn_802376EC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802376EC (136 bytes) */
}
#pragma peephole reset

/* Address: 0x80237774 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80237774(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802377E8 | Size: 0x74 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802377E8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8023785C | Size: 0xE0 (224 bytes) */
#pragma peephole off
void fn_8023785C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023785C (224 bytes) */
}
#pragma peephole reset

/* Address: 0x8023793C | Size: 0x37C (892 bytes) */
#pragma peephole off
void fn_8023793C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023793C (892 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80237CB8 | Size: 0x104 (260 bytes) */
#pragma peephole off
void fn_80237CB8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80237CB8 (260 bytes) */
}
#pragma peephole reset

/* Address: 0x80237DBC | Size: 0x1B8 (440 bytes) */
#pragma peephole off
void fn_80237DBC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80237DBC (440 bytes) */
}
#pragma peephole reset

/* Address: 0x80237F74 | Size: 0xEC (236 bytes) */
#pragma peephole off
void fn_80237F74(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80237F74 (236 bytes) */
}
#pragma peephole reset

/* Address: 0x80238060 | Size: 0x164 (356 bytes) */
#pragma peephole off
void fn_80238060(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80238060 (356 bytes) */
}
#pragma peephole reset

/* Address: 0x802381C4 | Size: 0xAC */
#pragma peephole off
void fn_802381C4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802381C4 (172 bytes) */
}
#pragma peephole reset

/* Address: 0x80238270 | Size: 0xAC */
#pragma peephole off
void fn_80238270(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80238270 (172 bytes) */
}
#pragma peephole reset

/* Address: 0x8023831C | Size: 0x88 */
#pragma peephole off
void fn_8023831C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023831C (136 bytes) */
}
#pragma peephole reset

/* Address: 0x802383A4 | Size: 0x88 */
#pragma peephole off
void fn_802383A4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802383A4 (136 bytes) */
}
#pragma peephole reset

/* Address: 0x8023842C | Size: 0x88 */
#pragma peephole off
void fn_8023842C(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023842C (136 bytes) */
}
#pragma peephole reset

/* Address: 0x802384B4 | Size: 0x84 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802384B4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80238538 | Size: 0xC8 (200 bytes) */
#pragma peephole off
void fn_80238538(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80238538 (200 bytes) */
}
#pragma peephole reset

/* Address: 0x80238600 | Size: 0xC8 (200 bytes) */
#pragma peephole off
void fn_80238600(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80238600 (200 bytes) */
}
#pragma peephole reset

/* Address: 0x802386C8 | Size: 0x80 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802386C8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80238748 | Size: 0x80 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80238748(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802387C8 | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802387C8(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x8023881C | Size: 0x110 (272 bytes) */
#pragma peephole off
void fn_8023881C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023881C (272 bytes) */
}
#pragma peephole reset

/* Address: 0x8023892C | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8023892C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80238980 | Size: 0x54 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80238980(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802389D4 | Size: 0x138 (312 bytes) */
#pragma peephole off
void fn_802389D4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802389D4 (312 bytes) */
}
#pragma peephole reset

/* Address: 0x80238B0C | Size: 0x324 (804 bytes) */
#pragma peephole off
void fn_80238B0C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80238B0C (804 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80238E30 | Size: 0x228 (552 bytes) */
#pragma peephole off
void fn_80238E30(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80238E30 (552 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80239058 | Size: 0xFC (252 bytes) */
#pragma peephole off
void fn_80239058(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80239058 (252 bytes) */
}
#pragma peephole reset

/* Address: 0x80239154 | Size: 0x8C */
#pragma peephole off
void fn_80239154(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_80239154 (140 bytes) */
}
#pragma peephole reset

/* Address: 0x802391E0 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802391E0(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80239244 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80239244(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802392A8 | Size: 0xF8 (248 bytes) */
#pragma peephole off
void fn_802392A8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802392A8 (248 bytes) */
}
#pragma peephole reset

/* Address: 0x802393A0 | Size: 0x9C */
#pragma peephole off
void fn_802393A0(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802393A0 (156 bytes) */
}
#pragma peephole reset

/* Address: 0x8023943C | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_8023943C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80239498 | Size: 0x68 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80239498(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80239500 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80239500(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80239564 | Size: 0x64 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80239564(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802395C8 | Size: 0x1F0 (496 bytes) */
#pragma peephole off
void fn_802395C8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802395C8 (496 bytes) */
}
#pragma peephole reset

/* Address: 0x802397B8 | Size: 0x12C (300 bytes) */
#pragma peephole off
void fn_802397B8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_802397B8 (300 bytes) */
}
#pragma peephole reset

/* Address: 0x802398E4 | Size: 0xA0 */
#pragma peephole off
void fn_802398E4(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_802398E4 (160 bytes) */
}
#pragma peephole reset

/* Address: 0x802399FC | Size: 0x44 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802399FC(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80239A40 | Size: 0x28C (652 bytes) */
#pragma peephole off
void fn_80239A40(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80239A40 (652 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x80239CCC | Size: 0x21C (540 bytes) */
#pragma peephole off
void fn_80239CCC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_80239CCC (540 bytes) - complex function */
}
#pragma peephole reset


/* -------------------------------------------------------------------
 * Battle Sequence & Pre-Battle (0x8023A000-0x80240000)
 * 35 functions
 * ------------------------------------------------------------------- */

/* Address: 0x8023A118 | Size: 0x1F0 (496 bytes) */
#pragma peephole off
void fn_8023A118(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023A118 (496 bytes) */
}
#pragma peephole reset

/* Address: 0x8023A308 | Size: 0x438 (1080 bytes) */
#pragma peephole off
void fn_8023A308(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023A308 (1080 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023A740 | Size: 0xD58 (3416 bytes) */
#pragma peephole off
void fn_8023A740(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023A740 (3416 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023B498 | Size: 0xED0 (3792 bytes) */
#pragma peephole off
void fn_8023B498(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023B498 (3792 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023C370 | Size: 0x1C0 (448 bytes) */
#pragma peephole off
void fn_8023C370(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023C370 (448 bytes) */
}
#pragma peephole reset

/* Address: 0x8023C530 | Size: 0x56C (1388 bytes) */
#pragma peephole off
void fn_8023C530(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023C530 (1388 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023CA9C | Size: 0xC4 (196 bytes) */
#pragma peephole off
void fn_8023CA9C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023CA9C (196 bytes) */
}
#pragma peephole reset

/* Address: 0x8023CB60 | Size: 0x140 (320 bytes) */
#pragma peephole off
void fn_8023CB60(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023CB60 (320 bytes) */
}
#pragma peephole reset

/* Address: 0x8023CCA0 | Size: 0x12C (300 bytes) */
#pragma peephole off
void fn_8023CCA0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023CCA0 (300 bytes) */
}
#pragma peephole reset

/* Address: 0x8023CDCC | Size: 0x94 */
#pragma peephole off
void fn_8023CDCC(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023CDCC (148 bytes) */
}
#pragma peephole reset

/* Address: 0x8023CE60 | Size: 0x17C (380 bytes) */
#pragma peephole off
void fn_8023CE60(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023CE60 (380 bytes) */
}
#pragma peephole reset

/* Address: 0x8023CFDC | Size: 0x17C (380 bytes) */
#pragma peephole off
void fn_8023CFDC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023CFDC (380 bytes) */
}
#pragma peephole reset

/* Address: 0x8023D158 | Size: 0x1CC (460 bytes) */
#pragma peephole off
void fn_8023D158(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023D158 (460 bytes) */
}
#pragma peephole reset

/* Address: 0x8023D324 | Size: 0x15C (348 bytes) */
#pragma peephole off
void fn_8023D324(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023D324 (348 bytes) */
}
#pragma peephole reset

/* Address: 0x8023D480 | Size: 0x90 */
#pragma peephole off
void fn_8023D480(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023D480 (144 bytes) */
}
#pragma peephole reset

/* Address: 0x8023D510 | Size: 0x23C (572 bytes) */
#pragma peephole off
void fn_8023D510(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023D510 (572 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023D74C | Size: 0x200 (512 bytes) */
#pragma peephole off
void fn_8023D74C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023D74C (512 bytes) */
}
#pragma peephole reset

/* Address: 0x8023D94C | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023D94C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023D94C (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023DB98 | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023DB98(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023DB98 (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023DDE4 | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023DDE4(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023DDE4 (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023E030 | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023E030(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023E030 (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023E27C | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023E27C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023E27C (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023E4C8 | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023E4C8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023E4C8 (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023E714 | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023E714(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023E714 (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023E960 | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023E960(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023E960 (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023EBAC | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023EBAC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023EBAC (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023EDF8 | Size: 0x24C (588 bytes) */
#pragma peephole off
void fn_8023EDF8(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023EDF8 (588 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023F044 | Size: 0x100 (256 bytes) */
#pragma peephole off
void fn_8023F044(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023F044 (256 bytes) */
}
#pragma peephole reset

/* Address: 0x8023F144 | Size: 0x134 (308 bytes) */
#pragma peephole off
void fn_8023F144(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023F144 (308 bytes) */
}
#pragma peephole reset

/* Address: 0x8023F278 | Size: 0x648 (1608 bytes) */
#pragma peephole off
void fn_8023F278(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023F278 (1608 bytes) - complex function */
}
#pragma peephole reset

/* Address: 0x8023F8C0 | Size: 0x15C (348 bytes) */
#pragma peephole off
void fn_8023F8C0(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023F8C0 (348 bytes) */
}
#pragma peephole reset

/* Address: 0x8023FA1C | Size: 0x140 (320 bytes) */
#pragma peephole off
void fn_8023FA1C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023FA1C (320 bytes) */
}
#pragma peephole reset

/* Address: 0x8023FB5C | Size: 0x1E8 (488 bytes) */
#pragma peephole off
void fn_8023FB5C(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023FB5C (488 bytes) */
}
#pragma peephole reset

/* Address: 0x8023FD44 | Size: 0x98 */
#pragma peephole off
void fn_8023FD44(void* ctx, u32 param1, u32 param2) {
    /* TODO: Decompile fn_8023FD44 (152 bytes) */
}
#pragma peephole reset

/* Address: 0x8023FDDC | Size: 0x2FC (764 bytes) */
#pragma peephole off
void fn_8023FDDC(void* ctx, u32 param1, u32 param2, u32 param3) {
    /* TODO: Decompile fn_8023FDDC (764 bytes) - complex function */
}
#pragma peephole reset

/* #######################################################################
 * COVERAGE STUBS: Colosseum script system (0x80212000 - 0x80220000)
 * 212 functions remaining for full coverage of the target range
 * within colosseum_script.c TU.
 *
 * Key functions in this range:
 *   fn_80212D6C - Colosseum round transition handler
 *   fn_80213xxx - Event state management helpers
 *   fn_80214xxx - Scene transition helpers
 *   fn_80215xxx-0x80220000 - Colosseum match setup, team validation
 * ####################################################################### */

#pragma push
#pragma force_active on

/* 0x802126C4 | size: 0x17C | medium */
#pragma peephole off
void fn_802126C4(void) {
    /* TODO: decompile (0x17C bytes, ~95 instructions) */
}
#pragma peephole reset

/* 0x80212840 | size: 0x90 | medium */
#pragma peephole off
void fn_80212840(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x802128D0 | size: 0x49C | large */
#pragma peephole off
void fn_802128D0(void) {
    /* TODO: decompile (0x49C bytes, ~295 instructions) */
}
#pragma peephole reset

/* 0x80212D6C | size: 0x3EC | large */
#pragma peephole off
void fn_80212D6C(void) {
    /* TODO: decompile (0x3EC bytes, ~251 instructions) */
}
#pragma peephole reset

/* 0x80213158 | size: 0x118 | medium */
#pragma peephole off
void fn_80213158(void) {
    /* TODO: decompile (0x118 bytes, ~70 instructions) */
}
#pragma peephole reset

/* 0x80213270 | size: 0x264 | large */
#pragma peephole off
void fn_80213270(void) {
    /* TODO: decompile (0x264 bytes, ~153 instructions) */
}
#pragma peephole reset

/* 0x802134D4 | size: 0x84 | medium */
#pragma peephole off
void fn_802134D4(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x80213558 | size: 0x14C | medium */
#pragma peephole off
void fn_80213558(void) {
    /* TODO: decompile (0x14C bytes, ~83 instructions) */
}
#pragma peephole reset

/* 0x802136A4 | size: 0x24 | small */
void fn_802136A4(void) {
    /* TODO: decompile (0x24 bytes) */
}

/* 0x802136C8 | size: 0x88 | medium */
#pragma peephole off
void fn_802136C8(void) {
    /* TODO: decompile (0x88 bytes, ~34 instructions) */
}
#pragma peephole reset

/* 0x80213750 | size: 0x88 | medium */
#pragma peephole off
void fn_80213750(void) {
    /* TODO: decompile (0x88 bytes, ~34 instructions) */
}
#pragma peephole reset

/* 0x802137D8 | size: 0x88 | medium */
#pragma peephole off
void fn_802137D8(void) {
    /* TODO: decompile (0x88 bytes, ~34 instructions) */
}
#pragma peephole reset

/* 0x80213860 | size: 0x90 | medium */
#pragma peephole off
void fn_80213860(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x802138F0 | size: 0x90 | medium */
#pragma peephole off
void fn_802138F0(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x80213980 | size: 0x90 | medium */
#pragma peephole off
void fn_80213980(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x80213A10 | size: 0xC | tiny */
void fn_80213A10(void) { }

/* 0x80213A1C | size: 0xC | tiny */
void fn_80213A1C(void) { }

/* 0x80213A28 | size: 0x10 | tiny */
void fn_80213A28(void) { }

/* 0x80213A38 | size: 0x10 | tiny */
void fn_80213A38(void) { }

/* 0x80213A48 | size: 0x10 | tiny */
void fn_80213A48(void) { }

/* 0x80213A58 | size: 0x10 | tiny */
void fn_80213A58(void) { }

/* 0x80213A68 | size: 0x10 | tiny */
void fn_80213A68(void) { }

/* 0x80213A78 | size: 0x41C | large */
#pragma peephole off
void fn_80213A78(void) {
    /* TODO: decompile (0x41C bytes, ~263 instructions) */
}
#pragma peephole reset

/* 0x80213E94 | size: 0x5BC | large */
#pragma peephole off
void fn_80213E94(void) {
    /* TODO: decompile (0x5BC bytes, ~367 instructions) */
}
#pragma peephole reset

/* 0x80214450 | size: 0x178 | medium */
#pragma peephole off
void fn_80214450(void) {
    /* TODO: decompile (0x178 bytes, ~94 instructions) */
}
#pragma peephole reset

/* 0x802145C8 | size: 0xEC | medium */
#pragma peephole off
void fn_802145C8(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x802146B4 | size: 0x10 | tiny */
void fn_802146B4(void) { }

/* 0x802146C4 | size: 0xD0 | medium */
#pragma peephole off
void fn_802146C4(void) {
    /* TODO: decompile (0xD0 bytes, ~52 instructions) */
}
#pragma peephole reset

/* 0x80214794 | size: 0xD0 | medium */
#pragma peephole off
void fn_80214794(void) {
    /* TODO: decompile (0xD0 bytes, ~52 instructions) */
}
#pragma peephole reset

/* 0x80214864 | size: 0x154 | medium */
#pragma peephole off
void fn_80214864(void) {
    /* TODO: decompile (0x154 bytes, ~85 instructions) */
}
#pragma peephole reset

/* 0x802149B8 | size: 0xFC | medium */
#pragma peephole off
void fn_802149B8(void) {
    /* TODO: decompile (0xFC bytes, ~63 instructions) */
}
#pragma peephole reset

/* 0x80214AB4 | size: 0x48 | small */
void fn_80214AB4(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x80214AFC | size: 0x5C | small */
void fn_80214AFC(void) {
    /* TODO: decompile (0x5C bytes) */
}

/* 0x80214B58 | size: 0x10 | tiny */
void fn_80214B58(void) { }

/* 0x80214B68 | size: 0x4C | small */
void fn_80214B68(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x80214BB4 | size: 0x50 | small */
void fn_80214BB4(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x80214C04 | size: 0xAC | medium */
#pragma peephole off
void fn_80214C04(void) {
    /* TODO: decompile (0xAC bytes, ~43 instructions) */
}
#pragma peephole reset

/* 0x80214CB0 | size: 0x4C | small */
void fn_80214CB0(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x80214CFC | size: 0xB4 | medium */
#pragma peephole off
void fn_80214CFC(void) {
    /* TODO: decompile (0xB4 bytes, ~45 instructions) */
}
#pragma peephole reset

/* 0x80214DB0 | size: 0xA0 | medium */
#pragma peephole off
void fn_80214DB0(void) {
    /* TODO: decompile (0xA0 bytes, ~40 instructions) */
}
#pragma peephole reset

/* 0x80214E50 | size: 0xC0 | medium */
#pragma peephole off
void fn_80214E50(void) {
    /* TODO: decompile (0xC0 bytes, ~48 instructions) */
}
#pragma peephole reset

/* 0x80214F10 | size: 0xF8 | medium */
#pragma peephole off
void fn_80214F10(void) {
    /* TODO: decompile (0xF8 bytes, ~62 instructions) */
}
#pragma peephole reset

/* 0x80215008 | size: 0x1B8 | medium */
#pragma peephole off
void fn_80215008(void) {
    /* TODO: decompile (0x1B8 bytes, ~110 instructions) */
}
#pragma peephole reset

/* 0x802151C0 | size: 0xE8 | medium */
#pragma peephole off
void fn_802151C0(void) {
    /* TODO: decompile (0xE8 bytes, ~58 instructions) */
}
#pragma peephole reset

/* 0x802152A8 | size: 0x58 | small */
void fn_802152A8(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x80215300 | size: 0x74 | small */
void fn_80215300(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x80215374 | size: 0x1B4 | medium */
#pragma peephole off
void fn_80215374(void) {
    /* TODO: decompile (0x1B4 bytes, ~109 instructions) */
}
#pragma peephole reset

/* 0x80215528 | size: 0xEC | medium */
#pragma peephole off
void fn_80215528(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x80215614 | size: 0x10C | medium */
#pragma peephole off
void fn_80215614(void) {
    /* TODO: decompile (0x10C bytes, ~67 instructions) */
}
#pragma peephole reset

/* 0x80215720 | size: 0xE8 | medium */
#pragma peephole off
void fn_80215720(void) {
    /* TODO: decompile (0xE8 bytes, ~58 instructions) */
}
#pragma peephole reset

/* 0x80215808 | size: 0xC8 | medium */
#pragma peephole off
void fn_80215808(void) {
    /* TODO: decompile (0xC8 bytes, ~50 instructions) */
}
#pragma peephole reset

/* 0x802158D0 | size: 0x84 | medium */
#pragma peephole off
void fn_802158D0(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x80215954 | size: 0x124 | medium */
#pragma peephole off
void fn_80215954(void) {
    /* TODO: decompile (0x124 bytes, ~73 instructions) */
}
#pragma peephole reset

/* 0x80215A78 | size: 0x74 | small */
void fn_80215A78(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x80215AEC | size: 0x184 | medium */
#pragma peephole off
void fn_80215AEC(void) {
    /* TODO: decompile (0x184 bytes, ~97 instructions) */
}
#pragma peephole reset

/* 0x80215C70 | size: 0x80 | small */
void fn_80215C70(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x80215CF0 | size: 0x358 | large */
#pragma peephole off
void fn_80215CF0(void) {
    /* TODO: decompile (0x358 bytes, ~214 instructions) */
}
#pragma peephole reset

/* 0x80216048 | size: 0xA4 | medium */
#pragma peephole off
void fn_80216048(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x802160EC | size: 0x104 | medium */
#pragma peephole off
void fn_802160EC(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x802161F0 | size: 0x74 | small */
void fn_802161F0(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x80216264 | size: 0x8C | medium */
#pragma peephole off
void fn_80216264(void) {
    /* TODO: decompile (0x8C bytes, ~35 instructions) */
}
#pragma peephole reset

/* 0x802162F0 | size: 0x74 | small */
void fn_802162F0(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x80216364 | size: 0xAC | medium */
#pragma peephole off
void fn_80216364(void) {
    /* TODO: decompile (0xAC bytes, ~43 instructions) */
}
#pragma peephole reset

/* 0x80216410 | size: 0x140 | medium */
#pragma peephole off
void fn_80216410(void) {
    /* TODO: decompile (0x140 bytes, ~80 instructions) */
}
#pragma peephole reset

/* 0x80216550 | size: 0x64 | small */
void fn_80216550(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x802165B4 | size: 0x9C | medium */
#pragma peephole off
void fn_802165B4(void) {
    /* TODO: decompile (0x9C bytes, ~39 instructions) */
}
#pragma peephole reset

/* 0x80216650 | size: 0x130 | medium */
#pragma peephole off
void fn_80216650(void) {
    /* TODO: decompile (0x130 bytes, ~76 instructions) */
}
#pragma peephole reset

/* 0x80216780 | size: 0x84 | medium */
#pragma peephole off
void fn_80216780(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x80216804 | size: 0x70 | small */
void fn_80216804(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x80216874 | size: 0xEC | medium */
#pragma peephole off
void fn_80216874(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x80216960 | size: 0xF8 | medium */
#pragma peephole off
void fn_80216960(void) {
    /* TODO: decompile (0xF8 bytes, ~62 instructions) */
}
#pragma peephole reset

/* 0x80216A58 | size: 0x2A0 | large */
#pragma peephole off
void fn_80216A58(void) {
    /* TODO: decompile (0x2A0 bytes, ~168 instructions) */
}
#pragma peephole reset

/* 0x80216CF8 | size: 0xA4 | medium */
#pragma peephole off
void fn_80216CF8(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x80216D9C | size: 0x1B4 | medium */
#pragma peephole off
void fn_80216D9C(void) {
    /* TODO: decompile (0x1B4 bytes, ~109 instructions) */
}
#pragma peephole reset

/* 0x80216F50 | size: 0xC8 | medium */
#pragma peephole off
void fn_80216F50(void) {
    /* TODO: decompile (0xC8 bytes, ~50 instructions) */
}
#pragma peephole reset

/* 0x80217018 | size: 0x9C | medium */
#pragma peephole off
void fn_80217018(void) {
    /* TODO: decompile (0x9C bytes, ~39 instructions) */
}
#pragma peephole reset

/* 0x802170B4 | size: 0x108 | medium */
#pragma peephole off
void fn_802170B4(void) {
    /* TODO: decompile (0x108 bytes, ~66 instructions) */
}
#pragma peephole reset

/* 0x802171BC | size: 0x64 | small */
void fn_802171BC(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x80217220 | size: 0x1B4 | medium */
#pragma peephole off
void fn_80217220(void) {
    /* TODO: decompile (0x1B4 bytes, ~109 instructions) */
}
#pragma peephole reset

/* 0x802173D4 | size: 0x60 | small */
void fn_802173D4(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x80217434 | size: 0xF0 | medium */
#pragma peephole off
void fn_80217434(void) {
    /* TODO: decompile (0xF0 bytes, ~60 instructions) */
}
#pragma peephole reset

/* 0x80217524 | size: 0x84 | medium */
#pragma peephole off
void fn_80217524(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x802175A8 | size: 0x23C | large */
#pragma peephole off
void fn_802175A8(void) {
    /* TODO: decompile (0x23C bytes, ~143 instructions) */
}
#pragma peephole reset

/* 0x802177E4 | size: 0x110 | medium */
#pragma peephole off
void fn_802177E4(void) {
    /* TODO: decompile (0x110 bytes, ~68 instructions) */
}
#pragma peephole reset

/* 0x802178F4 | size: 0xA8 | medium */
#pragma peephole off
void fn_802178F4(void) {
    /* TODO: decompile (0xA8 bytes, ~42 instructions) */
}
#pragma peephole reset

/* 0x8021799C | size: 0x148 | medium */
#pragma peephole off
void fn_8021799C(void) {
    /* TODO: decompile (0x148 bytes, ~82 instructions) */
}
#pragma peephole reset

/* 0x80217AE4 | size: 0xEC | medium */
#pragma peephole off
void fn_80217AE4(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x80217BD0 | size: 0x1C */
u16 fn_80217BD0(u16 val) {
    return (u16)(((s32)(0xFF - (u16)val) * 10) / 25);
}

/* 0x80217BEC | size: 0x18 */
u16 fn_80217BEC(u16 val) {
    return (u16)(((s32)(u16)val * 10) / 25);
}

/* 0x80217C04 | size: 0x130 | medium */
#pragma peephole off
void fn_80217C04(void) {
    /* TODO: decompile (0x130 bytes, ~76 instructions) */
}
#pragma peephole reset

/* 0x80217D34 | size: 0xEC | medium */
#pragma peephole off
void fn_80217D34(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x80217E20 | size: 0x1F8 | medium */
#pragma peephole off
void fn_80217E20(void) {
    /* TODO: decompile (0x1F8 bytes, ~126 instructions) */
}
#pragma peephole reset

/* 0x80218018 | size: 0x1C0 | medium */
#pragma peephole off
void fn_80218018(void) {
    /* TODO: decompile (0x1C0 bytes, ~112 instructions) */
}
#pragma peephole reset

/* 0x802181D8 | size: 0x98 | medium */
#pragma peephole off
void fn_802181D8(void) {
    /* TODO: decompile (0x98 bytes, ~38 instructions) */
}
#pragma peephole reset

/* 0x80218270 | size: 0x64 | small */
void fn_80218270(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x802182D4 | size: 0xE8 | medium */
#pragma peephole off
void fn_802182D4(void) {
    /* TODO: decompile (0xE8 bytes, ~58 instructions) */
}
#pragma peephole reset

/* 0x802183BC | size: 0xC0 | medium */
#pragma peephole off
void fn_802183BC(void) {
    /* TODO: decompile (0xC0 bytes, ~48 instructions) */
}
#pragma peephole reset

/* 0x8021847C | size: 0x3A8 | large */
#pragma peephole off
void fn_8021847C(void) {
    /* TODO: decompile (0x3A8 bytes, ~234 instructions) */
}
#pragma peephole reset

/* 0x80218824 | size: 0x248 | large */
#pragma peephole off
void fn_80218824(void) {
    /* TODO: decompile (0x248 bytes, ~146 instructions) */
}
#pragma peephole reset

/* 0x80218A6C | size: 0x100 | medium */
#pragma peephole off
void fn_80218A6C(void) {
    /* TODO: decompile (0x100 bytes, ~64 instructions) */
}
#pragma peephole reset

/* 0x80218B6C | size: 0x68 | small */
void fn_80218B6C(void) {
    /* TODO: decompile (0x68 bytes) */
}

/* 0x80218BD4 | size: 0xA0 | medium */
#pragma peephole off
void fn_80218BD4(void) {
    /* TODO: decompile (0xA0 bytes, ~40 instructions) */
}
#pragma peephole reset

/* 0x80218C74 | size: 0xB0 | medium */
#pragma peephole off
void fn_80218C74(void) {
    /* TODO: decompile (0xB0 bytes, ~44 instructions) */
}
#pragma peephole reset

/* 0x80218D24 | size: 0x2B8 | large */
#pragma peephole off
void fn_80218D24(void) {
    /* TODO: decompile (0x2B8 bytes, ~174 instructions) */
}
#pragma peephole reset

/* 0x80218FDC | size: 0x40 | small */
void fn_80218FDC(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8021901C | size: 0x70 | small */
void fn_8021901C(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8021908C | size: 0x1E4 | medium */
#pragma peephole off
void fn_8021908C(void) {
    /* TODO: decompile (0x1E4 bytes, ~121 instructions) */
}
#pragma peephole reset

/* 0x80219270 | size: 0x44 | small */
void fn_80219270(void) {
    /* TODO: decompile (0x44 bytes) */
}

/* 0x802192B4 | size: 0xA0 | medium */
#pragma peephole off
void fn_802192B4(void) {
    /* TODO: decompile (0xA0 bytes, ~40 instructions) */
}
#pragma peephole reset

/* 0x80219354 | size: 0x24C | large */
#pragma peephole off
void fn_80219354(void) {
    /* TODO: decompile (0x24C bytes, ~147 instructions) */
}
#pragma peephole reset

/* 0x802195A0 | size: 0x108 | medium */
#pragma peephole off
void fn_802195A0(void) {
    /* TODO: decompile (0x108 bytes, ~66 instructions) */
}
#pragma peephole reset

/* 0x802196A8 | size: 0x15C | medium */
#pragma peephole off
void fn_802196A8(void) {
    /* TODO: decompile (0x15C bytes, ~87 instructions) */
}
#pragma peephole reset

/* 0x80219804 | size: 0x34 | small */
void fn_80219804(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x80219838 | size: 0x12C | medium */
#pragma peephole off
void fn_80219838(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80219964 | size: 0x1C8 | medium */
#pragma peephole off
void fn_80219964(void) {
    /* TODO: decompile (0x1C8 bytes, ~114 instructions) */
}
#pragma peephole reset

/* 0x80219B2C | size: 0x1C8 | medium */
#pragma peephole off
void fn_80219B2C(void) {
    /* TODO: decompile (0x1C8 bytes, ~114 instructions) */
}
#pragma peephole reset

/* 0x80219CF4 | size: 0xA4 | medium */
#pragma peephole off
void fn_80219CF4(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x80219D98 | size: 0x78 | small */
void fn_80219D98(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x80219E10 | size: 0x1D4 | medium */
#pragma peephole off
void fn_80219E10(void) {
    /* TODO: decompile (0x1D4 bytes, ~117 instructions) */
}
#pragma peephole reset

/* 0x80219FE4 | size: 0x70 | small */
void fn_80219FE4(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8021A054 | size: 0x26C | large */
#pragma peephole off
void fn_8021A054(void) {
    /* TODO: decompile (0x26C bytes, ~155 instructions) */
}
#pragma peephole reset

/* 0x8021A2C0 | size: 0x78 | small */
void fn_8021A2C0(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x8021A338 | size: 0x140 | medium */
#pragma peephole off
void fn_8021A338(void) {
    /* TODO: decompile (0x140 bytes, ~80 instructions) */
}
#pragma peephole reset

/* 0x8021A478 | size: 0x254 | large */
#pragma peephole off
void fn_8021A478(void) {
    /* TODO: decompile (0x254 bytes, ~149 instructions) */
}
#pragma peephole reset

/* 0x8021A6CC | size: 0x98 | medium */
#pragma peephole off
void fn_8021A6CC(void) {
    /* TODO: decompile (0x98 bytes, ~38 instructions) */
}
#pragma peephole reset

/* 0x8021A764 | size: 0xA8 | medium */
#pragma peephole off
void fn_8021A764(void) {
    /* TODO: decompile (0xA8 bytes, ~42 instructions) */
}
#pragma peephole reset

/* 0x8021A80C | size: 0x6C | small */
void fn_8021A80C(void) {
    /* TODO: decompile (0x6C bytes) */
}

/* 0x8021A878 | size: 0x10C | medium */
#pragma peephole off
void fn_8021A878(void) {
    /* TODO: decompile (0x10C bytes, ~67 instructions) */
}
#pragma peephole reset

/* 0x8021A984 | size: 0x194 | medium */
#pragma peephole off
void fn_8021A984(void) {
    /* TODO: decompile (0x194 bytes, ~101 instructions) */
}
#pragma peephole reset

/* 0x8021AB18 | size: 0x84 | medium */
#pragma peephole off
void fn_8021AB18(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x8021AB9C | size: 0x80 | small */
void fn_8021AB9C(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x8021AC1C | size: 0x390 | large */
#pragma peephole off
void fn_8021AC1C(void) {
    /* TODO: decompile (0x390 bytes, ~228 instructions) */
}
#pragma peephole reset

/* 0x8021AFAC | size: 0x104 | medium */
#pragma peephole off
void fn_8021AFAC(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x8021B0B0 | size: 0xF4 | medium */
#pragma peephole off
void fn_8021B0B0(void) {
    /* TODO: decompile (0xF4 bytes, ~61 instructions) */
}
#pragma peephole reset

/* 0x8021B1A4 | size: 0x1C0 | medium */
#pragma peephole off
void fn_8021B1A4(void) {
    /* TODO: decompile (0x1C0 bytes, ~112 instructions) */
}
#pragma peephole reset

/* 0x8021B364 | size: 0x120 | medium */
#pragma peephole off
void fn_8021B364(void) {
    /* TODO: decompile (0x120 bytes, ~72 instructions) */
}
#pragma peephole reset

/* 0x8021B484 | size: 0x18C | medium */
#pragma peephole off
void fn_8021B484(void) {
    /* TODO: decompile (0x18C bytes, ~99 instructions) */
}
#pragma peephole reset

/* 0x8021B610 | size: 0x18 */
void fn_8021B610(void) {
    u32 pc = lbl_8047B610;
    lbl_8047B626 = 0;
    lbl_8047B610 = pc + 1;
}

/* 0x8021B628 | size: 0xE4 | medium */
#pragma peephole off
void fn_8021B628(void) {
    /* TODO: decompile (0xE4 bytes, ~57 instructions) */
}
#pragma peephole reset

/* 0x8021B70C | size: 0x54 | small */
void fn_8021B70C(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x8021B760 | size: 0xD0 | medium */
#pragma peephole off
void fn_8021B760(void) {
    /* TODO: decompile (0xD0 bytes, ~52 instructions) */
}
#pragma peephole reset

/* 0x8021B830 | size: 0x40 | small */
void fn_8021B830(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8021B870 | size: 0x48 | small */
void fn_8021B870(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x8021B8B8 | size: 0x58 | small */
void fn_8021B8B8(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x8021B910 | size: 0x724 | large */
#pragma peephole off
void fn_8021B910(void) {
    /* TODO: decompile (0x724 bytes, ~457 instructions) */
}
#pragma peephole reset

/* 0x8021C034 | size: 0x5C | small */
void fn_8021C034(void) {
    /* TODO: decompile (0x5C bytes) */
}

/* 0x8021C090 | size: 0x64 | small */
void fn_8021C090(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x8021C0F4 | size: 0x9C | medium */
#pragma peephole off
void fn_8021C0F4(void) {
    /* TODO: decompile (0x9C bytes, ~39 instructions) */
}
#pragma peephole reset

/* 0x8021C190 | size: 0x178 | medium */
#pragma peephole off
void fn_8021C190(void) {
    /* TODO: decompile (0x178 bytes, ~94 instructions) */
}
#pragma peephole reset

/* 0x8021C308 | size: 0x188 | medium */
#pragma peephole off
void fn_8021C308(void) {
    /* TODO: decompile (0x188 bytes, ~98 instructions) */
}
#pragma peephole reset

/* 0x8021C490 | size: 0xF8 | medium */
#pragma peephole off
void fn_8021C490(void) {
    /* TODO: decompile (0xF8 bytes, ~62 instructions) */
}
#pragma peephole reset

/* 0x8021C588 | size: 0xB0 | medium */
#pragma peephole off
void fn_8021C588(void) {
    /* TODO: decompile (0xB0 bytes, ~44 instructions) */
}
#pragma peephole reset

/* 0x8021C638 | size: 0xBC | medium */
#pragma peephole off
void fn_8021C638(void) {
    /* TODO: decompile (0xBC bytes, ~47 instructions) */
}
#pragma peephole reset

/* 0x8021C6F4 | size: 0x10 | tiny */
void fn_8021C6F4(void) { }

/* 0x8021C704 | size: 0x58 | small */
void fn_8021C704(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x8021C75C | size: 0x1A4 | medium */
#pragma peephole off
void fn_8021C75C(void) {
    /* TODO: decompile (0x1A4 bytes, ~105 instructions) */
}
#pragma peephole reset

/* 0x8021C900 | size: 0x100 | medium */
#pragma peephole off
void fn_8021C900(void) {
    /* TODO: decompile (0x100 bytes, ~64 instructions) */
}
#pragma peephole reset

/* 0x8021CA00 | size: 0x158 | medium */
#pragma peephole off
void fn_8021CA00(void) {
    /* TODO: decompile (0x158 bytes, ~86 instructions) */
}
#pragma peephole reset

/* 0x8021CB58 | size: 0x104 | medium */
#pragma peephole off
void fn_8021CB58(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x8021CC5C | size: 0x84 | medium */
#pragma peephole off
void fn_8021CC5C(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x8021CCE0 | size: 0x180 | medium */
#pragma peephole off
void fn_8021CCE0(void) {
    /* TODO: decompile (0x180 bytes, ~96 instructions) */
}
#pragma peephole reset

/* 0x8021CE60 | size: 0xDC | medium */
#pragma peephole off
void fn_8021CE60(void) {
    /* TODO: decompile (0xDC bytes, ~55 instructions) */
}
#pragma peephole reset

/* 0x8021CF3C | size: 0xD4 | medium */
#pragma peephole off
void fn_8021CF3C(void) {
    /* TODO: decompile (0xD4 bytes, ~53 instructions) */
}
#pragma peephole reset

/* 0x8021D010 | size: 0x80 | small */
void fn_8021D010(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x8021D090 | size: 0x194 | medium */
#pragma peephole off
void fn_8021D090(void) {
    /* TODO: decompile (0x194 bytes, ~101 instructions) */
}
#pragma peephole reset

/* 0x8021D224 | size: 0x1E8 | medium */
#pragma peephole off
void fn_8021D224(void) {
    /* TODO: decompile (0x1E8 bytes, ~122 instructions) */
}
#pragma peephole reset

/* 0x8021D40C | size: 0x27C | large */
#pragma peephole off
void fn_8021D40C(void) {
    /* TODO: decompile (0x27C bytes, ~159 instructions) */
}
#pragma peephole reset

/* 0x8021D688 | size: 0x338 | large */
#pragma peephole off
void fn_8021D688(void) {
    /* TODO: decompile (0x338 bytes, ~206 instructions) */
}
#pragma peephole reset

/* 0x8021D9C0 | size: 0x1B8 | medium */
#pragma peephole off
void fn_8021D9C0(void) {
    /* TODO: decompile (0x1B8 bytes, ~110 instructions) */
}
#pragma peephole reset

/* 0x8021DB78 | size: 0x1AC | medium */
#pragma peephole off
void fn_8021DB78(void) {
    /* TODO: decompile (0x1AC bytes, ~107 instructions) */
}
#pragma peephole reset

/* 0x8021DD24 | size: 0x10 | tiny */
void fn_8021DD24(void) { }

/* 0x8021DD34 | size: 0x10 | tiny */
void fn_8021DD34(void) { }

/* 0x8021DD44 | size: 0x74 | small */
void fn_8021DD44(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x8021DDB8 | size: 0x10 | tiny */
void fn_8021DDB8(void) { }

/* 0x8021DDC8 | size: 0x10 | tiny */
void fn_8021DDC8(void) { }

/* 0x8021DDD8 | size: 0x64 | small */
void fn_8021DDD8(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x8021DE3C | size: 0x10 | tiny */
void fn_8021DE3C(void) { }

/* 0x8021DE4C | size: 0x7C | small */
void fn_8021DE4C(void) {
    /* TODO: decompile (0x7C bytes) */
}

/* 0x8021DEC8 | size: 0x34 | small */
void fn_8021DEC8(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x8021DEFC | size: 0x40 | small */
void fn_8021DEFC(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8021DF3C | size: 0x34 | small */
void fn_8021DF3C(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x8021DF70 | size: 0x10 | tiny */
void fn_8021DF70(void) { }

/* 0x8021DF80 | size: 0xCC | medium */
#pragma peephole off
void fn_8021DF80(void) {
    /* TODO: decompile (0xCC bytes, ~51 instructions) */
}
#pragma peephole reset

/* 0x8021E04C | size: 0x23C | large */
#pragma peephole off
void fn_8021E04C(void) {
    /* TODO: decompile (0x23C bytes, ~143 instructions) */
}
#pragma peephole reset

/* 0x8021E288 | size: 0x378 | large */
#pragma peephole off
void fn_8021E288(void) {
    /* TODO: decompile (0x378 bytes, ~222 instructions) */
}
#pragma peephole reset

/* 0x8021E600 | size: 0xCC | medium */
#pragma peephole off
void fn_8021E600(void) {
    /* TODO: decompile (0xCC bytes, ~51 instructions) */
}
#pragma peephole reset

/* 0x8021E6CC | size: 0x10 | tiny */
void fn_8021E6CC(void) { }

/* 0x8021E6DC | size: 0x10 | tiny */
void fn_8021E6DC(void) { }

/* 0x8021E6EC | size: 0x10 | tiny */
void fn_8021E6EC(void) { }

/* 0x8021E6FC | size: 0x48 | small */
void fn_8021E6FC(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x8021E744 | size: 0x10 | tiny */
void fn_8021E744(void) { }

/* 0x8021E754 | size: 0x2A0 | large */
#pragma peephole off
void fn_8021E754(void) {
    /* TODO: decompile (0x2A0 bytes, ~168 instructions) */
}
#pragma peephole reset

/* 0x8021E9F4 | size: 0xA0 | medium */
#pragma peephole off
void fn_8021E9F4(void) {
    /* TODO: decompile (0xA0 bytes, ~40 instructions) */
}
#pragma peephole reset

/* 0x8021EA94 | size: 0x54 | small */
void fn_8021EA94(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x8021EAE8 | size: 0x118 | medium */
#pragma peephole off
void fn_8021EAE8(void) {
    /* TODO: decompile (0x118 bytes, ~70 instructions) */
}
#pragma peephole reset

/* 0x8021EC00 | size: 0xF8 | medium */
#pragma peephole off
void fn_8021EC00(void) {
    /* TODO: decompile (0xF8 bytes, ~62 instructions) */
}
#pragma peephole reset

/* 0x8021ECF8 | size: 0x78 | small */
void fn_8021ECF8(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x8021ED70 | size: 0xC8 | medium */
#pragma peephole off
void fn_8021ED70(void) {
    /* TODO: decompile (0xC8 bytes, ~50 instructions) */
}
#pragma peephole reset

/* 0x8021EE38 | size: 0x10 | tiny */
void fn_8021EE38(void) { }

/* 0x8021EE48 | size: 0x50 | small */
void fn_8021EE48(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x8021EE98 | size: 0x3C | small */
void fn_8021EE98(void) {
    /* TODO: decompile (0x3C bytes) */
}

/* 0x8021EED4 | size: 0x40 | small */
void fn_8021EED4(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8021EF14 | size: 0x10 | tiny */
void fn_8021EF14(void) { }

/* 0x8021EF24 | size: 0x2A8 | large */
#pragma peephole off
void fn_8021EF24(void) {
    /* TODO: decompile (0x2A8 bytes, ~170 instructions) */
}
#pragma peephole reset

/* 0x8021F1CC | size: 0x80 | small */
void fn_8021F1CC(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x8021F24C | size: 0x150 | medium */
#pragma peephole off
void fn_8021F24C(void) {
    /* TODO: decompile (0x150 bytes, ~84 instructions) */
}
#pragma peephole reset

/* 0x8021F39C | size: 0xBC | medium */
#pragma peephole off
void fn_8021F39C(void) {
    /* TODO: decompile (0xBC bytes, ~47 instructions) */
}
#pragma peephole reset

/* 0x8021F458 | size: 0x20C | large */
#pragma peephole off
void fn_8021F458(void) {
    /* TODO: decompile (0x20C bytes, ~131 instructions) */
}
#pragma peephole reset

/* 0x8021F664 | size: 0x2C8 | large */
#pragma peephole off
void fn_8021F664(void) {
    /* TODO: decompile (0x2C8 bytes, ~178 instructions) */
}
#pragma peephole reset

/* 0x8021F92C | size: 0x6C | small */
void fn_8021F92C(void) {
    /* TODO: decompile (0x6C bytes) */
}

/* 0x8021F998 | size: 0x13C | medium */
#pragma peephole off
void fn_8021F998(void) {
    /* TODO: decompile (0x13C bytes, ~79 instructions) */
}
#pragma peephole reset

/* 0x8021FAD4 | size: 0xCA4 | massive */
#pragma peephole off
void fn_8021FAD4(void) {
    /* TODO: decompile (0xCA4 bytes, ~809 instructions) */
}
#pragma peephole reset


#pragma pop
