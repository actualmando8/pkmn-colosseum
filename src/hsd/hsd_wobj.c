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
#include "hsd/hsd_archive.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_object.h"
#include "hsd/hsd_robj.h"

void WObjInfoInit(void);
static int WObjLoad_Early(HSD_WObj* wobj, HSD_WObjDesc* desc);
int WObjLoad(HSD_WObj* wobj, HSD_WObjDesc* desc);
void WObjAmnesia(HSD_ClassInfo* info);
void WObjRelease(HSD_WObj* wobj);
void WObjUpdateFunc(void* obj, u32 type, void* value);

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
    HSD_WObjSetPosition_Early(wobj, desc->pos.x, desc->pos.y, desc->pos.z);
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

    HSD_WObjSetPosition_Early(wobj, desc->pos.x, desc->pos.y, desc->pos.z);
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
    wobj->pos.x = x;
    wobj->pos.y = y;
    wobj->pos.z = z;
    wobj->flags |= 0x2;
    wobj->flags &= ~0x1;
}

void HSD_WObjSetPositionX(HSD_WObj* wobj, f32 val)
{
    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            wobj->flags &= ~0x1;
        }
        wobj->pos.x = val;
        wobj->flags |= 0x2;
    }
}

void HSD_WObjSetPositionY(HSD_WObj* wobj, f32 val)
{
    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            wobj->flags &= ~0x1;
        }
        wobj->pos.y = val;
        wobj->flags |= 0x2;
    }
}

void HSD_WObjSetPositionZ(HSD_WObj* wobj, f32 val)
{
    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            wobj->flags &= ~0x1;
        }
        wobj->pos.z = val;
        wobj->flags |= 0x2;
    }
}

void HSD_WObjGetPosition_Early(HSD_WObj* wobj, f32* x, f32* y, f32* z)
{
    if (wobj == NULL) {
        return;
    }
    if (x != NULL) *x = wobj->pos.x;
    if (y != NULL) *y = wobj->pos.y;
    if (z != NULL) *z = wobj->pos.z;
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

/* 0x801914F4 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u8 lbl_8036CC00[];   /* hsdObj  class info */
extern char lbl_80274468[]; /* "sysdolphin_base_library" */
extern char lbl_80274480[]; /* "had_wobj" */
void WObjInfoInit(void)
{
    extern u8 lbl_8036C5F0[]; /* hsdWObj class info */

    hsdInitClassInfo(HSD_CLASS_INFO(lbl_8036C5F0),
                     HSD_CLASS_INFO(lbl_8036CC00), lbl_80274468, lbl_80274480,
                     sizeof(HSD_WObjInfo), sizeof(HSD_WObj));
    HSD_CLASS_INFO(lbl_8036C5F0)->release = (void (*)(HSD_Class*)) WObjRelease;
    HSD_CLASS_INFO(lbl_8036C5F0)->amnesia = WObjAmnesia;
    HSD_WOBJ_INFO(lbl_8036C5F0)->load = WObjLoad;
    HSD_WOBJ_INFO(lbl_8036C5F0)->update =
        (void (*)(HSD_WObj*, u32, void*)) WObjUpdateFunc;
}
#pragma pop

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

#undef WOBJ_USE_ANIM_POS
#undef WOBJ_POS_DIRTY
#define WOBJ_USE_ANIM_POS 0x1u
#define WOBJ_POS_DIRTY 0x2u

/* Local mirrors of HSD_JObjSetupMatrix's dirty test. These cannot come from
   hsd_jobj.h: the assert baked into the target carries that header's own
   __FILE__ / line, which the externs above name directly. */
static inline BOOL JObjMtxIsDirty(HSD_JObj* jobj)
{
    BOOL result;

    if (jobj == NULL) {
        __assert(&lbl_8047D8D8, 0x25D, &lbl_8047D8E0);
    }
    result = FALSE;
    if (!(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY)) {
        result = TRUE;
    }
    return result;
}

static inline void WObjJObjSetupMatrix(HSD_JObj* jobj)
{
    if (jobj == NULL || !JObjMtxIsDirty(jobj)) {
        return;
    }
    fn_8019D9DC(jobj);
}

/* Bake the animated position into pos and drop the "animated" flag, so that
   callers always read a plain world-space vector. jobj_ is the caller's
   scratch HSD_JObj*, shared across every use in a function. */
#define WOBJ_RESOLVE_ANIM_POSITION(wobj_, jobj_)                               \
    do {                                                                       \
        if (((wobj_)->flags & WOBJ_USE_ANIM_POS) != 0) {                       \
            if ((wobj_)->aobj != NULL) {                                       \
                if ((wobj_)->aobj->hsd_obj != NULL) {                          \
                    (jobj_) = (HSD_JObj*) (wobj_)->aobj->hsd_obj;              \
                    if ((jobj_) != NULL) {                                     \
                        if (JObjMtxIsDirty(jobj_)) {                           \
                            fn_8019D9DC(jobj_);                                \
                        }                                                      \
                    }                                                          \
                    PSMTXMultVec((jobj_)->mtx, &(wobj_)->pos, &(wobj_)->pos);  \
                }                                                              \
            }                                                                  \
            (wobj_)->flags &= 0xFFFFFFFE;                                      \
        }                                                                      \
    } while (0)

/* Mirrors HSD_WObjSetPositionX/Y/Z: evaluate the new component, then (for a
   non-NULL wobj) bake any animated position and store the component. */
#define WOBJ_SET_POSITION_COMPONENT(wobj_, jobj_, field_, val_)                \
    do {                                                                       \
        f32 component_ = (val_);                                               \
        if ((wobj_) != NULL) {                                                 \
            WOBJ_RESOLVE_ANIM_POSITION(wobj_, jobj_);                          \
            (wobj_)->pos.field_ = component_;                                  \
            (wobj_)->flags |= WOBJ_POS_DIRTY;                                  \
        }                                                                      \
    } while (0)

#if 0
asm void HSD_WObjGetPosition(void) {
#include "src/hsd/hsd_wobj_fn_80191688.inc"
}
#else
#pragma optimization_level 1
void HSD_WObjGetPosition(HSD_WObj* wobj, Vec* position)
{
    HSD_JObj* jobj;

    if (wobj == NULL || position == NULL) {
        return;
    }
    if ((wobj->flags & WOBJ_USE_ANIM_POS) != 0) {
        if (wobj->aobj != NULL && wobj->aobj->hsd_obj != NULL) {
            WObjJObjSetupMatrix(jobj = (HSD_JObj*) wobj->aobj->hsd_obj);
            PSMTXMultVec(jobj->mtx, &wobj->pos, &wobj->pos);
        }
        wobj->flags &= 0xFFFFFFFE;
    }
    *position = wobj->pos;
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
void HSD_WObjSetPosition(HSD_WObj* wobj, Vec* position)
{
    if (wobj == NULL || position == NULL) {
        return;
    }
    wobj->pos = *position;
    wobj->flags = wobj->flags | WOBJ_POS_DIRTY;
    wobj->flags = wobj->flags & 0xFFFFFFFE;
}
#endif
#pragma pop

/* 0x801917D0 | 0xCC */
#pragma push
#pragma optimization_level 1
#pragma optimizewithasm off
extern HSD_ClassInfo* fn_80193748(char* class_name);
#if 0
asm HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc) {
#include "src/hsd/hsd_wobj_HSD_WObjLoadDesc.inc"
}
#else
HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc)
{
    extern u8 lbl_8036C5F0[];

    if (desc != NULL) {
        HSD_WObj* wobj;
        HSD_ClassInfo* info;

        if (desc->class_name == NULL ||
            !(info = fn_80193748(desc->class_name)))
        {
            /* HSD_WObjAlloc() inlined */
            wobj = (HSD_WObj*) fn_80193828(
                lbl_8047B218 ? lbl_8047B218 : (HSD_ClassInfo*) lbl_8036C5F0);
            if (wobj == NULL) {
                __assert(lbl_8047D8C8, 0x257, lbl_8047D8D0);
            }
        } else {
            wobj = (HSD_WObj*) fn_80193828(info);
            if (wobj == NULL) {
                __assert(lbl_8047D8C8, 0x104, lbl_8047D8D0);
            }
        }
        HSD_WOBJ_METHOD(wobj)->load(wobj, desc);
        return wobj;
    }
    return NULL;
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
#pragma optimization_level 1
void HSD_WObjInit(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    if (wobj == NULL || desc == NULL) {
        return;
    }

    HSD_WObjSetPosition(wobj, &desc->pos);
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
#pragma optimization_level 1
int WObjLoad(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    HSD_WObjSetPosition(wobj, &desc->pos);
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
#pragma optimization_level 1
#pragma optimizewithasm off
extern void splArcLengthPoint(Vec* out, HSD_Spline* spline, f32 frame);
#if 0
asm void WObjUpdateFunc(void) {
#include "src/hsd/hsd_wobj_WObjUpdateFunc.inc"
}
#else
void WObjUpdateFunc(void* obj, u32 type, void* value)
{
    HSD_WObj* wobj;
    Vec position;
    HSD_JObj* jobj;
    f32* fval;

    wobj = (HSD_WObj*) obj;
    fval = (f32*) value;

    if (wobj == NULL) {
        return;
    }

    switch (type) {
    case HSD_A_J_PATH:
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
        HSD_WObjSetPosition(wobj, &position);
        wobj->flags |= WOBJ_USE_ANIM_POS;
        break;
    case HSD_A_J_TRAX:
        WOBJ_SET_POSITION_COMPONENT(wobj, jobj, x, *fval);
        break;
    case HSD_A_J_TRAY:
        WOBJ_SET_POSITION_COMPONENT(wobj, jobj, y, *fval);
        break;
    case HSD_A_J_TRAZ:
        WOBJ_SET_POSITION_COMPONENT(wobj, jobj, z, *fval);
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
#pragma optimization_level 1
#pragma optimizewithasm off
extern int strcmp(const char* s1, const char* s2);
void* HSD_ArchiveGetPublicAddress(HSD_Archive* archive, const char* symbols)
{
    u32 i;

    for (i = 0; i < archive->header.nb_public; i++) {
        int comparison =
            strcmp(archive->symbols + archive->public_info[i].symbol, symbols);

        if (comparison == 0) {
            return archive->data + archive->public_info[i].offset;
        }
    }

    return NULL;
}
#pragma pop

/* 0x80191F64 | 0x180 */
#pragma push
#pragma optimization_level 1
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void* memset(void* dst, int val, u32 size);
extern const char lbl_802744A8[];

static inline void Locate(HSD_Archive* archive)
{
    u32 i;
    u32* ptr;

    for (i = 0; i < archive->header.nb_reloc; i++) {
        ptr = (u32*) (archive->data + archive->reloc_info[i].offset);
        *ptr += (u32) archive->data;
    }
}

s32 HSD_ArchiveParse(HSD_Archive* archive, u8* src, u32 file_size)
{
    u32 offset = 0;

    if (archive == NULL) {
        return -1;
    }

    memset(archive, 0, sizeof(HSD_Archive));
    archive->flags = archive->flags | HSD_ARCHIVE_DONT_FREE;
    memcpy(archive, src, sizeof(HSD_ArchiveHeader));

    if (archive->header.file_size != file_size) {
        OSReport(lbl_802744A8);
        return -1;
    }

    offset += sizeof(HSD_ArchiveHeader);
    if (archive->header.data_size != 0) {
        archive->data = src + offset;
        offset += archive->header.data_size;
    }
    if (archive->header.nb_reloc != 0) {
        archive->reloc_info = (HSD_ArchiveRelocationInfo*) (src + offset);
        offset += archive->header.nb_reloc * sizeof(HSD_ArchiveRelocationInfo);
    }
    if (archive->header.nb_public != 0) {
        archive->public_info = (HSD_ArchivePublicInfo*) (src + offset);
        offset += archive->header.nb_public * sizeof(HSD_ArchivePublicInfo);
    }
    if (archive->header.nb_extern != 0) {
        archive->extern_info = (HSD_ArchiveExternInfo*) (src + offset);
        offset += archive->header.nb_extern * sizeof(HSD_ArchiveExternInfo);
    }
    if (offset < archive->header.file_size) {
        archive->symbols = (char*) (src + offset);
    }

    archive->top_ptr = src;
    Locate(archive);

    return 0;
}
#pragma pop
