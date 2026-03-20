#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSTime.h"

/*
 * DVDLow.c - Low-level DVD drive interface.
 *
 * Handles direct communication with the DVD controller hardware
 * at 0xCC006000. Implements workaround logic and timeout handling.
 *
 * Matches: 0x800A3EB0 - 0x800A4CF0
 */

/* DVD hardware registers at 0xCC006000 */
#define DVD_REG_BASE    ((volatile u32*)0xCC006000)
#define DVD_STATUS      (DVD_REG_BASE[0])
#define DVD_COVER       (DVD_REG_BASE[1])
#define DVD_CMD         (DVD_REG_BASE[2])
#define DVD_OFFSET_LO   (DVD_REG_BASE[3])
#define DVD_LENGTH      (DVD_REG_BASE[4])
#define DVD_DMA_ADDR    (DVD_REG_BASE[5])
#define DVD_DMA_LEN     (DVD_REG_BASE[6])
#define DVD_CONTROL     (DVD_REG_BASE[7])

/* DI reset register at 0xCC003024 */
#define DI_RESET_REG    (*(volatile u32*)0xCC003024)

/* Boot info at 0x80000000 */
#define BUS_CLOCK       (*(u32*)0x800000F8)

/* Workaround command list structure */
typedef struct {
    u32 type;       /* 0x00: command type (1=read, 2=seek, -1=none) */
    u32 cmd;        /* 0x04 */
    u32 addr;       /* 0x08 */
    u32 offset;     /* 0x0C */
    u32 callback;   /* 0x10 */
} WACommand;

/* Command list - located at 0x803FC290 */
extern WACommand CommandList[];

/* Alarm for timeout - 0x803FC2F8 */
extern OSAlarm AlarmForTimeout;

/* SDA-relative globals */
static u32 StopAtNextInt;           /* 0x8047A780 */
static u32 WorkAroundSeekLocation;  /* 0x8047A7A8 */
static DVDCBCallback Callback;      /* 0x8047A788 */
static u32 WaitingCoverClose;       /* 0x8047A79C */
static u32 ResetOccurred;           /* 0x8047A798 */
static u32 NextCommandNumber;       /* 0x8047A7C4 */
static u32 WorkAroundType;          /* 0x8047A7A4 */
static s64 LastResetEnd;            /* 0x8047A790 */

/* Forward declarations */
static void AlarmHandlerForTimeout(OSAlarm* alarm, OSContext* context);
extern void OSInitAlarm(void);
extern s64 __OSGetSystemTime(void);

/*
 * __DVDInitWA - Initialize workaround system
 * 0x800A3EB0 | size: 0x40
 */
void __DVDInitWA(void) {
    NextCommandNumber = 0;
    CommandList[0].type = (u32)-1;
    __DVDLowSetWAType(0, 0);
    OSInitAlarm();
}

/*
 * AlarmHandlerForTimeout - Called when a DVD command times out
 * 0x800A4254 | size: 0x70
 */
static void AlarmHandlerForTimeout(OSAlarm* alarm, OSContext* context) {
    OSContext exceptionContext;
    DVDCBCallback cb;

    __OSMaskInterrupts(0x00000400);

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);

    cb = Callback;
    Callback = NULL;
    if (cb != NULL) {
        cb(0x10, NULL);
    }

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

/*
 * DVDLowWaitCoverClose - Wait for the DVD cover to be closed
 * 0x800A4780 | size: 0x2C
 */
BOOL DVDLowWaitCoverClose(DVDCBCallback callback) {
    Callback = callback;
    WaitingCoverClose = 1;
    StopAtNextInt = 0;

    /* Enable cover interrupt */
    DVD_COVER = 0x2;

    return TRUE;
}

/*
 * DVDLowStopMotor - Stop the DVD drive motor
 * 0x800A4850 | size: 0x8C
 */
BOOL DVDLowStopMotor(DVDCBCallback callback) {
    u32 timeout;

    Callback = callback;
    StopAtNextInt = 0;

    /* Send stop motor command */
    DVD_CMD = 0xE3000000;

    /* Start command */
    DVD_CONTROL = 0x1;

    /* Set up timeout alarm */
    timeout = (BUS_CLOCK / 4) * 10;
    OSCreateAlarm(&AlarmForTimeout);
    OSSetAlarm(&AlarmForTimeout, (s64)timeout, AlarmHandlerForTimeout);

    return TRUE;
}

/*
 * DVDLowReset - Reset the DVD drive
 * 0x800A4BC4 | size: 0xBC
 */
void DVDLowReset(void) {
    u32 resetReg;
    s64 startTime;
    u32 busSpeedQuarter;
    u32 waitTicks;

    /* Assert reset */
    DVD_COVER = 0x2;

    resetReg = DI_RESET_REG;
    DI_RESET_REG = (resetReg & ~0x4) | 0x1;

    /* Calculate wait time: ~12 ticks at bus clock / 4 / (1/2^15) */
    busSpeedQuarter = BUS_CLOCK / 4;
    waitTicks = (u32)((((u64)0x431CDE83ULL * busSpeedQuarter) >> 47) * 12) >> 3;

    startTime = __OSGetSystemTime();

    /* Busy-wait for the required time */
    while (1) {
        s64 elapsed = __OSGetSystemTime() - startTime;
        if (elapsed >= (s64)waitTicks) {
            break;
        }
    }

    /* Deassert reset */
    DI_RESET_REG = resetReg | 0x5;

    /* Mark reset occurred */
    ResetOccurred = 1;
    LastResetEnd = __OSGetSystemTime();
}

/*
 * __DVDLowSetWAType - Set the workaround type for the DVD drive
 * 0x800A4CAC | size: 0x44
 */
void __DVDLowSetWAType(u32 type, u32 location) {
    BOOL enabled = OSDisableInterrupts();
    WorkAroundType = type;
    WorkAroundSeekLocation = location;
    OSRestoreInterrupts(enabled);
}
