#include "dolphin/types.h"

extern const f64 lbl_8047DD98;

void RObjUpdateFunc(u32* obj, s32 mode, f32* val_ptr)
{
    if (obj == NULL) {
        return;
    }
    if (mode != 1) {
        return;
    }
    if (*val_ptr >= lbl_8047DD98) {
        obj[1] |= 0x80000000u;
    } else {
        obj[1] &= ~0x80000000u;
    }
}
