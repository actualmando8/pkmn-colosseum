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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    r0 = (u32)fn_801A6494;
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
    *(u32*)((u8*)r3 + 0x40) = r0;
    return;
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
    extern u8 lbl_8036CA20[];
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x48);
    fn_801C25E4();
    if ((u32)r31 == (u32)0x0) goto L_801A4128;
    r30 = *(u32*)((u8*)r31 + 0x18);
    goto L_801A412C;
L_801A4128: ;
    r30 = 0x0;
L_801A412C: ;
    if ((u32)r30 == (u32)0x0) goto L_801A41A8;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r30 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_801A4158;
    goto L_801A4170;
L_801A4158: ;
    r0 = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A4170: ;
    if ((s32)r3 == (s32)0x0) goto L_801A41A8;
    if ((u32)r30 == (u32)0x0) goto L_801A41A8;
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
L_801A41A8: ;
    if ((u32)r31 == (u32)0x0) goto L_801A41B8;
    r30 = *(u32*)((u8*)r31 + 0x1C);
    goto L_801A41BC;
L_801A41B8: ;
    r30 = 0x0;
L_801A41BC: ;
    if ((u32)r30 == (u32)0x0) goto L_801A4238;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r30 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_801A41E8;
    goto L_801A4200;
L_801A41E8: ;
    r0 = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A4200: ;
    if ((s32)r3 == (s32)0x0) goto L_801A4238;
    if ((u32)r30 == (u32)0x0) goto L_801A4238;
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
L_801A4238: ;
    r4 = (u32)lbl_8036CA20;
    r3 = r31;
    r4 = (u32)lbl_8036CA20;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801A426C | 0xD8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A426C(void) {
    extern void fn_80191DCC();
    extern void fn_801C25E4();
    extern void fn_801C2670();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if ((u32)r3 == (u32)0x0) goto L_801A432C;
    r31 = r3;
    r30 = r4;
    goto L_801A4324;
L_801A4294: ;
    if ((u32)r31 == (u32)0x0) goto L_801A42FC;
    if ((u32)r30 == (u32)0x0) goto L_801A42FC;
    r0 = *(u32*)((u8*)r31 + 0x48);
    if ((u32)r0 == (u32)0x0) goto L_801A42B8;
    r3 = *(u32*)((u8*)r31 + 0x48);
    fn_801C25E4();
L_801A42B8: ;
    r3 = *(u32*)((u8*)r30 + 0x4);
    fn_801C2670();
    *(u32*)((u8*)r31 + 0x48) = r3;
    if ((u32)r31 == (u32)0x0) goto L_801A42D4;
    r3 = *(u32*)((u8*)r31 + 0x18);
    goto L_801A42D8;
L_801A42D4: ;
    r3 = 0x0;
L_801A42D8: ;
    r4 = *(u32*)((u8*)r30 + 0x8);
    fn_80191DCC();
    if ((u32)r31 == (u32)0x0) goto L_801A42F0;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    goto L_801A42F4;
L_801A42F0: ;
    r3 = 0x0;
L_801A42F4: ;
    r4 = *(u32*)((u8*)r30 + 0xC);
    fn_80191DCC();
L_801A42FC: ;
    if ((u32)r31 == (u32)0x0) goto L_801A430C;
    r31 = *(u32*)((u8*)r31 + 0xC);
    goto L_801A4310;
L_801A430C: ;
    r31 = 0x0;
L_801A4310: ;
    if ((u32)r30 == (u32)0x0) goto L_801A4320;
    r30 = *(u32*)((u8*)r30 + 0x0);
    goto L_801A4324;
L_801A4320: ;
    r30 = 0x0;
L_801A4324: ;
    if ((u32)r31 != (u32)0x0) goto L_801A4294;
L_801A432C: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801A4344 | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r1 + 0x8;
    r30 = r3;
    goto L_801A4410;
L_801A4368: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_801A4384;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_80193748();
    if ((u32)r3 != (u32)0x0) goto L_801A43C4;
L_801A4384: ;
    r0 = *(u32*)lbl_8047B2B0;
    if ((u32)r0 == (u32)0x0) goto L_801A4398;
    r3 = *(u32*)lbl_8047B2B0;
    goto L_801A43A0;
L_801A4398: ;
    r3 = (u32)lbl_8036CA20;
    r3 = (u32)lbl_8036CA20;
L_801A43A0: ;
    fn_80193828();
    /* mr. r29, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_801A43BC;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x5d5;
    r5 = (u32)lbl_8047DBC0;
    fn_80196E10();
L_801A43BC: ;
    *(u32*)((u8*)r31 + 0x0) = r29;
    goto L_801A43E8;
L_801A43C4: ;
    fn_80193828();
    *(u32*)((u8*)r31 + 0x0) = r3;
    r0 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801A43E8;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x67b;
    r5 = (u32)lbl_8047DBC4;
    fn_80196E10();
L_801A43E8: ;
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
L_801A4410: ;
    if ((u32)r30 != (u32)0x0) goto L_801A4368;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x0) = r0;
    r3 = *(u32*)(sp + 0x8);
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x801A4440 | 0x470 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4440(void) {
    extern u8 lbl_80274D94[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBC8[];
    extern void fn_801917D0();
    extern void fn_80196D78();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    r0 = *(u32*)((u8*)r4 + 0xC);
    *(u32*)(sp + 0x8) = r0;
    r0 = *(u32*)(sp + 0x8);
    *(u32*)((u8*)r3 + 0x10) = r0;
    r3 = *(u16*)((u8*)r4 + 0x8);
    if ((s32)r0 == (s32)0) goto L_801A4488;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 | r3;
    *(u16*)((u8*)r30 + 0x8) = r0;
L_801A4488: ;
    r0 = *(u16*)((u8*)r31 + 0x8);
    r0 = r0 & 0x3;
    if ((s32)r0 == (s32)0x2) goto L_801A4554;
    if ((s32)r0 >= (s32)0x2) goto L_801A44AC;
    if ((s32)r0 == (s32)0x0) goto L_801A488C;
    if ((s32)r0 >= (s32)0x0) goto L_801A44B8;
    goto L_801A4868;
L_801A44AC: ;
    if ((s32)r0 >= (s32)0x4) goto L_801A4868;
    goto L_801A466C;
L_801A44B8: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801917D0();
    r31 = r3;
    if ((u32)r30 == (u32)0x0) goto L_801A488C;
    r29 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r29 == (u32)0x0) goto L_801A454C;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r29 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r29 == (u32)0x0) goto L_801A44FC;
    goto L_801A4514;
L_801A44FC: ;
    r0 = *(u16*)((u8*)r29 + 0x4);
    r3 = *(u16*)((u8*)r29 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r29 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A4514: ;
    if ((s32)r3 == (s32)0x0) goto L_801A454C;
    if ((u32)r29 == (u32)0x0) goto L_801A454C;
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
L_801A454C: ;
    *(u32*)((u8*)r30 + 0x18) = r31;
    goto L_801A488C;
L_801A4554: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801917D0();
    r29 = r3;
    if ((u32)r30 == (u32)0x0) goto L_801A45EC;
    r28 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r28 == (u32)0x0) goto L_801A45E8;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r28 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r28 == (u32)0x0) goto L_801A4598;
    goto L_801A45B0;
L_801A4598: ;
    r0 = *(u16*)((u8*)r28 + 0x4);
    r3 = *(u16*)((u8*)r28 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r28 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A45B0: ;
    if ((s32)r3 == (s32)0x0) goto L_801A45E8;
    if ((u32)r28 == (u32)0x0) goto L_801A45E8;
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
L_801A45E8: ;
    *(u32*)((u8*)r30 + 0x18) = r29;
L_801A45EC: ;
    r0 = *(u16*)((u8*)r31 + 0xA);
    r0 = r0 & 0x1;
    if ((u32)r28 == (u32)0x0) goto L_801A463C;
    if ((u32)r30 == (u32)0x0) goto L_801A460C;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 | 0x40;
    *(u16*)((u8*)r30 + 0x8) = r0;
L_801A460C: ;
    r5 = *(u32*)((u8*)r31 + 0x18);
    r4 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x18);
    f2 = *(f32*)((u8*)r5 + 0x14);
    f1 = *(f32*)((u8*)r4 + 0x10);
    f0 = *(f32*)((u8*)r3 + 0xC);
    if ((u32)r30 == (u32)0x0) goto L_801A488C;
    *(f32*)((u8*)r30 + 0x2C) = f0;
    *(f32*)((u8*)r30 + 0x30) = f1;
    *(f32*)((u8*)r30 + 0x34) = f2;
    goto L_801A488C;
L_801A463C: ;
    r5 = *(u32*)((u8*)r31 + 0x18);
    r4 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x18);
    r0 = *(u32*)((u8*)r5 + 0x8);
    f1 = *(f32*)((u8*)r4 + 0x0);
    f0 = *(f32*)((u8*)r3 + 0x4);
    if ((u32)r30 == (u32)0x0) goto L_801A488C;
    *(f32*)((u8*)r30 + 0x2C) = f0;
    *(f32*)((u8*)r30 + 0x28) = f1;
    *(u32*)((u8*)r30 + 0x30) = r0;
    goto L_801A488C;
L_801A466C: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801917D0();
    r29 = r3;
    if ((u32)r30 == (u32)0x0) goto L_801A4704;
    r28 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r28 == (u32)0x0) goto L_801A4700;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r28 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r28 == (u32)0x0) goto L_801A46B0;
    goto L_801A46C8;
L_801A46B0: ;
    r0 = *(u16*)((u8*)r28 + 0x4);
    r3 = *(u16*)((u8*)r28 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r28 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A46C8: ;
    if ((s32)r3 == (s32)0x0) goto L_801A4700;
    if ((u32)r28 == (u32)0x0) goto L_801A4700;
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
L_801A4700: ;
    *(u32*)((u8*)r30 + 0x18) = r29;
L_801A4704: ;
    r3 = *(u32*)((u8*)r31 + 0x14);
    fn_801917D0();
    r29 = r3;
    if ((u32)r30 == (u32)0x0) goto L_801A479C;
    r28 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r28 == (u32)0x0) goto L_801A4798;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r28 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r28 == (u32)0x0) goto L_801A4748;
    goto L_801A4760;
L_801A4748: ;
    r0 = *(u16*)((u8*)r28 + 0x4);
    r3 = *(u16*)((u8*)r28 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r28 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A4760: ;
    if ((s32)r3 == (s32)0x0) goto L_801A4798;
    if ((u32)r28 == (u32)0x0) goto L_801A4798;
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
L_801A4798: ;
    *(u32*)((u8*)r30 + 0x1C) = r29;
L_801A479C: ;
    r0 = *(u16*)((u8*)r31 + 0xA);
    r0 = r0 & 0x1;
    if ((u32)r28 == (u32)0x0) goto L_801A4818;
    if ((u32)r30 == (u32)0x0) goto L_801A47BC;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 | 0x40;
    *(u16*)((u8*)r30 + 0x8) = r0;
L_801A47BC: ;
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
    if ((u32)r30 == (u32)0x0) goto L_801A4800;
    *(f32*)((u8*)r30 + 0x20) = f0;
    *(f32*)((u8*)r30 + 0x24) = f1;
    *(f32*)((u8*)r30 + 0x28) = f2;
L_801A4800: ;
    if ((u32)r30 == (u32)0x0) goto L_801A488C;
    *(f32*)((u8*)r30 + 0x2C) = f3;
    *(f32*)((u8*)r30 + 0x30) = f4;
    *(f32*)((u8*)r30 + 0x34) = f5;
    goto L_801A488C;
L_801A4818: ;
    r5 = *(u32*)((u8*)r31 + 0x18);
    r4 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x18);
    r0 = *(u32*)((u8*)r5 + 0x10);
    f1 = *(f32*)((u8*)r4 + 0x8);
    f0 = *(f32*)((u8*)r3 + 0xC);
    if ((u32)r30 == (u32)0x0) goto L_801A4844;
    *(f32*)((u8*)r30 + 0x2C) = f0;
    *(f32*)((u8*)r30 + 0x28) = f1;
    *(u32*)((u8*)r30 + 0x30) = r0;
L_801A4844: ;
    r4 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x18);
    r0 = *(u32*)((u8*)r4 + 0x4);
    f0 = *(f32*)((u8*)r3 + 0x0);
    if ((u32)r30 == (u32)0x0) goto L_801A488C;
    *(f32*)((u8*)r30 + 0x20) = f0;
    *(u32*)((u8*)r30 + 0x24) = r0;
    goto L_801A488C;
L_801A4868: ;
    r3 = (u32)lbl_80274D94;
    r4 = *(u16*)((u8*)r31 + 0x8);
    r3 = (u32)lbl_80274D94;
    /* crclr cr1eq */;
    OSReport();
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x659;
    r5 = (u32)lbl_8047DBC8;
    fn_80196D78();
L_801A488C: ;
    r3 = 0x0;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
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
    extern u8 lbl_80274DB8[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBCC[];
    extern void fn_80191628();
    extern void fn_80191788();
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_801A4924;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x58c;
    r5 = (u32)lbl_8047DBCC;
    fn_80196E10();
L_801A4924: ;
    r0 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r0 != (u32)0x0) goto L_801A4958;
    fn_80191628();
    *(u32*)((u8*)r30 + 0x1C) = r3;
    r0 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r0 != (u32)0x0) goto L_801A4958;
    r4 = (u32)lbl_80274DB8;
    r3 = (u32)lbl_8047DBB8;
    r5 = (u32)lbl_80274DB8;
    r4 = 0x58f;
    fn_80196E10();
L_801A4958: ;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = r31;
    fn_80191788();
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
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
    extern u8 lbl_80274DC8[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBCC[];
    extern void fn_80191628();
    extern void fn_80191788();
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_801A49F0;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x568;
    r5 = (u32)lbl_8047DBCC;
    fn_80196E10();
L_801A49F0: ;
    r0 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r0 != (u32)0x0) goto L_801A4A24;
    fn_80191628();
    *(u32*)((u8*)r30 + 0x18) = r3;
    r0 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r0 != (u32)0x0) goto L_801A4A24;
    r4 = (u32)lbl_80274DC8;
    r3 = (u32)lbl_8047DBB8;
    r5 = (u32)lbl_80274DC8;
    r4 = 0x56b;
    fn_80196E10();
L_801A4A24: ;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r31;
    fn_80191788();
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
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
    extern u8 lbl_8047B2B4[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = *(u32*)lbl_8047B2B4;
    r3 = r3 & 0x3;
    goto L_801A4AF0;
L_801A4AD0: ;
    r4 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u16*)((u8*)r4 + 0x8);
    r0 = r0 & 0x3;
    if ((u32)r3 != (u32)r0) goto L_801A4AEC;
    r3 = *(u32*)((u8*)r5 + 0x4);
    return;
L_801A4AEC: ;
    r5 = *(u32*)((u8*)r5 + 0x0);
L_801A4AF0: ;
    if ((u32)r5 != (u32)0x0) goto L_801A4AD0;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801A4B00 | 0x220 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A4B00(void) {
    extern u8 lbl_804655E0[];
    extern u8 lbl_8047B2B4[];
    extern void fn_801A3E64();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801A4C34;
    goto L_801A4C28;
L_801A4B20: ;
    if ((u32)r31 == (u32)0x0) goto L_801A4C24;
    r30 = (u32)lbl_8047B2B4;
    goto L_801A4C18;
L_801A4B30: ;
    r3 = *(u32*)((u8*)r30 + 0x0);
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r31) goto L_801A4C14;
    r6 = 0x0;
    goto L_801A4B80;
L_801A4B48: ;
    r3 = (u32)lbl_804655E0;
    r4 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r31 != (u32)r0) goto L_801A4B7C;
    r3 = (u32)lbl_804655E0;
    r4 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r5 = 0x0;
    r3 = r0 + r4;
    *(u32*)((u8*)r3 + 0x0) = r5;
L_801A4B7C: ;
    r6 = r6 + 0x1;
L_801A4B80: ;
    if ((s32)r6 < (s32)0x9) goto L_801A4B48;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_801A3E64();
    *(u32*)((u8*)r30 + 0x0) = r3;
    if ((u32)r31 == (u32)0x0) goto L_801A4C24;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r31 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_801A4BC0;
    goto L_801A4BD8;
L_801A4BC0: ;
    r0 = *(u16*)((u8*)r31 + 0x4);
    r3 = *(u16*)((u8*)r31 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r31 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A4BD8: ;
    if ((s32)r3 == (s32)0x0) goto L_801A4C24;
    if ((u32)r31 == (u32)0x0) goto L_801A4C24;
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
    goto L_801A4C24;
L_801A4C14: ;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_801A4C18: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801A4B30;
L_801A4C24: ;
    r31 = *(u32*)((u8*)r31 + 0xC);
L_801A4C28: ;
    if ((u32)r31 != (u32)0x0) goto L_801A4B20;
    goto L_801A4D08;
L_801A4C34: ;
    r6 = 0x0;
    goto L_801A4C58;
L_801A4C3C: ;
    r3 = (u32)lbl_804655E0;
    r4 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r5 = 0x0;
    r3 = r0 + r4;
    r6 = r6 + 0x1;
    *(u32*)((u8*)r3 + 0x0) = r5;
L_801A4C58: ;
    if ((s32)r6 < (s32)0x9) goto L_801A4C3C;
    r0 = 0x0;
    *(u32*)&lbl_8047B2B8 = r0;
    goto L_801A4CFC;
L_801A4C6C: ;
    r3 = *(u32*)lbl_8047B2B4;
    r30 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r30 == (u32)0x0) goto L_801A4CF0;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r30 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_801A4CA0;
    goto L_801A4CB8;
L_801A4CA0: ;
    r0 = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A4CB8: ;
    if ((s32)r3 == (s32)0x0) goto L_801A4CF0;
    if ((u32)r30 == (u32)0x0) goto L_801A4CF0;
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
L_801A4CF0: ;
    r3 = *(u32*)lbl_8047B2B4;
    fn_801A3E64();
    *(u32*)lbl_8047B2B4 = r3;
L_801A4CFC: ;
    r0 = *(u32*)lbl_8047B2B4;
    if ((u32)r0 != (u32)0x0) goto L_801A4C6C;
L_801A4D08: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801A4D20 | 0x234 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    goto L_801A4F2C;
L_801A4D44: ;
    if ((u32)r31 == (u32)0x0) goto L_801A4F28;
    r3 = *(u32*)lbl_8047B2B4;
    goto L_801A4E6C;
L_801A4D54: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r31) goto L_801A4E68;
    if ((u32)r31 == (u32)0x0) goto L_801A4E74;
    r30 = (u32)lbl_8047B2B4;
    goto L_801A4E58;
L_801A4D70: ;
    r3 = *(u32*)((u8*)r30 + 0x0);
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r31) goto L_801A4E54;
    r6 = 0x0;
    goto L_801A4DC0;
L_801A4D88: ;
    r3 = (u32)lbl_804655E0;
    r4 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r31 != (u32)r0) goto L_801A4DBC;
    r3 = (u32)lbl_804655E0;
    r4 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r5 = 0x0;
    r3 = r0 + r4;
    *(u32*)((u8*)r3 + 0x0) = r5;
L_801A4DBC: ;
    r6 = r6 + 0x1;
L_801A4DC0: ;
    if ((s32)r6 < (s32)0x9) goto L_801A4D88;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_801A3E64();
    *(u32*)((u8*)r30 + 0x0) = r3;
    if ((u32)r31 == (u32)0x0) goto L_801A4E74;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r31 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_801A4E00;
    goto L_801A4E18;
L_801A4E00: ;
    r0 = *(u16*)((u8*)r31 + 0x4);
    r3 = *(u16*)((u8*)r31 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r31 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A4E18: ;
    if ((s32)r3 == (s32)0x0) goto L_801A4E74;
    if ((u32)r31 == (u32)0x0) goto L_801A4E74;
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
    goto L_801A4E74;
L_801A4E54: ;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_801A4E58: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801A4D70;
    goto L_801A4E74;
L_801A4E68: ;
    r3 = *(u32*)((u8*)r3 + 0x0);
L_801A4E6C: ;
    if ((u32)r3 != (u32)0x0) goto L_801A4D54;
L_801A4E74: ;
    if ((u32)r31 == (u32)0x0) goto L_801A4EAC;
    r3 = *(u16*)((u8*)r31 + 0x4);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r31 + 0x4) = r0;
    r0 = *(u16*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)0xffff) goto L_801A4EAC;
    r3 = (u32)lbl_80274DD8;
    r5 = (u32)lbl_80274DE4;
    r3 = (u32)lbl_80274DD8;
    r4 = 0x5d;
    r5 = (u32)lbl_80274DE4;
    fn_80196E10();
L_801A4EAC: ;
    r30 = (u32)lbl_8047B2B4;
    goto L_801A4F0C;
L_801A4EB4: ;
    if ((u32)r31 != (u32)0x0) goto L_801A4ECC;
    r3 = (u32)lbl_8047DBD8;
    r4 = 0x163;
    r5 = (u32)lbl_8047DBCC;
    fn_80196E10();
L_801A4ECC: ;
    r3 = *(u32*)((u8*)r30 + 0x0);
    r0 = *(u16*)((u8*)r31 + 0xA);
    r29 = *(u32*)((u8*)r3 + 0x4);
    r28 = r0 & 0xFF;
    if ((u32)r29 != (u32)0x0) goto L_801A4EF4;
    r3 = (u32)lbl_8047DBD8;
    r4 = 0x163;
    r5 = (u32)lbl_8047DBCC;
    fn_80196E10();
L_801A4EF4: ;
    r3 = *(u16*)((u8*)r29 + 0xA);
    r0 = r28 & 0xFF;
    r3 = r3 & 0xFF;
    if ((u32)r3 > (u32)r0) goto L_801A4F18;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_801A4F0C: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_801A4EB4;
L_801A4F18: ;
    r3 = *(u32*)((u8*)r30 + 0x0);
    r4 = r31;
    fn_801A3EB4();
    *(u32*)((u8*)r30 + 0x0) = r3;
L_801A4F28: ;
    r31 = *(u32*)((u8*)r31 + 0xC);
L_801A4F2C: ;
    if ((u32)r31 != (u32)0x0) goto L_801A4D44;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801A4F54 | 0xE78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    *(f64*)(sp + 0xE0) = f31;
    /* psq_st f31, 0xe8(r1), 0, qr0 */;
    *(f64*)(sp + 0xD0) = f30;
    /* psq_st f30, 0xd8(r1), 0, qr0 */;
    *(f64*)(sp + 0xC0) = f29;
    /* psq_st f29, 0xc8(r1), 0, qr0 */;
    /* stmw r25, 0xa4(r1) */;
    r6 = 0x0;
    r5 = 0x0;
    r4 = 0x0;
    r0 = 0x0;
    r3 = r3 + 0x54;
    *(u32*)&lbl_8047B2BC = r6;
    r31 = r3;
    r30 = 0x0;
    *(u32*)&lbl_8047B2C0 = r5;
    r6 = 0x0;
    *(u32*)&lbl_8047B2C4 = r4;
    *(u32*)&lbl_8047B2C8 = r0;
    goto L_801A4FCC;
L_801A4FB0: ;
    r3 = (u32)lbl_804655E0;
    r4 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r5 = 0x0;
    r3 = r0 + r4;
    r6 = r6 + 0x1;
    *(u32*)((u8*)r3 + 0x0) = r5;
L_801A4FCC: ;
    if ((s32)r6 < (s32)0x9) goto L_801A4FB0;
    r0 = 0x0;
    r27 = *(u32*)lbl_8047B2B4;
    *(u32*)&lbl_8047B2B8 = r0;
    goto L_801A57D0;
L_801A4FE4: ;
    r28 = *(u32*)((u8*)r27 + 0x4);
    if ((u32)r28 == (u32)0x0) goto L_801A57CC;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x00000020;
    if ((u32)r28 != (u32)0x0) goto L_801A57CC;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r3 = *(u16*)((u8*)r28 + 0x8);
    r29 = r0 & 0x0000001C;
    r0 = r3 & 0x3;
    if ((u32)r28 == (u32)0x0) goto L_801A57CC;
    r3 = *(u16*)((u8*)r28 + 0x8);
    r3 = r3 & 0x3;
    if ((u32)r28 != (u32)0x0) goto L_801A5040;
    r5 = 0x8;
    r3 = (u32)lbl_804655E0;
    r4 = r5 << 2;
    r3 = (u32)lbl_804655E0;
    r3 = r3 + r4;
    r3 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_801A504C;
    goto L_801A50D0;
L_801A5040: ;
    r5 = *(u32*)&lbl_8047B2B8;
    r3 = r5 + 0x1;
    *(u32*)&lbl_8047B2B8 = r3;
L_801A504C: ;
    r3 = (u32)lbl_804655E0;
    r4 = r5 << 2;
    r3 = (u32)lbl_804655E0;
    r3 = r3 + r4;
    *(u32*)((u8*)r3 + 0x0) = r28;
    if ((u32)r5 > (u32)0x8) goto L_801A50C8;
    r4 = (u32)jumptable_8036CAD0;
    r3 = r5 << 2;
    r4 = (u32)jumptable_8036CAD0;
    r4 = *(u32*)(r4 + r3);
    ctr_fn = (void(*)(void))r4;
    /* indirect jump via ctr */;
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
L_801A50C8: ;
    r3 = 0x0;
L_801A50CC: ;
    *(u32*)((u8*)r28 + 0x4C) = r3;
L_801A50D0: ;
    r30 = *(u32*)&lbl_8047B2B8;
    r3 = 0x0;
    *(u32*)((u8*)r28 + 0x90) = r3;
    if ((s32)r0 == (s32)0x2) goto L_801A51C8;
    if ((s32)r0 >= (s32)0x2) goto L_801A50F8;
    if ((s32)r0 == (s32)0x0) goto L_801A57CC;
    if ((s32)r0 >= (s32)0x0) goto L_801A5104;
    goto L_801A54E8;
L_801A50F8: ;
    if ((s32)r0 >= (s32)0x4) goto L_801A54E8;
    goto L_801A52B4;
L_801A5104: ;
    if ((u32)r28 == (u32)0x0) goto L_801A5124;
    r0 = *(u32*)((u8*)r28 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A5124;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = r1 + 0x8c;
    fn_80191688();
L_801A5124: ;
    f1 = *(f32*)(sp + 0x8C);
    r3 = r31;
    f0 = *(f32*)lbl_8047DBE0;
    r4 = r1 + 0x8c;
    f3 = *(f32*)(sp + 0x90);
    r5 = r1 + 0x8c;
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
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x00000004;
    if ((u32)r0 == (u32)0x0) goto L_801A51A4;
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
L_801A51A4: ;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x00000008;
    if ((u32)r0 == (u32)0x0) goto L_801A54E8;
    f1 = *(f32*)(sp + 0x8C);
    r3 = r28 + 0x94;
    f2 = *(f32*)(sp + 0x90);
    f3 = *(f32*)(sp + 0x94);
    fn_800BA414();
    goto L_801A54E8;
L_801A51C8: ;
    r0 = *(u32*)((u8*)r28 + 0x10);
    r4 = r1 + 0x18;
    r3 = r28 + 0x50;
    *(u32*)(sp + 0x18) = r0;
    fn_800BA440();
    r0 = *(u32*)((u8*)r28 + 0x10);
    *(u32*)((u8*)r28 + 0x14) = r0;
    if ((u32)r28 == (u32)0x0) goto L_801A5204;
    r0 = *(u32*)((u8*)r28 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A5204;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = r1 + 0x80;
    fn_80191688();
L_801A5204: ;
    r3 = r31;
    r4 = r1 + 0x80;
    r5 = r1 + 0x80;
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
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x00000040;
    if ((u32)r0 == (u32)0x0) goto L_801A526C;
    f1 = *(f32*)lbl_8047DBE4;
    r3 = r28 + 0x50;
    f2 = *(f32*)lbl_8047DBE8;
    f3 = *(f32*)lbl_8047DBE8;
    f4 = *(f32*)((u8*)r28 + 0x2C);
    f5 = *(f32*)((u8*)r28 + 0x30);
    f6 = *(f32*)((u8*)r28 + 0x34);
    fn_800BA198();
    goto L_801A54E8;
L_801A526C: ;
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
L_801A52B4: ;
    if ((u32)r28 == (u32)0x0) goto L_801A52D4;
    r0 = *(u32*)((u8*)r28 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A52D4;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = r1 + 0x68;
    fn_80191688();
L_801A52D4: ;
    r3 = r31;
    r4 = r1 + 0x68;
    r5 = r1 + 0x68;
    fn_800A37CC();
    r4 = (u32)lbl_80274D58;
    r8 = *(u32*)lbl_80274D58;
    r3 = (u32)lbl_80274D64;
    r5 = (u32)lbl_80274D64;
    r7 = *(u32*)((u8*)r4 + 0x4);
    r6 = *(u32*)((u8*)r4 + 0x8);
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x64) = r0;
    if ((u32)r28 == (u32)0x0) goto L_801A5404;
    if ((u32)r28 == (u32)0x0) goto L_801A5348;
    r0 = *(u32*)((u8*)r28 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A5348;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = r1 + 0x50;
    fn_80191688();
L_801A5348: ;
    if ((u32)r28 == (u32)0x0) goto L_801A5368;
    r0 = *(u32*)((u8*)r28 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_801A5368;
    r3 = *(u32*)((u8*)r28 + 0x1C);
    r4 = r1 + 0x5c;
    fn_80191688();
L_801A5368: ;
    r3 = r1 + 0x5c;
    r4 = r1 + 0x50;
    r5 = r1 + 0x74;
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
    r0 = -0x1;
    goto L_801A53E4;
L_801A53D4: ;
    r3 = r1 + 0x74;
    r4 = r1 + 0x74;
    fn_800A3ADC();
    r0 = 0x0;
L_801A53E4: ;
    if ((s32)r0 == (s32)0x0) goto L_801A5404;
    f2 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    f0 = *(f32*)lbl_8047DBE4;
    *(f32*)(sp + 0x74) = f2;
    *(f32*)(sp + 0x78) = f1;
    *(f32*)(sp + 0x7C) = f0;
L_801A5404: ;
    r3 = r31;
    r4 = r1 + 0x74;
    r5 = r1 + 0x74;
    fn_800A3820();
    r3 = r1 + 0x74;
    r4 = r1 + 0x74;
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
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x00000040;
    if ((s32)r0 == (s32)0x0) goto L_801A548C;
    f1 = *(f32*)((u8*)r28 + 0x20);
    r3 = r28 + 0x50;
    f2 = *(f32*)((u8*)r28 + 0x24);
    f3 = *(f32*)((u8*)r28 + 0x28);
    f4 = *(f32*)((u8*)r28 + 0x2C);
    f5 = *(f32*)((u8*)r28 + 0x30);
    f6 = *(f32*)((u8*)r28 + 0x34);
    fn_800BA198();
    goto L_801A54E8;
L_801A548C: ;
    f31 = *(f32*)((u8*)r28 + 0x2C);
    f0 = *(f64*)lbl_8047DBF0;
    f29 = *(f32*)((u8*)r28 + 0x28);
    f30 = *(f32*)((u8*)r28 + 0x20);
    r25 = *(u32*)((u8*)r28 + 0x24);
    r26 = *(u32*)((u8*)r28 + 0x30);
    if (f31 >= f0) goto L_801A54B0;
    f31 = *(f32*)lbl_8047DBEC;
L_801A54B0: ;
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
L_801A54E8: ;
    r0 = r29 & 0x14;
    if (f31 == f0) goto L_801A5594;
    r0 = *(u32*)((u8*)r28 + 0x10);
    r4 = r1 + 0x14;
    r3 = r28 + 0x50;
    *(u32*)(sp + 0x14) = r0;
    fn_800BA440();
    r0 = *(u32*)((u8*)r28 + 0x10);
    *(u32*)((u8*)r28 + 0x14) = r0;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 | 0x80;
    *(u16*)((u8*)r28 + 0x8) = r0;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x3;
    if ((s32)r0 == (s32)0x1) goto L_801A5558;
    if ((s32)r0 < (s32)0x1) goto L_801A5548;
    if ((s32)r0 >= (s32)0x4) goto L_801A5548;
    r3 = *(u32*)&lbl_8047B2C4;
    r0 = *(u32*)((u8*)r28 + 0x4C);
    r0 = r3 | r0;
    *(u32*)&lbl_8047B2C4 = r0;
    goto L_801A5558;
L_801A5548: ;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x298;
    r5 = (u32)lbl_8047DBD4;
    fn_80196E10();
L_801A5558: ;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x00000004;
    if ((s32)r0 == (s32)0x4) goto L_801A5574;
    r3 = *(u32*)&lbl_8047B2BC;
    r0 = *(u32*)((u8*)r28 + 0x4C);
    r0 = r3 | r0;
    *(u32*)&lbl_8047B2BC = r0;
L_801A5574: ;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x00000010;
    if ((s32)r0 == (s32)0x4) goto L_801A57CC;
    r3 = *(u32*)&lbl_8047B2C8;
    r0 = *(u32*)((u8*)r28 + 0x4C);
    r0 = r3 | r0;
    *(u32*)&lbl_8047B2C8 = r0;
    goto L_801A57CC;
L_801A5594: ;
    r0 = r29 & 0x00000008;
    if ((s32)r0 == (s32)0x4) goto L_801A57CC;
    r29 = *(u32*)((u8*)r28 + 0x4C);
    *(u32*)((u8*)r28 + 0x90) = r29;
    if ((s32)r29 == (s32)0x0) goto L_801A57CC;
    r0 = *(u32*)((u8*)r28 + 0x10);
    r4 = r1 + 0x10;
    r3 = r28 + 0x94;
    *(u32*)(sp + 0x10) = r0;
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
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 & 0x3;
    if ((s32)r0 == (s32)0x1) goto L_801A564C;
    if ((s32)r0 < (s32)0x1) goto L_801A57A4;
    if ((s32)r0 >= (s32)0x4) goto L_801A57A4;
    if ((u32)r28 == (u32)0x0) goto L_801A5638;
    r0 = *(u32*)((u8*)r28 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A5638;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = r28 + 0x3c;
    fn_80191688();
L_801A5638: ;
    r3 = r31;
    r4 = r28 + 0x3c;
    r5 = r28 + 0x3c;
    fn_800A37CC();
    goto L_801A57B4;
L_801A564C: ;
    r4 = (u32)lbl_80274D58;
    r8 = *(u32*)lbl_80274D58;
    r3 = (u32)lbl_80274D64;
    r5 = (u32)lbl_80274D64;
    r7 = *(u32*)((u8*)r4 + 0x4);
    r6 = *(u32*)((u8*)r4 + 0x8);
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x4C) = r0;
    if ((u32)r28 == (u32)0x0) goto L_801A5784;
    if ((u32)r28 == (u32)0x0) goto L_801A56B0;
    r0 = *(u32*)((u8*)r28 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A56B0;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = r1 + 0x38;
    fn_80191688();
L_801A56B0: ;
    if ((u32)r28 == (u32)0x0) goto L_801A56D0;
    r0 = *(u32*)((u8*)r28 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_801A56D0;
    r3 = *(u32*)((u8*)r28 + 0x1C);
    r4 = r1 + 0x44;
    fn_80191688();
L_801A56D0: ;
    r3 = r1 + 0x44;
    r4 = r1 + 0x38;
    r5 = r28 + 0x3c;
    fn_800A3A9C();
    /* addic. r0, r28, 0x3c */;
    if ((u32)r0 == (u32)0x0) goto L_801A56F0;
    /* addic. r0, r28, 0x3c */;
    if ((u32)r0 != (u32)0x0) goto L_801A56F8;
L_801A56F0: ;
    r0 = -0x1;
    goto L_801A5764;
L_801A56F8: ;
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
    r0 = -0x1;
    goto L_801A5764;
L_801A5754: ;
    r3 = r28 + 0x3c;
    r4 = r28 + 0x3c;
    fn_800A3ADC();
    r0 = 0x0;
L_801A5764: ;
    if ((s32)r0 == (s32)0x0) goto L_801A5784;
    f0 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    *(f32*)((u8*)r28 + 0x3C) = f0;
    f0 = *(f32*)lbl_8047DBE4;
    *(f32*)((u8*)r28 + 0x40) = f1;
    *(f32*)((u8*)r28 + 0x44) = f0;
L_801A5784: ;
    r3 = r31;
    r4 = r28 + 0x3c;
    r5 = r28 + 0x3c;
    fn_800A3820();
    r3 = r28 + 0x3c;
    r4 = r28 + 0x3c;
    fn_800A3ADC();
    goto L_801A57B4;
L_801A57A4: ;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x2c0;
    r5 = (u32)lbl_8047DBD4;
    fn_80196E10();
L_801A57B4: ;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r0 | 0x100;
    *(u16*)((u8*)r28 + 0x8) = r0;
    r0 = *(u32*)&lbl_8047B2C0;
    r0 = r0 | r29;
    *(u32*)&lbl_8047B2C0 = r0;
L_801A57CC: ;
    r27 = *(u32*)((u8*)r27 + 0x0);
L_801A57D0: ;
    if ((s32)r30 >= (s32)0x8) goto L_801A57E0;
    if ((u32)r27 != (u32)0x0) goto L_801A4FE4;
L_801A57E0: ;
    r0 = 0x8;
    if ((s32)r0 < (s32)0x0) goto L_801A580C;
    if ((s32)r0 >= (s32)0x9) goto L_801A580C;
    r3 = (u32)lbl_804655E0;
    r4 = r0 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    goto L_801A5810;
L_801A580C: ;
    r0 = 0x0;
L_801A5810: ;
    if ((u32)r0 != (u32)0x0) goto L_801A591C;
    goto L_801A5914;
L_801A581C: ;
    r3 = *(u32*)((u8*)r27 + 0x4);
    if ((u32)r3 == (u32)0x0) goto L_801A5910;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x00000020;
    if ((u32)r3 != (u32)0x0) goto L_801A5910;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x3;
    if ((u32)r3 != (u32)0x0) goto L_801A5910;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x14;
    if ((u32)r3 == (u32)0x0) goto L_801A5910;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x3;
    if ((u32)r3 != (u32)0x0) goto L_801A587C;
    r6 = 0x8;
    r4 = (u32)lbl_804655E0;
    r5 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r4 = r0 + r5;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_801A5888;
    goto L_801A591C;
L_801A587C: ;
    r6 = *(u32*)&lbl_8047B2B8;
    r0 = r6 + 0x1;
    *(u32*)&lbl_8047B2B8 = r0;
L_801A5888: ;
    r4 = (u32)lbl_804655E0;
    r5 = r6 << 2;
    r0 = (u32)lbl_804655E0;
    r4 = r0 + r5;
    *(u32*)((u8*)r4 + 0x0) = r3;
    if ((u32)r6 > (u32)0x8) goto L_801A5904;
    r4 = (u32)jumptable_8036CAAC;
    r0 = r6 << 2;
    r4 = (u32)jumptable_8036CAAC;
    r4 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r4;
    /* indirect jump via ctr */;
    r0 = 0x1;
    goto L_801A5908;
    r0 = 0x2;
    goto L_801A5908;
    r0 = 0x4;
    goto L_801A5908;
    r0 = 0x8;
    goto L_801A5908;
    r0 = 0x10;
    goto L_801A5908;
    r0 = 0x20;
    goto L_801A5908;
    r0 = 0x40;
    goto L_801A5908;
    r0 = 0x80;
    goto L_801A5908;
    r0 = 0x100;
    goto L_801A5908;
L_801A5904: ;
    r0 = 0x0;
L_801A5908: ;
    *(u32*)((u8*)r3 + 0x4C) = r0;
    goto L_801A591C;
L_801A5910: ;
    r27 = *(u32*)((u8*)r27 + 0x0);
L_801A5914: ;
    if ((u32)r27 != (u32)0x0) goto L_801A581C;
L_801A591C: ;
    r27 = *(u32*)&lbl_8047B2B8;
    r28 = 0x0;
    goto L_801A5C14;
L_801A5928: ;
    if ((s32)r28 < (s32)0x0) goto L_801A5950;
    if ((s32)r28 >= (s32)0x8) goto L_801A5950;
    r3 = (u32)lbl_804655E0;
    r4 = r28 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    goto L_801A5954;
L_801A5950: ;
    r0 = 0x0;
L_801A5954: ;
    /* mr. r26, r0 */;
    if ((s32)r28 == (s32)0x8) goto L_801A5C10;
    r3 = *(u16*)((u8*)r26 + 0x8);
    r0 = r3 & 0x00000008;
    if ((s32)r28 == (s32)0x8) goto L_801A5C10;
    r0 = r3 & 0x14;
    if ((s32)r28 == (s32)0x8) goto L_801A5C10;
    r0 = r30;
    r30 = r30 + 0x1;
    if ((u32)r0 > (u32)0x8) goto L_801A59E0;
    r3 = (u32)jumptable_8036CA88;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8036CA88;
    r3 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r3;
    /* indirect jump via ctr */;
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
L_801A59E0: ;
    r29 = 0x0;
L_801A59E4: ;
    *(u32*)((u8*)r26 + 0x90) = r29;
    if ((s32)r29 == (s32)0x0) goto L_801A5C10;
    r0 = *(u32*)((u8*)r26 + 0x10);
    r4 = r1 + 0xc;
    r3 = r26 + 0x94;
    *(u32*)(sp + 0xC) = r0;
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
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 & 0x3;
    if ((s32)r0 == (s32)0x1) goto L_801A5A90;
    if ((s32)r0 < (s32)0x1) goto L_801A5BE8;
    if ((s32)r0 >= (s32)0x4) goto L_801A5BE8;
    if ((u32)r26 == (u32)0x0) goto L_801A5A7C;
    r0 = *(u32*)((u8*)r26 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A5A7C;
    r3 = *(u32*)((u8*)r26 + 0x18);
    r4 = r26 + 0x3c;
    fn_80191688();
L_801A5A7C: ;
    r3 = r31;
    r4 = r26 + 0x3c;
    r5 = r26 + 0x3c;
    fn_800A37CC();
    goto L_801A5BF8;
L_801A5A90: ;
    r4 = (u32)lbl_80274D58;
    r8 = *(u32*)lbl_80274D58;
    r3 = (u32)lbl_80274D64;
    r5 = (u32)lbl_80274D64;
    r7 = *(u32*)((u8*)r4 + 0x4);
    r6 = *(u32*)((u8*)r4 + 0x8);
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x34) = r0;
    if ((u32)r26 == (u32)0x0) goto L_801A5BC8;
    if ((u32)r26 == (u32)0x0) goto L_801A5AF4;
    r0 = *(u32*)((u8*)r26 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A5AF4;
    r3 = *(u32*)((u8*)r26 + 0x18);
    r4 = r1 + 0x20;
    fn_80191688();
L_801A5AF4: ;
    if ((u32)r26 == (u32)0x0) goto L_801A5B14;
    r0 = *(u32*)((u8*)r26 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_801A5B14;
    r3 = *(u32*)((u8*)r26 + 0x1C);
    r4 = r1 + 0x2c;
    fn_80191688();
L_801A5B14: ;
    r3 = r1 + 0x2c;
    r4 = r1 + 0x20;
    r5 = r26 + 0x3c;
    fn_800A3A9C();
    /* addic. r0, r26, 0x3c */;
    if ((u32)r0 == (u32)0x0) goto L_801A5B34;
    /* addic. r0, r26, 0x3c */;
    if ((u32)r0 != (u32)0x0) goto L_801A5B3C;
L_801A5B34: ;
    r0 = -0x1;
    goto L_801A5BA8;
L_801A5B3C: ;
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
    r0 = -0x1;
    goto L_801A5BA8;
L_801A5B98: ;
    r3 = r26 + 0x3c;
    r4 = r26 + 0x3c;
    fn_800A3ADC();
    r0 = 0x0;
L_801A5BA8: ;
    if ((s32)r0 == (s32)0x0) goto L_801A5BC8;
    f0 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    *(f32*)((u8*)r26 + 0x3C) = f0;
    f0 = *(f32*)lbl_8047DBE4;
    *(f32*)((u8*)r26 + 0x40) = f1;
    *(f32*)((u8*)r26 + 0x44) = f0;
L_801A5BC8: ;
    r3 = r31;
    r4 = r26 + 0x3c;
    r5 = r26 + 0x3c;
    fn_800A3820();
    r3 = r26 + 0x3c;
    r4 = r26 + 0x3c;
    fn_800A3ADC();
    goto L_801A5BF8;
L_801A5BE8: ;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x2c0;
    r5 = (u32)lbl_8047DBD4;
    fn_80196E10();
L_801A5BF8: ;
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 | 0x100;
    *(u16*)((u8*)r26 + 0x8) = r0;
    r0 = *(u32*)&lbl_8047B2C0;
    r0 = r0 | r29;
    *(u32*)&lbl_8047B2C0 = r0;
L_801A5C10: ;
    r28 = r28 + 0x1;
L_801A5C14: ;
    if ((s32)r30 >= (s32)0x8) goto L_801A5C24;
    if ((s32)r28 < (s32)r27) goto L_801A5928;
L_801A5C24: ;
    r28 = 0x0;
    goto L_801A5D98;
L_801A5C2C: ;
    if ((s32)r28 < (s32)0x0) goto L_801A5C54;
    if ((s32)r28 >= (s32)0x8) goto L_801A5C54;
    r3 = (u32)lbl_804655E0;
    r4 = r28 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    goto L_801A5C58;
L_801A5C54: ;
    r0 = 0x0;
L_801A5C58: ;
    /* mr. r26, r0 */;
    if ((s32)r28 == (s32)0x8) goto L_801A5D94;
    r0 = *(u16*)((u8*)r26 + 0x8);
    r3 = *(u32*)((u8*)r26 + 0x10);
    r0 = r0 & 0x00000020;
    f29 = *(f32*)((u8*)r26 + 0x38);
    if ((s32)r28 != (s32)0x8) goto L_801A5D94;
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 & 0x3;
    if ((s32)r28 == (s32)0x8) goto L_801A5D94;
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 & 0x14;
    if ((s32)r28 == (s32)0x8) goto L_801A5D1C;
    r3 = *(u8*)((u8*)r26 + 0x14);
    r0 = *(u8*)(sp + 0x1C);
    if ((u32)r3 != (u32)r0) goto L_801A5CD0;
    r3 = *(u8*)((u8*)r26 + 0x15);
    r0 = *(u8*)(sp + 0x1D);
    if ((u32)r3 != (u32)r0) goto L_801A5CD0;
    r3 = *(u8*)((u8*)r26 + 0x16);
    r0 = *(u8*)(sp + 0x1E);
    if ((u32)r3 != (u32)r0) goto L_801A5CD0;
    r3 = *(u8*)((u8*)r26 + 0x17);
    r0 = *(u8*)(sp + 0x1F);
    if ((u32)r3 == (u32)r0) goto L_801A5CF8;
L_801A5CD0: ;
    r0 = *(u32*)(sp + 0x1C);
    r4 = r1 + 0x8;
    r3 = r26 + 0x50;
    *(u32*)(sp + 0x8) = r0;
    fn_800BA440();
    r0 = *(u32*)(sp + 0x1C);
    *(u32*)((u8*)r26 + 0x14) = r0;
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 | 0x80;
    *(u16*)((u8*)r26 + 0x8) = r0;
L_801A5CF8: ;
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 & 0x00000080;
    if ((u32)r3 == (u32)r0) goto L_801A5D1C;
    r4 = *(u32*)((u8*)r26 + 0x4C);
    r3 = r26 + 0x50;
    fn_800BA44C();
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 & 0xFFFFFF7F;
    *(u16*)((u8*)r26 + 0x8) = r0;
L_801A5D1C: ;
    r0 = *(u32*)((u8*)r26 + 0x90);
    if ((s32)r0 == (s32)0x0) goto L_801A5D94;
    f0 = *(f32*)((u8*)r26 + 0x38);
    if (f0 == f29) goto L_801A5D70;
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
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 | 0x100;
    *(u16*)((u8*)r26 + 0x8) = r0;
L_801A5D70: ;
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 & 0x00000100;
    if (f0 == f29) goto L_801A5D94;
    r4 = *(u32*)((u8*)r26 + 0x90);
    r3 = r26 + 0x94;
    fn_800BA44C();
    r0 = *(u16*)((u8*)r26 + 0x8);
    r0 = r0 & 0xFFFFFEFF;
    *(u16*)((u8*)r26 + 0x8) = r0;
L_801A5D94: ;
    r28 = r28 + 0x1;
L_801A5D98: ;
    if ((s32)r28 < (s32)r27) goto L_801A5C2C;
    /* psq_l f31, 0xe8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0xE0);
    /* psq_l f30, 0xd8(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0xD0);
    /* psq_l f29, 0xc8(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0xC0);
    /* lmw r25, 0xa4(r1) */;
    return;
}
#pragma pop

/* 0x801A5DCC | 0x2CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    r0 = -0x1;
    goto L_801A5E68;
L_801A5E58: ;
    r3 = r1 + 0x20;
    r4 = r1 + 0x2c;
    fn_800A3ADC();
    r0 = 0x0;
L_801A5E68: ;
    if ((s32)r0 == (s32)0x0) goto L_801A5E88;
    f2 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    f0 = *(f32*)lbl_8047DC00;
    *(f32*)(sp + 0x2C) = f2;
    *(f32*)(sp + 0x30) = f1;
    *(f32*)(sp + 0x34) = f0;
L_801A5E88: ;
    r30 = *(u32*)&lbl_8047B2B8;
    r31 = 0x0;
    goto L_801A6074;
L_801A5E94: ;
    if ((s32)r31 < (s32)0x0) goto L_801A5EBC;
    if ((s32)r31 >= (s32)0x8) goto L_801A5EBC;
    r3 = (u32)lbl_804655E0;
    r4 = r31 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    goto L_801A5EC0;
L_801A5EBC: ;
    r0 = 0x0;
L_801A5EC0: ;
    r29 = r0;
    r0 = *(u32*)((u8*)r29 + 0x90);
    if ((s32)r0 == (s32)0x0) goto L_801A6070;
    r0 = *(u16*)((u8*)r29 + 0x8);
    r0 = r0 & 0x3;
    if ((s32)r0 == (s32)0x1) goto L_801A5FA0;
    if ((s32)r0 < (s32)0x1) goto L_801A5FB4;
    if ((s32)r0 >= (s32)0x4) goto L_801A5FB4;
    r3 = r1 + 0x20;
    r4 = r29 + 0x3c;
    r5 = r1 + 0x8;
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
    r0 = -0x1;
    goto L_801A5F68;
L_801A5F58: ;
    r3 = r1 + 0x8;
    r4 = r1 + 0x8;
    fn_800A3ADC();
    r0 = 0x0;
L_801A5F68: ;
    if ((s32)r0 != (s32)0x0) goto L_801A5F84;
    r3 = r1 + 0x8;
    r4 = r1 + 0x2c;
    r5 = r1 + 0x14;
    fn_800A3A78();
    goto L_801A5FC4;
L_801A5F84: ;
    f2 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    f0 = *(f32*)lbl_8047DBE4;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f1;
    *(f32*)(sp + 0x1C) = f0;
    goto L_801A6050;
L_801A5FA0: ;
    r3 = r29 + 0x3c;
    r4 = r1 + 0x2c;
    r5 = r1 + 0x14;
    fn_800A3A78();
    goto L_801A5FC4;
L_801A5FB4: ;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x27a;
    r5 = (u32)lbl_8047DBD4;
    fn_80196E10();
L_801A5FC4: ;
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
    r0 = -0x1;
    goto L_801A6030;
L_801A6020: ;
    r3 = r1 + 0x14;
    r4 = r1 + 0x14;
    fn_800A3ADC();
    r0 = 0x0;
L_801A6030: ;
    if ((s32)r0 == (s32)0x0) goto L_801A6050;
    f2 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    f0 = *(f32*)lbl_8047DBE4;
    *(f32*)(sp + 0x14) = f2;
    *(f32*)(sp + 0x18) = f1;
    *(f32*)(sp + 0x1C) = f0;
L_801A6050: ;
    f1 = *(f32*)(sp + 0x14);
    r3 = r29 + 0x94;
    f2 = *(f32*)(sp + 0x18);
    f3 = *(f32*)(sp + 0x1C);
    fn_800BA424();
    r0 = *(u16*)((u8*)r29 + 0x8);
    r0 = r0 | 0x100;
    *(u16*)((u8*)r29 + 0x8) = r0;
L_801A6070: ;
    r31 = r31 + 0x1;
L_801A6074: ;
    if ((s32)r31 < (s32)r30) goto L_801A5E94;
    r31 = *(u32*)(sp + 0x4C);
    r30 = *(u32*)(sp + 0x48);
    r29 = *(u32*)(sp + 0x44);
    return;
}
#pragma pop

/* 0x801A6098 | 0x174 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6098(void) {
    extern u8 lbl_8047DBE4[];
    extern u8 lbl_8047DBE8[];
    extern u8 lbl_8047DBFC[];
    extern void fn_800BA198();
    extern void fn_800BA440();
    extern void fn_800BA44C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28(r1), 0, qr0 */;
    r0 = *(u16*)((u8*)r3 + 0x8);
    f31 = f1;
    r30 = r3;
    r31 = r4;
    r0 = r0 & 0x00000020;
    if ((s32)r0 != (s32)0) goto L_801A61EC;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 & 0x3;
    if ((s32)r0 != (s32)0) goto L_801A60DC;
    goto L_801A61EC;
L_801A60DC: ;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 & 0x14;
    if ((s32)r0 == (s32)0) goto L_801A6174;
    r3 = *(u8*)((u8*)r30 + 0x14);
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_801A6128;
    r3 = *(u8*)((u8*)r30 + 0x15);
    r0 = *(u8*)((u8*)r31 + 0x1);
    if ((u32)r3 != (u32)r0) goto L_801A6128;
    r3 = *(u8*)((u8*)r30 + 0x16);
    r0 = *(u8*)((u8*)r31 + 0x2);
    if ((u32)r3 != (u32)r0) goto L_801A6128;
    r3 = *(u8*)((u8*)r30 + 0x17);
    r0 = *(u8*)((u8*)r31 + 0x3);
    if ((u32)r3 == (u32)r0) goto L_801A6150;
L_801A6128: ;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r4 = r1 + 0x8;
    r3 = r30 + 0x50;
    *(u32*)(sp + 0x8) = r0;
    fn_800BA440();
    r0 = *(u32*)((u8*)r31 + 0x0);
    *(u32*)((u8*)r30 + 0x14) = r0;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 | 0x80;
    *(u16*)((u8*)r30 + 0x8) = r0;
L_801A6150: ;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 & 0x00000080;
    if ((u32)r3 == (u32)r0) goto L_801A6174;
    r4 = *(u32*)((u8*)r30 + 0x4C);
    r3 = r30 + 0x50;
    fn_800BA44C();
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 & 0xFFFFFF7F;
    *(u16*)((u8*)r30 + 0x8) = r0;
L_801A6174: ;
    r0 = *(u32*)((u8*)r30 + 0x90);
    if ((s32)r0 == (s32)0x0) goto L_801A61EC;
    f0 = *(f32*)((u8*)r30 + 0x38);
    if (f0 == f31) goto L_801A61C8;
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
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 | 0x100;
    *(u16*)((u8*)r30 + 0x8) = r0;
L_801A61C8: ;
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 & 0x00000100;
    if (f0 == f31) goto L_801A61EC;
    r4 = *(u32*)((u8*)r30 + 0x90);
    r3 = r30 + 0x94;
    fn_800BA44C();
    r0 = *(u16*)((u8*)r30 + 0x8);
    r0 = r0 & 0xFFFFFEFF;
    *(u16*)((u8*)r30 + 0x8) = r0;
L_801A61EC: ;
    /* psq_l f31, 0x28(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* 0x801A620C | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x10) = r0;
    if ((s32)r0 == (s32)0) goto L_801A6358;
    if ((u32)r30 == (u32)0x0) goto L_801A6284;
    r0 = *(u32*)((u8*)r30 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_801A6284;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r1 + 0x14;
    fn_80191688();
L_801A6284: ;
    if ((u32)r30 == (u32)0x0) goto L_801A62A4;
    r0 = *(u32*)((u8*)r30 + 0x1C);
    if ((u32)r0 == (u32)0x0) goto L_801A62A4;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = r1 + 0x8;
    fn_80191688();
L_801A62A4: ;
    r5 = r31;
    r3 = r1 + 0x8;
    r4 = r1 + 0x14;
    fn_800A3A9C();
    if ((u32)r31 == (u32)0x0) goto L_801A62C4;
    if ((u32)r31 != (u32)0x0) goto L_801A62CC;
L_801A62C4: ;
    r0 = -0x1;
    goto L_801A6338;
L_801A62CC: ;
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
    r0 = -0x1;
    goto L_801A6338;
L_801A6328: ;
    r3 = r31;
    r4 = r31;
    fn_800A3ADC();
    r0 = 0x0;
L_801A6338: ;
    if ((s32)r0 == (s32)0x0) goto L_801A6358;
    f0 = *(f32*)lbl_8047DBE8;
    f1 = *(f32*)lbl_8047DBE8;
    *(f32*)((u8*)r31 + 0x0) = f0;
    f0 = *(f32*)lbl_8047DBE4;
    *(f32*)((u8*)r31 + 0x4) = f1;
    *(f32*)((u8*)r31 + 0x8) = f0;
L_801A6358: ;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    return;
}
#pragma pop

/* 0x801A6370 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6370(void) {
    extern void fn_80191E38();
    extern void fn_801C29C4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x18) = f31;
    f31 = f1;
    if ((u32)r3 == (u32)0x0) goto L_801A63F0;
    r31 = r3;
    goto L_801A63E8;
L_801A6398: ;
    if ((u32)r31 == (u32)0x0) goto L_801A63E4;
    f1 = f31;
    r3 = *(u32*)((u8*)r31 + 0x48);
    fn_801C29C4();
    if ((u32)r31 == (u32)0x0) goto L_801A63BC;
    r3 = *(u32*)((u8*)r31 + 0x18);
    goto L_801A63C0;
L_801A63BC: ;
    r3 = 0x0;
L_801A63C0: ;
    f1 = f31;
    fn_80191E38();
    if ((u32)r31 == (u32)0x0) goto L_801A63D8;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    goto L_801A63DC;
L_801A63D8: ;
    r3 = 0x0;
L_801A63DC: ;
    f1 = f31;
    fn_80191E38();
L_801A63E4: ;
    r31 = *(u32*)((u8*)r31 + 0xC);
L_801A63E8: ;
    if ((u32)r31 != (u32)0x0) goto L_801A6398;
L_801A63F0: ;
    f31 = *(f64*)(sp + 0x18);
    r31 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x801A6408 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6408(void) {
    extern void fn_801919EC();
    extern void fn_801C27F4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    if ((u32)r3 == (u32)0x0) goto L_801A6480;
    r31 = r3;
    goto L_801A6478;
L_801A6428: ;
    if ((u32)r31 == (u32)0x0) goto L_801A6474;
    r5 = *(u32*)((u8*)r31 + 0x0);
    r4 = r31;
    r3 = *(u32*)((u8*)r31 + 0x48);
    r5 = *(u32*)((u8*)r5 + 0x40);
    fn_801C27F4();
    if ((u32)r31 == (u32)0x0) goto L_801A6454;
    r3 = *(u32*)((u8*)r31 + 0x18);
    goto L_801A6458;
L_801A6454: ;
    r3 = 0x0;
L_801A6458: ;
    fn_801919EC();
    if ((u32)r31 == (u32)0x0) goto L_801A646C;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    goto L_801A6470;
L_801A646C: ;
    r3 = 0x0;
L_801A6470: ;
    fn_801919EC();
L_801A6474: ;
    r31 = *(u32*)((u8*)r31 + 0xC);
L_801A6478: ;
    if ((u32)r31 != (u32)0x0) goto L_801A6428;
L_801A6480: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801A6494 | 0x24C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6494(void) {
    extern u8 lbl_8047DBE4[];
    extern u8 lbl_8047DBE8[];
    extern u8 lbl_8047DC08[];
    extern u8 lbl_8047DC10[];
    extern u8 jumptable_8036CAF4[];
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    if ((u32)r3 == (u32)0x0) goto L_801A66D8;
    /* subi r0, r4, 0x9 */;
    if ((u32)r0 > (u32)0xd) goto L_801A66D8;
    r4 = (u32)jumptable_8036CAF4;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8036CAF4;
    r4 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r4;
    /* indirect jump via ctr */;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f64*)lbl_8047DC08;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A64E8;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0xFFFFFFDF;
    *(u16*)((u8*)r3 + 0x8) = r0;
    goto L_801A66D8;
L_801A64E8: ;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 | 0x20;
    *(u16*)((u8*)r3 + 0x8) = r0;
    goto L_801A66D8;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x00000040;
    if (f1 == f0) goto L_801A6510;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x20) = f0;
    goto L_801A66D8;
L_801A6510: ;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x20) = f0;
    goto L_801A66D8;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x00000040;
    if (f1 == f0) goto L_801A6534;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x24) = f0;
    goto L_801A66D8;
L_801A6534: ;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x2C) = f0;
    goto L_801A66D8;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x00000040;
    if (f1 == f0) goto L_801A6558;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x28) = f0;
    goto L_801A66D8;
L_801A6558: ;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x28) = f0;
    goto L_801A66D8;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x00000040;
    if (f1 == f0) goto L_801A66D8;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x2C) = f0;
    goto L_801A66D8;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x00000040;
    if (f1 == f0) goto L_801A66D8;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x30) = f0;
    goto L_801A66D8;
    r0 = *(u16*)((u8*)r3 + 0x8);
    r0 = r0 & 0x00000040;
    if (f1 == f0) goto L_801A66D8;
    f0 = *(f32*)((u8*)r5 + 0x0);
    *(f32*)((u8*)r3 + 0x34) = f0;
    goto L_801A66D8;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A65C8;
    f1 = *(f32*)lbl_8047DBE8;
    goto L_801A65DC;
L_801A65C8: ;
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A65DC;
    f1 = *(f32*)lbl_8047DBE4;
L_801A65DC: ;
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x10) = r0;
    goto L_801A66D8;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A6614;
    f1 = *(f32*)lbl_8047DBE8;
    goto L_801A6628;
L_801A6614: ;
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A6628;
    f1 = *(f32*)lbl_8047DBE4;
L_801A6628: ;
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x11) = r0;
    goto L_801A66D8;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A6660;
    f1 = *(f32*)lbl_8047DBE8;
    goto L_801A6674;
L_801A6660: ;
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A6674;
    f1 = *(f32*)lbl_8047DBE4;
L_801A6674: ;
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x12) = r0;
    goto L_801A66D8;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DBE8;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A66AC;
    f1 = *(f32*)lbl_8047DBE8;
    goto L_801A66C0;
L_801A66AC: ;
    f0 = *(f32*)lbl_8047DBE4;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A66C0;
    f1 = *(f32*)lbl_8047DBE4;
L_801A66C0: ;
    f0 = *(f32*)lbl_8047DC10;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x8) = f0;
    *(u8*)((u8*)r3 + 0x13) = r0;
L_801A66D8: ;
    return;
}
#pragma pop

/* 0x801A66E0 | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A66E0(void) {
    extern void fn_80191E88();
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if ((u32)r3 == (u32)0x0) goto L_801A6774;
    r30 = r3;
    goto L_801A676C;
L_801A6704: ;
    if ((u32)r30 == (u32)0x0) goto L_801A6768;
    r31 = r30;
    goto L_801A6760;
L_801A6714: ;
    if ((u32)r31 == (u32)0x0) goto L_801A675C;
    r3 = *(u32*)((u8*)r31 + 0x48);
    fn_801C25E4();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x48) = r0;
    if ((u32)r31 == (u32)0x0) goto L_801A673C;
    r3 = *(u32*)((u8*)r31 + 0x18);
    goto L_801A6740;
L_801A673C: ;
    r3 = 0x0;
L_801A6740: ;
    fn_80191E88();
    if ((u32)r31 == (u32)0x0) goto L_801A6754;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    goto L_801A6758;
L_801A6754: ;
    r3 = 0x0;
L_801A6758: ;
    fn_80191E88();
L_801A675C: ;
    r31 = *(u32*)((u8*)r31 + 0xC);
L_801A6760: ;
    if ((u32)r31 != (u32)0x0) goto L_801A6714;
L_801A6768: ;
    r30 = *(u32*)((u8*)r30 + 0xC);
L_801A676C: ;
    if ((u32)r30 != (u32)0x0) goto L_801A6704;
L_801A6774: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801A678C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A678C(void) {
    extern u8 lbl_804655E0[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((s32)r3 < (s32)0x0) goto L_801A67B4;
    if ((s32)r3 >= (s32)0x8) goto L_801A67B4;
    r4 = (u32)lbl_804655E0;
    r3 = r3 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r3;
    r3 = *(u32*)((u8*)r3 + 0x0);
    return;
L_801A67B4: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* 0x801A67BC | 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A67BC(void) {
    extern u8 lbl_804655E0[];
    extern u8 lbl_8047DBB8[];
    extern u8 lbl_8047DBD4[];
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    if ((s32)r3 == (s32)0x10) goto L_801A6858;
    if ((s32)r3 >= (s32)0x10) goto L_801A6808;
    if ((s32)r3 == (s32)0x4) goto L_801A6848;
    if ((s32)r3 >= (s32)0x4) goto L_801A67FC;
    if ((s32)r3 == (s32)0x2) goto L_801A6840;
    if ((s32)r3 >= (s32)0x2) goto L_801A6880;
    if ((s32)r3 >= (s32)0x1) goto L_801A6838;
    goto L_801A6880;
L_801A67FC: ;
    if ((s32)r3 == (s32)0x8) goto L_801A6850;
    goto L_801A6880;
L_801A6808: ;
    if ((s32)r3 == (s32)0x80) goto L_801A6870;
    if ((s32)r3 >= (s32)0x80) goto L_801A682C;
    if ((s32)r3 == (s32)0x40) goto L_801A6868;
    if ((s32)r3 >= (s32)0x40) goto L_801A6880;
    if ((s32)r3 == (s32)0x20) goto L_801A6860;
    goto L_801A6880;
L_801A682C: ;
    if ((s32)r3 == (s32)0x100) goto L_801A6878;
    goto L_801A6880;
L_801A6838: ;
    r31 = 0x0;
    goto L_801A6890;
L_801A6840: ;
    r31 = 0x1;
    goto L_801A6890;
L_801A6848: ;
    r31 = 0x2;
    goto L_801A6890;
L_801A6850: ;
    r31 = 0x3;
    goto L_801A6890;
L_801A6858: ;
    r31 = 0x4;
    goto L_801A6890;
L_801A6860: ;
    r31 = 0x5;
    goto L_801A6890;
L_801A6868: ;
    r31 = 0x6;
    goto L_801A6890;
L_801A6870: ;
    r31 = 0x7;
    goto L_801A6890;
L_801A6878: ;
    r31 = 0x8;
    goto L_801A6890;
L_801A6880: ;
    r3 = (u32)lbl_8047DBB8;
    r4 = 0x4a1;
    r5 = (u32)lbl_8047DBD4;
    fn_80196E10();
L_801A6890: ;
    /* mr. r0, r31 */;
    if ((s32)r3 < (s32)0x100) goto L_801A68B8;
    if ((s32)r0 >= (s32)0x9) goto L_801A68B8;
    r3 = (u32)lbl_804655E0;
    r4 = r0 << 2;
    r0 = (u32)lbl_804655E0;
    r3 = r0 + r4;
    r3 = *(u32*)((u8*)r3 + 0x0);
    goto L_801A68BC;
L_801A68B8: ;
    r3 = 0x0;
L_801A68BC: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6928(void) {
    extern u8 lbl_80465608[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
#pragma pop

/* 0x801A6960 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6960(void) {
    extern u8 lbl_80465608[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
#pragma pop

/* 0x801A6990 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6990(void) {
    extern u8 lbl_80465608[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
#pragma pop
