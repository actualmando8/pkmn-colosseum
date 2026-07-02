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
static int WObjLoad_Early(HSD_WObj* wobj, HSD_WObjDesc* desc);
int WObjLoad(HSD_WObj* wobj, HSD_WObjDesc* desc);
void WObjAmnesia(HSD_ClassInfo* info);
void WObjRelease(HSD_WObj* wobj);
void WObjUpdateFunc(HSD_WObj* wobj, u32 type, void* value);

HSD_WObjInfo hsdWObj = { WObjInfoInit };

static HSD_ClassInfo* lbl_8047B218 = NULL;

/* ========================================================================= */
/*  Animation                                                                */
/* ========================================================================= */

void HSD_WObjRemoveAnim_Early(HSD_WObj* wobj)
{
    if (wobj != NULL) {
        HSD_AObjRemove(wobj->aobj);
        wobj->aobj = NULL;
        HSD_RObjRemoveAnimAll(wobj->robj);
    }
}

void HSD_WObjReqAnim_Early(HSD_WObj* wobj, f32 frame)
{
    if (wobj != NULL) {
        HSD_AObjReqAnim(wobj->aobj, frame);
        HSD_RObjReqAnimAll(wobj->robj, frame);
    }
}

void HSD_WObjAddAnim_Early(HSD_WObj* wobj, HSD_WObjAnim* anim)
{
    if (wobj != NULL && anim != NULL) {
        if (wobj->aobj != NULL) {
            HSD_AObjRemove(wobj->aobj);
        }
        wobj->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
        HSD_RObjAddAnimAll(wobj->robj, anim->robjanim);
    }
}

void HSD_WObjInterpretAnim_Early(HSD_WObj* wobj)
{
    if (wobj != NULL) {
        /* WObjUpdateFunc callback handles position animation */
        HSD_RObjAnimAll(wobj->robj);
    }
}

/* ========================================================================= */
/*  Load / Init                                                              */
/* ========================================================================= */

static int WObjLoad_Early(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    HSD_WObjSetPosition_Early(wobj, desc->pos_x, desc->pos_y, desc->pos_z);
    if (wobj->robj != NULL) {
        HSD_RObjRemoveAll(wobj->robj);
    }
    wobj->robj = HSD_RObjLoadDesc(desc->robjdesc);
    HSD_RObjResolveRefsAll(wobj->robj, desc->robjdesc);
    return 0;
}

void HSD_WObjInit_Early(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    if (wobj == NULL || desc == NULL) {
        return;
    }

    HSD_WObjSetPosition_Early(wobj, desc->pos_x, desc->pos_y, desc->pos_z);
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
    lbl_8047B218 = info;
}

/* ========================================================================= */
/*  Load from descriptor                                                     */
/* ========================================================================= */

HSD_WObj* HSD_WObjLoadDesc_Early(HSD_WObjDesc* desc)
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

void HSD_WObjSetPosition_Early(HSD_WObj* wobj, f32 x, f32 y, f32 z)
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

void HSD_WObjGetPosition_Early(HSD_WObj* wobj, f32* x, f32* y, f32* z)
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

HSD_WObj* HSD_WObjAlloc_Early(void)
{
    HSD_WObj* wobj = (HSD_WObj*) hsdNew(
        lbl_8047B218 ? lbl_8047B218 : &hsdWObj.parent.parent);
    HSD_ASSERT(591, wobj);
    return wobj;
}

/* ========================================================================= */
/*  Class lifecycle                                                          */
/* ========================================================================= */

static void WObjRelease_Early(HSD_Class* o)
{
    HSD_WObj* wobj = (HSD_WObj*) o;
    HSD_RObjRemoveAll(wobj->robj);
    HSD_AObjRemove(wobj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdWObj)->release(o);
}

static void WObjAmnesia_Early(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(lbl_8047B218)) {
        lbl_8047B218 = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdWObj)->amnesia(info);
}

static void WObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdWObj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "had_wobj",
                     sizeof(HSD_WObjInfo), sizeof(HSD_WObj));
    HSD_CLASS_INFO(&hsdWObj)->release = (void (*)(HSD_Class*)) WObjRelease;
    HSD_CLASS_INFO(&hsdWObj)->amnesia = WObjAmnesia;
    HSD_WOBJ_INFO(&hsdWObj)->load = WObjLoad;
    HSD_WOBJ_INFO(&hsdWObj)->update = WObjUpdateFunc;
}

/* 0x8019158C | 0x48 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 1
#pragma optimization_level 4
void WObjAmnesia(HSD_ClassInfo* info)
{
    extern u8 lbl_8036C5F0[];

    if (info == lbl_8047B218) {
        lbl_8047B218 = NULL;
    }
    ((HSD_ClassInfo*) *(u32*) (lbl_8036C5F0 + 0x14))->amnesia(info);
}
#else
void WObjAmnesia(void) { /* TODO */ }
#endif
#pragma pop

/* 0x801915D4 | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AE50C(void* aobj);
extern void fn_801C25E4(void* aobj);
#if 0
asm void WObjRelease(void) {
#include "src/hsd/hsd_wobj_WObjRelease.inc"
}
#else
#pragma optimization_level 1
extern u8 lbl_8036C5F0[];
void WObjRelease(HSD_WObj* wobj) {
    fn_801AE50C(wobj->robj);
    fn_801C25E4(wobj->aobj);
    {
        u32* parent_info = *(u32**)(lbl_8036C5F0 + 0x14);
        ((void (*)(HSD_WObj*))parent_info[0x30 / 4])(wobj);
    }
}
#endif
#pragma pop

/* 0x80191628 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void* fn_80193828(void*);
extern void __assert(const char*, u32, const char*);
extern const char lbl_8047D8C8[7];
extern const char lbl_8047D8D0[5];
#if 0
asm void HSD_WObjAlloc(void) {
#include "src/hsd/hsd_wobj_HSD_WObjAlloc.inc"
}
#else
#pragma optimization_level 1
HSD_WObj* HSD_WObjAlloc(void)
{
    extern u8 lbl_8036C5F0[];
    HSD_WObj* wobj;

    if ((wobj = (HSD_WObj*) fn_80193828(
        (*(HSD_ClassInfo* volatile*) &lbl_8047B218 != NULL)
            ? lbl_8047B218
            : (HSD_ClassInfo*) lbl_8036C5F0)) == NULL)
    {
        __assert(lbl_8047D8C8, 0x257, lbl_8047D8D0);
    }
    return wobj;
}
#endif
#pragma pop

/* 0x80191688 | 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019D9DC(HSD_JObj* jobj);
/* sdata2/rodata string labels referenced by asm incs (symbolmap port) */
extern u8 lbl_8027448C[];
extern u8 lbl_80274498[];
extern char lbl_8047D8D8;
extern char lbl_8047D8E0;
extern f64 lbl_8047D8E8;
extern f32 lbl_8047D8F0;
extern f64 lbl_8047D8F8;
extern f32 lbl_8047D900;
extern char lbl_8047D904;

typedef struct WObjVec {
    f32 x;
    f32 y;
    f32 z;
} WObjVec;

#undef WOBJ_USE_ANIM_POS
#undef WOBJ_POS_DIRTY
#define WOBJ_USE_ANIM_POS 0x1
#define WOBJ_POS_DIRTY 0x2

extern void PSMTXMultVec(f32 mtx[3][4], WObjVec* src, WObjVec* dst);

#define WOBJ_RESOLVE_ANIM_POSITION(wobj_)                                      \
    do {                                                                       \
        HSD_JObj* jobj_;                                                       \
        if (((wobj_)->flags & WOBJ_USE_ANIM_POS) != 0) {                       \
            if ((wobj_)->aobj != NULL) {                                       \
                if ((wobj_)->aobj->hsd_obj != NULL) {                          \
                    jobj_ = (HSD_JObj*) (wobj_)->aobj->hsd_obj;                \
                    if (jobj_ != NULL) {                                       \
                        if (jobj_ == NULL) {                                   \
                            __assert(&lbl_8047D8D8, 0x25D, &lbl_8047D8E0);     \
                        }                                                      \
                        if (((jobj_->flags & JOBJ_USER_DEF_MTX) == 0) &&       \
                            ((jobj_->flags & JOBJ_MTX_DIRTY) != 0)) {          \
                            fn_8019D9DC(jobj_);                                \
                        }                                                      \
                    }                                                          \
                    PSMTXMultVec(jobj_->mtx, (WObjVec*) &(wobj_)->pos_x,        \
                                (WObjVec*) &(wobj_)->pos_x);                   \
                }                                                              \
            }                                                                  \
            (wobj_)->flags = ((wobj_)->flags >> 1) << 1;                       \
        }                                                                      \
    } while (0)

#define WOBJ_SET_POSITION(wobj_, pos_)                                         \
    do {                                                                       \
        ((u32*) &(wobj_)->pos_x)[0] = ((u32*) (pos_))[0];                      \
        ((u32*) &(wobj_)->pos_x)[1] = ((u32*) (pos_))[1];                      \
        ((u32*) &(wobj_)->pos_x)[2] = ((u32*) (pos_))[2];                      \
        (wobj_)->flags |= WOBJ_POS_DIRTY;                                      \
        (wobj_)->flags = ((wobj_)->flags >> 1) << 1;                           \
    } while (0)

#if 0
asm void HSD_WObjGetPosition(void) {
#include "src/hsd/hsd_wobj_fn_80191688.inc"
}
#else
void HSD_WObjGetPosition(HSD_WObj* wobj, void* position)
{
    if (wobj == NULL) {
        return;
    }
    if (position == NULL) {
        return;
    }
    WOBJ_RESOLVE_ANIM_POSITION(wobj);
    ((u32*) position)[0] = ((u32*) &wobj->pos_x)[0];
    ((u32*) position)[1] = ((u32*) &wobj->pos_x)[1];
    ((u32*) position)[2] = ((u32*) &wobj->pos_x)[2];
}
#endif
#pragma pop

/* 0x80191788 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void HSD_WObjSetPosition(HSD_WObj* wobj, void* position) {
#include "src/hsd/hsd_wobj_HSD_WObjSetPosition.inc"
}
#else
#pragma optimization_level 4
void HSD_WObjSetPosition(HSD_WObj* wobj, void* position) {
    u32* src = (u32*)position;
    u32* dst;
    if (wobj == NULL) {
        return;
    }
    if (src != NULL) {
        dst = (u32*)&wobj->pos_x;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        wobj->flags |= 0x2;
        wobj->flags = (wobj->flags >> 1) << 1;
    }
}
#endif
#pragma pop

/* 0x801917D0 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern HSD_ClassInfo* fn_80193748(char* class_name);
#if 0
asm HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc) {
#include "src/hsd/hsd_wobj_HSD_WObjLoadDesc.inc"
}
#else
HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc)
{
    HSD_ClassInfo* info;
    HSD_WObj* wobj;
    extern u8 lbl_8036C5F0[];

    if (desc == NULL) {
        return NULL;
    }

    if (desc->class_name != NULL && (info = fn_80193748(desc->class_name)) != NULL) {
        wobj = fn_80193828(info);
        if (wobj == NULL) {
            __assert(lbl_8047D8C8, 0x104, lbl_8047D8D0);
        }
    } else {
        wobj = fn_80193828(lbl_8047B218 != NULL ? lbl_8047B218 : (HSD_ClassInfo*) lbl_8036C5F0);
        if (wobj == NULL) {
            __assert(lbl_8047D8C8, 0x257, lbl_8047D8D0);
        }
    }

    HSD_WOBJ_METHOD(wobj)->load(wobj, desc);
    return wobj;
}
#endif
#pragma pop

/* 0x8019189C | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AEBE4(void* robj, void* desc);
#if 0
asm void HSD_WObjInit(HSD_WObj* wobj, HSD_WObjDesc* desc) {
#include "src/hsd/hsd_wobj_HSD_WObjInit.inc"
}
#else
#pragma optimization_level 4
void HSD_WObjInit(HSD_WObj* wobj, HSD_WObjDesc* desc) {
    if (wobj == NULL) {
        return;
    }
    if (desc == NULL) {
        return;
    }
    if ((u32)desc + 4 != 0) {
        ((u32*) &wobj->pos_x)[0] = ((u32*) &desc->pos_x)[0];
        ((u32*) &wobj->pos_x)[1] = ((u32*) &desc->pos_x)[1];
        ((u32*) &wobj->pos_x)[2] = ((u32*) &desc->pos_x)[2];
        wobj->flags |= 0x2;
        wobj->flags = (wobj->flags >> 1) << 1;
    }
    if (wobj->robj != NULL) {
        fn_801AE50C(wobj->robj);
    }
    wobj->robj = (HSD_RObj*) HSD_RObjLoadDesc(desc->robjdesc);
    fn_801AEBE4(wobj->robj, desc->robjdesc);
}
#endif
#pragma pop

/* 0x8019194C | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm int WObjLoad(HSD_WObj* wobj, HSD_WObjDesc* desc) {
#include "src/hsd/hsd_wobj_WObjLoad.inc"
}
#else
#pragma optimization_level 4
int WObjLoad(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    if (wobj != NULL && (u32) desc + 4 != 0) {
        ((u32*) &wobj->pos_x)[0] = ((u32*) &desc->pos_x)[0];
        ((u32*) &wobj->pos_x)[1] = ((u32*) &desc->pos_x)[1];
        ((u32*) &wobj->pos_x)[2] = ((u32*) &desc->pos_x)[2];
        wobj->flags |= WOBJ_POS_DIRTY;
        wobj->flags = (wobj->flags >> 1) << 1;
    }
    if (wobj->robj != NULL) {
        fn_801AE50C(wobj->robj);
    }
    wobj->robj = (HSD_RObj*) HSD_RObjLoadDesc(desc->robjdesc);
    fn_801AEBE4(wobj->robj, desc->robjdesc);
    return 0;
}
#endif
#pragma pop

/* 0x801919EC | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801B0040(void* robj);
extern void fn_801C27F4(void* aobj, void* wobj, void* method);
#if 0
asm void HSD_WObjInterpretAnim(void) {
#include "src/hsd/hsd_wobj_HSD_WObjInterpretAnim.inc"
}
#else
#pragma optimization_level 4
void HSD_WObjInterpretAnim(HSD_WObj* wobj) {
    if (wobj != NULL) {
        fn_801C27F4(wobj->aobj, wobj, HSD_WOBJ_METHOD(wobj)->update);
        fn_801B0040(wobj->robj);
    }
}
#endif
#pragma pop

/* 0x80191A34 | 0x398 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void splArcLengthPoint(WObjVec* out, HSD_Spline* spline, f32 frame);
#if 0
asm void WObjUpdateFunc(void) {
#include "src/hsd/hsd_wobj_WObjUpdateFunc.inc"
}
#else
void WObjUpdateFunc(HSD_WObj* wobj, u32 type, void* value)
{
    WObjVec position;
    HSD_JObj* jobj;
    f32 component;
    f32* fval;

    if (wobj == NULL) {
        return;
    }

    switch (type) {
    case HSD_A_J_PATH:
        fval = (f32*) value;
        if (*fval < lbl_8047D8E8) {
            *fval = lbl_8047D8F0;
        }
        if (lbl_8047D8F8 < *fval) {
            *fval = lbl_8047D900;
        }
        if (wobj->aobj == NULL) {
            __assert(lbl_8047D8C8, 0x98, (const char*) lbl_8027448C);
        }
        jobj = (HSD_JObj*) wobj->aobj->hsd_obj;
        if (jobj == NULL) {
            __assert(lbl_8047D8C8, 0x9A, &lbl_8047D904);
        }
        if (jobj->u.spline == NULL) {
            __assert(lbl_8047D8C8, 0x9B, (const char*) lbl_80274498);
        }
        splArcLengthPoint(&position, jobj->u.spline, *fval);
        WOBJ_SET_POSITION(wobj, &position);
        wobj->flags |= WOBJ_USE_ANIM_POS;
        break;
    case HSD_A_J_TRAX:
        component = *(f32*) value;
        WOBJ_RESOLVE_ANIM_POSITION(wobj);
        wobj->pos_x = component;
        wobj->flags |= WOBJ_POS_DIRTY;
        break;
    case HSD_A_J_TRAY:
        component = *(f32*) value;
        WOBJ_RESOLVE_ANIM_POSITION(wobj);
        wobj->pos_y = component;
        wobj->flags |= WOBJ_POS_DIRTY;
        break;
    case HSD_A_J_TRAZ:
        component = *(f32*) value;
        WOBJ_RESOLVE_ANIM_POSITION(wobj);
        wobj->pos_z = component;
        wobj->flags |= WOBJ_POS_DIRTY;
        break;
    }
}
#endif
#pragma pop

/* 0x80191DCC | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AFE68(void* robj, void* robj_desc);
extern void* fn_801C2670(void* aobj_desc);
typedef struct { void* aobj_desc; void* robj_desc; } WObjADesc;
#if 0
asm void HSD_WObjAddAnim(void) {
#include "src/hsd/hsd_wobj_HSD_WObjAddAnim.inc"
}
#else
#pragma optimization_level 4
void HSD_WObjAddAnim(HSD_WObj* wobj, HSD_WObjAnim* desc) {
    if (wobj == NULL) {
        return;
    }
    if (desc == NULL) {
        return;
    }
    if (*(volatile u32*) ((u8*) wobj + 0x18) != 0) {
        fn_801C25E4(wobj->aobj);
    }
    wobj->aobj = (HSD_AObj*) fn_801C2670(desc->aobjdesc);
    fn_801AFE68(wobj->robj, desc->robjanim);
}
#endif
#pragma pop

/* 0x80191E38 | 0x50 */
#pragma push
extern void fn_801AFEFC(void* robj, f32 frame);
#pragma push
#pragma optimization_level 1
void HSD_WObjReqAnim(HSD_WObj* wobj, f32 frame) {
    if (wobj != NULL) {
        HSD_AObjReqAnim(wobj->aobj, frame);
        fn_801AFEFC(wobj->robj, frame);
    }
}
#pragma pop

/* 0x80191E88 | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AFFE0(void* robj);
#if 0
asm void HSD_WObjRemoveAnim(void) {
#include "src/hsd/hsd_wobj_HSD_WObjRemoveAnim.inc"
}
#else
#pragma optimization_level 4
void HSD_WObjRemoveAnim(HSD_WObj* wobj) {
    if (wobj != NULL) {
        fn_801C25E4(wobj->aobj);
        wobj->aobj = NULL;
        fn_801AFFE0(wobj->robj);
    }
}
#endif
#pragma pop

/* HSD_ArchiveGetPublicAddress (0x80191ECC) | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern int strcmp(void* entry, void* key);
typedef struct {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 count;    /* 0x0C */
    u32 field10;
    u32 field14;
    u32 field18;
    u32 field1C;
    u8* base;     /* 0x20 */
    u32 field24;
    u8** pairs;   /* 0x28: ptr to array of (offset, key_offset) pairs */
    u32 field2C;
    u8* data;     /* 0x30: data buffer */
} WObjTable;
typedef struct {
    u32 result_offset;  /* 0x00 */
    u32 key_offset;     /* 0x04 */
} WObjTablePair;
#if 0
asm void HSD_ArchiveGetPublicAddress(void) {
#include "src/hsd/hsd_wobj_HSD_ArchiveGetPublicAddress.inc"
}
#else
void* HSD_ArchiveGetPublicAddress(WObjTable* table, void* key) {
    u32 i;
    for (i = 0; i < table->count; i++) {
        if (strcmp(table->data + ((WObjTablePair*) table->pairs)[i].key_offset,
                        key) == 0) {
            return table->base + ((WObjTablePair*) table->pairs)[i].result_offset;
        }
    }
    return NULL;
}
#endif
#pragma pop

/* 0x80191F64 | 0x180 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void* memset(void* dst, int val, u32 size);
typedef struct {
    u32 magic;     /* 0x00 */
    u32 count_a;   /* 0x04 */
    u32 count_b;   /* 0x08 */
    u32 count_c;   /* 0x0C */
    u32 count_d;   /* 0x10 */
    u32 field14;   /* 0x14 */
    u32 field18;   /* 0x18 */
    u32 field1C;   /* 0x1C */
    u8* ptr_a;     /* 0x20 */
    u8* ptr_b;     /* 0x24 */
    u8* ptr_c;     /* 0x28 */
    u8* ptr_d;     /* 0x2C */
    u8* ptr_e;     /* 0x30 */
    u32 field34;   /* 0x34 */
    u32 field38;   /* 0x38 */
    u32 flags;     /* 0x3C */
    u8* raw_data;  /* 0x40 */
} WObjTableFull;
extern const char lbl_802744A8[];
#if 0
asm void HSD_ArchiveParse(void) {
#include "src/hsd/hsd_wobj_HSD_ArchiveParse.inc"
}
#else
#pragma optimization_level 4
s32 HSD_ArchiveParse(WObjTableFull* tbl, u8* data, u32 magic) {
    u32 offset;
    u32 i;
    if (tbl == NULL) {
        return -1;
    }
    memset(tbl, 0, 0x44);
    tbl->flags |= 1;
    memcpy(tbl, data, 0x20);
    if (tbl->magic != magic) {
        OSReport(lbl_802744A8);
        return -1;
    }
    offset = 0x20;
    if (tbl->count_a != 0) {
        tbl->ptr_a = data + offset;
        offset += tbl->count_a;
    }
    if (tbl->count_b != 0) {
        tbl->ptr_b = data + offset;
        offset += tbl->count_b * 4;
    }
    if (tbl->count_c != 0) {
        tbl->ptr_c = data + offset;
        offset += tbl->count_c * 8;
    }
    if (tbl->count_d != 0) {
        tbl->ptr_d = data + offset;
        offset += tbl->count_d * 8;
    }
    if (offset < tbl->magic) {
        tbl->ptr_e = data + offset;
    }
    tbl->raw_data = data;
    for (i = 0; i < tbl->count_b; i++) {
        u32 off = ((u32*) tbl->ptr_b)[i];
        u32* entry = (u32*)(tbl->ptr_a + off);
        *entry += (u32) tbl->ptr_a;
    }
    return 0;
}
#endif
#pragma pop
