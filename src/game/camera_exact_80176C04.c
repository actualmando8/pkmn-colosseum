#include "game/gs_render_util.h"

extern f32 GScameraGetAnimFrame(GSRenderCamera* camera);

s32 fn_80176C04(u32 group, u32 id)
{
    GSRenderCamera* animation;

    if (group == 0 || id == 0) {
        return 0;
    }

    animation = (GSRenderCamera*) GSresGetResource(group, id);
    if (animation == NULL) {
        animation = (GSRenderCamera*) fn_800F92D4(id);
    }
    if (animation == NULL) {
        return 0;
    }

    return (s32) GScameraGetAnimFrame(animation);
}
