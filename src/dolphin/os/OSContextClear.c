#include "dolphin/os/OSContext.h"

#define OS_FPUCONTEXT (*(OSContext* volatile*)0x800000D8)

#pragma peephole off
void OSClearContext(OSContext* context) {
    context->mode = 0;
    context->state = 0;

    if (context == OS_FPUCONTEXT) {
        OS_FPUCONTEXT = NULL;
    }
}
#pragma peephole reset
