/**
 * @file hsd_jobj_exact_801A053C.c
 * @brief Exact pure-C HSD JObj individual-reference release.
 */

#include "dolphin/mtx.h"

#define iref_DEC hsd_inline_iref_DEC
#define ref_INC hsd_inline_ref_INC
#include "hsd/hsd_jobj.h"
#undef iref_DEC
#undef ref_INC
#include "hsd/hsd_class.h"

static inline BOOL JObjIRefDec(void* object)
{
    BOOL result;

    if ((result = (HSD_OBJ(object)->ref_count_individual == 0))) {
        return result;
    }
    HSD_OBJ(object)->ref_count_individual -= 1;
    return HSD_OBJ(object)->ref_count_individual == 0;
}

static inline s32 JObjRefCount(void* object)
{
    if (HSD_OBJ(object)->ref_count == HSD_OBJ_NOREF) {
        return -1;
    }
    return HSD_OBJ(object)->ref_count;
}

static inline void JObjDelete(void* object)
{
    if (object == NULL) {
        return;
    }
    HSD_CLASS_METHOD(object)->release((HSD_Class*) object);
    HSD_CLASS_METHOD(object)->destroy((HSD_Class*) object);
}

#pragma push
#pragma optimization_level 1
void HSD_JObjUnrefThis(HSD_JObj* jobj)
{
    if (jobj != NULL && JObjIRefDec(jobj) && JObjRefCount(jobj) < 0) {
        JObjDelete(jobj);
    }
}
#pragma pop
