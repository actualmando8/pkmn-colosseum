#include "dolphin/types.h"

extern u32 lbl_8047B078;

void fn_80163BCC(u8* unused, u32 size)
{
    lbl_8047B078 -= (size + 0x1F) & ~0x1FU;
}
