#include "game/camera_types.h"

extern void* lbl_80478C40;

u32 GSscene_SetMode(u32 mode)
{
    CameraPadState* state = (CameraPadState*)lbl_80478C40;
    u32 previous;

    if (state->mode == (u8)mode) {
        return mode;
    }
    previous = state->mode;
    state->mode = (u8)mode;
    return previous;
}
