#include "dolphin/gx/GX.h"

typedef struct GXLightObj_800BA440 {
    u8 pad_00[0x0C];
    GXColor color;
} GXLightObj_800BA440;

void fn_800BA440(GXLightObj* light, GXColor color) {
    ((GXLightObj_800BA440*) light)->color = color;
}
