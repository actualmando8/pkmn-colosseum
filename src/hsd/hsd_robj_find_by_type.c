#include "dolphin/types.h"

u32* HSD_RObjGetByType(volatile u32* node, u32 type, u32 subtype)
{
    if (node == NULL) {
        return NULL;
    }
    for (; node != NULL; node = (volatile u32*)node[0]) {
        if (!(node[1] & 0x80000000u)) {
            continue;
        }
        if ((node[1] & 0x70000000) != type) {
            continue;
        }
        if ((subtype != 0) && (subtype != (node[1] & 0x0FFFFFFF))) {
            continue;
        }
        return (u32*)node;
    }
    return NULL;
}
