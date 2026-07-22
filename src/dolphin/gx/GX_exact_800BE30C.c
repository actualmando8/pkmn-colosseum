#include "dolphin/gx/GXInternal.h"

void fn_800BE30C(void)
{
    __cpReg[2] = 4;
}

u32 fn_800BE31C(void)
{
    u32 hi;
    u32 lo;

    fn_800BE164(&hi, &lo);
    return hi;
}
