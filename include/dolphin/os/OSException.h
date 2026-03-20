#ifndef DOLPHIN_OS_OSEXCEPTION_H
#define DOLPHIN_OS_OSEXCEPTION_H

#include "dolphin/types.h"
#include "dolphin/os/OS.h"

void OSExceptionInit(void);
__OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler);
__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);
void OSDefaultExceptionHandler(__OSException exception, OSContext* context, u32 dsisr, u32 dar);

#endif /* DOLPHIN_OS_OSEXCEPTION_H */
