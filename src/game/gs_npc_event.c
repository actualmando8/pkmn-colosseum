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

/* =========================================================================
 * Stubs for remaining GSnpcEvt functions (0x80031188-0x800330B8)
 * ========================================================================= */

/* 0x80031188 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80031188(void) {
    extern u8 lbl_803A2688[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    f32 f6 = 0.0f;

    r5 = *(u32*)((u8*)r4 + 0x64);
    r0 = *(s16*)((u8*)r4 + 0x6);
    r4 = r5 & (0xa140 << 16);
    r3 = *(u8*)((u8*)r3 + 0x8B);
    r31 = r4 | r3;
    if ((s32)r0 == (s32)0xf6b) goto L_800311C4;
    if ((s32)r0 >= (s32)0xf6b) goto L_80031214;
    if ((s32)r0 >= (s32)0xf6a) goto L_800311DC;
    goto L_80031214;
L_800311C4: ;
    r5 = r31;
    r3 = 0x0;
    r4 = 0x0;
    r6 = 0x4412;
    ((void(*)(void))fn_800FB680)();
    goto L_80031214;
L_800311DC: ;
    r3 = (u32)lbl_803A2688;
    r4 = 0x1;
    r3 = (u32)lbl_803A2688;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
    r0 = r3;
    r3 = 0x4d;
    r4 = r0;
    ((void(*)(void))fn_80132A38)();
    r5 = r31;
    r3 = 0x0;
    r4 = 0x0;
    r6 = 0x4413;
    ((void(*)(void))fn_800FB680)();
L_80031214: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80031228 | 0x1DC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80031228(void) {
    extern u8 lbl_80266E90[];
    extern u8 lbl_803A2688[];
    extern void fn_80075FEC();
    extern void fn_80109220();
    extern void fn_8010B718();
    extern void fn_8011E850();
    extern void fn_80123FBC();
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

    r30 = r3;
    r31 = r4;
    r3 = (u32)lbl_80266E90;
    r29 = 0x0;
    r4 = (u32)lbl_80266E90;
    r6 = 0x0;
    r7 = 0x0;
    r5 = 0x0;
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
L_80031268: ;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0x2);
    if ((s32)r3 != (s32)r0) goto L_80031280;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_80031280: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0x2);
    if ((s32)r3 != (s32)r0) goto L_8003129C;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_8003129C: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0x2);
    if ((s32)r3 != (s32)r0) goto L_800312B8;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800312B8: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0x2);
    if ((s32)r3 != (s32)r0) goto L_800312D4;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800312D4: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0x2);
    if ((s32)r3 != (s32)r0) goto L_800312F0;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800312F0: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0x2);
    if ((s32)r3 != (s32)r0) goto L_8003130C;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_8003130C: ;
    r4 = r4 + 0x12;
    r5 = r5 + 0x5;
    if (--ctr != 0) goto L_80031268;
    if ((s32)r6 == (s32)0x2) goto L_80031348;
    if ((s32)r6 >= (s32)0x2) goto L_80031360;
    if ((s32)r6 >= (s32)0x1) goto L_80031330;
    goto L_80031360;
L_80031330: ;
    r5 = r7 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    r29 = r3;
    goto L_80031360;
L_80031348: ;
    r3 = (u32)lbl_803A2688;
    r5 = r7 & 0xFFFF;
    r3 = (u32)lbl_803A2688;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    r29 = r3;
L_80031360: ;
    r3 = r29;
    fn_8011E850();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80031394;
    r3 = r30;
    r4 = r31;
    r5 = r29;
    fn_8010B718();
    r3 = r31;
    r4 = 0x1;
    fn_80109220();
    goto L_800313E8;
L_80031394: ;
    r3 = r29;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800313DC;
    r3 = r29;
    fn_80075FEC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800313DC;
    r3 = r30;
    r4 = r31;
    r5 = r29;
    fn_8010B718();
    r3 = r31;
    r4 = 0x1;
    fn_80109220();
    goto L_800313E8;
L_800313DC: ;
    r3 = r31;
    r4 = 0x0;
    fn_80109220();
L_800313E8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80031404 | 0x244 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80031404(void) {
    extern u8 lbl_80266E90[];
    extern u8 lbl_803A2688[];
    extern void fn_8001DA60();
    extern void fn_80075FEC();
    extern void fn_800FA280();
    extern void fn_80109220();
    extern void fn_8011E8DC();
    extern void fn_80123FBC();
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r4;
    r5 = *(u32*)((u8*)r30 + 0x64);
    r4 = (u32)lbl_80266E90;
    r0 = *(u8*)((u8*)r3 + 0x8B);
    r4 = (u32)lbl_80266E90;
    /* clrrwi r3, r5, 8 */;
    r29 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r31 = r3 | r0;
    r5 = 0x0;
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
L_80031454: ;
    r3 = *(s16*)((u8*)r30 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xC);
    if ((s32)r3 != (s32)r0) goto L_8003146C;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_8003146C: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r30 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xC);
    if ((s32)r3 != (s32)r0) goto L_80031488;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_80031488: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r30 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xC);
    if ((s32)r3 != (s32)r0) goto L_800314A4;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800314A4: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r30 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xC);
    if ((s32)r3 != (s32)r0) goto L_800314C0;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800314C0: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r30 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xC);
    if ((s32)r3 != (s32)r0) goto L_800314DC;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800314DC: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r30 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xC);
    if ((s32)r3 != (s32)r0) goto L_800314F8;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800314F8: ;
    r4 = r4 + 0x12;
    r5 = r5 + 0x5;
    if (--ctr != 0) goto L_80031454;
    if ((s32)r6 == (s32)0x2) goto L_80031534;
    if ((s32)r6 >= (s32)0x2) goto L_8003154C;
    if ((s32)r6 >= (s32)0x1) goto L_8003151C;
    goto L_8003154C;
L_8003151C: ;
    r5 = r7 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    r29 = r3;
    goto L_8003154C;
L_80031534: ;
    r3 = (u32)lbl_803A2688;
    r5 = r7 & 0xFFFF;
    r3 = (u32)lbl_803A2688;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    r29 = r3;
L_8003154C: ;
    r3 = r29;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8003161C;
    r3 = r29;
    fn_80075FEC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8003161C;
    r3 = r29;
    fn_8001DA60();
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_800315A8;
    if ((s32)r0 >= (s32)0x1) goto L_80031598;
    if ((s32)r0 >= (s32)0x0) goto L_800315A0;
    goto L_800315B0;
L_80031598: ;
    goto L_800315B0;
L_800315A0: ;
    r28 = 0xd67;
    goto L_800315B4;
L_800315A8: ;
    r28 = 0xd68;
    goto L_800315B4;
L_800315B0: ;
    r28 = 0x0;
L_800315B4: ;
    r3 = r29;
    fn_8011E8DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800315CC;
    r28 = 0x0;
L_800315CC: ;
    if ((u32)r28 == (u32)0x0) goto L_8003160C;
    r3 = r28;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r5 = r31;
    r3 = 0x2;
    r4 = 0x0;
    r6 = 0xcf;
    ((void(*)(void))fn_800FB680)();
    r3 = r30;
    r4 = 0x1;
    fn_80109220();
    goto L_80031628;
L_8003160C: ;
    r3 = r30;
    r4 = 0x0;
    fn_80109220();
    goto L_80031628;
L_8003161C: ;
    r3 = r30;
    r4 = 0x0;
    fn_80109220();
L_80031628: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x80031648 | 0x290 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80031648(void) {
    extern u8 lbl_80266E90[];
    extern u8 lbl_803A2688[];
    extern void fn_80075FEC();
    extern void fn_800FA280();
    extern void fn_800FA444();
    extern void fn_80109220();
    extern void fn_8011E850();
    extern void fn_8011E8DC();
    extern void fn_8011F4F0();
    extern void fn_80123FBC();
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r4;
    r3 = (u32)lbl_80266E90;
    r30 = 0x0;
    r4 = (u32)lbl_80266E90;
    r6 = 0x0;
    r7 = 0x0;
    r5 = 0x0;
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
L_80031680: ;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xA);
    if ((s32)r3 != (s32)r0) goto L_80031698;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_80031698: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xA);
    if ((s32)r3 != (s32)r0) goto L_800316B4;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800316B4: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xA);
    if ((s32)r3 != (s32)r0) goto L_800316D0;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800316D0: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xA);
    if ((s32)r3 != (s32)r0) goto L_800316EC;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_800316EC: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xA);
    if ((s32)r3 != (s32)r0) goto L_80031708;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_80031708: ;
    r4 = r4 + 0x12;
    r3 = *(s16*)((u8*)r31 + 0x6);
    r0 = *(u16*)((u8*)r4 + 0xA);
    if ((s32)r3 != (s32)r0) goto L_80031724;
    r6 = *(u8*)((u8*)r4 + 0x0);
    r7 = *(u8*)((u8*)r4 + 0x1);
L_80031724: ;
    r4 = r4 + 0x12;
    r5 = r5 + 0x5;
    if (--ctr != 0) goto L_80031680;
    if ((s32)r6 == (s32)0x2) goto L_80031760;
    if ((s32)r6 >= (s32)0x2) goto L_80031778;
    if ((s32)r6 >= (s32)0x1) goto L_80031748;
    goto L_80031778;
L_80031748: ;
    r5 = r7 & 0xFFFF;
    r3 = 0x0;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    r30 = r3;
    goto L_80031778;
L_80031760: ;
    r3 = (u32)lbl_803A2688;
    r5 = r7 & 0xFFFF;
    r3 = (u32)lbl_803A2688;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    r30 = r3;
L_80031778: ;
    r3 = r30;
    fn_8011E850();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800317F4;
    r3 = 0x56c;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r3 = 0xe7;
    fn_800FA444();
    r0 = (u32)r3 >> 16;
    r3 = *(s16*)((u8*)r31 + 0x54);
    r6 = (s16)r0;
    r4 = 0x0;
    r5 = (u32)r6 >> 31;
    r0 = (u32)r3 >> 31;
    r6 = r5 + r6;
    r7 = (s32)r6 >> 1;
    r0 = r0 + r3;
    r0 = (s32)r0 >> 1;
    r5 = -0x1;
    r0 = r0 - r7;
    r6 = 0xe7;
    r3 = (s16)r0;
    ((void(*)(void))fn_800FB680)();
    r3 = r31;
    r4 = 0x1;
    fn_80109220();
    goto L_800318C0;
L_800317F4: ;
    r3 = r30;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_800318B4;
    r3 = r30;
    fn_80075FEC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800318B4;
    r3 = r30;
    fn_8011E8DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80031848;
    r3 = 0x56b;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    goto L_80031860;
L_80031848: ;
    r3 = r30;
    fn_8011F4F0();
    r0 = r3;
    r3 = 0x37;
    r4 = r0;
    ((void(*)(void))fn_80132A38)();
L_80031860: ;
    r3 = 0xe7;
    fn_800FA444();
    r0 = (u32)r3 >> 16;
    r3 = *(s16*)((u8*)r31 + 0x54);
    r6 = (s16)r0;
    r4 = 0x0;
    r5 = (u32)r6 >> 31;
    r0 = (u32)r3 >> 31;
    r6 = r5 + r6;
    r7 = (s32)r6 >> 1;
    r0 = r0 + r3;
    r0 = (s32)r0 >> 1;
    r5 = -0x1;
    r0 = r0 - r7;
    r6 = 0xe7;
    r3 = (s16)r0;
    ((void(*)(void))fn_800FB680)();
    r3 = r31;
    r4 = 0x1;
    fn_80109220();
    goto L_800318C0;
L_800318B4: ;
    r3 = r31;
    r4 = 0x0;
    fn_80109220();
L_800318C0: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x800318D8 | 0x144 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800318D8(void) {
    extern u8 lbl_80266E90[];
    extern u8 lbl_8047A420[];
    extern u8 lbl_8047A424[];
    extern void fn_80109220();
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

    r3 = (u32)lbl_80266E90;
    r7 = 0x0;
    r5 = (u32)lbl_80266E90;
    r8 = 0x0;
    r6 = 0x0;
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
L_80031900: ;
    r3 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u16*)((u8*)r5 + 0xE);
    if ((s32)r3 != (s32)r0) goto L_80031918;
    r7 = *(u8*)((u8*)r5 + 0x0);
    r8 = *(u8*)((u8*)r5 + 0x1);
L_80031918: ;
    r5 = r5 + 0x12;
    r3 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u16*)((u8*)r5 + 0xE);
    if ((s32)r3 != (s32)r0) goto L_80031934;
    r7 = *(u8*)((u8*)r5 + 0x0);
    r8 = *(u8*)((u8*)r5 + 0x1);
L_80031934: ;
    r5 = r5 + 0x12;
    r3 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u16*)((u8*)r5 + 0xE);
    if ((s32)r3 != (s32)r0) goto L_80031950;
    r7 = *(u8*)((u8*)r5 + 0x0);
    r8 = *(u8*)((u8*)r5 + 0x1);
L_80031950: ;
    r5 = r5 + 0x12;
    r3 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u16*)((u8*)r5 + 0xE);
    if ((s32)r3 != (s32)r0) goto L_8003196C;
    r7 = *(u8*)((u8*)r5 + 0x0);
    r8 = *(u8*)((u8*)r5 + 0x1);
L_8003196C: ;
    r5 = r5 + 0x12;
    r3 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u16*)((u8*)r5 + 0xE);
    if ((s32)r3 != (s32)r0) goto L_80031988;
    r7 = *(u8*)((u8*)r5 + 0x0);
    r8 = *(u8*)((u8*)r5 + 0x1);
L_80031988: ;
    r5 = r5 + 0x12;
    r3 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u16*)((u8*)r5 + 0xE);
    if ((s32)r3 != (s32)r0) goto L_800319A4;
    r7 = *(u8*)((u8*)r5 + 0x0);
    r8 = *(u8*)((u8*)r5 + 0x1);
L_800319A4: ;
    r5 = r5 + 0x12;
    r6 = r6 + 0x5;
    if (--ctr != 0) goto L_80031900;
    if ((s32)r7 == (s32)0x2) goto L_800319E4;
    if ((s32)r7 >= (s32)0x2) goto L_80031A00;
    if ((s32)r7 >= (s32)0x1) goto L_800319C8;
    goto L_80031A00;
L_800319C8: ;
    r0 = *(u32*)lbl_8047A424;
    if ((s32)r0 != (s32)r8) goto L_80031A00;
    r3 = r4;
    r4 = 0x1;
    fn_80109220();
    goto L_80031A0C;
L_800319E4: ;
    r0 = *(u32*)lbl_8047A420;
    if ((s32)r0 != (s32)r8) goto L_80031A00;
    r3 = r4;
    r4 = 0x1;
    fn_80109220();
    goto L_80031A0C;
L_80031A00: ;
    r3 = r4;
    r4 = 0x0;
    fn_80109220();
L_80031A0C: ;
    return;
}
#pragma pop

/* 0x54 | fn_80031A1C | generic */
void fn_80031A1C(void) {
    /* refs: lbl_8047A42C */
    fn_80109220();
    fn_80109220();
}

/* 0x50 | fn_80031A70 | generic */
void fn_80031A70(void) {
    /* refs: lbl_8047A42C */
    fn_80109220();
    fn_80109220();
}

/* 0x50 | fn_80031AC0 | generic */
void fn_80031AC0(void) {
    /* refs: lbl_8047A42C */
    fn_80109220();
    fn_80109220();
}

/* 0x80031B10 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80031B10(void) {
    extern u8 lbl_8047A418[];
    extern void fn_800F0308();
    extern void fn_8010B560();
    extern void fn_801CB9D8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    goto L_80031B24;
L_80031B20: ;
    fn_800F0308();
L_80031B24: ;
    fn_8010B560();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80031B20;
    r3 = *(u32*)lbl_8047A418;
    fn_801CB9D8();
    return;
}
#pragma pop

/* 0x80031B4C | 0x954 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80031B4C(void) {
    extern u8 lbl_80266F90[];
    extern u8 lbl_803A2518[];
    extern u8 lbl_804788B0[];
    extern u8 lbl_8047A409[];
    extern u8 lbl_8047A40A[];
    extern u8 lbl_8047A410[];
    extern u8 lbl_8047A414[];
    extern u8 lbl_8047A420[];
    extern u8 lbl_8047A424[];
    extern u8 lbl_8047A428[];
    extern u8 lbl_8047A42C[];
    extern u8 lbl_8047B9D0[];
    extern u8 lbl_8047B9D4[];
    extern void fn_8001E074();
    extern void fn_8002DD24();
    extern void fn_8002DF10();
    extern void fn_8002E26C();
    extern void fn_8002E460();
    extern void fn_8002EA5C();
    extern void fn_8002EE74();
    extern void fn_8002F284();
    extern void fn_8002F79C();
    extern void fn_8002FC58();
    extern void fn_800849B4();
    extern void fn_80097CD0();
    extern void fn_800E4014();
    extern void fn_800FF52C();
    extern void fn_800FF660();
    extern void fn_80102004();
    extern void fn_801021F8();
    extern void fn_80102254();
    extern void fn_801023E4();
    extern void fn_801024E8();
    extern void fn_80102510();
    extern void fn_8010264C();
    extern void fn_801026A4();
    extern void fn_80103CC0();
    extern void fn_801043A4();
    extern void fn_801045A8();
    extern void fn_801046B8();
    extern void fn_801046C8();
    extern void fn_80104704();
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_80109220();
    extern void fn_80109C88();
    extern void fn_8010A420();
    extern void fn_8010A5BC();
    extern void fn_80113828();
    extern void fn_8011394C();
    extern void fn_8011D904();
    extern void fn_8011F4F0();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801D0314();
    extern void fn_801D036C();
    extern void fn_8025FF9C();
    extern u8 jumptable_802E4F90[];
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
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r26, 0x28(r1) */;
    r0 = *(u8*)lbl_8047A409;
    r3 = 0x1;
    r4 = (u32)lbl_803A2518;
    *(u8*)lbl_804788B0 = r3;
    r31 = (u32)lbl_803A2518;
    if ((u32)r0 == (u32)0x1) goto L_80032484;
    fn_801D036C();
    r30 = r3;
    goto L_800323E4;
L_80031B84: ;
    if ((u32)r0 > (u32)0x13) goto L_800323E4;
    r3 = (u32)jumptable_802E4F90;
    r0 = r0 << 2;
    r3 = (u32)jumptable_802E4F90;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = (u32)lbl_80266F90;
    r5 = r1 + 0x10;
    r10 = (u32)lbl_80266F90;
    r3 = 0x0;
    r9 = *(u32*)((u8*)r10 + 0x0);
    r4 = 0x1;
    r8 = *(u32*)((u8*)r10 + 0x4);
    r6 = 0x0;
    r7 = *(u32*)((u8*)r10 + 0x8);
    r0 = *(u32*)((u8*)r10 + 0xC);
    *(u32*)(sp + 0x1C) = r0;
    fn_800849B4();
    if ((s32)r3 >= (s32)0x0) goto L_80031BF8;
    r0 = 0x0;
    *(u8*)lbl_8047A40A = r0;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_80031BF8: ;
    r0 = 0x2;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    fn_8002FC58();
    goto L_800323E4;
    r3 = 0xd9;
    r4 = 0x0;
    fn_801021F8();
    r0 = 0x1;
    *(u32*)(sp + 0xC) = r0;
    fn_801046B8();
    r4 = r3;
    r5 = r1 + 0xc;
    r3 = 0xe3;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xe3;
    fn_80104704();
    r29 = r3;
    r4 = 0x102a;
    fn_801046C8();
    r29 = r3;
    if ((u32)r29 == (u32)0x0) goto L_80031C7C;
    if ((u32)r29 == (u32)0x0) goto L_80031C7C;
    r4 = 0x1;
    fn_80109220();
    r0 = 0x43e4;
    *(u32*)((u8*)r29 + 0x4C) = r0;
L_80031C7C: ;
    r3 = 0xe3;
    fn_80104704();
    r29 = r3;
    r4 = 0x1029;
    fn_801046C8();
    r29 = r3;
    if ((u32)r29 == (u32)0x0) goto L_80031CB4;
    if ((u32)r29 == (u32)0x0) goto L_80031CB4;
    r4 = 0x1;
    fn_80109220();
    r0 = 0x43e5;
    *(u32*)((u8*)r29 + 0x4C) = r0;
L_80031CB4: ;
    r3 = 0xe3;
    r4 = 0x1;
    fn_801045A8();
    r3 = 0xe3;
    fn_801043A4();
    r29 = r3;
    r3 = 0xe3;
    fn_801023E4();
    r26 = r3;
    r3 = 0xe3;
    fn_80102510();
    if ((s32)r29 != (s32)-0x1) goto L_80031CEC;
    r26 = -0x1;
L_80031CEC: ;
    if ((s32)r26 == (s32)0x0) goto L_80031D18;
    if ((s32)r26 >= (s32)0x0) goto L_80031D04;
    if ((s32)r26 >= (s32)-0x1) goto L_80031D24;
    goto L_800323E4;
L_80031D04: ;
    if ((s32)r26 >= (s32)0x2) goto L_800323E4;
    r0 = 0x5;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_80031D18: ;
    r0 = 0x6;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_80031D24: ;
    r0 = 0x2;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    r3 = 0xd9;
    r4 = 0x0;
    fn_801021F8();
    r3 = 0xd9;
    fn_80104704();
    r29 = r3;
    r4 = 0x10b2;
    fn_801046C8();
    r29 = r3;
    if ((u32)r29 == (u32)0x0) goto L_80031D74;
    if ((u32)r29 == (u32)0x0) goto L_80031D74;
    r4 = 0x1;
    fn_80109220();
    r0 = 0x43db;
    *(u32*)((u8*)r29 + 0x4C) = r0;
L_80031D74: ;
    r0 = 0x1;
    *(u32*)(sp + 0x8) = r0;
    fn_801046B8();
    r4 = r3;
    r5 = r1 + 0x8;
    r3 = 0xe3;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    /* crclr cr1eq */;
    fn_801026A4();
    r3 = 0xe3;
    fn_80104704();
    r29 = r3;
    r4 = 0x102a;
    fn_801046C8();
    r29 = r3;
    if ((u32)r29 == (u32)0x0) goto L_80031DD8;
    if ((u32)r29 == (u32)0x0) goto L_80031DD8;
    r4 = 0x1;
    fn_80109220();
    r0 = 0x43d4;
    *(u32*)((u8*)r29 + 0x4C) = r0;
L_80031DD8: ;
    r3 = 0xe3;
    fn_80104704();
    r29 = r3;
    r4 = 0x1029;
    fn_801046C8();
    r29 = r3;
    if ((u32)r29 == (u32)0x0) goto L_80031E10;
    if ((u32)r29 == (u32)0x0) goto L_80031E10;
    r4 = 0x1;
    fn_80109220();
    r0 = 0x43d5;
    *(u32*)((u8*)r29 + 0x4C) = r0;
L_80031E10: ;
    r3 = 0xe3;
    r4 = 0x1;
    fn_801045A8();
    r3 = 0xe3;
    fn_801043A4();
    r29 = r3;
    r3 = 0xe3;
    fn_801023E4();
    r26 = r3;
    r3 = 0xe3;
    fn_80102510();
    if ((s32)r29 != (s32)-0x1) goto L_80031E48;
    r26 = -0x1;
L_80031E48: ;
    r3 = 0xd9;
    fn_80104704();
    r29 = r3;
    r4 = 0x10b2;
    fn_801046C8();
    if ((u32)r29 == (u32)0x0) goto L_80031E7C;
    if ((u32)r3 == (u32)0x0) goto L_80031E7C;
    r0 = 0x0;
    r4 = 0x0;
    *(u32*)((u8*)r3 + 0x4C) = r0;
    fn_80109220();
L_80031E7C: ;
    if ((s32)r26 == (s32)0x0) goto L_80031EA0;
    if ((s32)r26 >= (s32)0x0) goto L_80031E94;
    if ((s32)r26 >= (s32)-0x1) goto L_80031EB8;
    goto L_800323E4;
L_80031E94: ;
    if ((s32)r26 >= (s32)0x2) goto L_800323E4;
    goto L_80031EAC;
L_80031EA0: ;
    r0 = 0x2;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_80031EAC: ;
    r0 = 0x0;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_80031EB8: ;
    r0 = 0x2;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    f1 = *(f32*)lbl_8047B9D0;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = *(u32*)lbl_8047A414;
    r4 = 0x0;
    fn_800E4014();
    r3 = 0xd9;
    r4 = 0x0;
    fn_80102254();
    f1 = *(f32*)lbl_8047B9D4;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r0 = *(u32*)lbl_8047A428;
    r3 = 0x0;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    ((void(*)(void))fn_8012A5B0)();
    r4 = 0x0;
    r5 = 0x0;
    fn_80097CD0();
    f1 = *(f32*)lbl_8047B9D4;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = *(u32*)lbl_8047A414;
    r4 = 0x1;
    fn_800E4014();
    r3 = 0xd9;
    r4 = 0x1;
    fn_80102254();
    f1 = *(f32*)lbl_8047B9D0;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r0 = 0x2;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    fn_8002F79C();
    goto L_800323E4;
    fn_8002F284();
    goto L_800323E4;
    fn_8002EE74();
    goto L_800323E4;
    r4 = 0x1;
    r3 = -0x1;
    r0 = 0x2;
    *(u8*)lbl_8047A410 = r4;
    *(u32*)lbl_8047A424 = r3;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    f1 = *(f32*)lbl_8047B9D0;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = *(u32*)lbl_8047A414;
    r4 = 0x0;
    fn_800E4014();
    r3 = 0xdc;
    r4 = 0x0;
    fn_80102254();
    r3 = 0xd9;
    r4 = 0x0;
    fn_80102254();
    f1 = *(f32*)lbl_8047B9D4;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r0 = *(u32*)lbl_8047A428;
    r3 = r31 + 0x170;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    ((void(*)(void))fn_8012A5B0)();
    r29 = r3;
    r3 = 0x2;
    fn_80103CC0();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x0;
    fn_80097CD0();
    r3 = 0x1;
    fn_80103CC0();
    f1 = *(f32*)lbl_8047B9D4;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = *(u32*)lbl_8047A414;
    r4 = 0x1;
    fn_800E4014();
    r3 = 0xd9;
    r4 = 0x1;
    fn_80102254();
    f1 = *(f32*)lbl_8047B9D0;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    fn_80102004();
    if ((s32)r3 != (s32)0x1) goto L_8003209C;
    r3 = 0x2;
    r4 = 0x4448;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0xd9;
    fn_80102510();
    r0 = 0x0;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_8003209C: ;
    r0 = 0x7;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    fn_8002EA5C();
    goto L_800323E4;
    r3 = r30;
    fn_8002E460();
    goto L_800323E4;
    r3 = r31 + 0x170;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
    r0 = *(u32*)lbl_8047A424;
    r26 = r3;
    r3 = 0x0;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    ((void(*)(void))fn_8012A5B0)();
    r0 = *(u32*)lbl_8047A420;
    r28 = r3;
    r3 = r31 + 0x170;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    ((void(*)(void))fn_8012A5B0)();
    r29 = r3;
    r3 = 0xde;
    r4 = 0x0;
    fn_8010264C();
    r3 = r31 + 0xd18;
    r4 = 0xe8;
    r5 = 0x11c;
    fn_8010A5BC();
    r3 = r31 + 0xcd0;
    r4 = 0xe8;
    r5 = 0x11c;
    fn_8010A5BC();
    r4 = r28;
    r3 = r31 + 0xd18;
    fn_80109C88();
    r4 = r29;
    r3 = r31 + 0xcd0;
    fn_80109C88();
    r3 = r28;
    fn_8011F4F0();
    r27 = r3;
    r3 = r29;
    fn_8011F4F0();
    r28 = r3;
    r4 = r26;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r4 = r27;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r4 = r28;
    r3 = 0x33;
    ((void(*)(void))fn_80132A38)();
    f1 = *(f32*)lbl_8047B9D0;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x2;
    r4 = 0x43d6;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r4 = r27;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r4 = r28;
    r3 = 0x33;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x2;
    r4 = 0x43d8;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    r0 = 0xe;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    fn_8002E26C();
    goto L_800323E4;
    r3 = 0x2;
    r4 = 0x43dc;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0xde;
    fn_80102510();
    r3 = r31 + 0xd18;
    fn_8010A420();
    r3 = r31 + 0xcd0;
    fn_8010A420();
    r0 = 0x0;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    r3 = r31 + 0x170;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
    r0 = *(u32*)lbl_8047A424;
    r28 = r3;
    r3 = 0x0;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    ((void(*)(void))fn_8012A5B0)();
    r0 = *(u32*)lbl_8047A420;
    r27 = r3;
    r3 = r31 + 0x170;
    r4 = 0x3;
    r5 = r0 & 0xFFFF;
    ((void(*)(void))fn_8012A5B0)();
    r26 = r3;
    r3 = r27;
    r4 = 0x46;
    fn_8011D904();
    r3 = r26;
    r4 = 0x46;
    fn_8011D904();
    r4 = r27;
    r3 = 0x0;
    fn_8025FF9C();
    r3 = r27;
    fn_8011F4F0();
    r27 = r3;
    r3 = r26;
    fn_8011F4F0();
    r26 = r3;
    r4 = r28;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r4 = r27;
    r3 = 0x32;
    ((void(*)(void))fn_80132A38)();
    r4 = r26;
    r3 = 0x33;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x2;
    r4 = 0x43da;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    r0 = 0x11;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
    fn_8002DF10();
    goto L_800323E4;
    r3 = r30;
    fn_8002DD24();
    goto L_800323E4;
    r3 = 0x2;
    r4 = 0x43de;
    r5 = 0x0;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x0;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    fn_8001E074();
    r29 = r3;
    r3 = 0x1;
    fn_801069FC();
    r0 = (s8)r29;
    if ((s32)r0 == (s32)0x0) goto L_80032364;
    if ((s32)r0 >= (s32)0x0) goto L_80032358;
    if ((s32)r0 >= (s32)-0x1) goto L_800323C4;
    goto L_800323E4;
L_80032358: ;
    if ((s32)r0 >= (s32)0x2) goto L_800323E4;
    goto L_800323A0;
L_80032364: ;
    r3 = 0xde;
    fn_80102510();
    r0 = -0x1;
    r3 = r31 + 0xd18;
    *(u32*)lbl_8047A428 = r0;
    *(u32*)lbl_8047A424 = r0;
    *(u32*)lbl_8047A420 = r0;
    fn_8010A420();
    r3 = r31 + 0xcd0;
    fn_8010A420();
    r3 = 0x1;
    r0 = 0x2;
    *(u8*)lbl_8047A410 = r3;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_800323A0: ;
    r3 = 0xde;
    fn_80102510();
    r3 = r31 + 0xd18;
    fn_8010A420();
    r3 = r31 + 0xcd0;
    fn_8010A420();
    r0 = 0x0;
    *(u32*)lbl_8047A42C = r0;
    goto L_800323E4;
L_800323C4: ;
    r3 = 0xde;
    fn_80102510();
    r3 = r31 + 0xd18;
    fn_8010A420();
    r3 = r31 + 0xcd0;
    fn_8010A420();
    r0 = 0x0;
    *(u32*)lbl_8047A42C = r0;
L_800323E4: ;
    r0 = *(u32*)lbl_8047A42C;
    if ((s32)r0 > (s32)0x0) goto L_80031B84;
    r3 = r30;
    fn_801D0314();
    r0 = *(u8*)lbl_8047A40A;
    if ((u32)r0 == (u32)0x0) goto L_80032450;
    r0 = *(u8*)lbl_804788B0;
    if ((u32)r0 == (u32)0x0) goto L_80032430;
    r3 = 0x2;
    r4 = 0x44d0;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_80032458;
L_80032430: ;
    r3 = 0x2;
    r4 = 0x44f1;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_80032458;
L_80032450: ;
    r0 = 0x1;
    *(u8*)lbl_8047A40A = r0;
L_80032458: ;
    r3 = 0x1;
    fn_801024E8();
    fn_800FF52C();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80032478;
    fn_800FF660();
    goto L_80032484;
L_80032478: ;
    fn_8011394C();
    r4 = 0x0;
    fn_80113828();
L_80032484: ;
    r0 = 0x1;
    *(u8*)lbl_8047A409 = r0;
    /* lmw r26, 0x28(r1) */;
    return;
}
#pragma pop

/* 0x800324A0 | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800324A0(void) {
    extern u8 lbl_8047A408[];
    extern u8 lbl_8047A409[];
    extern u8 lbl_8047A40A[];
    extern u8 lbl_8047A410[];
    extern u8 lbl_8047A414[];
    extern u8 lbl_8047A418[];
    extern u8 lbl_8047A41C[];
    extern u8 lbl_8047A420[];
    extern u8 lbl_8047A424[];
    extern u8 lbl_8047A428[];
    extern u8 lbl_8047A42C[];
    extern void fn_800F9318();
    extern void fn_80112260();
    extern void fn_80113F48();
    extern void fn_80176E0C();
    extern void fn_80177A44();
    extern void fn_801CB7C4();
    extern void fn_801CBA0C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r3 = 0x0;
    fn_80112260();
    r0 = *(u8*)lbl_8047A408;
    if ((u32)r0 == (u32)0x0) goto L_800324D0;
    r0 = 0x0;
    *(u8*)lbl_8047A408 = r0;
    goto L_80032550;
L_800324D0: ;
    r3 = 0x1;
    r0 = -0x1;
    r4 = 0x0;
    *(u32*)lbl_8047A42C = r3;
    *(u8*)lbl_8047A409 = r4;
    *(u32*)lbl_8047A428 = r0;
    *(u32*)lbl_8047A424 = r0;
    *(u32*)lbl_8047A420 = r0;
    *(u8*)lbl_8047A410 = r3;
    *(u8*)lbl_8047A41C = r3;
    *(u8*)lbl_8047A40A = r3;
    fn_80113F48();
    r4 = (0xffe << 16);
    r31 = r3;
    r3 = r4 + 0x1000;
    fn_801CBA0C();
    r4 = r3;
    r3 = r31;
    *(u32*)lbl_8047A418 = r4;
    fn_800F9318();
    *(u32*)lbl_8047A414 = r3;
    r3 = (0xfff << 16);
    r4 = r3 + 0x1800;
    r3 = 0x37c;
    r5 = 0x0;
    r6 = 0x1;
    fn_80176E0C();
    r3 = 0x4;
    fn_80177A44();
    r3 = (0x10b1 << 16);
    r3 = r3 + 0x1000;
    fn_801CB7C4();
L_80032550: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80032564 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80032564(void) {
    extern void fn_800F0308();
    extern void fn_800FF730();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r3 = 0x394;
    fn_800FF730();
    fn_800F0308();
    return;
}
#pragma pop

/* 0x8003258C | 0x270 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8003258C(void) {
    extern u8 lbl_803A3278[];
    extern u8 lbl_8047A43C[];
    extern u8 lbl_8047A44A[];
    extern u8 lbl_8047A450[];
    extern u8 lbl_8047A458[];
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_801293FC();
    extern void fn_80166AB8();
    extern u8 jumptable_802E4FE0[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r3 = *(u32*)lbl_8047A450;
    r0 = -0x1;
    if ((u32)r3 >= (u32)r0) goto L_800325B4;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047A450 = r0;
L_800325B4: ;
    r4 = *(u32*)lbl_8047A450;
    r3 = (0xaaab << 16);
    /* subi r0, r3, 0x5555 */;
    r3 = r4 + 0x1;
    r0 = (u32)((u64)r0 * (u64)r3 >> 32);
    r0 = (u32)r0 >> 2;
    r0 = r0 * 0x6;
    r0 = r3 - r0;
    if ((u32)r0 != (u32)0x0) goto L_800325E8;
    r0 = 0xe;
    *(u32*)lbl_8047A458 = r0;
    goto L_800327E8;
L_800325E8: ;
    r0 = *(u8*)lbl_8047A44A;
    if ((u32)r0 == (u32)0x0) goto L_800327E0;
    r0 = 0x0;
    *(u8*)lbl_8047A44A = r0;
    if ((u32)r4 >= (u32)0x1e) goto L_80032694;
    /* subi r0, r4, 0x6 */;
    if ((u32)r0 > (u32)0x12) goto L_80032648;
    r3 = (u32)jumptable_802E4FE0;
    r0 = r0 << 2;
    r3 = (u32)jumptable_802E4FE0;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r31 = 0x258;
    goto L_8003264C;
    r31 = 0x4b0;
    goto L_8003264C;
    r31 = 0x960;
    goto L_8003264C;
    r31 = 0x12c0;
    goto L_8003264C;
L_80032648: ;
    r31 = 0x258;
L_8003264C: ;
    r3 = 0x3cc;
    r4 = 0x0;
    r5 = 0x0;
    fn_80166AB8();
    r4 = r31;
    r3 = 0x0;
    fn_801293FC();
    r4 = r31;
    r3 = 0x2f;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x8;
    r4 = 0x3b60;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_80032794;
L_80032694: ;
    if ((s32)r4 == (s32)0x5a) goto L_80032738;
    if ((s32)r4 >= (s32)0x5a) goto L_800326B8;
    if ((s32)r4 == (s32)0x3c) goto L_80032718;
    if ((s32)r4 >= (s32)0x3c) goto L_80032794;
    if ((s32)r4 == (s32)0x1e) goto L_800326D0;
    goto L_80032794;
L_800326B8: ;
    if ((s32)r4 == (s32)0x96) goto L_80032778;
    if ((s32)r4 >= (s32)0x96) goto L_80032794;
    if ((s32)r4 == (s32)0x78) goto L_80032758;
    goto L_80032794;
L_800326D0: ;
    r3 = 0x3cc;
    r4 = 0x0;
    r5 = 0x0;
    fn_80166AB8();
    r3 = 0x0;
    r4 = 0x2580;
    fn_801293FC();
    r3 = 0x2f;
    r4 = 0x2580;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x8;
    r4 = 0x3b8c;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_80032794;
L_80032718: ;
    r3 = 0x8;
    r4 = 0x3b8f;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_80032794;
L_80032738: ;
    r3 = 0x8;
    r4 = 0x3b53;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_80032794;
L_80032758: ;
    r3 = 0x8;
    r4 = 0x3b56;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
    goto L_80032794;
L_80032778: ;
    r3 = 0x8;
    r4 = 0x3b59;
    r5 = 0x1;
    r6 = 0x0;
    fn_80106D3C();
    r3 = 0x1;
    fn_801069FC();
L_80032794: ;
    r3 = (u32)lbl_803A3278;
    r0 = 0x0;
    r3 = (u32)lbl_803A3278;
    *(u32*)lbl_8047A43C = r0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    *(u8*)((u8*)r3 + 0x1) = r0;
    *(u8*)((u8*)r3 + 0x2) = r0;
    *(u8*)((u8*)r3 + 0x3) = r0;
    *(u8*)((u8*)r3 + 0x4) = r0;
    *(u8*)((u8*)r3 + 0x5) = r0;
    *(u8*)((u8*)r3 + 0x6) = r0;
    *(u8*)((u8*)r3 + 0x7) = r0;
    *(u8*)((u8*)r3 + 0x8) = r0;
    *(u8*)((u8*)r3 + 0x9) = r0;
    *(u8*)((u8*)r3 + 0xA) = r0;
    *(u8*)((u8*)r3 + 0xB) = r0;
    *(u8*)((u8*)r3 + 0xC) = r0;
    *(u8*)((u8*)r3 + 0xD) = r0;
    *(u8*)((u8*)r3 + 0xE) = r0;
L_800327E0: ;
    r0 = 0x5;
    *(u32*)lbl_8047A458 = r0;
L_800327E8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x800327FC | 0x6DC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800327FC(void) {
    extern u8 lbl_803A3278[];
    extern u8 lbl_803A3334[];
    extern u8 lbl_8047A430[];
    extern u8 lbl_8047A434[];
    extern u8 lbl_8047A43C[];
    extern u8 lbl_8047A448[];
    extern u8 lbl_8047A449[];
    extern u8 lbl_8047A44A[];
    extern u8 lbl_8047A458[];
    extern u8 lbl_8047B9F8[];
    extern u8 lbl_8047BA00[];
    extern u8 lbl_8047BA08[];
    extern u8 lbl_8047BA10[];
    extern u8 lbl_8047BA18[];
    extern void fn_80032ED8();
    extern void fn_80082EA4();
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_800E0C04();
    extern void fn_800F0308();
    extern void fn_800F9E70();
    extern void fn_80102510();
    extern void fn_80102620();
    extern void fn_8010264C();
    extern void fn_80123FBC();
    extern void fn_8012640C();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    u8 sp[0x2C0];
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f4 = 0.0f;
    f32 f6 = 0.0f;
    f32 f8 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    *(f64*)(sp + 0x2B0) = f31;
    /* psq_st f31, 0x2b8(r1), 0, qr0 */;
    *(f64*)(sp + 0x2A0) = f30;
    /* psq_st f30, 0x2a8(r1), 0, qr0 */;
    *(f64*)(sp + 0x290) = f29;
    /* psq_st f29, 0x298(r1), 0, qr0 */;
    *(f64*)(sp + 0x280) = f28;
    /* psq_st f28, 0x288(r1), 0, qr0 */;
    *(f64*)(sp + 0x270) = f27;
    /* psq_st f27, 0x278(r1), 0, qr0 */;
    /* stmw r14, 0x228(r1) */;
    r0 = *(u8*)lbl_8047A44A;
    if ((u32)r0 == (u32)0x0) goto L_80032BB8;
    r4 = *(u32*)lbl_8047A430;
    r14 = 0x0;
    r0 = 0xa;
    ctr_fn = (void(*)(void))r0;
L_80032850: ;
    r3 = r4 + 0x41e4;
    if ((u32)r3 == (u32)0x0) goto L_80032894;
    r0 = *(u16*)((u8*)r3 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_80032874;
    r0 = *(u16*)((u8*)r3 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_80032894;
L_80032874: ;
    r3 = 0xa - r14;
    fn_800E0C04();
    r3 = r14 + r3;
    r0 = *(u32*)lbl_8047A430;
    r3 = r3 * 0xca;
    r3 = r3 + 0x41e4;
    r3 = r0 + r3;
    goto L_800328A0;
L_80032894: ;
    r4 = r4 + 0xca;
    r14 = r14 + 0x1;
    if (--ctr != 0) goto L_80032850;
L_800328A0: ;
    r0 = 0x19;
    r5 = r1 + 0xe0;
    /* subi r4, r3, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_800328B0: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_800328B0;
    r0 = *(u16*)((u8*)r4 + 0x4);
    r3 = r1 + 0x14;
    r4 = r1 + 0xea;
    *(u16*)((u8*)r5 + 0x4) = r0;
    fn_800F9E70();
    r15 = *(u32*)(sp + 0x150);
    r0 = 0x0;
    r3 = *(u32*)(sp + 0x148);
    r15 = *(u32*)(sp + 0x154);
    r3 = *(u32*)(sp + 0x220);
    r15 = *(u16*)(sp + 0x158);
    r3 = *(u32*)(sp + 0x1C0);
    r15 = *(u32*)(sp + 0x15A);
    r3 = *(u32*)(sp + 0x1C4);
    r15 = *(u32*)(sp + 0x15E);
    *(u16*)(sp + 0x8E) = r3;
    r3 = *(u32*)(sp + 0x1C8);
    r15 = *(u32*)(sp + 0x162);
    r3 = *(u32*)(sp + 0x1CC);
    r15 = *(u32*)(sp + 0x166);
    r3 = *(u32*)(sp + 0x1D0);
    r15 = *(u32*)(sp + 0x16A);
    r3 = *(u32*)(sp + 0x1D4);
    r15 = *(u32*)(sp + 0x16E);
    r3 = *(u32*)(sp + 0x1D8);
    r15 = *(u32*)(sp + 0x172);
    r3 = *(u32*)(sp + 0x1DC);
    r15 = *(u32*)(sp + 0x176);
    r3 = *(u32*)(sp + 0x1E0);
    r15 = *(u32*)(sp + 0x17A);
    r3 = *(u32*)(sp + 0x1E4);
    r15 = *(u32*)(sp + 0x17E);
    r3 = *(u32*)(sp + 0x1E8);
    r15 = *(u16*)(sp + 0x182);
    r3 = *(u32*)(sp + 0x1EC);
    r15 = *(u32*)(sp + 0x184);
    r3 = *(u32*)(sp + 0x1F0);
    r15 = *(u32*)(sp + 0x188);
    *(u16*)(sp + 0xB8) = r3;
    r3 = *(u32*)(sp + 0x1F4);
    r15 = *(u32*)(sp + 0x18C);
    r3 = *(u32*)(sp + 0x1F8);
    r15 = *(u32*)(sp + 0x190);
    r3 = *(u32*)(sp + 0x1FC);
    r15 = *(u32*)(sp + 0x194);
    r3 = *(u32*)(sp + 0x200);
    r15 = *(u32*)(sp + 0x198);
    r3 = *(u32*)(sp + 0x204);
    r15 = *(u32*)(sp + 0x19C);
    r3 = *(u32*)(sp + 0x208);
    r15 = *(u32*)(sp + 0x1A0);
    r3 = *(u32*)(sp + 0x20C);
    r15 = *(u32*)(sp + 0x1A4);
    r3 = *(u32*)(sp + 0x210);
    r15 = *(u32*)(sp + 0x1A8);
    r3 = *(u32*)(sp + 0x214);
    r15 = *(u16*)(sp + 0x1AC);
    r3 = *(u32*)(sp + 0x218);
    r16 = *(u8*)(sp + 0xF6);
    r17 = *(u16*)(sp + 0xF8);
    r18 = *(u16*)(sp + 0xFA);
    r19 = *(u16*)(sp + 0xFC);
    r20 = *(u16*)(sp + 0xFE);
    r21 = *(u8*)(sp + 0x100);
    r22 = *(u16*)(sp + 0x102);
    r23 = *(u8*)(sp + 0x104);
    r24 = *(u32*)(sp + 0x106);
    r25 = *(u32*)(sp + 0x10A);
    r26 = *(u32*)(sp + 0x10E);
    r27 = *(u32*)(sp + 0x112);
    r28 = *(u32*)(sp + 0x116);
    r29 = *(u32*)(sp + 0x11A);
    r30 = *(u32*)(sp + 0x11E);
    r31 = *(u32*)(sp + 0x122);
    r12 = *(u32*)(sp + 0x126);
    r11 = *(u32*)(sp + 0x12A);
    r10 = *(u16*)(sp + 0x12E);
    r9 = *(u32*)(sp + 0x130);
    r8 = *(u32*)(sp + 0x134);
    r7 = *(u32*)(sp + 0x138);
    r6 = *(u32*)(sp + 0x13C);
    r5 = *(u32*)(sp + 0x140);
    r4 = *(u32*)(sp + 0x144);
    r14 = *(u32*)(sp + 0x14C);
    r3 = r15;
    *(u8*)(sp + 0x20) = r16;
    *(u8*)(sp + 0x21) = r0;
    *(u8*)(sp + 0x22) = r0;
    *(u8*)(sp + 0x23) = r0;
    *(u8*)(sp + 0x24) = r0;
    *(u16*)(sp + 0x26) = r17;
    *(u16*)(sp + 0x28) = r18;
    *(u16*)(sp + 0x2A) = r19;
    *(u16*)(sp + 0x2C) = r20;
    *(u16*)(sp + 0x34) = r22;
    *(u16*)(sp + 0x36) = r0;
    *(u8*)(sp + 0x38) = r23;
    *(u16*)(sp + 0x64) = r10;
    *(u16*)(sp + 0xE2) = r3;
    r3 = (u32)lbl_803A3278;
    *(u32*)lbl_8047A43C = r0;
    r6 = (u32)lbl_803A3278;
    r3 = r1 + 0x14;
    *(u8*)((u8*)r6 + 0x0) = r0;
    r5 = r1 + 0x3c;
    r4 = 0x209;
    *(u8*)((u8*)r6 + 0x1) = r0;
    *(u8*)((u8*)r6 + 0x2) = r0;
    *(u8*)((u8*)r6 + 0x3) = r0;
    *(u8*)((u8*)r6 + 0x4) = r0;
    *(u8*)((u8*)r6 + 0x5) = r0;
    *(u8*)((u8*)r6 + 0x6) = r0;
    *(u8*)((u8*)r6 + 0x7) = r0;
    *(u8*)((u8*)r6 + 0x8) = r0;
    *(u8*)((u8*)r6 + 0x9) = r0;
    *(u8*)((u8*)r6 + 0xA) = r0;
    *(u8*)((u8*)r6 + 0xB) = r0;
    *(u8*)((u8*)r6 + 0xC) = r0;
    *(u8*)((u8*)r6 + 0xD) = r0;
    *(u8*)((u8*)r6 + 0xE) = r0;
    fn_80032ED8();
    r15 = r3;
    goto L_80032DC8;
L_80032BB8: ;
    r3 = (u32)lbl_803A3334;
    r16 = 0x0;
    r3 = (u32)lbl_803A3334;
    r15 = 0x1;
    r19 = *(u8*)((u8*)r3 + 0x24);
    r17 = r15;
    r18 = *(u8*)((u8*)r3 + 0x26);
    r20 = r16;
    goto L_80032C2C;
L_80032BDC: ;
    r5 = r20;
    r3 = 0x0;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    r14 = r3;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80032C28;
    r3 = r14;
    r4 = 0x0;
    r5 = 0x7a;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFF;
    r0 = r17 & 0xFF;
    if ((u32)r0 >= (u32)r3) goto L_80032C28;
    r17 = r3;
L_80032C28: ;
    r20 = r20 + 0x1;
L_80032C2C: ;
    r0 = r20 & 0xFFFF;
    if ((u32)r0 < (u32)0x6) goto L_80032BDC;
    r3 = (u32)lbl_803A3334;
    r21 = r1 + 0x8;
    r14 = (u32)lbl_803A3334;
    r22 = r17 & 0xFF;
    r20 = r14;
    r17 = 0x0;
    r23 = r21;
    goto L_80032D30;
L_80032C58: ;
    r3 = *(u32*)lbl_8047A434;
    r5 = r19;
    r6 = r18;
    r4 = (s8)r17;
    fn_80082EA4();
    r0 = *(u8*)((u8*)r20 + 0x5B);
    r4 = 0x0;
    r5 = r4;
    r0 = (s8)r0;
    r0 = r0 * 0x28;
    r6 = r14 + r0;
    goto L_80032CC0;
L_80032C88: ;
    r3 = r5 & 0xFFFF;
    r0 = r3 + 0x3b9;
    r0 = *(u8*)(r6 + r0);
    r0 = (s8)r0;
    if ((s32)r0 < (s32)0x0) goto L_80032CBC;
    r3 = r0 * 0x2a;
    r0 = r4 & 0xFF;
    r3 = r14 + r3;
    r3 = *(u8*)((u8*)r3 + 0x517);
    if ((u32)r0 >= (u32)r3) goto L_80032CBC;
    r4 = r3;
L_80032CBC: ;
    r5 = r5 + 0x1;
L_80032CC0: ;
    r0 = r5 & 0xFFFF;
    if ((u32)r0 < (u32)0x4) goto L_80032C88;
    r3 = r4 & 0xFF;
    r0 = (s8)r16;
    r3 = r22 - r3;
    r4 = (s32)r3 >> 31;
    r0 = r0 << 2;
    r3 = r4 ^ r3;
    r3 = r3 - r4;
    *(u32*)((u8*)r21 + 0x0) = r3;
    r3 = *(u32*)((u8*)r21 + 0x0);
    r0 = *(u32*)(r23 + r0);
    if ((s32)r3 >= (s32)r0) goto L_80032D08;
    r16 = (s8)r17;
    r15 = 0x1;
    goto L_80032D24;
L_80032D08: ;
    if ((s32)r3 != (s32)r0) goto L_80032D24;
    r15 = r15 + 0x1;
    r3 = r15;
    fn_800E0C04();
    if ((u32)r3 != (u32)0x0) goto L_80032D24;
    r16 = (s8)r17;
L_80032D24: ;
    r20 = r20 + 0x1;
    r21 = r21 + 0x4;
    r17 = r17 + 0x1;
L_80032D30: ;
    r0 = *(u8*)((u8*)r14 + 0x58);
    r0 = (s8)r0;
    if ((s32)r17 < (s32)r0) goto L_80032C58;
    r3 = (u32)lbl_803A3334;
    r0 = (s8)r16;
    r6 = (u32)lbl_803A3334;
    *(u8*)lbl_8047A448 = r16;
    r4 = r6 + r0;
    r3 = r1 + 0x14;
    r0 = *(u8*)((u8*)r4 + 0x5B);
    r4 = 0x209;
    r5 = 0x0;
    r0 = (s8)r0;
    r0 = r0 * 0x28;
    r16 = r6 + r0;
    r15 = *(u32*)((u8*)r16 + 0x3AC);
    r14 = *(u32*)((u8*)r16 + 0x3B0);
    r12 = *(u32*)((u8*)r16 + 0x3B4);
    r11 = *(u32*)((u8*)r16 + 0x3B8);
    r10 = *(u32*)((u8*)r16 + 0x3BC);
    r9 = *(u32*)((u8*)r16 + 0x3C0);
    r8 = *(u32*)((u8*)r16 + 0x3C4);
    r7 = *(u32*)((u8*)r16 + 0x3C8);
    r6 = *(u32*)((u8*)r16 + 0x3CC);
    r0 = *(u32*)((u8*)r16 + 0x3D0);
    *(u32*)(sp + 0x38) = r0;
    fn_80032ED8();
    r15 = r3;
L_80032DC8: ;
    r3 = 0xa5;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80032DE4;
    r3 = 0xa5;
    fn_80102510();
L_80032DE4: ;
    f1 = *(f32*)lbl_8047BA00;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0xa5;
    r4 = 0x1;
    fn_8010264C();
    r3 = 0x1;
    fn_801C40F0();
    r0 = 0x1;
    f27 = *(f32*)lbl_8047B9F8;
    *(u8*)lbl_8047A449 = r0;
    f28 = *(f64*)lbl_8047BA08;
    r14 = (0x4330 << 16);
    f30 = *(f64*)lbl_8047BA10;
    f31 = *(f32*)lbl_8047BA18;
    goto L_80032E5C;
L_80032E24: ;
    fn_800F0308();
    fn_800D37CC();
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x1B4) = r0;
    f0 = *(f64*)(sp + 0x1B0);
    f29 = f0 - f28;
    fn_800D3088();
    f0 = *(f64*)(sp + 0x1B8);
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_80032E5C: ;
    if (f27 < f31) goto L_80032E24;
    if ((s32)r15 == (s32)0x3) goto L_80032E88;
    if ((s32)r15 >= (s32)0x3) goto L_80032E94;
    if ((s32)r15 >= (s32)0x2) goto L_80032E7C;
    goto L_80032E94;
L_80032E7C: ;
    r0 = 0xd;
    *(u32*)lbl_8047A458 = r0;
    goto L_80032E9C;
L_80032E88: ;
    r0 = 0xc;
    *(u32*)lbl_8047A458 = r0;
    goto L_80032E9C;
L_80032E94: ;
    r0 = 0xc;
    *(u32*)lbl_8047A458 = r0;
L_80032E9C: ;
    /* psq_l f31, 0x2b8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x2B0);
    /* psq_l f30, 0x2a8(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x2A0);
    /* psq_l f29, 0x298(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x290);
    /* psq_l f28, 0x288(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0x280);
    /* psq_l f27, 0x278(r1), 0, qr0 */;
    f27 = *(f64*)(sp + 0x270);
    /* lmw r14, 0x228(r1) */;
    return;
}
#pragma pop

/* 0x80032ED8 | 0x1E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80032ED8(void) {
    extern u8 lbl_803A3334[];
    extern u8 lbl_8047A439[];
    extern u8 lbl_8047A444[];
    extern void fn_800330B8();
    extern void fn_800896D0();
    extern void fn_800896D8();
    extern void fn_800896E0();
    extern void fn_80113F48();
    extern void fn_801653C4();
    extern void fn_80165A20();
    extern void fn_80176E0C();
    extern void fn_801CA5C4();
    extern void fn_801FC794();
    extern void fn_801FCA2C();
    extern void fn_801FCAFC();
    extern void fn_801FCB40();
    extern void fn_801FCB84();
    extern void fn_801FCB94();
    extern void fn_801FCC3C();
    extern void fn_801FCC54();
    extern void fn_801FCCC4();
    extern void fn_8020DF90();
    extern void fn_8020DFA0();
    extern void fn_8020E0F8();
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

    /* stmw r24, 0x10(r1) */;
    r29 = r3;
    r30 = r4;
    r31 = r5;
    r3 = r30;
    fn_8020E0F8();
    r0 = *(u32*)((u8*)r29 + 0x1C);
    r26 = r3;
    if ((s32)r0 != (s32)0x0) goto L_80032F18;
    r4 = 0x0;
    fn_8020DFA0();
    goto L_80032F20;
L_80032F18: ;
    r4 = 0x1;
    fn_8020DFA0();
L_80032F20: ;
    r0 = *(u32*)lbl_8047A444;
    r3 = r26;
    r4 = r0 & 0xFFFF;
    fn_8020DF90();
    r3 = 0x9;
    fn_801FCCC4();
    r4 = *(u8*)((u8*)r29 + 0xC);
    r27 = r3;
    fn_801FCB94();
    r3 = r27;
    r4 = 0x26;
    fn_801FCC54();
    r4 = *(u16*)((u8*)r29 + 0x20);
    r3 = r27;
    fn_801FCB84();
    r4 = *(u8*)((u8*)r29 + 0x24);
    r3 = r27;
    fn_801FCAFC();
    r3 = r29;
    fn_800896E0();
    r3 = 0x0;
    fn_800896D8();
    r3 = 0x9;
    fn_800896D0();
    r28 = r29;
    r26 = 0x0;
L_80032F88: ;
    r5 = *(u16*)((u8*)r28 + 0x12);
    r3 = r27;
    r4 = r26 & 0xFF;
    fn_801FCB40();
    r28 = r28 + 0x2;
    r26 = r26 + 0x1;
    if ((s32)r26 < (s32)0x4) goto L_80032F88;
    r3 = r27;
    fn_801FCC3C();
    fn_801FCA2C();
    r25 = r3;
    r27 = r31;
    r24 = 0x0;
    r26 = r25;
    r3 = (u32)lbl_803A3334;
    r28 = (u32)lbl_803A3334;
L_80032FCC: ;
    if ((u32)r31 == (u32)0x0) goto L_80032FE4;
    r3 = r26;
    r4 = r27;
    fn_800330B8();
    goto L_80033004;
L_80032FE4: ;
    r0 = r24 + 0xd;
    r3 = r26;
    r0 = *(u8*)(r29 + r0);
    r0 = (s8)r0;
    r0 = r0 * 0x2a;
    r4 = r28 + r0;
    r4 = r4 + 0x514;
    fn_800330B8();
L_80033004: ;
    r27 = r27 + 0x2a;
    r26 = r26 + 0x50;
    r24 = r24 + 0x1;
    if ((s32)r24 < (s32)0x4) goto L_80032FCC;
    r0 = r24 * 0x50;
    r26 = r25 + r0;
    goto L_80033038;
L_80033024: ;
    r3 = r26;
    r4 = 0x0;
    fn_801FC794();
    r26 = r26 + 0x50;
    r24 = r24 + 0x1;
L_80033038: ;
    if ((s32)r24 < (s32)0x6) goto L_80033024;
    r3 = r30;
    r4 = 0x1;
    r5 = 0x0;
    fn_801CA5C4();
    r26 = r3;
    fn_80113F48();
    r4 = (0x1117 << 16);
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    fn_80176E0C();
    r0 = *(u8*)lbl_8047A439;
    if ((u32)r0 == (u32)0x0) goto L_80033080;
    r28 = 0x446;
    goto L_80033084;
L_80033080: ;
    r28 = 0x4cd;
L_80033084: ;
    fn_801653C4();
    if ((u32)r28 == (u32)r3) goto L_800330A0;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x7f;
    fn_80165A20();
L_800330A0: ;
    r3 = r26;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x800330B8 | 0x1C0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800330B8(void) {
    extern void fn_801EEE6C();
    extern void fn_801FC684();
    extern void fn_801FC694();
    extern void fn_801FC6D4();
    extern void fn_801FC6E4();
    extern void fn_801FC6F4();
    extern void fn_801FC744();
    extern void fn_801FC784();
    extern void fn_801FC794();
    extern void fn_801FC7A4();
    extern void fn_801FC7B4();
    extern void fn_801FC7D4();
    extern void fn_801FC7E4();
    extern void fn_801FC808();
    extern void fn_801FC828();
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

    r28 = r3;
    r29 = r4;
    r4 = *(u16*)((u8*)r29 + 0x0);
    fn_801FC794();
    r3 = r28;
    r4 = 0x0;
    fn_801FC828();
    r4 = *(u8*)((u8*)r29 + 0x2);
    r3 = r28;
    fn_801FC7B4();
    r3 = *(u8*)((u8*)r29 + 0x2);
    if ((u32)r3 == (u32)0x0) goto L_80033110;
    r4 = *(u8*)((u8*)r29 + 0x28);
    fn_801EEE6C();
L_80033110: ;
    r4 = *(u8*)((u8*)r29 + 0x3);
    r3 = r28;
    fn_801FC7D4();
    r31 = r29;
    r30 = 0x0;
L_80033124: ;
    r5 = *(u16*)((u8*)r31 + 0x4);
    r3 = r28;
    r4 = r30 & 0xFF;
    fn_801FC744();
    r31 = r31 + 0x2;
    r30 = r30 + 0x1;
    if ((s32)r30 < (s32)0x4) goto L_80033124;
    r4 = *(u16*)((u8*)r29 + 0xC);
    r3 = r28;
    fn_801FC784();
    r4 = *(u8*)((u8*)r29 + 0xE);
    r3 = r28;
    fn_801FC7A4();
    r5 = *(u8*)((u8*)r29 + 0xF);
    r3 = r28;
    r4 = 0x0;
    fn_801FC808();
    r5 = *(u8*)((u8*)r29 + 0x10);
    r3 = r28;
    r4 = 0x1;
    fn_801FC808();
    r5 = *(u8*)((u8*)r29 + 0x11);
    r3 = r28;
    r4 = 0x2;
    fn_801FC808();
    r5 = *(u8*)((u8*)r29 + 0x12);
    r3 = r28;
    r4 = 0x3;
    fn_801FC808();
    r5 = *(u8*)((u8*)r29 + 0x13);
    r3 = r28;
    r4 = 0x4;
    fn_801FC808();
    r5 = *(u8*)((u8*)r29 + 0x14);
    r3 = r28;
    r4 = 0x5;
    fn_801FC808();
    r5 = *(s16*)((u8*)r29 + 0x16);
    r3 = r28;
    r4 = 0x0;
    fn_801FC7E4();
    r5 = *(s16*)((u8*)r29 + 0x18);
    r3 = r28;
    r4 = 0x1;
    fn_801FC7E4();
    r5 = *(s16*)((u8*)r29 + 0x1A);
    r3 = r28;
    r4 = 0x2;
    fn_801FC7E4();
    r5 = *(s16*)((u8*)r29 + 0x1C);
    r3 = r28;
    r4 = 0x3;
    fn_801FC7E4();
    r5 = *(s16*)((u8*)r29 + 0x1E);
    r3 = r28;
    r4 = 0x4;
    fn_801FC7E4();
    r5 = *(s16*)((u8*)r29 + 0x20);
    r3 = r28;
    r4 = 0x5;
    fn_801FC7E4();
    r4 = *(s16*)((u8*)r29 + 0x22);
    r3 = r28;
    fn_801FC6F4();
    r4 = *(u8*)((u8*)r29 + 0x24);
    r3 = r28;
    fn_801FC6E4();
    r4 = *(u8*)((u8*)r29 + 0x25);
    r3 = r28;
    fn_801FC6D4();
    r4 = *(u8*)((u8*)r29 + 0x26);
    r3 = r28;
    fn_801FC694();
    r4 = *(u8*)((u8*)r29 + 0x27);
    r3 = r28;
    fn_801FC684();
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop
