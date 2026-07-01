#include "dolphin/types.h"

extern u8 gTRKState[]; /* large state structure */

/* fn_800C1310 - 0x800C1310 | size: 0x18 */
s32 fn_800C1310(void) {
    *(s32*)&gTRKState[0x98] = 1;
    return 0;
}
