#include "game/camera_types.h"

extern f32 lbl_8047D740;

void cameraMoveRotationXYZ(f32 x, f32 y, f32 z, f32 duration)
{
    GSSceneVec3 rotation;

    set__5GSvecFfff(&rotation, x, y, z);
    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->rotationMoveEnd, &rotation);
    ((CameraPadState*) lbl_80478C40)->rotationMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->rotationMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->rotationMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->rotationMoveStart,
              &((CameraPadState*) lbl_80478C40)->rotation);
}
