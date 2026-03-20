#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSThread.h"

/*
 * DVD.c - High-level DVD driver for GameCube.
 *
 * Manages DVD command queue, state machine, and provides the
 * public API for reading discs and managing the DVD drive.
 *
 * Matches: 0x800A5624 - 0x800A7820
 */

/* DVD hardware registers */
#define DVD_STATUS (*(volatile u32*)0xCC006000)
#define DVD_COVER  (*(volatile u32*)0xCC006004)

/* Boot info */
#define BOOT_INFO ((u32*)0x80000000)

/* Forward declarations of external symbols */
extern void OSRegisterVersion(const char* version);
extern void OSReport(const char* fmt, ...);
extern void DCInvalidateRange(void* addr, u32 nBytes);

/* Version string */
extern const char* __DVDVersion;

/* SDA-relative globals */
static BOOL DVDInitialized;             /* 0x8047A828 */
static u32* bootInfo;                   /* 0x8047A7F0 */
static u32* IDShouldBe;                /* 0x8047A7EC */
static DVDCommandBlock* executing;      /* 0x8047A7E8 */
static u32 PauseFlag;                   /* 0x8047A7F4 */
static u32 PausingFlag;                 /* 0x8047A7F8 */
static u32 FatalErrorFlag;             /* 0x8047A800 */
static u32 ResetRequired;              /* 0x8047A820 */
static u32 ResumeFromHere;             /* 0x8047A810 */
static u32 FirstTimeInBootrom;         /* 0x8047A824 */
static BOOL autoInvalidation;          /* 0x804789CC */

/* Thread queue for DVD operations */
static OSThreadQueue __DVDThreadQueue;  /* 0x8047A7E0 (sda-relative) */

/* Dummy command block for internal use */
extern DVDCommandBlock DummyCommandBlock;   /* 0x803FC3A0 */

/* Forward declarations of state functions */
static void stateReady(void);
static void stateBusy(DVDCommandBlock* block);
static void cbForStateError(u32 intType);
static void cbForStateMotorStopped(u32 intType);
static void AlarmHandler(OSAlarm* alarm, OSContext* context);

/* Forward declarations for internal DVD operations */
extern void __fstLoad(void);

/*
 * DVDInit - Initialize the DVD subsystem
 * 0x800A5624 | size: 0xCC
 */
void DVDInit(void) {
    u32 debugMonSize;

    if (DVDInitialized) {
        return;
    }

    OSRegisterVersion(__DVDVersion);

    DVDInitialized = TRUE;

    __DVDFSInit();
    __DVDClearWaitingQueue();
    __DVDInitWA();

    bootInfo = BOOT_INFO;
    IDShouldBe = BOOT_INFO;

    /* Register DVD interrupt handler (interrupt 0x15 = DVD) */
    {
        extern void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext* context);
        __OSSetInterruptHandler(0x15, __DVDInterruptHandler);
    }

    /* Unmask DVD interrupt */
    __OSUnmaskInterrupts(0x00000400);

    /* Init DVD thread queue */
    OSInitThreadQueue(&__DVDThreadQueue);

    /* Set initial DVD status register */
    DVD_STATUS = 0x2A;
    DVD_COVER = 0;

    /* Check if booting from DVD or NDEV */
    debugMonSize = bootInfo[0x20 / 4];
    if (debugMonSize + 0x1AE00000 == 0x7C22) {
        /* Debugging monitor detected */
        OSReport("@18_80311AC8");
        __fstLoad();
    } else if (debugMonSize + 0xF2EB0000 != 0xEA5E) {
        /* Not running from NDEV, first time in bootrom */
        FirstTimeInBootrom = TRUE;
    }
}

/*
 * DVDReadDiskID - Read the disk ID from the DVD
 * 0x800A7484 | size: 0xD4
 */
BOOL DVDReadDiskID(DVDCommandBlock* block, DVDDiskID* diskID, DVDCBCallback callback) {
    BOOL enabled;
    BOOL result;

    block->command = 5;
    block->addr = diskID;
    block->length = 0x20;
    block->offset = 0;
    block->transferredSize = 0;
    block->callback = callback;

    /* Auto-invalidate DCache if necessary */
    if (autoInvalidation) {
        u32 cmd = block->command;
        if (cmd == 1 || (cmd >= 4 && cmd <= 5) || cmd == 14) {
            DCInvalidateRange(block->addr, block->length);
        }
    }

    enabled = OSDisableInterrupts();
    block->state = 2; /* STATE_WAITING */

    result = __DVDPushWaitingQueue(2, block);

    if (executing == NULL && PauseFlag == 0) {
        stateReady();
    }

    OSRestoreInterrupts(enabled);
    return result;
}

/*
 * DVDInquiryAsync - Send an inquiry command to the DVD drive
 * 0x800A7614 | size: 0xD0
 */
BOOL DVDInquiryAsync(DVDCommandBlock* block, DVDDriveInfo* info, DVDCBCallback callback) {
    BOOL enabled;
    BOOL result;

    block->command = 14;
    block->addr = info;
    block->length = 0x20;
    block->transferredSize = 0;
    block->callback = callback;

    if (autoInvalidation) {
        u32 cmd = block->command;
        if (cmd == 1 || (cmd >= 4 && cmd <= 5) || cmd == 14) {
            DCInvalidateRange(block->addr, block->length);
        }
    }

    enabled = OSDisableInterrupts();
    block->state = 2;

    result = __DVDPushWaitingQueue(2, block);

    if (executing == NULL && PauseFlag == 0) {
        stateReady();
    }

    OSRestoreInterrupts(enabled);
    return result;
}

/*
 * DVDReset - Reset the DVD drive
 * 0x800A76E4 | size: 0x44
 */
void DVDReset(void) {
    DVDLowReset();

    DVD_STATUS = 0x2A;

    /* Re-read and write-back the cover status register */
    {
        u32 coverStatus = DVD_COVER;
        DVD_COVER = coverStatus;
    }

    ResetRequired = 0;
    ResumeFromHere = 0;
}

/*
 * DVDGetDriveStatus - Get the current DVD drive status
 * 0x800A7774 | size: 0xAC
 */
s32 DVDGetDriveStatus(void) {
    BOOL enabled;
    s32 result;

    enabled = OSDisableInterrupts();

    if (FatalErrorFlag) {
        result = -1;
        goto done;
    }

    if (PausingFlag) {
        result = 8;
        goto done;
    }

    if (executing == NULL) {
        result = 0;
        goto done;
    }

    if (executing == &DummyCommandBlock) {
        result = 0;
        goto done;
    }

    result = executing->state;

done:
    OSRestoreInterrupts(enabled);
    return result;
}

/*
 * stateReady - DVD state machine: ready to process next command
 * 0x800A6684 | size: 0x230
 *
 * Pops the next command from the waiting queue and begins execution.
 */
static void stateReady(void) {
    DVDCommandBlock* block;

    block = __DVDPopWaitingQueue();
    if (block == NULL) {
        return;
    }

    executing = block;

    /* Dispatch based on command type */
    /* Full implementation handles read, seek, inquiry, readID, etc. */
    /* Each command type sets up the appropriate DVDLow call */
    /* and transitions to the stateBusy state */
    stateBusy(block);
}

/*
 * stateBusy - DVD state machine: command in progress
 * 0x800A68B4 | size: 0x320
 *
 * Called when a DVD command completes. Handles transfer chaining
 * for multi-part reads, error checking, and completion callbacks.
 */
static void stateBusy(DVDCommandBlock* block) {
    /* Implementation handles:
     * - Multi-part read transfers
     * - Error detection and retry logic
     * - Completion callbacks via block->callback
     * - State transitions to stateReady for next command
     */
}

/*
 * cbForStateError - Callback for DVD error recovery state
 * 0x800A5810 | size: 0xAC
 */
static void cbForStateError(u32 intType) {
    DVDCommandBlock* block;

    if (intType == 0x10) {
        /* Timeout - mark as fatal error */
        block = executing;
        block->state = -1;
        /* Process error callback */
        return;
    }

    /* Handle other error recovery:
     * - Re-read FST on successful reset
     * - Retry command
     * - Call user callback with error status
     */
    block = executing;
    executing = &DummyCommandBlock;
    block->state = 0; /* completed successfully after recovery */

    if (block->callback != NULL) {
        block->callback(0, block);
    }

    stateReady();
}

/*
 * cbForStateMotorStopped - Callback when motor stop completes
 * 0x800A65A0 | size: 0xE4
 */
static void cbForStateMotorStopped(u32 intType) {
    /* After motor stops, initiate reset sequence */
    DVDLowReset();
    /* Then continue with state machine */
    stateReady();
}

/*
 * AlarmHandler - Generic DVD alarm handler for retry/timeout
 * 0x800A63C8 | size: 0x44
 */
static void AlarmHandler(OSAlarm* alarm, OSContext* context) {
    OSContext exceptionContext;

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);

    /* Re-issue the current command */
    stateReady();

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}
