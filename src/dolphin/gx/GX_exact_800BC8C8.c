#include "dolphin/gx/GXInternal.h"

void fn_800BC8C8(u32 nStages)
{
    GXData* data = gx;

    data->genMode = (data->genMode & ~0x3C00U) |
                    (((nStages & 0xFF) - 1) << 10);
    data->dirtyState |= 4;
}
