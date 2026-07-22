#include "dolphin/types.h"

extern f64 cos(f32 angle);
extern f32 lbl_8047CB38;
extern f32 lbl_8047CB30;
extern f32 lbl_804011B8[];

void GSmathInitCosTable(void)
{
    s32 i;
    f32 step;
    f32 scale;

    scale = lbl_8047CB38;
    step = lbl_8047CB30;
    for (i = 0; i < 181; i++) {
        lbl_804011B8[i] = (f32)cos(scale * (step * (f32)i));
    }
}
