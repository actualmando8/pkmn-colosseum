#include "dolphin/types.h"
#include "dolphin/pad/Pad.h"
#include "dolphin/si/SI.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSInterrupt.h"

/* Internal PAD library state. Names kept as lbl_/fn_ where the exact
 * original SDK identifier is not yet confirmed by symbols.txt. */
extern char *lbl_80478A08;         /* version string, passed to OSRegisterVersion */
extern u32 lbl_80478A0C;           /* ResettingChan */
extern u32 lbl_80478A10;           /* per-channel enable mask (bongo-fix related) */
extern u32 lbl_80478A14;           /* AnalogMode */
extern u32 lbl_80478A18;           /* Spec */
extern u32 lbl_80478A1C;           /* MakeStatus function pointer */
extern u32 lbl_80478A20;           /* SITransfer scratch output buffer (1 byte cmd) */
extern u32 lbl_80478A24;           /* SITransfer scratch output buffer (3 byte cmd) */

extern u32 lbl_8047A8A0;           /* Initialized */
extern u32 lbl_8047A8A4;           /* EnabledBits */
extern u32 lbl_8047A8A8;           /* ResettingBits */
extern u32 lbl_8047A8AC;           /* RecalibrateBits */
extern u32 lbl_8047A8B0;           /* ProbingBits */
extern u32 lbl_8047A8B4;           /* WaitingBits */
extern u32 lbl_8047A8B8;           /* CheckingBits */
extern PADSamplingCallback lbl_8047A8BC; /* SamplingCallback */
extern u32 lbl_8047A8C0;           /* OnReset() static "recalibrated" flag */
extern u32 __PADSpec;
extern u32 lbl_8047AA58;

extern PADStatus lbl_803FC5E0[4];  /* Origin[4] */
extern u32 lbl_803FC5D0[4];        /* Type[4] */

extern u16 __OSWirelessPadFixMode; /* absolute address 0x800030E0 */

/* SI library helpers not yet recovered by name (unassigned SI unit). */
extern void fn_800D0338(s32 chan, u32 command);       /* SISetCommand */
extern u32 fn_800D03C8(u32 poll);                       /* SIEnablePolling */
extern u32 fn_800D0464(u32 poll);                        /* SIDisablePolling */
extern BOOL fn_800D05A4(s32 chan, void *data);            /* SIGetResponse */
extern void fn_800D034C(void);                             /* SITransferCommands */
extern void fn_800D0CBC(s32 chan, SITypeAndStatusCallback cb);
extern BOOL fn_800CF708(void);
extern BOOL fn_800CF728(s32 chan);
extern u32 fn_800D02BC(s32 chan);
extern u32 fn_800C4C98(u32 hi, u32 lo, u32 shift);
extern void fn_800D104C(void);
extern void OSRegisterVersion(char *version);

static void SPEC0_MakeStatus(s32 chan, PADStatus *status, u32 data[2]);
static void SPEC1_MakeStatus(s32 chan, PADStatus *status, u32 data[2]);
static void SPEC2_MakeStatus(s32 chan, PADStatus *status, u32 data[2]);
static void fn_800ABEFC(__OSInterrupt interrupt, OSContext *context);
static void UpdateOrigin(s32 chan);
static void PADOriginCallback(s32 chan, u32 error, OSContext *context);
static void fn_800AA73C(s32 chan, u32 error, OSContext *context);
static void PADProbeCallback(s32 chan, u32 error, OSContext *context);
static void PADTypeAndStatusCallback(s32 chan, u32 type);
/* forward declaration provided by dolphin/pad/Pad.h */

/*
 * fn_800AB5B4 = PADSetSpec (byte-matches).
 */
void PADSetSpec(s32 spec) {
    __PADSpec = 0;
    switch (spec) {
    case 0:
        lbl_80478A1C = (u32)SPEC0_MakeStatus;
        break;
    case 1:
        lbl_80478A1C = (u32)SPEC1_MakeStatus;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        lbl_80478A1C = (u32)SPEC2_MakeStatus;
        break;
    }
    lbl_80478A18 = spec;
}

/*
 * fn_800AB614 = SPEC0_MakeStatus
 */
static void SPEC0_MakeStatus(s32 chan, PADStatus *status, u32 data[2]) {
    status->button = 0;
    status->button |= ((data[0] >> 16) & 0x0008) ? PAD_BUTTON_A : 0;
    status->button |= ((data[0] >> 16) & 0x0020) ? PAD_BUTTON_B : 0;
    status->button |= ((data[0] >> 16) & 0x0100) ? PAD_BUTTON_X : 0;
    status->button |= ((data[0] >> 16) & 0x0001) ? PAD_BUTTON_Y : 0;
    status->button |= ((data[0] >> 16) & 0x0010) ? PAD_BUTTON_START : 0;
    status->stickX = (s8)(data[1] >> 16);
    status->stickY = (s8)(data[1] >> 24);
    status->substickX = (s8)(data[1]);
    status->substickY = (s8)(data[1] >> 8);
    status->triggerLeft = (u8)(data[0] >> 8);
    status->triggerRight = (u8)data[0];
    status->analogA = 0;
    status->analogB = 0;
    if (170 <= status->triggerLeft)
        status->button |= PAD_TRIGGER_L;
    if (170 <= status->triggerRight)
        status->button |= PAD_TRIGGER_R;
    status->stickX -= 128;
    status->stickY -= 128;
    status->substickX -= 128;
    status->substickY -= 128;
}

/*
 * fn_800AB788 = SPEC1_MakeStatus
 */
static void SPEC1_MakeStatus(s32 chan, PADStatus *status, u32 data[2]) {
    status->button = 0;
    status->button |= ((data[0] >> 16) & 0x0080) ? PAD_BUTTON_A : 0;
    status->button |= ((data[0] >> 16) & 0x0100) ? PAD_BUTTON_B : 0;
    status->button |= ((data[0] >> 16) & 0x0020) ? PAD_BUTTON_X : 0;
    status->button |= ((data[0] >> 16) & 0x0010) ? PAD_BUTTON_Y : 0;
    status->button |= ((data[0] >> 16) & 0x0200) ? PAD_BUTTON_START : 0;
    status->stickX = (s8)(data[1] >> 16);
    status->stickY = (s8)(data[1] >> 24);
    status->substickX = (s8)(data[1]);
    status->substickY = (s8)(data[1] >> 8);
    status->triggerLeft = (u8)(data[0] >> 8);
    status->triggerRight = (u8)data[0];
    status->analogA = 0;
    status->analogB = 0;
    if (170 <= status->triggerLeft)
        status->button |= PAD_TRIGGER_L;
    if (170 <= status->triggerRight)
        status->button |= PAD_TRIGGER_R;
    status->stickX -= 128;
    status->stickY -= 128;
    status->substickX -= 128;
    status->substickY -= 128;
}

static s8 ClampS8(s8 var, s8 org) {
    if (0 < org) {
        s8 min = (s8)(-128 + org);
        if (var < min)
            var = min;
    } else if (org < 0) {
        s8 max = (s8)(127 + org);
        if (max < var)
            var = max;
    }
    return var -= org;
}

static u8 ClampU8(u8 var, u8 org) {
    if (var < org)
        var = org;
    return var -= org;
}

/*
 * fn_800AB8FC = SPEC2_MakeStatus
 */
static void SPEC2_MakeStatus(s32 chan, PADStatus *status, u32 data[2]) {
    PADStatus *origin;

    status->button = (u16)((data[0] >> 16) & 0x3FFF);
    status->stickX = (s8)(data[0] >> 8);
    status->stickY = (s8)(data[0]);

    switch (lbl_80478A14 & 0x00000700) {
    case 0x00000000:
    case 0x00000500:
    case 0x00000600:
    case 0x00000700:
        status->substickX = (s8)(data[1] >> 24);
        status->substickY = (s8)(data[1] >> 16);
        status->triggerLeft = (u8)(((data[1] >> 12) & 0x0f) << 4);
        status->triggerRight = (u8)(((data[1] >> 8) & 0x0f) << 4);
        status->analogA = (u8)(((data[1] >> 4) & 0x0f) << 4);
        status->analogB = (u8)(((data[1] >> 0) & 0x0f) << 4);
        break;
    case 0x00000100:
        status->substickX = (s8)(((data[1] >> 28) & 0x0f) << 4);
        status->substickY = (s8)(((data[1] >> 24) & 0x0f) << 4);
        status->triggerLeft = (u8)(data[1] >> 16);
        status->triggerRight = (u8)(data[1] >> 8);
        status->analogA = (u8)(((data[1] >> 4) & 0x0f) << 4);
        status->analogB = (u8)(((data[1] >> 0) & 0x0f) << 4);
        break;
    case 0x00000200:
        status->substickX = (s8)(((data[1] >> 28) & 0x0f) << 4);
        status->substickY = (s8)(((data[1] >> 24) & 0x0f) << 4);
        status->triggerLeft = (u8)(((data[1] >> 20) & 0x0f) << 4);
        status->triggerRight = (u8)(((data[1] >> 16) & 0x0f) << 4);
        status->analogA = (u8)(data[1] >> 8);
        status->analogB = (u8)(data[1] >> 0);
        break;
    case 0x00000300:
        status->substickX = (s8)(data[1] >> 24);
        status->substickY = (s8)(data[1] >> 16);
        status->triggerLeft = (u8)(data[1] >> 8);
        status->triggerRight = (u8)(data[1] >> 0);
        status->analogA = 0;
        status->analogB = 0;
        break;
    case 0x00000400:
        status->substickX = (s8)(data[1] >> 24);
        status->substickY = (s8)(data[1] >> 16);
        status->triggerLeft = 0;
        status->triggerRight = 0;
        status->analogA = (u8)(data[1] >> 8);
        status->analogB = (u8)(data[1] >> 0);
        break;
    }

    status->stickX -= 128;
    status->stickY -= 128;
    status->substickX -= 128;
    status->substickY -= 128;

    origin = &lbl_803FC5E0[chan];
    status->stickX = ClampS8(status->stickX, origin->stickX);
    status->stickY = ClampS8(status->stickY, origin->stickY);
    status->substickX = ClampS8(status->substickX, origin->substickX);
    status->substickY = ClampS8(status->substickY, origin->substickY);
    status->triggerLeft = ClampU8(status->triggerLeft, origin->triggerLeft);
    status->triggerRight = ClampU8(status->triggerRight, origin->triggerRight);
}

/*
 * fn_800AA4D4 = UpdateOrigin
 */
static void UpdateOrigin(s32 chan) {
    PADStatus *origin = &lbl_803FC5E0[chan];
    u32 chanBit = 0x80000000u >> chan;

    switch (lbl_80478A14 & 0x00000700) {
    case 0x00000000:
    case 0x00000500:
    case 0x00000600:
    case 0x00000700:
        origin->triggerLeft &= ~0xF;
        origin->triggerRight &= ~0xF;
        origin->analogA &= ~0xF;
        origin->analogB &= ~0xF;
        break;
    case 0x00000100:
        origin->substickX &= ~0xF;
        origin->substickY &= ~0xF;
        origin->analogA &= ~0xF;
        origin->analogB &= ~0xF;
        break;
    case 0x00000200:
        origin->substickX &= ~0xF;
        origin->substickY &= ~0xF;
        origin->triggerLeft &= ~0xF;
        origin->triggerRight &= ~0xF;
        break;
    case 0x00000300:
    case 0x00000400:
        break;
    }

    origin->stickX -= 128;
    origin->stickY -= 128;
    origin->substickX -= 128;
    origin->substickY -= 128;

    if (lbl_80478A10 & chanBit) {
        if (origin->stickX > 0x40) {
            if ((SIGetType(chan) & 0xFFFF0000u) == 0x09000000u) {
                origin->stickX = 0;
            }
        }
    }
}

/*
 * fn_800AA678 = PADOriginCallback
 */
static void PADOriginCallback(s32 chan, u32 error, OSContext *context) {
    if (!(error & 0xF)) {
        UpdateOrigin(lbl_80478A0C);
        lbl_8047A8A4 |= (0x80000000u >> lbl_80478A0C);
        fn_800D05A4(lbl_80478A0C, context);
        fn_800D0338(lbl_80478A0C, lbl_80478A14 | 0x400000);
        fn_800D03C8(lbl_8047A8A4);
    }

    /* DoReset() */
    lbl_80478A0C = __cntlzw(lbl_8047A8A8);
    if (lbl_80478A0C != 0x20) {
        u32 chanBit = 0x80000000u >> lbl_80478A0C;
        lbl_8047A8A8 &= ~chanBit;
        memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
        lbl_803FC5D0[lbl_80478A0C] = 0;
        fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
    }
}

/*
 * fn_800AA73C = fn_800AA73C
 */
static void fn_800AA73C(s32 chan, u32 error, OSContext *context) {
    u32 chanBit = 0x80000000u >> chan;

    if (lbl_8047A8A4 & chanBit) {
        if (!(error & 0xF)) {
            UpdateOrigin(chan);
        }
        if (error & 0x8) {
            BOOL enabled = OSDisableInterrupts();
            lbl_8047A8A4 &= ~chanBit;
            lbl_8047A8B0 &= ~chanBit;
            lbl_8047A8B4 &= ~chanBit;
            lbl_8047A8B8 &= ~chanBit;
            OSSetWirelessID(chan, 0);
            OSRestoreInterrupts(enabled);
        }
    }
}

/*
 * fn_800AA7FC = PADProbeCallback
 */
static void PADProbeCallback(s32 chan, u32 error, OSContext *context) {
    if (!(error & 0xF)) {
        lbl_8047A8A4 |= (0x80000000u >> lbl_80478A0C);
        fn_800D05A4(lbl_80478A0C, context);
        fn_800D0338(lbl_80478A0C, lbl_80478A14 | 0x400000);
        fn_800D03C8(lbl_8047A8A4);
        lbl_8047A8B0 |= (0x80000000u >> lbl_80478A0C);
    }

    /* DoReset() */
    lbl_80478A0C = __cntlzw(lbl_8047A8A8);
    if (lbl_80478A0C != 0x20) {
        u32 chanBit = 0x80000000u >> lbl_80478A0C;
        lbl_8047A8A8 &= ~chanBit;
        memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
        lbl_803FC5D0[lbl_80478A0C] = 0;
        fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
    }
}

/*
 * fn_800AA8D4 = PADTypeAndStatusCallback
 */
static void PADTypeAndStatusCallback(s32 chan, u32 type) {
    u32 chanBit;

    chanBit = 0x80000000u >> lbl_80478A0C;
    lbl_8047A8AC &= ~chanBit;

    if (!(lbl_8047A8AC & chanBit)) {
        /* recalibrate not requested: DoReset() */
        lbl_80478A0C = __cntlzw(lbl_8047A8A8);
        if (lbl_80478A0C == 0x20)
            return;
        chanBit = 0x80000000u >> lbl_80478A0C;
        lbl_8047A8A8 &= ~chanBit;
        memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
        lbl_803FC5D0[lbl_80478A0C] = 0;
        fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
        return;
    }

    lbl_803FC5D0[chan] = type & 0xFFFF0000u;

    if (((type >> 16) & 0x18) != 0 && !((type >> 16) & 0x80)) {
        /* DoReset() */
        lbl_80478A0C = __cntlzw(lbl_8047A8A8);
        if (lbl_80478A0C == 0x20)
            return;
        chanBit = 0x80000000u >> lbl_80478A0C;
        lbl_8047A8A8 &= ~chanBit;
        memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
        lbl_803FC5D0[lbl_80478A0C] = 0;
        fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
        return;
    }

    if (lbl_80478A18 < 2) {
        lbl_8047A8A4 |= (0x80000000u >> chan);
        fn_800D05A4(chan, 0);
        fn_800D0338(chan, lbl_80478A14 | 0x400000);
        fn_800D03C8(lbl_8047A8A4);

        /* DoReset() */
        lbl_80478A0C = __cntlzw(lbl_8047A8A8);
        if (lbl_80478A0C == 0x20)
            return;
        chanBit = 0x80000000u >> lbl_80478A0C;
        lbl_8047A8A8 &= ~chanBit;
        memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
        lbl_803FC5D0[lbl_80478A0C] = 0;
        fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
        return;
    }

    /* type has the 0x100000 wireless-id-present bit */
    if (type & 0x100000) {
        BOOL idMatch = FALSE;
        if (idMatch) {
            fn_800D0338(chan, lbl_80478A14 | 0x400000);
        }
        SITransfer(chan, &lbl_80478A24, 3, &lbl_803FC5E0[chan].triggerLeft, 0xA,
                   PADOriginCallback, 0);
    } else if ((type & 0x40000000) && !(type & 0x80000) && !(type & 0x40000)) {
        SITransfer(chan, &lbl_80478A20, 1, &lbl_803FC5E0[chan].triggerLeft, 0xA,
                   PADOriginCallback, 0);
    } else {
        SITransfer(chan, &lbl_80478A20, 1, &lbl_803FC5E0[chan].analogA, 8,
                   PADProbeCallback, 0);
    }
}

/*
 * fn_800AAC00 (unmatched attempt; see .inc for the ground-truth asm)
 */
void fn_800AAC00(s32 chan, u32 error) {
    u32 chanBit = 0x80000000u >> chan;

    if (lbl_8047A8A4 & chanBit) {
        u32 masked;
        lbl_8047A8B0 &= ~chanBit;
        lbl_8047A8B4 &= ~chanBit;

        masked = error & ~0xFF;
        if ((error & 0xF) == 0 && (masked & 0x80000000u) && (masked & 0x100000) &&
            (masked & 0x40000000) && !(masked & 0x80000) && !(masked & 0x40000) &&
            !(masked & 0x4000000)) {
            SITransfer(chan, &lbl_80478A20, 1, &lbl_803FC5E0[chan].triggerLeft, 0xA,
                       fn_800AA73C, 0);
            return;
        }
    }

    {
        BOOL enabled = OSDisableInterrupts();
        lbl_8047A8A4 &= ~chanBit;
        lbl_8047A8B0 &= ~chanBit;
        lbl_8047A8B4 &= ~chanBit;
        lbl_8047A8B8 &= ~chanBit;
        OSSetWirelessID(chan, 0);
        OSRestoreInterrupts(enabled);
    }
}

/*
 * fn_800AAD34 = PADReset (unmatched attempt)
 */
BOOL fn_800AAD34(u32 mask) {
    BOOL enabled = OSDisableInterrupts();

    mask |= lbl_8047A8B8;
    lbl_8047A8B8 = 0;
    mask &= ~(lbl_8047A8B0 | lbl_8047A8B4);
    lbl_8047A8A8 |= mask;
    lbl_8047A8A4 &= ~mask;
    if (lbl_80478A18 == 4) {
        lbl_8047A8AC |= mask;
    }
    fn_800D0464(lbl_8047A8A8);

    if (lbl_80478A0C == 0x20) {
        lbl_80478A0C = __cntlzw(lbl_8047A8A8);
        if (lbl_80478A0C != 0x20) {
            u32 chanBit = 0x80000000u >> lbl_80478A0C;
            lbl_8047A8A8 &= ~chanBit;
            memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
            lbl_803FC5D0[lbl_80478A0C] = 0;
            fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
        }
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/*
 * fn_800AAE34 = PADRecalibrate (unmatched attempt)
 */
BOOL fn_800AAE34(u32 mask) {
    BOOL enabled = OSDisableInterrupts();

    mask |= lbl_8047A8B8;
    lbl_8047A8B8 = 0;
    mask &= ~(lbl_8047A8B0 | lbl_8047A8B4);
    lbl_8047A8A8 |= mask;
    lbl_8047A8A4 &= ~mask;
    lbl_8047A8AC |= mask;
    fn_800D0464(lbl_8047A8A8);

    if (lbl_80478A0C == 0x20) {
        lbl_80478A0C = __cntlzw(lbl_8047A8A8);
        if (lbl_80478A0C != 0x20) {
            u32 chanBit = 0x80000000u >> lbl_80478A0C;
            lbl_8047A8A8 &= ~chanBit;
            memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
            lbl_803FC5D0[lbl_80478A0C] = 0;
            fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
        }
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

/*
 * fn_800AAF38 = PADInit (unmatched attempt)
 */
BOOL fn_800AAF38(void) {
    if (!lbl_8047A8A0) {
        OSRegisterVersion(lbl_80478A08);

        if (__PADSpec)
            PADSetSpec(__PADSpec);

        lbl_8047A8A0 = 1;

        if (lbl_8047AA58) {
            OSTime time = OSGetTime();
            __OSWirelessPadFixMode = (u16)(fn_800C4C98((u32)(time >> 32), (u32)time, 0x30) & 0x3FFF);
            lbl_8047A8AC = 0xF0000000;
        }

        fn_800D104C();
        OSRegisterResetFunction(NULL);
    }

    return fn_800AAD34(0xF0000000);
}

/*
 * fn_800AB150 = PADRead (unmatched attempt)
 */
BOOL fn_800AB150(PADStatus *status) {
    s32 chan;
    BOOL rumble = FALSE;
    BOOL enabled = OSDisableInterrupts();

    for (chan = 0; chan < 4; chan++, status++) {
        u32 chanBit = 0x80000000u >> chan;

        if (lbl_8047A8B8 & chanBit) {
            /* fn_800AAD34(chanBit)-equivalent inline reset */
            status->err = -2;
            memset(status, 0, 0xA);
            continue;
        }
        if (!(lbl_8047A8A8 & chanBit) && lbl_80478A0C == chan) {
            status->err = -2;
            memset(status, 0, 0xA);
            continue;
        }
        if (!(lbl_8047A8A4 & chanBit)) {
            status->err = -1;
            memset(status, 0, 0xA);
            continue;
        }
        if (!fn_800CF728(chan)) {
            status->err = -3;
            memset(status, 0, 0xA);
            continue;
        }
        if (fn_800D02BC(chan) & 0x8) {
            OSContext ctx;
            fn_800D05A4(chan, &ctx);
            if (lbl_8047A8B0 & chanBit) {
                status->err = 0;
                memset(status, 0, 0xA);
                if (!(lbl_8047A8B4 & chanBit)) {
                    lbl_8047A8B4 |= chanBit;
                    fn_800D0CBC(chan, (SITypeAndStatusCallback)fn_800AAC00);
                }
                continue;
            }
            /* fn_800AAC00-style disable path */
            {
                BOOL en2 = OSDisableInterrupts();
                lbl_8047A8A4 &= ~chanBit;
                lbl_8047A8B0 &= ~chanBit;
                lbl_8047A8B4 &= ~chanBit;
                lbl_8047A8B8 &= ~chanBit;
                OSSetWirelessID(chan, 0);
                OSRestoreInterrupts(en2);
            }
            status->err = -1;
            memset(status, 0, 0xA);
            continue;
        }

        if (SIGetType(chan) & 0x20000000) {
            rumble |= chanBit;
        }

        if (!fn_800D05A4(chan, 0)) {
            status->err = -3;
            memset(status, 0, 0xA);
            continue;
        }

        {
            u32 data[2];
            if (data[0] & 0x80000000u) {
                status->err = -3;
                memset(status, 0, 0xA);
                continue;
            }
            ((void (*)(s32, PADStatus *, u32 *))lbl_80478A1C)(chan, status, data);
            if (status->button & 0x2000) {
                status->err = -3;
                memset(status, 0, 0xA);
                SITransfer(chan, &lbl_80478A20, 1, &lbl_803FC5E0[chan].triggerLeft, 0xA,
                           fn_800AA73C, 0);
            } else {
                status->err = 0;
                status->button &= ~0x80;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return rumble;
}

/*
 * fn_800AB4FC = PADControlAllMotors (unmatched attempt)
 */
void fn_800AB4FC(const u32 *commandArray) {
    BOOL enabled = OSDisableInterrupts();
    s32 chan;
    BOOL commit = FALSE;

    for (chan = 0; chan < 4; chan++, commandArray++) {
        u32 chanBit = 0x80000000u >> chan;
        if (!(lbl_8047A8A4 & chanBit))
            continue;
        if (SIGetType(chan) & 0x20000000)
            continue;

        {
            u32 command = *commandArray;
            if (lbl_80478A18 < 2 && command == 2)
                command = 0;
            fn_800D0338(chan, (lbl_80478A14 | 0x400000) | (command & 3));
            commit = TRUE;
        }
    }

    if (commit)
        fn_800D034C();

    OSRestoreInterrupts(enabled);
}

/*
 * fn_800ABCF4 = PADSetAnalogMode (unmatched attempt)
 */
void fn_800ABCF4(s32 mode) {
    BOOL enabled = OSDisableInterrupts();
    u32 oldEnabled = lbl_8047A8A4;

    lbl_80478A14 = mode << 8;
    lbl_8047A8A4 = 0;
    lbl_8047A8B0 = ~0u & lbl_8047A8B0;
    lbl_8047A8B4 = 0;

    fn_800D0464(oldEnabled);

    OSRestoreInterrupts(enabled);
}

/*
 * fn_800ABD68 (unmatched attempt) - OSResetFunctionInfo callback
 */
BOOL fn_800ABD68(BOOL final) {
    BOOL wasSampling;
    BOOL sync;

    wasSampling = (lbl_8047A8BC != 0);
    if (wasSampling) {
        PADSetSamplingCallback(0);
    }

    if (final == 0) {
        sync = (lbl_8047A8A8 == 0) && (lbl_80478A0C == 0x20);
        if (sync) {
            sync = fn_800CF708();
        }

        if (lbl_8047A8C0 == 0 && sync) {
            BOOL enabled = OSDisableInterrupts();
            u32 mask = 0xF0000000;

            mask |= lbl_8047A8B8;
            lbl_8047A8B8 = 0;
            mask &= ~(lbl_8047A8B0 | lbl_8047A8B4);
            lbl_8047A8A8 |= mask;
            lbl_8047A8A4 &= ~mask;
            lbl_8047A8AC |= mask;
            fn_800D0464(lbl_8047A8A8);

            if (lbl_80478A0C == 0x20) {
                lbl_80478A0C = __cntlzw(lbl_8047A8A8);
                if (lbl_80478A0C != 0x20) {
                    u32 chanBit = 0x80000000u >> lbl_80478A0C;
                    lbl_8047A8A8 &= ~chanBit;
                    memset(&lbl_803FC5E0[lbl_80478A0C], 0, sizeof(PADStatus));
                    lbl_803FC5D0[lbl_80478A0C] = 0;
                    fn_800D0CBC(lbl_80478A0C, (SITypeAndStatusCallback)PADTypeAndStatusCallback);
                }
            }

            OSRestoreInterrupts(enabled);
            lbl_8047A8C0 = 1;
            return FALSE;
        }
        return sync;
    }

    lbl_8047A8C0 = 0;
    return TRUE;
}

/*
 * fn_800ABEFC - internal handler invoked from the SI polling interrupt to
 * dispatch the user PADSamplingCallback with a clean register/context state.
 */
static void fn_800ABEFC(__OSInterrupt interrupt, OSContext *context) {
    OSContext newContext;
    if (lbl_8047A8BC != NULL) {
        OSClearContext(&newContext);
        OSSetCurrentContext(&newContext);
        lbl_8047A8BC();
        OSClearContext(&newContext);
        OSSetCurrentContext(context);
    }
}

/*
 * fn_800ABF5C = PADSetSamplingCallback (byte-matches)
 */
PADSamplingCallback PADSetSamplingCallback(PADSamplingCallback callback) {
    PADSamplingCallback old = lbl_8047A8BC;
    lbl_8047A8BC = callback;
    if (callback != NULL) {
        SIRegisterPollingHandler((__OSInterruptHandler)fn_800ABEFC);
    } else {
        SIUnregisterPollingHandler((__OSInterruptHandler)fn_800ABEFC);
    }
    return old;
}

/*
 * fn_800ABFB0 = __PADDisableRecalibration
 */
BOOL __PADDisableRecalibration(BOOL disable) {
    BOOL old;
    BOOL enabled = OSDisableInterrupts();
    int flags;

    old = (*(volatile u8 *)0x800030E3 & 0x40) ? TRUE : FALSE;
    flags = *(volatile u8 *)0x800030E3;
    flags &= 0xbf;
    *(volatile u8 *)0x800030E3 = flags;
    if (disable) {
        *(volatile u8 *)0x800030E3 = *(volatile u8 *)0x800030E3 | 0x40;
    }

    OSRestoreInterrupts(enabled);
    return old;
}
