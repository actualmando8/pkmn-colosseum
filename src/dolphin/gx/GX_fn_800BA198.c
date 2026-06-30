#include "dolphin/types.h"

typedef struct GXLightObj_800BA198 {
    u8 pad_00[0x10];
    f32 a0;
    f32 a1;
    f32 a2;
    f32 k0;
    f32 k1;
    f32 k2;
} GXLightObj_800BA198;

void fn_800BA198(GXLightObj_800BA198* light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2) {
    light->a0 = a0;
    light->a1 = a1;
    light->a2 = a2;
    light->k0 = k0;
    light->k1 = k1;
    light->k2 = k2;
}
