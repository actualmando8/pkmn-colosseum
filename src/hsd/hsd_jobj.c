/**
 * @file hsd_jobj.c
 * @brief HSD JObj - Joint object implementation (skeletal hierarchy).
 *
 * Colosseum address: 0x8019CE50 (HSD_JObjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/jobj.c
 *
 * This is one of the largest and most critical HSD modules.
 * JObj manages the skeletal hierarchy that drives all 3D models.
 */

#include "hsd/hsd_jobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_pobj.h"
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_robj.h"

static void JObjInfoInit(void);

HSD_JObjInfo hsdJObj = { JObjInfoInit };

static HSD_ClassInfo* default_class = NULL;
static HSD_JObj* current_jobj = NULL;

/* ========================================================================= */
/*  Current JObj tracking                                                    */
/* ========================================================================= */

void HSD_JObjSetCurrent(HSD_JObj* jobj)
{
    current_jobj = jobj;
}

HSD_JObj* HSD_JObjGetCurrent(void)
{
    return current_jobj;
}

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_JObjGetFlags(HSD_JObj* jobj)
{
    HSD_ASSERT(0, jobj);
    return jobj->flags;
}

void HSD_JObjSetFlags(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        jobj->flags |= flags;
    }
}

void HSD_JObjSetFlagsAll(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        HSD_JObjSetFlags(jobj, flags);
        HSD_JObjSetFlagsAll(jobj->child, flags);
        HSD_JObjSetFlagsAll(jobj->next, flags);
    }
}

void HSD_JObjClearFlags(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        jobj->flags &= ~flags;
    }
}

void HSD_JObjClearFlagsAll(HSD_JObj* jobj, u32 flags)
{
    if (jobj != NULL) {
        HSD_JObjClearFlags(jobj, flags);
        HSD_JObjClearFlagsAll(jobj->child, flags);
        HSD_JObjClearFlagsAll(jobj->next, flags);
    }
}

/* ========================================================================= */
/*  DObj accessor                                                            */
/* ========================================================================= */

HSD_DObj* HSD_JObjGetDObj(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return NULL;
    }
    if (!union_type_dobj(jobj)) {
        return NULL;
    }
    return jobj->u.dobj;
}

/* ========================================================================= */
/*  Matrix dirty management                                                  */
/* ========================================================================= */

void HSD_JObjSetMtxDirtySub(HSD_JObj* jobj)
{
    HSD_JObj* child;

    if (jobj == NULL) {
        return;
    }
    jobj->flags |= JOBJ_MTX_DIRTY;
    child = jobj->child;
    while (child != NULL) {
        if (!(child->flags & JOBJ_MTX_INDEP_PARENT)) {
            HSD_JObjSetMtxDirtySub(child);
        }
        child = child->next;
    }
}

/* ========================================================================= */
/*  Hierarchy manipulation                                                   */
/* ========================================================================= */

void HSD_JObjAddChild(HSD_JObj* jobj, HSD_JObj* child)
{
    HSD_JObj* c;

    HSD_ASSERT(0, jobj);
    HSD_ASSERT(0, child);
    HSD_ASSERT(0, child->parent == NULL);

    child->parent = jobj;
    if (jobj->child == NULL) {
        jobj->child = child;
    } else {
        c = jobj->child;
        while (c->next != NULL) {
            c = c->next;
        }
        c->next = child;
    }
    HSD_JObjSetMtxDirty(child);
}

void HSD_JObjAddNext(HSD_JObj* jobj, HSD_JObj* next)
{
    HSD_ASSERT(0, jobj);
    HSD_ASSERT(0, next);

    next->parent = jobj->parent;
    next->next = jobj->next;
    jobj->next = next;
    HSD_JObjSetMtxDirty(next);
}

void HSD_JObjAddDObj(HSD_JObj* jobj, HSD_DObj* dobj)
{
    HSD_DObj* d;

    HSD_ASSERT(0, jobj);
    HSD_ASSERT(0, union_type_dobj(jobj));

    if (jobj->u.dobj == NULL) {
        jobj->u.dobj = dobj;
    } else {
        d = jobj->u.dobj;
        while (d->next != NULL) {
            d = d->next;
        }
        d->next = dobj;
    }
}

/* ========================================================================= */
/*  Reference counting                                                       */
/* ========================================================================= */

void HSD_JObjUnref(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    if (ref_DEC(jobj) != 0) {
        if (jobj != NULL) {
            ((HSD_ClassInfo*)jobj->object.parent.class_info)->release((HSD_Class*) jobj);
            ((HSD_ClassInfo*)jobj->object.parent.class_info)->destroy((HSD_Class*) jobj);
        }
    }
}

/* ========================================================================= */
/*  Remove                                                                   */
/* ========================================================================= */

HSD_JObj* HSD_JObjRemove(HSD_JObj* jobj)
{
    HSD_JObj* next;

    if (jobj == NULL) {
        return NULL;
    }

    next = jobj->next;

    /* Unlink from parent's child list */
    if (jobj->parent != NULL) {
        HSD_JObj* p = jobj->parent;
        if (p->child == jobj) {
            p->child = jobj->next;
        } else {
            HSD_JObj* c = p->child;
            while (c != NULL && c->next != jobj) {
                c = c->next;
            }
            if (c != NULL) {
                c->next = jobj->next;
            }
        }
    }

    jobj->parent = NULL;
    jobj->next = NULL;
    HSD_JObjUnref(jobj);

    return next;
}

void HSD_JObjRemoveAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_JObjRemoveAll(jobj->child);
    HSD_JObjRemoveAll(jobj->next);
    jobj->child = NULL;
    jobj->next = NULL;
    jobj->parent = NULL;
    HSD_JObjUnref(jobj);
}

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_JObjRemoveAnim(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_AObjRemove(jobj->aobj);
    jobj->aobj = NULL;
    if (union_type_dobj(jobj)) {
        /* Remove DObj animations would go here */
    }
    HSD_RObjRemoveAnimAll(jobj->robj);
}

void HSD_JObjRemoveAnimAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_JObjRemoveAnim(jobj);
    HSD_JObjRemoveAnimAll(jobj->child);
    HSD_JObjRemoveAnimAll(jobj->next);
}

void HSD_JObjReqAnimAll(HSD_JObj* jobj, f32 frame)
{
    if (jobj == NULL) {
        return;
    }
    HSD_AObjReqAnim(jobj->aobj, frame);
    HSD_RObjReqAnimAll(jobj->robj, frame);
    if (union_type_dobj(jobj)) {
        HSD_DObjReqAnimAll(jobj->u.dobj, frame);
    }
    HSD_JObjReqAnimAll(jobj->child, frame);
    HSD_JObjReqAnimAll(jobj->next, frame);
}

void HSD_JObjAddAnimAll(HSD_JObj* jobj, HSD_AnimJoint* animjoint,
                        HSD_MatAnimJoint* matanimjoint,
                        HSD_ShapeAnimJoint* shapeanimjoint)
{
    if (jobj == NULL) {
        return;
    }

    if (animjoint != NULL) {
        if (jobj->aobj != NULL) {
            HSD_AObjRemove(jobj->aobj);
        }
        jobj->aobj = HSD_AObjLoadDesc(animjoint->aobjdesc);
        HSD_RObjAddAnimAll(jobj->robj, animjoint->robj_anim);
    }

    if (union_type_dobj(jobj)) {
        HSD_DObjAddAnimAll(jobj->u.dobj,
            matanimjoint ? matanimjoint->matanim : NULL,
            shapeanimjoint ? shapeanimjoint->shapeanimdobj : NULL);
    }

    HSD_JObjAddAnimAll(jobj->child,
        animjoint ? animjoint->child : NULL,
        matanimjoint ? matanimjoint->child : NULL,
        shapeanimjoint ? shapeanimjoint->child : NULL);

    HSD_JObjAddAnimAll(jobj->next,
        animjoint ? animjoint->next : NULL,
        matanimjoint ? matanimjoint->next : NULL,
        shapeanimjoint ? shapeanimjoint->next : NULL);
}

void HSD_JObjAnim(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    /* Interpret AObj animation -> update JObj transform */
    HSD_RObjAnimAll(jobj->robj);
    if (union_type_dobj(jobj)) {
        HSD_DObjAnim(jobj->u.dobj);
    }
}

void HSD_JObjAnimAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_JObjAnim(jobj);
    HSD_JObjAnimAll(jobj->child);
    HSD_JObjAnimAll(jobj->next);
}

/* ========================================================================= */
/*  Alloc / Load                                                             */
/* ========================================================================= */

HSD_JObj* HSD_JObjAlloc(void)
{
    HSD_JObj* jobj;
    jobj = (HSD_JObj*) hsdNew(
        default_class ? default_class : &hsdJObj.parent.parent);
    HSD_ASSERT(0, jobj);
    return jobj;
}

void HSD_JObjSetDefaultClass(HSD_ClassInfo* info)
{
    if (info) {
        HSD_ASSERT(0, hsdIsDescendantOf(info, &hsdJObj));
    }
    default_class = info;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void JObjRelease(HSD_Class* o)
{
    HSD_JObj* jobj = (HSD_JObj*) o;

    /* Release children */
    HSD_JObjRemoveAll(jobj->child);
    jobj->child = NULL;

    /* Release DObj chain */
    if (union_type_dobj(jobj)) {
        HSD_DObjRemoveAll(jobj->u.dobj);
        jobj->u.dobj = NULL;
    }

    /* Release animation */
    HSD_AObjRemove(jobj->aobj);
    jobj->aobj = NULL;

    /* Release constraints */
    HSD_RObjRemoveAll(jobj->robj);
    jobj->robj = NULL;

    HSD_OBJECT_PARENT_INFO(&hsdJObj)->release(o);
}

static void JObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    current_jobj = NULL;
    HSD_OBJECT_PARENT_INFO(&hsdJObj)->amnesia(info);
}

static void JObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdJObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_jobj",
                     sizeof(HSD_JObjInfo), sizeof(HSD_JObj));
    HSD_CLASS_INFO(&hsdJObj)->release = JObjRelease;
    HSD_CLASS_INFO(&hsdJObj)->amnesia = JObjAmnesia;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 4 functions matched
 * =================================================================== */

extern u32 lbl_8047B2A0;
extern u32 lbl_8047B2A8;
extern u32 lbl_8047B2AC;

/* Address: 0x8019D610 | Size: 0x8 | Pattern: sda_setter */
void fn_8019D610(u32 val) {
    lbl_8047B2A8 = val;
}

/* Address: 0x8019D618 | Size: 0x8 | Pattern: sda_setter */
void fn_8019D618(u32 val) {
    lbl_8047B2A0 = val;
}

/* Address: 0x8019F01C | Size: 0x8 | Pattern: sda_getter */
u32 fn_8019F01C(void) {
    return lbl_8047B2AC;
}

/* Address: 0x801A1980 | Size: 0x8 | Pattern: addi_ptr_return */
u8* fn_801A1980(u8* obj) {
    return (u8*)obj + 0x54;
}

/* =========================================================================
 *  Internal stubs: 0x8019CB70-0x801A1988 (39 functions)
 * ========================================================================= */

/* 0x8019CB70 | 0x2E0 */
void fn_8019CB70(void) {
    extern u8 lbl_80274818[];
    extern u8 lbl_80465568[];
    extern u8 lbl_8046557C[];
    extern u8 lbl_80478C74[];
    extern u8 lbl_80478C7C[];
    extern u8 lbl_80478C80[];
    extern u8 lbl_80478C84[];
    extern u8 lbl_8047B278[];
    extern u8 lbl_8047B280[];
    extern u8 lbl_8047DB14[];
    extern u8 lbl_8047DB18[];
    extern u8 lbl_8047E728[];
    extern void fn_8009AFD0();
    extern void fn_800A8FE4();
    extern void fn_800BA198();
    extern void fn_800BA414();
    extern void fn_800BA424();
    extern void fn_800BA440();
    extern void fn_800BA44C();
    extern void fn_80196D78();
    extern void fn_80198F4C();
    extern void fn_8019B784();
    extern void fn_8019C358();
    extern void fn_8019C388();
    extern void fn_8019C978();
    extern void fn_801A3FBC();
    extern void fn_801A4A54();
    extern void fn_801A8478();
    extern void fn_801A84B4();
    extern void fn_801B0158();
    extern void fn_801B1854();
    extern void fn_801B25C4();
    extern void fn_801B4264();
    extern void fn_801BF1F0();
    extern void fn_801C2AB8();
    u8 sp[0xC0];
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
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;

    r3 = (u32)lbl_80274818;
    r31 = (u32)lbl_80274818;
    DVDInit();
    r5 = *(u32*)lbl_80478C74;
    r29 = *(u32*)lbl_80478C80;
    if (r5 != 0) {
        r4 = *(u16*)((u8*)r5 + 0x4);
        r3 = (u32)lbl_80465568;
        tmp = *(u16*)((u8*)r5 + 0x8);
        r3 = (u32)lbl_80465568;
        r4 = r4 + 0xf;
        r30 = 0x0;
        r4 = r4 & 0x0000FFF0;
        tmp = r4 * tmp;
        r27 = tmp << 1;
        tmp = r27 * r29;
        *(u32*)((u8*)r3 + 0x8) = tmp;
        while ((s32)r30 < (s32)r29) {

            r3 = r27;
            r4 = 0x20;
            fn_8009AFD0();
            r4 = (u32)lbl_8046557C;
            r5 = r30 << 2;
            tmp = (u32)lbl_8046557C;
            r4 = tmp + r5;
            *(u32*)((u8*)r4 + 0x0) = r3;
            if (r3 == 0) {
                r3 = r31 + 0x1cc;
                r5 = r31 + 0x260;
                r4 = 0xf1;
                fn_80196D78();
            }
            r30 = r30 + 0x1;

        }
        r30 = r29;
        while ((s32)r30 < 3) {

            r3 = (u32)lbl_8046557C;
            r4 = r30 << 2;
            tmp = (u32)lbl_8046557C;
            r5 = 0x0;
            r3 = tmp + r4;
            r30 = r30 + 0x1;
            *(u32*)((u8*)r3 + 0x0) = r5;

        }
    }
    r3 = *(u32*)lbl_80478C7C;
    r4 = 0x20;
    fn_8009AFD0();
    /* mr. r27, r3 */;
    if ((s32)r30 == 3) {
        r3 = r31 + 0x1cc;
        r5 = r31 + 0x240;
        r4 = 0x104;
        fn_80196D78();
    }
    r4 = *(u32*)lbl_80478C7C;
    r3 = r27;
    GXInit();
    r4 = (u32)lbl_80465568;
    tmp = *(u32*)lbl_80478C7C;
    r4 = (u32)lbl_80465568;
    *(u32*)lbl_8047B278 = r3;
    *(u32*)((u8*)r4 + 0xC) = tmp;
    fn_8019C978();
    r7 = *(u32*)lbl_80478C74;
    r11 = 0x1000000;
    r6 = *(u32*)lbl_8047E728;
    r5 = (u32)lbl_8046557C;
    r9 = *(u32*)((u8*)r7 + 0x0);
    r4 = (u32)lbl_8046557C;
    tmp = *(u32*)((u8*)r7 + 0x4);
    r3 = (u32)lbl_8046557C;
    r8 = (u32)lbl_8046557C;
    r5 = (u32)lbl_8046557C;
    r6 = (u32)lbl_8046557C;
    r31 = 0x1;
    *(u32*)(sp + 0x54) = tmp;
    r30 = 0x1;
    r29 = 0x0;
    r10 = 0x1;
    r27 = *(u32*)((u8*)r7 + 0x8);
    r9 = 0x1;
    r28 = *(u32*)((u8*)r7 + 0xC);
    tmp = 0x1;
    r4 = *(u32*)((u8*)r8 + 0x0);
    r3 = (u32)sp + 0x50;
    r5 = *(u32*)((u8*)r5 + 0x4);
    r6 = *(u32*)((u8*)r6 + 0x8);
    r28 = *(u32*)((u8*)r7 + 0x10);
    r8 = *(u32*)((u8*)r7 + 0x14);
    r28 = *(u32*)((u8*)r7 + 0x18);
    r8 = *(u32*)((u8*)r7 + 0x1C);
    r28 = *(u32*)((u8*)r7 + 0x20);
    r8 = *(u32*)((u8*)r7 + 0x24);
    r28 = *(u32*)((u8*)r7 + 0x28);
    r8 = *(u32*)((u8*)r7 + 0x2C);
    r28 = *(u32*)((u8*)r7 + 0x30);
    r8 = *(u32*)((u8*)r7 + 0x34);
    r7 = *(u32*)((u8*)r7 + 0x38);
    *(u8*)(sp + 0x90) = r30;
    *(u8*)(sp + 0xA0) = r10;
    *(u8*)(sp + 0xA1) = r9;
    *(u8*)(sp + 0xA2) = tmp;
    fn_801BF1F0();
    f1 = *(f32*)lbl_8047DB14;
    r3 = (u32)sp + 0x10;
    f2 = *(f32*)lbl_8047DB18;
    f3 = *(f32*)lbl_8047DB18;
    fn_800BA414();
    f1 = *(f32*)lbl_8047DB14;
    r3 = (u32)sp + 0x10;
    f2 = *(f32*)lbl_8047DB18;
    f3 = *(f32*)lbl_8047DB18;
    fn_800BA424();
    f1 = *(f32*)lbl_8047DB14;
    r3 = (u32)sp + 0x10;
    f2 = *(f32*)lbl_8047DB18;
    f3 = *(f32*)lbl_8047DB18;
    f4 = *(f32*)lbl_8047DB14;
    f5 = *(f32*)lbl_8047DB18;
    f6 = *(f32*)lbl_8047DB18;
    fn_800BA198();
    tmp = *(u32*)lbl_80478C84;
    r4 = (u32)sp + 0x8;
    r3 = (u32)sp + 0x10;
    *(u32*)(sp + 0x8) = tmp;
    fn_800BA440();
    r27 = 0x0;
    while ((s32)r27 < 8) {

        r3 = r27;
        fn_801A4A54();
        r4 = r3;
        r3 = (u32)sp + 0x10;
        fn_800BA44C();
        r27 = r27 + 0x1;

    }
    r3 = -0x1;
    fn_801B25C4();
    fn_8019C358();
    fn_800A8FE4();
    fn_801A3FBC();
    fn_801C2AB8();
    fn_8019B784();
    fn_8019C388();
    fn_801A84B4();
    fn_801A8478();
    fn_801B0158();
    fn_801B4264();
    fn_801B1854();
    fn_80198F4C();
    tmp = 0x1;
    *(u32*)lbl_8047B280 = tmp;
    return;
}

/* 0x8019CE50 | 0x104 */
void fn_8019CE50(void) {
    extern u8 lbl_80274AD0[];
    extern u8 lbl_80274AE8[];
    extern u8 lbl_8036C8E0[];
    extern u8 lbl_8036CC00[];
    extern void fn_80193B30();
    extern void fn_80197998();
    extern void fn_80197B6C();
    extern void fn_8019CF54();
    extern void fn_8019CFBC();
    extern void fn_8019D05C();
    extern void fn_8019D5A0();
    extern void fn_801A1098();
    extern void fn_801A20C8();
    extern void fn_801A3600();
    u8 sp[0x30];
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
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)lbl_8036C8E0;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274AD0;
    r6 = (u32)lbl_80274AE8;
    r3 = (u32)lbl_8036C8E0;
    r4 = (u32)lbl_8036CC00;
    r5 = (u32)lbl_80274AD0;
    r6 = (u32)lbl_80274AE8;
    r7 = 0x54;
    r8 = 0x88;
    fn_80193B30();
    r11 = (u32)fn_8019D5A0;
    r10 = (u32)fn_8019CFBC;
    r6 = (u32)fn_801A1098;
    r4 = (u32)fn_8019D05C;
    r3 = (u32)fn_801A20C8;
    r7 = (u32)fn_80197B6C;
    r5 = (u32)fn_80197998;
    r31 = (u32)fn_8019CFBC;
    tmp = (u32)fn_801A1098;
    r25 = (u32)fn_8019D05C;
    r27 = (u32)fn_801A20C8;
    r30 = (u32)lbl_8036C8E0;
    r12 = (u32)lbl_8036C8E0;
    r10 = (u32)lbl_8036C8E0;
    r6 = (u32)lbl_8036C8E0;
    r4 = (u32)lbl_8036C8E0;
    r3 = (u32)lbl_8036C8E0;
    r26 = (u32)lbl_8036C8E0;
    r28 = (u32)lbl_8036C8E0;
    r12 = (u32)lbl_8036C8E0;
    r3 = (u32)lbl_8036C8E0;
    r26 = (u32)lbl_8036C8E0;
    r28 = (u32)lbl_8036C8E0;
    r9 = (u32)fn_8019CF54;
    r29 = (u32)fn_8019D5A0;
    r30 = (u32)lbl_8036C8E0;
    r8 = (u32)fn_801A3600;
    r11 = (u32)fn_8019CF54;
    r9 = (u32)fn_801A3600;
    r10 = (u32)lbl_8036C8E0;
    r8 = (u32)lbl_8036C8E0;
    r7 = (u32)fn_80197B6C;
    r8 = (u32)lbl_8036C8E0;
    r6 = (u32)lbl_8036C8E0;
    r5 = (u32)fn_80197998;
    r4 = (u32)lbl_8036C8E0;
    *(u32*)((u8*)r30 + 0x2C) = r29;
    *(u32*)((u8*)r12 + 0x30) = r31;
    *(u32*)((u8*)r10 + 0x38) = r11;
    *(u32*)((u8*)r8 + 0x40) = r9;
    *(u32*)((u8*)r6 + 0x44) = r7;
    *(u32*)((u8*)r4 + 0x48) = r5;
    *(u32*)((u8*)r3 + 0x3C) = tmp;
    *(u32*)((u8*)r26 + 0x4C) = r25;
    *(u32*)((u8*)r28 + 0x50) = r27;
    return;
}

/* 0x68 | fn_8019CF54 | framed_no_calls */
void fn_8019CF54(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B298, lbl_8047B29C, lbl_8047B2AC */
}

/* 0x8019CFBC | 0xA0 */
void fn_8019CFBC(void) {
    extern u8 lbl_8036C8E0[];
    extern void fn_8019C128();
    extern void fn_8019C1B0();
    extern void fn_801A84F0();
    extern void fn_801A8570();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = *(u32*)((u8*)r31 + 0x84);
    r3 = 0x0;
    r5 = 0x0;
    fn_8019C128();
    if (r3 == r31) {
        r4 = *(u32*)((u8*)r31 + 0x84);
        r3 = 0x0;
        fn_8019C1B0();
    }
    tmp = *(u32*)((u8*)r31 + 0x74);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r31 + 0x74);
        fn_801A8570();
    }
    tmp = *(u32*)((u8*)r31 + 0x78);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r31 + 0x78);
        fn_801A84F0();
    }
    r4 = (u32)lbl_8036C8E0;
    r3 = r31;
    r4 = (u32)lbl_8036C8E0;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

/* 0x8019D05C | 0x544 */
void fn_8019D05C(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB28[];
    extern void fn_80196D78();
    extern void fn_80196E10();
    extern void fn_80199264();
    extern void fn_801AE50C();
    extern void fn_801C25E4();
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r28 = r3;
    r3 = (u32)lbl_80274AA0;
    tmp = *(u32*)((u8*)r28 + 0x10);
    r31 = (u32)lbl_80274AA0;
    if (tmp != 0) {
    do {
        tmp = *(u32*)((u8*)r28 + 0x14);
        tmp = tmp & 0x00001000;
        if (tmp != 0) {
            r30 = *(u32*)((u8*)r28 + 0x10);
            if (r30 == 0) break;
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
            if ((s32)r3 == 0) break;
            r3 = *(u16*)((u8*)r30 + 0x6);
            /* subic. tmp, r3, 0x1 */;
            if ((s32)r3 < 0) {
                if (r30 == 0) break;
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
                break;
            }
            r3 = *(u16*)((u8*)r30 + 0x6);
            tmp = r3 + 0x1;
            *(u16*)((u8*)r30 + 0x6) = tmp;
            tmp = *(u16*)((u8*)r30 + 0x6);
            if (tmp == 0) {
                r3 = r31 + 0x54;
                r5 = r31 + 0x60;
                r4 = 0x9e;
                fn_80196E10();
            }
            r4 = *(u32*)((u8*)r30 + 0x0);
            r3 = r30;
            r12 = *(u32*)((u8*)r4 + 0x4C);
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
            tmp = *(u16*)((u8*)r30 + 0x6);
            tmp = __cntlzw(tmp);
            /* srwi. tmp, tmp, 5 */;
            if (tmp != 0) {
            } else {

                r3 = *(u16*)((u8*)r30 + 0x6);
                *(u16*)((u8*)r30 + 0x6) = tmp;
                tmp = *(u16*)((u8*)r30 + 0x6);
                tmp = __cntlzw(tmp);
                tmp = (u32)tmp >> 5;
            }
            if ((s32)tmp == 0 || r30 == 0) break;

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
            break;
        }
        r3 = *(u32*)((u8*)r28 + 0x10);
        tmp = 0x0;
        *(u32*)((u8*)r3 + 0xC) = tmp;
        r29 = *(u32*)((u8*)r28 + 0x10);
        if (r29 == 0) break;
        tmp = *(u32*)((u8*)r29 + 0xC);
        if (tmp != 0) {
            do {
                if (r29 != 0) {
                    tmp = *(u32*)((u8*)r29 + 0xC);
                    if (tmp == 0) {
                    }
                    r3 = 0x0;
                    break;
                    }
                r3 = *(u32*)((u8*)r29 + 0xC);
                tmp = *(u32*)((u8*)r3 + 0x10);
                if (r29 == tmp) {
                    r3 = 0x0;
                    break;
                }
                r3 = *(u32*)((u8*)r29 + 0xC);
                r3 = *(u32*)((u8*)r3 + 0x10);
                while (1) {
                    if (r3 == 0) break;
                    tmp = *(u32*)((u8*)r3 + 0x8);
                    if (tmp == r29) {
                        break;
                    }
                    r3 = *(u32*)((u8*)r3 + 0x8);

                }
                r5 = r31 + 0x88;
                r3 = (u32)lbl_8047DB20;
                r4 = 0x5f8;
                fn_80196D78();
                r3 = 0x0;
            } while (0);

            if (r3 != 0) {
                tmp = 0x0;
                *(u32*)((u8*)r3 + 0x8) = tmp;
        }
            if (r29 != 0) continue;
        break;
    }
    } while (0);
        tmp = 0x0;
        *(u32*)((u8*)r28 + 0x10) = tmp;
    }
    tmp = *(u32*)((u8*)r28 + 0xC);
    if (tmp != 0 && r28 != 0) {

        tmp = *(u32*)((u8*)r28 + 0xC);
        r29 = *(u32*)((u8*)r28 + 0x8);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r28 + 0xC);
            tmp = *(u32*)((u8*)r3 + 0x10);
            if (tmp == r28) {
                r3 = *(u32*)((u8*)r28 + 0xC);
                *(u32*)((u8*)r3 + 0x10) = r29;

            } else {
                do {
                    if (r28 != 0) {
                        tmp = *(u32*)((u8*)r28 + 0xC);
                        if (tmp == 0) {
                        }
                        r30 = 0x0;
                        break;
                        }
                    r3 = *(u32*)((u8*)r28 + 0xC);
                    tmp = *(u32*)((u8*)r3 + 0x10);
                    if (r28 == tmp) {
                        r30 = 0x0;
                        break;
                    }
                    r3 = *(u32*)((u8*)r28 + 0xC);
                    r30 = *(u32*)((u8*)r3 + 0x10);
                    while (1) {
                        if (r30 == 0) break;
                        tmp = *(u32*)((u8*)r30 + 0x8);
                        if (tmp == r28) {
                            break;
                        }
                        r30 = *(u32*)((u8*)r30 + 0x8);

                    }
                    r5 = r31 + 0x88;
                    r3 = (u32)lbl_8047DB20;
                    r4 = 0x5f8;
                    fn_80196D78();
                    r30 = 0x0;
                } while (0);

                if (r30 == 0) {
                    r3 = (u32)lbl_8047DB20;
                    r4 = 0x57b;
                    r5 = (u32)lbl_8047DB28;
                    fn_80196E10();
                }
                *(u32*)((u8*)r30 + 0x8) = r29;
            }
            r6 = *(u32*)((u8*)r28 + 0xC);
            while (1) {
                if (r6 == 0) break;
                r4 = 0x90000000;
                r5 = *(u32*)((u8*)r6 + 0x10);
                while (r5 != 0) {

                    tmp = *(u32*)((u8*)r5 + 0x14);
                    r3 = *(u32*)((u8*)r5 + 0x14);
                    tmp = tmp << 10;
                    r5 = *(u32*)((u8*)r5 + 0x8);
                    tmp = r3 | tmp;
                    tmp = tmp & 0x70000000;
                    r4 = r4 | tmp;

                }
                tmp = *(u32*)((u8*)r6 + 0x14);
                /* andc. tmp, tmp, r4 */;
                if (r5 == 0) break;
                tmp = *(u32*)((u8*)r6 + 0x14);
                tmp = tmp & r4;
                *(u32*)((u8*)r6 + 0x14) = tmp;
                r6 = *(u32*)((u8*)r6 + 0x8);

            }

            tmp = 0x0;
            *(u32*)((u8*)r28 + 0xC) = tmp;
        }
        tmp = 0x0;
        *(u32*)((u8*)r28 + 0x8) = tmp;
    }
    tmp = *(u32*)((u8*)r28 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (r6 != 0) {
        tmp = *(u32*)((u8*)r28 + 0x18);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r28 + 0x18);
            fn_80199264();
            tmp = 0x0;
            *(u32*)((u8*)r28 + 0x18) = tmp;
    }
    }
    tmp = *(u32*)((u8*)r28 + 0x80);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r28 + 0x80);
        fn_801AE50C();
        tmp = 0x0;
        *(u32*)((u8*)r28 + 0x80) = tmp;
    }
    tmp = *(u32*)((u8*)r28 + 0x7C);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r28 + 0x7C);
        fn_801C25E4();
        tmp = 0x0;
        *(u32*)((u8*)r28 + 0x7C) = tmp;
    }
    return;
}

/* 0x70 | fn_8019D5A0 | framed_no_calls */
u32 fn_8019D5A0(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047DB30 */
    return 0;
}

/* 0x8019D620 | 0x360 */
void fn_8019D620(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
    extern void fn_8019D980();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r26 = r3;
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r3 + 0x14) = tmp;
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00001000;
    if ((s32)tmp != 0) return;
    r26 = *(u32*)((u8*)r26 + 0x10);
    while (1) {
        if (r26 == 0) break;
        tmp = *(u32*)((u8*)r26 + 0x14);
        tmp = tmp & 0x01000000;
        do {
        if ((s32)tmp != 0) break;

        if (r26 == 0) {
            r3 = (u32)lbl_8047DB34;
            r4 = 0x25d;
            r5 = (u32)lbl_8047DB3C;
            fn_80196E10();
        }
        tmp = *(u32*)((u8*)r26 + 0x14);
        r3 = 0x0;
        tmp = tmp & 0x00800000;
        if (r26 == 0) {
            tmp = *(u32*)((u8*)r26 + 0x14);
            tmp = tmp & 0x00000040;
            if (r26 != 0) {
                r3 = 0x1;
        }
        }
        if ((s32)r3 != 0) break;

        tmp = *(u32*)((u8*)r26 + 0x14);
        r31 = r26;
        tmp = tmp | 0x40;
        *(u32*)((u8*)r26 + 0x14) = tmp;
        tmp = *(u32*)((u8*)r26 + 0x14);
        tmp = tmp & 0x00001000;
        if ((s32)r3 != 0) break;

        r31 = *(u32*)((u8*)r31 + 0x10);
        while (1) {
            if (r31 == 0) break;
            tmp = *(u32*)((u8*)r31 + 0x14);
            tmp = tmp & 0x01000000;
            do {
            if ((s32)r3 != 0) break;

            if (r31 == 0) {
                r3 = (u32)lbl_8047DB34;
                r4 = 0x25d;
                r5 = (u32)lbl_8047DB3C;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r31 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r31 == 0) {
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp & 0x00000040;
                if (r31 != 0) {
                    r3 = 0x1;
            }
            }
            if ((s32)r3 != 0) break;

            tmp = *(u32*)((u8*)r31 + 0x14);
            r30 = r31;
            tmp = tmp | 0x40;
            *(u32*)((u8*)r31 + 0x14) = tmp;
            tmp = *(u32*)((u8*)r31 + 0x14);
            tmp = tmp & 0x00001000;
            if ((s32)r3 != 0) break;

            r30 = *(u32*)((u8*)r30 + 0x10);
            while (1) {
                if (r30 == 0) break;
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x01000000;
                do {
                if ((s32)r3 != 0) break;

                if (r30 == 0) {
                    r3 = (u32)lbl_8047DB34;
                    r4 = 0x25d;
                    r5 = (u32)lbl_8047DB3C;
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
                if ((s32)r3 != 0) break;

                tmp = *(u32*)((u8*)r30 + 0x14);
                r29 = r30;
                tmp = tmp | 0x40;
                *(u32*)((u8*)r30 + 0x14) = tmp;
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00001000;
                if ((s32)r3 != 0) break;

                r29 = *(u32*)((u8*)r29 + 0x10);
                while (1) {
                    if (r29 == 0) break;
                    tmp = *(u32*)((u8*)r29 + 0x14);
                    tmp = tmp & 0x01000000;
                    do {
                    if ((s32)r3 != 0) break;

                    if (r29 == 0) {
                        r3 = (u32)lbl_8047DB34;
                        r4 = 0x25d;
                        r5 = (u32)lbl_8047DB3C;
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
                    if ((s32)r3 != 0) break;

                    tmp = *(u32*)((u8*)r29 + 0x14);
                    r28 = r29;
                    tmp = tmp | 0x40;
                    *(u32*)((u8*)r29 + 0x14) = tmp;
                    tmp = *(u32*)((u8*)r29 + 0x14);
                    tmp = tmp & 0x00001000;
                    if ((s32)r3 != 0) break;

                    r28 = *(u32*)((u8*)r28 + 0x10);
                    while (1) {
                        if (r28 == 0) break;
                        tmp = *(u32*)((u8*)r28 + 0x14);
                        tmp = tmp & 0x01000000;
                        do {
                            if ((s32)r3 != 0) break;
                            if (r28 == 0) {
                                r3 = (u32)lbl_8047DB34;
                                r4 = 0x25d;
                                r5 = (u32)lbl_8047DB3C;
                                fn_80196E10();
                            }
                            tmp = *(u32*)((u8*)r28 + 0x14);
                            r3 = 0x0;
                            tmp = tmp & 0x00800000;
                            if (r28 == 0) {
                                tmp = *(u32*)((u8*)r28 + 0x14);
                                tmp = tmp & 0x00000040;
                                if (r28 != 0) {
                                    r3 = 0x1;
                            }
                            }
                            if ((s32)r3 != 0) break;
                            tmp = *(u32*)((u8*)r28 + 0x14);
                            r27 = r28;
                            tmp = tmp | 0x40;
                            *(u32*)((u8*)r28 + 0x14) = tmp;
                            tmp = *(u32*)((u8*)r28 + 0x14);
                            tmp = tmp & 0x00001000;
                            if ((s32)r3 != 0) break;
                            r27 = *(u32*)((u8*)r27 + 0x10);
                            while (r27 != 0) {

                                tmp = *(u32*)((u8*)r27 + 0x14);
                                tmp = tmp & 0x01000000;
                                if ((s32)r3 == 0) {
                                    if (r27 == 0) {
                                        r3 = (u32)lbl_8047DB34;
                                        r4 = 0x25d;
                                        r5 = (u32)lbl_8047DB3C;
                                        fn_80196E10();
                                    }
                                    tmp = *(u32*)((u8*)r27 + 0x14);
                                    r3 = 0x0;
                                    tmp = tmp & 0x00800000;
                                    if (r27 == 0) {
                                        tmp = *(u32*)((u8*)r27 + 0x14);
                                        tmp = tmp & 0x00000040;
                                        if (r27 != 0) {
                                            r3 = 0x1;
                                    }
                                    }
                                    if ((s32)r3 == 0) {
                                        tmp = *(u32*)((u8*)r27 + 0x14);
                                        r25 = r27;
                                        tmp = tmp | 0x40;
                                        *(u32*)((u8*)r27 + 0x14) = tmp;
                                        tmp = *(u32*)((u8*)r27 + 0x14);
                                        tmp = tmp & 0x00001000;
                                        if ((s32)r3 == 0) {
                                            r25 = *(u32*)((u8*)r25 + 0x10);
                                            while (r25 != 0) {

                                                tmp = *(u32*)((u8*)r25 + 0x14);
                                                tmp = tmp & 0x01000000;
                                                if ((s32)r3 == 0) {
                                                    r3 = r25;
                                                    fn_8019D980();
                                                    if ((s32)r3 == 0) {
                                                        r3 = r25;
                                                        fn_8019D620();
                                                }
                                                }
                                                r25 = *(u32*)((u8*)r25 + 0x8);

                                            }
                                }
                                }
                                }
                                r27 = *(u32*)((u8*)r27 + 0x8);

                            }
                        } while (0);

                        r28 = *(u32*)((u8*)r28 + 0x8);

                    }
                    } while (0);

                    r29 = *(u32*)((u8*)r29 + 0x8);

                }
                } while (0);

                r30 = *(u32*)((u8*)r30 + 0x8);

            }
            } while (0);

            r31 = *(u32*)((u8*)r31 + 0x8);

        }
        } while (0);

        r26 = *(u32*)((u8*)r26 + 0x8);

    }

    return;
}

/* 0x5C | fn_8019D980 | generic */
u32 fn_8019D980(void) {
    fn_80196E10();
    return 1;
}

/* 0x8019D9DC | 0x324 */
void fn_8019D9DC(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047DB30[];
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern u8 lbl_8047DB44[];
    extern u8 lbl_8047DB48[];
    extern u8 lbl_8047DB50[];
    extern u8 lbl_8047DB58[];
    extern u8 lbl_8047DB60[];
    extern void fn_800A3A78();
    extern void fn_800A3AC0();
    extern void fn_800A3B7C();
    extern void fn_80196E10();
    extern void fn_8019DD00();
    extern void fn_8019E460();
    extern void fn_801AED88();
    extern void fn_801B00E0();
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
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
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r4 = *(u32*)((u8*)r3 + 0x0);
    r31 = r3;
    r12 = *(u32*)((u8*)r4 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0xFFFFFFBF;
    *(u32*)((u8*)r31 + 0x14) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00800000;
    if ((s32)tmp != 0) return;
    r3 = *(u32*)((u8*)r31 + 0x14);
    tmp = 0x400000;
    r3 = r3 & 0x00600000;
    if ((s32)r3 != (s32)tmp) {
        if ((s32)r3 < (s32)tmp) {
            tmp = 0x200000;
            if ((s32)r3 != (s32)tmp) {
                goto L_8019DC3C;
            }
            tmp = 0x600000;
            if ((s32)r3 != (s32)tmp) {
                goto L_8019DC3C;
                }
            r3 = r31;
            fn_8019E460();
            goto L_8019DCD0;
        }
        r3 = r31;
        fn_8019DD00();
        goto L_8019DCD0;
            }
    r29 = *(u32*)((u8*)r31 + 0xC);
    f31 = *(f32*)lbl_8047DB30;
    if (r29 != 0) {
        r3 = *(u32*)((u8*)r29 + 0x80);
        r4 = 0x40000000;
        r5 = 0x0;
        fn_801B00E0();
        /* mr. r30, r3 */;
        if (r29 != 0) {
            f0 = *(f32*)((u8*)r29 + 0x50);
            r3 = (u32)sp + 0xc;
            r4 = (u32)sp + 0xc;
            *(f32*)(sp + 0x18) = f0;
            f0 = *(f32*)((u8*)r29 + 0x60);
            *(f32*)(sp + 0x1C) = f0;
            f0 = *(f32*)((u8*)r29 + 0x70);
            *(f32*)(sp + 0x20) = f0;
            f0 = *(f32*)((u8*)r29 + 0x44);
            *(f32*)(sp + 0xC) = f0;
            f0 = *(f32*)((u8*)r29 + 0x54);
            *(f32*)(sp + 0x10) = f0;
            f0 = *(f32*)((u8*)r29 + 0x64);
            *(f32*)(sp + 0x14) = f0;
            fn_800A3B7C();
            f3 = *(f32*)lbl_8047DB44;
            f2 = *(f32*)lbl_8047DB30;
            f1 = f3 + f1;
            f0 = *(f32*)lbl_8047DB48;
            f1 = f2 / f1;
            if (f1 > f0) {
                /* frsqrte f8, f1 */;
                f7 = *(f64*)lbl_8047DB50;
                f4 = *(f64*)lbl_8047DB58;
                f5 = *(f64*)lbl_8047DB50;
                f6 = f8 * f8;
                f2 = *(f64*)lbl_8047DB58;
                f3 = *(f64*)lbl_8047DB50;
                f7 = f7 * f8;
                f0 = *(f64*)lbl_8047DB58;
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

            } else {
                f0 = *(f64*)lbl_8047DB60;
                if (f1 < f0) {
                    r3 = (u32)lbl_80478AC0;
                    f1 = *(f32*)lbl_80478AC0;

                } else {
                do {
                    *(f32*)(sp + 0x8) = f1;
                    tmp = 0x7F800000;
                    r3 = r3 & 0x7F800000;
                    if ((s32)r3 != (s32)tmp) {
                        if ((s32)r3 >= (s32)tmp) goto L_8019DBC8;
                        if ((s32)r3 != 0) {
                            goto L_8019DBC8;
                        }
                        tmp = tmp & 0x7FFFFF;
                        if ((s32)r3 != 0) {
                            tmp = 0x1;
                            break;
                        }
                        tmp = 0x2;
                        break;
                        }
                    tmp = tmp & 0x7FFFFF;
                    if ((s32)r3 != 0) {
                        tmp = 0x5;
                        break;
                    }
                    tmp = 0x3;
                    break;
                L_8019DBC8:
                    tmp = 0x4;
                } while (0);
                    if ((s32)tmp == 1) {
                        r3 = (u32)lbl_80478AC0;
                        f1 = *(f32*)lbl_80478AC0;
                    }
                }
            }
            r3 = (u32)sp + 0xc;
            r4 = (u32)sp + 0xc;
            fn_800A3AC0();
            tmp = *(u32*)((u8*)r29 + 0x74);
            if (tmp != 0) {
                r3 = *(u32*)((u8*)r29 + 0x74);
                f31 = *(f32*)((u8*)r3 + 0x0);
            }
            f0 = *(f32*)((u8*)r30 + 0x8);
            r3 = (u32)sp + 0xc;
            r4 = (u32)sp + 0xc;
            f1 = f0 * f31;
            fn_800A3AC0();
            r3 = (u32)sp + 0x18;
            r4 = (u32)sp + 0xc;
            r5 = (u32)sp + 0x24;
            fn_800A3A78();
            f0 = *(f32*)(sp + 0x24);
            *(f32*)((u8*)r31 + 0x50) = f0;
            f0 = *(f32*)(sp + 0x28);
            *(f32*)((u8*)r31 + 0x60) = f0;
            f0 = *(f32*)(sp + 0x2C);
            *(f32*)((u8*)r31 + 0x70) = f0;
    }
    }
    goto L_8019DCD0;
L_8019DC3C:
    tmp = *(u32*)((u8*)r31 + 0x80);
    if (tmp != 0 || r31 == 0) {

        tmp = *(u32*)((u8*)r31 + 0x80);
        if (tmp != 0) {
            r5 = *(u32*)((u8*)r31 + 0x0);
            r4 = r31;
            r3 = *(u32*)((u8*)r31 + 0x80);
            r5 = *(u32*)((u8*)r5 + 0x50);
            fn_801AED88();
            if (r31 == 0) {
                r3 = (u32)lbl_8047DB34;
                r4 = 0x25d;
                r5 = (u32)lbl_8047DB3C;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r31 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r31 == 0) {
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp & 0x00000040;
                if (r31 != 0) {
                    r3 = 0x1;
            }
            }
        }
        if ((s32)r3 == 0) goto L_8019DCD0;
        r4 = *(u32*)((u8*)r31 + 0x0);
        r3 = r31;
        r12 = *(u32*)((u8*)r4 + 0x40);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        tmp = *(u32*)((u8*)r31 + 0x14);
        tmp = tmp & 0xFFFFFFBF;
        *(u32*)((u8*)r31 + 0x14) = tmp;
    }
L_8019DCD0:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0xFFFFFFBF;
    *(u32*)((u8*)r31 + 0x14) = tmp;

    return;
}

/* 0x8019DD00 | 0x760 */
void fn_8019DD00(void) {
    extern u8 lbl_80274AC4[];
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB30[];
    extern u8 lbl_8047DB44[];
    extern u8 lbl_8047DB48[];
    extern u8 lbl_8047DB50[];
    extern u8 lbl_8047DB58[];
    extern u8 lbl_8047DB60[];
    extern u8 lbl_8047DB68[];
    extern u8 lbl_8047DB6C[];
    extern u8 lbl_8047DB74[];
    extern u8 lbl_8047DB78[];
    extern void fn_800A3244();
    extern void fn_800A37CC();
    extern void fn_800A3A78();
    extern void fn_800A3A9C();
    extern void fn_800A3AC0();
    extern void fn_800A3ADC();
    extern void fn_800A3B7C();
    extern void fn_800A3B9C();
    extern void fn_800CE298();
    extern void fn_80196E10();
    extern void fn_801B00E0();
    u8 sp[0xD0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
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
    f32 f31 = 0.0f;

    r4 = (u32)lbl_80274AC4;
    r30 = r3;
    r5 = (u32)lbl_80274AC4;
    f31 = *(f32*)lbl_8047DB30;
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    tmp = *(u32*)((u8*)r5 + 0x8);
    r31 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)(sp + 0x70) = tmp;
    while (1) {
        if (r31 == 0) break;
        tmp = *(u32*)((u8*)r31 + 0x14);
        r3 = tmp & 0x00600000;
        /* subis tmp, r3, 0x60 */;
        if (tmp == 0) {
            break;
        }
        r31 = *(u32*)((u8*)r31 + 0x8);

    }
    r31 = 0x0;

    if (r31 == 0) {
        r3 = (u32)lbl_8047DB20;
        r4 = 0x82d;
        r5 = (u32)lbl_8047DB68;
        fn_80196E10();
    }
    r3 = *(u32*)((u8*)r31 + 0x80);
    r4 = 0x10000000;
    r5 = 0x1;
    fn_801B00E0();
    if (r3 != 0) {
    } else {

        r31 = 0x0;
    }
    if (r31 == 0) return;
    tmp = *(u32*)((u8*)r30 + 0xC);
    if (tmp == 0) {
        return;
    }
    tmp = *(u32*)((u8*)r30 + 0x74);
    if (tmp != 0) {
        r5 = *(u32*)((u8*)r30 + 0x74);
        r4 = *(u32*)((u8*)r5 + 0x0);
        r3 = *(u32*)((u8*)r5 + 0x4);
        tmp = *(u32*)((u8*)r5 + 0x8);
        *(u32*)(sp + 0x70) = tmp;
    }
    r5 = *(u32*)((u8*)r30 + 0xC);
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x44;
    r5 = r5 + 0x44;
    f0 = *(f32*)((u8*)r5 + 0xC);
    *(f32*)(sp + 0x50) = f0;
    f0 = *(f32*)((u8*)r5 + 0x1C);
    *(f32*)(sp + 0x54) = f0;
    f0 = *(f32*)((u8*)r5 + 0x2C);
    *(f32*)(sp + 0x58) = f0;
    r5 = *(u32*)((u8*)r30 + 0xC);
    f0 = *(f32*)((u8*)r5 + 0x44);
    *(f32*)(sp + 0x44) = f0;
    f0 = *(f32*)((u8*)r5 + 0x10);
    *(f32*)(sp + 0x48) = f0;
    f0 = *(f32*)((u8*)r5 + 0x20);
    *(f32*)(sp + 0x4C) = f0;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f8 = f2 / f1;
    if (f8 > f0) {
        /* frsqrte f7, f8 */;
        f6 = *(f64*)lbl_8047DB50;
        f3 = *(f64*)lbl_8047DB58;
        f4 = *(f64*)lbl_8047DB50;
        f5 = f7 * f7;
        f1 = *(f64*)lbl_8047DB58;
        f2 = *(f64*)lbl_8047DB50;
        f6 = f6 * f7;
        f0 = *(f64*)lbl_8047DB58;
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

    } else {
        f0 = *(f64*)lbl_8047DB60;
        if (f8 < f0) {
            r3 = (u32)lbl_80478AC0;
            f8 = *(f32*)lbl_80478AC0;

        } else {
        do {
            *(f32*)(sp + 0x10) = f8;
            tmp = 0x7F800000;
            r3 = r3 & 0x7F800000;
            if ((s32)r3 != (s32)tmp) {
                if ((s32)r3 >= (s32)tmp) goto L_8019DF1C;
                if ((s32)r3 != 0) {
                    goto L_8019DF1C;
                }
                tmp = tmp & 0x7FFFFF;
                if ((s32)r3 != 0) {
                    tmp = 0x1;
                    break;
                }
                tmp = 0x2;
                break;
                }
            tmp = tmp & 0x7FFFFF;
            if ((s32)r3 != 0) {
                tmp = 0x5;
                break;
            }
            tmp = 0x3;
            break;
        L_8019DF1C:
            tmp = 0x4;
        } while (0);
            if ((s32)tmp == 1) {
                r3 = (u32)lbl_80478AC0;
                f8 = *(f32*)lbl_80478AC0;
            }
        }
    }
do {
    f1 = f8;
do {
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x44;
    fn_800A3AC0();
    r3 = *(u32*)((u8*)r30 + 0xC);
    tmp = *(u32*)((u8*)r3 + 0x74);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r30 + 0xC);
        r3 = *(u32*)((u8*)r3 + 0x74);
        f31 = *(f32*)((u8*)r3 + 0x0);
    }
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x40000000;
    r5 = 0x0;
    r3 = *(u32*)((u8*)r3 + 0x80);
    fn_801B00E0();
    /* mr. r29, r3 */;
    if (tmp == 0) {
        r3 = (u32)lbl_8047DB20;
        r4 = 0x905;
        r5 = (u32)lbl_8047DB6C;
        fn_80196E10();
    }
    f0 = *(f32*)((u8*)r29 + 0x8);
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x44;
    f1 = f0 * f31;
    fn_800A3AC0();
    r3 = (u32)sp + 0x50;
    r4 = (u32)sp + 0x44;
    r5 = (u32)sp + 0x5c;
    fn_800A3A78();
    r3 = r31 + 0x38;
    r4 = (u32)sp + 0x5c;
    r5 = (u32)sp + 0x44;
    fn_800A3A9C();
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x44;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f1 = f2 / f1;
    if (f1 > f0) {
        /* frsqrte f8, f1 */;
        f7 = *(f64*)lbl_8047DB50;
        f4 = *(f64*)lbl_8047DB58;
        f5 = *(f64*)lbl_8047DB50;
        f6 = f8 * f8;
        f2 = *(f64*)lbl_8047DB58;
        f3 = *(f64*)lbl_8047DB50;
        f7 = f7 * f8;
        f0 = *(f64*)lbl_8047DB58;
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
        break;
    }
    f0 = *(f64*)lbl_8047DB60;
    if (f1 < f0) {
        r3 = (u32)lbl_80478AC0;
        f1 = *(f32*)lbl_80478AC0;
        break;
    }
    *(f32*)(sp + 0xC) = f1;
    tmp = 0x7F800000;
    r3 = r3 & 0x7F800000;
    if ((s32)r3 != (s32)tmp) {
        if ((s32)r3 >= (s32)tmp) goto L_8019E0B4;
        if ((s32)r3 != 0) {
            goto L_8019E0B4;
        }
        tmp = tmp & 0x7FFFFF;
        if ((s32)r3 != 0) {
            tmp = 0x1;
            break;
        }
        tmp = 0x2;
        break;
        }
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 != 0) {
        tmp = 0x5;
        break;
    }
    tmp = 0x3;
    break;
L_8019E0B4:
    tmp = 0x4;
} while (0);
    if ((s32)tmp != 1) break;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
} while (0);
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x44;
    fn_800A3AC0();
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = 0x20000000;
    r5 = 0x5;
    fn_801B00E0();
    tmp = r3;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r31 = tmp;
    r4 = 0x20000000;
    r5 = 0x6;
    fn_801B00E0();
    r29 = r3;
    if (r31 == 0) {
        if (r29 != 0) {
        }
        r3 = *(u32*)((u8*)r30 + 0x80);
        r28 = 0x0;
        r4 = 0x40000000;
        r5 = 0x0;
        fn_801B00E0();
        /* mr. r27, r3 */;
        if (r29 == 0) {
            r3 = (u32)lbl_8047DB20;
            r4 = 0x927;
            r5 = (u32)lbl_8047DB6C;
            fn_80196E10();
        }
        r5 = *(u32*)((u8*)r30 + 0xC);
        r3 = (u32)sp + 0x20;
        tmp = *(u32*)((u8*)r27 + 0x4);
        r4 = (u32)sp + 0x20;
        f0 = *(f32*)((u8*)r5 + 0x44);
        /* extrwi r27, tmp, 1, 29 */;
        *(f32*)(sp + 0x20) = f0;
        f0 = *(f32*)((u8*)r5 + 0x10);
        *(f32*)(sp + 0x24) = f0;
        f0 = *(f32*)((u8*)r5 + 0x20);
        *(f32*)(sp + 0x28) = f0;
        fn_800A3ADC();
        r3 = (u32)sp + 0x20;
        r4 = (u32)sp + 0x44;
        fn_800A3B7C();
        f0 = *(f32*)lbl_8047DB30;
        /* cror eq, gt, eq */;
        do {
            if (f1 == f0) {
                f0 = *(f32*)lbl_8047DB48;
                break;
            }
            f0 = *(f32*)lbl_8047DB74;
            /* cror eq, lt, eq */;
            if (f1 == f0) {
                f0 = *(f32*)lbl_8047DB78;
                break;
            }
            fn_800CE298();
            f0 = (f32)f1;
        } while (0);

        f1 = f0;
        if ((s32)r27 == 0) {
            f1 = -f1;
        }
        do {
            if (r31 == 0) break;
            f0 = *(f32*)((u8*)r31 + 0x8);
            if (f1 >= f0) break;
            f1 = *(f32*)((u8*)r31 + 0x8);
            r28 = 0x1;
            break;
        } while (0);

        do {
            if (r29 == 0) break;
            f0 = *(f32*)((u8*)r29 + 0x8);
            if (f0 >= f1) break;
            f1 = *(f32*)((u8*)r29 + 0x8);
            r28 = 0x1;
        } while (0);

        if ((s32)r28 != 0) {
            r5 = *(u32*)((u8*)r30 + 0xC);
            r3 = (u32)sp + 0x74;
            r4 = (u32)sp + 0x14;
            r5 = r5 + 0x44;
            f0 = *(f32*)((u8*)r5 + 0x8);
            *(f32*)(sp + 0x14) = f0;
            f0 = *(f32*)((u8*)r5 + 0x18);
            *(f32*)(sp + 0x18) = f0;
            f0 = *(f32*)((u8*)r5 + 0x28);
            *(f32*)(sp + 0x1C) = f0;
            fn_800A3244();
            r3 = (u32)sp + 0x74;
            r4 = (u32)sp + 0x20;
            r5 = (u32)sp + 0x44;
            fn_800A37CC();
        }
        }
do {
    r6 = *(u32*)((u8*)r30 + 0xC);
do {
    r3 = (u32)sp + 0x2c;
    r4 = (u32)sp + 0x44;
    r5 = (u32)sp + 0x38;
    r6 = r6 + 0x44;
    f0 = *(f32*)((u8*)r6 + 0x8);
    *(f32*)(sp + 0x2C) = f0;
    f0 = *(f32*)((u8*)r6 + 0x18);
    *(f32*)(sp + 0x30) = f0;
    f0 = *(f32*)((u8*)r6 + 0x28);
    *(f32*)(sp + 0x34) = f0;
    fn_800A3B9C();
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0x38;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f8 = f2 / f1;
    if (f8 > f0) {
        /* frsqrte f7, f8 */;
        f6 = *(f64*)lbl_8047DB50;
        f3 = *(f64*)lbl_8047DB58;
        f4 = *(f64*)lbl_8047DB50;
        f5 = f7 * f7;
        f1 = *(f64*)lbl_8047DB58;
        f2 = *(f64*)lbl_8047DB50;
        f6 = f6 * f7;
        f0 = *(f64*)lbl_8047DB58;
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
        break;
    }
    f0 = *(f64*)lbl_8047DB60;
    if (f8 < f0) {
        r3 = (u32)lbl_80478AC0;
        f8 = *(f32*)lbl_80478AC0;
        break;
    }
    *(f32*)(sp + 0x8) = f8;
    tmp = 0x7F800000;
    r3 = r3 & 0x7F800000;
    if ((s32)r3 != (s32)tmp) {
        if ((s32)r3 >= (s32)tmp) goto L_8019E368;
        if ((s32)r3 != 0) {
            goto L_8019E368;
        }
        tmp = tmp & 0x7FFFFF;
        if ((s32)r3 != 0) {
            tmp = 0x1;
            break;
        }
        tmp = 0x2;
        break;
        }
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 != 0) {
        tmp = 0x5;
        break;
    }
    tmp = 0x3;
    break;
L_8019E368:
    tmp = 0x4;
} while (0);
    if ((s32)tmp != 1) break;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
} while (0);
    f1 = f8;
    r3 = (u32)sp + 0x38;
    r4 = (u32)sp + 0x38;
    fn_800A3AC0();
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x38;
    r5 = (u32)sp + 0x2c;
    fn_800A3B9C();
    f2 = *(f32*)(sp + 0x44);
    f1 = *(f32*)(sp + 0x68);
    f0 = *(f32*)(sp + 0x68);
    f1 = f2 * f1;
    f6 = *(f32*)(sp + 0x68);
    f5 = *(f32*)(sp + 0x6C);
    f4 = *(f32*)(sp + 0x6C);
    *(f32*)((u8*)r30 + 0x44) = f1;
    f3 = *(f32*)(sp + 0x6C);
    f1 = *(f32*)(sp + 0x48);
    f2 = *(f32*)(sp + 0x70);
    f7 = f1 * f0;
    f1 = *(f32*)(sp + 0x70);
    f0 = *(f32*)(sp + 0x70);
    *(f32*)((u8*)r30 + 0x54) = f7;
    f7 = *(f32*)(sp + 0x4C);
    f6 = f7 * f6;
    *(f32*)((u8*)r30 + 0x64) = f6;
    f6 = *(f32*)(sp + 0x38);
    f5 = f6 * f5;
    *(f32*)((u8*)r30 + 0x48) = f5;
    f5 = *(f32*)(sp + 0x3C);
    f4 = f5 * f4;
    *(f32*)((u8*)r30 + 0x58) = f4;
    f4 = *(f32*)(sp + 0x40);
    f3 = f4 * f3;
    *(f32*)((u8*)r30 + 0x68) = f3;
    f3 = *(f32*)(sp + 0x2C);
    f2 = f3 * f2;
    *(f32*)((u8*)r30 + 0x4C) = f2;
    f2 = *(f32*)(sp + 0x30);
    f1 = f2 * f1;
    *(f32*)((u8*)r30 + 0x5C) = f1;
    f1 = *(f32*)(sp + 0x34);
    f0 = f1 * f0;
    *(f32*)((u8*)r30 + 0x6C) = f0;
    f0 = *(f32*)(sp + 0x5C);
    *(f32*)((u8*)r30 + 0x50) = f0;
    f0 = *(f32*)(sp + 0x60);
    *(f32*)((u8*)r30 + 0x60) = f0;
    f0 = *(f32*)(sp + 0x64);
    *(f32*)((u8*)r30 + 0x70) = f0;

    return;
}

/* 0x8019E460 | 0xBBC */
void fn_8019E460(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB30[];
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern u8 lbl_8047DB44[];
    extern u8 lbl_8047DB48[];
    extern u8 lbl_8047DB50[];
    extern u8 lbl_8047DB58[];
    extern u8 lbl_8047DB60[];
    extern u8 lbl_8047DB68[];
    extern u8 lbl_8047DB6C[];
    extern u8 lbl_8047DB7C[];
    extern u8 lbl_8047DB80[];
    extern u8 lbl_8047DB84[];
    extern void fn_800A3244();
    extern void fn_800A37CC();
    extern void fn_800A3A78();
    extern void fn_800A3A9C();
    extern void fn_800A3AC0();
    extern void fn_800A3B7C();
    extern void fn_800A3B9C();
    extern void fn_80196E10();
    extern void fn_801A9570();
    extern void fn_801AED88();
    extern void fn_801AFCAC();
    extern void fn_801B00E0();
    u8 sp[0x140];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r4 = (u32)lbl_80274AA0;
    r29 = r3;
    r7 = (u32)lbl_80274AA0;
    r5 = *(u32*)((u8*)r3 + 0x10);
    r4 = *(u32*)((u8*)r7 + 0x0);
    r31 = 0x0;
    r3 = *(u32*)((u8*)r7 + 0x4);
    tmp = *(u32*)((u8*)r7 + 0x8);
    f30 = *(f32*)lbl_8047DB48;
    *(u32*)(sp + 0x90) = tmp;
    while (1) {
        if (r5 == 0) break;
        tmp = *(u32*)((u8*)r5 + 0x14);
        r3 = tmp & 0x00600000;
        /* subis tmp, r3, 0x40 */;
        if (tmp == 0) {
            break;
        }
        r5 = *(u32*)((u8*)r5 + 0x8);

    }
    r5 = 0x0;

    r3 = *(u32*)((u8*)r7 + 0xC);
    r30 = r5;
    r6 = *(u32*)((u8*)r7 + 0x10);
    tmp = *(u32*)((u8*)r7 + 0x14);
    r5 = *(u32*)((u8*)r7 + 0x18);
    r4 = *(u32*)((u8*)r7 + 0x1C);
    r3 = *(u32*)((u8*)r7 + 0x20);
    *(u32*)(sp + 0x84) = tmp;
    tmp = *(u32*)((u8*)r29 + 0x74);
    if (tmp != 0) {
        r5 = *(u32*)((u8*)r29 + 0x74);
        r4 = *(u32*)((u8*)r5 + 0x0);
        r3 = *(u32*)((u8*)r5 + 0x4);
        tmp = *(u32*)((u8*)r5 + 0x8);
        *(u32*)(sp + 0x90) = tmp;
    }
    r3 = *(u32*)((u8*)r29 + 0x80);
    r4 = 0x40000000;
    r5 = 0x0;
    fn_801B00E0();
    /* mr. r28, r3 */;
    if (tmp == 0) {
        r3 = (u32)lbl_8047DB20;
        r4 = 0x85c;
        r5 = (u32)lbl_8047DB6C;
        fn_80196E10();
    }
    f0 = *(f32*)(sp + 0x88);
    f1 = *(f32*)((u8*)r28 + 0x8);
    f26 = *(f32*)((u8*)r28 + 0xC);
    f31 = f1 * f0;
    if (r30 != 0) {
        r3 = *(u32*)((u8*)r30 + 0x80);
        r4 = 0x40000000;
        r5 = 0x0;
        fn_801B00E0();
        /* mr. r28, r3 */;
        if (r30 == 0) {
            r3 = (u32)lbl_8047DB20;
            r4 = 0x867;
            r5 = (u32)lbl_8047DB6C;
            fn_80196E10();
        }
        f0 = *(f32*)((u8*)r30 + 0x2C);
        f1 = *(f32*)((u8*)r28 + 0x8);
        tmp = *(u32*)((u8*)r28 + 0x4);
        f0 = f1 * f0;
        f1 = *(f32*)(sp + 0x88);
        r30 = *(u32*)((u8*)r30 + 0x10);
        /* extrwi r31, tmp, 1, 29 */;
        f30 = f1 * f0;
        while (1) {
            if (r30 == 0) break;
            tmp = *(u32*)((u8*)r30 + 0x14);
            r3 = tmp & 0x00600000;
            /* subis tmp, r3, 0x60 */;
            if (tmp == 0) {
                break;
            }
            r30 = *(u32*)((u8*)r30 + 0x8);

        }
        r30 = 0x0;

        if (r30 == 0) {
            r3 = (u32)lbl_8047DB20;
            r4 = 0x82d;
            r5 = (u32)lbl_8047DB68;
            fn_80196E10();
        }
        r3 = *(u32*)((u8*)r30 + 0x80);
        r4 = 0x10000000;
        r5 = 0x1;
        fn_801B00E0();
        if (r3 != 0) {
            goto L_8019E6BC;
        }
        r30 = 0x0;

    } else {
        r30 = *(u32*)((u8*)r29 + 0x10);
        while (1) {
            if (r30 == 0) break;
            tmp = *(u32*)((u8*)r30 + 0x14);
            r3 = tmp & 0x00600000;
            /* subis tmp, r3, 0x60 */;
            if (tmp == 0) {
                break;
            }
            r30 = *(u32*)((u8*)r30 + 0x8);

        }
        r30 = 0x0;

        if (r30 == 0) {
            r3 = (u32)lbl_8047DB20;
            r4 = 0x82d;
            r5 = (u32)lbl_8047DB68;
            fn_80196E10();
        }
        r3 = *(u32*)((u8*)r30 + 0x80);
        r4 = 0x10000000;
        r5 = 0x1;
        fn_801B00E0();
        if (r3 != 0) {

        } else {
            r30 = 0x0;
        }
    }
L_8019E6BC:
do {
    if (r30 == 0) return;
    r3 = *(u32*)((u8*)r29 + 0x80);
    r4 = 0x10000000;
    r5 = 0x3;
    fn_801B00E0();
    if (r3 == 0) {
        if (r29 != 0) {
            tmp = *(u32*)((u8*)r29 + 0x80);
            if (tmp != 0) {
                r5 = *(u32*)((u8*)r29 + 0x0);
                r4 = r29;
                r3 = *(u32*)((u8*)r29 + 0x80);
                r5 = *(u32*)((u8*)r5 + 0x50);
                fn_801AED88();
                if (r29 == 0) {
                    r3 = (u32)lbl_8047DB34;
                    r4 = 0x25d;
                    r5 = (u32)lbl_8047DB3C;
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
                    r4 = *(u32*)((u8*)r29 + 0x0);
                    r3 = r29;
                    r12 = *(u32*)((u8*)r4 + 0x40);
                    ctr_fn = (void(*)(void))r12;
                    ctr_fn();
                    tmp = *(u32*)((u8*)r29 + 0x14);
                    tmp = tmp & 0xFFFFFFBF;
                    *(u32*)((u8*)r29 + 0x14) = tmp;
    }
    }
    }
    }
    tmp = *(u32*)((u8*)r29 + 0xC);
    if (tmp != 0) {
        r3 = *(u32*)((u8*)r29 + 0xC);
        r4 = (u32)sp + 0x7c;
        r3 = r3 + 0x44;
        fn_801A9570();
    }
    r3 = *(u32*)((u8*)r30 + 0x80);
    r5 = r30 + 0x38;
    r4 = 0x1;
    fn_801AFCAC();
    r3 = r30 + 0x38;
    r4 = (u32)sp + 0x7c;
    r5 = (u32)sp + 0x58;
    fn_800A3A9C();
    r3 = (u32)sp + 0x58;
    r4 = (u32)sp + 0x58;
    fn_800A3B7C();
    f29 = f1;
    f0 = *(f32*)lbl_8047DB7C;
    if (f29 > f0) {
    do {
        r5 = (u32)sp + 0x28;
    do {
        r4 = 0x3;
        *(u32*)(sp + 0x3C) = tmp;
        r3 = *(u32*)((u8*)r29 + 0x80);
        fn_801AFCAC();
        if ((s32)r3 != 0) {
            r3 = (u32)sp + 0x28;
            r4 = (u32)sp + 0x7c;
            r5 = (u32)sp + 0x28;
            fn_800A3A9C();
            f0 = *(f32*)lbl_8047DB48;
            if (f0 != f26) {
                f1 = f26;
                r3 = (u32)sp + 0x94;
                r4 = (u32)sp + 0x34;
                fn_800A3244();
                r3 = (u32)sp + 0x94;
                r4 = (u32)sp + 0x28;
                r5 = (u32)sp + 0x28;
                fn_800A37CC();
            }
            r3 = (u32)sp + 0x34;
            r4 = (u32)sp + 0x28;
            r5 = (u32)sp + 0x1c;
            fn_800A3B9C();
            r3 = (u32)sp + 0x1c;
            r4 = (u32)sp + 0x34;
            r5 = (u32)sp + 0x28;
            fn_800A3B9C();
        } else {

            f0 = *(f32*)((u8*)r29 + 0x4C);
            r3 = (u32)sp + 0x1c;
            r4 = (u32)sp + 0x34;
            r5 = (u32)sp + 0x28;
            *(f32*)(sp + 0x1C) = f0;
            f0 = *(f32*)((u8*)r29 + 0x5C);
            *(f32*)(sp + 0x20) = f0;
            f0 = *(f32*)((u8*)r29 + 0x6C);
            *(f32*)(sp + 0x24) = f0;
            fn_800A3B9C();
            r3 = (u32)sp + 0x34;
            r4 = (u32)sp + 0x28;
            r5 = (u32)sp + 0x1c;
            fn_800A3B9C();
        }
        r3 = (u32)sp + 0x1c;
        r4 = (u32)sp + 0x1c;
        fn_800A3B7C();
        f3 = *(f32*)lbl_8047DB44;
        f2 = *(f32*)lbl_8047DB30;
        f1 = f3 + f1;
        f0 = *(f32*)lbl_8047DB48;
        f8 = f2 / f1;
        if (f8 > f0) {
            /* frsqrte f7, f8 */;
            f6 = *(f64*)lbl_8047DB50;
            f3 = *(f64*)lbl_8047DB58;
            f4 = *(f64*)lbl_8047DB50;
            f5 = f7 * f7;
            f1 = *(f64*)lbl_8047DB58;
            f2 = *(f64*)lbl_8047DB50;
            f6 = f6 * f7;
            f0 = *(f64*)lbl_8047DB58;
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
            break;
        }
        f0 = *(f64*)lbl_8047DB60;
        if (f8 < f0) {
            r3 = (u32)lbl_80478AC0;
            f8 = *(f32*)lbl_80478AC0;
            break;
        }
        *(f32*)(sp + 0x18) = f8;
        tmp = 0x7F800000;
        r3 = r3 & 0x7F800000;
        if ((s32)r3 != (s32)tmp) {
            if ((s32)r3 >= (s32)tmp) goto L_8019E97C;
            if ((s32)r3 != 0) {
                goto L_8019E97C;
            }
            tmp = tmp & 0x7FFFFF;
            if ((s32)r3 != 0) {
                tmp = 0x1;
                break;
            }
            tmp = 0x2;
            break;
            }
        tmp = tmp & 0x7FFFFF;
        if ((s32)r3 != 0) {
            tmp = 0x5;
            break;
        }
        tmp = 0x3;
        break;
    L_8019E97C:
        tmp = 0x4;
    } while (0);
        if ((s32)tmp != 1) break;
        r3 = (u32)lbl_80478AC0;
        f8 = *(f32*)lbl_80478AC0;
    } while (0);
    do {
        f1 = f8;
    do {
        r3 = (u32)sp + 0x1c;
        r4 = (u32)sp + 0x4c;
        fn_800A3AC0();
        r3 = (u32)sp + 0x28;
        r4 = (u32)sp + 0x28;
        fn_800A3B7C();
        f3 = *(f32*)lbl_8047DB44;
        f2 = *(f32*)lbl_8047DB30;
        f1 = f3 + f1;
        f0 = *(f32*)lbl_8047DB48;
        f8 = f2 / f1;
        if (f8 > f0) {
            /* frsqrte f7, f8 */;
            f6 = *(f64*)lbl_8047DB50;
            f3 = *(f64*)lbl_8047DB58;
            f4 = *(f64*)lbl_8047DB50;
            f5 = f7 * f7;
            f1 = *(f64*)lbl_8047DB58;
            f2 = *(f64*)lbl_8047DB50;
            f6 = f6 * f7;
            f0 = *(f64*)lbl_8047DB58;
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
            break;
        }
        f0 = *(f64*)lbl_8047DB60;
        if (f8 < f0) {
            r3 = (u32)lbl_80478AC0;
            f8 = *(f32*)lbl_80478AC0;
            break;
        }
        *(f32*)(sp + 0x14) = f8;
        tmp = 0x7F800000;
        r3 = r3 & 0x7F800000;
        if ((s32)r3 != (s32)tmp) {
            if ((s32)r3 >= (s32)tmp) goto L_8019EA98;
            if ((s32)r3 != 0) {
                goto L_8019EA98;
            }
            tmp = tmp & 0x7FFFFF;
            if ((s32)r3 != 0) {
                tmp = 0x1;
                break;
            }
            tmp = 0x2;
            break;
            }
        tmp = tmp & 0x7FFFFF;
        if ((s32)r3 != 0) {
            tmp = 0x5;
            break;
        }
        tmp = 0x3;
        break;
    L_8019EA98:
        tmp = 0x4;
    } while (0);
        if ((s32)tmp != 1) break;
        r3 = (u32)lbl_80478AC0;
        f8 = *(f32*)lbl_80478AC0;
    } while (0);
    do {
        f1 = f8;
    do {
        r3 = (u32)sp + 0x28;
        r4 = (u32)sp + 0x40;
        fn_800A3AC0();
        f6 = f31 * f31;
        f4 = *(f32*)lbl_8047DB84;
        f28 = f30 * f30;
        f5 = *(f32*)lbl_8047DB80;
        f0 = *(f32*)lbl_8047DB48;
        f2 = f6 - f28;
        f1 = f6 - f28;
        f3 = f6 + f28;
        f1 = f2 * f1;
        f2 = f4 * f3 - f29;
        f1 = f1 / f29;
        f1 = f2 - f1;
        f27 = f5 * f1;
        if (f27 < f0) {
            f27 = *(f32*)lbl_8047DB48;
        }
        f0 = f6 - f27;
        f2 = *(f32*)lbl_8047DB44;
        f3 = *(f32*)lbl_8047DB30;
        f1 = *(f32*)lbl_8047DB48;
        f0 = f0 / f29;
        f2 = f2 + f0;
        f2 = f3 / f2;
        if (f2 > f1) {
            f1 = *(f32*)lbl_8047DB44;
            f2 = *(f32*)lbl_8047DB30;
            f1 = f1 + f0;
            f7 = *(f64*)lbl_8047DB50;
            f4 = *(f64*)lbl_8047DB58;
            f5 = *(f64*)lbl_8047DB50;
            f9 = f2 / f1;
            f2 = *(f64*)lbl_8047DB58;
            f3 = *(f64*)lbl_8047DB50;
            f1 = *(f64*)lbl_8047DB58;
            /* frsqrte f8, f9 */;
            f6 = f8 * f8;
            f7 = f7 * f8;
            f4 = -(f9 * f6 - f4);
            f8 = f7 * f4;
            f4 = f8 * f8;
            f5 = f5 * f8;
            f2 = -(f9 * f4 - f2);
            f8 = f5 * f2;
            f2 = f8 * f8;
            f3 = f3 * f8;
            f1 = -(f9 * f2 - f1);
            f8 = f3 * f1;
            f1 = f9 * f8;
            f1 = (f32)f1;
            break;
        }
        f1 = *(f32*)lbl_8047DB44;
        f3 = *(f32*)lbl_8047DB30;
        f2 = f1 + f0;
        f1 = *(f64*)lbl_8047DB60;
        f2 = f3 / f2;
        if (f2 < f1) {
            r3 = (u32)lbl_80478AC0;
            f1 = *(f32*)lbl_80478AC0;
            break;
        }
        f1 = *(f32*)lbl_8047DB44;
        tmp = 0x7F800000;
        f2 = *(f32*)lbl_8047DB30;
        f1 = f1 + f0;
        f1 = f2 / f1;
        *(f32*)(sp + 0x10) = f1;
        r3 = r3 & 0x7F800000;
        if ((s32)r3 != (s32)tmp) {
            if ((s32)r3 >= (s32)tmp) goto L_8019EC20;
            if ((s32)r3 != 0) {
                goto L_8019EC20;
            }
            tmp = tmp & 0x7FFFFF;
            if ((s32)r3 != 0) {
                tmp = 0x1;
                break;
            }
            tmp = 0x2;
            break;
            }
        tmp = tmp & 0x7FFFFF;
        if ((s32)r3 != 0) {
            tmp = 0x5;
            break;
        }
        tmp = 0x3;
        break;
    L_8019EC20:
        tmp = 0x4;
    } while (0);
        if ((s32)tmp == 1) {
            r3 = (u32)lbl_80478AC0;
            f1 = *(f32*)lbl_80478AC0;
            break;
        }
        f1 = *(f32*)lbl_8047DB44;
        f2 = *(f32*)lbl_8047DB30;
        f1 = f1 + f0;
        f1 = f2 / f1;
    } while (0);
    do {
        f2 = *(f32*)lbl_8047DB44;
    do {
        f1 = f0 * f1;
        f3 = *(f32*)lbl_8047DB30;
        f2 = f2 + f27;
        f0 = *(f32*)lbl_8047DB48;
        f2 = f3 / f2;
        if (f2 > f0) {
            f0 = *(f32*)lbl_8047DB44;
            f2 = *(f32*)lbl_8047DB30;
            f0 = f0 + f27;
            f7 = *(f64*)lbl_8047DB50;
            f4 = *(f64*)lbl_8047DB58;
            f5 = *(f64*)lbl_8047DB50;
            f9 = f2 / f0;
            f2 = *(f64*)lbl_8047DB58;
            f3 = *(f64*)lbl_8047DB50;
            f0 = *(f64*)lbl_8047DB58;
            /* frsqrte f8, f9 */;
            f6 = f8 * f8;
            f7 = f7 * f8;
            f4 = -(f9 * f6 - f4);
            f8 = f7 * f4;
            f4 = f8 * f8;
            f5 = f5 * f8;
            f2 = -(f9 * f4 - f2);
            f8 = f5 * f2;
            f2 = f8 * f8;
            f3 = f3 * f8;
            f0 = -(f9 * f2 - f0);
            f8 = f3 * f0;
            f0 = f9 * f8;
            f0 = (f32)f0;
            break;
        }
        f0 = *(f32*)lbl_8047DB44;
        f3 = *(f32*)lbl_8047DB30;
        f2 = f0 + f27;
        f0 = *(f64*)lbl_8047DB60;
        f2 = f3 / f2;
        if (f2 < f0) {
            r3 = (u32)lbl_80478AC0;
            f0 = *(f32*)lbl_80478AC0;
            break;
        }
        f0 = *(f32*)lbl_8047DB44;
        tmp = 0x7F800000;
        f2 = *(f32*)lbl_8047DB30;
        f0 = f0 + f27;
        f0 = f2 / f0;
        *(f32*)(sp + 0xC) = f0;
        r3 = r3 & 0x7F800000;
        if ((s32)r3 != (s32)tmp) {
            if ((s32)r3 >= (s32)tmp) goto L_8019ED68;
            if ((s32)r3 != 0) {
                goto L_8019ED68;
            }
            tmp = tmp & 0x7FFFFF;
            if ((s32)r3 != 0) {
                tmp = 0x1;
                break;
            }
            tmp = 0x2;
            break;
            }
        tmp = tmp & 0x7FFFFF;
        if ((s32)r3 != 0) {
            tmp = 0x5;
            break;
        }
        tmp = 0x3;
        break;
    L_8019ED68:
        tmp = 0x4;
    } while (0);
        if ((s32)tmp == 1) {
            r3 = (u32)lbl_80478AC0;
            f0 = *(f32*)lbl_80478AC0;
            break;
        }
        f0 = *(f32*)lbl_8047DB44;
        f2 = *(f32*)lbl_8047DB30;
        f0 = f0 + f27;
        f0 = f2 / f0;
    } while (0);
        f30 = f27 * f0;

    } else {
        f1 = *(f32*)lbl_8047DB48;
        f30 = f31;
    }
    if ((s32)r31 != 0) {
        f30 = -f30;
    }
    f0 = f28 - f27;
    if (f0 < f29) {
        r3 = (u32)sp + 0x58;
        r4 = (u32)sp + 0x64;
        fn_800A3AC0();
    } else {

        f1 = -f1;
        r3 = (u32)sp + 0x58;
        r4 = (u32)sp + 0x64;
        fn_800A3AC0();
    }
    f1 = f30;
    r3 = (u32)sp + 0x40;
    r4 = (u32)sp + 0x28;
    fn_800A3AC0();
    r3 = (u32)sp + 0x64;
    r4 = (u32)sp + 0x28;
    r5 = (u32)sp + 0x64;
    fn_800A3A78();
    r3 = (u32)sp + 0x64;
    r4 = (u32)sp + 0x64;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f8 = f2 / f1;
    if (f8 > f0) {
        /* frsqrte f7, f8 */;
        f6 = *(f64*)lbl_8047DB50;
        f3 = *(f64*)lbl_8047DB58;
        f4 = *(f64*)lbl_8047DB50;
        f5 = f7 * f7;
        f1 = *(f64*)lbl_8047DB58;
        f2 = *(f64*)lbl_8047DB50;
        f6 = f6 * f7;
        f0 = *(f64*)lbl_8047DB58;
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
        break;
    }
    f0 = *(f64*)lbl_8047DB60;
    if (f8 < f0) {
        r3 = (u32)lbl_80478AC0;
        f8 = *(f32*)lbl_80478AC0;
        break;
    }
    *(f32*)(sp + 0x8) = f8;
    tmp = 0x7F800000;
    r3 = r3 & 0x7F800000;
    if ((s32)r3 != (s32)tmp) {
        if ((s32)r3 >= (s32)tmp) goto L_8019EEF0;
        if ((s32)r3 != 0) {
            goto L_8019EEF0;
        }
        tmp = tmp & 0x7FFFFF;
        if ((s32)r3 != 0) {
            tmp = 0x1;
            goto L_8019EEF4;
        }
        tmp = 0x2;
        goto L_8019EEF4;
        }
    tmp = tmp & 0x7FFFFF;
    if ((s32)r3 != 0) {
        tmp = 0x5;
        goto L_8019EEF4;
    }
    tmp = 0x3;
    goto L_8019EEF4;
L_8019EEF0:
    tmp = 0x4;
L_8019EEF4:
    if ((s32)tmp != 1) break;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
} while (0);
    f1 = f8;
    r3 = (u32)sp + 0x64;
    r4 = (u32)sp + 0x64;
    fn_800A3AC0();
    f2 = *(f32*)(sp + 0x64);
    r3 = (u32)sp + 0x4c;
    f0 = *(f32*)(sp + 0x88);
    r4 = (u32)sp + 0x64;
    f1 = *(f32*)(sp + 0x88);
    r5 = (u32)sp + 0x28;
    f2 = f2 * f0;
    f0 = *(f32*)(sp + 0x88);
    *(f32*)((u8*)r29 + 0x44) = f2;
    f2 = *(f32*)(sp + 0x68);
    f1 = f2 * f1;
    *(f32*)((u8*)r29 + 0x54) = f1;
    f1 = *(f32*)(sp + 0x6C);
    f0 = f1 * f0;
    *(f32*)((u8*)r29 + 0x64) = f0;
    fn_800A3B9C();
    f1 = *(f32*)(sp + 0x28);
    f0 = *(f32*)(sp + 0x8C);
    f4 = *(f32*)(sp + 0x8C);
    f0 = f1 * f0;
    f3 = *(f32*)(sp + 0x8C);
    f2 = *(f32*)(sp + 0x90);
    f1 = *(f32*)(sp + 0x90);
    *(f32*)((u8*)r29 + 0x48) = f0;
    f0 = *(f32*)(sp + 0x90);
    f5 = *(f32*)(sp + 0x2C);
    f4 = f5 * f4;
    *(f32*)((u8*)r29 + 0x58) = f4;
    f4 = *(f32*)(sp + 0x30);
    f3 = f4 * f3;
    *(f32*)((u8*)r29 + 0x68) = f3;
    f3 = *(f32*)(sp + 0x4C);
    f2 = f3 * f2;
    *(f32*)((u8*)r29 + 0x4C) = f2;
    f2 = *(f32*)(sp + 0x50);
    f1 = f2 * f1;
    *(f32*)((u8*)r29 + 0x5C) = f1;
    f1 = *(f32*)(sp + 0x54);
    f0 = f1 * f0;
    *(f32*)((u8*)r29 + 0x6C) = f0;
    f0 = *(f32*)(sp + 0x7C);
    *(f32*)((u8*)r29 + 0x50) = f0;
    f0 = *(f32*)(sp + 0x80);
    *(f32*)((u8*)r29 + 0x60) = f0;
    f0 = *(f32*)(sp + 0x84);
    *(f32*)((u8*)r29 + 0x70) = f0;

    return;
}

/* 0x8019F024 | 0x1A0 */
void fn_8019F024(void) {
    extern u8 lbl_80274AA0[];
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

    /* mr. r29, r3 */;
    r3 = (u32)lbl_80274AA0;
    r31 = (u32)lbl_80274AA0;
    if ((s32)tmp != 0) {
        r3 = *(u16*)((u8*)r29 + 0x4);
        tmp = r3 + 0x1;
        *(u16*)((u8*)r29 + 0x4) = tmp;
        tmp = *(u16*)((u8*)r29 + 0x4);
        if (tmp == 0xffff) {
            r3 = r31 + 0x54;
            r5 = r31 + 0xc4;
            r4 = 0x5d;
            fn_80196E10();
    }
    }
    r30 = *(u32*)&lbl_8047B2AC;
do {
    if (r30 == 0) break;
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
    if ((s32)r3 == 0) break;
    r3 = *(u16*)((u8*)r30 + 0x6);
    /* subic. tmp, r3, 0x1 */;
    if ((s32)r3 < 0) {
        if (r30 == 0) break;
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
        break;
    }
    r3 = *(u16*)((u8*)r30 + 0x6);
    tmp = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x6) = tmp;
    tmp = *(u16*)((u8*)r30 + 0x6);
    if (tmp == 0) {
        r3 = r31 + 0x54;
        r5 = r31 + 0x60;
        r4 = 0x9e;
        fn_80196E10();
    }
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = *(u16*)((u8*)r30 + 0x6);
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp != 0) {
    } else {

        r3 = *(u16*)((u8*)r30 + 0x6);
        *(u16*)((u8*)r30 + 0x6) = tmp;
        tmp = *(u16*)((u8*)r30 + 0x6);
        tmp = __cntlzw(tmp);
        tmp = (u32)tmp >> 5;
    }
    if ((s32)tmp == 0 || r30 == 0) break;

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
} while (0);
    *(u32*)&lbl_8047B2AC = r29;
    return;
}

/* 0x8019F1C4 | 0x554 */
void fn_8019F1C4(void) {
    extern void fn_80199178();
    extern void fn_8019F1C4();
    u8 sp[0x90];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    tmp = 0x0;
    /* mr. r27, r3 */;
    r3 = 0x0;
    r28 = r4;
    r29 = r5;
    *(u32*)(sp + 0x70) = tmp;
    if ((s32)tmp != 0) {
        tmp = *(u32*)((u8*)r27 + 0x14);
        tmp = tmp & 0x00001000;
        if ((s32)tmp != 0) {
            r30 = *(u32*)((u8*)r27 + 0x10);
            r3 = 0x0;
            tmp = 0x0;
            *(u32*)(sp + 0x5C) = tmp;
            if (r30 != 0) {
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00001000;
                if (r30 != 0) {
                    r30 = *(u32*)((u8*)r30 + 0x10);
                    r3 = 0x0;
                    tmp = 0x0;
                    *(u32*)(sp + 0x3C) = tmp;
                    if (r30 != 0) {
                        tmp = *(u32*)((u8*)r30 + 0x14);
                        tmp = tmp & 0x00001000;
                        if (r30 != 0) {
                            r3 = *(u32*)((u8*)r30 + 0x10);
                            r4 = (u32)sp + 0x38;
                            r5 = (u32)sp + 0x3c;
                            fn_8019F1C4();
                            goto L_8019F2D8;
                        }
                        tmp = *(u32*)((u8*)r30 + 0x14);
                        tmp = tmp & 0x00000010;
                        if (r30 == 0) {
                            tmp = *(u32*)((u8*)r30 + 0x14);
                            tmp = tmp & 0x4020;
                            tmp = __cntlzw(tmp);
                            /* srwi. tmp, tmp, 5 */;
                            if (r30 != 0) {
                                r3 = *(u32*)((u8*)r30 + 0x18);
                                r4 = (u32)sp + 0x38;
                                r5 = (u32)sp + 0x3c;
                                fn_80199178();
                        }
                        }
                        r30 = *(u32*)((u8*)r30 + 0x10);
                        while (r30 != 0) {

                            r3 = r30;
                            r4 = (u32)sp + 0x40;
                            r5 = (u32)sp + 0x44;
                            fn_8019F1C4();
                            r4 = r5 + r4;
                            tmp = r3 + tmp;
                            *(u32*)(sp + 0x3C) = tmp;
                            r30 = *(u32*)((u8*)r30 + 0x8);

                        }
                    }
                L_8019F2D8:
                    /* addic. tmp, (u32)sp, 0x58 */;
                    if (r30 != 0) {
                        *(u32*)(sp + 0x58) = tmp;
                    }
                    *(u32*)(sp + 0x5C) = tmp;
                    goto L_8019F424;
                }
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00000010;
                if (r30 == 0) {
                    tmp = *(u32*)((u8*)r30 + 0x14);
                    tmp = tmp & 0x4020;
                    tmp = __cntlzw(tmp);
                    /* srwi. tmp, tmp, 5 */;
                    if (r30 != 0) {
                        r3 = *(u32*)((u8*)r30 + 0x18);
                        r4 = (u32)sp + 0x58;
                        r5 = (u32)sp + 0x5c;
                        fn_80199178();
                }
                }
                r31 = *(u32*)((u8*)r30 + 0x10);
                while (1) {
                    if (r31 == 0) break;
                    r3 = 0x0;
                    tmp = 0x0;
                    *(u32*)(sp + 0x2C) = tmp;
                    if (r31 != 0) {
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        tmp = tmp & 0x00001000;
                        if (r31 != 0) {
                            r3 = *(u32*)((u8*)r31 + 0x10);
                            r4 = (u32)sp + 0x28;
                            r5 = (u32)sp + 0x2c;
                            fn_8019F1C4();
                            goto L_8019F3D8;
                        }
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        tmp = tmp & 0x00000010;
                        if (r31 == 0) {
                            tmp = *(u32*)((u8*)r31 + 0x14);
                            tmp = tmp & 0x4020;
                            tmp = __cntlzw(tmp);
                            /* srwi. tmp, tmp, 5 */;
                            if (r31 != 0) {
                                r3 = *(u32*)((u8*)r31 + 0x18);
                                r4 = (u32)sp + 0x28;
                                r5 = (u32)sp + 0x2c;
                                fn_80199178();
                        }
                        }
                        r30 = *(u32*)((u8*)r31 + 0x10);
                        while (r30 != 0) {

                            r3 = r30;
                            r4 = (u32)sp + 0x30;
                            r5 = (u32)sp + 0x34;
                            fn_8019F1C4();
                            r4 = r5 + r4;
                            tmp = r3 + tmp;
                            *(u32*)(sp + 0x2C) = tmp;
                            r30 = *(u32*)((u8*)r30 + 0x8);

                        }
                    }
                L_8019F3D8:
                    /* addic. tmp, (u32)sp, 0x60 */;
                    if (r30 != 0) {
                        *(u32*)(sp + 0x60) = tmp;
                    }
                    /* addic. tmp, (u32)sp, 0x64 */;
                    if (r30 != 0) {
                        *(u32*)(sp + 0x64) = tmp;
                    }
                    r4 = r5 + r4;
                    tmp = r3 + tmp;
                    *(u32*)(sp + 0x5C) = tmp;
                    r31 = *(u32*)((u8*)r31 + 0x8);

                }
            }
        L_8019F424:
            /* addic. tmp, (u32)sp, 0x74 */;
            if (r31 != 0) {
                *(u32*)(sp + 0x74) = tmp;
            }
            *(u32*)(sp + 0x70) = tmp;
            goto L_8019F6E4;
        }
        tmp = *(u32*)((u8*)r27 + 0x14);
        tmp = tmp & 0x00000010;
        if (r31 == 0) {
            tmp = *(u32*)((u8*)r27 + 0x14);
            tmp = tmp & 0x4020;
            tmp = __cntlzw(tmp);
            /* srwi. tmp, tmp, 5 */;
            if (r31 != 0) {
                r3 = *(u32*)((u8*)r27 + 0x18);
                r4 = (u32)sp + 0x74;
                r5 = (u32)sp + 0x70;
                fn_80199178();
        }
        }
        r30 = *(u32*)((u8*)r27 + 0x10);
        while (1) {
            if (r30 == 0) break;
            r3 = 0x0;
            tmp = 0x0;
            *(u32*)(sp + 0x4C) = tmp;
            if (r30 != 0) {
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00001000;
                if (r30 != 0) {
                    r31 = *(u32*)((u8*)r30 + 0x10);
                    r3 = 0x0;
                    tmp = 0x0;
                    *(u32*)(sp + 0x1C) = tmp;
                    if (r31 != 0) {
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        tmp = tmp & 0x00001000;
                        if (r31 != 0) {
                            r3 = *(u32*)((u8*)r31 + 0x10);
                            r4 = (u32)sp + 0x18;
                            r5 = (u32)sp + 0x1c;
                            fn_8019F1C4();
                            goto L_8019F54C;
                        }
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        tmp = tmp & 0x00000010;
                        if (r31 == 0) {
                            tmp = *(u32*)((u8*)r31 + 0x14);
                            tmp = tmp & 0x4020;
                            tmp = __cntlzw(tmp);
                            /* srwi. tmp, tmp, 5 */;
                            if (r31 != 0) {
                                r3 = *(u32*)((u8*)r31 + 0x18);
                                r4 = (u32)sp + 0x18;
                                r5 = (u32)sp + 0x1c;
                                fn_80199178();
                        }
                        }
                        r31 = *(u32*)((u8*)r31 + 0x10);
                        while (r31 != 0) {

                            r3 = r31;
                            r4 = (u32)sp + 0x20;
                            r5 = (u32)sp + 0x24;
                            fn_8019F1C4();
                            r4 = r5 + r4;
                            tmp = r3 + tmp;
                            *(u32*)(sp + 0x1C) = tmp;
                            r31 = *(u32*)((u8*)r31 + 0x8);

                        }
                    }
                L_8019F54C:
                    /* addic. tmp, (u32)sp, 0x48 */;
                    if (r31 != 0) {
                        *(u32*)(sp + 0x48) = tmp;
                    }
                    *(u32*)(sp + 0x4C) = tmp;
                    goto L_8019F698;
                }
                tmp = *(u32*)((u8*)r30 + 0x14);
                tmp = tmp & 0x00000010;
                if (r31 == 0) {
                    tmp = *(u32*)((u8*)r30 + 0x14);
                    tmp = tmp & 0x4020;
                    tmp = __cntlzw(tmp);
                    /* srwi. tmp, tmp, 5 */;
                    if (r31 != 0) {
                        r3 = *(u32*)((u8*)r30 + 0x18);
                        r4 = (u32)sp + 0x48;
                        r5 = (u32)sp + 0x4c;
                        fn_80199178();
                }
                }
                r31 = *(u32*)((u8*)r30 + 0x10);
                while (1) {
                    if (r31 == 0) break;
                    r3 = 0x0;
                    tmp = 0x0;
                    *(u32*)(sp + 0xC) = tmp;
                    if (r31 != 0) {
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        tmp = tmp & 0x00001000;
                        if (r31 != 0) {
                            r3 = *(u32*)((u8*)r31 + 0x10);
                            r4 = (u32)sp + 0x8;
                            r5 = (u32)sp + 0xc;
                            fn_8019F1C4();
                            goto L_8019F64C;
                        }
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        tmp = tmp & 0x00000010;
                        if (r31 == 0) {
                            tmp = *(u32*)((u8*)r31 + 0x14);
                            tmp = tmp & 0x4020;
                            tmp = __cntlzw(tmp);
                            /* srwi. tmp, tmp, 5 */;
                            if (r31 != 0) {
                                r3 = *(u32*)((u8*)r31 + 0x18);
                                r4 = (u32)sp + 0x8;
                                r5 = (u32)sp + 0xc;
                                fn_80199178();
                        }
                        }
                        r27 = *(u32*)((u8*)r31 + 0x10);
                        while (r27 != 0) {

                            r3 = r27;
                            r4 = (u32)sp + 0x10;
                            r5 = (u32)sp + 0x14;
                            fn_8019F1C4();
                            r4 = r5 + r4;
                            tmp = r3 + tmp;
                            *(u32*)(sp + 0xC) = tmp;
                            r27 = *(u32*)((u8*)r27 + 0x8);

                        }
                    }
                L_8019F64C:
                    /* addic. tmp, (u32)sp, 0x50 */;
                    if (r27 != 0) {
                        *(u32*)(sp + 0x50) = tmp;
                    }
                    /* addic. tmp, (u32)sp, 0x54 */;
                    if (r27 != 0) {
                        *(u32*)(sp + 0x54) = tmp;
                    }
                    r4 = r5 + r4;
                    tmp = r3 + tmp;
                    *(u32*)(sp + 0x4C) = tmp;
                    r31 = *(u32*)((u8*)r31 + 0x8);

                }
            }
        L_8019F698:
            /* addic. tmp, (u32)sp, 0x6c */;
            if (r31 != 0) {
                *(u32*)(sp + 0x6C) = tmp;
            }
            /* addic. tmp, (u32)sp, 0x68 */;
            if (r31 != 0) {
                *(u32*)(sp + 0x68) = tmp;
            }
            r4 = r5 + r4;
            tmp = r3 + tmp;
            *(u32*)(sp + 0x70) = tmp;
            r30 = *(u32*)((u8*)r30 + 0x8);

        }
    }
L_8019F6E4:
    if (r28 != 0) {
        *(u32*)((u8*)r28 + 0x0) = tmp;
    }
    if (r29 != 0) {
        *(u32*)((u8*)r29 + 0x0) = tmp;
    }
    return;
}

/* 0x60 | fn_8019F718 | generic */
void fn_8019F718(void) {
    /* refs: lbl_8036C8E0, lbl_8047B298 */
    fn_80193828();
    fn_80196E10();
}

/* 0x78 | fn_8019F778 | generic */
u32 fn_8019F778(void) {
    fn_80196E10();
    fn_8019D620();
    return 1;
}

/* 0x8019F7F0 | 0x2FC */
void fn_8019F7F0(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
    extern void fn_8019D980();
    extern void fn_8019F778();
    extern void fn_8019F7F0();
    extern void fn_8019FAEC();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r25, r3 */;
    r28 = r4;
    if ((s32)tmp == 0) return;
    if (r25 != 0) {
        tmp = *(u32*)((u8*)r25 + 0x14);
        tmp = tmp ^ r28;
        tmp = tmp & 0x00000008;
        if (r25 != 0) {
            if (r25 != 0) {
                if (r25 == 0) {
                    r3 = (u32)lbl_8047DB34;
                    r4 = 0x25d;
                    r5 = (u32)lbl_8047DB3C;
                    fn_80196E10();
                }
                tmp = *(u32*)((u8*)r25 + 0x14);
                r3 = 0x0;
                tmp = tmp & 0x00800000;
                if (r25 == 0) {
                    tmp = *(u32*)((u8*)r25 + 0x14);
                    tmp = tmp & 0x00000040;
                    if (r25 != 0) {
                        r3 = 0x1;
                }
                }
                if ((s32)r3 == 0) {
                    r3 = r25;
                    fn_8019D620();
        }
        }
        }
        tmp = *(u32*)((u8*)r25 + 0x14);
        tmp = tmp & ~r28;
        *(u32*)((u8*)r25 + 0x14) = tmp;
    }
    tmp = *(u32*)((u8*)r25 + 0x14);
    tmp = tmp & 0x00001000;
    if ((s32)r3 != 0) return;
    r29 = *(u32*)((u8*)r25 + 0x10);
    while (1) {
        if (r29 == 0) break;
        do {
        if (r29 == 0) break;

        if (r29 != 0) {
            tmp = *(u32*)((u8*)r29 + 0x14);
            tmp = tmp ^ r28;
            tmp = tmp & 0x00000008;
            if (r29 != 0) {
                if (r29 != 0) {
                    if (r29 == 0) {
                        r3 = (u32)lbl_8047DB34;
                        r4 = 0x25d;
                        r5 = (u32)lbl_8047DB3C;
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
                    if ((s32)r3 == 0) {
                        r3 = r29;
                        fn_8019D620();
            }
            }
            }
            tmp = *(u32*)((u8*)r29 + 0x14);
            tmp = tmp & ~r28;
            *(u32*)((u8*)r29 + 0x14) = tmp;
        }
        tmp = *(u32*)((u8*)r29 + 0x14);
        tmp = tmp & 0x00001000;
        if ((s32)r3 != 0) break;

        r31 = *(u32*)((u8*)r29 + 0x10);
        while (1) {
            if (r31 == 0) break;
            do {
            if (r31 == 0) break;

            if (r31 != 0) {
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp ^ r28;
                tmp = tmp & 0x00000008;
                if (r31 != 0) {
                    if (r31 != 0) {
                        if (r31 == 0) {
                            r3 = (u32)lbl_8047DB34;
                            r4 = 0x25d;
                            r5 = (u32)lbl_8047DB3C;
                            fn_80196E10();
                        }
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        r3 = 0x0;
                        tmp = tmp & 0x00800000;
                        if (r31 == 0) {
                            tmp = *(u32*)((u8*)r31 + 0x14);
                            tmp = tmp & 0x00000040;
                            if (r31 != 0) {
                                r3 = 0x1;
                        }
                        }
                        if ((s32)r3 == 0) {
                            r3 = r31;
                            fn_8019D620();
                }
                }
                }
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp & ~r28;
                *(u32*)((u8*)r31 + 0x14) = tmp;
            }
            tmp = *(u32*)((u8*)r31 + 0x14);
            tmp = tmp & 0x00001000;
            if ((s32)r3 != 0) break;

            r30 = *(u32*)((u8*)r31 + 0x10);
            while (1) {
                if (r30 == 0) break;
                do {
                    if (r30 == 0) break;
                    if (r30 != 0) {
                        tmp = *(u32*)((u8*)r30 + 0x14);
                        tmp = tmp ^ r28;
                        tmp = tmp & 0x00000008;
                        if (r30 != 0) {
                            if (r30 != 0) {
                                r3 = r30;
                                fn_8019D980();
                                if ((s32)r3 == 0) {
                                    r3 = r30;
                                    fn_8019D620();
                        }
                        }
                        }
                        tmp = *(u32*)((u8*)r30 + 0x14);
                        tmp = tmp & ~r28;
                        *(u32*)((u8*)r30 + 0x14) = tmp;
                    }
                    tmp = *(u32*)((u8*)r30 + 0x14);
                    tmp = tmp & 0x00001000;
                    if ((s32)r3 != 0) break;
                    r27 = *(u32*)((u8*)r30 + 0x10);
                    while (r27 != 0) {

                        if (r27 != 0) {
                            if (r27 != 0) {
                                tmp = *(u32*)((u8*)r27 + 0x14);
                                tmp = tmp ^ r28;
                                tmp = tmp & 0x00000008;
                                if (r27 != 0) {
                                    r3 = r27;
                                    fn_8019F778();
                                }
                                tmp = *(u32*)((u8*)r27 + 0x14);
                                tmp = tmp & ~r28;
                                *(u32*)((u8*)r27 + 0x14) = tmp;
                            }
                            tmp = *(u32*)((u8*)r27 + 0x14);
                            tmp = tmp & 0x00001000;
                            if (r27 == 0) {
                                r25 = *(u32*)((u8*)r27 + 0x10);
                                while (r25 != 0) {

                                    if (r25 != 0) {
                                        r3 = r25;
                                        r4 = r28;
                                        fn_8019FAEC();
                                        tmp = *(u32*)((u8*)r25 + 0x14);
                                        tmp = tmp & 0x00001000;
                                        if (r25 == 0) {
                                            r26 = *(u32*)((u8*)r25 + 0x10);
                                            while (r26 != 0) {

                                                r3 = r26;
                                                r4 = r28;
                                                fn_8019F7F0();
                                                r26 = *(u32*)((u8*)r26 + 0x8);

                                            }
                                    }
                                    }
                                    r25 = *(u32*)((u8*)r25 + 0x8);

                                }
                        }
                        }
                        r27 = *(u32*)((u8*)r27 + 0x8);

                    }
                } while (0);

                r30 = *(u32*)((u8*)r30 + 0x8);

            }
            } while (0);

            r31 = *(u32*)((u8*)r31 + 0x8);

        }
        } while (0);

        r29 = *(u32*)((u8*)r29 + 0x8);

    }

    return;
}

/* 0x8019FAEC | 0xA4 */
void fn_8019FAEC(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) return;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp ^ r31;
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) {
        if (r30 != 0) {
            if (r30 == 0) {
                r3 = (u32)lbl_8047DB34;
                r4 = 0x25d;
                r5 = (u32)lbl_8047DB3C;
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
            if ((s32)r3 == 0) {
                r3 = r30;
                fn_8019D620();
    }
    }
    }
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & ~r31;
    *(u32*)((u8*)r30 + 0x14) = tmp;

    return;
}

/* 0x8019FB90 | 0x2FC */
void fn_8019FB90(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
    extern void fn_8019D980();
    extern void fn_8019F778();
    extern void fn_8019FB90();
    extern void fn_8019FE8C();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r25, r3 */;
    r28 = r4;
    if ((s32)tmp == 0) return;
    if (r25 != 0) {
        tmp = *(u32*)((u8*)r25 + 0x14);
        tmp = tmp ^ r28;
        tmp = tmp & 0x00000008;
        if (r25 != 0) {
            if (r25 != 0) {
                if (r25 == 0) {
                    r3 = (u32)lbl_8047DB34;
                    r4 = 0x25d;
                    r5 = (u32)lbl_8047DB3C;
                    fn_80196E10();
                }
                tmp = *(u32*)((u8*)r25 + 0x14);
                r3 = 0x0;
                tmp = tmp & 0x00800000;
                if (r25 == 0) {
                    tmp = *(u32*)((u8*)r25 + 0x14);
                    tmp = tmp & 0x00000040;
                    if (r25 != 0) {
                        r3 = 0x1;
                }
                }
                if ((s32)r3 == 0) {
                    r3 = r25;
                    fn_8019D620();
        }
        }
        }
        tmp = *(u32*)((u8*)r25 + 0x14);
        tmp = tmp | r28;
        *(u32*)((u8*)r25 + 0x14) = tmp;
    }
    tmp = *(u32*)((u8*)r25 + 0x14);
    tmp = tmp & 0x00001000;
    if ((s32)r3 != 0) return;
    r29 = *(u32*)((u8*)r25 + 0x10);
    while (1) {
        if (r29 == 0) break;
        do {
        if (r29 == 0) break;

        if (r29 != 0) {
            tmp = *(u32*)((u8*)r29 + 0x14);
            tmp = tmp ^ r28;
            tmp = tmp & 0x00000008;
            if (r29 != 0) {
                if (r29 != 0) {
                    if (r29 == 0) {
                        r3 = (u32)lbl_8047DB34;
                        r4 = 0x25d;
                        r5 = (u32)lbl_8047DB3C;
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
                    if ((s32)r3 == 0) {
                        r3 = r29;
                        fn_8019D620();
            }
            }
            }
            tmp = *(u32*)((u8*)r29 + 0x14);
            tmp = tmp | r28;
            *(u32*)((u8*)r29 + 0x14) = tmp;
        }
        tmp = *(u32*)((u8*)r29 + 0x14);
        tmp = tmp & 0x00001000;
        if ((s32)r3 != 0) break;

        r31 = *(u32*)((u8*)r29 + 0x10);
        while (1) {
            if (r31 == 0) break;
            do {
            if (r31 == 0) break;

            if (r31 != 0) {
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp ^ r28;
                tmp = tmp & 0x00000008;
                if (r31 != 0) {
                    if (r31 != 0) {
                        if (r31 == 0) {
                            r3 = (u32)lbl_8047DB34;
                            r4 = 0x25d;
                            r5 = (u32)lbl_8047DB3C;
                            fn_80196E10();
                        }
                        tmp = *(u32*)((u8*)r31 + 0x14);
                        r3 = 0x0;
                        tmp = tmp & 0x00800000;
                        if (r31 == 0) {
                            tmp = *(u32*)((u8*)r31 + 0x14);
                            tmp = tmp & 0x00000040;
                            if (r31 != 0) {
                                r3 = 0x1;
                        }
                        }
                        if ((s32)r3 == 0) {
                            r3 = r31;
                            fn_8019D620();
                }
                }
                }
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp | r28;
                *(u32*)((u8*)r31 + 0x14) = tmp;
            }
            tmp = *(u32*)((u8*)r31 + 0x14);
            tmp = tmp & 0x00001000;
            if ((s32)r3 != 0) break;

            r30 = *(u32*)((u8*)r31 + 0x10);
            while (1) {
                if (r30 == 0) break;
                do {
                    if (r30 == 0) break;
                    if (r30 != 0) {
                        tmp = *(u32*)((u8*)r30 + 0x14);
                        tmp = tmp ^ r28;
                        tmp = tmp & 0x00000008;
                        if (r30 != 0) {
                            if (r30 != 0) {
                                r3 = r30;
                                fn_8019D980();
                                if ((s32)r3 == 0) {
                                    r3 = r30;
                                    fn_8019D620();
                        }
                        }
                        }
                        tmp = *(u32*)((u8*)r30 + 0x14);
                        tmp = tmp | r28;
                        *(u32*)((u8*)r30 + 0x14) = tmp;
                    }
                    tmp = *(u32*)((u8*)r30 + 0x14);
                    tmp = tmp & 0x00001000;
                    if ((s32)r3 != 0) break;
                    r27 = *(u32*)((u8*)r30 + 0x10);
                    while (r27 != 0) {

                        if (r27 != 0) {
                            if (r27 != 0) {
                                tmp = *(u32*)((u8*)r27 + 0x14);
                                tmp = tmp ^ r28;
                                tmp = tmp & 0x00000008;
                                if (r27 != 0) {
                                    r3 = r27;
                                    fn_8019F778();
                                }
                                tmp = *(u32*)((u8*)r27 + 0x14);
                                tmp = tmp | r28;
                                *(u32*)((u8*)r27 + 0x14) = tmp;
                            }
                            tmp = *(u32*)((u8*)r27 + 0x14);
                            tmp = tmp & 0x00001000;
                            if (r27 == 0) {
                                r25 = *(u32*)((u8*)r27 + 0x10);
                                while (r25 != 0) {

                                    if (r25 != 0) {
                                        r3 = r25;
                                        r4 = r28;
                                        fn_8019FE8C();
                                        tmp = *(u32*)((u8*)r25 + 0x14);
                                        tmp = tmp & 0x00001000;
                                        if (r25 == 0) {
                                            r26 = *(u32*)((u8*)r25 + 0x10);
                                            while (r26 != 0) {

                                                r3 = r26;
                                                r4 = r28;
                                                fn_8019FB90();
                                                r26 = *(u32*)((u8*)r26 + 0x8);

                                            }
                                    }
                                    }
                                    r25 = *(u32*)((u8*)r25 + 0x8);

                                }
                        }
                        }
                        r27 = *(u32*)((u8*)r27 + 0x8);

                    }
                } while (0);

                r30 = *(u32*)((u8*)r30 + 0x8);

            }
            } while (0);

            r31 = *(u32*)((u8*)r31 + 0x8);

        }
        } while (0);

        r29 = *(u32*)((u8*)r29 + 0x8);

    }

    return;
}

/* 0x8019FE8C | 0xA4 */
void fn_8019FE8C(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) return;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp ^ r31;
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) {
        if (r30 != 0) {
            if (r30 == 0) {
                r3 = (u32)lbl_8047DB34;
                r4 = 0x25d;
                r5 = (u32)lbl_8047DB3C;
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
            if ((s32)r3 == 0) {
                r3 = r30;
                fn_8019D620();
    }
    }
    }
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp | r31;
    *(u32*)((u8*)r30 + 0x14) = tmp;

    return;
}

/* 0x8019FF30 | 0x18 */
u32 fn_8019FF30(u8* ptr) {
    if (ptr != NULL) {
        return *(u32*)(ptr + 0x14);
    }
    return 0;
}

/* 0x8019FF48 | 0x2C */
u32 fn_8019FF48(u8* ptr) {
    if (ptr == NULL) { return 0; }
    if ((*(u32*)(ptr + 0x14) & 0x4020) == 0) { return 0; }
    return *(u32*)(ptr + 0x18);
}

/* 0x8019FF74 | 0x1E8 */
void fn_8019FF74(void) {
    extern u8 lbl_80274B28[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB28[];
    extern void fn_80196D78();
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) return;
    if (r31 == 0) {
        return;
    }
    if (r31 != 0) {
        tmp = *(u32*)((u8*)r31 + 0xC);
        r29 = *(u32*)((u8*)r31 + 0x8);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r31 + 0xC);
            tmp = *(u32*)((u8*)r3 + 0x10);
            if (tmp == r31) {
                r3 = *(u32*)((u8*)r31 + 0xC);
                *(u32*)((u8*)r3 + 0x10) = r29;

            } else {
                do {
                    if (r31 != 0) {
                        tmp = *(u32*)((u8*)r31 + 0xC);
                        if (tmp == 0) {
                        }
                        r28 = 0x0;
                        break;
                        }
                    r3 = *(u32*)((u8*)r31 + 0xC);
                    tmp = *(u32*)((u8*)r3 + 0x10);
                    if (r31 == tmp) {
                        r28 = 0x0;
                        break;
                    }
                    r3 = *(u32*)((u8*)r31 + 0xC);
                    r28 = *(u32*)((u8*)r3 + 0x10);
                    while (1) {
                        if (r28 == 0) break;
                        tmp = *(u32*)((u8*)r28 + 0x8);
                        if (tmp == r31) {
                            break;
                        }
                        r28 = *(u32*)((u8*)r28 + 0x8);

                    }
                    r4 = (u32)lbl_80274B28;
                    r3 = (u32)lbl_8047DB20;
                    r5 = (u32)lbl_80274B28;
                    r4 = 0x5f8;
                    fn_80196D78();
                    r28 = 0x0;
                } while (0);

                if (r28 == 0) {
                    r3 = (u32)lbl_8047DB20;
                    r4 = 0x57b;
                    r5 = (u32)lbl_8047DB28;
                    fn_80196E10();
                }
                *(u32*)((u8*)r28 + 0x8) = r29;
            }
            r6 = *(u32*)((u8*)r31 + 0xC);
            while (1) {
                if (r6 == 0) break;
                r4 = 0x90000000;
                r5 = *(u32*)((u8*)r6 + 0x10);
                while (r5 != 0) {

                    tmp = *(u32*)((u8*)r5 + 0x14);
                    r3 = *(u32*)((u8*)r5 + 0x14);
                    tmp = tmp << 10;
                    r5 = *(u32*)((u8*)r5 + 0x8);
                    tmp = r3 | tmp;
                    tmp = tmp & 0x70000000;
                    r4 = r4 | tmp;

                }
                tmp = *(u32*)((u8*)r6 + 0x14);
                /* andc. tmp, tmp, r4 */;
                if (r5 == 0) break;
                tmp = *(u32*)((u8*)r6 + 0x14);
                tmp = tmp & r4;
                *(u32*)((u8*)r6 + 0x14) = tmp;
                r6 = *(u32*)((u8*)r6 + 0x8);

            }

            tmp = 0x0;
            *(u32*)((u8*)r31 + 0xC) = tmp;
        }
        tmp = 0x0;
        *(u32*)((u8*)r31 + 0x8) = tmp;
    }
    tmp = *(u32*)((u8*)r30 + 0xC);
    *(u32*)((u8*)r31 + 0xC) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r31 + 0x8) = tmp;
    *(u32*)((u8*)r30 + 0x8) = r31;
    tmp = *(u32*)((u8*)r30 + 0xC);
    if (tmp == 0) return;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp << 10;
    r4 = *(u32*)((u8*)r30 + 0xC);
    tmp = r3 | tmp;
    r3 = tmp & 0x70000000;
    while (r4 != 0) {

        tmp = *(u32*)((u8*)r4 + 0x14);
        /* andc. tmp, r3, tmp */;
        if (tmp == 0) return;
        tmp = *(u32*)((u8*)r4 + 0x14);
        tmp = tmp | r3;
        *(u32*)((u8*)r4 + 0x14) = tmp;
        r4 = *(u32*)((u8*)r4 + 0xC);

    }

    return;
}

/* 0x801A015C | 0x154 */
void fn_801A015C(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r4;
    /* mr. r28, r3 */;
    r3 = (u32)lbl_80274AA0;
    r31 = (u32)lbl_80274AA0;
    if ((s32)tmp == 0) return;
    if (r29 == 0) {
        return;
    }
    tmp = *(u32*)((u8*)r29 + 0xC);
    if (tmp != 0) {
        r3 = r31 + 0x178;
        OSReport();
        r5 = r31 + 0x194;
        r3 = (u32)lbl_8047DB20;
        r4 = 0x552;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r29 + 0x8);
    if (tmp != 0) {
        r3 = r31 + 0x1ac;
        OSReport();
        r5 = r31 + 0x1cc;
        r3 = (u32)lbl_8047DB20;
        r4 = 0x553;
        fn_80196E10();
    }
    tmp = *(u32*)((u8*)r28 + 0x10);
    if (tmp == 0) {
        *(u32*)((u8*)r28 + 0x10) = r29;
    } else {

        tmp = *(u32*)((u8*)r28 + 0x14);
        tmp = tmp & 0x00001000;
        if (tmp != 0) {
            r5 = r31 + 0x1e0;
            r3 = (u32)lbl_8047DB20;
            r4 = 0x559;
            fn_80196E10();
        }
        r30 = *(u32*)((u8*)r28 + 0x10);
        while (1) {
            tmp = *(u32*)((u8*)r30 + 0x8);
            if (tmp == 0) break;
            if (r30 == r29) {
                r5 = r31 + 0x200;
                r3 = (u32)lbl_8047DB20;
                r4 = 0x55c;
                fn_80196E10();
            }
            r30 = *(u32*)((u8*)r30 + 0x8);

        }
        *(u32*)((u8*)r30 + 0x8) = r29;
    }
    *(u32*)((u8*)r29 + 0xC) = r28;
    r4 = r28;
    tmp = *(u32*)((u8*)r29 + 0x14);
    r3 = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp << 10;
    tmp = r3 | tmp;
    r3 = tmp & 0x70000000;
    while (r4 != 0) {

        tmp = *(u32*)((u8*)r4 + 0x14);
        /* andc. tmp, r3, tmp */;
        if (tmp == 0) return;
        tmp = *(u32*)((u8*)r4 + 0x14);
        tmp = tmp | r3;
        *(u32*)((u8*)r4 + 0x14) = tmp;
        r4 = *(u32*)((u8*)r4 + 0xC);

    }

    return;
}

/* 0x801A02B0 | 0x28C */
void fn_801A02B0(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196D78();
    extern void fn_80196E10();
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* mr. r29, r3 */;
    r3 = (u32)lbl_80274AA0;
    r31 = (u32)lbl_80274AA0;
    if ((s32)tmp == 0) {
        r3 = 0x0;
        return;
    }
    r30 = *(u32*)((u8*)r29 + 0x10);
    if (r30 != 0) {
        tmp = *(u32*)((u8*)r30 + 0x8);
        if (tmp != 0) {
            r5 = r31 + 0x1cc;
            r3 = (u32)lbl_8047DB20;
            r4 = 0x4cb;
            fn_80196E10();
    }
    }
    if (r30 != 0) {
        r28 = r30;
    } else {

        r28 = *(u32*)((u8*)r29 + 0x8);
    }
    do {
        if (r29 != 0) {
            tmp = *(u32*)((u8*)r29 + 0xC);
            if (tmp == 0) {
            }
            r3 = 0x0;
            break;
            }
        r3 = *(u32*)((u8*)r29 + 0xC);
        tmp = *(u32*)((u8*)r3 + 0x10);
        if (r29 == tmp) {
            r3 = 0x0;
            break;
        }
        r3 = *(u32*)((u8*)r29 + 0xC);
        r3 = *(u32*)((u8*)r3 + 0x10);
        while (1) {
            if (r3 == 0) break;
            tmp = *(u32*)((u8*)r3 + 0x8);
            if (tmp == r29) {
                break;
            }
            r3 = *(u32*)((u8*)r3 + 0x8);

        }
        r5 = r31 + 0x88;
        r3 = (u32)lbl_8047DB20;
        r4 = 0x5f8;
        fn_80196D78();
        r3 = 0x0;
    } while (0);

    if (r3 != 0) {
        *(u32*)((u8*)r3 + 0x8) = r28;

    } else {
        tmp = *(u32*)((u8*)r29 + 0xC);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r29 + 0xC);
            *(u32*)((u8*)r3 + 0x10) = r28;
        }
    }
    if ((r28 != 0) && (r28 == r30)) {

        tmp = *(u32*)((u8*)r29 + 0x8);
        *(u32*)((u8*)r28 + 0x8) = tmp;
        tmp = *(u32*)((u8*)r29 + 0xC);
        *(u32*)((u8*)r28 + 0xC) = tmp;
    }
    tmp = 0x0;
    r3 = 0x0;
    *(u32*)((u8*)r29 + 0xC) = tmp;
    tmp = 0x0;
    *(u32*)((u8*)r29 + 0x10) = r3;
    *(u32*)((u8*)r29 + 0x8) = tmp;
    if (r29 == 0) { r3 = r30; return; }
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
    if ((s32)r3 == 0) { r3 = r30; return; }
    r3 = *(u16*)((u8*)r29 + 0x6);
    /* subic. tmp, r3, 0x1 */;
    if ((s32)r3 < 0) {
        if (r29 == 0) { r3 = r30; return; }
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
        r3 = r30;
        return;
    }
    r3 = *(u16*)((u8*)r29 + 0x6);
    tmp = r3 + 0x1;
    *(u16*)((u8*)r29 + 0x6) = tmp;
    tmp = *(u16*)((u8*)r29 + 0x6);
    if (tmp == 0) {
        r3 = r31 + 0x54;
        r5 = r31 + 0x60;
        r4 = 0x9e;
        fn_80196E10();
    }
    r4 = *(u32*)((u8*)r29 + 0x0);
    r3 = r29;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = *(u16*)((u8*)r29 + 0x6);
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp != 0) {
    } else {

        r3 = *(u16*)((u8*)r29 + 0x6);
        *(u16*)((u8*)r29 + 0x6) = tmp;
        tmp = *(u16*)((u8*)r29 + 0x6);
        tmp = __cntlzw(tmp);
        tmp = (u32)tmp >> 5;
    }
    if ((s32)tmp == 0) { r3 = r30; return; }
    if (r29 == 0) { r3 = r30; return; }
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

    r3 = r30;

    return;
}

/* 0x801A053C | 0xB0 */
void fn_801A053C(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp != 0) {
        tmp = *(u16*)((u8*)r31 + 0x6);
        tmp = __cntlzw(tmp);
        /* srwi. tmp, tmp, 5 */;
        if ((s32)tmp != 0) {
        } else {

            r3 = *(u16*)((u8*)r31 + 0x6);
            *(u16*)((u8*)r31 + 0x6) = tmp;
            tmp = *(u16*)((u8*)r31 + 0x6);
            tmp = __cntlzw(tmp);
            tmp = (u32)tmp >> 5;
        }
        if ((s32)tmp != 0) {
            tmp = *(u16*)((u8*)r31 + 0x4);
            if (tmp == 0xffff) {
                tmp = -0x1;
            } else {

                tmp = *(u16*)((u8*)r31 + 0x4);
            }
            if ((s32)tmp < 0) {
                if (r31 != 0) {
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
    }
    }
    }
    }
    return;
}

/* 0x801A05EC | 0x158 */
void fn_801A05EC(void) {
    extern u8 lbl_80274AF4[];
    extern u8 lbl_80274B00[];
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) return;
    r3 = 0x10000;
    r4 = *(u16*)((u8*)r31 + 0x4);
    tmp = r3 & 0xFFFF;
    tmp = tmp - r4;
    tmp = __cntlzw(tmp);
    /* srwi. r3, tmp, 5 */;
    if ((s32)tmp != 0) {
    } else {

        tmp = *(u16*)((u8*)r31 + 0x4);
        r3 = *(u16*)((u8*)r31 + 0x4);
        r4 = __cntlzw(tmp);
        *(u16*)((u8*)r31 + 0x4) = tmp;
        r3 = (u32)r4 >> 5;
    }
    if ((s32)r3 == 0) return;
    r3 = *(u16*)((u8*)r31 + 0x6);
    /* subic. tmp, r3, 0x1 */;
    if ((s32)r3 < 0) {
        if (r31 == 0) return;
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
        return;
    }
    r3 = *(u16*)((u8*)r31 + 0x6);
    tmp = r3 + 0x1;
    *(u16*)((u8*)r31 + 0x6) = tmp;
    tmp = *(u16*)((u8*)r31 + 0x6);
    if (tmp == 0) {
        r3 = (u32)lbl_80274AF4;
        r5 = (u32)lbl_80274B00;
        r3 = (u32)lbl_80274AF4;
        r4 = 0x9e;
        r5 = (u32)lbl_80274B00;
        fn_80196E10();
    }
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = *(u16*)((u8*)r31 + 0x6);
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp != 0) {
    } else {

        r3 = *(u16*)((u8*)r31 + 0x6);
        *(u16*)((u8*)r31 + 0x6) = tmp;
        tmp = *(u16*)((u8*)r31 + 0x6);
        tmp = __cntlzw(tmp);
        tmp = (u32)tmp >> 5;
    }
    if ((s32)tmp == 0) return;
    if (r31 == 0) return;
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

    return;
}

/* 0x801A0744 | 0x458 */
void fn_801A0744(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196E10();
    extern void fn_801991F8();
    extern void fn_8019C128();
    extern void fn_801A05EC();
    extern void fn_801A0744();
    extern void fn_801A0B9C();
    extern void fn_801A0BF0();
    extern void fn_801A0C1C();
    extern void fn_801A0C68();
    extern void fn_801A0C9C();
    extern void fn_801A0CE8();
    extern void fn_801A0D3C();
    extern void fn_801A0D48();
    extern void fn_801A0D94();
    extern void fn_801AEBE4();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r5 = (u32)lbl_80274AA0;
    r30 = r3;
    r29 = (u32)lbl_80274AA0;
    r31 = r4;
    while (1) {
        if (r30 == 0) return;
        if (r31 == 0) break;
        do {
        if (r30 == 0 || r31 == 0) break;

        r3 = *(u32*)((u8*)r30 + 0x80);
        r4 = *(u32*)((u8*)r31 + 0x3C);
        fn_801AEBE4();
        tmp = *(u32*)((u8*)r30 + 0x14);
        tmp = tmp & 0x00001000;
        do {
        do {
            if (r31 == 0) break;
            r28 = *(u32*)((u8*)r30 + 0x10);
            if (r28 == 0) break;
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
            if ((s32)r3 == 0) break;
            r3 = *(u16*)((u8*)r28 + 0x6);
            /* subic. tmp, r3, 0x1 */;
            if ((s32)r3 < 0) {
                if (r28 == 0) break;
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
                break;
            }
            r3 = *(u16*)((u8*)r28 + 0x6);
            tmp = r3 + 0x1;
            *(u16*)((u8*)r28 + 0x6) = tmp;
            tmp = *(u16*)((u8*)r28 + 0x6);
            if (tmp == 0) {
                r3 = r29 + 0x54;
                r5 = r29 + 0x60;
                r4 = 0x9e;
                fn_80196E10();
            }
            r4 = *(u32*)((u8*)r28 + 0x0);
            r3 = r28;
            r12 = *(u32*)((u8*)r4 + 0x4C);
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
            tmp = *(u16*)((u8*)r28 + 0x6);
            tmp = __cntlzw(tmp);
            /* srwi. tmp, tmp, 5 */;
            if (tmp != 0) {
            } else {

                r3 = *(u16*)((u8*)r28 + 0x6);
                *(u16*)((u8*)r28 + 0x6) = tmp;
                tmp = *(u16*)((u8*)r28 + 0x6);
                tmp = __cntlzw(tmp);
                tmp = (u32)tmp >> 5;
            }
            if ((s32)tmp == 0 || r28 == 0) break;

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
        } while (0);
            r4 = *(u32*)((u8*)r31 + 0x8);
            r3 = 0x0;
            r5 = 0x0;
            fn_8019C128();
            *(u32*)((u8*)r30 + 0x10) = r3;
            tmp = *(u32*)((u8*)r30 + 0x10);
            if (tmp == 0) {
                r5 = r29 + 0x210;
                r3 = (u32)lbl_8047DB20;
                r4 = 0x45f;
                fn_80196E10();
            }
            r4 = *(u32*)((u8*)r30 + 0x10);
            if (r4 == 0) break;
            r3 = *(u16*)((u8*)r4 + 0x4);
            tmp = r3 + 0x1;
            *(u16*)((u8*)r4 + 0x4) = tmp;
            tmp = *(u16*)((u8*)r4 + 0x4);
            if (tmp != 0xffff) break;
            r3 = r29 + 0x54;
            r5 = r29 + 0xc4;
            r4 = 0x5d;
            fn_80196E10();
        } while (0);

        tmp = *(u32*)((u8*)r30 + 0x14);
        tmp = tmp & 0x4020;
        tmp = __cntlzw(tmp);
        /* srwi. tmp, tmp, 5 */;
        if (tmp == 0xffff) break;

        r3 = *(u32*)((u8*)r30 + 0x18);
        r4 = *(u32*)((u8*)r31 + 0x10);
        fn_801991F8();
        } while (0);

        tmp = *(u32*)((u8*)r30 + 0x14);
        tmp = tmp & 0x00001000;
        if (tmp == 0xffff) {
            r27 = *(u32*)((u8*)r31 + 0x8);
            r28 = *(u32*)((u8*)r30 + 0x10);
            goto L_801A0B60;
        do {
                do {
                    if (r28 == 0 || r27 == 0) break;

                    r3 = *(u32*)((u8*)r28 + 0x80);
                    r4 = *(u32*)((u8*)r27 + 0x3C);
                    fn_801AEBE4();
                    tmp = *(u32*)((u8*)r28 + 0x14);
                    tmp = tmp & 0x00001000;
                    do {
                    do {
                        if (r27 == 0) break;
                        r25 = *(u32*)((u8*)r28 + 0x10);
                        if (r25 == 0) break;
                        r3 = r25;
                        fn_801A0D48();
                        if ((s32)r3 == 0) break;
                        r3 = r25;
                        fn_801A0D3C();
                        if ((s32)r3 < 0) {
                            r3 = r25;
                            fn_801A0CE8();
                            break;
                        }
                        r3 = r25;
                        fn_801A0C9C();
                        r4 = *(u32*)((u8*)r25 + 0x0);
                        r3 = r25;
                        r12 = *(u32*)((u8*)r4 + 0x4C);
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        r3 = r25;
                        fn_801A0C68();
                        if ((s32)r3 == 0) break;
                        r3 = r25;
                        fn_801A0CE8();
                    } while (0);
                        r4 = *(u32*)((u8*)r27 + 0x8);
                        r3 = 0x0;
                        r5 = 0x0;
                        fn_8019C128();
                        *(u32*)((u8*)r28 + 0x10) = r3;
                        tmp = *(u32*)((u8*)r28 + 0x10);
                        if (tmp == 0) {
                            r5 = r29 + 0x210;
                            r3 = (u32)lbl_8047DB20;
                            r4 = 0x45f;
                            fn_80196E10();
                        }
                        r3 = *(u32*)((u8*)r28 + 0x10);
                        if (r3 == 0) break;
                        fn_801A0C1C();
                    } while (0);

                    tmp = *(u32*)((u8*)r28 + 0x14);
                    tmp = tmp & 0x4020;
                    tmp = __cntlzw(tmp);
                    /* srwi. tmp, tmp, 5 */;
                    if (r3 == 0) break;
                    r3 = *(u32*)((u8*)r28 + 0x18);
                    r4 = *(u32*)((u8*)r27 + 0x10);
                    fn_801991F8();
                } while (0);

                tmp = *(u32*)((u8*)r28 + 0x14);
                tmp = tmp & 0x00001000;
                if (r3 == 0) {
                    r25 = *(u32*)((u8*)r27 + 0x8);
                    r26 = *(u32*)((u8*)r28 + 0x10);
                    while (r26 != 0 && r25 != 0) {

                        if (r26 != 0) {
                            if (r25 != 0) {
                                r3 = *(u32*)((u8*)r26 + 0x80);
                                r4 = *(u32*)((u8*)r25 + 0x3C);
                                fn_801AEBE4();
                                tmp = *(u32*)((u8*)r26 + 0x14);
                                tmp = tmp & 0x00001000;
                                if (r25 != 0) {
                                    r3 = *(u32*)((u8*)r26 + 0x10);
                                    fn_801A05EC();
                                    r3 = *(u32*)((u8*)r25 + 0x8);
                                    r4 = 0x0;
                                    fn_801A0BF0();
                                    *(u32*)((u8*)r26 + 0x10) = r3;
                                    tmp = *(u32*)((u8*)r26 + 0x10);
                                    if (tmp == 0) {
                                        r5 = r29 + 0x210;
                                        r3 = (u32)lbl_8047DB20;
                                        r4 = 0x45f;
                                        fn_80196E10();
                                    }
                                    r3 = *(u32*)((u8*)r26 + 0x10);
                                    fn_801A0B9C();
                                }
                                tmp = *(u32*)((u8*)r26 + 0x14);
                                tmp = tmp & 0x4020;
                                tmp = __cntlzw(tmp);
                                /* srwi. tmp, tmp, 5 */;
                                if (tmp != 0) {
                                    r3 = *(u32*)((u8*)r26 + 0x18);
                                    r4 = *(u32*)((u8*)r25 + 0x10);
                                    fn_801991F8();
                        }
                        }
                        }
                        tmp = *(u32*)((u8*)r26 + 0x14);
                        tmp = tmp & 0x00001000;
                        if (tmp == 0) {
                            r24 = *(u32*)((u8*)r26 + 0x10);
                            r23 = *(u32*)((u8*)r25 + 0x8);
                            while (r24 != 0 && r23 != 0) {

                                r3 = r24;
                                r4 = r23;
                                fn_801A0D94();
                                tmp = *(u32*)((u8*)r24 + 0x14);
                                tmp = tmp & 0x00001000;
                                if (tmp == 0) {
                                    r3 = *(u32*)((u8*)r24 + 0x10);
                                    r4 = *(u32*)((u8*)r23 + 0x8);
                                    fn_801A0744();
                                }
                                r24 = *(u32*)((u8*)r24 + 0x8);
                                r23 = *(u32*)((u8*)r23 + 0xC);

                            }
                        }
                        r26 = *(u32*)((u8*)r26 + 0x8);
                        r25 = *(u32*)((u8*)r25 + 0xC);

                    }
                }
                r28 = *(u32*)((u8*)r28 + 0x8);
                r27 = *(u32*)((u8*)r27 + 0xC);
            L_801A0B60:
                if (r28 == 0) break;
        } while (r27 != 0);
        }
        r30 = *(u32*)((u8*)r30 + 0x8);
        r31 = *(u32*)((u8*)r31 + 0xC);

    }

    return;
}

/* 0x54 | fn_801A0B9C | guarded_call */
void fn_801A0B9C(void) {
    if (0 /* guard r3 == 0 */) { return; }
    if (1 /* guard r0 != 0xFFFF */) { return; }
    fn_80196E10();
}

/* 0x801A0BF0 | 0x2C */
void fn_801A0BF0(void) {
    extern void fn_8019C128();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = r4;
    r4 = r3;
    r3 = 0x0;
    fn_8019C128();
    return;
}

/* 0x4C | fn_801A0C1C | generic */
void fn_801A0C1C(void) {
    /* refs: lbl_80274AF4, lbl_80274B64 */
    fn_80196E10();
}

/* 0x801A0C68 | 0x34 */
void fn_801A0C68(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    tmp = *(u16*)((u8*)r3 + 0x6);
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if ((s32)tmp != 0) {
        r3 = tmp;
        return;
    }
    r4 = *(u16*)((u8*)r3 + 0x6);
    *(u16*)((u8*)r3 + 0x6) = tmp;
    tmp = *(u16*)((u8*)r3 + 0x6);
    tmp = __cntlzw(tmp);
    r3 = (u32)tmp >> 5;
    return;
}

/* 0x4C | fn_801A0C9C | generic */
void fn_801A0C9C(void) {
    /* refs: lbl_80274AF4, lbl_80274B00 */
    fn_80196E10();
}

/* 0x54 | fn_801A0CE8 | framed_no_calls */
void fn_801A0CE8(u32 arg1, u32 arg2) {
    /* data manipulation using stack locals */
}

/* 0x801A0D3C | 0xC */
void fn_801A0D3C(void) {
}

/* 0x4C | fn_801A0D48 | generic */
u32 fn_801A0D48(void) {
    return 0;
}

/* 0x801A0D94 | 0x228 */
void fn_801A0D94(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196E10();
    extern void fn_801991F8();
    extern void fn_8019C128();
    extern void fn_801AEBE4();
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

    r29 = r4;
    /* mr. r28, r3 */;
    r3 = (u32)lbl_80274AA0;
    r31 = (u32)lbl_80274AA0;
    if ((s32)tmp == 0) return;
    if (r29 == 0) {
        return;
    }
    r3 = *(u32*)((u8*)r28 + 0x80);
    r4 = *(u32*)((u8*)r29 + 0x3C);
    fn_801AEBE4();
    tmp = *(u32*)((u8*)r28 + 0x14);
    tmp = tmp & 0x00001000;
    do {
    do {
        if (r29 == 0) break;
        r30 = *(u32*)((u8*)r28 + 0x10);
        if (r30 == 0) break;
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
        if ((s32)r3 == 0) break;
        r3 = *(u16*)((u8*)r30 + 0x6);
        /* subic. tmp, r3, 0x1 */;
        if ((s32)r3 < 0) {
            if (r30 == 0) break;
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
            break;
        }
        r3 = *(u16*)((u8*)r30 + 0x6);
        tmp = r3 + 0x1;
        *(u16*)((u8*)r30 + 0x6) = tmp;
        tmp = *(u16*)((u8*)r30 + 0x6);
        if (tmp == 0) {
            r3 = r31 + 0x54;
            r5 = r31 + 0x60;
            r4 = 0x9e;
            fn_80196E10();
        }
        r4 = *(u32*)((u8*)r30 + 0x0);
        r3 = r30;
        r12 = *(u32*)((u8*)r4 + 0x4C);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
        tmp = *(u16*)((u8*)r30 + 0x6);
        tmp = __cntlzw(tmp);
        /* srwi. tmp, tmp, 5 */;
        if (tmp != 0) {
        } else {

            r3 = *(u16*)((u8*)r30 + 0x6);
            *(u16*)((u8*)r30 + 0x6) = tmp;
            tmp = *(u16*)((u8*)r30 + 0x6);
            tmp = __cntlzw(tmp);
            tmp = (u32)tmp >> 5;
        }
        if ((s32)tmp == 0 || r30 == 0) break;

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
    } while (0);
        r4 = *(u32*)((u8*)r29 + 0x8);
        r3 = 0x0;
        r5 = 0x0;
        fn_8019C128();
        *(u32*)((u8*)r28 + 0x10) = r3;
        tmp = *(u32*)((u8*)r28 + 0x10);
        if (tmp == 0) {
            r5 = r31 + 0x210;
            r3 = (u32)lbl_8047DB20;
            r4 = 0x45f;
            fn_80196E10();
        }
        r4 = *(u32*)((u8*)r28 + 0x10);
        if (r4 == 0) break;
        r3 = *(u16*)((u8*)r4 + 0x4);
        tmp = r3 + 0x1;
        *(u16*)((u8*)r4 + 0x4) = tmp;
        tmp = *(u16*)((u8*)r4 + 0x4);
        if (tmp != 0xffff) break;
        r3 = r31 + 0x54;
        r5 = r31 + 0xc4;
        r4 = 0x5d;
        fn_80196E10();
    } while (0);

    tmp = *(u32*)((u8*)r28 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp == 0xffff) return;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = *(u32*)((u8*)r29 + 0x10);
    fn_801991F8();

    return;
}

/* 0x801A0FBC | 0xDC */
void fn_801A0FBC(void) {
    extern u8 lbl_8036C8E0[];
    extern u8 lbl_8047B298[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80193748();
    extern void fn_80193828();
    extern void fn_80196E10();
    extern void fn_801A0744();
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

    /* mr. r30, r3 */;
    if ((s32)tmp == 0) {
        r31 = 0x0;

    } else {
        tmp = *(u32*)((u8*)r30 + 0x0);
        if (tmp != 0) {
            r3 = *(u32*)((u8*)r30 + 0x0);
            fn_80193748();
            if (r3 == 0) {
            }
            tmp = *(u32*)lbl_8047B298;
            if (tmp != 0) {
                r3 = *(u32*)lbl_8047B298;
            } else {

                r3 = (u32)lbl_8036C8E0;
                r3 = (u32)lbl_8036C8E0;
            }
            fn_80193828();
            /* mr. r31, r3 */;
            if (tmp == 0) {
                r3 = (u32)lbl_8047DB20;
                r4 = 0x7df;
                r5 = (u32)lbl_8047DB3C;
                fn_80196E10();
            }

            } else {
            fn_80193828();
            /* mr. r31, r3 */;
            if (tmp == 0) {
                r3 = (u32)lbl_8047DB20;
                r4 = 0x3d5;
                r5 = (u32)lbl_8047DB3C;
                fn_80196E10();
            }
            }
        r6 = *(u32*)((u8*)r31 + 0x0);
        r3 = r31;
        r4 = r30;
        r5 = 0x0;
        r12 = *(u32*)((u8*)r6 + 0x3C);
        ctr_fn = (void(*)(void))r12;
        ctr_fn();
    }
    r4 = r30;
    r3 = r31;
    fn_801A0744();
    r3 = r31;
    return;
}

/* 0x801A1098 | 0x334 */
void fn_801A1098(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8036C8E0[];
    extern u8 lbl_8047B298[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB3C[];
    extern void fn_800A2D38();
    extern void fn_80193748();
    extern void fn_80193828();
    extern void fn_80196E10();
    extern void fn_801992D8();
    extern void fn_8019C264();
    extern void fn_801A8524();
    extern void fn_801AE5E8();
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
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r5 = (u32)lbl_80274AA0;
    tmp = 0x0;
    r29 = r3;
    r31 = (u32)lbl_80274AA0;
    r30 = r4;
    *(u32*)((u8*)r3 + 0x10) = tmp;
    r3 = 0x0;
    tmp = 0x0;
    *(u32*)((u8*)r29 + 0x8) = r3;
    *(u32*)((u8*)r29 + 0xC) = tmp;
    r3 = *(u32*)((u8*)r29 + 0x14);
    tmp = *(u32*)((u8*)r4 + 0x4);
    tmp = r3 | tmp;
    *(u32*)((u8*)r29 + 0x14) = tmp;
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x00004000;
    do {
        if ((s32)tmp != 0) {
            tmp = *(u32*)((u8*)r30 + 0x10);
            *(u32*)((u8*)r29 + 0x18) = tmp;
            break;
        }
        tmp = *(u32*)((u8*)r29 + 0x14);
        tmp = tmp & 0x00000020;
        if ((s32)tmp != 0) {
            tmp = *(u32*)((u8*)r30 + 0x10);
            *(u32*)((u8*)r29 + 0x18) = tmp;
            r3 = *(u32*)((u8*)r30 + 0x10);
            while (r3 != 0) {

                tmp = *(u32*)((u8*)r3 + 0x4);
                tmp = tmp | (0x8000 << 16);
                *(u32*)((u8*)r3 + 0x4) = tmp;
                r3 = *(u32*)((u8*)r3 + 0x0);

            }
            break;
        }
        r3 = *(u32*)((u8*)r30 + 0x10);
        fn_801992D8();
        *(u32*)((u8*)r29 + 0x18) = r3;
    } while (0);

    r3 = *(u32*)((u8*)r30 + 0x3C);
    fn_801AE5E8();
    tmp = r3;
    r3 = r29 + 0x44;
    *(u32*)((u8*)r29 + 0x80) = tmp;
    f0 = *(f32*)((u8*)r30 + 0x14);
    *(f32*)((u8*)r29 + 0x1C) = f0;
    f0 = *(f32*)((u8*)r30 + 0x18);
    *(f32*)((u8*)r29 + 0x20) = f0;
    f0 = *(f32*)((u8*)r30 + 0x1C);
    *(f32*)((u8*)r29 + 0x24) = f0;
    r4 = *(u32*)((u8*)r30 + 0x20);
    tmp = *(u32*)((u8*)r30 + 0x24);
    *(u32*)((u8*)r29 + 0x2C) = r4;
    *(u32*)((u8*)r29 + 0x30) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x28);
    *(u32*)((u8*)r29 + 0x34) = tmp;
    r4 = *(u32*)((u8*)r30 + 0x2C);
    tmp = *(u32*)((u8*)r30 + 0x30);
    *(u32*)((u8*)r29 + 0x38) = r4;
    *(u32*)((u8*)r29 + 0x3C) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x34);
    *(u32*)((u8*)r29 + 0x40) = tmp;
    fn_800A2D38();
    tmp = 0x0;
    *(u32*)((u8*)r29 + 0x74) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x38);
    if (tmp != 0) {
        fn_801A8524();
        *(u32*)((u8*)r29 + 0x78) = r3;
        r5 = 0x30;
        r3 = *(u32*)((u8*)r29 + 0x78);
        r4 = *(u32*)((u8*)r30 + 0x38);
        memcpy((void*)r3, (const void*)r4, (u32)r5);
    }
    r4 = r30;
    r5 = r29;
    r3 = 0x0;
    fn_8019C264();
    *(u32*)((u8*)r29 + 0x84) = r30;
    tmp = *(u32*)((u8*)r30 + 0x4);
    tmp = tmp & 0x00001000;
    if (tmp != 0) { r3 = 0x0; return; }
    r30 = *(u32*)((u8*)r30 + 0x8);
    while (1) {
        if (r30 == 0) break;
        if (r30 == 0) {
            r28 = 0x0;

        } else {
            tmp = *(u32*)((u8*)r30 + 0x0);
            if (tmp != 0) {
                r3 = *(u32*)((u8*)r30 + 0x0);
                fn_80193748();
                if (r3 == 0) {
                }
                tmp = *(u32*)lbl_8047B298;
                if (tmp != 0) {
                    r3 = *(u32*)lbl_8047B298;
                } else {

                    r3 = (u32)lbl_8036C8E0;
                    r3 = (u32)lbl_8036C8E0;
                }
                fn_80193828();
                /* mr. r28, r3 */;
                if (tmp == 0) {
                    r3 = (u32)lbl_8047DB20;
                    r4 = 0x7df;
                    r5 = (u32)lbl_8047DB3C;
                    fn_80196E10();
                }

                } else {
                fn_80193828();
                /* mr. r28, r3 */;
                if (tmp == 0) {
                    r3 = (u32)lbl_8047DB20;
                    r4 = 0x3d5;
                    r5 = (u32)lbl_8047DB3C;
                    fn_80196E10();
                }
                }
            r6 = *(u32*)((u8*)r28 + 0x0);
            r3 = r28;
            r4 = r30;
            r5 = r29;
            r12 = *(u32*)((u8*)r6 + 0x3C);
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
        }
        /* mr. r27, r28 */;
        if (tmp == 0) {
            r3 = -0x1;
            return;
        }
        if (r29 != 0 && r27 != 0) {

            tmp = *(u32*)((u8*)r27 + 0xC);
            if (tmp != 0) {
                r3 = r31 + 0x178;
                OSReport();
                r5 = r31 + 0x194;
                r3 = (u32)lbl_8047DB20;
                r4 = 0x552;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r27 + 0x8);
            if (tmp != 0) {
                r3 = r31 + 0x1ac;
                OSReport();
                r5 = r31 + 0x1cc;
                r3 = (u32)lbl_8047DB20;
                r4 = 0x553;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r29 + 0x10);
            if (tmp == 0) {
                *(u32*)((u8*)r29 + 0x10) = r27;
            } else {

                tmp = *(u32*)((u8*)r29 + 0x14);
                tmp = tmp & 0x00001000;
                if (tmp != 0) {
                    r5 = r31 + 0x1e0;
                    r3 = (u32)lbl_8047DB20;
                    r4 = 0x559;
                    fn_80196E10();
                }
                r28 = *(u32*)((u8*)r29 + 0x10);
                while (1) {
                    tmp = *(u32*)((u8*)r28 + 0x8);
                    if (tmp == 0) break;
                    if (r28 == r27) {
                        r5 = r31 + 0x200;
                        r3 = (u32)lbl_8047DB20;
                        r4 = 0x55c;
                        fn_80196E10();
                    }
                    r28 = *(u32*)((u8*)r28 + 0x8);

                }
                *(u32*)((u8*)r28 + 0x8) = r27;
            }
            *(u32*)((u8*)r27 + 0xC) = r29;
            r4 = r29;
            tmp = *(u32*)((u8*)r27 + 0x14);
            r3 = *(u32*)((u8*)r27 + 0x14);
            tmp = tmp << 10;
            tmp = r3 | tmp;
            r3 = tmp & 0x70000000;
            while (1) {
                if (r4 == 0) break;
                tmp = *(u32*)((u8*)r4 + 0x14);
                /* andc. tmp, r3, tmp */;
                if (tmp == 0) break;
                tmp = *(u32*)((u8*)r4 + 0x14);
                tmp = tmp | r3;
                *(u32*)((u8*)r4 + 0x14) = tmp;
                r4 = *(u32*)((u8*)r4 + 0xC);

            }
        }
        r30 = *(u32*)((u8*)r30 + 0xC);

    }

    r3 = 0x0;

    return;
}

/* 0x801A13CC | 0x5B4 */
void fn_801A13CC(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_800A2D98();
    extern void fn_800A2EB4();
    extern void fn_801942B8();
    extern void fn_80196E10();
    extern void fn_80197344();
    extern void fn_8019D9DC();
    extern void fn_801A13CC();
    extern void fn_801A1980();
    extern void fn_801A1988();
    extern void fn_801A1A00();
    u8 sp[0x170];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    /* mr. r27, r3 */;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    if ((s32)tmp == 0) return;
    tmp = *(u32*)((u8*)r27 + 0x14);
    tmp = tmp & 0x00001000;
    if ((s32)tmp != 0) {
        tmp = *(u32*)((u8*)r27 + 0x14);
        tmp = tmp & 0x00000010;
        if ((s32)tmp != 0) return;
        if (r27 != 0) {
            if (r27 == 0) {
                r3 = (u32)lbl_8047DB34;
                r4 = 0x25d;
                r5 = (u32)lbl_8047DB3C;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r27 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r27 == 0) {
                tmp = *(u32*)((u8*)r27 + 0x14);
                tmp = tmp & 0x00000040;
                if (r27 != 0) {
                    r3 = 0x1;
            }
            }
            if ((s32)r3 != 0) {
                r3 = r27;
                fn_8019D9DC();
        }
        }
        r31 = *(u32*)((u8*)r27 + 0x10);
        if (r31 != 0) {
            if (r31 == 0) {
                r3 = (u32)lbl_8047DB34;
                r4 = 0x25d;
                r5 = (u32)lbl_8047DB3C;
                fn_80196E10();
            }
            tmp = *(u32*)((u8*)r31 + 0x14);
            r3 = 0x0;
            tmp = tmp & 0x00800000;
            if (r31 == 0) {
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp & 0x00000040;
                if (r31 != 0) {
                    r3 = 0x1;
            }
            }
            if ((s32)r3 != 0) {
                r3 = r31;
                fn_8019D9DC();
        }
        }
        r3 = *(u32*)((u8*)r27 + 0x10);
        r4 = (u32)sp + 0x128;
        r3 = r3 + 0x44;
        fn_800A2EB4();
        r3 = r27 + 0x44;
        r4 = (u32)sp + 0x128;
        r5 = (u32)sp + 0x128;
        fn_800A2D98();
        if (r28 != 0) {
            r3 = r28;
            r4 = (u32)sp + 0x128;
            r5 = (u32)sp + 0x128;
            fn_800A2D98();

        } else {
            fn_801942B8();
            if (r3 != 0) {
                r4 = (u32)sp + 0x128;
                r3 = r3 + 0x54;
                r5 = (u32)sp + 0x128;
                fn_800A2D98();
            }
        }
        r31 = *(u32*)((u8*)r27 + 0x10);
        if (r31 == 0) return;
        tmp = *(u32*)((u8*)r31 + 0x14);
        tmp = tmp & 0x00001000;
        if (r31 != 0) {
            tmp = *(u32*)((u8*)r31 + 0x14);
            tmp = tmp & 0x00000010;
            if (r31 != 0) return;
            r3 = r31;
            fn_801A1988();
            r3 = *(u32*)((u8*)r31 + 0x10);
            fn_801A1988();
            r3 = *(u32*)((u8*)r31 + 0x10);
            r4 = (u32)sp + 0xf8;
            r3 = r3 + 0x44;
            fn_800A2EB4();
            r3 = r31 + 0x44;
            r4 = (u32)sp + 0xf8;
            r5 = (u32)sp + 0xf8;
            fn_800A2D98();
            r3 = (u32)sp + 0x128;
            r4 = (u32)sp + 0xf8;
            r5 = (u32)sp + 0xf8;
            fn_800A2D98();
            r27 = *(u32*)((u8*)r31 + 0x10);
            if (r27 == 0) return;
            tmp = *(u32*)((u8*)r27 + 0x14);
            tmp = tmp & 0x00001000;
            if (r27 != 0) {
                tmp = *(u32*)((u8*)r27 + 0x14);
                tmp = tmp & 0x00000010;
                if (r27 != 0) return;
                r4 = r27;
                r3 = (u32)sp + 0xf8;
                r5 = (u32)sp + 0x98;
                fn_801A1A00();
                r3 = *(u32*)((u8*)r27 + 0x10);
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0x98;
                fn_801A13CC();
                return;
            }
            r3 = *(u32*)((u8*)r27 + 0x14);
            tmp = r29 << 18;
            /* and. tmp, r3, tmp */;
            if (r27 != 0) {
                r3 = r27;
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0xf8;
                fn_80197344();
            }
            r3 = *(u32*)((u8*)r27 + 0x14);
            tmp = r29 << 28;
            /* and. tmp, r3, tmp */;
            if (r27 == 0) return;
            r27 = *(u32*)((u8*)r27 + 0x10);
            while (r27 != 0) {

                r3 = r27;
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0xf8;
                fn_801A13CC();
                r27 = *(u32*)((u8*)r27 + 0x8);

            }
            return;
        }
        r3 = *(u32*)((u8*)r31 + 0x14);
        tmp = r29 << 18;
        /* and. tmp, r3, tmp */;
        if (r27 != 0) {
            r3 = r31;
            r5 = r29;
            r6 = r30;
            r4 = (u32)sp + 0x128;
            fn_80197344();
        }
        r3 = *(u32*)((u8*)r31 + 0x14);
        tmp = r29 << 28;
        /* and. tmp, r3, tmp */;
        if (r27 == 0) return;
        r31 = *(u32*)((u8*)r31 + 0x10);
        while (1) {
        do {
            if (r31 == 0) break;
            /* mr. r27, r31 */;
            if (r27 == 0) break;
            tmp = *(u32*)((u8*)r27 + 0x14);
            tmp = tmp & 0x00001000;
            if (r27 != 0) {
                tmp = *(u32*)((u8*)r27 + 0x14);
                tmp = tmp & 0x00000010;
                if (r27 != 0) break;
                r4 = r27;
                r3 = (u32)sp + 0x128;
                r5 = (u32)sp + 0x68;
                fn_801A1A00();
                r3 = *(u32*)((u8*)r27 + 0x10);
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0x68;
                fn_801A13CC();
                break;
            }
            r3 = *(u32*)((u8*)r27 + 0x14);
            tmp = r29 << 18;
            /* and. tmp, r3, tmp */;
            if (r27 != 0) {
                r3 = r27;
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0x128;
                fn_80197344();
            }
            r3 = *(u32*)((u8*)r27 + 0x14);
            tmp = r29 << 28;
            /* and. tmp, r3, tmp */;
            if (r27 == 0) break;
            r27 = *(u32*)((u8*)r27 + 0x10);
            while (r27 != 0) {

                r3 = r27;
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0x128;
                fn_801A13CC();
                r27 = *(u32*)((u8*)r27 + 0x8);

            }
        } while (0);
            r31 = *(u32*)((u8*)r31 + 0x8);

        }
        return;
    }
    r3 = *(u32*)((u8*)r27 + 0x14);
    tmp = r29 << 18;
    /* and. tmp, r3, tmp */;
    if (r31 != 0) {
        r3 = r27;
        r4 = r28;
        r5 = r29;
        r6 = r30;
        fn_80197344();
    }
    r3 = *(u32*)((u8*)r27 + 0x14);
    tmp = r29 << 28;
    /* and. tmp, r3, tmp */;
    if (r31 == 0) return;
    r27 = *(u32*)((u8*)r27 + 0x10);
    while (1) {
    do {
        if (r27 == 0) break;
        /* mr. r31, r27 */;
        if (r31 == 0) break;
        tmp = *(u32*)((u8*)r31 + 0x14);
        tmp = tmp & 0x00001000;
        if (r31 != 0) {
            tmp = *(u32*)((u8*)r31 + 0x14);
            tmp = tmp & 0x00000010;
            if (r31 != 0) break;
            r3 = r31;
            fn_801A1988();
            r3 = *(u32*)((u8*)r31 + 0x10);
            fn_801A1988();
            r3 = *(u32*)((u8*)r31 + 0x10);
            r4 = (u32)sp + 0xc8;
            r3 = r3 + 0x44;
            fn_800A2EB4();
            r3 = r31 + 0x44;
            r4 = (u32)sp + 0xc8;
            r5 = (u32)sp + 0xc8;
            fn_800A2D98();
            if (r28 != 0) {
                r3 = r28;
                r4 = (u32)sp + 0xc8;
                r5 = (u32)sp + 0xc8;
                fn_800A2D98();

            } else {
                fn_801942B8();
                if (r3 != 0) {
                    fn_801A1980();
                    r4 = (u32)sp + 0xc8;
                    r5 = (u32)sp + 0xc8;
                    fn_800A2D98();
                }
            }
            r31 = *(u32*)((u8*)r31 + 0x10);
            if (r31 == 0) break;
            tmp = *(u32*)((u8*)r31 + 0x14);
            tmp = tmp & 0x00001000;
            if (r31 != 0) {
                tmp = *(u32*)((u8*)r31 + 0x14);
                tmp = tmp & 0x00000010;
                if (r31 != 0) break;
                r4 = r31;
                r3 = (u32)sp + 0xc8;
                r5 = (u32)sp + 0x38;
                fn_801A1A00();
                r3 = *(u32*)((u8*)r31 + 0x10);
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0x38;
                fn_801A13CC();
                break;
            }
            r3 = *(u32*)((u8*)r31 + 0x14);
            tmp = r29 << 18;
            /* and. tmp, r3, tmp */;
            if (r31 != 0) {
                r3 = r31;
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0xc8;
                fn_80197344();
            }
            r3 = *(u32*)((u8*)r31 + 0x14);
            tmp = r29 << 28;
            /* and. tmp, r3, tmp */;
            if (r31 == 0) break;
            r31 = *(u32*)((u8*)r31 + 0x10);
            while (r31 != 0) {

                r3 = r31;
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0xc8;
                fn_801A13CC();
                r31 = *(u32*)((u8*)r31 + 0x8);

            }
            break;
        }
        r3 = *(u32*)((u8*)r31 + 0x14);
        tmp = r29 << 18;
        /* and. tmp, r3, tmp */;
        if (r31 != 0) {
            r3 = r31;
            r4 = r28;
            r5 = r29;
            r6 = r30;
            fn_80197344();
        }
        r3 = *(u32*)((u8*)r31 + 0x14);
        tmp = r29 << 28;
        /* and. tmp, r3, tmp */;
        if (r31 == 0) break;
        r31 = *(u32*)((u8*)r31 + 0x10);
        while (1) {
        do {
            if (r31 == 0) break;
            /* mr. r26, r31 */;
            if (r31 == 0) break;
            tmp = *(u32*)((u8*)r26 + 0x14);
            tmp = tmp & 0x00001000;
            if (r31 != 0) {
                tmp = *(u32*)((u8*)r26 + 0x14);
                tmp = tmp & 0x00000010;
                if (r31 != 0) break;
                r3 = r28;
                r4 = r26;
                r5 = (u32)sp + 0x8;
                fn_801A1A00();
                r3 = *(u32*)((u8*)r26 + 0x10);
                r5 = r29;
                r6 = r30;
                r4 = (u32)sp + 0x8;
                fn_801A13CC();
                break;
            }
            r3 = *(u32*)((u8*)r26 + 0x14);
            tmp = r29 << 18;
            /* and. tmp, r3, tmp */;
            if (r31 != 0) {
                r3 = r26;
                r4 = r28;
                r5 = r29;
                r6 = r30;
                fn_80197344();
            }
            r3 = *(u32*)((u8*)r26 + 0x14);
            tmp = r29 << 28;
            /* and. tmp, r3, tmp */;
            if (r31 == 0) break;
            r26 = *(u32*)((u8*)r26 + 0x10);
            while (r26 != 0) {

                r3 = r26;
                r4 = r28;
                r5 = r29;
                r6 = r30;
                fn_801A13CC();
                r26 = *(u32*)((u8*)r26 + 0x8);

            }
        } while (0);
            r31 = *(u32*)((u8*)r31 + 0x8);

        }
    } while (0);
        r27 = *(u32*)((u8*)r27 + 0x8);

    }

    return;
}
