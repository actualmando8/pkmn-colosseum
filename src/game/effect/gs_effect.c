/**
 * @file gs_effect.c
 * @brief GSeffect -- Core effect manager for Pokemon Colosseum.
 *
 * Decompiled from:
 *   fn_80130CE0 (GSEffectInit)        -- System initialisation
 *   fn_80130F04 (effectUpdateTask)    -- Per-frame update callback
 *   fn_80130F68 (effectRenderTask)    -- Per-frame render callback
 *   fn_80131010 (GSEffectStop)        -- Stop an active effect
 *   fn_801310A8 (GSEffectIsActive)    -- Query effect active state
 *   fn_8013111C (GSEffectTrigger)     -- Trigger an effect by ID
 *   fn_80131200 (GSEffectRegister)    -- Register callbacks for an effect
 *   fn_80131268 (GSEffectFree)        -- Destroy and free an effect slot
 *   fn_8013139C (GSEffectResetState)  -- Re-trigger an existing effect
 *   fn_80131428 (GSEffectAllocSlot)   -- Allocate a new effect slot
 *
 * Debug strings:
 *   "GSeffect: Cannot trigger effect - instance uninitialised: effect ID %d."
 *       (lbl_80272A58 -- referenced in GSEffectTrigger)
 *
 * Global state:
 *   lbl_803635C0 -- GSEffectGlobals (0x18 bytes in .data)
 *
 * The effect system uses a pool of GSEffectInstance entries (each 0x34 bytes).
 * Slots are managed via two doubly-linked lists: a free list (for unused
 * slots) and an active list (for running effects).
 *
 * Per-frame execution is driven by two task callbacks registered with
 * GStaskRegister (fn_800FE834):
 *   - effectRenderTask at priority 0x7F (called during the render pass)
 *   - effectUpdateTask at priority 0x80 (called after rendering)
 *
 * Address range: 0x80130CE0 - 0x8013151C
 */

#include "dolphin/types.h"
#include "game/effect/gs_effect.h"
#include "game/gs_mem.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);          /* OSReport / GSlog */
extern u16   GSmemAllocRaw(u32 size);                    /* fn_800E3534 */
extern void* GSmemGetPtr(u16 handle);                    /* fn_800E27B0 */
extern void* GSmemLock(u16 handle);                      /* fn_800E24B0 */
extern void  GSmemFree(u16 handle);                      /* fn_800E209C */
extern u32   GStaskRegister(u32 type, u32 priority,
                             void* param, void* func);   /* fn_800FE834 */
extern u32   GSgfxGetTickCount(void);                    /* fn_800D3088 */
extern void  fn_801E12A0(void);                          /* battle VFX init */
extern void  memset(void* dst, u32 val, u32 size);
extern void  memcpy(void* dst, void* src, u32 size);

/* ===== String constants (rodata) ===== */
extern const char lbl_80272A58[]; /* "GSeffect: Cannot trigger effect..." */

/* ===== Global state ===== */

/*
 * The master effect globals at lbl_803635C0.
 * Layout (0x18 bytes):
 *   0x00: u32  maxEffects
 *   0x04: u16  memHandle
 *   0x06: u16  (pad)
 *   0x08: GSEffectInstance*  freeListHead
 *   0x0C: void*              instanceTable (raw pointer from GSmemGetPtr)
 *   0x10: GSEffectInstance*  activeListHead
 *   0x14: u32  reserved
 */
static GSEffectGlobals gsEffectGlobals;  /* @data lbl_803635C0 */

/* Task handle for the render callback (stored at lbl_8047ADC0 via sda21) */
static u32 gsEffectRenderTaskHandle;     /* @sda21 lbl_8047ADC0 */

/* -----------------------------------------------------------------------
 * Internal helper: look up an effect by ID.
 *
 * Validates the ID is in range, computes the table offset (mulli 0x34),
 * and verifies the slot is not in the UNINIT state.
 *
 * This pattern appears identically in GSEffectTrigger, GSEffectRegister,
 * GSEffectStop, GSEffectFree, GSEffectIsActive, and GSEffectResetState.
 * ----------------------------------------------------------------------- */
static GSEffectInstance* effectLookup(u32 effectId) {
    GSEffectInstance* inst;

    if (effectId == 0) {
        return NULL;
    }
    if (effectId > gsEffectGlobals.maxEffects) {
        return NULL;
    }

    /* Instance table: each slot is 0x34 bytes, 1-based indexing */
    inst = (GSEffectInstance*)((u8*)gsEffectGlobals.instanceTable +
                               (effectId - 1) * sizeof(GSEffectInstance));

    if (inst->state == GSEFFECT_STATE_UNINIT) {
        return NULL;
    }

    return inst;
}

/* =======================================================================
 *  effectUpdateTask / fn_80130F04
 *  Address: 0x80130F04, Size: 0x64
 *
 *  Per-frame task callback (priority 0x80).  Walks the active list and
 *  invokes each effect's renderFunc if the effect is in ACTIVE state.
 *
 *  Assembly:
 *    lwz r31, 0x10(gsEffectGlobals)  ; load activeListHead
 *    loop:
 *      cmplwi r31, 0 -> done
 *      lwz r12, 0x1C(r31)           ; renderFunc
 *      cmplwi r12, 0 -> skip
 *      lwz r0, 0x04(r31)            ; state
 *      cmpwi r0, 2 -> skip if != ACTIVE
 *      lwz r3, 0x24(r31)            ; userData
 *      mtctr r12 / bctrl
 *    skip:
 *      lwz r31, 0x2C(r31)           ; next
 *      b loop
 * ======================================================================= */
static void effectUpdateTask(void) {
    GSEffectInstance* cur;

    cur = gsEffectGlobals.activeListHead;
    while (cur != NULL) {
        if (cur->renderFunc != NULL && cur->state == GSEFFECT_STATE_ACTIVE) {
            cur->renderFunc(cur->userData);
        }
        cur = cur->next;
    }
}

/* =======================================================================
 *  effectRenderTask / fn_80130F68
 *  Address: 0x80130F68, Size: 0xA8
 *
 *  Per-frame task callback (priority 0x7F).  Walks the active list and
 *  invokes each effect's updateFunc.  If the update returns 0 (failure /
 *  done), the effect is transitioned to STOPPING and its stopFunc is
 *  called.
 *
 *  Assembly:
 *    lwz r30, 0x10(gsEffectGlobals)  ; load activeListHead
 *    bl GSgfxGetTickCount            ; r3 = tick, saved as r29
 *    li r31, 1                       ; constant for STOPPING state
 *    loop:
 *      cmplwi r30, 0 -> done
 *      lwz r12, 0x18(r30)           ; updateFunc
 *      cmplwi r12, 0 -> skip
 *      lwz r0, 0x04(r30)            ; state
 *      cmpwi r0, 2 -> skip if != ACTIVE
 *      mr r4, r29                   ; tickCount
 *      lwz r3, 0x24(r30)            ; userData
 *      mtctr r12 / bctrl            ; call updateFunc(userData, tickCount)
 *      cmplwi r3, 0                 ; check result
 *      bne skip                     ; nonzero = keep running
 *      stw r31, 0x04(r30)           ; state = STOPPING
 *      lwz r12, 0x14(r30)           ; stopFunc
 *      cmplwi r12, 0 -> skip
 *      lwz r3, 0x24(r30)            ; userData
 *      mtctr r12 / bctrl            ; call stopFunc(userData)
 *    skip:
 *      lwz r30, 0x2C(r30)           ; next
 *      b loop
 * ======================================================================= */
static void effectRenderTask(void) {
    GSEffectInstance* cur;
    u32 tickCount;

    cur = gsEffectGlobals.activeListHead;
    tickCount = GSgfxGetTickCount();

    while (cur != NULL) {
        if (cur->updateFunc != NULL && cur->state == GSEFFECT_STATE_ACTIVE) {
            BOOL result = ((GSEffectStartFunc)cur->updateFunc)(cur->userData, tickCount);
            if (result == 0) {
                /* Effect finished -- transition to stopping */
                cur->state = GSEFFECT_STATE_STOPPING;
                if (cur->stopFunc != NULL) {
                    cur->stopFunc(cur->userData);
                }
            }
        }
        cur = cur->next;
    }
}

/* =======================================================================
 *  GSEffectInit / fn_80130CE0
 *  Address: 0x80130CE0, Size: 0x224
 *
 *  Allocates the effect instance table from GSmem (maxEffects * 0x34 bytes),
 *  builds a doubly-linked free list, and registers the two per-frame task
 *  callbacks.
 *
 *  Assembly sequence:
 *  1. clrlwi r25, r3, 16            ; maxEffects (u16)
 *  2. mulli r23, r25, 0x34          ; total table size
 *  3. bl GSmemAllocRaw(tableSize)   ; r3 = memHandle
 *  4. sth r3, 0x04(globals)         ; store handle
 *  5. if (handle == 0) goto fail
 *  6. bl GSmemGetPtr(handle)        ; r3 = table pointer
 *  7. stw r3, 0x0C(globals)         ; instanceTable
 *  8. memset(table, 0, tableSize)   ; zero the table
 *  9. stw table, 0x08(globals)      ; freeListHead = first entry
 * 10. Build linked list: for each entry, set prev/next pointers
 *     and store the 1-based ID at offset 0x00.
 *     The last entry gets next=NULL and state=-1 (UNINIT sentinel).
 * 11. stw maxEffects, globals+0x00
 * 12. Set globals+0x10 (activeListHead) = 0
 * 13. Register render task:  GStaskRegister(1, 0x7F, 0, effectRenderTask)
 *     Store returned handle at lbl_8047ADC0
 * 14. bl fn_801E12A0                ; battle VFX init
 * 15. Register update task:  GStaskRegister(1, 0x80, 0, effectUpdateTask)
 * ======================================================================= */
void GSEffectInit(u16 maxEffects) {
    u32 tableSize;
    u16 memHandle;
    GSEffectInstance* table;
    GSEffectInstance* entry;
    u16 i;

    tableSize = (u32)maxEffects * sizeof(GSEffectInstance);

    /* Allocate the instance table from GSmem */
    memHandle = GSmemAllocRaw(tableSize);
    gsEffectGlobals.memHandle = memHandle;

    if (memHandle == 0) {
        /* Allocation failed */
        gsEffectGlobals.instanceTable = NULL;
        gsEffectGlobals.freeListHead = NULL;
        gsEffectGlobals.maxEffects = 0;
        goto register_tasks;
    }

    table = (GSEffectInstance*)GSmemGetPtr(memHandle);
    gsEffectGlobals.instanceTable = table;

    /* Zero the entire table */
    memset(table, 0, tableSize);

    /* Set the free list head to the first entry */
    gsEffectGlobals.freeListHead = table;

    /* Build the doubly-linked free list */
    table[0].prev = NULL;
    table[0].next = &table[1];
    table[0].id = 1;

    for (i = 1; i < maxEffects - 1; i++) {
        entry = &table[i];
        entry->prev = &table[i - 1];
        entry->next = &table[i + 1];
        entry->id = i + 1;
    }

    /* Last entry: terminate the list */
    entry = &table[maxEffects - 1];
    entry->prev = &table[maxEffects - 2];
    entry->next = NULL;
    entry->id = maxEffects;
    entry->state = GSEFFECT_STATE_UNINIT;

    /* Store the max count */
    gsEffectGlobals.maxEffects = (u32)maxEffects;

register_tasks:
    /* Clear the active list */
    gsEffectGlobals.activeListHead = NULL;

    /* Register per-frame task callbacks:
     * Render task at priority 0x7F (runs first, draws effects)
     * Update task at priority 0x80 (runs second, updates logic) */
    gsEffectRenderTaskHandle = GStaskRegister(1, 0x7F, 0,
                                               (void*)effectRenderTask);

    /* Initialise battle VFX subsystem */
    fn_801E12A0();

    /* Register the update task */
    GStaskRegister(1, 0x80, 0, (void*)effectUpdateTask);
}

/* =======================================================================
 *  GSEffectTrigger / fn_8013111C
 *  Address: 0x8013111C, Size: 0xE4
 *
 *  Triggers an effect by its 1-based ID.  The function:
 *  1. Looks up the effect instance (via effectLookup).
 *  2. Verifies the effect is in a valid state (not IDLE/0x00).
 *  3. Sets state = ACTIVE.
 *  4. Calls triggerFunc(userData).
 *  5. If triggerFunc returns 0 (failure), calls stopFunc(userData)
 *     and transitions the effect back to STOPPING.
 *
 *  Assembly:
 *    cmplwi r3, 0          ; effectId == 0 -> fail
 *    beq .null
 *    <effectLookup pattern>
 *    cmplwi r31, 0         ; instance == NULL -> fail
 *    beq .fail
 *    lwz r0, 0x04(r31)     ; state
 *    cmpwi r0, 0           ; IDLE -> print error
 *    beq .print_error
 *    lwz r12, 0x10(r31)    ; triggerFunc
 *    li r0, 2
 *    cmplwi r12, 0
 *    stw r0, 0x04(r31)     ; state = ACTIVE
 *    beq .fail
 *    lwz r3, 0x24(r31)     ; userData
 *    mtctr r12 / bctrl     ; result = triggerFunc(userData)
 *    cmplwi r3, 0
 *    bne .success           ; nonzero = OK
 *    lwz r12, 0x14(r31)    ; stopFunc
 *    li r0, 1
 *    stw r0, 0x04(r31)     ; state = STOPPING
 *    cmplwi r12, 0
 *    beq .ret_zero
 *    lwz r3, 0x24(r31)     ; userData
 *    mtctr r12 / bctrl     ; stopFunc(userData)
 *    li r3, 0
 *    blr
 *  .success:
 *    li r3, 1
 *    blr
 *  .print_error:
 *    lis r3, lbl_80272A58@ha
 *    addi r3, r3, lbl_80272A58@l
 *    crclr cr1eq
 *    bl OSReport
 *  .fail:
 *    li r3, 0
 *    blr
 * ======================================================================= */
BOOL GSEffectTrigger(u32 effectId) {
    GSEffectInstance* inst;

    inst = effectLookup(effectId);

    if (inst == NULL) {
        return FALSE;
    }

    /* Effect must be in a registered (non-IDLE) state to trigger */
    if (inst->state == GSEFFECT_STATE_IDLE) {
        /* Effect slot exists but has no active registration */
        fn_800DD970(lbl_80272A58, effectId);
        return FALSE;
    }

    /* Transition to ACTIVE */
    inst->state = GSEFFECT_STATE_ACTIVE;

    if (inst->triggerFunc == NULL) {
        return FALSE;
    }

    /* Call the trigger callback */
    if (inst->triggerFunc(inst->userData, 0) != 0) {
        /* Trigger succeeded */
        return TRUE;
    }

    /* Trigger failed -- transition to STOPPING */
    inst->state = GSEFFECT_STATE_STOPPING;
    if (inst->stopFunc != NULL) {
        inst->stopFunc(inst->userData);
    }

    return FALSE;
}

/* =======================================================================
 *  GSEffectRegister / fn_80131200
 *  Address: 0x80131200, Size: 0x68
 *
 *  Simply stores callback pointers into the effect instance.
 *
 *  Assembly:
 *    <effectLookup pattern>
 *    cmplwi r3, 0 -> return
 *    stw r4, 0x08(r3)   ; startFunc
 *    stw r5, 0x0C(r3)   ; destroyFunc
 *    stw r6, 0x10(r3)   ; triggerFunc
 *    stw r7, 0x14(r3)   ; stopFunc
 *    stw r8, 0x20(r3)   ; extraParam
 *    stw r9, 0x18(r3)   ; updateFunc
 *    stw r10, 0x1C(r3)  ; renderFunc
 *    blr
 * ======================================================================= */
void GSEffectRegister(u32 effectId, GSEffectStartFunc startFunc,
                      GSEffectStopFunc destroyFunc,
                      GSEffectStartFunc triggerFunc,
                      GSEffectStopFunc stopFunc,
                      void* extraParam,
                      GSEffectUpdateFunc updateFunc,
                      GSEffectRenderFunc renderFunc) {
    GSEffectInstance* inst;

    inst = effectLookup(effectId);
    if (inst == NULL) {
        return;
    }

    inst->startFunc   = startFunc;
    inst->destroyFunc = destroyFunc;
    inst->triggerFunc = triggerFunc;
    inst->stopFunc    = stopFunc;
    inst->extraParam  = extraParam;
    inst->updateFunc  = updateFunc;
    inst->renderFunc  = renderFunc;
}

/* =======================================================================
 *  GSEffectStop / fn_80131010
 *  Address: 0x80131010, Size: 0x98
 *
 *  Requests an active effect to stop gracefully.
 *
 *  Assembly:
 *    <effectLookup pattern>
 *    cmplwi r3, 0 -> return
 *    lwz r0, 0x04(r3)      ; state
 *    lwz r12, 0x14(r3)     ; stopFunc
 *    cmpwi r0, 0 -> return  ; IDLE -> nothing to stop
 *    cmpwi r0, 1 -> return  ; STOPPING -> already stopping
 *    li r0, 1
 *    cmplwi r12, 0
 *    stw r0, 0x04(r3)      ; state = STOPPING
 *    beq .done
 *    lwz r3, 0x24(r3)      ; userData
 *    mtctr r12 / bctrl     ; stopFunc(userData)
 * ======================================================================= */
void GSEffectStop(u32 effectId) {
    GSEffectInstance* inst;

    inst = effectLookup(effectId);
    if (inst == NULL) {
        return;
    }

    /* Can only stop effects that are currently active */
    if (inst->state == GSEFFECT_STATE_IDLE ||
        inst->state == GSEFFECT_STATE_STOPPING) {
        return;
    }

    /* Transition to STOPPING and invoke the stop callback */
    inst->state = GSEFFECT_STATE_STOPPING;
    if (inst->stopFunc != NULL) {
        inst->stopFunc(inst->userData);
    }
}

/* =======================================================================
 *  GSEffectIsActive / fn_801310A8
 *  Address: 0x801310A8, Size: 0x74
 *
 *  Returns TRUE if the effect is in ACTIVE state.
 *
 *  Assembly:
 *    <effectLookup pattern>
 *    cmplwi r3, 0 -> return 0
 *    lwz r0, 0x04(r3)
 *    cmpwi r0, -1 -> return 0  ; UNINIT
 *    cmpwi r0, 0  -> return 0  ; IDLE
 *    cmpwi r0, 1  -> return 0  ; STOPPING
 *    li r3, 1                   ; ACTIVE
 *    blr
 * ======================================================================= */
BOOL GSEffectIsActive(u32 effectId) {
    GSEffectInstance* inst;

    inst = effectLookup(effectId);
    if (inst == NULL) {
        return FALSE;
    }

    if (inst->state == GSEFFECT_STATE_UNINIT ||
        inst->state == GSEFFECT_STATE_IDLE ||
        inst->state == GSEFFECT_STATE_STOPPING) {
        return FALSE;
    }

    return TRUE;
}

/* =======================================================================
 *  GSEffectFree / fn_80131268
 *  Address: 0x80131268, Size: 0x134
 *
 *  Destroys an effect: calls stop and destroy callbacks, unlinks the
 *  instance from the active list, and returns it to the free list.
 *
 *  Assembly (abbreviated):
 *    <effectLookup pattern>  -> r31
 *    cmplwi r31, 0 -> return
 *    lwz r0, 0x04(r31)      ; state
 *    lwz r30, 0x30(r31)     ; prev
 *    lwz r29, 0x2C(r31)     ; next
 *    cmpwi r0, 0 -> skip_callbacks  ; IDLE -> skip
 *    cmpwi r0, 1 -> skip_stop       ; STOPPING -> skip stopFunc
 *    -- call stopFunc if needed --
 *    lwz r12, 0x14(r31)     ; stopFunc
 *    cmplwi r12, 0 -> skip_stop
 *    lwz r3, 0x24(r31)
 *    bctrl
 *  skip_stop:
 *    -- call destroyFunc --
 *    lwz r12, 0x0C(r31)     ; destroyFunc
 *    cmplwi r12, 0 -> skip_destroy
 *    lwz r3, 0x24(r31)
 *    bctrl
 *  skip_destroy:
 *    li r0, -1
 *    stw r0, 0x04(r31)      ; state = UNINIT
 *  -- Unlink from active list --
 *    if (prev != NULL):
 *      stw r29, 0x2C(r30)   ; prev->next = this->next
 *    else:
 *      stw r29, 0x10(globals) ; activeListHead = this->next
 *    if (next != NULL):
 *      stw r30, 0x30(r29)   ; next->prev = this->prev
 *  -- Return to free list --
 *    stw 0, 0x30(r31)       ; this->prev = NULL
 *    lwz r0, 0x08(globals)  ; freeListHead
 *    stw r0, 0x2C(r31)      ; this->next = old freeHead
 *    if (old freeHead != NULL):
 *      stw r31, 0x30(old freeHead)  ; old freeHead->prev = this
 *    stw r31, 0x08(globals) ; freeListHead = this
 *    -- Free the user data memory --
 *    lhz r3, 0x28(r31)     ; memHandle
 *    bl GSmemLock
 *    lhz r3, 0x28(r31)
 *    bl GSmemFree
 * ======================================================================= */
void GSEffectFree(u32 effectId) {
    GSEffectInstance* inst;
    GSEffectInstance* prevInst;
    GSEffectInstance* nextInst;

    inst = effectLookup(effectId);
    if (inst == NULL) {
        return;
    }

    prevInst = inst->prev;
    nextInst = inst->next;

    /* Call stop callback if effect is still active */
    if (inst->state != GSEFFECT_STATE_IDLE) {
        if (inst->state != GSEFFECT_STATE_STOPPING) {
            /* Call stopFunc for effects that haven't begun stopping */
            if (inst->stopFunc != NULL) {
                inst->stopFunc(inst->userData);
            }
        }

        /* Call destroy callback */
        if (inst->destroyFunc != NULL) {
            inst->destroyFunc(inst->userData);
        }

        /* Mark as uninitialised */
        inst->state = GSEFFECT_STATE_UNINIT;
    }

    /* Unlink from the active list */
    if (prevInst != NULL) {
        prevInst->next = nextInst;
    } else {
        gsEffectGlobals.activeListHead = nextInst;
    }

    if (nextInst != NULL) {
        nextInst->prev = prevInst;
    }

    /* Return to the free list (prepend) */
    inst->prev = NULL;
    inst->next = gsEffectGlobals.freeListHead;
    if (gsEffectGlobals.freeListHead != NULL) {
        gsEffectGlobals.freeListHead->prev = inst;
    }
    gsEffectGlobals.freeListHead = inst;

    /* Free the user-data memory block */
    GSmemLock(inst->memHandle);
    GSmemFree(inst->memHandle);
}

/* =======================================================================
 *  GSEffectResetState / fn_8013139C
 *  Address: 0x8013139C, Size: 0x8C
 *
 *  Re-triggers a registered effect: calls the startFunc callback and
 *  sets the state to STOPPING.
 *
 *  Assembly:
 *    <effectLookup pattern>  -> r31
 *    cmplwi r31, 0 -> return
 *    lwz r12, 0x08(r31)     ; startFunc
 *    cmplwi r12, 0 -> skip
 *    lwz r3, 0x24(r31)      ; userData
 *    bctrl
 *  skip:
 *    li r0, 1
 *    stw r0, 0x04(r31)      ; state = STOPPING
 * ======================================================================= */
void GSEffectResetState(u32 effectId) {
    GSEffectInstance* inst;

    inst = effectLookup(effectId);
    if (inst == NULL) {
        return;
    }

    /* Call the start callback to re-initialise */
    if (inst->startFunc != NULL) {
        inst->startFunc(inst->userData, 0);
    }

    /* Set state to STOPPING (effect needs to be re-triggered) */
    inst->state = GSEFFECT_STATE_STOPPING;
}

/* =======================================================================
 *  GSEffectAllocSlot / fn_80131428
 *  Address: 0x80131428, Size: 0xF4
 *
 *  Allocates a new effect slot from the free list.  The slot's user data
 *  is allocated from GSmem.  The callback data is memcpy'd into the
 *  newly allocated user-data region.
 *
 *  Assembly (abbreviated):
 *    lwz r28, 0x08(globals)    ; freeListHead
 *    cmplwi r28, 0 -> return 0
 *    lhz r27, 0x00(r28)       ; save the ID
 *    clrlwi r29, r25, 16      ; dataSize
 *    bl GSmemAllocRaw(dataSize); r31 = memHandle
 *    clrlwi. r0, r3, 16
 *    beq .fail                  ; allocation failed
 *    bl GSmemGetPtr(handle)     ; r26 = userData pointer
 *    -- Remove from free list --
 *    lwz r4, 0x2C(r28)         ; next
 *    stw r4, 0x08(globals)     ; freeListHead = next
 *    if (next != NULL):
 *      stw 0, 0x30(next)       ; next->prev = NULL
 *    -- Clear the slot --
 *    memset(r28, 0, 0x34)
 *    sth r27, 0x00(r28)        ; restore ID
 *    -- Insert into active list (prepend) --
 *    lwz r3, 0x10(globals)     ; activeListHead
 *    if (r3 != NULL):
 *      stw r28, 0x30(r3)       ; old head->prev = this
 *    stw activeHead, 0x2C(r28) ; this->next = old head
 *    stw r28, 0x10(globals)    ; activeListHead = this
 *    stw 0, 0x30(r28)          ; this->prev = NULL
 *    sth r31, 0x28(r28)        ; memHandle
 *    stw r26, 0x24(r28)        ; userData
 *    -- Copy callback data into userData --
 *    memcpy(r26, callbacks, dataSize)
 *    sth dataSize, 0x02(r28)   ; store dataSize
 *    stw 0, 0x04(r28)          ; state = IDLE
 *    lhz r3, 0x00(r28)         ; return the effect ID
 * ======================================================================= */
u16 GSEffectAllocSlot(void* callbacks, u16 dataSize) {
    GSEffectInstance* slot;
    u16 savedId;
    u16 memHandle;
    void* userData;
    GSEffectInstance* oldHead;

    slot = gsEffectGlobals.freeListHead;
    if (slot == NULL) {
        return 0;
    }

    /* Save the slot's ID before clearing */
    savedId = slot->id;

    /* Allocate user-data memory */
    memHandle = GSmemAllocRaw((u32)dataSize);
    if (memHandle == 0) {
        return 0;
    }

    userData = GSmemGetPtr(memHandle);

    /* Remove from free list */
    gsEffectGlobals.freeListHead = slot->next;
    if (slot->next != NULL) {
        slot->next->prev = NULL;
    }

    /* Clear the slot */
    memset(slot, 0, sizeof(GSEffectInstance));

    /* Restore the ID */
    slot->id = savedId;

    /* Insert into active list (prepend) */
    oldHead = gsEffectGlobals.activeListHead;
    if (oldHead != NULL) {
        oldHead->prev = slot;
    }
    slot->next = oldHead;
    gsEffectGlobals.activeListHead = slot;
    slot->prev = NULL;

    /* Store memory handle and user-data pointer */
    slot->memHandle = memHandle;
    slot->userData = userData;

    /* Copy callback configuration into user data */
    memcpy(userData, callbacks, (u32)dataSize);

    /* Set the data size and initial state */
    slot->dataSize = dataSize;
    slot->state = GSEFFECT_STATE_IDLE;

    return slot->id;
}
