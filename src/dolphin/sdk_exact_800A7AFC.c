/**
 * @file sdk_exact_800A7AFC.c
 * @brief Exact Dolphin DVD cancellation helpers, 0x800A7AFC - 0x800A7BD4.
 */
#include "dolphin/types.h"
#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSThread.h"

extern OSThreadQueue __DVDThreadQueue;
extern BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback);

static void fn_800A7BA8(s32 result, DVDCommandBlock* block);

BOOL DVDCancel(DVDCommandBlock* block) {
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

static void fn_800A7BA8(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

void* fn_800A7BCC(void) {
    return (void*)0x80000000;
}
