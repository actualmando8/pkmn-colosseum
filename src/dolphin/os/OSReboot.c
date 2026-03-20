#include "dolphin/os/OS.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/PPCArch.h"
#include "dolphin/os/OSReset.h"

/*
 * OSReboot.c - System reboot and application reload.
 *
 * Contains __OSReboot which handles warm-restarting the system,
 * copying the new DOL to memory and jumping to it.
 *
 * Matches: 0x800A03B4 - 0x800A064C (first function in the gap)
 *   fn_800A03B4 (0x298) - __OSReboot
 */

extern void DCFlushRange(void* addr, u32 size);
extern void ICInvalidateRange(void* addr, u32 size);
extern void __OSStopAudioSystem(void);
extern void LCDisable(void);
extern void* memcpy(void* dest, const void* src, u32 n);

/*
 * __OSReboot - Perform a full system reboot.
 * 0x800A03B4 | size: 0x298
 *
 * Shuts down subsystems, copies the boot program to its run address,
 * flushes caches, and jumps to the entry point.
 */
void __OSReboot(u32 resetCode, u32 bootDol) {
    volatile u32* piReg = (volatile u32*)0xCC003000;

    OSDisableInterrupts();

    /* Stop audio */
    __OSStopAudioSystem();

    /* Disable locked cache */
    LCDisable();

    /* Write reset parameters */
    *(volatile u32*)0x800030E0 = resetCode;
    *(volatile u32*)0x800030E4 = bootDol;

    /* Flush the parameter area */
    DCFlushRange((void*)0x800030E0, 0x20);

    /* Copy the apploader/BS2 back */
    {
        u32 bootInfoAddr = *(volatile u32*)0x800000F4;
        if (bootInfoAddr != 0) {
            /* The boot info area contains the reset vector */
        }
    }

    /* Flush all of low memory */
    DCFlushRange((void*)0x80000000, 0x4000);
    ICInvalidateRange((void*)0x80000000, 0x4000);

    /* Trigger hardware reset via PI register */
    piReg[9] = (resetCode << 3) | 0x3;
}
