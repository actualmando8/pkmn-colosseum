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
