#include "dolphin/types.h"
#include "game/fsys/fsys.h"

u32 fn_8017C568(void)
{
    return 1;
}

u32 fn_8017C570(void)
{
    return 1;
}

u32 fn_8017C578(void)
{
    return 0;
}

u32 fn_8017C580(FSYSSlot* slot)
{
    slot->status = 0x66;
    return 0;
}

u32 fn_8017C590(void)
{
    return 1;
}

u32 fn_8017C598(void)
{
    return 1;
}

u32 fn_8017C5A0(FSYSSlot* slot)
{
    slot->status = 0x66;
    return 0;
}

u32 fn_8017C5B0(void)
{
    return 1;
}
