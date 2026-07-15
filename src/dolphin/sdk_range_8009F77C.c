/**
 * @file sdk_range_8009F77C.c
 * @brief dolphin-sdk code, 0x8009F77C - 0x8009FAEC (8 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct {
    void* head;
    void* tail;
} OSThreadQueue;

typedef struct {
    OSThreadQueue queue;
    void* unk_08;
    void* unk_0C;
} fn_8009F77C_Worker;

extern void OSInitThreadQueue(OSThreadQueue* queue);
extern void OSWakeupThread(void* queue);

void fn_8009F77C(fn_8009F77C_Worker* arg) {
    OSInitThreadQueue(&arg->queue);
    arg->unk_08 = 0;
    arg->unk_0C = 0;
}

void fn_8009F7B4(fn_8009F77C_Worker* mutex) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL enabled);
    extern fn_8009F7B4_Thread* fn_800A13F8(void);
    extern void fn_800A16E8(fn_8009F7B4_Thread* thread, s32 priority);
    extern void OSSleepThread(OSThreadQueue* queue);
    BOOL enabled;
    fn_8009F7B4_Thread* current;

    enabled = OSDisableInterrupts();
    current = fn_800A13F8();

    for (;;) {
        if (mutex->unk_08 == NULL) {
            fn_8009F77C_Worker* tail;

            mutex->unk_08 = current;
            mutex->unk_0C++;
            tail = current->queueMutex.tail;
            if (tail == NULL) {
                current->queueMutex.head = mutex;
            } else {
                tail->link.next = mutex;
            }
            mutex->link.prev = tail;
            mutex->link.next = NULL;
            current->queueMutex.tail = mutex;
            break;
        }

        if (mutex->unk_08 == current) {
            mutex->unk_0C++;
            break;
        }

        current->mutex = mutex;
        fn_800A16E8(mutex->unk_08, current->priority);
        OSSleepThread(&mutex->queue);
        current->mutex = NULL;
    }

    OSRestoreInterrupts(enabled);
}

void fn_8009F890(fn_8009F77C_Worker* mutex) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL enabled);
    extern fn_8009F7B4_Thread* fn_800A13F8(void);
    extern s32 __OSGetEffectivePriority(fn_8009F7B4_Thread* thread);
    BOOL enabled;
    fn_8009F7B4_Thread* current;

    enabled = OSDisableInterrupts();
    current = fn_800A13F8();

    if (mutex->unk_08 == current && --mutex->unk_0C == 0) {
        fn_8009F77C_Worker* next = mutex->link.next;
        fn_8009F77C_Worker* prev = mutex->link.prev;

        if (next == NULL) {
            current->queueMutex.tail = prev;
        } else {
            next->link.prev = prev;
        }
        if (prev == NULL) {
            current->queueMutex.head = next;
        } else {
            prev->link.next = next;
        }

        mutex->unk_08 = NULL;
        if (current->priority < current->base) {
            current->priority = __OSGetEffectivePriority(current);
        }
        OSWakeupThread(&mutex->queue);
    }

    OSRestoreInterrupts(enabled);
}

void __OSUnlockAllMutex(fn_8009F7B4_Thread* thread) {
    fn_8009F77C_Worker* mutex;
    fn_8009F77C_Worker* next;

    while (thread->queueMutex.head != NULL) {
        mutex = thread->queueMutex.head;
        next = mutex->link.next;
        if (next == NULL) {
            thread->queueMutex.tail = NULL;
        } else {
            next->link.prev = NULL;
        }
        thread->queueMutex.head = next;
        mutex->unk_0C = 0;
        mutex->unk_08 = NULL;
        OSWakeupThread(&mutex->queue);
    }
}

void fn_8009F9C8(void* queue) {
    OSInitThreadQueue((OSThreadQueue*)queue);
}

void fn_8009F9E8(OSThreadQueue* queue, fn_8009F77C_Worker* mutex) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL enabled);
    extern fn_8009F7B4_Thread* fn_800A13F8(void);
    extern s32 __OSGetEffectivePriority(fn_8009F7B4_Thread* thread);
    extern s32 OSDisableScheduler(void);
    extern s32 OSEnableScheduler(void);
    extern void OSSleepThread(OSThreadQueue* queue);
    BOOL enabled;
    fn_8009F7B4_Thread* current;

    enabled = OSDisableInterrupts();
    current = fn_800A13F8();

    if (mutex->unk_08 == current) {
        s32 count = mutex->unk_0C;
        fn_8009F77C_Worker* next;
        fn_8009F77C_Worker* prev;

        mutex->unk_0C = 0;
        next = mutex->link.next;
        prev = mutex->link.prev;
        if (next == NULL) {
            current->queueMutex.tail = prev;
        } else {
            next->link.prev = prev;
        }
        if (prev == NULL) {
            current->queueMutex.head = next;
        } else {
            prev->link.next = next;
        }

        mutex->unk_08 = NULL;
        if (current->priority < current->base) {
            current->priority = __OSGetEffectivePriority(current);
        }
        OSDisableScheduler();
        OSWakeupThread(&mutex->queue);
        OSEnableScheduler();
        OSSleepThread(queue);
        fn_8009F7B4(mutex);
        mutex->unk_0C = count;
    }

    OSRestoreInterrupts(enabled);
}

void fn_8009FABC(void* queue) {
    OSWakeupThread(queue);
}
