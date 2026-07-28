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
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_wobj.h"

/* renamed symbols referenced by asm incs (symbolmap port) */
extern void C_MTXLookAt();
extern void C_MTXPerspective();
extern void OSFillFPUContext();
extern int setupTopHalfCamera();   /* wrk7: was `void` asm-wrapper decl; typed-C returns int */
extern void CObjUpdateFunc(HSD_CObj*, u32, f32*);
extern int CObjInit(HSD_CObj*);
extern void CObjRelease(HSD_CObj*);
extern void CObjAmnesia(HSD_ClassInfo*);
extern int CObjLoad(HSD_CObj*, HSD_CObjDesc*);
extern void* fn_80193828(void*);
extern void fn_800A3874(f32*, f32, f32, f32, f32, f32, f32);
extern void fn_800A39E0(f32*, f32, f32, f32, f32, f32, f32);
extern void fn_800BD2E0(f32*, int);
extern void fn_800BD7A0(int, int, int, int);
extern int fn_800C46B0(f32);
extern char lbl_80465080[];

#if !defined(HSD_COBJ_SPLIT)
static HSD_ClassInfo* default_class;
static HSD_CObj* current;

void fn_80193C24(void);

HSD_CObjInfo hsdCObj = { fn_80193C24 };

/* ========================================================================= */
/*  Accessors                                                                */
/* ========================================================================= */

HSD_CObj* HSD_CObjGetCurrent_Early(void)
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

int HSD_CObjGetProjectionType_Early(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->projection_type;
}

void HSD_CObjSetProjectionType_Early(HSD_CObj* cobj, u32 type)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_type = (u8) type;
}

f32 HSD_CObjGetFov(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->projection_param.perspective.fov;
}

void HSD_CObjSetFov_Early(HSD_CObj* cobj, f32 fov)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_param.perspective.fov = fov;
}

f32 HSD_CObjGetAspect(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->projection_param.perspective.aspect;
}

void HSD_CObjSetAspect_Early(HSD_CObj* cobj, f32 aspect)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_param.perspective.aspect = aspect;
}

f32 HSD_CObjGetNear_Early(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->near;
}

void HSD_CObjSetNear_Early(HSD_CObj* cobj, f32 near)
{
    HSD_ASSERT(0, cobj);
    cobj->near = near;
}

f32 HSD_CObjGetFar_Early(HSD_CObj* cobj)
{
    HSD_ASSERT(0, cobj);
    return cobj->far;
}

void HSD_CObjSetFar_Early(HSD_CObj* cobj, f32 far)
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

void HSD_CObjSetPerspective_Early(HSD_CObj* cobj, f32 fov, f32 aspect)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_type = PROJ_PERSPECTIVE;
    cobj->projection_param.perspective.fov = fov;
    cobj->projection_param.perspective.aspect = aspect;
}

void HSD_CObjSetFrustum_Early(HSD_CObj* cobj, f32 top, f32 bottom,
                              f32 left, f32 right)
{
    HSD_ASSERT(0, cobj);
    cobj->projection_type = PROJ_FRUSTUM;
    cobj->projection_param.frustum.top = top;
    cobj->projection_param.frustum.bottom = bottom;
    cobj->projection_param.frustum.left = left;
    cobj->projection_param.frustum.right = right;
}

void HSD_CObjSetOrtho_Early(HSD_CObj* cobj, f32 top, f32 bottom,
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

void HSD_CObjRemoveAnim_Early(HSD_CObj* cobj)
{
    if (cobj != NULL) {
        HSD_AObjRemove(cobj->aobj);
        cobj->aobj = NULL;
        HSD_WObjRemoveAnim(cobj->eyepos);
        HSD_WObjRemoveAnim(cobj->interest);
    }
}

void HSD_CObjAddAnim_Early(HSD_CObj* cobj, HSD_CameraAnim* canim)
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

void HSD_CObjReqAnim_Early(HSD_CObj* cobj, f32 startframe)
{
    if (cobj != NULL) {
        HSD_AObjReqAnim(cobj->aobj, startframe);
        HSD_WObjReqAnim(cobj->eyepos, startframe);
        HSD_WObjReqAnim(cobj->interest, startframe);
    }
}

void HSD_CObjAnim_Early(HSD_CObj* cobj)
{
    if (cobj != NULL) {
        HSD_WObjInterpretAnim(cobj->eyepos);
        HSD_WObjInterpretAnim(cobj->interest);
    }
}

/* ========================================================================= */
/*  Alloc                                                                    */
/* ========================================================================= */

HSD_CObj* HSD_CObjAlloc_Early(void)
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

static void CObjRelease_Early(HSD_Class* o)
{
    HSD_CObj* cobj = (HSD_CObj*) o;
    HSD_WObjUnref(cobj->eyepos);
    HSD_WObjUnref(cobj->interest);
    HSD_AObjRemove(cobj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdCObj)->release(o);
}

static void CObjAmnesia_Early(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    current = NULL;
    HSD_OBJECT_PARENT_INFO(&hsdCObj)->amnesia(info);
}
#endif /* !HSD_COBJ_SPLIT */

/* 0x80193C24 | 0xAC - CObjInfoInit */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80193C24)
void fn_80193C24(void)
{
    extern HSD_CObjInfo lbl_8036C678;
    extern char lbl_80274628[];
    extern char lbl_80274640[];

    hsdInitClassInfo(HSD_CLASS_INFO(&lbl_8036C678), &hsdObj,
                     lbl_80274628, lbl_80274640, sizeof(HSD_CObjInfo),
                     sizeof(HSD_CObj));
    HSD_CLASS_INFO(&lbl_8036C678)->init =
        (int (*)(HSD_Class*)) CObjInit;
    HSD_CLASS_INFO(&lbl_8036C678)->release =
        (void (*)(HSD_Class*)) CObjRelease;
    HSD_CLASS_INFO(&lbl_8036C678)->amnesia = CObjAmnesia;
    HSD_COBJ_INFO(&lbl_8036C678)->load = CObjLoad;
    HSD_COBJ_INFO(&lbl_8036C678)->update =
        (void (*)(HSD_CObj*, u32, void*)) CObjUpdateFunc;
}
#endif /* HSD_COBJ_CANDIDATE_80193C24 */

/* 0x80193CD0 | 0x60 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_80193CD0)
#if 1
void CObjAmnesia(HSD_ClassInfo* info) {
    extern HSD_ClassInfo* lbl_8047B230;
    extern HSD_CObj* lbl_8047B234;
    extern HSD_CObjInfo lbl_8036C678;

    if (info == lbl_8047B230) {
        lbl_8047B230 = NULL;
    }
    if (info == &lbl_8036C678.parent.parent) {
        lbl_8047B234 = NULL;
    }
    lbl_8036C678.parent.parent.head.parent->amnesia(info);
}
#endif
#endif /* HSD_COBJ_EXACT_80193CD0 */

/* 0x80193D30 | 0x198 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80193D30)
extern void __assert(const char*, u32, const char*);
extern HSD_CObjInfo lbl_8036C678;
extern char lbl_8047D958;
extern char lbl_8047D960;
static inline BOOL ref_DEC_v(void* o)
{
    BOOL ret;
    if ((ret = (HSD_OBJ(o)->ref_count == HSD_OBJ_NOREF))) {
        return ret;
    }
    ret = (*(volatile u16*)&HSD_OBJ(o)->ref_count == 0);
    HSD_OBJ(o)->ref_count--;
    return ret;
}
#if 1
/* decompiled wrk5: functional (TU not byte-measurable) - CObj destructor:
   removes its anim, releases the eye/interest WObjs (ref-counted), frees the
   projection matrix, then chains to the parent class destroy method. */
void CObjRelease(HSD_CObj* cobj) {
    extern void HSD_MtxFree(void*);
    HSD_WObj* eyepos;
    HSD_WObj* interest;

    HSD_AObjRemove(cobj->aobj);
    if (cobj == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
    eyepos = cobj->eyepos;
    if (eyepos != NULL) {
        if (ref_DEC_v(eyepos) && eyepos != NULL) {
            eyepos->parent.parent.class_info->release(&eyepos->parent.parent);
            eyepos->parent.parent.class_info->destroy(&eyepos->parent.parent);
        }
    }
    if (cobj == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
    interest = cobj->interest;
    if (interest != NULL) {
        if (ref_DEC_v(interest) && interest != NULL) {
            interest->parent.parent.class_info->release(&interest->parent.parent);
            interest->parent.parent.class_info->destroy(&interest->parent.parent);
        }
    }
    if (cobj->proj_mtx != NULL) {
        HSD_MtxFree(cobj->proj_mtx);
    }
    lbl_8036C678.parent.parent.head.parent->release(&cobj->parent.parent);
}
#endif

/* 0x80193EC8 | 0x7C */
#if 1
int CObjInit(HSD_CObj* cobj) {
    extern HSD_CObjInfo lbl_8036C678;
    HSD_WObj* interest;
    int result;
    result = HSD_CLASS_INFO(&lbl_8036C678)->head.parent->init((HSD_Class*)cobj);
    if (result < 0) return result;
    if (cobj != NULL) {
        cobj->flags |= 0xC0000000;
    }
    cobj->eyepos = HSD_WObjAlloc();
    interest = HSD_WObjAlloc();
    result = 0;
    cobj->interest = interest;
    return result;
}
#endif

/* 0x80193F44 | 0xCC */
extern HSD_ClassInfo* fn_80193748(const char*);
#if 1
HSD_CObj* HSD_CObjLoadDesc(HSD_CObjDesc* desc) {
    extern HSD_ClassInfo* lbl_8047B230;
    extern HSD_CObjInfo lbl_8036C678;
    extern char lbl_8047D958;
    extern char lbl_8047D960;
    HSD_CObj* cobj;
    HSD_ClassInfo* info;

    if (desc == NULL) {
        goto return_null;
    }

    if (desc->class_name != NULL) {
        info = fn_80193748(desc->class_name);
        if (info != NULL) {
            goto do_alloc_from_info;
        }
    }

    if (lbl_8047B230 != NULL) {
        info = lbl_8047B230;
    } else {
        info = &lbl_8036C678.parent.parent;
    }
    cobj = fn_80193828(info);
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x7a4, &lbl_8047D960);
    }
    goto setup;

do_alloc_from_info:
    cobj = fn_80193828(info);
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x7f9, &lbl_8047D960);
    }
setup:
    HSD_COBJ_METHOD(cobj)->load(cobj, desc);
    return cobj;
return_null:
    return NULL;
}
#endif

/* 0x80194010 | 0x248 */
extern void HSD_WObjInit(HSD_WObj*, HSD_WObjDesc*);
extern void HSD_CObjSetRoll(HSD_CObj* cobj, f32 roll);
extern void HSD_CObjSetUpVector(HSD_CObj* cobj, Vec* up);
extern Vec lbl_8036C6D4; /* default up vector = { 0, 1, 0 } */
#if 1
/* decompiled wrk6: functional (TU not byte-measurable) - CObj class `load`:
   initialises a CObj from its descriptor. Copies flags, viewport (s16->f32),
   scissor and near/far; binds the eye/interest WObjs; sets the orientation
   from either an explicit up vector or a roll angle; then installs the
   projection parameters for perspective/frustum/ortho. Asserts on an unknown
   projection type. Returns 0 (success). The leading null-guards mirror the
   original's inlined HSD_ASSERT checks. */
int CObjLoad(HSD_CObj* cobj, HSD_CObjDesc* desc)
{
    /* flags: net effect is cobj->flags = desc->flags (the original also does a
       redundant top-2-bit-preserving rlwimi merge, a no-op for u16 flags). */
    if (cobj != NULL) {
        cobj->flags = desc->common.flags;
    }
    if (cobj != NULL) {
        cobj->viewport.xmin = (f32) desc->common.viewport.xmin;
        cobj->viewport.xmax = (f32) desc->common.viewport.xmax;
        cobj->viewport.ymin = (f32) desc->common.viewport.ymin;
        cobj->viewport.ymax = (f32) desc->common.viewport.ymax;
    }
    if (cobj != NULL) {
        cobj->scissor = desc->common.scissor;
    }
    HSD_WObjInit(cobj->eyepos, desc->common.eyepos);
    HSD_WObjInit(cobj->interest, desc->common.interest);
    if (cobj != NULL) {
        cobj->near = desc->common.nnear;
    }
    if (cobj != NULL) {
        cobj->far = desc->common.ffar;
    }
    if (desc->common.flags & 1) {
        Vec* up = desc->common.up_vector;
        if (up == NULL) {
            up = &lbl_8036C6D4;
        }
        HSD_CObjSetUpVector(cobj, up);
    } else {
        HSD_CObjSetRoll(cobj, desc->common.roll);
    }
    switch (desc->common.projection_type) {
    case PROJ_PERSPECTIVE:
        if (cobj != NULL) {
            cobj->projection_type = PROJ_PERSPECTIVE;
            cobj->projection_param.perspective.fov = desc->perspective.fov;
            cobj->projection_param.perspective.aspect = desc->perspective.aspect;
        }
        break;
    case PROJ_FRUSTUM:
        if (cobj != NULL) {
            cobj->projection_type = PROJ_FRUSTUM;
            cobj->projection_param.frustum.top = desc->frustum.top;
            cobj->projection_param.frustum.bottom = desc->frustum.bottom;
            cobj->projection_param.frustum.left = desc->frustum.left;
            cobj->projection_param.frustum.right = desc->frustum.right;
        }
        break;
    case PROJ_ORTHO:
        if (cobj != NULL) {
            cobj->projection_type = PROJ_ORTHO;
            cobj->projection_param.ortho.top = desc->ortho.top;
            cobj->projection_param.ortho.bottom = desc->ortho.bottom;
            cobj->projection_param.ortho.left = desc->ortho.left;
            cobj->projection_param.ortho.right = desc->ortho.right;
        }
        break;
    default:
        HSD_ASSERT(2002, 0);
        break;
    }
    return 0;
}
#endif

/* 0x80194258 | 0x60 */
#if 1
HSD_CObj* HSD_CObjAlloc(void) {
    extern HSD_ClassInfo* lbl_8047B230;
    extern HSD_CObjInfo lbl_8036C678;
    extern char lbl_8047D958;
    extern char lbl_8047D960;
    HSD_CObj* result;

    result = fn_80193828(lbl_8047B230 != NULL ? lbl_8047B230 : &lbl_8036C678.parent.parent);
    if (result == NULL) {
        __assert(&lbl_8047D958, 0x7a4, &lbl_8047D960);
    }
    return result;
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80193D30 */

/* 0x801942B8 | 0x8 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_801942B8)
#if 1
HSD_CObj* HSD_CObjGetCurrent(void)
{
    extern HSD_CObj* lbl_8047B234;
    return lbl_8047B234;
}
#endif

/* 0x801942C0 | 0x5C */
#if 1
void HSD_CObjGetOrtho(HSD_CObj* cobj, f32* a, f32* b, f32* c, f32* d) {
    if (cobj == NULL || cobj->projection_type != PROJ_ORTHO) {
        return;
    }
    if (a != NULL) *a = cobj->projection_param.ortho.top;
    if (b != NULL) *b = cobj->projection_param.ortho.bottom;
    if (c != NULL) *c = cobj->projection_param.ortho.left;
    if (d != NULL) *d = cobj->projection_param.ortho.right;
}
#endif

/* 0x8019431C | 0x3C */
#if 1
void HSD_CObjGetPerspective(HSD_CObj* cobj, f32* a, f32* b) {
    if (cobj == NULL || cobj->projection_type != PROJ_PERSPECTIVE) {
        return;
    }
    if (a != NULL) *a = cobj->projection_param.perspective.fov;
    if (b != NULL) *b = cobj->projection_param.perspective.aspect;
}
#endif

/* 0x80194358 | 0x24 */
#if 1
void HSD_CObjSetOrtho(HSD_CObj* cobj, f32 top, f32 bottom, f32 left,
                      f32 right)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_ORTHO;
    cobj->projection_param.ortho.top = top;
    cobj->projection_param.ortho.bottom = bottom;
    cobj->projection_param.ortho.left = left;
    cobj->projection_param.ortho.right = right;
}
#endif

/* 0x8019437C | 0x24 */
#if 1
void HSD_CObjSetFrustum(HSD_CObj* cobj, f32 top, f32 bottom, f32 left,
                        f32 right)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_FRUSTUM;
    cobj->projection_param.frustum.top = top;
    cobj->projection_param.frustum.bottom = bottom;
    cobj->projection_param.frustum.left = left;
    cobj->projection_param.frustum.right = right;
}
#endif

/* 0x801943A0 | 0x1C */
#if 1
void HSD_CObjSetPerspective(HSD_CObj* cobj, f32 fov, f32 aspect)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = PROJ_PERSPECTIVE;
    cobj->projection_param.perspective.fov = fov;
    cobj->projection_param.perspective.aspect = aspect;
}
#endif

/* 0x801943BC | 0x10 */
#if 1
void HSD_CObjSetProjectionType(HSD_CObj* cobj, u32 projection_type)
{
    if (cobj == NULL) {
        return;
    }
    cobj->projection_type = projection_type;
}
#endif

/* 0x801943CC | 0x18 */
#if 1
int HSD_CObjGetProjectionType(HSD_CObj* cobj) {
    if (cobj == NULL) {
        return PROJ_PERSPECTIVE;
    }
    return cobj->projection_type;
}
#endif

/* 0x801943E4 | 0x1C */
#if 1
void HSD_CObjSetViewportfx4(HSD_CObj* cobj, f32 left, f32 right, f32 top,
                            f32 bottom)
{
    if (cobj == NULL) {
        return;
    }
    cobj->viewport.xmin = left;
    cobj->viewport.xmax = right;
    cobj->viewport.ymin = top;
    cobj->viewport.ymax = bottom;
}
#endif
#endif /* HSD_COBJ_EXACT_801942B8 */

/* 0x80194400 | 0xA4 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80194400)
#if 1
/* decompiled wrk4 2026-06-16: functional (TU not byte-measurable).
 * Set the camera viewport from an s16 rect (signed lha + xoris/0x8000 magic
 * => (f32)(s16) on each of the four halfwords). */
void HSD_CObjSetViewport(HSD_CObj* cobj, HSD_RectS16* rect) {
    if (cobj == NULL) {
        return;
    }
    cobj->viewport.xmin = (f32) rect->xmin;
    cobj->viewport.xmax = (f32) rect->xmax;
    cobj->viewport.ymin = (f32) rect->ymin;
    cobj->viewport.ymax = (f32) rect->ymax;
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80194400 */

/* 0x801944A4 | 0x1C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_801944A4)
#if 1
void HSD_CObjSetScissorx4(HSD_CObj* cobj, u16 left, u16 right, u16 top,
                          u16 bottom)
{
    if (cobj == NULL) {
        return;
    }
    cobj->scissor.left = left;
    cobj->scissor.right = right;
    cobj->scissor.top = top;
    cobj->scissor.bottom = bottom;
}
#endif

/* 0x801944C0 | 0x10 */
#if 1
void HSD_CObjSetFar(HSD_CObj* cobj, f32 far)
{
    if (cobj != NULL) {
        cobj->far = far;
    }
}
#endif

/* 0x801944D0 | 0x18 */
#if 1
f32 HSD_CObjGetFar(HSD_CObj* cobj) {
    extern f32 lbl_8047D978;
    if (cobj == NULL) {
        return lbl_8047D978;
    }
    return cobj->far;
}
#endif

/* 0x801944E8 | 0x10 */
#if 1
void HSD_CObjSetNear(HSD_CObj* cobj, f32 near)
{
    if (cobj != NULL) {
        cobj->near = near;
    }
}
#endif

/* 0x801944F8 | 0x18 */
#if 1
f32 HSD_CObjGetNear(HSD_CObj* cobj) {
    extern f32 lbl_8047D978;
    if (cobj == NULL) {
        return lbl_8047D978;
    }
    return cobj->near;
}
#endif
#endif /* HSD_COBJ_EXACT_801944A4 */

/* 0x80194510 | 0xA0 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80194510)
extern double fn_800CE220(f32);
extern f32 lbl_8047D978;
extern f32 lbl_8047D97C;
extern f32 lbl_8047D980;
#if 1
f32 HSD_CObjGetRight(u8* ptr) {
    if (ptr == NULL) {
        return lbl_8047D978;
    }
    switch (ptr[0x50]) {
    case 1:
    {
        f32 inner = *(f32*)(ptr + 0x38) * (f32) fn_800CE220(lbl_8047D97C * (lbl_8047D980 * *(f32*)(ptr + 0x40)));
        f32 x44 = *(f32*)(ptr + 0x44);
        return x44 * inner;
    }
    case 2:
        return *(f32*)(ptr + 0x4C);
    case 3:
        return *(f32*)(ptr + 0x4C);
    default:
        return lbl_8047D978;
    }
}
#endif

/* 0x801945B0 | 0xA4 */
#if 1
/* MATCH 100% (opt1 + case1 split into inner/x44 temps: -x38*tan accumulates in f0, x44 loaded
 * after into f2, final fmuls written x44*inner). */
f32 HSD_CObjGetLeft(u8* ptr) {
    if (ptr == NULL) {
        return lbl_8047D978;
    }
    switch (ptr[0x50]) {
    case 1:
    {
        f32 inner = -*(f32*)(ptr + 0x38) * (f32) fn_800CE220(lbl_8047D97C * (lbl_8047D980 * *(f32*)(ptr + 0x40)));
        f32 x44 = *(f32*)(ptr + 0x44);
        return x44 * inner;
    }
    case 2:
        return *(f32*)(ptr + 0x48);
    case 3:
        return *(f32*)(ptr + 0x48);
    default:
        return lbl_8047D978;
    }
}
#endif

/* 0x80194654 | 0x9C */
#if 1
f32 HSD_CObjGetBottom(u8* ptr) {
    if (ptr == NULL) {
        return lbl_8047D978;
    }
    switch (ptr[0x50]) {
    case 1:
        return -*(f32*)(ptr + 0x38) * (f32) fn_800CE220(lbl_8047D97C * (lbl_8047D980 * *(f32*)(ptr + 0x40)));
    case 2:
        return *(f32*)(ptr + 0x44);
    case 3:
        return *(f32*)(ptr + 0x44);
    default:
        return lbl_8047D978;
    }
}
#endif

/* 0x801946F0 | 0x98 */
#if 1
/* decompiled wrk4 2026-06-16: functional (TU not byte-measurable).
 * Get the top extent of the view frustum. Twin of HSD_CObjGetBottom (which returns
 * the bottom, i.e. -near*tan); this returns +near*tan(D97C * D980 * fov) for
 * perspective, the +0x40 field for frustum/ortho, else the default. */
f32 HSD_CObjGetTop(HSD_CObj* cobj) {
    extern f32 lbl_8047D978; /* default (no/invalid camera) */
    extern f32 lbl_8047D97C; /* deg->rad scale            */
    extern f32 lbl_8047D980; /* 0.5 (half-fov)            */
    extern double fn_800CE220(f32); /* tan */
    if (cobj == NULL) {
        return lbl_8047D978;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return cobj->near *
               (f32) fn_800CE220(lbl_8047D97C *
                                 (lbl_8047D980 *
                                  cobj->projection_param.perspective.fov));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.top;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.top;
    default:
        return lbl_8047D978;
    }
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80194510 */

/* 0x80194788 | 0x20 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_80194788)
#if 1
void HSD_CObjSetAspect(HSD_CObj* cobj, f32 val)
{
    if (cobj == NULL || cobj->projection_type != 1) {
        return;
    }
    cobj->projection_param.perspective.aspect = val;
}
#endif

/* 0x801947A8 | 0x20 */
void HSD_CObjSetFov(HSD_CObj* cobj, f32 val)
{
    if (cobj == NULL || cobj->projection_type != PROJ_PERSPECTIVE) {
        return;
    }
    cobj->projection_param.perspective.fov = val;
}
#endif /* HSD_COBJ_EXACT_80194788 */

/* HSD_CObjGetEyePosition / HSD_CObjGetInterest are defined here, ahead of the
 * up-vector cluster, because roll2upvec / upvec2roll / HSD_CObjSetUpVector
 * inline them. */
extern char lbl_8047D958;
extern char lbl_8047D960;
extern void HSD_WObjGetPosition(HSD_WObj* wobj, Vec* position);

static inline HSD_WObj* cobj_get_eye_position_wobj(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x2E8, &lbl_8047D960);
    }
    return cobj->eyepos;
}

static inline HSD_WObj* cobj_get_interest_wobj(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x2D0, &lbl_8047D960);
    }
    return cobj->interest;
}

static inline void cobj_get_eye_position(HSD_CObj* cobj, Vec* position)
{
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x318, &lbl_8047D960);
    }
    HSD_WObjGetPosition(cobj_get_eye_position_wobj(cobj), position);
}

static inline void cobj_set_eye_position(HSD_CObj* cobj, Vec* position)
{
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x324, &lbl_8047D960);
    }
    HSD_WObjSetPosition(cobj_get_eye_position_wobj(cobj), position);
}

static inline void cobj_get_interest(HSD_CObj* cobj, Vec* position)
{
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x300, &lbl_8047D960);
    }
    HSD_WObjGetPosition(cobj_get_interest_wobj(cobj), position);
}

static inline void cobj_set_interest(HSD_CObj* cobj, Vec* position)
{
    if (cobj == NULL) {
        __assert(&lbl_8047D958, 0x30C, &lbl_8047D960);
    }
    HSD_WObjSetPosition(cobj_get_interest_wobj(cobj), position);
}

static inline void cobj_clear_flags(HSD_CObj* cobj, u32 flags)
{
    if (cobj != NULL) {
        cobj->flags &= ~flags;
    }
}

/* 0x80195904 | 0x6C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_GET_EYE_POSITION)
void HSD_CObjGetEyePosition(HSD_CObj* cobj, Vec* position)
{
    cobj_get_eye_position(cobj, position);
}
#endif /* HSD_COBJ_EXACT_GET_EYE_POSITION */

/* 0x801959DC | 0x6C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_GET_INTEREST)
void HSD_CObjGetInterest(HSD_CObj* cobj, Vec* interest)
{
    cobj_get_interest(cobj, interest);
}
#endif /* HSD_COBJ_EXACT_GET_INTEREST */

/* ========================================================================= */
/*  Up-vector / roll cluster - shared declarations                           */
/*                                                                           */
/*  The original HSD sources (cf. doldecomp/melee sysdolphin/baselib/cobj.c) */
/*  work on real `Vec` values and pull `vec_normalize_check` / `sqrtf` in as  */
/*  inlines from util.h / <math.h>. Reproducing that shape is what makes the  */
/*  codegen line up.                                                          */
/* ========================================================================= */

/* .sdata floats. These are addressed absolutely (lis/@ha + @l), not through  */
/* the SDA base, so they must be declared as arrays - exactly as the matched   */
/* crt/math_range_800CAA58.c does for lbl_80478AC0.                            */
extern const f32 lbl_80478AC0[]; /* NaN                                       */
extern const f32 lbl_80478AC8[]; /* FLT_MIN (degenerate-vector epsilon)       */

/* .sdata2 literal pool for this TU. */
extern f32 lbl_8047D978; /* 0.0f   */
extern f64 lbl_8047D988; /* 1.0    */
extern f64 lbl_8047D990; /* 0.0001 */
extern f64 lbl_8047D998; /* 0.5    */
extern f64 lbl_8047D9A0; /* 3.0    */
extern f64 lbl_8047D9A8; /* 0.0    */

/* .data: the reference frame used by upvec2roll, and CObjLoad's default up. */
extern Vec lbl_8036C6BC; /* orig = { 0, 0, 0 } */
extern Vec lbl_8036C6C8; /* uy   = { 0, 1, 0 } */
extern Vec lbl_8036C6D4; /* uy2  = { 0, 1, 0 } */

extern void OSReport(const char* fmt, ...);
extern void PSVECSubtract(void*, void*, void*);
extern void PSVECNormalize(Vec* src, Vec* dst);
extern f32 PSVECDotProduct(Vec* a, Vec* b);
extern f32 PSVECMag(Vec* v);
extern void PSMTXMultVecSR(f32 m[3][4], Vec* src, Vec* dst);
extern void PSMTXRotAxisRad(f32 m[3][4], Vec* axis, f32 rad);
extern void HSD_CObjSetMtxDirty(HSD_CObj*);
extern void HSD_CObjGetEyePosition(HSD_CObj*, Vec*);
extern void HSD_CObjGetInterest(HSD_CObj*, Vec*);
#if defined(HSD_COBJ_SPLIT) && defined(HSD_COBJ_CANDIDATE_80194DA4)
/* util.h supplies this helper inline in the original HSD translation unit. */
static inline int vec_normalize_check(Vec* src, Vec* dst)
{
    if (!src || !dst) {
        return -1;
    }
    if (__fabs(src->x) <= lbl_80478AC8[0] &&
        __fabs(src->y) <= lbl_80478AC8[0] &&
        __fabs(src->z) <= lbl_80478AC8[0])
    {
        return -1;
    }
    PSVECNormalize(src, dst);
    return 0;
}
#else
extern int vec_normalize_check(Vec* src, Vec* dst);
#endif
extern int roll2upvec(HSD_CObj* cobj, Vec* up, f32 roll);
extern f32 upvec2roll(HSD_CObj* cobj, Vec* up);

typedef union CObjFloatShape {
    f32 value;
    u32 bits;
} CObjFloatShape;

/* MSL <math.h> inline sqrtf: frsqrte + 3 Newton steps, then the NaN/zero
 * fallbacks. Byte-identical in shape to the out-of-line copy in
 * src/crt/math_range_800CAA58.c; the original TU got this inlined from the
 * header, so we must too. */
static inline f32 cobj_sqrtf(f32 x)
{
    CObjFloatShape shape;
    f64 y;
    s32 exp;
    s32 fpclass;

    if (x > lbl_8047D978) {
        y = __frsqrte(x);
        y = lbl_8047D998 * y * (lbl_8047D9A0 - x * (y * y));
        y = lbl_8047D998 * y * (lbl_8047D9A0 - x * (y * y));
        y = lbl_8047D998 * y * (lbl_8047D9A0 - x * (y * y));
        return (f32) (x * y);
    }
    if ((f64) x < lbl_8047D9A8) {
        return lbl_80478AC0[0];
    }
    shape.value = x;
    exp = shape.bits & 0x7F800000;
    switch (exp) {
    case 0x7F800000:
        if ((shape.bits & 0x007FFFFF) != 0) {
            fpclass = 1;
        } else {
            fpclass = 2;
        }
        break;
    case 0:
        if ((shape.bits & 0x007FFFFF) != 0) {
            fpclass = 5;
        } else {
            fpclass = 3;
        }
        break;
    default:
        fpclass = 4;
        break;
    }
    if (fpclass == 1) {
        return lbl_80478AC0[0];
    }
    return x;
}

/* __fabsf(*v) widened to f64 - matches melee's cobj_fabsf_p (fabs + frsp). */
static inline f64 cobj_fabsf_p(f32* v)
{
    return (f32) __fabs(*v);
}

/* melee's HSD_CObjGetEyeVector, as inlined by this TU: normalised
 * eye->interest direction. Returns 1 on success, 0 if unusable. */
static inline int cobj_get_eye_vector(HSD_CObj* cobj, Vec* eye)
{
    Vec eyepos;
    Vec interest;

    if (!cobj || !cobj->eyepos || !cobj->interest) {
        return 0;
    }
    cobj_get_eye_position(cobj, &eyepos);
    cobj_get_interest(cobj, &interest);
    PSVECSubtract(&interest, &eyepos, eye);
    return vec_normalize_check(eye, eye) == 0;
}

/* 0x80194C2C | 0x98 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_801947C8)
int vec_normalize_check(Vec* vec, Vec* out)
{
    if (!vec || !out) {
        return -1;
    }
    if (__fabs(vec->x) <= lbl_80478AC8[0] &&
        __fabs(vec->y) <= lbl_80478AC8[0] &&
        __fabs(vec->z) <= lbl_80478AC8[0])
    {
        return -1;
    }
    PSVECNormalize(vec, out);
    return 0;
}
#endif /* HSD_COBJ_CANDIDATE_801947C8 */

/* vec_normalize_check (0x80194C2C | 0x98) is defined above, before its first
 * inline use. */

/* WP-0061 external references */
extern void fn_801C25E4(void);
extern void fn_801C2670(void);
extern void fn_801C27F4(void*, void*, void*);
extern void fn_801C29C4(void*, f32);
/* WP-0061 forward declarations (defined later in same TU) */

/* 0x80194CC4 | 0x30 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_80194CC4)
extern void HSD_CObjSetupViewingMtx(HSD_CObj*);
#if 1
f32* HSD_CObjGetViewingMtxPtr(HSD_CObj* cobj)
{
    HSD_CObjSetupViewingMtx(cobj);
    return cobj->view_mtx[0];
}
#endif

/* 0x80194CF4 | 0x6C */
extern f32* HSD_MtxAlloc(void);
extern void PSMTXInverse(f32*, f32*);
#if 1
f32* HSD_CObjGetInvViewingMtxPtrDirect(HSD_CObj* cobj) {
    if (cobj->flags & 0x80000000) {
        if (cobj->proj_mtx == NULL) {
            cobj->proj_mtx = HSD_MtxAlloc();
        }
        PSMTXInverse(cobj->view_mtx[0], cobj->proj_mtx);
        cobj_clear_flags(cobj, 1U << 31);
    }
    return cobj->proj_mtx;
}
#endif
#endif /* HSD_COBJ_EXACT_80194CC4 */

/* 0x80194D60 | 0x34 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80194D60)
extern void PSMTXCopy(f32*, f32*);
#if 1
void HSD_CObjGetViewingMtx(HSD_CObj* cobj, f32 mtx[3][4]) {
    PSMTXCopy(HSD_CObjGetViewingMtxPtr(cobj), mtx[0]);
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80194D60 */

/* 0x80194D94 | 0x10 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_80194D94)
#if 1
void HSD_CObjSetMtxDirty(HSD_CObj* cobj)
{
    cobj->flags |= (1U << 30) | (1U << 31);
}
#endif
#endif /* HSD_COBJ_EXACT_80194D94 */

/* 0x801950D0 | 0x6C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80194DA4)
#if 1
/* Returns the camera's up vector: the stored one when the CObj holds an
   explicit up vector (flag bit 0), otherwise the one derived from the stored
   roll angle. 1 = got a vector, 0 = nothing to give. */
int HSD_CObjGetUpVector(HSD_CObj* cobj, Vec* up)
{
    if (cobj == NULL || up == NULL) {
        return 0;
    }
    if (cobj->flags & 1) {
        *up = cobj->u.up;
        return 1;
    }
    return roll2upvec(cobj, up, cobj->u.roll);
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80194DA4 */

/* 0x8019513C | 0x454 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80194DA4)
#if 1
/* Derives the camera up-vector that corresponds to a roll angle: take the
   eye->interest direction, Gram-Schmidt a world axis against it (choosing the
   axis that avoids gimbal lock near vertical), rotate that about the direction
   by -roll and normalise it into `up`. Returns 1 on success, 0 if the
   eye->interest direction is degenerate. */
int roll2upvec(HSD_CObj* cobj, Vec* up, f32 roll)
{
    Vec eye;
    Vec v0;
    Vec v1;
    f32 m[3][4];

    if (!cobj_get_eye_vector(cobj, &eye)) {
        return 0;
    }

    if (lbl_8047D988 - cobj_fabsf_p(&eye.y) < lbl_8047D990) {
        /* near-vertical: Gram-Schmidt the world x axis against eye */
        v0.x = cobj_sqrtf(eye.y * eye.y + eye.z * eye.z);
        v0.y = eye.y * (-eye.x / v0.x);
        v0.z = eye.z * (-eye.x / v0.x);
    } else {
        /* otherwise Gram-Schmidt the world y axis against eye */
        v0.y = cobj_sqrtf(eye.x * eye.x + eye.z * eye.z);
        v0.x = eye.x * (-eye.y / v0.y);
        v0.z = eye.z * (-eye.y / v0.y);
    }

    PSMTXRotAxisRad(m, &eye, -roll);
    PSMTXMultVecSR(m, &v0, &v1);
    PSVECNormalize(&v1, up);
    return 1;
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80194DA4 */

/* 0x80195590 | 0x204 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80194DA4)
extern f64 atan2();
extern f32 lbl_8047D9B0; /* 1.0f      */
extern f32 lbl_8047D9B4; /*  PI/2     */
extern f32 lbl_8047D9B8; /* -PI/2     */
extern char lbl_8047D958;
extern char lbl_8047D960;
#if 1
/* The inverse of roll2upvec: the roll (twist) angle of `up` about the camera's
   eye->interest direction. Returns 0 when the direction is degenerate or `up`
   is nearly parallel to it. */
f32 upvec2roll(HSD_CObj* cobj, Vec* up)
{
    Vec v;
    Vec eye;
    f32 vmtx[3][4];
    f32 dot;

    if (!cobj_get_eye_vector(cobj, &eye)) {
        return lbl_8047D978;
    }

    dot = __fabs(PSVECDotProduct(up, &eye));
    if (lbl_8047D9B0 - dot < lbl_80478AC8[0]) {
        return lbl_8047D978;
    }

    C_MTXLookAt(vmtx, &lbl_8036C6BC, &lbl_8036C6C8, &eye);
    PSMTXMultVecSR(vmtx, up, &v);
    {
        f32 x = v.x;
        f32 y = v.y;
        f32 neg_x = -x;

        if (lbl_8047D978 == y) {
            return (neg_x >= lbl_8047D978) ? lbl_8047D9B4 : lbl_8047D9B8;
        }
        return (f32) atan2(neg_x, y);
    }
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80194DA4 */

/* 0x80194DA4 | 0x32C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80194DA4)
extern char lbl_8027464C[]; /* "cobj up vector is zero vector" (OSReport fmt) */
extern char lbl_8047D968;   /* __assert message for the up-vector path        */
/* Storage twin of upvec2roll. With an explicit up vector (flag bit 0) the
   normalised `up` is stored, marking the matrices dirty when it changed; a
   degenerate `up` warns and asserts. Otherwise the vector is converted to a
   roll angle and committed through HSD_CObjSetRoll. */
void HSD_CObjSetUpVector(HSD_CObj* cobj, Vec* up)
{
    Vec v;

    if (cobj == NULL || up == NULL) {
        return;
    }

    if (cobj->flags & 1) {
        if (vec_normalize_check(up, &v) != 0) {
            OSReport(lbl_8027464C);
            __assert(&lbl_8047D958, 0x3E4, &lbl_8047D968);
        }
        if (cobj->u.up.x != v.x || cobj->u.up.y != v.y || cobj->u.up.z != v.z) {
            cobj->flags |= 0xC0000000;
            cobj->u.up = v;
        }
    } else {
        HSD_CObjSetRoll(cobj, upvec2roll(cobj, up));
    }
}
#endif /* HSD_COBJ_CANDIDATE_80194DA4 */

/* 0x801947C8 | 0x464 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_801947C8)
/* With an explicit up vector (flag bit 0) the roll is turned back into an up
   vector; otherwise it is stored directly, marking the matrices dirty when it
   changed. */
void HSD_CObjSetRoll(HSD_CObj* cobj, f32 roll)
{
    Vec up;

    if (cobj == NULL) {
        return;
    }
    if (cobj->flags & 1) {
        roll2upvec(cobj, &up, roll);
        HSD_CObjSetUpVector(cobj, &up);
    } else {
        if (cobj->u.roll != roll) {
            cobj->flags |= 0xC0000000;
        }
        cobj->u.roll = roll;
    }
}
#endif /* HSD_COBJ_CANDIDATE_801947C8 */

/* 0x80195794 | 0x104 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_80195794)
extern char lbl_8047D958;
extern char lbl_8047D960;
extern char lbl_80274660[];
extern char lbl_80274670[];
#if 1
f32 HSD_CObjGetEyeDistance(HSD_CObj* cobj) {
    Vec position;
    Vec interest;
    Vec look_vector;

    if (cobj == NULL) {
        return lbl_8047D978;
    }
    if (cobj->eyepos == NULL) {
        __assert(&lbl_8047D958, 0x353, lbl_80274660);
    }
    if (cobj->interest == NULL) {
        __assert(&lbl_8047D958, 0x354, lbl_80274670);
    }
    cobj_get_eye_position(cobj, &position);
    cobj_get_interest(cobj, &interest);
    PSVECSubtract(&interest, &position, &look_vector);
    return PSVECMag(&look_vector);
}
#endif

extern char lbl_8047D958;
extern char lbl_8047D960;

/* 0x80195898 | 0x6C */
#if 1
void HSD_CObjSetEyePosition(HSD_CObj* cobj, Vec* position)
{
    cobj_set_eye_position(cobj, position);
}
#endif
#endif /* HSD_COBJ_EXACT_80195794 */

/* 0x80195970 | 0x6C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_SET_INTEREST)
#if 1
void HSD_CObjSetInterest(HSD_CObj* cobj, Vec* interest)
{
    cobj_set_interest(cobj, interest);
}
#endif
#endif /* HSD_COBJ_EXACT_SET_INTEREST */

/* 0x80195A48 | 0x24 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_80195A48)
extern void fn_801975FC(void);
extern void fn_801974A8(void);
#if 1
void fn_80195A48(void) { fn_801975FC(); fn_801974A8(); }
#endif
#endif /* HSD_COBJ_EXACT_80195A48 */

/* 0x80195A6C | 0x4A0 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80195A6C)
extern void fn_800A3910(void);
extern int setupBottomHalfCamera(HSD_CObj*);   /* wrk7: was `void (void)`; typed-C returns int, takes HSD_CObj* */
extern int setupTopHalfCamera(HSD_CObj*);   /* wrk7: was `void (void)`; typed-C returns int, takes HSD_CObj* */
extern void HSD_Panic(const char*, u32, const char*);
extern void fn_80197400(void);
extern int fn_8019C7B0(void);
#if 1
/* decompiled wrk8 2026-06-16: functional (TU not byte-measurable) - the camera
   "set current / program GX" dispatcher. Stores `current` (lbl_8047B234), queries
   the active render/projection mode (fn_8019C7B0) and ticks fn_80197400, then
   programs the GX viewport + scissor + projection matrix one of four ways:
     mode 0 : viewport/scissor scaled through the active EFB dims (lbl_80466BC0)
              and per-axis denominators (lbl_80478C50/C54) - sub-screen render.
     mode 1 : delegate to setupTopHalfCamera (plain EFB-clamped viewport).
     mode 2 : delegate to setupBottomHalfCamera (scissor-box + ratio variant).
     mode 3 : viewport/scissor used directly (no EFB scaling).
   On success it recomputes the viewing matrix (HSD_CObjSetupViewingMtx) and returns 1; an
   unknown mode panics and returns 0. The viewport callback's last two args are
   the GX near/far Z (0.0, 1.0). INFERRED: the projection/scaling constants and the
   panic message string are anonymous SDA2 literals reconstructed by value/name. */
int HSD_CObjSetCurrent(HSD_CObj* cobj)
{
    extern u32 lbl_8047B234;     /* `current` camera (SDA) */
    extern u8 lbl_80466BC0[];    /* GX render-mode global; +4/+6 = u16 EFB w/h, +0x18 = u8 offset flag */
    extern u32 lbl_80478C58;     /* viewport/scissor callback (default fn_80196C54) */
    extern s32 lbl_80478C50;     /* EFB-relative x denominator */
    extern s32 lbl_80478C54;     /* EFB-relative y denominator */
    extern char lbl_8047D958;
    extern char lbl_80274680[];  /* HSD_Panic message: bad camera mode - INFERRED name */
    extern f32 lbl_8047D978;     /* 0.0f (GX near Z) - named SDA2 constant */
    extern f32 lbl_8047D9B0;     /* 1.0f (GX far Z)  - named SDA2 constant */
    int mode;
    int ok;

    if (cobj == NULL) {
        return 0;
    }
    mode = fn_8019C7B0();
    fn_80197400();
    lbl_8047B234 = (u32) cobj; /* current = cobj */

    switch (mode) {
    case 3: {
        int ortho_flag;
        f32 mtx[3][4];
        /* direct path: viewport/scissor used verbatim */
        ((void (*)(int, f32, f32, f32, f32, f32, f32)) lbl_80478C58)(
            0,
            *(volatile f32*)&cobj->viewport.xmin,
            *(volatile f32*)&cobj->viewport.ymin,
            cobj->viewport.xmax - cobj->viewport.xmin,
            cobj->viewport.ymax - cobj->viewport.ymin,
            lbl_8047D978, lbl_8047D9B0);
        fn_800BD7A0(
            cobj->scissor.left, cobj->scissor.top,
            cobj->scissor.right - cobj->scissor.left,
            cobj->scissor.bottom - cobj->scissor.top);
        switch (cobj->projection_type) {
        case PROJ_PERSPECTIVE:
            ortho_flag = 0;
            C_MTXPerspective(&mtx[0][0],
                             cobj->projection_param.perspective.fov,
                             cobj->projection_param.perspective.aspect,
                             cobj->near, cobj->far);
            break;
        case PROJ_FRUSTUM:
            ortho_flag = 0;
            fn_800A3874(
                &mtx[0][0],
                cobj->projection_param.frustum.top,
                cobj->projection_param.frustum.bottom,
                cobj->projection_param.frustum.left,
                cobj->projection_param.frustum.right,
                cobj->near, cobj->far);
            break;
        case PROJ_ORTHO:
            ortho_flag = 1;
            fn_800A39E0(
                &mtx[0][0],
                cobj->projection_param.ortho.top,
                cobj->projection_param.ortho.bottom,
                cobj->projection_param.ortho.left,
                cobj->projection_param.ortho.right,
                cobj->near, cobj->far);
            break;
        }
        fn_800BD2E0(&mtx[0][0], ortho_flag);
        ok = 1;
        break;
    }
    case 0: {
        int ortho_flag;
        f32 mtx[3][4];
        /* sub-screen path: scale viewport/scissor through the EFB dimensions */
        f64 sx = (f64)(u32) *(u16*)(lbl_80466BC0 + 4) / (f64) lbl_80478C50;
        f64 sy = (f64)(u32) *(u16*)(lbl_80466BC0 + 6) / (f64) lbl_80478C54;
        int has_off = (lbl_80466BC0[0x18] != 0);
        f32 vx = (f32)(cobj->viewport.xmin * sx);
        f32 vy = (f32)(cobj->viewport.ymin * sy);
        f32 vw = (f32)(cobj->viewport.xmax * sx) - vx;
        f32 vh = (f32)(cobj->viewport.ymax * sy) - vy;
        /* viewport.* are f32; scissor.* are u16 (lhz) */
        if (has_off) {
            ((void (*)(int, f32, f32, f32, f32, f32, f32)) lbl_80478C58)(
                1, vx, vy, vw, vh, lbl_8047D978, lbl_8047D9B0);
        } else {
            ((void (*)(int, f32, f32, f32, f32, f32, f32)) lbl_80478C58)(
                0, vx, vy, vw, vh, lbl_8047D978, lbl_8047D9B0);
        }
        {
            f32 sc_right = (f32)((f64)(u32) cobj->scissor.right * sx);
            f32 sc_bottom = (f32)((f64)(u32) cobj->scissor.bottom * sy);
            f32 sc_left = (f32)((f64)(u32) cobj->scissor.left * sx);
            f32 sc_top = (f32)((f64)(u32) cobj->scissor.top * sy);
            fn_800BD7A0(
                fn_800C46B0(sc_left),
                fn_800C46B0(sc_top),
                fn_800C46B0(sc_right - sc_left),
                fn_800C46B0(sc_bottom - sc_top));
        }
        switch (cobj->projection_type) {
        case PROJ_PERSPECTIVE:
            ortho_flag = 0;
            C_MTXPerspective(&mtx[0][0],
                             cobj->projection_param.perspective.fov,
                             cobj->projection_param.perspective.aspect,
                             cobj->near, cobj->far);
            break;
        case PROJ_FRUSTUM:
            ortho_flag = 0;
            fn_800A3874(
                &mtx[0][0],
                cobj->projection_param.frustum.top,
                cobj->projection_param.frustum.bottom,
                cobj->projection_param.frustum.left,
                cobj->projection_param.frustum.right,
                cobj->near, cobj->far);
            break;
        case PROJ_ORTHO:
            ortho_flag = 1;
            fn_800A39E0(
                &mtx[0][0],
                cobj->projection_param.ortho.top,
                cobj->projection_param.ortho.bottom,
                cobj->projection_param.ortho.left,
                cobj->projection_param.ortho.right,
                cobj->near, cobj->far);
            break;
        }
        fn_800BD2E0(&mtx[0][0], ortho_flag);
        ok = 1;
        break;
    }
    case 1:
        ok = setupTopHalfCamera(cobj);
        break;
    case 2:
        ok = setupBottomHalfCamera(cobj);
        break;
    default:
        HSD_Panic(&lbl_8047D958, 0x2ab, lbl_80274680);
        return 0;
    }

    if (ok == 0) {
        return 0;
    }
    HSD_CObjSetupViewingMtx(cobj);
    return 1;
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80195A6C */

/* 0x80195F0C | 0x1B8 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80195A6C)
extern int HSD_CObjGetUpVector(HSD_CObj* cobj, Vec* up);
extern char lbl_8047D958;
extern char lbl_8047D960;
#if 1
/* decompiled wrk5: functional (TU not byte-measurable) - recomputes the camera
   viewing matrix via C_MTXLookAt when it (or its eye/interest WObjs) is dirty.
   flags: 0x2 = up-to-date, 0x40000000 = force-recompute, 0x80000000 = updated. */
void HSD_CObjSetupViewingMtx(HSD_CObj* cobj) {
    Vec eye;
    Vec up;
    Vec interest;
    int update;
    int interest_dirty;
    int eye_dirty;

    if (cobj->flags & 0x2) {
        return;
    }
    update = 1;
    interest_dirty = 1;
    if (!(cobj->flags & 0x40000000)) {
        eye_dirty = 0;
        if (cobj->eyepos != NULL) {
            if (cobj->eyepos->flags & 0x2) {
                eye_dirty = 1;
            }
        }
        if (eye_dirty == 0) {
            interest_dirty = 0;
        }
    }
    if (interest_dirty == 0) {
        interest_dirty = 0;
        if (cobj->interest != NULL) {
            if (cobj->interest->flags & 0x2) {
                interest_dirty = 1;
            }
        }
        if (interest_dirty == 0) {
            update = 0;
        }
    }
    if (update == 0) {
        return;
    }
    if (cobj == NULL) __assert(&lbl_8047D958, 0x318, &lbl_8047D960);
    if (cobj == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
    HSD_WObjGetPosition(cobj->eyepos, &eye);
    if (HSD_CObjGetUpVector(cobj, &up) == 0) {
        extern f32 lbl_8047D978;   /* 0.0f named SDA2 constant */
        extern f32 lbl_8047D9B0;   /* 1.0f named SDA2 constant */
        up.x = lbl_8047D978;
        up.y = lbl_8047D9B0;
        up.z = lbl_8047D978;
    }
    if (cobj == NULL) __assert(&lbl_8047D958, 0x300, &lbl_8047D960);
    if (cobj == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
    HSD_WObjGetPosition(cobj->interest, &interest);
    C_MTXLookAt(cobj->view_mtx, &eye, &up, &interest);
    {
        HSD_WObj* w;
        w = cobj->eyepos;
        w->flags = w->flags & ~0x2;
        w = cobj->interest;
        w->flags = w->flags & ~0x2;
    }
    if (cobj != NULL) {
        cobj->flags = cobj->flags & ~0x40000000;
    }
    if (cobj != NULL) {
        cobj->flags |= 0x80000000;
    }
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80195A6C */

/* 0x801960C4 | 0x31C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80195A6C)
#if 1
/* decompiled wrk7: functional (TU not byte-measurable) - near-twin of setupTopHalfCamera
   that ALSO programs the GX scissor box. Reads the GX render-mode scan height
   (lbl_80466BC0 + 6, u16) and a guard band `vfilter = height - 8`; returns 0 if the
   viewport bottom is above the band. Clamps cobj->scissor (u16 left/right/top/
   bottom at 0x1C..0x22) to the band and commits it via fn_800BD7A0; clamps the
   viewport, invokes the viewport callback (lbl_80478C58, default fn_80196C54),
   computes the visible-height ratio, then builds and loads the projection matrix
   (1 perspective / 2 frustum via fn_800A3874, 3 ortho via fn_800A39E0, then
   fn_800BD2E0). Returns 1.
   SDA2 constants resolved by address (see NOTES): lbl_8047D998/lbl_8047D9C8 are
   doubles (half-fov / deg->rad), lbl_8047D9D4/lbl_8047D9D0 the perspective
   recenter terms, lbl_8047D978/lbl_8047D9B0 the callback constants. */
int setupBottomHalfCamera(HSD_CObj* cobj)
{
    extern u8 lbl_80466BC0[];   /* GX render-mode global; +6 = u16 scan/EFB height */
    extern u32 lbl_80478C58;    /* viewport/scissor callback (default fn_80196C54) */
    extern f32 lbl_8047D978;    /* callback const A */
    extern f32 lbl_8047D9B0;    /* callback const B */
    extern double lbl_8047D998; /* perspective half-fov scale */
    extern double lbl_8047D9C8; /* perspective deg->rad scale */
    extern f32 lbl_8047D9D4;    /* perspective ratio scale (D3) */
    extern f32 lbl_8047D9D0;    /* perspective recenter base (D0) */

    int vfilter;
    f32 fvfilter;
    f32 ratio;
    f32 mtx[3][4];
    int ortho_flag;

    vfilter = (int) *(u16*)(lbl_80466BC0 + 6) - 8;
    fvfilter = (f32)(u32) vfilter;
    if (cobj->viewport.ymax < fvfilter) {
        return 0;
    }

    /* ---- GX scissor box, clamped to the guard band ---- */
    {
        f32 sc_left = (f32)(u16) cobj->scissor.left;
        f32 sc_right = (f32)(u16) cobj->scissor.right;
        u16 clamped_top = (cobj->scissor.top <= (u16) vfilter)
                              ? (u16) vfilter : cobj->scissor.top;
        f32 top_off = (f32)(u32) ((int) clamped_top - vfilter);
        f32 bottom_off = (f32)(u32) ((int) cobj->scissor.bottom - vfilter);
        f32 sc_width = sc_right - sc_left;
        f32 sc_height = bottom_off - top_off;
        ((void (*)(int, int, int, int)) fn_800BD7A0)(
            ((int (*)(f32)) fn_800C46B0)(sc_left),
            ((int (*)(f32)) fn_800C46B0)(top_off),
            ((int (*)(f32)) fn_800C46B0)(sc_width),
            ((int (*)(f32)) fn_800C46B0)(sc_height));
    }

    /* ---- viewport rect + callback + visible-height ratio ---- */
    {
        f32 vp_xmin = cobj->viewport.xmin;
        f32 vp_width = cobj->viewport.xmax - cobj->viewport.xmin;
        f32 vp_ymax = cobj->viewport.ymax;
        f32 vp_ymin_clamped =
            (cobj->viewport.ymin <= fvfilter) ? fvfilter : cobj->viewport.ymin;
        f32 top_off = vp_ymin_clamped - fvfilter;
        f32 visible_h = vp_ymax - vp_ymin_clamped;
        ((void (*)(int, f32, f32, f32, f32, f32, f32)) lbl_80478C58)(
            0, vp_xmin, top_off, vp_width, visible_h, lbl_8047D978, lbl_8047D9B0);
        ratio = visible_h / (vp_ymax - cobj->viewport.ymin);
    }

    /* ---- build & load the projection matrix ---- */
    ortho_flag = 0;
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE: {
        f32 t = (f32) fn_800CE220(
                    (f32) (lbl_8047D9C8 *
                           (lbl_8047D998 * cobj->projection_param.perspective.fov)));
        f32 nt = cobj->near * t;
        f32 off = lbl_8047D9D4 * ratio + lbl_8047D9D0;
        f32 aspect = cobj->projection_param.perspective.aspect;
        ((void (*)(f32*, f32, f32, f32, f32, f32, f32)) fn_800A3874)(
            &mtx[0][0], nt * off, -nt, -(nt * aspect), nt * aspect,
            cobj->near, cobj->far);
        break;
    }
    case PROJ_FRUSTUM: {
        f32 top = cobj->projection_param.frustum.top;
        f32 bottom = cobj->projection_param.frustum.bottom;
        ((void (*)(f32*, f32, f32, f32, f32, f32, f32)) fn_800A3874)(
            &mtx[0][0], ratio * (top - bottom) + bottom, bottom,
            cobj->projection_param.frustum.left,
            cobj->projection_param.frustum.right, cobj->near, cobj->far);
        break;
    }
    case PROJ_ORTHO: {
        f32 top = cobj->projection_param.ortho.top;
        f32 bottom = cobj->projection_param.ortho.bottom;
        ortho_flag = 1;
        ((void (*)(f32*, f32, f32, f32, f32, f32, f32)) fn_800A39E0)(
            &mtx[0][0], ratio * (top - bottom) + bottom, bottom,
            cobj->projection_param.ortho.left,
            cobj->projection_param.ortho.right, cobj->near, cobj->far);
        break;
    }
    default:
        break;
    }

    ((void (*)(f32*, int)) fn_800BD2E0)(&mtx[0][0], ortho_flag);
    return 1;
}
#endif

/* 0x801963E0 | 0x2B8 */
#if 1
/* decompiled wrk7: functional (TU not byte-measurable) - programs the camera's
   GX viewport and projection matrix for the frame (the CObj setup path).
   Returns 0 if the viewport is clipped past the EFB top (viewport.ymin >= EFB
   height); otherwise sets the GX viewport (fn_800BD7A0), invokes the viewport
   callback (lbl_80478C58, default fn_80196C54), builds the projection matrix on
   the stack per projection_type (1 perspective / 2 frustum via fn_800A3874,
   3 ortho via fn_800A39E0), loads it with fn_800BD2E0, and returns 1. `ratio`
   shrinks the frustum vertically to the visible fraction of a partially
   off-screen viewport. lbl_80466BC0 = active GX render-mode global (+6 = u16 EFB
   height). SDA2 constants resolved by address (see NOTES). */
int setupTopHalfCamera(HSD_CObj* cobj)
{
    extern u8 lbl_80466BC0[];   /* GX render-mode global; +6 = u16 EFB height */
    extern u32 lbl_80478C58;    /* viewport/scissor callback (default fn_80196C54) */
    extern f32 lbl_8047D978;    /* callback const A */
    extern f32 lbl_8047D9B0;    /* const B (perspective recenter base) */
    extern double lbl_8047D998; /* perspective half-fov scale */
    extern double lbl_8047D9C8; /* perspective deg->rad scale */
    extern f32 lbl_8047D9D4;    /* perspective ratio scale (D3) */

    f32 efb_h;
    f32 clamped_ymax;
    f32 vheight;
    f32 vwidth;
    f32 ratio;
    f32 mtx[3][4];
    int ortho_flag;

    efb_h = (f32)(u16) *(u16*)(lbl_80466BC0 + 6);
    if (cobj->viewport.ymin >= efb_h) {
        return 0;
    }

    /* program the GX viewport box (float coords -> fixed via fn_800C46B0) */
    efb_h = (f32)(u16) *(u16*)(lbl_80466BC0 + 6);
    clamped_ymax = (cobj->viewport.ymax < efb_h) ? cobj->viewport.ymax : efb_h;
    vheight = clamped_ymax - cobj->viewport.ymin;
    vwidth = cobj->viewport.xmax - cobj->viewport.xmin;
    ((void (*)(int, int, int, int)) fn_800BD7A0)(
        ((int (*)(f32)) fn_800C46B0)(cobj->viewport.xmin),
        ((int (*)(f32)) fn_800C46B0)(cobj->viewport.ymin),
        ((int (*)(f32)) fn_800C46B0)(vwidth),
        ((int (*)(f32)) fn_800C46B0)(vheight));

    /* recompute clamp + visible-height ratio for the projection */
    efb_h = (f32)(u16) *(u16*)(lbl_80466BC0 + 6);
    clamped_ymax = (cobj->viewport.ymax < efb_h) ? cobj->viewport.ymax : efb_h;
    vheight = clamped_ymax - cobj->viewport.ymin;
    vwidth = cobj->viewport.xmax - cobj->viewport.xmin;
    ratio = vheight / (cobj->viewport.ymax - cobj->viewport.ymin);

    /* viewport/scissor offset callback */
    ((void (*)(int, f32, f32, f32, f32, f32, f32)) lbl_80478C58)(
        0, cobj->viewport.xmin, cobj->viewport.ymin, vwidth, vheight,
        lbl_8047D978, lbl_8047D9B0);

    ortho_flag = 0;
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE: {
        f32 t = (f32) fn_800CE220(
                    (f32) (lbl_8047D9C8 *
                           (lbl_8047D998 * cobj->projection_param.perspective.fov)));
        f32 nt = cobj->near * t;
        f32 off = lbl_8047D9B0 - lbl_8047D9D4 * ratio;
        f32 aspect = cobj->projection_param.perspective.aspect;
        ((void (*)(f32*, f32, f32, f32, f32, f32, f32)) fn_800A3874)(
            &mtx[0][0], nt, nt * off, -(nt * aspect), nt * aspect,
            cobj->near, cobj->far);
        break;
    }
    case PROJ_FRUSTUM: {
        f32 top = cobj->projection_param.frustum.top;
        f32 bottom = cobj->projection_param.frustum.bottom;
        ((void (*)(f32*, f32, f32, f32, f32, f32, f32)) fn_800A3874)(
            &mtx[0][0], top, top - ratio * (top - bottom),
            cobj->projection_param.frustum.left,
            cobj->projection_param.frustum.right, cobj->near, cobj->far);
        break;
    }
    case PROJ_ORTHO: {
        f32 top = cobj->projection_param.ortho.top;
        f32 bottom = cobj->projection_param.ortho.bottom;
        ortho_flag = 1;
        ((void (*)(f32*, f32, f32, f32, f32, f32, f32)) fn_800A39E0)(
            &mtx[0][0], top, top - ratio * (top - bottom),
            cobj->projection_param.ortho.left,
            cobj->projection_param.ortho.right, cobj->near, cobj->far);
        break;
    }
    default:
        break;
    }

    ((void (*)(f32*, int)) fn_800BD2E0)(&mtx[0][0], ortho_flag);
    return 1;
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80195A6C */

/* 0x80196698 | 0x64 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80195A6C)
#if 1
void HSD_CObjReqAnim(HSD_CObj* cobj, f32 frame)
{
    if ((cobj = cobj) && cobj) {
        fn_801C29C4(cobj->aobj, frame);
        HSD_WObjReqAnim(cobj->eyepos, frame);
        HSD_WObjReqAnim(cobj->interest, frame);
    }
}
#else
void HSD_CObjReqAnim(void) { /* TODO */ }
#endif

/* 0x801966FC | 0x50 */
#if 1
void HSD_CObjAnim(HSD_CObj* cobj) {
    if (!cobj) return;
    fn_801C27F4(cobj->aobj, cobj, HSD_COBJ_METHOD(cobj)->update);
    HSD_WObjInterpretAnim(cobj->eyepos);
    HSD_WObjInterpretAnim(cobj->interest);
}
#endif

/* 0x8019674C | 0x3C4 */
#if 1
/* decompiled wrk7: functional (TU not byte-measurable) - the CObj `update` method
   installed by CObjInfoInit. Dispatches on an animation channel `type` (jumptable
   jumptable_8036C6E0, valid 0..0xC) and applies *(f32*)value to the matching
   camera property. Index->case mapping taken from the jumptable .data dump; bodies
   match the recovered instruction flow. NOTE: interest channels 5/6/7 all overwrite
   component 0 (eye channels 1/2/3 write X/Y/Z) - reproduced verbatim,
   FLAGGED for review (likely an original-game copy/paste artefact). */
void CObjUpdateFunc(HSD_CObj* cobj, u32 type, f32* value)
{
    HSD_CObj* c = cobj;
    f32* val = value;
    Vec v;

    if (c == NULL) {
        return;
    }
    switch (type) {
    case 1: /* eye position, component X */
        if (c == NULL) __assert(&lbl_8047D958, 0x318, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
        HSD_WObjGetPosition(c->eyepos, &v);
        v.x = *val;
        if (c == NULL) __assert(&lbl_8047D958, 0x324, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
        HSD_WObjSetPosition(c->eyepos, &v);
        break;
    case 2: /* eye position, component Y */
        if (c == NULL) __assert(&lbl_8047D958, 0x318, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
        HSD_WObjGetPosition(c->eyepos, &v);
        v.y = *val;
        if (c == NULL) __assert(&lbl_8047D958, 0x324, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
        HSD_WObjSetPosition(c->eyepos, &v);
        break;
    case 3: /* eye position, component Z */
        if (c == NULL) __assert(&lbl_8047D958, 0x318, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
        HSD_WObjGetPosition(c->eyepos, &v);
        v.z = *val;
        if (c == NULL) __assert(&lbl_8047D958, 0x324, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
        HSD_WObjSetPosition(c->eyepos, &v);
        break;
    case 5: /* interest position (asm writes component 0) */
        if (c == NULL) __assert(&lbl_8047D958, 0x300, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
        HSD_WObjGetPosition(c->interest, &v);
        v.x = *val;
        if (c == NULL) __assert(&lbl_8047D958, 0x30c, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
        HSD_WObjSetPosition(c->interest, &v);
        break;
    case 6: /* interest position (asm writes component 0) */
        if (c == NULL) __assert(&lbl_8047D958, 0x300, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
        HSD_WObjGetPosition(c->interest, &v);
        v.x = *val;
        if (c == NULL) __assert(&lbl_8047D958, 0x30c, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
        HSD_WObjSetPosition(c->interest, &v);
        break;
    case 7: /* interest position (asm writes component 0) */
        if (c == NULL) __assert(&lbl_8047D958, 0x300, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
        HSD_WObjGetPosition(c->interest, &v);
        v.x = *val;
        if (c == NULL) __assert(&lbl_8047D958, 0x30c, &lbl_8047D960);
        if (c == NULL) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
        HSD_WObjSetPosition(c->interest, &v);
        break;
    case 9: /* roll */
        HSD_CObjSetRoll(c, *val);
        break;
    case 10: /* field of view (perspective only) */
    {
        f32 temp = *val;
        if (c != NULL && c->projection_type == PROJ_PERSPECTIVE) {
            c->projection_param.perspective.fov = temp;
        }
        break;
    }
    case 11: /* near clip plane */
    {
        f32 temp = *val;
        if (c != NULL) {
            c->near = temp;
        }
        break;
    }
    case 12: /* far clip plane */
    {
        f32 temp = *val;
        if (c != NULL) {
            c->far = temp;
        }
        break;
    }
    default: /* 0, 4, 8: no-op */
        break;
    }
}
#endif

/* 0x80196B10 | 0xA8 */
#if 1
void HSD_CObjAddAnim(HSD_CObj* cobj_arg, HSD_CameraAnim* aobj_info_arg) {
    extern void fn_801C25E4(u32);
    extern u32 fn_801C2670(u32);
    extern void __assert(const char*, u32, const char*);
    extern char lbl_8047D958;
    extern char lbl_8047D960;
    u8* cobj = (u8*)cobj_arg;
    u8* aobj_info = (u8*)aobj_info_arg;
    if (cobj == NULL) return;
    if (aobj_info == NULL) return;
    if (*(volatile u32*)(cobj + 0x84) != 0) {
        fn_801C25E4(*(u32*)(cobj + 0x84));
    }
    *(u32*)(cobj + 0x84) = fn_801C2670(*(u32*)aobj_info);
    if (!cobj) __assert(&lbl_8047D958, 0x2e8, &lbl_8047D960);
    HSD_WObjAddAnim((HSD_WObj*)*(u32*)(cobj + 0x24),
                    (HSD_WObjAnim*)*(u32*)(aobj_info + 0x4));
    if (!cobj) __assert(&lbl_8047D958, 0x2d0, &lbl_8047D960);
    HSD_WObjAddAnim((HSD_WObj*)*(u32*)(cobj + 0x28),
                    (HSD_WObjAnim*)*(u32*)(aobj_info + 0x8));
}
#endif

/* 0x80196BB8 | 0x84 */
#if 1
void HSD_CObjRemoveAnim(HSD_CObj* cobj) {
    extern void fn_801C25E4(u32);
    extern void __assert(const char*, u32, const char*);
    extern char lbl_8047D958;
    extern char lbl_8047D960;
    u8* ptr = (u8*)cobj;
    if (!ptr) { return; }
    if (!ptr) { return; }
    fn_801C25E4(*(u32*)(ptr + 0x84));
    *(u32*)(ptr + 0x84) = 0;
    if (!ptr) { __assert(&lbl_8047D958, 0x2E8, &lbl_8047D960); }
    HSD_WObjRemoveAnim((HSD_WObj*)*(u32*)(ptr + 0x24));
    if (!ptr) { __assert(&lbl_8047D958, 0x2D0, &lbl_8047D960); }
    HSD_WObjRemoveAnim((HSD_WObj*)*(u32*)(ptr + 0x28));
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80195A6C */

/* 0x80196C3C | 0x18 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_80196C3C)
typedef void (*HSD_CObjViewportCallback)(int, f32, f32, f32, f32, f32, f32);
#if 1
void fn_80196C3C(HSD_CObjViewportCallback callback)
{
    extern HSD_CObjViewportCallback lbl_80478C58;
    extern void fn_80196C54(int, f32, f32, f32, f32, f32, f32);

    if (callback == NULL) {
        callback = fn_80196C54;
    }
    lbl_80478C58 = callback;
}
#endif
#endif /* HSD_COBJ_EXACT_80196C3C */

/* 0x80196C54 | 0x8C */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80196C54)
extern void _savefpr_26(void);
extern void _restfpr_26(void);
extern void fn_800AA2F0(void);
extern void fn_800BD640(void);
extern void fn_800BD744(void);
#if 1
void fn_80196C54(int flag, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f) {
    extern void fn_800AA2F0(void);
    extern void fn_800BD640(f32, f32, f32, f32, f32, f32);
    extern void fn_800BD744(f32, f32, f32, f32, f32, f32);
    if (flag != 0) {
        fn_800AA2F0();
        fn_800BD640(a, b, c, d, e, f);
    } else {
        fn_800BD744(a, b, c, d, e, f);
    }
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80196C54 */

/* 0x80196CE0 | 0x98 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80196C54)
extern void fn_8009C1B4(void);
#if 1
void fn_80196CE0(void) {
    /* PPC live-register SPR/GQR save is not expressible in portable C. */
    *(u16*)(lbl_80465080 + 0x1A2) |= 1;
    OSFillFPUContext(lbl_80465080);
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80196C54 */

/* 0x80196D78 | 0x98 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80196C54)
extern void fn_800060F0(const char*, u32, const char*, ...);
extern void fn_80196CE0(void);
extern char lbl_802746A0[];
extern char lbl_80465080[];
#if 1
void HSD_Panic(const char* file, u32 line, const char* expr) {
    extern u32 lbl_8047B238;
    if (lbl_8047B238 != 0) {
        fn_80196CE0();
        OSReport(lbl_802746A0, expr, file, line);
        ((void(*)(char*, ...))lbl_8047B238)(lbl_80465080);
    }
    fn_800060F0(file, line, expr);
}
#endif

/* 0x80196E10 | 0xA4 */
extern char lbl_802746B8[];
extern char lbl_8047D9D8;
#if 1
void __assert(const char* file, u32 line, const char* expr) {
    extern u32 lbl_8047B238;
    OSReport(lbl_802746B8, expr);
    if (lbl_8047B238 != 0) {
        fn_80196CE0();
        OSReport(lbl_802746A0, &lbl_8047D9D8, file, line);
        ((void(*)(char*, ...))lbl_8047B238)(lbl_80465080);
    }
    fn_800060F0(file, line, &lbl_8047D9D8);
}
#endif

/* 0x80196EB4 | 0x44 */
#if 1
void _HSD_DispForgetMemory(void) {
    extern u32 lbl_8047B24C;
    extern u32 lbl_8047B250;
    extern u32 lbl_8047B254;
    extern u32 lbl_8047B258;
    extern u32 lbl_8047B25C;
    extern u32 lbl_80478C64;
    extern u32 lbl_80478C68;
    extern u32 lbl_80478C6C;
    lbl_8047B24C = 0;
    lbl_80478C64 = (u32)&lbl_8047B24C;
    lbl_8047B250 = 0;
    lbl_80478C68 = (u32)&lbl_8047B250;
    lbl_8047B254 = 0;
    lbl_8047B258 = 0;
    lbl_80478C6C = (u32)&lbl_8047B258;
    lbl_8047B25C = 0;
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80196C54 */

/* 0x80196EF8 | 0x424 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80196C54)
extern void _savefpr_27(void);
extern void _restfpr_27(void);
extern void fn_800B7874(u32, u32);
extern void fn_800B7D3C(void);
extern void fn_800B7D74(u32, u32, u32, u32, u32);
extern void fn_800B857C(u32, u32, u32, u32, u32, u32);
extern void fn_800B884C(u32);
extern void fn_800B928C(u32, u32, u16);
extern void fn_800B94F0(u32);
extern void fn_800BA6B0(u32);
extern void fn_800BA6F4(u32, u32, u32, u32, u32, u32, u32);
extern void fn_800BA9E4(void*, void*, u16, u16, u32, u32, u32, u32);
extern void fn_800BAFFC(void*, u32);
extern void fn_800BC114(u32, u32);
extern void fn_800BC618(u32, u8, u32, u32, u8);
extern void fn_800BC66C(u32, u32, u32);
extern void fn_800BC6F0(u32, u32, u32, u32);
extern void fn_800BC8C8(u32);
extern void fn_800BCDDC(u32, u32, u32, u32);
extern void fn_800BCE30(u32);
extern void fn_800BCE5C(u32);
extern void fn_800BCE88(u32, u32, u32);
extern void fn_800BCEBC(u32);
extern void fn_800BD4B4(f32*, u32);
extern void fn_800BD554(u32);
extern void fn_801B25C4(s32);
extern void HSD_StateInvalidate(void);
extern u8 lbl_8036C720[];
extern u8 lbl_8036CBC0[];
extern u8 lbl_80478C60;
#if 1
void HSD_EraseRect(s32 arg0, s32 arg1, s32 arg2, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5) {
    u8 texobj[0x20];
    u32 color_word;
    u8* color;
    volatile f32* fifo_f32;
    volatile u8* fifo_u8;

    if (arg0 == 0 && arg1 == 0 && arg2 == 0) {
        return;
    }

    if (arg2 != 0) {
        fn_800BA9E4(texobj, lbl_8036C720, 4, 4, 0x11, 1, 1, 0);
        fn_800BAFFC(texobj, 0);
        fn_800B884C(1);
        fn_800B857C(0, 1, 4, 0x3C, 0, 0x7D);
        fn_800BC8C8(1);
        fn_800BC6F0(0, 0, 0, 4);
        fn_800BC114(0, 4);
        fn_800BC66C(2, 0x11, 0);
    } else {
        fn_800B884C(0);
        fn_800BC8C8(1);
        fn_800BC6F0(0, 0xFF, 0xFF, 4);
        fn_800BC114(0, 4);
    }

    fn_800B94F0(0);
    fn_800BC618(7, 0, 1, 7, 0);
    fn_800BCEBC(1);
    fn_800BCE88(1, 7, arg2 != 0);
    fn_800BCDDC(2, 1, 0, 3);
    fn_800BCE30(arg0 != 0);
    fn_800BCE5C(arg1 != 0);
    fn_800BA6B0(1);
    fn_800BA6F4(4, 0, 0, 1, 0, 0, 2);
    fn_800B7D3C();
    fn_800B7D74(0, 9, 1, 4, 0);
    fn_800B7D74(0, 0xB, 1, 5, 0);
    fn_800B7D74(0, 0xD, 1, 0, 0);
    fn_800BD4B4((f32*)lbl_8036CBC0, 0);
    fn_800BD554(0);
    fn_800B7874(9, 1);
    fn_800B7874(0xB, 1);
    fn_800B7874(0xD, 1);

    color_word = *(u32*)&lbl_80478C60;
    color = (u8*)&color_word;
    fn_800B928C(0x80, 0, 4);

    fifo_f32 = (volatile f32*)0xCC008000;
    fifo_u8 = (volatile u8*)0xCC008000;

    *fifo_f32 = f3;
    *fifo_f32 = f1;
    *fifo_f32 = f5;
    *fifo_u8 = color[0];
    *fifo_u8 = color[1];
    *fifo_u8 = color[2];
    *fifo_u8 = color[3];
    *fifo_u8 = 0;
    *fifo_u8 = 0;

    *fifo_f32 = f4;
    *fifo_f32 = f1;
    *fifo_f32 = f5;
    *fifo_u8 = color[0];
    *fifo_u8 = color[1];
    *fifo_u8 = color[2];
    *fifo_u8 = color[3];
    *fifo_u8 = 1;
    *fifo_u8 = 0;

    *fifo_f32 = f4;
    *fifo_f32 = f2;
    *fifo_f32 = f5;
    *fifo_u8 = color[0];
    *fifo_u8 = color[1];
    *fifo_u8 = color[2];
    *fifo_u8 = color[3];
    *fifo_u8 = 1;
    *fifo_u8 = 1;

    *fifo_f32 = f3;
    *fifo_f32 = f2;
    *fifo_f32 = f5;
    *fifo_u8 = color[0];
    *fifo_u8 = color[1];
    *fifo_u8 = color[2];
    *fifo_u8 = color[3];
    *fifo_u8 = 0;
    *fifo_u8 = 1;

    fn_800BC66C(0, 0x11, 0);
    fn_801B25C4(-1);
}
#endif

/* 0x8019731C | 0x20 */
#if 1
void HSD_SetEraseColor(u8 a, u8 b, u8 c, u8 d) {
    extern u8 lbl_80478C60;
    /* WALL: CW CSE collapses the repeated SDA base; target keeps r9/r8/r7 bases. */
    (&lbl_80478C60)[0] = a;
    (&lbl_80478C60)[1] = b;
    (&lbl_80478C60)[2] = c;
    (&lbl_80478C60)[3] = d;
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80196C54 */

/* 0x8019733C | 0x8 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_EXACT_8019733C)
typedef void (*HSD_DispParticleCallback)(s32, s32, s32, HSD_JObj*);
#if 1
void fn_8019733C(HSD_DispParticleCallback callback)
{
    extern HSD_DispParticleCallback lbl_8047B240;
    lbl_8047B240 = callback;
}
#endif
#endif /* HSD_COBJ_EXACT_8019733C */

/* 0x80197344 | 0xBC */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80197344)
extern void fn_80197784(HSD_JObj* jobj, f32 vmtx[3][4], u32 trsp_mask,
                        u32 rendermode);
void fn_80197344(HSD_JObj* jobj, f32 vmtx[3][4], u32 trsp_mask,
                 u32 rendermode)
{
    extern void (*lbl_8047B240)(s32, s32, s32, HSD_JObj*);

    if (jobj != NULL) {
        if (union_type_dobj(jobj)) {
            fn_80197784(jobj, vmtx, trsp_mask, rendermode);
        } else if (union_type_ptcl(jobj) && lbl_8047B240 != NULL) {
            HSD_SList* sp;

            for (sp = jobj->u.ptcl; sp != NULL; sp = sp->next) {
                if (((u32) sp->data & 0x80000000) != 0) {
                    /* Packed particle reference: bank[0:5], offset[6:29]. */
                    u32 bank = 0x3F & (u32) sp->data;
                    u32 offset = ((u32) sp->data & 0x3FFFFFC0) >> 6;

                    (*lbl_8047B240)(0, bank, offset, jobj);
                }
                sp->data = (void*) ((u32) sp->data & 0x7FFFFFFF);
            }
        }
    }
}

typedef struct HSD_ZListNode {
    f32 projection_mtx[3][4];
    void* vmtx;
    void* jobj;
    u32 rendermode;
    struct {
        struct HSD_ZListNode* texedge;
        struct HSD_ZListNode* translucent;
    } sort;
    struct HSD_ZListNode* next;
} HSD_ZListNode;

/* 0x80197400 | 0xA8 */
extern void HSD_MtxFree(void* mtx);
extern void HSD_ObjFree(void* allocator, void* object);
extern u8 lbl_80465348[];
void fn_80197400(void)
{
    extern HSD_ZListNode* lbl_8047B24C;
    extern HSD_ZListNode* lbl_8047B250;
    extern u32 lbl_8047B254;
    extern HSD_ZListNode* lbl_8047B258;
    extern u32 lbl_8047B25C;
    extern HSD_ZListNode** lbl_80478C64;
    extern HSD_ZListNode** lbl_80478C68;
    extern HSD_ZListNode** lbl_80478C6C;
    HSD_ZListNode* list = lbl_8047B24C;

    while (list) {
        HSD_ZListNode* next = list->next;
        if (list->vmtx) {
            HSD_MtxFree(list->vmtx);
        }
        HSD_ObjFree(lbl_80465348, list);
        list = next;
    }
    lbl_8047B24C = NULL;
    lbl_80478C64 = &lbl_8047B24C;

    lbl_8047B250 = NULL;
    lbl_80478C68 = &lbl_8047B250;
    lbl_8047B254 = 0;

    lbl_8047B258 = NULL;
    lbl_80478C6C = &lbl_8047B258;
    lbl_8047B25C = 0;
}
#endif /* HSD_COBJ_CANDIDATE_80197344 */

/* 0x801975FC | 0x54 */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80197344)
extern u32 fn_80197650(u32, u32, u32);
#if 1
void fn_801975FC(void) {
    extern s32 lbl_8047B248;
    extern u32 lbl_8047B250;
    extern u32 lbl_8047B254;
    extern u32 lbl_8047B258;
    extern u32 lbl_8047B25C;
    if (lbl_8047B248 == 0) return;
    lbl_8047B250 = fn_80197650(lbl_8047B250, lbl_8047B254, 0x3c);
    lbl_8047B258 = fn_80197650(lbl_8047B258, lbl_8047B25C, 0x40);
}
#endif
#endif /* HSD_COBJ_CANDIDATE_80197344 */

/* 0x80197998 | 0xCC */
#if !defined(HSD_COBJ_SPLIT) || defined(HSD_COBJ_CANDIDATE_80197344)
extern void fn_8019F024(HSD_JObj* jobj);
extern void fn_801A5DCC(f32 pmtx[3][4]);
extern void fn_801AB63C(u32 first, u32 second);
void fn_80197998(HSD_JObj* jobj, f32 vmtx[3][4], f32 pmtx[3][4],
                 u32 trsp_mask, u32 rendermode)
{
    HSD_DObj* dobj;
    u32 dobj_trsp;

    fn_8019F024(jobj);

    dobj_trsp = trsp_mask << 1;

    if (!(rendermode & RENDER_SHADOW)) {
        if (jobj->flags & JOBJ_SPECULAR) {
            fn_801A5DCC(pmtx);
        }
    }

    fn_801AB63C(0, 0);
    for (dobj = jobj->u.dobj; dobj; dobj = dobj->next) {
        if (dobj->flags & DOBJ_HIDDEN) {
            continue;
        }

        if (dobj->flags & dobj_trsp) {
            HSD_DObjSetCurrent(dobj);
            HSD_DOBJ_METHOD(dobj)->disp(dobj, vmtx, pmtx, rendermode);
        }
    }
    HSD_DObjSetCurrent(NULL);
    fn_8019F024(NULL);
}
#endif /* HSD_COBJ_CANDIDATE_80197344 */
