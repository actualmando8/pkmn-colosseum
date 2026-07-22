#include "dolphin/gx/GX.h"
#include "dolphin/types.h"

typedef struct HSD_MaterialState {
    GXColor ambient;
    GXColor diffuse;
    GXColor specular;
    u8 alpha;
    u8 pad_0D[3];
    f32 shininess;
} HSD_MaterialState;

extern HSD_MaterialState lbl_80465710;

void fn_801B28B8(f32 value)
{
    lbl_80465710.shininess = value;
}
