#include "dolphin/types.h"

extern void fn_80193AF0(void* ptr, s32 size);

void HSD_TExpFreeTevDesc(u8* node)
{
    u8* next;

    while (node != NULL) {
        next = *(u8**) node;
        fn_80193AF0(node, 0x88);
        node = next;
    }
}
