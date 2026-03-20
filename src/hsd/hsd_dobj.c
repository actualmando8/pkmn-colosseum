/**
 * @file hsd_dobj.c
 * @brief HSD DObj - Display object implementation.
 *
 * Colosseum address: 0x80198F7C (HSD_DObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/dobj.c
 */

#include "hsd/hsd_dobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_pobj.h"

static void DObjInfoInit(void);

static HSD_DObjInfo hsdDObj = { DObjInfoInit };
static HSD_ClassInfo* default_class = NULL;

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_DObjGetFlags(HSD_DObj* dobj)
{
    HSD_ASSERT(0, dobj);
    return dobj->flags;
}

void HSD_DObjSetFlags(HSD_DObj* dobj, u32 flags)
{
    HSD_ASSERT(0, dobj);
    dobj->flags |= flags;
}

void HSD_DObjClearFlags(HSD_DObj* dobj, u32 flags)
{
    HSD_ASSERT(0, dobj);
    dobj->flags &= ~flags;
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_DObjAddAnim(HSD_DObj* dobj, HSD_MatAnim* mat_anim,
                     HSD_ShapeAnimDObj* sh_anim)
{
    if (dobj == NULL) {
        return;
    }
    HSD_MObjAddAnim(dobj->mobj, mat_anim);
    if (sh_anim != NULL) {
        HSD_PObjAddAnim(dobj->pobj, sh_anim->shapeanim);
    }
}

void HSD_DObjAddAnimAll(HSD_DObj* dobj, HSD_MatAnim* matanim,
                        HSD_ShapeAnimDObj* shapeanimdobj)
{
    HSD_DObj* d;
    HSD_MatAnim* ma;
    HSD_ShapeAnimDObj* sa;

    d = dobj;
    ma = matanim;
    sa = shapeanimdobj;

    while (d != NULL) {
        HSD_DObjAddAnim(d, ma, sa);
        d = d->next;
        if (ma != NULL) ma = ma->next;
        if (sa != NULL) sa = sa->next;
    }
}

void HSD_DObjReqAnimAll(HSD_DObj* dobj, f32 startframe)
{
    HSD_DObj* d;
    for (d = dobj; d != NULL; d = d->next) {
        HSD_MObjReqAnim(d->mobj, startframe);
        /* PObj shape anim req would go here */
    }
}

void HSD_DObjAnim(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        HSD_MObjAnim(dobj->mobj);
        HSD_PObjAnim(dobj->pobj);
    }
}

void HSD_DObjAnimAll(HSD_DObj* dobj)
{
    HSD_DObj* d;
    for (d = dobj; d != NULL; d = d->next) {
        HSD_DObjAnim(d);
    }
}

/* ========================================================================= */
/*  Load                                                                     */
/* ========================================================================= */

static int DObjLoad(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    if (dobj->mobj != NULL) {
        HSD_MObjRemove(dobj->mobj);
    }
    dobj->mobj = HSD_MObjLoadDesc(desc->mobjdesc);
    if (dobj->pobj != NULL) {
        HSD_PObjRemoveAll(dobj->pobj);
    }
    dobj->pobj = HSD_PObjLoadDesc(desc->pobjdesc);
    return 0;
}

HSD_DObj* HSD_DObjLoadDesc(HSD_DObjDesc* desc)
{
    HSD_DObj* dobj = NULL;
    HSD_DObj* first = NULL;
    HSD_DObj* prev = NULL;
    HSD_DObjDesc* d;

    for (d = desc; d != NULL; d = d->next) {
        HSD_ClassInfo* info;
        if (d->class_name == NULL ||
            !(info = hsdSearchClassInfo(d->class_name)))
        {
            dobj = HSD_DObjAlloc();
        } else {
            dobj = hsdNew(info);
            HSD_ASSERT(0, dobj);
        }
        HSD_DOBJ_METHOD(dobj)->load(dobj, d);
        if (prev != NULL) {
            prev->next = dobj;
        } else {
            first = dobj;
        }
        prev = dobj;
    }
    return first;
}

/* ========================================================================= */
/*  Resolve refs                                                             */
/* ========================================================================= */

void HSD_DObjResolveRefs(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    if (dobj != NULL && desc != NULL) {
        HSD_PObjResolveRefsAll(dobj->pobj, desc->pobjdesc);
    }
}

void HSD_DObjResolveRefsAll(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    HSD_DObj* d;
    HSD_DObjDesc* dd;

    d = dobj;
    dd = desc;
    while (d != NULL && dd != NULL) {
        HSD_DObjResolveRefs(d, dd);
        d = d->next;
        dd = dd->next;
    }
}

/* ========================================================================= */
/*  Remove                                                                   */
/* ========================================================================= */

void HSD_DObjRemove(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        HSD_CLASS_METHOD(dobj)->release((HSD_Class*) dobj);
        HSD_CLASS_METHOD(dobj)->destroy((HSD_Class*) dobj);
    }
}

void HSD_DObjRemoveAll(HSD_DObj* dobj)
{
    HSD_DObj* next;
    while (dobj != NULL) {
        next = dobj->next;
        HSD_DObjRemove(dobj);
        dobj = next;
    }
}

/* ========================================================================= */
/*  Alloc                                                                    */
/* ========================================================================= */

HSD_DObj* HSD_DObjAlloc(void)
{
    HSD_DObj* dobj;
    dobj = (HSD_DObj*) hsdNew(
        default_class ? default_class : (HSD_ClassInfo*) &hsdDObj);
    HSD_ASSERT(0, dobj);
    return dobj;
}

void HSD_DObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        HSD_ASSERT(0, hsdIsDescendantOf(info, &hsdDObj));
    }
    default_class = info;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void DObjRelease(HSD_Class* o)
{
    HSD_DObj* dobj = (HSD_DObj*) o;
    HSD_MObjRemove(dobj->mobj);
    HSD_PObjRemoveAll(dobj->pobj);
    HSD_AObjRemove(dobj->aobj);
    HSD_PARENT_INFO(&hsdDObj)->release(o);
}

static void DObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdDObj)->amnesia(info);
}

static void DObjInfoInit(void)
{
    hsdInitClassInfo((HSD_ClassInfo*) &hsdDObj, &hsdClass,
                     "sysdolphin_base_library", "hsd_dobj",
                     sizeof(HSD_DObjInfo), sizeof(HSD_DObj));
    ((HSD_ClassInfo*) &hsdDObj)->release = DObjRelease;
    ((HSD_ClassInfo*) &hsdDObj)->amnesia = DObjAmnesia;
    hsdDObj.load = DObjLoad;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

extern u32 lbl_8047B264;

/* Address: 0x80199704 | Size: 0x8 | Pattern: sda_setter */
void fn_80199704(u32 val) {
    lbl_8047B264 = val;
}
