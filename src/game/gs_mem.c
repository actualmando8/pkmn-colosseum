/**
 * @file gs_mem.c
 * @brief GSmem (heap allocator)
 *
 * Split from gs_range_800E202C.c (0x800E202C-0x800E3604) — one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"
#include "game/gs_mem.h"

extern u8 lbl_8047AB28;
extern u32 lbl_8047AB2C;
extern GSmemBlock* lbl_8047AB30;
extern GSmemEntry* lbl_8047AB34;
extern GSmemEntry* lbl_8047AB38;
extern u32 lbl_8047AB48;
extern u32 lbl_8047AB4C;
extern u32 lbl_8047AB50;
extern u32 lbl_8047AB54;
extern u32 lbl_8047AB58;
extern u32 lbl_8047AB5C;
extern u32 lbl_8047AB60;
extern void* lbl_8047AB64;
extern void* lbl_8047AB68;
extern char lbl_80270DFC[];

extern void GSlogWrite(const char* format, ...);
extern u16 fn_800E2C04(u32 size, u32 alignment);

u16 fn_800E202C(void* ptr)
{
    GSmemEntry* entry;
    u32 count;

    if (lbl_8047AB28 == 1) {
        ptr = (u8*)ptr - 4;
    }

    entry = lbl_8047AB34;
    count = ((u32)lbl_8047AB34 + sizeof(GSmemEntry) -
             (u32)lbl_8047AB38) /
            sizeof(GSmemEntry);
    while (entry >= lbl_8047AB38) {
        if (entry->handle != 0 && entry->data == ptr) {
            break;
        }
        entry--;
        if (--count == 0) {
            break;
        }
    }
    if (entry == NULL || entry < lbl_8047AB38) {
        return 0;
    }
    return entry->handle;
}

s32 fn_800E2AF8(void)
{
    return 1;
}

void fn_800E3560(u32 value)
{
    lbl_8047AB2C = value;
}

u16 _toolentryAlloc__FUl(u32 size)
{
    size = (size + 0x1F) & ~0x1F;
    return fn_800E2C04(size, 0x20);
}

void GSmemInit(u32 heapId, void* start, void* end)
{
    GSmemBlock* block;
    GSmemEntry* entry;
    void* alignedStart = (void*)(((u32)start + 0x1F) & ~0x1F);
    void* alignedEnd = (void*)((u32)end & ~0x1F);

    lbl_8047AB28 = heapId;
    lbl_8047AB68 = alignedStart;
    lbl_8047AB64 = alignedEnd;
    lbl_8047AB60 = 0;
    lbl_8047AB5C = 0;
    lbl_8047AB58 = 0;
    lbl_8047AB54 = 0;
    lbl_8047AB50 = 0;
    lbl_8047AB4C = 0;
    lbl_8047AB48 = 0;

    entry = (GSmemEntry*)((u8*)alignedEnd - sizeof(GSmemEntry));
    lbl_8047AB34 = entry;
    lbl_8047AB38 = entry;
    ((GSmemEntry*)((u8*)alignedEnd - sizeof(GSmemEntry)))->handle = 0;

    block = lbl_8047AB68;
    block->prev = NULL;
    block->next = NULL;
    block->size = (u8*)lbl_8047AB38 - (u8*)lbl_8047AB68;
    lbl_8047AB30 = block;
    GSlogWrite(lbl_80270DFC, lbl_8047AB68, lbl_8047AB64);
}
