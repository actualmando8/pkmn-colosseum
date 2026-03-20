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
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 4 functions matched
 * =================================================================== */

extern u32 lbl_8047B2A0;
extern u32 lbl_8047B2A8;
extern u32 lbl_8047B2AC;

/* Address: 0x8019D610 | Size: 0x8 | Pattern: sda_setter */
void fn_8019D610(u32 val) {
    lbl_8047B2A8 = val;
}

/* Address: 0x8019D618 | Size: 0x8 | Pattern: sda_setter */
void fn_8019D618(u32 val) {
    lbl_8047B2A0 = val;
}

/* Address: 0x8019F01C | Size: 0x8 | Pattern: sda_getter */
u32 fn_8019F01C(void) {
    return lbl_8047B2AC;
}

/* Address: 0x801A1980 | Size: 0x8 | Pattern: addi_ptr_return */
u8* fn_801A1980(u8* obj) {
    return (u8*)obj + 0x54;
}

/* =========================================================================
 *  Internal stubs: 0x8019CB70-0x801A1988 (39 functions)
 * ========================================================================= */

/* 0x8019CB70 | 0x2E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019CB70(void) {
    /* TODO: match -- 0x2E0 bytes at 0x8019CB70 */
}
#pragma pop

/* 0x8019CE50 | 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019CE50(void) {
    /* TODO: match -- 0x104 bytes at 0x8019CE50 */
}
#pragma pop

/* 0x68 | fn_8019CF54 | framed_no_calls */
void fn_8019CF54(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B298, lbl_8047B29C, lbl_8047B2AC */
}

/* 0x8019CFBC | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019CFBC(void) {
    /* TODO: match -- 0xA0 bytes at 0x8019CFBC */
}
#pragma pop

/* 0x8019D05C | 0x544 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019D05C(void) {
    /* TODO: match -- 0x544 bytes at 0x8019D05C */
}
#pragma pop

/* 0x70 | fn_8019D5A0 | framed_no_calls */
u32 fn_8019D5A0(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047DB30 */
    return 0;
}

/* 0x8019D620 | 0x360 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019D620(void) {
    /* TODO: match -- 0x360 bytes at 0x8019D620 */
}
#pragma pop

/* 0x5C | fn_8019D980 | generic */
u32 fn_8019D980(void) {
    fn_80196E10();
    return 1;
}

/* 0x8019D9DC | 0x324 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019D9DC(void) {
    /* TODO: match -- 0x324 bytes at 0x8019D9DC */
}
#pragma pop

/* 0x8019DD00 | 0x760 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019DD00(void) {
    /* TODO: match -- 0x760 bytes at 0x8019DD00 */
}
#pragma pop

/* 0x8019E460 | 0xBBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019E460(void) {
    /* TODO: match -- 0xBBC bytes at 0x8019E460 */
}
#pragma pop

/* 0x8019F024 | 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019F024(void) {
    /* TODO: match -- 0x1A0 bytes at 0x8019F024 */
}
#pragma pop

/* 0x8019F1C4 | 0x554 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019F1C4(void) {
    /* TODO: match -- 0x554 bytes at 0x8019F1C4 */
}
#pragma pop

/* 0x60 | fn_8019F718 | generic */
void fn_8019F718(void) {
    /* refs: lbl_8036C8E0, lbl_8047B298 */
    fn_80193828();
    fn_80196E10();
}

/* 0x78 | fn_8019F778 | generic */
u32 fn_8019F778(void) {
    fn_80196E10();
    fn_8019D620();
    return 1;
}

/* 0x8019F7F0 | 0x2FC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019F7F0(void) {
    /* TODO: match -- 0x2FC bytes at 0x8019F7F0 */
}
#pragma pop

/* 0x8019FAEC | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FAEC(void) {
    /* TODO: match -- 0xA4 bytes at 0x8019FAEC */
}
#pragma pop

/* 0x8019FB90 | 0x2FC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FB90(void) {
    /* TODO: match -- 0x2FC bytes at 0x8019FB90 */
}
#pragma pop

/* 0x8019FE8C | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FE8C(void) {
    /* TODO: match -- 0xA4 bytes at 0x8019FE8C */
}
#pragma pop

/* 0x8019FF30 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FF30(void) {
    /* TODO: match -- 0x18 bytes at 0x8019FF30 */
}
#pragma pop

/* 0x8019FF48 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FF48(void) {
    /* TODO: match -- 0x2C bytes at 0x8019FF48 */
}
#pragma pop

/* 0x8019FF74 | 0x1E8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FF74(void) {
    /* TODO: match -- 0x1E8 bytes at 0x8019FF74 */
}
#pragma pop

/* 0x801A015C | 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A015C(void) {
    /* TODO: match -- 0x154 bytes at 0x801A015C */
}
#pragma pop

/* 0x801A02B0 | 0x28C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A02B0(void) {
    /* TODO: match -- 0x28C bytes at 0x801A02B0 */
}
#pragma pop

/* 0x801A053C | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A053C(void) {
    /* TODO: match -- 0xB0 bytes at 0x801A053C */
}
#pragma pop

/* 0x801A05EC | 0x158 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A05EC(void) {
    /* TODO: match -- 0x158 bytes at 0x801A05EC */
}
#pragma pop

/* 0x801A0744 | 0x458 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0744(void) {
    /* TODO: match -- 0x458 bytes at 0x801A0744 */
}
#pragma pop

/* 0x54 | fn_801A0B9C | guarded_call */
void fn_801A0B9C(void) {
    if (0 /* guard r3 == 0 */) { return; }
    if (1 /* guard r0 != 0xFFFF */) { return; }
    fn_80196E10();
}

/* 0x801A0BF0 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0BF0(void) {
    /* TODO: match -- 0x2C bytes at 0x801A0BF0 */
}
#pragma pop

/* 0x4C | fn_801A0C1C | generic */
void fn_801A0C1C(void) {
    /* refs: lbl_80274AF4, lbl_80274B64 */
    fn_80196E10();
}

/* 0x801A0C68 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0C68(void) {
    /* TODO: match -- 0x34 bytes at 0x801A0C68 */
}
#pragma pop

/* 0x4C | fn_801A0C9C | generic */
void fn_801A0C9C(void) {
    /* refs: lbl_80274AF4, lbl_80274B00 */
    fn_80196E10();
}

/* 0x54 | fn_801A0CE8 | framed_no_calls */
void fn_801A0CE8(u32 arg1, u32 arg2) {
    /* data manipulation using stack locals */
}

/* 0x801A0D3C | 0xC */
void fn_801A0D3C(void) {
}

/* 0x4C | fn_801A0D48 | generic */
u32 fn_801A0D48(void) {
    return 0;
}

/* 0x801A0D94 | 0x228 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0D94(void) {
    /* TODO: match -- 0x228 bytes at 0x801A0D94 */
}
#pragma pop

/* 0x801A0FBC | 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0FBC(void) {
    /* TODO: match -- 0xDC bytes at 0x801A0FBC */
}
#pragma pop

/* 0x801A1098 | 0x334 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A1098(void) {
    /* TODO: match -- 0x334 bytes at 0x801A1098 */
}
#pragma pop

/* 0x801A13CC | 0x5B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A13CC(void) {
    /* TODO: match -- 0x5B4 bytes at 0x801A13CC */
}
#pragma pop
