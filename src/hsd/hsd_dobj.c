/**
 * @file hsd_dobj.c
 * @brief HSD DObj - Display object implementation.
 *
 * Colosseum address: 0x80198F7C (HSD_DObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/dobj.c
 */

#include "hsd/hsd_dobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_pobj.h"

static void DObjInfoInit(void);

static HSD_DObjInfo hsdDObj = { DObjInfoInit };
static HSD_ClassInfo* default_class = NULL;

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_DObjGetFlags(HSD_DObj* dobj)
{
    HSD_ASSERT(0, dobj);
    return dobj->flags;
}

void HSD_DObjSetFlags(HSD_DObj* dobj, u32 flags)
{
    HSD_ASSERT(0, dobj);
    dobj->flags |= flags;
}

void HSD_DObjClearFlags(HSD_DObj* dobj, u32 flags)
{
    HSD_ASSERT(0, dobj);
    dobj->flags &= ~flags;
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_DObjAddAnim(HSD_DObj* dobj, HSD_MatAnim* mat_anim,
                     HSD_ShapeAnimDObj* sh_anim)
{
    if (dobj == NULL) {
        return;
    }
    HSD_MObjAddAnim(dobj->mobj, mat_anim);
    if (sh_anim != NULL) {
        HSD_PObjAddAnim(dobj->pobj, sh_anim->shapeanim);
    }
}

void HSD_DObjAddAnimAll(HSD_DObj* dobj, HSD_MatAnim* matanim,
                        HSD_ShapeAnimDObj* shapeanimdobj)
{
    HSD_DObj* d;
    HSD_MatAnim* ma;
    HSD_ShapeAnimDObj* sa;

    d = dobj;
    ma = matanim;
    sa = shapeanimdobj;

    while (d != NULL) {
        HSD_DObjAddAnim(d, ma, sa);
        d = d->next;
        if (ma != NULL) ma = ma->next;
        if (sa != NULL) sa = sa->next;
    }
}

void HSD_DObjReqAnimAll(HSD_DObj* dobj, f32 startframe)
{
    HSD_DObj* d;
    for (d = dobj; d != NULL; d = d->next) {
        HSD_MObjReqAnim(d->mobj, startframe);
        /* PObj shape anim req would go here */
    }
}

void HSD_DObjAnim(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        HSD_MObjAnim(dobj->mobj);
        HSD_PObjAnim(dobj->pobj);
    }
}

void HSD_DObjAnimAll(HSD_DObj* dobj)
{
    HSD_DObj* d;
    for (d = dobj; d != NULL; d = d->next) {
        HSD_DObjAnim(d);
    }
}

/* ========================================================================= */
/*  Load                                                                     */
/* ========================================================================= */

static int DObjLoad(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    if (dobj->mobj != NULL) {
        HSD_MObjRemove(dobj->mobj);
    }
    dobj->mobj = HSD_MObjLoadDesc(desc->mobjdesc);
    if (dobj->pobj != NULL) {
        HSD_PObjRemoveAll(dobj->pobj);
    }
    dobj->pobj = HSD_PObjLoadDesc(desc->pobjdesc);
    return 0;
}

HSD_DObj* HSD_DObjLoadDesc(HSD_DObjDesc* desc)
{
    HSD_DObj* dobj = NULL;
    HSD_DObj* first = NULL;
    HSD_DObj* prev = NULL;
    HSD_DObjDesc* d;

    for (d = desc; d != NULL; d = d->next) {
        HSD_ClassInfo* info;
        if (d->class_name == NULL ||
            !(info = hsdSearchClassInfo(d->class_name)))
        {
            dobj = HSD_DObjAlloc();
        } else {
            dobj = hsdNew(info);
            HSD_ASSERT(0, dobj);
        }
        HSD_DOBJ_METHOD(dobj)->load(dobj, d);
        if (prev != NULL) {
            prev->next = dobj;
        } else {
            first = dobj;
        }
        prev = dobj;
    }
    return first;
}

/* ========================================================================= */
/*  Resolve refs                                                             */
/* ========================================================================= */

void HSD_DObjResolveRefs(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    if (dobj != NULL && desc != NULL) {
        HSD_PObjResolveRefsAll(dobj->pobj, desc->pobjdesc);
    }
}

void HSD_DObjResolveRefsAll(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    HSD_DObj* d;
    HSD_DObjDesc* dd;

    d = dobj;
    dd = desc;
    while (d != NULL && dd != NULL) {
        HSD_DObjResolveRefs(d, dd);
        d = d->next;
        dd = dd->next;
    }
}

/* ========================================================================= */
/*  Remove                                                                   */
/* ========================================================================= */

void HSD_DObjRemove(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        HSD_CLASS_METHOD(dobj)->release((HSD_Class*) dobj);
        HSD_CLASS_METHOD(dobj)->destroy((HSD_Class*) dobj);
    }
}

void HSD_DObjRemoveAll(HSD_DObj* dobj)
{
    HSD_DObj* next;
    while (dobj != NULL) {
        next = dobj->next;
        HSD_DObjRemove(dobj);
        dobj = next;
    }
}

/* ========================================================================= */
/*  Alloc                                                                    */
/* ========================================================================= */

HSD_DObj* HSD_DObjAlloc(void)
{
    HSD_DObj* dobj;
    dobj = (HSD_DObj*) hsdNew(
        default_class ? default_class : (HSD_ClassInfo*) &hsdDObj);
    HSD_ASSERT(0, dobj);
    return dobj;
}

void HSD_DObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        HSD_ASSERT(0, hsdIsDescendantOf(info, &hsdDObj));
    }
    default_class = info;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void DObjRelease(HSD_Class* o)
{
    HSD_DObj* dobj = (HSD_DObj*) o;
    HSD_MObjRemove(dobj->mobj);
    HSD_PObjRemoveAll(dobj->pobj);
    HSD_AObjRemove(dobj->aobj);
    HSD_PARENT_INFO(&hsdDObj)->release(o);
}

static void DObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdDObj)->amnesia(info);
}

static void DObjInfoInit(void)
{
    hsdInitClassInfo((HSD_ClassInfo*) &hsdDObj, &hsdClass,
                     "sysdolphin_base_library", "hsd_dobj",
                     sizeof(HSD_DObjInfo), sizeof(HSD_DObj));
    ((HSD_ClassInfo*) &hsdDObj)->release = DObjRelease;
    ((HSD_ClassInfo*) &hsdDObj)->amnesia = DObjAmnesia;
    hsdDObj.load = DObjLoad;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

extern u32 lbl_8047B264;

/* Address: 0x80199704 | Size: 0x8 | Pattern: sda_setter */
void fn_80199704(u32 val) {
    lbl_8047B264 = val;
}

/* =========================================================================
 *  Internal stubs: 0x80198F4C-0x80199A84 (16 functions)
 * ========================================================================= */

/* 0x80198F4C | 0x30 */
void fn_80198F4C(void) {
    extern u8 lbl_80465348[];
    extern void fn_801AA35C();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r3 = (u32)lbl_80465348;
    r4 = 0x48;
    r3 = (u32)lbl_80465348;
    r5 = 0x4;
    fn_801AA35C();
    return;
}

/* 0x80198F7C | 0x98 */
void fn_80198F7C(void) {
    extern u8 lbl_80274708[];
    extern u8 lbl_80274720[];
    extern u8 lbl_8036C638[];
    extern u8 lbl_8036C7A0[];
    extern void fn_80193B30();
    extern void fn_80199014();
    extern void fn_8019905C();
    extern void fn_801990B8();
    extern void fn_801993A4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;

    r3 = (u32)lbl_8036C7A0;
    r4 = (u32)lbl_8036C638;
    r5 = (u32)lbl_80274708;
    r6 = (u32)lbl_80274720;
    r3 = (u32)lbl_8036C7A0;
    r4 = (u32)lbl_8036C638;
    r5 = (u32)lbl_80274708;
    r6 = (u32)lbl_80274720;
    r7 = 0x48;
    r8 = 0x18;
    fn_80193B30();
    r6 = (u32)fn_8019905C;
    r5 = (u32)fn_80199014;
    r3 = (u32)fn_801993A4;
    r8 = (u32)lbl_8036C7A0;
    r9 = (u32)fn_8019905C;
    r6 = (u32)lbl_8036C7A0;
    tmp = (u32)fn_801993A4;
    r3 = (u32)lbl_8036C7A0;
    r8 = (u32)lbl_8036C7A0;
    r4 = (u32)fn_801990B8;
    r7 = (u32)fn_80199014;
    r6 = (u32)lbl_8036C7A0;
    r5 = (u32)fn_801990B8;
    r4 = (u32)lbl_8036C7A0;
    r4 = (u32)lbl_8036C7A0;
    r3 = (u32)lbl_8036C7A0;
    *(u32*)((u8*)r8 + 0x30) = r9;
    *(u32*)((u8*)r6 + 0x38) = r7;
    *(u32*)((u8*)r4 + 0x3C) = r5;
    *(u32*)((u8*)r3 + 0x40) = tmp;
    return;
}

/* 0x48 | fn_80199014 | framed_no_calls */
void fn_80199014(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B260 */
}

/* 0x5C | fn_8019905C | call_sequence */
void fn_8019905C(void) {
    fn_801A6D08();
    fn_801AD214();
    fn_801C25E4();
}

/* 0x801990B8 | 0xC0 */
void fn_801990B8(void) {
    extern void fn_801A8470();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r27 = r3;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_801A8470();
    tmp = r30 & 0x04000000;
    if ((s32)tmp != 0) goto L_80199104;
    r5 = *(u32*)((u8*)r27 + 0x8);
    r4 = r30;
    r3 = *(u32*)((u8*)r27 + 0x8);
    r5 = *(u32*)((u8*)r5 + 0x0);
    r12 = *(u32*)((u8*)r5 + 0x3C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80199104:
    r31 = *(u32*)((u8*)r27 + 0xC);
    goto L_80199130;
L_8019910C:
    r6 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r4 = r28;
    r5 = r29;
    r12 = *(u32*)((u8*)r6 + 0x3C);
    r6 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r31 = *(u32*)((u8*)r31 + 0x4);
L_80199130:
    if (r31 != 0) goto L_8019910C;
    tmp = r30 & 0x04000000;
    if (r31 != 0) goto L_8019915C;
    r5 = *(u32*)((u8*)r27 + 0x8);
    r4 = r30;
    r3 = *(u32*)((u8*)r27 + 0x8);
    r5 = *(u32*)((u8*)r5 + 0x0);
    r12 = *(u32*)((u8*)r5 + 0x50);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8019915C:
    r3 = 0x0;
    fn_801A8470();
    return;
}

/* 0x80 | fn_80199178 | generic */
void fn_80199178(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_801ACDAC();
}

/* 0x6C | fn_801991F8 | guarded_call */
void fn_801991F8(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (0 /* guard r30 == 0 */) { return; }
    if (0 /* guard r30 == 0 */) { return; }
    if (0 /* guard r31 == 0 */) { return; }
    fn_801AD044();
}

/* 0x74 | fn_80199264 | framed_no_calls */
void fn_80199264(u32 arg1, u32 arg2) {
    /* data manipulation using stack locals */
}

/* 0x801992D8 | 0xCC */
void fn_801992D8(void) {
    extern u8 lbl_8036C7A0[];
    extern u8 lbl_8047B260[];
    extern u8 lbl_8047DA18[];
    extern u8 lbl_8047DA20[];
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
    if ((s32)tmp != 0) goto L_801992FC;
    r3 = 0x0;
    goto L_8019938C;
L_801992FC:
    tmp = *(u32*)((u8*)r30 + 0x0);
    if (tmp == 0) goto L_80199318;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_80193748();
    if (r3 != 0) goto L_80199354;
L_80199318:
    tmp = *(u32*)lbl_8047B260;
    if (tmp == 0) goto L_8019932C;
    r3 = *(u32*)lbl_8047B260;
    goto L_80199334;
L_8019932C:
    r3 = (u32)lbl_8036C7A0;
    r3 = (u32)lbl_8036C7A0;
L_80199334:
    fn_80193828();
    /* mr. r31, r3 */;
    if (tmp != 0) goto L_80199370;
    r3 = (u32)lbl_8047DA18;
    r4 = 0x214;
    r5 = (u32)lbl_8047DA20;
    fn_80196E10();
    goto L_80199370;
L_80199354:
    fn_80193828();
    /* mr. r31, r3 */;
    if (tmp != 0) goto L_80199370;
    r3 = (u32)lbl_8047DA18;
    r4 = 0x181;
    r5 = (u32)lbl_8047DA20;
    fn_80196E10();
L_80199370:
    r5 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r4 = r30;
    r12 = *(u32*)((u8*)r5 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r3 = r31;
L_8019938C:
    return;
}

/* 0x801993A4 | 0x1C4 */
void fn_801993A4(void) {
    extern u8 lbl_8027472C[];
    extern u8 lbl_8036C7A0[];
    extern u8 lbl_8047B260[];
    extern u8 lbl_8047DA18[];
    extern u8 lbl_8047DA20[];
    extern u8 lbl_8047DA28[];
    extern void fn_80193748();
    extern void fn_80193828();
    extern void fn_80196D78();
    extern void fn_80196E10();
    extern void fn_801A7B24();
    extern void fn_801AD288();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r4;
    r30 = r3;
    r29 = *(u32*)((u8*)r4 + 0x4);
    if (r29 != 0) goto L_801993DC;
    r28 = 0x0;
    goto L_80199468;
L_801993DC:
    tmp = *(u32*)((u8*)r29 + 0x0);
    if (tmp == 0) goto L_801993F8;
    r3 = *(u32*)((u8*)r29 + 0x0);
    fn_80193748();
    if (r3 != 0) goto L_80199434;
L_801993F8:
    tmp = *(u32*)lbl_8047B260;
    if (tmp == 0) goto L_8019940C;
    r3 = *(u32*)lbl_8047B260;
    goto L_80199414;
L_8019940C:
    r3 = (u32)lbl_8036C7A0;
    r3 = (u32)lbl_8036C7A0;
L_80199414:
    fn_80193828();
    /* mr. r28, r3 */;
    if (tmp != 0) goto L_80199450;
    r3 = (u32)lbl_8047DA18;
    r4 = 0x214;
    r5 = (u32)lbl_8047DA20;
    fn_80196E10();
    goto L_80199450;
L_80199434:
    fn_80193828();
    /* mr. r28, r3 */;
    if (tmp != 0) goto L_80199450;
    r3 = (u32)lbl_8047DA18;
    r4 = 0x181;
    r5 = (u32)lbl_8047DA20;
    fn_80196E10();
L_80199450:
    r5 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r4 = r29;
    r12 = *(u32*)((u8*)r5 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80199468:
    *(u32*)((u8*)r30 + 0x4) = r28;
    r3 = *(u32*)((u8*)r31 + 0x8);
    fn_801A7B24();
    *(u32*)((u8*)r30 + 0x8) = r3;
    r3 = *(u32*)((u8*)r31 + 0xC);
    fn_801AD288();
    *(u32*)((u8*)r30 + 0xC) = r3;
    tmp = *(u32*)((u8*)r30 + 0x8);
    if (tmp == 0) goto L_80199544;
    r3 = *(u32*)((u8*)r30 + 0x8);
    tmp = 0x40000000;
    r3 = *(u32*)((u8*)r3 + 0x4);
    r3 = r3 & 0x60000000;
    if ((s32)r3 == (s32)tmp) goto L_801994E4;
    if ((s32)r3 >= (s32)tmp) goto L_801994B8;
    if ((s32)r3 == 0) goto L_801994C8;
    goto L_8019951C;
L_801994B8:
    tmp = 0x60000000;
    if ((s32)r3 == (s32)tmp) goto L_80199500;
    goto L_8019951C;
L_801994C8:
    if (r30 == 0) goto L_80199544;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0xFFFFFFF1;
    tmp = tmp | 0x2;
    *(u32*)((u8*)r30 + 0x14) = tmp;
    goto L_80199544;
L_801994E4:
    if (r30 == 0) goto L_80199544;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0xFFFFFFF1;
    tmp = tmp | 0x8;
    *(u32*)((u8*)r30 + 0x14) = tmp;
    goto L_80199544;
L_80199500:
    if (r30 == 0) goto L_80199544;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0xFFFFFFF1;
    tmp = tmp | 0x4;
    *(u32*)((u8*)r30 + 0x14) = tmp;
    goto L_80199544;
L_8019951C:
    r4 = *(u32*)((u8*)r30 + 0x8);
    r3 = (u32)lbl_8027472C;
    r3 = (u32)lbl_8027472C;
    r4 = *(u32*)((u8*)r4 + 0x4);
    OSReport();
    r3 = (u32)lbl_8047DA18;
    r4 = 0x13f;
    r5 = (u32)lbl_8047DA28;
    fn_80196D78();
L_80199544:
    r3 = 0x0;
    return;
}

/* 0x6C | fn_80199568 | generic */
void fn_80199568(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_801C27F4();
    fn_801AD61C();
    fn_801A7E3C();
}

/* 0x80 | fn_801995D4 | generic */
void fn_801995D4(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_801AD6C4();
    fn_801A8354();
}

/* 0x80199654 | 0xB0 */
void fn_80199654(void) {
    extern void fn_801A83BC();
    extern void fn_801AD738();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if (r3 == 0) goto L_801996E8;
    r31 = r3;
    r30 = r4;
    r29 = r5;
    goto L_801996E0;
L_80199684:
    if (r31 == 0) goto L_801996B4;
    r3 = *(u32*)((u8*)r31 + 0xC);
    if (r29 == 0) goto L_801996A0;
    r4 = *(u32*)((u8*)r29 + 0x4);
    goto L_801996A4;
L_801996A0:
    r4 = 0x0;
L_801996A4:
    fn_801AD738();
    r3 = *(u32*)((u8*)r31 + 0x8);
    r4 = r30;
    fn_801A83BC();
L_801996B4:
    r31 = *(u32*)((u8*)r31 + 0x4);
    if (r30 == 0) goto L_801996C8;
    r30 = *(u32*)((u8*)r30 + 0x0);
    goto L_801996CC;
L_801996C8:
    r30 = 0x0;
L_801996CC:
    if (r29 == 0) goto L_801996DC;
    r29 = *(u32*)((u8*)r29 + 0x0);
    goto L_801996E0;
L_801996DC:
    r29 = 0x0;
L_801996E0:
    if (r31 != 0) goto L_80199684;
L_801996E8:
    return;
}

/* 0x8019970C | 0x2C */
void fn_8019970C(void) {
    extern u8 lbl_80465378[];
    extern void fn_801AA498();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = (u32)lbl_80465378;
    r4 = r3;
    r3 = (u32)lbl_80465378;
    fn_801AA498();
    return;
}

/* 0x5C | fn_80199738 | multi_call_guarded */
void fn_80199738(void) {
    { fn_801AA4CC(); return; }
    fn_80196E10();
    memset();
}

/* 0x80199794 | 0x2F0 */
void fn_80199794(void) {
    extern u8 lbl_80274758[];
    extern u8 lbl_80465378[];
    extern u8 lbl_8047DA30[];
    extern u8 lbl_8047DA38[];
    extern void fn_80196E10();
    extern void fn_80199738();
    extern void fn_80199794();
    extern void fn_80199A84();
    extern void fn_801AA4CC();
    u8 sp[0x40];
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
    f32 f3 = 0.0f;

    /* mr. r25, r3 */;
    if ((s32)tmp == 0) goto L_80199A6C;
    r3 = (u32)lbl_80465378;
    r3 = (u32)lbl_80465378;
    fn_801AA4CC();
    /* mr. r22, r3 */;
    if ((s32)tmp != 0) goto L_801997D0;
    r3 = (u32)lbl_8047DA30;
    r4 = 0x2f3;
    r5 = (u32)lbl_8047DA38;
    fn_80196E10();
L_801997D0:
    r3 = r22;
    r4 = 0x0;
    r5 = 0x30;
    memset((void*)r3, (int)r4, (u32)r5);
    r28 = *(u32*)((u8*)r25 + 0x0);
    r27 = r22;
    if (r28 == 0) goto L_80199A18;
    r3 = (u32)lbl_80465378;
    r3 = (u32)lbl_80465378;
    fn_801AA4CC();
    /* mr. r31, r3 */;
    if (r28 != 0) goto L_80199814;
    r3 = (u32)lbl_8047DA30;
    r4 = 0x2f3;
    r5 = (u32)lbl_8047DA38;
    fn_80196E10();
L_80199814:
    r3 = r31;
    r4 = 0x0;
    r5 = 0x30;
    memset((void*)r3, (int)r4, (u32)r5);
    r30 = *(u32*)((u8*)r28 + 0x0);
    if (r30 == 0) goto L_801999C8;
    r3 = (u32)lbl_80465378;
    r3 = (u32)lbl_80465378;
    fn_801AA4CC();
    /* mr. r26, r3 */;
    if (r30 != 0) goto L_80199854;
    r3 = (u32)lbl_8047DA30;
    r4 = 0x2f3;
    r5 = (u32)lbl_8047DA38;
    fn_80196E10();
L_80199854:
    r3 = r26;
    r4 = 0x0;
    r5 = 0x30;
    memset((void*)r3, (int)r4, (u32)r5);
    r29 = *(u32*)((u8*)r30 + 0x0);
    if (r29 == 0) goto L_80199978;
    r3 = (u32)lbl_80465378;
    r3 = (u32)lbl_80465378;
    fn_801AA4CC();
    r5 = (u32)lbl_80465378;
    r4 = (u32)lbl_80274758;
    tmp = (u32)lbl_80465378;
    r22 = r3;
    r4 = (u32)lbl_80274758;
    r3 = tmp;
    fn_80199A84();
    if (r22 != 0) goto L_801998B0;
    r3 = (u32)lbl_8047DA30;
    r4 = 0x2f3;
    r5 = (u32)lbl_8047DA38;
    fn_80196E10();
L_801998B0:
    r3 = r22;
    r4 = 0x0;
    r5 = 0x30;
    memset((void*)r3, (int)r4, (u32)r5);
    r23 = *(u32*)((u8*)r29 + 0x0);
    if (r23 == 0) goto L_80199928;
    fn_80199738();
    r24 = r3;
    r3 = *(u32*)((u8*)r23 + 0x0);
    fn_80199794();
    *(u32*)((u8*)r24 + 0x0) = r3;
    tmp = 0x0;
    f0 = *(f32*)((u8*)r23 + 0x8);
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r24 + 0x18) = r3;
    r3 = *(u8*)((u8*)r23 + 0xC);
    *(u8*)((u8*)r24 + 0x13) = r3;
    r3 = *(u8*)((u8*)r23 + 0xD);
    *(u8*)((u8*)r24 + 0x14) = r3;
    r3 = *(u8*)((u8*)r23 + 0xE);
    *(u8*)((u8*)r24 + 0x15) = r3;
    r3 = *(u32*)((u8*)r23 + 0x10);
    *(u32*)((u8*)r24 + 0x8) = r3;
    r3 = *(u32*)((u8*)r23 + 0x4);
    *(u32*)((u8*)r24 + 0xC) = r3;
    *(u8*)((u8*)r24 + 0x10) = tmp;
    goto L_8019992C;
L_80199928:
    r24 = 0x0;
L_8019992C:
    *(u32*)((u8*)r22 + 0x0) = r24;
    tmp = 0x0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r22 + 0x18) = r3;
    r3 = *(u8*)((u8*)r29 + 0xC);
    *(u8*)((u8*)r22 + 0x13) = r3;
    r3 = *(u8*)((u8*)r29 + 0xD);
    *(u8*)((u8*)r22 + 0x14) = r3;
    r3 = *(u8*)((u8*)r29 + 0xE);
    *(u8*)((u8*)r22 + 0x15) = r3;
    r3 = *(u32*)((u8*)r29 + 0x10);
    *(u32*)((u8*)r22 + 0x8) = r3;
    r3 = *(u32*)((u8*)r29 + 0x4);
    *(u32*)((u8*)r22 + 0xC) = r3;
    *(u8*)((u8*)r22 + 0x10) = tmp;
    goto L_8019997C;
L_80199978:
    r22 = 0x0;
L_8019997C:
    *(u32*)((u8*)r26 + 0x0) = r22;
    tmp = 0x0;
    f0 = *(f32*)((u8*)r30 + 0x8);
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r26 + 0x18) = r3;
    r3 = *(u8*)((u8*)r30 + 0xC);
    *(u8*)((u8*)r26 + 0x13) = r3;
    r3 = *(u8*)((u8*)r30 + 0xD);
    *(u8*)((u8*)r26 + 0x14) = r3;
    r3 = *(u8*)((u8*)r30 + 0xE);
    *(u8*)((u8*)r26 + 0x15) = r3;
    r3 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)((u8*)r26 + 0x8) = r3;
    r3 = *(u32*)((u8*)r30 + 0x4);
    *(u32*)((u8*)r26 + 0xC) = r3;
    *(u8*)((u8*)r26 + 0x10) = tmp;
    goto L_801999CC;
L_801999C8:
    r26 = 0x0;
L_801999CC:
    *(u32*)((u8*)r31 + 0x0) = r26;
    tmp = 0x0;
    f0 = *(f32*)((u8*)r28 + 0x8);
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r31 + 0x18) = r3;
    r3 = *(u8*)((u8*)r28 + 0xC);
    *(u8*)((u8*)r31 + 0x13) = r3;
    r3 = *(u8*)((u8*)r28 + 0xD);
    *(u8*)((u8*)r31 + 0x14) = r3;
    r3 = *(u8*)((u8*)r28 + 0xE);
    *(u8*)((u8*)r31 + 0x15) = r3;
    r3 = *(u32*)((u8*)r28 + 0x10);
    *(u32*)((u8*)r31 + 0x8) = r3;
    r3 = *(u32*)((u8*)r28 + 0x4);
    *(u32*)((u8*)r31 + 0xC) = r3;
    *(u8*)((u8*)r31 + 0x10) = tmp;
    goto L_80199A1C;
L_80199A18:
    r31 = 0x0;
L_80199A1C:
    *(u32*)((u8*)r27 + 0x0) = r31;
    tmp = 0x0;
    r3 = r27;
    f0 = *(f32*)((u8*)r25 + 0x8);
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r27 + 0x18) = r4;
    r4 = *(u8*)((u8*)r25 + 0xC);
    *(u8*)((u8*)r27 + 0x13) = r4;
    r4 = *(u8*)((u8*)r25 + 0xD);
    *(u8*)((u8*)r27 + 0x14) = r4;
    r4 = *(u8*)((u8*)r25 + 0xE);
    *(u8*)((u8*)r27 + 0x15) = r4;
    r4 = *(u32*)((u8*)r25 + 0x10);
    *(u32*)((u8*)r27 + 0x8) = r4;
    r4 = *(u32*)((u8*)r25 + 0x4);
    *(u32*)((u8*)r27 + 0xC) = r4;
    *(u8*)((u8*)r27 + 0x10) = tmp;
    goto L_80199A70;
L_80199A6C:
    r3 = 0x0;
L_80199A70:
    return;
}
