#include "musyx/runtime/reverb.h"

u32 sndAuxCallbackUpdateSettingsReverbHI(SND_AUX_REVERBHI* reverb)
{
    reverb->tempDisableFX = 1;
    ReverbHIModify(&reverb->rv, reverb->coloration, reverb->time,
                   reverb->mix, reverb->damping, reverb->preDelay,
                   reverb->crosstalk);
    reverb->tempDisableFX = 0;
    return 1;
}
