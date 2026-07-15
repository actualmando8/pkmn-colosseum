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
#include "dolphin/os/PPCArch.h"

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

#ifdef __MWERKS__
#define AT_ADDRESS(addr) : addr
volatile u32 __EXIRegs[0x40] AT_ADDRESS(0xCC006800);
#else
#define __EXIRegs ((volatile u32*)0xCC006800)
#endif

BOOL fn_80099400(s32 chan, u32 dev, u32* id);
BOOL fn_80098790(s32 chan);
void fn_80098110(s32 chan, EXIControl* exi);
void* memmove(void* dst, const void* src, size_t n);

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
void fn_80098110(volatile s32 chan, EXIControl* exi) {
    EXIControl* exi2 = &lbl_803FB3C8[2];

    switch (chan) {
    case 0:
        if ((!exi->exiCallback && !exi2->exiCallback) || (exi->state & 0x10)) {
            __OSMaskInterrupts(0x410000);
        } else {
            __OSUnmaskInterrupts(0x410000);
        }
        break;
    case 1:
        if (!exi->exiCallback || (exi->state & 0x10)) {
            __OSMaskInterrupts(0x80000);
        } else {
            __OSUnmaskInterrupts(0x80000);
        }
        break;
    case 2:
        if (!__OSGetInterruptHandler(0x19) || (exi->state & 0x10)) {
            __OSMaskInterrupts(0x40);
        } else {
            __OSUnmaskInterrupts(0x40);
        }
        break;
    }
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma scheduling off
BOOL fn_80098368(s32 chan, u8* buf, s32 len, volatile u32 type) {
    while (len != 0) {
        s32 xfer;

        if (len < 4) {
            xfer = len;
        } else {
            xfer = 4;
        }

        if (!EXIImm(chan, buf, xfer, type, NULL)) {
            return FALSE;
        }
        if (!EXISync(chan)) {
            return FALSE;
        }

        buf += xfer;
        len -= xfer;
    }

    return TRUE;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
u32 fn_800986A0(s32 chan, s32 exi, s32 tc, s32 ext) {
    u32 csr;
    u32 oldCsr;

    {
        volatile u32* regs = (volatile u32*)0xCC006800;
        csr = regs[chan * 5];
        oldCsr = csr;
    }

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

    {
        volatile u32* regs = (volatile u32*)0xCC006800;
        regs[chan * 5] = csr;
    }
    return oldCsr;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
EXICallback fn_8009870C(s32 chan, EXICallback volatile callback) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    EXICallback oldCallback;
    BOOL enabled = OSDisableInterrupts();

    oldCallback = exi->exiCallback;

    exi->exiCallback = callback;
    if (chan != 2) {
        fn_80098110(chan, exi);
    } else {
        fn_80098110(0, &lbl_803FB3C8[0]);
    }
    OSRestoreInterrupts(enabled);
    return oldCallback;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
BOOL fn_80098944(s32 chan) {
    BOOL probe;
    EXIControl* exi = &lbl_803FB3C8[chan];
    u32 id;

    probe = fn_80098790(chan);
    if (probe && exi->idTime == 0) {
        probe = fn_80099400(chan, 0, &id) ? TRUE : FALSE;
    }
    return probe;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
BOOL fn_80098AE8(s32 chan) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    BOOL enabled = OSDisableInterrupts();

    if (!(exi->state & 8)) {
        OSRestoreInterrupts(enabled);
        return TRUE;
    }
    if ((exi->state & 0x10) && exi->device == 0) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= ~8;
    __OSMaskInterrupts(0x700000u >> (chan * 3));
    OSRestoreInterrupts(enabled);
    return TRUE;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
BOOL EXIDeselect(s32 chan) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    u32 csr;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    if (!(exi->state & 4)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= ~4;
    csr = __EXIRegs[chan * 5];
    __EXIRegs[chan * 5] = csr & 0x405;

    if (exi->state & 8) {
        switch (chan) {
        case 0:
            __OSUnmaskInterrupts(0x100000);
            break;
        case 1:
            __OSUnmaskInterrupts(0x20000);
            break;
        }
    }

    OSRestoreInterrupts(enabled);

    if (chan != 2 && (csr & 0x80)) {
        return fn_80098790(chan) ? TRUE : FALSE;
    }

    return TRUE;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
void fn_80098DDC(volatile s16 interrupt, OSContext* context) {
    OSContext exceptionContext;
    s32 chan = (interrupt - 9) / 3;
    EXIControl* exi = &lbl_803FB3C8[chan];
    u32 csr = __EXIRegs[chan * 5];
    volatile u32 oldCsr = csr;
    EXICallback callback;

    csr &= 0x7F5;
    csr |= 2;
    __EXIRegs[chan * 5] = csr;
    callback = exi->exiCallback;
    if (callback) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        callback(chan, context);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
void fn_80098FF8(volatile s16 interrupt, OSContext* context) {
    OSContext exceptionContext;
    s32 chan = (interrupt - 11) / 3;
    EXIControl* exi;
    EXICallback callback;

    __OSMaskInterrupts(0x700000u >> (chan * 3));
    __EXIRegs[chan * 5] = 0;
    exi = &lbl_803FB3C8[chan];
    callback = exi->extCallback;
    exi->state &= ~8;
    if (callback) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        exi->extCallback = NULL;
        callback(chan, context);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
BOOL EXILock(s32 chan, u32 dev, EXICallback unlockedCallback) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    BOOL enabled = OSDisableInterrupts();
    s32 i;

    if (exi->state & 0x10) {
        if (unlockedCallback) {
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
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma optimize_for_size on
#pragma scheduling off
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

        if (--exi->items > 0) {
            memmove(&exi->queue[0], &exi->queue[1], exi->items * sizeof(EXIQueueEntry));
        }
        callback(chan, NULL);
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma peephole off
#pragma scheduling off
void OSRegisterVersion(const char* version);
u32 fn_800993A8(s32 chan) {
    EXIControl* exi = &lbl_803FB3C8[chan];
    return exi->state;
}
#pragma scheduling reset
#pragma peephole reset
#pragma pop

#pragma push
#pragma optimization_level 0
#pragma scheduling off
BOOL fn_800993D0(s32 chan) {
    u32 id;
    u32 unused;

    return fn_80099400(chan, 0, &id);
}
#pragma scheduling reset
#pragma pop

#pragma peephole off
u32 OSGetConsoleType(void) {
    if (BootInfo_8047A6A0 == NULL || BootInfo_8047A6A0->consoleType == 0) {
        return 0x10000002;
    }
    return BootInfo_8047A6A0->consoleType;
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

/* 0x80098108 | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void PPCSetFpNonIEEEMode(void) {
    nofralloc
    mtfsb1 29
    blr
}
#pragma pop

/* 0x8009A09C | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off


#pragma optimization_level 0
void fn_8009A0C0(void) {
    extern void __OSDBINTEND(void);
    __OSDBINTEND();
}
#pragma pop
