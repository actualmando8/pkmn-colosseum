#include "dolphin/types.h"

extern u32 lbl_8047B370;
extern void fn_800BC8C8(u32 value);

void fn_801B3770(void)
{
    fn_800BC8C8((u32)(u8)lbl_8047B370);
    lbl_8047B370 = 0;
}
