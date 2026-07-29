/** Candidate-only owner for 0x801786F4 - 0x80179DFC. */
#include "src/game/camera.c"

void cameraSetFloorDefault(f32 height, f32 distance, f32 rotationY) {
    CameraFloorEntry* floorEntry;

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry == NULL) {
        return;
    }

    floorEntry->defaultHeight = height;
    floorEntry->defaultDistance = distance;
    floorEntry->defaultRotationY = rotationY;
    if (floorEntry->field_00 != 1) {
        return;
    }

    floorEntry->field_00 = 2;
    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->height = height;
    }
    ((CameraPadState*)lbl_80478C40)->height = height;

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->distance = distance;
    }
    ((CameraPadState*)lbl_80478C40)->distance = distance;

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->rotationY = rotationY;
    }
    ((CameraPadState*)lbl_80478C40)->rotation.y = rotationY;
}

void cameraResetFloor(void) {
    CameraFloorEntry* defaults;
    CameraFloorEntry* floorEntry;
    f32 value;

    if (((CameraPadState*)lbl_80478C40)->mode == 6) {
        if (((CameraPadState*)lbl_80478C40)->mode != 0) {
            ((CameraPadState*)lbl_80478C40)->mode = 0;
        }
    }

    defaults = cameraFindFloorEntry(fn_800FF56C());
    if (defaults == NULL) {
        return;
    }

    value = defaults->defaultHeight;
    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->height = value;
    }
    ((CameraPadState*)lbl_80478C40)->height = value;

    value = defaults->defaultDistance;
    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->distance = value;
    }
    ((CameraPadState*)lbl_80478C40)->distance = value;

    value = defaults->defaultRotationY;
    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->rotationY = value;
    }
    ((CameraPadState*)lbl_80478C40)->rotation.y = value;

    value = defaults->defaultFov;
    if (value < lbl_8047D728.value) {
        value = lbl_8047D728.value;
    }
    if (value > lbl_8047D72C.value) {
        value = lbl_8047D72C.value;
    }
    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->fov = value;
    }
    ((CameraPadState*)lbl_80478C40)->fov = value;
}

extern const GSSceneVec3 lbl_80273D98[4];
extern u8 lbl_80452EC8[];
extern u8* lbl_80478FBC;
extern void* fn_800D29A0(void);
extern void GSresRegisterResource(void* resource, u32 group, u32 id, u32 flags);
extern void clear__5GSvecFv(void* vector);
extern void fn_800FF4D4(void* data, u8 typeId);
extern void _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID(void);

void cameraInit(void) {
    GSSceneVec3 view = lbl_80273D98[0];
    GSSceneVec3 interest = lbl_80273D98[1];
    GSSceneVec3 eye = lbl_80273D98[2];
    GSSceneVec3 floorData = lbl_80273D98[3];
    CameraPadState* state;
    CameraFloorEntry* floorEntries;
    void* camera;
    u32 count;
    u32 i;
    u16 handle;

    memset(lbl_80452EC8, 0, sizeof(CameraPadState));
    lbl_80478C40 = lbl_80452EC8;
    camera = fn_800D29A0();
    GSresRegisterResource(camera, 0, 0, 0);

    state = lbl_80478C40;
    if (state->mode != 0) {
        state->mode = 0;
    }
    state->targetGroup = 0;
    state->targetId = 100;
    state->targetSubId = -1;
    GSvecCopy(&state->view, &view);

    count = *(u32*)lbl_80478FB8;
    handle = _toolentryAlloc__FUl(count * sizeof(CameraFloorEntry));
    lbl_8047B1AC = handle;
    floorEntries = fn_800E27B0(handle);
    lbl_8047B1A8 = floorEntries;
    memset(floorEntries, 0, count * sizeof(CameraFloorEntry));
    for (i = 0; i < count; i++) {
        floorEntries[i].field_00 = 0;
        floorEntries[i].floor =
            *(void**)(lbl_80478FBC + 0x0C + i * 0x4C);
    }

    clear__5GSvecFv(&state->offsetPosition);
    clear__5GSvecFv(&state->offsetRotation);
    set__5GSvecFfff(&state->offsetScale, lbl_8047D724,
                    lbl_8047D724, lbl_8047D724);
    state->fov = lbl_8047D720.value;
    fn_800FF4D4(&floorData, 1);
    fn_800FF4D4(&floorData, 2);
    GScameraLookAt((GSRenderCamera*)camera,
                   (const GSRenderVec3*)&interest,
                   (const GSRenderVec3*)&view);
    GScameraSetPosition(camera, &eye);
    fn_800D258C(camera);
    _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID();
}
