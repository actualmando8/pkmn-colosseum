#ifndef DOLPHIN_OS_OSMUTEX_H
#define DOLPHIN_OS_OSMUTEX_H

#include "dolphin/types.h"
#include "dolphin/os/OSThread.h"

void OSInitMutex(OSMutex* mutex);
void OSLockMutex(OSMutex* mutex);
void OSUnlockMutex(OSMutex* mutex);
BOOL OSTryLockMutex(OSMutex* mutex);
void __OSUnlockAllMutex(OSThread* thread);

#endif /* DOLPHIN_OS_OSMUTEX_H */
