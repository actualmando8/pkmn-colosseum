#include "dolphin/types.h"

extern void aramSetUploadCallback(u8* address, u32 size);

void fn_801630E4(u8* address, u32 size)
{
    aramSetUploadCallback(address, size);
}
