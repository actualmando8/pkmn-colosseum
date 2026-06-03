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

/* 0x80193CD0 | 0x60 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80193CD0(void) {
#include "src/hsd/hsd_cobj_fn_80193CD0.inc"
}
#else
void fn_80193CD0(u8* ptr) {
    extern u32 lbl_8047B230;
    extern u32 lbl_8047B234;
    extern u8 lbl_8036C678[];
    u32 class_info;
    if (ptr == (u8*)lbl_8047B230) { lbl_8047B230 = 0; }
    if (ptr == lbl_8036C678) { lbl_8047B234 = 0; }
    class_info = *(u32*)(lbl_8036C678 + 0x14);
    ((void(*)(u8*))*(u32*)(class_info + 0x38))(ptr);
}
#endif
#pragma pop

/* 0x80193D30 | 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80196E10(const char*, u32, const char*);
extern void fn_801A84F0(void);
extern void fn_801C25E4(void);
#if 1
asm void fn_80193D30(void) {
#include "src/hsd/hsd_cobj_fn_80193D30.inc"
}
#else
void fn_80193D30(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80193EC8 | 0x7C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern void* fn_80191628(void);
#if 0
asm void fn_80193EC8(void) {
#include "src/hsd/hsd_cobj_fn_80193EC8.inc"
}
#else
int fn_80193EC8(HSD_CObj* cobj) {
    extern HSD_CObjInfo lbl_8036C678;
    HSD_ClassInfo* info = HSD_CLASS_INFO(&lbl_8036C678);
    int result = (int)info->head.parent->alloc((HSD_ClassInfo*)cobj);
    if (result < 0) return result;
    if (cobj != NULL) {
        cobj->flags |= 0xC0000000;
    }
    cobj->eyepos = fn_80191628();
    cobj->interest = fn_80191628();
    return 0;
}
#endif
#pragma pop

/* 0x80193F44 | 0xCC */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern HSD_ClassInfo* fn_80193748(const char*);
extern void* fn_80193828(void*);
#if 0
asm void fn_80193F44(void) {
#include "src/hsd/hsd_cobj_fn_80193F44.inc"
}
#else
void* fn_80193F44(u8* ptr) {
    extern u32 lbl_8047B230;
    extern u8 lbl_8036C678[];
    extern char lbl_8047D958;
    extern char lbl_8047D960;
    void* cobj_result;
    u32 tmp;
    HSD_ClassInfo* info;
    if (ptr == NULL) return NULL;
    tmp = *(u32*)ptr;
    if (tmp == 0) goto default_alloc;
    info = fn_80193748(*(const char**)ptr);
    if (info != 0) goto do_alloc_from_info;
default_alloc:
    if (lbl_8047B230 != 0) {
        cobj_result = fn_80193828((void*)lbl_8047B230);
    } else {
        cobj_result = fn_80193828((void*)lbl_8036C678);
    }
    if (cobj_result == NULL) {
        fn_80196E10(&lbl_8047D958, 0x7a4, &lbl_8047D960);
    }
    goto setup;
do_alloc_from_info:
    cobj_result = fn_80193828(info);
    if (cobj_result == NULL) {
        fn_80196E10(&lbl_8047D958, 0x7f9, &lbl_8047D960);
    }
setup:
    {
        void** vtable = *(void***)cobj_result;
        ((void(*)(void*, u8*))vtable[0x3c / 4])(cobj_result, ptr);
    }
    return cobj_result;
}
#endif
#pragma pop

/* 0x80194010 | 0x248 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019189C(void);
extern void fn_801947C8(void);
extern void fn_80194DA4(void);
#if 1
asm void fn_80194010(void) {
#include "src/hsd/hsd_cobj_fn_80194010.inc"
}
#else
void fn_80194010(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80194258 | 0x60 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80194258(void) {
#include "src/hsd/hsd_cobj_fn_80194258.inc"
}
#else
void* fn_80194258(void) {
    extern u32 lbl_8047B230;
    extern const char lbl_8047D958[];
    extern const char lbl_8047D960[];
    typedef void* (*fn_80193828_t)(void*);
    typedef void (*fn_80196E10_t)(const char*, int, const char*);
    void* arg = lbl_8047B230 != 0 ? (void*)lbl_8047B230 : (void*)0x8037C678;
    void* result = ((fn_80193828_t)fn_80193828)(arg);
    if (result == NULL) ((fn_80196E10_t)fn_80196E10)(lbl_8047D958, 0x7A4, lbl_8047D960);
    return result;
}
#endif
#pragma pop

/* 0x801942B8 | 0x8 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801942B8(void) {
#include "src/hsd/hsd_cobj_fn_801942B8.inc"
}
#else
u32 fn_801942B8(void) { extern u32 lbl_8047B234; return lbl_8047B234; }
#endif
#pragma pop

/* 0x801942C0 | 0x5C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801942C0(void) {
#include "src/hsd/hsd_cobj_fn_801942C0.inc"
}
#else
void fn_801942C0(u8* ptr, f32* a, f32* b, f32* c, f32* d) {
    if (ptr == NULL) return;
    if (ptr[0x50] != 3) return;
    if (a != NULL) *(f32*)a = *(f32*)(ptr + 0x40);
    if (b != NULL) *(f32*)b = *(f32*)(ptr + 0x44);
    if (c != NULL) *(f32*)c = *(f32*)(ptr + 0x48);
    if (d != NULL) *(f32*)d = *(f32*)(ptr + 0x4C);
}
#endif
#pragma pop

/* 0x8019431C | 0x3C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8019431C(void) {
#include "src/hsd/hsd_cobj_fn_8019431C.inc"
}
#else
void fn_8019431C(u8* ptr, f32* a, f32* b) {
    if (ptr == NULL) return;
    if (ptr[0x50] != 1) return;
    if (a != NULL) *(f32*)a = *(f32*)(ptr + 0x40);
    if (b != NULL) *(f32*)b = *(f32*)(ptr + 0x44);
}
#endif
#pragma pop

/* 0x80194358 | 0x24 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80194358(void) {
#include "src/hsd/hsd_cobj_fn_80194358.inc"
}
#else
void fn_80194358(u8* ptr, f32 f1, f32 f2, f32 f3, f32 f4) {
    if (ptr == NULL) return;
    ptr[0x50] = 3;
    *(f32*)(ptr + 0x40) = f1;
    *(f32*)(ptr + 0x44) = f2;
    *(f32*)(ptr + 0x48) = f3;
    *(f32*)(ptr + 0x4C) = f4;
}
#endif
#pragma pop

/* 0x8019437C | 0x24 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8019437C(void) {
#include "src/hsd/hsd_cobj_fn_8019437C.inc"
}
#else
void fn_8019437C(u8* ptr, f32 f1, f32 f2, f32 f3, f32 f4) {
    if (ptr == NULL) return;
    ptr[0x50] = 2;
    *(f32*)(ptr + 0x40) = f1;
    *(f32*)(ptr + 0x44) = f2;
    *(f32*)(ptr + 0x48) = f3;
    *(f32*)(ptr + 0x4C) = f4;
}
#endif
#pragma pop

/* 0x801943A0 | 0x1C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801943A0(void) {
#include "src/hsd/hsd_cobj_fn_801943A0.inc"
}
#else
void fn_801943A0(u8* ptr, f32 f1, f32 f2) {
    if (ptr == NULL) return;
    ptr[0x50] = 1;
    *(f32*)(ptr + 0x40) = f1;
    *(f32*)(ptr + 0x44) = f2;
}
#endif
#pragma pop

/* 0x801943BC | 0x10 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801943BC(void) {
#include "src/hsd/hsd_cobj_fn_801943BC.inc"
}
#else
void fn_801943BC(u8* ptr, u8 val) { if (ptr == NULL) return; ptr[0x50] = val; }
#endif
#pragma pop

/* 0x801943CC | 0x18 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801943CC(void) {
#include "src/hsd/hsd_cobj_fn_801943CC.inc"
}
#else
u8 fn_801943CC(u8* ptr) {
    if (ptr == NULL) return 1;
    return ptr[0x50];
}
#endif
#pragma pop

/* 0x801943E4 | 0x1C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801943E4(void) {
#include "src/hsd/hsd_cobj_fn_801943E4.inc"
}
#else
void fn_801943E4(u8* ptr, f32 f1, f32 f2, f32 f3, f32 f4) {
    if (ptr == NULL) return;
    *(f32*)(ptr + 0x0C) = f1;
    *(f32*)(ptr + 0x10) = f2;
    *(f32*)(ptr + 0x14) = f3;
    *(f32*)(ptr + 0x18) = f4;
}
#endif
#pragma pop

/* 0x80194400 | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80194400(void) {
#include "src/hsd/hsd_cobj_fn_80194400.inc"
}
#else
void fn_80194400(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801944A4 | 0x1C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801944A4(void) {
#include "src/hsd/hsd_cobj_fn_801944A4.inc"
}
#else
void fn_801944A4(u8* ptr, u16 a, u16 b, u16 c, u16 d) {
    if (ptr == NULL) return;
    *(u16*)(ptr + 0x1C) = a;
    *(u16*)(ptr + 0x1E) = b;
    *(u16*)(ptr + 0x20) = c;
    *(u16*)(ptr + 0x22) = d;
}
#endif
#pragma pop

/* 0x801944C0 | 0x10 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801944C0(void) {
#include "src/hsd/hsd_cobj_fn_801944C0.inc"
}
#else
void fn_801944C0(u8* ptr, f32 val) { if (ptr == NULL) return; *(f32*)(ptr + 0x3C) = val; }
#endif
#pragma pop

/* 0x801944D0 | 0x18 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801944D0(void) {
#include "src/hsd/hsd_cobj_fn_801944D0.inc"
}
#else
f32 fn_801944D0(u8* ptr) {
    extern f32 lbl_8047D978;
    if (ptr == NULL) return lbl_8047D978;
    return *(f32*)(ptr + 0x3C);
}
#endif
#pragma pop

/* 0x801944E8 | 0x10 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801944E8(void) {
#include "src/hsd/hsd_cobj_fn_801944E8.inc"
}
#else
void fn_801944E8(u8* ptr, f32 val) { if (ptr == NULL) return; *(f32*)(ptr + 0x38) = val; }
#endif
#pragma pop

/* 0x801944F8 | 0x18 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801944F8(void) {
#include "src/hsd/hsd_cobj_fn_801944F8.inc"
}
#else
f32 fn_801944F8(u8* ptr) {
    extern f32 lbl_8047D978;
    if (ptr == NULL) return lbl_8047D978;
    return *(f32*)(ptr + 0x38);
}
#endif
#pragma pop

/* 0x80194510 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern double fn_800CE220(f32);
extern f32 lbl_8047D978;
extern f32 lbl_8047D97C;
extern f32 lbl_8047D980;
#if 1
asm void fn_80194510(void) {
#include "src/hsd/hsd_cobj_fn_80194510.inc"
}
#else
/* NEAR (W1 reg-permutation in tan-result FP chain): case1 keeps tan in f1, decomp evacuates to f2. */
f32 fn_80194510(u8* ptr) {
    if (ptr == NULL) {
        return lbl_8047D978;
    }
    switch (ptr[0x50]) {
    case 1:
        return *(f32*)(ptr + 0x44) * (*(f32*)(ptr + 0x38) * (f32) fn_800CE220(lbl_8047D97C * (lbl_8047D980 * *(f32*)(ptr + 0x40))));
    case 2:
        return *(f32*)(ptr + 0x4C);
    case 3:
        return *(f32*)(ptr + 0x4C);
    default:
        return lbl_8047D978;
    }
}
#endif
#pragma pop

/* 0x801945B0 | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801945B0(void) {
#include "src/hsd/hsd_cobj_fn_801945B0.inc"
}
#else
/* NEAR (W1 reg-permutation in tan-result FP chain): case1 keeps tan in f1, decomp evacuates to f3. */
f32 fn_801945B0(u8* ptr) {
    if (ptr == NULL) {
        return lbl_8047D978;
    }
    switch (ptr[0x50]) {
    case 1:
        return *(f32*)(ptr + 0x44) * (-*(f32*)(ptr + 0x38) * (f32) fn_800CE220(lbl_8047D97C * (lbl_8047D980 * *(f32*)(ptr + 0x40))));
    case 2:
        return *(f32*)(ptr + 0x48);
    case 3:
        return *(f32*)(ptr + 0x48);
    default:
        return lbl_8047D978;
    }
}
#endif
#pragma pop

/* 0x80194654 | 0x9C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80194654(void) {
#include "src/hsd/hsd_cobj_fn_80194654.inc"
}
#else
f32 fn_80194654(u8* ptr) {
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
#pragma pop

/* 0x801946F0 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801946F0(void) {
#include "src/hsd/hsd_cobj_fn_801946F0.inc"
}
#else
void fn_801946F0(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80194788 | 0x20 */
#if 0
asm void fn_80194788(void) {
#include "src/hsd/hsd_cobj_fn_80194788.inc"
}
#else
void fn_80194788(HSD_CObj* cobj, f32 val)
{
    if (cobj == NULL || cobj->projection_type != 1) {
        return;
    }
    cobj->projection_param.perspective.aspect = val;
}
#endif

/* 0x801947A8 | 0x20 */
void fn_801947A8(u8* ptr, f32 val) {
    if (ptr == NULL) return;
    if (*(u8*)(ptr + 0x50) == 1) {
        *(f32*)(ptr + 0x40) = val;
    }
}

/* 0x801947C8 | 0x464 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern void fn_800A3244(void);
extern void fn_800A3820(void);
extern void fn_800A3A9C(void);
extern void fn_800A3ADC(void);
extern void fn_800CE59C(void);
extern void fn_800CE718(void);
extern void fn_80194C2C(void);
extern void fn_80194D94(u8*);
extern void fn_8019513C(void);
extern void fn_80195590(void);
extern void fn_80195904(HSD_CObj*, void*);
extern void fn_801959DC(HSD_CObj*, void*);
#if 1
asm void fn_801947C8(void) {
#include "src/hsd/hsd_cobj_fn_801947C8.inc"
}
#else
void fn_801947C8(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80194C2C | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80194C2C(void) {
#include "src/hsd/hsd_cobj_fn_80194C2C.inc"
}
#else
void fn_80194C2C(void) { /* TODO */ }
#endif
#pragma pop

/* WP-0061 external references */
extern void fn_80191688(HSD_WObj*, void*);
extern void fn_80191788(HSD_WObj*, void*);
extern void fn_80191DCC(void);
extern void fn_80191E38(HSD_WObj*, f32);
extern void fn_80191E88(void);
extern void fn_801919EC(HSD_WObj*);
extern void fn_801C25E4(void);
extern void fn_801C2670(void);
extern void fn_801C27F4(void*, void*, void*);
extern void fn_801C29C4(void*, f32);
/* WP-0061 forward declarations (defined later in same TU) */
extern void fn_80196E10(const char*, u32, const char*);

/* 0x80194CC4 | 0x30 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern void fn_80195F0C(u8*);
#if 0
asm void fn_80194CC4(void) {
#include "src/hsd/hsd_cobj_fn_80194CC4.inc"
}
#else
u8* fn_80194CC4(u8* ptr) { fn_80195F0C(ptr); return ptr + 0x54; }
#endif
#pragma pop

/* 0x80194CF4 | 0x6C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern f32* fn_801A8524(void);
extern void fn_800A2EB4(f32*, f32*);
#if 0
asm void fn_80194CF4(void) {
#include "src/hsd/hsd_cobj_fn_80194CF4.inc"
}
#else
f32* fn_80194CF4(HSD_CObj* cobj) {
    if (cobj->flags & 0x80000000) {
        if (cobj->proj_mtx == NULL) {
            cobj->proj_mtx = fn_801A8524();
        }
        fn_800A2EB4(cobj->view_mtx[0], cobj->proj_mtx);
        if (cobj != NULL) {
            cobj->flags &= ~0x80000000;
        }
    }
    return cobj->proj_mtx;
}
#endif
#pragma pop

/* 0x80194D60 | 0x34 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern void fn_800A2D64(void*, void*);
#if 0
asm void fn_80194D60(void) {
#include "src/hsd/hsd_cobj_fn_80194D60.inc"
}
#else
void fn_80194D60(u8* ptr, void* arg2) {
    fn_800A2D64(fn_80194CC4(ptr), arg2);
}
#endif
#pragma pop

/* 0x80194D94 | 0x10 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80194D94(void) {
#include "src/hsd/hsd_cobj_fn_80194D94.inc"
}
#else
void fn_80194D94(u8* ptr) { *(u32*)(ptr + 0x8) |= 0xC0000000; }
#endif
#pragma pop

/* 0x80194DA4 | 0x32C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern void fn_800A3244(void);
extern void fn_800A3458(void);
extern void fn_800A3820(void);
extern void fn_800A3A9C(void);
extern void fn_800A3ADC(void);
extern void fn_800A3B7C(void);
extern void fn_800CE2D8(void);
extern void fn_80191688(HSD_WObj*, void*);
extern void fn_80196E10(const char*, u32, const char*);
#if 1
asm void fn_80194DA4(void) {
#include "src/hsd/hsd_cobj_fn_80194DA4.inc"
}
#else
void fn_80194DA4(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801950D0 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019513C(void);
#if 1
asm void fn_801950D0(void) {
#include "src/hsd/hsd_cobj_fn_801950D0.inc"
}
#else
void fn_801950D0(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019513C | 0x454 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800A3820(void);
extern void fn_800A3A9C(void);
extern void fn_800A3ADC(void);
#if 1
asm void fn_8019513C(void) {
#include "src/hsd/hsd_cobj_fn_8019513C.inc"
}
#else
void fn_8019513C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80195590 | 0x204 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800A3B38(void);
#if 1
asm void fn_80195590(void) {
#include "src/hsd/hsd_cobj_fn_80195590.inc"
}
#else
void fn_80195590(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80195794 | 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80195794(void) {
#include "src/hsd/hsd_cobj_fn_80195794.inc"
}
#else
void fn_80195794(void) { /* TODO */ }
#endif
#pragma pop

extern char lbl_8047D958;
extern char lbl_8047D960;

/* 0x80195898 | 0x6C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80195898(void) {
#include "src/hsd/hsd_cobj_fn_80195898.inc"
}
#else
void fn_80195898(HSD_CObj* cobj, void* arg) {
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x324, &lbl_8047D960);
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x2e8, &lbl_8047D960);
    fn_80191788(cobj->eyepos, arg);
}
#endif
#pragma pop

/* 0x80195904 | 0x6C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80195904(void) {
#include "src/hsd/hsd_cobj_fn_80195904.inc"
}
#else
void fn_80195904(HSD_CObj* cobj, void* arg) {
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x318, &lbl_8047D960);
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x2e8, &lbl_8047D960);
    fn_80191688(cobj->eyepos, arg);
}
#endif
#pragma pop

/* 0x80195970 | 0x6C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80195970(void) {
#include "src/hsd/hsd_cobj_fn_80195970.inc"
}
#else
void fn_80195970(HSD_CObj* cobj, void* arg) {
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x30c, &lbl_8047D960);
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x2d0, &lbl_8047D960);
    fn_80191788(cobj->interest, arg);
}
#endif
#pragma pop

/* 0x801959DC | 0x6C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801959DC(void) {
#include "src/hsd/hsd_cobj_fn_801959DC.inc"
}
#else
void fn_801959DC(HSD_CObj* cobj, void* arg) {
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x300, &lbl_8047D960);
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x2d0, &lbl_8047D960);
    fn_80191688(cobj->interest, arg);
}
#endif
#pragma pop

/* 0x80195A48 | 0x24 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern void fn_801975FC(void);
extern void fn_801974A8(void);
#if 0
asm void fn_80195A48(void) {
#include "src/hsd/hsd_cobj_fn_80195A48.inc"
}
#else
void fn_80195A48(void) { fn_801975FC(); fn_801974A8(); }
#endif
#pragma pop

/* 0x80195A6C | 0x4A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800A3874(void);
extern void fn_800A3910(void);
extern void fn_800A39E0(void);
extern void fn_800BD2E0(void);
extern void fn_800BD7A0(void);
extern void fn_800C46B0(void);
extern void fn_801960C4(void);
extern void fn_801963E0(void);
extern void fn_80196D78(const char*, u32, const char*);
extern void fn_80197400(void);
extern void fn_8019C7B0(void);
#if 1
asm void fn_80195A6C(void) {
#include "src/hsd/hsd_cobj_fn_80195A6C.inc"
}
#else
void fn_80195A6C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80195F0C | 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801950D0(void);
#if 1
asm void fn_80195F0C(u8* ptr) {
#include "src/hsd/hsd_cobj_fn_80195F0C.inc"
}
#else
void fn_80195F0C(u8* ptr) { /* TODO */ }
#endif
#pragma pop

/* 0x801960C4 | 0x31C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801960C4(void) {
#include "src/hsd/hsd_cobj_fn_801960C4.inc"
}
#else
void fn_801960C4(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801963E0 | 0x2B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801963E0(void) {
#include "src/hsd/hsd_cobj_fn_801963E0.inc"
}
#else
void fn_801963E0(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80196698 | 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801C29C4(void*, f32);
#if 1
void fn_80196698(HSD_CObj* cobj, f32 frame)
{
    if ((cobj = cobj) && cobj) {
        fn_801C29C4(cobj->aobj, frame);
        fn_80191E38(cobj->eyepos, frame);
        fn_80191E38(cobj->interest, frame);
    }
}
#else
void fn_80196698(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801966FC | 0x50 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801966FC(void) {
#include "src/hsd/hsd_cobj_fn_801966FC.inc"
}
#else
void fn_801966FC(HSD_CObj* cobj) {
    if (!cobj) return;
    fn_801C27F4(cobj->aobj, cobj, ((HSD_CObjInfo*)cobj->parent.parent.class_info)->load);
    fn_801919EC(cobj->eyepos);
    fn_801919EC(cobj->interest);
}
#endif
#pragma pop

/* 0x8019674C | 0x3C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8019674C(void) {
#include "src/hsd/hsd_cobj_fn_8019674C.inc"
}
#else
void fn_8019674C(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80196B10 | 0xA8 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern void fn_801C2670(void);
#if 0
asm void fn_80196B10(void) {
#include "src/hsd/hsd_cobj_fn_80196B10.inc"
}
#else
void fn_80196B10(u8* cobj, u8* aobj_info) {
    extern void fn_801C25E4(u32);
    extern u32 fn_801C2670(u32);
    extern void fn_80191DCC(u32, u32);
    extern void fn_80196E10(const char*, u32, const char*);
    extern char lbl_8047D958;
    extern char lbl_8047D960;
    if (cobj == NULL) return;
    if (aobj_info == NULL) return;
    if (*(volatile u32*)(cobj + 0x84) != 0) {
        fn_801C25E4(*(u32*)(cobj + 0x84));
    }
    *(u32*)(cobj + 0x84) = fn_801C2670(*(u32*)aobj_info);
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x2e8, &lbl_8047D960);
    fn_80191DCC(*(u32*)(cobj + 0x24), *(u32*)(aobj_info + 0x4));
    if (!cobj) fn_80196E10(&lbl_8047D958, 0x2d0, &lbl_8047D960);
    fn_80191DCC(*(u32*)(cobj + 0x28), *(u32*)(aobj_info + 0x8));
}
#endif
#pragma pop

/* 0x80196BB8 | 0x84 */
#pragma push
#pragma optimization_level 1
#pragma optimizewithasm off
#if 0
asm void fn_80196BB8(void) {
#include "src/hsd/hsd_cobj_fn_80196BB8.inc"
}
#else
void fn_80196BB8(u8* ptr) {
    extern void fn_801C25E4(u32);
    extern void fn_80191E88(u32);
    extern void fn_80196E10(const char*, u32, const char*);
    extern char lbl_8047D958;
    extern char lbl_8047D960;
    if (!ptr) { return; }
    if (!ptr) { return; }
    fn_801C25E4(*(u32*)(ptr + 0x84));
    *(u32*)(ptr + 0x84) = 0;
    if (!ptr) { fn_80196E10(&lbl_8047D958, 0x2E8, &lbl_8047D960); }
    fn_80191E88(*(u32*)(ptr + 0x24));
    if (!ptr) { fn_80196E10(&lbl_8047D958, 0x2D0, &lbl_8047D960); }
    fn_80191E88(*(u32*)(ptr + 0x28));
}
#endif
#pragma pop

/* 0x80196C3C | 0x18 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80196C3C(void) {
#include "src/hsd/hsd_cobj_fn_80196C3C.inc"
}
#else
void fn_80196C3C(u8* ptr) {
    extern u32 lbl_80478C58;
    extern void fn_80196C54(void);
    if (ptr == NULL) ptr = (u8*)fn_80196C54;
    lbl_80478C58 = (u32)ptr;
}
#endif
#pragma pop

/* 0x80196C54 | 0x8C */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern void _savefpr_26(void);
extern void _restfpr_26(void);
extern void fn_800AA2F0(void);
extern void fn_800BD640(void);
extern void fn_800BD744(void);
#if 0
asm void fn_80196C54(void) {
#include "src/hsd/hsd_cobj_fn_80196C54.inc"
}
#else
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
#pragma pop

/* 0x80196CE0 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8009C1B4(void);
#if 1
asm void fn_80196CE0(void) {
#include "src/hsd/hsd_cobj_fn_80196CE0.inc"
}
#else
void fn_80196CE0(void) { /* TODO */ }
#endif
#pragma pop

/* 0x80196D78 | 0x98 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern void fn_800060F0(const char*, u32, const char*, ...);
extern void fn_80196CE0(void);
extern void OSReport(const char* fmt, ...);
extern char lbl_802746A0[];
extern char lbl_80465080[];
#if 0
asm void fn_80196D78(void) {
#include "src/hsd/hsd_cobj_fn_80196D78.inc"
}
#else
void fn_80196D78(const char* file, u32 line, const char* expr) {
    extern u32 lbl_8047B238;
    if (lbl_8047B238 != 0) {
        fn_80196CE0();
        OSReport(lbl_802746A0, expr, file, line);
        ((void(*)(char*, ...))lbl_8047B238)(lbl_80465080);
    }
    fn_800060F0(file, line, expr);
}
#endif
#pragma pop

/* 0x80196E10 | 0xA4 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern char lbl_802746B8[];
extern char lbl_8047D9D8;
#if 0
asm void fn_80196E10(const char*, u32, const char*) {
#include "src/hsd/hsd_cobj_fn_80196E10.inc"
}
#else
void fn_80196E10(const char* file, u32 line, const char* expr) {
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
#pragma pop

/* 0x80196EB4 | 0x44 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80196EB4(void) {
#include "src/hsd/hsd_cobj_fn_80196EB4.inc"
}
#else
void fn_80196EB4(void) {
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
#pragma pop

/* 0x80196EF8 | 0x424 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void _savefpr_27(void);
extern void _restfpr_27(void);
extern void fn_800B7874(void);
extern void fn_800B7D3C(void);
extern void fn_800B7D74(void);
extern void fn_800B857C(void);
extern void fn_800B884C(void);
extern void fn_800B928C(void);
extern void fn_800B94F0(void);
extern void fn_800BA6B0(void);
extern void fn_800BA6F4(void);
extern void fn_800BA9E4(void);
extern void fn_800BAFFC(void);
extern void fn_800BC114(void);
extern void fn_800BC618(void);
extern void fn_800BC66C(void);
extern void fn_800BC6F0(void);
extern void fn_800BC8C8(void);
extern void fn_800BCDDC(void);
extern void fn_800BCE30(void);
extern void fn_800BCE5C(void);
extern void fn_800BCE88(void);
extern void fn_800BCEBC(void);
extern void fn_800BD4B4(void);
extern void fn_800BD554(void);
extern void fn_801B25C4(void);
#if 1
asm void fn_80196EF8(void) {
#include "src/hsd/hsd_cobj_fn_80196EF8.inc"
}
#else
void fn_80196EF8(void) { /* TODO */ }
#endif
#pragma pop

/* 0x8019731C | 0x20 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8019731C(void) {
#include "src/hsd/hsd_cobj_fn_8019731C.inc"
}
#else
void fn_8019731C(u8 a, u8 b, u8 c, u8 d) {
    extern u8 lbl_80478C60;
    u8* p;
    lbl_80478C60 = a;
    p = &lbl_80478C60;
    p[1] = b;
    p[2] = c;
    p[3] = d;
}
#endif
#pragma pop

/* 0x8019733C | 0x8 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8019733C(void) {
#include "src/hsd/hsd_cobj_fn_8019733C.inc"
}
#else
void fn_8019733C(u32 val) { extern u32 lbl_8047B240; lbl_8047B240 = val; }
#endif
#pragma pop

/* 0x801975FC | 0x54 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern u32 fn_80197650(u32, u32, u32);
#if 0
asm void fn_801975FC(void) {
#include "src/hsd/hsd_cobj_fn_801975FC.inc"
}
#else
void fn_801975FC(void) {
    extern s32 lbl_8047B248;
    extern u32 lbl_8047B250;
    extern u32 lbl_8047B254;
    extern u32 lbl_8047B258;
    extern u32 lbl_8047B25C;
    u32 tmp;
    if (lbl_8047B248 == 0) return;
    tmp = fn_80197650(lbl_8047B250, lbl_8047B254, 0x3c);
    lbl_8047B250 = tmp;
    lbl_8047B258 = fn_80197650(lbl_8047B258, lbl_8047B25C, 0x40);
}
#endif
#pragma pop
