#include "dolphin/gx/GXInternal.h"

void fn_800BD744(void)
{
    fn_800BD640(1);
}

void fn_800BD768(f32* projection)
{
    GXData* data = gx;

    projection[0] = data->projection[0];
    projection[1] = data->projection[1];
    projection[2] = data->projection[2];
    projection[3] = data->projection[3];
    projection[4] = data->projection[4];
    projection[5] = data->projection[5];
}
