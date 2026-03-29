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

/* ===== Phase 2 recovery stubs ===== */

/* fn_800129A8 - 0x800129A8 | size: 0x1ec */
extern void fn_801040A0(void);
extern void fn_80103FFC(void);
extern void fn_80103FE4(void);
extern void fn_80104704(void);
extern void fn_80103F74(void);
extern void fn_801669BC(void);
extern void* memcpy(void* dst, const void* src, u32 n);
#if 1
asm void fn_800129A8(void) {
#include "src/game/gs_event_exec_fn_800129A8.inc"
}
#else
void fn_800129A8(void) { /* TODO */ }
#endif

/* fn_80012B94 - 0x80012B94 | size: 0x18c */
extern void fn_801040D0(void);
extern void fn_8005D9E4(void);
extern void fn_800FA444(void);
extern void fn_8001E644(void);
extern void fn_8001EA98(void);
extern void fn_800FB680(void);
extern void fn_801040F0(void);
#if 1
asm void fn_80012B94(void) {
#include "src/game/gs_event_exec_fn_80012B94.inc"
}
#else
void fn_80012B94(void) { /* TODO */ }
#endif

/* fn_80012D20 - 0x80012D20 | size: 0xf8 */
extern void fn_801080CC(void);
#if 1
asm void fn_80012D20(void) {
#include "src/game/gs_event_exec_fn_80012D20.inc"
}
#else
void fn_80012D20(void) { /* TODO */ }
#endif

/* fn_80012E18 - 0x80012E18 | size: 0x198 */
extern void fn_80105624(void);
#if 1
asm void fn_80012E18(void) {
#include "src/game/gs_event_exec_fn_80012E18.inc"
}
#else
void fn_80012E18(void) { /* TODO */ }
#endif

/* fn_80012FB0 - 0x80012FB0 | size: 0x2ec */
extern void fn_801040B8(void);
#if 1
asm void fn_80012FB0(void) {
#include "src/game/gs_event_exec_fn_80012FB0.inc"
}
#else
void fn_80012FB0(void) { /* TODO */ }
#endif

/* fn_8001329C - 0x8001329C | size: 0x3cc */
extern void fn_8001EC08(void);
extern void fn_80132A38(void);
extern void jumptable_802E4D90();
#if 1
asm void fn_8001329C(void) {
#include "src/game/gs_event_exec_fn_8001329C.inc"
}
#else
void fn_8001329C(void) { /* TODO */ }
#endif

/* fn_80013668 - 0x80013668 | size: 0xdc */
extern void fn_80107ED8(void);
#if 1
asm void fn_80013668(void) {
#include "src/game/gs_event_exec_fn_80013668.inc"
}
#else
void fn_80013668(void) { /* TODO */ }
#endif

/* fn_8001374C - 0x8001374C | size: 0x168 */
extern void fn_80129BC8(void);
extern void fn_801297D8(void);
extern void fn_801429E8(void);
extern void fn_80143C68(void);
extern void fn_80129A78(void);
extern void fn_8012959C(void);
extern u8 lbl_80266918[];
extern u32 lbl_8047A2F8;
#if 1
asm void fn_8001374C(void) {
#include "src/game/gs_event_exec_fn_8001374C.inc"
}
#else
void fn_8001374C(void) { /* TODO */ }
#endif

/* fn_800138B4 - 0x800138B4 | size: 0x164 */
extern void fn_80129718(void);
extern void fn_80129650(void);
extern u32 lbl_8047A2F8;
#if 1
asm void fn_800138B4(void) {
#include "src/game/gs_event_exec_fn_800138B4.inc"
}
#else
void fn_800138B4(void) { /* TODO */ }
#endif

/* fn_80013A18 - 0x80013A18 | size: 0x3e4 */
extern void fn_80143C50(void);
extern void fn_801046B8(void);
extern void fn_801026A4(void);
extern void fn_80102510(void);
extern void fn_80102428(void);
extern u8 lbl_80266BD8[];
extern u32 lbl_8047A2F8;
extern u32 lbl_8047A2DC;
extern u32 lbl_8047A2FC;
#if 1
asm void fn_80013A18(void) {
#include "src/game/gs_event_exec_fn_80013A18.inc"
}
#else
void fn_80013A18(void) { /* TODO */ }
#endif

/* fn_80013DFC - 0x80013DFC | size: 0x184 */
extern void fn_801440A0(void);
extern void fn_80143E88(void);
extern void fn_80144014(void);
extern void fn_80102568(void);
extern void fn_8001BCEC(void);
extern void fn_8001B184(void);
extern u32 lbl_8047A2F8;
extern u32 lbl_8047A2EC;
#if 1
asm void fn_80013DFC(void) {
#include "src/game/gs_event_exec_fn_80013DFC.inc"
}
#else
void fn_80013DFC(void) { /* TODO */ }
#endif

/* fn_80013F80 - 0x80013F80 | size: 0x17c */
extern void fn_80143DE4(void);
extern void fn_80143DCC(void);
extern u32 lbl_8047A2F8;
extern u32 lbl_8047A2E0;
#if 1
asm void fn_80013F80(void) {
#include "src/game/gs_event_exec_fn_80013F80.inc"
}
#else
void fn_80013F80(void) { /* TODO */ }
#endif

/* fn_800140FC - 0x800140FC | size: 0x14 */
extern u32 lbl_8047A2F8;
extern u32 lbl_8047A2F4;
#if 1
asm void fn_800140FC(void) {
#include "src/game/gs_event_exec_fn_800140FC.inc"
}
#else
void fn_800140FC(void) { /* TODO */ }
#endif

/* fn_80014118 - 0x80014118 | size: 0x80 */
extern void fn_801FCE60(void);
extern void fn_80205BE8(void);
extern void fn_8012A5B0(void);
extern u32 lbl_8047A2E0;
extern u32 lbl_8047A2F4;
extern u32 lbl_8047A2F8;
#if 1
asm void fn_80014118(void) {
#include "src/game/gs_event_exec_fn_80014118.inc"
}
#else
void fn_80014118(void) { /* TODO */ }
#endif

/* fn_80014198 - 0x80014198 | size: 0x24 */
extern u32 lbl_8047A2EC;
#if 1
asm void fn_80014198(void) {
#include "src/game/gs_event_exec_fn_80014198.inc"
}
#else
void fn_80014198(void) { /* TODO */ }
#endif

/* fn_800141BC - 0x800141BC | size: 0x78 */
extern void fn_8001BD3C(void);
extern u32 lbl_8047A2E0;
extern u32 lbl_8047A2F4;
#if 1
asm void fn_800141BC(void) {
#include "src/game/gs_event_exec_fn_800141BC.inc"
}
#else
void fn_800141BC(void) { /* TODO */ }
#endif

/* fn_80014234 - 0x80014234 | size: 0xe8 */
extern u8 lbl_80266B58[];
#if 1
asm void fn_80014234(void) {
#include "src/game/gs_event_exec_fn_80014234.inc"
}
#else
void fn_80014234(void) { /* TODO */ }
#endif

/* fn_8001431C - 0x8001431C | size: 0x7c */
extern u32 lbl_8047A2FC;
#if 1
asm void fn_8001431C(void) {
#include "src/game/gs_event_exec_fn_8001431C.inc"
}
#else
void fn_8001431C(void) { /* TODO */ }
#endif

/* fn_80014398 - 0x80014398 | size: 0x1b8 */
extern void fn_800FB8C8(void);
extern u32 lbl_8047A2FC;
#if 1
asm void fn_80014398(void) {
#include "src/game/gs_event_exec_fn_80014398.inc"
}
#else
void fn_80014398(void) { /* TODO */ }
#endif

/* fn_80014550 - 0x80014550 | size: 0x24 */
#if 1
asm void fn_80014550(void) {
#include "src/game/gs_event_exec_fn_80014550.inc"
}
#else
void fn_80014550(void) { /* TODO */ }
#endif

/* fn_80014574 - 0x80014574 | size: 0x4d4 */
extern void fn_80166A50(void);
extern u32 lbl_8047A2FC;
#if 1
asm void fn_80014574(void) {
#include "src/game/gs_event_exec_fn_80014574.inc"
}
#else
void fn_80014574(void) { /* TODO */ }
#endif

/* fn_80014A48 - 0x80014A48 | size: 0x9c */
#if 1
asm void fn_80014A48(void) {
#include "src/game/gs_event_exec_fn_80014A48.inc"
}
#else
void fn_80014A48(void) { /* TODO */ }
#endif

/* fn_80014AE4 - 0x80014AE4 | size: 0xbc */
extern u8 lbl_80266BC4[];
extern u32 lbl_8047B744;
extern u32 lbl_8047A2C0;
extern u32 lbl_8047B740;
#if 1
asm void fn_80014AE4(void) {
#include "src/game/gs_event_exec_fn_80014AE4.inc"
}
#else
void fn_80014AE4(void) { /* TODO */ }
#endif

/* fn_80014BA0 - 0x80014BA0 | size: 0x98 */
extern u8 lbl_80266BB0[];
#if 1
asm void fn_80014BA0(void) {
#include "src/game/gs_event_exec_fn_80014BA0.inc"
}
#else
void fn_80014BA0(void) { /* TODO */ }
#endif

/* fn_80014C38 - 0x80014C38 | size: 0xe4 */
extern u8 lbl_80266B9C[];
#if 1
asm void fn_80014C38(void) {
#include "src/game/gs_event_exec_fn_80014C38.inc"
}
#else
void fn_80014C38(void) { /* TODO */ }
#endif

/* fn_80014D1C - 0x80014D1C | size: 0x134 */
extern u8 lbl_802E4DB0[];
#if 1
asm void fn_80014D1C(void) {
#include "src/game/gs_event_exec_fn_80014D1C.inc"
}
#else
void fn_80014D1C(void) { /* TODO */ }
#endif

/* fn_80014E50 - 0x80014E50 | size: 0xf8 */
#if 1
asm void fn_80014E50(void) {
#include "src/game/gs_event_exec_fn_80014E50.inc"
}
#else
void fn_80014E50(void) { /* TODO */ }
#endif

/* fn_80014F48 - 0x80014F48 | size: 0xd4 */
extern u32 lbl_8047B748;
extern u32 lbl_8047A2C0;
extern u32 lbl_8047B74C;
extern u32 lbl_8047B744;
#if 1
asm void fn_80014F48(void) {
#include "src/game/gs_event_exec_fn_80014F48.inc"
}
#else
void fn_80014F48(void) { /* TODO */ }
#endif

