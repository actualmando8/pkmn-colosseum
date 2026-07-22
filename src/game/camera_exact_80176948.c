#include "game/camera_types.h"

void fn_80176948(f32 x, f32 y, f32 z)
{
    GSSceneVec3 direction;

    set__5GSvecFfff(&direction, x, y, z);
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->direction, &direction);
}

void cameraSetTargetPosXYZ(f32 x, f32 y, f32 z)
{
    GSSceneVec3 position;

    set__5GSvecFfff(&position, x, y, z);
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->position, &position);
}

void cameraSetTargetOfsXYZ(f32 x, f32 y, f32 z)
{
    GSSceneVec3 view;

    set__5GSvecFfff(&view, x, y, z);
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->view, &view);
}
