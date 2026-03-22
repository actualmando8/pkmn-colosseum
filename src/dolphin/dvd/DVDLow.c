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

/* SDA symbol aliases used by stub functions */
extern u32 CommandList_803FC290;
extern u32 NextCommandNumber_8047A7C4;
extern u32 StopAtNextInt_8047A780;
extern u32 Callback_8047A788;
extern u32 WorkAroundSeekLocation_8047A7A8;
extern u32 WorkAroundType_8047A7A4;
extern u32 AlarmForTimeout_803FC2F8;

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

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800A41D0 - 0x800A41D0 | size: 0x84
 * DVDLowProcessNextCommand - Execute the next command in the command list.
 * Command type 1 = read (fn_800A42C4), type 2 = seek (fn_800A46EC).
 * Advances NextCommandNumber after processing.
 */
void fn_800A41D0(void) {
    extern void fn_800A42C4(u32 cmd, u32 addr, u32 len, u32 offset);
    extern void fn_800A46EC(u32 addr, u32 offset);
    u8* cmdList = (u8*)(u32)CommandList_803FC290;
    u32 idx;
    u32 offset;
    u8* cmd;
    s32 cmdType;

    idx = *(u32*)&NextCommandNumber_8047A7C4;
    offset = idx * 0x14;
    cmd = cmdList + offset;
    cmdType = *(s32*)(cmd);

    if (cmdType == 1) {
        *(u32*)&NextCommandNumber_8047A7C4 = idx + 1;
        fn_800A42C4(*(u32*)(cmd + 0x4), *(u32*)(cmd + 0x8),
                     *(u32*)(cmd + 0xC), *(u32*)(cmd + 0x10));
    } else if (cmdType == 2) {
        *(u32*)&NextCommandNumber_8047A7C4 = idx + 1;
        fn_800A46EC(*(u32*)(cmd + 0xC), *(u32*)(cmd + 0x10));
    }
}

/*
 * DVDLowRead - Issue a DVD read command and set up timeout alarm.
 *
 * Writes the DI read command (0xA8) to the DVD hardware registers,
 * sets up the DMA address and length, then creates a timeout alarm.
 * The timeout period is 20x or 10x the bus clock tick depending on
 * whether the DMA length exceeds 3.
 *
 * 0x800A42C4 | size: 0x110
 */
void fn_800A42C4(u32 cmd, u32 dmaAddr, u32 offset, u32 callback) {
    extern u32 lbl_8047A784;
    extern u32 lbl_8047A7B8;
    extern u32 lbl_8047A7BC;
    extern u32 lbl_8047A7C0;
    volatile u32* dvdRegs = (volatile u32*)0xCC006000;
    u8* cmdListBase = (u8*)(u32)CommandList_803FC290;
    u32 busTick;
    u32 timeout;

    *(u32*)&StopAtNextInt_8047A780 = 0;
    *(u32*)&Callback_8047A788 = callback;
    lbl_8047A7C0 = 1;

    /* Record start time */
    {
        s64 time = __OSGetSystemTime();
        lbl_8047A7B8 = (u32)(time >> 32);
        lbl_8047A7BC = (u32)time;
    }

    /* Write DVD hardware registers */
    dvdRegs[2] = 0xA8000000;              /* CMD: read */
    dvdRegs[3] = offset >> 2;             /* offset (in 4-byte units) */
    dvdRegs[4] = dmaAddr;                 /* DMA buffer address */
    dvdRegs[5] = cmd;                     /* DMA length */
    dvdRegs[6] = dmaAddr;                 /* DMA length (again) */
    lbl_8047A784 = dmaAddr;
    dvdRegs[7] = 3;                       /* control: start */

    /* Set up timeout alarm */
    busTick = BUS_CLOCK >> 2;
    if (dmaAddr > 3) {
        timeout = busTick * 20;
    } else {
        timeout = busTick * 10;
    }
    OSCreateAlarm((OSAlarm*)(cmdListBase + 0x68));
    OSSetAlarm((OSAlarm*)(cmdListBase + 0x68), (s64)timeout,
               (OSAlarmHandler)AlarmHandlerForTimeout);
}

/* fn_800A43D4 - 0x800A43D4 | size: 0x80
 * DVDLowSeekRead - Set up a seek command in the command list and execute it.
 * Computes the seek location based on address alignment.
 */
void fn_800A43D4(u32 cmd, u32 addr, u32 length, u32 offset) {
    extern void fn_800A46EC(u32 seekAddr, u32 offset);
    u8* cmdList = (u8*)(u32)CommandList_803FC290;
    u32 seekAddr;
    u32 alignedLen;

    /* If length is page-aligned (lower 15 bits clear), compute seek address */
    alignedLen = length & ~0x7FFF;
    if (alignedLen != 0) {
        seekAddr = alignedLen + *(u32*)&WorkAroundSeekLocation_8047A7A8;
    } else {
        seekAddr = 0;
    }

    /* Fill in the command entry */
    *(u32*)(cmdList + 0x0) = 2;      /* command type: seek */
    *(u32*)(cmdList + 0xC) = seekAddr;
    *(u32*)(cmdList + 0x10) = offset;
    *(u32*)(cmdList + 0x14) = 1;
    *(u32*)(cmdList + 0x18) = cmd;
    *(u32*)(cmdList + 0x1C) = addr;
    *(u32*)(cmdList + 0x20) = length;
    *(u32*)(cmdList + 0x24) = offset;
    *(u32*)(cmdList + 0x28) = (u32)-1;
    *(u32*)&NextCommandNumber_8047A7C4 = 0;

    fn_800A46EC(seekAddr, offset);
}

/* fn_800A4454 - 0x800A4454 | size: 0x298 */
void fn_800A4454(void) {
    extern u8 lbl_804789B8[];
    extern u8 lbl_8047A7B0[];
    extern u8 lbl_8047A7B4[];
    extern void fn_800A42C4();
    extern void fn_800A43D4();
    extern void fn_800A7BCC();
    extern void fn_800A41D0();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f4 = 0.0f;
    f32 f8 = 0.0f;

    r7 = 0xCC000000;
    r25 = r4 + 0x0;
    r4 = r7 + 0x6000;
    r7 = (u32)CommandList_803FC290;
    r31 = (u32)CommandList_803FC290;
    r24 = r3 + 0x0;
    r26 = r5 + 0x0;
    r30 = r31 + 0xcc;
    r27 = r6;
    *(u32*)((u8*)r4 + 0x18) = r25;
    *(u32*)((u8*)r31 + 0xC4) = r24;
    *(u32*)((u8*)r31 + 0xC8) = r25;
    *(u32*)((u8*)r31 + 0xCC) = r26;
    tmp = *(u32*)WorkAroundType_8047A7A4;
    if (tmp == 0) {
        tmp = -0x1;
        *(u32*)((u8*)r31 + 0x0) = tmp;
        tmp = 0x0;
        r3 = r24 + 0x0;
        *(u32*)NextCommandNumber_8047A7C4 = tmp;
        r4 = r25 + 0x0;
        r5 = r26 + 0x0;
        r6 = r27 + 0x0;
        fn_800A42C4();
        r3 = 0x1;
        return;
    }
    tmp = *(u32*)WorkAroundType_8047A7A4;
    if (tmp != 1) { r3 = 0x1; return; }
    tmp = *(u32*)lbl_804789B8;
    if ((s32)tmp != 0) {
        r3 = r24 + 0x0;
        r4 = r25 + 0x0;
        r5 = r26 + 0x0;
        r6 = r27 + 0x0;
        fn_800A43D4();
        r3 = 0x1;
        return;
    }
    r29 = r31 + 0xbc;
    tmp = *(u32*)((u8*)r30 + 0x0);
    r28 = r31 + 0xc0;
    r3 = *(u32*)((u8*)r31 + 0xBC);
    r4 = *(u32*)((u8*)r31 + 0xC0);
    r23 = (u32)tmp >> 15;
    tmp = r4 + tmp;
    r22 = (u32)tmp >> 15;
    fn_800A7BCC();
    tmp = *(u8*)((u8*)r3 + 0x8);
    if (tmp != 0) {
        tmp = 0x1;
    } else {

        tmp = 0x0;
    }
    if ((s32)tmp != 0) {
        r3 = 0x5;
    } else {

        r3 = 0xf;
    }
    if (r23 > tmp) goto L_800A456C;
    tmp = r3 + 0x3;
    tmp = r22 + tmp;
    if (r23 >= tmp) goto L_800A4574;
L_800A456C:
    tmp = 0x1;
    goto L_800A4578;
L_800A4574:
    tmp = 0x0;
L_800A4578:
    if ((s32)tmp == 0) {
        tmp = -0x1;
        *(u32*)((u8*)r31 + 0x0) = tmp;
        tmp = 0x0;
        r3 = r24 + 0x0;
        *(u32*)NextCommandNumber_8047A7C4 = tmp;
        r4 = r25 + 0x0;
        r5 = r26 + 0x0;
        r6 = r27 + 0x0;
        fn_800A42C4();
        r3 = 0x1;
        return;
    }
    r3 = *(u32*)((u8*)r29 + 0x0);
    r4 = *(u32*)((u8*)r28 + 0x0);
    tmp = *(u32*)((u8*)r30 + 0x0);
    r3 = r4 + r3;
    r3 = (u32)r3 >> 15;
    r4 = (u32)tmp >> 15;
    if (r3 == r4) goto L_800A45D8;
    tmp = r3 + 0x1;
    if (tmp != r4) goto L_800A46C0;
L_800A45D8:
    __OSGetSystemTime();
    r5 = 0x80000000;
    r8 = *(u32*)lbl_8047A7B0;
    tmp = *(u32*)((u8*)r5 + 0xF8);
    r5 = 0x10620000;
    r9 = *(u32*)lbl_8047A7B4;
    r6 = 0x0;
    r7 = (u32)tmp >> 2;
    tmp = r5 + 0x4dd3;
    tmp = (u32)((u64)tmp * (u64)r7 >> 32);
    tmp = (u32)tmp >> 6;
    r9 = r4 - r9;
    r8 = r3 - r8; /* -borrow */;
    r5 = tmp * 0x5;
    tmp = r5 - r9;
    r3 = r4 - r3; /* -borrow */;
    r3 = r4 - r4; /* -borrow */;
    /* neg. r3, r3 */;
    if (tmp != r4) {
        tmp = -0x1;
        *(u32*)((u8*)r31 + 0x0) = tmp;
        r3 = r24 + 0x0;
        r4 = r25 + 0x0;
        *(u32*)NextCommandNumber_8047A7C4 = r6;
        r5 = r26 + 0x0;
        r6 = r27 + 0x0;
        fn_800A42C4();
        r3 = 0x1;
        return;
    }
    tmp = 0x1;
    *(u32*)((u8*)r31 + 0x0) = tmp;
    r3 = 0x431C0000;
    *(u32*)((u8*)r31 + 0x4) = r24;
    tmp = (u32)((u64)tmp * (u64)r7 >> 32);
    *(u32*)((u8*)r31 + 0x8) = r25;
    *(u32*)((u8*)r31 + 0xC) = r26;
    tmp = (u32)tmp >> 15;
    r3 = tmp * 0x1f4;
    *(u32*)((u8*)r31 + 0x10) = r27;
    tmp = -0x1;
    *(u32*)((u8*)r31 + 0x14) = tmp;
    r5 = r5 - r9;
    r4 = r6 - r8; /* -borrow */;
    tmp = (u32)r3 >> 3;
    *(u32*)NextCommandNumber_8047A7C4 = r6;
    r23 = r5 + tmp;
    r22 = r4 + r6; /* +carry */;
    r3 = r31 + 0x40;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)fn_800A41D0;
    r7 = (u32)fn_800A41D0;
    r6 = r23 + 0x0;
    r5 = r22 + 0x0;
    r3 = r31 + 0x40;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
L_800A46C0:
    r3 = r24 + 0x0;
    r4 = r25 + 0x0;
    r5 = r26 + 0x0;
    r6 = r27 + 0x0;
    fn_800A43D4();

    r3 = 0x1;
    return;
}

/* fn_800A46EC - 0x800A46EC | size: 0x94 */
void fn_800A46EC(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    tmp = 0x0;
    *(u32*)Callback_8047A788 = r4;
    r4 = 0xCC000000;
    r4 = r4 + 0x6000;
    *(u32*)StopAtNextInt_8047A780 = tmp;
    tmp = 0xAB000000;
    *(u32*)((u8*)r4 + 0x8) = tmp;
    tmp = (u32)r3 >> 2;
    r3 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r4 + 0xC) = tmp;
    tmp = 0x1;
    r31 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r4 + 0x1C) = tmp;
    r4 = 0x80000000;
    r3 = r31 + 0x0;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    tmp = (u32)tmp >> 2;
    r30 = tmp * 0xa;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)AlarmHandlerForTimeout;
    r7 = (u32)AlarmHandlerForTimeout;
    r3 = r31 + 0x0;
    r6 = r30 + 0x0;
    r5 = 0x0;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
}

/* fn_800A47AC - 0x800A47AC | size: 0xA4 */
void fn_800A47AC(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r8 = 0x0;
    r5 = 0xA8000000;
    tmp = r5 + 0x40;
    r6 = 0x20;
    r5 = 0x80000000;
    *(u32*)Callback_8047A788 = r4;
    r4 = 0xCC000000;
    r7 = r4 + 0x6000;
    *(u32*)StopAtNextInt_8047A780 = r8;
    *(u32*)((u8*)r4 + 0x6008) = tmp;
    r4 = (u32)AlarmForTimeout_803FC2F8;
    tmp = 0x3;
    *(u32*)((u8*)r7 + 0xC) = r8;
    r31 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r7 + 0x10) = r6;
    *(u32*)((u8*)r7 + 0x14) = r3;
    r3 = r31;
    *(u32*)((u8*)r7 + 0x18) = r6;
    *(u32*)((u8*)r7 + 0x1C) = tmp;
    tmp = *(u32*)((u8*)r5 + 0xF8);
    tmp = (u32)tmp >> 2;
    r30 = tmp * 0xa;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)AlarmHandlerForTimeout;
    r7 = (u32)AlarmHandlerForTimeout;
    r3 = r31 + 0x0;
    r6 = r30 + 0x0;
    r5 = 0x0;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
}

/* fn_800A48DC - 0x800A48DC | size: 0x8C */
void fn_800A48DC(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    tmp = 0x0;
    *(u32*)Callback_8047A788 = r3;
    r3 = 0xCC000000;
    r4 = r3 + 0x6000;
    *(u32*)StopAtNextInt_8047A780 = tmp;
    tmp = 0xE0000000;
    *(u32*)((u8*)r3 + 0x6008) = tmp;
    tmp = 0x1;
    r3 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r4 + 0x1C) = tmp;
    r4 = 0x80000000;
    r31 = (u32)AlarmForTimeout_803FC2F8;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r31 + 0x0;
    tmp = (u32)tmp >> 2;
    r30 = tmp * 0xa;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)AlarmHandlerForTimeout;
    r7 = (u32)AlarmHandlerForTimeout;
    r3 = r31 + 0x0;
    r6 = r30 + 0x0;
    r5 = 0x0;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
}

/* fn_800A4968 - 0x800A4968 | size: 0x9C */
void fn_800A4968(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r6 = 0x20;
    tmp = 0x0;
    r5 = 0x80000000;
    *(u32*)Callback_8047A788 = r4;
    r4 = 0xCC000000;
    r7 = r4 + 0x6000;
    *(u32*)StopAtNextInt_8047A780 = tmp;
    tmp = 0x12000000;
    *(u32*)((u8*)r4 + 0x6008) = tmp;
    r4 = (u32)AlarmForTimeout_803FC2F8;
    tmp = 0x3;
    *(u32*)((u8*)r7 + 0x10) = r6;
    r31 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r7 + 0x14) = r3;
    r3 = r31 + 0x0;
    *(u32*)((u8*)r7 + 0x18) = r6;
    *(u32*)((u8*)r7 + 0x1C) = tmp;
    tmp = *(u32*)((u8*)r5 + 0xF8);
    tmp = (u32)tmp >> 2;
    r30 = tmp * 0xa;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)AlarmHandlerForTimeout;
    r7 = (u32)AlarmHandlerForTimeout;
    r3 = r31 + 0x0;
    r6 = r30 + 0x0;
    r5 = 0x0;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
}

/* fn_800A4A04 - 0x800A4A04 | size: 0x98 */
void fn_800A4A04(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    tmp = 0x0;
    *(u32*)Callback_8047A788 = r6;
    r6 = 0xCC000000;
    r6 = r6 + 0x6000;
    *(u32*)StopAtNextInt_8047A780 = tmp;
    tmp = r3 | (0xe100 << 16);
    r3 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r6 + 0x8) = tmp;
    tmp = (u32)r5 >> 2;
    r31 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r6 + 0xC) = tmp;
    tmp = 0x1;
    r3 = r31 + 0x0;
    *(u32*)((u8*)r6 + 0x10) = r4;
    r4 = 0x80000000;
    *(u32*)((u8*)r6 + 0x1C) = tmp;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    tmp = (u32)tmp >> 2;
    r30 = tmp * 0xa;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)AlarmHandlerForTimeout;
    r7 = (u32)AlarmHandlerForTimeout;
    r3 = r31 + 0x0;
    r6 = r30 + 0x0;
    r5 = 0x0;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
}

/* fn_800A4A9C - 0x800A4A9C | size: 0x8C */
void fn_800A4A9C(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    tmp = 0x0;
    *(u32*)Callback_8047A788 = r4;
    r4 = 0xCC000000;
    r4 = r4 + 0x6000;
    *(u32*)StopAtNextInt_8047A780 = tmp;
    tmp = r3 | (0xe200 << 16);
    r3 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r4 + 0x8) = tmp;
    tmp = 0x1;
    r31 = (u32)AlarmForTimeout_803FC2F8;
    *(u32*)((u8*)r4 + 0x1C) = tmp;
    r4 = 0x80000000;
    r3 = r31 + 0x0;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    tmp = (u32)tmp >> 2;
    r30 = tmp * 0xa;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)AlarmHandlerForTimeout;
    r7 = (u32)AlarmHandlerForTimeout;
    r3 = r31 + 0x0;
    r6 = r30 + 0x0;
    r5 = 0x0;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
}

/* fn_800A4B28 - 0x800A4B28 | size: 0x9C */
void fn_800A4B28(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    tmp = 0x0;
    *(u32*)Callback_8047A788 = r5;
    *(u32*)StopAtNextInt_8047A780 = tmp;
    if ((s32)r3 != 0) {
        tmp = 0x10000;
    }
    tmp = tmp | (0xe400 << 16);
    r3 = 0xCC000000;
    tmp = r4 | tmp;
    r3 = r3 + 0x6000;
    *(u32*)((u8*)r3 + 0x8) = tmp;
    tmp = 0x1;
    r4 = 0x80000000;
    *(u32*)((u8*)r3 + 0x1C) = tmp;
    r3 = (u32)AlarmForTimeout_803FC2F8;
    r31 = (u32)AlarmForTimeout_803FC2F8;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r31 + 0x0;
    tmp = (u32)tmp >> 2;
    r30 = tmp * 0xa;
    OSCreateAlarm((OSAlarm*)r3);
    r3 = (u32)AlarmHandlerForTimeout;
    r7 = (u32)AlarmHandlerForTimeout;
    r3 = r31 + 0x0;
    r6 = r30 + 0x0;
    r5 = 0x0;
    OSSetAlarm((OSAlarm*)r3, 0, (OSAlarmHandler)r7);
    r3 = 0x1;
    return;
}

/* fn_800A4C80 - 0x800A4C80 | size: 0x14 */
void fn_800A4C80(void) {
    extern u8 lbl_8047A7A0[];
    u32 tmp = 0;
    u32 r3 = 0;

    tmp = 0x1;
    *(u32*)StopAtNextInt_8047A780 = tmp;
    r3 = 0x1;
    *(u32*)lbl_8047A7A0 = tmp;
    return;
}

/* fn_800A4C94 - 0x800A4C94 | size: 0x18 */
void fn_800A4C94(void) {
    u32 tmp = 0;
    u32 r3 = 0;

    r3 = 0xCC000000;
    tmp = 0x0;
    *(u32*)((u8*)r3 + 0x6004) = tmp;
    r3 = *(u32*)Callback_8047A788;
    *(u32*)Callback_8047A788 = tmp;
    return;
}

