#include "dolphin/types.h"

typedef void (*OSExceptionHandler)(u8 exception, void* context, u32 dsisr, u32 dar);

extern OSExceptionHandler* OSExceptionTable_8047A6C4;

#pragma peephole off
OSExceptionHandler __OSGetExceptionHandler(u8 exception) {
    return OSExceptionTable_8047A6C4[exception];
}
#pragma peephole reset
