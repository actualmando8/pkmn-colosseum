/**
 * @file gs_spline.c
 * @brief GSspline -- spline control-vector free/create management
 *        (0x80176068 - 0x801765F4).
 *
 * Split from the former game/gs_scene.c CodeCandidate bucket
 * (0x8017572C - 0x8017A5FC); see config/GC6E01/splits.txt for the exact
 * address ranges of the four resulting translation units:
 *   game/ps_generator_range_8017572C.c  0x8017572C - 0x80175F6C
 *   game/gs_xfb_capture.c               0x80175F6C - 0x80176068
 *   game/gs_spline.c                    0x80176068 - 0x801765F4 (this file)
 *   game/camera.c                       0x801765F4 - 0x8017A5FC
 *
 * Corresponds to game/pxdvs/GSAPI/GSspline/GSspline.cpp in the XD-era
 * source tree. Shared externs/typedefs for the whole former gs_scene.c
 * range live in include/game/gs_scene_types.h.
 *
 * Functions (3, per config/GC6E01/symbols.txt):
 *   GSsplineFree                  (0x80176068, was fn_80176068)
 *   GSsplineAddControlVectorValue (0x801760C4, not yet decompiled)
 *   GSsplineCreate                (0x80176228, not yet decompiled)
 */

#include "game/gs_scene_types.h"

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSsplineFree(void) {
#include "src/game/gs_scene_fn_80176068.inc"
}
#else
#pragma optimization_level 4
void GSsplineFree(u8* ptr) {
    u16 handle;
    if (ptr != NULL) {
        handle = *(u16*)(ptr + 0x22);
        fn_800E24B0(handle);
        fn_800E209C(handle);
        handle = *(u16*)(ptr + 0x20);
        fn_800E24B0(handle);
        fn_800E209C(handle);
    }
}
#endif
#pragma pop
