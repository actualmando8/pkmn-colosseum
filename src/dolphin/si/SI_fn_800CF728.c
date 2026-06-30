#include "dolphin/types.h"

extern s32 Si_80313F8C[];
extern s32 Packet_803FFFB0[];

BOOL fn_800CF728(s32 channel) {
    BOOL available = TRUE;

    if (Packet_803FFFB0[channel * 8] == -1 && Si_80313F8C[0] != channel) {
        available = FALSE;
    }
    return available;
}
