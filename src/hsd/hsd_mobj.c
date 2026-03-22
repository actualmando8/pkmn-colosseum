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
