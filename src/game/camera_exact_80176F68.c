#include "game/camera_types.h"

void cameraMoveStop(void)
{
    ((CameraPadState*) lbl_80478C40)->targetMoveActive = 0;
    ((CameraPadState*) lbl_80478C40)->targetOffsetMoveActive = 0;
    ((CameraPadState*) lbl_80478C40)->positionMoveActive = 0;
    ((CameraPadState*) lbl_80478C40)->rotationMoveActive = 0;
    ((CameraPadState*) lbl_80478C40)->flags[0] = 0;
}
