#include "dolphin/types.h"

extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern u8 lbl_8047AD70;
extern u8 lbl_8047AD71;

u32 fn_801174C4(void)
{
    u8 result = 0;

    if (lbl_8047AD68 != 0 && lbl_8047AD6C != 0) {
        result = 1;
    }
    return result;
}

u8 fn_801174EC(void)
{
    return lbl_8047AD71;
}

void fn_801174F4(void)
{
    lbl_8047AD71 = 0;
}

void fn_80117500(void)
{
    lbl_8047AD71 = 1;
    lbl_8047AD70 = 0;
}
