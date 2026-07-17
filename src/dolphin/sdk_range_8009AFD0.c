/**
 * @file sdk_range_8009AFD0.c
 * @brief dolphin-sdk code, 0x8009AFD0 - 0x8009B290 (3 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef void OSArenaAddress;

extern OSArenaAddress* __OSArenaLo;

void* OSAllocFromArenaLo(u32 size, u32 align) {
    u32 am1 = align - 1;
    u32 mask = ~am1;
    OSArenaAddress* ptr;

    ptr = (OSArenaAddress*)(((u32)__OSArenaLo + am1) & mask);
    size = (u32)ptr + size;
    size += am1;
    __OSArenaLo = (OSArenaAddress*)(mask & size);
    return ptr;
}

void __OSStopAudioSystem(void) {
    volatile u16* regs = (volatile u16*)0xCC005000;
    volatile u16* dmaRegs = regs;
    s32 start;
    u16 status;

    extern u32 OSGetTick(void);

    regs[5] = 0x804;
    dmaRegs[27] &= ~0x8000;

    status = *(regs += 5);
    while (status & 0x400) {
        status = *regs;
    }
    status = *regs;
    while (status & 0x200) {
        status = *regs;
    }

    dmaRegs = (volatile u16*)0xCC005000;
    *regs = 0x8AC;
    dmaRegs[0] = 0;
    while ((((u32)dmaRegs[2] << 16) | dmaRegs[3]) & 0x80000000) {
    }

    start = OSGetTick();
    while ((s32)(OSGetTick() - start) < 44) {
    }

    *regs |= 1;
    status = *regs;
    while (status & 1) {
        status = *regs;
    }
}
