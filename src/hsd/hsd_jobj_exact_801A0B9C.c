/**
 * @file hsd_jobj_exact_801A0B9C.c
 * @brief Exact pure-C HSD reference and ID helpers.
 */

#include "dolphin/mtx.h"

#define iref_DEC hsd_inline_iref_DEC
#define ref_INC hsd_inline_ref_INC
#include "hsd/hsd_jobj.h"
#undef iref_DEC
#undef ref_INC
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_id.h"

extern char lbl_80274AF4[];
extern char lbl_80274B00[];
extern char lbl_80274B64[];

#pragma push
#pragma optimization_level 4
void HSD_JObjRef(HSD_JObj* jobj)
{
    if (jobj != NULL) {
        jobj->object.ref_count++;
        if (jobj->object.ref_count == HSD_OBJ_NOREF) {
            __assert(lbl_80274AF4, 0x5D, lbl_80274B64);
        }
    }
}
#pragma pop

#pragma push
#pragma optimization_level 4
void* HSD_IDGetData(u32 key, s32* found)
{
    return HSD_IDGetDataFromTable(NULL, key, found);
}
#pragma pop

#pragma push
#pragma optimization_level 1
void ref_INC(void* object)
{
    HSD_OBJ(object)->ref_count++;
    if (HSD_OBJ(object)->ref_count == HSD_OBJ_NOREF) {
        __assert(lbl_80274AF4, 0x5D, lbl_80274B64);
    }
}
#pragma pop

#pragma push
#pragma optimization_level 1
BOOL iref_DEC(void* object)
{
    BOOL result;

    if ((result = (HSD_OBJ(object)->ref_count_individual == 0))) {
        return result;
    }
    HSD_OBJ(object)->ref_count_individual -= 1;
    return HSD_OBJ(object)->ref_count_individual == 0;
}
#pragma pop

void iref_INC_801A0C9C(void* object)
{
    HSD_OBJ(object)->ref_count_individual++;
    if (HSD_OBJ(object)->ref_count_individual == 0) {
        __assert(lbl_80274AF4, 0x9E, lbl_80274B00);
    }
}
