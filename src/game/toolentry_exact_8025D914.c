/** Exact tool-entry hero pointer accessor, 0x8025D914 - 0x8025D938. */
#include "dolphin/types.h"

extern u8* fn_8006B09C(s32 index);

void* toolentryTaisenGetHeroPtr(s32 index)
{
    return fn_8006B09C(index) + 0xB44;
}
