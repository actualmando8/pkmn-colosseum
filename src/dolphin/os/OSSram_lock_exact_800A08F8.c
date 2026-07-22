/* Canonical Dolphin SRAM lock entry points. */
#include "dolphin/types.h"

void* __OSLockSram(void)
{
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern u32 Scb_803FB840[0x54 / sizeof(u32)];
    BOOL enabled;
    void* result = Scb_803FB840;

    enabled = OSDisableInterrupts();
    if ((s32) Scb_803FB840[0x12] != 0) {
        OSRestoreInterrupts(enabled);
        result = 0;
    } else {
        Scb_803FB840[0x11] = enabled;
        Scb_803FB840[0x12] = 1;
    }

    return result;
}

void* __OSLockSramEx(void)
{
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern u32 Scb_803FB840[0x54 / sizeof(u32)];
    BOOL enabled;
    u32* sram = Scb_803FB840;
    u32* lock;

    enabled = OSDisableInterrupts();
    lock = &sram[0x12];
    if ((s32) sram[0x12] != 0) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    sram[0x11] = enabled;
    *lock = 1;
    return (u8*) sram + 0x14;
}
