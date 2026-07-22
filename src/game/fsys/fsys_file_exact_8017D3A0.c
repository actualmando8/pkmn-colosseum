#include "dolphin/types.h"
#include "game/fsys/fsys.h"

u32 fn_8017D3A0(FSYSSlot* slot)
{
    switch (slot->padding054) {
    case 4:
    case 5:
    case 11:
        return 1;
    default:
        return 0;
    }
}
