#ifndef DOLPHIN_OS_OSCACHE_H
#define DOLPHIN_OS_OSCACHE_H

#include "dolphin/types.h"

void DCEnable(void);
void DCInvalidateRange(void* addr, u32 nBytes);
void DCFlushRange(void* addr, u32 nBytes);
void DCFlushRangeNoSync(void* addr, u32 nBytes);

void ICInvalidateRange(void* addr, u32 nBytes);
void ICFlashInvalidate(void);
void ICEnable(void);

void L2GlobalInvalidate(void);
void LCDisable(void);

void DMAErrorHandler(u16 error, ...);
void __OSCacheInit(void);

#endif /* DOLPHIN_OS_OSCACHE_H */
