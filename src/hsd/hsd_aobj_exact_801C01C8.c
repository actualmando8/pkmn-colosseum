/**
 * @file hsd_aobj_exact_801C01C8.c
 * @brief Strict interrupt-safe HSD animation callback setters,
 *        0x801C01C8 - 0x801C0270.
 */

#include "dolphin/types.h"
#include "dolphin/os/OSInterrupt.h"

extern void* volatile lbl_80466BC0[];

void* fn_801C01C8(void* callback)
{
    void* previous = lbl_80466BC0[0x7A];
    BOOL enabled = OSDisableInterrupts();

    lbl_80466BC0[0x7A] = callback;
    OSRestoreInterrupts(enabled);
    return previous;
}

void* fn_801C021C(void* callback)
{
    void* previous = lbl_80466BC0[0x77];
    BOOL enabled = OSDisableInterrupts();

    lbl_80466BC0[0x77] = callback;
    OSRestoreInterrupts(enabled);
    return previous;
}
