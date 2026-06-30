#include "dolphin/types.h"

#pragma push
#pragma fp_contract on
void HSD_MtxScaledAdd(f32* src, f32 scale, f32* add, f32* dst)
{
    dst[0] = scale * src[0] + add[0];
    dst[1] = scale * src[1] + add[1];
    dst[2] = scale * src[2] + add[2];
    dst[3] = scale * src[3] + add[3];
    dst[4] = scale * src[4] + add[4];
    dst[5] = scale * src[5] + add[5];
    dst[6] = scale * src[6] + add[6];
    dst[7] = scale * src[7] + add[7];
    dst[8] = scale * src[8] + add[8];
    dst[9] = scale * src[9] + add[9];
    dst[10] = scale * src[10] + add[10];
    dst[11] = scale * src[11] + add[11];
}
#pragma pop
