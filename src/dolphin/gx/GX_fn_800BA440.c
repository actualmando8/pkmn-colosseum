#include "dolphin/types.h"

typedef struct GXColor_800BA440 {
    u32 rgba;
} GXColor_800BA440;

typedef struct GXLightObj_800BA440 {
    u8 pad_00[0x0C];
    u32 color;
} GXLightObj_800BA440;

void fn_800BA440(GXLightObj_800BA440* light, const GXColor_800BA440* color) {
    light->color = color->rgba;
}
