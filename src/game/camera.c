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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void _cameraPadMoveUpdate__FP9_GScamera(void* camera) {
    /* TODO: match -- 996 bytes at 0x80179020 */
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

f32 fn_80176948(void* obj) {
    /* TODO: match -- 52 bytes at 0x80176948 */
}

f32 cameraSetTargetPosXYZ(void* obj) {
    /* TODO: match -- 52 bytes at 0x8017697C */
}

f32 cameraSetTargetOfsXYZ(void* obj) {
    /* TODO: match -- 52 bytes at 0x801769B0 */
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
    fn_800E01F4(tmp, a, b, c);
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
    fn_800E01F4(tmp, a, b, c);
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
