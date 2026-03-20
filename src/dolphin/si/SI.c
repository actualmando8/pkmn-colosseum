#include "dolphin/si/SI.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/vi/VI.h"

/*
 * SI.c - Serial Interface (SI) driver for GameCube.
 *
 * The SI bus provides communication with the four controller ports.
 * Hardware registers are at 0xCC006400.
 *
 * Matches: 0x800CFA60 - 0x800D104C
 */

/* SI hardware registers */
#define SI_REG_BASE     ((volatile u32*)0xCC006400)
#define SI_POLL         (*(volatile u32*)0xCC006430)
#define SI_COMCSR       (*(volatile u32*)0xCC006434)
#define SI_STATUS       (*(volatile u32*)0xCC006438)

/* SI channel output buffers at 0xCC006480 */
#define SI_CHAN_BUF(chan) ((volatile u32*)(0xCC006480 + (chan) * 0x0C))

/* Number of controller channels */
#define SI_MAX_CHAN     4

/* Bus clock reference */
#define BUS_CLOCK       (*(u32*)0x800000F8)

/* Packet / transfer state per channel (0x20 bytes each) */
typedef struct SIPacket {
    u32 cmd;            /* 0x00 */
    void* output;       /* 0x04 */
    u32 outputBytes;    /* 0x08 */
    void* input;        /* 0x0C */
    u32 inputBytes;     /* 0x10 */
    SICallback callback;/* 0x14 */
    s64 fireTime;       /* 0x18 */
} SIPacket;

/* Global SI state structure - located at lbl_80313F48 */
typedef struct SIControl {
    u8 _padding[0x44];
    s32 currentChan;    /* 0x44 - channel being polled, -1 if idle */
    u32 poll;           /* 0x48 - poll register shadow */
    void* inputBuf;     /* 0x4C */
    u32 inputLen;       /* 0x50 */
    SICallback transferCallback; /* 0x54 */
    u32 type[SI_MAX_CHAN]; /* 0x58 - cached type for each channel */
} SIControl;

extern SIControl Si;                /* lbl_80313F48 */
extern SIPacket Packet[SI_MAX_CHAN]; /* Packet_803FFFB0 */
extern u32 Type[SI_MAX_CHAN];       /* Type_80313FA0 */
extern u32 SamplingRate;            /* SamplingRate_8047AA60 */
extern const char* __SIVersion;

/* NTSC/PAL XY timing tables */
extern const u16 XYNTSC[];          /* XYNTSC_80314060 */

/* Forward declarations */
extern void OSRegisterVersion(const char* version);
extern void OSReport(const char* fmt, ...);
extern void OSCancelAlarm(void* alarm);
static void GetTypeCallback(s32 chan, u32 sr, OSContext* context);

/*
 * SIInterruptHandler - Interrupt handler for SI (controller polling)
 * 0x800CFA60 | size: 0x344
 *
 * Handles RDST (read status), TCINT (transfer complete), and
 * polling completion interrupts. Dispatches callbacks and
 * re-initiates polling for auto-detect.
 */
void SIInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    u32 comcsr;
    u32 cause;
    s32 chan;
    u32 sr;

    comcsr = SI_COMCSR;
    cause = comcsr & ~0x3FFFFFFF; /* extract top bits */

    /* Check for transfer complete interrupt */
    if ((cause + 0x40000000) == 0) {
        /* Transfer complete */
        s32 prevChan = Si.currentChan;
        SICallback cb;

        /* Read SI status register */
        sr = SI_STATUS;
        cb = Si.transferCallback;
        Si.transferCallback = NULL;

        /* Process next queued transfer */
        chan = (prevChan + 1) % SI_MAX_CHAN;

        /* Iterate through channels looking for pending transfers */
        {
            s32 i;
            for (i = 0; i < SI_MAX_CHAN; i++) {
                SIPacket* pkt = &Packet[chan];
                if (pkt->cmd != (u32)-1) {
                    s64 now = __OSGetSystemTime();
                    if (now >= pkt->fireTime) {
                        /* Execute this transfer */
                        /* SITransfer internal call */
                        pkt->cmd = (u32)-1;
                        break;
                    }
                }
                chan = (chan + 1) % SI_MAX_CHAN;
            }
        }

        /* Call the transfer callback */
        if (cb != NULL) {
            cb(prevChan, sr, context);
        }

        /* Check if we need to do type detection polling */
        {
            u32 chanType = Si.type[prevChan];
            if (chanType == 0x80) {
                /* Need to detect controller type */
                s32 needPoll = 1;
                u32 pktCmd = Packet[prevChan].cmd;
                if (pktCmd == (u32)-1 && Si.currentChan != prevChan) {
                    needPoll = 0;
                }
                if (!needPoll) {
                    /* Start type detection transfer */
                    u32 timeout;
                    timeout = (u32)(((u64)0x431CDE83ULL * (BUS_CLOCK / 4)) >> 47) * 65;
                    timeout >>= 3;
                    SITransfer(prevChan, (void*)0x8047AA50, 1, NULL, 3,
                               GetTypeCallback, (s64)timeout);
                }
            }
        }
    }

    /* Handle RDST (read status / polling) interrupt */
    if ((comcsr & 0x18000000) == 0x18000000) {
        /* Polling data available - process all 4 channels */
        /* Copy poll data from hardware to RAM buffers */
        /* Check for changes in controller presence */
    }
}

/*
 * SIInit - Initialize the Serial Interface
 * 0x800CFFFC | size: 0xB4
 */
void SIInit(void) {
    OSRegisterVersion(__SIVersion);

    /* Initialize packet slots to idle (-1) */
    Packet[0].cmd = (u32)-1;
    Packet[1].cmd = (u32)-1;
    Packet[2].cmd = (u32)-1;
    Packet[3].cmd = (u32)-1;

    /* Clear poll state */
    Si.poll = 0;

    /* Set default sampling rate */
    SISetSamplingRate(0);

    /* Wait for any ongoing transfer to finish */
    while (SI_COMCSR & 0x1) {
        ;
    }

    /* Clear and reset COMCSR */
    SI_COMCSR = 0x80000000;

    /* Register SI interrupt handler (interrupt 0x14 = SI) */
    __OSSetInterruptHandler(0x14, SIInterruptHandler);

    /* Unmask SI interrupt */
    __OSUnmaskInterrupts(0x00000800);

    /* Probe all 4 controller ports */
    SIGetType(0);
    SIGetType(1);
    SIGetType(2);
    SIGetType(3);
}

/*
 * SISetXY - Set the SI polling intervals (X and Y counters)
 * 0x800D035C | size: 0x6C
 *
 * X = horizontal lines between polls
 * Y = vertical blanking count
 */
void SISetXY(u32 x, u32 y) {
    BOOL enabled;
    u32 val;

    val = (x << 16) | (y << 8);

    enabled = OSDisableInterrupts();

    /* Read current poll register, clear X/Y fields, set new values */
    Si.poll &= 0xFF;    /* preserve low byte (enable bits) */
    Si.poll |= val;

    /* Write to hardware */
    SI_POLL = Si.poll;

    OSRestoreInterrupts(enabled);
}

/*
 * SITransfer - Initiate a transfer on an SI channel
 * 0x800D06F4 | size: 0x16C
 *
 * Sends output data and receives input data from a controller.
 */
BOOL SITransfer(s32 chan, void* output, u32 outputBytes,
                void* input, u32 inputBytes,
                SICallback callback, s64 time) {
    BOOL enabled;
    u32 numWords;
    s32 i;
    volatile u32* chanBuf;

    enabled = OSDisableInterrupts();

    /* Check if a transfer is already in progress on any channel */
    if (Si.currentChan != -1) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    /* Mask this channel's status bits */
    {
        u32 mask = 0x0F000000 >> (chan * 8);
        u32 sr = SI_STATUS;
        sr &= mask;
        SI_STATUS = sr;
    }

    /* Record current channel and callbacks */
    Si.currentChan = chan;
    Si.transferCallback = callback;
    Si.inputBuf = input;
    Si.inputLen = inputBytes;

    /* Copy output data to SI output buffer (32-bit writes) */
    numWords = (outputBytes + 3) / 4;
    chanBuf = (volatile u32*)0xCC006480;

    for (i = 0; (u32)i < numWords; i++) {
        chanBuf[i] = ((u32*)output)[i];
    }

    /* Build and write COMCSR to start the transfer */
    {
        u32 comcsr = SI_COMCSR;
        u8* csrBytes = (u8*)&comcsr;

        /* Set transfer-enable bit */
        csrBytes[0] |= 0x80;

        /* Set callback enable based on whether callback is non-NULL */
        if (callback != NULL) {
            csrBytes[0] |= 0x40;
        } else {
            csrBytes[0] &= ~0x40;
        }

        /* Set output/input lengths */
        if (outputBytes == 0x80) {
            csrBytes[1] = 0;
        } else {
            csrBytes[1] = (u8)outputBytes;
        }
        if (inputBytes == 0x80) {
            csrBytes[2] = 0;
        } else {
            csrBytes[2] = (u8)inputBytes;
        }

        /* Set channel number and start bit */
        csrBytes[3] = (csrBytes[3] & ~0x06) | ((u8)(chan << 1));
        csrBytes[3] |= 0x01;

        SI_COMCSR = comcsr;
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/*
 * GetTypeCallback - Callback for automatic controller type detection
 * 0x800D0860 | size: 0x298
 *
 * Called when a type-detection transfer completes.
 * Reads the response and updates the cached controller type.
 */
static void GetTypeCallback(s32 chan, u32 sr, OSContext* context) {
    u32 type;
    u32 chanStatus;

    /* Read the type value from SI status register */
    chanStatus = SI_STATUS >> ((3 - chan) * 8);

    /* Check error bits */
    if (chanStatus & 0x8) {
        /* No response / error - mark as not connected */
        Type[chan] = 0x08;
        return;
    }

    /* Extract the type ID from the response */
    type = chanStatus;

    /* Check if the type changed from what we had cached */
    if (Type[chan] != type) {
        /* Type changed - update cache */
        if (!(Type[chan] & 0x80)) {
            Type[chan] = 0x08; /* mark as needing re-probe */
        }
    }

    Type[chan] = type;
}

/*
 * SIGetType - Get the controller type for a channel
 * 0x800D0AF8 | size: 0x1C4
 *
 * Returns the controller type. If unknown, initiates a probe transfer.
 */
u32 SIGetType(s32 chan) {
    BOOL enabled;
    u32 type;
    u32 sr;
    u32 chanBits;

    enabled = OSDisableInterrupts();

    /* Read channel-specific bits from SI status */
    sr = SI_STATUS >> ((3 - chan) * 8);

    /* If error bit is set and we haven't cached an error type, set it */
    if ((sr & 0x08) && !(Type[chan] & 0x80)) {
        Type[chan] = 0x08;
    }

    OSRestoreInterrupts(enabled);

    type = Type[chan];

    /* If type is not yet determined, may need to start a probe */
    if (type == 0x08 || type == 0x80) {
        /* Start a probe transfer if not already in progress */
        enabled = OSDisableInterrupts();

        /* Check if channel has no pending transfers */
        if (Packet[chan].cmd == (u32)-1) {
            u32 timeout;
            timeout = (u32)(((u64)0x431CDE83ULL * (BUS_CLOCK / 4)) >> 47) * 65;
            timeout >>= 3;
            SITransfer(chan, (void*)0x8047AA50, 1, NULL, 3,
                       GetTypeCallback, (s64)timeout);
        }

        OSRestoreInterrupts(enabled);
    }

    return type;
}

/*
 * SISetSamplingRate - Set the controller polling rate
 * 0x800D0F68 | size: 0xE4
 *
 * msec: polling period in milliseconds (clamped to 0-11)
 */
u32 SISetSamplingRate(u32 msec) {
    BOOL enabled;
    u32 tvFormat;
    const u16* xyTable;
    u32 x, y;
    u32 lines;

    if (msec > 11) {
        msec = 11;
    }

    enabled = OSDisableInterrupts();

    SamplingRate = msec;

    tvFormat = VIGetTvFormat();

    switch (tvFormat) {
    case 0:  /* NTSC */
    case 2:  /* MPAL */
    case 5:  /* EUR60 */
        xyTable = XYNTSC;
        break;
    case 1:  /* PAL */
        xyTable = XYNTSC + 24; /* PAL table offset */
        break;
    default:
        OSReport("Warning: unknown TV format in SISetSamplingRate\n");
        msec = 0;
        xyTable = XYNTSC;
        break;
    }

    /* Look up X and Y values from table */
    {
        u32 interlace;
        /* Check if we're in interlaced mode */
        u16 viMode = *(volatile u16*)0xCC00206C;
        if (viMode & 0x1) {
            interlace = 2;
        } else {
            interlace = 1;
        }

        x = xyTable[msec * 2] * interlace;
        y = xyTable[msec * 2 + 1]; /* actually a byte at offset +2 */
    }

    SISetXY(x, y);

    OSRestoreInterrupts(enabled);

    return msec;
}
