/**
 * @file hsd_wobj.c
 * @brief HSD WObj - World object implementation.
 *
 * Colosseum address: 0x801914F4 (HSD_WObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/wobj.c
 *
 * The WObj class name in Colosseum is "had_wobj" (matching Melee),
 * visible in the binary via hsdInitClassInfo.
 */

#include "hsd/hsd_wobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_robj.h"

static void WObjInfoInit(void);

HSD_WObjInfo hsdWObj = { WObjInfoInit };

static HSD_ClassInfo* default_class = NULL;

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_WObjRemoveAnim(HSD_WObj* wobj)
{
    if (wobj != NULL) {
        HSD_AObjRemove(wobj->aobj);
        wobj->aobj = NULL;
        HSD_RObjRemoveAnimAll(wobj->robj);
    }
}

void HSD_WObjReqAnim(HSD_WObj* wobj, f32 frame)
{
    if (wobj != NULL) {
        HSD_AObjReqAnim(wobj->aobj, frame);
        HSD_RObjReqAnimAll(wobj->robj, frame);
    }
}

void HSD_WObjAddAnim(HSD_WObj* wobj, HSD_WObjAnim* anim)
{
    if (wobj != NULL && anim != NULL) {
        if (wobj->aobj != NULL) {
            HSD_AObjRemove(wobj->aobj);
        }
        wobj->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
        HSD_RObjAddAnimAll(wobj->robj, anim->robjanim);
    }
}

void HSD_WObjInterpretAnim(HSD_WObj* wobj)
{
    if (wobj != NULL) {
        /* WObjUpdateFunc callback handles position animation */
        HSD_RObjAnimAll(wobj->robj);
    }
}

/* ========================================================================= */
/*  Load / Init                                                              */
/* ========================================================================= */

static int WObjLoad(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    HSD_WObjSetPosition(wobj, desc->pos_x, desc->pos_y, desc->pos_z);
    if (wobj->robj != NULL) {
        HSD_RObjRemoveAll(wobj->robj);
    }
    wobj->robj = HSD_RObjLoadDesc(desc->robjdesc);
    HSD_RObjResolveRefsAll(wobj->robj, desc->robjdesc);
    return 0;
}

void HSD_WObjInit(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    if (wobj == NULL || desc == NULL) {
        return;
    }

    HSD_WObjSetPosition(wobj, desc->pos_x, desc->pos_y, desc->pos_z);
    if (wobj->robj != NULL) {
        HSD_RObjRemoveAll(wobj->robj);
    }
    wobj->robj = HSD_RObjLoadDesc(desc->robjdesc);
    HSD_RObjResolveRefsAll(wobj->robj, desc->robjdesc);
}

/* ========================================================================= */
/*  Default class                                                            */
/* ========================================================================= */

void HSD_WObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        HSD_ASSERT(221, hsdIsDescendantOf(info, &hsdWObj));
    }
    default_class = info;
}

/* ========================================================================= */
/*  Load from descriptor                                                     */
/* ========================================================================= */

HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc)
{
    if (desc != NULL) {
        HSD_WObj* wobj;
        HSD_ClassInfo* info;
        if (desc->class_name == NULL ||
            !(info = hsdSearchClassInfo(desc->class_name)))
        {
            wobj = HSD_WObjAlloc();
        } else {
            wobj = hsdNew(info);
            HSD_ASSERT(252, wobj);
        }
        HSD_WOBJ_METHOD(wobj)->load(wobj, desc);
        return wobj;
    }
    return NULL;
}

/* ========================================================================= */
/*  Position accessors                                                       */
/* ========================================================================= */

void HSD_WObjSetPosition(HSD_WObj* wobj, f32 x, f32 y, f32 z)
{
    if (wobj == NULL) {
        return;
    }
    wobj->pos_x = x;
    wobj->pos_y = y;
    wobj->pos_z = z;
    wobj->flags |= 0x2;
    wobj->flags &= ~0x1;
}

void HSD_WObjSetPositionX(HSD_WObj* wobj, f32 val)
{
    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            wobj->flags &= ~0x1;
        }
        wobj->pos_x = val;
        wobj->flags |= 0x2;
    }
}

void HSD_WObjSetPositionY(HSD_WObj* wobj, f32 val)
{
    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            wobj->flags &= ~0x1;
        }
        wobj->pos_y = val;
        wobj->flags |= 0x2;
    }
}

void HSD_WObjSetPositionZ(HSD_WObj* wobj, f32 val)
{
    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            wobj->flags &= ~0x1;
        }
        wobj->pos_z = val;
        wobj->flags |= 0x2;
    }
}

void HSD_WObjGetPosition(HSD_WObj* wobj, f32* x, f32* y, f32* z)
{
    if (wobj == NULL) {
        return;
    }
    if (x != NULL) *x = wobj->pos_x;
    if (y != NULL) *y = wobj->pos_y;
    if (z != NULL) *z = wobj->pos_z;
}

/* ========================================================================= */
/*  Alloc                                                                    */
/* ========================================================================= */

HSD_WObj* HSD_WObjAlloc(void)
{
    HSD_WObj* wobj = (HSD_WObj*) hsdNew(
        default_class ? default_class : &hsdWObj.parent.parent);
    HSD_ASSERT(591, wobj);
    return wobj;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void WObjRelease(HSD_Class* o)
{
    HSD_WObj* wobj = (HSD_WObj*) o;
    HSD_RObjRemoveAll(wobj->robj);
    HSD_AObjRemove(wobj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdWObj)->release(o);
}

static void WObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdWObj)->amnesia(info);
}

static void WObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdWObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "had_wobj",
                     sizeof(HSD_WObjInfo), sizeof(HSD_WObj));
    HSD_CLASS_INFO(&hsdWObj)->release = WObjRelease;
    HSD_CLASS_INFO(&hsdWObj)->amnesia = WObjAmnesia;
    HSD_WOBJ_INFO(&hsdWObj)->load = WObjLoad;
}

/* 0x8019158C | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8019158C(void) {
#include "src/hsd/hsd_wobj_fn_8019158C.inc"
}
#else
void fn_8019158C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801915D4 | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AE50C(void);
extern void fn_801C25E4(void);
#if 1
asm void fn_801915D4(void) {
#include "src/hsd/hsd_wobj_fn_801915D4.inc"
}
#else
void fn_801915D4(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191628 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80193828(void);
extern void fn_80196E10(void);
#if 1
asm void fn_80191628(void) {
#include "src/hsd/hsd_wobj_fn_80191628.inc"
}
#else
void fn_80191628(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191688 | 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800A37CC(void);
extern void fn_8019D9DC(void);
#if 1
asm void fn_80191688(void) {
#include "src/hsd/hsd_wobj_fn_80191688.inc"
}
#else
void fn_80191688(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191788 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80191788(void) {
#include "src/hsd/hsd_wobj_fn_80191788.inc"
}
#else
void fn_80191788(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801917D0 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80193748(void);
#if 1
asm void fn_801917D0(void) {
#include "src/hsd/hsd_wobj_fn_801917D0.inc"
}
#else
void fn_801917D0(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019189C | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AE5E8(void);
extern void fn_801AEBE4(void);
#if 1
asm void fn_8019189C(void) {
#include "src/hsd/hsd_wobj_fn_8019189C.inc"
}
#else
void fn_8019189C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019194C | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8019194C(void) {
#include "src/hsd/hsd_wobj_fn_8019194C.inc"
}
#else
void fn_8019194C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801919EC | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801B0040(void);
extern void fn_801C27F4(void);
#if 1
asm void fn_801919EC(void) {
#include "src/hsd/hsd_wobj_fn_801919EC.inc"
}
#else
void fn_801919EC(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191A34 | 0x398 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801B1890(void);
#if 1
asm void fn_80191A34(void) {
#include "src/hsd/hsd_wobj_fn_80191A34.inc"
}
#else
void fn_80191A34(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191DCC | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AFE68(void);
extern void fn_801C2670(void);
#if 1
asm void fn_80191DCC(void) {
#include "src/hsd/hsd_wobj_fn_80191DCC.inc"
}
#else
void fn_80191DCC(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191E38 | 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AFEFC(void);
extern void fn_801C29C4(void);
#if 1
asm void fn_80191E38(void) {
#include "src/hsd/hsd_wobj_fn_80191E38.inc"
}
#else
void fn_80191E38(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191E88 | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AFFE0(void);
#if 1
asm void fn_80191E88(void) {
#include "src/hsd/hsd_wobj_fn_80191E88.inc"
}
#else
void fn_80191E88(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191ECC | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800CA7FC(void);
#if 1
asm void fn_80191ECC(void) {
#include "src/hsd/hsd_wobj_fn_80191ECC.inc"
}
#else
void fn_80191ECC(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80191F64 | 0x180 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void* memset(void* dst, int val, u32 size);
#if 1
asm void fn_80191F64(void) {
#include "src/hsd/hsd_wobj_fn_80191F64.inc"
}
#else
void fn_80191F64(void) { /* TODO */ }
#endif
#pragma pop
