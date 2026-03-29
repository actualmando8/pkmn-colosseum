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

/* 0x801A6B24 | 0x68 */
extern u32 lbl_8047B2D0;
extern u8 lbl_8036CB30[];
extern u32 lbl_8047B2D8;
extern u32 lbl_8047B2DC;
#if 1
asm void fn_801A6B24(void) {
#include "src/hsd/hsd_mobj_fn_801A6B24.inc"
}
#else
void fn_801A6B24(void) {}
#endif

/* 0x801A6B8C | 0xA8 */
extern void fn_801C25E4(void);
extern void fn_80193AF0(void);
extern void fn_801BBE60(void);
extern void fn_801B42C0(void);
extern void fn_801B7178(void);
#if 1
asm void fn_801A6B8C(void) {
#include "src/hsd/hsd_mobj_fn_801A6B8C.inc"
}
#else
void fn_801A6B8C(void) {}
#endif

/* 0x801A6C34 | 0x70 */
#if 1
asm void fn_801A6C34(void) {
#include "src/hsd/hsd_mobj_fn_801A6C34.inc"
}
#else
void fn_801A6C34(void) {}
#endif

/* 0x801A6CA4 | 0x64 */
extern void fn_80196E10(void);
extern u8 lbl_8047DC18[];
extern u8 lbl_8047DC20[];
#if 1
asm void fn_801A6CA4(void) {
#include "src/hsd/hsd_mobj_fn_801A6CA4.inc"
}
#else
void fn_801A6CA4(void) {}
#endif

/* 0x801A6D08 | 0x54 */
#if 1
asm void fn_801A6D08(void) {
#include "src/hsd/hsd_mobj_fn_801A6D08.inc"
}
#else
void fn_801A6D08(void) {}
#endif

/* 0x801A6D5C | 0x44 */
extern void fn_801BBE3C(void);
#if 1
asm void fn_801A6D5C(void) {
#include "src/hsd/hsd_mobj_fn_801A6D5C.inc"
}
#else
void fn_801A6D5C(void) {}
#endif

/* 0x801A6DA0 | 0x24 */
#if 1
asm void fn_801A6DA0(void) {
#include "src/hsd/hsd_mobj_fn_801A6DA0.inc"
}
#else
void fn_801A6DA0(void) {}
#endif

/* 0x801A6DC4 | 0x18 */
#if 1
asm void fn_801A6DC4(void) {
#include "src/hsd/hsd_mobj_fn_801A6DC4.inc"
}
#else
void fn_801A6DC4(void) {}
#endif

/* 0x801A6DDC | 0x24 */
#if 1
asm void fn_801A6DDC(void) {
#include "src/hsd/hsd_mobj_fn_801A6DDC.inc"
}
#else
void fn_801A6DDC(void) {}
#endif

/* 0x801A6E00 | 0x24 */
extern void fn_801BBFE4(void);
#if 1
asm void fn_801A6E00(void) {
#include "src/hsd/hsd_mobj_fn_801A6E00.inc"
}
#else
void fn_801A6E00(void) {}
#endif

/* 0x801A6E24 | 0x154 */
extern void fn_801B28B8(void);
extern void fn_801B28C8(void);
extern void fn_801B294C(void);
extern void fn_801B3884(void);
extern void fn_801BDA58(void);
#if 1
asm void fn_801A6E24(void) {
#include "src/hsd/hsd_mobj_fn_801A6E24.inc"
}
#else
void fn_801A6E24(void) {}
#endif

/* 0x801A6F78 | 0x78 */
extern void fn_801B45A4(void);
extern void fn_801BD8D0(void);
extern u8 lbl_80274E5C[];
#if 1
asm void fn_801A6F78(void) {
#include "src/hsd/hsd_mobj_fn_801A6F78.inc"
}
#else
void fn_801A6F78(void) {}
#endif

/* 0x801A7128 | 0x9FC */
extern void fn_801B5E40(void);
extern void fn_801B5F08(void);
extern void fn_801B64EC(void);
extern void fn_801B6CD8(void);
extern void fn_801B6E74(void);
extern void fn_801B6F5C(void);
extern void fn_801B707C(void);
extern void fn_801B7C60(void);
extern u8 lbl_80478C88[];
extern u32 lbl_8047DC38;
#if 1
asm void fn_801A7128(void) {
#include "src/hsd/hsd_mobj_fn_801A7128.inc"
}
#else
void fn_801A7128(void) {}
#endif

/* 0x801A7B24 | 0x1D8 */
extern void fn_80193748(void);
extern void fn_80193828(void);
extern void fn_801B4300(void);
extern void fn_801BC33C(void);
extern u32 lbl_8047DC30;
#if 1
asm void fn_801A7B24(void) {
#include "src/hsd/hsd_mobj_fn_801A7B24.inc"
}
#else
void fn_801A7B24(void) {}
#endif

/* 0x801A7D58 | 0xE4 */
extern void fn_801BE4CC(void);
extern void fn_80193B10(void);
extern void* memcpy(void* dst, const void* src, u32 n);
extern u8 lbl_8047DC28[];
extern u32 lbl_8047DC2C;
#if 1
asm void fn_801A7D58(void) {
#include "src/hsd/hsd_mobj_fn_801A7D58.inc"
}
#else
void fn_801A7D58(void) {}
#endif

/* 0x801A7E3C | 0x48 */
extern void fn_801C27F4(void);
extern void fn_801BE800(void);
#if 1
asm void fn_801A7E3C(void) {
#include "src/hsd/hsd_mobj_fn_801A7E3C.inc"
}
#else
void fn_801A7E3C(void) {}
#endif

/* 0x801A7E84 | 0x4D0 */
extern void jumptable_8036CB84();
extern u32 lbl_8047DC40;
extern u32 lbl_8047DC44;
#if 1
asm void fn_801A7E84(void) {
#include "src/hsd/hsd_mobj_fn_801A7E84.inc"
}
#else
void fn_801A7E84(void) {}
#endif
