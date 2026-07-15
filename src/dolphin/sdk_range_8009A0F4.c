/**
 * @file sdk_range_8009A0F4.c
 * @brief dolphin-sdk code, 0x8009A0F4 - 0x8009A2C8 (6 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/PPCArch.h"

extern void OSReport(const char* format, ...);
extern const char lbl_8047897C;

typedef struct {
    u8 pad[0x24];
    u32 reg;
} OSVersionReg;

void OSExceptionVector(void) {
    extern void OSDefaultExceptionHandler(u8 exception, OSContext* context, u32 dsisr, u32 dar);

    asm {
        mtsprg 0, r4
        lwz r4, 0xc0(r0)
        stw r3, 0xc(r4)
        mfsprg r3, 0
        stw r3, 0x10(r4)
        stw r5, 0x14(r4)
        lhz r3, 0x1a2(r4)
        ori r3, r3, 0x2
        sth r3, 0x1a2(r4)
        mfcr r3
        stw r3, 0x80(r4)
        mflr r3
        stw r3, 0x84(r4)
        mfctr r3
        stw r3, 0x88(r4)
        mfxer r3
        stw r3, 0x8c(r4)
        mfsrr0 r3
        stw r3, 0x198(r4)
        mfsrr1 r3
        stw r3, 0x19c(r4)
        mr r5, r3
        nop
        mfmsr r3
        ori r3, r3, 0x30
        mtsrr1 r3
        li r3, 0
        lwz r4, 0xd4(r0)
        rlwinm. r5, r5, 0, 30, 30
        bne exc_lookup
        lis r5, OSDefaultExceptionHandler@ha
        addi r5, r5, OSDefaultExceptionHandler@l
        mtsrr0 r5
        rfi
    exc_lookup:
        clrlslwi r5, r3, 24, 2
        lwz r5, 0x3000(r5)
        mtsrr0 r5
        rfi
        nop
    }
}

#pragma push
#pragma optimize_for_size on
#pragma scheduling off
u32 fn_8009A23C(void) {
    return ((const volatile OSVersionReg*)0xCC006000)->reg & 0xFF;
}
#pragma scheduling reset
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma scheduling off
void OSRegisterVersion(const char* version) {
    OSReport(&lbl_8047897C, version);
}
#pragma scheduling reset
#pragma pop

void OSDefaultExceptionHandler(u8 exception, register OSContext* context, register u32 dsisr,
                                register u32 dar) {
    extern void __OSUnhandledException(u8 exception, OSContext* context, u32 dsisr, u32 dar);

    asm {
        stw r0, 0x0(context)
        stw r1, 0x4(context)
        stw r2, 0x8(context)
        stmw r6, 0x18(context)
        mfspr r0, GQR1
        stw r0, 0x1a8(context)
        mfspr r0, GQR2
        stw r0, 0x1ac(context)
        mfspr r0, GQR3
        stw r0, 0x1b0(context)
        mfspr r0, GQR4
        stw r0, 0x1b4(context)
        mfspr r0, GQR5
        stw r0, 0x1b8(context)
        mfspr r0, GQR6
        stw r0, 0x1bc(context)
        mfspr r0, GQR7
        stw r0, 0x1c0(context)
        mfdsisr dsisr
        mfdar dar
    }
    __OSUnhandledException(exception, context, dsisr, dar);
}

void __OSPSInit(void) {
    u32 hid2 = PPCMfhid2();
    PPCMthid2(hid2 | 0xA0000000);
    ICFlashInvalidate();
    asm {
        sync
        li r3, 0
        mtspr GQR0, r3
        mtspr GQR1, r3
        mtspr GQR2, r3
        mtspr GQR3, r3
        mtspr GQR4, r3
        mtspr GQR5, r3
        mtspr GQR6, r3
        mtspr GQR7, r3
    }
}

void OSInitAlarm(void) {
    typedef void (*OSExceptionHandler)(u8 exception, OSContext* context, u32 dsisr, u32 dar);
    typedef struct {
        void* head;
        void* tail;
    } OSAlarmQueue;
    extern OSExceptionHandler __OSGetExceptionHandler(u8 exception);
    extern OSExceptionHandler __OSSetExceptionHandler(u8 exception, OSExceptionHandler handler);
    extern void DecrementerExceptionHandler_8009A8DC(u8 exception, OSContext* context, u32 dsisr,
                                                      u32 dar);
    extern OSAlarmQueue AlarmQueue_8047A6E0;

    if (__OSGetExceptionHandler(8) != DecrementerExceptionHandler_8009A8DC) {
        AlarmQueue_8047A6E0.tail = NULL;
        AlarmQueue_8047A6E0.head = NULL;
        __OSSetExceptionHandler(8, DecrementerExceptionHandler_8009A8DC);
    }
}
