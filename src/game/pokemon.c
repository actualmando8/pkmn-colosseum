/**
 * @file pokemon.c
 * @brief Pokemon data structure accessors and party management.
 *
 * =========================================================================
 * SUBSYSTEM ANALYSIS
 * =========================================================================
 *
 * Address range: 0x801F000C - 0x801F7F80
 * Total functions: ~170
 * Total code size: ~32KB
 *
 * This file implements the runtime Pokemon data access layer. It is the
 * single most critical data interface in Colosseum -- virtually every
 * game subsystem calls through these functions to read/write Pokemon data.
 *
 * KEY FUNCTIONS (by call frequency):
 *
 *   fn_801F54A4 (PokemonGet)              - 510 calls, 0xD18 bytes
 *     Reads any field from a Pokemon structure. Dispatches through
 *     jumptable_803754AC (93 entries, one per field type). The field ID
 *     is passed as r5 and validated against 0x60 (max). If r5 == 0, it
 *     calls GetCurrentPokemon (fn_801F6B48). For r5 in 1-7, it resolves
 *     the slot via fn_801F6738. For r5 in 8-0x5E, it uses the jumptable.
 *
 *   fn_801F025C (PokemonSlotLookupDefault) - 438 calls, 0x50 bytes
 *     Convenience wrapper: calls PokemonGet(0,0,0x14,0) to get party
 *     count, then calls PokemonSlotLookup(slotType, index, count).
 *
 *   fn_801F4C14 (PokemonSet)              - 223 calls, 0x890 bytes
 *     Writes any field. Similar dispatch to PokemonGet but uses
 *     jumptable_80375330 (94 entries). Takes an extra r7 parameter
 *     as the value to write.
 *
 *   fn_801F02AC (PokemonSlotLookup)       - 89 calls, 0x46C bytes
 *     Resolves a "slot type" (party, battle, PC, etc.) to a concrete
 *     Pokemon pointer. Uses jumptable_803752F8 (13 entries) for slot
 *     types 0x11-0x1D. Each case calls PokemonGet with different field
 *     IDs (0x36, 0x42, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4B, 0x4C).
 *
 * GETTER/SETTER PAIRS (0x801F640C - 0x801F6B48):
 *   These are ~88 tiny (0x08-0x38 byte) functions that directly read/write
 *   fields in the Pokemon structure. They follow two patterns:
 *
 *   Pattern A (global state, 0x08 bytes):
 *     stw/lwz r3, lbl_XXXX@sda21(r0)
 *     blr
 *
 *   Pattern B (struct field, 0x14-0x1C bytes):
 *     cmplwi r3, 0x0
 *     beqlr / bne .next
 *     addis r3, r3, 0x1       ; High 16-bit offset
 *     stw/lwz r4, -0x5bXX(r3) ; Low 16-bit offset -> actual = 0xA4XX
 *     blr
 *
 *   Pattern C (array field, 0x28-0x38 bytes):
 *     cmplwi r3, 0x0
 *     beqlr
 *     cmplwi r4, 0x8           ; Array bounds check
 *     bgelr
 *     addis r3, r3, 0x1
 *     clrlslwi r0, r4, 16, 2  ; index * 4
 *     add r3, r3, r0
 *     lwz r3, -0x5b3c(r3)     ; Array base at 0xA4C4
 *     blr
 *
 * STRUCT FIELD MAP (offsets from base pointer):
 *   The addis+negative-offset pattern encodes fields at offsets 0xA480-0xA4F0:
 *
 *   0xA490 : u32  (fn_801F641C set, fn_801F6430 get) - possibly personality value
 *   0xA48C : u32  (fn_801F644C set, fn_801F6460 get) - possibly OT ID
 *   0xA488 : u32  (fn_801F647C set, fn_801F6490 get) - possibly experience
 *   0xA484 : u32  (fn_801F64AC set, fn_801F64C0 get) - possibly encryption key
 *   0xA4E4 : u16  (fn_801F64DC set, fn_801F64F0 get) - possibly species
 *   0xA4C4 : u32[8] (fn_801F650C set, fn_801F6544 get) - possibly move/PP data
 *   0xA4C0 : u16  (fn_801F65C0 set, fn_801F65D4 get) - possibly held item
 *
 * STAT CALCULATION HELPERS (0x801F6B54 - 0x801F7F80):
 *   Functions that compute derived stats (effective HP, stat stages,
 *   nature modifiers, level-up calculations). These call into the
 *   pokemon_data.c base stat tables and apply formulas.
 *
 * PARTY MANAGEMENT (0x801F0718 - 0x801F4C14):
 *   fn_801F0B00 (0x404 bytes): Large party initialization function
 *   fn_801F3CE8 (0x538 bytes): Party comparison/sorting (calls fn_802050F4)
 *   fn_801F3BB4 (0x134 bytes): Party slot swap
 *   fn_801F2B5C (0x3E0 bytes): Party data copy/clone
 *
 * BSS STATE (SDA21-relative globals):
 *   lbl_8047B5F0 : void*, Pokemon system state pointer (fn_801F640C/14)
 *
 * =========================================================================
 */

#include "game/pokemon.h"

/* =========================================================================
 * External function declarations
 * ========================================================================= */

/* fn_80008184: GSthread_GetCurrentContext or similar - called from fn_801F000C */
extern u32 fn_80008184(void);

/* fn_800F0308: VSync/frame wait - called in frame loop */
extern void fn_800F0308(void);

/* fn_800D3088: Returns frame delta or similar timing value */
extern u32 fn_800D3088(void);

/* fn_801F6738: Resolve party slot to Pokemon pointer by index */
extern struct Pokemon* fn_801F6738(u32 index);

/* fn_801F6B48: Get currently selected/active Pokemon */
extern struct Pokemon* fn_801F6B48(void);

/* fn_801F61BC: Get Pokemon system context pointer */
extern struct Pokemon* fn_801F61BC(void);

/* fn_802050F4: Pokemon nature/friendship comparison helper */
extern s8 fn_802050F4(struct Pokemon* pokemon);

/* =========================================================================
 * fn_801F000C - FrameWaitForDuration
 *
 * Waits for a specified number of frames by calling the frame-wait
 * and frame-delta functions in a loop.
 *
 * @param duration  Number of frame units to wait
 * ========================================================================= */
#pragma peephole off
void FrameWaitForDuration(u32 duration) {
    /* fn_80008184 returns a context/flag; if non-zero, enter wait loop */
    u32 target = fn_80008184();
    if (target != 0) {
        u32 elapsed = 0;
        while (elapsed < target) {
            fn_800F0308();
            elapsed += fn_800D3088();
        }
    }
}

/* =========================================================================
 * fn_801F025C - PokemonSlotLookupDefault
 *
 * Gets the total party count via PokemonGet, then calls PokemonSlotLookup
 * to resolve the given slot type and index.
 *
 * r3 = slot type (POKE_SLOT_PARTY, etc.)
 * r4 = slot index
 * ========================================================================= */
u32 PokemonSlotLookupDefault(u16 slotType, u32 index) {
    u16 count = (u16)PokemonGet(NULL, 0, POKE_FIELD_IV_SPATK, 0); /* field 0x14 = party count */
    return PokemonSlotLookup(slotType, index, count);
}

/* =========================================================================
 * fn_801F02AC - PokemonSlotLookup
 *
 * Large dispatch function (0x46C bytes) that resolves slot types to data.
 *
 * Phase 1: Validates slot type. If type == 1, returns immediately.
 * Phase 2: For types 0x11-0x1D, uses jumptable_803752F8 to dispatch.
 *          Each case calls PokemonGet with the appropriate field ID:
 *            0x11: field 0x36 (party member)
 *            0x12: field 0x42 (active battle Pokemon)
 *            0x13: field 0x44
 *            0x14: field 0x45
 *            ...through 0x1D: field 0x4C
 * Phase 3: For types > 0x1D, performs additional lookups via
 *          fn_8012640C and fn_80125424.
 *
 * [Assembly stub - full decompilation requires jump table analysis]
 * ========================================================================= */
/* TODO: Decompile fn_801F02AC (0x46C bytes, jumptable_803752F8) */

/* =========================================================================
 * fn_801F4C14 - PokemonSet
 *
 * Massive setter dispatch (0x890 bytes). Validates the field ID against
 * 0x60, then determines the target Pokemon:
 *   - If field < 8: resolve via fn_801F6738 (party slot by index)
 *   - If field < 0x5F and pokemon is NULL: use fn_801F6B48 (current)
 * Then dispatches through jumptable_80375330 (94 entries) to call
 * the appropriate setter function.
 *
 * First few jumptable entries call:
 *   Case 0x00: fn_801F65F0 (set species via u16)
 *   Case 0x01: fn_801F667C (set held item via u16)
 *   Case 0x02: fn_801F66EC (set move by slot)
 *   ...
 *
 * [Assembly stub - full decompilation requires jump table analysis]
 * ========================================================================= */
/* TODO: Decompile fn_801F4C14 (0x890 bytes, jumptable_80375330) */

/* =========================================================================
 * fn_801F54A4 - PokemonGet
 *
 * Massive getter dispatch (0xD18 bytes). Same structure as PokemonSet:
 *   - Validates field ID against 0x60
 *   - Resolves Pokemon pointer
 *   - Dispatches through jumptable_803754AC (93 entries)
 *
 * First few jumptable entries call:
 *   Case 0x00: fn_801F6600 (get species, u16)
 *   Case 0x01: fn_801F6720 (get held item, u16, clrlwi r3, r3, 16)
 *   Case 0x02: fn_801F66EC (get move by slot index)
 *   ...
 *
 * [Assembly stub - full decompilation requires jump table analysis]
 * ========================================================================= */
/* TODO: Decompile fn_801F54A4 (0xD18 bytes, jumptable_803754AC) */

/* =========================================================================
 * Getter/Setter pairs for Pokemon structure fields
 *
 * These are the lowest-level accessors. Each pair accesses a specific
 * offset in the Pokemon structure. The "addis r3, r3, 1" plus negative
 * offset pattern means the actual struct offset = 0x10000 + (-0x5bXX) = 0xA4XX.
 * ========================================================================= */

/* --- fn_801F640C / fn_801F6414: Global state pointer (lbl_8047B5F0) --- */
void SetPokemonStatePtr(void* ptr) {
    /* stw r3, lbl_8047B5F0@sda21(r0) */
    /* Direct SDA write - handled by linker */
}

void* GetPokemonStatePtr(void) {
    /* lwz r3, lbl_8047B5F0@sda21(r0) */
    return NULL; /* Placeholder */
}

/* --- fn_801F641C / fn_801F6430: Struct field at offset 0xA490 --- */
/* TODO: Identify this field - possibly personality value or PID */

/* --- fn_801F644C / fn_801F6460: Struct field at offset 0xA48C --- */
/* TODO: Identify this field - possibly original trainer ID */

/* --- fn_801F647C / fn_801F6490: Struct field at offset 0xA488 --- */
/* TODO: Identify this field - possibly experience points */

/* --- fn_801F64AC / fn_801F64C0: Struct field at offset 0xA484 --- */
/* TODO: Identify this field - possibly encryption key */

/* --- fn_801F64DC / fn_801F64F0: Struct field at offset 0xA4E4 (u16) --- */
/* TODO: Identify this field - possibly species ID */

/* --- fn_801F650C / fn_801F6544: Struct array at offset 0xA4C4 (u32[8]) --- */
/* This is likely the move/PP data array. 8 entries x 4 bytes = 32 bytes.
 * With 4 moves, each entry could hold move ID + PP packed as u16+u16. */

/* --- fn_801F65C0 / fn_801F65D4: Struct field at offset 0xA4C0 (u16) --- */
/* TODO: Identify this field - possibly held item */

/* =========================================================================
 * fn_801F3CE8 - PartyCompare
 *
 * Large function (0x538 bytes) that compares two Pokemon for sorting.
 * Calls fn_802050F4 on each to get a comparison value (likely nature
 * or friendship-derived), then swaps entries if needed. Used by the
 * party sorting algorithm in fn_801F3BB4.
 *
 * Parameters: r3=context, r4=pokemonA, r5=pokemonB, r6=sortFlags
 * ========================================================================= */
/* TODO: Decompile fn_801F3CE8 (0x538 bytes) */

/* =========================================================================
 * fn_801F0B00 - PartyInit
 *
 * Large initialization function (0x404 bytes) that sets up the party
 * system. Called from battle_main.c during battle scene setup.
 * Initializes multiple rendering passes via scene callbacks.
 * ========================================================================= */
/* TODO: Decompile fn_801F0B00 (0x404 bytes) */

/* =========================================================================
 * fn_801F2B5C - PartyCopy
 *
 * Deep-copies Pokemon data between party slots (0x3E0 bytes).
 * Iterates through all fields and copies them individually via
 * the PokemonGet/PokemonSet interface.
 * ========================================================================= */
/* TODO: Decompile fn_801F2B5C (0x3E0 bytes) */

/* =========================================================================
 * Stat calculation helpers (0x801F6B54 - 0x801F7F80)
 * ========================================================================= */

/* fn_801F6B54 (0xF8 bytes): Calculate effective stat with nature modifier */
/* fn_801F7090 (0xE4 bytes): Calculate HP stat */
/* fn_801F7174 (0xE4 bytes): Calculate non-HP stat */
/* fn_801F7530 (0xC8 bytes): Apply stat stage modifiers */
/* fn_801F7954 (0x21C bytes): Level-up stat recalculation */
/* fn_801F7C54 (0x20C bytes): Full stat recalculation from base+IV+EV+nature */
