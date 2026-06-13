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
extern void fn_801A4440();
extern void LObjUpdateFunc();

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

void HSD_LObjSetFlags_Early(HSD_LObj* lobj, u32 flags)
{
    HSD_ASSERT(0, lobj);
    lobj->flags |= (u16) flags;
}

void HSD_LObjClearFlags_Early(HSD_LObj* lobj, u32 flags)
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

s32 HSD_LObjGetNbActive_Early(void)
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

void HSD_LObjAddAnimAll_Early(HSD_LObj* lobj, HSD_LightAnim* lanim)
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

void HSD_LObjAnimAll_Early(HSD_LObj* lobj)
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

void HSD_LObjReqAnimAll_Early(HSD_LObj* lobj, f32 startframe)
{
    HSD_LObj* l;
    for (l = lobj; l != NULL; l = l->next) {
        HSD_LObjReqAnim(l, startframe);
    }
}

/* ========================================================================= */
/*  Color / position                                                         */
/* ========================================================================= */

void HSD_LObjSetColor_Early(HSD_LObj* lobj, u32 color)
{
    HSD_ASSERT(0, lobj);
    lobj->color = color;
}

void HSD_LObjSetPosition_Early(HSD_LObj* lobj, f32 x, f32 y, f32 z)
{
    HSD_ASSERT(0, lobj);
    HSD_WObjSetPosition_Early(lobj->position, x, y, z);
}

void HSD_LObjSetInterest_Early(HSD_LObj* lobj, f32 x, f32 y, f32 z)
{
    HSD_ASSERT(0, lobj);
    HSD_WObjSetPosition_Early(lobj->interest, x, y, z);
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
    HSD_LOBJ_INFO(&hsdLObj)->load =
        (int (*)(HSD_LObj*, HSD_LightDesc*)) fn_801A4440;
    HSD_LOBJ_INFO(&hsdLObj)->update =
        (void (*)(HSD_LObj*, u32, void*)) LObjUpdateFunc;
}

/* 0x801A4098 | 0x60 */
extern u32 lbl_8047B2B0;
extern u8 lbl_8036CA20[];
extern u32 lbl_8047B2B4;
#if 1
asm void fn_801A4098(void) {
#include "src/hsd/hsd_lobj_fn_801A4098.inc"
}
#else
#pragma push
#pragma peephole off
extern u32 lbl_8047B2B0;
extern u32 lbl_8047B2B4;
extern u8 lbl_8036CA20[];
extern void fn_801C25E4(HSD_AObj*);
void fn_801A4098(void* param) {
    if (param == (void*)lbl_8047B2B0) {
        lbl_8047B2B0 = 0;
    }
    if (param == (void*)lbl_8036CA20) {
        lbl_8047B2B4 = 0;
    }
    {
        void* ptr = (void*)lbl_8036CA20;
        void* vtable = *(void**)((u8*)ptr + 0x14);
        void (*func)(void) = *(void(**)(void))((u8*)vtable + 0x38);
        func();
    }
}
#pragma pop
#endif

/* 0x801A40F8 | 0x174 */
#if 0
asm void fn_801A40F8(void) {
#include "src/hsd/hsd_lobj_fn_801A40F8.inc"
}
#else
void fn_801A40F8(HSD_LObj* lobj)
{
    fn_801C25E4(lobj->aobj);
    HSD_WObjUnref(lobj != NULL ? lobj->position : NULL);
    HSD_WObjUnref(lobj != NULL ? lobj->interest : NULL);
    HSD_OBJECT_PARENT_INFO(&hsdLObj)->release((HSD_Class*) lobj);
}
#endif

/* 0x801A426C | 0xD8 */
extern HSD_AObj* fn_801C2670(HSD_AObjDesc*);
#if 0
asm void HSD_LObjAddAnimAll(void) {
#include "src/hsd/hsd_lobj_HSD_LObjAddAnimAll.inc"
}
#else
void HSD_LObjAddAnimAll(HSD_LObj* lobj, HSD_LightAnim* lanim)
{
    HSD_LObj* l;
    HSD_LightAnim* a;

    if (lobj == NULL) {
        return;
    }
    l = lobj;
    a = lanim;
    while (l != NULL) {
        if (l != NULL && a != NULL) {
            if (*(volatile u32*)((u8*)l + 0x48) != 0) {
                fn_801C25E4(l->aobj);
            }
            l->aobj = fn_801C2670(a->aobjdesc);
            HSD_WObjAddAnim(l != NULL ? l->position : NULL,
                            a->position_anim);
            HSD_WObjAddAnim(l != NULL ? l->interest : NULL,
                            a->interest_anim);
        }
        l = l != NULL ? l->next : NULL;
        a = a != NULL ? a->next : NULL;
    }
}
#endif

/* 0x801A4344 | 0xFC */
extern void fn_80193748(void);
extern void fn_80193828(void);
extern void __assert();
extern u32 lbl_8047B2B0;
extern char lbl_8047DBB8;
extern char lbl_8047DBC0;
extern char lbl_8047DBC4;
#if 1
asm void fn_801A4344(void) {
#include "src/hsd/hsd_lobj_fn_801A4344.inc"
}
#else
void fn_801A4344(void) {}
#endif

/* 0x801A4440 | 0x470 */
extern HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc);
extern void HSD_Panic(void);
extern void OSReport();
/* renamed symbols referenced by asm incs (symbolmap port) */
extern void GXInitLightSpot();
extern void GXInitLightDistAttn();
extern u8 lbl_80274D94[];
extern char lbl_8047DBC8;
#if 1
asm void fn_801A4440(void) {
#include "src/hsd/hsd_lobj_fn_801A4440.inc"
}
#else
void fn_801A4440(void) {}
#endif

/* 0x801A48B0 | 0x44 */
extern void fn_80191688();
/* HSD_LObjGetInterest */
s32 HSD_LObjGetInterest(HSD_LObj* lobj, void* out) {
    if (lobj != NULL) {
        if (*(volatile u32*)((u8*)lobj + 0x1C) != 0) {
            fn_80191688(*(void**)((u8*)lobj + 0x1C), out);
            return 1;
        }
    }
    return 0;
}

/* 0x801A48F4 | 0x88 */
extern void HSD_WObjSetPosition(HSD_WObj* wobj, void* position);
extern char lbl_8047DBCC;
extern u8 lbl_80274DB8[];
extern char lbl_8047DBB8;
#if 0
asm void HSD_LObjSetInterest(void) {
#include "src/hsd/hsd_lobj_HSD_LObjSetInterest.inc"
}
#else
#pragma push
#pragma scheduling on
void HSD_LObjSetInterest(HSD_LObj* lobj, void* desc) {
    if (lobj == NULL) {
        __assert(&lbl_8047DBB8, 0x58c, &lbl_8047DBCC);
    }
    if (lobj->interest == NULL) {
        lobj->interest = HSD_WObjAlloc();
        if (lobj->interest == NULL) {
            __assert(&lbl_8047DBB8, 0x58f, lbl_80274DB8);
        }
    }
    HSD_WObjSetPosition(lobj->interest, desc);
}
#pragma pop
#endif

/* 0x801A497C | 0x44 */
#if 0
asm void HSD_LObjGetPosition(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetPosition.inc"
}
#else
s32 HSD_LObjGetPosition(HSD_LObj* lobj, void* out) {
    if (lobj != NULL) {
        if (*(volatile u32*)((u8*)lobj + 0x18) != 0) {
            fn_80191688(*(void**)((u8*)lobj + 0x18), out);
            return 1;
        }
    }
    return 0;
}
#endif

/* 0x801A49C0 | 0x88 */
extern u8 lbl_80274DC8[];
#if 0
asm void HSD_LObjSetPosition(void) {
#include "src/hsd/hsd_lobj_HSD_LObjSetPosition.inc"
}
#else
void HSD_LObjSetPosition(HSD_LObj* lobj, void* desc) {
    if (lobj == NULL) {
        __assert(&lbl_8047DBB8, 0x568, &lbl_8047DBCC);
    }
    if (lobj->position == NULL) {
        lobj->position = HSD_WObjAlloc();
        if (lobj->position == NULL) {
            __assert(&lbl_8047DBB8, 0x56b, lbl_80274DC8);
        }
    }
    HSD_WObjSetPosition(lobj->position, desc);
}
#endif

/* 0x801A4A48 | 0xC */
#if 0
asm void HSD_LObjSetColor(void) {
#include "src/hsd/hsd_lobj_HSD_LObjSetColor.inc"
}
#else
void HSD_LObjSetColor(HSD_LObj* lobj, u32* color) {
    lobj->color = *color;
}
#endif

/* 0x801A4A54 | 0x70 */
extern void jumptable_8036CA64();
/* GXLightIndex to GXLightID bitmask */
#if 1
asm void fn_801A4A54(void) {
#include "src/hsd/hsd_lobj_fn_801A4A54.inc"
}
#else
u32 fn_801A4A54(u32 idx) {
    switch (idx) {
    case 0: return 0x1;
    case 1: return 0x2;
    case 2: return 0x4;
    case 3: return 0x8;
    case 4: return 0x10;
    case 5: return 0x20;
    case 6: return 0x40;
    case 7: return 0x80;
    case 8: return 0x100;
    }
    return 0;
}
#endif

/* 0x801A4AC4 | 0x3C */
extern u32 lbl_8047B2B4;
#if 0
asm void HSD_LObjGetCurrentByType(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetCurrentByType.inc"
}
#else
#pragma push
#pragma peephole off
/* Find active light by type */
HSD_LObj* HSD_LObjGetCurrentByType(u32 type) {
    void* data;
    HSD_SList* p;
    type &= 3;
    for (p = (HSD_SList*) lbl_8047B2B4; p != NULL; p = p->next) {
        data = ((volatile HSD_SList*) p)->data;
        if (type != (((HSD_LObj*) data)->flags & 3)) {
            continue;
        }
        return (HSD_LObj*) ((volatile HSD_SList*) p)->data;
    }
    return NULL;
}
#pragma pop
#endif

/* 0x801A4B00 | 0x220 */
extern void fn_801A3E64(void);
extern u8 lbl_804655E0[];
extern u32 lbl_8047B2B8;
#if 1
asm void HSD_LObjDeleteCurrentAll(HSD_LObj* lobj) {
#include "src/hsd/hsd_lobj_HSD_LObjDeleteCurrentAll.inc"
}
#else
void HSD_LObjDeleteCurrentAll(HSD_LObj* lobj) {}
#endif

/* 0x801A4D20 | 0x234 */
extern void fn_801A3EB4(void);
extern u8 lbl_80274DD8[];
extern u8 lbl_80274DE4[];
extern char lbl_8047DBD8;
#if 1
asm void fn_801A4D20(void) {
#include "src/hsd/hsd_lobj_fn_801A4D20.inc"
}
#else
void fn_801A4D20(void) {}
#endif

/* 0x801A4F54 | 0xE78 */
extern void fn_800A37CC(void);
extern void fn_800BA414(void);
extern void fn_800BA198(void);
extern void fn_800BA440(void);
extern void fn_800BA344(void);
extern void fn_800BA1B4(void);
extern void fn_800A3A9C(void*, void*, void*);
extern void fn_800A3ADC(void*, void*);
extern void fn_800A3820(void);
extern void fn_800BA424(void);
extern void fn_800BA44C(void);
extern void jumptable_8036CAD0();
extern void jumptable_8036CAAC();
extern void jumptable_8036CA88();
extern u32 lbl_8047B2BC;
extern u32 lbl_8047B2C0;
extern u32 lbl_8047B2C4;
extern u32 lbl_8047B2C8;
extern u32 lbl_8047B2B8;
extern u32 lbl_8047DBE0;
extern u32 lbl_8047DBE4;
extern u32 lbl_8047DBE8;
extern u8 lbl_80274D58[];
extern u8 lbl_80274D64[];
extern u8 lbl_80478AC8[];
extern u32 lbl_8047DBF0;
extern u32 lbl_8047DBEC;
extern char lbl_8047DBD4;
extern u32 lbl_8047DBF8;
extern u32 lbl_8047DBFC;
#if 1
asm void HSD_LObjSetup(void* setup) {
#include "src/hsd/hsd_lobj_HSD_LObjSetup.inc"
}
#else
void HSD_LObjSetup(void* setup) {}
#endif

/* 0x801A6098 | 0x174 */
extern u32 lbl_8047DBFC;
extern u32 lbl_8047DBE4;
extern u32 lbl_8047DBE8;
#if 1
asm void fn_801A6098(void) {
#include "src/hsd/hsd_lobj_fn_801A6098.inc"
}
#else
void fn_801A6098(void) {}
#endif

/* 0x801A620C | 0x164 */
extern u32 lbl_8047DBE8;
extern u32 lbl_8047DBE4;
#if 0
asm void fn_801A620C(void) {
#include "src/hsd/hsd_lobj_fn_801A620C.inc"
}
#else
void fn_801A620C(HSD_LObj* lobj, f32* out)
{
    u32 pos[3];
    u32 interest[3];
    s32 invalid;

    pos[0] = *(u32*)&lbl_80274D58[0];
    pos[1] = *(u32*)&lbl_80274D58[4];
    pos[2] = *(u32*)&lbl_80274D58[8];
    interest[0] = *(u32*)&lbl_80274D64[0];
    interest[1] = *(u32*)&lbl_80274D64[4];
    interest[2] = *(u32*)&lbl_80274D64[8];

    if (lobj != NULL) {
        if (lobj != NULL && *(volatile u32*)((u8*)lobj + 0x18) != 0) {
            fn_80191688(lobj->position, pos);
        }
        if (lobj != NULL && *(volatile u32*)((u8*)lobj + 0x1C) != 0) {
            fn_80191688(lobj->interest, interest);
        }
        fn_800A3A9C(interest, pos, out);

        if (out == NULL) {
            goto invalid_vec;
        }
        if (out != NULL) {
            goto check_vec;
        }
    invalid_vec:
        invalid = -1;
        goto checked_vec;
    check_vec:
        if (__fabs(out[0]) <= *(volatile f32*)&lbl_80478AC8 &&
            __fabs(out[1]) <= *(volatile f32*)&lbl_80478AC8 &&
            __fabs(out[2]) <= *(volatile f32*)&lbl_80478AC8) {
            invalid = -1;
        } else {
            fn_800A3ADC(out, out);
            invalid = 0;
        }

    checked_vec:
        if (invalid != 0) {
            f32 x = *(f32*)&lbl_8047DBE8;
            f32 y = *(f32*)&lbl_8047DBE8;
            f32 z = *(f32*)&lbl_8047DBE4;
            out[0] = x;
            out[1] = y;
            out[2] = z;
        }
    }
}
#endif

/* 0x801A6370 | 0x98 */
extern void fn_801C29C4();
#if 0
asm void HSD_LObjReqAnimAll(void) {
#include "src/hsd/hsd_lobj_HSD_LObjReqAnimAll.inc"
}
#else
/* HSD_LObjReqAnimAll - request animation start for all lobjs in list */
void HSD_LObjReqAnimAll(HSD_LObj* lobj, f32 startframe) {
    HSD_LObj* l;
    if (lobj == NULL) return;
    for (l = lobj; l != NULL; l = l->next) {
        if (l == NULL) continue;
        fn_801C29C4(l->aobj, startframe);
        HSD_WObjReqAnim(l != NULL ? l->position : NULL, startframe);
        HSD_WObjReqAnim(l != NULL ? l->interest : NULL, startframe);
    }
}
#endif

/* 0x801A6408 | 0x8C */
extern void fn_801C27F4();
#if 0
asm void HSD_LObjAnimAll(void) {
#include "src/hsd/hsd_lobj_HSD_LObjAnimAll.inc"
}
#else
/* HSD_LObjAnimAll - animate all lobjs in list */
void HSD_LObjAnimAll(HSD_LObj* lobj) {
    HSD_LObj* l;
    if (lobj == NULL) return;
    for (l = lobj; l != NULL; l = l->next) {
        if (l == NULL) continue;
        fn_801C27F4(l->aobj, l, HSD_LOBJ_METHOD(l)->update);
        HSD_WObjInterpretAnim(l != NULL ? l->position : NULL);
        HSD_WObjInterpretAnim(l != NULL ? l->interest : NULL);
    }
}
#endif

/* 0x801A6494 | 0x24C */
extern void jumptable_8036CAF4();
extern u32 lbl_8047DC08;
extern u32 lbl_8047DBE8;
extern u32 lbl_8047DBE4;
extern u32 lbl_8047DC10;
#if 1
asm void LObjUpdateFunc(void) {
#include "src/hsd/hsd_lobj_LObjUpdateFunc.inc"
}
#else
void LObjUpdateFunc(void) {}
#endif

/* 0x801A66E0 | 0xAC */
extern void HSD_WObjRemoveAnim(HSD_WObj* wobj);
#if 1
asm void HSD_LObjRemoveAnimAll(HSD_LObj* lobj) {
#include "src/hsd/hsd_lobj_HSD_LObjRemoveAnimAll.inc"
}
#else
void HSD_LObjRemoveAnimAll(HSD_LObj* lobj) {}
#endif

/* 0x801A678C | 0x30 */
#if 0
asm void HSD_LObjGetActiveByIndex(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetActiveByIndex.inc"
}
#else
/* Get active light by index from lbl_804655E0 table */
extern u8 lbl_804655E0[];
#pragma optimization_level 1
HSD_LObj* HSD_LObjGetActiveByIndex(s32 idx) {
    if (idx >= 0) {
        if (idx < 8) {
            return *(HSD_LObj**)(lbl_804655E0 + idx * 4);
        }
    }
    return NULL;
}
#endif

/* 0x801A67BC | 0x114 */
#if 0
asm void HSD_LObjGetActiveByID(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetActiveByID.inc"
}
#else
HSD_LObj* HSD_LObjGetActiveByID(s32 light_id)
{
    s32 index;

    switch (light_id) {
        case 1:
            index = 0;
            break;
        case 2:
            index = 1;
            break;
        case 4:
            index = 2;
            break;
        case 8:
            index = 3;
            break;
        case 0x10:
            index = 4;
            break;
        case 0x20:
            index = 5;
            break;
        case 0x40:
            index = 6;
            break;
        case 0x80:
            index = 7;
            break;
        case 0x100:
            index = 8;
            break;
        default:
            __assert(&lbl_8047DBB8, 0x4A1, &lbl_8047DBD4);
            break;
    }

    {
        s32 checked;

        checked = index;
        if (checked >= 0 && checked < 9) {
            return *(HSD_LObj**)(lbl_804655E0 + checked * 4);
        }
    }
    return NULL;
}
#endif

/* 0x801A68D0 | 0x8 */
extern u32 lbl_8047B2B8;
#if 0
asm void HSD_LObjGetNbActive(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetNbActive.inc"
}
#else
s32 HSD_LObjGetNbActive(void) {
    return lbl_8047B2B8;
}
#endif

/* 0x801A68D8 | 0x8 */
extern u32 lbl_8047B2C0;
#if 0
asm void HSD_LObjGetLightMaskSpecular(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetLightMaskSpecular.inc"
}
#else
u32 HSD_LObjGetLightMaskSpecular(void) {
    return lbl_8047B2C0;
}
#endif

/* 0x801A68E0 | 0x8 */
extern u32 lbl_8047B2C8;
#if 0
asm void HSD_LObjGetLightMaskAlpha(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetLightMaskAlpha.inc"
}
#else
u32 HSD_LObjGetLightMaskAlpha(void) {
    return lbl_8047B2C8;
}
#endif

/* 0x801A68E8 | 0x8 */
extern u32 lbl_8047B2C4;
#if 0
asm void HSD_LObjGetLightMaskAttnFunc(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetLightMaskAttnFunc.inc"
}
#else
u32 HSD_LObjGetLightMaskAttnFunc(void) {
    return lbl_8047B2C4;
}
#endif

/* 0x801A68F0 | 0x8 */
extern u32 lbl_8047B2BC;
#if 0
asm void HSD_LObjGetLightMaskDiffuse(void) {
#include "src/hsd/hsd_lobj_HSD_LObjGetLightMaskDiffuse.inc"
}
#else
u32 HSD_LObjGetLightMaskDiffuse(void) {
    return lbl_8047B2BC;
}
#endif

/* 0x801A68F8 | 0x18 */
#if 0
asm void HSD_LObjClearFlags(void) {
#include "src/hsd/hsd_lobj_HSD_LObjClearFlags.inc"
}
#else
void HSD_LObjClearFlags(HSD_LObj* lobj, u32 flags) {
    if (lobj == NULL) return;
    lobj->flags &= ~flags;
}
#endif

/* 0x801A6910 | 0x18 */
#if 0
asm void HSD_LObjSetFlags(void) {
#include "src/hsd/hsd_lobj_HSD_LObjSetFlags.inc"
}
#else
void HSD_LObjSetFlags(HSD_LObj* lobj, u32 flags) {
    if (lobj == NULL) return;
    lobj->flags |= flags;
}
#endif

/* 0x801A6990 | 0x30 */
extern u8 lbl_80465608[];
#if 0
asm void fn_801A6990(void) {
#include "src/hsd/hsd_lobj_fn_801A6990.inc"
}
#else
/* Call virtual dispatch via lbl_80465608 */
typedef void (*fn_ptr)(void*);
void fn_801A6990(HSD_LObj* lobj) {
    fn_ptr func;
    func = ((fn_ptr*) lbl_80465608)[4];
    func(lobj);
}
#endif
