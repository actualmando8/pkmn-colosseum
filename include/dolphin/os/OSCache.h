#ifndef DOLPHIN_OS_OSCACHE_H
#define DOLPHIN_OS_OSCACHE_H

#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"

void DCEnable(void);
void DCInvalidateRange(void* addr, u32 nBytes);
void DCFlushRange(void* addr, u32 nBytes);
void DCFlushRangeNoSync(void* addr, u32 nBytes);

void ICInvalidateRange(void* addr, u32 nBytes);
void ICFlashInvalidate(void);
void ICEnable(void);

void L2GlobalInvalidate(void);
void LCDisable(void);

void DMAErrorHandler(u16 error, OSContext* context, ...);

/*
 * 2026-07-02 reconciliation: LCEnable here now refers to the real
 * wrapper (0x8009B4D8, formerly declared/defined as the orphan
 * "LCEnableNoInterrupts"). The big asm implementation it wraps is
 * __LCEnable (0x8009B40C, formerly this file's "LCEnable"); it has no
 * prototype here since nothing outside src/dolphin/os/OSCache.c calls
 * it directly. See src/dolphin/os/OSCache.c for details.
 */
void LCEnable(void);
u32 LCQueueLength(void);
void __OSCacheInit(void);

#endif /* DOLPHIN_OS_OSCACHE_H */
