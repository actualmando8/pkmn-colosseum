#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"

u8 dbgMenuGetEnable(void)
{
    return lbl_8047AED0;
}

void dbgMenuSetEnable(u8 enabled)
{
    lbl_8047AED0 = enabled;
}

u32 dbgMenuIsOpen(void)
{
    u8 result = menuIsCheck(lbl_80478848);
    return result != 0 ? 1 : 0;
}
