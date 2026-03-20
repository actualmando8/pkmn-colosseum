#include "dolphin/os/OSReset.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/PPCArch.h"

/*
 * OSReset.c - System reset and reboot functionality.
 *
 * Manages the reset function chain, system reset procedure, and the
 * software reset interrupt handler.
 *
 * Matches: 0x8009FAF8 - 0x800A03B4
 */

/* Hardware registers */
#define PI_RESET        (*(volatile u32*)0xCC003024)
#define PI_INTSR        (*(volatile u32*)0xCC003000)
#define PI_INTMR        (*(volatile u32*)0xCC003004)

/* Memory-mapped values */
#define OS_RESET_CODE   (*(volatile u32*)0x800030F0)
#define OS_BOOT_INFO    ((volatile u32*)0x80000000)

static OSResetFunctionInfo* ResetFunctionList;

extern void __OSReboot(u32 resetCode, u32 bootDol);
extern OSErrorHandler __OSErrorTable[];

void OSRegisterResetFunction(OSResetFunctionInfo* info) {
    OSResetFunctionInfo* iter;
    OSResetFunctionInfo* prev;

    /* Insert into the list sorted by priority (lower = higher priority) */
    prev = NULL;
    for (iter = ResetFunctionList; iter != NULL; iter = iter->next) {
        if (iter->priority > info->priority) {
            break;
        }
        prev = iter;
    }

    info->next = iter;
    if (prev != NULL) {
        prev->next = info;
    } else {
        ResetFunctionList = info;
    }
}

static void Reset(u32 resetCode) {
    /* Assert reset */
    PI_RESET = 0x00000001;

    /* Wait a bit by polling the time base */
    {
        s64 start;
        u32 count;

        start = OSGetTime();
        while ((u32)(OSGetTime() - start) < 0x0000000C) {
            ;
        }
    }

    /* Deassert reset */
    PI_RESET = 0x00000003;

    /* Wait for hardware to come back */
    {
        s64 start;
        start = OSGetTime();
        while ((u32)(OSGetTime() - start) < 0x00000054) {
            ;
        }
    }
}

void OSResetSystem(u32 reset, u32 resetCode, BOOL forceMenu) {
    BOOL enabled;
    OSResetFunctionInfo* iter;
    BOOL finalize;

    enabled = OSDisableInterrupts();

    /* Call each reset function with final = FALSE first */
    finalize = FALSE;

    /* Give registered reset functions a chance to prepare */
    for (iter = ResetFunctionList; iter != NULL; iter = iter->next) {
        if (!iter->func(FALSE)) {
            finalize = TRUE;
        }
    }

    /* If any function wanted more time, call again with final = TRUE */
    if (finalize) {
        for (iter = ResetFunctionList; iter != NULL; iter = iter->next) {
            iter->func(TRUE);
        }
    }

    /* Disable all interrupts at the hardware level */
    __OSMaskInterrupts(0xFFFFFFFF);

    /* Store the reset code */
    OS_RESET_CODE = resetCode;

    if (reset == 0) {
        /* Soft reset / hot reset */
        if (forceMenu) {
            /* Jump to the IPL (system menu) */
            __OSReboot(resetCode, 0);
        } else {
            /* Standard reset */
            Reset(resetCode);
        }
    } else if (reset == 1) {
        /* Shutdown */
        Reset(resetCode);
    } else if (reset == 2) {
        /* Restart */
        __OSReboot(resetCode, 0);
    }

    OSRestoreInterrupts(enabled);
}

u32 OSGetResetCode(void) {
    u32 code;

    code = *(volatile u32*)0x800030F0;

    if (code != 0) {
        return (code | 0x80000000);
    }

    /* Read from hardware */
    code = PI_RESET;
    return code & 0x7FFFFFFF;
}

void __OSResetSWInterruptHandler(s16 interrupt, OSContext* context) {
    u32 piIntSr;
    u32 resetCode;

    piIntSr = PI_INTSR;

    /* Check if it's actually a reset interrupt */
    if (!(piIntSr & 0x00000010)) {
        return;
    }

    /* Acknowledge the interrupt */
    PI_INTSR = piIntSr;

    /* Read the reset code from memory */
    resetCode = OS_RESET_CODE;

    {
        /* The error table's reset entry */
        OSErrorHandler handler;
        handler = __OSErrorTable[OS_ERROR_SYSTEM_INTERRUPT];
        if (handler != NULL) {
            handler(OS_ERROR_SYSTEM_INTERRUPT, context);
            return;
        }
    }

    OSResetSystem(0, resetCode, FALSE);
}
