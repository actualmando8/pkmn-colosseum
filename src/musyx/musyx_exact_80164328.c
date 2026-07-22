#include "dolphin/os/OSInterrupt.h"
#include "dolphin/types.h"

extern u16 lbl_8047B084;
extern BOOL lbl_8047B080;

void hwEnableIrq(void)
{
    lbl_8047B084 = lbl_8047B084 - 1;
    if (lbl_8047B084 == 0) {
        OSRestoreInterrupts(lbl_8047B080);
    }
}
