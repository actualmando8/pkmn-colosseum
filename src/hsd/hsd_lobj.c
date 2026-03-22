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
void fn_801A4000(void) {
    extern u8 lbl_80274D70[];
    extern u8 lbl_80274D88[];
    extern u8 lbl_8036CA20[];
    extern u8 lbl_8036CC00[];
    extern void fn_80193B30();
    extern void fn_801A4098();
    extern void fn_801A40F8();
    extern void fn_801A4440();
    extern void fn_801A6494();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r3 = (u32)lbl_8036CA20;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274D70;
    r6 = (u32)lbl_80274D88;
    r3 = (u32)lbl_8036CA20;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274D70;
    r6 = (u32)lbl_80274D88;
    r7 = 0x44;
    r8 = 0xd4;
    fn_80193B30();
    r6 = (u32)fn_801A40F8;
    r5 = (u32)fn_801A4098;
    r3 = (u32)fn_801A6494;
    r8 = (u32)lbl_8036CA20;
    r9 = (u32)fn_801A40F8;
    r6 = (u32)lbl_8036CA20;
    tmp = (u32)fn_801A6494;
    r3 = (u32)lbl_8036CA20;
    r8 = (u32)lbl_8036CA20;
    r4 = (u32)fn_801A4440;
    r7 = (u32)fn_801A4098;
    r6 = (u32)lbl_8036CA20;
    r5 = (u32)fn_801A4440;
    r4 = (u32)lbl_8036CA20;
    r4 = (u32)lbl_8036CA20;
    r3 = (u32)lbl_8036CA20;
    *(u32*)((u8*)r8 + 0x30) = r9;
    *(u32*)((u8*)r6 + 0x38) = r7;
    *(u32*)((u8*)r4 + 0x3C) = r5;
    *(u32*)((u8*)r3 + 0x40) = tmp;
    return;
}

/* 0x60 | fn_801A4098 | framed_no_calls */
void fn_801A4098(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B2B4, lbl_8047B2B0 */
}

/* 0x801A40F8 | 0x174 */
void fn_801A40F8(void) {
    extern u8 lbl_8036CA20[];
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x48);
    fn_801C25E4();
    if (r31 != 0) {
        r30 = *(u32*)((u8*)r31 + 0x18);
    } else {

        r30 = 0x0;
    }
    if (r30 != 0) {
        r3 = 0x10000;
        r4 = *(u16*)((u8*)r30 + 0x4);
        tmp = r3 & 0xFFFF;
        tmp = tmp - r4;
        tmp = __cntlzw(tmp);
        /* srwi. r3, tmp, 5 */;
        if (r30 != 0) {
        } else {

            tmp = *(u16*)((u8*)r30 + 0x4);
            r3 = *(u16*)((u8*)r30 + 0x4);
            r4 = __cntlzw(tmp);
            *(u16*)((u8*)r30 + 0x4) = tmp;
            r3 = (u32)r4 >> 5;
        }
        if ((s32)r3 != 0) {
            if (r30 != 0) {
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
    }
    }
    }
    if (r31 != 0) {
        r30 = *(u32*)((u8*)r31 + 0x1C);
    } else {

        r30 = 0x0;
    }
    if (r30 != 0) {
        r3 = 0x10000;
        r4 = *(u16*)((u8*)r30 + 0x4);
        tmp = r3 & 0xFFFF;
        tmp = tmp - r4;
        tmp = __cntlzw(tmp);
        /* srwi. r3, tmp, 5 */;
        if (r30 != 0) {
        } else {

            tmp = *(u16*)((u8*)r30 + 0x4);
            r3 = *(u16*)((u8*)r30 + 0x4);
            r4 = __cntlzw(tmp);
            *(u16*)((u8*)r30 + 0x4) = tmp;
            r3 = (u32)r4 >> 5;
        }
        if ((s32)r3 != 0) {
            if (r30 != 0) {
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
    }
    }
    }
    r4 = (u32)lbl_8036CA20;
    r3 = r31;
    r4 = (u32)lbl_8036CA20;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

/* 0x801A426C | 0xD8 */
void fn_801A426C(void) {
    extern void fn_80191DCC();
    extern void fn_801C25E4();
    extern void fn_801C2670();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if (r3 == 0) return;
    r31 = r3;
    r30 = r4;
    goto L_801A4324;
L_801A4294:
    if ((r31 != 0) && (r30 != 0)) {

        tmp = *(u32*)((u8*)r31 + 0x48);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r31 + 0x48);
            fn_801C25E4();
        }
        r3 = *(u32*)((u8*)r30 + 0x4);
        fn_801C2670();
        *(u32*)((u8*)r31 + 0x48) = r3;
        if (r31 != 0) {
            r3 = *(u32*)((u8*)r31 + 0x18);
        } else {

            r3 = 0x0;
        }
        r4 = *(u32*)((u8*)r30 + 0x8);
        fn_80191DCC();
        if (r31 != 0) {
            r3 = *(u32*)((u8*)r31 + 0x1C);
        } else {

            r3 = 0x0;
        }
        r4 = *(u32*)((u8*)r30 + 0xC);
        fn_80191DCC();
    }
    if (r31 != 0) {
        r31 = *(u32*)((u8*)r31 + 0xC);
    } else {

        r31 = 0x0;
    }
    if (r30 != 0) {
        r30 = *(u32*)((u8*)r30 + 0x0);
        goto L_801A4324;
    }
    r30 = 0x0;
L_801A4324:
    if (r31 != 0) goto L_801A4294;

    return;
}

/* 0x801A4344 | 0xFC */
void fn_801A4344(void) {
    extern u8 lbl_8036CA20[];
    extern u8 lbl_8047B2B0[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBC0[];
    extern u8 lbl_8047DBC4[];
    extern void fn_80193748();
    extern void fn_80193828();
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = (u32)sp + 0x8;
    r30 = r3;
    while (1) {
        if (r30 == 0) break;
        tmp = *(u32*)((u8*)r30 + 0x0);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r30 + 0x0);
            fn_80193748();
            if (r3 == 0) {
            }
            tmp = *(u32*)lbl_8047B2B0;
            if (tmp != 0) {
                r3 = *(u32*)lbl_8047B2B0;
            } else {

                r3 = (u32)lbl_8036CA20;
                r3 = (u32)lbl_8036CA20;
            }
            fn_80193828();
            /* mr. r29, r3 */;
            if (tmp == 0) {
                r3 = (u32)lbl_8047DBB8;
                r4 = 0x5d5;
                r5 = (u32)lbl_8047DBC0;
                fn_80196E10();
            }
            *(u32*)((u8*)r31 + 0x0) = r29;
            goto L_801A43E8;
            }
        fn_80193828();
        *(u32*)((u8*)r31 + 0x0) = r3;
        tmp = *(u32*)((u8*)r31 + 0x0);
        if (tmp != 0) goto L_801A43E8;
        r3 = (u32)lbl_8047DBB8;
        r4 = 0x67b;
        r5 = (u32)lbl_8047DBC4;
        fn_80196E10();
    L_801A43E8:
        r5 = *(u32*)((u8*)r31 + 0x0);
        r4 = r30;
        r3 = *(u32*)((u8*)r31 + 0x0);
        r5 = *(u32*)((u8*)r5 + 0x0);
        r12 = *(u32*)((u8*)r5 + 0x3C);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        r3 = *(u32*)((u8*)r31 + 0x0);
        r30 = *(u32*)((u8*)r30 + 0x4);
        r31 = r3 + 0xc;

    }
    tmp = 0x0;
    *(u32*)((u8*)r31 + 0x0) = tmp;
    return;
}

/* 0x801A4440 | 0x470 */
void fn_801A4440(void) {
    extern u8 lbl_80274D94[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBC8[];
    extern void fn_801917D0();
    extern void fn_80196D78();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
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
    void (*ctr_fn)(void) = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    tmp = *(u32*)((u8*)r4 + 0xC);
    *(u32*)(sp + 0x8) = tmp;
    *(u32*)((u8*)r3 + 0x10) = tmp;
    r3 = *(u16*)((u8*)r4 + 0x8);
    if ((s32)tmp != 0) {
        tmp = *(u16*)((u8*)r30 + 0x8);
        tmp = tmp | r3;
        *(u16*)((u8*)r30 + 0x8) = tmp;
    }
    tmp = *(u16*)((u8*)r31 + 0x8);
    tmp = tmp & 0x3;
    if ((s32)tmp != 2) {
        if ((s32)tmp < 2) {
            if ((s32)tmp == 0) { r3 = 0x0; return; }
            if ((s32)tmp < 0) {
                goto L_801A4868;
            }
            if ((s32)tmp >= 4) goto L_801A4868;
            goto L_801A466C;
            }
        r3 = *(u32*)((u8*)r31 + 0x10);
        fn_801917D0();
        r31 = r3;
        if (r30 == 0) { r3 = 0x0; return; }
        r29 = *(u32*)((u8*)r30 + 0x18);
        if (r29 != 0) {
            r3 = 0x10000;
            r4 = *(u16*)((u8*)r29 + 0x4);
            tmp = r3 & 0xFFFF;
            tmp = tmp - r4;
            tmp = __cntlzw(tmp);
            /* srwi. r3, tmp, 5 */;
            if (r29 != 0) {
            } else {

                tmp = *(u16*)((u8*)r29 + 0x4);
                r3 = *(u16*)((u8*)r29 + 0x4);
                r4 = __cntlzw(tmp);
                *(u16*)((u8*)r29 + 0x4) = tmp;
                r3 = (u32)r4 >> 5;
            }
            if ((s32)r3 != 0) {
                if (r29 != 0) {
                    r4 = *(u32*)((u8*)r29 + 0x0);
                    r3 = r29;
                    r12 = *(u32*)((u8*)r4 + 0x30);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
                    r4 = *(u32*)((u8*)r29 + 0x0);
                    r3 = r29;
                    r12 = *(u32*)((u8*)r4 + 0x34);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
        }
        }
        }
        *(u32*)((u8*)r30 + 0x18) = r31;
        r3 = 0x0;
        return;
    }
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801917D0();
    r29 = r3;
    if (r30 != 0) {
        r28 = *(u32*)((u8*)r30 + 0x18);
        if (r28 != 0) {
            r3 = 0x10000;
            r4 = *(u16*)((u8*)r28 + 0x4);
            tmp = r3 & 0xFFFF;
            tmp = tmp - r4;
            tmp = __cntlzw(tmp);
            /* srwi. r3, tmp, 5 */;
            if (r28 != 0) {
            } else {

                tmp = *(u16*)((u8*)r28 + 0x4);
                r3 = *(u16*)((u8*)r28 + 0x4);
                r4 = __cntlzw(tmp);
                *(u16*)((u8*)r28 + 0x4) = tmp;
                r3 = (u32)r4 >> 5;
            }
            if ((s32)r3 != 0) {
                if (r28 != 0) {
                    r4 = *(u32*)((u8*)r28 + 0x0);
                    r3 = r28;
                    r12 = *(u32*)((u8*)r4 + 0x30);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
                    r4 = *(u32*)((u8*)r28 + 0x0);
                    r3 = r28;
                    r12 = *(u32*)((u8*)r4 + 0x34);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
        }
        }
        }
        *(u32*)((u8*)r30 + 0x18) = r29;
    }
    tmp = *(u16*)((u8*)r31 + 0xA);
    tmp = tmp & 0x1;
    if (r28 != 0) {
        if (r30 != 0) {
            tmp = *(u16*)((u8*)r30 + 0x8);
            tmp = tmp | 0x40;
            *(u16*)((u8*)r30 + 0x8) = tmp;
        }
        r5 = *(u32*)((u8*)r31 + 0x18);
        r4 = *(u32*)((u8*)r31 + 0x18);
        r3 = *(u32*)((u8*)r31 + 0x18);
        f2 = *(f32*)((u8*)r5 + 0x14);
        f1 = *(f32*)((u8*)r4 + 0x10);
        f0 = *(f32*)((u8*)r3 + 0xC);
        if (r30 == 0) { r3 = 0x0; return; }
        *(f32*)((u8*)r30 + 0x2C) = f0;
        *(f32*)((u8*)r30 + 0x30) = f1;
        *(f32*)((u8*)r30 + 0x34) = f2;
        r3 = 0x0;
        return;
    }
    r5 = *(u32*)((u8*)r31 + 0x18);
    r4 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x18);
    tmp = *(u32*)((u8*)r5 + 0x8);
    f1 = *(f32*)((u8*)r4 + 0x0);
    f0 = *(f32*)((u8*)r3 + 0x4);
    if (r30 == 0) { r3 = 0x0; return; }
    *(f32*)((u8*)r30 + 0x2C) = f0;
    *(f32*)((u8*)r30 + 0x28) = f1;
    *(u32*)((u8*)r30 + 0x30) = tmp;
    r3 = 0x0;
    return;
L_801A466C:
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801917D0();
    r29 = r3;
    if (r30 != 0) {
        r28 = *(u32*)((u8*)r30 + 0x18);
        if (r28 != 0) {
            r3 = 0x10000;
            r4 = *(u16*)((u8*)r28 + 0x4);
            tmp = r3 & 0xFFFF;
            tmp = tmp - r4;
            tmp = __cntlzw(tmp);
            /* srwi. r3, tmp, 5 */;
            if (r28 != 0) {
            } else {

                tmp = *(u16*)((u8*)r28 + 0x4);
                r3 = *(u16*)((u8*)r28 + 0x4);
                r4 = __cntlzw(tmp);
                *(u16*)((u8*)r28 + 0x4) = tmp;
                r3 = (u32)r4 >> 5;
            }
            if ((s32)r3 != 0) {
                if (r28 != 0) {
                    r4 = *(u32*)((u8*)r28 + 0x0);
                    r3 = r28;
                    r12 = *(u32*)((u8*)r4 + 0x30);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
                    r4 = *(u32*)((u8*)r28 + 0x0);
                    r3 = r28;
                    r12 = *(u32*)((u8*)r4 + 0x34);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
        }
        }
        }
        *(u32*)((u8*)r30 + 0x18) = r29;
    }
    r3 = *(u32*)((u8*)r31 + 0x14);
    fn_801917D0();
    r29 = r3;
    if (r30 != 0) {
        r28 = *(u32*)((u8*)r30 + 0x1C);
        if (r28 != 0) {
            r3 = 0x10000;
            r4 = *(u16*)((u8*)r28 + 0x4);
            tmp = r3 & 0xFFFF;
            tmp = tmp - r4;
            tmp = __cntlzw(tmp);
            /* srwi. r3, tmp, 5 */;
            if (r28 != 0) {
            } else {

                tmp = *(u16*)((u8*)r28 + 0x4);
                r3 = *(u16*)((u8*)r28 + 0x4);
                r4 = __cntlzw(tmp);
                *(u16*)((u8*)r28 + 0x4) = tmp;
                r3 = (u32)r4 >> 5;
            }
            if ((s32)r3 != 0) {
                if (r28 != 0) {
                    r4 = *(u32*)((u8*)r28 + 0x0);
                    r3 = r28;
                    r12 = *(u32*)((u8*)r4 + 0x30);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
                    r4 = *(u32*)((u8*)r28 + 0x0);
                    r3 = r28;
                    r12 = *(u32*)((u8*)r4 + 0x34);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
        }
        }
        }
        *(u32*)((u8*)r30 + 0x1C) = r29;
    }
    tmp = *(u16*)((u8*)r31 + 0xA);
    tmp = tmp & 0x1;
    if (r28 != 0) {
        if (r30 != 0) {
            tmp = *(u16*)((u8*)r30 + 0x8);
            tmp = tmp | 0x40;
            *(u16*)((u8*)r30 + 0x8) = tmp;
        }
        r8 = *(u32*)((u8*)r31 + 0x18);
        r7 = *(u32*)((u8*)r31 + 0x18);
        r6 = *(u32*)((u8*)r31 + 0x18);
        r5 = *(u32*)((u8*)r31 + 0x18);
        r4 = *(u32*)((u8*)r31 + 0x18);
        r3 = *(u32*)((u8*)r31 + 0x18);
        f5 = *(f32*)((u8*)r8 + 0x14);
        f4 = *(f32*)((u8*)r7 + 0x10);
        f3 = *(f32*)((u8*)r6 + 0xC);
        f2 = *(f32*)((u8*)r5 + 0x8);
        f1 = *(f32*)((u8*)r4 + 0x4);
        f0 = *(f32*)((u8*)r3 + 0x0);
        if (r30 != 0) {
            *(f32*)((u8*)r30 + 0x20) = f0;
            *(f32*)((u8*)r30 + 0x24) = f1;
            *(f32*)((u8*)r30 + 0x28) = f2;
        }
        if (r30 == 0) { r3 = 0x0; return; }
        *(f32*)((u8*)r30 + 0x2C) = f3;
        *(f32*)((u8*)r30 + 0x30) = f4;
        *(f32*)((u8*)r30 + 0x34) = f5;
        r3 = 0x0;
        return;
    }
    r5 = *(u32*)((u8*)r31 + 0x18);
    r4 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x18);
    tmp = *(u32*)((u8*)r5 + 0x10);
    f1 = *(f32*)((u8*)r4 + 0x8);
    f0 = *(f32*)((u8*)r3 + 0xC);
    if (r30 != 0) {
        *(f32*)((u8*)r30 + 0x2C) = f0;
        *(f32*)((u8*)r30 + 0x28) = f1;
        *(u32*)((u8*)r30 + 0x30) = tmp;
    }
    r4 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x18);
    tmp = *(u32*)((u8*)r4 + 0x4);
    f0 = *(f32*)((u8*)r3 + 0x0);
    if (r30 == 0) { r3 = 0x0; return; }
    *(f32*)((u8*)r30 + 0x20) = f0;
    *(u32*)((u8*)r30 + 0x24) = tmp;
    r3 = 0x0;
    return;
L_801A4868:
    r3 = (u32)lbl_80274D94;
    r4 = *(u16*)((u8*)r31 + 0x8);
    r3 = (u32)lbl_80274D94;
    OSReport();
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x659;
    r5 = (u32)lbl_8047DBC8;
    fn_80196D78();

    r3 = 0x0;
    return;
}

/* 0x44 | fn_801A48B0 | guarded_call */
u32 fn_801A48B0(void) {
    if (0 /* guard r3 == 0 */) { return 1; }
    if (0 /* guard r0 == 0 */) { return 1; }
    fn_80191688();
    return 0;
}

/* 0x801A48F4 | 0x88 */
void fn_801A48F4(void) {
    extern u8 lbl_80274DB8[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBCC[];
    extern void fn_80191628();
    extern void fn_80191788();
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) {
        r3 = (u32)lbl_8047DBB8;
        r4 = 0x58c;
        r5 = (u32)lbl_8047DBCC;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r30 + 0x1C);
    if (tmp == 0) {
        fn_80191628();
        *(u32*)((u8*)r30 + 0x1C) = r3;
        tmp = *(u32*)((u8*)r30 + 0x1C);
        if (tmp == 0) {
            r4 = (u32)lbl_80274DB8;
            r3 = (u32)lbl_8047DBB8;
            r5 = (u32)lbl_80274DB8;
            r4 = 0x58f;
            fn_80196E10();
    }
    }
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = r31;
    fn_80191788();
    return;
}

/* 0x44 | fn_801A497C | guarded_call */
u32 fn_801A497C(void) {
    if (0 /* guard r3 == 0 */) { return 1; }
    if (0 /* guard r0 == 0 */) { return 1; }
    fn_80191688();
    return 0;
}

/* 0x801A49C0 | 0x88 */
void fn_801A49C0(void) {
    extern u8 lbl_80274DC8[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBCC[];
    extern void fn_80191628();
    extern void fn_80191788();
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) {
        r3 = (u32)lbl_8047DBB8;
        r4 = 0x568;
        r5 = (u32)lbl_8047DBCC;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r30 + 0x18);
    if (tmp == 0) {
        fn_80191628();
        *(u32*)((u8*)r30 + 0x18) = r3;
        tmp = *(u32*)((u8*)r30 + 0x18);
        if (tmp == 0) {
            r4 = (u32)lbl_80274DC8;
            r3 = (u32)lbl_8047DBB8;
            r5 = (u32)lbl_80274DC8;
            r4 = 0x56b;
            fn_80196E10();
    }
    }
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r31;
    fn_80191788();
    return;
}

/* 0x801A4A48 | 0xC */
void fn_801A4A48(void) {
}

/* 0x70 | fn_801A4A54 | generic */
u32 fn_801A4A54(u32 arg1) {
    /* refs: jumptable_8036CA64 */
    return 0;
}

/* 0x801A4AC4 | 0x3C */
void fn_801A4AC4(void) {
    extern u8 lbl_8047B2B4[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = *(u32*)lbl_8047B2B4;
    r3 = r3 & 0x3;
    while (r5 != 0) {

        r4 = *(u32*)((u8*)r5 + 0x4);
        tmp = *(u16*)((u8*)r4 + 0x8);
        tmp = tmp & 0x3;
        if (r3 == tmp) {
            r3 = *(u32*)((u8*)r5 + 0x4);
            return;
        }
        r5 = *(u32*)((u8*)r5 + 0x0);

    }
    r3 = 0x0;
    return;
}

/* 0x801A4B00 | 0x220 */
void fn_801A4B00(void) {
    extern u8 lbl_804655E0[];
    extern u8 lbl_8047B2B4[];
    extern void fn_801A3E64();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp != 0) {
        while (1) {
            if (r31 == 0) break;
            if (r31 != 0) {
                r30 = (u32)lbl_8047B2B4;
                while (1) {
                    tmp = *(u32*)((u8*)r30 + 0x0);
                if (tmp == 0) break;
                    r3 = *(u32*)((u8*)r30 + 0x0);
                    tmp = *(u32*)((u8*)r3 + 0x4);
                    if (tmp == r31) {
                        r6 = 0x0;
                        while ((s32)r6 < 9) {

                            r3 = (u32)lbl_804655E0;
                            r4 = r6 << 2;
                            tmp = (u32)lbl_804655E0;
                            r3 = tmp + r4;
                            tmp = *(u32*)((u8*)r3 + 0x0);
                            if (r31 == tmp) {
                                r3 = (u32)lbl_804655E0;
                                r4 = r6 << 2;
                                tmp = (u32)lbl_804655E0;
                                r5 = 0x0;
                                r3 = tmp + r4;
                                *(u32*)((u8*)r3 + 0x0) = r5;
                            }
                            r6 = r6 + 0x1;

                        }
                        r3 = *(u32*)((u8*)r30 + 0x0);
                        fn_801A3E64();
                        *(u32*)((u8*)r30 + 0x0) = r3;
                        if (r31 == 0) break;
                        r3 = 0x10000;
                        r4 = *(u16*)((u8*)r31 + 0x4);
                        tmp = r3 & 0xFFFF;
                        tmp = tmp - r4;
                        tmp = __cntlzw(tmp);
                        /* srwi. r3, tmp, 5 */;
                        if (r31 != 0) {
                        } else {

                            tmp = *(u16*)((u8*)r31 + 0x4);
                            r3 = *(u16*)((u8*)r31 + 0x4);
                            r4 = __cntlzw(tmp);
                            *(u16*)((u8*)r31 + 0x4) = tmp;
                            r3 = (u32)r4 >> 5;
                        }
                        if ((s32)r3 == 0 || r31 == 0) break;

                        r4 = *(u32*)((u8*)r31 + 0x0);
                        r3 = r31;
                        r12 = *(u32*)((u8*)r4 + 0x30);
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        r4 = *(u32*)((u8*)r31 + 0x0);
                        r3 = r31;
                        r12 = *(u32*)((u8*)r4 + 0x34);
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        break;
                    }
                    r30 = *(u32*)((u8*)r30 + 0x0);


                }
            }
            r31 = *(u32*)((u8*)r31 + 0xC);

        }
        return;
    }
    r6 = 0x0;
    while ((s32)r6 < 9) {

        r3 = (u32)lbl_804655E0;
        r4 = r6 << 2;
        tmp = (u32)lbl_804655E0;
        r5 = 0x0;
        r3 = tmp + r4;
        r6 = r6 + 0x1;
        *(u32*)((u8*)r3 + 0x0) = r5;

    }
    tmp = 0x0;
    *(u32*)&lbl_8047B2B8 = tmp;
    while (1) {
        tmp = *(u32*)lbl_8047B2B4;
        if (tmp == 0) break;
        r3 = *(u32*)lbl_8047B2B4;
        r30 = *(u32*)((u8*)r3 + 0x4);
        if (r30 != 0) {
            r3 = 0x10000;
            r4 = *(u16*)((u8*)r30 + 0x4);
            tmp = r3 & 0xFFFF;
            tmp = tmp - r4;
            tmp = __cntlzw(tmp);
            /* srwi. r3, tmp, 5 */;
            if (r30 != 0) {
            } else {

                tmp = *(u16*)((u8*)r30 + 0x4);
                r3 = *(u16*)((u8*)r30 + 0x4);
                r4 = __cntlzw(tmp);
                *(u16*)((u8*)r30 + 0x4) = tmp;
                r3 = (u32)r4 >> 5;
            }
            if ((s32)r3 != 0) {
                if (r30 != 0) {
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
        }
        }
        }
        r3 = *(u32*)lbl_8047B2B4;
        fn_801A3E64();
        *(u32*)lbl_8047B2B4 = r3;


    }

    return;
}

/* 0x801A4D20 | 0x234 */
void fn_801A4D20(void) {
    extern u8 lbl_80274DD8[];
    extern u8 lbl_80274DE4[];
    extern u8 lbl_804655E0[];
    extern u8 lbl_8047B2B4[];
    extern u8 lbl_8047DBCC[];
    extern u8 lbl_8047DBD8[];
    extern void fn_80196E10();
    extern void fn_801A3E64();
    extern void fn_801A3EB4();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    while (1) {
        if (r31 == 0) break;
        if (r31 != 0) {
            r3 = *(u32*)lbl_8047B2B4;
            while (1) {
                if (r3 == 0) break;
                tmp = *(u32*)((u8*)r3 + 0x4);
                if (tmp == r31) {
                    if (r31 == 0) break;
                    r30 = (u32)lbl_8047B2B4;
                    while (1) {
                        tmp = *(u32*)((u8*)r30 + 0x0);
                        if (tmp == 0) break;
                        r3 = *(u32*)((u8*)r30 + 0x0);
                        tmp = *(u32*)((u8*)r3 + 0x4);
                        if (tmp == r31) {
                            r6 = 0x0;
                            while ((s32)r6 < 9) {

                                r3 = (u32)lbl_804655E0;
                                r4 = r6 << 2;
                                tmp = (u32)lbl_804655E0;
                                r3 = tmp + r4;
                                tmp = *(u32*)((u8*)r3 + 0x0);
                                if (r31 == tmp) {
                                    r3 = (u32)lbl_804655E0;
                                    r4 = r6 << 2;
                                    tmp = (u32)lbl_804655E0;
                                    r5 = 0x0;
                                    r3 = tmp + r4;
                                    *(u32*)((u8*)r3 + 0x0) = r5;
                                }
                                r6 = r6 + 0x1;

                            }
                            r3 = *(u32*)((u8*)r30 + 0x0);
                            fn_801A3E64();
                            *(u32*)((u8*)r30 + 0x0) = r3;
                            if (r31 == 0) break;
                            r3 = 0x10000;
                            r4 = *(u16*)((u8*)r31 + 0x4);
                            tmp = r3 & 0xFFFF;
                            tmp = tmp - r4;
                            tmp = __cntlzw(tmp);
                            /* srwi. r3, tmp, 5 */;
                            if (r31 != 0) {
                            } else {

                                tmp = *(u16*)((u8*)r31 + 0x4);
                                r3 = *(u16*)((u8*)r31 + 0x4);
                                r4 = __cntlzw(tmp);
                                *(u16*)((u8*)r31 + 0x4) = tmp;
                                r3 = (u32)r4 >> 5;
                            }
                            if ((s32)r3 == 0 || r31 == 0) break;

                            r4 = *(u32*)((u8*)r31 + 0x0);
                            r3 = r31;
                            r12 = *(u32*)((u8*)r4 + 0x30);
                            ctr_fn = (void(*)(void))r12;
                            ctr_fn();
                            r4 = *(u32*)((u8*)r31 + 0x0);
                            r3 = r31;
                            r12 = *(u32*)((u8*)r4 + 0x34);
                            ctr_fn = (void(*)(void))r12;
                            ctr_fn();
                            break;
                        }
                        r30 = *(u32*)((u8*)r30 + 0x0);


                    }
                    break;
                }
                r3 = *(u32*)((u8*)r3 + 0x0);

            }

            if (r31 != 0) {
                r3 = *(u16*)((u8*)r31 + 0x4);
                tmp = r3 + 0x1;
                *(u16*)((u8*)r31 + 0x4) = tmp;
                tmp = *(u16*)((u8*)r31 + 0x4);
                if (tmp == 0xffff) {
                    r3 = (u32)lbl_80274DD8;
                    r5 = (u32)lbl_80274DE4;
                    r3 = (u32)lbl_80274DD8;
                    r4 = 0x5d;
                    r5 = (u32)lbl_80274DE4;
                    fn_80196E10();
            }
            }
            r30 = (u32)lbl_8047B2B4;
            while (1) {
                tmp = *(u32*)((u8*)r30 + 0x0);
                if (tmp == 0) break;
                if (r31 == 0) {
                    r3 = (u32)lbl_8047DBD8;
                    r4 = 0x163;
                    r5 = (u32)lbl_8047DBCC;
                    fn_80196E10();
                }
                r3 = *(u32*)((u8*)r30 + 0x0);
                tmp = *(u16*)((u8*)r31 + 0xA);
                r29 = *(u32*)((u8*)r3 + 0x4);
                r28 = tmp & 0xFF;
                if (r29 == 0) {
                    r3 = (u32)lbl_8047DBD8;
                    r4 = 0x163;
                    r5 = (u32)lbl_8047DBCC;
                    fn_80196E10();
                }
                r3 = *(u16*)((u8*)r29 + 0xA);
                tmp = r28 & 0xFF;
                r3 = r3 & 0xFF;
                if (r3 > tmp) break;
                r30 = *(u32*)((u8*)r30 + 0x0);


            }

            r3 = *(u32*)((u8*)r30 + 0x0);
            r4 = r31;
            fn_801A3EB4();
            *(u32*)((u8*)r30 + 0x0) = r3;
        }
        r31 = *(u32*)((u8*)r31 + 0xC);

    }
    return;
}

/* 0x801A4F54 | 0xE78 */
void fn_801A4F54(void) {
    extern u8 lbl_80274D58[];
    extern u8 lbl_80274D64[];
    extern u8 lbl_804655E0[];
    extern u8 lbl_80478AC8[];
    extern u8 lbl_8047B2B4[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBD4[];
    extern u8 lbl_8047DBE0[];
    extern u8 lbl_8047DBE4[];
    extern u8 lbl_8047DBE8[];
    extern u8 lbl_8047DBEC[];
    extern u8 lbl_8047DBF0[];
    extern u8 lbl_8047DBF8[];
    extern u8 lbl_8047DBFC[];
    extern void fn_800A37CC();
    extern void fn_800A3820();
    extern void fn_800A3A9C();
    extern void fn_800A3ADC();
    extern void fn_800BA198();
    extern void fn_800BA1B4();
    extern void fn_800BA344();
    extern void fn_800BA414();
    extern void fn_800BA424();
    extern void fn_800BA440();
    extern void fn_800BA44C();
    extern void fn_80191688();
    extern void fn_80196E10();
    extern u8 jumptable_8036CA88[];
    extern u8 jumptable_8036CAAC[];
    extern u8 jumptable_8036CAD0[];
    u8 sp[0xF0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r6 = 0x0;
    r5 = 0x0;
    r4 = 0x0;
    tmp = 0x0;
    r3 = r3 + 0x54;
    *(u32*)&lbl_8047B2BC = r6;
    r31 = r3;
    r30 = 0x0;
    *(u32*)&lbl_8047B2C0 = r5;
    r6 = 0x0;
    *(u32*)&lbl_8047B2C4 = r4;
    *(u32*)&lbl_8047B2C8 = tmp;
    while ((s32)r6 < 9) {

        r3 = (u32)lbl_804655E0;
        r4 = r6 << 2;
        tmp = (u32)lbl_804655E0;
        r5 = 0x0;
        r3 = tmp + r4;
        r6 = r6 + 0x1;
        *(u32*)((u8*)r3 + 0x0) = r5;

    }
    tmp = 0x0;
    r27 = *(u32*)lbl_8047B2B4;
    *(u32*)&lbl_8047B2B8 = tmp;
    while (1) {
        if ((s32)r30 < 8) {
        if (r27 == 0) break;
        r28 = *(u32*)((u8*)r27 + 0x4);
        if (r28 == 0) goto L_801A57CC;
        tmp = *(u16*)((u8*)r28 + 0x8);
        tmp = tmp & 0x00000020;
        if (r28 != 0) goto L_801A57CC;
        tmp = *(u16*)((u8*)r28 + 0x8);
        r3 = *(u16*)((u8*)r28 + 0x8);
        r29 = tmp & 0x0000001C;
        tmp = r3 & 0x3;
        if (r28 == 0) goto L_801A57CC;
        r3 = *(u16*)((u8*)r28 + 0x8);
        r3 = r3 & 0x3;
        if (r28 == 0) {
            r5 = 0x8;
            r3 = (u32)lbl_804655E0;
            r4 = r5 << 2;
            r3 = (u32)lbl_804655E0;
            r3 = r3 + r4;
            r3 = *(u32*)((u8*)r3 + 0x0);
            if (r3 != 0) {
                goto L_801A50D0;
            }
            r5 = *(u32*)&lbl_8047B2B8;
            r3 = r5 + 0x1;
            *(u32*)&lbl_8047B2B8 = r3;
            }
        r3 = (u32)lbl_804655E0;
        r4 = r5 << 2;
        r3 = (u32)lbl_804655E0;
        r3 = r3 + r4;
        *(u32*)((u8*)r3 + 0x0) = r28;
        if (r5 <= 8) {
            r4 = (u32)jumptable_8036CAD0;
            r3 = r5 << 2;
            r4 = (u32)jumptable_8036CAD0;
            r4 = *(u32*)(r4 + r3);
            ctr_fn = (void(*)(void))r4;
            r3 = 0x1;
            goto L_801A50CC;
            r3 = 0x2;
            goto L_801A50CC;
            r3 = 0x4;
            goto L_801A50CC;
            r3 = 0x8;
            goto L_801A50CC;
            r3 = 0x10;
            goto L_801A50CC;
            r3 = 0x20;
            goto L_801A50CC;
            r3 = 0x40;
            goto L_801A50CC;
            r3 = 0x80;
            goto L_801A50CC;
            r3 = 0x100;
            goto L_801A50CC;
        }
        r3 = 0x0;
    L_801A50CC:
        *(u32*)((u8*)r28 + 0x4C) = r3;
    L_801A50D0:
        r30 = *(u32*)&lbl_8047B2B8;
        r3 = 0x0;
        *(u32*)((u8*)r28 + 0x90) = r3;
        if ((s32)tmp != 2) {
            if ((s32)tmp < 2) {
                if ((s32)tmp == 0) goto L_801A57CC;
                if ((s32)tmp < 0) {
                    goto L_801A54E8;
                }
                if ((s32)tmp >= 4) goto L_801A54E8;
                goto L_801A52B4;
                }
            if (r28 != 0) {
                tmp = *(u32*)((u8*)r28 + 0x18);
                if (tmp != 0) {
                    r3 = *(u32*)((u8*)r28 + 0x18);
                    r4 = (u32)sp + 0x8c;
                    fn_80191688();
            }
            }
            f1 = *(f32*)(sp + 0x8C);
            r3 = r31;
            f0 = *(f32*)lbl_8047DBE0;
            r4 = (u32)sp + 0x8c;
            f3 = *(f32*)(sp + 0x90);
            r5 = (u32)sp + 0x8c;
            f2 = *(f32*)lbl_8047DBE0;
            f4 = f1 * f0;
            f1 = *(f32*)(sp + 0x94);
            f0 = *(f32*)lbl_8047DBE0;
            f2 = f3 * f2;
            *(f32*)(sp + 0x8C) = f4;
            f0 = f1 * f0;
            *(f32*)(sp + 0x90) = f2;
            *(f32*)(sp + 0x94) = f0;
            fn_800A37CC();
            tmp = *(u16*)((u8*)r28 + 0x8);
            tmp = tmp & 0x00000004;
            if (tmp != 0) {
                f1 = *(f32*)(sp + 0x8C);
                r3 = r28 + 0x50;
                f2 = *(f32*)(sp + 0x90);
                f3 = *(f32*)(sp + 0x94);
                fn_800BA414();
                f1 = *(f32*)lbl_8047DBE4;
                r3 = r28 + 0x50;
                f2 = *(f32*)lbl_8047DBE8;
                f3 = *(f32*)lbl_8047DBE8;
                f4 = *(f32*)lbl_8047DBE4;
                f5 = *(f32*)lbl_8047DBE8;
                f6 = *(f32*)lbl_8047DBE8;
                fn_800BA198();
            }
            tmp = *(u16*)((u8*)r28 + 0x8);
            tmp = tmp & 0x00000008;
            if (tmp == 0) goto L_801A54E8;
            f1 = *(f32*)(sp + 0x8C);
            r3 = r28 + 0x94;
            f2 = *(f32*)(sp + 0x90);
            f3 = *(f32*)(sp + 0x94);
            fn_800BA414();
            goto L_801A54E8;
        }
        tmp = *(u32*)((u8*)r28 + 0x10);
        r4 = (u32)sp + 0x18;
        r3 = r28 + 0x50;
        *(u32*)(sp + 0x18) = tmp;
        fn_800BA440();
        tmp = *(u32*)((u8*)r28 + 0x10);
        *(u32*)((u8*)r28 + 0x14) = tmp;
        if (r28 != 0) {
            tmp = *(u32*)((u8*)r28 + 0x18);
            if (tmp != 0) {
                r3 = *(u32*)((u8*)r28 + 0x18);
                r4 = (u32)sp + 0x80;
                fn_80191688();
        }
        }
        r3 = r31;
        r4 = (u32)sp + 0x80;
        r5 = (u32)sp + 0x80;
        fn_800A37CC();
        f1 = *(f32*)(sp + 0x80);
        r3 = r28 + 0x50;
        f2 = *(f32*)(sp + 0x84);
        f3 = *(f32*)(sp + 0x88);
        fn_800BA414();
        f1 = *(f32*)(sp + 0x80);
        r3 = r28 + 0x94;
        f2 = *(f32*)(sp + 0x84);
        f3 = *(f32*)(sp + 0x88);
        fn_800BA414();
        tmp = *(u16*)((u8*)r28 + 0x8);
        tmp = tmp & 0x00000040;
        if (tmp != 0) {
            f1 = *(f32*)lbl_8047DBE4;
            r3 = r28 + 0x50;
            f2 = *(f32*)lbl_8047DBE8;
            f3 = *(f32*)lbl_8047DBE8;
            f4 = *(f32*)((u8*)r28 + 0x2C);
            f5 = *(f32*)((u8*)r28 + 0x30);
            f6 = *(f32*)((u8*)r28 + 0x34);
            fn_800BA198();
            goto L_801A54E8;
        }
        f30 = *(f32*)((u8*)r28 + 0x28);
        r3 = r28 + 0x50;
        f31 = *(f32*)((u8*)r28 + 0x2C);
        r26 = *(u32*)((u8*)r28 + 0x30);
        f2 = f30;
        f1 = f31;
        r4 = r26;
        fn_800BA344();
        f1 = *(f32*)lbl_8047DBE8;
        r3 = r28 + 0x50;
        r4 = 0x0;
        fn_800BA1B4();
        f1 = f31;
        r4 = r26;
        f2 = f30;
        r3 = r28 + 0x94;
        fn_800BA344();
        goto L_801A54E8;
    L_801A52B4:
        if (r28 != 0) {
            tmp = *(u32*)((u8*)r28 + 0x18);
            if (tmp != 0) {
                r3 = *(u32*)((u8*)r28 + 0x18);
                r4 = (u32)sp + 0x68;
                fn_80191688();
        }
        }
        r3 = r31;
        r4 = (u32)sp + 0x68;
        r5 = (u32)sp + 0x68;
        fn_800A37CC();
        r4 = (u32)lbl_80274D58;
        r8 = *(u32*)lbl_80274D58;
        r3 = (u32)lbl_80274D64;
        r5 = (u32)lbl_80274D64;
        r7 = *(u32*)((u8*)r4 + 0x4);
        r6 = *(u32*)((u8*)r4 + 0x8);
        r4 = *(u32*)((u8*)r5 + 0x0);
        r3 = *(u32*)((u8*)r5 + 0x4);
        tmp = *(u32*)((u8*)r5 + 0x8);
        *(u32*)(sp + 0x64) = tmp;
        if (r28 == 0) goto L_801A5404;
        if (r28 != 0) {
            tmp = *(u32*)((u8*)r28 + 0x18);
            if (tmp != 0) {
                r3 = *(u32*)((u8*)r28 + 0x18);
                r4 = (u32)sp + 0x50;
                fn_80191688();
        }
        }
        if (r28 != 0) {
            tmp = *(u32*)((u8*)r28 + 0x1C);
            if (tmp != 0) {
                r3 = *(u32*)((u8*)r28 + 0x1C);
                r4 = (u32)sp + 0x5c;
                fn_80191688();
        }
        }
        r3 = (u32)sp + 0x5c;
        r4 = (u32)sp + 0x50;
        r5 = (u32)sp + 0x74;
        fn_800A3A9C();
        f0 = *(f32*)(sp + 0x74);
        r3 = (u32)lbl_80478AC8;
        /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
        f0 = *(f32*)lbl_80478AC8;
        /* cror eq, lt, eq */;
        if (f1 != f0) goto L_801A53D4;
        f0 = *(f32*)(sp + 0x78);
        r3 = (u32)lbl_80478AC8;
        /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
        f0 = *(f32*)lbl_80478AC8;
        /* cror eq, lt, eq */;
        if (f1 != f0) goto L_801A53D4;
        f0 = *(f32*)(sp + 0x7C);
        r3 = (u32)lbl_80478AC8;
        /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
        f0 = *(f32*)lbl_80478AC8;
        /* cror eq, lt, eq */;
        if (f1 != f0) goto L_801A53D4;
        tmp = -0x1;
        goto L_801A53E4;
    L_801A53D4:
        r3 = (u32)sp + 0x74;
        r4 = (u32)sp + 0x74;
        fn_800A3ADC();
        tmp = 0x0;
    L_801A53E4:
        if ((s32)tmp == 0) goto L_801A5404;
        f2 = *(f32*)lbl_8047DBE8;
        f1 = *(f32*)lbl_8047DBE8;
        f0 = *(f32*)lbl_8047DBE4;
        *(f32*)(sp + 0x74) = f2;
        *(f32*)(sp + 0x78) = f1;
        *(f32*)(sp + 0x7C) = f0;
    L_801A5404:
        r3 = r31;
        r4 = (u32)sp + 0x74;
        r5 = (u32)sp + 0x74;
        fn_800A3820();
        r3 = (u32)sp + 0x74;
        r4 = (u32)sp + 0x74;
        fn_800A3ADC();
        f1 = *(f32*)(sp + 0x68);
        r3 = r28 + 0x50;
        f2 = *(f32*)(sp + 0x6C);
        f3 = *(f32*)(sp + 0x70);
        fn_800BA414();
        f1 = *(f32*)(sp + 0x68);
        r3 = r28 + 0x94;
        f2 = *(f32*)(sp + 0x6C);
        f3 = *(f32*)(sp + 0x70);
        fn_800BA414();
        f1 = *(f32*)(sp + 0x74);
        r3 = r28 + 0x50;
        f2 = *(f32*)(sp + 0x78);
        f3 = *(f32*)(sp + 0x7C);
        fn_800BA424();
        tmp = *(u16*)((u8*)r28 + 0x8);
        tmp = tmp & 0x00000040;
        if ((s32)tmp != 0) {
            f1 = *(f32*)((u8*)r28 + 0x20);
            r3 = r28 + 0x50;
            f2 = *(f32*)((u8*)r28 + 0x24);
            f3 = *(f32*)((u8*)r28 + 0x28);
            f4 = *(f32*)((u8*)r28 + 0x2C);
            f5 = *(f32*)((u8*)r28 + 0x30);
            f6 = *(f32*)((u8*)r28 + 0x34);
            fn_800BA198();
            goto L_801A54E8;
        }
        f31 = *(f32*)((u8*)r28 + 0x2C);
        f0 = *(f64*)lbl_8047DBF0;
        f29 = *(f32*)((u8*)r28 + 0x28);
        f30 = *(f32*)((u8*)r28 + 0x20);
        r25 = *(u32*)((u8*)r28 + 0x24);
        r26 = *(u32*)((u8*)r28 + 0x30);
        if (f31 < f0) {
            f31 = *(f32*)lbl_8047DBEC;
        }
        f1 = f31;
        r4 = r26;
        f2 = f29;
        r3 = r28 + 0x50;
        fn_800BA344();
        f1 = f31;
        r4 = r26;
        f2 = f29;
        r3 = r28 + 0x94;
        fn_800BA344();
        f1 = f30;
        r4 = r25;
        r3 = r28 + 0x50;
        fn_800BA1B4();
    L_801A54E8:
        tmp = r29 & 0x14;
        if (f31 != f0) {
            tmp = *(u32*)((u8*)r28 + 0x10);
            r4 = (u32)sp + 0x14;
            r3 = r28 + 0x50;
            *(u32*)(sp + 0x14) = tmp;
            fn_800BA440();
            tmp = *(u32*)((u8*)r28 + 0x10);
            *(u32*)((u8*)r28 + 0x14) = tmp;
            tmp = *(u16*)((u8*)r28 + 0x8);
            tmp = tmp | 0x80;
            *(u16*)((u8*)r28 + 0x8) = tmp;
            tmp = *(u16*)((u8*)r28 + 0x8);
            tmp = tmp & 0x3;
            if ((s32)tmp == 1) goto L_801A5558;
            if ((s32)tmp >= 1 && (s32)tmp < 4) {

                r3 = *(u32*)&lbl_8047B2C4;
                tmp = *(u32*)((u8*)r28 + 0x4C);
                tmp = r3 | tmp;
                *(u32*)&lbl_8047B2C4 = tmp;
                goto L_801A5558;
            }
            r3 = (u32)lbl_8047DBB8;
            r4 = 0x298;
            r5 = (u32)lbl_8047DBD4;
            fn_80196E10();
        L_801A5558:
            tmp = *(u16*)((u8*)r28 + 0x8);
            tmp = tmp & 0x00000004;
            if ((s32)tmp != 4) {
                r3 = *(u32*)&lbl_8047B2BC;
                tmp = *(u32*)((u8*)r28 + 0x4C);
                tmp = r3 | tmp;
                *(u32*)&lbl_8047B2BC = tmp;
            }
            tmp = *(u16*)((u8*)r28 + 0x8);
            tmp = tmp & 0x00000010;
            if ((s32)tmp == 4) goto L_801A57CC;
            r3 = *(u32*)&lbl_8047B2C8;
            tmp = *(u32*)((u8*)r28 + 0x4C);
            tmp = r3 | tmp;
            *(u32*)&lbl_8047B2C8 = tmp;
            goto L_801A57CC;
        }
        tmp = r29 & 0x00000008;
        if ((s32)tmp == 4) goto L_801A57CC;
        r29 = *(u32*)((u8*)r28 + 0x4C);
        *(u32*)((u8*)r28 + 0x90) = r29;
        if ((s32)r29 == 0) goto L_801A57CC;
        tmp = *(u32*)((u8*)r28 + 0x10);
        r4 = (u32)sp + 0x10;
        r3 = r28 + 0x94;
        *(u32*)(sp + 0x10) = tmp;
        fn_800BA440();
        f0 = *(f32*)lbl_8047DBF8;
        r3 = r28 + 0x94;
        f1 = *(f32*)lbl_8047DBFC;
        *(f32*)((u8*)r28 + 0x38) = f0;
        f2 = *(f32*)lbl_8047DBFC;
        f4 = *(f32*)((u8*)r28 + 0x38);
        f3 = *(f32*)((u8*)r28 + 0x38);
        f0 = *(f32*)lbl_8047DBE4;
        f4 = f4 * f1;
        f1 = *(f32*)lbl_8047DBE8;
        f6 = -(f3 * f2 - f0);
        f2 = *(f32*)lbl_8047DBE8;
        f3 = *(f32*)lbl_8047DBE4;
        f5 = *(f32*)lbl_8047DBE8;
        fn_800BA198();
        tmp = *(u16*)((u8*)r28 + 0x8);
        tmp = tmp & 0x3;
        if ((s32)tmp != 1) {
            if ((s32)tmp >= 1 && (s32)tmp < 4) {

                if (r28 != 0) {
                    tmp = *(u32*)((u8*)r28 + 0x18);
                    if (tmp != 0) {
                        r3 = *(u32*)((u8*)r28 + 0x18);
                        r4 = r28 + 0x3c;
                        fn_80191688();
                }
                }
                r3 = r31;
                r4 = r28 + 0x3c;
                r5 = r28 + 0x3c;
                fn_800A37CC();
                goto L_801A57B4;
            }
            r4 = (u32)lbl_80274D58;
            r8 = *(u32*)lbl_80274D58;
            r3 = (u32)lbl_80274D64;
            r5 = (u32)lbl_80274D64;
            r7 = *(u32*)((u8*)r4 + 0x4);
            r6 = *(u32*)((u8*)r4 + 0x8);
            r4 = *(u32*)((u8*)r5 + 0x0);
            r3 = *(u32*)((u8*)r5 + 0x4);
            tmp = *(u32*)((u8*)r5 + 0x8);
            *(u32*)(sp + 0x4C) = tmp;
            if (r28 == 0) goto L_801A5784;
            if (r28 != 0) {
                tmp = *(u32*)((u8*)r28 + 0x18);
                if (tmp != 0) {
                    r3 = *(u32*)((u8*)r28 + 0x18);
                    r4 = (u32)sp + 0x38;
                    fn_80191688();
            }
            }
            if (r28 != 0) {
                tmp = *(u32*)((u8*)r28 + 0x1C);
                if (tmp != 0) {
                    r3 = *(u32*)((u8*)r28 + 0x1C);
                    r4 = (u32)sp + 0x44;
                    fn_80191688();
            }
            }
            r3 = (u32)sp + 0x44;
            r4 = (u32)sp + 0x38;
            r5 = r28 + 0x3c;
            fn_800A3A9C();
            /* addic. tmp, r28, 0x3c */;
            if (tmp != 0) {
                /* addic. tmp, r28, 0x3c */;
                if (tmp == 0) {
                }
                tmp = -0x1;
                goto L_801A5764;
                }
            f0 = *(f32*)((u8*)r28 + 0x3C);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A5754;
            f0 = *(f32*)((u8*)r28 + 0x40);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A5754;
            f0 = *(f32*)((u8*)r28 + 0x44);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A5754;
            tmp = -0x1;
            goto L_801A5764;
        L_801A5754:
            r3 = r28 + 0x3c;
            r4 = r28 + 0x3c;
            fn_800A3ADC();
            tmp = 0x0;
        L_801A5764:
            if ((s32)tmp == 0) goto L_801A5784;
            f0 = *(f32*)lbl_8047DBE8;
            f1 = *(f32*)lbl_8047DBE8;
            *(f32*)((u8*)r28 + 0x3C) = f0;
            f0 = *(f32*)lbl_8047DBE4;
            *(f32*)((u8*)r28 + 0x40) = f1;
            *(f32*)((u8*)r28 + 0x44) = f0;
        L_801A5784:
            r3 = r31;
            r4 = r28 + 0x3c;
            r5 = r28 + 0x3c;
            fn_800A3820();
            r3 = r28 + 0x3c;
            r4 = r28 + 0x3c;
            fn_800A3ADC();
            goto L_801A57B4;
            }
        r3 = (u32)lbl_8047DBB8;
        r4 = 0x2c0;
        r5 = (u32)lbl_8047DBD4;
        fn_80196E10();
    L_801A57B4:
        tmp = *(u16*)((u8*)r28 + 0x8);
        tmp = tmp | 0x100;
        *(u16*)((u8*)r28 + 0x8) = tmp;
        tmp = *(u32*)&lbl_8047B2C0;
        tmp = tmp | r29;
        *(u32*)&lbl_8047B2C0 = tmp;
    L_801A57CC:
        r27 = *(u32*)((u8*)r27 + 0x0);


    }
    }
    tmp = 0x8;
    if ((s32)tmp >= 0 || (s32)tmp >= 9) {

        r3 = (u32)lbl_804655E0;
        r4 = tmp << 2;
        tmp = (u32)lbl_804655E0;
        r3 = tmp + r4;
        tmp = *(u32*)((u8*)r3 + 0x0);
    } else {

        tmp = 0x0;
    }
    if (tmp == 0) {
        while (1) {
            if (r27 == 0) break;
            r3 = *(u32*)((u8*)r27 + 0x4);
            if (r3 == 0) goto L_801A5910;
            tmp = *(u16*)((u8*)r3 + 0x8);
            tmp = tmp & 0x00000020;
            if (r3 != 0) goto L_801A5910;
            tmp = *(u16*)((u8*)r3 + 0x8);
            tmp = tmp & 0x3;
            if (r3 != 0) goto L_801A5910;
            tmp = *(u16*)((u8*)r3 + 0x8);
            tmp = tmp & 0x14;
            if (r3 == 0) goto L_801A5910;
            tmp = *(u16*)((u8*)r3 + 0x8);
            tmp = tmp & 0x3;
            if (r3 == 0) {
                r6 = 0x8;
                r4 = (u32)lbl_804655E0;
                r5 = r6 << 2;
                tmp = (u32)lbl_804655E0;
                r4 = tmp + r5;
                tmp = *(u32*)((u8*)r4 + 0x0);
                if (tmp != 0) {
                    break;
                }
                r6 = *(u32*)&lbl_8047B2B8;
                tmp = r6 + 0x1;
                *(u32*)&lbl_8047B2B8 = tmp;
                }
            r4 = (u32)lbl_804655E0;
            r5 = r6 << 2;
            tmp = (u32)lbl_804655E0;
            r4 = tmp + r5;
            *(u32*)((u8*)r4 + 0x0) = r3;
            if (r6 <= 8) {
                r4 = (u32)jumptable_8036CAAC;
                tmp = r6 << 2;
                r4 = (u32)jumptable_8036CAAC;
                r4 = *(u32*)(r4 + tmp);
                ctr_fn = (void(*)(void))r4;
                tmp = 0x1;
                goto L_801A5908;
                tmp = 0x2;
                goto L_801A5908;
                tmp = 0x4;
                goto L_801A5908;
                tmp = 0x8;
                goto L_801A5908;
                tmp = 0x10;
                goto L_801A5908;
                tmp = 0x20;
                goto L_801A5908;
                tmp = 0x40;
                goto L_801A5908;
                tmp = 0x80;
                goto L_801A5908;
                tmp = 0x100;
                goto L_801A5908;
            }
            tmp = 0x0;
        L_801A5908:
            *(u32*)((u8*)r3 + 0x4C) = tmp;
            break;
        L_801A5910:
            r27 = *(u32*)((u8*)r27 + 0x0);

        }
    }
    r27 = *(u32*)&lbl_8047B2B8;
    r28 = 0x0;
    while (1) {
        if ((s32)r30 < 8) {
        if ((s32)r28 >= (s32)r27) break;
        if ((s32)r28 >= 0 || (s32)r28 >= 8) {

            r3 = (u32)lbl_804655E0;
            r4 = r28 << 2;
            tmp = (u32)lbl_804655E0;
            r3 = tmp + r4;
            tmp = *(u32*)((u8*)r3 + 0x0);
        } else {

            tmp = 0x0;
        }
        /* mr. r26, tmp */;
        if ((s32)r28 == 8) goto L_801A5C10;
        r3 = *(u16*)((u8*)r26 + 0x8);
        tmp = r3 & 0x00000008;
        if ((s32)r28 == 8) goto L_801A5C10;
        tmp = r3 & 0x14;
        if ((s32)r28 == 8) goto L_801A5C10;
        tmp = r30;
        r30 = r30 + 0x1;
        if (tmp <= 8) {
            r3 = (u32)jumptable_8036CA88;
            tmp = tmp << 2;
            r3 = (u32)jumptable_8036CA88;
            r3 = *(u32*)(r3 + tmp);
            ctr_fn = (void(*)(void))r3;
            r29 = 0x1;
            goto L_801A59E4;
            r29 = 0x2;
            goto L_801A59E4;
            r29 = 0x4;
            goto L_801A59E4;
            r29 = 0x8;
            goto L_801A59E4;
            r29 = 0x10;
            goto L_801A59E4;
            r29 = 0x20;
            goto L_801A59E4;
            r29 = 0x40;
            goto L_801A59E4;
            r29 = 0x80;
            goto L_801A59E4;
            r29 = 0x100;
            goto L_801A59E4;
        }
        r29 = 0x0;
    L_801A59E4:
        *(u32*)((u8*)r26 + 0x90) = r29;
        if ((s32)r29 == 0) goto L_801A5C10;
        tmp = *(u32*)((u8*)r26 + 0x10);
        r4 = (u32)sp + 0xc;
        r3 = r26 + 0x94;
        *(u32*)(sp + 0xC) = tmp;
        fn_800BA440();
        f0 = *(f32*)lbl_8047DBF8;
        r3 = r26 + 0x94;
        f1 = *(f32*)lbl_8047DBFC;
        *(f32*)((u8*)r26 + 0x38) = f0;
        f2 = *(f32*)lbl_8047DBFC;
        f4 = *(f32*)((u8*)r26 + 0x38);
        f3 = *(f32*)((u8*)r26 + 0x38);
        f0 = *(f32*)lbl_8047DBE4;
        f4 = f4 * f1;
        f1 = *(f32*)lbl_8047DBE8;
        f6 = -(f3 * f2 - f0);
        f2 = *(f32*)lbl_8047DBE8;
        f3 = *(f32*)lbl_8047DBE4;
        f5 = *(f32*)lbl_8047DBE8;
        fn_800BA198();
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp & 0x3;
        if ((s32)tmp != 1) {
            if ((s32)tmp >= 1 && (s32)tmp < 4) {

                if (r26 != 0) {
                    tmp = *(u32*)((u8*)r26 + 0x18);
                    if (tmp != 0) {
                        r3 = *(u32*)((u8*)r26 + 0x18);
                        r4 = r26 + 0x3c;
                        fn_80191688();
                }
                }
                r3 = r31;
                r4 = r26 + 0x3c;
                r5 = r26 + 0x3c;
                fn_800A37CC();
                goto L_801A5BF8;
            }
            r4 = (u32)lbl_80274D58;
            r8 = *(u32*)lbl_80274D58;
            r3 = (u32)lbl_80274D64;
            r5 = (u32)lbl_80274D64;
            r7 = *(u32*)((u8*)r4 + 0x4);
            r6 = *(u32*)((u8*)r4 + 0x8);
            r4 = *(u32*)((u8*)r5 + 0x0);
            r3 = *(u32*)((u8*)r5 + 0x4);
            tmp = *(u32*)((u8*)r5 + 0x8);
            *(u32*)(sp + 0x34) = tmp;
            if (r26 == 0) goto L_801A5BC8;
            if (r26 != 0) {
                tmp = *(u32*)((u8*)r26 + 0x18);
                if (tmp != 0) {
                    r3 = *(u32*)((u8*)r26 + 0x18);
                    r4 = (u32)sp + 0x20;
                    fn_80191688();
            }
            }
            if (r26 != 0) {
                tmp = *(u32*)((u8*)r26 + 0x1C);
                if (tmp != 0) {
                    r3 = *(u32*)((u8*)r26 + 0x1C);
                    r4 = (u32)sp + 0x2c;
                    fn_80191688();
            }
            }
            r3 = (u32)sp + 0x2c;
            r4 = (u32)sp + 0x20;
            r5 = r26 + 0x3c;
            fn_800A3A9C();
            /* addic. tmp, r26, 0x3c */;
            if (tmp != 0) {
                /* addic. tmp, r26, 0x3c */;
                if (tmp == 0) {
                }
                tmp = -0x1;
                goto L_801A5BA8;
                }
            f0 = *(f32*)((u8*)r26 + 0x3C);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A5B98;
            f0 = *(f32*)((u8*)r26 + 0x40);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A5B98;
            f0 = *(f32*)((u8*)r26 + 0x44);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A5B98;
            tmp = -0x1;
            goto L_801A5BA8;
        L_801A5B98:
            r3 = r26 + 0x3c;
            r4 = r26 + 0x3c;
            fn_800A3ADC();
            tmp = 0x0;
        L_801A5BA8:
            if ((s32)tmp == 0) goto L_801A5BC8;
            f0 = *(f32*)lbl_8047DBE8;
            f1 = *(f32*)lbl_8047DBE8;
            *(f32*)((u8*)r26 + 0x3C) = f0;
            f0 = *(f32*)lbl_8047DBE4;
            *(f32*)((u8*)r26 + 0x40) = f1;
            *(f32*)((u8*)r26 + 0x44) = f0;
        L_801A5BC8:
            r3 = r31;
            r4 = r26 + 0x3c;
            r5 = r26 + 0x3c;
            fn_800A3820();
            r3 = r26 + 0x3c;
            r4 = r26 + 0x3c;
            fn_800A3ADC();
            goto L_801A5BF8;
            }
        r3 = (u32)lbl_8047DBB8;
        r4 = 0x2c0;
        r5 = (u32)lbl_8047DBD4;
        fn_80196E10();
    L_801A5BF8:
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp | 0x100;
        *(u16*)((u8*)r26 + 0x8) = tmp;
        tmp = *(u32*)&lbl_8047B2C0;
        tmp = tmp | r29;
        *(u32*)&lbl_8047B2C0 = tmp;
    L_801A5C10:
        r28 = r28 + 0x1;


    }
    }
    r28 = 0x0;
    while (1) {
        if ((s32)r28 >= (s32)r27) break;
        if ((s32)r28 >= 0 || (s32)r28 >= 8) {

            r3 = (u32)lbl_804655E0;
            r4 = r28 << 2;
            tmp = (u32)lbl_804655E0;
            r3 = tmp + r4;
            tmp = *(u32*)((u8*)r3 + 0x0);
        } else {

            tmp = 0x0;
        }
        /* mr. r26, tmp */;
        if ((s32)r28 == 8) goto L_801A5D94;
        tmp = *(u16*)((u8*)r26 + 0x8);
        r3 = *(u32*)((u8*)r26 + 0x10);
        tmp = tmp & 0x00000020;
        f29 = *(f32*)((u8*)r26 + 0x38);
        if ((s32)r28 != 8) goto L_801A5D94;
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp & 0x3;
        if ((s32)r28 == 8) goto L_801A5D94;
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp & 0x14;
        if ((s32)r28 == 8) goto L_801A5D1C;
        r3 = *(u8*)((u8*)r26 + 0x14);
        tmp = *(u8*)(sp + 0x1C);
        if (r3 != tmp) goto L_801A5CD0;
        r3 = *(u8*)((u8*)r26 + 0x15);
        tmp = *(u8*)(sp + 0x1D);
        if (r3 != tmp) goto L_801A5CD0;
        r3 = *(u8*)((u8*)r26 + 0x16);
        tmp = *(u8*)(sp + 0x1E);
        if (r3 != tmp) goto L_801A5CD0;
        r3 = *(u8*)((u8*)r26 + 0x17);
        tmp = *(u8*)(sp + 0x1F);
        if (r3 == tmp) goto L_801A5CF8;
    L_801A5CD0:
        r4 = (u32)sp + 0x8;
        r3 = r26 + 0x50;
        *(u32*)(sp + 0x8) = tmp;
        fn_800BA440();
        *(u32*)((u8*)r26 + 0x14) = tmp;
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp | 0x80;
        *(u16*)((u8*)r26 + 0x8) = tmp;
    L_801A5CF8:
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp & 0x00000080;
        if (r3 == tmp) goto L_801A5D1C;
        r4 = *(u32*)((u8*)r26 + 0x4C);
        r3 = r26 + 0x50;
        fn_800BA44C();
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp & 0xFFFFFF7F;
        *(u16*)((u8*)r26 + 0x8) = tmp;
    L_801A5D1C:
        tmp = *(u32*)((u8*)r26 + 0x90);
        if ((s32)tmp == 0) goto L_801A5D94;
        f0 = *(f32*)((u8*)r26 + 0x38);
        if (f0 != f29) {
            f2 = *(f32*)lbl_8047DBFC;
            r3 = r26 + 0x94;
            f1 = *(f32*)lbl_8047DBFC;
            f0 = *(f32*)lbl_8047DBE4;
            f4 = f29 * f2;
            *(f32*)((u8*)r26 + 0x38) = f29;
            f6 = -(f29 * f1 - f0);
            f1 = *(f32*)lbl_8047DBE8;
            f2 = *(f32*)lbl_8047DBE8;
            f3 = *(f32*)lbl_8047DBE4;
            f5 = *(f32*)lbl_8047DBE8;
            fn_800BA198();
            tmp = *(u16*)((u8*)r26 + 0x8);
            tmp = tmp | 0x100;
            *(u16*)((u8*)r26 + 0x8) = tmp;
        }
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp & 0x00000100;
        if (f0 == f29) goto L_801A5D94;
        r4 = *(u32*)((u8*)r26 + 0x90);
        r3 = r26 + 0x94;
        fn_800BA44C();
        tmp = *(u16*)((u8*)r26 + 0x8);
        tmp = tmp & 0xFFFFFEFF;
        *(u16*)((u8*)r26 + 0x8) = tmp;
    L_801A5D94:
        r28 = r28 + 0x1;

    }
    return;
}

/* 0x801A5DCC | 0x2CC */
void fn_801A5DCC(void) {
    extern u8 lbl_804655E0[];
    extern u8 lbl_80478AC8[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBD4[];
    extern u8 lbl_8047DBE4[];
    extern u8 lbl_8047DBE8[];
    extern u8 lbl_8047DC00[];
    extern void fn_800A3A78();
    extern void fn_800A3A9C();
    extern void fn_800A3ADC();
    extern void fn_800BA424();
    extern void fn_80196E10();
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r4 = (u32)lbl_80478AC8;
    f1 = *(f32*)((u8*)r3 + 0xC);
    f0 = *(f32*)lbl_80478AC8;
    *(f32*)(sp + 0x20) = f1;
    f2 = *(f32*)(sp + 0x20);
    f1 = *(f32*)((u8*)r3 + 0x1C);
    /* fabs */ f2 = (f2 < 0) ? -f2 : f2;
    *(f32*)(sp + 0x24) = f1;
    f1 = *(f32*)((u8*)r3 + 0x2C);
    *(f32*)(sp + 0x28) = f1;
    /* cror eq, lt, eq */;
    if (f2 != f0) goto L_801A5E58;
    f0 = *(f32*)(sp + 0x24);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A5E58;
    f0 = *(f32*)(sp + 0x28);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A5E58;
    tmp = -0x1;
    goto L_801A5E68;
L_801A5E58:
    r3 = (u32)sp + 0x20;
    r4 = (u32)sp + 0x2c;
    fn_800A3ADC();
    tmp = 0x0;
L_801A5E68:
    if ((s32)tmp != 0) {
        f2 = *(f32*)lbl_8047DBE8;
        f1 = *(f32*)lbl_8047DBE8;
        f0 = *(f32*)lbl_8047DC00;
        *(f32*)(sp + 0x2C) = f2;
        *(f32*)(sp + 0x30) = f1;
        *(f32*)(sp + 0x34) = f0;
    }
    r30 = *(u32*)&lbl_8047B2B8;
    r31 = 0x0;
    while (1) {
        if ((s32)r31 >= (s32)r30) break;
        if ((s32)r31 >= 0 || (s32)r31 >= 8) {

            r3 = (u32)lbl_804655E0;
            r4 = r31 << 2;
            tmp = (u32)lbl_804655E0;
            r3 = tmp + r4;
            tmp = *(u32*)((u8*)r3 + 0x0);
        } else {

            tmp = 0x0;
        }
        r29 = tmp;
        tmp = *(u32*)((u8*)r29 + 0x90);
        if ((s32)tmp != 0) {
            tmp = *(u16*)((u8*)r29 + 0x8);
            tmp = tmp & 0x3;
            if ((s32)tmp != 1) {
                if ((s32)tmp >= 1 && (s32)tmp < 4) {

                    r3 = (u32)sp + 0x20;
                    r4 = r29 + 0x3c;
                    r5 = (u32)sp + 0x8;
                    fn_800A3A9C();
                    f0 = *(f32*)(sp + 0x8);
                    r3 = (u32)lbl_80478AC8;
                    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
                    f0 = *(f32*)lbl_80478AC8;
                    /* cror eq, lt, eq */;
                    if (f1 != f0) goto L_801A5F58;
                    f0 = *(f32*)(sp + 0xC);
                    r3 = (u32)lbl_80478AC8;
                    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
                    f0 = *(f32*)lbl_80478AC8;
                    /* cror eq, lt, eq */;
                    if (f1 != f0) goto L_801A5F58;
                    f0 = *(f32*)(sp + 0x10);
                    r3 = (u32)lbl_80478AC8;
                    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
                    f0 = *(f32*)lbl_80478AC8;
                    /* cror eq, lt, eq */;
                    if (f1 != f0) goto L_801A5F58;
                    tmp = -0x1;
                    goto L_801A5F68;
                L_801A5F58:
                    r3 = (u32)sp + 0x8;
                    r4 = (u32)sp + 0x8;
                    fn_800A3ADC();
                    tmp = 0x0;
                L_801A5F68:
                    if ((s32)tmp == 0) {
                        r3 = (u32)sp + 0x8;
                        r4 = (u32)sp + 0x2c;
                        r5 = (u32)sp + 0x14;
                        fn_800A3A78();
                        goto L_801A5FC4;
                    }
                    f2 = *(f32*)lbl_8047DBE8;
                    f1 = *(f32*)lbl_8047DBE8;
                    f0 = *(f32*)lbl_8047DBE4;
                    *(f32*)(sp + 0x14) = f2;
                    *(f32*)(sp + 0x18) = f1;
                    *(f32*)(sp + 0x1C) = f0;
                    goto L_801A6050;
                }
                r3 = r29 + 0x3c;
                r4 = (u32)sp + 0x2c;
                r5 = (u32)sp + 0x14;
                fn_800A3A78();
                goto L_801A5FC4;
                }
            r3 = (u32)lbl_8047DBB8;
            r4 = 0x27a;
            r5 = (u32)lbl_8047DBD4;
            fn_80196E10();
        L_801A5FC4:
            f0 = *(f32*)(sp + 0x14);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A6020;
            f0 = *(f32*)(sp + 0x18);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A6020;
            f0 = *(f32*)(sp + 0x1C);
            r3 = (u32)lbl_80478AC8;
            /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
            f0 = *(f32*)lbl_80478AC8;
            /* cror eq, lt, eq */;
            if (f1 != f0) goto L_801A6020;
            tmp = -0x1;
            goto L_801A6030;
        L_801A6020:
            r3 = (u32)sp + 0x14;
            r4 = (u32)sp + 0x14;
            fn_800A3ADC();
            tmp = 0x0;
        L_801A6030:
            if ((s32)tmp == 0) goto L_801A6050;
            f2 = *(f32*)lbl_8047DBE8;
            f1 = *(f32*)lbl_8047DBE8;
            f0 = *(f32*)lbl_8047DBE4;
            *(f32*)(sp + 0x14) = f2;
            *(f32*)(sp + 0x18) = f1;
            *(f32*)(sp + 0x1C) = f0;
        L_801A6050:
            f1 = *(f32*)(sp + 0x14);
            r3 = r29 + 0x94;
            f2 = *(f32*)(sp + 0x18);
            f3 = *(f32*)(sp + 0x1C);
            fn_800BA424();
            tmp = *(u16*)((u8*)r29 + 0x8);
            tmp = tmp | 0x100;
            *(u16*)((u8*)r29 + 0x8) = tmp;
        }
        r31 = r31 + 0x1;

    }
    return;
}

/* 0x801A6098 | 0x174 */
void fn_801A6098(void) {
    extern u8 lbl_8047DBE4[];
    extern u8 lbl_8047DBE8[];
    extern u8 lbl_8047DBFC[];
    extern void fn_800BA198();
    extern void fn_800BA440();
    extern void fn_800BA44C();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f31 = 0.0f;

    tmp = *(u16*)((u8*)r3 + 0x8);
    f31 = f1;
    r30 = r3;
    r31 = r4;
    tmp = tmp & 0x00000020;
    if ((s32)tmp != 0) return;
    tmp = *(u16*)((u8*)r30 + 0x8);
    tmp = tmp & 0x3;
    if ((s32)tmp == 0) {
        return;
    }
    tmp = *(u16*)((u8*)r30 + 0x8);
    tmp = tmp & 0x14;
    if ((s32)tmp == 0) goto L_801A6174;
    r3 = *(u8*)((u8*)r30 + 0x14);
    tmp = *(u8*)((u8*)r31 + 0x0);
    if (r3 != tmp) goto L_801A6128;
    r3 = *(u8*)((u8*)r30 + 0x15);
    tmp = *(u8*)((u8*)r31 + 0x1);
    if (r3 != tmp) goto L_801A6128;
    r3 = *(u8*)((u8*)r30 + 0x16);
    tmp = *(u8*)((u8*)r31 + 0x2);
    if (r3 != tmp) goto L_801A6128;
    r3 = *(u8*)((u8*)r30 + 0x17);
    tmp = *(u8*)((u8*)r31 + 0x3);
    if (r3 == tmp) goto L_801A6150;
L_801A6128:
    tmp = *(u32*)((u8*)r31 + 0x0);
    r4 = (u32)sp + 0x8;
    r3 = r30 + 0x50;
    *(u32*)(sp + 0x8) = tmp;
    fn_800BA440();
    tmp = *(u32*)((u8*)r31 + 0x0);
    *(u32*)((u8*)r30 + 0x14) = tmp;
    tmp = *(u16*)((u8*)r30 + 0x8);
    tmp = tmp | 0x80;
    *(u16*)((u8*)r30 + 0x8) = tmp;
L_801A6150:
    tmp = *(u16*)((u8*)r30 + 0x8);
    tmp = tmp & 0x00000080;
    if (r3 == tmp) goto L_801A6174;
    r4 = *(u32*)((u8*)r30 + 0x4C);
    r3 = r30 + 0x50;
    fn_800BA44C();
    tmp = *(u16*)((u8*)r30 + 0x8);
    tmp = tmp & 0xFFFFFF7F;
    *(u16*)((u8*)r30 + 0x8) = tmp;
L_801A6174:
    tmp = *(u32*)((u8*)r30 + 0x90);
    if ((s32)tmp == 0) return;
    f0 = *(f32*)((u8*)r30 + 0x38);
    if (f0 != f31) {
        f2 = *(f32*)lbl_8047DBFC;
        r3 = r30 + 0x94;
        f1 = *(f32*)lbl_8047DBFC;
        f0 = *(f32*)lbl_8047DBE4;
        f4 = f31 * f2;
        *(f32*)((u8*)r30 + 0x38) = f31;
        f6 = -(f31 * f1 - f0);
        f1 = *(f32*)lbl_8047DBE8;
        f2 = *(f32*)lbl_8047DBE8;
        f3 = *(f32*)lbl_8047DBE4;
        f5 = *(f32*)lbl_8047DBE8;
        fn_800BA198();
        tmp = *(u16*)((u8*)r30 + 0x8);
        tmp = tmp | 0x100;
        *(u16*)((u8*)r30 + 0x8) = tmp;
    }
    tmp = *(u16*)((u8*)r30 + 0x8);
    tmp = tmp & 0x00000100;
    if (f0 == f31) return;
    r4 = *(u32*)((u8*)r30 + 0x90);
    r3 = r30 + 0x94;
    fn_800BA44C();
    tmp = *(u16*)((u8*)r30 + 0x8);
    tmp = tmp & 0xFFFFFEFF;
    *(u16*)((u8*)r30 + 0x8) = tmp;

    return;
}

/* 0x801A620C | 0x164 */
void fn_801A620C(void) {
    extern u8 lbl_80274D58[];
    extern u8 lbl_80274D64[];
    extern u8 lbl_80478AC8[];
    extern u8 lbl_8047DBE4[];
    extern u8 lbl_8047DBE8[];
    extern void fn_800A3A9C();
    extern void fn_800A3ADC();
    extern void fn_80191688();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r6 = (u32)lbl_80274D58;
    r5 = (u32)lbl_80274D64;
    r31 = r4;
    /* mr. r30, r3 */;
    r4 = *(u32*)lbl_80274D64;
    r8 = *(u32*)lbl_80274D58;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r7 = *(u32*)((u8*)r6 + 0x4);
    r6 = *(u32*)((u8*)r6 + 0x8);
    tmp = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x10) = tmp;
    if ((s32)tmp == 0) return;
    if (r30 != 0) {
        tmp = *(u32*)((u8*)r30 + 0x18);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r30 + 0x18);
            r4 = (u32)sp + 0x14;
            fn_80191688();
    }
    }
    if (r30 != 0) {
        tmp = *(u32*)((u8*)r30 + 0x1C);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r30 + 0x1C);
            r4 = (u32)sp + 0x8;
            fn_80191688();
    }
    }
    r5 = r31;
    r3 = (u32)sp + 0x8;
    r4 = (u32)sp + 0x14;
    fn_800A3A9C();

    if (r31 == 0 || r31 == 0) {

        tmp = -0x1;
        goto L_801A6338;
    }
    f0 = *(f32*)((u8*)r31 + 0x0);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A6328;
    f0 = *(f32*)((u8*)r31 + 0x4);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A6328;
    f0 = *(f32*)((u8*)r31 + 0x8);
    r3 = (u32)lbl_80478AC8;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    f0 = *(f32*)lbl_80478AC8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A6328;
    tmp = -0x1;
    goto L_801A6338;
L_801A6328:
    r3 = r31;
    r4 = r31;
    fn_800A3ADC();
    tmp = 0x0;
L_801A6338:
    if ((s32)tmp == 0) return;
    f0 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    *(f32*)((u8*)r31 + 0x0) = f0;
    f0 = *(f32*)lbl_8047DBE4;
    *(f32*)((u8*)r31 + 0x4) = f1;
    *(f32*)((u8*)r31 + 0x8) = f0;

    return;
}

/* 0x801A6370 | 0x98 */
void fn_801A6370(void) {
    extern void fn_80191E38();
    extern void fn_801C29C4();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    f31 = f1;
    if (r3 != 0) {
        r31 = r3;
        while (r31 != 0) {

            if (r31 != 0) {
                f1 = f31;
                r3 = *(u32*)((u8*)r31 + 0x48);
                fn_801C29C4();
                if (r31 != 0) {
                    r3 = *(u32*)((u8*)r31 + 0x18);
                } else {

                    r3 = 0x0;
                }
                f1 = f31;
                fn_80191E38();
                if (r31 != 0) {
                    r3 = *(u32*)((u8*)r31 + 0x1C);
                } else {

                    r3 = 0x0;
                }
                f1 = f31;
                fn_80191E38();
            }
            r31 = *(u32*)((u8*)r31 + 0xC);

        }
    }
    return;
}

/* 0x801A6408 | 0x8C */
void fn_801A6408(void) {
    extern void fn_801919EC();
    extern void fn_801C27F4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    if (r3 != 0) {
        r31 = r3;
        while (r31 != 0) {

            if (r31 != 0) {
                r5 = *(u32*)((u8*)r31 + 0x0);
                r4 = r31;
                r3 = *(u32*)((u8*)r31 + 0x48);
                r5 = *(u32*)((u8*)r5 + 0x40);
                fn_801C27F4();
                if (r31 != 0) {
                    r3 = *(u32*)((u8*)r31 + 0x18);
                } else {

                    r3 = 0x0;
                }
                fn_801919EC();
                if (r31 != 0) {
                    r3 = *(u32*)((u8*)r31 + 0x1C);
                } else {

                    r3 = 0x0;
                }
                fn_801919EC();
            }
            r31 = *(u32*)((u8*)r31 + 0xC);

        }
    }
    return;
}

/* 0x801A6494 | 0x24C */
void fn_801A6494(void) {
    extern u8 lbl_8047DBE4[];
    extern u8 lbl_8047DBE8[];
    extern u8 lbl_8047DC08[];
    extern u8 lbl_8047DC10[];
    extern u8 jumptable_8036CAF4[];
    u8 sp[0x100];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    if (r3 == 0) return;
    if (tmp > 0xd) return;
    r4 = (u32)jumptable_8036CAF4;
    tmp = tmp << 2;
    r4 = (u32)jumptable_8036CAF4;
    r4 = *(u32*)(r4 + tmp);
    ctr_fn = (void(*)(void))r4;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f64*)lbl_8047DC08;
    /* cror eq, gt, eq */;
    if (f1 == f0) {
        tmp = *(u16*)((u8*)r3 + 0x8);
        tmp = tmp & 0xFFFFFFDF;
        *(u16*)((u8*)r3 + 0x8) = tmp;
        return;
    }
    tmp = *(u16*)((u8*)r3 + 0x8);
    tmp = tmp | 0x20;
    *(u16*)((u8*)r3 + 0x8) = tmp;
    return;
    tmp = *(u16*)((u8*)r3 + 0x8);
    tmp = tmp & 0x00000040;
    if (f1 != f0) {
        f0 = *(f32*)((u8*)r5 + 0x0);
        *(f32*)((u8*)r3 + 0x20) = f0;
        return;
    }
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x20) = f0;
    return;
    tmp = *(u16*)((u8*)r3 + 0x8);
    tmp = tmp & 0x00000040;
    if (f1 != f0) {
        f0 = *(f32*)((u8*)r5 + 0x0);
        *(f32*)((u8*)r3 + 0x24) = f0;
        return;
    }
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x2C) = f0;
    return;
    tmp = *(u16*)((u8*)r3 + 0x8);
    tmp = tmp & 0x00000040;
    if (f1 != f0) {
        f0 = *(f32*)((u8*)r5 + 0x0);
        *(f32*)((u8*)r3 + 0x28) = f0;
        return;
    }
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x28) = f0;
    return;
    tmp = *(u16*)((u8*)r3 + 0x8);
    tmp = tmp & 0x00000040;
    if (f1 == f0) return;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x2C) = f0;
    return;
    tmp = *(u16*)((u8*)r3 + 0x8);
    tmp = tmp & 0x00000040;
    if (f1 == f0) return;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x30) = f0;
    return;
    tmp = *(u16*)((u8*)r3 + 0x8);
    tmp = tmp & 0x00000040;
    if (f1 == f0) return;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x34) = f0;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DBE8;
        goto L_801A65DC;
    }
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A65DC;
    f1 = *(f32*)lbl_8047DBE4;
L_801A65DC:
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x10) = tmp;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DBE8;
        goto L_801A6628;
    }
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A6628;
    f1 = *(f32*)lbl_8047DBE4;
L_801A6628:
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x11) = tmp;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DBE8;
        goto L_801A6674;
    }
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A6674;
    f1 = *(f32*)lbl_8047DBE4;
L_801A6674:
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x12) = tmp;
    return;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 == f0) {
        f1 = *(f32*)lbl_8047DBE8;
        goto L_801A66C0;
    }
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A66C0;
    f1 = *(f32*)lbl_8047DBE4;
L_801A66C0:
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x13) = tmp;

    return;
}

/* 0x801A66E0 | 0xAC */
void fn_801A66E0(void) {
    extern void fn_80191E88();
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if (r3 != 0) {
        r30 = r3;
        while (r30 != 0) {

            if (r30 != 0) {
                r31 = r30;
                while (r31 != 0) {

                    if (r31 != 0) {
                        r3 = *(u32*)((u8*)r31 + 0x48);
                        fn_801C25E4();
                        tmp = 0x0;
                        *(u32*)((u8*)r31 + 0x48) = tmp;
                        if (r31 != 0) {
                            r3 = *(u32*)((u8*)r31 + 0x18);
                        } else {

                            r3 = 0x0;
                        }
                        fn_80191E88();
                        if (r31 != 0) {
                            r3 = *(u32*)((u8*)r31 + 0x1C);
                        } else {

                            r3 = 0x0;
                        }
                        fn_80191E88();
                    }
                    r31 = *(u32*)((u8*)r31 + 0xC);

                }
            }
            r30 = *(u32*)((u8*)r30 + 0xC);

        }
    }
    return;
}

/* 0x801A678C | 0x30 */
void fn_801A678C(void) {
    extern u8 lbl_804655E0[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if (((s32)r3 >= 0) && ((s32)r3 < 8)) {

        r4 = (u32)lbl_804655E0;
        r3 = r3 << 2;
        tmp = (u32)lbl_804655E0;
        r3 = tmp + r3;
        r3 = *(u32*)((u8*)r3 + 0x0);
        return;
    }
    r3 = 0x0;
    return;
}

/* 0x801A67BC | 0x114 */
void fn_801A67BC(void) {
    extern u8 lbl_804655E0[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBD4[];
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    switch ((s32)r3) {
        case 1:
            r31 = 0x0;
            break;
        case 2:
            r31 = 0x1;
            break;
        case 4:
            r31 = 0x2;
            break;
        case 8:
            r31 = 0x3;
            break;
        case 0x10:
            r31 = 0x4;
            break;
        case 0x20:
            r31 = 0x5;
            break;
        case 0x40:
            r31 = 0x6;
            break;
        case 0x80:
            r31 = 0x7;
            break;
        case 0x100:
            r31 = 0x8;
            break;
        default:
            r3 = (u32)lbl_8047DBB8;
            r4 = 0x4a1;
            r5 = (u32)lbl_8047DBD4;
            fn_80196E10();
            break;
    }
    /* mr. tmp, r31 */;
    tmp = r31;
    if (((s32)r3 >= 0x100) && ((s32)tmp < 9)) {

        r3 = (u32)lbl_804655E0;
        r4 = tmp << 2;
        tmp = (u32)lbl_804655E0;
        r3 = tmp + r4;
        r3 = *(u32*)((u8*)r3 + 0x0);
        return;
    }
    r3 = 0x0;

    return;
}

/* 0x801A68F8 | 0x18 */
void fn_801A68F8(u8* ptr, u16 mask) {
    if (ptr == NULL) { return; }
    *(u16*)(ptr + 0x8) &= ~mask;
}

/* 0x801A6910 | 0x18 */
void fn_801A6910(u8* ptr, u16 mask) {
    if (ptr == NULL) { return; }
    *(u16*)(ptr + 0x8) |= mask;
}

/* 0x801A6928 | 0x38 */
void fn_801A6928(void) {
    extern u8 lbl_80465608[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    void (*ctr_fn)(void) = 0;

    r4 = (u32)lbl_80465608;
    r5 = (u32)lbl_80465608;
    r4 = 0x20;
    r12 = *(u32*)((u8*)r5 + 0x0);
    r5 = 0x0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

/* 0x801A6960 | 0x30 */
void fn_801A6960(void) {
    extern u8 lbl_80465608[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    void (*ctr_fn)(void) = 0;

    r4 = (u32)lbl_80465608;
    r4 = (u32)lbl_80465608;
    r12 = *(u32*)((u8*)r4 + 0x4);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

/* 0x801A6990 | 0x30 */
void fn_801A6990(void) {
    extern u8 lbl_80465608[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    void (*ctr_fn)(void) = 0;

    r4 = (u32)lbl_80465608;
    r4 = (u32)lbl_80465608;
    r12 = *(u32*)((u8*)r4 + 0x10);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}
