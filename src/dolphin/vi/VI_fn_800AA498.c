#include "dolphin/types.h"

extern u32 OSDisableInterrupts(void);
extern void OSRestoreInterrupts(u32 interrupts);

u32 fn_800AA498(void) {
    u32 interrupts;
    u16 reg;

    interrupts = OSDisableInterrupts();
    reg = *(volatile u16*)0xCC00206E & 3;
    OSRestoreInterrupts(interrupts);
    return reg & 1;
}
