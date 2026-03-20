/**
 * @file gs_event_exec.c
 * @brief GSeventExec -- Scene scripting and event execution system.
 *
 * Address range: 0x80012858 - 0x80015000 (~35 functions)
 *
 * This module implements the event execution engine that drives scripted
 * sequences in the game. Events are triggered by NPC interactions, story
 * progression flags, item pickups, and location-based triggers.
 *
 * The event system uses a bytecode-like command format where each event
 * is a sequence of operations (set flag, display text, move NPC, play
 * animation, trigger battle, warp player, etc.).
 *
 * Key functions:
 *   fn_80012858  GSevent_InitExecutor         -- 0x150 bytes, initialize event state
 *   fn_800129A8  GSevent_RunStep              -- 0x1EC bytes, execute one event step
 *   fn_80012B94  GSevent_ProcessCommand       -- 0x18C bytes, decode & dispatch command
 *   fn_80012D20  GSevent_EvalCondition        -- 0xF8 bytes, evaluate flag conditions
 *   fn_80012E18  GSevent_SetFlag              -- 0x198 bytes, set game flag from event
 *   fn_80012FB0  GSevent_DisplayText          -- 0x2EC bytes, show event text box
 *   fn_8001329C  GSevent_MoveNPC              -- 0x3CC bytes, script NPC movement
 *   fn_80013668  GSevent_PlayAnimation        -- 0xDC bytes, trigger NPC animation
 *   fn_80013744  GSevent_Nop                  -- 8 bytes, no-op command
 *   fn_8001374C  GSevent_WaitFrames           -- 0x168 bytes, wait N frames
 *   fn_800138B4  GSevent_FadeScreen           -- 0x164 bytes, screen fade in/out
 *   fn_80013A18  GSevent_CameraControl        -- 0x3E4 bytes, camera movement script
 *   fn_80013DFC  GSevent_PlaySound            -- 0x184 bytes, play sound effect
 *   fn_80013F80  GSevent_PlayMusic            -- 0x17C bytes, change BGM
 *   fn_800140FC  GSevent_StopMusic            -- 0x14 bytes, stop current BGM
 *   fn_80014110  GSevent_ReturnTrue           -- 8 bytes, stub returns 1
 *   fn_80014118  GSevent_CheckPartyState      -- 0x80 bytes, check party condition
 *   fn_80014198  GSevent_GetPartySize         -- 0x24 bytes, return party count
 *   fn_800141BC  GSevent_GiveItem             -- 0x78 bytes, give item to player
 *   fn_80014234  GSevent_GivePokemon          -- 0xE8 bytes, give Pokemon to player
 *   fn_8001431C  GSevent_CheckBag             -- 0x7C bytes, check bag for item
 *   fn_80014398  GSevent_BranchOnFlag         -- 0x1B8 bytes, conditional branch
 *   fn_80014550  GSevent_UnlockLocation       -- 0x24 bytes, unlock map location
 *   fn_80014574  GSevent_CutsceneSequence     -- 0x4D4 bytes, full cutscene playback
 *   fn_80014A48  GSevent_SetWeather           -- 0x9C bytes, change weather effect
 *   fn_80014AE4  GSevent_SetTimeOfDay         -- 0xBC bytes, set time/lighting
 *   fn_80014BA0  GSevent_TriggerBattle        -- 0x98 bytes, start battle from event
 *   fn_80014C38  GSevent_ShadowEncounter      -- 0xE4 bytes, Shadow Pokemon event
 *   fn_80014D1C  GSevent_PurificationEvent    -- 0x134 bytes, purification ceremony
 *   fn_80014E50  GSevent_ColosseumMatch       -- 0xF8 bytes, colosseum match setup
 *   fn_80014F48  GSevent_MtBattleFloor        -- 0xD4 bytes, Mt. Battle progression
 *
 * Event command format (reconstructed):
 *   Each command is a variable-length structure:
 *     byte 0:    Command opcode (0x00-0x3F)
 *     byte 1:    Flags / subcommand
 *     bytes 2-3: Parameter count
 *     bytes 4+:  Command-specific parameters
 *
 * The event executor (fn_800129A8) is a loop that:
 *   1. Reads the next command from the event bytecode stream
 *   2. Dispatches to the appropriate handler via fn_80012B94
 *   3. If the handler returns "wait", yields until next frame
 *   4. If the handler returns "done", advances to next command
 *   5. If the handler returns "branch", jumps to the target offset
 *
 * Rodata references:
 *   lbl_80266788: Shift-JIS format string for NPC dialog ("%s: [dialog text]")
 *   lbl_802667B0-802668AC: Various event-related string templates
 *
 * SDA globals:
 *   Variables in 0x8047A2A0-0x8047A2D0 range hold event execution state:
 *   - Current event ID
 *   - Program counter within event
 *   - Stack depth for nested events
 *   - Wait counter for timed events
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* Flag system */
extern void fn_80190048(s32 flagId, s32 value);    /* GSflagSet */
extern s32  fn_80190118(s32 flagId);                /* GSflagGet */

/* Text system */
extern void fn_80106D3C(s32 slot, s32 msgId, s32 p3, s32 p4);
extern void fn_801069FC(s32 slot);

/* Sound system */
extern void fn_80166AB8(s32 soundId, s32 p2, s32 p3);   /* Play SE */
extern void fn_801669E4(s32 soundId, s32 p2, s32 p3);   /* Play BGM */

/* Scene/camera */
extern void fn_80113828(s32 cameraId, s32 mode);
extern u8   fn_800FF548(void);                     /* Scene transition check */
extern void fn_800FF56C(s32 floorId);              /* Floor load */

/* Frame control */
extern void fn_800F0308(void);                     /* Frame advance */
extern void fn_800F05A0(void* threadCtx);          /* Resume thread */
extern void fn_800F07A8(s32 priority, void* stack, s32 stackSize,
                         s32 flags, s32 p5, void* entry);

/* =========================================================================
 * Data tables
 * ========================================================================= */

/* lbl_80266918: Event handler table
 * Array of structures, each 0x4C bytes:
 *   +0x00: Event type ID (s32)
 *   +0x04: Handler function pointer (or -1 for default)
 *   +0x08: Parameter block pointer
 *   +0x0C-0x48: Type-specific data
 *
 * This table is referenced heavily by fn_80015050 and fn_800150E4
 * which index into it using: entry = table + (slotIndex * 0x4C)
 */

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */

extern u32 lbl_8047A2EC;

/* Address: 0x80013744 | Size: 0x8 | Pattern: return_constant */
u32 fn_80013744(void) { return 1; }

/* Address: 0x80014110 | Size: 0x8 | Pattern: sda_getter */
u32 fn_80014110(void) {
    return lbl_8047A2EC;
}
