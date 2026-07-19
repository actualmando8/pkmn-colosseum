/**
 * @file hsd_jobj_exact_801A1B40.c
 * @brief Exact pure-C HSD JObj animation entry point.
 */

#include "dolphin/mtx.h"

#define iref_DEC hsd_inline_iref_DEC
#define ref_INC hsd_inline_ref_INC
#include "hsd/hsd_jobj.h"
#undef iref_DEC
#undef ref_INC
#include "hsd/hsd_aobj.h"

extern void fn_801A1B7C(HSD_JObj*);

void HSD_JObjAnimAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_AObjInitEndCallBack();
    fn_801A1B7C(jobj);
    HSD_AObjInvokeCallBacks();
}
