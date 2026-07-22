#include "game/camera_types.h"

void fn_801765F4(s32 value)
{
    ((CameraPadState*) lbl_80478C40)->flags[2] = value;
}

void cameraSetOffsetScale(GSSceneVec3* scale)
{
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->offsetScale, scale);
}

void cameraSetOffsetRotation(GSSceneVec3* rotation)
{
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->offsetRotation, rotation);
}

void cameraSetOffsetPosition(GSSceneVec3* position)
{
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->offsetPosition, position);
}

f32 cameraGetRotY(void)
{
    return ((CameraPadState*) lbl_80478C40)->rotation.y;
}

f32 cameraGetDistance(void)
{
    return ((CameraPadState*) lbl_80478C40)->distance;
}

f32 cameraGetHeight(void)
{
    return ((CameraPadState*) lbl_80478C40)->height;
}
