/**
 * @file sdk_range_800B71FC.c
 * @brief dolphin-sdk code, 0x800B71FC - 0x800B770C (10 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/gx/GX.h"
#include "dolphin/os/OSInterrupt.h"

extern GXData* gx;
extern volatile u16* __cpReg;

void fn_800B7514(void) {
    u32* gxStatus = (u32*)gx;
    u32 value;

    value = gxStatus[2];
    gxStatus[2] = (value & ~1U) | 1U;
    __cpReg[1] = (u16)gxStatus[2];
}

void fn_800B7538(void) {
    u32* gxStatus = (u32*)gx;

    gxStatus[2] &= ~1U;
    __cpReg[1] = (u16)gxStatus[2];
}

void fn_800B7558(u8 arg0) {
    u32* gxStatus = (u32*)gx;
    u32 bit;

    if ((arg0 & 0xFF) != 0) {
        bit = 1U;
    } else {
        bit = 0U;
    }
    gxStatus[2] = (gxStatus[2] & 0xF7FFFFFFU) | (bit << 4);
    __cpReg[1] = (u16)gxStatus[2];
}

void fn_800B7594(u8 arg0, u8 arg1) {
    u32* gxStatus = (u32*)gx;
    gxStatus[2] = (gxStatus[2] & ~4U) | ((u32)(arg0 << 2));
    gxStatus[2] = (gxStatus[2] & ~8U) | ((u32)(arg1 << 3));
    __cpReg[1] = (u16)gxStatus[2];
}

void fn_800B75D0(u8 arg0, u8 arg1) {
    u32* gxStatus = (u32*)gx;
    gxStatus[4] = (gxStatus[4] & ~1U) | (u32)arg0;
    gxStatus[4] = (gxStatus[4] & (u32)~(1ULL << 30)) | (u32)(arg1 * 2U);
    __cpReg[2] = (u16)gxStatus[4];
}

typedef struct GXFifoObjPrivate {
    GXFifoObj fifo;
    u8 padding[0x80 - sizeof(GXFifoObj)];
} GXFifoObjPrivate;

static inline void GXResetFifoPtrs(GXFifoObj* fifo, void* readPtr, void* writePtr) {
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

void __GXCleanGPFifo(void) {
    extern GXFifoObjPrivate* fn_800B770C(void);
    extern GXFifoObjPrivate* fn_800B7714(void);
    GXFifoObjPrivate* gpFifo;
    GXFifoObjPrivate* cpuFifo;
    GXFifoObjPrivate cleanFifo;
    u32 _pad[3];
    void* base;

    gpFifo = fn_800B7714();
    if (gpFifo != NULL) {
        cpuFifo = fn_800B770C();
        base = gpFifo->fifo.base;
        (void)_pad;
        cleanFifo = *gpFifo;
        GXResetFifoPtrs(&cleanFifo.fifo, base, base);
        GXSetGPFifo(&cleanFifo.fifo);
        if (cpuFifo == gpFifo) {
            GXSetCPUFifo(&cleanFifo.fifo);
        }
        GXResetFifoPtrs(&gpFifo->fifo, base, base);
        GXSetGPFifo(&gpFifo->fifo);
        if (cpuFifo == gpFifo) {
            GXSetCPUFifo(&cpuFifo->fifo);
        }
    }
}
