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
#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"
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

void DVDResume(void) {
    extern volatile s32 PauseFlag_8047A7F4;
    extern volatile s32 PausingFlag_8047A7F8;
    extern void stateReady_800A6684(void);
    BOOL enabled = OSDisableInterrupts();

    PauseFlag_8047A7F4 = FALSE;
    if (PausingFlag_8047A7F8 != FALSE) {
        PausingFlag_8047A7F8 = FALSE;
        stateReady_800A6684();
    }
    OSRestoreInterrupts(enabled);
}

extern OSThreadQueue __DVDThreadQueue;

BOOL DVDCancel(DVDCommandBlock* block) {
    extern BOOL DVDCancelAsync(DVDCommandBlock*, void (*)(void));
    extern void fn_800A7BA8(void);
    BOOL enabled;

    if (!DVDCancelAsync(block, fn_800A7BA8)) {
        return -1;
    }

    enabled = OSDisableInterrupts();
    for (;;) {
        s32 state = block->state;

        if ((u32)(state + 1) <= 1 || state == 10) {
            break;
        }
        if (state == 3) {
            u32 command = block->command;

            if ((u32)(command - 4) <= 1 || command == 13 || command == 15) {
                break;
            }
        }
        OSSleepThread(&__DVDThreadQueue);
    }
    OSRestoreInterrupts(enabled);
    return 0;
}

extern OSThreadQueue __DVDThreadQueue;

void fn_800A7BA8(void) {
    OSWakeupThread(&__DVDThreadQueue);
}

void* fn_800A7BCC(void) {
    return (void*)0x80000000;
}

extern volatile u32 __DIRegs[16] : 0xCC006000;

BOOL DVDCheckDisk(void) {
    extern s32 FatalErrorFlag_8047A800;
    extern s32 PausingFlag_8047A7F8;
    extern DVDCommandBlock* executing_8047A7E8;
    extern DVDCommandBlock DummyCommandBlock_803FC3A0;
    extern u32 ResumeFromHere_8047A810;
    BOOL enabled;
    s32 result;
    s32 state;
    u32 cover;

    enabled = OSDisableInterrupts();

    if (FatalErrorFlag_8047A800) {
        state = -1;
    } else if (PausingFlag_8047A7F8) {
        state = 8;
    } else if (executing_8047A7E8 == NULL) {
        state = 0;
    } else if (executing_8047A7E8 == &DummyCommandBlock_803FC3A0) {
        state = 0;
    } else {
        state = executing_8047A7E8->state;
    }

    switch (state) {
    case 1:
    case 2:
    case 9:
    case 10:
        result = TRUE;
        break;
    case -1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 11:
        result = FALSE;
        break;
    case 0:
    case 8:
        cover = __DIRegs[1];
        if (((cover >> 2) & 1) || (cover & 1)) {
            result = FALSE;
        } else if (ResumeFromHere_8047A810 != 0) {
            result = FALSE;
        } else {
            result = TRUE;
        }
        break;
    }

    OSRestoreInterrupts(enabled);
    return result;
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
