/**
 * @file hsd_mobj.c
 * @brief HSD MObj - Material object implementation.
 *
 * Colosseum address: 0x801A6A34 (HSD_MObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/mobj.c
 */

#include "hsd/hsd_mobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_memory.h"
#include "hsd/hsd_tobj.h"

extern void* memset(void* dst, int val, u32 size);

static void MObjInfoInit(void);

HSD_MObjInfo hsdMObj = { MObjInfoInit };

static HSD_ClassInfo* default_class = NULL;

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

void HSD_MObjSetFlags(HSD_MObj* mobj, u32 flags)
{
    if (mobj != NULL) {
        mobj->rendermode |= flags;
    }
}

void HSD_MObjClearFlags(HSD_MObj* mobj, u32 flags)
{
    if (mobj != NULL) {
        mobj->rendermode &= ~flags;
    }
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_MObjAddAnim(HSD_MObj* mobj, HSD_MatAnim* matanim)
{
    if (mobj == NULL || matanim == NULL) {
        return;
    }
    if (mobj->aobj != NULL) {
        HSD_AObjRemove(mobj->aobj);
    }
    mobj->aobj = HSD_AObjLoadDesc(matanim->aobjdesc);
    HSD_TObjAddAnimAll(mobj->tobj, matanim->texanim);
}

void HSD_MObjReqAnim(HSD_MObj* mobj, f32 startframe)
{
    if (mobj != NULL) {
        HSD_AObjReqAnim(mobj->aobj, startframe);
        HSD_TObjReqAnimAll(mobj->tobj, startframe);
    }
}

void HSD_MObjAnim(HSD_MObj* mobj)
{
    if (mobj != NULL) {
        HSD_TObjAnimAll(mobj->tobj);
    }
}

/* ========================================================================= */
/*  TObj accessor                                                            */
/* ========================================================================= */

HSD_TObj* HSD_MObjGetTObj(HSD_MObj* mobj)
{
    if (mobj == NULL) {
        return NULL;
    }
    return mobj->tobj;
}

/* ========================================================================= */
/*  Alpha                                                                    */
/* ========================================================================= */

void HSD_MObjSetAlpha(HSD_MObj* mobj, f32 alpha)
{
    HSD_ASSERT(0, mobj);
    if (mobj->mat != NULL) {
        mobj->mat->alpha = alpha;
    }
}

/* ========================================================================= */
/*  Load                                                                     */
/* ========================================================================= */

static int MObjLoad(HSD_MObj* mobj, HSD_MObjDesc* desc)
{
    mobj->rendermode = desc->rendermode;
    if (mobj->tobj != NULL) {
        HSD_TObjRemoveAll(mobj->tobj);
    }
    mobj->tobj = HSD_TObjLoadDesc(desc->texdesc);
    mobj->mat = desc->mat;
    mobj->pe = desc->pedesc;
    return 0;
}

HSD_MObj* HSD_MObjLoadDesc(HSD_MObjDesc* mobjdesc)
{
    HSD_MObj* mobj;
    HSD_ClassInfo* info;

    if (mobjdesc == NULL) {
        return NULL;
    }

    if (mobjdesc->class_name == NULL ||
        !(info = hsdSearchClassInfo(mobjdesc->class_name)))
    {
        mobj = HSD_MObjAlloc();
    } else {
        mobj = hsdNew(info);
        HSD_ASSERT(0, mobj);
    }

    HSD_MOBJ_METHOD(mobj)->load(mobj, mobjdesc);
    return mobj;
}

/* ========================================================================= */
/*  Remove / Alloc                                                           */
/* ========================================================================= */

void HSD_MObjRemove(HSD_MObj* mobj)
{
    if (mobj != NULL) {
        HSD_CLASS_METHOD(mobj)->release((HSD_Class*) mobj);
        HSD_CLASS_METHOD(mobj)->destroy((HSD_Class*) mobj);
    }
}

HSD_MObj* HSD_MObjAlloc(void)
{
    HSD_MObj* mobj;
    mobj = (HSD_MObj*) hsdNew(
        default_class ? default_class : (HSD_ClassInfo*) &hsdMObj);
    HSD_ASSERT(0, mobj);
    return mobj;
}

HSD_Material* HSD_MaterialAlloc(void)
{
    HSD_Material* mat = (HSD_Material*) HSD_MemAlloc(sizeof(HSD_Material));
    if (mat != NULL) {
        memset(mat, 0, sizeof(HSD_Material));
    }
    return mat;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void MObjRelease(HSD_Class* o)
{
    HSD_MObj* mobj = (HSD_MObj*) o;
    HSD_TObjRemoveAll(mobj->tobj);
    HSD_AObjRemove(mobj->aobj);
    HSD_PARENT_INFO(&hsdMObj)->release(o);
}

static void MObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdMObj)->amnesia(info);
}

static void MObjInfoInit(void)
{
    hsdInitClassInfo((HSD_ClassInfo*) &hsdMObj, &hsdClass,
                     "sysdolphin_base_library", "hsd_mobj",
                     sizeof(HSD_MObjInfo), sizeof(HSD_MObj));
    ((HSD_ClassInfo*) &hsdMObj)->release = MObjRelease;
    ((HSD_ClassInfo*) &hsdMObj)->amnesia = MObjAmnesia;
    hsdMObj.load = MObjLoad;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */

extern u32 lbl_8047B2D4;

/* Address: 0x801A6DC4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_801A6DC4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x801A8470 | Size: 0x8 | Pattern: sda_setter */
void fn_801A8470(u32 val) {
    lbl_8047B2D4 = val;
}

/* =========================================================================
 *  Internal stubs: 0x801A6A34-0x801A8400 (19 functions)
 * ========================================================================= */

/* 0x801A6A34 | 0xF0 */
void fn_801A6A34(void) {
    extern u8 lbl_80274E38[];
    extern u8 lbl_80274E50[];
    extern u8 lbl_8036C638[];
    extern u8 lbl_8036CB30[];
    extern void fn_80193B30();
    extern void fn_801A6B24();
    extern void fn_801A6B8C();
    extern void fn_801A6E00();
    extern void fn_801A6E24();
    extern void fn_801A6F78();
    extern void fn_801A7128();
    extern void fn_801A7D58();
    extern void fn_801A7E84();
    u8 sp[0x20];
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
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_8036CB30;
    r4 = (u32)lbl_8036C638;
    r5 = (u32)lbl_80274E38;
    r6 = (u32)lbl_80274E50;
    r3 = (u32)lbl_8036CB30;
    r4 = (u32)lbl_8036C638;
    r5 = (u32)lbl_80274E38;
    r6 = (u32)lbl_80274E50;
    r7 = 0x54;
    r8 = 0x20;
    fn_80193B30();
    r10 = (u32)fn_801A6E00;
    r9 = (u32)fn_801A7D58;
    r7 = (u32)fn_801A7128;
    r5 = (u32)fn_801A6F78;
    r8 = (u32)fn_801A6B8C;
    r6 = (u32)fn_801A6B24;
    r4 = (u32)fn_801A6E24;
    r28 = (u32)fn_801A7D58;
    r30 = (u32)fn_801A7128;
    r12 = (u32)fn_801A6F78;
    r7 = (u32)lbl_8036CB30;
    r5 = (u32)lbl_8036CB30;
    r27 = (u32)lbl_8036CB30;
    r29 = (u32)lbl_8036CB30;
    r31 = (u32)lbl_8036CB30;
    r11 = (u32)lbl_8036CB30;
    r9 = (u32)lbl_8036CB30;
    r29 = (u32)lbl_8036CB30;
    r31 = (u32)lbl_8036CB30;
    r11 = (u32)lbl_8036CB30;
    r3 = (u32)fn_801A7E84;
    tmp = (u32)fn_801A6E00;
    r10 = (u32)fn_801A7E84;
    r27 = (u32)lbl_8036CB30;
    r3 = (u32)lbl_8036CB30;
    r9 = (u32)lbl_8036CB30;
    r8 = (u32)fn_801A6B8C;
    r7 = (u32)lbl_8036CB30;
    r6 = (u32)fn_801A6B24;
    r5 = (u32)lbl_8036CB30;
    r4 = (u32)fn_801A6E24;
    r3 = (u32)lbl_8036CB30;
    *(u32*)((u8*)r7 + 0x30) = r8;
    *(u32*)((u8*)r5 + 0x38) = r6;
    *(u32*)((u8*)r3 + 0x3C) = r4;
    *(u32*)((u8*)r27 + 0x50) = tmp;
    *(u32*)((u8*)r29 + 0x40) = r28;
    *(u32*)((u8*)r31 + 0x44) = r30;
    *(u32*)((u8*)r11 + 0x48) = r12;
    *(u32*)((u8*)r9 + 0x4C) = r10;
    return;
}

/* 0x68 | fn_801A6B24 | framed_no_calls */
void fn_801A6B24(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B2D8, lbl_8047B2D0, lbl_8047B2DC */
}

/* 0x801A6B8C | 0xA8 */
void fn_801A6B8C(void) {
    extern u8 lbl_8036CB30[];
    extern void fn_80193AF0();
    extern void fn_801B42C0();
    extern void fn_801B7178();
    extern void fn_801BBE60();
    extern void fn_801C25E4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x14);
    fn_801C25E4();
    r3 = *(u32*)((u8*)r31 + 0xC);
    r4 = 0x14;
    fn_80193AF0();
    r3 = *(u32*)((u8*)r31 + 0x8);
    fn_801BBE60();
    tmp = *(u32*)((u8*)r31 + 0x18);
    if (tmp == 0) goto L_801A6BD0;
    r3 = *(u32*)((u8*)r31 + 0x18);
    fn_801B42C0();
L_801A6BD0:
    tmp = *(u32*)((u8*)r31 + 0x1C);
    if (tmp == 0) goto L_801A6BEC;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    r4 = 0x7;
    r5 = 0x1;
    fn_801B7178();
L_801A6BEC:
    tmp = *(u32*)((u8*)r31 + 0x10);
    if (tmp == 0) goto L_801A6C04;
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = 0xc;
    fn_80193AF0();
L_801A6C04:
    r4 = (u32)lbl_8036CB30;
    r3 = r31;
    r4 = (u32)lbl_8036CB30;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

/* 0x70 | fn_801A6C34 | generic */
void fn_801A6C34(u32 arg1) {
    /* refs: lbl_8047B2DC */
}

/* 0x64 | fn_801A6CA4 | generic */
void fn_801A6CA4(void) {
    /* refs: lbl_8047B2DC */
    fn_80196E10();
}

/* 0x54 | fn_801A6D08 | framed_no_calls */
void fn_801A6D08(u32 arg1, u32 arg2) {
    /* data manipulation using stack locals */
}

/* 0x44 | fn_801A6D5C | guarded_call */
void fn_801A6D5C(void) {
    if (0 /* guard r3 == 0 */) { return; }
    if (0 /* guard r4 == 0 */) { return; }
    if (1 /* guard r4 != 0 */) { return; }
    if (1 /* guard r5 != 0 */) { return; }
    fn_801BBE3C();
}

/* 0x801A6DA0 | 0x24 */
void fn_801A6DA0(u32* head, u32* node) {
    if (head == NULL) { return; }
    if (node == NULL) { return; }
    node[2] = head[2];
    head[2] = (u32)node;
}

/* 0x801A6DDC | 0x24 */
void fn_801A6DDC(u8* ptr, f32 val) {
    u8* child;
    if (ptr == NULL) { return; }
    child = *(u8**)(ptr + 0xC);
    if (child == NULL) { return; }
    *(f32*)(child + 0xC) = val;
}

/* 0x801A6E00 | 0x24 */
extern void* fn_801BBFE4(u32);
void* fn_801A6E00(void) {
    return fn_801BBFE4(0);
}

/* 0x801A6E24 | 0x154 */
void fn_801A6E24(void) {
    extern u8 lbl_8047B2D8[];
    extern u8 lbl_8047B2DC[];
    extern void fn_801B28B8();
    extern void fn_801B28C8();
    extern void fn_801B294C();
    extern void fn_801B3884();
    extern void fn_801BDA58();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    fn_801B3884();
    r6 = *(u32*)((u8*)r30 + 0xC);
    r5 = r1 + 0x8;
    r31 = *(u32*)((u8*)r30 + 0x4);
    r4 = r1 + 0xc;
    tmp = *(u32*)((u8*)r6 + 0x8);
    r3 = r1 + 0x10;
    *(u32*)(sp + 0x8) = tmp;
    r6 = *(u32*)((u8*)r30 + 0xC);
    tmp = *(u32*)((u8*)r6 + 0x4);
    *(u32*)(sp + 0xC) = tmp;
    r6 = *(u32*)((u8*)r30 + 0xC);
    tmp = *(u32*)((u8*)r6 + 0x0);
    *(u32*)(sp + 0x10) = tmp;
    r6 = *(u32*)((u8*)r30 + 0xC);
    f1 = *(f32*)((u8*)r6 + 0xC);
    fn_801B28C8();
    tmp = r31 & 0x00000008;
    if ((s32)tmp == 0) goto L_801A6E98;
    r3 = *(u32*)((u8*)r30 + 0xC);
    f1 = *(f32*)((u8*)r3 + 0x10);
    fn_801B28B8();
L_801A6E98:
    r3 = *(u32*)((u8*)r30 + 0x8);
    tmp = r31 & 0x04000000;
    r29 = 0x0;
    if ((s32)tmp == 0) goto L_801A6EDC;
    tmp = *(u32*)lbl_8047B2DC;
    if (tmp == 0) goto L_801A6EDC;
    r29 = r1 + 0x14;
    goto L_801A6EC8;
L_801A6EC0:
    r3 = *(u32*)((u8*)r29 + 0x0);
    r29 = r3 + 0x8;
L_801A6EC8:
    tmp = *(u32*)((u8*)r29 + 0x0);
    if (tmp != 0) goto L_801A6EC0;
    tmp = *(u32*)lbl_8047B2DC;
    *(u32*)((u8*)r29 + 0x0) = tmp;
L_801A6EDC:
    tmp = r31 & 0x00001000;
    if (tmp == 0) goto L_801A6F14;
    tmp = *(u32*)lbl_8047B2D8;
    if (tmp == 0) goto L_801A6F14;
    r3 = *(u32*)lbl_8047B2D8;
    tmp = *(u32*)((u8*)r3 + 0x58);
    if (tmp == 0) goto L_801A6F14;
    r3 = *(u32*)lbl_8047B2D8;
    *(u32*)((u8*)r3 + 0x8) = tmp;
    tmp = *(u32*)lbl_8047B2D8;
    *(u32*)(sp + 0x14) = tmp;
L_801A6F14:
    ((void(*)(void))fn_801BBFE4)();
    fn_801BDA58();
    r6 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r5 = r31;
    r12 = *(u32*)((u8*)r6 + 0x48);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = *(u32*)((u8*)r30 + 0x10);
    r3 = r31;
    fn_801B294C();
    if (r29 == 0) goto L_801A6F5C;
    tmp = 0x0;
    *(u32*)((u8*)r29 + 0x0) = tmp;
L_801A6F5C:
    return;
}

/* 0x78 | fn_801A6F78 | generic */
void fn_801A6F78(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    /* refs: lbl_80274E5C */
    fn_80196E10();
    fn_801B45A4();
    fn_801BD8D0();
}

/* 0x801A6FF0 | 0x138 */
void fn_801A6FF0(void) {
    extern u8 lbl_8047B2D8[];
    extern u8 lbl_8047B2DC[];
    extern void fn_801B42C0();
    extern void fn_801B4300();
    extern void fn_801B7178();
    extern void fn_801BC33C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = 0x0;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) goto L_801A7110;
    tmp = *(u32*)((u8*)r30 + 0x18);
    if (tmp == 0) goto L_801A702C;
    r3 = *(u32*)((u8*)r30 + 0x18);
    fn_801B42C0();
    tmp = 0x0;
    *(u32*)((u8*)r30 + 0x18) = tmp;
L_801A702C:
    tmp = *(u32*)((u8*)r30 + 0x1C);
    if (tmp == 0) goto L_801A7050;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = 0x7;
    r5 = 0x1;
    fn_801B7178();
    tmp = 0x0;
    *(u32*)((u8*)r30 + 0x1C) = tmp;
L_801A7050:
    tmp = *(u32*)((u8*)r30 + 0x8);
    *(u32*)(sp + 0x8) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x4);
    tmp = tmp & 0x04000000;
    if (tmp == 0) goto L_801A7094;
    tmp = *(u32*)lbl_8047B2DC;
    if (tmp == 0) goto L_801A7094;
    r31 = r1 + 0x8;
    goto L_801A7080;
L_801A7078:
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r3 + 0x8;
L_801A7080:
    tmp = *(u32*)((u8*)r31 + 0x0);
    if (tmp != 0) goto L_801A7078;
    tmp = *(u32*)lbl_8047B2DC;
    *(u32*)((u8*)r31 + 0x0) = tmp;
L_801A7094:
    tmp = *(u32*)((u8*)r30 + 0x4);
    tmp = tmp & 0x00001000;
    if (tmp == 0) goto L_801A70D0;
    tmp = *(u32*)lbl_8047B2D8;
    if (tmp == 0) goto L_801A70D0;
    r3 = *(u32*)lbl_8047B2D8;
    tmp = *(u32*)((u8*)r3 + 0x58);
    if (tmp == 0) goto L_801A70D0;
    r3 = *(u32*)lbl_8047B2D8;
    *(u32*)((u8*)r3 + 0x8) = tmp;
    tmp = *(u32*)lbl_8047B2D8;
    *(u32*)(sp + 0x8) = tmp;
L_801A70D0:
    fn_801BC33C();
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r5 = r30 + 0x1c;
    r12 = *(u32*)((u8*)r4 + 0x44);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = r30 + 0x18;
    r5 = r30 + 0x1c;
    fn_801B4300();
    if (r31 == 0) goto L_801A7110;
    tmp = 0x0;
    *(u32*)((u8*)r31 + 0x0) = tmp;
L_801A7110:
    return;
}

/* 0x801A7128 | 0x9FC */
void fn_801A7128(void) {
    extern u8 lbl_80478C88[];
    extern u8 lbl_8047DC18[];
    extern u8 lbl_8047DC38[];
    extern void fn_80196E10();
    extern void fn_801B5E40();
    extern void fn_801B5F08();
    extern void fn_801B64EC();
    extern void fn_801B6CD8();
    extern void fn_801B6E74();
    extern void fn_801B6F5C();
    extern void fn_801B707C();
    extern void fn_801B7C60();
    u8 sp[0x40];
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
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r26, r5 */;
    r24 = r3;
    r25 = r4;
    r29 = 0x0;
    r28 = 0x0;
    if ((s32)tmp != 0) goto L_801A7160;
    r3 = (u32)lbl_8047DC18;
    r4 = 0x18b;
    r5 = (u32)lbl_8047DC38;
    fn_80196E10();
L_801A7160:
    tmp = 0x0;
    r30 = r25;
    *(u32*)((u8*)r26 + 0x0) = tmp;
    goto L_801A7188;
L_801A7170:
    tmp = *(u32*)((u8*)r30 + 0x4C);
    tmp = tmp & 0xF;
    if (tmp != 4) goto L_801A7184;
    r29 = r30;
L_801A7184:
    r30 = *(u32*)((u8*)r30 + 0x8);
L_801A7188:
    if (r30 != 0) goto L_801A7170;
    tmp = *(u32*)((u8*)r24 + 0x4);
    r30 = tmp & 0x3;
    if (r30 != 0) goto L_801A71A0;
    r30 = 0x1;
L_801A71A0:
    tmp = *(u32*)((u8*)r24 + 0x4);
    r27 = tmp & 0x00006000;
    if (r30 != 0) goto L_801A71B0;
    r27 = r30 << 13;
L_801A71B0:
    r3 = r26;
    fn_801B707C();
    tmp = *(u32*)((u8*)r24 + 0x4);
    r31 = r3;
    tmp = tmp & 0x00000004;
    if (r30 == 0) goto L_801A734C;
    if ((s32)r30 == 2) goto L_801A71D4;
    goto L_801A721C;
L_801A71D4:
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x8;
    fn_801B64EC();
    goto L_801A727C;
L_801A721C:
    r3 = *(u32*)((u8*)r24 + 0xC);
    r6 = r26;
    r4 = 0x1;
    r5 = 0x0;
    r3 = r3 + 0x4;
    fn_801B6F5C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
L_801A727C:
    if ((s32)r27 == 0x4000) goto L_801A7288;
    goto L_801A72E8;
L_801A7288:
    r6 = r26;
    r3 = (u32)lbl_80478C88;
    r4 = 0x6;
    r5 = 0x0;
    fn_801B6F5C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x6;
    fn_801B5F08();
    goto L_801A75F0;
L_801A72E8:
    r3 = *(u32*)((u8*)r24 + 0xC);
    r6 = r26;
    r4 = 0x6;
    r5 = 0x3;
    r3 = r3 + 0xc;
    fn_801B6F5C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x6;
    fn_801B5F08();
    goto L_801A75F0;
L_801A734C:
    if ((s32)r30 == 2) goto L_801A73C8;
    if ((s32)r30 >= 2) goto L_801A742C;
    if ((s32)r30 >= 1) goto L_801A7364;
    goto L_801A742C;
L_801A7364:
    r3 = *(u32*)((u8*)r24 + 0xC);
    r6 = r26;
    r4 = 0x1;
    r5 = 0x0;
    r3 = r3 + 0x4;
    fn_801B6F5C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
    goto L_801A74A8;
L_801A73C8:
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    tmp = -0x2;
    if (r29 == 0) goto L_801A7400;
    tmp = -0x1;
L_801A7400:
    *(u32*)(sp + 0x8) = tmp;
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
    goto L_801A74A8;
L_801A742C:
    r3 = *(u32*)((u8*)r24 + 0xC);
    r6 = r26;
    r4 = 0x1;
    r5 = 0x0;
    r3 = r3 + 0x4;
    fn_801B6F5C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = r29;
    r5 = 0x4;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    r3 = r31;
    r4 = 0x1;
    r5 = -0x2;
    if (r29 == 0) goto L_801A7490;
    r5 = -0x1;
L_801A7490:
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
L_801A74A8:
    if ((s32)r27 == 0x4000) goto L_801A7524;
    if ((s32)r27 >= 0x4000) goto L_801A757C;
    if ((s32)r27 == 0x2000) goto L_801A74C0;
    goto L_801A757C;
L_801A74C0:
    r3 = *(u32*)((u8*)r24 + 0xC);
    r6 = r26;
    r4 = 0x6;
    r5 = 0x3;
    r3 = r3 + 0xc;
    fn_801B6F5C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x6;
    fn_801B5F08();
    goto L_801A75F0;
L_801A7524:
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    tmp = -0x2;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x5;
    fn_801B5F08();
    goto L_801A75F0;
L_801A757C:
    r3 = *(u32*)((u8*)r24 + 0xC);
    r6 = r26;
    r4 = 0x6;
    r5 = 0x3;
    r3 = r3 + 0xc;
    fn_801B6F5C();
    tmp = r3;
    r3 = r31;
    r30 = tmp;
    r4 = r29;
    r5 = 0x4;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r9 = r30;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x5;
    r7 = -0x2;
    r8 = 0x6;
    r10 = 0x7;
    fn_801B5F08();
L_801A75F0:
    r30 = r25;
    goto L_801A7644;
L_801A7600:
    tmp = *(u32*)((u8*)r30 + 0x4C);
    tmp = tmp & 0x50;
    if ((s32)r27 == 0x2000) goto L_801A7640;
    tmp = *(u32*)((u8*)r30 + 0xC);
    if ((s32)tmp == 0xff) goto L_801A7640;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r5 = r28;
    r8 = r26;
    r12 = *(u32*)((u8*)r4 + 0x44);
    r6 = r1 + 0x1c;
    r7 = r1 + 0x10;
    r4 = 0x50;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A7640:
    r30 = *(u32*)((u8*)r30 + 0x8);
L_801A7644:
    if (r30 != 0) goto L_801A7600;
    tmp = *(u32*)((u8*)r24 + 0x4);
    r28 = r28 | 0x50;
    tmp = tmp & 0x00000004;
    if (r30 == 0) goto L_801A7868;
    tmp = r27 & 0x00004000;
    if (r30 == 0) goto L_801A770C;
    r3 = r26;
    fn_801B707C();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x5;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    *(u32*)(sp + 0x8) = tmp;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x5;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x5;
    r9 = -0x2;
    r10 = 0x7;
    fn_801B5F08();
L_801A770C:
    r3 = r26;
    fn_801B707C();
    r31 = r3;
    if (r29 == 0) goto L_801A7778;
    r3 = r31;
    r4 = r29;
    r5 = 0x4;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    r8 = 0x1;
    r9 = -0x1;
    r10 = 0x7;
    fn_801B64EC();
    goto L_801A77CC;
L_801A7778:
    r3 = r31;
    r4 = 0x0;
    r5 = 0x4;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    r8 = 0x1;
    r9 = -0x2;
    r10 = 0x7;
    fn_801B64EC();
L_801A77CC:
    tmp = r27 & 0x00004000;
    if (r29 == 0) goto L_801A7820;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x5;
    r8 = 0x5;
    r9 = -0x2;
    r10 = 0x7;
    fn_801B5F08();
    goto L_801A7864;
L_801A7820:
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x5;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x5;
    r9 = -0x2;
    r10 = 0x7;
    fn_801B5F08();
L_801A7864:
L_801A7868:
    tmp = *(u32*)((u8*)r24 + 0x4);
    tmp = tmp & 0x00000008;
    if (r29 == 0) goto L_801A79E8;
    r3 = *(u32*)((u8*)r24 + 0xC);
    r6 = r26;
    r4 = 0x1;
    r5 = 0x0;
    r3 = r3 + 0x8;
    fn_801B6F5C();
    tmp = r3;
    r3 = r26;
    r30 = tmp;
    fn_801B707C();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
    r30 = r25;
    goto L_801A792C;
L_801A78E8:
    tmp = *(u32*)((u8*)r30 + 0x4C);
    tmp = tmp & 0x00000020;
    if (r29 == 0) goto L_801A7928;
    tmp = *(u32*)((u8*)r30 + 0xC);
    if ((s32)tmp == 0xff) goto L_801A7928;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r5 = r28;
    r8 = r26;
    r12 = *(u32*)((u8*)r4 + 0x44);
    r6 = r1 + 0x18;
    r7 = r1 + 0x10;
    r4 = 0x20;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A7928:
    r30 = *(u32*)((u8*)r30 + 0x8);
L_801A792C:
    if (r30 != 0) goto L_801A78E8;
    r3 = r26;
    r28 = r28 | 0x20;
    fn_801B707C();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x5;
    fn_801B5E40();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    tmp = 0x0;
    r3 = r31;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    r8 = 0x1;
    r9 = -0x2;
    r10 = 0x7;
    fn_801B64EC();
    r3 = r26;
    fn_801B707C();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    r3 = r31;
    r4 = 0x1;
    r6 = 0x7;
    *(u32*)(sp + 0x8) = tmp;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
L_801A79E8:
    r30 = r25;
    *(u32*)(sp + 0x14) = tmp;
    goto L_801A7A3C;
L_801A79F8:
    tmp = *(u32*)((u8*)r30 + 0x4C);
    tmp = tmp & 0x00000080;
    if (r30 == 0) goto L_801A7A38;
    tmp = *(u32*)((u8*)r30 + 0xC);
    if ((s32)tmp == 0xff) goto L_801A7A38;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r5 = r28;
    r8 = r26;
    r12 = *(u32*)((u8*)r4 + 0x44);
    r6 = r1 + 0x14;
    r7 = r1 + 0x10;
    r4 = 0x80;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A7A38:
    r30 = *(u32*)((u8*)r30 + 0x8);
L_801A7A3C:
    if (r30 != 0) goto L_801A79F8;
    if (r3 != tmp) goto L_801A7A74;
    fn_801B7C60();
    if ((s32)r3 != 1) goto L_801A7A74;
    fn_801B7C60();
    if ((s32)r3 == 1) goto L_801A7B0C;
L_801A7A74:
    r3 = r26;
    fn_801B707C();
    r4 = 0x0;
    r31 = r3;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6E74();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    *(u32*)(sp + 0x8) = tmp;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x1;
    fn_801B64EC();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    r3 = r31;
    r4 = 0x7;
    r5 = 0x0;
    *(u32*)(sp + 0x8) = tmp;
    r6 = 0x7;
    r7 = 0x0;
    r8 = 0x7;
    r9 = 0x0;
    r10 = 0x5;
    fn_801B5F08();
    r3 = r31;
    goto L_801A7B10;
L_801A7B0C:
L_801A7B10:
    return;
}

/* 0x801A7B24 | 0x1D8 */
void fn_801A7B24(void) {
    extern u8 lbl_8036CB30[];
    extern u8 lbl_8047B2D0[];
    extern u8 lbl_8047B2D8[];
    extern u8 lbl_8047B2DC[];
    extern u8 lbl_8047DC18[];
    extern u8 lbl_8047DC30[];
    extern void fn_80193748();
    extern void fn_80193828();
    extern void fn_80196E10();
    extern void fn_801B42C0();
    extern void fn_801B4300();
    extern void fn_801B7178();
    extern void fn_801BC33C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) goto L_801A7CE0;
    tmp = *(u32*)((u8*)r31 + 0x0);
    if (tmp == 0) goto L_801A7B5C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    fn_80193748();
    if (r3 != 0) goto L_801A7B98;
L_801A7B5C:
    tmp = *(u32*)lbl_8047B2D0;
    if (tmp == 0) goto L_801A7B70;
    r3 = *(u32*)lbl_8047B2D0;
    goto L_801A7B78;
L_801A7B70:
    r3 = (u32)lbl_8036CB30;
    r3 = (u32)lbl_8036CB30;
L_801A7B78:
    fn_80193828();
    /* mr. r30, r3 */;
    if (tmp != 0) goto L_801A7BB4;
    r3 = (u32)lbl_8047DC18;
    r4 = 0x44a;
    r5 = (u32)lbl_8047DC30;
    fn_80196E10();
    goto L_801A7BB4;
L_801A7B98:
    fn_80193828();
    /* mr. r30, r3 */;
    if (tmp != 0) goto L_801A7BB4;
    r3 = (u32)lbl_8047DC18;
    r4 = 0x175;
    r5 = (u32)lbl_8047DC30;
    fn_80196E10();
L_801A7BB4:
    r5 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r4 = r31;
    r12 = *(u32*)((u8*)r5 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r31 = 0x0;
    if (r30 == 0) goto L_801A7CD8;
    tmp = *(u32*)((u8*)r30 + 0x18);
    if (tmp == 0) goto L_801A7BF4;
    r3 = *(u32*)((u8*)r30 + 0x18);
    fn_801B42C0();
    tmp = 0x0;
    *(u32*)((u8*)r30 + 0x18) = tmp;
L_801A7BF4:
    tmp = *(u32*)((u8*)r30 + 0x1C);
    if (tmp == 0) goto L_801A7C18;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r4 = 0x7;
    r5 = 0x1;
    fn_801B7178();
    tmp = 0x0;
    *(u32*)((u8*)r30 + 0x1C) = tmp;
L_801A7C18:
    tmp = *(u32*)((u8*)r30 + 0x8);
    *(u32*)(sp + 0x8) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x4);
    tmp = tmp & 0x04000000;
    if (tmp == 0) goto L_801A7C5C;
    tmp = *(u32*)lbl_8047B2DC;
    if (tmp == 0) goto L_801A7C5C;
    r31 = r1 + 0x8;
    goto L_801A7C48;
L_801A7C40:
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r3 + 0x8;
L_801A7C48:
    tmp = *(u32*)((u8*)r31 + 0x0);
    if (tmp != 0) goto L_801A7C40;
    tmp = *(u32*)lbl_8047B2DC;
    *(u32*)((u8*)r31 + 0x0) = tmp;
L_801A7C5C:
    tmp = *(u32*)((u8*)r30 + 0x4);
    tmp = tmp & 0x00001000;
    if (tmp == 0) goto L_801A7C98;
    tmp = *(u32*)lbl_8047B2D8;
    if (tmp == 0) goto L_801A7C98;
    r3 = *(u32*)lbl_8047B2D8;
    tmp = *(u32*)((u8*)r3 + 0x58);
    if (tmp == 0) goto L_801A7C98;
    r3 = *(u32*)lbl_8047B2D8;
    *(u32*)((u8*)r3 + 0x8) = tmp;
    tmp = *(u32*)lbl_8047B2D8;
    *(u32*)(sp + 0x8) = tmp;
L_801A7C98:
    fn_801BC33C();
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r5 = r30 + 0x1c;
    r12 = *(u32*)((u8*)r4 + 0x44);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = r30 + 0x18;
    r5 = r30 + 0x1c;
    fn_801B4300();
    if (r31 == 0) goto L_801A7CD8;
    tmp = 0x0;
    *(u32*)((u8*)r31 + 0x0) = tmp;
L_801A7CD8:
    r3 = r30;
    goto L_801A7CE4;
L_801A7CE0:
    r3 = 0x0;
L_801A7CE4:
    return;
}

/* 0x5C | fn_801A7CFC | generic */
void fn_801A7CFC(void) {
    /* refs: lbl_80274E6C, lbl_8036CB30, lbl_8047B2D0 */
    fn_80193788();
    fn_80196E10();
}

/* 0x801A7D58 | 0xE4 */
void fn_801A7D58(void) {
    extern u8 lbl_8047DC18[];
    extern u8 lbl_8047DC28[];
    extern u8 lbl_8047DC2C[];
    extern void fn_80193B10();
    extern void fn_80196E10();
    extern void fn_801BE4CC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r30 = r4;
    r29 = r3;
    tmp = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r3 + 0x4) = tmp;
    r3 = *(u32*)((u8*)r4 + 0x8);
    fn_801BE4CC();
    tmp = r3;
    r3 = 0x14;
    *(u32*)((u8*)r29 + 0x8) = tmp;
    fn_80193B10();
    /* mr. r31, r3 */;
    if ((s32)tmp != 0) goto L_801A7DB0;
    r3 = (u32)lbl_8047DC18;
    r4 = 0x466;
    r5 = (u32)lbl_8047DC28;
    fn_80196E10();
L_801A7DB0:
    r3 = r31;
    r4 = 0x0;
    r5 = 0x14;
    memset((void*)r3, (int)r4, (u32)r5);
    f0 = *(f32*)lbl_8047DC2C;
    tmp = r31;
    r5 = 0x14;
    *(f32*)((u8*)r31 + 0xC) = f0;
    *(u32*)((u8*)r29 + 0xC) = tmp;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r4 = *(u32*)((u8*)r30 + 0xC);
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    tmp = *(u32*)((u8*)r29 + 0x4);
    tmp = tmp | 0x1000;
    *(u32*)((u8*)r29 + 0x4) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x14);
    if (tmp == 0) goto L_801A7E14;
    r3 = 0xc;
    fn_80193B10();
    *(u32*)((u8*)r29 + 0x10) = r3;
    r5 = 0xc;
    r3 = *(u32*)((u8*)r29 + 0x10);
    r4 = *(u32*)((u8*)r30 + 0x14);
    memcpy((void*)r3, (const void*)r4, (u32)r5);
L_801A7E14:
    tmp = 0x0;
    r3 = 0x0;
    *(u32*)((u8*)r29 + 0x14) = tmp;
    return;
}

/* 0x48 | fn_801A7E3C | generic */
void fn_801A7E3C(void) {
    fn_801C27F4();
    fn_801BE800();
}

/* 0x801A7E84 | 0x4D0 */
void fn_801A7E84(void) {
    extern u8 lbl_8047DC2C[];
    extern u8 lbl_8047DC40[];
    extern u8 lbl_8047DC44[];
    extern u8 jumptable_8036CB84[];
    u8 sp[0x100];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    void (*ctr_fn)(void) = 0;

    if (r3 == 0) goto L_801A834C;
    if (r4 > 0xd) goto L_801A834C;
    r6 = (u32)jumptable_8036CB84;
    tmp = r4 << 2;
    r4 = (u32)jumptable_8036CB84;
    r4 = *(u32*)(r4 + tmp);
    ctr_fn = (void(*)(void))r4;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A7ED8;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A7EEC;
L_801A7ED8:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A7EEC;
    f1 = *(f32*)lbl_8047DC2C;
L_801A7EEC:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x0) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A7F34;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A7F48;
L_801A7F34:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A7F48;
    f1 = *(f32*)lbl_8047DC2C;
L_801A7F48:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x1) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A7F90;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A7FA4;
L_801A7F90:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A7FA4;
    f1 = *(f32*)lbl_8047DC2C;
L_801A7FA4:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x2) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A7FEC;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A8000;
L_801A7FEC:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A8000;
    f1 = *(f32*)lbl_8047DC2C;
L_801A8000:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x4) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A8048;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A805C;
L_801A8048:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A805C;
    f1 = *(f32*)lbl_8047DC2C;
L_801A805C:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x5) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A80A4;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A80B8;
L_801A80A4:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A80B8;
    f1 = *(f32*)lbl_8047DC2C;
L_801A80B8:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x6) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f2 = *(f32*)lbl_8047DC2C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    f1 = f2 - f1;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A8108;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A811C;
L_801A8108:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A811C;
    f1 = *(f32*)lbl_8047DC2C;
L_801A811C:
    r3 = *(u32*)((u8*)r3 + 0xC);
    *(f32*)((u8*)r3 + 0xC) = f1;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A8150;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A8164;
L_801A8150:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A8164;
    f1 = *(f32*)lbl_8047DC2C;
L_801A8164:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x8) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A81AC;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A81C0;
L_801A81AC:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A81C0;
    f1 = *(f32*)lbl_8047DC2C;
L_801A81C0:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x9) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0xC);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A8208;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A821C;
L_801A8208:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A821C;
    f1 = *(f32*)lbl_8047DC2C;
L_801A821C:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0xC);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0xA) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0x10);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A8264;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A8278;
L_801A8264:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A8278;
    f1 = *(f32*)lbl_8047DC2C;
L_801A8278:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0x10);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x1) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0x10);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A82C0;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A82D4;
L_801A82C0:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A82D4;
    f1 = *(f32*)lbl_8047DC2C;
L_801A82D4:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0x10);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x2) = tmp;
    goto L_801A834C;
    tmp = *(u32*)((u8*)r3 + 0x10);
    if (tmp == 0) goto L_801A834C;
    f1 = *(f32*)((u8*)r5 + 0x0);
    f0 = *(f32*)lbl_8047DC40;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_801A831C;
    f1 = *(f32*)lbl_8047DC40;
    goto L_801A8330;
L_801A831C:
    f0 = *(f32*)lbl_8047DC2C;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_801A8330;
    f1 = *(f32*)lbl_8047DC2C;
L_801A8330:
    f0 = *(f32*)lbl_8047DC44;
    r3 = *(u32*)((u8*)r3 + 0x10);
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(u8*)((u8*)r3 + 0x3) = tmp;
L_801A834C:
    return;
}
