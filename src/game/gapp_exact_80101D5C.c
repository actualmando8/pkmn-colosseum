#include "dolphin/types.h"

extern u32 lbl_8047ACF0;
extern void fn_800B8C58(s32);

void fn_80101D5C(void)
{
    if ((s32)lbl_8047ACF0 != 0) {
        fn_800B8C58(3);
    }
}
