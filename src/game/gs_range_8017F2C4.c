/**
 * @file gs_range_8017F2C4.c
 * @brief gs-engine code, 0x8017F2C4 - 0x80180C78 (21 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct GsRangeRequest {
    u8 _pad_0[0x20];
    s32 field_20;
    s32 field_24;
} GsRangeRequest;

#pragma optimize_for_size on
s32 fn_801808B4(volatile GsRangeRequest* req)
{
    volatile GsRangeRequest* ptr = req;
    s32 out;

    if (ptr->field_24 != 1) {
        ptr->field_20 = 0;
    }
    out = ptr->field_20;
    return out;
}
#pragma optimize_for_size reset
