/**
 * @file sdk_range_8009BD84.c
 * @brief dolphin-sdk code, 0x8009BD84 - 0x8009DF88 (21 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/db/DB.h"
#include "dolphin/gx/GX.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/vi/VI.h"

#define OS_FPUCONTEXT (*(OSContext* volatile*)0x800000D8)

void OSInitContext(OSContext* context, u32 pc, u32 newsp) {
    extern u8 _SDA_BASE_[];
    extern u8 _SDA2_BASE_[];

    context->srr0 = pc;
    context->gpr[1] = newsp;
    context->srr1 = 0x9032;
    context->cr = 0;
    context->xer = 0;
    context->gpr[2] = (u32)_SDA2_BASE_;
    context->gpr[13] = (u32)_SDA_BASE_;
    context->gpr[3] = 0;
    context->gpr[4] = 0;
    context->gpr[5] = 0;
    context->gpr[6] = 0;
    context->gpr[7] = 0;
    context->gpr[8] = 0;
    context->gpr[9] = 0;
    context->gpr[10] = 0;
    context->gpr[11] = 0;
    context->gpr[12] = 0;
    context->gpr[14] = 0;
    context->gpr[15] = 0;
    context->gpr[16] = 0;
    context->gpr[17] = 0;
    context->gpr[18] = 0;
    context->gpr[19] = 0;
    context->gpr[20] = 0;
    context->gpr[21] = 0;
    context->gpr[22] = 0;
    context->gpr[23] = 0;
    context->gpr[24] = 0;
    context->gpr[25] = 0;
    context->gpr[26] = 0;
    context->gpr[27] = 0;
    context->gpr[28] = 0;
    context->gpr[29] = 0;
    context->gpr[30] = 0;
    context->gpr[31] = 0;
    context->gqr[0] = 0;
    context->gqr[1] = 0;
    context->gqr[2] = 0;
    context->gqr[3] = 0;
    context->gqr[4] = 0;
    context->gqr[5] = 0;
    context->gqr[6] = 0;
    context->gqr[7] = 0;
    OSClearContext(context);
}

#pragma scheduling off
void OSSwitchFPUContext(u8 exception, OSContext* context) {
    OSContext** fpuContext = (OSContext**)0x800000D8;
    OSContext* previous = *fpuContext;

    context->srr1 |= 0x2000;
    *fpuContext = context;
    if ((s32)previous != (s32)context) {
        if (previous != 0) {
            __OSSaveFPUContext(exception, 0, previous);
        }
        __OSLoadFPUContext(exception, context);
    }
    context->state &= ~OS_CONTEXT_STATE_EXC;
}
#pragma scheduling reset

#pragma peephole off
void __OSContextInit(void) {
    __OSSetExceptionHandler(OS_EXCEPTION_FLOATING_POINT,
                            (__OSExceptionHandler)OSSwitchFPUContext);
    OS_FPUCONTEXT = NULL;
    DBPrintf("FPU-unavailable handler installed\n");
}
#pragma peephole reset

void OSReport(const char* format, ...) {
    typedef struct {
        u8 gpr;
        u8 fpr;
        u16 reserved;
        u32* overflow_arg_area;
        u32* reg_save_area;
    } VaList[1];
    extern s32 vprintf(const char* format, VaList args);
    VaList args;

    __builtin_va_info(&args);
    vprintf(format, args);
}

void ConfigureVideo(u16 fbWidth, u16 xfbHeight) {
    extern void VIConfigure(GXRenderModeObj* mode);
    extern void VIConfigurePan(u16 xOrigin, u16 yOrigin, u16 width, u16 height);
    GXRenderModeObj mode;

    mode.fbWidth = fbWidth;
    mode.efbHeight = 480;
    mode.xfbHeight = xfbHeight;
    mode.viXOrigin = 40;
    mode.viWidth = 640;
    mode.viHeight = xfbHeight;

    switch (VIGetTvFormat()) {
    case VI_NTSC:
    case VI_MPAL:
        if (*(volatile u16*)0xCC00206C & 1) {
            mode.viTVmode = 2;
            mode.viYOrigin = 0;
            mode.xfbMode = 0;
        } else {
            mode.viTVmode = 0;
            mode.viYOrigin = 0;
            mode.xfbMode = 1;
        }
        break;
    case VI_EUR60:
        mode.viTVmode = 20;
        mode.viYOrigin = 0;
        mode.xfbMode = 1;
        break;
    case VI_PAL:
        mode.viTVmode = 4;
        mode.viYOrigin = 47;
        mode.xfbMode = 1;
        break;
    }

    VIConfigure(&mode);
    VIConfigurePan(0, 0, 640, 480);
}


/* 0x8009D820 | 0x58 */
u16 fn_8009D820(void) {
    extern u16 lbl_804789A0;

    if (lbl_804789A0 <= 1) {
        return lbl_804789A0;
    }

    switch (*(s32*)0x800000CC) {
    case 0:
        lbl_804789A0 = (*(volatile u16*)0xCC00206E & 2) ? 1 : 0;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    default:
        lbl_804789A0 = 0;
        break;
    }

    return lbl_804789A0;
}

void fn_8009D878(void* dest, s32 size, s32 offset) {
    extern BOOL __OSReadROM(void* dest, s32 size, s32 offset);

    while (size > 0) {
        s32 chunkSize = size <= 0x100 ? size : 0x100;
        size -= chunkSize;
        while (!__OSReadROM(dest, chunkSize, offset)) {
        }
        offset += chunkSize;
        dest = (u8*)dest + chunkSize;
    }
}
