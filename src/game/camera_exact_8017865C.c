#include "game/camera_types.h"

void* GSmodelGetPart(void* model, s32 partIndex);
void GSpartGetTransform(void* part, void* transform, u32 arg2, u32 arg3);
void GSpartFree(void* part);
void fn_800E0168(void* dst, void* lhs, void* rhs);

void cameraRefreshTargetPos(void)
{
    void* object;

    object = GSresGetResource(
        ((CameraPadState*)lbl_80478C40)->targetGroup,
        ((CameraPadState*)lbl_80478C40)->targetId);
    if (object != 0) {
        GSmodelGetPosition(
            object, &((CameraPadState*)lbl_80478C40)->position);
        if (((CameraPadState*)lbl_80478C40)->targetSubId >= 0) {
            object = GSmodelGetPart(
                object, ((CameraPadState*)lbl_80478C40)->targetSubId);
            if (object != 0) {
                GSpartGetTransform(
                    object, &((CameraPadState*)lbl_80478C40)->view, 0, 0);
                fn_800E0168(
                    &((CameraPadState*)lbl_80478C40)->view,
                    &((CameraPadState*)lbl_80478C40)->view,
                    &((CameraPadState*)lbl_80478C40)->position);
                GSpartFree(object);
            }
        }
    }
}
