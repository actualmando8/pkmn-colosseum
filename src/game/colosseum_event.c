/**
 * @file colosseum_event.c
 * @brief Event handlers, scene setup, and colosseum initialization.
 *
 * =========================================================================
 * SUBSYSTEM ANALYSIS
 * =========================================================================
 *
 * Address range: 0x80201800 - 0x80212000
 * Total functions: ~100
 * Total code size: ~40KB
 *
 * This module handles the event/scene management layer that sits between
 * the low-level data accessors (pokemon.c, trainer.c) and the high-level
 * story scripting (colosseum_script.c). It manages:
 *
 *   - Scene transitions into/out of Colosseum areas
 *   - Event flag management for story progression
 *   - Pre-battle dialogue and cutscene triggers
 *   - Colosseum registration and entry validation
 *
 * KEY FUNCTIONS:
 *
 *   fn_802026E4 (CheckEventFlag) - 291 calls, checks boolean flags
 *     Takes (context, flagId) and returns TRUE/FALSE. This is the
 *     primary predicate used throughout the script system to gate
 *     story progression.
 *
 *   fn_8020248C (SetEventState) - 67 calls, sets event state
 *   fn_802025B8 (GetEventState) - 59 calls, gets event state
 *     These manage multi-valued event states (not just booleans).
 *     Used for tracking things like "which round of the Colosseum"
 *     or "which rival encounter".
 *
 *   fn_80211B94 (TriggerEvent) - 121 calls
 *     Invokes an event handler on a scene/trainer context. The event
 *     data pointer (r4) comes from rodata tables like lbl_8027A00C
 *     which contain function pointer pairs (init, update).
 *
 *   fn_80211E18 (EventCallback) - post-battle result processing
 *     Called after story battles to handle rewards, flag updates, etc.
 *
 *   fn_80212D6C - Colosseum round transition handler
 *     Called to advance from one Colosseum round to the next.
 *
 * SCENE-LEVEL FUNCTIONS (0x80201800-0x80206000):
 *
 *   fn_80203620 (0x5C bytes): Navigate trainer context to a specific
 *     data table. Calls fn_8012640C twice: first with field 0xCC,
 *     then with field 0x79. This resolves trainer -> Pokemon -> extended data.
 *
 *   fn_8020367C (0x58 bytes): Similar navigation but takes an extra
 *     parameter and calls fn_80125424 to write data.
 *
 *   fn_802036D4 (0x84 bytes): Three-hop navigation: field 0xD6, then
 *     conditional check, then another hop. Used for complex trainer data.
 *
 *   These utility functions are the "glue" between the trainer system
 *   and the event/scripting system, translating between different data
 *   table reference formats.
 *
 * EVENT FLAG SYSTEM (0x80202000-0x80203000):
 *   The event flag system maps flag IDs to bits in a large array stored
 *   in the save data. CheckEventFlag (fn_802026E4) resolves the flag ID
 *   to an array index + bit position, then reads the corresponding bit.
 *
 *   Flag ID ranges appear to be:
 *     0x00-0x0F : System flags (game completion, save state)
 *     0x10-0x3D : Story progression flags
 *     0x3E+     : Colosseum/optional content flags
 *
 * COLOSSEUM ENTRY (0x80206000-0x80212000):
 *   This section handles the mechanics of entering a Colosseum:
 *   - Checking if the player's team is valid
 *   - Registering for a tournament
 *   - Setting up opponent brackets
 *   - Initializing the battle sequence
 *
 *   fn_8020A000-0x8020B000 region: Dense cluster of 76 functions that
 *   appear to be Colosseum-specific event handlers, one per game event
 *   or dialogue trigger.
 *
 * RODATA TABLES:
 *   lbl_80279C28-lbl_80279D08: Constant pairs (possibly text/message IDs)
 *   lbl_8027A00C: Function pointer table for event dispatching
 *   lbl_80279E7C: Constant data (0x00007693) - possibly model/animation ID
 *   lbl_80279F7C: Byte table {0x01,0xC8,0x04,0x96,...} - timing/difficulty curve
 *   lbl_80279FA0: Constant pairs - possibly UI element IDs
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
extern void  fn_80125424(void* context, u32 value);
extern u32   fn_80142CF4(u32 context, u32 param, u16 field, u32 flags);

/* =========================================================================
 * fn_80203620 - ResolveTrainerExtendedData
 *
 * Navigate from a trainer context through two data table hops to reach
 * extended Pokemon/trainer data.
 *
 * Hop 1: fn_8012640C(ctx, 0, 0xCC, 0) -> intermediate pointer
 * Hop 2: fn_8012640C(intermediate, 0, 0x79, 0) -> extended data
 *
 * If either hop returns NULL, the function returns NULL.
 *
 * @param context  Trainer/party context
 * @return         Extended data pointer, or NULL
 * ========================================================================= */
void* ResolveTrainerExtendedData(void* context) {
    void* intermediate;
    if (context == NULL) {
        return NULL;
    }

    intermediate = fn_8012640C(context, 0, 0xCC, 0);
    if (intermediate == NULL) {
        return NULL;
    }

    return fn_8012640C(intermediate, 0, 0x79, 0);
}

/* =========================================================================
 * fn_8020367C - WriteTrainerExtendedData
 *
 * Similar two-hop navigation, but the second call writes data via
 * fn_80125424 instead of reading it.
 *
 * @param context  Trainer/party context
 * @param value    Value to write
 * ========================================================================= */
void WriteTrainerExtendedData(void* context, u32 value) {
    void* intermediate;
    if (context == NULL) {
        return;
    }

    intermediate = fn_8012640C(context, 0, 0xCC, 0);
    if (intermediate == NULL) {
        return;
    }

    fn_80125424(intermediate, value);
}

/* =========================================================================
 * fn_802036D4 - ResolveTrainerExtendedDataThreeHop
 *
 * Three-hop data navigation for complex trainer structures.
 * Hop 1: field 0xD6 (party list)
 * Conditional check on result
 * Hop 2: if valid, field 0xD2 (secondary reference)
 *
 * @param context  Trainer context
 * @return         Resolved data pointer, or NULL
 * ========================================================================= */
/* TODO: Decompile fn_802036D4 (0x84 bytes) */

/* =========================================================================
 * fn_802026E4 - CheckEventFlag
 *
 * Checks a boolean event flag. This is the gating predicate used by
 * the story script system -- virtually every branching decision in the
 * script interpreter calls this to check whether a condition is met.
 *
 * 291 call sites.
 *
 * @param context  Context pointer (trainer, scene, or NULL for global)
 * @param flagId   Flag identifier to check
 * @return         TRUE (1) if flag is set, FALSE (0) otherwise
 * ========================================================================= */
/* TODO: Decompile fn_802026E4 */

/* =========================================================================
 * fn_802025B8 - GetEventState
 *
 * Gets a multi-valued event state (not just boolean).
 * 59 call sites. Returns a u8 value.
 * ========================================================================= */
/* TODO: Decompile fn_802025B8 */

/* =========================================================================
 * fn_8020248C - SetEventState
 *
 * Sets a multi-valued event state.
 * 67 call sites.
 * ========================================================================= */
/* TODO: Decompile fn_8020248C */

/* =========================================================================
 * fn_80211B94 - TriggerEvent
 *
 * Triggers an event on a scene/trainer context. Takes an event data
 * pointer that comes from rodata function pointer tables.
 *
 * The event data pointer (e.g., lbl_8027A00C) contains pairs of
 * function pointers: the first is an init function, the second is
 * an update/tick function. TriggerEvent calls the init function
 * and registers the update function for per-frame callbacks.
 *
 * 121 call sites.
 * ========================================================================= */
/* TODO: Decompile fn_80211B94 */

/* =========================================================================
 * fn_80211E18 - EventCallback
 *
 * Post-event callback processor. Called after battles and other events
 * complete to process results (give rewards, set flags, advance story).
 * ========================================================================= */
/* TODO: Decompile fn_80211E18 */

/* =========================================================================
 * Dense event handler block (0x8020A000 - 0x8020B000)
 *
 * 76 functions in 4KB. These are individual event handlers, likely
 * one per game event type. Each follows a similar pattern:
 *   1. Call PokemonGet/TrainerDataGet to check conditions
 *   2. Call SetEventState or TrainerDataSet to update state
 *   3. Return a status code
 *
 * The functions are called via function pointer tables set up by
 * the colosseum initialization code.
 * ========================================================================= */

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 140 functions matched
 * =================================================================== */

/* Address: 0x8020A068 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020A068(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020A0A4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A0A4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA9]) = val;
}

/* Address: 0x8020A0B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A0B4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA8]) = val;
}

/* Address: 0x8020A0C4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A0C4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA6]) = val;
}

/* Address: 0x8020A0D4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A0D4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA4]) = val;
}

/* Address: 0x8020A0E4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A0E4(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xA0]) = val;
}

/* Address: 0x8020A0F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A0F4(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x9C]) = val;
}

/* Address: 0x8020A104 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A104(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x99]) = val;
}

/* Address: 0x8020A114 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A114(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x98]) = val;
}

/* Address: 0x8020A124 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A124(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}

/* Address: 0x8020A134 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A134(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x4]) = val;
}

/* Address: 0x8020A144 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A144(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x8020A154 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A154(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x8020A164 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020A164(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA9]);
}

/* Address: 0x8020A17C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020A17C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA8]);
}

/* Address: 0x8020A194 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020A194(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA6]);
}

/* Address: 0x8020A1AC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020A1AC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA4]);
}

/* Address: 0x8020A1C4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020A1C4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA0]);
}

/* Address: 0x8020A1DC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020A1DC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x9C]);
}

/* Address: 0x8020A1F4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020A1F4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x99]);
}

/* Address: 0x8020A20C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020A20C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x98]);
}

/* Address: 0x8020A258 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020A258(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}

/* Address: 0x8020A270 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020A270(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x8020A288 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020A288(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x8020A2A0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020A2A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020A2F8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A2F8(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x8020A308 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A308(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x8020A318 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A318(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x8020A328 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020A328(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x8020A338 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020A338(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020A350 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020A350(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020A368 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020A368(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x8020A380 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020A380(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x8020AE30 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE30(void) { return 1; }

/* Address: 0x8020AE38 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE38(void) { return 1; }

/* Address: 0x8020AE40 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE40(void) { return 1; }

/* Address: 0x8020AE48 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE48(void) { return 1; }

/* Address: 0x8020AE50 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE50(void) { return 1; }

/* Address: 0x8020AE58 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE58(void) { return 1; }

/* Address: 0x8020AE60 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE60(void) { return 1; }

/* Address: 0x8020AE68 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE68(void) { return 1; }

/* Address: 0x8020AE70 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE70(void) { return 1; }

/* Address: 0x8020AE78 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE78(void) { return 1; }

/* Address: 0x8020AE80 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE80(void) { return 1; }

/* Address: 0x8020AE88 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE88(void) { return 1; }

/* Address: 0x8020AE90 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE90(void) { return 1; }

/* Address: 0x8020AE98 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AE98(void) { return 1; }

/* Address: 0x8020AEA0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AEA0(void) { return 1; }

/* Address: 0x8020AEA8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AEA8(void) { return 1; }

/* Address: 0x8020AEB0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AEB0(void) { return 1; }

/* Address: 0x8020AEB8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AEB8(void) { return 1; }

/* Address: 0x8020AEC0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AEC0(void) { return 1; }

/* Address: 0x8020AEC8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020AEC8(void) { return 1; }

/* Address: 0x8020B050 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020B050(void) { return 1; }

/* Address: 0x8020D784 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020D784(void) { return 1; }

/* Address: 0x8020D78C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D78C(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x1C]) = val;
}

/* Address: 0x8020D79C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D79C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020D7B4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D7B4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020D814 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D814(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020D82C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020D82C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x8020D868 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D868(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x18]) = val;
}

/* Address: 0x8020D878 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D878(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x10]) = val;
}

/* Address: 0x8020D888 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D888(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* Address: 0x8020D898 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D898(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}

/* Address: 0x8020D8A8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D8A8(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x8020D8B8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D8B8(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x8020D8C8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020D8C8(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x8020D8D8 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D8D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x8020D8F0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D8F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x8020D908 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D908(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}

/* Address: 0x8020D920 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D920(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020D938 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020D938(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020D950 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020D950(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x8020D9A0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020D9A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x8020D9B8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020D9B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x8020D9D0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020D9D0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x8020DE50 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020DE50(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x8020DE80 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020DE80(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020DE98 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020DE98(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020DED8 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020DED8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020DEF0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020DEF0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}

/* Address: 0x8020DF00 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020DF00(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xC]) = val;
}

/* Address: 0x8020DF90 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020DF90(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x4]) = val;
}

/* Address: 0x8020DFA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020DFA0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}

/* Address: 0x8020DFB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8020DFB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x8020DFC0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020DFC0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}

/* Address: 0x8020DFD8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020DFD8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}

/* Address: 0x8020DFF0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020DFF0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x10]);
}

/* Address: 0x8020E008 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020E008(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xC]);
}

/* Address: 0x8020E0B0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8020E0B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x8020E0C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E0C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* Address: 0x8020E0E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E0E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020E1A4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E1A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}

/* Address: 0x8020E1BC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E1BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* Address: 0x8020E1D4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E1D4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020E1EC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020E1EC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020E230 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E230(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x18]);
}

/* Address: 0x8020E248 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8020E248(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x1C]);
}

/* Address: 0x8020E260 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E260(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x17]);
}

/* Address: 0x8020E278 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E278(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x16]);
}

/* Address: 0x8020E290 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E290(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x15]);
}

/* Address: 0x8020E2A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E2A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x14]);
}

/* Address: 0x8020E2C0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E2C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x13]);
}

/* Address: 0x8020E2D8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E2D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x12]);
}

/* Address: 0x8020E2F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E2F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}

/* Address: 0x8020E308 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E308(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}

/* Address: 0x8020E320 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E320(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}

/* Address: 0x8020E338 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E338(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}

/* Address: 0x8020E350 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E350(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}

/* Address: 0x8020E368 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E368(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}

/* Address: 0x8020E380 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E380(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}

/* Address: 0x8020E398 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E398(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}

/* Address: 0x8020E3B0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E3B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA]);
}

/* Address: 0x8020E3C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E3C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}

/* Address: 0x8020E3E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E3E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}

/* Address: 0x8020E3F8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E3F8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}

/* Address: 0x8020E410 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E410(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}

/* Address: 0x8020E428 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E428(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}

/* Address: 0x8020E440 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E440(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}

/* Address: 0x8020E458 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E458(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}

/* Address: 0x8020E470 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E470(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}

/* Address: 0x8020E4B4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8020E4B4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* Address: 0x8020F100 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F100(void) { return 0; }

/* Address: 0x8020F230 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F230(void) { return 0; }

/* Address: 0x8020F360 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F360(void) { return 0; }

/* Address: 0x8020F3E8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F3E8(void) { return 0; }

/* Address: 0x8020F7B0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F7B0(void) { return 0; }

/* Address: 0x8020F8CC | Size: 0x8 | Pattern: return_constant */
u32 fn_8020F8CC(void) { return 0; }

/* Address: 0x8020FB30 | Size: 0x8 | Pattern: return_constant */
u32 fn_8020FB30(void) { return 0; }

/* Address: 0x8020FBFC | Size: 0x8 | Pattern: return_constant */
u32 fn_8020FBFC(void) { return 0; }

/* Address: 0x80210880 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210880(void) { return 0; }

/* Address: 0x80210990 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210990(void) { return 0; }

/* Address: 0x80210B00 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210B00(void) { return 0; }

/* Address: 0x80210BF0 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210BF0(void) { return 0; }

/* Address: 0x80210CFC | Size: 0x8 | Pattern: return_constant */
u32 fn_80210CFC(void) { return 0; }

/* Address: 0x80210E54 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210E54(void) { return 0; }

/* Address: 0x80210EC0 | Size: 0x8 | Pattern: return_constant */
u32 fn_80210EC0(void) { return 0; }

/* Address: 0x80211038 | Size: 0x8 | Pattern: return_constant */
u32 fn_80211038(void) { return 0; }

/* Address: 0x8021115C | Size: 0x8 | Pattern: return_constant */
u32 fn_8021115C(void) { return 0; }

/* Address: 0x80211168 | Size: 0x8 | Pattern: return_constant */
u32 fn_80211168(void) { return 0; }
