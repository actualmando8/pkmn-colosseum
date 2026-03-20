#ifndef DOLPHIN_SI_SI_H
#define DOLPHIN_SI_SI_H

#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"

typedef void (*SICallback)(s32 chan, u32 sr, OSContext* context);
typedef void (*SITypeAndStatusCallback)(s32 chan, u32 type);

void SIInit(void);
u32 SIGetType(s32 chan);
void SISetXY(u32 x, u32 y);
u32 SISetSamplingRate(u32 msec);
BOOL SITransfer(s32 chan, void* output, u32 outputBytes,
                void* input, u32 inputBytes,
                SICallback callback, s64 time);

#endif /* DOLPHIN_SI_SI_H */
