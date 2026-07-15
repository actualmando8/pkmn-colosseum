/**
 * @file SI_range_800CF764.c
 * @brief Dolphin SI (one TU proven by static GetTypeCallback linkage), 0x800CF764 - 0x800D0DF8.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) - mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"
#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OSInterrupt.h"

#pragma push
#pragma optimization_level 0
#pragma scheduling off
#pragma scheduling off
typedef struct {
    u32 reg;
    u32 unk4;
    u32 unk8;
} SICommandQueueEntry;
#pragma scheduling reset
#pragma scheduling reset
#pragma pop

#pragma peephole off
BOOL SIEnablePollingInterrupt(BOOL enable) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern u32 Packet_803FFFB0[];
    volatile u32* regs;
    u32* inputBufferVcount;
    BOOL enabled;
    BOOL previous;
    u32 reg;

    inputBufferVcount = &Packet_803FFFB0[120];
    enabled = OSDisableInterrupts();
    regs = (volatile u32*)0xCC006400;
    reg = regs[13];
    previous = (reg & 0x08000000) ? TRUE : FALSE;

    if (enable) {
        inputBufferVcount[3] = inputBufferVcount[2] = inputBufferVcount[1] =
            inputBufferVcount[0] = 0;
        reg |= 0x08000000;
    } else {
        reg &= ~0x08000000;
    }

    reg &= ~0x80000001;
    regs[13] = reg;
    OSRestoreInterrupts(enabled);
    return previous;
}
#pragma peephole reset

#pragma dont_inline on
BOOL SIRegisterPollingHandler(__OSInterruptHandler handler) {
    extern __OSInterruptHandler lbl_80400150[];
    BOOL enabled;
    s32 i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        if (lbl_80400150[i] == handler) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    for (i = 0; i < 4; i++) {
        if (lbl_80400150[i] == 0) {
            lbl_80400150[i] = handler;
            SIEnablePollingInterrupt(TRUE);
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}
#pragma dont_inline reset

#pragma dont_inline on
BOOL SIUnregisterPollingHandler(__OSInterruptHandler handler) {
    extern __OSInterruptHandler lbl_80400150[];
    BOOL enabled;
    s32 i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        if (lbl_80400150[i] == handler) {
            lbl_80400150[i] = 0;

            for (i = 0; i < 4; i++) {
                if (lbl_80400150[i] != 0) {
                    break;
                }
            }

            if (i == 4) {
                SIEnablePollingInterrupt(FALSE);
            }

            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}
#pragma dont_inline reset

void SIInit(void) {
    typedef struct {
        s32 chan;
        u8 pad[0x1C];
    } SIPacket;
    extern char* __SIVersion;
    extern SIPacket Packet_803FFFB0[];
    extern u32 Si_80313F8C[];
    extern void OSRegisterVersion(const char* version);
    extern u32 SISetSamplingRate(u32 msec);
    extern void SIInterruptHandler_800CFA60(__OSInterrupt interrupt,
                                            OSContext* context);
    extern u32 SIGetType(s32 chan);
    SIPacket* packet = Packet_803FFFB0;
    volatile u32* regs;

    OSRegisterVersion(__SIVersion);
    packet[0].chan = packet[1].chan = packet[2].chan = packet[3].chan = -1;
    Si_80313F8C[1] = 0;
    SISetSamplingRate(0);

    regs = (volatile u32*)0xCC006400;
    while (regs[13] & 1) {
    }
    regs[13] = 0x80000000;

    __OSSetInterruptHandler(0x14, SIInterruptHandler_800CFA60);
    __OSUnmaskInterrupts(0x800);

    SIGetType(0);
    SIGetType(1);
    SIGetType(2);
    SIGetType(3);
}

u32 SIGetStatus(s32 chan) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern u32 Type_80313FA0[];
    BOOL enabled;
    u32 sr;
    s32 chanShift;

    enabled = OSDisableInterrupts();
    sr = *(volatile u32*)0xCC006438;
    chanShift = (3 - chan) * 8;
    sr >>= chanShift;

    if ((sr & 8) != 0) {
        if ((Type_80313FA0[chan] & 0x80) == 0) {
            Type_80313FA0[chan] = 8;
        }
    }

    OSRestoreInterrupts(enabled);
    return sr;
}

void fn_800D0338(s32 chan, u32 command) {
    ((volatile SICommandQueueEntry*)0xCC006400)[chan].reg = command;
}

void fn_800D034C(void) {
    u32 command;

    command = 0x80000000u;
    *(volatile u32*)0xCC006438 = command;
}

u32 SISetXY(u32 x, u32 y) {
    typedef struct {
        s32 chan;
        u32 poll;
        u32 inputBytes;
        void* input;
        void* callback;
    } SIControl;
    extern SIControl Si_80313F8C;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    u32 poll;
    BOOL enabled;

    poll = x << 16;
    poll |= y << 8;
    enabled = OSDisableInterrupts();
    Si_80313F8C.poll &= 0xFC0000FF;
    Si_80313F8C.poll |= poll;
    poll = Si_80313F8C.poll;
    *(volatile u32*)0xCC006430 = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

u32 SIEnablePolling(u32 poll) {
    typedef struct {
        s32 chan;
        u32 poll;
        u32 inputBytes;
        void* input;
        void* callback;
    } SIControl;
    extern SIControl Si_80313F8C;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    u32 enabled;
    u32 en;
    u32* pp;
    volatile u32* regs = (volatile u32*)0xCC006400;

    if (poll == 0) {
        return Si_80313F8C.poll;
    }

    enabled = OSDisableInterrupts();
    poll >>= 24;
    en = (poll >> 4) & 0xF;
    pp = &Si_80313F8C.poll;
    *pp &= ~en;
    poll &= en | 0x03FFFFF0;
    poll &= 0xFC0000FF;
    *pp |= poll;
    poll = *pp;
    regs[14] = 0x80000000;
    regs[12] = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

u32 SIDisablePolling(u32 poll) {
    typedef struct {
        s32 chan;
        u32 poll;
        u32 inputBytes;
        void* input;
        void* callback;
    } SIControl;
    extern SIControl Si_80313F8C;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    BOOL enabled;

    if (poll == 0) {
        return Si_80313F8C.poll;
    }

    enabled = OSDisableInterrupts();
    poll >>= 24;
    poll &= 0xF0;
    poll = Si_80313F8C.poll & ~poll;
    *(volatile u32*)0xCC006430 = poll;
    Si_80313F8C.poll = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

BOOL SIGetResponseRaw(s32 chan) {
    extern u8 Packet_803FFFB0[];
    extern u32 Type_80313FA0[];
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    u8* packet = Packet_803FFFB0;
    volatile u32* regs = (volatile u32*)0xCC006400;
    BOOL enabled;
    u32 sr;
    s32 chanShift;

    enabled = OSDisableInterrupts();
    sr = regs[14];
    chanShift = (3 - chan) * 8;
    sr >>= chanShift;

    if ((sr & 8) != 0) {
        if ((Type_80313FA0[chan] & 0x80) == 0) {
            Type_80313FA0[chan] = 8;
        }
    }

    OSRestoreInterrupts(enabled);
    if ((sr & 0x20) != 0) {
        *(u32*)(packet + 0x1C0 + chan * 8) = regs[1 + chan * 3];
        *(u32*)(packet + 0x1C4 + chan * 8) = regs[2 + chan * 3];
        *(BOOL*)(packet + 0x1B0 + chan * 4) = TRUE;
        return TRUE;
    }
    return FALSE;
}

#pragma dont_inline on
BOOL SIGetResponse(s32 chan, void* data) {
    typedef struct {
        u8 pad[0x1B0];
        BOOL valid[4];
        u32 input[4][2];
    } SIResponseState;
    extern SIResponseState Packet_803FFFB0;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    SIResponseState* packet = &Packet_803FFFB0;
    volatile SICommandQueueEntry* regs =
        (volatile SICommandQueueEntry*)0xCC006400;
    BOOL enabled;
    BOOL valid;

    enabled = OSDisableInterrupts();

    if ((SIGetStatus(chan) & 0x20) != 0) {
        packet->input[chan][0] = regs[chan].unk4;
        packet->input[chan][1] = regs[chan].unk8;
        packet->valid[chan] = TRUE;
    }

    valid = packet->valid[chan];
    packet->valid[chan] = FALSE;
    if (valid) {
        ((u32*)data)[0] = packet->input[chan][0];
        ((u32*)data)[1] = packet->input[chan][1];
    }

    OSRestoreInterrupts(enabled);
    return valid;
}
#pragma dont_inline reset

void AlarmHandler_800D0668(OSAlarm* alarm, OSContext* context) {
    typedef void (*SICallback)(s32 chan, u32 sr, OSContext* context);
    typedef struct {
        s32 chan;
        void* output;
        u32 outputBytes;
        void* input;
        u32 inputBytes;
        SICallback callback;
        s64 fire;
    } SIPacket;
    extern OSAlarm lbl_80400030[];
    extern SIPacket Packet_803FFFB0[];
    extern int __SITransfer(s32 chan, void* output, u32 outputBytes,
                            void* input, u32 inputBytes, SICallback callback);
    s32 chan;
    SIPacket* packet;

    chan = (s32)(alarm - lbl_80400030);
    packet = &Packet_803FFFB0[chan];
    if (packet->chan != -1) {
        if (__SITransfer(packet->chan, packet->output, packet->outputBytes,
                         packet->input, packet->inputBytes, packet->callback)) {
            packet->chan = -1;
        }
    }
}
