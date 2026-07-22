#include "dolphin/mtx.h"
#include "dolphin/types.h"
#include "hsd/hsd_debug.h"

typedef struct HSD_ViewingRect {
    Vec3 origin;
    Vec3 up;
    Vec3 right;
    Vec3 eye;
    Vec3 eye_vn;
    f32 distance;
    f32 top;
    f32 bottom;
    f32 left;
    f32 right_edge;
} HSD_ViewingRect;

extern char lbl_802752C0[];
extern char lbl_8047DDB8;

s32 HSD_ViewingRectCheck(HSD_ViewingRect* rect)
{
    s32 result;

    if (rect == NULL) {
        __assert(lbl_802752C0, 0x37D, &lbl_8047DDB8);
    }
    result = 0;
    if (rect->top > rect->bottom) {
        if (rect->right_edge > rect->left) {
            result = 1;
        }
    }
    return result;
}
