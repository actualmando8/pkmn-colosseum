#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSInterrupt.h"

/*
 * OSTime.c - Time base access functions.
 *
 * OSGetTime reads the PowerPC time base register pair (TBU/TBL).
 * OSGetTick reads just the lower 32 bits (TBL).
 * __OSGetSystemTime adds the system time bias stored at 0x800030D8.
 *
 * Matches: 0x800A2778 - 0x800A27F8
 */

#define OS_TIME_BASE_HI (*(volatile u32*)0x800030D8)
#define OS_TIME_BASE_LO (*(volatile u32*)0x800030DC)
#define OS_TIMER_CLOCK  (*(volatile u32*)0x800000F8 >> 2)

typedef struct OSCalendarTime {
    s32 sec;        /* 0x00 */
    s32 min;        /* 0x04 */
    s32 hour;       /* 0x08 */
    s32 mday;       /* 0x0C */
    s32 mon;        /* 0x10 */
    s32 year;       /* 0x14 */
    s32 wday;       /* 0x18 */
    s32 yday;       /* 0x1C */
    s32 msec;       /* 0x20 */
    s32 usec;       /* 0x24 */
} OSCalendarTime;

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm s64 OSGetTime(void) {
    nofralloc
    mftbu r3
    mftb  r4
    mftbu r5
    cmpw  r3, r5
    bne   OSGetTime
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm u32 OSGetTick(void) {
    nofralloc
    mftb r3
    blr
}
#pragma pop

s64 __OSGetSystemTime(void) {
#pragma peephole off
    BOOL enabled;
    s64 time;
    s64 bias;
    s64 result;

    enabled = OSDisableInterrupts();
    time = OSGetTime();
    bias = *(s64*)&OS_TIME_BASE_HI;
    result = bias + time;
    OSRestoreInterrupts(enabled);
    return result;
}

/* ========================================================== */
/* Calendar conversion helpers                               */
/* ========================================================== */

/* fn_800A27FC - 0x800A27FC | size: 0x19C */
void fn_800A27FC(s32 days, OSCalendarTime* td) {
    extern int lbl_80311878[];
    extern int lbl_803118A8[];
    int year;
    int n;
    int month;
    int* md;

    td->wday = (days + 6) % 7;

    for (year = days / 365;
         days < (n = year * 365 + ((year < 1) ? 0 : ((year + 3) / 4 - (year - 1) / 100 + (year - 1) / 400)));
         year--) {
        ;
    }

    days -= n;
    td->year = year;
    td->yday = days;

    md = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? lbl_803118A8 : lbl_80311878;
    for (month = 12; days < md[--month];) {
        ;
    }

    td->mon = month;
    td->mday = days - md[month] + 1;
}

/* fn_800A2998 - 0x800A2998 | size: 0x204 */
void fn_800A2998(s64 ticks, OSCalendarTime* td) {
    s32 seconds;
    s64 tickPart;
    s32 days;

    tickPart = ticks % OS_TIMER_CLOCK;
    if (tickPart < 0) {
        tickPart += OS_TIMER_CLOCK;
    }

    td->usec = ((tickPart * 8) / (OS_TIMER_CLOCK / 125000)) % 1000;
    td->msec = (tickPart / (OS_TIMER_CLOCK / 1000)) % 1000;

    ticks -= tickPart;
    days = (ticks / OS_TIMER_CLOCK) / 86400 + 0xB2575;
    if ((seconds = ((ticks / OS_TIMER_CLOCK) % 86400)) < 0) {
        days--;
        seconds += 86400;
    }

    fn_800A27FC(days, td);

    td->hour = (seconds / 60) / 60;
    td->min = (seconds / 60) % 60;
    td->sec = seconds % 60;
}
