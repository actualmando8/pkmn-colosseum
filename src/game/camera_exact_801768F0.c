#include "game/camera_types.h"
#include "game/gs_render_util.h"


void cameraSetRotation(f32 x, f32 y, f32 z)
{
    GSSceneVec3 rotation;
    GSRenderCamera* camera;

    set__5GSvecFfff(&rotation, x, y, z);
    camera = (GSRenderCamera*) GSresGetResource(0, 0);
    GSvecCopy(&((CameraPadState*) lbl_80478C40)->rotation, &rotation);
    GScameraSetRotation(camera, (const GSRenderVec3*) &rotation);
}
