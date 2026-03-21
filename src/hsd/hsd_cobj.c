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
void fn_80193C24(void) {
    extern u8 lbl_80274628[];
    extern u8 lbl_80274640[];
    extern u8 lbl_8036C678[];
    extern u8 lbl_8036CC00[];
    extern void fn_80193B30();
    extern void fn_80193CD0();
    extern void fn_80193D30();
    extern void fn_80193EC8();
    extern void fn_80194010();
    extern void fn_8019674C();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;

    r3 = (u32)lbl_8036C678;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274628;
    r6 = (u32)lbl_80274640;
    r3 = (u32)lbl_8036C678;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274628;
    r6 = (u32)lbl_80274640;
    r7 = 0x44;
    r8 = 0x8c;
    fn_80193B30();
    r7 = (u32)fn_80193EC8;
    r6 = (u32)fn_80193D30;
    r3 = (u32)fn_8019674C;
    r10 = (u32)lbl_8036C678;
    r9 = (u32)fn_80193D30;
    r8 = (u32)lbl_8036C678;
    tmp = (u32)fn_8019674C;
    r6 = (u32)lbl_8036C678;
    r3 = (u32)lbl_8036C678;
    r8 = (u32)lbl_8036C678;
    r3 = (u32)lbl_8036C678;
    r5 = (u32)fn_80193CD0;
    r11 = (u32)fn_80193EC8;
    r10 = (u32)lbl_8036C678;
    r4 = (u32)fn_80194010;
    r7 = (u32)fn_80193CD0;
    r5 = (u32)fn_80194010;
    r6 = (u32)lbl_8036C678;
    r4 = (u32)lbl_8036C678;
    *(u32*)((u8*)r10 + 0x2C) = r11;
    r4 = (u32)lbl_8036C678;
    *(u32*)((u8*)r8 + 0x30) = r9;
    *(u32*)((u8*)r6 + 0x38) = r7;
    *(u32*)((u8*)r4 + 0x3C) = r5;
    *(u32*)((u8*)r3 + 0x40) = tmp;
    return;
}

/* 0x60 | fn_80193CD0 | framed_no_calls */
void fn_80193CD0(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B234, lbl_8047B230 */
}

/* 0x80193D30 | 0x198 */
void fn_80193D30(void) {
    extern u8 lbl_8036C678[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern void fn_80196E10();
    extern void fn_801A84F0();
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x84);
    fn_801C25E4();
    if (r31 != 0) goto L_80193D68;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80193D68:
    r30 = *(u32*)((u8*)r31 + 0x24);
    if (r30 == 0) goto L_80193DE8;
    r3 = 0x10000;
    r4 = *(u16*)((u8*)r30 + 0x4);
    tmp = r3 & 0xFFFF;
    tmp = tmp - r4;
    tmp = __cntlzw(tmp);
    /* srwi. r3, tmp, 5 */;
    if (r30 == 0) goto L_80193D98;
    goto L_80193DB0;
L_80193D98:
    tmp = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(tmp);
    *(u16*)((u8*)r30 + 0x4) = tmp;
    r3 = (u32)r4 >> 5;
L_80193DB0:
    if ((s32)r3 == 0) goto L_80193DE8;
    if (r30 == 0) goto L_80193DE8;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x34);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80193DE8:
    if (r31 != 0) goto L_80193E00;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80193E00:
    r30 = *(u32*)((u8*)r31 + 0x28);
    if (r30 == 0) goto L_80193E80;
    r3 = 0x10000;
    r4 = *(u16*)((u8*)r30 + 0x4);
    tmp = r3 & 0xFFFF;
    tmp = tmp - r4;
    tmp = __cntlzw(tmp);
    /* srwi. r3, tmp, 5 */;
    if (r30 == 0) goto L_80193E30;
    goto L_80193E48;
L_80193E30:
    tmp = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(tmp);
    *(u16*)((u8*)r30 + 0x4) = tmp;
    r3 = (u32)r4 >> 5;
L_80193E48:
    if ((s32)r3 == 0) goto L_80193E80;
    if (r30 == 0) goto L_80193E80;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x34);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80193E80:
    tmp = *(u32*)((u8*)r31 + 0x88);
    if (tmp == 0) goto L_80193E94;
    r3 = *(u32*)((u8*)r31 + 0x88);
    fn_801A84F0();
L_80193E94:
    r4 = (u32)lbl_8036C678;
    r3 = r31;
    r4 = (u32)lbl_8036C678;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

/* 0x7C | fn_80193EC8 | generic */
u32 fn_80193EC8(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    /* refs: lbl_8036C678 */
    fn_80191628();
    fn_80191628();
    return 0;
}

/* 0x80193F44 | 0xCC */
void fn_80193F44(void) {
    extern u8 lbl_8036C678[];
    extern u8 lbl_8047B230[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern void fn_80193748();
    extern void fn_80193828();
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f9 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* mr. r30, r3 */;
    if ((s32)tmp == 0) goto L_80193FF4;
    tmp = *(u32*)((u8*)r30 + 0x0);
    if (tmp == 0) goto L_80193F7C;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_80193748();
    if (r3 != 0) goto L_80193FB8;
L_80193F7C:
    tmp = *(u32*)lbl_8047B230;
    if (tmp == 0) goto L_80193F90;
    r3 = *(u32*)lbl_8047B230;
    goto L_80193F98;
L_80193F90:
    r3 = (u32)lbl_8036C678;
    r3 = (u32)lbl_8036C678;
L_80193F98:
    fn_80193828();
    /* mr. r31, r3 */;
    if (tmp != 0) goto L_80193FD4;
    r3 = (u32)lbl_8047D958;
    r4 = 0x7a4;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
    goto L_80193FD4;
L_80193FB8:
    fn_80193828();
    /* mr. r31, r3 */;
    if (tmp != 0) goto L_80193FD4;
    r3 = (u32)lbl_8047D958;
    r4 = 0x7f9;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80193FD4:
    r5 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r4 = r30;
    r12 = *(u32*)((u8*)r5 + 0x3C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r3 = r31;
    goto L_80193FF8;
L_80193FF4:
    r3 = 0x0;
L_80193FF8:
    return;
}

/* 0x80194010 | 0x248 */
void fn_80194010(void) {
    extern u8 lbl_8036C6D4[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D968[];
    extern u8 lbl_8047D970[];
    extern void fn_8019189C();
    extern void fn_801947C8();
    extern void fn_80194DA4();
    extern void fn_80196E10();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r31 = r4;
    /* mr. r30, r3 */;
    tmp = *(u16*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r3 + 0x8) = tmp;
    r3 = *(u16*)((u8*)r4 + 0x4);
    if ((s32)tmp == 0) goto L_80194048;
    tmp = *(u32*)((u8*)r30 + 0x8);
    tmp = (tmp & ~0x3FFFFFFF) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x3FFFFFFF);
    *(u32*)((u8*)r30 + 0x8) = tmp;
L_80194048:
    if (r30 == 0) goto L_801940E0;
    tmp = *(s16*)((u8*)r31 + 0x8);
    r5 = 0x43300000;
    r4 = 0x43300000;
    r3 = 0x43300000;
    tmp = 0x43300000;
    f1 = *(f64*)lbl_8047D970;
    f3 = *(f64*)lbl_8047D970;
    f0 = f0 - f1;
    f2 = *(f64*)lbl_8047D970;
    f1 = *(f64*)lbl_8047D970;
    *(f32*)((u8*)r30 + 0xC) = f0;
    r3 = *(s16*)((u8*)r31 + 0xA);
    *(u32*)(sp + 0x20) = tmp;
    *(u32*)(sp + 0x14) = tmp;
    f0 = f0 - f3;
    *(f32*)((u8*)r30 + 0x10) = f0;
    tmp = *(s16*)((u8*)r31 + 0xC);
    *(u32*)(sp + 0x1C) = tmp;
    f0 = f0 - f2;
    *(f32*)((u8*)r30 + 0x14) = f0;
    tmp = *(s16*)((u8*)r31 + 0xE);
    *(u32*)(sp + 0x24) = tmp;
    f0 = f0 - f1;
    *(f32*)((u8*)r30 + 0x18) = f0;
L_801940E0:
    if (r30 == 0) goto L_801940F8;
    r3 = *(u32*)((u8*)r31 + 0x10);
    tmp = *(u32*)((u8*)r31 + 0x14);
    *(u32*)((u8*)r30 + 0x1C) = r3;
    *(u32*)((u8*)r30 + 0x20) = tmp;
L_801940F8:
    r3 = *(u32*)((u8*)r30 + 0x24);
    r4 = *(u32*)((u8*)r31 + 0x18);
    fn_8019189C();
    r3 = *(u32*)((u8*)r30 + 0x28);
    r4 = *(u32*)((u8*)r31 + 0x1C);
    fn_8019189C();
    f0 = *(f32*)((u8*)r31 + 0x28);
    if (r30 == 0) goto L_80194120;
    *(f32*)((u8*)r30 + 0x38) = f0;
L_80194120:
    f0 = *(f32*)((u8*)r31 + 0x2C);
    if (r30 == 0) goto L_80194130;
    *(f32*)((u8*)r30 + 0x3C) = f0;
L_80194130:
    tmp = *(u16*)((u8*)r31 + 0x4);
    tmp = tmp & 0x1;
    if (r30 == 0) goto L_8019416C;
    tmp = *(u32*)((u8*)r31 + 0x24);
    if (tmp == 0) goto L_80194158;
    r4 = *(u32*)((u8*)r31 + 0x24);
    r3 = r30;
    fn_80194DA4();
    goto L_80194178;
L_80194158:
    r4 = (u32)lbl_8036C6D4;
    r3 = r30;
    r4 = (u32)lbl_8036C6D4;
    fn_80194DA4();
    goto L_80194178;
L_8019416C:
    f1 = *(f32*)((u8*)r31 + 0x20);
    r3 = r30;
    fn_801947C8();
L_80194178:
    tmp = *(u16*)((u8*)r31 + 0x6);
    if ((s32)tmp == 2) goto L_801941F8;
    if ((s32)tmp >= 2) goto L_80194194;
    if ((s32)tmp >= 1) goto L_801941A0;
    goto L_8019422C;
L_80194194:
    if ((s32)tmp >= 4) goto L_8019422C;
    goto L_801941C4;
L_801941A0:
    f1 = *(f32*)((u8*)r31 + 0x34);
    f0 = *(f32*)((u8*)r31 + 0x30);
    if (r30 == 0) goto L_8019423C;
    tmp = 0x1;
    *(u8*)((u8*)r30 + 0x50) = tmp;
    *(f32*)((u8*)r30 + 0x40) = f0;
    *(f32*)((u8*)r30 + 0x44) = f1;
    goto L_8019423C;
L_801941C4:
    f3 = *(f32*)((u8*)r31 + 0x3C);
    f2 = *(f32*)((u8*)r31 + 0x38);
    f1 = *(f32*)((u8*)r31 + 0x34);
    f0 = *(f32*)((u8*)r31 + 0x30);
    if (r30 == 0) goto L_8019423C;
    tmp = 0x3;
    *(u8*)((u8*)r30 + 0x50) = tmp;
    *(f32*)((u8*)r30 + 0x40) = f0;
    *(f32*)((u8*)r30 + 0x44) = f1;
    *(f32*)((u8*)r30 + 0x48) = f2;
    *(f32*)((u8*)r30 + 0x4C) = f3;
    goto L_8019423C;
L_801941F8:
    f3 = *(f32*)((u8*)r31 + 0x3C);
    f2 = *(f32*)((u8*)r31 + 0x38);
    f1 = *(f32*)((u8*)r31 + 0x34);
    f0 = *(f32*)((u8*)r31 + 0x30);
    if (r30 == 0) goto L_8019423C;
    tmp = 0x2;
    *(u8*)((u8*)r30 + 0x50) = tmp;
    *(f32*)((u8*)r30 + 0x40) = f0;
    *(f32*)((u8*)r30 + 0x44) = f1;
    *(f32*)((u8*)r30 + 0x48) = f2;
    *(f32*)((u8*)r30 + 0x4C) = f3;
    goto L_8019423C;
L_8019422C:
    r3 = (u32)lbl_8047D958;
    r4 = 0x7d2;
    r5 = (u32)lbl_8047D968;
    fn_80196E10();
L_8019423C:
    r3 = 0x0;
    return;
}

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
void fn_80194400(void) {
    extern u8 lbl_8047D970[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    if (r3 == 0) goto L_8019449C;
    tmp = *(s16*)((u8*)r4 + 0x0);
    r7 = 0x43300000;
    r6 = 0x43300000;
    r5 = 0x43300000;
    tmp = 0x43300000;
    f1 = *(f64*)lbl_8047D970;
    f3 = *(f64*)lbl_8047D970;
    f0 = *(f64*)((u8*)(u32)sp + 0x8);
    f0 = f0 - f1;
    f2 = *(f64*)lbl_8047D970;
    f1 = *(f64*)lbl_8047D970;
    *(f32*)((u8*)r3 + 0xC) = f0;
    r5 = *(s16*)((u8*)r4 + 0x2);
    f0 = *(f64*)((u8*)(u32)sp + 0x10);
    f0 = f0 - f3;
    *(f32*)((u8*)r3 + 0x10) = f0;
    tmp = *(s16*)((u8*)r4 + 0x4);
    f0 = *(f64*)((u8*)(u32)sp + 0x18);
    f0 = f0 - f2;
    *(f32*)((u8*)r3 + 0x14) = f0;
    tmp = *(s16*)((u8*)r4 + 0x6);
    f0 = *(f64*)((u8*)(u32)sp + 0x20);
    f0 = f0 - f1;
    *(f32*)((u8*)r3 + 0x18) = f0;
L_8019449C:
    return;
}

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
void fn_80194510(void) {
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D97C[];
    extern u8 lbl_8047D980[];
    extern void fn_800CE220();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    /* mr. r31, r3 */;
    if ((s32)tmp != 0) goto L_80194530;
    f1 = *(f32*)lbl_8047D978;
    goto L_8019459C;
L_80194530:
    tmp = *(u8*)((u8*)r31 + 0x50);
    if ((s32)tmp == 2) goto L_80194588;
    if ((s32)tmp >= 2) goto L_8019454C;
    if ((s32)tmp >= 1) goto L_80194558;
    goto L_80194598;
L_8019454C:
    if ((s32)tmp >= 4) goto L_80194598;
    goto L_80194590;
L_80194558:
    f1 = *(f32*)lbl_8047D980;
    f0 = *(f32*)((u8*)r31 + 0x40);
    f2 = *(f32*)lbl_8047D97C;
    f0 = f1 * f0;
    f1 = f2 * f0;
    fn_800CE220();
    f1 = (f32)f1;
    f0 = *(f32*)((u8*)r31 + 0x38);
    f2 = *(f32*)((u8*)r31 + 0x44);
    f0 = f0 * f1;
    f1 = f2 * f0;
    goto L_8019459C;
L_80194588:
    f1 = *(f32*)((u8*)r31 + 0x4C);
    goto L_8019459C;
L_80194590:
    f1 = *(f32*)((u8*)r31 + 0x4C);
    goto L_8019459C;
L_80194598:
    f1 = *(f32*)lbl_8047D978;
L_8019459C:
    return;
}

/* 0x801945B0 | 0xA4 */
void fn_801945B0(void) {
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D97C[];
    extern u8 lbl_8047D980[];
    extern void fn_800CE220();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    /* mr. r31, r3 */;
    if ((s32)tmp != 0) goto L_801945D0;
    f1 = *(f32*)lbl_8047D978;
    goto L_80194640;
L_801945D0:
    tmp = *(u8*)((u8*)r31 + 0x50);
    if ((s32)tmp == 2) goto L_8019462C;
    if ((s32)tmp >= 2) goto L_801945EC;
    if ((s32)tmp >= 1) goto L_801945F8;
    goto L_8019463C;
L_801945EC:
    if ((s32)tmp >= 4) goto L_8019463C;
    goto L_80194634;
L_801945F8:
    f1 = *(f32*)lbl_8047D980;
    f0 = *(f32*)((u8*)r31 + 0x40);
    f2 = *(f32*)lbl_8047D97C;
    f0 = f1 * f0;
    f1 = f2 * f0;
    fn_800CE220();
    f0 = *(f32*)((u8*)r31 + 0x38);
    f1 = (f32)f1;
    f2 = *(f32*)((u8*)r31 + 0x44);
    f0 = -f0;
    f0 = f0 * f1;
    f1 = f2 * f0;
    goto L_80194640;
L_8019462C:
    f1 = *(f32*)((u8*)r31 + 0x48);
    goto L_80194640;
L_80194634:
    f1 = *(f32*)((u8*)r31 + 0x48);
    goto L_80194640;
L_8019463C:
    f1 = *(f32*)lbl_8047D978;
L_80194640:
    return;
}

/* 0x80194654 | 0x9C */
void fn_80194654(void) {
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D97C[];
    extern u8 lbl_8047D980[];
    extern void fn_800CE220();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    /* mr. r31, r3 */;
    if ((s32)tmp != 0) goto L_80194674;
    f1 = *(f32*)lbl_8047D978;
    goto L_801946DC;
L_80194674:
    tmp = *(u8*)((u8*)r31 + 0x50);
    if ((s32)tmp == 2) goto L_801946C8;
    if ((s32)tmp >= 2) goto L_80194690;
    if ((s32)tmp >= 1) goto L_8019469C;
    goto L_801946D8;
L_80194690:
    if ((s32)tmp >= 4) goto L_801946D8;
    goto L_801946D0;
L_8019469C:
    f1 = *(f32*)lbl_8047D980;
    f0 = *(f32*)((u8*)r31 + 0x40);
    f2 = *(f32*)lbl_8047D97C;
    f0 = f1 * f0;
    f1 = f2 * f0;
    fn_800CE220();
    f0 = *(f32*)((u8*)r31 + 0x38);
    f1 = (f32)f1;
    f0 = -f0;
    f1 = f0 * f1;
    goto L_801946DC;
L_801946C8:
    f1 = *(f32*)((u8*)r31 + 0x44);
    goto L_801946DC;
L_801946D0:
    f1 = *(f32*)((u8*)r31 + 0x44);
    goto L_801946DC;
L_801946D8:
    f1 = *(f32*)lbl_8047D978;
L_801946DC:
    return;
}

/* 0x801946F0 | 0x98 */
void fn_801946F0(void) {
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D97C[];
    extern u8 lbl_8047D980[];
    extern void fn_800CE220();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    /* mr. r31, r3 */;
    if ((s32)tmp != 0) goto L_80194710;
    f1 = *(f32*)lbl_8047D978;
    goto L_80194774;
L_80194710:
    tmp = *(u8*)((u8*)r31 + 0x50);
    if ((s32)tmp == 2) goto L_80194760;
    if ((s32)tmp >= 2) goto L_8019472C;
    if ((s32)tmp >= 1) goto L_80194738;
    goto L_80194770;
L_8019472C:
    if ((s32)tmp >= 4) goto L_80194770;
    goto L_80194768;
L_80194738:
    f1 = *(f32*)lbl_8047D980;
    f0 = *(f32*)((u8*)r31 + 0x40);
    f2 = *(f32*)lbl_8047D97C;
    f0 = f1 * f0;
    f1 = f2 * f0;
    fn_800CE220();
    f1 = (f32)f1;
    f0 = *(f32*)((u8*)r31 + 0x38);
    f1 = f0 * f1;
    goto L_80194774;
L_80194760:
    f1 = *(f32*)((u8*)r31 + 0x40);
    goto L_80194774;
L_80194768:
    f1 = *(f32*)((u8*)r31 + 0x40);
    goto L_80194774;
L_80194770:
    f1 = *(f32*)lbl_8047D978;
L_80194774:
    return;
}

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
void fn_801947C8(void) {
    extern u8 lbl_8027464C[];
    extern u8 lbl_80478AC0[];
    extern u8 lbl_80478AC8[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D968[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D988[];
    extern u8 lbl_8047D990[];
    extern u8 lbl_8047D998[];
    extern u8 lbl_8047D9A0[];
    extern u8 lbl_8047D9A8[];
    extern void fn_800A3244();
    extern void fn_800A3820();
    extern void fn_800A3A9C();
    extern void fn_800A3ADC();
    extern void fn_800CE59C();
    extern void fn_800CE718();
    extern void fn_80194C2C();
    extern void fn_80194D94();
    extern void fn_80194DA4();
    extern void fn_8019513C();
    extern void fn_80195590();
    extern void fn_80195904();
    extern void fn_801959DC();
    extern void fn_80196E10();
    u8 sp[0xC0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    /* mr. r31, r3 */;
    f30 = f1;
    if ((s32)tmp == 0) goto L_80194C08;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp & 0x1;
    if ((s32)tmp == 0) goto L_80194BEC;
    if (r31 == 0) goto L_80194820;
    tmp = *(u32*)((u8*)r31 + 0x24);
    if (tmp == 0) goto L_80194820;
    tmp = *(u32*)((u8*)r31 + 0x28);
    if (tmp != 0) goto L_80194828;
L_80194820:
    tmp = 0x0;
    goto L_80194864;
L_80194828:
    r3 = r31;
    r4 = (u32)sp + 0x14;
    fn_80195904();
    r3 = r31;
    r4 = (u32)sp + 0x20;
    fn_801959DC();
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x14;
    r5 = (u32)sp + 0x38;
    fn_800A3A9C();
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0x38;
    fn_80194C2C();
    tmp = __cntlzw(r3);
    tmp = (u32)tmp >> 5;
L_80194864:
    if ((s32)tmp == 0) goto L_80194A8C;
    f1 = *(f32*)(sp + 0x3C);
    fn_800CE59C();
    f1 = (f32)f1;
    f2 = *(f64*)lbl_8047D988;
    f0 = *(f64*)lbl_8047D990;
    f1 = f2 - f1;
    if (f1 >= f0) goto L_80194978;
    f1 = *(f32*)(sp + 0x40);
    f0 = *(f32*)(sp + 0x40);
    f3 = *(f32*)(sp + 0x3C);
    f1 = f1 * f0;
    f2 = *(f32*)(sp + 0x3C);
    f0 = *(f32*)lbl_8047D978;
    f31 = f3 * f2 + f1;
    if (f31 <= f0) goto L_80194908;
    /* frsqrte f7, f31 */;
    f6 = *(f64*)lbl_8047D998;
    f3 = *(f64*)lbl_8047D9A0;
    f4 = *(f64*)lbl_8047D998;
    f5 = f7 * f7;
    f1 = *(f64*)lbl_8047D9A0;
    f2 = *(f64*)lbl_8047D998;
    f6 = f6 * f7;
    f0 = *(f64*)lbl_8047D9A0;
    f3 = -(f31 * f5 - f3);
    f7 = f6 * f3;
    f3 = f7 * f7;
    f4 = f4 * f7;
    f1 = -(f31 * f3 - f1);
    f7 = f4 * f1;
    f1 = f7 * f7;
    f2 = f2 * f7;
    f0 = -(f31 * f1 - f0);
    f7 = f2 * f0;
    f31 = f31 * f7;
    f31 = (f32)f31;
    goto L_80194938;
L_80194908:
    f0 = *(f64*)lbl_8047D9A8;
    if (f31 >= f0) goto L_80194920;
    r3 = (u32)lbl_80478AC0;
    f31 = *(f32*)lbl_80478AC0;
    goto L_80194938;
L_80194920:
    f1 = f31;
    fn_800CE718();
    if ((s32)r3 != 1) goto L_80194938;
    r3 = (u32)lbl_80478AC0;
    f31 = *(f32*)lbl_80478AC0;
L_80194938:
    f1 = *(f32*)(sp + 0x38);
    *(f32*)(sp + 0x44) = f31;
    f0 = *(f32*)(sp + 0x38);
    f3 = -f1;
    f2 = *(f32*)(sp + 0x44);
    f1 = -f0;
    f0 = *(f32*)(sp + 0x44);
    f3 = f3 / f2;
    f4 = *(f32*)(sp + 0x3C);
    f2 = *(f32*)(sp + 0x40);
    f0 = f1 / f0;
    f1 = f4 * f3;
    f0 = f2 * f0;
    *(f32*)(sp + 0x48) = f1;
    *(f32*)(sp + 0x4C) = f0;
    goto L_80194A60;
L_80194978:
    f1 = *(f32*)(sp + 0x40);
    f0 = *(f32*)(sp + 0x40);
    f3 = *(f32*)(sp + 0x38);
    f1 = f1 * f0;
    f2 = *(f32*)(sp + 0x38);
    f0 = *(f32*)lbl_8047D978;
    f31 = f3 * f2 + f1;
    if (f31 <= f0) goto L_801949F4;
    /* frsqrte f7, f31 */;
    f6 = *(f64*)lbl_8047D998;
    f3 = *(f64*)lbl_8047D9A0;
    f4 = *(f64*)lbl_8047D998;
    f5 = f7 * f7;
    f1 = *(f64*)lbl_8047D9A0;
    f2 = *(f64*)lbl_8047D998;
    f6 = f6 * f7;
    f0 = *(f64*)lbl_8047D9A0;
    f3 = -(f31 * f5 - f3);
    f7 = f6 * f3;
    f3 = f7 * f7;
    f4 = f4 * f7;
    f1 = -(f31 * f3 - f1);
    f7 = f4 * f1;
    f1 = f7 * f7;
    f2 = f2 * f7;
    f0 = -(f31 * f1 - f0);
    f7 = f2 * f0;
    f31 = f31 * f7;
    f31 = (f32)f31;
    goto L_80194A24;
L_801949F4:
    f0 = *(f64*)lbl_8047D9A8;
    if (f31 >= f0) goto L_80194A0C;
    r3 = (u32)lbl_80478AC0;
    f31 = *(f32*)lbl_80478AC0;
    goto L_80194A24;
L_80194A0C:
    f1 = f31;
    fn_800CE718();
    if ((s32)r3 != 1) goto L_80194A24;
    r3 = (u32)lbl_80478AC0;
    f31 = *(f32*)lbl_80478AC0;
L_80194A24:
    f1 = *(f32*)(sp + 0x3C);
    *(f32*)(sp + 0x48) = f31;
    f0 = *(f32*)(sp + 0x3C);
    f3 = -f1;
    f2 = *(f32*)(sp + 0x48);
    f1 = -f0;
    f0 = *(f32*)(sp + 0x48);
    f3 = f3 / f2;
    f4 = *(f32*)(sp + 0x38);
    f2 = *(f32*)(sp + 0x40);
    f0 = f1 / f0;
    f1 = f4 * f3;
    f0 = f2 * f0;
    *(f32*)(sp + 0x44) = f1;
    *(f32*)(sp + 0x4C) = f0;
L_80194A60:
    f1 = -f30;
    r3 = (u32)sp + 0x68;
    r4 = (u32)sp + 0x38;
    fn_800A3244();
    r3 = (u32)sp + 0x68;
    r4 = (u32)sp + 0x44;
    r5 = (u32)sp + 0x50;
    fn_800A3820();
    r3 = (u32)sp + 0x50;
    r4 = (u32)sp + 0x5c;
    fn_800A3ADC();
L_80194A8C:
    if (r31 == 0) goto L_80194C08;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp & 0x1;
    if (r31 == 0) goto L_80194B8C;
    f1 = *(f32*)(sp + 0x5C);
    fn_800CE59C();
    r3 = (u32)lbl_80478AC8;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194AFC;
    f1 = *(f32*)(sp + 0x60);
    fn_800CE59C();
    r3 = (u32)lbl_80478AC8;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194AFC;
    f1 = *(f32*)(sp + 0x64);
    fn_800CE59C();
    r3 = (u32)lbl_80478AC8;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194AFC;
    tmp = -0x1;
    goto L_80194B0C;
L_80194AFC:
    r3 = (u32)sp + 0x5c;
    r4 = (u32)sp + 0x2c;
    fn_800A3ADC();
    tmp = 0x0;
L_80194B0C:
    if ((s32)tmp == 0) goto L_80194B34;
    r3 = (u32)lbl_8027464C;
    r3 = (u32)lbl_8027464C;
    OSReport();
    r3 = (u32)lbl_8047D958;
    r4 = 0x3e4;
    r5 = (u32)lbl_8047D968;
    fn_80196E10();
L_80194B34:
    f1 = *(f32*)((u8*)r31 + 0x2C);
    f0 = *(f32*)(sp + 0x2C);
    if (f1 != f0) goto L_80194B64;
    f1 = *(f32*)((u8*)r31 + 0x30);
    f0 = *(f32*)(sp + 0x30);
    if (f1 != f0) goto L_80194B64;
    f1 = *(f32*)((u8*)r31 + 0x34);
    f0 = *(f32*)(sp + 0x34);
    if (f1 == f0) goto L_80194C08;
L_80194B64:
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp | (0xc000 << 16);
    *(u32*)((u8*)r31 + 0x8) = tmp;
    *(u32*)((u8*)r31 + 0x2C) = r3;
    *(u32*)((u8*)r31 + 0x30) = tmp;
    *(u32*)((u8*)r31 + 0x34) = tmp;
    goto L_80194C08;
L_80194B8C:
    r3 = r31;
    r4 = (u32)sp + 0x5c;
    fn_80195590();
    f30 = f1;
    if (r31 == 0) goto L_80194C08;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp & 0x1;
    if (r31 == 0) goto L_80194BD0;
    f1 = f30;
    r3 = r31;
    r4 = (u32)sp + 0x8;
    fn_8019513C();
    r3 = r31;
    r4 = (u32)sp + 0x8;
    fn_80194DA4();
    goto L_80194C08;
L_80194BD0:
    f0 = *(f32*)((u8*)r31 + 0x2C);
    if (f0 == f30) goto L_80194BE4;
    r3 = r31;
    fn_80194D94();
L_80194BE4:
    *(f32*)((u8*)r31 + 0x2C) = f30;
    goto L_80194C08;
L_80194BEC:
    f0 = *(f32*)((u8*)r31 + 0x2C);
    if (f0 == f30) goto L_80194C04;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp | (0xc000 << 16);
    *(u32*)((u8*)r31 + 0x8) = tmp;
L_80194C04:
    *(f32*)((u8*)r31 + 0x2C) = f30;
L_80194C08:
    return;
}

/* 0x80194C2C | 0x98 */
void fn_80194C2C(void) {
    extern u8 lbl_80478AC8[];
    extern void fn_800A3ADC();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    if (r3 == 0) goto L_80194C48;
    if (r4 != 0) goto L_80194C50;
L_80194C48:
    r3 = -0x1;
    goto L_80194CB4;
L_80194C50:
    f0 = *(f32*)((u8*)r3 + 0x0);
    r5 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194CAC;
    f0 = *(f32*)((u8*)r3 + 0x4);
    r5 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194CAC;
    f0 = *(f32*)((u8*)r3 + 0x8);
    r5 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194CAC;
    r3 = -0x1;
    goto L_80194CB4;
L_80194CAC:
    fn_800A3ADC();
    r3 = 0x0;
L_80194CB4:
    return;
}

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
void fn_80194D94(void) {
    u32 tmp = 0;
    u32 r3 = 0;

    tmp = *(u32*)((u8*)r3 + 0x8);
    tmp = tmp | (0xc000 << 16);
    *(u32*)((u8*)r3 + 0x8) = tmp;
    return;
}

/* 0x80194DA4 | 0x32C */
void fn_80194DA4(void) {
    extern u8 lbl_8027464C[];
    extern u8 lbl_8036C6BC[];
    extern u8 lbl_8036C6C8[];
    extern u8 lbl_80478AC8[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern u8 lbl_8047D968[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D9B0[];
    extern u8 lbl_8047D9B4[];
    extern u8 lbl_8047D9B8[];
    extern void fn_800A3458();
    extern void fn_800A3820();
    extern void fn_800A3A9C();
    extern void fn_800A3ADC();
    extern void fn_800A3B7C();
    extern void fn_800CE2D8();
    extern void fn_80191688();
    extern void fn_801947C8();
    extern void fn_80196E10();
    u8 sp[0x80];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) goto L_801950B8;
    if (r31 != 0) goto L_80194DD0;
    goto L_801950B8;
L_80194DD0:
    tmp = *(u32*)((u8*)r30 + 0x8);
    tmp = tmp & 0x1;
    if (r31 == 0) goto L_80194EDC;
    if (r31 == 0) goto L_80194DE8;
    goto L_80194DF0;
L_80194DE8:
    tmp = -0x1;
    goto L_80194E5C;
L_80194DF0:
    f0 = *(f32*)((u8*)r31 + 0x0);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194E4C;
    f0 = *(f32*)((u8*)r31 + 0x4);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194E4C;
    f0 = *(f32*)((u8*)r31 + 0x8);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194E4C;
    tmp = -0x1;
    goto L_80194E5C;
L_80194E4C:
    r3 = r31;
    r4 = (u32)sp + 0x38;
    fn_800A3ADC();
    tmp = 0x0;
L_80194E5C:
    if ((s32)tmp == 0) goto L_80194E84;
    r3 = (u32)lbl_8027464C;
    r3 = (u32)lbl_8027464C;
    OSReport();
    r3 = (u32)lbl_8047D958;
    r4 = 0x3e4;
    r5 = (u32)lbl_8047D968;
    fn_80196E10();
L_80194E84:
    f1 = *(f32*)((u8*)r30 + 0x2C);
    f0 = *(f32*)(sp + 0x38);
    if (f1 != f0) goto L_80194EB4;
    f1 = *(f32*)((u8*)r30 + 0x30);
    f0 = *(f32*)(sp + 0x3C);
    if (f1 != f0) goto L_80194EB4;
    f1 = *(f32*)((u8*)r30 + 0x34);
    f0 = *(f32*)(sp + 0x40);
    if (f1 == f0) goto L_801950B8;
L_80194EB4:
    tmp = *(u32*)((u8*)r30 + 0x8);
    tmp = tmp | (0xc000 << 16);
    *(u32*)((u8*)r30 + 0x8) = tmp;
    *(u32*)((u8*)r30 + 0x2C) = r3;
    *(u32*)((u8*)r30 + 0x30) = tmp;
    *(u32*)((u8*)r30 + 0x34) = tmp;
    goto L_801950B8;
L_80194EDC:
    if (r30 == 0) goto L_80194EFC;
    tmp = *(u32*)((u8*)r30 + 0x24);
    if (tmp == 0) goto L_80194EFC;
    tmp = *(u32*)((u8*)r30 + 0x28);
    if (tmp != 0) goto L_80194F04;
L_80194EFC:
    tmp = 0x0;
    goto L_80195000;
L_80194F04:
    if (r30 != 0) goto L_80194F1C;
    r3 = (u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80194F1C:
    if (r30 != 0) goto L_80194F34;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80194F34:
    r3 = *(u32*)((u8*)r30 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    if (r30 != 0) goto L_80194F58;
    r3 = (u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80194F58:
    if (r30 != 0) goto L_80194F70;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80194F70:
    r3 = *(u32*)((u8*)r30 + 0x28);
    r4 = (u32)sp + 0x14;
    fn_80191688();
    r3 = (u32)sp + 0x14;
    r4 = (u32)sp + 0x8;
    r5 = (u32)sp + 0x2c;
    fn_800A3A9C();
    f0 = *(f32*)(sp + 0x2C);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194FE8;
    f0 = *(f32*)(sp + 0x30);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194FE8;
    f0 = *(f32*)(sp + 0x34);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80194FE8;
    tmp = -0x1;
    goto L_80194FF8;
L_80194FE8:
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x2c;
    fn_800A3ADC();
    tmp = 0x0;
L_80194FF8:
    tmp = __cntlzw(tmp);
    tmp = (u32)tmp >> 5;
L_80195000:
    if ((s32)tmp != 0) goto L_80195010;
    f1 = *(f32*)lbl_8047D978;
    goto L_801950B0;
L_80195010:
    r3 = r31;
    r4 = (u32)sp + 0x2c;
    fn_800A3B7C();
    /* fabs */ f1 = (f1 < 0) ? -f1 : f1;
    r3 = (u32)lbl_80478AC8;
    f2 = *(f32*)lbl_8047D9B0;
    f0 = *(f32*)lbl_80478AC8;
    f1 = (f32)f1;
    f1 = f2 - f1;
    if (f1 >= f0) goto L_80195044;
    f1 = *(f32*)lbl_8047D978;
    goto L_801950B0;
L_80195044:
    r3 = (u32)lbl_8036C6BC;
    r5 = (u32)lbl_8036C6C8;
    r4 = (u32)lbl_8036C6BC;
    r6 = (u32)sp + 0x2c;
    r3 = (u32)sp + 0x44;
    r5 = (u32)lbl_8036C6C8;
    fn_800A3458();
    r4 = r31;
    r3 = (u32)sp + 0x44;
    r5 = (u32)sp + 0x20;
    fn_800A3820();
    f1 = *(f32*)(sp + 0x20);
    f2 = *(f32*)(sp + 0x24);
    f0 = *(f32*)lbl_8047D978;
    f1 = -f1;
    if (f0 != f2) goto L_801950A8;
    f0 = *(f32*)lbl_8047D978;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801950A0;
    f1 = *(f32*)lbl_8047D9B4;
    goto L_801950B0;
L_801950A0:
    f1 = *(f32*)lbl_8047D9B8;
    goto L_801950B0;
L_801950A8:
    fn_800CE2D8();
    f1 = (f32)f1;
L_801950B0:
    r3 = r30;
    fn_801947C8();
L_801950B8:
    return;
}

/* 0x6C | fn_801950D0 | guarded_call */
u32 fn_801950D0(void) {
    if (1 /* guard r4 != 0 */) { return 0; }
    fn_8019513C();
    return 1;
}

/* 0x8019513C | 0x454 */
void fn_8019513C(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_80478AC8[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D988[];
    extern u8 lbl_8047D990[];
    extern u8 lbl_8047D998[];
    extern u8 lbl_8047D9A0[];
    extern u8 lbl_8047D9A8[];
    extern void fn_800A3244();
    extern void fn_800A3820();
    extern void fn_800A3A9C();
    extern void fn_800A3ADC();
    extern void fn_80191688();
    extern void fn_80196E10();
    u8 sp[0xA0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f31 = 0.0f;

    /* mr. r30, r3 */;
    f31 = f1;
    r31 = r4;
    if ((s32)tmp == 0) goto L_80195180;
    tmp = *(u32*)((u8*)r30 + 0x24);
    if (tmp == 0) goto L_80195180;
    tmp = *(u32*)((u8*)r30 + 0x28);
    if (tmp != 0) goto L_80195188;
L_80195180:
    tmp = 0x0;
    goto L_80195284;
L_80195188:
    if (r30 != 0) goto L_801951A0;
    r3 = (u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801951A0:
    if (r30 != 0) goto L_801951B8;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801951B8:
    r3 = *(u32*)((u8*)r30 + 0x24);
    r4 = (u32)sp + 0x10;
    fn_80191688();
    if (r30 != 0) goto L_801951DC;
    r3 = (u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801951DC:
    if (r30 != 0) goto L_801951F4;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801951F4:
    r3 = *(u32*)((u8*)r30 + 0x28);
    r4 = (u32)sp + 0x1c;
    fn_80191688();
    r3 = (u32)sp + 0x1c;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0x40;
    fn_800A3A9C();
    f0 = *(f32*)(sp + 0x40);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019526C;
    f0 = *(f32*)(sp + 0x44);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019526C;
    f0 = *(f32*)(sp + 0x48);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019526C;
    tmp = -0x1;
    goto L_8019527C;
L_8019526C:
    r3 = (u32)sp + 0x40;
    r4 = (u32)sp + 0x40;
    fn_800A3ADC();
    tmp = 0x0;
L_8019527C:
    tmp = __cntlzw(tmp);
    tmp = (u32)tmp >> 5;
L_80195284:
    if ((s32)tmp != 0) goto L_80195294;
    r3 = 0x0;
    goto L_80195570;
L_80195294:
    f0 = *(f32*)(sp + 0x44);
    f1 = *(f64*)lbl_8047D988;
    /* fabs */ f2 = (f0 < 0) ? -f0 : f0;
    f0 = *(f64*)lbl_8047D990;
    f2 = (f32)f2;
    f1 = f1 - f2;
    if (f1 >= f0) goto L_801953FC;
    f1 = *(f32*)(sp + 0x48);
    f0 = *(f32*)(sp + 0x48);
    f3 = *(f32*)(sp + 0x44);
    f1 = f1 * f0;
    f2 = *(f32*)(sp + 0x44);
    f0 = *(f32*)lbl_8047D978;
    f8 = f3 * f2 + f1;
    if (f8 <= f0) goto L_80195330;
    /* frsqrte f7, f8 */;
    f6 = *(f64*)lbl_8047D998;
    f3 = *(f64*)lbl_8047D9A0;
    f4 = *(f64*)lbl_8047D998;
    f5 = f7 * f7;
    f1 = *(f64*)lbl_8047D9A0;
    f2 = *(f64*)lbl_8047D998;
    f6 = f6 * f7;
    f0 = *(f64*)lbl_8047D9A0;
    f3 = -(f8 * f5 - f3);
    f7 = f6 * f3;
    f3 = f7 * f7;
    f4 = f4 * f7;
    f1 = -(f8 * f3 - f1);
    f7 = f4 * f1;
    f1 = f7 * f7;
    f2 = f2 * f7;
    f0 = -(f8 * f1 - f0);
    f7 = f2 * f0;
    f8 = f8 * f7;
    f8 = (f32)f8;
    goto L_801953BC;
L_80195330:
    f0 = *(f64*)lbl_8047D9A8;
    if (f8 >= f0) goto L_80195348;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
    goto L_801953BC;
L_80195348:
    *(f32*)(sp + 0xC) = f8;
    tmp = 0x7F800000;
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)tmp) goto L_80195370;
    if ((s32)r3 >= (s32)tmp) goto L_801953A8;
    if ((s32)r3 == 0) goto L_8019538C;
    goto L_801953A8;
L_80195370:
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_80195384;
    tmp = 0x1;
    goto L_801953AC;
L_80195384:
    tmp = 0x2;
    goto L_801953AC;
L_8019538C:
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_801953A0;
    tmp = 0x5;
    goto L_801953AC;
L_801953A0:
    tmp = 0x3;
    goto L_801953AC;
L_801953A8:
    tmp = 0x4;
L_801953AC:
    if ((s32)tmp != 1) goto L_801953BC;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
L_801953BC:
    f1 = *(f32*)(sp + 0x40);
    *(f32*)(sp + 0x34) = f8;
    f0 = *(f32*)(sp + 0x40);
    f3 = -f1;
    f2 = *(f32*)(sp + 0x34);
    f1 = -f0;
    f0 = *(f32*)(sp + 0x34);
    f3 = f3 / f2;
    f4 = *(f32*)(sp + 0x44);
    f2 = *(f32*)(sp + 0x48);
    f0 = f1 / f0;
    f1 = f4 * f3;
    f0 = f2 * f0;
    *(f32*)(sp + 0x38) = f1;
    *(f32*)(sp + 0x3C) = f0;
    goto L_80195540;
L_801953FC:
    f1 = *(f32*)(sp + 0x48);
    f0 = *(f32*)(sp + 0x48);
    f3 = *(f32*)(sp + 0x40);
    f1 = f1 * f0;
    f2 = *(f32*)(sp + 0x40);
    f0 = *(f32*)lbl_8047D978;
    f8 = f3 * f2 + f1;
    if (f8 <= f0) goto L_80195478;
    /* frsqrte f7, f8 */;
    f6 = *(f64*)lbl_8047D998;
    f3 = *(f64*)lbl_8047D9A0;
    f4 = *(f64*)lbl_8047D998;
    f5 = f7 * f7;
    f1 = *(f64*)lbl_8047D9A0;
    f2 = *(f64*)lbl_8047D998;
    f6 = f6 * f7;
    f0 = *(f64*)lbl_8047D9A0;
    f3 = -(f8 * f5 - f3);
    f7 = f6 * f3;
    f3 = f7 * f7;
    f4 = f4 * f7;
    f1 = -(f8 * f3 - f1);
    f7 = f4 * f1;
    f1 = f7 * f7;
    f2 = f2 * f7;
    f0 = -(f8 * f1 - f0);
    f7 = f2 * f0;
    f8 = f8 * f7;
    f8 = (f32)f8;
    goto L_80195504;
L_80195478:
    f0 = *(f64*)lbl_8047D9A8;
    if (f8 >= f0) goto L_80195490;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
    goto L_80195504;
L_80195490:
    *(f32*)(sp + 0x8) = f8;
    tmp = 0x7F800000;
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)tmp) goto L_801954B8;
    if ((s32)r3 >= (s32)tmp) goto L_801954F0;
    if ((s32)r3 == 0) goto L_801954D4;
    goto L_801954F0;
L_801954B8:
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_801954CC;
    tmp = 0x1;
    goto L_801954F4;
L_801954CC:
    tmp = 0x2;
    goto L_801954F4;
L_801954D4:
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_801954E8;
    tmp = 0x5;
    goto L_801954F4;
L_801954E8:
    tmp = 0x3;
    goto L_801954F4;
L_801954F0:
    tmp = 0x4;
L_801954F4:
    if ((s32)tmp != 1) goto L_80195504;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
L_80195504:
    f1 = *(f32*)(sp + 0x44);
    *(f32*)(sp + 0x38) = f8;
    f0 = *(f32*)(sp + 0x44);
    f3 = -f1;
    f2 = *(f32*)(sp + 0x38);
    f1 = -f0;
    f0 = *(f32*)(sp + 0x38);
    f3 = f3 / f2;
    f4 = *(f32*)(sp + 0x40);
    f2 = *(f32*)(sp + 0x48);
    f0 = f1 / f0;
    f1 = f4 * f3;
    f0 = f2 * f0;
    *(f32*)(sp + 0x34) = f1;
    *(f32*)(sp + 0x3C) = f0;
L_80195540:
    f1 = -f31;
    r3 = (u32)sp + 0x4c;
    r4 = (u32)sp + 0x40;
    fn_800A3244();
    r3 = (u32)sp + 0x4c;
    r4 = (u32)sp + 0x34;
    r5 = (u32)sp + 0x28;
    fn_800A3820();
    r4 = r31;
    r3 = (u32)sp + 0x28;
    fn_800A3ADC();
    r3 = 0x1;
L_80195570:
    return;
}

/* 0x80195590 | 0x204 */
void fn_80195590(void) {
    extern u8 lbl_8036C6BC[];
    extern u8 lbl_8036C6C8[];
    extern u8 lbl_80478AC8[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D9B0[];
    extern u8 lbl_8047D9B4[];
    extern u8 lbl_8047D9B8[];
    extern void fn_800A3458();
    extern void fn_800A3820();
    extern void fn_800A3A9C();
    extern void fn_800A3ADC();
    extern void fn_800A3B7C();
    extern void fn_800CE2D8();
    extern void fn_80191688();
    extern void fn_80196E10();
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) goto L_801955C8;
    tmp = *(u32*)((u8*)r30 + 0x24);
    if (tmp == 0) goto L_801955C8;
    tmp = *(u32*)((u8*)r30 + 0x28);
    if (tmp != 0) goto L_801955D0;
L_801955C8:
    tmp = 0x0;
    goto L_801956CC;
L_801955D0:
    if (r30 != 0) goto L_801955E8;
    r3 = (u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801955E8:
    if (r30 != 0) goto L_80195600;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80195600:
    r3 = *(u32*)((u8*)r30 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    if (r30 != 0) goto L_80195624;
    r3 = (u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80195624:
    if (r30 != 0) goto L_8019563C;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_8019563C:
    r3 = *(u32*)((u8*)r30 + 0x28);
    r4 = (u32)sp + 0x14;
    fn_80191688();
    r3 = (u32)sp + 0x14;
    r4 = (u32)sp + 0x8;
    r5 = (u32)sp + 0x20;
    fn_800A3A9C();
    f0 = *(f32*)(sp + 0x20);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801956B4;
    f0 = *(f32*)(sp + 0x24);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801956B4;
    f0 = *(f32*)(sp + 0x28);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801956B4;
    tmp = -0x1;
    goto L_801956C4;
L_801956B4:
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x20;
    fn_800A3ADC();
    tmp = 0x0;
L_801956C4:
    tmp = __cntlzw(tmp);
    tmp = (u32)tmp >> 5;
L_801956CC:
    if ((s32)tmp != 0) goto L_801956DC;
    f1 = *(f32*)lbl_8047D978;
    goto L_8019577C;
L_801956DC:
    r3 = r31;
    r4 = (u32)sp + 0x20;
    fn_800A3B7C();
    /* fabs */ f1 = (f1 < 0) ? -f1 : f1;
    r3 = (u32)lbl_80478AC8;
    f2 = *(f32*)lbl_8047D9B0;
    f0 = *(f32*)lbl_80478AC8;
    f1 = (f32)f1;
    f1 = f2 - f1;
    if (f1 >= f0) goto L_80195710;
    f1 = *(f32*)lbl_8047D978;
    goto L_8019577C;
L_80195710:
    r3 = (u32)lbl_8036C6BC;
    r5 = (u32)lbl_8036C6C8;
    r4 = (u32)lbl_8036C6BC;
    r6 = (u32)sp + 0x20;
    r3 = (u32)sp + 0x38;
    r5 = (u32)lbl_8036C6C8;
    fn_800A3458();
    r4 = r31;
    r3 = (u32)sp + 0x38;
    r5 = (u32)sp + 0x2c;
    fn_800A3820();
    f1 = *(f32*)(sp + 0x2C);
    f2 = *(f32*)(sp + 0x30);
    f0 = *(f32*)lbl_8047D978;
    f1 = -f1;
    if (f0 != f2) goto L_80195774;
    f0 = *(f32*)lbl_8047D978;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8019576C;
    f1 = *(f32*)lbl_8047D9B4;
    goto L_8019577C;
L_8019576C:
    f1 = *(f32*)lbl_8047D9B8;
    goto L_8019577C;
L_80195774:
    fn_800CE2D8();
    f1 = (f32)f1;
L_8019577C:
    return;
}

/* 0x80195794 | 0x104 */
void fn_80195794(void) {
    extern u8 lbl_80274660[];
    extern u8 lbl_80274670[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern u8 lbl_8047D978[];
    extern void fn_800A3A9C();
    extern void fn_800A3B38();
    extern void fn_80191688();
    extern void fn_80196E10();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    /* mr. r31, r3 */;
    if ((s32)tmp != 0) goto L_801957B4;
    f1 = *(f32*)lbl_8047D978;
    goto L_80195884;
L_801957B4:
    tmp = *(u32*)((u8*)r31 + 0x24);
    if (tmp != 0) goto L_801957D4;
    r4 = (u32)lbl_80274660;
    r3 = (u32)lbl_8047D958;
    r5 = (u32)lbl_80274660;
    r4 = 0x353;
    fn_80196E10();
L_801957D4:
    tmp = *(u32*)((u8*)r31 + 0x28);
    if (tmp != 0) goto L_801957F4;
    r4 = (u32)lbl_80274670;
    r3 = (u32)lbl_8047D958;
    r5 = (u32)lbl_80274670;
    r4 = 0x354;
    fn_80196E10();
L_801957F4:
    if (r31 != 0) goto L_8019580C;
    r3 = (u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_8019580C:
    if (r31 != 0) goto L_80195824;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80195824:
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = (u32)sp + 0x20;
    fn_80191688();
    if (r31 != 0) goto L_80195848;
    r3 = (u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80195848:
    if (r31 != 0) goto L_80195860;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80195860:
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = (u32)sp + 0x14;
    fn_80191688();
    r3 = (u32)sp + 0x14;
    r4 = (u32)sp + 0x20;
    r5 = (u32)sp + 0x8;
    fn_800A3A9C();
    r3 = (u32)sp + 0x8;
    fn_800A3B38();
L_80195884:
    return;
}

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
void fn_80195A6C(void) {
    extern u8 lbl_80274680[];
    extern u8 lbl_80466BC0[];
    extern u8 lbl_80478C50[];
    extern u8 lbl_80478C54[];
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D970[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D9B0[];
    extern u8 lbl_8047D9C0[];
    extern void fn_800A3874();
    extern void fn_800A3910();
    extern void fn_800A39E0();
    extern void fn_800BD2E0();
    extern void fn_800BD7A0();
    extern void fn_800C46B0();
    extern void fn_801960C4();
    extern void fn_801963E0();
    extern void fn_80196D78();
    extern void fn_80197400();
    extern void fn_8019C7B0();
    extern u8 lbl_80478C58;
    u8 sp[0x110];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* mr. r30, r3 */;
    if ((s32)tmp != 0) goto L_80195AB4;
    r3 = 0x0;
    goto L_80195ED0;
L_80195AB4:
    fn_8019C7B0();
    r28 = r3;
    fn_80197400();
    *(u32*)&lbl_8047B234 = r30;
    if ((s32)r28 == 2) goto L_80195E8C;
    if ((s32)r28 >= 2) goto L_80195AE0;
    if ((s32)r28 == 0) goto L_80195BEC;
    if ((s32)r28 >= 0) goto L_80195E80;
    goto L_80195E98;
L_80195AE0:
    if ((s32)r28 >= 4) goto L_80195E98;
    f3 = *(f32*)((u8*)r30 + 0x10);
    r3 = 0x0;
    f2 = *(f32*)((u8*)r30 + 0xC);
    f1 = *(f32*)((u8*)r30 + 0x18);
    f0 = *(f32*)((u8*)r30 + 0x14);
    f3 = f3 - f2;
    r12 = *(u32*)&lbl_80478C58;
    f4 = f1 - f0;
    f1 = *(f32*)((u8*)r30 + 0xC);
    f2 = *(f32*)((u8*)r30 + 0x14);
    f5 = *(f32*)lbl_8047D978;
    f6 = *(f32*)lbl_8047D9B0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = *(u16*)((u8*)r30 + 0x1C);
    r3 = *(u16*)((u8*)r30 + 0x1E);
    r6 = *(u16*)((u8*)r30 + 0x20);
    tmp = *(u16*)((u8*)r30 + 0x22);
    r5 = r3 - r4;
    r3 = *(u16*)((u8*)r30 + 0x1C);
    r4 = *(u16*)((u8*)r30 + 0x20);
    r6 = tmp - r6;
    fn_800BD7A0();
    tmp = *(u8*)((u8*)r30 + 0x50);
    if ((s32)tmp == 2) goto L_80195B8C;
    if ((s32)tmp >= 2) goto L_80195B60;
    if ((s32)tmp >= 1) goto L_80195B6C;
    goto L_80195BD8;
L_80195B60:
    if ((s32)tmp >= 4) goto L_80195BD8;
    goto L_80195BB4;
L_80195B6C:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x48;
    f2 = *(f32*)((u8*)r30 + 0x44);
    r27 = 0x0;
    f3 = *(f32*)((u8*)r30 + 0x38);
    f4 = *(f32*)((u8*)r30 + 0x3C);
    fn_800A3910();
    goto L_80195BD8;
L_80195B8C:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x48;
    f2 = *(f32*)((u8*)r30 + 0x44);
    r27 = 0x0;
    f3 = *(f32*)((u8*)r30 + 0x48);
    f4 = *(f32*)((u8*)r30 + 0x4C);
    f5 = *(f32*)((u8*)r30 + 0x38);
    f6 = *(f32*)((u8*)r30 + 0x3C);
    fn_800A3874();
    goto L_80195BD8;
L_80195BB4:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x48;
    f2 = *(f32*)((u8*)r30 + 0x44);
    r27 = 0x1;
    f3 = *(f32*)((u8*)r30 + 0x48);
    f4 = *(f32*)((u8*)r30 + 0x4C);
    f5 = *(f32*)((u8*)r30 + 0x38);
    f6 = *(f32*)((u8*)r30 + 0x3C);
    fn_800A39E0();
L_80195BD8:
    r3 = (u32)sp + 0x48;
    r4 = r27;
    fn_800BD2E0();
    r3 = 0x1;
    goto L_80195EB4;
L_80195BEC:
    r4 = (u32)lbl_80466BC0;
    r3 = *(u32*)lbl_80478C50;
    r10 = (u32)lbl_80466BC0;
    r8 = 0x43300000;
    r9 = *(u16*)((u8*)r10 + 0x4);
    r6 = 0x43300000;
    tmp = *(u32*)lbl_80478C54;
    r5 = 0x43300000;
    tmp = *(u16*)((u8*)r10 + 0x6);
    r3 = 0x43300000;
    f2 = *(f64*)lbl_8047D9C0;
    f1 = *(f64*)lbl_8047D970;
    f2 = f0 - f2;
    f6 = *(f64*)lbl_8047D9C0;
    *(u32*)(sp + 0x9C) = tmp;
    f1 = f0 - f1;
    tmp = *(u8*)((u8*)r10 + 0x18);
    f4 = *(f64*)lbl_8047D970;
    f30 = f2 / f1;
    f2 = *(f32*)((u8*)r30 + 0x10);
    f0 = *(f32*)((u8*)r30 + 0xC);
    f1 = *(f32*)((u8*)r30 + 0x14);
    f7 = f2 * f30;
    f5 = f5 - f6;
    f28 = f0 * f30;
    f0 = *(f32*)((u8*)r30 + 0x18);
    f2 = f3 - f4;
    f7 = (f32)f7;
    f28 = (f32)f28;
    f31 = f5 / f2;
    f27 = f1 * f31;
    f4 = f0 * f31;
    f27 = (f32)f27;
    f29 = f7 - f28;
    f4 = (f32)f4;
    f4 = f4 - f27;
    if (tmp == 0) goto L_80195CE0;
    r12 = *(u32*)&lbl_80478C58;
    f1 = f28;
    f2 = f27;
    f5 = *(f32*)lbl_8047D978;
    f3 = f29;
    f6 = *(f32*)lbl_8047D9B0;
    r3 = 0x1;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    goto L_80195D04;
L_80195CE0:
    r12 = *(u32*)&lbl_80478C58;
    f1 = f28;
    f2 = f27;
    f5 = *(f32*)lbl_8047D978;
    f3 = f29;
    f6 = *(f32*)lbl_8047D9B0;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80195D04:
    r5 = *(u16*)((u8*)r30 + 0x20);
    r4 = 0x43300000;
    r3 = *(u16*)((u8*)r30 + 0x22);
    tmp = 0x43300000;
    r6 = 0x43300000;
    r7 = *(u16*)((u8*)r30 + 0x1C);
    r5 = 0x43300000;
    r4 = *(u16*)((u8*)r30 + 0x1E);
    f3 = *(f64*)lbl_8047D9C0;
    *(u32*)(sp + 0x88) = tmp;
    f1 = *(f64*)lbl_8047D9C0;
    f2 = f2 - f3;
    f0 = f0 - f1;
    f3 = *(f64*)lbl_8047D9C0;
    f27 = f2 * f31;
    f1 = *(f64*)lbl_8047D9C0;
    f4 = f0 * f31;
    f2 = f2 - f3;
    f27 = (f32)f27;
    f28 = f2 * f30;
    f0 = f0 - f1;
    f4 = (f32)f4;
    f28 = (f32)f28;
    f7 = f0 * f30;
    f4 = f4 - f27;
    f7 = (f32)f7;
    f1 = f4;
    f29 = f7 - f28;
    fn_800C46B0();
    f1 = f29;
    r27 = r3;
    fn_800C46B0();
    f1 = f27;
    r28 = r3;
    fn_800C46B0();
    f1 = f28;
    r29 = r3;
    fn_800C46B0();
    r4 = r29;
    r5 = r28;
    r6 = r27;
    fn_800BD7A0();
    tmp = *(u8*)((u8*)r30 + 0x50);
    if ((s32)tmp == 2) goto L_80195E20;
    if ((s32)tmp >= 2) goto L_80195DF4;
    if ((s32)tmp >= 1) goto L_80195E00;
    goto L_80195E6C;
L_80195DF4:
    if ((s32)tmp >= 4) goto L_80195E6C;
    goto L_80195E48;
L_80195E00:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x8;
    f2 = *(f32*)((u8*)r30 + 0x44);
    r31 = 0x0;
    f3 = *(f32*)((u8*)r30 + 0x38);
    f4 = *(f32*)((u8*)r30 + 0x3C);
    fn_800A3910();
    goto L_80195E6C;
L_80195E20:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x8;
    f2 = *(f32*)((u8*)r30 + 0x44);
    r31 = 0x0;
    f3 = *(f32*)((u8*)r30 + 0x48);
    f4 = *(f32*)((u8*)r30 + 0x4C);
    f5 = *(f32*)((u8*)r30 + 0x38);
    f6 = *(f32*)((u8*)r30 + 0x3C);
    fn_800A3874();
    goto L_80195E6C;
L_80195E48:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x8;
    f2 = *(f32*)((u8*)r30 + 0x44);
    r31 = 0x1;
    f3 = *(f32*)((u8*)r30 + 0x48);
    f4 = *(f32*)((u8*)r30 + 0x4C);
    f5 = *(f32*)((u8*)r30 + 0x38);
    f6 = *(f32*)((u8*)r30 + 0x3C);
    fn_800A39E0();
L_80195E6C:
    r3 = (u32)sp + 0x8;
    r4 = r31;
    fn_800BD2E0();
    r3 = 0x1;
    goto L_80195EB4;
L_80195E80:
    r3 = r30;
    fn_801963E0();
    goto L_80195EB4;
L_80195E8C:
    r3 = r30;
    fn_801960C4();
    goto L_80195EB4;
L_80195E98:
    r4 = (u32)lbl_80274680;
    r3 = (u32)lbl_8047D958;
    r5 = (u32)lbl_80274680;
    r4 = 0x2ab;
    fn_80196D78();
    r3 = 0x0;
    goto L_80195ED0;
L_80195EB4:
    if ((s32)r3 != 0) goto L_80195EC4;
    r3 = 0x0;
    goto L_80195ED0;
L_80195EC4:
    r3 = r30;
    ((void(*)(void))fn_80195F0C)();
    r3 = 0x1;
L_80195ED0:
    return;
}

/* 0x80195F0C | 0x1B8 */
void fn_80195F0C(HSD_CObj* cobj_param) {
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D9B0[];
    extern void fn_800A3458();
    extern void fn_80191688();
    extern void fn_801950D0();
    extern void fn_80196E10();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    cobj_param = cobj_param;
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    tmp = tmp & 0x00000002;
    if ((s32)tmp != 0) goto L_801960B0;
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    r4 = 0x1;
    r5 = 0x1;
    tmp = tmp & 0x40000000;
    if ((s32)tmp != 0) goto L_80195F70;
    tmp = *(u32*)((u8*)cobj_param + 0x24);
    r6 = 0x0;
    if (tmp == 0) goto L_80195F64;
cobj_param = (HSD_CObj*)((HSD_CObj*)*(u32*)((u8*)(u32)cobj_param + 0x24));
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    tmp = tmp & 0x00000002;
    if (tmp == 0) goto L_80195F64;
    r6 = 0x1;
L_80195F64:
    if ((s32)r6 != 0) goto L_80195F70;
    r5 = 0x0;
L_80195F70:
    if ((s32)r5 != 0) goto L_80195FA8;
    tmp = *(u32*)((u8*)cobj_param + 0x28);
    r5 = 0x0;
    if (tmp == 0) goto L_80195F9C;
cobj_param = (HSD_CObj*)((HSD_CObj*)*(u32*)((u8*)(u32)cobj_param + 0x28));
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    tmp = tmp & 0x00000002;
    if (tmp == 0) goto L_80195F9C;
    r5 = 0x1;
L_80195F9C:
    if ((s32)r5 != 0) goto L_80195FA8;
    r4 = 0x0;
L_80195FA8:
    if ((s32)r4 == 0) goto L_801960B0;
if ((u32)cobj_param != 0) goto L_80195FC8;
cobj_param = (HSD_CObj*)(u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80195FC8:
if ((u32)cobj_param != 0) goto L_80195FE0;
cobj_param = (HSD_CObj*)(u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80195FE0:
cobj_param = (HSD_CObj*)((HSD_CObj*)*(u32*)((u8*)(u32)cobj_param + 0x24));
    r4 = (u32)sp + 0x20;
    fn_80191688();
    cobj_param = cobj_param;
    r4 = (u32)sp + 0x14;
    fn_801950D0();
    if ((s32)cobj_param != 0) goto L_80196018;
    f2 = *(f32*)lbl_8047D978;
    f1 = *(f32*)lbl_8047D9B0;
    f0 = *(f32*)lbl_8047D978;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f1;
    *(f32*)(sp + 0x1C) = f0;
L_80196018:
if ((u32)cobj_param != 0) goto L_80196030;
cobj_param = (HSD_CObj*)(u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196030:
if ((u32)cobj_param != 0) goto L_80196048;
cobj_param = (HSD_CObj*)(u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196048:
cobj_param = (HSD_CObj*)((HSD_CObj*)*(u32*)((u8*)(u32)cobj_param + 0x28));
    r4 = (u32)sp + 0x8;
    fn_80191688();
cobj_param = (HSD_CObj*)((HSD_CObj*)((u32)(u32)cobj_param + 0x54));
    r4 = (u32)sp + 0x20;
    r5 = (u32)sp + 0x14;
    r6 = (u32)sp + 0x8;
    fn_800A3458();
cobj_param = (HSD_CObj*)((HSD_CObj*)*(u32*)((u8*)(u32)cobj_param + 0x24));
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    tmp = tmp & 0xFFFFFFFD;
    *(u32*)((u8*)cobj_param + 0x8) = tmp;
cobj_param = (HSD_CObj*)((HSD_CObj*)*(u32*)((u8*)(u32)cobj_param + 0x28));
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    tmp = tmp & 0xFFFFFFFD;
    *(u32*)((u8*)cobj_param + 0x8) = tmp;
if ((u32)cobj_param == 0) goto L_8019609C;
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    tmp = tmp & 0xBFFFFFFF;
    *(u32*)((u8*)cobj_param + 0x8) = tmp;
L_8019609C:
if ((u32)cobj_param == 0) goto L_801960B0;
    tmp = *(u32*)((u8*)cobj_param + 0x8);
    tmp = tmp | (0x8000 << 16);
    *(u32*)((u8*)cobj_param + 0x8) = tmp;
L_801960B0:
    return;
}

/* 0x801960C4 | 0x31C */
void fn_801960C4(void) {
    extern u8 lbl_80466BC0[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D998[];
    extern u8 lbl_8047D9B0[];
    extern u8 lbl_8047D9C0[];
    extern u8 lbl_8047D9C8[];
    extern u8 lbl_8047D9D0[];
    extern u8 lbl_8047D9D4[];
    extern void fn_800A3874();
    extern void fn_800A39E0();
    extern void fn_800BD2E0();
    extern void fn_800BD7A0();
    extern void fn_800C46B0();
    extern void fn_800CE220();
    extern u8 lbl_80478C58;
    u8 sp[0xD0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r4 = (u32)lbl_80466BC0;
    r29 = r3;
    r4 = (u32)lbl_80466BC0;
    tmp = 0x43300000;
    r3 = *(u16*)((u8*)r4 + 0x6);
    *(u32*)(sp + 0x48) = tmp;
    f1 = *(f64*)lbl_8047D9C0;
    f2 = *(f32*)((u8*)r29 + 0x18);
    f0 = f0 - f1;
    if (f2 >= f0) goto L_8019612C;
    r3 = 0x0;
    goto L_801963B4;
L_8019612C:
    tmp = *(u16*)((u8*)r29 + 0x1C);
    r5 = 0x43300000;
    r4 = *(u16*)((u8*)r29 + 0x1E);
    r3 = 0x43300000;
    *(u32*)(sp + 0x4C) = tmp;
    tmp = *(u16*)((u8*)r29 + 0x20);
    f2 = *(f64*)lbl_8047D9C0;
    f1 = *(f64*)lbl_8047D9C0;
    f30 = f0 - f2;
    f3 = f0 - f1;
    if (tmp <= r30) goto L_80196178;
    tmp = *(u16*)((u8*)r29 + 0x20);
    goto L_8019617C;
L_80196178:
    tmp = r30;
L_8019617C:
    r3 = *(u16*)((u8*)r29 + 0x22);
    r5 = tmp - r30;
    r4 = 0x43300000;
    tmp = 0x43300000;
    r3 = r3 - r30;
    f2 = *(f64*)lbl_8047D9C0;
    f29 = f3 - f30;
    f1 = *(f64*)lbl_8047D9C0;
    f31 = f0 - f2;
    *(u32*)(sp + 0x60) = tmp;
    f4 = f0 - f1;
    f4 = f4 - f31;
    f1 = f4;
    fn_800C46B0();
    f1 = f29;
    r26 = r3;
    fn_800C46B0();
    f1 = f31;
    r27 = r3;
    fn_800C46B0();
    f1 = f30;
    r28 = r3;
    fn_800C46B0();
    r4 = r28;
    r5 = r27;
    r6 = r26;
    fn_800BD7A0();
    tmp = 0x43300000;
    f1 = *(f64*)lbl_8047D9C0;
    *(u32*)(sp + 0x68) = tmp;
    f2 = *(f32*)((u8*)r29 + 0x14);
    f30 = *(f32*)((u8*)r29 + 0xC);
    f0 = f0 - f1;
    f3 = *(f32*)((u8*)r29 + 0x10);
    if (f2 <= f0) goto L_80196230;
    f6 = *(f32*)((u8*)r29 + 0x14);
    goto L_80196248;
L_80196230:
    tmp = 0x43300000;
    f1 = *(f64*)lbl_8047D9C0;
    *(u32*)(sp + 0x70) = tmp;
    f6 = f0 - f1;
L_80196248:
    r3 = 0x43300000;
    tmp = 0x43300000;
    f29 = f3 - f30;
    f2 = *(f64*)lbl_8047D9C0;
    f1 = f30;
    r3 = 0x0;
    f5 = *(f64*)lbl_8047D9C0;
    f3 = f29;
    f0 = f0 - f2;
    f7 = *(f32*)((u8*)r29 + 0x18);
    *(u32*)(sp + 0x80) = tmp;
    f2 = *(f32*)((u8*)r29 + 0x18);
    f31 = f6 - f0;
    f0 = *(f32*)((u8*)r29 + 0x14);
    f4 = f4 - f5;
    r12 = *(u32*)&lbl_80478C58;
    f0 = f2 - f0;
    f2 = f31;
    f5 = *(f32*)lbl_8047D978;
    f4 = f7 - f4;
    f6 = *(f32*)lbl_8047D9B0;
    f4 = f4 - f31;
    f29 = f4 / f0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = *(u8*)((u8*)r29 + 0x50);
    if ((s32)tmp == 2) goto L_80196348;
    if ((s32)tmp >= 2) goto L_801962DC;
    if ((s32)tmp >= 1) goto L_801962E8;
    goto L_801963A4;
L_801962DC:
    if ((s32)tmp >= 4) goto L_801963A4;
    goto L_80196378;
L_801962E8:
    f1 = *(f64*)lbl_8047D998;
    r31 = 0x0;
    f0 = *(f32*)((u8*)r29 + 0x40);
    f2 = *(f64*)lbl_8047D9C8;
    f0 = f1 * f0;
    f1 = f2 * f0;
    f1 = (f32)f1;
    fn_800CE220();
    f4 = (f32)f1;
    f3 = *(f32*)((u8*)r29 + 0x38);
    f2 = *(f32*)lbl_8047D9D4;
    r3 = (u32)sp + 0x8;
    f0 = *(f32*)lbl_8047D9D0;
    f1 = *(f32*)((u8*)r29 + 0x44);
    f0 = f2 * f29 + f0;
    f5 = *(f32*)((u8*)r29 + 0x38);
    f2 = f3 * f4;
    f6 = *(f32*)((u8*)r29 + 0x3C);
    f4 = f2 * f1;
    f1 = f2 * f0;
    f2 = -f2;
    f3 = -f4;
    fn_800A3874();
    goto L_801963A4;
L_80196348:
    f2 = *(f32*)((u8*)r29 + 0x44);
    r3 = (u32)sp + 0x8;
    f0 = *(f32*)((u8*)r29 + 0x40);
    r31 = 0x0;
    f3 = *(f32*)((u8*)r29 + 0x48);
    f0 = f0 - f2;
    f4 = *(f32*)((u8*)r29 + 0x4C);
    f5 = *(f32*)((u8*)r29 + 0x38);
    f6 = *(f32*)((u8*)r29 + 0x3C);
    f1 = f29 * f0 + f2;
    fn_800A3874();
    goto L_801963A4;
L_80196378:
    f2 = *(f32*)((u8*)r29 + 0x44);
    r3 = (u32)sp + 0x8;
    f0 = *(f32*)((u8*)r29 + 0x40);
    r31 = 0x1;
    f3 = *(f32*)((u8*)r29 + 0x48);
    f0 = f0 - f2;
    f4 = *(f32*)((u8*)r29 + 0x4C);
    f5 = *(f32*)((u8*)r29 + 0x38);
    f6 = *(f32*)((u8*)r29 + 0x3C);
    f1 = f29 * f0 + f2;
    fn_800A39E0();
L_801963A4:
    r4 = r31;
    r3 = (u32)sp + 0x8;
    fn_800BD2E0();
    r3 = 0x1;
L_801963B4:
    return;
}

/* 0x801963E0 | 0x2B8 */
void fn_801963E0(void) {
    extern u8 lbl_80466BC0[];
    extern u8 lbl_8047D978[];
    extern u8 lbl_8047D998[];
    extern u8 lbl_8047D9B0[];
    extern u8 lbl_8047D9C0[];
    extern u8 lbl_8047D9C8[];
    extern u8 lbl_8047D9D4[];
    extern void fn_800A3874();
    extern void fn_800A39E0();
    extern void fn_800BD2E0();
    extern void fn_800BD7A0();
    extern void fn_800C46B0();
    extern void fn_800CE220();
    extern u8 lbl_80478C58;
    u8 sp[0xB0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r4 = (u32)lbl_80466BC0;
    tmp = 0x43300000;
    r4 = (u32)lbl_80466BC0;
    r30 = r3;
    r26 = r4;
    *(u32*)(sp + 0x48) = tmp;
    tmp = *(u16*)((u8*)r4 + 0x6);
    f1 = *(f64*)lbl_8047D9C0;
    *(u32*)(sp + 0x4C) = tmp;
    f2 = *(f32*)((u8*)r3 + 0x14);
    f0 = f0 - f1;
    /* cror eq, gt, eq */;
    if (f2 != f0) goto L_8019644C;
    r3 = 0x0;
    goto L_8019666C;
L_8019644C:
    r3 = *(u16*)((u8*)r26 + 0x6);
    tmp = 0x43300000;
    *(u32*)(sp + 0x48) = tmp;
    f1 = *(f64*)lbl_8047D9C0;
    f2 = *(f32*)((u8*)r30 + 0x18);
    f30 = *(f32*)((u8*)r30 + 0xC);
    f0 = f0 - f1;
    f3 = *(f32*)((u8*)r30 + 0x10);
    f31 = *(f32*)((u8*)r30 + 0x14);
    if (f2 >= f0) goto L_80196488;
    f0 = *(f32*)((u8*)r30 + 0x18);
    goto L_801964A4;
L_80196488:
    r3 = *(u16*)((u8*)r26 + 0x6);
    tmp = 0x43300000;
    *(u32*)(sp + 0x50) = tmp;
    f1 = *(f64*)lbl_8047D9C0;
    f0 = f0 - f1;
L_801964A4:
    f4 = f0 - f31;
    f29 = f3 - f30;
    f1 = f4;
    fn_800C46B0();
    f1 = f29;
    r27 = r3;
    fn_800C46B0();
    f1 = f31;
    r28 = r3;
    fn_800C46B0();
    f1 = f30;
    r29 = r3;
    fn_800C46B0();
    r4 = r29;
    r5 = r28;
    r6 = r27;
    fn_800BD7A0();
    r3 = *(u16*)((u8*)r26 + 0x6);
    tmp = 0x43300000;
    *(u32*)(sp + 0x58) = tmp;
    f1 = *(f64*)lbl_8047D9C0;
    f2 = *(f32*)((u8*)r30 + 0x18);
    f30 = *(f32*)((u8*)r30 + 0xC);
    f0 = f0 - f1;
    f3 = *(f32*)((u8*)r30 + 0x10);
    f31 = *(f32*)((u8*)r30 + 0x14);
    if (f2 >= f0) goto L_80196524;
    f0 = *(f32*)((u8*)r30 + 0x18);
    goto L_80196540;
L_80196524:
    r3 = *(u16*)((u8*)r26 + 0x6);
    tmp = 0x43300000;
    *(u32*)(sp + 0x60) = tmp;
    f1 = *(f64*)lbl_8047D9C0;
    f0 = f0 - f1;
L_80196540:
    f4 = f0 - f31;
    f2 = *(f32*)((u8*)r30 + 0x18);
    f0 = *(f32*)((u8*)r30 + 0x14);
    f29 = f3 - f30;
    r12 = *(u32*)&lbl_80478C58;
    f1 = f30;
    f0 = f2 - f0;
    f5 = *(f32*)lbl_8047D978;
    f3 = f29;
    f6 = *(f32*)lbl_8047D9B0;
    f2 = f31;
    r3 = 0x0;
    f29 = f4 / f0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = *(u8*)((u8*)r30 + 0x50);
    if ((s32)tmp == 2) goto L_80196600;
    if ((s32)tmp >= 2) goto L_80196598;
    if ((s32)tmp >= 1) goto L_801965A4;
    goto L_8019665C;
L_80196598:
    if ((s32)tmp >= 4) goto L_8019665C;
    goto L_80196630;
L_801965A4:
    f1 = *(f64*)lbl_8047D998;
    r31 = 0x0;
    f0 = *(f32*)((u8*)r30 + 0x40);
    f2 = *(f64*)lbl_8047D9C8;
    f0 = f1 * f0;
    f1 = f2 * f0;
    f1 = (f32)f1;
    fn_800CE220();
    f1 = (f32)f1;
    f3 = *(f32*)((u8*)r30 + 0x38);
    f2 = *(f32*)lbl_8047D9D4;
    r3 = (u32)sp + 0x8;
    f0 = *(f32*)lbl_8047D9B0;
    f1 = f3 * f1;
    f0 = -(f2 * f29 - f0);
    f2 = *(f32*)((u8*)r30 + 0x44);
    f5 = *(f32*)((u8*)r30 + 0x38);
    f4 = f1 * f2;
    f6 = *(f32*)((u8*)r30 + 0x3C);
    f2 = f1 * f0;
    f3 = -f4;
    fn_800A3874();
    goto L_8019665C;
L_80196600:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x8;
    f0 = *(f32*)((u8*)r30 + 0x44);
    r31 = 0x0;
    f3 = *(f32*)((u8*)r30 + 0x48);
    f0 = f1 - f0;
    f4 = *(f32*)((u8*)r30 + 0x4C);
    f5 = *(f32*)((u8*)r30 + 0x38);
    f6 = *(f32*)((u8*)r30 + 0x3C);
    f2 = -(f29 * f0 - f1);
    fn_800A3874();
    goto L_8019665C;
L_80196630:
    f1 = *(f32*)((u8*)r30 + 0x40);
    r3 = (u32)sp + 0x8;
    f0 = *(f32*)((u8*)r30 + 0x44);
    r31 = 0x1;
    f3 = *(f32*)((u8*)r30 + 0x48);
    f0 = f1 - f0;
    f4 = *(f32*)((u8*)r30 + 0x4C);
    f5 = *(f32*)((u8*)r30 + 0x38);
    f6 = *(f32*)((u8*)r30 + 0x3C);
    f2 = -(f29 * f0 - f1);
    fn_800A39E0();
L_8019665C:
    r4 = r31;
    r3 = (u32)sp + 0x8;
    fn_800BD2E0();
    r3 = 0x1;
L_8019666C:
    return;
}

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
void fn_8019674C(void) {
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern void fn_80191688();
    extern void fn_80191788();
    extern void fn_801947C8();
    extern void fn_80196E10();
    extern u8 jumptable_8036C6E0[];
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    r30 = r5;
    if ((s32)tmp == 0) goto L_80196AF8;
    if (r4 > 0xc) goto L_80196AF8;
    r3 = (u32)jumptable_8036C6E0;
    tmp = r4 << 2;
    r3 = (u32)jumptable_8036C6E0;
    r3 = *(u32*)(r3 + tmp);
    ctr_fn = (void(*)(void))r3;
    if (r31 != 0) goto L_801967A4;
    r3 = (u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801967A4:
    if (r31 != 0) goto L_801967BC;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801967BC:
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0x8) = f0;
    if (r31 != 0) goto L_801967E8;
    r3 = (u32)lbl_8047D958;
    r4 = 0x324;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801967E8:
    if (r31 != 0) goto L_80196800;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196800:
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191788();
    goto L_80196AF8;
    if (r31 != 0) goto L_80196828;
    r3 = (u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196828:
    if (r31 != 0) goto L_80196840;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196840:
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0xC) = f0;
    if (r31 != 0) goto L_8019686C;
    r3 = (u32)lbl_8047D958;
    r4 = 0x324;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_8019686C:
    if (r31 != 0) goto L_80196884;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196884:
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191788();
    goto L_80196AF8;
    if (r31 != 0) goto L_801968AC;
    r3 = (u32)lbl_8047D958;
    r4 = 0x318;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801968AC:
    if (r31 != 0) goto L_801968C4;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801968C4:
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0x10) = f0;
    if (r31 != 0) goto L_801968F0;
    r3 = (u32)lbl_8047D958;
    r4 = 0x324;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801968F0:
    if (r31 != 0) goto L_80196908;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196908:
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = (u32)sp + 0x8;
    fn_80191788();
    goto L_80196AF8;
    if (r31 != 0) goto L_80196930;
    r3 = (u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196930:
    if (r31 != 0) goto L_80196948;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196948:
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0x8) = f0;
    if (r31 != 0) goto L_80196974;
    r3 = (u32)lbl_8047D958;
    r4 = 0x30c;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196974:
    if (r31 != 0) goto L_8019698C;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_8019698C:
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = (u32)sp + 0x8;
    fn_80191788();
    goto L_80196AF8;
    if (r31 != 0) goto L_801969B4;
    r3 = (u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801969B4:
    if (r31 != 0) goto L_801969CC;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801969CC:
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0x8) = f0;
    if (r31 != 0) goto L_801969F8;
    r3 = (u32)lbl_8047D958;
    r4 = 0x30c;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_801969F8:
    if (r31 != 0) goto L_80196A10;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196A10:
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = (u32)sp + 0x8;
    fn_80191788();
    goto L_80196AF8;
    if (r31 != 0) goto L_80196A38;
    r3 = (u32)lbl_8047D958;
    r4 = 0x300;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196A38:
    if (r31 != 0) goto L_80196A50;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196A50:
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = (u32)sp + 0x8;
    fn_80191688();
    f0 = *(f32*)((u8*)r30 + 0x0);
    *(f32*)(sp + 0x8) = f0;
    if (r31 != 0) goto L_80196A7C;
    r3 = (u32)lbl_8047D958;
    r4 = 0x30c;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196A7C:
    if (r31 != 0) goto L_80196A94;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196A94:
    r3 = *(u32*)((u8*)r31 + 0x28);
    r4 = (u32)sp + 0x8;
    fn_80191788();
    goto L_80196AF8;
    f1 = *(f32*)((u8*)r30 + 0x0);
    r3 = r31;
    fn_801947C8();
    goto L_80196AF8;
    f0 = *(f32*)((u8*)r30 + 0x0);
    if (r31 == 0) goto L_80196AF8;
    tmp = *(u8*)((u8*)r31 + 0x50);
    if (tmp != 1) goto L_80196AF8;
    *(f32*)((u8*)r31 + 0x40) = f0;
    goto L_80196AF8;
    f0 = *(f32*)((u8*)r30 + 0x0);
    if (r31 == 0) goto L_80196AF8;
    *(f32*)((u8*)r31 + 0x38) = f0;
    goto L_80196AF8;
    f0 = *(f32*)((u8*)r30 + 0x0);
    if (r31 == 0) goto L_80196AF8;
    *(f32*)((u8*)r31 + 0x3C) = f0;
L_80196AF8:
    return;
}

/* 0x80196B10 | 0xA8 */
void fn_80196B10(void) {
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern void fn_80191DCC();
    extern void fn_80196E10();
    extern void fn_801C25E4();
    extern void fn_801C2670();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) goto L_80196BA0;
    if (r31 == 0) goto L_80196BA0;
    tmp = *(u32*)((u8*)r30 + 0x84);
    if (tmp == 0) goto L_80196B4C;
    r3 = *(u32*)((u8*)r30 + 0x84);
    fn_801C25E4();
L_80196B4C:
    r3 = *(u32*)((u8*)r31 + 0x0);
    fn_801C2670();
    *(u32*)((u8*)r30 + 0x84) = r3;
    if (r30 != 0) goto L_80196B70;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196B70:
    r3 = *(u32*)((u8*)r30 + 0x24);
    r4 = *(u32*)((u8*)r31 + 0x4);
    fn_80191DCC();
    if (r30 != 0) goto L_80196B94;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196B94:
    r3 = *(u32*)((u8*)r30 + 0x28);
    r4 = *(u32*)((u8*)r31 + 0x8);
    fn_80191DCC();
L_80196BA0:
    return;
}

/* 0x80196BB8 | 0x84 */
void fn_80196BB8(void) {
    extern u8 lbl_8047D958[];
    extern u8 lbl_8047D960[];
    extern void fn_80191E88();
    extern void fn_80196E10();
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) goto L_80196C28;
    if (r31 == 0) goto L_80196C28;
    r3 = *(u32*)((u8*)r31 + 0x84);
    fn_801C25E4();
    tmp = 0x0;
    *(u32*)((u8*)r31 + 0x84) = tmp;
    if (r31 != 0) goto L_80196C00;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2e8;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196C00:
    r3 = *(u32*)((u8*)r31 + 0x24);
    fn_80191E88();
    if (r31 != 0) goto L_80196C20;
    r3 = (u32)lbl_8047D958;
    r4 = 0x2d0;
    r5 = (u32)lbl_8047D960;
    fn_80196E10();
L_80196C20:
    r3 = *(u32*)((u8*)r31 + 0x28);
    fn_80191E88();
L_80196C28:
    return;
}

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
void fn_80196C54(void) {
    extern void fn_800AA2F0();
    extern void fn_800BD640();
    extern void fn_800BD744();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r11 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r11 = (u32)sp + 0x40;
    f26 = f1;
    f27 = f2;
    f28 = f3;
    f29 = f4;
    f30 = f5;
    f31 = f6;
    if ((s32)r3 == 0) goto L_80196CAC;
    fn_800AA2F0();
    f1 = f26;
    f2 = f27;
    f3 = f28;
    f4 = f29;
    f5 = f30;
    f6 = f31;
    fn_800BD640();
    goto L_80196CC8;
L_80196CAC:
    f1 = f26;
    f2 = f27;
    f3 = f28;
    f4 = f29;
    f5 = f30;
    f6 = f31;
    fn_800BD744();
L_80196CC8:
    r11 = (u32)sp + 0x40;
    return;
}

/* 0x80196CE0 | 0x98 */
void fn_80196CE0(void) {
    extern u8 lbl_80465080[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    /* mtsprg 0, r3 */;
    r3 = (u32)lbl_80465080;
    r3 = (u32)lbl_80465080;
    /* mfsprg r4, 0 */;
    *(u32*)((u8*)r3 + 0xC) = r4;
    r4 = 0; /* mfspr GQR0 */;
    *(u32*)((u8*)r3 + 0x1A4) = r4;
    r4 = 0; /* mfspr GQR1 */;
    *(u32*)((u8*)r3 + 0x1A8) = r4;
    r4 = 0; /* mfspr GQR2 */;
    *(u32*)((u8*)r3 + 0x1AC) = r4;
    r4 = 0; /* mfspr GQR3 */;
    *(u32*)((u8*)r3 + 0x1B0) = r4;
    r4 = 0; /* mfspr GQR4 */;
    *(u32*)((u8*)r3 + 0x1B4) = r4;
    r4 = 0; /* mfspr GQR5 */;
    *(u32*)((u8*)r3 + 0x1B8) = r4;
    r4 = 0; /* mfspr GQR6 */;
    *(u32*)((u8*)r3 + 0x1BC) = r4;
    r4 = 0; /* mfspr GQR7 */;
    *(u32*)((u8*)r3 + 0x1C0) = r4;
    r4 = 0; /* mfcr */;
    *(u32*)((u8*)r3 + 0x80) = r4;
    *(u32*)((u8*)r3 + 0x84) = r4;
    r4 = 0; /* mfctr */;
    *(u32*)((u8*)r3 + 0x88) = r4;
    /* mfxer r4 */;
    *(u32*)((u8*)r3 + 0x8C) = r4;
    /* mfsrr0 r4 */;
    *(u32*)((u8*)r3 + 0x198) = r4;
    /* mfsrr1 r4 */;
    *(u32*)((u8*)r3 + 0x19C) = r4;
    r4 = *(u16*)((u8*)r3 + 0x1A2);
    r4 = r4 | 0x1;
    *(u16*)((u8*)r3 + 0x1A2) = r4;
    /* b fn_8009C1B4 */;
}

/* 0x44 | fn_80196EB4 | generic */
void fn_80196EB4(void) {
    /* refs: lbl_80478C64, lbl_80478C68, lbl_80478C6C, lbl_8047B24C, lbl_8047B250, lbl_8047B254, lbl_8047B258, lbl_8047B25C */
}

/* 0x80196EF8 | 0x424 */
void fn_80196EF8(void) {
    extern u8 lbl_8036C720[];
    extern u8 lbl_8036CBC0[];
    extern void fn_800B7874();
    extern void fn_800B7D3C();
    extern void fn_800B7D74();
    extern void fn_800B857C();
    extern void fn_800B884C();
    extern void fn_800B928C();
    extern void fn_800B94F0();
    extern void fn_800BA6B0();
    extern void fn_800BA6F4();
    extern void fn_800BA9E4();
    extern void fn_800BAFFC();
    extern void fn_800BC114();
    extern void fn_800BC618();
    extern void fn_800BC66C();
    extern void fn_800BC6F0();
    extern void fn_800BC8C8();
    extern void fn_800BCDDC();
    extern void fn_800BCE30();
    extern void fn_800BCE5C();
    extern void fn_800BCE88();
    extern void fn_800BCEBC();
    extern void fn_800BD4B4();
    extern void fn_800BD554();
    extern void fn_801B25C4();
    extern u8 lbl_80478C60;
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r11 = (u32)sp + 0x70;
    /* mr. r31, r3 */;
    f27 = f1;
    f28 = f2;
    r30 = r4;
    f29 = f3;
    r29 = r5;
    f30 = f4;
    f31 = f5;
    if ((s32)tmp != 0) goto L_80196F4C;
    if ((s32)r30 != 0) goto L_80196F4C;
    if ((s32)r29 == 0) goto L_801972F8;
L_80196F4C:
    if ((s32)r29 == 0) goto L_80196FE8;
    r4 = (u32)lbl_8036C720;
    r3 = (u32)sp + 0x10;
    r4 = (u32)lbl_8036C720;
    r5 = 0x4;
    r6 = 0x4;
    r7 = 0x11;
    r8 = 0x1;
    r9 = 0x1;
    r10 = 0x0;
    fn_800BA9E4();
    r3 = (u32)sp + 0x10;
    r4 = 0x0;
    fn_800BAFFC();
    r3 = 0x1;
    fn_800B884C();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x4;
    r6 = 0x3c;
    r7 = 0x0;
    r8 = 0x7d;
    fn_800B857C();
    r3 = 0x1;
    fn_800BC8C8();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x4;
    fn_800BC6F0();
    r3 = 0x0;
    r4 = 0x4;
    fn_800BC114();
    r3 = 0x2;
    r4 = 0x11;
    r5 = 0x0;
    fn_800BC66C();
    goto L_80197018;
L_80196FE8:
    r3 = 0x0;
    fn_800B884C();
    r3 = 0x1;
    fn_800BC8C8();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0x4;
    fn_800BC6F0();
    r3 = 0x0;
    r4 = 0x4;
    fn_800BC114();
L_80197018:
    r3 = 0x0;
    fn_800B94F0();
    r3 = 0x7;
    r4 = 0x0;
    r5 = 0x1;
    r6 = 0x7;
    r7 = 0x0;
    fn_800BC618();
    r3 = 0x1;
    fn_800BCEBC();
    tmp = -r29;
    r3 = 0x1;
    tmp = tmp | r29;
    r4 = 0x7;
    r5 = (u32)tmp >> 31;
    fn_800BCE88();
    r3 = 0x2;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x3;
    fn_800BCDDC();
    tmp = -r31;
    tmp = tmp | r31;
    r3 = (u32)tmp >> 31;
    fn_800BCE30();
    tmp = -r30;
    tmp = tmp | r30;
    r3 = (u32)tmp >> 31;
    fn_800BCE5C();
    r3 = 0x1;
    fn_800BA6B0();
    r3 = 0x4;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x0;
    r8 = 0x0;
    r9 = 0x2;
    fn_800BA6F4();
    fn_800B7D3C();
    r3 = 0x0;
    r4 = 0x9;
    r5 = 0x1;
    r6 = 0x4;
    r7 = 0x0;
    fn_800B7D74();
    r3 = 0x0;
    r4 = 0xb;
    r5 = 0x1;
    r6 = 0x5;
    r7 = 0x0;
    fn_800B7D74();
    r3 = 0x0;
    r4 = 0xd;
    r5 = 0x1;
    r6 = 0x0;
    r7 = 0x0;
    fn_800B7D74();
    r3 = (u32)lbl_8036CBC0;
    r4 = 0x0;
    r3 = (u32)lbl_8036CBC0;
    fn_800BD4B4();
    r3 = 0x0;
    fn_800BD554();
    r3 = 0x9;
    r4 = 0x1;
    fn_800B7874();
    r3 = 0xb;
    r4 = 0x1;
    fn_800B7874();
    r3 = 0xd;
    r4 = 0x1;
    fn_800B7874();
    tmp = *(u32*)&lbl_80478C60;
    r3 = 0x80;
    r4 = 0x0;
    r5 = 0x4;
    *(u32*)(sp + 0x8) = tmp;
    *(u32*)(sp + 0xC) = tmp;
    fn_800B928C();
    r3 = 0xCC010000;
    r4 = 0xCC010000;
    *(f32*)((u8*)r3 + (-32768)) = f29;
    r5 = 0xCC010000;
    tmp = *(u8*)(sp + 0xC);
    r3 = 0xCC010000;
    *(f32*)((u8*)r4 + (-32768)) = f27;
    r31 = 0xCC010000;
    r6 = *(u8*)(sp + 0xD);
    r12 = 0xCC010000;
    *(f32*)((u8*)r5 + (-32768)) = f31;
    r11 = 0x1;
    r7 = *(u8*)(sp + 0xE);
    r10 = 0xCC010000;
    *(u8*)((u8*)r3 + (-32768)) = tmp;
    r3 = 0xCC010000;
    r4 = *(u8*)(sp + 0xF);
    *(u8*)((u8*)r3 + (-32768)) = r6;
    r3 = 0xCC010000;
    r5 = *(u8*)(sp + 0xC);
    *(u8*)((u8*)r3 + (-32768)) = r7;
    r3 = 0xCC010000;
    r6 = *(u8*)(sp + 0xD);
    *(u8*)((u8*)r3 + (-32768)) = r4;
    r4 = 0x0;
    r3 = 0xCC010000;
    r8 = *(u8*)(sp + 0xE);
    *(u8*)((u8*)r3 + (-32768)) = r4;
    r4 = 0x0;
    r3 = 0xCC010000;
    r9 = *(u8*)(sp + 0xF);
    *(u8*)((u8*)r3 + (-32768)) = r4;
    r3 = 0xCC010000;
    r4 = 0xCC010000;
    r7 = *(u8*)(sp + 0xC);
    *(f32*)((u8*)r3 + (-32768)) = f30;
    r3 = 0xCC010000;
    tmp = *(u8*)(sp + 0xD);
    *(f32*)((u8*)r3 + (-32768)) = f27;
    r3 = 0xCC010000;
    r30 = *(u8*)(sp + 0xF);
    *(f32*)((u8*)r3 + (-32768)) = f31;
    r3 = *(u8*)(sp + 0xE);
    *(u8*)((u8*)r4 + (-32768)) = r5;
    r4 = 0xCC010000;
    r5 = 0xCC010000;
    r29 = *(u8*)(sp + 0xF);
    *(u8*)((u8*)r4 + (-32768)) = r6;
    r6 = 0x1;
    *(u8*)((u8*)r5 + (-32768)) = r8;
    r5 = 0xCC010000;
    r8 = 0x0;
    *(u8*)((u8*)r5 + (-32768)) = r9;
    r5 = 0xCC010000;
    r9 = 0xCC010000;
    *(u8*)((u8*)r5 + (-32768)) = r6;
    r6 = 0xCC010000;
    r5 = 0xCC010000;
    *(u8*)((u8*)r6 + (-32768)) = r8;
    r8 = 0xCC010000;
    r6 = 0xCC010000;
    *(f32*)((u8*)r5 + (-32768)) = f30;
    r5 = 0xCC010000;
    *(f32*)((u8*)r9 + (-32768)) = f28;
    r9 = 0x1;
    *(f32*)((u8*)r8 + (-32768)) = f31;
    r8 = 0xCC010000;
    *(u8*)((u8*)r6 + (-32768)) = r7;
    r7 = 0xCC010000;
    r6 = 0xCC010000;
    *(u8*)((u8*)r5 + (-32768)) = tmp;
    r5 = 0xCC010000;
    *(u8*)((u8*)r31 + (-32768)) = r3;
    *(u8*)((u8*)r12 + (-32768)) = r30;
    *(u8*)((u8*)r10 + (-32768)) = r11;
    *(u8*)((u8*)r8 + (-32768)) = r9;
    *(f32*)((u8*)r7 + (-32768)) = f29;
    *(f32*)((u8*)r6 + (-32768)) = f28;
    *(f32*)((u8*)r5 + (-32768)) = f31;
    tmp = *(u8*)(sp + 0xC);
    r4 = 0xCC010000;
    r5 = *(u8*)(sp + 0xD);
    r3 = 0xCC010000;
    *(u8*)((u8*)r4 + (-32768)) = tmp;
    r4 = 0xCC010000;
    r6 = *(u8*)(sp + 0xE);
    *(u8*)((u8*)r3 + (-32768)) = r5;
    r3 = 0xCC010000;
    tmp = r6 & 0xFF;
    r5 = 0x0;
    *(u8*)((u8*)r3 + (-32768)) = tmp;
    r3 = 0xCC010000;
    tmp = 0x1;
    r6 = 0xCC010000;
    *(u8*)((u8*)r3 + (-32768)) = r29;
    r3 = 0x0;
    *(u8*)((u8*)r4 + (-32768)) = r5;
    r4 = 0x11;
    r5 = 0x0;
    *(u8*)((u8*)r6 + (-32768)) = tmp;
    fn_800BC66C();
    r3 = -0x1;
    fn_801B25C4();
L_801972F8:
    r11 = (u32)sp + 0x70;
    return;
}

/* 0x8019731C | 0x20 */
extern u8 lbl_80478C60;
void fn_8019731C(u8 a, u8 b, u8 c, u8 d) {
    (&lbl_80478C60)[0] = a;
    (&lbl_80478C60)[1] = b;
    (&lbl_80478C60)[2] = c;
    (&lbl_80478C60)[3] = d;
}
