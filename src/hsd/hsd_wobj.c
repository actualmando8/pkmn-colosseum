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
    extern u8 lbl_80274468[];
    extern u8 lbl_80274480[];
    extern u8 lbl_8036C5F0[];
    extern u8 lbl_8036CC00[];
    extern void fn_80193B30();
    extern void fn_8019158C();
    extern void fn_801915D4();
    extern void fn_8019194C();
    extern void fn_80191A34();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r3 = (u32)lbl_8036C5F0;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274468;
    r6 = (u32)lbl_80274480;
    r3 = (u32)lbl_8036C5F0;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274468;
    r6 = (u32)lbl_80274480;
    r7 = 0x44;
    r8 = 0x20;
    fn_80193B30();
    r6 = (u32)fn_801915D4;
    r5 = (u32)fn_8019158C;
    r3 = (u32)fn_80191A34;
    r8 = (u32)lbl_8036C5F0;
    r9 = (u32)fn_801915D4;
    r6 = (u32)lbl_8036C5F0;
    r0 = (u32)fn_80191A34;
    r3 = (u32)lbl_8036C5F0;
    r8 = (u32)lbl_8036C5F0;
    r4 = (u32)fn_8019194C;
    r7 = (u32)fn_8019158C;
    r6 = (u32)lbl_8036C5F0;
    r5 = (u32)fn_8019194C;
    r4 = (u32)lbl_8036C5F0;
    r4 = (u32)lbl_8036C5F0;
    r3 = (u32)lbl_8036C5F0;
    *(u32*)((u8*)r8 + 0x30) = r9;
    *(u32*)((u8*)r6 + 0x38) = r7;
    *(u32*)((u8*)r4 + 0x3C) = r5;
    *(u32*)((u8*)r3 + 0x40) = r0;
    return;
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
    extern u8 lbl_8047D8D8[];
    extern u8 lbl_8047D8E0[];
    extern void fn_800A37CC();
    extern void fn_80196E10();
    extern void fn_8019D9DC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_8019176C;
    if ((u32)r31 != (u32)0x0) goto L_801916B8;
    goto L_8019176C;
L_801916B8: ;
    r0 = *(u32*)((u8*)r30 + 0x8);
    r0 = r0 & 0x1;
    if ((u32)r31 == (u32)0x0) goto L_80191754;
    r0 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191748;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r0 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191748;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r29 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r29 == (u32)0x0) goto L_80191738;
    if ((u32)r29 != (u32)0x0) goto L_80191708;
    r3 = (u32)lbl_8047D8D8;
    r4 = 0x25d;
    r5 = (u32)lbl_8047D8E0;
    fn_80196E10();
L_80191708: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r29 != (u32)0x0) goto L_80191728;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r29 == (u32)0x0) goto L_80191728;
    r3 = 0x1;
L_80191728: ;
    if ((s32)r3 == (s32)0x0) goto L_80191738;
    r3 = r29;
    fn_8019D9DC();
L_80191738: ;
    r3 = r29 + 0x44;
    r4 = r30 + 0xc;
    r5 = r30 + 0xc;
    fn_800A37CC();
L_80191748: ;
    r0 = *(u32*)((u8*)r30 + 0x8);
    /* clrrwi r0, r0, 1 */;
    *(u32*)((u8*)r30 + 0x8) = r0;
L_80191754: ;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r0 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)((u8*)r31 + 0x0) = r3;
    *(u32*)((u8*)r31 + 0x4) = r0;
    r0 = *(u32*)((u8*)r30 + 0x14);
    *(u32*)((u8*)r31 + 0x8) = r0;
L_8019176C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    extern u8 lbl_8036C5F0[];
    extern u8 lbl_8047B218[];
    extern u8 lbl_8047D8C8[];
    extern u8 lbl_8047D8D0[];
    extern void fn_80193748();
    extern void fn_80193828();
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_80191880;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_80191808;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_80193748();
    if ((u32)r3 != (u32)0x0) goto L_80191844;
L_80191808: ;
    r0 = *(u32*)lbl_8047B218;
    if ((u32)r0 == (u32)0x0) goto L_8019181C;
    r3 = *(u32*)lbl_8047B218;
    goto L_80191824;
L_8019181C: ;
    r3 = (u32)lbl_8036C5F0;
    r3 = (u32)lbl_8036C5F0;
L_80191824: ;
    fn_80193828();
    /* mr. r31, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_80191860;
    r3 = (u32)lbl_8047D8C8;
    r4 = 0x257;
    r5 = (u32)lbl_8047D8D0;
    fn_80196E10();
    goto L_80191860;
L_80191844: ;
    fn_80193828();
    /* mr. r31, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_80191860;
    r3 = (u32)lbl_8047D8C8;
    r4 = 0x104;
    r5 = (u32)lbl_8047D8D0;
    fn_80196E10();
L_80191860: ;
    r5 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r4 = r30;
    r12 = *(u32*)((u8*)r5 + 0x3C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r3 = r31;
    goto L_80191884;
L_80191880: ;
    r3 = 0x0;
L_80191884: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x8019189C | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019189C(void) {
    extern void fn_801AE50C();
    extern void fn_801AE5E8();
    extern void fn_801AEBE4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_80191934;
    if ((u32)r31 != (u32)0x0) goto L_801918C8;
    goto L_80191934;
L_801918C8: ;
    if ((u32)r30 == (u32)0x0) goto L_80191908;
    /* addic. r0, r31, 0x4 */;
    if ((u32)r30 == (u32)0x0) goto L_80191908;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    *(u32*)((u8*)r30 + 0xC) = r3;
    *(u32*)((u8*)r30 + 0x10) = r0;
    r0 = *(u32*)((u8*)r31 + 0xC);
    *(u32*)((u8*)r30 + 0x14) = r0;
    r0 = *(u32*)((u8*)r30 + 0x8);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r30 + 0x8) = r0;
    r0 = *(u32*)((u8*)r30 + 0x8);
    /* clrrwi r0, r0, 1 */;
    *(u32*)((u8*)r30 + 0x8) = r0;
L_80191908: ;
    r0 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_8019191C;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    fn_801AE50C();
L_8019191C: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801AE5E8();
    *(u32*)((u8*)r30 + 0x1C) = r3;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = *(u32*)((u8*)r31 + 0x10);
    fn_801AEBE4();
L_80191934: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x8019194C | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019194C(void) {
    extern void fn_801AE50C();
    extern void fn_801AE5E8();
    extern void fn_801AEBE4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)r0 == (s32)0) goto L_801919A4;
    /* addic. r0, r31, 0x4 */;
    if ((s32)r0 == (s32)0) goto L_801919A4;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    *(u32*)((u8*)r30 + 0xC) = r3;
    *(u32*)((u8*)r30 + 0x10) = r0;
    r0 = *(u32*)((u8*)r31 + 0xC);
    *(u32*)((u8*)r30 + 0x14) = r0;
    r0 = *(u32*)((u8*)r30 + 0x8);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r30 + 0x8) = r0;
    r0 = *(u32*)((u8*)r30 + 0x8);
    /* clrrwi r0, r0, 1 */;
    *(u32*)((u8*)r30 + 0x8) = r0;
L_801919A4: ;
    r0 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_801919B8;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    fn_801AE50C();
L_801919B8: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801AE5E8();
    *(u32*)((u8*)r30 + 0x1C) = r3;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = *(u32*)((u8*)r31 + 0x10);
    fn_801AEBE4();
    r3 = 0x0;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
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
    extern u8 lbl_8027448C[];
    extern u8 lbl_80274498[];
    extern u8 lbl_8047D8C8[];
    extern u8 lbl_8047D8D8[];
    extern u8 lbl_8047D8E0[];
    extern u8 lbl_8047D8E8[];
    extern u8 lbl_8047D8F0[];
    extern u8 lbl_8047D8F8[];
    extern u8 lbl_8047D900[];
    extern u8 lbl_8047D904[];
    extern void fn_800A37CC();
    extern void fn_80196E10();
    extern void fn_8019D9DC();
    extern void fn_801B1890();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x30) = f31;
    /* psq_st f31, 0x38(r1), 0, qr0 */;
    /* mr. r31, r3 */;
    r29 = r5;
    if ((s32)r0 == (s32)0) goto L_80191DA8;
    if ((s32)r4 == (s32)0x6) goto L_80191C34;
    if ((s32)r4 >= (s32)0x6) goto L_80191A7C;
    if ((s32)r4 == (s32)0x4) goto L_80191A88;
    if ((s32)r4 >= (s32)0x4) goto L_80191B78;
    goto L_80191DA8;
L_80191A7C: ;
    if ((s32)r4 >= (s32)0x8) goto L_80191DA8;
    goto L_80191CF0;
L_80191A88: ;
    f1 = *(f32*)((u8*)r29 + 0x0);
    f0 = *(f64*)lbl_8047D8E8;
    if (f1 >= f0) goto L_80191AA0;
    f0 = *(f32*)lbl_8047D8F0;
    *(f32*)((u8*)r29 + 0x0) = f0;
L_80191AA0: ;
    f1 = *(f64*)lbl_8047D8F8;
    f0 = *(f32*)((u8*)r29 + 0x0);
    if (f1 >= f0) goto L_80191AB8;
    f0 = *(f32*)lbl_8047D900;
    *(f32*)((u8*)r29 + 0x0) = f0;
L_80191AB8: ;
    r0 = *(u32*)((u8*)r31 + 0x18);
    if ((u32)r0 != (u32)0x0) goto L_80191AD8;
    r4 = (u32)lbl_8027448C;
    r3 = (u32)lbl_8047D8C8;
    r5 = (u32)lbl_8027448C;
    r4 = 0x98;
    fn_80196E10();
L_80191AD8: ;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r30 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r30 != (u32)0x0) goto L_80191AF8;
    r3 = (u32)lbl_8047D8C8;
    r4 = 0x9a;
    r5 = (u32)lbl_8047D904;
    fn_80196E10();
L_80191AF8: ;
    r0 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r0 != (u32)0x0) goto L_80191B18;
    r4 = (u32)lbl_80274498;
    r3 = (u32)lbl_8047D8C8;
    r5 = (u32)lbl_80274498;
    r4 = 0x9b;
    fn_80196E10();
L_80191B18: ;
    r4 = *(u32*)((u8*)r30 + 0x18);
    r3 = r1 + 0x8;
    f1 = *(f32*)((u8*)r29 + 0x0);
    fn_801B1890();
    if ((u32)r31 == (u32)0x0) goto L_80191B68;
    /* addic. r0, r1, 0x8 */;
    if ((u32)r31 == (u32)0x0) goto L_80191B68;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    *(u32*)((u8*)r31 + 0xC) = r3;
    *(u32*)((u8*)r31 + 0x10) = r0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r31 + 0x8) = r0;
    r0 = *(u32*)((u8*)r31 + 0x8);
    /* clrrwi r0, r0, 1 */;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80191B68: ;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 | 0x1;
    *(u32*)((u8*)r31 + 0x8) = r0;
    goto L_80191DA8;
L_80191B78: ;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if ((u32)r31 == (u32)0x0) goto L_80191DA8;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 & 0x1;
    if ((u32)r31 == (u32)0x0) goto L_80191C20;
    r0 = *(u32*)((u8*)r31 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191C14;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r0 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191C14;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r30 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r30 == (u32)0x0) goto L_80191C04;
    if ((u32)r30 != (u32)0x0) goto L_80191BD4;
    r3 = (u32)lbl_8047D8D8;
    r4 = 0x25d;
    r5 = (u32)lbl_8047D8E0;
    fn_80196E10();
L_80191BD4: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r30 != (u32)0x0) goto L_80191BF4;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r30 == (u32)0x0) goto L_80191BF4;
    r3 = 0x1;
L_80191BF4: ;
    if ((s32)r3 == (s32)0x0) goto L_80191C04;
    r3 = r30;
    fn_8019D9DC();
L_80191C04: ;
    r3 = r30 + 0x44;
    r4 = r31 + 0xc;
    r5 = r31 + 0xc;
    fn_800A37CC();
L_80191C14: ;
    r0 = *(u32*)((u8*)r31 + 0x8);
    /* clrrwi r0, r0, 1 */;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80191C20: ;
    *(f32*)((u8*)r31 + 0xC) = f31;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r31 + 0x8) = r0;
    goto L_80191DA8;
L_80191C34: ;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if ((u32)r31 == (u32)0x0) goto L_80191DA8;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 & 0x1;
    if ((u32)r31 == (u32)0x0) goto L_80191CDC;
    r0 = *(u32*)((u8*)r31 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191CD0;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r0 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191CD0;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r30 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r30 == (u32)0x0) goto L_80191CC0;
    if ((u32)r30 != (u32)0x0) goto L_80191C90;
    r3 = (u32)lbl_8047D8D8;
    r4 = 0x25d;
    r5 = (u32)lbl_8047D8E0;
    fn_80196E10();
L_80191C90: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r30 != (u32)0x0) goto L_80191CB0;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r30 == (u32)0x0) goto L_80191CB0;
    r3 = 0x1;
L_80191CB0: ;
    if ((s32)r3 == (s32)0x0) goto L_80191CC0;
    r3 = r30;
    fn_8019D9DC();
L_80191CC0: ;
    r3 = r30 + 0x44;
    r4 = r31 + 0xc;
    r5 = r31 + 0xc;
    fn_800A37CC();
L_80191CD0: ;
    r0 = *(u32*)((u8*)r31 + 0x8);
    /* clrrwi r0, r0, 1 */;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80191CDC: ;
    *(f32*)((u8*)r31 + 0x10) = f31;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r31 + 0x8) = r0;
    goto L_80191DA8;
L_80191CF0: ;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if ((u32)r31 == (u32)0x0) goto L_80191DA8;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 & 0x1;
    if ((u32)r31 == (u32)0x0) goto L_80191D98;
    r0 = *(u32*)((u8*)r31 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191D8C;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r0 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80191D8C;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r30 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r30 == (u32)0x0) goto L_80191D7C;
    if ((u32)r30 != (u32)0x0) goto L_80191D4C;
    r3 = (u32)lbl_8047D8D8;
    r4 = 0x25d;
    r5 = (u32)lbl_8047D8E0;
    fn_80196E10();
L_80191D4C: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r30 != (u32)0x0) goto L_80191D6C;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r30 == (u32)0x0) goto L_80191D6C;
    r3 = 0x1;
L_80191D6C: ;
    if ((s32)r3 == (s32)0x0) goto L_80191D7C;
    r3 = r30;
    fn_8019D9DC();
L_80191D7C: ;
    r3 = r30 + 0x44;
    r4 = r31 + 0xc;
    r5 = r31 + 0xc;
    fn_800A37CC();
L_80191D8C: ;
    r0 = *(u32*)((u8*)r31 + 0x8);
    /* clrrwi r0, r0, 1 */;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80191D98: ;
    *(f32*)((u8*)r31 + 0x14) = f31;
    r0 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80191DA8: ;
    /* psq_l f31, 0x38(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x30);
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    return;
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
    extern void fn_800CA7FC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = 0x0;
    r30 = r4;
    r29 = r3;
    goto L_80191F38;
L_80191EF4: ;
    r3 = *(u32*)((u8*)r29 + 0x28);
    r0 = r31 << 3;
    r5 = *(u32*)((u8*)r29 + 0x30);
    r4 = r30;
    r3 = r3 + r0;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r3 = r5 + r0;
    fn_800CA7FC();
    if ((s32)r3 != (s32)0x0) goto L_80191F34;
    r3 = *(u32*)((u8*)r29 + 0x28);
    r0 = r31 << 3;
    r4 = *(u32*)((u8*)r29 + 0x20);
    r0 = *(u32*)(r3 + r0);
    r3 = r4 + r0;
    goto L_80191F48;
L_80191F34: ;
    r31 = r31 + 0x1;
L_80191F38: ;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r31 < (u32)r0) goto L_80191EF4;
    r3 = 0x0;
L_80191F48: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80191F64 | 0x180 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80191F64(void) {
    extern u8 lbl_802744A8[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = 0x0;
    r30 = r4;
    /* mr. r29, r3 */;
    r28 = r5;
    if ((s32)r0 != (s32)0) goto L_80191F9C;
    r3 = -0x1;
    goto L_801920C4;
L_80191F9C: ;
    r3 = r29;
    r4 = 0x0;
    r5 = 0x44;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = *(u32*)((u8*)r29 + 0x3C);
    r3 = r29;
    r4 = r30;
    r5 = 0x20;
    r0 = r0 | 0x1;
    *(u32*)((u8*)r29 + 0x3C) = r0;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)((u8*)r29 + 0x0);
    if ((u32)r0 == (u32)r28) goto L_80191FEC;
    r3 = (u32)lbl_802744A8;
    r3 = (u32)lbl_802744A8;
    /* crclr cr1eq */;
    OSReport();
    r3 = -0x1;
    goto L_801920C4;
L_80191FEC: ;
    r0 = *(u32*)((u8*)r29 + 0x4);
    r31 = r31 + 0x20;
    if ((u32)r0 == (u32)0x0) goto L_8019200C;
    r0 = r30 + r31;
    *(u32*)((u8*)r29 + 0x20) = r0;
    r0 = *(u32*)((u8*)r29 + 0x4);
    r31 = r31 + r0;
L_8019200C: ;
    r0 = *(u32*)((u8*)r29 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_8019202C;
    r0 = r30 + r31;
    *(u32*)((u8*)r29 + 0x24) = r0;
    r0 = *(u32*)((u8*)r29 + 0x8);
    r0 = r0 << 2;
    r31 = r31 + r0;
L_8019202C: ;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_8019204C;
    r0 = r30 + r31;
    *(u32*)((u8*)r29 + 0x28) = r0;
    r0 = *(u32*)((u8*)r29 + 0xC);
    r0 = r0 << 3;
    r31 = r31 + r0;
L_8019204C: ;
    r0 = *(u32*)((u8*)r29 + 0x10);
    if ((u32)r0 == (u32)0x0) goto L_8019206C;
    r0 = r30 + r31;
    *(u32*)((u8*)r29 + 0x2C) = r0;
    r0 = *(u32*)((u8*)r29 + 0x10);
    r0 = r0 << 3;
    r31 = r31 + r0;
L_8019206C: ;
    r0 = *(u32*)((u8*)r29 + 0x0);
    if ((u32)r31 >= (u32)r0) goto L_80192080;
    r0 = r30 + r31;
    *(u32*)((u8*)r29 + 0x30) = r0;
L_80192080: ;
    *(u32*)((u8*)r29 + 0x40) = r30;
    r5 = 0x0;
    goto L_801920B4;
L_8019208C: ;
    r3 = *(u32*)((u8*)r29 + 0x24);
    r0 = r5 << 2;
    r4 = *(u32*)((u8*)r29 + 0x20);
    r5 = r5 + 0x1;
    r3 = *(u32*)(r3 + r0);
    r0 = *(u32*)((u8*)r29 + 0x20);
    r4 = r4 + r3;
    r3 = *(u32*)((u8*)r4 + 0x0);
    r0 = r3 + r0;
    *(u32*)((u8*)r4 + 0x0) = r0;
L_801920B4: ;
    r0 = *(u32*)((u8*)r29 + 0x8);
    if ((u32)r5 < (u32)r0) goto L_8019208C;
    r3 = 0x0;
L_801920C4: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801920E4 | 0x1664 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801920E4(void) {
    extern u8 lbl_802744F0[];
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D908[];
    extern u8 lbl_8047D90C[];
    extern u8 lbl_8047D910[];
    extern u8 lbl_8047D918[];
    extern u8 lbl_8047D920[];
    extern u8 lbl_8047D928[];
    extern u8 lbl_8047D930[];
    extern u8 lbl_8047D938[];
    extern u8 lbl_8047D940[];
    extern u8 lbl_8047D944[];
    extern u8 lbl_8047D948[];
    extern void fn_800CD85C();
    extern void fn_800CDBE0();
    extern void fn_800CE148();
    extern void fn_800CE220();
    extern void fn_800CE298();
    extern void fn_800CE2B8();
    extern void fn_800CE2D8();
    extern void fn_800CE2F8();
    extern void fn_800CE318();
    extern void fn_800CE338();
    extern void fn_800CE358();
    extern void fn_80196D78();
    extern void fn_80196E10();
    extern void fn_801A3E64();
    extern void fn_801A3EB4();
    extern void fn_801ADC3C();
    extern void fn_801ADC7C();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
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
    f32 f8 = 0.0f;

    /* stmw r22, 0x28(r1) */;
    /* mr. r28, r3 */;
    r3 = (u32)lbl_802744F0;
    r25 = r4;
    r31 = (u32)lbl_802744F0;
    r24 = r5;
    r23 = 0x0;
    r27 = 0x0;
    if ((s32)r0 != (s32)0) goto L_8019211C;
    f1 = *(f32*)lbl_8047D908;
    goto L_80193734;
L_8019211C: ;
    if ((s32)r27 <= (s32)0x0) goto L_801922BC;
    r0 = *(u8*)((u8*)r28 + 0x0);
    /* subic. r27, r27, 0x1 */;
    r3 = r29 << 8;
    r28 = r28 + 0x1;
    r29 = r3;
    r29 = (r29 & ~0x000000FF) | (((r0 << 0) | ((u32)r0 >> 32)) & 0x000000FF);
    if ((s32)r27 != (s32)0x0) goto L_8019211C;
    r0 = r26 & 0xFF;
    if ((s32)r0 == (s32)0x6) goto L_80192284;
    if ((s32)r0 >= (s32)0x6) goto L_80192174;
    if ((s32)r0 == (s32)0x3) goto L_80192244;
    if ((s32)r0 >= (s32)0x3) goto L_80192168;
    if ((s32)r0 >= (s32)0x2) goto L_8019218C;
    goto L_801922A8;
L_80192168: ;
    if ((s32)r0 >= (s32)0x5) goto L_801921BC;
    goto L_8019227C;
L_80192174: ;
    if ((s32)r0 == (s32)0xff) goto L_80192298;
    if ((s32)r0 >= (s32)0xff) goto L_801922A8;
    if ((s32)r0 == (s32)0x3c) goto L_801921E0;
    goto L_801922A8;
L_8019218C: ;
    if ((u32)r29 < (u32)r24) goto L_801921A4;
    r3 = r31 + 0x0;
    r5 = r31 + 0xc;
    r4 = 0x119;
    fn_80196E10();
L_801921A4: ;
    r0 = r29 << 2;
    r3 = r23;
    r4 = *(u32*)(r25 + r0);
    fn_801A3EB4();
    r23 = r3;
    goto L_8019211C;
L_801921BC: ;
    r22 = 0x0;
    goto L_801921D4;
L_801921C4: ;
    r3 = r23;
    fn_801A3E64();
    r23 = r3;
    r22 = r22 + 0x1;
L_801921D4: ;
    if ((u32)r22 < (u32)r29) goto L_801921C4;
    goto L_8019211C;
L_801921E0: ;
    r3 = r23;
    r22 = 0x0;
    goto L_801921F4;
L_801921EC: ;
    r3 = *(u32*)((u8*)r3 + 0x0);
    r22 = r22 + 0x1;
L_801921F4: ;
    if ((u32)r3 == (u32)0x0) goto L_80192204;
    if ((u32)r22 < (u32)r29) goto L_801921EC;
L_80192204: ;
    if ((u32)r3 != (u32)0x0) goto L_80192230;
    r4 = r29;
    r3 = r31 + 0x20;
    /* crclr cr1eq */;
    OSReport();
    r3 = r31 + 0x0;
    r4 = 0x12b;
    r5 = (u32)lbl_8047D90C;
    fn_80196D78();
    goto L_8019211C;
L_80192230: ;
    r4 = *(u32*)((u8*)r3 + 0x4);
    r3 = r23;
    fn_801A3EB4();
    r23 = r3;
    goto L_8019211C;
L_80192244: ;
    if ((u32)r23 != (u32)0x0) goto L_8019225C;
    r3 = r31 + 0x0;
    r4 = 0x133;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019225C: ;
    r0 = *(u32*)((u8*)r23 + 0x4);
    if ((s32)r0 == (s32)0x0) goto L_8019226C;
    r28 = r28 + r29;
L_8019226C: ;
    r3 = r23;
    fn_801A3E64();
    r23 = r3;
    goto L_8019211C;
L_8019227C: ;
    r28 = r28 + r29;
    goto L_8019211C;
L_80192284: ;
    r3 = r23;
    r4 = r29;
    fn_801A3EB4();
    r23 = r3;
    goto L_8019211C;
L_80192298: ;
    r3 = r31 + 0x0;
    r5 = r31 + 0x48;
    r4 = 0x143;
    fn_80196D78();
L_801922A8: ;
    r3 = r31 + 0x0;
    r5 = r31 + 0x60;
    r4 = 0x146;
    fn_80196D78();
    goto L_8019211C;
L_801922BC: ;
    r26 = *(u8*)((u8*)r28 + 0x0);
    r28 = r28 + 0x1;
    r0 = r26 & 0xFF;
    if ((s32)r0 == (s32)0x20) goto L_80192CE8;
    if ((s32)r0 >= (s32)0x20) goto L_801923AC;
    if ((s32)r0 == (s32)0x10) goto L_801926C4;
    if ((s32)r0 >= (s32)0x10) goto L_80192348;
    if ((s32)r0 == (s32)0x8) goto L_80192510;
    if ((s32)r0 >= (s32)0x8) goto L_8019231C;
    if ((s32)r0 == (s32)0x5) goto L_801924BC;
    if ((s32)r0 >= (s32)0x5) goto L_80192310;
    if ((s32)r0 == (s32)0x1) goto L_8019247C;
    if ((s32)r0 >= (s32)0x1) goto L_801924C8;
    if ((s32)r0 >= (s32)0x0) goto L_8019211C;
    goto L_80193710;
L_80192310: ;
    if ((s32)r0 >= (s32)0x7) goto L_801924E0;
    goto L_801924D4;
L_8019231C: ;
    if ((s32)r0 == (s32)0xc) goto L_801925D8;
    if ((s32)r0 >= (s32)0xc) goto L_80192338;
    if ((s32)r0 == (s32)0xa) goto L_80192588;
    if ((s32)r0 >= (s32)0xa) goto L_801925B0;
    goto L_80192558;
L_80192338: ;
    if ((s32)r0 == (s32)0xe) goto L_80192644;
    if ((s32)r0 >= (s32)0xe) goto L_80192684;
    goto L_80192604;
L_80192348: ;
    if ((s32)r0 == (s32)0x18) goto L_80192A04;
    if ((s32)r0 >= (s32)0x18) goto L_80192380;
    if ((s32)r0 == (s32)0x14) goto L_801927B8;
    if ((s32)r0 >= (s32)0x14) goto L_80192370;
    if ((s32)r0 == (s32)0x12) goto L_80192744;
    if ((s32)r0 >= (s32)0x12) goto L_80192784;
    goto L_80192704;
L_80192370: ;
    if ((s32)r0 == (s32)0x16) goto L_80192858;
    if ((s32)r0 >= (s32)0x16) goto L_801929A0;
    goto L_801927EC;
L_80192380: ;
    if ((s32)r0 == (s32)0x1c) goto L_80192B98;
    if ((s32)r0 >= (s32)0x1c) goto L_8019239C;
    if ((s32)r0 == (s32)0x1a) goto L_80192ACC;
    if ((s32)r0 >= (s32)0x1a) goto L_80192B30;
    goto L_80192A68;
L_8019239C: ;
    if ((s32)r0 == (s32)0x1e) goto L_80192C40;
    if ((s32)r0 >= (s32)0x1e) goto L_80192C94;
    goto L_80192BEC;
L_801923AC: ;
    if ((s32)r0 == (s32)0x30) goto L_801934D4;
    if ((s32)r0 >= (s32)0x30) goto L_8019241C;
    if ((s32)r0 == (s32)0x28) goto L_80192828;
    if ((s32)r0 >= (s32)0x28) goto L_801923F0;
    if ((s32)r0 == (s32)0x24) goto L_80192E74;
    if ((s32)r0 >= (s32)0x24) goto L_801923E0;
    if ((s32)r0 == (s32)0x22) goto L_80192DAC;
    if ((s32)r0 >= (s32)0x22) goto L_80192E10;
    goto L_80192D44;
L_801923E0: ;
    if ((s32)r0 == (s32)0x26) goto L_80192F24;
    if ((s32)r0 >= (s32)0x26) goto L_801936B0;
    goto L_80192ECC;
L_801923F0: ;
    if ((s32)r0 == (s32)0x2c) goto L_80193350;
    if ((s32)r0 >= (s32)0x2c) goto L_8019240C;
    if ((s32)r0 == (s32)0x2a) goto L_8019328C;
    if ((s32)r0 >= (s32)0x2a) goto L_801932F0;
    goto L_80193228;
L_8019240C: ;
    if ((s32)r0 == (s32)0x2e) goto L_8019340C;
    if ((s32)r0 >= (s32)0x2e) goto L_8019346C;
    goto L_801933B0;
L_8019241C: ;
    if ((s32)r0 == (s32)0x38) goto L_801931C0;
    if ((s32)r0 >= (s32)0x38) goto L_80192454;
    if ((s32)r0 == (s32)0x34) goto L_80193028;
    if ((s32)r0 >= (s32)0x34) goto L_80192444;
    if ((s32)r0 == (s32)0x32) goto L_8019353C;
    if ((s32)r0 >= (s32)0x32) goto L_80192FC4;
    goto L_80192974;
L_80192444: ;
    if ((s32)r0 == (s32)0x36) goto L_801930F4;
    if ((s32)r0 >= (s32)0x36) goto L_8019315C;
    goto L_8019308C;
L_80192454: ;
    if ((s32)r0 == (s32)0x3c) goto L_801924BC;
    if ((s32)r0 >= (s32)0x3c) goto L_80192470;
    if ((s32)r0 == (s32)0x3a) goto L_80193608;
    if ((s32)r0 >= (s32)0x3a) goto L_8019365C;
    goto L_801935B4;
L_80192470: ;
    if ((s32)r0 == (s32)0xff) goto L_801924BC;
    goto L_80193710;
L_8019247C: ;
    if ((u32)r23 != (u32)0x0) goto L_80192494;
    r3 = r31 + 0x0;
    r4 = 0x153;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192494: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    *(f32*)(sp + 0xC) = f0;
    goto L_801924AC;
L_801924A0: ;
    r3 = r23;
    fn_801A3E64();
    r23 = r3;
L_801924AC: ;
    if ((u32)r23 != (u32)0x0) goto L_801924A0;
    f1 = *(f32*)(sp + 0xC);
    goto L_80193734;
L_801924BC: ;
    r27 = 0x1;
    r29 = 0x0;
    goto L_8019211C;
L_801924C8: ;
    r27 = 0x2;
    r29 = 0x0;
    goto L_8019211C;
L_801924D4: ;
    r27 = 0x4;
    r29 = 0x0;
    goto L_8019211C;
L_801924E0: ;
    if ((u32)r23 != (u32)0x0) goto L_801924F8;
    r3 = r31 + 0x0;
    r4 = 0x178;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801924F8: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = (f64)(s32)f1;
    *(f64*)(sp + 0x18) = f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192510: ;
    if ((u32)r23 != (u32)0x0) goto L_80192528;
    r3 = r31 + 0x0;
    r4 = 0x17d;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192528: ;
    r22 = *(u32*)((u8*)r23 + 0x4);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x18) = r0;
    /* xoris r0, r22, 0x8000 */;
    f1 = *(f64*)lbl_8047D948;
    *(u32*)(sp + 0x1C) = r0;
    f0 = *(f64*)(sp + 0x18);
    f0 = f0 - f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192558: ;
    if ((u32)r23 != (u32)0x0) goto L_80192570;
    r3 = r31 + 0x0;
    r4 = 0x183;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192570: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = -f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192588: ;
    if ((u32)r23 != (u32)0x0) goto L_801925A0;
    r3 = r31 + 0x0;
    r4 = 0x189;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801925A0: ;
    r22 = *(u32*)((u8*)r23 + 0x4);
    r0 = -r22;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801925B0: ;
    if ((u32)r23 != (u32)0x0) goto L_801925C8;
    r3 = r31 + 0x0;
    r4 = 0x18f;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801925C8: ;
    r3 = 0x2;
    fn_801ADC3C();
    *(u32*)((u8*)r23 + 0x4) = r3;
    goto L_8019211C;
L_801925D8: ;
    if ((u32)r23 != (u32)0x0) goto L_801925F0;
    r3 = r31 + 0x0;
    r4 = 0x195;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801925F0: ;
    fn_801ADC7C();
    *(f32*)(sp + 0x10) = f1;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192604: ;
    if ((u32)r23 != (u32)0x0) goto L_8019261C;
    r3 = r31 + 0x0;
    r4 = 0x19b;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019261C: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f64*)lbl_8047D918;
    f1 = f0 * f1;
    f1 = (f32)f1;
    fn_800CE148();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192644: ;
    if ((u32)r23 != (u32)0x0) goto L_8019265C;
    r3 = r31 + 0x0;
    r4 = 0x1a1;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019265C: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f64*)lbl_8047D918;
    f1 = f0 * f1;
    f1 = (f32)f1;
    fn_800CDBE0();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192684: ;
    if ((u32)r23 != (u32)0x0) goto L_8019269C;
    r3 = r31 + 0x0;
    r4 = 0x1a7;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019269C: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f64*)lbl_8047D918;
    f1 = f0 * f1;
    f1 = (f32)f1;
    fn_800CE220();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801926C4: ;
    if ((u32)r23 != (u32)0x0) goto L_801926DC;
    r3 = r31 + 0x0;
    r4 = 0x1ad;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801926DC: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE2B8();
    f1 = (f32)f1;
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192704: ;
    if ((u32)r23 != (u32)0x0) goto L_8019271C;
    r3 = r31 + 0x0;
    r4 = 0x1b3;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019271C: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE298();
    f1 = (f32)f1;
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192744: ;
    if ((u32)r23 != (u32)0x0) goto L_8019275C;
    r3 = r31 + 0x0;
    r4 = 0x1b9;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019275C: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CD85C();
    f1 = (f32)f1;
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192784: ;
    if ((u32)r23 != (u32)0x0) goto L_8019279C;
    r3 = r31 + 0x0;
    r4 = 0x1bf;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019279C: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE338();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801927B8: ;
    if ((u32)r23 != (u32)0x0) goto L_801927D0;
    r3 = r31 + 0x0;
    r4 = 0x1c5;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801927D0: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE2F8();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801927EC: ;
    if ((u32)r23 != (u32)0x0) goto L_80192804;
    r3 = r31 + 0x0;
    r4 = 0x1cb;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192804: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f32*)lbl_8047D908;
    if (f1 >= f0) goto L_8019211C;
    f0 = -f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192828: ;
    if ((u32)r23 != (u32)0x0) goto L_80192840;
    r3 = r31 + 0x0;
    r4 = 0x1d3;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192840: ;
    r22 = *(u32*)((u8*)r23 + 0x4);
    if ((s32)r22 >= (s32)0x0) goto L_8019211C;
    r0 = -r22;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192858: ;
    if ((u32)r23 != (u32)0x0) goto L_80192870;
    r3 = r31 + 0x0;
    r4 = 0x1da;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192870: ;
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f32*)lbl_8047D908;
    if (f1 <= f0) goto L_801928D8;
    /* frsqrte f8, f1 */;
    f7 = *(f64*)lbl_8047D928;
    f4 = *(f64*)lbl_8047D930;
    f5 = *(f64*)lbl_8047D928;
    f6 = f8 * f8;
    f2 = *(f64*)lbl_8047D930;
    f3 = *(f64*)lbl_8047D928;
    f7 = f7 * f8;
    f0 = *(f64*)lbl_8047D930;
    f4 = -(f1 * f6 - f4);
    f8 = f7 * f4;
    f4 = f8 * f8;
    f5 = f5 * f8;
    f2 = -(f1 * f4 - f2);
    f8 = f5 * f2;
    f2 = f8 * f8;
    f3 = f3 * f8;
    f0 = -(f1 * f2 - f0);
    f8 = f3 * f0;
    f1 = f1 * f8;
    f1 = (f32)f1;
    goto L_80192964;
L_801928D8: ;
    f0 = *(f64*)lbl_8047D938;
    if (f1 >= f0) goto L_801928F0;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_80192964;
L_801928F0: ;
    *(f32*)(sp + 0x8) = f1;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0x8);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_80192918;
    if ((s32)r3 >= (s32)r0) goto L_80192950;
    if ((s32)r3 == (s32)0x0) goto L_80192934;
    goto L_80192950;
L_80192918: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019292C;
    r0 = 0x1;
    goto L_80192954;
L_8019292C: ;
    r0 = 0x2;
    goto L_80192954;
L_80192934: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_80192948;
    r0 = 0x5;
    goto L_80192954;
L_80192948: ;
    r0 = 0x3;
    goto L_80192954;
L_80192950: ;
    r0 = 0x4;
L_80192954: ;
    if ((s32)r0 != (s32)0x1) goto L_80192964;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
L_80192964: ;
    *(f32*)(sp + 0x10) = f1;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192974: ;
    if ((u32)r23 != (u32)0x0) goto L_8019298C;
    r3 = r31 + 0x0;
    r4 = 0x1e0;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019298C: ;
    r22 = *(u32*)((u8*)r23 + 0x4);
    r0 = __cntlzw(r22);
    r0 = (u32)r0 >> 5;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801929A0: ;
    if ((u32)r23 != (u32)0x0) goto L_801929B8;
    r3 = r31 + 0x0;
    r4 = 0x1f5;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801929B8: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801929D4;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x1f5;
    fn_80196E10();
L_801929D4: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 + f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192A04: ;
    if ((u32)r23 != (u32)0x0) goto L_80192A1C;
    r3 = r31 + 0x0;
    r4 = 0x1fb;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192A1C: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192A38;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x1fb;
    fn_80196E10();
L_80192A38: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 - f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192A68: ;
    if ((u32)r23 != (u32)0x0) goto L_80192A80;
    r3 = r31 + 0x0;
    r4 = 0x201;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192A80: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192A9C;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x201;
    fn_80196E10();
L_80192A9C: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 * f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192ACC: ;
    if ((u32)r23 != (u32)0x0) goto L_80192AE4;
    r3 = r31 + 0x0;
    r4 = 0x207;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192AE4: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192B00;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x207;
    fn_80196E10();
L_80192B00: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 / f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192B30: ;
    if ((u32)r23 != (u32)0x0) goto L_80192B48;
    r3 = r31 + 0x0;
    r4 = 0x20d;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192B48: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192B64;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x20d;
    fn_80196E10();
L_80192B64: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f2 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    fn_800CE318();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192B98: ;
    if ((u32)r23 != (u32)0x0) goto L_80192BB0;
    r3 = r31 + 0x0;
    r4 = 0x213;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192BB0: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192BCC;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x213;
    fn_80196E10();
L_80192BCC: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r22 + r30;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192BEC: ;
    if ((u32)r23 != (u32)0x0) goto L_80192C04;
    r3 = r31 + 0x0;
    r4 = 0x218;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192C04: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192C20;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x218;
    fn_80196E10();
L_80192C20: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r22 - r30;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192C40: ;
    if ((u32)r23 != (u32)0x0) goto L_80192C58;
    r3 = r31 + 0x0;
    r4 = 0x21d;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192C58: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192C74;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x21d;
    fn_80196E10();
L_80192C74: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r22 * r30;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192C94: ;
    if ((u32)r23 != (u32)0x0) goto L_80192CAC;
    r3 = r31 + 0x0;
    r4 = 0x222;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192CAC: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192CC8;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x222;
    fn_80196E10();
L_80192CC8: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = (s32)r22 / (s32)r30;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192CE8: ;
    if ((u32)r23 != (u32)0x0) goto L_80192D00;
    r3 = r31 + 0x0;
    r4 = 0x227;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192D00: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192D1C;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x227;
    fn_80196E10();
L_80192D1C: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = (s32)r22 / (s32)r30;
    r0 = r0 * r30;
    r0 = r22 - r0;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80192D44: ;
    if ((u32)r23 != (u32)0x0) goto L_80192D5C;
    r3 = r31 + 0x0;
    r4 = 0x22c;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192D5C: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192D78;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x22c;
    fn_80196E10();
L_80192D78: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f2 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    fn_800CE358();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192DAC: ;
    if ((u32)r23 != (u32)0x0) goto L_80192DC4;
    r3 = r31 + 0x0;
    r4 = 0x232;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192DC4: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192DE0;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x232;
    fn_80196E10();
L_80192DE0: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    if (f1 <= f0) goto L_8019211C;
    r0 = *(u32*)(sp + 0xC);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192E10: ;
    if ((u32)r23 != (u32)0x0) goto L_80192E28;
    r3 = r31 + 0x0;
    r4 = 0x239;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192E28: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192E44;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x239;
    fn_80196E10();
L_80192E44: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    if (f1 >= f0) goto L_8019211C;
    r0 = *(u32*)(sp + 0xC);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192E74: ;
    if ((u32)r23 != (u32)0x0) goto L_80192E8C;
    r3 = r31 + 0x0;
    r4 = 0x240;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192E8C: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192EA8;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x240;
    fn_80196E10();
L_80192EA8: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    if ((s32)r22 <= (s32)r30) goto L_8019211C;
    *(u32*)((u8*)r23 + 0x4) = r30;
    goto L_8019211C;
L_80192ECC: ;
    if ((u32)r23 != (u32)0x0) goto L_80192EE4;
    r3 = r31 + 0x0;
    r4 = 0x247;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192EE4: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192F00;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x247;
    fn_80196E10();
L_80192F00: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    if ((s32)r22 >= (s32)r30) goto L_8019211C;
    *(u32*)((u8*)r23 + 0x4) = r30;
    goto L_8019211C;
L_80192F24: ;
    if ((u32)r23 != (u32)0x0) goto L_80192F3C;
    r3 = r31 + 0x0;
    r4 = 0x24e;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192F3C: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192F58;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x24e;
    fn_80196E10();
L_80192F58: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f2 = *(f32*)(sp + 0xC);
    r23 = r3;
    f0 = *(f32*)lbl_8047D908;
    f1 = *(f32*)((u8*)r3 + 0x4);
    if (f0 != f2) goto L_80192FA0;
    f0 = *(f32*)lbl_8047D908;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_80192F98;
    f1 = *(f32*)lbl_8047D940;
    goto L_80192FA8;
L_80192F98: ;
    f1 = *(f32*)lbl_8047D944;
    goto L_80192FA8;
L_80192FA0: ;
    fn_800CE2D8();
    f1 = (f32)f1;
L_80192FA8: ;
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80192FC4: ;
    if ((u32)r23 != (u32)0x0) goto L_80192FDC;
    r3 = r31 + 0x0;
    r4 = 0x254;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80192FDC: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80192FF8;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x254;
    fn_80196E10();
L_80192FF8: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    r0 = 0; /* mfcr */;
    r0 = (u32)r0 >> 31;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80193028: ;
    if ((u32)r23 != (u32)0x0) goto L_80193040;
    r3 = r31 + 0x0;
    r4 = 0x259;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193040: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_8019305C;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x259;
    fn_80196E10();
L_8019305C: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    r0 = 0; /* mfcr */;
    /* extrwi r0, r0, 1, 1 */;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_8019308C: ;
    if ((u32)r23 != (u32)0x0) goto L_801930A4;
    r3 = r31 + 0x0;
    r4 = 0x25e;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801930A4: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801930C0;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x25e;
    fn_80196E10();
L_801930C0: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    /* cror eq, lt, eq */;
    r0 = 0; /* mfcr */;
    /* extrwi r0, r0, 1, 2 */;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_801930F4: ;
    if ((u32)r23 != (u32)0x0) goto L_8019310C;
    r3 = r31 + 0x0;
    r4 = 0x263;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_8019310C: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193128;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x263;
    fn_80196E10();
L_80193128: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    /* cror eq, gt, eq */;
    r0 = 0; /* mfcr */;
    /* extrwi r0, r0, 1, 2 */;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_8019315C: ;
    if ((u32)r23 != (u32)0x0) goto L_80193174;
    r3 = r31 + 0x0;
    r4 = 0x268;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193174: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193190;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x268;
    fn_80196E10();
L_80193190: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    r0 = 0; /* mfcr */;
    /* extrwi r0, r0, 1, 2 */;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_801931C0: ;
    if ((u32)r23 != (u32)0x0) goto L_801931D8;
    r3 = r31 + 0x0;
    r4 = 0x26d;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801931D8: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801931F4;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x26d;
    fn_80196E10();
L_801931F4: ;
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    r0 = 0; /* mfcr */;
    /* extrwi r0, r0, 1, 2 */;
    r0 = r0 ^ 0x1;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80193228: ;
    if ((u32)r23 != (u32)0x0) goto L_80193240;
    r3 = r31 + 0x0;
    r4 = 0x272;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193240: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_8019325C;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x272;
    fn_80196E10();
L_8019325C: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r30 ^ r22;
    r3 = (s32)r0 >> 1;
    r0 = r0 & r30;
    r0 = r3 - r0;
    r0 = (u32)r0 >> 31;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_8019328C: ;
    if ((u32)r23 != (u32)0x0) goto L_801932A4;
    r3 = r31 + 0x0;
    r4 = 0x277;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801932A4: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801932C0;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x277;
    fn_80196E10();
L_801932C0: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r22 ^ r30;
    r3 = (s32)r0 >> 1;
    r0 = r0 & r22;
    r0 = r3 - r0;
    r0 = (u32)r0 >> 31;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801932F0: ;
    if ((u32)r23 != (u32)0x0) goto L_80193308;
    r3 = r31 + 0x0;
    r4 = 0x27c;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193308: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193324;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x27c;
    fn_80196E10();
L_80193324: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r4 = (s32)r30 >> 31;
    r23 = r3;
    r3 = (u32)r22 >> 31;
    r0 = r30 - r22;
    r0 = r4 + r3; /* +carry */;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80193350: ;
    if ((u32)r23 != (u32)0x0) goto L_80193368;
    r3 = r31 + 0x0;
    r4 = 0x281;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193368: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193384;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x281;
    fn_80196E10();
L_80193384: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r4 = (u32)r30 >> 31;
    r3 = (s32)r22 >> 31;
    r0 = r22 - r30;
    r0 = r3 + r4; /* +carry */;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801933B0: ;
    if ((u32)r23 != (u32)0x0) goto L_801933C8;
    r3 = r31 + 0x0;
    r4 = 0x286;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801933C8: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801933E4;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x286;
    fn_80196E10();
L_801933E4: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r30 - r22;
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_8019340C: ;
    if ((u32)r23 != (u32)0x0) goto L_80193424;
    r3 = r31 + 0x0;
    r4 = 0x28b;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193424: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193440;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x28b;
    fn_80196E10();
L_80193440: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r3 = r30 - r22;
    r0 = r22 - r30;
    r0 = r3 | r0;
    r0 = (u32)r0 >> 31;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_8019346C: ;
    if ((u32)r23 != (u32)0x0) goto L_80193484;
    r3 = r31 + 0x0;
    r4 = 0x290;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193484: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801934A0;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x290;
    fn_80196E10();
L_801934A0: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r0 = 0x0;
    r23 = r3;
    if ((s32)r22 == (s32)0x0) goto L_801934CC;
    if ((s32)r30 == (s32)0x0) goto L_801934CC;
    r0 = 0x1;
L_801934CC: ;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801934D4: ;
    if ((u32)r23 != (u32)0x0) goto L_801934EC;
    r3 = r31 + 0x0;
    r4 = 0x295;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801934EC: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193508;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x295;
    fn_80196E10();
L_80193508: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r0 = 0x0;
    r23 = r3;
    if ((s32)r22 != (s32)0x0) goto L_80193530;
    if ((s32)r30 == (s32)0x0) goto L_80193534;
L_80193530: ;
    r0 = 0x1;
L_80193534: ;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_8019353C: ;
    if ((u32)r23 != (u32)0x0) goto L_80193554;
    r3 = r31 + 0x0;
    r4 = 0x29a;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193554: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193570;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x29a;
    fn_80196E10();
L_80193570: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r0 = 0x0;
    r23 = r3;
    if ((s32)r22 != (s32)0x0) goto L_80193598;
    if ((s32)r30 != (s32)0x0) goto L_801935A8;
L_80193598: ;
    if ((s32)r22 == (s32)0x0) goto L_801935AC;
    if ((s32)r30 != (s32)0x0) goto L_801935AC;
L_801935A8: ;
    r0 = 0x1;
L_801935AC: ;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_801935B4: ;
    if ((u32)r23 != (u32)0x0) goto L_801935CC;
    r3 = r31 + 0x0;
    r4 = 0x29f;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801935CC: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801935E8;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x29f;
    fn_80196E10();
L_801935E8: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r22 & r30;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_80193608: ;
    if ((u32)r23 != (u32)0x0) goto L_80193620;
    r3 = r31 + 0x0;
    r4 = 0x2a4;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193620: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_8019363C;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x2a4;
    fn_80196E10();
L_8019363C: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r22 | r30;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_8019365C: ;
    if ((u32)r23 != (u32)0x0) goto L_80193674;
    r3 = r31 + 0x0;
    r4 = 0x2a9;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_80193674: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80193690;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x2a9;
    fn_80196E10();
L_80193690: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r0 = r22 ^ r30;
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_8019211C;
L_801936B0: ;
    if ((u32)r23 != (u32)0x0) goto L_801936C8;
    r3 = r31 + 0x0;
    r4 = 0x2af;
    r5 = (u32)lbl_8047D910;
    fn_80196E10();
L_801936C8: ;
    r0 = *(u32*)((u8*)r23 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801936E4;
    r3 = r31 + 0x0;
    r5 = r31 + 0x78;
    r4 = 0x2af;
    fn_80196E10();
L_801936E4: ;
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r3 = r30 - r22;
    r3 = r3 + 0x1;
    fn_801ADC3C();
    r0 = r22 + r3;
    *(u32*)((u8*)r23 + 0x4) = r0;
    goto L_8019211C;
L_80193710: ;
    r3 = r31 + 0x84;
    r4 = r26 & 0xFF;
    /* crclr cr1eq */;
    OSReport();
    r3 = r31 + 0x0;
    r4 = 0x2b5;
    r5 = (u32)lbl_8047D90C;
    fn_80196D78();
    goto L_8019211C;
L_80193734: ;
    /* lmw r22, 0x28(r1) */;
    return;
}
#pragma pop
