#include "dolphin/types.h"

typedef struct HSDMaterialObject {
    u8 field_00[0x10];
    u32 peDescriptor;
} HSDMaterialObject;

typedef struct GSmaterialObject {
    u8 field_00[8];
    HSDMaterialObject* materialObject;
    u8 field_0C[0x30];
    u32 savedPeDescriptor;
} GSmaterialObject;

void GSmaterialResetPEdescr(GSmaterialObject* material)
{
    u32 descriptor = material->savedPeDescriptor;

    if (descriptor + 0x01020000 == 0xFEFE) {
        return;
    }
    material->materialObject->peDescriptor = descriptor;
    material->savedPeDescriptor = 0xFEFEFEFE;
}
