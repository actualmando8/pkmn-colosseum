/**
 * @file gs_scratch.c
 * @brief GSscratch (scratch/ARAM-backed allocator)
 *
 * Split from gs_range_800E202C.c (0x800EE928-0x800EEF48) — one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"

typedef struct GSscratchAllocation {
    u8 firstBlock;
    u8 blockCount;
    u8 _pad[2];
    void (*callback)(BOOL valid, void *ptr, u8 blockCount);
} GSscratchAllocation;

extern GSscratchAllocation lbl_804018F0[32];
extern u8 *lbl_8047ABE0;
extern u32 lbl_8047ABEC;

static inline GSscratchAllocation *GSscratchFindAllocation(u8 firstBlock)
{
    GSscratchAllocation *allocation;
    u32 i;

    allocation = lbl_804018F0;
    for (i = 0; i < 32; allocation++, i++) {
        if (allocation->firstBlock == firstBlock) {
            return allocation;
        }
    }
    return NULL;
}

u8 GSscratchIsPtr(void *ptr)
{
    return ((u32)ptr & 0xF0000000) == ((u32)lbl_8047ABE0 & 0xF0000000);
}

void GSscratchFree(void *ptr)
{
    GSscratchAllocation *allocation;
    u32 blockMask;
    u8 firstBlock;
    u8 blockCount;
    u32 usedBlocks;

    allocation = GSscratchFindAllocation(
        ((u32)ptr - (u32)lbl_8047ABE0) >> 9);
    if (allocation == NULL) {
        return;
    }

    blockCount = allocation->blockCount;
    firstBlock = allocation->firstBlock;
    blockMask = 0x80000000;
    while (firstBlock-- != 0) {
        blockMask >>= 1;
    }

    usedBlocks = lbl_8047ABEC;
    while (blockCount-- != 0) {
        usedBlocks &= ~blockMask;
        blockMask >>= 1;
    }
    lbl_8047ABEC = usedBlocks;
    allocation->firstBlock = 0xFF;
}

extern void LCQueueWait(u32 len);
extern u32 lbl_8047ABDC;

void GSscratchWaitForCompletion(void)
{
    LCQueueWait(lbl_8047ABDC);
    lbl_8047ABDC = 0;
}
