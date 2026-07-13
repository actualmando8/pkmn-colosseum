/**
 * @file sdk_range_8009A2D8.c
 * @brief dolphin-sdk code, 0x8009A2D8 - 0x8009AFB0 (13 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct {
    s32 unk0;
    u32 unk4;
    u32 unk8;
} AlarmCallback;

extern u32 lbl_80478980;
extern AlarmCallback* lbl_8047A6E8;

u32 fn_8009AB50(u32 xfb) {
    u32 previous = lbl_80478980;

    lbl_80478980 = xfb;
    return previous;
}

void fn_8009AC3C(u32 xfb) {
    lbl_8047A6E8[xfb].unk0 = -1;
}
