#include "dolphin/gx/GX.h"
#include "dolphin/types.h"

typedef struct GSmaterialEnvMapObject {
    u8 field_00[0x50];
    f32 blend;
} GSmaterialEnvMapObject;

typedef struct GSmaterialObject {
    u8 field_00[0xC];
    GXColor modulate;
    u32 colorChannels[4];
    GSmaterialEnvMapObject* envMapObject;
    u32 field_24;
    u32 envMapMode;
    u32 envMapParam0;
    u32 envMapParam1;
    f32 envMapBlend;
} GSmaterialObject;

void GSmaterialSetEnvMapBlendValue(GSmaterialObject* material, f32 blend)
{
    GSmaterialEnvMapObject* envMapObject = material->envMapObject;

    material->envMapBlend = blend;
    if (envMapObject != 0) {
        envMapObject->blend = blend;
    }
}

void GSmaterialSetEnvMapParams(GSmaterialObject* material, u32 param0,
                               u32 param1, f32 blend, u32 mode)
{
    material->envMapParam0 = param0;
    material->envMapParam1 = param1;
    material->envMapBlend = blend;
    material->envMapMode = mode;
}

void GSmaterialSetModulate(GSmaterialObject* material, const GXColor* color)
{
    material->modulate.r = color->r;
    material->modulate.g = color->g;
    material->modulate.b = color->b;
    material->modulate.a = color->a;
}

void GSmaterialSetColorChannels(GSmaterialObject* material, u32 channel0,
                                u32 channel1, u32 channel2, u32 channel3)
{
    material->colorChannels[0] = channel0;
    material->colorChannels[1] = channel1;
    material->colorChannels[2] = channel2;
    material->colorChannels[3] = channel3;
}
