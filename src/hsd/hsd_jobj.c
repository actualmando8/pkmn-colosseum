/**
 * @file hsd_jobj.c
 * @brief HSD JObj - Joint object implementation (skeletal hierarchy).
 *
 * Colosseum address: 0x8019CE50 (HSD_JObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/jobj.c
 *
 * This is one of the largest and most critical HSD modules.
 * JObj manages the skeletal hierarchy that drives all 3D models.
 */

#include "hsd/hsd_jobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_pobj.h"
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_robj.h"

static void JObjInfoInit(void);

HSD_JObjInfo hsdJObj = { JObjInfoInit };

static HSD_ClassInfo* default_class = NULL;
static HSD_JObj* current_jobj = NULL;

/* ========================================================================= */
/*  Current JObj tracking                                                    */
/* ========================================================================= */

void HSD_JObjSetCurrent(HSD_JObj* jobj)
{
    current_jobj = jobj;
}

HSD_JObj* HSD_JObjGetCurrent(void)
{
    return current_jobj;
}

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_JObjGetFlags(HSD_JObj* jobj)
{
    HSD_ASSERT(0, jobj);
    return jobj->flags;
}

void HSD_JObjSetFlags(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        jobj->flags |= flags;
    }
}

void HSD_JObjSetFlagsAll(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        HSD_JObjSetFlags(jobj, flags);
        HSD_JObjSetFlagsAll(jobj->child, flags);
        HSD_JObjSetFlagsAll(jobj->next, flags);
    }
}

void HSD_JObjClearFlags(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        jobj->flags &= ~flags;
    }
}

void HSD_JObjClearFlagsAll(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        HSD_JObjClearFlags(jobj, flags);
        HSD_JObjClearFlagsAll(jobj->child, flags);
        HSD_JObjClearFlagsAll(jobj->next, flags);
    }
}

/* ========================================================================= */
/*  DObj accessor                                                            */
/* ========================================================================= */

HSD_DObj* HSD_JObjGetDObj(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return NULL;
    }
    if (!union_type_dobj(jobj)) {
        return NULL;
    }
    return jobj->u.dobj;
}

/* ========================================================================= */
/*  Matrix dirty management                                                  */
/* ========================================================================= */

void HSD_JObjSetMtxDirtySub(HSD_JObj* jobj)
{
    HSD_JObj* child;

    if (jobj == NULL) {
        return;
    }
    jobj->flags |= JOBJ_MTX_DIRTY;
    child = jobj->child;
    while (child != NULL) {
        if (!(child->flags & JOBJ_MTX_INDEP_PARENT)) {
            HSD_JObjSetMtxDirtySub(child);
        }
        child = child->next;
    }
}

/* ========================================================================= */
/*  Hierarchy manipulation                                                   */
/* ========================================================================= */

void HSD_JObjAddChild(HSD_JObj* jobj, HSD_JObj* child)
{
    HSD_JObj* c;

    HSD_ASSERT(0, jobj);
    HSD_ASSERT(0, child);
    HSD_ASSERT(0, child->parent == NULL);

    child->parent = jobj;
    if (jobj->child == NULL) {
        jobj->child = child;
    } else {
        c = jobj->child;
        while (c->next != NULL) {
            c = c->next;
        }
        c->next = child;
    }
    HSD_JObjSetMtxDirty(child);
}

void HSD_JObjAddNext(HSD_JObj* jobj, HSD_JObj* next)
{
    HSD_ASSERT(0, jobj);
    HSD_ASSERT(0, next);

    next->parent = jobj->parent;
    next->next = jobj->next;
    jobj->next = next;
    HSD_JObjSetMtxDirty(next);
}

void HSD_JObjAddDObj(HSD_JObj* jobj, HSD_DObj* dobj)
{
    HSD_DObj* d;

    HSD_ASSERT(0, jobj);
    HSD_ASSERT(0, union_type_dobj(jobj));

    if (jobj->u.dobj == NULL) {
        jobj->u.dobj = dobj;
    } else {
        d = jobj->u.dobj;
        while (d->next != NULL) {
            d = d->next;
        }
        d->next = dobj;
    }
}

/* ========================================================================= */
/*  Reference counting                                                       */
/* ========================================================================= */

void HSD_JObjUnref(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    if (ref_DEC(jobj) != 0) {
        if (jobj != NULL) {
            ((HSD_ClassInfo*)jobj->object.parent.class_info)->release((HSD_Class*) jobj);
            ((HSD_ClassInfo*)jobj->object.parent.class_info)->destroy((HSD_Class*) jobj);
        }
    }
}

/* ========================================================================= */
/*  Remove                                                                   */
/* ========================================================================= */

HSD_JObj* HSD_JObjRemove(HSD_JObj* jobj)
{
    HSD_JObj* next;

    if (jobj == NULL) {
        return NULL;
    }

    next = jobj->next;

    /* Unlink from parent's child list */
    if (jobj->parent != NULL) {
        HSD_JObj* p = jobj->parent;
        if (p->child == jobj) {
            p->child = jobj->next;
        } else {
            HSD_JObj* c = p->child;
            while (c != NULL && c->next != jobj) {
                c = c->next;
            }
            if (c != NULL) {
                c->next = jobj->next;
            }
        }
    }

    jobj->parent = NULL;
    jobj->next = NULL;
    HSD_JObjUnref(jobj);

    return next;
}

void HSD_JObjRemoveAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_JObjRemoveAll(jobj->child);
    HSD_JObjRemoveAll(jobj->next);
    jobj->child = NULL;
    jobj->next = NULL;
    jobj->parent = NULL;
    HSD_JObjUnref(jobj);
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_JObjRemoveAnim(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_AObjRemove(jobj->aobj);
    jobj->aobj = NULL;
    if (union_type_dobj(jobj)) {
        /* Remove DObj animations would go here */
    }
    HSD_RObjRemoveAnimAll(jobj->robj);
}

void HSD_JObjRemoveAnimAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_JObjRemoveAnim(jobj);
    HSD_JObjRemoveAnimAll(jobj->child);
    HSD_JObjRemoveAnimAll(jobj->next);
}

void HSD_JObjReqAnimAll(HSD_JObj* jobj, f32 frame)
{
    if (jobj == NULL) {
        return;
    }
    HSD_AObjReqAnim(jobj->aobj, frame);
    HSD_RObjReqAnimAll(jobj->robj, frame);
    if (union_type_dobj(jobj)) {
        HSD_DObjReqAnimAll(jobj->u.dobj, frame);
    }
    HSD_JObjReqAnimAll(jobj->child, frame);
    HSD_JObjReqAnimAll(jobj->next, frame);
}

void HSD_JObjAddAnimAll(HSD_JObj* jobj, HSD_AnimJoint* animjoint,
                        HSD_MatAnimJoint* matanimjoint,
                        HSD_ShapeAnimJoint* shapeanimjoint)
{
    if (jobj == NULL) {
        return;
    }

    if (animjoint != NULL) {
        if (jobj->aobj != NULL) {
            HSD_AObjRemove(jobj->aobj);
        }
        jobj->aobj = HSD_AObjLoadDesc(animjoint->aobjdesc);
        HSD_RObjAddAnimAll(jobj->robj, animjoint->robj_anim);
    }

    if (union_type_dobj(jobj)) {
        HSD_DObjAddAnimAll(jobj->u.dobj,
            matanimjoint ? matanimjoint->matanim : NULL,
            shapeanimjoint ? shapeanimjoint->shapeanimdobj : NULL);
    }

    HSD_JObjAddAnimAll(jobj->child,
        animjoint ? animjoint->child : NULL,
        matanimjoint ? matanimjoint->child : NULL,
        shapeanimjoint ? shapeanimjoint->child : NULL);

    HSD_JObjAddAnimAll(jobj->next,
        animjoint ? animjoint->next : NULL,
        matanimjoint ? matanimjoint->next : NULL,
        shapeanimjoint ? shapeanimjoint->next : NULL);
}

void HSD_JObjAnim(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    /* Interpret AObj animation -> update JObj transform */
    HSD_RObjAnimAll(jobj->robj);
    if (union_type_dobj(jobj)) {
        HSD_DObjAnim(jobj->u.dobj);
    }
}

void HSD_JObjAnimAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_JObjAnim(jobj);
    HSD_JObjAnimAll(jobj->child);
    HSD_JObjAnimAll(jobj->next);
}

/* ========================================================================= */
/*  Alloc / Load                                                             */
/* ========================================================================= */

HSD_JObj* HSD_JObjAlloc(void)
{
    HSD_JObj* jobj;
    jobj = (HSD_JObj*) hsdNew(
        default_class ? default_class : &hsdJObj.parent.parent);
    HSD_ASSERT(0, jobj);
    return jobj;
}

void HSD_JObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        HSD_ASSERT(0, hsdIsDescendantOf(info, &hsdJObj));
    }
    default_class = info;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void JObjRelease(HSD_Class* o)
{
    HSD_JObj* jobj = (HSD_JObj*) o;

    /* Release children */
    HSD_JObjRemoveAll(jobj->child);
    jobj->child = NULL;

    /* Release DObj chain */
    if (union_type_dobj(jobj)) {
        HSD_DObjRemoveAll(jobj->u.dobj);
        jobj->u.dobj = NULL;
    }

    /* Release animation */
    HSD_AObjRemove(jobj->aobj);
    jobj->aobj = NULL;

    /* Release constraints */
    HSD_RObjRemoveAll(jobj->robj);
    jobj->robj = NULL;

    HSD_OBJECT_PARENT_INFO(&hsdJObj)->release(o);
}

static void JObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    current_jobj = NULL;
    HSD_OBJECT_PARENT_INFO(&hsdJObj)->amnesia(info);
}

static void JObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdJObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_jobj",
                     sizeof(HSD_JObjInfo), sizeof(HSD_JObj));
    HSD_CLASS_INFO(&hsdJObj)->release = JObjRelease;
    HSD_CLASS_INFO(&hsdJObj)->amnesia = JObjAmnesia;
}

/* ===================================================================
 * Generated: 0 pattern-matched + 28 stubs
 * Range: 0x8019CE50 - 0x801A1988
 * =================================================================== */

/* 0x8019CF54 | 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047B298;
extern u8 lbl_8036C8E0[];
extern u32 lbl_8047B29C;
extern u32 lbl_8047B2AC;
#if 0
asm void fn_8019CF54(void) {
#include "src/hsd/hsd_jobj_fn_8019CF54.inc"
}
#else
#pragma optimization_level 1
void fn_8019CF54(void* info) {
    if (info == (void*)lbl_8047B298) {
        lbl_8047B298 = 0;
    }
    if (info == (void*)lbl_8036C8E0) {
        lbl_8047B29C = 0;
        lbl_8047B2AC = 0;
    }
    ((HSD_ClassInfo*)lbl_8036C8E0)->head.parent->destroy((HSD_Class*)info);
}
#endif
#pragma pop

/* 0x8019CFBC | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019C128();
extern void fn_8019C1B0(void);
extern void fn_801A8570(void);
extern void fn_801A84F0(void);
#if 1
asm void fn_8019CFBC(void) {
#include "src/hsd/hsd_jobj_fn_8019CFBC.inc"
}
#else
void fn_8019CFBC(void) {
    /* TODO: match -- 160 bytes at 0x8019CFBC */
}
#endif
#pragma pop

/* 0x8019D05C | 0x544 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80196E10();
extern void fn_80196D78(void);
extern void fn_80199264(void);
extern void fn_801AE50C(void);
extern void fn_801C25E4(void);
extern u8 lbl_80274AA0[];
extern char lbl_8047DB20;
extern u8 lbl_8047DB28[];
#if 1
asm void fn_8019D05C(void) {
#include "src/hsd/hsd_jobj_fn_8019D05C.inc"
}
#else
void fn_8019D05C(void) {
    /* TODO: match -- 1348 bytes at 0x8019D05C */
}
#endif
#pragma pop

/* 0x8019D5A0 | 0x70 */
#pragma push
extern u32 lbl_8047DB30;
#if 1
asm void fn_8019D5A0(void) {
#include "src/hsd/hsd_jobj_fn_8019D5A0.inc"
}
#else
s32 fn_8019D5A0(HSD_JObj* jobj)
{
    if ((s32) ((HSD_ClassInfo*) lbl_8036C8E0)->head.parent->alloc((HSD_ClassInfo*) jobj) >= 0) {
        f32 a = *(f32*) &lbl_8047DB30;
        f32 b = *(f32*) &lbl_8047DB30;
        jobj->flags = JOBJ_MTX_DIRTY;
        jobj->scale_x = a;
        jobj->scale_y = b;
        jobj->scale_z = *(f32*) &lbl_8047DB30;
        return 0;
    }
}
#endif
#pragma pop

/* 0x8019DD00 | 0x760 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801B00E0(void);
extern void fn_800A3B7C(void);
extern void fn_800A3AC0(void);
extern void fn_800A3A78(void);
extern void fn_800A3A9C(void);
extern void fn_800A3ADC(void);
extern void fn_800CE298(void);
extern void fn_800A3244(void);
extern void fn_800A37CC(void);
extern void fn_800A3B9C(void);
extern u8 lbl_80274AC4[];
extern u32 lbl_8047DB30;
extern u8 lbl_8047DB68[];
extern u32 lbl_8047DB44;
extern u32 lbl_8047DB48;
extern u32 lbl_8047DB50;
extern u32 lbl_8047DB58;
extern u32 lbl_8047DB60;
extern u8 lbl_80478AC0[];
extern u8 lbl_8047DB6C[];
extern u32 lbl_8047DB74;
extern u32 lbl_8047DB78;
#if 1
asm void fn_8019DD00(void) {
#include "src/hsd/hsd_jobj_fn_8019DD00.inc"
}
#else
void fn_8019DD00(void) {
    /* TODO: match -- 1888 bytes at 0x8019DD00 */
}
#endif
#pragma pop

/* 0x8019E460 | 0xBBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AED88(void);
extern void fn_801A9570(void);
extern void fn_801AFCAC(void);
extern u32 lbl_8047DB48;
extern char lbl_8047DB34;
extern char lbl_8047DB3C;
extern u32 lbl_8047DB7C;
extern u32 lbl_8047DB44;
extern u32 lbl_8047DB30;
extern u32 lbl_8047DB50;
extern u32 lbl_8047DB58;
extern u32 lbl_8047DB60;
extern u32 lbl_8047DB84;
extern u32 lbl_8047DB80;
#if 1
asm void fn_8019E460(void) {
#include "src/hsd/hsd_jobj_fn_8019E460.inc"
}
#else
void fn_8019E460(void) {
    /* TODO: match -- 3004 bytes at 0x8019E460 */
}
#endif
#pragma pop

/* 0x8019F01C | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047B2AC;
#if 0
asm void fn_8019F01C(void) {
#include "src/hsd/hsd_jobj_fn_8019F01C.inc"
}
#else
u32 fn_8019F01C(void) {
    return lbl_8047B2AC;
}
#endif
#pragma pop

/* 0x8019F1C4 | 0x554 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80199178(void);
#if 1
asm void fn_8019F1C4(void) {
#include "src/hsd/hsd_jobj_fn_8019F1C4.inc"
}
#else
void fn_8019F1C4(void) {
    /* TODO: match -- 1364 bytes at 0x8019F1C4 */
}
#endif
#pragma pop

/* 0x8019F778 | 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019D620(HSD_JObj*);
#if 0
asm void fn_8019F778(void) {
#include "src/hsd/hsd_jobj_fn_8019F778.inc"
}
#else
#pragma optimization_level 1
void fn_8019F778(HSD_JObj* jobj) {
    extern void fn_80196E10();
    extern char lbl_8047DB34;
    extern char lbl_8047DB3C;
    s32 result;
    if (!jobj) return;
    if (!jobj) fn_80196E10(&lbl_8047DB34, 0x25d, &lbl_8047DB3C);
    result = 0;
    if (!(jobj->flags & 0x00800000)) {
        if (jobj->flags & 0x00000040) {
            result = 1;
        }
    }
    switch (result) {
    case 0:
        fn_8019D620(jobj);
        break;
    }
}
#endif
#pragma pop

/* 0x8019FF30 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8019FF30(void) {
#include "src/hsd/hsd_jobj_fn_8019FF30.inc"
}
#else
#pragma optimization_level 4
u32 fn_8019FF30(HSD_JObj* jobj) {
    if (jobj != NULL) {
        return jobj->flags;
    }
    return 0;
}
#endif
#pragma pop

/* 0x8019FF48 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8019FF48(void) {
#include "src/hsd/hsd_jobj_fn_8019FF48.inc"
}
#else
#pragma optimization_level 4
HSD_DObj* fn_8019FF48(HSD_JObj* jobj) {
    if (jobj == NULL) {
        goto end;
    }
    if (union_type_dobj(jobj)) {
        goto ok;
    }
end:
    return NULL;
ok:
    return jobj->u.dobj;
}
#endif
#pragma pop

/* 0x8019FF74 | 0x1E8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u8 lbl_80274B28[];
#if 1
asm void fn_8019FF74(void) {
#include "src/hsd/hsd_jobj_fn_8019FF74.inc"
}
#else
void fn_8019FF74(void) {
    /* TODO: match -- 488 bytes at 0x8019FF74 */
}
#endif
#pragma pop

/* 0x801A015C | 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport();
#if 1
asm void fn_801A015C(void) {
#include "src/hsd/hsd_jobj_fn_801A015C.inc"
}
#else
void fn_801A015C(void) {
    /* TODO: match -- 340 bytes at 0x801A015C */
}
#endif
#pragma pop

/* 0x801A02B0 | 0x28C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801A02B0(void) {
#include "src/hsd/hsd_jobj_fn_801A02B0.inc"
}
#else
void fn_801A02B0(void) {
    /* TODO: match -- 652 bytes at 0x801A02B0 */
}
#endif
#pragma pop

/* 0x801A053C | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801A053C(void) {
#include "src/hsd/hsd_jobj_fn_801A053C.inc"
}
#else
#pragma optimization_level 1
static inline BOOL iref_DEC_indiv(void* o) {
    BOOL r;
    if ((r = (*(volatile u16*)&HSD_OBJ(o)->ref_count_individual == 0))) {
        return r;
    }
    HSD_OBJ(o)->ref_count_individual -= 1;
    return HSD_OBJ(o)->ref_count_individual == 0;
}
static inline s32 ref_CNT_obj(void* o) {
    if (*(volatile u16*)&HSD_OBJ(o)->ref_count == HSD_OBJ_NOREF) {
        return -1;
    }
    return HSD_OBJ(o)->ref_count;
}
void fn_801A053C(void* obj) {
    HSD_ClassInfo* info;

    if (obj == NULL) {
        return;
    }
    if (iref_DEC_indiv(obj)) {
        if (ref_CNT_obj(obj) < 0) {
            if (obj != NULL) {
                info = HSD_CLASS_METHOD(obj);
                info->init((HSD_Class*)obj);
                info = HSD_CLASS_METHOD(obj);
                info->release((HSD_Class*)obj);
            }
        }
    }
}
#endif
#pragma pop

/* 0x801A0744 | 0x458 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80196E10();
extern void fn_801991F8(void);
extern void fn_8019C128();
extern void fn_801A05EC(void);
extern void fn_801A0B9C();
extern void fn_801A0BF0();
extern void fn_801A0C1C();
extern BOOL fn_801A0C68(HSD_Obj*);
extern void fn_801A0C9C();
extern void fn_801A0CE8(void*);
extern s32 fn_801A0D3C();
extern BOOL fn_801A0D48(void*);
extern void fn_801A0D94(void);
extern void fn_801AEBE4(void);
#if 1
asm void fn_801A0744(HSD_JObj* jobj, HSD_Joint* joint) {
#include "src/hsd/hsd_jobj_fn_801A0744.inc"
}
#else
void fn_801A0744(void) {
    /* TODO: match -- 1112 bytes at 0x801A0744 */
}
#endif
#pragma pop

/* 0x801A0B9C | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u8 lbl_80274AF4[];
extern u8 lbl_80274B64[];
#if 0
asm void fn_801A0B9C(void) {
#include "src/hsd/hsd_jobj_fn_801A0B9C.inc"
}
#else
#pragma optimization_level 4
void fn_801A0B9C(HSD_Obj* obj) {
    if (obj != NULL) {
        obj->ref_count++;
        if (!(obj->ref_count != HSD_OBJ_NOREF)) {
            fn_80196E10(lbl_80274AF4, 0x5d, lbl_80274B64);
        }
    }
}
#endif
#pragma pop

/* 0x801A0BF0 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801A0BF0(void) {
#include "src/hsd/hsd_jobj_fn_801A0BF0.inc"
}
#else
#pragma optimization_level 4
void fn_801A0BF0(HSD_JObj* jobj, u32 flags) {
    fn_8019C128(NULL, jobj, flags);
}
#endif
#pragma pop

/* 0x801A0C1C | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801A0C1C(void) {
#include "src/hsd/hsd_jobj_fn_801A0C1C.inc"
}
#else
#pragma optimization_level 4
void fn_801A0C1C(HSD_Obj* obj) {
    obj->ref_count++;
    if (!(obj->ref_count != HSD_OBJ_NOREF)) {
        fn_80196E10(lbl_80274AF4, 0x5d, lbl_80274B64);
    }
}
#endif
#pragma pop

/* 0x801A0C68 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801A0C68(void) {
#include "src/hsd/hsd_jobj_fn_801A0C68.inc"
}
#else
#pragma optimization_level 4
BOOL fn_801A0C68(HSD_Obj* obj) {
    BOOL ret;

    if ((ret = (*(volatile u16*)&obj->ref_count_individual == 0))) {
        return ret;
    }
    obj->ref_count_individual--;
    return obj->ref_count_individual == 0;
}
#endif
#pragma pop

/* 0x801A0C9C | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u8 lbl_80274B00[];
#if 0
asm void fn_801A0C9C(void) {
#include "src/hsd/hsd_jobj_fn_801A0C9C.inc"
}
#else
#pragma optimization_level 4
void fn_801A0C9C(HSD_Obj* obj) {
    obj->ref_count_individual++;
    if (!(obj->ref_count_individual != 0)) {
        fn_80196E10(lbl_80274AF4, 0x9e, lbl_80274B00);
    }
}
#endif
#pragma pop

/* 0x801A0CE8 | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801A0CE8(void) {
#include "src/hsd/hsd_jobj_fn_801A0CE8.inc"
}
#else
#pragma optimization_level 1
void fn_801A0CE8(void* obj) {
    HSD_ClassInfo* info;

    if (obj != NULL) {
        info = HSD_CLASS_METHOD(obj);
        info->init((HSD_Class*)obj);
        info = HSD_CLASS_METHOD(obj);
        info->release((HSD_Class*)obj);
    }
}
#endif
#pragma pop

/* 0x801A0D3C | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm s32 fn_801A0D3C(void) {
#include "src/hsd/hsd_jobj_fn_801A0D3C.inc"
}
#else
#pragma optimization_level 4
s32 fn_801A0D3C(HSD_Obj* obj) {
    return obj->ref_count_individual - 1;
}
#endif
#pragma pop

/* 0x801A0D48 | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801A0D48(void) {
#include "src/hsd/hsd_jobj_fn_801A0D48.inc"
}
#else
#pragma optimization_level 1
BOOL fn_801A0D48(void* o) {
    BOOL ret;

    if ((ret = (*(volatile u16*)&HSD_OBJ(o)->ref_count == HSD_OBJ_NOREF))) {
        return ret;
    }
    ret = (*(volatile u16*)&HSD_OBJ(o)->ref_count == 0);
    HSD_OBJ(o)->ref_count--;
    return ret;
}
#endif
#pragma pop

/* 0x801A0D94 | 0x228 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801A0D94(void) {
#include "src/hsd/hsd_jobj_fn_801A0D94.inc"
}
#else
void fn_801A0D94(void) {
    /* TODO: match -- 552 bytes at 0x801A0D94 */
}
#endif
#pragma pop

/* 0x801A0FBC | 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern HSD_ClassInfo* fn_80193748(const char*);
extern HSD_JObj* fn_80193828(HSD_ClassInfo*);
extern u32 lbl_8047B298;
#if 1
asm void fn_801A0FBC(void) {
#include "src/hsd/hsd_jobj_fn_801A0FBC.inc"
}
#else
#pragma optimization_level 1
HSD_JObj* fn_801A0FBC(HSD_Joint* joint)
{
    HSD_JObj* jobj;
    HSD_ClassInfo* info;

    if (joint == NULL) {
        jobj = NULL;
        goto done;
    }

    if (joint->class_name != NULL) {
        info = fn_80193748(joint->class_name);
        if (info != NULL) {
            goto found;
        }
    }

    if (lbl_8047B298 != 0) {
        info = (HSD_ClassInfo*) lbl_8047B298;
    } else {
        info = (HSD_ClassInfo*) lbl_8036C8E0;
    }
    jobj = fn_80193828(info);
    if (jobj == NULL) {
        fn_80196E10(&lbl_8047DB20, 0x7DF, &lbl_8047DB3C);
    }
    goto setup;

found:
    jobj = fn_80193828(info);
    if (jobj == NULL) {
        fn_80196E10(&lbl_8047DB20, 0x3D5, &lbl_8047DB3C);
    }

setup:
    {
        void** vtable = *(void***) jobj;
        ((void (*)(HSD_JObj*, HSD_Joint*, HSD_JObj*)) vtable[0x3C / 4])(jobj, joint, NULL);
    }

done:
    fn_801A0744(jobj, joint);
    return jobj;
}
#endif
#pragma pop

/* 0x801A1098 | 0x334 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801992D8(void);
extern void fn_801AE5E8(void);
extern void fn_800A2D38(void);
extern void fn_801A8524(void);
extern void HSD_IDInsertToTable(void);
extern void* memcpy(void* dst, const void* src, u32 n);
extern u32 lbl_8047B298;
#if 1
asm void fn_801A1098(void) {
#include "src/hsd/hsd_jobj_fn_801A1098.inc"
}
#else
void fn_801A1098(void) {
    /* TODO: match -- 820 bytes at 0x801A1098 */
}
#endif
#pragma pop

/* 0x801A1980 | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801A1980(void) {
#include "src/hsd/hsd_jobj_fn_801A1980.inc"
}
#else
f32* fn_801A1980(HSD_JObj* jobj) {
    return jobj->mtx[1];
}
#endif
#pragma pop
