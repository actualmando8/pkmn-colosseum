#include "dolphin/os/OSReset.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/PPCArch.h"

/* SDA symbol aliases used by stub functions */
extern u32 ResetFunctionQueue_8047A738;

/*
 * OSReset.c - System reset and reboot functionality.
 *
 * Adapted from doldecomp/melee and zeldaret/tp matching implementations.
 *
 * Matches: 0x8009FAF8 - 0x800A03B4
 */

#define ENQUEUE_INFO(info, queue)                            \
    do {                                                     \
        OSResetFunctionInfo* __prev = (queue)->tail;         \
        if (__prev == 0) {                                   \
            (queue)->head = (info);                          \
        } else {                                             \
            __prev->next = (info);                           \
        }                                                    \
        (info)->prev = __prev;                               \
        (info)->next = 0;                                    \
        (queue)->tail = (info);                              \
    } while(0);

#define ENQUEUE_INFO_PRIO(info, queue)               \
    do {                                             \
        OSResetFunctionInfo* __prev;                 \
        OSResetFunctionInfo* __next;                 \
        for(__next = (queue)->head; __next           \
          && (__next->priority <= (info)->priority); \
                __next = __next->next) ;             \
                                                     \
        if (__next == 0) {                           \
            ENQUEUE_INFO(info, queue);               \
        } else {                                     \
            (info)->next = __next;                   \
            __prev = __next->prev;                   \
            __next->prev = (info);                   \
            (info)->prev = __prev;                   \
            if (__prev == 0) {                       \
                (queue)->head = (info);              \
            } else {                                 \
                __prev->next = (info);               \
            }                                        \
        }                                            \
    } while(0);

typedef struct OSResetFunctionQueue {
    OSResetFunctionInfo* head;
    OSResetFunctionInfo* tail;
} OSResetFunctionQueue;

static OSResetFunctionQueue ResetFunctionQueue;

extern void __OSReboot(u32 resetCode, u32 bootDol);
extern void __OSStopAudioSystem(void);
extern BOOL __OSSyncSram(void);
extern void* __OSLockSram(void);
extern void __OSUnlockSram(BOOL commit);
extern BOOL __PADDisableRecalibration(BOOL disable);
extern void LCDisable(void);
extern void ICFlashInvalidate(void);
extern void* memset(void* dest, int val, u32 n);

#define __VIRegs     ((volatile u32*)0xCC002000)
#define __PIRegs     ((volatile u32*)0xCC003000)

static int CallResetFunctions(int final);
static void CancelThreads(void);

void OSRegisterResetFunction(OSResetFunctionInfo* info) {
    ENQUEUE_INFO_PRIO(info, &ResetFunctionQueue);
}

static int CallResetFunctions(int final) {
    OSResetFunctionInfo* info;
    int err = 0;

    for (info = ResetFunctionQueue.head; info; info = info->next) {
        err |= !info->func(final);
    }
    err |= !__OSSyncSram();
    if (err) {
        return 0;
    }
    return 1;
}

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
static asm void Reset(u32 resetCode) {
    nofralloc
    b       _skip1
_cache:
    mfspr   r8, HID0
    ori     r8, r8, 0x8
    mtspr   HID0, r8
    isync
    sync
    nop
    b       _wait
_skip1:
    b       _skip2
_wait:
    mftb    r5, 268
_waitloop:
    mftb    r6, 268
    subf    r7, r5, r6
    cmplwi  r7, 0x1124
    blt     _waitloop
    nop
    b       _reset
_skip2:
    b       _skip3
_reset:
    lis     r8, 0xCC00
    ori     r8, r8, 0x3000
    li      r4, 0x3
    stw     r4, 0x24(r8)
    stw     r3, 0x24(r8)
    nop
    b       _hang
_skip3:
    b       _hang2
_hang:
    nop
    b       _hang
_hang2:
    b       _cache
}
#pragma pop

static void CancelThreads(void) {
    OSThread* thread;
    OSThread* next;

    for (thread = ((OSThreadQueue*)0x800000DC)->head; thread != NULL; thread = next) {
        next = thread->linkActive.next;
        switch (thread->state) {
        case 1:
        case 4:
            OSCancelThread(thread);
            break;
        default:
            break;
        }
    }
}

void __OSDoHotReset(u32 resetCode) {
    OSDisableInterrupts();
    __VIRegs[1] = 0;
    ICFlashInvalidate();
    Reset(resetCode * 8);
}

void OSResetSystem(u32 reset, u32 resetCode, BOOL forceMenu) {
    int rc;
    BOOL enabled;
    BOOL padcal;

    OSDisableScheduler();
    __OSStopAudioSystem();

    if (reset == 2) {
        padcal = __PADDisableRecalibration(TRUE);
    }

    do {} while (CallResetFunctions(0) == 0);

    if (reset == 1 && forceMenu != 0) {
        void* sram;
        sram = __OSLockSram();
        *(u8*)((u8*)sram + 0x13) |= 0x40;
        __OSUnlockSram(TRUE);
        do {} while (__OSSyncSram() == 0);
    }

    enabled = OSDisableInterrupts();
    rc = CallResetFunctions(1);
    LCDisable();

    if (reset == 1) {
        enabled = OSDisableInterrupts();
        __VIRegs[1] = 0;
        ICFlashInvalidate();
        Reset(resetCode * 8);
    } else if (reset == 0) {
        CancelThreads();
        OSEnableScheduler();
        __OSReboot(resetCode, forceMenu);
    }

    CancelThreads();

    memset((void*)0x80000040, 0, 0x8C);
    memset((void*)0x800000D4, 0, 0x14);
    memset((void*)0x800000F4, 0, 4);
    memset((void*)0x80003000, 0, 0xC0);
    memset((void*)0x800030C8, 0, 0xC);

    if (reset == 2) {
        __PADDisableRecalibration(padcal);
    }
}

u32 OSGetResetCode(void) {
    if (*(volatile u8*)0x800030E2 != 0) {
        return 0x80000000;
    }
    return (__PIRegs[9] & ~7) >> 3;
}

void __OSResetSWInterruptHandler(s16 interrupt, OSContext* context) {
    u32 piIntSr;

    piIntSr = __PIRegs[0];

    if (!(piIntSr & 0x00000010)) {
        return;
    }

    __PIRegs[0] = piIntSr;

    {
        OSErrorHandler handler;
        handler = ((OSErrorHandler*)0x80003040)[OS_ERROR_SYSTEM_INTERRUPT];
        if (handler != NULL) {
            handler(OS_ERROR_SYSTEM_INTERRUPT, context);
            return;
        }
    }

    OSResetSystem(0, 0, FALSE);
}

/* ===================================================================
 * Stub functions for coverage -- TODO: decompile
 * 2 function(s)
 * =================================================================== */

/* fn_8009FEBC - 0x8009FEBC | size: 0x94 */
void fn_8009FEBC(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = 0x0;
    r29 = r3 + 0x0;
    r31 = *(u32*)ResetFunctionQueue_8047A738;
    goto L_8009FF04;
L_8009FEE4:
    r12 = *(u32*)((u8*)r31 + 0x0);
    r3 = r29 + 0x0;
    /* blrl  */;
    tmp = __cntlzw(r3);
    r31 = *(u32*)((u8*)r31 + 0x8);
    tmp = (u32)tmp >> 5;
    r30 = r30 | tmp;
L_8009FF04:
    if (r31 == 0) goto L_8009FF14;
    if ((s32)r30 == 0) goto L_8009FEE4;
L_8009FF14:
    __OSSyncSram();
    tmp = __cntlzw(r3);
    tmp = (u32)tmp >> 5;
    /* or. r30, r30, tmp */;
    if ((s32)r30 == 0) goto L_8009FF30;
    r3 = 0x0;
    goto L_8009FF34;
L_8009FF30:
    r3 = 0x1;
L_8009FF34:
    return;
}

/* fn_8009FFC0 - 0x8009FFC0 | size: 0x48 */
void fn_8009FFC0(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    OSDisableInterrupts();
    r3 = 0xCC000000;
    r3 = r3 + 0x2000;
    tmp = 0x0;
    *(u16*)((u8*)r3 + 0x2) = tmp;
    ICFlashInvalidate();
    r3 = r31 << 3;
    Reset(r3);
    return;
}

