/**
 * @file hsd_lobj.c
 * @brief HSD LObj - Light object implementation.
 *
 * Colosseum address: 0x801A4000 (HSD_LObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/lobj.c
 */

#include "hsd/hsd_lobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_wobj.h"

static void LObjInfoInit(void);

HSD_LObjInfo hsdLObj = { LObjInfoInit };

static HSD_LObjInfo* default_class = NULL;
static s32 nb_active = 0;

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_LObjGetFlags(HSD_LObj* lobj)
{
    HSD_ASSERT(0, lobj);
    return lobj->flags;
}

void HSD_LObjSetFlags(HSD_LObj* lobj, u32 flags)
{
    HSD_ASSERT(0, lobj);
    lobj->flags |= (u16) flags;
}

void HSD_LObjClearFlags(HSD_LObj* lobj, u32 flags)
{
    HSD_ASSERT(0, lobj);
    lobj->flags &= (u16) ~flags;
}

/* ========================================================================= */
/*  Active lights                                                            */
/* ========================================================================= */

void HSD_LObjSetActive(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        nb_active++;
    }
}

s32 HSD_LObjGetNbActive(void)
{
    return nb_active;
}

void HSD_LObjClearActive(void)
{
    nb_active = 0;
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_LObjAddAnim(HSD_LObj* lobj, HSD_LightAnim* lanim)
{
    if (lobj == NULL || lanim == NULL) {
        return;
    }
    if (lobj->aobj != NULL) {
        HSD_AObjRemove(lobj->aobj);
    }
    lobj->aobj = HSD_AObjLoadDesc(lanim->aobjdesc);
    HSD_WObjAddAnim(lobj->position, lanim->position_anim);
    HSD_WObjAddAnim(lobj->interest, lanim->interest_anim);
}

void HSD_LObjAddAnimAll(HSD_LObj* lobj, HSD_LightAnim* lanim)
{
    HSD_LObj* l;
    HSD_LightAnim* a;

    l = lobj;
    a = lanim;
    while (l != NULL) {
        HSD_LObjAddAnim(l, a);
        l = l->next;
        if (a != NULL) a = a->next;
    }
}

void HSD_LObjAnim(HSD_LObj* lobj)
{
    if (lobj != NULL) {
        HSD_WObjInterpretAnim(lobj->position);
        HSD_WObjInterpretAnim(lobj->interest);
    }
}

void HSD_LObjAnimAll(HSD_LObj* lobj)
{
    HSD_LObj* l;
    for (l = lobj; l != NULL; l = l->next) {
        HSD_LObjAnim(l);
    }
}

void HSD_LObjReqAnim(HSD_LObj* lobj, f32 startframe)
{
    if (lobj != NULL) {
        HSD_AObjReqAnim(lobj->aobj, startframe);
        HSD_WObjReqAnim(lobj->position, startframe);
        HSD_WObjReqAnim(lobj->interest, startframe);
    }
}

void HSD_LObjReqAnimAll(HSD_LObj* lobj, f32 startframe)
{
    HSD_LObj* l;
    for (l = lobj; l != NULL; l = l->next) {
        HSD_LObjReqAnim(l, startframe);
    }
}

/* ========================================================================= */
/*  Color / position                                                         */
/* ========================================================================= */

void HSD_LObjSetColor(HSD_LObj* lobj, u32 color)
{
    HSD_ASSERT(0, lobj);
    lobj->color = color;
}

void HSD_LObjSetPosition(HSD_LObj* lobj, f32 x, f32 y, f32 z)
{
    HSD_ASSERT(0, lobj);
    HSD_WObjSetPosition(lobj->position, x, y, z);
}

void HSD_LObjSetInterest(HSD_LObj* lobj, f32 x, f32 y, f32 z)
{
    HSD_ASSERT(0, lobj);
    HSD_WObjSetPosition(lobj->interest, x, y, z);
}

/* ========================================================================= */
/*  Remove / Alloc                                                           */
/* ========================================================================= */

void HSD_LObjRemoveAll(HSD_LObj* lobj)
{
    HSD_LObj* next;
    while (lobj != NULL) {
        next = lobj->next;
        HSD_OBJECT_METHOD(lobj)->release((HSD_Class*) lobj);
        HSD_OBJECT_METHOD(lobj)->destroy((HSD_Class*) lobj);
        lobj = next;
    }
}

HSD_LObj* HSD_LObjAlloc(void)
{
    HSD_LObj* lobj;
    lobj = (HSD_LObj*) hsdNew(
        default_class ? (HSD_ClassInfo*) default_class
                      : &hsdLObj.parent.parent);
    HSD_ASSERT(0, lobj);
    return lobj;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void LObjRelease(HSD_Class* o)
{
    HSD_LObj* lobj = (HSD_LObj*) o;
    HSD_WObjUnref(lobj->position);
    HSD_WObjUnref(lobj->interest);
    HSD_AObjRemove(lobj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdLObj)->release(o);
}

static void LObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    nb_active = 0;
    HSD_OBJECT_PARENT_INFO(&hsdLObj)->amnesia(info);
}

static void LObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdLObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_lobj",
                     sizeof(HSD_LObjInfo), sizeof(HSD_LObj));
    HSD_CLASS_INFO(&hsdLObj)->release = LObjRelease;
    HSD_CLASS_INFO(&hsdLObj)->amnesia = LObjAmnesia;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 5 functions matched
 * =================================================================== */

extern u32 lbl_8047B2B8;
extern u32 lbl_8047B2BC;
extern u32 lbl_8047B2C0;
extern u32 lbl_8047B2C4;
extern u32 lbl_8047B2C8;

/* Address: 0x801A68D0 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801A68D0(void) {
    return lbl_8047B2B8;
}

/* Address: 0x801A68D8 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801A68D8(void) {
    return lbl_8047B2C0;
}

/* Address: 0x801A68E0 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801A68E0(void) {
    return lbl_8047B2C8;
}

/* Address: 0x801A68E8 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801A68E8(void) {
    return lbl_8047B2C4;
}

/* Address: 0x801A68F0 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801A68F0(void) {
    return lbl_8047B2BC;
}

/* =========================================================================
 *  Internal stubs: 0x801A3FBC-0x801A69C0 (31 functions)
 * ========================================================================= */

/* 0x44 | fn_801A3FBC | call_sequence */
void fn_801A3FBC(void) {
    fn_801AA35C();
    fn_801AA35C();
}

/* 0x801A4000 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4000(void) {
    /* TODO: match -- 0x98 bytes at 0x801A4000 */
}
#pragma pop

/* 0x60 | fn_801A4098 | framed_no_calls */
void fn_801A4098(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B2B4, lbl_8047B2B0 */
}

/* 0x801A40F8 | 0x174 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A40F8(void) {
    /* TODO: match -- 0x174 bytes at 0x801A40F8 */
}
#pragma pop

/* 0x801A426C | 0xD8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A426C(void) {
    /* TODO: match -- 0xD8 bytes at 0x801A426C */
}
#pragma pop

/* 0x801A4344 | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4344(void) {
    /* TODO: match -- 0xFC bytes at 0x801A4344 */
}
#pragma pop

/* 0x801A4440 | 0x470 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4440(void) {
    /* TODO: match -- 0x470 bytes at 0x801A4440 */
}
#pragma pop

/* 0x44 | fn_801A48B0 | guarded_call */
u32 fn_801A48B0(void) {
    if (0 /* guard r3 == 0 */) { return 1; }
    if (0 /* guard r0 == 0 */) { return 1; }
    fn_80191688();
    return 0;
}

/* 0x801A48F4 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A48F4(void) {
    /* TODO: match -- 0x88 bytes at 0x801A48F4 */
}
#pragma pop

/* 0x44 | fn_801A497C | guarded_call */
u32 fn_801A497C(void) {
    if (0 /* guard r3 == 0 */) { return 1; }
    if (0 /* guard r0 == 0 */) { return 1; }
    fn_80191688();
    return 0;
}

/* 0x801A49C0 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A49C0(void) {
    /* TODO: match -- 0x88 bytes at 0x801A49C0 */
}
#pragma pop

/* 0x801A4A48 | 0xC */
void fn_801A4A48(void) {
}

/* 0x70 | fn_801A4A54 | generic */
u32 fn_801A4A54(u32 arg1) {
    /* refs: jumptable_8036CA64 */
    return 0;
}

/* 0x801A4AC4 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4AC4(void) {
    /* TODO: match -- 0x3C bytes at 0x801A4AC4 */
}
#pragma pop

/* 0x801A4B00 | 0x220 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4B00(void) {
    /* TODO: match -- 0x220 bytes at 0x801A4B00 */
}
#pragma pop

/* 0x801A4D20 | 0x234 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4D20(void) {
    /* TODO: match -- 0x234 bytes at 0x801A4D20 */
}
#pragma pop

/* 0x801A4F54 | 0xE78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4F54(void) {
    /* TODO: match -- 0xE78 bytes at 0x801A4F54 */
}
#pragma pop

/* 0x801A5DCC | 0x2CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A5DCC(void) {
    /* TODO: match -- 0x2CC bytes at 0x801A5DCC */
}
#pragma pop

/* 0x801A6098 | 0x174 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6098(void) {
    /* TODO: match -- 0x174 bytes at 0x801A6098 */
}
#pragma pop

/* 0x801A620C | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A620C(void) {
    /* TODO: match -- 0x164 bytes at 0x801A620C */
}
#pragma pop

/* 0x801A6370 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6370(void) {
    /* TODO: match -- 0x98 bytes at 0x801A6370 */
}
#pragma pop

/* 0x801A6408 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6408(void) {
    /* TODO: match -- 0x8C bytes at 0x801A6408 */
}
#pragma pop

/* 0x801A6494 | 0x24C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6494(void) {
    /* TODO: match -- 0x24C bytes at 0x801A6494 */
}
#pragma pop

/* 0x801A66E0 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A66E0(void) {
    /* TODO: match -- 0xAC bytes at 0x801A66E0 */
}
#pragma pop

/* 0x801A678C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A678C(void) {
    /* TODO: match -- 0x30 bytes at 0x801A678C */
}
#pragma pop

/* 0x801A67BC | 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A67BC(void) {
    /* TODO: match -- 0x114 bytes at 0x801A67BC */
}
#pragma pop

/* 0x801A68F8 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A68F8(void) {
    /* TODO: match -- 0x18 bytes at 0x801A68F8 */
}
#pragma pop

/* 0x801A6910 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6910(void) {
    /* TODO: match -- 0x18 bytes at 0x801A6910 */
}
#pragma pop

/* 0x801A6928 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6928(void) {
    /* TODO: match -- 0x38 bytes at 0x801A6928 */
}
#pragma pop

/* 0x801A6960 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6960(void) {
    /* TODO: match -- 0x30 bytes at 0x801A6960 */
}
#pragma pop

/* 0x801A6990 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6990(void) {
    /* TODO: match -- 0x30 bytes at 0x801A6990 */
}
#pragma pop
