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

extern void OSReport(const char* format, ...);
extern const char lbl_8047897C;

typedef struct {
    u8 pad[0x24];
    u32 reg;
} OSVersionReg;

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
