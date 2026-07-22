#include "game/camera_types.h"
#include "game/gs_render_util.h"

extern void GScameraSetAnimRate(GSRenderCamera* camera, f32 rate);
extern void GScameraStartAnimation(GSRenderCamera* camera);

void cameraSetAnimeRate(f32 rate)
{
    CameraPadState* state;
    GSRenderCamera* animation;

    state = lbl_80478C40;
    animation = (GSRenderCamera*) GSresGetResource(state->animationGroup,
                                                   state->animationId);
    if (animation == NULL) {
        animation = (GSRenderCamera*) fn_800F92D4(state->animationId);
    }
    if (animation != NULL) {
        GScameraSetAnimRate(animation, rate);
    }
}

void cameraStartAnimation(void)
{
    CameraPadState* state;
    GSRenderCamera* animation;

    state = lbl_80478C40;
    animation = (GSRenderCamera*) GSresGetResource(state->animationGroup,
                                                   state->animationId);
    if (animation == NULL) {
        animation = (GSRenderCamera*) fn_800F92D4(state->animationId);
    }
    if (animation != NULL) {
        GScameraStartAnimation(animation);
    }
}

void cameraStopAnimation(void)
{
    CameraPadState* state;
    GSRenderCamera* animation;

    state = lbl_80478C40;
    animation = (GSRenderCamera*) GSresGetResource(state->animationGroup,
                                                   state->animationId);
    if (animation == NULL) {
        animation = (GSRenderCamera*) fn_800F92D4(state->animationId);
    }
    if (animation != NULL) {
        GScameraStopAnimation(animation);
    }
}

void cameraStopAnime(void* object)
{
    CameraPadState* state;
    GSRenderCamera* animation;

    state = lbl_80478C40;
    animation = (GSRenderCamera*) GSresGetResource(state->animationGroup,
                                                   state->animationId);
    if (animation == NULL) {
        animation = (GSRenderCamera*) fn_800F92D4(state->animationId);
    }

    ((CameraPadState*) lbl_80478C40)->animationGroup = 0;
    ((CameraPadState*) lbl_80478C40)->animationId = 0;
    if (animation != NULL) {
        GScameraStopAnimation(animation);
    }
}
