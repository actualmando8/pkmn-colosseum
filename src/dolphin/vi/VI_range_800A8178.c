/**
 * @file sdk_range_800A8178.c
 * @brief dolphin-sdk code, 0x800A8178 - 0x800AA280 (20 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/dvd/dvd.h"

BOOL DVDCompareDiskID(const DVDDiskID* id1, const DVDDiskID* id2) {
    extern s32 strncmp(const char* str1, const char* str2, u32 length);

    if (id1->gameName[0] != '\0' && id2->gameName[0] != '\0' &&
        strncmp(id1->gameName, id2->gameName, 4) != 0) {
        return FALSE;
    }

    if (id1->company[0] == '\0' || id2->company[0] == '\0' ||
        strncmp(id1->company, id2->company, 2) != 0) {
        return FALSE;
    }

    if (id1->diskNumber != 0xFF && id2->diskNumber != 0xFF &&
        id1->diskNumber != id2->diskNumber) {
        return FALSE;
    }

    if (id1->gameVersion != 0xFF && id2->gameVersion != 0xFF &&
        id1->gameVersion != id2->gameVersion) {
        return FALSE;
    }

    return TRUE;
}

void ShowMessage(void) {
    typedef struct GXColor {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColor;
    extern const GXColor lbl_8047C2D8;
    extern const GXColor lbl_8047C2DC;
    extern const char* lbl_804789E0;
    extern const char* lbl_804789E4;
    extern const char* lbl_8026F5F8[];
    extern u32 VIGetTvFormat(void);
    extern u16 fn_8009D820(void);
    extern u8 OSGetLanguage(void);
    extern void fn_8009CD38(GXColor foreground, GXColor background, const char* message);
    GXColor background = lbl_8047C2D8;
    GXColor foreground = lbl_8047C2DC;
    const char* message;

    if (VIGetTvFormat() == 0) {
        if (fn_8009D820() == 1) {
            message = lbl_804789E0;
        } else {
            message = lbl_804789E4;
        }
    } else {
        message = lbl_8026F5F8[OSGetLanguage()];
    }

    fn_8009CD38(foreground, background, message);
}

extern void (*FatalFunc_8047A830)(void);

BOOL DVDSetAutoFatalMessaging(BOOL enable) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    BOOL enabled;
    BOOL previous;

    enabled = OSDisableInterrupts();
    if (FatalFunc_8047A830 != 0) {
        previous = TRUE;
    } else {
        previous = FALSE;
    }
    FatalFunc_8047A830 = enable ? ShowMessage : 0;
    OSRestoreInterrupts(enabled);
    return previous;
}

#include "dolphin/types.h"

extern void (*FatalFunc_8047A830)(void);

void fn_800A836C(void) {
    if (FatalFunc_8047A830 != 0) {
        FatalFunc_8047A830();
    }
}

static void cb(s32 result, DVDCommandBlock* block) {
    typedef struct BB2 {
        u32 bootFilePosition;
        u32 fstPosition;
        u32 fstLength;
        u32 fstMaxLength;
        void* fstAddress;
    } BB2;
    extern s32 lbl_8047A838;
    extern BB2* bb2_8047A83C;
    extern DVDDiskID* idTmp_8047A840;
    extern BOOL DVDReadAbsAsyncForBS(DVDCommandBlock* block, void* addr, s32 length,
                                     s32 offset, DVDCBCallback callback);

    if (result > 0) {
        switch (lbl_8047A838) {
        case 0:
            lbl_8047A838 = 1;
            DVDReadAbsAsyncForBS(block, bb2_8047A83C, 0x20, 0x420, cb);
            break;
        case 1:
            lbl_8047A838 = 2;
            DVDReadAbsAsyncForBS(block, bb2_8047A83C->fstAddress,
                                 (bb2_8047A83C->fstLength + 0x1F) & ~0x1F,
                                 bb2_8047A83C->fstPosition, cb);
            break;
        }
    } else if (result == -1) {
    } else if (result == -4) {
        lbl_8047A838 = 0;
        DVDReset();
        DVDReadDiskID(block, idTmp_8047A840, cb);
    }
}

typedef void (*VIRetraceCallback)(u32 retraceCount);

VIRetraceCallback fn_800A880C(VIRetraceCallback callback) {
    extern VIRetraceCallback lbl_8047A85C;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    VIRetraceCallback previous = lbl_8047A85C;
    BOOL enabled = OSDisableInterrupts();

    lbl_8047A85C = callback;
    OSRestoreInterrupts(enabled);
    return previous;
}

VIRetraceCallback fn_800A8850(VIRetraceCallback callback) {
    extern VIRetraceCallback lbl_8047A860;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    VIRetraceCallback previous = lbl_8047A860;
    BOOL enabled = OSDisableInterrupts();

    lbl_8047A860 = callback;
    OSRestoreInterrupts(enabled);
    return previous;
}

void* fn_800A8894(u32 mode) {
    extern u8 lbl_803120E8[];
    u8* timing = lbl_803120E8;

    switch (mode) {
    case 0:
        return timing + 0x44;
    case 1:
        return timing + 0x6A;
    case 4:
        return timing + 0x90;
    case 5:
        return timing + 0xB6;
    case 20:
        return timing + 0x44;
    case 21:
        return timing + 0x6A;
    case 8:
        return timing + 0xDC;
    case 9:
        return timing + 0x102;
    case 2:
        return timing + 0x128;
    case 3:
        return timing + 0x14E;
    case 16:
        return timing + 0x90;
    case 17:
        return timing + 0xB6;
    case 24:
        return timing + 0x174;
    case 26:
        return timing + 0x19A;
    default:
        return 0;
    }
}

void VIWaitForRetrace(void) {
    extern u32 lbl_8047A84C;
    extern u32 lbl_8047A854;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern void OSSleepThread(void* queue);
    BOOL enabled;
    u32 count;

    enabled = OSDisableInterrupts();
    count = lbl_8047A84C;
    do {
        OSSleepThread(&lbl_8047A854);
    } while (count == lbl_8047A84C);
    OSRestoreInterrupts(enabled);
}

typedef struct VITiming {
    u8 equ;
    u16 acv;
    u16 prbOdd;
    u16 prbEven;
    u16 psbOdd;
    u16 psbEven;
    u8 bs1;
    u8 bs2;
    u8 bs3;
    u8 bs4;
    u16 be1;
    u16 be2;
    u16 be3;
    u16 be4;
    u16 nhlines;
    u16 hlw;
    u8 hsy;
    u8 hcs;
    u8 hce;
    u8 hbe640;
    u16 hbs640;
    u8 hbeCCIR656;
    u16 hbsCCIR656;
} VITiming;

typedef struct SomeVIStruct {
    u16 DispPosX;
    u16 DispPosY;
    u16 DispSizeX;
    u16 DispSizeY;
    u16 AdjustedDispPosX;
    u16 AdjustedDispPosY;
    u16 AdjustedDispSizeY;
    u16 AdjustedPanPosY;
    u16 AdjustedPanSizeY;
    u16 FBSizeX;
    u16 FBSizeY;
    u16 PanPosX;
    u16 PanPosY;
    u16 PanSizeX;
    u16 PanSizeY;
    u32 FBMode;
    u32 nonInter;
    u32 tv;
    u8 wordPerLine;
    u8 std;
    u8 wpl;
    u32 bufAddr;
    u32 tfbb;
    u32 bfbb;
    u8 xof;
    BOOL black;
    BOOL threeD;
    u32 rbufAddr;
    u32 rtfbb;
    u32 rbfbb;
    void* timing;
} SomeVIStruct;

/*
 * VI shadow state: regs[59] at 0x803FC488, shdwRegs[59] at +0x78,
 * HorVer at +0xF0.  Declared per-function to match this file's convention.
 */
#define VI_CONTEXT_DECL                                                        \
    typedef struct VIContext {                                                 \
        volatile u16 viRegs[59];                                                 \
        u8 _76[2];                                                             \
        volatile u16 viShdwRegs[59];                                             \
        u8 _EE[2];                                                             \
        SomeVIStruct HorVer;                                                   \
    } VIContext;                                                               \
    extern VIContext lbl_803FC488;                                             \
    extern volatile u64 lbl_8047A870

#define regs lbl_803FC488.viRegs
#define shdwRegs lbl_803FC488.viShdwRegs
#define changed lbl_8047A870
#define MARK_CHANGED(index) (changed |= 1LL << (63 - (index)))

static void calcFbbs(u32 bufAddr, u16 panPosX, u16 panPosY, u8 wordPerLine,
                     u32 xfbMode, u16 dispPosY, u32* tfbb, u32* bfbb) {
    u32 bytesPerLine;
    u32 xoffInWords;
    u32 tmp;

    xoffInWords = (panPosX & ~0xF) >> 4;
    bytesPerLine = (wordPerLine & 0xFF) << 5;
    *tfbb = bufAddr + (xoffInWords << 5) + (bytesPerLine * panPosY);
    *bfbb = (xfbMode == 0) ? *tfbb : *tfbb + bytesPerLine;
    if (dispPosY % 2 == 1) {
        tmp = *tfbb;
        *tfbb = *bfbb;
        *bfbb = tmp;
    }
    *tfbb &= 0x3FFFFFFF;
    *bfbb &= 0x3FFFFFFF;
}

void setFbbRegs(void* horVer, u32* tfbb, u32* bfbb, u32* rtfbb, u32* rbfbb) {
    VI_CONTEXT_DECL;
    SomeVIStruct* HorVer = horVer;
    u32 shifted;

    calcFbbs(HorVer->bufAddr, HorVer->PanPosX, HorVer->AdjustedPanPosY,
             HorVer->wordPerLine, HorVer->FBMode, HorVer->AdjustedDispPosY, tfbb,
             bfbb);
    if (HorVer->threeD) {
        calcFbbs(HorVer->rbufAddr, HorVer->PanPosX, HorVer->AdjustedPanPosY,
                 HorVer->wordPerLine, HorVer->FBMode, HorVer->AdjustedDispPosY,
                 rtfbb, rbfbb);
    }

    if (*tfbb < 0x01000000U && *bfbb < 0x01000000U && *rtfbb < 0x01000000U &&
        *rbfbb < 0x01000000U) {
        shifted = 0;
    } else {
        shifted = 1;
    }

    if (shifted) {
        *tfbb >>= 5;
        *bfbb >>= 5;
        *rtfbb >>= 5;
        *rbfbb >>= 5;
    }

    regs[15] = (u16)*tfbb & 0xFFFF;
    MARK_CHANGED(15);
    regs[14] = (shifted << 12) | ((*tfbb >> 16) | (HorVer->xof << 8));
    MARK_CHANGED(14);
    regs[19] = (u16)*bfbb & 0xFFFF;
    MARK_CHANGED(19);
    regs[18] = (*bfbb >> 16);
    MARK_CHANGED(18);

    if (HorVer->threeD) {
        regs[17] = (u16)*rtfbb & 0xFFFF;
        MARK_CHANGED(17);
        regs[16] = *rtfbb >> 16;
        MARK_CHANGED(16);
        regs[21] = (u16)*rbfbb & 0xFFFF;
        MARK_CHANGED(21);
        regs[20] = *rbfbb >> 16;
        MARK_CHANGED(20);
    }
}

void setVerticalRegs(u16 dispPosY, u16 dispSizeY, u8 equ, u16 acv, u16 prbOdd,
                     u16 prbEven, u16 psbOdd, u16 psbEven, BOOL black) {
    VI_CONTEXT_DECL;
    u16 actualPrbOdd;
    u16 actualPrbEven;
    u16 actualPsbOdd;
    u16 actualPsbEven;
    u16 actualAcv;
    u16 c;
    u16 d;

    if (regs[54] & 1) {
        c = 1;
        d = 2;
    } else {
        c = 2;
        d = 1;
    }

    if ((dispPosY % 2) == 0) {
        actualPrbOdd = prbOdd + (d * dispPosY);
        actualPsbOdd = psbOdd + (d * (((c * acv) - dispSizeY) - dispPosY));
        actualPrbEven = prbEven + (d * dispPosY);
        actualPsbEven = psbEven + (d * (((c * acv) - dispSizeY) - dispPosY));
    } else {
        actualPrbOdd = prbEven + (d * dispPosY);
        actualPsbOdd = psbEven + (d * (((c * acv) - dispSizeY) - dispPosY));
        actualPrbEven = prbOdd + (d * dispPosY);
        actualPsbEven = psbOdd + (d * (((c * acv) - dispSizeY) - dispPosY));
    }

    actualAcv = dispSizeY / c;

    if (black) {
        actualPrbOdd += 2 * actualAcv - 2;
        actualPsbOdd += 2;
        actualPrbEven += 2 * actualAcv - 2;
        actualPsbEven += 2;
        actualAcv = 0;
    }

    regs[0] = equ | (actualAcv << 4);
    MARK_CHANGED(0);
    regs[7] = (u16)(u32)actualPrbOdd;
    MARK_CHANGED(7);
    regs[6] = (u16)(u32)actualPsbOdd;
    MARK_CHANGED(6);
    regs[9] = (u16)(u32)actualPrbEven;
    MARK_CHANGED(9);
    regs[8] = (u16)(u32)actualPsbEven;
    MARK_CHANGED(8);
}

#define VI_MIN(a, b) ((a) < (b) ? (a) : (b))
#define VI_MAX(a, b) ((a) > (b) ? (a) : (b))
#define VI_CLAMP(val, min, max)                                                \
    ((val) > (max) ? (max) : (val) < (min) ? (min) : (val))

static void AdjustPosition(u16 acv) {
    VI_CONTEXT_DECL;
    extern s16 lbl_8047A868; /* displayOffsetH */
    extern s16 lbl_8047A86A; /* displayOffsetV */
    SomeVIStruct* HorVer = &lbl_803FC488.HorVer;
    s32 coeff;
    s32 frac;

    HorVer->AdjustedDispPosX = VI_CLAMP((s16)HorVer->DispPosX + lbl_8047A868, 0,
                                        0x2D0 - HorVer->DispSizeX);
    coeff = (HorVer->FBMode == 0) ? 2 : 1;
    frac = HorVer->DispPosY & 1;
    HorVer->AdjustedDispPosY =
        VI_MAX((s16)HorVer->DispPosY + lbl_8047A86A, frac);
    HorVer->AdjustedDispSizeY =
        HorVer->DispSizeY +
        VI_MIN((s16)HorVer->DispPosY + lbl_8047A86A - frac, 0) -
        VI_MAX((s16)HorVer->DispPosY + (s16)HorVer->DispSizeY + lbl_8047A86A -
                   (((s16)acv * 2) - frac),
               0);
    HorVer->AdjustedPanPosY =
        HorVer->PanPosY -
        (VI_MIN((s16)HorVer->DispPosY + lbl_8047A86A - frac, 0) / coeff);
    HorVer->AdjustedPanSizeY =
        HorVer->PanSizeY +
        (VI_MIN((s16)HorVer->DispPosY + lbl_8047A86A - frac, 0) / coeff) -
        (VI_MAX((s16)HorVer->DispPosY + (s16)HorVer->DispSizeY + lbl_8047A86A -
                    (((s16)acv * 2) - frac),
                0) /
         coeff);
}

static void setScalingRegs(u16 panSizeX, u16 dispSizeX, BOOL threeD) {
    VI_CONTEXT_DECL;
    u32 scale;

    panSizeX = threeD ? (panSizeX << 1) : panSizeX;
    if (panSizeX < dispSizeX) {
        scale = (u32)(dispSizeX + (panSizeX << 8) - 1) / dispSizeX;
        regs[37] = scale | 0x1000;
        changed |= 0x04000000;
        regs[56] = (u32)panSizeX;
        changed |= 0x80;
    } else {
        regs[37] = 0x100;
        changed |= 0x04000000;
    }
}

static void setPicConfig(u16 fbSizeX, u32 xfbMode, u16 panPosX, u16 panSizeX,
                         u8* wordPerLine, u8* std, u8* wpl, u8* xof) {
    VI_CONTEXT_DECL;

    *wordPerLine = (fbSizeX + 15) / 16;
    *std = (xfbMode == 0) ? *wordPerLine : (u8)(*wordPerLine * 2);
    *xof = panPosX % 16;
    *wpl = (*xof + panSizeX + 15) / 16;
    regs[0x24] = *std | (*wpl << 8);
    changed |= 0x8000000;
}

void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height) {
    VI_CONTEXT_DECL;
    extern u32 lbl_8047A898; /* FBSet */
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    SomeVIStruct* HorVer = &lbl_803FC488.HorVer;
    BOOL enabled;
    VITiming* tm;

    enabled = OSDisableInterrupts();
    HorVer->PanPosX = xOrg;
    HorVer->PanPosY = yOrg;
    HorVer->PanSizeX = width;
    HorVer->PanSizeY = height;
    HorVer->DispSizeY = (HorVer->nonInter == 2)   ? HorVer->PanSizeY
                        : (HorVer->nonInter == 3) ? HorVer->PanSizeY
                        : (HorVer->FBMode == 0)   ? (u16)(HorVer->PanSizeY * 2)
                                                  : HorVer->PanSizeY;
    tm = HorVer->timing;
    AdjustPosition(tm->acv);
    setScalingRegs(HorVer->PanSizeX, HorVer->DispSizeX, HorVer->threeD);
    setPicConfig(HorVer->FBSizeX, HorVer->FBMode, HorVer->PanPosX,
                 HorVer->PanSizeX, &HorVer->wordPerLine, &HorVer->std,
                 &HorVer->wpl, &HorVer->xof);
    if (lbl_8047A898 != 0) {
        setFbbRegs(HorVer, &HorVer->tfbb, &HorVer->bfbb, &HorVer->rtfbb,
                   &HorVer->rbfbb);
    }
    setVerticalRegs(HorVer->AdjustedDispPosY, HorVer->DispSizeY, tm->equ,
                    tm->acv, tm->prbOdd, tm->prbEven, tm->psbOdd, tm->psbEven,
                    HorVer->black);
    OSRestoreInterrupts(enabled);
}

static s32 cntlzd(u64 bit) {
    u32 hi;
    u32 lo;
    s32 value;

    hi = bit >> 32;
    lo = bit & 0xFFFFFFFF;
    value = __cntlzw(hi);
    if (value < 32) {
        return value;
    }
    return __cntlzw(lo) + 32;
}

void VIFlush(void) {
    VI_CONTEXT_DECL;
    extern volatile u32 lbl_8047A850;
    extern volatile u32 lbl_8047A86C;
    extern volatile u32 lbl_8047A878;
    extern volatile u64 lbl_8047A880;
    extern u32 lbl_8047A890;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    BOOL enabled;
    s32 regIndex;

    enabled = OSDisableInterrupts();
    lbl_8047A878 |= lbl_8047A86C;
    lbl_8047A86C = 0;
    lbl_8047A880 |= changed;

    while (changed != 0) {
        regIndex = cntlzd(changed);
        shdwRegs[regIndex] = regs[regIndex];
        changed &= ~((u64)1 << (63 - regIndex));
    }

    lbl_8047A850 = 1;
    lbl_8047A890 = lbl_803FC488.HorVer.bufAddr;
    OSRestoreInterrupts(enabled);
}

void VISetNextFrameBuffer(void* fb) {
    typedef struct VIContext {
        u8 _00[0xF0];
        u8 horVer[0x30];
        void* bufAddr;
        u32 fbb;
        u32 tfbb;
        u8 _12C[0x10];
        u32 bfbb;
        u32 btfbb;
    } VIContext;
    extern VIContext lbl_803FC488;
    extern u32 lbl_8047A898;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern void setFbbRegs(void* horVer, u32* fbb, u32* tfbb, u32* bfbb, u32* btfbb);
    VIContext* context = &lbl_803FC488;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    context->bufAddr = fb;
    lbl_8047A898 = TRUE;
    setFbbRegs(context->horVer, &context->fbb, &context->tfbb,
               &context->bfbb, &context->btfbb);
    OSRestoreInterrupts(enabled);
}

void VISetBlack(BOOL black) {
    typedef struct VITiming {
        u8 equ;
        u16 acv;
        u16 prbOdd;
        u16 prbEven;
        u16 psbOdd;
        u16 psbEven;
    } VITiming;
    typedef struct VIContext {
        u8 _00[0xF6];
        u16 dispSizeY;
        u8 _F8[2];
        u16 dispPosY;
        u8 _FC[0x34];
        BOOL black;
        u8 _134[0x10];
        VITiming* timing;
    } VIContext;
    extern VIContext lbl_803FC488;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern void setVerticalRegs(u16 dispPosY, u16 dispSizeY, u8 equ, u16 acv,
                                u16 prbOdd, u16 prbEven, u16 psbOdd, u16 psbEven,
                                BOOL black);
    VIContext* context = &lbl_803FC488;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    context->black = black;
    setVerticalRegs(context->dispPosY, context->dispSizeY, context->timing->equ,
                    context->timing->acv, context->timing->prbOdd,
                    context->timing->prbEven, context->timing->psbOdd,
                    context->timing->psbEven, context->black);
    OSRestoreInterrupts(enabled);
}
