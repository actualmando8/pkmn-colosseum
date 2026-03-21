#include "dolphin/os/OSMemory.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSReset.h"

/*
 * OSMemory.c - Memory protection and BAT configuration.
 *
 * Sets up the memory protection registers, BAT registers for 24MB/48MB
 * memory configurations, and the memory protection interrupt handler.
 *
 * Matches: 0x8009F1B8 - 0x8009F77C
 */

/* Hardware registers */
#define MI_BASE     ((volatile u16*)0xCC004000)
#define MI_MARR_HI  (*(volatile u16*)0xCC00401E)
#define MI_MARR_LO  (*(volatile u16*)0xCC004020)
#define MI_MARR_CTL (*(volatile u16*)0xCC004022)
#define MI_PROT     (*(volatile u16*)0xCC004010)
#define MI_INTMSK   (*(volatile u16*)0xCC004028)

/* Error table for memory protection */
extern OSErrorHandler __OSErrorTable[];

/* Reset function info for memory protection */
extern OSResetFunctionInfo ResetFunctionInfo;

static void MEMIntrruptHandler(s16 interrupt, OSContext* context);
static void Config24MB(void);
static void Config48MB(void);
static void RealMode(void* target);

void __OSModuleInit(void) {
    volatile u32* bootInfo = (volatile u32*)0x80000000;
    bootInfo[0x30CC / 4] = 0;
    bootInfo[0x30C8 / 4] = 0;
    bootInfo[0x30D0 / 4] = 0;
}

static void MEMIntrruptHandler(s16 interrupt, OSContext* context) {
    volatile u16* mi = (volatile u16*)0xCC004000;
    u32 cause;
    u16 hi, lo;

    hi  = mi[0x24 / 2];
    lo  = mi[0x22 / 2];

    /* Combine address */
    cause = ((u32)hi << 16) | lo;

    /* Clear the interrupt */
    mi[0x20 / 2] = 0;

    if (__OSErrorTable[OS_ERROR_PROTECTION] != NULL) {
        __OSErrorTable[OS_ERROR_PROTECTION](OS_ERROR_PROTECTION, context, cause, 0);
    } else {
        __OSUnhandledException(OS_ERROR_PROTECTION, context, 0, 0);
    }
}

/* Config24MB - sets BAT registers for 24MB physical memory layout */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
static asm void Config24MB(void) {
    nofralloc
    li      r7, 0

    /* DBAT0: 0x80000000, 16MB */
    lis     r4, 0x0000
    addi    r4, r4, 0x0002
    lis     r3, 0x8000
    addi    r3, r3, 0x01FF
    /* Pre-load DBAT2/IBAT2 values */
    lis     r6, 0x0100
    addi    r6, r6, 0x0002
    lis     r5, 0x8100
    addi    r5, r5, 0x00FF
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3

    /* DBAT2: 0x81000000, 8MB */
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5

    /* Return via rfi to re-enable address translation */
    isync
    mfmsr   r3
    ori     r3, r3, 0x0030
    mtsrr1  r3
    mflr    r3
    mtsrr0  r3
    rfi
}
#pragma pop

/* Config48MB - sets BAT registers for 48MB physical memory layout */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
static asm void Config48MB(void) {
    nofralloc
    li      r7, 0

    /* DBAT0: 0x80000000, 32MB */
    lis     r4, 0x0000
    addi    r4, r4, 0x0002
    lis     r3, 0x8000
    addi    r3, r3, 0x03FF
    /* Pre-load DBAT2/IBAT2 values */
    lis     r6, 0x0200
    addi    r6, r6, 0x0002
    lis     r5, 0x8200
    addi    r5, r5, 0x01FF
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3

    /* DBAT2: 0x82000000, 16MB */
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5

    /* Return via rfi */
    isync
    mfmsr   r3
    ori     r3, r3, 0x0030
    mtsrr1  r3
    mflr    r3
    mtsrr0  r3
    rfi
}
#pragma pop

/* RealMode - enter real mode (disable address translation) then jump to target */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
static asm void RealMode(register void* target) {
    nofralloc
    clrlwi  r3, r3, 2         /* mask to physical address */
    mtsrr0  r3
    mfmsr   r3
    rlwinm  r3, r3, 0, 28, 25 /* clear IR, DR bits */
    mtsrr1  r3
    rfi
}
#pragma pop

void __OSInitMemoryProtection(void) {
    BOOL enabled;
    u32  memSize;
    volatile u16* mi = (volatile u16*)0xCC004000;

    memSize = *(volatile u32*)0x800000F0;

    enabled = OSDisableInterrupts();

    /* Clear protection registers */
    mi[0x20 / 2] = 0;
    mi[0x10 / 2] = 0x00FF;

    /* Mask all memory protection interrupts initially */
    __OSMaskInterrupts(0xF0000000);

    /* Install memory interrupt handler for all 5 channels */
    __OSSetInterruptHandler(0, (__OSInterruptHandler)MEMIntrruptHandler);
    __OSSetInterruptHandler(1, (__OSInterruptHandler)MEMIntrruptHandler);
    __OSSetInterruptHandler(2, (__OSInterruptHandler)MEMIntrruptHandler);
    __OSSetInterruptHandler(3, (__OSInterruptHandler)MEMIntrruptHandler);
    __OSSetInterruptHandler(4, (__OSInterruptHandler)MEMIntrruptHandler);

    /* Register reset function */
    OSRegisterResetFunction(&ResetFunctionInfo);

    /* Check for extended memory and configure BATs */
    {
        u32 physMemSize = *(volatile u32*)0x800000F0;
        u32 memSizeField = *(volatile u32*)0x80000028;

        if (physMemSize < memSizeField) {
            /* Check for 24MB expansion */
            if (physMemSize - 0x01800000 == 0) {
                DCInvalidateRange((void*)0x81800000, 0x01800000);
                mi[0x28 / 2] = 2;
            }
        }
    }

    /* Set BAT registers based on physical memory size */
    if (memSize <= 0x01800000) {
        /* 24 MB or less */
        RealMode((void*)Config24MB);
    } else if (memSize <= 0x03000000) {
        /* 48 MB or less */
        RealMode((void*)Config48MB);
    }

    /* Unmask the memory protection interrupt */
    __OSUnmaskInterrupts(0x00000800);

    OSRestoreInterrupts(enabled);
}

/* ===================================================================
 * Stub functions for coverage -- TODO: decompile
 * 4 function(s)
 * =================================================================== */

/* fn_8009F230 - 0x8009F230 | size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8009F230(void) {
    extern void fn_800A238C();
    extern void fn_800A2478();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r5 + 0x0;
    r29 = r4 + 0x0;
    r28 = r3 + 0x0;
    OSDisableInterrupts();
    r30 = r3 + 0x0;
    r31 = r31 & 0x1;
    goto L_8009F288;
L_8009F268: ;
    if ((s32)r31 != (s32)0x0) goto L_8009F280;
    r3 = r30;
    OSRestoreInterrupts();
    r3 = 0x0;
    goto L_8009F2D8;
L_8009F280: ;
    r3 = r28;
    fn_800A238C();
L_8009F288: ;
    r6 = *(u32*)((u8*)r28 + 0x14);
    r4 = *(u32*)((u8*)r28 + 0x1C);
    if ((s32)r6 <= (s32)r4) goto L_8009F268;
    r0 = *(u32*)((u8*)r28 + 0x18);
    r3 = r28 + 0x8;
    r5 = *(u32*)((u8*)r28 + 0x10);
    r4 = r0 + r4;
    r0 = (s32)r4 / (s32)r6;
    r0 = r0 * r6;
    r0 = r4 - r0;
    r0 = r0 << 2;
    *(u32*)(r5 + r0) = r29;
    r4 = *(u32*)((u8*)r28 + 0x1C);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r28 + 0x1C) = r0;
    fn_800A2478();
    r3 = r30;
    OSRestoreInterrupts();
    r3 = 0x1;
L_8009F2D8: ;
    return;
}
#pragma pop

/* fn_8009F2F8 - 0x8009F2F8 | size: 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8009F2F8(void) {
    extern void fn_800A238C();
    extern void fn_800A2478();
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3 + 0x0;
    r30 = r5 + 0x0;
    r28 = r4 + 0x0;
    OSDisableInterrupts();
    r29 = r3 + 0x0;
    r30 = r30 & 0x1;
    goto L_8009F350;
L_8009F330: ;
    if ((s32)r30 != (s32)0x0) goto L_8009F348;
    r3 = r29;
    OSRestoreInterrupts();
    r3 = 0x0;
    goto L_8009F3B4;
L_8009F348: ;
    r3 = r31 + 0x8;
    fn_800A238C();
L_8009F350: ;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    if ((s32)r0 == (s32)0x0) goto L_8009F330;
    if ((u32)r28 == (u32)0x0) goto L_8009F378;
    r0 = *(u32*)((u8*)r31 + 0x18);
    r3 = *(u32*)((u8*)r31 + 0x10);
    r0 = r0 << 2;
    r0 = *(u32*)(r3 + r0);
    *(u32*)((u8*)r28 + 0x0) = r0;
L_8009F378: ;
    r5 = *(u32*)((u8*)r31 + 0x18);
    r3 = r31;
    r4 = *(u32*)((u8*)r31 + 0x14);
    r5 = r5 + 0x1;
    r0 = (s32)r5 / (s32)r4;
    r0 = r0 * r4;
    r0 = r5 - r0;
    *(u32*)((u8*)r31 + 0x18) = r0;
    r4 = *(u32*)((u8*)r31 + 0x1C);
    /* subi r0, r4, 0x1 */;
    *(u32*)((u8*)r31 + 0x1C) = r0;
    fn_800A2478();
    r3 = r29;
    OSRestoreInterrupts();
    r3 = 0x1;
L_8009F3B4: ;
    return;
}
#pragma pop

/* fn_8009F3D4 - 0x8009F3D4 | size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8009F3D4(void) {
    u32 r3 = 0;

    r3 = (0x8000 << 16);
    r3 = *(u32*)((u8*)r3 + 0x28);
    return;
}
#pragma pop

/* fn_8009F3E0 - 0x8009F3E0 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8009F3E0(void) {
    u32 r0 = 0;
    u32 r1 = 0;
    u32 r3 = 0;
    f32 f0 = 0.0f;

    if ((s32)r3 == (s32)0x0) goto L_8009F408;
    r3 = (0xcc00 << 16);
    r0 = 0xff;
    *(u16*)((u8*)r3 + 0x4010) = r0;
    r3 = (0xf000 << 16);
    __OSMaskInterrupts();
L_8009F408: ;
    r3 = 0x1;
    return;
}
#pragma pop

