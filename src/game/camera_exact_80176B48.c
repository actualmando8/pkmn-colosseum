#include "game/camera_types.h"
#include "game/gs_render_util.h"

extern u8 GScameraHasAnimationEnded(GSRenderCamera* camera);
extern u32 GSthreadGetCurrentThread(void);
extern const char lbl_80273F34[];

static inline GSRenderCamera* cameraGetCurrentAnimation(void)
{
    CameraPadState* state = lbl_80478C40;
    GSRenderCamera* animation = GSresGetResource(
        state->animationGroup, state->animationId);

    if (animation == NULL) {
        animation = fn_800F92D4(state->animationId);
    }
    return animation;
}

s32 cameraWaitSyncAnime(s32 sync)
{
    GSRenderCamera* animation;

    animation = cameraGetCurrentAnimation();
    if (animation == NULL) {
        return 0;
    }

    if ((u8)sync != 0) {
        for (;;) {
            if (GScameraHasAnimationEnded(animation) != 0) {
                break;
            }
            if (GSthreadGetCurrentThread() == 0) {
                GSlogWrite(lbl_80273F34);
                break;
            }
            _threadSwitch();
        }
    } else if (GScameraHasAnimationEnded(animation) == 0) {
        return 1;
    }

    return 0;
}
