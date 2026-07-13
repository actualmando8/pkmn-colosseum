/**
 * @file sdk_range_800A7820.c
 * @brief dolphin-sdk code, 0x800A7820 - 0x800A7F28 (11 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/os/OSThread.h"

typedef struct DVDQueueNode {
    struct DVDQueueNode* next;
    struct DVDQueueNode* prev;
} DVDQueueNode;

s32 fn_800A7820(s32 arg0) {
    extern s32 autoInvalidation_804789CC;
    s32 oldValue = autoInvalidation_804789CC;

    autoInvalidation_804789CC = arg0;
    return oldValue;
}

extern OSThreadQueue __DVDThreadQueue;

void fn_800A7BA8(void) {
    OSWakeupThread(&__DVDThreadQueue);
}

void* fn_800A7BCC(void) {
    return (void*)0x80000000;
}

extern DVDQueueNode WaitingQueue_803FC3F8[4];

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
