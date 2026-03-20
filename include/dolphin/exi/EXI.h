#ifndef DOLPHIN_EXI_EXI_H
#define DOLPHIN_EXI_EXI_H

#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"

typedef void (*EXICallback)(s32 chan, OSContext* context);

void EXIInit(void);
BOOL EXIImm(s32 chan, void* buf, s32 len, u32 type, EXICallback callback);
BOOL EXIDma(s32 chan, void* buf, s32 len, u32 type, EXICallback callback);
BOOL EXISync(s32 chan);
BOOL EXISelect(s32 chan, u32 dev, u32 freq);
BOOL EXIDeselect(s32 chan);
BOOL EXILock(s32 chan, u32 dev, EXICallback unlockedCallback);
BOOL EXIUnlock(s32 chan);

#endif /* DOLPHIN_EXI_EXI_H */
