#include "dolphin/types.h"

extern u32 lbl_8047B618;

u32 fightSeqGetEffectAminFlag(void)
{
    return !(lbl_8047B618 & 0x80);
}
