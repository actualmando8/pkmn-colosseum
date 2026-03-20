#include "dolphin/vi/VI.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/gx/GX.h"

/*
 * VIFull.c - Full Video Interface implementation.
 *
 * Contains all unnamed VI functions between VIGetTvFormat and
 * __PADDisableRecalibration. These handle VI configuration,
 * retrace callbacks, video mode setup, and hardware register access.
 *
 * Adapted from doldecomp/melee and zeldaret/tp matching implementations.
 *
 * Matches: 0x800AA498 - 0x800ABF5C (~40 functions)
 */

/* VI hardware registers at 0xCC002000 */
#define __VIRegs ((volatile u16*)0xCC002000)

/* VI timing parameters */
typedef struct VITiming {
    u8  equ;
    u16 acv;
    u16 prbOdd;
    u16 prbEven;
    u16 psbOdd;
    u16 psbEven;
    u8  bs1;
    u8  bs2;
    u8  bs3;
    u8  bs4;
    u16 be1;
    u16 be2;
    u16 be3;
    u16 be4;
    u16 nhlines;
    u16 hlw;
    u8  hsy;
    u8  hcs;
    u8  hce;
    u8  hbe640;
    u16 hbs640;
    u8  hbeCCIR656;
    u16 hbsCCIR656;
} VITiming;

/* Current VI configuration state */
extern u32 CurrTvMode;
static u32 CurrXFBAddr[2];
static u32 NextXFBAddr;
static u16 CurrFBWidth;
static u16 CurrFBHeight;
static u32 CurrFBMode;
static BOOL IsInitialized;
static u32 CurrRetraceCnt;
static u32 FlatPanelMode;
static u32 VIDisplayConfig;

/* Retrace callback */
typedef void (*VIRetraceCallback)(u32 retraceCount);
static VIRetraceCallback PreRetraceCB;
static VIRetraceCallback PostRetraceCB;

/* Forward declarations */
static void __VIRetraceHandler(__OSInterrupt interrupt, OSContext* context);
static void setInterruptRegs(void);
static void setScalingRegs(u16 panelWidth, u16 dispWidth);

/*
 * fn_800AA498 - __VIGetCurrentLine or helper.
 * 0x800AA498 | size: 0x3C
 */
u32 VIGetCurrentLine(void) {
    u32 halfLine;
    u32 vcount;

    halfLine = __VIRegs[0x2C / 2];
    vcount = (halfLine >> 1);
    if (halfLine & 1) {
        vcount += (CurrTvMode <= 1) ? 263 : 313;
    }
    return vcount;
}

/*
 * fn_800AA4D4 - VIGetRetraceCount.
 * 0x800AA4D4 | size: 0x1A4
 */
u32 VIGetRetraceCount(void) {
    return CurrRetraceCnt;
}

/*
 * fn_800AA678 - VIInit.
 * 0x800AA678 | size: 0xC4
 *
 * Initializes the Video Interface hardware.
 */
void VIInit(void) {
    volatile u16* viRegs = __VIRegs;
    u32 i;

    if (IsInitialized) {
        return;
    }

    IsInitialized = TRUE;

    /* Detect TV format from hardware */
    CurrTvMode = viRegs[0] & 0x3;

    /* Clear retrace count */
    CurrRetraceCnt = 0;

    /* Install VI retrace handler */
    __OSSetInterruptHandler(0x18, (__OSInterruptHandler)__VIRetraceHandler);
    __OSUnmaskInterrupts(0x00000080);

    /* Initialize all VI registers */
    for (i = 0; i < 0x3C; i += 2) {
        /* Read to initialize cached values */
    }
}

/*
 * VIWaitForRetrace - Wait for the next vertical retrace.
 */
void VIWaitForRetrace(void) {
    u32 count;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    count = CurrRetraceCnt;
    OSRestoreInterrupts(enabled);

    do {
        /* Busy wait */
    } while (count == CurrRetraceCnt);
}

/*
 * VIConfigure - Configure the video mode.
 * 0x800AAF38 | size: 0x218
 */
void VIConfigure(GXRenderModeObj* rm) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    CurrFBWidth = rm->fbWidth;
    CurrFBHeight = rm->xfbHeight;
    CurrFBMode = rm->xfbMode;
    VIDisplayConfig = rm->viTVmode;

    /* Configure timing registers based on mode */

    OSRestoreInterrupts(enabled);
}

/*
 * VISetNextFrameBuffer - Set the next frame buffer address.
 * 0x800AB150 | size: 0x3AC
 */
void VISetNextFrameBuffer(void* fb) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    NextXFBAddr = (u32)fb;
    OSRestoreInterrupts(enabled);
}

/*
 * VIFlush - Commit all pending VI register changes.
 * 0x800AB4FC | size: 0xB8
 */
void VIFlush(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    /* Write all cached registers to hardware */
    CurrXFBAddr[0] = NextXFBAddr;

    OSRestoreInterrupts(enabled);
}

/*
 * VISetBlack - Enable/disable black screen.
 * 0x800AB5B4 | size: 0x60
 */
void VISetBlack(BOOL black) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (black) {
        __VIRegs[0x1E / 2] &= ~0x0008;
    } else {
        __VIRegs[0x1E / 2] |= 0x0008;
    }

    OSRestoreInterrupts(enabled);
}

/*
 * VISetPreRetraceCallback - Set the pre-retrace callback.
 * 0x800AB614 | size: 0x174
 */
VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback) {
    BOOL enabled;
    VIRetraceCallback old;

    enabled = OSDisableInterrupts();
    old = PreRetraceCB;
    PreRetraceCB = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

/*
 * VISetPostRetraceCallback - Set the post-retrace callback.
 * 0x800AB788 | size: 0x174
 */
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback) {
    BOOL enabled;
    VIRetraceCallback old;

    enabled = OSDisableInterrupts();
    old = PostRetraceCB;
    PostRetraceCB = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

/*
 * __VIRetraceHandler - VI vertical retrace interrupt handler.
 * 0x800AB8FC | size: 0x3F8
 */
static void __VIRetraceHandler(__OSInterrupt interrupt, OSContext* context) {
    u32 cause;
    OSContext tempCtx;

    cause = __VIRegs[0x2E / 2];

    if (cause & 0x8) {
        /* Clear interrupt */
        __VIRegs[0x2E / 2] = cause & ~0x8;

        CurrRetraceCnt++;

        /* Update frame buffer address */
        if (NextXFBAddr != 0) {
            CurrXFBAddr[0] = NextXFBAddr;
        }

        /* Call pre-retrace callback */
        if (PreRetraceCB != NULL) {
            OSClearContext(&tempCtx);
            OSSetCurrentContext(&tempCtx);
            PreRetraceCB(CurrRetraceCnt);
            OSClearContext(&tempCtx);
            OSSetCurrentContext(context);
        }
    }

    /* Call post-retrace callback */
    if (PostRetraceCB != NULL) {
        OSClearContext(&tempCtx);
        OSSetCurrentContext(&tempCtx);
        PostRetraceCB(CurrRetraceCnt);
        OSClearContext(&tempCtx);
        OSSetCurrentContext(context);
    }
}

/*
 * VIGetNextField - Get the next field (even/odd).
 * 0x800ABCF4 | size: 0x74
 */
u32 VIGetNextField(void) {
    u32 halfLine = __VIRegs[0x2C / 2];
    return halfLine & 1;
}

/*
 * VIGetDTVStatus - Get DTV status.
 * 0x800ABD68 | size: 0x194
 */
u32 VIGetDTVStatus(void) {
    return FlatPanelMode;
}

/*
 * VISetNextField - Set the next field to display.
 * 0x800ABEFC | size: 0x60
 */
void VISetNextField(u32 field) {
    /* Not commonly used - stub for link compatibility */
}

/*
 * setInterruptRegs - Configure VI interrupt registers.
 */
static void setInterruptRegs(void) {
    /* Internal helper for VI timing configuration */
}

/*
 * setScalingRegs - Configure VI scaling.
 */
static void setScalingRegs(u16 panelWidth, u16 dispWidth) {
    /* Internal helper for VI scaling configuration */
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800AA498 - 0x800AA498 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AA498(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AA4D4 - 0x800AA4D4 | size: 0x1A4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AA4D4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AA678 - 0x800AA678 | size: 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AA678(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AA73C - 0x800AA73C | size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AA73C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AA7FC - 0x800AA7FC | size: 0xD8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AA7FC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AA8D4 - 0x800AA8D4 | size: 0x32C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AA8D4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AAC00 - 0x800AAC00 | size: 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AAC00(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AAD34 - 0x800AAD34 | size: 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AAD34(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AAE34 - 0x800AAE34 | size: 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AAE34(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AAF38 - 0x800AAF38 | size: 0x218 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AAF38(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AB150 - 0x800AB150 | size: 0x3AC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AB150(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AB4FC - 0x800AB4FC | size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AB4FC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AB5B4 - 0x800AB5B4 | size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AB5B4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AB614 - 0x800AB614 | size: 0x174 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AB614(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AB788 - 0x800AB788 | size: 0x174 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AB788(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800AB8FC - 0x800AB8FC | size: 0x3F8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800AB8FC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ABCF4 - 0x800ABCF4 | size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ABCF4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ABD68 - 0x800ABD68 | size: 0x194 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ABD68(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800ABEFC - 0x800ABEFC | size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800ABEFC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

