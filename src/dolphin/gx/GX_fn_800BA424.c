#include "dolphin/types.h"

typedef struct GXLightObj_800BA424 {
    u8 pad_00[0x34];
    f32 nx;
    f32 ny;
    f32 nz;
} GXLightObj_800BA424;

void fn_800BA424(GXLightObj_800BA424* light, f32 x, f32 y, f32 z) {
    light->nx = -x;
    light->ny = -y;
    light->nz = -z;
}
