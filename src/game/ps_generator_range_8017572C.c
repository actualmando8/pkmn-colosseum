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
 *   psInitGenerator    (0x80175DF0)
 *   genPosUpdate       (0x80175E88)
 *   psGetNewIDNum      (0x80175F44)
 */

#include "game/gs_scene_types.h"

typedef struct GenPosJObj {
    u8 pad00[0x14];
    u32 flags;
    u8 pad18[0x2C];
    f32 matrix[3][4];
} GenPosJObj;

typedef struct GenPosGenerator {
    u8 pad00[0x20];
    f32 positionX;
    f32 positionY;
    f32 positionZ;
    u8 pad2C[0x5C];
    u16 flags;
    u8 pad8A[0x1A];
    GenPosJObj* jobj;
} GenPosGenerator;

typedef struct PSGeneratorPoolNode {
    struct PSGeneratorPoolNode* next;
    u8 data[0xB0];
} PSGeneratorPoolNode;

extern void* fn_801A6928(s32 size);
extern u16 lbl_8047B112;
extern u32 lbl_8047B180;
extern u32 lbl_8047B190;
extern u32 lbl_8047B194;
extern u32 lbl_8047B198;

static inline s32 genPosJObjMtxIsDirty(GenPosJObj* jobj) {
    extern void __assert(const char* file, u32 line, const char* condition);
    extern const char lbl_8047D6E0[7];
    extern const char lbl_8047D6E8[5];
    s32 result;

    if (jobj == NULL) {
        __assert(lbl_8047D6E0, 0x25D, lbl_8047D6E8);
    }
    result = FALSE;
    if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40)) {
        result = TRUE;
    }
    return result;
}

static inline void genPosJObjSetupMatrix(GenPosJObj* jobj) {
    extern void fn_8019D9DC(GenPosJObj* jobj);

    if (jobj == NULL || !genPosJObjMtxIsDirty(jobj)) {
        return;
    }
    fn_8019D9DC(jobj);
}

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

void psInitGenerator(s32 count) {
    s32 i;
    PSGeneratorPoolNode* node;

    lbl_8047B188 = NULL;
    lbl_8047B18C = NULL;

    for (i = count - 1; i >= 0; i--) {
        node = fn_801A6928(sizeof(PSGeneratorPoolNode));
        memset(node, 0, sizeof(PSGeneratorPoolNode));
        if (node == NULL) {
            return;
        }
        node->next = lbl_8047B18C;
        lbl_8047B18C = node;
    }

    lbl_8047B118 = 0;
    lbl_8047B112 = 0;
    lbl_8047B180 = 0;
    lbl_8047B190 = 0;
    lbl_8047B198 = 0;
    lbl_8047B194 = 0;
    lbl_8047B184 = NULL;
}

void genPosUpdate(GenPosGenerator* generator) {
    GenPosJObj* jobj;

    if (generator != NULL && !(generator->flags & 2) &&
        (generator->flags & 1)) {
        jobj = generator->jobj;
        if (jobj != NULL) {
            genPosJObjSetupMatrix(jobj);
            generator->positionX = generator->jobj->matrix[0][3];
            generator->positionY = generator->jobj->matrix[1][3];
            generator->positionZ = generator->jobj->matrix[2][3];
        }
    }
}

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
