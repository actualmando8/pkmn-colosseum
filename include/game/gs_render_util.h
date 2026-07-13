#ifndef GAME_GS_RENDER_UTIL_H
#define GAME_GS_RENDER_UTIL_H

#include "dolphin/types.h"

/* ===================================================================
 * gs_render_util layout structs
 *
 * These describe the render-object ("camera") array element used by
 * gs_render_util.c (lbl_8047AA6C, stride 0x128), the compact snapshot
 * buffer used to save/restore camera state (fn_800D13C8/fn_800D1674),
 * and the small global render-state block pointed to by lbl_8047AA80.
 * =================================================================== */

typedef struct GSRenderVec3 {
    f32 x;
    f32 y;
    f32 z;
} GSRenderVec3;

typedef f32 GSRenderMtx[3][4];

typedef struct GSRenderCameraDesc {
    void* cobjDesc;
    void** animations;
} GSRenderCameraDesc;

typedef struct GSRenderCamera {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 useLookAt;
    /* 0x02 */ u8 dirty;
    /* 0x03 */ u8 hasAnimation;
    /* 0x04 */ u8 isAnimating;
    /* 0x05 */ u8 pad_05[3];
    /* 0x08 */ GSRenderCameraDesc* desc;
    /* 0x0C */ void* cobj;
    /* 0x10 */ u8 unk_10[0x60];
    /* 0x70 */ GSRenderVec3 eye;
    /* 0x7C */ GSRenderVec3 prevEye;
    /* 0x88 */ GSRenderVec3 rotation;
    /* 0x94 */ GSRenderMtx viewMtx;
    /* 0xC4 */ GSRenderMtx projectionMtx;
    /* 0xF4 */ GSRenderVec3 upVector;
    /* 0x100 */ GSRenderVec3 interest;
    /* 0x10C */ s32 animMode;
    /* 0x110 */ u32 animCount;
    /* 0x114 */ u32 animIndex;
    /* 0x118 */ f32 animRate;
    /* 0x11C */ f32 animFrame;
    /* 0x120 */ f32 animEndFrame;
    /* 0x124 */ s8 animEnded;
    /* 0x125 */ s8 animDirection;
    /* 0x126 */ u8 pad_126[2];
} GSRenderCamera; /* size 0x128 */

/* Compact save/restore snapshot for a GSRenderCamera (see fn_800D13C8,
 * fn_800D1674). Same field semantics as GSRenderCamera but packed
 * without the leading state-flag bytes / cobj pointers. */
typedef struct GSRenderCameraSnapshot {
    /* 0x00 */ u8 isAnimating;
    /* 0x01 */ u8 animEnded;
    /* 0x02 */ u8 pad_02[2];
    /* 0x04 */ GSRenderVec3 eye;
    /* 0x10 */ GSRenderVec3 prevEye;
    /* 0x1C */ GSRenderVec3 rotation;
    /* 0x28 */ GSRenderMtx viewMtx;
    /* 0x58 */ GSRenderMtx projectionMtx;
    /* 0x88 */ GSRenderVec3 upVector;
    /* 0x94 */ GSRenderVec3 interest;
    /* 0xA0 */ s32 animMode;
    /* 0xA4 */ u32 animIndex;
    /* 0xA8 */ f32 animRate;
    /* 0xAC */ f32 animFrame;
} GSRenderCameraSnapshot; /* size 0xB0 */

/* Global render-state block pointed to by lbl_8047AA80. Only the
 * fields actually touched by gs_render_util.c are named; everything
 * else is left as padding since this struct is never iterated/strided. */
typedef struct GSRenderState {
    /* 0x00 */ u8 unk_00[0x19];
    /* 0x19 */ u8 fogEnabled;
    /* 0x1A */ u8 pad_1A[2];
    /* 0x1C */ u8 fogColorR;
    /* 0x1D */ u8 fogColorG;
    /* 0x1E */ u8 fogColorB;
    /* 0x1F */ u8 fogColorA;
    /* 0x20 */ u8 unk_20[0x38];
    /* 0x58 */ u32 renderWidth;
    /* 0x5C */ u8 frameLevel;
} GSRenderState;

void GScameraGetDistanceVector(GSRenderCamera* camera, GSRenderVec3* dest);
void GScameraGetPerspective(GSRenderCamera* camera, f32* fov, f32* aspect,
                            f32* near, f32* far);
void GScameraSetPerspective(GSRenderCamera* camera, f32 fov, f32 aspect,
                            f32 near, f32 far);
void GScameraGetLookAt(GSRenderCamera* camera, void* up, void* interest);
void GScameraGetPosition(GSRenderCamera* camera, void* position);

#endif /* GAME_GS_RENDER_UTIL_H */
