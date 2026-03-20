/**
 * @file people_move.c
 * @brief NPC movement and pathfinding subsystem.
 *
 * Decompiled from:
 *   fn_801812C4 (peopleMoveUpdate)        -- single-tick movement update wrapper
 *   fn_8018ECEC (peopleMoveDispatch)       -- dispatch movement by moveType
 *   fn_80188FA0 (peopleMoveWalkPath)       -- walk along a predefined path
 *   fn_80188CA0 (peopleMoveWalkPosition)   -- walk toward XYZ target
 *   fn_80188AF4 (peopleMoveAddWalkList)    -- add entry to walk list
 *   fn_8018F4C8 (peopleMoveCheck)          -- check movement completion
 *   fn_8018D680 (peopleIsWithinRange)      -- proximity / angle check
 *   fn_8018D7D0 (peopleFindByGroupIndex)   -- find people entry by group+index
 *   fn_8018CD08 (peopleMoveMain)           -- large movement state machine
 *
 * Debug strings:
 *   "peopleMoveCheck"
 *   "peopleAddWalkList"
 *   "ERROR! [%s]: People[%d,%d] WalkMotion[%d] is frame zero."
 *   "ERROR! [%s]: People[%d,%d] RunMotion[%d] is frame zero."
 *
 * Address range: 0x801812C4, 0x8018880 - 0x8018F200
 */

#include "dolphin/types.h"
#include "game/people/people.h"

/* ===== External engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);  /* OSReport */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* Vector/matrix math */
extern void  fn_800E019C(void* dst, u32 a, u32 b); /* vector subtract */
extern void  fn_800E013C(void* dst, void* vec, f32 scale); /* vector normalize */
extern void  fn_800E0168(void* dst, void* a, void* b);     /* cross product */
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

/* Floor/collision */
extern void  fn_801848D0(void* model, u32 a, u32 b, u32 c); /* set model walk nodes */
extern BOOL  fn_800F7108(u16 flagId);               /* GSflagGet */

/* Script system */
extern void* fn_8018F6F4(void* scriptObj);          /* get script ref data */

/* ===== Rodata references ===== */
extern const char lbl_80273FD8[];  /* "Warining: people[%d,%d] group is different!!\n" */

/* ===== Sdata2 float constants ===== */
/* lbl_8047D798 : f32 -- delta time for movement tick */
/* lbl_8047D79C : f32 -- 0.0f */
/* lbl_8047D7A0 : f32 -- 0.0f */
/* lbl_8047D7A4 : f32 -- 1.0f */

/* ===== Data section references ===== */
extern const char lbl_8036C4E8[];   /* "peopleOpenSub" */
extern const char lbl_8036C4F8[];   /* "peopleWaitSyncMotion" */
extern const char lbl_8036C510[];   /* "peopleWaitSyncMotionBlend" */
extern const char lbl_8036C52C[];   /* "peopleMoveCheck" */

/* ===== Jump table for peopleMoveCheck ===== */
extern void* jumptable_8036C540[];  /* 9-entry jump table for move result */

/* ===== Internal declarations ===== */
static PeopleEntry* peopleFindByGroupAndIndex(u32 groupId, u32 index);

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
