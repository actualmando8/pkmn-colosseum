#include "game/gs_render_util.h"

extern void GSvecCopy(GSRenderVec3* dst, const GSRenderVec3* src);
extern void HSD_CObjGetEyePosition(void* cobj, GSRenderVec3* position);
extern void HSD_CObjGetPerspective(void* cobj, f32* fov, f32* aspect);
extern f32 HSD_CObjGetNear(void* cobj);
extern f32 HSD_CObjGetFar(void* cobj);

void GScameraGetRotation(GSRenderCamera* camera, GSRenderVec3* rotation)
{
    GSvecCopy(rotation, &camera->rotation);
}

void GScameraGetPosition(GSRenderCamera* camera, GSRenderVec3* position)
{
    if (camera->isAnimating != 0) {
        HSD_CObjGetEyePosition(camera->cobj, &camera->eye);
    }
    GSvecCopy(position, &camera->eye);
}

void GScameraGetPerspective(GSRenderCamera* camera, f32* fov, f32* aspect,
                            f32* near, f32* far)
{
    HSD_CObjGetPerspective(camera->cobj, fov, aspect);
    *near = HSD_CObjGetNear(camera->cobj);
    *far = HSD_CObjGetFar(camera->cobj);
}

void GScameraSetRotation(GSRenderCamera* camera,
                         const GSRenderVec3* rotation)
{
    GSvecCopy(&camera->rotation, rotation);
    camera->dirty = 1;
    camera->useLookAt = 0;
}
