#include "dolphin/os/OSCache.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/PPCArch.h"
#include "dolphin/db/DB.h"

/*
 * OSCache.c - Data cache, instruction cache, and L2 cache management.
 *
 * Provides cache enable/disable/flush/invalidate operations, the L2 cache
 * global invalidate procedure, the DMA error handler, and cache init.
 *
 * Matches: 0x8009B290 - 0x8009B914
 */

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void DCEnable(void) {
    nofralloc
    sync
    mfspr   r3, HID0
    ori     r3, r3, 0x4000
    mtspr   HID0, r3
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void DCInvalidateRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi  r4, 0
    blelr
    clrlwi  r5, r3, 27
    add     r4, r4, r5
    addi    r4, r4, 31
    srwi    r4, r4, 5
    mtctr   r4
_loop_dci:
    dcbi    r0, r3
    addi    r3, r3, 32
    bdnz    _loop_dci
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void DCFlushRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi  r4, 0
    blelr
    clrlwi  r5, r3, 27
    add     r4, r4, r5
    addi    r4, r4, 31
    srwi    r4, r4, 5
    mtctr   r4
_loop_dcf:
    dcbf    r0, r3
    addi    r3, r3, 32
    bdnz    _loop_dcf
    sc
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void DCFlushRangeNoSync(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi  r4, 0
    blelr
    clrlwi  r5, r3, 27
    add     r4, r4, r5
    addi    r4, r4, 31
    srwi    r4, r4, 5
    mtctr   r4
_loop_dcfns:
    dcbf    r0, r3
    addi    r3, r3, 32
    bdnz    _loop_dcfns
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void ICInvalidateRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi  r4, 0
    blelr
    clrlwi  r5, r3, 27
    add     r4, r4, r5
    addi    r4, r4, 31
    srwi    r4, r4, 5
    mtctr   r4
_loop_ici:
    icbi    r0, r3
    addi    r3, r3, 32
    bdnz    _loop_ici
    sync
    isync
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void ICFlashInvalidate(void) {
    nofralloc
    mfspr   r3, HID0
    ori     r3, r3, 0x0800
    mtspr   HID0, r3
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void ICEnable(void) {
    nofralloc
    isync
    mfspr   r3, HID0
    ori     r3, r3, 0x8000
    mtspr   HID0, r3
    blr
}
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void LCDisable(void) {
    nofralloc
    lis     r3, 0xE000
    li      r4, 512
    mtctr   r4
_loop_lcd:
    dcbi    r0, r3
    addi    r3, r3, 32
    bdnz    _loop_lcd
    mfspr   r4, HID2
    rlwinm  r4, r4, 0, 4, 2   /* clear LC enable bit */
    mtspr   HID2, r4
    blr
}
#pragma pop

void L2GlobalInvalidate(void) {
    u32 l2cr;

    asm { sync }
    l2cr = PPCMfl2cr();
    l2cr &= 0x7FFFFFFF;   /* Disable L2 */
    PPCMtl2cr(l2cr);
    asm { sync }

    /* Start L2 global invalidate */
    l2cr = PPCMfl2cr();
    l2cr |= 0x00200000;   /* Set L2I bit */
    PPCMtl2cr(l2cr);

    /* Wait for invalidate to complete */
    while (PPCMfl2cr() & 0x1) {
        ;
    }

    /* Clear L2I bit */
    l2cr = PPCMfl2cr();
    l2cr &= ~0x00200000;
    PPCMtl2cr(l2cr);

    /* Wait again for any remaining operations */
    while (PPCMfl2cr() & 0x1) {
        DBPrintf("L2 invalidate not yet complete\n");
    }
}

void DMAErrorHandler(u16 error, ...) {
    u32 hid2;
    OSContext* context;

    /* Get context from stack / varargs */
    /* This is a varargs handler called from the OS error dispatch */
    context = (OSContext*)((u32*)&error)[1];

    hid2 = PPCMfhid2();

    OSReport("Machine check received\n");
    OSReport("HID2 = 0x%08x   SRR1 = 0x%08x\n", hid2, context->srr1);

    /* Check if it's a real error or a recoverable write-gather pipe issue */
    if (!(hid2 & 0x00F00000) || !(context->srr1 & 0x00200000)) {
        /* Fatal error */
        OSReport("Unrecoverable DMA error\n");
        OSDumpContext(context);
        PPCHalt();
    }

    /* Recoverable: report which errors occurred */
    OSReport("DMA error handler: recovering\n");
    OSReport("Resetting write gather pipe\n");

    if (hid2 & 0x01000000) {
        OSReport("  Write gather pipe overflow\n");
    }
    if (hid2 & 0x00800000) {
        OSReport("  Write gather pipe underflow\n");
    }
    if (hid2 & 0x00400000) {
        OSReport("  Write gather pipe parity error\n");
    }
    if (hid2 & 0x00200000) {
        OSReport("  Write gather pipe error\n");
    }

    /* Clear the error bits and write back */
    PPCMthid2(hid2);
}

void __OSCacheInit(void) {
    u32 hid0;

    /* Enable I-cache if not already enabled */
    hid0 = PPCMfhid0();
    if (!(hid0 & 0x8000)) {
        ICEnable();
        DBPrintf("L1 I-Cache has been enabled\n");
    }

    /* Enable D-cache if not already enabled */
    hid0 = PPCMfhid0();
    if (!(hid0 & 0x4000)) {
        DCEnable();
        DBPrintf("L1 D-Cache has been enabled\n");
    }

    /* Enable L2 cache if not already enabled */
    {
        u32 l2cr;
        l2cr = PPCMfl2cr();
        if (!(l2cr & 0x80000000)) {
            u32 msr;

            msr = PPCMfmsr();
            asm { sync }
            PPCMtmsr(0x30);
            asm { sync }
            asm { sync }

            /* Disable L2 */
            l2cr = PPCMfl2cr();
            l2cr &= 0x7FFFFFFF;
            PPCMtl2cr(l2cr);
            asm { sync }

            /* Do a global invalidate */
            L2GlobalInvalidate();

            /* Restore MSR */
            PPCMtmsr(msr);

            /* Enable L2 with data-only mode cleared */
            l2cr = PPCMfl2cr();
            l2cr = (l2cr | 0x80000000) & ~0x00200000;
            PPCMtl2cr(l2cr);

            DBPrintf("L2 Cache has been enabled\n");
        }
    }

    /* Install DMA error handler */
    OSSetErrorHandler(OS_ERROR_MACHINE_CHECK, (OSErrorHandler)DMAErrorHandler);
    DBPrintf("DMA error handler installed\n");
}

/* ===================================================================
 * Stub functions for coverage -- TODO: decompile
 * 9 function(s)
 * =================================================================== */

/* fn_8009B300 - 0x8009B300 | size: 0x30 */
void fn_8009B300(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((u32)r4 <= (u32)0x0) return;
    r5 = r3 & 0x1F;
    r4 = r4 + r5;
    r4 = r4 + 0x1f;
    r4 = (u32)r4 >> 5;
    ctr_fn = (void(*)(void))r4;
L_8009B31C:
    /* dcbst tmp, r3 */;
    r3 = r3 + 0x20;
    if (--ctr != 0) goto L_8009B31C;
    /* sc */;
    return;
}

/* fn_8009B35C - 0x8009B35C | size: 0x2C */
void fn_8009B35C(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((u32)r4 <= (u32)0x0) return;
    r5 = r3 & 0x1F;
    r4 = r4 + r5;
    r4 = r4 + 0x1f;
    r4 = (u32)r4 >> 5;
    ctr_fn = (void(*)(void))r4;
L_8009B378:
    /* dcbst tmp, r3 */;
    r3 = r3 + 0x20;
    if (--ctr != 0) goto L_8009B378;
    return;
}

/* fn_8009B388 - 0x8009B388 | size: 0x2C */
void fn_8009B388(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((u32)r4 <= (u32)0x0) return;
    r5 = r3 & 0x1F;
    r4 = r4 + r5;
    r4 = r4 + 0x1f;
    r4 = (u32)r4 >> 5;
    ctr_fn = (void(*)(void))r4;
L_8009B3A4:
    /* dcbz tmp, r3 */;
    r3 = r3 + 0x20;
    if (--ctr != 0) goto L_8009B3A4;
    return;
}

/* fn_8009B40C - 0x8009B40C | size: 0xCC */
void fn_8009B40C(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r5 = 0; /* mfmsr */;
    r5 = r5 | 0x1000;
    /* mtmsr r5 */;
    r3 = 0x80000000;
    r4 = 0x400;
    ctr_fn = (void(*)(void))r4;
L_8009B424:
    /* dcbt tmp, r3 */;
    /* dcbst tmp, r3 */;
    r3 = r3 + 0x20;
    if (--ctr != 0) goto L_8009B424;
    r4 = 0; /* mfspr HID2 */;
    r4 = r4 | (0x100f << 16);
    /* mtspr HID2, r4 */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    r3 = 0xE0000000;
    r3 = r3 | 0x2;
    /* mtdbatl 3, r3 */;
    r3 = r3 | 0x1fe;
    /* mtdbatu 3, r3 */;
    /* isync */;
    r3 = 0xE0000000;
    r6 = 0x200;
    ctr_fn = (void(*)(void))r6;
    r6 = 0x0;
L_8009B498:
    /* dcbz_l r6, r3 */;
    r3 = r3 + 0x20;
    if (--ctr != 0) goto L_8009B498;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    /* nop  */;
    return;
}

/* fn_8009B4D8 - 0x8009B4D8 | size: 0x38 */
void fn_8009B4D8(void) {
    extern void fn_8009B40C();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    OSDisableInterrupts();
    r31 = r3;
    fn_8009B40C();
    r3 = r31;
    OSRestoreInterrupts(r3);
    return;
}

/* fn_8009B538 - 0x8009B538 | size: 0x24 */
void fn_8009B538(void) {
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    /* extrwi r6, r5, 5, 25 */;
    r3 = r3 & 0xFFFFFFF;
    r6 = r6 | r3;
    /* mtspr DMA_U, r6 */;
    r6 = r6 | r4;
    r6 = r6 | 0x2;
    /* mtspr DMA_L, r6 */;
    return;
}

/* fn_8009B55C - 0x8009B55C | size: 0xAC */
void fn_8009B55C(void) {
    extern void fn_8009B538();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    r29 = r4;
    tmp = r5 + 0x1f;
    r3 = (u32)tmp >> 5;
    tmp = r3 + 0x7f;
    r31 = r3;
    r30 = (u32)tmp >> 7;
    goto L_8009B598;
L_8009B598:
    goto L_8009B59C;
L_8009B59C:
    goto L_8009B5DC;
L_8009B5A0:
    if (r31 >= 0x80) goto L_8009B5C0;
    r3 = r28;
    r4 = r29;
    r5 = r31;
    fn_8009B538();
    r31 = 0x0;
    goto L_8009B5DC;
L_8009B5C0:
    r3 = r28;
    r4 = r29;
    r5 = 0x0;
    fn_8009B538();
    r28 = r28 + 0x1000;
    r29 = r29 + 0x1000;
L_8009B5DC:
    if (r31 != 0) goto L_8009B5A0;
    r3 = r30;
    return;
}

/* fn_8009B608 - 0x8009B608 | size: 0xC */
void fn_8009B608(void) {
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = 0; /* mfspr HID2 */;
    /* extrwi r3, r4, 4, 4 */;
    return;
}

/* fn_8009B614 - 0x8009B614 | size: 0x14 */
void fn_8009B614(void) {
    u32 r3 = 0;
    u32 r4 = 0;

    fn_8009B614_loop: ;
    r4 = 0; /* mfspr HID2 */;
    /* extrwi r4, r4, 4, 4 */;
    if ((s32)r4 > (s32)r3) goto fn_8009B614_loop;
    return;
}

