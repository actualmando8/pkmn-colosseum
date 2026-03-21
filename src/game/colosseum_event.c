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

/* SDA table pointers for event data arrays */
extern u32 lbl_80478D38;   /* Event table count */
extern u8  lbl_80478D30[];   /* Event table base (6 bytes per entry) */

/* Forward declarations for converted functions */
void fn_8020248C(void);
void fn_802025B8(void);
void fn_802026E4(void);
void fn_80203620(void);
void fn_8020367C(void);
void fn_802036D4(void);
void fn_80211B94(void);
void fn_80211E18(void);


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

/* #######################################################################
 * COVERAGE STUBS: Colosseum event system (0x80201800 - 0x80212000)
 * 256 functions remaining for full coverage of colosseum_event.c TU.
 *
 * Key functions in this range:
 *   fn_802026E4 (CheckEventFlag)  - 291 calls, primary story gate
 *   fn_8020248C (SetEventState)   - 67 calls, multi-valued state setter
 *   fn_802025B8 (GetEventState)   - 59 calls, multi-valued state getter
 *   fn_80211B94 (TriggerEvent)    - 121 calls, event dispatcher
 *   fn_80211E18 (EventCallback)   - post-battle result processing
 *   fn_80212D6C (ColosseumRoundTransition) - round advancement
 *   fn_8020C840 (BattleSystemInit) - battle subsystem initialization
 *   fn_80205B8C (GetTrainerPokemonPtr) - 668 calls, trainer->pokemon nav
 * ####################################################################### */

#pragma push
#pragma force_active on

/* 0x80201890 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80201890(void) {
    extern void fn_80119ED0();
    extern void fn_8011A3E4();
    extern void fn_80121574();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802018E4;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_802018E4;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201980;
L_802018E4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201924;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80201954;
L_80201924: ;
    if ((u32)r31 != (u32)0x0) goto L_80201934;
    r3 = 0x0;
    goto L_80201948;
L_80201934: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80201948: ;
    r4 = r30;
    fn_80121574();
    goto L_802019A8;
L_80201954: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80201970;
    r3 = 0x0;
    goto L_802019A8;
L_80201970: ;
    r3 = r31;
    r4 = r30;
    fn_8011A3E4();
    goto L_802019A8;
L_80201980: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_8020199C;
    r3 = 0x0;
    goto L_802019A8;
L_8020199C: ;
    r3 = r31;
    r4 = r30;
    fn_8011A3E4();
L_802019A8: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x802019BC | size: 0x170 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802019BC(void) {
    extern void fn_80119ED0();
    extern void fn_8011A0A8();
    extern void fn_80121484();
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
    r29 = r5;
    r31 = r3;
    r30 = r4;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201A14;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80201A14;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201AF4;
L_80201A14: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = r3;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201A6C;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80201ACC;
L_80201A6C: ;
    if ((u32)r31 != (u32)0x0) goto L_80201A7C;
    r31 = 0x0;
    goto L_80201A94;
L_80201A7C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
L_80201A94: ;
    if ((u32)r30 != (u32)0x0) goto L_80201AA4;
    r4 = 0x0;
    goto L_80201ABC;
L_80201AA4: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3;
L_80201ABC: ;
    r3 = r31;
    r5 = r29;
    fn_80121484();
    goto L_80201B18;
L_80201ACC: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201B18;
    r3 = r31;
    r4 = r30;
    r5 = r29;
    fn_8011A0A8();
    goto L_80201B18;
L_80201AF4: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xd8) goto L_80201B18;
    r3 = r31;
    r4 = r30;
    r5 = r29;
    fn_8011A0A8();
L_80201B18: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80201B2C | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80201B2C(void) {
    extern void fn_80119ED0();
    extern void fn_8011A570();
    extern void fn_801215E4();
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
    r29 = r4;
    r31 = r3;
    r30 = r5;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201B84;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80201B84;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201C20;
L_80201B84: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201BC4;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80201BF8;
L_80201BC4: ;
    if ((u32)r31 != (u32)0x0) goto L_80201BD4;
    r3 = 0x0;
    goto L_80201BE8;
L_80201BD4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80201BE8: ;
    r4 = r29;
    r5 = r30;
    fn_801215E4();
    goto L_80201C44;
L_80201BF8: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201C44;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011A570();
    goto L_80201C44;
L_80201C20: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xd8) goto L_80201C44;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011A570();
L_80201C44: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80201C58 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80201C58(void) {
    extern void fn_80119ED0();
    extern void fn_8011A6D4();
    extern void fn_8012165C();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201CAC;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80201CAC;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201D48;
L_80201CAC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201CEC;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80201D1C;
L_80201CEC: ;
    if ((u32)r31 != (u32)0x0) goto L_80201CFC;
    r3 = 0x0;
    goto L_80201D10;
L_80201CFC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80201D10: ;
    r4 = r30;
    fn_8012165C();
    goto L_80201D70;
L_80201D1C: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80201D38;
    r3 = 0x0;
    goto L_80201D70;
L_80201D38: ;
    r3 = r31;
    r4 = r30;
    fn_8011A6D4();
    goto L_80201D70;
L_80201D48: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_80201D64;
    r3 = 0x0;
    goto L_80201D70;
L_80201D64: ;
    r3 = r31;
    r4 = r30;
    fn_8011A6D4();
L_80201D70: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80201D84 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80201D84(void) {
    extern void fn_80119ED0();
    extern void fn_8011A860();
    extern void fn_801216CC();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201DD8;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80201DD8;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201E74;
L_80201DD8: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201E18;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80201E48;
L_80201E18: ;
    if ((u32)r31 != (u32)0x0) goto L_80201E28;
    r3 = 0x0;
    goto L_80201E3C;
L_80201E28: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80201E3C: ;
    r4 = r30;
    fn_801216CC();
    goto L_80201E9C;
L_80201E48: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80201E64;
    r3 = 0x0;
    goto L_80201E9C;
L_80201E64: ;
    r3 = r31;
    r4 = r30;
    fn_8011A860();
    goto L_80201E9C;
L_80201E74: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_80201E90;
    r3 = 0x0;
    goto L_80201E9C;
L_80201E90: ;
    r3 = r31;
    r4 = r30;
    fn_8011A860();
L_80201E9C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80201EB0 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80201EB0(void) {
    extern void fn_80119ED0();
    extern void fn_8011A9EC();
    extern void fn_8012173C();
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
    r29 = r4;
    r31 = r3;
    r30 = r5;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201F08;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80201F08;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201FA4;
L_80201F08: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80201F48;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80201F7C;
L_80201F48: ;
    if ((u32)r31 != (u32)0x0) goto L_80201F58;
    r3 = 0x0;
    goto L_80201F6C;
L_80201F58: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80201F6C: ;
    r4 = r29;
    r5 = r30;
    fn_8012173C();
    goto L_80201FC8;
L_80201F7C: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80201FC8;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011A9EC();
    goto L_80201FC8;
L_80201FA4: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xd8) goto L_80201FC8;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011A9EC();
L_80201FC8: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80201FDC | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80201FDC(void) {
    extern void fn_80119ED0();
    extern void fn_8011AB50();
    extern void fn_801217B4();
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
    r29 = r4;
    r31 = r3;
    r30 = r5;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202034;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80202034;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_802020D0;
L_80202034: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202074;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802020A8;
L_80202074: ;
    if ((u32)r31 != (u32)0x0) goto L_80202084;
    r3 = 0x0;
    goto L_80202098;
L_80202084: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202098: ;
    r4 = r29;
    r5 = r30;
    fn_801217B4();
    goto L_802020F4;
L_802020A8: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_802020F4;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011AB50();
    goto L_802020F4;
L_802020D0: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xd8) goto L_802020F4;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011AB50();
L_802020F4: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80202108 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202108(void) {
    extern void fn_80119ED0();
    extern void fn_8011ACB4();
    extern void fn_8012182C();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_8020215C;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_8020215C;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_802021F8;
L_8020215C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_8020219C;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802021CC;
L_8020219C: ;
    if ((u32)r31 != (u32)0x0) goto L_802021AC;
    r3 = 0x0;
    goto L_802021C0;
L_802021AC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802021C0: ;
    r4 = r30;
    fn_8012182C();
    goto L_80202220;
L_802021CC: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_802021E8;
    r3 = -0x1;
    goto L_80202220;
L_802021E8: ;
    r3 = r31;
    r4 = r30;
    fn_8011ACB4();
    goto L_80202220;
L_802021F8: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_80202214;
    r3 = -0x1;
    goto L_80202220;
L_80202214: ;
    r3 = r31;
    r4 = r30;
    fn_8011ACB4();
L_80202220: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80202234 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202234(void) {
    extern void fn_80119ED0();
    extern void fn_8011AE40();
    extern void fn_8012189C();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202288;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80202288;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80202324;
L_80202288: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802022C8;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802022F8;
L_802022C8: ;
    if ((u32)r31 != (u32)0x0) goto L_802022D8;
    r3 = 0x0;
    goto L_802022EC;
L_802022D8: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802022EC: ;
    r4 = r30;
    fn_8012189C();
    goto L_8020234C;
L_802022F8: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80202314;
    r3 = -0x1;
    goto L_8020234C;
L_80202314: ;
    r3 = r31;
    r4 = r30;
    fn_8011AE40();
    goto L_8020234C;
L_80202324: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_80202340;
    r3 = -0x1;
    goto L_8020234C;
L_80202340: ;
    r3 = r31;
    r4 = r30;
    fn_8011AE40();
L_8020234C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80202360 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202360(void) {
    extern void fn_80119ED0();
    extern void fn_8011B130();
    extern void fn_80121984();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802023B4;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_802023B4;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80202450;
L_802023B4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802023F4;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80202424;
L_802023F4: ;
    if ((u32)r31 != (u32)0x0) goto L_80202404;
    r3 = 0x0;
    goto L_80202418;
L_80202404: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202418: ;
    r4 = r30;
    fn_80121984();
    goto L_80202478;
L_80202424: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80202440;
    r3 = -0x1;
    goto L_80202478;
L_80202440: ;
    r3 = r31;
    r4 = r30;
    fn_8011B130();
    goto L_80202478;
L_80202450: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_8020246C;
    r3 = -0x1;
    goto L_80202478;
L_8020246C: ;
    r3 = r31;
    r4 = r30;
    fn_8011B130();
L_80202478: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020248C | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020248C(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_801219F4();
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
    r29 = r4;
    r31 = r3;
    r30 = r5;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802024E4;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_802024E4;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80202580;
L_802024E4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202524;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80202558;
L_80202524: ;
    if ((u32)r31 != (u32)0x0) goto L_80202534;
    r3 = 0x0;
    goto L_80202548;
L_80202534: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202548: ;
    r4 = r29;
    r5 = r30;
    fn_801219F4();
    goto L_802025A4;
L_80202558: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_802025A4;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011B2C0();
    goto L_802025A4;
L_80202580: ;
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xd8) goto L_802025A4;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_8011B2C0();
L_802025A4: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x802025B8 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802025B8(void) {
    extern void fn_80119ED0();
    extern void fn_8011B444();
    extern void fn_80121A6C();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_8020260C;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_8020260C;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_802026A8;
L_8020260C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_8020264C;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_8020267C;
L_8020264C: ;
    if ((u32)r31 != (u32)0x0) goto L_8020265C;
    r3 = 0x0;
    goto L_80202670;
L_8020265C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202670: ;
    r4 = r30;
    fn_80121A6C();
    goto L_802026D0;
L_8020267C: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80202698;
    r3 = 0x0;
    goto L_802026D0;
L_80202698: ;
    r3 = r31;
    r4 = r30;
    fn_8011B444();
    goto L_802026D0;
L_802026A8: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_802026C4;
    r3 = 0x0;
    goto L_802026D0;
L_802026C4: ;
    r3 = r31;
    r4 = r30;
    fn_8011B444();
L_802026D0: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x802026E4 | size: 0x12C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802026E4(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
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
    r30 = r4;
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202738;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80202738;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_802027D4;
L_80202738: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202778;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802027A8;
L_80202778: ;
    if ((u32)r31 != (u32)0x0) goto L_80202788;
    r3 = 0x0;
    goto L_8020279C;
L_80202788: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020279C: ;
    r4 = r30;
    fn_80121ADC();
    goto L_802027FC;
L_802027A8: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_802027C4;
    r3 = 0x0;
    goto L_802027FC;
L_802027C4: ;
    r3 = r31;
    r4 = r30;
    fn_8011B67C();
    goto L_802027FC;
L_802027D4: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_802027F0;
    r3 = 0x0;
    goto L_802027FC;
L_802027F0: ;
    r3 = r31;
    r4 = r30;
    fn_8011B67C();
L_802027FC: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80202810 | size: 0x188 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202810(void) {
    extern void fn_80119ED0();
    extern void fn_8011B788();
    extern void fn_80121B4C();
    extern void fn_801DA36C();
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

    r5 = 0xee;
    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r30 = r4;
    r29 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r30 & 0xFFFF;
    r31 = r3;
    if ((s32)r0 != (s32)0) goto L_80202864;
    if ((u32)r31 == (u32)0x0) goto L_80202894;
    r4 = 0x1;
    fn_801DA36C();
    r3 = r31;
    r4 = 0x2;
    fn_801DA36C();
    goto L_80202894;
L_80202864: ;
    if ((u32)r31 == (u32)0x0) goto L_80202894;
    if ((u32)r0 != (u32)0x8) goto L_8020287C;
    r4 = 0x1;
    fn_801DA36C();
L_8020287C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x7) goto L_80202894;
    r3 = r31;
    r4 = 0x2;
    fn_801DA36C();
L_80202894: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802028D0;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_802028D0;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80202964;
L_802028D0: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202910;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80202940;
L_80202910: ;
    if ((u32)r31 != (u32)0x0) goto L_80202920;
    r3 = 0x0;
    goto L_80202934;
L_80202920: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202934: ;
    r4 = r30;
    fn_80121B4C();
    goto L_80202984;
L_80202940: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80202984;
    r3 = r31;
    r4 = r30;
    fn_8011B788();
    goto L_80202984;
L_80202964: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xd8) goto L_80202984;
    r3 = r29;
    r4 = r30;
    fn_8011B788();
L_80202984: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80202998 | size: 0x94 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202998(void) {
    extern void fn_801DA36C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0xee;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r4;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r30 & 0xFFFF;
    r31 = r3;
    if ((s32)r0 != (s32)0) goto L_802029E8;
    if ((u32)r31 == (u32)0x0) goto L_80202A18;
    r4 = 0x1;
    fn_801DA36C();
    r3 = r31;
    r4 = 0x2;
    fn_801DA36C();
    goto L_80202A18;
L_802029E8: ;
    if ((u32)r31 == (u32)0x0) goto L_80202A18;
    if ((u32)r0 != (u32)0x8) goto L_80202A00;
    r4 = 0x1;
    fn_801DA36C();
L_80202A00: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x7) goto L_80202A18;
    r3 = r31;
    r4 = 0x2;
    fn_801DA36C();
L_80202A18: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80202A2C | size: 0xB0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202A2C(void) {
    extern void fn_80119ED0();
    extern void fn_8011AFCC();
    extern void fn_8012190C();
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
    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202A70;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80202AA4;
L_80202A70: ;
    if ((u32)r29 != (u32)0x0) goto L_80202A80;
    r3 = 0x0;
    goto L_80202A94;
L_80202A80: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202A94: ;
    r4 = r30;
    r5 = r31;
    fn_8012190C();
    goto L_80202AC8;
L_80202AA4: ;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80202AC8;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8011AFCC();
L_80202AC8: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80202ADC | size: 0xAC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202ADC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
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
    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80202B1C;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80202B4C;
L_80202B1C: ;
    if ((u32)r30 != (u32)0x0) goto L_80202B2C;
    r3 = 0x0;
    goto L_80202B40;
L_80202B2C: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202B40: ;
    r4 = r31;
    fn_80121ADC();
    goto L_80202B74;
L_80202B4C: ;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80202B68;
    r3 = 0x0;
    goto L_80202B74;
L_80202B68: ;
    r3 = r30;
    r4 = r31;
    fn_8011B67C();
L_80202B74: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80202B88 | size: 0x94 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202B88(void) {
    extern void fn_801F02AC();
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

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r31 = r3;
    r29 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    if ((u32)r31 != (u32)0x0) goto L_80202BC8;
    r3 = 0x0;
    goto L_80202C08;
L_80202BC8: ;
    if ((u32)r29 != (u32)0x0) goto L_80202BD8;
    r3 = 0x0;
    goto L_80202C08;
L_80202BD8: ;
    r4 = r31;
    r5 = r30;
    r3 = 0x2;
    fn_801F02AC();
    r31 = r3;
    r4 = r29;
    r5 = r30;
    r3 = 0x2;
    fn_801F02AC();
    r0 = r3 - r31;
    r0 = __cntlzw(r0);
    /* extrwi r3, r0, 8, 19 */;
L_80202C08: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80202C1C | size: 0x57C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80202C1C(void) {
    extern void fn_801233F4();
    extern void fn_80123FBC();
    extern void fn_801EF634();
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FA634();
    extern void fn_801FB1C0();
    extern void fn_8020E57C();
    extern void fn_8020E614();
    extern void fn_8020E640();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r21, 0x14(r1) */;
    r28 = r3;
    r26 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r24 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r25 = r3 & 0xFFFF;
    r31 = 0x0;
    goto L_80203178;
L_80202C80: ;
    r3 = r26;
    r6 = r31;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    r27 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_80203174;
    r30 = 0x0;
    goto L_80203168;
L_80202CAC: ;
    r3 = r27;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    /* mr. r29, r3 */;
    if ((s32)r0 != (s32)0) goto L_80202CD0;
    r0 = 0x0;
    goto L_80202FB0;
L_80202CD0: ;
    if ((s32)r0 != (s32)0) goto L_80202CDC;
    r0 = 0x0;
    goto L_80202DF8;
L_80202CDC: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80202CF4;
    r0 = 0x0;
    goto L_80202DF8;
L_80202CF4: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r21, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_80202D18;
    r0 = 0x0;
    goto L_80202DF8;
L_80202D18: ;
    if ((u32)r0 != (u32)0x1) goto L_80202D24;
    r0 = 0x0;
    goto L_80202DE4;
L_80202D24: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80202D3C;
    r0 = 0x0;
    goto L_80202DE4;
L_80202D3C: ;
    r3 = r21;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80202D60;
    r0 = 0x0;
    goto L_80202DE4;
L_80202D60: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80202D74;
    r0 = 0x0;
    goto L_80202DE4;
L_80202D74: ;
    if ((u32)r21 != (u32)0x0) goto L_80202D84;
    r3 = 0x0;
    goto L_80202D98;
L_80202D84: ;
    r3 = r21;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202D98: ;
    if ((u32)r3 != (u32)0x0) goto L_80202DA8;
    r0 = 0x0;
    goto L_80202DE4;
L_80202DA8: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80202DBC;
    r0 = 0x0;
    goto L_80202DE4;
L_80202DBC: ;
    r3 = r21;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_80202DE0;
    r0 = 0x0;
    goto L_80202DE4;
L_80202DE0: ;
    r0 = 0x1;
L_80202DE4: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80202DF4;
    r0 = 0x0;
    goto L_80202DF8;
L_80202DF4: ;
    r0 = 0x1;
L_80202DF8: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80202E08;
    r0 = 0x0;
    goto L_80202FB0;
L_80202E08: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_80202E2C;
    r0 = 0x0;
    goto L_80202FB0;
L_80202E2C: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r23, r3 */;
    if ((s32)r3 != (s32)0x1) goto L_80202E50;
    r0 = 0x0;
    goto L_80202F9C;
L_80202E50: ;
    if ((s32)r3 != (s32)0x1) goto L_80202E5C;
    r0 = 0x0;
    goto L_80202F1C;
L_80202E5C: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80202E74;
    r0 = 0x0;
    goto L_80202F1C;
L_80202E74: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80202E98;
    r0 = 0x0;
    goto L_80202F1C;
L_80202E98: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80202EAC;
    r0 = 0x0;
    goto L_80202F1C;
L_80202EAC: ;
    if ((u32)r23 != (u32)0x0) goto L_80202EBC;
    r3 = 0x0;
    goto L_80202ED0;
L_80202EBC: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202ED0: ;
    if ((u32)r3 != (u32)0x0) goto L_80202EE0;
    r0 = 0x0;
    goto L_80202F1C;
L_80202EE0: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80202EF4;
    r0 = 0x0;
    goto L_80202F1C;
L_80202EF4: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_80202F18;
    r0 = 0x0;
    goto L_80202F1C;
L_80202F18: ;
    r0 = 0x1;
L_80202F1C: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80202F2C;
    r0 = 0x0;
    goto L_80202F9C;
L_80202F2C: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_80202F50;
    r0 = 0x0;
    goto L_80202F9C;
L_80202F50: ;
    if ((u32)r23 != (u32)0x0) goto L_80202F60;
    r3 = 0x0;
    goto L_80202F74;
L_80202F60: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80202F74: ;
    if ((u32)r3 != (u32)0x0) goto L_80202F84;
    r0 = 0x0;
    goto L_80202F9C;
L_80202F84: ;
    fn_801233F4();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80202F98;
    r0 = 0x0;
    goto L_80202F9C;
L_80202F98: ;
    r0 = 0x1;
L_80202F9C: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80202FAC;
    r0 = 0x0;
    goto L_80202FB0;
L_80202FAC: ;
    r0 = 0x1;
L_80202FB0: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_80203164;
    if ((u32)r28 == (u32)0x0) goto L_80203164;
    if ((u32)r29 != (u32)0x0) goto L_80202FD0;
    r0 = 0x0;
    goto L_802030EC;
L_80202FD0: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80202FE8;
    r0 = 0x0;
    goto L_802030EC;
L_80202FE8: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r21, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_8020300C;
    r0 = 0x0;
    goto L_802030EC;
L_8020300C: ;
    if ((u32)r0 != (u32)0x1) goto L_80203018;
    r0 = 0x0;
    goto L_802030D8;
L_80203018: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80203030;
    r0 = 0x0;
    goto L_802030D8;
L_80203030: ;
    r3 = r21;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203054;
    r0 = 0x0;
    goto L_802030D8;
L_80203054: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80203068;
    r0 = 0x0;
    goto L_802030D8;
L_80203068: ;
    if ((u32)r21 != (u32)0x0) goto L_80203078;
    r3 = 0x0;
    goto L_8020308C;
L_80203078: ;
    r3 = r21;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020308C: ;
    if ((u32)r3 != (u32)0x0) goto L_8020309C;
    r0 = 0x0;
    goto L_802030D8;
L_8020309C: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802030B0;
    r0 = 0x0;
    goto L_802030D8;
L_802030B0: ;
    r3 = r21;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_802030D4;
    r0 = 0x0;
    goto L_802030D8;
L_802030D4: ;
    r0 = 0x1;
L_802030D8: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_802030E8;
    r0 = 0x0;
    goto L_802030EC;
L_802030E8: ;
    r0 = 0x1;
L_802030EC: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_80203164;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x122;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r23 = r3;
    r5 = r29;
    r4 = 0x4;
    fn_8020E57C();
    if ((u32)r3 != (u32)0x0) goto L_80203164;
    r22 = 0x0;
    goto L_80203158;
L_80203128: ;
    r0 = r22 & 0xFFFF;
    r0 = r0 * 0xc;
    r21 = r23 + r0;
    r3 = r21;
    fn_8020E614();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80203154;
    r3 = r21;
    r4 = r29;
    fn_8020E640();
    goto L_80203164;
L_80203154: ;
    r22 = r22 + 0x1;
L_80203158: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_80203128;
L_80203164: ;
    r30 = r30 + 0x1;
L_80203168: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r25) goto L_80202CAC;
L_80203174: ;
    r31 = r31 + 0x1;
L_80203178: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_80202C80;
    /* lmw r21, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80203198 | size: 0x14C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203198(void) {
    extern void fn_801FD0EC();
    extern void fn_8020E57C();
    extern void fn_8020E614();
    extern void fn_8020E758();
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
    f32 f7 = 0.0f;

    /* stmw r26, 0x8(r1) */;
    /* mr. r31, r3 */;
    r26 = r4;
    if ((s32)r0 == (s32)0) goto L_802032D0;
    r4 = 0x0;
    r5 = 0x122;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r5 = r26;
    r4 = 0x4;
    fn_8020E57C();
    /* mr. r26, r3 */;
    if ((s32)r0 == (s32)0) goto L_802032D0;
    fn_801FD0EC();
    r29 = r3;
    r3 = r26;
    fn_8020E758();
    r0 = r29 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_802032D0;
    if ((u32)r0 == (u32)0x165) goto L_802032D0;
    if ((u32)r0 == (u32)0xffff) goto L_802032D0;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf7;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x0) goto L_802032D0;
    if ((u32)r31 != (u32)0x0) goto L_8020322C;
    r28 = 0x0;
    goto L_802032B0;
L_8020322C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x122;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = r3;
    r3 = 0x0;
    goto L_80203250;
L_8020324C: ;
    r3 = r3 + 0x1;
L_80203250: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020324C;
    r28 = 0x0;
    r26 = r28;
    goto L_802032A4;
L_80203268: ;
    r0 = r26 & 0xFF;
    r0 = r0 * 0xc;
    r27 = r30 + r0;
    r3 = r27;
    fn_8020E614();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x4) goto L_802032A0;
    r3 = r27;
    fn_801FD0EC();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x4) goto L_802032A0;
    if ((u32)r0 == (u32)0x165) goto L_802032A0;
    r28 = r28 + 0x1;
L_802032A0: ;
    r26 = r26 + 0x1;
L_802032A4: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_80203268;
L_802032B0: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x4) goto L_802032D0;
    r3 = r31;
    r7 = r29 & 0xFFFF;
    r4 = 0x0;
    r5 = 0xf7;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_802032D0: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x802032E4 | size: 0x138 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802032E4(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    extern void fn_80122370();
    extern void fn_80123090();
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
    /* mr. r28, r3 */;
    r31 = r4;
    if ((s32)r0 != (s32)0) goto L_80203308;
    r30 = 0x0;
    goto L_8020331C;
L_80203308: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = r3;
L_8020331C: ;
    if ((u32)r30 == (u32)0x0) goto L_80203408;
    if ((u32)r28 != (u32)0x0) goto L_80203334;
    r29 = 0x0;
    goto L_8020334C;
L_80203334: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r29 = r3;
L_8020334C: ;
    if ((u32)r29 != (u32)0x0) goto L_8020335C;
    r4 = 0x0;
    goto L_802033FC;
L_8020335C: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80203384;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802033B4;
L_80203384: ;
    if ((u32)r28 != (u32)0x0) goto L_80203394;
    r3 = 0x0;
    goto L_802033A8;
L_80203394: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802033A8: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_802033DC;
L_802033B4: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_802033D0;
    r3 = 0x0;
    goto L_802033DC;
L_802033D0: ;
    r3 = r28;
    r4 = 0x3d;
    fn_8011B67C();
L_802033DC: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802033F0;
    r4 = 0x0;
    goto L_802033FC;
L_802033F0: ;
    r3 = r29;
    fn_80123090();
    r4 = r3;
L_802033FC: ;
    r3 = r30;
    r5 = r31;
    fn_80122370();
L_80203408: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020341C | size: 0x140 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020341C(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    extern void fn_801226D0();
    extern void fn_80123090();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    /* mr. r27, r3 */;
    r30 = r4;
    r31 = r5;
    if ((s32)r0 != (s32)0) goto L_80203444;
    r29 = 0x0;
    goto L_80203458;
L_80203444: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r29 = r3;
L_80203458: ;
    if ((u32)r29 == (u32)0x0) goto L_80203548;
    if ((u32)r27 != (u32)0x0) goto L_80203470;
    r28 = 0x0;
    goto L_80203488;
L_80203470: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r28 = r3;
L_80203488: ;
    if ((u32)r28 != (u32)0x0) goto L_80203498;
    r4 = 0x0;
    goto L_80203538;
L_80203498: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802034C0;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802034F0;
L_802034C0: ;
    if ((u32)r27 != (u32)0x0) goto L_802034D0;
    r3 = 0x0;
    goto L_802034E4;
L_802034D0: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802034E4: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_80203518;
L_802034F0: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_8020350C;
    r3 = 0x0;
    goto L_80203518;
L_8020350C: ;
    r3 = r27;
    r4 = 0x3d;
    fn_8011B67C();
L_80203518: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020352C;
    r4 = 0x0;
    goto L_80203538;
L_8020352C: ;
    r3 = r28;
    fn_80123090();
    r4 = r3;
L_80203538: ;
    r3 = r29;
    r5 = r30;
    r6 = r31;
    fn_801226D0();
L_80203548: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020355C | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020355C(void) {
    extern void fn_801229F4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 != (u32)0x0) goto L_80203580;
    r3 = 0x0;
    goto L_80203590;
L_80203580: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203590: ;
    if ((u32)r3 != (u32)0x0) goto L_802035A0;
    r3 = 0x0;
    goto L_802035A8;
L_802035A0: ;
    r4 = r31;
    fn_801229F4();
L_802035A8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802035BC | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802035BC(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 != (u32)0x0) goto L_802035E0;
    r3 = 0x0;
    goto L_802035F0;
L_802035E0: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802035F0: ;
    if ((u32)r3 == (u32)0x0) goto L_8020360C;
    r7 = r31;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_8020360C: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80203620 | size: 0x5C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203620(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8020363C;
    r3 = 0x0;
    goto L_8020364C;
L_8020363C: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020364C: ;
    if ((u32)r3 != (u32)0x0) goto L_8020365C;
    r3 = 0x0;
    goto L_8020366C;
L_8020365C: ;
    r4 = 0x0;
    r5 = 0x79;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020366C: ;
    return;
}
#pragma pop

/* 0x8020367C | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020367C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 != (u32)0x0) goto L_802036A0;
    r3 = 0x0;
    goto L_802036B0;
L_802036A0: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802036B0: ;
    if ((u32)r3 == (u32)0x0) goto L_802036C0;
    r4 = r31;
    ((void(*)(void))fn_80125424)();
L_802036C0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802036D4 | size: 0x84 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802036D4(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_802036F0;
    r3 = 0x0;
    goto L_80203720;
L_802036F0: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203710;
    r3 = 0x0;
    goto L_80203720;
L_80203710: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203720: ;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x61;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    return;
}
#pragma pop

/* 0x80203758 | size: 0x84 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203758(void) {
    extern void fn_800FA280();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203784;
    r3 = 0x0;
    goto L_80203794;
L_80203784: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203794: ;
    if ((u32)r3 != (u32)0x0) goto L_802037A4;
    r3 = 0x0;
    goto L_802037CC;
L_802037A4: ;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_800FA280();
L_802037CC: ;
    return;
}
#pragma pop

/* 0x802037DC | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802037DC(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203808;
    r3 = 0x0;
    goto L_80203818;
L_80203808: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203818: ;
    if ((u32)r3 != (u32)0x0) goto L_80203828;
    r3 = 0x0;
    goto L_80203838;
L_80203828: ;
    r4 = 0x0;
    r5 = 0x77;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203838: ;
    return;
}
#pragma pop

/* 0x80203848 | size: 0x5C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203848(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80203864;
    r3 = 0x0;
    goto L_80203874;
L_80203864: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203874: ;
    if ((u32)r3 != (u32)0x0) goto L_80203884;
    r3 = 0x0;
    goto L_80203894;
L_80203884: ;
    r4 = 0x0;
    r5 = 0x77;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203894: ;
    return;
}
#pragma pop

/* 0x802038A4 | size: 0x1C8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802038A4(void) {
    extern void fn_80123FBC();
    extern void fn_801EF634();
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
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_802038C4;
    r0 = 0x0;
    goto L_802039E0;
L_802038C4: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802038DC;
    r0 = 0x0;
    goto L_802039E0;
L_802038DC: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_80203900;
    r0 = 0x0;
    goto L_802039E0;
L_80203900: ;
    if ((u32)r0 != (u32)0x1) goto L_8020390C;
    r0 = 0x0;
    goto L_802039CC;
L_8020390C: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80203924;
    r0 = 0x0;
    goto L_802039CC;
L_80203924: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203948;
    r0 = 0x0;
    goto L_802039CC;
L_80203948: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8020395C;
    r0 = 0x0;
    goto L_802039CC;
L_8020395C: ;
    if ((u32)r31 != (u32)0x0) goto L_8020396C;
    r3 = 0x0;
    goto L_80203980;
L_8020396C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203980: ;
    if ((u32)r3 != (u32)0x0) goto L_80203990;
    r0 = 0x0;
    goto L_802039CC;
L_80203990: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802039A4;
    r0 = 0x0;
    goto L_802039CC;
L_802039A4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_802039C8;
    r0 = 0x0;
    goto L_802039CC;
L_802039C8: ;
    r0 = 0x1;
L_802039CC: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_802039DC;
    r0 = 0x0;
    goto L_802039E0;
L_802039DC: ;
    r0 = 0x1;
L_802039E0: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_802039F0;
    r3 = 0x1;
    goto L_80203A58;
L_802039F0: ;
    if ((u32)r30 != (u32)0x0) goto L_80203A00;
    r3 = 0x0;
    goto L_80203A34;
L_80203A00: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203A24;
    r3 = 0x0;
    goto L_80203A34;
L_80203A24: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203A34: ;
    if ((u32)r3 != (u32)0x0) goto L_80203A44;
    r3 = 0x1;
    goto L_80203A58;
L_80203A44: ;
    r4 = 0x0;
    r5 = 0x7b;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFF;
L_80203A58: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80203A6C | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203A6C(void) {
    extern void fn_80122A70();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80203A88;
    r3 = 0x0;
    goto L_80203AB8;
L_80203A88: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203AA8;
    r3 = 0x0;
    goto L_80203AB8;
L_80203AA8: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203AB8: ;
    if ((u32)r3 != (u32)0x0) goto L_80203AC8;
    r3 = 0x0;
    goto L_80203ACC;
L_80203AC8: ;
    fn_80122A70();
L_80203ACC: ;
    return;
}
#pragma pop

/* 0x80203ADC | size: 0x80 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203ADC(void) {
    extern void fn_80122AE0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 != (u32)0x0) goto L_80203B00;
    r3 = 0x0;
    goto L_80203B30;
L_80203B00: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203B20;
    r3 = 0x0;
    goto L_80203B30;
L_80203B20: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203B30: ;
    if ((u32)r3 != (u32)0x0) goto L_80203B40;
    r3 = 0x0;
    goto L_80203B48;
L_80203B40: ;
    r4 = r31;
    fn_80122AE0();
L_80203B48: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80203B5C | size: 0x80 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203B5C(void) {
    extern void fn_80122B50();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 != (u32)0x0) goto L_80203B80;
    r3 = 0x0;
    goto L_80203BB0;
L_80203B80: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203BA0;
    r3 = 0x0;
    goto L_80203BB0;
L_80203BA0: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203BB0: ;
    if ((u32)r3 != (u32)0x0) goto L_80203BC0;
    r3 = 0x0;
    goto L_80203BC8;
L_80203BC0: ;
    r4 = r31;
    fn_80122B50();
L_80203BC8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80203BDC | size: 0x80 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203BDC(void) {
    extern void fn_80122BC0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 != (u32)0x0) goto L_80203C00;
    r3 = 0x0;
    goto L_80203C30;
L_80203C00: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203C20;
    r3 = 0x0;
    goto L_80203C30;
L_80203C20: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203C30: ;
    if ((u32)r3 != (u32)0x0) goto L_80203C40;
    r3 = 0x0;
    goto L_80203C48;
L_80203C40: ;
    r4 = r31;
    fn_80122BC0();
L_80203C48: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80203C5C | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203C5C(void) {
    extern void fn_80122C64();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80203C78;
    r3 = 0x0;
    goto L_80203CA8;
L_80203C78: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203C98;
    r3 = 0x0;
    goto L_80203CA8;
L_80203C98: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203CA8: ;
    if ((u32)r3 != (u32)0x0) goto L_80203CB8;
    r3 = 0x0;
    goto L_80203CBC;
L_80203CB8: ;
    fn_80122C64();
L_80203CBC: ;
    return;
}
#pragma pop

/* 0x80203CCC | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203CCC(void) {
    extern void fn_80122DDC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80203CE8;
    r3 = 0x0;
    goto L_80203D18;
L_80203CE8: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203D08;
    r3 = 0x0;
    goto L_80203D18;
L_80203D08: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203D18: ;
    if ((u32)r3 != (u32)0x0) goto L_80203D28;
    r3 = 0x0;
    goto L_80203D2C;
L_80203D28: ;
    fn_80122DDC();
L_80203D2C: ;
    return;
}
#pragma pop

/* 0x80203D3C | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203D3C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203D68;
    r3 = 0x0;
    goto L_80203D78;
L_80203D68: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203D78: ;
    if ((u32)r3 != (u32)0x0) goto L_80203D88;
    r3 = 0x0;
    goto L_80203D9C;
L_80203D88: ;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
L_80203D9C: ;
    return;
}
#pragma pop

/* 0x80203DAC | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203DAC(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80203DC8;
    r3 = 0x0;
    goto L_80203DD8;
L_80203DC8: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203DD8: ;
    if ((u32)r3 != (u32)0x0) goto L_80203DE8;
    r3 = 0x0;
    goto L_80203DFC;
L_80203DE8: ;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
L_80203DFC: ;
    return;
}
#pragma pop

/* 0x80203E0C | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203E0C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80203E38;
    r3 = 0x0;
    goto L_80203E48;
L_80203E38: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203E48: ;
    if ((u32)r3 != (u32)0x0) goto L_80203E58;
    r3 = 0x0;
    goto L_80203E6C;
L_80203E58: ;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFF;
L_80203E6C: ;
    return;
}
#pragma pop

/* 0x80203E7C | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203E7C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80203E98;
    r3 = 0x0;
    goto L_80203EA8;
L_80203E98: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203EA8: ;
    if ((u32)r3 != (u32)0x0) goto L_80203EB8;
    r3 = 0x0;
    goto L_80203ECC;
L_80203EB8: ;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFF;
L_80203ECC: ;
    return;
}
#pragma pop

/* 0x80203EDC | size: 0x108 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203EDC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    extern void fn_80122FF4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_80203F0C;
    r31 = 0x0;
    goto L_80203F20;
L_80203F0C: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
L_80203F20: ;
    if ((u32)r31 != (u32)0x0) goto L_80203F30;
    r3 = 0x0;
    goto L_80203FD0;
L_80203F30: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80203F58;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80203F88;
L_80203F58: ;
    if ((u32)r30 != (u32)0x0) goto L_80203F68;
    r3 = 0x0;
    goto L_80203F7C;
L_80203F68: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80203F7C: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_80203FB0;
L_80203F88: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80203FA4;
    r3 = 0x0;
    goto L_80203FB0;
L_80203FA4: ;
    r3 = r30;
    r4 = 0x3d;
    fn_8011B67C();
L_80203FB0: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80203FC4;
    r3 = 0x0;
    goto L_80203FD0;
L_80203FC4: ;
    r3 = r31;
    fn_80122FF4();
    r3 = r3 & 0xFFFF;
L_80203FD0: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80203FE4 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80203FE4(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    extern void fn_80123090();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_80204014;
    r31 = 0x0;
    goto L_80204028;
L_80204014: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
L_80204028: ;
    if ((u32)r31 != (u32)0x0) goto L_80204038;
    r3 = 0x0;
    goto L_802040D4;
L_80204038: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80204060;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80204090;
L_80204060: ;
    if ((u32)r30 != (u32)0x0) goto L_80204070;
    r3 = 0x0;
    goto L_80204084;
L_80204070: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80204084: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_802040B8;
L_80204090: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_802040AC;
    r3 = 0x0;
    goto L_802040B8;
L_802040AC: ;
    r3 = r30;
    r4 = 0x3d;
    fn_8011B67C();
L_802040B8: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802040CC;
    r3 = 0x0;
    goto L_802040D4;
L_802040CC: ;
    r3 = r31;
    fn_80123090();
L_802040D4: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x802041EC | size: 0xF4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802041EC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    extern void fn_80123090();
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
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_8020420C;
    r31 = 0x0;
    goto L_80204220;
L_8020420C: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
L_80204220: ;
    if ((u32)r31 != (u32)0x0) goto L_80204230;
    r3 = 0x0;
    goto L_802042CC;
L_80204230: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80204258;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80204288;
L_80204258: ;
    if ((u32)r30 != (u32)0x0) goto L_80204268;
    r3 = 0x0;
    goto L_8020427C;
L_80204268: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020427C: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_802042B0;
L_80204288: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_802042A4;
    r3 = 0x0;
    goto L_802042B0;
L_802042A4: ;
    r3 = r30;
    r4 = 0x3d;
    fn_8011B67C();
L_802042B0: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802042C4;
    r3 = 0x0;
    goto L_802042CC;
L_802042C4: ;
    r3 = r31;
    fn_80123090();
L_802042CC: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x802042E0 | size: 0xF4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802042E0(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    extern void fn_801230E0();
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
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_80204300;
    r31 = 0x0;
    goto L_80204314;
L_80204300: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
L_80204314: ;
    if ((u32)r31 != (u32)0x0) goto L_80204324;
    r3 = 0x0;
    goto L_802043C0;
L_80204324: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_8020434C;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_8020437C;
L_8020434C: ;
    if ((u32)r30 != (u32)0x0) goto L_8020435C;
    r3 = 0x0;
    goto L_80204370;
L_8020435C: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80204370: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_802043A4;
L_8020437C: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80204398;
    r3 = 0x0;
    goto L_802043A4;
L_80204398: ;
    r3 = r30;
    r4 = 0x3d;
    fn_8011B67C();
L_802043A4: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802043B8;
    r3 = 0x0;
    goto L_802043C0;
L_802043B8: ;
    r3 = r31;
    fn_801230E0();
L_802043C0: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x802043D4 | size: 0x480 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802043D4(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    extern void fn_80122FF4();
    extern void fn_80123090();
    extern void fn_8012A5B0();
    extern void fn_8020E4E8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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
    /* mr. r31, r3 */;
    r24 = r4;
    r26 = r5;
    r23 = r6;
    r27 = r7;
    if ((s32)r0 != (s32)0) goto L_80204404;
    r29 = 0x0;
    goto L_80204438;
L_80204404: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80204424;
    r3 = 0x0;
    goto L_80204434;
L_80204424: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80204434: ;
    r29 = r3;
L_80204438: ;
    if ((u32)r29 != (u32)0x0) goto L_80204448;
    r3 = 0x0;
    goto L_80204840;
L_80204448: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x100;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r28 = r3 & 0xFFFF;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((u32)r29 != (u32)0x0) goto L_80204484;
    r25 = 0x0;
    goto L_80204498;
L_80204484: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r25 = r3;
L_80204498: ;
    if ((u32)r25 != (u32)0x0) goto L_802044A8;
    r30 = 0x0;
    goto L_80204548;
L_802044A8: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802044D0;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80204500;
L_802044D0: ;
    if ((u32)r30 != (u32)0x0) goto L_802044E0;
    r3 = 0x0;
    goto L_802044F4;
L_802044E0: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802044F4: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_80204528;
L_80204500: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_8020451C;
    r3 = 0x0;
    goto L_80204528;
L_8020451C: ;
    r3 = r30;
    r4 = 0x3d;
    fn_8011B67C();
L_80204528: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020453C;
    r30 = 0x0;
    goto L_80204548;
L_8020453C: ;
    r3 = r25;
    fn_80123090();
    r30 = r3;
L_80204548: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r22, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_8020456C;
    r25 = 0x0;
    goto L_80204580;
L_8020456C: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r25 = r3;
L_80204580: ;
    if ((u32)r25 != (u32)0x0) goto L_80204590;
    r25 = 0x0;
    goto L_80204630;
L_80204590: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802045B8;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802045E8;
L_802045B8: ;
    if ((u32)r22 != (u32)0x0) goto L_802045C8;
    r3 = 0x0;
    goto L_802045DC;
L_802045C8: ;
    r3 = r22;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802045DC: ;
    r4 = 0x3d;
    fn_80121ADC();
    goto L_80204610;
L_802045E8: ;
    r3 = 0x3d;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80204604;
    r3 = 0x0;
    goto L_80204610;
L_80204604: ;
    r3 = r22;
    r4 = 0x3d;
    fn_8011B67C();
L_80204610: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80204624;
    r25 = 0x0;
    goto L_80204630;
L_80204624: ;
    r3 = r25;
    fn_80122FF4();
    r25 = r3 & 0xFFFF;
L_80204630: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xea;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r22 = r3 & 0xFF;
    if ((u32)r27 == (u32)0x0) goto L_80204668;
    r3 = r27;
    r4 = 0x11;
    r5 = 0x0;
    fn_8012A5B0();
    r27 = r3 & 0xFF;
    goto L_8020466C;
L_80204668: ;
    r27 = 0x0;
L_8020466C: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x8c;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r4 = r3;
    if ((u32)r28 != (u32)0x21) goto L_802046A4;
    r0 = r26 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_802046A4;
    r4 = r3 << 1;
    goto L_802046BC;
L_802046A4: ;
    if ((u32)r28 != (u32)0x22) goto L_802046BC;
    r0 = r26 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802046BC;
    r4 = r4 << 1;
L_802046BC: ;
    r3 = r22;
    fn_8020E4E8();
    r0 = r24 & 0xFF;
    r24 = r3;
    if ((u32)r0 != (u32)0x1) goto L_802046EC;
    r0 = r27 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802046EC;
    r3 = r24 * 0x6e;
    r0 = 0x64;
    r24 = (u32)r3 / (u32)r0;
L_802046EC: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x18) goto L_802046FC;
    r24 = (u32)r24 >> 1;
L_802046FC: ;
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80204738;
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80204738;
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_802047D4;
L_80204738: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r22 = r3;
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80204778;
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_802047A8;
L_80204778: ;
    if ((u32)r22 != (u32)0x0) goto L_80204788;
    r3 = 0x0;
    goto L_8020479C;
L_80204788: ;
    r3 = r22;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020479C: ;
    r4 = 0x5;
    fn_80121ADC();
    goto L_802047FC;
L_802047A8: ;
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_802047C4;
    r3 = 0x0;
    goto L_802047FC;
L_802047C4: ;
    r3 = r22;
    r4 = 0x5;
    fn_8011B67C();
    goto L_802047FC;
L_802047D4: ;
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_802047F0;
    r3 = 0x0;
    goto L_802047FC;
L_802047F0: ;
    r3 = r31;
    r4 = 0x5;
    fn_8011B67C();
L_802047FC: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020480C;
    r24 = (u32)r24 >> 2;
L_8020480C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 != (u32)0x1a) goto L_8020483C;
    r3 = (0x1 << 16);
    r0 = 0x64;
    /* subi r3, r3, 0x1 */;
    r4 = r23 & 0xFFFF;
    r3 = r25 * r3;
    r0 = (s32)r3 / (s32)r0;
    if ((s32)r4 >= (s32)r0) goto L_8020483C;
    r24 = -0x1;
L_8020483C: ;
    r3 = r24;
L_80204840: ;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80204854 | size: 0xD4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204854(void) {
    extern void fn_801F1170();
    extern void fn_8020D8D8();
    extern void fn_8020D950();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0xce;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r31 = r3;
    r3 = r4;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = (s16)r3;
    if ((s32)r0 >= (s32)0) goto L_8020488C;
    r3 = 0x0;
    goto L_80204914;
L_8020488C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x121;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = (s16)r3;
    if ((s32)r30 != (s32)r0) goto L_802048B4;
    r3 = 0x1;
    goto L_80204914;
L_802048B4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r30 == (s32)r0) goto L_80204910;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80204910;
    r3 = r31;
    fn_8020D950();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x9) goto L_80204910;
    r3 = r31;
    fn_8020D8D8();
    r0 = (s16)r3;
    if ((s32)r30 != (s32)r0) goto L_80204910;
    r3 = 0x1;
    goto L_80204914;
L_80204910: ;
    r3 = 0x0;
L_80204914: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80204928 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204928(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r5 = 0xd5;
    r6 = 0x0;
    r31 = r3;
    r3 = r4;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r31 - r3;
    r31 = *(u32*)(sp + 0xC);
    r0 = __cntlzw(r0);
    /* extrwi r3, r0, 8, 19 */;
    return;
}
#pragma pop

/* 0x80204970 | size: 0xA0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204970(void) {
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((u32)r3 == (u32)0x0) goto L_80204A08;
    if ((u32)r4 == (u32)0x0) goto L_80204A08;
    r0 = 0x2a;
    r8 = r1 + 0x4;
    /* subi r6, r3, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_80204994: ;
    r5 = *(u32*)((u8*)r6 + 0x4);
    r0 = *(u32*)((u8*)r6 + 0x8);
    *(u32*)((u8*)r8 + 0x4) = r5;
    r8 += 8; *(u32*)r8 = r0;
    if (--ctr != 0) goto L_80204994;
    r7 = *(u32*)((u8*)r6 + 0x4);
    r0 = 0x2a;
    /* subi r6, r3, 0x4 */;
    /* subi r5, r4, 0x4 */;
    *(u32*)((u8*)r8 + 0x4) = r7;
    ctr_fn = (void(*)(void))r0;
L_802049C0: ;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)((u8*)r6 + 0x4) = r3;
    r6 += 8; *(u32*)r6 = r0;
    if (--ctr != 0) goto L_802049C0;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = 0x2a;
    /* subi r5, r4, 0x4 */;
    r4 = r1 + 0x4;
    *(u32*)((u8*)r6 + 0x4) = r3;
    ctr_fn = (void(*)(void))r0;
L_802049EC: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_802049EC;
    r0 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r5 + 0x4) = r0;
L_80204A08: ;
    return;
}
#pragma pop

/* 0x80204A10 | size: 0x4C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204A10(void) {
    extern void fn_801F4354();
    extern void fn_801FB8F8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = r3;
    r3 = 0x0;
    fn_801F4354();
    if ((u32)r3 != (u32)0x0) goto L_80204A38;
    r3 = 0x0;
    goto L_80204A4C;
L_80204A38: ;
    fn_801FB8F8();
    r0 = r3 & 0xFF;
    r0 = 0x1 - r0;
    r0 = __cntlzw(r0);
    /* extrwi r3, r0, 8, 19 */;
L_80204A4C: ;
    return;
}
#pragma pop

/* 0x80204A5C | size: 0x1AC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204A5C(void) {
    extern u8 lbl_80478BD8[];
    extern void fn_80142984();
    extern void fn_801F0898();
    extern void fn_801F1170();
    extern void fn_801F54A4();
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

    /* stmw r26, 0x8(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    goto L_80204BE0;
L_80204A80: ;
    r3 = r31;
    fn_80142984();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_80204BDC;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80204AC4;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r3 = r3 & 0xFF;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)r3) goto L_80204BDC;
    goto L_80204AE8;
L_80204AC4: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r3 = r3 & 0xFF;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)r3) goto L_80204BDC;
L_80204AE8: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r28 != (u32)0x0) goto L_80204B0C;
    r0 = 0x0;
    goto L_80204BC8;
L_80204B0C: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r26, r3 */;
    if ((u32)r28 != (u32)0x0) goto L_80204B30;
    r0 = 0x0;
    goto L_80204BC8;
L_80204B30: ;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r28 != (u32)0x0) goto L_80204B44;
    r0 = 0x0;
    goto L_80204BC8;
L_80204B44: ;
    r3 = r26;
    fn_801F0898();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x12) goto L_80204B60;
    r0 = 0x0;
    goto L_80204BC8;
L_80204B60: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r26, r3 */;
    if ((u32)r0 != (u32)0x12) goto L_80204B84;
    r0 = 0x0;
    goto L_80204BC8;
L_80204B84: ;
    r4 = 0x0;
    r5 = 0x1e;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r27 = r3 & 0xFFFF;
    r3 = r26;
    r4 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x12) goto L_80204BC4;
    if ((u32)r27 == (u32)r0) goto L_80204BC4;
    r0 = 0x0;
    goto L_80204BC8;
L_80204BC4: ;
    r0 = 0x1;
L_80204BC8: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80204BDC;
    r3 = 0x1;
    goto L_80204BF4;
L_80204BDC: ;
    r31 = r31 + 0x1;
L_80204BE0: ;
    r0 = *(u32*)lbl_80478BD8;
    r3 = r31 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_80204A80;
    r3 = 0x0;
L_80204BF4: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80204C08 | size: 0xD8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204C08(void) {
    extern void fn_801F0898();
    extern void fn_801F1170();
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

    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r3 = 0x0;
    fn_801F54A4();
    if ((u32)r30 != (u32)0x0) goto L_80204C40;
    r3 = 0x0;
    goto L_80204CCC;
L_80204C40: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((u32)r30 != (u32)0x0) goto L_80204C64;
    r3 = 0x0;
    goto L_80204CCC;
L_80204C64: ;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r30 != (u32)0x0) goto L_80204C78;
    r3 = 0x0;
    goto L_80204CCC;
L_80204C78: ;
    r3 = r31;
    fn_801F0898();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x12) goto L_80204C94;
    r3 = 0x0;
    goto L_80204CCC;
L_80204C94: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80204CB8;
    r3 = 0x0;
    goto L_80204CCC;
L_80204CB8: ;
    r4 = 0x0;
    r5 = 0x1e;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r3 = r3 & 0xFFFF;
L_80204CCC: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80204CE0 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204CE0(void) {
    extern void fn_80142B24();
    extern void fn_801F11CC();
    extern void fn_8020A398();
    extern void fn_8020D878();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
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

    /* stmw r21, 0x14(r1) */;
    r22 = r4;
    r23 = r5;
    r24 = r6;
    r29 = *(u8*)((u8*)r1 + 0x4B);
    r26 = r8;
    r21 = r3;
    r25 = r7;
    r27 = r9;
    r28 = r10;
    r30 = r8 & 0xFFFF;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_80204D38;
    r3 = 0x0;
    goto L_80204DD0;
L_80204D38: ;
    r4 = r30;
    r5 = r27;
    r6 = r28;
    fn_8020A398();
    r3 = r31;
    r7 = r29;
    r4 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    fn_80142B24();
    r3 = r21;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_80204D84;
    r31 = 0x0;
    goto L_80204DBC;
L_80204D84: ;
    r4 = r22;
    r5 = r21;
    r6 = r23;
    r7 = r24;
    r8 = r25;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80204DB8;
    r3 = r31;
    r4 = r26;
    fn_8020D878();
    goto L_80204DBC;
L_80204DB8: ;
    r31 = 0x0;
L_80204DBC: ;
    if ((u32)r31 != (u32)0x0) goto L_80204DCC;
    r3 = 0x0;
    goto L_80204DD0;
L_80204DCC: ;
    r3 = r31;
L_80204DD0: ;
    /* lmw r21, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80204DE4 | size: 0x188 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204DE4(void) {
    extern void fn_8011BEB4();
    extern void fn_801F0134();
    extern void fn_801F025C();
    extern void fn_801F0898();
    extern void fn_801F1170();
    extern void fn_801F54A4();
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

    r6 = 0x0;
    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    if ((u32)r25 != (u32)0x0) goto L_80204E28;
    r3 = 0x0;
    goto L_80204F58;
L_80204E28: ;
    if ((u32)r27 != (u32)0x0) goto L_80204E38;
    r31 = 0x0;
    goto L_80204E48;
L_80204E38: ;
    r3 = r27;
    r4 = r29;
    fn_801F0134();
    r31 = r3;
L_80204E48: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((u32)r27 != (u32)0x0) goto L_80204E6C;
    r3 = 0x0;
    goto L_80204F58;
L_80204E6C: ;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r27 != (u32)0x0) goto L_80204E80;
    r3 = 0x0;
    goto L_80204F58;
L_80204E80: ;
    r3 = r30;
    fn_801F0898();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x13) goto L_80204E9C;
    r3 = 0x0;
    goto L_80204F58;
L_80204E9C: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((u32)r0 != (u32)0x13) goto L_80204EC0;
    r3 = 0x0;
    goto L_80204F58;
L_80204EC0: ;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BEB4();
    r28 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = r28;
    r5 = 0x9;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r26 & 0xFFFF;
    r27 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x13) goto L_80204F04;
    if ((u32)r28 == (u32)r0) goto L_80204F04;
    r3 = 0x0;
    goto L_80204F58;
L_80204F04: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    if ((u32)r27 != (u32)0xb0) goto L_80204F38;
    r4 = r25;
    r3 = 0xe;
    fn_801F025C();
    r4 = r29;
    fn_801F0134();
L_80204F38: ;
    r4 = r31 & 0xFFFF;
    if ((u32)r27 == (u32)0xb0) goto L_80204F54;
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)r4) goto L_80204F54;
    r3 = 0x0;
    goto L_80204F58;
L_80204F54: ;
    r3 = 0x1;
L_80204F58: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80204F6C | size: 0xF0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80204F6C(void) {
    extern void fn_801F11CC();
    extern void fn_802099AC();
    extern void fn_8020D878();
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
    u32 r10 = 0;
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
    r23 = r4;
    r24 = r5;
    r25 = r6;
    r30 = *(u8*)((u8*)r1 + 0x3B);
    r27 = r8;
    r22 = r3;
    r26 = r7;
    r28 = r9;
    r29 = r10;
    r31 = r8 & 0xFFFF;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80204FC4;
    r3 = 0x0;
    goto L_80205048;
L_80204FC4: ;
    r4 = r29;
    r5 = r31;
    r6 = r28;
    r7 = r30;
    fn_802099AC();
    r3 = r22;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((u32)r3 != (u32)0x0) goto L_80204FFC;
    r31 = 0x0;
    goto L_80205034;
L_80204FFC: ;
    r4 = r23;
    r5 = r22;
    r6 = r24;
    r7 = r25;
    r8 = r26;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80205030;
    r3 = r31;
    r4 = r27;
    fn_8020D878();
    goto L_80205034;
L_80205030: ;
    r31 = 0x0;
L_80205034: ;
    if ((u32)r31 != (u32)0x0) goto L_80205044;
    r3 = 0x0;
    goto L_80205048;
L_80205044: ;
    r3 = r31;
L_80205048: ;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020505C | size: 0x98 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020505C(void) {
    extern void fn_801F11CC();
    extern void fn_8020D878();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r25 = r3;
    r29 = r7;
    r30 = r8;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_802050A4;
    r3 = 0x0;
    goto L_802050E0;
L_802050A4: ;
    r4 = r26;
    r5 = r25;
    r6 = r27;
    r7 = r28;
    r8 = r29;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802050DC;
    r3 = r31;
    r4 = r30;
    fn_8020D878();
    r3 = r31;
    goto L_802050E0;
L_802050DC: ;
    r3 = 0x0;
L_802050E0: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80205134 | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205134(void) {
    extern void fn_8011BEB4();
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
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205160;
    r3 = 0x9;
    goto L_80205174;
L_80205160: ;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
L_80205174: ;
    return;
}
#pragma pop

/* 0x802051D4 | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802051D4(void) {
    extern void fn_8011BEB4();
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
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205200;
    r3 = 0x0;
    goto L_80205214;
L_80205200: ;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
L_80205214: ;
    return;
}
#pragma pop

/* 0x80205224 | size: 0x50 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205224(void) {
    extern void fn_8011BEB4();
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
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205250;
    r3 = 0x0;
    goto L_80205264;
L_80205250: ;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
L_80205264: ;
    return;
}
#pragma pop

/* 0x80205274 | size: 0x690 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205274(void) {
    extern void fn_801233F4();
    extern void fn_80123FBC();
    extern void fn_801EF634();
    extern void fn_801F54A4();
    extern void fn_801F76B8();
    extern void fn_801FA634();
    extern void fn_801FB1C0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r22, 0x8(r1) */;
    r31 = r3;
    r27 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r24 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r26 = r3 & 0xFFFF;
    r29 = 0x0;
    goto L_802058E4;
L_802052D8: ;
    r3 = r27;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    r28 = r3;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_802058E0;
    r30 = 0x0;
    goto L_802058D4;
L_80205304: ;
    r3 = r28;
    r6 = r30;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    /* mr. r25, r3 */;
    if ((s32)r0 != (s32)0) goto L_80205328;
    r0 = 0x0;
    goto L_80205608;
L_80205328: ;
    if ((s32)r0 != (s32)0) goto L_80205334;
    r0 = 0x0;
    goto L_80205450;
L_80205334: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8020534C;
    r0 = 0x0;
    goto L_80205450;
L_8020534C: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r23, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_80205370;
    r0 = 0x0;
    goto L_80205450;
L_80205370: ;
    if ((u32)r0 != (u32)0x1) goto L_8020537C;
    r0 = 0x0;
    goto L_8020543C;
L_8020537C: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80205394;
    r0 = 0x0;
    goto L_8020543C;
L_80205394: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802053B8;
    r0 = 0x0;
    goto L_8020543C;
L_802053B8: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802053CC;
    r0 = 0x0;
    goto L_8020543C;
L_802053CC: ;
    if ((u32)r23 != (u32)0x0) goto L_802053DC;
    r3 = 0x0;
    goto L_802053F0;
L_802053DC: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802053F0: ;
    if ((u32)r3 != (u32)0x0) goto L_80205400;
    r0 = 0x0;
    goto L_8020543C;
L_80205400: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205414;
    r0 = 0x0;
    goto L_8020543C;
L_80205414: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_80205438;
    r0 = 0x0;
    goto L_8020543C;
L_80205438: ;
    r0 = 0x1;
L_8020543C: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_8020544C;
    r0 = 0x0;
    goto L_80205450;
L_8020544C: ;
    r0 = 0x1;
L_80205450: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80205460;
    r0 = 0x0;
    goto L_80205608;
L_80205460: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_80205484;
    r0 = 0x0;
    goto L_80205608;
L_80205484: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r23, r3 */;
    if ((s32)r3 != (s32)0x1) goto L_802054A8;
    r0 = 0x0;
    goto L_802055F4;
L_802054A8: ;
    if ((s32)r3 != (s32)0x1) goto L_802054B4;
    r0 = 0x0;
    goto L_80205574;
L_802054B4: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802054CC;
    r0 = 0x0;
    goto L_80205574;
L_802054CC: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802054F0;
    r0 = 0x0;
    goto L_80205574;
L_802054F0: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205504;
    r0 = 0x0;
    goto L_80205574;
L_80205504: ;
    if ((u32)r23 != (u32)0x0) goto L_80205514;
    r3 = 0x0;
    goto L_80205528;
L_80205514: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80205528: ;
    if ((u32)r3 != (u32)0x0) goto L_80205538;
    r0 = 0x0;
    goto L_80205574;
L_80205538: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8020554C;
    r0 = 0x0;
    goto L_80205574;
L_8020554C: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_80205570;
    r0 = 0x0;
    goto L_80205574;
L_80205570: ;
    r0 = 0x1;
L_80205574: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80205584;
    r0 = 0x0;
    goto L_802055F4;
L_80205584: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_802055A8;
    r0 = 0x0;
    goto L_802055F4;
L_802055A8: ;
    if ((u32)r23 != (u32)0x0) goto L_802055B8;
    r3 = 0x0;
    goto L_802055CC;
L_802055B8: ;
    r3 = r23;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802055CC: ;
    if ((u32)r3 != (u32)0x0) goto L_802055DC;
    r0 = 0x0;
    goto L_802055F4;
L_802055DC: ;
    fn_801233F4();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802055F0;
    r0 = 0x0;
    goto L_802055F4;
L_802055F0: ;
    r0 = 0x1;
L_802055F4: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205604;
    r0 = 0x0;
    goto L_80205608;
L_80205604: ;
    r0 = 0x1;
L_80205608: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_802058D0;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r25 = r3;
    if ((u32)r31 == (u32)0x0) goto L_802058D0;
    if ((u32)r25 != (u32)0x0) goto L_80205640;
    r0 = 0x0;
    goto L_80205700;
L_80205640: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80205658;
    r0 = 0x0;
    goto L_80205700;
L_80205658: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_8020567C;
    r0 = 0x0;
    goto L_80205700;
L_8020567C: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205690;
    r0 = 0x0;
    goto L_80205700;
L_80205690: ;
    if ((u32)r25 != (u32)0x0) goto L_802056A0;
    r3 = 0x0;
    goto L_802056B4;
L_802056A0: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802056B4: ;
    if ((u32)r3 != (u32)0x0) goto L_802056C4;
    r0 = 0x0;
    goto L_80205700;
L_802056C4: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802056D8;
    r0 = 0x0;
    goto L_80205700;
L_802056D8: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_802056FC;
    r0 = 0x0;
    goto L_80205700;
L_802056FC: ;
    r0 = 0x1;
L_80205700: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x0) goto L_802058D0;
    if ((u32)r31 != (u32)0x0) goto L_80205718;
    r0 = 0x0;
    goto L_80205858;
L_80205718: ;
    if ((u32)r25 != (u32)0x0) goto L_80205728;
    r0 = 0x0;
    goto L_802057E8;
L_80205728: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80205740;
    r0 = 0x0;
    goto L_802057E8;
L_80205740: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205764;
    r0 = 0x0;
    goto L_802057E8;
L_80205764: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205778;
    r0 = 0x0;
    goto L_802057E8;
L_80205778: ;
    if ((u32)r25 != (u32)0x0) goto L_80205788;
    r3 = 0x0;
    goto L_8020579C;
L_80205788: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020579C: ;
    if ((u32)r3 != (u32)0x0) goto L_802057AC;
    r0 = 0x0;
    goto L_802057E8;
L_802057AC: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802057C0;
    r0 = 0x0;
    goto L_802057E8;
L_802057C0: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_802057E4;
    r0 = 0x0;
    goto L_802057E8;
L_802057E4: ;
    r0 = 0x1;
L_802057E8: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_802057F8;
    r0 = 0x0;
    goto L_80205858;
L_802057F8: ;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r22 = (s16)r3;
    r23 = 0x0;
    goto L_80205848;
L_80205818: ;
    r3 = r31;
    r6 = r23 & 0xFF;
    r4 = 0x0;
    r5 = 0xfd;
    ((void(*)(void))fn_8012640C)();
    r0 = (s16)r3;
    if ((s32)r3 < (s32)0x0) goto L_80205844;
    if ((s32)r0 != (s32)r22) goto L_80205844;
    r0 = 0x1;
    goto L_80205858;
L_80205844: ;
    r23 = r23 + 0x1;
L_80205848: ;
    r0 = r23 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80205818;
    r0 = 0x0;
L_80205858: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802058D0;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r25 = (s16)r3;
    r23 = 0x0;
    goto L_802058C4;
L_80205884: ;
    r22 = r23 & 0xFF;
    r3 = r31;
    r6 = r22;
    r4 = 0x0;
    r5 = 0xfd;
    ((void(*)(void))fn_8012640C)();
    r0 = (s16)r3;
    if ((u32)r0 >= (u32)0x1) goto L_802058C0;
    r3 = r31;
    r6 = r22;
    r7 = r25;
    r4 = 0x0;
    r5 = 0xfd;
    ((void(*)(void))fn_801254B4)();
    goto L_802058D0;
L_802058C0: ;
    r23 = r23 + 0x1;
L_802058C4: ;
    r0 = r23 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80205884;
L_802058D0: ;
    r30 = r30 + 0x1;
L_802058D4: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r26) goto L_80205304;
L_802058E0: ;
    r29 = r29 + 0x1;
L_802058E4: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r24) goto L_802052D8;
    /* lmw r22, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80205904 | size: 0x178 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205904(void) {
    extern void fn_80123FBC();
    extern void fn_801EF634();
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
    /* mr. r29, r3 */;
    r30 = r4;
    if ((s32)r0 != (s32)0) goto L_80205928;
    r3 = 0x0;
    goto L_80205A68;
L_80205928: ;
    if ((u32)r30 != (u32)0x0) goto L_80205938;
    r0 = 0x0;
    goto L_802059F8;
L_80205938: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80205950;
    r0 = 0x0;
    goto L_802059F8;
L_80205950: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205974;
    r0 = 0x0;
    goto L_802059F8;
L_80205974: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205988;
    r0 = 0x0;
    goto L_802059F8;
L_80205988: ;
    if ((u32)r30 != (u32)0x0) goto L_80205998;
    r3 = 0x0;
    goto L_802059AC;
L_80205998: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802059AC: ;
    if ((u32)r3 != (u32)0x0) goto L_802059BC;
    r0 = 0x0;
    goto L_802059F8;
L_802059BC: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802059D0;
    r0 = 0x0;
    goto L_802059F8;
L_802059D0: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_802059F4;
    r0 = 0x0;
    goto L_802059F8;
L_802059F4: ;
    r0 = 0x1;
L_802059F8: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80205A08;
    r3 = 0x0;
    goto L_80205A68;
L_80205A08: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = (s16)r3;
    r30 = 0x0;
    goto L_80205A58;
L_80205A28: ;
    r3 = r29;
    r6 = r30 & 0xFF;
    r4 = 0x0;
    r5 = 0xfd;
    ((void(*)(void))fn_8012640C)();
    r0 = (s16)r3;
    if ((s32)r3 < (s32)0x0) goto L_80205A54;
    if ((s32)r0 != (s32)r31) goto L_80205A54;
    r3 = 0x1;
    goto L_80205A68;
L_80205A54: ;
    r30 = r30 + 0x1;
L_80205A58: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80205A28;
    r3 = 0x0;
L_80205A68: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80205A7C | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205A7C(void) {
    extern void fn_801232E0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 == (u32)0x0) goto L_80205AC0;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r31;
    fn_801232E0();
L_80205AC0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80205AD4 | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205AD4(void) {
    extern void fn_80123368();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    if ((u32)r3 == (u32)0x0) goto L_80205B18;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r31;
    fn_80123368();
L_80205B18: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80205B2C | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205B2C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80205B48;
    r3 = -0x1;
    goto L_80205B7C;
L_80205B48: ;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205B68;
    r3 = -0x1;
    goto L_80205B7C;
L_80205B68: ;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = (s16)r3;
L_80205B7C: ;
    return;
}
#pragma pop

/* 0x80205BE8 | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205BE8(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80205C04;
    r3 = 0x0;
    goto L_80205C14;
L_80205C04: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80205C14: ;
    return;
}
#pragma pop

/* 0x80205C24 | size: 0x684 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80205C24(void) {
    extern u8 lbl_80375CA8[];
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_8011BEB4();
    extern void fn_80121ADC();
    extern void fn_801233F4();
    extern void fn_80123FBC();
    extern void fn_801EF634();
    extern void fn_801F0134();
    extern void fn_801F11CC();
    extern void fn_801F54A4();
    extern void fn_802099AC();
    extern void fn_80209CB4();
    extern void fn_8020D878();
    extern void fn_8022B2CC();
    extern void fn_802062A8();
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
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r27, 0xc(r1) */;
    r31 = r3;
    r29 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    if ((u32)r31 != (u32)0x0) goto L_80205C64;
    r3 = 0x0;
    goto L_80206294;
L_80205C64: ;
    if ((u32)r31 != (u32)0x0) goto L_80205C70;
    r0 = 0x0;
    goto L_80205F50;
L_80205C70: ;
    if ((u32)r31 != (u32)0x0) goto L_80205C7C;
    r0 = 0x0;
    goto L_80205D98;
L_80205C7C: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80205C94;
    r0 = 0x0;
    goto L_80205D98;
L_80205C94: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r28, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_80205CB8;
    r0 = 0x0;
    goto L_80205D98;
L_80205CB8: ;
    if ((u32)r0 != (u32)0x1) goto L_80205CC4;
    r0 = 0x0;
    goto L_80205D84;
L_80205CC4: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80205CDC;
    r0 = 0x0;
    goto L_80205D84;
L_80205CDC: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205D00;
    r0 = 0x0;
    goto L_80205D84;
L_80205D00: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205D14;
    r0 = 0x0;
    goto L_80205D84;
L_80205D14: ;
    if ((u32)r28 != (u32)0x0) goto L_80205D24;
    r3 = 0x0;
    goto L_80205D38;
L_80205D24: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80205D38: ;
    if ((u32)r3 != (u32)0x0) goto L_80205D48;
    r0 = 0x0;
    goto L_80205D84;
L_80205D48: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205D5C;
    r0 = 0x0;
    goto L_80205D84;
L_80205D5C: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_80205D80;
    r0 = 0x0;
    goto L_80205D84;
L_80205D80: ;
    r0 = 0x1;
L_80205D84: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80205D94;
    r0 = 0x0;
    goto L_80205D98;
L_80205D94: ;
    r0 = 0x1;
L_80205D98: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80205DA8;
    r0 = 0x0;
    goto L_80205F50;
L_80205DA8: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_80205DCC;
    r0 = 0x0;
    goto L_80205F50;
L_80205DCC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r28, r3 */;
    if ((s32)r3 != (s32)0x1) goto L_80205DF0;
    r0 = 0x0;
    goto L_80205F3C;
L_80205DF0: ;
    if ((s32)r3 != (s32)0x1) goto L_80205DFC;
    r0 = 0x0;
    goto L_80205EBC;
L_80205DFC: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80205E14;
    r0 = 0x0;
    goto L_80205EBC;
L_80205E14: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80205E38;
    r0 = 0x0;
    goto L_80205EBC;
L_80205E38: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205E4C;
    r0 = 0x0;
    goto L_80205EBC;
L_80205E4C: ;
    if ((u32)r28 != (u32)0x0) goto L_80205E5C;
    r3 = 0x0;
    goto L_80205E70;
L_80205E5C: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80205E70: ;
    if ((u32)r3 != (u32)0x0) goto L_80205E80;
    r0 = 0x0;
    goto L_80205EBC;
L_80205E80: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205E94;
    r0 = 0x0;
    goto L_80205EBC;
L_80205E94: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_80205EB8;
    r0 = 0x0;
    goto L_80205EBC;
L_80205EB8: ;
    r0 = 0x1;
L_80205EBC: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80205ECC;
    r0 = 0x0;
    goto L_80205F3C;
L_80205ECC: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_80205EF0;
    r0 = 0x0;
    goto L_80205F3C;
L_80205EF0: ;
    if ((u32)r28 != (u32)0x0) goto L_80205F00;
    r3 = 0x0;
    goto L_80205F14;
L_80205F00: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80205F14: ;
    if ((u32)r3 != (u32)0x0) goto L_80205F24;
    r0 = 0x0;
    goto L_80205F3C;
L_80205F24: ;
    fn_801233F4();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205F38;
    r0 = 0x0;
    goto L_80205F3C;
L_80205F38: ;
    r0 = 0x1;
L_80205F3C: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205F4C;
    r0 = 0x0;
    goto L_80205F50;
L_80205F4C: ;
    r0 = 0x1;
L_80205F50: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80205F60;
    r3 = 0x0;
    goto L_80206294;
L_80205F60: ;
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80205F9C;
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80205F9C;
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80206038;
L_80205F9C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r28 = r3;
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80205FDC;
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_8020600C;
L_80205FDC: ;
    if ((u32)r28 != (u32)0x0) goto L_80205FEC;
    r3 = 0x0;
    goto L_80206000;
L_80205FEC: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80206000: ;
    r4 = 0x12;
    fn_80121ADC();
    goto L_80206060;
L_8020600C: ;
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80206028;
    r3 = 0x0;
    goto L_80206060;
L_80206028: ;
    r3 = r28;
    r4 = 0x12;
    fn_8011B67C();
    goto L_80206060;
L_80206038: ;
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_80206054;
    r3 = 0x0;
    goto L_80206060;
L_80206054: ;
    r3 = r31;
    r4 = 0x12;
    fn_8011B67C();
L_80206060: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80206178;
    r3 = 0x22;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802060A8;
    r3 = 0x22;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_802060A8;
    r3 = 0x22;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80206144;
L_802060A8: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r28 = r3;
    r3 = 0x22;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_802060E8;
    r3 = 0x22;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80206118;
L_802060E8: ;
    if ((u32)r28 != (u32)0x0) goto L_802060F8;
    r3 = 0x0;
    goto L_8020610C;
L_802060F8: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020610C: ;
    r4 = 0x22;
    fn_80121ADC();
    goto L_8020616C;
L_80206118: ;
    r3 = 0x22;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80206134;
    r3 = 0x0;
    goto L_8020616C;
L_80206134: ;
    r3 = r28;
    r4 = 0x22;
    fn_8011B67C();
    goto L_8020616C;
L_80206144: ;
    r3 = 0x22;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_80206160;
    r3 = 0x0;
    goto L_8020616C;
L_80206160: ;
    r3 = r31;
    r4 = 0x22;
    fn_8011B67C();
L_8020616C: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80206290;
L_80206178: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80206288;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf8;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r28 = r3;
    fn_80209CB4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80206288;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_8011BEB4();
    r29 = r3 & 0xFFFF;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x26;
    r6 = 0x0;
    fn_8011BEB4();
    r4 = (u32)fn_802062A8;
    r27 = (s8)r3;
    r6 = (u32)fn_802062A8;
    r3 = r31;
    r4 = r29;
    r5 = r30;
    r7 = 0x1;
    r8 = 0x0;
    r9 = -0x1;
    fn_8022B2CC();
    r4 = r30;
    fn_801F0134();
    r0 = r3;
    r3 = r31;
    r28 = r0;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80206288;
    r4 = r27;
    r5 = r29;
    r6 = r28;
    r7 = 0x1;
    fn_802099AC();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r28, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_80206288;
    r4 = (u32)lbl_80375CA8;
    r5 = r31;
    r8 = (u32)lbl_80375CA8;
    r6 = 0x13;
    r4 = 0x0;
    r7 = 0x0;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80206288;
    r3 = r28;
    r4 = r29;
    fn_8020D878();
L_80206288: ;
    r3 = 0x0;
    goto L_80206294;
L_80206290: ;
    r3 = 0x1;
L_80206294: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x802062A8 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802062A8(void) {
    extern void fn_8011BEB4();
    extern void fn_801F00D0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r4 = 0x0;
    r6 = 0x0;
    r31 = r5;
    r5 = 0xf8;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    r4 = r31;
    fn_801F00D0();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802062FC | size: 0x30C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802062FC(void) {
    extern void fn_801233F4();
    extern void fn_80123FBC();
    extern void fn_801EF634();
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
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_8020631C;
    r3 = 0x0;
    goto L_802065F4;
L_8020631C: ;
    if ((s32)r0 != (s32)0) goto L_80206328;
    r0 = 0x0;
    goto L_80206444;
L_80206328: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80206340;
    r0 = 0x0;
    goto L_80206444;
L_80206340: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_80206364;
    r0 = 0x0;
    goto L_80206444;
L_80206364: ;
    if ((u32)r0 != (u32)0x1) goto L_80206370;
    r0 = 0x0;
    goto L_80206430;
L_80206370: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80206388;
    r0 = 0x0;
    goto L_80206430;
L_80206388: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802063AC;
    r0 = 0x0;
    goto L_80206430;
L_802063AC: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802063C0;
    r0 = 0x0;
    goto L_80206430;
L_802063C0: ;
    if ((u32)r31 != (u32)0x0) goto L_802063D0;
    r3 = 0x0;
    goto L_802063E4;
L_802063D0: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802063E4: ;
    if ((u32)r3 != (u32)0x0) goto L_802063F4;
    r0 = 0x0;
    goto L_80206430;
L_802063F4: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80206408;
    r0 = 0x0;
    goto L_80206430;
L_80206408: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_8020642C;
    r0 = 0x0;
    goto L_80206430;
L_8020642C: ;
    r0 = 0x1;
L_80206430: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80206440;
    r0 = 0x0;
    goto L_80206444;
L_80206440: ;
    r0 = 0x1;
L_80206444: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80206454;
    r3 = 0x0;
    goto L_802065F4;
L_80206454: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_80206478;
    r3 = 0x0;
    goto L_802065F4;
L_80206478: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r3 != (s32)0x1) goto L_8020649C;
    r0 = 0x0;
    goto L_802065E8;
L_8020649C: ;
    if ((s32)r3 != (s32)0x1) goto L_802064A8;
    r0 = 0x0;
    goto L_80206568;
L_802064A8: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802064C0;
    r0 = 0x0;
    goto L_80206568;
L_802064C0: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802064E4;
    r0 = 0x0;
    goto L_80206568;
L_802064E4: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802064F8;
    r0 = 0x0;
    goto L_80206568;
L_802064F8: ;
    if ((u32)r31 != (u32)0x0) goto L_80206508;
    r3 = 0x0;
    goto L_8020651C;
L_80206508: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020651C: ;
    if ((u32)r3 != (u32)0x0) goto L_8020652C;
    r0 = 0x0;
    goto L_80206568;
L_8020652C: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80206540;
    r0 = 0x0;
    goto L_80206568;
L_80206540: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_80206564;
    r0 = 0x0;
    goto L_80206568;
L_80206564: ;
    r0 = 0x1;
L_80206568: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80206578;
    r0 = 0x0;
    goto L_802065E8;
L_80206578: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_8020659C;
    r0 = 0x0;
    goto L_802065E8;
L_8020659C: ;
    if ((u32)r31 != (u32)0x0) goto L_802065AC;
    r3 = 0x0;
    goto L_802065C0;
L_802065AC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802065C0: ;
    if ((u32)r3 != (u32)0x0) goto L_802065D0;
    r0 = 0x0;
    goto L_802065E8;
L_802065D0: ;
    fn_801233F4();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802065E4;
    r0 = 0x0;
    goto L_802065E8;
L_802065E4: ;
    r0 = 0x1;
L_802065E8: ;
    r3 = r0 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_802065F4: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80206608 | size: 0x178 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80206608(void) {
    extern void fn_801233F4();
    extern void fn_80123FBC();
    extern void fn_801EF634();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_80206628;
    r3 = 0x0;
    goto L_8020676C;
L_80206628: ;
    if ((s32)r0 != (s32)0) goto L_80206634;
    r0 = 0x0;
    goto L_802066F4;
L_80206634: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8020664C;
    r0 = 0x0;
    goto L_802066F4;
L_8020664C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80206670;
    r0 = 0x0;
    goto L_802066F4;
L_80206670: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80206684;
    r0 = 0x0;
    goto L_802066F4;
L_80206684: ;
    if ((u32)r31 != (u32)0x0) goto L_80206694;
    r3 = 0x0;
    goto L_802066A8;
L_80206694: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802066A8: ;
    if ((u32)r3 != (u32)0x0) goto L_802066B8;
    r0 = 0x0;
    goto L_802066F4;
L_802066B8: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802066CC;
    r0 = 0x0;
    goto L_802066F4;
L_802066CC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_802066F0;
    r0 = 0x0;
    goto L_802066F4;
L_802066F0: ;
    r0 = 0x1;
L_802066F4: ;
    r0 = r0 & 0xFF;
    if ((s32)r3 != (s32)0x0) goto L_80206704;
    r3 = 0x0;
    goto L_8020676C;
L_80206704: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x1) goto L_80206728;
    r3 = 0x0;
    goto L_8020676C;
L_80206728: ;
    if ((u32)r31 != (u32)0x0) goto L_80206738;
    r3 = 0x0;
    goto L_8020674C;
L_80206738: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020674C: ;
    if ((u32)r3 != (u32)0x0) goto L_8020675C;
    r3 = 0x0;
    goto L_8020676C;
L_8020675C: ;
    fn_801233F4();
    r3 = r3 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_8020676C: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80206780 | size: 0x148 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80206780(void) {
    extern void fn_80123FBC();
    extern void fn_801EF634();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_802067A0;
    r3 = 0x0;
    goto L_802068B4;
L_802067A0: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_802067B8;
    r3 = 0x0;
    goto L_802068B4;
L_802067B8: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_802067DC;
    r3 = 0x0;
    goto L_802068B4;
L_802067DC: ;
    if ((u32)r0 != (u32)0x1) goto L_802067E8;
    r0 = 0x0;
    goto L_802068A8;
L_802067E8: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80206800;
    r0 = 0x0;
    goto L_802068A8;
L_80206800: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80206824;
    r0 = 0x0;
    goto L_802068A8;
L_80206824: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80206838;
    r0 = 0x0;
    goto L_802068A8;
L_80206838: ;
    if ((u32)r31 != (u32)0x0) goto L_80206848;
    r3 = 0x0;
    goto L_8020685C;
L_80206848: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_8020685C: ;
    if ((u32)r3 != (u32)0x0) goto L_8020686C;
    r0 = 0x0;
    goto L_802068A8;
L_8020686C: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80206880;
    r0 = 0x0;
    goto L_802068A8;
L_80206880: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 >= (s32)0x0) goto L_802068A4;
    r0 = 0x0;
    goto L_802068A8;
L_802068A4: ;
    r0 = 0x1;
L_802068A8: ;
    r3 = r0 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_802068B4: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802068C8 | size: 0x13C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802068C8(void) {
    extern void fn_801248C4();
    extern void fn_801F198C();
    extern void fn_80206C94();
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
    /* mr. r28, r3 */;
    r30 = r4;
    r29 = r5;
    if ((s32)r0 == (s32)0) goto L_802069F0;
    if ((u32)r30 == (u32)0x0) goto L_802069F0;
    if ((u32)r30 != (u32)0x0) goto L_802068FC;
    r31 = 0x0;
    goto L_80206914;
L_802068FC: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
L_80206914: ;
    r3 = r28;
    fn_80206C94();
    r3 = r28;
    r7 = r30;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r28;
    r7 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    if ((u32)r29 == (u32)0x0) goto L_80206970;
    r3 = r28;
    r7 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    fn_801F198C();
L_80206970: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = r3 & 0xFFFF;
    r29 = 0x0;
    goto L_802069C0;
L_80206990: ;
    r4 = r30;
    r6 = r29;
    r3 = 0x0;
    r5 = 0x16;
    ((void(*)(void))fn_8012640C)();
    r7 = r3 & 0xFFFF;
    r6 = r29 & 0xFF;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xff;
    ((void(*)(void))fn_801254B4)();
    r29 = r29 + 0x1;
L_802069C0: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_80206990;
    r3 = r31;
    fn_801248C4();
    r0 = r3;
    r3 = r28;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x100;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_802069F0: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80206A04 | size: 0xE8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80206A04(void) {
    extern void fn_80123FBC();
    extern void fn_801EF634();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_80206A24;
    r3 = 0x0;
    goto L_80206AD8;
L_80206A24: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_80206A3C;
    r3 = 0x0;
    goto L_80206AD8;
L_80206A3C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80206A60;
    r3 = 0x0;
    goto L_80206AD8;
L_80206A60: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80206A74;
    r3 = 0x0;
    goto L_80206AD8;
L_80206A74: ;
    if ((u32)r31 != (u32)0x0) goto L_80206A84;
    r3 = 0x0;
    goto L_80206A98;
L_80206A84: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80206A98: ;
    if ((u32)r3 != (u32)0x0) goto L_80206AA8;
    r3 = 0x0;
    goto L_80206AD8;
L_80206AA8: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80206ABC;
    r3 = 0x0;
    goto L_80206AD8;
L_80206ABC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = (u32)r3 >> 31;
    r3 = r0 ^ 0x1;
L_80206AD8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80206AEC | size: 0x150 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80206AEC(void) {
    extern void fn_8011B950();
    extern void fn_8011F5FC();
    extern void fn_80124A60();
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

    /* stmw r29, 0x14(r1) */;
    /* mr. r29, r3 */;
    r30 = r4;
    r31 = r5;
    if ((s32)r0 == (s32)0) goto L_80206C28;
    if ((u32)r30 == (u32)0x0) goto L_80206C28;
    if ((u32)r29 == (u32)0x0) goto L_80206BDC;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80124A60();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcd;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x1;
    fn_8011B950();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    r7 = -0x1;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcf;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd0;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd1;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_80206BDC: ;
    r3 = r29;
    r7 = r30;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r30;
    fn_8011F5FC();
    r3 = r29;
    r7 = (s16)r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_80206C28: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80206C3C | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80206C3C(void) {
    extern void fn_80206C94();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r29, 0x14(r1) */;
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) goto L_80206C80;
    r31 = r4 & 0xFFFF;
    r30 = 0x0;
    goto L_80206C74;
L_80206C60: ;
    r0 = r30 & 0xFFFF;
    r0 = r0 * 0x6e0;
    r3 = r29 + r0;
    fn_80206C94();
    r30 = r30 + 0x1;
L_80206C74: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_80206C60;
L_80206C80: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80206C94 | size: 0x72C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80206C94(void) {
    extern u8 lbl_80279C60[];
    extern void fn_8011B950();
    extern void fn_80124A60();
    extern void fn_801F1460();
    extern void fn_801FD830();
    extern void fn_80209D90();
    extern void fn_8020A478();
    extern void fn_8020E6D4();
    u8 sp[0x30];
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;

    /* stmw r29, 0x24(r1) */;
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_802073AC;
    r4 = 0x0;
    r5 = 0xd5;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd7;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) goto L_80206DB4;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80124A60();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcd;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x1;
    fn_8011B950();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    r7 = -0x1;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcf;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd0;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd1;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_80206DB4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd8;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x34;
    fn_8011B950();
    r3 = (u32)lbl_80279C60;
    r30 = r1 + 0x8;
    r6 = (u32)lbl_80279C60;
    r29 = 0x0;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r3 = *(u32*)((u8*)r6 + 0x8);
    r0 = *(u16*)((u8*)r6 + 0xC);
    *(u16*)(sp + 0x14) = r0;
    goto L_80206E24;
L_80206E04: ;
    /* clrlslwi r0, r29, 24, 1 */;
    r3 = r31;
    r5 = *(u16*)(r30 + r0);
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x6;
    ((void(*)(void))fn_801254B4)();
    r29 = r29 + 0x1;
L_80206E24: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80206E04;
    r3 = r31;
    r4 = 0x0;
    fn_801FD830();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xed;
    r6 = 0x0;
    r7 = 0x2;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r29 = 0x0;
    goto L_80206E90;
L_80206E74: ;
    r3 = r31;
    r6 = r29 & 0xFF;
    r4 = 0x0;
    r5 = 0xfd;
    r7 = -0x1;
    ((void(*)(void))fn_801254B4)();
    r29 = r29 + 0x1;
L_80206E90: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 < (u32)0xc) goto L_80206E74;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80206EEC;
    fn_801F1460();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80209D90();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_8020A478();
L_80206EEC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf8;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80209D90();
    r29 = 0x0;
    goto L_80206F28;
L_80206F0C: ;
    r3 = r31;
    r6 = r29 & 0xFF;
    r4 = 0x0;
    r5 = 0xff;
    r7 = 0x9;
    ((void(*)(void))fn_801254B4)();
    r29 = r29 + 0x1;
L_80206F28: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_80206F0C;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x100;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x101;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80206F70;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x0) = r0;
L_80206F70: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xef;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf0;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf1;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf2;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf3;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf4;
    r6 = 0x0;
    r7 = 0x9;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf5;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf6;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf7;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf9;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfc;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfb;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x102;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x103;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x104;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x105;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x106;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x107;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x108;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x109;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10a;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10b;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10c;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10d;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10e;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10f;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x110;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x111;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x113;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x114;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x115;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x116;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x117;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x118;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x119;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11a;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11b;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11c;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11d;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11e;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11f;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x121;
    r6 = 0x0;
    r7 = -0x1;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x122;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x4;
    fn_8020E6D4();
L_802073AC: ;
    /* lmw r29, 0x24(r1) */;
    return;
}
#pragma pop

/* 0x802073C0 | size: 0x88 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802073C0(void) {
    extern u8 lbl_80279C60[];
    u8 sp[0x30];
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

    r4 = (u32)lbl_80279C60;
    r6 = (u32)lbl_80279C60;
    /* stmw r29, 0x24(r1) */;
    r29 = r3;
    r31 = r1 + 0x8;
    r30 = 0x0;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r3 = *(u32*)((u8*)r6 + 0x8);
    r0 = *(u16*)((u8*)r6 + 0xC);
    *(u16*)(sp + 0x14) = r0;
    goto L_80207428;
L_80207408: ;
    /* clrlslwi r0, r30, 24, 1 */;
    r3 = r29;
    r5 = *(u16*)(r31 + r0);
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x6;
    ((void(*)(void))fn_801254B4)();
    r30 = r30 + 0x1;
L_80207428: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 < (u32)0x7) goto L_80207408;
    /* lmw r29, 0x24(r1) */;
    return;
}
#pragma pop

/* 0x80207448 | size: 0x15C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80207448(void) {
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
    r5 = 0x113;
    r6 = 0x0;
    r7 = 0x0;
    r31 = r3;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x114;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x115;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x116;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x117;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x118;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x119;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11a;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11b;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11c;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11d;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11e;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x11f;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802075A4 | size: 0x1BC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802075A4(void) {
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
    r5 = 0x102;
    r6 = 0x0;
    r7 = 0x0;
    r31 = r3;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x103;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x104;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x105;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x106;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x107;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x108;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x109;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10a;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10b;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10c;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10d;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10e;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x10f;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x110;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x111;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80207760 | size: 0x74 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80207760(void) {
    extern void fn_801F1460();
    extern void fn_80209D90();
    extern void fn_8020A478();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    r31 = r3;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_802077C0;
    fn_801F1460();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80209D90();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_8020A478();
L_802077C0: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802077D4 | size: 0x11C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802077D4(void) {
    extern void fn_8011B950();
    extern void fn_80124A60();
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
    /* mr. r28, r3 */;
    if ((s32)r0 == (s32)0) goto L_802078DC;
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    goto L_802078D0;
L_802077F8: ;
    r0 = r29 & 0xFFFF;
    r0 = r0 * 0x154;
    /* add. r30, r28, r0 */;
    if ((s32)r0 == (s32)0) goto L_802078CC;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80124A60();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcd;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x1;
    fn_8011B950();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    r7 = -0x1;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xcf;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd0;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd1;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_802078CC: ;
    r29 = r29 + 0x1;
L_802078D0: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_802077F8;
L_802078DC: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x802078F0 | size: 0xEC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802078F0(void) {
    extern void fn_8011B950();
    extern void fn_80124A60();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_802079C8;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80124A60();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcd;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x1;
    fn_8011B950();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    r7 = -0x1;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcf;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd0;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd1;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
L_802079C8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802079DC | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802079DC(void) {
    extern void fn_8010C74C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r4 = 0x0;
    r3 = -0x1;
    goto L_80207A10;
L_80207A04: ;
    /* clrlslwi r0, r4, 16, 2 */;
    r4 = r4 + 0x1;
    *(u32*)(r31 + r0) = r3;
L_80207A10: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x12) goto L_80207A04;
    r27 = 0x0;
    r28 = 0x0;
    goto L_80207ABC;
L_80207A28: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 == (u32)r3) goto L_80207A70;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x1;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_80207A78;
L_80207A70: ;
    r0 = 0x1;
    goto L_80207A7C;
L_80207A78: ;
    r0 = 0x0;
L_80207A7C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80207AB8;
    r3 = r30;
    r4 = r28;
    fn_8010C74C();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x42) goto L_80207AA8;
    if ((u32)r0 != (u32)0x43) goto L_80207AB8;
L_80207AA8: ;
    /* clrlslwi r0, r27, 16, 2 */;
    r3 = r28 & 0xFFFF;
    *(u32*)(r31 + r0) = r3;
    r27 = r27 + 0x1;
L_80207AB8: ;
    r28 = r28 + 0x1;
L_80207ABC: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x12) goto L_80207A28;
    r3 = r27;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80207AE0 | size: 0x7C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80207AE0(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0xff;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)r3) goto L_80207B3C;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x1;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)r3) goto L_80207B44;
L_80207B3C: ;
    r3 = 0x1;
    goto L_80207B48;
L_80207B44: ;
    r3 = 0x0;
L_80207B48: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80207B5C | size: 0x30 */
u32 fn_80207B5C(void* context, u8 flags, u16 value) {
    return fn_801254B4(context, 0, 0xFF, (u8)flags, (u16)value);
}

/* 0x80207B8C | size: 0x34 */
u16 fn_80207B8C(void* context, u8 field) {
    return (u16)(u32)fn_8012640C(context, 0, 0xFF, (u8)field);
}

/* 0x80207BC0 | size: 0x34 */
u32 fn_80207BC0(void* context, u16 value) {
    return fn_801254B4(context, 0, 0x100, 0, (u16)value);
}

/* 0x80207C24 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80207C24(void) {
    extern void fn_801DA5AC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r5 = 0xee;
    r6 = 0x0;
    r31 = r4;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80207C58;
    r4 = r31;
    fn_801DA5AC();
L_80207C58: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80207C6C | size: 0x2F0 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80207C6C(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_8011F5FC();
    extern void fn_80121ADC();
    extern void fn_80122040();
    extern void fn_80125390();
    extern void fn_801DE190();
    extern void fn_801FDB78();
    u8 sp[0x160];
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

    /* stmw r28, 0x150(r1) */;
    /* mr. r30, r3 */;
    r28 = r4;
    if ((s32)r0 != (s32)0) goto L_80207C90;
    r3 = 0x0;
    goto L_80207CC0;
L_80207C90: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80207CB0;
    r3 = 0x0;
    goto L_80207CC0;
L_80207CB0: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80207CC0: ;
    r4 = r3;
    r3 = r1 + 0x10;
    fn_8011F5FC();
    r3 = r1 + 0x10;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r29 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = r29;
    r5 = 0x66;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = r3;
    if ((u32)r29 != (u32)0x181) goto L_80207D50;
    r0 = r28 & 0xFFFF;
    if ((s32)r0 == (s32)0x3) goto L_80207D48;
    if ((s32)r0 >= (s32)0x3) goto L_80207D24;
    if ((s32)r0 == (s32)0x1) goto L_80207D30;
    if ((s32)r0 >= (s32)0x1) goto L_80207D38;
    goto L_80207D48;
L_80207D24: ;
    if ((s32)r0 >= (s32)0x5) goto L_80207D48;
    goto L_80207D40;
L_80207D30: ;
    r0 = 0x19f;
    goto L_80207D4C;
L_80207D38: ;
    r0 = 0x19e;
    goto L_80207D4C;
L_80207D40: ;
    r0 = 0x1a0;
    goto L_80207D4C;
L_80207D48: ;
    r0 = 0x181;
L_80207D4C: ;
    r31 = r0;
L_80207D50: ;
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80207D8C;
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xc8) goto L_80207D8C;
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xcd) goto L_80207E28;
L_80207D8C: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r29 = r3;
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x7c) goto L_80207DCC;
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0xc8) goto L_80207DFC;
L_80207DCC: ;
    if ((u32)r29 != (u32)0x0) goto L_80207DDC;
    r3 = 0x0;
    goto L_80207DF0;
L_80207DDC: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80207DF0: ;
    r4 = 0x14;
    fn_80121ADC();
    goto L_80207E50;
L_80207DFC: ;
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xcd) goto L_80207E18;
    r3 = 0x0;
    goto L_80207E50;
L_80207E18: ;
    r3 = r29;
    r4 = 0x14;
    fn_8011B67C();
    goto L_80207E50;
L_80207E28: ;
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0xd8) goto L_80207E44;
    r3 = 0x0;
    goto L_80207E50;
L_80207E44: ;
    r3 = r30;
    r4 = 0x14;
    fn_8011B67C();
L_80207E50: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80207E60;
    r31 = 0x19d;
L_80207E60: ;
    if ((u32)r31 != (u32)0x0) goto L_80207E70;
    r3 = 0x0;
    goto L_80207F48;
L_80207E70: ;
    r3 = r30;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    fn_801FDB78();
    r7 = *(u32*)(sp + 0xC);
    r3 = r1 + 0x10;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r7 = *(u32*)(sp + 0x8);
    r3 = r1 + 0x10;
    r4 = 0x0;
    r5 = 0x75;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r1 + 0x10;
    fn_80125390();
    r4 = *(u32*)(sp + 0xC);
    r5 = r3;
    r3 = r31 & 0xFFFF;
    fn_801DE190();
    r0 = r3;
    r3 = r1 + 0x10;
    r29 = r0;
    r4 = r29;
    fn_80122040();
    if ((u32)r30 != (u32)0x0) goto L_80207EEC;
    r3 = 0x0;
    goto L_80207F20;
L_80207EEC: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80207F10;
    r3 = 0x0;
    goto L_80207F20;
L_80207F10: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80207F20: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r3 = r29;
L_80207F48: ;
    /* lmw r28, 0x150(r1) */;
    return;
}
#pragma pop

/* 0x80207F5C | size: 0xCC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80207F5C(void) {
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
    /* mr. r29, r3 */;
    r30 = r5;
    r31 = *(u32*)((u8*)r5 + 0x0);
    if ((s32)r0 != (s32)0) goto L_80207F84;
    r3 = 0x0;
    goto L_80207FB4;
L_80207F84: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80207FA4;
    r3 = 0x0;
    goto L_80207FB4;
L_80207FA4: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80207FB4: ;
    if ((u32)r29 != (u32)r31) goto L_80207FC4;
    r3 = 0x1;
    goto L_80208014;
L_80207FC4: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    if ((u32)r3 != (u32)0x0) goto L_80207FF8;
    r3 = 0x1;
    goto L_80208014;
L_80207FF8: ;
    r0 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r0 != (u32)r3) goto L_80208010;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r30 + 0x8) = r0;
L_80208010: ;
    r3 = 0x1;
L_80208014: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80208028 | size: 0x80 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80208028(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 != (u32)0x0) goto L_80208044;
    r3 = 0x0;
    goto L_80208074;
L_80208044: ;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80208064;
    r3 = 0x0;
    goto L_80208074;
L_80208064: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80208074: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    return;
}
#pragma pop

/* 0x802080A8 | size: 0x35C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802080A8(void) {
    extern void fn_800F0308();
    extern void fn_80166A50();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
    extern void fn_801F54A4();
    extern void fn_802624CC();
    extern void fn_802653FC();
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

    /* stmw r24, 0x10(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r24 = r7;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r31 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r26, r3 */;
    if ((s32)r0 == (s32)0) goto L_802083F0;
    r0 = r24 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020816C;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208124;
    r4 = 0xa3;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
L_80208124: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208144;
    r3 = r26;
    r4 = 0x9f;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
L_80208144: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802083F0;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802083F0;
    r3 = r26;
    r4 = 0x57;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    goto L_802083F0;
L_8020816C: ;
    if ((u32)r0 != (u32)0x1) goto L_80208338;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208228;
    r4 = 0xa3;
    r5 = 0x4;
    fn_801DA9E8();
    if ((u32)r27 != (u32)0x0) goto L_8020819C;
    r3 = 0x0;
    goto L_802081D0;
L_8020819C: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802081C0;
    r3 = 0x0;
    goto L_802081D0;
L_802081C0: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802081D0: ;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x61;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x0;
    fn_80166A50();
    r3 = r30;
    fn_802624CC();
    r0 = r29 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80208228;
    r3 = r27;
    r4 = r31;
    r5 = 0x1;
    fn_802653FC();
L_80208228: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020830C;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208260;
L_80208240: ;
    r3 = r26;
    r4 = 0xa3;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80208260;
    fn_800F0308();
    goto L_80208240;
L_80208260: ;
    r3 = r26;
    r4 = 0x9f;
    r5 = 0x4;
    fn_801DA9E8();
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802082FC;
    if ((u32)r27 != (u32)0x0) goto L_80208288;
    r3 = 0x0;
    goto L_802082BC;
L_80208288: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802082AC;
    r3 = 0x0;
    goto L_802082BC;
L_802082AC: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802082BC: ;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x61;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x0;
    fn_80166A50();
    r3 = r30;
    fn_802624CC();
L_802082FC: ;
    r3 = r27;
    r4 = r31;
    r5 = 0x1;
    fn_802653FC();
L_8020830C: ;
    r0 = r28 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802083F0;
    r0 = r29 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_802083F0;
    r3 = r26;
    r4 = 0x57;
    r5 = 0x4;
    fn_801DA9E8();
    r3 = r30;
    fn_802624CC();
    goto L_802083F0;
L_80208338: ;
    if ((u32)r0 != (u32)0x2) goto L_80208394;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208350;
    r25 = 0xa3;
L_80208350: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208360;
    r25 = 0x9f;
L_80208360: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208374;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208374;
    r25 = 0x57;
L_80208374: ;
    r3 = r26;
    r4 = r25;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802083F0;
    fn_800F0308();
    goto L_80208374;
L_80208394: ;
    if ((u32)r0 != (u32)0x3) goto L_802083F0;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802083B4;
    r4 = 0xa3;
    r5 = 0x4;
    fn_801DA8C4();
L_802083B4: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802083D0;
    r3 = r26;
    r4 = 0x9f;
    r5 = 0x4;
    fn_801DA8C4();
L_802083D0: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802083F0;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802083F0;
    r3 = r26;
    r4 = 0x57;
    r5 = 0x4;
    fn_801DA8C4();
L_802083F0: ;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80208404 | size: 0x150 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80208404(void) {
    extern void fn_800F0308();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9B4();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
    extern void fn_801F54A4();
    extern void fn_80265598();
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

    /* stmw r26, 0x8(r1) */;
    r26 = r3;
    r27 = r4;
    r29 = r5;
    r28 = r6;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_80208540;
    r0 = r29 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80208468;
    r29 = 0x3a;
    goto L_8020848C;
L_80208468: ;
    if ((u32)r0 != (u32)0x1) goto L_80208478;
    r29 = 0x88;
    goto L_8020848C;
L_80208478: ;
    if ((u32)r0 != (u32)0x2) goto L_80208488;
    r29 = 0x57;
    goto L_8020848C;
L_80208488: ;
    r29 = 0xd9;
L_8020848C: ;
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x2) goto L_802084AC;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    goto L_80208540;
L_802084AC: ;
    if ((u32)r0 != (u32)0x1) goto L_802084E4;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801DA9E8();
    r0 = r27 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208540;
    r3 = r26;
    r4 = r30;
    r5 = 0x1;
    fn_80265598();
    goto L_80208540;
L_802084E4: ;
    if ((u32)r0 != (u32)0x2) goto L_8020850C;
L_802084EC: ;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_80208540;
    fn_800F0308();
    goto L_802084EC;
L_8020850C: ;
    if ((u32)r0 != (u32)0x3) goto L_80208528;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801DA8C4();
    goto L_80208540;
L_80208528: ;
    if ((u32)r0 != (u32)0x4) goto L_80208540;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801DA9B4();
L_80208540: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80208554 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80208554(void) {
    extern void fn_800F0308();
    extern void fn_801DA698();
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
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_802085B0;
L_80208588: ;
    r3 = r31;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    fn_801DA698();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802085B0;
    fn_800F0308();
    goto L_80208588;
L_802085B0: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x802085C4 | size: 0xEC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802085C4(void) {
    extern void fn_80102568();
    extern void fn_801026A4();
    extern void fn_801F54A4();
    extern void fn_801FE168();
    extern void fn_802094CC();
    extern void fn_802656AC();
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
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x44(r1) */;
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r29 = r7;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_8020869C;
    r3 = r25;
    r4 = r1 + 0x8;
    fn_801FE168();
    if ((s32)r29 < (s32)0x0) goto L_80208664;
    r3 = r25;
    r4 = r30;
    r5 = 0x1;
    fn_802656AC();
    r9 = r1 + 0x8;
    r29 = r3;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
L_80208664: ;
    r3 = r31;
    r4 = r26;
    r5 = r27;
    r6 = r28;
    fn_802094CC();
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020869C;
    if ((s32)r29 < (s32)0x0) goto L_8020869C;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x0;
    fn_80102568();
L_8020869C: ;
    /* lmw r25, 0x44(r1) */;
    return;
}
#pragma pop

/* 0x802086B0 | size: 0x38 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802086B0(void) {
    extern void fn_801DA83C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_802086D8;
    fn_801DA83C();
L_802086D8: ;
    return;
}
#pragma pop

/* 0x802086E8 | size: 0x68 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802086E8(void) {
    extern void fn_8011BEB4();
    extern void fn_801DA8C4();
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

    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    r30 = r5;
    r3 = 0x0;
    r5 = 0x1f;
    fn_8011BEB4();
    r31 = r3;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_8020873C;
    r5 = r30;
    r4 = r31 & 0xFFFF;
    fn_801DA8C4();
L_8020873C: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80208750 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80208750(void) {
    extern void fn_8011BEB4();
    extern void fn_801DDD28();
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
    r29 = r5;
    r30 = r6;
    r3 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_8011BEB4();
    r31 = r3;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_802087AC;
    r5 = r29;
    r6 = r30;
    r4 = r31 & 0xFFFF;
    fn_801DDD28();
L_802087AC: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x802087C0 | size: 0x458 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802087C0(void) {
    extern void fn_800F0308();
    extern void fn_801C3C98();
    extern void fn_801DA224();
    extern void fn_801DA2C4();
    extern void fn_801DA354();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB100();
    extern void fn_801DDD28();
    extern void fn_801F37B0();
    extern void fn_801F54A4();
    extern void fn_8026532C();
    extern void fn_80207F5C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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

    /* stmw r22, 0x18(r1) */;
    r30 = r4;
    r22 = r5;
    r28 = r6;
    r29 = r3;
    r23 = r7;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_80208C04;
    r4 = r22;
    r3 = 0x0;
    r5 = 0x17;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r24 = r3;
    r4 = r22;
    r3 = 0x0;
    r5 = 0x13;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r25 = r3;
    r4 = r22;
    r3 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r26 = r3;
    r4 = r22;
    r3 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r27 = r3;
    r4 = r22;
    r3 = 0x0;
    r5 = 0x15;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r0 = r28 & 0xFF;
    r28 = r3;
    if ((s32)r0 != (s32)0) goto L_80208900;
    r3 = r31;
    r4 = r24 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r3 = r31;
    r4 = r25 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r3 = r31;
    r4 = r26 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r3 = r31;
    r4 = r27 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r3 = r31;
    r4 = r28 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    if ((u32)r23 == (u32)0x0) goto L_80208C04;
    r3 = r31;
    fn_801DA354();
    *(u8*)((u8*)r23 + 0x0) = r3;
    r3 = r31;
    fn_801DA2C4();
    goto L_80208C04;
L_80208900: ;
    if ((u32)r0 != (u32)0x1) goto L_802089F4;
    r3 = r31;
    r4 = r24 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
L_80208918: ;
    r3 = r31;
    r4 = r24 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80208938;
    fn_800F0308();
    goto L_80208918;
L_80208938: ;
    r3 = r31;
    r4 = r25 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
L_80208948: ;
    r3 = r31;
    r4 = r25 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80208968;
    fn_800F0308();
    goto L_80208948;
L_80208968: ;
    r24 = 0x0;
    r29 = r30 & 0xFF;
L_80208970: ;
    r3 = r31;
    r4 = r26 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
L_80208980: ;
    r3 = r31;
    r4 = r26 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802089A0;
    fn_800F0308();
    goto L_80208980;
L_802089A0: ;
    r24 = r24 + 0x1;
    r0 = r24 & 0xFF;
    if ((u32)r0 >= (u32)0x3) goto L_802089B8;
    if ((u32)r0 < (u32)r29) goto L_80208970;
L_802089B8: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 >= (u32)0x4) goto L_80208C04;
    r3 = r31;
    r4 = r27 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
L_802089D4: ;
    r3 = r31;
    r4 = r27 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x4) goto L_80208C04;
    fn_800F0308();
    goto L_802089D4;
L_802089F4: ;
    if ((u32)r0 != (u32)0x2) goto L_80208A1C;
    r0 = r30 & 0xFF;
    if ((u32)r0 >= (u32)0x4) goto L_80208C04;
    r3 = r31;
    r4 = r28 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
    goto L_80208C04;
L_80208A1C: ;
    if ((u32)r0 != (u32)0x3) goto L_80208A64;
    r0 = r30 & 0xFF;
    if ((u32)r0 >= (u32)0x4) goto L_80208C04;
    if ((u32)r23 == (u32)0x0) goto L_80208A44;
    r4 = *(u8*)((u8*)r23 + 0x0);
    r3 = r31;
    fn_801DA224();
L_80208A44: ;
    r3 = r31;
    r4 = r28 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r23 == (u32)0x0) goto L_80208C04;
    fn_800F0308();
    goto L_80208A44;
L_80208A64: ;
    if ((u32)r0 != (u32)0x4) goto L_80208C04;
    r3 = r31;
    r4 = r24 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r31;
    r4 = r25 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r31;
    r4 = r26 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r31;
    r4 = r27 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r31;
    r4 = r28 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r0 = r30 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_80208BF0;
    if ((u32)r29 != (u32)0x0) goto L_80208AD8;
    r3 = 0x0;
    goto L_80208B0C;
L_80208AD8: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80208AFC;
    r3 = 0x0;
    goto L_80208B0C;
L_80208AFC: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80208B0C: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    if ((u32)r3 == (u32)0x0) goto L_80208B60;
    r0 = 0x0;
    r4 = (u32)fn_80207F5C;
    r4 = (u32)fn_80207F5C;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
L_80208B60: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r24, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_80208BC8;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80208BA0;
    r4 = 0x0;
    fn_801DA4E8();
L_80208BA0: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r24;
    fn_801C3C98();
    r3 = r24;
    fn_801DB100();
L_80208BC8: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r29;
    r5 = 0x1;
    fn_8026532C();
    goto L_80208C04;
L_80208BF0: ;
    if ((u32)r23 == (u32)0x0) goto L_80208C04;
    r4 = *(u8*)((u8*)r23 + 0x0);
    r3 = r31;
    fn_801DA224();
L_80208C04: ;
    /* lmw r22, 0x18(r1) */;
    return;
}
#pragma pop

/* 0x80208C18 | size: 0x2B8 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80208C18(void) {
    extern void fn_800F0308();
    extern void fn_80125390();
    extern void fn_80166A50();
    extern void fn_801DA5C4();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
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

    r5 = 0xee;
    r6 = 0x0;
    /* stmw r26, 0x8(r1) */;
    r27 = r4;
    r26 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_80208EBC;
    if ((u32)r26 != (u32)0x0) goto L_80208C58;
    r28 = 0x0;
    goto L_80208C90;
L_80208C58: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80208C7C;
    r3 = 0x0;
    goto L_80208C8C;
L_80208C7C: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80208C8C: ;
    r28 = r3;
L_80208C90: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r30 = r4;
    r5 = 0xe;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r29 = r3;
    r4 = r30;
    r3 = 0x0;
    r5 = 0xf;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r0 = r27 & 0xFF;
    r30 = r3;
    if ((u32)r3 != (u32)0x0) goto L_80208D20;
    r3 = r31;
    r4 = r29 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r3 = r31;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r3 = r31;
    r4 = 0x67;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    goto L_80208EBC;
L_80208D20: ;
    if ((u32)r0 != (u32)0x1) goto L_80208D3C;
    r3 = r31;
    r4 = r29 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
    goto L_80208EBC;
L_80208D3C: ;
    if ((u32)r0 != (u32)0x2) goto L_80208D64;
L_80208D44: ;
    r3 = r31;
    r4 = r29 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_80208EBC;
    fn_800F0308();
    goto L_80208D44;
L_80208D64: ;
    if ((u32)r0 != (u32)0x3) goto L_80208D80;
    r3 = r31;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
    goto L_80208EBC;
L_80208D80: ;
    if ((u32)r0 != (u32)0x4) goto L_80208E84;
L_80208D88: ;
    r3 = 0x0;
    fn_801DA5C4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80208DA4;
    fn_800F0308();
    goto L_80208D88;
L_80208DA4: ;
    if ((u32)r26 != (u32)0x0) goto L_80208DB4;
    r3 = 0x0;
    goto L_80208DE8;
L_80208DB4: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80208DD8;
    r3 = 0x0;
    goto L_80208DE8;
L_80208DD8: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80208DE8: ;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x61;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x0;
    fn_80166A50();
L_80208E20: ;
    r3 = r31;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_80208E40;
    fn_800F0308();
    goto L_80208E20;
L_80208E40: ;
    r3 = r28;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80208EBC;
    r3 = r31;
    r4 = 0x67;
    r5 = 0x4;
    fn_801DA9E8();
L_80208E64: ;
    r3 = r31;
    r4 = 0x67;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80208EBC;
    fn_800F0308();
    goto L_80208E64;
L_80208E84: ;
    if ((u32)r0 != (u32)0x5) goto L_80208EBC;
    r3 = r31;
    r4 = r29 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r31;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r31;
    r4 = 0x67;
    r5 = 0x4;
    fn_801DA8C4();
L_80208EBC: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80208ED0 | size: 0x25C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80208ED0(void) {
    extern void fn_800F0308();
    extern void fn_801C3C98();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB100();
    extern void fn_801DDD28();
    extern void fn_801F37B0();
    extern void fn_801F54A4();
    extern void fn_8026532C();
    extern void fn_80207F5C();
    u8 sp[0x30];
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

    r5 = 0xee;
    r6 = 0x0;
    /* stmw r29, 0x24(r1) */;
    r30 = r4;
    r31 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) goto L_80209118;
    if ((u32)r31 != (u32)0x0) goto L_80208F10;
    r3 = 0x0;
    goto L_80208F44;
L_80208F10: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80208F34;
    r3 = 0x0;
    goto L_80208F44;
L_80208F34: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80208F44: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0xd;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r0 = r30 & 0xFF;
    r30 = r3;
    if ((u32)r3 != (u32)0x0) goto L_80208F8C;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    goto L_80209118;
L_80208F8C: ;
    if ((u32)r0 != (u32)0x1) goto L_80208FA8;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
    goto L_80209118;
L_80208FA8: ;
    if ((u32)r0 != (u32)0x2) goto L_80208FD0;
L_80208FB0: ;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_80209118;
    fn_800F0308();
    goto L_80208FB0;
L_80208FD0: ;
    if ((u32)r0 != (u32)0x3) goto L_80208FEC;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    goto L_80209118;
L_80208FEC: ;
    if ((u32)r0 != (u32)0x4) goto L_80209118;
    if ((u32)r31 != (u32)0x0) goto L_80209004;
    r3 = 0x0;
    goto L_80209038;
L_80209004: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80209028;
    r3 = 0x0;
    goto L_80209038;
L_80209028: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_80209038: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    if ((u32)r3 == (u32)0x0) goto L_8020908C;
    r0 = 0x0;
    r4 = (u32)fn_80207F5C;
    r4 = (u32)fn_80207F5C;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
L_8020908C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_802090F4;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_802090CC;
    r4 = 0x0;
    fn_801DA4E8();
L_802090CC: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    fn_801C3C98();
    r3 = r30;
    fn_801DB100();
L_802090F4: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r31;
    r5 = 0x1;
    fn_8026532C();
L_80209118: ;
    /* lmw r29, 0x24(r1) */;
    return;
}
#pragma pop

/* 0x8020912C | size: 0x254 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020912C(void) {
    extern void fn_800F0308();
    extern void fn_801C3C98();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB100();
    extern void fn_801DDD28();
    extern void fn_801F37B0();
    extern void fn_801F54A4();
    extern void fn_8026532C();
    extern void fn_80207F5C();
    u8 sp[0x30];
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

    r5 = 0xee;
    r6 = 0x0;
    /* stmw r29, 0x24(r1) */;
    r30 = r4;
    r31 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) goto L_8020936C;
    if ((u32)r31 != (u32)0x0) goto L_8020916C;
    r3 = 0x0;
    goto L_802091A0;
L_8020916C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80209190;
    r3 = 0x0;
    goto L_802091A0;
L_80209190: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802091A0: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r0 = r30 & 0xFF;
    r30 = r3;
    if ((u32)r3 != (u32)0x0) goto L_802091E8;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    goto L_8020936C;
L_802091E8: ;
    if ((u32)r0 != (u32)0x1) goto L_80209204;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
    goto L_8020936C;
L_80209204: ;
    if ((u32)r0 != (u32)0x2) goto L_80209264;
L_8020920C: ;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_8020922C;
    fn_800F0308();
    goto L_8020920C;
L_8020922C: ;
    r3 = r29;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r31;
    r5 = 0x1;
    fn_8026532C();
    goto L_8020936C;
L_80209264: ;
    if ((u32)r0 != (u32)0x3) goto L_8020936C;
    if ((u32)r31 != (u32)0x0) goto L_8020927C;
    r3 = 0x0;
    goto L_802092B0;
L_8020927C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802092A0;
    r3 = 0x0;
    goto L_802092B0;
L_802092A0: ;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
L_802092B0: ;
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    if ((u32)r3 == (u32)0x0) goto L_80209304;
    r0 = 0x0;
    r4 = (u32)fn_80207F5C;
    r4 = (u32)fn_80207F5C;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    *(u32*)(sp + 0x10) = r0;
    fn_801F37B0();
L_80209304: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_8020936C;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80209344;
    r4 = 0x0;
    fn_801DA4E8();
L_80209344: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    fn_801C3C98();
    r3 = r30;
    fn_801DB100();
L_8020936C: ;
    /* lmw r29, 0x24(r1) */;
    return;
}
#pragma pop

/* 0x80209380 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209380(void) {
    extern void fn_800E3D08();
    extern void fn_800E4014();
    extern void fn_801DA4E8();
    extern void fn_801DAC3C();
    extern void fn_801F000C();
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

    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    /* stmw r29, 0x14(r1) */;
    r29 = r3;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_802093B4;
    r3 = 0x0;
    goto L_802093B8;
L_802093B4: ;
    fn_801DAC3C();
L_802093B8: ;
    r31 = r3;
    if ((u32)r3 == (u32)0x0) goto L_80209470;
    fn_800E3D08();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_80209470;
    r30 = 0x0;
    goto L_80209440;
L_802093D8: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_802093FC;
    r4 = 0x1;
    fn_801DA4E8();
L_802093FC: ;
    r3 = r31;
    r4 = 0x1;
    fn_800E4014();
    r3 = 0x3;
    fn_801F000C();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80209434;
    r4 = 0x0;
    fn_801DA4E8();
L_80209434: ;
    r3 = 0x2;
    fn_801F000C();
    r30 = r30 + 0x1;
L_80209440: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 < (u32)0x8) goto L_802093D8;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_80209470;
    r4 = 0x1;
    fn_801DA4E8();
L_80209470: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80209484 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209484(void) {
    extern void fn_801DA4E8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r5 = 0xee;
    r6 = 0x0;
    r31 = r4;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_802094B8;
    r4 = r31;
    fn_801DA4E8();
L_802094B8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802094CC | size: 0x90 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802094CC(void) {
    extern void fn_800F0308();
    extern void fn_8011BEB4();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
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
    r29 = r5;
    r30 = r6;
    r3 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_8011BEB4();
    r31 = r3;
    r3 = r28;
    r5 = r29;
    r4 = r31 & 0xFFFF;
    fn_801DA9E8();
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80209548;
L_80209518: ;
    r3 = r28;
    r5 = r29;
    r4 = r31 & 0xFFFF;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80209538;
    fn_800F0308();
    goto L_80209518;
L_80209538: ;
    r3 = r28;
    r5 = r29;
    r4 = r31 & 0xFFFF;
    fn_801DA8C4();
L_80209548: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020955C | size: 0xBC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020955C(void) {
    extern void fn_8011BEB4();
    extern void fn_80211164();
    extern void fn_80211168();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x20;
    r6 = 0x0;
    fn_8011BEB4();
    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_802095A4;
    r3 = (u32)fn_80211164;
    r0 = (u32)fn_80211164;
    r31 = r0;
L_802095A4: ;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    fn_8011BEB4();
    if ((u32)r3 != (u32)0x0) goto L_802095C8;
    r3 = (u32)fn_80211168;
    r3 = (u32)fn_80211168;
L_802095C8: ;
    r12 = r3;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r12 = r31;
    r7 = r3;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80209618 | size: 0xD0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209618(void) {
    extern u8 lbl_80279D08[];
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_80279D08;
    r6 = (u32)lbl_80279D08;
    /* stmw r27, 0x1c(r1) */;
    r31 = r3;
    r30 = r1 + 0x8;
    r27 = 0x0;
    r28 = 0x0;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r3 = *(u32*)((u8*)r6 + 0x8);
    r0 = *(u16*)((u8*)r6 + 0xC);
    *(u16*)(sp + 0x14) = r0;
    goto L_802096B0;
L_80209664: ;
    /* clrlslwi r0, r28, 16, 1 */;
    r29 = *(u16*)(r30 + r0);
    r3 = r29;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2a) goto L_80209688;
    r3 = 0x0;
    goto L_80209694;
L_80209688: ;
    r3 = r31;
    r4 = r29;
    fn_8011B67C();
L_80209694: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802096AC;
    r3 = r27 & 0xFFFF;
    r0 = r3 + 0x1;
    r27 = r0 & 0xFFFF;
L_802096AC: ;
    r28 = r28 + 0x1;
L_802096B0: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)0x7) goto L_80209664;
    r3 = r27 & 0xFFFF;
    r0 = 0x2;
    r0 = r3 - r0;
    /* lmw r27, 0x1c(r1) */;
    r0 = -0x1;
    /* subfze r0, r0 */;
    r3 = r0 & 0xFF;
    return;
}
#pragma pop

/* 0x802096E8 | size: 0xE0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802096E8(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x40;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2a) goto L_80209718;
    r3 = 0x0;
    goto L_80209724;
L_80209718: ;
    r3 = r31;
    r4 = 0x40;
    fn_8011B67C();
L_80209724: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80209738;
    r3 = 0x0;
    goto L_802097B4;
L_80209738: ;
    r3 = 0x43;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2a) goto L_80209754;
    r3 = 0x0;
    goto L_80209760;
L_80209754: ;
    r3 = r31;
    r4 = 0x43;
    fn_8011B67C();
L_80209760: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80209774;
    r3 = 0x0;
    goto L_802097B4;
L_80209774: ;
    r3 = 0x45;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2a) goto L_80209790;
    r3 = 0x0;
    goto L_8020979C;
L_80209790: ;
    r3 = r31;
    r4 = 0x45;
    fn_8011B67C();
L_8020979C: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802097B0;
    r3 = 0x0;
    goto L_802097B4;
L_802097B0: ;
    r3 = 0x1;
L_802097B4: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x802097C8 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802097C8(void) {
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
    if ((u32)r0 != (u32)0x2a) goto L_80209808;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8011B2C0();
L_80209808: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8020981C | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020981C(void) {
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
    if ((u32)r0 == (u32)0x2a) goto L_80209850;
    r3 = 0x0;
    goto L_8020985C;
L_80209850: ;
    r3 = r30;
    r4 = r31;
    fn_8011B444();
L_8020985C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80209870 | size: 0x9C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209870(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x41;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2a) goto L_802098A0;
    r3 = 0x0;
    goto L_802098AC;
L_802098A0: ;
    r3 = r31;
    r4 = 0x41;
    fn_8011B67C();
L_802098AC: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802098F4;
    r3 = 0x42;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2a) goto L_802098D4;
    r3 = 0x0;
    goto L_802098E0;
L_802098D4: ;
    r3 = r31;
    r4 = 0x42;
    fn_8011B67C();
L_802098E0: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802098F4;
    r3 = 0x1;
    goto L_802098F8;
L_802098F4: ;
    r3 = 0x0;
L_802098F8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020990C | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020990C(void) {
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
    if ((u32)r0 == (u32)0x2a) goto L_80209940;
    r3 = 0x0;
    goto L_8020994C;
L_80209940: ;
    r3 = r30;
    r4 = r31;
    fn_8011B67C();
L_8020994C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80209960 | size: 0x4C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209960(void) {
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
    if ((u32)r0 != (u32)0x2a) goto L_80209998;
    r3 = r30;
    r4 = r31;
    fn_8011B788();
L_80209998: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x802099AC | size: 0x270 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802099AC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    /* mr. r27, r3 */;
    r28 = r4;
    r29 = r5;
    r31 = r6;
    r30 = r7;
    if ((s32)r0 == (s32)0) goto L_80209C08;
    if ((s32)r0 == (s32)0) goto L_80209B34;
    r4 = 0x0;
    r5 = 0x26;
    r6 = 0x0;
    r7 = -0x1;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2a;
    r6 = 0x0;
    fn_8011BEB4();
    r4 = 0x9;
    fn_8011B950();
    r3 = 0x3f;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) goto L_80209A74;
    r3 = r27;
    r4 = 0x3f;
    r5 = 0x0;
    fn_8011B2C0();
L_80209A74: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2c;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2e;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    r7 = 0x9;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x31;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r4 = 0x0;
    r5 = 0x32;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
L_80209B34: ;
    r3 = r27;
    r7 = (s8)r28;
    r4 = 0x0;
    r5 = 0x26;
    r6 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r7 = r31 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BBD8();
    r31 = r29 & 0xFFFF;
    r3 = r27;
    r7 = r31;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r7 = r31;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_8011BBD8();
    r4 = r29;
    r3 = 0x0;
    r5 = 0x7;
    r6 = 0x0;
    fn_8011BEB4();
    r7 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    r4 = r29;
    r3 = 0x0;
    r5 = 0x3;
    r6 = 0x0;
    fn_8011BEB4();
    r7 = r3 & 0xFFFF;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_8011BBD8();
    r3 = r27;
    r7 = r30 & 0xFF;
    r4 = 0x0;
    r5 = 0x32;
    r6 = 0x0;
    fn_8011BBD8();
L_80209C08: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80209C1C | size: 0x98 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209C1C(void) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
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

    r7 = r4 & 0xFFFF;
    r5 = 0x28;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    fn_8011BBD8();
    r4 = r31;
    r3 = 0x0;
    r5 = 0x7;
    r6 = 0x0;
    fn_8011BEB4();
    r7 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BBD8();
    r4 = r31;
    r3 = 0x0;
    r5 = 0x3;
    r6 = 0x0;
    fn_8011BEB4();
    r7 = r3 & 0xFFFF;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_8011BBD8();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80209CB4 | size: 0xDC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209CB4(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) goto L_80209CD4;
    r3 = 0x0;
    goto L_80209D7C;
L_80209CD4: ;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 != (s32)0x0) goto L_80209CF4;
    r3 = 0x0;
    goto L_80209D7C;
L_80209CF4: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 != (s32)0x163) goto L_80209D18;
    r3 = 0x0;
    goto L_80209D7C;
L_80209D18: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 != (s32)0x0) goto L_80209D3C;
    r3 = 0x0;
    goto L_80209D7C;
L_80209D3C: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 != (s32)0x163) goto L_80209D60;
    r3 = 0x0;
    goto L_80209D7C;
L_80209D60: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BEB4();
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
L_80209D7C: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80209D90 | size: 0x188 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209D90(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_80209F04;
    r4 = 0x0;
    r5 = 0x26;
    r6 = 0x0;
    r7 = -0x1;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2a;
    r6 = 0x0;
    fn_8011BEB4();
    r4 = 0x9;
    fn_8011B950();
    r3 = 0x3f;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) goto L_80209E44;
    r3 = r31;
    r4 = 0x3f;
    r5 = 0x0;
    fn_8011B2C0();
L_80209E44: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2c;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2e;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    r7 = 0x9;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x31;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x32;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
L_80209F04: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80209F18 | size: 0x94 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209F18(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
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
    r5 = 0x2a;
    r6 = 0x0;
    r31 = r3;
    fn_8011BEB4();
    r4 = 0x9;
    fn_8011B950();
    r3 = 0x3f;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) goto L_80209F68;
    r3 = r31;
    r4 = 0x3f;
    r5 = 0x0;
    fn_8011B2C0();
L_80209F68: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x2c;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80209FAC | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80209FAC(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0x2a;
    r6 = 0x0;
    r31 = r3;
    fn_8011BEB4();
    r4 = 0x9;
    fn_8011B950();
    r3 = 0x3f;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) goto L_80209FFC;
    r3 = r31;
    r4 = 0x3f;
    r5 = 0x0;
    fn_8011B2C0();
L_80209FFC: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020A010 | size: 0x18 */
u32 fn_8020A010(u8* ptr) {
    if (ptr == NULL) { return 1; }
    return ptr[0x1];
}

/* 0x8020A028 | size: 0x18 */
u32 fn_8020A028(u8* ptr) {
    if (ptr == NULL) { return 1; }
    return ptr[0x0];
}

/* 0x8020A040 | size: 0x28 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A040(void) {
    extern u8 lbl_80375DD0[];
    extern u8 lbl_80478D70[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D70;
    r5 = r3 & 0xFFFF;
    r4 = (u32)lbl_80375DD0;
    /* clrlslwi r3, r3, 16, 1 */;
    r0 = (u32)lbl_80375DD0;
    r3 = r0 + r3;
    if ((u32)r5 < (u32)r0) return;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x8020A080 | size: 0x24 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A080(void) {
    extern u8 lbl_80478D58[];
    extern u8 lbl_80478D60[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u32*)lbl_80478D60;
    r4 = r3 & 0xFFFF;
    r3 = (u32)lbl_80478D58;
    r0 = (u32)lbl_80478D58;
    r3 = r0 + r4;
    if ((u32)r4 < (u32)r0) return;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x8020A224 | size: 0x34 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A224(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    /* mr. r5, r3 */;
    if ((s32)r0 != (s32)0) goto L_8020A234;
    r3 = 0x0;
    return;
L_8020A234: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x9) goto L_8020A248;
    r3 = 0x0;
    return;
L_8020A248: ;
    /* clrlslwi r3, r4, 16, 4 */;
    r3 = r3 + 0x8;
    r3 = r5 + r3;
    return;
}
#pragma pop

/* 0x8020A2B8 | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A2B8(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((u32)r3 == (u32)0x0) return;
    if ((u32)r4 == (u32)0x0) return;
    r0 = 0x15;
    /* subi r5, r3, 0x4 */;
    /* subi r4, r4, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_8020A2D8: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8020A2D8;
    r0 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r5 + 0x4) = r0;
    return;
}
#pragma pop

/* 0x8020A398 | size: 0xE0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A398(void) {
    extern void fn_80142B24();
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
    /* mr. r28, r3 */;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    if ((s32)r0 == (s32)0) goto L_8020A464;
    if ((s32)r0 == (s32)0) goto L_8020A41C;
    r4 = 0x0;
    r5 = 0x1e;
    r6 = 0x0;
    r7 = 0x0;
    fn_80142B24();
    r3 = r28;
    r4 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    r7 = 0x0;
    fn_80142B24();
    r3 = r28;
    r4 = 0x0;
    r5 = 0x20;
    r6 = 0x0;
    r7 = -0x1;
    fn_80142B24();
    r3 = r28;
    r4 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    r7 = 0x0;
    fn_80142B24();
L_8020A41C: ;
    r3 = r28;
    r7 = r29 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x1e;
    r6 = 0x0;
    fn_80142B24();
    r3 = r28;
    r7 = r30 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_80142B24();
    r3 = r28;
    r7 = r31;
    r4 = 0x0;
    r5 = 0x20;
    r6 = 0x0;
    fn_80142B24();
L_8020A464: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020A478 | size: 0x88 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A478(void) {
    extern void fn_80142B24();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_8020A4EC;
    r4 = 0x0;
    r5 = 0x1e;
    r6 = 0x0;
    r7 = 0x0;
    fn_80142B24();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    r7 = 0x0;
    fn_80142B24();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x20;
    r6 = 0x0;
    r7 = -0x1;
    fn_80142B24();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    r7 = 0x0;
    fn_80142B24();
L_8020A4EC: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020A500 | size: 0x40 */
u32 fn_8020A500(u16 idx) {
    u8* entry;
    idx = (u16)idx;
    if (idx >= lbl_80478D38) {
        entry = NULL;
    } else {
        entry = lbl_80478D30 + idx * 6;
    }
    if (entry == NULL) { return 0; }
    return *(u16*)(entry + 4);
}

/* 0x8020A540 | size: 0x40 */
u32 fn_8020A540(u16 idx) {
    u8* entry;
    idx = (u16)idx;
    if (idx >= lbl_80478D38) {
        entry = NULL;
    } else {
        entry = lbl_80478D30 + idx * 6;
    }
    if (entry == NULL) { return 0; }
    return *(u16*)(entry + 2);
}

/* 0x8020A580 | size: 0x40 */
u32 fn_8020A580(u16 idx) {
    u8* entry;
    idx = (u16)idx;
    if (idx >= lbl_80478D38) {
        entry = NULL;
    } else {
        entry = lbl_80478D30 + idx * 6;
    }
    if (entry == NULL) { return 0; }
    return entry[0];
}

/* 0x8020A5C0 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A5C0(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A5D8;
    r5 = 0x0;
    goto L_8020A5E8;
L_8020A5D8: ;
    r5 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r5 = r0 + r5;
L_8020A5E8: ;
    if ((u32)r5 != (u32)0x0) goto L_8020A5F8;
    r3 = 0x0;
    goto L_8020A618;
L_8020A5F8: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020A60C;
    r3 = 0x0;
    goto L_8020A618;
L_8020A60C: ;
    r3 = r0 * 0xa;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
L_8020A618: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A628;
    r3 = 0x0;
    return;
L_8020A628: ;
    r3 = *(s16*)((u8*)r3 + 0x4);
    return;
}
#pragma pop

/* 0x8020A630 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A630(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A648;
    r5 = 0x0;
    goto L_8020A658;
L_8020A648: ;
    r5 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r5 = r0 + r5;
L_8020A658: ;
    if ((u32)r5 != (u32)0x0) goto L_8020A668;
    r3 = 0x0;
    goto L_8020A688;
L_8020A668: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020A67C;
    r3 = 0x0;
    goto L_8020A688;
L_8020A67C: ;
    r3 = r0 * 0xa;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
L_8020A688: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A698;
    r3 = 0x0;
    return;
L_8020A698: ;
    r3 = *(s16*)((u8*)r3 + 0x2);
    return;
}
#pragma pop

/* 0x8020A6A0 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A6A0(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A6B8;
    r5 = 0x0;
    goto L_8020A6C8;
L_8020A6B8: ;
    r5 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r5 = r0 + r5;
L_8020A6C8: ;
    if ((u32)r5 != (u32)0x0) goto L_8020A6D8;
    r3 = 0x0;
    goto L_8020A6F8;
L_8020A6D8: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020A6EC;
    r3 = 0x0;
    goto L_8020A6F8;
L_8020A6EC: ;
    r3 = r0 * 0xa;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
L_8020A6F8: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A708;
    r3 = 0x0;
    return;
L_8020A708: ;
    r3 = *(u8*)((u8*)r3 + 0x1);
    return;
}
#pragma pop

/* 0x8020A710 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A710(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A728;
    r5 = 0x0;
    goto L_8020A738;
L_8020A728: ;
    r5 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r5 = r0 + r5;
L_8020A738: ;
    if ((u32)r5 != (u32)0x0) goto L_8020A748;
    r3 = 0x0;
    goto L_8020A768;
L_8020A748: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020A75C;
    r3 = 0x0;
    goto L_8020A768;
L_8020A75C: ;
    r3 = r0 * 0xa;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
L_8020A768: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A778;
    r3 = 0x0;
    return;
L_8020A778: ;
    r3 = *(u16*)((u8*)r3 + 0x8);
    return;
}
#pragma pop

/* 0x8020A780 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A780(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A798;
    r5 = 0x0;
    goto L_8020A7A8;
L_8020A798: ;
    r5 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r5 = r0 + r5;
L_8020A7A8: ;
    if ((u32)r5 != (u32)0x0) goto L_8020A7B8;
    r3 = 0x0;
    goto L_8020A7D8;
L_8020A7B8: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020A7CC;
    r3 = 0x0;
    goto L_8020A7D8;
L_8020A7CC: ;
    r3 = r0 * 0xa;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
L_8020A7D8: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A7E8;
    r3 = 0x0;
    return;
L_8020A7E8: ;
    r3 = *(u16*)((u8*)r3 + 0x6);
    return;
}
#pragma pop

/* 0x8020A7F0 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A7F0(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A808;
    r5 = 0x0;
    goto L_8020A818;
L_8020A808: ;
    r5 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r5 = r0 + r5;
L_8020A818: ;
    if ((u32)r5 != (u32)0x0) goto L_8020A828;
    r3 = 0x0;
    goto L_8020A848;
L_8020A828: ;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020A83C;
    r3 = 0x0;
    goto L_8020A848;
L_8020A83C: ;
    r3 = r0 * 0xa;
    r3 = r3 + 0x4;
    r3 = r5 + r3;
L_8020A848: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A858;
    r3 = 0x0;
    return;
L_8020A858: ;
    r3 = *(u8*)((u8*)r3 + 0x0);
    return;
}
#pragma pop

/* 0x8020A860 | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A860(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A878;
    r3 = 0x0;
    goto L_8020A888;
L_8020A878: ;
    r4 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r3 = r0 + r4;
L_8020A888: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A898;
    r3 = 0x0;
    return;
L_8020A898: ;
    r3 = *(u16*)((u8*)r3 + 0x2);
    return;
}
#pragma pop

/* 0x8020A8A0 | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A8A0(void) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u32*)lbl_80478D28;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020A8B8;
    r3 = 0x0;
    goto L_8020A8C8;
L_8020A8B8: ;
    r4 = r3 * 0x18;
    r3 = (u32)lbl_80375A08;
    r0 = (u32)lbl_80375A08;
    r3 = r0 + r4;
L_8020A8C8: ;
    if ((u32)r3 != (u32)0x0) goto L_8020A8D8;
    r3 = 0x0;
    return;
L_8020A8D8: ;
    r3 = *(u8*)((u8*)r3 + 0x0);
    return;
}
#pragma pop

/* 0x8020A8E0 | size: 0x550 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020A8E0(void) {
    extern void fn_800E0C04();
    extern void fn_80135E44();
    extern void fn_801F021C();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_8020A500();
    extern void fn_8020A540();
    extern void fn_8020A580();
    extern void fn_8020A5C0();
    extern void fn_8020A630();
    extern void fn_8020A6A0();
    extern void fn_8020A710();
    extern void fn_8020A780();
    extern void fn_8020A7F0();
    extern void fn_8020A860();
    extern void fn_8020A8A0();
    extern u8 jumptable_80375938[];
    extern u8 jumptable_80375954[];
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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

    /* stmw r19, 0x1c(r1) */;
    r20 = r3;
    r31 = r4;
    r21 = r1 + 0x10;
    r24 = 0x0;
    goto L_8020AA64;
L_8020A904: ;
    r29 = r24 & 0xFF;
    r3 = r20;
    r4 = r29;
    r23 = 0x0;
    fn_8020A7F0();
    r22 = r3;
    r3 = r20;
    r4 = r29;
    fn_8020A780();
    r28 = r3;
    r3 = r20;
    r4 = r29;
    fn_8020A710();
    r27 = r3;
    r3 = r20;
    r4 = r29;
    fn_8020A630();
    r26 = r3;
    r3 = r20;
    r4 = r29;
    fn_8020A5C0();
    r25 = r3;
    r3 = r20;
    r4 = r29;
    fn_8020A6A0();
    r0 = r22 & 0xFF;
    r22 = r3;
    if ((s32)r0 == (s32)0x2) goto L_8020A9A0;
    if ((s32)r0 >= (s32)0x2) goto L_8020A98C;
    if ((s32)r0 == (s32)0x0) goto L_8020AA34;
    if ((s32)r0 >= (s32)0x0) goto L_8020A998;
    goto L_8020AA34;
L_8020A98C: ;
    if ((s32)r0 >= (s32)0x4) goto L_8020AA34;
    goto L_8020A9B8;
L_8020A998: ;
    r23 = r28 & 0xFFFF;
    goto L_8020AA34;
L_8020A9A0: ;
    r19 = r28 & 0xFFFF;
    r0 = r27 & 0xFFFF;
    r3 = r0 - r19;
    fn_800E0C04();
    r23 = r19 + r3;
    goto L_8020AA34;
L_8020A9B8: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r5 = r3 & 0xFFFF;
    r3 = r28;
    r4 = r31;
    fn_801F02AC();
    /* mr. r23, r3 */;
    if ((s32)r0 != (s32)0x4) goto L_8020A9EC;
    r23 = 0x0;
    goto L_8020AA58;
L_8020A9EC: ;
    r3 = r28;
    fn_801F0234();
    fn_801F021C();
    r0 = r22 & 0xFF;
    if ((s32)r0 != (s32)0x4) goto L_8020AA1C;
    r4 = r23;
    r6 = r27;
    r5 = r26 & 0xFFFF;
    r7 = r25 & 0xFFFF;
    fn_80135E44();
    r23 = r3;
    goto L_8020AA34;
L_8020AA1C: ;
    r4 = r23;
    r6 = r27;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    r23 = r3;
L_8020AA34: ;
    r0 = r22 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020AA58;
    r3 = (s16)r26;
    r0 = (s16)r25;
    r23 = r23 * r3;
    if ((u32)r0 == (u32)0x1) goto L_8020AA58;
    r0 = (s16)r25;
    r23 = (s32)r23 / (s32)r0;
L_8020AA58: ;
    /* clrlslwi r0, r24, 24, 2 */;
    r24 = r24 + 0x1;
    *(u32*)(r21 + r0) = r23;
L_8020AA64: ;
    r0 = r24 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_8020A904;
    r3 = r20;
    r24 = 0x0;
    fn_8020A8A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x6) goto L_8020AB34;
    r3 = (u32)jumptable_80375954;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80375954;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r24 = 0x1;
    goto L_8020AB34;
    r3 = *(u32*)(sp + 0x10);
    r0 = *(u32*)(sp + 0x14);
    if ((s32)r3 != (s32)r0) goto L_8020AB34;
    r24 = 0x1;
    goto L_8020AB34;
    r3 = *(u32*)(sp + 0x10);
    r0 = *(u32*)(sp + 0x14);
    if ((s32)r3 == (s32)r0) goto L_8020AB34;
    r24 = 0x1;
    goto L_8020AB34;
    r3 = *(u32*)(sp + 0x10);
    r0 = *(u32*)(sp + 0x14);
    if ((s32)r3 < (s32)r0) goto L_8020AB34;
    r24 = 0x1;
    goto L_8020AB34;
    r3 = *(u32*)(sp + 0x10);
    r0 = *(u32*)(sp + 0x14);
    if ((s32)r3 > (s32)r0) goto L_8020AB34;
    r24 = 0x1;
    goto L_8020AB34;
    r3 = *(u32*)(sp + 0x10);
    r0 = *(u32*)(sp + 0x14);
    if ((s32)r3 >= (s32)r0) goto L_8020AB34;
    r24 = 0x1;
    goto L_8020AB34;
    r3 = *(u32*)(sp + 0x10);
    r0 = *(u32*)(sp + 0x14);
    if ((s32)r3 <= (s32)r0) goto L_8020AB34;
    r24 = 0x1;
L_8020AB34: ;
    r30 = r24;
    r3 = r20;
    fn_8020A860();
    r0 = r3 & 0xFFFF;
    r28 = r3;
    if ((s32)r3 != (s32)r0) goto L_8020AB54;
    r3 = r24;
    goto L_8020AE1C;
L_8020AB54: ;
    r27 = r1 + 0x8;
L_8020AB58: ;
    r3 = r28;
    fn_8020A540();
    r23 = 0x0;
    r29 = r3;
    goto L_8020ACCC;
L_8020AB6C: ;
    r19 = r23 & 0xFF;
    r3 = r29;
    r4 = r19;
    r20 = 0x0;
    fn_8020A7F0();
    r26 = r3;
    r3 = r29;
    r4 = r19;
    fn_8020A780();
    r21 = r3;
    r3 = r29;
    r4 = r19;
    fn_8020A710();
    r22 = r3;
    r3 = r29;
    r4 = r19;
    fn_8020A630();
    r24 = r3;
    r3 = r29;
    r4 = r19;
    fn_8020A5C0();
    r25 = r3;
    r3 = r29;
    r4 = r19;
    fn_8020A6A0();
    r0 = r26 & 0xFF;
    r26 = r3;
    if ((s32)r0 == (s32)0x2) goto L_8020AC08;
    if ((s32)r0 >= (s32)0x2) goto L_8020ABF4;
    if ((s32)r0 == (s32)0x0) goto L_8020AC9C;
    if ((s32)r0 >= (s32)0x0) goto L_8020AC00;
    goto L_8020AC9C;
L_8020ABF4: ;
    if ((s32)r0 >= (s32)0x4) goto L_8020AC9C;
    goto L_8020AC20;
L_8020AC00: ;
    r20 = r21 & 0xFFFF;
    goto L_8020AC9C;
L_8020AC08: ;
    r19 = r21 & 0xFFFF;
    r0 = r22 & 0xFFFF;
    r3 = r0 - r19;
    fn_800E0C04();
    r20 = r19 + r3;
    goto L_8020AC9C;
L_8020AC20: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r5 = r3 & 0xFFFF;
    r3 = r21;
    r4 = r31;
    fn_801F02AC();
    /* mr. r20, r3 */;
    if ((s32)r0 != (s32)0x4) goto L_8020AC54;
    r20 = 0x0;
    goto L_8020ACC0;
L_8020AC54: ;
    r3 = r21;
    fn_801F0234();
    fn_801F021C();
    r0 = r26 & 0xFF;
    if ((s32)r0 != (s32)0x4) goto L_8020AC84;
    r4 = r20;
    r6 = r22;
    r5 = r24 & 0xFFFF;
    r7 = r25 & 0xFFFF;
    fn_80135E44();
    r20 = r3;
    goto L_8020AC9C;
L_8020AC84: ;
    r4 = r20;
    r6 = r22;
    r5 = 0x0;
    r7 = 0x0;
    fn_80135E44();
    r20 = r3;
L_8020AC9C: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020ACC0;
    r3 = (s16)r24;
    r0 = (s16)r25;
    r20 = r20 * r3;
    if ((u32)r0 == (u32)0x1) goto L_8020ACC0;
    r0 = (s16)r25;
    r20 = (s32)r20 / (s32)r0;
L_8020ACC0: ;
    /* clrlslwi r0, r23, 24, 2 */;
    r23 = r23 + 0x1;
    *(u32*)(r27 + r0) = r20;
L_8020ACCC: ;
    r0 = r23 & 0xFF;
    if ((u32)r0 < (u32)0x2) goto L_8020AB6C;
    r3 = r29;
    r23 = 0x0;
    fn_8020A8A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 > (u32)0x6) goto L_8020AD9C;
    r3 = (u32)jumptable_80375938;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80375938;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r23 = 0x1;
    goto L_8020AD9C;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((s32)r3 != (s32)r0) goto L_8020AD9C;
    r23 = 0x1;
    goto L_8020AD9C;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((s32)r3 == (s32)r0) goto L_8020AD9C;
    r23 = 0x1;
    goto L_8020AD9C;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((s32)r3 < (s32)r0) goto L_8020AD9C;
    r23 = 0x1;
    goto L_8020AD9C;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((s32)r3 > (s32)r0) goto L_8020AD9C;
    r23 = 0x1;
    goto L_8020AD9C;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((s32)r3 >= (s32)r0) goto L_8020AD9C;
    r23 = 0x1;
    goto L_8020AD9C;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((s32)r3 <= (s32)r0) goto L_8020AD9C;
    r23 = 0x1;
L_8020AD9C: ;
    r3 = r29;
    fn_8020A580();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x2) goto L_8020ADE0;
    if ((s32)r0 >= (s32)0x2) goto L_8020ADFC;
    if ((s32)r0 >= (s32)0x1) goto L_8020ADC0;
    goto L_8020ADFC;
L_8020ADC0: ;
    r0 = r30 & 0xFF;
    if ((s32)r0 != (s32)0x1) goto L_8020ADD0;
    r0 = r23 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_8020ADD8;
L_8020ADD0: ;
    r30 = 0x1;
    goto L_8020ADFC;
L_8020ADD8: ;
    r30 = 0x0;
    goto L_8020ADFC;
L_8020ADE0: ;
    r0 = r30 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_8020ADF8;
    r0 = r23 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_8020ADF8;
    r30 = 0x1;
    goto L_8020ADFC;
L_8020ADF8: ;
    r30 = 0x0;
L_8020ADFC: ;
    r3 = r28;
    fn_8020A500();
    r0 = r3 & 0xFFFF;
    r28 = r3;
    if ((s32)r0 != (s32)0x1) goto L_8020AB58;
    r3 = r30;
    goto L_8020AE1C;
    goto L_8020AB54;
L_8020AE1C: ;
    /* lmw r19, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x8020AED0 | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020AED0(void) {
    extern void fn_801F4C14();
    extern void fn_8020D8F0();
    extern void fn_8020D908();
    extern void fn_80211B94();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r31 = r3;
    fn_8020D908();
    r7 = r3;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r31;
    fn_8020D8F0();
    r4 = r3;
    r3 = r31;
    r5 = 0x0;
    fn_80211B94();
    r3 = 0x1;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020AF30 | size: 0xC4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020AF30(void) {
    extern void fn_80136368();
    extern void fn_801F37B0();
    extern void fn_801F453C();
    extern void fn_801F4C14();
    extern void fn_8020D8F0();
    extern void fn_8020DA14();
    extern void fn_80211B94();
    extern void fn_8020AFF4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x1;
    /* stmw r30, 0x18(r1) */;
    r30 = r3;
    r3 = 0x0;
    fn_8020DA14();
    r3 = 0x0;
    r4 = 0x0;
    fn_801F453C();
    r0 = 0x0;
    r4 = (u32)fn_8020AFF4;
    *(u32*)(sp + 0x8) = r0;
    r31 = r3 & 0xFF;
    r4 = (u32)fn_8020AFF4;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    if ((u32)r31 == (u32)0x0) goto L_8020AFDC;
    r7 = *(u32*)(sp + 0x8);
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r31;
    fn_80136368();
    r0 = r3;
    r3 = 0x0;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x50;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r30;
    fn_8020D8F0();
    r4 = r3;
    r3 = r30;
    r5 = 0x0;
    fn_80211B94();
L_8020AFDC: ;
    /* lmw r30, 0x18(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020AFF4 | size: 0x5C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020AFF4(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r31 = r5;
    r30 = r3;
    r5 = 0xee;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) goto L_8020B038;
    if ((u32)r31 == (u32)0x0) goto L_8020B030;
    *(u32*)((u8*)r31 + 0x0) = r30;
L_8020B030: ;
    r3 = 0x0;
    goto L_8020B03C;
L_8020B038: ;
    r3 = 0x1;
L_8020B03C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020B058 | size: 0x2D8 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020B058(void) {
    extern void fn_8006B0F8();
    extern void fn_8006B57C();
    extern void fn_8011FC74();
    extern void fn_801233F4();
    extern void fn_8012805C();
    extern void fn_80128A64();
    extern void fn_80129280();
    extern void fn_80129840();
    extern void fn_8012A5B0();
    extern void fn_8012AC64();
    extern void fn_801EF634();
    extern void fn_801EFFC4();
    extern void fn_801F1DBC();
    extern void fn_801F2A7C();
    extern void fn_801F47B4();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F86C0();
    extern void fn_801F9034();
    extern void fn_801F9930();
    extern void fn_801FB1C0();
    extern void fn_80206608();
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
    u32 r10 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    /* stmw r25, 0x14(r1) */;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B098;
    r3 = 0x1;
    goto L_8020B31C;
L_8020B098: ;
    r3 = 0x0;
    fn_801F2A7C();
    /* mr. r31, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020B268;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    /* mr. r30, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020B268;
    r3 = r31;
    r4 = 0x0;
    fn_801F86C0();
    fn_801EF634();
    r4 = r3;
    r3 = 0x0;
    fn_801F1DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B22C;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x24;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B1F0;
    r3 = r31;
    fn_801F9034();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B1F0;
    r26 = 0x0;
    goto L_8020B1E4;
L_8020B124: ;
    r3 = r30;
    r5 = r26 & 0xFFFF;
    r4 = 0x3;
    fn_8012A5B0();
    r27 = r3;
    fn_801233F4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020B1E0;
    r3 = r27;
    fn_8011FC74();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020B1E0;
    r3 = r31;
    r4 = r27;
    fn_801F9930();
    /* mr. r25, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020B1E0;
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020B1E0;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xd0;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020B1E0;
    r3 = r27;
    r6 = r1 + 0x8;
    r7 = r1 + 0xc;
    r4 = 0x0;
    r5 = 0x0;
    fn_80128A64();
    r0 = r3 & 0xFFFF;
    r4 = r3;
    if ((u32)r0 == (u32)0x1) goto L_8020B1E0;
    r5 = *(u16*)(sp + 0x8);
    r3 = r27;
    r7 = r30;
    r6 = r1 + 0xc;
    r8 = 0x1;
    r9 = 0x1;
    r10 = 0x0;
    fn_8012805C();
    r3 = 0xa;
    fn_801EFFC4();
L_8020B1E0: ;
    r26 = r26 + 0x1;
L_8020B1E4: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8020B124;
L_8020B1F0: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B218;
    r3 = r30;
    fn_80129840();
L_8020B218: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_801F54A4();
L_8020B22C: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x1c;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B268;
    r3 = 0x0;
    r4 = 0x2;
    fn_80129280();
    if ((u32)r3 == (u32)0x0) goto L_8020B268;
    r4 = r30;
    fn_8012AC64();
L_8020B268: ;
    fn_8006B57C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B318;
    r26 = 0x0;
    goto L_8020B30C;
L_8020B280: ;
    r4 = r26;
    r3 = 0x0;
    fn_801F47B4();
    /* mr. r25, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020B308;
    r0 = r26 & 0xFFFF;
    r27 = 0x0;
    r30 = r0 * r29;
    goto L_8020B2FC;
L_8020B2A4: ;
    r3 = r25;
    r4 = r27;
    fn_801F7258();
    /* mr. r31, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020B2F8;
    r4 = 0x0;
    fn_801F86C0();
    r0 = r27 + r30;
    r3 = r0 & 0xFF;
    fn_8006B0F8();
    /* mr. r28, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020B2F8;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    /* mr. r4, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020B2F8;
    r3 = r28;
    fn_8012AC64();
L_8020B2F8: ;
    r27 = r27 + 0x1;
L_8020B2FC: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r29) goto L_8020B2A4;
L_8020B308: ;
    r26 = r26 + 0x1;
L_8020B30C: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020B280;
L_8020B318: ;
    r3 = 0x1;
L_8020B31C: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8020B330 | size: 0x3A4 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020B330(void) {
    extern u8 lbl_80378801[];
    extern u8 lbl_8037880F[];
    extern void fn_800896B8();
    extern void fn_800896C0();
    extern void fn_800F0308();
    extern void fn_80132A38();
    extern void fn_80165668();
    extern void fn_801C2D54();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
    extern void fn_801EF2D4();
    extern void fn_801EF634();
    extern void fn_801EF8F4();
    extern void fn_801F000C();
    extern void fn_801F025C();
    extern void fn_801F1DBC();
    extern void fn_801F54A4();
    extern void fn_801F8000();
    extern void fn_801F8100();
    extern void fn_801FB1C0();
    extern void fn_8020D814();
    extern void fn_8020D920();
    extern void fn_80211B94();
    extern void fn_8026246C();
    extern void fn_80262490();
    extern void fn_802624CC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f5 = 0.0f;

    /* stmw r23, 0xc(r1) */;
    r31 = r3;
    fn_8020D920();
    fn_8020D814();
    r30 = r3 & 0xFFFF;
    r3 = 0xb;
    r4 = 0x0;
    fn_801F025C();
    r0 = r3;
    r3 = 0x9;
    r25 = r0;
    r4 = r25;
    fn_801F025C();
    r4 = 0x0;
    r24 = r3;
    r5 = 0x43;
    r6 = 0x0;
    fn_801FB1C0();
    r26 = r3 & 0xFFFF;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x4c;
    r6 = 0x0;
    fn_801FB1C0();
    r29 = r3;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x1;
    fn_801FB1C0();
    r28 = r3;
    fn_800896B8();
    if ((u32)r26 != (u32)r3) goto L_8020B3EC;
    fn_800896C0();
    if ((u32)r3 != (u32)0x0) goto L_8020B3D8;
    r23 = 0x0;
    goto L_8020B458;
L_8020B3D8: ;
    r4 = r3;
    r3 = 0x24;
    fn_80132A38();
    r23 = 0x7531;
    goto L_8020B458;
L_8020B3EC: ;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x4a;
    r6 = 0x0;
    fn_801FB1C0();
    if ((s32)r3 != (s32)0x0) goto L_8020B424;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x2;
    fn_801FB1C0();
    r23 = r3;
    goto L_8020B458;
L_8020B424: ;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x3;
    fn_801FB1C0();
    /* mr. r23, r3 */;
    if ((s32)r3 != (s32)0x0) goto L_8020B458;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x2;
    fn_801FB1C0();
    r23 = r3;
L_8020B458: ;
    r3 = r24;
    fn_801F8000();
    r4 = r3;
    r3 = 0x22;
    fn_80132A38();
    r3 = r24;
    fn_801F8100();
    r4 = r3;
    r3 = 0x23;
    fn_80132A38();
    r3 = r25;
    fn_801F8100();
    r4 = r3;
    r3 = 0x13;
    fn_80132A38();
    r3 = r24;
    fn_801F8100();
    r4 = r3;
    r3 = 0x25;
    fn_80132A38();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B654;
    if ((u32)r30 != (u32)0x2) goto L_8020B57C;
    if ((u32)r23 == (u32)0x0) goto L_8020B4F4;
    r3 = r29;
    r4 = 0x5a;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    fn_801C2D54();
    r27 = r3;
L_8020B4F4: ;
    r3 = 0x3f5;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    r3 = 0x5d;
    r4 = 0x0;
    fn_80132A38();
    r3 = 0x766c;
    fn_802624CC();
    fn_8026246C();
    if ((u32)r23 == (u32)0x0) goto L_8020B654;
    r3 = r29;
    r4 = 0x5a;
    r5 = 0x4;
    fn_801DA9E8();
    r3 = r23;
    fn_80262490();
L_8020B53C: ;
    r3 = r29;
    r4 = 0x5a;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r23 == (u32)0x0) goto L_8020B55C;
    fn_800F0308();
    goto L_8020B53C;
L_8020B55C: ;
    r3 = r27;
    fn_801EF8F4();
    fn_8026246C();
    r3 = r29;
    r4 = 0x5a;
    r5 = 0x4;
    fn_801DA8C4();
    goto L_8020B654;
L_8020B57C: ;
    if ((u32)r30 != (u32)0x3) goto L_8020B620;
    if ((u32)r28 == (u32)0x0) goto L_8020B5A8;
    r3 = r29;
    r4 = 0x59;
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    fn_801C2D54();
    r27 = r3;
L_8020B5A8: ;
    r3 = 0x7547;
    fn_802624CC();
    fn_8026246C();
    if ((u32)r28 == (u32)0x0) goto L_8020B610;
    r3 = r29;
    r4 = 0x59;
    r5 = 0x4;
    fn_801DA9E8();
    r3 = r28;
    fn_80262490();
L_8020B5D4: ;
    r3 = r29;
    r4 = 0x59;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r28 == (u32)0x0) goto L_8020B5F4;
    fn_800F0308();
    goto L_8020B5D4;
L_8020B5F4: ;
    r3 = r27;
    fn_801EF8F4();
    fn_8026246C();
    r3 = r29;
    r4 = 0x59;
    r5 = 0x4;
    fn_801DA8C4();
L_8020B610: ;
    r3 = 0x7548;
    fn_802624CC();
    fn_8026246C();
    goto L_8020B654;
L_8020B620: ;
    /* subi r0, r30, 0x4 */;
    r0 = r0 & 0xFFFF;
    if ((u32)r0 <= (u32)0x1) goto L_8020B654;
    if ((u32)r30 == (u32)0x7) goto L_8020B640;
    if ((u32)r30 != (u32)0x6) goto L_8020B654;
L_8020B640: ;
    r3 = 0x7640;
    fn_802624CC();
    r3 = 0x40;
    fn_801F000C();
    fn_8026246C();
L_8020B654: ;
    fn_801EF634();
    r4 = r3;
    r3 = 0x0;
    fn_801F1DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B6B8;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x25;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B6B8;
    r4 = (u32)lbl_80378801;
    r3 = r31;
    r4 = (u32)lbl_80378801;
    r5 = 0x0;
    fn_80211B94();
    r4 = (u32)lbl_8037880F;
    r3 = r31;
    r4 = (u32)lbl_8037880F;
    r5 = 0x0;
    fn_80211B94();
L_8020B6B8: ;
    fn_801EF2D4();
    /* lmw r23, 0xc(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020B6D4 | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020B6D4(void) {
    extern void fn_8016597C();
    extern void fn_801F000C();
    extern void fn_801F54A4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x12;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r3 == (u32)0x0) goto L_8020B718;
    r3 = 0x1;
    r4 = 0x3e8;
    r5 = 0x3e8;
    r6 = 0xff;
    fn_8016597C();
    r3 = 0x3c;
    fn_801F000C();
L_8020B718: ;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020B72C | size: 0x1E4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020B72C(void) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_801F00D0();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_80205224();
    extern void fn_80205B8C();
    extern void fn_80209C1C();
    extern void fn_80209CB4();
    extern void fn_8020D908();
    extern void fn_802128D0();
    extern void fn_8022B2CC();
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
    /* stmw r25, 0x14(r1) */;
    r31 = r3;
    r3 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    r3 = r31;
    fn_8020D908();
    r4 = 0x0;
    r30 = r3;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r26 = r3;
    fn_80209CB4();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020B78C;
    r3 = 0x0;
    goto L_8020B8FC;
L_8020B78C: ;
    r3 = r26;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    r4 = r27;
    fn_801F00D0();
    r29 = r3;
    r7 = r30;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r7 = r29;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x42;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r30;
    fn_80205B8C();
    r0 = r3;
    r3 = r30;
    r25 = r0;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r28 = r3;
    r3 = r30;
    fn_80205224();
    r0 = r3;
    r3 = r28;
    r26 = r0;
    r4 = 0x0;
    r5 = 0x26;
    r6 = 0x0;
    fn_8011BEB4();
    r29 = (s8)r3;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x32;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020B8EC;
    r29 = r29 & 0xFF;
    r3 = r25;
    r4 = 0x0;
    r5 = 0x7f;
    r6 = r29;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 == (u32)r3) goto L_8020B8EC;
    r3 = r25;
    r6 = r29;
    r4 = 0x0;
    r5 = 0x7f;
    ((void(*)(void))fn_8012640C)();
    r26 = r3 & 0xFFFF;
    r3 = r28;
    r7 = r26;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BBD8();
    r3 = r28;
    r4 = r26;
    fn_80209C1C();
    r3 = r30;
    r4 = r26;
    r5 = r27;
    r6 = 0x0;
    r7 = 0x1;
    r8 = 0x0;
    r9 = -0x1;
    fn_8022B2CC();
    r0 = r3;
    r3 = 0x0;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x43;
    r6 = 0x0;
    fn_801F4C14();
L_8020B8EC: ;
    r3 = r31;
    r4 = r26;
    fn_802128D0();
    r3 = 0x1;
L_8020B8FC: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8020B910 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020B910(void) {
    extern void fn_801F00D0();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_8020D908();
    extern void fn_80211E18();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r3 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r3 = r27;
    fn_8020D908();
    r0 = r3;
    r3 = 0x0;
    r31 = r0;
    r4 = 0x0;
    r7 = r31;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r30 = r3;
    r5 = 0x1e;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r29 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = r29;
    r5 = 0x2;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020B9D4;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r3 = r3 & 0xFFFF;
    r4 = r28;
    fn_801F00D0();
    goto L_8020B9D8;
L_8020B9D4: ;
    r3 = r31;
L_8020B9D8: ;
    r7 = r3;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x42;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r27;
    r4 = r29;
    fn_80211E18();
    /* lmw r27, 0xc(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020BA14 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BA14(void) {
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_8020D908();
    extern void fn_80212D6C();
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
    r5 = 0x14;
    r6 = 0x0;
    r31 = r3;
    r3 = 0x0;
    fn_801F54A4();
    r3 = r31;
    fn_8020D908();
    r0 = r3;
    r3 = 0x0;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r31;
    fn_80212D6C();
    r3 = 0x1;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020BA80 | size: 0x78 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BA80(void) {
    extern void fn_801F4C14();
    extern void fn_8020D8D8();
    extern void fn_8020D908();
    extern void fn_80213158();
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
    fn_8020D908();
    r0 = r3;
    r3 = 0x0;
    r31 = r0;
    r4 = 0x0;
    r7 = r31;
    r5 = 0x45;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r30;
    fn_8020D8D8();
    r7 = (s16)r3;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x121;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r30;
    fn_80213158();
    /* lmw r30, 0x8(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020BAF8 | size: 0xAC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BAF8(void) {
    extern void fn_801F0058();
    extern void fn_801F3984();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_8020D908();
    extern void fn_80212840();
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
    r5 = 0x14;
    r6 = 0x0;
    /* stmw r30, 0x8(r1) */;
    r31 = r3;
    r3 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r3 = r31;
    fn_8020D908();
    r31 = r3;
    fn_80212840();
    r3 = r31;
    r4 = r30;
    fn_801F0058();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020BB5C;
    r3 = 0x0;
    r4 = 0x4;
    fn_801F3984();
    goto L_8020BB68;
L_8020BB5C: ;
    r3 = 0x0;
    r4 = 0x5;
    fn_801F3984();
L_8020BB68: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020BB8C;
    r7 = r31;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801F4C14();
L_8020BB8C: ;
    /* lmw r30, 0x8(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020BBA4 | size: 0x58 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BBA4(void) {
    extern void fn_801EF634();
    extern void fn_801F000C();
    extern void fn_801F4AC0();
    extern void fn_802119D4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    r31 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_8020BBCC;
    r3 = 0x1;
    goto L_8020BBE8;
L_8020BBCC: ;
    r3 = r31;
    fn_802119D4();
    r3 = 0x5;
    fn_801F000C();
    r3 = 0x0;
    fn_801F4AC0();
    r3 = 0x1;
L_8020BBE8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020BBFC | size: 0x98 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BBFC(void) {
    extern void fn_801EF634();
    extern void fn_801F3B24();
    extern void fn_801F4718();
    extern void fn_8020BC94();
    extern void fn_80211A00();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x0;
    fn_801F4718();
    r3 = 0x0;
    r4 = 0x1;
    fn_801F3B24();
    r3 = 0x0;
    r4 = 0x0;
    fn_8020BC94();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020BC40;
    goto L_8020BC80;
L_8020BC40: ;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_8020BC54;
    r3 = 0x1;
    goto L_8020BC80;
L_8020BC54: ;
    r3 = r31;
    fn_80211A00();
    r3 = 0x0;
    r4 = 0x1;
    fn_8020BC94();
    r0 = r3 & 0xFF;
    r4 = 0x1;
    if ((u32)r0 == (u32)0x1) goto L_8020BC7C;
    r4 = r3;
L_8020BC7C: ;
    r3 = r4;
L_8020BC80: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020BC94 | size: 0x1A4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BC94(void) {
    extern void fn_801EF634();
    extern void fn_801F0898();
    extern void fn_801F0F04();
    extern void fn_801F1170();
    extern void fn_801F4AC0();
    extern void fn_801F54A4();
    extern void fn_802062FC();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r27, 0x3c(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = 0x0;
    goto L_8020BE14;
L_8020BCB4: ;
    r3 = r29;
    r6 = r31;
    r4 = 0x0;
    r5 = 0x59;
    fn_801F54A4();
    /* mr. r27, r3 */;
    if ((s32)r0 == (s32)0) goto L_8020BE10;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020BCF8;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    r7 = 0x1;
    ((void(*)(void))fn_801254B4)();
    goto L_8020BE10;
L_8020BCF8: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r28, r3 */;
    if ((s32)r0 != (s32)0) goto L_8020BD30;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    r7 = 0x1;
    ((void(*)(void))fn_801254B4)();
    goto L_8020BE10;
L_8020BD30: ;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020BD58;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    r7 = 0x1;
    ((void(*)(void))fn_801254B4)();
    goto L_8020BE10;
L_8020BD58: ;
    r0 = r30 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020BD78;
    r3 = r28;
    fn_801F0898();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x8) goto L_8020BE10;
    goto L_8020BD8C;
L_8020BD78: ;
    r3 = r28;
    fn_801F0898();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x8) goto L_8020BE10;
L_8020BD8C: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 == (s32)0x1) goto L_8020BE10;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x112;
    r6 = 0x0;
    r7 = 0x1;
    ((void(*)(void))fn_801254B4)();
    r0 = 0x6;
    r5 = r1 + 0x4;
    /* subi r4, r28, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_8020BDD0: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8020BDD0;
    r3 = r1 + 0x8;
    fn_801F0F04();
    r0 = r30 & 0xFF;
    if ((s32)r3 == (s32)0x1) goto L_8020BE10;
    r3 = 0x0;
    fn_801F4AC0();
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((s32)r3 == (s32)0x1) goto L_8020BE10;
    r3 = 0x1;
    goto L_8020BE24;
L_8020BE10: ;
    r31 = r31 + 0x1;
L_8020BE14: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)0x8) goto L_8020BCB4;
    r3 = 0x1;
L_8020BE24: ;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* 0x8020BE38 | size: 0x108 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BE38(void) {
    extern void fn_80008174();
    extern void fn_801F2B5C();
    extern void fn_801F47B4();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_8026316C();
    extern void fn_8020BF40();
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
    fn_80008174();
    r0 = r3 & 0xFF;
    r30 = r3;
    if ((u32)r0 != (u32)0x1) goto L_8020BF10;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r25 = 0x0;
    goto L_8020BF00;
L_8020BE94: ;
    r4 = r25;
    r3 = 0x0;
    fn_801F47B4();
    /* mr. r29, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_8020BEFC;
    r31 = r25 & 0xFFFF;
    r26 = 0x0;
    goto L_8020BEF0;
L_8020BEB4: ;
    r3 = r29;
    r4 = r26;
    fn_801F7258();
    if ((u32)r3 == (u32)0x0) goto L_8020BEEC;
    r4 = r27;
    r5 = r30;
    fn_8026316C();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8020BEEC;
    if ((u32)r31 == (u32)0x0) goto L_8020BEB4;
    /* subi r25, r25, 0x1 */;
    goto L_8020BE94;
L_8020BEEC: ;
    r26 = r26 + 0x1;
L_8020BEF0: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020BEB4;
L_8020BEFC: ;
    r25 = r25 + 0x1;
L_8020BF00: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_8020BE94;
    goto L_8020BF28;
L_8020BF10: ;
    r4 = (u32)fn_8020BF40;
    r3 = 0x0;
    r4 = (u32)fn_8020BF40;
    r5 = 0x0;
    r6 = 0x1;
    fn_801F2B5C();
L_8020BF28: ;
    /* lmw r25, 0x14(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020BF40 | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BF40(void) {
    extern void fn_801EF634();
    extern void fn_801F150C();
    extern void fn_801F923C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_8020BF6C;
    r3 = 0x1;
    goto L_8020BF8C;
L_8020BF6C: ;
    r3 = r30;
    r4 = r31;
    fn_801F923C();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020BF88;
    r3 = 0x0;
    fn_801F150C();
L_8020BF88: ;
    r3 = 0x1;
L_8020BF8C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020BFA0 | size: 0x120 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020BFA0(void) {
    extern u8 lbl_80375CC8[];
    extern u8 lbl_80378AA0[];
    extern void fn_800E0C54();
    extern void fn_801DA7AC();
    extern void fn_801F2F3C();
    extern void fn_801F3074();
    extern void fn_801F3178();
    extern void fn_801F37B0();
    extern void fn_801F3B24();
    extern void fn_801F4718();
    extern void fn_801F4C14();
    extern void fn_8020D920();
    extern void fn_80211830();
    extern void fn_80211948();
    extern void fn_8022E1C4();
    extern void fn_8022E314();
    extern void fn_8020C0C0();
    extern void fn_8020C0E4();
    extern void fn_8020C108();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x0;
    fn_801F4718();
    r3 = 0x0;
    r4 = 0x0;
    fn_801F3B24();
    fn_80211830();
    r0 = 0x0;
    r3 = (u32)fn_8020C108;
    *(u8*)(sp + 0x8) = r0;
    r4 = (u32)fn_8020C108;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = r31;
    fn_8020D920();
    r4 = (u32)lbl_80375CC8;
    r5 = (u32)lbl_80378AA0;
    r7 = (u32)lbl_80375CC8;
    r6 = 0x0;
    r8 = (u32)lbl_80378AA0;
    r4 = 0x0;
    r5 = 0x6;
    fn_80211948();
    r4 = (u32)fn_8020C0E4;
    r3 = 0x0;
    r4 = (u32)fn_8020C0E4;
    r5 = 0x0;
    r6 = 0x1;
    fn_801F37B0();
    r3 = 0x1;
    fn_8022E314();
    fn_8022E1C4();
    r4 = (u32)fn_8020C0C0;
    r3 = 0x0;
    r4 = (u32)fn_8020C0C0;
    r5 = 0x0;
    r6 = 0x1;
    fn_801F37B0();
    r0 = 0x1;
    r3 = (u32)fn_8020C108;
    *(u8*)(sp + 0x8) = r0;
    r4 = (u32)fn_8020C108;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r3 = 0x0;
    fn_801F3178();
    r3 = 0x0;
    fn_801F3074();
    r3 = 0x0;
    fn_801F2F3C();
    fn_800E0C54();
    r7 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    fn_801F4C14();
    fn_801DA7AC();
    r3 = 0x1;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* 0x8020C0C0 | size: 0x24 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020C0C0(void) {
    extern void fn_8022D084();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    fn_8022D084();
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020C0E4 | size: 0x24 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020C0E4(void) {
    extern void fn_8022E410();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    fn_8022E410();
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020C108 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020C108(void) {
    extern void fn_8022E6F0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((u32)r5 == (u32)0x0) goto L_8020C130;
    r4 = *(u8*)((u8*)r5 + 0x0);
    fn_8022E6F0();
    goto L_8020C144;
L_8020C130: ;
    r4 = 0x0;
    fn_8022E6F0();
    r3 = r31;
    r4 = 0x1;
    fn_8022E6F0();
L_8020C144: ;
    r3 = 0x1;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020C15C | size: 0x6E4 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020C15C(void) {
    extern u8 lbl_8047B5F8[];
    extern u8 lbl_8047E520[];
    extern void fn_800F0308();
    extern void fn_80103BA8();
    extern void fn_80132A38();
    extern void fn_80165A20();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern void fn_801DA94C();
    extern void fn_801DA9B4();
    extern void fn_801DA9E8();
    extern void fn_801EF7C4();
    extern void fn_801F025C();
    extern void fn_801F1888();
    extern void fn_801F54A4();
    extern void fn_801F8000();
    extern void fn_801F8100();
    extern void fn_801FB1C0();
    extern void fn_8020DFC0();
    extern void fn_8020E0F8();
    extern void fn_8026246C();
    extern void fn_80262490();
    u8 sp[0x80];
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
    f32 f1 = 0.0f;

    r3 = 0x0;
    r4 = 0x0;
    r0 = 0x0;
    r5 = 0xe;
    r6 = 0x0;
    /* stmw r24, 0x60(r1) */;
    *(u8*)(sp + 0x8) = r0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    r3 = 0xb;
    r4 = 0x0;
    fn_801F025C();
    r4 = 0x0;
    r25 = r3;
    r5 = 0x4c;
    r6 = 0x0;
    fn_801FB1C0();
    r4 = r25;
    r25 = r3;
    r3 = 0x9;
    fn_801F025C();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x4c;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3;
    r3 = r29;
    r26 = r0;
    r4 = 0x0;
    r5 = 0x43;
    r6 = 0x0;
    fn_801FB1C0();
    r24 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    fn_801F54A4();
    r30 = r3;
    r4 = r27;
    r3 = 0x0;
    r5 = 0xd;
    r6 = 0x0;
    fn_801F54A4();
    r3 = r3 & 0xFFFF;
    fn_8020E0F8();
    fn_8020DFC0();
    r31 = r3;
    r3 = r29;
    r4 = r24;
    r5 = 0x7;
    r6 = 0x0;
    fn_801FB1C0();
    /* mr. r28, r3 */;
    if ((s32)r0 != (s32)0) goto L_8020C24C;
    r28 = 0x5f;
L_8020C24C: ;
    r4 = r24;
    r3 = 0x0;
    r5 = 0x8;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3;
    r3 = 0x0;
    r27 = r0;
    fn_801F1888();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020C69C;
    if ((u32)r30 == (u32)0x0) goto L_8020C430;
    if ((u32)r31 == (u32)0x0) goto L_8020C298;
    r3 = r26;
    r4 = r31 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
L_8020C298: ;
    r3 = r26;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
L_8020C2A8: ;
    /* addic. r0, r1, 0x8 */;
    if ((u32)r31 != (u32)0x0) goto L_8020C2B8;
    r0 = 0x0;
    goto L_8020C2EC;
L_8020C2B8: ;
    r3 = r1 + 0x44;
    r4 = 0x1;
    fn_80103BA8();
    r3 = 0x0;
    fn_801C40F0();
    r0 = (s8)r3;
    if ((u32)r31 != (u32)0x0) goto L_8020C2E8;
    r0 = *(u16*)(sp + 0x44);
    r0 = r0 & 0x00000020;
    if ((u32)r31 == (u32)0x0) goto L_8020C2E8;
    r0 = 0x1;
    *(u8*)(sp + 0x8) = r0;
L_8020C2E8: ;
    r0 = *(u8*)(sp + 0x8);
L_8020C2EC: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C430;
    r3 = r26;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C318;
    fn_800F0308();
    goto L_8020C2A8;
L_8020C318: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020C3B8;
    r4 = *(u16*)lbl_8047B5F8;
    r3 = r26;
    r5 = 0x4;
    fn_801DA9E8();
L_8020C348: ;
    /* addic. r0, r1, 0x8 */;
    if ((u32)r0 != (u32)0x1) goto L_8020C358;
    r0 = 0x0;
    goto L_8020C38C;
L_8020C358: ;
    r3 = r1 + 0x28;
    r4 = 0x1;
    fn_80103BA8();
    r3 = 0x0;
    fn_801C40F0();
    r0 = (s8)r3;
    if ((u32)r0 != (u32)0x1) goto L_8020C388;
    r0 = *(u16*)(sp + 0x28);
    r0 = r0 & 0x00000020;
    if ((u32)r0 == (u32)0x1) goto L_8020C388;
    r0 = 0x1;
    *(u8*)(sp + 0x8) = r0;
L_8020C388: ;
    r0 = *(u8*)(sp + 0x8);
L_8020C38C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C430;
    r4 = *(u16*)lbl_8047B5F8;
    r3 = r26;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C3B8;
    fn_800F0308();
    goto L_8020C348;
L_8020C3B8: ;
    if ((u32)r31 == (u32)0x0) goto L_8020C430;
L_8020C3C0: ;
    /* addic. r0, r1, 0x8 */;
    if ((u32)r31 != (u32)0x0) goto L_8020C3D0;
    r0 = 0x0;
    goto L_8020C404;
L_8020C3D0: ;
    r3 = r1 + 0xc;
    r4 = 0x1;
    fn_80103BA8();
    r3 = 0x0;
    fn_801C40F0();
    r0 = (s8)r3;
    if ((u32)r31 != (u32)0x0) goto L_8020C400;
    r0 = *(u16*)(sp + 0xC);
    r0 = r0 & 0x00000020;
    if ((u32)r31 == (u32)0x0) goto L_8020C400;
    r0 = 0x1;
    *(u8*)(sp + 0x8) = r0;
L_8020C400: ;
    r0 = *(u8*)(sp + 0x8);
L_8020C404: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C430;
    r3 = r26;
    r4 = r31 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C430;
    fn_800F0308();
    goto L_8020C3C0;
L_8020C430: ;
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r0 != (u32)0x1) goto L_8020C4A8;
    f1 = *(f32*)lbl_8047E520;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    if ((u32)r31 == (u32)0x0) goto L_8020C468;
    r3 = r26;
    r4 = r31 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9B4();
L_8020C468: ;
    r3 = r26;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9B4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020C4A8;
    r4 = *(u16*)lbl_8047B5F8;
    r3 = r26;
    r5 = 0x4;
    fn_801DA9B4();
L_8020C4A8: ;
    if ((u32)r27 == (u32)0x0) goto L_8020C50C;
    r3 = r26;
    r4 = 0x5f;
    r5 = 0x4;
    fn_801DA9E8();
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r0 != (u32)0x1) goto L_8020C4E0;
    f1 = *(f32*)lbl_8047E520;
    r3 = 0x2;
    fn_801C41C8();
    r0 = 0x0;
    *(u8*)(sp + 0x8) = r0;
L_8020C4E0: ;
    r3 = r27;
    fn_80262490();
L_8020C4E8: ;
    r3 = r26;
    r4 = 0x5f;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C508;
    fn_800F0308();
    goto L_8020C4E8;
L_8020C508: ;
    fn_8026246C();
L_8020C50C: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x11;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r3 == (u32)0x0) goto L_8020C534;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();
L_8020C534: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020C5D8;
    r3 = r29;
    fn_801F8000();
    r4 = r3;
    r3 = 0x22;
    fn_80132A38();
    r3 = r29;
    fn_801F8100();
    r4 = r3;
    r3 = 0x23;
    fn_80132A38();
    r3 = r26;
    r4 = r28 & 0xFFFF;
    r5 = 0x4;
    fn_801DA9E8();
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r0 != (u32)0x1) goto L_8020C5AC;
    f1 = *(f32*)lbl_8047E520;
    r3 = 0x2;
    fn_801C41C8();
    r0 = 0x0;
    *(u8*)(sp + 0x8) = r0;
L_8020C5AC: ;
    r3 = 0x766d;
    fn_80262490();
L_8020C5B4: ;
    r3 = r26;
    r4 = r28 & 0xFFFF;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C5D4;
    fn_800F0308();
    goto L_8020C5B4;
L_8020C5D4: ;
    fn_8026246C();
L_8020C5D8: ;
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r0 != (u32)0x1) goto L_8020C5F0;
    f1 = *(f32*)lbl_8047E520;
    r3 = 0x2;
    fn_801C41C8();
L_8020C5F0: ;
    if ((u32)r30 == (u32)0x0) goto L_8020C650;
    if ((u32)r31 == (u32)0x0) goto L_8020C610;
    r3 = r26;
    r4 = r31 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
L_8020C610: ;
    r3 = r26;
    r4 = r30 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020C650;
    r4 = *(u16*)lbl_8047B5F8;
    r3 = r26;
    r5 = 0x4;
    fn_801DA8C4();
L_8020C650: ;
    if ((u32)r27 == (u32)0x0) goto L_8020C668;
    r3 = r26;
    r4 = 0x5f;
    r5 = 0x4;
    fn_801DA8C4();
L_8020C668: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020C828;
    r3 = r26;
    r4 = r28 & 0xFFFF;
    r5 = 0x4;
    fn_801DA8C4();
    goto L_8020C828;
L_8020C69C: ;
    r3 = 0x0;
    fn_801EF7C4();
    r3 = r25;
    r4 = 0x1;
    fn_801DA4E8();
    r3 = r25;
    r4 = 0x54;
    r5 = 0x4;
    fn_801DA9E8();
L_8020C6C0: ;
    r3 = r25;
    r4 = 0x54;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C6E0;
    fn_800F0308();
    goto L_8020C6C0;
L_8020C6E0: ;
    r3 = 0x0;
    fn_801EF7C4();
    r3 = r26;
    r4 = 0x1;
    fn_801DA4E8();
    r3 = r26;
    r4 = 0x55;
    r5 = 0x4;
    fn_801DA9E8();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020C754;
    r3 = r29;
    fn_801F8000();
    r4 = r3;
    r3 = 0x22;
    fn_80132A38();
    r3 = r29;
    fn_801F8100();
    r4 = r3;
    r3 = 0x23;
    fn_80132A38();
    r3 = 0x766d;
    fn_80262490();
L_8020C754: ;
    r3 = r26;
    r4 = 0x55;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C774;
    fn_800F0308();
    goto L_8020C754;
L_8020C774: ;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020C798;
    fn_8026246C();
L_8020C798: ;
    r3 = 0x1;
    fn_801EF7C4();
    r3 = r25;
    r4 = 0x56;
    r5 = 0x4;
    fn_801DA9E8();
L_8020C7B0: ;
    r3 = r25;
    r4 = 0x56;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020C7D0;
    fn_800F0308();
    goto L_8020C7B0;
L_8020C7D0: ;
    r3 = r25;
    r4 = 0x54;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r26;
    r4 = 0x55;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = r25;
    r4 = 0x56;
    r5 = 0x4;
    fn_801DA8C4();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x11;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r3 == (u32)0x0) goto L_8020C828;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();
L_8020C828: ;
    /* lmw r24, 0x60(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020CA98 | size: 0x548 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020CA98(void) {
    extern void fn_8010AE2C();
    extern void fn_80121C18();
    extern void fn_80132A38();
    extern void fn_801C3430();
    extern void fn_801C3E3C();
    extern void fn_801F02AC();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F7388();
    extern void fn_801F7404();
    extern void fn_801F8000();
    extern void fn_801F8100();
    extern void fn_801F981C();
    extern void fn_801F98BC();
    extern void fn_801FB1C0();
    extern void fn_801FBC20();
    extern void fn_80204A10();
    extern void fn_80205A7C();
    extern void fn_80205AD4();
    extern void fn_80205BE8();
    extern void fn_80206608();
    extern void fn_802068C8();
    extern void fn_80208028();
    extern void fn_80208C18();
    extern void fn_8020CFE0();
    extern void fn_8020D814();
    extern void fn_8020D920();
    extern void fn_8026246C();
    extern void fn_8026532C();
    extern void fn_80265598();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
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

    /* stmw r19, 0xc(r1) */;
    fn_8020D920();
    fn_8020D814();
    r21 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r31 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r3 = r21;
    r5 = r29;
    r4 = 0x0;
    fn_801F02AC();
    r30 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020CB24;
    r3 = 0x0;
    goto L_8020CFCC;
L_8020CB24: ;
    r3 = r30;
    fn_801F7388();
    r27 = r3 & 0xFF;
    r23 = 0x0;
    goto L_8020CC6C;
L_8020CB38: ;
    r3 = r30;
    r4 = r23;
    fn_801F7258();
    /* mr. r25, r3 */;
    if ((s32)r0 == (s32)0) goto L_8020CC68;
    r4 = 0x0;
    r5 = 0x4c;
    r6 = 0x0;
    fn_801FB1C0();
    /* mr. r21, r3 */;
    if ((s32)r0 == (s32)0) goto L_8020CC68;
    r3 = r25;
    fn_801F98BC();
    r24 = 0x0;
    r22 = 0x0;
    goto L_8020CC5C;
L_8020CB78: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 >= (u32)r31) goto L_8020CC68;
    if ((u32)r0 >= (u32)0x2) goto L_8020CC68;
    r3 = r25;
    r6 = r22;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    r20 = r3;
    fn_80206608();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_8020CC58;
    r3 = r20;
    r4 = 0x0;
    r5 = 0x0;
    fn_8010AE2C();
    r3 = r20;
    fn_80205BE8();
    fn_80121C18();
    r0 = r3;
    r3 = r25;
    r19 = r0;
    r6 = r22;
    r4 = 0x0;
    r5 = 0x46;
    fn_801FB1C0();
    r4 = r20;
    r26 = r3;
    r5 = r19;
    fn_802068C8();
    r3 = r26;
    r24 = r24 + 0x1;
    fn_80208028();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x1e;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020CC4C;
    r3 = r26;
    fn_80204A10();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020CC4C;
    r3 = r26;
    r4 = 0x0;
    fn_80205AD4();
    r3 = r26;
    r4 = 0x0;
    fn_80205A7C();
L_8020CC4C: ;
    r3 = r21;
    r4 = r19;
    fn_801C3E3C();
L_8020CC58: ;
    r22 = r22 + 0x1;
L_8020CC5C: ;
    r0 = r22 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8020CB78;
L_8020CC68: ;
    r23 = r23 + 0x1;
L_8020CC6C: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020CB38;
    r21 = 0x0;
    goto L_8020CD08;
L_8020CC80: ;
    r3 = r30;
    r4 = r21;
    fn_801F7258();
    /* mr. r22, r3 */;
    if ((u32)r0 == (u32)r28) goto L_8020CD04;
    r23 = 0x0;
    goto L_8020CCB4;
L_8020CC9C: ;
    r3 = r22;
    r4 = r23;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r0 != (u32)r28) goto L_8020CCC0;
    r23 = r23 + 0x1;
L_8020CCB4: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020CC9C;
L_8020CCC0: ;
    r3 = r22;
    r4 = r26;
    r5 = 0x0;
    fn_801FBC20();
    r23 = 0x0;
    goto L_8020CCF8;
L_8020CCD8: ;
    r3 = r22;
    r4 = r23;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r0 == (u32)r31) goto L_8020CCF4;
    r4 = 0x0;
    fn_80208C18();
L_8020CCF4: ;
    r23 = r23 + 0x1;
L_8020CCF8: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020CCD8;
L_8020CD04: ;
    r21 = r21 + 0x1;
L_8020CD08: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020CC80;
    r25 = 0x0;
    goto L_8020CF1C;
L_8020CD1C: ;
    r3 = r30;
    r4 = r25;
    fn_801F7258();
    /* mr. r24, r3 */;
    if ((u32)r0 == (u32)r28) goto L_8020CF18;
    fn_801F98BC();
    r19 = r3 & 0xFF;
    r21 = 0x0;
    goto L_8020CD78;
L_8020CD40: ;
    r3 = r24;
    r4 = r21;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r0 == (u32)r28) goto L_8020CD74;
    r3 = r24;
    r4 = r26;
    r5 = r27;
    r6 = r19;
    r7 = r25;
    r8 = r21;
    r9 = 0x0;
    fn_8020CFE0();
L_8020CD74: ;
    r21 = r21 + 0x1;
L_8020CD78: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020CD40;
    r21 = 0x0;
    goto L_8020CDA4;
L_8020CD8C: ;
    r3 = r24;
    r4 = r21;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r0 != (u32)r31) goto L_8020CDB0;
    r21 = r21 + 0x1;
L_8020CDA4: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020CD8C;
L_8020CDB0: ;
    fn_801C3430();
    r3 = r24;
    r4 = r26;
    r5 = 0x1;
    fn_801FBC20();
    r3 = r24;
    fn_801F8000();
    r4 = r3;
    r3 = 0x22;
    fn_80132A38();
    r3 = r24;
    fn_801F8100();
    r4 = r3;
    r3 = 0x23;
    fn_80132A38();
    r3 = r24;
    fn_801F8100();
    r4 = r3;
    r3 = 0x25;
    fn_80132A38();
    r3 = r24;
    r4 = r26;
    r5 = r27;
    r6 = r19;
    r7 = r25;
    r8 = r21;
    r9 = 0x1;
    fn_8020CFE0();
    r3 = r24;
    r4 = r26;
    r5 = 0x2;
    fn_801FBC20();
    r21 = 0x0;
    goto L_8020CF0C;
L_8020CE38: ;
    r3 = r24;
    r4 = r21;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r0 == (u32)r31) goto L_8020CF08;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F54A4();
    r20 = r3;
    r7 = r26;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r26;
    r4 = 0x1;
    fn_80208C18();
    r3 = r26;
    fn_80204A10();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)r31) goto L_8020CEAC;
    r3 = r26;
    r4 = r29;
    r5 = 0x0;
    fn_80265598();
    goto L_8020CEBC;
L_8020CEAC: ;
    r3 = r26;
    r4 = r29;
    r5 = 0x1;
    fn_80265598();
L_8020CEBC: ;
    r3 = r26;
    r4 = 0x2;
    fn_80208C18();
    r3 = r26;
    r4 = 0x3;
    fn_80208C18();
    r3 = r26;
    r4 = 0x4;
    fn_80208C18();
    r3 = r26;
    r4 = r29;
    r5 = 0x0;
    fn_8026532C();
    r7 = r20;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
L_8020CF08: ;
    r21 = r21 + 0x1;
L_8020CF0C: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020CE38;
L_8020CF18: ;
    r25 = r25 + 0x1;
L_8020CF1C: ;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020CD1C;
    fn_8026246C();
    r21 = 0x0;
    goto L_8020CFBC;
L_8020CF34: ;
    r3 = r30;
    r4 = r21;
    fn_801F7258();
    /* mr. r22, r3 */;
    if ((u32)r0 == (u32)r28) goto L_8020CFB8;
    r23 = 0x0;
    goto L_8020CF68;
L_8020CF50: ;
    r3 = r22;
    r4 = r23;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r0 != (u32)r28) goto L_8020CF74;
    r23 = r23 + 0x1;
L_8020CF68: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020CF50;
L_8020CF74: ;
    r3 = r22;
    r4 = r26;
    r5 = 0x3;
    fn_801FBC20();
    r23 = 0x0;
    goto L_8020CFAC;
L_8020CF8C: ;
    r3 = r22;
    r4 = r23;
    fn_801F981C();
    /* mr. r26, r3 */;
    if ((u32)r0 == (u32)r31) goto L_8020CFA8;
    r4 = 0x5;
    fn_80208C18();
L_8020CFA8: ;
    r23 = r23 + 0x1;
L_8020CFAC: ;
    r0 = r23 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020CF8C;
L_8020CFB8: ;
    r21 = r21 + 0x1;
L_8020CFBC: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020CF34;
    r3 = 0x1;
L_8020CFCC: ;
    /* lmw r19, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020CFE0 | size: 0x21C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020CFE0(void) {
    extern void fn_80132A38();
    extern void fn_801F18DC();
    extern void fn_801F8000();
    extern void fn_80204A10();
    extern void fn_80205B8C();
    extern void fn_802624CC();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r4;
    r29 = r3;
    r26 = r5;
    r27 = r6;
    r28 = r8;
    r31 = r9;
    r3 = r25;
    fn_80204A10();
    r0 = r3 & 0xFF;
    r3 = 0x0;
    r0 = 0x1 - r0;
    r0 = __cntlzw(r0);
    r30 = (u32)r0 >> 5;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    r3 = r29;
    r0 = 0x1 - r0;
    r0 = __cntlzw(r0);
    r29 = (u32)r0 >> 5;
    fn_801F8000();
    if ((u32)r3 != (u32)0x0) goto L_8020D054;
    r0 = r30 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8020D054;
    r29 = 0x1;
L_8020D054: ;
    r3 = r25;
    fn_80205B8C();
    r4 = 0x0;
    r5 = 0x77;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r31 & 0xFF;
    r31 = r3;
    if ((u32)r3 != (u32)0x0) goto L_8020D150;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D0C4;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D0A8;
    r4 = r31;
    r3 = 0x14;
    fn_80132A38();
    r4 = r31;
    r3 = 0x16;
    fn_80132A38();
    goto L_8020D1E8;
L_8020D0A8: ;
    r4 = r31;
    r3 = 0x15;
    fn_80132A38();
    r4 = r31;
    r3 = 0x17;
    fn_80132A38();
    goto L_8020D1E8;
L_8020D0C4: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D110;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D0F4;
    r4 = r31;
    r3 = 0x15;
    fn_80132A38();
    r4 = r31;
    r3 = 0x17;
    fn_80132A38();
    goto L_8020D1E8;
L_8020D0F4: ;
    r4 = r31;
    r3 = 0x14;
    fn_80132A38();
    r4 = r31;
    r3 = 0x16;
    fn_80132A38();
    goto L_8020D1E8;
L_8020D110: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D134;
    r4 = r31;
    r3 = 0x14;
    fn_80132A38();
    r4 = r31;
    r3 = 0x16;
    fn_80132A38();
    goto L_8020D1E8;
L_8020D134: ;
    r4 = r31;
    r3 = 0x15;
    fn_80132A38();
    r4 = r31;
    r3 = 0x17;
    fn_80132A38();
    goto L_8020D1E8;
L_8020D150: ;
    if ((u32)r0 != (u32)0x1) goto L_8020D1E8;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 > (u32)0x1) goto L_8020D1A0;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 <= (u32)0x1) goto L_8020D1A0;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D184;
    r3 = 0x7674;
    goto L_8020D1E4;
L_8020D184: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D198;
    r3 = 0x7679;
    goto L_8020D1E4;
L_8020D198: ;
    r3 = 0x7671;
    goto L_8020D1E4;
L_8020D1A0: ;
    r4 = r31;
    r3 = 0x14;
    fn_80132A38();
    r4 = r31;
    r3 = 0x16;
    fn_80132A38();
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D1CC;
    r3 = 0x7673;
    goto L_8020D1E4;
L_8020D1CC: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D1E0;
    r3 = 0x7678;
    goto L_8020D1E4;
L_8020D1E0: ;
    r3 = 0x7670;
L_8020D1E4: ;
    fn_802624CC();
L_8020D1E8: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8020D1FC | size: 0x49C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020D1FC(void) {
    extern void fn_8006B0F8();
    extern void fn_8006B57C();
    extern void fn_801233F4();
    extern void fn_80123FBC();
    extern void fn_8012A5B0();
    extern void fn_8012AC64();
    extern void fn_801C3430();
    extern void fn_801C3FBC();
    extern void fn_801DA4E8();
    extern void fn_801F02AC();
    extern void fn_801F4804();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F72B0();
    extern void fn_801F7388();
    extern void fn_801F7404();
    extern void fn_801F76B8();
    extern void fn_801F8FD8();
    extern void fn_801F9930();
    extern void fn_801F99C8();
    extern void fn_801F9CBC();
    extern void fn_801FA4B4();
    extern void fn_801FA634();
    extern void fn_801FA6D8();
    extern void fn_801FB1C0();
    extern void fn_801FB8F8();
    extern void fn_802032E4();
    extern void fn_80206AEC();
    extern void fn_8020D814();
    extern void fn_8020D920();
    extern void fn_8020E020();
    extern void fn_8020E068();
    extern void fn_8020E0F8();
    u8 sp[0xB60];
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

    /* stmw r18, 0xb28(r1) */;
    fn_8020D920();
    fn_8020D814();
    r18 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0xd;
    r6 = 0x0;
    fn_801F54A4();
    r3 = r3 & 0xFFFF;
    fn_8020E0F8();
    r0 = r3;
    r3 = 0x0;
    r23 = r0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r19 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x16;
    r6 = 0x0;
    fn_801F54A4();
    r28 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x17;
    r6 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x18;
    r6 = 0x0;
    fn_801F54A4();
    r31 = r3 & 0xFFFF;
    r3 = r18;
    r5 = r19;
    r4 = 0x0;
    fn_801F02AC();
    r25 = r3;
    fn_801F7404();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_8020D2C4;
    r3 = 0x0;
    goto L_8020D684;
L_8020D2C4: ;
    r5 = 0x4 - r18;
    r3 = r25;
    /* subic r0, r5, 0x1 */;
    r4 = 0x0;
    r18 = r5 - r0; /* -borrow */;
    r5 = 0x5;
    r6 = 0x0;
    fn_801F76B8();
    r0 = r18 & 0xFFFF;
    r30 = r3 & 0xFFFF;
    r29 = r0 * r28;
    r24 = 0x0;
    goto L_8020D5EC;
L_8020D2F8: ;
    r3 = r25;
    r6 = r24;
    r4 = 0x0;
    r5 = 0x7;
    fn_801F76B8();
    r0 = r24 + r29;
    r4 = r3;
    r20 = r0 & 0xFF;
    r3 = r23;
    r26 = r4;
    r4 = r20;
    fn_8020E068();
    r0 = r3;
    r3 = r23;
    r18 = r0;
    r4 = r20;
    fn_8020E020();
    r0 = r3;
    r3 = r18;
    r19 = r0;
    r4 = r19;
    fn_801FA4B4();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8020D5E8;
    fn_8006B57C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D380;
    r3 = r20;
    fn_8006B0F8();
    r4 = r3;
    r3 = r1 + 0xc;
    fn_8012AC64();
    goto L_8020D390;
L_8020D380: ;
    r3 = r18;
    r4 = r19;
    r5 = r1 + 0xc;
    fn_801F9CBC();
L_8020D390: ;
    r3 = r18;
    fn_801F8FD8();
    r0 = r3;
    r3 = r26;
    r7 = r0;
    r5 = r18;
    r6 = r19;
    r4 = r1 + 0xc;
    fn_801FA6D8();
    r3 = r26;
    r4 = 0x0;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3;
    r3 = r26;
    r22 = r0;
    fn_801FA634();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8020D5E8;
    r3 = r26;
    r4 = r27;
    r5 = r31;
    fn_801F99C8();
    r21 = 0x0;
    r19 = 0x0;
    goto L_8020D4E8;
L_8020D3FC: ;
    r0 = (s8)r21;
    if ((s32)r0 >= (s32)r31) goto L_8020D4F4;
    if ((s32)r0 >= (s32)r27) goto L_8020D4F4;
    if ((s32)r0 >= (s32)0x6) goto L_8020D4F4;
    r3 = r22;
    r5 = r19;
    r4 = 0x3;
    fn_8012A5B0();
    r18 = r3;
    fn_801233F4();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x6) goto L_8020D4E4;
    r3 = r26;
    r4 = r18;
    fn_801F9930();
    if ((u32)r3 != (u32)0x0) goto L_8020D4E4;
    r3 = r26;
    r6 = (s8)r21;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    r0 = r3;
    r3 = 0x0;
    r20 = r0;
    fn_801F4804();
    r5 = r3;
    r3 = r20;
    r4 = r18;
    fn_80206AEC();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D4E0;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x2e;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D4E0;
    r3 = r26;
    fn_801FB8F8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D4E0;
    r3 = r20;
    r4 = 0x3;
    fn_802032E4();
L_8020D4E0: ;
    r21 = r21 + 0x1;
L_8020D4E4: ;
    r19 = r19 + 0x1;
L_8020D4E8: ;
    r0 = r19 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8020D3FC;
L_8020D4F4: ;
    r20 = 0x0;
    goto L_8020D5DC;
L_8020D4FC: ;
    r0 = (s8)r21;
    if ((s32)r0 >= (s32)r27) goto L_8020D5E8;
    if ((s32)r0 >= (s32)0x6) goto L_8020D5E8;
    r3 = r22;
    r5 = r20;
    r4 = 0x3;
    fn_8012A5B0();
    r18 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x6) goto L_8020D5D8;
    r3 = r26;
    r4 = r18;
    fn_801F9930();
    if ((u32)r3 != (u32)0x0) goto L_8020D5D8;
    r3 = r26;
    r6 = (s8)r21;
    r4 = 0x0;
    r5 = 0x45;
    fn_801FB1C0();
    r19 = r3;
    r3 = 0x0;
    fn_801F4804();
    r5 = r3;
    r3 = r19;
    r4 = r18;
    fn_80206AEC();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D5D4;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x2e;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D5D4;
    r3 = r26;
    fn_801FB8F8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020D5D4;
    r3 = r19;
    r4 = 0x3;
    fn_802032E4();
L_8020D5D4: ;
    r21 = r21 + 0x1;
L_8020D5D8: ;
    r20 = r20 + 0x1;
L_8020D5DC: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_8020D4FC;
L_8020D5E8: ;
    r24 = r24 + 0x1;
L_8020D5EC: ;
    r0 = r24 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020D2F8;
    r3 = r25;
    fn_801F7388();
    r18 = r3 & 0xFF;
    r20 = 0x0;
    goto L_8020D674;
L_8020D60C: ;
    r3 = r25;
    r4 = r20;
    fn_801F7258();
    if ((u32)r3 == (u32)0x0) goto L_8020D670;
    r4 = 0x0;
    r5 = 0x4c;
    r6 = 0x0;
    fn_801FB1C0();
    /* mr. r19, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_8020D670;
    r3 = r30;
    r4 = r18;
    r5 = r20;
    r6 = r1 + 0x9;
    r7 = r1 + 0x8;
    fn_801F72B0();
    r4 = *(u8*)(sp + 0x9);
    r3 = r19;
    r5 = *(u8*)(sp + 0x8);
    fn_801C3FBC();
    fn_801C3430();
    r3 = r19;
    r4 = 0x1;
    fn_801DA4E8();
L_8020D670: ;
    r20 = r20 + 0x1;
L_8020D674: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020D60C;
    r3 = 0x1;
L_8020D684: ;
    /* lmw r18, 0xb28(r1) */;
    return;
}
#pragma pop

/* 0x8020D698 | size: 0xEC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020D698(void) {
    extern void fn_801EF624();
    extern void fn_801F02AC();
    extern void fn_801F17B0();
    extern void fn_801F4860();
    extern void fn_801F54A4();
    extern void fn_801F7480();
    extern void fn_8020E0B0();
    extern void fn_8020E0F8();
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
    fn_801EF624();
    r29 = r3;
    fn_8020E0F8();
    r0 = r3;
    r3 = 0x0;
    r31 = r0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r29;
    fn_801F4860();
    r3 = 0x0;
    fn_801F17B0();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    r3 = r31;
    fn_8020E0B0();
    r30 = r3;
    r5 = r29;
    r3 = 0x4;
    r4 = 0x0;
    fn_801F02AC();
    r31 = r3;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x3;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r31;
    fn_801F7480();
    r5 = r29;
    r3 = 0x5;
    r4 = 0x0;
    fn_801F02AC();
    r31 = r3;
    r4 = r30;
    r3 = 0x0;
    r5 = 0x3;
    r6 = 0x1;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r31;
    fn_801F7480();
    /* lmw r29, 0x14(r1) */;
    r3 = 0x1;
    return;
}
#pragma pop

/* 0x8020D7CC | size: 0x1C | tiny */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020D7CC(void) {
    u32 r0 = 0;
    u32 r3 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8020D7DC;
    r3 = -0x80;
    return;
L_8020D7DC: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r3 = (s8)r0;
    return;
}
#pragma pop

/* 0x8020D7E8 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020D7E8(void) {
    extern u8 lbl_80375BB8[];
    extern u8 lbl_80478D48[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u32*)lbl_80478D48;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020D800;
    r3 = 0x0;
    return;
L_8020D800: ;
    r4 = r3 * 0xc;
    r3 = (u32)lbl_80375BB8;
    r0 = (u32)lbl_80375BB8;
    r3 = r0 + r4;
    return;
}
#pragma pop

/* 0x8020D844 | size: 0x24 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020D844(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r3 == (u32)0x0) return;
    r0 = r4 & 0xFFFF;
    if ((u32)r0 >= (u32)0x4) return;
    /* clrlslwi r0, r4, 16, 2 */;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x20) = r5;
    return;
}
#pragma pop

/* 0x8020D968 | size: 0x38 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020D968(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((u32)r3 == (u32)0x0) return;
    if ((u32)r4 == (u32)0x0) return;
    r0 = 0x6;
    /* subi r5, r3, 0x4 */;
    /* subi r4, r4, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_8020D988: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8020D988;
    return;
}
#pragma pop

/* 0x8020D9E8 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020D9E8(void) {
    extern u8 lbl_80375CB8[];
    extern u8 lbl_80478D50[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u32*)lbl_80478D50;
    r3 = r3 & 0xFFFF;
    if ((u32)r3 < (u32)r0) goto L_8020DA00;
    r3 = 0x0;
    return;
L_8020DA00: ;
    r4 = r3 * 0x6;
    r3 = (u32)lbl_80375CB8;
    r0 = (u32)lbl_80375CB8;
    r3 = r0 + r4;
    return;
}
#pragma pop

/* 0x8020DA14 | size: 0xBC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DA14(void) {
    extern void fn_80136078();
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_8020A8E0();
    extern void fn_8020D9A0();
    extern void fn_8020D9B8();
    extern void fn_8020D9D0();
    extern void fn_8020D9E8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r29 = r4;
    r27 = r3;
    r3 = r29;
    fn_8020D9E8();
    fn_8020D9D0();
    r28 = r3;
    r3 = r29;
    fn_8020D9E8();
    fn_8020D9B8();
    r30 = r3;
    r3 = r29;
    fn_8020D9E8();
    fn_8020D9A0();
    r31 = r3;
    r29 = 0x0;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r5 = r3 & 0xFFFF;
    r3 = r30;
    r4 = r27;
    fn_801F02AC();
    r30 = r3;
    r3 = r28;
    r4 = r27;
    fn_8020A8E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020DAB8;
    r4 = r30;
    r5 = r27;
    r3 = r31 & 0xFFFF;
    r6 = 0x0;
    fn_80136078();
    r29 = 0x1;
L_8020DAB8: ;
    r3 = r29;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020DAD0 | size: 0x274 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DAD0(void) {
    extern u8 lbl_8047E528[];
    extern u8 lbl_8047E52C[];
    extern void fn_800F0308();
    extern void fn_800FF56C();
    extern void fn_800FF730();
    extern void fn_80112700();
    extern void fn_8011288C();
    extern void fn_8011395C();
    extern void fn_80113FE8();
    extern void fn_801140C8();
    extern void fn_80129474();
    extern void fn_8012A5B0();
    extern void fn_80132A38();
    extern void fn_801657D0();
    extern void fn_80165A20();
    extern void fn_80166AB8();
    extern void fn_8018DA88();
    extern void fn_801902E0();
    extern void fn_801903B0();
    extern void fn_80190528();
    extern void fn_801C40F0();
    extern void fn_801C4164();
    extern void fn_801C41C8();
    extern void fn_801D0AFC();
    extern void fn_801D23C0();
    extern void fn_801EF61C();
    extern void fn_801EF62C();
    extern void fn_801EF634();
    extern void fn_801EF7B4();
    extern void fn_801F1DBC();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_801FCC7C();
    extern void fn_801FCCC4();
    extern void fn_8020DE50();
    extern void fn_8020DE68();
    extern void fn_8020DE80();
    extern void fn_8020DE98();
    extern void fn_8020DEB0();
    extern void fn_8020DFD8();
    extern void fn_8020DFF0();
    extern void fn_8020E068();
    extern void fn_8020E0B0();
    extern void fn_8020E0E0();
    extern void fn_8020E0F8();
    extern void fn_8020E260();
    extern void fn_8020E488();
    extern void fn_80261388();
    extern void fn_80261444();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r0 = r3 & 0xFFFF;
    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    if ((s32)r0 != (s32)0) goto L_8020DAF4;
    r3 = 0x0;
    goto L_8020DD30;
L_8020DAF4: ;
    fn_8020E0F8();
    r0 = r3;
    r3 = 0x0;
    r29 = r0;
    fn_801EF62C();
    r3 = 0x9b0;
    fn_801903B0();
    r3 = r27;
    fn_801EF61C();
    fn_800FF56C();
    r0 = r3;
    r3 = 0x0;
    r7 = r0;
    r4 = 0x0;
    r5 = 0x4a;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r29;
    fn_8020E0B0();
    r0 = r3;
    r3 = 0x0;
    r4 = r0;
    r5 = 0x2;
    r6 = 0x0;
    fn_801F54A4();
    r27 = r3 & 0xFFFF;
    fn_801D23C0();
    r3 = r29;
    fn_8020E0E0();
    r3 = r3 & 0xFF;
    fn_8020E488();
    if ((u32)r3 == (u32)0x0) goto L_8020DBCC;
    fn_8020E260();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_8020DBCC;
    r3 = r29;
    r4 = 0x1;
    fn_8020E068();
    fn_801FCCC4();
    if ((u32)r3 == (u32)0x0) goto L_8020DBCC;
    fn_801FCC7C();
    r28 = r3;
    fn_80261388();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_8020DBC0;
    fn_80261444();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_8020DBC0;
    fn_80190528();
L_8020DBC0: ;
    r4 = r28;
    r3 = 0x59;
    fn_80132A38();
L_8020DBCC: ;
    r3 = 0x1;
    r4 = 0x3e8;
    r5 = 0xff;
    fn_80165A20();
    r3 = 0x3e8;
    fn_801657D0();
    r3 = r29;
    fn_8020DFF0();
    fn_8020DEB0();
    r28 = r3;
    fn_8020DE80();
    r30 = r3;
    r3 = r28;
    fn_8020DE98();
    r31 = r3;
    r3 = r28;
    fn_8020DE68();
    f2 = f1;
    f1 = *(f32*)lbl_8047E528;
    r4 = r31;
    r5 = r30;
    r3 = 0x9;
    fn_801C4164();
    r3 = r28;
    fn_8020DE50();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_8020DC44;
    r4 = 0x0;
    r5 = 0x0;
    fn_80166AB8();
L_8020DC44: ;
    fn_801EF7B4();
    r3 = r27;
    fn_800FF730();
    r3 = 0x0;
    r4 = 0x0;
    fn_8011288C();
    fn_800F0308();
    r3 = r27;
    fn_8011395C();
    r3 = r29;
    fn_8020DFD8();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_8020DCF0;
    fn_801EF634();
    r4 = r3;
    r3 = 0x0;
    fn_801F1DBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8020DCF0;
    r3 = 0x0;
    fn_801EF61C();
    r3 = 0xe05;
    fn_801903B0();
    r3 = 0x0;
    r4 = 0xc;
    r5 = 0x0;
    fn_8012A5B0();
    r0 = r3;
    r3 = 0x0;
    r0 = (s32)r0 >> 1;
    /* addze r4, r0 */;
    fn_80129474();
    r3 = 0x1;
    fn_801D0AFC();
    fn_8018DA88();
    fn_80113FE8();
    r4 = (0x596 << 16);
    r3 = 0x0;
    r4 = r4 + 0x8;
    fn_8011288C();
    fn_800F0308();
    fn_801EF634();
    goto L_8020DD30;
L_8020DCF0: ;
    r3 = 0x9b0;
    fn_80190528();
    fn_80112700();
    fn_801140C8();
    r3 = 0xe05;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_8020DD24;
    f1 = *(f32*)lbl_8047E52C;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
L_8020DD24: ;
    r3 = 0x0;
    fn_801EF61C();
    fn_801EF634();
L_8020DD30: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020DD44 | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DD44(void) {
    extern void fn_801F54A4();
    extern void fn_8020E0B0();
    extern void fn_8020E0F8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    fn_8020E0F8();
    fn_8020E0B0();
    r0 = r3;
    r3 = 0x0;
    r4 = r0;
    r5 = 0x7;
    r6 = 0x0;
    fn_801F54A4();
    return;
}
#pragma pop

/* 0x8020DD80 | size: 0xD0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DD80(void) {
    extern void fn_801F54A4();
    extern void fn_801FB1C0();
    extern void fn_801FBD10();
    extern void fn_801FBD58();
    extern void fn_8020E008();
    extern void fn_8020E068();
    extern void fn_8020E0B0();
    extern void fn_8020E0F8();
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
    fn_8020E0F8();
    r31 = r3;
    fn_8020E008();
    if ((u32)r3 == (u32)0x0) goto L_8020DDAC;
    goto L_8020DE3C;
L_8020DDAC: ;
    r3 = r30;
    fn_8020E0F8();
    fn_8020E0B0();
    r0 = r3;
    r3 = 0x0;
    r4 = r0;
    r5 = 0x6;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r3 == (u32)0x0) goto L_8020DDDC;
    goto L_8020DE3C;
L_8020DDDC: ;
    r30 = 0x0;
    goto L_8020DE2C;
L_8020DDE4: ;
    r3 = r31;
    r4 = r30 & 0xFF;
    fn_8020E068();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_8020DE28;
    r4 = r3;
    r3 = 0x0;
    r5 = 0x4;
    r6 = 0x0;
    fn_801FB1C0();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_8020DE28;
    fn_801FBD58();
    fn_801FBD10();
    if ((u32)r3 == (u32)0x0) goto L_8020DE28;
    goto L_8020DE3C;
L_8020DE28: ;
    r30 = r30 + 0x1;
L_8020DE2C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_8020DDE4;
    r3 = 0x1;
L_8020DE3C: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020DE68 | size: 0x18 | tiny */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DE68(void) {
    extern u8 lbl_8047E530[];
    u32 r0 = 0;
    u32 r3 = 0;
    f32 f1 = 0.0f;

    if ((u32)r3 != (u32)0x0) goto L_8020DE78;
    f1 = *(f32*)lbl_8047E530;
    return;
L_8020DE78: ;
    f1 = *(f32*)((u8*)r3 + 0x4);
    return;
}
#pragma pop

/* 0x8020DEB0 | size: 0x28 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DEB0(void) {
    extern u8 lbl_80375980[];
    extern u8 lbl_80478D20[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u32*)lbl_80478D20;
    if ((u32)r3 < (u32)r0) goto L_8020DEC4;
    r3 = 0x0;
    return;
L_8020DEC4: ;
    r4 = r3 * 0xc;
    r3 = (u32)lbl_80375980;
    r0 = (u32)lbl_80375980;
    r3 = r0 + r4;
    return;
}
#pragma pop

/* 0x8020DF10 | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DF10(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8020DF20;
    r4 = 0x0;
    goto L_8020DF40;
L_8020DF20: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020DF34;
    r4 = 0x0;
    goto L_8020DF40;
L_8020DF34: ;
    /* clrlslwi r4, r4, 24, 3 */;
    r4 = r4 + 0x18;
    r4 = r3 + r4;
L_8020DF40: ;
    if ((u32)r4 == (u32)0x0) return;
    *(u32*)((u8*)r4 + 0x4) = r5;
    return;
}
#pragma pop

/* 0x8020DF50 | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020DF50(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8020DF60;
    r4 = 0x0;
    goto L_8020DF80;
L_8020DF60: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020DF74;
    r4 = 0x0;
    goto L_8020DF80;
L_8020DF74: ;
    /* clrlslwi r4, r4, 24, 3 */;
    r4 = r4 + 0x18;
    r4 = r3 + r4;
L_8020DF80: ;
    if ((u32)r4 == (u32)0x0) return;
    *(u16*)((u8*)r4 + 0x0) = r5;
    return;
}
#pragma pop

/* 0x8020E020 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E020(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8020E030;
    r4 = 0x0;
    goto L_8020E050;
L_8020E030: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020E044;
    r4 = 0x0;
    goto L_8020E050;
L_8020E044: ;
    /* clrlslwi r4, r4, 24, 3 */;
    r4 = r4 + 0x18;
    r4 = r3 + r4;
L_8020E050: ;
    if ((u32)r4 != (u32)0x0) goto L_8020E060;
    r3 = 0x0;
    return;
L_8020E060: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    return;
}
#pragma pop

/* 0x8020E068 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E068(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8020E078;
    r4 = 0x0;
    goto L_8020E098;
L_8020E078: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020E08C;
    r4 = 0x0;
    goto L_8020E098;
L_8020E08C: ;
    /* clrlslwi r4, r4, 24, 3 */;
    r4 = r4 + 0x18;
    r4 = r3 + r4;
L_8020E098: ;
    if ((u32)r4 != (u32)0x0) goto L_8020E0A8;
    r3 = 0x0;
    return;
L_8020E0A8: ;
    r3 = *(u16*)((u8*)r4 + 0x0);
    return;
}
#pragma pop

/* 0x8020E0F8 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E0F8(void) {
    extern u8 lbl_80478F50[];
    extern u8 lbl_80478F54[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = *(u32*)lbl_80478F50;
    r3 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r3 < (u32)r0) goto L_8020E114;
    r3 = 0x0;
    return;
L_8020E114: ;
    r0 = r3 * 0x38;
    r3 = *(u32*)lbl_80478F54;
    r3 = r3 + r0;
    return;
}
#pragma pop

/* 0x8020E124 | size: 0x80 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E124(void) {
    extern u8 lbl_80478F00[];
    extern u8 lbl_80478F04[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r4 = *(u32*)lbl_80478F00;
    r7 = r3 & 0xFFFF;
    r6 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r7 <= (u32)r6) goto L_8020E140;
    r4 = 0x0;
    goto L_8020E14C;
L_8020E140: ;
    r4 = *(u32*)lbl_80478F04;
    /* clrlslwi r0, r3, 16, 3 */;
    r4 = r4 + r0;
L_8020E14C: ;
    if ((u32)r4 != (u32)0x0) goto L_8020E15C;
    r5 = 0x0;
    goto L_8020E160;
L_8020E15C: ;
    r5 = *(u8*)((u8*)r4 + 0x0);
L_8020E160: ;
    if ((u32)r7 <= (u32)r6) goto L_8020E170;
    r4 = 0x0;
    goto L_8020E17C;
L_8020E170: ;
    r4 = *(u32*)lbl_80478F04;
    /* clrlslwi r0, r3, 16, 3 */;
    r4 = r4 + r0;
L_8020E17C: ;
    r3 = r5 & 0xFF;
    if ((u32)r4 != (u32)0x0) goto L_8020E190;
    r0 = 0x0;
    goto L_8020E194;
L_8020E190: ;
    r0 = *(u8*)((u8*)r4 + 0x2);
L_8020E194: ;
    r0 = r0 & 0xFF;
    r0 = r3 * r0;
    r3 = r0 & 0xFFFF;
    return;
}
#pragma pop

/* 0x8020E204 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E204(void) {
    extern u8 lbl_80478F00[];
    extern u8 lbl_80478F04[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = *(u32*)lbl_80478F00;
    r5 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r5 <= (u32)r0) goto L_8020E220;
    r3 = 0x0;
    return;
L_8020E220: ;
    r4 = *(u32*)lbl_80478F04;
    /* clrlslwi r0, r3, 16, 3 */;
    r3 = r4 + r0;
    return;
}
#pragma pop

/* 0x8020E488 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E488(void) {
    extern u8 lbl_80478F40[];
    extern u8 lbl_80478F44[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r4 = *(u32*)lbl_80478F40;
    r5 = r3 & 0xFFFF;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r5 <= (u32)r0) goto L_8020E4A4;
    r3 = 0x0;
    return;
L_8020E4A4: ;
    r4 = *(u32*)lbl_80478F44;
    /* clrlslwi r0, r3, 16, 5 */;
    r3 = r4 + r0;
    return;
}
#pragma pop

/* 0x8020E4CC | size: 0x1C | tiny */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E4CC(void) {
    u32 r3 = 0;

    if ((s32)r3 >= (s32)0x0) goto L_8020E4D8;
    r3 = 0x0;
L_8020E4D8: ;
    if ((s32)r3 <= (s32)0xc) return;
    r3 = 0xc;
    return;
}
#pragma pop

/* 0x8020E4E8 | size: 0x94 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E4E8(void) {
    extern u8 lbl_80375D10[];
    extern u8 lbl_80478D68[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r7 = *(u32*)lbl_80478D68;
    r8 = r3 & 0xFFFF;
    r5 = (u32)lbl_80375D10;
    /* clrlslwi r6, r3, 16, 1 */;
    r0 = (u32)lbl_80375D10;
    r5 = r0 + r6;
    if ((u32)r8 < (u32)r7) goto L_8020E50C;
    r5 = 0x0;
L_8020E50C: ;
    if ((u32)r5 != (u32)0x0) goto L_8020E51C;
    r6 = 0x0;
    goto L_8020E52C;
L_8020E51C: ;
    if ((u32)r5 != (u32)0x0) goto L_8020E528;
    r6 = 0x0;
    goto L_8020E52C;
L_8020E528: ;
    r6 = *(u8*)((u8*)r5 + 0x0);
L_8020E52C: ;
    r5 = (u32)lbl_80375D10;
    /* clrlslwi r3, r3, 16, 1 */;
    r0 = (u32)lbl_80375D10;
    r5 = r0 + r3;
    if ((u32)r8 < (u32)r7) goto L_8020E548;
    r5 = 0x0;
L_8020E548: ;
    r0 = r6 & 0xFF;
    r3 = r4 * r0;
    if ((u32)r5 != (u32)0x0) goto L_8020E560;
    r0 = 0x1;
    goto L_8020E570;
L_8020E560: ;
    if ((u32)r5 != (u32)0x0) goto L_8020E56C;
    r0 = 0x1;
    goto L_8020E570;
L_8020E56C: ;
    r0 = *(u8*)((u8*)r5 + 0x1);
L_8020E570: ;
    r0 = r0 & 0xFF;
    r3 = (u32)r3 / (u32)r0;
    return;
}
#pragma pop

/* 0x8020E57C | size: 0x98 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E57C(void) {
    extern void fn_801FD104();
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
    /* mr. r27, r3 */;
    r28 = r5;
    if ((s32)r0 != (s32)0) goto L_8020E5A0;
    r3 = 0x0;
    goto L_8020E600;
L_8020E5A0: ;
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    goto L_8020E5F0;
L_8020E5AC: ;
    r0 = r29 & 0xFFFF;
    r0 = r0 * 0xc;
    r30 = r27 + r0;
    r3 = r30;
    fn_801FD104();
    r3 = -r3;
    /* subic r0, r3, 0x1 */;
    r0 = r3 - r0; /* -borrow */;
    r0 = r0 & 0xFF;
    if ((s32)r0 == (s32)0) goto L_8020E5EC;
    r3 = r30;
    fn_801FD104();
    if ((u32)r3 != (u32)r28) goto L_8020E5EC;
    r3 = r30;
    goto L_8020E600;
L_8020E5EC: ;
    r29 = r29 + 0x1;
L_8020E5F0: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020E5AC;
    r3 = 0x0;
L_8020E600: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020E614 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E614(void) {
    extern void fn_801FD104();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    fn_801FD104();
    r3 = -r3;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;
    return;
}
#pragma pop

/* 0x8020E640 | size: 0x94 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E640(void) {
    extern void fn_801FD07C();
    extern void fn_801FD08C();
    extern void fn_801FD09C();
    extern void fn_801FD0AC();
    extern void fn_80205B8C();
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
    /* mr. r30, r3 */;
    r31 = r4;
    if ((s32)r0 == (s32)0) goto L_8020E6C0;
    if ((u32)r31 == (u32)0x0) goto L_8020E6C0;
    r4 = 0x0;
    fn_801FD0AC();
    r3 = r30;
    r4 = 0x0;
    fn_801FD09C();
    r3 = r30;
    r4 = 0x0;
    fn_801FD08C();
    r3 = r30;
    r4 = 0x0;
    fn_801FD07C();
    r3 = r30;
    r4 = r31;
    fn_801FD0AC();
    r3 = r31;
    fn_80205B8C();
    r4 = 0x0;
    r5 = 0x83;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFFFF;
    r3 = r30;
    fn_801FD08C();
L_8020E6C0: ;
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020E6D4 | size: 0x84 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E6D4(void) {
    extern void fn_801FD07C();
    extern void fn_801FD08C();
    extern void fn_801FD09C();
    extern void fn_801FD0AC();
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
    if ((s32)r0 == (s32)0) goto L_8020E744;
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    goto L_8020E738;
L_8020E6F8: ;
    r0 = r29 & 0xFFFF;
    r4 = 0x0;
    r0 = r0 * 0xc;
    r30 = r28 + r0;
    r3 = r30;
    fn_801FD0AC();
    r3 = r30;
    r4 = 0x0;
    fn_801FD09C();
    r3 = r30;
    r4 = 0x0;
    fn_801FD08C();
    r3 = r30;
    r4 = 0x0;
    fn_801FD07C();
    r29 = r29 + 0x1;
L_8020E738: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020E6F8;
L_8020E744: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020E758 | size: 0x54 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E758(void) {
    extern void fn_801FD07C();
    extern void fn_801FD08C();
    extern void fn_801FD09C();
    extern void fn_801FD0AC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r31 = r3;
    fn_801FD0AC();
    r3 = r31;
    r4 = 0x0;
    fn_801FD09C();
    r3 = r31;
    r4 = 0x0;
    fn_801FD08C();
    r3 = r31;
    r4 = 0x0;
    fn_801FD07C();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020E7AC | size: 0x1B0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E7AC(void) {
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    extern void fn_801FBDD4();
    extern void fn_801FBDE4();
    extern void fn_801FBDF4();
    extern void fn_801FBE18();
    extern void fn_801FBF04();
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
    /* mr. r27, r3 */;
    r28 = r5;
    if ((s32)r0 != (s32)0) goto L_8020E7D0;
    r3 = 0x0;
    goto L_8020E948;
L_8020E7D0: ;
    r0 = (s16)r28;
    if ((s32)r0 >= (s32)0) goto L_8020E7E0;
    r3 = 0x0;
    goto L_8020E948;
L_8020E7E0: ;
    if ((u32)r27 != (u32)0x0) goto L_8020E7F0;
    r29 = 0x0;
    goto L_8020E8AC;
L_8020E7F0: ;
    r31 = r4 & 0xFFFF;
    r30 = 0x0;
    goto L_8020E89C;
L_8020E7FC: ;
    r3 = r30 & 0xFFFF;
    r0 = (s16)r28;
    r0 = r3 * 0x14;
    r29 = r27 + r0;
    if ((u32)r27 >= (u32)0x0) goto L_8020E848;
    if ((u32)r29 != (u32)0x0) goto L_8020E820;
    r0 = 0x0;
    goto L_8020E83C;
L_8020E820: ;
    r3 = r29;
    fn_801FBF04();
    r0 = (s16)r3;
    if ((u32)r29 >= (u32)0x0) goto L_8020E838;
    r0 = 0x0;
    goto L_8020E83C;
L_8020E838: ;
    r0 = 0x1;
L_8020E83C: ;
    r0 = r0 & 0xFF;
    if ((u32)r29 != (u32)0x0) goto L_8020E898;
    goto L_8020E8AC;
L_8020E848: ;
    if ((u32)r29 != (u32)0x0) goto L_8020E858;
    r0 = 0x0;
    goto L_8020E874;
L_8020E858: ;
    r3 = r29;
    fn_801FBF04();
    r0 = (s16)r3;
    if ((u32)r29 >= (u32)0x0) goto L_8020E870;
    r0 = 0x0;
    goto L_8020E874;
L_8020E870: ;
    r0 = 0x1;
L_8020E874: ;
    r0 = r0 & 0xFF;
    if ((u32)r29 == (u32)0x0) goto L_8020E898;
    r3 = r29;
    fn_801FBF04();
    r3 = (s16)r3;
    r0 = (s16)r28;
    if ((s32)r0 != (s32)r3) goto L_8020E898;
    goto L_8020E8AC;
L_8020E898: ;
    r30 = r30 + 0x1;
L_8020E89C: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020E7FC;
    r29 = 0x0;
L_8020E8AC: ;
    if ((u32)r29 != (u32)0x0) goto L_8020E8BC;
    r3 = 0x0;
    goto L_8020E948;
L_8020E8BC: ;
    r3 = r29;
    r4 = -0x1;
    fn_801FBE18();
    r30 = 0x0;
    goto L_8020E8E4;
L_8020E8D0: ;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    fn_801FBDF4();
    r30 = r30 + 0x1;
L_8020E8E4: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020E8D0;
    r3 = r29;
    r4 = 0x0;
    fn_801FBDE4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDD4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDC4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDB4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDA4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBD94();
    r3 = r29;
    r4 = 0x0;
    fn_801FBD84();
    r3 = 0x1;
L_8020E948: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020E95C | size: 0x24C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020E95C(void) {
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    extern void fn_801FBDD4();
    extern void fn_801FBDE4();
    extern void fn_801FBDF4();
    extern void fn_801FBE18();
    extern void fn_801FBF04();
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
    /* mr. r29, r3 */;
    r30 = r4;
    r31 = r5;
    if ((s32)r0 != (s32)0) goto L_8020E984;
    r3 = 0x0;
    goto L_8020EB94;
L_8020E984: ;
    r0 = (s16)r31;
    if ((s32)r0 >= (s32)0) goto L_8020E994;
    r3 = 0x0;
    goto L_8020EB94;
L_8020E994: ;
    if ((u32)r29 != (u32)0x0) goto L_8020E9A4;
    r26 = 0x0;
    goto L_8020EA60;
L_8020E9A4: ;
    r28 = r30 & 0xFFFF;
    r27 = 0x0;
    goto L_8020EA50;
L_8020E9B0: ;
    r3 = r27 & 0xFFFF;
    r0 = (s16)r31;
    r0 = r3 * 0x14;
    r26 = r29 + r0;
    if ((u32)r29 >= (u32)0x0) goto L_8020E9FC;
    if ((u32)r26 != (u32)0x0) goto L_8020E9D4;
    r0 = 0x0;
    goto L_8020E9F0;
L_8020E9D4: ;
    r3 = r26;
    fn_801FBF04();
    r0 = (s16)r3;
    if ((u32)r26 >= (u32)0x0) goto L_8020E9EC;
    r0 = 0x0;
    goto L_8020E9F0;
L_8020E9EC: ;
    r0 = 0x1;
L_8020E9F0: ;
    r0 = r0 & 0xFF;
    if ((u32)r26 != (u32)0x0) goto L_8020EA4C;
    goto L_8020EA60;
L_8020E9FC: ;
    if ((u32)r26 != (u32)0x0) goto L_8020EA0C;
    r0 = 0x0;
    goto L_8020EA28;
L_8020EA0C: ;
    r3 = r26;
    fn_801FBF04();
    r0 = (s16)r3;
    if ((u32)r26 >= (u32)0x0) goto L_8020EA24;
    r0 = 0x0;
    goto L_8020EA28;
L_8020EA24: ;
    r0 = 0x1;
L_8020EA28: ;
    r0 = r0 & 0xFF;
    if ((u32)r26 == (u32)0x0) goto L_8020EA4C;
    r3 = r26;
    fn_801FBF04();
    r3 = (s16)r3;
    r0 = (s16)r31;
    if ((s32)r0 != (s32)r3) goto L_8020EA4C;
    goto L_8020EA60;
L_8020EA4C: ;
    r27 = r27 + 0x1;
L_8020EA50: ;
    r0 = r27 & 0xFFFF;
    if ((u32)r0 < (u32)r28) goto L_8020E9B0;
    r26 = 0x0;
L_8020EA60: ;
    if ((u32)r26 == (u32)0x0) goto L_8020EA70;
    r3 = 0x0;
    goto L_8020EB94;
L_8020EA70: ;
    if ((u32)r29 != (u32)0x0) goto L_8020EA80;
    r27 = 0x0;
    goto L_8020EAE0;
L_8020EA80: ;
    r30 = r30 & 0xFFFF;
    r26 = 0x0;
    goto L_8020EAD0;
L_8020EA8C: ;
    r0 = r26 & 0xFFFF;
    r0 = r0 * 0x14;
    /* add. r27, r29, r0 */;
    if ((u32)r29 != (u32)0x0) goto L_8020EAA4;
    r0 = 0x0;
    goto L_8020EAC0;
L_8020EAA4: ;
    r3 = r27;
    fn_801FBF04();
    r0 = (s16)r3;
    if ((u32)r29 >= (u32)0x0) goto L_8020EABC;
    r0 = 0x0;
    goto L_8020EAC0;
L_8020EABC: ;
    r0 = 0x1;
L_8020EAC0: ;
    r0 = r0 & 0xFF;
    if ((u32)r29 != (u32)0x0) goto L_8020EACC;
    goto L_8020EAE0;
L_8020EACC: ;
    r26 = r26 + 0x1;
L_8020EAD0: ;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_8020EA8C;
    r27 = 0x0;
L_8020EAE0: ;
    if ((u32)r27 != (u32)0x0) goto L_8020EAF0;
    r3 = 0x0;
    goto L_8020EB94;
L_8020EAF0: ;
    if ((u32)r27 == (u32)0x0) goto L_8020EB90;
    r0 = (s16)r31;
    if ((u32)r27 < (u32)0x0) goto L_8020EB90;
    r3 = r27;
    r4 = -0x1;
    fn_801FBE18();
    r26 = 0x0;
    goto L_8020EB24;
L_8020EB10: ;
    r3 = r27;
    r4 = r26;
    r5 = 0x0;
    fn_801FBDF4();
    r26 = r26 + 0x1;
L_8020EB24: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020EB10;
    r3 = r27;
    r4 = 0x0;
    fn_801FBDE4();
    r3 = r27;
    r4 = 0x0;
    fn_801FBDD4();
    r3 = r27;
    r4 = 0x0;
    fn_801FBDC4();
    r3 = r27;
    r4 = 0x0;
    fn_801FBDB4();
    r3 = r27;
    r4 = 0x0;
    fn_801FBDA4();
    r3 = r27;
    r4 = 0x0;
    fn_801FBD94();
    r3 = r27;
    r4 = 0x0;
    fn_801FBD84();
    r3 = r27;
    r4 = r31;
    fn_801FBE18();
L_8020EB90: ;
    r3 = 0x1;
L_8020EB94: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020EBA8 | size: 0xFC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020EBA8(void) {
    extern void fn_801FBF04();
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
    /* mr. r27, r3 */;
    r28 = r5;
    if ((s32)r0 != (s32)0) goto L_8020EBCC;
    r3 = 0x0;
    goto L_8020EC90;
L_8020EBCC: ;
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    goto L_8020EC80;
L_8020EBD8: ;
    r3 = r29 & 0xFFFF;
    r0 = (s16)r28;
    r0 = r3 * 0x14;
    r30 = r27 + r0;
    if ((s32)r0 >= (s32)0) goto L_8020EC28;
    if ((u32)r30 != (u32)0x0) goto L_8020EBFC;
    r0 = 0x0;
    goto L_8020EC18;
L_8020EBFC: ;
    r3 = r30;
    fn_801FBF04();
    r0 = (s16)r3;
    if ((u32)r30 >= (u32)0x0) goto L_8020EC14;
    r0 = 0x0;
    goto L_8020EC18;
L_8020EC14: ;
    r0 = 0x1;
L_8020EC18: ;
    r0 = r0 & 0xFF;
    if ((u32)r30 != (u32)0x0) goto L_8020EC7C;
    r3 = r30;
    goto L_8020EC90;
L_8020EC28: ;
    if ((u32)r30 != (u32)0x0) goto L_8020EC38;
    r0 = 0x0;
    goto L_8020EC54;
L_8020EC38: ;
    r3 = r30;
    fn_801FBF04();
    r0 = (s16)r3;
    if ((u32)r30 >= (u32)0x0) goto L_8020EC50;
    r0 = 0x0;
    goto L_8020EC54;
L_8020EC50: ;
    r0 = 0x1;
L_8020EC54: ;
    r0 = r0 & 0xFF;
    if ((u32)r30 == (u32)0x0) goto L_8020EC7C;
    r3 = r30;
    fn_801FBF04();
    r3 = (s16)r3;
    r0 = (s16)r28;
    if ((s32)r0 != (s32)r3) goto L_8020EC7C;
    r3 = r30;
    goto L_8020EC90;
L_8020EC7C: ;
    r29 = r29 + 0x1;
L_8020EC80: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020EBD8;
    r3 = 0x0;
L_8020EC90: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020ECA4 | size: 0x3C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020ECA4(void) {
    extern void fn_801FBF04();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    if ((u32)r3 != (u32)0x0) goto L_8020ECC0;
    r3 = 0x0;
    goto L_8020ECD0;
L_8020ECC0: ;
    fn_801FBF04();
    r0 = (s16)r3;
    r0 = (u32)r0 >> 31;
    r3 = r0 ^ 0x1;
L_8020ECD0: ;
    return;
}
#pragma pop

/* 0x8020ECE0 | size: 0xDC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020ECE0(void) {
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    extern void fn_801FBDD4();
    extern void fn_801FBDE4();
    extern void fn_801FBDF4();
    extern void fn_801FBE18();
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
    /* mr. r27, r3 */;
    if ((s32)r0 == (s32)0) goto L_8020EDA8;
    r31 = r4 & 0xFFFF;
    r28 = 0x0;
    goto L_8020ED9C;
L_8020ED04: ;
    r0 = r28 & 0xFFFF;
    r4 = -0x1;
    r0 = r0 * 0x14;
    r29 = r27 + r0;
    r3 = r29;
    fn_801FBE18();
    r30 = 0x0;
    goto L_8020ED38;
L_8020ED24: ;
    r3 = r29;
    r4 = r30;
    r5 = 0x0;
    fn_801FBDF4();
    r30 = r30 + 0x1;
L_8020ED38: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 < (u32)0x4) goto L_8020ED24;
    r3 = r29;
    r4 = 0x0;
    fn_801FBDE4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDD4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDC4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDB4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBDA4();
    r3 = r29;
    r4 = 0x0;
    fn_801FBD94();
    r3 = r29;
    r4 = 0x0;
    fn_801FBD84();
    r28 = r28 + 0x1;
L_8020ED9C: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_8020ED04;
L_8020EDA8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020EDBC | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020EDBC(void) {
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r31 = r3;
    fn_801FBDC4();
    r3 = r31;
    r4 = 0x0;
    fn_801FBDB4();
    r3 = r31;
    r4 = 0x0;
    fn_801FBDA4();
    r3 = r31;
    r4 = 0x0;
    fn_801FBD94();
    r3 = r31;
    r4 = 0x0;
    fn_801FBD84();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8020EE1C | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020EE1C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = r7;
    r5 = 0x1;
    r3 = r29;
    r4 = r28;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020EEC0 | size: 0x14 | tiny */
void fn_8020EEC0(void) { }

/* 0x8020EED4 | size: 0x22C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020EED4(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_802062FC();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r27, 0xc(r1) */;
    r31 = r3;
    r27 = r4;
    r29 = r7;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r4 = r27;
    r5 = r30;
    r3 = 0xf;
    fn_801F02AC();
    r28 = r3;
    r4 = r27;
    r5 = r30;
    r3 = 0x10;
    fn_801F02AC();
    r0 = r3;
    r3 = r27;
    r30 = r0;
    r4 = r31;
    r6 = r29;
    r5 = 0x1;
    fn_80208750();
    r3 = r28;
    r4 = r31;
    r6 = r29;
    r5 = 0x2;
    fn_80208750();
    r3 = r30;
    r4 = r31;
    r6 = r29;
    r5 = 0x2;
    fn_80208750();
    r3 = r28;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020F008;
    r3 = r30;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020F008;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r27;
    r4 = r31;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r31;
    r5 = 0x2;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r31;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_8020F0EC;
L_8020F008: ;
    r3 = r30;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020F068;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r27;
    r4 = r31;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r31;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_8020F0EC;
L_8020F068: ;
    r3 = r28;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020F0C8;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r27;
    r4 = r31;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r31;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_8020F0EC;
L_8020F0C8: ;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r27;
    r4 = r31;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
L_8020F0EC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020F108 | size: 0x128 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F108(void) {
    extern void fn_8011BEB4();
    extern void fn_801C3430();
    extern void fn_801C3D64();
    extern void fn_801DB100();
    extern void fn_801F453C();
    extern void fn_801FCEC4();
    extern void fn_80207C6C();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x700];
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
    f32 f0 = 0.0f;

    /* stmw r28, 0x6f0(r1) */;
    r28 = r3;
    r29 = r4;
    r3 = 0x0;
    r4 = 0x1;
    fn_801F453C();
    r30 = r3 & 0xFF;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    r4 = r30;
    fn_80207C6C();
    r30 = r3;
    r4 = r29;
    r3 = r1 + 0x8;
    fn_801FCEC4();
    r7 = r30;
    r3 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x0;
    fn_80208750();
    r4 = r28;
    r3 = r1 + 0x8;
    r5 = 0x3;
    r6 = 0x0;
    fn_80208750();
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    r7 = 0x0;
    fn_802085C4();
    r3 = r31;
    r4 = r30;
    fn_801C3D64();
    fn_801C3430();
    r3 = r29;
    r7 = r30;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x0;
    r7 = 0x0;
    fn_802085C4();
    r3 = r31;
    fn_801DB100();
    /* lmw r28, 0x6f0(r1) */;
    return;
}
#pragma pop

/* 0x8020F238 | size: 0x128 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F238(void) {
    extern void fn_8011BEB4();
    extern void fn_801C3430();
    extern void fn_801C3D64();
    extern void fn_801DB100();
    extern void fn_801F453C();
    extern void fn_801FCEC4();
    extern void fn_80207C6C();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x700];
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
    f32 f0 = 0.0f;

    /* stmw r28, 0x6f0(r1) */;
    r28 = r3;
    r29 = r4;
    r3 = 0x0;
    r4 = 0x1;
    fn_801F453C();
    r30 = r3 & 0xFF;
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r29;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    r4 = r30;
    fn_80207C6C();
    r30 = r3;
    r4 = r29;
    r3 = r1 + 0x8;
    fn_801FCEC4();
    r7 = r30;
    r3 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x0;
    fn_80208750();
    r4 = r28;
    r3 = r1 + 0x8;
    r5 = 0x3;
    r6 = 0x1;
    fn_80208750();
    r3 = r29;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x1;
    r7 = 0x0;
    fn_802085C4();
    r3 = r31;
    r4 = r30;
    fn_801C3D64();
    fn_801C3430();
    r3 = r29;
    r7 = r30;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = r29;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x0;
    r7 = 0x0;
    fn_802085C4();
    r3 = r31;
    fn_801DB100();
    /* lmw r28, 0x6f0(r1) */;
    return;
}
#pragma pop

/* 0x8020F368 | size: 0x80 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F368(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802026E4();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = r7;
    /* stmw r29, 0x14(r1) */;
    r31 = r5;
    r29 = r3;
    r30 = r4;
    r3 = r30;
    r5 = 0x1;
    r4 = r29;
    fn_80208750();
    r3 = r31;
    r4 = 0x37;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020F3D4;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r29;
    r5 = 0x1;
    r6 = 0x0;
    fn_802085C4();
L_8020F3D4: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8020F3F0 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F3F0(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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
    r31 = r7;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r6 = r31;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020F494 | size: 0x84 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F494(void) {
    extern void fn_801F453C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0x1;
    fn_801F453C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x2) goto L_8020F4E4;
    if ((s32)r0 >= (s32)0x2) goto L_8020F4CC;
    if ((s32)r0 == (s32)0x0) goto L_8020F4DC;
    if ((s32)r0 >= (s32)0x0) goto L_8020F4F4;
    goto L_8020F504;
L_8020F4CC: ;
    if ((s32)r0 == (s32)0x4) goto L_8020F4EC;
    if ((s32)r0 >= (s32)0x4) goto L_8020F504;
    goto L_8020F4FC;
L_8020F4DC: ;
    r3 = 0x0;
    goto L_8020F508;
L_8020F4E4: ;
    r3 = 0x1;
    goto L_8020F508;
L_8020F4EC: ;
    r3 = 0x2;
    goto L_8020F508;
L_8020F4F4: ;
    r3 = 0x3;
    goto L_8020F508;
L_8020F4FC: ;
    r3 = 0x4;
    goto L_8020F508;
L_8020F504: ;
    r3 = 0x0;
L_8020F508: ;
    return;
}
#pragma pop

/* 0x8020F518 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F518(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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
    r31 = r7;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r6 = r31;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020F5BC | size: 0x8C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F5BC(void) {
    extern void fn_80202360();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r4;
    r4 = 0x2f;
    fn_80202360();
    r0 = (s16)r3;
    if ((s32)r0 == (s32)0x2) goto L_8020F614;
    if ((s32)r0 >= (s32)0x2) goto L_8020F5FC;
    if ((s32)r0 == (s32)0x0) goto L_8020F634;
    if ((s32)r0 >= (s32)0x0) goto L_8020F60C;
    if ((s32)r0 >= (s32)-0x1) goto L_8020F62C;
    goto L_8020F634;
L_8020F5FC: ;
    if ((s32)r0 == (s32)0x4) goto L_8020F624;
    if ((s32)r0 >= (s32)0x4) goto L_8020F634;
    goto L_8020F61C;
L_8020F60C: ;
    r3 = 0x0;
    goto L_8020F638;
L_8020F614: ;
    r3 = 0x1;
    goto L_8020F638;
L_8020F61C: ;
    r3 = 0x2;
    goto L_8020F638;
L_8020F624: ;
    r3 = 0x3;
    goto L_8020F638;
L_8020F62C: ;
    r3 = 0x4;
    goto L_8020F638;
L_8020F634: ;
    r3 = 0x0;
L_8020F638: ;
    return;
}
#pragma pop

/* 0x8020F648 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F648(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = r7;
    r5 = 0x1;
    r3 = r29;
    r4 = r28;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020F6EC | size: 0x60 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F6EC(void) {
    extern u8 lbl_80379F58[];
    u32 r0 = 0;
    u32 r3 = 0;

    r3 = (u32)lbl_80379F58;
    r3 = (u32)lbl_80379F58;
    r3 = r3 + (0x1 << 16);
    r0 = *(u8*)((u8*)r3 + 0x6002);
    if ((s32)r0 == (s32)0x2) goto L_8020F734;
    if ((s32)r0 >= (s32)0x2) goto L_8020F718;
    if ((s32)r0 == (s32)0x0) goto L_8020F724;
    if ((s32)r0 >= (s32)0x0) goto L_8020F72C;
    goto L_8020F744;
L_8020F718: ;
    if ((s32)r0 >= (s32)0x4) goto L_8020F744;
    goto L_8020F73C;
L_8020F724: ;
    r3 = 0x3;
    return;
L_8020F72C: ;
    r3 = 0x0;
    return;
L_8020F734: ;
    r3 = 0x1;
    return;
L_8020F73C: ;
    r3 = 0x2;
    return;
L_8020F744: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x8020F74C | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F74C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r5 = 0x1;
    r6 = r7;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = r31;
    r4 = r30;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r31;
    r4 = r30;
    r5 = 0x1;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020F7B8 | size: 0x114 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F7B8(void) {
    extern void fn_8011BEB4();
    extern void fn_801F00D0();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_802062FC();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r7;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r31 = r3 & 0xFFFF;
    r3 = r29;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    r4 = r31;
    fn_801F00D0();
    r4 = r29;
    r5 = r31;
    r3 = 0xe;
    fn_801F02AC();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    r4 = r28;
    r6 = r30;
    r5 = 0x1;
    fn_80208750();
    r3 = r31;
    r4 = r28;
    r6 = r30;
    r5 = 0x3;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = r31;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8020F8B8;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r31;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
L_8020F8B8: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020F8D4 | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F8D4(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r5 = 0x3;
    r6 = r7;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = r31;
    r4 = r30;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r31;
    r4 = r30;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020F938 | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F938(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r5 = 0x3;
    r6 = r7;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = r31;
    r4 = r30;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r31;
    r4 = r30;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8020F99C | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F99C(void) {
    extern u8 lbl_80379F58[];
    u32 r0 = 0;
    u32 r3 = 0;

    r3 = (u32)lbl_80379F58;
    r3 = (u32)lbl_80379F58;
    r3 = r3 + (0x1 << 16);
    r0 = *(u8*)((u8*)r3 + 0x6002);
    if ((s32)r0 == (s32)0x3) goto L_8020F9CC;
    if ((s32)r0 >= (s32)0x3) goto L_8020F9D4;
    if ((s32)r0 >= (s32)0x1) goto L_8020F9C4;
    goto L_8020F9D4;
L_8020F9C4: ;
    r3 = 0x0;
    return;
L_8020F9CC: ;
    r3 = 0x1;
    return;
L_8020F9D4: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x8020F9DC | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020F9DC(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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
    r31 = r7;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r6 = r31;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020FA80 | size: 0x40 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FA80(void) {
    extern u8 lbl_80379F58[];
    u32 r0 = 0;
    u32 r3 = 0;

    r3 = (u32)lbl_80379F58;
    r3 = (u32)lbl_80379F58;
    r3 = r3 + (0x1 << 16);
    r0 = *(u8*)((u8*)r3 + 0x6002);
    if ((s32)r0 == (s32)0x3) goto L_8020FAB0;
    if ((s32)r0 >= (s32)0x3) goto L_8020FAB8;
    if ((s32)r0 >= (s32)0x1) goto L_8020FAA8;
    goto L_8020FAB8;
L_8020FAA8: ;
    r3 = 0x0;
    return;
L_8020FAB0: ;
    r3 = 0x1;
    return;
L_8020FAB8: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x8020FAC0 | size: 0x70 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FAC0(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r5 = 0x1;
    /* stmw r29, 0x14(r1) */;
    r31 = r6;
    r29 = r3;
    r30 = r4;
    r3 = r30;
    r6 = r7;
    r4 = r29;
    fn_80208750();
    r0 = r31 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_8020FB1C;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r29;
    r5 = 0x1;
    r6 = 0x0;
    fn_802085C4();
L_8020FB1C: ;
    /* lmw r29, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8020FB38 | size: 0xC4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FB38(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r0 = r6 & 0xFFFF;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r7;
    if ((s32)r0 == (s32)0) goto L_8020FB9C;
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_8020FBE8;
L_8020FB9C: ;
    r3 = r29;
    r4 = r28;
    r6 = r31;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x0;
    fn_802085C4();
L_8020FBE8: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020FC04 | size: 0x6C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FC04(void) {
    extern void fn_801F453C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0x1;
    fn_801F453C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_8020FC44;
    if ((s32)r0 >= (s32)0x1) goto L_8020FC38;
    if ((s32)r0 >= (s32)0x0) goto L_8020FC4C;
    goto L_8020FC5C;
L_8020FC38: ;
    if ((s32)r0 >= (s32)0x5) goto L_8020FC5C;
    goto L_8020FC54;
L_8020FC44: ;
    r3 = 0x0;
    goto L_8020FC60;
L_8020FC4C: ;
    r3 = 0x1;
    goto L_8020FC60;
L_8020FC54: ;
    r3 = 0x2;
    goto L_8020FC60;
L_8020FC5C: ;
    r3 = 0x0;
L_8020FC60: ;
    return;
}
#pragma pop

/* 0x8020FC70 | size: 0x11C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FC70(void) {
    extern void fn_8011BEB4();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x4) goto L_8020FCC4;
    if ((u32)r0 == (u32)0x6) goto L_8020FCC4;
    if ((u32)r0 != (u32)0x1) goto L_8020FD08;
L_8020FCC4: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_8020FD08;
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_8020FD78;
L_8020FD08: ;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x0;
    fn_80208750();
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
L_8020FD78: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x8020FD8C | size: 0xB4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FD8C(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = r4;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0x46) goto L_8020FE1C;
    if ((s32)r0 >= (s32)0x46) goto L_8020FDF0;
    if ((s32)r0 == (s32)0x1e) goto L_8020FE14;
    if ((s32)r0 >= (s32)0x1e) goto L_8020FDE4;
    if ((s32)r0 == (s32)0xa) goto L_8020FE14;
    goto L_8020FE2C;
L_8020FDE4: ;
    if ((s32)r0 == (s32)0x32) goto L_8020FE1C;
    goto L_8020FE2C;
L_8020FDF0: ;
    if ((s32)r0 == (s32)0x6e) goto L_8020FE24;
    if ((s32)r0 >= (s32)0x6e) goto L_8020FE08;
    if ((s32)r0 == (s32)0x5a) goto L_8020FE1C;
    goto L_8020FE2C;
L_8020FE08: ;
    if ((s32)r0 == (s32)0x96) goto L_8020FE24;
    goto L_8020FE2C;
L_8020FE14: ;
    r3 = 0x0;
    goto L_8020FE30;
L_8020FE1C: ;
    r3 = 0x1;
    goto L_8020FE30;
L_8020FE24: ;
    r3 = 0x2;
    goto L_8020FE30;
L_8020FE2C: ;
    r3 = 0x0;
L_8020FE30: ;
    return;
}
#pragma pop

/* 0x8020FE40 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FE40(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = r7;
    r5 = 0x1;
    r3 = r29;
    r4 = r28;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8020FEE4 | size: 0x78 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FEE4(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = r4;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r4 = r3 & 0xFFFF;
    if ((u32)r4 <= (u32)0x59) goto L_8020FF28;
    r3 = 0x3;
    goto L_8020FF4C;
L_8020FF28: ;
    if ((u32)r4 < (u32)0x3e) goto L_8020FF38;
    r3 = 0x2;
    goto L_8020FF4C;
L_8020FF38: ;
    r0 = 0x16;
    r3 = -0x1;
    r0 = r4 - r0;
    /* subfze r0, r3 */;
    r3 = r0 & 0xFF;
L_8020FF4C: ;
    return;
}
#pragma pop

/* 0x8020FF5C | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8020FF5C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = r7;
    r5 = 0x1;
    r3 = r29;
    r4 = r28;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210000 | size: 0x74 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210000(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = r4;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0x50) goto L_80210058;
    if ((s32)r0 >= (s32)0x50) goto L_8021004C;
    if ((s32)r0 == (s32)0x28) goto L_80210058;
    goto L_80210060;
L_8021004C: ;
    if ((s32)r0 == (s32)0x78) goto L_80210058;
    goto L_80210060;
L_80210058: ;
    r3 = 0x0;
    goto L_80210064;
L_80210060: ;
    r3 = 0x1;
L_80210064: ;
    return;
}
#pragma pop

/* 0x80210074 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210074(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = r7;
    r5 = 0x1;
    r3 = r29;
    r4 = r28;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210118 | size: 0x78 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210118(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = r4;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 > (u32)0x18) goto L_8021015C;
    r3 = 0x0;
    goto L_80210180;
L_8021015C: ;
    if ((u32)r3 > (u32)0x24) goto L_8021016C;
    r3 = 0x1;
    goto L_80210180;
L_8021016C: ;
    r0 = 0x50;
    r0 = r0 - r3;
    /* addze r0, r3 */;
    r3 = r3 - r0;
    r3 = r3 + 0x3;
L_80210180: ;
    return;
}
#pragma pop

/* 0x80210190 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210190(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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
    r31 = r7;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r6 = r31;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210234 | size: 0x84 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210234(void) {
    extern void fn_80202360();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r4;
    r4 = 0x2e;
    fn_80202360();
    r0 = (s16)r3;
    if ((s32)r0 == (s32)0x3) goto L_8021028C;
    if ((s32)r0 >= (s32)0x3) goto L_8021026C;
    if ((s32)r0 == (s32)0x1) goto L_8021027C;
    if ((s32)r0 >= (s32)0x1) goto L_80210284;
    goto L_802102A4;
L_8021026C: ;
    if ((s32)r0 == (s32)0x5) goto L_8021029C;
    if ((s32)r0 >= (s32)0x5) goto L_802102A4;
    goto L_80210294;
L_8021027C: ;
    r3 = 0x0;
    goto L_802102A8;
L_80210284: ;
    r3 = 0x1;
    goto L_802102A8;
L_8021028C: ;
    r3 = 0x2;
    goto L_802102A8;
L_80210294: ;
    r3 = 0x3;
    goto L_802102A8;
L_8021029C: ;
    r3 = 0x4;
    goto L_802102A8;
L_802102A4: ;
    r3 = 0x0;
L_802102A8: ;
    return;
}
#pragma pop

/* 0x802102B8 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802102B8(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = r7;
    r5 = 0x1;
    r3 = r29;
    r4 = r28;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x8021035C | size: 0x8C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8021035C(void) {
    extern void fn_80202360();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r4;
    r4 = 0x2f;
    fn_80202360();
    r0 = (s16)r3;
    if ((s32)r0 == (s32)0x2) goto L_802103B4;
    if ((s32)r0 >= (s32)0x2) goto L_8021039C;
    if ((s32)r0 == (s32)0x0) goto L_802103D4;
    if ((s32)r0 >= (s32)0x0) goto L_802103AC;
    if ((s32)r0 >= (s32)-0x1) goto L_802103CC;
    goto L_802103D4;
L_8021039C: ;
    if ((s32)r0 == (s32)0x4) goto L_802103C4;
    if ((s32)r0 >= (s32)0x4) goto L_802103D4;
    goto L_802103BC;
L_802103AC: ;
    r3 = 0x0;
    goto L_802103D8;
L_802103B4: ;
    r3 = 0x1;
    goto L_802103D8;
L_802103BC: ;
    r3 = 0x2;
    goto L_802103D8;
L_802103C4: ;
    r3 = 0x3;
    goto L_802103D8;
L_802103CC: ;
    r3 = 0x4;
    goto L_802103D8;
L_802103D4: ;
    r3 = 0x0;
L_802103D8: ;
    return;
}
#pragma pop

/* 0x802103E8 | size: 0x100 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802103E8(void) {
    extern void fn_8011BEB4();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r30 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80210464;
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_802104D4;
L_80210464: ;
    r3 = r28;
    r4 = r27;
    r6 = r31;
    r5 = 0x1;
    fn_80208750();
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
L_802104D4: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x802104E8 | size: 0x84 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802104E8(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = r4;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0x14) goto L_80210548;
    if ((s32)r0 >= (s32)0x14) goto L_80210534;
    if ((s32)r0 == (s32)0xa) goto L_80210540;
    goto L_80210558;
L_80210534: ;
    if ((s32)r0 == (s32)0x1e) goto L_80210550;
    goto L_80210558;
L_80210540: ;
    r3 = 0x0;
    goto L_8021055C;
L_80210548: ;
    r3 = 0x1;
    goto L_8021055C;
L_80210550: ;
    r3 = 0x2;
    goto L_8021055C;
L_80210558: ;
    r3 = 0x0;
L_8021055C: ;
    return;
}
#pragma pop

/* 0x8021056C | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8021056C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r31 = r7;
    r5 = 0x1;
    r3 = r29;
    r4 = r28;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210610 | size: 0x48 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210610(void) {
    extern void fn_80203E0C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r4;
    fn_80203E0C();
    r3 = r3 & 0xFF;
    if ((u32)r3 >= (u32)0x21) goto L_80210638;
    r3 = 0x0;
    goto L_80210648;
L_80210638: ;
    r0 = 0x42;
    r0 = r3 - r0;
    r3 = r0 - r0; /* -borrow */;
    r3 = r3 + 0x2;
L_80210648: ;
    return;
}
#pragma pop

/* 0x80210658 | size: 0xA4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210658(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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
    r31 = r7;
    r30 = r5;
    r28 = r3;
    r29 = r4;
    r6 = r31;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x802106FC | size: 0xC0 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802106FC(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = r4;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0x50) goto L_80210790;
    if ((s32)r0 >= (s32)0x50) goto L_80210760;
    if ((s32)r0 == (s32)0x28) goto L_80210780;
    if ((s32)r0 >= (s32)0x28) goto L_80210754;
    if ((s32)r0 == (s32)0x14) goto L_80210778;
    goto L_802107A8;
L_80210754: ;
    if ((s32)r0 == (s32)0x3c) goto L_80210788;
    goto L_802107A8;
L_80210760: ;
    if ((s32)r0 == (s32)0x78) goto L_802107A0;
    if ((s32)r0 >= (s32)0x78) goto L_802107A8;
    if ((s32)r0 == (s32)0x64) goto L_80210798;
    goto L_802107A8;
L_80210778: ;
    r3 = 0x0;
    goto L_802107AC;
L_80210780: ;
    r3 = 0x1;
    goto L_802107AC;
L_80210788: ;
    r3 = 0x2;
    goto L_802107AC;
L_80210790: ;
    r3 = 0x3;
    goto L_802107AC;
L_80210798: ;
    r3 = 0x4;
    goto L_802107AC;
L_802107A0: ;
    r3 = 0x5;
    goto L_802107AC;
L_802107A8: ;
    r3 = 0x0;
L_802107AC: ;
    return;
}
#pragma pop

/* 0x802107BC | size: 0xC4 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802107BC(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r0 = r6 & 0xFFFF;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r7;
    if ((s32)r0 != (s32)0) goto L_80210834;
    r3 = r29;
    r4 = r28;
    r6 = r31;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x0;
    fn_802085C4();
    goto L_8021086C;
L_80210834: ;
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
L_8021086C: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210888 | size: 0x108 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210888(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_802062FC();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r7;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r5 = r3 & 0xFFFF;
    r4 = r29;
    r3 = 0xe;
    fn_801F02AC();
    r0 = r3;
    r3 = r29;
    r31 = r0;
    r4 = r28;
    r6 = r30;
    r5 = 0x3;
    fn_80208750();
    r3 = r31;
    r4 = r28;
    r6 = r30;
    r5 = 0x3;
    fn_80208750();
    r3 = r31;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80210958;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r31;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
    goto L_8021097C;
L_80210958: ;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
L_8021097C: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210998 | size: 0x168 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210998(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_801F02AC();
    extern void fn_801F54A4();
    extern void fn_802062FC();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x0;
    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r7;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    fn_801F54A4();
    r5 = r3 & 0xFFFF;
    r4 = r28;
    r3 = 0x10;
    fn_801F02AC();
    r0 = r3;
    r3 = r28;
    r31 = r0;
    r4 = r27;
    r6 = r30;
    r5 = 0x1;
    fn_80208750();
    r3 = r29;
    r4 = r27;
    r6 = r30;
    r5 = 0x2;
    fn_80208750();
    r3 = r31;
    r4 = r27;
    r6 = r30;
    r5 = 0x2;
    fn_80208750();
    r3 = r31;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80210AA4;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r31;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_80210AEC;
L_80210AA4: ;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
L_80210AEC: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80210B08 | size: 0xE8 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210B08(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r0 = r6 & 0xFFFF;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r7;
    if ((s32)r0 == (s32)0) goto L_80210B6C;
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_80210BDC;
L_80210B6C: ;
    r3 = r29;
    r4 = r28;
    r6 = r31;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
L_80210BDC: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210BF8 | size: 0x104 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210BF8(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r0 = r6 & 0xFFFF;
    /* stmw r28, 0x10(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r7;
    if ((s32)r0 != (s32)0) goto L_80210CA8;
    r3 = r29;
    r4 = r28;
    r6 = r31;
    r5 = 0x1;
    fn_80208750();
    r3 = r30;
    r4 = r28;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = r29;
    r4 = r28;
    r6 = r31;
    r5 = 0x3;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r30;
    r4 = r28;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_80210CE8;
L_80210CA8: ;
    if ((u32)r0 != (u32)0x1) goto L_80210CE8;
    r3 = r29;
    r4 = r28;
    r6 = r31;
    r5 = 0x3;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r28;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
L_80210CE8: ;
    /* lmw r28, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80210D04 | size: 0x150 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210D04(void) {
    extern void fn_8011BEB4();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    extern void fn_802221EC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r28 = r4;
    r27 = r3;
    r29 = r5;
    r31 = r6;
    r30 = r7;
    r3 = r28;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r31 & 0xFFFF;
    r31 = r3;
    if ((s32)r0 != (s32)0) goto L_80210DE0;
    r3 = r28;
    r4 = r27;
    r6 = r30;
    r5 = 0x1;
    fn_80208750();
    r3 = r29;
    r4 = r27;
    r6 = r30;
    r5 = 0x2;
    fn_80208750();
    r3 = r28;
    r4 = r27;
    r6 = r30;
    r5 = 0x3;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_80210E40;
L_80210DE0: ;
    if ((u32)r0 != (u32)0x1) goto L_80210E40;
    r3 = r28;
    r4 = r27;
    r6 = r30;
    r5 = 0x3;
    fn_80208750();
    if ((s32)r31 <= (s32)0x0) goto L_80210E1C;
    r4 = r28;
    r3 = 0x32;
    r5 = 0x0;
    r6 = 0x1;
    fn_802221EC();
    goto L_80210E40;
L_80210E1C: ;
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
L_80210E40: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80210E5C | size: 0x64 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210E5C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
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

    r5 = 0x3;
    r6 = r7;
    /* stmw r30, 0x8(r1) */;
    r30 = r3;
    r31 = r4;
    r3 = r31;
    r4 = r30;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r31;
    r4 = r30;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
    /* lmw r30, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80210EC8 | size: 0x170 | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80210EC8(void) {
    extern u8 lbl_80379F58[];
    extern void fn_8011BEB4();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r8 = (u32)lbl_80379F58;
    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r3 = (u32)lbl_80379F58;
    r28 = r4;
    r3 = r3 + (0x1 << 16);
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r0 = *(u8*)((u8*)r3 + 0x6002);
    if ((u32)r0 != (u32)0x1) goto L_80210FEC;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x4) goto L_80210F34;
    if ((u32)r0 == (u32)0x6) goto L_80210F34;
    if ((u32)r0 != (u32)0x1) goto L_80210F78;
L_80210F34: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_80210F78;
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_80211024;
L_80210F78: ;
    r3 = r28;
    r4 = r27;
    r6 = r31;
    r5 = 0x1;
    fn_80208750();
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_80211024;
L_80210FEC: ;
    r3 = r28;
    r4 = r27;
    r6 = r31;
    r5 = 0x3;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x3;
    r6 = 0x0;
    fn_802085C4();
L_80211024: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80211040 | size: 0x11C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211040(void) {
    extern void fn_8011BEB4();
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r4 = r27;
    r3 = 0x0;
    r5 = 0x5;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x4) goto L_80211094;
    if ((u32)r0 == (u32)0x6) goto L_80211094;
    if ((u32)r0 != (u32)0x1) goto L_802110D8;
L_80211094: ;
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) goto L_802110D8;
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
    goto L_80211148;
L_802110D8: ;
    r3 = r28;
    r4 = r27;
    r6 = r31;
    r5 = 0x1;
    fn_80208750();
    r3 = r29;
    r4 = r27;
    r6 = r31;
    r5 = 0x2;
    fn_80208750();
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r28;
    r4 = r27;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();
    r3 = 0x12;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r29;
    r4 = r27;
    r5 = 0x2;
    r6 = 0x0;
    fn_802085C4();
L_80211148: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80211164 | size: 0x4 | trivial */
s32 fn_80211164(void) { return 0; }

/* 0x80211170 | size: 0x68C | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211170(void) {
    extern u8 lbl_80478D60[];
    extern u8 lbl_80478D78[];
    extern u8 lbl_8047B610[];
    extern u8 lbl_8047B618[];
    extern void fn_800E0C54();
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    extern void fn_801F025C();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_801FCEC4();
    extern void fn_802026E4();
    extern void fn_80203D3C();
    extern void fn_80203FE4();
    extern void fn_80205184();
    extern void fn_80207BF4();
    extern void fn_802096E8();
    extern void fn_802099AC();
    extern void fn_80209D90();
    extern void fn_8020A068();
    extern void fn_8020A080();
    extern void fn_8020A2B8();
    extern void fn_802271E0();
    extern void fn_802274F0();
    extern void fn_80232110();
    u8 sp[0xED0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
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

    /* stmw r14, 0xe88(r1) */;
    r16 = r3;
    r17 = r4;
    r18 = r5;
    r19 = r6;
    r15 = r7;
    r20 = r8;
    r14 = r9;
    r21 = r10;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3;
    r3 = 0x0;
    *(u32*)(sp + 0xE7C) = r0;
    r4 = 0x0;
    r5 = 0x42;
    r6 = 0x0;
    fn_801F54A4();
    r0 = *(u32*)lbl_8047B610;
    r4 = 0x8;
    r6 = (u32)lbl_80478D78;
    r5 = r1 + 0x8;
    r23 = r3;
    *(u32*)(sp + 0xE80) = r0;
    r22 = *(u32*)lbl_8047B618;
    ctr_fn = (void(*)(void))r4;
L_802111F0: ;
    r0 = *(u8*)((u8*)r6 + 0x0);
    r6 = r6 + 0x1;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto L_802111F0;
    r4 = r18;
    r3 = r1 + 0x79c;
    fn_801FCEC4();
    r4 = r19;
    r3 = r1 + 0xbc;
    fn_801FCEC4();
    r7 = r18;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r7 = r19;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x42;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r18;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3;
    r3 = r1 + 0x10;
    r24 = r0;
    r4 = r24;
    fn_8020A2B8();
    r3 = r24;
    fn_80209D90();
    r3 = r24;
    r5 = r17;
    r4 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    fn_802099AC();
    r4 = 0x0;
    r0 = 0x8;
    r3 = (u32)lbl_80478D78;
    *(u32*)lbl_8047B618 = r4;
    ctr_fn = (void(*)(void))r0;
L_802112A8: ;
    *(u8*)((u8*)r3 + 0x0) = r4;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_802112A8;
    if ((u32)r14 == (u32)0x0) goto L_802112D8;
    r12 = r14;
    r3 = r16;
    r4 = r17;
    r5 = r18;
    r6 = r19;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_802112D8: ;
    r0 = r15 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8021158C;
    r3 = 0x11;
    r4 = 0x0;
    fn_801F025C();
    r25 = r3;
    fn_80207BF4();
    r3 = r25;
    fn_80203FE4();
    r29 = r3;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r27 = r3;
    r3 = r25;
    fn_80205184();
    r28 = r3;
    r3 = r25;
    fn_80203D3C();
    r14 = r3;
    r3 = 0x12;
    r4 = 0x0;
    fn_801F025C();
    r0 = r29 & 0xFFFF;
    r30 = 0x0;
    r26 = r3;
    if ((u32)r0 != (u32)0x3f) goto L_80211364;
    r0 = r14 & 0xFFFF;
    if ((u32)r0 != (u32)0x71) goto L_80211364;
    r30 = 0x1;
L_80211364: ;
    r0 = r29 & 0xFFFF;
    r31 = 0x0;
    if ((u32)r0 != (u32)0x42) goto L_80211384;
    r0 = r14 & 0xFFFF;
    if ((u32)r0 != (u32)0x53) goto L_80211384;
    r31 = 0x1;
L_80211384: ;
    r3 = r25;
    r4 = 0xf;
    fn_802026E4();
    r0 = r3 & 0xFF;
    r4 = r28;
    r0 = 0x1 - r0;
    r3 = 0x0;
    r0 = __cntlzw(r0);
    r5 = 0x9;
    r6 = 0x0;
    r15 = ((r0 << 28) | ((u32)r0 >> 4)) & 0x0FFFFFFE;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    r4 = r28;
    r0 = 0x2b - r0;
    r3 = 0x0;
    r0 = __cntlzw(r0);
    r5 = 0x9;
    r14 = (u32)r0 >> 5;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    r4 = r28;
    r0 = 0x4b - r0;
    r3 = 0x0;
    r0 = __cntlzw(r0);
    r5 = 0x9;
    r0 = (u32)r0 >> 5;
    r6 = 0x0;
    r15 = r15 + r0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    r4 = r28;
    r0 = 0xc8 - r0;
    r3 = 0x0;
    r0 = __cntlzw(r0);
    r5 = 0x9;
    r0 = (u32)r0 >> 5;
    r6 = 0x0;
    r15 = r15 + r0;
    fn_8011BEB4();
    r0 = r3 & 0xFFFF;
    r3 = *(u32*)lbl_80478D60;
    r4 = 0xd1 - r0;
    r0 = r29 & 0xFFFF;
    r6 = __cntlzw(r4);
    r4 = r30 << 1;
    r5 = 0x29 - r0;
    r0 = r31 << 1;
    r6 = (u32)r6 >> 5;
    /* subi r7, r3, 0x1 */;
    r3 = __cntlzw(r5);
    r5 = r15 + r6;
    r3 = (u32)r3 >> 5;
    r3 = r5 + r3;
    r3 = r3 + r4;
    r0 = r3 + r0;
    r0 = r14 + r0;
    r14 = r0 & 0xFFFF;
    if ((u32)r14 <= (u32)r7) goto L_8021147C;
    r14 = r7 & 0xFFFF;
L_8021147C: ;
    r3 = r26;
    fn_80207BF4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x4) goto L_80211568;
    r3 = r26;
    fn_80207BF4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x4b) goto L_80211568;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211568;
    r3 = r14;
    fn_8020A080();
    fn_8020A068();
    r14 = r3 & 0xFF;
    fn_800E0C54();
    r3 = r3 & 0xFFFF;
    r0 = (s32)r3 / (s32)r14;
    r0 = r0 * r14;
    /* subf. r0, r0, r3 */;
    if ((u32)r0 == (u32)0x1) goto L_80211530;
    r3 = r25;
    r4 = 0x3e;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8021154C;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x164) goto L_8021154C;
    fn_800E0C54();
    r4 = r3 & 0xFFFF;
    r3 = 0x64;
    r0 = (s32)r4 / (s32)r3;
    r0 = r0 * r3;
    r0 = r4 - r0;
    if ((s32)r0 >= (s32)0x5a) goto L_8021154C;
L_80211530: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    r7 = 0x2;
    fn_8011BBD8();
    goto L_80211580;
L_8021154C: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
    goto L_80211580;
L_80211568: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    r7 = 0x1;
    fn_8011BBD8();
L_80211580: ;
    r3 = *(u32*)lbl_8047B610;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047B610 = r0;
L_8021158C: ;
    r3 = 0x11;
    r4 = 0x0;
    fn_801F025C();
    r15 = r3;
    r3 = 0x12;
    r4 = 0x0;
    fn_801F025C();
    r29 = r3;
    r3 = 0x2;
    r4 = r29;
    fn_801F025C();
    r28 = r3;
    r3 = r15;
    fn_80205184();
    r27 = r3;
    r3 = r15;
    r4 = 0x0;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x0;
    r14 = r3;
    r5 = 0x2f;
    r6 = 0x0;
    fn_8011BEB4();
    r25 = r3 & 0xFFFF;
    r3 = r14;
    r4 = 0x0;
    r5 = 0x30;
    r6 = 0x0;
    fn_8011BEB4();
    r26 = r3 & 0xFFFF;
    r3 = r15;
    r4 = r29;
    r5 = r28;
    r6 = r27;
    r7 = r25;
    r8 = r26;
    fn_80232110();
    r25 = r3;
    r3 = r14;
    r4 = 0x0;
    r5 = 0x2b;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFF;
    r3 = r14;
    r25 = r25 * r0;
    r4 = 0x0;
    r5 = 0x2c;
    r6 = 0x0;
    fn_8011BEB4();
    r0 = r3 & 0xFF;
    r3 = r15;
    r25 = r25 * r0;
    r4 = 0x24;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211688;
    if ((u32)r26 != (u32)0xd) goto L_80211688;
    r25 = r25 << 1;
L_80211688: ;
    r3 = r15;
    r4 = 0x32;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802116AC;
    r3 = r25 * 0xf;
    r0 = 0xa;
    r25 = (s32)r3 / (s32)r0;
L_802116AC: ;
    r3 = r14;
    r7 = r25;
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    fn_8011BBD8();
    r6 = *(u32*)lbl_8047B610;
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x1;
    r0 = r6 + 0x1;
    r6 = 0x0;
    *(u32*)lbl_8047B610 = r0;
    fn_802274F0();
    r0 = r20 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211708;
    r3 = 0x1;
    r4 = 0x1;
    fn_802271E0();
    r3 = *(u32*)lbl_8047B610;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047B610 = r0;
L_80211708: ;
    if ((u32)r21 == (u32)0x0) goto L_8021172C;
    r12 = r21;
    r3 = r16;
    r4 = r17;
    r5 = r18;
    r6 = r19;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8021172C: ;
    r3 = r24;
    fn_802096E8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211758;
    r3 = r24;
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    fn_8011BEB4();
    goto L_8021175C;
L_80211758: ;
    r3 = 0x0;
L_8021175C: ;
    r7 = *(u32*)(sp + 0xE7C);
    r14 = r3;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x36;
    r6 = 0x0;
    fn_801F4C14();
    r7 = r23;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x42;
    r6 = 0x0;
    fn_801F4C14();
    r3 = r24;
    r4 = r1 + 0x10;
    fn_8020A2B8();
    r0 = *(u32*)(sp + 0xE80);
    r3 = 0x8;
    r4 = (u32)lbl_80478D78;
    r5 = r1 + 0x8;
    *(u32*)lbl_8047B610 = r0;
    *(u32*)lbl_8047B618 = r22;
    ctr_fn = (void(*)(void))r3;
L_802117B8: ;
    r0 = *(u8*)((u8*)r5 + 0x0);
    r5 = r5 + 0x1;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_802117B8;
    r3 = r18;
    r4 = r1 + 0x79c;
    fn_801FCEC4();
    r3 = r19;
    r4 = r1 + 0xbc;
    fn_801FCEC4();
    r3 = r14;
    /* lmw r14, 0xe88(r1) */;
    return;
}
#pragma pop

/* 0x802117FC | size: 0x14 | tiny */
void fn_802117FC(void) { }

/* 0x80211810 | size: 0x20 | tiny */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211810(void) {
    extern u8 lbl_8047B618[];
    u32 r0 = 0;
    u32 r3 = 0;

    r3 = r3 & 0xFF;
    r0 = *(u32*)lbl_8047B618;
    r3 = r0 | 0x80;
    if ((u32)r3 != (u32)0x1) goto L_80211828;
    r3 = r0 & 0xFFFFFF7F;
L_80211828: ;
    *(u32*)lbl_8047B618 = r3;
    return;
}
#pragma pop

/* 0x80211830 | size: 0xCC | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211830(void) {
    extern u8 lbl_80478D78[];
    extern u8 lbl_8047B62C[];
    extern void fn_801F37B0();
    extern void fn_801F47B4();
    extern void fn_801F6EEC();
    extern void fn_802118FC();
    extern void fn_80213558();
    extern void fn_802136A4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = (u32)fn_80213558;
    r6 = 0x0;
    r4 = (u32)fn_80213558;
    r0 = 0x0;
    r5 = r1 + 0x8;
    r3 = 0x0;
    *(u8*)(sp + 0x8) = r0;
    fn_801F37B0();
    r31 = 0x0;
    goto L_80211888;
L_80211868: ;
    r4 = r31;
    r3 = 0x0;
    fn_801F47B4();
    if ((u32)r3 == (u32)0x0) goto L_80211884;
    r4 = 0x4d;
    fn_801F6EEC();
L_80211884: ;
    r31 = r31 + 0x1;
L_80211888: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 < (u32)0x2) goto L_80211868;
    r4 = (u32)fn_802136A4;
    r3 = 0x0;
    r4 = (u32)fn_802136A4;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = 0x8;
    r3 = (u32)lbl_80478D78;
    r4 = 0x0;
    ctr_fn = (void(*)(void))r0;
L_802118BC: ;
    *(u8*)((u8*)r3 + 0x0) = r4;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_802118BC;
    r0 = 0x0;
    r3 = (u32)fn_802118FC;
    *(u32*)lbl_8047B62C = r0;
    r4 = (u32)fn_802118FC;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* 0x802118FC | size: 0x4C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802118FC(void) {
    extern void fn_80202810();
    extern void fn_802062FC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80211924;
    r3 = 0x1;
    goto L_80211934;
L_80211924: ;
    r3 = r31;
    r4 = 0x11;
    fn_80202810();
    r3 = 0x1;
L_80211934: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80211948 | size: 0x8C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211948(void) {
    extern void fn_801F0F04();
    extern void fn_801F11CC();
    extern void fn_8020D888();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r31 = 0;

    r11 = r4;
    r10 = r5;
    r9 = r6;
    r0 = r7;
    r4 = r3;
    r31 = r8;
    r5 = r11;
    r6 = r10;
    r7 = r9;
    r8 = r0;
    r3 = r1 + 0x8;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80211998;
    goto L_802119A8;
L_80211998: ;
    r4 = r31;
    r3 = r1 + 0x8;
    fn_8020D888();
    r3 = 0x1;
L_802119A8: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802119B8;
    goto L_802119C0;
L_802119B8: ;
    r3 = r1 + 0x8;
    fn_801F0F04();
L_802119C0: ;
    r31 = *(u32*)(sp + 0x3C);
    return;
}
#pragma pop

/* 0x802119D4 | size: 0x2C | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_802119D4(void) {
    extern u8 lbl_8047B618[];
    extern void fn_80213270();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;

    r0 = *(u32*)lbl_8047B618;
    r0 = r0 & 0xFFEFFFFF;
    *(u32*)lbl_8047B618 = r0;
    fn_80213270();
    return;
}
#pragma pop

/* 0x80211A00 | size: 0x78 | small */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211A00(void) {
    extern u8 lbl_80379F58[];
    extern u8 lbl_80478D78[];
    extern u8 lbl_8047B625[];
    extern void fn_801DA7AC();
    extern void fn_801F37B0();
    extern void fn_80211A78();
    extern void fn_8022FE20();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = (u32)fn_80211A78;
    r3 = 0x0;
    r4 = (u32)fn_80211A78;
    r5 = 0x0;
    r6 = 0x1;
    fn_801F37B0();
    r4 = (u32)fn_8022FE20;
    r3 = 0x0;
    r4 = (u32)fn_8022FE20;
    r5 = 0x0;
    r6 = 0x1;
    fn_801F37B0();
    r3 = (u32)lbl_80379F58;
    r0 = 0x0;
    r3 = (u32)lbl_80379F58;
    r4 = (u32)lbl_80478D78;
    r3 = r3 + (0x1 << 16);
    *(u8*)((u8*)r4 + 0x3) = r0;
    *(u8*)lbl_8047B625 = r0;
    *(u8*)((u8*)r4 + 0x4) = r0;
    *(u8*)((u8*)r3 + 0x6002) = r0;
    *(u8*)((u8*)r3 + 0x60A1) = r0;
    fn_801DA7AC();
    return;
}
#pragma pop

/* 0x80211A78 | size: 0x11C | medium */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211A78(void) {
    extern u8 lbl_80375D30[];
    extern u8 lbl_803791FE[];
    extern u8 lbl_8047B62C[];
    extern void fn_801F0F04();
    extern void fn_801F1170();
    extern void fn_801F11CC();
    extern void fn_802026E4();
    extern void fn_80205224();
    extern void fn_802062FC();
    extern void fn_8020D888();
    extern void fn_8020D920();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;
    f32 f9 = 0.0f;

    r31 = r3;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_80211AA0;
    r3 = 0x1;
    goto L_80211B80;
L_80211AA0: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) goto L_80211AC4;
    r3 = 0x1;
    goto L_80211B80;
L_80211AC4: ;
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80211AD8;
    r3 = 0x1;
    goto L_80211B80;
L_80211AD8: ;
    r3 = r31;
    fn_80205224();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x108) goto L_80211B7C;
    r3 = r31;
    r4 = 0x8;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x108) goto L_80211B7C;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x108) goto L_80211B7C;
    r3 = *(u32*)lbl_8047B62C;
    fn_8020D920();
    r6 = (u32)lbl_80375D30;
    r4 = r3;
    r8 = (u32)lbl_80375D30;
    r5 = r31;
    r3 = r1 + 0x8;
    r6 = 0xc;
    r7 = 0x0;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_80211B54;
    goto L_80211B68;
L_80211B54: ;
    r4 = (u32)lbl_803791FE;
    r3 = r1 + 0x8;
    r4 = (u32)lbl_803791FE;
    fn_8020D888();
    r3 = 0x1;
L_80211B68: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211B7C;
    r3 = r1 + 0x8;
    fn_801F0F04();
L_80211B7C: ;
    r3 = 0x1;
L_80211B80: ;
    r31 = *(u32*)(sp + 0x3C);
    return;
}
#pragma pop

/* 0x80211B94 | size: 0x284 | large */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211B94(void) {
    extern u8 lbl_8027A00C[];
    extern u8 lbl_80378798[];
    extern u8 lbl_80379F58[];
    extern u8 lbl_80478D78[];
    extern u8 lbl_8047B610[];
    extern u8 lbl_8047B614[];
    extern u8 lbl_8047B618[];
    extern u8 lbl_8047B625[];
    extern u8 lbl_8047B62C[];
    extern void fn_8011BBD8();
    extern void fn_801F025C();
    extern void fn_801F37B0();
    extern void fn_802136A4();
    extern void fn_8022E1F8();
    extern void fn_8022E34C();
    extern void fn_8022EB9C();
    extern void fn_80230088();
    extern void fn_8023011C();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r12 = 0;
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
    f32 f1 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r0 = r5 & 0xFF;
    /* stmw r22, 0x18(r1) */;
    r23 = r5;
    r24 = *(u32*)lbl_8047B610;
    r26 = *(u32*)lbl_8047B62C;
    *(u32*)lbl_8047B610 = r4;
    r25 = *(u8*)lbl_8047B614;
    if ((s32)r0 != (s32)0) goto L_80211BC8;
    r0 = 0x0;
    *(u8*)lbl_8047B614 = r0;
L_80211BC8: ;
    r4 = (u32)lbl_8027A00C;
    *(u32*)lbl_8047B62C = r3;
    r31 = r23 & 0xFF;
    r30 = (u32)lbl_8027A00C;
L_80211BD8: ;
    r3 = *(u32*)lbl_8047B610;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r0 = r0 << 2;
    r12 = *(u32*)(r30 + r0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    if ((u32)r31 == (u32)0x0) goto L_80211DC8;
    r0 = *(u8*)lbl_8047B614;
    if ((u32)r0 != (u32)0x1) goto L_80211CF8;
    r4 = (u32)fn_8023011C;
    r3 = 0x0;
    r4 = (u32)fn_8023011C;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r27 = *(u32*)lbl_8047B62C;
    r4 = (u32)lbl_80378798;
    r28 = *(u8*)lbl_8047B614;
    r0 = 0x0;
    r3 = (u32)lbl_8027A00C;
    r29 = *(u32*)lbl_8047B610;
    r4 = (u32)lbl_80378798;
    *(u8*)lbl_8047B614 = r0;
    r22 = (u32)lbl_8027A00C;
    *(u32*)lbl_8047B610 = r4;
    *(u32*)lbl_8047B62C = r27;
L_80211C48: ;
    r3 = *(u32*)lbl_8047B610;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r0 = r0 << 2;
    r12 = *(u32*)(r22 + r0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u8*)lbl_8047B614;
    if ((u32)r0 == (u32)0x1) goto L_80211C74;
    if ((u32)r0 != (u32)0x2) goto L_80211C48;
L_80211C74: ;
    r0 = 0x1;
    r3 = (u32)fn_8022E34C;
    *(u32*)lbl_8047B62C = r27;
    r4 = (u32)fn_8022E34C;
    r5 = r1 + 0x9;
    r3 = 0x0;
    *(u8*)lbl_8047B614 = r28;
    r6 = 0x0;
    *(u32*)lbl_8047B610 = r29;
    *(u8*)(sp + 0x9) = r0;
    fn_801F37B0();
    r4 = (u32)fn_8022E1F8;
    r3 = 0x0;
    r4 = (u32)fn_8022E1F8;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r4 = (u32)fn_80230088;
    r3 = 0x0;
    r4 = (u32)fn_80230088;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = 0x0;
    r3 = (u32)fn_8022EB9C;
    *(u8*)(sp + 0x8) = r0;
    r4 = (u32)fn_8022EB9C;
    r5 = r1 + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r0 = 0x2;
    *(u8*)lbl_8047B614 = r0;
L_80211CF8: ;
    r0 = *(u8*)lbl_8047B614;
    if ((u32)r0 != (u32)0x2) goto L_80211BD8;
    r3 = 0x11;
    r4 = 0x0;
    fn_801F025C();
    r4 = 0x0;
    r27 = r3;
    r5 = 0xd9;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = (u32)fn_802136A4;
    r22 = r3;
    r4 = (u32)fn_802136A4;
    r3 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    r4 = (0xf1e9 << 16);
    r3 = (u32)lbl_80379F58;
    r3 = (u32)lbl_80379F58;
    r5 = *(u32*)lbl_8047B618;
    /* subi r4, r4, 0x6d51 */;
    r0 = 0x0;
    r8 = r3 + (0x1 << 16);
    r9 = (u32)lbl_80478D78;
    r5 = r5 & r4;
    *(u8*)((u8*)r9 + 0x3) = r0;
    r3 = r27;
    r4 = 0x0;
    *(u32*)lbl_8047B618 = r5;
    r5 = 0xf3;
    r6 = 0x0;
    r7 = 0x0;
    *(u8*)lbl_8047B625 = r0;
    *(u8*)((u8*)r9 + 0x4) = r0;
    *(u8*)((u8*)r8 + 0x6002) = r0;
    *(u8*)((u8*)r8 + 0x60A1) = r0;
    ((void(*)(void))fn_801254B4)();
    r3 = r27;
    r4 = 0x0;
    r5 = 0xf4;
    r6 = 0x0;
    r7 = 0x9;
    ((void(*)(void))fn_801254B4)();
    r3 = r22;
    r4 = 0x0;
    r5 = 0x2d;
    r6 = 0x0;
    r7 = 0x0;
    fn_8011BBD8();
    goto L_80211DDC;
L_80211DC8: ;
    r0 = *(u8*)lbl_8047B614;
    if ((u32)r0 == (u32)0x1) goto L_80211DDC;
    if ((u32)r0 != (u32)0x2) goto L_80211BD8;
L_80211DDC: ;
    r0 = r23 & 0xFF;
    if ((u32)r0 == (u32)0x2) goto L_80211DF8;
    r0 = *(u32*)lbl_8047B618;
    r0 = r0 & 0xFFFFFDFF;
    *(u32*)lbl_8047B618 = r0;
    r0 = r0 & 0xFFF7FFFF;
    *(u32*)lbl_8047B618 = r0;
L_80211DF8: ;
    *(u32*)lbl_8047B62C = r26;
    *(u8*)lbl_8047B614 = r25;
    *(u32*)lbl_8047B610 = r24;
    /* lmw r22, 0x18(r1) */;
    return;
}
#pragma pop

/* 0x80211E18 | size: 0x8AC | massive */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80211E18(void) {
    extern u8 lbl_80279E7C[];
    extern u8 lbl_80375DF0[];
    extern u8 lbl_80375E24[];
    extern u8 lbl_80375E44[];
    extern u8 lbl_80379F58[];
    extern u8 lbl_80478D78[];
    extern u8 lbl_8047B614[];
    extern void fn_800FA280();
    extern void fn_801299C8();
    extern void fn_80132A38();
    extern void fn_801437E0();
    extern void fn_80143878();
    extern void fn_801438A0();
    extern void fn_801438C8();
    extern void fn_801438F0();
    extern void fn_80143918();
    extern void fn_80143940();
    extern void fn_80143990();
    extern void fn_801439B8();
    extern void fn_801439D4();
    extern void fn_801439F0();
    extern void fn_80143A0C();
    extern void fn_80143A28();
    extern void fn_80143A44();
    extern void fn_80143A94();
    extern void fn_80143DFC();
    extern void fn_801440A0();
    extern void fn_801DA7AC();
    extern void fn_801EF8F4();
    extern void fn_801F025C();
    extern void fn_801F4354();
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_801F8000();
    extern void fn_801F8100();
    extern void fn_801FB1C0();
    extern void fn_802026E4();
    extern void fn_80202810();
    extern void fn_80211B94();
    extern void fn_80261B68();
    extern void fn_80261E7C();
    extern void fn_8026246C();
    extern void fn_80265598();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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

    r5 = 0x14;
    r6 = 0x0;
    /* stmw r17, 0x14(r1) */;
    r21 = r3;
    r31 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r20 = r3 & 0xFFFF;
    r3 = 0x11;
    r4 = 0x0;
    fn_801F025C();
    r0 = r3;
    r3 = 0x12;
    r19 = r0;
    r4 = 0x0;
    fn_801F025C();
    r22 = r3;
    r4 = r19;
    r3 = 0x0;
    fn_801F4354();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x44;
    r6 = 0x0;
    fn_801FB1C0();
    r0 = r3;
    r3 = r19;
    r23 = r0;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r17, r3 */;
    if ((s32)r0 == (s32)0) goto L_802126B0;
    r4 = 0x0;
    r5 = 0x1f;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r3 = r17;
    r4 = 0x0;
    r5 = 0x20;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r24 = r3;
    r3 = r17;
    r4 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r28 = r3 & 0xFF;
    r3 = r31;
    fn_801440A0();
    fn_80143DFC();
    fn_80143A94();
    r4 = (u32)lbl_80379F58;
    r0 = r3;
    r4 = (u32)lbl_80379F58;
    r3 = r19;
    r30 = r4 + (0x1 << 16);
    r17 = r0;
    r26 = *(u8*)((u8*)r30 + 0x601E);
    r4 = 0x2e;
    r27 = *(u8*)((u8*)r30 + 0x60A4);
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211F40;
    r3 = r19;
    r4 = 0x2e;
    fn_80202810();
L_80211F40: ;
    r3 = r19;
    r4 = 0x15;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211F64;
    r3 = r19;
    r4 = 0x15;
    fn_80202810();
L_80211F64: ;
    r3 = r19;
    r4 = 0x28;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211F88;
    r3 = r19;
    r4 = 0x28;
    fn_80202810();
L_80211F88: ;
    if ((u32)r28 != (u32)0x1) goto L_80211F9C;
    r3 = (u32)lbl_80375E24;
    r17 = *(u32*)lbl_80375E24;
    goto L_802125DC;
L_80211F9C: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x2;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80211FD0;
    r3 = (u32)lbl_80375DF0;
    /* clrlslwi r0, r31, 16, 2 */;
    r3 = (u32)lbl_80375DF0;
    r17 = *(u32*)(r3 + r0);
    goto L_802125DC;
L_80211FD0: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)0x50) goto L_80211FE4;
    if ((u32)r0 != (u32)0x51) goto L_80211FF0;
L_80211FE4: ;
    r3 = (u32)lbl_80375E44;
    r17 = *(u32*)lbl_80375E44;
    goto L_802125DC;
L_80211FF0: ;
    r3 = r31;
    fn_801440A0();
    fn_80143DFC();
    fn_80143A94();
    /* mr. r25, r3 */;
    if ((u32)r0 != (u32)0x51) goto L_80212010;
    r25 = 0x7;
    goto L_8021214C;
L_80212010: ;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)0x13) goto L_80212024;
    r25 = 0x1;
    goto L_8021214C;
L_80212024: ;
    fn_801437E0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x13) goto L_80212038;
    r25 = 0x2;
    goto L_8021214C;
L_80212038: ;
    r3 = r25;
    fn_80143940();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802120B0;
    r3 = r25;
    fn_80143918();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802120B0;
    r3 = r25;
    fn_801438F0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802120B0;
    r3 = r25;
    fn_801438C8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802120B0;
    r3 = r25;
    fn_801438A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_802120B0;
    r3 = r25;
    fn_80143878();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802120B8;
L_802120B0: ;
    r25 = 0x3;
    goto L_8021214C;
L_802120B8: ;
    r3 = r25;
    fn_80143A44();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802120D4;
    r25 = 0x4;
    goto L_8021214C;
L_802120D4: ;
    r3 = r25;
    fn_80143A28();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212124;
    r3 = r25;
    fn_80143A0C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212124;
    r3 = r25;
    fn_801439F0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212124;
    r3 = r25;
    fn_801439D4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212124;
    r3 = r25;
    fn_801439B8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) goto L_8021212C;
L_80212124: ;
    r25 = 0x5;
    goto L_8021214C;
L_8021212C: ;
    r3 = r25;
    fn_80143990();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212148;
    r25 = 0x6;
    goto L_8021214C;
L_80212148: ;
    r25 = 0x7;
L_8021214C: ;
    r0 = r25 & 0xFF;
    if ((u32)r0 != (u32)0x7) goto L_80212164;
    r3 = (u32)lbl_80375E24;
    r17 = *(u32*)lbl_80375E24;
    goto L_802125DC;
L_80212164: ;
    r7 = r19;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x4b;
    r6 = 0x0;
    fn_801F4C14();
    r0 = r25 & 0xFF;
    r3 = 0x0;
    r18 = (u32)lbl_80478D78;
    *(u8*)((u8*)r18 + 0x5) = r3;
    if ((s32)r0 == (s32)0x4) goto L_802123B0;
    if ((s32)r0 >= (s32)0x4) goto L_802121A4;
    if ((s32)r0 >= (s32)0x3) goto L_802121B4;
    goto L_802125CC;
L_802121A4: ;
    if ((s32)r0 == (s32)0x6) goto L_802125CC;
    if ((s32)r0 >= (s32)0x6) goto L_802125CC;
    goto L_802123BC;
L_802121B4: ;
    r3 = r17;
    fn_80143940();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802122F0;
    r3 = r17;
    fn_80143918();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802122F0;
    r3 = r17;
    fn_801438F0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802122F0;
    r3 = r17;
    fn_801438C8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802122F0;
    r3 = r17;
    fn_801438A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802122F0;
    r3 = r19;
    r4 = 0x8;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8021223C;
    r0 = 0x5;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_8021223C: ;
    r3 = r19;
    r4 = 0x3;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212260;
    r0 = 0x4;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_80212260: ;
    r3 = r19;
    r4 = 0x4;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212284;
    r0 = 0x4;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_80212284: ;
    r3 = r19;
    r4 = 0x6;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802122A8;
    r0 = 0x3;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_802122A8: ;
    r3 = r19;
    r4 = 0x7;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802122CC;
    r0 = 0x2;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_802122CC: ;
    r3 = r19;
    r4 = 0x5;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802125CC;
    r0 = 0x1;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_802122F0: ;
    r3 = r17;
    fn_80143940();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212310;
    r0 = 0x5;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_80212310: ;
    r3 = r17;
    fn_80143918();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212330;
    r0 = 0x4;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_80212330: ;
    r3 = r17;
    fn_801438F0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212350;
    r0 = 0x3;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_80212350: ;
    r3 = r17;
    fn_801438C8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212370;
    r0 = 0x2;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_80212370: ;
    r3 = r17;
    fn_801438A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80212390;
    r0 = 0x1;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_80212390: ;
    r3 = r17;
    fn_80143878();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_802125CC;
    r0 = 0x0;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_802123B0: ;
    r0 = 0x5;
    *(u8*)((u8*)r18 + 0x5) = r0;
    goto L_802125CC;
L_802123BC: ;
    r0 = 0x4;
    r3 = r17;
    *(u8*)((u8*)r18 + 0x5) = r0;
    fn_80143A28();
    r19 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x1) goto L_802123FC;
    r3 = (u32)lbl_80279E7C;
    r3 = (u32)lbl_80279E7C;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_800FA280();
    r4 = r3;
    r3 = 0xd;
    fn_80132A38();
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x601E) = r0;
    goto L_802124D8;
L_802123FC: ;
    r3 = r17;
    fn_80143A0C();
    r19 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x1) goto L_80212434;
    r3 = (u32)lbl_80279E7C;
    r3 = (u32)lbl_80279E7C;
    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_800FA280();
    r4 = r3;
    r3 = 0xd;
    fn_80132A38();
    r0 = 0x2;
    *(u8*)((u8*)r30 + 0x601E) = r0;
    goto L_802124D8;
L_80212434: ;
    r3 = r17;
    fn_801439F0();
    r19 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x1) goto L_8021246C;
    r3 = (u32)lbl_80279E7C;
    r3 = (u32)lbl_80279E7C;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_800FA280();
    r4 = r3;
    r3 = 0xd;
    fn_80132A38();
    r0 = 0x3;
    *(u8*)((u8*)r30 + 0x601E) = r0;
    goto L_802124D8;
L_8021246C: ;
    r3 = r17;
    fn_801439D4();
    r19 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x1) goto L_802124A4;
    r3 = (u32)lbl_80279E7C;
    r3 = (u32)lbl_80279E7C;
    r3 = *(u32*)((u8*)r3 + 0x18);
    fn_800FA280();
    r4 = r3;
    r3 = 0xd;
    fn_80132A38();
    r0 = 0x6;
    *(u8*)((u8*)r30 + 0x601E) = r0;
    goto L_802124D8;
L_802124A4: ;
    r3 = r17;
    fn_801439B8();
    r19 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x1) goto L_802124D8;
    r3 = (u32)lbl_80279E7C;
    r3 = (u32)lbl_80279E7C;
    r3 = *(u32*)((u8*)r3 + 0x10);
    fn_800FA280();
    r4 = r3;
    r3 = 0xd;
    fn_80132A38();
    r0 = 0x4;
    *(u8*)((u8*)r30 + 0x601E) = r0;
L_802124D8: ;
    r0 = (s16)r19;
    if ((u32)r0 >= (u32)0x1) goto L_80212558;
    r0 = (s16)r19;
    if ((s32)r0 == (s32)0x1) goto L_802124F4;
    if ((s32)r0 != (s32)-0x1) goto L_8021251C;
L_802124F4: ;
    r3 = 0x76bd;
    fn_800FA280();
    r4 = r3;
    r3 = 0xe;
    fn_80132A38();
    r0 = *(u8*)((u8*)r30 + 0x601E);
    r3 = r0 & 0xF;
    r0 = r3 + 0x15;
    *(u8*)((u8*)r30 + 0x601E) = r0;
    goto L_80212540;
L_8021251C: ;
    r3 = 0x7628;
    fn_800FA280();
    r4 = r3;
    r3 = 0xe;
    fn_80132A38();
    r0 = *(u8*)((u8*)r30 + 0x601E);
    r3 = r0 & 0xF;
    r0 = r3 + 0x2d;
    *(u8*)((u8*)r30 + 0x60A4) = r0;
L_80212540: ;
    r3 = 0x7629;
    fn_800FA280();
    r4 = r3;
    r3 = 0x41;
    fn_80132A38();
    goto L_802125CC;
L_80212558: ;
    r0 = (s16)r19;
    if ((s32)r0 == (s32)0x1) goto L_8021256C;
    if ((s32)r0 != (s32)-0x1) goto L_80212594;
L_8021256C: ;
    r3 = 0x76bd;
    fn_800FA280();
    r4 = r3;
    r3 = 0xe;
    fn_80132A38();
    r0 = *(u8*)((u8*)r30 + 0x601E);
    r3 = r0 & 0xF;
    r0 = r3 + 0xe;
    *(u8*)((u8*)r30 + 0x60A4) = r0;
    goto L_802125B8;
L_80212594: ;
    r3 = 0x7626;
    fn_800FA280();
    r4 = r3;
    r3 = 0xe;
    fn_80132A38();
    r0 = *(u8*)((u8*)r30 + 0x601E);
    r3 = r0 & 0xF;
    r0 = r3 + 0x26;
    *(u8*)((u8*)r30 + 0x60A4) = r0;
L_802125B8: ;
    r3 = 0x7627;
    fn_800FA280();
    r4 = r3;
    r3 = 0x41;
    fn_80132A38();
L_802125CC: ;
    r3 = (u32)lbl_80375E24;
    /* clrlslwi r0, r25, 24, 2 */;
    r3 = (u32)lbl_80375E24;
    r17 = *(u32*)(r3 + r0);
L_802125DC: ;
    r0 = 0x0;
    r3 = r29;
    *(u8*)lbl_8047B614 = r0;
    fn_801F8000();
    r4 = r3;
    r3 = 0x22;
    fn_80132A38();
    r3 = r29;
    fn_801F8100();
    r4 = r3;
    r3 = 0x23;
    fn_80132A38();
    r3 = r29;
    fn_801F8100();
    r4 = r3;
    r3 = 0x13;
    fn_80132A38();
    r4 = r31 & 0xFFFF;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    fn_800FA280();
    r4 = r3;
    r3 = 0x29;
    fn_80132A38();
    r3 = 0x1;
    fn_801EF8F4();
    if ((u32)r28 != (u32)0x0) goto L_80212664;
    r3 = r22;
    r4 = r20;
    r5 = 0x1;
    fn_80265598();
L_80212664: ;
    r3 = r21;
    r4 = r17;
    r5 = 0x1;
    fn_80211B94();
    if ((u32)r28 != (u32)0x0) goto L_80212690;
    r3 = r23;
    r4 = r31;
    r6 = (s16)r24;
    r5 = 0x1;
    fn_801299C8();
L_80212690: ;
    *(u8*)((u8*)r30 + 0x601E) = r26;
    *(u8*)((u8*)r30 + 0x60A4) = r27;
    fn_801DA7AC();
    r3 = 0x0;
    fn_80261B68();
    r3 = 0x0;
    fn_80261E7C();
    fn_8026246C();
L_802126B0: ;
    /* lmw r17, 0x14(r1) */;
    return;
}
#pragma pop


#pragma pop
