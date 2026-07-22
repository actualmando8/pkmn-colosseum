#include "dolphin/types.h"

extern void* lbl_8047B2E0;

void _HSD_ObjAllocForgetMemory(void)
{
    lbl_8047B2E0 = NULL;
}
