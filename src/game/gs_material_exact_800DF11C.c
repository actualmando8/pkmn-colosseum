#include "dolphin/gx/GX.h"
#include "dolphin/types.h"

typedef struct GSmaterialObject {
    u8 field_00[0xC];
    GXColor modulate;
} GSmaterialObject;

void GSmaterialGetModulate(const GSmaterialObject* material, GXColor* color)
{
    color->r = material->modulate.r;
    color->g = material->modulate.g;
    color->b = material->modulate.b;
    color->a = material->modulate.a;
}
