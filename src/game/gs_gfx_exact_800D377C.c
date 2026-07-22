#include "dolphin/types.h"

extern void fn_8019C690(s32 mode, s32 value);
extern s32 GSgfxVideoVsyncRate;

void fn_800D377C(s32 mode)
{
    switch (mode) {
    case 1:
        fn_8019C690(0, 0);
        break;
    case 2:
        fn_8019C690(1, 0);
        break;
    }
}

s32 fn_800D37CC(void)
{
    return GSgfxVideoVsyncRate;
}
