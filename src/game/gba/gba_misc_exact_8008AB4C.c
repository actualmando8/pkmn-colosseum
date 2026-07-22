#include "dolphin/types.h"

extern s32 lbl_803FB308[4];
extern s32 lbl_803FB318[4];
extern u16 lbl_8047A67C[4];

void gbaCommandSendWazaText(s32 param0, s32 param1)
{
    extern void fn_80083D30(s32, void*);
    extern void fn_800733D0(s32, void*);
    u8 buf[0x780];

    fn_80083D30(param1, buf);
    fn_800733D0(param0 - 1, buf);
}

s32 fn_8008AB8C(s32 value)
{
    return lbl_8047A67C[value - 1];
}

u8 fn_8008ABA0(s32 index)
{
    u32 result = 0;

    if (lbl_803FB318[index - 1] != 0) {
        if (lbl_803FB308[index - 1] == 0) {
            result = 1;
        }
    }
    return (u8)result;
}
