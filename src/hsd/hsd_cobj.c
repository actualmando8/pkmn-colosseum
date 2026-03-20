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

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 5 functions matched
 * =================================================================== */

extern u32 lbl_8047B234;
extern u32 lbl_8047B240;

/* Forward declarations for converted functions */
void fn_8019513C(void);


/* Address: 0x801942B8 | Size: 0x8 | Pattern: sda_getter */
u32 fn_801942B8(void) {
    return lbl_8047B234;
}

/* Address: 0x801943BC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801943BC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x50]) = val;
}

/* Address: 0x801944C0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801944C0(u8* ptr, f32 val) {
    if (ptr == NULL) { return; }
    *(f32*)(&ptr[0x3C]) = val;
}

/* Address: 0x801944E8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_801944E8(u8* ptr, f32 val) {
    if (ptr == NULL) { return; }
    *(f32*)(&ptr[0x38]) = val;
}

/* Address: 0x8019733C | Size: 0x8 | Pattern: sda_setter */
void fn_8019733C(u32 val) {
    lbl_8047B240 = val;
}

/* =========================================================================
 *  Internal stubs: 0x80193C24-0x80197344 (55 functions)
 * ========================================================================= */

/* 0x80193C24 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80193C24(void) {
    /* TODO: match -- 0xAC bytes at 0x80193C24 */
}
#pragma pop

/* 0x60 | fn_80193CD0 | framed_no_calls */
void fn_80193CD0(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B234, lbl_8047B230 */
}

/* 0x80193D30 | 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80193D30(void) {
    /* TODO: match -- 0x198 bytes at 0x80193D30 */
}
#pragma pop

/* 0x7C | fn_80193EC8 | generic */
u32 fn_80193EC8(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    /* refs: lbl_8036C678 */
    fn_80191628();
    fn_80191628();
    return 0;
}

/* 0x80193F44 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80193F44(void) {
    /* TODO: match -- 0xCC bytes at 0x80193F44 */
}
#pragma pop

/* 0x80194010 | 0x248 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80194010(void) {
    /* TODO: match -- 0x248 bytes at 0x80194010 */
}
#pragma pop

/* 0x60 | fn_80194258 | generic */
void fn_80194258(void) {
    /* refs: lbl_8036C678, lbl_8047B230 */
    fn_80193828();
    fn_80196E10();
}

/* 0x5C | fn_801942C0 | generic */
void fn_801942C0(u32 arg1, u32 arg2) {

}

/* 0x8019431C | 0x3C */
void fn_8019431C(HSD_CObj* cobj, f32* top, f32* bottom) {
    if (cobj == NULL) { return; }
    if (cobj->projection_type != PROJ_PERSPECTIVE) { return; }
    if (top != NULL) {
        *top = cobj->projection_param.perspective.fov;
    }
    if (bottom != NULL) {
        *bottom = cobj->projection_param.perspective.aspect;
    }
}

/* 0x80194358 | 0x24 */
void fn_80194358(HSD_CObj* cobj, f32 top, f32 bottom, f32 left, f32 right) {
    if (cobj == NULL) { return; }
    cobj->projection_type = PROJ_ORTHO;
    cobj->projection_param.ortho.top = top;
    cobj->projection_param.ortho.bottom = bottom;
    cobj->projection_param.ortho.left = left;
    cobj->projection_param.ortho.right = right;
}

/* 0x8019437C | 0x24 */
void fn_8019437C(HSD_CObj* cobj, f32 top, f32 bottom, f32 left, f32 right) {
    if (cobj == NULL) { return; }
    cobj->projection_type = PROJ_FRUSTUM;
    cobj->projection_param.frustum.top = top;
    cobj->projection_param.frustum.bottom = bottom;
    cobj->projection_param.frustum.left = left;
    cobj->projection_param.frustum.right = right;
}

/* 0x801943A0 | 0x1C */
void fn_801943A0(HSD_CObj* cobj, f32 fov, f32 aspect) {
    if (cobj == NULL) { return; }
    cobj->projection_type = PROJ_PERSPECTIVE;
    cobj->projection_param.perspective.fov = fov;
    cobj->projection_param.perspective.aspect = aspect;
}

/* 0x801943CC | 0x18 */
u8 fn_801943CC(HSD_CObj* cobj) {
    if (cobj == NULL) { return 1; }
    return cobj->projection_type;
}

/* 0x801943E4 | 0x1C */
void fn_801943E4(HSD_CObj* cobj, f32 xmin, f32 xmax, f32 ymin, f32 ymax) {
    if (cobj == NULL) { return; }
    cobj->viewport.xmin = xmin;
    cobj->viewport.xmax = xmax;
    cobj->viewport.ymin = ymin;
    cobj->viewport.ymax = ymax;
}

/* 0x80194400 | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80194400(void) {
    /* TODO: match -- 0xA4 bytes at 0x80194400 */
}
#pragma pop

/* 0x801944A4 | 0x1C */
void fn_801944A4(HSD_CObj* cobj, u16 left, u16 right, u16 top, u16 bottom) {
    if (cobj == NULL) { return; }
    cobj->scissor.left = left;
    cobj->scissor.right = right;
    cobj->scissor.top = top;
    cobj->scissor.bottom = bottom;
}

/* 0x801944D0 | 0x18 */
f32 fn_801944D0(HSD_CObj* cobj) {
    if (cobj == NULL) { return 0.0f; }
    return cobj->far;
}

/* 0x801944F8 | 0x18 */
f32 fn_801944F8(HSD_CObj* cobj) {
    if (cobj == NULL) { return 0.0f; }
    return cobj->near;
}

/* 0x80194510 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80194510(void) {
    /* TODO: match -- 0xA0 bytes at 0x80194510 */
}
#pragma pop

/* 0x801945B0 | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801945B0(void) {
    /* TODO: match -- 0xA4 bytes at 0x801945B0 */
}
#pragma pop

/* 0x80194654 | 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80194654(void) {
    /* TODO: match -- 0x9C bytes at 0x80194654 */
}
#pragma pop

/* 0x801946F0 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801946F0(void) {
    /* TODO: match -- 0x98 bytes at 0x801946F0 */
}
#pragma pop

/* 0x80194788 | 0x20 */
void fn_80194788(HSD_CObj* cobj, f32 aspect) {
    if (cobj == NULL) { return; }
    if (cobj->projection_type != PROJ_PERSPECTIVE) { return; }
    cobj->projection_param.perspective.aspect = aspect;
}

/* 0x801947A8 | 0x20 */
void fn_801947A8(HSD_CObj* cobj, f32 fov) {
    if (cobj == NULL) { return; }
    if (cobj->projection_type != PROJ_PERSPECTIVE) { return; }
    cobj->projection_param.perspective.fov = fov;
}

/* 0x801947C8 | 0x464 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801947C8(void) {
    /* TODO: match -- 0x464 bytes at 0x801947C8 */
}
#pragma pop

/* 0x80194C2C | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80194C2C(void) {
    /* TODO: match -- 0x98 bytes at 0x80194C2C */
}
#pragma pop

/* 0x80194CC4 | 0x30 */
extern void fn_80195F0C(HSD_CObj*);
f32* fn_80194CC4(HSD_CObj* cobj) {
    fn_80195F0C(cobj);
    return (f32*)cobj->view_mtx;
}

/* 0x6C | fn_80194CF4 | generic */
void fn_80194CF4(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_801A8524();
    fn_800A2EB4();
}

/* 0x80194D60 | 0x34 */
extern void fn_800A2D64(void*, void*);
void fn_80194D60(HSD_CObj* cobj, void* b) {
    f32* mtx = fn_80194CC4(cobj);
    fn_800A2D64(mtx, b);
}

/* 0x80194D94 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80194D94(void) {
    /* TODO: match -- 0x10 bytes at 0x80194D94 */
}
#pragma pop

/* 0x80194DA4 | 0x32C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80194DA4(void) {
    /* TODO: match -- 0x32C bytes at 0x80194DA4 */
}
#pragma pop

/* 0x6C | fn_801950D0 | guarded_call */
u32 fn_801950D0(void) {
    if (1 /* guard r4 != 0 */) { return 0; }
    fn_8019513C();
    return 1;
}

/* 0x8019513C | 0x454 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019513C(void) {
    /* TODO: match -- 0x454 bytes at 0x8019513C */
}
#pragma pop

/* 0x80195590 | 0x204 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80195590(void) {
    /* TODO: match -- 0x204 bytes at 0x80195590 */
}
#pragma pop

/* 0x80195794 | 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80195794(void) {
    /* TODO: match -- 0x104 bytes at 0x80195794 */
}
#pragma pop

/* 0x6C | fn_80195898 | generic */
void fn_80195898(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80196E10();
    fn_80196E10();
    fn_80191788();
}

/* 0x6C | fn_80195904 | generic */
void fn_80195904(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80196E10();
    fn_80196E10();
    fn_80191688();
}

/* 0x6C | fn_80195970 | generic */
void fn_80195970(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80196E10();
    fn_80196E10();
    fn_80191788();
}

/* 0x6C | fn_801959DC | generic */
void fn_801959DC(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80196E10();
    fn_80196E10();
    fn_80191688();
}

/* 0x80195A48 | 0x24 */
extern void fn_801975FC(void);
extern void fn_801974A8(void);
void fn_80195A48(void) {
    fn_801975FC();
    fn_801974A8();
}

/* 0x80195A6C | 0x4A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80195A6C(void) {
    /* TODO: match -- 0x4A0 bytes at 0x80195A6C */
}
#pragma pop

/* 0x80195F0C | 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80195F0C(void) {
    /* TODO: match -- 0x1B8 bytes at 0x80195F0C */
}
#pragma pop

/* 0x801960C4 | 0x31C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801960C4(void) {
    /* TODO: match -- 0x31C bytes at 0x801960C4 */
}
#pragma pop

/* 0x801963E0 | 0x2B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801963E0(void) {
    /* TODO: match -- 0x2B8 bytes at 0x801963E0 */
}
#pragma pop

/* 0x64 | fn_80196698 | generic */
void fn_80196698(void) {
    fn_801C29C4();
    fn_80191E38();
    fn_80191E38();
}

/* 0x50 | fn_801966FC | generic */
void fn_801966FC(void) {
    fn_801C27F4();
    fn_801919EC();
    fn_801919EC();
}

/* 0x8019674C | 0x3C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019674C(void) {
    /* TODO: match -- 0x3C4 bytes at 0x8019674C */
}
#pragma pop

/* 0x80196B10 | 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80196B10(void) {
    /* TODO: match -- 0xA8 bytes at 0x80196B10 */
}
#pragma pop

/* 0x80196BB8 | 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80196BB8(void) {
    /* TODO: match -- 0x84 bytes at 0x80196BB8 */
}
#pragma pop

/* 0x80196C3C | 0x18 */
extern void fn_80196C54(void);
extern u32 lbl_80478C58;
void fn_80196C3C(void* callback) {
    if (callback == NULL) {
        callback = (void*)fn_80196C54;
    }
    lbl_80478C58 = (u32)callback;
}

/* 0x80196C54 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80196C54(void) {
    /* TODO: match -- 0x8C bytes at 0x80196C54 */
}
#pragma pop

/* 0x80196CE0 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80196CE0(void) {
    /* TODO: match -- 0x98 bytes at 0x80196CE0 */
}
#pragma pop

/* 0x44 | fn_80196EB4 | generic */
void fn_80196EB4(void) {
    /* refs: lbl_80478C64, lbl_80478C68, lbl_80478C6C, lbl_8047B24C, lbl_8047B250, lbl_8047B254, lbl_8047B258, lbl_8047B25C */
}

/* 0x80196EF8 | 0x424 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80196EF8(void) {
    /* TODO: match -- 0x424 bytes at 0x80196EF8 */
}
#pragma pop

/* 0x8019731C | 0x20 */
extern u8 lbl_80478C60;
void fn_8019731C(u8 a, u8 b, u8 c, u8 d) {
    (&lbl_80478C60)[0] = a;
    (&lbl_80478C60)[1] = b;
    (&lbl_80478C60)[2] = c;
    (&lbl_80478C60)[3] = d;
}
