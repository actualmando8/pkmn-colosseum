/* Canonical HSD allocator/free forwarding entry points. */
#include "dolphin/types.h"

extern void* fn_801A6928(s32 size);
extern void fn_801A6960(void* mem);

void fn_80193AF0(void* mem, s32 size)
{
    fn_801A6960(mem);
}

void* fn_80193B10(s32 size)
{
    return fn_801A6928(size);
}
