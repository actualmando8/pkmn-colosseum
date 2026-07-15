#include "dolphin/os/OSContext.h"
#include "dolphin/os/PPCArch.h"

void OSSaveFPUContext(OSContext* context) {
    __OSSaveFPUContext(0, 0, context);
}

#pragma peephole off
void OSSetCurrentContext(OSContext* context) {
    u32 msr;

    *(OSContext* volatile*)0x800000D4 = context;
    *(OSContext* volatile*)0x800000C0 = (OSContext*)((u32)context & 0x3FFFFFFF);

    if ((s32)*(OSContext* volatile*)0x800000D8 == (s32)context) {
        context->srr1 |= 0x2000;
        msr = PPCMfmsr();
        PPCMtmsr(msr | 2);
    } else {
        context->srr1 &= ~0x2000;
        msr = PPCMfmsr();
        PPCMtmsr((msr & ~0x2000) | 2);
        __isync();
    }
}
#pragma peephole reset
