/**
 * @file sdk_range_8009A2D8.c
 * @brief dolphin-sdk code, 0x8009A2D8 - 0x8009AFB0 (13 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/os/OSAlarm.h"

typedef struct {
    s32 unk0;
    struct FreeBlock* unk4;
    struct FreeBlock* unk8;
} AlarmCallback;

typedef struct FreeBlock {
    struct FreeBlock* prev;
    struct FreeBlock* next;
    s32 size;
} FreeBlock;

extern u32 lbl_80478980;
extern AlarmCallback* lbl_8047A6E8;

void OSSetAlarm(OSAlarm* alarm, s64 tick, OSAlarmHandler handler) {
    BOOL enabled;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL enabled);
    extern s64 __OSGetSystemTime(void);
    extern void InsertAlarm(OSAlarm* alarm, s64 fire, OSAlarmHandler handler);

    enabled = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
    OSRestoreInterrupts(enabled);
}

FreeBlock* fn_8009A92C(FreeBlock* head, FreeBlock* block) {
    FreeBlock* prev;
    FreeBlock* next;

    next = head;
    prev = NULL;
    while (next != NULL) {
        if (block <= next) {
            break;
        }
        prev = next;
        next = next->next;
    }

    block->next = next;
    block->prev = prev;
    if (next != NULL) {
        next->prev = block;
        if ((u8*)block + block->size == (u8*)next) {
            block->size += next->size;
            next = next->next;
            block->next = next;
            if (next != NULL) {
                next->prev = block;
            }
        }
    }

    if (prev != NULL) {
        prev->next = block;
        if ((u8*)prev + prev->size == (u8*)block) {
            prev->size += block->size;
            prev->next = next;
            if (next != NULL) {
                next->prev = prev;
            }
        }
        return head;
    }
    return block;
}

void* fn_8009A9D8(s32 heap, s32 size) {
    AlarmCallback* descriptor;
    FreeBlock* remainder;
    FreeBlock* block;
    FreeBlock* next;
    FreeBlock* free;
    u32 remainderSize;

    descriptor = &lbl_8047A6E8[heap];
    size = (size + 63) & ~31;
    block = descriptor->unk4;
    while (block != NULL) {
        if (size <= block->size) {
            break;
        }
        block = block->next;
    }

    if (block == NULL) {
        return NULL;
    }

    remainderSize = block->size - size;
    if (remainderSize < 64) {
        next = block->next;
        free = descriptor->unk4;
        if (next != NULL) {
            next->prev = block->prev;
        }
        if (block->prev == NULL) {
            free = block->next;
        } else {
            block->prev->next = block->next;
        }
        descriptor->unk4 = free;
    } else {
        block->size = size;
        remainder = (FreeBlock*)((u8*)block + size);
        remainder->size = remainderSize;
        remainder->prev = block->prev;
        remainder->next = block->next;
        if (remainder->next != NULL) {
            remainder->next->prev = remainder;
        }
        if (remainder->prev != NULL) {
            remainder->prev->next = remainder;
        } else {
            descriptor->unk4 = remainder;
        }
    }

    {
        FreeBlock* allocated;

        allocated = descriptor->unk8;
        block->next = allocated;
        block->prev = NULL;
        if (allocated != NULL) {
            allocated->prev = block;
        }
    }
    descriptor->unk8 = block;
    return (u8*)block + 32;
}

void fn_8009AAD4(s32 heap, void* ptr) {
    FreeBlock* allocated;
    AlarmCallback* descriptor;
    FreeBlock* block;

    block = (FreeBlock*)((u8*)ptr - 32);
    descriptor = &lbl_8047A6E8[heap];
    allocated = descriptor->unk8;
    if (block->next != NULL) {
        block->next->prev = block->prev;
    }
    if (block->prev == NULL) {
        allocated = block->next;
    } else {
        block->prev->next = block->next;
    }
    descriptor->unk8 = allocated;
    descriptor->unk4 = fn_8009A92C(descriptor->unk4, block);
}


typedef struct {
    s32 unk0;
    u32 unk4;
    u32 unk8;
} AlarmCallback;

extern u32 lbl_80478980;
extern AlarmCallback* lbl_8047A6E8;

u32 fn_8009AB50(u32 xfb) {
    u32 previous = lbl_80478980;

    lbl_80478980 = xfb;
    return previous;
}

void* fn_8009AB60(void* arenaStart, void* arenaEnd, s32 maxHeaps) {
    s32 heapsSize;
    s32 i;
    AlarmCallback* heap;
    void* result;
    extern s32 lbl_8047A6EC;
    extern void* lbl_8047A6F0;
    extern void* lbl_8047A6F4;

    heapsSize = maxHeaps * sizeof(AlarmCallback);
    lbl_8047A6E8 = arenaStart;
    lbl_8047A6EC = maxHeaps;
    for (i = 0; i < lbl_8047A6EC; i++) {
        heap = &lbl_8047A6E8[i];
        heap->unk0 = -1;
        heap->unk4 = heap->unk8 = NULL;
    }

    arenaEnd = (void*)((u32)arenaEnd & ~31);
    lbl_8047A6F4 = arenaEnd;
    lbl_80478980 = -1;
    arenaStart = (u8*)lbl_8047A6E8 + heapsSize;
    result = (void*)(((u32)arenaStart + 31) & ~31);
    lbl_8047A6F0 = result;
    return result;
}

s32 fn_8009ABD0(void* start, void* end) {
    AlarmCallback* descriptor;
    s32 count;
    s32 heap;
    FreeBlock* block;
    extern s32 lbl_8047A6EC;

    start = (void*)(((u32)start + 31) & ~31);
    end = (void*)((u32)end & ~31);
    descriptor = lbl_8047A6E8;
    count = lbl_8047A6EC;
    for (heap = 0; heap < count; descriptor++, heap++) {
        if (descriptor->unk0 < 0) {
            descriptor->unk0 = (u8*)end - (u8*)start;
            block = start;
            block->prev = NULL;
            block->next = NULL;
            block->size = descriptor->unk0;
            descriptor->unk4 = block;
            descriptor->unk8 = NULL;
            return heap;
        }
    }
    return -1;
}

void fn_8009AC3C(u32 xfb) {
    lbl_8047A6E8[xfb].unk0 = -1;
}
