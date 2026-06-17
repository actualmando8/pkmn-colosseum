/**
 * @file people.c
 * @brief Core People/NPC system -- init, alloc, update, spawn, despawn.
 *
 * Decompiled from:
 *   fn_8018FDD0 (peopleInit)         -- allocate array for N people slots
 *   fn_8018FDB4 (peopleGetMaxCount)  -- return max slot count
 *   fn_8018FD88 (peopleGetEntry)     -- index -> PeopleEntry*
 *   fn_8018FCE0 (peopleAlloc)        -- find free slot, memset, mark active
 *   fn_8018FDBC (peopleFree)         -- mark slot inactive
 *   fn_80181850 (peopleUpdate)       -- per-frame update loop for all NPCs
 *   fn_80181224 (peopleFloorInit)    -- draw 30 blank frames at floor start
 *   fn_80180C78 (peopleOpenCallback) -- floor loader callback (opens people)
 *   fn_80181094 (peopleOpenThread)   -- continuation callback after open
 *   fn_8018114C (peopleCloseCallback)-- cleanup callback
 *   fn_8018FBD4 (peopleGetModel)     -- return modelHandle from entry
 *   fn_8018FB94 (peopleTestFlags)    -- test flag bits
 *   fn_8018FBBC (peopleSetFlags)     -- set flag bits
 *   fn_8018FBAC (peopleClearFlags)   -- clear flag bits
 *   fn_8018FBCC (peopleWriteFlags)   -- write all flags
 *   fn_8018FBDC (peopleSetTransform) -- copy 3x3 matrix into entry
 *   fn_8018FC00 (peopleGetTransform) -- return pointer to entry transform
 *
 * Address range: 0x80180C78 - 0x80181850, 0x8018FB94 - 0x8018FE30
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

/* External functions referenced from asm wrappers */
extern void fn_800DCC3C(void);
extern void fn_800DCC60(void);
extern void fn_800E01F4(void);
extern void fn_800E3CF8(void*);
extern void fn_800E3D00(void*);
extern void fn_800E3D08(void*);
extern void fn_800E3D6C(void*);
extern void fn_800E3D98(void*, void*);
extern void fn_800E4014(void*, u8);
extern void fn_800E4170(void*);
extern void fn_800E43A4(void*);
extern void fn_800E9B2C(void);
extern void fn_800E9C6C(void);
extern void fn_800EC2A4(void);
extern void fn_800EC308(void);
extern void fn_800EC35C(void);
extern void fn_800EC578(void);
extern void fn_800EC954(void);
extern void fn_800EC960(void);
extern void fn_800EC9DC(void);
extern void fn_800ECA78(void);
extern void fn_800ECB74(void);
extern void fn_800ECCA8(void);
extern void fn_8010FFC4(void);
extern void fn_801812C4(void);
extern void fn_801848D0(void);
extern void fn_80188AF4(void);
extern void fn_80188CA0(void);
extern void fn_80188FA0(void);
extern void fn_8018E1C4(void);
extern void fn_8018F08C(void);
extern void fn_8018F4C8(void);
extern void fn_8018F6F4(void);
extern void fn_8018FBD4(void);
extern void fn_8018FCBC(void);
extern void fn_8018FD88(void);
extern void fn_8018FDB4(void);

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

/* ===== Rodata string references ===== */
extern const char lbl_80273F80[];  /* floor name for blank-frame init */
extern const char lbl_80273FD8[];  /* "Warining: people[%d,%d] group is different!!\n" */

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

/* ===== Sdata2 float constants ===== */
/* lbl_8047D798 @sda21 : constant used in peopleMoveUpdate */
/* lbl_8047D79C @sda21 : float 0.0 */
/* lbl_8047D7A0 @sda21 : float 0.0 (zero) */
/* lbl_8047D7A4 @sda21 : float 1.0 */
/* lbl_8047D8B0 @sda21 : float default moveSpeed */

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
    gPeopleArray = (PeopleEntry*)fn_800E27B0(gPeopleMemHandle);

    /* Zero-fill entire array */
    memset(gPeopleArray, 0, totalSize);

    /* Store max count */
    gPeopleMaxCount = (s32)maxPeople;

    return gPeopleArray;
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
 * fn_8018FBBC -- peopleSetFlags
 *
 * Set (OR) flag bits on a people entry.
 * ======================================================================= */
void peopleSetFlags(PeopleEntry* entry, u32 mask)
{
    entry->flags |= mask;
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
 * fn_8018FBCC -- peopleWriteFlags
 *
 * Overwrite all flags on a people entry.
 * ======================================================================= */
void peopleWriteFlags(PeopleEntry* entry, u32 flags)
{
    entry->flags = flags;
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

/* 0x8018A44C | 0x2B4 */
extern void fn_8018FC2C(void);
extern u32 lbl_8047D814;
extern u32 lbl_8047D7C0;
extern u32 lbl_8047D7C8;
#if 1
asm void fn_8018A44C(void) {
    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    stw r28, 0x30(r1)
    fmr f31, f1
    mr r30, r3
    mr r31, r4
    li r29, 0x0
    b @L_8018A4C4
@L_8018A48C:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A4C0
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_8018A4C0
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_8018A4C0
    lwz r29, 0x4(r3)
    b @L_8018A530
@L_8018A4C0:
    addi r29, r29, 0x1
@L_8018A4C4:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018A48C
    li r28, 0x0
    b @L_8018A520
@L_8018A4D8:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A51C
    lwz r0, 0x2c(r29)
    cmplw r0, r31
    bne @L_8018A51C
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r29, 0x4(r29)
    b @L_8018A530
@L_8018A51C:
    addi r28, r28, 0x1
@L_8018A520:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A4D8
    li r29, 0x0
@L_8018A530:
    li r28, 0x0
    b @L_8018A560
@L_8018A538:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A55C
    lwz r0, 0x4(r3)
    cmplw r0, r29
    bne @L_8018A55C
    b @L_8018A570
@L_8018A55C:
    addi r28, r28, 0x1
@L_8018A560:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A538
    li r3, 0x0
@L_8018A570:
    cmplwi r3, 0x0
    beq @L_8018A6D0
    lfs f0, lbl_8047D814(r2)
    li r28, 0x0
    lfs f30, 0x58(r3)
    fmuls f31, f0, f31
    b @L_8018A5C4
@L_8018A58C:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A5C0
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_8018A5C0
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_8018A5C0
    lwz r28, 0x4(r3)
    b @L_8018A630
@L_8018A5C0:
    addi r28, r28, 0x1
@L_8018A5C4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A58C
    li r28, 0x0
    b @L_8018A620
@L_8018A5D8:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A61C
    lwz r0, 0x2c(r29)
    cmplw r0, r31
    bne @L_8018A61C
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r29)
    b @L_8018A630
@L_8018A61C:
    addi r28, r28, 0x1
@L_8018A620:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018A5D8
    li r28, 0x0
@L_8018A630:
    li r29, 0x0
    b @L_8018A664
@L_8018A638:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A660
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018A660
    mr r28, r3
    b @L_8018A674
@L_8018A660:
    addi r29, r29, 0x1
@L_8018A664:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018A638
    li r28, 0x0
@L_8018A674:
    cmplwi r28, 0x0
    beq @L_8018A6D0
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f2, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x20(r1)
    fdivs f0, f0, f2
    lfd f1, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fmadds f31, f2, f0, f31
    stfs f31, 0x40(r28)
    stfs f30, 0x44(r28)
@L_8018A6D0:
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r0, 0x64(r1)
    lwz r28, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
}
#else
void fn_8018A44C(void) {
    /* TODO: match -- 692 bytes at 0x8018A44C */
}
#endif

/* 0x8018A700 | 0x3CC */
extern void fn_8018FC98(void);
extern void fn_800E008C(void);
extern void fn_800E013C(void);
extern void fn_800E019C(void);
extern void fn_8018AACC(void);
extern u32 lbl_8047D800;
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D79C;
#if 1
asm void fn_8018A700(void) {
    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stmw r25, 0x34(r1)
    lfs f0, lbl_8047D800(r2)
    addic. r0, r1, 0x8
    mr r27, r3
    mr r28, r4
    fmuls f31, f0, f1
    mr r29, r5
    mr r30, r6
    mr r31, r7
    beq @L_8018A838
    li r26, 0x0
    b @L_8018A77C
@L_8018A744:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A778
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_8018A778
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_8018A778
    lwz r26, 0x4(r3)
    b @L_8018A7E8
@L_8018A778:
    addi r26, r26, 0x1
@L_8018A77C:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018A744
    li r25, 0x0
    b @L_8018A7D8
@L_8018A790:
    mr r3, r25
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A7D4
    lwz r0, 0x2c(r26)
    cmplw r0, r28
    bne @L_8018A7D4
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r26)
    b @L_8018A7E8
@L_8018A7D4:
    addi r25, r25, 0x1
@L_8018A7D8:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A790
    li r26, 0x0
@L_8018A7E8:
    li r25, 0x0
    b @L_8018A818
@L_8018A7F0:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A814
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_8018A814
    b @L_8018A828
@L_8018A814:
    addi r25, r25, 0x1
@L_8018A818:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A7F0
    li r3, 0x0
@L_8018A828:
    cmplwi r3, 0x0
    beq @L_8018A838
    addi r4, r1, 0x8
    bl fn_8018FC98
@L_8018A838:
    addic. r0, r1, 0x14
    beq @L_8018A93C
    li r25, 0x0
    b @L_8018A880
@L_8018A848:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A87C
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_8018A87C
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_8018A87C
    lwz r25, 0x4(r3)
    b @L_8018A8EC
@L_8018A87C:
    addi r25, r25, 0x1
@L_8018A880:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A848
    li r25, 0x0
    b @L_8018A8DC
@L_8018A894:
    mr r3, r25
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A8D8
    lwz r0, 0x2c(r26)
    cmplw r0, r30
    bne @L_8018A8D8
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r25, 0x4(r26)
    b @L_8018A8EC
@L_8018A8D8:
    addi r25, r25, 0x1
@L_8018A8DC:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A894
    li r25, 0x0
@L_8018A8EC:
    li r26, 0x0
    b @L_8018A91C
@L_8018A8F4:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A918
    lwz r0, 0x4(r3)
    cmplw r0, r25
    bne @L_8018A918
    b @L_8018A92C
@L_8018A918:
    addi r26, r26, 0x1
@L_8018A91C:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018A8F4
    li r3, 0x0
@L_8018A92C:
    cmplwi r3, 0x0
    beq @L_8018A93C
    addi r4, r1, 0x14
    bl fn_8018FC98
@L_8018A93C:
    addi r3, r1, 0x20
    addi r4, r1, 0x8
    addi r5, r1, 0x14
    bl fn_800E0168
    addi r3, r1, 0x20
    bl fn_800E008C
    lfs f0, lbl_8047D7A0(r2)
    fcmpu cr0, f0, f1
    bne @L_8018A968
    lfs f1, lbl_8047D79C(r2)
    b @L_8018A96C
@L_8018A968:
    fdivs f1, f31, f1
@L_8018A96C:
    lfs f0, lbl_8047D79C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8018AA80
    li r26, 0x0
    b @L_8018A9BC
@L_8018A984:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018A9B8
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_8018A9B8
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_8018A9B8
    lwz r26, 0x4(r3)
    b @L_8018AA28
@L_8018A9B8:
    addi r26, r26, 0x1
@L_8018A9BC:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018A984
    li r25, 0x0
    b @L_8018AA18
@L_8018A9D0:
    mr r3, r25
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AA14
    lwz r0, 0x2c(r29)
    cmplw r0, r28
    bne @L_8018AA14
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r29)
    b @L_8018AA28
@L_8018AA14:
    addi r25, r25, 0x1
@L_8018AA18:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018A9D0
    li r26, 0x0
@L_8018AA28:
    li r27, 0x0
    b @L_8018AA58
@L_8018AA30:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AA54
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_8018AA54
    b @L_8018AA68
@L_8018AA54:
    addi r27, r27, 0x1
@L_8018AA58:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018AA30
    li r3, 0x0
@L_8018AA68:
    cmplwi r3, 0x0
    beq @L_8018AAB0
    li r0, 0x0
    stb r0, 0x54(r3)
    stb r0, 0x22(r3)
    b @L_8018AAB0
@L_8018AA80:
    addi r3, r1, 0x20
    mr r4, r3
    bl fn_800E013C
    addi r3, r1, 0x14
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800E019C
    mr r3, r27
    mr r4, r28
    mr r5, r31
    addi r6, r1, 0x14
    bl fn_8018AACC
@L_8018AAB0:
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    lmw r25, 0x34(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
}
#else
void fn_8018A700(void) {
    /* TODO: match -- 972 bytes at 0x8018A700 */
}
#endif

/* 0x8018AEC0 | 0x1BC */
extern void _threadSwitch(void);
extern u32 lbl_8047D79C;
extern u8 lbl_80274008[];
extern u8 lbl_8036C510[];
#if 1
asm void fn_8018AEC0(void) {
    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stmw r27, 0xc(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    li r28, 0x0
    b @L_8018AF24
@L_8018AEEC:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AF20
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_8018AF20
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_8018AF20
    lwz r28, 0x4(r3)
    b @L_8018AF90
@L_8018AF20:
    addi r28, r28, 0x1
@L_8018AF24:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018AEEC
    li r27, 0x0
    b @L_8018AF80
@L_8018AF38:
    mr r3, r27
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AF7C
    lwz r0, 0x2c(r28)
    cmplw r0, r30
    bne @L_8018AF7C
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_8018AF90
@L_8018AF7C:
    addi r27, r27, 0x1
@L_8018AF80:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018AF38
    li r28, 0x0
@L_8018AF90:
    li r27, 0x0
    b @L_8018AFC4
@L_8018AF98:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018AFC0
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018AFC0
    mr r27, r3
    b @L_8018AFD4
@L_8018AFC0:
    addi r27, r27, 0x1
@L_8018AFC4:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018AF98
    li r27, 0x0
@L_8018AFD4:
    cmplwi r27, 0x0
    bne @L_8018AFE4
    li r3, 0x0
    b @L_8018B060
@L_8018AFE4:
    mr r3, r27
    bl fn_8018FBD4
    mr. r28, r3
    bne @L_8018AFFC
    li r3, 0x0
    b @L_8018B060
@L_8018AFFC:
    clrlwi r31, r31, 24
    lfs f31, lbl_8047D79C(r2)
@L_8018B004:
    lfs f0, 0xd4(r27)
    fcmpu cr0, f31, f0
    bne @L_8018B018
    li r3, 0x0
    b @L_8018B060
@L_8018B018:
    cmplwi r31, 0x0
    beq @L_8018B05C
    lwz r0, 0x8c(r28)
    cmpwi r0, 0x1
    bne @L_8018B054
    lis r3, lbl_80274008@ha
    lis r4, lbl_8036C510@ha
    addi r3, r3, lbl_80274008@l
    mr r5, r29
    addi r4, r4, lbl_8036C510@l
    mr r6, r30
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8018B060
@L_8018B054:
    bl _threadSwitch
    b @L_8018B004
@L_8018B05C:
    li r3, 0x1
@L_8018B060:
    psq_l f31, 0x28(r1), 0, 0
    lfd f31, 0x20(r1)
    lmw r27, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
}
#else
void fn_8018AEC0(void) {
    /* TODO: match -- 444 bytes at 0x8018AEC0 */
}
#endif

/* 0x8018B220 | 0x148 */
extern void fn_800EC96C(void);
#if 1
asm void fn_8018B220(void) {
#include "src/game/people/people_fn_8018B220.inc"
}
#else
void fn_8018B220(u32 groupId, u32 index) {
    PeopleEntry* found;
    s32 i;
    s32 j;
    PeopleEntry* entry;

    found = NULL;

    /* Loop 1: search by groupId + index */
    for (i = 0; i < fn_8018FDB4(); i++) {
        entry = (PeopleEntry*)fn_8018FD88(i);
        if (*(u8*)entry == 0) continue;
        if (*(u32*)((u8*)entry + 0x28) != groupId) continue;
        if (*(u32*)((u8*)entry + 0x2C) != index) continue;
        found = *(PeopleEntry**)((u8*)entry + 0x04);
        goto loop3;
    }

    /* Loop 2: fallback search by index only */
    for (j = 0; j < fn_8018FDB4(); j++) {
        entry = (PeopleEntry*)fn_8018FD88(j);
        found = entry;
        if (*(u8*)entry == 0) continue;
        if (*(u32*)((u8*)found + 0x2C) != index) continue;
        fn_800DD970((u8*)lbl_80273FD8, groupId, index);
        found = *(PeopleEntry**)((u8*)found + 0x04);
        goto loop3;
    }
    found = NULL;

loop3:
    /* Loop 3: search by selfPtr */
    for (j = 0; j < fn_8018FDB4(); j++) {
        entry = (PeopleEntry*)fn_8018FD88(j);
        if (*(u8*)entry == 0) continue;
        if (*(u32*)((u8*)entry + 0x04) != (u32)found) continue;
        goto found_entry;
    }
    entry = NULL;

found_entry:
    if (entry != NULL) {
        void* model = fn_8018FBD4(entry);
        if (model != NULL) {
            fn_800EC96C(model);
        }
    }
}
#endif

/* 0x8018B368 | 0x1F0 */
extern void fn_800EC990(void);
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7C8;
extern u32 lbl_8047D7A4;
#if 1
asm void fn_8018B368(void) {
    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr. r29, r5
    mr r25, r3
    mr r26, r4
    mr r30, r6
    mr r31, r7
    blt @L_8018B544
    li r28, 0x0
    b @L_8018B3D0
@L_8018B398:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B3CC
    lwz r0, 0x28(r3)
    cmplw r0, r25
    bne @L_8018B3CC
    lwz r0, 0x2c(r3)
    cmplw r0, r26
    bne @L_8018B3CC
    lwz r28, 0x4(r3)
    b @L_8018B43C
@L_8018B3CC:
    addi r28, r28, 0x1
@L_8018B3D0:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018B398
    li r27, 0x0
    b @L_8018B42C
@L_8018B3E4:
    mr r3, r27
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B428
    lwz r0, 0x2c(r28)
    cmplw r0, r26
    bne @L_8018B428
    lis r3, lbl_80273FD8@ha
    mr r4, r25
    addi r3, r3, lbl_80273FD8@l
    mr r5, r26
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_8018B43C
@L_8018B428:
    addi r27, r27, 0x1
@L_8018B42C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B3E4
    li r28, 0x0
@L_8018B43C:
    li r27, 0x0
    b @L_8018B470
@L_8018B444:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B46C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018B46C
    mr r28, r3
    b @L_8018B480
@L_8018B46C:
    addi r27, r27, 0x1
@L_8018B470:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B444
    li r28, 0x0
@L_8018B480:
    cmplwi r28, 0x0
    beq @L_8018B544
    mr r3, r28
    bl fn_8018FBD4
    mr. r27, r3
    beq @L_8018B544
    stw r29, 0x48(r28)
    mr r4, r29
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xd8(r28)
    bl fn_800ECCA8
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    mr r3, r27
    lfd f1, lbl_8047D7C8(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f1
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r27
    bl fn_800EC9DC
    mr r3, r27
    mr r4, r29
    bl fn_800EC35C
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0x14(r1)
    mr r3, r27
    lfd f1, lbl_8047D7C8(r2)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f1, f0, f1
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r27
    bl fn_800EC308
    clrlwi. r0, r31, 24
    beq @L_8018B530
    mr r3, r27
    li r4, 0x1
    bl fn_800ECB74
    b @L_8018B53C
@L_8018B530:
    mr r3, r27
    li r4, 0x0
    bl fn_800ECB74
@L_8018B53C:
    mr r3, r27
    bl fn_800EC990
@L_8018B544:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
}
#else
void fn_8018B368(void) {
    /* TODO: match -- 496 bytes at 0x8018B368 */
}
#endif

/* 0x8018B558 | 0x214 */
extern void fn_800EC5FC(void);
extern void fn_800EC5B8(void);
extern void fn_800EC4D0(void);
extern void fn_800EC8C8(void);
extern u32 lbl_8047D7D0;
extern u32 lbl_8047D79C;
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D834;
#if 1
asm void fn_8018B558(void) {
    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr. r29, r5
    mr r25, r3
    mr r26, r4
    mr r30, r6
    mr r31, r7
    blt @L_8018B588
    cmpwi r30, 0x0
    bge @L_8018B590
@L_8018B588:
    li r3, 0x0
    b @L_8018B758
@L_8018B590:
    cmplwi r31, 0x1
    bge @L_8018B5A0
    li r3, 0x0
    b @L_8018B758
@L_8018B5A0:
    li r28, 0x0
    b @L_8018B5E0
@L_8018B5A8:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B5DC
    lwz r0, 0x28(r3)
    cmplw r0, r25
    bne @L_8018B5DC
    lwz r0, 0x2c(r3)
    cmplw r0, r26
    bne @L_8018B5DC
    lwz r28, 0x4(r3)
    b @L_8018B64C
@L_8018B5DC:
    addi r28, r28, 0x1
@L_8018B5E0:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018B5A8
    li r27, 0x0
    b @L_8018B63C
@L_8018B5F4:
    mr r3, r27
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B638
    lwz r0, 0x2c(r28)
    cmplw r0, r26
    bne @L_8018B638
    lis r3, lbl_80273FD8@ha
    mr r4, r25
    addi r3, r3, lbl_80273FD8@l
    mr r5, r26
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_8018B64C
@L_8018B638:
    addi r27, r27, 0x1
@L_8018B63C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B5F4
    li r28, 0x0
@L_8018B64C:
    li r27, 0x0
    b @L_8018B680
@L_8018B654:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018B67C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018B67C
    mr r28, r3
    b @L_8018B690
@L_8018B67C:
    addi r27, r27, 0x1
@L_8018B680:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018B654
    li r28, 0x0
@L_8018B690:
    cmplwi r28, 0x0
    bne @L_8018B6A0
    li r3, 0x0
    b @L_8018B758
@L_8018B6A0:
    mr r3, r28
    bl fn_8018FBD4
    mr. r27, r3
    bne @L_8018B6B8
    li r3, 0x0
    b @L_8018B758
@L_8018B6B8:
    lis r0, 0x4330
    stw r31, 0x14(r1)
    lfd f1, lbl_8047D7D0(r2)
    mr r4, r29
    stw r0, 0x10(r1)
    mr r5, r30
    lfs f2, lbl_8047D79C(r2)
    lfd f0, 0x10(r1)
    stw r30, 0x48(r28)
    fsubs f1, f0, f1
    lfs f0, lbl_8047D7A0(r2)
    fdivs f1, f2, f1
    stfs f1, 0xd8(r28)
    stfs f0, 0xd4(r28)
    bl fn_800EC5FC
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r27
    bl fn_800ECA78
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r27
    bl fn_800EC9DC
    lfs f1, 0xd4(r28)
    mr r3, r27
    bl fn_800EC5B8
    mr r3, r27
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r27
    bl fn_800EC990
    mr r3, r27
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_800EC4D0
    lfs f1, 0x8(r1)
    mr r3, r27
    lfs f0, lbl_8047D834(r2)
    lfs f2, lbl_8047D7A0(r2)
    fsubs f1, f1, f0
    bl fn_800EC8C8
    li r3, 0x1
@L_8018B758:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
}
#else
void fn_8018B558(void) {
    /* TODO: match -- 532 bytes at 0x8018B558 */
}
#endif

/* 0x8018BC88 | 0x16C */
extern void fn_800EE150(void);
extern void fn_800EE3BC(void);
extern void fn_800EE828(void);
#if 1
asm void fn_8018BC88(void) {
#include "src/game/people/people_fn_8018BC88.inc"
}
#else
void fn_8018BC88(void) {
    /* TODO: match -- 364 bytes at 0x8018BC88 */
}
#endif

/* 0x8018D680 | 0x150 */
extern void fn_800CE2D8(void);
extern void fn_800CE318(void);
extern void fn_800E0718(void);
extern void fn_800DFEEC(void);
extern void fn_800E0040(void);
extern u32 lbl_8047D7A4;
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7F0;
extern u32 lbl_8047D7A8;
extern u32 lbl_8047D820;
extern u8 lbl_8031554C[];
#if 1
asm void fn_8018D680(void) {
    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    fmr f31, f1
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r4, r29
    addi r3, r1, 0x30
    mr r5, r30
    bl fn_800E019C
    addi r3, r1, 0x30
    lfs f1, lbl_8047D7A4(r2)
    mr r4, r3
    bl fn_800E013C
    mr r4, r30
    mr r5, r29
    addi r3, r1, 0x8
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f2, 0x10(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f0, lbl_8047D7A0(r2)
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f1, f0
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f1, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f1, f0
    ble @L_8018D724
    lfd f0, lbl_8047D7F0(r2)
    fsub f1, f1, f0
    frsp f1, f1
    b @L_8018D73C
@L_8018D724:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f1, f0
    bge @L_8018D73C
    lfd f0, lbl_8047D7F0(r2)
    fadd f1, f0, f1
    frsp f1, f1
@L_8018D73C:
    lis r3, lbl_8031554C@ha
    addi r4, r3, lbl_8031554C@l
    addi r3, r1, 0x14
    bl fn_800E0718
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x30
    bl fn_800E0168
    mr r5, r31
    addi r3, r1, 0x24
    addi r4, r1, 0x14
    bl fn_800DFEEC
    mr r4, r30
    addi r3, r1, 0x30
    bl fn_800E0040
    lfs f0, 0x24(r1)
    fabs f0, f0
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    bne @L_8018D7A8
    lfs f0, 0x2c(r1)
    fabs f0, f0
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8018D7A8
    li r3, 0x1
    b @L_8018D7AC
@L_8018D7A8:
    li r3, 0x0
@L_8018D7AC:
    psq_l f31, 0x58(r1), 0, 0
    lwz r0, 0x64(r1)
    lfd f31, 0x50(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
}
#else
void fn_8018D680(void) {
    /* TODO: match -- 336 bytes at 0x8018D680 */
}
#endif

/* 0x8018DB68 | 0x140 */
extern void fn_8018DCA8(void);
#if 1
asm void fn_8018DB68(void) {
#include "src/game/people/people_fn_8018DB68.inc"
}
#else
void fn_8018DB68(void) {
    /* TODO: match -- 320 bytes at 0x8018DB68 */
}
#endif

/* 0x8018DCA8 | 0x3A8 */
extern void fn_8018FB60(void);
extern void fn_8018FB2C(void);
extern void fn_800E24B0(void);
extern void fn_800E209C(void);
extern void fn_800F9210(void);
extern void fn_800E4BF4(void);
extern void fn_8018FDBC(void);
#if 1
asm void fn_8018DCA8(void) {
#include "src/game/people/people_fn_8018DCA8.inc"
}
#else
void fn_8018DCA8(void) {
    /* TODO: match -- 936 bytes at 0x8018DCA8 */
}
#endif

/* 0x8018E9B4 | 0x338 */
extern void fn_800F9318(void);
extern void fn_800F7BC4(void);
extern void fn_8018FC74(void);
extern void fn_80101B90(void);
extern void fn_8018FB94(void);
extern void fn_8018F5E4(void);
extern void fn_8011163C(void);
extern void fn_8012BAF0(void);
extern void fn_8010FDF8(void);
extern void fn_8010F320(void);
extern void fn_800A3A9C(void);
extern void fn_800A3A78(void);
extern void fn_801101B4(void);
extern void fn_8010E138(void);
extern u32 lbl_8047D890;
extern const f32 lbl_8047D7EC;
extern u32 lbl_8047D894;
extern u32 lbl_8047D800;
extern u32 lbl_8047D7A0;
/* renamed symbols referenced by asm incs (symbolmap port) */
extern void heroMoveSetEventList();
extern void sin();   /* MSL trig (renamed) — referenced by asm incs */
extern void cos();   /* MSL trig (renamed) — referenced by asm incs */
extern void GScolsy2UtilGetCpPlanePoint();
extern void GScolsy2UtilChkInTri();
extern void GSmodelPopState();
#if 1
asm void fn_8018E9B4(void) {
#include "src/game/people/people_fn_8018E9B4.inc"
}
#else
void fn_8018E9B4(void) {
    /* TODO: match -- 824 bytes at 0x8018E9B4 */
}
#endif

/* 0x8018ECEC | 0x3A0 */
extern void fn_8018F698(void);
extern void fn_8018F678(void);
extern void fn_8018F658(void);
extern void fn_8018F638(void);
extern void fn_8018F618(void);
extern void fn_800D3088(void);
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7F0;
extern u32 lbl_8047D7A8;
extern u32 lbl_8047D820;
extern u32 lbl_8047D898;
extern u32 lbl_8047D7B0;
extern u32 lbl_8047D7D0;
extern u32 lbl_8047D89C;
#if 1
asm void fn_8018ECEC(void) {
    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stfd f28, 0x70(r1)
    psq_st f28, 0x78(r1), 0, 0
    stfd f27, 0x60(r1)
    psq_st f27, 0x68(r1), 0, 0
    stmw r26, 0x48(r1)
    mr. r31, r3
    fmr f29, f1
    li r27, 0x1
    li r26, 0x0
    beq @L_8018F050
    bl fn_8018FBD4
    mr. r28, r3
    beq @L_8018F050
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_8018EF9C
    lwz r3, 0x30(r31)
    bl fn_8018F6F4
    mr r29, r3
    bl fn_8018F698
    mr r30, r3
    mr r3, r29
    bl fn_8018F678
    fmr f28, f1
    mr r3, r29
    bl fn_8018F658
    fmr f27, f1
    mr r3, r29
    bl fn_8018F638
    fmr f31, f1
    mr r3, r29
    bl fn_8018F618
    extsb. r0, r30
    fmr f30, f1
    blt @L_8018F050
    mr r3, r28
    extsb r4, r30
    bl fn_800EE150
    addi r4, r1, 0x2c
    mr r28, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r28
    bl fn_800EE828
    mr r3, r31
    li r4, 0x2
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EDE0
    li r26, 0x1
    b @L_8018EE10
@L_8018EDE0:
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f29, f0
    bge @L_8018EDF4
    li r26, 0x1
    b @L_8018EE10
@L_8018EDF4:
    lwz r3, 0x18(r31)
    addi r4, r1, 0x2c
    bl fn_800E0040
    fcmpo cr0, f1, f29
    cror eq, lt, eq
    bne @L_8018EE10
    li r26, 0x1
@L_8018EE10:
    clrlwi. r0, r26, 24
    beq @L_8018EF90
    mr r3, r31
    addi r4, r1, 0x20
    bl fn_8018FC2C
    lfs f29, 0x24(r1)
    addi r3, r1, 0x8
    lwz r4, 0x18(r31)
    addi r5, r1, 0x2c
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f2, 0x10(r1)
    bl fn_800CE2D8
    frsp f0, f1
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f0, f29
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f1, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f1, f0
    ble @L_8018EE78
    lfd f0, lbl_8047D7F0(r2)
    fsub f1, f1, f0
    frsp f1, f1
    b @L_8018EE90
@L_8018EE78:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f1, f0
    bge @L_8018EE90
    lfd f0, lbl_8047D7F0(r2)
    fadd f1, f0, f1
    frsp f1, f1
@L_8018EE90:
    fmr f29, f1
    fcmpo cr0, f1, f28
    bge @L_8018EEC0
    mr r3, r31
    li r4, 0x2
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EEB8
    fmr f29, f28
    b @L_8018EEE8
@L_8018EEB8:
    li r27, 0x0
    b @L_8018EEE8
@L_8018EEC0:
    fcmpo cr0, f1, f27
    ble @L_8018EEE8
    mr r3, r31
    li r4, 0x2
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EEE4
    fmr f29, f27
    b @L_8018EEE8
@L_8018EEE4:
    li r27, 0x0
@L_8018EEE8:
    clrlwi. r0, r27, 24
    beq @L_8018EF80
    stfs f29, 0xb8(r31)
    addi r3, r1, 0x14
    addi r5, r1, 0x2c
    lwz r4, 0x18(r31)
    bl fn_800E0168
    lfs f1, 0x18(r1)
    lfs f0, lbl_8047D898(r2)
    lfs f3, 0x1c(r1)
    fadds f2, f1, f0
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f3, f0
    stfs f2, 0x18(r1)
    ble @L_8018EF28
    b @L_8018EF2C
@L_8018EF28:
    fneg f3, f3
@L_8018EF2C:
    lfs f1, 0x14(r1)
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    ble @L_8018EF40
    b @L_8018EF44
@L_8018EF40:
    fneg f1, f1
@L_8018EF44:
    fadds f1, f1, f3
    bl fn_800CE2D8
    frsp f1, f1
    lfd f0, lbl_8047D7B0(r2)
    fsub f0, f1, f0
    frsp f0, f0
    fcmpo cr0, f0, f31
    bge @L_8018EF6C
    fmr f0, f31
    b @L_8018EF78
@L_8018EF6C:
    fcmpo cr0, f0, f30
    ble @L_8018EF78
    fmr f0, f30
@L_8018EF78:
    stfs f0, 0xb4(r31)
    b @L_8018EF9C
@L_8018EF80:
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xb4(r31)
    stfs f0, 0xb8(r31)
    b @L_8018EF9C
@L_8018EF90:
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xb4(r31)
    stfs f0, 0xb8(r31)
@L_8018EF9C:
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x3c(r1)
    lfd f2, lbl_8047D7D0(r2)
    stw r0, 0x38(r1)
    lfs f0, lbl_8047D89C(r2)
    lfd f1, 0x38(r1)
    lfs f3, 0xb4(r31)
    fsubs f1, f1, f2
    lfs f2, 0xc(r31)
    fcmpo cr0, f2, f3
    fmuls f4, f0, f1
    ble @L_8018EFEC
    fsubs f0, f2, f4
    stfs f0, 0xc(r31)
    lfs f0, 0xc(r31)
    fcmpo cr0, f0, f3
    bge @L_8018F008
    stfs f3, 0xc(r31)
    b @L_8018F008
@L_8018EFEC:
    bge @L_8018F008
    fadds f0, f2, f4
    stfs f0, 0xc(r31)
    lfs f0, 0xc(r31)
    fcmpo cr0, f0, f3
    ble @L_8018F008
    stfs f3, 0xc(r31)
@L_8018F008:
    lfs f1, 0xb8(r31)
    lfs f0, 0x10(r31)
    fcmpo cr0, f0, f1
    ble @L_8018F034
    fsubs f0, f0, f4
    stfs f0, 0x10(r31)
    lfs f0, 0x10(r31)
    fcmpo cr0, f0, f1
    bge @L_8018F050
    stfs f1, 0x10(r31)
    b @L_8018F050
@L_8018F034:
    bge @L_8018F050
    fadds f0, f0, f4
    stfs f0, 0x10(r31)
    lfs f0, 0x10(r31)
    fcmpo cr0, f0, f1
    ble @L_8018F050
    stfs f1, 0x10(r31)
@L_8018F050:
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    psq_l f28, 0x78(r1), 0, 0
    lfd f28, 0x70(r1)
    psq_l f27, 0x68(r1), 0, 0
    lfd f27, 0x60(r1)
    lmw r26, 0x48(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
}
#else
void fn_8018ECEC(void) {
    /* TODO: match -- 928 bytes at 0x8018ECEC */
}
#endif

/* WP-0010 stubs */

/* 0x80181478 | 0x3D8 */
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7A4;
#if 1
asm void fn_80181478(void) {
#include "src/game/people/people_fn_80181478.inc"
}
#else
void fn_80181478(void) { /* TODO: match -- 984 bytes at 0x80181478 */ }
#endif

/* 0x80181EB0 | 0x308 */
extern void fn_80113F48(void);
extern void fn_801CBA0C(void);
extern void fn_801845E4(void);
extern void fn_800E3CC8(void);
extern void fn_800E90C8(void);
extern void fn_801CB834(void);
extern void fn_80166A28(void);
extern void fn_800F7318(void);
#if 1
asm void fn_80181EB0(void) {
#include "src/game/people/people_fn_80181EB0.inc"
}
#else
void fn_80181EB0(void) { /* TODO: match -- 776 bytes at 0x80181EB0 */ }
#endif

/* 0x801821B8 | 0xE60 */
extern void fn_8018FBAC(void);
extern void fn_8018FBDC(void);
extern u32 lbl_8047D7D8;
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7A4;
extern u32 lbl_8047D7D0;
extern u32 lbl_8047D7E0;
extern u8 lbl_8036C4F8[];
#if 1
asm void fn_801821B8(void) {
#include "src/game/people/people_fn_801821B8.inc"
}
#else
void fn_801821B8(void) { /* TODO: match -- 3680 bytes at 0x801821B8 */ }
#endif

/* 0x801837D8 | 0x180 */
#if 1
asm void fn_801837D8(void) {
#include "src/game/people/people_fn_801837D8.inc"
}
#else
void fn_801837D8(void) { /* TODO: match -- 384 bytes at 0x801837D8 */ }
#endif

/* 0x80183958 | 0x24 */
extern void fn_801170A4(void);
extern void fn_80116EC8(void);
#if 0
asm void fn_80183958(void) {
#include "src/game/people/people_fn_80183958.inc"
}
#else
#pragma optimization_level 4
void fn_80183958(void) {
    fn_801170A4();
    fn_80116EC8();
}
#endif

/* 0x801839A0 | 0x1A4 */
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D79C;
#if 1
asm void fn_801839A0(void) {
#include "src/game/people/people_fn_801839A0.inc"
}
#else
void fn_801839A0(void) { /* TODO: match -- 420 bytes at 0x801839A0 */ }
#endif

/* 0x80183B44 | 0x19C */
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7E8;
extern const f32 lbl_8047D7EC;
extern u32 lbl_8047D79C;
#if 1
asm void fn_80183B44(void) {
    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    fmr f31, f1
    mr r28, r3
    mr r29, r4
    li r31, 0x0
    b @L_80183BB4
@L_80183B7C:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183BB0
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80183BB0
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80183BB0
    lwz r31, 0x4(r3)
    b @L_80183C20
@L_80183BB0:
    addi r31, r31, 0x1
@L_80183BB4:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80183B7C
    li r30, 0x0
    b @L_80183C10
@L_80183BC8:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183C0C
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_80183C0C
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_80183C20
@L_80183C0C:
    addi r30, r30, 0x1
@L_80183C10:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80183BC8
    li r31, 0x0
@L_80183C20:
    li r30, 0x0
    b @L_80183C54
@L_80183C28:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183C50
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_80183C50
    mr r31, r3
    b @L_80183C64
@L_80183C50:
    addi r30, r30, 0x1
@L_80183C54:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80183C28
    li r31, 0x0
@L_80183C64:
    cmplwi r31, 0x0
    bne @L_80183C74
    li r3, 0x0
    b @L_80183CB8
@L_80183C74:
    li r3, 0x4
    li r0, 0x0
    stb r3, 0x54(r31)
    mr r3, r31
    addi r4, r31, 0x74
    stb r0, 0x55(r31)
    bl fn_8018FC98
    stfs f31, 0x80(r31)
    li r3, 0x1
    lfs f1, lbl_8047D7A0(r2)
    lfs f0, lbl_8047D7E8(r2)
    stfs f1, 0x84(r31)
    lfs f1, lbl_8047D7EC(r2)
    stfs f0, 0x88(r31)
    lfs f0, lbl_8047D79C(r2)
    stfs f1, 0x8c(r31)
    stfs f0, 0x58(r31)
@L_80183CB8:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
}
#else
void fn_80183B44(void) { /* TODO: match -- 412 bytes at 0x80183B44 */ }
#endif

/* 0x80183CE0 | 0x17C */
#if 1
asm void fn_80183CE0(void) {
#include "src/game/people/people_fn_80183CE0.inc"
}
#else
void fn_80183CE0(void) { /* TODO: match -- 380 bytes at 0x80183CE0 */ }
#endif

/* 0x80183E5C | 0x168 */
extern u32 lbl_8047D79C;
#if 1
asm void fn_80183E5C(void) {
    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r31, r5
    li r30, 0x0
    b @L_80183EB8
@L_80183E80:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183EB4
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_80183EB4
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80183EB4
    lwz r30, 0x4(r3)
    b @L_80183F24
@L_80183EB4:
    addi r30, r30, 0x1
@L_80183EB8:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80183E80
    li r29, 0x0
    b @L_80183F14
@L_80183ECC:
    mr r3, r29
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183F10
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80183F10
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r30, 0x4(r30)
    b @L_80183F24
@L_80183F10:
    addi r29, r29, 0x1
@L_80183F14:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80183ECC
    li r30, 0x0
@L_80183F24:
    li r29, 0x0
    b @L_80183F54
@L_80183F2C:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183F50
    lwz r0, 0x4(r3)
    cmplw r0, r30
    bne @L_80183F50
    b @L_80183F64
@L_80183F50:
    addi r29, r29, 0x1
@L_80183F54:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80183F2C
    li r3, 0x0
@L_80183F64:
    cmplwi r3, 0x0
    bne @L_80183F74
    li r3, 0x0
    b @L_80183FB0
@L_80183F74:
    li r4, 0x0
    lfs f0, lbl_8047D79C(r2)
    stb r4, 0x55(r3)
    cmplwi r31, 0x0
    li r0, 0x1
    sth r4, 0x6a(r3)
    stfs f0, 0x58(r3)
    stb r0, 0x55(r3)
    beq @L_80183FA4
    li r0, 0x3
    stb r0, 0x54(r3)
    b @L_80183FAC
@L_80183FA4:
    li r0, 0x2
    stb r0, 0x54(r3)
@L_80183FAC:
    li r3, 0x1
@L_80183FB0:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
}
#else
void fn_80183E5C(void) { /* TODO: match -- 360 bytes at 0x80183E5C */ }
#endif

/* 0x80183FC4 | 0x1CC */
extern u8 lbl_8027404C[];
#if 1
asm void fn_80183FC4(void) {
#include "src/game/people/people_fn_80183FC4.inc"
}
#else
void fn_80183FC4(void) { /* TODO: match -- 460 bytes at 0x80183FC4 */ }
#endif

/* 0x80184190 | 0x2C0 */
#if 1
asm void fn_80184190(void) {
#include "src/game/people/people_fn_80184190.inc"
}
#else
void fn_80184190(void) { /* TODO: match -- 704 bytes at 0x80184190 */ }
#endif

/* 0x80184450 | 0x20 */
#if 0
asm void fn_80184450(void) {
#include "src/game/people/people_fn_80184450.inc"
}
#else
#pragma optimization_level 4
void fn_80184450(void) {
    _threadSwitch();
}
#endif

/* 0x80184948 | 0x148 */
#if 1
asm void fn_80184948(void) {
#include "src/game/people/people_fn_80184948.inc"
}
#else
void fn_80184948(void) { /* TODO: match -- 328 bytes at 0x80184948 */ }
#endif

/* 0x80184A90 | 0x2F0 */
extern void fn_8018FC08(void);
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7F0;
extern u32 lbl_8047D7F8;
extern u32 lbl_8047D7A8;
extern u32 lbl_8047D7FC;
extern u32 lbl_8047D7C0;
extern u32 lbl_8047D804;
extern u32 lbl_8047D800;
extern u32 lbl_8047D808;
extern u32 lbl_8047D810;
extern u32 lbl_8047D814;
extern u32 lbl_8047D7D0;
#if 1
asm void fn_80184A90(void) {
    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r3
    li r30, 0x1
    lbz r31, 0x22(r3)
    cmplwi r31, 0x0
    beq @L_80184D54
    addi r4, r1, 0x8
    bl fn_8018FC2C
    mr r3, r29
    lis r4, 0x4000
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_80184B04
    mr r3, r29
    li r30, 0x0
    li r4, 0x8
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_80184B04
    li r30, 0x1
@L_80184B04:
    clrlwi. r0, r30, 24
    beq @L_80184C4C
    lfs f2, 0xc(r1)
    lfs f1, lbl_8047D7A0(r2)
    fcmpo cr0, f2, f1
    bge @L_80184B38
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B2C
@L_80184B24:
    fadd f2, f2, f0
    frsp f2, f2
@L_80184B2C:
    fcmpo cr0, f2, f1
    blt @L_80184B24
    b @L_80184B54
@L_80184B38:
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B48
@L_80184B40:
    fsub f2, f2, f0
    frsp f2, f2
@L_80184B48:
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    beq @L_80184B40
@L_80184B54:
    stfs f2, 0xc(r1)
    lfs f1, lbl_8047D7A0(r2)
    lfs f31, 0x40(r29)
    fcmpo cr0, f31, f1
    bge @L_80184B84
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B78
@L_80184B70:
    fadd f31, f31, f0
    frsp f31, f31
@L_80184B78:
    fcmpo cr0, f31, f1
    blt @L_80184B70
    b @L_80184BA0
@L_80184B84:
    lfd f0, lbl_8047D7F0(r2)
    b @L_80184B94
@L_80184B8C:
    fsub f31, f31, f0
    frsp f31, f31
@L_80184B94:
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    beq @L_80184B8C
@L_80184BA0:
    lfs f1, lbl_8047D7F8(r2)
    lfs f0, 0x44(r29)
    fmuls f30, f1, f0
    bl fn_800D3088
    lfd f2, lbl_8047D7A8(r2)
    lfs f1, lbl_8047D7A0(r2)
    mtctr r3
    cmplwi r3, 0x0
    beq @L_80184D44
@L_80184BC4:
    lfs f0, 0xc(r1)
    fsubs f3, f31, f0
    fabs f0, f3
    fcmpo cr0, f0, f2
    cror eq, gt, eq
    bne @L_80184C04
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f3, f0
    bge @L_80184BF8
    lfd f0, lbl_8047D7F0(r2)
    fadd f3, f0, f3
    frsp f3, f3
    b @L_80184C04
@L_80184BF8:
    lfd f0, lbl_8047D7F0(r2)
    fsub f3, f3, f0
    frsp f3, f3
@L_80184C04:
    fabs f0, f3
    fcmpo cr0, f0, f30
    cror eq, lt, eq
    bne @L_80184C20
    stfs f31, 0xc(r1)
    li r31, 0x0
    b @L_80184D44
@L_80184C20:
    fcmpo cr0, f3, f1
    ble @L_80184C38
    lfs f0, 0xc(r1)
    fadds f0, f0, f30
    stfs f0, 0xc(r1)
    b @L_80184C44
@L_80184C38:
    lfs f0, 0xc(r1)
    fsubs f0, f0, f30
    stfs f0, 0xc(r1)
@L_80184C44:
    bdnz @L_80184BC4
    b @L_80184D44
@L_80184C4C:
    lfs f2, 0xc(r1)
    lfs f3, 0x40(r29)
    lfs f1, lbl_8047D7FC(r2)
    fsubs f0, f2, f3
    fcmpo cr0, f0, f1
    ble @L_80184C74
    lfs f0, lbl_8047D7C0(r2)
    fsubs f0, f2, f0
    stfs f0, 0xc(r1)
    b @L_80184C8C
@L_80184C74:
    fsubs f0, f3, f2
    fcmpo cr0, f0, f1
    ble @L_80184C8C
    lfs f0, lbl_8047D7C0(r2)
    fadds f0, f2, f0
    stfs f0, 0xc(r1)
@L_80184C8C:
    lfs f1, lbl_8047D804(r2)
    lfs f0, 0x44(r29)
    lfs f4, 0x40(r29)
    lfs f3, 0xc(r1)
    fsubs f1, f1, f0
    lfs f2, lbl_8047D800(r2)
    fsubs f3, f4, f3
    lfd f0, lbl_8047D808(r2)
    fmuls f1, f2, f1
    fdivs f31, f3, f1
    fabs f1, f31
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80184CDC
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f31, f0
    bge @L_80184CD8
    lfs f31, lbl_8047D810(r2)
    b @L_80184CDC
@L_80184CD8:
    lfs f31, lbl_8047D814(r2)
@L_80184CDC:
    bl fn_800D3088
    lis r0, 0x4330
    lfs f0, lbl_8047D7A0(r2)
    stw r3, 0x1c(r1)
    lfd f2, lbl_8047D7D0(r2)
    fcmpo cr0, f31, f0
    stw r0, 0x18(r1)
    lfs f0, 0xc(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f2
    fmadds f0, f31, f1, f0
    stfs f0, 0xc(r1)
    bge @L_80184D2C
    lfs f1, 0x40(r29)
    fcmpo cr0, f0, f1
    cror eq, lt, eq
    bne @L_80184D44
    stfs f1, 0xc(r1)
    li r31, 0x0
    b @L_80184D44
@L_80184D2C:
    lfs f1, 0x40(r29)
    fcmpo cr0, f0, f1
    cror eq, gt, eq
    bne @L_80184D44
    stfs f1, 0xc(r1)
    li r31, 0x0
@L_80184D44:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8018FC08
    stb r31, 0x22(r29)
@L_80184D54:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r0, 0x54(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
}
#else
void fn_80184A90(void) { /* TODO: match -- 752 bytes at 0x80184A90 */ }
#endif

/* 0x80184D80 | 0x4CC */
extern void fn_800D37CC(void);
extern void fn_800E0BE4(void);
extern void fn_800CE148(void);
extern void fn_800CDBE0(void);
extern void fn_800E0BA0(void);
extern u8 lbl_80273FC0[];
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7C8;
extern u32 lbl_8047D7D0;
extern u32 lbl_8047D7C4;
extern u32 lbl_8047D818;
extern u32 lbl_8047D7F0;
extern u32 lbl_8047D7A8;
extern u32 lbl_8047D820;
extern u32 lbl_8047D7C0;
extern u32 lbl_8047D79C;
extern void fn_80185AAC(void);
#if 1
asm void fn_80184D80(void) {
    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stmw r27, 0x4c(r1)
    lis r4, lbl_80273FC0@ha
    mr r31, r3
    addi r5, r4, lbl_80273FC0@l
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x2c(r1)
    stw r3, 0x30(r1)
    stw r0, 0x34(r1)
    lbz r0, 0x55(r31)
    cmpwi r0, 0x1
    beq @L_80184E60
    bge @L_80184DD8
    cmpwi r0, 0x0
    bge @L_80184DE4
    b @L_80185230
@L_80184DD8:
    cmpwi r0, 0x3
    bge @L_80185230
    b @L_80185088
@L_80184DE4:
    lfs f1, 0x84(r31)
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    ble @L_80184E58
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x3c(r1)
    lfd f1, lbl_8047D7C8(r2)
    stw r0, 0x38(r1)
    lfd f0, 0x38(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x44(r1)
    lfd f3, lbl_8047D7D0(r2)
    stw r0, 0x40(r1)
    lfs f1, 0x84(r31)
    lfd f2, 0x40(r1)
    lfs f0, lbl_8047D7A0(r2)
    fsubs f2, f2, f3
    fdivs f2, f2, f31
    fsubs f1, f1, f2
    stfs f1, 0x84(r31)
    lfs f1, 0x84(r31)
    fcmpo cr0, f1, f0
    bge @L_80185230
    stfs f0, 0x84(r31)
    b @L_80185230
@L_80184E58:
    li r0, 0x1
    stb r0, 0x55(r31)
@L_80184E60:
    bl fn_800E0BE4
    lfs f2, lbl_8047D7C4(r2)
    lfs f0, lbl_8047D818(r2)
    fmuls f1, f2, f1
    fmuls f31, f0, f1
    fmr f1, f31
    bl sin
    lfs f0, 0x80(r31)
    fmul f0, f0, f1
    fmr f1, f31
    frsp f0, f0
    stfs f0, 0x2c(r1)
    bl cos
    lfs f0, 0x80(r31)
    addi r3, r31, 0x5c
    addi r4, r31, 0x74
    addi r5, r1, 0x2c
    fmul f0, f0, f1
    frsp f0, f0
    stfs f0, 0x34(r1)
    bl fn_800E019C
    mr r3, r31
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x20
    addi r4, r31, 0x5c
    bl fn_800E0168
    lfs f1, 0x20(r1)
    lfs f2, 0x28(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f0, lbl_8047D7A0(r2)
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f1, f0
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f31, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f31, f0
    ble @L_80184F10
    lfd f0, lbl_8047D7F0(r2)
    fsub f31, f31, f0
    frsp f31, f31
    b @L_80184F28
@L_80184F10:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f31, f0
    bge @L_80184F28
    lfd f0, lbl_8047D7F0(r2)
    fadd f31, f0, f31
    frsp f31, f31
@L_80184F28:
    lwz r28, 0x2c(r31)
    li r30, 0x0
    lwz r29, 0x28(r31)
    b @L_80184F70
@L_80184F38:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184F6C
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_80184F6C
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80184F6C
    lwz r28, 0x4(r3)
    b @L_80184FDC
@L_80184F6C:
    addi r30, r30, 0x1
@L_80184F70:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80184F38
    li r27, 0x0
    b @L_80184FCC
@L_80184F84:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184FC8
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80184FC8
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_80184FDC
@L_80184FC8:
    addi r27, r27, 0x1
@L_80184FCC:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80184F84
    li r28, 0x0
@L_80184FDC:
    li r29, 0x0
    b @L_80185010
@L_80184FE4:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018500C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018500C
    mr r28, r3
    b @L_80185020
@L_8018500C:
    addi r29, r29, 0x1
@L_80185010:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80184FE4
    li r28, 0x0
@L_80185020:
    cmplwi r28, 0x0
    beq @L_80185080
    mr r3, r28
    addi r4, r1, 0x14
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0x18(r1)
    li r0, 0x1
    stw r3, 0x38(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D79C(r2)
    fctiwz f1, f1
    stfd f1, 0x40(r1)
    lwz r0, 0x44(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x3c(r1)
    lfd f1, 0x38(r1)
    fsubs f1, f1, f2
    fmadds f1, f3, f1, f31
    stfs f1, 0x40(r28)
    stfs f0, 0x44(r28)
@L_80185080:
    li r0, 0x2
    stb r0, 0x55(r31)
@L_80185088:
    mr r3, r31
    bl fn_80185AAC
    cmpwi r3, 0x2
    bne @L_8018520C
    lfd f1, lbl_8047D7A8(r2)
    li r30, 0x0
    lfs f0, 0x40(r31)
    lwz r29, 0x2c(r31)
    fadd f31, f1, f0
    lwz r28, 0x28(r31)
    frsp f31, f31
    b @L_801850F0
@L_801850B8:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801850EC
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_801850EC
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_801850EC
    lwz r28, 0x4(r3)
    b @L_8018515C
@L_801850EC:
    addi r30, r30, 0x1
@L_801850F0:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801850B8
    li r27, 0x0
    b @L_8018514C
@L_80185104:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185148
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_80185148
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_8018515C
@L_80185148:
    addi r27, r27, 0x1
@L_8018514C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80185104
    li r28, 0x0
@L_8018515C:
    li r29, 0x0
    b @L_80185190
@L_80185164:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018518C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018518C
    mr r28, r3
    b @L_801851A0
@L_8018518C:
    addi r29, r29, 0x1
@L_80185190:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80185164
    li r28, 0x0
@L_801851A0:
    cmplwi r28, 0x0
    beq @L_80185200
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x38(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D79C(r2)
    fctiwz f1, f1
    stfd f1, 0x40(r1)
    lwz r0, 0x44(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x3c(r1)
    lfd f1, 0x38(r1)
    fsubs f1, f1, f2
    fmadds f31, f3, f1, f31
    stfs f31, 0x40(r28)
    stfs f0, 0x44(r28)
@L_80185200:
    li r0, 0x2
    stb r0, 0x55(r31)
    b @L_80185230
@L_8018520C:
    cmpwi r3, 0x1
    bne @L_80185230
    bl fn_800E0BA0
    lfs f2, 0x8c(r31)
    li r0, 0x0
    lfs f0, 0x88(r31)
    fmadds f0, f2, f1, f0
    stfs f0, 0x84(r31)
    stb r0, 0x55(r31)
@L_80185230:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    lmw r27, 0x4c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
}
#else
void fn_80184D80(void) { /* TODO: match -- 1228 bytes at 0x80184D80 */ }
#endif

/* 0x8018524C | 0x678 */
extern void fn_80188214(void);
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7F0;
extern u32 lbl_8047D7A8;
extern u32 lbl_8047D820;
extern u32 lbl_8047D7C0;
extern u32 lbl_8047D7C8;
extern u32 lbl_8047D79C;
extern u32 lbl_8047D7C4;
#if 1
asm void fn_8018524C(void) {
    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stfd f31, 0xc0(r1)
    psq_st f31, 0xc8(r1), 0, 0
    stfd f30, 0xb0(r1)
    psq_st f30, 0xb8(r1), 0, 0
    stfd f29, 0xa0(r1)
    psq_st f29, 0xa8(r1), 0, 0
    stfd f28, 0x90(r1)
    psq_st f28, 0x98(r1), 0, 0
    stfd f27, 0x80(r1)
    psq_st f27, 0x88(r1), 0, 0
    stfd f26, 0x70(r1)
    psq_st f26, 0x78(r1), 0, 0
    stmw r27, 0x5c(r1)
    mr r31, r3
    lbz r0, 0x55(r3)
    cmpwi r0, 0x1
    beq @L_801852B0
    bge @L_801852A4
    b @L_80185880
@L_801852A4:
    cmpwi r0, 0x3
    bge @L_80185880
    b @L_8018549C
@L_801852B0:
    lhz r0, 0x6a(r31)
    addi r3, r31, 0x5c
    lwz r4, 0x70(r31)
    mulli r0, r0, 0xc
    add r4, r4, r0
    bl fn_800E01D0
    mr r3, r31
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x2c
    addi r4, r31, 0x5c
    bl fn_800E0168
    lfs f1, 0x2c(r1)
    lfs f2, 0x34(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f0, lbl_8047D7A0(r2)
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f1, f0
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f27, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f27, f0
    ble @L_80185324
    lfd f0, lbl_8047D7F0(r2)
    fsub f27, f27, f0
    frsp f27, f27
    b @L_8018533C
@L_80185324:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f27, f0
    bge @L_8018533C
    lfd f0, lbl_8047D7F0(r2)
    fadd f27, f0, f27
    frsp f27, f27
@L_8018533C:
    lwz r28, 0x2c(r31)
    li r30, 0x0
    lwz r29, 0x28(r31)
    b @L_80185384
@L_8018534C:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185380
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_80185380
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80185380
    lwz r28, 0x4(r3)
    b @L_801853F0
@L_80185380:
    addi r30, r30, 0x1
@L_80185384:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_8018534C
    li r27, 0x0
    b @L_801853E0
@L_80185398:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801853DC
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_801853DC
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_801853F0
@L_801853DC:
    addi r27, r27, 0x1
@L_801853E0:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80185398
    li r28, 0x0
@L_801853F0:
    li r29, 0x0
    b @L_80185424
@L_801853F8:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185420
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80185420
    mr r28, r3
    b @L_80185434
@L_80185420:
    addi r29, r29, 0x1
@L_80185424:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801853F8
    li r28, 0x0
@L_80185434:
    cmplwi r28, 0x0
    beq @L_80185494
    mr r3, r28
    addi r4, r1, 0x20
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0x24(r1)
    li r0, 0x1
    stw r3, 0x50(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D79C(r2)
    fctiwz f1, f1
    stfd f1, 0x48(r1)
    lwz r0, 0x4c(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x54(r1)
    lfd f1, 0x50(r1)
    fsubs f1, f1, f2
    fmadds f1, f3, f1, f27
    stfs f1, 0x40(r28)
    stfs f0, 0x44(r28)
@L_80185494:
    li r0, 0x2
    stb r0, 0x55(r31)
@L_8018549C:
    lwz r3, 0x28(r31)
    lwz r4, 0x2c(r31)
    lfs f1, 0x58(r31)
    bl fn_80188214
    mr r3, r31
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x38
    addi r4, r31, 0x9c
    bl fn_800E0168
    addi r3, r1, 0x38
    bl fn_800E008C
    fmr f31, f1
    lfd f28, lbl_8047D7F0(r2)
    lfs f29, lbl_8047D7A0(r2)
    lfd f30, lbl_8047D7A8(r2)
@L_801854DC:
    addi r3, r1, 0x38
    addi r4, r31, 0x9c
    addi r5, r31, 0x5c
    bl fn_800E0168
    addi r3, r1, 0x38
    bl fn_800E008C
    fmr f27, f1
    fcmpo cr0, f27, f31
    bge @L_80185840
    addi r3, r31, 0x9c
    addi r4, r31, 0x5c
    bl fn_800E01D0
    lhz r3, 0x6a(r31)
    addi r0, r3, 0x1
    sth r0, 0x6a(r31)
    lhz r3, 0x6a(r31)
    lhz r0, 0x68(r31)
    cmplw r3, r0
    blt @L_80185664
    lbz r0, 0x54(r31)
    cmplwi r0, 0x3
    bne @L_80185540
    li r0, 0x0
    sth r0, 0x6a(r31)
    b @L_80185664
@L_80185540:
    lwz r29, 0x2c(r31)
    li r30, 0x0
    lwz r28, 0x28(r31)
    b @L_80185588
@L_80185550:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185584
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80185584
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80185584
    lwz r28, 0x4(r3)
    b @L_801855F4
@L_80185584:
    addi r30, r30, 0x1
@L_80185588:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80185550
    li r27, 0x0
    b @L_801855E4
@L_8018559C:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801855E0
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_801855E0
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_801855F4
@L_801855E0:
    addi r27, r27, 0x1
@L_801855E4:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018559C
    li r28, 0x0
@L_801855F4:
    li r29, 0x0
    b @L_80185628
@L_801855FC:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185624
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80185624
    mr r28, r3
    b @L_80185638
@L_80185624:
    addi r29, r29, 0x1
@L_80185628:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801855FC
    li r28, 0x0
@L_80185638:
    cmplwi r28, 0x0
    beq @L_80185658
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FC74
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FBDC
@L_80185658:
    li r0, 0x0
    stb r0, 0x54(r31)
    b @L_80185880
@L_80185664:
    lhz r0, 0x6a(r31)
    addi r3, r31, 0x5c
    lwz r4, 0x70(r31)
    mulli r0, r0, 0xc
    add r4, r4, r0
    bl fn_800E01D0
    addi r3, r1, 0x14
    addi r4, r31, 0x5c
    addi r5, r31, 0x9c
    bl fn_800E0168
    lfs f1, 0x14(r1)
    lfs f2, 0x1c(r1)
    bl fn_800CE2D8
    frsp f0, f1
    fmr f2, f28
    fsubs f0, f0, f29
    fadd f1, f28, f0
    bl fn_800CE318
    frsp f26, f1
    fcmpo cr0, f26, f30
    ble @L_801856C8
    lfd f0, lbl_8047D7F0(r2)
    fsub f26, f26, f0
    frsp f26, f26
    b @L_801856E0
@L_801856C8:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f26, f0
    bge @L_801856E0
    lfd f0, lbl_8047D7F0(r2)
    fadd f26, f0, f26
    frsp f26, f26
@L_801856E0:
    lwz r29, 0x2c(r31)
    li r30, 0x0
    lwz r28, 0x28(r31)
    b @L_80185728
@L_801856F0:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185724
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80185724
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80185724
    lwz r28, 0x4(r3)
    b @L_80185794
@L_80185724:
    addi r30, r30, 0x1
@L_80185728:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801856F0
    li r27, 0x0
    b @L_80185784
@L_8018573C:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185780
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_80185780
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_80185794
@L_80185780:
    addi r27, r27, 0x1
@L_80185784:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_8018573C
    li r28, 0x0
@L_80185794:
    li r29, 0x0
    b @L_801857C8
@L_8018579C:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801857C4
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_801857C4
    mr r28, r3
    b @L_801857D8
@L_801857C4:
    addi r29, r29, 0x1
@L_801857C8:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018579C
    li r28, 0x0
@L_801857D8:
    cmplwi r28, 0x0
    beq @L_80185838
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x48(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D7C4(r2)
    fctiwz f1, f1
    stfd f1, 0x50(r1)
    lwz r0, 0x54(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x4c(r1)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fmadds f1, f3, f1, f26
    stfs f1, 0x40(r28)
    stfs f0, 0x44(r28)
@L_80185838:
    fsubs f31, f31, f27
    b @L_801854DC
@L_80185840:
    addi r3, r1, 0x38
    addi r4, r31, 0x5c
    addi r5, r31, 0x9c
    bl fn_800E0168
    fdivs f1, f31, f27
    addi r3, r1, 0x38
    mr r4, r3
    bl fn_800E013C
    addi r3, r1, 0x38
    addi r4, r31, 0x9c
    mr r5, r3
    bl fn_800E019C
    mr r3, r31
    addi r4, r1, 0x38
    addi r5, r31, 0x9c
    bl fn_8018E9B4
@L_80185880:
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    psq_l f30, 0xb8(r1), 0, 0
    lfd f30, 0xb0(r1)
    psq_l f29, 0xa8(r1), 0, 0
    lfd f29, 0xa0(r1)
    psq_l f28, 0x98(r1), 0, 0
    lfd f28, 0x90(r1)
    psq_l f27, 0x88(r1), 0, 0
    lfd f27, 0x80(r1)
    psq_l f26, 0x78(r1), 0, 0
    lfd f26, 0x70(r1)
    lmw r27, 0x5c(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
}
#else
void fn_8018524C(void) { /* TODO: match -- 1656 bytes at 0x8018524C */ }
#endif

/* 0x801858C4 | 0x1E8 */
extern void fn_8018FC00(void);
extern u32 lbl_8047D828;
#if 1
asm void fn_801858C4(void) {
    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r31, r3
    bl fn_80185AAC
    cmpwi r3, 0x0
    beq @L_801858EC
    li r0, 0x0
    stb r0, 0x54(r31)
@L_801858EC:
    cmplwi r31, 0x0
    beq @L_80185A98
    mr r3, r31
    bl fn_8018FCBC
    mr r0, r3
    mr r3, r31
    mr r27, r0
    bl fn_8018FC00
    mr r5, r3
    mr r4, r27
    addi r3, r1, 0x8
    bl fn_800E0168
    lfs f0, 0x8(r1)
    lfd f1, lbl_8047D828(r2)
    fabs f0, f0
    fcmpo cr0, f0, f1
    bge @L_80185A90
    lfs f0, 0xc(r1)
    fabs f0, f0
    fcmpo cr0, f0, f1
    bge @L_80185A90
    lfs f0, 0x10(r1)
    fabs f0, f0
    fcmpo cr0, f0, f1
    bge @L_80185A90
    lbz r3, 0x97(r31)
    addi r0, r3, 0x1
    stb r0, 0x97(r31)
    lbz r0, 0x97(r31)
    cmplwi r0, 0x3c
    ble @L_80185A98
    lwz r28, 0x2c(r31)
    li r30, 0x0
    lwz r29, 0x28(r31)
    b @L_801859B0
@L_80185978:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801859AC
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_801859AC
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_801859AC
    lwz r28, 0x4(r3)
    b @L_80185A1C
@L_801859AC:
    addi r30, r30, 0x1
@L_801859B0:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80185978
    li r27, 0x0
    b @L_80185A0C
@L_801859C4:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185A08
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80185A08
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_80185A1C
@L_80185A08:
    addi r27, r27, 0x1
@L_80185A0C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_801859C4
    li r28, 0x0
@L_80185A1C:
    li r29, 0x0
    b @L_80185A50
@L_80185A24:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185A4C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80185A4C
    mr r28, r3
    b @L_80185A60
@L_80185A4C:
    addi r29, r29, 0x1
@L_80185A50:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80185A24
    li r28, 0x0
@L_80185A60:
    cmplwi r28, 0x0
    beq @L_80185A80
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FC74
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FBDC
@L_80185A80:
    li r0, 0x0
    stb r0, 0x54(r31)
    stb r0, 0x97(r31)
    b @L_80185A98
@L_80185A90:
    li r0, 0x0
    stb r0, 0x97(r31)
@L_80185A98:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
}
#else
void fn_801858C4(void) { /* TODO: match -- 488 bytes at 0x801858C4 */ }
#endif

/* 0x80185AAC | 0xE4 */
#if 1
asm void fn_80185AAC(void) {
#include "src/game/people/people_fn_80185AAC.inc"
}
#else
void fn_80185AAC(void) { /* TODO: match -- 228 bytes at 0x80185AAC */ }
#endif

/* 0x80185B90 | 0x358 */
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D7D0;
extern u32 lbl_8047D79C;
extern u32 lbl_8047D830;
extern u32 lbl_8047D834;
extern u32 lbl_8047D7A4;
#if 1
asm void fn_80185B90(void) {
    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stmw r25, 0x24(r1)
    mr r27, r3
    fmr f31, f1
    bl fn_8018FBD4
    mr. r25, r3
    beq @L_80185ECC
    lfs f1, lbl_8047D7A0(r2)
    lfs f0, 0xd8(r27)
    fcmpu cr0, f1, f0
    beq @L_80185C20
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f3, lbl_8047D7D0(r2)
    stw r0, 0x18(r1)
    lfs f2, 0xd8(r27)
    lfd f0, 0x18(r1)
    lfs f1, 0xd4(r27)
    fsubs f3, f0, f3
    lfs f0, lbl_8047D79C(r2)
    fmadds f1, f2, f3, f1
    stfs f1, 0xd4(r27)
    lfs f1, 0xd4(r27)
    fcmpo cr0, f1, f0
    ble @L_80185C14
    stfs f0, 0xd4(r27)
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xd8(r27)
@L_80185C14:
    lfs f1, 0xd4(r27)
    mr r3, r25
    bl fn_800EC5B8
@L_80185C20:
    mr r3, r27
    li r4, 0x8
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_80185ECC
    lfs f0, lbl_8047D830(r2)
    fcmpo cr0, f31, f0
    bge @L_80185C48
    li r3, 0x1
    b @L_80185C68
@L_80185C48:
    cror eq, gt, eq
    bne @L_80185C64
    lfs f0, lbl_8047D834(r2)
    fcmpo cr0, f31, f0
    bge @L_80185C64
    li r3, 0x2
    b @L_80185C68
@L_80185C64:
    li r3, 0x3
@L_80185C68:
    lwz r0, 0x90(r27)
    cmpw r0, r3
    beq @L_80185ECC
    stw r3, 0x90(r27)
    lwz r3, 0x30(r27)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_80185ECC
    lwz r0, 0x90(r27)
    addi r5, r1, 0x14
    addi r6, r1, 0x8
    clrlwi r4, r0, 24
    bl fn_8018F4C8
    lwz r30, 0x14(r1)
    cmpwi r30, -0x1
    beq @L_80185ECC
    cmpwi r30, 0x0
    lwz r26, 0x2c(r27)
    lbz r29, 0x8(r1)
    li r31, 0x0
    lwz r27, 0x28(r27)
    blt @L_80185ECC
    mr r28, r31
    b @L_80185D00
@L_80185CC8:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185CFC
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_80185CFC
    lwz r0, 0x2c(r3)
    cmplw r0, r26
    bne @L_80185CFC
    lwz r26, 0x4(r3)
    b @L_80185D6C
@L_80185CFC:
    addi r28, r28, 0x1
@L_80185D00:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80185CC8
    li r25, 0x0
    b @L_80185D5C
@L_80185D14:
    mr r3, r25
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185D58
    lwz r0, 0x2c(r28)
    cmplw r0, r26
    bne @L_80185D58
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r26
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r28)
    b @L_80185D6C
@L_80185D58:
    addi r25, r25, 0x1
@L_80185D5C:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80185D14
    li r26, 0x0
@L_80185D6C:
    li r27, 0x0
    b @L_80185DA0
@L_80185D74:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185D9C
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_80185D9C
    mr r26, r3
    b @L_80185DB0
@L_80185D9C:
    addi r27, r27, 0x1
@L_80185DA0:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80185D74
    li r26, 0x0
@L_80185DB0:
    cmplwi r26, 0x0
    beq @L_80185ECC
    mr r3, r26
    bl fn_8018FBD4
    mr. r28, r3
    beq @L_80185ECC
    bl fn_800EC954
    clrlwi. r0, r3, 24
    beq @L_80185DDC
    li r31, 0x1
    b @L_80185E20
@L_80185DDC:
    mr r3, r28
    bl fn_800EC960
    clrlwi. r0, r3, 24
    bne @L_80185DF4
    li r31, 0x1
    b @L_80185E20
@L_80185DF4:
    mr r3, r28
    addi r4, r1, 0xc
    addi r5, r1, 0x10
    bl fn_800EC578
    lwz r0, 0xc(r1)
    cmpw r0, r30
    bne @L_80185E1C
    lwz r0, 0x10(r1)
    cmpwi r0, -0x1
    beq @L_80185E20
@L_80185E1C:
    li r31, 0x1
@L_80185E20:
    clrlwi. r0, r31, 24
    beq @L_80185EA8
    stw r30, 0x48(r26)
    mr r3, r28
    lfs f0, lbl_8047D7A0(r2)
    mr r4, r30
    stfs f0, 0xd8(r26)
    bl fn_800ECCA8
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r28
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    mr r4, r30
    bl fn_800EC35C
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r28
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r28
    bl fn_800EC308
    cmplwi r29, 0x0
    beq @L_80185E94
    mr r3, r28
    li r4, 0x1
    bl fn_800ECB74
    b @L_80185EA0
@L_80185E94:
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
@L_80185EA0:
    mr r3, r28
    bl fn_800EC990
@L_80185EA8:
    cmplwi r29, 0x0
    beq @L_80185EC0
    mr r3, r28
    li r4, 0x1
    bl fn_800ECB74
    b @L_80185ECC
@L_80185EC0:
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
@L_80185ECC:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    lmw r25, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
}
#else
void fn_80185B90(void) { /* TODO: match -- 856 bytes at 0x80185B90 */ }
#endif

/* 0x80185F44 | 0x1B4 */
extern u32 lbl_8047D814;
extern u32 lbl_8047D7F0;
#if 1
asm void fn_80185F44(void) {
    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lfs f0, lbl_8047D814(r2)
    stw r0, 0x34(r1)
    fmuls f4, f0, f1
    stw r31, 0x2c(r1)
    fmuls f1, f0, f2
    fmuls f0, f0, f3
    li r31, 0x0
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r4
    stw r28, 0x20(r1)
    mr r28, r3
    stfs f4, 0x8(r1)
    stfs f1, 0xc(r1)
    stfs f0, 0x10(r1)
    b @L_80185FC4
@L_80185F8C:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185FC0
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80185FC0
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80185FC0
    lwz r31, 0x4(r3)
    b @L_80186030
@L_80185FC0:
    addi r31, r31, 0x1
@L_80185FC4:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80185F8C
    li r30, 0x0
    b @L_80186020
@L_80185FD8:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018601C
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_8018601C
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_80186030
@L_8018601C:
    addi r30, r30, 0x1
@L_80186020:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80185FD8
    li r31, 0x0
@L_80186030:
    li r30, 0x0
    b @L_80186064
@L_80186038:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186060
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_80186060
    mr r31, r3
    b @L_80186074
@L_80186060:
    addi r30, r30, 0x1
@L_80186064:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80186038
    li r31, 0x0
@L_80186074:
    cmplwi r31, 0x0
    beq @L_801860D8
    lfd f2, lbl_8047D7F0(r2)
    lfs f0, 0x8(r1)
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f3, f1
    lfd f2, lbl_8047D7F0(r2)
    lfs f0, 0xc(r1)
    fadd f1, f2, f0
    stfs f3, 0x8(r1)
    bl fn_800CE318
    frsp f3, f1
    lfd f2, lbl_8047D7F0(r2)
    lfs f0, 0x10(r1)
    fadd f1, f2, f0
    stfs f3, 0xc(r1)
    bl fn_800CE318
    frsp f0, f1
    mr r3, r31
    addi r4, r1, 0x8
    stfs f0, 0x10(r1)
    bl fn_8018FC08
    lfs f0, 0xc(r1)
    stfs f0, 0x40(r31)
@L_801860D8:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
}
#else
void fn_80185F44(void) { /* TODO: match -- 436 bytes at 0x80185F44 */ }
#endif

/* 0x80186254 | 0x30 */
extern const f32 lbl_8047D7EC;
extern const f32 lbl_8047D838;
extern void fn_80186284(u32 a, u32 b, f32 x, s32 c, s32 d, f32 y);
#if 0
asm void fn_80186254(void) {
#include "src/game/people/people_fn_80186254.inc"
}
#else
#pragma push
#pragma scheduling on
void fn_80186254(u32 a, u32 b) {
    fn_80186284(a, b, lbl_8047D7EC, 0, 100, lbl_8047D838);
}
#pragma pop
#endif

/* 0x80186284 | 0x39C */
extern void fn_8010F188(void);
extern u32 lbl_8047D83C;
extern u32 lbl_8047D800;
extern u32 lbl_8047D7F0;
extern u32 lbl_8047D7A8;
extern u32 lbl_8047D820;
extern u32 lbl_8047D814;
extern u32 lbl_8047D7A4;
#if 1
asm void fn_80186284(u32 a, u32 b, f32 x, s32 c, s32 d, f32 y) {
    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stmw r26, 0x48(r1)
    fmr f31, f1
    lfs f30, lbl_8047D83C(r2)
    fmr f29, f2
    mr r30, r3
    mr r31, r4
    mr r26, r5
    mr r27, r6
    li r29, 0x0
    b @L_80186308
@L_801862D0:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186304
    lwz r0, 0x28(r3)
    cmplw r0, r26
    bne @L_80186304
    lwz r0, 0x2c(r3)
    cmplw r0, r27
    bne @L_80186304
    lwz r29, 0x4(r3)
    b @L_80186374
@L_80186304:
    addi r29, r29, 0x1
@L_80186308:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801862D0
    li r28, 0x0
    b @L_80186364
@L_8018631C:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186360
    lwz r0, 0x2c(r29)
    cmplw r0, r27
    bne @L_80186360
    lis r3, lbl_80273FD8@ha
    mr r4, r26
    addi r3, r3, lbl_80273FD8@l
    mr r5, r27
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r29, 0x4(r29)
    b @L_80186374
@L_80186360:
    addi r28, r28, 0x1
@L_80186364:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018631C
    li r29, 0x0
@L_80186374:
    li r28, 0x0
    b @L_801863A4
@L_8018637C:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801863A0
    lwz r0, 0x4(r3)
    cmplw r0, r29
    bne @L_801863A0
    b @L_801863B4
@L_801863A0:
    addi r28, r28, 0x1
@L_801863A4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018637C
    li r3, 0x0
@L_801863B4:
    cmplwi r3, 0x0
    bne @L_801863C4
    li r3, 0x0
    b @L_801865F4
@L_801863C4:
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x2c
    bl fn_800E01D0
    li r28, 0x0
    b @L_80186414
@L_801863DC:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186410
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_80186410
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_80186410
    lwz r28, 0x4(r3)
    b @L_80186480
@L_80186410:
    addi r28, r28, 0x1
@L_80186414:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_801863DC
    li r28, 0x0
    b @L_80186470
@L_80186428:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018646C
    lwz r0, 0x2c(r29)
    cmplw r0, r31
    bne @L_8018646C
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r29)
    b @L_80186480
@L_8018646C:
    addi r28, r28, 0x1
@L_80186470:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80186428
    li r28, 0x0
@L_80186480:
    li r29, 0x0
    b @L_801864B4
@L_80186488:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801864B0
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_801864B0
    mr r28, r3
    b @L_801864C4
@L_801864B0:
    addi r29, r29, 0x1
@L_801864B4:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186488
    li r28, 0x0
@L_801864C4:
    cmplwi r28, 0x0
    bne @L_801864D4
    li r3, 0x0
    b @L_801865F4
@L_801864D4:
    mr r3, r28
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x38
    bl fn_800E01D0
    addi r3, r1, 0x14
    addi r4, r1, 0x38
    addi r5, r1, 0x2c
    bl fn_800E0168
    lfs f0, lbl_8047D800(r2)
    addi r3, r1, 0x14
    fmuls f31, f0, f31
    bl fn_800E008C
    fcmpo cr0, f1, f31
    ble @L_80186518
    li r3, 0x0
    b @L_801865F4
@L_80186518:
    mr r3, r28
    addi r4, r1, 0x20
    bl fn_8018FC2C
    lfs f31, 0x24(r1)
    addi r3, r1, 0x8
    addi r4, r1, 0x2c
    addi r5, r1, 0x38
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f2, 0x10(r1)
    bl fn_800CE2D8
    frsp f0, f1
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f0, f31
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f1, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f1, f0
    ble @L_80186578
    lfd f0, lbl_8047D7F0(r2)
    fsub f1, f1, f0
    frsp f1, f1
    b @L_80186590
@L_80186578:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f1, f0
    bge @L_80186590
    lfd f0, lbl_8047D7F0(r2)
    fadd f1, f0, f1
    frsp f1, f1
@L_80186590:
    lfs f0, lbl_8047D814(r2)
    fabs f2, f1
    lfs f1, lbl_8047D7A4(r2)
    fmuls f0, f0, f29
    fmuls f29, f1, f0
    fcmpo cr0, f2, f29
    ble @L_801865B4
    li r3, 0x0
    b @L_801865F4
@L_801865B4:
    lwz r3, 0x30(r28)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_801865CC
    bl fn_8018F5E4
    fmr f30, f1
@L_801865CC:
    fmr f1, f30
    addi r3, r1, 0x38
    addi r4, r1, 0x2c
    li r5, 0x0
    bl fn_8010F188
    cmpwi r3, 0x0
    beq @L_801865F0
    li r3, 0x0
    b @L_801865F4
@L_801865F0:
    li r3, 0x1
@L_801865F4:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    lmw r26, 0x48(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
}
#else
void fn_80186284(void) { /* TODO: match -- 924 bytes at 0x80186284 */ }
#endif

/* 0x80186620 | 0x53C */
extern void fn_801779EC(void);
extern void fn_800D258C(void);
extern void fn_800D2248(void);
extern void fn_800DA028(void);
extern void fn_800D7820(void);
extern void fn_800D88DC(void);
extern void fn_800D888C(void);
extern void fn_800DA4C4(void);
extern void fn_800D9ED8(void);
extern void fn_800D6A00(void);
extern void fn_800D67BC(void);
extern void fn_800D6680(void);
extern void fn_800D5CB8(void);
extern void fn_800D6728(void);
extern void fn_800E0060(void);
extern void fn_800E0000(void);
extern void fn_8010F6A0(void);
extern void fn_8012B184(void);
extern u32 lbl_8047D840;
extern u8 lbl_80314638[];
extern u32 lbl_8047D844;
extern u32 lbl_8047D83C;
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D79C;
extern u32 lbl_8047D848;
extern void fn_80186B5C(void);
extern void fn_801870E8(void);
#if 1
asm void fn_80186620(void) {
    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    stfd f31, 0x100(r1)
    psq_st f31, 0x108(r1), 0, 0
    stfd f30, 0xf0(r1)
    psq_st f30, 0xf8(r1), 0, 0
    stfd f29, 0xe0(r1)
    psq_st f29, 0xe8(r1), 0, 0
    stfd f28, 0xd0(r1)
    psq_st f28, 0xd8(r1), 0, 0
    stmw r27, 0xbc(r1)
    fmr f31, f1
    mr r27, r3
    fmr f30, f2
    mr r28, r4
    fmr f29, f3
    mr r31, r5
    fmr f28, f4
    li r30, 0x0
    b @L_801866AC
@L_80186674:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801866A8
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_801866A8
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_801866A8
    lwz r30, 0x4(r3)
    b @L_80186718
@L_801866A8:
    addi r30, r30, 0x1
@L_801866AC:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80186674
    li r29, 0x0
    b @L_80186708
@L_801866C0:
    mr r3, r29
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186704
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80186704
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r30, 0x4(r30)
    b @L_80186718
@L_80186704:
    addi r29, r29, 0x1
@L_80186708:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801866C0
    li r30, 0x0
@L_80186718:
    li r29, 0x0
    b @L_8018674C
@L_80186720:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186748
    lwz r0, 0x4(r3)
    cmplw r0, r30
    bne @L_80186748
    mr r30, r3
    b @L_8018675C
@L_80186748:
    addi r29, r29, 0x1
@L_8018674C:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186720
    li r30, 0x0
@L_8018675C:
    cmplwi r30, 0x0
    bne @L_8018676C
    li r3, 0x0
    b @L_80186B28
@L_8018676C:
    mr r3, r30
    lis r4, 0x4000
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_801867D4
    addi r3, r1, 0x68
    addi r4, r30, 0x74
    bl fn_800E01D0
    lfs f1, lbl_8047D840(r2)
    addi r3, r1, 0x5c
    addi r4, r1, 0x68
    bl fn_800E013C
    addi r3, r1, 0x68
    li r4, 0x0
    li r5, 0x64
    bl fn_80186B5C
    mr r3, r30
    bl fn_8018FC00
    mr r4, r3
    addi r3, r1, 0x80
    bl fn_800E01D0
    addi r3, r1, 0x74
    addi r4, r1, 0x80
    addi r5, r1, 0x68
    bl fn_800E019C
    b @L_8018680C
@L_801867D4:
    mr r3, r30
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x74
    bl fn_800E01D0
    mr r3, r30
    bl fn_8018FC00
    mr r4, r3
    addi r3, r1, 0x80
    bl fn_800E01D0
    addi r3, r1, 0x68
    addi r4, r1, 0x74
    addi r5, r1, 0x80
    bl fn_800E0168
@L_8018680C:
    fmr f1, f31
    lfs f2, 0x78(r1)
    fmr f3, f30
    addi r3, r1, 0x98
    bl fn_800E01F4
    fmr f1, f29
    lfs f2, 0x78(r1)
    fmr f3, f28
    addi r3, r1, 0x8c
    bl fn_800E01F4
    li r3, 0x0
    li r4, 0x2
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80186970
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186970
    bl fn_801779EC
    bl fn_800D258C
    bl fn_800D2248
    li r3, 0x0
    bl fn_800DA028
    lis r3, lbl_80314638@ha
    addi r3, r3, lbl_80314638@l
    bl fn_800D7820
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x4
    bl fn_800D6A00
    li r3, 0x4
    bl fn_800D67BC
    lfs f2, 0x78(r1)
    fmr f1, f31
    lfs f0, lbl_8047D844(r2)
    fmr f3, f30
    fsubs f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f31
    lfs f0, 0x78(r1)
    fmr f3, f30
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, 0x78(r1)
    fmr f1, f29
    lfs f0, lbl_8047D844(r2)
    fmr f3, f28
    fsubs f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f29
    lfs f0, 0x78(r1)
    fmr f3, f28
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    bl fn_800D6728
@L_80186970:
    lwz r3, 0x30(r30)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_8018698C
    bl fn_8018F5E4
    fmr f31, f1
    b @L_80186990
@L_8018698C:
    lfs f31, lbl_8047D83C(r2)
@L_80186990:
    fmr f30, f31
    addi r3, r1, 0x68
    bl fn_800E008C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    ble @L_801869C0
    fdivs f29, f31, f1
    lfs f0, lbl_8047D79C(r2)
    fcmpo cr0, f29, f0
    ble @L_801869C4
    fmr f29, f0
    b @L_801869C4
@L_801869C0:
    lfs f29, lbl_8047D79C(r2)
@L_801869C4:
    addi r3, r1, 0x14
    addi r4, r1, 0x8c
    addi r5, r1, 0x98
    bl fn_800E0168
    addi r3, r1, 0x14
    bl fn_800E008C
    lfs f0, lbl_8047D848(r2)
    fcmpo cr0, f1, f0
    bge @L_801869F0
    li r3, 0x0
    b @L_80186B28
@L_801869F0:
    addi r3, r1, 0x14
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x8
    addi r4, r1, 0x80
    addi r5, r1, 0x98
    bl fn_800E0168
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    bl fn_800E0000
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    bl fn_800E013C
    addi r3, r1, 0x2c
    addi r4, r1, 0x8
    addi r5, r1, 0x98
    bl fn_800E019C
    addi r3, r1, 0x20
    addi r4, r1, 0x80
    addi r5, r1, 0x2c
    bl fn_800E0168
    lfs f28, lbl_8047D7A0(r2)
    lfs f31, lbl_8047D79C(r2)
    b @L_80186B1C
@L_80186A50:
    fadds f1, f28, f29
    fcmpo cr0, f1, f31
    ble @L_80186A60
    fmr f1, f31
@L_80186A60:
    addi r3, r1, 0xa4
    addi r4, r1, 0x68
    bl fn_800E013C
    addi r3, r1, 0xa4
    addi r4, r1, 0x80
    mr r5, r3
    bl fn_800E019C
    fmr f1, f30
    addi r3, r1, 0x2c
    addi r4, r1, 0xa4
    addi r5, r1, 0x98
    addi r6, r1, 0x8c
    addi r7, r1, 0x20
    bl fn_801870E8
    clrlwi. r0, r3, 24
    beq @L_80186B18
    clrlwi. r0, r31, 24
    beq @L_80186B10
    lfs f0, lbl_8047D848(r2)
    addi r3, r1, 0x38
    addi r4, r1, 0x2c
    addi r5, r1, 0xa4
    fadds f1, f0, f30
    bl fn_8010F6A0
    lfs f1, lbl_8047D83C(r2)
    addi r3, r1, 0x80
    addi r4, r1, 0x38
    addi r5, r1, 0x50
    bl fn_8010F320
    cmpwi r3, 0x0
    beq @L_80186AFC
    addi r3, r1, 0x44
    addi r4, r1, 0x50
    addi r5, r1, 0x38
    bl fn_800E0168
    addi r3, r1, 0x38
    addi r5, r1, 0x44
    mr r4, r3
    bl fn_800E019C
@L_80186AFC:
    mr r3, r30
    addi r4, r1, 0x38
    bl fn_8018FC74
    li r3, 0x1
    bl fn_8012B184
@L_80186B10:
    li r3, 0x1
    b @L_80186B28
@L_80186B18:
    fadds f28, f28, f29
@L_80186B1C:
    fcmpo cr0, f28, f31
    blt @L_80186A50
    li r3, 0x0
@L_80186B28:
    psq_l f31, 0x108(r1), 0, 0
    lfd f31, 0x100(r1)
    psq_l f30, 0xf8(r1), 0, 0
    lfd f30, 0xf0(r1)
    psq_l f29, 0xe8(r1), 0, 0
    lfd f29, 0xe0(r1)
    psq_l f28, 0xd8(r1), 0, 0
    lfd f28, 0xd0(r1)
    lmw r27, 0xbc(r1)
    lwz r0, 0x114(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr
}
#else
void fn_80186620(void) { /* TODO: match -- 1340 bytes at 0x80186620 */ }
#endif

/* 0x80186B5C | 0x58C */
extern void fn_800F7A7C(void);
extern void fn_800F7A08(void);
extern void fn_80176684(void);
extern u8 lbl_80273FB4[];
extern u32 lbl_8047D7C8;
extern u32 lbl_8047D84C;
extern u32 lbl_8047D7A0;
extern u32 lbl_8047D850;
extern u32 lbl_8047D858;
extern u32 lbl_8047D860;
extern u8 lbl_80478AC0[];
extern u32 lbl_8047D7C4;
extern u32 lbl_8047D868;
extern u32 lbl_8047D86C;
extern u32 lbl_8047D7E8;
extern u32 lbl_8047D870;
extern u32 lbl_8047D818;
extern u32 lbl_8047D830;
extern u32 lbl_8047D7A4;
extern u32 lbl_8047D79C;
extern u32 lbl_8047D874;
extern u32 lbl_8047D7D0;
#if 1
asm void fn_80186B5C(void) {
    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stmw r27, 0x5c(r1)
    lis r6, lbl_80273FB4@ha
    mr r31, r4
    lwzu r4, lbl_80273FB4@l(r6)
    mr r27, r3
    mr r30, r5
    li r28, 0x0
    lwz r3, 0x4(r6)
    lwz r0, 0x8(r6)
    stw r4, 0x34(r1)
    stw r3, 0x38(r1)
    stw r0, 0x3c(r1)
    b @L_80186BE4
@L_80186BAC:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186BE0
    lwz r0, 0x28(r3)
    cmplw r0, r31
    bne @L_80186BE0
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_80186BE0
    lwz r28, 0x4(r3)
    b @L_80186C50
@L_80186BE0:
    addi r28, r28, 0x1
@L_80186BE4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80186BAC
    li r29, 0x0
    b @L_80186C40
@L_80186BF8:
    mr r3, r29
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186C3C
    lwz r0, 0x2c(r28)
    cmplw r0, r30
    bne @L_80186C3C
    lis r3, lbl_80273FD8@ha
    mr r4, r31
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_80186C50
@L_80186C3C:
    addi r29, r29, 0x1
@L_80186C40:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186BF8
    li r28, 0x0
@L_80186C50:
    li r29, 0x0
    b @L_80186C84
@L_80186C58:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186C80
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80186C80
    mr r31, r3
    b @L_80186C94
@L_80186C80:
    addi r29, r29, 0x1
@L_80186C84:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186C58
    li r31, 0x0
@L_80186C94:
    cmplwi r31, 0x0
    bne @L_80186CB8
    lwz r3, 0x34(r1)
    lwz r0, 0x38(r1)
    stw r3, 0x0(r27)
    stw r0, 0x4(r27)
    lwz r0, 0x3c(r1)
    stw r0, 0x8(r27)
    b @L_801870C4
@L_80186CB8:
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7A7C
    mr r0, r3
    li r3, 0x1
    mr r30, r0
    li r4, 0x1
    bl fn_800F7A08
    mr r0, r3
    li r3, 0x1
    mr r29, r0
    li r4, 0x0
    bl fn_800F7A7C
    mr r0, r3
    li r3, 0x1
    mr r28, r0
    li r4, 0x0
    bl fn_800F7A08
    extsb. r0, r30
    bne @L_80186D68
    extsb. r0, r29
    bne @L_80186D68
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm. r0, r3, 0, 28, 28
    beq @L_80186D24
    li r29, -0x38
@L_80186D24:
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm. r0, r3, 0, 29, 29
    beq @L_80186D38
    li r29, 0x38
@L_80186D38:
    li r3, 0x1
    bl fn_800F7BC4
    clrlwi. r0, r3, 31
    beq @L_80186D4C
    li r30, -0x38
@L_80186D4C:
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm. r0, r3, 0, 30, 30
    beq @L_80186D60
    li r30, 0x38
@L_80186D60:
    mr r28, r30
    mr r3, r29
@L_80186D68:
    extsb. r0, r30
    bne @L_80186D78
    extsb. r0, r29
    beq @L_801870AC
@L_80186D78:
    extsb r0, r30
    cmpwi r0, 0x38
    ble @L_80186D8C
    li r30, 0x38
    b @L_80186D98
@L_80186D8C:
    cmpwi r0, -0x38
    bge @L_80186D98
    li r30, -0x38
@L_80186D98:
    extsb r0, r29
    cmpwi r0, 0x38
    ble @L_80186DAC
    li r29, 0x38
    b @L_80186DB8
@L_80186DAC:
    cmpwi r0, -0x38
    bge @L_80186DB8
    li r29, -0x38
@L_80186DB8:
    extsb. r0, r30
    extsb r0, r30
    neg r4, r0
    ble @L_80186DCC
    mr r4, r0
@L_80186DCC:
    xoris r0, r4, 0x8000
    lis r5, 0x4330
    stw r0, 0x44(r1)
    extsb r0, r29
    lfd f2, lbl_8047D7C8(r2)
    extsb. r4, r29
    stw r5, 0x40(r1)
    neg r4, r0
    lfs f0, lbl_8047D84C(r2)
    lfd f1, 0x40(r1)
    fsubs f1, f1, f2
    fdivs f5, f1, f0
    ble @L_80186E04
    mr r4, r0
@L_80186E04:
    xoris r4, r4, 0x8000
    lis r0, 0x4330
    stw r4, 0x4c(r1)
    lfd f3, lbl_8047D7C8(r2)
    stw r0, 0x48(r1)
    lfs f1, lbl_8047D84C(r2)
    lfd f2, 0x48(r1)
    lfs f0, lbl_8047D7A0(r2)
    fsubs f2, f2, f3
    fdivs f6, f2, f1
    fmuls f1, f6, f6
    fmadds f4, f5, f5, f1
    fcmpo cr0, f4, f0
    ble @L_80186E84
    frsqrte f1, f4
    lfd f3, lbl_8047D850(r2)
    lfd f2, lbl_8047D858(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_80186F08
@L_80186E84:
    lfd f0, lbl_8047D860(r2)
    fcmpo cr0, f4, f0
    bge @L_80186E9C
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
    b @L_80186F08
@L_80186E9C:
    stfs f4, 0x8(r1)
    lis r0, 0x7f80
    lwz r5, 0x8(r1)
    rlwinm r4, r5, 0, 1, 8
    cmpw r4, r0
    beq @L_80186EC4
    bge @L_80186EF4
    cmpwi r4, 0x0
    beq @L_80186EDC
    b @L_80186EF4
@L_80186EC4:
    clrlwi. r0, r5, 9
    beq @L_80186ED4
    li r0, 0x1
    b @L_80186EF8
@L_80186ED4:
    li r0, 0x2
    b @L_80186EF8
@L_80186EDC:
    clrlwi. r0, r5, 9
    beq @L_80186EEC
    li r0, 0x5
    b @L_80186EF8
@L_80186EEC:
    li r0, 0x3
    b @L_80186EF8
@L_80186EF4:
    li r0, 0x4
@L_80186EF8:
    cmpwi r0, 0x1
    bne @L_80186F08
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
@L_80186F08:
    lfs f0, lbl_8047D7C4(r2)
    fmr f31, f4
    fcmpo cr0, f4, f0
    ble @L_80186F1C
    fmr f31, f0
@L_80186F1C:
    extsb r0, r28
    cmpwi r0, -0x2
    ble @L_80186F44
    cmpwi r0, 0x2
    bge @L_80186F44
    extsb r0, r3
    cmpwi r0, -0x2
    ble @L_80186F44
    cmpwi r0, 0x2
    blt @L_80186FD0
@L_80186F44:
    lfs f0, lbl_8047D868(r2)
    fcmpo cr0, f6, f0
    bge @L_80186F58
    lfs f2, lbl_8047D86C(r2)
    b @L_80186F84
@L_80186F58:
    fdivs f1, f5, f6
    lfs f0, lbl_8047D7E8(r2)
    fcmpo cr0, f1, f0
    ble @L_80186F6C
    fmr f1, f0
@L_80186F6C:
    lfs f0, lbl_8047D870(r2)
    fdivs f1, f1, f0
    bl sin
    frsp f1, f1
    lfs f0, lbl_8047D86C(r2)
    fmuls f2, f0, f1
@L_80186F84:
    extsb. r0, r29
    blt @L_80186F94
    fmr f30, f2
    b @L_80186F9C
@L_80186F94:
    lfs f0, lbl_8047D818(r2)
    fsubs f30, f0, f2
@L_80186F9C:
    extsb. r0, r30
    bge @L_80186FC4
    extsb. r0, r29
    blt @L_80186FBC
    lfs f1, lbl_8047D818(r2)
    fsubs f0, f1, f2
    fadds f30, f1, f0
    b @L_80186FC4
@L_80186FBC:
    lfs f0, lbl_8047D818(r2)
    fadds f30, f0, f2
@L_80186FC4:
    bl fn_80176684
    fadds f1, f30, f1
    b @L_80186FD4
@L_80186FD0:
    lfs f1, 0x40(r31)
@L_80186FD4:
    lfs f2, lbl_8047D830(r2)
    fcmpo cr0, f31, f2
    cror eq, lt, eq
    bne @L_80186FFC
    fdivs f3, f31, f2
    lfs f2, 0x34(r31)
    lfs f0, lbl_8047D7A4(r2)
    fmuls f0, f2, f0
    fmuls f30, f3, f0
    b @L_80187050
@L_80186FFC:
    fcmpo cr0, f31, f2
    ble @L_80187038
    lfs f0, lbl_8047D79C(r2)
    fcmpo cr0, f31, f0
    cror eq, lt, eq
    bne @L_80187038
    fsubs f2, f31, f2
    lfs f0, lbl_8047D874(r2)
    lfs f4, 0x34(r31)
    lfs f3, lbl_8047D7A4(r2)
    fdivs f2, f2, f0
    fmuls f3, f4, f3
    fsubs f0, f4, f3
    fmadds f30, f2, f0, f3
    b @L_80187050
@L_80187038:
    lfs f2, lbl_8047D79C(r2)
    lfs f3, 0x34(r31)
    lfs f0, 0x38(r31)
    fsubs f2, f31, f2
    fsubs f0, f0, f3
    fmadds f30, f2, f0, f3
@L_80187050:
    lis r3, lbl_8031554C@ha
    addi r4, r3, lbl_8031554C@l
    addi r3, r1, 0xc
    bl fn_800E0718
    lfs f1, lbl_8047D7A0(r2)
    fmr f3, f30
    addi r3, r1, 0x28
    fmr f2, f1
    bl fn_800E01F4
    addi r3, r1, 0x1c
    addi r4, r1, 0xc
    addi r5, r1, 0x28
    bl fn_800DFEEC
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f1, lbl_8047D7D0(r2)
    addi r3, r1, 0x34
    stw r0, 0x48(r1)
    addi r4, r1, 0x1c
    lfd f0, 0x48(r1)
    fsubs f1, f0, f1
    bl fn_800E013C
@L_801870AC:
    lwz r3, 0x34(r1)
    lwz r0, 0x38(r1)
    stw r3, 0x0(r27)
    stw r0, 0x4(r27)
    lwz r0, 0x3c(r1)
    stw r0, 0x8(r27)
@L_801870C4:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    lmw r27, 0x5c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
}
#else
void fn_80186B5C(void) { /* TODO: match -- 1420 bytes at 0x80186B5C */ }
#endif

/* 0x801870E8 | 0x3D4 */
extern void fn_8010C77C(void);
extern void fn_8010FA54(void);
extern void fn_800A3BD8(void);
extern void fn_8010F71C(void);
extern u32 lbl_8047D844;
extern u32 lbl_8047D7A0;
#if 1
asm void fn_801870E8(void) {
    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stmw r27, 0x6c(r1)
    mr r28, r5
    mr r27, r4
    lfs f4, 0x0(r5)
    mr r31, r3
    mr r30, r7
    lfs f0, lbl_8047D844(r2)
    stfs f4, 0x38(r1)
    mr r29, r6
    fmr f31, f1
    mr r3, r30
    lfs f1, 0x4(r31)
    mr r5, r27
    addi r4, r1, 0x38
    fsubs f3, f1, f0
    fadds f0, f0, f1
    stfs f3, 0x3c(r1)
    lfs f2, 0x8(r28)
    stfs f2, 0x40(r1)
    lfs f1, 0x0(r6)
    stfs f1, 0x44(r1)
    stfs f3, 0x48(r1)
    lfs f1, 0x8(r6)
    stfs f1, 0x4c(r1)
    stfs f4, 0x50(r1)
    stfs f0, 0x54(r1)
    stfs f2, 0x58(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187180
    li r4, 0x0
    b @L_801871F4
@L_80187180:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x2c
    addi r5, r1, 0x38
    bl GScolsy2UtilGetCpPlanePoint
    mr r4, r27
    addi r3, r1, 0x2c
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801871B8
    li r4, 0x0
    b @L_801871F4
@L_801871B8:
    mr r5, r30
    addi r3, r1, 0x2c
    addi r4, r1, 0x38
    bl GScolsy2UtilChkInTri
    cmpwi r3, 0x0
    bne @L_801871D8
    li r4, 0x0
    b @L_801871F4
@L_801871D8:
    lwz r3, 0x2c(r1)
    li r4, 0x1
    lwz r0, 0x30(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x34(r1)
    stw r0, 0x8(r31)
@L_801871F4:
    cmpwi r4, 0x0
    beq @L_80187204
    li r3, 0x1
    b @L_801874A0
@L_80187204:
    lfs f3, 0x0(r29)
    mr r3, r30
    lfs f0, lbl_8047D844(r2)
    mr r5, r27
    stfs f3, 0x38(r1)
    addi r4, r1, 0x38
    lfs f2, 0x4(r31)
    fsubs f1, f2, f0
    fadds f0, f0, f2
    stfs f1, 0x3c(r1)
    lfs f1, 0x8(r29)
    stfs f1, 0x40(r1)
    stfs f3, 0x44(r1)
    stfs f0, 0x48(r1)
    stfs f1, 0x4c(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187258
    li r4, 0x0
    b @L_801872CC
@L_80187258:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x20
    addi r5, r1, 0x38
    bl GScolsy2UtilGetCpPlanePoint
    mr r4, r27
    addi r3, r1, 0x20
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80187290
    li r4, 0x0
    b @L_801872CC
@L_80187290:
    mr r5, r30
    addi r3, r1, 0x20
    addi r4, r1, 0x38
    bl GScolsy2UtilChkInTri
    cmpwi r3, 0x0
    bne @L_801872B0
    li r4, 0x0
    b @L_801872CC
@L_801872B0:
    lwz r3, 0x20(r1)
    li r4, 0x1
    lwz r0, 0x24(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x28(r1)
    stw r0, 0x8(r31)
@L_801872CC:
    cmpwi r4, 0x0
    beq @L_801872DC
    li r3, 0x1
    b @L_801874A0
@L_801872DC:
    lfs f1, 0x0(r29)
    mr r3, r30
    lfs f0, lbl_8047D844(r2)
    mr r5, r27
    stfs f1, 0x38(r1)
    addi r4, r1, 0x38
    lfs f1, 0x4(r31)
    fsubs f3, f1, f0
    fadds f0, f0, f1
    stfs f3, 0x3c(r1)
    lfs f1, 0x8(r29)
    stfs f1, 0x40(r1)
    lfs f2, 0x0(r28)
    stfs f2, 0x44(r1)
    stfs f3, 0x48(r1)
    lfs f1, 0x8(r28)
    stfs f1, 0x4c(r1)
    stfs f2, 0x50(r1)
    stfs f0, 0x54(r1)
    stfs f1, 0x58(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187344
    li r4, 0x0
    b @L_801873B8
@L_80187344:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x14
    addi r5, r1, 0x38
    bl GScolsy2UtilGetCpPlanePoint
    mr r4, r27
    addi r3, r1, 0x14
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8018737C
    li r4, 0x0
    b @L_801873B8
@L_8018737C:
    mr r5, r30
    addi r3, r1, 0x14
    addi r4, r1, 0x38
    bl GScolsy2UtilChkInTri
    cmpwi r3, 0x0
    bne @L_8018739C
    li r4, 0x0
    b @L_801873B8
@L_8018739C:
    lwz r3, 0x14(r1)
    li r4, 0x1
    lwz r0, 0x18(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x1c(r1)
    stw r0, 0x8(r31)
@L_801873B8:
    cmpwi r4, 0x0
    beq @L_801873C8
    li r3, 0x1
    b @L_801874A0
@L_801873C8:
    lfs f0, 0x0(r28)
    mr r3, r30
    lfs f1, lbl_8047D844(r2)
    mr r5, r27
    stfs f0, 0x44(r1)
    addi r4, r1, 0x38
    lfs f0, 0x4(r31)
    fadds f1, f1, f0
    stfs f1, 0x48(r1)
    lfs f0, 0x8(r28)
    stfs f0, 0x4c(r1)
    lfs f0, 0x0(r29)
    stfs f0, 0x50(r1)
    stfs f1, 0x54(r1)
    lfs f0, 0x8(r29)
    stfs f0, 0x58(r1)
    bl fn_8010C77C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    bge @L_80187420
    li r4, 0x0
    b @L_80187494
@L_80187420:
    mr r4, r30
    mr r6, r27
    addi r3, r1, 0x8
    addi r5, r1, 0x38
    bl GScolsy2UtilGetCpPlanePoint
    mr r4, r27
    addi r3, r1, 0x8
    bl fn_800A3BD8
    fmuls f0, f31, f31
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80187458
    li r4, 0x0
    b @L_80187494
@L_80187458:
    mr r5, r30
    addi r3, r1, 0x8
    addi r4, r1, 0x38
    bl GScolsy2UtilChkInTri
    cmpwi r3, 0x0
    bne @L_80187478
    li r4, 0x0
    b @L_80187494
@L_80187478:
    lwz r3, 0x8(r1)
    li r4, 0x1
    lwz r0, 0xc(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x10(r1)
    stw r0, 0x8(r31)
@L_80187494:
    neg r0, r4
    or r0, r0, r4
    srwi r3, r0, 31
@L_801874A0:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    lmw r27, 0x6c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
}
#else
void fn_801870E8(void) { /* TODO: match -- 980 bytes at 0x801870E8 */ }
#endif

/* 0x801874BC | 0x450 */
extern u32 lbl_8047D844;
#if 1
asm void fn_801874BC(void) {
    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    fmr f28, f1
    mr r28, r3
    fmr f29, f2
    mr r29, r4
    fmr f30, f3
    li r31, 0x0
    fmr f31, f4
    b @L_80187550
@L_80187518:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018754C
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_8018754C
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_8018754C
    lwz r31, 0x4(r3)
    b @L_801875BC
@L_8018754C:
    addi r31, r31, 0x1
@L_80187550:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80187518
    li r30, 0x0
    b @L_801875AC
@L_80187564:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801875A8
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_801875A8
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_801875BC
@L_801875A8:
    addi r30, r30, 0x1
@L_801875AC:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80187564
    li r31, 0x0
@L_801875BC:
    li r30, 0x0
    b @L_801875EC
@L_801875C4:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801875E8
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_801875E8
    b @L_801875FC
@L_801875E8:
    addi r30, r30, 0x1
@L_801875EC:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801875C4
    li r3, 0x0
@L_801875FC:
    cmplwi r3, 0x0
    bne @L_8018760C
    li r3, 0x0
    b @L_801878CC
@L_8018760C:
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x8
    bl fn_800E01D0
    li r3, 0x0
    li r4, 0x2
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80187850
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187850
    bl fn_801779EC
    bl fn_800D258C
    bl fn_800D2248
    li r3, 0x0
    bl fn_800DA028
    lis r3, lbl_80314638@ha
    addi r3, r3, lbl_80314638@l
    bl fn_800D7820
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x4
    bl fn_800D6A00
    li r3, 0xa
    bl fn_800D67BC
    fmr f1, f28
    lfs f2, 0xc(r1)
    fmr f3, f29
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f28
    lfs f0, 0xc(r1)
    fmr f3, f29
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f28
    lfs f2, 0xc(r1)
    fmr f3, f31
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f28
    lfs f0, 0xc(r1)
    fmr f3, f31
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f30
    lfs f2, 0xc(r1)
    fmr f3, f31
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f30
    lfs f0, 0xc(r1)
    fmr f3, f31
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f30
    lfs f2, 0xc(r1)
    fmr f3, f29
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f30
    lfs f0, 0xc(r1)
    fmr f3, f29
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    fmr f1, f28
    lfs f2, 0xc(r1)
    fmr f3, f29
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f28
    lfs f0, 0xc(r1)
    fmr f3, f29
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    bl fn_800D6728
@L_80187850:
    fcmpo cr0, f28, f30
    bge @L_80187864
    fmr f1, f28
    fmr f2, f30
    b @L_8018786C
@L_80187864:
    fmr f1, f30
    fmr f2, f28
@L_8018786C:
    fcmpo cr0, f29, f31
    bge @L_80187880
    fmr f3, f29
    fmr f4, f31
    b @L_80187888
@L_80187880:
    fmr f3, f31
    fmr f4, f29
@L_80187888:
    lfs f0, 0x8(r1)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801878C8
    fcmpo cr0, f0, f2
    cror eq, lt, eq
    bne @L_801878C8
    lfs f0, 0x10(r1)
    fcmpo cr0, f3, f0
    cror eq, lt, eq
    bne @L_801878C8
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    bne @L_801878C8
    li r3, 0x1
    b @L_801878CC
@L_801878C8:
    li r3, 0x0
@L_801878CC:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r0, 0x74(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
}
#else
void fn_801874BC(void) { /* TODO: match -- 1104 bytes at 0x801874BC */ }
#endif

/* 0x80187A60 | 0x2E8 */
extern u32 lbl_8047D7C0;
extern u32 lbl_8047D7C8;
#if 1
asm void fn_80187A60(void) {
    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stmw r26, 0x48(r1)
    fmr f30, f1
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    li r27, 0x0
    b @L_80187AD4
@L_80187A9C:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187AD0
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80187AD0
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80187AD0
    lwz r27, 0x4(r3)
    b @L_80187B40
@L_80187AD0:
    addi r27, r27, 0x1
@L_80187AD4:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80187A9C
    li r26, 0x0
    b @L_80187B30
@L_80187AE8:
    mr r3, r26
    bl fn_8018FD88
    mr r27, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187B2C
    lwz r0, 0x2c(r27)
    cmplw r0, r29
    bne @L_80187B2C
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r27, 0x4(r27)
    b @L_80187B40
@L_80187B2C:
    addi r26, r26, 0x1
@L_80187B30:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187AE8
    li r27, 0x0
@L_80187B40:
    li r26, 0x0
    b @L_80187B74
@L_80187B48:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187B70
    lwz r0, 0x4(r3)
    cmplw r0, r27
    bne @L_80187B70
    mr r27, r3
    b @L_80187B84
@L_80187B70:
    addi r26, r26, 0x1
@L_80187B74:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187B48
    li r27, 0x0
@L_80187B84:
    cmplwi r27, 0x0
    beq @L_80187D24
    mr r3, r30
    mr r4, r31
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80187D24
    bl fn_800E3D00
    mr r4, r3
    addi r3, r1, 0x14
    bl fn_800E01D0
    mr r3, r27
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    bl fn_800E0168
    lfs f1, 0x20(r1)
    lfs f2, 0x28(r1)
    bl fn_800CE2D8
    frsp f31, f1
    li r26, 0x0
    b @L_80187C18
@L_80187BE0:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187C14
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80187C14
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80187C14
    lwz r26, 0x4(r3)
    b @L_80187C84
@L_80187C14:
    addi r26, r26, 0x1
@L_80187C18:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187BE0
    li r26, 0x0
    b @L_80187C74
@L_80187C2C:
    mr r3, r26
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187C70
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_80187C70
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r30)
    b @L_80187C84
@L_80187C70:
    addi r26, r26, 0x1
@L_80187C74:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80187C2C
    li r26, 0x0
@L_80187C84:
    li r27, 0x0
    b @L_80187CB8
@L_80187C8C:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80187CB4
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_80187CB4
    mr r26, r3
    b @L_80187CC8
@L_80187CB4:
    addi r27, r27, 0x1
@L_80187CB8:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80187C8C
    li r26, 0x0
@L_80187CC8:
    cmplwi r26, 0x0
    beq @L_80187D24
    mr r3, r26
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f2, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x38(r1)
    fdivs f0, f0, f2
    lfd f1, lbl_8047D7C8(r2)
    stb r0, 0x22(r26)
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x3c(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fmadds f0, f2, f0, f31
    stfs f0, 0x40(r26)
    stfs f30, 0x44(r26)
@L_80187D24:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    lmw r26, 0x48(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
}
#else
void fn_80187A60(void) { /* TODO: match -- 744 bytes at 0x80187A60 */ }
#endif

/* 0x80188984 | 0x170 */
#if 1
asm void fn_80188984(void) {
#include "src/game/people/people_fn_80188984.inc"
}
#else
void fn_80188984(void) { /* TODO: match -- 368 bytes at 0x80188984 */ }
#endif

/* SDA data labels referenced by asm incs (symbolmap port), typed by load width */
extern f32 lbl_8047D8A0;
extern u32 lbl_8047B1F0[2];
extern f32 lbl_8047D8B0;

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8018F30C(void) {
    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    bl fn_8018FDB4
    cmpwi r3, 0x0
    beq @L_8018F440
    lfs f30, lbl_8047D7A0(r2)
    li r30, 0x0
    lfs f31, lbl_8047D8A0(r2)
    la r31, lbl_8047B1F0(r13)
@L_8018F354:
    lfs f1, lbl_8047D7A0(r2)
    addi r3, r1, 0x8
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    li r29, 0x0
    b @L_8018F404
@L_8018F370:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    mr r28, r3
    cmplwi r0, 0x0
    beq @L_8018F400
    bl fn_8018FBD4
    cmplwi r3, 0x0
    beq @L_8018F400
    bl fn_800E3D08
    clrlwi. r0, r3, 24
    beq @L_8018F400
    lwz r0, 0x28(r28)
    lwz r3, 0x2c(r28)
    cmplwi r0, 0x0
    bne @L_8018F3C8
    cmplwi r3, 0x64
    beq @L_8018F3C0
    cmplwi r3, 0x65
    bne @L_8018F3C8
@L_8018F3C0:
    li r0, 0x1
    b @L_8018F3CC
@L_8018F3C8:
    li r0, 0x0
@L_8018F3CC:
    clrlwi r4, r0, 24
    subfic r3, r4, 0x1
    subi r0, r4, 0x1
    or r0, r3, r0
    srwi r0, r0, 31
    cmpw r0, r30
    bne @L_8018F400
    mr r3, r28
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x8
    bl fn_800E01D0
    b @L_8018F410
@L_8018F400:
    addi r29, r29, 0x1
@L_8018F404:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018F370
@L_8018F410:
    stfs f30, 0xc(r1)
    addi r4, r1, 0x8
    lwz r3, 0x0(r31)
    bl fn_800DCC3C
    stfs f31, 0xc(r1)
    addi r4, r1, 0x8
    lwz r3, 0x0(r31)
    bl fn_800DCC60
    addi r30, r30, 0x1
    addi r31, r31, 0x4
    cmpwi r30, 0x2
    blt @L_8018F354
@L_8018F440:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r0, 0x54(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
}
#else
void fn_8018F30C(void) { /* TODO */ }
#endif
#pragma pop
u32 fn_8018F470(u32 r3) {
    if (r3 >= 2) {
        return 0;
    }
    return lbl_8047B1F0[r3];
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8018F490(void) {
#include "src/game/people/people_fn_8018F490.inc"
}
#else
#pragma optimization_level 4
u32 fn_8018F490(void* p) {
    if (p != NULL) {
        return (*(u8*)p >> 4) & 1;
    }
    return 0;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8018F4AC(void) {
#include "src/game/people/people_fn_8018F4AC.inc"
}
#else
#pragma optimization_level 4
u32 fn_8018F4AC(void* p) {
    if (p != NULL) {
        return (*(u8*)p >> 5) & 7;
    }
    return 0;
}
#endif
#pragma pop
extern f32 lbl_8047D8A8;
f32 fn_8018F5B4(u8* ptr) {
    if (ptr != NULL) {
        return *(f32*)(ptr + 0x14);
    }
    return lbl_8047D8A8;
}
f32 fn_8018F5CC(u8* ptr) {
    if (ptr != NULL) {
        return *(f32*)(ptr + 0x10);
    }
    return lbl_8047D8A8;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8018F5FC(void) {
#include "src/game/people/people_fn_8018F5FC.inc"
}
#else
#pragma optimization_level 4
u32 fn_8018F5FC(void* p) {
    if (p != NULL) {
        return (*(u8*)p >> 2) & 3;
    }
    return 0;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8018F6B4(void) {
#include "src/game/people/people_fn_8018F6B4.inc"
}
#else
#pragma optimization_level 4
void* fn_8018F6B4(void* p) {
    if (p != NULL) {
        return *(void**)((u8*)p + 0x0C);
    }
    return NULL;
}
#endif
#pragma pop
extern u32 lbl_80478E78;
extern u32 lbl_80478E7C;
extern s32 lbl_8047B1F8;
extern PeopleEntry* lbl_8047B200;
u8* fn_8018F6CC(u32 index) {
    if (index >= *(u32*)lbl_80478E78) {
        return (u8*)0;
    }
    return (u8*)lbl_80478E7C + index * 0x2C;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8018F730(void) {
#include "src/game/people/people_fn_8018F730.inc"
}
#else
#pragma optimization_level 4
u32 fn_8018F730(void) {
    PeopleEntry* current;
    PeopleEntry* entry = lbl_8047B200;
    s32 count = lbl_8047B1F8;
    u32 total;
    s32 i;

    total = 0;
    for (i = 0; i < count; i++) {
        if (i < 0 || count <= i) {
            current = NULL;
        } else {
            current = entry;
        }
        if (current->active != 0) {
            total += PEOPLE_SPAWN_DATA_SIZE;
        }
        entry = (PeopleEntry*)((u8*)entry + PEOPLE_ENTRY_SIZE);
    }

    return total;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8018F788(void) {
#include "src/game/people/people_fn_8018F788.inc"
}
#else
void fn_8018F788(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8018F87C(void) {
    nofralloc
    stwu r1, -0x140(r1)
    mflr r0
    stw r0, 0x144(r1)
    stmw r25, 0x124(r1)
    mr r30, r3
    mr r25, r4
    li r4, 0x0
    lwz r0, lbl_8047B1F8(r13)
    lwz r3, lbl_8047B200(r13)
    mulli r5, r0, 0xdc
    bl memset
    lis r3, 0x2fa1
    subi r0, r3, 0x417d
    mulhwu r0, r0, r25
    srwi r31, r0, 6
    mulli r0, r31, 0x158
    subf. r0, r0, r25
    bne @L_8018FB18
    cmplwi r31, 0x40
    bgt @L_8018FB18
    mr r26, r30
    mr r25, r31
    addi r28, r1, 0x8
    li r29, 0x0
    b @L_8018F990
@L_8018F8E0:
    lwz r5, lbl_8047B1F8(r13)
    li r3, 0x0
    lwz r4, lbl_8047B200(r13)
    mtctr r5
    cmpwi r5, 0x0
    ble @L_8018F95C
@L_8018F8F8:
    cmpwi r3, 0x0
    blt @L_8018F908
    cmpw r5, r3
    bgt @L_8018F910
@L_8018F908:
    li r27, 0x0
    b @L_8018F914
@L_8018F910:
    mr r27, r4
@L_8018F914:
    lbz r0, 0x0(r27)
    cmplwi r0, 0x0
    bne @L_8018F950
    mr r3, r27
    li r4, 0x0
    li r5, 0xdc
    bl memset
    li r3, 0x1
    li r0, -0x1
    stb r3, 0x0(r27)
    lfs f0, lbl_8047D8B0(r2)
    stw r27, 0x4(r27)
    stw r0, 0x50(r27)
    stfs f0, 0x58(r27)
    b @L_8018F960
@L_8018F950:
    addi r4, r4, 0xdc
    addi r3, r3, 0x1
    bdnz @L_8018F8F8
@L_8018F95C:
    li r27, 0x0
@L_8018F960:
    lwz r4, 0x8(r26)
    mr r3, r27
    lwz r5, 0xc(r26)
    lwz r6, 0x10(r26)
    bl fn_8018E1C4
    mr r4, r26
    addi r3, r27, 0x20
    li r5, 0xbc
    bl memcpy
    stwx r27, r28, r29
    addi r26, r26, 0x158
    addi r29, r29, 0x4
@L_8018F990:
    cmplwi r25, 0x0
    subi r25, r25, 0x1
    bne @L_8018F8E0
    mr r27, r30
    addi r29, r1, 0x8
    li r30, 0x0
    b @L_8018FB0C
@L_8018F9AC:
    lwzx r28, r29, r30
    addi r30, r30, 0x4
    lwz r3, 0x8(r28)
    lbz r4, 0x21(r28)
    cmplwi r3, 0x0
    mr r25, r3
    beq @L_8018F9D0
    stb r4, 0x21(r28)
    bl fn_800E4014
@L_8018F9D0:
    lwz r3, 0x50(r28)
    lbz r4, 0x23(r28)
    cmpwi r3, 0x0
    blt @L_8018F9E4
    bl fn_8010FFC4
@L_8018F9E4:
    lwz r4, 0x90(r28)
    mr r3, r28
    bl fn_8018F08C
    mr r3, r25
    addi r4, r27, 0xec
    bl GSmodelPopState
    lwz r4, 0xc8(r28)
    cmpwi r4, -0x1
    beq @L_8018FA58
    lwz r5, 0xcc(r28)
    cmpwi r5, -0x1
    beq @L_8018FA58
    lwz r6, 0xd0(r28)
    mr r3, r25
    bl fn_801848D0
    mr r3, r25
    bl fn_800E3D00
    addi r4, r27, 0xbc
    bl fn_800E01D0
    mr r3, r25
    bl fn_800E3CF8
    addi r4, r27, 0xc8
    bl fn_800E01D0
    mr r3, r25
    addi r4, r27, 0xd4
    bl fn_800E43A4
    mr r3, r25
    addi r4, r27, 0xe0
    bl fn_800E4170
@L_8018FA58:
    lbz r0, 0x96(r28)
    cmpwi r0, 0x1
    beq @L_8018FA90
    bge @L_8018FA74
    cmpwi r0, 0x0
    bge @L_8018FA80
    b @L_8018FAE4
@L_8018FA74:
    cmpwi r0, 0x3
    bge @L_8018FAE4
    b @L_8018FAA8
@L_8018FA80:
    lwz r3, 0x28(r28)
    lwz r4, 0x2c(r28)
    bl fn_80188AF4
    b @L_8018FAE4
@L_8018FA90:
    lwz r3, 0x28(r28)
    lwz r4, 0x2c(r28)
    lwz r5, 0xc0(r28)
    lwz r6, 0xc4(r28)
    bl fn_80188FA0
    b @L_8018FAE4
@L_8018FAA8:
    lfs f2, 0xa8(r28)
    lfs f1, 0xac(r28)
    lfs f0, 0xb0(r28)
    fctiwz f2, f2
    fctiwz f1, f1
    lwz r3, 0x28(r28)
    fctiwz f0, f0
    lwz r4, 0x2c(r28)
    stfd f2, 0x108(r1)
    stfd f1, 0x110(r1)
    lwz r5, 0x10c(r1)
    stfd f0, 0x118(r1)
    lwz r6, 0x114(r1)
    lwz r7, 0x11c(r1)
    bl fn_80188CA0
@L_8018FAE4:
    lbz r0, 0x96(r28)
    cmplwi r0, 0x0
    beq @L_8018FB08
    li r25, 0x0
@L_8018FAF4:
    mr r3, r28
    bl fn_801812C4
    addi r25, r25, 0x1
    cmplwi r25, 0x3c
    blt @L_8018FAF4
@L_8018FB08:
    addi r27, r27, 0x158
@L_8018FB0C:
    cmplwi r31, 0x0
    subi r31, r31, 0x1
    bne @L_8018F9AC
@L_8018FB18:
    lmw r25, 0x124(r1)
    lwz r0, 0x144(r1)
    mtlr r0
    addi r1, r1, 0x140
    blr
}
#else
void fn_8018F87C(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8018FC50(void) {
#include "src/game/people/people_fn_8018FC50.inc"
}
#else
void fn_8018FC50(PeopleEntry* entry) {
    fn_800E3CF8(entry->modelHandle);
}
#endif
#pragma pop
