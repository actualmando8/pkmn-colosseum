#include "dolphin/types.h"

/* SDA-relative flag: nonzero if port is open */
extern s32 lbl_8047A9E0; /* ddh_cc open flag */

/*
 * ddh_cc_open - Open the debug port.
 *
 * Checks if the port is already open. If so, returns an error.
 * Otherwise, sets the open flag and returns success.
 */
s32 ddh_cc_open(void) {
    if (lbl_8047A9E0 != 0) {
        return -10005; /* 0xD8EB */
    }

    lbl_8047A9E0 = 1;
    return 0;
}
