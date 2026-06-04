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

/* Forward declarations for fn_801F54A4 asm wrapper */
extern void fn_80119ED0(void);
extern void fn_8011B444(void);
extern void fn_8011B67C(void);
extern void fn_8012640C(void);
extern void fn_801EF634(void);
extern void fn_801F981C(void);
extern void fn_8020DD44(void);
extern void fn_8020DD80(void);
extern void fn_8020E0B0(void);
extern void fn_8020E0C8(void);
extern void fn_8020E0E0(void);
extern void fn_8020E0F8(void);
extern void fn_8020E124(void);
extern void fn_8020E1A4(void);
extern void fn_8020E1BC(void);
extern void fn_8020E1D4(void);
extern void fn_8020E1EC(void);
extern void fn_8020E204(void);
extern void fn_8020E230(void);
extern void fn_8020E248(void);
extern void fn_8020E260(void);
extern void fn_8020E278(void);
extern void fn_8020E290(void);
extern void fn_8020E2A8(void);
extern void fn_8020E2C0(void);
extern void fn_8020E2D8(void);
extern void fn_8020E2F0(void);
extern void fn_8020E308(void);
extern void fn_8020E320(void);
extern void fn_8020E338(void);
extern void fn_8020E350(void);
extern void fn_8020E368(void);
extern void fn_8020E380(void);
extern void fn_8020E398(void);
extern void fn_8020E3B0(void);
extern void fn_8020E3C8(void);
extern void fn_8020E3E0(void);
extern void fn_8020E3F8(void);
extern void fn_8020E410(void);
extern void fn_8020E428(void);
extern void fn_8020E440(void);
extern void fn_8020E458(void);
extern void fn_8020E470(void);
extern void fn_8020E488(void);
extern void fn_8020E4B4(void);

/* =========================================================================
 * fn_801F000C - FrameWaitForDuration
 *
 * Waits for a specified number of frames by calling the frame-wait
 * and frame-delta functions in a loop.
 *
 * @param duration  Number of frame units to wait
 * ========================================================================= */
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

/* Forward declarations for functions referenced by fn_801F54A4 asm wrapper */
u8* fn_801F6544(u8* ptr);
u32 fn_801F6588(u8* ptr, u16 idx);
u8* fn_801F6AB4(u8* ptr, u16 idx);
u8* fn_801F6B18(u8* ptr, u16 idx);
u32 fn_801F7258(u32 param_1);
u32 fn_801F7404(u32 param_1);
void fn_801F025C(void);
void fn_801F02AC(void);
void fn_801F2B5C(void);
void fn_801F3BB4(void);
void fn_801F3CE8(void);
void fn_801F4C14(void);
void fn_801F65F0(u8* ptr, u32 val);
void fn_801F667C(u8* ptr, u16 val);
void fn_801F6B54(u32, u32, u32, u32, u32);
s32 fn_801F7090(u32, u32, u32);
s32 fn_801F7174(u32, u32, u32);
void fn_801F7530(u32, u16);
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

/* 0x801F000C | size: 0x4C | small */
void fn_801F000C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    ((void(*)(void))fn_80008184)();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) {
        r30 = 0x0;
        while (r30 < r31) {

            ((void(*)(void))fn_800F0308)();
            ((void(*)(void))fn_800D3088)();
            r30 = r30 + r3;

        }
    }
    return;
}

/* 0x801F0058 | size: 0x78 | small */
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

    r30 = r4;
    r29 = r3;
    r3 = 0x4;
    r5 = r30;
    r4 = 0x0;
    fn_801F02AC();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = r29;
    r5 = r30;
    r3 = 0x2;
    fn_801F02AC();
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = r31 - r3;
    r0 = __cntlzw(r0);
    r3 = (u32)r0 >> 5;

    return;
}

/* 0x801F00D0 | size: 0x64 | small */
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

    r31 = r4;
    r30 = r3;
    r3 = 0x4;
    r5 = r31;
    r4 = 0x0;
    fn_801F02AC();
    /* mr. r4, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r5 = r31;
    fn_801F02AC();
    if (r3 != (u32)0x0) return;
    r3 = 0x0;

    return;
}

/* 0x801F0134 | size: 0xD0 | medium */
void fn_801F0134(void) {
    extern u8 lbl_80375AC8[];
    extern u32 lbl_80478D40;
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

    r29 = r4;
    r28 = r3;
    r3 = 0x4;
    r5 = r29;
    r4 = 0x0;
    fn_801F02AC();
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r31 = 0x0;
    while (1) {
        r4 = lbl_80478D40;
        r5 = r31 & 0xFFFF;
        if (r5 >= (u32)r4) break;
        r0 = r31 & 0xFFFF;
        do {
        if ((s32)r0 == (s32)0) break;
        r3 = (u32)lbl_80375AC8;
        /* clrlslwi r4, r31, 16, 3 */;
        r0 = (u32)lbl_80375AC8;
        r3 = r0 + r4;
        if (r5 >= (u32)r4) {
            r3 = 0x0;
        }
        if (r3 == (u32)0x0) break;
        if (r3 == (u32)0x0) {
            r0 = 0x0;
        } else {

            r0 = *(u8*)((u8*)r3 + 0x1);
        }
        r0 = r0 & 0xFF;
        if (r3 == (u32)0x0) break;
        r3 = r31;
        r4 = r30;
        r5 = r29;
        fn_801F02AC();
        if (r3 != (u32)r28) break;
        r3 = r31;
        return;
        } while (0);
        r31 = r31 + 0x1;

    }
    r31 = 0x0;

    r3 = r31;

    return;
}

/* 0x801F0234 | size: 0x28 | small */
void fn_801F0234(void) {
    extern u8 lbl_80375AC8[];
    extern u32 lbl_80478D40;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = lbl_80478D40;
    r5 = r3 & 0xFFFF;
    r4 = (u32)lbl_80375AC8;
    /* clrlslwi r3, r3, 16, 3 */;
    r0 = (u32)lbl_80375AC8;
    r3 = r0 + r3;
    if (r5 < r0) return;
    r3 = 0x0;
    return;
}

/* 0x801F025C | size: 0x50 | small */
void fn_801F025C(void) {
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    u8 sp[0x10];
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x14;
    r6 = 0x0;
    r30 = r3;
    r31 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r5 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r31;
    fn_801F02AC();
    return;
}

/* 0x801F02AC | size: 0x46C | large */
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
    u32 ctr = 0;

    r0 = r3 & 0xFFFF;
    r15 = r3;
    r16 = r4;
    r18 = r5;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    ((void(*)(void))fn_801F61BC)();
    r4 = r15 & 0xFFFF;
    if (r4 == (u32)0x1) {
        return;
    }
    /* subi r0, r4, 0x11 */;
    if (r0 <= (u32)0xc) {
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
        return;
        r4 = 0x0;
        r5 = 0x42;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x44;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x45;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x46;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x47;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x48;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x49;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x4b;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x4c;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x4d;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x4e;
        r6 = 0x0;
        fn_801F54A4();
        return;
        r4 = 0x0;
        r5 = 0x4f;
        r6 = 0x0;
        fn_801F54A4();
        return;
    }
    r3 = r18;
    fn_8020E204();
    /* mr. r14, r3 */;
    if (r0 == (u32)0xc) {
        r3 = 0x0;
        return;
    }
    if (r16 != (u32)0x0) {
        r17 = r16;
        r3 = r16;
        r4 = r18;
        fn_801F0718();
        /* mr. r16, r3 */;
        if (r16 == (u32)0x0) {
            r3 = 0x0;
            return;
    }
    }
    r3 = r14;
    r21 = 0x0;
    r19 = 0x0;
    fn_8020E1D4();
    r18 = r3;
    r3 = r14;
    fn_8020E1A4();
    r14 = r3 & 0xFF;
    r27 = r18 & 0xFF;
    r24 = (u32)sp + 0x20;
    r25 = (u32)sp + 0x10;
    r30 = r15 & 0xFFFF;
    r26 = (u32)sp + 0x8;
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r6 = r22;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        /* clrlslwi r28, r22, 16, 2 */;
        *(u32*)(r26 + r28) = r3;
        r0 = r22 & 0xFFFF;
        if ((r30 == (u32)0x4) && (r0 == (u32)0x0)) {

            r3 = *(u32*)(r26 + r28);
            return;
        }
        if (r30 == (u32)0x5) {
            r0 = r22 & 0xFFFF;
            if (r0 == (u32)0x1) {
                r3 = *(u32*)(r26 + r28);
                return;
        }
        }
        r3 = *(u32*)(r26 + r28);
        if (r3 == (u32)r16) {
            r0 = r15 & 0xFFFF;
            r4 = 0x1;
            if (r0 == (u32)0x2) return;

        }
        r0 = r15 & 0xFFFF;
        r4 = 0x0;
        if (r0 == (u32)0x3) return;

        r31 = r4 & 0xFF;
        r18 = 0x0;
        r23 = 0x0;
        while (1) {
            r0 = r23 & 0xFFFF;
            if ((s32)r0 >= (s32)r27) break;
            r3 = *(u32*)(r26 + r28);
            r6 = r23;
            r4 = 0x0;
            r5 = 0x7;
            fn_801F76B8();
            /* clrlslwi r29, r21, 16, 2 */;
            *(u32*)(r25 + r29) = r3;
            if (r30 == (u32)0xb) {
                r0 = r22 & 0xFFFF;
                if (r30 == (u32)0xb) {
                    r0 = r23 & 0xFFFF;
                    if (r30 == (u32)0xb) {
                        r3 = *(u32*)(r25 + r29);
                        return;
            }
            }
            }
            if (r31 == (u32)0x1) {
                r0 = r15 & 0xFFFF;
                if (r0 == (u32)0x6) {
                    r0 = r23 & 0xFFFF;
                    if (r0 == (u32)0x6) {
                        r3 = *(u32*)(r25 + r29);
                        return;
                }
                }
                r0 = r15 & 0xFFFF;
                if (r0 == (u32)0x7) {
                    r0 = r23 & 0xFFFF;
                    if (r0 == (u32)0x1) {
                        r3 = *(u32*)(r25 + r29);
                        return;
                }
                }
                r0 = r15 & 0xFFFF;
                if (r0 == (u32)0x8) {
                    r3 = *(u32*)(r25 + r29);
                    if (r17 != (u32)r3) return;

                }
                r0 = r15 & 0xFFFF;
                if (r0 == (u32)0x9) {
                    r0 = r23 & 0xFFFF;
                    if (r0 == (u32)0x9) {
                        r3 = *(u32*)(r25 + r29);
                        return;
                }
                }
                r0 = r15 & 0xFFFF;
                if (r0 == (u32)0xa) {
                    r0 = r23 & 0xFFFF;
                    if (r0 == (u32)0x1) {
                        r3 = *(u32*)(r25 + r29);
                        return;
                }
                }
                }
            r20 = 0x0;
            while (1) {
                r0 = r20 & 0xFFFF;
                if ((s32)r0 >= (s32)r14) break;
                r3 = *(u32*)(r25 + r29);
                r6 = r20;
                r4 = 0x0;
                r5 = 0x46;
                fn_801FB1C0();
                /* clrlslwi r0, r19, 16, 2 */;
                *(u32*)(r24 + r0) = r3;
                if (r31 == (u32)0x1) {
                    r3 = r15 & 0xFFFF;
                    if (r3 == (u32)0xc) {
                        r3 = r18 & 0xFFFF;
                        if (r3 == (u32)0xc) {
                            r3 = *(u32*)(r24 + r0);
                            return;
                    }
                    }
                    r3 = r15 & 0xFFFF;
                    if (r3 == (u32)0xd) {
                        r3 = r18 & 0xFFFF;
                        if (r3 == (u32)0x1) {
                            r3 = *(u32*)(r24 + r0);
                            return;
                    }
                    }
                    r3 = r15 & 0xFFFF;
                    if (r3 == (u32)0xe) {
                        r3 = *(u32*)(r24 + r0);
                        if (r17 != (u32)r3) return;

                    }
                    r3 = r15 & 0xFFFF;
                    if (r3 == (u32)0xf) {
                        r3 = r18 & 0xFFFF;
                        if (r3 == (u32)0xf) {
                            r3 = *(u32*)(r24 + r0);
                            return;
                    }
                    }
                    r3 = r15 & 0xFFFF;
                    if (r3 == (u32)0x10) {
                        r3 = r18 & 0xFFFF;
                        if (r3 == (u32)0x1) {
                            r3 = *(u32*)(r24 + r0);
                            return;
                    }
                    }
                    }
                r19 = r19 + 0x1;
                r18 = r18 + 0x1;
                r20 = r20 + 0x1;

            }
            r21 = r21 + 0x1;
            r23 = r23 + 0x1;

        }
        r22 = r22 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x801F0718 | size: 0x180 | medium */
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
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
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
    while (1) {
        r0 = r25 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r26;
        r6 = r25;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r23 = r3;
        if (r23 == (u32)r31) {
            return;
        }
        r24 = 0x0;
        while (1) {
            r0 = r24 & 0xFFFF;
            if ((s32)r0 >= (s32)r29) break;
            r3 = r23;
            r6 = r24;
            r4 = 0x0;
            r5 = 0x7;
            fn_801F76B8();
            r22 = r3;
            if (r22 == (u32)r31) {
                r3 = r23;
                return;
            }
            r27 = 0x0;
            while (1) {
                r0 = r27 & 0xFFFF;
                if ((s32)r0 >= (s32)r28) break;
                r3 = r22;
                r6 = r27;
                r4 = 0x0;
                r5 = 0x45;
                fn_801FB1C0();
                if ((u32)r3 == (u32)r31) {
                    r3 = r23;
                    return;
                }
                r27 = r27 + 0x1;

            }
            r27 = 0x0;
            while (1) {
                r0 = r27 & 0xFFFF;
                if ((s32)r0 >= (s32)r30) break;
                r3 = r22;
                r6 = r27;
                r4 = 0x0;
                r5 = 0x46;
                fn_801FB1C0();
                if ((u32)r3 == (u32)r31) {
                    r3 = r23;
                    return;
                }
                r27 = r27 + 0x1;

            }
            r24 = r24 + 0x1;

        }
        r25 = r25 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x801F0898 | size: 0x90 | medium */
void fn_801F0898(void) {
    extern void fn_8020D82C();
    extern void fn_8020D920();
    extern void fn_8020D950();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) {

    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {

        r3 = r31;
        fn_8020D920();
    if (r3 != (u32)0x0) {

            r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    fn_8020D920();
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_8020D82C();

    return;
}

/* 0x801F0928 | size: 0xA8 | medium */
void fn_801F0928(void) {
    extern void fn_8020D7CC();
    extern void fn_8020D7E8();
    extern void fn_8020D82C();
    extern void fn_8020D920();
    extern void fn_8020D950();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) {

    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {

        r3 = r31;
        fn_8020D920();
    if (r3 != (u32)0x0) {

            r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if (r3 == (u32)0x0) {
        r3 = -0x80;
        return;
    }
    r3 = r31;
    fn_8020D920();
    if (r3 == (u32)0x0) {
        r3 = -0x80;
        return;
    }
    fn_8020D82C();
    fn_8020D7E8();
    if (r3 == (u32)0x0) {
        r3 = -0x80;
        return;
    }
    fn_8020D7CC();

    return;
}

/* 0x801F0F04 | size: 0x188 | medium */
void fn_801F0F04(void) {
    extern u8 lbl_8046D790[];
    extern u32 lbl_8047B5E8;
    extern u32 lbl_8047B5EC;
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

    r30 = r3;
    r29 = lbl_8047B5E8;
    r0 = lbl_8047B5EC;
    r3 = r29 + 0x1;
    r3 = r3 & 0x1F;
    if (r3 != (u32)r0) {
        r4 = r29 * 0x30;
        r3 = (u32)lbl_8046D790;
        r0 = 0x6;
        r3 = (u32)lbl_8046D790;
        r31 = r3 + r4;
        /* subi r4, r30, 0x4 */;
        /* subi r5, r31, 0x4 */;
        ctr_fn = (void(*)(void))r0;
        do {
            r3 = *(u32*)((u8*)r4 + 0x4);
            r0 = *(u32*)((u8*)r4 + 0x8);
            *(u32*)((u8*)r5 + 0x4) = r3;
            r5 += 8; *(u32*)r5 = r0;
        } while (--ctr != 0);
        r3 = lbl_8047B5E8;
        r0 = r3 + 0x1;
        lbl_8047B5E8 = r0;
        r0 = r0 & 0x1F;
        lbl_8047B5E8 = r0;
        r3 = r30;
        r4 = r29;
        fn_8020D78C();
        r3 = r31;
        r4 = r29;
        fn_8020D78C();
    } else {
        r31 = 0x0;
    }
    if (r31 == (u32)0x0) {
        r3 = 0x2;
        return;
    }
    if (r30 != (u32)0x0) {

    r3 = r30;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if (r30 != (u32)0x0) {

        r3 = r30;
        fn_8020D920();
    if (r3 != (u32)0x0) {

            r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if (r3 == (u32)0x0) {
        r29 = 0x0;

    } else {
        r3 = r30;
        fn_8020D920();
        fn_8020D82C();
        fn_8020D7E8();
        fn_8020D7B4();
        if (r3 != (u32)0x0) {
            r12 = r3;
            r3 = r30;
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
            r29 = r3;
        } else {
            r29 = 0x1;
        }
    }
    r0 = r29 & 0xFF;
    if (r0 != (u32)0x1) {
        r0 = lbl_8047B5E8;
        r3 = lbl_8047B5EC;
        if (r0 == (u32)r3) { r3 = r29; return; }
        r0 = r3 + 0x1;
        lbl_8047B5EC = r0;
        r0 = r0 & 0x1F;
        lbl_8047B5EC = r0;
        r3 = r29;
        return;
    }
    r3 = r31;
    r4 = r30;
    fn_8020D968();

    r3 = r29;

    return;
}

/* 0x801F1170 | size: 0x5C | small */
void fn_801F1170(void) {
    extern void fn_8020D920();
    extern void fn_8020D950();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    fn_8020D920();
    r3 = -r3;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;

    return;
}

/* 0x801F11CC | size: 0x294 | large */
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
    while (1) {
        r0 = r25 & 0xFFFF;
        if (r0 >= (u32)0x4) break;
        r3 = r29;
        r4 = r25;
        r5 = 0x0;
        fn_8020D844();
        r25 = r25 + 0x1;

    }
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
    if (r29 != (u32)0x0) {

    r3 = r29;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if (r29 != (u32)0x0) {

        r3 = r29;
        fn_8020D920();
    if (r3 != (u32)0x0) {

            r0 = 0x1;
    }
    }
    }
    r0 = r0 & 0xFF;
    if (r3 != (u32)0x0) {

    r3 = r29;
    fn_8020D950();
    r27 = r3;
    r3 = r29;
    fn_8020D938();
    r25 = r3;
    r3 = r29;
    fn_8020D920();
    /* mr. r26, r3 */;
    if (r3 != (u32)0x0) {

    r23 = 0x0;
    r28 = r27 & 0xFFFF;
    while (1) {
    /* clrlslwi r0, r23, 16, 3 */;
    r24 = r26 + r0;
    r3 = r24;
    fn_8020D82C();
    r0 = r3 & 0xFFFF;
    r27 = r3;
    if (r3 != (u32)0x0) {
        r3 = r24;
        fn_8020D814();
        r0 = r27 & 0xFFFF;
        if (r28 != (u32)r0 || (s32)r25 != (s32)r3) {

        r23 = r23 + 0x1;
        continue;
    }
    r24 = 0x0;
    }
    break;
    }
    }
    if (r24 != (u32)0x0) {

        r3 = r29;
        r4 = r24;
        fn_8020D8A8();
    }
    }
    if (r24 == (u32)0x0) {
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
        while (1) {
            r0 = r25 & 0xFFFF;
            if (r0 >= (u32)0x4) break;
            r3 = r29;
            r4 = r25;
            r5 = 0x0;
            fn_8020D844();
            r25 = r25 + 0x1;

        }
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
        return;
    }
    r3 = r29;
    r4 = r31;
    fn_8020D878();
    r3 = r29;
    r4 = r30;
    fn_8020D868();
    r3 = 0x1;

    return;
}

/* 0x801F1460 | size: 0xAC | medium */
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
    r30 = r3;
    fn_8020D8C8();
    r3 = r30;
    r4 = 0x0;
    fn_8020D8B8();
    r3 = r30;
    r4 = 0x0;
    fn_8020D8A8();
    r31 = 0x0;

    while (r0 < (u32)0x4) {
        r3 = r30;
        r4 = r31;
        r5 = 0x0;
        fn_8020D844();
        r31 = r31 + 0x1;

    r0 = r31 & 0xFFFF;
    }
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
    return;
}

/* 0x801F150C | size: 0x48 | small */
void fn_801F150C(void) {
    extern void fn_801F37B0();
    extern void fn_801F3984();
    extern void fn_801F1554();
    u8 sp[0x10];
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
    return;
}

/* 0x801F1554 | size: 0x34 */
extern u32 fn_801254B4(void* context, u32 slot, u16 tableId, u32 flags, u32 value);
s32 fn_801F1554(void* context) {
    fn_801254B4(context, 0, 0x112, 0, 1);
    return 1;
}

/* 0x801F1588 | size: 0x178 | medium */
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

    r31 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x1) {
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
        if (r3 > r0) {
            r3 = r31;
            r4 = 0x2;
            fn_801F3984();
        }
        r3 = r28 & 0xFFFF;
        r0 = r29 & 0xFFFF;
        if (r3 < r0) {
            r3 = r31;
            r4 = 0x3;
            fn_801F3984();
        }
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        if (r3 == (u32)r0) {
            if (r26 >= (u32)r27) {
                r3 = r31;
                r4 = 0x2;
                fn_801F3984();
            }
            if (r26 <= (u32)r27) {
                r3 = r31;
                r4 = 0x3;
                fn_801F3984();
    }
    }
    }
    return;
}

/* 0x801F1700 | size: 0x58 | small */
void fn_801F1700(void) {
    extern void fn_80077B84();
    extern void fn_801F54A4();
    u8 sp[0x10];
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
    if ((r31 == (u32)0x1) && ((s32)r3 > (s32)0x0)) {

        r3 = 0x1;
        return;
    }
    r3 = 0x0;

    return;
}

/* 0x801F17B0 | size: 0xD8 | medium */
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

    r26 = r3;
    r27 = 0x0;
    while (1) {
        r0 = r27 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r26;
        r6 = r27;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        /* mr. r31, r3 */;
        if ((s32)r0 != (s32)0) {
            r29 = 0x0;
            while (1) {
                r0 = r29 & 0xFFFF;
                if (r0 >= (u32)0x2) break;
                r3 = r31;
                r6 = r29;
                r4 = 0x0;
                r5 = 0x7;
                fn_801F76B8();
                /* mr. r30, r3 */;
                if ((s32)r0 != (s32)0) {
                    r28 = 0x0;
                    while (1) {
                        r0 = r28 & 0xFFFF;
                        if (r0 >= (u32)0x2) break;
                        r3 = r30;
                        r6 = r28;
                        r4 = 0x0;
                        r5 = 0x46;
                        fn_801FB1C0();
                        if (r3 != (u32)0x0) {
                            r4 = 0x0;
                            r5 = 0xfa;
                            r6 = 0x0;
                            r7 = 0x0;
                            ((void(*)(void))fn_801254B4)();
                        }
                        r28 = r28 + 0x1;

                    }
                }
                r29 = r29 + 0x1;

            }
        }
        r27 = r27 + 0x1;

    }
    return;
}

/* 0x801F1888 | size: 0x54 | small */
void fn_801F1888(void) {
    extern void fn_801F54A4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0x1a;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0xa) { r3 = 0x1; return; }
    if (r0 == (u32)0x11) { r3 = 0x1; return; }
    if (r0 != (u32)0xb) { r3 = 0x0; return; }

    r3 = 0x1;
    return;

    r3 = 0x0;

    return;
}

/* 0x801F18DC | size: 0x3C | small */
u8 fn_801F18DC(u32 param_1) {
    extern u32 fn_801F54A4(u32, u32, u32, u32);

    return (0x10 - (fn_801F54A4(param_1, 0, 0x1a, 0) & 0xFFFF)) == 0;
}

/* 0x801F1918 | size: 0x74 | small */
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
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        *(u8*)((u8*)r31 + 0x16) = r0;
    } else {

        r0 = 0x1;
        *(u8*)((u8*)r31 + 0x16) = r0;
    }
    return;
}

/* 0x801F198C | size: 0x4 | trivial */
s32 fn_801F198C(void) { return 0; }

/* 0x801F1990 | size: 0xDC | medium */
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
    r30 = r7;
    r31 = r8;
    r8 = r9;
    r7 = (u32)sp + 0x1c;
    while (1) {
        r0 = r9 & 0xFFFF;
        if (r0 >= (u32)0x8) break;
        /* clrlslwi r0, r9, 16, 2 */;
        r9 = r9 + 0x1;
        *(u32*)(r7 + r0) = r8;

    }
    r7 = r5 & 0xFF;
    r0 = r6 & 0xFF;
    r6 = (u32)sp + 0x1c;
    r8 = 0x0;
    r5 = (u32)fn_801F1C98;
    r6 = 0x0;
    r4 = (u32)fn_801F1C98;
    r5 = (u32)sp + 0x8;
    *(u32*)(sp + 0x18) = r0;
    fn_801F37B0();
    r29 = (u32)sp + 0x1c;
    r27 = 0x0;
    r28 = r0 & 0xFFFF;
    while (1) {
        r0 = r27 & 0xFFFF;
        if (r0 >= (u32)r28) break;
        /* clrlslwi r0, r27, 16, 2 */;
        r4 = r30;
        r3 = *(u32*)(r29 + r0);
        r5 = r31;
        fn_80204DE4();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x1;
            return;
        }
        r27 = r27 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x801F1A6C | size: 0xA8 | medium */
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

    r30 = r3;
    r31 = r7;
    r7 = 0x0;
    r3 = 0x0;

    while (r0 < (u32)0x18) {
        /* clrlslwi r0, r7, 16, 2 */;
        r7 = r7 + 0x1;
        *(u32*)(r5 + r0) = r3;

    r0 = r7 & 0xFFFF;
    }
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
    r5 = (u32)sp + 0x8;
    *(u32*)(sp + 0x1C) = r0;
    r6 = 0x0;
    fn_801F2B5C();
    r3 = r0 & 0xFFFF;
    return;
}

/* 0x801F1B14 | size: 0x104 | medium */
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

    r31 = r5;
    r26 = r4;
    r29 = r3;
    r30 = *(u32*)((u8*)r5 + 0x0);
    r28 = *(u32*)((u8*)r5 + 0xC);
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((s32)r0 == (s32)0x0) {
        r3 = r29;
        r4 = r30;
        r5 = r26;
        fn_801F8424();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x1;
            return;
        }
        r3 = r29;
        r4 = r30;
        r5 = r26;
        fn_801F8424();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x1;
            return;
        }
        }
    r0 = *(u32*)((u8*)r31 + 0x10);
    r27 = 0x0;
    r26 = r0 & 0xFFFF;
    while (1) {
        r0 = r27 & 0xFFFF;
        if (r0 >= (u32)r26) break;
        r3 = r29;
        r4 = r27;
        fn_801F986C();
        /* mr. r30, r3 */;
        do {
            if (r0 == (u32)0x1) break;
            r0 = *(u32*)((u8*)r31 + 0x14);
            if ((s32)r0 == (s32)0x1) {
                fn_80206608();
                r0 = r3 & 0xFF;
                if ((s32)r0 == (s32)0x1) break;
            }
            r0 = *(u32*)((u8*)r31 + 0x8);
            r0 = r0 << 2;
            *(u32*)(r28 + r0) = r30;
            r3 = *(u32*)((u8*)r31 + 0x8);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r31 + 0x8) = r0;
        } while (0);

        r27 = r27 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* 0x801F1C18 | size: 0x80 | small */
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

    while (r0 < (u32)0x8) {
        /* clrlslwi r0, r9, 16, 2 */;
        r9 = r9 + 0x1;
        *(u32*)(r5 + r0) = r8;

    r0 = r9 & 0xFFFF;
    }
    r8 = r6 & 0xFF;
    r7 = r7 & 0xFF;
    r9 = 0x0;
    r6 = (u32)fn_801F1C98;
    r0 = (u32)fn_801F1C98;
    r5 = (u32)sp + 0x8;
    r6 = 0x0;
    r4 = r0;
    fn_801F37B0();
    r3 = r0 & 0xFFFF;
    return;
}

/* 0x801F1C98 | size: 0xC4 | medium */
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

    r28 = r5;
    r27 = r4;
    r31 = r3;
    r0 = *(u32*)((u8*)r5 + 0x10);
    r30 = *(u32*)((u8*)r5 + 0x0);
    r29 = *(u32*)((u8*)r5 + 0x4);
    if ((s32)r0 == (s32)0x1) {
        fn_802062FC();
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0x1) {
            r3 = 0x1;
            return;
    }
    }
    r0 = *(u32*)((u8*)r28 + 0xC);
    if ((s32)r0 == (s32)0x0) {
        r3 = r30;
        r4 = r31;
        r5 = r27;
        fn_801F8424();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x1;
            return;
        }
        r3 = r30;
        r4 = r31;
        r5 = r27;
        fn_801F8424();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x1;
            return;
        }
        }
    r0 = *(u32*)((u8*)r28 + 0x8);
    r3 = 0x1;
    r0 = r0 << 2;
    *(u32*)(r29 + r0) = r31;
    r4 = *(u32*)((u8*)r28 + 0x8);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r28 + 0x8) = r0;

    return;
}

/* 0x801F1D5C | size: 0x60 | small */
void fn_801F1D5C(void) {
    extern void fn_801F61EC();
    u32 r0 = 0;
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

    while (r0 < (u32)0x8) {
        /* clrlslwi r0, r8, 16, 2 */;
        r8 = r8 + 0x1;
        *(u32*)(r7 + r0) = r4;

    r0 = r8 & 0xFFFF;
    }
    r4 = r7;
    r5 = r11;
    r6 = r10;
    r7 = r9;
    fn_801F61EC();
    return;
}

/* 0x801F1DBC | size: 0x174 | medium */
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
    r31 = r4;
    r24 = r3;
    if (r0 != (u32)0x2) {
        if (r0 != (u32)0x3) { r3 = 0x0; return; }
    }
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
    while (1) {
        r0 = r27 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r24;
        r6 = r27;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r25 = r3;
        fn_801F7404();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x3) {
            r25 = 0x0;
        }
        if (r25 != (u32)0x0) {
            r26 = 0x0;
            while (1) {
                r0 = r26 & 0xFFFF;
                if (r0 >= (u32)r28) break;
                r3 = r25;
                r4 = r26;
                fn_801F7258();
                /* mr. r29, r3 */;
                if (r25 != (u32)0x0) {
                    fn_801FB8F8();
                    r0 = r3 & 0xFF;
                    if (r0 == (u32)0x1) {
                        break;
                }
                }
                r26 = r26 + 0x1;

            }
        }
        r27 = r27 + 0x1;

    }
    r29 = 0x0;

    if (r29 != (u32)0x0) {
        r3 = r29;
        r4 = r30;
        fn_801F0058();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x1;
        } else {
            r0 = 0x0;
        }
    } else {
        r0 = 0x0;
    }
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r31 & 0xFFFF;
        if (r0 != (u32)0x2) { r3 = 0x0; return; }
        r3 = 0x1;
        return;
    }
    r0 = r31 & 0xFFFF;
    if (r0 != (u32)0x3) { r3 = 0x0; return; }
    r3 = 0x1;
    return;

    r3 = 0x0;

    return;
}

/* 0x801F1F30 | size: 0x4C | small */
u8 fn_801F1F30(u32 param_1, u32 param_2, u16 param_3) {
    extern void fn_801F37B0(u32, void*, void*, u32);
    extern void fn_801F1F7C(void);
    u32 buf[3];

    buf[0] = param_2;
    buf[1] = param_3;
    buf[2] = 0;
    fn_801F37B0(param_1, (void*)fn_801F1F7C, buf, 0);
    return (u8)buf[2];
}

/* 0x801F1F7C | size: 0xA4 | medium */
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

    r28 = r5;
    r31 = r3;
    r0 = *(u32*)((u8*)r5 + 0x4);
    r30 = *(u32*)((u8*)r5 + 0x0);
    r29 = r0 & 0xFFFF;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r3 = r31;
    r4 = r30;
    fn_80202B88();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = r31;
        r4 = 0x27;
        fn_802026E4();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = r31;
            fn_80205B8C();
            r4 = r29;
            fn_80123B5C();
            r0 = (s8)r3;
            if (r0 >= (u32)0x1) {
                r0 = 0x1;
                r3 = 0x0;
                *(u32*)((u8*)r28 + 0x8) = r0;
                return;
    }
    }
    }
    r3 = 0x1;

    return;
}

/* 0x801F2020 | size: 0x1FC | medium */
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

    r27 = r4;
    r26 = r3;
    r28 = r5;
    r30 = 0x0;
    r3 = r27;
    r29 = 0x0;
    fn_80206780();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r5 = 0x17;
    r6 = 0x0;
    r0 = 0x2;
    r3 = (u32)fn_801F34EC;
    r4 = (u32)fn_801F34EC;
    r3 = r26;
    r5 = (u32)sp + 0x28;
    r6 = 0x0;
    *(u32*)(sp + 0x30) = r0;
    fn_801F37B0();
    r6 = 0x47;
    r5 = 0x0;
    r0 = 0x2;
    r3 = (u32)fn_801F34EC;
    r4 = (u32)fn_801F34EC;
    r3 = r26;
    r5 = (u32)sp + 0x18;
    r6 = 0x0;
    *(u32*)(sp + 0x20) = r0;
    fn_801F37B0();
    r0 = 0x0;
    r4 = 0x2a;
    r3 = (u32)fn_801F34EC;
    r4 = (u32)fn_801F34EC;
    *(u32*)(sp + 0xC) = r0;
    r3 = r26;
    r5 = (u32)sp + 0x8;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
    r3 = r27;
    r4 = 0x2;
    fn_80207AE0();
    r0 = r3 & 0xFF;
    if (r0 != (u32)0x1) {
        r3 = r27;
        fn_80207BF4();
        r0 = r3 & 0xFFFF;
        if (r0 == (u32)0x1a) {
        }
        r30 = 0x1;
        }
    r3 = r27;
    r4 = 0x8;
    fn_80207AE0();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r29 = 0x1;
    }
    r3 = r27;
    r4 = 0x16;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) { r3 = 0x1; return; }
    r3 = r27;
    r4 = 0xe;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) { r3 = 0x1; return; }
    r3 = r27;
    r4 = 0x25;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {

        r3 = 0x1;
        return;
    }
    if (r31 != (u32)0x0) {
        if (r28 != (u32)0x0) {
            *(u32*)((u8*)r28 + 0x0) = r31;
        }
        r3 = 0x2;
        return;
    }
    if (r25 != (u32)0x0) {
        r0 = r30 & 0xFF;
        if (r25 == (u32)0x0) {
            if (r28 != (u32)0x0) {
                *(u32*)((u8*)r28 + 0x0) = r25;
            }
            r3 = 0x2;
            return;
    }
    }
    if (r26 != (u32)0x0) {
        r0 = r29 & 0xFF;
        if (r0 == (u32)0x1) {
            if (r28 != (u32)0x0) {
                *(u32*)((u8*)r28 + 0x0) = r26;
            }
            r3 = 0x2;
            return;
    }
    }
    r3 = 0x0;

    return;
}

/* 0x801F221C | size: 0xBC | medium */
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

    r28 = r3;
    r29 = 0x1;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)0x8) break;
        r3 = r28;
        r6 = r30;
        r4 = 0x0;
        r5 = 0x59;
        fn_801F54A4();
        /* mr. r31, r3 */;
        if ((s32)r0 != (s32)0) {
            fn_802062FC();
            r0 = r3 & 0xFF;
            if ((s32)r0 != (s32)0) {
                r3 = r31;
                r4 = 0x0;
                r5 = 0xfe;
                r6 = 0x0;
                fn_8012640C();
                if (r3 != (u32)0x0) {
                    fn_801F1170();
                    r0 = r3 & 0xFF;
                    if (r3 != (u32)0x0) {
                        r3 = r31;
                        r4 = 0x0;
                        r5 = 0x112;
                        r6 = 0x0;
                        fn_8012640C();
                        if ((s32)r3 == (s32)0x0) {
                            r29 = 0x0;
                            r3 = r29;
                            return;
        }
        }
        }
        }
        }
        r30 = r30 + 0x1;

    }

    r3 = r29;
    return;
}

/* 0x801F22D8 | size: 0x78 | small */
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
    return;
}

/* 0x801F2350 | size: 0xE4 | medium */
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

    if (r4 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r3 = r4;
    r4 = 0x0;
    r5 = 0x122;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3;
    r30 = -0x1;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if (r0 >= (u32)0x4) break;
        r0 = r28 & 0xFFFF;
        r0 = r0 * 0xc;
        r29 = r31 + r0;
        r3 = r29;
        fn_8020E614();
        r0 = r3 & 0xFF;
        if (r4 != (u32)0x0) {
            r3 = r29;
            fn_801FD104();
            if (r3 != (u32)0x0) {
                fn_80206780();
                r0 = r3 & 0xFF;
                if (r3 != (u32)0x0) {
                    r3 = r29;
                    fn_801FD0D4();
                    r0 = r3;
                    r3 = r29;
                    r29 = r0;
                    fn_801FD0BC();
                    r0 = r29 & 0xFFFF;
                    if (r3 == (u32)0x0) {
                        r29 = 0x1;
                    }
                    r3 = r3 & 0xFFFF;
                    r0 = r29 & 0xFFFF;
                    r3 = r3 * 0x64;
                    r0 = (s32)r3 / (s32)r0;
                    if ((s32)r0 > (s32)r30) {
                        r30 = r0;
        }
        }
        }
        }
        r28 = r28 + 0x1;

    }
    r3 = r30;

    return;
}

/* 0x801F2434 | size: 0x164 | medium */
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

    if (r4 == (u32)0x0) {
        r3 = -0x1;
        return;
    }
    r7 = 0x0;
    r5 = (u32)sp + 0x8;
    r6 = r7;
    while (1) {
        r0 = r7 & 0xFFFF;
        if (r0 >= (u32)0x8) break;
        /* clrlslwi r0, r7, 16, 2 */;
        r7 = r7 + 0x1;
        *(u32*)(r5 + r0) = r6;

    }
    r7 = r4;
    r4 = (u32)sp + 0x8;
    r5 = 0x1;
    r6 = 0x2;
    fn_801F61EC();
    r4 = r3 & 0xFFFF;
    r29 = r3;
    if (r0 == (u32)0x8) {
        r3 = -0x1;
        return;
    }
    r3 = (u32)sp + 0x8;
    r5 = 0x0;
    while (1) {
        r0 = r5 & 0xFFFF;
        if (r0 >= (u32)r4) break;
        /* clrlslwi r0, r5, 16, 2 */;
        r26 = *(u32*)(r3 + r0);
        if (r26 != (u32)0x0) {
            r3 = r26;
            r4 = 0x1;
            fn_80203ADC();
            r28 = r3;
            r3 = r26;
            r4 = 0x1;
            fn_80203B5C();
            r0 = r3 & 0xFFFF;
            if (r26 == (u32)0x0) {
                r3 = 0x1;
            }
            r4 = r28 & 0xFFFF;
            r0 = r3 & 0xFFFF;
            r3 = r4 * 0x64;
            r31 = (s32)r3 / (s32)r0;
            break;
        }
        r5 = r5 + 0x1;

    }

    r28 = (u32)sp + 0x8;
    r30 = r29 & 0xFFFF;
    r27 = 0x0;
    while (1) {
        r0 = r27 & 0xFFFF;
        if (r0 >= (u32)r30) break;
        /* clrlslwi r0, r27, 16, 2 */;
        r26 = *(u32*)(r28 + r0);
        if (r26 != (u32)0x0) {
            r3 = r26;
            r4 = 0x1;
            fn_80203ADC();
            r29 = r3;
            r3 = r26;
            r4 = 0x1;
            fn_80203B5C();
            r0 = r3 & 0xFFFF;
            if (r26 == (u32)0x0) {
                r3 = 0x1;
            }
            r4 = r29 & 0xFFFF;
            r0 = r3 & 0xFFFF;
            r3 = r4 * 0x64;
            r0 = (s32)r3 / (s32)r0;
            if ((s32)r0 < (s32)r31) {
                r31 = r0;
        }
        }
        r27 = r27 + 0x1;

    }
    r3 = r31;

    return;
}

/* 0x801F2598 | size: 0xBC | medium */
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
    r5 = (u32)sp + 0x8;
    while (1) {
        r0 = r8 & 0xFFFF;
        if (r0 >= (u32)0x8) break;
        /* clrlslwi r0, r8, 16, 2 */;
        r8 = r8 + 0x1;
        *(u32*)(r5 + r0) = r6;

    }
    r5 = r4;
    r6 = r9;
    r4 = (u32)sp + 0x8;
    fn_801F61EC();
    r0 = r3 & 0xFFFF;
    r31 = r3;
    if (r0 == (u32)0x8) {
        r3 = 0x0;
        return;
    }
    fn_800E0C54();
    r4 = r3 & 0xFFFF;
    r3 = r31 & 0xFFFF;
    r0 = (s32)r4 / (s32)r3;
    r0 = r0 * r3;
    r0 = r4 - r0;
    r0 = r0 & 0xFFFF;
    if (r0 >= (u32)0x8) {
        r3 = 0x0;
        return;
    }
    /* clrlslwi r0, r0, 16, 2 */;
    r3 = (u32)sp + 0x8;
    r3 = *(u32*)(r3 + r0);
    if (r3 != (u32)0x0) return;
    r3 = 0x0;

    return;
}

/* 0x801F2654 | size: 0x54 | small */
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
    r5 = (u32)sp + 0x8;
    r6 = 0x0;
    *(u32*)(sp + 0x14) = r0;
    fn_801F37B0();
    r3 = r0 & 0xFFFF;
    return;
}

/* 0x801F26A8 | size: 0x12C | medium */
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

    r28 = r5;
    r31 = r3;
    r0 = *(u32*)((u8*)r5 + 0xC);
    r30 = *(u32*)((u8*)r5 + 0x0);
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        fn_802062FC();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = 0x1;
            return;
    }
    }
    if (r30 == (u32)0x0) {
        r29 = 0x0;

    } else {
        r0 = *(u32*)((u8*)r28 + 0x8);
        if ((s32)r0 == (s32)0x1) {
            r4 = r30;
            r3 = 0x2;
            fn_801F025C();
            r29 = r3;

        } else if ((s32)r0 == (s32)0x2) {
            r4 = r30;
            r3 = 0x3;
            fn_801F025C();
            r29 = r3;

        } else {
            r29 = 0x0;
        }
    }
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r0 = *(u32*)((u8*)r28 + 0x8);

    if ((s32)r0 == (s32)0x1 || (s32)r0 == (s32)0x2 && r29 == (u32)0x0 && r29 == (u32)0x0 && r29 == (u32)0x0 && r29 == (u32)0x0) {

        if (r29 == (u32)0x0) {
            r3 = 0x1;
            return;
        }
    }
    if ((s32)r0 == (s32)0x0) {
        if (r30 != (u32)0x0 || r30 != (u32)r31) {

            r3 = 0x1;
            return;
        }
        if ((s32)r0 != (s32)0x1) {
            if ((s32)r0 != (s32)0x2) { r3 = 0x1; return; }
        }
        if (r29 != (u32)r3) {
            r3 = 0x1;
            return;

            r3 = 0x1;
            return;
        }
        }
    r4 = *(u32*)((u8*)r28 + 0x4);
    r3 = 0x1;
    r0 = r4 + 0x1;
    *(u32*)((u8*)r28 + 0x4) = r0;

    return;
}

/* 0x801F27D4 | size: 0x30 | small */
void fn_801F27D4(u32 param_1) {
    extern void fn_801F37B0(u32, void*, u32, u32);
    extern u32 fn_801F2804(u32);

    fn_801F37B0(param_1, (void*)fn_801F2804, 0, 0);
}

/* 0x801F2804 | size: 0x34 | small */
u32 fn_801F2804(u32 param_1) {
    extern void fn_8012640C(u32, u32, u32, u32);
    extern void fn_80209FAC(void);

    fn_8012640C(param_1, 0, 0xd9, 0);
    fn_80209FAC();
    return 1;
}

/* 0x801F2838 | size: 0x54 | small */
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

    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x9) {
        r3 = r29;
        r4 = r30;
        r5 = r31;
        fn_8011AB50();
    }
    return;
}

/* 0x801F288C | size: 0x54 | small */
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

    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x9) {
        r3 = -0x1;
    } else {

        r3 = r30;
        r4 = r31;
        fn_8011ACB4();
    }
    return;
}

/* 0x801F28E0 | size: 0x54 | small */
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

    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x9) {
        r3 = -0x1;
    } else {

        r3 = r30;
        r4 = r31;
        fn_8011AE40();
    }
    return;
}

/* 0x801F2934 | size: 0x54 | small */
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

    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x9) {
        r3 = r29;
        r4 = r30;
        r5 = r31;
        fn_8011B2C0();
    }
    return;
}

/* 0x801F2988 | size: 0x54 | small */
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

    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x9) {
        r3 = 0x0;
    } else {

        r3 = r30;
        r4 = r31;
        fn_8011B444();
    }
    return;
}

/* 0x801F29DC | size: 0x54 | small */
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

    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x9) {
        r3 = 0x0;
    } else {

        r3 = r30;
        r4 = r31;
        fn_8011B67C();
    }
    return;
}

/* 0x801F2A30 | size: 0x4C | small */
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

    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x9) {
        r3 = r30;
        r4 = r31;
        fn_8011B788();
    }
    return;
}

/* 0x801F2A7C | size: 0xE0 | medium */
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
    r26 = r3;
    fn_801F54A4();
    r3 = r26;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r29 = 0x0;
    while (1) {
        r0 = r29 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r26;
        r6 = r29;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r31 = r3;
        fn_801F7404();
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0) {
            r31 = 0x0;
        }
        if (r31 != (u32)0x0) {
            r30 = 0x0;
            while (1) {
                r0 = r30 & 0xFFFF;
                if (r0 >= (u32)r28) break;
                r3 = r31;
                r4 = r30;
                fn_801F7258();
                /* mr. r27, r3 */;
                if (r31 != (u32)0x0) {
                    fn_801FB8F8();
                    r0 = r3 & 0xFF;
                    if (r0 == (u32)0x1) {
                        r3 = r27;
                        return;
                }
                }
                r30 = r30 + 0x1;

            }
        }
        r29 = r29 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x801F2B5C | size: 0x3E0 | large */
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
    r3 = (u32)sp + 0x8;
    r5 = 0x0;
    r4 = 0x0;
    while (1) {
        r0 = r5 & 0xFFFF;
        if (r0 >= (u32)0x4) break;
        /* clrlslwi r0, r5, 16, 2 */;
        r5 = r5 + 0x1;
        *(u32*)(r3 + r0) = r4;

    }
    r0 = r20 & 0xFF;
    if (r0 == (u32)0x1) {
        r25 = r28;
        r22 = 0x0;
        r24 = 0x0;
        while (1) {
            r0 = r24 & 0xFFFF;
            if (r0 >= (u32)0x2) break;
            r3 = r31;
            r6 = r24;
            r4 = 0x0;
            r5 = 0x35;
            fn_801F54A4();
            r26 = r3;
            fn_801F7404();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r26 = 0x0;
            }
            if (r26 != (u32)0x0) {
                r23 = 0x0;
                while (1) {
                    r0 = r23 & 0xFFFF;
                    if (r0 >= (u32)r27) break;
                    r3 = r26;
                    r4 = r23;
                    fn_801F7258();
                    /* mr. r21, r3 */;
                    if (r26 != (u32)0x0) {
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
                        if (r20 == (u32)0x0) {
                            r0 = r22 & 0xFFFF;
                            if (r0 < (u32)0x4) {
                                fn_800FF560();
                                r4 = r3;
                                r8 = r30;
                                r3 = 0x12;
                                r5 = 0x2000;
                                r6 = 0x1;
                                r7 = 0x0;
                                fn_800F07A8();
                                /* clrlslwi r4, r22, 16, 2 */;
                                r5 = (u32)sp + 0x8;
                                /* addic. r0, (u32)sp, 0x8 */;
                                *(u32*)(r5 + r4) = r3;
                                if (r0 != (u32)0x4) {
                                    r3 = *(u32*)(r5 + r4);
                                    r5 = r21;
                                    r6 = r25;
                                    r7 = r29;
                                    r4 = 0x3;
                                    /* crclr cr1eq */;
                                    fn_800F0654();
                                    r22 = r22 + 0x1;
                    }
                    }
                    }
                    }
                    r23 = r23 + 0x1;

                }
            }
            r24 = r24 + 0x1;

        }
        r21 = (u32)sp + 0x8;
        do {
            ((void(*)(void))fn_800F0308)();
            r22 = 0x0;
            while (1) {
                r0 = r22 & 0xFFFF;
            if (r0 >= (u32)0x4) break;
                /* clrlslwi r0, r22, 16, 2 */;
                r3 = *(u32*)(r21 + r0);
                if (r3 != (u32)0x0) {
                    fn_800F04BC();
                    r0 = r3 & 0xFF;
                    if (r0 == (u32)0x1) break;
                }
                r22 = r22 + 0x1;

            }

            r0 = r22 & 0xFFFF;
        } while (r0 < (u32)0x4);
        r24 = 0x0;
        r23 = (u32)sp + 0x8;
        r21 = r24;
        while (1) {
            r0 = r24 & 0xFFFF;
            if (r0 >= (u32)0x4) break;
            /* clrlslwi r22, r24, 16, 2 */;
            r3 = *(u32*)(r23 + r22);
            if (r3 != (u32)0x0) {
                fn_800F0494();
                *(u32*)(r23 + r22) = r21;
            }
            r24 = r24 + 0x1;

        }

    } else {
        r22 = 0x0;
        while (1) {
            r0 = r22 & 0xFFFF;
            if (r0 >= (u32)0x2) break;
            r3 = r31;
            r6 = r22;
            r4 = 0x0;
            r5 = 0x35;
            fn_801F54A4();
            r21 = r3;
            fn_801F7404();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x4) {
                r21 = 0x0;
            }
            if (r21 != (u32)0x0) {
                r23 = 0x0;
                while (1) {
                    r0 = r23 & 0xFFFF;
                    if (r0 >= (u32)r27) break;
                    r3 = r21;
                    r4 = r23;
                    fn_801F7258();
                    /* mr. r24, r3 */;
                    if (r21 != (u32)0x0) {
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
                        if (r25 == (u32)0x0) {
                            r12 = r30;
                            r3 = r24;
                            r4 = r28;
                            r5 = r29;
                            ctr_fn = (void(*)(void))r12;
                            ctr_fn();
                    }
                    }
                    r23 = r23 + 0x1;

                }
            }
            r22 = r22 + 0x1;

        }
    }
    r22 = 0x0;
    while (1) {
        r0 = r22 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r31;
        r6 = r22;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r21 = r3;
        fn_801F7404();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x2) {
            r21 = 0x0;
        }
        if (r21 != (u32)0x0) {
            r23 = 0x0;
            while (1) {
                r0 = r23 & 0xFFFF;
                if (r0 >= (u32)r27) break;
                r3 = r21;
                r4 = r23;
                fn_801F7258();
                /* mr. r24, r3 */;
                if (r21 != (u32)0x0) {
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
                    if (r25 != (u32)0x0) {
                        r12 = r30;
                        r3 = r24;
                        r4 = r28;
                        r5 = r29;
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                }
                }
                r23 = r23 + 0x1;

            }
        }
        r22 = r22 + 0x1;

    }
    return;
}

/* 0x801F2F3C | size: 0x138 | medium */
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
    while (1) {
        r0 = r31 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r27;
        r6 = r31;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r26 = r3;
        fn_801F7404();
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0) {
            r26 = 0x0;
        }
        if (r26 != (u32)0x0) {
            r25 = 0x0;
            while (1) {
                r0 = r25 & 0xFFFF;
                if (r0 >= (u32)r29) break;
                r3 = r26;
                r4 = r25;
                fn_801F7258();
                /* mr. r23, r3 */;
                if (r26 != (u32)0x0) {
                    r24 = 0x0;
                    while (1) {
                        r0 = r24 & 0xFFFF;
                        if (r0 >= (u32)r28) break;
                        r3 = r23;
                        r4 = r24;
                        fn_801F981C();
                        /* mr. r22, r3 */;
                        if (r26 != (u32)0x0) {
                            fn_802062FC();
                            r0 = r3 & 0xFF;
                            if (r26 != (u32)0x0) {
                                r4 = r22;
                                r5 = r30;
                                r3 = 0x3;
                                fn_801F02AC();
                                r0 = r3;
                                r3 = r22;
                                r4 = r0;
                                fn_80202C1C();
                        }
                        }
                        r24 = r24 + 0x1;

                    }
                }
                r25 = r25 + 0x1;

            }
        }
        r31 = r31 + 0x1;

    }
    return;
}

/* 0x801F3074 | size: 0x104 | medium */
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
    while (1) {
        r0 = r29 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r24;
        r6 = r29;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r31 = r3;
        fn_801F7404();
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0) {
            r31 = 0x0;
        }
        if (r31 != (u32)0x0) {
            r30 = 0x0;
            while (1) {
                r0 = r30 & 0xFFFF;
                if (r0 >= (u32)r27) break;
                r3 = r31;
                r4 = r30;
                fn_801F7258();
                /* mr. r25, r3 */;
                if (r31 != (u32)0x0) {
                    r4 = r25;
                    r5 = r28;
                    r3 = 0x3;
                    fn_801F02AC();
                    r4 = r25;
                    r5 = r28;
                    r6 = r27;
                    r7 = r26;
                    fn_801F6B54();
                }
                r30 = r30 + 0x1;

            }
        }
        r29 = r29 + 0x1;

    }
    return;
}

/* 0x801F3178 | size: 0x138 | medium */
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
    while (1) {
        r0 = r31 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r27;
        r6 = r31;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r26 = r3;
        fn_801F7404();
        r0 = r3 & 0xFF;
        if ((s32)r0 == (s32)0) {
            r26 = 0x0;
        }
        if (r26 != (u32)0x0) {
            r25 = 0x0;
            while (1) {
                r0 = r25 & 0xFFFF;
                if (r0 >= (u32)r29) break;
                r3 = r26;
                r4 = r25;
                fn_801F7258();
                /* mr. r23, r3 */;
                if (r26 != (u32)0x0) {
                    r24 = 0x0;
                    while (1) {
                        r0 = r24 & 0xFFFF;
                        if (r0 >= (u32)r28) break;
                        r3 = r23;
                        r4 = r24;
                        fn_801F981C();
                        /* mr. r22, r3 */;
                        if (r26 != (u32)0x0) {
                            fn_802062FC();
                            r0 = r3 & 0xFF;
                            if (r26 != (u32)0x0) {
                                r4 = r22;
                                r5 = r30;
                                r3 = 0x3;
                                fn_801F02AC();
                                r0 = r3;
                                r3 = r22;
                                r4 = r0;
                                fn_80205274();
                        }
                        }
                        r24 = r24 + 0x1;

                    }
                }
                r25 = r25 + 0x1;

            }
        }
        r31 = r31 + 0x1;

    }
    return;
}

/* 0x801F32B0 | size: 0x3C | small */
u8 fn_801F32B0(u32 param_1) {
    extern void fn_801F2B5C(u32, void*, void*, u32);
    extern void fn_801F32EC(void);
    u8 local;

    local = 0;
    fn_801F2B5C(param_1, (void*)fn_801F32EC, &local, 0);
    return local;
}

/* 0x801F32EC | size: 0xFC | medium */
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

    r28 = r5;
    r31 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    r0 = 0x0;
    r3 = r31;
    *(u16*)(sp + 0x8) = r0;
    r4 = (u32)sp + 0x8;
    fn_801F8A18();
    if (r3 == (u32)0x0) {
        r3 = 0x1;
        return;
    }
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)r29) break;
        r3 = r31;
        r6 = r30;
        r4 = 0x0;
        r5 = 0x46;
        fn_801FB1C0();
        do {
        if (r3 == (u32)0x0) break;
        fn_802062FC();
        r0 = r3 & 0xFF;
        if (r3 != (u32)0x0 || r28 == (u32)0x0) break;

        r0 = *(u8*)((u8*)r28 + 0x0);
        if (r0 == (u32)0x2) break;
        r3 = r31;
        fn_801FB8F8();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r0 = 0x2;
            *(u8*)((u8*)r28 + 0x0) = r0;
            break;
        }
        r0 = 0x1;
        *(u8*)((u8*)r28 + 0x0) = r0;
        } while (0);
        r30 = r30 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* 0x801F33E8 | size: 0x48 | small */
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
    r5 = (u32)sp + 0x8;
    fn_801F37B0();
    r3 = r0 & 0xFFFF;
    return;
}

/* 0x801F3430 | size: 0x6C | small */
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

    r30 = r5;
    r31 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
    } else {

        r0 = *(u32*)((u8*)r30 + 0x0);
        r3 = r31;
        r4 = r0 & 0xFFFF;
        fn_802026E4();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = *(u32*)((u8*)r30 + 0x4);
            r0 = r3 + 0x1;
            *(u32*)((u8*)r30 + 0x4) = r0;
        }
        r3 = 0x1;
    }
    return;
}

/* 0x801F349C | size: 0x50 | small */
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
    u32 r8 = 0;
    u32 r9 = 0;

    r9 = r4 & 0xFFFF;
    r4 = (u32)fn_801F34EC;
    r0 = r6 & 0xFF;
    r8 = 0x0;
    r6 = r5;
    r4 = (u32)fn_801F34EC;
    r5 = (u32)sp + 0x8;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
    return;
}

/* 0x801F34EC | size: 0x138 | medium */
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

    r28 = r5;
    r31 = r3;
    r30 = *(u32*)((u8*)r5 + 0xC);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    if (r30 == (u32)0x0) {
        r29 = 0x0;

    } else {
        r0 = *(u32*)((u8*)r28 + 0x8);
        if ((s32)r0 == (s32)0x1) {
            r4 = r30;
            r3 = 0x2;
            fn_801F025C();
            r29 = r3;

        } else if ((s32)r0 == (s32)0x2) {
            r4 = r30;
            r3 = 0x3;
            fn_801F025C();
            r29 = r3;

        } else {
            r29 = 0x0;
        }
    }
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r0 = *(u32*)((u8*)r28 + 0x8);

    if ((s32)r0 == (s32)0x1 || (s32)r0 == (s32)0x2 && r29 == (u32)0x0 && r29 == (u32)0x0 && r29 == (u32)0x0 && r29 == (u32)0x0) {

        if (r29 == (u32)0x0) {
            r3 = 0x1;
            return;
        }
    }
    if ((s32)r0 == (s32)0x0) {
        if (r30 != (u32)0x0 || r30 != (u32)r31) {

            r3 = 0x1;
            return;
        }
        if ((s32)r0 != (s32)0x1) {
            if ((s32)r0 != (s32)0x2) { r3 = 0x1; return; }
        }
        if (r29 != (u32)r3) {
            r3 = 0x1;
            return;

            r3 = 0x1;
            return;
        }
        }
    r3 = r31;
    fn_80207BF4();
    r0 = *(u32*)((u8*)r28 + 0x0);
    r3 = r3 & 0xFFFF;
    r0 = r0 & 0xFFFF;
    if (r0 == (u32)r3) {
        *(u32*)((u8*)r28 + 0x4) = r31;
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* 0x801F3624 | size: 0x54 | small */
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
    r5 = (u32)sp + 0x8;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
    r3 = r0 & 0xFFFF;
    return;
}

/* 0x801F3678 | size: 0x138 | medium */
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

    r28 = r5;
    r31 = r3;
    r30 = *(u32*)((u8*)r5 + 0xC);
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r3 = 0x1;
        return;
    }
    if (r30 == (u32)0x0) {
        r29 = 0x0;

    } else {
        r0 = *(u32*)((u8*)r28 + 0x8);
        if ((s32)r0 == (s32)0x1) {
            r4 = r30;
            r3 = 0x2;
            fn_801F025C();
            r29 = r3;

        } else if ((s32)r0 == (s32)0x2) {
            r4 = r30;
            r3 = 0x3;
            fn_801F025C();
            r29 = r3;

        } else {
            r29 = 0x0;
        }
    }
    r4 = r31;
    r3 = 0x2;
    fn_801F025C();
    r0 = *(u32*)((u8*)r28 + 0x8);

    if ((s32)r0 == (s32)0x1 || (s32)r0 == (s32)0x2 && r29 == (u32)0x0 && r29 == (u32)0x0 && r29 == (u32)0x0 && r29 == (u32)0x0) {

        if (r29 == (u32)0x0) {
            r3 = 0x1;
            return;
        }
    }
    if ((s32)r0 == (s32)0x0) {
        if (r30 != (u32)0x0 || r30 != (u32)r31) {

            r3 = 0x1;
            return;
        }
        if ((s32)r0 != (s32)0x1) {
            if ((s32)r0 != (s32)0x2) { r3 = 0x1; return; }
        }
        if (r29 != (u32)r3) {
            r3 = 0x1;
            return;

            r3 = 0x1;
            return;
        }
        }
    r3 = r31;
    fn_80207BF4();
    r0 = *(u32*)((u8*)r28 + 0x0);
    r3 = r3 & 0xFFFF;
    r0 = r0 & 0xFFFF;
    if (r0 == (u32)r3) {
        r3 = *(u32*)((u8*)r28 + 0x4);
        r0 = r3 + 0x1;
        *(u32*)((u8*)r28 + 0x4) = r0;
    }
    r3 = 0x1;

    return;
}

/* 0x801F37B0 | size: 0x1D4 | medium */
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
    if (r0 == (u32)0x1) {
        r21 = 0x0;
        while (1) {
            r0 = r21 & 0xFFFF;
            if (r0 >= (u32)0x8) break;
            r3 = r22;
            r6 = r21;
            r4 = 0x0;
            r5 = 0x59;
            fn_801F54A4();
            /* mr. r26, r3 */;
            if (r0 != (u32)0x1) {
                fn_80206780();
                r0 = r3 & 0xFF;
                if (r0 != (u32)0x1) {
                    r12 = r23;
                    r3 = r26;
                    r4 = r28;
                    r5 = r24;
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
                    r0 = r3 & 0xFF;
                    if (r0 == (u32)0x1) {
                        r25 = 0x0;
                        r3 = r25;
                        return;
            }
            }
            }
            r21 = r21 + 0x1;

        }
        r3 = r25;
        return;
    }
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)r26) break;
        r31 = 0x0;
        while (1) {
            r0 = r31 & 0xFFFF;
            if (r0 >= (u32)r27) break;
            r29 = 0x0;
            while (1) {
                r0 = r29 & 0xFFFF;
                if (r0 >= (u32)0x2) break;
                r3 = r22;
                r6 = r29;
                r4 = 0x0;
                r5 = 0x35;
                fn_801F54A4();
                r21 = r3;
                fn_801F7404();
                r0 = r3 & 0xFF;
                if (r0 == (u32)0x8) {
                    r21 = 0x0;
                }
                if (r21 != (u32)0x0) {

                r3 = r21;
                r4 = r31;
                fn_801F7258();
                if (r3 != (u32)0x0) {

                    r4 = r30;
                    fn_801F981C();
                    if (r3 == (u32)0x0) {
                        r3 = 0x0;
                    }
        }
                    }
                if (r3 != (u32)0x0) {
                    r12 = r23;
                    r4 = r28;
                    r5 = r24;
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
                    r0 = r3 & 0xFF;
                    if (r3 == (u32)0x0) {
                        r25 = 0x0;
                        r3 = r25;
                        return;
                }
                }
                r29 = r29 + 0x1;

            }
            r31 = r31 + 0x1;

        }
        r30 = r30 + 0x1;

    }

    r3 = r25;
    return;
}

/* 0x801F3984 | size: 0x1A0 | medium */
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
    if (r4 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    if (r4 == (u32)0x1) {
        r3 = 0x1;
        return;
    }

    if (r0 == (u32)0x0 || r0 == (u32)0x1) {

        r3 = r30;
        fn_801EF62C();
        r3 = 0x1;
        return;
    }
    if (r3 == (u32)0x0) {
        if (r4 == (u32)0x3) { r3 = 0x0; return; }
        if (r4 == (u32)0x5) {

            r3 = 0x0;
            return;
        }
        if (r0 == (u32)0x7) {
            r3 = 0x3;
            fn_801EF62C();
            r3 = 0x1;
            return;
        }
        if (r0 == (u32)0x6) {
            r3 = 0x5;
            fn_801EF62C();
            r3 = 0x1;
            return;
        }
        if (r4 == (u32)0x7) { r3 = 0x0; return; }
        if (r4 == (u32)0x6) {

            r3 = 0x0;
            return;
        }
        if ((r4 == (u32)0x3) && (r0 == (u32)0x2)) {

            r3 = 0x7;
            fn_801EF62C();
            r3 = 0x1;
            return;
        }
        r0 = r31 & 0xFFFF;
        if (r0 == (u32)0x2) {
            r0 = r30 & 0xFFFF;
            if (r0 == (u32)0x3) {
                r3 = 0x7;
                fn_801EF62C();
                r3 = 0x1;
                return;
        }
        }
        r0 = r31 & 0xFFFF;
        if (r0 == (u32)0x4) {
            r0 = r30 & 0xFFFF;
            if (r0 == (u32)0x5) {
                r3 = 0x6;
                fn_801EF62C();
                r3 = 0x1;
                return;
        }
        }
        r0 = r31 & 0xFFFF;
        if (r0 == (u32)0x5) {
            r0 = r30 & 0xFFFF;
            if (r0 == (u32)0x4) {
                r3 = 0x6;
                fn_801EF62C();
                r3 = 0x1;
                return;
        }
        }
        }
    r3 = r30;
    fn_801EF62C();
    r3 = 0x1;

    return;
}

/* 0x801F3B24 | size: 0x90 | medium */
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

    r28 = r3;
    r29 = r4;
    r31 = (u32)sp + 0x8;
    r30 = 0x0;

    while (r0 < (u32)0x8) {
        r3 = r28;
        r6 = r30;
        r4 = 0x0;
        r5 = 0x59;
        fn_801F54A4();
        /* clrlslwi r0, r30, 16, 2 */;
        r30 = r30 + 0x1;
        *(u32*)(r31 + r0) = r3;

    r0 = r30 & 0xFFFF;
    }
    r3 = r28;
    r6 = r29;
    r4 = (u32)sp + 0x8;
    r5 = 0x8;
    fn_801F3BB4();
    r3 = r28;
    r7 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x5a;
    r6 = 0x0;
    fn_801F4C14();
    return;
}

/* 0x801F3BB4 | size: 0x134 | medium */
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

    r29 = r5 & 0xFFFF;
    r23 = r3;
    r24 = r4;
    r25 = r6;
    /* subi r28, r29, 0x1 */;
    r27 = 0x0;
    while (1) {
        r3 = r27 & 0xFFFF;
        if ((s32)r3 >= (s32)r28) break;
        r0 = r3 + 0x1;
        /* clrlslwi r30, r27, 16, 2 */;
        r26 = r0 & 0xFFFF;
        while (1) {
            r0 = r26 & 0xFFFF;
            if (r0 >= (u32)r29) break;
            r3 = *(u32*)(r24 + r30);
            if (r3 == (u32)0x0) {
                /* clrlslwi r0, r26, 16, 2 */;
                r0 = *(u32*)(r24 + r0);
                if (r0 != (u32)0x0) {
                }
                if (r3 == (u32)0x0) {
                    /* clrlslwi r4, r26, 16, 2 */;
                    r0 = *(u32*)(r24 + r4);
                    *(u32*)(r24 + r30) = r0;
                    *(u32*)(r24 + r4) = r3;
                }

            } else {
                /* clrlslwi r31, r26, 16, 2 */;
                r0 = *(u32*)(r24 + r31);
                if (r0 != (u32)0x0) {
                    r0 = r25 & 0xFF;
                    if (r0 == (u32)0x0) {
                        r22 = 0x0;
                        r3 = 0x0;
                    } else {

                        ((void(*)(void))fn_802050F4)();
                        r0 = r3;
                        r3 = *(u32*)(r24 + r31);
                        r22 = r0;
                        ((void(*)(void))fn_802050F4)();
                    }
                    r4 = (s8)r22;
                    r0 = (s8)r3;
                }
                if ((s32)r4 >= (s32)r0) {
                    if ((s32)r4 > (s32)r0) {
                        r3 = *(u32*)(r24 + r30);
                        r0 = *(u32*)(r24 + r31);
                        *(u32*)(r24 + r30) = r0;
                        *(u32*)(r24 + r31) = r3;
                    } else {
                        r4 = *(u32*)(r24 + r30);
                        r3 = r23;
                        r5 = *(u32*)(r24 + r31);
                        r6 = r25;
                        fn_801F3CE8();
                        r0 = r3 & 0xFF;
                        if ((s32)r4 == (s32)r0) {
                            r3 = *(u32*)(r24 + r30);
                            r0 = *(u32*)(r24 + r31);
                            *(u32*)(r24 + r30) = r0;
                            *(u32*)(r24 + r31) = r3;
                        }
                    }
                }
            }
            r26 = r26 + 0x1;

        }
        r27 = r27 + 0x1;

    }
    return;
}

/* 0x801F3CE8 | size: 0x538 | large */
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

    /* mr. r31, r4 */;
    r26 = r3;
    r30 = r5;
    r25 = r6;
    if ((s32)r0 == (s32)0) { r3 = 0x1; return; }
    if (r30 == (u32)0x0) {

        r3 = 0x1;
        return;
    }
    r0 = 0x0;
    r5 = 0xd;
    r4 = (u32)fn_801F3678;
    r4 = (u32)fn_801F3678;
    r5 = (u32)sp + 0x18;
    *(u32*)(sp + 0x1C) = r0;
    r6 = 0x0;
    *(u32*)(sp + 0x20) = r0;
    *(u32*)(sp + 0x24) = r0;
    fn_801F37B0();
    r0 = r0 & 0xFFFF;
    if (r30 != (u32)0x0) {
        r29 = 0x0;

    } else {
        r0 = 0x0;
        r4 = 0x4d;
        r3 = (u32)fn_801F3678;
        r4 = (u32)fn_801F3678;
        r5 = (u32)sp + 0x8;
        *(u32*)(sp + 0xC) = r0;
        r3 = r26;
        r6 = 0x0;
        *(u32*)(sp + 0x10) = r0;
        *(u32*)(sp + 0x14) = r0;
        fn_801F37B0();
        r0 = r0 & 0xFFFF;
        if (r30 != (u32)0x0) {
            r29 = 0x0;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x4e;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x0;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x4f;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x1;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x50;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x2;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x51;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x3;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x52;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x4;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x53;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x1;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x54;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x2;
        } else {
        r3 = r26;
        r4 = 0x0;
        r5 = 0xa;
        r6 = 0x55;
        fn_801F54A4();
        if ((s32)r3 == (s32)0x1) {
            r29 = 0x3;
        } else {
        r29 = 0x0;
        }
        }
        }
        }
        }
        }
        }
        }
        }
    }
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
    if ((s32)r3 == (s32)0x1) {
        r24 = 0x0;

    } else {
        r22 = 0x0;
        while (1) {
            r0 = r22 & 0xFFFF;
            if (r0 >= (u32)0x2) break;
            r3 = r26;
            r6 = r22;
            r4 = 0x0;
            r5 = 0x35;
            fn_801F54A4();
            /* mr. r19, r3 */;
            if ((s32)r3 != (s32)0x1) {
                r20 = 0x0;
                while (1) {
                    r0 = r20 & 0xFFFF;
                    if (r0 >= (u32)0x2) break;
                    r3 = r19;
                    r6 = r20;
                    r4 = 0x0;
                    r5 = 0x7;
                    fn_801F76B8();
                    /* mr. r24, r3 */;
                    if ((s32)r3 != (s32)0x1) {
                        r21 = 0x0;
                        while (1) {
                            r0 = r21 & 0xFFFF;
                            if (r0 >= (u32)0x6) break;
                            r3 = r24;
                            r6 = r21;
                            r4 = 0x0;
                            r5 = 0x45;
                            fn_801FB1C0();
                            if ((u32)r3 != (u32)0x0 && r23 == (u32)r3) {

                                break;
                            }
                            r21 = r21 + 0x1;

                        }
                    }
                    r20 = r20 + 0x1;

                }
            }
            r22 = r22 + 0x1;

        }
        r24 = 0x0;

        if (r24 == (u32)0x0) {
            r24 = 0x0;
        }
    }
    if (r24 != (u32)0x0) {

    r3 = r24;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if (r3 != (u32)0x0) {

        r7 = r3;
    }
    }
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
    if (r3 == (u32)0x0) {
        r19 = 0x0;

    } else {
        r20 = 0x0;
        while (1) {
            r0 = r20 & 0xFFFF;
            if (r0 >= (u32)0x2) break;
            r3 = r26;
            r6 = r20;
            r4 = 0x0;
            r5 = 0x35;
            fn_801F54A4();
            /* mr. r18, r3 */;
            if (r3 != (u32)0x0) {
                r22 = 0x0;
                while (1) {
                    r0 = r22 & 0xFFFF;
                    if (r0 >= (u32)0x2) break;
                    r3 = r18;
                    r6 = r22;
                    r4 = 0x0;
                    r5 = 0x7;
                    fn_801F76B8();
                    /* mr. r19, r3 */;
                    if (r3 != (u32)0x0) {
                        r21 = 0x0;
                        while (1) {
                            r0 = r21 & 0xFFFF;
                            if (r0 >= (u32)0x6) break;
                            r3 = r19;
                            r6 = r21;
                            r4 = 0x0;
                            r5 = 0x45;
                            fn_801FB1C0();
                            if ((u32)r3 != (u32)0x0 && r23 == (u32)r3) {

                                break;
                            }
                            r21 = r21 + 0x1;

                        }
                    }
                    r22 = r22 + 0x1;

                }
            }
            r20 = r20 + 0x1;

        }
        r19 = 0x0;

        if (r19 == (u32)0x0) {
            r19 = 0x0;
        }
    }
    if (r19 != (u32)0x0) {

    r3 = r19;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if (r3 != (u32)0x0) {

        r7 = r3;
    }
    }
    r3 = r30;
    r4 = r27;
    r5 = r29;
    r6 = r28;
    fn_802043D4();
    r0 = r25 & 0xFF;
    r25 = r3;
    if (r3 == (u32)0x0) {
        r18 = 0x0;
        r19 = 0x0;
    } else {

        r3 = r31;
        fn_802051D4();
        r0 = r3;
        r3 = r30;
        r18 = r0;
        fn_802051D4();
        r19 = r3;
    }
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
    if (r3 == (u32)0x0) {
        r0 = (s8)r3;
        if (r3 != (u32)0x0) {
        }
        if ((s32)r18 > (s32)r3) {
            r3 = 0x1;
            return;
        }
        if ((s32)r18 < (s32)r3) {
            r3 = 0x0;
            return;
        }
        }
    if (r24 > r25) {
        r3 = 0x1;
        return;
    }
    if (r24 < r25) {
        r3 = 0x0;
        return;
    }
    fn_800E0C54();
    r0 = r3 & 0x1;
    if (r24 != (u32)r25) {
        r3 = 0x1;
        return;
    }
    r3 = 0x0;

    return;
}

/* 0x801F4220 | size: 0x134 | medium */
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
    r25 = r3;
    r3 = r4;
    r4 = 0x0;
    fn_8012640C();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r30 = 0x0;

    } else {
        r29 = 0x0;
        while (1) {
            r0 = r29 & 0xFFFF;
            if (r0 >= (u32)0x2) break;
            r3 = r25;
            r6 = r29;
            r4 = 0x0;
            r5 = 0x35;
            fn_801F54A4();
            /* mr. r26, r3 */;
            if ((s32)r0 != (s32)0) {
                r27 = 0x0;
                while (1) {
                    r0 = r27 & 0xFFFF;
                    if (r0 >= (u32)0x2) break;
                    r3 = r26;
                    r6 = r27;
                    r4 = 0x0;
                    r5 = 0x7;
                    fn_801F76B8();
                    /* mr. r30, r3 */;
                    if ((s32)r0 != (s32)0) {
                        r28 = 0x0;
                        while (1) {
                            r0 = r28 & 0xFFFF;
                            if (r0 >= (u32)0x6) break;
                            r3 = r30;
                            r6 = r28;
                            r4 = 0x0;
                            r5 = 0x45;
                            fn_801FB1C0();
                            if ((u32)r3 != (u32)0x0 && r31 == (u32)r3) {

                                break;
                            }
                            r28 = r28 + 0x1;

                        }
                    }
                    r27 = r27 + 0x1;

                }
            }
            r29 = r29 + 0x1;

        }
        r30 = 0x0;

        if (r30 == (u32)0x0) {
            r30 = 0x0;
        }
    }
    if (r30 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if (r3 != (u32)0x0) return;
    r3 = 0x0;

    return;
}

/* 0x801F4354 | size: 0x10C | medium */
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
    r25 = r3;
    r3 = r4;
    r4 = 0x0;
    fn_8012640C();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r29 = 0x0;
    while (1) {
        r0 = r29 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r25;
        r6 = r29;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        /* mr. r26, r3 */;
        if ((s32)r0 != (s32)0) {
            r27 = 0x0;
            while (1) {
                r0 = r27 & 0xFFFF;
                if (r0 >= (u32)0x2) break;
                r3 = r26;
                r6 = r27;
                r4 = 0x0;
                r5 = 0x7;
                fn_801F76B8();
                /* mr. r30, r3 */;
                if ((s32)r0 != (s32)0) {
                    r28 = 0x0;
                    while (1) {
                        r0 = r28 & 0xFFFF;
                        if (r0 >= (u32)0x6) break;
                        r3 = r30;
                        r6 = r28;
                        r4 = 0x0;
                        r5 = 0x45;
                        fn_801FB1C0();
                        if ((u32)r3 != (u32)0x0 && r31 == (u32)r3) {

                            break;
                        }
                        r28 = r28 + 0x1;

                    }
                }
                r27 = r27 + 0x1;

            }
        }
        r29 = r29 + 0x1;

    }
    r30 = 0x0;

    if (r30 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;

    return;
}

/* 0x801F4460 | size: 0xDC | medium */
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

    r25 = r3;
    r26 = r4;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r25;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        /* mr. r31, r3 */;
        if ((s32)r0 != (s32)0) {
            r30 = 0x0;
            while (1) {
                r0 = r30 & 0xFFFF;
                if (r0 >= (u32)0x2) break;
                r3 = r31;
                r6 = r30;
                r4 = 0x0;
                r5 = 0x7;
                fn_801F76B8();
                /* mr. r27, r3 */;
                if ((s32)r0 != (s32)0) {
                    r29 = 0x0;
                    while (1) {
                        r0 = r29 & 0xFFFF;
                        if (r0 >= (u32)0x6) break;
                        r3 = r27;
                        r6 = r29;
                        r4 = 0x0;
                        r5 = 0x45;
                        fn_801FB1C0();
                        if (((u32)r3 != (u32)0x0) && (r26 == (u32)r3)) {

                            r3 = r27;
                            return;
                        }
                        r29 = r29 + 0x1;

                    }
                }
                r30 = r30 + 0x1;

            }
        }
        r28 = r28 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x801F4718 | size: 0x9C | medium */
void fn_801F4718(void) {
    extern void fn_801F54A4();
    extern void fn_801F61EC();
    u8 sp[0x10];
    u32 r0 = 0;
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
    if (r3 != (u32)0x0) {
        r5 = 0x0;
        r4 = r5;
        while (1) {
            r0 = r5 & 0xFFFF;
            if (r0 >= (u32)0x8) break;
            /* clrlslwi r0, r5, 16, 2 */;
            r5 = r5 + 0x1;
            *(u32*)(r3 + r0) = r4;

        }
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x5a;
    r6 = 0x0;
    fn_801F54A4();
    /* mr. r4, r3 */;
    if (r0 == (u32)0x8) {
        r3 = 0x0;
    } else {

        r3 = r31;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        fn_801F61EC();
    }
    return;
}

/* 0x801F47B4 | size: 0x50 | small */
void fn_801F47B4(void) {
    extern void fn_801F54A4();
    extern void fn_801F7404();
    u8 sp[0x10];
    u32 r0 = 0;
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
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {

        r3 = r31;
    }
    return;
}

/* 0x801F4804 | size: 0x5C | small */
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
    return;
}

/* 0x801F4860 | size: 0x260 | large */
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

    /* mr. r30, r3 */;
    r31 = r4;
    if ((s32)r0 == (s32)0) return;
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
    r5 = (u32)sp + 0x4;
    /* subi r4, r3, 0x4 */;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u32*)((u8*)r4 + 0x4);
        r0 = *(u32*)((u8*)r4 + 0x8);
        *(u32*)((u8*)r5 + 0x4) = r3;
        r5 += 8; *(u32*)r5 = r0;
    } while (--ctr != 0);
    r0 = *(u16*)((u8*)r4 + 0x4);
    r29 = (u32)sp + 0x8;
    r28 = 0x0;
    *(u16*)((u8*)r5 + 0x4) = r0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if (r0 >= (u32)0xd) break;
        /* clrlslwi r0, r28, 16, 1 */;
        r3 = r30;
        r5 = *(u16*)(r29 + r0);
        r4 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        fn_801F4C14();
        r28 = r28 + 0x1;

    }
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
    if (r3 != (u32)0x0) {
        r5 = 0x0;
        r4 = r5;
        while (1) {
            r0 = r5 & 0xFFFF;
            if (r0 >= (u32)0x8) break;
            /* clrlslwi r0, r5, 16, 2 */;
            r5 = r5 + 0x1;
            *(u32*)(r3 + r0) = r4;

        }
    }
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

    return;
}

/* 0x801F4AC0 | size: 0x154 | medium */
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
    r5 = (u32)sp + 0x4;
    r29 = r3;
    /* subi r4, r4, 0x4 */;
    ctr_fn = (void(*)(void))r0;
    do {
        r3 = *(u32*)((u8*)r4 + 0x4);
        r0 = *(u32*)((u8*)r4 + 0x8);
        *(u32*)((u8*)r5 + 0x4) = r3;
        r5 += 8; *(u32*)r5 = r0;
    } while (--ctr != 0);
    r0 = *(u16*)((u8*)r4 + 0x4);
    r31 = (u32)sp + 0x8;
    r30 = 0x0;
    *(u16*)((u8*)r5 + 0x4) = r0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)0xd) break;
        /* clrlslwi r0, r30, 16, 1 */;
        r3 = r29;
        r5 = *(u16*)(r31 + r0);
        r4 = 0x0;
        r6 = 0x0;
        r7 = 0x0;
        fn_801F4C14();
        r30 = r30 + 0x1;

    }
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
    return;
}

/* 0x801F4C14 | size: 0x890 | massive */
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
    u32 ctr = 0;

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
    if ((s32)r0 == (s32)0) { r3 = 0x0; return; }
    if (r0 >= (u32)0x60) {

        r3 = 0x0;
        return;
    }
    if (r0 < (u32)0x8) {
        r3 = r25;
        ((void(*)(void))fn_801F6738)();
        /* mr. r29, r3 */;
        if (r0 == (u32)0x8) {
            r3 = 0x0;
            return;
        }
        if (r0 < (u32)0x5f && r29 == (u32)0x0) {

            ((void(*)(void))fn_801F6B48)();
            /* mr. r29, r3 */;
            if (r29 == (u32)0x0) {
                r3 = 0x0;
                return;
        }
        }
        }
    r0 = r26 & 0xFFFF;
    if (r0 > (u32)0x5e) { r3 = r31 & 0xFF; return; }
    r3 = (u32)jumptable_80375330;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80375330;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = r29;
    r4 = r30;
    fn_801F65F0();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F667C();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r27 & 0xFF;
    r5 = r30 & 0xFFFF;
    fn_801F6658();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFF;
    fn_801F6648();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F6638();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F6628();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F6618();
    r3 = r31 & 0xFF;
    return;
    r3 = r30 & 0xFFFF;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x9) {
        r3 = 0x0;
    } else {

        r3 = r29;
        r4 = r30 & 0xFFFF;
        fn_8011B444();
    }
    r0 = r3 & 0xFF;
    r31 = r3;
    if (r0 != (u32)0x2) { r3 = r31 & 0xFF; return; }
    r3 = r30 & 0xFFFF;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x9) { r3 = r31 & 0xFF; return; }
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = r27 & 0xFFFF;
    fn_8011B2C0();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F68C8();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F68B8();
    r3 = r31 & 0xFF;
    return;
    if (r30 != (u32)0x0) {
        r3 = r30;
        fn_80206780();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
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
        } else {
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
        }

    } else {
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
    }
    r3 = r29;
    r4 = r30;
    fn_801F68A4();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xda;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdb;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdc;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdd;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xde;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xdf;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe0;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe1;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe2;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe3;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    fn_801F6A98();
    r7 = r30;
    r4 = 0x0;
    r5 = 0xe4;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31 & 0xFF;
    return;
    if (r30 != (u32)0x0) {
        r3 = r30;
        fn_80206780();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
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
        } else {
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
        }

    } else {
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
    }
    r3 = r29;
    r4 = r30;
    fn_801F6890();
    r3 = r31 & 0xFF;
    return;
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
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F687C();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F6868();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F64AC();
    r3 = r31 & 0xFF;
    return;
    if (r30 != (u32)0x0) {
        r3 = r30;
        fn_80206780();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
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
        } else {
        r3 = 0x12;
        r4 = 0x0;
        fn_80132A38();
        r3 = 0x1d;
        r4 = 0x0;
        fn_80132A38();
        }

    } else {
        r3 = 0x12;
        r4 = 0x0;
        fn_80132A38();
        r3 = 0x1d;
        r4 = 0x0;
        fn_80132A38();
    }
    r3 = r29;
    r4 = r30;
    fn_801F647C();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F644C();
    r3 = r31 & 0xFF;
    return;
    if (r30 != (u32)0x0) {
        r3 = r30;
        fn_80206780();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
            r3 = r30;
            fn_802040E8();
            r0 = r3;
            r3 = r29;
            r7 = r0 & 0xFFFF;
            r4 = 0x0;
            r5 = 0x56;
            r6 = 0x0;
            fn_801F4C14();
    }
    }
    r3 = r29;
    r4 = r30;
    fn_801F641C();
    r3 = r31 & 0xFF;
    return;
    r3 = r30;
    fn_801F640C();
    r3 = r31 & 0xFF;
    return;
    if (r30 != (u32)0x0) {
        r3 = r30;
        fn_80206780();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x1) {
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
        } else {
        r3 = 0x1e;
        r4 = 0x0;
        fn_80132A38();
        r3 = 0x1c;
        r4 = 0x0;
        fn_80132A38();
        }

    } else {
        r3 = 0x1e;
        r4 = 0x0;
        fn_80132A38();
        r3 = 0x1c;
        r4 = 0x0;
        fn_80132A38();
    }
    r3 = r29;
    r4 = r30;
    fn_801F6854();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F6840();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F682C();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F6818();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F6804();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F67F0();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F67DC();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F67C8();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F67B4();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F67A0();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F678C();
    r3 = r31 & 0xFF;
    return;
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
    r3 = r31 & 0xFF;
    return;
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
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = (s16)r30;
    fn_801F65C0();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r5 = r30;
    r4 = r27 & 0xFFFF;
    fn_801F6560();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30;
    fn_801F650C();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F64DC();
    r3 = r31 & 0xFF;
    return;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    fn_801F3984();
    r3 = r31 & 0xFF;
    return;
    r4 = r30;
    r3 = 0x2f;
    fn_80132A38();

    r3 = r31 & 0xFF;

    return;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801F54A4(void) {
#include "src/game/pokemon_fn_801F54A4.inc"
}
#else
void fn_801F54A4(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801F61EC | size: 0x220 | large */
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
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r3 = r8;
    while (1) {
        r0 = r8 & 0xFFFF;
        if (r0 >= (u32)0x8) break;
        /* clrlslwi r0, r8, 16, 2 */;
        r8 = r8 + 0x1;
        *(u32*)(r28 + r0) = r3;

    }
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
    if (r0 != (u32)0x8) {
        r3 = r31;
        fn_80206780();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x8) {
            r3 = 0x0;
            return;
        }
        r4 = r31;
        r5 = r17;
        r3 = 0x2;
        fn_801F02AC();
        r24 = r3;
    } else {

        r24 = 0x0;
    }
    r21 = 0x0;
    while (1) {
        r0 = r21 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r27;
        r6 = r21;
        r4 = 0x0;
        r5 = 0x35;
        fn_801F54A4();
        r25 = r3;
        fn_801F7404();
        r0 = r3 & 0xFF;
        if (r0 == (u32)0x8) {
            r25 = 0x0;
        }
        do {
        if (r25 == (u32)0x0) break;
        r0 = r30 & 0xFF;
        if (r0 == (u32)0x1) {
            if (r24 != (u32)r25) break;

        } else {
            if (r0 == (u32)0x2) {
                if (r24 == (u32)r25) break;
            }
        }
        r23 = 0x0;
        while (1) {
            r0 = r23 & 0xFFFF;
            if (r0 >= (u32)r20) break;
            r3 = r25;
            r4 = r23;
            fn_801F7258();
            /* mr. r17, r3 */;
            if (r24 != (u32)r25) {
                r22 = 0x0;
                while (1) {
                    r0 = r22 & 0xFFFF;
                    if (r0 >= (u32)r19) break;
                    r3 = r17;
                    r4 = r22;
                    fn_801F981C();
                    /* mr. r26, r3 */;
                    do {
                    if (r24 == (u32)r25) break;
                    r0 = r29 & 0xFF;
                    if (r0 == (u32)0x1) {
                        fn_802062FC();
                        r0 = r3 & 0xFF;
                        if (r0 == (u32)0x1) break;
                    }
                    r0 = r30 & 0xFF;
                    if (r0 == (u32)0x3) {
                        if (r26 == (u32)r31) break;
                    }
                    r4 = 0x0;
                    while (1) {
                        r0 = r4 & 0xFFFF;
                        if (r0 >= (u32)0x8) break;
                        /* clrlslwi r3, r4, 16, 2 */;
                        r0 = *(u32*)(r28 + r3);
                        if (r0 == (u32)0x0) {
                            *(u32*)(r28 + r3) = r26;
                            r0 = (s16)r4;
                            break;
                        }
                        r4 = r4 + 0x1;

                    }
                    r0 = -0x1;

                    r0 = (s16)r0;
                    if (r0 < (u32)0x8) break;
                    r18 = r18 + 0x1;
                    } while (0);
                    r22 = r22 + 0x1;

                }
            }
            r23 = r23 + 0x1;

        }
        } while (0);
        r21 = r21 + 0x1;

    }
    r3 = r18;

    return;
}

/* 0x801F650C | size: 0x38 | small */
void fn_801F650C(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    if (r3 == (u32)0x0) return;
    r7 = 0x0;

    while (r0 < (u32)0x8) {
        /* clrlslwi r6, r7, 16, 2 */;
        r7 = r7 + 0x1;
        r5 = r6 + (0x1 << 16);
        r0 = *(u32*)(r4 + r6);
        /* subi r5, r5, 0x5b3c */;
        *(u32*)(r3 + r5) = r0;

    r0 = r7 & 0xFFFF;
    }
    return;
}

/* 0x801F6544 | size: 0x1C */
u8* fn_801F6544(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return ptr + 0xA4C4;
}

/* 0x801F6560 | size: 0x28 */
void fn_801F6560(u8* ptr, u16 idx, u32 val) {
    u32* base;
    if (ptr == NULL) { return; }
    if (idx >= 8) { return; }
    base = (u32*)(ptr + 0xA4C4);
    base[idx] = val;
}

/* 0x801F6588 | size: 0x38 */
u32 fn_801F6588(u8* ptr, u16 idx) {
    u32* base;
    if (ptr == NULL) { return 0; }
    if (idx >= 8) { return 0; }
    base = (u32*)(ptr + 0xA4C4);
    return base[idx];
}

/* 0x801F6658 | size: 0x24 */
void fn_801F6658(u8* ptr, u8 idx, u16 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr += idx * 2;
    *(u16*)(ptr + 0x14) = val;
}

/* 0x801F66EC | size: 0x34 */
u32 fn_801F66EC(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    ptr += idx * 2;
    return *(u16*)(ptr + 0x14);
}

/* 0x801F6AB4 | size: 0x34 */
u8* fn_801F6AB4(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 2) { return NULL; }
    return ptr + idx * 0x5230 + 0x14;
}

/* 0x801F6B18 | size: 0x30 */
u8* fn_801F6B18(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 1) { return NULL; }
    return ptr + idx * 0x10;
}

/* 0x801F6B54 | size: 0xF8 | medium */
void fn_801F6B54(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5) {
    extern u32 fn_801F78D4(u32, u32);
    extern s8 fn_801FA634(u32);
    extern u32 fn_801FB1C0(u32, u32, u16, u32);
    extern s8 fn_802062FC(u32);
    extern u32 fn_8012640C(u32, u32, u16, u32);
    extern void fn_801FAA58(u32, u32, u16, u32);
    u32 uVar1;
    s8 cVar4;
    u32 uVar2;
    s16 sVar3;
    u32 uVar5;
    u32 uVar6;

    for (uVar6 = 0; (uVar6 & 0xFFFF) < (param_4 & 0xFFFF); uVar6 = uVar6 + 1) {
        if (param_1 == 0) {
            uVar1 = 0;
        } else {
            uVar1 = fn_801F78D4(param_1, uVar6);
        }
        cVar4 = fn_801FA634(uVar1);
        if (cVar4 != 0) {
            uVar5 = 0;
            while ((param_5 & 0xFFFF) > (uVar5 & 0xFFFF)) {
                uVar2 = fn_801FB1C0(uVar1, 0, 0x46, uVar5);
                cVar4 = fn_802062FC(uVar2);
                if (cVar4 != 0) {
                    uVar2 = fn_8012640C(uVar2, 0, 0xD5, 0);
                    sVar3 = fn_8012640C(uVar2, 0, 0xCE, 0);
                    if (sVar3 >= 0) {
                        fn_801FAA58(param_2, 0, 0x57, 0);
                    }
                }
                uVar5 = uVar5 + 1;
            }
        }
    }
}

/* 0x801F6C4C | size: 0x54 | small */
u32 fn_801F6C4C(u32 param_1, u32 param_2) {
    extern s16 fn_80119ED0(u32);
    extern u32 fn_8011A860(u32, u32);
    s16 sVar2;
    u32 uVar1;

    sVar2 = fn_80119ED0(param_2);
    if (sVar2 == 6) {
        uVar1 = fn_8011A860(param_1, param_2);
    } else {
        uVar1 = 0;
    }
    return uVar1;
}

/* 0x801F6CA0 | size: 0x54 | small */
void fn_801F6CA0(u32 param_1, u32 param_2, u32 param_3) {
    extern s16 fn_80119ED0(u32);
    extern void fn_8011AB50(u32, u32, u32);
    s16 sVar1;

    sVar1 = fn_80119ED0(param_2);
    if (sVar1 == 6) {
        fn_8011AB50(param_1, param_2, param_3);
    }
}

/* 0x801F6CF4 | size: 0x54 | small */
u32 fn_801F6CF4(u32 param_1, u32 param_2) {
    extern s16 fn_80119ED0(u32);
    extern u32 fn_8011ACB4(u32, u32);
    s16 sVar2;
    u32 uVar1;

    sVar2 = fn_80119ED0(param_2);
    if (sVar2 == 6) {
        uVar1 = fn_8011ACB4(param_1, param_2);
    } else {
        uVar1 = 0xFFFFFFFF;
    }
    return uVar1;
}

/* 0x801F6D48 | size: 0x54 | small */
u32 fn_801F6D48(u32 param_1, u32 param_2) {
    extern s16 fn_80119ED0(u32);
    extern u32 fn_8011AE40(u32, u32);
    s16 sVar2;
    u32 uVar1;

    sVar2 = fn_80119ED0(param_2);
    if (sVar2 == 6) {
        uVar1 = fn_8011AE40(param_1, param_2);
    } else {
        uVar1 = 0xFFFFFFFF;
    }
    return uVar1;
}

/* 0x801F6D9C | size: 0x54 | small */
u32 fn_801F6D9C(u32 param_1, u32 param_2) {
    extern s16 fn_80119ED0(u32);
    extern u32 fn_8011B130(u32, u32);
    s16 sVar2;
    u32 uVar1;

    sVar2 = fn_80119ED0(param_2);
    if (sVar2 == 6) {
        uVar1 = fn_8011B130(param_1, param_2);
    } else {
        uVar1 = 0xFFFFFFFF;
    }
    return uVar1;
}

/* 0x801F6DF0 | size: 0x54 | small */
void fn_801F6DF0(u32 param_1, u32 param_2, u32 param_3) {
    extern s16 fn_80119ED0(u32);
    extern void fn_8011B2C0(u32, u32, u32);
    s16 sVar1;

    sVar1 = fn_80119ED0(param_2);
    if (sVar1 == 6) {
        fn_8011B2C0(param_1, param_2, param_3);
    }
}

/* 0x801F6E44 | size: 0x54 | small */
u32 fn_801F6E44(u32 param_1, u32 param_2) {
    extern s16 fn_80119ED0(u32);
    extern u32 fn_8011B444(u32, u32);
    s16 sVar2;
    u32 uVar1;

    sVar2 = fn_80119ED0(param_2);
    if (sVar2 == 6) {
        uVar1 = fn_8011B444(param_1, param_2);
    } else {
        uVar1 = 0;
    }
    return uVar1;
}

/* 0x801F6E98 | size: 0x54 | small */
u32 fn_801F6E98(u32 param_1, u32 param_2) {
    extern s16 fn_80119ED0(u32);
    extern u32 fn_8011B67C(u32, u32);
    s16 sVar2;
    u32 uVar1;

    sVar2 = fn_80119ED0(param_2);
    if (sVar2 == 6) {
        uVar1 = fn_8011B67C(param_1, param_2);
    } else {
        uVar1 = 0;
    }
    return uVar1;
}

/* 0x801F6EEC | size: 0x4C | small */
void fn_801F6EEC(u32 param_1, u32 param_2) {
    extern s16 fn_80119ED0(u32);
    extern void fn_8011B788(u32, u32);
    s16 sVar1;

    sVar1 = fn_80119ED0(param_2);
    if (sVar1 == 6) {
        fn_8011B788(param_1, param_2);
    }
}

/* 0x801F6F38 | size: 0x9C | medium */
s16 fn_801F6F38(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    extern u32 fn_801F78D4(u32, u32);
    extern s16 fn_801F81F8(u32, u32, u32);
    extern s8 fn_801FA634(u32);
    s16 sVar1;
    s8 cVar3;
    s16 sVar2;
    u32 uVar4;
    u32 uVar5;

    sVar1 = 0;
    for (uVar4 = 0; (uVar4 & 0xFFFF) < (param_2 & 0xFFFF); uVar4 = uVar4 + 1) {
        if (param_1 == 0) {
            uVar5 = 0;
        } else {
            uVar5 = fn_801F78D4(param_1, uVar4);
        }
        cVar3 = fn_801FA634(uVar5);
        if (cVar3 != 0) {
            sVar2 = fn_801F81F8(uVar5, param_3, param_4);
            sVar1 = sVar1 + sVar2;
        }
    }
    return sVar1;
}

/* 0x801F6FD4 | size: 0xBC | medium */
s16 fn_801F6FD4(u32 param_1, u32 param_2, u32 param_3) {
    extern u32 fn_801F78D4(u32, u32);
    extern u32 fn_801F986C(u32, u32);
    extern s8 fn_801FA634(u32);
    extern s8 fn_80206608(u32);
    u32 uVar1;
    s8 cVar3;
    u32 iVar2;
    u32 uVar4;
    u32 uVar5;
    s16 sVar6;

    sVar6 = 0;
    for (uVar4 = 0; (uVar4 & 0xFFFF) < (param_2 & 0xFFFF); uVar4 = uVar4 + 1) {
        if (param_1 == 0) {
            uVar1 = 0;
        } else {
            uVar1 = fn_801F78D4(param_1, uVar4);
        }
        cVar3 = fn_801FA634(uVar1);
        if (cVar3 != 0) {
            for (uVar5 = 0; (uVar5 & 0xFFFF) < (param_3 & 0xFFFF); uVar5 = uVar5 + 1) {
                iVar2 = fn_801F986C(uVar1, uVar5);
                if ((iVar2 != 0) && (cVar3 = fn_80206608(iVar2), cVar3 != 0)) {
                    sVar6 = sVar6 + 1;
                }
            }
        }
    }
    return sVar6;
}

/* 0x801F7090 | size: 0xE4 | medium */
s32 fn_801F7090(u32 param_1, u32 param_2, u32 param_3) {
    extern u32 fn_801F78D4(u32, u32);
    extern u32 fn_801F986C(u32, u32);
    extern s8 fn_801FA634(u32);
    extern u32 fn_80205BE8(u32);
    extern s8 fn_80206608(u32);
    extern u32 fn_8012640C(u32, u32, u16, u32);
    u32 uVar1;
    s8 cVar5;
    u32 iVar2;
    u32 iVar3;
    u32 uVar4;
    u32 uVar6;
    u32 uVar7;
    s32 iVar8;

    iVar8 = 0;
    for (uVar7 = 0; (uVar7 & 0xFFFF) < (param_2 & 0xFFFF); uVar7 = uVar7 + 1) {
        if (param_1 == 0) {
            uVar1 = 0;
        } else {
            uVar1 = fn_801F78D4(param_1, uVar7);
        }
        cVar5 = fn_801FA634(uVar1);
        if (cVar5 != 0) {
            uVar6 = 0;
            while ((param_3 & 0xFFFF) > (uVar6 & 0xFFFF)) {
                iVar2 = fn_801F986C(uVar1, uVar6);
                if (iVar2 != 0) {
                    iVar3 = fn_80205BE8(iVar2);
                    if ((iVar3 != 0) && (cVar5 = fn_80206608(iVar2), cVar5 != 0)) {
                        uVar4 = fn_8012640C(iVar3, 0, 0x87, 0);
                        iVar8 = iVar8 + (uVar4 & 0xFFFF);
                    }
                }
                uVar6 = uVar6 + 1;
            }
        }
    }
    return iVar8;
}

/* 0x801F7174 | size: 0xE4 | medium */
s32 fn_801F7174(u32 param_1, u32 param_2, u32 param_3) {
    extern u32 fn_801F78D4(u32, u32);
    extern u32 fn_801F986C(u32, u32);
    extern s8 fn_801FA634(u32);
    extern u32 fn_80205BE8(u32);
    extern s8 fn_80206608(u32);
    extern u32 fn_8012640C(u32, u32, u16, u32);
    u32 uVar1;
    s8 cVar5;
    u32 iVar2;
    u32 iVar3;
    u32 uVar4;
    u32 uVar6;
    u32 uVar7;
    s32 iVar8;

    iVar8 = 0;
    for (uVar7 = 0; (uVar7 & 0xFFFF) < (param_2 & 0xFFFF); uVar7 = uVar7 + 1) {
        if (param_1 == 0) {
            uVar1 = 0;
        } else {
            uVar1 = fn_801F78D4(param_1, uVar7);
        }
        cVar5 = fn_801FA634(uVar1);
        if (cVar5 != 0) {
            uVar6 = 0;
            while ((param_3 & 0xFFFF) > (uVar6 & 0xFFFF)) {
                iVar2 = fn_801F986C(uVar1, uVar6);
                if (iVar2 != 0) {
                    iVar3 = fn_80205BE8(iVar2);
                    if ((iVar3 != 0) && (cVar5 = fn_80206608(iVar2), cVar5 != 0)) {
                        uVar4 = fn_8012640C(iVar3, 0, 0x83, 0);
                        iVar8 = iVar8 + (uVar4 & 0xFFFF);
                    }
                }
                uVar6 = uVar6 + 1;
            }
        }
    }
    return iVar8;
}

/* 0x801F7258 | size: 0x58 | small */
u32 fn_801F7258(u32 param_1) {
    extern u32 fn_801F78D4(u32, u32);
    extern s8 fn_801FA634(u32);
    s8 cVar1;
    u32 uVar2;

    if (param_1 == 0) {
        uVar2 = 0;
    } else {
        uVar2 = fn_801F78D4(param_1, 0);
    }
    cVar1 = fn_801FA634(uVar2);
    if (cVar1 == 0) {
        uVar2 = 0;
    }
    return uVar2;
}

/* 0x801F72B0 | size: 0xD8 | medium */
void fn_801F72B0(u32 param_1, s16 param_2, s16 param_3, s8* param_4, u8* param_5) {
    extern u32 fn_801F7870(u32);
    extern s8 fn_801F7858(u32);
    s32 iVar1;
    s8 cVar2;

    if (param_4 == NULL) return;
    if (param_5 == NULL) return;
    iVar1 = fn_801F7870(param_1);
    if (iVar1 == 0) {
        cVar2 = 0;
    } else {
        cVar2 = fn_801F7858(iVar1);
    }
    *param_4 = cVar2;
    *param_5 = 0;
    if ((param_2 != 1) && (param_2 == 2)) {
        if (*param_4 == 1) {
            if (param_3 == 0) {
                *param_5 = 1;
            } else if (param_3 == 1) {
                *param_5 = 0xFF;
            }
        } else {
            if (param_3 == 0) {
                *param_5 = 0xFF;
            } else if (param_3 == 1) {
                *param_5 = 1;
            }
        }
    }
}

/* 0x801F7388 | size: 0x7C | small */
s8 fn_801F7388(u32 param_1) {
    extern u32 fn_801F78D4(u32, u32);
    extern s8 fn_801FA524(u32);
    u32 uVar2;
    s8 cVar3;
    s8 cVar1;
    u8 bVar4;

    cVar1 = 0;
    for (bVar4 = 0; bVar4 < 2; bVar4 = bVar4 + 1) {
        if (param_1 == 0) {
            uVar2 = 0;
        } else {
            uVar2 = fn_801F78D4(param_1, bVar4);
        }
        cVar3 = fn_801FA524(uVar2);
        if (cVar3 != 0) {
            cVar1 = cVar1 + 1;
        }
    }
    return cVar1;
}

/* 0x801F7404 | size: 0x7C | small */
u32 fn_801F7404(u32 param_1) {
    extern s16 fn_801EF634(void);
    extern s16 fn_801F793C(u32);
    u32 uVar1;
    s16 sVar2;

    if (param_1 == 0) {
        uVar1 = 0;
    } else {
        sVar2 = fn_801EF634();
        if (sVar2 == 1) {
            uVar1 = 0;
        } else {
            if (param_1 == 0) {
                sVar2 = 0;
            } else {
                sVar2 = fn_801F793C(param_1);
            }
            if (sVar2 == 0) {
                uVar1 = 0;
            } else {
                uVar1 = 1;
            }
        }
    }
    return uVar1;
}

/* 0x801F7480 | size: 0xB0 | medium */
void fn_801F7480(u32 param_1, u16 param_2) {
    extern void fn_8011B950(u32, u32);
    extern void fn_801F789C(u32, u32);
    extern void fn_801F78AC(u32, u16);
    extern u32 fn_801F78D4(u32, u32);
    extern u32 fn_801F7908(u32, u32);
    extern void fn_801FA8CC(u32, u32);
    u32 uVar1;

    if (param_1 != 0) {
        fn_801F78AC(param_1, 0);
        uVar1 = fn_801F7908(param_1, 0);
        fn_8011B950(uVar1, 6);
        if (param_1 == 0) {
            uVar1 = 0;
        } else {
            uVar1 = fn_801F78D4(param_1, 0);
        }
        fn_801FA8CC(uVar1, 2);
        if (param_1 != 0) {
            fn_801F789C(param_1, 0);
            fn_801F78AC(param_1, param_2);
        }
    }
}

/* 0x801F7530 | size: 0xC8 | medium */
void fn_801F7530(u32 param_1, u16 param_2) {
    extern void fn_8011B950(u32, u32);
    extern void fn_801F789C(u32, u32);
    extern void fn_801F78AC(u32, u16);
    extern u32 fn_801F78D4(u32, u32);
    extern u32 fn_801F7908(u32, u32);
    extern void fn_801FA8CC(u32, u32);
    u32 uVar1;
    u16 uVar2;
    s32 iVar3;

    if (param_1 != 0) {
        for (uVar2 = 0; uVar2 < param_2; uVar2 = uVar2 + 1) {
            iVar3 = param_1 + (u32)uVar2 * 0x5230;
            if (iVar3 != 0) {
                if (iVar3 == 0) {
                    uVar1 = 0;
                } else {
                    fn_801F78AC(iVar3, 0);
                    uVar1 = fn_801F7908(iVar3, 0);
                }
                fn_8011B950(uVar1, 6);
                if (iVar3 == 0) {
                    uVar1 = 0;
                } else {
                    uVar1 = fn_801F78D4(iVar3, 0);
                }
                fn_801FA8CC(uVar1, 2);
                if (iVar3 != 0) {
                    fn_801F789C(iVar3, 0);
                }
            }
        }
    }
}

/* 0x801F75F8 | size: 0xC0 | medium */
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
    u32 ctr = 0;

    r0 = r5 & 0xFFFF;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    if ((s32)r0 == (s32)0) return;
    if (r0 >= (u32)0xa) {
        return;
    }
    if (r0 < (u32)0x4) {
        r3 = r4;
        fn_801F7870();
    }
    if (r3 == (u32)0x0) return;
    r0 = r29 & 0xFFFF;
    if (r0 > (u32)0x8) return;
    r4 = (u32)jumptable_80375628;
    r0 = r0 << 2;
    r4 = (u32)jumptable_80375628;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = r31 & 0xFFFF;
    fn_801F77E0();
    return;
    r5 = r31;
    r4 = r30 & 0xFF;
    fn_801F77BC();
    return;
    r5 = r31;
    r4 = r30 & 0xFF;
    fn_801F7798();
    return;
    r4 = r31 & 0xFFFF;
    fn_801F78AC();
    return;
    r4 = r31 & 0xFF;
    fn_801F789C();

    return;
}

/* 0x801F76B8 | size: 0xE0 | medium */
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
    u32 ctr = 0;

    r0 = r5 & 0xFFFF;
    r30 = r5;
    r31 = r6;
    if ((s32)r0 == (s32)0) { r3 = 0x0; return; }
    if (r0 >= (u32)0xa) {

        r3 = 0x0;
        return;
    }
    if (r0 < (u32)0x4) {
        r3 = r4;
        fn_801F7870();
    }
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = r30 & 0xFFFF;
    if (r0 <= (u32)0x8) {
        r4 = (u32)jumptable_8037564C;
        r0 = r0 << 2;
        r4 = (u32)jumptable_8037564C;
        r0 = *(u32*)(r4 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        fn_801F7858();
        r3 = r3 & 0xFFFF;
        return;
        r4 = r31 & 0xFF;
        fn_801F7824();
        return;
        r4 = r31 & 0xFF;
        fn_801F77F0();
        return;
        fn_801F793C();
        r3 = r3 & 0xFFFF;
        return;
        r4 = r31;
        fn_801F7908();
        return;
        r4 = r31;
        fn_801F78D4();
        return;
        fn_801F78BC();
        r3 = r3 & 0xFF;
        return;
    }
    r3 = 0x0;

    return;
}

/* 0x801F7798 | size: 0x24 | small */
void fn_801F7798(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if (r3 == (u32)0x0) return;
    r0 = r4 & 0xFF;
    if (r0 >= (u32)0x2) return;
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0xC) = r5;
    return;
}

/* 0x801F77BC | size: 0x24 | small */
void fn_801F77BC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if (r3 == (u32)0x0) return;
    r0 = r4 & 0xFF;
    if (r0 >= (u32)0x2) return;
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x4) = r5;
    return;
}

/* 0x801F77F0 | size: 0x34 | small */
void fn_801F77F0(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = r4 & 0xFF;
    if (r0 >= (u32)0x2) {
        r3 = 0x0;
        return;
    }
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    return;
}

/* 0x801F7824 | size: 0x34 | small */
void fn_801F7824(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r0 = r4 & 0xFF;
    if (r0 >= (u32)0x2) {
        r3 = 0x0;
        return;
    }
    /* clrlslwi r0, r4, 24, 2 */;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0x4);
    return;
}

/* 0x801F7870 | size: 0x2C | small */
void fn_801F7870(void) {
    extern u32 lbl_80478F38;
    extern u32 lbl_80478F3C;
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = lbl_80478F38;
    r3 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if (r3 >= (u32)r0) {
        r3 = 0x0;
        return;
    }
    r0 = r3 * 0x14;
    r3 = lbl_80478F3C;
    r3 = r3 + r0;
    return;
}

/* 0x801F78D4 | size: 0x34 | small */
void fn_801F78D4(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    /* mr. r5, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r0 = r4 & 0xFFFF;
    if (r0 >= (u32)0x2) {
        r3 = 0x0;
        return;
    }
    r3 = r0 * 0x28e4;
    r3 = r3 + 0x64;
    r3 = r5 + r3;
    return;
}

/* 0x801F7908 | size: 0x34 | small */
void fn_801F7908(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    /* mr. r5, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r0 = r4 & 0xFFFF;
    if (r0 >= (u32)0x6) {
        r3 = 0x0;
        return;
    }
    /* clrlslwi r3, r4, 16, 4 */;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
    return;
}

/* 0x801F7954 | size: 0x21C | large */
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

    /* mr. r28, r3 */;
    r29 = r4;
    if ((s32)r0 == (s32)0) return;
    if ((s32)r0 != (s32)0) {

    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if (r0 != (u32)0x1) {

        r3 = r28;
        r4 = 0x0;
        r5 = 0x43;
        r6 = 0x0;
        fn_801FB1C0();
    if ((s32)r3 != (s32)0x0) {

            r3 = r28;
            r4 = 0x0;
            r5 = 0x44;
            r6 = 0x0;
            fn_801FB1C0();
    if (r3 != (u32)0x0) {

                fn_8012A130();
                r0 = r3 & 0xFF;
    if (r3 != (u32)0x0) {

                    r0 = 0x1;
    }
    }
    }
    }
    }
    r0 = r0 & 0xFF;
    if (r3 == (u32)0x0) return;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    r30 = r3;
    r4 = 0x0;
    r3 = 0x0;
    while (1) {
        r0 = r4 & 0xFFFF;
        if (r0 >= (u32)0x6) break;
        r0 = r4 & 0xFFFF;
        r4 = r4 + 0x1;
        *(u8*)(r29 + r0) = r3;

    }
    r31 = 0x0;
    while (1) {
        r0 = r31 & 0xFFFF;
        if (r0 >= (u32)0x6) break;
        r3 = r30;
        r5 = r31 & 0xFFFF;
        r4 = 0x3;
        fn_8012A5B0();
        r27 = r3;
        fn_80123FBC();
        r0 = r3 & 0xFF;
        do {
        if (r0 == (u32)0x6) break;
        r25 = 0x0;
        while (1) {
            r0 = r25 & 0xFFFF;
            if (r0 >= (u32)0x6) break;
            r3 = r28;
            r6 = r25;
            r4 = 0x0;
            r5 = 0x45;
            fn_801FB1C0();
            r26 = r3;
            fn_80206A04();
            r0 = r3 & 0xFF;
            if (r0 != (u32)0x6) {
                r3 = r26;
                r4 = 0x0;
                r5 = 0xcb;
                r6 = 0x0;
                fn_8012640C();
                if ((u32)r3 != (u32)0x0 && r27 == (u32)r3) {

                    break;
            }
            }
            r25 = r25 + 0x1;

        }
        r26 = 0x0;

        if (r26 == (u32)0x0) break;
        r3 = r26;
        fn_80206A04();
        r0 = r3 & 0xFF;
        if (r26 == (u32)0x0) break;
        r3 = r26;
        r4 = 0x0;
        r5 = 0xd2;
        r6 = 0x0;
        fn_8012640C();
        if ((s32)r3 == (s32)0x1) break;
        r3 = r26;
        fn_80206608();
        r0 = r3 & 0xFF;
        if ((s32)r3 == (s32)0x1) {
            r0 = r31 & 0xFFFF;
            r3 = 0x3;
            *(u8*)(r29 + r0) = r3;
            break;
        }
        r3 = r26;
        fn_80205BE8();
        fn_80122DDC();
        r0 = r3 & 0xFF;
        if ((s32)r3 == (s32)0x1) {
            r0 = r31 & 0xFFFF;
            r3 = 0x2;
            *(u8*)(r29 + r0) = r3;
            break;
        }
        r0 = r31 & 0xFFFF;
        r3 = 0x1;
        *(u8*)(r29 + r0) = r3;
        } while (0);
        r31 = r31 + 0x1;

    }

    return;
}

/* 0x801F7B70 | size: 0xE4 | medium */
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
    fn_801FB1C0();
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r5 = (u32)sp + 0x8;
    r4 = 0x1;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    fn_80129BC8();
    /* mr. r31, r3 */;
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r29 = 0x0;
    while (1) {
        r0 = *(u16*)(sp + 0x8);
        r3 = r29 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        /* clrlslwi r0, r29, 16, 2 */;
        r30 = r31 + r0;
        r3 = r30;
        fn_801429E8();
        r0 = r3 & 0xFF;
        if (r3 != (u32)0x0) {
            r3 = r30;
            r4 = 0x0;
            r5 = 0x1b;
            r6 = 0x0;
            fn_80142CF4();
            r0 = r3 & 0xFFFF;
            if (r3 != (u32)0x0) {
                r3 = r30;
                r4 = 0x0;
                r5 = 0x1c;
                r6 = 0x0;
                fn_80142CF4();
                r0 = r3 & 0xFFFF;
                if (r3 != (u32)0x0) {
                    r3 = 0x1;
                    return;
        }
        }
        }
        r29 = r29 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x801F7C54 | size: 0x20C | large */
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

    r24 = r4;
    r25 = r5;
    r26 = r6;
    r27 = r3;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r5 = (u32)sp + 0x8;
    r4 = 0x2;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    fn_80129BC8();
    /* mr. r31, r3 */;
    if (r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r5 = 0x0;
    r3 = (u32)sp + 0xc;
    r4 = r5;
    while (1) {
        r0 = r5 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        /* clrlslwi r0, r5, 16, 1 */;
        r5 = r5 + 0x1;
        *(u16*)(r3 + r0) = r4;

    }
    r30 = 0x0;
    r28 = (u32)sp + 0xc;
    r23 = r30;
    while (1) {
        r0 = r23 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r27;
        r6 = r23;
        r4 = 0x0;
        r5 = 0x46;
        fn_801FB1C0();
        r29 = r3;
        fn_802062FC();
        r0 = r3 & 0xFF;
        if (r0 != (u32)0x2) {
            r3 = r29;
            fn_80204C08();
            r0 = r3 & 0xFFFF;
            if (r0 != (u32)0x2) {
                /* clrlslwi r0, r30, 16, 1 */;
                r30 = r30 + 0x1;
                *(u16*)(r28 + r0) = r3;
        }
        }
        r23 = r23 + 0x1;

    }
    r0 = r25 & 0xFFFF;
    r5 = 0x0;
    r4 = 0x0;
    while (1) {
        r3 = r5 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        /* clrlslwi r3, r5, 16, 1 */;
        r5 = r5 + 0x1;
        *(u16*)(r24 + r3) = r4;

    }
    r28 = 0x0;
    r29 = 0x0;
    while (1) {
        r0 = *(u16*)(sp + 0x8);
        r3 = r29 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        /* clrlslwi r0, r29, 16, 2 */;
        r23 = r31 + r0;
        r3 = r23;
        fn_801429E8();
        r0 = r3 & 0xFF;
        do {
        if (r3 == (u32)r0) break;
        r3 = r23;
        r4 = 0x0;
        r5 = 0x1b;
        r6 = 0x0;
        fn_80142CF4();
        r27 = r3 & 0xFFFF;
        if (r3 == (u32)r0) break;
        r3 = r23;
        r4 = 0x0;
        r5 = 0x1c;
        r6 = 0x0;
        fn_80142CF4();
        r5 = r3 & 0xFFFF;
        if (r3 == (u32)r0) break;
        r0 = r26 & 0xFF;
        if (r0 == (u32)0x1) {
            r4 = (u32)sp + 0xc;
            r0 = r30 & 0xFFFF;
            r6 = 0x0;
            while (1) {
                r3 = r6 & 0xFFFF;
                if (r3 >= (u32)r0) break;
                /* clrlslwi r3, r6, 16, 1 */;
                r3 = *(u16*)(r4 + r3);
                if (r27 == (u32)r3) {
                    r3 = r5 & 0xFFFF;
                    if (r27 != (u32)r3) {
                        /* subi r5, r5, 0x1 */;
                }
                }
                r6 = r6 + 0x1;

            }
            r0 = r5 & 0xFFFF;
            if (r3 == (u32)r0) break;
        }
        r3 = r28 & 0xFFFF;
        r0 = r25 & 0xFFFF;
        if (r3 >= (u32)r0) break;
        /* clrlslwi r0, r28, 16, 1 */;
        r28 = r28 + 0x1;
        *(u16*)(r24 + r0) = r27;
        } while (0);
        r29 = r29 + 0x1;

    }
    r3 = r28;

    return;
}

/* 0x801F7E60 | size: 0x90 | medium */
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

    r29 = r3;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r29;
        r6 = r30;
        r4 = 0x0;
        r5 = 0x46;
        fn_801FB1C0();
        r31 = r3;
        fn_802062FC();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r31;
            r4 = 0x1;
            r5 = 0x0;
            r6 = 0x0;
            fn_80204A5C();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = 0x1;
                return;
        }
        }
        r30 = r30 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x801F7EF0 | size: 0x90 | medium */
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

    r29 = r3;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r29;
        r6 = r30;
        r4 = 0x0;
        r5 = 0x46;
        fn_801FB1C0();
        r31 = r3;
        fn_802062FC();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r31;
            r4 = 0x1;
            r5 = 0x1;
            r6 = 0x0;
            fn_80204A5C();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x1) {
                r3 = 0x1;
                return;
        }
        }
        r30 = r30 + 0x1;

    }
    r3 = 0x0;

    return;
}
