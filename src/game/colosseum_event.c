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
void fn_8020248C(void* ctx, void* typeObj, u32 param);
void fn_802025B8(void* ctx, void* typeObj);
void fn_802026E4(void* ctx, void* typeObj);
u32 fn_80203620(u32 obj);
void fn_8020367C(u32 obj, u32 param);
u32 fn_802036D4(void* ctx);
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

/* 0x80201890 | size: 0x12C */
void fn_80201890(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011A3E4();
    extern void fn_80121574();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_80121574(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011A3E4(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011A3E4(ctx, typeObj);
    }
}

/* 0x802019BC | size: 0x170 */
void fn_802019BC(void* ctx, void* src, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011A0A8();
    extern void fn_80121484();
    u16 typeId;
    void* srcResolved;
    void* ctxResolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        srcResolved = fn_8012640C(src, 0, 0xD6, 0);
        ctxResolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (ctxResolved == NULL) {
                ctxResolved = NULL;
            } else {
                ctxResolved = fn_8012640C(ctxResolved, 0, 0xCC, 0);
            }
            if (srcResolved == NULL) {
                srcResolved = NULL;
            } else {
                srcResolved = fn_8012640C(srcResolved, 0, 0xCC, 0);
            }
            fn_80121484(ctxResolved, srcResolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011A0A8(ctxResolved, srcResolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011A0A8(ctx, src, typeObj);
    }
}

/* 0x80201B2C | size: 0x12C */
void fn_80201B2C(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern void fn_8011A570();
    extern void fn_801215E4();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_801215E4(resolved, typeObj, param);
        } else if (typeId == 0xCD) {
            fn_8011A570(resolved, typeObj, param);
        }
    } else if (typeId == 0xD8) {
        fn_8011A570(ctx, typeObj, param);
    }
}

/* 0x80201C58 | size: 0x12C */
void fn_80201C58(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011A6D4();
    extern void fn_8012165C();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_8012165C(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011A6D4(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011A6D4(ctx, typeObj);
    }
}

/* 0x80201D84 | size: 0x12C */
void fn_80201D84(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011A860();
    extern void fn_801216CC();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_801216CC(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011A860(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011A860(ctx, typeObj);
    }
}

/* 0x80201EB0 | size: 0x12C */
void fn_80201EB0(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern void fn_8011A9EC();
    extern void fn_8012173C();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_8012173C(resolved, typeObj, param);
        } else if (typeId == 0xCD) {
            fn_8011A9EC(resolved, typeObj, param);
        }
    } else if (typeId == 0xD8) {
        fn_8011A9EC(ctx, typeObj, param);
    }
}

/* 0x80201FDC | size: 0x12C */
void fn_80201FDC(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern void fn_8011AB50();
    extern void fn_801217B4();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_801217B4(resolved, typeObj, param);
        } else if (typeId == 0xCD) {
            fn_8011AB50(resolved, typeObj, param);
        }
    } else if (typeId == 0xD8) {
        fn_8011AB50(ctx, typeObj, param);
    }
}

/* 0x80202108 | size: 0x12C */
void fn_80202108(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011ACB4();
    extern void fn_8012182C();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_8012182C(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011ACB4(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011ACB4(ctx, typeObj);
    }
}

/* 0x80202234 | size: 0x12C */
void fn_80202234(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011AE40();
    extern void fn_8012189C();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_8012189C(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011AE40(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011AE40(ctx, typeObj);
    }
}

/* 0x80202360 | size: 0x12C */
void fn_80202360(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011B130();
    extern void fn_80121984();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_80121984(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011B130(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011B130(ctx, typeObj);
    }
}

/* 0x8020248C | size: 0x12C */
void fn_8020248C(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_801219F4();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_801219F4(resolved, typeObj, param);
        } else if (typeId == 0xCD) {
            fn_8011B2C0(resolved, typeObj, param);
        }
    } else if (typeId == 0xD8) {
        fn_8011B2C0(ctx, typeObj, param);
    }
}

/* 0x802025B8 | size: 0x12C */
void fn_802025B8(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011B444();
    extern void fn_80121A6C();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_80121A6C(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011B444(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011B444(ctx, typeObj);
    }
}

/* 0x802026E4 | size: 0x12C */
void fn_802026E4(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_80121ADC(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011B67C(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011B67C(ctx, typeObj);
    }
}

/* 0x80202810 | size: 0x188 | medium */
/* 0x80202810 | size: 0x188 */
void fn_80202810(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011B788();
    extern void fn_80121B4C();
    extern void fn_801DA36C();
    u16 typeId;
    u16 mode;
    void* resolved;
    void* eeData;

    eeData = fn_8012640C(ctx, 0, 0xEE, 0);
    mode = (u16)(u32)typeObj;
    if (mode == 0) {
        if (eeData != NULL) {
            fn_801DA36C(eeData, 1);
            fn_801DA36C(eeData, 2);
        }
    } else {
        if (eeData != NULL) {
            if (mode == 8) {
                fn_801DA36C(eeData, 1);
            }
            if (mode == 7) {
                fn_801DA36C(eeData, 2);
            }
        }
    }
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8 || typeId == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        typeId = fn_80119ED0(typeObj);
        if (typeId == 0x7C || typeId == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            fn_80121B4C(resolved, typeObj);
        } else if (typeId == 0xCD) {
            fn_8011B788(resolved, typeObj);
        }
    } else if (typeId == 0xD8) {
        fn_8011B788(ctx, typeObj);
    }
}

/* 0x80202998 | size: 0x94 */
void fn_80202998(void* ctx, u16 mode) {
    extern void fn_801DA36C();
    void* obj;
    u16 modeVal;
    obj = fn_8012640C(ctx, 0, 0xEE, 0);
    modeVal = mode;
    if (modeVal == 0) {
        if (obj != NULL) {
            fn_801DA36C(obj, 1);
            fn_801DA36C(obj, 2);
        }
    } else {
        if (obj != NULL) {
            if (modeVal == 8) {
                fn_801DA36C(obj, 1);
            }
            modeVal = mode;
            if (modeVal == 7) {
                fn_801DA36C(obj, 2);
            }
        }
    }
}

/* 0x80202A2C | size: 0xB0 */
void fn_80202A2C(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern void fn_8011AFCC();
    extern void fn_8012190C();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8) {
        if (ctx == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(ctx, 0, 0xCC, 0);
        }
        fn_8012190C(resolved, typeObj, param);
    } else if (typeId == 0xCD) {
        fn_8011AFCC(ctx, typeObj, param);
    }
}

/* 0x80202ADC | size: 0xAC */
void fn_80202ADC(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011B67C();
    extern void fn_80121ADC();
    u16 typeId;
    void* resolved;
    typeId = fn_80119ED0(typeObj);
    if (typeId == 0x7C || typeId == 0xC8) {
        if (ctx == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(ctx, 0, 0xCC, 0);
        }
        fn_80121ADC(resolved, typeObj);
    } else if (typeId == 0xCD) {
        fn_8011B67C(ctx, typeObj);
    }
}

/* 0x80202B88 | size: 0x94 */
u32 fn_80202B88(void* obj1, void* obj2) {
    extern u32 fn_801F02AC();
    extern u16 fn_801F54A4();
    u16 tableId;
    u32 val1;
    u32 val2;
    tableId = fn_801F54A4(NULL, 0, 0x14, 0);
    if (obj1 == NULL) {
        return 0;
    }
    if (obj2 == NULL) {
        return 0;
    }
    val1 = fn_801F02AC(2, obj1, tableId);
    val2 = fn_801F02AC(2, obj2, tableId);
    return (val1 == val2) ? 1 : 0;
}

/* 0x80202C1C | size: 0x57C | large */
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
    while (1) {
        r0 = r31 & 0xFFFF;
        if ((u32)r0 >= (u32)r24) break;
        r3 = r26;
        r6 = r31;
        r4 = 0x0;
        r5 = 0x7;
        fn_801F76B8();
        r27 = r3;
        fn_801FA634();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r30 = 0x0;
            while (1) {
                r0 = r30 & 0xFFFF;
                if ((u32)r0 >= (u32)r25) break;
                r3 = r27;
                r6 = r30;
                r4 = 0x0;
                r5 = 0x46;
                fn_801FB1C0();
                /* mr. r29, r3 */;
                if ((s32)r0 == (s32)0) {
                    r0 = 0x0;

                } else {
                    if ((s32)r0 == (s32)0) {
                        r0 = 0x0;

                    } else {
                        fn_801EF634();
                        r0 = r3 & 0xFFFF;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;
                            goto L_80202DF8;
                        }
                        r3 = r29;
                        r4 = 0x0;
                        r5 = 0xd6;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        /* mr. r21, r3 */;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;
                            goto L_80202DF8;
                        }
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;

                        } else {
                            fn_801EF634();
                            r0 = r3 & 0xFFFF;
                            if ((u32)r0 == (u32)0x1) {
                                r0 = 0x0;
                                goto L_80202DE4;
                            }
                            r3 = r21;
                            r4 = 0x0;
                            r5 = 0xcb;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202DE4;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202DE4;
                            }
                            if ((u32)r21 == (u32)0x0) {
                                r3 = 0x0;
                            } else {

                                r3 = r21;
                                r4 = 0x0;
                                r5 = 0xcc;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                            }
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202DE4;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202DE4;
                            }
                            r3 = r21;
                            r4 = 0x0;
                            r5 = 0xce;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((s32)r3 < (s32)0x0) {
                                r0 = 0x0;
                                goto L_80202DE4;
                            }
                            r0 = 0x1;
                        }
                        L_80202DE4: ;
                        r0 = r0 & 0xFF;
                        if ((s32)r3 == (s32)0x0) {
                            r0 = 0x0;
                            goto L_80202DF8;
                        }
                        r0 = 0x1;
                    }
                    L_80202DF8: ;
                    r0 = r0 & 0xFF;
                    if ((s32)r3 == (s32)0x0) {
                        r0 = 0x0;
                        goto L_80202FB0;
                    }
                    r3 = r29;
                    r4 = 0x0;
                    r5 = 0x120;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    if ((s32)r3 == (s32)0x1) {
                        r0 = 0x0;
                        goto L_80202FB0;
                    }
                    r3 = r29;
                    r4 = 0x0;
                    r5 = 0xd6;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    /* mr. r23, r3 */;
                    if ((s32)r3 == (s32)0x1) {
                        r0 = 0x0;

                    } else {
                        if ((s32)r3 == (s32)0x1) {
                            r0 = 0x0;

                        } else {
                            fn_801EF634();
                            r0 = r3 & 0xFFFF;
                            if ((u32)r0 == (u32)0x1) {
                                r0 = 0x0;
                                goto L_80202F1C;
                            }
                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xcb;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202F1C;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202F1C;
                            }
                            if ((u32)r23 == (u32)0x0) {
                                r3 = 0x0;
                            } else {

                                r3 = r23;
                                r4 = 0x0;
                                r5 = 0xcc;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                            }
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202F1C;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_80202F1C;
                            }
                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xce;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((s32)r3 < (s32)0x0) {
                                r0 = 0x0;
                                goto L_80202F1C;
                            }
                            r0 = 0x1;
                        }
                        L_80202F1C: ;
                        r0 = r0 & 0xFF;
                        if ((s32)r3 == (s32)0x0) {
                            r0 = 0x0;
                            goto L_80202F9C;
                        }
                        r3 = r23;
                        r4 = 0x0;
                        r5 = 0xd2;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        if ((s32)r3 == (s32)0x1) {
                            r0 = 0x0;
                            goto L_80202F9C;
                        }
                        if ((u32)r23 == (u32)0x0) {
                            r3 = 0x0;
                        } else {

                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xcc;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                        }
                        if ((u32)r3 == (u32)0x0) {
                            r0 = 0x0;
                            goto L_80202F9C;
                        }
                        fn_801233F4();
                        r0 = r3 & 0xFF;
                        if ((u32)r3 == (u32)0x0) {
                            r0 = 0x0;
                            goto L_80202F9C;
                        }
                        r0 = 0x1;
                    }
                    L_80202F9C: ;
                    r0 = r0 & 0xFF;
                    if ((u32)r3 == (u32)0x0) {
                        r0 = 0x0;
                        goto L_80202FB0;
                    }
                    r0 = 0x1;
                }
                L_80202FB0: ;
                r0 = r0 & 0xFF;
                if ((u32)r3 != (u32)0x0 && (u32)r28 != (u32)0x0) {

                    if ((u32)r29 == (u32)0x0) {
                        r0 = 0x0;

                    } else {
                        fn_801EF634();
                        r0 = r3 & 0xFFFF;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;
                            goto L_802030EC;
                        }
                        r3 = r29;
                        r4 = 0x0;
                        r5 = 0xd6;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        /* mr. r21, r3 */;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;
                            goto L_802030EC;
                        }
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;

                        } else {
                            fn_801EF634();
                            r0 = r3 & 0xFFFF;
                            if ((u32)r0 == (u32)0x1) {
                                r0 = 0x0;
                                goto L_802030D8;
                            }
                            r3 = r21;
                            r4 = 0x0;
                            r5 = 0xcb;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_802030D8;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_802030D8;
                            }
                            if ((u32)r21 == (u32)0x0) {
                                r3 = 0x0;
                            } else {

                                r3 = r21;
                                r4 = 0x0;
                                r5 = 0xcc;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                            }
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_802030D8;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                goto L_802030D8;
                            }
                            r3 = r21;
                            r4 = 0x0;
                            r5 = 0xce;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((s32)r3 < (s32)0x0) {
                                r0 = 0x0;
                                goto L_802030D8;
                            }
                            r0 = 0x1;
                        }
                        L_802030D8: ;
                        r0 = r0 & 0xFF;
                        if ((s32)r3 == (s32)0x0) {
                            r0 = 0x0;
                            goto L_802030EC;
                        }
                        r0 = 0x1;
                    }
                    L_802030EC: ;
                    r0 = r0 & 0xFF;
                    if ((s32)r3 != (s32)0x0) {
                        r3 = r28;
                        r4 = 0x0;
                        r5 = 0x122;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        r23 = r3;
                        r5 = r29;
                        r4 = 0x4;
                        fn_8020E57C();
                        if ((u32)r3 == (u32)0x0) {
                            r22 = 0x0;
                            while (1) {
                                r0 = r22 & 0xFFFF;
                                if ((u32)r0 >= (u32)0x4) break;
                                r0 = r22 & 0xFFFF;
                                r0 = r0 * 0xc;
                                r21 = r23 + r0;
                                r3 = r21;
                                fn_8020E614();
                                r0 = r3 & 0xFF;
                                if ((u32)r3 == (u32)0x0) {
                                    r3 = r21;
                                    r4 = r29;
                                    fn_8020E640();
                                    break;
                                }
                                r22 = r22 + 0x1;

                            }
                }
                }
                }
                r30 = r30 + 0x1;

            }
        }
        r31 = r31 + 0x1;

    }
    return;
}

/* 0x80203198 | size: 0x14C | medium */
/* 0x80203198 | size: 0x14C */
void fn_80203198(void* ctx, u32 param) {
    extern u16 fn_801FD0EC();
    extern void* fn_8020E57C();
    extern u8 fn_8020E614();
    extern void fn_8020E758();
    void* tableData;
    void* entry;
    void* entryPtr;
    u16 species;
    u8 count;
    u8 i;

    if (!ctx) { return; }
    tableData = fn_8012640C(ctx, 0, 0x122, 0);
    entry = fn_8020E57C(tableData, 4, param);
    if (!entry) { return; }
    species = fn_801FD0EC(entry);
    fn_8020E758(entry);
    if (species == 0 || species == 0x165 || species == 0xFFFF) { return; }
    if ((s32)(u32)fn_8012640C(ctx, 0, 0xF7, 0) != 0) { return; }
    if (ctx != NULL) {
        entryPtr = fn_8012640C(ctx, 0, 0x122, 0);
        for (i = 0; (u8)i < 4; i++) {}
        count = 0;
        for (i = 0; (u8)i < 4; i++) {
            entry = (void*)((u32)entryPtr + (u8)i * 0xC);
            if ((u8)fn_8020E614(entry) == 4) { continue; }
            species = fn_801FD0EC(entry);
            if (species == 4 || species == 0x165) { continue; }
            count++;
        }
    } else {
        count = 0;
    }
    if ((u8)count == 4) {
        fn_801254B4(ctx, 0, 0xF7, 0, (u32)species);
    }
}

/* 0x802032E4 | size: 0x138 */
void fn_802032E4(void* ctx, u32 param) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern void fn_80122370();
    extern u32 fn_80123090();
    void* ccData;
    void* ccCtx;
    u16 typeId;
    u8 result;
    u32 value;

    ccData = !ctx ? NULL : fn_8012640C(ctx, 0, 0xCC, 0);
    if (ccData == NULL) { return; }
    ccCtx = !ctx ? NULL : fn_8012640C(ctx, 0, 0xCC, 0);
    if (ccCtx == NULL) {
        value = 0;
    } else {
        typeId = fn_80119ED0(0x3D);
        if (typeId == 0x7C || typeId == 0xC8) {
            result = fn_80121ADC(!ctx ? NULL : fn_8012640C(ctx, 0, 0xCC, 0), 0x3D);
        } else if (fn_80119ED0(0x3D) == 0xCD) {
            result = fn_8011B67C(ctx, 0x3D);
        } else {
            result = 0;
        }
        if ((u8)result == 1) {
            value = 0;
        } else {
            value = fn_80123090(ccCtx);
        }
    }
    fn_80122370(ccData, value, param);
}

/* 0x8020341C | size: 0x140 */
void fn_8020341C(void* ctx, u32 param1, u32 param2) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern void fn_801226D0();
    extern u32 fn_80123090();
    void* ccData;
    void* ccCtx;
    u16 typeId;
    u8 result;
    u32 value;

    ccData = !ctx ? NULL : fn_8012640C(ctx, 0, 0xCC, 0);
    if (ccData == NULL) { return; }
    ccCtx = !ctx ? NULL : fn_8012640C(ctx, 0, 0xCC, 0);
    if (ccCtx == NULL) {
        value = 0;
    } else {
        typeId = fn_80119ED0(0x3D);
        if (typeId == 0x7C || typeId == 0xC8) {
            result = fn_80121ADC(!ctx ? NULL : fn_8012640C(ctx, 0, 0xCC, 0), 0x3D);
        } else if (fn_80119ED0(0x3D) == 0xCD) {
            result = fn_8011B67C(ctx, 0x3D);
        } else {
            result = 0;
        }
        if ((u8)result == 1) {
            value = 0;
        } else {
            value = fn_80123090(ccCtx);
        }
    }
    fn_801226D0(ccData, value, param1, param2);
}

/* 0x8020355C | size: 0x60 */
void fn_8020355C(u32 obj, u32 param) {
    extern void fn_801229F4();
    extern u32 fn_8012640C();
    u32 result;
    if (obj == 0) {
        result = 0;
    } else {
        result = fn_8012640C(obj, 0, 0xCC, 0);
    }
    if (result != 0) {
        fn_801229F4(result, param);
    }
}

/* 0x802035BC | size: 0x64 */
void fn_802035BC(void* obj, u32 value) {
    void* intermediate;
    if (obj == NULL) {
        intermediate = NULL;
    } else {
        intermediate = fn_8012640C(obj, 0, 0xCC, 0);
    }
    if (intermediate != NULL) {
        fn_801254B4(intermediate, 0, 0x79, 0, value);
    }
}

/* fn_80203620 and fn_8020367C are implemented above as
   ResolveTrainerExtendedData and WriteTrainerExtendedData */

/* 0x802036D4 | size: 0x84 */
u32 fn_802036D4(void* ctx) {
    void* resolved;
    u16 species;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(resolved, 0, 0xCC, 0);
        }
    }
    species = (u16)(u32)fn_8012640C(resolved, 0, 0x6E, 0);
    return (u16)(u32)fn_8012640C(NULL, species, 0x61, 0);
}

/* 0x80203758 | size: 0x84 */
void fn_80203758(void* ctx) {
    extern void fn_800FA280();
    void* resolved;
    u16 species;
    resolved = fn_8012640C(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return;
    }
    species = (u16)(u32)fn_8012640C(resolved, 0, 0x6E, 0);
    resolved = fn_8012640C(NULL, species, 0x01, 0);
    fn_800FA280(resolved);
}

/* 0x802037DC | size: 0x6C */
void* fn_802037DC(void* ctx) {
    void* resolved;
    resolved = fn_8012640C(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return NULL;
    }
    return fn_8012640C(resolved, 0, 0x77, 0);
}

/* 0x80203848 | size: 0x5C | small */
u32 fn_80203848(void* param_1) {
    void* iVar1;
    u32 uVar2;

    if (param_1 == NULL) {
        iVar1 = NULL;
    } else {
        iVar1 = fn_8012640C(param_1, 0, 0xCC, 0);
    }
    if (iVar1 == NULL) {
        uVar2 = 0;
    } else {
        uVar2 = (u32)fn_8012640C(iVar1, 0, 0x77, 0);
    }
    return uVar2;
}

/* 0x802038A4 | size: 0x1C8 | medium */
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

    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
    } else {
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
        } else {
            r3 = r30;
            r4 = 0x0;
            r5 = 0xd6;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            /* mr. r31, r3 */;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x0;
            } else {
                if ((u32)r0 == (u32)0x1) {
                    r0 = 0x0;
                } else {
                    fn_801EF634();
                    r0 = r3 & 0xFFFF;
                    if ((u32)r0 == (u32)0x1) {
                        r0 = 0x0;
                    } else {
                        r3 = r31;
                        r4 = 0x0;
                        r5 = 0xcb;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        if ((u32)r3 == (u32)0x0) {
                            r0 = 0x0;
                        } else {
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                            } else {
                                if ((u32)r31 == (u32)0x0) {
                                    r3 = 0x0;
                                } else {
                                    r3 = r31;
                                    r4 = 0x0;
                                    r5 = 0xcc;
                                    r6 = 0x0;
                                    ((void(*)(void))fn_8012640C)();
                                }
                                if ((u32)r3 == (u32)0x0) {
                                    r0 = 0x0;
                                } else {
                                    fn_80123FBC();
                                    r0 = r3 & 0xFF;
                                    if ((u32)r3 == (u32)0x0) {
                                        r0 = 0x0;
                                    } else {
                                        r3 = r31;
                                        r4 = 0x0;
                                        r5 = 0xce;
                                        r6 = 0x0;
                                        ((void(*)(void))fn_8012640C)();
                                        if ((s32)r3 < (s32)0x0) {
                                            r0 = 0x0;
                                        } else {
                                            r0 = 0x1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                r0 = r0 & 0xFF;
                if ((s32)r3 == (s32)0x0) {
                    r0 = 0x0;
                } else {
                    r0 = 0x1;
                }
            }
        }
    }
    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x1;
        return;
    }
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x0;
    } else {
        r3 = r30;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 != (u32)0x0) {
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        }
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x1;
        return;
    }
    r4 = 0x0;
    r5 = 0x7b;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFF;

    return;
}

/* 0x80203A6C | size: 0x70 */
u32 fn_80203A6C(void* ctx) {
    extern u32 fn_80122A70();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return fn_80122A70(resolved);
}

/* 0x80203ADC | size: 0x80 */
void fn_80203ADC(void* ctx, u32 param) {
    extern void fn_80122AE0();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved != NULL) {
        fn_80122AE0(resolved, param);
    }
}

/* 0x80203B5C | size: 0x80 */
void fn_80203B5C(void* ctx, u32 param) {
    extern void fn_80122B50();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved != NULL) {
        fn_80122B50(resolved, param);
    }
}

/* 0x80203BDC | size: 0x80 */
void fn_80203BDC(void* ctx, u32 param) {
    extern void fn_80122BC0();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved != NULL) {
        fn_80122BC0(resolved, param);
    }
}

/* 0x80203C5C | size: 0x70 */
u32 fn_80203C5C(void* ctx) {
    extern u32 fn_80122C64();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return fn_80122C64(resolved);
}

/* 0x80203CCC | size: 0x70 */
u32 fn_80203CCC(void* ctx) {
    extern u32 fn_80122DDC();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = fn_8012640C(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return fn_80122DDC(resolved);
}

/* 0x80203D3C | size: 0x70 */
u16 fn_80203D3C(void* ctx) {
    void* resolved;
    resolved = fn_8012640C(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return (u16)(u32)fn_8012640C(resolved, 0, 0x6E, 0);
}

/* 0x80203DAC | size: 0x60 */
u16 fn_80203DAC(void* ctx) {
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(ctx, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return (u16)(u32)fn_8012640C(resolved, 0, 0x6E, 0);
}

/* 0x80203E0C | size: 0x70 */
u8 fn_80203E0C(void* ctx) {
    void* resolved;
    resolved = fn_8012640C(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = fn_8012640C(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return (u8)(u32)fn_8012640C(resolved, 0, 0x7A, 0);
}

/* 0x80203E7C | size: 0x60 */
u32 fn_80203E7C(u32 obj) {
    extern u32 fn_8012640C();
    u32 result;
    if (obj == 0) {
        result = 0;
    } else {
        result = fn_8012640C(obj, 0, 0xCC, 0);
    }
    if (result == 0) {
        return 0;
    }
    return fn_8012640C(result, 0, 0x7A, 0) & 0xFF;
}

/* 0x80203EDC | size: 0x108 */
u16 fn_80203EDC(void* ctx) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern u16 fn_80122FF4();
    void* d6Data;
    void* ccData;
    u16 typeId;
    u8 result;

    d6Data = fn_8012640C(ctx, 0, 0xD6, 0);
    ccData = !d6Data ? NULL : fn_8012640C(d6Data, 0, 0xCC, 0);
    if (ccData == NULL) { return 0; }
    typeId = fn_80119ED0(0x3D);
    if (typeId == 0x7C || typeId == 0xC8) {
        result = fn_80121ADC(!d6Data ? NULL : fn_8012640C(d6Data, 0, 0xCC, 0), 0x3D);
    } else if (fn_80119ED0(0x3D) == 0xCD) {
        result = fn_8011B67C(d6Data, 0x3D);
    } else {
        result = 0;
    }
    if ((u8)result == 1) { return 0; }
    return fn_80122FF4(ccData);
}

/* 0x80203FE4 | size: 0x104 */
u32 fn_80203FE4(void* ctx) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern u32 fn_80123090();
    void* d6Data;
    void* ccData;
    u16 typeId;
    u8 result;

    d6Data = fn_8012640C(ctx, 0, 0xD6, 0);
    ccData = !d6Data ? NULL : fn_8012640C(d6Data, 0, 0xCC, 0);
    if (ccData == NULL) { return 0; }
    typeId = fn_80119ED0(0x3D);
    if (typeId == 0x7C || typeId == 0xC8) {
        result = fn_80121ADC(!d6Data ? NULL : fn_8012640C(d6Data, 0, 0xCC, 0), 0x3D);
    } else if (fn_80119ED0(0x3D) == 0xCD) {
        result = fn_8011B67C(d6Data, 0x3D);
    } else {
        result = 0;
    }
    if ((u8)result == 1) { return 0; }
    return fn_80123090(ccData);
}

/* 0x802041EC | size: 0xF4 | medium */
u32 fn_802041EC(void* param_1) {
    extern s16 fn_80119ED0(u32);
    extern s8 fn_8011B67C(void*, u32);
    extern s8 fn_80121ADC(void*, u32);
    extern u32 fn_80123090(void*);
    u32 uVar1;
    s16 sVar2;
    s8 cVar3;
    void* iVar4;

    if (param_1 == NULL) {
        iVar4 = NULL;
    } else {
        iVar4 = fn_8012640C(param_1, 0, 0xCC, 0);
    }
    if (iVar4 == NULL) {
        uVar1 = 0;
    } else {
        sVar2 = fn_80119ED0(0x3D);
        if ((sVar2 == 0x7C) || (sVar2 = fn_80119ED0(0x3D), sVar2 == 200)) {
            if (param_1 == NULL) {
                uVar1 = 0;
            } else {
                uVar1 = (u32)fn_8012640C(param_1, 0, 0xCC, 0);
            }
            cVar3 = fn_80121ADC((void*)uVar1, 0x3D);
        } else {
            sVar2 = fn_80119ED0(0x3D);
            if (sVar2 == 0xCD) {
                cVar3 = fn_8011B67C(param_1, 0x3D);
            } else {
                cVar3 = 0;
            }
        }
        if (cVar3 == 1) {
            uVar1 = 0;
        } else {
            uVar1 = fn_80123090(iVar4);
        }
    }
    return uVar1;
}

/* 0x802042E0 | size: 0xF4 | medium */
u32 fn_802042E0(void* param_1) {
    extern s16 fn_80119ED0(u32);
    extern s8 fn_8011B67C(void*, u32);
    extern s8 fn_80121ADC(void*, u32);
    extern u32 fn_801230E0(void*);
    u32 uVar1;
    s16 sVar2;
    s8 cVar3;
    void* iVar4;

    if (param_1 == NULL) {
        iVar4 = NULL;
    } else {
        iVar4 = fn_8012640C(param_1, 0, 0xCC, 0);
    }
    if (iVar4 == NULL) {
        uVar1 = 0;
    } else {
        sVar2 = fn_80119ED0(0x3D);
        if ((sVar2 == 0x7C) || (sVar2 = fn_80119ED0(0x3D), sVar2 == 200)) {
            if (param_1 == NULL) {
                uVar1 = 0;
            } else {
                uVar1 = (u32)fn_8012640C(param_1, 0, 0xCC, 0);
            }
            cVar3 = fn_80121ADC((void*)uVar1, 0x3D);
        } else {
            sVar2 = fn_80119ED0(0x3D);
            if (sVar2 == 0xCD) {
                cVar3 = fn_8011B67C(param_1, 0x3D);
            } else {
                cVar3 = 0;
            }
        }
        if (cVar3 == 1) {
            uVar1 = 0;
        } else {
            uVar1 = fn_801230E0(iVar4);
        }
    }
    return uVar1;
}

/* 0x802043D4 | size: 0x480 | large */
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

    /* mr. r31, r3 */;
    r24 = r4;
    r26 = r5;
    r23 = r6;
    r27 = r7;
    if ((s32)r0 == (s32)0) {
        r29 = 0x0;
    } else {

        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
        } else {

            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        }
        r29 = r3;
    }
    if ((u32)r29 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r29 == (u32)0x0) {
        r25 = 0x0;
    } else {

        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r25 = r3;
    }
    do {
        if ((u32)r25 == (u32)0x0) {
            r30 = 0x0;
            break;
        }
        r3 = 0x3d;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        do {
            if ((u32)r0 != (u32)0x7c) {
                r3 = 0x3d;
                fn_80119ED0();
                r0 = r3 & 0xFFFF;
                if ((u32)r0 == (u32)0xc8) {
                }
                if ((u32)r30 == (u32)0x0) {
                    r3 = 0x0;
                } else {

                    r3 = r30;
                    r4 = 0x0;
                    r5 = 0xcc;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                }
                r4 = 0x3d;
                fn_80121ADC();
                break;
                }
            r3 = 0x3d;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0xcd) {
                r3 = 0x0;
                break;
            }
            r3 = r30;
            r4 = 0x3d;
            fn_8011B67C();
        } while (0);

        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r30 = 0x0;
            break;
        }
        r3 = r25;
        fn_80123090();
        r30 = r3;
    } while (0);

    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r22, r3 */;
    if ((u32)r0 == (u32)0x1) {
        r25 = 0x0;
    } else {

        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r25 = r3;
    }
    do {
        if ((u32)r25 == (u32)0x0) {
            r25 = 0x0;
            break;
        }
        r3 = 0x3d;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        do {
            if ((u32)r0 != (u32)0x7c) {
                r3 = 0x3d;
                fn_80119ED0();
                r0 = r3 & 0xFFFF;
                if ((u32)r0 == (u32)0xc8) {
                }
                if ((u32)r22 == (u32)0x0) {
                    r3 = 0x0;
                } else {

                    r3 = r22;
                    r4 = 0x0;
                    r5 = 0xcc;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                }
                r4 = 0x3d;
                fn_80121ADC();
                break;
                }
            r3 = 0x3d;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0xcd) {
                r3 = 0x0;
                break;
            }
            r3 = r22;
            r4 = 0x3d;
            fn_8011B67C();
        } while (0);

        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r25 = 0x0;
            break;
        }
        r3 = r25;
        fn_80122FF4();
        r25 = r3 & 0xFFFF;
    } while (0);

    r3 = r31;
    r4 = 0x0;
    r5 = 0xea;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r22 = r3 & 0xFF;
    if ((u32)r27 != (u32)0x0) {
        r3 = r27;
        r4 = 0x11;
        r5 = 0x0;
        fn_8012A5B0();
        r27 = r3 & 0xFF;
    } else {

        r27 = 0x0;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x8c;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r4 = r3;
    if ((u32)r28 == (u32)0x21) {
        r0 = r26 & 0xFF;
        if ((u32)r0 == (u32)0x2) {
            r4 = r3 << 1;
            goto L_802046BC;
    }
    }
    if ((u32)r28 == (u32)0x22) {
        r0 = r26 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r4 = r4 << 1;
    }
    }
    L_802046BC: ;
    r3 = r22;
    fn_8020E4E8();
    r0 = r24 & 0xFF;
    r24 = r3;
    if ((u32)r0 == (u32)0x1) {
        r0 = r27 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = r24 * 0x6e;
            r0 = 0x64;
            r24 = (u32)r3 / (u32)r0;
    }
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x18) {
        r24 = (u32)r24 >> 1;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x7c) {
        r3 = 0x5;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xc8) {
            r3 = 0x5;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0xcd) {
        }
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r22 = r3;
        r3 = 0x5;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x7c) {
            r3 = 0x5;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0xc8) {
            }
            if ((u32)r22 == (u32)0x0) {
                r3 = 0x0;
            } else {

                r3 = r22;
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
            }
            r4 = 0x5;
            fn_80121ADC();
            goto L_802047FC;
            }
        r3 = 0x5;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xcd) {
            r3 = 0x0;
            goto L_802047FC;
        }
        r3 = r22;
        r4 = 0x5;
        fn_8011B67C();
            } else {

        r3 = 0x5;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xd8) {
            r3 = 0x0;
            goto L_802047FC;
        }
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
            }
    L_802047FC: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r24 = (u32)r24 >> 2;
    }
    r0 = r30 & 0xFFFF;
    if ((u32)r0 == (u32)0x1a) {
        r3 = (0x1 << 16);
        r0 = 0x64;
        /* subi r3, r3, 0x1 */;
        r4 = r23 & 0xFFFF;
        r3 = r25 * r3;
        r0 = (s32)r3 / (s32)r0;
        if ((s32)r4 < (s32)r0) {
            r24 = -0x1;
    }
    }
    r3 = r24;

    return;
}

/* 0x80204854 | size: 0xD4 | medium */
u32 fn_80204854(void* param_1, void* param_2) {
    extern s8 fn_801F1170(void*);
    extern s16 fn_8020D8D8(void*);
    extern s16 fn_8020D950(void*);
    s16 sVar3;
    u32 uVar1;
    s16 sVar4;
    void* iVar2;
    s8 cVar5;

    sVar3 = (s16)(u32)fn_8012640C(param_2, 0, 0xCE, 0);
    if (sVar3 < 0) {
        uVar1 = 0;
    } else {
        sVar4 = (s16)(u32)fn_8012640C(param_1, 0, 0x121, 0);
        if (sVar3 == sVar4) {
            uVar1 = 1;
        } else {
            iVar2 = fn_8012640C(param_1, 0, 0xFE, 0);
            if ((((iVar2 == NULL) || (cVar5 = fn_801F1170(iVar2), cVar5 != 1)) ||
                (sVar4 = fn_8020D950(iVar2), sVar4 != 9)) ||
               (sVar4 = fn_8020D8D8(iVar2), sVar3 != sVar4)) {
                uVar1 = 0;
            } else {
                uVar1 = 1;
            }
        }
    }
    return uVar1;
}

/* 0x80204928 | size: 0x48 | small */
u32 fn_80204928(u32 expected, void* ctx) {
    u32 result = (u32)fn_8012640C(ctx, 0, 0xd5, 0);
    return (expected == result) ? 1 : 0;
}

/* 0x80204970 | size: 0xA0 | medium */
void fn_80204970(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if (((u32)r3 != (u32)0x0) && ((u32)r4 != (u32)0x0)) {

        r0 = 0x2a;
        r8 = (u32)sp + 0x4;
        /* subi r6, r3, 0x4 */;
        ctr_fn = (void(*)(void))r0;
        do {
            r5 = *(u32*)((u8*)r6 + 0x4);
            r0 = *(u32*)((u8*)r6 + 0x8);
            *(u32*)((u8*)r8 + 0x4) = r5;
            r8 += 8; *(u32*)r8 = r0;
        } while (--ctr != 0);
        r7 = *(u32*)((u8*)r6 + 0x4);
        r0 = 0x2a;
        /* subi r6, r3, 0x4 */;
        /* subi r5, r4, 0x4 */;
        *(u32*)((u8*)r8 + 0x4) = r7;
        ctr_fn = (void(*)(void))r0;
        do {
            r3 = *(u32*)((u8*)r5 + 0x4);
            r0 = *(u32*)((u8*)r5 + 0x8);
            *(u32*)((u8*)r6 + 0x4) = r3;
            r6 += 8; *(u32*)r6 = r0;
        } while (--ctr != 0);
        r3 = *(u32*)((u8*)r5 + 0x4);
        r0 = 0x2a;
        /* subi r5, r4, 0x4 */;
        r4 = (u32)sp + 0x4;
        *(u32*)((u8*)r6 + 0x4) = r3;
        ctr_fn = (void(*)(void))r0;
        do {
            r3 = *(u32*)((u8*)r4 + 0x4);
            r0 = *(u32*)((u8*)r4 + 0x8);
            *(u32*)((u8*)r5 + 0x4) = r3;
            r5 += 8; *(u32*)r5 = r0;
        } while (--ctr != 0);
        r0 = *(u32*)((u8*)r4 + 0x4);
        *(u32*)((u8*)r5 + 0x4) = r0;
    }
    return;
}

/* fn_80204A10 | Size: 0x4C | Check if trainer slot is active */
BOOL fn_80204A10(u32 slotId) {
    extern void* fn_801F4354(u32 context, u32 slot);
    extern u8 fn_801FB8F8(void* trainer);
    void* trainer = fn_801F4354(0, slotId);
    if (trainer == NULL) {
        return FALSE;
    }
    return fn_801FB8F8(trainer) == 1;
}

/* 0x80204A5C | size: 0x1AC | medium */
/* 0x80204A5C | size: 0x1AC */
u32 fn_80204A5C(void* ctx, u8 targetSlot, u8 mode) {
    extern u32 lbl_80478BD8;
    extern u8 fn_80142984();
    extern u16 fn_801F0898();
    extern u8 fn_801F1170();
    extern void fn_801F54A4();
    u32 i;
    u8 slotType;
    void* feData;
    void* e5Data;
    u16 field1E;
    u8 valid;

    for (i = 0; (u16)i < lbl_80478BD8; i++) {
        if ((u8)fn_80142984(i) == 0) { continue; }
        slotType = (u8)fn_80142CF4(0, i, 0x2, 0);
        if ((u8)mode == 1) {
            if ((u8)targetSlot != slotType) { continue; }
        } else {
            if ((u8)targetSlot == slotType) { continue; }
        }
        fn_801F54A4(0, 0, 0x14, 0);
        if (!ctx) { valid = 0; }
        else {
            feData = fn_8012640C(ctx, 0, 0xFE, 0);
            if (!feData) { valid = 0; }
            else if ((u8)fn_801F1170(feData) == 0) { valid = 0; }
            else if (fn_801F0898(feData) != 0x12) { valid = 0; }
            else {
                e5Data = fn_8012640C(ctx, 0, 0xE5, 0);
                if (!e5Data) { valid = 0; }
                else {
                    field1E = (u16)fn_80142CF4((u32)e5Data, 0, 0x1E, 0);
                    if ((u16)i == 0x12 || field1E == (u16)i) {
                        valid = 1;
                    } else {
                        valid = 0;
                    }
                }
            }
        }
        if ((u8)valid == 1) { return 1; }
    }
    return 0;
}

/* 0x80204C08 | size: 0xD8 | medium */
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
    r30 = r3;
    r3 = 0x0;
    fn_801F54A4();
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_801F1170();
    r0 = r3 & 0xFF;
    if ((u32)r30 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    fn_801F0898();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x12) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xe5;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0x1e;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    r3 = r3 & 0xFFFF;

    return;
}

/* 0x80204CE0 | size: 0x104 */
void* fn_80204CE0(void* ctx, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7, u32 p8, u8 p9) {
    extern void fn_80142B24();
    extern u8 fn_801F11CC();
    extern void fn_8020A398();
    extern void fn_8020D878();
    void* e5Data;
    void* feData;

    e5Data = fn_8012640C(ctx, 0, 0xE5, 0);
    if (!e5Data) { return NULL; }
    fn_8020A398(e5Data, (u16)p6, p7, p8);
    fn_80142B24(e5Data, 0, 0x21, 0, (u32)p9);
    feData = fn_8012640C(ctx, 0, 0xFE, 0);
    if (!feData) { feData = NULL; }
    else {
        if ((u8)fn_801F11CC(feData, p2, ctx, p3, p4, p5) == 1) {
            fn_8020D878(feData, p6);
        } else {
            feData = NULL;
        }
    }
    return feData;
}

/* 0x80204DE4 | size: 0x188 */
u32 fn_80204DE4(void* ctx, u16 slotId, void* tablePtr) {
    extern u16 fn_8011BEB4();
    extern void* fn_801F0134();
    extern void* fn_801F025C();
    extern u16 fn_801F0898();
    extern u8 fn_801F1170();
    extern u16 fn_801F54A4();
    u16 partyCount;
    void* savedEntry;
    void* feData;
    void* d9Data;
    u16 field27;
    u16 field09;
    u16 field29;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    if (!ctx) { return 0; }
    savedEntry = !tablePtr ? NULL : fn_801F0134(tablePtr, partyCount);
    feData = fn_8012640C(ctx, 0, 0xFE, 0);
    if (!feData) { return 0; }
    if ((u8)fn_801F1170(feData) == 0) { return 0; }
    if (fn_801F0898(feData) != 0x13) { return 0; }
    d9Data = fn_8012640C(ctx, 0, 0xD9, 0);
    if (!d9Data) { return 0; }
    field27 = fn_8011BEB4(d9Data, 0, 0x27, 0);
    field09 = fn_8011BEB4(0, field27, 0x9, 0);
    if ((u16)slotId != 0x13 && field27 != (u16)slotId) { return 0; }
    field29 = fn_8011BEB4(d9Data, 0, 0x29, 0);
    if (field09 == 0xB0) {
        savedEntry = fn_801F0134(fn_801F025C(0xE, ctx), partyCount);
    }
    if (field09 == 0xB0) { return 1; }
    if ((u16)(u32)savedEntry == field29) { return 1; }
    return 0;
}

/* 0x80204F6C | size: 0xF0 | medium */
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

    r23 = r4;
    r24 = r5;
    r25 = r6;
    r30 = *(u8*)((u8*)(u32)sp + 0x3B);
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
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
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
    do {
        if ((u32)r3 == (u32)0x0) {
            r31 = 0x0;
            break;
        }
        r4 = r23;
        r5 = r22;
        r6 = r24;
        r7 = r25;
        r8 = r26;
        fn_801F11CC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = r31;
            r4 = r27;
            fn_8020D878();
            break;
        }
        r31 = 0x0;
    } while (0);

    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;

    return;
}

/* 0x8020505C | size: 0x98 | medium */
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
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = r26;
    r5 = r25;
    r6 = r27;
    r7 = r28;
    r8 = r29;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = r30;
        fn_8020D878();
        r3 = r31;
        return;
    }
    r3 = 0x0;

    return;
}

/* fn_80205134 | Size: 0x50 | Get field 0x30 from resolved 0xD9, default 9 */
u16 fn_80205134(void* ctx) {
    extern u16 fn_8011BEB4(void* ctx, u32 slot, u16 field, u32 flags);
    void* resolved = fn_8012640C(ctx, 0, 0xD9, 0);
    if (resolved == NULL) {
        return 9;
    }
    return fn_8011BEB4(resolved, 0, 0x30, 0);
}

/* fn_802051D4 | Size: 0x50 | Get field 0x27 from resolved 0xD9, default 0 */
u16 fn_802051D4(void* ctx) {
    extern u16 fn_8011BEB4(void* ctx, u32 slot, u16 field, u32 flags);
    void* resolved = fn_8012640C(ctx, 0, 0xD9, 0);
    if (resolved == NULL) {
        return 0;
    }
    return fn_8011BEB4(resolved, 0, 0x27, 0);
}

/* fn_80205224 | Size: 0x50 | Get field 0x27 from resolved 0xD9, default 0 */
u16 fn_80205224(void* ctx) {
    extern u16 fn_8011BEB4(void* ctx, u32 slot, u16 field, u32 flags);
    void* resolved = fn_8012640C(ctx, 0, 0xD9, 0);
    if (resolved == NULL) {
        return 0;
    }
    return fn_8011BEB4(resolved, 0, 0x27, 0);
}

/* 0x80205274 | size: 0x690 | large */
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
    while (1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 >= (u32)r24) break;
        r3 = r27;
        r6 = r29;
        r4 = 0x0;
        r5 = 0x7;
        fn_801F76B8();
        r28 = r3;
        fn_801FA634();
        r0 = r3 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r30 = 0x0;
            while (1) {
                r0 = r30 & 0xFFFF;
                if ((u32)r0 >= (u32)r26) break;
                r3 = r28;
                r6 = r30;
                r4 = 0x0;
                r5 = 0x46;
                fn_801FB1C0();
                /* mr. r25, r3 */;
                do {
                    if ((s32)r0 == (s32)0) {
                        r0 = 0x0;
                        break;
                    }
                    do {
                        if ((s32)r0 == (s32)0) {
                            r0 = 0x0;
                            break;
                        }
                        fn_801EF634();
                        r0 = r3 & 0xFFFF;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;
                            break;
                        }
                        r3 = r25;
                        r4 = 0x0;
                        r5 = 0xd6;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        /* mr. r23, r3 */;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = 0x0;
                            break;
                        }
                        do {
                            if ((u32)r0 == (u32)0x1) {
                                r0 = 0x0;
                                break;
                            }
                            fn_801EF634();
                            r0 = r3 & 0xFFFF;
                            if ((u32)r0 == (u32)0x1) {
                                r0 = 0x0;
                                break;
                            }
                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xcb;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            if ((u32)r23 == (u32)0x0) {
                                r3 = 0x0;
                            } else {

                                r3 = r23;
                                r4 = 0x0;
                                r5 = 0xcc;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                            }
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xce;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((s32)r3 < (s32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            r0 = 0x1;
                        } while (0);

                        r0 = r0 & 0xFF;
                        if ((s32)r3 == (s32)0x0) {
                            r0 = 0x0;
                            break;
                        }
                        r0 = 0x1;
                    } while (0);

                    r0 = r0 & 0xFF;
                    if ((s32)r3 == (s32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r3 = r25;
                    r4 = 0x0;
                    r5 = 0x120;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    if ((s32)r3 == (s32)0x1) {
                        r0 = 0x0;
                        break;
                    }
                    r3 = r25;
                    r4 = 0x0;
                    r5 = 0xd6;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    /* mr. r23, r3 */;
                    do {
                        if ((s32)r3 == (s32)0x1) {
                            r0 = 0x0;
                            break;
                        }
                        do {
                            if ((s32)r3 == (s32)0x1) {
                                r0 = 0x0;
                                break;
                            }
                            fn_801EF634();
                            r0 = r3 & 0xFFFF;
                            if ((u32)r0 == (u32)0x1) {
                                r0 = 0x0;
                                break;
                            }
                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xcb;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            if ((u32)r23 == (u32)0x0) {
                                r3 = 0x0;
                            } else {

                                r3 = r23;
                                r4 = 0x0;
                                r5 = 0xcc;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                            }
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xce;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((s32)r3 < (s32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            r0 = 0x1;
                        } while (0);

                        r0 = r0 & 0xFF;
                        if ((s32)r3 == (s32)0x0) {
                            r0 = 0x0;
                            break;
                        }
                        r3 = r23;
                        r4 = 0x0;
                        r5 = 0xd2;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        if ((s32)r3 == (s32)0x1) {
                            r0 = 0x0;
                            break;
                        }
                        if ((u32)r23 == (u32)0x0) {
                            r3 = 0x0;
                        } else {

                            r3 = r23;
                            r4 = 0x0;
                            r5 = 0xcc;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                        }
                        if ((u32)r3 == (u32)0x0) {
                            r0 = 0x0;
                            break;
                        }
                        fn_801233F4();
                        r0 = r3 & 0xFF;
                        if ((u32)r3 == (u32)0x0) {
                            r0 = 0x0;
                            break;
                        }
                        r0 = 0x1;
                    } while (0);

                    r0 = r0 & 0xFF;
                    if ((u32)r3 == (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r0 = 0x1;
                } while (0);

                r0 = r0 & 0xFF;
                if ((u32)r3 != (u32)0x0) {
                    r3 = r25;
                    r4 = 0x0;
                    r5 = 0xd5;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    r25 = r3;
                    if ((u32)r31 != (u32)0x0) {
                        do {
                            if ((u32)r25 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            fn_801EF634();
                            r0 = r3 & 0xFFFF;
                            if ((u32)r0 == (u32)0x1) {
                                r0 = 0x0;
                                break;
                            }
                            r3 = r25;
                            r4 = 0x0;
                            r5 = 0xcb;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            if ((u32)r25 == (u32)0x0) {
                                r3 = 0x0;
                            } else {

                                r3 = r25;
                                r4 = 0x0;
                                r5 = 0xcc;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                            }
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            fn_80123FBC();
                            r0 = r3 & 0xFF;
                            if ((u32)r3 == (u32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            r3 = r25;
                            r4 = 0x0;
                            r5 = 0xce;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((s32)r3 < (s32)0x0) {
                                r0 = 0x0;
                                break;
                            }
                            r0 = 0x1;
                        } while (0);

                        r0 = r0 & 0xFF;
                        if ((s32)r3 != (s32)0x0) {
                            do {
                                if ((u32)r31 == (u32)0x0) {
                                    r0 = 0x0;
                                    break;
                                }
                                do {
                                    if ((u32)r25 == (u32)0x0) {
                                        r0 = 0x0;
                                        break;
                                    }
                                    fn_801EF634();
                                    r0 = r3 & 0xFFFF;
                                    if ((u32)r0 == (u32)0x1) {
                                        r0 = 0x0;
                                        break;
                                    }
                                    r3 = r25;
                                    r4 = 0x0;
                                    r5 = 0xcb;
                                    r6 = 0x0;
                                    ((void(*)(void))fn_8012640C)();
                                    if ((u32)r3 == (u32)0x0) {
                                        r0 = 0x0;
                                        break;
                                    }
                                    fn_80123FBC();
                                    r0 = r3 & 0xFF;
                                    if ((u32)r3 == (u32)0x0) {
                                        r0 = 0x0;
                                        break;
                                    }
                                    if ((u32)r25 == (u32)0x0) {
                                        r3 = 0x0;
                                    } else {

                                        r3 = r25;
                                        r4 = 0x0;
                                        r5 = 0xcc;
                                        r6 = 0x0;
                                        ((void(*)(void))fn_8012640C)();
                                    }
                                    if ((u32)r3 == (u32)0x0) {
                                        r0 = 0x0;
                                        break;
                                    }
                                    fn_80123FBC();
                                    r0 = r3 & 0xFF;
                                    if ((u32)r3 == (u32)0x0) {
                                        r0 = 0x0;
                                        break;
                                    }
                                    r3 = r25;
                                    r4 = 0x0;
                                    r5 = 0xce;
                                    r6 = 0x0;
                                    ((void(*)(void))fn_8012640C)();
                                    if ((s32)r3 < (s32)0x0) {
                                        r0 = 0x0;
                                        break;
                                    }
                                    r0 = 0x1;
                                } while (0);

                                r0 = r0 & 0xFF;
                                if ((s32)r3 == (s32)0x0) {
                                    r0 = 0x0;
                                    break;
                                }
                                r3 = r25;
                                r4 = 0x0;
                                r5 = 0xce;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                                r22 = (s16)r3;
                                r23 = 0x0;
                                while (1) {
                                    r0 = r23 & 0xFF;
                                    if ((u32)r0 >= (u32)0xc) break;
                                    r3 = r31;
                                    r6 = r23 & 0xFF;
                                    r4 = 0x0;
                                    r5 = 0xfd;
                                    ((void(*)(void))fn_8012640C)();
                                    r0 = (s16)r3;
                                    if ((s32)r3 < (s32)0x0 || (s32)r0 == (s32)r22) {

                                        r0 = 0x1;
                                        break;
                                    }
                                    r23 = r23 + 0x1;

                                }
                                r0 = 0x0;
                            } while (0);

                            r0 = r0 & 0xFF;
                            if ((u32)r0 != (u32)0x1) {
                                r3 = r25;
                                r4 = 0x0;
                                r5 = 0xce;
                                r6 = 0x0;
                                ((void(*)(void))fn_8012640C)();
                                r25 = (s16)r3;
                                r23 = 0x0;
                                while (1) {
                                    r0 = r23 & 0xFF;
                                    if ((u32)r0 >= (u32)0xc) break;
                                    r22 = r23 & 0xFF;
                                    r3 = r31;
                                    r6 = r22;
                                    r4 = 0x0;
                                    r5 = 0xfd;
                                    ((void(*)(void))fn_8012640C)();
                                    r0 = (s16)r3;
                                    if ((u32)r0 < (u32)0x1) {
                                        r3 = r31;
                                        r6 = r22;
                                        r7 = r25;
                                        r4 = 0x0;
                                        r5 = 0xfd;
                                        ((void(*)(void))fn_801254B4)();
                                        break;
                                    }
                                    r23 = r23 + 0x1;

                                }
                }
                }
                }
                }
                r30 = r30 + 0x1;

            }
        }
        r29 = r29 + 0x1;

    }
    return;
}

/* 0x80205904 | size: 0x178 | medium */
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

    /* mr. r29, r3 */;
    r30 = r4;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((u32)r30 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            break;
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xcb;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        if ((u32)r30 == (u32)0x0) {
            r3 = 0x0;
        } else {

            r3 = r30;
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        }
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xce;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((s32)r3 < (s32)0x0) {
            r0 = 0x0;
            break;
        }
        r0 = 0x1;
    } while (0);

    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r31 = (s16)r3;
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFF;
        if ((u32)r0 >= (u32)0xc) break;
        r3 = r29;
        r6 = r30 & 0xFF;
        r4 = 0x0;
        r5 = 0xfd;
        ((void(*)(void))fn_8012640C)();
        r0 = (s16)r3;
        if (((s32)r3 >= (s32)0x0) && ((s32)r0 == (s32)r31)) {

            r3 = 0x1;
            return;
        }
        r30 = r30 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* fn_80205A7C | Size: 0x58 | Two-hop resolve and call fn_801232E0 */
void fn_80205A7C(void* ctx, u32 param) {
    extern void fn_801232E0(void* obj, u32 param);
    if (ctx == NULL) {
        return;
    }
    ctx = fn_8012640C(ctx, 0, 0xD5, 0);
    ctx = fn_8012640C(ctx, 0, 0xCB, 0);
    fn_801232E0(ctx, param);
}

/* fn_80205AD4 | Size: 0x58 | Two-hop resolve and call fn_80123368 */
void fn_80205AD4(void* ctx, u32 param) {
    extern void fn_80123368(void* obj, u32 param);
    if (ctx == NULL) {
        return;
    }
    ctx = fn_8012640C(ctx, 0, 0xD5, 0);
    ctx = fn_8012640C(ctx, 0, 0xCB, 0);
    fn_80123368(ctx, param);
}

/* fn_80205B2C | Size: 0x60 | Two-hop resolve (0xD5 -> 0xCE), return s16 or -1 */
s16 fn_80205B2C(void* ctx) {
    void* hop1;
    if (ctx == NULL) {
        return -1;
    }
    hop1 = fn_8012640C(ctx, 0, 0xD5, 0);
    if (hop1 == NULL) {
        return -1;
    }
    return (s16)(u32)fn_8012640C(hop1, 0, 0xCE, 0);
}

/* 0x80205BE8 | size: 0x3C | small */
void* fn_80205BE8(void* ctx) {
    if (ctx == 0) return 0;
    return fn_8012640C(ctx, 0, 0xcc, 0);
}

/* 0x80205C24 | size: 0x684 | large */
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

    r5 = 0x14;
    r6 = 0x0;
    r31 = r3;
    r29 = r4;
    r3 = 0x0;
    r4 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((u32)r31 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        do {
            if ((u32)r31 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            fn_801EF634();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x0;
                break;
            }
            r3 = r31;
            r4 = 0x0;
            r5 = 0xd6;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            /* mr. r28, r3 */;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x0;
                break;
            }
            do {
                if ((u32)r0 == (u32)0x1) {
                    r0 = 0x0;
                    break;
                }
                fn_801EF634();
                r0 = r3 & 0xFFFF;
                if ((u32)r0 == (u32)0x1) {
                    r0 = 0x0;
                    break;
                }
                r3 = r28;
                r4 = 0x0;
                r5 = 0xcb;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                fn_80123FBC();
                r0 = r3 & 0xFF;
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                if ((u32)r28 == (u32)0x0) {
                    r3 = 0x0;
                } else {

                    r3 = r28;
                    r4 = 0x0;
                    r5 = 0xcc;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                }
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                fn_80123FBC();
                r0 = r3 & 0xFF;
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r3 = r28;
                r4 = 0x0;
                r5 = 0xce;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((s32)r3 < (s32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r0 = 0x1;
            } while (0);

            r0 = r0 & 0xFF;
            if ((s32)r3 == (s32)0x0) {
                r0 = 0x0;
                break;
            }
            r0 = 0x1;
        } while (0);

        r0 = r0 & 0xFF;
        if ((s32)r3 == (s32)0x0) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0x120;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((s32)r3 == (s32)0x1) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        /* mr. r28, r3 */;
        do {
            if ((s32)r3 == (s32)0x1) {
                r0 = 0x0;
                break;
            }
            do {
                if ((s32)r3 == (s32)0x1) {
                    r0 = 0x0;
                    break;
                }
                fn_801EF634();
                r0 = r3 & 0xFFFF;
                if ((u32)r0 == (u32)0x1) {
                    r0 = 0x0;
                    break;
                }
                r3 = r28;
                r4 = 0x0;
                r5 = 0xcb;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                fn_80123FBC();
                r0 = r3 & 0xFF;
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                if ((u32)r28 == (u32)0x0) {
                    r3 = 0x0;
                } else {

                    r3 = r28;
                    r4 = 0x0;
                    r5 = 0xcc;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                }
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                fn_80123FBC();
                r0 = r3 & 0xFF;
                if ((u32)r3 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r3 = r28;
                r4 = 0x0;
                r5 = 0xce;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((s32)r3 < (s32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r0 = 0x1;
            } while (0);

            r0 = r0 & 0xFF;
            if ((s32)r3 == (s32)0x0) {
                r0 = 0x0;
                break;
            }
            r3 = r28;
            r4 = 0x0;
            r5 = 0xd2;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == (s32)0x1) {
                r0 = 0x0;
                break;
            }
            if ((u32)r28 == (u32)0x0) {
                r3 = 0x0;
            } else {

                r3 = r28;
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
            }
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            fn_801233F4();
            r0 = r3 & 0xFF;
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            r0 = 0x1;
        } while (0);

        r0 = r0 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        r0 = 0x1;
    } while (0);

    r0 = r0 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = 0x12;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x7c) {
        r3 = 0x12;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xc8) {
            r3 = 0x12;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0xcd) {
        }
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r28 = r3;
        r3 = 0x12;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x7c) {
            r3 = 0x12;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0xc8) {
            }
            if ((u32)r28 == (u32)0x0) {
                r3 = 0x0;
            } else {

                r3 = r28;
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
            }
            r4 = 0x12;
            fn_80121ADC();
            goto L_80206060;
            }
        r3 = 0x12;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xcd) {
            r3 = 0x0;
            goto L_80206060;
        }
        r3 = r28;
        r4 = 0x12;
        fn_8011B67C();
            } else {

        r3 = 0x12;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xd8) {
            r3 = 0x0;
            goto L_80206060;
        }
        r3 = r31;
        r4 = 0x12;
        fn_8011B67C();
            }
    L_80206060: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = 0x22;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x7c) {
            r3 = 0x22;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0xc8) {
                r3 = 0x22;
                fn_80119ED0();
                r0 = r3 & 0xFFFF;
                if ((u32)r0 == (u32)0xcd) {
            }
            }
            r3 = r31;
            r4 = 0x0;
            r5 = 0xd6;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            r28 = r3;
            r3 = 0x22;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0x7c) {
                r3 = 0x22;
                fn_80119ED0();
                r0 = r3 & 0xFFFF;
                if ((u32)r0 == (u32)0xc8) {
                }
                if ((u32)r28 == (u32)0x0) {
                    r3 = 0x0;
                } else {

                    r3 = r28;
                    r4 = 0x0;
                    r5 = 0xcc;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                }
                r4 = 0x22;
                fn_80121ADC();
                goto L_8020616C;
                }
            r3 = 0x22;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0xcd) {
                r3 = 0x0;
                goto L_8020616C;
            }
            r3 = r28;
            r4 = 0x22;
            fn_8011B67C();
                } else {

            r3 = 0x22;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0xd8) {
                r3 = 0x0;
                goto L_8020616C;
            }
            r3 = r31;
            r4 = 0x22;
            fn_8011B67C();
                }
        L_8020616C: ;
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) { r3 = 0x1; return; }
    }
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = r31;
        r4 = 0x0;
        r5 = 0xf8;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r28 = r3;
        fn_80209CB4();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
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
            if ((u32)r3 != (u32)0x0) {
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
                if ((u32)r3 != (u32)0x0) {
                    r4 = (u32)lbl_80375CA8;
                    r5 = r31;
                    r8 = (u32)lbl_80375CA8;
                    r6 = 0x13;
                    r4 = 0x0;
                    r7 = 0x0;
                    fn_801F11CC();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) {
                        r3 = r28;
                        r4 = r29;
                        fn_8020D878();
    }
    }
    }
    }
    }
    r3 = 0x0;
    return;

    r3 = 0x1;

    return;
}

/* 0x802062A8 | size: 0x54 | small */
void fn_802062A8(void* param_1, u32 param_2, u32 param_3) {
    extern u16 fn_8011BEB4(void*, u32, u16, u32);
    extern void fn_801F00D0(u16, u32);
    void* uVar1;
    u16 uVar2;

    uVar1 = fn_8012640C(param_1, 0, 0xF8, 0);
    uVar2 = fn_8011BEB4(uVar1, 0, 0x29, 0);
    fn_801F00D0(uVar2, param_3);
}

/* 0x802062FC | size: 0x30C | large */
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

    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((s32)r0 == (s32)0) {
            r0 = 0x0;
            break;
        }
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            break;
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        /* mr. r31, r3 */;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            break;
        }
        do {
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x0;
                break;
            }
            fn_801EF634();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x0;
                break;
            }
            r3 = r31;
            r4 = 0x0;
            r5 = 0xcb;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            if ((u32)r31 == (u32)0x0) {
                r3 = 0x0;
            } else {

                r3 = r31;
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
            }
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            r3 = r31;
            r4 = 0x0;
            r5 = 0xce;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 < (s32)0x0) {
                r0 = 0x0;
                break;
            }
            r0 = 0x1;
        } while (0);

        r0 = r0 & 0xFF;
        if ((s32)r3 == (s32)0x0) {
            r0 = 0x0;
            break;
        }
        r0 = 0x1;
    } while (0);

    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0x120;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 == (s32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    do {
        if ((s32)r3 == (s32)0x1) {
            r0 = 0x0;
            break;
        }
        do {
            if ((s32)r3 == (s32)0x1) {
                r0 = 0x0;
                break;
            }
            fn_801EF634();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x0;
                break;
            }
            r3 = r31;
            r4 = 0x0;
            r5 = 0xcb;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            if ((u32)r31 == (u32)0x0) {
                r3 = 0x0;
            } else {

                r3 = r31;
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
            }
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            fn_80123FBC();
            r0 = r3 & 0xFF;
            if ((u32)r3 == (u32)0x0) {
                r0 = 0x0;
                break;
            }
            r3 = r31;
            r4 = 0x0;
            r5 = 0xce;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 < (s32)0x0) {
                r0 = 0x0;
                break;
            }
            r0 = 0x1;
        } while (0);

        r0 = r0 & 0xFF;
        if ((s32)r3 == (s32)0x0) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd2;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((s32)r3 == (s32)0x1) {
            r0 = 0x0;
            break;
        }
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
        } else {

            r3 = r31;
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        }
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_801233F4();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        r0 = 0x1;
    } while (0);

    r3 = r0 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;

    return;
}

/* 0x80206608 | size: 0x178 | medium */
void fn_80206608(void) {
    extern void fn_801233F4();
    extern void fn_80123FBC();
    extern void fn_801EF634();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((s32)r0 == (s32)0) {
            r0 = 0x0;
            break;
        }
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xcb;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
        } else {

            r3 = r31;
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        }
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xce;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((s32)r3 < (s32)0x0) {
            r0 = 0x0;
            break;
        }
        r0 = 0x1;
    } while (0);

    r0 = r0 & 0xFF;
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd2;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 == (s32)0x1) {
        r3 = 0x0;
        return;
    }
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r31;
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_801233F4();
    r3 = r3 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;

    return;
}

/* 0x80206780 | size: 0x148 | medium */
void fn_80206780(void) {
    extern void fn_80123FBC();
    extern void fn_801EF634();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd6;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    do {
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            break;
        }
        fn_801EF634();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xcb;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
        } else {

            r3 = r31;
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        }
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        fn_80123FBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r0 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xce;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((s32)r3 < (s32)0x0) {
            r0 = 0x0;
            break;
        }
        r0 = 0x1;
    } while (0);

    r3 = r0 & 0xFF;
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;

    return;
}

/* 0x802068C8 | size: 0x13C | medium */
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

    /* mr. r28, r3 */;
    r30 = r4;
    r29 = r5;
    if ((s32)r0 == (s32)0) return;
    if ((u32)r30 == (u32)0x0) return;
    if ((u32)r30 == (u32)0x0) {
        r31 = 0x0;
    } else {

        r3 = r30;
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r31 = r3;
    }
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
    if ((u32)r29 != (u32)0x0) {
        r3 = r28;
        r7 = r29;
        r4 = 0x0;
        r5 = 0xee;
        r6 = 0x0;
        ((void(*)(void))fn_801254B4)();
        fn_801F198C();
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r30 = r3 & 0xFFFF;
    r29 = 0x0;
    while (1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 >= (u32)0x2) break;
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

    }
    r3 = r31;
    fn_801248C4();
    r0 = r3;
    r3 = r28;
    r7 = r0 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x100;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();

    return;
}

/* 0x80206A04 | size: 0xE8 | medium */
void fn_80206A04(void) {
    extern void fn_80123FBC();
    extern void fn_801EF634();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xcb;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
    } else {

        r3 = r31;
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    }
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xce;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = (u32)r3 >> 31;
    r3 = r0 ^ 0x1;

    return;
}

/* 0x80206AEC | size: 0x150 | medium */
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

    /* mr. r29, r3 */;
    r30 = r4;
    r31 = r5;
    if (((s32)r0 != (s32)0) && ((u32)r30 != (u32)0x0)) {

        if ((u32)r29 != (u32)0x0) {
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
        }
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
    }
    return;
}

/* 0x80206C3C | size: 0x58 | small */
void fn_80206C3C(u32 param_1, u16 param_2) {
    extern void fn_80206C94(u32);
    u16 uVar1;

    if (param_1 != 0) {
        for (uVar1 = 0; uVar1 < param_2; uVar1 = uVar1 + 1) {
            fn_80206C94(param_1 + (u32)uVar1 * 0x6E0);
        }
    }
}

/* 0x80206C94 | size: 0x72C | large */
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

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) return;
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
    if ((s32)r0 != (s32)0) {
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
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xd8;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = 0x34;
    fn_8011B950();
    r3 = (u32)lbl_80279C60;
    r30 = (u32)sp + 0x8;
    r6 = (u32)lbl_80279C60;
    r29 = 0x0;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r3 = *(u32*)((u8*)r6 + 0x8);
    r0 = *(u16*)((u8*)r6 + 0xC);
    *(u16*)(sp + 0x14) = r0;
    while (1) {
        r0 = r29 & 0xFF;
        if ((u32)r0 >= (u32)0x7) break;
        /* clrlslwi r0, r29, 24, 1 */;
        r3 = r31;
        r5 = *(u16*)(r30 + r0);
        r4 = 0x0;
        r6 = 0x0;
        r7 = 0x6;
        ((void(*)(void))fn_801254B4)();
        r29 = r29 + 0x1;

    }
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
    while (1) {
        r0 = r29 & 0xFF;
        if ((u32)r0 >= (u32)0xc) break;
        r3 = r31;
        r6 = r29 & 0xFF;
        r4 = 0x0;
        r5 = 0xfd;
        r7 = -0x1;
        ((void(*)(void))fn_801254B4)();
        r29 = r29 + 0x1;

    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xfe;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) {
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
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xf8;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    fn_80209D90();
    r29 = 0x0;
    while (1) {
        r0 = r29 & 0xFF;
        if ((u32)r0 >= (u32)0x2) break;
        r3 = r31;
        r6 = r29 & 0xFF;
        r4 = 0x0;
        r5 = 0xff;
        r7 = 0x9;
        ((void(*)(void))fn_801254B4)();
        r29 = r29 + 0x1;

    }
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
    if ((u32)r3 != (u32)0x0) {
        r0 = 0x0;
        *(u32*)((u8*)r3 + 0x0) = r0;
    }
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

    return;
}

/* 0x802073C0 | size: 0x88 | medium */
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
    r29 = r3;
    r31 = (u32)sp + 0x8;
    r30 = 0x0;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r3 = *(u32*)((u8*)r6 + 0x8);
    r0 = *(u16*)((u8*)r6 + 0xC);
    *(u16*)(sp + 0x14) = r0;

    while ((u32)r0 < (u32)0x7) {
        /* clrlslwi r0, r30, 24, 1 */;
        r3 = r29;
        r5 = *(u16*)(r31 + r0);
        r4 = 0x0;
        r6 = 0x0;
        r7 = 0x6;
        ((void(*)(void))fn_801254B4)();
        r30 = r30 + 0x1;

    r0 = r30 & 0xFF;
    }
    return;
}

/* 0x80207448 | size: 0x15C | medium */
void fn_80207448(void* param_1) {
    fn_801254B4(param_1, 0, 0x113, 0, 0);
    fn_801254B4(param_1, 0, 0x114, 0, 0);
    fn_801254B4(param_1, 0, 0x115, 0, 0);
    fn_801254B4(param_1, 0, 0x116, 0, 0);
    fn_801254B4(param_1, 0, 0x117, 0, 0);
    fn_801254B4(param_1, 0, 0x118, 0, 0);
    fn_801254B4(param_1, 0, 0x119, 0, 0);
    fn_801254B4(param_1, 0, 0x11A, 0, 0);
    fn_801254B4(param_1, 0, 0x11B, 0, 0);
    fn_801254B4(param_1, 0, 0x11C, 0, 0);
    fn_801254B4(param_1, 0, 0x11D, 0, 0);
    fn_801254B4(param_1, 0, 0x11E, 0, 0);
    fn_801254B4(param_1, 0, 0x11F, 0, 0);
}

/* 0x802075A4 | size: 0x1BC | medium */
void fn_802075A4(void* param_1) {
    fn_801254B4(param_1, 0, 0x102, 0, 0);
    fn_801254B4(param_1, 0, 0x103, 0, 0);
    fn_801254B4(param_1, 0, 0x104, 0, 0);
    fn_801254B4(param_1, 0, 0x105, 0, 0);
    fn_801254B4(param_1, 0, 0x106, 0, 0);
    fn_801254B4(param_1, 0, 0x107, 0, 0);
    fn_801254B4(param_1, 0, 0x108, 0, 0);
    fn_801254B4(param_1, 0, 0x109, 0, 0);
    fn_801254B4(param_1, 0, 0x10A, 0, 0);
    fn_801254B4(param_1, 0, 0x10B, 0, 0);
    fn_801254B4(param_1, 0, 0x10C, 0, 0);
    fn_801254B4(param_1, 0, 0x10D, 0, 0);
    fn_801254B4(param_1, 0, 0x10E, 0, 0);
    fn_801254B4(param_1, 0, 0x10F, 0, 0);
    fn_801254B4(param_1, 0, 0x110, 0, 0);
    fn_801254B4(param_1, 0, 0x111, 0, 0);
    fn_801254B4(param_1, 0, 0x112, 0, 0);
}

/* 0x80207760 | size: 0x74 | small */
void fn_80207760(void* param_1) {
    extern void fn_801F1460(void*);
    extern void fn_80209D90(void*);
    extern void fn_8020A478(void*);
    void* iVar1;

    iVar1 = fn_8012640C(param_1, 0, 0xFE, 0);
    if (iVar1 != NULL) {
        fn_801F1460(iVar1);
        fn_8012640C(param_1, 0, 0xD9, 0);
        fn_80209D90(iVar1);
        fn_8012640C(param_1, 0, 0xE5, 0);
        fn_8020A478(iVar1);
    }
}

/* 0x802077D4 | size: 0x11C */
void fn_802077D4(void* basePtr, u16 count) {
    extern void fn_8011B950();
    extern void fn_80124A60();
    u16 i;
    void* entry;

    if (!basePtr) { return; }
    for (i = 0; (u16)i < count; i++) {
        entry = (void*)((u32)basePtr + (u16)i * 0x154);
        if (!entry) { continue; }
        fn_801254B4(entry, 0, 0xCB, 0, 0);
        fn_80124A60(fn_8012640C(entry, 0, 0xCC, 0));
        fn_8011B950(fn_8012640C(entry, 0, 0xCD, 0), 1);
        fn_801254B4(entry, 0, 0xCE, 0, (u32)-1);
        fn_801254B4(entry, 0, 0xCF, 0, 0);
        fn_801254B4(entry, 0, 0xD0, 0, 0);
        fn_801254B4(entry, 0, 0xD1, 0, 0);
        fn_801254B4(entry, 0, 0xD2, 0, 0);
    }
}

/* 0x802078F0 | size: 0xEC | medium */
void fn_802078F0(void) {
    extern void fn_8011B950();
    extern void fn_80124A60();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) {
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
    }
    return;
}

/* 0x802079DC | size: 0x104 */
u32 fn_802079DC(void* ctx, void* battleCtx, u32* outSlots) {
    extern u16 fn_8010C74C();
    u16 i;
    u16 outCount;
    u16 slot0;
    u16 slot1;
    u16 result;
    u8 isPlayerSlot;

    for (i = 0; (u16)i < 0x12; i++) {
        outSlots[(u16)i] = (u32)-1;
    }
    outCount = 0;
    for (i = 0; (u16)i < 0x12; i++) {
        slot0 = (u16)(u32)fn_8012640C(ctx, 0, 0xFF, 0);
        slot1 = (u16)(u32)fn_8012640C(ctx, 0, 0xFF, 1);
        isPlayerSlot = ((u16)i == slot0 || (u16)i == slot1) ? 1 : 0;
        if ((u8)isPlayerSlot == 1) { continue; }
        result = fn_8010C74C(battleCtx, i);
        if (result == 0x42 || result == 0x43) {
            outSlots[(u16)outCount] = (u16)i;
            outCount++;
        }
    }
    return outCount;
}

/* 0x80207AE0 | size: 0x7C | small */
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
    r31 = r4;
    r30 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 == (u32)r3) { r3 = 0x1; return; }
    r3 = r30;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x1;
    ((void(*)(void))fn_8012640C)();
    r3 = r3 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    if ((u32)r0 != (u32)r3) { r3 = 0x0; return; }

    r3 = 0x1;
    return;

    r3 = 0x0;

    return;
}

/* 0x80207B5C | size: 0x30 */
u32 fn_80207B5C(void* context, u8 flags, u16 value) {
    return fn_801254B4(context, 0, 0xFF, flags, value);
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
void fn_80207C24(void* ctx, u32 param) {
    extern void fn_801DA5AC();
    void* obj = fn_8012640C(ctx, 0, 0xee, 0);
    if (obj != 0) {
        fn_801DA5AC(obj, param);
    }
}

/* 0x80207C6C | size: 0x2F0 | large */
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

    /* mr. r30, r3 */;
    r28 = r4;
    do {
        if ((s32)r0 == (s32)0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

    r4 = r3;
    r3 = (u32)sp + 0x10;
    fn_8011F5FC();
    r3 = (u32)sp + 0x10;
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
    if ((u32)r29 == (u32)0x181) {
        r0 = r28 & 0xFFFF;
        if ((s32)r0 != (s32)0x3) {
            if ((s32)r0 < (s32)0x3) {
                if ((s32)r0 != (s32)0x1) {
                    if ((s32)r0 < (s32)0x1) {
                        goto L_80207D48;
                    }
                    if ((s32)r0 >= (s32)0x5) goto L_80207D48;
                    goto L_80207D40;
                    }
                r0 = 0x19f;
                goto L_80207D4C;
                    }
            r0 = 0x19e;
            goto L_80207D4C;
            L_80207D40: ;
            r0 = 0x1a0;
            goto L_80207D4C;
        }
        L_80207D48: ;
        r0 = 0x181;
        L_80207D4C: ;
        r31 = r0;
    }
    r3 = 0x14;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x7c) {
        r3 = 0x14;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xc8) {
            r3 = 0x14;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0xcd) {
        }
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r29 = r3;
        r3 = 0x14;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x7c) {
            r3 = 0x14;
            fn_80119ED0();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 == (u32)0xc8) {
            }
            if ((u32)r29 == (u32)0x0) {
                r3 = 0x0;
            } else {

                r3 = r29;
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
            }
            r4 = 0x14;
            fn_80121ADC();
            goto L_80207E50;
            }
        r3 = 0x14;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xcd) {
            r3 = 0x0;
            goto L_80207E50;
        }
        r3 = r29;
        r4 = 0x14;
        fn_8011B67C();
            } else {

        r3 = 0x14;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0xd8) {
            r3 = 0x0;
            goto L_80207E50;
        }
        r3 = r30;
        r4 = 0x14;
        fn_8011B67C();
            }
    L_80207E50: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r31 = 0x19d;
    }
    if ((u32)r31 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x8;
    fn_801FDB78();
    r3 = (u32)sp + 0x10;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = (u32)sp + 0x10;
    r4 = 0x0;
    r5 = 0x75;
    r6 = 0x0;
    ((void(*)(void))fn_801254B4)();
    r3 = (u32)sp + 0x10;
    fn_80125390();
    r5 = r3;
    r3 = r31 & 0xFFFF;
    fn_801DE190();
    r0 = r3;
    r3 = (u32)sp + 0x10;
    r29 = r0;
    r4 = r29;
    fn_80122040();
    do {
        if ((u32)r30 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r3 = r30;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

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

    return;
}

/* 0x80207F5C | size: 0xCC | medium */
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

    /* mr. r29, r3 */;
    r30 = r5;
    r31 = *(u32*)((u8*)r5 + 0x0);
    do {
        if ((s32)r0 == (s32)0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

    if ((u32)r29 == (u32)r31) {
        r3 = 0x1;
        return;
    }
    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    if ((u32)r3 == (u32)0x0) {
        r3 = 0x1;
        return;
    }
    r0 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r0 == (u32)r3) {
        r3 = *(u32*)((u8*)r30 + 0x8);
        r0 = r3 + 0x1;
        *(u32*)((u8*)r30 + 0x8) = r0;
    }
    r3 = 0x1;

    return;
}

/* 0x80208028 | size: 0x80 | small */
void fn_80208028(void) {
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    do {
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

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

/* 0x802080A8 | size: 0x35C | large */
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
    if ((s32)r0 == (s32)0) return;
    r0 = r24 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r0 = r28 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r4 = 0xa3;
            r5 = 0x4;
            r6 = 0x0;
            fn_801DDD28();
        }
        r0 = r29 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = r26;
            r4 = 0x9f;
            r5 = 0x4;
            r6 = 0x0;
            fn_801DDD28();
        }
        r0 = r28 & 0xFF;
        if ((u32)r0 != (u32)0x1) return;
        r0 = r29 & 0xFF;
        if ((u32)r0 != (u32)0x1) return;
        r3 = r26;
        r4 = 0x57;
        r5 = 0x4;
        r6 = 0x0;
        fn_801DDD28();
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r0 = r28 & 0xFF;
        if ((u32)r0 == (u32)0x1 && (u32)r3 == (u32)0x0) {
            r4 = 0xa3;
            r5 = 0x4;
            fn_801DA9E8();
            do {
                if ((u32)r27 == (u32)0x0) {
                    r3 = 0x0;
                    break;
                }
                r3 = r27;
                r4 = 0x0;
                r5 = 0xd6;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((u32)r3 == (u32)0x0) {
                    r3 = 0x0;
                    break;
                }
                r4 = 0x0;
                r5 = 0xcc;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
            } while (0);

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

            r3 = r27;
            r4 = r31;
            r5 = 0x1;
            fn_802653FC();
        }
        r0 = r29 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r28 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                do {
                    r3 = r26;
                    r4 = 0xa3;
                    r5 = 0x4;
                    fn_801DA94C();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) break;
                    fn_800F0308();
                } while (1);
            }
            r3 = r26;
            r4 = 0x9f;
            r5 = 0x4;
            fn_801DA9E8();
            r0 = r28 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                do {
                    if ((u32)r27 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r3 = r27;
                    r4 = 0x0;
                    r5 = 0xd6;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    if ((u32)r3 == (u32)0x0) {
                        r3 = 0x0;
                        break;
                    }
                    r4 = 0x0;
                    r5 = 0xcc;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                } while (0);

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
            }
            r3 = r27;
            r4 = r31;
            r5 = 0x1;
            fn_802653FC();
        }
        r0 = r28 & 0xFF;
        if ((u32)r3 != (u32)0x0) return;
        r0 = r29 & 0xFF;
        if ((u32)r3 != (u32)0x0) return;
        r3 = r26;
        r4 = 0x57;
        r5 = 0x4;
        fn_801DA9E8();
        r3 = r30;
        fn_802624CC();
        return;
    }
    if ((u32)r0 == (u32)0x2) {
        r0 = r28 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r25 = 0xa3;
        }
        r0 = r29 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r25 = 0x9f;
        }
        r0 = r28 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r29 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r25 = 0x57;
        }
        }
        while (1) {
            r3 = r26;
            r4 = r25;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) return;
            fn_800F0308();
        }
    }
    if ((u32)r0 != (u32)0x3) return;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r4 = 0xa3;
        r5 = 0x4;
        fn_801DA8C4();
    }
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r26;
        r4 = 0x9f;
        r5 = 0x4;
        fn_801DA8C4();
    }
    r0 = r28 & 0xFF;
    if ((u32)r0 != (u32)0x1) return;
    r0 = r29 & 0xFF;
    if ((u32)r0 != (u32)0x1) return;
    r3 = r26;
    r4 = 0x57;
    r5 = 0x4;
    fn_801DA8C4();

    return;
}

/* 0x80208404 | size: 0x150 | medium */
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
    if ((s32)r0 == (s32)0) return;
    r0 = r29 & 0xFF;
    if ((s32)r0 == (s32)0) {
        r29 = 0x3a;

    } else if ((u32)r0 == (u32)0x1) {
        r29 = 0x88;

    } else if ((u32)r0 == (u32)0x2) {
        r29 = 0x57;

    } else {
        r29 = 0xd9;
    }
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x2) {
        r3 = r31;
        r4 = r29;
        r5 = 0x4;
        r6 = 0x0;
        fn_801DDD28();
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = r29;
        r5 = 0x4;
        fn_801DA9E8();
        r0 = r27 & 0xFF;
        if ((u32)r0 != (u32)0x1) return;
        r3 = r26;
        r4 = r30;
        r5 = 0x1;
        fn_80265598();
        return;
    }
    if ((u32)r0 == (u32)0x2) {
        do {
            r3 = r31;
            r4 = r29;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x2) return;
            fn_800F0308();
        } while (1);
    }
    if ((u32)r0 == (u32)0x3) {
        r3 = r31;
        r4 = r29;
        r5 = 0x4;
        fn_801DA8C4();
        return;
    }
    if ((u32)r0 != (u32)0x4) return;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801DA9B4();

    return;
}

/* 0x80208554 | size: 0x70 | small */
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

    r28 = r4;
    r29 = r5;
    r30 = r6;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) return;
    do {
        r3 = r31;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        fn_801DA698();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) return;
        fn_800F0308();
    } while (1);

    return;
}

/* 0x802085C4 | size: 0xEC | medium */
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
    if ((s32)r0 != (s32)0) {
        r3 = r25;
        r4 = (u32)sp + 0x8;
        fn_801FE168();
        if ((s32)r29 >= (s32)0x0) {
            r3 = r25;
            r4 = r30;
            r5 = 0x1;
            fn_802656AC();
            r9 = (u32)sp + 0x8;
            r29 = r3;
            r4 = 0x0;
            r5 = 0x0;
            r6 = 0x0;
            r7 = 0x0;
            r8 = 0x1;
            /* crclr cr1eq */;
            fn_801026A4();
        }
        r3 = r31;
        r4 = r26;
        r5 = r27;
        r6 = r28;
        fn_802094CC();
        r0 = r28 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            if ((s32)r29 >= (s32)0x0) {
                r3 = r29;
                r4 = 0x0;
                r5 = 0x0;
                fn_80102568();
    }
    }
    }
    return;
}

/* 0x802086B0 | size: 0x38 | small */
void fn_802086B0(void* ctx) {
    extern void fn_801DA83C();
    void* obj = fn_8012640C(ctx, 0, 0xee, 0);
    if (obj != 0) {
        fn_801DA83C(obj);
    }
}

/* 0x802086E8 | size: 0x68 | small */
void fn_802086E8(void) {
    extern void fn_8011BEB4();
    extern void fn_801DA8C4();
    u8 sp[0x20];
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = 0x0;
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
    if ((u32)r3 != (u32)0x0) {
        r5 = r30;
        r4 = r31 & 0xFFFF;
        fn_801DA8C4();
    }
    return;
}

/* 0x80208750 | size: 0x70 | small */
void fn_80208750(void) {
    extern void fn_8011BEB4();
    extern void fn_801DDD28();
    u8 sp[0x20];
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
    if ((u32)r3 != (u32)0x0) {
        r5 = r29;
        r6 = r30;
        r4 = r31 & 0xFFFF;
        fn_801DDD28();
    }
    return;
}

/* 0x802087C0 | size: 0x458 | large */
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
    if ((s32)r0 == (s32)0) return;
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
    if ((s32)r0 == (s32)0) {
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
        if ((u32)r23 == (u32)0x0) return;
        r3 = r31;
        fn_801DA354();
        *(u8*)((u8*)r23 + 0x0) = r3;
        r3 = r31;
        fn_801DA2C4();
        return;
    }
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 != (u32)0x1) {
            fn_800F0308();
            goto L_80208918;
        }
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
        if ((u32)r0 != (u32)0x1) {
            fn_800F0308();
            goto L_80208948;
        }
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
        if ((u32)r0 != (u32)0x1) {
            fn_800F0308();
            goto L_80208980;
        }
        r24 = r24 + 0x1;
        r0 = r24 & 0xFF;
        if ((u32)r0 < (u32)0x3) {
            if ((u32)r0 < (u32)r29) goto L_80208970;
        }
        r0 = r30 & 0xFF;
        if ((u32)r0 >= (u32)0x4) return;
        r3 = r31;
        r4 = r27 & 0xFFFF;
        r5 = 0x4;
        fn_801DA9E8();
        do {
            r3 = r31;
            r4 = r27 & 0xFFFF;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x4) return;
            fn_800F0308();
        } while (1);
    }
    if ((u32)r0 == (u32)0x2) {
        r0 = r30 & 0xFF;
        if ((u32)r0 >= (u32)0x4) return;
        r3 = r31;
        r4 = r28 & 0xFFFF;
        r5 = 0x4;
        fn_801DA9E8();
        return;
    }
    if ((u32)r0 == (u32)0x3) {
        r0 = r30 & 0xFF;
        if ((u32)r0 >= (u32)0x4) return;
        if ((u32)r23 != (u32)0x0) {
            r4 = *(u8*)((u8*)r23 + 0x0);
            r3 = r31;
            fn_801DA224();
        }
        while (1) {
            r3 = r31;
            r4 = r28 & 0xFFFF;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r23 == (u32)0x0) return;
            fn_800F0308();
        }
    }
    if ((u32)r0 != (u32)0x4) return;
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
    if ((u32)r0 >= (u32)0x4) {
        do {
            if ((u32)r29 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            r3 = r29;
            r4 = 0x0;
            r5 = 0xd6;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        } while (0);

        r4 = 0x0;
        r5 = 0x73;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        r4 = r3 & 0xFF;
        r3 = 0x0;
        r5 = 0x10;
        r6 = 0x0;
        ((void(*)(void))fn_80142CF4)();
        if ((u32)r3 != (u32)0x0) {
            r0 = 0x0;
            r4 = (u32)fn_80207F5C;
            r4 = (u32)fn_80207F5C;
            r5 = (u32)sp + 0x8;
            r3 = 0x0;
            r6 = 0x0;
            *(u32*)(sp + 0x10) = r0;
            fn_801F37B0();
        }
        r3 = r29;
        r4 = 0x0;
        r5 = 0xee;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        /* mr. r24, r3 */;
        if ((u32)r3 != (u32)0x0) {
            r3 = r29;
            r4 = 0x0;
            r5 = 0xee;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((u32)r3 != (u32)0x0) {
                r4 = 0x0;
                fn_801DA4E8();
            }
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
        }
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x14;
        r6 = 0x0;
        fn_801F54A4();
        r4 = r3 & 0xFFFF;
        r3 = r29;
        r5 = 0x1;
        fn_8026532C();
        return;
    }
    if ((u32)r23 == (u32)0x0) return;
    r4 = *(u8*)((u8*)r23 + 0x0);
    r3 = r31;
    fn_801DA224();

    return;
}

/* 0x80208C18 | size: 0x2B8 | large */
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
    r27 = r4;
    r26 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) return;
    if ((u32)r26 == (u32)0x0) {
        r28 = 0x0;
    } else {

        r3 = r26;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
        } else {

            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        }
        r28 = r3;
    }
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
    if ((u32)r3 == (u32)0x0) {
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
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = r31;
        r4 = r29 & 0xFFFF;
        r5 = 0x4;
        fn_801DA9E8();
        return;
    }
    if ((u32)r0 == (u32)0x2) {
        do {
            r3 = r31;
            r4 = r29 & 0xFFFF;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x2) return;
            fn_800F0308();
        } while (1);
    }
    if ((u32)r0 == (u32)0x3) {
        r3 = r31;
        r4 = r30 & 0xFFFF;
        r5 = 0x4;
        fn_801DA9E8();
        return;
    }
    if ((u32)r0 == (u32)0x4) {
        L_80208D88: ;
        r3 = 0x0;
        fn_801DA5C4();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) {
            fn_800F0308();
            goto L_80208D88;
        }
        do {
            if ((u32)r26 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            r3 = r26;
            r4 = 0x0;
            r5 = 0xd6;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((u32)r3 == (u32)0x0) {
                r3 = 0x0;
                break;
            }
            r4 = 0x0;
            r5 = 0xcc;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
        } while (0);

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
        if ((u32)r3 != (u32)0x0) {
            fn_800F0308();
            goto L_80208E20;
        }
        r3 = r28;
        fn_80125390();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) return;
        r3 = r31;
        r4 = 0x67;
        r5 = 0x4;
        fn_801DA9E8();
        do {
            r3 = r31;
            r4 = 0x67;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) return;
            fn_800F0308();
        } while (1);
    }
    if ((u32)r0 != (u32)0x5) return;
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

    return;
}

/* 0x80208ED0 | size: 0x25C | large */
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
    r30 = r4;
    r31 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) return;
    do {
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

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
    if ((u32)r3 == (u32)0x0) {
        r3 = r29;
        r4 = r30 & 0xFFFF;
        r5 = 0x4;
        r6 = 0x0;
        fn_801DDD28();
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = r29;
        r4 = r30 & 0xFFFF;
        r5 = 0x4;
        fn_801DA9E8();
        return;
    }
    if ((u32)r0 == (u32)0x2) {
        do {
            r3 = r29;
            r4 = r30 & 0xFFFF;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x2) return;
            fn_800F0308();
        } while (1);
    }
    if ((u32)r0 == (u32)0x3) {
        r3 = r29;
        r4 = r30 & 0xFFFF;
        r5 = 0x4;
        fn_801DA8C4();
        return;
    }
    if ((u32)r0 != (u32)0x4) return;
    do {
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    if ((u32)r3 != (u32)0x0) {
        r0 = 0x0;
        r4 = (u32)fn_80207F5C;
        r4 = (u32)fn_80207F5C;
        r5 = (u32)sp + 0x8;
        r3 = 0x0;
        r6 = 0x0;
        *(u32*)(sp + 0x10) = r0;
        fn_801F37B0();
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((u32)r3 != (u32)0x0) {
        r3 = r31;
        r4 = 0x0;
        r5 = 0xee;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 != (u32)0x0) {
            r4 = 0x0;
            fn_801DA4E8();
        }
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
    }
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r4 = r3 & 0xFFFF;
    r3 = r31;
    r5 = 0x1;
    fn_8026532C();

    return;
}

/* 0x8020912C | size: 0x254 | large */
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
    r30 = r4;
    r31 = r3;
    r4 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r29, r3 */;
    if ((s32)r0 == (s32)0) return;
    do {
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

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
    if ((u32)r3 == (u32)0x0) {
        r3 = r29;
        r4 = r30 & 0xFFFF;
        r5 = 0x4;
        r6 = 0x0;
        fn_801DDD28();
        return;
    }
    if ((u32)r0 == (u32)0x1) {
        r3 = r29;
        r4 = r30 & 0xFFFF;
        r5 = 0x4;
        fn_801DA9E8();
        return;
    }
    if ((u32)r0 == (u32)0x2) {
        L_8020920C: ;
        r3 = r29;
        r4 = r30 & 0xFFFF;
        r5 = 0x4;
        fn_801DA94C();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x2) {
            fn_800F0308();
            goto L_8020920C;
        }
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
        return;
    }
    if ((u32)r0 != (u32)0x3) return;
    do {
        if ((u32)r31 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r3 = r31;
        r4 = 0x0;
        r5 = 0xd6;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((u32)r3 == (u32)0x0) {
            r3 = 0x0;
            break;
        }
        r4 = 0x0;
        r5 = 0xcc;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
    } while (0);

    r4 = 0x0;
    r5 = 0x73;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r4 = r3 & 0xFF;
    r3 = 0x0;
    r5 = 0x10;
    r6 = 0x0;
    ((void(*)(void))fn_80142CF4)();
    if ((u32)r3 != (u32)0x0) {
        r0 = 0x0;
        r4 = (u32)fn_80207F5C;
        r4 = (u32)fn_80207F5C;
        r5 = (u32)sp + 0x8;
        r3 = 0x0;
        r6 = 0x0;
        *(u32*)(sp + 0x10) = r0;
        fn_801F37B0();
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    /* mr. r30, r3 */;
    if ((u32)r3 == (u32)0x0) return;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xee;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) {
        r4 = 0x0;
        fn_801DA4E8();
    }
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

    return;
}

/* 0x80209380 | size: 0x104 */
void fn_80209380(void* ctx) {
    extern u8 fn_800E3D08();
    extern void fn_800E4014();
    extern void fn_801DA4E8();
    extern void* fn_801DAC3C();
    extern void fn_801F000C();
    void* eeData;
    void* resolved;
    u8 i;

    eeData = fn_8012640C(ctx, 0, 0xEE, 0);
    resolved = !eeData ? NULL : fn_801DAC3C(eeData);
    if (resolved == NULL) { return; }
    if ((u8)fn_800E3D08(resolved) == 0) { return; }
    for (i = 0; (u8)i < 8; i++) {
        eeData = fn_8012640C(ctx, 0, 0xEE, 0);
        if (eeData != NULL) {
            fn_801DA4E8(eeData, 1);
        }
        fn_800E4014(resolved, 1);
        fn_801F000C(3);
        eeData = fn_8012640C(ctx, 0, 0xEE, 0);
        if (eeData != NULL) {
            fn_801DA4E8(eeData, 0);
        }
        fn_801F000C(2);
    }
    eeData = fn_8012640C(ctx, 0, 0xEE, 0);
    if (eeData != NULL) {
        fn_801DA4E8(eeData, 1);
    }
}

/* 0x80209484 | size: 0x48 | small */
void fn_80209484(void* ctx, u32 param) {
    extern void fn_801DA4E8();
    void* obj = fn_8012640C(ctx, 0, 0xee, 0);
    if (obj != 0) {
        fn_801DA4E8(obj, param);
    }
}

/* 0x802094CC | size: 0x90 | medium */
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
    if ((u32)r0 != (u32)0x1) return;
    L_80209518: ;
    r3 = r28;
    r5 = r29;
    r4 = r31 & 0xFFFF;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        fn_800F0308();
        goto L_80209518;
    }
    r3 = r28;
    r5 = r29;
    r4 = r31 & 0xFFFF;
    fn_801DA8C4();

    return;
}

/* 0x8020955C | size: 0xBC | medium */
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
    if ((s32)r0 == (s32)0) {
        r3 = (u32)fn_80211164;
        r0 = (u32)fn_80211164;
        r31 = r0;
    }
    r4 = r27;
    r3 = 0x0;
    r5 = 0x21;
    r6 = 0x0;
    fn_8011BEB4();
    if ((u32)r3 == (u32)0x0) {
        r3 = (u32)fn_80211168;
        r3 = (u32)fn_80211168;
    }
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
    return;
}

/* 0x80209618 | size: 0xD0 | medium */
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
    r31 = r3;
    r30 = (u32)sp + 0x8;
    r27 = 0x0;
    r28 = 0x0;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r3 = *(u32*)((u8*)r6 + 0x8);
    r0 = *(u16*)((u8*)r6 + 0xC);
    *(u16*)(sp + 0x14) = r0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if ((u32)r0 >= (u32)0x7) break;
        /* clrlslwi r0, r28, 16, 1 */;
        r29 = *(u16*)(r30 + r0);
        r3 = r29;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x2a) {
            r3 = 0x0;
        } else {

            r3 = r31;
            r4 = r29;
            fn_8011B67C();
        }
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = r27 & 0xFFFF;
            r0 = r3 + 0x1;
            r27 = r0 & 0xFFFF;
        }
        r28 = r28 + 0x1;

    }
    r3 = r27 & 0xFFFF;
    r0 = 0x2;
    r0 = r3 - r0;
    r0 = -0x1;
    /* subfze r0, r0 */;
    r3 = r0 & 0xFF;
    return;
}

/* 0x802096E8 | size: 0xE0 | medium */
void fn_802096E8(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x40;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) {
        r3 = 0x0;
    } else {

        r3 = r31;
        r4 = 0x40;
        fn_8011B67C();
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x43;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) {
        r3 = 0x0;
    } else {

        r3 = r31;
        r4 = 0x43;
        fn_8011B67C();
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x45;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) {
        r3 = 0x0;
    } else {

        r3 = r31;
        r4 = 0x45;
        fn_8011B67C();
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        return;
    }
    r3 = 0x1;

    return;
}

/* 0x802097C8 | size: 0x54 | small */
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

    r30 = r4;
    r29 = r3;
    r31 = r5;
    r3 = r30;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x2a) {
        r3 = r29;
        r4 = r30;
        r5 = r31;
        fn_8011B2C0();
    }
    return;
}

/* 0x8020981C | size: 0x54 | small */
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

    r31 = r4;
    r30 = r3;
    r3 = r31;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) {
        r3 = 0x0;
    } else {

        r3 = r30;
        r4 = r31;
        fn_8011B444();
    }
    return;
}

/* 0x80209870 | size: 0x9C | medium */
void fn_80209870(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x41;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x2a) {
        r3 = 0x0;
    } else {

        r3 = r31;
        r4 = 0x41;
        fn_8011B67C();
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x42;
        fn_80119ED0();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x2a) {
            r3 = 0x0;
        } else {

            r3 = r31;
            r4 = 0x42;
            fn_8011B67C();
        }
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x1;
            return;
    }
    }
    r3 = 0x0;

    return;
}

/* 0x8020990C | size: 0x54 */
u32 fn_8020990C(void* ctx, u32 param) {
    extern u32 fn_80119ED0();
    extern u32 fn_8011B67C();
    if ((fn_80119ED0(param) & 0xFFFF) == 0x2A) {
        return fn_8011B67C(ctx, param);
    }
    return 0;
}

/* 0x80209960 | size: 0x4C | small */
void fn_80209960(void* ctx, u32 param) {
    extern u32 fn_80119ED0();
    extern void fn_8011B788();
    if ((fn_80119ED0(param) & 0xFFFF) == 0x2a) {
        fn_8011B788(ctx, param);
    }
}

/* 0x802099AC | size: 0x270 | large */
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

    /* mr. r27, r3 */;
    r28 = r4;
    r29 = r5;
    r31 = r6;
    r30 = r7;
    if ((s32)r0 != (s32)0) {
        if ((s32)r0 != (s32)0) {
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
            if ((u32)r0 == (u32)0x2a) {
                r3 = r27;
                r4 = 0x3f;
                r5 = 0x0;
                fn_8011B2C0();
            }
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
        }
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
    }
    return;
}

/* 0x80209C1C | size: 0x98 | medium */
void fn_80209C1C(void) {
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    u8 sp[0x10];
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
    return;
}

/* 0x80209CB4 | size: 0xDC | medium */
void fn_80209CB4(void) {
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x27;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 == (s32)0x163) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 == (s32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x28;
    r6 = 0x0;
    fn_8011BEB4();
    if ((s32)r3 == (s32)0x163) {
        r3 = 0x0;
        return;
    }
    r3 = r31;
    r4 = 0x0;
    r5 = 0x29;
    r6 = 0x0;
    fn_8011BEB4();
    /* subic r0, r3, 0x1 */;
    r3 = r3 - r0; /* -borrow */;

    return;
}

/* 0x80209D90 | size: 0x188 */
void fn_80209D90(void* ctx) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void fn_8011BBD8();
    extern void* fn_8011BEB4();

    if (!ctx) { return; }
    fn_8011BBD8(ctx, 0, 0x26, 0, (u32)-1);
    fn_8011BBD8(ctx, 0, 0x27, 0, 0);
    fn_8011BBD8(ctx, 0, 0x28, 0, 0);
    fn_8011BBD8(ctx, 0, 0x29, 0, 0);
    fn_8011B950(fn_8011BEB4(ctx, 0, 0x2A, 0), 9);
    if (fn_80119ED0(0x3F) == 0x2A) {
        fn_8011B2C0(ctx, 0x3F, 0);
    }
    fn_8011BBD8(ctx, 0, 0x2B, 0, 1);
    fn_8011BBD8(ctx, 0, 0x2C, 0, 1);
    fn_8011BBD8(ctx, 0, 0x2D, 0, 0);
    fn_8011BBD8(ctx, 0, 0x2E, 0, 0);
    fn_8011BBD8(ctx, 0, 0x2F, 0, 0);
    fn_8011BBD8(ctx, 0, 0x30, 0, 9);
    fn_8011BBD8(ctx, 0, 0x31, 0, 0);
    fn_8011BBD8(ctx, 0, 0x32, 0, 0);
}

/* 0x80209F18 | size: 0x94 | medium */
void fn_80209F18(void) {
    extern void fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void fn_8011BBD8();
    extern void fn_8011BEB4();
    u8 sp[0x10];
    u32 r0 = 0;
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
    if ((u32)r0 == (u32)0x2a) {
        r3 = r31;
        r4 = 0x3f;
        r5 = 0x0;
        fn_8011B2C0();
    }
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
    return;
}

/* 0x80209FAC | size: 0x64 */
void fn_80209FAC(void* ctx) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void fn_8011BEB4();
    fn_8011BEB4(ctx, 0, 0x2A, 0);
    fn_8011B950(ctx, 9);
    if (fn_80119ED0(0x3F) == 0x2A) {
        fn_8011B2C0(ctx, 0x3F, 0);
    }
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

/* fn_8020A040 | Size: 0x28 | Look up 2-byte entry in table */
u16* fn_8020A040(u16 index) {
    extern u8 lbl_80375DD0[];
    extern u32 lbl_80478D70;
    if (index < lbl_80478D70) {
        return (u16*)&lbl_80375DD0[index * 2];
    }
    return NULL;
}

/* fn_8020A080 | Size: 0x24 | Look up byte in table with bounds check */
u8* fn_8020A080(u16 index) {
    extern u8 lbl_80478D58[];
    extern u32 lbl_80478D60;
    if (index < lbl_80478D60) {
        return &lbl_80478D58[index];
    }
    return NULL;
}

/* 0x8020A224 | size: 0x34 | small */
void* fn_8020A224(void* base, u32 index) {
    if (base == 0) return 0;
    if ((index & 0xFFFF) >= 9) return 0;
    return (u8*)base + 0x8 + (index & 0xFFFF) * 16;
}

/* 0x8020A2B8 | size: 0x40 -- copy 0xAC bytes (43 u32s) */
void fn_8020A2B8(u32* dst, u32* src) {
    s32 i;
    if (dst == 0) return;
    if (src == 0) return;
    for (i = 0; i < 43; i++) {
        dst[i] = src[i];
    }
}

/* 0x8020A398 | size: 0xE0 | medium */
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

    /* mr. r28, r3 */;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    if ((s32)r0 != (s32)0) {
        if ((s32)r0 != (s32)0) {
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
        }
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
    }
    return;
}

/* 0x8020A478 | size: 0x88 | medium */
void fn_8020A478(void) {
    extern void fn_80142B24();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 != (s32)0) {
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
    }
    return;
}

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

/* 0x8020A5C0 | size: 0x70 */
s16 fn_8020A5C0(u16 index, u16 slot) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    u8* sub;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = entry + (slot * 0xA) + 0x4;
    }
    if (sub == NULL) {
        return 0;
    }
    return *(s16*)(sub + 0x4);
}

/* 0x8020A630 | size: 0x70 */
s16 fn_8020A630(u16 index, u16 slot) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    u8* sub;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = entry + (slot * 0xA) + 0x4;
    }
    if (sub == NULL) {
        return 0;
    }
    return *(s16*)(sub + 0x2);
}

/* 0x8020A6A0 | size: 0x70 */
u8 fn_8020A6A0(u16 index, u16 slot) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    u8* sub;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = entry + (slot * 0xA) + 0x4;
    }
    if (sub == NULL) {
        return 0;
    }
    return sub[1];
}

/* 0x8020A710 | size: 0x70 */
u16 fn_8020A710(u16 index, u16 slot) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    u8* sub;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = entry + (slot * 0xA) + 0x4;
    }
    if (sub == NULL) {
        return 0;
    }
    return *(u16*)(sub + 0x8);
}

/* 0x8020A780 | size: 0x70 */
u16 fn_8020A780(u16 index, u16 slot) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    u8* sub;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = entry + (slot * 0xA) + 0x4;
    }
    if (sub == NULL) {
        return 0;
    }
    return *(u16*)(sub + 0x6);
}

/* 0x8020A7F0 | size: 0x70 */
u8 fn_8020A7F0(u16 index, u16 slot) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    u8* sub;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = entry + (slot * 0xA) + 0x4;
    }
    if (sub == NULL) {
        return 0;
    }
    return sub[0];
}

/* fn_8020A860 | Size: 0x40 | Look up u16 field at offset 2 in 0x18-byte table */
u16 fn_8020A860(u16 index) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        return 0;
    }
    return *(u16*)(entry + 0x2);
}

/* fn_8020A8A0 | Size: 0x40 | Look up u8 field at offset 0 in 0x18-byte table */
u8 fn_8020A8A0(u16 index) {
    extern u8 lbl_80375A08[];
    extern u8 lbl_80478D28[];
    u8* entry;
    if (index >= *(u32*)lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index * 0x18];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry[0];
}

/* 0x8020A8E0 | size: 0x550 | large */
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
    u32 ctr = 0;

    r20 = r3;
    r31 = r4;
    r21 = (u32)sp + 0x10;
    r24 = 0x0;
    while (1) {
        r0 = r24 & 0xFF;
        if ((u32)r0 >= (u32)0x2) break;
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
        if ((s32)r0 != (s32)0x2) {
            if ((s32)r0 < (s32)0x2) {
                if ((s32)r0 == (s32)0x0) goto L_8020AA34;
                if ((s32)r0 < (s32)0x0) {
                    goto L_8020AA34;
                }
                if ((s32)r0 >= (s32)0x4) goto L_8020AA34;
                goto L_8020A9B8;
                }
            r23 = r28 & 0xFFFF;
            goto L_8020AA34;
        }
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
        if ((s32)r0 == (s32)0x4) {
            r23 = 0x0;
            goto L_8020AA58;
        }
        r3 = r28;
        fn_801F0234();
        fn_801F021C();
        r0 = r22 & 0xFF;
        if ((s32)r0 == (s32)0x4) {
            r4 = r23;
            r6 = r27;
            r5 = r26 & 0xFFFF;
            r7 = r25 & 0xFFFF;
            fn_80135E44();
            r23 = r3;

        } else {
            r4 = r23;
            r6 = r27;
            r5 = 0x0;
            r7 = 0x0;
            fn_80135E44();
            r23 = r3;
        }
        L_8020AA34: ;
        r0 = r22 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = (s16)r26;
            r0 = (s16)r25;
            r23 = r23 * r3;
            if ((u32)r0 != (u32)0x1) {
                r0 = (s16)r25;
                r23 = (s32)r23 / (s32)r0;
        }
        }
        L_8020AA58: ;
        /* clrlslwi r0, r24, 24, 2 */;
        r24 = r24 + 0x1;
        *(u32*)(r21 + r0) = r23;

    }
    r3 = r20;
    r24 = 0x0;
    fn_8020A8A0();
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x6) {
        r3 = (u32)jumptable_80375954;
        r0 = r0 << 2;
        r3 = (u32)jumptable_80375954;
        r0 = *(u32*)(r3 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        r24 = 0x1;
    }
    do {
    break;

    if ((s32)r3 == (s32)r0) {
        r24 = 0x1;
    }
    break;

    if ((s32)r3 != (s32)r0) {
        r24 = 0x1;
    }
    break;

    if ((s32)r3 >= (s32)r0) {
        r24 = 0x1;
    }
    break;

    if ((s32)r3 <= (s32)r0) {
        r24 = 0x1;
    }
    break;

    if ((s32)r3 < (s32)r0) {
        r24 = 0x1;
    }
    break;

    if ((s32)r3 > (s32)r0) {
        r24 = 0x1;
    }
    } while (0);

    r30 = r24;
    r3 = r20;
    fn_8020A860();
    r0 = r3 & 0xFFFF;
    r28 = r3;
    if ((s32)r3 == (s32)r0) {
        r3 = r24;
        return;
    }
    while (1) {
        r27 = (u32)sp + 0x8;
        do {
            r3 = r28;
            fn_8020A540();
            r23 = 0x0;
            r29 = r3;
            while (1) {
                r0 = r23 & 0xFF;
                if ((u32)r0 >= (u32)0x2) break;
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
                if ((s32)r0 != (s32)0x2) {
                    if ((s32)r0 < (s32)0x2) {
                        if ((s32)r0 == (s32)0x0) goto L_8020AC9C;
                        if ((s32)r0 < (s32)0x0) {
                            goto L_8020AC9C;
                        }
                        if ((s32)r0 >= (s32)0x4) goto L_8020AC9C;
                        goto L_8020AC20;
                        }
                    r20 = r21 & 0xFFFF;
                    goto L_8020AC9C;
                }
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
                if ((s32)r0 == (s32)0x4) {
                    r20 = 0x0;
                    goto L_8020ACC0;
                }
                r3 = r21;
                fn_801F0234();
                fn_801F021C();
                r0 = r26 & 0xFF;
                if ((s32)r0 == (s32)0x4) {
                    r4 = r20;
                    r6 = r22;
                    r5 = r24 & 0xFFFF;
                    r7 = r25 & 0xFFFF;
                    fn_80135E44();
                    r20 = r3;

                } else {
                    r4 = r20;
                    r6 = r22;
                    r5 = 0x0;
                    r7 = 0x0;
                    fn_80135E44();
                    r20 = r3;
                }
                L_8020AC9C: ;
                r0 = r26 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r3 = (s16)r24;
                    r0 = (s16)r25;
                    r20 = r20 * r3;
                    if ((u32)r0 != (u32)0x1) {
                        r0 = (s16)r25;
                        r20 = (s32)r20 / (s32)r0;
                }
                }
                L_8020ACC0: ;
                /* clrlslwi r0, r23, 24, 2 */;
                r23 = r23 + 0x1;
                *(u32*)(r27 + r0) = r20;

            }
            r3 = r29;
            r23 = 0x0;
            fn_8020A8A0();
            r0 = r3 & 0xFF;
            if ((u32)r0 <= (u32)0x6) {
                r3 = (u32)jumptable_80375938;
                r0 = r0 << 2;
                r3 = (u32)jumptable_80375938;
                r0 = *(u32*)(r3 + r0);
                ctr_fn = (void(*)(void))r0;
                /* indirect jump via ctr */;
                r23 = 0x1;
            }
            do {
            break;

            if ((s32)r3 == (s32)r0) {
                r23 = 0x1;
            }
            break;

            if ((s32)r3 != (s32)r0) {
                r23 = 0x1;
            }
            break;

            if ((s32)r3 >= (s32)r0) {
                r23 = 0x1;
            }
            break;

            if ((s32)r3 <= (s32)r0) {
                r23 = 0x1;
            }
            break;

            if ((s32)r3 < (s32)r0) {
                r23 = 0x1;
            }
            break;

            if ((s32)r3 > (s32)r0) {
                r23 = 0x1;
            }
            } while (0);

            r3 = r29;
            fn_8020A580();
            r0 = r3 & 0xFF;
            if ((s32)r0 != (s32)0x2) {
                if ((s32)r0 >= (s32)0x2) goto L_8020ADFC;
                if ((s32)r0 < (s32)0x1) {
                    goto L_8020ADFC;
                }
                r0 = r30 & 0xFF;
                if ((s32)r0 == (s32)0x1) {
                    r0 = r23 & 0xFF;
                    if ((s32)r0 != (s32)0x1) {
                    }
                    r30 = 0x1;
                    goto L_8020ADFC;
                    }
                r30 = 0x0;

            } else {
                r0 = r30 & 0xFF;
                if ((s32)r0 != (s32)0x1) {
                    r0 = r23 & 0xFF;
                    if ((s32)r0 != (s32)0x1) {
                        r30 = 0x1;
                        goto L_8020ADFC;
                }
                }
                r30 = 0x0;
            }
            L_8020ADFC: ;
            r3 = r28;
            fn_8020A500();
            r0 = r3 & 0xFFFF;
            r28 = r3;
        } while ((s32)r0 != (s32)0x1);
        r3 = r30;
        return;
    }

    return;
}

/* 0x8020AED0 | size: 0x60 */
u32 fn_8020AED0(void* ctx) {
    extern void fn_801F4C14();
    extern u32 fn_8020D8F0();
    extern u32 fn_8020D908();
    extern void fn_80211B94();
    u32 obj;
    u32 data;
    obj = fn_8020D908(ctx);
    fn_801F4C14(0, 0, 0x36, 0, obj);
    data = fn_8020D8F0(ctx);
    fn_80211B94(ctx, data, 0);
    return 1;
}

/* 0x8020AF30 | size: 0xC4 | medium */
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
    r5 = (u32)sp + 0x8;
    r3 = 0x0;
    r6 = 0x0;
    fn_801F37B0();
    if ((u32)r31 != (u32)0x0) {
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
    }
    r3 = 0x1;
    return;
}

/* 0x8020AFF4 | size: 0x5C | small */
void fn_8020AFF4(void) {
    u8 sp[0x10];
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = 0x0;
    r6 = 0x0;
    r31 = r5;
    r30 = r3;
    r5 = 0xee;
    ((void(*)(void))fn_8012640C)();
    if ((u32)r3 != (u32)0x0) {
        if ((u32)r31 != (u32)0x0) {
            *(u32*)((u8*)r31 + 0x0) = r30;
        }
        r3 = 0x0;
    } else {

        r3 = 0x1;
    }
    return;
}

/* 0x8020B058 | size: 0x2D8 | large */
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
    fn_801F54A4();
    r29 = r3 & 0xFFFF;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = 0x0;
    fn_801F2A7C();
    /* mr. r31, r3 */;
    if ((u32)r0 == (u32)0x1 || (u32)r0 == (u32)0x1 || (u32)r0 == (u32)0x1 || (u32)r3 == (u32)0x0) {
        r4 = 0x0;
        r5 = 0x44;
        r6 = 0x0;
        fn_801FB1C0();
        /* mr. r30, r3 */;

        r3 = r31;
        r4 = 0x0;
        fn_801F86C0();
        fn_801EF634();
        r4 = r3;
        r3 = 0x0;
        fn_801F1DBC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x0;
            r4 = 0x0;
            r5 = 0x24;
            r6 = 0x0;
            fn_801F54A4();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r3 = r31;
                fn_801F9034();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r26 = 0x0;
                    while (1) {
                        r0 = r26 & 0xFFFF;
                        if ((u32)r0 >= (u32)0x6) break;
                        r3 = r30;
                        r5 = r26 & 0xFFFF;
                        r4 = 0x3;
                        fn_8012A5B0();
                        r27 = r3;
                        fn_801233F4();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 != (u32)0x1) {
                            r3 = r27;
                            fn_8011FC74();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 != (u32)0x1) {
                                r3 = r31;
                                r4 = r27;
                                fn_801F9930();
                                /* mr. r25, r3 */;
                                if ((u32)r0 != (u32)0x1) {
                                    fn_80206608();
                                    r0 = r3 & 0xFF;
                                    if ((u32)r0 != (u32)0x1) {
                                        r3 = r25;
                                        r4 = 0x0;
                                        r5 = 0xd0;
                                        r6 = 0x0;
                                        ((void(*)(void))fn_8012640C)();
                                        r0 = r3 & 0xFF;
                                        if ((u32)r0 != (u32)0x1) {
                                            r3 = r27;
                                            r6 = (u32)sp + 0x8;
                                            r7 = (u32)sp + 0xc;
                                            r4 = 0x0;
                                            r5 = 0x0;
                                            fn_80128A64();
                                            r0 = r3 & 0xFFFF;
                                            r4 = r3;
                                            if ((u32)r0 != (u32)0x1) {
                                                r5 = *(u16*)(sp + 0x8);
                                                r3 = r27;
                                                r7 = r30;
                                                r6 = (u32)sp + 0xc;
                                                r8 = 0x1;
                                                r9 = 0x1;
                                                r10 = 0x0;
                                                fn_8012805C();
                                                r3 = 0xa;
                                                fn_801EFFC4();
                        }
                        }
                        }
                        }
                        }
                        }
                        r26 = r26 + 0x1;

                    }
            }
            }
            r3 = 0x0;
            r4 = 0x0;
            r5 = 0x30;
            r6 = 0x0;
            fn_801F54A4();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r3 = r30;
                fn_80129840();
            }
            r3 = 0x0;
            r4 = 0x0;
            r5 = 0x28;
            r6 = 0x0;
            fn_801F54A4();
        }
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x1c;
        r6 = 0x0;
        fn_801F54A4();
        r0 = r3 & 0xFF;

        r3 = 0x0;
        r4 = 0x2;
        fn_80129280();

        r4 = r30;
        fn_8012AC64();
    }
    fn_8006B57C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) { r3 = 0x1; return; }
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFFFF;
        if ((u32)r0 >= (u32)0x2) break;
        r4 = r26;
        r3 = 0x0;
        fn_801F47B4();
        /* mr. r25, r3 */;
        if ((u32)r0 != (u32)0x1) {
            r0 = r26 & 0xFFFF;
            r27 = 0x0;
            r30 = r0 * r29;
            while (1) {
                r0 = r27 & 0xFFFF;
                if ((u32)r0 >= (u32)r29) break;
                r3 = r25;
                r4 = r27;
                fn_801F7258();
                /* mr. r31, r3 */;
                if ((u32)r0 != (u32)0x1) {
                    r4 = 0x0;
                    fn_801F86C0();
                    r0 = r27 + r30;
                    r3 = r0 & 0xFF;
                    fn_8006B0F8();
                    /* mr. r28, r3 */;
                    if ((u32)r0 != (u32)0x1) {
                        r3 = r31;
                        r4 = 0x0;
                        r5 = 0x44;
                        r6 = 0x0;
                        fn_801FB1C0();
                        /* mr. r4, r3 */;
                        if ((u32)r0 != (u32)0x1) {
                            r3 = r28;
                            fn_8012AC64();
                }
                }
                }
                r27 = r27 + 0x1;

            }
        }
        r26 = r26 + 0x1;

    }

    r3 = 0x1;

    return;
}

/* 0x8020B330 | size: 0x3A4 | large */
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
    if ((u32)r26 == (u32)r3) {
        fn_800896C0();
        if ((u32)r3 == (u32)0x0) {
            r23 = 0x0;
            goto L_8020B458;
        }
        r4 = r3;
        r3 = 0x24;
        fn_80132A38();
        r23 = 0x7531;

    } else {
        r3 = r24;
        r4 = 0x0;
        r5 = 0x4a;
        r6 = 0x0;
        fn_801FB1C0();
        if ((s32)r3 == (s32)0x0) {
            r4 = r26;
            r3 = 0x0;
            r5 = 0x8;
            r6 = 0x2;
            fn_801FB1C0();
            r23 = r3;
            goto L_8020B458;
        }
        r4 = r26;
        r3 = 0x0;
        r5 = 0x8;
        r6 = 0x3;
        fn_801FB1C0();
        /* mr. r23, r3 */;
        if ((s32)r3 == (s32)0x0) {
            r4 = r26;
            r3 = 0x0;
            r5 = 0x8;
            r6 = 0x2;
            fn_801FB1C0();
            r23 = r3;
        }
    }
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
    if ((u32)r0 == (u32)0x1) {
        if ((u32)r30 == (u32)0x2) {
            if ((u32)r23 != (u32)0x0) {
                r3 = r29;
                r4 = 0x5a;
                r5 = 0x4;
                r6 = 0x0;
                fn_801DDD28();
                fn_801C2D54();
                r27 = r3;
            }
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
            if ((u32)r23 != (u32)0x0) {
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
                if ((u32)r23 != (u32)0x0) {
                    fn_800F0308();
                    goto L_8020B53C;
                }
                r3 = r27;
                fn_801EF8F4();
                fn_8026246C();
                r3 = r29;
                r4 = 0x5a;
                r5 = 0x4;
                fn_801DA8C4();
            }
            goto L_8020B654;
        }
        if ((u32)r30 == (u32)0x3) {
            if ((u32)r28 != (u32)0x0) {
                r3 = r29;
                r4 = 0x59;
                r5 = 0x4;
                r6 = 0x0;
                fn_801DDD28();
                fn_801C2D54();
                r27 = r3;
            }
            r3 = 0x7547;
            fn_802624CC();
            fn_8026246C();
            if ((u32)r28 != (u32)0x0) {
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
                if ((u32)r28 != (u32)0x0) {
                    fn_800F0308();
                    goto L_8020B5D4;
                }
                r3 = r27;
                fn_801EF8F4();
                fn_8026246C();
                r3 = r29;
                r4 = 0x59;
                r5 = 0x4;
                fn_801DA8C4();
            }
            r3 = 0x7548;
            fn_802624CC();
            fn_8026246C();
            goto L_8020B654;
        }
        /* subi r0, r30, 0x4 */;
        r0 = r0 & 0xFFFF;
        if ((u32)r0 <= (u32)0x1 || (u32)r30 != (u32)0x7 && (u32)r30 != (u32)0x6) goto L_8020B654;

        if ((u32)r30 == (u32)0x7 || (u32)r30 == (u32)0x6) {

            r3 = 0x7640;
            fn_802624CC();
            r3 = 0x40;
            fn_801F000C();
            fn_8026246C();
        }
    }
    L_8020B654: ;
    fn_801EF634();
    r4 = r3;
    r3 = 0x0;
    fn_801F1DBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x25;
        r6 = 0x0;
        fn_801F54A4();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
    }
    }
    fn_801EF2D4();
    r3 = 0x1;
    return;
}

/* 0x8020B6D4 | size: 0x58 | small */
void fn_8020B6D4(void) {
    extern void fn_8016597C();
    extern void fn_801F000C();
    extern void fn_801F54A4();
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x12;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r3 != (u32)0x0) {
        r3 = 0x1;
        r4 = 0x3e8;
        r5 = 0x3e8;
        r6 = 0xff;
        fn_8016597C();
        r3 = 0x3c;
        fn_801F000C();
    }
    r3 = 0x1;
    return;
}

/* 0x8020B72C | size: 0x1E4 | medium */
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
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
    } else {

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
        if ((s32)r0 == (s32)0) {
            r29 = r29 & 0xFF;
            r3 = r25;
            r4 = 0x0;
            r5 = 0x7f;
            r6 = r29;
            ((void(*)(void))fn_8012640C)();
            r3 = r3 & 0xFFFF;
            r0 = r26 & 0xFFFF;
            if ((u32)r0 != (u32)r3) {
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
        }
        }
        r3 = r31;
        r4 = r26;
        fn_802128D0();
        r3 = 0x1;
    }
    return;
}

/* 0x8020B910 | size: 0x104 */
u32 fn_8020B910(void* ctx) {
    extern u32 fn_801F00D0();
    extern void fn_801F4C14();
    extern u16 fn_801F54A4();
    extern u32 fn_8020D908();
    extern void fn_80211E18();
    u16 partyCount;
    u32 d908val;
    void* e5Data;
    u16 field1E;
    u8 slotType;
    u32 finalVal;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    d908val = fn_8020D908(ctx);
    fn_801F4C14(0, 0, 0x36, 0, d908val);
    e5Data = fn_8012640C((void*)d908val, 0, 0xE5, 0);
    field1E = (u16)fn_80142CF4((u32)e5Data, 0, 0x1E, 0);
    slotType = (u8)fn_80142CF4(0, field1E, 0x2, 0);
    if ((u8)slotType == 1) {
        finalVal = (u32)fn_801F00D0((u16)fn_80142CF4((u32)e5Data, 0, 0x1F, 0), partyCount);
    } else {
        finalVal = d908val;
    }
    fn_801F4C14(0, 0, 0x42, 0, finalVal);
    fn_80211E18(ctx, field1E);
    return 1;
}

/* 0x8020BA14 | size: 0x6C | small */
void fn_8020BA14(void) {
    extern void fn_801F4C14();
    extern void fn_801F54A4();
    extern void fn_8020D908();
    extern void fn_80212D6C();
    u8 sp[0x10];
    u32 r0 = 0;
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
    return;
}

/* 0x8020BA80 | size: 0x78 | small */
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
    r3 = 0x1;
    return;
}

/* 0x8020BAF8 | size: 0xAC */
void fn_8020BAF8(void* ctx) {
    extern u8 fn_801F0058();
    extern u8 fn_801F3984();
    extern void fn_801F4C14();
    extern u16 fn_801F54A4();
    extern u32 fn_8020D908();
    extern void fn_80212840();
    u16 tableId;
    u32 obj;
    u8 result;
    tableId = fn_801F54A4(NULL, 0, 0x14, 0);
    obj = fn_8020D908(ctx);
    fn_80212840(obj);
    if (fn_801F0058(obj, tableId) == 1) {
        result = fn_801F3984(0, 4);
    } else {
        result = fn_801F3984(0, 5);
    }
    if (result == 1) {
        fn_801F4C14(0, 0, 0x44, 0, obj);
    }
}

/* 0x8020BBA4 | size: 0x58 | small */
void fn_8020BBA4(void) {
    extern void fn_801EF634();
    extern void fn_801F000C();
    extern void fn_801F4AC0();
    extern void fn_802119D4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r31 = 0;

    r31 = r3;
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) {
        r3 = 0x1;
    } else {

        r3 = r31;
        fn_802119D4();
        r3 = 0x5;
        fn_801F000C();
        r3 = 0x0;
        fn_801F4AC0();
        r3 = 0x1;
    }
    return;
}

/* 0x8020BBFC | size: 0x98 | medium */
void fn_8020BBFC(void) {
    extern void fn_801EF634();
    extern void fn_801F3B24();
    extern void fn_801F4718();
    extern void fn_8020BC94();
    extern void fn_80211A00();
    u8 sp[0x10];
    u32 r0 = 0;
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
    if ((u32)r0 != (u32)0x1) {
        return;
    }
    fn_801EF634();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x1) {
        r3 = 0x1;
        return;
    }
    r3 = r31;
    fn_80211A00();
    r3 = 0x0;
    r4 = 0x1;
    fn_8020BC94();
    r0 = r3 & 0xFF;
    r4 = 0x1;
    if ((u32)r0 != (u32)0x1) {
        r4 = r3;
    }
    r3 = r4;

    return;
}

/* 0x8020BC94 | size: 0x1A4 | medium */
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

    r29 = r3;
    r30 = r4;
    r31 = 0x0;
    while (1) {
        r0 = r31 & 0xFFFF;
        if ((u32)r0 >= (u32)0x8) break;
        r3 = r29;
        r6 = r31;
        r4 = 0x0;
        r5 = 0x59;
        fn_801F54A4();
        /* mr. r27, r3 */;
        if ((s32)r0 != (s32)0) {
            fn_802062FC();
            r0 = r3 & 0xFF;
            if ((s32)r0 == (s32)0) {
                r3 = r27;
                r4 = 0x0;
                r5 = 0x112;
                r6 = 0x0;
                r7 = 0x1;
                ((void(*)(void))fn_801254B4)();
            } else {
                r3 = r27;
                r4 = 0x0;
                r5 = 0xfe;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                /* mr. r28, r3 */;
                if ((s32)r0 == (s32)0) {
                    r3 = r27;
                    r4 = 0x0;
                    r5 = 0x112;
                    r6 = 0x0;
                    r7 = 0x1;
                    ((void(*)(void))fn_801254B4)();
                } else {
                    fn_801F1170();
                    r0 = r3 & 0xFF;
                    if ((s32)r0 == (s32)0) {
                        r3 = r27;
                        r4 = 0x0;
                        r5 = 0x112;
                        r6 = 0x0;
                        r7 = 0x1;
                        ((void(*)(void))fn_801254B4)();
                    } else {
                        r0 = r30 & 0xFF;
                        r3 = r28;
                        fn_801F0898();
                        r0 = r3 & 0xFFFF;
                        if (((s32)(r30 & 0xFF) == (s32)0 && (u32)r0 == (u32)0x8) ||
                            ((s32)(r30 & 0xFF) != (s32)0 && (u32)r0 != (u32)0x8)) {
                            r3 = r27;
                            r4 = 0x0;
                            r5 = 0x112;
                            r6 = 0x0;
                            ((void(*)(void))fn_8012640C)();
                            if ((s32)r3 != (s32)0x1) {
                                r3 = r27;
                                r4 = 0x0;
                                r5 = 0x112;
                                r6 = 0x0;
                                r7 = 0x1;
                                ((void(*)(void))fn_801254B4)();
                                r0 = 0x6;
                                r5 = (u32)sp + 0x4;
                                /* subi r4, r28, 0x4 */;
                                ctr_fn = (void(*)(void))r0;
                                do {
                                    r3 = *(u32*)((u8*)r4 + 0x4);
                                    r0 = *(u32*)((u8*)r4 + 0x8);
                                    *(u32*)((u8*)r5 + 0x4) = r3;
                                    r5 += 8; *(u32*)r5 = r0;
                                } while (--ctr != 0);
                                r3 = (u32)sp + 0x8;
                                fn_801F0F04();
                                r0 = r30 & 0xFF;
                                if ((s32)r3 != (s32)0x1) {
                                    r3 = 0x0;
                                    fn_801F4AC0();
                                    fn_801EF634();
                                    r0 = r3 & 0xFFFF;
                                    if ((s32)r3 != (s32)0x1) {
                                        r3 = 0x1;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        r31 = r31 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* 0x8020BE38 | size: 0x108 */
u32 fn_8020BE38(void) {
    extern u8 fn_80008174();
    extern void fn_801F2B5C();
    extern void* fn_801F47B4();
    extern u16 fn_801F54A4();
    extern void* fn_801F7258();
    extern u8 fn_8026316C();
    extern u32 fn_8020BF40();
    u8 checkResult;
    u16 partyCount;
    u16 slotCount;
    u16 i;
    u16 j;
    void* slotData;
    void* entry;

    checkResult = fn_80008174();
    if ((u8)checkResult != 1) {
        fn_801F2B5C(0, (u32)fn_8020BF40, 0, 1);
    } else {
        partyCount = fn_801F54A4(0, 0, 0x14, 0);
        slotCount = fn_801F54A4(0, 0, 0x16, 0);
        for (i = 0; (u16)i < 2; i++) {
            slotData = fn_801F47B4(0, i);
            if (!slotData) { continue; }
            for (j = 0; (u16)j < slotCount; j++) {
                entry = fn_801F7258(slotData, j);
                if (entry == NULL) { continue; }
                if ((u8)fn_8026316C(entry, partyCount, checkResult) != 0) { continue; }
                if ((u16)i == 0) { continue; }
                i--;
                break;
            }
        }
    }
    return 1;
}

/* 0x8020BF40 | size: 0x60 */
u32 fn_8020BF40(void* ctx, u32 param) {
    extern u16 fn_801EF634();
    extern void fn_801F150C();
    extern u8 fn_801F923C();
    if (fn_801EF634() != 0) {
        return 1;
    }
    if (fn_801F923C(ctx, param) == 0) {
        fn_801F150C(0);
    }
    return 1;
}

/* 0x8020BFA0 | size: 0x120 */
u32 fn_8020BFA0(void* ctx) {
    extern u8 lbl_80375CC8[];
    extern u8 lbl_80378AA0[];
    extern u16 fn_800E0C54();
    extern void fn_801DA7AC();
    extern void fn_801F2F3C();
    extern void fn_801F3074();
    extern void fn_801F3178();
    extern void fn_801F37B0();
    extern void fn_801F3B24();
    extern void fn_801F4718();
    extern void fn_801F4C14();
    extern u32 fn_8020D920();
    extern void fn_80211830();
    extern void fn_80211948();
    extern void fn_8022E1C4();
    extern void fn_8022E314();
    extern s32 fn_8020C0C0();
    extern s32 fn_8020C0E4();
    extern s32 fn_8020C108();
    u8 localBuf[0x10];
    u16 evtId;

    fn_801F4718(0);
    fn_801F3B24(0, 0);
    fn_80211830();
    localBuf[0] = 0;
    fn_801F37B0(0, (u32)fn_8020C108, &localBuf[0], 0);
    fn_80211948(fn_8020D920(ctx), 0, 6, 0, lbl_80375CC8, lbl_80378AA0);
    fn_801F37B0(0, (u32)fn_8020C0E4, 0, 1);
    fn_8022E314(1);
    fn_8022E1C4();
    fn_801F37B0(0, (u32)fn_8020C0C0, 0, 1);
    localBuf[0] = 1;
    fn_801F37B0(0, (u32)fn_8020C108, &localBuf[0], 0);
    fn_801F3178(0);
    fn_801F3074(0);
    fn_801F2F3C(0);
    evtId = fn_800E0C54();
    fn_801F4C14(0, 0, 0x5B, 0, (u32)evtId);
    fn_801DA7AC();
    return 1;
}

/* 0x8020C0C0 | size: 0x24 | small */
/* fn_8020C0C0 | Size: 0x24 | Call fn_8022D084 and return 1 */
s32 fn_8020C0C0(void) {
    extern void fn_8022D084(void);
    fn_8022D084();
    return 1;
}

/* 0x8020C0E4 | size: 0x24 | small */
/* fn_8020C0E4 | Size: 0x24 | Call fn_8022E410 and return 1 */
s32 fn_8020C0E4(void) {
    extern void fn_8022E410(void);
    fn_8022E410();
    return 1;
}

/* fn_8020C108 | Size: 0x54 | Apply effect with optional data parameter */
s32 fn_8020C108(void* ctx, u32 unused, u8* data) {
    extern void fn_8022E6F0(void* ctx, u32 value);
    if (data != NULL) {
        fn_8022E6F0(ctx, data[0]);
    } else {
        fn_8022E6F0(ctx, 0);
        fn_8022E6F0(ctx, 1);
    }
    return 1;
}

/* 0x8020C15C | size: 0x6E4 | large */
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
    if ((s32)r0 == (s32)0) {
        r28 = 0x5f;
    }
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
    if ((s32)r0 == (s32)0) {
        if ((u32)r30 != (u32)0x0) {
            if ((u32)r31 != (u32)0x0) {
                r3 = r26;
                r4 = r31 & 0xFFFF;
                r5 = 0x4;
                fn_801DA9E8();
            }
            r3 = r26;
            r4 = r30 & 0xFFFF;
            r5 = 0x4;
            fn_801DA9E8();
            L_8020C2A8: ;
            /* addic. r0, (u32)sp, 0x8 */;
            if ((u32)r31 == (u32)0x0) {
                r0 = 0x0;
            } else {

                r3 = (u32)sp + 0x44;
                r4 = 0x1;
                fn_80103BA8();
                r3 = 0x0;
                fn_801C40F0();
                r0 = (s8)r3;
                if ((u32)r31 == (u32)0x0) {
                    r0 = *(u16*)(sp + 0x44);
                    r0 = r0 & 0x00000020;
                    if ((u32)r31 != (u32)0x0) {
                        r0 = 0x1;
                        *(u8*)(sp + 0x8) = r0;
                }
                }
                r0 = *(u8*)(sp + 0x8);
            }
            r0 = r0 & 0xFF;
            if ((u32)r0 == (u32)0x1) goto L_8020C430;
            r3 = r26;
            r4 = r30 & 0xFFFF;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                fn_800F0308();
                goto L_8020C2A8;
            }
            r3 = 0x0;
            r4 = 0x0;
            r5 = 0x33;
            r6 = 0x0;
            fn_801F54A4();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r4 = *(u16*)lbl_8047B5F8;
                r3 = r26;
                r5 = 0x4;
                fn_801DA9E8();
                do {
                    /* addic. r0, (u32)sp, 0x8 */;
                    if ((u32)r0 == (u32)0x1) {
                        r0 = 0x0;
                    } else {

                        r3 = (u32)sp + 0x28;
                        r4 = 0x1;
                        fn_80103BA8();
                        r3 = 0x0;
                        fn_801C40F0();
                        r0 = (s8)r3;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = *(u16*)(sp + 0x28);
                            r0 = r0 & 0x00000020;
                            if ((u32)r0 != (u32)0x1) {
                                r0 = 0x1;
                                *(u8*)(sp + 0x8) = r0;
                        }
                        }
                        r0 = *(u8*)(sp + 0x8);
                    }
                    r0 = r0 & 0xFF;
                    if ((u32)r0 == (u32)0x1) goto L_8020C430;
                    r4 = *(u16*)lbl_8047B5F8;
                    r3 = r26;
                    r5 = 0x4;
                    fn_801DA94C();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) break;
                    fn_800F0308();
                } while (1);
            }
            if ((u32)r31 != (u32)0x0) {
                do {
                    /* addic. r0, (u32)sp, 0x8 */;
                    if ((u32)r31 == (u32)0x0) {
                        r0 = 0x0;
                    } else {

                        r3 = (u32)sp + 0xc;
                        r4 = 0x1;
                        fn_80103BA8();
                        r3 = 0x0;
                        fn_801C40F0();
                        r0 = (s8)r3;
                        if ((u32)r31 == (u32)0x0) {
                            r0 = *(u16*)(sp + 0xC);
                            r0 = r0 & 0x00000020;
                            if ((u32)r31 != (u32)0x0) {
                                r0 = 0x1;
                                *(u8*)(sp + 0x8) = r0;
                        }
                        }
                        r0 = *(u8*)(sp + 0x8);
                    }
                    r0 = r0 & 0xFF;
                    if ((u32)r0 == (u32)0x1) break;
                    r3 = r26;
                    r4 = r31 & 0xFFFF;
                    r5 = 0x4;
                    fn_801DA94C();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) break;
                    fn_800F0308();
                } while (1);
            }
        }
        L_8020C430: ;
        r0 = *(u8*)(sp + 0x8);
        if ((u32)r0 == (u32)0x1) {
            f1 = *(f32*)lbl_8047E520;
            r3 = 0x3;
            fn_801C41C8();
            r3 = 0x1;
            fn_801C40F0();
            if ((u32)r31 != (u32)0x0) {
                r3 = r26;
                r4 = r31 & 0xFFFF;
                r5 = 0x4;
                fn_801DA9B4();
            }
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
            if ((u32)r0 == (u32)0x1) {
                r4 = *(u16*)lbl_8047B5F8;
                r3 = r26;
                r5 = 0x4;
                fn_801DA9B4();
        }
        }
        if ((u32)r27 != (u32)0x0) {
            r3 = r26;
            r4 = 0x5f;
            r5 = 0x4;
            fn_801DA9E8();
            r0 = *(u8*)(sp + 0x8);
            if ((u32)r0 == (u32)0x1) {
                f1 = *(f32*)lbl_8047E520;
                r3 = 0x2;
                fn_801C41C8();
                r0 = 0x0;
                *(u8*)(sp + 0x8) = r0;
            }
            r3 = r27;
            fn_80262490();
            L_8020C4E8: ;
            r3 = r26;
            r4 = 0x5f;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                fn_800F0308();
                goto L_8020C4E8;
            }
            fn_8026246C();
        }
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x11;
        r6 = 0x0;
        fn_801F54A4();
        if ((u32)r3 != (u32)0x0) {
            r4 = 0x0;
            r5 = 0xff;
            fn_80165A20();
        }
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x33;
        r6 = 0x0;
        fn_801F54A4();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            if ((u32)r0 == (u32)0x1) {
                f1 = *(f32*)lbl_8047E520;
                r3 = 0x2;
                fn_801C41C8();
                r0 = 0x0;
                *(u8*)(sp + 0x8) = r0;
            }
            r3 = 0x766d;
            fn_80262490();
            L_8020C5B4: ;
            r3 = r26;
            r4 = r28 & 0xFFFF;
            r5 = 0x4;
            fn_801DA94C();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                fn_800F0308();
                goto L_8020C5B4;
            }
            fn_8026246C();
        }
        r0 = *(u8*)(sp + 0x8);
        if ((u32)r0 == (u32)0x1) {
            f1 = *(f32*)lbl_8047E520;
            r3 = 0x2;
            fn_801C41C8();
        }
        if ((u32)r30 != (u32)0x0) {
            if ((u32)r31 != (u32)0x0) {
                r3 = r26;
                r4 = r31 & 0xFFFF;
                r5 = 0x4;
                fn_801DA8C4();
            }
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
            if ((u32)r0 == (u32)0x1) {
                r4 = *(u16*)lbl_8047B5F8;
                r3 = r26;
                r5 = 0x4;
                fn_801DA8C4();
        }
        }
        if ((u32)r27 != (u32)0x0) {
            r3 = r26;
            r4 = 0x5f;
            r5 = 0x4;
            fn_801DA8C4();
        }
        r3 = 0x0;
        r4 = 0x0;
        r5 = 0x33;
        r6 = 0x0;
        fn_801F54A4();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x1) { r3 = 0x1; return; }
        r3 = r26;
        r4 = r28 & 0xFFFF;
        r5 = 0x4;
        fn_801DA8C4();
        r3 = 0x1;
        return;
    }
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
    if ((u32)r0 != (u32)0x1) {
        fn_800F0308();
        goto L_8020C6C0;
    }
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
    if ((u32)r0 == (u32)0x1) {
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
    }
    L_8020C754: ;
    r3 = r26;
    r4 = 0x55;
    r5 = 0x4;
    fn_801DA94C();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
        fn_800F0308();
        goto L_8020C754;
    }
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x33;
    r6 = 0x0;
    fn_801F54A4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        fn_8026246C();
    }
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
    if ((u32)r0 != (u32)0x1) {
        fn_800F0308();
        goto L_8020C7B0;
    }
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
    if ((u32)r3 == (u32)0x0) { r3 = 0x1; return; }
    r4 = 0x0;
    r5 = 0xff;
    fn_80165A20();

    r3 = 0x1;
    return;
}

/* 0x8020CA98 | size: 0x548 | large */
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
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r3 = r30;
    fn_801F7388();
    r27 = r3 & 0xFF;
    r23 = 0x0;
    while (1) {
        r0 = r23 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r3 = r30;
        r4 = r23;
        fn_801F7258();
        /* mr. r25, r3 */;
        if ((s32)r0 != (s32)0) {
            r4 = 0x0;
            r5 = 0x4c;
            r6 = 0x0;
            fn_801FB1C0();
            /* mr. r21, r3 */;
            if ((s32)r0 != (s32)0) {
                r3 = r25;
                fn_801F98BC();
                r24 = 0x0;
                r22 = 0x0;
                while (1) {
                    r0 = r22 & 0xFFFF;
                    if ((u32)r0 >= (u32)0x6) break;
                    r0 = r24 & 0xFFFF;
                    if ((u32)r0 >= (u32)r31 || (u32)r0 >= (u32)0x2) break;

                    r3 = r25;
                    r6 = r22;
                    r4 = 0x0;
                    r5 = 0x45;
                    fn_801FB1C0();
                    r20 = r3;
                    fn_80206608();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x2) {
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
                        if ((u32)r0 == (u32)0x1) {
                            r3 = r26;
                            fn_80204A10();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 == (u32)0x1) {
                                r3 = r26;
                                r4 = 0x0;
                                fn_80205AD4();
                                r3 = r26;
                                r4 = 0x0;
                                fn_80205A7C();
                        }
                        }
                        r3 = r21;
                        r4 = r19;
                        fn_801C3E3C();
                    }
                    r22 = r22 + 0x1;

                }
        }
        }
        r23 = r23 + 0x1;

    }
    r21 = 0x0;
    while (1) {
        r0 = r21 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r3 = r30;
        r4 = r21;
        fn_801F7258();
        /* mr. r22, r3 */;
        if ((u32)r0 != (u32)r28) {
            r23 = 0x0;
            while (1) {
                r0 = r23 & 0xFFFF;
                if ((u32)r0 >= (u32)r31) break;
                r3 = r22;
                r4 = r23;
                fn_801F981C();
                /* mr. r26, r3 */;
                if ((u32)r0 != (u32)r28) break;
                r23 = r23 + 0x1;

            }

            r3 = r22;
            r4 = r26;
            r5 = 0x0;
            fn_801FBC20();
            r23 = 0x0;
            while (1) {
                r0 = r23 & 0xFFFF;
                if ((u32)r0 >= (u32)r31) break;
                r3 = r22;
                r4 = r23;
                fn_801F981C();
                /* mr. r26, r3 */;
                if ((u32)r0 != (u32)r31) {
                    r4 = 0x0;
                    fn_80208C18();
                }
                r23 = r23 + 0x1;

            }
        }
        r21 = r21 + 0x1;

    }
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r3 = r30;
        r4 = r25;
        fn_801F7258();
        /* mr. r24, r3 */;
        if ((u32)r0 != (u32)r28) {
            fn_801F98BC();
            r19 = r3 & 0xFF;
            r21 = 0x0;
            while (1) {
                r0 = r21 & 0xFFFF;
                if ((u32)r0 >= (u32)r31) break;
                r3 = r24;
                r4 = r21;
                fn_801F981C();
                /* mr. r26, r3 */;
                if ((u32)r0 != (u32)r28) {
                    r3 = r24;
                    r4 = r26;
                    r5 = r27;
                    r6 = r19;
                    r7 = r25;
                    r8 = r21;
                    r9 = 0x0;
                    fn_8020CFE0();
                }
                r21 = r21 + 0x1;

            }
            r21 = 0x0;
            while (1) {
                r0 = r21 & 0xFFFF;
                if ((u32)r0 >= (u32)r31) break;
                r3 = r24;
                r4 = r21;
                fn_801F981C();
                /* mr. r26, r3 */;
                if ((u32)r0 != (u32)r31) break;
                r21 = r21 + 0x1;

            }

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
            while (1) {
                r0 = r21 & 0xFFFF;
                if ((u32)r0 >= (u32)r31) break;
                r3 = r24;
                r4 = r21;
                fn_801F981C();
                /* mr. r26, r3 */;
                if ((u32)r0 != (u32)r31) {
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
                    if ((u32)r0 == (u32)r31) {
                        r3 = r26;
                        r4 = r29;
                        r5 = 0x0;
                        fn_80265598();
                    } else {

                        r3 = r26;
                        r4 = r29;
                        r5 = 0x1;
                        fn_80265598();
                    }
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
                }
                r21 = r21 + 0x1;

            }
        }
        r25 = r25 + 0x1;

    }
    fn_8026246C();
    r21 = 0x0;
    while (1) {
        r0 = r21 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r3 = r30;
        r4 = r21;
        fn_801F7258();
        /* mr. r22, r3 */;
        if ((u32)r0 != (u32)r28) {
            r23 = 0x0;
            while (1) {
                r0 = r23 & 0xFFFF;
                if ((u32)r0 >= (u32)r31) break;
                r3 = r22;
                r4 = r23;
                fn_801F981C();
                /* mr. r26, r3 */;
                if ((u32)r0 != (u32)r28) break;
                r23 = r23 + 0x1;

            }

            r3 = r22;
            r4 = r26;
            r5 = 0x3;
            fn_801FBC20();
            r23 = 0x0;
            while (1) {
                r0 = r23 & 0xFFFF;
                if ((u32)r0 >= (u32)r31) break;
                r3 = r22;
                r4 = r23;
                fn_801F981C();
                /* mr. r26, r3 */;
                if ((u32)r0 != (u32)r31) {
                    r4 = 0x5;
                    fn_80208C18();
                }
                r23 = r23 + 0x1;

            }
        }
        r21 = r21 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* 0x8020CFE0 | size: 0x21C | large */
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
    if ((u32)r3 == (u32)0x0) {
        r0 = r30 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
            r29 = 0x1;
    }
    }
    r3 = r25;
    fn_80205B8C();
    r4 = 0x0;
    r5 = 0x77;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r31 & 0xFF;
    r31 = r3;
    if ((u32)r3 == (u32)0x0) {
        r0 = r29 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r28 & 0xFFFF;
            if ((u32)r0 == (u32)0x1) {
                r4 = r31;
                r3 = 0x14;
                fn_80132A38();
                r4 = r31;
                r3 = 0x16;
                fn_80132A38();
                return;
            }
            r4 = r31;
            r3 = 0x15;
            fn_80132A38();
            r4 = r31;
            r3 = 0x17;
            fn_80132A38();
            return;
        }
        r0 = r30 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r0 = r28 & 0xFFFF;
            if ((u32)r0 == (u32)0x1) {
                r4 = r31;
                r3 = 0x15;
                fn_80132A38();
                r4 = r31;
                r3 = 0x17;
                fn_80132A38();
                return;
            }
            r4 = r31;
            r3 = 0x14;
            fn_80132A38();
            r4 = r31;
            r3 = 0x16;
            fn_80132A38();
            return;
        }
        r0 = r28 & 0xFFFF;
        if ((u32)r0 == (u32)0x1) {
            r4 = r31;
            r3 = 0x14;
            fn_80132A38();
            r4 = r31;
            r3 = 0x16;
            fn_80132A38();
            return;
        }
        r4 = r31;
        r3 = 0x15;
        fn_80132A38();
        r4 = r31;
        r3 = 0x17;
        fn_80132A38();
        return;
    }
    if ((u32)r0 != (u32)0x1) return;
    r0 = r26 & 0xFFFF;
    if ((u32)r0 <= (u32)0x1) {
        r0 = r27 & 0xFFFF;
        if ((u32)r0 > (u32)0x1) {
            r0 = r29 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r3 = 0x7674;
                goto L_8020D1E4;
            }
            r0 = r30 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r3 = 0x7679;
                goto L_8020D1E4;
            }
            r3 = 0x7671;
            goto L_8020D1E4;
    }
    }
    r4 = r31;
    r3 = 0x14;
    fn_80132A38();
    r4 = r31;
    r3 = 0x16;
    fn_80132A38();
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x7673;

    } else {
        r0 = r30 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = 0x7678;
            goto L_8020D1E4;
        }
        r3 = 0x7670;
    }
    L_8020D1E4: ;
    fn_802624CC();

    return;
}

/* 0x8020D1FC | size: 0x49C | large */
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
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
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
    while (1) {
        r0 = r24 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
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
        if ((s32)r0 != (s32)0) {
            fn_8006B57C();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r3 = r20;
                fn_8006B0F8();
                r4 = r3;
                r3 = (u32)sp + 0xc;
                fn_8012AC64();
            } else {

                r3 = r18;
                r4 = r19;
                r5 = (u32)sp + 0xc;
                fn_801F9CBC();
            }
            r3 = r18;
            fn_801F8FD8();
            r0 = r3;
            r3 = r26;
            r7 = r0;
            r5 = r18;
            r6 = r19;
            r4 = (u32)sp + 0xc;
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
            if ((u32)r0 != (u32)0x1) {
                r3 = r26;
                r4 = r27;
                r5 = r31;
                fn_801F99C8();
                r21 = 0x0;
                r19 = 0x0;
                while (1) {
                    r0 = r19 & 0xFFFF;
                    if ((u32)r0 >= (u32)0x6) break;
                    r0 = (s8)r21;
                    if ((s32)r0 >= (s32)r31 || (s32)r0 >= (s32)r27 || (s32)r0 >= (s32)0x6) break;

                    r3 = r22;
                    r5 = r19;
                    r4 = 0x3;
                    fn_8012A5B0();
                    r18 = r3;
                    fn_801233F4();
                    r0 = r3 & 0xFF;
                    if ((s32)r0 != (s32)0x6) {
                        r3 = r26;
                        r4 = r18;
                        fn_801F9930();
                        if ((u32)r3 == (u32)0x0) {
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
                            if ((u32)r0 == (u32)0x1) {
                                r3 = 0x0;
                                r4 = 0x0;
                                r5 = 0x2e;
                                r6 = 0x0;
                                fn_801F54A4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r3 = r26;
                                    fn_801FB8F8();
                                    r0 = r3 & 0xFF;
                                    if ((u32)r0 == (u32)0x1) {
                                        r3 = r20;
                                        r4 = 0x3;
                                        fn_802032E4();
                            }
                            }
                            }
                            r21 = r21 + 0x1;
                    }
                    }
                    r19 = r19 + 0x1;

                }

                r20 = 0x0;
                while (1) {
                    r0 = r20 & 0xFFFF;
                    if ((u32)r0 >= (u32)0x6) break;
                    r0 = (s8)r21;
                    if ((s32)r0 >= (s32)r27 || (s32)r0 >= (s32)0x6) break;

                    r3 = r22;
                    r5 = r20;
                    r4 = 0x3;
                    fn_8012A5B0();
                    r18 = r3;
                    fn_80123FBC();
                    r0 = r3 & 0xFF;
                    if ((s32)r0 != (s32)0x6) {
                        r3 = r26;
                        r4 = r18;
                        fn_801F9930();
                        if ((u32)r3 == (u32)0x0) {
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
                            if ((u32)r0 == (u32)0x1) {
                                r3 = 0x0;
                                r4 = 0x0;
                                r5 = 0x2e;
                                r6 = 0x0;
                                fn_801F54A4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r3 = r26;
                                    fn_801FB8F8();
                                    r0 = r3 & 0xFF;
                                    if ((u32)r0 == (u32)0x1) {
                                        r3 = r19;
                                        r4 = 0x3;
                                        fn_802032E4();
                            }
                            }
                            }
                            r21 = r21 + 0x1;
                    }
                    }
                    r20 = r20 + 0x1;

                }
        }
        }
        r24 = r24 + 0x1;

    }
    r3 = r25;
    fn_801F7388();
    r18 = r3 & 0xFF;
    r20 = 0x0;
    while (1) {
        r0 = r20 & 0xFFFF;
        if ((u32)r0 >= (u32)r28) break;
        r3 = r25;
        r4 = r20;
        fn_801F7258();
        if ((u32)r3 != (u32)0x0) {
            r4 = 0x0;
            r5 = 0x4c;
            r6 = 0x0;
            fn_801FB1C0();
            /* mr. r19, r3 */;
            if ((u32)r3 != (u32)0x0) {
                r3 = r30;
                r4 = r18;
                r5 = r20;
                r6 = (u32)sp + 0x9;
                r7 = (u32)sp + 0x8;
                fn_801F72B0();
                r4 = *(u8*)(sp + 0x9);
                r3 = r19;
                r5 = *(u8*)(sp + 0x8);
                fn_801C3FBC();
                fn_801C3430();
                r3 = r19;
                r4 = 0x1;
                fn_801DA4E8();
        }
        }
        r20 = r20 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* 0x8020D698 | size: 0xEC | medium */
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
    r3 = 0x1;
    return;
}

/* fn_8020D7CC | Size: 0x1C | Read signed byte, return -128 if NULL */
s32 fn_8020D7CC(u8* ptr) {
    if (ptr == NULL) {
        return -128;
    }
    return (s8)ptr[0];
}

/* fn_8020D7E8 | Size: 0x2C | Look up entry in 12-byte table (u16 index) */
void* fn_8020D7E8(u16 index) {
    extern u8 lbl_80375BB8[];
    extern u32 lbl_80478D48;
    if (index >= lbl_80478D48) {
        return NULL;
    }
    return &lbl_80375BB8[index * 12];
}

/* fn_8020D844 | Size: 0x24 | Store value at indexed slot (max 4) */
void fn_8020D844(u8* ptr, u16 index, u32 value) {
    if (ptr == NULL) {
        return;
    }
    if (index >= 4) {
        return;
    }
    *(u32*)(ptr + (u32)index * 4 + 0x20) = value;
}

/* 0x8020D968 | size: 0x38 | small */
void fn_8020D968(void* dst, void* src) {
    u32 i;
    if (dst == 0) return;
    if (src == 0) return;
    for (i = 0; i < 6; i++) {
        ((u32*)dst)[i] = ((u32*)src)[i];
    }
}

/* fn_8020D9E8 | Size: 0x2C | Look up entry in 6-byte table (u16 index) */
void* fn_8020D9E8(u16 index) {
    extern u8 lbl_80375CB8[];
    extern u32 lbl_80478D50;
    if (index >= lbl_80478D50) {
        return NULL;
    }
    return &lbl_80375CB8[index * 6];
}

/* 0x8020DA14 | size: 0xBC | medium */
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
    if ((u32)r0 == (u32)0x1) {
        r4 = r30;
        r5 = r27;
        r3 = r31 & 0xFFFF;
        r6 = 0x0;
        fn_80136078();
        r29 = 0x1;
    }
    r3 = r29;
    return;
}

/* 0x8020DAD0 | size: 0x274 | large */
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
    r27 = r3;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
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
    if ((u32)r3 != (u32)0x0) {
        fn_8020E260();
        r0 = r3 & 0xFF;
        if ((u32)r3 != (u32)0x0) {
            r3 = r29;
            r4 = 0x1;
            fn_8020E068();
            fn_801FCCC4();
            if ((u32)r3 != (u32)0x0) {
                fn_801FCC7C();
                r28 = r3;
                fn_80261388();
                r0 = r3 & 0xFFFF;
                if ((u32)r3 != (u32)0x0) {
                    fn_80261444();
                    r3 = r3 & 0xFFFF;
                    if ((u32)r3 != (u32)0x0) {
                        fn_80190528();
                }
                }
                r4 = r28;
                r3 = 0x59;
                fn_80132A38();
    }
    }
    }
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
    if ((u32)r3 != (u32)0x0) {
        r4 = 0x0;
        r5 = 0x0;
        fn_80166AB8();
    }
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
    if ((u32)r3 != (u32)0x0) {
        fn_801EF634();
        r4 = r3;
        r3 = 0x0;
        fn_801F1DBC();
        r0 = r3 & 0xFF;
        if ((u32)r3 == (u32)0x0) {
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
            return;
    }
    }
    r3 = 0x9b0;
    fn_80190528();
    fn_80112700();
    fn_801140C8();
    r3 = 0xe05;
    fn_801902E0();
    r0 = r3 & 0xFF;
    if ((u32)r3 == (u32)0x0) {
        f1 = *(f32*)lbl_8047E52C;
        r3 = 0x2;
        fn_801C41C8();
        r3 = 0x1;
        fn_801C40F0();
    }
    r3 = 0x0;
    fn_801EF61C();
    fn_801EF634();

    return;
}

/* 0x8020DD44 | size: 0x3C | small */
void fn_8020DD44(void) {
    extern void fn_801F54A4();
    extern u32 fn_8020E0B0();
    extern void fn_8020E0F8();
    u32 val;
    fn_8020E0F8();
    val = fn_8020E0B0();
    fn_801F54A4(0, val, 0x7, 0);
}

/* 0x8020DD80 | size: 0xD0 | medium */
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

    r30 = r3;
    fn_8020E0F8();
    r31 = r3;
    fn_8020E008();
    if ((u32)r3 != (u32)0x0) {
        return;
    }
    r3 = r30;
    fn_8020E0F8();
    fn_8020E0B0();
    r0 = r3;
    r3 = 0x0;
    r4 = r0;
    r5 = 0x6;
    r6 = 0x0;
    fn_801F54A4();
    if ((u32)r3 != (u32)0x0) {
        return;
    }
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFFFF;
        if ((u32)r0 >= (u32)0x4) break;
        r3 = r31;
        r4 = r30 & 0xFF;
        fn_8020E068();
        r0 = r3 & 0xFFFF;
        if ((u32)r3 != (u32)0x0) {
            r4 = r3;
            r3 = 0x0;
            r5 = 0x4;
            r6 = 0x0;
            fn_801FB1C0();
            r3 = r3 & 0xFFFF;
            if ((u32)r3 != (u32)0x0) {
                fn_801FBD58();
                fn_801FBD10();
                if ((u32)r3 != (u32)0x0) {
                    return;
        }
        }
        }
        r30 = r30 + 0x1;

    }
    r3 = 0x1;

    return;
}

/* fn_8020DE68 | Size: 0x18 | Get float from ptr+4, or default if NULL */
f32 fn_8020DE68(u8* ptr) {
    extern f32 lbl_8047E530;
    if (ptr == NULL) {
        return lbl_8047E530;
    }
    return *(f32*)(ptr + 0x4);
}

/* fn_8020DEB0 | Size: 0x28 | Look up entry in 12-byte table */
void* fn_8020DEB0(u32 index) {
    extern u8 lbl_80375980[];
    extern u32 lbl_80478D20;
    if (index >= lbl_80478D20) {
        return NULL;
    }
    return &lbl_80375980[index * 12];
}

/* fn_8020DF10 | Size: 0x40 | Write u32 to slot in 8-byte array at offset 0x18 */
void fn_8020DF10(u8* base, u8 slot, u32 value) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return;
    }
    *(u32*)(entry + 0x4) = value;
}

/* fn_8020DF50 | Size: 0x40 | Write u16 to slot in 8-byte array at offset 0x18 */
void fn_8020DF50(u8* base, u8 slot, u16 value) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + (u32)slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return;
    }
    *(u16*)(entry) = value;
}

/* fn_8020E020 | Size: 0x48 | Read u32 from slot in 8-byte array at offset 0x18 */
u32 fn_8020E020(u8* base, u8 slot) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + (u32)slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return 0;
    }
    return *(u32*)(entry + 0x4);
}

/* fn_8020E068 | Size: 0x48 | Read u16 from slot in 8-byte array at offset 0x18 */
u16 fn_8020E068(u8* base, u8 slot) {
    u8* entry;
    if (base == NULL) {
        entry = NULL;
    } else if (slot >= 4) {
        entry = NULL;
    } else {
        entry = base + (u32)slot * 8 + 0x18;
    }
    if (entry == NULL) {
        return 0;
    }
    return *(u16*)(entry);
}

/* fn_8020E0F8 | Size: 0x2C | Look up entry in 0x38-byte table (indirect) */
void* fn_8020E0F8(u16 index) {
    extern u32* lbl_80478F50;
    extern u8* lbl_80478F54;
    if (index >= *lbl_80478F50) {
        return NULL;
    }
    return lbl_80478F54 + index * 0x38;
}

/* 0x8020E124 | size: 0x80 | small */
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
    if ((u32)r7 > (u32)r6) {
        r4 = 0x0;
    } else {

        r4 = *(u32*)lbl_80478F04;
        /* clrlslwi r0, r3, 16, 3 */;
        r4 = r4 + r0;
    }
    if ((u32)r4 == (u32)0x0) {
        r5 = 0x0;
    } else {

        r5 = *(u8*)((u8*)r4 + 0x0);
    }
    if ((u32)r7 > (u32)r6) {
        r4 = 0x0;
    } else {

        r4 = *(u32*)lbl_80478F04;
        /* clrlslwi r0, r3, 16, 3 */;
        r4 = r4 + r0;
    }
    r3 = r5 & 0xFF;
    if ((u32)r4 == (u32)0x0) {
        r0 = 0x0;
    } else {

        r0 = *(u8*)((u8*)r4 + 0x2);
    }
    r0 = r0 & 0xFF;
    r0 = r3 * r0;
    r3 = r0 & 0xFFFF;
    return;
}

/* fn_8020E204 | Size: 0x2C | Look up entry in 8-byte table (indirect) */
void* fn_8020E204(u16 index) {
    extern u32 lbl_80478F00;
    extern u32 lbl_80478F04;
    u32* countPtr = (u32*)lbl_80478F00;
    if (index > *countPtr) {
        return NULL;
    }
    return (u8*)lbl_80478F04 + (u32)index * 8;
}

/* fn_8020E488 | Size: 0x2C | Look up entry in 32-byte table (indirect) */
void* fn_8020E488(u16 index) {
    extern u32 lbl_80478F40;
    extern u32 lbl_80478F44;
    u32* countPtr = (u32*)lbl_80478F40;
    if (index > *countPtr) {
        return NULL;
    }
    return (u8*)lbl_80478F44 + (u32)index * 32;
}

/* 0x8020E4CC | size: 0x1C | tiny */
/* fn_8020E4CC | Size: 0x1C | Clamp value to [0, 12] */
s32 fn_8020E4CC(s32 value) {
    if (value < 0) {
        value = 0;
    }
    if (value > 12) {
        value = 12;
    }
    return value;
}

/* 0x8020E4E8 | size: 0x94 | medium */
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
    if ((u32)r8 >= (u32)r7) {
        r5 = 0x0;
    }
    if ((u32)r5 == (u32)0x0) {
        r6 = 0x0;

    } else if ((u32)r5 == (u32)0x0) {
        r6 = 0x0;

    } else {
        r6 = *(u8*)((u8*)r5 + 0x0);
    }
    r5 = (u32)lbl_80375D10;
    /* clrlslwi r3, r3, 16, 1 */;
    r0 = (u32)lbl_80375D10;
    r5 = r0 + r3;
    if ((u32)r8 >= (u32)r7) {
        r5 = 0x0;
    }
    r0 = r6 & 0xFF;
    r3 = r4 * r0;
    if ((u32)r5 == (u32)0x0) {
        r0 = 0x1;

    } else if ((u32)r5 == (u32)0x0) {
        r0 = 0x1;

    } else {
        r0 = *(u8*)((u8*)r5 + 0x1);
    }
    r0 = r0 & 0xFF;
    r3 = (u32)r3 / (u32)r0;
    return;
}

/* 0x8020E57C | size: 0x98 | medium */
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

    /* mr. r27, r3 */;
    r28 = r5;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    while (1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r0 = r29 & 0xFFFF;
        r0 = r0 * 0xc;
        r30 = r27 + r0;
        r3 = r30;
        fn_801FD104();
        r3 = -r3;
        /* subic r0, r3, 0x1 */;
        r0 = r3 - r0; /* -borrow */;
        r0 = r0 & 0xFF;
        if ((s32)r0 != (s32)0) {
            r3 = r30;
            fn_801FD104();
            if ((u32)r3 == (u32)r28) {
                r3 = r30;
                return;
        }
        }
        r29 = r29 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* fn_8020E614 | Size: 0x2C | Check if fn_801FD104 returns non-zero */
BOOL fn_8020E614(void) {
    extern s32 fn_801FD104(void);
    return fn_801FD104() != 0;
}

/* 0x8020E640 | size: 0x94 | medium */
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

    /* mr. r30, r3 */;
    r31 = r4;
    if (((s32)r0 != (s32)0) && ((u32)r31 != (u32)0x0)) {

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
    }
    return;
}

/* 0x8020E6D4 | size: 0x84 | medium */
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

    /* mr. r28, r3 */;
    if ((s32)r0 == (s32)0) return;
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    while (1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
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

    }

    return;
}

/* 0x8020E758 | size: 0x54 | small */
void fn_8020E758(void) {
    extern void fn_801FD07C();
    extern void fn_801FD08C();
    extern void fn_801FD09C();
    extern void fn_801FD0AC();
    u8 sp[0x10];
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
    return;
}

/* 0x8020E7AC | size: 0x1B0 | medium */
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

    /* mr. r27, r3 */;
    r28 = r5;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r0 = (s16)r28;
    if ((s32)r0 < (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((u32)r27 == (u32)0x0) {
            r29 = 0x0;
            break;
        }
        r31 = r4 & 0xFFFF;
        r30 = 0x0;
        while (1) {
            r0 = r30 & 0xFFFF;
            if ((u32)r0 >= (u32)r31) break;
            r3 = r30 & 0xFFFF;
            r0 = (s16)r28;
            r0 = r3 * 0x14;
            r29 = r27 + r0;
            if ((u32)r27 < (u32)0x0) {
                do {
                    if ((u32)r29 == (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r3 = r29;
                    fn_801FBF04();
                    r0 = (s16)r3;
                    if ((u32)r29 < (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r0 = 0x1;
                } while (0);

                r0 = r0 & 0xFF;
                if ((u32)r29 == (u32)0x0) {
                    break;
                }
                do {
                    if ((u32)r29 == (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r3 = r29;
                    fn_801FBF04();
                    r0 = (s16)r3;
                    if ((u32)r29 < (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r0 = 0x1;
                } while (0);

                r0 = r0 & 0xFF;
                if ((u32)r29 != (u32)0x0) {
                    r3 = r29;
                    fn_801FBF04();
                    r3 = (s16)r3;
                    r0 = (s16)r28;
                    if ((s32)r0 == (s32)r3) {
                        break;
                }
                }
                }
            r30 = r30 + 0x1;

        }
        r29 = 0x0;
    } while (0);

    if ((u32)r29 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    r3 = r29;
    r4 = -0x1;
    fn_801FBE18();
    r30 = 0x0;
    while (1) {
        r0 = r30 & 0xFF;
        if ((u32)r0 >= (u32)0x4) break;
        r3 = r29;
        r4 = r30;
        r5 = 0x0;
        fn_801FBDF4();
        r30 = r30 + 0x1;

    }
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

    return;
}

/* 0x8020E95C | size: 0x24C | large */
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

    /* mr. r29, r3 */;
    r30 = r4;
    r31 = r5;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r0 = (s16)r31;
    if ((s32)r0 < (s32)0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((u32)r29 == (u32)0x0) {
            r26 = 0x0;
            break;
        }
        r28 = r30 & 0xFFFF;
        r27 = 0x0;
        while (1) {
            r0 = r27 & 0xFFFF;
            if ((u32)r0 >= (u32)r28) break;
            r3 = r27 & 0xFFFF;
            r0 = (s16)r31;
            r0 = r3 * 0x14;
            r26 = r29 + r0;
            if ((u32)r29 < (u32)0x0) {
                do {
                    if ((u32)r26 == (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r3 = r26;
                    fn_801FBF04();
                    r0 = (s16)r3;
                    if ((u32)r26 < (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r0 = 0x1;
                } while (0);

                r0 = r0 & 0xFF;
                if ((u32)r26 == (u32)0x0) {
                    break;
                }
                do {
                    if ((u32)r26 == (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r3 = r26;
                    fn_801FBF04();
                    r0 = (s16)r3;
                    if ((u32)r26 < (u32)0x0) {
                        r0 = 0x0;
                        break;
                    }
                    r0 = 0x1;
                } while (0);

                r0 = r0 & 0xFF;
                if ((u32)r26 != (u32)0x0) {
                    r3 = r26;
                    fn_801FBF04();
                    r3 = (s16)r3;
                    r0 = (s16)r31;
                    if ((s32)r0 == (s32)r3) {
                        break;
                }
                }
                }
            r27 = r27 + 0x1;

        }
        r26 = 0x0;
    } while (0);

    if ((u32)r26 != (u32)0x0) {
        r3 = 0x0;
        return;
    }
    do {
        if ((u32)r29 == (u32)0x0) {
            r27 = 0x0;
            break;
        }
        r30 = r30 & 0xFFFF;
        r26 = 0x0;
        while (1) {
            r0 = r26 & 0xFFFF;
            if ((u32)r0 >= (u32)r30) break;
            r0 = r26 & 0xFFFF;
            r0 = r0 * 0x14;
            /* add. r27, r29, r0 */;
            do {
                if ((u32)r29 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r3 = r27;
                fn_801FBF04();
                r0 = (s16)r3;
                if ((u32)r29 < (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r0 = 0x1;
            } while (0);

            r0 = r0 & 0xFF;
            if ((u32)r29 == (u32)0x0) {
                break;
            }
            r26 = r26 + 0x1;

        }
        r27 = 0x0;
    } while (0);

    if ((u32)r27 == (u32)0x0) {
        r3 = 0x0;
        return;
    }
    if ((u32)r27 == (u32)0x0) { r3 = 0x1; return; }
    r0 = (s16)r31;
    if ((u32)r27 < (u32)0x0) { r3 = 0x1; return; }
    r3 = r27;
    r4 = -0x1;
    fn_801FBE18();
    r26 = 0x0;
    while (1) {
        r0 = r26 & 0xFF;
        if ((u32)r0 >= (u32)0x4) break;
        r3 = r27;
        r4 = r26;
        r5 = 0x0;
        fn_801FBDF4();
        r26 = r26 + 0x1;

    }
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

    r3 = 0x1;

    return;
}

/* 0x8020EBA8 | size: 0xFC | medium */
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

    /* mr. r27, r3 */;
    r28 = r5;
    if ((s32)r0 == (s32)0) {
        r3 = 0x0;
        return;
    }
    r31 = r4 & 0xFFFF;
    r29 = 0x0;
    while (1) {
        r0 = r29 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r3 = r29 & 0xFFFF;
        r0 = (s16)r28;
        r0 = r3 * 0x14;
        r30 = r27 + r0;
        if ((s32)r0 < (s32)0) {
            do {
                if ((u32)r30 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r3 = r30;
                fn_801FBF04();
                r0 = (s16)r3;
                if ((u32)r30 < (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r0 = 0x1;
            } while (0);

            r0 = r0 & 0xFF;
            if ((u32)r30 != (u32)0x0 || (u32)r30 != (u32)0x0 || (s32)r0 != (s32)r3) {
                r3 = r30;
                return;
            }
            do {
                if ((u32)r30 == (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r3 = r30;
                fn_801FBF04();
                r0 = (s16)r3;
                if ((u32)r30 < (u32)0x0) {
                    r0 = 0x0;
                    break;
                }
                r0 = 0x1;
            } while (0);

            r0 = r0 & 0xFF;

            r3 = r30;
            fn_801FBF04();
            r3 = (s16)r3;
            r0 = (s16)r28;

            r3 = r30;
            return;
            }
        r29 = r29 + 0x1;

    }
    r3 = 0x0;

    return;
}

/* 0x8020ECA4 | size: 0x3C | small */
u32 fn_8020ECA4(void* obj) {
    extern s16 fn_801FBF04();
    s16 val;
    if (obj == 0) return 0;
    val = fn_801FBF04(obj);
    return (val >= 0) ? 1 : 0;
}

/* 0x8020ECE0 | size: 0xDC | medium */
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

    /* mr. r27, r3 */;
    if ((s32)r0 == (s32)0) return;
    r31 = r4 & 0xFFFF;
    r28 = 0x0;
    while (1) {
        r0 = r28 & 0xFFFF;
        if ((u32)r0 >= (u32)r31) break;
        r0 = r28 & 0xFFFF;
        r4 = -0x1;
        r0 = r0 * 0x14;
        r29 = r27 + r0;
        r3 = r29;
        fn_801FBE18();
        r30 = 0x0;
        while (1) {
            r0 = r30 & 0xFF;
            if ((u32)r0 >= (u32)0x4) break;
            r3 = r29;
            r4 = r30;
            r5 = 0x0;
            fn_801FBDF4();
            r30 = r30 + 0x1;

        }
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

    }

    return;
}

/* 0x8020EDBC | size: 0x60 */
void fn_8020EDBC(void* ctx) {
    extern void fn_801FBD84();
    extern void fn_801FBD94();
    extern void fn_801FBDA4();
    extern void fn_801FBDB4();
    extern void fn_801FBDC4();
    fn_801FBDC4(ctx, 0);
    fn_801FBDB4(ctx, 0);
    fn_801FBDA4(ctx, 0);
    fn_801FBD94(ctx, 0);
    fn_801FBD84(ctx, 0);
}

/* 0x8020EE1C | size: 0xA4 | medium */
void fn_8020EE1C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x8020EEC0 | size: 0x14 | tiny */
extern u8 lbl_8047B600;
u8 fn_8020EEC0(void) { return (u8)(lbl_8047B600 == 1); }

/* 0x8020EED4 | size: 0x22C | large */
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r30;
        fn_802062FC();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
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
            return;
    }
    }
    r3 = r30;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        return;
    }
    r3 = r28;
    fn_802062FC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        return;
    }
    r3 = 0x11;
    fn_801F0234();
    fn_801F0204();
    r7 = r3;
    r3 = r27;
    r4 = r31;
    r5 = 0x1;
    r6 = 0x1;
    fn_802085C4();

    return;
}

/* 0x8020F108 | size: 0x128 */
void fn_8020F108(void* battleCtx, void* ctx) {
    extern void fn_8011BEB4();
    extern void fn_801C3430();
    extern void fn_801C3D64();
    extern void fn_801DB100();
    extern u8 fn_801F453C();
    extern void fn_801FCEC4();
    extern void* fn_80207C6C();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 localBuf[0x6E8];
    u8 partySlot;
    void* eeData;
    void* resolved;

    partySlot = fn_801F453C(0, 1);
    fn_8011BEB4(0, battleCtx, 0x1F, 0);
    eeData = fn_8012640C(ctx, 0, 0xEE, 0);
    resolved = fn_80207C6C(ctx, partySlot);
    fn_801FCEC4(localBuf, ctx);
    fn_801254B4(localBuf, 0, 0xEE, 0, (u32)resolved);
    fn_80208750(ctx, battleCtx, 1, 0);
    fn_80208750(localBuf, battleCtx, 3, 0);
    fn_802085C4(ctx, battleCtx, 1, 1, 0);
    fn_801C3D64(eeData, resolved);
    fn_801C3430();
    fn_801254B4(ctx, 0, 0xEE, 0, (u32)resolved);
    fn_802085C4(ctx, battleCtx, 3, 0, 0);
    fn_801DB100(eeData);
}

/* 0x8020F238 | size: 0x128 */
void fn_8020F238(void* battleCtx, void* ctx) {
    extern void fn_8011BEB4();
    extern void fn_801C3430();
    extern void fn_801C3D64();
    extern void fn_801DB100();
    extern u8 fn_801F453C();
    extern void fn_801FCEC4();
    extern void* fn_80207C6C();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 localBuf[0x6E8];
    u8 partySlot;
    void* eeData;
    void* resolved;

    partySlot = fn_801F453C(0, 1);
    fn_8011BEB4(0, battleCtx, 0x1F, 0);
    eeData = fn_8012640C(ctx, 0, 0xEE, 0);
    resolved = fn_80207C6C(ctx, partySlot);
    fn_801FCEC4(localBuf, ctx);
    fn_801254B4(localBuf, 0, 0xEE, 0, (u32)resolved);
    fn_80208750(ctx, battleCtx, 3, 0);
    fn_80208750(localBuf, battleCtx, 3, 1);
    fn_802085C4(ctx, battleCtx, 3, 1, 0);
    fn_801C3D64(eeData, resolved);
    fn_801C3430();
    fn_801254B4(ctx, 0, 0xEE, 0, (u32)resolved);
    fn_802085C4(ctx, battleCtx, 3, 0, 0);
    fn_801DB100(eeData);
}

/* 0x8020F368 | size: 0x80 | small */
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x11;
        fn_801F0234();
        fn_801F0204();
        r7 = r3;
        r3 = r30;
        r4 = r29;
        r5 = 0x1;
        r6 = 0x0;
        fn_802085C4();
    }
    return;
}

/* 0x8020F3F0 | size: 0xA4 | medium */
void fn_8020F3F0(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x8020F494 | size: 0x84 | medium */
void fn_8020F494(void) {
    extern void fn_801F453C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0x1;
    fn_801F453C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x2) { r3 = 0x1; return; }
    if ((s32)r0 < (s32)0x2) {
        if ((s32)r0 == (s32)0x0) { r3 = 0x0; return; }
        if ((s32)r0 >= (s32)0x0) { r3 = 0x3; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0x4) { r3 = 0x2; return; }
    if ((s32)r0 >= (s32)0x4) { r3 = 0x0; return; }
    r3 = 0x4;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x3;
    return;

    r3 = 0x4;
    return;

    r3 = 0x0;

    return;
}

/* 0x8020F518 | size: 0xA4 | medium */
void fn_8020F518(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x8020F5BC | size: 0x8C | medium */
void fn_8020F5BC(void) {
    extern void fn_80202360();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r4;
    r4 = 0x2f;
    fn_80202360();
    r0 = (s16)r3;
    if ((s32)r0 == (s32)0x2) { r3 = 0x1; return; }
    if ((s32)r0 < (s32)0x2) {
        if ((s32)r0 == (s32)0x0) { r3 = 0x0; return; }
        if ((s32)r0 >= (s32)0x0) { r3 = 0x0; return; }
        if ((s32)r0 >= (s32)-0x1) { r3 = 0x4; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0x4) { r3 = 0x3; return; }
    if ((s32)r0 >= (s32)0x4) { r3 = 0x0; return; }
    r3 = 0x2;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x3;
    return;

    r3 = 0x4;
    return;

    r3 = 0x0;

    return;
}

/* 0x8020F648 | size: 0xA4 | medium */
void fn_8020F648(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x8020F6EC | size: 0x60 | small */
void fn_8020F6EC(void) {
    extern u8 lbl_80379F58[];
    u32 r0 = 0;
    u32 r3 = 0;

    r3 = (u32)lbl_80379F58;
    r3 = (u32)lbl_80379F58;
    r3 = r3 + (0x1 << 16);
    r0 = *(u8*)((u8*)r3 + 0x6002);
    if ((s32)r0 == (s32)0x2) { r3 = 0x1; return; }
    if ((s32)r0 < (s32)0x2) {
        if ((s32)r0 == (s32)0x0) { r3 = 0x3; return; }
        if ((s32)r0 >= (s32)0x0) { r3 = 0x0; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 >= (s32)0x4) { r3 = 0x0; return; }
    r3 = 0x2;
    return;

    r3 = 0x3;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x0;
    return;
}

/* 0x8020F74C | size: 0x64 */
void fn_8020F74C(void* p1, void* p2, u32 unused1, u32 unused2, u32 p3) {
    extern u32 fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u32 result;
    fn_80208750(p2, p1, 1, p3);
    fn_801F0234(0x11);
    result = fn_801F0204();
    fn_802085C4(p2, p1, 1, 0, result);
}

/* 0x8020F7B8 | size: 0x114 */
void fn_8020F7B8(void* p1, void* p2, u32 p3, u32 p4) {
    extern u16 fn_8011BEB4();
    extern void fn_801F00D0();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void* fn_801F02AC();
    extern u16 fn_801F54A4();
    extern u8 fn_802062FC();
    extern void fn_802085C4();
    extern void fn_80208750();
    u16 partyCount;
    void* d9Data;
    u16 field29;
    void* resolved;
    void* tablePtr;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    d9Data = fn_8012640C(p2, 0, 0xD9, 0);
    field29 = fn_8011BEB4(d9Data, 0, 0x29, 0);
    fn_801F00D0(field29, partyCount);
    resolved = fn_801F02AC(0xE, p2, partyCount);
    fn_80208750(p2, p1, 1, p4);
    fn_80208750(resolved, p1, 3, p4);
    tablePtr = fn_801F0204(fn_801F0234(0x11));
    fn_802085C4(p2, p1, 1, 1, tablePtr);
    if ((u8)fn_802062FC(resolved) == 1) {
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(resolved, p1, 3, 0, tablePtr);
    }
}

/* 0x8020F8D4 | size: 0x64 | small */
void fn_8020F8D4(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x10];
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
    return;
}

/* 0x8020F938 | size: 0x64 | small */
void fn_8020F938(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x10];
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
    return;
}

/* fn_8020F99C | Size: 0x40 | Check if byte at fixed address equals 3 */
BOOL fn_8020F99C(void) {
    extern u8 lbl_80379F58[];
    u8 val = *(u8*)((u8*)lbl_80379F58 + 0x16002);
    switch (val) {
        case 1:
        case 2:
            return FALSE;
        case 3:
            return TRUE;
        default:
            return FALSE;
    }
}

/* 0x8020F9DC | size: 0xA4 | medium */
void fn_8020F9DC(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* fn_8020FA80 | Size: 0x40 | Check if byte at fixed address equals 3 */
BOOL fn_8020FA80(void) {
    extern u8 lbl_80379F58[];
    u8 val = *(u8*)((u8*)lbl_80379F58 + 0x16002);
    switch (val) {
        case 1:
        case 2:
            return FALSE;
        case 3:
            return TRUE;
        default:
            return FALSE;
    }
}

/* 0x8020FAC0 | size: 0x70 */
void fn_8020FAC0(void* p1, void* p2, u32 unused1, u16 flag, u32 p4) {
    extern u32 fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u32 result;
    fn_80208750(p2, p1, 1, p4);
    if (flag == 0) {
        fn_801F0234(0x11);
        result = fn_801F0204();
        fn_802085C4(p2, p1, 1, 0, result);
    }
}

/* 0x8020FB38 | size: 0xC4 | medium */
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
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = r7;
    if ((s32)r0 != (s32)0) {
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
    } else {

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
    }
    return;
}

/* 0x8020FC04 | size: 0x6C | small */
void fn_8020FC04(void) {
    extern void fn_801F453C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0x1;
    fn_801F453C();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x1) { r3 = 0x0; return; }
    if ((s32)r0 < (s32)0x1) {
        if ((s32)r0 >= (s32)0x0) { r3 = 0x1; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 >= (s32)0x5) { r3 = 0x0; return; }
    r3 = 0x2;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x0;

    return;
}

/* 0x8020FC70 | size: 0x11C */
void fn_8020FC70(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern u8 fn_8011BEB4();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 battleType;
    void* tablePtr;

    battleType = (u8)fn_8011BEB4(0, p1, 5, 0);
    if ((battleType == 4 || battleType == 6 || battleType == 1) && (u16)mode != 1) {
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    } else {
        fn_80208750(p2, p1, 1, 0);
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    }
}

/* 0x8020FD8C | size: 0xB4 | medium */
void fn_8020FD8C(void) {
    extern void fn_8011BEB4();
    u32 r0 = 0;
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
    if ((s32)r0 == (s32)0x46) { r3 = 0x1; return; }
    if ((s32)r0 < (s32)0x46) {
        if ((s32)r0 == (s32)0x1e) { r3 = 0x0; return; }
        if ((s32)r0 < (s32)0x1e) {
            if ((s32)r0 == (s32)0xa) { r3 = 0x0; return; }
            r3 = 0x0;
            return;
        }
        if ((s32)r0 == (s32)0x32) { r3 = 0x1; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0x6e) { r3 = 0x2; return; }
    if ((s32)r0 < (s32)0x6e) {
        if ((s32)r0 == (s32)0x5a) { r3 = 0x1; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0x96) { r3 = 0x2; return; }
    r3 = 0x0;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x0;

    return;
}

/* 0x8020FE40 | size: 0xA4 | medium */
void fn_8020FE40(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x8020FEE4 | size: 0x78 | small */
void fn_8020FEE4(void) {
    extern void fn_8011BEB4();
    u32 r0 = 0;
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
    if ((u32)r4 > (u32)0x59) {
        r3 = 0x3;
        return;
    }
    if ((u32)r4 >= (u32)0x3e) {
        r3 = 0x2;
        return;
    }
    r0 = 0x16;
    r3 = -0x1;
    r0 = r4 - r0;
    /* subfze r0, r3 */;
    r3 = r0 & 0xFF;

    return;
}

/* 0x8020FF5C | size: 0xA4 | medium */
void fn_8020FF5C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x80210000 | size: 0x74 | small */
void fn_80210000(void) {
    extern void fn_8011BEB4();
    u32 r0 = 0;
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
    if ((s32)r0 == (s32)0x50) { r3 = 0x0; return; }
    if ((s32)r0 < (s32)0x50) {
        if ((s32)r0 == (s32)0x28) { r3 = 0x0; return; }
        r3 = 0x1;
        return;
    }
    if ((s32)r0 == (s32)0x78) { r3 = 0x0; return; }
    r3 = 0x1;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;

    return;
}

/* 0x80210074 | size: 0xA4 | medium */
void fn_80210074(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x80210118 | size: 0x78 | small */
void fn_80210118(void) {
    extern void fn_8011BEB4();
    u32 r0 = 0;
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
    if ((u32)r3 <= (u32)0x18) {
        r3 = 0x0;
        return;
    }
    if ((u32)r3 <= (u32)0x24) {
        r3 = 0x1;
        return;
    }
    r0 = 0x50;
    r0 = r0 - r3;
    /* addze r0, r3 */;
    r3 = r3 - r0;
    r3 = r3 + 0x3;

    return;
}

/* 0x80210190 | size: 0xA4 */
void fn_80210190(void* p1, void* p2, void* p3, u32 unused, u32 p4) {
    extern u32 fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u32 result;
    fn_80208750(p2, p1, 1, p4);
    fn_80208750(p3, p1, 2, p4);
    fn_801F0234(0x11);
    result = fn_801F0204();
    fn_802085C4(p2, p1, 1, 1, result);
    fn_801F0234(0x12);
    result = fn_801F0204();
    fn_802085C4(p3, p1, 2, 0, result);
}

/* 0x80210234 | size: 0x84 */
u32 fn_80210234(u32 unused, void* typeObj) {
    extern s16 fn_80202360();
    s16 val;
    val = fn_80202360(typeObj, 0x2E);
    switch (val) {
        case 1: return 0;
        case 2: return 1;
        case 3: return 2;
        case 4: return 3;
        case 5: return 4;
        default: return 0;
    }
}

/* 0x802102B8 | size: 0xA4 | medium */
void fn_802102B8(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x8021035C | size: 0x8C | medium */
void fn_8021035C(void) {
    extern void fn_80202360();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = r4;
    r4 = 0x2f;
    fn_80202360();
    r0 = (s16)r3;
    if ((s32)r0 == (s32)0x2) { r3 = 0x1; return; }
    if ((s32)r0 < (s32)0x2) {
        if ((s32)r0 == (s32)0x0) { r3 = 0x0; return; }
        if ((s32)r0 >= (s32)0x0) { r3 = 0x0; return; }
        if ((s32)r0 >= (s32)-0x1) { r3 = 0x4; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0x4) { r3 = 0x3; return; }
    if ((s32)r0 >= (s32)0x4) { r3 = 0x0; return; }
    r3 = 0x2;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x3;
    return;

    r3 = 0x4;
    return;

    r3 = 0x0;

    return;
}

/* 0x802103E8 | size: 0x100 */
void fn_802103E8(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern u8 fn_8011BEB4();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    void* tablePtr;

    fn_8011BEB4(0, p1, 5, 0);
    if ((s16)mode != 0) {
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    } else {
        fn_80208750(p2, p1, 1, p5);
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    }
}

/* 0x802104E8 | size: 0x84 | medium */
void fn_802104E8(void) {
    extern void fn_8011BEB4();
    u32 r0 = 0;
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
    if ((s32)r0 == (s32)0x14) { r3 = 0x1; return; }
    if ((s32)r0 < (s32)0x14) {
        if ((s32)r0 == (s32)0xa) { r3 = 0x0; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0x1e) { r3 = 0x2; return; }
    r3 = 0x0;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x0;

    return;
}

/* 0x8021056C | size: 0xA4 | medium */
void fn_8021056C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* fn_80210610 | Size: 0x48 | Get level category from fn_80203E0C result */
u32 fn_80210610(void* unused, void* param) {
    extern u8 fn_80203E0C(void* param);
    u8 val = fn_80203E0C(param);
    if (val < 0x21) {
        return 0;
    }
    if (val >= 0x42) {
        return 2;
    }
    return 1;
}

/* 0x80210658 | size: 0xA4 | medium */
void fn_80210658(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x20];
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
    return;
}

/* 0x802106FC | size: 0xC0 | medium */
void fn_802106FC(void) {
    extern void fn_8011BEB4();
    u32 r0 = 0;
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
    if ((s32)r0 == (s32)0x50) { r3 = 0x3; return; }
    if ((s32)r0 < (s32)0x50) {
        if ((s32)r0 == (s32)0x28) { r3 = 0x1; return; }
        if ((s32)r0 < (s32)0x28) {
            if ((s32)r0 == (s32)0x14) { r3 = 0x0; return; }
            r3 = 0x0;
            return;
        }
        if ((s32)r0 == (s32)0x3c) { r3 = 0x2; return; }
        r3 = 0x0;
        return;
    }
    if ((s32)r0 == (s32)0x78) { r3 = 0x5; return; }
    if ((s32)r0 >= (s32)0x78) { r3 = 0x0; return; }
    if ((s32)r0 == (s32)0x64) { r3 = 0x4; return; }
    r3 = 0x0;
    return;

    r3 = 0x0;
    return;

    r3 = 0x1;
    return;

    r3 = 0x2;
    return;

    r3 = 0x3;
    return;

    r3 = 0x4;
    return;

    r3 = 0x5;
    return;

    r3 = 0x0;

    return;
}

/* 0x802107BC | size: 0xC4 */
void fn_802107BC(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    void* tablePtr;

    if ((s16)mode == 0) {
        fn_80208750(p2, p1, 1, p5);
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 0, tablePtr);
    } else {
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    }
}

/* 0x80210888 | size: 0x108 */
void fn_80210888(void* p1, void* p2, u32 p3, u32 p4) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void* fn_801F02AC();
    extern u16 fn_801F54A4();
    extern u8 fn_802062FC();
    extern void fn_802085C4();
    extern void fn_80208750();
    u16 partyCount;
    void* resolved;
    void* tablePtr;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    resolved = fn_801F02AC(0xE, p2, partyCount);
    fn_80208750(p2, p1, 3, p4);
    fn_80208750(resolved, p1, 3, p4);
    if ((u8)fn_802062FC(resolved) == 1) {
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 3, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(resolved, p1, 3, 0, tablePtr);
    } else {
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 3, 0, tablePtr);
    }
}

/* 0x80210998 | size: 0x168 */
void fn_80210998(void* p1, void* p2, void* p3, u32 p4) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void* fn_801F02AC();
    extern u16 fn_801F54A4();
    extern u8 fn_802062FC();
    extern void fn_802085C4();
    extern void fn_80208750();
    u16 partyCount;
    void* resolved;
    void* tablePtr;

    partyCount = fn_801F54A4(0, 0, 0x14, 0);
    resolved = fn_801F02AC(0x10, p2, partyCount);
    fn_80208750(p2, p1, 1, p4);
    fn_80208750(p3, p1, 2, p4);
    fn_80208750(resolved, p1, 2, p4);
    if ((u8)fn_802062FC(resolved) == 1) {
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(resolved, p1, 2, 0, tablePtr);
    } else {
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    }
}

/* 0x80210B08 | size: 0xE8 */
void fn_80210B08(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    void* tablePtr;

    if ((s16)mode != 0) {
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    } else {
        fn_80208750(p2, p1, 1, p5);
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    }
}

/* 0x80210BF8 | size: 0x104 */
void fn_80210BF8(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    void* tablePtr;

    if ((s16)mode == 0) {
        fn_80208750(p2, p1, 1, p5);
        fn_80208750(p3, p1, 2, p5);
        fn_80208750(p2, p1, 3, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    } else if ((u16)mode == 1) {
        fn_80208750(p2, p1, 3, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 3, 0, tablePtr);
    }
}

/* 0x80210D04 | size: 0x150 */
void fn_80210D04(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern u16 fn_8011BEB4();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    extern void fn_802221EC();
    void* d9Data;
    s32 field2D;
    void* tablePtr;

    d9Data = fn_8012640C(p2, 0, 0xD9, 0);
    field2D = (s32)(u32)fn_8011BEB4(d9Data, 0, 0x2D, 0);
    if ((s16)mode == 0) {
        fn_80208750(p2, p1, 1, p5);
        fn_80208750(p3, p1, 2, p5);
        fn_80208750(p2, p1, 3, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    } else if ((u16)mode == 1) {
        fn_80208750(p2, p1, 3, p5);
        if (field2D > 0) {
            fn_802221EC(0x32, p2, 0, 1);
        } else {
            tablePtr = fn_801F0204(fn_801F0234(0x11));
            fn_802085C4(p2, p1, 3, 0, tablePtr);
        }
    }
}

/* 0x80210E5C | size: 0x64 | small */
void fn_80210E5C(void) {
    extern void fn_801F0204();
    extern void fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 sp[0x10];
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
    return;
}

/* 0x80210EC8 | size: 0x170 | medium */
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
    r27 = r3;
    r3 = (u32)lbl_80379F58;
    r28 = r4;
    r3 = r3 + (0x1 << 16);
    r29 = r5;
    r30 = r6;
    r31 = r7;
    r0 = *(u8*)((u8*)r3 + 0x6002);
    if ((u32)r0 == (u32)0x1) {
        r4 = r27;
        r3 = 0x0;
        r5 = 0x5;
        r6 = 0x0;
        fn_8011BEB4();
        r0 = r3 & 0xFF;
        if ((u32)r0 != (u32)0x4 && (u32)r0 != (u32)0x6) {

            if ((u32)r0 == (u32)0x1) {
            }
            r0 = r30 & 0xFFFF;
            if ((u32)r0 != (u32)0x1) {
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
                return;
            }
            }
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
        return;
    }
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

    return;
}

/* 0x80211040 | size: 0x11C */
void fn_80211040(void* p1, void* p2, void* p3, u16 mode, u32 p5) {
    extern u8 fn_8011BEB4();
    extern void* fn_801F0204();
    extern void* fn_801F0234();
    extern void fn_802085C4();
    extern void fn_80208750();
    u8 battleType;
    void* tablePtr;

    battleType = (u8)fn_8011BEB4(0, p1, 5, 0);
    if ((battleType == 4 || battleType == 6 || battleType == 1) && (u16)mode != 1) {
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    } else {
        fn_80208750(p2, p1, 1, p5);
        fn_80208750(p3, p1, 2, p5);
        tablePtr = fn_801F0204(fn_801F0234(0x11));
        fn_802085C4(p2, p1, 1, 1, tablePtr);
        tablePtr = fn_801F0204(fn_801F0234(0x12));
        fn_802085C4(p3, p1, 2, 0, tablePtr);
    }
}

/* 0x80211164 | size: 0x4 | trivial */
void fn_80211164(void) { return; }

/* 0x80211170 | size: 0x68C | large */
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
    r5 = (u32)sp + 0x8;
    r23 = r3;
    *(u32*)(sp + 0xE80) = r0;
    r22 = *(u32*)lbl_8047B618;
    ctr_fn = (void(*)(void))r4;
    do {
        r0 = *(u8*)((u8*)r6 + 0x0);
        r6 = r6 + 0x1;
        *(u8*)((u8*)r5 + 0x0) = r0;
        r5 = r5 + 0x1;
    } while (--ctr != 0);
    r4 = r18;
    r3 = (u32)sp + 0x79c;
    fn_801FCEC4();
    r4 = r19;
    r3 = (u32)sp + 0xbc;
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
    r3 = (u32)sp + 0x10;
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
    do {
        *(u8*)((u8*)r3 + 0x0) = r4;
        r3 = r3 + 0x1;
    } while (--ctr != 0);
    if ((u32)r14 != (u32)0x0) {
        r12 = r14;
        r3 = r16;
        r4 = r17;
        r5 = r18;
        r6 = r19;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r0 = r15 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
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
        if ((u32)r0 == (u32)0x3f) {
            r0 = r14 & 0xFFFF;
            if ((u32)r0 == (u32)0x71) {
                r30 = 0x1;
        }
        }
        r0 = r29 & 0xFFFF;
        r31 = 0x0;
        if ((u32)r0 == (u32)0x42) {
            r0 = r14 & 0xFFFF;
            if ((u32)r0 == (u32)0x53) {
                r31 = 0x1;
        }
        }
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
        if ((u32)r14 > (u32)r7) {
            r14 = r7 & 0xFFFF;
        }
        r3 = r26;
        fn_80207BF4();
        r0 = r3 & 0xFFFF;
        if ((u32)r0 != (u32)0x4) {
            r3 = r26;
            fn_80207BF4();
            r0 = r3 & 0xFFFF;
            if ((u32)r0 != (u32)0x4b) {
                r3 = 0x0;
                r4 = 0x0;
                r5 = 0x29;
                r6 = 0x0;
                fn_801F54A4();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r3 = r14;
                    fn_8020A080();
                    fn_8020A068();
                    r14 = r3 & 0xFF;
                    fn_800E0C54();
                    r3 = r3 & 0xFFFF;
                    r0 = (s32)r3 / (s32)r14;
                    r0 = r0 * r14;
                    /* subf. r0, r0, r3 */;
                    if ((u32)r0 != (u32)0x1) {
                        r3 = r25;
                        r4 = 0x3e;
                        fn_802026E4();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x1) {
                            r0 = r28 & 0xFFFF;
                            if ((u32)r0 == (u32)0x164) {
                                fn_800E0C54();
                                r4 = r3 & 0xFFFF;
                                r3 = 0x64;
                                r0 = (s32)r4 / (s32)r3;
                                r0 = r0 * r3;
                                r0 = r4 - r0;
            }
                }
                        if ((s32)r0 < (s32)0x5a) {
                        }
                        r3 = r27;
                        r4 = 0x0;
                        r5 = 0x2b;
                        r6 = 0x0;
                        r7 = 0x2;
                        fn_8011BBD8();
                        } else {

                        r3 = r27;
                        r4 = 0x0;
                        r5 = 0x2b;
                        r6 = 0x0;
                        r7 = 0x1;
                        fn_8011BBD8();
                        goto L_80211580;
        }
        }
            }
            r3 = r27;
            r4 = 0x0;
            r5 = 0x2b;
            r6 = 0x0;
            r7 = 0x1;
            fn_8011BBD8();
                        }
        L_80211580: ;
        r3 = *(u32*)lbl_8047B610;
        r0 = r3 + 0x1;
        *(u32*)lbl_8047B610 = r0;
    }
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
    if (((u32)r0 == (u32)0x1) && ((u32)r26 == (u32)0xd)) {

        r25 = r25 << 1;
    }
    r3 = r15;
    r4 = 0x32;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r25 * 0xf;
        r0 = 0xa;
        r25 = (s32)r3 / (s32)r0;
    }
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
    if ((u32)r0 == (u32)0x1) {
        r3 = 0x1;
        r4 = 0x1;
        fn_802271E0();
        r3 = *(u32*)lbl_8047B610;
        r0 = r3 + 0x1;
        *(u32*)lbl_8047B610 = r0;
    }
    if ((u32)r21 != (u32)0x0) {
        r12 = r21;
        r3 = r16;
        r4 = r17;
        r5 = r18;
        r6 = r19;
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r3 = r24;
    fn_802096E8();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r24;
        r4 = 0x0;
        r5 = 0x2d;
        r6 = 0x0;
        fn_8011BEB4();
    } else {

        r3 = 0x0;
    }
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
    r4 = (u32)sp + 0x10;
    fn_8020A2B8();
    r3 = 0x8;
    r4 = (u32)lbl_80478D78;
    r5 = (u32)sp + 0x8;
    *(u32*)lbl_8047B610 = r0;
    *(u32*)lbl_8047B618 = r22;
    ctr_fn = (void(*)(void))r3;
    do {
        r0 = *(u8*)((u8*)r5 + 0x0);
        r5 = r5 + 0x1;
        *(u8*)((u8*)r4 + 0x0) = r0;
        r4 = r4 + 0x1;
    } while (--ctr != 0);
    r3 = r18;
    r4 = (u32)sp + 0x79c;
    fn_801FCEC4();
    r3 = r19;
    r4 = (u32)sp + 0xbc;
    fn_801FCEC4();
    r3 = r14;
    return;
}

/* 0x802117FC | size: 0x14 | tiny */
u32 fn_802117FC(void) {
    extern u32 lbl_8047B618;
    return !(lbl_8047B618 & 0x80);
}

/* fn_80211810 | Size: 0x20 | Set/clear bit 0x80 in flags */
void fn_80211810(u8 enable) {
    extern u32 lbl_8047B618;
    u32 val = lbl_8047B618;
    u32 result = val | 0x80u;
    if (enable == 1) {
        result = val & ~0x80u;
    }
    lbl_8047B618 = result;
}

/* 0x80211830 | size: 0xCC */
void fn_80211830(void) {
    extern u8 lbl_80478D78[];
    extern u32 lbl_8047B62C;
    extern void fn_801F37B0();
    extern void* fn_801F47B4();
    extern void fn_801F6EEC();
    extern s32 fn_802118FC();
    extern s32 fn_80213558();
    extern s32 fn_802136A4();
    u8 localBuf[0x10];
    u16 i;
    void* slotData;

    localBuf[0] = 0;
    fn_801F37B0(0, (u32)fn_80213558, &localBuf[0], 0);
    for (i = 0; (u16)i < 2; i++) {
        slotData = fn_801F47B4(0, i);
        if (slotData != NULL) {
            fn_801F6EEC(slotData, 0x4D);
        }
    }
    fn_801F37B0(0, (u32)fn_802136A4, 0, 0);
    for (i = 0; (u16)i < 8; i++) {
        lbl_80478D78[i] = 0;
    }
    lbl_8047B62C = 0;
    fn_801F37B0(0, (u32)fn_802118FC, 0, 0);
}

/* fn_802118FC | Size: 0x4C | Check state and optionally trigger event 0x11 */
s32 fn_802118FC(void* ctx) {
    extern u8 fn_802062FC(void);
    extern void fn_80202810(void* ctx, u32 eventId);
    if (fn_802062FC() != 0) {
        fn_80202810(ctx, 0x11);
    }
    return 1;
}

/* 0x80211948 | size: 0x8C | medium */
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
    r3 = (u32)sp + 0x8;
    fn_801F11CC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
    } else {

        r4 = r31;
        r3 = (u32)sp + 0x8;
        fn_8020D888();
        r3 = 0x1;
    }
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) {
    } else {

        r3 = (u32)sp + 0x8;
        fn_801F0F04();
    }
    return;
}

/* fn_802119D4 | Size: 0x2C | Clear bit 20 in flags and call fn_80213270 */
void fn_802119D4(void) {
    extern u32 lbl_8047B618;
    extern void fn_80213270(void);
    lbl_8047B618 &= ~0x00100000u;
    fn_80213270();
}

/* 0x80211A00 | size: 0x78 | small */
void fn_80211A00(void) {
    extern u8 lbl_80379F58[];
    extern u8 lbl_80478D78[];
    extern u8 lbl_8047B625[];
    extern void fn_801DA7AC();
    extern void fn_801F37B0();
    extern void fn_80211A78();
    extern void fn_8022FE20();
    u32 r0 = 0;
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

/* 0x80211A78 | size: 0x11C */
u32 fn_80211A78(void* ctx) {
    extern u8 lbl_80375D30[];
    extern u16 lbl_803791FE;
    extern u32 lbl_8047B62C;
    extern void fn_801F0F04();
    extern u8 fn_801F1170();
    extern u8 fn_801F11CC();
    extern u8 fn_802026E4();
    extern u16 fn_80205224();
    extern u8 fn_802062FC();
    extern void fn_8020D888();
    extern u32 fn_8020D920();
    u8 localBuf[0x30];
    void* feData;
    u32 d920val;
    u8 result;

    if ((u8)fn_802062FC(ctx) == 0) { return 1; }
    feData = fn_8012640C(ctx, 0, 0xFE, 0);
    if (!feData) { return 1; }
    if ((u8)fn_801F1170(feData) == 0) { return 1; }
    if (fn_80205224(ctx) != 0x108) { return 1; }
    if ((u8)fn_802026E4(ctx, 8) != 0x108) { return 1; }
    if ((u8)(u32)fn_8012640C(ctx, 0, 0xF9, 0) != 0x108) { return 1; }
    d920val = fn_8020D920(lbl_8047B62C);
    result = fn_801F11CC(localBuf, d920val, ctx, 0xC, 0, lbl_80375D30);
    if ((u8)result == 1) {
        fn_8020D888(localBuf, (u32)&lbl_803791FE);
        result = 1;
    }
    if ((u8)result == 1) {
        fn_801F0F04(localBuf);
    }
    return 1;
}

/* 0x80211B94 | size: 0x284 | large */
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
    void (*ctr_fn)(void) = 0;

    r0 = r5 & 0xFF;
    r23 = r5;
    r24 = *(u32*)lbl_8047B610;
    r26 = *(u32*)lbl_8047B62C;
    *(u32*)lbl_8047B610 = r4;
    r25 = *(u8*)lbl_8047B614;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        *(u8*)lbl_8047B614 = r0;
    }
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
    if ((u32)r31 != (u32)0x0) {
        r0 = *(u8*)lbl_8047B614;
        if ((u32)r0 == (u32)0x1) {
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
            do {
                r3 = *(u32*)lbl_8047B610;
                r0 = *(u8*)((u8*)r3 + 0x0);
                r0 = r0 << 2;
                r12 = *(u32*)(r22 + r0);
                ctr_fn = (void(*)(void))r12;
                ctr_fn();
                r0 = *(u8*)lbl_8047B614;

            } while ((u32)r0 != (u32)0x1 && (u32)r0 != (u32)0x2);

            r0 = 0x1;
            r3 = (u32)fn_8022E34C;
            *(u32*)lbl_8047B62C = r27;
            r4 = (u32)fn_8022E34C;
            r5 = (u32)sp + 0x9;
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
            r5 = (u32)sp + 0x8;
            r3 = 0x0;
            r6 = 0x0;
            fn_801F37B0();
            r0 = 0x2;
            *(u8*)lbl_8047B614 = r0;
        }
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

    } else {
        r0 = *(u8*)lbl_8047B614;
        if ((u32)r0 != (u32)0x1) {
            if ((u32)r0 != (u32)0x2) goto L_80211BD8;
        }
    }
    r0 = r23 & 0xFF;
    if ((u32)r0 != (u32)0x2) {
        r0 = *(u32*)lbl_8047B618;
        r0 = r0 & 0xFFFFFDFF;
        *(u32*)lbl_8047B618 = r0;
        r0 = r0 & 0xFFF7FFFF;
        *(u32*)lbl_8047B618 = r0;
    }
    *(u32*)lbl_8047B62C = r26;
    *(u8*)lbl_8047B614 = r25;
    *(u32*)lbl_8047B610 = r24;
    return;
}

/* 0x80211E18 | size: 0x8AC | massive */
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
    if ((s32)r0 == (s32)0) return;
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
    if ((u32)r0 == (u32)0x1) {
        r3 = r19;
        r4 = 0x2e;
        fn_80202810();
    }
    r3 = r19;
    r4 = 0x15;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r19;
        r4 = 0x15;
        fn_80202810();
    }
    r3 = r19;
    r4 = 0x28;
    fn_802026E4();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x1) {
        r3 = r19;
        r4 = 0x28;
        fn_80202810();
    }
    if ((u32)r28 == (u32)0x1) {
        r3 = (u32)lbl_80375E24;
        r17 = *(u32*)lbl_80375E24;

    } else {
        r4 = r31;
        r3 = 0x0;
        r5 = 0x2;
        r6 = 0x0;
        ((void(*)(void))fn_80142CF4)();
        r0 = r3 & 0xFF;
        if ((u32)r0 == (u32)0x1) {
            r3 = (u32)lbl_80375DF0;
            /* clrlslwi r0, r31, 16, 2 */;
            r3 = (u32)lbl_80375DF0;
            r17 = *(u32*)(r3 + r0);
            goto L_802125DC;
        }
        r0 = r31 & 0xFFFF;

        if ((u32)r0 == (u32)0x50 || (u32)r0 == (u32)0x51) {

            r3 = (u32)lbl_80375E44;
            r17 = *(u32*)lbl_80375E44;
            goto L_802125DC;
        }
        r3 = r31;
        fn_801440A0();
        fn_80143DFC();
        fn_80143A94();
        /* mr. r25, r3 */;
        do {
            if ((u32)r0 == (u32)0x51) {
                r25 = 0x7;
                break;
            }
            r0 = r31 & 0xFFFF;
            if ((u32)r0 == (u32)0x13) {
                r25 = 0x1;
                break;
            }
            fn_801437E0();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x13) {
                r25 = 0x2;
                break;
            }
            r3 = r25;
            fn_80143940();
            r0 = r3 & 0xFF;
            if ((u32)r0 != (u32)0x1) {
                r3 = r25;
                fn_80143918();
                r0 = r3 & 0xFF;
                if ((u32)r0 != (u32)0x1) {
                    r3 = r25;
                    fn_801438F0();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 != (u32)0x1) {
                        r3 = r25;
                        fn_801438C8();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 != (u32)0x1) {
                            r3 = r25;
                            fn_801438A0();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 != (u32)0x1) {
                                r3 = r25;
                                fn_80143878();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                }
                }
                }
                }
                }
                r25 = 0x3;
                break;
                                }
            r3 = r25;
            fn_80143A44();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r25 = 0x4;
                break;
            }
            r3 = r25;
            fn_80143A28();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r3 = r25;
                fn_80143A0C();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r3 = r25;
                    fn_801439F0();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) {
                        r3 = r25;
                        fn_801439D4();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x1) {
                            r3 = r25;
                            fn_801439B8();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 != (u32)0x1) {
                }
                }
                }
                }
                r25 = 0x5;
                break;
                            }
            r3 = r25;
            fn_80143990();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r25 = 0x6;
                break;
            }
            r25 = 0x7;
        } while (0);

        r0 = r25 & 0xFF;
        if ((u32)r0 == (u32)0x7) {
            r3 = (u32)lbl_80375E24;
            r17 = *(u32*)lbl_80375E24;
            goto L_802125DC;
        }
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
        if ((s32)r0 != (s32)0x4) {
            if ((s32)r0 < (s32)0x4) {
                if ((s32)r0 < (s32)0x3) {
                    goto L_802125CC;
                }
                if ((s32)r0 == (s32)0x6 || (s32)r0 >= (s32)0x6) goto L_802125CC;

                goto L_802123BC;
                }
            r3 = r17;
            fn_80143940();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r3 = r17;
                fn_80143918();
                r0 = r3 & 0xFF;
                if ((u32)r0 == (u32)0x1) {
                    r3 = r17;
                    fn_801438F0();
                    r0 = r3 & 0xFF;
                    if ((u32)r0 == (u32)0x1) {
                        r3 = r17;
                        fn_801438C8();
                        r0 = r3 & 0xFF;
                        if ((u32)r0 == (u32)0x1) {
                            r3 = r17;
                            fn_801438A0();
                            r0 = r3 & 0xFF;
                            if ((u32)r0 == (u32)0x1) {
                                r3 = r19;
                                r4 = 0x8;
                                fn_802026E4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r0 = 0x5;
                                    *(u8*)((u8*)r18 + 0x5) = r0;
                                    goto L_802125CC;
                                }
                                r3 = r19;
                                r4 = 0x3;
                                fn_802026E4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r0 = 0x4;
                                    *(u8*)((u8*)r18 + 0x5) = r0;
                                    goto L_802125CC;
                                }
                                r3 = r19;
                                r4 = 0x4;
                                fn_802026E4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r0 = 0x4;
                                    *(u8*)((u8*)r18 + 0x5) = r0;
                                    goto L_802125CC;
                                }
                                r3 = r19;
                                r4 = 0x6;
                                fn_802026E4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r0 = 0x3;
                                    *(u8*)((u8*)r18 + 0x5) = r0;
                                    goto L_802125CC;
                                }
                                r3 = r19;
                                r4 = 0x7;
                                fn_802026E4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r0 = 0x2;
                                    *(u8*)((u8*)r18 + 0x5) = r0;
                                    goto L_802125CC;
                                }
                                r3 = r19;
                                r4 = 0x5;
                                fn_802026E4();
                                r0 = r3 & 0xFF;
                                if ((u32)r0 == (u32)0x1) {
                                    r0 = 0x1;
                                    *(u8*)((u8*)r18 + 0x5) = r0;
                    }
                                goto L_802125CC;
            }
            }
            }
            }
            }
            r3 = r17;
            fn_80143940();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x5;
                *(u8*)((u8*)r18 + 0x5) = r0;
                goto L_802125CC;
            }
            r3 = r17;
            fn_80143918();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x4;
                *(u8*)((u8*)r18 + 0x5) = r0;
                goto L_802125CC;
            }
            r3 = r17;
            fn_801438F0();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x3;
                *(u8*)((u8*)r18 + 0x5) = r0;
                goto L_802125CC;
            }
            r3 = r17;
            fn_801438C8();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x2;
                *(u8*)((u8*)r18 + 0x5) = r0;
                goto L_802125CC;
            }
            r3 = r17;
            fn_801438A0();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x1;
                *(u8*)((u8*)r18 + 0x5) = r0;
                goto L_802125CC;
            }
            r3 = r17;
            fn_80143878();
            r0 = r3 & 0xFF;
            if ((u32)r0 == (u32)0x1) {
                r0 = 0x0;
                *(u8*)((u8*)r18 + 0x5) = r0;
            }
            goto L_802125CC;
        }
        r0 = 0x5;
        *(u8*)((u8*)r18 + 0x5) = r0;
        goto L_802125CC;
        L_802123BC: ;
        r0 = 0x4;
        r3 = r17;
        *(u8*)((u8*)r18 + 0x5) = r0;
        fn_80143A28();
        r19 = r3 & 0xFF;
        if ((u32)r0 > (u32)0x1) {
            r3 = (u32)lbl_80279E7C;
            r3 = (u32)lbl_80279E7C;
            r3 = *(u32*)((u8*)r3 + 0x4);
            fn_800FA280();
            r4 = r3;
            r3 = 0xd;
            fn_80132A38();
            r0 = 0x1;
            *(u8*)((u8*)r30 + 0x601E) = r0;

        } else {
            r3 = r17;
            fn_80143A0C();
            r19 = r3 & 0xFF;
            if ((u32)r0 > (u32)0x1) {
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
            }
            r3 = r17;
            fn_801439F0();
            r19 = r3 & 0xFF;
            if ((u32)r0 > (u32)0x1) {
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
            }
            r3 = r17;
            fn_801439D4();
            r19 = r3 & 0xFF;
            if ((u32)r0 > (u32)0x1) {
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
            }
            r3 = r17;
            fn_801439B8();
            r19 = r3 & 0xFF;
            if ((u32)r0 > (u32)0x1) {
                r3 = (u32)lbl_80279E7C;
                r3 = (u32)lbl_80279E7C;
                r3 = *(u32*)((u8*)r3 + 0x10);
                fn_800FA280();
                r4 = r3;
                r3 = 0xd;
                fn_80132A38();
                r0 = 0x4;
                *(u8*)((u8*)r30 + 0x601E) = r0;
            }
        }
        L_802124D8: ;
        r0 = (s16)r19;
        if ((u32)r0 < (u32)0x1) {
            r0 = (s16)r19;

            if ((s32)r0 == (s32)0x1 || (s32)r0 == (s32)-0x1) {

                r3 = 0x76bd;
                fn_800FA280();
                r4 = r3;
                r3 = 0xe;
                fn_80132A38();
                r0 = *(u8*)((u8*)r30 + 0x601E);
                r3 = r0 & 0xF;
                r0 = r3 + 0x15;
                *(u8*)((u8*)r30 + 0x601E) = r0;

            } else {
                r3 = 0x7628;
                fn_800FA280();
                r4 = r3;
                r3 = 0xe;
                fn_80132A38();
                r0 = *(u8*)((u8*)r30 + 0x601E);
                r3 = r0 & 0xF;
                r0 = r3 + 0x2d;
                *(u8*)((u8*)r30 + 0x60A4) = r0;
            }
            r3 = 0x7629;
            fn_800FA280();
            r4 = r3;
            r3 = 0x41;
            fn_80132A38();

        } else {
            r0 = (s16)r19;

            if ((s32)r0 == (s32)0x1 || (s32)r0 == (s32)-0x1) {

                r3 = 0x76bd;
                fn_800FA280();
                r4 = r3;
                r3 = 0xe;
                fn_80132A38();
                r0 = *(u8*)((u8*)r30 + 0x601E);
                r3 = r0 & 0xF;
                r0 = r3 + 0xe;
                *(u8*)((u8*)r30 + 0x60A4) = r0;

            } else {
                r3 = 0x7626;
                fn_800FA280();
                r4 = r3;
                r3 = 0xe;
                fn_80132A38();
                r0 = *(u8*)((u8*)r30 + 0x601E);
                r3 = r0 & 0xF;
                r0 = r3 + 0x26;
                *(u8*)((u8*)r30 + 0x60A4) = r0;
            }
            r3 = 0x7627;
            fn_800FA280();
            r4 = r3;
            r3 = 0x41;
            fn_80132A38();
        }
        L_802125CC: ;
        r3 = (u32)lbl_80375E24;
        /* clrlslwi r0, r25, 24, 2 */;
        r3 = (u32)lbl_80375E24;
        r17 = *(u32*)(r3 + r0);
    }
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
    if ((u32)r28 == (u32)0x0) {
        r3 = r22;
        r4 = r20;
        r5 = 0x1;
        fn_80265598();
    }
    r3 = r21;
    r4 = r17;
    r5 = 0x1;
    fn_80211B94();
    if ((u32)r28 == (u32)0x0) {
        r3 = r23;
        r4 = r31;
        r6 = (s16)r24;
        r5 = 0x1;
        fn_801299C8();
    }
    *(u8*)((u8*)r30 + 0x601E) = r26;
    *(u8*)((u8*)r30 + 0x60A4) = r27;
    fn_801DA7AC();
    r3 = 0x0;
    fn_80261B68();
    r3 = 0x0;
    fn_80261E7C();
    fn_8026246C();

    return;
}
