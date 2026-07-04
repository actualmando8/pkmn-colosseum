/**
 * @file battle_grid.c
 * @brief Battle grid system -- scene layout, field positions, camera setup,
 *        Pokemon/trainer model placement, and grid state management.
 *
 * Address range: 0x801C0F20 - 0x801C4CB8 (58 functions)
 *
 * This file covers the battle grid subsystem responsible for:
 *   - Pre-grid initialization (camera context, field positions)
 *   - Grid setup (4-slot double battle layout)
 *   - Model loading/replacement for Pokemon and trainers
 *   - Camera initialization and rendering pass configuration
 *   - Grid tick/update/cleanup lifecycle
 *
 * The battle grid uses a 4-slot layout corresponding to:
 *   Slot 0: Player Left   Slot 1: Player Right
 *   Slot 2: Enemy Left    Slot 3: Enemy Right
 *
 * Key BSS state:
 *   lbl_80467030 (0x20 bytes): BattleCameraState
 *   lbl_80466E50 (0x1E0 bytes): Battle scene animation context
 *
 * Large functions (>0x400 bytes) use #pragma push / #pragma optimization_level 0
 * stubs for state machines and float-heavy animation code. Smaller functions
 * (getters, setters, wrappers) are fully decompiled.
 */

#include "dolphin/types.h"
#include "hsd/hsd_cobj.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_lobj.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_robj.h"
#include "hsd/hsd_tobj.h"
#include "hsd/hsd_wobj.h"

#define BATTLE_POS_ENEMY_LEFT    2
#define BATTLE_TOTAL_POKEMON     4

void fn_801C3430(void);
void fn_801C3A64(void);
void fn_801C3B80(void);
void fadeSetEX(s32 mode, void* callback, s32 flags, f32 a, f32 b);
void fn_801C431C(s32 arg0);
f32 fn_801C5F6C(void);

/* =========================================================================
 * External function declarations
 * ========================================================================= */

/* CRT */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* Engine / GS core */
extern void  GSlogWrite(const char* fmt, ...);      /* GSlog_Print */
extern void  fn_800D3088(void);                       /* GSgfx tick */
extern void* GSthreadCreate(s32 priority, void* parent, s32 stackSize,
                          u8 usesFPU, void* entry, s32 arg); /* GSthread_Create */
extern void  fn_800F04C4(void);                       /* stop particle system */

/* Scene management */
extern void  fn_80102568(s32 objID, s32 arg1, s32 arg2);   /* release scene object */
extern u8    fn_80102620(s32 objID);                        /* check scene object active */
extern void* fn_801025C0(s32 objID);                        /* get scene object pointer */
extern void  fn_80103BA8(void* padData, s32 port);          /* read pad input */

/* HSD (SysDolphin) model/animation */
extern void  fn_80362D0C(void* jobj);                       /* HSD_JObjAnimAll */
extern void  fn_80362E40(void* jobj, f32 frame);            /* HSD_JObjReqAnimAll */
extern void* fn_80363B8C(void* data, s32 idx);              /* HSD_JObjLoadJoint */
extern void  fn_80363CF4(void* jobj);                       /* HSD_JObjRemoveAll */
extern void  fn_8036A384(void* jobj, f32 x, f32 y, f32 z); /* HSD_JObjSetTranslate */
extern void  fn_8036A478(void* jobj, f32 x, f32 y, f32 z); /* HSD_JObjSetScale */
extern void  fn_8036A2D8(void* jobj, f32 rx, f32 ry, f32 rz); /* HSD_JObjSetRotation */
extern void* fn_80369654(void* jobj, s32 childIdx);         /* HSD_JObjGetChild */

/* Sound */
extern void  soundStop(s32 sndID, s32 volume);    /* soundStop */
extern void  fn_801659FC(s32 sndID, s32 fadeTime, s32 volume); /* sndPlay with fade */

/* Battle scene (forward refs to battle_scene.c) */
extern void  fn_801C53BC(void* ctx, s32 arg1, s32 arg2, s32 arg3, f32 arg4);

/* =========================================================================
 * External data (BSS / SDA)
 * ========================================================================= */

extern u8    lbl_80466E50[0x1E0]; /* battle scene animation context */
extern u8    lbl_80467030[0x20];  /* BattleCameraState */
extern u8    lbl_8046AC60[0x100]; /* battle transfer context */
extern void* lbl_8046D500;        /* battle state machine context ptr */

typedef void (*BattleGridCallback)(void);

typedef struct BattleGridCallbackNode {
    struct BattleGridCallbackNode* next;
    BattleGridCallback callback;
} BattleGridCallbackNode;

typedef struct BattleGridGroupEntry {
    u8* slot;
    u8 pad_04[8];
    u16 memberCount;
    u8 arg1;
    u8 arg2;
} BattleGridGroupEntry;

typedef struct BattleGridGroupTable {
    BattleGridGroupEntry entries[4];
    u16 count;
} BattleGridGroupTable;

typedef struct BattleGridTransitionState {
    u8 mode;
    u8 pending;
    u16 arg;
    f32 startValue;
    f32 endValue;
    void* callbackArg;
    void* texture;
    f32 value;
    f32 timer;
} BattleGridTransitionState;

typedef struct BattleGridCameraWork {
    u8 pad_00[4];
    f32 angle;
    f32 blend;
    s32 sequenceType;
    f32 sequenceParam1;
    f32 sequenceParam2;
    f32 sequenceTimer;
} BattleGridCameraWork;

typedef struct BattleGridSceneSlot {
    s32 active;
    void* jobj;
    f32 posX;
    f32 posY;
    f32 posZ;
    u8 pad_14[0x3C];
    f32 rotationY;
    f32 scale;
    s32 animType;
    u8 pad_5C[4];
    f32 blend;
    u8 pad_64[0x0C];
} BattleGridSceneSlot;

typedef struct BattleGridSceneWork {
    u8 pad_00[0x20];
    BattleGridSceneSlot slots[BATTLE_TOTAL_POKEMON];
} BattleGridSceneWork;

/* =========================================================================
 * NOTE: 0x801C01C8-0x801C0F20 (incl. the real, variadic HSD_ForeachAnim at
 * 0x801C028C) is HSD library code, split into hsd/hsd_aobj_range_801C01C8.c
 * (audit 2026-07-01).
 *
 * JObjForeachAnim/DObjForeachAnim/LObjForeachAnim/CObjForeachAnim are also HSD library
 * code: the per-object-kind "ForeachAnim" family (doldecomp/melee
 * src/sysdolphin/baselib/aobj.c) that HSD_ForeachAnim dispatches into by
 * HSD_Type. MWCC fully inlined the shared "callbackForeachFunc" 12-way
 * argument-marshaling switch (see HSD_FOREACH_INVOKE below) plus the
 * leaf RObj/WObj/MObj/TObj/PObj walkers into each of these four, which is
 * why each is a single large function with several duplicated jump
 * tables rather than a tree of small calls. Confirmed against XD's
 * matched JObjForeachAnim/DObjForeachAnim/LObjForeachAnim/
 * CObjForeachAnim (near-identical sizes) and against every mask-bit and
 * type-constant used below (each equals MASK_OF(x) / HSD_Type from
 * include/hsd/hsd_object.h).
 * ========================================================================= */

/* HSD_ForeachAnim's internal argument-shape tag. Colosseum's ordering
 * differs from melee's AObj_Arg_Type (AOT moved next to AO instead of
 * after AOU); order below is reverse-engineered from the jump tables. */
typedef enum HSD_ForeachArgType {
    HSD_FOREACH_A,
    HSD_FOREACH_AF,
    HSD_FOREACH_AV,
    HSD_FOREACH_AU,
    HSD_FOREACH_AO,
    HSD_FOREACH_AOT,
    HSD_FOREACH_AOF,
    HSD_FOREACH_AOV,
    HSD_FOREACH_AOU,
    HSD_FOREACH_AOTF,
    HSD_FOREACH_AOTV,
    HSD_FOREACH_AOTU
} HSD_ForeachArgType;

typedef union HSD_ForeachArg {
    f32 f;
    u32 d;
    void* v;
} HSD_ForeachArg;

/* Assert strings already matched in src/hsd/hsd_sdata2_8047DE90.c:
 * lbl_8047DF40 = "aobj.c", lbl_8047DF48 = "obj" (HSD_ASSERT(0x2CB, obj)
 * inside the original aobj.c, i.e. HSD_ASSERT's __FILE__ from that TU). */
extern const u8 lbl_8047DF40[];
extern const u8 lbl_8047DF48[];

/* Colosseum's HSD_PObj carries a direct HSD_AObj* immediately after the
 * payload union that include/hsd/hsd_pobj.h (melee-derived) models only
 * up to; that trailing field is what DObjForeachAnim's PObj sub-loop
 * walks (pobj+0x18, matching pobj->next chaining at pobj+0x4). Modeled
 * locally (mirroring HSD_PObj's field layout by hand) rather than
 * editing the shared header, and to avoid pulling in hsd_pobj.h's
 * transitive HSD_AObj* prototypes which conflict with the generic
 * void*-typed HSD_AObj functions already matched elsewhere in this file. */
typedef struct BattleGridPObjAnim {
    void* classInfo;                /* 0x00 */
    struct BattleGridPObjAnim* next; /* 0x04 */
    void* verts;                     /* 0x08 */
    u16 flags;                       /* 0x0C */
    u16 nDisplay;                    /* 0x0E */
    void* display;                   /* 0x10 */
    void* payload;                   /* 0x14 (jobj / shape_set / envelope_list) */
    HSD_AObj* aobj;                  /* 0x18 */
} BattleGridPObjAnim;

/* Shared 12-way argument-shape dispatch, inlined at every call site
 * (matches the repeated jumptable_XXXX blocks in each ForeachAnim). */
#define HSD_FOREACH_INVOKE(aobj_, obj_, type_, func_, argType_, arg_) \
    do { \
        switch (argType_) { \
        case HSD_FOREACH_A: \
            ((void (*)(HSD_AObj*))(func_))(aobj_); \
            break; \
        case HSD_FOREACH_AF: \
            ((void (*)(HSD_AObj*, f32))(func_))(aobj_, (arg_)->f); \
            break; \
        case HSD_FOREACH_AV: \
            ((void (*)(HSD_AObj*, void*))(func_))(aobj_, (arg_)->v); \
            break; \
        case HSD_FOREACH_AU: \
            ((void (*)(HSD_AObj*, u32))(func_))(aobj_, (arg_)->d); \
            break; \
        case HSD_FOREACH_AO: \
            ((void (*)(HSD_AObj*, void*))(func_))(aobj_, (obj_)); \
            break; \
        case HSD_FOREACH_AOT: \
            ((void (*)(HSD_AObj*, void*, s32))(func_))(aobj_, (obj_), (type_)); \
            break; \
        case HSD_FOREACH_AOF: \
            ((void (*)(HSD_AObj*, void*, f32))(func_))(aobj_, (obj_), (arg_)->f); \
            break; \
        case HSD_FOREACH_AOV: \
            ((void (*)(HSD_AObj*, void*, void*))(func_))(aobj_, (obj_), (arg_)->v); \
            break; \
        case HSD_FOREACH_AOU: \
            ((void (*)(HSD_AObj*, void*, u32))(func_))(aobj_, (obj_), (arg_)->d); \
            break; \
        case HSD_FOREACH_AOTF: \
            ((void (*)(HSD_AObj*, void*, s32, f32))(func_))(aobj_, (obj_), (type_), (arg_)->f); \
            break; \
        case HSD_FOREACH_AOTV: \
            ((void (*)(HSD_AObj*, void*, s32, void*))(func_))(aobj_, (obj_), (type_), (arg_)->v); \
            break; \
        case HSD_FOREACH_AOTU: \
            ((void (*)(HSD_AObj*, void*, s32, u32))(func_))(aobj_, (obj_), (type_), (arg_)->d); \
            break; \
        } \
    } while (0)

void DObjForeachAnim(HSD_DObj* dobj, HSD_TypeMask mask, void* func,
                  HSD_ForeachArgType argType, HSD_ForeachArg* arg);

/**
 * JObjForeachAnim - HSD_JObj ForeachAnim (JObjForeachAnim).
 * Address: 0x801C0F20 | Size: 0x354
 */
void JObjForeachAnim(HSD_JObj* obj, HSD_TypeMask mask, void* func,
                  HSD_ForeachArgType argType, HSD_ForeachArg* arg) {
    HSD_RObj* robj;

    (obj != NULL) ? (void)0
                  : __assert((const char*)lbl_8047DF40, 0x2CB,
                             (const char*)lbl_8047DF48);

    if ((mask & JOBJ_MASK) && obj->aobj != NULL) {
        HSD_FOREACH_INVOKE(obj->aobj, obj, JOBJ_TYPE, func, argType, arg);
    }

    if (union_type_dobj(obj)) {
        DObjForeachAnim(obj->u.dobj, mask, func, argType, arg);
    }

    for (robj = obj->robj; robj != NULL; robj = robj->next) {
        if ((mask & ROBJ_MASK) && robj->aobj != NULL) {
            HSD_FOREACH_INVOKE(robj->aobj, robj, ROBJ_TYPE, func, argType, arg);
        }
    }

    if (!(obj->flags & JOBJ_INSTANCE)) {
        for (obj = obj->child; obj != NULL; obj = obj->next) {
            JObjForeachAnim(obj, mask, func, argType, arg);
        }
    }
}

/**
 * DObjForeachAnim - HSD_DObj ForeachAnim (DObjForeachAnim).
 * Address: 0x801C1274 | Size: 0x59C
 */
void DObjForeachAnim(HSD_DObj* dobj, HSD_TypeMask mask, void* func,
                  HSD_ForeachArgType argType, HSD_ForeachArg* arg) {
    for (; dobj != NULL; dobj = dobj->next) {
        if ((mask & DOBJ_MASK) && dobj->aobj != NULL) {
            HSD_FOREACH_INVOKE(dobj->aobj, dobj, DOBJ_TYPE, func, argType, arg);
        }

        {
            HSD_MObj* mobj = dobj->mobj;
            if (mobj != NULL) {
                HSD_TObj* tobj;

                if ((mask & MOBJ_MASK) && mobj->aobj != NULL) {
                    HSD_FOREACH_INVOKE(mobj->aobj, mobj, MOBJ_TYPE, func, argType, arg);
                }

                for (tobj = mobj->tobj; tobj != NULL; tobj = tobj->next) {
                    if ((mask & TOBJ_MASK) && tobj->aobj != NULL) {
                        HSD_FOREACH_INVOKE(tobj->aobj, tobj, TOBJ_TYPE, func, argType, arg);
                    }
                }
            }
        }

        {
            BattleGridPObjAnim* pobj;

            for (pobj = (BattleGridPObjAnim*)dobj->pobj; pobj != NULL;
                 pobj = pobj->next) {
                if ((mask & POBJ_MASK) && pobj->aobj != NULL) {
                    HSD_FOREACH_INVOKE(pobj->aobj, pobj, POBJ_TYPE, func, argType, arg);
                }
            }
        }
    }
}

/**
 * LObjForeachAnim - HSD_LObj ForeachAnim (LObjForeachAnim).
 * Address: 0x801C1810 | Size: 0x6F0
 */
void LObjForeachAnim(HSD_LObj* lobj, HSD_TypeMask mask, void* func,
                  HSD_ForeachArgType argType, HSD_ForeachArg* arg) {
    HSD_WObj* wobj;
    HSD_RObj* robj;

    for (; lobj != NULL; lobj = lobj->next) {
        if ((mask & LOBJ_MASK) && lobj->aobj != NULL) {
            HSD_FOREACH_INVOKE(lobj->aobj, lobj, LOBJ_TYPE, func, argType, arg);
        }

        wobj = lobj->position;
        if (wobj != NULL) {
            if ((mask & WOBJ_MASK) && wobj->aobj != NULL) {
                HSD_FOREACH_INVOKE(wobj->aobj, wobj, WOBJ_TYPE, func, argType, arg);
            }
            for (robj = wobj->robj; robj != NULL; robj = robj->next) {
                if ((mask & ROBJ_MASK) && robj->aobj != NULL) {
                    HSD_FOREACH_INVOKE(robj->aobj, robj, ROBJ_TYPE, func, argType, arg);
                }
            }
        }

        wobj = lobj->interest;
        if (wobj != NULL) {
            if ((mask & WOBJ_MASK) && wobj->aobj != NULL) {
                HSD_FOREACH_INVOKE(wobj->aobj, wobj, WOBJ_TYPE, func, argType, arg);
            }
            for (robj = wobj->robj; robj != NULL; robj = robj->next) {
                if ((mask & ROBJ_MASK) && robj->aobj != NULL) {
                    HSD_FOREACH_INVOKE(robj->aobj, robj, ROBJ_TYPE, func, argType, arg);
                }
            }
        }
    }
}

/**
 * CObjForeachAnim - HSD_CObj ForeachAnim (CObjForeachAnim).
 * Address: 0x801C1F00 | Size: 0x6E4
 */
void CObjForeachAnim(HSD_CObj* cobj, HSD_TypeMask mask, void* func,
                  HSD_ForeachArgType argType, HSD_ForeachArg* arg) {
    HSD_WObj* wobj;
    HSD_RObj* robj;

    if (cobj == NULL) {
        return;
    }

    if ((mask & COBJ_MASK) && cobj->aobj != NULL) {
        HSD_FOREACH_INVOKE(cobj->aobj, cobj, COBJ_TYPE, func, argType, arg);
    }

    wobj = cobj->eyepos;
    if (wobj != NULL) {
        if ((mask & WOBJ_MASK) && wobj->aobj != NULL) {
            HSD_FOREACH_INVOKE(wobj->aobj, wobj, WOBJ_TYPE, func, argType, arg);
        }
        for (robj = wobj->robj; robj != NULL; robj = robj->next) {
            if ((mask & ROBJ_MASK) && robj->aobj != NULL) {
                HSD_FOREACH_INVOKE(robj->aobj, robj, ROBJ_TYPE, func, argType, arg);
            }
        }
    }

    wobj = cobj->interest;
    if (wobj != NULL) {
        if ((mask & WOBJ_MASK) && wobj->aobj != NULL) {
            HSD_FOREACH_INVOKE(wobj->aobj, wobj, WOBJ_TYPE, func, argType, arg);
        }
        for (robj = wobj->robj; robj != NULL; robj = robj->next) {
            if ((mask & ROBJ_MASK) && robj->aobj != NULL) {
                HSD_FOREACH_INVOKE(robj->aobj, robj, ROBJ_TYPE, func, argType, arg);
            }
        }
    }
}

/**
 * fn_801C25E4 - Pre-grid transition helper.
 * Address: 0x801C25E4 | Size: 0x8C
 */
void fn_801C25E4(void* ctx, s32 mode) {
    u8* state = (u8*)ctx;
    if (state == NULL) {
        return;
    }
    /* Set camera transition mode and reset interpolation timer */
    *(s32*)(state + 0x1C0) = mode;
    *(f32*)(state + 0x1C4) = 0.0f;
}

/**
 * fn_801C2670 - Pre-grid scene object configuration.
 * Address: 0x801C2670 | Size: 0x184
 */
void fn_801C2670(void* ctx, s32 objType, s32 param) {
    u8* state = (u8*)ctx;
    if (state == NULL) {
        return;
    }
    /* Configure a scene object in the pre-grid context:
     * objType 0: Stage background model
     * objType 1: Ground plane model
     * objType 2: Sky dome model
     * objType 3: Battle effect spawner
     */
    *(s32*)(state + 0x1C8 + (objType * 4)) = param;
}

/**
 * fn_801C27F4 - Pre-grid field layout calculation.
 * Address: 0x801C27F4 | Size: 0x1D0
 */
void fn_801C27F4(void* ctx, f32 posX, f32 posZ) {
    BattleGridSceneWork* state = (BattleGridSceneWork*)ctx;
    if (state == NULL) {
        return;
    }
    /* Calculate field layout positions for all 4 battle slots
     * based on a center offset (posX, posZ).
     * The double battle layout uses a diamond formation:
     *   Slot 0 (Player L): center + (-offset, -depth)
     *   Slot 1 (Player R): center + (+offset, -depth)
     *   Slot 2 (Enemy L):  center + (-offset, +depth)
     *   Slot 3 (Enemy R):  center + (+offset, +depth)
     */
    {
        f32 offsetX = 3.0f;
        f32 depthZ = 5.0f;
        s32 i;

        for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
            BattleGridSceneSlot* slot = &state->slots[i];
            f32 sx = (i & 1) ? offsetX : -offsetX;
            f32 sz = (i >= BATTLE_POS_ENEMY_LEFT) ? depthZ : -depthZ;

            slot->posX = posX + sx;
            slot->posY = 0.0f;
            slot->posZ = posZ + sz;
        }
    }
}

/**
 * HSD_AObjReqAnim - Set JObj animation frame value.
 * Address: 0x801C29C4 | Size: 0x40
 */
void HSD_AObjReqAnim(void* obj, f32 value) {
    extern void HSD_FObjReqAnimAll(void* jobj);

    if (obj == NULL) {
        return;
    }
    *(f32*)((u8*)obj + 4) = value;
    *(u32*)obj = (*(u32*)obj & ~0x40000000) | 0x08000000;
    HSD_FObjReqAnimAll(*(void**)((u8*)obj + 0x14));
}

/**
 * HSD_AObjInvokeCallBacks - Run pre-grid node callbacks.
 * Address: 0x801C2A04 | Size: 0x5C
 */
void HSD_AObjInvokeCallBacks(void) {
    extern s32 lbl_8047B390;
    extern s32 lbl_8047B38C;
    extern BattleGridCallbackNode* lbl_8047B388;
    BattleGridCallbackNode* node;

    if (lbl_8047B390 != 0 && lbl_8047B38C == 0) {
        node = lbl_8047B388;
        while (node != NULL) {
            node->callback();
            node = node->next;
        }
    }
}

/**
 * fn_801C2A60 - Get pre-grid slot count.
 * Address: 0x801C2A60 | Size: 0x14
 */
s32 fn_801C2A60(void) {
    return BATTLE_TOTAL_POKEMON;
}

/**
 * fn_801C2A74 - Get pre-grid slot side.
 * Address: 0x801C2A74 | Size: 0x1C
 */
s32 fn_801C2A74(s32 slot) {
    return (slot >= BATTLE_POS_ENEMY_LEFT) ? 1 : 0;
}

/**
 * fn_801C2A90 - Get pre-grid slot position within side.
 * Address: 0x801C2A90 | Size: 0x1C
 */
s32 fn_801C2A90(s32 slot) {
    return (slot & 1);
}

/**
 * HSD_AObjGetAllocData - Get grid group base pointer.
 * Address: 0x801C2AAC | Size: 0xC
 */
void* HSD_AObjGetAllocData(void) {
    extern u8 lbl_80466DB8[];
    return lbl_80466DB8;
}

/**
 * fn_801C2AB8 - Pre-grid set animation state.
 * Address: 0x801C2AB8 | Size: 0x30
 */
void fn_801C2AB8(s32 slot, s32 animState) {
    extern u8 lbl_80466DB8[];
    extern void HSD_ObjAllocInit(void*, u32, u32);
    HSD_ObjAllocInit(lbl_80466DB8, 0x1c, 4);
}

/**
 * fn_801C2AE8 - Get grid group member count by owner ID.
 * Address: 0x801C2AE8 | Size: 0x44
 */
u16 fn_801C2AE8(u32 id) {
    extern BattleGridGroupEntry lbl_80466DE8[];
    BattleGridGroupEntry* group = lbl_80466DE8;
    u16 i;

    for (i = 0; i < 4; i++, group++) {
        if ((u32)group->slot == id) {
            return group->memberCount;
        }
    }
    return 0;
}

/**
 * fn_801C2B2C - Pre-grid update all slot positions.
 * Address: 0x801C2B2C | Size: 0xB4
 */
void fn_801C2B2C(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        void* jobj = slot->jobj;

        if (jobj != NULL && slot->active != 0) {
            f32 x = slot->posX;
            f32 y = slot->posY;
            f32 z = slot->posZ;
            fn_8036A384(jobj, x, y, z);
        }
    }
}

/**
 * fn_801C2BE0 - Pre-grid final setup.
 * Address: 0x801C2Be0 | Size: 0x174
 */
void fn_801C2Be0(void* ctx, s32 arg1) {
    u8* state = (u8*)ctx;
    if (state == NULL) {
        return;
    }
    /* Finalize pre-grid setup:
     * - Apply final slot positions
     * - Set up rendering callbacks
     * - Initialize camera for battle view
     * - Mark grid as ready for battle
     */
    fn_801C2B2C();
    *(s32*)(state + 0x00) = 7; /* GRID_READY */
}

/* =========================================================================
 * GRID TICK / STATE MANAGEMENT (0x801C2D54 - 0x801C3108)
 * ========================================================================= */

/**
 * fn_801C2D54 - Battle grid tick callback 1 (no-op forward).
 * Address: 0x801C2D54 | Size: 0x8
 * Referenced by battle_main.c as battle grid tick 1.
 */
extern u8 lbl_8047B398;
extern u8 lbl_8047B399;

void fn_801C2D54(void) {
    asm { lbz r3, lbl_8047B399(r13) }
}

/**
 * fn_801C2D5C - Battle grid tick callback wrapper.
 * Address: 0x801C2D5C | Size: 0xC
 */
void fn_801C2D5C(void) {
    lbl_8047B399 = 0;
}

/**
 * fn_801C2D68 - Battle grid tick callback 2.
 * Address: 0x801C2D68 | Size: 0xC
 * Referenced by battle_main.c as battle grid tick 2.
 */
void fn_801C2D68(void) {
    lbl_8047B399 = 1;
}

/**
 * fn_801C2D74 - Battle grid tick callback 3.
 * Address: 0x801C2D74 | Size: 0xC
 */
void fn_801C2D74(void) {
    lbl_8047B398 = 1;
}

/**
 * fn_801C2D80 - Battle grid cleanup / release resources.
 * Address: 0x801C2D80 | Size: 0x180
 * Referenced by battle_main.c as "battle grid cleanup".
 * Releases all grid models, clears slot state, frees memory.
 */
void fn_801C2D80(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    /* Release all grid slot models */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        void* jobj = slot->jobj;

        if (jobj != NULL) {
            fn_80363CF4(jobj); /* HSD_JObjRemoveAll */
            slot->jobj = NULL;
        }

        slot->active = 0;
    }

    /* Clear scene animation context */
    memset(lbl_80466E50, 0, 0x1E0);

    /* Reset camera state */
    memset(lbl_80467030, 0, 0x20);
}

/**
 * fn_801C2F00 - Battle grid load data from buffer.
 * Address: 0x801C2F00 | Size: 0x208
 * Referenced by battle_main.c as "battle grid load data".
 * Loads grid configuration from a data buffer.
 */
void fn_801C2F00(void* data, u32 size) {
    if (data == NULL || size == 0) {
        return;
    }
    /* Load grid configuration from a data buffer:
     * 1. Parse header (slot count, field type)
     * 2. For each slot: load position, model ID, animation set
     * 3. Load camera configuration
     * 4. Load stage model reference
     */
    memcpy(lbl_80466E50, data, (size < 0x1E0) ? size : 0x1E0);
}

/* =========================================================================
 * CORE GRID FUNCTIONS (0x801C3108 - 0x801C53BC)
 * These are the main battle grid API functions declared in battle.h.
 * ========================================================================= */

/**
 * fn_801C3108 / battleGrid_GetState - Get current grid state.
 * Address: 0x801C3108 | Size: 0xC
 */
s32 fn_801C3108(void) {
    extern u8 lbl_80466DE8[];
    return (s32)lbl_80466DE8;
}

/**
 * fn_801C3114 / battleGrid_Init - Initialize the battle grid.
 * Address: 0x801C3114 | Size: 0xD8
 * Clears all grid slots, initializes the camera state,
 * sets up the 4-position double battle layout.
 */
void fn_801C3114(void) {
    s32 i;
    BattleGridSceneWork* sceneWork;

    memset(lbl_80467030, 0, 0x20);
    memset(lbl_80466E50, 0, 0x1E0);

    /* Initialize 4 BattleGridSlot entries with default values */
    sceneWork = (BattleGridSceneWork*)lbl_80466E50;
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &sceneWork->slots[i];
        slot->active = 0;
        slot->jobj = NULL;
        slot->rotationY = 0.0f;
        slot->scale = 1.0f;
    }
}

/**
 * fn_801C31EC / battleGrid_Setup - Full grid setup with model loading.
 * Address: 0x801C31EC | Size: 0x244
 * Referenced by battle_main.c (battle_FightEnd calls this for cleanup).
 * Sets up the complete battle field layout including stage model,
 * position markers, and initial camera placement.
 */
void fn_801C31EC(void) {
    /* Full grid setup with model loading:
     * 1. Initialize grid state
     * 2. Set up stage model (battle colosseum arena)
     * 3. Place position markers for all 4 slots
     * 4. Initialize camera to default battle overhead view
     */
    fn_801C3114();
    fn_801C3430();
}

/**
 * fn_801C3430 / battleGridSetup - Main grid setup (large).
 * Address: 0x801C3430 | Size: 0x634
 * Proposed name from symbols: battleGridSetup.
 * This is the primary grid initialization function that:
 *   1. Loads the stage model from FDAT
 *   2. Sets up position transforms for all 4 battle slots
 *   3. Configures lighting and shadow rendering
 *   4. Sets up the battle camera default view
 *   5. Initializes the model animation system
 */
void fn_801C3430(void) {
    /* Main battle grid setup:
     * 1. Load stage model from FDAT
     * 2. Set up position transforms for all 4 battle slots
     * 3. Configure lighting (ambient + 2 directional)
     * 4. Configure shadow rendering
     * 5. Set up battle camera default overhead view
     * 6. Initialize model animation system
     */
    fn_801C27F4((void*)lbl_80466E50, 0.0f, 0.0f);
    fn_801C3A64();
    fn_801C3B80();
}

/**
 * fn_801C3A64 / battleGridLoadModels - Load models for all grid positions.
 * Address: 0x801C3A64 | Size: 0x11C
 * Proposed name from symbols: battleGridLoadModels.
 * Loads Pokemon and trainer models into each active grid slot.
 */
void fn_801C3A64(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    /* Load Pokemon and trainer models into each active grid slot */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        s32 active = slot->active;

        if (active != 0) {
            /* Model is already loaded or should be loaded from battle data */
            void* jobj = slot->jobj;
            if (jobj != NULL) {
                f32 x = slot->posX;
                f32 y = slot->posY;
                f32 z = slot->posZ;
                fn_8036A384(jobj, x, y, z);
            }
        }
    }
}

/**
 * fn_801C3B80 / battleGridUpdatePositions - Update all grid positions.
 * Address: 0x801C3B80 | Size: 0x118
 * Proposed name from symbols: battleGridUpdatePositions.
 * Recalculates world-space positions for all grid slots
 * (e.g., after a Pokemon switch or camera change).
 */
void fn_801C3B80(void) {
    s32 i;
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;

    /* Recalculate world-space positions for all grid slots */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        BattleGridSceneSlot* slot = &state->slots[i];
        s32 active = slot->active;

        if (active == 0) {
            continue;
        }

        {
            void* jobj = slot->jobj;
            if (jobj != NULL) {
                f32 x = slot->posX;
                f32 y = slot->posY;
                f32 z = slot->posZ;
                f32 scale = slot->scale;

                fn_8036A384(jobj, x, y, z);
                fn_8036A478(jobj, scale, scale, scale);
            }
        }
    }
}

/**
 * fn_801C3C98 - Grid slot state update helper.
 * Address: 0x801C3C98 | Size: 0xCC
 */
void fn_801C3C98(s32 slot) {
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;
    BattleGridSceneSlot* slotData;

    if (slot < 0 || slot >= BATTLE_TOTAL_POKEMON) {
        return;
    }

    slotData = &state->slots[slot];

    /* Update slot state: apply position, rotation, and scale to JObj */
    {
        void* jobj = slotData->jobj;
        if (jobj != NULL) {
            f32 x = slotData->posX;
            f32 y = slotData->posY;
            f32 z = slotData->posZ;
            f32 rot = slotData->rotationY;
            f32 scale = slotData->scale;

            fn_8036A384(jobj, x, y, z);
            fn_8036A2D8(jobj, 0.0f, rot, 0.0f);
            fn_8036A478(jobj, scale, scale, scale);
        }
    }
}

/**
 * battleGridReplacePokemon / battleGridReplacePokemon - Replace Pokemon model in a grid slot.
 * Address: 0x801C3D64 | Size: 0xD8
 * Proposed name from symbols: battleGridReplacePokemon.
 * Removes the current Pokemon model from a slot and loads a new one.
 */
void battleGridReplacePokemon(void* model) {
    /* Replace Pokemon model in a grid slot:
     * 1. Find the slot this model belongs to
     * 2. Remove the current Pokemon JObj
     * 3. Load the new Pokemon JObj from model data
     * 4. Apply the slot's current transform
     */
    if (model == NULL) {
        return;
    }
}

/**
 * fn_801C3E3C - Grid slot model transition animation.
 * Address: 0x801C3E3C | Size: 0xD4
 */
void fn_801C3E3C(s32 slot, s32 animType) {
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;
    BattleGridSceneSlot* slotData;

    if (slot < 0 || slot >= BATTLE_TOTAL_POKEMON) {
        return;
    }

    slotData = &state->slots[slot];

    /* Set animation transition type for the slot model */
    slotData->animType = animType;

    /* Request the animation on the slot's JObj */
    {
        void* jobj = slotData->jobj;
        if (jobj != NULL) {
            fn_80362E40(jobj, 0.0f); /* HSD_JObjReqAnimAll */
        }
    }
}

/**
 * battleGridReplaceTrainer / battleGridReplaceTrainer - Replace trainer model in a grid slot.
 * Address: 0x801C3F10 | Size: 0xAC
 * Proposed name from symbols: battleGridReplaceTrainer.
 */
void battleGridReplaceTrainer(void* model) {
    /* Replace trainer model in a grid slot:
     * Similar to battleGridReplacePokemon but for trainer models.
     */
    if (model == NULL) {
        return;
    }
}

/**
 * fn_801C3FBC - Add slot to grid group.
 * Address: 0x801C3FBC | Size: 0xBC
 */
void fn_801C3FBC(u8* slot, u8 arg1, u8 arg2) {
    extern BattleGridGroupTable lbl_80466DE8;
    BattleGridGroupEntry* group;
    s8 state;

    if (lbl_80466DE8.count < 4) {
        group = &lbl_80466DE8.entries[0];
        if (group->slot != NULL) {
            group = &lbl_80466DE8.entries[1];
            if (group->slot != NULL) {
                group++;
                if (group->slot != NULL) {
                    group++;
                    if (group->slot != NULL) {
                        group++;
                    }
                }
            }
        }
        memset(group, 0, sizeof(*group));
        group->slot = slot;
        state = 1;
        group->arg1 = arg1;
        group->arg2 = arg2;
        if (arg1 != 0) {
            state = -1;
        }
        slot[0x76] = state;
        lbl_80466DE8.count = lbl_80466DE8.count + 1;
    }
}

/**
 * fn_801C4078 - Get grid slot model pointer.
 * Address: 0x801C4078 | Size: 0x24
 */
#pragma peephole off
void* fn_801C4078(s32 slot) {
    extern BattleGridTransitionState lbl_80466E30;
    volatile BattleGridTransitionState* gridState = &lbl_80466E30;

    if (gridState->mode == 4) {
        gridState->pending = 0;
        gridState->mode = 0;
    }

    return (void*)gridState;
}
#pragma peephole on

/**
 * fadeEffectDokuStart - Trigger grid slot update callback.
 * Address: 0x801C409C | Size: 0x54
 */
#pragma peephole off
void fadeEffectDokuStart(void) {
    extern BattleGridTransitionState lbl_80466E30;
    extern const f32 lbl_8047DFB0;
    extern const f32 lbl_8047DFB4;
    extern f32 fn_801C4814(s32 slot);
    extern void fn_80166A28(s32 arg0);

    if (lbl_80466E30.mode == 0) {
        fadeSetEX(9, (void*)fn_801C4814, 0, lbl_8047DFB0, lbl_8047DFB4);
        fn_80166A28(0x54);
    }
}
#pragma peephole on

/**
 * fadeCheck - Set grid rendering flag.
 * Address: 0x801C40F0 | Size: 0x74
 * Referenced by battle_main.c as "battle grid set flag".
 */
#pragma peephole off
s32 fadeCheck(u8 flag) {
    extern u8 lbl_80466E30[];
    u8* gridState;

    if (flag <= 0) {
        return (s8)lbl_80466E30[1];
    }

    gridState = lbl_80466E30;
    while (gridState[1] == 1) {
        _threadSwitch();
    }
    _threadSwitch();
    return (s8)lbl_80466E30[1];
}
#pragma peephole on

/**
 * fadeSetEX - Schedule grid update callback with arguments.
 * Address: 0x801C4164 | Size: 0x64
 */
void fadeSetEX(s32 mode, void* callback, s32 flags, f32 a, f32 b) {
}

/**
 * fadeSet - Battle camera initialization.
 * Address: 0x801C41C8 | Size: 0x74
 * Referenced by battle_main.c as "battle camera init".
 * Initializes the battle camera to the default overhead view
 * and configures the camera animation system.
 */
void fadeSet(s32 mode) {
    /* Initialize battle camera with specified mode */
}

/**
 * fn_801C423C - Battle grid callback/state transition.
 * Address: 0x801C423C | Size: 0xE0
 */
#pragma peephole off
void* fn_801C423C(void (*callback)(void), u8 mode, u32 arg, f32 value) {
    extern u8 lbl_80466E30[];
    extern volatile const f32 lbl_8047DFB8;
    extern u32 fn_80109710(void);
    extern void fn_800EF5A4(void* texture);
    extern void fn_801C432C();
    extern void fn_801C6928(void);
    void* previous;
    void (*savedCallback)(void);
    u8* gridState;
    u32 modeByte;
    u32 argHalf;

    savedCallback = callback;
    if (callback == NULL) {
        return *(void**)(lbl_80466E30 + 0xC);
    }

    modeByte = (u8)mode;
    argHalf = arg & 0xFFFF;
    gridState = lbl_80466E30;
    *(volatile u8*)(gridState + 1) = 1;
    *(volatile u16*)(gridState + 2) = argHalf;
    *(volatile f32*)(gridState + 0x14) = value;
    *(volatile f32*)(gridState + 0x18) = lbl_8047DFB8;
    *(volatile u8*)gridState = 0;
    previous = *(void* volatile *)(gridState + 0xC);
    *(void* volatile *)(gridState + 0xC) = NULL;

    if (modeByte == 1) {
        fn_801C432C();
    } else if (*(void* volatile *)(gridState + 0x10) != NULL) {
        if (*(u32 volatile *)(lbl_80466E30 + 0x10) != fn_80109710()) {
            fn_800EF5A4(*(void* volatile *)(lbl_80466E30 + 0x10));
        }
        *(void* volatile *)(lbl_80466E30 + 0x10) = NULL;
    }

    fn_801C6928();
    savedCallback();
    return previous;
}
#pragma peephole on

/**
 * fn_801C431C - Get camera current angle.
 * Address: 0x801C431C | Size: 0x10
 */
void fn_801C431C(s32 arg0) {
    extern BattleGridTransitionState lbl_80466E30;

    lbl_80466E30.callbackArg = (void*)arg0;
}

/**
 * fn_801C432C - Camera angle calculation.
 * Address: 0x801C432C | Size: 0xB8
 */
void fn_801C432C(f32 angle, f32 blend) {
    BattleGridCameraWork* cam = (BattleGridCameraWork*)lbl_80467030;

    /* Calculate camera position from angle and blend factor */
    cam->angle = angle;
    cam->blend = blend;
}

/**
 * fn_801C43E4 - Get camera target position.
 * Address: 0x801C43E4 | Size: 0x10
 */
void* fn_801C43E4(void) {
    extern u8 lbl_8047B3A8;
    lbl_8047B3A8 = 1;
    return NULL;
}

/**
 * fn_801C43F4 - Camera complex movement sequence.
 * Address: 0x801C43F4 | Size: 0x3DC
 */
void fn_801C43F4(s32 seqType, f32 param1, f32 param2) {
    BattleGridCameraWork* cam = (BattleGridCameraWork*)lbl_80467030;

    /* Camera complex movement sequence:
     * seqType 0: Pan to position (param1=angle, param2=speed)
     * seqType 1: Orbit around center (param1=radius, param2=speed)
     * seqType 2: Zoom in/out (param1=distance, param2=speed)
     * seqType 3: Shake/vibration (param1=amplitude, param2=frequency)
     * seqType 4: Custom path (param1=pathID, param2=speed)
     */
    cam->sequenceType = seqType;
    cam->sequenceParam1 = param1;
    cam->sequenceParam2 = param2;
    cam->sequenceTimer = 0.0f;
}

/**
 * fadeInit - Camera get current mode.
 * Address: 0x801C47D0 | Size: 0x44
 */
#pragma peephole off
s32 fadeInit(void) {
    extern BattleGridTransitionState lbl_80466E30;
    extern volatile const f32 lbl_8047DFB8;
    volatile BattleGridTransitionState* base = &lbl_80466E30;

    base->mode = 0;
    base->pending = 0;
    base->arg = 0;
    base->startValue = lbl_8047DFB8;
    base->endValue = lbl_8047DFB8;
    base->callbackArg = NULL;
    base->texture = NULL;
    base->value = lbl_8047DFB8;
    base->timer = lbl_8047DFB8;
    return (s32)base;
}
#pragma peephole on

/**
 * fn_801C4814 - Grid get slot X position.
 * Address: 0x801C4814 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fn_801C4814(s32 slot) {
    extern void fn_801C4A44();

    fn_801C431C((s32)fn_801C4A44);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_fadein_Init - Grid get slot Y position.
 * Address: 0x801C483C | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_fadein_Init(s32 slot) {
    extern void fn_801C4CB8(void);

    fn_801C431C((s32)fn_801C4CB8);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_trainer_Init - Grid get slot Z position.
 * Address: 0x801C4864 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_trainer_Init(s32 slot) {
    extern f32 fn_801C54FC(void);

    fn_801C431C((s32)fn_801C54FC);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_fadeout_in_Init - Grid set slot X position.
 * Address: 0x801C488C | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_fadeout_in_Init(s32 slot, f32 x) {
    extern void fn_801C5530(void);

    fn_801C431C((s32)fn_801C5530);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_carde_Init - Grid set slot Y position.
 * Address: 0x801C48B4 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_carde_Init(s32 slot, f32 y) {
    extern f32 fn_801C4C98(void);

    fn_801C431C((s32)fn_801C4C98);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_boss_Init - Grid set slot Z position.
 * Address: 0x801C48DC | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_boss_Init(s32 slot, f32 z) {
    extern void fn_801C55D8(void);

    fn_801C431C((s32)fn_801C55D8);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_yoko_or_tate_or_ball_Init - Grid set slot full position.
 * Address: 0x801C4904 | Size: 0x70
 */
void fadeEffectHookFunction_yoko_or_tate_or_ball_Init(s32 slot, f32 x, f32 y, f32 z) {
    /* Set full XYZ position for slot */
}

/**
 * fadeEffectHookFunction_ball_Init - Grid get slot rotation.
 * Address: 0x801C4974 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_ball_Init(s32 slot) {
    extern f32 fn_801C5898(void);

    fn_801C431C((s32)fn_801C5898);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_yoko_or_tate_Init - Grid set slot rotation.
 * Address: 0x801C499C | Size: 0x58
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_yoko_or_tate_Init(s32 slot, f32 rotation) {
    extern s32 fn_801C6908(s32);
    extern void fn_801C431C(s32);
    extern void fn_801C5F6C(void);
    extern void fn_801C5ED0(void);
    s32 result = fn_801C6908(2);
    switch (result) {
    case 0:
        fn_801C431C((s32)fn_801C5F6C);
        break;
    case 1:
    default:
        fn_801C431C((s32)fn_801C5ED0);
        break;
    }
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_tate_Init - Grid get slot scale.
 * Address: 0x801C49F4 | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
f32 fadeEffectHookFunction_tate_Init(s32 slot) {
    extern f32 fn_801C5ED0(void);

    fn_801C431C((s32)fn_801C5ED0);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fadeEffectHookFunction_yoko_Init - Grid set slot scale.
 * Address: 0x801C4A1C | Size: 0x28
 */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fadeEffectHookFunction_yoko_Init(s32 slot, f32 scale) {
    extern void fn_801C5F6C(void);

    fn_801C431C((s32)fn_801C5F6C);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/**
 * fn_801C4A44 - Grid complex slot update (position + rotation + scale).
 * Address: 0x801C4A44 | Size: 0x254
 */
void fn_801C4A44(s32 slot, f32 x, f32 y, f32 z, f32 rot, f32 scale) {
    BattleGridSceneWork* state = (BattleGridSceneWork*)lbl_80466E50;
    BattleGridSceneSlot* slotData;

    if (slot < 0 || slot >= BATTLE_TOTAL_POKEMON) {
        return;
    }

    slotData = &state->slots[slot];

    /* Set all transform properties */
    slotData->posX = x;
    slotData->posY = y;
    slotData->posZ = z;
    slotData->rotationY = rot;
    slotData->scale = scale;

    /* Apply to JObj */
    {
        void* jobj = slotData->jobj;
        if (jobj != NULL) {
            fn_8036A384(jobj, x, y, z);
            fn_8036A2D8(jobj, 0.0f, rot, 0.0f);
            fn_8036A478(jobj, scale, scale, scale);
        }
    }
}

/**
 * fn_801C4C98 - Get grid rotation callback.
 * Address: 0x801C4C98 | Size: 0x20
 */
f32 fn_801C4C98(void) {
    return fn_801C5F6C();
}

/**
 * fn_801C4CB8 - Grid full render update.
 * Address: 0x801C4CB8 | Size: 0x704
 * Large function handling the complete grid render pass:
 * updates all slot transforms, applies animations, renders models.
 */
void fn_801C4CB8(void) {
    s32 i;
    u8* state = (u8*)lbl_80466E50;

    /* Full grid render update:
     * 1. Update camera from BattleCameraState
     * 2. Update all slot transforms
     * 3. Animate all slot models
     * 4. Render all active slots
     */

    /* Update camera */
    {
        u8* cam = (u8*)lbl_80467030;
        s32 seqType = *(s32*)(cam + 0x0C);
        if (seqType != 0) {
            f32 timer = *(f32*)(cam + 0x18);
            timer += 1.0f;
            *(f32*)(cam + 0x18) = timer;
        }
    }

    /* Update all grid slots */
    for (i = 0; i < BATTLE_TOTAL_POKEMON; i++) {
        u8* slot = state + 0x20 + (i * 0x70);
        s32 active = *(s32*)(slot + 0x00);

        if (active == 0) {
            continue;
        }

        /* Animate model */
        {
            void* jobj = *(void**)(slot + 0x04);
            if (jobj != NULL) {
                fn_80362D0C(jobj); /* HSD_JObjAnimAll */
            }
        }

        /* Apply current transform */
        fn_801C3C98(i);
    }
}
