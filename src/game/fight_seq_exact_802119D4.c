#include "dolphin/types.h"

extern u32 lbl_8047B618;
extern void fn_80213270(void);

void fightSeqPost(void)
{
    lbl_8047B618 &= ~0x00100000u;
    fn_80213270();
}
