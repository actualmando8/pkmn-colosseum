/**
 * @file gs_npc_event.c
 * @brief GSnpcEvent -- NPC event callbacks and overworld sprite rendering.
 *
 * Address range: 0x80030170 - 0x80033278 (~30 functions)
 *
 * This module provides the NPC event callback functions that are invoked
 * when specific NPC interactions trigger game events. It also handles
 * NPC overworld model/sprite rendering during event sequences.
 *
 * The functions in this module are registered as callbacks in the event
 * system and are called when the player interacts with specific NPCs
 * or triggers location-based events.
 *
 * Key functions:
 *   fn_80030170  GSnpcEvt_PopEventQueue      -- 0x38 bytes, dequeue next event
 *   fn_800301A8  GSnpcEvt_NopA               -- 4 bytes, blr stub
 *   fn_800301AC  GSnpcEvt_NopB               -- 4 bytes, blr stub
 *   fn_800301B0  GSnpcEvt_ProcessTrainerSprite -- 0x120 bytes, trainer sprite display
 *   fn_800302D0  GSnpcEvt_ProcessOverworldNPC -- 0xA0 bytes, overworld NPC event
 *   fn_80030370  GSnpcEvt_SetModelA          -- 0xC bytes, set model ID 0x43E3
 *   fn_8003037C  GSnpcEvt_SetModelB          -- 0xC bytes, set model ID 0x43E2
 *   fn_80030388  GSnpcEvt_ProcessMirorB      -- 0xA0 bytes, Miror B. encounter event
 *   fn_80030428  GSnpcEvt_ReturnZero         -- 4 bytes, li r3,0 + blr
 *   fn_8003042C  GSnpcEvt_ShadowEncounterCB  -- 0x148 bytes, Shadow Pokemon event CB
 *   fn_80030574  GSnpcEvt_ColosseumePreBattle -- 0x234 bytes, pre-battle setup
 *   fn_800307A8  GSnpcEvt_StoryBattleCB      -- 0x12C bytes, story battle callback
 *   fn_800308D4  GSnpcEvt_QuestCompleteCB    -- 0x170 bytes, quest completion handler
 *   fn_80030A44  GSnpcEvt_ItemPickupCB       -- 0x1D0 bytes, overworld item pickup
 *   fn_80030C14  GSnpcEvt_HiddenItemCB       -- 0x120 bytes, hidden item event
 *   fn_80030D34  GSnpcEvt_TreasureChestCB    -- 0x1D8 bytes, treasure chest interaction
 *   fn_80030F0C  GSnpcEvt_FinalChunk         -- remaining functions to 0x80033278
 *
 * fn_800301B0 (GSnpcEvt_ProcessTrainerSprite):
 *   Handles the display of a trainer sprite during battle transitions.
 *   Checks the NPC event type at offset +0x06 (lha) and dispatches:
 *     0x10CE: Load trainer model from lbl_803A3230
 *     0x10CF: Load trainer model from lbl_803A31E8
 *   Then sets up the renderer:
 *     - fn_800D88DC: Set render layer 3
 *     - fn_800D888C: Set render layer 4
 *     - fn_800D6A00: Set render mode 7
 *     - fn_800D7820: Load model resource
 *     - fn_800D85D4: Bind model to slot 0
 *     - fn_800D67BC: Set blend mode 2
 *     - fn_800D61E4: Set position (x, y)
 *     - fn_800D5CB8: Set color RGBA (255, 255, 255, 255)
 *     - fn_800D59B8: Set scale (1.0, 1.0)
 *     - fn_800D6728: Commit render state
 *
 * fn_800302D0 (GSnpcEvt_ProcessOverworldNPC):
 *   Handles overworld NPC events based on the NPC's event type ID:
 *     0x0FBC: Generic NPC dialog event
 *     0x0FBD: Special NPC battle trigger
 *   For battle triggers, looks up the trainer data from lbl_803A2688,
 *   sets the battle parameter via fn_80132A38 (param 0x4D), then
 *   initiates a trainer model render via fn_800FB680.
 *
 * fn_80030388 (GSnpcEvt_ProcessMirorB):
 *   Same structure as fn_800302D0 but for Miror B. encounters:
 *     0x10CA: Miror B. pre-battle
 *     0x10CB: Miror B. post-battle
 *   Uses model ID 0x4412 for the standard render and 0x4413 for
 *   the alt render.
 *
 * SDA globals:
 *   lbl_8047B9D4: Model scale factor (float, sdata2)
 *   lbl_8047B9F0: Model Y offset (float, sdata2)
 *   lbl_8047B834: Float constant for render setup (sdata2)
 *   lbl_8047B850: Float constant for render setup (sdata2)
 *
 * BSS globals:
 *   lbl_803A3230: Trainer model data A (loaded from FSYS)
 *   lbl_803A31E8: Trainer model data B (loaded from FSYS)
 *   lbl_803A2688: Party/trainer working data
 *   lbl_80314F98: Default model resource pointer
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* Model/Rendering system */
extern void  fn_800D88DC(s32 layer);              /* Set render layer */
extern void  fn_800D888C(s32 layer);              /* Set secondary layer */
extern void  fn_800D6A00(s32 mode);               /* Set render mode */
extern void  fn_800D7820(void* resource);          /* Load model resource */
extern void  fn_800D85D4(s32 slot, void* model);  /* Bind model to slot */
extern void  fn_800D67BC(s32 blendMode);           /* Set blend mode */
extern void  fn_800D61E4(s32 x, s32 y);           /* Set position */
extern void  fn_800D5CB8(s32 slot, s32 r, s32 g, s32 b, s32 a); /* Set color */
extern void  fn_800D59B8(s32 slot, f32 scaleX, f32 scaleY);     /* Set scale */
extern void  fn_800D6728(void);                    /* Commit render state */

/* NPC/interaction */
extern void* fn_80109934(void* data);              /* Get model from data */
extern s32   fn_8012A5B0(void* partyData, s32 slot, s32 p3);
extern void  fn_80132A38(s32 paramId, s32 value);
extern void  fn_800FB680(s32 x, s32 y, s32 flags, u32 color, u16 modelId);

/* Event queue */
/* The event queue is a simple array-based queue where fn_80030170 pops
 * the next pending event. Events are enqueued by the scene system when
 * NPC interactions or location triggers fire. */

/* =========================================================================
 * Function: GSnpcEvt_PopEventQueue
 * Address:  0x80030170
 * Size:     0x38
 *
 * Dequeues the next event from the event queue. Returns NULL if the
 * queue is empty (field at +0x34 >= 0xD indicates end of queue).
 * The queue is stored as an array of pointers at the start of the
 * context structure, with a count/index at offset +0x34.
 * ========================================================================= */

/* =========================================================================
 * Function: GSnpcEvt_ProcessTrainerSprite
 * Address:  0x800301B0
 * Size:     0x120
 *
 * Renders a trainer sprite/model during battle transition sequences.
 * Dispatches on the event type ID from the NPC data structure.
 * ========================================================================= */

/* =========================================================================
 * Function: GSnpcEvt_ProcessOverworldNPC
 * Address:  0x800302D0
 * Size:     0xA0
 *
 * Processes an overworld NPC event. Checks the NPC's event flags and
 * model type to determine the appropriate rendering and behavior.
 * ========================================================================= */
