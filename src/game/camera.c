/**
 * @file camera.c
 * @brief GSscene camera system -- offset/target animation, movement,
 *        interpolation, XFB-driven camera update, floor/environment
 *        state, and camera init/save-state (0x801765F4 - 0x8017A5FC).
 *
 * Split from the former game/gs_scene.c CodeCandidate bucket
 * (0x8017572C - 0x8017A5FC); see config/GC6E01/splits.txt for the exact
 * address ranges of the four resulting translation units:
 *   game/ps_generator_range_8017572C.c  0x8017572C - 0x80175F6C
 *   game/gs_xfb_capture.c               0x80175F6C - 0x80176068
 *   game/gs_spline.c                    0x80176068 - 0x801765F4
 *   game/camera.c                       0x801765F4 - 0x8017A5FC (this file)
 *
 * This is the largest of the four TUs (63 functions). All 23 confirmed
 * anchors fall inside XD-era game/pxdvs/app/camera/camera.cpp
 * (0x80196A14 - 0x8019B01C); one anchor, cameraSetRotation, was
 * relocated within the TU by XD and is not itself decompiled here, so
 * it does not affect the strict-monotonicity read of this unit's
 * identity. Shared externs/typedefs for the whole former gs_scene.c
 * range live in include/game/gs_scene_types.h.
 *
 * Eleven addresses in this unit previously carried invented GSscene_*
 * names from an old campaign transplant that collided with real,
 * differently-named symbols.txt entries at the same address; those
 * were corrected to their real symbols.txt/fn_ identities in a prior
 * pass (bodies unchanged). The genuinely-matched GSscene_* names below
 * (GSscene_GetCameraRotationVector, GSscene_GetMode, etc., all 100%)
 * are real and untouched.
 *
 * 19 further fn_ -> name renames from this split's naming pass are
 * applied directly below (cameraSetOffsetRotation, cameraMoveTarget,
 * _cameraMakeStateData, etc. -- see the split commit message for the
 * full fn_ -> name mapping).
 */

#include "crt/math.h"
#include "game/gs_render_util.h"
#include "game/gs_scene_types.h"

#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void cameraUpdate(u32 captureIndex) {
    /* TODO: match -- 3064 bytes at 0x80177A64 */
}
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void _cameraPadRotateUpdate__FP9_GScamera(void* sceneObj) {
    /* TODO: match -- 1400 bytes at 0x80178AA8 */
}
#pragma pop
typedef struct CameraPadState {
    /* 0x00 */ u8 mode;
    /* 0x01 */ u8 flags[3];
    /* 0x04 */ GSSceneVec3 direction;
    /* 0x10 */ GSSceneVec3 rotation;
    /* 0x1C */ GSSceneVec3 position;
    /* 0x28 */ GSSceneVec3 view;
    /* 0x34 */ u32 targetGroup;
    /* 0x38 */ u32 targetId;
    /* 0x3C */ s32 targetSubId;
    /* 0x40 */ u8 field_40[8];
    /* 0x48 */ f32 fov;
} CameraPadState;

void* GSmodelGetPart(void* model, s32 partIndex);
void GSpartGetTransform(void* part, void* transform, u32 arg2, u32 arg3);
void GSpartFree(void* part);
void fn_800E0168(void* dst, void* lhs, void* rhs);

void cameraRefreshTargetPos(void) {
    void* object;

    object = GSresGetResource(
        ((CameraPadState*)lbl_80478C40)->targetGroup,
        ((CameraPadState*)lbl_80478C40)->targetId);
    if (object != 0) {
        GSmodelGetPosition(
            object, &((CameraPadState*)lbl_80478C40)->position);
        if (((CameraPadState*)lbl_80478C40)->targetSubId >= 0) {
            object = GSmodelGetPart(
                object, ((CameraPadState*)lbl_80478C40)->targetSubId);
            if (object != 0) {
                GSpartGetTransform(
                    object, &((CameraPadState*)lbl_80478C40)->view, 0, 0);
                fn_800E0168(
                    &((CameraPadState*)lbl_80478C40)->view,
                    &((CameraPadState*)lbl_80478C40)->view,
                    &((CameraPadState*)lbl_80478C40)->position);
                GSpartFree(object);
            }
        }
    }
}

typedef struct CameraFloorEntry {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ void* floor;
    /* 0x08 */ u8 field_08[0x1C];
    /* 0x24 */ f32 fov;
} CameraFloorEntry;

s32 fn_800D3088(void);
u8 dbgMenuIsOpen(void);
u32 fn_800F7AF0(s32 pad);
u32 fn_800F7BC4(s32 pad);
s32 fn_800F7A7C(s32 pad, s32 mode);
s32 fn_800F7A08(s32 pad, s32 mode);
s32 fn_800F7994(s32 pad, s32 mode);
s32 fn_800F7920(s32 pad, s32 mode);
void menuClose(u32 id);
void menuSetPosition(u32 id, s32 x, s32 y);
void fn_800E0718(void* out, const void* axis, f32 angle);
void fn_800E0738(void* out, const void* a, const void* b);
void GSvecTransformQuat(void* out, const void* quat, const void* vec);
void fn_80179748(f32 a, f32 b, f32 c, f32 d);
extern const GSSceneVec3 lbl_80315540;
extern const GSSceneVec3 lbl_8031554C;
extern const GSSceneVec3 lbl_80315558;

typedef struct CameraFloatConstant {
    f32 value;
} CameraFloatConstant;

extern const CameraFloatConstant lbl_8047D728;
extern const CameraFloatConstant lbl_8047D72C;
extern const CameraFloatConstant lbl_8047D780;
extern const CameraFloatConstant lbl_8047D784;
extern const CameraFloatConstant lbl_8047D788;

static inline CameraFloorEntry* cameraFindFloorEntry(void* floor) {
    CameraFloorEntry* entries = (CameraFloorEntry*)lbl_8047B1A8;
    u32 i;

    for (i = 0; i < *(u32*)lbl_80478FB8; i++) {
        if (floor == entries[i].floor) {
            return &entries[i];
        }
    }
    return 0;
}

#pragma push
#pragma optimization_level 4
void _cameraPadMoveUpdate__FP9_GScamera(void* camera) {
    GSSceneVec3 transformed;
    f32 quatX[4];
    f32 quatY[4];
    f32 quatZ[4];
    f32 quat[4];
    GSSceneVec3 movement;
    f32 far;
    f32 near;
    f32 fov;
    f32 aspect;
    CameraFloorEntry* floorEntry;
    s32 frameDelta;
    f32 value;
    f32 moveX;
    f32 moveZ;
    f32 rotateX;
    f32 rotateY;
    f32 movementDivisor;

    frameDelta = fn_800D3088();
    movementDivisor = lbl_8047D780.value;
    if (dbgMenuIsOpen()) {
        return;
    }

    if ((fn_800F7BC4(1) & fn_800F7AF0(1) & 0x400) != 0) {
        if ((u8)menuIsCheck(6)) {
            menuClose(6);
        } else {
            menuOpenCustom(6, 0, 0, 0, 1, 0);
            menuSetPosition(6, 20, 260);
        }
    }

    fn_800E0718(quatX, &lbl_80315540,
                 ((CameraPadState*)lbl_80478C40)->rotation.x);
    fn_800E0718(quatY, &lbl_8031554C,
                 ((CameraPadState*)lbl_80478C40)->rotation.y);
    fn_800E0718(quatZ, &lbl_80315558,
                 ((CameraPadState*)lbl_80478C40)->rotation.z);
    fn_800E0738(quat, quatX, quatZ);
    fn_800E0738(quat, quatY, quat);

    moveX = (s8)fn_800F7A7C(1, 0) * frameDelta;
    moveZ = (s8)fn_800F7A08(1, 0) * frameDelta;
    rotateX = (s8)fn_800F7994(1, 0) * frameDelta;
    rotateY = (s8)fn_800F7920(1, 0) * frameDelta;

    if ((fn_800F7BC4(1) & 0x40) != 0) {
        movementDivisor *= lbl_8047D784.value;
    }

    if ((fn_800F7BC4(1) & 0x800) != 0) {
        set__5GSvecFfff(&movement, moveX / movementDivisor,
                        -moveZ / movementDivisor,
                        lbl_8047D740);
    } else {
        set__5GSvecFfff(&movement, moveX / movementDivisor,
                        lbl_8047D740, moveZ / movementDivisor);
    }
    GSvecTransformQuat(&transformed, quat, &movement);
    GSvecAdd(&((CameraPadState*)lbl_80478C40)->direction,
             &((CameraPadState*)lbl_80478C40)->direction, &transformed);

    if ((fn_800F7BC4(1) & 0x20) != 0) {
        value = rotateY / movementDivisor;
        value += ((CameraPadState*)lbl_80478C40)->fov;
        moveX = value;
        if (value < lbl_8047D728.value) {
            moveX = lbl_8047D728.value;
        }
        if (moveX > lbl_8047D72C.value) {
            moveX = lbl_8047D72C.value;
        }

        floorEntry = cameraFindFloorEntry(fn_800FF56C());
        if (floorEntry != 0) {
            floorEntry->fov = moveX;
        }
        ((CameraPadState*)lbl_80478C40)->fov = moveX;
    } else {
        ((CameraPadState*)lbl_80478C40)->rotation.x -=
            rotateY / (lbl_8047D788.value * movementDivisor);
    }

    ((CameraPadState*)lbl_80478C40)->rotation.y -=
        rotateX / (lbl_8047D788.value * movementDivisor);
    GScameraSetPosition(camera, &((CameraPadState*)lbl_80478C40)->direction);
    GScameraSetRotation(camera, &((CameraPadState*)lbl_80478C40)->rotation);
    GScameraGetPerspective(camera, &aspect, &fov, &near, &far);
    GScameraSetPerspective(camera, ((CameraPadState*)lbl_80478C40)->fov,
                           fov, near, far);
}
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraSetGScamera(void* camera) {
    GSRenderCamera* source;
    GSRenderCamera* target;
    GSRenderVec3 dist;
    f32 perspective;
    f32 aspect;
    f32 near;
    f32 far;

    if (camera == 0) {
        return;
    }

    source = (GSRenderCamera*)camera;
    target = (GSRenderCamera*)GSresGetResource(0, 0);

    GScameraGetPerspective(camera, &perspective, &aspect, &near, &far);
    GScameraSetPerspective(target, perspective, aspect, near, far);
    GScameraGetDistanceVector(camera, &dist);

    if (((CameraPadState*)lbl_80478C40)->mode != 3) {
        ((CameraPadState*)lbl_80478C40)->mode = 3;
    }

    target->interest = source->interest;
    target->eye = source->eye;
    fn_80179748(dist.y, dist.z, (f32)atan2(dist.x, dist.z), perspective);
}
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80179FA4(void) {
    /* TODO: match -- 1624 bytes at 0x80179FA4 */
}
#pragma pop
void fn_801765F4(u8 value) {
    extern void* lbl_80478C40;
    *((u8*)lbl_80478C40 + 3) = value;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off

void fn_80176948(void) {
    extern void fn_800E01D0(void* dst, void* src);
    void fn_800E01F4();
    u8 local[24];
    fn_800E01F4(local);
    fn_800E01D0(&((GSCameraStateVectors*)lbl_80478C40)->direction, local);
}

void cameraSetTargetPosXYZ(void) {
    extern void fn_800E01D0(void* dst, void* src);
    void fn_800E01F4();
    u8 local[24];
    fn_800E01F4(local);
    fn_800E01D0(&((GSCameraStateVectors*)lbl_80478C40)->position, local);
}

void cameraSetTargetOfsXYZ(void) {
    extern void fn_800E01D0(void* dst, void* src);
    void fn_800E01F4();
    u8 local[24];
    fn_800E01F4(local);
    fn_800E01D0(&((GSCameraStateVectors*)lbl_80478C40)->view, local);
}

#pragma pop
u32 _cameraGetStateSize(void) { return 428; }
#if 0
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void cameraSetOffsetRotation(void) {
#include "src/game/gs_scene_fn_8017662C.inc"
}
#pragma pop
#else
void cameraSetOffsetRotation(void* src) {
    GSvecCopy((u8*)lbl_80478C40 + 0xE4, src);
}
#endif
#if 0
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void cameraSetOffsetPosition(void) {
#include "src/game/gs_scene_fn_80176658.inc"
}
#pragma pop
#else
void cameraSetOffsetPosition(void* src) {
    GSvecCopy((u8*)lbl_80478C40 + 0xD8, src);
}
#endif
#if 0
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void cameraGetRotY(void) {
#include "src/game/gs_scene_cameraGetRotY.inc"
}
#pragma pop
#else
f32 cameraGetRotY(void) {
    return *(f32*)((u8*)lbl_80478C40 + 0x14);
}
#endif
#if 0
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void cameraGetHeight(void) {
#include "src/game/gs_scene_cameraGetHeight.inc"
}
#pragma pop
#else
f32 cameraGetHeight(void) {
    return *(f32*)((u8*)lbl_80478C40 + 0x40);
}
#endif
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void cameraSetFov(f32 fov) {
    CameraFloorEntry* floorEntry;

    if (fov < lbl_8047D728.value) {
        fov = lbl_8047D728.value;
    }
    if (fov > lbl_8047D72C.value) {
        fov = lbl_8047D72C.value;
    }

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != 0) {
        floorEntry->fov = fov;
    }
    ((CameraPadState*)lbl_80478C40)->fov = fov;
}
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraStopAnimation(void) {
#include "src/game/gs_scene_fn_80176A94.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraStopAnimation(void) {
    void* p;
    void* r;
    p = lbl_80478C40;
    r = GSresGetResource(*(u32*)((u8*)p + 0xD0), *(u32*)((u8*)p + 0xD4));
    if (r == 0)
        r = fn_800F92D4(*(u32*)((u8*)p + 0xD4));
    if (r != 0)
        GScameraStopAnimation(r);
}
#pragma pop
#endif
#if 0
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void cameraMoveStop(void) {
#include "src/game/gs_scene_fn_80176F68.inc"
}
#pragma pop
#else
void cameraMoveStop(void) {
    *((u8*)lbl_80478C40 + 0x4C) = 0;
    *((u8*)lbl_80478C40 + 0x4D) = 0;
    *((u8*)lbl_80478C40 + 0x4E) = 0;
    *((u8*)lbl_80478C40 + 0x4F) = 0;
    *((u8*)lbl_80478C40 + 0x01) = 0;
}
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMoveEndCheckSpecial(void) {
#include "src/game/gs_scene_cameraMoveEndCheckSpecial.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
u32 cameraMoveEndCheckSpecial(u8 param) {
    void* p;
    for (;;) {
        p = lbl_80478C40;
        if (*((u8*)p + 0x4C) != 0) goto nonzero;
        if (*((u8*)p + 0x4E) != 0) goto nonzero;
        if (*((u8*)p + 0x4F) != 0) goto nonzero;
        return 0;
    nonzero:
        if (param != 0) {
            _threadSwitch();
        } else {
            return 1;
        }
    }
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMoveRotation(void) {
#include "src/game/gs_scene_fn_80177478.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraMoveRotation(void* unused, void* src, f32 param) {
    void* p;
    p = lbl_80478C40;
    *((u8*)p + 0x01) = 1;
    p = lbl_80478C40;
    GSvecCopy((u8*)p + 0xB0, src);
    {
        void* q = lbl_80478C40;
        *(f32*)((u8*)q + 0xCC) = lbl_8047D740;
        q = lbl_80478C40;
        *(f32*)((u8*)q + 0xC8) = param;
        q = lbl_80478C40;
        *((u8*)q + 0x4F) = 1;
    }
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0xBC, (u8*)q + 0x10);
    }
}
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMovePositionXYZ(void) {
#include "src/game/gs_scene_fn_801774F0.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraMovePositionXYZ(f32 a, f32 b, f32 c, f32 angle) {
    f32 tmp[3];
    void* p;
    set__5GSvecFfff(tmp, a, b, c);
    p = lbl_80478C40;
    *((u8*)p + 0x01) = 1;
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x90, tmp);
    }
    {
        void* q = lbl_80478C40;
        *(f32*)((u8*)q + 0xAC) = lbl_8047D740;
        q = lbl_80478C40;
        *(f32*)((u8*)q + 0xA8) = angle;
        q = lbl_80478C40;
        *((u8*)q + 0x4E) = 1;
    }
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x9C, (u8*)q + 0x4);
    }
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMovePosition(void) {
#include "src/game/gs_scene_fn_80177574.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraMovePosition(void* unused, void* src, f32 param) {
    void* p;
    p = lbl_80478C40;
    *((u8*)p + 0x01) = 1;
    p = lbl_80478C40;
    GSvecCopy((u8*)p + 0x90, src);
    {
        void* q = lbl_80478C40;
        *(f32*)((u8*)q + 0xAC) = lbl_8047D740;
        q = lbl_80478C40;
        *(f32*)((u8*)q + 0xA8) = param;
        q = lbl_80478C40;
        *((u8*)q + 0x4E) = 1;
    }
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x9C, (u8*)q + 0x4);
    }
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMoveTargetXYZ(void) {
#include "src/game/gs_scene_fn_801775EC.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraMoveTargetXYZ(f32 a, f32 b, f32 c, f32 angle) {
    f32 tmp[3];
    void* p;
    set__5GSvecFfff(tmp, a, b, c);
    p = lbl_80478C40;
    *((u8*)p + 0x01) = 1;
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x50, tmp);
    }
    {
        void* q = lbl_80478C40;
        *(f32*)((u8*)q + 0x6C) = lbl_8047D740;
        q = lbl_80478C40;
        *(f32*)((u8*)q + 0x68) = angle;
        q = lbl_80478C40;
        *((u8*)q + 0x4C) = 1;
    }
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x5C, (u8*)q + 0x1C);
    }
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMoveTargetPos(void) {
#include "src/game/gs_scene_fn_801776E8.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraMoveTargetPos(void* unused, void* src, f32 param) {
    void* p;
    p = lbl_80478C40;
    *((u8*)p + 0x01) = 1;
    p = lbl_80478C40;
    GSvecCopy((u8*)p + 0x50, src);
    {
        void* q = lbl_80478C40;
        *(f32*)((u8*)q + 0x6C) = lbl_8047D740;
        q = lbl_80478C40;
        *(f32*)((u8*)q + 0x68) = param;
        q = lbl_80478C40;
        *((u8*)q + 0x4C) = 1;
    }
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x5C, (u8*)q + 0x1C);
    }
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMoveTargetOfs(void) {
#include "src/game/gs_scene_fn_80177670.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraMoveTargetOfs(void* unused, void* src, f32 param) {
    void* p;
    p = lbl_80478C40;
    *((u8*)p + 0x01) = 1;
    p = lbl_80478C40;
    GSvecCopy((u8*)p + 0x70, src);
    {
        void* q = lbl_80478C40;
        *(f32*)((u8*)q + 0x8C) = lbl_8047D740;
        q = lbl_80478C40;
        *(f32*)((u8*)q + 0x88) = param;
        q = lbl_80478C40;
        *((u8*)q + 0x4D) = 1;
    }
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x7C, (u8*)q + 0x28);
    }
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraMoveTarget(void) {
#include "src/game/gs_scene_fn_80177760.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void cameraMoveTarget(void* unused, u32 a, u32 b, f32 param) {
    u32 local[3];
    void* handle;
    local[0] = lbl_80273DC8[0];
    local[1] = lbl_80273DC8[1];
    local[2] = lbl_80273DC8[2];
    {
        void* p = lbl_80478C40;
        *(u32*)((u8*)p + 0x34) = a;
        p = lbl_80478C40;
        *(u32*)((u8*)p + 0x38) = b;
        p = lbl_80478C40;
        *(u32*)((u8*)p + 0x3C) = (u32)-1;
    }
    handle = GSresGetResource(a, b);
    if (handle != 0) {
        GSmodelGetPosition(handle, local);
    }
    {
        void* p = lbl_80478C40;
        *((u8*)p + 0x01) = 1;
        p = lbl_80478C40;
        GSvecCopy((u8*)p + 0x50, local);
    }
    {
        void* q = lbl_80478C40;
        *(f32*)((u8*)q + 0x6C) = lbl_8047D740;
        q = lbl_80478C40;
        *(f32*)((u8*)q + 0x68) = param;
        q = lbl_80478C40;
        *((u8*)q + 0x4C) = 1;
    }
    {
        void* q = lbl_80478C40;
        GSvecCopy((u8*)q + 0x5C, (u8*)q + 0x1C);
    }
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_GetCameraRotationVector(void) {
#include "src/game/gs_scene_fn_80177830.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void GSscene_GetCameraRotationVector(void* dst) {
    GSvecCopy(dst, (u8*)lbl_80478C40 + 0x10);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_SetCameraRotationVector(void) {
#include "src/game/gs_scene_fn_80177858.inc"
}
#else
void GSscene_SetCameraRotationVector(void* src) {
    void* handle;
    handle = GSresGetResource(0, 0);
    GSvecCopy((u8*)lbl_80478C40 + 0x10, src);
    GScameraSetRotation(handle, src);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_GetCameraDirectionVector(void) {
#include "src/game/gs_scene_fn_801778B4.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void GSscene_GetCameraDirectionVector(void* dst) {
    GSvecCopy(dst, (u8*)lbl_80478C40 + 0x4);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_SetCameraDirectionVector(void) {
#include "src/game/gs_scene_fn_801778DC.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void GSscene_SetCameraDirectionVector(void* src) {
    GSvecCopy((u8*)lbl_80478C40 + 0x4, src);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_GetCameraPositionVector(void) {
#include "src/game/gs_scene_fn_80177908.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void GSscene_GetCameraPositionVector(void* dst) {
    GSvecCopy(dst, (u8*)lbl_80478C40 + 0x1C);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_SetCameraPositionVector(void) {
#include "src/game/gs_scene_fn_80177930.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void GSscene_SetCameraPositionVector(void* src) {
    GSvecCopy((u8*)lbl_80478C40 + 0x1C, src);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_GetCameraViewVector(void) {
#include "src/game/gs_scene_fn_8017795C.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void GSscene_GetCameraViewVector(void* dst) {
    GSvecCopy(dst, (u8*)lbl_80478C40 + 0x28);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_SetCameraViewVector(void) {
#include "src/game/gs_scene_fn_80177984.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void GSscene_SetCameraViewVector(void* src) {
    GSvecCopy((u8*)lbl_80478C40 + 0x28, src);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraSetTargetExt(void) {
#include "src/game/gs_scene_fn_801779B0.inc"
}
#else
#pragma push
#pragma optimization_level 1
void cameraSetTargetExt(u32 a, u32 b, u32 c) {
    *(u32*)((u8*)lbl_80478C40 + 0x34) = a;
    *(u32*)((u8*)lbl_80478C40 + 0x38) = b;
    *(u32*)((u8*)lbl_80478C40 + 0x3C) = c;
}
#pragma pop
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void cameraSetTarget(void) {
#include "src/game/gs_scene_cameraSetTarget.inc"
}
#else
#pragma push
#pragma optimization_level 1
void cameraSetTarget(u32 a, u32 b) {
    *(u32*)((u8*)lbl_80478C40 + 0x34) = a;
    *(u32*)((u8*)lbl_80478C40 + 0x38) = b;
    *(u32*)((u8*)lbl_80478C40 + 0x3C) = (u32)-1;
}
#pragma pop
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void GSscene_GetMode(void) {
#include "src/game/gs_scene_fn_80177A38.inc"
}
#else
u32 GSscene_GetMode(void) {
    return *(u8*)lbl_80478C40;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void GSscene_SetMode(void) {
#include "src/game/gs_scene_fn_80177A44.inc"
}
#else
u32 GSscene_SetMode(u32 val) {
    u8* state = lbl_80478C40;
    u32 old;
    if (state[0] == (u8)val) return val;
    old = state[0];
    state[0] = val;
    return old;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void _cameraMakeStateData(void) {
#include "src/game/gs_scene_fn_80179E04.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma push
#pragma optimization_level 1
void _cameraMakeStateData(void* dst) {
    void* ptr;
    void* out;
    void* handle;
    u32 mode;
    out = dst;
    handle = GSresGetResource(0, 0);
    ptr = lbl_80478C40;
    mode = *(u8*)ptr;
    if (mode == 4 || mode == 8) {
        void* next;
        next = GSresGetResource(*(u32*)((u8*)ptr + 0xD0), *(u32*)((u8*)ptr + 0xD4));
        if (next == 0)
            next = fn_800F92D4(*(u32*)((u8*)ptr + 0xD4));
        handle = next;
    }
    memcpy(out, lbl_80478C40, 0xFC);
    fn_800D1674(handle, (u8*)out + 0xFC);
}
#pragma pop
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void _cameraRestoreStateData(void) {
#include "src/game/gs_scene_fn_80179EA4.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 4
void _cameraRestoreStateData(void* src) {
    u32 src_val;
    void* saved_src;
    void* ptr;
    void* handle;
    void* next;
    u8 r0;
    src_val = (u32)src;
    saved_src = (void*)src_val;
    memcpy(lbl_80478C40, saved_src, 0xFC);
    handle = GSresGetResource(0, 0);
    ptr = lbl_80478C40;
    r0 = *(u8*)ptr;
    if (r0 == 4 || r0 == 8) {
        next = GSresGetResource(*(u32*)((u8*)ptr + 0xD0), *(u32*)((u8*)ptr + 0xD4));
        if (next == 0) {
            next = fn_800F92D4(*(u32*)((u8*)ptr + 0xD4));
        }
        handle = next;
    }
    fn_800D13C8(handle, (u8*)saved_src + 0xFC);
    fn_800D258C(handle);
}
#pragma pop
#endif
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80179F4C(void) {
#include "src/game/gs_scene_fn_80179F4C.inc"
}
#pragma pop
#else
#pragma pop
#pragma push
#pragma optimization_level 1
void fn_80179F4C(u32 param) {
    volatile u32* saved = &param;

    lbl_80478C4C = *saved;
    if ((u8)menuIsCheck(0xFE) == 0) {
        menuOpenCustom(0xFE, 0, 0, 0, 0, 0);
    }
}
#pragma pop
#endif
