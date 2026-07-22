/** Exact VI retrace waiter, 0x800A8FE4 - 0x800A9038. */
#include "dolphin/types.h"
#include "dolphin/os/OSThread.h"

void VIWaitForRetrace(void)
{
    extern u32 lbl_8047A84C;
    extern OSThreadQueue lbl_8047A854;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    BOOL enabled;
    u32 count;

    enabled = OSDisableInterrupts();
    count = lbl_8047A84C;
    do {
        OSSleepThread(&lbl_8047A854);
    } while (count == lbl_8047A84C);
    OSRestoreInterrupts(enabled);
}
