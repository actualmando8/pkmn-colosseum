#include "dolphin/os/OSInterrupt.h"

__OSInterruptHandler* InterruptHandlerTable_8047A710;

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler) {
    __OSInterruptHandler old = InterruptHandlerTable_8047A710[interrupt];
    InterruptHandlerTable_8047A710[interrupt] = handler;
    return old;
}

__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt) {
    return InterruptHandlerTable_8047A710[interrupt];
}
