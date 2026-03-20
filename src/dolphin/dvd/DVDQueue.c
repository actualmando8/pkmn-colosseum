#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"

/*
 * DVDQueue.c - DVD command priority queue implementation.
 *
 * Manages a 4-priority-level doubly-linked waiting queue for DVD commands.
 * Priority 0 is highest, 3 is lowest.
 *
 * Matches: 0x800A7DE8 - 0x800A7F80
 */

/* Queue link node - embedded in DVDCommandBlock at offset 0x00/0x04 */
typedef struct DVDQueueNode {
    struct DVDQueueNode* next;
    struct DVDQueueNode* prev;
} DVDQueueNode;

/* 4 priority levels, each is a doubly-linked list sentinel */
/* Located at 0x803FC3F8 */
extern DVDQueueNode WaitingQueue[4];

/*
 * __DVDClearWaitingQueue - Initialize all 4 priority queues as empty
 * 0x800A7DE8 | size: 0x38
 *
 * Each queue is a circular doubly-linked list with a sentinel node.
 * Empty state: sentinel.next == sentinel.prev == &sentinel
 */
void __DVDClearWaitingQueue(void) {
    DVDQueueNode* q;
    s32 i;

    for (i = 0; i < 4; i++) {
        q = &WaitingQueue[i];
        q->next = q;
        q->prev = q;
    }
}

/*
 * __DVDPushWaitingQueue - Insert a command block at the tail of a priority queue
 * 0x800A7E20 | size: 0x68
 *
 * Parameters:
 *   prio  - priority level (0-3)
 *   block - command block to enqueue (links at offset 0x00/0x04)
 */
BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block) {
    BOOL enabled;
    DVDQueueNode* sentinel;
    DVDQueueNode* node = (DVDQueueNode*)block;
    DVDQueueNode* tail;

    enabled = OSDisableInterrupts();

    sentinel = &WaitingQueue[prio];
    tail = sentinel->prev;

    /* Insert before sentinel (at end of queue) */
    tail->next = node;
    node->prev = tail;
    node->next = sentinel;
    sentinel->prev = node;

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/*
 * __DVDPopWaitingQueue - Remove and return the highest-priority waiting command
 * 0x800A7E88 | size: 0xA0
 *
 * Scans priorities 0-3, returns the first non-empty queue's head entry.
 * Returns NULL if all queues are empty.
 */
DVDCommandBlock* __DVDPopWaitingQueue(void) {
    BOOL enabled;
    DVDQueueNode* sentinel;
    DVDQueueNode* node;
    s32 i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        sentinel = &WaitingQueue[i];

        if (sentinel->next == sentinel) {
            /* This priority level is empty */
            continue;
        }

        /* Found a non-empty queue */
        OSRestoreInterrupts(enabled);
        enabled = OSDisableInterrupts();

        sentinel = &WaitingQueue[i];
        node = sentinel->next;

        /* Remove from queue */
        sentinel->next = node->next;
        node->next->prev = sentinel;

        OSRestoreInterrupts(enabled);

        /* Clear links */
        node->next = NULL;
        node->prev = NULL;

        return (DVDCommandBlock*)node;
    }

    OSRestoreInterrupts(enabled);
    return NULL;
}

/*
 * __DVDCheckWaitingQueue - Check if any commands are waiting
 * 0x800A7F28 | size: 0x58
 *
 * Returns TRUE if at least one priority queue is non-empty.
 */
BOOL __DVDCheckWaitingQueue(void) {
    BOOL enabled;
    DVDQueueNode* sentinel;
    s32 i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        sentinel = &WaitingQueue[i];

        if (sentinel->next != sentinel) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}
