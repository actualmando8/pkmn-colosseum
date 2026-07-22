#include "musyx/runtime/hw_dspctrl.h"

u32 hwGetVirtualSampleID(u32 index)
{
    DSPvoice* voice = &lbl_8047B024[index];

    if (voice->state == 0) {
        return -1;
    }
    return voice->virtualSampleID;
}
