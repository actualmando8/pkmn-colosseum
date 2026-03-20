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
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801F021C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801F021C(u8* ptr) {
    if (ptr == NULL) { return 0; }
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
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA490) = val;
}

/* Address: 0x801F644C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F644C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA48C) = val;
}

/* Address: 0x801F647C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F647C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA488) = val;
}

/* Address: 0x801F64AC | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F64AC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA484) = val;
}

/* Address: 0x801F64DC | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F64DC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)((u8*)ptr + 0xA4E4) = val;
}

/* Address: 0x801F65C0 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F65C0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)((u8*)ptr + 0xA4C0) = val;
}

/* Address: 0x801F6618 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6618(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x10]) = val;
}

/* Address: 0x801F6628 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6628(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* Address: 0x801F6638 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6638(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x801F6648 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F6648(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x801F668C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F668C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x801F66A4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F66A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x801F66BC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F66BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x801F66D4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801F66D4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x801F6764 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6764(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)((u8*)ptr + 0xA4BE) = val;
}

/* Address: 0x801F6778 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6778(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)((u8*)ptr + 0xA4BC) = val;
}

/* Address: 0x801F678C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F678C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)((u8*)ptr + 0xA4BA) = val;
}

/* Address: 0x801F67A0 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67A0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)((u8*)ptr + 0xA4B8) = val;
}

/* Address: 0x801F67B4 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67B4(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA4B0) = val;
}

/* Address: 0x801F67C8 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67C8(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA4AC) = val;
}

/* Address: 0x801F67DC | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67DC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA4A8) = val;
}

/* Address: 0x801F67F0 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F67F0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA4B4) = val;
}

/* Address: 0x801F6804 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6804(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA4A4) = val;
}

/* Address: 0x801F6818 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6818(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA4A0) = val;
}

/* Address: 0x801F682C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F682C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA49C) = val;
}

/* Address: 0x801F6840 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6840(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA498) = val;
}

/* Address: 0x801F6854 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6854(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA494) = val;
}

/* Address: 0x801F6868 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6868(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA480) = val;
}

/* Address: 0x801F687C | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F687C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA47C) = val;
}

/* Address: 0x801F6890 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F6890(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA478) = val;
}

/* Address: 0x801F68A4 | Size: 0x14 | Pattern: nullcheck_addis_setter */
void fn_801F68A4(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xA474) = val;
}

/* Address: 0x801F68B8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F68B8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x12]) = val;
}

/* Address: 0x801F68C8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F68C8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x10]) = val;
}

/* Address: 0x801F6AE8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F6AE8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x12]);
}

/* Address: 0x801F6B00 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F6B00(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x10]);
}

/* Address: 0x801F77E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F77E0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x801F7858 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F7858(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x801F789C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F789C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x522C]) = val;
}

/* Address: 0x801F78AC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F78AC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x801F78BC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_801F78BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x522C]);
}

/* Address: 0x801F793C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F793C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 27 functions matched
 * =================================================================== */

/* Address: 0x801F6430 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6430(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA490);
}

/* Address: 0x801F6460 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6460(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA48C);
}

/* Address: 0x801F6490 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6490(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA488);
}

/* Address: 0x801F64C0 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F64C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA484);
}

/* Address: 0x801F64F0 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F64F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4E4);
}

/* Address: 0x801F65D4 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F65D4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)((u8*)ptr + 0xA4C0);
}

/* Address: 0x801F65F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F65F0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x801F6600 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801F6600(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x801F667C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801F667C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x801F6720 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_801F6720(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x801F68D8 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F68D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4BE);
}

/* Address: 0x801F68F4 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F68F4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4BC);
}

/* Address: 0x801F6910 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6910(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4BA);
}

/* Address: 0x801F692C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F692C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)((u8*)ptr + 0xA4B8);
}

/* Address: 0x801F6948 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6948(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4B0);
}

/* Address: 0x801F6964 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6964(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4AC);
}

/* Address: 0x801F6980 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6980(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4A8);
}

/* Address: 0x801F699C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F699C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4B4);
}

/* Address: 0x801F69B8 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F69B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4A4);
}

/* Address: 0x801F69D4 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F69D4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA4A0);
}

/* Address: 0x801F69F0 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F69F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA49C);
}

/* Address: 0x801F6A0C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A0C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA498);
}

/* Address: 0x801F6A28 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA494);
}

/* Address: 0x801F6A44 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A44(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA480);
}

/* Address: 0x801F6A60 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A60(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA47C);
}

/* Address: 0x801F6A7C | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A7C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xA478);
}

/* Address: 0x801F6A98 | Size: 0x1C | Pattern: nullcheck_addis_getter_bne */
u32 fn_801F6A98(u8* ptr) {
    if (ptr == NULL) { return 0; }
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
void fn_801F000C(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x801F0058 | size: 0x78 | small */
void fn_801F0058(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x801F00D0 | size: 0x64 | small */
void fn_801F00D0(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x801F0134 | size: 0xD0 | medium */
#pragma peephole off
void fn_801F0134(void) {
    /* TODO: decompile (0xD0 bytes, ~52 instructions) */
}
#pragma peephole reset

/* 0x801F0234 | size: 0x28 | small */
void fn_801F0234(void) {
    /* TODO: decompile (0x28 bytes) */
}

/* 0x801F025C | size: 0x50 | small */
void fn_801F025C(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x801F02AC | size: 0x46C | large */
#pragma peephole off
void fn_801F02AC(void) {
    /* TODO: decompile (0x46C bytes, ~283 instructions) */
}
#pragma peephole reset

/* 0x801F0718 | size: 0x180 | medium */
#pragma peephole off
void fn_801F0718(void) {
    /* TODO: decompile (0x180 bytes, ~96 instructions) */
}
#pragma peephole reset

/* 0x801F0898 | size: 0x90 | medium */
#pragma peephole off
void fn_801F0898(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x801F0928 | size: 0xA8 | medium */
#pragma peephole off
void fn_801F0928(void) {
    /* TODO: decompile (0xA8 bytes, ~42 instructions) */
}
#pragma peephole reset

/* 0x801F0F04 | size: 0x188 | medium */
#pragma peephole off
void fn_801F0F04(void) {
    /* TODO: decompile (0x188 bytes, ~98 instructions) */
}
#pragma peephole reset

/* 0x801F1170 | size: 0x5C | small */
void fn_801F1170(void) {
    /* TODO: decompile (0x5C bytes) */
}

/* 0x801F11CC | size: 0x294 | large */
#pragma peephole off
void fn_801F11CC(void) {
    /* TODO: decompile (0x294 bytes, ~165 instructions) */
}
#pragma peephole reset

/* 0x801F1460 | size: 0xAC | medium */
#pragma peephole off
void fn_801F1460(void) {
    /* TODO: decompile (0xAC bytes, ~43 instructions) */
}
#pragma peephole reset

/* 0x801F150C | size: 0x48 | small */
void fn_801F150C(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x801F1554 | size: 0x34 */
extern u32 fn_801254B4(void* context, u32 slot, u16 tableId, u32 flags, u32 value);
s32 fn_801F1554(void* context) {
    fn_801254B4(context, 0, 0x112, 0, 1);
    return 1;
}

/* 0x801F1588 | size: 0x178 | medium */
#pragma peephole off
void fn_801F1588(void) {
    /* TODO: decompile (0x178 bytes, ~94 instructions) */
}
#pragma peephole reset

/* 0x801F1700 | size: 0x58 | small */
void fn_801F1700(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x801F17B0 | size: 0xD8 | medium */
#pragma peephole off
void fn_801F17B0(void) {
    /* TODO: decompile (0xD8 bytes, ~54 instructions) */
}
#pragma peephole reset

/* 0x801F1888 | size: 0x54 | small */
void fn_801F1888(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F18DC | size: 0x3C | small */
void fn_801F18DC(void) {
    /* TODO: decompile (0x3C bytes) */
}

/* 0x801F1918 | size: 0x74 | small */
void fn_801F1918(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x801F198C | size: 0x4 | trivial */
s32 fn_801F198C(void) { return 0; }

/* 0x801F1990 | size: 0xDC | medium */
#pragma peephole off
void fn_801F1990(void) {
    /* TODO: decompile (0xDC bytes, ~55 instructions) */
}
#pragma peephole reset

/* 0x801F1A6C | size: 0xA8 | medium */
#pragma peephole off
void fn_801F1A6C(void) {
    /* TODO: decompile (0xA8 bytes, ~42 instructions) */
}
#pragma peephole reset

/* 0x801F1B14 | size: 0x104 | medium */
#pragma peephole off
void fn_801F1B14(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x801F1C18 | size: 0x80 | small */
void fn_801F1C18(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x801F1C98 | size: 0xC4 | medium */
#pragma peephole off
void fn_801F1C98(void) {
    /* TODO: decompile (0xC4 bytes, ~49 instructions) */
}
#pragma peephole reset

/* 0x801F1D5C | size: 0x60 | small */
void fn_801F1D5C(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x801F1DBC | size: 0x174 | medium */
#pragma peephole off
void fn_801F1DBC(void) {
    /* TODO: decompile (0x174 bytes, ~93 instructions) */
}
#pragma peephole reset

/* 0x801F1F30 | size: 0x4C | small */
void fn_801F1F30(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x801F1F7C | size: 0xA4 | medium */
#pragma peephole off
void fn_801F1F7C(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x801F2020 | size: 0x1FC | medium */
#pragma peephole off
void fn_801F2020(void) {
    /* TODO: decompile (0x1FC bytes, ~127 instructions) */
}
#pragma peephole reset

/* 0x801F221C | size: 0xBC | medium */
#pragma peephole off
void fn_801F221C(void) {
    /* TODO: decompile (0xBC bytes, ~47 instructions) */
}
#pragma peephole reset

/* 0x801F22D8 | size: 0x78 | small */
void fn_801F22D8(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x801F2350 | size: 0xE4 | medium */
#pragma peephole off
void fn_801F2350(void) {
    /* TODO: decompile (0xE4 bytes, ~57 instructions) */
}
#pragma peephole reset

/* 0x801F2434 | size: 0x164 | medium */
#pragma peephole off
void fn_801F2434(void) {
    /* TODO: decompile (0x164 bytes, ~89 instructions) */
}
#pragma peephole reset

/* 0x801F2598 | size: 0xBC | medium */
#pragma peephole off
void fn_801F2598(void) {
    /* TODO: decompile (0xBC bytes, ~47 instructions) */
}
#pragma peephole reset

/* 0x801F2654 | size: 0x54 | small */
void fn_801F2654(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F26A8 | size: 0x12C | medium */
#pragma peephole off
void fn_801F26A8(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x801F27D4 | size: 0x30 | small */
void fn_801F27D4(void) {
    /* TODO: decompile (0x30 bytes) */
}

/* 0x801F2804 | size: 0x34 | small */
void fn_801F2804(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x801F2838 | size: 0x54 | small */
void fn_801F2838(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F288C | size: 0x54 | small */
void fn_801F288C(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F28E0 | size: 0x54 | small */
void fn_801F28E0(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F2934 | size: 0x54 | small */
void fn_801F2934(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F2988 | size: 0x54 | small */
void fn_801F2988(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F29DC | size: 0x54 | small */
void fn_801F29DC(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F2A30 | size: 0x4C | small */
void fn_801F2A30(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x801F2A7C | size: 0xE0 | medium */
#pragma peephole off
void fn_801F2A7C(void) {
    /* TODO: decompile (0xE0 bytes, ~56 instructions) */
}
#pragma peephole reset

/* 0x801F2B5C | size: 0x3E0 | large */
#pragma peephole off
void fn_801F2B5C(void) {
    /* TODO: decompile (0x3E0 bytes, ~248 instructions) */
}
#pragma peephole reset

/* 0x801F2F3C | size: 0x138 | medium */
#pragma peephole off
void fn_801F2F3C(void) {
    /* TODO: decompile (0x138 bytes, ~78 instructions) */
}
#pragma peephole reset

/* 0x801F3074 | size: 0x104 | medium */
#pragma peephole off
void fn_801F3074(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x801F3178 | size: 0x138 | medium */
#pragma peephole off
void fn_801F3178(void) {
    /* TODO: decompile (0x138 bytes, ~78 instructions) */
}
#pragma peephole reset

/* 0x801F32B0 | size: 0x3C | small */
void fn_801F32B0(void) {
    /* TODO: decompile (0x3C bytes) */
}

/* 0x801F32EC | size: 0xFC | medium */
#pragma peephole off
void fn_801F32EC(void) {
    /* TODO: decompile (0xFC bytes, ~63 instructions) */
}
#pragma peephole reset

/* 0x801F33E8 | size: 0x48 | small */
void fn_801F33E8(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x801F3430 | size: 0x6C | small */
void fn_801F3430(void) {
    /* TODO: decompile (0x6C bytes) */
}

/* 0x801F349C | size: 0x50 | small */
void fn_801F349C(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x801F34EC | size: 0x138 | medium */
#pragma peephole off
void fn_801F34EC(void) {
    /* TODO: decompile (0x138 bytes, ~78 instructions) */
}
#pragma peephole reset

/* 0x801F3624 | size: 0x54 | small */
void fn_801F3624(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F3678 | size: 0x138 | medium */
#pragma peephole off
void fn_801F3678(void) {
    /* TODO: decompile (0x138 bytes, ~78 instructions) */
}
#pragma peephole reset

/* 0x801F37B0 | size: 0x1D4 | medium */
#pragma peephole off
void fn_801F37B0(void) {
    /* TODO: decompile (0x1D4 bytes, ~117 instructions) */
}
#pragma peephole reset

/* 0x801F3984 | size: 0x1A0 | medium */
#pragma peephole off
void fn_801F3984(void) {
    /* TODO: decompile (0x1A0 bytes, ~104 instructions) */
}
#pragma peephole reset

/* 0x801F3B24 | size: 0x90 | medium */
#pragma peephole off
void fn_801F3B24(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x801F3BB4 | size: 0x134 | medium */
#pragma peephole off
void fn_801F3BB4(void) {
    /* TODO: decompile (0x134 bytes, ~77 instructions) */
}
#pragma peephole reset

/* 0x801F3CE8 | size: 0x538 | large */
#pragma peephole off
void fn_801F3CE8(void) {
    /* TODO: decompile (0x538 bytes, ~334 instructions) */
}
#pragma peephole reset

/* 0x801F4220 | size: 0x134 | medium */
#pragma peephole off
void fn_801F4220(void) {
    /* TODO: decompile (0x134 bytes, ~77 instructions) */
}
#pragma peephole reset

/* 0x801F4354 | size: 0x10C | medium */
#pragma peephole off
void fn_801F4354(void) {
    /* TODO: decompile (0x10C bytes, ~67 instructions) */
}
#pragma peephole reset

/* 0x801F4460 | size: 0xDC | medium */
#pragma peephole off
void fn_801F4460(void) {
    /* TODO: decompile (0xDC bytes, ~55 instructions) */
}
#pragma peephole reset

/* 0x801F4718 | size: 0x9C | medium */
#pragma peephole off
void fn_801F4718(void) {
    /* TODO: decompile (0x9C bytes, ~39 instructions) */
}
#pragma peephole reset

/* 0x801F47B4 | size: 0x50 | small */
void fn_801F47B4(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x801F4804 | size: 0x5C | small */
void fn_801F4804(void) {
    /* TODO: decompile (0x5C bytes) */
}

/* 0x801F4860 | size: 0x260 | large */
#pragma peephole off
void fn_801F4860(void) {
    /* TODO: decompile (0x260 bytes, ~152 instructions) */
}
#pragma peephole reset

/* 0x801F4AC0 | size: 0x154 | medium */
#pragma peephole off
void fn_801F4AC0(void) {
    /* TODO: decompile (0x154 bytes, ~85 instructions) */
}
#pragma peephole reset

/* 0x801F4C14 | size: 0x890 | massive */
#pragma peephole off
void fn_801F4C14(void) {
    /* TODO: decompile (0x890 bytes, ~548 instructions) */
}
#pragma peephole reset

/* 0x801F61EC | size: 0x220 | large */
#pragma peephole off
void fn_801F61EC(void) {
    /* TODO: decompile (0x220 bytes, ~136 instructions) */
}
#pragma peephole reset

/* 0x801F650C | size: 0x38 | small */
void fn_801F650C(void) {
    /* TODO: decompile (0x38 bytes) */
}

/* 0x801F6544 | size: 0x1C */
u8* fn_801F6544(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return ptr + 0xA4C4;
}

/* 0x801F6560 | size: 0x28 */
void fn_801F6560(u8* ptr, u16 idx, u32 val) {
    if (ptr == NULL) { return; }
    if ((u16)idx >= 8) { return; }
    *(u32*)(ptr + 0xA4C4 + (u16)idx * 4) = val;
}

/* 0x801F6588 | size: 0x38 */
u32 fn_801F6588(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if ((u16)idx >= 8) { return 0; }
    return *(u32*)(ptr + 0xA4C4 + (u16)idx * 4);
}

/* 0x801F6658 | size: 0x24 */
void fn_801F6658(u8* ptr, u8 idx, u16 val) {
    if (ptr == NULL) { return; }
    if ((u8)idx >= 2) { return; }
    *(u16*)(ptr + (u8)idx * 2 + 0x14) = val;
}

/* 0x801F66EC | size: 0x34 */
u32 fn_801F66EC(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if ((u8)idx >= 2) { return 0; }
    return *(u16*)(ptr + (u8)idx * 2 + 0x14);
}

/* 0x801F6AB4 | size: 0x34 */
u8* fn_801F6AB4(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if ((u16)idx >= 2) { return NULL; }
    return ptr + (u16)idx * 0x5230 + 0x14;
}

/* 0x801F6B18 | size: 0x30 */
u8* fn_801F6B18(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if ((u16)idx >= 1) { return NULL; }
    return ptr + (u16)idx * 0x10;
}

/* 0x801F6B54 | size: 0xF8 | medium */
#pragma peephole off
void fn_801F6B54(void) {
    /* TODO: decompile (0xF8 bytes, ~62 instructions) */
}
#pragma peephole reset

/* 0x801F6C4C | size: 0x54 | small */
void fn_801F6C4C(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6CA0 | size: 0x54 | small */
void fn_801F6CA0(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6CF4 | size: 0x54 | small */
void fn_801F6CF4(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6D48 | size: 0x54 | small */
void fn_801F6D48(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6D9C | size: 0x54 | small */
void fn_801F6D9C(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6DF0 | size: 0x54 | small */
void fn_801F6DF0(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6E44 | size: 0x54 | small */
void fn_801F6E44(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6E98 | size: 0x54 | small */
void fn_801F6E98(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x801F6EEC | size: 0x4C | small */
void fn_801F6EEC(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x801F6F38 | size: 0x9C | medium */
#pragma peephole off
void fn_801F6F38(void) {
    /* TODO: decompile (0x9C bytes, ~39 instructions) */
}
#pragma peephole reset

/* 0x801F6FD4 | size: 0xBC | medium */
#pragma peephole off
void fn_801F6FD4(void) {
    /* TODO: decompile (0xBC bytes, ~47 instructions) */
}
#pragma peephole reset

/* 0x801F7090 | size: 0xE4 | medium */
#pragma peephole off
void fn_801F7090(void) {
    /* TODO: decompile (0xE4 bytes, ~57 instructions) */
}
#pragma peephole reset

/* 0x801F7174 | size: 0xE4 | medium */
#pragma peephole off
void fn_801F7174(void) {
    /* TODO: decompile (0xE4 bytes, ~57 instructions) */
}
#pragma peephole reset

/* 0x801F7258 | size: 0x58 | small */
void fn_801F7258(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x801F72B0 | size: 0xD8 | medium */
#pragma peephole off
void fn_801F72B0(void) {
    /* TODO: decompile (0xD8 bytes, ~54 instructions) */
}
#pragma peephole reset

/* 0x801F7388 | size: 0x7C | small */
void fn_801F7388(void) {
    /* TODO: decompile (0x7C bytes) */
}

/* 0x801F7404 | size: 0x7C | small */
void fn_801F7404(void) {
    /* TODO: decompile (0x7C bytes) */
}

/* 0x801F7480 | size: 0xB0 | medium */
#pragma peephole off
void fn_801F7480(void) {
    /* TODO: decompile (0xB0 bytes, ~44 instructions) */
}
#pragma peephole reset

/* 0x801F7530 | size: 0xC8 | medium */
#pragma peephole off
void fn_801F7530(void) {
    /* TODO: decompile (0xC8 bytes, ~50 instructions) */
}
#pragma peephole reset

/* 0x801F75F8 | size: 0xC0 | medium */
#pragma peephole off
void fn_801F75F8(void) {
    /* TODO: decompile (0xC0 bytes, ~48 instructions) */
}
#pragma peephole reset

/* 0x801F76B8 | size: 0xE0 | medium */
#pragma peephole off
void fn_801F76B8(void) {
    /* TODO: decompile (0xE0 bytes, ~56 instructions) */
}
#pragma peephole reset

/* 0x801F7798 | size: 0x24 | small */
void fn_801F7798(void) {
    /* TODO: decompile (0x24 bytes) */
}

/* 0x801F77BC | size: 0x24 | small */
void fn_801F77BC(void) {
    /* TODO: decompile (0x24 bytes) */
}

/* 0x801F77F0 | size: 0x34 | small */
void fn_801F77F0(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x801F7824 | size: 0x34 | small */
void fn_801F7824(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x801F7870 | size: 0x2C | small */
void fn_801F7870(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x801F78D4 | size: 0x34 | small */
void fn_801F78D4(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x801F7908 | size: 0x34 | small */
void fn_801F7908(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x801F7954 | size: 0x21C | large */
#pragma peephole off
void fn_801F7954(void) {
    /* TODO: decompile (0x21C bytes, ~135 instructions) */
}
#pragma peephole reset

/* 0x801F7B70 | size: 0xE4 | medium */
#pragma peephole off
void fn_801F7B70(void) {
    /* TODO: decompile (0xE4 bytes, ~57 instructions) */
}
#pragma peephole reset

/* 0x801F7C54 | size: 0x20C | large */
#pragma peephole off
void fn_801F7C54(void) {
    /* TODO: decompile (0x20C bytes, ~131 instructions) */
}
#pragma peephole reset

/* 0x801F7E60 | size: 0x90 | medium */
#pragma peephole off
void fn_801F7E60(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x801F7EF0 | size: 0x90 | medium */
#pragma peephole off
void fn_801F7EF0(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset


#pragma pop
