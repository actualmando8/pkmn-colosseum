#include "dolphin/types.h"

typedef struct GSmaterialObject {
    u8 field_00[2];
    u16 enabledExtensions;
} GSmaterialObject;

u16 GSmaterialGetEnabledExtensions(const GSmaterialObject* material)
{
    return material->enabledExtensions;
}
