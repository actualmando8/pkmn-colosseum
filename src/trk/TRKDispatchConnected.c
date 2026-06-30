#include "dolphin/types.h"

extern u32 lbl_803FE7D0[];

void fn_800C04E8(u32 state) {
    lbl_803FE7D0[0] = state;
}

u32 fn_800C04F4(void) {
    return lbl_803FE7D0[0];
}
