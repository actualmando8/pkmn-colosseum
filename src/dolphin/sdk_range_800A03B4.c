#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/types.h"

extern BOOL lbl_8047A744;
extern BOOL lbl_8047A748;
/* OSResetSW state owned by the preceding SDK TU: HoldUp and HoldDown. */
extern OSTime lbl_8047A750;
extern OSTime lbl_8047A758;

#define __PIRegs ((volatile u32*)0xCC003000)
#define OS_BUS_CLOCK (*(u32*)0x800000F8)
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)
#define OSMicrosecondsToTicks(usec) (((usec) * (OS_TIMER_CLOCK / 125000)) / 8)
#define OSMillisecondsToTicks(msec) ((msec) * (OS_TIMER_CLOCK / 1000))
#define OSSecondsToTicks(sec) ((OSTime)(sec) * OS_TIMER_CLOCK)
#define OSTicksToSeconds(ticks) ((ticks) / OS_TIMER_CLOCK)

u8 GameChoice : 0x800030E3;

BOOL OSGetResetButtonState(void) {
    BOOL enabled;
    BOOL state;
    u32 reg;
    OSTime now;

    enabled = OSDisableInterrupts();

    now = __OSGetSystemTime();

    reg = __PIRegs[0];
    if (!(reg & 0x00010000)) {
        if (!lbl_8047A744) {
            lbl_8047A744 = TRUE;
            state = lbl_8047A750 ? TRUE : FALSE;
            lbl_8047A758 = now;
        } else {
            state = (lbl_8047A750 ||
                     (OSMicrosecondsToTicks(100) < now - lbl_8047A758))
                        ? TRUE
                        : FALSE;
        }
    } else if (lbl_8047A744) {
        lbl_8047A744 = FALSE;
        state = lbl_8047A748;
        if (state) {
            lbl_8047A750 = now;
        } else {
            lbl_8047A750 = 0;
        }
    } else if (lbl_8047A750 &&
               (now - lbl_8047A750 < OSMillisecondsToTicks(40))) {
        state = TRUE;
    } else {
        state = FALSE;
        lbl_8047A750 = 0;
    }

    lbl_8047A748 = state;

    if (GameChoice & 0x3F) {
        OSTime fire = (GameChoice & 0x3F) * 60;
        fire = __OSStartTime + OSSecondsToTicks(fire);
        if (fire < now) {
            now -= fire;
            now = OSTicksToSeconds(now) / 2;
            if ((now & 1) == 0) {
                state = TRUE;
            } else {
                state = FALSE;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return state;
}
