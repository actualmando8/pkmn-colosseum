#include "game/camera_types.h"

typedef struct CameraFloorEntry {
    s32 initialized;
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

static inline CameraFloorEntry* cameraFindFloorEntry(void* floor)
{
    CameraFloorEntry* entries = (CameraFloorEntry*) lbl_8047B1A8;
    u32 i;

    for (i = 0; i < *(u32*) lbl_80478FB8; i++) {
        if (floor == entries[i].floor) {
            return &entries[i];
        }
    }
    return 0;
}

void cameraSetFov(f32 fov)
{
    CameraFloorEntry* floorEntry;

    if (fov < lbl_8047D728) {
        fov = lbl_8047D728;
    }
    if (fov > lbl_8047D72C) {
        fov = lbl_8047D72C;
    }

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->fov = fov;
    }
    ((CameraPadState*) lbl_80478C40)->fov = fov;
}

void cameraSetRotY(f32 angle)
{
    CameraFloorEntry* floorEntry;

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->rotationY = angle;
    }
    ((CameraPadState*) lbl_80478C40)->rotation.y = angle;
}

void cameraSetDistance(f32 distance)
{
    CameraFloorEntry* floorEntry;

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->distance = distance;
    }
    ((CameraPadState*) lbl_80478C40)->distance = distance;
}

void cameraSetHeight(f32 height)
{
    CameraFloorEntry* floorEntry;

    floorEntry = cameraFindFloorEntry(fn_800FF56C());
    if (floorEntry != NULL) {
        floorEntry->height = height;
    }
    ((CameraPadState*) lbl_80478C40)->height = height;
}
