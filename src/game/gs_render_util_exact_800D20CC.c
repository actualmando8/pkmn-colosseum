#include "dolphin/types.h"
#include "game/gs_render_util.h"
#include "hsd/hsd_cobj.h"

void GScameraSetPerspective(GSRenderCamera* camera, f32 fov, f32 aspect,
                            f32 near, f32 far)
{
    HSD_CObjSetProjectionType((HSD_CObj*) camera->cobj, 1);
    HSD_CObjSetPerspective((HSD_CObj*) camera->cobj, fov, aspect);
    HSD_CObjSetNear((HSD_CObj*) camera->cobj, near);
    HSD_CObjSetFar((HSD_CObj*) camera->cobj, far);
    camera->dirty = 1;
}
