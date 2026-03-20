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

/* =========================================================================
 *  Internal stubs: 0x801914F4-0x80193748 (17 functions)
 * ========================================================================= */

/* 0x801914F4 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801914F4(void) {
    /* TODO: match -- 0x98 bytes at 0x801914F4 */
}
#pragma pop

/* 0x48 | fn_8019158C | framed_no_calls */
void fn_8019158C(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B218 */
}

/* 0x54 | fn_801915D4 | call_sequence */
void fn_801915D4(void) {
    fn_801AE50C();
    fn_801C25E4();
}

/* 0x60 | fn_80191628 | generic */
void fn_80191628(void) {
    /* refs: lbl_8036C5F0, lbl_8047B218 */
    fn_80193828();
    fn_80196E10();
}

/* 0x80191688 | 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80191688(void) {
    /* TODO: match -- 0x100 bytes at 0x80191688 */
}
#pragma pop

/* 0x48 | fn_80191788 | generic */
void fn_80191788(u32 arg1, u32 arg2) {

}

/* 0x801917D0 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801917D0(void) {
    /* TODO: match -- 0xCC bytes at 0x801917D0 */
}
#pragma pop

/* 0x8019189C | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019189C(void) {
    /* TODO: match -- 0xB0 bytes at 0x8019189C */
}
#pragma pop

/* 0x8019194C | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019194C(void) {
    /* TODO: match -- 0xA0 bytes at 0x8019194C */
}
#pragma pop

/* 0x48 | fn_801919EC | generic */
void fn_801919EC(void) {
    fn_801C27F4();
    fn_801B0040();
}

/* 0x80191A34 | 0x398 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80191A34(void) {
    /* TODO: match -- 0x398 bytes at 0x80191A34 */
}
#pragma pop

/* 0x6C | fn_80191DCC | generic */
void fn_80191DCC(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_801C25E4();
    fn_801C2670();
    fn_801AFE68();
}

/* 0x50 | fn_80191E38 | generic */
void fn_80191E38(void) {
    fn_801C29C4();
    fn_801AFEFC();
}

/* 0x44 | fn_80191E88 | generic */
void fn_80191E88(void) {
    fn_801C25E4();
    fn_801AFFE0();
}

/* 0x80191ECC | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80191ECC(void) {
    /* TODO: match -- 0x98 bytes at 0x80191ECC */
}
#pragma pop

/* 0x80191F64 | 0x180 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80191F64(void) {
    /* TODO: match -- 0x180 bytes at 0x80191F64 */
}
#pragma pop

/* 0x801920E4 | 0x1664 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801920E4(void) {
    /* TODO: match -- 0x1664 bytes at 0x801920E4 */
}
#pragma pop
