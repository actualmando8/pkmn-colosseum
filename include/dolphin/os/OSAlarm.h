#ifndef DOLPHIN_OS_OSALARM_H
#define DOLPHIN_OS_OSALARM_H

#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSTime.h"

typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(OSAlarm* alarm, OSContext* context);

struct OSAlarm {
    /* 0x00 */ OSAlarmHandler handler;
    /* 0x04 */ u32 tag;
    /* 0x08 */ s64 fire;         /* absolute time to fire */
    /* 0x10 */ OSAlarm* prev;    /* linked list prev */
    /* 0x14 */ OSAlarm* next;    /* linked list next */
    /* 0x18 */ s64 period;       /* repeat period (0 = one-shot) */
    /* 0x20 */ s64 start;        /* start of period */
};

void OSInitAlarm(void);
void OSCreateAlarm(OSAlarm* alarm);
void OSSetAlarm(OSAlarm* alarm, s64 tick, OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm* alarm);

#endif /* DOLPHIN_OS_OSALARM_H */
