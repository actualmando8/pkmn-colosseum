/**
 * @file hsd_jobj_exact_801A1980.c
 * @brief Exact pure-C HSD JObj matrix helpers.
 */

#include "dolphin/mtx.h"

#define iref_DEC hsd_inline_iref_DEC
#define ref_INC hsd_inline_ref_INC
#include "hsd/hsd_jobj.h"
#undef iref_DEC
#undef ref_INC

extern char lbl_8047DB34;
extern char lbl_8047DB3C;
extern void __assert();
extern void fn_8019D9DC(HSD_JObj*);

#pragma push
#pragma optimization_level 0
f32* fn_801A1980(HSD_JObj* jobj)
{
    return jobj->mtx[1];
}
#pragma pop

static inline s32 JObjMtxIsDirtyForSetup(HSD_JObj* jobj)
{
    s32 dirty;

    if (jobj == NULL) {
        __assert(&lbl_8047DB34, 0x25D, &lbl_8047DB3C);
    }
    dirty = 0;
    if (!(jobj->flags & JOBJ_USER_DEF_MTX) &&
        (jobj->flags & JOBJ_MTX_DIRTY))
    {
        dirty = 1;
    }
    return dirty;
}

#pragma push
#pragma optimization_level 1
void fn_801A1988(HSD_JObj* jobj)
{
    if (jobj == NULL || !JObjMtxIsDirtyForSetup(jobj)) {
        return;
    }
    fn_8019D9DC(jobj);
}
#pragma pop
