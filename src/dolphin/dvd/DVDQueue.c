#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"

typedef struct DVDQueueNode {
    struct DVDQueueNode* next;
    struct DVDQueueNode* prev;
} DVDQueueNode;

DVDQueueNode WaitingQueue_803FC3F8[4];

void __DVDClearWaitingQueue(void) {
    DVDQueueNode* queue;

    queue = WaitingQueue_803FC3F8;
    queue->next = queue;
    queue->prev = queue;

    queue = &WaitingQueue_803FC3F8[1];
    queue->next = queue;
    queue->prev = queue;

    queue = &WaitingQueue_803FC3F8[2];
    queue->next = queue;
    queue->prev = queue;

    queue = &WaitingQueue_803FC3F8[3];
    queue->next = queue;
    queue->prev = queue;
}

BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block) {
    BOOL enabled;
    DVDQueueNode* queue;

    enabled = OSDisableInterrupts();
    queue = &WaitingQueue_803FC3F8[prio];
    queue->prev->next = (DVDQueueNode*)block;
    block->prev = (DVDCommandBlock*)queue->prev;
    block->next = (DVDCommandBlock*)queue;
    queue->prev = (DVDQueueNode*)block;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

DVDCommandBlock* __DVDPopWaitingQueue(void) {
    BOOL enabled;
    DVDQueueNode* queue;
    DVDCommandBlock* block;
    s32 i;

    enabled = OSDisableInterrupts();
    for (i = 0; i < 4; i++) {
        queue = &WaitingQueue_803FC3F8[i];
        if (queue->next != queue) {
            OSRestoreInterrupts(enabled);
            enabled = OSDisableInterrupts();
            queue = &WaitingQueue_803FC3F8[i];
            block = (DVDCommandBlock*)queue->next;
            queue->next = (DVDQueueNode*)block->next;
            block->next->prev = (DVDCommandBlock*)queue;
            OSRestoreInterrupts(enabled);
            block->next = NULL;
            block->prev = NULL;
            return block;
        }
    }
    OSRestoreInterrupts(enabled);
    return NULL;
}

BOOL __DVDCheckWaitingQueue(void) {
    BOOL enabled;
    DVDQueueNode* sentinel;
    s32 i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        sentinel = &WaitingQueue_803FC3F8[i];

        if (sentinel->next != sentinel) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}

u32 __DVDDequeueWaitingQueue(u8* node) {
    BOOL enabled;
    u32 prev;
    u32 next;

    enabled = OSDisableInterrupts();
    prev = *(u32*)(node + 0x4);
    next = *(u32*)(node + 0x0);

    if (prev == 0 || next == 0) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    *(u32*)((u8*)prev + 0x0) = next;
    *(u32*)((u8*)next + 0x4) = prev;
    OSRestoreInterrupts(enabled);
    return 1;
}
