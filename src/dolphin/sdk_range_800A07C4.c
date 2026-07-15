/**
 * @file sdk_range_800A07C4.c
 * @brief dolphin-sdk code, 0x800A07C4 - 0x800A13E8 (19 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

void* __OSLockSram(void) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern u32 Scb_803FB840[0x54 / sizeof(u32)];
    BOOL enabled;
    void* result = Scb_803FB840;

    enabled = OSDisableInterrupts();
    if ((s32)Scb_803FB840[0x12] != 0) {
        OSRestoreInterrupts(enabled);
        result = 0;
    } else {
        Scb_803FB840[0x11] = enabled;
        Scb_803FB840[0x12] = 1;
    }

    return result;
}

void* __OSLockSramEx(void) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern u32 Scb_803FB840[0x54 / sizeof(u32)];
    BOOL enabled;
    u32* sram = Scb_803FB840;
    u32* lock;

    enabled = OSDisableInterrupts();
    lock = &sram[0x12];
    if ((s32)sram[0x12] != 0) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    sram[0x11] = enabled;
    *lock = 1;
    return (u8*)sram + 0x14;
}

void __OSUnlockSram(BOOL commit) {
    extern void fn_800A09B0(BOOL commit, u32 arg);

    fn_800A09B0(commit, 0);
}

BOOL __OSSyncSram(void) {
    extern u32 Scb_803FB840[0x54 / sizeof(u32)];

    return ((u32*)Scb_803FB840)[0x13];
}

BOOL __OSUnlockSramEx(BOOL commit) {
    extern BOOL fn_800A09B0(BOOL commit, u32 arg);

    return fn_800A09B0(commit, 0x14);
}

s32 OSGetSoundMode(void) {
    u8* sram;
    u32 flags;
    s32 mode;

    sram = __OSLockSram();
    flags = sram[0x13];
    if (flags & 4) {
        mode = 1;
    } else {
        mode = 0;
    }

    __OSUnlockSram(FALSE);
    return mode;
}

void fn_800A0EB4(u32 mode) {
    u8* sram;

    mode = (mode & 1) << 2;
    sram = __OSLockSram();
    if (mode == (sram[0x13] & 4)) {
        __OSUnlockSram(FALSE);
    } else {
        sram[0x13] &= ~4;
        sram[0x13] |= mode;
        __OSUnlockSram(TRUE);
    }
}

u32 OSGetProgressiveMode(void) {
    u8* sram;
    u32 mode;

    sram = __OSLockSram();
    mode = (sram[0x13] >> 7) & 1;
    __OSUnlockSram(FALSE);
    return mode;
}

void fn_800A0FC8(u32 mode) {
    u8* sram;

    mode = (mode & 1) << 7;
    sram = __OSLockSram();
    if (mode == (sram[0x13] & 0x80)) {
        __OSUnlockSram(FALSE);
    } else {
        sram[0x13] &= ~0x80;
        sram[0x13] |= mode;
        __OSUnlockSram(TRUE);
    }
}

u8 OSGetLanguage(void) {
    u8* sram;
    u8 language;

    sram = __OSLockSram();
    language = sram[0x12];
    __OSUnlockSram(FALSE);
    return language;
}

u16 OSGetWirelessID(s32 chan) {
    typedef struct OSSramEx {
        u8 flashID[2][12];
        u32 wirelessKbID;
        u16 wirelessPadID[4];
    } OSSramEx;
    OSSramEx* sram;
    s32 index;
    u16 id;

    index = chan;
    sram = __OSLockSramEx();
    id = sram->wirelessPadID[index];
    __OSUnlockSramEx(FALSE);
    return id;
}

void OSSetWirelessID(s32 chan, u16 id) {
    typedef struct OSSramEx {
        u8 flashID[2][12];
        u32 wirelessKbID;
        u16 wirelessPadID[4];
    } OSSramEx;
    OSSramEx* sram;

    sram = __OSLockSramEx();
    if (sram->wirelessPadID[chan] != id) {
        sram->wirelessPadID[chan] = id;
        __OSUnlockSramEx(TRUE);
    } else {
        __OSUnlockSramEx(FALSE);
    }
}

#pragma peephole off
void __OSInitSystemCall(void) {
    extern void __OSSystemCallVectorStart(void);
    extern void __OSSystemCallVectorEnd(void);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern void DCFlushRangeNoSync(void* addr, u32 size);
    extern void ICInvalidateRange(void* addr, u32 size);
    void* vector;
    u32 size;

    vector = (void*)0x80000C00;
    size = (u32)__OSSystemCallVectorEnd - (u32)__OSSystemCallVectorStart;
    memcpy(vector, __OSSystemCallVectorStart, size);
    DCFlushRangeNoSync(vector, 0x100);
    __sync();
    ICInvalidateRange(vector, 0x100);
}
#pragma peephole reset

void fn_800A128C(void) {
}
