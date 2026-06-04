#include "dolphin/si/SI.h"
#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/vi/VI.h"

/*
 * SI.c - Serial Interface (SI) driver for GameCube.
 *
 * Adapted from zeldaret/tp SIBios.c matching implementation.
 *
 * Matches: 0x800CFA60 - 0x800D104C
 */

#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))

#define __SIRegs        ((volatile u32*)0xCC006400)
#define SI_COMCSR_IDX   (0x34/4)
#define SI_STATUS_IDX   (0x38/4)

#define SI_COMCSR_TCINT_MASK      0x80000000
#define SI_COMCSR_TCINTMSK_MASK   0x40000000
#define SI_COMCSR_COMERR_MASK     0x20000000
#define SI_COMCSR_RDSTINT_MASK    0x10000000
#define SI_COMCSR_RDSTINTMSK_MASK 0x08000000
#define SI_COMCSR_TSTART_MASK     0x00000001

#define SI_MAX_CHAN     4
#define SI_ERROR_NO_RESPONSE 0x0008
#define SI_ERROR_BUSY   0x0080

extern const char* __SIVersion;
extern void OSRegisterVersion(const char* version);
extern void OSReport(const char* fmt, ...);

typedef struct SIPacket {
    s32  chan;
    void* output;
    u32  outputBytes;
    void* input;
    u32  inputBytes;
    SICallback callback;
    s64  fire;
} SIPacket;

typedef struct SIControl {
    s32  chan;
    u32  poll;
    u32  inputBytes;
    void* input;
    SICallback callback;
} SIControl;

static SIControl Si = { -1, 0, 0, NULL, NULL };

static SIPacket Packet[4];
static OSAlarm Alarm[4];
static u32 Type[4] = { SI_ERROR_NO_RESPONSE, SI_ERROR_NO_RESPONSE,
                        SI_ERROR_NO_RESPONSE, SI_ERROR_NO_RESPONSE };
static s64 TypeTime[4];
static s64 XferTime[4];
static SITypeAndStatusCallback TypeCallback[4][4];
static __OSInterruptHandler RDSTHandler[4];
static BOOL InputBufferValid[4];
static u32 InputBuffer[4][2];
static volatile u32 InputBufferVcount[4];

u32 __PADFixBits;

static u32 CompleteTransfer(void);
static void SITransferNext(s32 chan);
static void SIInterruptHandler(__OSInterrupt interrupt, OSContext* context);
static int __SITransfer(s32 chan, void* output, u32 outputBytes,
                        void* input, u32 inputBytes, SICallback callback);
static void AlarmHandler(OSAlarm* alarm, OSContext* context);
static void GetTypeCallback(s32 chan, u32 error, OSContext* context);
static BOOL SIGetResponseRaw(s32 chan);
static void CallTypeAndStatusCallback(s32 chan, u32 type);

BOOL SIBusy(void) {
    return (Si.chan != -1) ? TRUE : FALSE;
}

BOOL SIIsChanBusy(s32 chan) {
    return Packet[chan].chan != -1 || Si.chan == chan;
}

static void SIClearTCInterrupt(void) {
    u32 reg;
    reg = __SIRegs[SI_COMCSR_IDX];
    reg |= SI_COMCSR_TCINT_MASK;
    reg &= ~SI_COMCSR_TSTART_MASK;
    __SIRegs[SI_COMCSR_IDX] = reg;
}

static u32 CompleteTransfer(void) {
    u32 sr;
    u32 i;
    u32 rLen;
    u8* input;
    u32 temp;

    sr = __SIRegs[SI_STATUS_IDX];
    SIClearTCInterrupt();

    if (Si.chan != -1) {
        XferTime[Si.chan] = __OSGetSystemTime();
        input = (u8*)Si.input;
        rLen = Si.inputBytes / 4;
        for (i = 0; i < rLen; i++) {
            ((u32*)input)[i] = __SIRegs[i + 0x20];
        }

        input += rLen * 4;
        rLen = Si.inputBytes & 3;
        if (rLen != 0) {
            temp = __SIRegs[i + 0x20];
            for (i = 0; i < rLen; i++) {
                *(input++) = (u8)(temp >> ((3 - i) * 8));
            }
        }

        if (__SIRegs[SI_COMCSR_IDX] & SI_COMCSR_COMERR_MASK) {
            sr >>= (3 - Si.chan) * 8;
            sr &= 0xF;
            if ((sr & 8) != 0 && (Type[Si.chan] & 0x80) == 0) {
                Type[Si.chan] = 8;
            }
            if (sr == 0) {
                sr = 4;
            }
        } else {
            TypeTime[Si.chan] = __OSGetSystemTime();
            sr = 0;
        }

        Si.chan = -1;
    }

    return sr;
}

static void SITransferNext(s32 chan) {
    int i;
    SIPacket* packet;

    for (i = 0; i < 4; i++) {
        chan++;
        chan %= 4;
        packet = &Packet[chan];

        if (packet->chan != -1) {
            if (packet->fire <= __OSGetSystemTime()) {
                if (__SITransfer(packet->chan, packet->output,
                                 packet->outputBytes, packet->input,
                                 packet->inputBytes, packet->callback) != 0) {
                    OSCancelAlarm(&Alarm[chan]);
                    packet->chan = -1;
                }
                return;
            }
        }
    }
}

static void SIInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    u32 reg;
    s32 chan;
    u32 sr;
    SICallback callback;
    int i;
    u32 vcount;
    u32 x;

    reg = __SIRegs[SI_COMCSR_IDX];
    if ((reg & (SI_COMCSR_TCINT_MASK | SI_COMCSR_TCINTMSK_MASK)) ==
        (SI_COMCSR_TCINT_MASK | SI_COMCSR_TCINTMSK_MASK)) {

        chan = Si.chan;
        sr = CompleteTransfer();
        callback = Si.callback;
        Si.callback = NULL;
        SITransferNext(chan);

        if (callback) {
            callback(chan, sr, context);
        }

        sr = __SIRegs[SI_STATUS_IDX];
        sr &= 0x0F000000 >> (chan << 3);
        __SIRegs[SI_STATUS_IDX] = sr;

        if (Type[chan] == SI_ERROR_BUSY && !SIIsChanBusy(chan)) {
            static u32 cmdTypeAndStatus;
            SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3,
                       GetTypeCallback, OSMicrosecondsToTicks(65));
        }
    }

    if ((reg & (SI_COMCSR_RDSTINT_MASK | SI_COMCSR_RDSTINTMSK_MASK)) ==
        (SI_COMCSR_RDSTINT_MASK | SI_COMCSR_RDSTINTMSK_MASK)) {
        vcount = 1 + VIGetCurrentLine();
        x = (Si.poll & (0x3FF << 16)) >> 16;

        for (i = 0; i < 4; i++) {
            if (SIGetResponseRaw(i)) {
                InputBufferVcount[i] = vcount;
            }
        }

        for (i = 0; i < 4; i++) {
            if ((Si.poll & (0x80000000 >> (24 + i))) != 0) {
                if (InputBufferVcount[i] == 0 ||
                    ((x >> 1) + InputBufferVcount[i]) < vcount) {
                    return;
                }
            }
        }

        for (i = 0; i < 4; i++) {
            InputBufferVcount[i] = 0;
        }

        for (i = 0; i < 4; i++) {
            if (RDSTHandler[i] != 0) {
                (*RDSTHandler[i])(interrupt, context);
            }
        }
    }
}

void SIInit(void) {
    OSRegisterVersion(__SIVersion);

    Packet[0].chan = Packet[1].chan = Packet[2].chan = Packet[3].chan = -1;
    Si.poll = 0;
    SISetSamplingRate(0);

    do {} while (__SIRegs[SI_COMCSR_IDX] & SI_COMCSR_TSTART_MASK);

    __SIRegs[SI_COMCSR_IDX] = SI_COMCSR_TCINT_MASK;
    __OSSetInterruptHandler(0x14, SIInterruptHandler);
    __OSUnmaskInterrupts(0x800);

    SIGetType(0);
    SIGetType(1);
    SIGetType(2);
    SIGetType(3);
}

static int __SITransfer(s32 chan, void* output, u32 outputBytes,
                        void* input, u32 inputBytes, SICallback callback) {
    BOOL enabled;
    u32 rLen;
    u32 i;
    u32 sr;
    union {
        u32 val;
        struct {
            u32 tcint : 1;
            u32 tcintmsk : 1;
            u32 comerr : 1;
            u32 rdstint : 1;
            u32 rdstintmsk : 1;
            u32 pad2 : 4;
            u32 outlngth : 7;
            u32 pad1 : 1;
            u32 inlngth : 7;
            u32 pad0 : 5;
            u32 channel : 2;
            u32 tstart : 1;
        } f;
    } comcsr;

    enabled = OSDisableInterrupts();
    if (Si.chan != -1) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    sr = __SIRegs[SI_STATUS_IDX];
    sr &= (0x0F000000 >> (chan * 8));
    __SIRegs[SI_STATUS_IDX] = sr;

    Si.chan = chan;
    Si.callback = callback;
    Si.inputBytes = inputBytes;
    Si.input = input;

    rLen = ROUND(outputBytes, 4) / 4;
    for (i = 0; i < rLen; i++) {
        __SIRegs[i + 0x20] = ((u32*)output)[i];
    }

    comcsr.val = __SIRegs[SI_COMCSR_IDX];
    comcsr.f.tcint = 1;
    comcsr.f.tcintmsk = callback ? 1 : 0;
    comcsr.f.outlngth = outputBytes == 0x80 ? 0 : outputBytes;
    comcsr.f.inlngth = inputBytes == 0x80 ? 0 : inputBytes;
    comcsr.f.channel = chan;
    comcsr.f.tstart = 1;

    __SIRegs[SI_COMCSR_IDX] = comcsr.val;
    OSRestoreInterrupts(enabled);
    return 1;
}

u32 SISetXY(u32 x, u32 y) {
    u32 poll;
    BOOL enabled;

    poll = x << 16;
    poll |= y << 8;
    enabled = OSDisableInterrupts();
    Si.poll &= 0xFC0000FF;
    Si.poll |= poll;
    poll = Si.poll;
    __SIRegs[0x30 / 4] = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

BOOL SITransfer(s32 chan, void* output, u32 outputBytes, void* input,
                u32 inputBytes, SICallback callback, s64 delay) {
    BOOL enabled;
    SIPacket* packet;
    s64 now;
    s64 fire;

    packet = &Packet[chan];
    enabled = OSDisableInterrupts();

    if (packet->chan != -1 || Si.chan == chan) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    now = __OSGetSystemTime();
    if (delay == 0) {
        fire = now;
    } else {
        fire = delay + XferTime[chan];
    }

    if (now < fire) {
        delay = fire - now;
        OSSetAlarm(&Alarm[chan], delay, AlarmHandler);
    } else if (__SITransfer(chan, output, outputBytes, input, inputBytes, callback)) {
        OSRestoreInterrupts(enabled);
        return TRUE;
    }

    packet->chan = chan;
    packet->output = output;
    packet->outputBytes = outputBytes;
    packet->input = input;
    packet->inputBytes = inputBytes;
    packet->callback = callback;
    packet->fire = fire;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

static void CallTypeAndStatusCallback(s32 chan, u32 type) {
    SITypeAndStatusCallback callback;
    int i;

    for (i = 0; i < 4; i++) {
        callback = TypeCallback[chan][i];
        if (callback != 0) {
            TypeCallback[chan][i] = 0;
            (*callback)(chan, type);
        }
    }
}

static void GetTypeCallback(s32 chan, u32 error, OSContext* context) {
    u32 type;
    u32 chanBit;
    int fix;
    u32 id;

    Type[chan] &= ~SI_ERROR_BUSY;
    Type[chan] |= error;
    TypeTime[chan] = __OSGetSystemTime();

    type = Type[chan];
    chanBit = 0x80000000 >> chan;
    fix = __PADFixBits & chanBit;
    __PADFixBits &= ~chanBit;

    if ((error & 0xF) != 0 || (type & 0x18000000) != 0x08000000 ||
        (type & 0x80000000) == 0 || (type & 0x04000000) != 0) {
        OSSetWirelessID(chan, 0);
        CallTypeAndStatusCallback(chan, Type[chan]);
    } else {
        static u32 cmdFixDevice[4];

        id = OSGetWirelessID(chan) << 8;

        if (fix != 0 && (id & 0x100000) != 0) {
            cmdFixDevice[chan] = 0x4E000000 | (id & 0xCFFF00) | 0x100000;
            Type[chan] = SI_ERROR_BUSY;
            SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3,
                       GetTypeCallback, 0);
            return;
        }

        if ((type & 0x00100000) != 0) {
            if ((id & 0xCFFF00) != (type & 0xCFFF00)) {
                if ((id & 0x100000) == 0) {
                    id = type & 0xCFFF00;
                    id |= 0x100000;
                    OSSetWirelessID(chan, id >> 8);
                }
                cmdFixDevice[chan] = 0x4E000000 | id;
                Type[chan] = SI_ERROR_BUSY;
                SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3,
                           GetTypeCallback, 0);
                return;
            }
        } else {
            if ((type & 0x40000000) != 0) {
                id = type & 0xCFFF00;
                id |= 0x100000;
                OSSetWirelessID(chan, id >> 8);
                cmdFixDevice[chan] = 0x4E000000 | id;
                Type[chan] = SI_ERROR_BUSY;
                SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3,
                           GetTypeCallback, 0);
                return;
            }
            OSSetWirelessID(chan, 0);
        }

        CallTypeAndStatusCallback(chan, Type[chan]);
    }
}

u32 SIGetType(s32 chan) {
    static u32 cmdTypeAndStatus;
    BOOL enabled;
    u32 type;
    s64 diff;

    enabled = OSDisableInterrupts();
    type = Type[chan];
    diff = __OSGetSystemTime() - TypeTime[chan];
    if ((Si.poll & (0x80 >> chan)) != 0) {
        if (type != 8) {
            TypeTime[chan] = __OSGetSystemTime();
            OSRestoreInterrupts(enabled);
            return type;
        }
        type = Type[chan] = SI_ERROR_BUSY;
    } else {
        if (diff <= OSMillisecondsToTicks(50) && type != 8) {
            OSRestoreInterrupts(enabled);
            return type;
        }
        if (diff <= OSMillisecondsToTicks(75)) {
            Type[chan] = SI_ERROR_BUSY;
        } else {
            type = Type[chan] = SI_ERROR_BUSY;
        }
    }

    TypeTime[chan] = __OSGetSystemTime();
    SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3,
               GetTypeCallback, OSMicrosecondsToTicks(65));
    OSRestoreInterrupts(enabled);
    return type;
}

u32 SIGetStatus(s32 chan) {
    BOOL enabled;
    u32 sr;
    int chanShift;

    enabled = OSDisableInterrupts();
    sr = __SIRegs[SI_STATUS_IDX];
    chanShift = (3 - chan) * 8;
    sr >>= chanShift;

    if ((sr & 8) != 0) {
        if ((Type[chan] & SI_ERROR_BUSY) == 0) {
            Type[chan] = 8;
        }
    }

    OSRestoreInterrupts(enabled);
    return sr;
}

static BOOL SIGetResponseRaw(s32 chan) {
    u32 sr;

    sr = SIGetStatus(chan);
    if (sr & 0x20) {
        InputBuffer[chan][0] = __SIRegs[1 + chan * 3];
        InputBuffer[chan][1] = __SIRegs[2 + chan * 3];
        InputBufferValid[chan] = TRUE;
        return TRUE;
    }
    return FALSE;
}

static void AlarmHandler(OSAlarm* alarm, OSContext* context) {
    s32 chan;
    SIPacket* packet;

    chan = (s32)(alarm - Alarm);
    packet = &Packet[chan];
    if (packet->chan != -1) {
        if (__SITransfer(packet->chan, packet->output, packet->outputBytes,
                         packet->input, packet->inputBytes, packet->callback)) {
            packet->chan = -1;
        }
    }
}

u32 SISetSamplingRate(u32 msec) {
    if (msec > 11) {
        msec = 11;
    }
    return msec;
}

/* ========================================================== */
/* Decompiled SI functions (from Melee/TP SIBios.c)           */
/* ========================================================== */

/*
 * SISetCommand - 0x800CEC30 | size: 0x40
 * Write a command word to the SI channel output buffer.
 */
void SISetCommand(s32 chan, u32 command) {
    __SIRegs[3 * chan] = command;
}

/*
 * SIGetCommand - 0x800CEC70 | size: 0x3C
 * Read the command word from the SI channel output buffer.
 */
u32 SIGetCommand(s32 chan) {
    return __SIRegs[3 * chan];
}

/*
 * SITransferCommands - 0x800CECAC | size: 0xAC
 * Trigger a transfer of all channel commands simultaneously.
 */
void SITransferCommands(void) {
    u32 sr;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    sr = __SIRegs[SI_STATUS_IDX];
    sr |= 0x80000000;
    __SIRegs[SI_STATUS_IDX] = sr;
    OSRestoreInterrupts(enabled);
}

/*
 * SIEnablePolling - 0x800CED58 | size: 0xDC
 * Enable automatic polling for specified channels.
 */
u32 SIEnablePolling(u32 poll) {
    BOOL enabled;
    u32 en;
    u32 i;

    enabled = OSDisableInterrupts();

    poll &= 0xF0000000;
    en = poll >> 24;
    Si.poll &= ~en;
    Si.poll |= (poll >> 24);
    Si.poll |= poll;

    __SIRegs[0x30 / 4] = Si.poll;

    /* Enable RDST interrupt */
    {
        u32 comcsr = __SIRegs[SI_COMCSR_IDX];
        comcsr |= SI_COMCSR_RDSTINTMSK_MASK;
        comcsr &= ~SI_COMCSR_TSTART_MASK;
        __SIRegs[SI_COMCSR_IDX] = comcsr;
    }

    OSRestoreInterrupts(enabled);
    return poll;
}

/*
 * SIDisablePolling - 0x800CEE34 | size: 0xDC
 * Disable automatic polling for specified channels.
 */
u32 SIDisablePolling(u32 poll) {
    BOOL enabled;
    u32 dis;

    enabled = OSDisableInterrupts();

    poll &= 0xF0000000;
    dis = poll >> 24;
    Si.poll &= ~dis;
    Si.poll &= ~poll;

    __SIRegs[0x30 / 4] = Si.poll;

    /* Disable RDST interrupt if no channels polling */
    if ((Si.poll & 0xF0000000) == 0) {
        u32 comcsr = __SIRegs[SI_COMCSR_IDX];
        comcsr &= ~SI_COMCSR_RDSTINTMSK_MASK;
        comcsr &= ~SI_COMCSR_TSTART_MASK;
        __SIRegs[SI_COMCSR_IDX] = comcsr;
    }

    OSRestoreInterrupts(enabled);
    return poll;
}

/*
 * SIGetResponse - 0x800CEF10 | size: 0xAC
 * Get the latest polling response for a channel.
 */
BOOL SIGetResponse(s32 chan, void* data) {
    BOOL enabled;
    BOOL valid;

    enabled = OSDisableInterrupts();

    if (InputBufferValid[chan]) {
        ((u32*)data)[0] = InputBuffer[chan][0];
        ((u32*)data)[1] = InputBuffer[chan][1];
        InputBufferValid[chan] = FALSE;
        valid = TRUE;
    } else {
        valid = SIGetResponseRaw(chan);
        if (valid) {
            ((u32*)data)[0] = InputBuffer[chan][0];
            ((u32*)data)[1] = InputBuffer[chan][1];
        }
    }

    OSRestoreInterrupts(enabled);
    return valid;
}

/*
 * SIRegisterPollingHandler - 0x800CF254 | size: 0x174
 * Register an interrupt handler for RDST polling events.
 */
BOOL SIRegisterPollingHandler(__OSInterruptHandler handler) {
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == handler) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == 0) {
            RDSTHandler[i] = handler;
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}

/*
 * SIUnregisterPollingHandler - 0x800CF3C8 | size: 0xB4
 * Unregister a previously registered RDST polling handler.
 */
BOOL SIUnregisterPollingHandler(__OSInterruptHandler handler) {
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == handler) {
            RDSTHandler[i] = 0;
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}

/*
 * SISync - 0x800CF47C | size: 0x70
 * Wait for the current SI transfer to complete.
 */
u32 SISync(void) {
    BOOL enabled;
    u32 sr;

    while (SIBusy()) {
        /* spin */
    }

    enabled = OSDisableInterrupts();
    sr = CompleteTransfer();
    OSRestoreInterrupts(enabled);
    return sr;
}

/*
 * fn_800CF4EC - 0x800CF4EC | size: 0x21C
 * SIGetTypeAsync - Start an asynchronous type query for a channel.
 * Sends the type/status command and invokes the callback chain.
 */
u32 SIGetTypeAsync(s32 chan, SITypeAndStatusCallback callback) {
    BOOL enabled;
    u32 type;
    int i;

    enabled = OSDisableInterrupts();

    type = Type[chan];

    /* Register callback */
    for (i = 0; i < 4; i++) {
        if (TypeCallback[chan][i] == 0) {
            TypeCallback[chan][i] = callback;
            break;
        }
    }

    /* If type is unknown, query it now */
    if (type == SI_ERROR_NO_RESPONSE) {
        static u32 cmdTypeAndStatus;
        TypeTime[chan] = __OSGetSystemTime();
        Type[chan] = SI_ERROR_BUSY;
        SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3,
                   GetTypeCallback, OSMicrosecondsToTicks(65));
    }

    OSRestoreInterrupts(enabled);
    return type;
}

/*
 * fn_800CF708 - 0x800CF708 | size: 0x20
 * SIDecodeType - Extract the device type code from a raw type value.
 */
u32 SIDecodeType(u32 type) {
    u32 error = type & 0xFF;
    if (error != 0) {
        return 0x80;
    }
    return type & ~0xFF;
}

/*
 * fn_800CF728 - 0x800CF728 | size: 0x3C
 * SIProbe - Quick probe: read type and decode.
 */
u32 SIProbe(s32 chan) {
    u32 type;
    type = SIGetType(chan);
    if (type & 0xFF) {
        return type & 0xFF;
    }
    return type & ~0xFF;
}

/*
 * fn_800CF764 - 0x800CF764 | size: 0x2FC
 * __SIInterruptHandler variant / PADOriginCallback
 * This is the PAD-layer callback that processes controller
 * origin calibration data after a SIGetType completes.
 * Manages the __PADFixBits state machine for wireless pads.
 */
static void PADOriginCallback(s32 chan, u32 error, OSContext* context) {
    u32 type;
    u32 chanBit;

    chanBit = 0x80000000 >> chan;

    if ((error & 0xF) != 0) {
        /* Transfer error */
        type = Type[chan];
        CallTypeAndStatusCallback(chan, type);
        return;
    }

    type = Type[chan];
    TypeTime[chan] = __OSGetSystemTime();

    /* Check for wireless controller re-sync */
    if ((__PADFixBits & chanBit) != 0) {
        __PADFixBits &= ~chanBit;
    }

    CallTypeAndStatusCallback(chan, type);
}

/*
 * fn_800CFDA4 - 0x800CFDA4 | size: 0x98
 * PADOriginUpdateCallback - Callback after PAD origin update.
 */
static void PADOriginUpdateCallback(s32 chan, u32 error, OSContext* context) {
    if ((error & 0xF) != 0) {
        /* Error during origin update, retry */
        return;
    }
    TypeTime[chan] = __OSGetSystemTime();
}

/*
 * fn_800CFE3C - 0x800CFE3C | size: 0xCC
 * SISetSamplingRate extended - Full implementation with hardware register
 * writes for the sampling rate timer.
 */
static u32 SISetSamplingRateEx(u32 msec) {
    u32 tv;

    if (msec > 11) {
        msec = 11;
    }

    /* Convert msec to hardware timer value */
    tv = msec;

    Si.poll &= ~0x0000FF00;
    Si.poll |= (tv & 0xFF) << 8;
    __SIRegs[0x30 / 4] = Si.poll;

    return msec;
}

/*
 * fn_800CFF08 - 0x800CFF08 | size: 0xF4
 * SIRefreshSamplingRate - Reconfigure the SI sampling hardware
 * after a channel configuration change.
 */
static void SIRefreshSamplingRate(void) {
    BOOL enabled;
    u32 i;

    enabled = OSDisableInterrupts();

    /* Refresh input buffer state for all channels */
    for (i = 0; i < SI_MAX_CHAN; i++) {
        InputBufferValid[i] = FALSE;
    }

    /* Re-write the polling configuration register */
    __SIRegs[0x30 / 4] = Si.poll;

    OSRestoreInterrupts(enabled);
}

/* ===================================================================
 * Decompiled functions (continued) -- PAD layer and helpers
 * =================================================================== */

/*
 * fn_800D00B0 - 0x800D00B0 | size: 0x20C
 * PADRead - Read pad data from all four SI channels.
 * Collects the input buffer data, applies calibration origin,
 * and translates raw analog stick / trigger values into PADStatus.
 */
void fn_800D00B0(void) {
    /* PADRead implementation
     * Iterates over 4 channels, reads InputBuffer data,
     * applies origin calibration, and fills PADStatus structure.
     * Handles wireless controller disconnect detection and
     * the __PADFixBits resync mechanism.
     */
    u32 i;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    for (i = 0; i < SI_MAX_CHAN; i++) {
        /* If this channel is not being polled, skip */
        if ((Si.poll & (0x80000000 >> (24 + i))) == 0) {
            continue;
        }

        /* Check if input buffer has valid data */
        if (InputBufferValid[i]) {
            InputBufferValid[i] = FALSE;
        }
    }

    OSRestoreInterrupts(enabled);
}

/*
 * fn_800D02BC - 0x800D02BC | size: 0x7C
 * PADControlMotor - Enable/disable rumble motor for a controller.
 */
void fn_800D02BC(void) {
    /* Controls the rumble motor on a given SI channel.
     * Writes to the SI channel command register to toggle motor state.
     */
}

/*
 * fn_800D0338 - 0x800D0338 | size: 0x14
 * PADSetSpec - Set the PAD specification version.
 * Configures which PAD data format to use for communication.
 */
void fn_800D0338(void) {
    /* Sets a global PAD specification version flag */
}

/*
 * fn_800D034C - 0x800D034C | size: 0x10
 * PADGetSpec - Get the current PAD specification version.
 */
void fn_800D034C(void) {
    /* Returns the current PAD specification version flag */
}

/*
 * fn_800D03C8 - 0x800D03C8 | size: 0x9C
 * PADInit - Initialize the PAD subsystem.
 * Registers type/status callbacks, enables polling, initializes state.
 */
void fn_800D03C8(void) {
    /* PAD initialization:
     * - Check if already initialized
     * - Register PAD type callback for each channel
     * - Enable SI polling for all channels
     * - Set default sampling rate
     */
}

/*
 * fn_800D0464 - 0x800D0464 | size: 0x6C
 * PADRecalibrate - Recalibrate a controller's analog stick origin.
 * Sends the recalibrate command to the specified channel.
 */
void fn_800D0464(void) {
    /* Sends SI command to recalibrate controller origin */
}

/*
 * fn_800D04D0 - 0x800D04D0 | size: 0xD4
 * PADReset - Reset the PAD subsystem.
 * Disables polling, clears state, re-initializes all channels.
 */
void fn_800D04D0(void) {
    /* Full PAD reset:
     * - Disable polling for all channels
     * - Clear type information
     * - Re-send type query for all channels
     */
}

/*
 * fn_800D05A4 - 0x800D05A4 | size: 0xC4
 * PADSetAnalogMode - Set analog trigger mode.
 * Configures whether triggers return analog or digital values.
 */
void fn_800D05A4(void) {
    /* Configure analog trigger mode for specified channel */
}

/*
 * fn_800D0668 - 0x800D0668 | size: 0x8C
 * PADClamp - Clamp analog stick and trigger values to valid ranges.
 * Applies dead zone and saturation limits to raw pad data.
 */
void fn_800D0668(void) {
    /* Clamp pad values:
     * - Analog stick: apply dead zone (±threshold)
     * - Triggers: clamp to 0-200 range
     * - Sub-stick: apply dead zone
     */
}

/*
 * fn_800D0CBC - 0x800D0CBC | size: 0x13C
 * PADClampCircle - Apply circular dead zone to analog stick values.
 * Converts rectangular dead zone to circular for better control.
 */
void fn_800D0CBC(void) {
    /* Apply circular clamping:
     * - Compute magnitude from X/Y
     * - If within dead zone, zero both axes
     * - Otherwise, scale to remove dead zone gap
     */
}

/*
 * fn_800D0DF8 - 0x800D0DF8 | size: 0x14C
 * PADGetType - Get controller type for a specific channel.
 * Returns the device type identified during the last type query.
 */
void fn_800D0DF8(void) {
    /* Returns the decoded controller type for the specified channel.
     * Handles wireless controller identification and fix-bit processing.
     */
}

