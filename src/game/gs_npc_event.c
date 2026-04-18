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
extern void  fn_800FB680(s32, s32, u32, u16);

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
extern u8 lbl_803A2688[];
#if 0
asm void fn_80031188(void) {
#include "src/game/gs_npc_event_fn_80031188.inc"
}
#else
#pragma peephole off
void fn_80031188(u8* r3, u8* r4) {
    u32 flags = *(u32*)(r4 + 0x64);
    s32 evtype = *(s16*)(r4 + 0x6);
    u32 combined = (flags & 0xa1400000) | *(u8*)(r3 + 0x8b);
    switch (evtype) {
    case 0xF6B:
        fn_800FB680(0, 0, combined, 0x4412);
        break;
    case 0xF6A:
        {
            s32 val = fn_8012A5B0((void*)lbl_803A2688, 1, 0);
            fn_80132A38(0x4D, val);
            fn_800FB680(0, 0, combined, 0x4413);
        }
        break;
    }
}
#pragma peephole on
#endif

/* 0x80031228 | 0x1DC */
extern void fn_8011E850(void);
extern void fn_8010B718(void);
extern void fn_80109220(void*, s32);
extern void fn_80123FBC(void);
extern void fn_80075FEC(void);
extern u8 lbl_80266E90[];
#if 1
asm void fn_80031228(void) {
#include "src/game/gs_npc_event_fn_80031228.inc"
}
#else
void fn_80031228(void) {
    extern u8 lbl_80266E90[];
    extern u8 lbl_803A2688[];
    extern void fn_80075FEC();
    extern void fn_80109220();
    extern void fn_8010B718();
    extern void fn_8011E850();
    extern void fn_80123FBC();
    u8 sp[0x20];
    u32 tmp = 0;
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
    tmp = 0x2;
    ctr_fn = (void(*)(void))tmp;
    do {
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0x2);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0x2);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0x2);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0x2);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0x2);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0x2);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r5 = r5 + 0x5;
    } while (--ctr != 0);
    if ((s32)r6 != 2) {
        if ((s32)r6 >= 2) goto L_80031360;
        if ((s32)r6 < 1) {
            goto L_80031360;
        }
        r5 = r7 & 0xFFFF;
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_8012A5B0)();
        r29 = r3;

    } else {
        r3 = (u32)lbl_803A2688;
        r5 = r7 & 0xFFFF;
        r3 = (u32)lbl_803A2688;
        r4 = 0x3;
        ((void(*)(void))fn_8012A5B0)();
        r29 = r3;
    }
L_80031360:
    r3 = r29;
    fn_8011E850();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r3 = r30;
        r4 = r31;
        r5 = r29;
        fn_8010B718();
        r3 = r31;
        r4 = 0x1;
        fn_80109220();
        return;
    }
    r3 = r29;
    fn_80123FBC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r3 = r29;
        fn_80075FEC();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            r3 = r30;
            r4 = r31;
            r5 = r29;
            fn_8010B718();
            r3 = r31;
            r4 = 0x1;
            fn_80109220();
            return;
    }
    }
    r3 = r31;
    r4 = 0x0;
    fn_80109220();

    return;
}
#endif

/* 0x80031404 | 0x244 */
extern void fn_8001DA60(void);
extern void fn_8011E8DC(void);
extern void fn_800FA280(void);
#if 1
asm void fn_80031404(void) {
#include "src/game/gs_npc_event_fn_80031404.inc"
}
#else
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
    u32 tmp = 0;
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
    tmp = *(u8*)((u8*)r3 + 0x8B);
    r4 = (u32)lbl_80266E90;
    /* clrrwi r3, r5, 8 */;
    r29 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r31 = r3 | tmp;
    r5 = 0x0;
    tmp = 0x2;
    ctr_fn = (void(*)(void))tmp;
    do {
        r3 = *(s16*)((u8*)r30 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xC);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r30 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xC);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r30 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xC);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r30 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xC);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r30 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xC);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r30 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xC);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r5 = r5 + 0x5;
    } while (--ctr != 0);
    if ((s32)r6 != 2) {
        if ((s32)r6 >= 2) goto L_8003154C;
        if ((s32)r6 < 1) {
            goto L_8003154C;
        }
        r5 = r7 & 0xFFFF;
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_8012A5B0)();
        r29 = r3;

    } else {
        r3 = (u32)lbl_803A2688;
        r5 = r7 & 0xFFFF;
        r3 = (u32)lbl_803A2688;
        r4 = 0x3;
        ((void(*)(void))fn_8012A5B0)();
        r29 = r3;
    }
L_8003154C:
    r3 = r29;
    fn_80123FBC();
    tmp = r3 & 0xFF;
    do {
        if (tmp == 0) break;
        r3 = r29;
        fn_80075FEC();
        tmp = r3 & 0xFF;
        if (tmp != 1) break;
        r3 = r29;
        fn_8001DA60();
        tmp = r3 & 0xFF;
        do {
            if ((s32)tmp != 1) {
                if ((s32)tmp < 1) {
                    if ((s32)tmp < 0) {
                        continue;
                    }
                    continue;
                    }
                r28 = 0xd67;
                break;
            }
            r28 = 0xd68;
            break;


        } while (0);

        r3 = r29;
        fn_8011E8DC();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            r28 = 0x0;
        }
        if (r28 != 0) {
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
            return;
        }
        r3 = r30;
        r4 = 0x0;
        fn_80109220();
        return;
    } while (0);

    r3 = r30;
    r4 = 0x0;
    fn_80109220();

    return;
}
#endif

/* 0x80031648 | 0x290 */
extern void fn_800FA444(void);
extern void fn_8011F4F0(void);
#if 1
asm void fn_80031648(void) {
#include "src/game/gs_npc_event_fn_80031648.inc"
}
#else
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
    u32 tmp = 0;
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
    tmp = 0x2;
    ctr_fn = (void(*)(void))tmp;
    do {
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xA);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xA);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xA);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xA);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xA);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r3 = *(s16*)((u8*)r31 + 0x6);
        tmp = *(u16*)((u8*)r4 + 0xA);
        if ((s32)r3 == (s32)tmp) {
            r6 = *(u8*)((u8*)r4 + 0x0);
            r7 = *(u8*)((u8*)r4 + 0x1);
        }
        r4 = r4 + 0x12;
        r5 = r5 + 0x5;
    } while (--ctr != 0);
    if ((s32)r6 != 2) {
        if ((s32)r6 >= 2) goto L_80031778;
        if ((s32)r6 < 1) {
            goto L_80031778;
        }
        r5 = r7 & 0xFFFF;
        r3 = 0x0;
        r4 = 0x3;
        ((void(*)(void))fn_8012A5B0)();
        r30 = r3;

    } else {
        r3 = (u32)lbl_803A2688;
        r5 = r7 & 0xFFFF;
        r3 = (u32)lbl_803A2688;
        r4 = 0x3;
        ((void(*)(void))fn_8012A5B0)();
        r30 = r3;
    }
L_80031778:
    r3 = r30;
    fn_8011E850();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r3 = 0x56c;
        fn_800FA280();
        r4 = r3;
        r3 = 0x37;
        ((void(*)(void))fn_80132A38)();
        r3 = 0xe7;
        fn_800FA444();
        tmp = (u32)r3 >> 16;
        r3 = *(s16*)((u8*)r31 + 0x54);
        r6 = (s16)tmp;
        r4 = 0x0;
        r5 = (u32)r6 >> 31;
        tmp = (u32)r3 >> 31;
        r6 = r5 + r6;
        r7 = (s32)r6 >> 1;
        tmp = tmp + r3;
        tmp = (s32)tmp >> 1;
        r5 = -0x1;
        tmp = tmp - r7;
        r6 = 0xe7;
        r3 = (s16)tmp;
        ((void(*)(void))fn_800FB680)();
        r3 = r31;
        r4 = 0x1;
        fn_80109220();
        return;
    }
    r3 = r30;
    fn_80123FBC();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r3 = r30;
        fn_80075FEC();
        tmp = r3 & 0xFF;
        if (tmp == 1) {
            r3 = r30;
            fn_8011E8DC();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                r3 = 0x56b;
                fn_800FA280();
                r4 = r3;
                r3 = 0x37;
                ((void(*)(void))fn_80132A38)();
            } else {

                r3 = r30;
                fn_8011F4F0();
                tmp = r3;
                r3 = 0x37;
                r4 = tmp;
                ((void(*)(void))fn_80132A38)();
            }
            r3 = 0xe7;
            fn_800FA444();
            tmp = (u32)r3 >> 16;
            r3 = *(s16*)((u8*)r31 + 0x54);
            r6 = (s16)tmp;
            r4 = 0x0;
            r5 = (u32)r6 >> 31;
            tmp = (u32)r3 >> 31;
            r6 = r5 + r6;
            r7 = (s32)r6 >> 1;
            tmp = tmp + r3;
            tmp = (s32)tmp >> 1;
            r5 = -0x1;
            tmp = tmp - r7;
            r6 = 0xe7;
            r3 = (s16)tmp;
            ((void(*)(void))fn_800FB680)();
            r3 = r31;
            r4 = 0x1;
            fn_80109220();
            return;
    }
    }
    r3 = r31;
    r4 = 0x0;
    fn_80109220();

    return;
}
#endif

/* 0x800318D8 | 0x144 */
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
#if 1
asm void fn_800318D8(void) {
#include "src/game/gs_npc_event_fn_800318D8.inc"
}
#else
void fn_800318D8(void) {
    extern u8 lbl_80266E90[];
    extern u32 lbl_8047A420;
    extern u32 lbl_8047A424;
    extern void fn_80109220();
    u8 sp[0x10];
    u32 tmp = 0;
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
    tmp = 0x2;
    ctr_fn = (void(*)(void))tmp;
    do {
        r3 = *(s16*)((u8*)r4 + 0x6);
        tmp = *(u16*)((u8*)r5 + 0xE);
        if ((s32)r3 == (s32)tmp) {
            r7 = *(u8*)((u8*)r5 + 0x0);
            r8 = *(u8*)((u8*)r5 + 0x1);
        }
        r5 = r5 + 0x12;
        r3 = *(s16*)((u8*)r4 + 0x6);
        tmp = *(u16*)((u8*)r5 + 0xE);
        if ((s32)r3 == (s32)tmp) {
            r7 = *(u8*)((u8*)r5 + 0x0);
            r8 = *(u8*)((u8*)r5 + 0x1);
        }
        r5 = r5 + 0x12;
        r3 = *(s16*)((u8*)r4 + 0x6);
        tmp = *(u16*)((u8*)r5 + 0xE);
        if ((s32)r3 == (s32)tmp) {
            r7 = *(u8*)((u8*)r5 + 0x0);
            r8 = *(u8*)((u8*)r5 + 0x1);
        }
        r5 = r5 + 0x12;
        r3 = *(s16*)((u8*)r4 + 0x6);
        tmp = *(u16*)((u8*)r5 + 0xE);
        if ((s32)r3 == (s32)tmp) {
            r7 = *(u8*)((u8*)r5 + 0x0);
            r8 = *(u8*)((u8*)r5 + 0x1);
        }
        r5 = r5 + 0x12;
        r3 = *(s16*)((u8*)r4 + 0x6);
        tmp = *(u16*)((u8*)r5 + 0xE);
        if ((s32)r3 == (s32)tmp) {
            r7 = *(u8*)((u8*)r5 + 0x0);
            r8 = *(u8*)((u8*)r5 + 0x1);
        }
        r5 = r5 + 0x12;
        r3 = *(s16*)((u8*)r4 + 0x6);
        tmp = *(u16*)((u8*)r5 + 0xE);
        if ((s32)r3 == (s32)tmp) {
            r7 = *(u8*)((u8*)r5 + 0x0);
            r8 = *(u8*)((u8*)r5 + 0x1);
        }
        r5 = r5 + 0x12;
        r6 = r6 + 0x5;
    } while (--ctr != 0);
    if ((s32)r7 != 2) {
        if ((s32)r7 >= 2) goto L_80031A00;
        if ((s32)r7 < 1) {
            goto L_80031A00;
        }
        tmp = lbl_8047A424;
        if ((s32)tmp == (s32)r8) {
            r3 = r4;
            r4 = 0x1;
            fn_80109220();
            return;
        }
        tmp = lbl_8047A420;
        if ((s32)tmp == (s32)r8) {
            r3 = r4;
            r4 = 0x1;
            fn_80109220();
            return;
        }
    }
L_80031A00:
    r3 = r4;
    r4 = 0x0;
    fn_80109220();

    return;
}
#endif

/* 0x54 | fn_80031A1C | generic */
/* fn_80031A1C - 0x80031A1C | size: 0x54 */
extern u32 lbl_8047A42C;
#if 0
asm void fn_80031A1C(void) {
#include "src/game/gs_npc_event_fn_80031A1C.inc"
}
#else
#pragma peephole off
void fn_80031A1C(void* r3, void* r4) {
    s32 val = (s32)lbl_8047A42C;
    switch (val) {
    case 7:
    case 2:
        fn_80109220(r4, 1);
        break;
    default:
        fn_80109220(r4, 0);
        break;
    }
}
#pragma peephole on
#endif

/* 0x50 | fn_80031A70 | generic */
/* fn_80031A70 - 0x80031A70 | size: 0x50 */
extern u32 lbl_8047A42C;
#if 0
asm void fn_80031A70(void) {
#include "src/game/gs_npc_event_fn_80031A70.inc"
}
#else
#pragma peephole off
void fn_80031A70(void* r3, void* r4) {
    s32 val = (s32)lbl_8047A42C;
    switch (val) {
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        fn_80109220(r4, 1);
        break;
    default:
        fn_80109220(r4, 0);
        break;
    }
}
#pragma peephole on
#endif

/* 0x50 | fn_80031AC0 | generic */
/* fn_80031AC0 - 0x80031AC0 | size: 0x50 */
extern u32 lbl_8047A42C;
#if 0
asm void fn_80031AC0(void) {
#include "src/game/gs_npc_event_fn_80031AC0.inc"
}
#else
#pragma peephole off
void fn_80031AC0(void* r3, void* r4) {
    s32 val = (s32)lbl_8047A42C;
    switch (val) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        fn_80109220(r4, 1);
        break;
    default:
        fn_80109220(r4, 0);
        break;
    }
}
#pragma peephole on
#endif

/* 0x80031B10 | 0x3C */
extern void fn_800F0308(void);
extern u32 fn_8010B560(void);
extern void fn_801CB9D8(u32);
extern u32 lbl_8047A418;
#if 0
asm void fn_80031B10(void) {
#include "src/game/gs_npc_event_fn_80031B10.inc"
}
#else
#pragma peephole off
void fn_80031B10(void) {
    while ((u8)fn_8010B560() != 0) {
        fn_800F0308();
    }
    fn_801CB9D8(lbl_8047A418);
}
#pragma peephole on
#endif

/* 0x80031B4C | 0x954 */
extern void fn_801D036C(void);
extern void fn_800849B4(void);
extern void fn_8002FC58(void);
extern void fn_801021F8(void);
extern void fn_801046B8(void);
extern void fn_801026A4(void);
extern void fn_80104704(void);
extern void fn_801046C8(void);
extern void fn_801045A8(void);
extern void fn_801043A4(void);
extern void fn_801023E4(void);
extern void fn_80102510(void);
extern void fn_801C41C8(void);
extern void fn_801C40F0(void);
extern void fn_800E4014(void);
extern void fn_80102254(void);
extern void fn_80097CD0(void);
extern void fn_8002F79C(void);
extern void fn_8002F284(void);
extern void fn_8002EE74(void);
extern void fn_80103CC0(void);
extern void fn_80102004(void);
extern void fn_80106D3C(void);
extern void fn_8002EA5C(void);
extern void fn_8002E460(void);
extern void fn_8010264C(void);
extern void fn_8010A5BC(void);
extern void fn_80109C88(void);
extern void fn_801069FC(void);
extern void fn_8002E26C(void);
extern void fn_8010A420(void);
extern void fn_8011D904(void);
extern void fn_8025FF9C(void);
extern void fn_8002DF10(void);
extern void fn_8002DD24(void);
extern void fn_8001E074(void);
extern void fn_801D0314(void);
extern void fn_801024E8(void);
extern void fn_800FF52C(void);
extern void fn_800FF660(void);
extern void fn_8011394C(void);
extern void fn_80113828(void);
extern void jumptable_802E4F90();
extern u8 lbl_8047A409;
extern u8 lbl_803A2518[];
extern u8 lbl_804788B0;
extern u8 lbl_80266F90[];
extern u8 lbl_8047A40A;
extern u32 lbl_8047A42C;
extern f32 lbl_8047B9D0;
extern u32 lbl_8047A414;
extern f32 lbl_8047B9D4;
extern u32 lbl_8047A428;
extern u8 lbl_8047A410;
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
#if 1
asm void fn_80031B4C(void) {
#include "src/game/gs_npc_event_fn_80031B4C.inc"
}
#else
void fn_80031B4C(void) {
    extern u8 lbl_80266F90[];
    extern u8 lbl_803A2518[];
    extern u8 lbl_804788B0;
    extern u8 lbl_8047A409;
    extern u8 lbl_8047A40A;
    extern u8 lbl_8047A410;
    extern u32 lbl_8047A414;
    extern u32 lbl_8047A420;
    extern u32 lbl_8047A424;
    extern u32 lbl_8047A428;
    extern u32 lbl_8047A42C;
    extern f32 lbl_8047B9D0;
    extern f32 lbl_8047B9D4;
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
    u32 tmp = 0;
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

    tmp = lbl_8047A409;
    r3 = 0x1;
    r4 = (u32)lbl_803A2518;
    lbl_804788B0 = r3;
    r31 = (u32)lbl_803A2518;
    if (tmp != 1) {
        fn_801D036C();
        r30 = r3;
        goto L_800323E4;
    do {
            if (tmp <= 0x13) {
                r3 = (u32)jumptable_802E4F90;
                tmp = tmp << 2;
                r3 = (u32)jumptable_802E4F90;
                tmp = *(u32*)(r3 + tmp);
                ctr_fn = (void(*)(void))tmp;
                r3 = (u32)lbl_80266F90;
                r5 = (u32)sp + 0x10;
                r10 = (u32)lbl_80266F90;
                r3 = 0x0;
                r9 = *(u32*)((u8*)r10 + 0x0);
                r4 = 0x1;
                r8 = *(u32*)((u8*)r10 + 0x4);
                r6 = 0x0;
                r7 = *(u32*)((u8*)r10 + 0x8);
                tmp = *(u32*)((u8*)r10 + 0xC);
                *(u32*)(sp + 0x1C) = tmp;
                fn_800849B4();
                if ((s32)r3 < 0) {
                    tmp = 0x0;
                    lbl_8047A40A = tmp;
                    lbl_8047A42C = tmp;
                    goto L_800323E4;
                }
                tmp = 0x2;
                lbl_8047A42C = tmp;
                goto L_800323E4;


                *(u32*)(sp + 0xC) = tmp;
                fn_801046B8();
                r4 = r3;
                r5 = (u32)sp + 0xc;
                r3 = 0xe3;
                r6 = 0x0;
                r7 = 0x0;
                r8 = 0x0;
                fn_801026A4();
                r3 = 0xe3;
                fn_80104704();
                r29 = r3;
                r4 = 0x102a;
                fn_801046C8();
                r29 = r3;
                if ((r29 != 0) && (r29 != 0)) {

                    r4 = 0x1;
                    fn_80109220();
                    tmp = 0x43e4;
                    *(u32*)((u8*)r29 + 0x4C) = tmp;
                }
                r3 = 0xe3;
                fn_80104704();
                r29 = r3;
                r4 = 0x1029;
                fn_801046C8();
                r29 = r3;
                if ((r29 != 0) && (r29 != 0)) {

                    r4 = 0x1;
                    fn_80109220();
                    tmp = 0x43e5;
                    *(u32*)((u8*)r29 + 0x4C) = tmp;
                }
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
                if ((s32)r29 == (s32)-0x1) {
                    r26 = -0x1;
                }
                if ((s32)r26 != 0) {
                    if ((s32)r26 < 0) {
                        if ((s32)r26 < (s32)-0x1) {
                            goto L_800323E4;
                        }
                        if ((s32)r26 < 2) {
                            tmp = 0x5;
                            lbl_8047A42C = tmp;
                        }
                        goto L_800323E4;
                    }
                    tmp = 0x6;
                    lbl_8047A42C = tmp;
                    goto L_800323E4;
                        }
                tmp = 0x2;
                lbl_8047A42C = tmp;
                goto L_800323E4;


                if ((r29 != 0) && (r29 != 0)) {

                    r4 = 0x1;
                    fn_80109220();
                    tmp = 0x43db;
                    *(u32*)((u8*)r29 + 0x4C) = tmp;
                }
                tmp = 0x1;
                *(u32*)(sp + 0x8) = tmp;
                fn_801046B8();
                r4 = r3;
                r5 = (u32)sp + 0x8;
                r3 = 0xe3;
                r6 = 0x0;
                r7 = 0x0;
                r8 = 0x0;
                fn_801026A4();
                r3 = 0xe3;
                fn_80104704();
                r29 = r3;
                r4 = 0x102a;
                fn_801046C8();
                r29 = r3;
                if ((r29 != 0) && (r29 != 0)) {

                    r4 = 0x1;
                    fn_80109220();
                    tmp = 0x43d4;
                    *(u32*)((u8*)r29 + 0x4C) = tmp;
                }
                r3 = 0xe3;
                fn_80104704();
                r29 = r3;
                r4 = 0x1029;
                fn_801046C8();
                r29 = r3;
                if ((r29 != 0) && (r29 != 0)) {

                    r4 = 0x1;
                    fn_80109220();
                    tmp = 0x43d5;
                    *(u32*)((u8*)r29 + 0x4C) = tmp;
                }
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
                if ((s32)r29 == (s32)-0x1) {
                    r26 = -0x1;
                }
                r3 = 0xd9;
                fn_80104704();
                r29 = r3;
                r4 = 0x10b2;
                fn_801046C8();
                if ((r29 != 0) && (r3 != 0)) {

                    tmp = 0x0;
                    r4 = 0x0;
                    *(u32*)((u8*)r3 + 0x4C) = tmp;
                    fn_80109220();
                }
                if ((s32)r26 != 0) {
                    if ((s32)r26 < 0) {
                        if ((s32)r26 < (s32)-0x1) {
                            goto L_800323E4;
                        }
                        if ((s32)r26 < 2) {

                        } else {
                            tmp = 0x2;
                            lbl_8047A42C = tmp;
                        }
                        goto L_800323E4;
                    }
                    tmp = 0x0;
                    lbl_8047A42C = tmp;
                    goto L_800323E4;
                        }
                tmp = 0x2;
                lbl_8047A42C = tmp;
                goto L_800323E4;


                lbl_8047A42C = tmp;
                goto L_800323E4;


                lbl_8047A410 = r4;
                lbl_8047A424 = r3;
                lbl_8047A42C = tmp;
                goto L_800323E4;


                if ((s32)r3 == 1) {
                    r3 = 0x2;
                    r4 = 0x4448;
                    r5 = 0x1;
                    r6 = 0x0;
                    fn_80106D3C();
                    r3 = 0xd9;
                    fn_80102510();
                    tmp = 0x0;
                    lbl_8047A42C = tmp;
                    goto L_800323E4;
                }
                tmp = 0x7;
                lbl_8047A42C = tmp;
                goto L_800323E4;


                lbl_8047A42C = tmp;
                goto L_800323E4;


                lbl_8047A42C = tmp;
                goto L_800323E4;


                lbl_8047A42C = tmp;
                goto L_800323E4;


                if ((s32)tmp != 0) {
                    if ((s32)tmp < 0) {
                        if ((s32)tmp < (s32)-0x1) {
                            goto L_800323E4;
                        }
                        if ((s32)tmp < 2) {

                        } else {
                            r3 = 0xde;
                            fn_80102510();
                            tmp = -0x1;
                            r3 = r31 + 0xd18;
                            lbl_8047A428 = tmp;
                            lbl_8047A424 = tmp;
                            lbl_8047A420 = tmp;
                            fn_8010A420();
                            r3 = r31 + 0xcd0;
                            fn_8010A420();
                            r3 = 0x1;
                            tmp = 0x2;
                            lbl_8047A410 = r3;
                            lbl_8047A42C = tmp;
                        }
                        goto L_800323E4;
                    }
                    r3 = 0xde;
                    fn_80102510();
                    r3 = r31 + 0xd18;
                    fn_8010A420();
                    r3 = r31 + 0xcd0;
                    fn_8010A420();
                    tmp = 0x0;
                    lbl_8047A42C = tmp;
                    goto L_800323E4;
                        }
                r3 = 0xde;
                fn_80102510();
                r3 = r31 + 0xd18;
                fn_8010A420();
                r3 = r31 + 0xcd0;
                fn_8010A420();
                tmp = 0x0;
                lbl_8047A42C = tmp;
            }
        L_800323E4:
            tmp = lbl_8047A42C;
    } while ((s32)tmp > 0);
        r3 = r30;
        fn_801D0314();
        tmp = lbl_8047A40A;
        if (tmp != 0) {
            tmp = lbl_804788B0;
            if (tmp != 0) {
                r3 = 0x2;
                r4 = 0x44d0;
                r5 = 0x1;
                r6 = 0x0;
                fn_80106D3C();
                r3 = 0x1;
                fn_801069FC();
                goto L_80032458;
            }
            r3 = 0x2;
            r4 = 0x44f1;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            r3 = 0x1;
            fn_801069FC();

        } else {
            tmp = 0x1;
            lbl_8047A40A = tmp;
        }
    L_80032458:
        r3 = 0x1;
        fn_801024E8();
        fn_800FF52C();
        tmp = r3 & 0xFF;
        if (tmp != 0) {
            fn_800FF660();
            goto L_80032484;
        }
        fn_8011394C();
        r4 = 0x0;
        fn_80113828();
    }
L_80032484:
    tmp = 0x1;
    lbl_8047A409 = tmp;
    return;
}
#endif

/* 0x800324A0 | 0xC4 */
extern void fn_80112260(s32);
extern u32  fn_80113F48(void);
extern u32  fn_801CBA0C(u32);
extern u32  fn_800F9318(u32);
extern void fn_80176E0C(s32, u32, s32, s32);
extern void fn_80177A44(s32);
extern void fn_801CB7C4(u32);
extern u8 lbl_8047A408;
extern u32 lbl_8047A42C;
extern u8 lbl_8047A409;
extern u32 lbl_8047A428;
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
extern u8 lbl_8047A410;
extern u8 lbl_8047A41C;
extern u8 lbl_8047A40A;
extern u32 lbl_8047A418;
extern u32 lbl_8047A414;
#if 0
asm void fn_800324A0(void) {
#include "src/game/gs_npc_event_fn_800324A0.inc"
}
#else
#pragma push
#pragma peephole off
void fn_800324A0(void) {
    u32 value;
    u32 handle;
    s32 minus_one;
    s32 zero;
    s32 one;

    fn_80112260(0);
    if (lbl_8047A408 != 0) {
        lbl_8047A408 = 0;
    } else {
        one = 1;
        minus_one = -1;
        zero = 0;
        lbl_8047A42C = one;
        lbl_8047A409 = zero;
        lbl_8047A428 = minus_one;
        lbl_8047A424 = minus_one;
        lbl_8047A420 = minus_one;
        lbl_8047A410 = one;
        lbl_8047A41C = one;
        lbl_8047A40A = one;
        handle = fn_80113F48();
        value = fn_801CBA0C(0x0FFE1000);
        lbl_8047A418 = value;
        lbl_8047A414 = fn_800F9318(handle);
        fn_80176E0C(0x37C, 0x0FFF1800, 0, 1);
        fn_80177A44(4);
        fn_801CB7C4(0x10B11000);
    }
}
#pragma pop
#endif

/* 0x80032564 | 0x28 */
extern void fn_800FF730(s32);
#if 0
asm void fn_80032564(void) {
#include "src/game/gs_npc_event_fn_80032564.inc"
}
#else
#pragma peephole off
void fn_80032564(void) {
    fn_800FF730(0x394);
    fn_800F0308();
}
#pragma peephole on
#endif

/* 0x8003258C | 0x270 */
extern void fn_80166AB8(void);
extern void fn_801293FC(void);
extern void jumptable_802E4FE0();
extern u32 lbl_8047A450;
extern u32 lbl_8047A458;
extern u8 lbl_8047A44A;
extern u8 lbl_803A3278[];
extern u32 lbl_8047A43C;
#if 1
asm void fn_8003258C(void) {
#include "src/game/gs_npc_event_fn_8003258C.inc"
}
#else
void fn_8003258C(void) {
    extern u8 lbl_803A3278[];
    extern u32 lbl_8047A43C;
    extern u8 lbl_8047A44A;
    extern u32 lbl_8047A450;
    extern u32 lbl_8047A458;
    extern void fn_801069FC();
    extern void fn_80106D3C();
    extern void fn_801293FC();
    extern void fn_80166AB8();
    extern u8 jumptable_802E4FE0[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r3 = lbl_8047A450;
    tmp = -0x1;
    if (r3 < tmp) {
        tmp = r3 + 0x1;
        lbl_8047A450 = tmp;
    }
    r4 = lbl_8047A450;
    r3 = 0xAAAB0000;
    r3 = r4 + 0x1;
    tmp = (u32)((u64)tmp * (u64)r3 >> 32);
    tmp = (u32)tmp >> 2;
    tmp = tmp * 0x6;
    tmp = r3 - tmp;
    if (tmp == 0) {
        tmp = 0xe;
        lbl_8047A458 = tmp;
        return;
    }
    tmp = lbl_8047A44A;
    if (tmp != 0) {
    do {
        tmp = 0x0;
        lbl_8047A44A = tmp;
        if (r4 < 0x1e) {
            do {
                if (tmp <= 0x12) {
                    r3 = (u32)jumptable_802E4FE0;
                    tmp = tmp << 2;
                    r3 = (u32)jumptable_802E4FE0;
                    tmp = *(u32*)(r3 + tmp);
                    ctr_fn = (void(*)(void))tmp;
                    r31 = 0x258;
                    break;

                    break;

                    break;

                    break;
                }
                r31 = 0x258;
            } while (0);

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
            break;
        }
        if ((s32)r4 != 0x5a) {
            if ((s32)r4 < 0x5a) {
                if ((s32)r4 != 0x3c) {
                    if ((s32)r4 >= 0x3c) break;
                    if ((s32)r4 != 0x1e) {
                        break;
                    }
                    if ((s32)r4 != 0x96) {
                        if ((s32)r4 >= 0x96) break;
                        if ((s32)r4 != 0x78) {
                            break;
                            }
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
                        break;
                        }
                    r3 = 0x8;
                    r4 = 0x3b8f;
                    r5 = 0x1;
                    r6 = 0x0;
                    fn_80106D3C();
                    r3 = 0x1;
                    fn_801069FC();
                    break;
                }
                r3 = 0x8;
                r4 = 0x3b53;
                r5 = 0x1;
                r6 = 0x0;
                fn_80106D3C();
                r3 = 0x1;
                fn_801069FC();
                break;
                        }
            r3 = 0x8;
            r4 = 0x3b56;
            r5 = 0x1;
            r6 = 0x0;
            fn_80106D3C();
            r3 = 0x1;
            fn_801069FC();
            break;
                    }
        r3 = 0x8;
        r4 = 0x3b59;
        r5 = 0x1;
        r6 = 0x0;
        fn_80106D3C();
        r3 = 0x1;
        fn_801069FC();
    } while (0);
        r3 = (u32)lbl_803A3278;
        tmp = 0x0;
        r3 = (u32)lbl_803A3278;
        lbl_8047A43C = tmp;
        *(u8*)((u8*)r3 + 0x0) = tmp;
        *(u8*)((u8*)r3 + 0x1) = tmp;
        *(u8*)((u8*)r3 + 0x2) = tmp;
        *(u8*)((u8*)r3 + 0x3) = tmp;
        *(u8*)((u8*)r3 + 0x4) = tmp;
        *(u8*)((u8*)r3 + 0x5) = tmp;
        *(u8*)((u8*)r3 + 0x6) = tmp;
        *(u8*)((u8*)r3 + 0x7) = tmp;
        *(u8*)((u8*)r3 + 0x8) = tmp;
        *(u8*)((u8*)r3 + 0x9) = tmp;
        *(u8*)((u8*)r3 + 0xA) = tmp;
        *(u8*)((u8*)r3 + 0xB) = tmp;
        *(u8*)((u8*)r3 + 0xC) = tmp;
        *(u8*)((u8*)r3 + 0xD) = tmp;
        *(u8*)((u8*)r3 + 0xE) = tmp;
    }
    tmp = 0x5;
    lbl_8047A458 = tmp;

    return;
}
#endif

/* 0x800327FC | 0x6DC */
extern void fn_800E0C04(void);
extern void fn_800F9E70(void);
extern void fn_8012640C(void);
extern void fn_80082EA4(void);
extern void fn_80102620(void);
extern void fn_800D37CC(void);
extern void fn_800D3088(void);
extern u8 lbl_8047A44A;
extern u32 lbl_8047A430;
extern u32 lbl_8047A43C;
extern u8 lbl_803A3334[];
extern u32 lbl_8047A434;
extern u8 lbl_8047A448;
extern f32 lbl_8047BA00;
extern f32 lbl_8047B9F8;
extern u8 lbl_8047A449;
extern f64 lbl_8047BA08;
extern f64 lbl_8047BA10;
extern f32 lbl_8047BA18;
extern u32 lbl_8047A458;
extern void fn_80032ED8(void);
#if 1
asm void fn_800327FC(void) {
#include "src/game/gs_npc_event_fn_800327FC.inc"
}
#else
void fn_800327FC(void) {
    extern u8 lbl_803A3278[];
    extern u8 lbl_803A3334[];
    extern u32 lbl_8047A430;
    extern u32 lbl_8047A434;
    extern u32 lbl_8047A43C;
    extern u8 lbl_8047A448;
    extern u8 lbl_8047A449;
    extern u8 lbl_8047A44A;
    extern u32 lbl_8047A458;
    extern f32 lbl_8047B9F8;
    extern f32 lbl_8047BA00;
    extern f64 lbl_8047BA08;
    extern f64 lbl_8047BA10;
    extern f32 lbl_8047BA18;
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
    u32 tmp = 0;
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

    tmp = lbl_8047A44A;
    if (tmp != 0) {
        r4 = lbl_8047A430;
        r14 = 0x0;
        tmp = 0xa;
        ctr_fn = (void(*)(void))tmp;
        do {
            r3 = r4 + 0x41e4;
            do {
                if (r3 == 0) break;
                tmp = *(u16*)((u8*)r3 + 0x6);
                if (tmp == 0) {
                    tmp = *(u16*)((u8*)r3 + 0x8);
                    if (tmp == 0) break;
                }
                r3 = 0xa - r14;
                fn_800E0C04();
                r3 = r14 + r3;
                tmp = lbl_8047A430;
                r3 = r3 * 0xca;
                r3 = r3 + 0x41e4;
                r3 = tmp + r3;
                break;
            } while (0);

            r4 = r4 + 0xca;
            r14 = r14 + 0x1;
        } while (--ctr != 0);

        tmp = 0x19;
        r5 = (u32)sp + 0xe0;
        ctr_fn = (void(*)(void))tmp;
        do {
            r3 = *(u32*)((u8*)r4 + 0x4);
            tmp = *(u32*)((u8*)r4 + 0x8);
            *(u32*)((u8*)r5 + 0x4) = r3;
            r5 += 8; *(u32*)r5 = tmp;
        } while (--ctr != 0);
        tmp = *(u16*)((u8*)r4 + 0x4);
        r3 = (u32)sp + 0x14;
        r4 = (u32)sp + 0xea;
        *(u16*)((u8*)r5 + 0x4) = tmp;
        fn_800F9E70();
        tmp = 0x0;
        r15 = *(u16*)(sp + 0x158);
        *(u16*)(sp + 0x8E) = r3;
        r15 = *(u16*)(sp + 0x182);
        *(u16*)(sp + 0xB8) = r3;
        r15 = *(u16*)(sp + 0x1AC);
        r16 = *(u8*)(sp + 0xF6);
        r17 = *(u16*)(sp + 0xF8);
        r18 = *(u16*)(sp + 0xFA);
        r19 = *(u16*)(sp + 0xFC);
        r20 = *(u16*)(sp + 0xFE);
        r21 = *(u8*)(sp + 0x100);
        r22 = *(u16*)(sp + 0x102);
        r23 = *(u8*)(sp + 0x104);
        r10 = *(u16*)(sp + 0x12E);
        r3 = r15;
        *(u8*)(sp + 0x20) = r16;
        *(u8*)(sp + 0x21) = tmp;
        *(u8*)(sp + 0x22) = tmp;
        *(u8*)(sp + 0x23) = tmp;
        *(u8*)(sp + 0x24) = tmp;
        *(u16*)(sp + 0x26) = r17;
        *(u16*)(sp + 0x28) = r18;
        *(u16*)(sp + 0x2A) = r19;
        *(u16*)(sp + 0x2C) = r20;
        *(u16*)(sp + 0x34) = r22;
        *(u16*)(sp + 0x36) = tmp;
        *(u8*)(sp + 0x38) = r23;
        *(u16*)(sp + 0x64) = r10;
        *(u16*)(sp + 0xE2) = r3;
        r3 = (u32)lbl_803A3278;
        lbl_8047A43C = tmp;
        r6 = (u32)lbl_803A3278;
        r3 = (u32)sp + 0x14;
        *(u8*)((u8*)r6 + 0x0) = tmp;
        r5 = (u32)sp + 0x3c;
        r4 = 0x209;
        *(u8*)((u8*)r6 + 0x1) = tmp;
        *(u8*)((u8*)r6 + 0x2) = tmp;
        *(u8*)((u8*)r6 + 0x3) = tmp;
        *(u8*)((u8*)r6 + 0x4) = tmp;
        *(u8*)((u8*)r6 + 0x5) = tmp;
        *(u8*)((u8*)r6 + 0x6) = tmp;
        *(u8*)((u8*)r6 + 0x7) = tmp;
        *(u8*)((u8*)r6 + 0x8) = tmp;
        *(u8*)((u8*)r6 + 0x9) = tmp;
        *(u8*)((u8*)r6 + 0xA) = tmp;
        *(u8*)((u8*)r6 + 0xB) = tmp;
        *(u8*)((u8*)r6 + 0xC) = tmp;
        *(u8*)((u8*)r6 + 0xD) = tmp;
        *(u8*)((u8*)r6 + 0xE) = tmp;
        fn_80032ED8();
        r15 = r3;

    } else {
        r3 = (u32)lbl_803A3334;
        r16 = 0x0;
        r3 = (u32)lbl_803A3334;
        r15 = 0x1;
        r19 = *(u8*)((u8*)r3 + 0x24);
        r17 = r15;
        r18 = *(u8*)((u8*)r3 + 0x26);
        r20 = r16;
        while (1) {
            tmp = r20 & 0xFFFF;
            if (tmp >= 6) break;
            r5 = r20;
            r3 = 0x0;
            r4 = 0x3;
            ((void(*)(void))fn_8012A5B0)();
            r14 = r3;
            fn_80123FBC();
            tmp = r3 & 0xFF;
            if (tmp == 1) {
                r3 = r14;
                r4 = 0x0;
                r5 = 0x7a;
                r6 = 0x0;
                fn_8012640C();
                r3 = r3 & 0xFF;
                tmp = r17 & 0xFF;
                if (tmp < r3) {
                    r17 = r3;
            }
            }
            r20 = r20 + 0x1;

        }
        r3 = (u32)lbl_803A3334;
        r21 = (u32)sp + 0x8;
        r14 = (u32)lbl_803A3334;
        r22 = r17 & 0xFF;
        r20 = r14;
        r17 = 0x0;
        r23 = r21;
        while (1) {
            tmp = *(u8*)((u8*)r14 + 0x58);
            tmp = (s8)tmp;
            if ((s32)r17 >= (s32)tmp) break;
            r3 = lbl_8047A434;
            r5 = r19;
            r6 = r18;
            r4 = (s8)r17;
            fn_80082EA4();
            tmp = *(u8*)((u8*)r20 + 0x5B);
            r4 = 0x0;
            r5 = r4;
            tmp = (s8)tmp;
            tmp = tmp * 0x28;
            r6 = r14 + tmp;
            while (1) {
                tmp = r5 & 0xFFFF;
                if (tmp >= 4) break;
                r3 = r5 & 0xFFFF;
                tmp = r3 + 0x3b9;
                tmp = *(u8*)(r6 + tmp);
                tmp = (s8)tmp;
                if ((s32)tmp >= 0) {
                    r3 = tmp * 0x2a;
                    tmp = r4 & 0xFF;
                    r3 = r14 + r3;
                    r3 = *(u8*)((u8*)r3 + 0x517);
                    if (tmp < r3) {
                        r4 = r3;
                }
                }
                r5 = r5 + 0x1;

            }
            r3 = r4 & 0xFF;
            tmp = (s8)r16;
            r3 = r22 - r3;
            r4 = (s32)r3 >> 31;
            tmp = tmp << 2;
            r3 = r4 ^ r3;
            r3 = r3 - r4;
            *(u32*)((u8*)r21 + 0x0) = r3;
            r3 = *(u32*)((u8*)r21 + 0x0);
            tmp = *(u32*)(r23 + tmp);
            if ((s32)r3 < (s32)tmp) {
                r16 = (s8)r17;
                r15 = 0x1;

            } else {
                if ((s32)r3 == (s32)tmp) {
                    r15 = r15 + 0x1;
                    r3 = r15;
                    fn_800E0C04();
                    if (r3 == 0) {
                        r16 = (s8)r17;
                    }
                }
                    }
            r20 = r20 + 0x1;
            r21 = r21 + 0x4;
            r17 = r17 + 0x1;

        }
        r3 = (u32)lbl_803A3334;
        tmp = (s8)r16;
        r6 = (u32)lbl_803A3334;
        lbl_8047A448 = r16;
        r4 = r6 + tmp;
        r3 = (u32)sp + 0x14;
        tmp = *(u8*)((u8*)r4 + 0x5B);
        r4 = 0x209;
        r5 = 0x0;
        tmp = (s8)tmp;
        tmp = tmp * 0x28;
        r16 = r6 + tmp;
        r15 = *(u32*)((u8*)r16 + 0x3AC);
        r14 = *(u32*)((u8*)r16 + 0x3B0);
        r12 = *(u32*)((u8*)r16 + 0x3B4);
        r11 = *(u32*)((u8*)r16 + 0x3B8);
        r10 = *(u32*)((u8*)r16 + 0x3BC);
        r9 = *(u32*)((u8*)r16 + 0x3C0);
        r8 = *(u32*)((u8*)r16 + 0x3C4);
        r7 = *(u32*)((u8*)r16 + 0x3C8);
        r6 = *(u32*)((u8*)r16 + 0x3CC);
        tmp = *(u32*)((u8*)r16 + 0x3D0);
        *(u32*)(sp + 0x38) = tmp;
        fn_80032ED8();
        r15 = r3;
    }
    r3 = 0xa5;
    fn_80102620();
    tmp = r3 & 0xFF;
    if (tmp != 0) {
        r3 = 0xa5;
        fn_80102510();
    }
    f1 = lbl_8047BA00;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0xa5;
    r4 = 0x1;
    fn_8010264C();
    r3 = 0x1;
    fn_801C40F0();
    tmp = 0x1;
    f27 = lbl_8047B9F8;
    lbl_8047A449 = tmp;
    f28 = lbl_8047BA08;
    r14 = 0x43300000;
    f30 = lbl_8047BA10;
    f31 = lbl_8047BA18;
    while (f27 < f31) {

        fn_800F0308();
        fn_800D37CC();
        *(u32*)(sp + 0x1B4) = tmp;
        f29 = f0 - f28;
        fn_800D3088();
        f0 = f0 - f30;
        f0 = f0 / f29;
        f27 = f27 + f0;

    }
    if ((s32)r15 != 3) {
        if ((s32)r15 >= 3) goto L_80032E94;
        if ((s32)r15 < 2) {
            goto L_80032E94;
        }
        tmp = 0xd;
        lbl_8047A458 = tmp;
        return;
    }
    tmp = 0xc;
    lbl_8047A458 = tmp;
    return;
L_80032E94:
    tmp = 0xc;
    lbl_8047A458 = tmp;

    return;
}
#endif

/* 0x80032ED8 | 0x1E0 */
extern void fn_8020E0F8(void);
extern void fn_8020DFA0(void);
extern void fn_8020DF90(void);
extern void fn_801FCCC4(void);
extern void fn_801FCB94(void);
extern void fn_801FCC54(void);
extern void fn_801FCB84(void);
extern void fn_801FCAFC(void);
extern void fn_800896E0(void);
extern void fn_800896D8(void);
extern void fn_800896D0(void);
extern void fn_801FCB40(void);
extern void fn_801FCC3C(void);
extern void fn_801FCA2C(void);
extern void fn_801FC794(s32, u16);
extern void fn_801CA5C4(void);
extern void fn_801653C4(void);
extern void fn_80165A20(void);
extern u32 lbl_8047A444;
extern u8 lbl_8047A439;
extern void fn_800330B8(s32, u8*);
#if 1
asm void fn_80032ED8(void) {
#include "src/game/gs_npc_event_fn_80032ED8.inc"
}
#else
void fn_80032ED8(void) {
    extern u8 lbl_803A3334[];
    extern u8 lbl_8047A439;
    extern u32 lbl_8047A444;
    extern void fn_800330B8(s32, u8*);
    extern void fn_800896D0();
    extern void fn_800896D8();
    extern void fn_800896E0();
    extern void fn_80113F48();
    extern void fn_801653C4();
    extern void fn_80165A20();
    extern void fn_80176E0C();
    extern void fn_801CA5C4();
    extern void fn_801FC794(s32, u16);
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
    u32 tmp = 0;
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

    r29 = r3;
    r30 = r4;
    r31 = r5;
    r3 = r30;
    fn_8020E0F8();
    tmp = *(u32*)((u8*)r29 + 0x1C);
    r26 = r3;
    if ((s32)tmp == 0) {
        r4 = 0x0;
        fn_8020DFA0();
    } else {

        r4 = 0x1;
        fn_8020DFA0();
    }
    tmp = lbl_8047A444;
    r3 = r26;
    r4 = tmp & 0xFFFF;
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
    do {
        r5 = *(u16*)((u8*)r28 + 0x12);
        r3 = r27;
        r4 = r26 & 0xFF;
        fn_801FCB40();
        r28 = r28 + 0x2;
        r26 = r26 + 0x1;
    } while ((s32)r26 < 4);
    r3 = r27;
    fn_801FCC3C();
    fn_801FCA2C();
    r25 = r3;
    r27 = r31;
    r24 = 0x0;
    r26 = r25;
    r3 = (u32)lbl_803A3334;
    r28 = (u32)lbl_803A3334;
    do {
        if (r31 != 0) {
            r3 = r26;
            r4 = r27;
            fn_800330B8();
        } else {

            tmp = r24 + 0xd;
            r3 = r26;
            tmp = *(u8*)(r29 + tmp);
            tmp = (s8)tmp;
            tmp = tmp * 0x2a;
            r4 = r28 + tmp;
            r4 = r4 + 0x514;
            fn_800330B8();
        }
        r27 = r27 + 0x2a;
        r26 = r26 + 0x50;
        r24 = r24 + 0x1;
    } while ((s32)r24 < 4);
    tmp = r24 * 0x50;
    r26 = r25 + tmp;
    while ((s32)r24 < 6) {

        r3 = r26;
        r4 = 0x0;
        fn_801FC794();
        r26 = r26 + 0x50;
        r24 = r24 + 0x1;

    }
    r3 = r30;
    r4 = 0x1;
    r5 = 0x0;
    fn_801CA5C4();
    r26 = r3;
    fn_80113F48();
    r4 = 0x11170000;
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    fn_80176E0C();
    tmp = lbl_8047A439;
    if (tmp != 0) {
        r28 = 0x446;
    } else {

        r28 = 0x4cd;
    }
    fn_801653C4();
    if (r28 != r3) {
        r3 = r28;
        r4 = 0x0;
        r5 = 0x7f;
        fn_80165A20();
    }
    r3 = r26;
    return;
}
#endif

/* 0x800330B8 | 0x1C0 */
extern void fn_801FC828(s32, s32);
extern void fn_801FC7B4(s32, u8);
extern void fn_801EEE6C(u8, u8);
extern void fn_801FC7D4(s32, u8);
extern void fn_801FC744(s32, u32, u16);
extern void fn_801FC784(s32, u16);
extern void fn_801FC7A4(s32, u8);
extern void fn_801FC808(s32, s32, u8);
extern void fn_801FC7E4(s32, s32, s16);
extern void fn_801FC6F4(s32, s16);
extern void fn_801FC6E4(s32, u8);
extern void fn_801FC6D4(s32, u8);
extern void fn_801FC694(s32, u8);
extern void fn_801FC684(s32, u8);
#if 0
asm void fn_800330B8(void) {
#include "src/game/gs_npc_event_fn_800330B8.inc"
}
#else
void fn_800330B8(s32 arg0, u8* arg1) {
    s32 i;
    u8* walk;

    fn_801FC794(arg0, *(u16*)(arg1 + 0x0));
    fn_801FC828(arg0, 0);
    fn_801FC7B4(arg0, *(u8*)(arg1 + 0x2));
    if (*(u8*)(arg1 + 0x2) != 0) {
        fn_801EEE6C(*(u8*)(arg1 + 0x2), *(u8*)(arg1 + 0x28));
    }
    fn_801FC7D4(arg0, *(u8*)(arg1 + 0x3));
    walk = arg1;
    for (i = 0; i < 4; i++) {
        fn_801FC744(arg0, (u8)i, *(u16*)(walk + 0x4));
        walk += 2;
    }
    fn_801FC784(arg0, *(u16*)(arg1 + 0xC));
    fn_801FC7A4(arg0, *(u8*)(arg1 + 0xE));
    fn_801FC808(arg0, 0, *(u8*)(arg1 + 0xF));
    fn_801FC808(arg0, 1, *(u8*)(arg1 + 0x10));
    fn_801FC808(arg0, 2, *(u8*)(arg1 + 0x11));
    fn_801FC808(arg0, 3, *(u8*)(arg1 + 0x12));
    fn_801FC808(arg0, 4, *(u8*)(arg1 + 0x13));
    fn_801FC808(arg0, 5, *(u8*)(arg1 + 0x14));
    fn_801FC7E4(arg0, 0, *(s16*)(arg1 + 0x16));
    fn_801FC7E4(arg0, 1, *(s16*)(arg1 + 0x18));
    fn_801FC7E4(arg0, 2, *(s16*)(arg1 + 0x1A));
    fn_801FC7E4(arg0, 3, *(s16*)(arg1 + 0x1C));
    fn_801FC7E4(arg0, 4, *(s16*)(arg1 + 0x1E));
    fn_801FC7E4(arg0, 5, *(s16*)(arg1 + 0x20));
    fn_801FC6F4(arg0, *(s16*)(arg1 + 0x22));
    fn_801FC6E4(arg0, *(u8*)(arg1 + 0x24));
    fn_801FC6D4(arg0, *(u8*)(arg1 + 0x25));
    fn_801FC694(arg0, *(u8*)(arg1 + 0x26));
    fn_801FC684(arg0, *(u8*)(arg1 + 0x27));
    return;
}
#endif

/* ===== Phase 2 recovery stubs ===== */

/* fn_800301A8 - 0x800301A8 | size: 0x4 */
#if 0
asm void fn_800301A8(void) {
#include "src/game/gs_npc_event_fn_800301A8.inc"
}
#else
void fn_800301A8(void) { }
#endif

/* fn_800301AC - 0x800301AC | size: 0x4 */
#if 0
asm void fn_800301AC(void) {
#include "src/game/gs_npc_event_fn_800301AC.inc"
}
#else
void fn_800301AC(void) { }
#endif

/* fn_80030370 - 0x80030370 | size: 0xc */
#if 0
asm void fn_80030370(void) {
#include "src/game/gs_npc_event_fn_80030370.inc"
}
#else
void fn_80030370(u32 r3, u32* r4) {
    r4[0x4C / 4] = 0x43E3;
}
#endif

/* fn_8003037C - 0x8003037C | size: 0xc */
#if 0
asm void fn_8003037C(void) {
#include "src/game/gs_npc_event_fn_8003037C.inc"
}
#else
void fn_8003037C(u32 r3, u32* r4) {
    r4[0x4C / 4] = 0x43E2;
}
#endif

/* fn_80030388 - 0x80030388 | size: 0xa0 */
#if 0
asm void fn_80030388(void) {
#include "src/game/gs_npc_event_fn_80030388.inc"
}
#else
#pragma peephole off
void fn_80030388(u8* r3, u8* r4) {
    u32 flags = *(u32*)(r4 + 0x64);
    s32 evtype = *(s16*)(r4 + 0x6);
    u32 combined = (flags & 0xa1400000) | *(u8*)(r3 + 0x8b);
    switch (evtype) {
    case 0x10CB:
        fn_800FB680(0, 0, combined, 0x4412);
        break;
    case 0x10CA:
        {
            s32 val = fn_8012A5B0((void*)lbl_803A2688, 1, 0);
            fn_80132A38(0x4D, val);
            fn_800FB680(0, 0, combined, 0x4413);
        }
        break;
    }
}
#pragma peephole on
#endif

/* fn_80030428 - 0x80030428 | size: 0x4 */
#if 0
asm void fn_80030428(void) {
#include "src/game/gs_npc_event_fn_80030428.inc"
}
#else
void fn_80030428(void) { }
#endif

/* fn_8003042C - 0x8003042C | size: 0x148 */
extern void fn_801230E0(void);
extern void fn_801440A0(void);
extern void fn_80144088(void);
extern u8 lbl_80266F68[];
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
#if 0
asm void fn_8003042C(void) {
#include "src/game/gs_npc_event_fn_8003042C.inc"
}
#else
#pragma push
#pragma peephole off
void fn_8003042C(u8* arg0, u8* arg1) {
    u32 combined;
    s32 kind;
    void* obj;

    combined = (*(u32*)(arg1 + 0x64) & ~0xFF) | *(u8*)(arg0 + 0x8B);
    kind = 0;
    if (*(s16*)(arg1 + 0x6) == *(u16*)(lbl_80266F68 + 0x12)) {
        kind = *(u16*)(lbl_80266F68 + 0x0);
    }
    if (*(s16*)(arg1 + 0x6) == *(u16*)(lbl_80266F68 + 0x26)) {
        kind = *(u16*)(lbl_80266F68 + 0x14);
    }

    obj = 0;
    if (kind == 1) {
        obj = (void*)fn_8012A5B0(0, 3, (u16)lbl_8047A424);
    } else if (kind == 2) {
        obj = (void*)fn_8012A5B0(lbl_803A2688, 3, (u16)lbl_8047A420);
    }

    if (obj != 0) {
        if (((u16 (*)(void*))fn_801230E0)(obj) != 0) {
            fn_801440A0();
            fn_80144088();
            fn_80132A38(0x37, ((s32 (*)(void))fn_800FA280)());
            fn_800FB680(0, 0, combined, 0xE7);
            *(u32*)(arg1 + 0x4C) = 0;
            fn_80109220(arg1, 1);
        } else {
            fn_80109220(arg1, 0);
        }
    } else {
        fn_80109220(arg1, 0);
    }
}
#pragma pop
#endif

/* fn_80030574 - 0x80030574 | size: 0x234 */
extern void fn_80123CD4(void);
extern void fn_8011BEB4(void);
extern void fn_800FBB34(s32, s32, s32, s32, u32, u16);
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
#if 1
asm void fn_80030574(void) {
#include "src/game/gs_npc_event_fn_80030574.inc"
}
#else
void fn_80030574(void) { /* TODO */ }
#endif

/* fn_800307A8 - 0x800307A8 | size: 0x12c */
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
#if 1
asm void fn_800307A8(void) {
#include "src/game/gs_npc_event_fn_800307A8.inc"
}
#else
void fn_800307A8(void) { /* TODO */ }
#endif

/* fn_800308D4 - 0x800308D4 | size: 0x170 */
extern void fn_8011E778(void);
extern void fn_8011E760(void);
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
#if 1
asm void fn_800308D4(void) {
#include "src/game/gs_npc_event_fn_800308D4.inc"
}
#else
void fn_800308D4(void) { /* TODO */ }
#endif

/* fn_80030A44 - 0x80030A44 | size: 0x1d0 */
extern u32 lbl_8047A424;
extern u32 lbl_8047A420;
#if 1
asm void fn_80030A44(void) {
#include "src/game/gs_npc_event_fn_80030A44.inc"
}
#else
void fn_80030A44(void) { /* TODO */ }
#endif

/* fn_80030C14 - 0x80030C14 | size: 0x120 */
extern u8 lbl_803A3230[];
extern u8 lbl_803A31E8[];
extern u8 lbl_80314F98[];
extern f32 lbl_8047B9D4;
extern f32 lbl_8047B9F0;
#if 0
asm void fn_80030C14(void) {
#include "src/game/gs_npc_event_fn_80030C14.inc"
}
#else
#pragma peephole off
void fn_80030C14(void* r3, u8* r4) {
    void* model = NULL;
    s32 evtype = *(s16*)(r4 + 0x6);
    switch (evtype) {
    case 0x10CC:
        model = fn_80109934(lbl_803A3230);
        break;
    case 0x10CD:
        model = fn_80109934(lbl_803A31E8);
        break;
    }
    if (model != NULL) {
        fn_800D88DC(3);
        fn_800D888C(4);
        fn_800D6A00(7);
        fn_800D7820(lbl_80314F98);
        fn_800D85D4(0, model);
        fn_800D67BC(2);
        fn_800D61E4(0, 0);
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, 0xFF);
        fn_800D59B8(0, lbl_8047B9D4, lbl_8047B9D4);
        fn_800D61E4(*(s16*)(r4 + 0x54), *(s16*)(r4 + 0x56));
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, 0xFF);
        fn_800D59B8(0, lbl_8047B9F0, lbl_8047B9F0);
        fn_800D6728();
    }
}
#pragma peephole on
#endif

/* fn_80030D34 - 0x80030D34 | size: 0x1d8 */
extern void fn_8011F1A0(void);
#if 1
asm void fn_80030D34(void) {
#include "src/game/gs_npc_event_fn_80030D34.inc"
}
#else
void fn_80030D34(void) { /* TODO */ }
#endif

/* fn_80030F0C - 0x80030F0C | size: 0x27c */
extern void fn_8011ED68(void);
#if 1
asm void fn_80030F0C(void) {
#include "src/game/gs_npc_event_fn_80030F0C.inc"
}
#else
void fn_80030F0C(void) { /* TODO */ }
#endif

/* fn_800347B8 - 0x800347B8 | size: 0xc */
extern u32 lbl_8047A440;
#if 0
asm void fn_800347B8(void) {
#include "src/game/gs_npc_event_fn_800347B8.inc"
}
#else
void fn_800347B8(void) {
    *(u8*)&lbl_8047A440 = 1;
}
#endif

/* fn_800347C4 - 0x800347C4 | size: 0x24 */
extern void fn_80166A28(s32);
#if 0
asm void fn_800347C4(void) {
#include "src/game/gs_npc_event_fn_800347C4.inc"
}
#else
void fn_800347C4(void) {
    fn_80166A28(0x26);
}
#endif

/* fn_800347E8 - 0x800347E8 | size: 0x24 */
#if 0
asm void fn_800347E8(void) {
#include "src/game/gs_npc_event_fn_800347E8.inc"
}
#else
void fn_800347E8(void) {
    fn_80166A28(0x26);
}
#endif

/* fn_8003480C - 0x8003480C | size: 0x24 */
#if 0
asm void fn_8003480C(void) {
#include "src/game/gs_npc_event_fn_8003480C.inc"
}
#else
void fn_8003480C(void) {
    fn_80166A28(0x26);
}
#endif

/* fn_80034DC0 - 0x80034DC0 | size: 0x78 */
extern u32 lbl_8047A44C;
#if 0
asm void fn_80034DC0(void) {
#include "src/game/gs_npc_event_fn_80034DC0.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80034DC0(u8* arg0, u8* arg1) {
    u32 byte;
    s32 mask;
    s32 combined;

    byte = *(u8*)(arg0 + 0x8B);
    mask = -0x100;
    combined = byte | mask;
    fn_800FB680(0, 0, combined, 0x3CC8);
    fn_80132A38(0x34, lbl_8047A44C);
    fn_800FBB34(0, 0, *(s16*)(arg1 + 0x54), *(s16*)(arg1 + 0x56), combined, 0x3CC9);
}
#pragma pop
#endif
