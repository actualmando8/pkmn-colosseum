#ifndef DOLPHIN_OS_OSTIME_H
#define DOLPHIN_OS_OSTIME_H

#include "dolphin/types.h"

s64 OSGetTime(void);
u32 OSGetTick(void);
s64 __OSGetSystemTime(void);

#endif /* DOLPHIN_OS_OSTIME_H */
