#include "dolphin/types.h"

extern u32 fn_800BE31C(void);
extern u32 lbl_8047ACF8;
extern u32 lbl_8047ACF0;

typedef void (*GappBreakPointCallback)(u32 token);
extern GappBreakPointCallback lbl_8047ACF4;

void fn_80101B34(u32 param)
{
    u32 value = param;

    if ((u16)value == 3) {
        lbl_8047ACF8 = fn_800BE31C();
    }

    {
        GappBreakPointCallback callback = lbl_8047ACF4;
        if (callback != 0) {
            callback(value);
        }
    }
}

void fn_80101B88(u32 value)
{
    lbl_8047ACF0 = value;
}
