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
void fn_800AA498(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    OSDisableInterrupts();
    r4 = 0xCC000000;
    tmp = *(u16*)((u8*)r4 + 0x206E);
    r31 = tmp & 0x3;
    OSRestoreInterrupts(r3);
    r3 = r31 & 0x1;
    return;
}

/* fn_800AA4D4 - 0x800AA4D4 | size: 0x1A4 */
void fn_800AA4D4(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A10[];
    extern u8 lbl_80478A14[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r5 = (u32)lbl_803FC5E0;
    r6 = r3 * 0xc;
    r5 = (u32)lbl_803FC5E0;
    r31 = r5 + r6;
    tmp = *(u32*)lbl_80478A14;
    r4 = tmp & 0x00000700;
    tmp = 0x80000000;
    tmp = (u32)tmp >> r3;
    if ((s32)r4 == 0x400) goto L_800AA5FC;
    if ((s32)r4 >= 0x400) goto L_800AA540;
    if ((s32)r4 == 0x200) goto L_800AA5CC;
    if ((s32)r4 >= 0x200) goto L_800AA534;
    if ((s32)r4 == 0x100) goto L_800AA598;
    if ((s32)r4 >= 0x100) goto L_800AA5FC;
    if ((s32)r4 == 0) goto L_800AA564;
    goto L_800AA5FC;
L_800AA534:
    if ((s32)r4 == 0x300) goto L_800AA5FC;
    goto L_800AA5FC;
L_800AA540:
    if ((s32)r4 == 0x600) goto L_800AA564;
    if ((s32)r4 >= 0x600) goto L_800AA558;
    if ((s32)r4 == 0x500) goto L_800AA564;
    goto L_800AA5FC;
L_800AA558:
    if ((s32)r4 == 0x700) goto L_800AA564;
    goto L_800AA5FC;
L_800AA564:
    r4 = *(u8*)((u8*)r31 + 0x6);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x6) = r4;
    r4 = *(u8*)((u8*)r31 + 0x7);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x7) = r4;
    r4 = *(u8*)((u8*)r31 + 0x8);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x8) = r4;
    r4 = *(u8*)((u8*)r31 + 0x9);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x9) = r4;
    goto L_800AA5FC;
L_800AA598:
    r4 = *(u8*)((u8*)r31 + 0x4);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x4) = r4;
    r4 = *(u8*)((u8*)r31 + 0x5);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x5) = r4;
    r4 = *(u8*)((u8*)r31 + 0x8);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x8) = r4;
    r4 = *(u8*)((u8*)r31 + 0x9);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x9) = r4;
    goto L_800AA5FC;
L_800AA5CC:
    r4 = *(u8*)((u8*)r31 + 0x4);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x4) = r4;
    r4 = *(u8*)((u8*)r31 + 0x5);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x5) = r4;
    r4 = *(u8*)((u8*)r31 + 0x6);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x6) = r4;
    r4 = *(u8*)((u8*)r31 + 0x7);
    /* clrrwi r4, r4, 4 */;
    *(u8*)((u8*)r31 + 0x7) = r4;
L_800AA5FC:
    r4 = *(u8*)((u8*)r31 + 0x2);
    *(u8*)((u8*)r31 + 0x2) = r4;
    r4 = *(u8*)((u8*)r31 + 0x3);
    *(u8*)((u8*)r31 + 0x3) = r4;
    r4 = *(u8*)((u8*)r31 + 0x4);
    *(u8*)((u8*)r31 + 0x4) = r4;
    r4 = *(u8*)((u8*)r31 + 0x5);
    *(u8*)((u8*)r31 + 0x5) = r4;
    r4 = *(u32*)lbl_80478A10;
    /* and. tmp, r4, tmp */;
    if ((s32)r4 == 0x700) goto L_800AA664;
    tmp = *(u8*)((u8*)r31 + 0x2);
    tmp = (s8)tmp;
    if ((s32)tmp <= 0x40) goto L_800AA664;
    SIGetType();
    /* clrrwi r3, r3, 16 */;
    /* subis tmp, r3, 0x900 */;
    if (tmp != 0) goto L_800AA664;
    tmp = 0x0;
    *(u8*)((u8*)r31 + 0x2) = tmp;
L_800AA664:
    return;
}

/* fn_800AA678 - 0x800AA678 | size: 0xC4 */
void fn_800AA678(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_80478A14[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern void fn_800AA4D4();
    extern void fn_800D0338();
    extern void fn_800D03C8();
    extern void fn_800D05A4();
    extern void fn_800D0CBC();
    extern void fn_800AA8D4();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    tmp = r4 & 0xF;
    if ((s32)tmp != 0) goto L_800AA6D4;
    r3 = *(u32*)lbl_80478A0C;
    fn_800AA4D4();
    r31 = *(u32*)lbl_80478A0C;
    tmp = 0x80000000;
    r3 = *(u32*)lbl_8047A8A4;
    r4 = r1 + 0x1c;
    tmp = (u32)tmp >> r31;
    tmp = r3 | tmp;
    *(u32*)lbl_8047A8A4 = tmp;
    r3 = r31;
    fn_800D05A4();
    tmp = *(u32*)lbl_80478A14;
    r3 = r31 + 0x0;
    r4 = tmp | (0x40 << 16);
    fn_800D0338();
    r3 = *(u32*)lbl_8047A8A4;
    fn_800D03C8();
L_800AA6D4:
    r5 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    r4 = *(u32*)lbl_80478A0C;
    if ((s32)r4 == 0x20) goto L_800AA728;
    tmp = 0x80000000;
    tmp = (u32)tmp >> r4;
    tmp = r5 & ~tmp;
    r4 = r4 * 0xc;
    *(u32*)lbl_8047A8A8 = tmp;
    r3 = (u32)lbl_803FC5E0;
    tmp = (u32)lbl_803FC5E0;
    r3 = tmp + r4;
    r4 = 0x0;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
L_800AA728:
    return;
}

/* fn_800AA73C - 0x800AA73C | size: 0xC0 */
void fn_800AA73C(void) {
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern u8 lbl_8047A8B8[];
    extern void fn_800A115C();
    extern void fn_800AA4D4();
    extern void fn_800D0464();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    tmp = 0x80000000;
    r31 = r4 + 0x0;
    r29 = r3;
    r30 = (u32)tmp >> r29;
    r3 = *(u32*)lbl_8047A8A4;
    /* and. tmp, r3, r30 */;
    if ((s32)tmp == 0) goto L_800AA7E0;
    tmp = r31 & 0xF;
    if ((s32)tmp != 0) goto L_800AA780;
    r3 = r29;
    fn_800AA4D4();
L_800AA780:
    tmp = r31 & 0x00000008;
    if ((s32)tmp == 0) goto L_800AA7E0;
    OSDisableInterrupts();
    r31 = r3 + 0x0;
    r3 = r30 + 0x0;
    fn_800D0464();
    r6 = *(u32*)lbl_8047A8A4;
    r8 = ~(r30 | r30);
    r5 = *(u32*)lbl_8047A8B0;
    r3 = r29;
    r4 = *(u32*)lbl_8047A8B4;
    tmp = *(u32*)lbl_8047A8B8;
    r7 = r6 & r8;
    r6 = r5 & r8;
    *(u32*)lbl_8047A8A4 = r7;
    r5 = r4 & r8;
    tmp = tmp & r8;
    *(u32*)lbl_8047A8B0 = r6;
    r4 = 0x0;
    *(u32*)lbl_8047A8B4 = r5;
    *(u32*)lbl_8047A8B8 = tmp;
    fn_800A115C();
    r3 = r31;
    OSRestoreInterrupts(r3);
L_800AA7E0:
    return;
}

/* fn_800AA7FC - 0x800AA7FC | size: 0xD8 */
void fn_800AA7FC(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_80478A14[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern u8 lbl_8047A8B0[];
    extern void fn_800D0338();
    extern void fn_800D03C8();
    extern void fn_800D05A4();
    extern void fn_800D0CBC();
    extern void fn_800AA8D4();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    tmp = r4 & 0xF;
    if ((s32)tmp != 0) goto L_800AA868;
    r30 = *(u32*)lbl_80478A0C;
    r31 = 0x80000000;
    r3 = *(u32*)lbl_8047A8A4;
    r4 = r1 + 0x1c;
    tmp = (u32)r31 >> r30;
    tmp = r3 | tmp;
    *(u32*)lbl_8047A8A4 = tmp;
    r3 = r30;
    fn_800D05A4();
    tmp = *(u32*)lbl_80478A14;
    r3 = r30 + 0x0;
    r4 = tmp | (0x40 << 16);
    fn_800D0338();
    r3 = *(u32*)lbl_8047A8A4;
    fn_800D03C8();
    tmp = *(u32*)lbl_80478A0C;
    r3 = *(u32*)lbl_8047A8B0;
    tmp = (u32)r31 >> tmp;
    tmp = r3 | tmp;
    *(u32*)lbl_8047A8B0 = tmp;
L_800AA868:
    r5 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    r4 = *(u32*)lbl_80478A0C;
    if ((s32)r4 == 0x20) goto L_800AA8BC;
    tmp = 0x80000000;
    tmp = (u32)tmp >> r4;
    tmp = r5 & ~tmp;
    r4 = r4 * 0xc;
    *(u32*)lbl_8047A8A8 = tmp;
    r3 = (u32)lbl_803FC5E0;
    tmp = (u32)lbl_803FC5E0;
    r3 = tmp + r4;
    r4 = 0x0;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
L_800AA8BC:
    return;
}

/* fn_800AA8D4 - 0x800AA8D4 | size: 0x32C */
void fn_800AA8D4(void) {
    extern u8 lbl_803FC5D0[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_80478A14[];
    extern u8 lbl_80478A18[];
    extern u8 lbl_80478A20[];
    extern u8 lbl_80478A24[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern u8 lbl_8047A8AC[];
    extern u8 lbl_8047A8B8[];
    extern void fn_800D0338();
    extern void fn_800D03C8();
    extern void fn_800D05A4();
    extern void fn_800D0CBC();
    extern void fn_800AA678();
    extern void fn_800AA7FC();
    extern void fn_800AA8D4();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r6 = (u32)lbl_803FC5D0;
    tmp = r4 & 0xF;
    r31 = 0x80000000;
    r30 = (u32)lbl_803FC5D0;
    r29 = *(u32*)lbl_80478A0C;
    r5 = *(u32*)lbl_8047A8AC;
    r28 = (u32)r31 >> r29;
    r3 = r5 & ~r28;
    *(u32*)lbl_8047A8AC = r3;
    r5 = r5 & r28;
    r3 = 0x1;
    if ((s32)tmp == 0) goto L_800AA970;
    r4 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r4);
    *(u32*)lbl_80478A0C = tmp;
    r3 = *(u32*)lbl_80478A0C;
    if ((s32)r3 == 0x20) goto L_800AABE0;
    tmp = r3 * 0xc;
    r3 = (u32)r31 >> r3;
    r4 = r4 & ~r3;
    r3 = r30 + tmp;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = 0x0;
    r5 = 0xc;
    r3 = r3 + 0x10;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
    goto L_800AABE0;
L_800AA970:
    /* clrrwi r6, r4, 8 */;
    r4 = r4 & 0x18000000;
    /* subis tmp, r4, 0x800 */;
    r4 = r29 << 2;
    *(u32*)(r30 + r4) = r6;
    if (tmp != 0) goto L_800AA994;
    tmp = r6 & 0x01000000;
    if (tmp != 0) goto L_800AA9E8;
L_800AA994:
    r5 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    r4 = *(u32*)lbl_80478A0C;
    if ((s32)r4 == 0x20) goto L_800AABE0;
    r3 = 0x80000000;
    tmp = r4 * 0xc;
    r3 = (u32)r3 >> r4;
    r4 = r5 & ~r3;
    r3 = r30 + tmp;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = 0x0;
    r5 = 0xc;
    r3 = r3 + 0x10;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
    goto L_800AABE0;
L_800AA9E8:
    tmp = *(u32*)lbl_80478A18;
    if (tmp >= 2) goto L_800AAA74;
    tmp = *(u32*)lbl_8047A8A4;
    r3 = r29 + 0x0;
    r4 = r1 + 0x1c;
    tmp = tmp | r28;
    *(u32*)lbl_8047A8A4 = tmp;
    fn_800D05A4();
    tmp = *(u32*)lbl_80478A14;
    r3 = r29 + 0x0;
    r4 = tmp | (0x40 << 16);
    fn_800D0338();
    r3 = *(u32*)lbl_8047A8A4;
    fn_800D03C8();
    r4 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r4);
    *(u32*)lbl_80478A0C = tmp;
    r3 = *(u32*)lbl_80478A0C;
    if ((s32)r3 == 0x20) goto L_800AABE0;
    tmp = r3 * 0xc;
    r3 = (u32)r31 >> r3;
    r4 = r4 & ~r3;
    r3 = r30 + tmp;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = 0x0;
    r5 = 0xc;
    r3 = r3 + 0x10;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
    goto L_800AABE0;
L_800AAA74:
    /* clrrwi. tmp, r6, 31 */;
    if ((s32)r3 == 0x20) goto L_800AAA84;
    tmp = r6 & 0x04000000;
    if ((s32)r3 == 0x20) goto L_800AAAF4;
L_800AAA84:
    if (r5 == 0) goto L_800AAAC0;
    tmp = r29 * 0xc;
    r3 = (u32)fn_800AA678;
    r6 = r30 + tmp;
    r8 = (u32)fn_800AA678;
    r3 = r29 + 0x0;
    r4 = (u32)lbl_80478A24;
    r5 = 0x3;
    r7 = 0xa;
    r10 = 0x0;
    r9 = 0x0;
    r6 = r6 + 0x10;
    SITransfer();
    goto L_800AAB7C;
L_800AAAC0:
    tmp = r29 * 0xc;
    r3 = (u32)fn_800AA678;
    r6 = r30 + tmp;
    r8 = (u32)fn_800AA678;
    r3 = r29 + 0x0;
    r4 = (u32)lbl_80478A20;
    r5 = 0x1;
    r7 = 0xa;
    r10 = 0x0;
    r9 = 0x0;
    r6 = r6 + 0x10;
    SITransfer();
    goto L_800AAB7C;
L_800AAAF4:
    tmp = r6 & 0x00100000;
    if (r5 == 0) goto L_800AAB7C;
    tmp = r6 & 0x00080000;
    if (r5 != 0) goto L_800AAB7C;
    tmp = r6 & 0x00040000;
    if (r5 != 0) goto L_800AAB7C;
    tmp = r6 & 0x40000000;
    if (r5 == 0) goto L_800AAB48;
    tmp = r29 * 0xc;
    r3 = (u32)fn_800AA678;
    r6 = r30 + tmp;
    r8 = (u32)fn_800AA678;
    r3 = r29 + 0x0;
    r4 = (u32)lbl_80478A20;
    r5 = 0x1;
    r7 = 0xa;
    r10 = 0x0;
    r9 = 0x0;
    r6 = r6 + 0x10;
    SITransfer();
    goto L_800AAB7C;
L_800AAB48:
    tmp = r29 * 0xc;
    r3 = (u32)fn_800AA7FC;
    r4 = r30 + r4;
    r6 = r30 + tmp;
    r8 = (u32)fn_800AA7FC;
    r3 = r29 + 0x0;
    r5 = 0x3;
    r7 = 0x8;
    r10 = 0x0;
    r9 = 0x0;
    r4 = r4 + 0x40;
    r6 = r6 + 0x10;
    SITransfer();
L_800AAB7C:
    if ((s32)r3 != 0) goto L_800AABE0;
    r5 = *(u32*)lbl_8047A8A8;
    r3 = *(u32*)lbl_8047A8B8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    tmp = r3 | r28;
    r4 = *(u32*)lbl_80478A0C;
    *(u32*)lbl_8047A8B8 = tmp;
    if ((s32)r4 == 0x20) goto L_800AABE0;
    r3 = 0x80000000;
    tmp = r4 * 0xc;
    r3 = (u32)r3 >> r4;
    r4 = r5 & ~r3;
    r3 = r30 + tmp;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = 0x0;
    r5 = 0xc;
    r3 = r3 + 0x10;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
L_800AABE0:
    return;
}

/* fn_800AAC00 - 0x800AAC00 | size: 0x134 */
void fn_800AAC00(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A20[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern u8 lbl_8047A8B8[];
    extern void fn_800A115C();
    extern void fn_800D0464();
    extern void fn_800AA73C();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x80000000;
    r29 = (u32)r3 >> r31;
    tmp = *(u32*)lbl_8047A8A4;
    /* and. tmp, tmp, r29 */;
    if ((s32)tmp == 0) goto L_800AAD18;
    r5 = *(u32*)lbl_8047A8B0;
    r6 = ~(r29 | r29);
    r3 = *(u32*)lbl_8047A8B4;
    tmp = r4 & 0xF;
    r5 = r5 & r6;
    r3 = r3 & r6;
    *(u32*)lbl_8047A8B0 = r5;
    *(u32*)lbl_8047A8B4 = r3;
    /* clrrwi r3, r4, 8 */;
    if ((s32)tmp != 0) goto L_800AACC0;
    /* clrrwi. tmp, r3, 31 */;
    if ((s32)tmp == 0) goto L_800AACC0;
    tmp = r3 & 0x00100000;
    if ((s32)tmp == 0) goto L_800AACC0;
    tmp = r3 & 0x40000000;
    if ((s32)tmp == 0) goto L_800AACC0;
    tmp = r3 & 0x04000000;
    if ((s32)tmp != 0) goto L_800AACC0;
    tmp = r3 & 0x00080000;
    if ((s32)tmp != 0) goto L_800AACC0;
    tmp = r3 & 0x00040000;
    if ((s32)tmp != 0) goto L_800AACC0;
    r4 = r31 * 0xc;
    r3 = (u32)lbl_803FC5E0;
    tmp = (u32)lbl_803FC5E0;
    r3 = (u32)fn_800AA73C;
    r6 = tmp + r4;
    r8 = (u32)fn_800AA73C;
    r3 = r31 + 0x0;
    r4 = (u32)lbl_80478A20;
    r5 = 0x1;
    r7 = 0xa;
    r10 = 0x0;
    r9 = 0x0;
    SITransfer();
    goto L_800AAD18;
L_800AACC0:
    OSDisableInterrupts();
    r30 = r3 + 0x0;
    r3 = r29 + 0x0;
    fn_800D0464();
    r6 = *(u32*)lbl_8047A8A4;
    r8 = ~(r29 | r29);
    r5 = *(u32*)lbl_8047A8B0;
    r3 = r31;
    r4 = *(u32*)lbl_8047A8B4;
    tmp = *(u32*)lbl_8047A8B8;
    r7 = r6 & r8;
    r6 = r5 & r8;
    *(u32*)lbl_8047A8A4 = r7;
    r5 = r4 & r8;
    tmp = tmp & r8;
    *(u32*)lbl_8047A8B0 = r6;
    r4 = 0x0;
    *(u32*)lbl_8047A8B4 = r5;
    *(u32*)lbl_8047A8B8 = tmp;
    fn_800A115C();
    r3 = r30;
    OSRestoreInterrupts(r3);
L_800AAD18:
    return;
}

/* fn_800AAD34 - 0x800AAD34 | size: 0x100 */
void fn_800AAD34(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_80478A18[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern u8 lbl_8047A8AC[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern u8 lbl_8047A8B8[];
    extern void fn_800D0464();
    extern void fn_800D0CBC();
    extern void fn_800AA8D4();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    OSDisableInterrupts();
    r4 = *(u32*)lbl_8047A8B8;
    r7 = 0x0;
    r5 = *(u32*)lbl_8047A8B0;
    r31 = r3 + 0x0;
    tmp = *(u32*)lbl_8047A8B4;
    r30 = r30 | r4;
    r4 = *(u32*)lbl_8047A8A8;
    r5 = r5 | tmp;
    tmp = *(u32*)lbl_80478A18;
    r30 = r30 & ~r5;
    r5 = *(u32*)lbl_8047A8A4;
    r6 = r4 | r30;
    *(u32*)lbl_8047A8B8 = r7;
    r4 = r5 & ~r30;
    *(u32*)lbl_8047A8A8 = r6;
    tmp = *(u32*)lbl_8047A8A8;
    *(u32*)lbl_8047A8A4 = r4;
    r3 = tmp & r5;
    if (tmp != 4) goto L_800AADAC;
    tmp = *(u32*)lbl_8047A8AC;
    tmp = tmp | r30;
    *(u32*)lbl_8047A8AC = tmp;
L_800AADAC:
    fn_800D0464();
    tmp = *(u32*)lbl_80478A0C;
    if ((s32)tmp != 0x20) goto L_800AAE10;
    r5 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    r4 = *(u32*)lbl_80478A0C;
    if ((s32)r4 == 0x20) goto L_800AAE10;
    tmp = 0x80000000;
    tmp = (u32)tmp >> r4;
    tmp = r5 & ~tmp;
    r4 = r4 * 0xc;
    *(u32*)lbl_8047A8A8 = tmp;
    r3 = (u32)lbl_803FC5E0;
    tmp = (u32)lbl_803FC5E0;
    r3 = tmp + r4;
    r4 = 0x0;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
L_800AAE10:
    r3 = r31;
    OSRestoreInterrupts(r3);
    r3 = 0x1;
    return;
}

/* fn_800AAE34 - 0x800AAE34 | size: 0x104 */
void fn_800AAE34(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern u8 lbl_8047A8AC[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern u8 lbl_8047A8B8[];
    extern void fn_800D0464();
    extern void fn_800D0CBC();
    extern void fn_800AA8D4();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    OSDisableInterrupts();
    r6 = *(u32*)lbl_8047A8B8;
    r7 = 0x0;
    r5 = *(u32*)lbl_8047A8B0;
    r4 = 0x80000000;
    tmp = *(u32*)lbl_8047A8B4;
    r30 = r30 | r6;
    r6 = *(u32*)lbl_8047A8A8;
    r5 = r5 | tmp;
    tmp = *(u8*)((u8*)r4 + 0x30E3);
    r30 = r30 & ~r5;
    r5 = *(u32*)lbl_8047A8A4;
    r4 = r6 | r30;
    *(u32*)lbl_8047A8B8 = r7;
    tmp = tmp & 0x00000040;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = r5 & ~r30;
    r31 = r3 + 0x0;
    r6 = *(u32*)lbl_8047A8A8;
    *(u32*)lbl_8047A8A4 = r4;
    r3 = r6 & r5;
    if ((s32)tmp != 0) goto L_800AAEB0;
    tmp = *(u32*)lbl_8047A8AC;
    tmp = tmp | r30;
    *(u32*)lbl_8047A8AC = tmp;
L_800AAEB0:
    fn_800D0464();
    tmp = *(u32*)lbl_80478A0C;
    if ((s32)tmp != 0x20) goto L_800AAF14;
    r5 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    r4 = *(u32*)lbl_80478A0C;
    if ((s32)r4 == 0x20) goto L_800AAF14;
    tmp = 0x80000000;
    tmp = (u32)tmp >> r4;
    tmp = r5 & ~tmp;
    r4 = r4 * 0xc;
    *(u32*)lbl_8047A8A8 = tmp;
    r3 = (u32)lbl_803FC5E0;
    tmp = (u32)lbl_803FC5E0;
    r3 = tmp + r4;
    r4 = 0x0;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
L_800AAF14:
    r3 = r31;
    OSRestoreInterrupts(r3);
    r3 = 0x1;
    return;
}

/* fn_800AAF38 - 0x800AAF38 | size: 0x218 */
void fn_800AAF38(void) {
    extern u8 lbl_80312500[];
    extern u8 lbl_803FC5D0[];
    extern u8 lbl_80478A08[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_80478A18[];
    extern u8 lbl_8047A8A0[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern u8 lbl_8047A8AC[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern u8 lbl_8047A8B8[];
    extern u8 lbl_8047AA58[];
    extern void fn_800AB5B4();
    extern void fn_800C4C98();
    extern void fn_800D0464();
    extern void fn_800D0CBC();
    extern void fn_800D104C();
    extern void fn_800AA8D4();
    extern u32 __PADSpec;
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r3 = (u32)lbl_803FC5D0;
    r31 = (u32)lbl_803FC5D0;
    tmp = *(u32*)lbl_8047A8A0;
    if ((s32)tmp == 0) goto L_800AAF64;
    r3 = 0x1;
    goto L_800AB13C;
L_800AAF64:
    r3 = *(u32*)lbl_80478A08;
    OSRegisterVersion((const char*)r3);
    r3 = *(u32*)__PADSpec;
    if (r3 == 0) goto L_800AAF7C;
    fn_800AB5B4();
L_800AAF7C:
    tmp = *(u32*)lbl_8047AA58;
    r3 = 0x1;
    *(u32*)lbl_8047A8A0 = r3;
    if (tmp == 0) goto L_800AB018;
    OSGetTime();
    r25 = r4 + 0x0;
    r26 = r3 + 0x0;
    r5 = 0x10;
    fn_800C4C98();
    r5 = 0x10000;
    r28 = 0x0;
    r6 = r4 & r27;
    r4 = r25 & r27;
    r5 = r3 & r28;
    tmp = r26 & r28;
    r29 = r4 + r6;
    r3 = r26 + 0x0;
    r4 = r25 + 0x0;
    r30 = tmp + r5; /* +carry */;
    r5 = 0x20;
    fn_800C4C98();
    r4 = r4 & r27;
    tmp = r3 & r28;
    r29 = r4 + r29;
    r3 = r26 + 0x0;
    r4 = r25 + 0x0;
    r30 = tmp + r30; /* +carry */;
    r5 = 0x30;
    fn_800C4C98();
    tmp = r4 & r27;
    r5 = tmp + r29;
    tmp = 0xF0000000;
    r4 = 0x3fff;
    *(u32*)lbl_8047A8AC = tmp;
    tmp = r5 & r4;
    r3 = 0x80000000;
    *(u16*)((u8*)r3 + 0x30E0) = tmp;
L_800AB018:
    r3 = 0x80000000;
    tmp = *(u16*)((u8*)r3 + 0x30E0);
    tmp = tmp | (0x4d00 << 16);
    *(u32*)((u8*)r31 + 0x40) = tmp;
    tmp = *(u16*)((u8*)r3 + 0x30E0);
    tmp = tmp | (0x4d40 << 16);
    *(u32*)((u8*)r31 + 0x44) = tmp;
    tmp = *(u16*)((u8*)r3 + 0x30E0);
    tmp = tmp | (0x4d80 << 16);
    *(u32*)((u8*)r31 + 0x48) = tmp;
    tmp = *(u16*)((u8*)r3 + 0x30E0);
    tmp = tmp | (0x4dc0 << 16);
    *(u32*)((u8*)r31 + 0x4C) = tmp;
    fn_800D104C();
    r3 = (u32)lbl_80312500;
    r3 = (u32)lbl_80312500;
    OSRegisterResetFunction();
    r28 = 0xF0000000;
    OSDisableInterrupts();
    r4 = *(u32*)lbl_8047A8B8;
    r7 = 0x0;
    r5 = *(u32*)lbl_8047A8B0;
    r27 = r3 + 0x0;
    tmp = *(u32*)lbl_8047A8B4;
    r28 = r28 | r4;
    r4 = *(u32*)lbl_8047A8A8;
    r5 = r5 | tmp;
    tmp = *(u32*)lbl_80478A18;
    r28 = r28 & ~r5;
    r5 = *(u32*)lbl_8047A8A4;
    r6 = r4 | r28;
    *(u32*)lbl_8047A8B8 = r7;
    r4 = r5 & ~r28;
    *(u32*)lbl_8047A8A8 = r6;
    tmp = *(u32*)lbl_8047A8A8;
    *(u32*)lbl_8047A8A4 = r4;
    r3 = tmp & r5;
    if (tmp != 4) goto L_800AB0D0;
    tmp = *(u32*)lbl_8047A8AC;
    tmp = tmp | r28;
    *(u32*)lbl_8047A8AC = tmp;
L_800AB0D0:
    fn_800D0464();
    tmp = *(u32*)lbl_80478A0C;
    if ((s32)tmp != 0x20) goto L_800AB130;
    r5 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    r4 = *(u32*)lbl_80478A0C;
    if ((s32)r4 == 0x20) goto L_800AB130;
    r3 = 0x80000000;
    tmp = r4 * 0xc;
    r3 = (u32)r3 >> r4;
    r4 = r5 & ~r3;
    r3 = r31 + tmp;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = 0x0;
    r5 = 0xc;
    r3 = r3 + 0x10;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
L_800AB130:
    r3 = r27;
    OSRestoreInterrupts(r3);
    r3 = 0x1;
L_800AB13C:
    return;
}

/* fn_800AB150 - 0x800AB150 | size: 0x3AC */
void fn_800AB150(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_80478A18[];
    extern u8 lbl_80478A1C[];
    extern u8 lbl_80478A20[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern u8 lbl_8047A8AC[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern u8 lbl_8047A8B8[];
    extern void fn_800A115C();
    extern void fn_800CF728();
    extern void fn_800D02BC();
    extern void fn_800D0464();
    extern void fn_800D05A4();
    extern void fn_800D0CBC();
    extern void fn_800AA73C();
    extern void fn_800AA8D4();
    extern void fn_800AAC00();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    OSDisableInterrupts();
    r21 = 0x0;
    tmp = r21 * 0xc;
    r4 = (u32)lbl_803FC5E0;
    r26 = (u32)lbl_803FC5E0;
    r4 = (u32)fn_800AA73C;
    r5 = (u32)fn_800AAC00;
    r6 = (u32)fn_800AA8D4;
    r24 = r26 + tmp;
    r22 = r3 + 0x0;
    r30 = (u32)fn_800AA73C;
    r29 = (u32)fn_800AAC00;
    r28 = (u32)fn_800AA8D4;
    r20 = 0x0;
    r27 = 0x80000000;
L_800AB1A0:
    tmp = *(u32*)lbl_8047A8B8;
    r23 = (u32)r27 >> r21;
    /* and. tmp, tmp, r23 */;
    if ((s32)tmp == 0) goto L_800AB284;
    OSDisableInterrupts();
    r4 = *(u32*)lbl_8047A8B0;
    r6 = 0x0;
    tmp = *(u32*)lbl_8047A8B4;
    r25 = r3;
    r5 = *(u32*)lbl_8047A8B8;
    r4 = r4 | tmp;
    tmp = *(u32*)lbl_8047A8A8;
    r7 = r5 & ~r4;
    r5 = *(u32*)lbl_8047A8A4;
    r4 = tmp | r7;
    tmp = *(u32*)lbl_80478A18;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = r5 & ~r7;
    tmp = *(u32*)lbl_8047A8A8;
    *(u32*)lbl_8047A8B8 = r6;
    r3 = tmp & r5;
    *(u32*)lbl_8047A8A4 = r4;
    if (tmp != 4) goto L_800AB20C;
    tmp = *(u32*)lbl_8047A8AC;
    tmp = tmp | r7;
    *(u32*)lbl_8047A8AC = tmp;
L_800AB20C:
    fn_800D0464();
    tmp = *(u32*)lbl_80478A0C;
    if ((s32)tmp != 0x20) goto L_800AB260;
    r4 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r4);
    *(u32*)lbl_80478A0C = tmp;
    r3 = *(u32*)lbl_80478A0C;
    if ((s32)r3 == 0x20) goto L_800AB260;
    tmp = r3 * 0xc;
    r3 = (u32)r27 >> r3;
    r4 = r4 & ~r3;
    *(u32*)lbl_8047A8A8 = r4;
    r3 = r26 + tmp;
    r4 = 0x0;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = *(u32*)lbl_80478A0C;
    r4 = r28;
    fn_800D0CBC();
L_800AB260:
    r3 = r25;
    OSRestoreInterrupts(r3);
    tmp = -0x2;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_800AB4C8;
L_800AB284:
    tmp = *(u32*)lbl_8047A8A8;
    /* and. tmp, tmp, r23 */;
    if ((s32)r3 != 0x20) goto L_800AB29C;
    tmp = *(u32*)lbl_80478A0C;
    if ((s32)tmp != (s32)r21) goto L_800AB2B8;
L_800AB29C:
    tmp = -0x2;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_800AB4C8;
L_800AB2B8:
    tmp = *(u32*)lbl_8047A8A4;
    /* and. tmp, tmp, r23 */;
    if ((s32)tmp != (s32)r21) goto L_800AB2E0;
    tmp = -0x1;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_800AB4C8;
L_800AB2E0:
    r3 = r21;
    fn_800CF728();
    if ((s32)r3 == 0) goto L_800AB30C;
    tmp = -0x3;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_800AB4C8;
L_800AB30C:
    r3 = r21;
    fn_800D02BC();
    tmp = r3 & 0x00000008;
    if ((s32)r3 == 0) goto L_800AB3E4;
    r3 = r21 + 0x0;
    r4 = r1 + 0x14;
    fn_800D05A4();
    tmp = *(u32*)lbl_8047A8B0;
    /* and. tmp, tmp, r23 */;
    if ((s32)r3 == 0) goto L_800AB370;
    tmp = 0x0;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = *(u32*)lbl_8047A8B4;
    /* and. tmp, r3, r23 */;
    if ((s32)r3 != 0) goto L_800AB4C8;
    tmp = r3 | r23;
    *(u32*)lbl_8047A8B4 = tmp;
    r3 = r21 + 0x0;
    r4 = r29 + 0x0;
    fn_800D0CBC();
    goto L_800AB4C8;
L_800AB370:
    OSDisableInterrupts();
    r25 = r3 + 0x0;
    r3 = r23 + 0x0;
    fn_800D0464();
    r6 = *(u32*)lbl_8047A8A4;
    r8 = ~(r23 | r23);
    r5 = *(u32*)lbl_8047A8B0;
    r3 = r21;
    r4 = *(u32*)lbl_8047A8B4;
    tmp = *(u32*)lbl_8047A8B8;
    r7 = r6 & r8;
    r6 = r5 & r8;
    *(u32*)lbl_8047A8A4 = r7;
    r5 = r4 & r8;
    tmp = tmp & r8;
    *(u32*)lbl_8047A8B0 = r6;
    r4 = 0x0;
    *(u32*)lbl_8047A8B4 = r5;
    *(u32*)lbl_8047A8B8 = tmp;
    fn_800A115C();
    r3 = r25;
    OSRestoreInterrupts(r3);
    tmp = -0x1;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_800AB4C8;
L_800AB3E4:
    r3 = r21;
    SIGetType();
    tmp = r3 & 0x20000000;
    if ((s32)r3 != 0) goto L_800AB3F8;
    r20 = r20 | r23;
L_800AB3F8:
    r3 = r21 + 0x0;
    r4 = r1 + 0x14;
    fn_800D05A4();
    if ((s32)r3 != 0) goto L_800AB428;
    tmp = -0x3;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_800AB4C8;
L_800AB428:
    /* clrrwi. tmp, tmp, 31 */;
    if ((s32)r3 == 0) goto L_800AB450;
    tmp = -0x3;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_800AB4C8;
L_800AB450:
    r12 = *(u32*)lbl_80478A1C;
    r3 = r21 + 0x0;
    r4 = r31 + 0x0;
    r5 = r1 + 0x14;
    /* blrl  */;
    tmp = *(u16*)((u8*)r31 + 0x0);
    tmp = tmp & 0x00002000;
    if ((s32)r3 == 0) goto L_800AB4B4;
    tmp = -0x3;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    r3 = r31 + 0x0;
    r4 = 0x0;
    r5 = 0xa;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r21 + 0x0;
    r6 = r24 + 0x0;
    r8 = r30 + 0x0;
    r4 = (u32)lbl_80478A20;
    r5 = 0x1;
    r7 = 0xa;
    r10 = 0x0;
    r9 = 0x0;
    SITransfer();
    goto L_800AB4C8;
L_800AB4B4:
    tmp = 0x0;
    *(u8*)((u8*)r31 + 0xA) = tmp;
    tmp = *(u16*)((u8*)r31 + 0x0);
    tmp = tmp & 0xFFFFFF7F;
    *(u16*)((u8*)r31 + 0x0) = tmp;
L_800AB4C8:
    r21 = r21 + 0x1;
    r24 = r24 + 0xc;
    r31 = r31 + 0xc;
    if ((s32)r21 < 4) goto L_800AB1A0;
    r3 = r22;
    OSRestoreInterrupts(r3);
    r3 = r20;
    return;
}

/* fn_800AB4FC - 0x800AB4FC | size: 0xB8 */
void fn_800AB4FC(void) {
    extern u8 lbl_80478A14[];
    extern u8 lbl_80478A18[];
    extern u8 lbl_8047A8A4[];
    extern void fn_800D0338();
    extern void fn_800D034C();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r27 = r3;
    OSDisableInterrupts();
    r30 = r3 + 0x0;
    r28 = 0x0;
    r29 = 0x0;
    r31 = 0x80000000;
L_800AB524:
    r3 = *(u32*)lbl_8047A8A4;
    tmp = (u32)r31 >> r29;
    /* and. tmp, r3, tmp */;
    if ((s32)tmp == 0) goto L_800AB57C;
    r3 = r29;
    SIGetType();
    tmp = r3 & 0x20000000;
    if ((s32)tmp != 0) goto L_800AB57C;
    tmp = *(u32*)lbl_80478A18;
    r3 = *(u32*)((u8*)r27 + 0x0);
    if (tmp >= 2) goto L_800AB560;
    if (r3 != 2) goto L_800AB560;
    r3 = 0x0;
L_800AB560:
    r4 = *(u32*)lbl_80478A14;
    tmp = r3 & 0x3;
    r3 = r29 + 0x0;
    r4 = r4 | (0x40 << 16);
    r4 = r4 | tmp;
    fn_800D0338();
    r28 = 0x1;
L_800AB57C:
    r29 = r29 + 0x1;
    r27 = r27 + 0x4;
    if ((s32)r29 < 4) goto L_800AB524;
    if ((s32)r28 == 0) goto L_800AB598;
    fn_800D034C();
L_800AB598:
    r3 = r30;
    OSRestoreInterrupts(r3);
    return;
}

/* fn_800AB5B4 - 0x800AB5B4 | size: 0x60 */
void fn_800AB5B4(void) {
    extern u8 lbl_80478A18[];
    extern u8 lbl_80478A1C[];
    extern void fn_800AB614();
    extern void fn_800AB788();
    extern void fn_800AB8FC();
    extern u32 __PADSpec;
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    tmp = 0x0;
    *(u32*)__PADSpec = tmp;
    if ((s32)r3 == 1) goto L_800AB5F0;
    if ((s32)r3 >= 1) goto L_800AB5D4;
    if ((s32)r3 >= 0) goto L_800AB5E0;
    goto L_800AB60C;
L_800AB5D4:
    if ((s32)r3 >= 6) goto L_800AB60C;
    goto L_800AB600;
L_800AB5E0:
    r4 = (u32)fn_800AB614;
    tmp = (u32)fn_800AB614;
    *(u32*)lbl_80478A1C = tmp;
    goto L_800AB60C;
L_800AB5F0:
    r4 = (u32)fn_800AB788;
    tmp = (u32)fn_800AB788;
    *(u32*)lbl_80478A1C = tmp;
    goto L_800AB60C;
L_800AB600:
    r4 = (u32)fn_800AB8FC;
    tmp = (u32)fn_800AB8FC;
    *(u32*)lbl_80478A1C = tmp;
L_800AB60C:
    *(u32*)lbl_80478A18 = r3;
    return;
}

/* fn_800AB614 - 0x800AB614 | size: 0x174 */
void fn_800AB614(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = 0x0;
    *(u16*)((u8*)r4 + 0x0) = r3;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000008;
    if ((s32)tmp == 0) goto L_800AB62C;
    r3 = 0x100;
L_800AB62C:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000020;
    if ((s32)tmp == 0) goto L_800AB64C;
    r3 = 0x200;
    goto L_800AB650;
L_800AB64C:
    r3 = 0x0;
L_800AB650:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000100;
    if ((s32)tmp == 0) goto L_800AB670;
    r3 = 0x400;
    goto L_800AB674;
L_800AB670:
    r3 = 0x0;
L_800AB674:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    /* extrwi. tmp, tmp, 1, 15 */;
    if ((s32)tmp == 0) goto L_800AB694;
    r3 = 0x800;
    goto L_800AB698;
L_800AB694:
    r3 = 0x0;
L_800AB698:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000010;
    if ((s32)tmp == 0) goto L_800AB6B8;
    r6 = 0x1000;
    goto L_800AB6BC;
L_800AB6B8:
    r6 = 0x0;
L_800AB6BC:
    r3 = *(u16*)((u8*)r4 + 0x0);
    tmp = 0x0;
    r3 = r3 | r6;
    *(u16*)((u8*)r4 + 0x0) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (u32)r3 >> 16;
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x2) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (u32)r3 >> 24;
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x3) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x4) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (u32)r3 >> 8;
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x5) = r3;
    r3 = *(u32*)((u8*)r5 + 0x0);
    /* extrwi r3, r3, 8, 16 */;
    *(u8*)((u8*)r4 + 0x6) = r3;
    r3 = *(u32*)((u8*)r5 + 0x0);
    *(u8*)((u8*)r4 + 0x7) = r3;
    *(u8*)((u8*)r4 + 0x8) = tmp;
    *(u8*)((u8*)r4 + 0x9) = tmp;
    tmp = *(u8*)((u8*)r4 + 0x6);
    if (tmp < 0xaa) goto L_800AB73C;
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | 0x40;
    *(u16*)((u8*)r4 + 0x0) = tmp;
L_800AB73C:
    tmp = *(u8*)((u8*)r4 + 0x7);
    if (tmp < 0xaa) goto L_800AB754;
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | 0x20;
    *(u16*)((u8*)r4 + 0x0) = tmp;
L_800AB754:
    r3 = *(u8*)((u8*)r4 + 0x2);
    *(u8*)((u8*)r4 + 0x2) = tmp;
    r3 = *(u8*)((u8*)r4 + 0x3);
    *(u8*)((u8*)r4 + 0x3) = tmp;
    r3 = *(u8*)((u8*)r4 + 0x4);
    *(u8*)((u8*)r4 + 0x4) = tmp;
    r3 = *(u8*)((u8*)r4 + 0x5);
    *(u8*)((u8*)r4 + 0x5) = tmp;
    return;
}

/* fn_800AB788 - 0x800AB788 | size: 0x174 */
void fn_800AB788(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r3 = 0x0;
    *(u16*)((u8*)r4 + 0x0) = r3;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000080;
    if ((s32)tmp == 0) goto L_800AB7A0;
    r3 = 0x100;
L_800AB7A0:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000100;
    if ((s32)tmp == 0) goto L_800AB7C0;
    r3 = 0x200;
    goto L_800AB7C4;
L_800AB7C0:
    r3 = 0x0;
L_800AB7C4:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000020;
    if ((s32)tmp == 0) goto L_800AB7E4;
    r3 = 0x400;
    goto L_800AB7E8;
L_800AB7E4:
    r3 = 0x0;
L_800AB7E8:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000010;
    if ((s32)tmp == 0) goto L_800AB808;
    r3 = 0x800;
    goto L_800AB80C;
L_800AB808:
    r3 = 0x0;
L_800AB80C:
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | r3;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x00000200;
    if ((s32)tmp == 0) goto L_800AB82C;
    r6 = 0x1000;
    goto L_800AB830;
L_800AB82C:
    r6 = 0x0;
L_800AB830:
    r3 = *(u16*)((u8*)r4 + 0x0);
    tmp = 0x0;
    r3 = r3 | r6;
    *(u16*)((u8*)r4 + 0x0) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (u32)r3 >> 16;
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x2) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (u32)r3 >> 24;
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x3) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x4) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = (u32)r3 >> 8;
    r3 = (s8)r3;
    *(u8*)((u8*)r4 + 0x5) = r3;
    r3 = *(u32*)((u8*)r5 + 0x0);
    /* extrwi r3, r3, 8, 16 */;
    *(u8*)((u8*)r4 + 0x6) = r3;
    r3 = *(u32*)((u8*)r5 + 0x0);
    *(u8*)((u8*)r4 + 0x7) = r3;
    *(u8*)((u8*)r4 + 0x8) = tmp;
    *(u8*)((u8*)r4 + 0x9) = tmp;
    tmp = *(u8*)((u8*)r4 + 0x6);
    if (tmp < 0xaa) goto L_800AB8B0;
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | 0x40;
    *(u16*)((u8*)r4 + 0x0) = tmp;
L_800AB8B0:
    tmp = *(u8*)((u8*)r4 + 0x7);
    if (tmp < 0xaa) goto L_800AB8C8;
    tmp = *(u16*)((u8*)r4 + 0x0);
    tmp = tmp | 0x20;
    *(u16*)((u8*)r4 + 0x0) = tmp;
L_800AB8C8:
    r3 = *(u8*)((u8*)r4 + 0x2);
    *(u8*)((u8*)r4 + 0x2) = tmp;
    r3 = *(u8*)((u8*)r4 + 0x3);
    *(u8*)((u8*)r4 + 0x3) = tmp;
    r3 = *(u8*)((u8*)r4 + 0x4);
    *(u8*)((u8*)r4 + 0x4) = tmp;
    r3 = *(u8*)((u8*)r4 + 0x5);
    *(u8*)((u8*)r4 + 0x5) = tmp;
    return;
}

/* fn_800AB8FC - 0x800AB8FC | size: 0x3F8 */
void fn_800AB8FC(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A14[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    tmp = *(u32*)((u8*)r5 + 0x0);
    /* extrwi tmp, tmp, 14, 2 */;
    *(u16*)((u8*)r4 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = (u32)tmp >> 8;
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x2) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x0);
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x3) = tmp;
    tmp = *(u32*)lbl_80478A14;
    tmp = tmp & 0x00000700;
    if ((s32)tmp == 0x400) goto L_800ABAC8;
    if ((s32)tmp >= 0x400) goto L_800AB968;
    if ((s32)tmp == 0x200) goto L_800ABA34;
    if ((s32)tmp >= 0x200) goto L_800AB95C;
    if ((s32)tmp == 0x100) goto L_800AB9E0;
    if ((s32)tmp >= 0x100) goto L_800ABB08;
    if ((s32)tmp == 0) goto L_800AB98C;
    goto L_800ABB08;
L_800AB95C:
    if ((s32)tmp == 0x300) goto L_800ABA84;
    goto L_800ABB08;
L_800AB968:
    if ((s32)tmp == 0x600) goto L_800AB98C;
    if ((s32)tmp >= 0x600) goto L_800AB980;
    if ((s32)tmp == 0x500) goto L_800AB98C;
    goto L_800ABB08;
L_800AB980:
    if ((s32)tmp == 0x700) goto L_800AB98C;
    goto L_800ABB08;
L_800AB98C:
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = (u32)tmp >> 24;
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x4) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = (u32)tmp >> 16;
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x5) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 24) | ((u32)tmp >> 8)) & 0x000000F0;
    *(u8*)((u8*)r4 + 0x6) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 28) | ((u32)tmp >> 4)) & 0x000000F0;
    *(u8*)((u8*)r4 + 0x7) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = tmp & 0x000000F0;
    *(u8*)((u8*)r4 + 0x8) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    *(u8*)((u8*)r4 + 0x9) = tmp;
    goto L_800ABB08;
L_800AB9E0:
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 8) | ((u32)tmp >> 24)) & 0x000000F0;
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x4) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 12) | ((u32)tmp >> 20)) & 0x000000F0;
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x5) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    /* extrwi tmp, tmp, 8, 8 */;
    *(u8*)((u8*)r4 + 0x6) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    /* extrwi tmp, tmp, 8, 16 */;
    *(u8*)((u8*)r4 + 0x7) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = tmp & 0x000000F0;
    *(u8*)((u8*)r4 + 0x8) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    *(u8*)((u8*)r4 + 0x9) = tmp;
    goto L_800ABB08;
L_800ABA34:
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 8) | ((u32)tmp >> 24)) & 0x000000F0;
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x4) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 12) | ((u32)tmp >> 20)) & 0x000000F0;
    tmp = (s8)tmp;
    *(u8*)((u8*)r4 + 0x5) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 16) | ((u32)tmp >> 16)) & 0x000000F0;
    *(u8*)((u8*)r4 + 0x6) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    tmp = ((tmp << 20) | ((u32)tmp >> 12)) & 0x000000F0;
    *(u8*)((u8*)r4 + 0x7) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    /* extrwi tmp, tmp, 8, 16 */;
    *(u8*)((u8*)r4 + 0x8) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    *(u8*)((u8*)r4 + 0x9) = tmp;
    goto L_800ABB08;
L_800ABA84:
    r6 = *(u32*)((u8*)r5 + 0x4);
    tmp = 0x0;
    r6 = (u32)r6 >> 24;
    r6 = (s8)r6;
    *(u8*)((u8*)r4 + 0x4) = r6;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r6 = (u32)r6 >> 16;
    r6 = (s8)r6;
    *(u8*)((u8*)r4 + 0x5) = r6;
    r6 = *(u32*)((u8*)r5 + 0x4);
    /* extrwi r6, r6, 8, 16 */;
    *(u8*)((u8*)r4 + 0x6) = r6;
    r5 = *(u32*)((u8*)r5 + 0x4);
    *(u8*)((u8*)r4 + 0x7) = r5;
    *(u8*)((u8*)r4 + 0x8) = tmp;
    *(u8*)((u8*)r4 + 0x9) = tmp;
    goto L_800ABB08;
L_800ABAC8:
    r6 = *(u32*)((u8*)r5 + 0x4);
    tmp = 0x0;
    r6 = (u32)r6 >> 24;
    r6 = (s8)r6;
    *(u8*)((u8*)r4 + 0x4) = r6;
    r6 = *(u32*)((u8*)r5 + 0x4);
    r6 = (u32)r6 >> 16;
    r6 = (s8)r6;
    *(u8*)((u8*)r4 + 0x5) = r6;
    *(u8*)((u8*)r4 + 0x6) = tmp;
    *(u8*)((u8*)r4 + 0x7) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    /* extrwi tmp, tmp, 8, 16 */;
    *(u8*)((u8*)r4 + 0x8) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x4);
    *(u8*)((u8*)r4 + 0x9) = tmp;
L_800ABB08:
    r6 = *(u8*)((u8*)r4 + 0x2);
    r5 = (u32)lbl_803FC5E0;
    r3 = r3 * 0xc;
    *(u8*)((u8*)r4 + 0x2) = tmp;
    tmp = (u32)lbl_803FC5E0;
    r3 = tmp + r3;
    r5 = *(u8*)((u8*)r4 + 0x3);
    *(u8*)((u8*)r4 + 0x3) = tmp;
    r5 = *(u8*)((u8*)r4 + 0x4);
    *(u8*)((u8*)r4 + 0x4) = tmp;
    r5 = *(u8*)((u8*)r4 + 0x5);
    *(u8*)((u8*)r4 + 0x5) = tmp;
    r7 = *(u8*)((u8*)r3 + 0x2);
    r6 = *(u8*)((u8*)r4 + 0x2);
    tmp = (s8)r7;
    if ((s32)tmp <= 0x700) goto L_800ABB78;
    r5 = (s8)r7;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)tmp >= (s32)r5) goto L_800ABB9C;
    r6 = r5;
    goto L_800ABB9C;
L_800ABB78:
    tmp = (s8)r7;
    if ((s32)tmp >= (s32)r5) goto L_800ABB9C;
    r5 = (s8)r7;
    tmp = r5 + 0x7f;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)r5 >= (s32)tmp) goto L_800ABB9C;
    r6 = r5;
L_800ABB9C:
    r6 = r6 - r7;
    *(u8*)((u8*)r4 + 0x2) = r6;
    r7 = *(u8*)((u8*)r3 + 0x3);
    r6 = *(u8*)((u8*)r4 + 0x3);
    tmp = (s8)r7;
    if ((s32)r5 <= (s32)tmp) goto L_800ABBD4;
    r5 = (s8)r7;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)tmp >= (s32)r5) goto L_800ABBF8;
    r6 = r5;
    goto L_800ABBF8;
L_800ABBD4:
    tmp = (s8)r7;
    if ((s32)tmp >= (s32)r5) goto L_800ABBF8;
    r5 = (s8)r7;
    tmp = r5 + 0x7f;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)r5 >= (s32)tmp) goto L_800ABBF8;
    r6 = r5;
L_800ABBF8:
    r6 = r6 - r7;
    *(u8*)((u8*)r4 + 0x3) = r6;
    r7 = *(u8*)((u8*)r3 + 0x4);
    r6 = *(u8*)((u8*)r4 + 0x4);
    tmp = (s8)r7;
    if ((s32)r5 <= (s32)tmp) goto L_800ABC30;
    r5 = (s8)r7;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)tmp >= (s32)r5) goto L_800ABC54;
    r6 = r5;
    goto L_800ABC54;
L_800ABC30:
    tmp = (s8)r7;
    if ((s32)tmp >= (s32)r5) goto L_800ABC54;
    r5 = (s8)r7;
    tmp = r5 + 0x7f;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)r5 >= (s32)tmp) goto L_800ABC54;
    r6 = r5;
L_800ABC54:
    r6 = r6 - r7;
    *(u8*)((u8*)r4 + 0x4) = r6;
    r7 = *(u8*)((u8*)r3 + 0x5);
    r6 = *(u8*)((u8*)r4 + 0x5);
    tmp = (s8)r7;
    if ((s32)r5 <= (s32)tmp) goto L_800ABC8C;
    r5 = (s8)r7;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)tmp >= (s32)r5) goto L_800ABCB0;
    r6 = r5;
    goto L_800ABCB0;
L_800ABC8C:
    tmp = (s8)r7;
    if ((s32)tmp >= (s32)r5) goto L_800ABCB0;
    r5 = (s8)r7;
    tmp = r5 + 0x7f;
    r5 = (s8)tmp;
    tmp = (s8)r6;
    if ((s32)r5 >= (s32)tmp) goto L_800ABCB0;
    r6 = r5;
L_800ABCB0:
    r6 = r6 - r7;
    *(u8*)((u8*)r4 + 0x5) = r6;
    tmp = *(u8*)((u8*)r3 + 0x6);
    r5 = *(u8*)((u8*)r4 + 0x6);
    if (r5 >= tmp) goto L_800ABCCC;
    r5 = tmp;
L_800ABCCC:
    r5 = r5 - tmp;
    *(u8*)((u8*)r4 + 0x6) = r5;
    tmp = *(u8*)((u8*)r3 + 0x7);
    r3 = *(u8*)((u8*)r4 + 0x7);
    if (r3 >= tmp) goto L_800ABCE8;
    r3 = tmp;
L_800ABCE8:
    r3 = r3 - tmp;
    *(u8*)((u8*)r4 + 0x7) = r3;
    return;
}

/* fn_800ABCF4 - 0x800ABCF4 | size: 0x74 */
void fn_800ABCF4(void) {
    extern u8 lbl_80478A14[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern void fn_800D0464();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;

    r31 = r3;
    OSDisableInterrupts();
    r5 = *(u32*)lbl_8047A8A4;
    r6 = r31 << 8;
    r4 = *(u32*)lbl_8047A8B0;
    r31 = r3;
    r8 = r5 + 0x0;
    r7 = ~(r8 | r8);
    tmp = *(u32*)lbl_8047A8B4;
    r5 = r5 & ~r5;
    *(u32*)lbl_80478A14 = r6;
    r4 = r4 & r7;
    tmp = tmp & r7;
    *(u32*)lbl_8047A8A4 = r5;
    r3 = r8;
    *(u32*)lbl_8047A8B0 = r4;
    *(u32*)lbl_8047A8B4 = tmp;
    fn_800D0464();
    r3 = r31;
    OSRestoreInterrupts(r3);
    return;
}

/* fn_800ABD68 - 0x800ABD68 | size: 0x194 */
void fn_800ABD68(void) {
    extern u8 lbl_803FC5E0[];
    extern u8 lbl_80478A0C[];
    extern u8 lbl_8047A8A4[];
    extern u8 lbl_8047A8A8[];
    extern u8 lbl_8047A8AC[];
    extern u8 lbl_8047A8B0[];
    extern u8 lbl_8047A8B4[];
    extern u8 lbl_8047A8B8[];
    extern u8 lbl_8047A8BC[];
    extern u8 lbl_8047A8C0[];
    extern void fn_800ABF5C();
    extern void fn_800CF708();
    extern void fn_800D0464();
    extern void fn_800D0CBC();
    extern void fn_800AA8D4();
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r30 = r3 + 0x0;
    tmp = *(u32*)lbl_8047A8BC;
    if (tmp == 0) goto L_800ABD94;
    r3 = 0x0;
    fn_800ABF5C();
L_800ABD94:
    if ((s32)r30 != 0) goto L_800ABED8;
    tmp = *(u32*)lbl_8047A8A8;
    r30 = 0x0;
    r3 = r30 + 0x0;
    if (tmp != 0) goto L_800ABDC0;
    tmp = *(u32*)lbl_80478A0C;
    if ((s32)tmp != 0x20) goto L_800ABDC0;
    r3 = 0x1;
L_800ABDC0:
    if ((s32)r3 == 0) goto L_800ABDD8;
    fn_800CF708();
    if ((s32)r3 != 0) goto L_800ABDD8;
    r30 = 0x1;
L_800ABDD8:
    tmp = *(u32*)lbl_8047A8C0;
    if ((s32)tmp != 0) goto L_800ABED0;
    if ((s32)r30 == 0) goto L_800ABED0;
    r30 = 0xF0000000;
    OSDisableInterrupts();
    r6 = *(u32*)lbl_8047A8B8;
    r7 = 0x0;
    r5 = *(u32*)lbl_8047A8B0;
    r4 = 0x80000000;
    tmp = *(u32*)lbl_8047A8B4;
    r30 = r30 | r6;
    r6 = *(u32*)lbl_8047A8A8;
    r5 = r5 | tmp;
    tmp = *(u8*)((u8*)r4 + 0x30E3);
    r30 = r30 & ~r5;
    r5 = *(u32*)lbl_8047A8A4;
    r4 = r6 | r30;
    *(u32*)lbl_8047A8B8 = r7;
    tmp = tmp & 0x00000040;
    *(u32*)lbl_8047A8A8 = r4;
    r4 = r5 & ~r30;
    r31 = r3 + 0x0;
    r6 = *(u32*)lbl_8047A8A8;
    *(u32*)lbl_8047A8A4 = r4;
    r3 = r6 & r5;
    if ((s32)r30 != 0) goto L_800ABE54;
    tmp = *(u32*)lbl_8047A8AC;
    tmp = tmp | r30;
    *(u32*)lbl_8047A8AC = tmp;
L_800ABE54:
    fn_800D0464();
    tmp = *(u32*)lbl_80478A0C;
    if ((s32)tmp != 0x20) goto L_800ABEB8;
    r5 = *(u32*)lbl_8047A8A8;
    tmp = __cntlzw(r5);
    *(u32*)lbl_80478A0C = tmp;
    r4 = *(u32*)lbl_80478A0C;
    if ((s32)r4 == 0x20) goto L_800ABEB8;
    tmp = 0x80000000;
    tmp = (u32)tmp >> r4;
    tmp = r5 & ~tmp;
    r4 = r4 * 0xc;
    *(u32*)lbl_8047A8A8 = tmp;
    r3 = (u32)lbl_803FC5E0;
    tmp = (u32)lbl_803FC5E0;
    r3 = tmp + r4;
    r4 = 0x0;
    r5 = 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)fn_800AA8D4;
    r3 = *(u32*)lbl_80478A0C;
    r4 = (u32)fn_800AA8D4;
    fn_800D0CBC();
L_800ABEB8:
    r3 = r31;
    OSRestoreInterrupts(r3);
    tmp = 0x1;
    *(u32*)lbl_8047A8C0 = tmp;
    r3 = 0x0;
    goto L_800ABEE4;
L_800ABED0:
    r3 = r30;
    goto L_800ABEE4;
L_800ABED8:
    tmp = 0x0;
    *(u32*)lbl_8047A8C0 = tmp;
    r3 = 0x1;
L_800ABEE4:
    return;
}

/* fn_800ABEFC - 0x800ABEFC | size: 0x60 */
void fn_800ABEFC(void) {
    extern u8 lbl_8047A8BC[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r31 = 0;

    r31 = r4 + 0x0;
    tmp = *(u32*)lbl_8047A8BC;
    if (tmp == 0) goto L_800ABF48;
    r3 = r1 + 0x10;
    OSClearContext((OSContext*)r3);
    r3 = r1 + 0x10;
    OSSetCurrentContext((OSContext*)r3);
    r12 = *(u32*)lbl_8047A8BC;
    /* blrl  */;
    r3 = r1 + 0x10;
    OSClearContext((OSContext*)r3);
    r3 = r31;
    OSSetCurrentContext((OSContext*)r3);
L_800ABF48:
    return;
}

