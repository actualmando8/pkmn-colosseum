#include "dolphin/os/OSContext.h"
#include "dolphin/os/PPCArch.h"

void OSSaveFPUContext(OSContext* context) {
    __OSSaveFPUContext(0, 0, context);
}

void OSSetCurrentContext(OSContext* context) {
    *(OSContext* volatile*)0x800000D4 = context;
    *(OSContext* volatile*)0x800000C0 = (OSContext*)((u32)context & 0x3FFFFFFF);

    if ((s32)*(OSContext* volatile*)0x800000D8 == (s32)context) {
        asm {
            lwz   r6, 0x19c(r3)
            ori   r6, r6, 0x2000
            stw   r6, 0x19c(r3)
            mfmsr r6
            ori   r6, r6, 2
            mtmsr r6
        }
    } else {
        asm {
            lwz     r6, 0x19c(r3)
            rlwinm  r6, r6, 0, 19, 17
            stw     r6, 0x19c(r3)
            mfmsr   r6
            rlwinm  r6, r6, 0, 19, 17
            ori     r6, r6, 2
            mtmsr   r6
        }
        __isync();
    }
}
