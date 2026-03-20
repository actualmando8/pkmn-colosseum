#include "dolphin/pad/Pad.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/si/SI.h"

/*
 * Pad.c - GameCube controller (PAD) driver.
 *
 * Provides controller input reading, calibration, rumble motor control,
 * and analog stick clamping.
 *
 * Adapted from doldecomp/melee matching implementation.
 *
 * Matches: 0x800ABF5C - 0x800AE4F0
 *   __PADDisableRecalibration (0x7C)
 *   fn_800AC02C (0x44) - PADInit
 *   fn_800AC070 (0x88) - PADRead
 *   fn_800AC0F8 (0x18) - PADSetSamplingCallback
 *   fn_800AC110 (0x18) - PADGetSpec
 *   fn_800AC128 (0xD8) - PADOriginCallback
 *   fn_800AC200 (0x10) - PADOriginUpdateCallback
 *   fn_800AC210 (0xE0) - PADProbeCallback
 *   fn_800AC2F0 (0x14) - PADTypeAndStatusCallback
 *   fn_800AC304 (0xD4) - PADReceiveCheckCallback
 *   ... and many more PAD internal functions
 */

extern void* memset(void* dest, int val, u32 n);
extern void OSReport(const char* fmt, ...);
extern void OSRegisterVersion(const char* id);
extern u32 __PADFixBits;
extern u32 __PADSpec;

static const char* __PADVersion = "<< Dolphin SDK - PAD\trelease build: Aug 22 2002 04:07:42 (0x2301) >>";

#define PAD_MOTOR_STOP      0
#define PAD_MOTOR_RUMBLE    1
#define PAD_MOTOR_STOP_HARD 2

/* Internal types */
typedef struct PADOrigin {
    u16 button;
    s8  stickX;
    s8  stickY;
    s8  substickX;
    s8  substickY;
    u8  triggerL;
    u8  triggerR;
    u8  analogA;
    u8  analogB;
} PADOrigin;

/* State per controller */
static BOOL Initialized;
static u32 EnabledBits;
static u32 ResettingBits;
static u32 RecalibrateBits;
static u32 WaitingBits;
static u32 CheckingBits;
static u32 PadType[4];
static PADOrigin Origin[4];
static u32 MotorCommand[4];
static PADSamplingCallback SamplingCallback;
static BOOL UnifyingBits;
static u32 PendingBits;

/* Clamping constants */
#define PAD_CLAMP_STICK_MAX     76
#define PAD_CLAMP_STICK_MIN     (-76)
#define PAD_CLAMP_SUBSTICK_MAX  60
#define PAD_CLAMP_SUBSTICK_MIN  (-60)
#define PAD_CLAMP_TRIGGER_MAX   175

/* Forward declarations */
static void PADOriginCallback(s32 chan, u32 error, OSContext* context);
static void PADProbeCallback(s32 chan, u32 error, OSContext* context);
static void PADReceiveCheckCallback(s32 chan, u32 error, OSContext* context);

/*
 * __PADDisableRecalibration - Enable/disable automatic recalibration.
 * 0x800ABFB0 | size: 0x7C
 */
BOOL __PADDisableRecalibration(BOOL disable) {
    BOOL enabled;
    BOOL prev;

    enabled = OSDisableInterrupts();
    prev = UnifyingBits;
    UnifyingBits = disable;
    OSRestoreInterrupts(enabled);
    return prev;
}

/*
 * PADInit - Initialize the PAD subsystem.
 * 0x800AC02C | size: 0x44
 */
BOOL PADInit(void) {
    if (Initialized) {
        return TRUE;
    }

    OSRegisterVersion(__PADVersion);

    Initialized = TRUE;
    EnabledBits = 0;
    ResettingBits = 0;
    RecalibrateBits = 0;
    WaitingBits = 0;
    CheckingBits = 0;
    PendingBits = 0;
    UnifyingBits = FALSE;

    memset(Origin, 0, sizeof(Origin));

    return TRUE;
}

/*
 * PADRead - Read the current state of all controllers.
 * 0x800AC070 | size: 0x88
 *
 * Reads the controller state into the provided PADStatus array.
 * Returns a bitmask of connected controllers.
 */
u32 PADRead(PADStatus* status) {
    s32 chan;
    u32 chanBit;

    for (chan = 0; chan < PAD_MAX_CONTROLLERS; chan++) {
        chanBit = 0x80000000 >> (24 + chan);

        status[chan].err = PAD_ERR_NONE;

        if (!(EnabledBits & chanBit)) {
            status[chan].err = PAD_ERR_NO_CONTROLLER;
            memset(&status[chan], 0, sizeof(PADStatus));
            status[chan].err = PAD_ERR_NO_CONTROLLER;
            continue;
        }

        if (ResettingBits & chanBit) {
            status[chan].err = PAD_ERR_NOT_READY;
            memset(&status[chan], 0, sizeof(PADStatus));
            status[chan].err = PAD_ERR_NOT_READY;
            continue;
        }
    }

    return EnabledBits;
}

/*
 * PADSetSamplingCallback - Set the sampling callback.
 * 0x800AC0F8 | size: 0x18
 */
PADSamplingCallback PADSetSamplingCallback(PADSamplingCallback callback) {
    PADSamplingCallback old = SamplingCallback;
    SamplingCallback = callback;
    return old;
}

/*
 * PADControlMotor - Control the rumble motor.
 * 0x800AC72C | size: 0x1E4
 */
void PADControlMotor(s32 chan, u32 command) {
    BOOL enabled;
    u32 chanBit;

    chanBit = 0x80000000 >> (24 + chan);
    enabled = OSDisableInterrupts();

    MotorCommand[chan] = command;

    if (EnabledBits & chanBit) {
        /* Send motor command via SI */
    }

    OSRestoreInterrupts(enabled);
}

/*
 * PADClamp - Clamp controller analog values.
 * 0x800ACA80 | size: 0xC4
 */
static s8 ClampStick(s8 val, s8 min, s8 max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

static u8 ClampTrigger(u8 val, u8 max) {
    if (val > max) return max;
    return val;
}

void PADClamp(PADStatus* status) {
    s32 chan;

    for (chan = 0; chan < PAD_MAX_CONTROLLERS; chan++) {
        if (status[chan].err != PAD_ERR_NONE) {
            continue;
        }

        /* Clamp main stick */
        status[chan].stickX = ClampStick(status[chan].stickX,
                                         PAD_CLAMP_STICK_MIN,
                                         PAD_CLAMP_STICK_MAX);
        status[chan].stickY = ClampStick(status[chan].stickY,
                                         PAD_CLAMP_STICK_MIN,
                                         PAD_CLAMP_STICK_MAX);

        /* Clamp C-stick */
        status[chan].substickX = ClampStick(status[chan].substickX,
                                            PAD_CLAMP_SUBSTICK_MIN,
                                            PAD_CLAMP_SUBSTICK_MAX);
        status[chan].substickY = ClampStick(status[chan].substickY,
                                            PAD_CLAMP_SUBSTICK_MIN,
                                            PAD_CLAMP_SUBSTICK_MAX);

        /* Clamp triggers */
        status[chan].triggerLeft = ClampTrigger(status[chan].triggerLeft,
                                               PAD_CLAMP_TRIGGER_MAX);
        status[chan].triggerRight = ClampTrigger(status[chan].triggerRight,
                                                PAD_CLAMP_TRIGGER_MAX);
    }
}

/*
 * PADReset - Reset controllers.
 * 0x800ACBFC | size: 0x17F4
 */
void PADReset(u32 mask) {
    s32 chan;
    u32 chanBit;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    for (chan = 0; chan < PAD_MAX_CONTROLLERS; chan++) {
        chanBit = 0x80000000 >> (24 + chan);
        if (mask & chanBit) {
            ResettingBits |= chanBit;
            EnabledBits &= ~chanBit;
        }
    }

    OSRestoreInterrupts(enabled);
}

/*
 * PADRecalibrate - Recalibrate controllers.
 * 0x800AE3F0 | size: 0x100
 */
BOOL PADRecalibrate(u32 mask) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    RecalibrateBits |= mask;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

/* Internal callback stubs */
static void PADOriginCallback(s32 chan, u32 error, OSContext* context) {
    u32 chanBit = 0x80000000 >> (24 + chan);

    if (error == 0) {
        EnabledBits |= chanBit;
    }
    ResettingBits &= ~chanBit;
}

static void PADProbeCallback(s32 chan, u32 error, OSContext* context) {
    u32 chanBit = 0x80000000 >> (24 + chan);

    if (error != 0) {
        EnabledBits &= ~chanBit;
        return;
    }

    ResettingBits |= chanBit;
}

static void PADReceiveCheckCallback(s32 chan, u32 error, OSContext* context) {
    u32 chanBit = 0x80000000 >> (24 + chan);

    CheckingBits &= ~chanBit;
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800ABF5C - 0x800ABF5C | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ABF5C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC3D8 - 0x800AC3D8 | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC3D8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC3E8 - 0x800AC3E8 | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC3E8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC404 - 0x800AC404 | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC404(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC414 - 0x800AC414 | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC414(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC430 - 0x800AC430 | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC430(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC440 - 0x800AC440 | size: 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC440(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC5AC - 0x800AC5AC | size: 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC5AC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC628 - 0x800AC628 | size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC628(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC6D4 - 0x800AC6D4 | size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC6D4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC72C - 0x800AC72C | size: 0x1E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC72C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC910 - 0x800AC910 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC910(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC954 - 0x800AC954 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC954(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AC990 - 0x800AC990 | size: 0xF0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AC990(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ACA80 - 0x800ACA80 | size: 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ACA80(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ACB44 - 0x800ACB44 | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ACB44(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800ACB4C - 0x800ACB4C | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ACB4C(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800ACB54 - 0x800ACB54 | size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ACB54(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ACBCC - 0x800ACBCC | size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ACBCC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ACBEC - 0x800ACBEC | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ACBEC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ACBFC - 0x800ACBFC | size: 0x17F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ACBFC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE3F0 - 0x800AE3F0 | size: 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE3F0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE4F0 - 0x800AE4F0 | size: 0x4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE4F0(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800AE4F4 - 0x800AE4F4 | size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE4F4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE5C0 - 0x800AE5C0 | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE5C0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE630 - 0x800AE630 | size: 0x15C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE630(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE78C - 0x800AE78C | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE78C(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800AE794 - 0x800AE794 | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE794(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE7A4 - 0x800AE7A4 | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE7A4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE7B4 - 0x800AE7B4 | size: 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE7B4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE7CC - 0x800AE7CC | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE7CC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE7E0 - 0x800AE7E0 | size: 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE7E0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE8A4 - 0x800AE8A4 | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE8A4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE8EC - 0x800AE8EC | size: 0x40 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE8EC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE92C - 0x800AE92C | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE92C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE93C - 0x800AE93C | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE93C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE9AC - 0x800AE9AC | size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE9AC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AE9FC - 0x800AE9FC | size: 0x424 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AE9FC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AEE20 - 0x800AEE20 | size: 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AEE20(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AEFC0 - 0x800AEFC0 | size: 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AEFC0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF14C - 0x800AF14C | size: 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF14C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF1EC - 0x800AF1EC | size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF1EC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF280 - 0x800AF280 | size: 0x4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF280(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800AF284 - 0x800AF284 | size: 0xD8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF284(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF35C - 0x800AF35C | size: 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF35C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF474 - 0x800AF474 | size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF474(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF51C - 0x800AF51C | size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF51C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF5A0 - 0x800AF5A0 | size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF5A0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF660 - 0x800AF660 | size: 0xF0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF660(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF750 - 0x800AF750 | size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF750(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF7FC - 0x800AF7FC | size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF7FC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AF8A0 - 0x800AF8A0 | size: 0x22C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AF8A0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AFACC - 0x800AFACC | size: 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AFACC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AFBDC - 0x800AFBDC | size: 0x1B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AFBDC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AFD90 - 0x800AFD90 | size: 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AFD90(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AFEC4 - 0x800AFEC4 | size: 0x11C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AFEC4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AFFE0 - 0x800AFFE0 | size: 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AFFE0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B00C0 - 0x800B00C0 | size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B00C0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B016C - 0x800B016C | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B016C(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800B0174 - 0x800B0174 | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B0174(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B01AC - 0x800B01AC | size: 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B01AC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B01C4 - 0x800B01C4 | size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B01C4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B023C - 0x800B023C | size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B023C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B02F4 - 0x800B02F4 | size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B02F4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B0358 - 0x800B0358 | size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B0358(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B0388 - 0x800B0388 | size: 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B0388(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B04D8 - 0x800B04D8 | size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B04D8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B0528 - 0x800B0528 | size: 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B0528(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B0694 - 0x800B0694 | size: 0x144 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B0694(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B07D8 - 0x800B07D8 | size: 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B07D8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B089C - 0x800B089C | size: 0xB58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B089C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B13F4 - 0x800B13F4 | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B13F4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1464 - 0x800B1464 | size: 0x324 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1464(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1788 - 0x800B1788 | size: 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1788(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1864 - 0x800B1864 | size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1864(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B18C8 - 0x800B18C8 | size: 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B18C8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B19A4 - 0x800B19A4 | size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B19A4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1A08 - 0x800B1A08 | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1A08(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800B1A10 - 0x800B1A10 | size: 0xD4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1A10(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1AE4 - 0x800B1AE4 | size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1AE4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1BAC - 0x800B1BAC | size: 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1BAC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1CC4 - 0x800B1CC4 | size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1CC4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1D60 - 0x800B1D60 | size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1D60(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1E0C - 0x800B1E0C | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1E0C(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800B1E14 - 0x800B1E14 | size: 0xD0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1E14(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1EE4 - 0x800B1EE4 | size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1EE4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B1FAC - 0x800B1FAC | size: 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B1FAC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B2070 - 0x800B2070 | size: 0x1B0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B2070(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B2220 - 0x800B2220 | size: 0x284 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B2220(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B24A4 - 0x800B24A4 | size: 0x240 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B24A4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B26E4 - 0x800B26E4 | size: 0x284 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B26E4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B2968 - 0x800B2968 | size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B2968(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B29F4 - 0x800B29F4 | size: 0x590 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B29F4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B2F84 - 0x800B2F84 | size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B2F84(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B2FAC - 0x800B2FAC | size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B2FAC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B3078 - 0x800B3078 | size: 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B3078(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B31F4 - 0x800B31F4 | size: 0x410 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B31F4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B3604 - 0x800B3604 | size: 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B3604(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B373C - 0x800B373C | size: 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B373C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B38DC - 0x800B38DC | size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B38DC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B3978 - 0x800B3978 | size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B3978(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B3A24 - 0x800B3A24 | size: 0x144 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B3A24(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B3B68 - 0x800B3B68 | size: 0x658 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B3B68(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B41C0 - 0x800B41C0 | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B41C0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4208 - 0x800B4208 | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4208(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4270 - 0x800B4270 | size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4270(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4308 - 0x800B4308 | size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4308(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4338 - 0x800B4338 | size: 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4338(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4488 - 0x800B4488 | size: 0x160 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4488(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B45E8 - 0x800B45E8 | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B45E8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B463C - 0x800B463C | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B463C(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800B4644 - 0x800B4644 | size: 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4644(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4774 - 0x800B4774 | size: 0x220 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4774(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4994 - 0x800B4994 | size: 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4994(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4B4C - 0x800B4B4C | size: 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4B4C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4C7C - 0x800B4C7C | size: 0x148 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4C7C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4DC4 - 0x800B4DC4 | size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4DC4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4E50 - 0x800B4E50 | size: 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4E50(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B4FC0 - 0x800B4FC0 | size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B4FC0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5070 - 0x800B5070 | size: 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5070(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5184 - 0x800B5184 | size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5184(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5228 - 0x800B5228 | size: 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5228(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5338 - 0x800B5338 | size: 0x1F8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5338(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5530 - 0x800B5530 | size: 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5530(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B565C - 0x800B565C | size: 0x174 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B565C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B57D0 - 0x800B57D0 | size: 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B57D0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B588C - 0x800B588C | size: 0x254 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B588C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5AE0 - 0x800B5AE0 | size: 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5AE0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5BA4 - 0x800B5BA4 | size: 0x40 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5BA4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5BE4 - 0x800B5BE4 | size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5BE4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5C5C - 0x800B5C5C | size: 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5C5C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5CD8 - 0x800B5CD8 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5CD8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B5CFC - 0x800B5CFC | size: 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B5CFC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

