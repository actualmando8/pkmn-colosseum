#include "dolphin/types.h"

#define OS_TIMER_CLOCK (*(volatile u32*)0x800000F8 >> 2)

typedef struct OSCalendarTime {
    s32 sec;
    s32 min;
    s32 hour;
    s32 mday;
    s32 mon;
    s32 year;
    s32 wday;
    s32 yday;
    s32 msec;
    s32 usec;
} OSCalendarTime;

#pragma peephole off
#pragma push
#pragma dont_inline on
void GetDates(s32 days, OSCalendarTime* td) {
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
#pragma pop

void OSTicksToCalendarTime(s64 ticks, OSCalendarTime* td) {
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

    GetDates(days, td);

    td->hour = (seconds / 60) / 60;
    td->min = (seconds / 60) % 60;
    td->sec = seconds % 60;
}
