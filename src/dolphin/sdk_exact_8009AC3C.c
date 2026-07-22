/* Canonical Dolphin arena heap invalidation helper. */
#include "dolphin/types.h"

typedef struct OSAllocCell {
    struct OSAllocCell* prev;
    struct OSAllocCell* next;
    s32 size;
} OSAllocCell;

typedef struct OSHeapDesc {
    s32 size;
    OSAllocCell* free;
    OSAllocCell* allocated;
} OSHeapDesc;

extern OSHeapDesc* lbl_8047A6E8;

void fn_8009AC3C(u32 xfb)
{
    lbl_8047A6E8[xfb].size = -1;
}
