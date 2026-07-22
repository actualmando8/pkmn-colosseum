#include "dolphin/types.h"

extern s32 lbl_80478CB0;

s32 scriptSetEventColID(s32 id)
{
    s32 old = lbl_80478CB0;

    lbl_80478CB0 = id;
    return old;
}
