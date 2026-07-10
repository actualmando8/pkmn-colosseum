/**
 * @file sdk_range_800AC02C.c
 * @brief dolphin-sdk code, 0x800AC02C - 0x800B71F0 (146 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/ai/AI.h"
#include "dolphin/ar/AR.h"
#include "dolphin/os/OSInterrupt.h"

typedef struct AIDmaRegisters {
    /* 0x00 */ u16 startHi;
    /* 0x02 */ u16 startLo;
    /* 0x04 */ u16 bytesLeft;
    /* 0x06 */ u16 control;
} AIDmaRegisters;

typedef struct AIRegisters {
    /* 0x00 */ u32 control;
    /* 0x04 */ u32 volume;
    /* 0x08 */ u32 sampleCount;
    /* 0x0C */ u32 interruptTiming;
} AIRegisters;

typedef struct DSPRegisters {
    /* 0x00 */ u16 mailToDspHi;
    /* 0x02 */ u16 mailToDspLo;
    /* 0x04 */ u16 mailFromDspHi;
    /* 0x06 */ u16 mailFromDspLo;
    /* 0x08 */ u16 control;
    /* 0x0A */ u16 dmaControl;
    /* 0x0C */ u16 arSize;
    /* 0x0E */ u16 _0e;
    /* 0x10 */ u16 arMode;
    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 _14;
    /* 0x16 */ u16 arRefresh;
    /* 0x18 */ u16 _18;
    /* 0x1A */ u16 aramSize;
    /* 0x1C */ u16 _1c;
    /* 0x1E */ u16 _1e;
    /* 0x20 */ u16 arDmaMainMemAddrHi;
    /* 0x22 */ u16 arDmaMainMemAddrLo;
    /* 0x24 */ u16 arDmaAramAddrHi;
    /* 0x26 */ u16 arDmaAramAddrLo;
    /* 0x28 */ u16 arDmaCntHi;
    /* 0x2A */ u16 arDmaCntLo;
    /* 0x2C */ u16 _2c;
    /* 0x2E */ u16 _2e;
    /* 0x30 */ u16 aiDmaStartHi;
    /* 0x32 */ u16 aiDmaStartLo;
    /* 0x34 */ u16 aiDmaBytesLeft;
    /* 0x36 */ u16 aiDmaControl;
} DSPRegisters;

typedef struct CARDControl {
    /* 0x000 */ s32 attached;
    /* 0x004 */ s32 result;
    /* 0x008 */ u8 _008[0x04];
    /* 0x00C */ u32 sectorSize;
    /* 0x010 */ u8 _010[0x70];
    /* 0x080 */ void* workArea;
    /* 0x084 */ void* dirBlock;
    /* 0x088 */ void* fatBlock;
    /* 0x08C */ u8 _08C[0x44];
    /* 0x0D0 */ void* apiCallback;
    /* 0x0D4 */ u8 _0D4[0x38];
    /* 0x10C */ void* diskId;
} CARDControl;

typedef struct CARDDirEntry {
    /* 0x00 */ u8 gameName[4];
    /* 0x04 */ u8 company[2];
    /* 0x06 */ u8 _06[2];
    /* 0x08 */ char fileName[32];
    /* 0x28 */ u32 time;
    /* 0x2C */ u32 iconAddr;
    /* 0x30 */ u16 iconFormat;
    /* 0x32 */ u16 animationSpeed;
    /* 0x34 */ u8 permission;
    /* 0x35 */ u8 copyTimes;
    /* 0x36 */ u8 _36[10];
} CARDDirEntry;

typedef struct CARDFileInfo {
    /* 0x00 */ s32 chan;
    /* 0x04 */ s32 fileNo;
    /* 0x08 */ u32 offset;
    /* 0x0C */ u32 length;
    /* 0x10 */ u16 startBlock;
} CARDFileInfo;

typedef struct GXTlutRegion {
    /* 0x00 */ u8 _00[0x10];
} GXTlutRegion;

typedef struct GXData {
    /* 0x000 */ u8 _000[0x2D0];
    /* 0x2D0 */ GXTlutRegion defaultTlutRegions[20];
} GXData;

#define DSP_REGS    ((volatile DSPRegisters*)0xCC005000)
#define AI_REGS     ((volatile AIRegisters*)0xCC006C00)

extern const char* lbl_80478A38;
extern AIDCallback lbl_8047A8CC;
extern ARCallback lbl_8047A908;
extern u32 lbl_8047A90C;
extern void* lbl_8047A928;
extern void* lbl_8047A930;
extern void* lbl_8047A938;
extern void* lbl_8047A93C;
extern void* lbl_8047A940;
extern void* lbl_8047A944;
extern u32 lbl_8047A948;
extern u32 lbl_8047A94C;
extern u32 lbl_8047A950;
extern u16 lbl_8047A970;
extern CARDControl lbl_803FC620[2];
extern u8 lbl_803FC840[];
extern GXData* gx;

extern s32 CARDCheckExAsync(s32 chan, s32* xferBytes, void* callback);
extern s32 CARDUnmount(s32 chan);
extern s32 __CARDFormatRegionAsync(s32 chan, u32 encode, void* callback);
extern s32 fn_800B57D0(s32 chan, s32 fileNo, CARDDirEntry* entry);
extern s32 fn_800B588C(s32 chan, s32 fileNo, CARDDirEntry* entry, void* callback);
extern void __ARQInterruptServiceRoutine(void);
extern void OSRegisterVersion(const char* version);
u32 __CARDGetFontEncode(void);
s32 __CARDPutControlBlock(CARDControl* card, s32 result);

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
    AIDCallback old = lbl_8047A8CC;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    lbl_8047A8CC = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

void AIStartDMA(void) {
    volatile DSPRegisters* dsp = DSP_REGS;

    dsp->aiDmaControl |= 0x8000;
}

void AIStopDMA(void) {
    volatile DSPRegisters* dsp = DSP_REGS;

    dsp->aiDmaControl &= ~0x8000;
}

u32 AIGetStreamPlayState(void) {
    return AI_REGS->control & 1;
}

u32 AIGetDSPSampleRate(void) {
    return ((AI_REGS->control >> 6) & 1) ^ 1;
}

u32 AIGetStreamSampleRate(void) {
    return (AI_REGS->control >> 1) & 1;
}

void AISetStreamVolLeft(u32 volume) {
    volatile AIRegisters* ai = AI_REGS;

    ai->volume = (ai->volume & ~0xff) | (volume & 0xff);
}

u32 AIGetStreamVolLeft(void) {
    return AI_REGS->volume & 0xff;
}

void AISetStreamVolRight(u32 volume) {
    volatile AIRegisters* ai = AI_REGS;

    ai->volume = (ai->volume & ~0xff00) | ((volume & 0xff) << 8);
}

u32 AIGetStreamVolRight(void) {
    return (AI_REGS->volume >> 8) & 0xff;
}

ARCallback ARRegisterDMACallback(ARCallback callback) {
    ARCallback old = lbl_8047A908;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    lbl_8047A908 = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

u32 ARGetBaseAddress(void) {
    return 0x4000;
}

u32 ARGetSize(void) {
    return lbl_8047A90C;
}

void __ARClearInterrupt(void) {
    volatile DSPRegisters* dsp = DSP_REGS;

    dsp->dmaControl = (dsp->dmaControl & ~0x88) | 0x20;
}

u32 __ARGetInterruptStatus(void) {
    return DSP_REGS->dmaControl & 0x20;
}

u32 ARGetDMAStatus(void) {
    BOOL enabled;
    u32 status;

    enabled = OSDisableInterrupts();
    status = DSP_REGS->dmaControl & 0x200;
    OSRestoreInterrupts(enabled);
    return status;
}

void __ARQCallbackHack(void) {
}

void ARQInit(void) {
    if (lbl_8047A94C != 1) {
        OSRegisterVersion(lbl_80478A38);
        lbl_8047A930 = NULL;
        lbl_8047A928 = NULL;
        lbl_8047A948 = 0x1000;
        ARRegisterDMACallback(__ARQInterruptServiceRoutine);
        lbl_8047A938 = NULL;
        lbl_8047A93C = NULL;
        lbl_8047A940 = NULL;
        lbl_8047A944 = NULL;
        lbl_8047A94C = 1;
    }
}

u32 ARQGetChunkSize(void) {
    return lbl_8047A948;
}

u32 fn_800AE794(void) {
    return (DSP_REGS->mailToDspHi >> 15) & 1;
}

u32 fn_800AE7A4(void) {
    return (DSP_REGS->mailFromDspHi >> 15) & 1;
}

u32 DSPReadMailFromDSP(void) {
    volatile DSPRegisters* dsp = DSP_REGS;

    return dsp->mailFromDspLo | (dsp->mailFromDspHi << 16);
}

void DSPSendMailToDSP(u32 mail) {
    volatile DSPRegisters* dsp = DSP_REGS;

    dsp->mailToDspHi = mail >> 16;
    dsp->mailToDspLo = mail;
}

void fn_800AE8A4(void) {
    volatile DSPRegisters* dsp = DSP_REGS;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    dsp->dmaControl = (dsp->dmaControl & ~0xa8) | 0x801;
    lbl_8047A950 = 0;
    OSRestoreInterrupts(enabled);
}

void fn_800AE8EC(void) {
    volatile DSPRegisters* dsp = DSP_REGS;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    dsp->dmaControl = (dsp->dmaControl & ~0xa8) | 4;
    OSRestoreInterrupts(enabled);
}

u32 fn_800AE92C(void) {
    return DSP_REGS->dmaControl & 0x200;
}

void __DSP_debug_printf(char* fmt, ...) {
}

void __CARDDefaultApiCallback(s32 chan, s32 result) {
}

s32 CARDCheckAsync(s32 chan, void* callback) {
    s32 xferBytes;

    return CARDCheckExAsync(chan, &xferBytes, callback);
}

s32 CARDFormatAsync(s32 chan, void* callback) {
    return __CARDFormatRegionAsync(chan, __CARDGetFontEncode(), callback);
}

u32 __CARDGetFontEncode(void) {
    return lbl_8047A970;
}

void __CARDSetDiskID(void* diskId) {
    CARDControl* card = lbl_803FC620;
    void* id;

    if (diskId != NULL) {
        id = diskId;
    } else {
        id = lbl_803FC840;
    }
    card[0].diskId = id;
    if (diskId == NULL) {
        diskId = lbl_803FC840;
    }
    card[1].diskId = diskId;
}

void* fn_800B01AC(s32 chan) {
    return lbl_803FC620[chan].diskId;
}

s32 fn_800B01C4(s32 chan, void* diskId) {
    CARDControl* card = &lbl_803FC620[chan];
    void* id;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    if (card->result == -1) {
        return -1;
    }
    if (diskId != NULL) {
        id = diskId;
    } else {
        id = (void*)0x80000000;
    }
    card->diskId = id;
    OSRestoreInterrupts(enabled);
    return 0;
}

s32 __CARDGetControlBlock(s32 chan, CARDControl** pcard) {
    s32 result;
    CARDControl* card = &lbl_803FC620[chan];
    BOOL enabled;

    if (chan < 0 || chan >= 2 || card->diskId == NULL) {
        return -128;
    }
    enabled = OSDisableInterrupts();
    if (card->attached == 0) {
        result = -3;
    } else if (card->result == -1) {
        result = -1;
    } else {
        card->result = -1;
        card->apiCallback = NULL;
        *pcard = card;
        result = 0;
    }
    OSRestoreInterrupts(enabled);
    return result;
}

s32 CARDClose(CARDFileInfo* fileInfo) {
    CARDControl* card;
    s32 result;

    result = __CARDGetControlBlock(fileInfo->chan, &card);
    if (result >= 0) {
        fileInfo->chan = -1;
        result = __CARDPutControlBlock(card, 0);
    }
    return result;
}

s32 __CARDPutControlBlock(CARDControl* card, s32 result) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    if (card->attached) {
        card->result = result;
    } else if (card->result == -1) {
        card->result = result;
    }
    OSRestoreInterrupts(enabled);
    return result;
}

BOOL OnReset_800C0734(BOOL final) {
    if (!final) {
        if (CARDUnmount(0) != -1) {
            if (CARDUnmount(1) != -1) {
                return TRUE;
            }
        }
        return FALSE;
    }
    return TRUE;
}

s32 CARDGetResultCode(s32 chan) {
    CARDControl* card;

    if (chan < 0 || chan >= 2) {
        return -128;
    }
    card = &lbl_803FC620[chan];
    return card->result;
}

void* __CARDGetFatBlock(CARDControl* card) {
    return card->fatBlock;
}

void* __CARDGetDirBlock(CARDControl* card) {
    return card->dirBlock;
}

BOOL __CARDCompareFileName(CARDDirEntry* entry, char* fileName) {
    char* entryName = entry->fileName;
    char entryChar;
    char c;
    s32 count = 32;

    while (--count >= 0) {
        entryChar = *entryName++;
        c = *fileName++;
        if (entryChar != c) {
            return FALSE;
        }
        if (c == 0) {
            return TRUE;
        }
    }
    if (*fileName == 0) {
        return TRUE;
    }
    return FALSE;
}

s32 fn_800B4308(CARDDirEntry* entry) {
    if (entry->gameName[0] == 0xff) {
        return -4;
    }
    if (entry->permission & 4) {
        return 0;
    }
    return -10;
}

BOOL __CARDIsOpened(void) {
    return FALSE;
}

s32 CARDGetAttributes(s32 chan, s32 fileNo, u8* attr) {
    s32 result;
    CARDDirEntry entry;

    result = fn_800B57D0(chan, fileNo, &entry);
    if (result == 0) {
        *attr = entry.permission;
    }
    return result;
}

s32 fn_800B5BE4(s32 chan, s32 fileNo, u8 attr, void* callback) {
    s32 result;
    CARDDirEntry entry;

    result = fn_800B57D0(chan, fileNo, &entry);
    if (result < 0) {
        return result;
    }
    entry.permission = attr;
    return fn_800B588C(chan, fileNo, &entry, callback);
}

GXTlutRegion* __GXDefaultTlutRegionCallback(u32 index) {
    GXTlutRegion* region;

    if (index >= 20) {
        region = NULL;
    } else {
        region = &gx->defaultTlutRegions[index];
    }
    return region;
}
