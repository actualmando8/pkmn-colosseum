/**
 * @file OSCache_exact_8009B4D8.c
 * @brief Interrupt-safe locked-cache enable wrapper, 0x8009B4D8 - 0x8009B510.
 */

#include "dolphin/types.h"
#include "dolphin/os/OSInterrupt.h"

extern void __LCEnable(void);

void LCEnable(void)
{
    BOOL enabled;

    enabled = OSDisableInterrupts();
    __LCEnable();
    OSRestoreInterrupts(enabled);
}
