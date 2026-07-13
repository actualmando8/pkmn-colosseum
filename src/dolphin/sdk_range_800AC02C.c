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
#include "dolphin/exi/EXI.h"
#include "dolphin/os/OSAlarm.h"
#include "dolphin/os/OSInterrupt.h"

typedef void (*CARDCallback)(s32 chan, s32 result);

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
    /* 0x010 */ u8 _010[0x14];
    /* 0x024 */ s32 field_24;
    /* 0x028 */ u8 _028[0x08];
    /* 0x030 */ u8 task[0x50];
    /* 0x080 */ void* workArea;
    /* 0x084 */ void* dirBlock;
    /* 0x088 */ void* fatBlock;
    /* 0x08C */ u8 _08C[0x08];
    /* 0x094 */ u8 cmd[5];
    /* 0x099 */ u8 _099[0x07];
    /* 0x0A0 */ s32 cmdLen;
    /* 0x0A4 */ s32 field_A4;
    /* 0x0A8 */ s32 field_A8;
    /* 0x0AC */ s32 repeat;
    /* 0x0B0 */ u32 addr;
    /* 0x0B4 */ u32 length;
    /* 0x0B8 */ u8* buffer;
    /* 0x0BC */ u8 _0BC[0x08];
    /* 0x0C4 */ CARDCallback extCallback;
    /* 0x0C8 */ CARDCallback txCallback;
    /* 0x0CC */ CARDCallback callback_CC;
    /* 0x0D0 */ CARDCallback apiCallback;
    /* 0x0D4 */ CARDCallback xferCallback;
    /* 0x0D8 */ CARDCallback updateCallback;
    /* 0x0DC */ CARDCallback unlockCallback;
    /* 0x0E0 */ OSAlarm alarm;
    /* 0x108 */ u8 _108[0x04];
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

typedef struct GXFifoObj {
    /* 0x00 */ u8* base;
    /* 0x04 */ u8* top;
    /* 0x08 */ u32 size;
    /* 0x0C */ u32 hiWatermark;
    /* 0x10 */ u32 loWatermark;
    /* 0x14 */ void* rdPtr;
    /* 0x18 */ void* wrPtr;
    /* 0x1C */ s32 count;
    /* 0x20 */ u8 wrap;
    /* 0x21 */ u8 _21[3];
} GXFifoObj;

typedef struct GXData {
    /* 0x000 */ u8 _000[0x2D0];
    /* 0x2D0 */ GXTlutRegion defaultTlutRegions[20];
} GXData;

#define DSP_REGS    ((volatile DSPRegisters*)0xCC005000)
#define AI_REGS     ((volatile AIRegisters*)0xCC006C00)

#define AT_ADDRESS(addr) : addr

volatile u16 __DSPRegs[32] AT_ADDRESS(0xCC005000);
volatile u32 __AIRegs[4] AT_ADDRESS(0xCC006C00);

extern const char* lbl_80478A30;
extern const char* lbl_80478A38;
extern const char* lbl_80478A40;
extern const char* lbl_80478A48;
extern volatile u32 lbl_80478A50;
extern AISCallback lbl_8047A8C8;
extern AIDCallback lbl_8047A8CC;
extern void* lbl_8047A8D0;
extern void* lbl_8047A8D4;
extern BOOL lbl_8047A8DC;
extern ARCallback lbl_8047A908;
extern u32 lbl_8047A90C;
extern u32 lbl_8047A918;
extern u32 lbl_8047A91C;
extern u32* lbl_8047A920;
extern s32 lbl_8047A924;
typedef struct ARQRequest ARQRequest;
typedef void (*ARQCallback)(ARQRequest* request);
struct ARQRequest {
    /* 0x00 */ ARQRequest* next;
    /* 0x04 */ u32 owner;
    /* 0x08 */ u32 type;
    /* 0x0C */ u32 priority;
    /* 0x10 */ u32 source;
    /* 0x14 */ u32 dest;
    /* 0x18 */ u32 length;
    /* 0x1C */ ARQCallback callback;
};
extern ARQRequest* lbl_8047A928;
extern ARQRequest* lbl_8047A930;
extern ARQRequest* lbl_8047A938;
extern ARQRequest* lbl_8047A93C;
extern ARQCallback lbl_8047A940;
extern ARQCallback lbl_8047A944;
extern u32 lbl_8047A948;
extern s32 lbl_8047A94C;
extern s32 lbl_8047A950;
extern u32 lbl_8047A960;
extern u16 lbl_8047A970;

typedef struct DSPTaskInfo DSPTaskInfo;
typedef void (*DSPCallback)(void* task);
struct DSPTaskInfo {
    /* 0x00 */ u32 state;
    /* 0x04 */ u32 priority;
    /* 0x08 */ u32 flags;
    /* 0x0C */ u16* iram_mmem_addr;
    /* 0x10 */ u32 iram_length;
    /* 0x14 */ u32 iram_addr;
    /* 0x18 */ u16* dram_mmem_addr;
    /* 0x1C */ u32 dram_length;
    /* 0x20 */ u32 dram_addr;
    /* 0x24 */ u16 dsp_init_vector;
    /* 0x26 */ u16 dsp_resume_vector;
    /* 0x28 */ DSPCallback init_cb;
    /* 0x2C */ DSPCallback res_cb;
    /* 0x30 */ DSPCallback done_cb;
    /* 0x34 */ DSPCallback req_cb;
    /* 0x38 */ DSPTaskInfo* next;
    /* 0x3C */ DSPTaskInfo* prev;
    /* 0x40 */ u8 _40[0x10];
};
extern DSPTaskInfo* lbl_8047A964; /* __DSP_last_task */
extern DSPTaskInfo* lbl_8047A968; /* __DSP_first_task */
extern DSPTaskInfo* lbl_8047A96C; /* __DSP_curr_task */
extern CARDControl lbl_803FC620[2];
extern u8 lbl_803FC840[];
extern GXData* gx;
extern u8 lbl_803125E8[];
extern u8 lbl_803127F0[];
extern u16 lbl_80478A58;
extern u32 lbl_80312960[];

extern s32 CARDCheckExAsync(s32 chan, s32* xferBytes, void* callback);
extern s32 CARDUnmount(s32 chan);
extern s32 __CARDFormatRegionAsync(s32 chan, u32 encode, void* callback);
extern s32 fn_800B57D0(s32 chan, s32 fileNo, CARDDirEntry* entry);
extern s32 fn_800B588C(s32 chan, s32 fileNo, CARDDirEntry* entry, void* callback);
extern void __ARQInterruptServiceRoutine(void);
extern void __ARHandler(__OSInterrupt interrupt, OSContext* context);
extern void __ARChecksize(void);
extern void __DSPHandler(__OSInterrupt interrupt, OSContext* context);
extern void __DSP_boot_task(DSPTaskInfo* task);
extern void __DSP_insert_task(DSPTaskInfo* task);
extern void __DSP_debug_printf(char* fmt, ...);
extern void fn_8009870C(s32 chan, s32 value);
extern BOOL fn_80098944(s32 chan);
extern void fn_80098AE8(s32 chan);
extern void OSRegisterVersion(const char* version);
u32 __CARDGetFontEncode(void);
s32 __CARDPutControlBlock(CARDControl* card, s32 result);
s32 fn_800AFBDC(s32 chan, void* buf, void* callback);
s32 fn_800AFFE0(s32 chan, u32 addr, void* callback);

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
    AIDCallback old = lbl_8047A8CC;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    lbl_8047A8CC = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

void AIInitDMA(u32 addr, u32 length) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    __DSPRegs[24] = (__DSPRegs[24] & ~0x3ff) | (addr >> 16);
    __DSPRegs[25] = (__DSPRegs[25] & ~0xffe0) | (addr & 0xffff);
    __DSPRegs[27] = (__DSPRegs[27] & ~0x7fff) | (u16)(length >> 5);
    OSRestoreInterrupts(enabled);
}

void AIStartDMA(void) {
    __DSPRegs[27] |= 0x8000;
}

void AIStopDMA(void) {
    __DSPRegs[27] &= ~0x8000;
}

u32 AIGetStreamPlayState(void);
u32 AIGetStreamSampleRate(void);
u32 AIGetStreamVolLeft(void);
u32 AIGetStreamVolRight(void);
void AISetStreamVolLeft(u32 volume);
void AISetStreamVolRight(u32 volume);
void __AI_SRC_INIT(void);

void AISetStreamPlayState(u32 state) {
    BOOL enabled;
    u32 volumeRight;
    u32 volumeLeft;

    if (state != AIGetStreamPlayState()) {
        if (AIGetStreamSampleRate() == 0 && state == 1) {
            volumeRight = AIGetStreamVolRight();
            volumeLeft = AIGetStreamVolLeft();
            AISetStreamVolRight(0);
            AISetStreamVolLeft(0);
            enabled = OSDisableInterrupts();
            __AI_SRC_INIT();
            __AIRegs[0] = (__AIRegs[0] & ~0x20) | 0x20;
            __AIRegs[0] = (__AIRegs[0] & ~1) | 1;
            OSRestoreInterrupts(enabled);
            AISetStreamVolLeft(volumeRight);
            AISetStreamVolRight(volumeLeft);
        } else {
            __AIRegs[0] = (__AIRegs[0] & ~1) | state;
        }
    }
}

u32 AIGetStreamPlayState(void) {
    return AI_REGS->control & 1;
}

u32 AIGetDSPSampleRate(void);

void AISetDSPSampleRate(u32 rate) {
    BOOL enabled;
    u32 oldVolL;
    u32 oldVolR;
    u32 oldStreamPlay;
    u32 oldStreamRate;

    if (rate == AIGetDSPSampleRate()) {
        return;
    }
    AI_REGS->control &= ~0x40;
    if (rate != 0) {
        return;
    }

    oldVolL = AIGetStreamVolLeft();
    oldVolR = AIGetStreamVolRight();
    oldStreamPlay = AIGetStreamPlayState();
    oldStreamRate = AIGetStreamSampleRate();

    AISetStreamVolLeft(0);
    AISetStreamVolRight(0);
    enabled = OSDisableInterrupts();
    __AI_SRC_INIT();
    AI_REGS->control = (AI_REGS->control & ~0x20) | 0x20;
    AI_REGS->control = (AI_REGS->control & ~0x2) | (oldStreamRate << 1);
    AI_REGS->control = (AI_REGS->control & ~0x1) | oldStreamPlay;
    AI_REGS->control |= 0x40;
    OSRestoreInterrupts(enabled);
    AISetStreamVolLeft(oldVolL);
    AISetStreamVolRight(oldVolR);
}

u32 AIGetDSPSampleRate(void) {
    return ((AI_REGS->control >> 6) & 1) ^ 1;
}

void __AI_set_stream_sample_rate(u32 rate) {
    BOOL enabled;
    u32 playState;
    u32 volumeLeft;
    u32 volumeRight;
    u32 dspSampleRate;

    if (rate != AIGetStreamSampleRate()) {
        playState = AIGetStreamPlayState();
        volumeLeft = AIGetStreamVolLeft();
        volumeRight = AIGetStreamVolRight();
        AISetStreamVolRight(0);
        AISetStreamVolLeft(0);
        dspSampleRate = __AIRegs[0] & 0x40;
        __AIRegs[0] &= ~0x40;
        enabled = OSDisableInterrupts();
        __AI_SRC_INIT();
        __AIRegs[0] |= dspSampleRate;
        __AIRegs[0] = (__AIRegs[0] & ~0x20) | 0x20;
        __AIRegs[0] = (__AIRegs[0] & ~2) | (rate << 1);
        OSRestoreInterrupts(enabled);
        AISetStreamPlayState(playState);
        AISetStreamVolLeft(volumeLeft);
        AISetStreamVolRight(volumeRight);
    }
}

u32 AIGetStreamSampleRate(void) {
    return (AI_REGS->control >> 1) & 1;
}

void AISetStreamVolLeft(u32 volume) {
    __AIRegs[1] = (__AIRegs[1] & ~0xff) | (volume & 0xff);
}

u32 AIGetStreamVolLeft(void) {
    return AI_REGS->volume & 0xff;
}

void AISetStreamVolRight(u32 volume) {
    __AIRegs[1] = (__AIRegs[1] & ~0xff00) | ((volume & 0xff) << 8);
}

u32 AIGetStreamVolRight(void) {
    return (AI_REGS->volume >> 8) & 0xff;
}

void __AISHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;

    __AIRegs[0] |= 8;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (lbl_8047A8C8 != NULL) {
        lbl_8047A8C8(__AIRegs[2]);
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

void __AICallbackStackSwitch(AIDCallback callback);

void __AIDHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;
    u16 tmp;

    tmp = __DSPRegs[5];
    __DSPRegs[5] = (tmp & ~0xa0) | 8;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (lbl_8047A8CC != NULL && lbl_8047A8DC == FALSE) {
        lbl_8047A8DC = TRUE;
        if (lbl_8047A8D0 != NULL) {
            __AICallbackStackSwitch(lbl_8047A8CC);
        } else {
            lbl_8047A8CC();
        }
        lbl_8047A8DC = FALSE;
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

void __AICallbackStackSwitch(AIDCallback callback) {
    *(void**)0x8047A8D4 = (void*)OSGetStackPointer();
    OSSwitchFiber((u32)callback, *(u32*)0x8047A8D0);
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

void __ARHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;
    u16 tmp;

    tmp = __DSPRegs[5];
    __DSPRegs[5] = (tmp & ~0x88) | 0x20;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (lbl_8047A908 != NULL) {
        lbl_8047A908();
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}

void __ARClearInterrupt(void) {
    u16 tmp = __DSPRegs[5];

    __DSPRegs[5] = (tmp & ~0x88) | 0x20;
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

#pragma dont_inline on
void ARStartDMA(u32 type, u32 mainMemoryAddress, u32 aramAddress, u32 length) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    __DSPRegs[16] = (__DSPRegs[16] & ~0x3ff) | (mainMemoryAddress >> 16);
    __DSPRegs[17] = (__DSPRegs[17] & ~0xffe0) | (u16)mainMemoryAddress;
    __DSPRegs[18] = (__DSPRegs[18] & ~0x3ff) | (aramAddress >> 16);
    __DSPRegs[19] = (__DSPRegs[19] & ~0xffe0) | (u16)aramAddress;
    __DSPRegs[20] = (__DSPRegs[20] & ~0x8000) | (type << 15);
    __DSPRegs[20] = (__DSPRegs[20] & ~0x3ff) | (length >> 16);
    __DSPRegs[21] = (__DSPRegs[21] & ~0xffe0) | (u16)length;
    OSRestoreInterrupts(enabled);
}
#pragma dont_inline off

u32 ARInit(u32* stack, u32 stackSize) {
    BOOL enabled;

    if (lbl_8047A924 == 1) {
        return 0x4000;
    }
    OSRegisterVersion(lbl_80478A30);
    enabled = OSDisableInterrupts();
    lbl_8047A908 = NULL;
    __OSSetInterruptHandler(6, __ARHandler);
    __OSUnmaskInterrupts(0x02000000);
    lbl_8047A91C = stackSize;
    lbl_8047A918 = 0x4000;
    lbl_8047A920 = stack;
    __DSPRegs[13] = (__DSPRegs[13] & 0xff) | (__DSPRegs[13] & ~0xff);
    __ARChecksize();
    lbl_8047A924 = 1;
    OSRestoreInterrupts(enabled);
    return lbl_8047A918;
}

void __ARQServiceQueueLo(void) {
    ARQRequest* request;

    if (lbl_8047A93C == NULL && lbl_8047A930 != NULL) {
        lbl_8047A93C = lbl_8047A930;
        lbl_8047A930 = lbl_8047A930->next;
    }

    request = lbl_8047A93C;
    if (request != NULL) {
        if (request->length <= lbl_8047A948) {
            if (request->type == 0) {
                ARStartDMA(request->type, request->source, request->dest, request->length);
            } else {
                ARStartDMA(request->type, request->dest, request->source, request->length);
            }
            lbl_8047A944 = lbl_8047A93C->callback;
        } else if (request->type == 0) {
            ARStartDMA(request->type, request->source, request->dest, lbl_8047A948);
        } else {
            ARStartDMA(request->type, request->dest, request->source, lbl_8047A948);
        }
        lbl_8047A93C->length -= lbl_8047A948;
        lbl_8047A93C->source += lbl_8047A948;
        lbl_8047A93C->dest += lbl_8047A948;
    }
}

void __ARQCallbackHack(void) {
}

void __ARQInterruptServiceRoutine(void) {
    ARQRequest* request;

    if (lbl_8047A940 != NULL) {
        lbl_8047A940(lbl_8047A938);
        lbl_8047A938 = NULL;
        lbl_8047A940 = NULL;
    } else if (lbl_8047A944 != NULL) {
        lbl_8047A944(lbl_8047A93C);
        lbl_8047A93C = NULL;
        lbl_8047A944 = NULL;
    }

    request = lbl_8047A928;
    if (request != NULL) {
        if (request->type == 0) {
            ARStartDMA(request->type, request->source, request->dest, request->length);
        } else {
            ARStartDMA(request->type, request->dest, request->source, request->length);
        }
        lbl_8047A940 = lbl_8047A928->callback;
        lbl_8047A938 = lbl_8047A928;
        lbl_8047A928 = lbl_8047A928->next;
    }
    if (lbl_8047A938 == NULL) {
        __ARQServiceQueueLo();
    }
}

#pragma dont_inline on
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
#pragma dont_inline off

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
    volatile u16* dsp = __DSPRegs;

    return ((u32)dsp[2] << 16) | dsp[3];
}

void DSPSendMailToDSP(u32 mail) {
    volatile u16* dsp = __DSPRegs;

    dsp[0] = mail >> 16;
    dsp[1] = mail;
}

void DSPInit(void) {
    BOOL enabled;
    u16 tmp;

    __DSP_debug_printf((char*)&lbl_803125E8[0x48], &lbl_803125E8[0x68], &lbl_803125E8[0x74]);
    if (lbl_8047A950 != 1) {
        OSRegisterVersion(lbl_80478A40);
        enabled = OSDisableInterrupts();
        __OSSetInterruptHandler(7, __DSPHandler);
        __OSUnmaskInterrupts(0x01000000);
        __DSPRegs[5] = (__DSPRegs[5] & ~0xa8) | 0x800;
        tmp = __DSPRegs[5];
        __DSPRegs[5] = tmp & ~0xac;
        lbl_8047A960 = 0;
        lbl_8047A96C = NULL;
        lbl_8047A964 = NULL;
        lbl_8047A968 = NULL;
        lbl_8047A950 = 1;
        OSRestoreInterrupts(enabled);
    }
}

void fn_800AE8A4(void) {
    BOOL enabled;
    u16 tmp;

    enabled = OSDisableInterrupts();
    tmp = __DSPRegs[5];
    __DSPRegs[5] = (tmp & ~0xa8) | 0x801;
    lbl_8047A950 = 0;
    OSRestoreInterrupts(enabled);
}

void fn_800AE8EC(void) {
    BOOL enabled;
    u16 tmp;

    enabled = OSDisableInterrupts();
    tmp = __DSPRegs[5];
    __DSPRegs[5] = (tmp & ~0xa8) | 4;
    OSRestoreInterrupts(enabled);
}

u32 fn_800AE92C(void) {
    return DSP_REGS->dmaControl & 0x200;
}

DSPTaskInfo* DSPAddTask(DSPTaskInfo* task) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    __DSP_insert_task(task);
    task->state = 0;
    task->flags = 1;
    OSRestoreInterrupts(enabled);
    if (task == lbl_8047A968) {
        __DSP_boot_task(task);
    }
    return task;
}

void __DSP_debug_printf(char* fmt, ...) {
}

void __DSP_insert_task(DSPTaskInfo* task) {
    DSPTaskInfo* current;

    if (lbl_8047A968 == NULL) {
        lbl_8047A96C = task;
        lbl_8047A964 = task;
        lbl_8047A968 = task;
        task->prev = NULL;
        task->next = NULL;
        return;
    }

    current = lbl_8047A968;
    while (current != NULL) {
        if (task->priority < current->priority) {
            task->prev = current->prev;
            current->prev = task;
            task->next = current;
            if (task->prev == NULL) {
                lbl_8047A968 = task;
            } else {
                task->prev->next = task;
            }
            break;
        }
        current = current->next;
    }

    if (current == NULL) {
        lbl_8047A964->next = task;
        task->next = NULL;
        task->prev = lbl_8047A964;
        lbl_8047A964 = task;
    }
}

void __DSP_remove_task(DSPTaskInfo* task) {
    task->flags = 0;
    task->state = 3;

    if (lbl_8047A968 == task) {
        if (task->next != NULL) {
            lbl_8047A968 = task->next;
            task->next->prev = NULL;
        } else {
            lbl_8047A96C = NULL;
            lbl_8047A964 = NULL;
            lbl_8047A968 = NULL;
        }
        return;
    }

    if (lbl_8047A964 == task) {
        lbl_8047A964 = task->prev;
        task->prev->next = NULL;
        lbl_8047A96C = lbl_8047A968;
        return;
    }

    lbl_8047A96C = task->next;
    task->prev->next = task->next;
    task->next->prev = task->prev;
}

void __CARDDefaultApiCallback(s32 chan, s32 result) {
}

void __CARDExtHandler(s32 chan) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;

    if (card->attached != 0) {
        card->attached = 0;
        fn_8009870C(chan, 0);
        OSCancelAlarm(&card->alarm);
        callback = card->callback_CC;
        if (callback != NULL) {
            card->callback_CC = NULL;
            callback(chan, -3);
        }
        if (card->result != -1) {
            card->result = -3;
        }
        callback = card->extCallback;
        if (callback != NULL && card->field_24 >= 7) {
            card->extCallback = NULL;
            callback(chan, -3);
        }
    }
}

#pragma optimize_for_size on
void __CARDTxHandler(s32 chan) {
    BOOL error;
    BOOL unlocked = FALSE;
    CARDControl* card;
    CARDCallback callback;

    card = &lbl_803FC620[chan];
    error = !EXIDeselect(chan);
    EXIUnlock(chan);
    callback = card->txCallback;
    if (callback != NULL) {
        card->txCallback = NULL;
        if (!error && fn_80098944(chan)) {
            unlocked = TRUE;
        }
        callback(chan, unlocked ? 0 : -3);
    }
}
#pragma optimize_for_size reset

void __CARDUnlockedHandler(s32 chan) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback = card->unlockCallback;

    if (callback != NULL) {
        card->unlockCallback = NULL;
        callback(chan, fn_80098944(chan) ? 1 : -3);
    }
}

s32 __CARDEnableInterrupt(s32 chan, BOOL enable) {
    u32 cmd;
    BOOL err;

    if (!EXISelect(chan, 0, 4)) {
        return -3;
    }
    cmd = enable ? 0x81010000 : 0x81000000;
    err = !EXIImm(chan, &cmd, 2, 1, NULL);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    return err ? -3 : 0;
}

s32 fn_800AF660(s32 chan, u8* status) {
    u32 cmd;
    s32 err;

    if (!EXISelect(chan, 0, 4)) {
        return -3;
    }
    cmd = 0x83000000;
    err  = !EXIImm(chan, &cmd, 2, 1, NULL);
    err |= !EXISync(chan);
    err |= !EXIImm(chan, status, 1, 0, NULL);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    if (err) {
        return -3;
    }
    return 0;
}

s32 __CARDClearStatus(s32 chan) {
    u32 cmd;
    BOOL err;

    if (!EXISelect(chan, 0, 4)) {
        return -3;
    }
    cmd = 0x89000000;
    err = !EXIImm(chan, &cmd, 1, 1, NULL);
    err |= !EXISync(chan);
    err |= !EXIDeselect(chan);
    return err ? -3 : 0;
}

void TimeoutHandler(OSAlarm* alarm) {
    s32 chan;
    CARDControl* card;
    CARDCallback callback;

    for (chan = 0; chan < 2; ++chan) {
        card = &lbl_803FC620[chan];
        if (alarm == &card->alarm) {
            break;
        }
    }
    if (card->attached != 0) {
        fn_8009870C(chan, 0);
        callback = card->callback_CC;
        if (callback != NULL) {
            card->callback_CC = NULL;
            callback(chan, -5);
        }
    }
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

void CARDInit(void) {
    CARDControl* card = lbl_803FC620;
    s32 chan;
    extern u16 fn_8009D820(void);
    extern void OSInitThreadQueue(void* queue);
    extern void OSRegisterResetFunction(void* info);
    extern void __CARDSetDiskID(void* diskId);

    if (card[0].diskId == NULL || card[1].diskId == NULL) {
        lbl_8047A970 = fn_8009D820();
        OSRegisterVersion(lbl_80478A48);
        DSPInit();
        OSInitAlarm();
        for (chan = 0; chan < 2; ++chan, ++card) {
            card->result = -3;
            OSInitThreadQueue(&card->_08C);
            OSCreateAlarm(&card->alarm);
        }
        __CARDSetDiskID((void*)0x80000000);
        OSRegisterResetFunction(lbl_803127F0);
    }
}

u32 DummyLen(void) {
    u32 shift = 1;
    u32 count = 0;
    s32 len;
    u32 tick;
    volatile u32 scratch[4];
    extern u32 OSGetTick(void);

    lbl_80478A50 = OSGetTick();
    lbl_80478A50 = lbl_80478A50 * 0x41c64e6d + 0x3039;
    len = ((lbl_80478A50 >> 16) & 0x1f) + 1;
    while (len < 4 && count < 10) {
        tick = OSGetTick() << shift;
        if (++shift > 16) {
            shift = 1;
        }
        lbl_80478A50 = tick;
        ++count;
        lbl_80478A50 = lbl_80478A50 * 0x41c64e6d + 0x3039;
        len = ((lbl_80478A50 >> 16) & 0x1f) + 1;
    }
    if (len < 4) {
        len = 4;
    }
    return len;
}

#pragma dont_inline on
void InitCallback(void* task) {
    CARDControl* card;
    s32 chan;
    u32 workArea;

    for (chan = 0; chan < 2; ++chan) {
        card = &lbl_803FC620[chan];
        if (card->task == task) {
            break;
        }
    }
    workArea = (u32)card->workArea;
    DSPSendMailToDSP(0xff000000);
    while (fn_800AE794() != 0) {
    }
    DSPSendMailToDSP(workArea);
    while (fn_800AE794() != 0) {
    }
}
#pragma dont_inline off

void BlockReadCallback(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;
    extern s32 __CARDReadSegment(s32 chan, CARDCallback callback);

    if (result >= 0) {
        card->buffer += 0x200;
        card->addr += 0x200;
        card->length += 0x200;
        if (--card->repeat > 0) {
            result = __CARDReadSegment(chan, BlockReadCallback);
            if (result >= 0) {
                return;
            }
        }
    }
    if (card->apiCallback == NULL) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->xferCallback;
    if (callback != NULL) {
        card->xferCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDRead(s32 chan, u32 addr, u32 length, void* buffer, CARDCallback callback) {
    CARDControl* card = &lbl_803FC620[chan];
    extern s32 __CARDReadSegment(s32 chan, CARDCallback callback);

    if (card->attached == 0) {
        return -3;
    }
    card->xferCallback = callback;
    card->repeat = length >> 9;
    card->addr = addr;
    card->length = (u32)buffer;
    return __CARDReadSegment(chan, BlockReadCallback);
}

void fn_800B18C8(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;
    extern s32 fn_800AFEC4(s32 chan, CARDCallback callback);

    if (result >= 0) {
        card->buffer += 0x80;
        card->addr += 0x80;
        card->length += 0x80;
        if (--card->repeat > 0) {
            result = fn_800AFEC4(chan, fn_800B18C8);
            if (result >= 0) {
                return;
            }
        }
    }
    if (card->apiCallback == NULL) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->xferCallback;
    if (callback != NULL) {
        card->xferCallback = NULL;
        callback(chan, result);
    }
}

#pragma dont_inline on
s32 fn_800B19A4(s32 chan, u32 addr, u32 length, void* buffer, CARDCallback callback) {
    CARDControl* card = &lbl_803FC620[chan];
    extern s32 fn_800AFEC4(s32 chan, CARDCallback callback);

    if (card->attached == 0) {
        return -3;
    }
    card->xferCallback = callback;
    card->repeat = length >> 7;
    card->addr = addr;
    card->length = (u32)buffer;
    return fn_800AFEC4(chan, fn_800B18C8);
}
#pragma dont_inline off

void WriteCallback_800C1C98(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;
    void* first;
    void* second;
    extern void* memcpy(void* dst, const void* src, u32 length);

    if (result >= 0) {
        first = (u8*)card->workArea + 0x6000;
        second = (u8*)card->workArea + 0x8000;
        if (card->fatBlock == first) {
            card->fatBlock = second;
            memcpy(second, first, 0x2000);
        } else {
            card->fatBlock = first;
            memcpy(first, second, 0x2000);
        }
    }
    if (card->apiCallback == NULL) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->updateCallback;
    if (callback != NULL) {
        card->updateCallback = NULL;
        callback(chan, result);
    }
}

void EraseCallback_800C1D6C(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;
    void* fat;
    u32 addr;
    u32 scratch[2];

    if (result >= 0) {
        fat = card->fatBlock;
        addr = card->sectorSize * (((u32)fat - (u32)card->workArea) >> 13);
        result = fn_800B19A4(chan, addr, 0x2000, fat, WriteCallback_800C1C98);
        if (result >= 0) {
            return;
        }
    }
    if (card->apiCallback == NULL) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->updateCallback;
    if (callback != NULL) {
        card->updateCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDFreeBlock(s32 chan, u16 block, CARDCallback callback) {
    u16* fat;
    CARDControl* card = &lbl_803FC620[chan];
    u16 next;
    extern s32 __CARDUpdateFatBlock(s32 chan, u16* fat, CARDCallback callback);

    if (card->attached == 0) {
        return -3;
    }
    fat = card->fatBlock;
    while (block != 0xffff) {
        if (block < 5 || block >= *(u16*)&card->_010[0]) {
            return -6;
        }
        next = fat[block];
        fat[block] = 0;
        fat[3]++;
        block = next;
    }
    return __CARDUpdateFatBlock(chan, fat, callback);
}

s32 __CARDUpdateFatBlock(s32 chan, u16* fat, CARDCallback callback) {
    CARDControl* card = &lbl_803FC620[chan];
    extern void __CARDCheckSum(void* data, u32 length, u16* checksum, u16* checksumInv);
    extern void DCStoreRange(void* addr, u32 length);
    extern s32 fn_800AFFE0(s32 chan, u32 addr, void* callback);

    fat[2]++;
    __CARDCheckSum(&fat[2], 0x1ffc, &fat[0], &fat[1]);
    DCStoreRange(fat, 0x2000);
    card->updateCallback = callback;
    return fn_800AFFE0(chan,
                       card->sectorSize * (((u32)fat - (u32)card->workArea) >> 13),
                       EraseCallback_800C1D6C);
}

void WriteCallback_800C209C(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;
    void* first;
    void* second;
    extern void* memcpy(void* dst, const void* src, u32 length);

    if (result >= 0) {
        first = (u8*)card->workArea + 0x2000;
        second = (u8*)card->workArea + 0x4000;
        if (card->dirBlock == first) {
            card->dirBlock = second;
            memcpy(second, first, 0x2000);
        } else {
            card->dirBlock = first;
            memcpy(first, second, 0x2000);
        }
    }
    if (card->apiCallback == NULL) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->updateCallback;
    if (callback != NULL) {
        card->updateCallback = NULL;
        callback(chan, result);
    }
}

void EraseCallback_800C216C(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;
    void* dir;
    u32 addr;
    u32 scratch[2];

    if (result >= 0) {
        dir = card->dirBlock;
        addr = card->sectorSize * (((u32)dir - (u32)card->workArea) >> 13);
        result = fn_800B19A4(chan, addr, 0x2000, dir, WriteCallback_800C209C);
        if (result >= 0) {
            return;
        }
    }
    if (card->apiCallback == NULL) {
        __CARDPutControlBlock(card, result);
    }
    callback = card->updateCallback;
    if (callback != NULL) {
        card->updateCallback = NULL;
        callback(chan, result);
    }
}

s32 __CARDUpdateDir(s32 chan, CARDCallback callback) {
    u8* dir;
    CARDControl* card = &lbl_803FC620[chan];
    u8* checksumBase;
    s16* checkCode;
    extern void __CARDCheckSum(void* data, u32 length, u16* checksum, u16* checksumInv);
    extern void DCStoreRange(void* addr, u32 length);

    if (card->attached == 0) {
        return -3;
    }
    dir = card->dirBlock;
    checkCode = (s16*)(dir + 0x1ffa);
    (*checkCode)++;
    checksumBase = dir + 0x1fc0;
    __CARDCheckSum(dir, 0x1ffc, (u16*)(checksumBase + 0x3c),
                   (u16*)(checksumBase + 0x3e));
    DCStoreRange(dir, 0x2000);
    card->updateCallback = callback;
    return fn_800AFFE0(chan,
                       card->sectorSize * (((u32)dir - (u32)card->workArea) >> 13),
                       EraseCallback_800C216C);
}

s32 __CARDVerify(CARDControl* card) {
    s32 result;
    s32 dirResult;
    extern s32 VerifyID(CARDControl* card);
    extern s32 VerifyDir(CARDControl* card, s32* checkCode);
    extern s32 VerifyFAT(CARDControl* card, s32* checkCode);

    result = VerifyID(card);
    if (result < 0) {
        return result;
    }
    dirResult = VerifyDir(card, NULL);
    switch (dirResult + VerifyFAT(card, NULL)) {
    case 0:
        return 0;
    case 1:
        return -6;
    default:
        return -6;
    }
}

BOOL IsCard(u32 id) {
    s32 sectorSize;
    u32 cardSize;

    if ((id & 0xffff0000) != 0) {
        if (id != 0x80000004 || lbl_80478A58 == 0xffff) {
            return FALSE;
        }
    }
    if ((id & 3) != 0) {
        return FALSE;
    }
    cardSize = id & 0xfc;
    switch (cardSize) {
    case 4:
    case 8:
    case 16:
    case 32:
    case 64:
    case 128:
        break;
    default:
        return FALSE;
    }
    sectorSize = lbl_80312960[(id >> 11) & 7];
    if (sectorSize == 0) {
        return FALSE;
    }
    if (((cardSize << 17) & 0x1ffe0000) / sectorSize < 8) {
        return FALSE;
    }
    return TRUE;
}

void DoUnmount(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    BOOL enabled;

    enabled = OSDisableInterrupts();
    if (card->attached != 0) {
        fn_8009870C(chan, 0);
        fn_80098AE8(chan);
        OSCancelAlarm(&card->alarm);
        card->attached = 0;
        card->result = result;
        card->field_24 = 0;
    }
    OSRestoreInterrupts(enabled);
}

#pragma dont_inline on
s32 CARDUnmount(s32 chan) {
    CARDControl* card;
    BOOL enabled;
    CARDControl* control;
    s32 result;
    extern s32 __CARDGetControlBlock(s32 chan, CARDControl** card);

    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }
    control = &lbl_803FC620[chan];
    enabled = OSDisableInterrupts();
    if (control->attached != 0) {
        fn_8009870C(chan, 0);
        fn_80098AE8(chan);
        OSCancelAlarm(&control->alarm);
        control->attached = 0;
        control->result = -3;
        control->field_24 = 0;
    }
    OSRestoreInterrupts(enabled);
    return 0;
}
#pragma dont_inline off

s32 fn_800B4270(CARDControl* card, CARDDirEntry* entry) {
    extern s32 memcmp(const void* first, const void* second, u32 length);

    if (entry->gameName[0] == 0xff) {
        return -4;
    }
    if (card->diskId == lbl_803FC840 ||
        (memcmp(entry->gameName, card->diskId, 4) == 0 &&
         memcmp(entry->company, (u8*)card->diskId + 4, 2) == 0)) {
        return 0;
    }
    return -10;
}

s32 CARDCancel(CARDFileInfo* fileInfo) {
    CARDFileInfo* file = fileInfo;
    CARDControl* card;
    BOOL enabled;
    s32 result;

    enabled = OSDisableInterrupts();
    card = &lbl_803FC620[file->chan];
    result = 0;
    if (card->attached == 0) {
        result = -3;
    } else if (card->result == -1 && *(CARDFileInfo**)&card->_0BC[4] == file) {
        file->length = -1;
        result = -14;
    }
    OSRestoreInterrupts(enabled);
    return result;
}

void EraseCallback(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback;
    CARDFileInfo* fileInfo;
    extern s32 fn_800B19A4(s32 chan, u32 addr, u32 length, void* buffer,
                           CARDCallback callback);
    extern void WriteCallback(s32 chan, s32 result);

    if (result >= 0) {
        fileInfo = *(CARDFileInfo**)&card->_0BC[4];
        result = fn_800B19A4(chan, card->sectorSize * fileInfo->startBlock,
                            card->sectorSize, (void*)card->length, WriteCallback);
        if (result >= 0) {
            return;
        }
    }
    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    callback(chan, result);
}

void DeleteCallback(s32 chan, s32 result) {
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback = card->apiCallback;
    extern s32 __CARDFreeBlock(s32 chan, u16 block, CARDCallback callback);

    card->apiCallback = NULL;
    if (result >= 0) {
        result = __CARDFreeBlock(chan, *(u16*)&card->_0BC[2], callback);
        if (result >= 0) {
            return;
        }
    }
    __CARDPutControlBlock(card, result);
    if (callback != NULL) {
        callback(chan, result);
    }
}

void __CARDSetDiskID(void* diskId) {
    CARDControl* card = lbl_803FC620;

    card[0].diskId = diskId ? diskId : (void*)&card[2];
    card[1].diskId = diskId ? diskId : (void*)&card[2];
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
    if (result < 0) {
        return result;
    }
    fileInfo->chan = -1;
    return __CARDPutControlBlock(card, 0);
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
        if (CARDUnmount(0) == -1 || CARDUnmount(1) == -1) {
            return FALSE;
        }
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

#pragma dont_inline on
s32 fn_800B57D0(s32 chan, s32 fileNo, CARDDirEntry* entry) {
    CARDControl* card;
    s32 result;
    CARDDirEntry* dirEntry;
    extern s32 __CARDGetControlBlock(s32 chan, CARDControl** card);
    extern CARDDirEntry* __CARDGetDirBlock(CARDControl* card);
    extern s32 fn_800B4270(CARDControl* card, CARDDirEntry* entry);
    extern s32 fn_800B4308(CARDDirEntry* entry);
    extern void* memcpy(void* dst, const void* src, u32 length);

    if (fileNo < 0 || fileNo >= 127) {
        return -128;
    }
    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }
    dirEntry = &__CARDGetDirBlock(card)[fileNo];
    result = fn_800B4270(card, dirEntry);
    if (result == -10) {
        result = fn_800B4308(dirEntry);
    }
    if (result >= 0) {
        memcpy(entry, dirEntry, sizeof(CARDDirEntry));
    }
    return __CARDPutControlBlock(card, result);
}
#pragma dont_inline off

#pragma dont_inline on
s32 CARDGetSerialNo(s32 chan, u64* serialNo) {
    CARDControl* card;
    u32* id;
    u32 high;
    u32 low;
    s32 i;
    s32 result;
    extern s32 __CARDGetControlBlock(s32 chan, CARDControl** card);

    if (chan < 0 || chan >= 2) {
        return -128;
    }
    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }
    id = card->workArea;
    high = low = 0;
    for (i = 0; i < 4; ++i) {
        high ^= *id++;
        low ^= *id++;
    }
    ((u32*)serialNo)[0] = high;
    ((u32*)serialNo)[1] = low;
    return __CARDPutControlBlock(card, 0);
}
#pragma dont_inline off

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

#pragma peephole off
void* fn_800B5C5C(void* object) {
    s32 format;
    u32 count;
    extern u32 fn_800BAE5C(void* object);

    format = fn_800BAE5C(object);
    if (format != 8 && format != 9 && format != 10) {
        u8* data = (u8*)gx;
        count = *(u32*)(data + 0x2c8);
        *(u32*)(data + 0x2c8) = count + 1;
        return data + ((count & 7) << 4) + 0x208;
    } else {
        u8* data = (u8*)gx;
        count = *(u32*)(data + 0x2cc);
        *(u32*)(data + 0x2cc) = count + 1;
        return data + ((count & 3) << 4) + 0x288;
    }
}
#pragma peephole reset

s32 fn_800AFFE0(s32 chan, u32 addr, void* callback) {
    CARDControl* card = &lbl_803FC620[chan];
    s32 result;

    card->cmd[0] = 0xf1;
    card->cmd[1] = (addr >> 17) & 0x7f;
    card->cmd[2] = (addr >> 9) & 0xff;
    card->cmdLen = 3;
    card->field_A4 = -1;
    card->field_A8 = 3;
    result = fn_800AFBDC(chan, NULL, callback);
    if (result == -1) {
        result = 0;
    } else if (result >= 0) {
        if (fn_80098368(chan, card->cmd, card->cmdLen, 1) == 0) {
            card->callback_CC = NULL;
            result = -3;
        } else {
            result = 0;
        }
        EXIDeselect(chan);
        EXIUnlock(chan);
    }
    return result;
}

#pragma peephole off
GXTlutRegion* __GXDefaultTlutRegionCallback(u32 index) {
    GXTlutRegion* region;

    if (index >= 20) {
        region = NULL;
    } else {
        region = &gx->defaultTlutRegions[index];
    }
    return region;
}
#pragma peephole reset

void GXInitFifoPtrs(GXFifoObj* fifo, void* readPtr, void* writePtr);

void GXInitFifoBase(GXFifoObj* fifo, void* base, u32 size) {
    extern void fn_800B71F0(GXFifoObj* fifo, u32 hiWatermark, u32 loWatermark);

    fifo->base = base;
    fifo->top = (u8*)base + size - 4;
    fifo->size = size;
    fifo->count = 0;
    fn_800B71F0(fifo, size - 0x4000, (size >> 1) & ~0x1f);
    GXInitFifoPtrs(fifo, base, base);
}

void GXInitFifoPtrs(GXFifoObj* fifo, void* readPtr, void* writePtr) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    fifo->rdPtr = readPtr;
    fifo->wrPtr = writePtr;
    fifo->count = (u8*)writePtr - (u8*)readPtr;
    if (fifo->count < 0) {
        fifo->count += fifo->size;
    }
    OSRestoreInterrupts(enabled);
}
