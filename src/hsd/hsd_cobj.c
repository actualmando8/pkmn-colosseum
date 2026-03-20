/**
 * @file hsd_cobj.c
 * @brief HSD CObj - Camera object implementation.
 *
 * Colosseum address: 0x80193C24 (HSD_CObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/cobj.c
 */

#include "hsd/hsd_cobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_wobj.h"

static HSD_ClassInfo* default_class;
static HSD_CObj* current;

static void CObjInfoInit(void);

HSD_CObjInfo hsdCObj = { CObjInfoInit };

/* ========================================================================= */
/*  Accessors                                                                */
/* ========================================================================= */

HSD_CObj* HSD_CObjGetCurrent(void)
{
    return current;
}

u32 HSD_CObjGetFlags(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->flags;
}

void HSD_CObjSetFlags(HSD_CObj* cobj, u32 flags)
{
    HSD_ASSERT(0, cobj);
    cobj->flags |= flags;
}

void HSD_CObjClearFlags(HSD_CObj* cobj, u32 flags)
{
    HSD_ASSERT(0, cobj);
    cobj->flags &= ~flags;
}

int HSD_CObjGetProjectionType(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->projection_type;
}

void HSD_CObjSetProjectionType(HSD_CObj* cobj, u32 type)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_type = (u8) type;
}

f32 HSD_CObjGetFov(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->projection_param.perspective.fov;
}

void HSD_CObjSetFov(HSD_CObj* cobj, f32 fov)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_param.perspective.fov = fov;
}

f32 HSD_CObjGetAspect(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->projection_param.perspective.aspect;
}

void HSD_CObjSetAspect(HSD_CObj* cobj, f32 aspect)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_param.perspective.aspect = aspect;
}

f32 HSD_CObjGetNear(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->near;
}

void HSD_CObjSetNear(HSD_CObj* cobj, f32 near)
{
    HSD_ASSERT(0, cobj);
    cobj->near = near;
}

f32 HSD_CObjGetFar(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->far;
}

void HSD_CObjSetFar(HSD_CObj* cobj, f32 far)
{
    HSD_ASSERT(0, cobj);
    cobj->far = far;
}

/* ========================================================================= */
/*  WObj accessors                                                           */
/* ========================================================================= */

HSD_WObj* HSD_CObjGetEyePositionWObj(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->eyepos;
}

void HSD_CObjSetEyePositionWObj(HSD_CObj* cobj, HSD_WObj* eyepos)
{
    HSD_ASSERT(0, cobj);
    if (cobj->eyepos == eyepos) {
        return;
    }
    ref_INC(eyepos);
    HSD_WObjUnref(cobj->eyepos);
    cobj->eyepos = eyepos;
}

HSD_WObj* HSD_CObjGetInterestWObj(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->interest;
}

void HSD_CObjSetInterestWObj(HSD_CObj* cobj, HSD_WObj* interest)
{
    HSD_ASSERT(0, cobj);
    if (cobj->interest == interest) {
        return;
    }
    ref_INC(interest);
    HSD_WObjUnref(cobj->interest);
    cobj->interest = interest;
}

/* ========================================================================= */
/*  Perspective / frustum / ortho setup                                      */
/* ========================================================================= */

void HSD_CObjSetPerspective(HSD_CObj* cobj, f32 fov, f32 aspect)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_type = PROJ_PERSPECTIVE;
    cobj->projection_param.perspective.fov = fov;
    cobj->projection_param.perspective.aspect = aspect;
}

void HSD_CObjSetFrustum(HSD_CObj* cobj, f32 top, f32 bottom,
                        f32 left, f32 right)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_type = PROJ_FRUSTUM;
    cobj->projection_param.frustum.top = top;
    cobj->projection_param.frustum.bottom = bottom;
    cobj->projection_param.frustum.left = left;
    cobj->projection_param.frustum.right = right;
}

void HSD_CObjSetOrtho(HSD_CObj* cobj, f32 top, f32 bottom,
                      f32 left, f32 right)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_type = PROJ_ORTHO;
    cobj->projection_param.ortho.top = top;
    cobj->projection_param.ortho.bottom = bottom;
    cobj->projection_param.ortho.left = left;
    cobj->projection_param.ortho.right = right;
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_CObjRemoveAnim(HSD_CObj* cobj)
{
    if (cobj != NULL) {
        HSD_AObjRemove(cobj->aobj);
        cobj->aobj = NULL;
        HSD_WObjRemoveAnim(cobj->eyepos);
        HSD_WObjRemoveAnim(cobj->interest);
    }
}

void HSD_CObjAddAnim(HSD_CObj* cobj, HSD_CameraAnim* canim)
{
    if (cobj == NULL || canim == NULL) {
        return;
    }
    if (cobj->aobj != NULL) {
        HSD_AObjRemove(cobj->aobj);
    }
    cobj->aobj = HSD_AObjLoadDesc(canim->aobjdesc);
    HSD_WObjAddAnim(cobj->eyepos, canim->eye_anim);
    HSD_WObjAddAnim(cobj->interest, canim->interest_anim);
}

void HSD_CObjReqAnim(HSD_CObj* cobj, f32 startframe)
{
    if (cobj != NULL) {
        HSD_AObjReqAnim(cobj->aobj, startframe);
        HSD_WObjReqAnim(cobj->eyepos, startframe);
        HSD_WObjReqAnim(cobj->interest, startframe);
    }
}

void HSD_CObjAnim(HSD_CObj* cobj)
{
    if (cobj != NULL) {
        HSD_WObjInterpretAnim(cobj->eyepos);
        HSD_WObjInterpretAnim(cobj->interest);
    }
}

/* ========================================================================= */
/*  Alloc                                                                    */
/* ========================================================================= */

HSD_CObj* HSD_CObjAlloc(void)
{
    HSD_CObj* cobj;
    cobj = (HSD_CObj*) hsdNew(
        default_class ? default_class : &hsdCObj.parent.parent);
    HSD_ASSERT(0, cobj);
    return cobj;
}

void HSD_CObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        HSD_ASSERT(0, hsdIsDescendantOf(info, &hsdCObj));
    }
    default_class = info;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void CObjRelease(HSD_Class* o)
{
    HSD_CObj* cobj = (HSD_CObj*) o;
    HSD_WObjUnref(cobj->eyepos);
    HSD_WObjUnref(cobj->interest);
    HSD_AObjRemove(cobj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdCObj)->release(o);
}

static void CObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    current = NULL;
    HSD_OBJECT_PARENT_INFO(&hsdCObj)->amnesia(info);
}

static void CObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdCObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_cobj",
                     sizeof(HSD_CObjInfo), sizeof(HSD_CObj));
    HSD_CLASS_INFO(&hsdCObj)->release = CObjRelease;
    HSD_CLASS_INFO(&hsdCObj)->amnesia = CObjAmnesia;
}
