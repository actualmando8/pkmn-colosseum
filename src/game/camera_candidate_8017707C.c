/** Candidate-only owner for 0x8017707C - 0x801773F4. */
#include "game/camera_types.h"
#include "game/data/sdata2_8047D690.h"

typedef struct CameraFloorEntry {
    s32 field_00;
    void* floor;
    f32 defaultHeight;
    f32 defaultDistance;
    f32 defaultRotationY;
    f32 defaultFov;
    f32 height;
    f32 distance;
    f32 rotationY;
    f32 fov;
} CameraFloorEntry;

extern const f32 lbl_8047D728;
extern const f32 lbl_8047D72C;

extern u32 fn_801174C4(void);
extern void fn_80117500(void);
extern void fn_80117330(f32 duration);

static CameraFloorEntry* cameraFindFloorEntry(void)
{
    CameraFloorEntry* entries;
    void* floor;
    u32 count;
    u32 i;

    floor = fn_800FF56C();
    entries = lbl_8047B1A8;
    count = *(u32*)lbl_80478FB8;
    for (i = 0; i < count; i++) {
        if (entries[i].floor == floor) {
            return &entries[i];
        }
    }
    return 0;
}

void cameraReturn(u8 wait, f32 duration)
{
    CameraPadState* state;
    CameraFloorEntry* floorEntry;
    GSSceneVec3 target;
    GSSceneVec3 rotation;
    void* model;
    f32 fov;
    f32 rotationY;

    state = lbl_80478C40;
    if (state->mode != 0) {
        state->mode = 0;
    }

    if (fn_801174C4() != 0) {
        fn_80117500();
        fn_80117330(duration);
    } else {
        floorEntry = cameraFindFloorEntry();
        rotationY = state->rotation.y;
        if (floorEntry != 0) {
            state->height = floorEntry->defaultHeight;
            floorEntry->height = floorEntry->defaultHeight;

            state->distance = floorEntry->defaultDistance;
            floorEntry->distance = floorEntry->defaultDistance;

            fov = floorEntry->defaultFov;
            if (fov < lbl_8047D728) {
                fov = lbl_8047D728;
            }
            if (fov > lbl_8047D72C) {
                fov = lbl_8047D72C;
            }
            state->fov = fov;
            floorEntry->fov = fov;
            rotationY = floorEntry->defaultRotationY;
        }

        target = lbl_80273DC8;
        state->targetGroup = 0;
        state->targetId = 100;
        state->targetSubId = -1;
        model = GSresGetResource(0, 100);
        if (model != 0) {
            GSmodelGetPosition(model, &target);
        }

        state->flags[0] = 1;
        GSvecCopy(&state->targetMoveEnd, &target);
        state->targetMoveTime = lbl_8047D740;
        state->targetMoveDuration = duration;
        state->targetMoveActive = 1;
        GSvecCopy(&state->targetMoveStart, &state->position);

        set__5GSvecFfff(&rotation, lbl_8047D740, rotationY,
                        lbl_8047D740);
        state->flags[0] = 1;
        GSvecCopy(&state->rotationMoveEnd, &rotation);
        state->rotationMoveTime = lbl_8047D740;
        state->rotationMoveDuration = duration;
        state->rotationMoveActive = 1;
        GSvecCopy(&state->rotationMoveStart, &state->rotation);
    }

    while (cameraMoveEndCheck(wait) != 0) {
    }
}
