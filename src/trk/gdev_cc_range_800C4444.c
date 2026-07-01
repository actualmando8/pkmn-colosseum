#include "dolphin/types.h"

/* SDA-relative flag: nonzero if port is open */
extern s32 lbl_8047A9E8; /* gdev_cc open flag */

/* gdev_cc_open - Open the debug port. */
s32 gdev_cc_open(void) {
    if (lbl_8047A9E8 != 0) {
        return -10005; /* 0xD8EB */
    }

    lbl_8047A9E8 = 1;
    return 0;
}
