#include "dolphin/types.h"

extern u8 lbl_8044FB90[];

void fn_80163490(void)
{
    u8* queue = lbl_8044FB90;

    /* Completion callbacks can decrement this count asynchronously. */
    while (*(volatile u8*)(queue + 0x281) != 0) {
    }
}
