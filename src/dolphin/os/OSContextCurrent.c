#include "dolphin/os/OSContext.h"

OSContext* OSGetCurrentContext(void) {
    return *(OSContext* volatile*)0x800000D4;
}
