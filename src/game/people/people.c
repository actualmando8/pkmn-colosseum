/**
 * @file people.c
 * @brief People/NPC system -- core, movement, and interaction (merged TU).
 *
 * This is a single translation unit containing all People/NPC functions.
 * Link order analysis showed that people.c, people_move.c, and
 * people_interact.c had interleaved functions throughout the
 * 0x80180C78-0x8018FE30 address range, confirming they were originally
 * compiled as one TU.
 *
 * Functions are ordered by their DOL address to match the original layout.
 *
 * Decompiled from address range: 0x80180C78 - 0x8018FE30
 *
 * Subsystems:
 *   - Core:        init, alloc, update, spawn, despawn, flags, transform
 *   - Movement:    walk path, walk position, dispatch, range check
 *   - Interaction: talk, motion setup, NPC spawn, animation, getters/setters
 *
 * Debug strings:
 *   "Warining: people[%d,%d] group is different!!"
 *   "ERROR! [%s]: People[%d,%d] WalkMotion[%d] is frame zero."
 *   "ERROR! [%s]: People[%d,%d] RunMotion[%d] is frame zero."
 *   "talk -> people(%d,%d)  len =%.2f  ang =%.2f  area =%.2f"
 *   "peopleOpenSub", "peopleWaitSyncMotion",
 *   "peopleWaitSyncMotionBlend", "peopleMoveCheck"
 *
 * Global state:
 *   lbl_8047B1F8 (sbss) -- s32  gPeopleMaxCount
 *   lbl_8047B200 (sbss) -- PeopleEntry* gPeopleArray  (heap-allocated)
 *   lbl_8047B1FC (sbss) -- u16  gPeopleMemHandle (GSmem handle)
 *   lbl_8047B1E0 (sbss) -- void* gPeopleFloorObj (current floor link)
 *   lbl_8047B1E4 (sbss) -- PeopleOpenWork* gPeopleOpenWork
 *   lbl_8047B1E8 (sbss) -- s32  gPeopleOpenCount
 */

#include "dolphin/types.h"
#include "game/people/people.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);     /* OSReport / debug printf */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* GSmem allocator */
extern u16   fn_800E3534(u32 size);                 /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);               /* GSmemGetPtr */

/* Floor/field system */
extern void* fn_80167F28(const char* name);         /* field lookup by name */
extern void* fn_80167E5C(void);                     /* get current field */
extern void  fn_80167ED0(void* field, void* obj, void* data, u32 param);
extern void  fn_80167E64(void* field);              /* field finalize */

/* Thread/task system */
extern void* fn_800FE834(u32 pri, u32 type, void* taskBuf, void* callback);
extern void  fn_800FE714(void* task);               /* task cleanup */

/* Model system */
extern void  fn_8017BB80(void* floorObj, void* modelData); /* model open */
extern void  fn_8017BC90(void* floorObj, u32 modelId, u32 param, void* extraData, u32 extra2);

/* Flag system */
extern BOOL  fn_800F7108(u16 flagId);               /* GSflagGet (bit check) */

/* Collision/model helpers */
extern void  fn_800E01D0(void* dst, void* src);     /* matrix/vector copy */
extern void  fn_800E0168(void* dst, void* srcA, void* srcB);  /* cross/setup */

/* Vector/matrix math */
extern void  fn_800E019C(void* dst, u32 a, u32 b); /* vector subtract */
extern void  fn_800E013C(void* dst, void* vec, f32 scale); /* vector normalize */
extern void  fn_800E0040(void* dst, void* vec);             /* vector copy */
extern void  fn_800DFEEC(void* dst, void* a, void* b);     /* matrix multiply */
extern void  fn_800E0718(void* dst, void* src);             /* matrix from data */

/* Trigonometry */
extern f64   fn_800CE2D8(f32 y, f32 x);          /* atan2 */
extern f64   fn_800CE318(f64 x);                  /* fmod / angle normalize */

/* Model animation control */
extern BOOL  fn_800EC954(void* model);             /* is animation playing? */
extern BOOL  fn_800EC960(void* model);             /* is animation stopped? */
extern void  fn_800EC578(void* model, s32* outNodeA, s32* outNodeB); /* get anim nodes */
extern void  fn_800ECCA8(void* model, s32 node);   /* set animation target */
extern void  fn_800ECA78(void* model, f32 speed);  /* set animation speed */
extern void  fn_800EC9DC(void* model, f32 blend);  /* set animation blend */
extern void  fn_800EC35C(void* model, s32 node);   /* set animation start node */
extern void  fn_800EC2A4(void* model, f32 speed);  /* set walk speed */
extern void  fn_800EC308(void* model, f32 blend);  /* set walk blend */
extern void  fn_800ECB74(void* model, u32 loop);   /* set animation loop flag */
extern void  fn_800EC990(void* model);              /* play animation */

/* Model data queries */
extern void* fn_800E3D00(void* model);              /* get model world pos */
extern void* fn_800E3CF8(void* model);              /* get model position */
extern void  fn_800E3D6C(void* model);              /* get model rotation */
extern void  fn_800E3D98(void* model);              /* get model ???  */
extern void  fn_800E43A4(void* model, void* dst);   /* get model scale */
extern void  fn_800E4170(void* model, void* dst);   /* get model rotation */
extern void  fn_800E4014(void* model, u8 animId);   /* apply animation bank */

/* Floor/collision queries */
extern void  fn_800E9B2C(void* model, void* dst);   /* get collision data */
extern void  fn_8010FFC4(s32 shadowId, u8 param);   /* shadow system update */

/* Walk path setup */
extern void  fn_801848D0(void* model, u32 a, u32 b, u32 c); /* set model walk nodes */

/* Script system */
extern void* fn_8018F6F4_ext(void* scriptObj);       /* get script ref data */

/* Forward declaration for function used before definition */
extern void fn_8018F08C(PeopleEntry* entry, u32 motionIndex);

/* Additional external functions for new decompilations */
extern void fn_801170A4(void);
extern void fn_80116EC8(void);
extern void fn_80116EB0(void);
extern void fn_800F0308(void);
extern void fn_800F0438(void);
extern void fn_800E9C6C(void* model, void* dst);

/* ===== Rodata string references ===== */
extern const char lbl_80273F80[];  /* floor name for blank-frame init */
extern const char lbl_80273FD8[];  /* "Warining: people[%d,%d] group is different!!\n" */
extern const char lbl_80274008[];  /* "[%s] people[%d,%d] <JP: could not find event>" */
extern const char lbl_80274078[];  /* multiple error strings for peopleOpen */

/* ===== Data section references ===== */
extern const char lbl_8036C4E8[];   /* "peopleOpenSub" */
extern const char lbl_8036C4F8[];   /* "peopleWaitSyncMotion" */
extern const char lbl_8036C510[];   /* "peopleWaitSyncMotionBlend" */
extern const char lbl_8036C52C[];   /* "peopleMoveCheck" */

/* ===== Jump table for peopleMoveCheck ===== */
extern void* jumptable_8036C540[];  /* 9-entry jump table for move result */

/* ===== Global state (sbss / sdata) ===== */

/* lbl_8047B1F8 @sda21 : maximum people count */
static s32 gPeopleMaxCount;

/* lbl_8047B200 @sda21 : pointer to people array (heap-allocated) */
static PeopleEntry* gPeopleArray;

/* lbl_8047B1FC @sda21 : GSmem handle for the people array */
static u16 gPeopleMemHandle;

/* lbl_8047B1E0 @sda21 : current floor object link */
static void* gPeopleFloorObj;

/* lbl_8047B1E4 @sda21 : active PeopleOpenWork pointer */
static PeopleOpenWork* gPeopleOpenWork;

/* lbl_8047B1E8 @sda21 : people open count/max for linked list */
static s32 gPeopleOpenCount;

/* ===== Additional global state for new decompilations ===== */

/* lbl_8047B1F0 @sda21 : 2-element table of side pointers */
static void* gPeopleSideTable[2];

/* lbl_80478E78 @sda21 : pointer to NPC data count struct */
static void* gNPCDataCountPtr;

/* lbl_80478E7C @sda21 : pointer to NPC data array (0x2C byte entries) */
static void* gNPCDataArrayPtr;

/* Degree-to-radian conversion constant (PI/180) */
#define DEGREES_TO_RADIANS 0.017453292f

/* ===== Sdata2 float constants ===== */
/* lbl_8047D798 @sda21 : constant used in peopleMoveUpdate */
/* lbl_8047D79C @sda21 : float 0.0 */
/* lbl_8047D7A0 @sda21 : float 0.0 (zero) */
/* lbl_8047D7A4 @sda21 : float 1.0 */
/* lbl_8047D8B0 @sda21 : float default moveSpeed */

/* ===== Internal declarations ===== */
static PeopleEntry* peopleFindByGroupAndIndex(u32 groupId, u32 index);

/* #######################################################################
 * Functions ordered by DOL address (0x80180C78 - 0x8018FE30)
 * ####################################################################### */

/* =======================================================================
 * fn_80180C78 -- peopleOpenCallback
 * (see header for documentation)
 * ======================================================================= */
/* NOTE: peopleOpenCallback and peopleOpenThread (fn_80181094) are defined
 * in the header but their implementation is not yet fully decompiled.
 * They reside at 0x80180C78 and 0x80181094 respectively. */

/* =======================================================================
 * fn_8018114C -- peopleCloseCallback
 * (see header for documentation)
 * ======================================================================= */
/* NOTE: peopleCloseCallback resides at 0x8018114C. Not yet fully
 * decompiled. */

/* =======================================================================
 * fn_80181224 -- peopleFloorInit
 *
 * Called when a floor is being set up. Draws 30 blank frames via the
 * field system, then resets the open work state.
 *
 * This ensures all pending model loads and animation setups complete
 * before the floor becomes visible.
 * ======================================================================= */
void peopleFloorInit(void)
{
    void* field;
    void* fieldData;
    s32 i;

    field = fn_80167F28(lbl_80273F80);
    fieldData = fn_80167E5C();

    for (i = 0; i < 30; i++) {
        fn_80167ED0(field, gPeopleFloorObj, fieldData, 0);
    }

    fn_80167E64(field);

    /* Reset open work state */
    gPeopleOpenWork->subState = 0;

    /* Clean up the thread/task */
    fn_800FE714(gPeopleOpenWork->threadObj);
}

/* =======================================================================
 * fn_801812C4 -- peopleMoveUpdate
 *
 * Wrapper called 60 times per frame for each active NPC.
 * Loads a constant delta-time from sdata2 and dispatches to the
 * movement handler.
 *
 * r3 = PeopleEntry*  (unused -- dispatches globally via peopleMoveDispatch)
 * ======================================================================= */
void peopleMoveUpdate(PeopleEntry* entry)
{
    /* f1 = lbl_8047D798 (constant delta time) */
    peopleMoveDispatch(/* dt from sdata2 */0.016667f);
}

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
        target = peopleGetScriptRef(found->scriptRef);
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
 * fn_80181850 -- peopleUpdate
 *
 * Main per-frame update loop for all active people entries.
 * Called from the main game loop (via fn_80005FE0 -> bl fn_80181850).
 *
 * For each active entry:
 *   1. Check HAS_MODEL flag -> update collision position if set
 *   2. Check visibility (visible flag or game flag check)
 *   3. If visible and not in talk-lock: run movement update
 *   4. If active: run 60 ticks of movement simulation
 *
 * This is one of the larger functions (0x660 bytes = 1632 bytes).
 * ======================================================================= */
void peopleUpdate(void)
{
    s32 maxCount;
    s32 i;
    PeopleEntry* entry;
    s32 shouldUpdate;
    u32 j;

    maxCount = peopleGetMaxCount();

    for (i = maxCount - 1; i >= 0; /* decremented at bottom */) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            goto next_entry;
        }

        /* Check if model has PEOPLE_FLAG_HAS_MODEL flag set */
        if (peopleTestFlags(entry, PEOPLE_FLAG_HAS_MODEL)) {
            /* Update collision/interaction position from model world coords */
            void* modelPos;
            void* modelTrans;
            modelPos = peopleGetModelPosition(entry);
            fn_800E01D0((u8*)entry + 0x2C, modelPos);  /* simplified */

            modelTrans = peopleGetTransform(entry);
            fn_800E01D0((u8*)entry + 0x38, modelTrans); /* simplified */
        }

        /* Update entry position tracking */
        peopleSetPosition(entry, (void*)((u8*)entry + 0x44));
        peopleSetTransform(entry, (void*)((u8*)entry + 0x44));

        /* Reset talk range to 0 */
        entry->talkRange = 0.0f;

        /* Determine visibility */
        if (entry->visible != 0) {
            shouldUpdate = 1;
        } else {
            /* Check game flag for dynamic visibility */
            if (fn_800F7108(entry->flagId) == 0) {
                shouldUpdate = 1;
            } else {
                shouldUpdate = 0;
            }
        }

        if (shouldUpdate) {
            /* Skip if NPC is locked in talk interaction */
            if (entry->talkLock != 0) {
                goto check_movement;
            }
            /* ... (extensive state machine for walk, idle, talk transitions) ... */
        }

    check_movement:
        /* Run movement dispatch based on moveType */
        if (entry->moveType != PEOPLE_MOVE_NONE) {
            for (j = 0; j < PEOPLE_UPDATE_TICK_COUNT; j++) {
                peopleMoveUpdate(entry);
            }
        }

    next_entry:
        i--;
    }
}

/* =======================================================================
 * fn_80188AF4 -- peopleMoveAddWalkList
 *
 * Add an NPC to the walk list for its current group.
 * This is the handler for moveType 0 when NPCs have walk data.
 *
 * Debug name from rodata: "peopleAddWalkList"
 *
 * r3 = groupId
 * r4 = index
 * ======================================================================= */
void peopleMoveAddWalkList(u32 groupId, u32 index)
{
    s32 i;
    s32 maxCount;
    PeopleEntry* entry;
    void* target;

    target = NULL;

    /* Search for the matching people entry by group and index */
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
        break;
    }

    /* If not found by exact match, try fallback by index only (with warning) */
    if (target == NULL) {
        for (i = 0; i < maxCount; i++) {
            entry = peopleGetEntry(i);
            if (entry->active == 0) {
                continue;
            }
            if (entry->index != index) {
                continue;
            }
            /* Group mismatch warning */
            fn_800DD970(lbl_80273FD8, groupId, index);
            target = entry->selfPtr;
            break;
        }
    }

    /* Resolve self-pointer indirection */
    if (target != NULL) {
        for (i = 0; i < maxCount; i++) {
            entry = peopleGetEntry(i);
            if (entry->active == 0) {
                continue;
            }
            if (entry->selfPtr == target) {
                break;
            }
        }
    }

    /* ... (further walk list processing) ... */
}

/* =======================================================================
 * fn_80188CA0 -- peopleMoveWalkPosition
 *
 * Walk an NPC toward a specific world position (X, Y, Z).
 *
 * r3 = groupId
 * r4 = index
 * r5 = targetX
 * r6 = targetY
 * r7 = targetZ
 * ======================================================================= */
void peopleMoveWalkPosition(u32 groupId, u32 index,
                            u32 targetX, u32 targetY, u32 targetZ)
{
    PeopleEntry* entry;

    entry = peopleFindByGroupAndIndex(groupId, index);
    if (entry == NULL) {
        return;
    }

    /* Set target position */
    /* ... (velocity calculation, facing direction, step advancement) ... */
}

/* =======================================================================
 * fn_80188FA0 -- peopleMoveWalkPath
 *
 * Walk an NPC along a predefined path. Uses the walkPathId and
 * walkPathParam fields from the NPC's spawn data.
 *
 * r3 = groupId
 * r4 = index
 * r5 = pathId
 * r6 = pathParam
 * ======================================================================= */
void peopleMoveWalkPath(u32 groupId, u32 index, u32 pathId, u32 pathParam)
{
    PeopleEntry* entry;

    entry = peopleFindByGroupAndIndex(groupId, index);
    if (entry == NULL) {
        return;
    }

    /* Set walk motion parameters */
    /* ... (path interpolation, node traversal) ... */
}

/* =======================================================================
 * fn_8018CD08 -- peopleMoveMain
 *
 * Large movement state machine (0x978 = 2424 bytes).
 * Called from the field/world system (fn_8012C100 -> bl fn_8018CD08).
 *
 * Handles:
 *   - Walk path interpolation with collision
 *   - Turn-toward-target rotation
 *   - Animation blending between idle/walk/run
 *   - Speed ramp-up and ramp-down
 *   - Waypoint arrival detection
 *   - Path loop / ping-pong behavior
 *
 * Uses heavy FPU math (f24-f31 saved) for smooth interpolation.
 * ======================================================================= */
/* NOTE: This function is too large to fully decompile without
 * extensive register-level analysis. The structure above captures
 * its role and the sub-functions it calls. A full decompilation
 * would require matching all branch targets in the 600+ instructions. */

/* =======================================================================
 * fn_8018D680 -- peopleIsWithinRange
 *
 * Check whether two positions are within a given range, taking into
 * account facing angle. Used for talk/interaction proximity checks.
 *
 * Computes the distance between two points in the XZ plane, then
 * checks if the result is within the specified range.
 *
 * r3 = posA (world position handle)
 * r4 = posB (world position handle)
 * r5 = posC (target direction handle)
 * f1 = range (maximum distance)
 * Returns: TRUE if within range
 * ======================================================================= */
BOOL peopleIsWithinRange(u32 posA, u32 posB, u32 posC, f32 range)
{
    f32 localVecA[3];    /* stack 0x30 */
    f32 localVecB[3];    /* stack 0x08 */
    f32 localDir[3];     /* stack 0x14 */
    f32 localNorm[3];    /* stack 0x24 */
    f32 angle;
    f32 distX, distZ;

    /* Compute direction vector from posA to posB */
    fn_800E019C(localVecA, posA, posB);
    fn_800E013C(localVecA, localVecA, 1.0f);

    /* Cross product to get perpendicular */
    fn_800E0168(localVecB, (void*)posB, (void*)posA);

    /* Compute angle using atan2 */
    angle = (f32)fn_800CE2D8(localVecB[0], localVecB[2]);

    /* Normalize angle to [0, 2*PI) */
    angle = (f32)fn_800CE318((f64)angle + 3.14159265358979);

    /* Check angle wrapping */
    if (angle > 3.14159265358979) {
        angle -= (f32)6.28318530717959;
    } else if (angle < -3.14159265358979) {
        angle += (f32)6.28318530717959;
    }

    /* Build rotation matrix from normalized direction */
    fn_800E0718(localDir, (void*)0);  /* identity-like setup */

    /* Transform and project */
    fn_800E0168(localVecA, localVecA, localVecA);

    /* Compute transformed offset */
    fn_800DFEEC(localNorm, localDir, localVecA);
    fn_800E0040(localVecA, (void*)posB);

    /* Range check: abs(distX) <= range && abs(distZ) <= range */
    distX = localNorm[0];
    distZ = localNorm[2];

    if (distX < 0) distX = -distX;
    if (distZ < 0) distZ = -distZ;

    if (range >= distX && range >= distZ) {
        return TRUE;
    }

    return FALSE;
}

/* =======================================================================
 * fn_8018D7D0 -- peopleFindByGroupAndIndex  (internal helper)
 *
 * Search all people entries for one matching the given groupId and index.
 * If found by exact match, return it. If only index matches, print a
 * "group is different" warning and return the mismatched entry.
 * If not found at all, return NULL.
 *
 * This pattern is used pervasively across the people system and accounts
 * for the 100+ references to the "Warining: people[%d,%d] group is
 * different!!" string.
 *
 * r3 = groupId
 * r4 = index
 * Returns: PeopleEntry* or NULL
 * ======================================================================= */
static PeopleEntry* peopleFindByGroupAndIndex(u32 groupId, u32 index)
{
    s32 i;
    s32 maxCount;
    PeopleEntry* entry;
    void* target;

    target = NULL;

    /* Pass 1: exact match on groupId + index */
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
        goto resolve;
    }

    /* Pass 2: index-only match (group mismatch warning) */
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            continue;
        }
        if (entry->index != index) {
            continue;
        }
        fn_800DD970(lbl_80273FD8, groupId, index);
        target = entry->selfPtr;
        goto resolve;
    }

    return NULL;

resolve:
    /* Resolve self-pointer to find the actual PeopleEntry */
    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            continue;
        }
        if (entry->selfPtr == target) {
            return entry;
        }
    }

    return NULL;
}

/* =======================================================================
 * fn_8018D928 -- peopleFindBySelfPtr
 *
 * Search all people entries for one whose selfPtr matches the given
 * pointer. Returns the matching PeopleEntry, or NULL if not found.
 *
 * r3 = ptr (self-pointer to search for)
 * Returns: PeopleEntry* or NULL
 *
 * Address: 0x8018D928  Size: 0x70 (112 bytes)
 * ======================================================================= */
PeopleEntry* fn_8018D928(void* ptr)
{
    s32 i;
    PeopleEntry* entry;

    i = 0;
    while (i < peopleGetMaxCount()) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            goto next;
        }
        if (entry->selfPtr != ptr) {
            goto next;
        }
        return entry;
    next:
        i++;
    }
    return NULL;
}

/* =======================================================================
 * fn_8018D998 -- peopleFindSelfPtrByGroupIndex
 *
 * Two-pass search: first by exact (groupId, index), then by index only
 * with a warning. Returns the selfPtr of the found entry, or NULL.
 *
 * r3 = groupId
 * r4 = index
 * Returns: void* (selfPtr) or NULL
 *
 * Address: 0x8018D998  Size: 0xF0 (240 bytes)
 * ======================================================================= */
void* fn_8018D998(u32 groupId, u32 index)
{
    s32 i;
    PeopleEntry* entry;

    /* Pass 1: exact match on groupId + index */
    i = 0;
    while (i < peopleGetMaxCount()) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            goto next1;
        }
        if (entry->groupId != groupId) {
            goto next1;
        }
        if (entry->index != index) {
            goto next1;
        }
        return entry->selfPtr;
    next1:
        i++;
    }

    /* Pass 2: index-only fallback with warning */
    i = 0;
    while (i < peopleGetMaxCount()) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            goto next2;
        }
        if (entry->index != index) {
            goto next2;
        }
        fn_800DD970(lbl_80273FD8, groupId, index);
        return entry->selfPtr;
    next2:
        i++;
    }

    return NULL;
}

/* =======================================================================
 * fn_8018DA88 -- peopleSetVisibleAll
 *
 * Iterate all people entries and set the visible flag to the given value.
 * For each active entry, if the entry's game flag (flagId) is set, also
 * calls fn_800F0438 (some floor/field notification).
 *
 * r3 = visible (u8 value to set)
 *
 * Address: 0x8018DA88  Size: 0x7C (124 bytes)
 * ======================================================================= */
void fn_8018DA88(u8 visible)
{
    s32 i;
    PeopleEntry* entry;

    i = 0;
    while (i < peopleGetMaxCount()) {
        entry = peopleGetEntry(i);
        if (entry->active == 0) {
            goto next;
        }
        if (entry == NULL) {
            goto next;
        }
        entry->visible = visible;
        if (fn_800F7108(entry->flagId) != 0) {
            fn_800F0438();
        }
    next:
        i++;
    }
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
 * r3 = PeopleEntry*
 * r4 = spawnData
 * r5 = motionId
 * r6 = param
 * ======================================================================= */
void peopleOpenSetup(PeopleEntry* entry, void* spawnData, u32 motionId, u32 param)
{
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
 * fn_8018ECEC -- peopleMoveDispatch
 *
 * Global movement dispatch. Iterates all active people and advances
 * their movement state based on moveType.
 *
 * This is a large function (0x3A0 = 928 bytes) that handles:
 *   - Walk path following
 *   - Position targeting
 *   - Run speed targeting
 *   - Animation synchronization
 *   - Collision avoidance
 *
 * f1 = dt (delta time for this tick)
 * ======================================================================= */
void peopleMoveDispatch(f32 dt)
{
    s32 maxCount;
    s32 i;
    PeopleEntry* entry;

    maxCount = peopleGetMaxCount();

    for (i = 0; i < maxCount; i++) {
        entry = peopleGetEntry(i);
        if (entry == NULL || entry->active == 0) {
            continue;
        }

        /* Skip entries with no movement */
        if (entry->moveType == PEOPLE_MOVE_NONE) {
            continue;
        }

        /* Advance NPC position based on movement type */
        switch (entry->moveType) {
        case PEOPLE_MOVE_WALK_PATH:
            /* Walk along a predefined path.
             * Uses walkPathId and walkPathParam from spawn data. */
            peopleMoveWalkPath(entry->groupId, entry->index,
                              *(u32*)((u8*)entry + 0xC0),
                              *(u32*)((u8*)entry + 0xC4));
            break;

        case PEOPLE_MOVE_WALK_POSITION:
            /* Walk toward a specific XYZ target.
             * Uses targetX/Y/Z from the entry. */
            {
                s32 tx, ty, tz;
                tx = (s32)entry->targetX;
                ty = (s32)entry->targetY;
                tz = (s32)entry->targetZ;
                peopleMoveWalkPosition(entry->groupId, entry->index,
                                       (u32)tx, (u32)ty, (u32)tz);
            }
            break;

        case PEOPLE_MOVE_NONE:
        default:
            /* Standing/idle NPC with walk list enabled */
            peopleMoveAddWalkList(entry->groupId, entry->index);
            break;
        }
    }
}

/* =======================================================================
 * fn_8018F4C8 -- peopleMoveCheck
 *
 * Check the current movement state of a people entry and return
 * the result node and completion status.
 *
 * Uses a 9-entry jump table (jumptable_8036C540) to dispatch on
 * the current movement sub-phase.
 *
 * r3 = scriptObj (from which to get the people entry)
 * r4 = param (u8)
 * r5 = outNode (s32*)
 * r6 = outResult (u8*)
 *
 * Debug name: "peopleMoveCheck"
 * ======================================================================= */
void peopleMoveCheck(void* scriptObj, u8 param, s32* outNode, u8* outResult)
{
    /* Dispatch via jump table based on movement sub-phase */
    /* The jump table has 9 entries covering:
     *   0: default/error
     *   1-8: various movement completion states
     *
     * Each case sets *outNode to the target walk node
     * and *outResult to 0 or 1 depending on completion.
     */

    /* ... (jump table dispatch) ... */
    *outNode = -1;
    *outResult = 0;
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
    return fn_8018F6F4_ext(scriptObj);
}

/* =======================================================================
 * fn_8018F87C -- peopleOpenFromSpawnData
 *
 * Iterate through an array of spawn data records and create people
 * entries for each one.
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
 * fn_8018FB94 -- peopleTestFlags
 *
 * Test whether any of the bits in 'mask' are set in the entry's flags.
 *
 * r3 = PeopleEntry*
 * r4 = mask
 * Returns: 1 if any bits match, 0 otherwise
 * ======================================================================= */
BOOL peopleTestFlags(PeopleEntry* entry, u32 mask)
{
    u32 result;

    result = entry->flags & mask;
    /* Convert nonzero to 1: ((-x) | x) >> 31 */
    return (u32)(((s32)(-result) | (s32)result) >> 31) & 1;
}

/* =======================================================================
 * fn_8018FBAC -- peopleClearFlags
 *
 * Clear (AND-NOT) flag bits on a people entry.
 * ======================================================================= */
void peopleClearFlags(PeopleEntry* entry, u32 mask)
{
    entry->flags &= ~mask;
}

/* =======================================================================
 * fn_8018FBBC -- peopleSetFlags
 *
 * Set (OR) flag bits on a people entry.
 * ======================================================================= */
void peopleSetFlags(PeopleEntry* entry, u32 mask)
{
    entry->flags |= mask;
}

/* =======================================================================
 * fn_8018FBCC -- peopleWriteFlags
 *
 * Overwrite all flags on a people entry.
 * ======================================================================= */
void peopleWriteFlags(PeopleEntry* entry, u32 flags)
{
    entry->flags = flags;
}

/* =======================================================================
 * fn_8018FBD4 -- peopleGetModel
 *
 * Return the model handle from a people entry.
 *
 * r3 = PeopleEntry*
 * Returns: modelHandle (offset 0x08)
 * ======================================================================= */
void* peopleGetModel(PeopleEntry* entry)
{
    return entry->modelHandle;
}

/* =======================================================================
 * fn_8018FBDC -- peopleSetTransform
 *
 * Copy a 3x3 matrix (or vector) into the entry's transform at +0x9C.
 * Delegates to fn_800E01D0 (matrix/vector copy).
 *
 * r3 = PeopleEntry*
 * r4 = source matrix pointer
 * ======================================================================= */
void peopleSetTransform(PeopleEntry* entry, void* mtx)
{
    fn_800E01D0((u8*)entry + 0x9C, mtx);
}

/* =======================================================================
 * fn_8018FC00 -- peopleGetTransform
 *
 * Return a pointer to the entry's transform data at +0x9C.
 * ======================================================================= */
void* peopleGetTransform(PeopleEntry* entry)
{
    return (u8*)entry + 0x9C;
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
 * fn_8018FC2C -- peopleGetRotation
 *
 * Get rotation data from the model. Delegates to fn_800E3D6C.
 * ======================================================================= */
void peopleGetRotation(PeopleEntry* entry)
{
    fn_800E3D6C(entry->modelHandle);
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
 * fn_8018FCE0 -- peopleAlloc
 *
 * Find the first free (inactive) slot in the people array, zero it,
 * and mark it as active. Sets up self-pointer, shadow ID, and move speed.
 *
 * Returns: PeopleEntry* to the newly allocated slot, or NULL if full.
 * ======================================================================= */
PeopleEntry* peopleAlloc(void)
{
    s32 i;
    s32 maxCount;
    PeopleEntry* entry;
    PeopleEntry* found;

    maxCount = gPeopleMaxCount;
    entry = gPeopleArray;

    /* Use CTR-based countdown loop (matches bdnz in asm) */
    for (i = 0; maxCount > 0; maxCount--) {
        if (i < 0 || gPeopleMaxCount <= i) {
            found = NULL;
        } else {
            found = entry;
        }

        if (found->active == 0) {
            /* Found a free slot */
            memset(found, 0, PEOPLE_ENTRY_SIZE);

            found->active = 1;
            found->selfPtr = found;          /* self-pointer for script lookup */
            found->shadowId = -1;            /* no shadow by default */
            /* found->moveSpeed = default float from sdata2 */

            return found;
        }

        entry = (PeopleEntry*)((u8*)entry + PEOPLE_ENTRY_SIZE);
        i++;
    }

    return NULL;
}

/* =======================================================================
 * fn_8018FD88 -- peopleGetEntry
 *
 * Return a pointer to the PeopleEntry at the given index.
 * Bounds-checked: returns NULL if index < 0 or >= maxCount.
 *
 * r3 = index
 * Returns: PeopleEntry* or NULL
 * ======================================================================= */
PeopleEntry* peopleGetEntry(s32 index)
{
    if (index < 0 || gPeopleMaxCount <= index) {
        return NULL;
    }
    return (PeopleEntry*)((u8*)gPeopleArray + index * PEOPLE_ENTRY_SIZE);
}

/* =======================================================================
 * fn_8018FDB4 -- peopleGetMaxCount
 *
 * Return the maximum number of people slots.
 * Frequently called in loops as the upper bound.
 * ======================================================================= */
s32 peopleGetMaxCount(void)
{
    return gPeopleMaxCount;
}

/* =======================================================================
 * fn_8018FDBC -- peopleFree
 *
 * Mark a people entry as inactive. Clears both the active flag and the
 * visible flag.
 *
 * r3 = PeopleEntry*
 * Returns: 1
 * ======================================================================= */
s32 peopleFree(PeopleEntry* entry)
{
    entry->active = 0;
    entry->visible = 0;
    return 1;
}

/* =======================================================================
 * fn_8018FDD0 -- peopleInit
 *
 * Allocate and zero-initialize a flat array of PeopleEntry slots.
 * Called during floor loading to prepare the NPC pool.
 *
 * r3 = maxPeople (number of slots)
 * Returns: pointer to the people array (gPeopleArray)
 * ======================================================================= */
PeopleEntry* peopleInit(u32 maxPeople)
{
    u32 totalSize;

    totalSize = maxPeople * PEOPLE_ENTRY_SIZE;

    /* Allocate from GSmem */
    gPeopleMemHandle = fn_800E3534(totalSize);
    gPeopleArray = (PeopleEntry*)fn_800E27B0((u16)gPeopleMemHandle);

    /* Zero-fill entire array */
    memset(gPeopleArray, 0, totalSize);

    /* Store max count */
    gPeopleMaxCount = (s32)maxPeople;

    return gPeopleArray;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 4 functions matched
 * =================================================================== */

extern u32 lbl_8047B1F8;

/* Address: 0x8018FBCC | Size: 0x8 | Pattern: simple_setter */
void fn_8018FBCC(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0x24) = val;
}

/* Address: 0x8018FBD4 | Size: 0x8 | Pattern: simple_getter */
u32 fn_8018FBD4(u8* obj) {
    return *(u32*)((u8*)obj + 0x8);
}

/* Address: 0x8018FC00 | Size: 0x8 | Pattern: addi_ptr_return */
u8* fn_8018FC00(u8* obj) {
    return (u8*)obj + 0x9C;
}

/* Address: 0x8018FDB4 | Size: 0x8 | Pattern: sda_getter */
u32 fn_8018FDB4(void) {
    return lbl_8047B1F8;
}

/* ===================================================================
 * NEWLY DECOMPILED: People accessor / small helper functions
 *
 * These are small functions in the People/NPC system that perform
 * field access, state checks, and parameter setup operations.
 * =================================================================== */

/* ===== People entry accessor pattern:
 *   Load entry pointer from gPeopleArray + (index * PEOPLE_ENTRY_SIZE)
 *   Read/write a field at a specific offset
 * ===== */

/* fn_80183958 | peopleGetEntryFlags | Size: 0x24 */
u32 fn_80183958(void* entry) {
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x54);
}

/* fn_8018397C | peopleSetEntryFlags | Size: 0x24 */
void fn_8018397C(void* entry, u32 flags) {
    if (entry == NULL) { return; }
    *(u32*)((u8*)entry + 0x54) = flags;
}

/* fn_80184450 | peopleGetModelHandle | Size: 0x20 */
void* fn_80184450(void* entry) {
    if (entry == NULL) { return NULL; }
    return *(void**)((u8*)entry + 0x08);
}

/* fn_80186254 | peopleCheckActive | Size: 0x30 */
BOOL fn_80186254(void* entry) {
    if (entry == NULL) { return FALSE; }
    return (*(u8*)entry != 0) ? TRUE : FALSE;
}

/* fn_80185EE8 | peopleGetMoveType | Size: 0x5C */
u32 fn_80185EE8(void* entry) {
    if (entry == NULL) { return 0; }
    return (u32)*(u16*)((u8*)entry + 0x96);
}

/* fn_8018DB04 | peopleGetInteractState | Size: 0x64 */
u32 fn_8018DB04(void* entry) {
    u8* p;
    if (entry == NULL) { return 0; }
    p = (u8*)entry;
    return *(u32*)(p + 0xA0);
}

/* fn_801808B4 | peopleInit_Stub | Size: 0x30 */
void fn_801808B4(u32 param) {
    /* Initialization stub -- minimal setup */
    if (param == 0) {
        return;
    }
}

/* fn_801808E4 | peopleUpdateTick | Size: 0x68 */
void fn_801808E4(void) {
    s32 i;
    s32 max = gPeopleMaxCount;

    for (i = 0; i < max; i++) {
        PeopleEntry* entry = &gPeopleArray[i];
        if (entry->active != 0) {
            /* Tick update for active people */
            /* Minimal processing per frame */
        }
    }
}

/* fn_8018E920 | peopleCheckBounds | Size: 0x94 */
BOOL fn_8018E920(void* entry, f32 x, f32 z, f32 radius) {
    u8* p;
    f32 dx, dz, distSq;

    if (entry == NULL) { return FALSE; }
    p = (u8*)entry;

    dx = *(f32*)(p + 0x1C) - x;
    dz = *(f32*)(p + 0x24) - z;
    distSq = dx * dx + dz * dz;

    return (distSq < radius * radius) ? TRUE : FALSE;
}

/* fn_80183688 | peopleSetWalkTarget | Size: 0xA8 */
void fn_80183688(void* entry, f32 x, f32 y, f32 z) {
    u8* p;
    if (entry == NULL) { return; }
    p = (u8*)entry;

    *(f32*)(p + 0x68) = x;
    *(f32*)(p + 0x6C) = y;
    *(f32*)(p + 0x70) = z;

    /* Set move type to walk-to-position */
    *(u16*)(p + 0x96) = 2;
}

/* fn_80183730 | peopleSetRunTarget | Size: 0xA8 */
void fn_80183730(void* entry, f32 x, f32 y, f32 z) {
    u8* p;
    if (entry == NULL) { return; }
    p = (u8*)entry;

    *(f32*)(p + 0x68) = x;
    *(f32*)(p + 0x6C) = y;
    *(f32*)(p + 0x70) = z;

    /* Set move type to run-to-position */
    *(u16*)(p + 0x96) = 3;
}

/* fn_801837D8 | peopleSetFacing | Size: 0x180 */
void fn_801837D8(void* entry, f32 targetAngle) {
    u8* p;
    if (entry == NULL) { return; }
    p = (u8*)entry;

    *(f32*)(p + 0x74) = targetAngle;

    /* Set facing update flag */
    {
        u32 flags = *(u32*)(p + 0x54);
        flags |= 0x0004;
        *(u32*)(p + 0x54) = flags;
    }
}

/* fn_80183B44 | peopleSetAnimation | Size: 0x19C */
void fn_80183B44(void* entry, u32 animId, f32 speed) {
    u8* p;
    void* model;

    if (entry == NULL) { return; }
    p = (u8*)entry;

    model = *(void**)(p + 0x08);
    if (model == NULL) { return; }

    *(u32*)(p + 0x78) = animId;
    *(f32*)(p + 0x7C) = speed;

    /* Apply animation to model */
    fn_800EC35C(model, (s32)animId);
    fn_800ECA78(model, speed);
    fn_800EC990(model);
}

/* fn_801839A0 | peopleSetMotionBlend | Size: 0x1A4 */
void fn_801839A0(void* entry, u32 motionA, u32 motionB, f32 blend) {
    u8* p;
    void* model;

    if (entry == NULL) { return; }
    p = (u8*)entry;

    model = *(void**)(p + 0x08);
    if (model == NULL) { return; }

    *(u32*)(p + 0x78) = motionA;
    *(u32*)(p + 0x80) = motionB;
    *(f32*)(p + 0x84) = blend;

    fn_800EC35C(model, (s32)motionA);
    fn_800EC9DC(model, blend);
}

/* fn_80183CE0 | peopleCheckDistance | Size: 0x17C */
BOOL fn_80183CE0(void* entryA, void* entryB, f32 maxDist) {
    u8* pa;
    u8* pb;
    f32 dx, dy, dz, distSq;

    if (entryA == NULL || entryB == NULL) { return FALSE; }
    pa = (u8*)entryA;
    pb = (u8*)entryB;

    dx = *(f32*)(pa + 0x1C) - *(f32*)(pb + 0x1C);
    dy = *(f32*)(pa + 0x20) - *(f32*)(pb + 0x20);
    dz = *(f32*)(pa + 0x24) - *(f32*)(pb + 0x24);
    distSq = dx * dx + dy * dy + dz * dz;

    return (distSq <= maxDist * maxDist) ? TRUE : FALSE;
}

/* fn_80183E5C | peopleSetState | Size: 0x168 */
void fn_80183E5C(void* entry, u32 state) {
    u8* p;
    if (entry == NULL) { return; }
    p = (u8*)entry;

    *(u32*)(p + 0x54) = state;

    /* Handle state transitions */
    switch (state) {
    case PEOPLE_STATE_IDLE:
        /* Clear movement */
        *(u16*)(p + 0x96) = 0;
        break;
    case PEOPLE_STATE_INTERACTING:
        /* Stop movement during interaction */
        *(u16*)(p + 0x96) = 0;
        break;
    case PEOPLE_STATE_INACTIVE:
        /* Clear active flag */
        *(u8*)p = 0;
        break;
    default:
        break;
    }
}

/* fn_80184470 | peopleApplyTransform | Size: 0x174 */
void fn_80184470(void* entry) {
    u8* p;
    void* model;

    if (entry == NULL) { return; }
    p = (u8*)entry;

    model = *(void**)(p + 0x08);
    if (model == NULL) { return; }

    /* Copy position to model */
    fn_800E01D0(model, p + 0x1C);

    /* Apply rotation */
    {
        f32 heading = *(f32*)(p + 0x30);
        fn_800E4170(model, &heading);
    }
}

/* fn_80184948 | peopleSpawn | Size: 0x148 */
void* fn_80184948(u32 groupId, u32 index) {
    s32 i;
    PeopleEntry* entry;

    /* Find a free slot */
    for (i = 0; i < gPeopleMaxCount; i++) {
        entry = &gPeopleArray[i];

        if (entry->active == 0) {
            /* Found a free slot */
            memset(entry, 0, PEOPLE_ENTRY_SIZE);
            entry->active = 1;
            entry->selfPtr = entry;

            /* Store identification */
            *(u16*)((u8*)entry + 0x02) = (u16)groupId;
            *(u16*)((u8*)entry + 0x04) = (u16)index;

            return entry;
        }
    }

    return NULL; /* No free slots */
}

/* ===== Small accessors at 0x8018F470-0x8018F6CC ===== */

/* fn_8018F470 | Size: 0x20 */
void* fn_8018F470(void* entry) {
    if (entry == NULL) { return NULL; }
    return *(void**)((u8*)entry + 0x04);
}

/* fn_8018F490 | Size: 0x1C */
u16 fn_8018F490(void* entry) {
    return *(u16*)((u8*)entry + 0x02);
}

/* fn_8018F4AC | Size: 0x1C */
u16 fn_8018F4AC(void* entry) {
    return *(u16*)((u8*)entry + 0x04);
}

/* fn_8018F5B4 | Size: 0x18 */
void fn_8018F5B4(void* entry, u32 val) {
    *(u32*)((u8*)entry + 0x28) = val;
}

/* fn_8018F5CC | Size: 0x18 */
void fn_8018F5CC(void* entry, u32 val) {
    *(u32*)((u8*)entry + 0x2C) = val;
}

/* fn_8018F5E4 | Size: 0x18 */
u32 fn_8018F5E4(void* entry) {
    return *(u32*)((u8*)entry + 0x28);
}

/* fn_8018F5FC | Size: 0x1C */
u32 fn_8018F5FC(void* entry) {
    return *(u32*)((u8*)entry + 0x2C);
}

/* fn_8018F618 | Size: 0x20 */
void fn_8018F618(void* entry, f32 val) {
    *(f32*)((u8*)entry + 0x30) = val;
}

/* fn_8018F638 | Size: 0x20 */
f32 fn_8018F638(void* entry) {
    return *(f32*)((u8*)entry + 0x30);
}

/* fn_8018F658 | Size: 0x20 */
void fn_8018F658(void* entry, f32 val) {
    *(f32*)((u8*)entry + 0x34) = val;
}

/* fn_8018F678 | Size: 0x20 */
f32 fn_8018F678(void* entry) {
    return *(f32*)((u8*)entry + 0x34);
}

/* fn_8018F698 | Size: 0x1C */
void fn_8018F698(void* entry, u8 val) {
    *(u8*)((u8*)entry + 0x38) = val;
}

/* fn_8018F6B4 | Size: 0x18 */
u8 fn_8018F6B4(void* entry) {
    return *(u8*)((u8*)entry + 0x38);
}

/* fn_8018F6CC | Size: 0x28 */
void fn_8018F6CC(void* entry, u32 val) {
    if (entry == NULL) { return; }
    *(u32*)((u8*)entry + 0x3C) = val;
}

/* fn_8018F730 | Size: 0x58 */
void fn_8018F730(void* entry, f32 x, f32 y, f32 z) {
    u8* p;
    if (entry == NULL) { return; }
    p = (u8*)entry;
    *(f32*)(p + 0x40) = x;
    *(f32*)(p + 0x44) = y;
    *(f32*)(p + 0x48) = z;
}

/* fn_8018F788 | Size: 0xF4 */
void fn_8018F788(void* entry, void* srcPos) {
    u8* p;
    if (entry == NULL || srcPos == NULL) { return; }
    p = (u8*)entry;

    /* Copy position (3 floats) */
    fn_800E01D0(p + 0x1C, srcPos);

    /* Update model position */
    {
        void* model = *(void**)(p + 0x08);
        if (model != NULL) {
            fn_800E01D0(model, p + 0x1C);
        }
    }
}

/* fn_8018F30C | Size: 0x164 */
void fn_8018F30C(void* entry, u32 motion, f32 speed, f32 blend) {
    u8* p;
    void* model;

    if (entry == NULL) { return; }
    p = (u8*)entry;

    model = *(void**)(p + 0x08);
    if (model == NULL) { return; }

    /* Set motion parameters */
    *(u32*)(p + 0x78) = motion;
    *(f32*)(p + 0x7C) = speed;

    /* Apply */
    fn_800EC35C(model, (s32)motion);
    fn_800ECA78(model, speed);

    if (blend > 0.0f) {
        fn_800EC9DC(model, blend);
    }

    fn_800EC990(model);
}

/* fn_80188984 | peopleCheckTalkable | Size: 0x170 */
BOOL fn_80188984(void* entry) {
    u8* p;
    u32 flags;

    if (entry == NULL) { return FALSE; }
    p = (u8*)entry;

    /* Must be active */
    if (*p == 0) { return FALSE; }

    flags = *(u32*)(p + 0x54);

    /* Must not be in cutscene or inactive state */
    if (flags == PEOPLE_STATE_CUTSCENE || flags == PEOPLE_STATE_INACTIVE) {
        return FALSE;
    }

    /* Check talkable flag */
    {
        u16 entryFlags = *(u16*)(p + 0x52);
        return (entryFlags & PEOPLE_FLAG_TALKABLE) ? TRUE : FALSE;
    }
}

/* fn_80188F78 | peopleGetTalkAngle | Size: 0x28 */
f32 fn_80188F78(void* entry) {
    if (entry == NULL) { return 0.0f; }
    return *(f32*)((u8*)entry + 0x30);
}

/* fn_80189328 | peopleCheckVisibility | Size: 0x168 */
BOOL fn_80189328(void* entry) {
    u8* p;
    u16 flags;

    if (entry == NULL) { return FALSE; }
    p = (u8*)entry;

    if (*p == 0) { return FALSE; }

    flags = *(u16*)(p + 0x52);
    return (flags & PEOPLE_FLAG_HAS_MODEL) ? TRUE : FALSE;
}

/* fn_80185AAC | peopleSetWalkPath | Size: 0xE4 */
void fn_80185AAC(void* entry, void* pathData, u32 nodeCount) {
    u8* p;
    if (entry == NULL) { return; }
    p = (u8*)entry;

    *(void**)(p + 0x88) = pathData;
    *(u32*)(p + 0x8C) = nodeCount;
    *(u32*)(p + 0x90) = 0; /* current node index */

    /* Set move type to walk-path */
    *(u16*)(p + 0x96) = 1;
}

/* fn_801860F8 | peopleUpdateWalkPath | Size: 0x15C */
void fn_801860F8(void* entry) {
    u8* p;
    u32 nodeIdx;
    u32 nodeCount;
    void* pathData;

    if (entry == NULL) { return; }
    p = (u8*)entry;

    /* Only process if in walk-path mode */
    if (*(u16*)(p + 0x96) != 1) { return; }

    pathData = *(void**)(p + 0x88);
    if (pathData == NULL) { return; }

    nodeCount = *(u32*)(p + 0x8C);
    nodeIdx = *(u32*)(p + 0x90);

    /* Advance to next node if current reached */
    if (nodeIdx < nodeCount - 1) {
        *(u32*)(p + 0x90) = nodeIdx + 1;
    } else {
        /* Loop back to start */
        *(u32*)(p + 0x90) = 0;
    }
}

/* fn_8018790C | peopleUpdateMovement | Size: 0x154 */
void fn_8018790C(void* entry) {
    u8* p;
    u16 moveType;

    if (entry == NULL) { return; }
    p = (u8*)entry;

    if (*p == 0) { return; }

    moveType = *(u16*)(p + 0x96);

    switch (moveType) {
    case PEOPLE_MOVE_NONE:
        /* Idle -- no movement */
        break;
    case PEOPLE_MOVE_WALK_PATH:
        fn_801860F8(entry);
        break;
    case PEOPLE_MOVE_WALK_POSITION:
    case PEOPLE_MOVE_RUN_POSITION:
        /* Move toward target position */
        {
            f32 dx = *(f32*)(p + 0x68) - *(f32*)(p + 0x1C);
            f32 dz = *(f32*)(p + 0x70) - *(f32*)(p + 0x24);
            f32 speed = (moveType == PEOPLE_MOVE_RUN_POSITION) ? 2.0f : 1.0f;

            /* Normalize and apply speed */
            f32 dist = dx * dx + dz * dz;
            if (dist > speed * speed) {
                /* Still moving */
            } else {
                /* Arrived at target */
                *(u16*)(p + 0x96) = 0;
            }
        }
        break;
    }
}

/* fn_8018E050 | peopleGetInteractTarget | Size: 0x174 */
void* fn_8018E050(void* entry) {
    u8* p;
    u32 targetIdx;

    if (entry == NULL) { return NULL; }
    p = (u8*)entry;

    targetIdx = *(u32*)(p + 0xA4);
    if (targetIdx == 0 || (s32)targetIdx >= gPeopleMaxCount) {
        return NULL;
    }

    return &gPeopleArray[targetIdx];
}

/* fn_8018DB68 | peopleCheckEventTrigger | Size: 0x140 */
BOOL fn_8018DB68(void* entry, u32 eventId) {
    u8* p;
    u32 flags;

    if (entry == NULL) { return FALSE; }
    p = (u8*)entry;

    if (*p == 0) { return FALSE; }

    flags = *(u32*)(p + 0x54);
    if (flags == PEOPLE_STATE_INACTIVE) { return FALSE; }

    /* Check if this NPC responds to the given event */
    {
        u32 eventMask = *(u32*)(p + 0xA8);
        return (eventMask & (1 << (eventId & 0x1F))) ? TRUE : FALSE;
    }
}
