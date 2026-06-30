#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"

extern u32 WorkAroundType;
extern u32 WorkAroundSeekLocation;

void __DVDLowSetWAType(u32 type, u32 location) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    WorkAroundType = type;
    WorkAroundSeekLocation = location;
    OSRestoreInterrupts(enabled);
}
