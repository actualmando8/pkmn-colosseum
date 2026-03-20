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

/* =========================================================================
 *  Internal stubs: 0x80198F4C-0x80199A84 (16 functions)
 * ========================================================================= */

/* 0x80198F4C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80198F4C(void) {
    /* TODO: match -- 0x30 bytes at 0x80198F4C */
}
#pragma pop

/* 0x80198F7C | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80198F7C(void) {
    /* TODO: match -- 0x98 bytes at 0x80198F7C */
}
#pragma pop

/* 0x48 | fn_80199014 | framed_no_calls */
void fn_80199014(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B260 */
}

/* 0x5C | fn_8019905C | call_sequence */
void fn_8019905C(void) {
    fn_801A6D08();
    fn_801AD214();
    fn_801C25E4();
}

/* 0x801990B8 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801990B8(void) {
    /* TODO: match -- 0xC0 bytes at 0x801990B8 */
}
#pragma pop

/* 0x80 | fn_80199178 | generic */
void fn_80199178(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_801ACDAC();
}

/* 0x6C | fn_801991F8 | guarded_call */
void fn_801991F8(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (0 /* guard r30 == 0 */) { return; }
    if (0 /* guard r30 == 0 */) { return; }
    if (0 /* guard r31 == 0 */) { return; }
    fn_801AD044();
}

/* 0x74 | fn_80199264 | framed_no_calls */
void fn_80199264(u32 arg1, u32 arg2) {
    /* data manipulation using stack locals */
}

/* 0x801992D8 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801992D8(void) {
    /* TODO: match -- 0xCC bytes at 0x801992D8 */
}
#pragma pop

/* 0x801993A4 | 0x1C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801993A4(void) {
    /* TODO: match -- 0x1C4 bytes at 0x801993A4 */
}
#pragma pop

/* 0x6C | fn_80199568 | generic */
void fn_80199568(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_801C27F4();
    fn_801AD61C();
    fn_801A7E3C();
}

/* 0x80 | fn_801995D4 | generic */
void fn_801995D4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_801AD6C4();
    fn_801A8354();
}

/* 0x80199654 | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80199654(void) {
    /* TODO: match -- 0xB0 bytes at 0x80199654 */
}
#pragma pop

/* 0x8019970C | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019970C(void) {
    /* TODO: match -- 0x2C bytes at 0x8019970C */
}
#pragma pop

/* 0x5C | fn_80199738 | multi_call_guarded */
void fn_80199738(void) {
    { fn_801AA4CC(); return; }
    fn_80196E10();
    memset();
}

/* 0x80199794 | 0x2F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80199794(void) {
    /* TODO: match -- 0x2F0 bytes at 0x80199794 */
}
#pragma pop
