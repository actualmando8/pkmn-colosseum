#include "dolphin/os/OSContext.h"

void OSSaveFPUContext(OSContext* context) {
    __OSSaveFPUContext(0, 0, context);
}
