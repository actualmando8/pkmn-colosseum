#include "dolphin/os/OSThread.h"

void OSInitThreadQueue(OSThreadQueue* queue) {
    queue->tail = NULL;
    queue->head = NULL;
}

OSThread* fn_800A13F8(void) {
    return *(OSThread* volatile*)0x800000E4;
}
