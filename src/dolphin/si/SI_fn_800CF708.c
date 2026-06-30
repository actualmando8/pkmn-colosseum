#include "dolphin/types.h"

extern s32 Si_80313F8C[];

BOOL fn_800CF708(void) {
    if (Si_80313F8C[0] != -1) {
        return TRUE;
    }
    return FALSE;
}
