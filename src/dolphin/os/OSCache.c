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
