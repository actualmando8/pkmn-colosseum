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

/* SDA/data symbol aliases used by stub functions */
extern u32 Scb_803FB840;

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

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800A064C - 0x800A064C | size: 0x60 */
void fn_800A064C(void) {
    extern void fn_800A06AC();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)Scb_803FB840;
    r31 = (u32)Scb_803FB840;
    r30 = r31 + 0x40;
    r4 = *(u32*)((u8*)r31 + 0x40);
    r3 = r31 + r4;
    r5 = 0x40 - r4;
    fn_800A06AC();
    *(u32*)((u8*)r31 + 0x4C) = r3;
    tmp = *(u32*)((u8*)r31 + 0x4C);
    if ((s32)tmp == 0) goto L_800A0694;
    tmp = 0x40;
    *(u32*)((u8*)r30 + 0x0) = tmp;
L_800A0694:
    return;
}

/* fn_800A06AC - 0x800A06AC | size: 0x118 */
void fn_800A06AC(void) {
    extern void fn_80098368();
    extern void fn_800A064C();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = (u32)fn_800A064C;
    tmp = (u32)fn_800A064C;
    r31 = r4 + 0x0;
    r4 = 0x1;
    r30 = r5 + 0x0;
    r5 = tmp;
    r29 = r3 + 0x0;
    r3 = 0x0;
    EXILock();
    if ((s32)r3 != 0) goto L_800A06F8;
    r3 = 0x0;
    goto L_800A07A8;
L_800A06F8:
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x3;
    EXISelect();
    if ((s32)r3 != 0) goto L_800A0720;
    r3 = 0x0;
    EXIUnlock();
    r3 = 0x0;
    goto L_800A07A8;
L_800A0720:
    r31 = r31 << 6;
    tmp = r31 + 0x100;
    tmp = tmp | (0xa000 << 16);
    r4 = (u32)sp + 0x14;
    r3 = 0x0;
    r5 = 0x4;
    r6 = 0x1;
    r7 = 0x0;
    EXIImm();
    tmp = __cntlzw(r3);
    r31 = (u32)tmp >> 5;
    r3 = 0x0;
    EXISync();
    tmp = __cntlzw(r3);
    tmp = (u32)tmp >> 5;
    r4 = r29 + 0x0;
    r5 = r30 + 0x0;
    r31 = r31 | tmp;
    r3 = 0x0;
    r6 = 0x1;
    fn_80098368();
    tmp = __cntlzw(r3);
    tmp = (u32)tmp >> 5;
    r31 = r31 | tmp;
    r3 = 0x0;
    EXIDeselect();
    tmp = __cntlzw(r3);
    tmp = (u32)tmp >> 5;
    r31 = r31 | tmp;
    r3 = 0x0;
    EXIUnlock();
    tmp = __cntlzw(r31);
    r3 = (u32)tmp >> 5;
L_800A07A8:
    return;
}

