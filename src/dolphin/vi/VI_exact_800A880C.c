/** Exact VI retrace-callback setters, 0x800A880C - 0x800A8894. */
#include "dolphin/vi/VI.h"

VIRetraceCallback fn_800A880C(VIRetraceCallback callback)
{
    extern VIRetraceCallback lbl_8047A85C;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    VIRetraceCallback previous = lbl_8047A85C;
    BOOL enabled = OSDisableInterrupts();

    lbl_8047A85C = callback;
    OSRestoreInterrupts(enabled);
    return previous;
}

VIRetraceCallback fn_800A8850(VIRetraceCallback callback)
{
    extern VIRetraceCallback lbl_8047A860;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    VIRetraceCallback previous = lbl_8047A860;
    BOOL enabled = OSDisableInterrupts();

    lbl_8047A860 = callback;
    OSRestoreInterrupts(enabled);
    return previous;
}
