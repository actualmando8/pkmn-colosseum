/**
 * @file sdk_range_800BF33C.c
 * @brief dolphin-sdk code, 0x800BF33C - 0x800BF534 (2 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

s32 usr_puts_serial(char* str) {
    extern u32 fn_800C04F4(void);
    extern void fn_800C04E8(u32 state);
    extern void OSReport(const char* fmt);
    u16 buf;
    u8 zero;
    s32 state;
    char ch;
    s32 result;

    zero = 0;
    result = 0;
    while ((result == 0) && ((ch = *str++) != 0)) {
        state = fn_800C04F4();
        ((u8*)&buf)[0] = ch;
        ((u8*)&buf)[1] = zero;
        fn_800C04E8(0);
        OSReport((const char*)&buf);
        fn_800C04E8(state);
        result = 0;
    }
    return result;
}
