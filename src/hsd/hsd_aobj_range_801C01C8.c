/**
 * @file hsd_aobj_range_801C01C8.c
 * @brief HSD library code split out of battle_grid.c (audit 2026-07-01).
 *
 * Address range: 0x801C01C8 - 0x801C2AE8. Contains the real, variadic
 * HSD_ForeachAnim (0x801C028C, classic PPC varargs prologue; callers in
 * gs_render*.c) plus small helpers, and the per-object-kind "ForeachAnim"
 * family (JObj/DObj/LObj/CObjForeachAnim) and the surrounding HSD_AObj*
 * functions (0x801C0F20-0x801C2AE8), all of which are HSD sysdolphin
 * library code, not battle-grid code -- this is the direct continuation
 * of the same XD translation unit (aobj.cpp) as the functions above.
 * fn_801C01C8 / fn_801C021C / HSD_ForeachAnim are asm-only for now.
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
#include "game/battle/battle_grid_types.h"

/**
 * _HSD_AObjForgetMemory - Address: 0x801C0270 | Size: 0xC
 */
s32 _HSD_AObjForgetMemory(void) {
    extern u32 lbl_8047B388;
    lbl_8047B388 = 0;
}

/**
 * HSD_AObjSetRate - Address: 0x801C027C | Size: 0x10
 */
void HSD_AObjSetRate(void* obj, f32 frame) {
    if (obj != NULL) {
        *(f32*)((u8*)obj + 0x10) = frame;
    }
}

/* =========================================================================
 * NOTE: 0x801C01C8-0x801C0F20 (incl. the real, variadic HSD_ForeachAnim at
 * 0x801C028C) is HSD library code (audit 2026-07-01).
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
 * HSD_AObjRemove - Pre-grid transition helper (renamed from fn_801C25E4;
 * confirmed name -- naming pass 2026-07-07). NOTE: not verified against
 * the real melee/XD HSD_AObjRemove(HSD_AObj*) signature (this body keeps
 * the pre-existing generic void*-typed placeholder implementation; other
 * TUs that call HSD_AObjRemove(HSD_AObj*) via include/hsd/hsd_aobj.h are
 * intentionally left untouched -- see split commit notes).
 * Address: 0x801C25E4 | Size: 0x8C
 */
void HSD_AObjRemove(void* ctx, s32 mode) {
    u8* state = (u8*)ctx;
    if (state == NULL) {
        return;
    }
    /* Set camera transition mode and reset interpolation timer */
    *(s32*)(state + 0x1C0) = mode;
    *(f32*)(state + 0x1C4) = 0.0f;
}

/**
 * HSD_AObjLoadDesc - Pre-grid scene object configuration (renamed from
 * fn_801C2670; confirmed name -- naming pass 2026-07-07). See
 * HSD_AObjRemove's note above re: signature vs. include/hsd/hsd_aobj.h.
 * Address: 0x801C2670 | Size: 0x184
 */
void HSD_AObjLoadDesc(void* ctx, s32 objType, s32 param) {
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
 * HSD_AObjInterpretAnim - Pre-grid field layout calculation (renamed from
 * fn_801C27F4; confirmed name -- naming pass 2026-07-07). See
 * HSD_AObjRemove's note above re: signature vs. include/hsd/hsd_aobj.h.
 * Address: 0x801C27F4 | Size: 0x1D0
 */
void HSD_AObjInterpretAnim(void* ctx, f32 posX, f32 posZ) {
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
typedef struct BattleGridCallbackNode {
    struct BattleGridCallbackNode* next;
    void (*callback)(void);
} BattleGridCallbackNode;

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
 * HSD_AObjInitEndCallBack - Get pre-grid slot count (renamed from
 * fn_801C2A60; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C2A60 | Size: 0x14
 */
s32 HSD_AObjInitEndCallBack(void) {
    return BATTLE_TOTAL_POKEMON;
}

/**
 * HSD_AObjClearFlags - Get pre-grid slot side (renamed from fn_801C2A74;
 * confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C2A74 | Size: 0x1C
 */
s32 HSD_AObjClearFlags(s32 slot) {
    return (slot >= BATTLE_POS_ENEMY_LEFT) ? 1 : 0;
}

/**
 * HSD_AObjSetFlags - Get pre-grid slot position within side (renamed from
 * fn_801C2A90; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C2A90 | Size: 0x1C
 */
s32 HSD_AObjSetFlags(s32 slot) {
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
 * HSD_AObjInitAllocData - Pre-grid set animation state (renamed from
 * fn_801C2AB8; confirmed name -- naming pass 2026-07-07).
 * Address: 0x801C2AB8 | Size: 0x30
 */
void HSD_AObjInitAllocData(s32 slot, s32 animState) {
    extern u8 lbl_80466DB8[];
    extern void HSD_ObjAllocInit(void*, u32, u32);
    HSD_ObjAllocInit(lbl_80466DB8, 0x1c, 4);
}
