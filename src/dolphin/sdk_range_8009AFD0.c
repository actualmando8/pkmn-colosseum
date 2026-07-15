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

extern void* __OSArenaLo_80478988;

#pragma peephole off
void* OSAllocFromArenaLo(u32 size, u32 alignment) {
    u32 mask;
    void* alignedLo;

    alignment -= 1;
    mask = ~alignment;
    alignedLo = (void*)(((u32)__OSArenaLo_80478988 + alignment) & mask);
    __OSArenaLo_80478988 = (void*)(((u32)alignedLo + size + alignment) & mask);
    return (void*)alignedLo;
}
#pragma peephole on

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

    *regs = 0x8AC;
    {
        volatile u16* mailbox = (volatile u16*)0xCC005000;

        mailbox[0] = 0;
        while ((((u32)mailbox[2] << 16) | mailbox[3]) & 0x80000000) {
        }
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
