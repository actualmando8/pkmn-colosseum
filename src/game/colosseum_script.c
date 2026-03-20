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
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222438 | Size: 0x5C | Pattern: field_accessor */
#pragma peephole off
u32 fn_80222438(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x80222494 | Size: 0x3C | Pattern: simple_wrapper */
u32 fn_80222494(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802224D0 | Size: 0xC | Pattern: simple_setter */
void fn_802224D0(u32 value) { /* stub */ }

/* Address: 0x802224DC | Size: 0xC | Pattern: simple_setter */
void fn_802224DC(u32 value) { /* stub */ }

/* Address: 0x802224E8 | Size: 0xC | Pattern: simple_setter */
void fn_802224E8(u32 value) { /* stub */ }

/* Address: 0x802224F4 | Size: 0xC | Pattern: simple_setter */
void fn_802224F4(u32 value) { /* stub */ }

/* Address: 0x80222500 | Size: 0x10 | Pattern: sda_getter */
u32 fn_80222500(void) { return 0; /* stub */ }

/* Address: 0x80222510 | Size: 0x10 | Pattern: sda_getter */
u32 fn_80222510(void) { return 0; /* stub */ }

/* Address: 0x80222520 | Size: 0x34 | Pattern: simple_wrapper */
u32 fn_80222520(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80222554 | Size: 0x30 | Pattern: simple_wrapper */
u32 fn_80222554(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80222584 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_80222584(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802225B0 | Size: 0x2C | Pattern: simple_wrapper */
u32 fn_802225B0(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x802225DC | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802225DC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80222604 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80222604(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8022262C | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_8022262C(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80222654 | Size: 0x50 | Pattern: field_accessor */
#pragma peephole off
u32 fn_80222654(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802226A4 | Size: 0x48 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802226A4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802226EC | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_802226EC(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x80222714 | Size: 0x28 | Pattern: simple_wrapper */
u32 fn_80222714(void* ctx, u32 param) { return 0; /* stub */ }

/* Address: 0x8022273C | Size: 0x20 | Pattern: null_check_getter */
u32 fn_8022273C(void* ctx) { if (!ctx) return 0; return 0; /* stub */ }

/* Address: 0x8022275C | Size: 0x78 | Pattern: field_accessor */
#pragma peephole off
u32 fn_8022275C(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
}
#pragma peephole reset

/* Address: 0x802227D4 | Size: 0x70 | Pattern: field_accessor */
#pragma peephole off
u32 fn_802227D4(void* ctx, u32 slot, u32 param) {
    if (!ctx) return 0;
    return 0; /* TODO: field access */
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
