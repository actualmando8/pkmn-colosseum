/**
 * @file trainer.c
 * @brief Trainer data access and party management interface.
 *
 * =========================================================================
 * SUBSYSTEM ANALYSIS
 * =========================================================================
 *
 * Address range: 0x801F7F80 - 0x80201800
 * Total functions: ~220 (including the dense accessor block at 0x801FC000-0x801FD000)
 * Total code size: ~38KB
 *
 * This file implements the trainer data access layer. The core function
 * TrainerDataGet (fn_801FB1C0) is the most frequently called function in
 * this range with 883 call sites, making it one of the most critical
 * interfaces in the entire game.
 *
 * KEY FUNCTIONS:
 *
 *   fn_801FB1C0 (TrainerDataGet) - 883 calls, 0x724 bytes (0x801FB1C0-0x801FB8E4)
 *     Two-phase dispatch:
 *     Phase 1 - Category resolution (r5 = field ID):
 *       0x01-0x09: Battle trainer (fn_801FCCC4)
 *       0x0A-0x0C: Party configuration (fn_801FCAD0)
 *       0x0D-0x1D: Team roster (fn_801FCA2C)
 *       0x1E-0x3C: Story/event data (fn_801FC658)
 *       0x3D-0x41: Misc attributes (fn_801FBFBC)
 *     Phase 2 - Field dispatch via jumptable_803757D8 (86 entries)
 *
 *   fn_801FAA58 (TrainerDataSet) - 169 calls
 *     Mirror of TrainerDataGet for writing values.
 *
 *   fn_80205B8C (GetTrainerPokemonPtr) - 668 calls, 0x58 bytes
 *     Navigates trainer -> party -> Pokemon by calling fn_8012640C twice:
 *       First call:  field=0xD6 (get party list)
 *       Second call: field=0xCC (get Pokemon from party slot)
 *     Returns the Pokemon data pointer, or NULL on any failure.
 *
 *   fn_80205BE8 (GetTrainerPokemonPtrSingle) - related helper
 *     Single-step version: just calls fn_8012640C with field=0xCC.
 *
 * TRAINER CATEGORY SUB-DISPATCHERS:
 *   fn_801FCCC4: Resolves battle trainer context. Likely reads from the
 *                active battle's trainer slot data.
 *   fn_801FCAD0: Resolves party configuration. Returns a sub-struct
 *                pointer used to access party layout info.
 *   fn_801FCA2C: Resolves team roster. Used for fields related to the
 *                full team of 6 Pokemon.
 *   fn_801FC658: Resolves story/event data. This bridges the trainer
 *                system to the story progression flags.
 *   fn_801FBFBC: Resolves miscellaneous trainer attributes (name, class,
 *                AI flags, etc.).
 *
 * DENSE ACCESSOR BLOCK (0x801FC000-0x801FD000, ~285 functions):
 *   This region contains 149+136 tiny functions that are individual field
 *   accessors for the trainer/party structure. They follow the same
 *   patterns as the Pokemon field accessors (null-check, offset load).
 *   The sheer density (285 functions in 8KB) indicates these are
 *   compiler-generated struct member access functions.
 *
 * EVENT INTEGRATION (0x801FE000-0x80200A8C):
 *   fn_801FE7EC (49 calls): Set event/story state on a trainer
 *   fn_801FECD4 (59 calls): Check event/story state on a trainer
 *   fn_801FE710 (5 calls):  Clear event state
 *
 * COMMONLY CALLED EXTERNAL FUNCTIONS:
 *   fn_8012640C (1769 total calls): Master data table resolver.
 *     This is THE core data access primitive for the entire game.
 *     It takes (pointer, slot, tableID, flags) and resolves a pointer
 *     into the common_rel data tables. Table IDs seen:
 *       0xCC = Pokemon in party slot
 *       0xD6 = Party/trainer data
 *       0x79 = Secondary Pokemon data
 *       0xE5 = Extended trainer info
 *
 *   fn_801254B4 (544 total calls): Data table write accessor.
 *     Mirror of fn_8012640C for writing values.
 *
 * BSS STATE:
 *   lbl_8047B610 : u32, trainer system program counter / script position
 *     This is read/written very frequently (lwz/stw with addi +1/+5)
 *     suggesting it's a script or event sequence counter.
 *
 * =========================================================================
 */

#include "game/trainer.h"
#include "game/pokemon.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* fn_8012640C - Master data table resolver
 * The most-called function in the entire game (1769 calls).
 * Takes a context pointer, slot index, table ID, and flags.
 * Returns a pointer to the resolved data, or NULL. */
extern void* fn_8012640C(void* context, u32 slot, u16 tableId, u32 flags);

/* fn_801254B4 - Master data table writer (544 calls) */
extern u32 fn_801254B4(void* context, u32 slot, u16 tableId, u32 flags, u32 value);

/* fn_80125424 - Data table auxiliary writer */
extern void fn_80125424(void* context, u32 value);

/* fn_80142CF4 - Secondary data accessor (169 calls) */
extern u32 fn_80142CF4(u32 context, u32 param, u16 field, u32 flags);

/* Category resolution sub-dispatchers */
extern void* fn_801FCCC4(u32 slot); /* Battle trainer */
extern void* fn_801FCAD0(u32 slot); /* Party config */
extern void* fn_801FCA2C(u32 slot); /* Team roster */
extern void* fn_801FC658(u32 slot); /* Story/event data */
extern void* fn_801FBFBC(u32 slot); /* Misc attributes */

/* Event integration */
extern void fn_801FE7EC(void* trainer, u32 eventId, u32 param1, u32 param2);
extern u8   fn_801FECD4(void* trainer);
extern void fn_801FE710(void* trainer, u32 eventId, u32 param);

/* =========================================================================
 * fn_80205B8C - GetTrainerPokemonPtr
 *
 * Navigate from a trainer/party context to a Pokemon data pointer.
 * This is the third most-called function in the range (668 calls).
 *
 * The function performs two hops through the data table system:
 *   1. context -> party list (table 0xD6)
 *   2. party list -> specific Pokemon (table 0xCC)
 *
 * @param context  Trainer or party context pointer
 * @return         Pokemon data pointer, or NULL if either hop fails
 * ========================================================================= */
void* GetTrainerPokemonPtr(void* context) {
    void* partyList;
    if (context == NULL) {
        return NULL;
    }

    /* First hop: get party list from trainer context */
    partyList = fn_8012640C(context, 0, 0xD6, 0);
    if (partyList == NULL) {
        return NULL;
    }

    /* Second hop: get Pokemon from party list */
    return fn_8012640C(partyList, 0, 0xCC, 0);
}

/* =========================================================================
 * fn_80205BE8 - GetTrainerPokemonPtrSingle
 *
 * Single-hop version of GetTrainerPokemonPtr. Only does the CC lookup.
 *
 * @param context  Party context pointer (already resolved to party level)
 * @return         Pokemon data pointer, or NULL
 * ========================================================================= */
void* GetTrainerPokemonPtrSingle(void* context) {
    if (context == NULL) {
        return NULL;
    }
    return fn_8012640C(context, 0, 0xCC, 0);
}

/* =========================================================================
 * fn_801FB1C0 - TrainerDataGet
 *
 * Core trainer data dispatch. This function is called 883 times and is
 * the primary interface for reading any trainer-related data.
 *
 * The two-phase dispatch:
 *
 * Phase 1 - Determine which sub-object to access:
 *   if (field == 0 || field >= 0x5B) return 0;
 *   if (field < 0x0A) ptr = fn_801FCCC4(slot); // battle trainer
 *   else if (field < 0x0D) ptr = fn_801FCAD0(slot); // party config
 *   else if (field < 0x1E) ptr = fn_801FCA2C(slot); // team roster
 *   else if (field < 0x3D) ptr = fn_801FC658(slot); // story/event
 *   else if (field < 0x42) ptr = fn_801FBFBC(slot); // misc
 *   if (ptr == NULL) return 0;
 *
 * Phase 2 - Dispatch on field ID through jumptable_803757D8 (86 entries):
 *   Each entry calls the appropriate getter. Example cases:
 *     Case 0x00: fn_801FCCAC (get trainer is-valid flag, u8)
 *     Case 0x01: fn_801FCC94 (get trainer name ptr, u16)
 *     Case 0x02-0x09: Various battle trainer properties
 *     ...
 *     Case 0x42-0x56: Team/party aggregate queries
 *
 * [Full decompilation requires analysis of 86 jumptable entries]
 * ========================================================================= */
/* TODO: Decompile fn_801FB1C0 (0x724 bytes, jumptable_803757D8) */

/* =========================================================================
 * fn_801FAA58 - TrainerDataSet
 *
 * Write-side counterpart to TrainerDataGet. Same category dispatch logic,
 * but uses a setter jumptable. 169 call sites.
 *
 * [Assembly stub - structure mirrors TrainerDataGet]
 * ========================================================================= */
/* TODO: Decompile fn_801FAA58 */

/* =========================================================================
 * fn_80236BFC - CheckTrainerPokemonFlag
 *
 * A higher-level helper that chains three TrainerDataGet calls:
 *   1. TrainerDataGet(r3, 0, 0x43, 0) -> Get Pokemon ptr from trainer
 *   2. TrainerDataGet(0, result, 0x02, 0) -> Get species or similar
 *   3. TrainerDataGet(0, result, 0x24, 0) -> Check shadow/special flag
 * If the shadow check returns 1, calls fn_802026E4 for the actual flag test.
 *
 * This pattern (resolve trainer -> resolve pokemon -> check property)
 * is the standard idiom throughout the script/event system.
 *
 * 272 call sites, making this one of the most important utility functions.
 * ========================================================================= */
/* TODO: Decompile fn_80236BFC (0x80 bytes) */

/* =========================================================================
 * Trainer accessor functions (0x801FC000 - 0x801FD000)
 *
 * This region contains 285 tiny functions that are individual field
 * getters/setters for various trainer sub-structures. They are
 * organized in interleaved get/set pairs.
 *
 * Due to the extreme density and repetitive nature, these are best
 * decompiled by identifying the structure layout first, then
 * generating the accessors from the struct definition.
 *
 * Notable sub-structures accessed:
 *   - Battle trainer data (via fn_801FCCC4)
 *   - Party configuration (via fn_801FCAD0)
 *   - Team roster entries (via fn_801FCA2C)
 *   - Story/event flags (via fn_801FC658)
 *   - Misc attributes (via fn_801FBFBC)
 * ========================================================================= */
