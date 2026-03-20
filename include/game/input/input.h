/**
 * @file input.h
 * @brief Game-level controller input wrapper API.
 *
 * The game wraps the Dolphin SDK PAD functions with a higher-level input
 * system that provides:
 *   - Button press detection (newly pressed this frame)
 *   - Button release detection
 *   - Button held detection (held across frames)
 *   - Analog stick normalization with configurable dead zones
 *   - Rumble control integrated with the global gRumbleEnabled flag
 *   - Per-controller state tracking
 *
 * The SDK PADRead function fills a PADStatus[4] array each frame. The
 * game's input wrapper compares the current frame's button state with
 * the previous frame's to derive press/release events.
 *
 * Address context:
 *   PAD wrapper functions are in the 0x800F7500 - 0x800F8268 range.
 *   The lower-level PAD motor/read functions used from main.c are:
 *     fn_800F7758  PADInput_Init (allocates per-pad state)
 *     fn_800F75FC  PADInput_SetMappingTable
 *     fn_800F7AF0  PADInput_ReadButtons (per-pad, updates prev/curr)
 *     fn_800F7BC4  PADInput_ReadHeld (per-pad, returns held mask)
 *     fn_800F7C8C  PADInput_ReadStickX (per-pad, returns normalized X)
 *     fn_800F7D38  PADInput_ReadStickY (per-pad, returns normalized Y)
 *     fn_800F7DE4  PADInput_SetStickMode
 *     fn_800F7E40  PADInput_SetDeadzone
 *     fn_800F7E9C  PADInput_SetRumbleMode
 *     fn_800F7EF8  PADInput_IsConnected
 *     fn_800F7F64  PADInput_ResetRumble
 *     fn_800F80B0  PADInput_InitMotor
 *
 *   Hardware-level PAD functions near the SDK region:
 *     fn_8008ABA0  PADInput_IsMotorAvailable (checks PAD type)
 *     fn_8008AC34  PADInput_Recalibrate (re-zeroes sticks)
 *     fn_8008AE18  PADRead_Internal (calls SDK PADRead)
 *
 * Global state:
 *   lbl_80478DC8 (u8)  gRumbleEnabled -- master rumble on/off
 *   lbl_802E1CF0       PAD mapping table (referenced from GameInit)
 */

#ifndef GAME_INPUT_INPUT_H
#define GAME_INPUT_INPUT_H

#include "dolphin/types.h"

/* ===================================================================
 * Constants
 * =================================================================== */

/* Maximum number of controllers the game supports */
#define PAD_MAX_CONTROLLERS  4

/* Button masks (Dolphin SDK PAD button bit definitions) */
#define PAD_BUTTON_LEFT     0x0001
#define PAD_BUTTON_RIGHT    0x0002
#define PAD_BUTTON_DOWN     0x0004
#define PAD_BUTTON_UP       0x0008
#define PAD_TRIGGER_Z       0x0010
#define PAD_TRIGGER_R       0x0020
#define PAD_TRIGGER_L       0x0040
#define PAD_BUTTON_A        0x0100
#define PAD_BUTTON_B        0x0200
#define PAD_BUTTON_X        0x0400
#define PAD_BUTTON_Y        0x0800
#define PAD_BUTTON_START    0x1000

/* D-pad convenience mask */
#define PAD_DPAD_MASK       (PAD_BUTTON_UP | PAD_BUTTON_DOWN | PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT)

/* Analog stick dead zone default (7 units out of 128) */
#define PAD_DEFAULT_DEADZONE  7

/* Stick modes */
#define PAD_STICK_MODE_NORMAL   0
#define PAD_STICK_MODE_DIGITAL  1  /* treat stick as d-pad */

/* Rumble modes */
#define PAD_RUMBLE_OFF        0
#define PAD_RUMBLE_ON         1
#define PAD_RUMBLE_BRAKE      2

/* ===================================================================
 * Structures
 * =================================================================== */

/**
 * Per-controller input state.
 * Stores current and previous frame button states for edge detection.
 * The game allocates an array of these during PADInput_Init.
 *
 * Based on the function sizes and access patterns:
 *   fn_800F7AF0 (0x6C) reads both buttonsHeld and buttonsPrev
 *   fn_800F7BC4 (0x64) reads buttonsHeld only
 *   fn_800F7C8C (0xAC) reads stickX with deadzone
 *   fn_800F7D38 (0xAC) reads stickY with deadzone
 */
typedef struct PADInputState {
    /* 0x00 */ u16  buttonsHeld;      /* buttons held this frame (raw from PADStatus) */
    /* 0x02 */ u16  buttonsPrev;      /* buttons held last frame */
    /* 0x04 */ u16  buttonsPressed;   /* buttons newly pressed (held & ~prev) */
    /* 0x06 */ u16  buttonsReleased;  /* buttons released (prev & ~held) */
    /* 0x08 */ s8   stickX;           /* main stick X (-128 to 127) */
    /* 0x09 */ s8   stickY;           /* main stick Y (-128 to 127) */
    /* 0x0A */ s8   cStickX;          /* C-stick X */
    /* 0x0B */ s8   cStickY;          /* C-stick Y */
    /* 0x0C */ u8   triggerL;         /* left analog trigger (0-255) */
    /* 0x0D */ u8   triggerR;         /* right analog trigger (0-255) */
    /* 0x0E */ u8   connected;        /* 1 if pad is connected */
    /* 0x0F */ u8   padding;
    /* 0x10 */ u8   stickMode;        /* 0 = normal, 1 = digital */
    /* 0x11 */ u8   deadzone;         /* stick dead zone threshold */
    /* 0x12 */ u8   rumbleMode;       /* rumble motor mode */
    /* 0x13 */ u8   rumbleActive;     /* 1 if rumble currently on */
    /* 0x14 */ u32  rumbleTimer;      /* frames remaining for timed rumble */
    /* 0x18 */ u8   motorInitialized; /* 1 if motor hardware was found */
    /* 0x19 */ u8   padding2[3];
} PADInputState; /* size: ~0x1C */

/* ===================================================================
 * Function declarations -- PAD Input System
 * =================================================================== */

/**
 * fn_800F7758: Initialize the PAD input system.
 * Allocates per-controller state structures. Called during GameInit.
 *
 * @param maxPads  Number of controller slots to allocate
 */
void PADInput_Init(u32 maxPads);

/**
 * fn_800F75FC: Set the controller-to-player mapping table.
 * The mapping table determines which physical controller port maps
 * to which logical player slot.
 *
 * @param table  Pointer to mapping table data
 */
void PADInput_SetMappingTable(void* table);

/**
 * fn_800F7AF0: Read buttons for a controller this frame.
 * Updates the PADInputState: copies current -> prev, reads new state
 * from PADStatus, and computes pressed/released edges.
 *
 * Called from TaskPadRead (fn_80005FFC) for each controller.
 *
 * @param padIdx  Controller index (1-4)
 */
void PADInput_ReadButtons(u32 padIdx);

/**
 * fn_800F7BC4: Get currently held buttons for a controller.
 *
 * @param padIdx  Controller index (1-4)
 * @return        Button mask of currently held buttons
 */
u16 PADInput_GetHeld(u32 padIdx);

/**
 * fn_800F7B5C: Get newly pressed buttons for a controller.
 * Returns buttons that transitioned from not-held to held this frame.
 *
 * @param padIdx  Controller index (1-4)
 * @return        Button mask of newly pressed buttons
 */
u16 PADInput_GetPressed(u32 padIdx);

/**
 * fn_800F7C28: Get released buttons for a controller.
 * Returns buttons that transitioned from held to not-held this frame.
 *
 * @param padIdx  Controller index (1-4)
 * @return        Button mask of released buttons
 */
u16 PADInput_GetReleased(u32 padIdx);

/**
 * fn_800F7C8C: Get normalized main stick X value.
 * Applies dead zone filtering. Returns 0 if within dead zone.
 *
 * @param padIdx  Controller index (1-4)
 * @return        Stick X value (-128 to 127), 0 if in dead zone
 */
s8 PADInput_GetStickX(u32 padIdx);

/**
 * fn_800F7D38: Get normalized main stick Y value.
 *
 * @param padIdx  Controller index (1-4)
 * @return        Stick Y value (-128 to 127), 0 if in dead zone
 */
s8 PADInput_GetStickY(u32 padIdx);

/**
 * fn_800F7DE4: Set stick input mode.
 *
 * @param padIdx  Controller index (1-4)
 * @param mode    0 = normal analog, 1 = digital (threshold-based)
 */
void PADInput_SetStickMode(u32 padIdx, u32 mode);

/**
 * fn_800F7E40: Set stick dead zone.
 *
 * @param padIdx    Controller index (1-4)
 * @param deadzone  Dead zone threshold (default: 7)
 */
void PADInput_SetDeadzone(u32 padIdx, u32 deadzone);

/**
 * fn_800F7E9C: Set rumble motor mode.
 *
 * @param padIdx  Controller index (1-4)
 * @param mode    0 = off, 1 = on, 2 = brake
 */
void PADInput_SetRumbleMode(u32 padIdx, u32 mode);

/**
 * fn_800F7EF8: Check if a controller is connected.
 *
 * @param padIdx  Controller index (1-4)
 * @return        1 if connected, 0 otherwise
 */
BOOL PADInput_IsConnected(u32 padIdx);

/**
 * fn_800F7F64: Reset rumble state for a controller.
 * Called when a controller is disconnected to stop rumble.
 *
 * @param padIdx  Controller index (1-4)
 */
void PADInput_ResetRumble(u32 padIdx);

/**
 * fn_800F80B0: Initialize the rumble motor hardware for a controller.
 * Checks if the motor is available and stores initialization state.
 *
 * @param padIdx  Controller index (1-4)
 */
void PADInput_InitMotor(u32 padIdx);

/**
 * fn_8008ABA0: Check if rumble motor hardware is available.
 * Queries the controller type via SIGetType and checks for motor support.
 *
 * @param padIdx  Controller index (1-4)
 * @return        1 if motor available, 0 otherwise
 */
BOOL PADInput_IsMotorAvailable(u32 padIdx);

/**
 * fn_8008AC34: Recalibrate a controller's analog sticks.
 * Re-zeroes the analog sticks and triggers. Called when a controller
 * is reconnected or when drift is detected.
 *
 * @param padIdx  Controller index (1-4)
 */
void PADInput_Recalibrate(u32 padIdx);

/* ===================================================================
 * Global state accessors
 * =================================================================== */

/**
 * fn_800056E4 (in main.c): Enable or disable rumble globally.
 * Sets gRumbleEnabled (lbl_80478DC8).
 *
 * @param enabled  1 to enable, 0 to disable
 */
extern void SetRumbleEnabled(u8 enabled);

#endif /* GAME_INPUT_INPUT_H */
