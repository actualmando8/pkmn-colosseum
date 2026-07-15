/**
 * @file sdk_range_800BA1B4.c
 * @brief dolphin-sdk code, 0x800BA1B4 - 0x800BA414 (2 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct GXLightObj_800BA344 {
    u8 pad_00[0x1C];
    f32 k0;
    f32 k1;
    f32 k2;
} GXLightObj_800BA344;

typedef enum GXDistAttnFn_800BA344 {
    GX_DA_OFF_800BA344,
    GX_DA_GENTLE_800BA344,
    GX_DA_MEDIUM_800BA344,
    GX_DA_STEEP_800BA344,
} GXDistAttnFn_800BA344;

void GXInitLightDistAttn(GXLightObj_800BA344* light, f32 ref_dist,
                         f32 ref_br, GXDistAttnFn_800BA344 dist_fn)
{
    f32 k0;
    f32 k1;
    f32 k2;

    if (ref_dist < 0.0f) {
        dist_fn = GX_DA_OFF_800BA344;
    }
    if (ref_br <= 0.0f || ref_br >= 1.0f) {
        dist_fn = GX_DA_OFF_800BA344;
    }

    switch (dist_fn) {
    case GX_DA_GENTLE_800BA344:
        k0 = 1.0f;
        k1 = (1.0f - ref_br) / (ref_br * ref_dist);
        k2 = 0.0f;
        break;
    case GX_DA_MEDIUM_800BA344:
        k0 = 1.0f;
        k1 = 0.5f * (1.0f - ref_br) / (ref_br * ref_dist);
        k2 = 0.5f * (1.0f - ref_br) /
             (ref_br * ref_dist * ref_dist);
        break;
    case GX_DA_STEEP_800BA344:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = (1.0f - ref_br) / (ref_br * ref_dist * ref_dist);
        break;
    case GX_DA_OFF_800BA344:
    default:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = 0.0f;
        break;
    }

    light->k0 = k0;
    light->k1 = k1;
    light->k2 = k2;
}
