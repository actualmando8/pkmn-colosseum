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
