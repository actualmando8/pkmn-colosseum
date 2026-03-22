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
    u32 tmp = 0;
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
    tmp = (u32)fn_80191A34;
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
    *(u32*)((u8*)r3 + 0x40) = tmp;
    return;
}

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
void fn_80191688(void) {
    extern u8 lbl_8047D8D8[];
    extern u8 lbl_8047D8E0[];
    extern void fn_800A37CC();
    extern void fn_80196E10();
    extern void fn_8019D9DC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) return;
    if (r31 == 0) {
        return;
    }
    tmp = *(u32*)((u8*)r30 + 0x8);
    tmp = tmp & 0x1;
    if (r31 != 0) {
        tmp = *(u32*)((u8*)r30 + 0x18);
        if (tmp == 0) goto L_80191748;
        r3 = *(u32*)((u8*)r30 + 0x18);
        tmp = *(u32*)((u8*)r3 + 0x18);
        if (tmp == 0) goto L_80191748;
        r3 = *(u32*)((u8*)r30 + 0x18);
        r29 = *(u32*)((u8*)r3 + 0x18);
        if (r29 != 0) {
            if (r29 == 0) {
                r3 = (u32)lbl_8047D8D8;
                r4 = 0x25d;
                r5 = (u32)lbl_8047D8E0;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r29 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r29 == 0) {
                tmp = *(u32*)((u8*)r29 + 0x14);
                tmp = tmp & 0x00000040;
                if (r29 != 0) {
                    r3 = 0x1;
            }
            }
            if ((s32)r3 != 0) {
                r3 = r29;
                fn_8019D9DC();
        }
        }
        r3 = r29 + 0x44;
        r4 = r30 + 0xc;
        r5 = r30 + 0xc;
        fn_800A37CC();
    L_80191748:
        tmp = *(u32*)((u8*)r30 + 0x8);
        /* clrrwi tmp, tmp, 1 */;
        *(u32*)((u8*)r30 + 0x8) = tmp;
    }
    r3 = *(u32*)((u8*)r30 + 0xC);
    tmp = *(u32*)((u8*)r30 + 0x10);
    *(u32*)((u8*)r31 + 0x0) = r3;
    *(u32*)((u8*)r31 + 0x4) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x14);
    *(u32*)((u8*)r31 + 0x8) = tmp;

    return;
}

/* 0x48 | fn_80191788 | generic */
void fn_80191788(u32 arg1, u32 arg2) {

}

/* 0x801917D0 | 0xCC */
void fn_801917D0(void) {
    extern u8 lbl_8036C5F0[];
    extern u8 lbl_8047B218[];
    extern u8 lbl_8047D8C8[];
    extern u8 lbl_8047D8D0[];
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
    void (*ctr_fn)(void) = 0;

    /* mr. r30, r3 */;
    if ((s32)tmp == 0) { r3 = 0x0; return; }
    tmp = *(u32*)((u8*)r30 + 0x0);
    if (tmp == 0) goto L_80191808;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_80193748();
    if (r3 != 0) goto L_80191844;
L_80191808:
    tmp = *(u32*)lbl_8047B218;
    if (tmp != 0) {
        r3 = *(u32*)lbl_8047B218;
    } else {

        r3 = (u32)lbl_8036C5F0;
        r3 = (u32)lbl_8036C5F0;
    }
    fn_80193828();
    /* mr. r31, r3 */;
    if (tmp != 0) goto L_80191860;
    r3 = (u32)lbl_8047D8C8;
    r4 = 0x257;
    r5 = (u32)lbl_8047D8D0;
    fn_80196E10();
    goto L_80191860;
L_80191844:
    fn_80193828();
    /* mr. r31, r3 */;
    if (tmp != 0) goto L_80191860;
    r3 = (u32)lbl_8047D8C8;
    r4 = 0x104;
    r5 = (u32)lbl_8047D8D0;
    fn_80196E10();
L_80191860:
    r5 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r4 = r30;
    r12 = *(u32*)((u8*)r5 + 0x3C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r3 = r31;
    return;

    r3 = 0x0;

    return;
}

/* 0x8019189C | 0xB0 */
void fn_8019189C(void) {
    extern void fn_801AE50C();
    extern void fn_801AE5E8();
    extern void fn_801AEBE4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) return;
    if (r31 == 0) {
        return;
    }
    if (r30 != 0) {
        /* addic. tmp, r31, 0x4 */;
        if (r30 != 0) {
            r3 = *(u32*)((u8*)r31 + 0x4);
            tmp = *(u32*)((u8*)r31 + 0x8);
            *(u32*)((u8*)r30 + 0xC) = r3;
            *(u32*)((u8*)r30 + 0x10) = tmp;
            tmp = *(u32*)((u8*)r31 + 0xC);
            *(u32*)((u8*)r30 + 0x14) = tmp;
            tmp = *(u32*)((u8*)r30 + 0x8);
            tmp = tmp | 0x2;
            *(u32*)((u8*)r30 + 0x8) = tmp;
            tmp = *(u32*)((u8*)r30 + 0x8);
            /* clrrwi tmp, tmp, 1 */;
            *(u32*)((u8*)r30 + 0x8) = tmp;
    }
    }
    tmp = *(u32*)((u8*)r30 + 0x1C);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r30 + 0x1C);
        fn_801AE50C();
    }
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801AE5E8();
    *(u32*)((u8*)r30 + 0x1C) = r3;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = *(u32*)((u8*)r31 + 0x10);
    fn_801AEBE4();

    return;
}

/* 0x8019194C | 0xA0 */
void fn_8019194C(void) {
    extern void fn_801AE50C();
    extern void fn_801AE5E8();
    extern void fn_801AEBE4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp != 0) {
        /* addic. tmp, r31, 0x4 */;
        if ((s32)tmp != 0) {
            r3 = *(u32*)((u8*)r31 + 0x4);
            tmp = *(u32*)((u8*)r31 + 0x8);
            *(u32*)((u8*)r30 + 0xC) = r3;
            *(u32*)((u8*)r30 + 0x10) = tmp;
            tmp = *(u32*)((u8*)r31 + 0xC);
            *(u32*)((u8*)r30 + 0x14) = tmp;
            tmp = *(u32*)((u8*)r30 + 0x8);
            tmp = tmp | 0x2;
            *(u32*)((u8*)r30 + 0x8) = tmp;
            tmp = *(u32*)((u8*)r30 + 0x8);
            /* clrrwi tmp, tmp, 1 */;
            *(u32*)((u8*)r30 + 0x8) = tmp;
    }
    }
    tmp = *(u32*)((u8*)r30 + 0x1C);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r30 + 0x1C);
        fn_801AE50C();
    }
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801AE5E8();
    *(u32*)((u8*)r30 + 0x1C) = r3;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = *(u32*)((u8*)r31 + 0x10);
    fn_801AEBE4();
    r3 = 0x0;
    return;
}

/* 0x48 | fn_801919EC | generic */
void fn_801919EC(void) {
    fn_801C27F4();
    fn_801B0040();
}

/* 0x80191A34 | 0x398 */
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
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    /* mr. r31, r3 */;
    r29 = r5;
    if ((s32)tmp == 0) return;
    if ((s32)r4 == 6) goto L_80191C34;
    if ((s32)r4 >= 6) goto L_80191A7C;
    if ((s32)r4 == 4) goto L_80191A88;
    if ((s32)r4 >= 4) goto L_80191B78;
    return;
L_80191A7C:
    if ((s32)r4 >= 8) return;
    goto L_80191CF0;
L_80191A88:
    f1 = *(f32*)((u8*)r29 + 0x0);
    f0 = *(f64*)lbl_8047D8E8;
    if (f1 < f0) {
        f0 = *(f32*)lbl_8047D8F0;
        *(f32*)((u8*)r29 + 0x0) = f0;
    }
    f1 = *(f64*)lbl_8047D8F8;
    f0 = *(f32*)((u8*)r29 + 0x0);
    if (f1 < f0) {
        f0 = *(f32*)lbl_8047D900;
        *(f32*)((u8*)r29 + 0x0) = f0;
    }
    tmp = *(u32*)((u8*)r31 + 0x18);
    if (tmp == 0) {
        r4 = (u32)lbl_8027448C;
        r3 = (u32)lbl_8047D8C8;
        r5 = (u32)lbl_8027448C;
        r4 = 0x98;
        fn_80196E10();
    }
    r3 = *(u32*)((u8*)r31 + 0x18);
    r30 = *(u32*)((u8*)r3 + 0x18);
    if (r30 == 0) {
        r3 = (u32)lbl_8047D8C8;
        r4 = 0x9a;
        r5 = (u32)lbl_8047D904;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r30 + 0x18);
    if (tmp == 0) {
        r4 = (u32)lbl_80274498;
        r3 = (u32)lbl_8047D8C8;
        r5 = (u32)lbl_80274498;
        r4 = 0x9b;
        fn_80196E10();
    }
    r4 = *(u32*)((u8*)r30 + 0x18);
    r3 = (u32)sp + 0x8;
    f1 = *(f32*)((u8*)r29 + 0x0);
    fn_801B1890();
    if (r31 != 0) {
        /* addic. tmp, (u32)sp, 0x8 */;
        if (r31 != 0) {
            *(u32*)((u8*)r31 + 0xC) = r3;
            *(u32*)((u8*)r31 + 0x10) = tmp;
            *(u32*)((u8*)r31 + 0x14) = tmp;
            tmp = *(u32*)((u8*)r31 + 0x8);
            tmp = tmp | 0x2;
            *(u32*)((u8*)r31 + 0x8) = tmp;
            tmp = *(u32*)((u8*)r31 + 0x8);
            /* clrrwi tmp, tmp, 1 */;
            *(u32*)((u8*)r31 + 0x8) = tmp;
    }
    }
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp | 0x1;
    *(u32*)((u8*)r31 + 0x8) = tmp;
    return;
L_80191B78:
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r31 == 0) return;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp & 0x1;
    if (r31 != 0) {
        tmp = *(u32*)((u8*)r31 + 0x18);
        if (tmp == 0) goto L_80191C14;
        r3 = *(u32*)((u8*)r31 + 0x18);
        tmp = *(u32*)((u8*)r3 + 0x18);
        if (tmp == 0) goto L_80191C14;
        r3 = *(u32*)((u8*)r31 + 0x18);
        r30 = *(u32*)((u8*)r3 + 0x18);
        if (r30 != 0) {
            if (r30 == 0) {
                r3 = (u32)lbl_8047D8D8;
                r4 = 0x25d;
                r5 = (u32)lbl_8047D8E0;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r30 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r30 == 0) {
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00000040;
                if (r30 != 0) {
                    r3 = 0x1;
            }
            }
            if ((s32)r3 != 0) {
                r3 = r30;
                fn_8019D9DC();
        }
        }
        r3 = r30 + 0x44;
        r4 = r31 + 0xc;
        r5 = r31 + 0xc;
        fn_800A37CC();
    L_80191C14:
        tmp = *(u32*)((u8*)r31 + 0x8);
        /* clrrwi tmp, tmp, 1 */;
        *(u32*)((u8*)r31 + 0x8) = tmp;
    }
    *(f32*)((u8*)r31 + 0xC) = f31;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp | 0x2;
    *(u32*)((u8*)r31 + 0x8) = tmp;
    return;
L_80191C34:
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r31 == 0) return;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp & 0x1;
    if (r31 != 0) {
        tmp = *(u32*)((u8*)r31 + 0x18);
        if (tmp == 0) goto L_80191CD0;
        r3 = *(u32*)((u8*)r31 + 0x18);
        tmp = *(u32*)((u8*)r3 + 0x18);
        if (tmp == 0) goto L_80191CD0;
        r3 = *(u32*)((u8*)r31 + 0x18);
        r30 = *(u32*)((u8*)r3 + 0x18);
        if (r30 != 0) {
            if (r30 == 0) {
                r3 = (u32)lbl_8047D8D8;
                r4 = 0x25d;
                r5 = (u32)lbl_8047D8E0;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r30 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r30 == 0) {
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00000040;
                if (r30 != 0) {
                    r3 = 0x1;
            }
            }
            if ((s32)r3 != 0) {
                r3 = r30;
                fn_8019D9DC();
        }
        }
        r3 = r30 + 0x44;
        r4 = r31 + 0xc;
        r5 = r31 + 0xc;
        fn_800A37CC();
    L_80191CD0:
        tmp = *(u32*)((u8*)r31 + 0x8);
        /* clrrwi tmp, tmp, 1 */;
        *(u32*)((u8*)r31 + 0x8) = tmp;
    }
    *(f32*)((u8*)r31 + 0x10) = f31;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp | 0x2;
    *(u32*)((u8*)r31 + 0x8) = tmp;
    return;
L_80191CF0:
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r31 == 0) return;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp & 0x1;
    if (r31 != 0) {
        tmp = *(u32*)((u8*)r31 + 0x18);
        if (tmp == 0) goto L_80191D8C;
        r3 = *(u32*)((u8*)r31 + 0x18);
        tmp = *(u32*)((u8*)r3 + 0x18);
        if (tmp == 0) goto L_80191D8C;
        r3 = *(u32*)((u8*)r31 + 0x18);
        r30 = *(u32*)((u8*)r3 + 0x18);
        if (r30 != 0) {
            if (r30 == 0) {
                r3 = (u32)lbl_8047D8D8;
                r4 = 0x25d;
                r5 = (u32)lbl_8047D8E0;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r30 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r30 == 0) {
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00000040;
                if (r30 != 0) {
                    r3 = 0x1;
            }
            }
            if ((s32)r3 != 0) {
                r3 = r30;
                fn_8019D9DC();
        }
        }
        r3 = r30 + 0x44;
        r4 = r31 + 0xc;
        r5 = r31 + 0xc;
        fn_800A37CC();
    L_80191D8C:
        tmp = *(u32*)((u8*)r31 + 0x8);
        /* clrrwi tmp, tmp, 1 */;
        *(u32*)((u8*)r31 + 0x8) = tmp;
    }
    *(f32*)((u8*)r31 + 0x14) = f31;
    tmp = *(u32*)((u8*)r31 + 0x8);
    tmp = tmp | 0x2;
    *(u32*)((u8*)r31 + 0x8) = tmp;

    return;
}

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
void fn_80191ECC(void) {
    extern void fn_800CA7FC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = 0x0;
    r30 = r4;
    r29 = r3;
    while (1) {
        tmp = *(u32*)((u8*)r29 + 0xC);
        if (r31 >= tmp) break;
        r3 = *(u32*)((u8*)r29 + 0x28);
        tmp = r31 << 3;
        r5 = *(u32*)((u8*)r29 + 0x30);
        r4 = r30;
        r3 = r3 + tmp;
        tmp = *(u32*)((u8*)r3 + 0x4);
        r3 = r5 + tmp;
        fn_800CA7FC();
        if ((s32)r3 == 0) {
            r3 = *(u32*)((u8*)r29 + 0x28);
            tmp = r31 << 3;
            r4 = *(u32*)((u8*)r29 + 0x20);
            tmp = *(u32*)(r3 + tmp);
            r3 = r4 + tmp;
            return;
        }
        r31 = r31 + 0x1;


    }
    r3 = 0x0;

    return;
}

/* 0x80191F64 | 0x180 */
void fn_80191F64(void) {
    extern u8 lbl_802744A8[];
    u8 sp[0x20];
    u32 tmp = 0;
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
    if ((s32)tmp == 0) {
        r3 = -0x1;
        return;
    }
    r3 = r29;
    r4 = 0x0;
    r5 = 0x44;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = *(u32*)((u8*)r29 + 0x3C);
    r3 = r29;
    r4 = r30;
    r5 = 0x20;
    tmp = tmp | 0x1;
    *(u32*)((u8*)r29 + 0x3C) = tmp;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    tmp = *(u32*)((u8*)r29 + 0x0);
    if (tmp != r28) {
        r3 = (u32)lbl_802744A8;
        r3 = (u32)lbl_802744A8;
        OSReport();
        r3 = -0x1;
        return;
    }
    tmp = *(u32*)((u8*)r29 + 0x4);
    r31 = r31 + 0x20;
    if (tmp != 0) {
        tmp = r30 + r31;
        *(u32*)((u8*)r29 + 0x20) = tmp;
        tmp = *(u32*)((u8*)r29 + 0x4);
        r31 = r31 + tmp;
    }
    tmp = *(u32*)((u8*)r29 + 0x8);
    if (tmp != 0) {
        tmp = r30 + r31;
        *(u32*)((u8*)r29 + 0x24) = tmp;
        tmp = *(u32*)((u8*)r29 + 0x8);
        tmp = tmp << 2;
        r31 = r31 + tmp;
    }
    tmp = *(u32*)((u8*)r29 + 0xC);
    if (tmp != 0) {
        tmp = r30 + r31;
        *(u32*)((u8*)r29 + 0x28) = tmp;
        tmp = *(u32*)((u8*)r29 + 0xC);
        tmp = tmp << 3;
        r31 = r31 + tmp;
    }
    tmp = *(u32*)((u8*)r29 + 0x10);
    if (tmp != 0) {
        tmp = r30 + r31;
        *(u32*)((u8*)r29 + 0x2C) = tmp;
        tmp = *(u32*)((u8*)r29 + 0x10);
        tmp = tmp << 3;
        r31 = r31 + tmp;
    }
    tmp = *(u32*)((u8*)r29 + 0x0);
    if (r31 < tmp) {
        tmp = r30 + r31;
        *(u32*)((u8*)r29 + 0x30) = tmp;
    }
    *(u32*)((u8*)r29 + 0x40) = r30;
    r5 = 0x0;
    while (1) {
        tmp = *(u32*)((u8*)r29 + 0x8);
        if (r5 >= tmp) break;
        r3 = *(u32*)((u8*)r29 + 0x24);
        tmp = r5 << 2;
        r4 = *(u32*)((u8*)r29 + 0x20);
        r5 = r5 + 0x1;
        r3 = *(u32*)(r3 + tmp);
        tmp = *(u32*)((u8*)r29 + 0x20);
        r4 = r4 + r3;
        r3 = *(u32*)((u8*)r4 + 0x0);
        tmp = r3 + tmp;
        *(u32*)((u8*)r4 + 0x0) = tmp;


    }
    r3 = 0x0;

    return;
}

/* 0x801920E4 | 0x1664 */
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
    u32 tmp = 0;
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

    /* mr. r28, r3 */;
    r3 = (u32)lbl_802744F0;
    r25 = r4;
    r31 = (u32)lbl_802744F0;
    r24 = r5;
    r23 = 0x0;
    r27 = 0x0;
    if ((s32)tmp != 0) goto L_8019211C;
    f1 = *(f32*)lbl_8047D908;
    return;
L_8019211C:
    if ((s32)r27 <= 0) goto L_801922BC;
    tmp = *(u8*)((u8*)r28 + 0x0);
    /* subic. r27, r27, 0x1 */;
    r3 = r29 << 8;
    r28 = r28 + 0x1;
    r29 = r3;
    r29 = (r29 & ~0x000000FF) | (((tmp << 0) | ((u32)tmp >> 32)) & 0x000000FF);
    if ((s32)r27 != 0) goto L_8019211C;
    tmp = r26 & 0xFF;
    if ((s32)tmp == 6) goto L_80192284;
    if ((s32)tmp >= 6) goto L_80192174;
    if ((s32)tmp == 3) goto L_80192244;
    if ((s32)tmp >= 3) goto L_80192168;
    if ((s32)tmp >= 2) goto L_8019218C;
    goto L_801922A8;
L_80192168:
    if ((s32)tmp >= 5) goto L_801921BC;
    goto L_8019227C;
L_80192174:
    if ((s32)tmp == 0xff) goto L_80192298;
    if ((s32)tmp >= 0xff) goto L_801922A8;
    if ((s32)tmp == 0x3c) goto L_801921E0;
    goto L_801922A8;
L_8019218C:
    if (r29 >= r24) {
        r3 = r31 + 0x0;
        r5 = r31 + 0xc;
        r4 = 0x119;
        fn_80196E10();
    }
    tmp = r29 << 2;
    r3 = r23;
    r4 = *(u32*)(r25 + tmp);
    fn_801A3EB4();
    r23 = r3;
    goto L_8019211C;
L_801921BC:
    r22 = 0x0;
    while (r22 < r29) {

        r3 = r23;
        fn_801A3E64();
        r23 = r3;
        r22 = r22 + 0x1;

    }
    goto L_8019211C;
L_801921E0:
    r3 = r23;
    r22 = 0x0;
    while (r3 != 0 && r22 < r29) {

        r3 = *(u32*)((u8*)r3 + 0x0);
        r22 = r22 + 0x1;


    }

    if (r3 != 0) goto L_80192230;
    r4 = r29;
    r3 = r31 + 0x20;
    OSReport();
    r3 = r31 + 0x0;
    r4 = 0x12b;
    r5 = (u32)lbl_8047D90C;
    fn_80196D78();
    goto L_8019211C;
L_80192230:
    r4 = *(u32*)((u8*)r3 + 0x4);
    r3 = r23;
    fn_801A3EB4();
    r23 = r3;
    goto L_8019211C;
L_80192244:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x133;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x4);
    if ((s32)tmp != 0) {
        r28 = r28 + r29;
    }
    r3 = r23;
    fn_801A3E64();
    r23 = r3;
    goto L_8019211C;
L_8019227C:
    r28 = r28 + r29;
    goto L_8019211C;
L_80192284:
    r3 = r23;
    r4 = r29;
    fn_801A3EB4();
    r23 = r3;
    goto L_8019211C;
L_80192298:
    r3 = r31 + 0x0;
    r5 = r31 + 0x48;
    r4 = 0x143;
    fn_80196D78();
L_801922A8:
    r3 = r31 + 0x0;
    r5 = r31 + 0x60;
    r4 = 0x146;
    fn_80196D78();
    goto L_8019211C;
L_801922BC:
    r26 = *(u8*)((u8*)r28 + 0x0);
    r28 = r28 + 0x1;
    tmp = r26 & 0xFF;
    if ((s32)tmp == 0x20) goto L_80192CE8;
    if ((s32)tmp >= 0x20) goto L_801923AC;
    if ((s32)tmp == 0x10) goto L_801926C4;
    if ((s32)tmp >= 0x10) goto L_80192348;
    if ((s32)tmp == 8) goto L_80192510;
    if ((s32)tmp >= 8) goto L_8019231C;
    if ((s32)tmp == 5) goto L_801924BC;
    if ((s32)tmp >= 5) goto L_80192310;
    if ((s32)tmp == 1) goto L_8019247C;
    if ((s32)tmp >= 1) goto L_801924C8;
    if ((s32)tmp >= 0) goto L_8019211C;
    goto L_80193710;
L_80192310:
    if ((s32)tmp >= 7) goto L_801924E0;
    goto L_801924D4;
L_8019231C:
    if ((s32)tmp == 0xc) goto L_801925D8;
    if ((s32)tmp >= 0xc) goto L_80192338;
    if ((s32)tmp == 0xa) goto L_80192588;
    if ((s32)tmp >= 0xa) goto L_801925B0;
    goto L_80192558;
L_80192338:
    if ((s32)tmp == 0xe) goto L_80192644;
    if ((s32)tmp >= 0xe) goto L_80192684;
    goto L_80192604;
L_80192348:
    if ((s32)tmp == 0x18) goto L_80192A04;
    if ((s32)tmp >= 0x18) goto L_80192380;
    if ((s32)tmp == 0x14) goto L_801927B8;
    if ((s32)tmp >= 0x14) goto L_80192370;
    if ((s32)tmp == 0x12) goto L_80192744;
    if ((s32)tmp >= 0x12) goto L_80192784;
    goto L_80192704;
L_80192370:
    if ((s32)tmp == 0x16) goto L_80192858;
    if ((s32)tmp >= 0x16) goto L_801929A0;
    goto L_801927EC;
L_80192380:
    if ((s32)tmp == 0x1c) goto L_80192B98;
    if ((s32)tmp >= 0x1c) goto L_8019239C;
    if ((s32)tmp == 0x1a) goto L_80192ACC;
    if ((s32)tmp >= 0x1a) goto L_80192B30;
    goto L_80192A68;
L_8019239C:
    if ((s32)tmp == 0x1e) goto L_80192C40;
    if ((s32)tmp >= 0x1e) goto L_80192C94;
    goto L_80192BEC;
L_801923AC:
    if ((s32)tmp == 0x30) goto L_801934D4;
    if ((s32)tmp >= 0x30) goto L_8019241C;
    if ((s32)tmp == 0x28) goto L_80192828;
    if ((s32)tmp >= 0x28) goto L_801923F0;
    if ((s32)tmp == 0x24) goto L_80192E74;
    if ((s32)tmp >= 0x24) goto L_801923E0;
    if ((s32)tmp == 0x22) goto L_80192DAC;
    if ((s32)tmp >= 0x22) goto L_80192E10;
    goto L_80192D44;
L_801923E0:
    if ((s32)tmp == 0x26) goto L_80192F24;
    if ((s32)tmp >= 0x26) goto L_801936B0;
    goto L_80192ECC;
L_801923F0:
    if ((s32)tmp == 0x2c) goto L_80193350;
    if ((s32)tmp >= 0x2c) goto L_8019240C;
    if ((s32)tmp == 0x2a) goto L_8019328C;
    if ((s32)tmp >= 0x2a) goto L_801932F0;
    goto L_80193228;
L_8019240C:
    if ((s32)tmp == 0x2e) goto L_8019340C;
    if ((s32)tmp >= 0x2e) goto L_8019346C;
    goto L_801933B0;
L_8019241C:
    if ((s32)tmp == 0x38) goto L_801931C0;
    if ((s32)tmp >= 0x38) goto L_80192454;
    if ((s32)tmp == 0x34) goto L_80193028;
    if ((s32)tmp >= 0x34) goto L_80192444;
    if ((s32)tmp == 0x32) goto L_8019353C;
    if ((s32)tmp >= 0x32) goto L_80192FC4;
    goto L_80192974;
L_80192444:
    if ((s32)tmp == 0x36) goto L_801930F4;
    if ((s32)tmp >= 0x36) goto L_8019315C;
    goto L_8019308C;
L_80192454:
    if ((s32)tmp == 0x3c) goto L_801924BC;
    if ((s32)tmp >= 0x3c) goto L_80192470;
    if ((s32)tmp == 0x3a) goto L_80193608;
    if ((s32)tmp >= 0x3a) goto L_8019365C;
    goto L_801935B4;
L_80192470:
    if ((s32)tmp == 0xff) goto L_801924BC;
    goto L_80193710;
L_8019247C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x153;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    *(f32*)(sp + 0xC) = f0;
    while (r23 != 0) {

        r3 = r23;
        fn_801A3E64();
        r23 = r3;

    }
    f1 = *(f32*)(sp + 0xC);
    return;
L_801924BC:
    r27 = 0x1;
    r29 = 0x0;
    goto L_8019211C;
L_801924C8:
    r27 = 0x2;
    r29 = 0x0;
    goto L_8019211C;
L_801924D4:
    r27 = 0x4;
    r29 = 0x0;
    goto L_8019211C;
L_801924E0:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x178;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = (f64)(s32)f1;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192510:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x17d;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    r22 = *(u32*)((u8*)r23 + 0x4);
    tmp = 0x43300000;
    *(u32*)(sp + 0x18) = tmp;
    f1 = *(f64*)lbl_8047D948;
    *(u32*)(sp + 0x1C) = tmp;
    f0 = f0 - f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192558:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x183;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = -f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192588:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x189;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    r22 = *(u32*)((u8*)r23 + 0x4);
    tmp = -r22;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801925B0:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x18f;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    r3 = 0x2;
    fn_801ADC3C();
    *(u32*)((u8*)r23 + 0x4) = r3;
    goto L_8019211C;
L_801925D8:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x195;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    fn_801ADC7C();
    *(f32*)(sp + 0x10) = f1;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192604:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x19b;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f64*)lbl_8047D918;
    f1 = f0 * f1;
    f1 = (f32)f1;
    fn_800CE148();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192644:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1a1;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f64*)lbl_8047D918;
    f1 = f0 * f1;
    f1 = (f32)f1;
    fn_800CDBE0();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192684:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1a7;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f64*)lbl_8047D918;
    f1 = f0 * f1;
    f1 = (f32)f1;
    fn_800CE220();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801926C4:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1ad;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE2B8();
    f1 = (f32)f1;
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192704:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1b3;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE298();
    f1 = (f32)f1;
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192744:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1b9;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CD85C();
    f1 = (f32)f1;
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192784:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1bf;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE338();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801927B8:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1c5;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    fn_800CE2F8();
    f0 = (f32)f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801927EC:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1cb;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    f1 = *(f32*)((u8*)r23 + 0x4);
    f0 = *(f32*)lbl_8047D908;
    if (f1 >= f0) goto L_8019211C;
    f0 = -f1;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192828:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1d3;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    r22 = *(u32*)((u8*)r23 + 0x4);
    if ((s32)r22 >= 0) goto L_8019211C;
    tmp = -r22;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192858:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1da;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
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
L_801928D8:
    f0 = *(f64*)lbl_8047D938;
    if (f1 >= f0) goto L_801928F0;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_80192964;
L_801928F0:
    *(f32*)(sp + 0x8) = f1;
    tmp = 0x7F800000;
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)tmp) goto L_80192918;
    if ((s32)r3 >= (s32)tmp) goto L_80192950;
    if ((s32)r3 == 0) goto L_80192934;
    goto L_80192950;
L_80192918:
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_8019292C;
    tmp = 0x1;
    goto L_80192954;
L_8019292C:
    tmp = 0x2;
    goto L_80192954;
L_80192934:
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_80192948;
    tmp = 0x5;
    goto L_80192954;
L_80192948:
    tmp = 0x3;
    goto L_80192954;
L_80192950:
    tmp = 0x4;
L_80192954:
    if ((s32)tmp != 1) goto L_80192964;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
L_80192964:
    *(f32*)(sp + 0x10) = f1;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192974:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1e0;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    r22 = *(u32*)((u8*)r23 + 0x4);
    tmp = __cntlzw(r22);
    tmp = (u32)tmp >> 5;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801929A0:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1f5;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x1f5;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 + f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192A04:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x1fb;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x1fb;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 - f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192A68:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x201;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x201;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 * f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192ACC:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x207;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x207;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    f0 = f1 / f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192B30:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x20d;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x20d;
        fn_80196E10();
    }
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
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192B98:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x213;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x213;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r22 + r30;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192BEC:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x218;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x218;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r22 - r30;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192C40:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x21d;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x21d;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r22 * r30;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192C94:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x222;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x222;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = (s32)r22 / (s32)r30;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192CE8:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x227;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x227;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = (s32)r22 / (s32)r30;
    tmp = tmp * r30;
    tmp = r22 - tmp;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80192D44:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x22c;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x22c;
        fn_80196E10();
    }
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
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192DAC:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x232;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x232;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    if (f1 <= f0) goto L_8019211C;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192E10:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x239;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x239;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    if (f1 >= f0) goto L_8019211C;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192E74:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x240;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x240;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    if ((s32)r22 <= (s32)r30) goto L_8019211C;
    *(u32*)((u8*)r23 + 0x4) = r30;
    goto L_8019211C;
L_80192ECC:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x247;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x247;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    if ((s32)r22 >= (s32)r30) goto L_8019211C;
    *(u32*)((u8*)r23 + 0x4) = r30;
    goto L_8019211C;
L_80192F24:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x24e;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x24e;
        fn_80196E10();
    }
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
L_80192F98:
    f1 = *(f32*)lbl_8047D944;
    goto L_80192FA8;
L_80192FA0:
    fn_800CE2D8();
    f1 = (f32)f1;
L_80192FA8:
    f0 = *(f64*)lbl_8047D920;
    f0 = f0 * f1;
    f0 = (f32)f0;
    *(f32*)(sp + 0x10) = f0;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80192FC4:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x254;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x254;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    tmp = 0; /* mfcr */;
    tmp = (u32)tmp >> 31;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80193028:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x259;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x259;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    tmp = 0; /* mfcr */;
    /* extrwi tmp, tmp, 1, 1 */;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_8019308C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x25e;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x25e;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    /* cror eq, lt, eq */;
    tmp = 0; /* mfcr */;
    /* extrwi tmp, tmp, 1, 2 */;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_801930F4:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x263;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x263;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    /* cror eq, gt, eq */;
    tmp = 0; /* mfcr */;
    /* extrwi tmp, tmp, 1, 2 */;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_8019315C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x268;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x268;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    tmp = 0; /* mfcr */;
    /* extrwi tmp, tmp, 1, 2 */;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_801931C0:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x26d;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x26d;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r23 + 0x4);
    r3 = r23;
    *(f32*)(sp + 0xC) = f0;
    fn_801A3E64();
    f0 = *(f32*)(sp + 0xC);
    r23 = r3;
    f1 = *(f32*)((u8*)r3 + 0x4);
    tmp = 0; /* mfcr */;
    /* extrwi tmp, tmp, 1, 2 */;
    tmp = tmp ^ 0x1;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80193228:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x272;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x272;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r30 ^ r22;
    r3 = (s32)tmp >> 1;
    tmp = tmp & r30;
    tmp = r3 - tmp;
    tmp = (u32)tmp >> 31;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_8019328C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x277;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x277;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r22 ^ r30;
    r3 = (s32)tmp >> 1;
    tmp = tmp & r22;
    tmp = r3 - tmp;
    tmp = (u32)tmp >> 31;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801932F0:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x27c;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x27c;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r4 = (s32)r30 >> 31;
    r23 = r3;
    r3 = (u32)r22 >> 31;
    tmp = r30 - r22;
    tmp = r4 + r3; /* +carry */;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80193350:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x281;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x281;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r4 = (u32)r30 >> 31;
    r3 = (s32)r22 >> 31;
    tmp = r22 - r30;
    tmp = r3 + r4; /* +carry */;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801933B0:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x286;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x286;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r30 - r22;
    tmp = __cntlzw(tmp);
    tmp = (u32)tmp >> 5;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_8019340C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x28b;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x28b;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r3 = r30 - r22;
    tmp = r22 - r30;
    tmp = r3 | tmp;
    tmp = (u32)tmp >> 31;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_8019346C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x290;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x290;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    tmp = 0x0;
    r23 = r3;
    if (((s32)r22 != 0) && ((s32)r30 != 0)) {

        tmp = 0x1;
    }
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801934D4:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x295;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x295;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    tmp = 0x0;
    r23 = r3;
    if ((s32)r22 != 0) goto L_80193530;
    if ((s32)r30 == 0) goto L_80193534;
L_80193530:
    tmp = 0x1;
L_80193534:
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_8019353C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x29a;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x29a;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    tmp = 0x0;
    r23 = r3;
    if ((s32)r22 != 0) goto L_80193598;
    if ((s32)r30 != 0) goto L_801935A8;
L_80193598:
    if ((s32)r22 == 0 || (s32)r30 != 0) goto L_801935AC;

L_801935A8:
    tmp = 0x1;
L_801935AC:
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_801935B4:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x29f;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x29f;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r22 & r30;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_80193608:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x2a4;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x2a4;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r22 | r30;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_8019365C:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x2a9;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x2a9;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    tmp = r22 ^ r30;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_8019211C;
L_801936B0:
    if (r23 == 0) {
        r3 = r31 + 0x0;
        r4 = 0x2af;
        r5 = (u32)lbl_8047D910;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r23 + 0x0);
    if (tmp == 0) {
        r3 = r31 + 0x0;
        r5 = r31 + 0x78;
        r4 = 0x2af;
        fn_80196E10();
    }
    r30 = *(u32*)((u8*)r23 + 0x4);
    r3 = r23;
    fn_801A3E64();
    r22 = *(u32*)((u8*)r3 + 0x4);
    r23 = r3;
    r3 = r30 - r22;
    r3 = r3 + 0x1;
    fn_801ADC3C();
    tmp = r22 + r3;
    *(u32*)((u8*)r23 + 0x4) = tmp;
    goto L_8019211C;
L_80193710:
    r3 = r31 + 0x84;
    r4 = r26 & 0xFF;
    OSReport();
    r3 = r31 + 0x0;
    r4 = 0x2b5;
    r5 = (u32)lbl_8047D90C;
    fn_80196D78();
    goto L_8019211C;

    return;
}
