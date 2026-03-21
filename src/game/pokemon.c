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
#pragma peephole on

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

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 45 functions matched
 * =================================================================== */

extern u32 lbl_8047B5F0;

/* Forward declarations for converted functions */
u16 fn_801F6720(u8* ptr);
u32 fn_801F6600(u8* ptr);
u32 fn_801F66EC(u8* ptr, u8 idx);
void fn_801F025C(void);
void fn_801F02AC(void);
void fn_801F2B5C(void);
void fn_801F3BB4(void);
void fn_801F3CE8(void);
void fn_801F4C14(void);
void fn_801F65F0(u8* ptr, u32 val);
void fn_801F667C(u8* ptr, u16 val);
void fn_801F6B54(void);
void fn_801F7090(void);
void fn_801F7174(void);
void fn_801F7530(void);
void fn_801F7954(void);
void fn_801F7C54(void);


/* Address: 0x801F0204 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F0204(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801F021C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801F021C(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x801F640C | Size: 0x8 | Pattern: sda_setter */
void fn_801F640C(u32 val) {
    lbl_8047B5F0 = val;
}

/* Address: 0x801F6414 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801F6414(void) {
    return lbl_8047B5F0;
}

/* Address: 0x801F641C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F641C(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA490) = val;
}

/* Address: 0x801F644C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F644C(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA48C) = val;
}

/* Address: 0x801F647C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F647C(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA488) = val;
}

/* Address: 0x801F64AC | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F64AC(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA484) = val;
}

/* Address: 0x801F64DC | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F64DC(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)((u8*)ptr + 0xA4E4) = val;
}

/* Address: 0x801F65C0 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F65C0(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)((u8*)ptr + 0xA4C0) = val;
}

/* Address: 0x801F6618 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6618(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)(&ptr[0x10]) = val;
}

/* Address: 0x801F6628 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6628(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* Address: 0x801F6638 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6638(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x801F6648 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6648(u8* ptr, u8 val) {
    if (!ptr) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x801F668C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F668C(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x801F66A4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F66A4(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x801F66BC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F66BC(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x801F66D4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801F66D4(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x801F6764 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6764(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)((u8*)ptr + 0xA4BE) = val;
}

/* Address: 0x801F6778 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6778(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)((u8*)ptr + 0xA4BC) = val;
}

/* Address: 0x801F678C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F678C(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)((u8*)ptr + 0xA4BA) = val;
}

/* Address: 0x801F67A0 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67A0(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)((u8*)ptr + 0xA4B8) = val;
}

/* Address: 0x801F67B4 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67B4(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA4B0) = val;
}

/* Address: 0x801F67C8 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67C8(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA4AC) = val;
}

/* Address: 0x801F67DC | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67DC(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA4A8) = val;
}

/* Address: 0x801F67F0 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67F0(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA4B4) = val;
}

/* Address: 0x801F6804 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6804(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA4A4) = val;
}

/* Address: 0x801F6818 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6818(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA4A0) = val;
}

/* Address: 0x801F682C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F682C(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA49C) = val;
}

/* Address: 0x801F6840 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6840(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA498) = val;
}

/* Address: 0x801F6854 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6854(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA494) = val;
}

/* Address: 0x801F6868 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6868(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA480) = val;
}

/* Address: 0x801F687C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F687C(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA47C) = val;
}

/* Address: 0x801F6890 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6890(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA478) = val;
}

/* Address: 0x801F68A4 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F68A4(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)((u8*)ptr + 0xA474) = val;
}

/* Address: 0x801F68B8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F68B8(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)(&ptr[0x12]) = val;
}

/* Address: 0x801F68C8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F68C8(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)(&ptr[0x10]) = val;
}

/* Address: 0x801F6AE8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F6AE8(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)(&ptr[0x12]);
}

/* Address: 0x801F6B00 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F6B00(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)(&ptr[0x10]);
}

/* Address: 0x801F77E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F77E0(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x801F7858 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F7858(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x801F789C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F789C(u8* ptr, u8 val) {
    if (!ptr) { return; }
    *(u8*)(&ptr[0x522C]) = val;
}

/* Address: 0x801F78AC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F78AC(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x801F78BC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801F78BC(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u8*)(&ptr[0x522C]);
}

/* Address: 0x801F793C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F793C(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 27 functions matched
 * =================================================================== */

/* Address: 0x801F6430 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6430(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA490);
}

/* Address: 0x801F6460 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6460(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA48C);
}

/* Address: 0x801F6490 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6490(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA488);
}

/* Address: 0x801F64C0 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F64C0(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA484);
}

/* Address: 0x801F64F0 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F64F0(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4E4);
}

/* Address: 0x801F65D4 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F65D4(u8* ptr) {
    if (!ptr) { return 0; }
    return *(s16*)((u8*)ptr + 0xA4C0);
}

/* Address: 0x801F65F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F65F0(u8* ptr, u32 val) {
    if (!ptr) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x801F6600 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F6600(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801F667C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F667C(u8* ptr, u16 val) {
    if (!ptr) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x801F6720 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F6720(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x801F68D8 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F68D8(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4BE);
}

/* Address: 0x801F68F4 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F68F4(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4BC);
}

/* Address: 0x801F6910 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6910(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4BA);
}

/* Address: 0x801F692C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F692C(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4B8);
}

/* Address: 0x801F6948 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6948(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4B0);
}

/* Address: 0x801F6964 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6964(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4AC);
}

/* Address: 0x801F6980 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6980(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4A8);
}

/* Address: 0x801F699C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F699C(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4B4);
}

/* Address: 0x801F69B8 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F69B8(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4A4);
}

/* Address: 0x801F69D4 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F69D4(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4A0);
}

/* Address: 0x801F69F0 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F69F0(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA49C);
}

/* Address: 0x801F6A0C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A0C(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA498);
}

/* Address: 0x801F6A28 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A28(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA494);
}

/* Address: 0x801F6A44 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A44(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA480);
}

/* Address: 0x801F6A60 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A60(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA47C);
}

/* Address: 0x801F6A7C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A7C(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA478);
}

/* Address: 0x801F6A98 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A98(u8* ptr) {
    if (!ptr) { return 0; }
    return *(u32*)((u8*)ptr + 0xA474);
}

/* #######################################################################
 * COVERAGE STUBS: Pokemon data management (0x801F000C - 0x801F7F80)
 * 119 functions remaining for full coverage of pokemon.c TU.
 *
 * Key functions in this range:
 *   fn_801F54A4 (PokemonGet)  - 510 calls, 0xD18 bytes, field dispatch
 *   fn_801F4C14 (PokemonSet)  - 223 calls, 0x890 bytes, field write
 *   fn_801F02AC (PokemonSlotLookup) - 89 calls, 0x46C bytes
 *   fn_801F025C (PokemonSlotLookupDefault) - 438 calls, 0x50 bytes
 *   fn_801F0B00 (PartyInit)   - 0x404 bytes
 *   fn_801F3CE8 (PartyCompare) - 0x538 bytes
 *   fn_801F2B5C (PartyDataCopy) - 0x3E0 bytes
 * ####################################################################### */

#pragma push
#pragma force_active on

/* 0x801F000C | size: 0x4C | small */
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F000C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    ((void(*)(void))fn_80008184)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F0044;
    r30 = 0x0;
    goto L_801F003C;
L_801F0030: ;
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D3088)();
    r30 = r30 + r3;
L_801F003C: ;
    if ((u32)r30 < (u32)r31) goto L_801F0030;
L_801F0044: ;
    /* lmw r30, 0x8(r1) */;
    return;
}

/* 0x801F0058 | size: 0x78 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F0058(void) {
    extern void fn_801F02AC();
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
    r30 = r4;
    r29 = r3;
    r3 = 0x4;
    r5 = r30;
    r4 = 0x0;
    fn_801F02AC();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F0090;
    r3 = 0x0;
    goto L_801F00BC;
L_801F0090: ;
    r4 = r29;
    r5 = r30;
    r3 = 0x2;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_801F00B0;
    r3 = 0x0;
    goto L_801F00BC;
L_801F00B0: ;
    r0 = r31 - r3;
    r0 = __cntlzw(r0);
    r3 = (u32)r0 >> 5;
L_801F00BC: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F00D0 | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F00D0(void) {
    extern void fn_801F02AC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = 0x4;
    r5 = r31;
    r4 = 0x0;
    fn_801F02AC();
    /* mr. r4, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F0108;
    r3 = 0x0;
    goto L_801F0120;
L_801F0108: ;
    r3 = r30;
    r5 = r31;
    fn_801F02AC();
    if ((u32)r3 != (u32)0x0) goto L_801F0120;
    r3 = 0x0;
L_801F0120: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F0134 | size: 0xD0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F0134(void) {
    extern u8 lbl_80375AC8[];
    extern u8 lbl_80478D40[];
    extern void fn_801F02AC();
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
    r29 = r4;
    r28 = r3;
    r3 = 0x4;
    r5 = r29;
    r4 = 0x0;
    fn_801F02AC();
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F016C;
    r3 = 0x0;
    goto L_801F01F0;
L_801F016C: ;
    r31 = 0x0;
    goto L_801F01D8;
L_801F0174: ;
    r0 = r31 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_801F01D4;
    r3 = (u32)lbl_80375AC8;
    /* clrlslwi r4, r31, 16, 3 */;
    r0 = (u32)lbl_80375AC8;
    r3 = r0 + r4;
    if ((u32)r5 < (u32)r4) goto L_801F0198;
    r3 = 0x0;
L_801F0198: ;
    if ((u32)r3 == (u32)0x0) goto L_801F01D4;
    if ((u32)r3 != (u32)0x0) goto L_801F01AC;
    r0 = 0x0;
    goto L_801F01B0;
L_801F01AC: ;
    r0 = *(u8*)((u8*)r3 + 0x1);
L_801F01B0: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_801F01D4;
    r3 = r31;
    r4 = r30;
    r5 = r29;
    fn_801F02AC();
    if ((u32)r3 != (u32)r28) goto L_801F01D4;
    goto L_801F01EC;
L_801F01D4: ;
    r31 = r31 + 0x1;
L_801F01D8: ;
    r4 = *(u32*)lbl_80478D40;
    r5 = r31 & 0xFFFF;
    if ((u32)r5 < (u32)r4) goto L_801F0174;
    r31 = 0x0;
L_801F01EC: ;
    r3 = r31;
L_801F01F0: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F0234 | size: 0x28 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F0234(void) {
    extern u8 lbl_80375AC8[];
    extern u8 lbl_80478D40[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D40;
    r5 = r3 & 0xFFFF;
    r4 = (u32)lbl_80375AC8;
    /* clrlslwi r3, r3, 16, 3 */;
    r0 = (u32)lbl_80375AC8;
    r3 = r0 + r3;
    if ((u32)r5 < (u32)r0) return;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801F025C | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F025C(void) {
    extern void fn_801F02AC();
    extern void fn_801F54A4();
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
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r5 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r31;
    fn_801F02AC();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F02AC | size: 0x46C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F02AC(void) {
    extern void fn_801F0718();
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
    extern void fn_8020E1A4();
    extern void fn_8020E1D4();
    extern void fn_8020E204();
    extern u8 jumptable_803752F8[];
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    void (*ctr_fn)(void) = 0;

    r0 = r3 & 0xFFFF;
    /* stmw r14, 0x48(r1) */;
    r15 = r3;
    r16 = r4;
    r18 = r5;
    if ((s32)r0 != (s32)0) goto L_801F02D8;
    r3 = 0x0;
    goto L_801F0704;
L_801F02D8: ;
    ((void(*)(void))fn_801F61BC)();
    r4 = r15 & 0xFFFF;
    if ((u32)r4 != (u32)0x1) goto L_801F02F0;
    goto L_801F0704;
L_801F02F0: ;
    /* subi r0, r4, 0x11 */;
    if ((u32)r0 > (u32)0xc) goto L_801F0418;
    r4 = (u32)jumptable_803752F8;
    r0 = r0 << 2;
    r4 = (u32)jumptable_803752F8;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x42;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x45;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x46;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x47;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x48;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x49;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x4b;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x4c;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x4d;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x4e;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
    r4 = 0x0;
    r5 = 0x4f;
    r6 = 0x0;
    fn_801F54A4();
    goto L_801F0704;
L_801F0418: ;
    r3 = r18;
    fn_8020E204();
    /* mr. r14, r3 */;
    if ((u32)r0 != (u32)0xc) goto L_801F0430;
    r3 = 0x0;
    goto L_801F0704;
L_801F0430: ;
    if ((u32)r16 == (u32)0x0) goto L_801F0458;
    r17 = r16;
    r3 = r16;
    r4 = r18;
    fn_801F0718();
    /* mr. r16, r3 */;
    if ((u32)r16 != (u32)0x0) goto L_801F0458;
    r3 = 0x0;
    goto L_801F0704;
L_801F0458: ;
    r3 = r14;
    r21 = 0x0;
    r19 = 0x0;
    fn_8020E1D4();
    r18 = r3;
    r3 = r14;
    fn_8020E1A4();
    r14 = r3 & 0xFF;
    r27 = r18 & 0xFF;
    r24 = r1 + 0x20;
    r25 = r1 + 0x10;
    r30 = r15 & 0xFFFF;
    r26 = r1 + 0x8;
    r22 = 0x0;
    goto L_801F06F4;
L_801F0494: ;
    r3 = *(u32*)(sp + 0x40);
    r6 = r22;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    /* clrlslwi r28, r22, 16, 2 */;
    *(u32*)(r26 + r28) = r3;
    r0 = r22 & 0xFFFF;
    if ((u32)r30 != (u32)0x4) goto L_801F04CC;
    if ((u32)r0 != (u32)0x0) goto L_801F04CC;
    r3 = *(u32*)(r26 + r28);
    goto L_801F0704;
L_801F04CC: ;
    if ((u32)r30 != (u32)0x5) goto L_801F04E8;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F04E8;
    r3 = *(u32*)(r26 + r28);
    goto L_801F0704;
L_801F04E8: ;
    r3 = *(u32*)(r26 + r28);
    if ((u32)r3 != (u32)r16) goto L_801F0508;
    r0 = r15 & 0xFFFF;
    r4 = 0x1;
    if ((u32)r0 != (u32)0x2) goto L_801F051C;
    goto L_801F0704;
L_801F0508: ;
    r0 = r15 & 0xFFFF;
    r4 = 0x0;
    if ((u32)r0 != (u32)0x3) goto L_801F051C;
    goto L_801F0704;
L_801F051C: ;
    r31 = r4 & 0xFF;
    r18 = 0x0;
    r23 = 0x0;
    goto L_801F06E4;
L_801F052C: ;
    r3 = *(u32*)(r26 + r28);
    r6 = r23;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    /* clrlslwi r29, r21, 16, 2 */;
    *(u32*)(r25 + r29) = r3;
    if ((u32)r30 != (u32)0xb) goto L_801F0568;
    r0 = r22 & 0xFFFF;
    if ((u32)r30 != (u32)0xb) goto L_801F0568;
    r0 = r23 & 0xFFFF;
    if ((u32)r30 != (u32)0xb) goto L_801F0568;
    r3 = *(u32*)(r25 + r29);
    goto L_801F0704;
L_801F0568: ;
    if ((u32)r31 != (u32)0x1) goto L_801F05C8;
    r0 = r15 & 0xFFFF;
    if ((u32)r0 != (u32)0x6) goto L_801F058C;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)0x6) goto L_801F058C;
    r3 = *(u32*)(r25 + r29);
    goto L_801F0704;
L_801F058C: ;
    r0 = r15 & 0xFFFF;
    if ((u32)r0 != (u32)0x7) goto L_801F05AC;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F05AC;
    r3 = *(u32*)(r25 + r29);
    goto L_801F0704;
L_801F05AC: ;
    r0 = r15 & 0xFFFF;
    if ((u32)r0 != (u32)0x8) goto L_801F0604;
    r3 = *(u32*)(r25 + r29);
    if ((u32)r17 == (u32)r3) goto L_801F0604;
    goto L_801F0704;
L_801F05C8: ;
    r0 = r15 & 0xFFFF;
    if ((u32)r0 != (u32)0x9) goto L_801F05E4;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)0x9) goto L_801F05E4;
    r3 = *(u32*)(r25 + r29);
    goto L_801F0704;
L_801F05E4: ;
    r0 = r15 & 0xFFFF;
    if ((u32)r0 != (u32)0xa) goto L_801F0604;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F0604;
    r3 = *(u32*)(r25 + r29);
    goto L_801F0704;
L_801F0604: ;
    r20 = 0x0;
    goto L_801F06D0;
L_801F060C: ;
    r3 = *(u32*)(r25 + r29);
    r6 = r20;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    /* clrlslwi r0, r19, 16, 2 */;
    *(u32*)(r24 + r0) = r3;
    if ((u32)r31 != (u32)0x1) goto L_801F0688;
    r3 = r15 & 0xFFFF;
    if ((u32)r3 != (u32)0xc) goto L_801F064C;
    r3 = r18 & 0xFFFF;
    if ((u32)r3 != (u32)0xc) goto L_801F064C;
    r3 = *(u32*)(r24 + r0);
    goto L_801F0704;
L_801F064C: ;
    r3 = r15 & 0xFFFF;
    if ((u32)r3 != (u32)0xd) goto L_801F066C;
    r3 = r18 & 0xFFFF;
    if ((u32)r3 != (u32)0x1) goto L_801F066C;
    r3 = *(u32*)(r24 + r0);
    goto L_801F0704;
L_801F066C: ;
    r3 = r15 & 0xFFFF;
    if ((u32)r3 != (u32)0xe) goto L_801F06C4;
    r3 = *(u32*)(r24 + r0);
    if ((u32)r17 == (u32)r3) goto L_801F06C4;
    goto L_801F0704;
L_801F0688: ;
    r3 = r15 & 0xFFFF;
    if ((u32)r3 != (u32)0xf) goto L_801F06A4;
    r3 = r18 & 0xFFFF;
    if ((u32)r3 != (u32)0xf) goto L_801F06A4;
    r3 = *(u32*)(r24 + r0);
    goto L_801F0704;
L_801F06A4: ;
    r3 = r15 & 0xFFFF;
    if ((u32)r3 != (u32)0x10) goto L_801F06C4;
    r3 = r18 & 0xFFFF;
    if ((u32)r3 != (u32)0x1) goto L_801F06C4;
    r3 = *(u32*)(r24 + r0);
    goto L_801F0704;
L_801F06C4: ;
    r19 = r19 + 0x1;
    r18 = r18 + 0x1;
    r20 = r20 + 0x1;
L_801F06D0: ;
    r0 = r20 & 0xFFFF;
    if ((s32)r0 < (s32)r14) goto L_801F060C;
    r21 = r21 + 0x1;
    r23 = r23 + 0x1;
L_801F06E4: ;
    r0 = r23 & 0xFFFF;
    if ((s32)r0 < (s32)r27) goto L_801F052C;
    r22 = r22 + 0x1;
L_801F06F4: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F0494;
    r3 = 0x0;
L_801F0704: ;
    /* lmw r14, 0x48(r1) */;
    return;
}
#pragma pop

/* 0x801F0718 | size: 0x180 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F0718(void) {
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
    extern void fn_8020E1A4();
    extern void fn_8020E1BC();
    extern void fn_8020E1D4();
    extern void fn_8020E204();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r5 = 0x0;
    r6 = 0x0;
    /* stmw r22, 0x8(r1) */;
    r31 = r3;
    r22 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r0 = r3;
    r3 = r22;
    r26 = r0;
    fn_8020E204();
    /* mr. r27, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F0764;
    r3 = 0x0;
    goto L_801F0884;
L_801F0764: ;
    fn_8020E1D4();
    r29 = r3;
    r3 = r27;
    fn_8020E1A4();
    r30 = r3;
    r3 = r27;
    fn_8020E1BC();
    r28 = r3 & 0xFF;
    r30 = r30 & 0xFF;
    r29 = r29 & 0xFF;
    r25 = 0x0;
    goto L_801F0874;
L_801F0794: ;
    r3 = r26;
    r6 = r25;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r23 = r3;
    if ((u32)r23 != (u32)r31) goto L_801F07B8;
    goto L_801F0884;
L_801F07B8: ;
    r24 = 0x0;
    goto L_801F0864;
L_801F07C0: ;
    r3 = r23;
    r6 = r24;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    r22 = r3;
    if ((u32)r22 != (u32)r31) goto L_801F07E8;
    r3 = r23;
    goto L_801F0884;
L_801F07E8: ;
    r27 = 0x0;
    goto L_801F0818;
L_801F07F0: ;
    r3 = r22;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    if ((u32)r3 != (u32)r31) goto L_801F0814;
    r3 = r23;
    goto L_801F0884;
L_801F0814: ;
    r27 = r27 + 0x1;
L_801F0818: ;
    r0 = r27 & 0xFFFF;
    if ((s32)r0 < (s32)r28) goto L_801F07F0;
    r27 = 0x0;
    goto L_801F0854;
L_801F082C: ;
    r3 = r22;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    if ((u32)r3 != (u32)r31) goto L_801F0850;
    r3 = r23;
    goto L_801F0884;
L_801F0850: ;
    r27 = r27 + 0x1;
L_801F0854: ;
    r0 = r27 & 0xFFFF;
    if ((s32)r0 < (s32)r30) goto L_801F082C;
    r24 = r24 + 0x1;
L_801F0864: ;
    r0 = r24 & 0xFFFF;
    if ((s32)r0 < (s32)r29) goto L_801F07C0;
    r25 = r25 + 0x1;
L_801F0874: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F0794;
    r3 = 0x0;
L_801F0884: ;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F0898 | size: 0x90 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F0898(void) {
    extern void fn_8020D82C();
    extern void fn_8020D920();
    extern void fn_8020D950();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F08B8;
    r0 = 0x0;
    goto L_801F08E8;
L_801F08B8: ;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_801F08CC;
    r0 = 0x0;
    goto L_801F08E8;
L_801F08CC: ;
    r3 = r31;
    fn_8020D920();
    if ((u32)r3 != (u32)0x0) goto L_801F08E4;
    r0 = 0x0;
    goto L_801F08E8;
L_801F08E4: ;
    r0 = 0x1;
L_801F08E8: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F08F8;
    r3 = 0x0;
    goto L_801F0914;
L_801F08F8: ;
    r3 = r31;
    fn_8020D920();
    if ((u32)r3 != (u32)0x0) goto L_801F0910;
    r3 = 0x0;
    goto L_801F0914;
L_801F0910: ;
    fn_8020D82C();
L_801F0914: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F0928 | size: 0xA8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F0928(void) {
    extern void fn_8020D7CC();
    extern void fn_8020D7E8();
    extern void fn_8020D82C();
    extern void fn_8020D920();
    extern void fn_8020D950();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F0948;
    r0 = 0x0;
    goto L_801F0978;
L_801F0948: ;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_801F095C;
    r0 = 0x0;
    goto L_801F0978;
L_801F095C: ;
    r3 = r31;
    fn_8020D920();
    if ((u32)r3 != (u32)0x0) goto L_801F0974;
    r0 = 0x0;
    goto L_801F0978;
L_801F0974: ;
    r0 = 0x1;
L_801F0978: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F0988;
    r3 = -0x80;
    goto L_801F09BC;
L_801F0988: ;
    r3 = r31;
    fn_8020D920();
    if ((u32)r3 != (u32)0x0) goto L_801F09A0;
    r3 = -0x80;
    goto L_801F09BC;
L_801F09A0: ;
    fn_8020D82C();
    fn_8020D7E8();
    if ((u32)r3 != (u32)0x0) goto L_801F09B8;
    r3 = -0x80;
    goto L_801F09BC;
L_801F09B8: ;
    fn_8020D7CC();
L_801F09BC: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F0F04 | size: 0x188 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F0F04(void) {
    extern u8 lbl_8046D790[];
    extern u8 lbl_8047B5E8[];
    extern u8 lbl_8047B5EC[];
    extern void fn_8020D78C();
    extern void fn_8020D7B4();
    extern void fn_8020D7E8();
    extern void fn_8020D82C();
    extern void fn_8020D920();
    extern void fn_8020D950();
    extern void fn_8020D968();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r29, 0x14(r1) */;
    r30 = r3;
    r29 = *(u32*)lbl_8047B5E8;
    r0 = *(u32*)lbl_8047B5EC;
    r3 = r29 + 0x1;
    r3 = r3 & 0x1F;
    if ((u32)r3 == (u32)r0) goto L_801F0F7C;
    r4 = r29 * 0x30;
    r3 = (u32)lbl_8046D790;
    r0 = 0x6;
    r3 = (u32)lbl_8046D790;
    r31 = r3 + r4;
    /* subi r4, r30, 0x4 */;
    /* subi r5, r31, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_801F0F50: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_801F0F50;
    r3 = *(u32*)lbl_8047B5E8;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047B5E8 = r0;
    r0 = r0 & 0x1F;
    *(u32*)lbl_8047B5E8 = r0;
    goto L_801F0F84;
L_801F0F7C: ;
    r31 = 0x0;
    goto L_801F0F9C;
L_801F0F84: ;
    r3 = r30;
    r4 = r29;
    fn_8020D78C();
    r3 = r31;
    r4 = r29;
    fn_8020D78C();
L_801F0F9C: ;
    if ((u32)r31 != (u32)0x0) goto L_801F0FAC;
    r3 = 0x2;
    goto L_801F1078;
L_801F0FAC: ;
    if ((u32)r30 != (u32)0x0) goto L_801F0FBC;
    r0 = 0x0;
    goto L_801F0FF0;
L_801F0FBC: ;
    r3 = r30;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((u32)r30 != (u32)0x0) goto L_801F0FD4;
    r0 = 0x0;
    goto L_801F0FF0;
L_801F0FD4: ;
    r3 = r30;
    fn_8020D920();
    if ((u32)r3 != (u32)0x0) goto L_801F0FEC;
    r0 = 0x0;
    goto L_801F0FF0;
L_801F0FEC: ;
    r0 = 0x1;
L_801F0FF0: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F1000;
    r29 = 0x0;
    goto L_801F1038;
L_801F1000: ;
    r3 = r30;
    fn_8020D920();
    fn_8020D82C();
    fn_8020D7E8();
    fn_8020D7B4();
    if ((u32)r3 == (u32)0x0) goto L_801F1034;
    r12 = r3;
    r3 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r29 = r3;
    goto L_801F1038;
L_801F1034: ;
    r29 = 0x1;
L_801F1038: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_801F1068;
    r0 = *(u32*)lbl_8047B5E8;
    r3 = *(u32*)lbl_8047B5EC;
    if ((u32)r0 == (u32)r3) goto L_801F1074;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047B5EC = r0;
    r0 = r0 & 0x1F;
    *(u32*)lbl_8047B5EC = r0;
    goto L_801F1074;
L_801F1068: ;
    r3 = r31;
    r4 = r30;
    fn_8020D968();
L_801F1074: ;
    r3 = r29;
L_801F1078: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F1170 | size: 0x5C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1170(void) {
    extern void fn_8020D920();
    extern void fn_8020D950();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F1190;
    r3 = 0x0;
    goto L_801F11B8;
L_801F1190: ;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_801F11A4;
    r3 = 0x0;
    goto L_801F11B8;
L_801F11A4: ;
    r3 = r31;
    fn_8020D920();
    r3 = -r3;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_801F11B8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F11CC | size: 0x294 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F11CC(void) {
    extern void fn_8020D78C();
    extern void fn_8020D814();
    extern void fn_8020D82C();
    extern void fn_8020D844();
    extern void fn_8020D868();
    extern void fn_8020D878();
    extern void fn_8020D888();
    extern void fn_8020D898();
    extern void fn_8020D8A8();
    extern void fn_8020D8B8();
    extern void fn_8020D8C8();
    extern void fn_8020D920();
    extern void fn_8020D938();
    extern void fn_8020D950();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r23, 0xc(r1) */;
    r30 = r4;
    r29 = r3;
    r26 = r5;
    r24 = r6;
    r31 = r7;
    r23 = r8;
    r4 = 0x0;
    fn_8020D8C8();
    r3 = r29;
    r4 = 0x0;
    fn_8020D8B8();
    r3 = r29;
    r4 = 0x0;
    fn_8020D8A8();
    r25 = 0x0;
    goto L_801F1230;
L_801F121C: ;
    r3 = r29;
    r4 = r25;
    r5 = 0x0;
    fn_8020D844();
    r25 = r25 + 0x1;
L_801F1230: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F121C;
    r3 = r29;
    r4 = 0x0;
    fn_8020D888();
    r3 = r29;
    r4 = 0x0;
    fn_8020D878();
    r3 = r29;
    r4 = 0x0;
    fn_8020D898();
    r3 = r29;
    r4 = 0x0;
    fn_8020D868();
    r3 = r29;
    r4 = -0x1;
    fn_8020D78C();
    r3 = r29;
    r4 = r24;
    fn_8020D8C8();
    r3 = r29;
    r4 = r31;
    fn_8020D8B8();
    r3 = r29;
    r4 = r23;
    fn_8020D8A8();
    r3 = r29;
    r4 = r26;
    fn_8020D898();
    if ((u32)r29 != (u32)0x0) goto L_801F12B8;
    r0 = 0x0;
    goto L_801F12EC;
L_801F12B8: ;
    r3 = r29;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((u32)r29 != (u32)0x0) goto L_801F12D0;
    r0 = 0x0;
    goto L_801F12EC;
L_801F12D0: ;
    r3 = r29;
    fn_8020D920();
    if ((u32)r3 != (u32)0x0) goto L_801F12E8;
    r0 = 0x0;
    goto L_801F12EC;
L_801F12E8: ;
    r0 = 0x1;
L_801F12EC: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F12FC;
    r24 = 0x0;
    goto L_801F1398;
L_801F12FC: ;
    r3 = r29;
    fn_8020D950();
    r27 = r3;
    r3 = r29;
    fn_8020D938();
    r25 = r3;
    r3 = r29;
    fn_8020D920();
    /* mr. r26, r3 */;
    if ((u32)r3 != (u32)0x0) goto L_801F132C;
    r24 = 0x0;
    goto L_801F137C;
L_801F132C: ;
    r23 = 0x0;
    r28 = r27 & 0xFFFF;
L_801F1334: ;
    /* clrlslwi r0, r23, 16, 3 */;
    r24 = r26 + r0;
    r3 = r24;
    fn_8020D82C();
    r0 = r3 & 0xFFFF;
    r27 = r3;
    if ((u32)r3 == (u32)0x0) goto L_801F1378;
    r3 = r24;
    fn_8020D814();
    r0 = r27 & 0xFFFF;
    if ((u32)r28 != (u32)r0) goto L_801F1370;
    if ((s32)r25 != (s32)r3) goto L_801F1370;
    goto L_801F137C;
L_801F1370: ;
    r23 = r23 + 0x1;
    goto L_801F1334;
L_801F1378: ;
    r24 = 0x0;
L_801F137C: ;
    if ((u32)r24 != (u32)0x0) goto L_801F138C;
    r24 = 0x0;
    goto L_801F1398;
L_801F138C: ;
    r3 = r29;
    r4 = r24;
    fn_8020D8A8();
L_801F1398: ;
    if ((u32)r24 != (u32)0x0) goto L_801F1430;
    r3 = r29;
    r4 = 0x0;
    fn_8020D8C8();
    r3 = r29;
    r4 = 0x0;
    fn_8020D8B8();
    r3 = r29;
    r4 = 0x0;
    fn_8020D8A8();
    r25 = 0x0;
    goto L_801F13E0;
L_801F13CC: ;
    r3 = r29;
    r4 = r25;
    r5 = 0x0;
    fn_8020D844();
    r25 = r25 + 0x1;
L_801F13E0: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F13CC;
    r3 = r29;
    r4 = 0x0;
    fn_8020D888();
    r3 = r29;
    r4 = 0x0;
    fn_8020D878();
    r3 = r29;
    r4 = 0x0;
    fn_8020D898();
    r3 = r29;
    r4 = 0x0;
    fn_8020D868();
    r3 = r29;
    r4 = -0x1;
    fn_8020D78C();
    r3 = 0x4;
    goto L_801F144C;
L_801F1430: ;
    r3 = r29;
    r4 = r31;
    fn_8020D878();
    r3 = r29;
    r4 = r30;
    fn_8020D868();
    r3 = 0x1;
L_801F144C: ;
    /* lmw r23, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x801F1460 | size: 0xAC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1460(void) {
    extern void fn_8020D78C();
    extern void fn_8020D844();
    extern void fn_8020D868();
    extern void fn_8020D878();
    extern void fn_8020D888();
    extern void fn_8020D898();
    extern void fn_8020D8A8();
    extern void fn_8020D8B8();
    extern void fn_8020D8C8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    fn_8020D8C8();
    r3 = r30;
    r4 = 0x0;
    fn_8020D8B8();
    r3 = r30;
    r4 = 0x0;
    fn_8020D8A8();
    r31 = 0x0;
    goto L_801F14B0;
L_801F149C: ;
    r3 = r30;
    r4 = r31;
    r5 = 0x0;
    fn_8020D844();
    r31 = r31 + 0x1;
L_801F14B0: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F149C;
    r3 = r30;
    r4 = 0x0;
    fn_8020D888();
    r3 = r30;
    r4 = 0x0;
    fn_8020D878();
    r3 = r30;
    r4 = 0x0;
    fn_8020D898();
    r3 = r30;
    r4 = 0x0;
    fn_8020D868();
    r3 = r30;
    r4 = -0x1;
    fn_8020D78C();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F150C | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F150C(void) {
    extern void fn_801F37B0();
    extern void fn_801F3984();
    extern void fn_801F1554();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r4 = (u32)fn_801F1554;
    r5 = 0x0;
    r4 = (u32)fn_801F1554;
    r6 = 0x0;
    r31 = r3;
    fn_801F37B0();
    r3 = r31;
    r4 = 0x1;
    fn_801F3984();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F1554 | size: 0x34 */
extern u32 fn_801254B4(void* context, u32 slot, u16 tableId, u32 flags, u32 value);
s32 fn_801F1554(void* context) {
    fn_801254B4(context, 0, 0x112, 0, 1);
    return 1;
}

/* 0x801F1588 | size: 0x178 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1588(void) {
    extern void fn_801EF634();
    extern void fn_801F025C();
    extern void fn_801F3984();
    extern void fn_801F54A4();
    extern void fn_801F6FD4();
    extern void fn_801F7090();
    extern void fn_801F7174();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r24, 0x10(r1) */;
    r31 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_801F16EC;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r25 = r3 & 0xFFFF;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x17;
    r6 = 0x0;
    fn_801F54A4();
    r24 = r3 & 0xFFFF;
    r3 = 0x4;
    r4 = 0x0;
    fn_801F025C();
    r4 = r25;
    r26 = r3;
    r5 = r24;
    fn_801F6FD4();
    r28 = r3;
    r3 = r26;
    r4 = r25;
    r5 = r24;
    fn_801F7174();
    r29 = r3;
    r3 = r26;
    r4 = r25;
    r5 = r24;
    fn_801F7090();
    r0 = r29 * 0x64;
    r4 = 0x0;
    r26 = (u32)r0 / (u32)r3;
    r3 = 0x5;
    fn_801F025C();
    r4 = r25;
    r27 = r3;
    r5 = r24;
    fn_801F6FD4();
    r29 = r3;
    r3 = r27;
    r4 = r25;
    r5 = r24;
    fn_801F7174();
    r30 = r3;
    r3 = r27;
    r4 = r25;
    r5 = r24;
    fn_801F7090();
    r0 = r30 * 0x64;
    r4 = 0x0;
    r27 = (u32)r0 / (u32)r3;
    r3 = r31;
    fn_801F3984();
    r3 = r28 & 0xFFFF;
    r0 = r29 & 0xFFFF;
    if ((u32)r3 <= (u32)r0) goto L_801F169C;
    r3 = r31;
    r4 = 0x2;
    fn_801F3984();
L_801F169C: ;
    r3 = r28 & 0xFFFF;
    r0 = r29 & 0xFFFF;
    if ((u32)r3 >= (u32)r0) goto L_801F16B8;
    r3 = r31;
    r4 = 0x3;
    fn_801F3984();
L_801F16B8: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)r0) goto L_801F16EC;
    if ((u32)r26 < (u32)r27) goto L_801F16D8;
    r3 = r31;
    r4 = 0x2;
    fn_801F3984();
L_801F16D8: ;
    if ((u32)r26 > (u32)r27) goto L_801F16EC;
    r3 = r31;
    r4 = 0x3;
    fn_801F3984();
L_801F16EC: ;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F1700 | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1700(void) {
    extern void fn_80077B84();
    extern void fn_801F54A4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0x34;
    r6 = 0x0;
    fn_801F54A4();
    r31 = r3 & 0xFF;
    fn_80077B84();
    if ((u32)r31 != (u32)0x1) goto L_801F1740;
    if ((s32)r3 <= (s32)0x0) goto L_801F1740;
    r3 = 0x1;
    goto L_801F1744;
L_801F1740: ;
    r3 = 0x0;
L_801F1744: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F17B0 | size: 0xD8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F17B0(void) {
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
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
    r26 = r3;
    r27 = 0x0;
    goto L_801F1868;
L_801F17CC: ;
    r3 = r26;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F1864;
    r29 = 0x0;
    goto L_801F1858;
L_801F17F0: ;
    r3 = r31;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F1854;
    r28 = 0x0;
    goto L_801F1848;
L_801F1814: ;
    r3 = r30;
    r6 = r28;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    if ((u32)r3 == (u32)0x0) goto L_801F1844;
    r4 = 0x0;
    r5 = 0xfa;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_801F1844: ;
    r28 = r28 + 0x1;
L_801F1848: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F1814;
L_801F1854: ;
    r29 = r29 + 0x1;
L_801F1858: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F17F0;
L_801F1864: ;
    r27 = r27 + 0x1;
L_801F1868: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F17CC;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F1888 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1888(void) {
    extern void fn_801F54A4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0x1a;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xa) goto L_801F18C0;
    if ((u32)r0 == (u32)0x11) goto L_801F18C0;
    if ((u32)r0 != (u32)0xb) goto L_801F18C8;
L_801F18C0: ;
    r3 = 0x1;
    goto L_801F18CC;
L_801F18C8: ;
    r3 = 0x0;
L_801F18CC: ;
    return;
}
#pragma pop

/* 0x801F18DC | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F18DC(void) {
    extern void fn_801F54A4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0x1a;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    r0 = 0x10 - r0;
    r0 = __cntlzw(r0);
    /* extrwi r3, r0, 8, 19 */;
    return;
}
#pragma pop

/* 0x801F1918 | size: 0x74 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1918(void) {
    extern void fn_800F9E70();
    extern void fn_801F54A4();
    extern void fn_802037DC();
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
    r3 = r4;
    fn_802037DC();
    r4 = r3;
    r3 = r31;
    fn_800F9E70();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x22;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1970;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x16) = r0;
    goto L_801F1978;
L_801F1970: ;
    r0 = 0x1;
    *(u8*)((u8*)r31 + 0x16) = r0;
L_801F1978: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F198C | size: 0x4 | trivial */
s32 fn_801F198C(void) { return 0; }

/* 0x801F1990 | size: 0xDC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1990(void) {
    extern void fn_801F37B0();
    extern void fn_80204DE4();
    extern void fn_801F1C98();
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
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r9 = 0x0;
    /* stmw r27, 0x4c(r1) */;
    r30 = r7;
    r31 = r8;
    r8 = r9;
    r7 = r1 + 0x1c;
    goto L_801F19C4;
L_801F19B8: ;
    /* clrlslwi r0, r9, 16, 2 */;
    r9 = r9 + 0x1;
    *(u32*)(r7 + r0) = r8;
L_801F19C4: ;
    r0 = r9 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F19B8;
    r7 = r5 & 0xFF;
    r0 = r6 & 0xFF;
    r6 = r1 + 0x1c;
    r8 = 0x0;
    r5 = (u32)fn_801F1C98;
    r6 = 0x0;
    r4 = (u32)fn_801F1C98;
    r5 = r1 + 0x8;
    *(u32*)(sp + 0x18) = r0;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0x10);
    r29 = r1 + 0x1c;
    r27 = 0x0;
    r28 = r0 & 0xFFFF;
    goto L_801F1A48;
L_801F1A1C: ;
    /* clrlslwi r0, r27, 16, 2 */;
    r4 = r30;
    r3 = *(u32*)(r29 + r0);
    r5 = r31;
    fn_80204DE4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1A44;
    r3 = 0x1;
    goto L_801F1A58;
L_801F1A44: ;
    r27 = r27 + 0x1;
L_801F1A48: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_801F1A1C;
    r3 = 0x0;
L_801F1A58: ;
    /* lmw r27, 0x4c(r1) */;
    return;
}
#pragma pop

/* 0x801F1A6C | size: 0xA8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1A6C(void) {
    extern void fn_801F2B5C();
    extern void fn_801F54A4();
    extern void fn_801F1B14();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x28(r1) */;
    r30 = r3;
    r31 = r7;
    r7 = 0x0;
    r3 = 0x0;
    goto L_801F1A9C;
L_801F1A90: ;
    /* clrlslwi r0, r7, 16, 2 */;
    r7 = r7 + 0x1;
    *(u32*)(r5 + r0) = r3;
L_801F1A9C: ;
    r0 = r7 & 0xFFFF;
    if ((u32)r0 < (u32)0x18) goto L_801F1A90;
    r7 = r6 & 0xFF;
    r0 = 0x0;
    r3 = 0x0;
    r4 = 0x0;
    r6 = 0x0;
    r5 = 0x17;
    *(u32*)(sp + 0x10) = r0;
    fn_801F54A4();
    r0 = r31 & 0xFF;
    r4 = (u32)fn_801F1B14;
    r4 = (u32)fn_801F1B14;
    r3 = r30;
    r5 = r1 + 0x8;
    *(u32*)(sp + 0x1C) = r0;
    r6 = 0x0;
    fn_801F2B5C();
    r0 = *(u32*)(sp + 0x10);
    /* lmw r30, 0x28(r1) */;
    r3 = r0 & 0xFFFF;
    return;
}
#pragma pop

/* 0x801F1B14 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1B14(void) {
    extern void fn_801F8424();
    extern void fn_801F986C();
    extern void fn_801FA634();
    extern void fn_80206608();
    u8 sp[0x20];
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

    /* stmw r26, 0x8(r1) */;
    r31 = r5;
    r26 = r4;
    r29 = r3;
    r30 = *(u32*)((u8*)r5 + 0x0);
    r28 = *(u32*)((u8*)r5 + 0xC);
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F1B4C;
    r3 = 0x1;
    goto L_801F1C04;
L_801F1B4C: ;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((s32)r0 != (s32)0x0) goto L_801F1B7C;
    r3 = r29;
    r4 = r30;
    r5 = r26;
    fn_801F8424();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1B9C;
    r3 = 0x1;
    goto L_801F1C04;
L_801F1B7C: ;
    r3 = r29;
    r4 = r30;
    r5 = r26;
    fn_801F8424();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1B9C;
    r3 = 0x1;
    goto L_801F1C04;
L_801F1B9C: ;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r27 = 0x0;
    r26 = r0 & 0xFFFF;
    goto L_801F1BF4;
L_801F1BAC: ;
    r3 = r29;
    r4 = r27;
    fn_801F986C();
    /* mr. r30, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_801F1BF0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    if ((s32)r0 != (s32)0x1) goto L_801F1BD8;
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_801F1BF0;
L_801F1BD8: ;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 << 2;
    *(u32*)(r28 + r0) = r30;
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_801F1BF0: ;
    r27 = r27 + 0x1;
L_801F1BF4: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_801F1BAC;
    r3 = 0x1;
L_801F1C04: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F1C18 | size: 0x80 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1C18(void) {
    extern void fn_801F37B0();
    extern void fn_801F1C98();
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

    r9 = 0x0;
    r8 = r9;
    goto L_801F1C3C;
L_801F1C30: ;
    /* clrlslwi r0, r9, 16, 2 */;
    r9 = r9 + 0x1;
    *(u32*)(r5 + r0) = r8;
L_801F1C3C: ;
    r0 = r9 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F1C30;
    r8 = r6 & 0xFF;
    r7 = r7 & 0xFF;
    r9 = 0x0;
    r6 = (u32)fn_801F1C98;
    r0 = (u32)fn_801F1C98;
    r5 = r1 + 0x8;
    r6 = 0x0;
    r4 = r0;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0x10);
    r3 = r0 & 0xFFFF;
    return;
}
#pragma pop

/* 0x801F1C98 | size: 0xC4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1C98(void) {
    extern void fn_801F8424();
    extern void fn_802062FC();
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
    r28 = r5;
    r27 = r4;
    r31 = r3;
    r0 = *(u32*)((u8*)r5 + 0x10);
    r30 = *(u32*)((u8*)r5 + 0x0);
    r29 = *(u32*)((u8*)r5 + 0x4);
    if ((s32)r0 != (s32)0x1) goto L_801F1CDC;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0x1) goto L_801F1CDC;
    r3 = 0x1;
    goto L_801F1D48;
L_801F1CDC: ;
    r0 = *(u32*)((u8*)r28 + 0xC);
    if ((s32)r0 != (s32)0x0) goto L_801F1D0C;
    r3 = r30;
    r4 = r31;
    r5 = r27;
    fn_801F8424();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1D2C;
    r3 = 0x1;
    goto L_801F1D48;
L_801F1D0C: ;
    r3 = r30;
    r4 = r31;
    r5 = r27;
    fn_801F8424();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1D2C;
    r3 = 0x1;
    goto L_801F1D48;
L_801F1D2C: ;
    r0 = *(u32*)((u8*)r28 + 0x8);
    r3 = 0x1;
    r0 = r0 << 2;
    *(u32*)(r29 + r0) = r31;
    r4 = *(u32*)((u8*)r28 + 0x8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r28 + 0x8) = r0;
L_801F1D48: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x801F1D5C | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1D5C(void) {
    extern void fn_801F61EC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;

    r8 = 0x0;
    r11 = r4;
    r10 = r5;
    r9 = r6;
    r4 = r8;
    goto L_801F1D8C;
L_801F1D80: ;
    /* clrlslwi r0, r8, 16, 2 */;
    r8 = r8 + 0x1;
    *(u32*)(r7 + r0) = r4;
L_801F1D8C: ;
    r0 = r8 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F1D80;
    r4 = r7;
    r5 = r11;
    r6 = r10;
    r7 = r9;
    fn_801F61EC();
    return;
}
#pragma pop

/* 0x801F1DBC | size: 0x174 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1DBC(void) {
    extern void fn_801F0058();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7404();
    extern void fn_801FB8F8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r0 = r4 & 0xFFFF;
    /* stmw r24, 0x10(r1) */;
    r31 = r4;
    r24 = r3;
    if ((u32)r0 == (u32)0x2) goto L_801F1DE8;
    if ((u32)r0 != (u32)0x3) goto L_801F1F18;
L_801F1DE8: ;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r3 = r24;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r27 = 0x0;
    goto L_801F1EA8;
L_801F1E34: ;
    r3 = r24;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r25 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x3) goto L_801F1E5C;
    r25 = 0x0;
L_801F1E5C: ;
    if ((u32)r25 == (u32)0x0) goto L_801F1EA4;
    r26 = 0x0;
    goto L_801F1E98;
L_801F1E6C: ;
    r3 = r25;
    r4 = r26;
    fn_801F7258();
    /* mr. r29, r3 */;
    if ((u32)r25 == (u32)0x0) goto L_801F1E94;
    fn_801FB8F8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1E94;
    goto L_801F1EB8;
L_801F1E94: ;
    r26 = r26 + 0x1;
L_801F1E98: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_801F1E6C;
L_801F1EA4: ;
    r27 = r27 + 0x1;
L_801F1EA8: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F1E34;
    r29 = 0x0;
L_801F1EB8: ;
    if ((u32)r29 == (u32)0x0) goto L_801F1EE0;
    r3 = r29;
    r4 = r30;
    fn_801F0058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1EE0;
    r0 = 0x1;
    goto L_801F1EE4;
L_801F1EE0: ;
    r0 = 0x0;
L_801F1EE4: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F1F04;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x2) goto L_801F1F18;
    r3 = 0x1;
    goto L_801F1F1C;
L_801F1F04: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x3) goto L_801F1F18;
    r3 = 0x1;
    goto L_801F1F1C;
L_801F1F18: ;
    r3 = 0x0;
L_801F1F1C: ;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F1F30 | size: 0x4C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1F30(void) {
    extern void fn_801F37B0();
    extern void fn_801F1F7C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r6 = r5 & 0xFFFF;
    r5 = (u32)fn_801F1F7C;
    r0 = 0x0;
    r4 = (u32)fn_801F1F7C;
    r5 = r1 + 0x8;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0x10);
    r3 = r0 & 0xFF;
    return;
}
#pragma pop

/* 0x801F1F7C | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F1F7C(void) {
    extern void fn_80123B5C();
    extern void fn_802026E4();
    extern void fn_80202B88();
    extern void fn_80205B8C();
    extern void fn_802062FC();
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
    r28 = r5;
    r31 = r3;
    r0 = *(u32*)((u8*)r5 + 0x4);
    r30 = *(u32*)((u8*)r5 + 0x0);
    r29 = r0 & 0xFFFF;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F1FB4;
    r3 = 0x1;
    goto L_801F200C;
L_801F1FB4: ;
    r3 = r31;
    r4 = r30;
    fn_80202B88();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F2008;
    r3 = r31;
    r4 = 0x27;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F2008;
    r3 = r31;
    fn_80205B8C();
    r4 = r29;
    fn_80123B5C();
    r0 = (s8)r3;
    if ((u32)r0 < (u32)0x1) goto L_801F2008;
    r0 = 0x1;
    r3 = 0x0;
    *(u32*)((u8*)r28 + 0x8) = r0;
    goto L_801F200C;
L_801F2008: ;
    r3 = 0x1;
L_801F200C: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F2020 | size: 0x1FC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2020(void) {
    extern void fn_801F37B0();
    extern void fn_802026E4();
    extern void fn_80206780();
    extern void fn_80207AE0();
    extern void fn_80207BF4();
    extern void fn_801F34EC();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x44(r1) */;
    r27 = r4;
    r26 = r3;
    r28 = r5;
    r30 = 0x0;
    r3 = r27;
    r29 = 0x0;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F205C;
    r3 = 0x0;
    goto L_801F2208;
L_801F205C: ;
    r5 = 0x17;
    r6 = 0x0;
    r0 = 0x2;
    r3 = (u32)fn_801F34EC;
    r4 = (u32)fn_801F34EC;
    r3 = r26;
    r5 = r1 + 0x28;
    r6 = 0x0;
    *(u32*)(sp + 0x30) = r0;
    fn_801F37B0();
    r6 = 0x47;
    r5 = 0x0;
    r0 = 0x2;
    r3 = (u32)fn_801F34EC;
    r4 = (u32)fn_801F34EC;
    r31 = *(u32*)(sp + 0x2C);
    r3 = r26;
    r5 = r1 + 0x18;
    r6 = 0x0;
    *(u32*)(sp + 0x20) = r0;
    fn_801F37B0();
    r0 = 0x0;
    r4 = 0x2a;
    r3 = (u32)fn_801F34EC;
    r4 = (u32)fn_801F34EC;
    r25 = *(u32*)(sp + 0x1C);
    *(u32*)(sp + 0xC) = r0;
    r3 = r26;
    r5 = r1 + 0x8;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
    r26 = *(u32*)(sp + 0xC);
    r3 = r27;
    r4 = 0x2;
    fn_80207AE0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_801F212C;
    r3 = r27;
    fn_80207BF4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1a) goto L_801F2130;
L_801F212C: ;
    r30 = 0x1;
L_801F2130: ;
    r3 = r27;
    r4 = 0x8;
    fn_80207AE0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F214C;
    r29 = 0x1;
L_801F214C: ;
    r3 = r27;
    r4 = 0x16;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_801F2194;
    r3 = r27;
    r4 = 0xe;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_801F2194;
    r3 = r27;
    r4 = 0x25;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F219C;
L_801F2194: ;
    r3 = 0x1;
    goto L_801F2208;
L_801F219C: ;
    if ((u32)r31 == (u32)0x0) goto L_801F21B8;
    if ((u32)r28 == (u32)0x0) goto L_801F21B0;
    *(u32*)((u8*)r28 + 0x0) = r31;
L_801F21B0: ;
    r3 = 0x2;
    goto L_801F2208;
L_801F21B8: ;
    if ((u32)r25 == (u32)0x0) goto L_801F21DC;
    r0 = r30 & 0xFF;
    if ((u32)r25 != (u32)0x0) goto L_801F21DC;
    if ((u32)r28 == (u32)0x0) goto L_801F21D4;
    *(u32*)((u8*)r28 + 0x0) = r25;
L_801F21D4: ;
    r3 = 0x2;
    goto L_801F2208;
L_801F21DC: ;
    if ((u32)r26 == (u32)0x0) goto L_801F2204;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F2204;
    if ((u32)r28 == (u32)0x0) goto L_801F21FC;
    *(u32*)((u8*)r28 + 0x0) = r26;
L_801F21FC: ;
    r3 = 0x2;
    goto L_801F2208;
L_801F2204: ;
    r3 = 0x0;
L_801F2208: ;
    /* lmw r25, 0x44(r1) */;
    return;
}
#pragma pop

/* 0x801F221C | size: 0xBC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F221C(void) {
    extern void fn_8012640C();
    extern void fn_801F1170();
    extern void fn_801F54A4();
    extern void fn_802062FC();
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
    r29 = 0x1;
    r30 = 0x0;
    goto L_801F22B4;
L_801F223C: ;
    r3 = r28;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x59;
    fn_801F54A4();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F22B0;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_801F22B0;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) goto L_801F22B0;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_801F22B0;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 != (s32)0x0) goto L_801F22B0;
    r29 = 0x0;
    goto L_801F22C0;
L_801F22B0: ;
    r30 = r30 + 0x1;
L_801F22B4: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F223C;
L_801F22C0: ;
    r3 = r29;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F22D8 | size: 0x78 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F22D8(void) {
    extern void fn_801F025C();
    extern void fn_801F4C14();
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
    r30 = r3;
    r3 = 0x11;
    r4 = r30;
    fn_801F025C();
    r31 = r3;
    r4 = r30;
    r3 = 0x12;
    fn_801F025C();
    r0 = r3;
    r3 = r30;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r7 = r31;
    r4 = 0x0;
    r5 = 0x43;
    r6 = 0x0;
    fn_801F4C14();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F2350 | size: 0xE4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2350(void) {
    extern void fn_8012640C();
    extern void fn_801FD0BC();
    extern void fn_801FD0D4();
    extern void fn_801FD104();
    extern void fn_80206780();
    extern void fn_8020E614();
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
    if ((u32)r4 != (u32)0x0) goto L_801F2370;
    r3 = -0x1;
    goto L_801F2420;
L_801F2370: ;
    r3 = r4;
    r4 = 0x0;
    r5 = 0x122;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3;
    r30 = -0x1;
    r28 = 0x0;
    goto L_801F2410;
L_801F2394: ;
    r0 = r28 & 0xFFFF;
    r0 = r0 * 0xc;
    r29 = r31 + r0;
    r3 = r29;
    fn_8020E614();
    r0 = r3 & 0xFF;
    if ((u32)r4 == (u32)0x0) goto L_801F240C;
    r3 = r29;
    fn_801FD104();
    if ((u32)r3 == (u32)0x0) goto L_801F240C;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_801F240C;
    r3 = r29;
    fn_801FD0D4();
    r0 = r3;
    r3 = r29;
    r29 = r0;
    fn_801FD0BC();
    r0 = r29 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_801F23F0;
    r29 = 0x1;
L_801F23F0: ;
    r3 = r3 & 0xFFFF;
    r0 = r29 & 0xFFFF;
    r3 = r3 * 0x64;
    r0 = (s32)r3 / (s32)r0;
    if ((s32)r0 <= (s32)r30) goto L_801F240C;
    r30 = r0;
L_801F240C: ;
    r28 = r28 + 0x1;
L_801F2410: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F2394;
    r3 = r30;
L_801F2420: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F2434 | size: 0x164 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2434(void) {
    extern void fn_801F61EC();
    extern void fn_80203ADC();
    extern void fn_80203B5C();
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

    /* stmw r26, 0x28(r1) */;
    if ((u32)r4 != (u32)0x0) goto L_801F2454;
    r3 = -0x1;
    goto L_801F2584;
L_801F2454: ;
    r7 = 0x0;
    r5 = r1 + 0x8;
    r6 = r7;
    goto L_801F2470;
L_801F2464: ;
    /* clrlslwi r0, r7, 16, 2 */;
    r7 = r7 + 0x1;
    *(u32*)(r5 + r0) = r6;
L_801F2470: ;
    r0 = r7 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F2464;
    r7 = r4;
    r4 = r1 + 0x8;
    r5 = 0x1;
    r6 = 0x2;
    fn_801F61EC();
    r4 = r3 & 0xFFFF;
    r29 = r3;
    if ((u32)r0 != (u32)0x8) goto L_801F24A4;
    r3 = -0x1;
    goto L_801F2584;
L_801F24A4: ;
    r3 = r1 + 0x8;
    r5 = 0x0;
    goto L_801F2500;
L_801F24B0: ;
    /* clrlslwi r0, r5, 16, 2 */;
    r26 = *(u32*)(r3 + r0);
    if ((u32)r26 == (u32)0x0) goto L_801F24FC;
    r3 = r26;
    r4 = 0x1;
    fn_80203ADC();
    r28 = r3;
    r3 = r26;
    r4 = 0x1;
    fn_80203B5C();
    r0 = r3 & 0xFFFF;
    if ((u32)r26 != (u32)0x0) goto L_801F24E8;
    r3 = 0x1;
L_801F24E8: ;
    r4 = r28 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    r3 = r4 * 0x64;
    r31 = (s32)r3 / (s32)r0;
    goto L_801F250C;
L_801F24FC: ;
    r5 = r5 + 0x1;
L_801F2500: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)r4) goto L_801F24B0;
L_801F250C: ;
    r28 = r1 + 0x8;
    r30 = r29 & 0xFFFF;
    r27 = 0x0;
    goto L_801F2574;
L_801F251C: ;
    /* clrlslwi r0, r27, 16, 2 */;
    r26 = *(u32*)(r28 + r0);
    if ((u32)r26 == (u32)0x0) goto L_801F2570;
    r3 = r26;
    r4 = 0x1;
    fn_80203ADC();
    r29 = r3;
    r3 = r26;
    r4 = 0x1;
    fn_80203B5C();
    r0 = r3 & 0xFFFF;
    if ((u32)r26 != (u32)0x0) goto L_801F2554;
    r3 = 0x1;
L_801F2554: ;
    r4 = r29 & 0xFFFF;
    r0 = r3 & 0xFFFF;
    r3 = r4 * 0x64;
    r0 = (s32)r3 / (s32)r0;
    if ((s32)r0 >= (s32)r31) goto L_801F2570;
    r31 = r0;
L_801F2570: ;
    r27 = r27 + 0x1;
L_801F2574: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_801F251C;
    r3 = r31;
L_801F2584: ;
    /* lmw r26, 0x28(r1) */;
    return;
}
#pragma pop

/* 0x801F2598 | size: 0xBC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2598(void) {
    extern void fn_800E0C54();
    extern void fn_801F61EC();
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
    u32 r31 = 0;

    r9 = r5;
    r8 = 0x0;
    r7 = r6;
    r6 = r8;
    r5 = r1 + 0x8;
    goto L_801F25CC;
L_801F25C0: ;
    /* clrlslwi r0, r8, 16, 2 */;
    r8 = r8 + 0x1;
    *(u32*)(r5 + r0) = r6;
L_801F25CC: ;
    r0 = r8 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F25C0;
    r5 = r4;
    r6 = r9;
    r4 = r1 + 0x8;
    fn_801F61EC();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if ((u32)r0 != (u32)0x8) goto L_801F25FC;
    r3 = 0x0;
    goto L_801F2640;
L_801F25FC: ;
    fn_800E0C54();
    r4 = r3 & 0xFFFF;
    r3 = r31 & 0xFFFF;
    r0 = (s32)r4 / (s32)r3;
    r0 = r0 * r3;
    r0 = r4 - r0;
    r0 = r0 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F2628;
    r3 = 0x0;
    goto L_801F2640;
L_801F2628: ;
    /* clrlslwi r0, r0, 16, 2 */;
    r3 = r1 + 0x8;
    r3 = *(u32*)(r3 + r0);
    if ((u32)r3 != (u32)0x0) goto L_801F2640;
    r3 = 0x0;
L_801F2640: ;
    r31 = *(u32*)(sp + 0x2C);
    return;
}
#pragma pop

/* 0x801F2654 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2654(void) {
    extern void fn_801F37B0();
    extern void fn_801F26A8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r7 = r4 & 0xFF;
    r4 = (u32)fn_801F26A8;
    r0 = r6 & 0xFF;
    r6 = 0x0;
    r4 = (u32)fn_801F26A8;
    r5 = r1 + 0x8;
    r6 = 0x0;
    *(u32*)(sp + 0x14) = r0;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0xC);
    r3 = r0 & 0xFFFF;
    return;
}
#pragma pop

/* 0x801F26A8 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F26A8(void) {
    extern void fn_801F025C();
    extern void fn_802062FC();
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
    r28 = r5;
    r31 = r3;
    r0 = *(u32*)((u8*)r5 + 0xC);
    r30 = *(u32*)((u8*)r5 + 0x0);
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F26E8;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F26E8;
    r3 = 0x1;
    goto L_801F27C0;
L_801F26E8: ;
    if ((u32)r30 != (u32)0x0) goto L_801F26F8;
    r29 = 0x0;
    goto L_801F2738;
L_801F26F8: ;
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((s32)r0 != (s32)0x1) goto L_801F2718;
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r29 = r3;
    goto L_801F2738;
L_801F2718: ;
    if ((s32)r0 != (s32)0x2) goto L_801F2734;
    r4 = r30;
    r3 = 0x3;
    fn_801F025C();
    r29 = r3;
    goto L_801F2738;
L_801F2734: ;
    r29 = 0x0;
L_801F2738: ;
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((s32)r0 == (s32)0x1) goto L_801F2758;
    if ((s32)r0 != (s32)0x2) goto L_801F2768;
L_801F2758: ;
    if ((u32)r29 != (u32)0x0) goto L_801F2768;
    r3 = 0x1;
    goto L_801F27C0;
L_801F2768: ;
    if ((s32)r0 != (s32)0x0) goto L_801F2788;
    if ((u32)r30 == (u32)0x0) goto L_801F27B0;
    if ((u32)r30 != (u32)r31) goto L_801F27B0;
    r3 = 0x1;
    goto L_801F27C0;
L_801F2788: ;
    if ((s32)r0 == (s32)0x1) goto L_801F2798;
    if ((s32)r0 != (s32)0x2) goto L_801F27A8;
L_801F2798: ;
    if ((u32)r29 == (u32)r3) goto L_801F27B0;
    r3 = 0x1;
    goto L_801F27C0;
L_801F27A8: ;
    r3 = 0x1;
    goto L_801F27C0;
L_801F27B0: ;
    r4 = *(u32*)((u8*)r28 + 0x4);
    r3 = 0x1;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r28 + 0x4) = r0;
L_801F27C0: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F27D4 | size: 0x30 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F27D4(void) {
    extern void fn_801F37B0();
    extern void fn_801F2804();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = (u32)fn_801F2804;
    r5 = 0x0;
    r4 = (u32)fn_801F2804;
    r6 = 0x0;
    fn_801F37B0();
    return;
}
#pragma pop

/* 0x801F2804 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2804(void) {
    extern void fn_8012640C();
    extern void fn_80209FAC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    fn_8012640C();
    fn_80209FAC();
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x801F2838 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2838(void) {
    extern void fn_80119ED0();
    extern void fn_8011AB50();
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
    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x9) goto L_801F2878;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8011AB50();
L_801F2878: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F288C | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F288C(void) {
    extern void fn_80119ED0();
    extern void fn_8011ACB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x9) goto L_801F28C0;
    r3 = -0x1;
    goto L_801F28CC;
L_801F28C0: ;
    r3 = r30;
    r4 = r31;
    fn_8011ACB4();
L_801F28CC: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F28E0 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F28E0(void) {
    extern void fn_80119ED0();
    extern void fn_8011AE40();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x9) goto L_801F2914;
    r3 = -0x1;
    goto L_801F2920;
L_801F2914: ;
    r3 = r30;
    r4 = r31;
    fn_8011AE40();
L_801F2920: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F2934 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2934(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
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
    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x9) goto L_801F2974;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8011B2C0();
L_801F2974: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F2988 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2988(void) {
    extern void fn_80119ED0();
    extern void fn_8011B444();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x9) goto L_801F29BC;
    r3 = 0x0;
    goto L_801F29C8;
L_801F29BC: ;
    r3 = r30;
    r4 = r31;
    fn_8011B444();
L_801F29C8: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F29DC | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F29DC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x9) goto L_801F2A10;
    r3 = 0x0;
    goto L_801F2A1C;
L_801F2A10: ;
    r3 = r30;
    r4 = r31;
    fn_8011B67C();
L_801F2A1C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F2A30 | size: 0x4C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2A30(void) {
    extern void fn_80119ED0();
    extern void fn_8011B788();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x9) goto L_801F2A68;
    r3 = r30;
    r4 = r31;
    fn_8011B788();
L_801F2A68: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F2A7C | size: 0xE0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2A7C(void) {
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7404();
    extern void fn_801FB8F8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    /* stmw r26, 0x8(r1) */;
    r26 = r3;
    fn_801F54A4();
    r3 = r26;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r29 = 0x0;
    goto L_801F2B38;
L_801F2AC0: ;
    r3 = r26;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r31 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F2AE8;
    r31 = 0x0;
L_801F2AE8: ;
    if ((u32)r31 == (u32)0x0) goto L_801F2B34;
    r30 = 0x0;
    goto L_801F2B28;
L_801F2AF8: ;
    r3 = r31;
    r4 = r30;
    fn_801F7258();
    /* mr. r27, r3 */;
    if ((u32)r31 == (u32)0x0) goto L_801F2B24;
    fn_801FB8F8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F2B24;
    r3 = r27;
    goto L_801F2B48;
L_801F2B24: ;
    r30 = r30 + 0x1;
L_801F2B28: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_801F2AF8;
L_801F2B34: ;
    r29 = r29 + 0x1;
L_801F2B38: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F2AC0;
    r3 = 0x0;
L_801F2B48: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F2B5C | size: 0x3E0 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2B5C(void) {
    extern void fn_800F0494();
    extern void fn_800F04BC();
    extern void fn_800F0654();
    extern void fn_800F07A8();
    extern void fn_800FF560();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7404();
    extern void fn_801FB1C0();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
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
    void (*ctr_fn)(void) = 0;

    /* stmw r20, 0x20(r1) */;
    r31 = r3;
    r30 = r4;
    r29 = r5;
    r20 = r6;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    r3 = r1 + 0x8;
    r5 = 0x0;
    r4 = 0x0;
    goto L_801F2BC8;
L_801F2BBC: ;
    /* clrlslwi r0, r5, 16, 2 */;
    r5 = r5 + 0x1;
    *(u32*)(r3 + r0) = r4;
L_801F2BC8: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F2BBC;
    r0 = r20 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F2D88;
    r25 = r28;
    r22 = 0x0;
    r24 = 0x0;
    goto L_801F2CF4;
L_801F2BF0: ;
    r3 = r31;
    r6 = r24;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r26 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F2C18;
    r26 = 0x0;
L_801F2C18: ;
    if ((u32)r26 == (u32)0x0) goto L_801F2CF0;
    r23 = 0x0;
    goto L_801F2CE4;
L_801F2C28: ;
    r3 = r26;
    r4 = r23;
    fn_801F7258();
    /* mr. r21, r3 */;
    if ((u32)r26 == (u32)0x0) goto L_801F2CE0;
    r4 = 0x0;
    r5 = 0x43;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r20 = r3 & 0xFFFF;
    r3 = r21;
    r4 = 0x0;
    r5 = 0x4b;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r20 != (u32)0x0) goto L_801F2CE0;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 >= (u32)0x4) goto L_801F2CE0;
    fn_800FF560();
    r4 = r3;
    r8 = r30;
    r3 = 0x12;
    r5 = 0x2000;
    r6 = 0x1;
    r7 = 0x0;
    fn_800F07A8();
    /* clrlslwi r4, r22, 16, 2 */;
    r5 = r1 + 0x8;
    /* addic. r0, r1, 0x8 */;
    *(u32*)(r5 + r4) = r3;
    if ((u32)r0 == (u32)0x4) goto L_801F2CE0;
    r3 = *(u32*)(r5 + r4);
    r5 = r21;
    r6 = r25;
    r7 = r29;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800F0654();
    r22 = r22 + 0x1;
L_801F2CE0: ;
    r23 = r23 + 0x1;
L_801F2CE4: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_801F2C28;
L_801F2CF0: ;
    r24 = r24 + 0x1;
L_801F2CF4: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F2BF0;
    r21 = r1 + 0x8;
L_801F2D04: ;
    ((void(*)(void))fn_800F0308)();
    r22 = 0x0;
    goto L_801F2D34;
L_801F2D10: ;
    /* clrlslwi r0, r22, 16, 2 */;
    r3 = *(u32*)(r21 + r0);
    if ((u32)r3 == (u32)0x0) goto L_801F2D30;
    fn_800F04BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_801F2D40;
L_801F2D30: ;
    r22 = r22 + 0x1;
L_801F2D34: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F2D10;
L_801F2D40: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F2D04;
    r24 = 0x0;
    r23 = r1 + 0x8;
    r21 = r24;
    goto L_801F2D78;
L_801F2D5C: ;
    /* clrlslwi r22, r24, 16, 2 */;
    r3 = *(u32*)(r23 + r22);
    if ((u32)r3 == (u32)0x0) goto L_801F2D74;
    fn_800F0494();
    *(u32*)(r23 + r22) = r21;
L_801F2D74: ;
    r24 = r24 + 0x1;
L_801F2D78: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_801F2D5C;
    goto L_801F2E58;
L_801F2D88: ;
    r22 = 0x0;
    goto L_801F2E4C;
L_801F2D90: ;
    r3 = r31;
    r6 = r22;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r21 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x4) goto L_801F2DB8;
    r21 = 0x0;
L_801F2DB8: ;
    if ((u32)r21 == (u32)0x0) goto L_801F2E48;
    r23 = 0x0;
    goto L_801F2E3C;
L_801F2DC8: ;
    r3 = r21;
    r4 = r23;
    fn_801F7258();
    /* mr. r24, r3 */;
    if ((u32)r21 == (u32)0x0) goto L_801F2E38;
    r4 = 0x0;
    r5 = 0x43;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r25 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x4b;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r25 != (u32)0x0) goto L_801F2E38;
    r12 = r30;
    r3 = r24;
    r4 = r28;
    r5 = r29;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801F2E38: ;
    r23 = r23 + 0x1;
L_801F2E3C: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_801F2DC8;
L_801F2E48: ;
    r22 = r22 + 0x1;
L_801F2E4C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F2D90;
L_801F2E58: ;
    r22 = 0x0;
    goto L_801F2F1C;
L_801F2E60: ;
    r3 = r31;
    r6 = r22;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r21 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_801F2E88;
    r21 = 0x0;
L_801F2E88: ;
    if ((u32)r21 == (u32)0x0) goto L_801F2F18;
    r23 = 0x0;
    goto L_801F2F0C;
L_801F2E98: ;
    r3 = r21;
    r4 = r23;
    fn_801F7258();
    /* mr. r24, r3 */;
    if ((u32)r21 == (u32)0x0) goto L_801F2F08;
    r4 = 0x0;
    r5 = 0x43;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801FB1C0();
    r25 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x4b;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r25 == (u32)0x0) goto L_801F2F08;
    r12 = r30;
    r3 = r24;
    r4 = r28;
    r5 = r29;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801F2F08: ;
    r23 = r23 + 0x1;
L_801F2F0C: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_801F2E98;
L_801F2F18: ;
    r22 = r22 + 0x1;
L_801F2F1C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F2E60;
    /* lmw r20, 0x20(r1) */;
    return;
}
#pragma pop

/* 0x801F2F3C | size: 0x138 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F2F3C(void) {
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7404();
    extern void fn_801F981C();
    extern void fn_80202C1C();
    extern void fn_802062FC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    /* stmw r22, 0x8(r1) */;
    r27 = r3;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r31 = 0x0;
    goto L_801F3054;
L_801F2F9C: ;
    r3 = r27;
    r6 = r31;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r26 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F2FC4;
    r26 = 0x0;
L_801F2FC4: ;
    if ((u32)r26 == (u32)0x0) goto L_801F3050;
    r25 = 0x0;
    goto L_801F3044;
L_801F2FD4: ;
    r3 = r26;
    r4 = r25;
    fn_801F7258();
    /* mr. r23, r3 */;
    if ((u32)r26 == (u32)0x0) goto L_801F3040;
    r24 = 0x0;
    goto L_801F3034;
L_801F2FF0: ;
    r3 = r23;
    r4 = r24;
    fn_801F981C();
    /* mr. r22, r3 */;
    if ((u32)r26 == (u32)0x0) goto L_801F3030;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r26 == (u32)0x0) goto L_801F3030;
    r4 = r22;
    r5 = r30;
    r3 = 0x3;
    fn_801F02AC();
    r0 = r3;
    r3 = r22;
    r4 = r0;
    fn_80202C1C();
L_801F3030: ;
    r24 = r24 + 0x1;
L_801F3034: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_801F2FF0;
L_801F3040: ;
    r25 = r25 + 0x1;
L_801F3044: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r29) goto L_801F2FD4;
L_801F3050: ;
    r31 = r31 + 0x1;
L_801F3054: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F2F9C;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F3074 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3074(void) {
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_801F6B54();
    extern void fn_801F7258();
    extern void fn_801F7404();
    u8 sp[0x30];
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

    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    /* stmw r24, 0x10(r1) */;
    r24 = r3;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r26 = r3 & 0xFFFF;
    r29 = 0x0;
    goto L_801F3158;
L_801F30D4: ;
    r3 = r24;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r31 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F30FC;
    r31 = 0x0;
L_801F30FC: ;
    if ((u32)r31 == (u32)0x0) goto L_801F3154;
    r30 = 0x0;
    goto L_801F3148;
L_801F310C: ;
    r3 = r31;
    r4 = r30;
    fn_801F7258();
    /* mr. r25, r3 */;
    if ((u32)r31 == (u32)0x0) goto L_801F3144;
    r4 = r25;
    r5 = r28;
    r3 = 0x3;
    fn_801F02AC();
    r4 = r25;
    r5 = r28;
    r6 = r27;
    r7 = r26;
    fn_801F6B54();
L_801F3144: ;
    r30 = r30 + 0x1;
L_801F3148: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_801F310C;
L_801F3154: ;
    r29 = r29 + 0x1;
L_801F3158: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F30D4;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F3178 | size: 0x138 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3178(void) {
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7404();
    extern void fn_801F981C();
    extern void fn_80205274();
    extern void fn_802062FC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    /* stmw r22, 0x8(r1) */;
    r27 = r3;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r31 = 0x0;
    goto L_801F3290;
L_801F31D8: ;
    r3 = r27;
    r6 = r31;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r26 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F3200;
    r26 = 0x0;
L_801F3200: ;
    if ((u32)r26 == (u32)0x0) goto L_801F328C;
    r25 = 0x0;
    goto L_801F3280;
L_801F3210: ;
    r3 = r26;
    r4 = r25;
    fn_801F7258();
    /* mr. r23, r3 */;
    if ((u32)r26 == (u32)0x0) goto L_801F327C;
    r24 = 0x0;
    goto L_801F3270;
L_801F322C: ;
    r3 = r23;
    r4 = r24;
    fn_801F981C();
    /* mr. r22, r3 */;
    if ((u32)r26 == (u32)0x0) goto L_801F326C;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r26 == (u32)0x0) goto L_801F326C;
    r4 = r22;
    r5 = r30;
    r3 = 0x3;
    fn_801F02AC();
    r0 = r3;
    r3 = r22;
    r4 = r0;
    fn_80205274();
L_801F326C: ;
    r24 = r24 + 0x1;
L_801F3270: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_801F322C;
L_801F327C: ;
    r25 = r25 + 0x1;
L_801F3280: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r29) goto L_801F3210;
L_801F328C: ;
    r31 = r31 + 0x1;
L_801F3290: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F31D8;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F32B0 | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F32B0(void) {
    extern void fn_801F2B5C();
    extern void fn_801F32EC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = (u32)fn_801F32EC;
    r6 = 0x0;
    r0 = 0x0;
    r4 = (u32)fn_801F32EC;
    r5 = r1 + 0x8;
    *(u8*)(sp + 0x8) = r0;
    fn_801F2B5C();
    r3 = *(u8*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801F32EC | size: 0xFC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F32EC(void) {
    extern void fn_801F54A4();
    extern void fn_801F8A18();
    extern void fn_801FA634();
    extern void fn_801FB1C0();
    extern void fn_801FB8F8();
    extern void fn_802062FC();
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
    r28 = r5;
    r31 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F3318;
    r3 = 0x1;
    goto L_801F33D4;
L_801F3318: ;
    r0 = 0x0;
    r3 = r31;
    *(u16*)(sp + 0x8) = r0;
    r4 = r1 + 0x8;
    fn_801F8A18();
    if ((u32)r3 != (u32)0x0) goto L_801F333C;
    r3 = 0x1;
    goto L_801F33D4;
L_801F333C: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    r30 = 0x0;
    goto L_801F33C4;
L_801F335C: ;
    r3 = r31;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    if ((u32)r3 == (u32)0x0) goto L_801F33C0;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F33C0;
    if ((u32)r28 == (u32)0x0) goto L_801F33C0;
    r0 = *(u8*)((u8*)r28 + 0x0);
    if ((u32)r0 == (u32)0x2) goto L_801F33C0;
    r3 = r31;
    fn_801FB8F8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F33B8;
    r0 = 0x2;
    *(u8*)((u8*)r28 + 0x0) = r0;
    goto L_801F33C0;
L_801F33B8: ;
    r0 = 0x1;
    *(u8*)((u8*)r28 + 0x0) = r0;
L_801F33C0: ;
    r30 = r30 + 0x1;
L_801F33C4: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r29) goto L_801F335C;
    r3 = 0x1;
L_801F33D4: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F33E8 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F33E8(void) {
    extern void fn_801F37B0();
    extern void fn_801F3430();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r5 = r4 & 0xFFFF;
    r4 = (u32)fn_801F3430;
    r0 = 0x0;
    r4 = (u32)fn_801F3430;
    r6 = 0x0;
    r5 = r1 + 0x8;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0xC);
    r3 = r0 & 0xFFFF;
    return;
}
#pragma pop

/* 0x801F3430 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3430(void) {
    extern void fn_802026E4();
    extern void fn_802062FC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r5;
    r31 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F345C;
    r3 = 0x1;
    goto L_801F3488;
L_801F345C: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    r3 = r31;
    r4 = r0 & 0xFFFF;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F3484;
    r3 = *(u32*)((u8*)r30 + 0x4);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r30 + 0x4) = r0;
L_801F3484: ;
    r3 = 0x1;
L_801F3488: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F349C | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F349C(void) {
    extern void fn_801F37B0();
    extern void fn_801F34EC();
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

    r9 = r4 & 0xFFFF;
    r4 = (u32)fn_801F34EC;
    r0 = r6 & 0xFF;
    r8 = 0x0;
    r6 = r5;
    r4 = (u32)fn_801F34EC;
    r5 = r1 + 0x8;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
    r3 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F34EC | size: 0x138 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F34EC(void) {
    extern void fn_801F025C();
    extern void fn_802062FC();
    extern void fn_80207BF4();
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
    r28 = r5;
    r31 = r3;
    r30 = *(u32*)((u8*)r5 + 0xC);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F351C;
    r3 = 0x1;
    goto L_801F3610;
L_801F351C: ;
    if ((u32)r30 != (u32)0x0) goto L_801F352C;
    r29 = 0x0;
    goto L_801F356C;
L_801F352C: ;
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((s32)r0 != (s32)0x1) goto L_801F354C;
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r29 = r3;
    goto L_801F356C;
L_801F354C: ;
    if ((s32)r0 != (s32)0x2) goto L_801F3568;
    r4 = r30;
    r3 = 0x3;
    fn_801F025C();
    r29 = r3;
    goto L_801F356C;
L_801F3568: ;
    r29 = 0x0;
L_801F356C: ;
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((s32)r0 == (s32)0x1) goto L_801F358C;
    if ((s32)r0 != (s32)0x2) goto L_801F359C;
L_801F358C: ;
    if ((u32)r29 != (u32)0x0) goto L_801F359C;
    r3 = 0x1;
    goto L_801F3610;
L_801F359C: ;
    if ((s32)r0 != (s32)0x0) goto L_801F35BC;
    if ((u32)r30 == (u32)0x0) goto L_801F35E4;
    if ((u32)r30 != (u32)r31) goto L_801F35E4;
    r3 = 0x1;
    goto L_801F3610;
L_801F35BC: ;
    if ((s32)r0 == (s32)0x1) goto L_801F35CC;
    if ((s32)r0 != (s32)0x2) goto L_801F35DC;
L_801F35CC: ;
    if ((u32)r29 == (u32)r3) goto L_801F35E4;
    r3 = 0x1;
    goto L_801F3610;
L_801F35DC: ;
    r3 = 0x1;
    goto L_801F3610;
L_801F35E4: ;
    r3 = r31;
    fn_80207BF4();
    r0 = *(u32*)((u8*)r28 + 0x0);
    r3 = r3 & 0xFFFF;
    r0 = r0 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_801F360C;
    *(u32*)((u8*)r28 + 0x4) = r31;
    r3 = 0x0;
    goto L_801F3610;
L_801F360C: ;
    r3 = 0x1;
L_801F3610: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F3624 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3624(void) {
    extern void fn_801F37B0();
    extern void fn_801F3678();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r8 = r4 & 0xFFFF;
    r4 = (u32)fn_801F3678;
    r0 = r5 & 0xFF;
    r7 = 0x0;
    r4 = (u32)fn_801F3678;
    r5 = r1 + 0x8;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0xC);
    r3 = r0 & 0xFFFF;
    return;
}
#pragma pop

/* 0x801F3678 | size: 0x138 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3678(void) {
    extern void fn_801F025C();
    extern void fn_802062FC();
    extern void fn_80207BF4();
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
    r28 = r5;
    r31 = r3;
    r30 = *(u32*)((u8*)r5 + 0xC);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F36A8;
    r3 = 0x1;
    goto L_801F379C;
L_801F36A8: ;
    if ((u32)r30 != (u32)0x0) goto L_801F36B8;
    r29 = 0x0;
    goto L_801F36F8;
L_801F36B8: ;
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((s32)r0 != (s32)0x1) goto L_801F36D8;
    r4 = r30;
    r3 = 0x2;
    fn_801F025C();
    r29 = r3;
    goto L_801F36F8;
L_801F36D8: ;
    if ((s32)r0 != (s32)0x2) goto L_801F36F4;
    r4 = r30;
    r3 = 0x3;
    fn_801F025C();
    r29 = r3;
    goto L_801F36F8;
L_801F36F4: ;
    r29 = 0x0;
L_801F36F8: ;
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((s32)r0 == (s32)0x1) goto L_801F3718;
    if ((s32)r0 != (s32)0x2) goto L_801F3728;
L_801F3718: ;
    if ((u32)r29 != (u32)0x0) goto L_801F3728;
    r3 = 0x1;
    goto L_801F379C;
L_801F3728: ;
    if ((s32)r0 != (s32)0x0) goto L_801F3748;
    if ((u32)r30 == (u32)0x0) goto L_801F3770;
    if ((u32)r30 != (u32)r31) goto L_801F3770;
    r3 = 0x1;
    goto L_801F379C;
L_801F3748: ;
    if ((s32)r0 == (s32)0x1) goto L_801F3758;
    if ((s32)r0 != (s32)0x2) goto L_801F3768;
L_801F3758: ;
    if ((u32)r29 == (u32)r3) goto L_801F3770;
    r3 = 0x1;
    goto L_801F379C;
L_801F3768: ;
    r3 = 0x1;
    goto L_801F379C;
L_801F3770: ;
    r3 = r31;
    fn_80207BF4();
    r0 = *(u32*)((u8*)r28 + 0x0);
    r3 = r3 & 0xFFFF;
    r0 = r0 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_801F3798;
    r3 = *(u32*)((u8*)r28 + 0x4);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r28 + 0x4) = r0;
L_801F3798: ;
    r3 = 0x1;
L_801F379C: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F37B0 | size: 0x1D4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F37B0(void) {
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7404();
    extern void fn_801F981C();
    extern void fn_80206780();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
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
    void (*ctr_fn)(void) = 0;

    /* stmw r21, 0x14(r1) */;
    r23 = r4;
    r24 = r5;
    r21 = r6;
    r22 = r3;
    r25 = 0x1;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r3 = r22;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    r3 = r22;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r21 & 0xFF;
    r26 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F3890;
    r21 = 0x0;
    goto L_801F3880;
L_801F382C: ;
    r3 = r22;
    r6 = r21;
    r4 = 0x0;
    r5 = 0x59;
    fn_801F54A4();
    /* mr. r26, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_801F387C;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_801F387C;
    r12 = r23;
    r3 = r26;
    r4 = r28;
    r5 = r24;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F387C;
    r25 = 0x0;
    goto L_801F396C;
L_801F387C: ;
    r21 = r21 + 0x1;
L_801F3880: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F382C;
    goto L_801F396C;
L_801F3890: ;
    r30 = 0x0;
    goto L_801F3960;
L_801F3898: ;
    r31 = 0x0;
    goto L_801F3950;
L_801F38A0: ;
    r29 = 0x0;
    goto L_801F3940;
L_801F38A8: ;
    r3 = r22;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r21 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x8) goto L_801F38D0;
    r21 = 0x0;
L_801F38D0: ;
    if ((u32)r21 != (u32)0x0) goto L_801F38E0;
    r3 = 0x0;
    goto L_801F3910;
L_801F38E0: ;
    r3 = r21;
    r4 = r31;
    fn_801F7258();
    if ((u32)r3 != (u32)0x0) goto L_801F38FC;
    r3 = 0x0;
    goto L_801F3910;
L_801F38FC: ;
    r4 = r30;
    fn_801F981C();
    if ((u32)r3 != (u32)0x0) goto L_801F3910;
    r3 = 0x0;
L_801F3910: ;
    if ((u32)r3 == (u32)0x0) goto L_801F393C;
    r12 = r23;
    r4 = r28;
    r5 = r24;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F393C;
    r25 = 0x0;
    goto L_801F396C;
L_801F393C: ;
    r29 = r29 + 0x1;
L_801F3940: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F38A8;
    r31 = r31 + 0x1;
L_801F3950: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_801F38A0;
    r30 = r30 + 0x1;
L_801F3960: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_801F3898;
L_801F396C: ;
    r3 = r25;
    /* lmw r21, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F3984 | size: 0x1A0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3984(void) {
    extern void fn_801EF62C();
    extern void fn_801EF634();
    extern void fn_801F54A4();
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
    fn_801EF634();
    r31 = r3;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x23;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r31 & 0xFFFF;
    r0 = r30 & 0xFFFF;
    r3 = r3 & 0xFF;
    if ((u32)r4 != (u32)r0) goto L_801F39D4;
    r3 = 0x0;
    goto L_801F3B10;
L_801F39D4: ;
    if ((u32)r4 != (u32)0x1) goto L_801F39E4;
    r3 = 0x1;
    goto L_801F3B10;
L_801F39E4: ;
    if ((u32)r0 == (u32)0x0) goto L_801F39F4;
    if ((u32)r0 != (u32)0x1) goto L_801F3A04;
L_801F39F4: ;
    r3 = r30;
    fn_801EF62C();
    r3 = 0x1;
    goto L_801F3B10;
L_801F3A04: ;
    if ((u32)r3 != (u32)0x0) goto L_801F3A54;
    if ((u32)r4 == (u32)0x3) goto L_801F3A1C;
    if ((u32)r4 != (u32)0x5) goto L_801F3A24;
L_801F3A1C: ;
    r3 = 0x0;
    goto L_801F3B10;
L_801F3A24: ;
    if ((u32)r0 != (u32)0x7) goto L_801F3A3C;
    r3 = 0x3;
    fn_801EF62C();
    r3 = 0x1;
    goto L_801F3B10;
L_801F3A3C: ;
    if ((u32)r0 != (u32)0x6) goto L_801F3B04;
    r3 = 0x5;
    fn_801EF62C();
    r3 = 0x1;
    goto L_801F3B10;
L_801F3A54: ;
    if ((u32)r4 == (u32)0x7) goto L_801F3A64;
    if ((u32)r4 != (u32)0x6) goto L_801F3A6C;
L_801F3A64: ;
    r3 = 0x0;
    goto L_801F3B10;
L_801F3A6C: ;
    if ((u32)r4 != (u32)0x3) goto L_801F3A8C;
    if ((u32)r0 != (u32)0x2) goto L_801F3A8C;
    r3 = 0x7;
    fn_801EF62C();
    r3 = 0x1;
    goto L_801F3B10;
L_801F3A8C: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x2) goto L_801F3AB4;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x3) goto L_801F3AB4;
    r3 = 0x7;
    fn_801EF62C();
    r3 = 0x1;
    goto L_801F3B10;
L_801F3AB4: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x4) goto L_801F3ADC;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x5) goto L_801F3ADC;
    r3 = 0x6;
    fn_801EF62C();
    r3 = 0x1;
    goto L_801F3B10;
L_801F3ADC: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x5) goto L_801F3B04;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x4) goto L_801F3B04;
    r3 = 0x6;
    fn_801EF62C();
    r3 = 0x1;
    goto L_801F3B10;
L_801F3B04: ;
    r3 = r30;
    fn_801EF62C();
    r3 = 0x1;
L_801F3B10: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F3B24 | size: 0x90 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3B24(void) {
    extern void fn_801F3BB4();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    u8 sp[0x40];
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

    /* stmw r28, 0x30(r1) */;
    r28 = r3;
    r29 = r4;
    r31 = r1 + 0x8;
    r30 = 0x0;
    goto L_801F3B68;
L_801F3B48: ;
    r3 = r28;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x59;
    fn_801F54A4();
    /* clrlslwi r0, r30, 16, 2 */;
    r30 = r30 + 0x1;
    *(u32*)(r31 + r0) = r3;
L_801F3B68: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F3B48;
    r3 = r28;
    r6 = r29;
    r4 = r1 + 0x8;
    r5 = 0x8;
    fn_801F3BB4();
    r3 = r28;
    r7 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0x5a;
    r6 = 0x0;
    fn_801F4C14();
    /* lmw r28, 0x30(r1) */;
    return;
}
#pragma pop

/* 0x801F3BB4 | size: 0x134 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3BB4(void) {
    extern void fn_801F3CE8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0x8(r1) */;
    r29 = r5 & 0xFFFF;
    r23 = r3;
    r24 = r4;
    r25 = r6;
    /* subi r28, r29, 0x1 */;
    r27 = 0x0;
    goto L_801F3CC8;
L_801F3BE0: ;
    r0 = r3 + 0x1;
    /* clrlslwi r30, r27, 16, 2 */;
    r26 = r0 & 0xFFFF;
    goto L_801F3CB8;
L_801F3BF0: ;
    r3 = *(u32*)(r24 + r30);
    if ((u32)r3 != (u32)0x0) goto L_801F3C0C;
    /* clrlslwi r0, r26, 16, 2 */;
    r0 = *(u32*)(r24 + r0);
    if ((u32)r0 == (u32)0x0) goto L_801F3CB4;
L_801F3C0C: ;
    if ((u32)r3 != (u32)0x0) goto L_801F3C28;
    /* clrlslwi r4, r26, 16, 2 */;
    r0 = *(u32*)(r24 + r4);
    *(u32*)(r24 + r30) = r0;
    *(u32*)(r24 + r4) = r3;
    goto L_801F3CB4;
L_801F3C28: ;
    /* clrlslwi r31, r26, 16, 2 */;
    r0 = *(u32*)(r24 + r31);
    if ((u32)r0 == (u32)0x0) goto L_801F3CB4;
    r0 = r25 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_801F3C4C;
    r22 = 0x0;
    r3 = 0x0;
    goto L_801F3C60;
L_801F3C4C: ;
    ((void(*)(void))fn_802050F4)();
    r0 = r3;
    r3 = *(u32*)(r24 + r31);
    r22 = r0;
    ((void(*)(void))fn_802050F4)();
L_801F3C60: ;
    r4 = (s8)r22;
    r0 = (s8)r3;
    if ((s32)r4 < (s32)r0) goto L_801F3CB4;
    if ((s32)r4 <= (s32)r0) goto L_801F3C88;
    r3 = *(u32*)(r24 + r30);
    r0 = *(u32*)(r24 + r31);
    *(u32*)(r24 + r30) = r0;
    *(u32*)(r24 + r31) = r3;
    goto L_801F3CB4;
L_801F3C88: ;
    r4 = *(u32*)(r24 + r30);
    r3 = r23;
    r5 = *(u32*)(r24 + r31);
    r6 = r25;
    fn_801F3CE8();
    r0 = r3 & 0xFF;
    if ((s32)r4 != (s32)r0) goto L_801F3CB4;
    r3 = *(u32*)(r24 + r30);
    r0 = *(u32*)(r24 + r31);
    *(u32*)(r24 + r30) = r0;
    *(u32*)(r24 + r31) = r3;
L_801F3CB4: ;
    r26 = r26 + 0x1;
L_801F3CB8: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r29) goto L_801F3BF0;
    r27 = r27 + 0x1;
L_801F3CC8: ;
    r3 = r27 & 0xFFFF;
    if ((s32)r3 < (s32)r28) goto L_801F3BE0;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F3CE8 | size: 0x538 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F3CE8(void) {
    extern void fn_800E0C54();
    extern void fn_8011BEB4();
    extern void fn_8012640C();
    extern void fn_801F37B0();
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
    extern void fn_802043D4();
    extern void fn_802051D4();
    extern void fn_801F3678();
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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

    /* stmw r18, 0x28(r1) */;
    /* mr. r31, r4 */;
    r26 = r3;
    r30 = r5;
    r25 = r6;
    if ((s32)r0 == (s32)0) goto L_801F3D14;
    if ((u32)r30 != (u32)0x0) goto L_801F3D1C;
L_801F3D14: ;
    r3 = 0x1;
    goto L_801F420C;
L_801F3D1C: ;
    r0 = 0x0;
    r5 = 0xd;
    r4 = (u32)fn_801F3678;
    r4 = (u32)fn_801F3678;
    r5 = r1 + 0x18;
    *(u32*)(sp + 0x1C) = r0;
    r6 = 0x0;
    *(u32*)(sp + 0x20) = r0;
    *(u32*)(sp + 0x24) = r0;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0x1C);
    r0 = r0 & 0xFFFF;
    if ((u32)r30 == (u32)0x0) goto L_801F3D5C;
    r29 = 0x0;
    goto L_801F3EC4;
L_801F3D5C: ;
    r0 = 0x0;
    r4 = 0x4d;
    r3 = (u32)fn_801F3678;
    r4 = (u32)fn_801F3678;
    r5 = r1 + 0x8;
    *(u32*)(sp + 0xC) = r0;
    r3 = r26;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    *(u32*)(sp + 0x14) = r0;
    fn_801F37B0();
    r0 = *(u32*)(sp + 0xC);
    r0 = r0 & 0xFFFF;
    if ((u32)r30 == (u32)0x0) goto L_801F3DA0;
    r29 = 0x0;
    goto L_801F3EC4;
L_801F3DA0: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x4e;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3DC4;
    r29 = 0x0;
    goto L_801F3EC4;
L_801F3DC4: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x4f;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3DE8;
    r29 = 0x1;
    goto L_801F3EC4;
L_801F3DE8: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x50;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3E0C;
    r29 = 0x2;
    goto L_801F3EC4;
L_801F3E0C: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x51;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3E30;
    r29 = 0x3;
    goto L_801F3EC4;
L_801F3E30: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x52;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3E54;
    r29 = 0x4;
    goto L_801F3EC4;
L_801F3E54: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x53;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3E78;
    r29 = 0x1;
    goto L_801F3EC4;
L_801F3E78: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x54;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3E9C;
    r29 = 0x2;
    goto L_801F3EC4;
L_801F3E9C: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xa;
    r6 = 0x55;
    fn_801F54A4();
    if ((s32)r3 != (s32)0x1) goto L_801F3EC0;
    r29 = 0x3;
    goto L_801F3EC4;
L_801F3EC0: ;
    r29 = 0x0;
L_801F3EC4: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0x1d;
    r6 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFF;
    r3 = r26;
    r4 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    fn_8012640C();
    /* mr. r23, r3 */;
    if ((s32)r3 != (s32)0x1) goto L_801F3F18;
    r24 = 0x0;
    goto L_801F3FD0;
L_801F3F18: ;
    r22 = 0x0;
    goto L_801F3FB4;
L_801F3F20: ;
    r3 = r26;
    r6 = r22;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    /* mr. r19, r3 */;
    if ((s32)r3 == (s32)0x1) goto L_801F3FB0;
    r20 = 0x0;
    goto L_801F3FA4;
L_801F3F44: ;
    r3 = r19;
    r6 = r20;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    /* mr. r24, r3 */;
    if ((s32)r3 == (s32)0x1) goto L_801F3FA0;
    r21 = 0x0;
    goto L_801F3F94;
L_801F3F68: ;
    r3 = r24;
    r6 = r21;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    if ((u32)r3 == (u32)0x0) goto L_801F3F90;
    if ((u32)r23 != (u32)r3) goto L_801F3F90;
    goto L_801F3FC4;
L_801F3F90: ;
    r21 = r21 + 0x1;
L_801F3F94: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F3F68;
L_801F3FA0: ;
    r20 = r20 + 0x1;
L_801F3FA4: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F3F44;
L_801F3FB0: ;
    r22 = r22 + 0x1;
L_801F3FB4: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F3F20;
    r24 = 0x0;
L_801F3FC4: ;
    if ((u32)r24 != (u32)0x0) goto L_801F3FD0;
    r24 = 0x0;
L_801F3FD0: ;
    if ((u32)r24 != (u32)0x0) goto L_801F3FE0;
    r7 = 0x0;
    goto L_801F4008;
L_801F3FE0: ;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r3 != (u32)0x0) goto L_801F4004;
    r7 = 0x0;
    goto L_801F4008;
L_801F4004: ;
    r7 = r3;
L_801F4008: ;
    r3 = r31;
    r4 = r27;
    r5 = r29;
    r6 = r28;
    fn_802043D4();
    r24 = r3;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    fn_8012640C();
    /* mr. r23, r3 */;
    if ((u32)r3 != (u32)0x0) goto L_801F4044;
    r19 = 0x0;
    goto L_801F40FC;
L_801F4044: ;
    r20 = 0x0;
    goto L_801F40E0;
L_801F404C: ;
    r3 = r26;
    r6 = r20;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    /* mr. r18, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_801F40DC;
    r22 = 0x0;
    goto L_801F40D0;
L_801F4070: ;
    r3 = r18;
    r6 = r22;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    /* mr. r19, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_801F40CC;
    r21 = 0x0;
    goto L_801F40C0;
L_801F4094: ;
    r3 = r19;
    r6 = r21;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    if ((u32)r3 == (u32)0x0) goto L_801F40BC;
    if ((u32)r23 != (u32)r3) goto L_801F40BC;
    goto L_801F40F0;
L_801F40BC: ;
    r21 = r21 + 0x1;
L_801F40C0: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F4094;
L_801F40CC: ;
    r22 = r22 + 0x1;
L_801F40D0: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F4070;
L_801F40DC: ;
    r20 = r20 + 0x1;
L_801F40E0: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F404C;
    r19 = 0x0;
L_801F40F0: ;
    if ((u32)r19 != (u32)0x0) goto L_801F40FC;
    r19 = 0x0;
L_801F40FC: ;
    if ((u32)r19 != (u32)0x0) goto L_801F410C;
    r7 = 0x0;
    goto L_801F4134;
L_801F410C: ;
    r3 = r19;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r3 != (u32)0x0) goto L_801F4130;
    r7 = 0x0;
    goto L_801F4134;
L_801F4130: ;
    r7 = r3;
L_801F4134: ;
    r3 = r30;
    r4 = r27;
    r5 = r29;
    r6 = r28;
    fn_802043D4();
    r0 = r25 & 0xFF;
    r25 = r3;
    if ((u32)r3 != (u32)0x0) goto L_801F4160;
    r18 = 0x0;
    r19 = 0x0;
    goto L_801F417C;
L_801F4160: ;
    r3 = r31;
    fn_802051D4();
    r0 = r3;
    r3 = r30;
    r18 = r0;
    fn_802051D4();
    r19 = r3;
L_801F417C: ;
    r4 = r18;
    r3 = 0x0;
    r5 = 0x4;
    r6 = 0x0;
    fn_8011BEB4();
    r18 = (s8)r3;
    r4 = r19;
    r3 = 0x0;
    r5 = 0x4;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = (s8)r18;
    r3 = (s8)r3;
    if ((u32)r3 != (u32)0x0) goto L_801F41BC;
    r0 = (s8)r3;
    if ((u32)r3 == (u32)0x0) goto L_801F41D8;
L_801F41BC: ;
    if ((s32)r18 <= (s32)r3) goto L_801F41CC;
    r3 = 0x1;
    goto L_801F420C;
L_801F41CC: ;
    if ((s32)r18 >= (s32)r3) goto L_801F41D8;
    r3 = 0x0;
    goto L_801F420C;
L_801F41D8: ;
    if ((u32)r24 <= (u32)r25) goto L_801F41E8;
    r3 = 0x1;
    goto L_801F420C;
L_801F41E8: ;
    if ((u32)r24 >= (u32)r25) goto L_801F41F4;
    r3 = 0x0;
    goto L_801F420C;
L_801F41F4: ;
    fn_800E0C54();
    r0 = r3 & 0x1;
    if ((u32)r24 == (u32)r25) goto L_801F4208;
    r3 = 0x1;
    goto L_801F420C;
L_801F4208: ;
    r3 = 0x0;
L_801F420C: ;
    /* lmw r18, 0x28(r1) */;
    return;
}
#pragma pop

/* 0x801F4220 | size: 0x134 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4220(void) {
    extern void fn_8012640C();
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0xd5;
    r6 = 0x0;
    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r3 = r4;
    r4 = 0x0;
    fn_8012640C();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F4258;
    r30 = 0x0;
    goto L_801F4310;
L_801F4258: ;
    r29 = 0x0;
    goto L_801F42F4;
L_801F4260: ;
    r3 = r25;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    /* mr. r26, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F42F0;
    r27 = 0x0;
    goto L_801F42E4;
L_801F4284: ;
    r3 = r26;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F42E0;
    r28 = 0x0;
    goto L_801F42D4;
L_801F42A8: ;
    r3 = r30;
    r6 = r28;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    if ((u32)r3 == (u32)0x0) goto L_801F42D0;
    if ((u32)r31 != (u32)r3) goto L_801F42D0;
    goto L_801F4304;
L_801F42D0: ;
    r28 = r28 + 0x1;
L_801F42D4: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F42A8;
L_801F42E0: ;
    r27 = r27 + 0x1;
L_801F42E4: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F4284;
L_801F42F0: ;
    r29 = r29 + 0x1;
L_801F42F4: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F4260;
    r30 = 0x0;
L_801F4304: ;
    if ((u32)r30 != (u32)0x0) goto L_801F4310;
    r30 = 0x0;
L_801F4310: ;
    if ((u32)r30 != (u32)0x0) goto L_801F4320;
    r3 = 0x0;
    goto L_801F4340;
L_801F4320: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r3 != (u32)0x0) goto L_801F4340;
    r3 = 0x0;
L_801F4340: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F4354 | size: 0x10C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4354(void) {
    extern void fn_8012640C();
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0xd5;
    r6 = 0x0;
    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r3 = r4;
    r4 = 0x0;
    fn_8012640C();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F438C;
    r3 = 0x0;
    goto L_801F444C;
L_801F438C: ;
    r29 = 0x0;
    goto L_801F4428;
L_801F4394: ;
    r3 = r25;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    /* mr. r26, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F4424;
    r27 = 0x0;
    goto L_801F4418;
L_801F43B8: ;
    r3 = r26;
    r6 = r27;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F4414;
    r28 = 0x0;
    goto L_801F4408;
L_801F43DC: ;
    r3 = r30;
    r6 = r28;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    if ((u32)r3 == (u32)0x0) goto L_801F4404;
    if ((u32)r31 != (u32)r3) goto L_801F4404;
    goto L_801F4438;
L_801F4404: ;
    r28 = r28 + 0x1;
L_801F4408: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F43DC;
L_801F4414: ;
    r27 = r27 + 0x1;
L_801F4418: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F43B8;
L_801F4424: ;
    r29 = r29 + 0x1;
L_801F4428: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F4394;
    r30 = 0x0;
L_801F4438: ;
    if ((u32)r30 != (u32)0x0) goto L_801F4448;
    r3 = 0x0;
    goto L_801F444C;
L_801F4448: ;
    r3 = r30;
L_801F444C: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F4460 | size: 0xDC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4460(void) {
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FB1C0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r26 = r4;
    r28 = 0x0;
    goto L_801F4518;
L_801F4480: ;
    r3 = r25;
    r6 = r28;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F4514;
    r30 = 0x0;
    goto L_801F4508;
L_801F44A4: ;
    r3 = r31;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    /* mr. r27, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F4504;
    r29 = 0x0;
    goto L_801F44F8;
L_801F44C8: ;
    r3 = r27;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    if ((u32)r3 == (u32)0x0) goto L_801F44F4;
    if ((u32)r26 != (u32)r3) goto L_801F44F4;
    r3 = r27;
    goto L_801F4528;
L_801F44F4: ;
    r29 = r29 + 0x1;
L_801F44F8: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F44C8;
L_801F4504: ;
    r30 = r30 + 0x1;
L_801F4508: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F44A4;
L_801F4514: ;
    r28 = r28 + 0x1;
L_801F4518: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F4480;
    r3 = 0x0;
L_801F4528: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F4718 | size: 0x9C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4718(void) {
    extern void fn_801F54A4();
    extern void fn_801F61EC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0x5a;
    r6 = 0x0;
    r31 = r3;
    fn_801F54A4();
    if ((u32)r3 == (u32)0x0) goto L_801F4768;
    r5 = 0x0;
    r4 = r5;
    goto L_801F475C;
L_801F4750: ;
    /* clrlslwi r0, r5, 16, 2 */;
    r5 = r5 + 0x1;
    *(u32*)(r3 + r0) = r4;
L_801F475C: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F4750;
L_801F4768: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x5a;
    r6 = 0x0;
    fn_801F54A4();
    /* mr. r4, r3 */;
    if ((u32)r0 != (u32)0x8) goto L_801F478C;
    r3 = 0x0;
    goto L_801F47A0;
L_801F478C: ;
    r3 = r31;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F61EC();
L_801F47A0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F47B4 | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F47B4(void) {
    extern void fn_801F54A4();
    extern void fn_801F7404();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r6 = r4;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r31 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_801F47EC;
    r3 = 0x0;
    goto L_801F47F0;
L_801F47EC: ;
    r3 = r31;
L_801F47F0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F4804 | size: 0x5C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4804(void) {
    extern void fn_801F4C14();
    extern void fn_801F54A4();
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
    r5 = 0x58;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    fn_801F54A4();
    r31 = (s16)r3;
    r3 = r30;
    r0 = r31 + 0x1;
    r4 = 0x0;
    r7 = (s16)r0;
    r5 = 0x58;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r31;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F4860 | size: 0x260 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4860(void) {
    extern u8 lbl_80279C28[];
    extern void fn_8011B950();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_801F7530();
    u8 sp[0x40];
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r28, 0x30(r1) */;
    /* mr. r30, r3 */;
    r31 = r4;
    if ((s32)r0 == (s32)0) goto L_801F4AAC;
    r4 = 0x0;
    r5 = 0x9;
    r6 = 0x0;
    fn_801F54A4();
    r4 = 0x1;
    fn_8011B950();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xc;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x35;
    r6 = 0x0;
    fn_801F54A4();
    r4 = 0x2;
    fn_801F7530();
    r3 = (u32)lbl_80279C28;
    r0 = 0x3;
    r3 = (u32)lbl_80279C28;
    r5 = r1 + 0x4;
    /* subi r4, r3, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_801F48F8: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_801F48F8;
    r0 = *(u16*)((u8*)r4 + 0x4);
    r29 = r1 + 0x8;
    r28 = 0x0;
    *(u16*)((u8*)r5 + 0x4) = r0;
    goto L_801F4940;
L_801F4920: ;
    /* clrlslwi r0, r28, 16, 1 */;
    r3 = r30;
    r5 = *(u16*)(r29 + r0);
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r28 = r28 + 0x1;
L_801F4940: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0xd) goto L_801F4920;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x50;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x51;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x52;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x53;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x54;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x55;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x56;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x57;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x58;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0x5a;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r3 == (u32)0x0) goto L_801F4A64;
    r5 = 0x0;
    r4 = r5;
    goto L_801F4A58;
L_801F4A4C: ;
    /* clrlslwi r0, r5, 16, 2 */;
    r5 = r5 + 0x1;
    *(u32*)(r3 + r0) = r4;
L_801F4A58: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F4A4C;
L_801F4A64: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r7 = r31 & 0xFFFF;
    r4 = 0x0;
    r5 = 0xd;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xc;
    r6 = 0x0;
    r7 = 0x1;
    fn_801F4C14();
L_801F4AAC: ;
    /* lmw r28, 0x30(r1) */;
    return;
}
#pragma pop

/* 0x801F4AC0 | size: 0x154 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4AC0(void) {
    extern u8 lbl_80279C28[];
    extern void fn_801F4C14();
    u8 sp[0x40];
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)lbl_80279C28;
    r4 = (u32)lbl_80279C28;
    r0 = 0x3;
    r5 = r1 + 0x4;
    /* stmw r29, 0x34(r1) */;
    r29 = r3;
    /* subi r4, r4, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_801F4AEC: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_801F4AEC;
    r0 = *(u16*)((u8*)r4 + 0x4);
    r31 = r1 + 0x8;
    r30 = 0x0;
    *(u16*)((u8*)r5 + 0x4) = r0;
    goto L_801F4B34;
L_801F4B14: ;
    /* clrlslwi r0, r30, 16, 1 */;
    r3 = r29;
    r5 = *(u16*)(r31 + r0);
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r30 = r30 + 0x1;
L_801F4B34: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0xd) goto L_801F4B14;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x50;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x51;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x52;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x53;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x54;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x55;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x56;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x57;
    r6 = 0x0;
    r7 = 0x0;
    fn_801F4C14();
    /* lmw r29, 0x34(r1) */;
    return;
}
#pragma pop

/* 0x801F4C14 | size: 0x890 | massive */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F4C14(void) {
    extern void fn_800FA280();
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B444();
    extern void fn_8011CB54();
    extern void fn_8011CB6C();
    extern void fn_80132A38();
    extern void fn_80142CF4();
    extern void fn_801F0134();
    extern void fn_801F3984();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_801F640C();
    extern void fn_801F641C();
    extern void fn_801F644C();
    extern void fn_801F647C();
    extern void fn_801F64AC();
    extern void fn_801F64DC();
    extern void fn_801F650C();
    extern void fn_801F6560();
    extern void fn_801F65C0();
    extern void fn_801F65F0();
    extern void fn_801F6618();
    extern void fn_801F6628();
    extern void fn_801F6638();
    extern void fn_801F6648();
    extern void fn_801F6658();
    extern void fn_801F667C();
    extern void fn_801F6764();
    extern void fn_801F6778();
    extern void fn_801F678C();
    extern void fn_801F67A0();
    extern void fn_801F67B4();
    extern void fn_801F67C8();
    extern void fn_801F67DC();
    extern void fn_801F67F0();
    extern void fn_801F6804();
    extern void fn_801F6818();
    extern void fn_801F682C();
    extern void fn_801F6840();
    extern void fn_801F6854();
    extern void fn_801F6868();
    extern void fn_801F687C();
    extern void fn_801F6890();
    extern void fn_801F68A4();
    extern void fn_801F68B8();
    extern void fn_801F68C8();
    extern void fn_801F6A98();
    extern void fn_802040E8();
    extern void fn_80206780();
    extern void fn_80207BF4();
    extern u8 jumptable_80375330[];
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
    void (*ctr_fn)(void) = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r29 = r3;
    r30 = r7;
    r31 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r26 & 0xFFFF;
    r28 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_801F4C60;
    if ((u32)r0 < (u32)0x60) goto L_801F4C68;
L_801F4C60: ;
    r3 = 0x0;
    goto L_801F5490;
L_801F4C68: ;
    if ((u32)r0 >= (u32)0x8) goto L_801F4C88;
    r3 = r25;
    ((void(*)(void))fn_801F6738)();
    /* mr. r29, r3 */;
    if ((u32)r0 != (u32)0x8) goto L_801F4CAC;
    r3 = 0x0;
    goto L_801F5490;
L_801F4C88: ;
    if ((u32)r0 >= (u32)0x5f) goto L_801F4CAC;
    if ((u32)r29 != (u32)0x0) goto L_801F4CAC;
    ((void(*)(void))fn_801F6B48)();
    /* mr. r29, r3 */;
    if ((u32)r29 != (u32)0x0) goto L_801F4CAC;
    r3 = 0x0;
    goto L_801F5490;
L_801F4CAC: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 > (u32)0x5e) goto L_801F548C;
    r3 = (u32)jumptable_80375330;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80375330;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = r29;
    r4 = r30;
    fn_801F65F0();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F667C();
    goto L_801F548C;
    r3 = r29;
    r4 = r27 & 0xFF;
    r5 = r30 & 0xFFFF;
    fn_801F6658();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFF;
    fn_801F6648();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F6638();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F6628();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F6618();
    goto L_801F548C;
    r3 = r30 & 0xFFFF;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x9) goto L_801F4D60;
    r3 = 0x0;
    goto L_801F4D6C;
L_801F4D60: ;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_8011B444();
L_801F4D6C: ;
    r0 = r3 & 0xFF;
    r31 = r3;
    if ((u32)r0 != (u32)0x2) goto L_801F548C;
    r3 = r30 & 0xFFFF;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x9) goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = r27 & 0xFFFF;
    fn_8011B2C0();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F68C8();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F68B8();
    goto L_801F548C;
    if ((u32)r30 == (u32)0x0) goto L_801F4E78;
    r3 = r30;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F4E38;
    r4 = r30;
    r3 = 0xf;
    fn_80132A38();
    r3 = r30;
    fn_80207BF4();
    r3 = r3 & 0xFFFF;
    fn_8011CB6C();
    fn_8011CB54();
    fn_800FA280();
    r4 = r3;
    r3 = 0x1a;
    fn_80132A38();
    r4 = r30;
    r3 = 0x1f;
    fn_80132A38();
    r4 = r30;
    r3 = 0x21;
    fn_80132A38();
    r4 = r30;
    r3 = 0x20;
    fn_80132A38();
    goto L_801F4EB4;
L_801F4E38: ;
    r3 = 0xf;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1a;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1f;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x21;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x20;
    r4 = 0x0;
    fn_80132A38();
    goto L_801F4EB4;
L_801F4E78: ;
    r3 = 0xf;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1a;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1f;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x21;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x20;
    r4 = 0x0;
    fn_80132A38();
L_801F4EB4: ;
    r3 = r29;
    r4 = r30;
    fn_801F68A4();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xda;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdb;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdc;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdd;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xde;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdf;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe0;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe1;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe2;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe3;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe4;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    goto L_801F548C;
    if ((u32)r30 == (u32)0x0) goto L_801F50D8;
    r3 = r30;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F5098;
    r4 = r30;
    r3 = 0x10;
    fn_80132A38();
    r3 = r30;
    fn_80207BF4();
    r3 = r3 & 0xFFFF;
    fn_8011CB6C();
    fn_8011CB54();
    fn_800FA280();
    r4 = r3;
    r3 = 0x1b;
    fn_80132A38();
    r4 = r30;
    r3 = 0x42;
    fn_80132A38();
    r4 = r30;
    r3 = 0x44;
    fn_80132A38();
    r4 = r30;
    r3 = 0x43;
    fn_80132A38();
    goto L_801F5114;
L_801F5098: ;
    r3 = 0x10;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1b;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x42;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x44;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x43;
    r4 = 0x0;
    fn_80132A38();
    goto L_801F5114;
L_801F50D8: ;
    r3 = 0x10;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1b;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x42;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x44;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x43;
    r4 = 0x0;
    fn_80132A38();
L_801F5114: ;
    r3 = r29;
    r4 = r30;
    fn_801F6890();
    goto L_801F548C;
    r3 = r29;
    r7 = r30;
    r4 = 0x0;
    r5 = 0x42;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r30;
    r4 = r28;
    fn_801F0134();
    r0 = r3;
    r3 = r29;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x40;
    r6 = 0x0;
    fn_801F4C14();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F687C();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F6868();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F64AC();
    goto L_801F548C;
    if ((u32)r30 == (u32)0x0) goto L_801F5204;
    r3 = r30;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F51E8;
    r4 = r30;
    r3 = 0x12;
    fn_80132A38();
    r3 = r30;
    fn_80207BF4();
    r3 = r3 & 0xFFFF;
    fn_8011CB6C();
    fn_8011CB54();
    fn_800FA280();
    r4 = r3;
    r3 = 0x1d;
    fn_80132A38();
    goto L_801F521C;
L_801F51E8: ;
    r3 = 0x12;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1d;
    r4 = 0x0;
    fn_80132A38();
    goto L_801F521C;
L_801F5204: ;
    r3 = 0x12;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1d;
    r4 = 0x0;
    fn_80132A38();
L_801F521C: ;
    r3 = r29;
    r4 = r30;
    fn_801F647C();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F644C();
    goto L_801F548C;
    if ((u32)r30 == (u32)0x0) goto L_801F527C;
    r3 = r30;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F527C;
    r3 = r30;
    fn_802040E8();
    r0 = r3;
    r3 = r29;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x56;
    r6 = 0x0;
    fn_801F4C14();
L_801F527C: ;
    r3 = r29;
    r4 = r30;
    fn_801F641C();
    goto L_801F548C;
    r3 = r30;
    fn_801F640C();
    goto L_801F548C;
    if ((u32)r30 == (u32)0x0) goto L_801F5304;
    r3 = r30;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F52E8;
    r4 = r30;
    r3 = 0x1e;
    fn_80132A38();
    r3 = r30;
    fn_80207BF4();
    r3 = r3 & 0xFFFF;
    fn_8011CB6C();
    fn_8011CB54();
    fn_800FA280();
    r4 = r3;
    r3 = 0x1c;
    fn_80132A38();
    goto L_801F531C;
L_801F52E8: ;
    r3 = 0x1e;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1c;
    r4 = 0x0;
    fn_80132A38();
    goto L_801F531C;
L_801F5304: ;
    r3 = 0x1e;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x1c;
    r4 = 0x0;
    fn_80132A38();
L_801F531C: ;
    r3 = r29;
    r4 = r30;
    fn_801F6854();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F6840();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F682C();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F6818();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F6804();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F67F0();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F67DC();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F67C8();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F67B4();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F67A0();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F678C();
    goto L_801F548C;
    r4 = r30 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_80142CF4();
    fn_800FA280();
    r4 = r3;
    r3 = 0x29;
    fn_80132A38();
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F6778();
    goto L_801F548C;
    r3 = r30 & 0xFFFF;
    fn_8011CB6C();
    fn_8011CB54();
    fn_800FA280();
    r4 = r3;
    r3 = 0x1c;
    fn_80132A38();
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F6764();
    goto L_801F548C;
    r3 = r29;
    r4 = (s16)r30;
    fn_801F65C0();
    goto L_801F548C;
    r3 = r29;
    r5 = r30;
    r4 = r27 & 0xFFFF;
    fn_801F6560();
    goto L_801F548C;
    r3 = r29;
    r4 = r30;
    fn_801F650C();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F64DC();
    goto L_801F548C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F3984();
    goto L_801F548C;
    r4 = r30;
    r3 = 0x2f;
    fn_80132A38();
L_801F548C: ;
    r3 = r31 & 0xFF;
L_801F5490: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F61EC | size: 0x220 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F61EC(void) {
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7404();
    extern void fn_801F981C();
    extern void fn_802062FC();
    extern void fn_80206780();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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

    r8 = 0x0;
    /* stmw r17, 0x14(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r3 = r8;
    goto L_801F6228;
L_801F621C: ;
    /* clrlslwi r0, r8, 16, 2 */;
    r8 = r8 + 0x1;
    *(u32*)(r28 + r0) = r3;
L_801F6228: ;
    r0 = r8 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F621C;
    r3 = r27;
    r18 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r17 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r20 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r30 & 0xFF;
    r19 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x8) goto L_801F62B8;
    r3 = r31;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x8) goto L_801F62A0;
    r3 = 0x0;
    goto L_801F63F8;
L_801F62A0: ;
    r4 = r31;
    r5 = r17;
    r3 = 0x2;
    fn_801F02AC();
    r24 = r3;
    goto L_801F62BC;
L_801F62B8: ;
    r24 = 0x0;
L_801F62BC: ;
    r21 = 0x0;
    goto L_801F63E8;
L_801F62C4: ;
    r3 = r27;
    r6 = r21;
    r4 = 0x0;
    r5 = 0x35;
    fn_801F54A4();
    r25 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x8) goto L_801F62EC;
    r25 = 0x0;
L_801F62EC: ;
    if ((u32)r25 == (u32)0x0) goto L_801F63E4;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F630C;
    if ((u32)r24 != (u32)r25) goto L_801F63E4;
    goto L_801F631C;
L_801F630C: ;
    if ((u32)r0 != (u32)0x2) goto L_801F631C;
    if ((u32)r24 == (u32)r25) goto L_801F63E4;
L_801F631C: ;
    r23 = 0x0;
    goto L_801F63D8;
L_801F6324: ;
    r3 = r25;
    r4 = r23;
    fn_801F7258();
    /* mr. r17, r3 */;
    if ((u32)r24 == (u32)r25) goto L_801F63D4;
    r22 = 0x0;
    goto L_801F63C8;
L_801F6340: ;
    r3 = r17;
    r4 = r22;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r24 == (u32)r25) goto L_801F63C4;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F636C;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_801F63C4;
L_801F636C: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x3) goto L_801F6380;
    if ((u32)r26 == (u32)r31) goto L_801F63C4;
L_801F6380: ;
    r4 = 0x0;
    goto L_801F63A8;
L_801F6388: ;
    /* clrlslwi r3, r4, 16, 2 */;
    r0 = *(u32*)(r28 + r3);
    if ((u32)r0 != (u32)0x0) goto L_801F63A4;
    *(u32*)(r28 + r3) = r26;
    r0 = (s16)r4;
    goto L_801F63B8;
L_801F63A4: ;
    r4 = r4 + 0x1;
L_801F63A8: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F6388;
    r0 = -0x1;
L_801F63B8: ;
    r0 = (s16)r0;
    if ((u32)r0 < (u32)0x8) goto L_801F63C4;
    r18 = r18 + 0x1;
L_801F63C4: ;
    r22 = r22 + 0x1;
L_801F63C8: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r19) goto L_801F6340;
L_801F63D4: ;
    r23 = r23 + 0x1;
L_801F63D8: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r20) goto L_801F6324;
L_801F63E4: ;
    r21 = r21 + 0x1;
L_801F63E8: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F62C4;
    r3 = r18;
L_801F63F8: ;
    /* lmw r17, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F650C | size: 0x38 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F650C(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    if ((u32)r3 == (u32)0x0) return;
    r7 = 0x0;
    goto L_801F6534;
L_801F651C: ;
    /* clrlslwi r6, r7, 16, 2 */;
    r7 = r7 + 0x1;
    r5 = r6 + (0x1 << 16);
    r0 = *(u32*)(r4 + r6);
    /* subi r5, r5, 0x5b3c */;
    *(u32*)(r3 + r5) = r0;
L_801F6534: ;
    r0 = r7 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_801F651C;
    return;
}
#pragma pop

/* 0x801F6544 | size: 0x1C */
u8* fn_801F6544(u8* ptr) {
    if (!ptr) { return NULL; }
    return ptr + 0xA4C4;
}

/* 0x801F6560 | size: 0x28 */
void fn_801F6560(u8* ptr, u16 idx, u32 val) {
    if (!ptr) { return; }
    if ((u16)idx >= 8) { return; }
    *(u32*)(ptr + 0xA4C4 + (u16)idx * 4) = val;
}

/* 0x801F6588 | size: 0x38 */
u32 fn_801F6588(u8* ptr, u16 idx) {
    if (!ptr) { return 0; }
    if ((u16)idx >= 8) { return 0; }
    ptr += (u16)idx * 4;
    return *(u32*)((u8*)ptr + 0xA4C4);
}

/* 0x801F6658 | size: 0x24 */
void fn_801F6658(u8* ptr, u8 idx, u16 val) {
    if (!ptr) { return; }
    if ((u8)idx >= 2) { return; }
    ptr += (u8)idx * 2;
    *(u16*)(ptr + 0x14) = val;
}

/* 0x801F66EC | size: 0x34 */
u32 fn_801F66EC(u8* ptr, u8 idx) {
    if (!ptr) { return 0; }
    if ((u8)idx >= 2) { return 0; }
    ptr += (u8)idx * 2;
    return *(u16*)(ptr + 0x14);
}

/* 0x801F6AB4 | size: 0x34 */
u8* fn_801F6AB4(u8* ptr, u16 idx) {
    if (!ptr) { return NULL; }
    if ((u16)idx >= 2) { return NULL; }
    return ptr + (u16)idx * 0x5230 + 0x14;
}

/* 0x801F6B18 | size: 0x30 */
u8* fn_801F6B18(u8* ptr, u16 idx) {
    if (!ptr) { return NULL; }
    if ((u16)idx >= 1) { return NULL; }
    return ptr + (u16)idx * 0x10;
}

/* 0x801F6B54 | size: 0xF8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6B54(void) {
    extern void fn_8012640C();
    extern void fn_801F78D4();
    extern void fn_801FA634();
    extern void fn_801FAA58();
    extern void fn_801FB1C0();
    extern void fn_802062FC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r23, 0xc(r1) */;
    r23 = r3;
    r24 = r4;
    r25 = r7;
    r31 = r6 & 0xFFFF;
    r27 = 0x0;
    goto L_801F6C2C;
L_801F6B7C: ;
    if ((u32)r23 != (u32)0x0) goto L_801F6B8C;
    r3 = 0x0;
    goto L_801F6B98;
L_801F6B8C: ;
    r3 = r23;
    r4 = r27;
    fn_801F78D4();
L_801F6B98: ;
    r29 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((u32)r23 == (u32)0x0) goto L_801F6C28;
    r30 = r25 & 0xFFFF;
    r26 = 0x0;
    goto L_801F6C1C;
L_801F6BB4: ;
    r3 = r29;
    r6 = r26;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    r28 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r23 == (u32)0x0) goto L_801F6C18;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    fn_8012640C();
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    fn_8012640C();
    r7 = (s16)r3;
    if ((u32)r23 < (u32)0x0) goto L_801F6C18;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x57;
    r6 = 0x0;
    fn_801FAA58();
L_801F6C18: ;
    r26 = r26 + 0x1;
L_801F6C1C: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_801F6BB4;
L_801F6C28: ;
    r27 = r27 + 0x1;
L_801F6C2C: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_801F6B7C;
    /* lmw r23, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x801F6C4C | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6C4C(void) {
    extern void fn_80119ED0();
    extern void fn_8011A860();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x6) goto L_801F6C80;
    r3 = 0x0;
    goto L_801F6C8C;
L_801F6C80: ;
    r3 = r30;
    r4 = r31;
    fn_8011A860();
L_801F6C8C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F6CA0 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6CA0(void) {
    extern void fn_80119ED0();
    extern void fn_8011AB50();
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
    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x6) goto L_801F6CE0;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8011AB50();
L_801F6CE0: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F6CF4 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6CF4(void) {
    extern void fn_80119ED0();
    extern void fn_8011ACB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x6) goto L_801F6D28;
    r3 = -0x1;
    goto L_801F6D34;
L_801F6D28: ;
    r3 = r30;
    r4 = r31;
    fn_8011ACB4();
L_801F6D34: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F6D48 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6D48(void) {
    extern void fn_80119ED0();
    extern void fn_8011AE40();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x6) goto L_801F6D7C;
    r3 = -0x1;
    goto L_801F6D88;
L_801F6D7C: ;
    r3 = r30;
    r4 = r31;
    fn_8011AE40();
L_801F6D88: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F6D9C | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6D9C(void) {
    extern void fn_80119ED0();
    extern void fn_8011B130();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x6) goto L_801F6DD0;
    r3 = -0x1;
    goto L_801F6DDC;
L_801F6DD0: ;
    r3 = r30;
    r4 = r31;
    fn_8011B130();
L_801F6DDC: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F6DF0 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6DF0(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
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
    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x6) goto L_801F6E30;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8011B2C0();
L_801F6E30: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F6E44 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6E44(void) {
    extern void fn_80119ED0();
    extern void fn_8011B444();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x6) goto L_801F6E78;
    r3 = 0x0;
    goto L_801F6E84;
L_801F6E78: ;
    r3 = r30;
    r4 = r31;
    fn_8011B444();
L_801F6E84: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F6E98 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6E98(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x6) goto L_801F6ECC;
    r3 = 0x0;
    goto L_801F6ED8;
L_801F6ECC: ;
    r3 = r30;
    r4 = r31;
    fn_8011B67C();
L_801F6ED8: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F6EEC | size: 0x4C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6EEC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B788();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x6) goto L_801F6F24;
    r3 = r30;
    r4 = r31;
    fn_8011B788();
L_801F6F24: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F6F38 | size: 0x9C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6F38(void) {
    extern void fn_801F78D4();
    extern void fn_801F81F8();
    extern void fn_801FA634();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r26 = r5;
    r27 = r6;
    r31 = r4 & 0xFFFF;
    r28 = 0x0;
    r29 = 0x0;
    goto L_801F6FB0;
L_801F6F64: ;
    if ((u32)r25 != (u32)0x0) goto L_801F6F74;
    r30 = 0x0;
    goto L_801F6F84;
L_801F6F74: ;
    r3 = r25;
    r4 = r29;
    fn_801F78D4();
    r30 = r3;
L_801F6F84: ;
    r3 = r30;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((u32)r25 == (u32)0x0) goto L_801F6FAC;
    r3 = r30;
    r4 = r26;
    r5 = r27;
    fn_801F81F8();
    r0 = r28 + r3;
    r28 = r0 & 0xFFFF;
L_801F6FAC: ;
    r29 = r29 + 0x1;
L_801F6FB0: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_801F6F64;
    r3 = r28;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F6FD4 | size: 0xBC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F6FD4(void) {
    extern void fn_801F78D4();
    extern void fn_801F986C();
    extern void fn_801FA634();
    extern void fn_80206608();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r24, 0x10(r1) */;
    r29 = 0x0;
    r24 = r3;
    r25 = r5;
    r31 = r4 & 0xFFFF;
    r27 = r29;
    goto L_801F706C;
L_801F6FFC: ;
    if ((u32)r24 != (u32)0x0) goto L_801F700C;
    r3 = 0x0;
    goto L_801F7018;
L_801F700C: ;
    r3 = r24;
    r4 = r27;
    fn_801F78D4();
L_801F7018: ;
    r26 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((u32)r24 == (u32)0x0) goto L_801F7068;
    r30 = r25 & 0xFFFF;
    r28 = 0x0;
    goto L_801F705C;
L_801F7034: ;
    r3 = r26;
    r4 = r28;
    fn_801F986C();
    if ((u32)r3 == (u32)0x0) goto L_801F7058;
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_801F7058;
    r29 = r29 + 0x1;
L_801F7058: ;
    r28 = r28 + 0x1;
L_801F705C: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_801F7034;
L_801F7068: ;
    r27 = r27 + 0x1;
L_801F706C: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_801F6FFC;
    r3 = r29 & 0xFFFF;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F7090 | size: 0xE4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7090(void) {
    extern void fn_8012640C();
    extern void fn_801F78D4();
    extern void fn_801F986C();
    extern void fn_801FA634();
    extern void fn_80205BE8();
    extern void fn_80206608();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0x8(r1) */;
    r29 = r3;
    r30 = r5;
    r28 = r4 & 0xFFFF;
    r31 = 0x0;
    r23 = 0x0;
    goto L_801F7150;
L_801F70B8: ;
    if ((u32)r29 != (u32)0x0) goto L_801F70C8;
    r3 = 0x0;
    goto L_801F70D4;
L_801F70C8: ;
    r3 = r29;
    r4 = r23;
    fn_801F78D4();
L_801F70D4: ;
    r26 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_801F714C;
    r27 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_801F7140;
L_801F70F0: ;
    r3 = r26;
    r4 = r22;
    fn_801F986C();
    /* mr. r25, r3 */;
    if ((u32)r29 == (u32)0x0) goto L_801F713C;
    fn_80205BE8();
    /* mr. r24, r3 */;
    if ((u32)r29 == (u32)0x0) goto L_801F713C;
    r3 = r25;
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_801F713C;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x87;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r31 = r31 + r0;
L_801F713C: ;
    r22 = r22 + 0x1;
L_801F7140: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_801F70F0;
L_801F714C: ;
    r23 = r23 + 0x1;
L_801F7150: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_801F70B8;
    r3 = r31;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F7174 | size: 0xE4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7174(void) {
    extern void fn_8012640C();
    extern void fn_801F78D4();
    extern void fn_801F986C();
    extern void fn_801FA634();
    extern void fn_80205BE8();
    extern void fn_80206608();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    /* stmw r22, 0x8(r1) */;
    r29 = r3;
    r30 = r5;
    r28 = r4 & 0xFFFF;
    r31 = 0x0;
    r23 = 0x0;
    goto L_801F7234;
L_801F719C: ;
    if ((u32)r29 != (u32)0x0) goto L_801F71AC;
    r3 = 0x0;
    goto L_801F71B8;
L_801F71AC: ;
    r3 = r29;
    r4 = r23;
    fn_801F78D4();
L_801F71B8: ;
    r26 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_801F7230;
    r27 = r30 & 0xFFFF;
    r22 = 0x0;
    goto L_801F7224;
L_801F71D4: ;
    r3 = r26;
    r4 = r22;
    fn_801F986C();
    /* mr. r25, r3 */;
    if ((u32)r29 == (u32)0x0) goto L_801F7220;
    fn_80205BE8();
    /* mr. r24, r3 */;
    if ((u32)r29 == (u32)0x0) goto L_801F7220;
    r3 = r25;
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_801F7220;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r31 = r31 + r0;
L_801F7220: ;
    r22 = r22 + 0x1;
L_801F7224: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)r27) goto L_801F71D4;
L_801F7230: ;
    r23 = r23 + 0x1;
L_801F7234: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_801F719C;
    r3 = r31;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F7258 | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7258(void) {
    extern void fn_801F78D4();
    extern void fn_801FA634();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    if ((u32)r3 != (u32)0x0) goto L_801F7278;
    r31 = 0x0;
    goto L_801F7280;
L_801F7278: ;
    fn_801F78D4();
    r31 = r3;
L_801F7280: ;
    r3 = r31;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F7298;
    r3 = 0x0;
    goto L_801F729C;
L_801F7298: ;
    r3 = r31;
L_801F729C: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F72B0 | size: 0xD8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F72B0(void) {
    extern void fn_801F7858();
    extern void fn_801F7870();
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
    /* mr. r30, r6 */;
    r28 = r4;
    r29 = r5;
    r31 = r7;
    if ((s32)r0 == (s32)0) goto L_801F7374;
    if ((u32)r31 == (u32)0x0) goto L_801F7374;
    fn_801F7870();
    if ((u32)r3 != (u32)0x0) goto L_801F72F0;
    r3 = 0x0;
    goto L_801F72F8;
L_801F72F0: ;
    fn_801F7858();
    r3 = r3 & 0xFFFF;
L_801F72F8: ;
    r0 = r28 & 0xFFFF;
    *(u8*)((u8*)r30 + 0x0) = r3;
    r3 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r3;
    if ((u32)r0 == (u32)0x1) goto L_801F7374;
    if ((u32)r0 != (u32)0x2) goto L_801F7374;
    r0 = *(u8*)((u8*)r30 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_801F7350;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F7338;
    r0 = 0x1;
    *(u8*)((u8*)r31 + 0x0) = r0;
    goto L_801F7374;
L_801F7338: ;
    if ((u32)r0 != (u32)0x1) goto L_801F7374;
    r0 = -0x1;
    *(u8*)((u8*)r31 + 0x0) = r0;
    goto L_801F7374;
    goto L_801F7374;
L_801F7350: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F7364;
    r0 = -0x1;
    *(u8*)((u8*)r31 + 0x0) = r0;
    goto L_801F7374;
L_801F7364: ;
    if ((u32)r0 != (u32)0x1) goto L_801F7374;
    r0 = 0x1;
    *(u8*)((u8*)r31 + 0x0) = r0;
L_801F7374: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F7388 | size: 0x7C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7388(void) {
    extern void fn_801F78D4();
    extern void fn_801FA524();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = 0x0;
    r31 = 0x0;
    goto L_801F73E0;
L_801F73A8: ;
    if ((u32)r29 != (u32)0x0) goto L_801F73B8;
    r3 = 0x0;
    goto L_801F73C4;
L_801F73B8: ;
    r3 = r29;
    r4 = r31 & 0xFF;
    fn_801F78D4();
L_801F73C4: ;
    fn_801FA524();
    r0 = r3 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_801F73DC;
    r3 = r30 & 0xFF;
    r0 = r3 + 0x1;
    r30 = r0 & 0xFF;
L_801F73DC: ;
    r31 = r31 + 0x1;
L_801F73E0: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_801F73A8;
    r3 = r30;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F7404 | size: 0x7C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7404(void) {
    extern void fn_801EF634();
    extern void fn_801F793C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F7424;
    r3 = 0x0;
    goto L_801F746C;
L_801F7424: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F743C;
    r3 = 0x0;
    goto L_801F746C;
L_801F743C: ;
    if ((u32)r31 != (u32)0x0) goto L_801F744C;
    r0 = 0x0;
    goto L_801F7458;
L_801F744C: ;
    r3 = r31;
    fn_801F793C();
    r0 = r3 & 0xFFFF;
L_801F7458: ;
    if ((s32)r0 != (s32)0x0) goto L_801F7468;
    r3 = 0x0;
    goto L_801F746C;
L_801F7468: ;
    r3 = 0x1;
L_801F746C: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801F7480 | size: 0xB0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7480(void) {
    extern void fn_8011B950();
    extern void fn_801F789C();
    extern void fn_801F78AC();
    extern void fn_801F78D4();
    extern void fn_801F7908();
    extern void fn_801FA8CC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    /* mr. r30, r3 */;
    r31 = r4;
    if ((s32)r0 == (s32)0) goto L_801F751C;
    if ((s32)r0 == (s32)0) goto L_801F7508;
    if ((s32)r0 == (s32)0) goto L_801F74AC;
    r4 = 0x0;
    fn_801F78AC();
L_801F74AC: ;
    if ((u32)r30 != (u32)0x0) goto L_801F74BC;
    r3 = 0x0;
    goto L_801F74C8;
L_801F74BC: ;
    r3 = r30;
    r4 = 0x0;
    fn_801F7908();
L_801F74C8: ;
    r4 = 0x6;
    fn_8011B950();
    if ((u32)r30 != (u32)0x0) goto L_801F74E0;
    r3 = 0x0;
    goto L_801F74EC;
L_801F74E0: ;
    r3 = r30;
    r4 = 0x0;
    fn_801F78D4();
L_801F74EC: ;
    r4 = 0x2;
    fn_801FA8CC();
    if ((u32)r30 == (u32)0x0) goto L_801F7508;
    r3 = r30;
    r4 = 0x0;
    fn_801F789C();
L_801F7508: ;
    if ((u32)r30 == (u32)0x0) goto L_801F751C;
    r4 = r31 & 0xFFFF;
    r3 = r30;
    fn_801F78AC();
L_801F751C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F7530 | size: 0xC8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7530(void) {
    extern void fn_8011B950();
    extern void fn_801F789C();
    extern void fn_801F78AC();
    extern void fn_801F78D4();
    extern void fn_801F7908();
    extern void fn_801FA8CC();
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
    /* mr. r28, r3 */;
    if ((s32)r0 == (s32)0) goto L_801F75E4;
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    goto L_801F75D8;
L_801F7554: ;
    r0 = r29 & 0xFFFF;
    r0 = r0 * 0x5230;
    /* add. r30, r28, r0 */;
    if ((s32)r0 == (s32)0) goto L_801F75D4;
    if ((u32)r30 == (u32)0x0) goto L_801F7578;
    r3 = r30;
    r4 = 0x0;
    fn_801F78AC();
L_801F7578: ;
    if ((u32)r30 != (u32)0x0) goto L_801F7588;
    r3 = 0x0;
    goto L_801F7594;
L_801F7588: ;
    r3 = r30;
    r4 = 0x0;
    fn_801F7908();
L_801F7594: ;
    r4 = 0x6;
    fn_8011B950();
    if ((u32)r30 != (u32)0x0) goto L_801F75AC;
    r3 = 0x0;
    goto L_801F75B8;
L_801F75AC: ;
    r3 = r30;
    r4 = 0x0;
    fn_801F78D4();
L_801F75B8: ;
    r4 = 0x2;
    fn_801FA8CC();
    if ((u32)r30 == (u32)0x0) goto L_801F75D4;
    r3 = r30;
    r4 = 0x0;
    fn_801F789C();
L_801F75D4: ;
    r29 = r29 + 0x1;
L_801F75D8: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_801F7554;
L_801F75E4: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x801F75F8 | size: 0xC0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F75F8(void) {
    extern void fn_801F7798();
    extern void fn_801F77BC();
    extern void fn_801F77E0();
    extern void fn_801F7870();
    extern void fn_801F789C();
    extern void fn_801F78AC();
    extern u8 jumptable_80375628[];
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
    void (*ctr_fn)(void) = 0;

    r0 = r5 & 0xFFFF;
    /* stmw r29, 0x14(r1) */;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    if ((s32)r0 == (s32)0) goto L_801F76A4;
    if ((u32)r0 < (u32)0xa) goto L_801F7628;
    goto L_801F76A4;
L_801F7628: ;
    if ((u32)r0 >= (u32)0x4) goto L_801F7638;
    r3 = r4;
    fn_801F7870();
L_801F7638: ;
    if ((u32)r3 == (u32)0x0) goto L_801F76A4;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 > (u32)0x8) goto L_801F76A4;
    r4 = (u32)jumptable_80375628;
    r0 = r0 << 2;
    r4 = (u32)jumptable_80375628;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = r31 & 0xFFFF;
    fn_801F77E0();
    goto L_801F76A4;
    r5 = r31;
    r4 = r30 & 0xFF;
    fn_801F77BC();
    goto L_801F76A4;
    r5 = r31;
    r4 = r30 & 0xFF;
    fn_801F7798();
    goto L_801F76A4;
    r4 = r31 & 0xFFFF;
    fn_801F78AC();
    goto L_801F76A4;
    r4 = r31 & 0xFF;
    fn_801F789C();
L_801F76A4: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F76B8 | size: 0xE0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F76B8(void) {
    extern void fn_801F77F0();
    extern void fn_801F7824();
    extern void fn_801F7858();
    extern void fn_801F7870();
    extern void fn_801F78BC();
    extern void fn_801F78D4();
    extern void fn_801F7908();
    extern void fn_801F793C();
    extern u8 jumptable_8037564C[];
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

    r0 = r5 & 0xFFFF;
    /* stmw r30, 0x8(r1) */;
    r30 = r5;
    r31 = r6;
    if ((s32)r0 == (s32)0) goto L_801F76E0;
    if ((u32)r0 < (u32)0xa) goto L_801F76E8;
L_801F76E0: ;
    r3 = 0x0;
    goto L_801F7784;
L_801F76E8: ;
    if ((u32)r0 >= (u32)0x4) goto L_801F76F8;
    r3 = r4;
    fn_801F7870();
L_801F76F8: ;
    if ((u32)r3 != (u32)0x0) goto L_801F7708;
    r3 = 0x0;
    goto L_801F7784;
L_801F7708: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 > (u32)0x8) goto L_801F7780;
    r4 = (u32)jumptable_8037564C;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8037564C;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    fn_801F7858();
    r3 = r3 & 0xFFFF;
    goto L_801F7784;
    r4 = r31 & 0xFF;
    fn_801F7824();
    goto L_801F7784;
    r4 = r31 & 0xFF;
    fn_801F77F0();
    goto L_801F7784;
    fn_801F793C();
    r3 = r3 & 0xFFFF;
    goto L_801F7784;
    r4 = r31;
    fn_801F7908();
    goto L_801F7784;
    r4 = r31;
    fn_801F78D4();
    goto L_801F7784;
    fn_801F78BC();
    r3 = r3 & 0xFF;
    goto L_801F7784;
L_801F7780: ;
    r3 = 0x0;
L_801F7784: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x801F7798 | size: 0x24 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7798(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r3 == (u32)0x0) return;
    r0 = r4 & 0xFF;
    if ((u32)r0 >= (u32)0x2) return;
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0xC) = r5;
    return;
}
#pragma pop

/* 0x801F77BC | size: 0x24 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F77BC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r3 == (u32)0x0) return;
    r0 = r4 & 0xFF;
    if ((u32)r0 >= (u32)0x2) return;
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x4) = r5;
    return;
}
#pragma pop

/* 0x801F77F0 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F77F0(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_801F7800;
    r3 = 0x0;
    return;
L_801F7800: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_801F7814;
    r3 = 0x0;
    return;
L_801F7814: ;
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    return;
}
#pragma pop

/* 0x801F7824 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7824(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_801F7834;
    r3 = 0x0;
    return;
L_801F7834: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_801F7848;
    r3 = 0x0;
    return;
L_801F7848: ;
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0x4);
    return;
}
#pragma pop

/* 0x801F7870 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7870(void) {
    extern u8 lbl_80478F38[];
    extern u8 lbl_80478F3C[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = *(u32*)lbl_80478F38;
    r3 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r3 < (u32)r0) goto L_801F788C;
    r3 = 0x0;
    return;
L_801F788C: ;
    r0 = r3 * 0x14;
    r3 = *(u32*)lbl_80478F3C;
    r3 = r3 + r0;
    return;
}
#pragma pop

/* 0x801F78D4 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F78D4(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    /* mr. r5, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F78E4;
    r3 = 0x0;
    return;
L_801F78E4: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F78F8;
    r3 = 0x0;
    return;
L_801F78F8: ;
    r3 = r0 * 0x28e4;
    r3 = r3 + 0x64;
    r3 = r5 + r3;
    return;
}
#pragma pop

/* 0x801F7908 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7908(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    /* mr. r5, r3 */;
    if ((s32)r0 != (s32)0) goto L_801F7918;
    r3 = 0x0;
    return;
L_801F7918: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F792C;
    r3 = 0x0;
    return;
L_801F792C: ;
    /* clrlslwi r3, r4, 16, 4 */;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
    return;
}
#pragma pop

/* 0x801F7954 | size: 0x21C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7954(void) {
    extern void fn_80122DDC();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_8012A130();
    extern void fn_8012A5B0();
    extern void fn_801EF634();
    extern void fn_801FB1C0();
    extern void fn_80205BE8();
    extern void fn_80206608();
    extern void fn_80206A04();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    /* mr. r28, r3 */;
    r29 = r4;
    if ((s32)r0 == (s32)0) goto L_801F7B5C;
    if ((s32)r0 != (s32)0) goto L_801F797C;
    r0 = 0x0;
    goto L_801F79F4;
L_801F797C: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_801F7994;
    r0 = 0x0;
    goto L_801F79F4;
L_801F7994: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x43;
    r6 = 0x0;
    fn_801FB1C0();
    if ((s32)r3 != (s32)0x0) goto L_801F79B8;
    r0 = 0x0;
    goto L_801F79F4;
L_801F79B8: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r3 != (u32)0x0) goto L_801F79DC;
    r0 = 0x0;
    goto L_801F79F4;
L_801F79DC: ;
    fn_8012A130();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_801F79F0;
    r0 = 0x0;
    goto L_801F79F4;
L_801F79F0: ;
    r0 = 0x1;
L_801F79F4: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_801F7B5C;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    r30 = r3;
    r4 = 0x0;
    r3 = 0x0;
    goto L_801F7A2C;
L_801F7A20: ;
    r0 = r4 & 0xFFFF;
    r4 = r4 + 0x1;
    *(u8*)(r29 + r0) = r3;
L_801F7A2C: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F7A20;
    r31 = 0x0;
    goto L_801F7B50;
L_801F7A40: ;
    r3 = r30;
    r5 = r31 & 0xFFFF;
    r4 = 0x3;
    fn_8012A5B0();
    r27 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x6) goto L_801F7B4C;
    r25 = 0x0;
    goto L_801F7AB8;
L_801F7A68: ;
    r3 = r28;
    r6 = r25;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    r26 = r3;
    fn_80206A04();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x6) goto L_801F7AB4;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    fn_8012640C();
    if ((u32)r3 == (u32)0x0) goto L_801F7AB4;
    if ((u32)r27 != (u32)r3) goto L_801F7AB4;
    goto L_801F7AC8;
L_801F7AB4: ;
    r25 = r25 + 0x1;
L_801F7AB8: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F7A68;
    r26 = 0x0;
L_801F7AC8: ;
    if ((u32)r26 == (u32)0x0) goto L_801F7B4C;
    r3 = r26;
    fn_80206A04();
    r0 = r3 & 0xFF;
    if ((u32)r26 == (u32)0x0) goto L_801F7B4C;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 == (s32)0x1) goto L_801F7B4C;
    r3 = r26;
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((s32)r3 != (s32)0x1) goto L_801F7B1C;
    r0 = r31 & 0xFFFF;
    r3 = 0x3;
    *(u8*)(r29 + r0) = r3;
    goto L_801F7B4C;
L_801F7B1C: ;
    r3 = r26;
    fn_80205BE8();
    fn_80122DDC();
    r0 = r3 & 0xFF;
    if ((s32)r3 != (s32)0x1) goto L_801F7B40;
    r0 = r31 & 0xFFFF;
    r3 = 0x2;
    *(u8*)(r29 + r0) = r3;
    goto L_801F7B4C;
L_801F7B40: ;
    r0 = r31 & 0xFFFF;
    r3 = 0x1;
    *(u8*)(r29 + r0) = r3;
L_801F7B4C: ;
    r31 = r31 + 0x1;
L_801F7B50: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_801F7A40;
L_801F7B5C: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F7B70 | size: 0xE4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7B70(void) {
    extern void fn_80129BC8();
    extern void fn_801429E8();
    extern void fn_80142CF4();
    extern void fn_801FB1C0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    fn_801FB1C0();
    if ((u32)r3 != (u32)0x0) goto L_801F7BA0;
    r3 = 0x0;
    goto L_801F7C40;
L_801F7BA0: ;
    r5 = r1 + 0x8;
    r4 = 0x1;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    fn_80129BC8();
    /* mr. r31, r3 */;
    if ((u32)r3 != (u32)0x0) goto L_801F7BC8;
    r3 = 0x0;
    goto L_801F7C40;
L_801F7BC8: ;
    r29 = 0x0;
    goto L_801F7C2C;
L_801F7BD0: ;
    /* clrlslwi r0, r29, 16, 2 */;
    r30 = r31 + r0;
    r3 = r30;
    fn_801429E8();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_801F7C28;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x1b;
    r6 = 0x0;
    fn_80142CF4();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_801F7C28;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x1c;
    r6 = 0x0;
    fn_80142CF4();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_801F7C28;
    r3 = 0x1;
    goto L_801F7C40;
L_801F7C28: ;
    r29 = r29 + 0x1;
L_801F7C2C: ;
    r0 = *(u16*)(sp + 0x8);
    r3 = r29 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_801F7BD0;
    r3 = 0x0;
L_801F7C40: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F7C54 | size: 0x20C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7C54(void) {
    extern void fn_80129BC8();
    extern void fn_801429E8();
    extern void fn_80142CF4();
    extern void fn_801FB1C0();
    extern void fn_80204C08();
    extern void fn_802062FC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r23, 0x1c(r1) */;
    r24 = r4;
    r25 = r5;
    r26 = r6;
    r27 = r3;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if ((u32)r3 != (u32)0x0) goto L_801F7C94;
    r3 = 0x0;
    goto L_801F7E4C;
L_801F7C94: ;
    r5 = r1 + 0x8;
    r4 = 0x2;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    fn_80129BC8();
    /* mr. r31, r3 */;
    if ((u32)r3 != (u32)0x0) goto L_801F7CBC;
    r3 = 0x0;
    goto L_801F7E4C;
L_801F7CBC: ;
    r5 = 0x0;
    r3 = r1 + 0xc;
    r4 = r5;
    goto L_801F7CD8;
L_801F7CCC: ;
    /* clrlslwi r0, r5, 16, 1 */;
    r5 = r5 + 0x1;
    *(u16*)(r3 + r0) = r4;
L_801F7CD8: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F7CCC;
    r30 = 0x0;
    r28 = r1 + 0xc;
    r23 = r30;
    goto L_801F7D38;
L_801F7CF4: ;
    r3 = r27;
    r6 = r23;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    r29 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_801F7D34;
    r3 = r29;
    fn_80204C08();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2) goto L_801F7D34;
    /* clrlslwi r0, r30, 16, 1 */;
    r30 = r30 + 0x1;
    *(u16*)(r28 + r0) = r3;
L_801F7D34: ;
    r23 = r23 + 0x1;
L_801F7D38: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F7CF4;
    r0 = r25 & 0xFFFF;
    r5 = 0x0;
    r4 = 0x0;
    goto L_801F7D60;
L_801F7D54: ;
    /* clrlslwi r3, r5, 16, 1 */;
    r5 = r5 + 0x1;
    *(u16*)(r24 + r3) = r4;
L_801F7D60: ;
    r3 = r5 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_801F7D54;
    r28 = 0x0;
    r29 = 0x0;
    goto L_801F7E38;
L_801F7D78: ;
    /* clrlslwi r0, r29, 16, 2 */;
    r23 = r31 + r0;
    r3 = r23;
    fn_801429E8();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)r0) goto L_801F7E34;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x1b;
    r6 = 0x0;
    fn_80142CF4();
    r27 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)r0) goto L_801F7E34;
    r3 = r23;
    r4 = 0x0;
    r5 = 0x1c;
    r6 = 0x0;
    fn_80142CF4();
    r5 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)r0) goto L_801F7E34;
    r0 = r26 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F7E18;
    r4 = r1 + 0xc;
    r0 = r30 & 0xFFFF;
    r6 = 0x0;
    goto L_801F7E04;
L_801F7DE4: ;
    /* clrlslwi r3, r6, 16, 1 */;
    r3 = *(u16*)(r4 + r3);
    if ((u32)r27 != (u32)r3) goto L_801F7E00;
    r3 = r5 & 0xFFFF;
    if ((u32)r27 == (u32)r3) goto L_801F7E00;
    /* subi r5, r5, 0x1 */;
L_801F7E00: ;
    r6 = r6 + 0x1;
L_801F7E04: ;
    r3 = r6 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_801F7DE4;
    r0 = r5 & 0xFFFF;
    if ((u32)r3 == (u32)r0) goto L_801F7E34;
L_801F7E18: ;
    r3 = r28 & 0xFFFF;
    r0 = r25 & 0xFFFF;
    if ((u32)r3 >= (u32)r0) goto L_801F7E34;
    /* clrlslwi r0, r28, 16, 1 */;
    r28 = r28 + 0x1;
    *(u16*)(r24 + r0) = r27;
L_801F7E34: ;
    r29 = r29 + 0x1;
L_801F7E38: ;
    r0 = *(u16*)(sp + 0x8);
    r3 = r29 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_801F7D78;
    r3 = r28;
L_801F7E4C: ;
    /* lmw r23, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x801F7E60 | size: 0x90 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7E60(void) {
    extern void fn_801FB1C0();
    extern void fn_80204A5C();
    extern void fn_802062FC();
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
    r30 = 0x0;
    goto L_801F7ECC;
L_801F7E7C: ;
    r3 = r29;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    r31 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_801F7EC8;
    r3 = r31;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    fn_80204A5C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F7EC8;
    r3 = 0x1;
    goto L_801F7EDC;
L_801F7EC8: ;
    r30 = r30 + 0x1;
L_801F7ECC: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F7E7C;
    r3 = 0x0;
L_801F7EDC: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x801F7EF0 | size: 0x90 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801F7EF0(void) {
    extern void fn_801FB1C0();
    extern void fn_80204A5C();
    extern void fn_802062FC();
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
    r30 = 0x0;
    goto L_801F7F5C;
L_801F7F0C: ;
    r3 = r29;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    r31 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_801F7F58;
    r3 = r31;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0x0;
    fn_80204A5C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801F7F58;
    r3 = 0x1;
    goto L_801F7F6C;
L_801F7F58: ;
    r30 = r30 + 0x1;
L_801F7F5C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_801F7F0C;
    r3 = 0x0;
L_801F7F6C: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop


#pragma pop
