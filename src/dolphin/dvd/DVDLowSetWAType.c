#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"

u32 WorkAroundSeekLocation_8047A7A8;
u32 WorkAroundType_8047A7A4;
u32 lbl_8047A7A0;

void __DVDLowSetWAType(u32 type, u32 location) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    WorkAroundType_8047A7A4 = type;
    WorkAroundSeekLocation_8047A7A8 = location;
    OSRestoreInterrupts(enabled);
}
