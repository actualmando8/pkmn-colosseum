#include "dolphin/types.h"

extern u8 gTRKState[];

void TRKTargetSetInputPendingPtr(u8* ptr) {
    *(u32*)&gTRKState[0xA0] = (u32)ptr;
}
