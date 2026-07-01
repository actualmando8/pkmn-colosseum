#include "dolphin/types.h"

/* fn_800C7558 - 0x800C7558 | size: 0x24 */
s32 fn_800C7558(s32 ch) {
    extern u8 lbl_80313C18[];

    if (ch == -1) {
        return -1;
    }
    return lbl_80313C18[(u8)ch];
}
