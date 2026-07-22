/* Canonical Dolphin low-arena allocator. */
#include "dolphin/types.h"

typedef void OSArenaAddress;

extern OSArenaAddress* __OSArenaLo;

void* OSAllocFromArenaLo(u32 size, u32 align)
{
    u32 am1 = align - 1;
    u32 mask = ~am1;
    OSArenaAddress* ptr;

    ptr = (OSArenaAddress*) (((u32) __OSArenaLo + am1) & mask);
    size = (u32) ptr + size;
    size += am1;
    __OSArenaLo = (OSArenaAddress*) (mask & size);
    return ptr;
}
