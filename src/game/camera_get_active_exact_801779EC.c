#include "game/gs_render_util.h"
#include "game/gs_scene_types.h"

extern GSRenderCamera* GScameraGetActiveCamera(void);

GSRenderCamera* cameraGetActive(void)
{
    GSRenderCamera* camera = GScameraGetActiveCamera();

    if (camera != 0) {
        return camera;
    }
    camera = (GSRenderCamera*)GSresGetResource(0, 0);
    fn_800D258C(camera);
    return camera;
}
