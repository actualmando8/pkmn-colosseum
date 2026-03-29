/**
 * @file hsd_pobj.c
 * @brief HSD PObj - Primitive/polygon object implementation.
 *
 * Colosseum address: 0x801AA608 (HSD_PObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/pobj.c
 */

#include "hsd/hsd_pobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"

static void PObjInfoInit(void);

HSD_PObjInfo hsdPObj = { PObjInfoInit };

static HSD_PObjInfo* default_class = NULL;

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_PObjGetFlags(HSD_PObj* pobj)
{
    HSD_ASSERT(0, pobj);
    return pobj->flags;
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_PObjAddAnim(HSD_PObj* pobj, HSD_ShapeAnim* anim)
{
    if (pobj == NULL || anim == NULL) {
        return;
    }
    if (pobj_type(pobj) == POBJ_SHAPEANIM && pobj->u.shape_set != NULL) {
        if (pobj->u.shape_set->aobj != NULL) {
            HSD_AObjRemove(pobj->u.shape_set->aobj);
        }
        pobj->u.shape_set->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
    }
}

void HSD_PObjAddAnimAll(HSD_PObj* pobj, HSD_ShapeAnim* anim)
{
    HSD_PObj* p;
    HSD_ShapeAnim* a;

    p = pobj;
    a = anim;
    while (p != NULL) {
        HSD_PObjAddAnim(p, a);
        p = p->next;
        if (a != NULL) a = a->next;
    }
}

void HSD_PObjAnim(HSD_PObj* pobj)
{
    /* Shape animation interpolation would go here */
}

void HSD_PObjAnimAll(HSD_PObj* pobj)
{
    HSD_PObj* p;
    for (p = pobj; p != NULL; p = p->next) {
        HSD_PObjAnim(p);
    }
}

/* ========================================================================= */
/*  Load                                                                     */
/* ========================================================================= */

static s32 PObjLoad(HSD_PObj* pobj, HSD_PObjDesc* desc)
{
    pobj->verts = desc->verts;
    pobj->flags = desc->flags;
    pobj->n_display = desc->n_display;
    pobj->display = desc->display;
    return 0;
}

HSD_PObj* HSD_PObjLoadDesc(HSD_PObjDesc* desc)
{
    HSD_PObj* pobj = NULL;
    HSD_PObj* first = NULL;
    HSD_PObj* prev = NULL;
    HSD_PObjDesc* d;

    for (d = desc; d != NULL; d = d->next) {
        pobj = HSD_PObjAlloc();
        HSD_POBJ_METHOD(pobj)->load(pobj, d);
        if (prev != NULL) {
            prev->next = pobj;
        } else {
            first = pobj;
        }
        prev = pobj;
    }
    return first;
}

/* ========================================================================= */
/*  Resolve refs                                                             */
/* ========================================================================= */

void HSD_PObjResolveRefs(HSD_PObj* pobj, HSD_PObjDesc* desc)
{
    /* Resolve joint references for skinning */
}

void HSD_PObjResolveRefsAll(HSD_PObj* pobj, HSD_PObjDesc* desc)
{
    HSD_PObj* p;
    HSD_PObjDesc* d;

    p = pobj;
    d = desc;
    while (p != NULL && d != NULL) {
        HSD_PObjResolveRefs(p, d);
        p = p->next;
        d = d->next;
    }
}

/* ========================================================================= */
/*  Remove / Alloc                                                           */
/* ========================================================================= */

void HSD_PObjRemove(HSD_PObj* pobj)
{
    if (pobj != NULL) {
        HSD_CLASS_METHOD(pobj)->release((HSD_Class*) pobj);
        HSD_CLASS_METHOD(pobj)->destroy((HSD_Class*) pobj);
    }
}

void HSD_PObjRemoveAll(HSD_PObj* pobj)
{
    HSD_PObj* next;
    while (pobj != NULL) {
        next = pobj->next;
        HSD_PObjRemove(pobj);
        pobj = next;
    }
}

HSD_PObj* HSD_PObjAlloc(void)
{
    HSD_PObj* pobj;
    pobj = (HSD_PObj*) hsdNew(
        default_class ? (HSD_ClassInfo*) default_class
                      : (HSD_ClassInfo*) &hsdPObj);
    HSD_ASSERT(0, pobj);
    return pobj;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void PObjRelease(HSD_Class* o)
{
    HSD_PObj* pobj = (HSD_PObj*) o;
    if (pobj_type(pobj) == POBJ_SHAPEANIM && pobj->u.shape_set != NULL) {
        HSD_AObjRemove(pobj->u.shape_set->aobj);
    }
    HSD_PARENT_INFO(&hsdPObj)->release(o);
}

static void PObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdPObj)->amnesia(info);
}

static void PObjInfoInit(void)
{
    hsdInitClassInfo((HSD_ClassInfo*) &hsdPObj, &hsdClass,
                     "sysdolphin_base_library", "hsd_pobj",
                     sizeof(HSD_PObjInfo), sizeof(HSD_PObj));
    ((HSD_ClassInfo*) &hsdPObj)->release = PObjRelease;
    ((HSD_ClassInfo*) &hsdPObj)->amnesia = PObjAmnesia;
    hsdPObj.load = PObjLoad;
}

/* 0x801AE000 | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801AE000(void) {
#include "src/hsd/hsd_pobj_ext_fn_801AE000.inc"
}
#else
void fn_801AE000(void) { /* TODO */ }
#endif
#pragma pop
