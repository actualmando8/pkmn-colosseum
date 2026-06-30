#include "dolphin/types.h"

typedef struct GXTexObj_800BAE5C {
    u8 pad_00[0x14];
    void* image;
} GXTexObj_800BAE5C;

void* fn_800BAE5C(GXTexObj_800BAE5C* texObj) {
    return texObj->image;
}
