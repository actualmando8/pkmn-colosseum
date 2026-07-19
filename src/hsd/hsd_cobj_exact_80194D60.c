/**
 * @file hsd_cobj_exact_80194D60.c
 * @brief Camera viewing-matrix copy helper.
 */
#include "hsd/hsd_cobj.h"

extern void PSMTXCopy(const f32* src, f32* dst);

void HSD_CObjGetViewingMtx(HSD_CObj* cobj, f32 mtx[3][4])
{
    PSMTXCopy(HSD_CObjGetViewingMtxPtr(cobj), mtx[0]);
}
