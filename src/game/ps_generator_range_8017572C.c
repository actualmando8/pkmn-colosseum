/**
 * @file ps_generator_range_8017572C.c
 * @brief ps* -- particle generator object pool free/spawn/id-allocation
 *        tail (0x8017572C - 0x80175F6C).
 *
 * Split from the former game/gs_scene.c CodeCandidate bucket
 * (0x8017572C - 0x8017A5FC); see config/GC6E01/splits.txt for the exact
 * address ranges of the four resulting translation units:
 *   game/ps_generator_range_8017572C.c  0x8017572C - 0x80175F6C (this file)
 *   game/gs_xfb_capture.c               0x80175F6C - 0x80176068
 *   game/gs_spline.c                    0x80176068 - 0x801765F4
 *   game/camera.c                       0x801765F4 - 0x8017A5FC
 *
 * This unit is the TAIL of a particle-generator translation unit whose
 * head lives in the preceding bucket game/ps_range_80168C64.c; a future
 * re-split should consider merging them into one particle-generator
 * unit. Shared externs/typedefs for the whole former gs_scene.c range
 * live in include/game/gs_scene_types.h.
 *
 * Functions (7, per config/GC6E01/symbols.txt):
 *   psKillAllGenerator (0x8017572C)
 *   psKillGeneratorID  (0x801758D8, not yet decompiled)
 *   psKillGenerator    (0x80175A1C, not yet decompiled)
 *   psRemoveGenerator  (0x80175B94)
 *   psInitGenerator    (0x80175DF0, not yet decompiled)
 *   genPosUpdate       (0x80175E88, not yet decompiled)
 *   psGetNewIDNum      (0x80175F44)
 */

#include "game/gs_scene_types.h"

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void psKillAllGenerator(void) {
    /* TODO: match -- 428 bytes at 0x8017572C */
}
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* psRemoveGenerator(u32 type, u32 param) {
    /* TODO: match -- 604 bytes at 0x80175B94 */
}
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 psGetNewIDNum(void) {
    extern u16 lbl_80478C38;
    if (++lbl_80478C38 < 256) {
        lbl_80478C38 = 256;
    }
    return lbl_80478C38;
}
#pragma pop
