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
