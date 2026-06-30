#include "dolphin/types.h"

extern u8 lbl_803FCE08[];

void* TRKGetBuffer(s32 index) {
    void* result = NULL;

    if (index >= 0 && index < 3) {
        result = lbl_803FCE08 + index * 0x890;
    }

    return result;
}
