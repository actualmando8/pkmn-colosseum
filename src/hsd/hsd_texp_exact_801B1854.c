#include "dolphin/types.h"
#include "hsd/hsd_objalloc.h"

typedef struct SplineVec3 {
    f32 x;
    f32 y;
    f32 z;
} SplineVec3;

typedef struct HSD_Spline {
    u8 type;
    u8 pad_01;
    s16 numcv;
    f32 tension;
    SplineVec3* cv;
    f32 totalLength;
    f32* segLength;
    f32 (*segPoly)[5];
} HSD_Spline;

extern HSD_ObjAllocData lbl_804656E0;
extern void HSD_ObjAllocInit(void* list, u32 size, u32 alignment);
extern f32 fn_801B18D8(HSD_Spline* spline, f32 distance);
extern void fn_801B2038(SplineVec3* point, HSD_Spline* spline, f32 value);

void fn_801B1854(void)
{
    HSD_ObjAllocInit(&lbl_804656E0, 0x28, 4);
}

HSD_ObjAllocData* HSD_ShadowGetAllocData(void)
{
    return &lbl_804656E0;
}

void splArcLengthPoint(SplineVec3* point, HSD_Spline* spline, f32 distance)
{
    fn_801B2038(point, spline, fn_801B18D8(spline, distance));
}
