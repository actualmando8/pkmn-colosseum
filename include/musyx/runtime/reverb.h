#ifndef MUSYX_RUNTIME_REVERB_H
#define MUSYX_RUNTIME_REVERB_H

#include "dolphin/types.h"

typedef struct _SND_REVHI_DELAYLINE {
    s32 inPoint;
    s32 outPoint;
    s32 length;
    f32* inputs;
    f32 lastOutput;
} _SND_REVHI_DELAYLINE;

typedef struct _SND_REVHI_WORK {
    _SND_REVHI_DELAYLINE AP[9];
    _SND_REVHI_DELAYLINE C[9];
    f32 allPassCoeff;
    f32 combCoef[9];
    f32 lpLastout[3];
    f32 level;
    f32 damping;
    s32 preDelayTime;
    f32 crosstalk;
    f32* preDelayLine[3];
    f32* preDelayPtr[3];
} _SND_REVHI_WORK;

typedef struct SND_AUX_REVERBHI {
    _SND_REVHI_WORK rv;
    u8 tempDisableFX;
    f32 coloration;
    f32 mix;
    f32 time;
    f32 damping;
    f32 preDelay;
    f32 crosstalk;
} SND_AUX_REVERBHI;

void ReverbHIModify(_SND_REVHI_WORK* work, f32 coloration, f32 time,
                    f32 mix, f32 damping, f32 preDelay, f32 crosstalk);
u32 sndAuxCallbackUpdateSettingsReverbHI(SND_AUX_REVERBHI* reverb);

#endif /* MUSYX_RUNTIME_REVERB_H */
