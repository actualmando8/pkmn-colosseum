/**
 * @file pokemon.h
 * @brief Pokemon data structures and core accessor API for Pokemon Colosseum.
 *
 * This header defines the runtime Pokemon data structure and the primary
 * API for reading/writing Pokemon fields. The three core functions --
 * PokemonGet, PokemonSet, and PokemonDataGet -- are among the most heavily
 * called in the entire codebase (510, 223, and 883 calls respectively).
 *
 * The runtime Pokemon structure is ~0xA4C0 bytes (a single Pokemon in
 * party context). Fields are accessed at large offsets using the
 * `addis r3, r3, 0x1; lwz r3, -0x5bXX(r3)` pattern, meaning the actual
 * field offsets are in the 0xA480-0xA4F0 range within the structure.
 *
 * Subsystem map (0x801F000C - 0x801F7F80, ~145 functions):
 *   0x801F000C - 0x801F025C : Scene/party iteration helpers (3 funcs)
 *   0x801F025C - 0x801F0718 : Party slot lookup via PokemonGet/Set dispatch
 *   0x801F0718 - 0x801F4C14 : Pokemon manipulation (sorting, copying, init)
 *   0x801F4C14 - 0x801F54A4 : PokemonSet (jumptable_80375330, 0x5E entries)
 *   0x801F54A4 - 0x801F61BC : PokemonGet (jumptable_803754AC, 0x5D entries)
 *   0x801F61BC - 0x801F6B54 : Field-level getter/setter pairs (~88 funcs)
 *   0x801F6B54 - 0x801F7F80 : Stat calculation helpers
 */

#ifndef GAME_POKEMON_H
#define GAME_POKEMON_H

#include "dolphin/types.h"

/* =========================================================================
 * Constants
 * ========================================================================= */

/* Maximum number of Pokemon in a party */
#define PARTY_MAX_POKEMON    6

/* Pokemon field IDs used by PokemonGet/PokemonSet dispatch.
 * These are the r5 argument values passed to fn_801F54A4 / fn_801F4C14.
 * The values 0x00-0x5D map into the jumptables at 803754AC / 80375330. */
#define POKE_FIELD_SPECIES        0x00  /* u16 species ID */
#define POKE_FIELD_HELD_ITEM      0x01  /* u16 item ID */
#define POKE_FIELD_MOVE1          0x02  /* u16 move ID */
#define POKE_FIELD_MOVE2          0x03  /* u16 move ID */
#define POKE_FIELD_MOVE3          0x04  /* u16 move ID */
#define POKE_FIELD_MOVE4          0x05  /* u16 move ID */
#define POKE_FIELD_PP1            0x06  /* u8 PP */
#define POKE_FIELD_PP2            0x07  /* u8 PP */
#define POKE_FIELD_PP3            0x08  /* u8 PP */
#define POKE_FIELD_PP4            0x09  /* u8 PP */
#define POKE_FIELD_CURRENT_HP     0x0A  /* u16 */
#define POKE_FIELD_MAX_HP         0x0B  /* u16 */
#define POKE_FIELD_ATTACK         0x0C  /* u16 */
#define POKE_FIELD_DEFENSE        0x0D  /* u16 */
#define POKE_FIELD_SP_ATTACK      0x0E  /* u16 */
#define POKE_FIELD_SP_DEFENSE     0x0F  /* u16 */
#define POKE_FIELD_SPEED          0x10  /* u16 */
#define POKE_FIELD_IV_HP          0x11  /* u8 */
#define POKE_FIELD_IV_ATK         0x12  /* u8 */
#define POKE_FIELD_IV_DEF         0x13  /* u8 */
#define POKE_FIELD_IV_SPATK       0x14  /* u16 */
#define POKE_FIELD_EV_HP          0x15  /* u8 */
#define POKE_FIELD_NATURE         0x16  /* u8 nature ID */
#define POKE_FIELD_STATUS         0x17  /* u8 status condition */
#define POKE_FIELD_LEVEL          0x18  /* u8 */
#define POKE_FIELD_EXP            0x19  /* u32 */
#define POKE_FIELD_FRIENDSHIP     0x1A  /* u8 */
#define POKE_FIELD_ABILITY        0x1B  /* u8 ability ID */
#define POKE_FIELD_TYPE1          0x1C  /* u8 type */
#define POKE_FIELD_TYPE2          0x1D  /* u8 type */
#define POKE_FIELD_OT_ID          0x1E  /* u16 trainer ID */
#define POKE_FIELD_GENDER         0x1F  /* u8 gender */
#define POKE_FIELD_IS_EGG         0x20  /* u8 bool */
#define POKE_FIELD_POKEBALL       0x21  /* u8 ball type */
#define POKE_FIELD_SHADOW_ID      0x22  /* u16 shadow ID (Colosseum-specific) */
#define POKE_FIELD_PURIFICATION   0x23  /* u16 purification gauge */
#define POKE_FIELD_IS_SHADOW      0x24  /* u8 bool */

/* Total number of PokemonGet field IDs */
#define POKE_FIELD_COUNT   0x5E

/* Pokemon slot categories for fn_801F02AC dispatch.
 * These are "slot type" IDs passed as r3 to PokemonSlotLookup.
 * Values 0x11-0x1D map into jumptable_803752F8. */
#define POKE_SLOT_PARTY           0x11  /* Player party member */
#define POKE_SLOT_BATTLE_ACTIVE   0x12  /* Currently in battle */
#define POKE_SLOT_PC              0x13  /* PC storage */
#define POKE_SLOT_DAYCARE         0x14  /* Day Care (if any) */

/* Gender constants */
#define GENDER_MALE     0
#define GENDER_FEMALE   1
#define GENDER_UNKNOWN  2

/* =========================================================================
 * Forward struct declarations
 * ========================================================================= */

/**
 * Runtime Pokemon data structure.
 * Full size is very large (~0xA4C0 bytes based on field offsets).
 * This encompasses the raw GBA-compatible data plus extensive
 * runtime state for the Colosseum engine.
 *
 * Key field offsets (accessed via addis + negative offset pattern):
 *   0xA490 : u32 field (set/get via fightFloorBiosSetItemPokemonPtr/fightFloorBiosGetItemPokemonPtr)
 *   0xA48C : u32 field (set/get via fightFloorBiosSetTokuseiPokemonPtr/fightFloorBiosGetTokuseiPokemonPtr)
 *   0xA488 : u32 field (set/get via fightFloorBiosSetTuikakoukaPokemonPtr/fightFloorBiosGetTuikakoukaPokemonPtr)
 *   0xA484 : u32 field (set/get via fightFloorBiosSetKizetuPokemonPtr/fightFloorBiosGetKizetuPokemonPtr)
 *   0xA4E4 : u16 field (set/get via fightFloorBiosSetFirstAttackRnd/fightFloorBiosGetFirstAttackRnd)
 *   0xA4C4 : u32[8] array (set/get via fightFloorBiosSetFightOutPokemonPtrAryPtr/fn_801F6544)
 *   0xA4C0 : u16 field (set/get via fightFloorBiosSetFightPokemonEntryCnt/fightFloorBiosGetFightPokemonEntryCnt)
 */
struct Pokemon;

/* =========================================================================
 * Core API functions
 * ========================================================================= */

/**
 * fn_801F54A4 - PokemonGet
 * Read a field from a Pokemon structure.
 *
 * @param pokemon  Pointer to Pokemon structure, or 0 for "current context"
 * @param slot     Party slot index (r4)
 * @param field    Field ID (POKE_FIELD_* constant, r5)
 * @param extra    Extra parameter for array-indexed fields (r6)
 * @return         Field value (type depends on field)
 *
 * 510 call sites throughout the codebase. Uses jumptable_803754AC
 * with 0x5D entries to dispatch to the appropriate getter function.
 */
u32 PokemonGet(struct Pokemon* pokemon, u32 slot, u16 field, u32 extra);

/**
 * fn_801F4C14 - PokemonSet
 * Write a field to a Pokemon structure.
 *
 * @param pokemon  Pointer to Pokemon structure, or 0 for "current context"
 * @param slot     Party slot index (r4)
 * @param field    Field ID (POKE_FIELD_* constant, r5)
 * @param extra    Extra parameter (r6)
 * @param value    Value to set (r7)
 * @return         Status (0 = failure)
 *
 * 223 call sites. Uses jumptable_80375330 with 0x5E entries.
 */
u32 PokemonSet(struct Pokemon* pokemon, u32 slot, u16 field, u32 extra, u32 value);

/**
 * fn_801F02AC - PokemonSlotLookup
 * Resolve a slot type + index to a Pokemon pointer, then read data.
 *
 * @param slotType  Slot category (POKE_SLOT_* constant, r3)
 * @param index     Slot index within category (r4)
 * @param count     Number of entries to query (r5)
 * @return          Pokemon pointer or data value
 *
 * 89 call sites. Uses jumptable_803752F8 (13 entries, for slot types 0x11-0x1D).
 */
u32 PokemonSlotLookup(u16 slotType, u32 index, u16 count);

/**
 * fn_801F025C - PokemonSlotLookupDefault
 * Convenience wrapper that calls PokemonGet(0,0,0x14,0) to get the party
 * count, then calls PokemonSlotLookup with that count.
 * 438 call sites. Implemented in pokemon.c as fn_801F025C (matched 100%).
 */

/**
 * fightFloorGetNowPtr - GetPokemonContext
 * Returns the current Pokemon context pointer from the global state.
 * Called from PokemonSlotLookup to get the base address for lookups.
 */
struct Pokemon* GetPokemonContext(void);

/**
 * fightFloorBiosGetFightFloorPtr - GetCurrentPokemon
 * Returns a pointer to the Pokemon currently selected/active in the
 * global context. Called when pokemon parameter is NULL in Get/Set.
 */
struct Pokemon* GetCurrentPokemon(void);

/**
 * fightFloorBiosSetEncountFloorId / fightFloorBiosGetEncountFloorId - Set/Get global Pokemon state pointer
 * These store/load lbl_8047B5F0 (SDA21). Implemented in pokemon.c
 * (matched 100%).
 */

#endif /* GAME_POKEMON_H */
