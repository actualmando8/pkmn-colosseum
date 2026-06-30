#include "dolphin/types.h"

typedef struct GXLightObj_800BA414 {
    u8 pad_00[0x28];
    f32 x;
    f32 y;
    f32 z;
} GXLightObj_800BA414;

void fn_800BA414(GXLightObj_800BA414* light, f32 x, f32 y, f32 z) {
    light->x = x;
    light->y = y;
    light->z = z;
}
