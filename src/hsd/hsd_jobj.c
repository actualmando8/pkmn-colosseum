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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    /* stmw r27, 0xac(r1) */;
    r31 = (u32)lbl_80274818;
    DVDInit();
    r5 = *(u32*)lbl_80478C74;
    r29 = *(u32*)lbl_80478C80;
    if ((u32)r5 == (u32)0x0) goto L_8019CC3C;
    r4 = *(u16*)((u8*)r5 + 0x4);
    r3 = (u32)lbl_80465568;
    r0 = *(u16*)((u8*)r5 + 0x8);
    r3 = (u32)lbl_80465568;
    r4 = r4 + 0xf;
    r30 = 0x0;
    r4 = r4 & 0x0000FFF0;
    r0 = r4 * r0;
    r27 = r0 << 1;
    r0 = r27 * r29;
    *(u32*)((u8*)r3 + 0x8) = r0;
    goto L_8019CC08;
L_8019CBCC: ;
    r3 = r27;
    r4 = 0x20;
    fn_8009AFD0();
    r4 = (u32)lbl_8046557C;
    r5 = r30 << 2;
    r0 = (u32)lbl_8046557C;
    r4 = r0 + r5;
    *(u32*)((u8*)r4 + 0x0) = r3;
    if ((u32)r3 != (u32)0x0) goto L_8019CC04;
    r3 = r31 + 0x1cc;
    r5 = r31 + 0x260;
    r4 = 0xf1;
    fn_80196D78();
L_8019CC04: ;
    r30 = r30 + 0x1;
L_8019CC08: ;
    if ((s32)r30 < (s32)r29) goto L_8019CBCC;
    r30 = r29;
    goto L_8019CC34;
L_8019CC18: ;
    r3 = (u32)lbl_8046557C;
    r4 = r30 << 2;
    r0 = (u32)lbl_8046557C;
    r5 = 0x0;
    r3 = r0 + r4;
    r30 = r30 + 0x1;
    *(u32*)((u8*)r3 + 0x0) = r5;
L_8019CC34: ;
    if ((s32)r30 < (s32)0x3) goto L_8019CC18;
L_8019CC3C: ;
    r3 = *(u32*)lbl_80478C7C;
    r4 = 0x20;
    fn_8009AFD0();
    /* mr. r27, r3 */;
    if ((s32)r30 != (s32)0x3) goto L_8019CC60;
    r3 = r31 + 0x1cc;
    r5 = r31 + 0x240;
    r4 = 0x104;
    fn_80196D78();
L_8019CC60: ;
    r4 = *(u32*)lbl_80478C7C;
    r3 = r27;
    GXInit();
    r4 = (u32)lbl_80465568;
    r0 = *(u32*)lbl_80478C7C;
    r4 = (u32)lbl_80465568;
    *(u32*)lbl_8047B278 = r3;
    *(u32*)((u8*)r4 + 0xC) = r0;
    fn_8019C978();
    r7 = *(u32*)lbl_80478C74;
    r11 = (0x100 << 16);
    r6 = *(u32*)lbl_8047E728;
    r5 = (u32)lbl_8046557C;
    r9 = *(u32*)((u8*)r7 + 0x0);
    r4 = (u32)lbl_8046557C;
    r0 = *(u32*)((u8*)r7 + 0x4);
    r3 = (u32)lbl_8046557C;
    r8 = (u32)lbl_8046557C;
    r5 = (u32)lbl_8046557C;
    r6 = (u32)lbl_8046557C;
    /* subi r11, r11, 0x1 */;
    r12 = *(u32*)(sp + 0xC);
    r31 = 0x1;
    *(u32*)(sp + 0x54) = r0;
    r30 = 0x1;
    r29 = 0x0;
    r10 = 0x1;
    r27 = *(u32*)((u8*)r7 + 0x8);
    r9 = 0x1;
    r28 = *(u32*)((u8*)r7 + 0xC);
    r0 = 0x1;
    r4 = *(u32*)((u8*)r8 + 0x0);
    r3 = r1 + 0x50;
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
    *(u8*)(sp + 0xA2) = r0;
    fn_801BF1F0();
    f1 = *(f32*)lbl_8047DB14;
    r3 = r1 + 0x10;
    f2 = *(f32*)lbl_8047DB18;
    f3 = *(f32*)lbl_8047DB18;
    fn_800BA414();
    f1 = *(f32*)lbl_8047DB14;
    r3 = r1 + 0x10;
    f2 = *(f32*)lbl_8047DB18;
    f3 = *(f32*)lbl_8047DB18;
    fn_800BA424();
    f1 = *(f32*)lbl_8047DB14;
    r3 = r1 + 0x10;
    f2 = *(f32*)lbl_8047DB18;
    f3 = *(f32*)lbl_8047DB18;
    f4 = *(f32*)lbl_8047DB14;
    f5 = *(f32*)lbl_8047DB18;
    f6 = *(f32*)lbl_8047DB18;
    fn_800BA198();
    r0 = *(u32*)lbl_80478C84;
    r4 = r1 + 0x8;
    r3 = r1 + 0x10;
    *(u32*)(sp + 0x8) = r0;
    fn_800BA440();
    r27 = 0x0;
    goto L_8019CDF4;
L_8019CDDC: ;
    r3 = r27;
    fn_801A4A54();
    r4 = r3;
    r3 = r1 + 0x10;
    fn_800BA44C();
    r27 = r27 + 0x1;
L_8019CDF4: ;
    if ((s32)r27 < (s32)0x8) goto L_8019CDDC;
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
    r0 = 0x1;
    *(u32*)lbl_8047B280 = r0;
    /* lmw r27, 0xac(r1) */;
    return;
}
#pragma pop

/* 0x8019CE50 | 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    /* stmw r25, 0x14(r1) */;
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
    r0 = (u32)fn_801A1098;
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
    *(u32*)((u8*)r3 + 0x3C) = r0;
    *(u32*)((u8*)r26 + 0x4C) = r25;
    *(u32*)((u8*)r28 + 0x50) = r27;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x68 | fn_8019CF54 | framed_no_calls */
void fn_8019CF54(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047B298, lbl_8047B29C, lbl_8047B2AC */
}

/* 0x8019CFBC | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019CFBC(void) {
    extern u8 lbl_8036C8E0[];
    extern void fn_8019C128();
    extern void fn_8019C1B0();
    extern void fn_801A84F0();
    extern void fn_801A8570();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    if ((u32)r3 != (u32)r31) goto L_8019D004;
    r4 = *(u32*)((u8*)r31 + 0x84);
    r3 = 0x0;
    fn_8019C1B0();
L_8019D004: ;
    r0 = *(u32*)((u8*)r31 + 0x74);
    if ((u32)r0 == (u32)0x0) goto L_8019D018;
    r3 = *(u32*)((u8*)r31 + 0x74);
    fn_801A8570();
L_8019D018: ;
    r0 = *(u32*)((u8*)r31 + 0x78);
    if ((u32)r0 == (u32)0x0) goto L_8019D02C;
    r3 = *(u32*)((u8*)r31 + 0x78);
    fn_801A84F0();
L_8019D02C: ;
    r4 = (u32)lbl_8036C8E0;
    r3 = r31;
    r4 = (u32)lbl_8036C8E0;
    r4 = *(u32*)((u8*)r4 + 0x14);
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x8019D05C | 0x544 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r28 = r3;
    r3 = (u32)lbl_80274AA0;
    r0 = *(u32*)((u8*)r28 + 0x10);
    r31 = (u32)lbl_80274AA0;
    if ((u32)r0 == (u32)0x0) goto L_8019D3DC;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r0 == (u32)0x0) goto L_8019D1D0;
    r30 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r30 == (u32)0x0) goto L_8019D3D4;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r30 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_8019D0CC;
    goto L_8019D0E4;
L_8019D0CC: ;
    r0 = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_8019D0E4: ;
    if ((s32)r3 == (s32)0x0) goto L_8019D3D4;
    r3 = *(u16*)((u8*)r30 + 0x6);
    /* subic. r0, r3, 0x1 */;
    if ((s32)r3 >= (s32)0x0) goto L_8019D12C;
    if ((u32)r30 == (u32)0x0) goto L_8019D3D4;
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
    goto L_8019D3D4;
L_8019D12C: ;
    r3 = *(u16*)((u8*)r30 + 0x6);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x6) = r0;
    r0 = *(u16*)((u8*)r30 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_8019D154;
    r3 = r31 + 0x54;
    r5 = r31 + 0x60;
    r4 = 0x9e;
    fn_80196E10();
L_8019D154: ;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u16*)((u8*)r30 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_8019D17C;
    goto L_8019D194;
L_8019D17C: ;
    r3 = *(u16*)((u8*)r30 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x6) = r0;
    r0 = *(u16*)((u8*)r30 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_8019D194: ;
    if ((s32)r0 == (s32)0x0) goto L_8019D3D4;
    if ((u32)r30 == (u32)0x0) goto L_8019D3D4;
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
    goto L_8019D3D4;
L_8019D1D0: ;
    r3 = *(u32*)((u8*)r28 + 0x10);
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0xC) = r0;
    r29 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r29 == (u32)0x0) goto L_8019D3D4;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_8019D3CC;
    if ((u32)r29 == (u32)0x0) goto L_8019D208;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_8019D210;
L_8019D208: ;
    r3 = 0x0;
    goto L_8019D264;
L_8019D210: ;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r29 != (u32)r0) goto L_8019D228;
    r3 = 0x0;
    goto L_8019D264;
L_8019D228: ;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r3 = *(u32*)((u8*)r3 + 0x10);
    goto L_8019D248;
L_8019D234: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 != (u32)r29) goto L_8019D244;
    goto L_8019D264;
L_8019D244: ;
    r3 = *(u32*)((u8*)r3 + 0x8);
L_8019D248: ;
    if ((u32)r3 != (u32)0x0) goto L_8019D234;
    r5 = r31 + 0x88;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x5f8;
    fn_80196D78();
    r3 = 0x0;
L_8019D264: ;
    if ((u32)r3 == (u32)0x0) goto L_8019D278;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x8) = r0;
    goto L_8019D3CC;
L_8019D278: ;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x10) = r0;
    goto L_8019D3CC;
L_8019D288: ;
    r30 = *(u32*)((u8*)r29 + 0x8);
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0xC) = r3;
    *(u32*)((u8*)r29 + 0x8) = r0;
    if ((u32)r29 == (u32)0x0) goto L_8019D3C8;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r29 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r29 == (u32)0x0) goto L_8019D2C8;
    goto L_8019D2E0;
L_8019D2C8: ;
    r0 = *(u16*)((u8*)r29 + 0x4);
    r3 = *(u16*)((u8*)r29 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r29 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_8019D2E0: ;
    if ((s32)r3 == (s32)0x0) goto L_8019D3C8;
    r3 = *(u16*)((u8*)r29 + 0x6);
    /* subic. r0, r3, 0x1 */;
    if ((s32)r3 >= (s32)0x0) goto L_8019D328;
    if ((u32)r29 == (u32)0x0) goto L_8019D3C8;
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
    goto L_8019D3C8;
L_8019D328: ;
    r3 = *(u16*)((u8*)r29 + 0x6);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r29 + 0x6) = r0;
    r0 = *(u16*)((u8*)r29 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_8019D350;
    r3 = r31 + 0x54;
    r5 = r31 + 0x60;
    r4 = 0x9e;
    fn_80196E10();
L_8019D350: ;
    r4 = *(u32*)((u8*)r29 + 0x0);
    r3 = r29;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u16*)((u8*)r29 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_8019D378;
    goto L_8019D390;
L_8019D378: ;
    r3 = *(u16*)((u8*)r29 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r29 + 0x6) = r0;
    r0 = *(u16*)((u8*)r29 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_8019D390: ;
    if ((s32)r0 == (s32)0x0) goto L_8019D3C8;
    if ((u32)r29 == (u32)0x0) goto L_8019D3C8;
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
L_8019D3C8: ;
    r29 = r30;
L_8019D3CC: ;
    if ((u32)r29 != (u32)0x0) goto L_8019D288;
L_8019D3D4: ;
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x10) = r0;
L_8019D3DC: ;
    r0 = *(u32*)((u8*)r28 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_8019D518;
    if ((u32)r28 == (u32)0x0) goto L_8019D518;
    r0 = *(u32*)((u8*)r28 + 0xC);
    r29 = *(u32*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_8019D510;
    r3 = *(u32*)((u8*)r28 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r0 != (u32)r28) goto L_8019D41C;
    r3 = *(u32*)((u8*)r28 + 0xC);
    *(u32*)((u8*)r3 + 0x10) = r29;
    goto L_8019D4A8;
L_8019D41C: ;
    if ((u32)r28 == (u32)0x0) goto L_8019D430;
    r0 = *(u32*)((u8*)r28 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_8019D438;
L_8019D430: ;
    r30 = 0x0;
    goto L_8019D48C;
L_8019D438: ;
    r3 = *(u32*)((u8*)r28 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r28 != (u32)r0) goto L_8019D450;
    r30 = 0x0;
    goto L_8019D48C;
L_8019D450: ;
    r3 = *(u32*)((u8*)r28 + 0xC);
    r30 = *(u32*)((u8*)r3 + 0x10);
    goto L_8019D470;
L_8019D45C: ;
    r0 = *(u32*)((u8*)r30 + 0x8);
    if ((u32)r0 != (u32)r28) goto L_8019D46C;
    goto L_8019D48C;
L_8019D46C: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019D470: ;
    if ((u32)r30 != (u32)0x0) goto L_8019D45C;
    r5 = r31 + 0x88;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x5f8;
    fn_80196D78();
    r30 = 0x0;
L_8019D48C: ;
    if ((u32)r30 != (u32)0x0) goto L_8019D4A4;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x57b;
    r5 = (u32)lbl_8047DB28;
    fn_80196E10();
L_8019D4A4: ;
    *(u32*)((u8*)r30 + 0x8) = r29;
L_8019D4A8: ;
    r6 = *(u32*)((u8*)r28 + 0xC);
    goto L_8019D500;
L_8019D4B0: ;
    r4 = (0x9000 << 16);
    r5 = *(u32*)((u8*)r6 + 0x10);
    /* subi r4, r4, 0x1 */;
    goto L_8019D4DC;
L_8019D4C0: ;
    r0 = *(u32*)((u8*)r5 + 0x14);
    r3 = *(u32*)((u8*)r5 + 0x14);
    r0 = r0 << 10;
    r5 = *(u32*)((u8*)r5 + 0x8);
    r0 = r3 | r0;
    r0 = r0 & 0x70000000;
    r4 = r4 | r0;
L_8019D4DC: ;
    if ((u32)r5 != (u32)0x0) goto L_8019D4C0;
    r0 = *(u32*)((u8*)r6 + 0x14);
    /* andc. r0, r0, r4 */;
    if ((u32)r5 == (u32)0x0) goto L_8019D508;
    r0 = *(u32*)((u8*)r6 + 0x14);
    r0 = r0 & r4;
    *(u32*)((u8*)r6 + 0x14) = r0;
    r6 = *(u32*)((u8*)r6 + 0x8);
L_8019D500: ;
    if ((u32)r6 != (u32)0x0) goto L_8019D4B0;
L_8019D508: ;
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0xC) = r0;
L_8019D510: ;
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x8) = r0;
L_8019D518: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r6 == (u32)0x0) goto L_8019D548;
    r0 = *(u32*)((u8*)r28 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_8019D548;
    r3 = *(u32*)((u8*)r28 + 0x18);
    fn_80199264();
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x18) = r0;
L_8019D548: ;
    r0 = *(u32*)((u8*)r28 + 0x80);
    if ((u32)r0 == (u32)0x0) goto L_8019D564;
    r3 = *(u32*)((u8*)r28 + 0x80);
    fn_801AE50C();
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x80) = r0;
L_8019D564: ;
    r0 = *(u32*)((u8*)r28 + 0x7C);
    if ((u32)r0 == (u32)0x0) goto L_8019D580;
    r3 = *(u32*)((u8*)r28 + 0x7C);
    fn_801C25E4();
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x7C) = r0;
L_8019D580: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x70 | fn_8019D5A0 | framed_no_calls */
u32 fn_8019D5A0(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047DB30 */
    return 0;
}

/* 0x8019D620 | 0x360 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019D620(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
    extern void fn_8019D980();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    /* stmw r25, 0x14(r1) */;
    r26 = r3;
    r0 = *(u32*)((u8*)r3 + 0x14);
    r0 = r0 | 0x40;
    *(u32*)((u8*)r3 + 0x14) = r0;
    r0 = *(u32*)((u8*)r3 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r0 != (s32)0) goto L_8019D96C;
    r26 = *(u32*)((u8*)r26 + 0x10);
    goto L_8019D964;
L_8019D654: ;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x01000000;
    if ((s32)r0 != (s32)0) goto L_8019D960;
    if ((u32)r26 != (u32)0x0) goto L_8019D678;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019D678: ;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r26 != (u32)0x0) goto L_8019D698;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r26 == (u32)0x0) goto L_8019D698;
    r3 = 0x1;
L_8019D698: ;
    if ((s32)r3 != (s32)0x0) goto L_8019D960;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r31 = r26;
    r0 = r0 | 0x40;
    *(u32*)((u8*)r26 + 0x14) = r0;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019D960;
    r31 = *(u32*)((u8*)r31 + 0x10);
    goto L_8019D958;
L_8019D6C4: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x01000000;
    if ((s32)r3 != (s32)0x0) goto L_8019D954;
    if ((u32)r31 != (u32)0x0) goto L_8019D6E8;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019D6E8: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r31 != (u32)0x0) goto L_8019D708;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r31 == (u32)0x0) goto L_8019D708;
    r3 = 0x1;
L_8019D708: ;
    if ((s32)r3 != (s32)0x0) goto L_8019D954;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r30 = r31;
    r0 = r0 | 0x40;
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019D954;
    r30 = *(u32*)((u8*)r30 + 0x10);
    goto L_8019D94C;
L_8019D734: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x01000000;
    if ((s32)r3 != (s32)0x0) goto L_8019D948;
    if ((u32)r30 != (u32)0x0) goto L_8019D758;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019D758: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r30 != (u32)0x0) goto L_8019D778;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r30 == (u32)0x0) goto L_8019D778;
    r3 = 0x1;
L_8019D778: ;
    if ((s32)r3 != (s32)0x0) goto L_8019D948;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r29 = r30;
    r0 = r0 | 0x40;
    *(u32*)((u8*)r30 + 0x14) = r0;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019D948;
    r29 = *(u32*)((u8*)r29 + 0x10);
    goto L_8019D940;
L_8019D7A4: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x01000000;
    if ((s32)r3 != (s32)0x0) goto L_8019D93C;
    if ((u32)r29 != (u32)0x0) goto L_8019D7C8;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019D7C8: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r29 != (u32)0x0) goto L_8019D7E8;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r29 == (u32)0x0) goto L_8019D7E8;
    r3 = 0x1;
L_8019D7E8: ;
    if ((s32)r3 != (s32)0x0) goto L_8019D93C;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r28 = r29;
    r0 = r0 | 0x40;
    *(u32*)((u8*)r29 + 0x14) = r0;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019D93C;
    r28 = *(u32*)((u8*)r28 + 0x10);
    goto L_8019D934;
L_8019D814: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x01000000;
    if ((s32)r3 != (s32)0x0) goto L_8019D930;
    if ((u32)r28 != (u32)0x0) goto L_8019D838;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019D838: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r28 != (u32)0x0) goto L_8019D858;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r28 == (u32)0x0) goto L_8019D858;
    r3 = 0x1;
L_8019D858: ;
    if ((s32)r3 != (s32)0x0) goto L_8019D930;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r27 = r28;
    r0 = r0 | 0x40;
    *(u32*)((u8*)r28 + 0x14) = r0;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019D930;
    r27 = *(u32*)((u8*)r27 + 0x10);
    goto L_8019D928;
L_8019D884: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x01000000;
    if ((s32)r3 != (s32)0x0) goto L_8019D924;
    if ((u32)r27 != (u32)0x0) goto L_8019D8A8;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019D8A8: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r27 != (u32)0x0) goto L_8019D8C8;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r27 == (u32)0x0) goto L_8019D8C8;
    r3 = 0x1;
L_8019D8C8: ;
    if ((s32)r3 != (s32)0x0) goto L_8019D924;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r25 = r27;
    r0 = r0 | 0x40;
    *(u32*)((u8*)r27 + 0x14) = r0;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019D924;
    r25 = *(u32*)((u8*)r25 + 0x10);
    goto L_8019D91C;
L_8019D8F4: ;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & 0x01000000;
    if ((s32)r3 != (s32)0x0) goto L_8019D918;
    r3 = r25;
    fn_8019D980();
    if ((s32)r3 != (s32)0x0) goto L_8019D918;
    r3 = r25;
    fn_8019D620();
L_8019D918: ;
    r25 = *(u32*)((u8*)r25 + 0x8);
L_8019D91C: ;
    if ((u32)r25 != (u32)0x0) goto L_8019D8F4;
L_8019D924: ;
    r27 = *(u32*)((u8*)r27 + 0x8);
L_8019D928: ;
    if ((u32)r27 != (u32)0x0) goto L_8019D884;
L_8019D930: ;
    r28 = *(u32*)((u8*)r28 + 0x8);
L_8019D934: ;
    if ((u32)r28 != (u32)0x0) goto L_8019D814;
L_8019D93C: ;
    r29 = *(u32*)((u8*)r29 + 0x8);
L_8019D940: ;
    if ((u32)r29 != (u32)0x0) goto L_8019D7A4;
L_8019D948: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019D94C: ;
    if ((u32)r30 != (u32)0x0) goto L_8019D734;
L_8019D954: ;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_8019D958: ;
    if ((u32)r31 != (u32)0x0) goto L_8019D6C4;
L_8019D960: ;
    r26 = *(u32*)((u8*)r26 + 0x8);
L_8019D964: ;
    if ((u32)r26 != (u32)0x0) goto L_8019D654;
L_8019D96C: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x5C | fn_8019D980 | generic */
u32 fn_8019D980(void) {
    fn_80196E10();
    return 1;
}

/* 0x8019D9DC | 0x324 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    *(f64*)(sp + 0x40) = f31;
    /* psq_st f31, 0x48(r1), 0, qr0 */;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r31 = r3;
    r12 = *(u32*)((u8*)r4 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0xFFFFFFBF;
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00800000;
    if ((s32)r0 != (s32)0) goto L_8019DCDC;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = (0x40 << 16);
    r3 = r3 & 0x00600000;
    if ((s32)r3 == (s32)r0) goto L_8019DA6C;
    if ((s32)r3 >= (s32)r0) goto L_8019DA50;
    r0 = (0x20 << 16);
    if ((s32)r3 == (s32)r0) goto L_8019DA60;
    goto L_8019DC3C;
L_8019DA50: ;
    r0 = (0x60 << 16);
    if ((s32)r3 == (s32)r0) goto L_8019DA78;
    goto L_8019DC3C;
L_8019DA60: ;
    r3 = r31;
    fn_8019E460();
    goto L_8019DCD0;
L_8019DA6C: ;
    r3 = r31;
    fn_8019DD00();
    goto L_8019DCD0;
L_8019DA78: ;
    r29 = *(u32*)((u8*)r31 + 0xC);
    f31 = *(f32*)lbl_8047DB30;
    if ((u32)r29 == (u32)0x0) goto L_8019DCD0;
    r3 = *(u32*)((u8*)r29 + 0x80);
    r4 = (0x4000 << 16);
    r5 = 0x0;
    fn_801B00E0();
    /* mr. r30, r3 */;
    if ((u32)r29 == (u32)0x0) goto L_8019DCD0;
    f0 = *(f32*)((u8*)r29 + 0x50);
    r3 = r1 + 0xc;
    r4 = r1 + 0xc;
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
    if (f1 <= f0) goto L_8019DB50;
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
    goto L_8019DBDC;
L_8019DB50: ;
    f0 = *(f64*)lbl_8047DB60;
    if (f1 >= f0) goto L_8019DB68;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_8019DBDC;
L_8019DB68: ;
    *(f32*)(sp + 0x8) = f1;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0x8);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019DB90;
    if ((s32)r3 >= (s32)r0) goto L_8019DBC8;
    if ((s32)r3 == (s32)0x0) goto L_8019DBAC;
    goto L_8019DBC8;
L_8019DB90: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019DBA4;
    r0 = 0x1;
    goto L_8019DBCC;
L_8019DBA4: ;
    r0 = 0x2;
    goto L_8019DBCC;
L_8019DBAC: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019DBC0;
    r0 = 0x5;
    goto L_8019DBCC;
L_8019DBC0: ;
    r0 = 0x3;
    goto L_8019DBCC;
L_8019DBC8: ;
    r0 = 0x4;
L_8019DBCC: ;
    if ((s32)r0 != (s32)0x1) goto L_8019DBDC;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
L_8019DBDC: ;
    r3 = r1 + 0xc;
    r4 = r1 + 0xc;
    fn_800A3AC0();
    r0 = *(u32*)((u8*)r29 + 0x74);
    if ((u32)r0 == (u32)0x0) goto L_8019DBFC;
    r3 = *(u32*)((u8*)r29 + 0x74);
    f31 = *(f32*)((u8*)r3 + 0x0);
L_8019DBFC: ;
    f0 = *(f32*)((u8*)r30 + 0x8);
    r3 = r1 + 0xc;
    r4 = r1 + 0xc;
    f1 = f0 * f31;
    fn_800A3AC0();
    r3 = r1 + 0x18;
    r4 = r1 + 0xc;
    r5 = r1 + 0x24;
    fn_800A3A78();
    f0 = *(f32*)(sp + 0x24);
    *(f32*)((u8*)r31 + 0x50) = f0;
    f0 = *(f32*)(sp + 0x28);
    *(f32*)((u8*)r31 + 0x60) = f0;
    f0 = *(f32*)(sp + 0x2C);
    *(f32*)((u8*)r31 + 0x70) = f0;
    goto L_8019DCD0;
L_8019DC3C: ;
    r0 = *(u32*)((u8*)r31 + 0x80);
    if ((u32)r0 == (u32)0x0) goto L_8019DCD0;
    if ((u32)r31 == (u32)0x0) goto L_8019DCD0;
    r0 = *(u32*)((u8*)r31 + 0x80);
    if ((u32)r0 == (u32)0x0) goto L_8019DCD0;
    r5 = *(u32*)((u8*)r31 + 0x0);
    r4 = r31;
    r3 = *(u32*)((u8*)r31 + 0x80);
    r5 = *(u32*)((u8*)r5 + 0x50);
    fn_801AED88();
    if ((u32)r31 != (u32)0x0) goto L_8019DC88;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019DC88: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r31 != (u32)0x0) goto L_8019DCA8;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r31 == (u32)0x0) goto L_8019DCA8;
    r3 = 0x1;
L_8019DCA8: ;
    if ((s32)r3 == (s32)0x0) goto L_8019DCD0;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0xFFFFFFBF;
    *(u32*)((u8*)r31 + 0x14) = r0;
L_8019DCD0: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0xFFFFFFBF;
    *(u32*)((u8*)r31 + 0x14) = r0;
L_8019DCDC: ;
    /* psq_l f31, 0x48(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x40);
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    r29 = *(u32*)(sp + 0x34);
    return;
}
#pragma pop

/* 0x8019DD00 | 0x760 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    *(f64*)(sp + 0xC0) = f31;
    /* psq_st f31, 0xc8(r1), 0, qr0 */;
    /* stmw r27, 0xac(r1) */;
    r4 = (u32)lbl_80274AC4;
    r30 = r3;
    r5 = (u32)lbl_80274AC4;
    f31 = *(f32*)lbl_8047DB30;
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    r31 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)(sp + 0x70) = r0;
    goto L_8019DD64;
L_8019DD48: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = r0 & 0x00600000;
    /* subis r0, r3, 0x60 */;
    if ((u32)r0 != (u32)0x0) goto L_8019DD60;
    goto L_8019DD70;
L_8019DD60: ;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_8019DD64: ;
    if ((u32)r31 != (u32)0x0) goto L_8019DD48;
    r31 = 0x0;
L_8019DD70: ;
    if ((u32)r31 != (u32)0x0) goto L_8019DD88;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x82d;
    r5 = (u32)lbl_8047DB68;
    fn_80196E10();
L_8019DD88: ;
    r3 = *(u32*)((u8*)r31 + 0x80);
    r4 = (0x1000 << 16);
    r5 = 0x1;
    fn_801B00E0();
    if ((u32)r3 == (u32)0x0) goto L_8019DDA4;
    goto L_8019DDA8;
L_8019DDA4: ;
    r31 = 0x0;
L_8019DDA8: ;
    if ((u32)r31 == (u32)0x0) goto L_8019E444;
    r0 = *(u32*)((u8*)r30 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_8019DDC0;
    goto L_8019E444;
L_8019DDC0: ;
    r0 = *(u32*)((u8*)r30 + 0x74);
    if ((u32)r0 == (u32)0x0) goto L_8019DDE8;
    r5 = *(u32*)((u8*)r30 + 0x74);
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x70) = r0;
L_8019DDE8: ;
    r5 = *(u32*)((u8*)r30 + 0xC);
    r3 = r1 + 0x44;
    r4 = r1 + 0x44;
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
    if (f8 <= f0) goto L_8019DEA4;
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
    goto L_8019DF30;
L_8019DEA4: ;
    f0 = *(f64*)lbl_8047DB60;
    if (f8 >= f0) goto L_8019DEBC;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
    goto L_8019DF30;
L_8019DEBC: ;
    *(f32*)(sp + 0x10) = f8;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0x10);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019DEE4;
    if ((s32)r3 >= (s32)r0) goto L_8019DF1C;
    if ((s32)r3 == (s32)0x0) goto L_8019DF00;
    goto L_8019DF1C;
L_8019DEE4: ;
    r0 = *(u32*)(sp + 0x10);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019DEF8;
    r0 = 0x1;
    goto L_8019DF20;
L_8019DEF8: ;
    r0 = 0x2;
    goto L_8019DF20;
L_8019DF00: ;
    r0 = *(u32*)(sp + 0x10);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019DF14;
    r0 = 0x5;
    goto L_8019DF20;
L_8019DF14: ;
    r0 = 0x3;
    goto L_8019DF20;
L_8019DF1C: ;
    r0 = 0x4;
L_8019DF20: ;
    if ((s32)r0 != (s32)0x1) goto L_8019DF30;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
L_8019DF30: ;
    f1 = f8;
    r3 = r1 + 0x44;
    r4 = r1 + 0x44;
    fn_800A3AC0();
    r3 = *(u32*)((u8*)r30 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x74);
    if ((u32)r0 == (u32)0x0) goto L_8019DF5C;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r3 = *(u32*)((u8*)r3 + 0x74);
    f31 = *(f32*)((u8*)r3 + 0x0);
L_8019DF5C: ;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = (0x4000 << 16);
    r5 = 0x0;
    r3 = *(u32*)((u8*)r3 + 0x80);
    fn_801B00E0();
    /* mr. r29, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_8019DF88;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x905;
    r5 = (u32)lbl_8047DB6C;
    fn_80196E10();
L_8019DF88: ;
    f0 = *(f32*)((u8*)r29 + 0x8);
    r3 = r1 + 0x44;
    r4 = r1 + 0x44;
    f1 = f0 * f31;
    fn_800A3AC0();
    r3 = r1 + 0x50;
    r4 = r1 + 0x44;
    r5 = r1 + 0x5c;
    fn_800A3A78();
    r3 = r31 + 0x38;
    r4 = r1 + 0x5c;
    r5 = r1 + 0x44;
    fn_800A3A9C();
    r3 = r1 + 0x44;
    r4 = r1 + 0x44;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f1 = f2 / f1;
    if (f1 <= f0) goto L_8019E03C;
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
    goto L_8019E0C8;
L_8019E03C: ;
    f0 = *(f64*)lbl_8047DB60;
    if (f1 >= f0) goto L_8019E054;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_8019E0C8;
L_8019E054: ;
    *(f32*)(sp + 0xC) = f1;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0xC);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019E07C;
    if ((s32)r3 >= (s32)r0) goto L_8019E0B4;
    if ((s32)r3 == (s32)0x0) goto L_8019E098;
    goto L_8019E0B4;
L_8019E07C: ;
    r0 = *(u32*)(sp + 0xC);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019E090;
    r0 = 0x1;
    goto L_8019E0B8;
L_8019E090: ;
    r0 = 0x2;
    goto L_8019E0B8;
L_8019E098: ;
    r0 = *(u32*)(sp + 0xC);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019E0AC;
    r0 = 0x5;
    goto L_8019E0B8;
L_8019E0AC: ;
    r0 = 0x3;
    goto L_8019E0B8;
L_8019E0B4: ;
    r0 = 0x4;
L_8019E0B8: ;
    if ((s32)r0 != (s32)0x1) goto L_8019E0C8;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
L_8019E0C8: ;
    r3 = r1 + 0x44;
    r4 = r1 + 0x44;
    fn_800A3AC0();
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = (0x2000 << 16);
    r5 = 0x5;
    fn_801B00E0();
    r0 = r3;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r31 = r0;
    r4 = (0x2000 << 16);
    r5 = 0x6;
    fn_801B00E0();
    r29 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8019E110;
    if ((u32)r29 == (u32)0x0) goto L_8019E240;
L_8019E110: ;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r28 = 0x0;
    r4 = (0x4000 << 16);
    r5 = 0x0;
    fn_801B00E0();
    /* mr. r27, r3 */;
    if ((u32)r29 != (u32)0x0) goto L_8019E13C;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x927;
    r5 = (u32)lbl_8047DB6C;
    fn_80196E10();
L_8019E13C: ;
    r5 = *(u32*)((u8*)r30 + 0xC);
    r3 = r1 + 0x20;
    r0 = *(u32*)((u8*)r27 + 0x4);
    r4 = r1 + 0x20;
    f0 = *(f32*)((u8*)r5 + 0x44);
    /* extrwi r27, r0, 1, 29 */;
    *(f32*)(sp + 0x20) = f0;
    f0 = *(f32*)((u8*)r5 + 0x10);
    *(f32*)(sp + 0x24) = f0;
    f0 = *(f32*)((u8*)r5 + 0x20);
    *(f32*)(sp + 0x28) = f0;
    fn_800A3ADC();
    r3 = r1 + 0x20;
    r4 = r1 + 0x44;
    fn_800A3B7C();
    f0 = *(f32*)lbl_8047DB30;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_8019E190;
    f0 = *(f32*)lbl_8047DB48;
    goto L_8019E1B0;
L_8019E190: ;
    f0 = *(f32*)lbl_8047DB74;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019E1A8;
    f0 = *(f32*)lbl_8047DB78;
    goto L_8019E1B0;
L_8019E1A8: ;
    fn_800CE298();
    f0 = (f32)f1;
L_8019E1B0: ;
    f1 = f0;
    if ((s32)r27 != (s32)0x0) goto L_8019E1C0;
    f1 = -f1;
L_8019E1C0: ;
    if ((u32)r31 == (u32)0x0) goto L_8019E1E0;
    f0 = *(f32*)((u8*)r31 + 0x8);
    if (f1 >= f0) goto L_8019E1E0;
    f1 = *(f32*)((u8*)r31 + 0x8);
    r28 = 0x1;
    goto L_8019E1FC;
L_8019E1E0: ;
    if ((u32)r29 == (u32)0x0) goto L_8019E1FC;
    f0 = *(f32*)((u8*)r29 + 0x8);
    if (f0 >= f1) goto L_8019E1FC;
    f1 = *(f32*)((u8*)r29 + 0x8);
    r28 = 0x1;
L_8019E1FC: ;
    if ((s32)r28 == (s32)0x0) goto L_8019E240;
    r5 = *(u32*)((u8*)r30 + 0xC);
    r3 = r1 + 0x74;
    r4 = r1 + 0x14;
    r5 = r5 + 0x44;
    f0 = *(f32*)((u8*)r5 + 0x8);
    *(f32*)(sp + 0x14) = f0;
    f0 = *(f32*)((u8*)r5 + 0x18);
    *(f32*)(sp + 0x18) = f0;
    f0 = *(f32*)((u8*)r5 + 0x28);
    *(f32*)(sp + 0x1C) = f0;
    fn_800A3244();
    r3 = r1 + 0x74;
    r4 = r1 + 0x20;
    r5 = r1 + 0x44;
    fn_800A37CC();
L_8019E240: ;
    r6 = *(u32*)((u8*)r30 + 0xC);
    r3 = r1 + 0x2c;
    r4 = r1 + 0x44;
    r5 = r1 + 0x38;
    r6 = r6 + 0x44;
    f0 = *(f32*)((u8*)r6 + 0x8);
    *(f32*)(sp + 0x2C) = f0;
    f0 = *(f32*)((u8*)r6 + 0x18);
    *(f32*)(sp + 0x30) = f0;
    f0 = *(f32*)((u8*)r6 + 0x28);
    *(f32*)(sp + 0x34) = f0;
    fn_800A3B9C();
    r3 = r1 + 0x38;
    r4 = r1 + 0x38;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f8 = f2 / f1;
    if (f8 <= f0) goto L_8019E2F0;
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
    goto L_8019E37C;
L_8019E2F0: ;
    f0 = *(f64*)lbl_8047DB60;
    if (f8 >= f0) goto L_8019E308;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
    goto L_8019E37C;
L_8019E308: ;
    *(f32*)(sp + 0x8) = f8;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0x8);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019E330;
    if ((s32)r3 >= (s32)r0) goto L_8019E368;
    if ((s32)r3 == (s32)0x0) goto L_8019E34C;
    goto L_8019E368;
L_8019E330: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019E344;
    r0 = 0x1;
    goto L_8019E36C;
L_8019E344: ;
    r0 = 0x2;
    goto L_8019E36C;
L_8019E34C: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019E360;
    r0 = 0x5;
    goto L_8019E36C;
L_8019E360: ;
    r0 = 0x3;
    goto L_8019E36C;
L_8019E368: ;
    r0 = 0x4;
L_8019E36C: ;
    if ((s32)r0 != (s32)0x1) goto L_8019E37C;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
L_8019E37C: ;
    f1 = f8;
    r3 = r1 + 0x38;
    r4 = r1 + 0x38;
    fn_800A3AC0();
    r3 = r1 + 0x44;
    r4 = r1 + 0x38;
    r5 = r1 + 0x2c;
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
L_8019E444: ;
    /* psq_l f31, 0xc8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0xC0);
    /* lmw r27, 0xac(r1) */;
    return;
}
#pragma pop

/* 0x8019E460 | 0xBBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    *(f64*)(sp + 0x130) = f31;
    /* psq_st f31, 0x138(r1), 0, qr0 */;
    *(f64*)(sp + 0x120) = f30;
    /* psq_st f30, 0x128(r1), 0, qr0 */;
    *(f64*)(sp + 0x110) = f29;
    /* psq_st f29, 0x118(r1), 0, qr0 */;
    *(f64*)(sp + 0x100) = f28;
    /* psq_st f28, 0x108(r1), 0, qr0 */;
    *(f64*)(sp + 0xF0) = f27;
    /* psq_st f27, 0xf8(r1), 0, qr0 */;
    *(f64*)(sp + 0xE0) = f26;
    /* psq_st f26, 0xe8(r1), 0, qr0 */;
    r4 = (u32)lbl_80274AA0;
    r29 = r3;
    r7 = (u32)lbl_80274AA0;
    r5 = *(u32*)((u8*)r3 + 0x10);
    r4 = *(u32*)((u8*)r7 + 0x0);
    r31 = 0x0;
    r3 = *(u32*)((u8*)r7 + 0x4);
    r0 = *(u32*)((u8*)r7 + 0x8);
    f30 = *(f32*)lbl_8047DB48;
    *(u32*)(sp + 0x90) = r0;
    goto L_8019E4FC;
L_8019E4E0: ;
    r0 = *(u32*)((u8*)r5 + 0x14);
    r3 = r0 & 0x00600000;
    /* subis r0, r3, 0x40 */;
    if ((u32)r0 != (u32)0x0) goto L_8019E4F8;
    goto L_8019E508;
L_8019E4F8: ;
    r5 = *(u32*)((u8*)r5 + 0x8);
L_8019E4FC: ;
    if ((u32)r5 != (u32)0x0) goto L_8019E4E0;
    r5 = 0x0;
L_8019E508: ;
    r3 = *(u32*)((u8*)r7 + 0xC);
    r30 = r5;
    r6 = *(u32*)((u8*)r7 + 0x10);
    r0 = *(u32*)((u8*)r7 + 0x14);
    r5 = *(u32*)((u8*)r7 + 0x18);
    r4 = *(u32*)((u8*)r7 + 0x1C);
    r3 = *(u32*)((u8*)r7 + 0x20);
    *(u32*)(sp + 0x84) = r0;
    r0 = *(u32*)((u8*)r29 + 0x74);
    if ((u32)r0 == (u32)0x0) goto L_8019E564;
    r5 = *(u32*)((u8*)r29 + 0x74);
    r4 = *(u32*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r5 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)(sp + 0x90) = r0;
L_8019E564: ;
    r3 = *(u32*)((u8*)r29 + 0x80);
    r4 = (0x4000 << 16);
    r5 = 0x0;
    fn_801B00E0();
    /* mr. r28, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_8019E58C;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x85c;
    r5 = (u32)lbl_8047DB6C;
    fn_80196E10();
L_8019E58C: ;
    f0 = *(f32*)(sp + 0x88);
    f1 = *(f32*)((u8*)r28 + 0x8);
    f26 = *(f32*)((u8*)r28 + 0xC);
    f31 = f1 * f0;
    if ((u32)r30 == (u32)0x0) goto L_8019E654;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = (0x4000 << 16);
    r5 = 0x0;
    fn_801B00E0();
    /* mr. r28, r3 */;
    if ((u32)r30 != (u32)0x0) goto L_8019E5CC;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x867;
    r5 = (u32)lbl_8047DB6C;
    fn_80196E10();
L_8019E5CC: ;
    f0 = *(f32*)((u8*)r30 + 0x2C);
    f1 = *(f32*)((u8*)r28 + 0x8);
    r0 = *(u32*)((u8*)r28 + 0x4);
    f0 = f1 * f0;
    f1 = *(f32*)(sp + 0x88);
    r30 = *(u32*)((u8*)r30 + 0x10);
    /* extrwi r31, r0, 1, 29 */;
    f30 = f1 * f0;
    goto L_8019E60C;
L_8019E5F0: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = r0 & 0x00600000;
    /* subis r0, r3, 0x60 */;
    if ((u32)r0 != (u32)0x0) goto L_8019E608;
    goto L_8019E618;
L_8019E608: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019E60C: ;
    if ((u32)r30 != (u32)0x0) goto L_8019E5F0;
    r30 = 0x0;
L_8019E618: ;
    if ((u32)r30 != (u32)0x0) goto L_8019E630;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x82d;
    r5 = (u32)lbl_8047DB68;
    fn_80196E10();
L_8019E630: ;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = (0x1000 << 16);
    r5 = 0x1;
    fn_801B00E0();
    if ((u32)r3 == (u32)0x0) goto L_8019E64C;
    goto L_8019E6BC;
L_8019E64C: ;
    r30 = 0x0;
    goto L_8019E6BC;
L_8019E654: ;
    r30 = *(u32*)((u8*)r29 + 0x10);
    goto L_8019E678;
L_8019E65C: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = r0 & 0x00600000;
    /* subis r0, r3, 0x60 */;
    if ((u32)r0 != (u32)0x0) goto L_8019E674;
    goto L_8019E684;
L_8019E674: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019E678: ;
    if ((u32)r30 != (u32)0x0) goto L_8019E65C;
    r30 = 0x0;
L_8019E684: ;
    if ((u32)r30 != (u32)0x0) goto L_8019E69C;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x82d;
    r5 = (u32)lbl_8047DB68;
    fn_80196E10();
L_8019E69C: ;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = (0x1000 << 16);
    r5 = 0x1;
    fn_801B00E0();
    if ((u32)r3 == (u32)0x0) goto L_8019E6B8;
    goto L_8019E6BC;
L_8019E6B8: ;
    r30 = 0x0;
L_8019E6BC: ;
    if ((u32)r30 == (u32)0x0) goto L_8019EFCC;
    r3 = *(u32*)((u8*)r29 + 0x80);
    r4 = (0x1000 << 16);
    r5 = 0x3;
    fn_801B00E0();
    if ((u32)r3 != (u32)0x0) goto L_8019E764;
    if ((u32)r29 == (u32)0x0) goto L_8019E764;
    r0 = *(u32*)((u8*)r29 + 0x80);
    if ((u32)r0 == (u32)0x0) goto L_8019E764;
    r5 = *(u32*)((u8*)r29 + 0x0);
    r4 = r29;
    r3 = *(u32*)((u8*)r29 + 0x80);
    r5 = *(u32*)((u8*)r5 + 0x50);
    fn_801AED88();
    if ((u32)r29 != (u32)0x0) goto L_8019E71C;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019E71C: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r29 != (u32)0x0) goto L_8019E73C;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r29 == (u32)0x0) goto L_8019E73C;
    r3 = 0x1;
L_8019E73C: ;
    if ((s32)r3 == (s32)0x0) goto L_8019E764;
    r4 = *(u32*)((u8*)r29 + 0x0);
    r3 = r29;
    r12 = *(u32*)((u8*)r4 + 0x40);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0xFFFFFFBF;
    *(u32*)((u8*)r29 + 0x14) = r0;
L_8019E764: ;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_8019E780;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r4 = r1 + 0x7c;
    r3 = r3 + 0x44;
    fn_801A9570();
L_8019E780: ;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r5 = r30 + 0x38;
    r4 = 0x1;
    fn_801AFCAC();
    r3 = r30 + 0x38;
    r4 = r1 + 0x7c;
    r5 = r1 + 0x58;
    fn_800A3A9C();
    r3 = r1 + 0x58;
    r4 = r1 + 0x58;
    fn_800A3B7C();
    f29 = f1;
    f0 = *(f32*)lbl_8047DB7C;
    if (f29 <= f0) goto L_8019ED98;
    r6 = *(u32*)(sp + 0x58);
    r5 = r1 + 0x28;
    r3 = *(u32*)(sp + 0x5C);
    r4 = 0x3;
    r0 = *(u32*)(sp + 0x60);
    *(u32*)(sp + 0x3C) = r0;
    r3 = *(u32*)((u8*)r29 + 0x80);
    fn_801AFCAC();
    if ((s32)r3 == (s32)0x0) goto L_8019E84C;
    r3 = r1 + 0x28;
    r4 = r1 + 0x7c;
    r5 = r1 + 0x28;
    fn_800A3A9C();
    f0 = *(f32*)lbl_8047DB48;
    if (f0 == f26) goto L_8019E828;
    f1 = f26;
    r3 = r1 + 0x94;
    r4 = r1 + 0x34;
    fn_800A3244();
    r3 = r1 + 0x94;
    r4 = r1 + 0x28;
    r5 = r1 + 0x28;
    fn_800A37CC();
L_8019E828: ;
    r3 = r1 + 0x34;
    r4 = r1 + 0x28;
    r5 = r1 + 0x1c;
    fn_800A3B9C();
    r3 = r1 + 0x1c;
    r4 = r1 + 0x34;
    r5 = r1 + 0x28;
    fn_800A3B9C();
    goto L_8019E884;
L_8019E84C: ;
    f0 = *(f32*)((u8*)r29 + 0x4C);
    r3 = r1 + 0x1c;
    r4 = r1 + 0x34;
    r5 = r1 + 0x28;
    *(f32*)(sp + 0x1C) = f0;
    f0 = *(f32*)((u8*)r29 + 0x5C);
    *(f32*)(sp + 0x20) = f0;
    f0 = *(f32*)((u8*)r29 + 0x6C);
    *(f32*)(sp + 0x24) = f0;
    fn_800A3B9C();
    r3 = r1 + 0x34;
    r4 = r1 + 0x28;
    r5 = r1 + 0x1c;
    fn_800A3B9C();
L_8019E884: ;
    r3 = r1 + 0x1c;
    r4 = r1 + 0x1c;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f8 = f2 / f1;
    if (f8 <= f0) goto L_8019E904;
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
    goto L_8019E990;
L_8019E904: ;
    f0 = *(f64*)lbl_8047DB60;
    if (f8 >= f0) goto L_8019E91C;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
    goto L_8019E990;
L_8019E91C: ;
    *(f32*)(sp + 0x18) = f8;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0x18);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019E944;
    if ((s32)r3 >= (s32)r0) goto L_8019E97C;
    if ((s32)r3 == (s32)0x0) goto L_8019E960;
    goto L_8019E97C;
L_8019E944: ;
    r0 = *(u32*)(sp + 0x18);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019E958;
    r0 = 0x1;
    goto L_8019E980;
L_8019E958: ;
    r0 = 0x2;
    goto L_8019E980;
L_8019E960: ;
    r0 = *(u32*)(sp + 0x18);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019E974;
    r0 = 0x5;
    goto L_8019E980;
L_8019E974: ;
    r0 = 0x3;
    goto L_8019E980;
L_8019E97C: ;
    r0 = 0x4;
L_8019E980: ;
    if ((s32)r0 != (s32)0x1) goto L_8019E990;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
L_8019E990: ;
    f1 = f8;
    r3 = r1 + 0x1c;
    r4 = r1 + 0x4c;
    fn_800A3AC0();
    r3 = r1 + 0x28;
    r4 = r1 + 0x28;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f8 = f2 / f1;
    if (f8 <= f0) goto L_8019EA20;
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
    goto L_8019EAAC;
L_8019EA20: ;
    f0 = *(f64*)lbl_8047DB60;
    if (f8 >= f0) goto L_8019EA38;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
    goto L_8019EAAC;
L_8019EA38: ;
    *(f32*)(sp + 0x14) = f8;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0x14);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019EA60;
    if ((s32)r3 >= (s32)r0) goto L_8019EA98;
    if ((s32)r3 == (s32)0x0) goto L_8019EA7C;
    goto L_8019EA98;
L_8019EA60: ;
    r0 = *(u32*)(sp + 0x14);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019EA74;
    r0 = 0x1;
    goto L_8019EA9C;
L_8019EA74: ;
    r0 = 0x2;
    goto L_8019EA9C;
L_8019EA7C: ;
    r0 = *(u32*)(sp + 0x14);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019EA90;
    r0 = 0x5;
    goto L_8019EA9C;
L_8019EA90: ;
    r0 = 0x3;
    goto L_8019EA9C;
L_8019EA98: ;
    r0 = 0x4;
L_8019EA9C: ;
    if ((s32)r0 != (s32)0x1) goto L_8019EAAC;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
L_8019EAAC: ;
    f1 = f8;
    r3 = r1 + 0x28;
    r4 = r1 + 0x40;
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
    if (f27 >= f0) goto L_8019EAFC;
    f27 = *(f32*)lbl_8047DB48;
L_8019EAFC: ;
    f0 = f6 - f27;
    f2 = *(f32*)lbl_8047DB44;
    f3 = *(f32*)lbl_8047DB30;
    f1 = *(f32*)lbl_8047DB48;
    f0 = f0 / f29;
    f2 = f2 + f0;
    f2 = f3 / f2;
    if (f2 <= f1) goto L_8019EB88;
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
    goto L_8019EC48;
L_8019EB88: ;
    f1 = *(f32*)lbl_8047DB44;
    f3 = *(f32*)lbl_8047DB30;
    f2 = f1 + f0;
    f1 = *(f64*)lbl_8047DB60;
    f2 = f3 / f2;
    if (f2 >= f1) goto L_8019EBB0;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_8019EC48;
L_8019EBB0: ;
    f1 = *(f32*)lbl_8047DB44;
    r0 = (0x7f80 << 16);
    f2 = *(f32*)lbl_8047DB30;
    f1 = f1 + f0;
    f1 = f2 / f1;
    *(f32*)(sp + 0x10) = f1;
    r3 = *(u32*)(sp + 0x10);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019EBE8;
    if ((s32)r3 >= (s32)r0) goto L_8019EC20;
    if ((s32)r3 == (s32)0x0) goto L_8019EC04;
    goto L_8019EC20;
L_8019EBE8: ;
    r0 = *(u32*)(sp + 0x10);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019EBFC;
    r0 = 0x1;
    goto L_8019EC24;
L_8019EBFC: ;
    r0 = 0x2;
    goto L_8019EC24;
L_8019EC04: ;
    r0 = *(u32*)(sp + 0x10);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019EC18;
    r0 = 0x5;
    goto L_8019EC24;
L_8019EC18: ;
    r0 = 0x3;
    goto L_8019EC24;
L_8019EC20: ;
    r0 = 0x4;
L_8019EC24: ;
    if ((s32)r0 != (s32)0x1) goto L_8019EC38;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_8019EC48;
L_8019EC38: ;
    f1 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f1 + f0;
    f1 = f2 / f1;
L_8019EC48: ;
    f2 = *(f32*)lbl_8047DB44;
    f1 = f0 * f1;
    f3 = *(f32*)lbl_8047DB30;
    f2 = f2 + f27;
    f0 = *(f32*)lbl_8047DB48;
    f2 = f3 / f2;
    if (f2 <= f0) goto L_8019ECD0;
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
    goto L_8019ED90;
L_8019ECD0: ;
    f0 = *(f32*)lbl_8047DB44;
    f3 = *(f32*)lbl_8047DB30;
    f2 = f0 + f27;
    f0 = *(f64*)lbl_8047DB60;
    f2 = f3 / f2;
    if (f2 >= f0) goto L_8019ECF8;
    r3 = (u32)lbl_80478AC0;
    f0 = *(f32*)lbl_80478AC0;
    goto L_8019ED90;
L_8019ECF8: ;
    f0 = *(f32*)lbl_8047DB44;
    r0 = (0x7f80 << 16);
    f2 = *(f32*)lbl_8047DB30;
    f0 = f0 + f27;
    f0 = f2 / f0;
    *(f32*)(sp + 0xC) = f0;
    r3 = *(u32*)(sp + 0xC);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019ED30;
    if ((s32)r3 >= (s32)r0) goto L_8019ED68;
    if ((s32)r3 == (s32)0x0) goto L_8019ED4C;
    goto L_8019ED68;
L_8019ED30: ;
    r0 = *(u32*)(sp + 0xC);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019ED44;
    r0 = 0x1;
    goto L_8019ED6C;
L_8019ED44: ;
    r0 = 0x2;
    goto L_8019ED6C;
L_8019ED4C: ;
    r0 = *(u32*)(sp + 0xC);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019ED60;
    r0 = 0x5;
    goto L_8019ED6C;
L_8019ED60: ;
    r0 = 0x3;
    goto L_8019ED6C;
L_8019ED68: ;
    r0 = 0x4;
L_8019ED6C: ;
    if ((s32)r0 != (s32)0x1) goto L_8019ED80;
    r3 = (u32)lbl_80478AC0;
    f0 = *(f32*)lbl_80478AC0;
    goto L_8019ED90;
L_8019ED80: ;
    f0 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f0 = f0 + f27;
    f0 = f2 / f0;
L_8019ED90: ;
    f30 = f27 * f0;
    goto L_8019EDA0;
L_8019ED98: ;
    f1 = *(f32*)lbl_8047DB48;
    f30 = f31;
L_8019EDA0: ;
    if ((s32)r31 == (s32)0x0) goto L_8019EDAC;
    f30 = -f30;
L_8019EDAC: ;
    f0 = f28 - f27;
    if (f0 >= f29) goto L_8019EDC8;
    r3 = r1 + 0x58;
    r4 = r1 + 0x64;
    fn_800A3AC0();
    goto L_8019EDD8;
L_8019EDC8: ;
    f1 = -f1;
    r3 = r1 + 0x58;
    r4 = r1 + 0x64;
    fn_800A3AC0();
L_8019EDD8: ;
    f1 = f30;
    r3 = r1 + 0x40;
    r4 = r1 + 0x28;
    fn_800A3AC0();
    r3 = r1 + 0x64;
    r4 = r1 + 0x28;
    r5 = r1 + 0x64;
    fn_800A3A78();
    r3 = r1 + 0x64;
    r4 = r1 + 0x64;
    fn_800A3B7C();
    f3 = *(f32*)lbl_8047DB44;
    f2 = *(f32*)lbl_8047DB30;
    f1 = f3 + f1;
    f0 = *(f32*)lbl_8047DB48;
    f8 = f2 / f1;
    if (f8 <= f0) goto L_8019EE78;
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
    goto L_8019EF04;
L_8019EE78: ;
    f0 = *(f64*)lbl_8047DB60;
    if (f8 >= f0) goto L_8019EE90;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
    goto L_8019EF04;
L_8019EE90: ;
    *(f32*)(sp + 0x8) = f8;
    r0 = (0x7f80 << 16);
    r3 = *(u32*)(sp + 0x8);
    r3 = r3 & 0x7F800000;
    if ((s32)r3 == (s32)r0) goto L_8019EEB8;
    if ((s32)r3 >= (s32)r0) goto L_8019EEF0;
    if ((s32)r3 == (s32)0x0) goto L_8019EED4;
    goto L_8019EEF0;
L_8019EEB8: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019EECC;
    r0 = 0x1;
    goto L_8019EEF4;
L_8019EECC: ;
    r0 = 0x2;
    goto L_8019EEF4;
L_8019EED4: ;
    r0 = *(u32*)(sp + 0x8);
    r0 = r0 & 0x7FFFFF;
    if ((s32)r3 == (s32)0x0) goto L_8019EEE8;
    r0 = 0x5;
    goto L_8019EEF4;
L_8019EEE8: ;
    r0 = 0x3;
    goto L_8019EEF4;
L_8019EEF0: ;
    r0 = 0x4;
L_8019EEF4: ;
    if ((s32)r0 != (s32)0x1) goto L_8019EF04;
    r3 = (u32)lbl_80478AC0;
    f8 = *(f32*)lbl_80478AC0;
L_8019EF04: ;
    f1 = f8;
    r3 = r1 + 0x64;
    r4 = r1 + 0x64;
    fn_800A3AC0();
    f2 = *(f32*)(sp + 0x64);
    r3 = r1 + 0x4c;
    f0 = *(f32*)(sp + 0x88);
    r4 = r1 + 0x64;
    f1 = *(f32*)(sp + 0x88);
    r5 = r1 + 0x28;
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
L_8019EFCC: ;
    /* psq_l f31, 0x138(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x130);
    /* psq_l f30, 0x128(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x120);
    /* psq_l f29, 0x118(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x110);
    /* psq_l f28, 0x108(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0x100);
    /* psq_l f27, 0xf8(r1), 0, qr0 */;
    f27 = *(f64*)(sp + 0xF0);
    /* psq_l f26, 0xe8(r1), 0, qr0 */;
    f26 = *(f64*)(sp + 0xE0);
    r31 = *(u32*)(sp + 0xDC);
    r30 = *(u32*)(sp + 0xD8);
    r29 = *(u32*)(sp + 0xD4);
    r28 = *(u32*)(sp + 0xD0);
    return;
}
#pragma pop

/* 0x8019F024 | 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019F024(void) {
    extern u8 lbl_80274AA0[];
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

    /* mr. r29, r3 */;
    r3 = (u32)lbl_80274AA0;
    r31 = (u32)lbl_80274AA0;
    if ((s32)r0 == (s32)0) goto L_8019F074;
    r3 = *(u16*)((u8*)r29 + 0x4);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r29 + 0x4) = r0;
    r0 = *(u16*)((u8*)r29 + 0x4);
    if ((u32)r0 != (u32)0xffff) goto L_8019F074;
    r3 = r31 + 0x54;
    r5 = r31 + 0xc4;
    r4 = 0x5d;
    fn_80196E10();
L_8019F074: ;
    r30 = *(u32*)&lbl_8047B2AC;
    if ((u32)r30 == (u32)0x0) goto L_8019F1A4;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r30 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_8019F0A4;
    goto L_8019F0BC;
L_8019F0A4: ;
    r0 = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_8019F0BC: ;
    if ((s32)r3 == (s32)0x0) goto L_8019F1A4;
    r3 = *(u16*)((u8*)r30 + 0x6);
    /* subic. r0, r3, 0x1 */;
    if ((s32)r3 >= (s32)0x0) goto L_8019F104;
    if ((u32)r30 == (u32)0x0) goto L_8019F1A4;
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
    goto L_8019F1A4;
L_8019F104: ;
    r3 = *(u16*)((u8*)r30 + 0x6);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x6) = r0;
    r0 = *(u16*)((u8*)r30 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_8019F12C;
    r3 = r31 + 0x54;
    r5 = r31 + 0x60;
    r4 = 0x9e;
    fn_80196E10();
L_8019F12C: ;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u16*)((u8*)r30 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_8019F154;
    goto L_8019F16C;
L_8019F154: ;
    r3 = *(u16*)((u8*)r30 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x6) = r0;
    r0 = *(u16*)((u8*)r30 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_8019F16C: ;
    if ((s32)r0 == (s32)0x0) goto L_8019F1A4;
    if ((u32)r30 == (u32)0x0) goto L_8019F1A4;
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
L_8019F1A4: ;
    *(u32*)&lbl_8047B2AC = r29;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x8019F1C4 | 0x554 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019F1C4(void) {
    extern void fn_80199178();
    extern void fn_8019F1C4();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r0 = 0x0;
    /* stmw r27, 0x7c(r1) */;
    /* mr. r27, r3 */;
    r3 = 0x0;
    r28 = r4;
    r29 = r5;
    *(u32*)(sp + 0x70) = r0;
    if ((s32)r0 == (s32)0) goto L_8019F6E4;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r0 == (s32)0) goto L_8019F440;
    r30 = *(u32*)((u8*)r27 + 0x10);
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)(sp + 0x5C) = r0;
    if ((u32)r30 == (u32)0x0) goto L_8019F424;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r30 == (u32)0x0) goto L_8019F2F4;
    r30 = *(u32*)((u8*)r30 + 0x10);
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)(sp + 0x3C) = r0;
    if ((u32)r30 == (u32)0x0) goto L_8019F2D8;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r30 == (u32)0x0) goto L_8019F264;
    r3 = *(u32*)((u8*)r30 + 0x10);
    r4 = r1 + 0x38;
    r5 = r1 + 0x3c;
    fn_8019F1C4();
    goto L_8019F2D8;
L_8019F264: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r30 != (u32)0x0) goto L_8019F294;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_8019F294;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r1 + 0x38;
    r5 = r1 + 0x3c;
    fn_80199178();
L_8019F294: ;
    r30 = *(u32*)((u8*)r30 + 0x10);
    goto L_8019F2D0;
L_8019F29C: ;
    r3 = r30;
    r4 = r1 + 0x40;
    r5 = r1 + 0x44;
    fn_8019F1C4();
    r5 = *(u32*)(sp + 0x38);
    r4 = *(u32*)(sp + 0x40);
    r3 = *(u32*)(sp + 0x3C);
    r0 = *(u32*)(sp + 0x44);
    r4 = r5 + r4;
    r0 = r3 + r0;
    *(u32*)(sp + 0x3C) = r0;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019F2D0: ;
    if ((u32)r30 != (u32)0x0) goto L_8019F29C;
L_8019F2D8: ;
    /* addic. r0, r1, 0x58 */;
    if ((u32)r30 == (u32)0x0) goto L_8019F2E8;
    r0 = *(u32*)(sp + 0x38);
    *(u32*)(sp + 0x58) = r0;
L_8019F2E8: ;
    r0 = *(u32*)(sp + 0x3C);
    *(u32*)(sp + 0x5C) = r0;
    goto L_8019F424;
L_8019F2F4: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r30 != (u32)0x0) goto L_8019F324;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_8019F324;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r1 + 0x58;
    r5 = r1 + 0x5c;
    fn_80199178();
L_8019F324: ;
    r31 = *(u32*)((u8*)r30 + 0x10);
    goto L_8019F41C;
L_8019F32C: ;
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)(sp + 0x2C) = r0;
    if ((u32)r31 == (u32)0x0) goto L_8019F3D8;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_8019F364;
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = r1 + 0x28;
    r5 = r1 + 0x2c;
    fn_8019F1C4();
    goto L_8019F3D8;
L_8019F364: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_8019F394;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F394;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r4 = r1 + 0x28;
    r5 = r1 + 0x2c;
    fn_80199178();
L_8019F394: ;
    r30 = *(u32*)((u8*)r31 + 0x10);
    goto L_8019F3D0;
L_8019F39C: ;
    r3 = r30;
    r4 = r1 + 0x30;
    r5 = r1 + 0x34;
    fn_8019F1C4();
    r5 = *(u32*)(sp + 0x28);
    r4 = *(u32*)(sp + 0x30);
    r3 = *(u32*)(sp + 0x2C);
    r0 = *(u32*)(sp + 0x34);
    r4 = r5 + r4;
    r0 = r3 + r0;
    *(u32*)(sp + 0x2C) = r0;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019F3D0: ;
    if ((u32)r30 != (u32)0x0) goto L_8019F39C;
L_8019F3D8: ;
    /* addic. r0, r1, 0x60 */;
    if ((u32)r30 == (u32)0x0) goto L_8019F3E8;
    r0 = *(u32*)(sp + 0x28);
    *(u32*)(sp + 0x60) = r0;
L_8019F3E8: ;
    /* addic. r0, r1, 0x64 */;
    if ((u32)r30 == (u32)0x0) goto L_8019F3F8;
    r0 = *(u32*)(sp + 0x2C);
    *(u32*)(sp + 0x64) = r0;
L_8019F3F8: ;
    r5 = *(u32*)(sp + 0x58);
    r4 = *(u32*)(sp + 0x60);
    r3 = *(u32*)(sp + 0x5C);
    r0 = *(u32*)(sp + 0x64);
    r4 = r5 + r4;
    r0 = r3 + r0;
    *(u32*)(sp + 0x5C) = r0;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_8019F41C: ;
    if ((u32)r31 != (u32)0x0) goto L_8019F32C;
L_8019F424: ;
    /* addic. r0, r1, 0x74 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F434;
    r0 = *(u32*)(sp + 0x58);
    *(u32*)(sp + 0x74) = r0;
L_8019F434: ;
    r0 = *(u32*)(sp + 0x5C);
    *(u32*)(sp + 0x70) = r0;
    goto L_8019F6E4;
L_8019F440: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_8019F470;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F470;
    r3 = *(u32*)((u8*)r27 + 0x18);
    r4 = r1 + 0x74;
    r5 = r1 + 0x70;
    fn_80199178();
L_8019F470: ;
    r30 = *(u32*)((u8*)r27 + 0x10);
    goto L_8019F6DC;
L_8019F478: ;
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)(sp + 0x4C) = r0;
    if ((u32)r30 == (u32)0x0) goto L_8019F698;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r30 == (u32)0x0) goto L_8019F568;
    r31 = *(u32*)((u8*)r30 + 0x10);
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)(sp + 0x1C) = r0;
    if ((u32)r31 == (u32)0x0) goto L_8019F54C;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_8019F4D8;
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = r1 + 0x18;
    r5 = r1 + 0x1c;
    fn_8019F1C4();
    goto L_8019F54C;
L_8019F4D8: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_8019F508;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F508;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r4 = r1 + 0x18;
    r5 = r1 + 0x1c;
    fn_80199178();
L_8019F508: ;
    r31 = *(u32*)((u8*)r31 + 0x10);
    goto L_8019F544;
L_8019F510: ;
    r3 = r31;
    r4 = r1 + 0x20;
    r5 = r1 + 0x24;
    fn_8019F1C4();
    r5 = *(u32*)(sp + 0x18);
    r4 = *(u32*)(sp + 0x20);
    r3 = *(u32*)(sp + 0x1C);
    r0 = *(u32*)(sp + 0x24);
    r4 = r5 + r4;
    r0 = r3 + r0;
    *(u32*)(sp + 0x1C) = r0;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_8019F544: ;
    if ((u32)r31 != (u32)0x0) goto L_8019F510;
L_8019F54C: ;
    /* addic. r0, r1, 0x48 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F55C;
    r0 = *(u32*)(sp + 0x18);
    *(u32*)(sp + 0x48) = r0;
L_8019F55C: ;
    r0 = *(u32*)(sp + 0x1C);
    *(u32*)(sp + 0x4C) = r0;
    goto L_8019F698;
L_8019F568: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_8019F598;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F598;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r1 + 0x48;
    r5 = r1 + 0x4c;
    fn_80199178();
L_8019F598: ;
    r31 = *(u32*)((u8*)r30 + 0x10);
    goto L_8019F690;
L_8019F5A0: ;
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)(sp + 0xC) = r0;
    if ((u32)r31 == (u32)0x0) goto L_8019F64C;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_8019F5D8;
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = r1 + 0x8;
    r5 = r1 + 0xc;
    fn_8019F1C4();
    goto L_8019F64C;
L_8019F5D8: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_8019F608;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F608;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r4 = r1 + 0x8;
    r5 = r1 + 0xc;
    fn_80199178();
L_8019F608: ;
    r27 = *(u32*)((u8*)r31 + 0x10);
    goto L_8019F644;
L_8019F610: ;
    r3 = r27;
    r4 = r1 + 0x10;
    r5 = r1 + 0x14;
    fn_8019F1C4();
    r5 = *(u32*)(sp + 0x8);
    r4 = *(u32*)(sp + 0x10);
    r3 = *(u32*)(sp + 0xC);
    r0 = *(u32*)(sp + 0x14);
    r4 = r5 + r4;
    r0 = r3 + r0;
    *(u32*)(sp + 0xC) = r0;
    r27 = *(u32*)((u8*)r27 + 0x8);
L_8019F644: ;
    if ((u32)r27 != (u32)0x0) goto L_8019F610;
L_8019F64C: ;
    /* addic. r0, r1, 0x50 */;
    if ((u32)r27 == (u32)0x0) goto L_8019F65C;
    r0 = *(u32*)(sp + 0x8);
    *(u32*)(sp + 0x50) = r0;
L_8019F65C: ;
    /* addic. r0, r1, 0x54 */;
    if ((u32)r27 == (u32)0x0) goto L_8019F66C;
    r0 = *(u32*)(sp + 0xC);
    *(u32*)(sp + 0x54) = r0;
L_8019F66C: ;
    r5 = *(u32*)(sp + 0x48);
    r4 = *(u32*)(sp + 0x50);
    r3 = *(u32*)(sp + 0x4C);
    r0 = *(u32*)(sp + 0x54);
    r4 = r5 + r4;
    r0 = r3 + r0;
    *(u32*)(sp + 0x4C) = r0;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_8019F690: ;
    if ((u32)r31 != (u32)0x0) goto L_8019F5A0;
L_8019F698: ;
    /* addic. r0, r1, 0x6c */;
    if ((u32)r31 == (u32)0x0) goto L_8019F6A8;
    r0 = *(u32*)(sp + 0x48);
    *(u32*)(sp + 0x6C) = r0;
L_8019F6A8: ;
    /* addic. r0, r1, 0x68 */;
    if ((u32)r31 == (u32)0x0) goto L_8019F6B8;
    r0 = *(u32*)(sp + 0x4C);
    *(u32*)(sp + 0x68) = r0;
L_8019F6B8: ;
    r5 = *(u32*)(sp + 0x74);
    r4 = *(u32*)(sp + 0x6C);
    r3 = *(u32*)(sp + 0x70);
    r0 = *(u32*)(sp + 0x68);
    r4 = r5 + r4;
    r0 = r3 + r0;
    *(u32*)(sp + 0x70) = r0;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019F6DC: ;
    if ((u32)r30 != (u32)0x0) goto L_8019F478;
L_8019F6E4: ;
    if ((u32)r28 == (u32)0x0) goto L_8019F6F4;
    r0 = *(u32*)(sp + 0x74);
    *(u32*)((u8*)r28 + 0x0) = r0;
L_8019F6F4: ;
    if ((u32)r29 == (u32)0x0) goto L_8019F704;
    r0 = *(u32*)(sp + 0x70);
    *(u32*)((u8*)r29 + 0x0) = r0;
L_8019F704: ;
    /* lmw r27, 0x7c(r1) */;
    return;
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    /* stmw r25, 0x14(r1) */;
    /* mr. r25, r3 */;
    r28 = r4;
    if ((s32)r0 == (s32)0) goto L_8019FAD8;
    if ((u32)r25 == (u32)0x0) goto L_8019F880;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r25 == (u32)0x0) goto L_8019F874;
    if ((u32)r25 == (u32)0x0) goto L_8019F874;
    if ((u32)r25 != (u32)0x0) goto L_8019F844;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019F844: ;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r25 != (u32)0x0) goto L_8019F864;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r25 == (u32)0x0) goto L_8019F864;
    r3 = 0x1;
L_8019F864: ;
    if ((s32)r3 != (s32)0x0) goto L_8019F874;
    r3 = r25;
    fn_8019D620();
L_8019F874: ;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & ~r28;
    *(u32*)((u8*)r25 + 0x14) = r0;
L_8019F880: ;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FAD8;
    r29 = *(u32*)((u8*)r25 + 0x10);
    goto L_8019FAD0;
L_8019F894: ;
    if ((u32)r29 == (u32)0x0) goto L_8019FACC;
    if ((u32)r29 == (u32)0x0) goto L_8019F910;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r29 == (u32)0x0) goto L_8019F904;
    if ((u32)r29 == (u32)0x0) goto L_8019F904;
    if ((u32)r29 != (u32)0x0) goto L_8019F8D4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019F8D4: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r29 != (u32)0x0) goto L_8019F8F4;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r29 == (u32)0x0) goto L_8019F8F4;
    r3 = 0x1;
L_8019F8F4: ;
    if ((s32)r3 != (s32)0x0) goto L_8019F904;
    r3 = r29;
    fn_8019D620();
L_8019F904: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & ~r28;
    *(u32*)((u8*)r29 + 0x14) = r0;
L_8019F910: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FACC;
    r31 = *(u32*)((u8*)r29 + 0x10);
    goto L_8019FAC4;
L_8019F924: ;
    if ((u32)r31 == (u32)0x0) goto L_8019FAC0;
    if ((u32)r31 == (u32)0x0) goto L_8019F9A0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r31 == (u32)0x0) goto L_8019F994;
    if ((u32)r31 == (u32)0x0) goto L_8019F994;
    if ((u32)r31 != (u32)0x0) goto L_8019F964;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019F964: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r31 != (u32)0x0) goto L_8019F984;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r31 == (u32)0x0) goto L_8019F984;
    r3 = 0x1;
L_8019F984: ;
    if ((s32)r3 != (s32)0x0) goto L_8019F994;
    r3 = r31;
    fn_8019D620();
L_8019F994: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & ~r28;
    *(u32*)((u8*)r31 + 0x14) = r0;
L_8019F9A0: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FAC0;
    r30 = *(u32*)((u8*)r31 + 0x10);
    goto L_8019FAB8;
L_8019F9B4: ;
    if ((u32)r30 == (u32)0x0) goto L_8019FAB4;
    if ((u32)r30 == (u32)0x0) goto L_8019FA00;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r30 == (u32)0x0) goto L_8019F9F4;
    if ((u32)r30 == (u32)0x0) goto L_8019F9F4;
    r3 = r30;
    fn_8019D980();
    if ((s32)r3 != (s32)0x0) goto L_8019F9F4;
    r3 = r30;
    fn_8019D620();
L_8019F9F4: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & ~r28;
    *(u32*)((u8*)r30 + 0x14) = r0;
L_8019FA00: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FAB4;
    r27 = *(u32*)((u8*)r30 + 0x10);
    goto L_8019FAAC;
L_8019FA14: ;
    if ((u32)r27 == (u32)0x0) goto L_8019FAA8;
    if ((u32)r27 == (u32)0x0) goto L_8019FA48;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r27 == (u32)0x0) goto L_8019FA3C;
    r3 = r27;
    fn_8019F778();
L_8019FA3C: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & ~r28;
    *(u32*)((u8*)r27 + 0x14) = r0;
L_8019FA48: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r27 != (u32)0x0) goto L_8019FAA8;
    r25 = *(u32*)((u8*)r27 + 0x10);
    goto L_8019FAA0;
L_8019FA5C: ;
    if ((u32)r25 == (u32)0x0) goto L_8019FA9C;
    r3 = r25;
    r4 = r28;
    fn_8019FAEC();
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r25 != (u32)0x0) goto L_8019FA9C;
    r26 = *(u32*)((u8*)r25 + 0x10);
    goto L_8019FA94;
L_8019FA84: ;
    r3 = r26;
    r4 = r28;
    fn_8019F7F0();
    r26 = *(u32*)((u8*)r26 + 0x8);
L_8019FA94: ;
    if ((u32)r26 != (u32)0x0) goto L_8019FA84;
L_8019FA9C: ;
    r25 = *(u32*)((u8*)r25 + 0x8);
L_8019FAA0: ;
    if ((u32)r25 != (u32)0x0) goto L_8019FA5C;
L_8019FAA8: ;
    r27 = *(u32*)((u8*)r27 + 0x8);
L_8019FAAC: ;
    if ((u32)r27 != (u32)0x0) goto L_8019FA14;
L_8019FAB4: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019FAB8: ;
    if ((u32)r30 != (u32)0x0) goto L_8019F9B4;
L_8019FAC0: ;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_8019FAC4: ;
    if ((u32)r31 != (u32)0x0) goto L_8019F924;
L_8019FACC: ;
    r29 = *(u32*)((u8*)r29 + 0x8);
L_8019FAD0: ;
    if ((u32)r29 != (u32)0x0) goto L_8019F894;
L_8019FAD8: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8019FAEC | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FAEC(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
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
    if ((s32)r0 == (s32)0) goto L_8019FB78;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 ^ r31;
    r0 = r0 & 0x00000008;
    if ((s32)r0 == (s32)0) goto L_8019FB6C;
    if ((u32)r30 == (u32)0x0) goto L_8019FB6C;
    if ((u32)r30 != (u32)0x0) goto L_8019FB3C;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019FB3C: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r30 != (u32)0x0) goto L_8019FB5C;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r30 == (u32)0x0) goto L_8019FB5C;
    r3 = 0x1;
L_8019FB5C: ;
    if ((s32)r3 != (s32)0x0) goto L_8019FB6C;
    r3 = r30;
    fn_8019D620();
L_8019FB6C: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & ~r31;
    *(u32*)((u8*)r30 + 0x14) = r0;
L_8019FB78: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x8019FB90 | 0x2FC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    /* stmw r25, 0x14(r1) */;
    /* mr. r25, r3 */;
    r28 = r4;
    if ((s32)r0 == (s32)0) goto L_8019FE78;
    if ((u32)r25 == (u32)0x0) goto L_8019FC20;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r25 == (u32)0x0) goto L_8019FC14;
    if ((u32)r25 == (u32)0x0) goto L_8019FC14;
    if ((u32)r25 != (u32)0x0) goto L_8019FBE4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019FBE4: ;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r25 != (u32)0x0) goto L_8019FC04;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r25 == (u32)0x0) goto L_8019FC04;
    r3 = 0x1;
L_8019FC04: ;
    if ((s32)r3 != (s32)0x0) goto L_8019FC14;
    r3 = r25;
    fn_8019D620();
L_8019FC14: ;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 | r28;
    *(u32*)((u8*)r25 + 0x14) = r0;
L_8019FC20: ;
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FE78;
    r29 = *(u32*)((u8*)r25 + 0x10);
    goto L_8019FE70;
L_8019FC34: ;
    if ((u32)r29 == (u32)0x0) goto L_8019FE6C;
    if ((u32)r29 == (u32)0x0) goto L_8019FCB0;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r29 == (u32)0x0) goto L_8019FCA4;
    if ((u32)r29 == (u32)0x0) goto L_8019FCA4;
    if ((u32)r29 != (u32)0x0) goto L_8019FC74;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019FC74: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r29 != (u32)0x0) goto L_8019FC94;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r29 == (u32)0x0) goto L_8019FC94;
    r3 = 0x1;
L_8019FC94: ;
    if ((s32)r3 != (s32)0x0) goto L_8019FCA4;
    r3 = r29;
    fn_8019D620();
L_8019FCA4: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 | r28;
    *(u32*)((u8*)r29 + 0x14) = r0;
L_8019FCB0: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FE6C;
    r31 = *(u32*)((u8*)r29 + 0x10);
    goto L_8019FE64;
L_8019FCC4: ;
    if ((u32)r31 == (u32)0x0) goto L_8019FE60;
    if ((u32)r31 == (u32)0x0) goto L_8019FD40;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r31 == (u32)0x0) goto L_8019FD34;
    if ((u32)r31 == (u32)0x0) goto L_8019FD34;
    if ((u32)r31 != (u32)0x0) goto L_8019FD04;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019FD04: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r31 != (u32)0x0) goto L_8019FD24;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r31 == (u32)0x0) goto L_8019FD24;
    r3 = 0x1;
L_8019FD24: ;
    if ((s32)r3 != (s32)0x0) goto L_8019FD34;
    r3 = r31;
    fn_8019D620();
L_8019FD34: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 | r28;
    *(u32*)((u8*)r31 + 0x14) = r0;
L_8019FD40: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FE60;
    r30 = *(u32*)((u8*)r31 + 0x10);
    goto L_8019FE58;
L_8019FD54: ;
    if ((u32)r30 == (u32)0x0) goto L_8019FE54;
    if ((u32)r30 == (u32)0x0) goto L_8019FDA0;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r30 == (u32)0x0) goto L_8019FD94;
    if ((u32)r30 == (u32)0x0) goto L_8019FD94;
    r3 = r30;
    fn_8019D980();
    if ((s32)r3 != (s32)0x0) goto L_8019FD94;
    r3 = r30;
    fn_8019D620();
L_8019FD94: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 | r28;
    *(u32*)((u8*)r30 + 0x14) = r0;
L_8019FDA0: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r3 != (s32)0x0) goto L_8019FE54;
    r27 = *(u32*)((u8*)r30 + 0x10);
    goto L_8019FE4C;
L_8019FDB4: ;
    if ((u32)r27 == (u32)0x0) goto L_8019FE48;
    if ((u32)r27 == (u32)0x0) goto L_8019FDE8;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 ^ r28;
    r0 = r0 & 0x00000008;
    if ((u32)r27 == (u32)0x0) goto L_8019FDDC;
    r3 = r27;
    fn_8019F778();
L_8019FDDC: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 | r28;
    *(u32*)((u8*)r27 + 0x14) = r0;
L_8019FDE8: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r27 != (u32)0x0) goto L_8019FE48;
    r25 = *(u32*)((u8*)r27 + 0x10);
    goto L_8019FE40;
L_8019FDFC: ;
    if ((u32)r25 == (u32)0x0) goto L_8019FE3C;
    r3 = r25;
    r4 = r28;
    fn_8019FE8C();
    r0 = *(u32*)((u8*)r25 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r25 != (u32)0x0) goto L_8019FE3C;
    r26 = *(u32*)((u8*)r25 + 0x10);
    goto L_8019FE34;
L_8019FE24: ;
    r3 = r26;
    r4 = r28;
    fn_8019FB90();
    r26 = *(u32*)((u8*)r26 + 0x8);
L_8019FE34: ;
    if ((u32)r26 != (u32)0x0) goto L_8019FE24;
L_8019FE3C: ;
    r25 = *(u32*)((u8*)r25 + 0x8);
L_8019FE40: ;
    if ((u32)r25 != (u32)0x0) goto L_8019FDFC;
L_8019FE48: ;
    r27 = *(u32*)((u8*)r27 + 0x8);
L_8019FE4C: ;
    if ((u32)r27 != (u32)0x0) goto L_8019FDB4;
L_8019FE54: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_8019FE58: ;
    if ((u32)r30 != (u32)0x0) goto L_8019FD54;
L_8019FE60: ;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_8019FE64: ;
    if ((u32)r31 != (u32)0x0) goto L_8019FCC4;
L_8019FE6C: ;
    r29 = *(u32*)((u8*)r29 + 0x8);
L_8019FE70: ;
    if ((u32)r29 != (u32)0x0) goto L_8019FC34;
L_8019FE78: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x8019FE8C | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FE8C(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_8019D620();
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
    if ((s32)r0 == (s32)0) goto L_8019FF18;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 ^ r31;
    r0 = r0 & 0x00000008;
    if ((s32)r0 == (s32)0) goto L_8019FF0C;
    if ((u32)r30 == (u32)0x0) goto L_8019FF0C;
    if ((u32)r30 != (u32)0x0) goto L_8019FEDC;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_8019FEDC: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r30 != (u32)0x0) goto L_8019FEFC;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r30 == (u32)0x0) goto L_8019FEFC;
    r3 = 0x1;
L_8019FEFC: ;
    if ((s32)r3 != (s32)0x0) goto L_8019FF0C;
    r3 = r30;
    fn_8019D620();
L_8019FF0C: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 | r31;
    *(u32*)((u8*)r30 + 0x14) = r0;
L_8019FF18: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019FF74(void) {
    extern u8 lbl_80274B28[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB28[];
    extern void fn_80196D78();
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    if ((s32)r0 == (s32)0) goto L_801A013C;
    if ((u32)r31 != (u32)0x0) goto L_8019FFA8;
    goto L_801A013C;
L_8019FFA8: ;
    if ((u32)r31 == (u32)0x0) goto L_801A00DC;
    r0 = *(u32*)((u8*)r31 + 0xC);
    r29 = *(u32*)((u8*)r31 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_801A00D4;
    r3 = *(u32*)((u8*)r31 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r0 != (u32)r31) goto L_8019FFDC;
    r3 = *(u32*)((u8*)r31 + 0xC);
    *(u32*)((u8*)r3 + 0x10) = r29;
    goto L_801A006C;
L_8019FFDC: ;
    if ((u32)r31 == (u32)0x0) goto L_8019FFF0;
    r0 = *(u32*)((u8*)r31 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_8019FFF8;
L_8019FFF0: ;
    r28 = 0x0;
    goto L_801A0050;
L_8019FFF8: ;
    r3 = *(u32*)((u8*)r31 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r31 != (u32)r0) goto L_801A0010;
    r28 = 0x0;
    goto L_801A0050;
L_801A0010: ;
    r3 = *(u32*)((u8*)r31 + 0xC);
    r28 = *(u32*)((u8*)r3 + 0x10);
    goto L_801A0030;
L_801A001C: ;
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((u32)r0 != (u32)r31) goto L_801A002C;
    goto L_801A0050;
L_801A002C: ;
    r28 = *(u32*)((u8*)r28 + 0x8);
L_801A0030: ;
    if ((u32)r28 != (u32)0x0) goto L_801A001C;
    r4 = (u32)lbl_80274B28;
    r3 = (u32)lbl_8047DB20;
    r5 = (u32)lbl_80274B28;
    r4 = 0x5f8;
    fn_80196D78();
    r28 = 0x0;
L_801A0050: ;
    if ((u32)r28 != (u32)0x0) goto L_801A0068;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x57b;
    r5 = (u32)lbl_8047DB28;
    fn_80196E10();
L_801A0068: ;
    *(u32*)((u8*)r28 + 0x8) = r29;
L_801A006C: ;
    r6 = *(u32*)((u8*)r31 + 0xC);
    goto L_801A00C4;
L_801A0074: ;
    r4 = (0x9000 << 16);
    r5 = *(u32*)((u8*)r6 + 0x10);
    /* subi r4, r4, 0x1 */;
    goto L_801A00A0;
L_801A0084: ;
    r0 = *(u32*)((u8*)r5 + 0x14);
    r3 = *(u32*)((u8*)r5 + 0x14);
    r0 = r0 << 10;
    r5 = *(u32*)((u8*)r5 + 0x8);
    r0 = r3 | r0;
    r0 = r0 & 0x70000000;
    r4 = r4 | r0;
L_801A00A0: ;
    if ((u32)r5 != (u32)0x0) goto L_801A0084;
    r0 = *(u32*)((u8*)r6 + 0x14);
    /* andc. r0, r0, r4 */;
    if ((u32)r5 == (u32)0x0) goto L_801A00CC;
    r0 = *(u32*)((u8*)r6 + 0x14);
    r0 = r0 & r4;
    *(u32*)((u8*)r6 + 0x14) = r0;
    r6 = *(u32*)((u8*)r6 + 0x8);
L_801A00C4: ;
    if ((u32)r6 != (u32)0x0) goto L_801A0074;
L_801A00CC: ;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0xC) = r0;
L_801A00D4: ;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_801A00DC: ;
    r0 = *(u32*)((u8*)r30 + 0xC);
    *(u32*)((u8*)r31 + 0xC) = r0;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r31 + 0x8) = r0;
    *(u32*)((u8*)r30 + 0x8) = r31;
    r0 = *(u32*)((u8*)r30 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_801A013C;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 << 10;
    r4 = *(u32*)((u8*)r30 + 0xC);
    r0 = r3 | r0;
    r3 = r0 & 0x70000000;
    goto L_801A0134;
L_801A0118: ;
    r0 = *(u32*)((u8*)r4 + 0x14);
    /* andc. r0, r3, r0 */;
    if ((u32)r0 == (u32)0x0) goto L_801A013C;
    r0 = *(u32*)((u8*)r4 + 0x14);
    r0 = r0 | r3;
    *(u32*)((u8*)r4 + 0x14) = r0;
    r4 = *(u32*)((u8*)r4 + 0xC);
L_801A0134: ;
    if ((u32)r4 != (u32)0x0) goto L_801A0118;
L_801A013C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801A015C | 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A015C(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196E10();
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

    r29 = r4;
    /* mr. r28, r3 */;
    r3 = (u32)lbl_80274AA0;
    r31 = (u32)lbl_80274AA0;
    if ((s32)r0 == (s32)0) goto L_801A0290;
    if ((u32)r29 != (u32)0x0) goto L_801A0198;
    goto L_801A0290;
L_801A0198: ;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_801A01C0;
    r3 = r31 + 0x178;
    /* crclr cr1eq */;
    OSReport();
    r5 = r31 + 0x194;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x552;
    fn_80196E10();
L_801A01C0: ;
    r0 = *(u32*)((u8*)r29 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_801A01E8;
    r3 = r31 + 0x1ac;
    /* crclr cr1eq */;
    OSReport();
    r5 = r31 + 0x1cc;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x553;
    fn_80196E10();
L_801A01E8: ;
    r0 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r0 != (u32)0x0) goto L_801A01FC;
    *(u32*)((u8*)r28 + 0x10) = r29;
    goto L_801A024C;
L_801A01FC: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r0 == (u32)0x0) goto L_801A0218;
    r5 = r31 + 0x1e0;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x559;
    fn_80196E10();
L_801A0218: ;
    r30 = *(u32*)((u8*)r28 + 0x10);
    goto L_801A023C;
L_801A0220: ;
    if ((u32)r30 != (u32)r29) goto L_801A0238;
    r5 = r31 + 0x200;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x55c;
    fn_80196E10();
L_801A0238: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
L_801A023C: ;
    r0 = *(u32*)((u8*)r30 + 0x8);
    if ((u32)r0 != (u32)0x0) goto L_801A0220;
    *(u32*)((u8*)r30 + 0x8) = r29;
L_801A024C: ;
    *(u32*)((u8*)r29 + 0xC) = r28;
    r4 = r28;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r3 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 << 10;
    r0 = r3 | r0;
    r3 = r0 & 0x70000000;
    goto L_801A0288;
L_801A026C: ;
    r0 = *(u32*)((u8*)r4 + 0x14);
    /* andc. r0, r3, r0 */;
    if ((u32)r0 == (u32)0x0) goto L_801A0290;
    r0 = *(u32*)((u8*)r4 + 0x14);
    r0 = r0 | r3;
    *(u32*)((u8*)r4 + 0x14) = r0;
    r4 = *(u32*)((u8*)r4 + 0xC);
L_801A0288: ;
    if ((u32)r4 != (u32)0x0) goto L_801A026C;
L_801A0290: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801A02B0 | 0x28C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A02B0(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196D78();
    extern void fn_80196E10();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    if ((s32)r0 != (s32)0) goto L_801A02E4;
    r3 = 0x0;
    goto L_801A051C;
L_801A02E4: ;
    r30 = *(u32*)((u8*)r29 + 0x10);
    if ((u32)r30 == (u32)0x0) goto L_801A030C;
    r0 = *(u32*)((u8*)r30 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_801A030C;
    r5 = r31 + 0x1cc;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x4cb;
    fn_80196E10();
L_801A030C: ;
    if ((u32)r30 == (u32)0x0) goto L_801A031C;
    r28 = r30;
    goto L_801A0320;
L_801A031C: ;
    r28 = *(u32*)((u8*)r29 + 0x8);
L_801A0320: ;
    if ((u32)r29 == (u32)0x0) goto L_801A0334;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_801A033C;
L_801A0334: ;
    r3 = 0x0;
    goto L_801A0390;
L_801A033C: ;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r29 != (u32)r0) goto L_801A0354;
    r3 = 0x0;
    goto L_801A0390;
L_801A0354: ;
    r3 = *(u32*)((u8*)r29 + 0xC);
    r3 = *(u32*)((u8*)r3 + 0x10);
    goto L_801A0374;
L_801A0360: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 != (u32)r29) goto L_801A0370;
    goto L_801A0390;
L_801A0370: ;
    r3 = *(u32*)((u8*)r3 + 0x8);
L_801A0374: ;
    if ((u32)r3 != (u32)0x0) goto L_801A0360;
    r5 = r31 + 0x88;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x5f8;
    fn_80196D78();
    r3 = 0x0;
L_801A0390: ;
    if ((u32)r3 == (u32)0x0) goto L_801A03A0;
    *(u32*)((u8*)r3 + 0x8) = r28;
    goto L_801A03B4;
L_801A03A0: ;
    r0 = *(u32*)((u8*)r29 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_801A03B4;
    r3 = *(u32*)((u8*)r29 + 0xC);
    *(u32*)((u8*)r3 + 0x10) = r28;
L_801A03B4: ;
    if ((u32)r28 == (u32)0x0) goto L_801A03D4;
    if ((u32)r28 != (u32)r30) goto L_801A03D4;
    r0 = *(u32*)((u8*)r29 + 0x8);
    *(u32*)((u8*)r28 + 0x8) = r0;
    r0 = *(u32*)((u8*)r29 + 0xC);
    *(u32*)((u8*)r28 + 0xC) = r0;
L_801A03D4: ;
    r0 = 0x0;
    r3 = 0x0;
    *(u32*)((u8*)r29 + 0xC) = r0;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x10) = r3;
    *(u32*)((u8*)r29 + 0x8) = r0;
    if ((u32)r29 == (u32)0x0) goto L_801A0518;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r29 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r29 == (u32)0x0) goto L_801A0418;
    goto L_801A0430;
L_801A0418: ;
    r0 = *(u16*)((u8*)r29 + 0x4);
    r3 = *(u16*)((u8*)r29 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r29 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A0430: ;
    if ((s32)r3 == (s32)0x0) goto L_801A0518;
    r3 = *(u16*)((u8*)r29 + 0x6);
    /* subic. r0, r3, 0x1 */;
    if ((s32)r3 >= (s32)0x0) goto L_801A0478;
    if ((u32)r29 == (u32)0x0) goto L_801A0518;
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
    goto L_801A0518;
L_801A0478: ;
    r3 = *(u16*)((u8*)r29 + 0x6);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r29 + 0x6) = r0;
    r0 = *(u16*)((u8*)r29 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_801A04A0;
    r3 = r31 + 0x54;
    r5 = r31 + 0x60;
    r4 = 0x9e;
    fn_80196E10();
L_801A04A0: ;
    r4 = *(u32*)((u8*)r29 + 0x0);
    r3 = r29;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u16*)((u8*)r29 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_801A04C8;
    goto L_801A04E0;
L_801A04C8: ;
    r3 = *(u16*)((u8*)r29 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r29 + 0x6) = r0;
    r0 = *(u16*)((u8*)r29 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_801A04E0: ;
    if ((s32)r0 == (s32)0x0) goto L_801A0518;
    if ((u32)r29 == (u32)0x0) goto L_801A0518;
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
L_801A0518: ;
    r3 = r30;
L_801A051C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801A053C | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A053C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801A05D8;
    r0 = *(u16*)((u8*)r31 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((s32)r0 == (s32)0) goto L_801A0568;
    goto L_801A0580;
L_801A0568: ;
    r3 = *(u16*)((u8*)r31 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r31 + 0x6) = r0;
    r0 = *(u16*)((u8*)r31 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_801A0580: ;
    if ((s32)r0 == (s32)0x0) goto L_801A05D8;
    r0 = *(u16*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)0xffff) goto L_801A059C;
    r0 = -0x1;
    goto L_801A05A0;
L_801A059C: ;
    r0 = *(u16*)((u8*)r31 + 0x4);
L_801A05A0: ;
    if ((s32)r0 >= (s32)0x0) goto L_801A05D8;
    if ((u32)r31 == (u32)0x0) goto L_801A05D8;
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
L_801A05D8: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801A05EC | 0x158 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A05EC(void) {
    extern u8 lbl_80274AF4[];
    extern u8 lbl_80274B00[];
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801A0730;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r31 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((s32)r0 == (s32)0) goto L_801A0628;
    goto L_801A0640;
L_801A0628: ;
    r0 = *(u16*)((u8*)r31 + 0x4);
    r3 = *(u16*)((u8*)r31 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r31 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A0640: ;
    if ((s32)r3 == (s32)0x0) goto L_801A0730;
    r3 = *(u16*)((u8*)r31 + 0x6);
    /* subic. r0, r3, 0x1 */;
    if ((s32)r3 >= (s32)0x0) goto L_801A0688;
    if ((u32)r31 == (u32)0x0) goto L_801A0730;
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
    goto L_801A0730;
L_801A0688: ;
    r3 = *(u16*)((u8*)r31 + 0x6);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r31 + 0x6) = r0;
    r0 = *(u16*)((u8*)r31 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_801A06B8;
    r3 = (u32)lbl_80274AF4;
    r5 = (u32)lbl_80274B00;
    r3 = (u32)lbl_80274AF4;
    r4 = 0x9e;
    r5 = (u32)lbl_80274B00;
    fn_80196E10();
L_801A06B8: ;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u16*)((u8*)r31 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_801A06E0;
    goto L_801A06F8;
L_801A06E0: ;
    r3 = *(u16*)((u8*)r31 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r31 + 0x6) = r0;
    r0 = *(u16*)((u8*)r31 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_801A06F8: ;
    if ((s32)r0 == (s32)0x0) goto L_801A0730;
    if ((u32)r31 == (u32)0x0) goto L_801A0730;
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
L_801A0730: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801A0744 | 0x458 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    /* stmw r23, 0xc(r1) */;
    r30 = r3;
    r29 = (u32)lbl_80274AA0;
    r31 = r4;
    goto L_801A0B78;
L_801A0768: ;
    if ((u32)r30 == (u32)0x0) goto L_801A0944;
    if ((u32)r31 == (u32)0x0) goto L_801A0944;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = *(u32*)((u8*)r31 + 0x3C);
    fn_801AEBE4();
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_801A0924;
    r28 = *(u32*)((u8*)r30 + 0x10);
    if ((u32)r28 == (u32)0x0) goto L_801A08C0;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r28 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r28 == (u32)0x0) goto L_801A07C0;
    goto L_801A07D8;
L_801A07C0: ;
    r0 = *(u16*)((u8*)r28 + 0x4);
    r3 = *(u16*)((u8*)r28 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r28 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A07D8: ;
    if ((s32)r3 == (s32)0x0) goto L_801A08C0;
    r3 = *(u16*)((u8*)r28 + 0x6);
    /* subic. r0, r3, 0x1 */;
    if ((s32)r3 >= (s32)0x0) goto L_801A0820;
    if ((u32)r28 == (u32)0x0) goto L_801A08C0;
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
    goto L_801A08C0;
L_801A0820: ;
    r3 = *(u16*)((u8*)r28 + 0x6);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r28 + 0x6) = r0;
    r0 = *(u16*)((u8*)r28 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_801A0848;
    r3 = r29 + 0x54;
    r5 = r29 + 0x60;
    r4 = 0x9e;
    fn_80196E10();
L_801A0848: ;
    r4 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u16*)((u8*)r28 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_801A0870;
    goto L_801A0888;
L_801A0870: ;
    r3 = *(u16*)((u8*)r28 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r28 + 0x6) = r0;
    r0 = *(u16*)((u8*)r28 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_801A0888: ;
    if ((s32)r0 == (s32)0x0) goto L_801A08C0;
    if ((u32)r28 == (u32)0x0) goto L_801A08C0;
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
L_801A08C0: ;
    r4 = *(u32*)((u8*)r31 + 0x8);
    r3 = 0x0;
    r5 = 0x0;
    fn_8019C128();
    *(u32*)((u8*)r30 + 0x10) = r3;
    r0 = *(u32*)((u8*)r30 + 0x10);
    if ((u32)r0 != (u32)0x0) goto L_801A08F0;
    r5 = r29 + 0x210;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x45f;
    fn_80196E10();
L_801A08F0: ;
    r4 = *(u32*)((u8*)r30 + 0x10);
    if ((u32)r4 == (u32)0x0) goto L_801A0924;
    r3 = *(u16*)((u8*)r4 + 0x4);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r4 + 0x4) = r0;
    r0 = *(u16*)((u8*)r4 + 0x4);
    if ((u32)r0 != (u32)0xffff) goto L_801A0924;
    r3 = r29 + 0x54;
    r5 = r29 + 0xc4;
    r4 = 0x5d;
    fn_80196E10();
L_801A0924: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0xffff) goto L_801A0944;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = *(u32*)((u8*)r31 + 0x10);
    fn_801991F8();
L_801A0944: ;
    r0 = *(u32*)((u8*)r30 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r0 != (u32)0xffff) goto L_801A0B70;
    r27 = *(u32*)((u8*)r31 + 0x8);
    r28 = *(u32*)((u8*)r30 + 0x10);
    goto L_801A0B60;
L_801A095C: ;
    if ((u32)r28 == (u32)0x0) goto L_801A0A50;
    if ((u32)r27 == (u32)0x0) goto L_801A0A50;
    r3 = *(u32*)((u8*)r28 + 0x80);
    r4 = *(u32*)((u8*)r27 + 0x3C);
    fn_801AEBE4();
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r27 == (u32)0x0) goto L_801A0A30;
    r25 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r25 == (u32)0x0) goto L_801A09F0;
    r3 = r25;
    fn_801A0D48();
    if ((s32)r3 == (s32)0x0) goto L_801A09F0;
    r3 = r25;
    fn_801A0D3C();
    if ((s32)r3 >= (s32)0x0) goto L_801A09BC;
    r3 = r25;
    fn_801A0CE8();
    goto L_801A09F0;
L_801A09BC: ;
    r3 = r25;
    fn_801A0C9C();
    r4 = *(u32*)((u8*)r25 + 0x0);
    r3 = r25;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r3 = r25;
    fn_801A0C68();
    if ((s32)r3 == (s32)0x0) goto L_801A09F0;
    r3 = r25;
    fn_801A0CE8();
L_801A09F0: ;
    r4 = *(u32*)((u8*)r27 + 0x8);
    r3 = 0x0;
    r5 = 0x0;
    fn_8019C128();
    *(u32*)((u8*)r28 + 0x10) = r3;
    r0 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r0 != (u32)0x0) goto L_801A0A20;
    r5 = r29 + 0x210;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x45f;
    fn_80196E10();
L_801A0A20: ;
    r3 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r3 == (u32)0x0) goto L_801A0A30;
    fn_801A0C1C();
L_801A0A30: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r3 == (u32)0x0) goto L_801A0A50;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = *(u32*)((u8*)r27 + 0x10);
    fn_801991F8();
L_801A0A50: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r3 != (u32)0x0) goto L_801A0B58;
    r25 = *(u32*)((u8*)r27 + 0x8);
    r26 = *(u32*)((u8*)r28 + 0x10);
    goto L_801A0B48;
L_801A0A68: ;
    if ((u32)r26 == (u32)0x0) goto L_801A0AEC;
    if ((u32)r25 == (u32)0x0) goto L_801A0AEC;
    r3 = *(u32*)((u8*)r26 + 0x80);
    r4 = *(u32*)((u8*)r25 + 0x3C);
    fn_801AEBE4();
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r25 == (u32)0x0) goto L_801A0ACC;
    r3 = *(u32*)((u8*)r26 + 0x10);
    fn_801A05EC();
    r3 = *(u32*)((u8*)r25 + 0x8);
    r4 = 0x0;
    fn_801A0BF0();
    *(u32*)((u8*)r26 + 0x10) = r3;
    r0 = *(u32*)((u8*)r26 + 0x10);
    if ((u32)r0 != (u32)0x0) goto L_801A0AC4;
    r5 = r29 + 0x210;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x45f;
    fn_80196E10();
L_801A0AC4: ;
    r3 = *(u32*)((u8*)r26 + 0x10);
    fn_801A0B9C();
L_801A0ACC: ;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_801A0AEC;
    r3 = *(u32*)((u8*)r26 + 0x18);
    r4 = *(u32*)((u8*)r25 + 0x10);
    fn_801991F8();
L_801A0AEC: ;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r0 != (u32)0x0) goto L_801A0B40;
    r24 = *(u32*)((u8*)r26 + 0x10);
    r23 = *(u32*)((u8*)r25 + 0x8);
    goto L_801A0B30;
L_801A0B04: ;
    r3 = r24;
    r4 = r23;
    fn_801A0D94();
    r0 = *(u32*)((u8*)r24 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r0 != (u32)0x0) goto L_801A0B28;
    r3 = *(u32*)((u8*)r24 + 0x10);
    r4 = *(u32*)((u8*)r23 + 0x8);
    fn_801A0744();
L_801A0B28: ;
    r24 = *(u32*)((u8*)r24 + 0x8);
    r23 = *(u32*)((u8*)r23 + 0xC);
L_801A0B30: ;
    if ((u32)r24 == (u32)0x0) goto L_801A0B40;
    if ((u32)r23 != (u32)0x0) goto L_801A0B04;
L_801A0B40: ;
    r26 = *(u32*)((u8*)r26 + 0x8);
    r25 = *(u32*)((u8*)r25 + 0xC);
L_801A0B48: ;
    if ((u32)r26 == (u32)0x0) goto L_801A0B58;
    if ((u32)r25 != (u32)0x0) goto L_801A0A68;
L_801A0B58: ;
    r28 = *(u32*)((u8*)r28 + 0x8);
    r27 = *(u32*)((u8*)r27 + 0xC);
L_801A0B60: ;
    if ((u32)r28 == (u32)0x0) goto L_801A0B70;
    if ((u32)r27 != (u32)0x0) goto L_801A095C;
L_801A0B70: ;
    r30 = *(u32*)((u8*)r30 + 0x8);
    r31 = *(u32*)((u8*)r31 + 0xC);
L_801A0B78: ;
    if ((u32)r30 == (u32)0x0) goto L_801A0B88;
    if ((u32)r31 != (u32)0x0) goto L_801A0768;
L_801A0B88: ;
    /* lmw r23, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x54 | fn_801A0B9C | guarded_call */
void fn_801A0B9C(void) {
    if (0 /* guard r3 == 0 */) { return; }
    if (1 /* guard r0 != 0xFFFF */) { return; }
    fn_80196E10();
}

/* 0x801A0BF0 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0BF0(void) {
    extern void fn_8019C128();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    r5 = r4;
    r4 = r3;
    r3 = 0x0;
    fn_8019C128();
    return;
}
#pragma pop

/* 0x4C | fn_801A0C1C | generic */
void fn_801A0C1C(void) {
    /* refs: lbl_80274AF4, lbl_80274B64 */
    fn_80196E10();
}

/* 0x801A0C68 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0C68(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((s32)r0 == (s32)0) goto L_801A0C80;
    r3 = r0;
    return;
L_801A0C80: ;
    r4 = *(u16*)((u8*)r3 + 0x6);
    /* subi r0, r4, 0x1 */;
    *(u16*)((u8*)r3 + 0x6) = r0;
    r0 = *(u16*)((u8*)r3 + 0x6);
    r0 = __cntlzw(r0);
    r3 = (u32)r0 >> 5;
    return;
}
#pragma pop

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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A0D94(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196E10();
    extern void fn_801991F8();
    extern void fn_8019C128();
    extern void fn_801AEBE4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    if ((s32)r0 == (s32)0) goto L_801A0F9C;
    if ((u32)r29 != (u32)0x0) goto L_801A0DD0;
    goto L_801A0F9C;
L_801A0DD0: ;
    r3 = *(u32*)((u8*)r28 + 0x80);
    r4 = *(u32*)((u8*)r29 + 0x3C);
    fn_801AEBE4();
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r29 == (u32)0x0) goto L_801A0F7C;
    r30 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r30 == (u32)0x0) goto L_801A0F18;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r30 + 0x4);
    /* subi r3, r3, 0x1 */;
    r0 = r3 & 0xFFFF;
    r0 = r0 - r4;
    r0 = __cntlzw(r0);
    /* srwi. r3, r0, 5 */;
    if ((u32)r30 == (u32)0x0) goto L_801A0E18;
    goto L_801A0E30;
L_801A0E18: ;
    r0 = *(u16*)((u8*)r30 + 0x4);
    r3 = *(u16*)((u8*)r30 + 0x4);
    r4 = __cntlzw(r0);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x4) = r0;
    r3 = (u32)r4 >> 5;
L_801A0E30: ;
    if ((s32)r3 == (s32)0x0) goto L_801A0F18;
    r3 = *(u16*)((u8*)r30 + 0x6);
    /* subic. r0, r3, 0x1 */;
    if ((s32)r3 >= (s32)0x0) goto L_801A0E78;
    if ((u32)r30 == (u32)0x0) goto L_801A0F18;
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
    goto L_801A0F18;
L_801A0E78: ;
    r3 = *(u16*)((u8*)r30 + 0x6);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x6) = r0;
    r0 = *(u16*)((u8*)r30 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_801A0EA0;
    r3 = r31 + 0x54;
    r5 = r31 + 0x60;
    r4 = 0x9e;
    fn_80196E10();
L_801A0EA0: ;
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r30;
    r12 = *(u32*)((u8*)r4 + 0x4C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u16*)((u8*)r30 + 0x6);
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0x0) goto L_801A0EC8;
    goto L_801A0EE0;
L_801A0EC8: ;
    r3 = *(u16*)((u8*)r30 + 0x6);
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x6) = r0;
    r0 = *(u16*)((u8*)r30 + 0x6);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
L_801A0EE0: ;
    if ((s32)r0 == (s32)0x0) goto L_801A0F18;
    if ((u32)r30 == (u32)0x0) goto L_801A0F18;
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
L_801A0F18: ;
    r4 = *(u32*)((u8*)r29 + 0x8);
    r3 = 0x0;
    r5 = 0x0;
    fn_8019C128();
    *(u32*)((u8*)r28 + 0x10) = r3;
    r0 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r0 != (u32)0x0) goto L_801A0F48;
    r5 = r31 + 0x210;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x45f;
    fn_80196E10();
L_801A0F48: ;
    r4 = *(u32*)((u8*)r28 + 0x10);
    if ((u32)r4 == (u32)0x0) goto L_801A0F7C;
    r3 = *(u16*)((u8*)r4 + 0x4);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r4 + 0x4) = r0;
    r0 = *(u16*)((u8*)r4 + 0x4);
    if ((u32)r0 != (u32)0xffff) goto L_801A0F7C;
    r3 = r31 + 0x54;
    r5 = r31 + 0xc4;
    r4 = 0x5d;
    fn_80196E10();
L_801A0F7C: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((u32)r0 == (u32)0xffff) goto L_801A0F9C;
    r3 = *(u32*)((u8*)r28 + 0x18);
    r4 = *(u32*)((u8*)r29 + 0x10);
    fn_801991F8();
L_801A0F9C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801A0FBC | 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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

    /* mr. r30, r3 */;
    if ((s32)r0 != (s32)0) goto L_801A0FE0;
    r31 = 0x0;
    goto L_801A1070;
L_801A0FE0: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_801A0FFC;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_80193748();
    if ((u32)r3 != (u32)0x0) goto L_801A1038;
L_801A0FFC: ;
    r0 = *(u32*)lbl_8047B298;
    if ((u32)r0 == (u32)0x0) goto L_801A1010;
    r3 = *(u32*)lbl_8047B298;
    goto L_801A1018;
L_801A1010: ;
    r3 = (u32)lbl_8036C8E0;
    r3 = (u32)lbl_8036C8E0;
L_801A1018: ;
    fn_80193828();
    /* mr. r31, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_801A1054;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x7df;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
    goto L_801A1054;
L_801A1038: ;
    fn_80193828();
    /* mr. r31, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_801A1054;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x3d5;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1054: ;
    r6 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r4 = r30;
    r5 = 0x0;
    r12 = *(u32*)((u8*)r6 + 0x3C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A1070: ;
    r4 = r30;
    r3 = r31;
    fn_801A0744();
    r3 = r31;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801A1098 | 0x334 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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
    r0 = 0x0;
    /* stmw r27, 0xc(r1) */;
    r29 = r3;
    r31 = (u32)lbl_80274AA0;
    r30 = r4;
    *(u32*)((u8*)r3 + 0x10) = r0;
    r3 = 0x0;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x8) = r3;
    *(u32*)((u8*)r29 + 0xC) = r0;
    r3 = *(u32*)((u8*)r29 + 0x14);
    r0 = *(u32*)((u8*)r4 + 0x4);
    r0 = r3 | r0;
    *(u32*)((u8*)r29 + 0x14) = r0;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00004000;
    if ((s32)r0 == (s32)0) goto L_801A10F8;
    r0 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)((u8*)r29 + 0x18) = r0;
    goto L_801A113C;
L_801A10F8: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00000020;
    if ((s32)r0 == (s32)0) goto L_801A1130;
    r0 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)((u8*)r29 + 0x18) = r0;
    r3 = *(u32*)((u8*)r30 + 0x10);
    goto L_801A1124;
L_801A1114: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 | (0x8000 << 16);
    *(u32*)((u8*)r3 + 0x4) = r0;
    r3 = *(u32*)((u8*)r3 + 0x0);
L_801A1124: ;
    if ((u32)r3 != (u32)0x0) goto L_801A1114;
    goto L_801A113C;
L_801A1130: ;
    r3 = *(u32*)((u8*)r30 + 0x10);
    fn_801992D8();
    *(u32*)((u8*)r29 + 0x18) = r3;
L_801A113C: ;
    r3 = *(u32*)((u8*)r30 + 0x3C);
    fn_801AE5E8();
    r0 = r3;
    r3 = r29 + 0x44;
    *(u32*)((u8*)r29 + 0x80) = r0;
    f0 = *(f32*)((u8*)r30 + 0x14);
    *(f32*)((u8*)r29 + 0x1C) = f0;
    f0 = *(f32*)((u8*)r30 + 0x18);
    *(f32*)((u8*)r29 + 0x20) = f0;
    f0 = *(f32*)((u8*)r30 + 0x1C);
    *(f32*)((u8*)r29 + 0x24) = f0;
    r4 = *(u32*)((u8*)r30 + 0x20);
    r0 = *(u32*)((u8*)r30 + 0x24);
    *(u32*)((u8*)r29 + 0x2C) = r4;
    *(u32*)((u8*)r29 + 0x30) = r0;
    r0 = *(u32*)((u8*)r30 + 0x28);
    *(u32*)((u8*)r29 + 0x34) = r0;
    r4 = *(u32*)((u8*)r30 + 0x2C);
    r0 = *(u32*)((u8*)r30 + 0x30);
    *(u32*)((u8*)r29 + 0x38) = r4;
    *(u32*)((u8*)r29 + 0x3C) = r0;
    r0 = *(u32*)((u8*)r30 + 0x34);
    *(u32*)((u8*)r29 + 0x40) = r0;
    fn_800A2D38();
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x74) = r0;
    r0 = *(u32*)((u8*)r30 + 0x38);
    if ((u32)r0 == (u32)0x0) goto L_801A11C8;
    fn_801A8524();
    *(u32*)((u8*)r29 + 0x78) = r3;
    r5 = 0x30;
    r3 = *(u32*)((u8*)r29 + 0x78);
    r4 = *(u32*)((u8*)r30 + 0x38);
    memcpy((void*)r3, (const void*)r4, (u32)r5);
L_801A11C8: ;
    r4 = r30;
    r5 = r29;
    r3 = 0x0;
    fn_8019C264();
    *(u32*)((u8*)r29 + 0x84) = r30;
    r0 = *(u32*)((u8*)r30 + 0x4);
    r0 = r0 & 0x00001000;
    if ((u32)r0 != (u32)0x0) goto L_801A13B4;
    r30 = *(u32*)((u8*)r30 + 0x8);
    goto L_801A13AC;
L_801A11F0: ;
    if ((u32)r30 != (u32)0x0) goto L_801A1200;
    r28 = 0x0;
    goto L_801A1290;
L_801A1200: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_801A121C;
    r3 = *(u32*)((u8*)r30 + 0x0);
    fn_80193748();
    if ((u32)r3 != (u32)0x0) goto L_801A1258;
L_801A121C: ;
    r0 = *(u32*)lbl_8047B298;
    if ((u32)r0 == (u32)0x0) goto L_801A1230;
    r3 = *(u32*)lbl_8047B298;
    goto L_801A1238;
L_801A1230: ;
    r3 = (u32)lbl_8036C8E0;
    r3 = (u32)lbl_8036C8E0;
L_801A1238: ;
    fn_80193828();
    /* mr. r28, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_801A1274;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x7df;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
    goto L_801A1274;
L_801A1258: ;
    fn_80193828();
    /* mr. r28, r3 */;
    if ((u32)r0 != (u32)0x0) goto L_801A1274;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x3d5;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1274: ;
    r6 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r4 = r30;
    r5 = r29;
    r12 = *(u32*)((u8*)r6 + 0x3C);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A1290: ;
    /* mr. r27, r28 */;
    if ((u32)r0 != (u32)0x0) goto L_801A12A0;
    r3 = -0x1;
    goto L_801A13B8;
L_801A12A0: ;
    if ((u32)r29 == (u32)0x0) goto L_801A13A8;
    if ((u32)r27 == (u32)0x0) goto L_801A13A8;
    r0 = *(u32*)((u8*)r27 + 0xC);
    if ((u32)r0 == (u32)0x0) goto L_801A12D8;
    r3 = r31 + 0x178;
    /* crclr cr1eq */;
    OSReport();
    r5 = r31 + 0x194;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x552;
    fn_80196E10();
L_801A12D8: ;
    r0 = *(u32*)((u8*)r27 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_801A1300;
    r3 = r31 + 0x1ac;
    /* crclr cr1eq */;
    OSReport();
    r5 = r31 + 0x1cc;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x553;
    fn_80196E10();
L_801A1300: ;
    r0 = *(u32*)((u8*)r29 + 0x10);
    if ((u32)r0 != (u32)0x0) goto L_801A1314;
    *(u32*)((u8*)r29 + 0x10) = r27;
    goto L_801A1364;
L_801A1314: ;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r0 == (u32)0x0) goto L_801A1330;
    r5 = r31 + 0x1e0;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x559;
    fn_80196E10();
L_801A1330: ;
    r28 = *(u32*)((u8*)r29 + 0x10);
    goto L_801A1354;
L_801A1338: ;
    if ((u32)r28 != (u32)r27) goto L_801A1350;
    r5 = r31 + 0x200;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x55c;
    fn_80196E10();
L_801A1350: ;
    r28 = *(u32*)((u8*)r28 + 0x8);
L_801A1354: ;
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((u32)r0 != (u32)0x0) goto L_801A1338;
    *(u32*)((u8*)r28 + 0x8) = r27;
L_801A1364: ;
    *(u32*)((u8*)r27 + 0xC) = r29;
    r4 = r29;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r3 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 << 10;
    r0 = r3 | r0;
    r3 = r0 & 0x70000000;
    goto L_801A13A0;
L_801A1384: ;
    r0 = *(u32*)((u8*)r4 + 0x14);
    /* andc. r0, r3, r0 */;
    if ((u32)r0 == (u32)0x0) goto L_801A13A8;
    r0 = *(u32*)((u8*)r4 + 0x14);
    r0 = r0 | r3;
    *(u32*)((u8*)r4 + 0x14) = r0;
    r4 = *(u32*)((u8*)r4 + 0xC);
L_801A13A0: ;
    if ((u32)r4 != (u32)0x0) goto L_801A1384;
L_801A13A8: ;
    r30 = *(u32*)((u8*)r30 + 0xC);
L_801A13AC: ;
    if ((u32)r30 != (u32)0x0) goto L_801A11F0;
L_801A13B4: ;
    r3 = 0x0;
L_801A13B8: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x801A13CC | 0x5B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
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
    u32 r0 = 0;
    u32 r1 = (u32)sp;
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

    /* stmw r26, 0x158(r1) */;
    /* mr. r27, r3 */;
    r28 = r4;
    r29 = r5;
    r30 = r6;
    if ((s32)r0 == (s32)0) goto L_801A196C;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00001000;
    if ((s32)r0 == (s32)0) goto L_801A1704;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00000010;
    if ((s32)r0 != (s32)0) goto L_801A196C;
    if ((u32)r27 == (u32)0x0) goto L_801A1458;
    if ((u32)r27 != (u32)0x0) goto L_801A1428;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1428: ;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r27 != (u32)0x0) goto L_801A1448;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r27 == (u32)0x0) goto L_801A1448;
    r3 = 0x1;
L_801A1448: ;
    if ((s32)r3 == (s32)0x0) goto L_801A1458;
    r3 = r27;
    fn_8019D9DC();
L_801A1458: ;
    r31 = *(u32*)((u8*)r27 + 0x10);
    if ((u32)r31 == (u32)0x0) goto L_801A14AC;
    if ((u32)r31 != (u32)0x0) goto L_801A147C;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A147C: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r31 != (u32)0x0) goto L_801A149C;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r31 == (u32)0x0) goto L_801A149C;
    r3 = 0x1;
L_801A149C: ;
    if ((s32)r3 == (s32)0x0) goto L_801A14AC;
    r3 = r31;
    fn_8019D9DC();
L_801A14AC: ;
    r3 = *(u32*)((u8*)r27 + 0x10);
    r4 = r1 + 0x128;
    r3 = r3 + 0x44;
    fn_800A2EB4();
    r3 = r27 + 0x44;
    r4 = r1 + 0x128;
    r5 = r1 + 0x128;
    fn_800A2D98();
    if ((u32)r28 == (u32)0x0) goto L_801A14E8;
    r3 = r28;
    r4 = r1 + 0x128;
    r5 = r1 + 0x128;
    fn_800A2D98();
    goto L_801A1504;
L_801A14E8: ;
    fn_801942B8();
    if ((u32)r3 == (u32)0x0) goto L_801A1504;
    r4 = r1 + 0x128;
    r3 = r3 + 0x54;
    r5 = r1 + 0x128;
    fn_800A2D98();
L_801A1504: ;
    r31 = *(u32*)((u8*)r27 + 0x10);
    if ((u32)r31 == (u32)0x0) goto L_801A196C;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_801A1614;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_801A196C;
    r3 = r31;
    fn_801A1988();
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801A1988();
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = r1 + 0xf8;
    r3 = r3 + 0x44;
    fn_800A2EB4();
    r3 = r31 + 0x44;
    r4 = r1 + 0xf8;
    r5 = r1 + 0xf8;
    fn_800A2D98();
    r3 = r1 + 0x128;
    r4 = r1 + 0xf8;
    r5 = r1 + 0xf8;
    fn_800A2D98();
    r27 = *(u32*)((u8*)r31 + 0x10);
    if ((u32)r27 == (u32)0x0) goto L_801A196C;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r27 == (u32)0x0) goto L_801A15B4;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r27 != (u32)0x0) goto L_801A196C;
    r4 = r27;
    r3 = r1 + 0xf8;
    r5 = r1 + 0x98;
    fn_801A1A00();
    r3 = *(u32*)((u8*)r27 + 0x10);
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0x98;
    fn_801A13CC();
    goto L_801A196C;
L_801A15B4: ;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r0 = r29 << 18;
    /* and. r0, r3, r0 */;
    if ((u32)r27 == (u32)0x0) goto L_801A15D8;
    r3 = r27;
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0xf8;
    fn_80197344();
L_801A15D8: ;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r0 = r29 << 28;
    /* and. r0, r3, r0 */;
    if ((u32)r27 == (u32)0x0) goto L_801A196C;
    r27 = *(u32*)((u8*)r27 + 0x10);
    goto L_801A1608;
L_801A15F0: ;
    r3 = r27;
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0xf8;
    fn_801A13CC();
    r27 = *(u32*)((u8*)r27 + 0x8);
L_801A1608: ;
    if ((u32)r27 != (u32)0x0) goto L_801A15F0;
    goto L_801A196C;
L_801A1614: ;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r29 << 18;
    /* and. r0, r3, r0 */;
    if ((u32)r27 == (u32)0x0) goto L_801A1638;
    r3 = r31;
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0x128;
    fn_80197344();
L_801A1638: ;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r29 << 28;
    /* and. r0, r3, r0 */;
    if ((u32)r27 == (u32)0x0) goto L_801A196C;
    r31 = *(u32*)((u8*)r31 + 0x10);
    goto L_801A16F8;
L_801A1650: ;
    /* mr. r27, r31 */;
    if ((u32)r27 == (u32)0x0) goto L_801A16F4;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r27 == (u32)0x0) goto L_801A1698;
    r0 = *(u32*)((u8*)r27 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r27 != (u32)0x0) goto L_801A16F4;
    r4 = r27;
    r3 = r1 + 0x128;
    r5 = r1 + 0x68;
    fn_801A1A00();
    r3 = *(u32*)((u8*)r27 + 0x10);
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0x68;
    fn_801A13CC();
    goto L_801A16F4;
L_801A1698: ;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r0 = r29 << 18;
    /* and. r0, r3, r0 */;
    if ((u32)r27 == (u32)0x0) goto L_801A16BC;
    r3 = r27;
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0x128;
    fn_80197344();
L_801A16BC: ;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r0 = r29 << 28;
    /* and. r0, r3, r0 */;
    if ((u32)r27 == (u32)0x0) goto L_801A16F4;
    r27 = *(u32*)((u8*)r27 + 0x10);
    goto L_801A16EC;
L_801A16D4: ;
    r3 = r27;
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0x128;
    fn_801A13CC();
    r27 = *(u32*)((u8*)r27 + 0x8);
L_801A16EC: ;
    if ((u32)r27 != (u32)0x0) goto L_801A16D4;
L_801A16F4: ;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_801A16F8: ;
    if ((u32)r31 != (u32)0x0) goto L_801A1650;
    goto L_801A196C;
L_801A1704: ;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r0 = r29 << 18;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1728;
    r3 = r27;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    fn_80197344();
L_801A1728: ;
    r3 = *(u32*)((u8*)r27 + 0x14);
    r0 = r29 << 28;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A196C;
    r27 = *(u32*)((u8*)r27 + 0x10);
    goto L_801A1964;
L_801A1740: ;
    /* mr. r31, r27 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1960;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_801A1874;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_801A1960;
    r3 = r31;
    fn_801A1988();
    r3 = *(u32*)((u8*)r31 + 0x10);
    fn_801A1988();
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = r1 + 0xc8;
    r3 = r3 + 0x44;
    fn_800A2EB4();
    r3 = r31 + 0x44;
    r4 = r1 + 0xc8;
    r5 = r1 + 0xc8;
    fn_800A2D98();
    if ((u32)r28 == (u32)0x0) goto L_801A17AC;
    r3 = r28;
    r4 = r1 + 0xc8;
    r5 = r1 + 0xc8;
    fn_800A2D98();
    goto L_801A17C8;
L_801A17AC: ;
    fn_801942B8();
    if ((u32)r3 == (u32)0x0) goto L_801A17C8;
    fn_801A1980();
    r4 = r1 + 0xc8;
    r5 = r1 + 0xc8;
    fn_800A2D98();
L_801A17C8: ;
    r31 = *(u32*)((u8*)r31 + 0x10);
    if ((u32)r31 == (u32)0x0) goto L_801A1960;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_801A1814;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_801A1960;
    r4 = r31;
    r3 = r1 + 0xc8;
    r5 = r1 + 0x38;
    fn_801A1A00();
    r3 = *(u32*)((u8*)r31 + 0x10);
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0x38;
    fn_801A13CC();
    goto L_801A1960;
L_801A1814: ;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r29 << 18;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1838;
    r3 = r31;
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0xc8;
    fn_80197344();
L_801A1838: ;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r29 << 28;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1960;
    r31 = *(u32*)((u8*)r31 + 0x10);
    goto L_801A1868;
L_801A1850: ;
    r3 = r31;
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0xc8;
    fn_801A13CC();
    r31 = *(u32*)((u8*)r31 + 0x8);
L_801A1868: ;
    if ((u32)r31 != (u32)0x0) goto L_801A1850;
    goto L_801A1960;
L_801A1874: ;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r29 << 18;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1898;
    r3 = r31;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    fn_80197344();
L_801A1898: ;
    r3 = *(u32*)((u8*)r31 + 0x14);
    r0 = r29 << 28;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1960;
    r31 = *(u32*)((u8*)r31 + 0x10);
    goto L_801A1958;
L_801A18B0: ;
    /* mr. r26, r31 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1954;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x00001000;
    if ((u32)r31 == (u32)0x0) goto L_801A18F8;
    r0 = *(u32*)((u8*)r26 + 0x14);
    r0 = r0 & 0x00000010;
    if ((u32)r31 != (u32)0x0) goto L_801A1954;
    r3 = r28;
    r4 = r26;
    r5 = r1 + 0x8;
    fn_801A1A00();
    r3 = *(u32*)((u8*)r26 + 0x10);
    r5 = r29;
    r6 = r30;
    r4 = r1 + 0x8;
    fn_801A13CC();
    goto L_801A1954;
L_801A18F8: ;
    r3 = *(u32*)((u8*)r26 + 0x14);
    r0 = r29 << 18;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A191C;
    r3 = r26;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    fn_80197344();
L_801A191C: ;
    r3 = *(u32*)((u8*)r26 + 0x14);
    r0 = r29 << 28;
    /* and. r0, r3, r0 */;
    if ((u32)r31 == (u32)0x0) goto L_801A1954;
    r26 = *(u32*)((u8*)r26 + 0x10);
    goto L_801A194C;
L_801A1934: ;
    r3 = r26;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    fn_801A13CC();
    r26 = *(u32*)((u8*)r26 + 0x8);
L_801A194C: ;
    if ((u32)r26 != (u32)0x0) goto L_801A1934;
L_801A1954: ;
    r31 = *(u32*)((u8*)r31 + 0x8);
L_801A1958: ;
    if ((u32)r31 != (u32)0x0) goto L_801A18B0;
L_801A1960: ;
    r27 = *(u32*)((u8*)r27 + 0x8);
L_801A1964: ;
    if ((u32)r27 != (u32)0x0) goto L_801A1740;
L_801A196C: ;
    /* lmw r26, 0x158(r1) */;
    return;
}
#pragma pop
