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

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 256 functions matched
 * =================================================================== */

/* Address: 0x801FBD10 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FBD10(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x801FBD28 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FBD28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801FBD40 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FBD40(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x801FBD84 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBD84(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x12]) = val;
}

/* Address: 0x801FBD94 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBD94(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x11]) = val;
}

/* Address: 0x801FBDA4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBDA4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x10]) = val;
}

/* Address: 0x801FBDB4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBDB4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xF]) = val;
}

/* Address: 0x801FBDC4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBDC4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xE]) = val;
}

/* Address: 0x801FBDD4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBDD4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}

/* Address: 0x801FBDE4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBDE4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA]) = val;
}

/* Address: 0x801FBE18 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBE18(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x801FBE28 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FBE28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x12]);
}

/* Address: 0x801FBE40 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FBE40(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}

/* Address: 0x801FBE58 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FBE58(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}

/* Address: 0x801FBE70 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FBE70(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}

/* Address: 0x801FBE88 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FBE88(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}

/* Address: 0x801FBEA0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FBEA0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}

/* Address: 0x801FBEB8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FBEB8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA]);
}

/* Address: 0x801FBF04 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_801FBF04(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x0]);
}

/* Address: 0x801FBF1C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBF1C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x801FBF2C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBF2C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x801FBF3C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBF3C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* Address: 0x801FBF4C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBF4C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x0]) = val;
}

/* Address: 0x801FBF5C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FBF5C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x801FBF74 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FBF74(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801FBF8C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FBF8C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x801FBFA4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FBFA4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x0]);
}

/* Address: 0x801FBFE8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBFE8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x19]) = val;
}

/* Address: 0x801FBFF8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FBFF8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x18]) = val;
}

/* Address: 0x801FC008 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC008(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x17]) = val;
}

/* Address: 0x801FC018 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC018(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x16]) = val;
}

/* Address: 0x801FC028 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC028(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x15]) = val;
}

/* Address: 0x801FC0C0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC0C0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x14]) = val;
}

/* Address: 0x801FC0D0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC0D0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x13]) = val;
}

/* Address: 0x801FC0E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC0E0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x12]) = val;
}

/* Address: 0x801FC0F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC0F0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x11]) = val;
}

/* Address: 0x801FC100 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC100(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x10]) = val;
}

/* Address: 0x801FC110 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC110(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xF]) = val;
}

/* Address: 0x801FC120 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC120(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xE]) = val;
}

/* Address: 0x801FC130 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC130(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xD]) = val;
}

/* Address: 0x801FC140 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC140(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xC]) = val;
}

/* Address: 0x801FC1D8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC1D8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xB]) = val;
}

/* Address: 0x801FC1E8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC1E8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA]) = val;
}

/* Address: 0x801FC1F8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC1F8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x9]) = val;
}

/* Address: 0x801FC208 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC208(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x8]) = val;
}

/* Address: 0x801FC218 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC218(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x7]) = val;
}

/* Address: 0x801FC228 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC228(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x6]) = val;
}

/* Address: 0x801FC238 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC238(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x5]) = val;
}

/* Address: 0x801FC248 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC248(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}

/* Address: 0x801FC258 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC258(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x3]) = val;
}

/* Address: 0x801FC268 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC268(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x2]) = val;
}

/* Address: 0x801FC278 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC278(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}

/* Address: 0x801FC288 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC288(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x801FC298 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC298(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x19]);
}

/* Address: 0x801FC2B0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC2B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x18]);
}

/* Address: 0x801FC2C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC2C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x17]);
}

/* Address: 0x801FC2E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC2E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x16]);
}

/* Address: 0x801FC2F8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC2F8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x15]);
}

/* Address: 0x801FC3B8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC3B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x14]);
}

/* Address: 0x801FC3D0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC3D0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x13]);
}

/* Address: 0x801FC3E8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC3E8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x12]);
}

/* Address: 0x801FC400 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC400(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}

/* Address: 0x801FC418 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC418(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}

/* Address: 0x801FC430 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC430(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}

/* Address: 0x801FC448 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC448(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}

/* Address: 0x801FC460 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC460(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}

/* Address: 0x801FC478 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC478(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}

/* Address: 0x801FC538 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}

/* Address: 0x801FC550 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC550(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA]);
}

/* Address: 0x801FC568 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC568(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}

/* Address: 0x801FC580 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC580(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}

/* Address: 0x801FC598 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC598(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}

/* Address: 0x801FC5B0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC5B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}

/* Address: 0x801FC5C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC5C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}

/* Address: 0x801FC5E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC5E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}

/* Address: 0x801FC5F8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC5F8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}

/* Address: 0x801FC610 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC610(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}

/* Address: 0x801FC628 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC628(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* Address: 0x801FC640 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC640(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x801FC684 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC684(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x6]) = val;
}

/* Address: 0x801FC694 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC694(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x5]) = val;
}

/* Address: 0x801FC6A4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC6A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}

/* Address: 0x801FC6BC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC6BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}

/* Address: 0x801FC6D4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC6D4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x2]) = val;
}

/* Address: 0x801FC6E4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC6E4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}

/* Address: 0x801FC6F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC6F4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x8]) = val;
}

/* Address: 0x801FC784 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC784(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x10]) = val;
}

/* Address: 0x801FC794 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC794(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA]) = val;
}

/* Address: 0x801FC7A4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC7A4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x801FC7B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC7B4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x3]) = val;
}

/* Address: 0x801FC7C4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC7C4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}

/* Address: 0x801FC7D4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC7D4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}

/* Address: 0x801FC828 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FC828(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}

/* Address: 0x801FC930 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FC930(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA]);
}

/* Address: 0x801FC964 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC964(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}

/* Address: 0x801FC97C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FC97C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}

/* Address: 0x801FC994 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FC994(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}

/* Address: 0x801FCA14 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FCA14(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}

/* Address: 0x801FCA78 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCA78(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x0]) = val;
}

/* Address: 0x801FCAB8 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FCAB8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x0]);
}

/* Address: 0x801FCAFC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCAFC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x10]) = val;
}

/* Address: 0x801FCB30 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCB30(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* Address: 0x801FCB64 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCB64(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x4]) = val;
}

/* Address: 0x801FCB74 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCB74(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x801FCB84 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCB84(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}

/* Address: 0x801FCB94 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCB94(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x801FCBA4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FCBA4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x801FCBF0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FCBF0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x801FCC3C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FCC3C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x801FCC54 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCC54(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x801FCC64 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FCC64(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x801FCC7C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FCC7C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x801FCD08 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCD08(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x27BC]) = val;
}

/* Address: 0x801FCD18 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCD18(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x27B5]) = val;
}

/* Address: 0x801FCD28 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCD28(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x27B8]) = val;
}

/* Address: 0x801FCD38 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCD38(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x27B4]) = val;
}

/* Address: 0x801FCD48 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCD48(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x801FCD8C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCD8C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x27C0]) = val;
}

/* Address: 0x801FCD9C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FCD9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x27C0]);
}

/* Address: 0x801FCDB4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FCDB4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x27BC]);
}

/* Address: 0x801FCDCC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FCDCC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x27B5]);
}

/* Address: 0x801FCDE4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FCDE4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x27B8]);
}

/* Address: 0x801FCDFC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FCDFC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x27B4]);
}

/* Address: 0x801FCE94 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FCE94(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x801FCEFC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCEFC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x678]) = val;
}

/* Address: 0x801FCF0C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF0C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x676]) = val;
}

/* Address: 0x801FCF1C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF1C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x674]) = val;
}

/* Address: 0x801FCF2C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF2C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x672]) = val;
}

/* Address: 0x801FCF3C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF3C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x670]) = val;
}

/* Address: 0x801FCF4C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF4C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x66E]) = val;
}

/* Address: 0x801FCF5C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF5C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x66C]) = val;
}

/* Address: 0x801FCF6C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF6C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x66A]) = val;
}

/* Address: 0x801FCF7C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FCF7C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x668]) = val;
}

/* Address: 0x801FCFA4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FCFA4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x678]);
}

/* Address: 0x801FCFBC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FCFBC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x676]);
}

/* Address: 0x801FCFD4 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_801FCFD4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x674]);
}

/* Address: 0x801FD004 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD004(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x670]);
}

/* Address: 0x801FD01C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD01C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x66E]);
}

/* Address: 0x801FD034 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD034(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x66C]);
}

/* Address: 0x801FD04C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD04C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x66A]);
}

/* Address: 0x801FD064 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD064(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x668]);
}

/* Address: 0x801FD07C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD07C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x8]) = val;
}

/* Address: 0x801FD08C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD08C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}

/* Address: 0x801FD09C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD09C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x4]) = val;
}

/* Address: 0x801FD0AC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD0AC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x0]) = val;
}

/* Address: 0x801FD0BC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD0BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8]);
}

/* Address: 0x801FD0D4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD0D4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}

/* Address: 0x801FD0EC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD0EC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x801FD104 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FD104(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x0]);
}

/* Address: 0x801FD150 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD150(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6AE]) = val;
}

/* Address: 0x801FD178 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD178(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x6AC]) = val;
}

/* Address: 0x801FD188 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD188(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6AC]);
}

/* Address: 0x801FD1A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD1A0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6AA]) = val;
}

/* Address: 0x801FD1B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD1B0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6A8]) = val;
}

/* Address: 0x801FD1C0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD1C0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6A6]) = val;
}

/* Address: 0x801FD1D0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD1D0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6A4]) = val;
}

/* Address: 0x801FD1E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD1E0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x6A0]) = val;
}

/* Address: 0x801FD1F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD1F0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x69E]) = val;
}

/* Address: 0x801FD200 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD200(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x69D]) = val;
}

/* Address: 0x801FD210 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD210(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x69C]) = val;
}

/* Address: 0x801FD220 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD220(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x69B]) = val;
}

/* Address: 0x801FD230 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD230(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x69A]) = val;
}

/* Address: 0x801FD240 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD240(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x699]) = val;
}

/* Address: 0x801FD250 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD250(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x698]) = val;
}

/* Address: 0x801FD260 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD260(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x697]) = val;
}

/* Address: 0x801FD270 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD270(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x696]) = val;
}

/* Address: 0x801FD280 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD280(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x695]) = val;
}

/* Address: 0x801FD290 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD290(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x694]) = val;
}

/* Address: 0x801FD2A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD2A0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x693]) = val;
}

/* Address: 0x801FD2B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD2B0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x692]) = val;
}

/* Address: 0x801FD2C0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD2C0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x691]) = val;
}

/* Address: 0x801FD2D0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD2D0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x690]) = val;
}

/* Address: 0x801FD2E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD2E0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x68F]) = val;
}

/* Address: 0x801FD2F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD2F0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x68E]) = val;
}

/* Address: 0x801FD300 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD300(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x68D]) = val;
}

/* Address: 0x801FD310 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD310(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x68C]) = val;
}

/* Address: 0x801FD320 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD320(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x68B]) = val;
}

/* Address: 0x801FD330 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD330(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x68A]) = val;
}

/* Address: 0x801FD340 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD340(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6AA]);
}

/* Address: 0x801FD358 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_801FD358(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x6A8]);
}

/* Address: 0x801FD370 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD370(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6A6]);
}

/* Address: 0x801FD388 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_801FD388(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x6A4]);
}

/* Address: 0x801FD3A0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FD3A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x6A0]);
}

/* Address: 0x801FD3B8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD3B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x69E]);
}

/* Address: 0x801FD3D0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD3D0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x69D]);
}

/* Address: 0x801FD3E8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD3E8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x69C]);
}

/* Address: 0x801FD400 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD400(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x69B]);
}

/* Address: 0x801FD418 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD418(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x69A]);
}

/* Address: 0x801FD430 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD430(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x699]);
}

/* Address: 0x801FD448 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD448(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x698]);
}

/* Address: 0x801FD460 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD460(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x697]);
}

/* Address: 0x801FD478 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD478(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x696]);
}

/* Address: 0x801FD490 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD490(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x695]);
}

/* Address: 0x801FD4A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD4A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x694]);
}

/* Address: 0x801FD4C0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD4C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x693]);
}

/* Address: 0x801FD4D8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD4D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x692]);
}

/* Address: 0x801FD4F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD4F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x691]);
}

/* Address: 0x801FD508 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD508(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x690]);
}

/* Address: 0x801FD520 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD520(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x68F]);
}

/* Address: 0x801FD538 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x68E]);
}

/* Address: 0x801FD550 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD550(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x68D]);
}

/* Address: 0x801FD568 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD568(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x68C]);
}

/* Address: 0x801FD580 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD580(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x68B]);
}

/* Address: 0x801FD598 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD598(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x68A]);
}

/* Address: 0x801FD5C8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD5C8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x660]) = val;
}

/* Address: 0x801FD5D8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD5D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x660]);
}

/* Address: 0x801FD6B8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD6B8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x688]) = val;
}

/* Address: 0x801FD6C8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD6C8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x686]) = val;
}

/* Address: 0x801FD6D8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD6D8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x684]) = val;
}

/* Address: 0x801FD6E8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD6E8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x682]) = val;
}

/* Address: 0x801FD6F8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD6F8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x67E]) = val;
}

/* Address: 0x801FD708 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD708(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x680]) = val;
}

/* Address: 0x801FD718 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD718(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x67C]) = val;
}

/* Address: 0x801FD728 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD728(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x67A]) = val;
}

/* Address: 0x801FD738 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD738(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x688]);
}

/* Address: 0x801FD750 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_801FD750(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x686]);
}

/* Address: 0x801FD768 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD768(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x684]);
}

/* Address: 0x801FD780 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_801FD780(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x682]);
}

/* Address: 0x801FD798 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD798(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x67E]);
}

/* Address: 0x801FD7B0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD7B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x680]);
}

/* Address: 0x801FD7C8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD7C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x67C]);
}

/* Address: 0x801FD7E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD7E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x67A]);
}

/* Address: 0x801FD7F8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD7F8(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x600]) = val;
}

/* Address: 0x801FD808 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FD808(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x600]);
}

/* Address: 0x801FD820 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD820(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x610]) = val;
}

/* Address: 0x801FD830 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD830(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x60C]) = val;
}

/* Address: 0x801FD840 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD840(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x60A]) = val;
}

/* Address: 0x801FD850 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD850(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x609]) = val;
}

/* Address: 0x801FD860 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD860(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x608]) = val;
}

/* Address: 0x801FD870 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD870(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x607]) = val;
}

/* Address: 0x801FD880 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD880(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x606]) = val;
}

/* Address: 0x801FD890 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD890(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x605]) = val;
}

/* Address: 0x801FD8A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD8A0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x604]) = val;
}

/* Address: 0x801FD8B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD8B0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x801FD8C0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD8C0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x0]) = val;
}

/* Address: 0x801FD8D0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD8D0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x151]) = val;
}

/* Address: 0x801FD8E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD8E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x151]);
}

/* Address: 0x801FD8F8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD8F8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x150]) = val;
}

/* Address: 0x801FD908 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD908(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x14F]) = val;
}

/* Address: 0x801FD918 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD918(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x14E]) = val;
}

/* Address: 0x801FD928 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD928(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x14C]) = val;
}

/* Address: 0x801FD938 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801FD938(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x0]) = val;
}

/* Address: 0x801FD948 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FD948(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x610]);
}

/* Address: 0x801FD960 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD960(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x60A]);
}

/* Address: 0x801FD978 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD978(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x609]);
}

/* Address: 0x801FD990 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD990(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x608]);
}

/* Address: 0x801FD9A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD9A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x607]);
}

/* Address: 0x801FD9C0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD9C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x606]);
}

/* Address: 0x801FD9D8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD9D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x605]);
}

/* Address: 0x801FD9F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FD9F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x604]);
}

/* Address: 0x801FDA84 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FDA84(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801FDA9C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FDA9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x0]);
}

/* Address: 0x801FDAB4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FDAB4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x150]);
}

/* Address: 0x801FDACC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FDACC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x14F]);
}

/* Address: 0x801FDAE4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FDAE4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x14E]);
}

/* Address: 0x801FDB60 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801FDB60(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x0]);
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */

/* Address: 0x801FCC94 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801FCC94(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}

/* Address: 0x801FCCAC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801FCCAC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}
