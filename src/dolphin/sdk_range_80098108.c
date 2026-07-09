/**
 * @file sdk_range_80098108.c
 * @brief dolphin-sdk code, 0x80098108 - 0x8009A0E0 (32 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/dvd/dvd.h"
#include "dolphin/exi/EXI.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSInterrupt.h"

typedef struct EXIRegs {
    u32 csr;
    u32 mar;
    u32 length;
    u32 cr;
    u32 data;
} EXIRegs;

typedef union EXIRegBlock {
    EXIRegs regs[3];
    u32 words[15];
} EXIRegBlock;

typedef struct EXIQueueEntry {
    u32 device;
    void (*callback)(s32 chan, OSContext* context);
} EXIQueueEntry;

typedef struct EXIControl {
    void (*exiCallback)(s32 chan, OSContext* context);
    void (*tcCallback)(s32 chan, OSContext* context);
    void (*extCallback)(s32 chan, OSContext* context);
    u32 state;
    s32 immLen;
    u8* immBuf;
    u32 device;
    u32 id;
    s32 idTime;
    s32 items;
    EXIQueueEntry queue[3];
} EXIControl;

typedef struct OSLowMem {
    u8 pad_0000[0x30C0];
    u32 exiProbeStartTime[2];
    u8 pad_30C8[0x1E];
    u16 dvdDeviceCode;
    u8 osDebugFlag;
    u8 padSpec;
} OSLowMem;

typedef void (*OSExceptionHandler)(u8 exception, OSContext* context, u32 dsisr, u32 dar);

extern OSBootInfo* BootInfo_8047A6A0;
extern OSExceptionHandler* OSExceptionTable_8047A6C4;
extern EXIControl lbl_803FB3C8[];
extern DVDDriveInfo DriveInfo_803FB4A0;

static volatile EXIRegBlock* const ExiHw = (volatile EXIRegBlock*)0xCC006800;
static volatile OSLowMem* const LowMem = (volatile OSLowMem*)0x80000000;

BOOL fn_80099400(s32 chan, u32 dev, u32* id);
void fn_80098110(s32 chan, EXIControl* exi);
void* memmove(void* dst, const void* src, size_t n);

BOOL fn_80098368(s32 chan, u8* buf, s32 len, u32 type) {
    u8* cursor = buf;

    while (len != 0) {
        s32 xfer = len < 4 ? len : 4;

        if (!EXIImm(chan, cursor, xfer, type, NULL)) {
            return FALSE;
        }
        if (!EXISync(chan)) {
            return FALSE;
        }

        cursor += xfer;
        len -= xfer;
    }

    return TRUE;
}

#pragma push
#pragma optimization_level 0
u32 fn_800986A0(s32 chan, s32 exi, s32 tc, s32 ext) {
    s32 csrIndex = chan * 5;
    u32 oldCsr = ExiHw->words[csrIndex];
    u32 csr = oldCsr;

    csr &= 0x7F5;

    if (exi != 0) {
        csr |= 0x2;
    }
    if (tc != 0) {
        csr |= 0x8;
    }
    if (ext != 0) {
        csr |= 0x800;
    }

    ExiHw->words[csrIndex] = csr;
    return oldCsr;
}
#pragma pop

EXICallback fn_8009870C(s32 chan, EXICallback callback) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    BOOL enabled = OSDisableInterrupts();
    EXICallback oldCallback = exi->exiCallback;

    exi->exiCallback = callback;
    if (chan != 2) {
        fn_80098110(chan, exi);
    } else {
        fn_80098110(0, &lbl_803FB3C8[0]);
    }
    OSRestoreInterrupts(enabled);
    return oldCallback;
}

BOOL EXILock(s32 chan, u32 dev, EXICallback unlockedCallback) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    BOOL enabled = OSDisableInterrupts();
    s32 i;

    if (exi->state & 0x10) {
        if (unlockedCallback != NULL) {
            for (i = 0; i < exi->items; i++) {
                if (exi->queue[i].device == dev) {
                    OSRestoreInterrupts(enabled);
                    return FALSE;
                }
            }

            exi->queue[exi->items].callback = unlockedCallback;
            exi->queue[exi->items].device = dev;
            exi->items++;
        }

        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state |= 0x10;
    exi->device = dev;
    fn_80098110(chan, exi);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIUnlock(s32 chan) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    BOOL enabled = OSDisableInterrupts();

    if (!(exi->state & 0x10)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= ~0x10;
    fn_80098110(chan, exi);

    if (exi->items > 0) {
        EXICallback callback = exi->queue[0].callback;

        exi->items--;
        if (exi->items > 0) {
            memmove(&exi->queue[0], &exi->queue[1], exi->items * sizeof(EXIQueueEntry));
        }
        callback(chan, NULL);
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

#pragma push
#pragma optimization_level 0
#pragma peephole off
u32 fn_800993A8(s32 chan) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    return exi->state;
}
#pragma peephole reset
#pragma pop

#pragma push
#pragma optimization_level 0
BOOL fn_800993D0(s32 chan) {
    u32 id;

    return fn_80099400(chan, 0, &id);
}
#pragma pop

#pragma peephole off
u32 OSGetConsoleType(void) {
    OSBootInfo* bootInfo = BootInfo_8047A6A0;

    if (bootInfo != NULL && bootInfo->consoleType != 0) {
        return bootInfo->consoleType;
    }
    return 0x10000002;
}
#pragma peephole reset

#pragma peephole off
static void InquiryCallback(s32 result, DVDCommandBlock* block) {
    (void)result;

    switch (block->state) {
    case 0:
        LowMem->dvdDeviceCode = DriveInfo_803FB4A0.deviceCode | 0x8000;
        break;
    default:
        LowMem->dvdDeviceCode = 1;
        break;
    }
}
#pragma peephole reset

#pragma peephole off
OSExceptionHandler __OSSetExceptionHandler(u8 exception, OSExceptionHandler handler) {
    OSExceptionHandler* entry = &OSExceptionTable_8047A6C4[exception];
    OSExceptionHandler old = *entry;

    *entry = handler;
    return old;
}
#pragma peephole reset
