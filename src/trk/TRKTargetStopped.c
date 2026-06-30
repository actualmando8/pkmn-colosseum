#include "dolphin/types.h"

extern u8 gTRKState[];

void TRKTargetSetStopped(s32 stopped) {
    *(s32*)&gTRKState[0x98] = stopped;
}

s32 TRKTargetStopped(void) {
    return *(s32*)&gTRKState[0x98];
}
