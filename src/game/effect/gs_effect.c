/**
 * @file gs_effect.c
 * @brief GSeffect -- Core effect manager for Pokemon Colosseum.
 *
 * Implements the fn_ symbols in [0x80130CE0, 0x8013151C), all matched at
 * 100% against the retail binary:
 *   fn_80130CE0 (GSEffectInit)        -- System initialisation
 *   fn_80130F04 (effectUpdateTask)    -- Per-frame update callback
 *   fn_80130F68 (effectRenderTask)    -- Per-frame render callback
 *   fn_80131010 (GSEffectStop)        -- Stop an active effect
 *   fn_801310A8 (GSEffectIsActive)    -- Query effect active state
 *   GSeffect (GSEffectTrigger)     -- Trigger an effect by ID (real symbol)
 *   fn_80131200 (GSEffectRegister)    -- Register callbacks for an effect
 *   fn_80131268 (GSEffectFree)        -- Destroy and free an effect slot
 *   fn_8013139C (GSEffectResetState)  -- Re-trigger an existing effect
 *   fn_80131428 (GSEffectAllocSlot)   -- Allocate a new effect slot
 *
 * A prior campaign transplant had also left a full second, invented copy
 * of these ten functions under "GSEffectXxx"/effectUpdateTask/
 * effectRenderTask names (plus an invented effectLookup helper), none of
 * which were ever called from anywhere else in the tree -- the real fn_
 * names above are what every other file's extern declarations reference
 * (see effect_visual.c, tracefx.c). That dead duplicate has been removed;
 * only the matched fn_ implementations remain.
 *
 * Debug strings:
 *   "GSeffect: Cannot trigger effect - instance uninitialised: effect ID %d."
 *       (lbl_80272A58 -- referenced in GSeffect)
 *
 * Global state:
 *   lbl_803635C0 -- GSEffectGlobals (0x18 bytes in .data)
 *
 * The effect system uses a pool of GSEffectInstance entries (each 0x34 bytes).
 * Slots are managed via two doubly-linked lists: a free list (for unused
 * slots) and an active list (for running effects).
 *
 * Per-frame execution is driven by two task callbacks registered with
 * GSgappCreate (GStaskRegister):
 *   - fn_80130F68 (effectRenderTask) at priority 0x7F (render pass)
 *   - fn_80130F04 (effectUpdateTask) at priority 0x80 (after rendering)
 *
 * Address range: 0x80130CE0 - 0x8013151C
 */

#include "dolphin/types.h"
#include "game/effect/gs_effect.h"
#include "game/gs_mem.h"

/* ===== External SDK / engine functions ===== */
extern void  GSlogWrite(const char* fmt, ...);          /* OSReport / GSlog */
extern void  fn_800E24B0(u16 handle);                    /* GSmemLock (raw) */
extern void  fn_800E209C(u16 handle);                    /* GSmemFree (raw) */
extern u32   fn_800D3088(void);                          /* GSgfxGetTickCount */
extern void  GSvtrRegisterGSgapp(void);                          /* battle VFX init */
extern void  memset(void* dst, u32 val, u32 size);
extern void  memcpy(void* dst, void* src, u32 size);

/* ===== String constants (rodata) ===== */
extern const char lbl_80272A58[]; /* "GSeffect: Cannot trigger effect..." */

/* ===== Global state ===== */
extern u8 lbl_803635C0[];              /* master effect globals, 0x18 bytes */
#define gsEffectGlobals (*(GSEffectGlobals*)(void*)lbl_803635C0)

/* Forward declarations for converted functions */
void fn_80130CE0(u16 maxEffects);
void fn_80130F04(void);
void fn_80130F68(void);
void fn_80131010(u32 effectId);
BOOL fn_801310A8(u32 effectId);
BOOL GSeffect(u32 effectId);
void fn_80131200(u32 effectId, GSEffectStartFunc startFunc,
                 GSEffectStopFunc destroyFunc,
                 GSEffectStartFunc triggerFunc,
                 GSEffectStopFunc stopFunc,
                 void* extraParam,
                 GSEffectUpdateFunc updateFunc,
                 GSEffectRenderFunc renderFunc);
void fn_80131268(u32 effectId);
void fn_8013139C(u32 effectId, u32 param);
u16 fn_80131428(void* callbacks, u16 dataSize);



/* ===== Global state ===== */
extern u32 lbl_8047ADC0;

/* =======================================================================
 * fn_80130CE0 / GSEffectInit
 * Address: 0x80130CE0, Size: 0x224
 * ======================================================================= */
void fn_80130CE0(u16 maxEffects) {
    u32 memHandle;
    GSEffectInstance* table;
    GSEffectInstance* entry;
    u16 cnt;
    extern u16 _toolentryAlloc__FUl(u32);
    extern void* fn_800E27B0(u16);
    extern u32 GSgappCreate(u32, u32, u32, void*);

    memHandle = _toolentryAlloc__FUl(maxEffects * sizeof(GSEffectInstance));
    gsEffectGlobals.memHandle = memHandle;

    if (memHandle != 0) {
        table = (GSEffectInstance*)fn_800E27B0(memHandle);
        gsEffectGlobals.instanceTable = table;
        memset(table, 0, maxEffects * sizeof(GSEffectInstance));
        gsEffectGlobals.freeListHead = table;
        table->prev = NULL;
        table->next = table + 1;
        table->id = 1;
        entry = table + 1;
        for (cnt = 1; cnt < maxEffects - 1; cnt++) {
            entry->prev = entry - 1;
            entry->next = entry + 1;
            entry->id = cnt + 1;
            entry++;
        }
        entry->prev = entry - 1;
        entry->next = NULL;
        entry->id = maxEffects;
        entry->state = GSEFFECT_STATE_UNINIT;
        *(volatile u32*)&gsEffectGlobals.maxEffects = (u32)maxEffects;
    } else {
        gsEffectGlobals.instanceTable = NULL;
        gsEffectGlobals.freeListHead = NULL;
        gsEffectGlobals.maxEffects = 0;
    }
    gsEffectGlobals.activeListHead = NULL;
    lbl_8047ADC0 = GSgappCreate(1, 0x7F, 0, (void*)fn_80130F68);
    GSvtrRegisterGSgapp();
    GSgappCreate(1, 0x80, 0, (void*)fn_80130F04);
}

/* =======================================================================
 * fn_80130F04 / effectUpdateTask
 * Address: 0x80130F04, Size: 0x64
 * ======================================================================= */
void fn_80130F04(void) {
    GSEffectInstance* cur;

    cur = ((GSEffectGlobals*)(void*)lbl_803635C0)->activeListHead;
    while (cur != NULL) {
        if (cur->renderFunc != NULL && cur->state == GSEFFECT_STATE_ACTIVE) {
            cur->renderFunc(cur->userData);
        }
        cur = cur->next;
    }
}

/* =======================================================================
 * fn_80130F68 / effectRenderTask
 * Address: 0x80130F68, Size: 0xA8
 * ======================================================================= */
void fn_80130F68(void) {
    GSEffectInstance* cur;
    u32 tick;

    cur = ((GSEffectGlobals*)(void*)lbl_803635C0)->activeListHead;
    tick = fn_800D3088();

    while (cur != NULL) {
        if (cur->updateFunc != NULL && cur->state == GSEFFECT_STATE_ACTIVE) {
            if ((u32)((GSEffectStartFunc)cur->updateFunc)(cur->userData, tick) == 0) {
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
 * fn_80131010 / GSEffectStop
 * Address: 0x80131010, Size: 0x98
 * ======================================================================= */
void fn_80131010(u32 effectId) {
    GSEffectInstance* inst;
    GSEffectGlobals* g;

    if (effectId == 0) {
        goto _null;
    }
    g = (GSEffectGlobals*)(void*)lbl_803635C0;
    if (effectId > g->maxEffects) {
        goto _null;
    }
    inst = (GSEffectInstance*)((u8*)g->instanceTable +
                               (effectId - 1) * sizeof(GSEffectInstance));
    switch (inst->state) {
    case GSEFFECT_STATE_UNINIT:
        goto _null;
    }
    goto _got_inst;
_null:
    inst = NULL;
_got_inst:
    if (inst == NULL) {
        return;
    }
    {
        s32 state = inst->state;
        GSEffectStopFunc stopFn = inst->stopFunc;
        if (state == GSEFFECT_STATE_IDLE) {
            return;
        }
        if (state == GSEFFECT_STATE_STOPPING) {
            return;
        }
        inst->state = GSEFFECT_STATE_STOPPING;
        if (stopFn != NULL) {
            stopFn(inst->userData);
        }
    }
}

/* =======================================================================
 * fn_801310A8 / GSEffectIsActive
 * Address: 0x801310A8, Size: 0x74
 * ======================================================================= */
BOOL fn_801310A8(u32 effectId) {
    GSEffectInstance* inst;
    GSEffectGlobals* g;

    if (effectId == 0) {
        goto _null;
    }
    g = (GSEffectGlobals*)(void*)lbl_803635C0;
    if (effectId > g->maxEffects) {
        goto _null;
    }
    inst = (GSEffectInstance*)((u8*)g->instanceTable +
                               (effectId - 1) * sizeof(GSEffectInstance));
    switch (inst->state) {
    case GSEFFECT_STATE_UNINIT:
        goto _null;
    }
    goto _got_inst;
_null:
    inst = NULL;
_got_inst:
    if (inst == NULL) {
        goto _ret0;
    }
    if (inst->state == GSEFFECT_STATE_UNINIT) {
        goto _ret0;
    }
    if (inst->state == GSEFFECT_STATE_IDLE) {
        goto _ret0;
    }
    if (inst->state == GSEFFECT_STATE_STOPPING) {
        goto _ret0;
    }
    return TRUE;
_ret0:
    return FALSE;
}

/* =======================================================================
 * GSeffect / GSEffectTrigger
 * Address: 0x8013111C, Size: 0xE4
 * ======================================================================= */
BOOL GSeffect(u32 effectId) {
    GSEffectInstance* inst;
    GSEffectGlobals* g;
    GSEffectStartFunc trigFn;
    GSEffectStopFunc stopFn;

    inst = NULL;
    if (effectId == 0) {
        goto _null;
    }
    g = (GSEffectGlobals*)(void*)lbl_803635C0;
    if (effectId > g->maxEffects) {
        goto _null;
    }
    inst = (GSEffectInstance*)((u8*)g->instanceTable +
                               (effectId - 1) * sizeof(GSEffectInstance));
    switch (inst->state) {
    case GSEFFECT_STATE_UNINIT:
        goto _null;
    }
    goto _got_inst;
_null:
    inst = NULL;
_got_inst:
    if (inst == NULL) {
        goto _final0;
    }
    if (inst->state == GSEFFECT_STATE_IDLE) {
        goto _print_error;
    }
    trigFn = inst->triggerFunc;
    inst->state = GSEFFECT_STATE_ACTIVE;
    if (trigFn == NULL) {
        goto _final0;
    }
    if ((u32)((BOOL(*)(void*))trigFn)(inst->userData) != 0) {
        goto _ret1;
    }
    stopFn = inst->stopFunc;
    inst->state = GSEFFECT_STATE_STOPPING;
    if (stopFn != NULL) {
        stopFn(inst->userData);
    }
    return FALSE;
_ret1:
    return TRUE;
_print_error:
    GSlogWrite(lbl_80272A58);
_final0:
    return FALSE;
}

/* =======================================================================
 * fn_80131200 / GSEffectRegister
 * Address: 0x80131200, Size: 0x68
 * ======================================================================= */
void fn_80131200(u32 effectId, GSEffectStartFunc startFunc,
                 GSEffectStopFunc destroyFunc,
                 GSEffectStartFunc triggerFunc,
                 GSEffectStopFunc stopFunc,
                 void* extraParam,
                 GSEffectUpdateFunc updateFunc,
                 GSEffectRenderFunc renderFunc) {
    GSEffectInstance* inst;
    GSEffectGlobals* g;

    if (effectId == 0) {
        goto _null;
    }
    g = (GSEffectGlobals*)(void*)lbl_803635C0;
    if (effectId > g->maxEffects) {
        goto _null;
    }
    inst = (GSEffectInstance*)((u8*)g->instanceTable +
                               (effectId - 1) * sizeof(GSEffectInstance));
    switch (inst->state) {
    case GSEFFECT_STATE_UNINIT:
        goto _null;
    }
    goto _got_inst;
_null:
    inst = NULL;
_got_inst:
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
 * fn_80131268 / GSEffectFree
 * Address: 0x80131268, Size: 0x134
 * ======================================================================= */
void fn_80131268(u32 effectId) {
    GSEffectInstance* inst;
    GSEffectInstance* prevInst;
    GSEffectInstance* nextInst;
    GSEffectGlobals* g;

    inst = NULL;
    if (effectId == 0) {
        goto _null;
    }
    g = (GSEffectGlobals*)(void*)lbl_803635C0;
    if (effectId > g->maxEffects) {
        goto _null;
    }
    inst = (GSEffectInstance*)((u8*)g->instanceTable +
                               (effectId - 1) * sizeof(GSEffectInstance));
    switch (inst->state) {
    case GSEFFECT_STATE_UNINIT:
        goto _null;
    }
    goto _got_inst;
_null:
    inst = NULL;
_got_inst:
    if (inst == NULL) {
        return;
    }
    prevInst = inst->prev;
    nextInst = inst->next;

    if (inst->state != GSEFFECT_STATE_IDLE) {
        if (inst->state != GSEFFECT_STATE_STOPPING) {
            if (inst->stopFunc != NULL) {
                inst->stopFunc(inst->userData);
            }
        }
        if (inst->destroyFunc != NULL) {
            inst->destroyFunc(inst->userData);
        }
        inst->state = GSEFFECT_STATE_UNINIT;
    }

    if (prevInst != NULL) {
        prevInst->next = nextInst;
    } else {
        ((GSEffectGlobals*)(void*)lbl_803635C0)->activeListHead = nextInst;
    }

    if (nextInst != NULL) {
        nextInst->prev = prevInst;
    }

    inst->prev = NULL;
    {
        GSEffectGlobals* gf = (GSEffectGlobals*)(void*)lbl_803635C0;
        inst->next = gf->freeListHead;
        if (gf->freeListHead != NULL) {
            gf->freeListHead->prev = inst;
        }
        gf->freeListHead = inst;
    }

    fn_800E24B0(inst->memHandle);
    fn_800E209C(inst->memHandle);
}

/* =======================================================================
 * fn_8013139C / GSEffectResetState
 * Address: 0x8013139C, Size: 0x8C
 * ======================================================================= */
void fn_8013139C(u32 effectId, u32 param) {
    GSEffectInstance* inst;
    GSEffectGlobals* g;
    GSEffectStartFunc startFunc;

    inst = NULL;
    if (effectId == 0) {
        goto _null;
    }
    g = (GSEffectGlobals*)(void*)lbl_803635C0;
    if (effectId > g->maxEffects) {
        goto _null;
    }
    inst = (GSEffectInstance*)((u8*)g->instanceTable +
                               (effectId - 1) * sizeof(GSEffectInstance));
    switch (inst->state) {
    case GSEFFECT_STATE_UNINIT:
        goto _null;
    }
    goto _got_inst;
_null:
    inst = NULL;
_got_inst:
    if (inst == NULL) {
        return;
    }
    startFunc = inst->startFunc;
    if (startFunc != NULL) {
        startFunc(inst->userData, param);
    }
    inst->state = GSEFFECT_STATE_STOPPING;
}

/* =======================================================================
 * fn_80131428 / GSEffectAllocSlot
 * Address: 0x80131428, Size: 0xF4
 * ======================================================================= */
u16 fn_80131428(void* callbacks, u16 dataSize) {
    GSEffectGlobals* globals;
    void* cbArg;
    u16 cbSize;
    GSEffectInstance* slot;
    u16 savedId;
    void* userData;
    u16 memHandle;
    GSEffectGlobals* ga;
    extern u16 _toolentryAlloc__FUl(u32 size);
    extern void* fn_800E27B0(u16 handle);

    globals = (GSEffectGlobals*)(void*)lbl_803635C0;
    cbArg = callbacks;
    cbSize = dataSize;
    slot = globals->freeListHead;
    if (slot == NULL) {
        goto _retNull;
    }

    savedId = slot->id;
    memHandle = _toolentryAlloc__FUl((u32)cbSize);
    if (memHandle == 0) {
        return 0;
    }

    userData = fn_800E27B0(memHandle);

    globals->freeListHead = slot->next;
    if (slot->next != NULL) {
        slot->next->prev = NULL;
    }

    memset(slot, 0, sizeof(GSEffectInstance));
    slot->id = savedId;

    ga = (GSEffectGlobals*)(void*)lbl_803635C0;
    if (ga->activeListHead != NULL) {
        ga->activeListHead->prev = slot;
    }
    slot->next = ga->activeListHead;
    ga->activeListHead = slot;
    slot->prev = NULL;
    slot->memHandle = memHandle;
    slot->userData = userData;

    memcpy(userData, cbArg, (u32)cbSize);
    slot->dataSize = dataSize;
    slot->state = GSEFFECT_STATE_IDLE;

    return slot->id;
_retNull:
    return 0;
}
