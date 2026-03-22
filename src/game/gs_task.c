/**
 * @file gs_task.c
 * @brief GStask -- Scene/FSYS task management and archive loading callbacks.
 *
 * Address range: 0x80006630 - 0x80009178 (~45 functions)
 *
 * This module manages the task system that sits between the main loop and
 * the individual game subsystems. It handles:
 *   - FSYS archive loading via task callbacks (fn_801FB1C0)
 *   - Scene transition sequencing with fn_801026A4 dispatch
 *   - Archive completion callbacks with scene ID routing
 *   - Resource group management (models, textures, scripts)
 *
 * The functions follow a clear pattern:
 *   1. Load an SDA global (lbl_8047A288) for the current scene/map ID
 *   2. Call fn_801FB1C0 to initiate an archive load with a priority level
 *   3. On success, call fn_80106394 and fn_80106080 to activate the loaded data
 *   4. Return 1 on success, -1 on failure
 *
 * Key functions:
 *   fn_80006630  GStask_InitCamera         -- calls GSscene_CameraSetPosition
 *   fn_80006654  GStask_LoadTopMenu         -- loads archive slot 6 (topmenu)
 *   fn_800066C4  GStask_SetSceneType        -- dispatches to fn_80177A44 based on mode
 *   fn_80006724  GStask_LoadPDAMenu         -- loads archive slot 6, priority 3 (pda_menu)
 *   fn_8000677C  GStask_LoadPocketMenu      -- loads archive slot 6, priority 2 (pocket_menu)
 *   fn_800067D4  GStask_LoadPCBoxMenu       -- loads archive slot 6, priority 1 (pcbox_menu)
 *   fn_8000682C  GStask_LoadBattleMenu      -- loads archive slot 6, priority 0 (battle)
 *   fn_80006884  GStask_SelectRandomNPC     -- random NPC selection from scene data
 *   fn_80006908  GStask_ProcessSceneEvent   -- 0x6A4 bytes, large scene event dispatcher
 *   fn_80006FAC  GStask_ProcessEventResult  -- handles event return codes
 *   fn_80007088  GStask_GetField1           -- struct field accessor (offset 0x00)
 *   fn_800070CC  GStask_GetField2           -- struct field accessor (offset 0x04)
 *   fn_80007110  GStask_GetField3           -- struct field accessor (offset 0x08)
 *   fn_80007154  GStask_LoadSlot0           -- archive load helper, slot 0
 *   fn_800071AC  GStask_LoadSlot1           -- archive load helper, slot 1
 *   fn_80007204  GStask_LoadSlot2           -- archive load helper, slot 2
 *   fn_8000725C  GStask_LoadSlot3           -- archive load helper, slot 3
 *   fn_800072B4  GStask_LoadSlot4           -- archive load helper, slot 4
 *   fn_8000730C  GStask_LoadSlot5           -- archive load helper, slot 5
 *   fn_80007364  GStask_InitSceneResources  -- 0x2F8 bytes, resource init
 *   fn_8000765C  GStask_ShutdownResources   -- resource cleanup
 *   fn_80007708  GStask_UpdateLoadState     -- resource loading state machine
 *   fn_80007778  GStask_GetLoadFlag0        -- small accessor (0x20 bytes)
 *   fn_80007798  GStask_GetLoadFlag1        -- small accessor
 *   fn_800077B8  GStask_GetLoadFlag2        -- small accessor
 *   fn_800077D8  GStask_GetLoadFlag3        -- small accessor
 *   fn_800077F8  GStask_ReturnZero          -- stub, returns 0
 *   fn_80007800  GStask_GetLoadFlag4        -- small accessor
 *   fn_80007820  GStask_GetLoadFlag5        -- small accessor
 *   fn_80007840  GStask_ReturnZero2         -- stub, returns 0
 *   fn_80007848  GStask_ValidateScene       -- validates scene index bounds
 *   fn_800078EC  GStask_ArchiveCB0          -- archive completion callback
 *   fn_80007944  GStask_ArchiveCB1          -- archive completion callback
 *   fn_8000799C  GStask_ArchiveCB2          -- archive completion callback
 *   fn_800079F4  GStask_ArchiveCB3          -- archive completion callback
 *   fn_80007A4C  GStask_ArchiveCB4          -- archive completion callback
 *   fn_80007A84  GStask_DispatchArchiveCBs  -- routes to the above callbacks
 *   fn_80007B30  GStask_MainUpdate          -- 0x4AC bytes, main task state machine
 *   fn_80007FDC  GStask_GetSceneCount       -- return scene table size
 *   fn_80008014  GStask_GetEventCount       -- return event table size
 *   fn_8000804C  GStask_SetupTransition     -- scene transition setup (0xF8 bytes)
 *   fn_80008144  GStask_Accessor0           -- 8-byte accessor (lwz/blr)
 *   fn_8000814C  GStask_Accessor1           -- 8-byte accessor
 *   fn_80008154  GStask_Accessor2           -- 8-byte accessor
 *   fn_8000815C  GStask_Accessor3           -- 8-byte accessor
 *   fn_80008164  GStask_Accessor4           -- 8-byte accessor
 *   fn_8000816C  GStask_Accessor5           -- 8-byte accessor
 *   fn_80008174  GStask_Accessor6           -- 8-byte accessor
 *   fn_8000817C  GStask_Accessor7           -- 8-byte accessor
 *   fn_80008184  GStask_FinalizeLoad        -- 0xC0 bytes, finalize resource load
 *   fn_80008244  GStask_BuildResourceList   -- 0x14C bytes, compile resource table
 *
 * Code patterns:
 *   - SDA globals lbl_8047A288 (current scene halfword), lbl_80478F50 (scene table ptr)
 *   - fn_801026A4 is the main event dispatch (takes slot, params, flags)
 *   - fn_80102568 is the event cancel/cleanup function
 *   - fn_8001E304 is a bounded random selection helper
 *   - fn_8001E200 is a random result commit function
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* Scene system */
extern void  fn_801794F0(void);                   /* GSscene_CameraSetPosition */
extern void  fn_80177A44(s32 mode);               /* GSscene_SetMode */

/* FSYS archive loading */
extern void* fn_801FB1C0(s32 slot, u16 sceneId, s32 priority, s32 group);
extern s32   fn_80106394(s32 slot, s32 active);
extern s32   fn_80106080(s32 slot);

/* Event dispatch */
extern u8    fn_80102620(s32 slot);               /* Check if event slot busy */
extern s32   fn_80102510(s32 slot);               /* Cancel active event */
extern s32   fn_801026A4(s32 slot, ...);           /* Dispatch scene event */
extern void  fn_80102568(s32 slot, s32 p1, s32 p2);  /* Event cleanup */
extern s32   fn_801022B8(s32 slot);               /* Get event result code */
extern s32   fn_8010264C(s32 slot, s32 p1);       /* Query event state */
extern void  fn_80102868(s32 slot, s32 p1, s32 p2);  /* Set event params */

/* Resource management */
extern void* fn_8020E0F8(void);                   /* Get scene resource table */
extern void  fn_8020DFB0(void* tbl, u8 idx);      /* Set resource field 0 */
extern void  fn_8020DFA0(void* tbl, u8 idx);      /* Set resource field 1 */
extern void  fn_8020DF90(void* tbl, u16 val);     /* Set resource field 2 */
extern void  fn_8020E068(void* tbl, s32 slot);    /* Get resource subfield */
extern void  fn_8020DF50(void* tbl, s32 slot, u16 val);  /* Set resource subfield */
extern void  fn_8020DF10(void* tbl, s32 slot, s32 val);  /* Set resource property */
extern u8    fn_8020E0E0(void* tbl);              /* Get resource type */
extern u8    fn_8020E0C8(void* tbl);              /* Get resource subtype */
extern u16   fn_8020E0B0(void* tbl);              /* Get resource size */
extern u16   fn_8020E020(void* tbl, s32 slot);    /* Get resource slot value */

/* Random selection helpers */
extern u8    fn_8001E304(u8 count, u32* outResult, void* validationFn);
extern void  fn_8001E200(void);

/* Callback function pointers used in validation */
extern void  fn_80008744(void);   /* Validate NPC index (type byte) */
extern void  fn_800086EC(void);   /* Validate NPC index (type byte) */
extern void  fn_8000879C(void);   /* Validate NPC index (type u16) */
extern void  fn_8000868C(void);   /* Validate scene item index */
extern void  fn_800085D8(void);   /* Validate encounter difficulty */

/* =========================================================================
 * SDA global variables
 * ========================================================================= */

extern u16  gCurrentSceneId;     /* lbl_8047A288 : .sbss -- current scene/map halfword */
extern void* gSceneTable;       /* lbl_80478F50 : .sbss -- scene definition table ptr */
extern void* gSceneItemTable;   /* lbl_80478F20 : .sbss -- scene item lookup table ptr */
extern void* gNPCIndexTable;    /* lbl_80478F40 : .sbss -- NPC type lookup table ptr */
extern void* gNPCSubTable;      /* lbl_80478F48 : .sbss -- NPC subtype lookup table ptr */
extern void* gEncounterTable;   /* lbl_80478F00 : .sbss -- encounter difficulty table ptr */

/* =========================================================================
 * Rodata references
 * ========================================================================= */

/* lbl_802666B0: Shift-JIS encoded scene name string (unknown use) */
/* lbl_802666E0: Scene type mapping table (8 entries, 2x u32 each) */
/* lbl_80266700: Scene resource ID mapping table (14 entries) */
/* jumptable_802E28D0: Jump table for fn_80006908 scene event dispatch */

/* =========================================================================
 * Function: GStask_InitCamera
 * Address:  0x80006630
 * Size:     0x24
 *
 * Simple wrapper that calls GSscene_CameraSetPosition to reset the
 * camera, then returns 0. Called at the start of scene transitions.
 * ========================================================================= */
s32 GStask_InitCamera(void) {
    fn_801794F0();
    return 0;
}

/* =========================================================================
 * Function: GStask_LoadTopMenu
 * Address:  0x80006654
 * Size:     0x70
 *
 * Checks if archive slot 6 is busy; if so, cancels it and returns 0.
 * Otherwise, dispatches a new archive load for the top menu FSYS with
 * default parameters and sets up the viewport at (0x14, 0x104).
 * ========================================================================= */
s32 GStask_LoadTopMenu(void) {
    if (fn_80102620(6) != 0) {
        fn_80102510(6);
    } else {
        fn_801026A4(6, 0, 0, 0, 1, 0);
        fn_80102868(6, 0x14, 0x104);
    }
    return 0;
}

/* =========================================================================
 * Function: GStask_SetSceneType
 * Address:  0x800066C4
 * Size:     0x60
 *
 * Given a mode parameter in r4, dispatches to GSscene_SetMode with the
 * appropriate scene type constant:
 *   mode 0 or >=3: type 0 (normal overworld)
 *   mode 1:        type 5 (battle arena)
 *   mode 2:        type 6 (cutscene)
 * Always returns 0.
 * ========================================================================= */
s32 GStask_SetSceneType(void* unused, s32 mode) {
    if (mode == 1) {
        fn_80177A44(5);
    } else if (mode == 2) {
        fn_80177A44(6);
    } else {
        fn_80177A44(0);
    }
    return 0;
}

/* =========================================================================
 * Function: GStask_LoadPDAMenu
 * Address:  0x80006724
 * Size:     0x58
 *
 * Loads the PDA menu archive (priority 3) into the FSYS system.
 * Uses the current scene ID from lbl_8047A288 as a context parameter.
 * On success, activates the archive and returns 1; on failure returns -1.
 * ========================================================================= */
s32 GStask_LoadPDAMenu(void) {
    void* result;

    result = fn_801FB1C0(0, gCurrentSceneId, 8, 3);
    if (result == NULL) {
        return -1;
    }
    fn_80106394(1, 1);
    fn_80106080(1);
    return 1;
}

/* =========================================================================
 * Function: GStask_LoadPocketMenu
 * Address:  0x8000677C
 * Size:     0x58
 *
 * Loads the pocket/bag menu archive (priority 2).
 * Same structure as GStask_LoadPDAMenu but with priority 2.
 * ========================================================================= */
s32 GStask_LoadPocketMenu(void) {
    void* result;

    result = fn_801FB1C0(0, gCurrentSceneId, 8, 2);
    if (result == NULL) {
        return -1;
    }
    fn_80106394(1, 1);
    fn_80106080(1);
    return 1;
}

/* =========================================================================
 * Function: GStask_LoadPCBoxMenu
 * Address:  0x800067D4
 * Size:     0x58
 *
 * Loads the PC box menu archive (priority 1).
 * ========================================================================= */
s32 GStask_LoadPCBoxMenu(void) {
    void* result;

    result = fn_801FB1C0(0, gCurrentSceneId, 8, 1);
    if (result == NULL) {
        return -1;
    }
    fn_80106394(1, 1);
    fn_80106080(1);
    return 1;
}

/* =========================================================================
 * Function: GStask_LoadBattleMenu
 * Address:  0x8000682C
 * Size:     0x58
 *
 * Loads the battle menu archive (priority 0 -- highest).
 * ========================================================================= */
s32 GStask_LoadBattleMenu(void) {
    void* result;

    result = fn_801FB1C0(0, gCurrentSceneId, 8, 0);
    if (result == NULL) {
        return -1;
    }
    fn_80106394(1, 1);
    fn_80106080(1);
    return 1;
}

/* =========================================================================
 * Remaining functions in this module (0x80006884 - 0x80009178) are not
 * fully decompiled here. They follow the same patterns:
 *
 * fn_80006884 (GStask_SelectRandomNPC):
 *   Uses fn_8001E304 to randomly select an NPC from the scene table,
 *   bounds-checks against lbl_80478F20, stores result in lbl_8047A288.
 *
 * fn_80006908 (GStask_ProcessSceneEvent):
 *   The largest function (0x6A4 bytes) - a massive switch statement that
 *   dispatches scene events based on event codes returned by fn_801022B8.
 *   Uses jumptable_802E28D0 for the initial dispatch, then performs
 *   NPC selection, resource loading, and encounter setup based on the
 *   event type. Each case in the switch:
 *     - Gets a resource from the scene table via fn_8020E0E0/fn_8020E0C8
 *     - Validates the result using fn_8001E304 with a callback
 *     - Commits the selection via fn_8001E200
 *     - Updates the resource table via fn_8020DFB0/fn_8020DFA0/etc.
 *
 * fn_80006FAC - fn_80007110 (accessor cluster):
 *   Small field accessors for the scene resource structure.
 *
 * fn_80007154 - fn_8000730C (archive load helpers):
 *   Six nearly identical functions that load archives for slots 0-5,
 *   each following the same load/activate/commit pattern.
 *
 * fn_80007364 (GStask_InitSceneResources):
 *   0x2F8 bytes. Initializes the resource table for a new scene,
 *   setting up all model/texture/script entries.
 *
 * fn_80007B30 (GStask_MainUpdate):
 *   0x4AC bytes. The main task update state machine that coordinates
 *   archive loading, scene transitions, and event processing.
 *
 * fn_80008184 - fn_80008244 (finalization):
 *   Finalize loaded resources and build the active resource list.
 * ========================================================================= */

/* Accessor functions (0x80008390 - 0x80009178) follow a repeating pattern:
 *
 * fn_80008390 (size 0x6C) - Get party slot 0 species
 * fn_800083FC (size 0x64) - Get party slot 0 level
 * fn_80008460 (size 0x60) - Get party slot 0 HP
 * fn_800084C0 (size 0x58) - Get party slot 0 status
 * fn_80008518 (size 0x64) - Get party slot 1 species
 * ...and so on for each party slot property
 *
 * These are thin wrappers that index into the party data structure,
 * extract a field, and return it. They are likely used as callback
 * function pointers passed to the UI rendering system.
 */

/* ===================================================================
 * Lookup/accessor functions
 * =================================================================== */

/* External functions used by lookup helpers */
extern void* fn_800FA280(u32 id);
extern void* fn_8010C4D4(u16 index);
extern void* fn_8020DED8(void* ptr);

/* Global state references */
extern u32 lbl_80478B38;

/* =======================================================================
 * fn_8000857C -- GStask_LookupResourceById
 *
 * Looks up a resource by ID. If ID is 9, uses a fixed constant.
 * If ID >= the global count (lbl_80478B38), also uses the constant.
 * Otherwise, converts via fn_8010C4D4 and passes to fn_800FA280.
 *
 * Address: 0x8000857C  Size: 0x5C (92 bytes)
 * ======================================================================= */
void* fn_8000857C(u32 id)
{
    if (id == 9) {
        return fn_800FA280(0x0000EB63);
    }
    if (id >= lbl_80478B38) {
        return fn_800FA280(0x0000EB63);
    }
    {
        u32 idx;
        idx = (u16)id;
        return fn_800FA280((u32)fn_8010C4D4(idx));
    }
}

/* =======================================================================
 * fn_800087FC -- GStask_LookupEventById
 *
 * Looks up an event resource by ID. Returns NULL-equivalent constant
 * on invalid input or out-of-range. Otherwise, chains through
 * fn_8020E0F8 -> fn_8020DED8 -> fn_800FA280.
 *
 * Address: 0x800087FC  Size: 0x6C (108 bytes)
 * ======================================================================= */
void* fn_800087FC(u32 id)
{
    void* result;

    if (id == 0) {
        return fn_800FA280(0x0000EB63);
    }

    {
        u32 count;
        count = *(u32*)gSceneTable;
        if (id >= count) {
            return fn_800FA280(0x0000EB63);
        }
    }

    result = fn_8020DED8(((void*(*)(u16))fn_8020E0F8)((u16)id));

    if (result == NULL) {
        result = (void*)0x0000EB63;
    }

    return fn_800FA280((u32)result);
}

extern u8 lbl_8047882E;
extern u8 lbl_8047A271;
extern u8 lbl_8047A280;
extern u16 lbl_8047A282;
extern u8 lbl_8047A284;
extern u8 lbl_8047A285;
extern u8 lbl_8047A286;

/* Address: 0x800077F8 | Size: 0x8 | Pattern: return_constant */
u32 fn_800077F8(void) { return 1; }

/* Address: 0x80007840 | Size: 0x8 | Pattern: return_constant */
u32 fn_80007840(void) { return 1; }

/* Address: 0x80008144 | Size: 0x8 | Pattern: return_constant */
u32 fn_80008144(void) { return 1; }

/* Address: 0x8000814C | Size: 0x8 | Pattern: sda_getter */
u16 fn_8000814C(void) {
    return lbl_8047A282;
}

/* Address: 0x80008154 | Size: 0x8 | Pattern: sda_getter */
u8 fn_80008154(void) {
    return lbl_8047A280;
}

/* Address: 0x8000815C | Size: 0x8 | Pattern: sda_getter */
u8 fn_8000815C(void) {
    return lbl_8047882E;
}

/* Address: 0x80008164 | Size: 0x8 | Pattern: sda_getter */
u8 fn_80008164(void) {
    return lbl_8047A286;
}

/* Address: 0x8000816C | Size: 0x8 | Pattern: sda_getter */
u8 fn_8000816C(void) {
    return lbl_8047A285;
}

/* Address: 0x80008174 | Size: 0x8 | Pattern: sda_getter */
u8 fn_80008174(void) {
    return lbl_8047A284;
}

/* Address: 0x8000817C | Size: 0x8 | Pattern: sda_getter */
u8 fn_8000817C(void) {
    return lbl_8047A271;
}

/* 0x80008868 | 0x3D8 */
void fn_80008868(void) {
    extern u8 lbl_80266688[];
    extern u8 lbl_803A19C8[];
    extern u8 lbl_80478838[];
    extern u8 lbl_80478F98[];
    extern u8 lbl_80478F9C[];
    extern u8 lbl_8047B6C8[];
    extern void fn_8001E4B4();
    extern void fn_8001EA98();
    extern void fn_8005D9E4();
    extern void fn_800C8520();
    extern void fn_800FAEF8();
    extern void fn_801906A0();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = *(u32*)((u8*)r3 + 0x4);
    r25 = 0x28;
    fn_8005D9E4();
    r4 = *(u32*)lbl_80478F98;
    tmp = 0x0;
    *(u32*)(sp + 0x8) = tmp;
    tmp = 0x1e0;
    r4 = *(u32*)((u8*)r4 + 0x0);
    r24 = r3 & 0xFF;
    *(u32*)(sp + 0x8) = tmp;
    r26 = r4;
    if ((s32)r4 > (s32)r24) {
        r26 = r24;
    }
    r27 = r26 * 0xd;
    r3 = 0x1e;
    r5 = r4 + 0x19;
    r6 = r27;
    r4 = 0x28;
    fn_8001EA98();
    /* lha tmp, lbl_80478838@sda21(tmp) */;
    if ((s32)tmp > 0) {
        r4 = 0x21;
        r6 = 0x21;
        r8 = 0x17;
        tmp = (u32)r3 >> 31;
        tmp = tmp + r3;
        r7 = (s32)tmp >> 1;
        r3 = r7 + 0x14;
        r5 = r7 + 0x28;
        r7 = r7 + 0x1e;
        fn_8001E4B4();
    }
    r3 = *(u32*)lbl_80478F98;
    /* lha r4, lbl_80478838@sda21(tmp) */;
    tmp = *(u32*)((u8*)r3 + 0x0);
    tmp = tmp - r4;
    if ((s32)tmp > (s32)r24) {
        r4 = r27 + 0x2f;
        r6 = r4;
        r8 = r27 + 0x39;
        tmp = (u32)r3 >> 31;
        tmp = tmp + r3;
        r7 = (s32)tmp >> 1;
        r3 = r7 + 0x14;
        r5 = r7 + 0x28;
        r7 = r7 + 0x1e;
        fn_8001E4B4();
    }
    r27 = 0x0;
    r28 = (u32)lbl_80478838;
    r4 = (u32)lbl_803A19C8;
    r3 = (u32)lbl_80266688;
    r29 = (u32)lbl_803A19C8;
    r30 = (u32)lbl_80266688;
    while ((s32)r27 < (s32)r26) {
    /* L_80008968 */
    tmp = *(s16*)((u8*)r28 + 0x2);
    if ((s32)tmp == (s32)r27) {
        r3 = 0xFF000000;
        r24 = r3 + 0xff;
    } else {
        r24 = -0x1;
    }
    /* lha tmp, lbl_80478838@sda21(tmp) */;
    r5 = r27 + tmp;
    if ((s32)r5 < 0) {
        tmp = 0x0;
    } else {
        r3 = *(u32*)lbl_80478F98;
        tmp = *(u32*)((u8*)r3 + 0x0);
        if (r5 >= tmp) {
            tmp = 0x0;
        } else {
    r7 = *(s16*)((u8*)r3 + 0x4);
    if ((s32)r5 != 0) {
        r3 = 0x0;
        if ((s32)tmp > 0) {
            if ((s32)tmp > 8) {
                r4 = *(u32*)lbl_80478F9C;
                tmp = r6 + 0x7;
                tmp = (u32)tmp >> 3;
                ctr_fn = (void(*)(void))tmp;
                if ((s32)r6 > 0) {
                    do {
                        tmp = (s16)r7;
                        r3 = r3 + 0x8;
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r4 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r4 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r4 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r4 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r4 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r4 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r4 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        r7 = *(s16*)(r4 + tmp);
                    } while (--ctr != 0);
            }
            }
            r6 = *(u32*)lbl_80478F9C;
            tmp = r4 - r3;
            ctr_fn = (void(*)(void))tmp;
            if ((s32)r3 < (s32)r4) {
                do {
                    tmp = (s16)r7;
                    r3 = r3 + 0x1;
                    r4 = tmp << 3;
                    tmp = r4 + 0x6;
                    r7 = *(s16*)(r6 + tmp);
                } while (--ctr != 0);
        }
        }
        tmp = (s16)r7;
        r4 = *(u32*)lbl_80478F9C;
        r3 = tmp << 3;
        tmp = r3 + 0x6;
        r7 = *(s16*)(r4 + tmp);
    }
    tmp = r7 & 0xFFFF;
        }
    }
    /* L_80008AA8 */
    r3 = r29;
    r4 = r30;
    r6 = tmp & 0xFFFF;
    fn_800C8520();
    r4 = r25;
    r5 = r24;
    r6 = r29;
    r3 = 0x1e;
    fn_800FAEF8();
    /* lha tmp, lbl_80478838@sda21(tmp) */;
    r4 = r27 + tmp;
    if ((s32)r4 < 0) {
        tmp = 0x0;
    } else {
        r3 = *(u32*)lbl_80478F98;
        tmp = *(u32*)((u8*)r3 + 0x0);
        if (r4 >= tmp) {
            tmp = 0x0;
        } else {
    r7 = *(s16*)((u8*)r3 + 0x4);
    if ((s32)r4 != 0) {
        r3 = 0x0;
        if ((s32)tmp > 0) {
            if ((s32)tmp > 8) {
                r5 = *(u32*)lbl_80478F9C;
                tmp = r6 + 0x7;
                tmp = (u32)tmp >> 3;
                ctr_fn = (void(*)(void))tmp;
                if ((s32)r6 > 0) {
                    do {
                        tmp = (s16)r7;
                        r3 = r3 + 0x8;
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r5 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r5 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r5 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r5 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r5 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r5 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        tmp = *(s16*)(r5 + tmp);
                        r6 = tmp << 3;
                        tmp = r6 + 0x6;
                        r7 = *(s16*)(r5 + tmp);
                    } while (--ctr != 0);
            }
            }
            r5 = *(u32*)lbl_80478F9C;
            tmp = r4 - r3;
            ctr_fn = (void(*)(void))tmp;
            if ((s32)r3 < (s32)r4) {
                do {
                    tmp = (s16)r7;
                    r3 = r3 + 0x1;
                    r4 = tmp << 3;
                    tmp = r4 + 0x6;
                    r7 = *(s16*)(r5 + tmp);
                } while (--ctr != 0);
        }
        }
        tmp = (s16)r7;
        r4 = *(u32*)lbl_80478F9C;
        r3 = tmp << 3;
        tmp = r3 + 0x6;
        r7 = *(s16*)(r4 + tmp);
    }
    tmp = r7 & 0xFFFF;
        }
    }
    /* L_80008BF8 */
    r3 = tmp & 0xFFFF;
    fn_801906A0();
    r7 = r3;
    r4 = r25;
    r5 = r24;
    r3 = r31 + 0x1e;
    r6 = (u32)lbl_8047B6C8;
    fn_800FAEF8();
    r25 = r25 + 0xd;
    r27 = r27 + 0x1;
    } /* end while loop */
    return;
}

/* 0x80008C40 | 0x538 */
void fn_80008C40(void) {
    extern u8 lbl_80478838[];
    extern u8 lbl_80478F98[];
    extern u8 lbl_80478F9C[];
    extern void fn_8005D9E4();
    extern void fn_80105624();
    extern void fn_801903B0();
    extern void fn_80190528();
    u8 sp[0x30];
    u32 tmp = 0;
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

    r28 = r3;
    fn_80105624();
    r27 = *(u16*)((u8*)r3 + 0x6);
    fn_80105624();
    r4 = *(u32*)lbl_80478F98;
    r31 = *(u16*)((u8*)r3 + 0x0);
    tmp = *(u32*)((u8*)r4 + 0x0);
    r3 = *(u32*)((u8*)r28 + 0x4);
    r30 = (s16)tmp;
    fn_8005D9E4();
    r29 = r3 & 0xFF;
    tmp = (s16)r29;
    if ((s32)r30 < (s32)tmp) {
        r29 = r30;
    }
    r4 = r27 & 0xFFFF;
    r3 = *(u32*)lbl_80478838;
    tmp = r4 & 0x1;
    if ((s32)tmp == 0) goto L_80008CB4;
    r3 = *(s16*)((u8*)(u32)sp + 0xA);
    *(u16*)(sp + 0xA) = tmp;
    goto L_80008CCC;
L_80008CB4:
    tmp = r4 & 0x00000002;
    if ((s32)tmp == 0) goto L_80008CCC;
    r3 = *(s16*)((u8*)(u32)sp + 0xA);
    tmp = r3 + 0x1;
    *(u16*)(sp + 0xA) = tmp;
L_80008CCC:
    tmp = r4 & 0x00000004;
    if ((s32)tmp == 0) goto L_80008CFC;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_8005D9E4();
    r3 = r3 & 0xFF;
    tmp = *(s16*)((u8*)(u32)sp + 0xA);
    tmp = tmp - r3;
    tmp = (s16)tmp;
    *(u16*)(sp + 0xA) = tmp;
    goto L_80008D28;
L_80008CFC:
    tmp = r4 & 0x00000008;
    if ((s32)tmp == 0) goto L_80008D28;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_8005D9E4();
    tmp = *(s16*)((u8*)(u32)sp + 0xA);
    r3 = r3 & 0xFF;
    r3 = r3 + tmp;
    tmp = (s16)tmp;
    *(u16*)(sp + 0xA) = tmp;
L_80008D28:
    r4 = *(s16*)((u8*)(u32)sp + 0xA);
    if ((s32)r4 >= 0) goto L_80008D74;
    r3 = *(s16*)((u8*)(u32)sp + 0x8);
    tmp = 0x0;
    *(u16*)(sp + 0xA) = tmp;
    r3 = r3 + r4;
    tmp = (s16)r3;
    *(u16*)(sp + 0x8) = r3;
    if ((s32)tmp >= 0) goto L_80008DB8;
    r4 = (s16)r29;
    tmp = r30 - r4;
    r3 = (s16)r3;
    tmp = (s16)tmp;
    *(u16*)(sp + 0xA) = r3;
    *(u16*)(sp + 0x8) = tmp;
    goto L_80008DB8;
L_80008D74:
    r3 = (s16)r29;
    if ((s32)r4 < (s32)r3) goto L_80008DB8;
    r3 = *(s16*)((u8*)(u32)sp + 0x8);
    tmp = r4 - r5;
    r3 = r3 + tmp;
    tmp = (s16)r5;
    r3 = (s16)r3;
    *(u16*)(sp + 0xA) = tmp;
    tmp = r3 + tmp;
    *(u16*)(sp + 0x8) = r3;
    if ((s32)tmp < (s32)r30) goto L_80008DB8;
    tmp = 0x0;
    *(u16*)(sp + 0x8) = tmp;
    *(u16*)(sp + 0xA) = tmp;
L_80008DB8:
    r3 = r31 & 0xFFFF;
    tmp = r3 & 0x00000080;
    if ((s32)tmp == 0) goto L_80008EF8;
    r3 = *(s16*)((u8*)(u32)sp + 0x8);
    tmp = *(s16*)((u8*)(u32)sp + 0xA);
    r5 = r3 + tmp;
    if ((s32)r5 >= 0) goto L_80008DE4;
    tmp = 0x0;
    goto L_80008EEC;
L_80008DE4:
    r3 = *(u32*)lbl_80478F98;
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (r5 < tmp) goto L_80008DFC;
    tmp = 0x0;
    goto L_80008EEC;
L_80008DFC:
    r7 = *(s16*)((u8*)r3 + 0x4);
    if ((s32)r5 != 0) {
        r4 = 0x0;
        if ((s32)tmp > 0) {
            if ((s32)tmp > 8) {
                r6 = *(u32*)lbl_80478F9C;
                tmp = r3 + 0x7;
                tmp = (u32)tmp >> 3;
                ctr_fn = (void(*)(void))tmp;
                if ((s32)r3 > 0) {
                    do {
                        tmp = (s16)r7;
                        r4 = r4 + 0x8;
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        r7 = *(s16*)(r6 + tmp);
                    } while (--ctr != 0);
            }
            }
            r6 = *(u32*)lbl_80478F9C;
            tmp = r3 - r4;
            ctr_fn = (void(*)(void))tmp;
            if ((s32)r4 < (s32)r3) {
                do {
                    tmp = (s16)r7;
                    r3 = tmp << 3;
                    tmp = r3 + 0x6;
                    r7 = *(s16*)(r6 + tmp);
                } while (--ctr != 0);
        }
        }
        tmp = (s16)r7;
        r3 = *(u32*)lbl_80478F9C;
        tmp = tmp << 3;
        r3 = r3 + tmp;
        r7 = *(s16*)((u8*)r3 + 0x6);
    }
    tmp = r7 & 0xFFFF;
L_80008EEC:
    r3 = tmp & 0xFFFF;
    fn_80190528();
    goto L_80009030;
L_80008EF8:
    tmp = r3 & 0x00000040;
    if ((s32)tmp == 0) goto L_80009030;
    r3 = *(s16*)((u8*)(u32)sp + 0x8);
    tmp = *(s16*)((u8*)(u32)sp + 0xA);
    r5 = r3 + tmp;
    if ((s32)r5 >= 0) goto L_80008F20;
    tmp = 0x0;
    goto L_80009028;
L_80008F20:
    r3 = *(u32*)lbl_80478F98;
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (r5 < tmp) goto L_80008F38;
    tmp = 0x0;
    goto L_80009028;
L_80008F38:
    r7 = *(s16*)((u8*)r3 + 0x4);
    if ((s32)r5 != 0) {
        r4 = 0x0;
        if ((s32)tmp > 0) {
            if ((s32)tmp > 8) {
                r6 = *(u32*)lbl_80478F9C;
                tmp = r3 + 0x7;
                tmp = (u32)tmp >> 3;
                ctr_fn = (void(*)(void))tmp;
                if ((s32)r3 > 0) {
                    do {
                        tmp = (s16)r7;
                        r4 = r4 + 0x8;
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        r7 = *(s16*)(r6 + tmp);
                    } while (--ctr != 0);
            }
            }
            r6 = *(u32*)lbl_80478F9C;
            tmp = r3 - r4;
            ctr_fn = (void(*)(void))tmp;
            if ((s32)r4 < (s32)r3) {
                do {
                    tmp = (s16)r7;
                    r3 = tmp << 3;
                    tmp = r3 + 0x6;
                    r7 = *(s16*)(r6 + tmp);
                } while (--ctr != 0);
        }
        }
        tmp = (s16)r7;
        r3 = *(u32*)lbl_80478F9C;
        tmp = tmp << 3;
        r3 = r3 + tmp;
        r7 = *(s16*)((u8*)r3 + 0x6);
    }
    tmp = r7 & 0xFFFF;
L_80009028:
    r3 = tmp & 0xFFFF;
    fn_801903B0();
L_80009030:
    r3 = *(s16*)((u8*)(u32)sp + 0x8);
    tmp = *(s16*)((u8*)(u32)sp + 0xA);
    r5 = r3 + tmp;
    *(u32*)lbl_80478838 = r4;
    if ((s32)r5 >= 0) goto L_80009054;
    tmp = 0x0;
    goto L_8000915C;
L_80009054:
    r3 = *(u32*)lbl_80478F98;
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (r5 < tmp) goto L_8000906C;
    tmp = 0x0;
    goto L_8000915C;
L_8000906C:
    r7 = *(s16*)((u8*)r3 + 0x4);
    if ((s32)r5 != 0) {
        r4 = 0x0;
        if ((s32)tmp > 0) {
            if ((s32)tmp > 8) {
                r6 = *(u32*)lbl_80478F9C;
                tmp = r3 + 0x7;
                tmp = (u32)tmp >> 3;
                ctr_fn = (void(*)(void))tmp;
                if ((s32)r3 > 0) {
                    do {
                        tmp = (s16)r7;
                        r4 = r4 + 0x8;
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        tmp = *(s16*)(r6 + tmp);
                        r3 = tmp << 3;
                        tmp = r3 + 0x6;
                        r7 = *(s16*)(r6 + tmp);
                    } while (--ctr != 0);
            }
            }
            r6 = *(u32*)lbl_80478F9C;
            tmp = r3 - r4;
            ctr_fn = (void(*)(void))tmp;
            if ((s32)r4 < (s32)r3) {
                do {
                    tmp = (s16)r7;
                    r3 = tmp << 3;
                    tmp = r3 + 0x6;
                    r7 = *(s16*)(r6 + tmp);
                } while (--ctr != 0);
        }
        }
        tmp = (s16)r7;
        r3 = *(u32*)lbl_80478F9C;
        tmp = tmp << 3;
        r3 = r3 + tmp;
        r7 = *(s16*)((u8*)r3 + 0x6);
    }
    tmp = r7 & 0xFFFF;
L_8000915C:
    tmp = tmp & 0xFFFF;
    *(u32*)((u8*)r28 + 0x80) = tmp;
    return;
}
