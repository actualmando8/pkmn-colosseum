/**
 * @file hsd_jobj_exact_801A0D3C.c
 * @brief Exact pure-C HSD reference-count helpers.
 */

#include "dolphin/mtx.h"

#define iref_DEC hsd_inline_iref_DEC
#define ref_INC hsd_inline_ref_INC
#include "hsd/hsd_jobj.h"
#undef iref_DEC
#undef ref_INC

#pragma push
#pragma optimization_level 4
s32 fn_801A0D3C(HSD_Obj* object)
{
    return object->ref_count_individual - 1;
}
#pragma pop

#pragma push
#pragma optimization_level 1
BOOL ref_DEC_801A0D48(void* object)
{
    BOOL result;

    if ((result = (HSD_OBJ(object)->ref_count == HSD_OBJ_NOREF))) {
        return result;
    }
    result = (HSD_OBJ(object)->ref_count == 0);
    HSD_OBJ(object)->ref_count -= 1;
    return result;
}
#pragma pop
