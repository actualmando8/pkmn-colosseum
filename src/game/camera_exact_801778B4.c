#include "game/camera_types.h"

void GSscene_GetCameraDirectionVector(GSSceneVec3* direction)
{
    GSvecCopy(direction, &((CameraPadState*) lbl_80478C40)->direction);
}

void GSscene_SetCameraDirectionVector(GSSceneVec3* direction)
{
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->direction, direction);
}

void GSscene_GetCameraPositionVector(GSSceneVec3* position)
{
    GSvecCopy(position, &((CameraPadState*) lbl_80478C40)->position);
}

void GSscene_SetCameraPositionVector(GSSceneVec3* position)
{
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->position, position);
}

void GSscene_GetCameraViewVector(GSSceneVec3* view)
{
    GSvecCopy(view, &((CameraPadState*) lbl_80478C40)->view);
}

void GSscene_SetCameraViewVector(GSSceneVec3* view)
{
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->view, view);
}
