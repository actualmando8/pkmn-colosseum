#include "dolphin/types.h"

/* Canonical SDK error table already owned by game/data/data_80311AD4.c. */
extern u32 lbl_80311C00[18];

u8 ErrorCode2Num_800A7FE0(u32 errorCode) {
    u32 i;

    for (i = 0; i < 18; i++) {
        if (lbl_80311C00[i] == errorCode) {
            return (u8)i;
        }
    }

    if (errorCode >= 0x00100000 && errorCode <= 0x00100008) {
        return 17;
    }

    return 29;
}
