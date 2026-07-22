/** Exact HSD JObj matrix-dirty helpers, 0x800EA60C - 0x800EA6D4. */
#include "dolphin/types.h"

typedef struct HSD_JObj {
    u8 _pad0[0x14];
    u32 flags;
} HSD_JObj;

extern const char lbl_8047CC00[7];
extern const char lbl_8047CC08[5];
extern void __assert(const char* file, u32 line, const char* condition);

BOOL HSD_JObjMtxIsDirty(HSD_JObj* jobj)
{
    BOOL result;

    if (jobj == NULL) {
        __assert(lbl_8047CC00, 0x25D, lbl_8047CC08);
    }
    result = FALSE;
    if (!(jobj->flags & 0x00800000) && (jobj->flags & 0x40)) {
        result = TRUE;
    }
    return result;
}

void HSD_JObjSetupMatrix_800EA664(HSD_JObj* jobj)
{
    extern void fn_8019D9DC(HSD_JObj* jobj);

    if (jobj == NULL || !HSD_JObjMtxIsDirty(jobj)) {
        return;
    }
    fn_8019D9DC(jobj);
}
