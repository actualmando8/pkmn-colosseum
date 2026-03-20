#ifndef DOLPHIN_DB_H
#define DOLPHIN_DB_H

#include "dolphin/types.h"

/*
 * Dolphin SDK debug interface.
 * Provides exception-level debugging support used by TRK.
 */

extern s32 DBVerbose;

void DBInit(void);
void __DBExceptionDestination(void);
s32  __DBIsExceptionMarked(u8 exceptionType);
void DBPrintf(const char* fmt, ...);

/* Internal globals */
extern void* __DBInterface;

#endif /* DOLPHIN_DB_H */
