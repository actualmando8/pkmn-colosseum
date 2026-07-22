#include "game/camera_types.h"

u32 cameraMoveEndCheckSpecial(u8 wait)
{
    CameraPadState* state;

    for (;;) {
        state = lbl_80478C40;
        if (state->targetMoveActive == 0 &&
            state->positionMoveActive == 0 &&
            state->rotationMoveActive == 0) {
            return 0;
        }
        if (wait != 0) {
            _threadSwitch();
        } else {
            return 1;
        }
    }
}
