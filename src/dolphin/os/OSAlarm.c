#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/PPCArch.h"

/*
 * OSAlarm.c - Alarm (timer callback) system.
 *
 * Manages a sorted linked list of alarms that fire at specific times
 * using the PowerPC decrementer exception.
 *
 * Matches: 0x8009A27C - 0x8009A92C
 */

typedef struct OSAlarmQueue {
    OSAlarm* head;
    OSAlarm* tail;
} OSAlarmQueue;

static OSAlarmQueue AlarmQueue;

static void InsertAlarm(OSAlarm* alarm, s64 fire, OSAlarmHandler handler);
static void DecrementerExceptionCallback(u8 exception, OSContext* context);

/* DecrementerExceptionHandler - asm stub that saves regs and calls callback */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
static asm void DecrementerExceptionHandler(register u8 exception, register OSContext* context) {
    nofralloc
    stw     r0,   0x0000(r4)
    stw     r1,   0x0004(r4)
    stw     r2,   0x0008(r4)
    stmw    r6,   0x0018(r4)
    mfspr   r0, GQR1
    stw     r0, 0x01A8(r4)
    mfspr   r0, GQR2
    stw     r0, 0x01AC(r4)
    mfspr   r0, GQR3
    stw     r0, 0x01B0(r4)
    mfspr   r0, GQR4
    stw     r0, 0x01B4(r4)
    mfspr   r0, GQR5
    stw     r0, 0x01B8(r4)
    mfspr   r0, GQR6
    stw     r0, 0x01BC(r4)
    mfspr   r0, GQR7
    stw     r0, 0x01C0(r4)
    stwu    r1, -0x0008(r1)
    b       DecrementerExceptionCallback
}
#pragma pop

void OSInitAlarm(void) {
    if (__OSGetExceptionHandler(OS_EXCEPTION_DECREMENTER)
        == (__OSExceptionHandler)DecrementerExceptionHandler) {
        return;
    }

    AlarmQueue.tail = NULL;
    AlarmQueue.head = NULL;
    __OSSetExceptionHandler(OS_EXCEPTION_DECREMENTER,
                            (__OSExceptionHandler)DecrementerExceptionHandler);
}

void OSCreateAlarm(OSAlarm* alarm) {
    alarm->handler = NULL;
    alarm->tag     = 0;
}

static void SetTimer(OSAlarm* alarm) {
    s64 now;
    s64 diff;

    now  = __OSGetSystemTime();
    diff = alarm->fire - now;

    if (diff < 0) {
        PPCMtdec(0);
    } else if (diff < 0x80000000LL) {
        PPCMtdec((u32)diff);
    } else {
        PPCMtdec(0x7FFFFFFF);
    }
}

static void InsertAlarm(OSAlarm* alarm, s64 fire, OSAlarmHandler handler) {
    OSAlarm* iter;
    OSAlarm* prev;

    /* Handle periodic alarm scheduling */
    if (alarm->period != 0) {
        s64 now = __OSGetSystemTime();
        if (now >= alarm->start) {
            /* We've missed at least one tick; advance to the next period */
            fire = alarm->start + alarm->period *
                   (((now - alarm->start) / alarm->period) + 1);
        }
    }

    alarm->handler = handler;
    alarm->fire    = fire;

    /* Insert into sorted list (sorted by fire time, ascending) */
    for (iter = AlarmQueue.head; iter != NULL; iter = iter->next) {
        if (iter->fire <= fire) {
            continue;
        }

        /* Insert before iter */
        alarm->prev = iter->prev;
        iter->prev  = alarm;
        alarm->next = iter;

        if (alarm->prev != NULL) {
            alarm->prev->next = alarm;
        } else {
            AlarmQueue.head = alarm;
            SetTimer(alarm);
        }
        return;
    }

    /* Insert at tail */
    alarm->next = NULL;
    prev = AlarmQueue.tail;
    AlarmQueue.tail = alarm;
    alarm->prev = prev;

    if (prev != NULL) {
        prev->next = alarm;
    } else {
        AlarmQueue.head = alarm;
        AlarmQueue.tail = alarm;
        SetTimer(alarm);
    }
}

void OSSetAlarm(OSAlarm* alarm, s64 tick, OSAlarmHandler handler) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    alarm->period = 0;

    InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
    OSRestoreInterrupts(enabled);
}

static void DecrementerExceptionCallback(u8 exception, OSContext* context) {
    OSAlarm*        alarm;
    OSAlarmHandler  handler;
    s64             now;
    OSContext       tmpCtx;

    now   = __OSGetSystemTime();
    alarm = AlarmQueue.head;

    if (alarm == NULL) {
        OSLoadContext(context);
    }

    /* Check if the first alarm should fire */
    if (alarm->fire > now) {
        /* Not time yet; set the decrementer and return */
        SetTimer(alarm);
        OSLoadContext(context);
    }

    /* Remove from queue head */
    AlarmQueue.head = alarm->next;
    if (alarm->next != NULL) {
        alarm->next->prev = NULL;
    } else {
        AlarmQueue.tail = NULL;
    }

    handler = alarm->handler;
    alarm->handler = NULL;

    /* Reschedule periodic alarms */
    if (alarm->period != 0) {
        InsertAlarm(alarm, 0, handler);
    }

    /* Set timer for the new head alarm if any */
    if (AlarmQueue.head != NULL) {
        SetTimer(AlarmQueue.head);
    }

    /* Call the alarm handler */
    OSDisableScheduler();
    OSClearContext(&tmpCtx);
    OSSetCurrentContext(&tmpCtx);
    handler(alarm, context);
    OSClearContext(&tmpCtx);
    OSSetCurrentContext(context);
    OSEnableScheduler();
    __OSReschedule();
    OSLoadContext(context);
}

/* ===================================================================
 * Stub functions for coverage -- TODO: decompile
 * 1 function(s)
 * =================================================================== */

/* fn_8009A590 - 0x8009A590 | size: 0x11C
 * OSCancelAlarm - Remove an alarm from the alarm queue.
 * Unlinks it from the doubly-linked list, and if the removed alarm
 * was at the head, re-programs the decrementer for the new head.
 * alarm+0x00 = handler, alarm+0x08 = fire (hi), alarm+0x0C = fire (lo),
 * alarm+0x10 = prev, alarm+0x14 = next
 */
void fn_8009A590(u8* alarm) {
    extern u8 AlarmQueue_8047A6E0[];
    BOOL enabled;
    u8* next;
    u8* prev;

    enabled = OSDisableInterrupts();

    /* If handler is NULL, alarm isn't active */
    if (*(u32*)(alarm + 0x0) == 0) {
        OSRestoreInterrupts(enabled);
        return;
    }

    next = (u8*)*(u32*)(alarm + 0x14);
    prev = (u8*)*(u32*)(alarm + 0x10);

    /* Unlink from list */
    if (next == NULL) {
        /* Alarm is tail - update queue tail */
        *(u32*)(AlarmQueue_8047A6E0 + 0x4) = (u32)prev;
    } else {
        /* Update next's prev pointer */
        *(u32*)(next + 0x10) = (u32)prev;
    }

    if (prev != NULL) {
        /* Update prev's next pointer */
        *(u32*)(prev + 0x14) = (u32)next;
    } else {
        /* Alarm was head - update queue head and reprogram decrementer */
        *(u32*)AlarmQueue_8047A6E0 = (u32)next;
        if (next != NULL) {
            /* Set decrementer to 0 to trigger immediate re-evaluation */
            PPCMtdec(0);
        }
    }

    /* Mark alarm as inactive */
    *(u32*)(alarm + 0x0) = 0;
    OSRestoreInterrupts(enabled);
}

