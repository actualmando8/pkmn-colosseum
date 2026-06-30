#include "dolphin/types.h"

extern u8 lbl_803FED70[];

u8 fn_800C39A0(void) {
    return lbl_803FED70[0];
}

void fn_800C39B0(u8 state) {
    lbl_803FED70[0] = state;
}
