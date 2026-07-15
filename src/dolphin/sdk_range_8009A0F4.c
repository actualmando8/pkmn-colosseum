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
#include "dolphin/os/OSContext.h"

extern void OSReport(const char* format, ...);
extern const char lbl_8047897C;

typedef struct {
    u8 pad[0x24];
    u32 reg;
} OSVersionReg;

u16 OSExceptionVector(u32 savedR3, u32 savedR4, u32 savedR5) {
    OSContext* context = *(OSContext* volatile*)0xC0;

    context->gpr[3] = savedR3;
    context->gpr[5] = savedR5;
    return context->state |= OS_CONTEXT_STATE_EXC;
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
