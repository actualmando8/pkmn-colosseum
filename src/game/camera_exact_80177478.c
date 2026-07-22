#include "game/camera_types.h"
#include "game/data/sdata2_8047D690.h"
#include "game/gs_render_util.h"

void cameraMoveRotation(void* unused, GSSceneVec3* rotation, f32 duration)
{
    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->rotationMoveEnd, rotation);
    ((CameraPadState*) lbl_80478C40)->rotationMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->rotationMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->rotationMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->rotationMoveStart,
              &((CameraPadState*) lbl_80478C40)->rotation);
}

void cameraMovePositionXYZ(f32 x, f32 y, f32 z, f32 duration)
{
    GSSceneVec3 position;
    set__5GSvecFfff(&position, x, y, z);
    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->positionMoveEnd, &position);
    ((CameraPadState*) lbl_80478C40)->positionMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->positionMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->positionMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->positionMoveStart,
              &((CameraPadState*) lbl_80478C40)->direction);
}

void cameraMovePosition(void* unused, GSSceneVec3* position, f32 duration)
{
    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->positionMoveEnd, position);
    ((CameraPadState*) lbl_80478C40)->positionMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->positionMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->positionMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->positionMoveStart,
              &((CameraPadState*) lbl_80478C40)->direction);
}

void cameraMoveTargetXYZ(f32 x, f32 y, f32 z, f32 duration)
{
    GSSceneVec3 target;
    set__5GSvecFfff(&target, x, y, z);
    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetMoveEnd, &target);
    ((CameraPadState*) lbl_80478C40)->targetMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->targetMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->targetMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetMoveStart,
              &((CameraPadState*) lbl_80478C40)->position);
}

void cameraMoveTargetOfs(void* unused, GSSceneVec3* offset, f32 duration)
{
    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetOffsetMoveEnd, offset);
    ((CameraPadState*) lbl_80478C40)->targetOffsetMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->targetOffsetMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->targetOffsetMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetOffsetMoveStart,
              &((CameraPadState*) lbl_80478C40)->view);
}

void cameraMoveTargetPos(void* unused, GSSceneVec3* target, f32 duration)
{
    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetMoveEnd, target);
    ((CameraPadState*) lbl_80478C40)->targetMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->targetMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->targetMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetMoveStart,
              &((CameraPadState*) lbl_80478C40)->position);
}

void cameraMoveTarget(void* unused, u32 group, u32 id, f32 duration)
{
    GSSceneVec3 target;
    void* model;

    target = lbl_80273DC8;

    ((CameraPadState*) lbl_80478C40)->targetGroup = group;
    ((CameraPadState*) lbl_80478C40)->targetId = id;
    ((CameraPadState*) lbl_80478C40)->targetSubId = -1;

    model = GSresGetResource(group, id);
    if (model != NULL) {
        GSmodelGetPosition(model, &target);
    }

    ((CameraPadState*) lbl_80478C40)->flags[0] = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetMoveEnd, &target);
    ((CameraPadState*) lbl_80478C40)->targetMoveTime = lbl_8047D740;
    ((CameraPadState*) lbl_80478C40)->targetMoveDuration = duration;
    ((CameraPadState*) lbl_80478C40)->targetMoveActive = 1;
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->targetMoveStart,
              &((CameraPadState*) lbl_80478C40)->position);
}

void GSscene_GetCameraRotationVector(GSSceneVec3* rotation)
{
    GSvecCopy(rotation, &((CameraPadState*) lbl_80478C40)->rotation);
}

void GSscene_SetCameraRotationVector(GSSceneVec3* rotation)
{
    GSRenderCamera* camera;

    camera = (GSRenderCamera*) GSresGetResource(0, 0);
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->rotation, rotation);
    GScameraSetRotation(camera, (const GSRenderVec3*) rotation);
}
