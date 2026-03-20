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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6A34(void) {
    /* TODO: match -- 0xF0 bytes at 0x801A6A34 */
}
#pragma pop

/* 0x801A6B24 | 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6B24(void) {
    /* TODO: match -- 0x68 bytes at 0x801A6B24 */
}
#pragma pop

/* 0x801A6B8C | 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6B8C(void) {
    /* TODO: match -- 0xA8 bytes at 0x801A6B8C */
}
#pragma pop

/* 0x801A6C34 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6C34(void) {
    /* TODO: match -- 0x70 bytes at 0x801A6C34 */
}
#pragma pop

/* 0x801A6CA4 | 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6CA4(void) {
    /* TODO: match -- 0x64 bytes at 0x801A6CA4 */
}
#pragma pop

/* 0x801A6D08 | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6D08(void) {
    /* TODO: match -- 0x54 bytes at 0x801A6D08 */
}
#pragma pop

/* 0x801A6D5C | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6D5C(void) {
    /* TODO: match -- 0x44 bytes at 0x801A6D5C */
}
#pragma pop

/* 0x801A6DA0 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6DA0(void) {
    /* TODO: match -- 0x24 bytes at 0x801A6DA0 */
}
#pragma pop

/* 0x801A6DDC | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6DDC(void) {
    /* TODO: match -- 0x24 bytes at 0x801A6DDC */
}
#pragma pop

/* 0x801A6E00 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6E00(void) {
    /* TODO: match -- 0x24 bytes at 0x801A6E00 */
}
#pragma pop

/* 0x801A6E24 | 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6E24(void) {
    /* TODO: match -- 0x154 bytes at 0x801A6E24 */
}
#pragma pop

/* 0x801A6F78 | 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6F78(void) {
    /* TODO: match -- 0x78 bytes at 0x801A6F78 */
}
#pragma pop

/* 0x801A6FF0 | 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A6FF0(void) {
    /* TODO: match -- 0x138 bytes at 0x801A6FF0 */
}
#pragma pop

/* 0x801A7128 | 0x9FC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A7128(void) {
    /* TODO: match -- 0x9FC bytes at 0x801A7128 */
}
#pragma pop

/* 0x801A7B24 | 0x1D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A7B24(void) {
    /* TODO: match -- 0x1D8 bytes at 0x801A7B24 */
}
#pragma pop

/* 0x801A7CFC | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A7CFC(void) {
    /* TODO: match -- 0x5C bytes at 0x801A7CFC */
}
#pragma pop

/* 0x801A7D58 | 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A7D58(void) {
    /* TODO: match -- 0xE4 bytes at 0x801A7D58 */
}
#pragma pop

/* 0x801A7E3C | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A7E3C(void) {
    /* TODO: match -- 0x48 bytes at 0x801A7E3C */
}
#pragma pop

/* 0x801A7E84 | 0x4D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801A7E84(void) {
    /* TODO: match -- 0x4D0 bytes at 0x801A7E84 */
}
#pragma pop
