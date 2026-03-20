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
extern u8  lbl_80478D30;   /* Event table base (6 bytes per entry) */

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
#pragma peephole off
void fn_80201890(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x802019BC | size: 0x170 | medium */
#pragma peephole off
void fn_802019BC(void) {
    /* TODO: decompile (0x170 bytes, ~92 instructions) */
}
#pragma peephole reset

/* 0x80201B2C | size: 0x12C | medium */
#pragma peephole off
void fn_80201B2C(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80201C58 | size: 0x12C | medium */
#pragma peephole off
void fn_80201C58(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80201D84 | size: 0x12C | medium */
#pragma peephole off
void fn_80201D84(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80201EB0 | size: 0x12C | medium */
#pragma peephole off
void fn_80201EB0(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80201FDC | size: 0x12C | medium */
#pragma peephole off
void fn_80201FDC(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80202108 | size: 0x12C | medium */
#pragma peephole off
void fn_80202108(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80202234 | size: 0x12C | medium */
#pragma peephole off
void fn_80202234(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80202360 | size: 0x12C | medium */
#pragma peephole off
void fn_80202360(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x8020248C | size: 0x12C | medium */
#pragma peephole off
void fn_8020248C(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x802025B8 | size: 0x12C | medium */
#pragma peephole off
void fn_802025B8(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x802026E4 | size: 0x12C | medium */
#pragma peephole off
void fn_802026E4(void) {
    /* TODO: decompile (0x12C bytes, ~75 instructions) */
}
#pragma peephole reset

/* 0x80202810 | size: 0x188 | medium */
#pragma peephole off
void fn_80202810(void) {
    /* TODO: decompile (0x188 bytes, ~98 instructions) */
}
#pragma peephole reset

/* 0x80202998 | size: 0x94 | medium */
#pragma peephole off
void fn_80202998(void) {
    /* TODO: decompile (0x94 bytes, ~37 instructions) */
}
#pragma peephole reset

/* 0x80202A2C | size: 0xB0 | medium */
#pragma peephole off
void fn_80202A2C(void) {
    /* TODO: decompile (0xB0 bytes, ~44 instructions) */
}
#pragma peephole reset

/* 0x80202ADC | size: 0xAC | medium */
#pragma peephole off
void fn_80202ADC(void) {
    /* TODO: decompile (0xAC bytes, ~43 instructions) */
}
#pragma peephole reset

/* 0x80202B88 | size: 0x94 | medium */
#pragma peephole off
void fn_80202B88(void) {
    /* TODO: decompile (0x94 bytes, ~37 instructions) */
}
#pragma peephole reset

/* 0x80202C1C | size: 0x57C | large */
#pragma peephole off
void fn_80202C1C(void) {
    /* TODO: decompile (0x57C bytes, ~351 instructions) */
}
#pragma peephole reset

/* 0x80203198 | size: 0x14C | medium */
#pragma peephole off
void fn_80203198(void) {
    /* TODO: decompile (0x14C bytes, ~83 instructions) */
}
#pragma peephole reset

/* 0x802032E4 | size: 0x138 | medium */
#pragma peephole off
void fn_802032E4(void) {
    /* TODO: decompile (0x138 bytes, ~78 instructions) */
}
#pragma peephole reset

/* 0x8020341C | size: 0x140 | medium */
#pragma peephole off
void fn_8020341C(void) {
    /* TODO: decompile (0x140 bytes, ~80 instructions) */
}
#pragma peephole reset

/* 0x8020355C | size: 0x60 | small */
void fn_8020355C(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x802035BC | size: 0x64 | small */
void fn_802035BC(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x80203620 | size: 0x5C | small */
void fn_80203620(void) {
    /* TODO: decompile (0x5C bytes) */
}

/* 0x8020367C | size: 0x58 | small */
void fn_8020367C(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x802036D4 | size: 0x84 | medium */
#pragma peephole off
void fn_802036D4(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x80203758 | size: 0x84 | medium */
#pragma peephole off
void fn_80203758(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x802037DC | size: 0x6C | small */
void fn_802037DC(void) {
    /* TODO: decompile (0x6C bytes) */
}

/* 0x80203848 | size: 0x5C | small */
void fn_80203848(void) {
    /* TODO: decompile (0x5C bytes) */
}

/* 0x802038A4 | size: 0x1C8 | medium */
#pragma peephole off
void fn_802038A4(void) {
    /* TODO: decompile (0x1C8 bytes, ~114 instructions) */
}
#pragma peephole reset

/* 0x80203A6C | size: 0x70 | small */
void fn_80203A6C(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x80203ADC | size: 0x80 | small */
void fn_80203ADC(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x80203B5C | size: 0x80 | small */
void fn_80203B5C(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x80203BDC | size: 0x80 | small */
void fn_80203BDC(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x80203C5C | size: 0x70 | small */
void fn_80203C5C(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x80203CCC | size: 0x70 | small */
void fn_80203CCC(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x80203D3C | size: 0x70 | small */
void fn_80203D3C(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x80203DAC | size: 0x60 | small */
void fn_80203DAC(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x80203E0C | size: 0x70 | small */
void fn_80203E0C(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x80203E7C | size: 0x60 | small */
void fn_80203E7C(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x80203EDC | size: 0x108 | medium */
#pragma peephole off
void fn_80203EDC(void) {
    /* TODO: decompile (0x108 bytes, ~66 instructions) */
}
#pragma peephole reset

/* 0x80203FE4 | size: 0x104 | medium */
#pragma peephole off
void fn_80203FE4(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x802041EC | size: 0xF4 | medium */
#pragma peephole off
void fn_802041EC(void) {
    /* TODO: decompile (0xF4 bytes, ~61 instructions) */
}
#pragma peephole reset

/* 0x802042E0 | size: 0xF4 | medium */
#pragma peephole off
void fn_802042E0(void) {
    /* TODO: decompile (0xF4 bytes, ~61 instructions) */
}
#pragma peephole reset

/* 0x802043D4 | size: 0x480 | large */
#pragma peephole off
void fn_802043D4(void) {
    /* TODO: decompile (0x480 bytes, ~288 instructions) */
}
#pragma peephole reset

/* 0x80204854 | size: 0xD4 | medium */
#pragma peephole off
void fn_80204854(void) {
    /* TODO: decompile (0xD4 bytes, ~53 instructions) */
}
#pragma peephole reset

/* 0x80204928 | size: 0x48 | small */
void fn_80204928(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x80204970 | size: 0xA0 | medium */
#pragma peephole off
void fn_80204970(void) {
    /* TODO: decompile (0xA0 bytes, ~40 instructions) */
}
#pragma peephole reset

/* 0x80204A10 | size: 0x4C | small */
void fn_80204A10(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x80204A5C | size: 0x1AC | medium */
#pragma peephole off
void fn_80204A5C(void) {
    /* TODO: decompile (0x1AC bytes, ~107 instructions) */
}
#pragma peephole reset

/* 0x80204C08 | size: 0xD8 | medium */
#pragma peephole off
void fn_80204C08(void) {
    /* TODO: decompile (0xD8 bytes, ~54 instructions) */
}
#pragma peephole reset

/* 0x80204CE0 | size: 0x104 | medium */
#pragma peephole off
void fn_80204CE0(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x80204DE4 | size: 0x188 | medium */
#pragma peephole off
void fn_80204DE4(void) {
    /* TODO: decompile (0x188 bytes, ~98 instructions) */
}
#pragma peephole reset

/* 0x80204F6C | size: 0xF0 | medium */
#pragma peephole off
void fn_80204F6C(void) {
    /* TODO: decompile (0xF0 bytes, ~60 instructions) */
}
#pragma peephole reset

/* 0x8020505C | size: 0x98 | medium */
#pragma peephole off
void fn_8020505C(void) {
    /* TODO: decompile (0x98 bytes, ~38 instructions) */
}
#pragma peephole reset

/* 0x80205134 | size: 0x50 | small */
void fn_80205134(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x802051D4 | size: 0x50 | small */
void fn_802051D4(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x80205224 | size: 0x50 | small */
void fn_80205224(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x80205274 | size: 0x690 | large */
#pragma peephole off
void fn_80205274(void) {
    /* TODO: decompile (0x690 bytes, ~420 instructions) */
}
#pragma peephole reset

/* 0x80205904 | size: 0x178 | medium */
#pragma peephole off
void fn_80205904(void) {
    /* TODO: decompile (0x178 bytes, ~94 instructions) */
}
#pragma peephole reset

/* 0x80205A7C | size: 0x58 | small */
void fn_80205A7C(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x80205AD4 | size: 0x58 | small */
void fn_80205AD4(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x80205B2C | size: 0x60 | small */
void fn_80205B2C(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x80205BE8 | size: 0x3C | small */
void fn_80205BE8(void) {
    /* TODO: decompile (0x3C bytes) */
}

/* 0x80205C24 | size: 0x684 | large */
#pragma peephole off
void fn_80205C24(void) {
    /* TODO: decompile (0x684 bytes, ~417 instructions) */
}
#pragma peephole reset

/* 0x802062A8 | size: 0x54 | small */
void fn_802062A8(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x802062FC | size: 0x30C | large */
#pragma peephole off
void fn_802062FC(void) {
    /* TODO: decompile (0x30C bytes, ~195 instructions) */
}
#pragma peephole reset

/* 0x80206608 | size: 0x178 | medium */
#pragma peephole off
void fn_80206608(void) {
    /* TODO: decompile (0x178 bytes, ~94 instructions) */
}
#pragma peephole reset

/* 0x80206780 | size: 0x148 | medium */
#pragma peephole off
void fn_80206780(void) {
    /* TODO: decompile (0x148 bytes, ~82 instructions) */
}
#pragma peephole reset

/* 0x802068C8 | size: 0x13C | medium */
#pragma peephole off
void fn_802068C8(void) {
    /* TODO: decompile (0x13C bytes, ~79 instructions) */
}
#pragma peephole reset

/* 0x80206A04 | size: 0xE8 | medium */
#pragma peephole off
void fn_80206A04(void) {
    /* TODO: decompile (0xE8 bytes, ~58 instructions) */
}
#pragma peephole reset

/* 0x80206AEC | size: 0x150 | medium */
#pragma peephole off
void fn_80206AEC(void) {
    /* TODO: decompile (0x150 bytes, ~84 instructions) */
}
#pragma peephole reset

/* 0x80206C3C | size: 0x58 | small */
void fn_80206C3C(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x80206C94 | size: 0x72C | large */
#pragma peephole off
void fn_80206C94(void) {
    /* TODO: decompile (0x72C bytes, ~459 instructions) */
}
#pragma peephole reset

/* 0x802073C0 | size: 0x88 | medium */
#pragma peephole off
void fn_802073C0(void) {
    /* TODO: decompile (0x88 bytes, ~34 instructions) */
}
#pragma peephole reset

/* 0x80207448 | size: 0x15C | medium */
#pragma peephole off
void fn_80207448(void) {
    /* TODO: decompile (0x15C bytes, ~87 instructions) */
}
#pragma peephole reset

/* 0x802075A4 | size: 0x1BC | medium */
#pragma peephole off
void fn_802075A4(void) {
    /* TODO: decompile (0x1BC bytes, ~111 instructions) */
}
#pragma peephole reset

/* 0x80207760 | size: 0x74 | small */
void fn_80207760(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x802077D4 | size: 0x11C | medium */
#pragma peephole off
void fn_802077D4(void) {
    /* TODO: decompile (0x11C bytes, ~71 instructions) */
}
#pragma peephole reset

/* 0x802078F0 | size: 0xEC | medium */
#pragma peephole off
void fn_802078F0(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x802079DC | size: 0x104 | medium */
#pragma peephole off
void fn_802079DC(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x80207AE0 | size: 0x7C | small */
void fn_80207AE0(void) {
    /* TODO: decompile (0x7C bytes) */
}

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
void fn_80207C24(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x80207C6C | size: 0x2F0 | large */
#pragma peephole off
void fn_80207C6C(void) {
    /* TODO: decompile (0x2F0 bytes, ~188 instructions) */
}
#pragma peephole reset

/* 0x80207F5C | size: 0xCC | medium */
#pragma peephole off
void fn_80207F5C(void) {
    /* TODO: decompile (0xCC bytes, ~51 instructions) */
}
#pragma peephole reset

/* 0x80208028 | size: 0x80 | small */
void fn_80208028(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x802080A8 | size: 0x35C | large */
#pragma peephole off
void fn_802080A8(void) {
    /* TODO: decompile (0x35C bytes, ~215 instructions) */
}
#pragma peephole reset

/* 0x80208404 | size: 0x150 | medium */
#pragma peephole off
void fn_80208404(void) {
    /* TODO: decompile (0x150 bytes, ~84 instructions) */
}
#pragma peephole reset

/* 0x80208554 | size: 0x70 | small */
void fn_80208554(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x802085C4 | size: 0xEC | medium */
#pragma peephole off
void fn_802085C4(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x802086B0 | size: 0x38 | small */
void fn_802086B0(void) {
    /* TODO: decompile (0x38 bytes) */
}

/* 0x802086E8 | size: 0x68 | small */
void fn_802086E8(void) {
    /* TODO: decompile (0x68 bytes) */
}

/* 0x80208750 | size: 0x70 | small */
void fn_80208750(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x802087C0 | size: 0x458 | large */
#pragma peephole off
void fn_802087C0(void) {
    /* TODO: decompile (0x458 bytes, ~278 instructions) */
}
#pragma peephole reset

/* 0x80208C18 | size: 0x2B8 | large */
#pragma peephole off
void fn_80208C18(void) {
    /* TODO: decompile (0x2B8 bytes, ~174 instructions) */
}
#pragma peephole reset

/* 0x80208ED0 | size: 0x25C | large */
#pragma peephole off
void fn_80208ED0(void) {
    /* TODO: decompile (0x25C bytes, ~151 instructions) */
}
#pragma peephole reset

/* 0x8020912C | size: 0x254 | large */
#pragma peephole off
void fn_8020912C(void) {
    /* TODO: decompile (0x254 bytes, ~149 instructions) */
}
#pragma peephole reset

/* 0x80209380 | size: 0x104 | medium */
#pragma peephole off
void fn_80209380(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x80209484 | size: 0x48 | small */
void fn_80209484(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x802094CC | size: 0x90 | medium */
#pragma peephole off
void fn_802094CC(void) {
    /* TODO: decompile (0x90 bytes, ~36 instructions) */
}
#pragma peephole reset

/* 0x8020955C | size: 0xBC | medium */
#pragma peephole off
void fn_8020955C(void) {
    /* TODO: decompile (0xBC bytes, ~47 instructions) */
}
#pragma peephole reset

/* 0x80209618 | size: 0xD0 | medium */
#pragma peephole off
void fn_80209618(void) {
    /* TODO: decompile (0xD0 bytes, ~52 instructions) */
}
#pragma peephole reset

/* 0x802096E8 | size: 0xE0 | medium */
#pragma peephole off
void fn_802096E8(void) {
    /* TODO: decompile (0xE0 bytes, ~56 instructions) */
}
#pragma peephole reset

/* 0x802097C8 | size: 0x54 | small */
void fn_802097C8(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x8020981C | size: 0x54 | small */
void fn_8020981C(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x80209870 | size: 0x9C | medium */
#pragma peephole off
void fn_80209870(void) {
    /* TODO: decompile (0x9C bytes, ~39 instructions) */
}
#pragma peephole reset

/* 0x8020990C | size: 0x54 | small */
void fn_8020990C(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x80209960 | size: 0x4C | small */
void fn_80209960(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x802099AC | size: 0x270 | large */
#pragma peephole off
void fn_802099AC(void) {
    /* TODO: decompile (0x270 bytes, ~156 instructions) */
}
#pragma peephole reset

/* 0x80209C1C | size: 0x98 | medium */
#pragma peephole off
void fn_80209C1C(void) {
    /* TODO: decompile (0x98 bytes, ~38 instructions) */
}
#pragma peephole reset

/* 0x80209CB4 | size: 0xDC | medium */
#pragma peephole off
void fn_80209CB4(void) {
    /* TODO: decompile (0xDC bytes, ~55 instructions) */
}
#pragma peephole reset

/* 0x80209D90 | size: 0x188 | medium */
#pragma peephole off
void fn_80209D90(void) {
    /* TODO: decompile (0x188 bytes, ~98 instructions) */
}
#pragma peephole reset

/* 0x80209F18 | size: 0x94 | medium */
#pragma peephole off
void fn_80209F18(void) {
    /* TODO: decompile (0x94 bytes, ~37 instructions) */
}
#pragma peephole reset

/* 0x80209FAC | size: 0x64 | small */
void fn_80209FAC(void) {
    /* TODO: decompile (0x64 bytes) */
}

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
void fn_8020A040(void) {
    /* TODO: decompile (0x28 bytes) */
}

/* 0x8020A080 | size: 0x24 | small */
void fn_8020A080(void) {
    /* TODO: decompile (0x24 bytes) */
}

/* 0x8020A224 | size: 0x34 | small */
void fn_8020A224(void) {
    /* TODO: decompile (0x34 bytes) */
}

/* 0x8020A2B8 | size: 0x40 | small */
void fn_8020A2B8(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8020A398 | size: 0xE0 | medium */
#pragma peephole off
void fn_8020A398(void) {
    /* TODO: decompile (0xE0 bytes, ~56 instructions) */
}
#pragma peephole reset

/* 0x8020A478 | size: 0x88 | medium */
#pragma peephole off
void fn_8020A478(void) {
    /* TODO: decompile (0x88 bytes, ~34 instructions) */
}
#pragma peephole reset

/* 0x8020A500 | size: 0x40 */
u32 fn_8020A500(u16 idx) {
    u8* entry;
    idx = (u16)idx;
    if (idx >= lbl_80478D38) {
        entry = NULL;
    } else {
        entry = &lbl_80478D30 + idx * 6;
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
        entry = &lbl_80478D30 + idx * 6;
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
        entry = &lbl_80478D30 + idx * 6;
    }
    if (entry == NULL) { return 0; }
    return entry[0];
}

/* 0x8020A5C0 | size: 0x70 | small */
void fn_8020A5C0(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8020A630 | size: 0x70 | small */
void fn_8020A630(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8020A6A0 | size: 0x70 | small */
void fn_8020A6A0(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8020A710 | size: 0x70 | small */
void fn_8020A710(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8020A780 | size: 0x70 | small */
void fn_8020A780(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8020A7F0 | size: 0x70 | small */
void fn_8020A7F0(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8020A860 | size: 0x40 | small */
void fn_8020A860(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8020A8A0 | size: 0x40 | small */
void fn_8020A8A0(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8020A8E0 | size: 0x550 | large */
#pragma peephole off
void fn_8020A8E0(void) {
    /* TODO: decompile (0x550 bytes, ~340 instructions) */
}
#pragma peephole reset

/* 0x8020AED0 | size: 0x60 | small */
void fn_8020AED0(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x8020AF30 | size: 0xC4 | medium */
#pragma peephole off
void fn_8020AF30(void) {
    /* TODO: decompile (0xC4 bytes, ~49 instructions) */
}
#pragma peephole reset

/* 0x8020AFF4 | size: 0x5C | small */
void fn_8020AFF4(void) {
    /* TODO: decompile (0x5C bytes) */
}

/* 0x8020B058 | size: 0x2D8 | large */
#pragma peephole off
void fn_8020B058(void) {
    /* TODO: decompile (0x2D8 bytes, ~182 instructions) */
}
#pragma peephole reset

/* 0x8020B330 | size: 0x3A4 | large */
#pragma peephole off
void fn_8020B330(void) {
    /* TODO: decompile (0x3A4 bytes, ~233 instructions) */
}
#pragma peephole reset

/* 0x8020B6D4 | size: 0x58 | small */
void fn_8020B6D4(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x8020B72C | size: 0x1E4 | medium */
#pragma peephole off
void fn_8020B72C(void) {
    /* TODO: decompile (0x1E4 bytes, ~121 instructions) */
}
#pragma peephole reset

/* 0x8020B910 | size: 0x104 | medium */
#pragma peephole off
void fn_8020B910(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x8020BA14 | size: 0x6C | small */
void fn_8020BA14(void) {
    /* TODO: decompile (0x6C bytes) */
}

/* 0x8020BA80 | size: 0x78 | small */
void fn_8020BA80(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x8020BAF8 | size: 0xAC | medium */
#pragma peephole off
void fn_8020BAF8(void) {
    /* TODO: decompile (0xAC bytes, ~43 instructions) */
}
#pragma peephole reset

/* 0x8020BBA4 | size: 0x58 | small */
void fn_8020BBA4(void) {
    /* TODO: decompile (0x58 bytes) */
}

/* 0x8020BBFC | size: 0x98 | medium */
#pragma peephole off
void fn_8020BBFC(void) {
    /* TODO: decompile (0x98 bytes, ~38 instructions) */
}
#pragma peephole reset

/* 0x8020BC94 | size: 0x1A4 | medium */
#pragma peephole off
void fn_8020BC94(void) {
    /* TODO: decompile (0x1A4 bytes, ~105 instructions) */
}
#pragma peephole reset

/* 0x8020BE38 | size: 0x108 | medium */
#pragma peephole off
void fn_8020BE38(void) {
    /* TODO: decompile (0x108 bytes, ~66 instructions) */
}
#pragma peephole reset

/* 0x8020BF40 | size: 0x60 | small */
void fn_8020BF40(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x8020BFA0 | size: 0x120 | medium */
#pragma peephole off
void fn_8020BFA0(void) {
    /* TODO: decompile (0x120 bytes, ~72 instructions) */
}
#pragma peephole reset

/* 0x8020C0C0 | size: 0x24 | small */
void fn_8020C0C0(void) {
    /* TODO: decompile (0x24 bytes) */
}

/* 0x8020C0E4 | size: 0x24 | small */
void fn_8020C0E4(void) {
    /* TODO: decompile (0x24 bytes) */
}

/* 0x8020C108 | size: 0x54 | small */
void fn_8020C108(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x8020C15C | size: 0x6E4 | large */
#pragma peephole off
void fn_8020C15C(void) {
    /* TODO: decompile (0x6E4 bytes, ~441 instructions) */
}
#pragma peephole reset

/* 0x8020CA98 | size: 0x548 | large */
#pragma peephole off
void fn_8020CA98(void) {
    /* TODO: decompile (0x548 bytes, ~338 instructions) */
}
#pragma peephole reset

/* 0x8020CFE0 | size: 0x21C | large */
#pragma peephole off
void fn_8020CFE0(void) {
    /* TODO: decompile (0x21C bytes, ~135 instructions) */
}
#pragma peephole reset

/* 0x8020D1FC | size: 0x49C | large */
#pragma peephole off
void fn_8020D1FC(void) {
    /* TODO: decompile (0x49C bytes, ~295 instructions) */
}
#pragma peephole reset

/* 0x8020D698 | size: 0xEC | medium */
#pragma peephole off
void fn_8020D698(void) {
    /* TODO: decompile (0xEC bytes, ~59 instructions) */
}
#pragma peephole reset

/* 0x8020D7CC | size: 0x1C | tiny */
void fn_8020D7CC(void) {
    /* TODO: decompile (0x1C bytes) */
}

/* 0x8020D7E8 | size: 0x2C | small */
void fn_8020D7E8(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x8020D844 | size: 0x24 | small */
void fn_8020D844(void) {
    /* TODO: decompile (0x24 bytes) */
}

/* 0x8020D968 | size: 0x38 | small */
void fn_8020D968(void) {
    /* TODO: decompile (0x38 bytes) */
}

/* 0x8020D9E8 | size: 0x2C | small */
void fn_8020D9E8(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x8020DA14 | size: 0xBC | medium */
#pragma peephole off
void fn_8020DA14(void) {
    /* TODO: decompile (0xBC bytes, ~47 instructions) */
}
#pragma peephole reset

/* 0x8020DAD0 | size: 0x274 | large */
#pragma peephole off
void fn_8020DAD0(void) {
    /* TODO: decompile (0x274 bytes, ~157 instructions) */
}
#pragma peephole reset

/* 0x8020DD44 | size: 0x3C | small */
void fn_8020DD44(void) {
    /* TODO: decompile (0x3C bytes) */
}

/* 0x8020DD80 | size: 0xD0 | medium */
#pragma peephole off
void fn_8020DD80(void) {
    /* TODO: decompile (0xD0 bytes, ~52 instructions) */
}
#pragma peephole reset

/* 0x8020DE68 | size: 0x18 | tiny */
void fn_8020DE68(void) {
    /* TODO: decompile (0x18 bytes) */
}

/* 0x8020DEB0 | size: 0x28 | small */
void fn_8020DEB0(void) {
    /* TODO: decompile (0x28 bytes) */
}

/* 0x8020DF10 | size: 0x40 | small */
void fn_8020DF10(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8020DF50 | size: 0x40 | small */
void fn_8020DF50(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8020E020 | size: 0x48 | small */
void fn_8020E020(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x8020E068 | size: 0x48 | small */
void fn_8020E068(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x8020E0F8 | size: 0x2C | small */
void fn_8020E0F8(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x8020E124 | size: 0x80 | small */
void fn_8020E124(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x8020E204 | size: 0x2C | small */
void fn_8020E204(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x8020E488 | size: 0x2C | small */
void fn_8020E488(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x8020E4CC | size: 0x1C | tiny */
void fn_8020E4CC(void) {
    /* TODO: decompile (0x1C bytes) */
}

/* 0x8020E4E8 | size: 0x94 | medium */
#pragma peephole off
void fn_8020E4E8(void) {
    /* TODO: decompile (0x94 bytes, ~37 instructions) */
}
#pragma peephole reset

/* 0x8020E57C | size: 0x98 | medium */
#pragma peephole off
void fn_8020E57C(void) {
    /* TODO: decompile (0x98 bytes, ~38 instructions) */
}
#pragma peephole reset

/* 0x8020E614 | size: 0x2C | small */
void fn_8020E614(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x8020E640 | size: 0x94 | medium */
#pragma peephole off
void fn_8020E640(void) {
    /* TODO: decompile (0x94 bytes, ~37 instructions) */
}
#pragma peephole reset

/* 0x8020E6D4 | size: 0x84 | medium */
#pragma peephole off
void fn_8020E6D4(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x8020E758 | size: 0x54 | small */
void fn_8020E758(void) {
    /* TODO: decompile (0x54 bytes) */
}

/* 0x8020E7AC | size: 0x1B0 | medium */
#pragma peephole off
void fn_8020E7AC(void) {
    /* TODO: decompile (0x1B0 bytes, ~108 instructions) */
}
#pragma peephole reset

/* 0x8020E95C | size: 0x24C | large */
#pragma peephole off
void fn_8020E95C(void) {
    /* TODO: decompile (0x24C bytes, ~147 instructions) */
}
#pragma peephole reset

/* 0x8020EBA8 | size: 0xFC | medium */
#pragma peephole off
void fn_8020EBA8(void) {
    /* TODO: decompile (0xFC bytes, ~63 instructions) */
}
#pragma peephole reset

/* 0x8020ECA4 | size: 0x3C | small */
void fn_8020ECA4(void) {
    /* TODO: decompile (0x3C bytes) */
}

/* 0x8020ECE0 | size: 0xDC | medium */
#pragma peephole off
void fn_8020ECE0(void) {
    /* TODO: decompile (0xDC bytes, ~55 instructions) */
}
#pragma peephole reset

/* 0x8020EDBC | size: 0x60 | small */
void fn_8020EDBC(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x8020EE1C | size: 0xA4 | medium */
#pragma peephole off
void fn_8020EE1C(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x8020EEC0 | size: 0x14 | tiny */
void fn_8020EEC0(void) { }

/* 0x8020EED4 | size: 0x22C | large */
#pragma peephole off
void fn_8020EED4(void) {
    /* TODO: decompile (0x22C bytes, ~139 instructions) */
}
#pragma peephole reset

/* 0x8020F108 | size: 0x128 | medium */
#pragma peephole off
void fn_8020F108(void) {
    /* TODO: decompile (0x128 bytes, ~74 instructions) */
}
#pragma peephole reset

/* 0x8020F238 | size: 0x128 | medium */
#pragma peephole off
void fn_8020F238(void) {
    /* TODO: decompile (0x128 bytes, ~74 instructions) */
}
#pragma peephole reset

/* 0x8020F368 | size: 0x80 | small */
void fn_8020F368(void) {
    /* TODO: decompile (0x80 bytes) */
}

/* 0x8020F3F0 | size: 0xA4 | medium */
#pragma peephole off
void fn_8020F3F0(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x8020F494 | size: 0x84 | medium */
#pragma peephole off
void fn_8020F494(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x8020F518 | size: 0xA4 | medium */
#pragma peephole off
void fn_8020F518(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x8020F5BC | size: 0x8C | medium */
#pragma peephole off
void fn_8020F5BC(void) {
    /* TODO: decompile (0x8C bytes, ~35 instructions) */
}
#pragma peephole reset

/* 0x8020F648 | size: 0xA4 | medium */
#pragma peephole off
void fn_8020F648(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x8020F6EC | size: 0x60 | small */
void fn_8020F6EC(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x8020F74C | size: 0x64 | small */
void fn_8020F74C(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x8020F7B8 | size: 0x114 | medium */
#pragma peephole off
void fn_8020F7B8(void) {
    /* TODO: decompile (0x114 bytes, ~69 instructions) */
}
#pragma peephole reset

/* 0x8020F8D4 | size: 0x64 | small */
void fn_8020F8D4(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x8020F938 | size: 0x64 | small */
void fn_8020F938(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x8020F99C | size: 0x40 | small */
void fn_8020F99C(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8020F9DC | size: 0xA4 | medium */
#pragma peephole off
void fn_8020F9DC(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x8020FA80 | size: 0x40 | small */
void fn_8020FA80(void) {
    /* TODO: decompile (0x40 bytes) */
}

/* 0x8020FAC0 | size: 0x70 | small */
void fn_8020FAC0(void) {
    /* TODO: decompile (0x70 bytes) */
}

/* 0x8020FB38 | size: 0xC4 | medium */
#pragma peephole off
void fn_8020FB38(void) {
    /* TODO: decompile (0xC4 bytes, ~49 instructions) */
}
#pragma peephole reset

/* 0x8020FC04 | size: 0x6C | small */
void fn_8020FC04(void) {
    /* TODO: decompile (0x6C bytes) */
}

/* 0x8020FC70 | size: 0x11C | medium */
#pragma peephole off
void fn_8020FC70(void) {
    /* TODO: decompile (0x11C bytes, ~71 instructions) */
}
#pragma peephole reset

/* 0x8020FD8C | size: 0xB4 | medium */
#pragma peephole off
void fn_8020FD8C(void) {
    /* TODO: decompile (0xB4 bytes, ~45 instructions) */
}
#pragma peephole reset

/* 0x8020FE40 | size: 0xA4 | medium */
#pragma peephole off
void fn_8020FE40(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x8020FEE4 | size: 0x78 | small */
void fn_8020FEE4(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x8020FF5C | size: 0xA4 | medium */
#pragma peephole off
void fn_8020FF5C(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x80210000 | size: 0x74 | small */
void fn_80210000(void) {
    /* TODO: decompile (0x74 bytes) */
}

/* 0x80210074 | size: 0xA4 | medium */
#pragma peephole off
void fn_80210074(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x80210118 | size: 0x78 | small */
void fn_80210118(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x80210190 | size: 0xA4 | medium */
#pragma peephole off
void fn_80210190(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x80210234 | size: 0x84 | medium */
#pragma peephole off
void fn_80210234(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x802102B8 | size: 0xA4 | medium */
#pragma peephole off
void fn_802102B8(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x8021035C | size: 0x8C | medium */
#pragma peephole off
void fn_8021035C(void) {
    /* TODO: decompile (0x8C bytes, ~35 instructions) */
}
#pragma peephole reset

/* 0x802103E8 | size: 0x100 | medium */
#pragma peephole off
void fn_802103E8(void) {
    /* TODO: decompile (0x100 bytes, ~64 instructions) */
}
#pragma peephole reset

/* 0x802104E8 | size: 0x84 | medium */
#pragma peephole off
void fn_802104E8(void) {
    /* TODO: decompile (0x84 bytes, ~33 instructions) */
}
#pragma peephole reset

/* 0x8021056C | size: 0xA4 | medium */
#pragma peephole off
void fn_8021056C(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x80210610 | size: 0x48 | small */
void fn_80210610(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x80210658 | size: 0xA4 | medium */
#pragma peephole off
void fn_80210658(void) {
    /* TODO: decompile (0xA4 bytes, ~41 instructions) */
}
#pragma peephole reset

/* 0x802106FC | size: 0xC0 | medium */
#pragma peephole off
void fn_802106FC(void) {
    /* TODO: decompile (0xC0 bytes, ~48 instructions) */
}
#pragma peephole reset

/* 0x802107BC | size: 0xC4 | medium */
#pragma peephole off
void fn_802107BC(void) {
    /* TODO: decompile (0xC4 bytes, ~49 instructions) */
}
#pragma peephole reset

/* 0x80210888 | size: 0x108 | medium */
#pragma peephole off
void fn_80210888(void) {
    /* TODO: decompile (0x108 bytes, ~66 instructions) */
}
#pragma peephole reset

/* 0x80210998 | size: 0x168 | medium */
#pragma peephole off
void fn_80210998(void) {
    /* TODO: decompile (0x168 bytes, ~90 instructions) */
}
#pragma peephole reset

/* 0x80210B08 | size: 0xE8 | medium */
#pragma peephole off
void fn_80210B08(void) {
    /* TODO: decompile (0xE8 bytes, ~58 instructions) */
}
#pragma peephole reset

/* 0x80210BF8 | size: 0x104 | medium */
#pragma peephole off
void fn_80210BF8(void) {
    /* TODO: decompile (0x104 bytes, ~65 instructions) */
}
#pragma peephole reset

/* 0x80210D04 | size: 0x150 | medium */
#pragma peephole off
void fn_80210D04(void) {
    /* TODO: decompile (0x150 bytes, ~84 instructions) */
}
#pragma peephole reset

/* 0x80210E5C | size: 0x64 | small */
void fn_80210E5C(void) {
    /* TODO: decompile (0x64 bytes) */
}

/* 0x80210EC8 | size: 0x170 | medium */
#pragma peephole off
void fn_80210EC8(void) {
    /* TODO: decompile (0x170 bytes, ~92 instructions) */
}
#pragma peephole reset

/* 0x80211040 | size: 0x11C | medium */
#pragma peephole off
void fn_80211040(void) {
    /* TODO: decompile (0x11C bytes, ~71 instructions) */
}
#pragma peephole reset

/* 0x80211164 | size: 0x4 | trivial */
s32 fn_80211164(void) { return 0; }

/* 0x80211170 | size: 0x68C | large */
#pragma peephole off
void fn_80211170(void) {
    /* TODO: decompile (0x68C bytes, ~419 instructions) */
}
#pragma peephole reset

/* 0x802117FC | size: 0x14 | tiny */
void fn_802117FC(void) { }

/* 0x80211810 | size: 0x20 | tiny */
void fn_80211810(void) {
    /* TODO: decompile (0x20 bytes) */
}

/* 0x80211830 | size: 0xCC | medium */
#pragma peephole off
void fn_80211830(void) {
    /* TODO: decompile (0xCC bytes, ~51 instructions) */
}
#pragma peephole reset

/* 0x802118FC | size: 0x4C | small */
void fn_802118FC(void) {
    /* TODO: decompile (0x4C bytes) */
}

/* 0x80211948 | size: 0x8C | medium */
#pragma peephole off
void fn_80211948(void) {
    /* TODO: decompile (0x8C bytes, ~35 instructions) */
}
#pragma peephole reset

/* 0x802119D4 | size: 0x2C | small */
void fn_802119D4(void) {
    /* TODO: decompile (0x2C bytes) */
}

/* 0x80211A00 | size: 0x78 | small */
void fn_80211A00(void) {
    /* TODO: decompile (0x78 bytes) */
}

/* 0x80211A78 | size: 0x11C | medium */
#pragma peephole off
void fn_80211A78(void) {
    /* TODO: decompile (0x11C bytes, ~71 instructions) */
}
#pragma peephole reset

/* 0x80211B94 | size: 0x284 | large */
#pragma peephole off
void fn_80211B94(void) {
    /* TODO: decompile (0x284 bytes, ~161 instructions) */
}
#pragma peephole reset

/* 0x80211E18 | size: 0x8AC | massive */
#pragma peephole off
void fn_80211E18(void) {
    /* TODO: decompile (0x8AC bytes, ~555 instructions) */
}
#pragma peephole reset


#pragma pop
