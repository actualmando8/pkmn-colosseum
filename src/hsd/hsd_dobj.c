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
 * WP-0061: asm wrappers
 * Range: 0x80199014 - 0x80199794
 * =================================================================== */

/* 0x80199014 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80199014(void) {
#include "src/hsd/hsd_dobj_fn_80199014.inc"
}
#else
void fn_80199014(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019905C | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A6D08(void);
extern void fn_801AD214(void);
extern void fn_801C25E4(void);
#if 1
asm void fn_8019905C(void) {
#include "src/hsd/hsd_dobj_fn_8019905C.inc"
}
#else
void fn_8019905C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801990B8 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A8470(void);
#if 1
asm void fn_801990B8(void) {
#include "src/hsd/hsd_dobj_fn_801990B8.inc"
}
#else
void fn_801990B8(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80199178 | 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801ACDAC(void);
#if 1
asm void fn_80199178(void) {
#include "src/hsd/hsd_dobj_fn_80199178.inc"
}
#else
void fn_80199178(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801991F8 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AD044(void);
#if 1
asm void fn_801991F8(void) {
#include "src/hsd/hsd_dobj_fn_801991F8.inc"
}
#else
void fn_801991F8(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80199264 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80199264(void) {
#include "src/hsd/hsd_dobj_fn_80199264.inc"
}
#else
void fn_80199264(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801992D8 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80193748(void);
extern void fn_80193828(void);
extern void fn_80196E10(void);
#if 1
asm void fn_801992D8(void) {
#include "src/hsd/hsd_dobj_fn_801992D8.inc"
}
#else
void fn_801992D8(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801993A4 | 0x1C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern void fn_801A7B24(void);
extern void fn_801AD288(void);
extern void fn_80196D78(void);
#if 1
asm void fn_801993A4(void) {
#include "src/hsd/hsd_dobj_fn_801993A4.inc"
}
#else
void fn_801993A4(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80199568 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A7E3C(void);
extern void fn_801AD61C(void);
extern void fn_801C27F4(void);
#if 1
asm void fn_80199568(void) {
#include "src/hsd/hsd_dobj_fn_80199568.inc"
}
#else
void fn_80199568(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801995D4 | 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A8354(void);
extern void fn_801AD6C4(void);
#if 1
asm void fn_801995D4(void) {
#include "src/hsd/hsd_dobj_fn_801995D4.inc"
}
#else
void fn_801995D4(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80199654 | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801A83BC(void);
extern void fn_801AD738(void);
#if 1
asm void fn_80199654(void) {
#include "src/hsd/hsd_dobj_fn_80199654.inc"
}
#else
void fn_80199654(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80199704 | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80199704(void) {
#include "src/hsd/hsd_dobj_fn_80199704.inc"
}
#else
void fn_80199704(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019970C | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AA498(void* list, void* data);
#if 1
asm void fn_8019970C(void) {
#include "src/hsd/hsd_dobj_fn_8019970C.inc"
}
#else
void fn_8019970C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80199738 | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AA4CC(void);
extern void* memset(void* dst, int val, u32 size);
#if 1
asm void fn_80199738(void) {
#include "src/hsd/hsd_dobj_fn_80199738.inc"
}
#else
void fn_80199738(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80199794 | 0x2F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80199A84(void);
#if 1
asm void fn_80199794(void) {
#include "src/hsd/hsd_dobj_fn_80199794.inc"
}
#else
void fn_80199794(void) { /* TODO */ }
#endif
#pragma pop

/* ===================================================================
 * Generated: 0 pattern-matched + 5 stubs
 * Range: 0x8019B528 - 0x8019B7C0
 * =================================================================== */

/* 0x8019B528 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047DA58;
extern u32 lbl_8047DA3C;
#if 1
asm void fn_8019B528(void) {
#include "src/hsd/hsd_dobj_fn_8019B528.inc"
}
#else
void fn_8019B528(void) {
    /* TODO: match -- 192 bytes at 0x8019B528 */
}
#endif
#pragma pop

/* 0x8019B5E8 | 0x168 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019970C(void);
extern void fn_801AA498(void* list, void* data);
extern void fn_80199A84(void);
extern void fn_8019B750(void* data);
extern u8 lbl_80465378[];
extern u8 lbl_802747AC[];
#if 1
asm void fn_8019B5E8(void) {
#include "src/hsd/hsd_dobj_fn_8019B5E8.inc"
}
#else
void fn_8019B5E8(void) {
    /* TODO: match -- 360 bytes at 0x8019B5E8 */
}
#endif
#pragma pop

/* 0x8019B750 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8019B750(void) {
#include "src/hsd/hsd_dobj_fn_8019B750.inc"
}
#else
#pragma optimization_level 4
void fn_8019B750(void* data) {
    if (data != NULL) {
        fn_801AA498(lbl_80465378, data);
    }
}
#endif
#pragma pop

/* 0x8019B784 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AA35C(void* list, u32 size, u32 alignment);
#if 0
asm void fn_8019B784(void) {
#include "src/hsd/hsd_dobj_fn_8019B784.inc"
}
#else
void fn_8019B784(void) {
    fn_801AA35C(lbl_80465378, 0x30, 4);
}
#endif
#pragma pop

/* 0x8019B7B4 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8019B7B4(void) {
#include "src/hsd/hsd_dobj_fn_8019B7B4.inc"
}
#else
void* fn_8019B7B4(void) {
    return lbl_80465378;
}
#endif
#pragma pop
