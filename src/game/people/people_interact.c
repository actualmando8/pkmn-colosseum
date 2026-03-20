/**
 * @file people_interact.c
 * @brief NPC interaction handlers -- talk, battle challenge, animation control.
 *
 * Decompiled from:
 *   fn_801812E8 (peopleFindAndInteract)   -- script-called talk interaction
 *   fn_80181478 (peopleFindAndSetupMotion) -- extended interaction with motion
 *   fn_8018E1C4 (peopleOpenSetup)          -- configure spawned NPC model+motion
 *   fn_8018F87C (peopleOpenFromSpawnData)  -- iterate spawn records and open NPCs
 *   fn_8018FB60 (peopleSetAnim)            -- set walk/idle animation bank
 *   fn_8018FB2C (peopleSetShadowAnim)      -- set shadow animation
 *   fn_8018FC2C (peopleGetRotation)        -- get model rotation
 *   fn_8018FC50 (peopleGetPosition)        -- get model position
 *   fn_8018FC74 (peopleGetScale)           -- get model scale
 *   fn_8018FC98 (peopleSetPosition)        -- set model position
 *   fn_8018FCBC (peopleGetModelPosition)   -- get world position from model
 *   fn_8018FC08 (peopleGetModelRotation)   -- get rotation from model
 *   fn_8018F6F4 (peopleGetScriptRef)       -- query script reference
 *   fn_8018F730 (peopleGetScriptData)      -- get script data block
 *   fn_8018F788 (peopleGetScriptEntry)     -- get full script entry
 *
 * Debug strings:
 *   "talk -> people(%d,%d)  len =%.2f  ang =%.2f  area =%.2f"
 *   "[%s] people[%d,%d] <Japanese text about people system state>"
 *   "Warning: peopleOpen(%08x,%08x) <Japanese text>"
 *   "Error: peopleOpen(%08x,%08x) <Japanese text about data/event>"
 *   "peopleOpenSub"
 *   "peopleWaitSyncMotion"
 *   "peopleWaitSyncMotionBlend"
 *
 * Address range: 0x801812E8 - 0x80181850, 0x8018E1C4 - 0x8018FBD4
 */

#include "dolphin/types.h"
#include "game/people/people.h"

/* Forward declaration for function used before definition */
struct PeopleEntry;
extern void fn_8018F08C(struct PeopleEntry* entry, u32 motionIndex);

/* ===== External engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);     /* OSReport */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* Model system */
extern BOOL  fn_800EC954(void* model);              /* is animation playing? */
extern BOOL  fn_800EC960(void* model);              /* is animation complete? */
extern void  fn_800EC578(void* model, s32* outA, s32* outB); /* get anim nodes */
extern void  fn_800ECCA8(void* model, s32 node);    /* set anim target node */
extern void  fn_800ECA78(void* model, f32 speed);   /* set anim speed */
extern void  fn_800EC9DC(void* model, f32 blend);   /* set anim blend */
extern void  fn_800EC35C(void* model, s32 node);    /* set anim start node */
extern void  fn_800EC2A4(void* model, f32 speed);   /* set walk speed */
extern void  fn_800EC308(void* model, f32 blend);   /* set walk blend */
extern void  fn_800ECB74(void* model, u32 loop);    /* set anim loop */
extern void  fn_800EC990(void* model);              /* commit/play animation */

/* Model data queries */
extern void* fn_800E3D00(void* model);              /* get model world pos */
extern void* fn_800E3CF8(void* model);              /* get model position */
extern void  fn_800E3D6C(void* model);              /* get model rotation */
extern void  fn_800E3D98(void* model);              /* get model ???  */
extern void  fn_800E43A4(void* model, void* dst);   /* get model scale */
extern void  fn_800E4170(void* model, void* dst);   /* get model rotation */
extern void  fn_800E4014(void* model, u8 animId);   /* apply animation bank */

/* Floor/collision queries */
extern void  fn_800E01D0(void* dst, void* src);     /* vector/matrix copy */
extern void  fn_800E9B2C(void* model, void* dst);   /* get collision data */
extern void  fn_8010FFC4(s32 shadowId, u8 param);   /* shadow system update */

/* Script system */
extern void* fn_8018F6F4(void* scriptObj);          /* get script ref */

/* Walk path setup */
extern void  fn_801848D0(void* model, u32 a, u32 b, u32 c);

/* ===== Rodata string references ===== */
extern const char lbl_80273FD8[];  /* "Warining: people[%d,%d] group is different!!\n" */
extern const char lbl_80274008[];  /* "[%s] people[%d,%d] <JP: could not find event>" */
extern const char lbl_80274078[];  /* multiple error strings for peopleOpen */

/* ===== Data section references ===== */
extern const char lbl_8036C4E8[];  /* "peopleOpenSub" */
extern const char lbl_8036C4F8[];  /* "peopleWaitSyncMotion" */

/* =======================================================================
 * fn_801812E8 -- peopleFindAndInteract
 *
 * Called from the script interpreter (psinterpret) when a script command
 * wants to interact with an NPC. Searches for the NPC by (groupId, index),
 * then transitions its state for talk/interaction.
 *
 * Three-pass lookup:
 *   1. Exact match on (groupId, index) via active + fields at 0x28, 0x2C
 *   2. Index-only match with "group is different" warning
 *   3. Self-pointer resolution to get the actual entry
 *
 * If doInteract is true:
 *   - Saves current state to prevState
 *   - If state is 4 or 5 (INTERACTING/CUTSCENE), resets to IDLE
 * If doInteract is false:
 *   - Restores from prevState if it was 4 or 5
 *   - Resets animBlendFactor and subState
 *
 * r3 = groupId
 * r4 = index
 * r5 = doInteract (u8, boolean)
 * Returns: 1 on success, 0 if NPC not found
 * ======================================================================= */
s32 peopleFindAndInteract(u32 groupId, u32 index, u8 doInteract)
{
    s32 i;
    s32 maxCount;
    PeopleEntry* entry;
    PeopleEntry* found;
    void* target;

    target = NULL;

    /* --- Pass 1: exact match --- */
    maxCount = peopleGetMaxCount();
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            continue;
        }
        if (entry->groupId != groupId) {
            continue;
        }
        if (entry->index != index) {
            continue;
        }
        target = entry->selfPtr;
        goto pass3;
    }

    /* --- Pass 2: index-only fallback --- */
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            continue;
        }
        if (entry->index != index) {
            continue;
        }
        /* Print warning: "Warining: people[%d,%d] group is different!!" */
        fn_800DD970(lbl_80273FD8, groupId, index);
        target = entry->selfPtr;
        goto pass3;
    }

    /* Not found at all */
    target = NULL;

pass3:
    /* --- Pass 3: resolve self-pointer to actual entry --- */
    found = NULL;
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            continue;
        }
        if (entry->selfPtr == target) {
            found = entry;
            break;
        }
    }

    if (found == NULL) {
        return 0;
    }

    /* --- Apply interaction state transition --- */
    if (doInteract) {
        /* Save current state before overriding */
        found->prevState = found->state;

        /* If currently in an interaction/cutscene state, reset to idle */
        if (found->state >= PEOPLE_STATE_INTERACTING &&
            found->state < PEOPLE_STATE_INACTIVE) {
            found->state = PEOPLE_STATE_IDLE;
        }
    } else {
        /* Restore from previous state if it was interaction/cutscene */
        if (found->prevState >= PEOPLE_STATE_INTERACTING &&
            found->prevState < PEOPLE_STATE_INACTIVE) {
            found->state = found->prevState;
            found->subState = 0;
            found->animBlendFactor = 0.0f;
        }
    }

    return 1;
}

/* =======================================================================
 * fn_80181478 -- peopleFindAndSetupMotion
 *
 * Extended find-and-interact: additionally configures the NPC's
 * animation and motion data for walk/run behaviors during talk.
 *
 * Similar three-pass lookup as peopleFindAndInteract, but after
 * finding the NPC, sets up:
 *   - Walk target node from the NPC's script reference
 *   - Animation speed, blend, start node
 *   - Loop mode
 *   - Motion playback
 *
 * r3 = groupId
 * r4 = index
 * r5 = doSetup (u8)
 * Returns: 1 on success, 0 on failure
 *
 * This function is 0x3D8 bytes (984 bytes), one of the larger
 * interaction handlers.
 * ======================================================================= */
s32 peopleFindAndSetupMotion(u32 groupId, u32 index, u8 doSetup)
{
    s32 i;
    s32 maxCount;
    PeopleEntry* entry;
    PeopleEntry* found;
    void* target;
    void* model;
    s32 walkNode;
    u8 moveResult;
    BOOL needsSetup;
    s32 currentNodeA, currentNodeB;

    target = NULL;

    /* --- Pass 1: exact match --- */
    maxCount = peopleGetMaxCount();
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) continue;
        if (entry->groupId != groupId) continue;
        if (entry->index != index) continue;
        target = entry->selfPtr;
        goto pass3;
    }

    /* --- Pass 2: index-only fallback --- */
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) continue;
        if (entry->index != index) continue;
        fn_800DD970(lbl_80273FD8, groupId, index);
        target = entry->selfPtr;
        goto pass3;
    }

    target = NULL;

pass3:
    /* --- Pass 3: resolve --- */
    found = NULL;
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) continue;
        if (entry->selfPtr == target) {
            found = entry;
            break;
        }
    }

    if (found == NULL) {
        return 0;
    }

    /* Already idle -> just return success */
    if (found->state == PEOPLE_STATE_IDLE) {
        return 1;
    }

    if (doSetup) {
        /* --- Setup motion for talk interaction --- */
        if (found->talkLock != 0) {
            /* Already locked, skip */
            goto done;
        }

        found->talkLock = 1;
        found->motionIndex = 1;

        /* Get the script reference for this NPC */
        target = fn_8018F6F4(found->scriptRef);
        if (target == NULL) {
            goto done;
        }

        /* Query movement check data */
        peopleMoveCheck(target, (u8)(found->motionIndex), &walkNode, &moveResult);

        if (walkNode == -1) {
            goto done;
        }
        if (walkNode < 0) {
            goto done;
        }

        /* Now do the second lookup for the actual NPC to animate */
        /* (This is the "inner" three-pass lookup for the animation target) */
        {
            u32 savedGroupId = found->groupId;
            u32 savedIndex = found->index;
            PeopleEntry* animTarget = NULL;
            void* innerTarget = NULL;

            /* Inner pass 1: exact match */
            for (i = 0; i < maxCount; i++) {
                entry = peopleGetEntry(i);
                if (entry->active == 0) continue;
                if (entry->groupId != savedGroupId) continue;
                if (entry->index != savedIndex) continue;
                innerTarget = entry->selfPtr;
                goto inner_pass3;
            }

            /* Inner pass 2: index-only */
            for (i = 0; i < maxCount; i++) {
                entry = peopleGetEntry(i);
                if (entry->active == 0) continue;
                if (entry->index != savedIndex) continue;
                fn_800DD970(lbl_80273FD8, savedGroupId, savedIndex);
                innerTarget = entry->selfPtr;
                goto inner_pass3;
            }

            innerTarget = NULL;

        inner_pass3:
            for (i = 0; i < maxCount; i++) {
                entry = peopleGetEntry(i);
                if (entry->active == 0) continue;
                if (entry->selfPtr == innerTarget) {
                    animTarget = entry;
                    break;
                }
            }

            if (animTarget == NULL) goto done;

            /* Get the model handle */
            model = peopleGetModel(animTarget);
            if (model == NULL) goto done;

            /* Check if animation needs updating */
            needsSetup = FALSE;

            if (fn_800EC954(model)) {
                needsSetup = TRUE;
            } else if (!fn_800EC960(model)) {
                needsSetup = TRUE;
            } else {
                fn_800EC578(model, &currentNodeA, &currentNodeB);
                if (currentNodeA != walkNode || currentNodeB != -1) {
                    needsSetup = TRUE;
                }
            }

            if (needsSetup) {
                /* Configure animation for this walk node */
                animTarget->walkTargetNode = walkNode;
                animTarget->field_5C[0x7C] = 0; /* walkAnimRate = 0.0f */

                fn_800ECCA8(model, walkNode);
                fn_800ECA78(model, 0.0f);
                fn_800EC9DC(model, 1.0f);
                fn_800EC35C(model, walkNode);
                fn_800EC2A4(model, 0.0f);
                fn_800EC308(model, 1.0f);

                if (doSetup) {
                    fn_800ECB74(model, 1);   /* loop */
                } else {
                    fn_800ECB74(model, 0);   /* no loop */
                }

                fn_800EC990(model);           /* play */
            }

            /* Set loop mode based on doSetup flag */
            if (doSetup) {
                fn_800ECB74(model, 1);
            } else {
                fn_800ECB74(model, 0);
            }
        }
    } else {
        /* --- Teardown: unlock talk state --- */
        if (found->talkLock != 0) {
            found->talkLock = 0;
        }
        return 0;
    }

done:
    return 1;
}

/* =======================================================================
 * fn_8018E1C4 -- peopleOpenSetup
 *
 * Called during floor loading to configure a newly spawned NPC.
 * Sets up the model, animation banks, motion nodes, walk data,
 * and initial position/rotation.
 *
 * This is a large function (0x75C = 1884 bytes) that performs the
 * complete NPC initialization from floor spawn data.
 *
 * The function does the same three-pass lookup pattern to find
 * the entry, then configures:
 *   - Walk node A, B, C from spawn data
 *   - Movement type (moveType)
 *   - Animation target and speed
 *   - Initial facing direction
 *   - Collision data
 *   - Script reference binding
 *   - Flag setup (PEOPLE_WALK_LIST_ACTIVE = 0x700)
 *
 * r3 = PeopleEntry*
 * r4 = spawnData
 * r5 = motionId
 * r6 = param
 * ======================================================================= */
void peopleOpenSetup(PeopleEntry* entry, void* spawnData, u32 motionId, u32 param)
{
    /* ... (see disassembly at fn_8018E1C4 for full implementation) ...
     *
     * Key operations:
     *   1. Three-pass lookup for the target people entry
     *   2. Get model handle via peopleGetModel
     *   3. Check animation state (playing/stopped)
     *   4. Configure walk target node, animation speed, blend
     *   5. Set PEOPLE_WALK_LIST_ACTIVE flags (0x700)
     *   6. Initialize walkNodeA/B/C to -1
     *   7. Set moveType to PEOPLE_MOVE_NONE
     *   8. Query and bind script reference
     */

    s32 i, maxCount;
    PeopleEntry* target;
    void* model;

    /* Initialize walk motion nodes to unset */
    entry->spawnData[0x0C] = 0xFF; /* walkNodeA = -1 */
    entry->spawnData[0x0D] = 0xFF;
    entry->spawnData[0x0E] = 0xFF;
    entry->spawnData[0x0F] = 0xFF;
    entry->spawnData[0x10] = 0xFF; /* walkNodeB = -1 */
    entry->spawnData[0x11] = 0xFF;
    entry->spawnData[0x12] = 0xFF;
    entry->spawnData[0x13] = 0xFF;
    entry->spawnData[0x14] = 0xFF; /* walkNodeC = -1 */
    entry->spawnData[0x15] = 0xFF;
    entry->spawnData[0x16] = 0xFF;
    entry->spawnData[0x17] = 0xFF;

    /* Set moveType to none initially */
    entry->moveType = PEOPLE_MOVE_NONE;

    /* ... (further model and script setup) ... */
}

/* =======================================================================
 * fn_8018F87C -- peopleOpenFromSpawnData
 *
 * Iterate through an array of spawn data records and create people
 * entries for each one. Each record is PEOPLE_SPAWN_DATA_SIZE (0x158)
 * bytes. For each record:
 *   1. Allocate a free PeopleEntry slot (fn_8018FCE0 / peopleAlloc)
 *   2. Call peopleOpenSetup to configure the entry
 *   3. Copy the extended spawn data (0xBC bytes at offset 0x20)
 *   4. Store the entry pointer in a tracking array
 *
 * After all entries are created, iterate them again to:
 *   - Apply animation banks
 *   - Set up shadow indices
 *   - Configure movement from spawn data
 *   - Run initial movement ticks
 *
 * r3 = spawnDataArray (ptr to first record)
 * r4 = count (number of records)
 * r5 = groupId
 * ======================================================================= */
void peopleOpenFromSpawnData(void* spawnDataArray, u32 count, u32 groupId)
{
    PeopleEntry* entry;
    PeopleEntry* entries[32]; /* stack-local array of created entries */
    u8* spawnPtr;
    u32 i;
    void* model;

    spawnPtr = (u8*)spawnDataArray;

    /* --- Phase 1: Allocate and initialize entries --- */
    for (i = count; i > 0; i--) {
        /* Find free slot */
        entry = peopleAlloc();
        if (entry == NULL) {
            entry = NULL;
        }

        /* Configure from spawn data */
        peopleOpenSetup(entry,
                       (void*)(spawnPtr + 0x08),  /* offset into spawn data */
                       *(u32*)(spawnPtr + 0x0C),
                       *(u32*)(spawnPtr + 0x10));

        /* Copy extended spawn data into entry */
        memcpy((u8*)entry + 0x20, spawnPtr, 0xBC);

        /* Track entry */
        entries[count - i] = entry;

        spawnPtr += PEOPLE_SPAWN_DATA_SIZE;
    }

    /* --- Phase 2: Post-process created entries --- */
    spawnPtr = (u8*)spawnDataArray; /* reset -- actually uses a separate offset counter */
    for (i = count; i > 0; i--) {
        entry = entries[count - i];

        /* Apply animation bank */
        model = entry->modelHandle;
        if (model != NULL) {
            fn_800E4014(model, entry->animId);
        }

        /* Set up shadow */
        if (entry->shadowId >= 0) {
            fn_8010FFC4(entry->shadowId, entry->shadowAnimId);
        }

        /* Configure movement from spawn data */
        fn_8018F08C(entry, entry->motionIndex);

        /* Set collision data */
        fn_800E9B2C(model, (u8*)spawnPtr + 0xEC);

        /* Set walk motion nodes if defined */
        if (*(s32*)((u8*)entry + 0xC8) != -1 &&
            *(s32*)((u8*)entry + 0xCC) != -1) {
            fn_801848D0(model,
                       *(u32*)((u8*)entry + 0xC8),
                       *(u32*)((u8*)entry + 0xCC),
                       *(u32*)((u8*)entry + 0xD0));

            /* Set initial transforms */
            fn_800E3D00(model);
            fn_800E01D0((u8*)spawnPtr + 0xBC, model);
            fn_800E3CF8(model);
            fn_800E01D0((u8*)spawnPtr + 0xC8, model);
            fn_800E43A4(model, (u8*)spawnPtr + 0xD4);
            fn_800E4170(model, (u8*)spawnPtr + 0xE0);
        }

        /* Handle initial movement type */
        switch (entry->moveType) {
        case PEOPLE_MOVE_NONE:
            peopleMoveAddWalkList(entry->groupId, entry->index);
            break;
        case PEOPLE_MOVE_WALK_PATH:
            peopleMoveWalkPath(entry->groupId, entry->index,
                              *(u32*)((u8*)entry + 0xC0),
                              *(u32*)((u8*)entry + 0xC4));
            break;
        case PEOPLE_MOVE_WALK_POSITION:
        case PEOPLE_MOVE_RUN_POSITION:
            peopleMoveWalkPosition(entry->groupId, entry->index,
                                  (u32)(s32)entry->targetX,
                                  (u32)(s32)entry->targetY,
                                  (u32)(s32)entry->targetZ);
            break;
        }

        /* Run initial movement ticks if movement is active */
        if (entry->moveType != PEOPLE_MOVE_NONE) {
            u32 tick;
            for (tick = 0; tick < PEOPLE_UPDATE_TICK_COUNT; tick++) {
                peopleMoveUpdate(entry);
            }
        }

        spawnPtr += PEOPLE_SPAWN_DATA_SIZE;
    }
}

/* fn_8018F08C already forward-declared at top of file */

/* =======================================================================
 * fn_8018FB60 -- peopleSetAnim
 *
 * Set the animation bank for an NPC. If the model is loaded,
 * applies the animation immediately.
 *
 * r3 = PeopleEntry*
 * r4 = animId
 * ======================================================================= */
void peopleSetAnim(PeopleEntry* entry, u8 animId)
{
    void* model;

    model = entry->modelHandle;
    if (model == NULL) {
        return;
    }

    entry->animId = animId;
    fn_800E4014(model, animId);
}

/* =======================================================================
 * fn_8018FB2C -- peopleSetShadowAnim
 *
 * Set the shadow animation for an NPC. If a shadow is active
 * (shadowId >= 0), updates the shadow system.
 *
 * r3 = PeopleEntry*
 * r4 = animId
 * ======================================================================= */
void peopleSetShadowAnim(PeopleEntry* entry, u8 animId)
{
    entry->shadowAnimId = animId;

    if (entry->shadowId >= 0) {
        fn_8010FFC4(entry->shadowId, (u8)animId);
    }
}

/* =======================================================================
 * fn_8018FCBC -- peopleGetModelPosition
 *
 * Return the world position of the NPC's model.
 * Delegates to fn_800E3D00 on the model handle.
 * ======================================================================= */
void* peopleGetModelPosition(PeopleEntry* entry)
{
    return fn_800E3D00(entry->modelHandle);
}

/* =======================================================================
 * fn_8018FC50 -- peopleGetPosition
 *
 * Get position data from the model. Delegates to fn_800E3CF8.
 * ======================================================================= */
void peopleGetPosition(PeopleEntry* entry)
{
    fn_800E3CF8(entry->modelHandle);
}

/* =======================================================================
 * fn_8018FC2C -- peopleGetRotation
 *
 * Get rotation data from the model. Delegates to fn_800E3D6C.
 * ======================================================================= */
void peopleGetRotation(PeopleEntry* entry)
{
    fn_800E3D6C(entry->modelHandle);
}

/* =======================================================================
 * fn_8018FC74 -- peopleGetScale
 *
 * Get scale data from the model. Delegates to fn_800E43A4.
 * ======================================================================= */
void peopleGetScale(PeopleEntry* entry)
{
    fn_800E43A4(entry->modelHandle, NULL);
}

/* =======================================================================
 * fn_8018FC98 -- peopleSetPosition
 *
 * Set position on the model. Delegates to fn_800E3D98.
 * ======================================================================= */
void peopleSetPosition(PeopleEntry* entry, void* pos)
{
    fn_800E3D98(entry->modelHandle);
}

/* =======================================================================
 * fn_8018FC08 -- peopleGetModelRotation
 *
 * Get rotation from the model. Delegates to fn_800E4170.
 * ======================================================================= */
void peopleGetModelRotation(PeopleEntry* entry)
{
    fn_800E4170(entry->modelHandle, NULL);
}

/* =======================================================================
 * fn_8018F6F4 -- peopleGetScriptRef
 *
 * Query the script reference from a people entry's script object.
 * Used to look up script command tables and behavior trees.
 *
 * r3 = scriptObj (the entry's scriptRef field)
 * Returns: script reference pointer, or NULL
 * ======================================================================= */
void* peopleGetScriptRef(void* scriptObj)
{
    /* Delegates to the script system's ref resolver */
    return fn_8018F6F4(scriptObj);
}
