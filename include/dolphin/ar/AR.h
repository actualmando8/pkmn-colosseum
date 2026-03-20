#ifndef DOLPHIN_AR_AR_H
#define DOLPHIN_AR_AR_H

#include "dolphin/types.h"

typedef void (*ARCallback)(void);

u32  ARInit(u32* stack, u32 stackSize);
u32  ARGetSize(void);
u32  ARGetBaseAddress(void);
u32  ARAlloc(u32 length);
u32  ARFree(u32* length);
BOOL ARCheckInit(void);
void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length);
u32  ARGetDMAStatus(void);
void ARReset(void);
void ARSetSize(void);

#endif /* DOLPHIN_AR_AR_H */
