/**
 * @file sdk_range_800B857C.c
 * @brief dolphin-sdk code, 0x800B857C - 0x800BA198 (47 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/PPCArch.h"

typedef void (*GXBreakPtCallback)(u16 token);
typedef void (*GXDrawDoneCallback)(void);

typedef struct GXData_800B857C {
    /* 0x000 */ u16 field_000;
    /* 0x002 */ u16 field_002;
    /* 0x004 */ u16 field_004;
    /* 0x006 */ u16 field_006;
    /* 0x008 */ u8 pad_008[0x74];
    /* 0x07C */ u32 field_07C;
    /* 0x080 */ u32 mtxIdx0;
    /* 0x084 */ u32 mtxIdx1;
    /* 0x088 */ u8 pad_088[0x30];
    /* 0x0B8 */ u32 field_0B8[8];
    /* 0x0D8 */ u8 pad_0D8[0xF8];
    /* 0x1D0 */ u32 field_1D0;
    /* 0x1D4 */ u32 field_1D4;
    /* 0x1D8 */ u32 field_1D8;
    /* 0x1DC */ u32 field_1DC;
    /* 0x1E0 */ u32 field_1E0;
    /* 0x1E4 */ u32 field_1E4;
    /* 0x1E8 */ u32 field_1E8;
    /* 0x1EC */ u32 field_1EC;
    /* 0x1F0 */ u32 field_1F0;
    /* 0x1F4 */ u32 field_1F4;
    /* 0x1F8 */ u32 field_1F8;
    /* 0x1FC */ u32 field_1FC;
    /* 0x200 */ u8 field_200;
    /* 0x201 */ u8 pad_201[3];
    /* 0x204 */ u32 field_204;
    /* 0x208 */ u8 pad_208[0x2E9];
    /* 0x4F1 */ u8 field_4F1;
    /* 0x4F2 */ u8 field_4F2;
    /* 0x4F3 */ u8 pad_4F3;
    /* 0x4F4 */ u32 dirtyState;
} GXData_800B857C;

extern GXData_800B857C* const gx;
extern volatile u16* __peReg;
extern volatile u16* __memReg;
extern GXBreakPtCallback lbl_8047A9C0;
extern GXDrawDoneCallback lbl_8047A9C4;
extern volatile u8 lbl_8047A9C8;
extern OSThreadQueue lbl_8047A9CC;

extern u32 fn_800B7714(void);
extern void __GXCleanGPFifo(void);
extern void __GXSetSUTexRegs(void);
extern void fn_800BC024(void);
extern void fn_800B9578(void);
extern void fn_800B7BC4(void);
extern void fn_800B8444(void);
extern void __GXCalculateVLim(void);
extern u32 __cvt_fp2unsigned(f32 value);

#define GX_FIFO_U8  (*(volatile u8*)0xCC008000)
#define GX_FIFO_U16 (*(volatile u16*)0xCC008000)
#define GX_FIFO_U32 (*(volatile u32*)0xCC008000)

#define GX_BP_REG(reg)      \
    do {                    \
        GX_FIFO_U8 = 0x61;  \
        GX_FIFO_U32 = (reg); \
    } while (0)

void fn_800B884C(u8 count) {
    u32 n = count;
    GXData_800B857C* p = gx;

    p->field_204 = (p->field_204 & ~0xFU) | n;
    GX_FIFO_U8 = 0x10;
    GX_FIFO_U32 = 0x103F;
    GX_FIFO_U32 = n;
    p->dirtyState |= 4;
}

void GXSetMisc(s32 token, u32 value) {
    switch (token) {
    case 0:
        break;
    case 1:
        gx->field_004 = value;
        gx->field_000 = (u16)((u32)__cntlzw(gx->field_004) >> 5);
        gx->field_002 = 1;
        if (gx->field_004 != 0) {
            gx->dirtyState |= 8;
        }
        break;
    case 2:
        gx->field_4F1 = (value != 0);
        break;
    case 3:
        gx->field_4F2 = (value != 0);
        break;
    }
}

void fn_800B91EC(void);
void __GXSendFlushPrim(void);

void GXFlush(void) {
    if (gx->dirtyState != 0) {
        fn_800B91EC();
    }

    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    PPCSync();
}

void fn_800B8C58(u16 token) {
    BOOL enabled;
    u32 reg;

    enabled = OSDisableInterrupts();
    reg = token | 0x48000000;
    GX_BP_REG(reg);
    reg = (reg & ~0xFFFFU) | token;
    reg = (reg & 0xFFFFFFU) | 0x47000000;
    GX_BP_REG(reg);
    GXFlush();
    OSRestoreInterrupts(enabled);
    gx->field_002 = 0;
}

void GXSetDrawDone(void) {
    BOOL enabled;
    u8 cmd = 0x61;
    GXData_800B857C* p;

    enabled = OSDisableInterrupts();
    p = gx;
    GX_FIFO_U8 = cmd;
    GX_FIFO_U32 = 0x45000002;
    if (p->dirtyState != 0) {
        fn_800B91EC();
    }

    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    GX_FIFO_U32 = 0;
    PPCSync();
    lbl_8047A9C8 = 0;
    OSRestoreInterrupts(enabled);
}

void fn_800B8DA8(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    while (lbl_8047A9C8 == 0) {
        OSSleepThread(&lbl_8047A9CC);
    }
    OSRestoreInterrupts(enabled);
}

#pragma dont_inline on
void GXDrawDone(void) {
    BOOL enabled;
    u32 scratch[2];

    enabled = OSDisableInterrupts();
    GX_FIFO_U8 = 0x61;
    GX_FIFO_U32 = 0x45000002;
    GXFlush();
    lbl_8047A9C8 = 0;
    OSRestoreInterrupts(enabled);

    enabled = OSDisableInterrupts();
    while (lbl_8047A9C8 == 0) {
        OSSleepThread(&lbl_8047A9CC);
    }
    OSRestoreInterrupts(enabled);
}
#pragma dont_inline reset

void fn_800B8E74(void) {
    GXData_800B857C* p = gx;

    GX_FIFO_U8 = 0x61;
    GX_FIFO_U32 = p->field_1DC;
    p->field_002 = 0;
}

void fn_800B8E98(u32 token, u32 value) {
    __peReg[3] = (u16)((token << 8) | (value & 0xFF));
}

void fn_800B8EAC(u32 value) {
    __peReg[4] = (u16)((value & ~4U) | 4);
}

void fn_800B8EC0(u32 enable) {
    volatile u16* reg = &__peReg[1];
    *reg = (u16)((*reg & ~0x10U) | ((enable & 0xFF) << 4));
}

void fn_800B8EDC(s32 type, u32 srcFactor, u32 dstFactor, u32 op) {
    volatile u16* regp = &__peReg[1];
    u32 reg = *regp;
    u32 enable = (type == 1 || type == 3);
    u32 subtract = (type == 3);
    u32 logic = (type == 2);

    reg = (reg & ~1U) | enable;
    reg = (reg & ~0x800U) | (subtract << 11);
    reg = (reg & ~2U) | (logic << 1);
    reg = (reg & ~0xF000U) | (op << 12);
    reg = (reg & ~0x700U) | (srcFactor << 8);
    reg = (reg & ~0xE0U) | (dstFactor << 5);
    reg = (reg & 0xFFFFFFU) | 0x41000000;
    *regp = (u16)reg;
}

void fn_800B8F64(u32 enable) {
    volatile u16* reg = &__peReg[1];
    *reg = (u16)((*reg & ~0x8U) | ((enable & 0xFF) << 3));
}

void fn_800B8F80(u8 func, u32 threshold) {
    u32 reg;

    reg = (threshold & 0xFF) | (func << 8);
    __peReg[2] = reg;
}

void fn_800B8F94(u32 enable) {
    volatile u16* reg = &__peReg[1];
    *reg = (u16)((*reg & ~0x4U) | ((enable & 0xFF) << 2));
}

void fn_800B8FB0(u32 arg0, u32 arg1, u32 arg2) {
    u32 reg = arg0 & 0xFF;

    reg = (reg & ~0xEU) | (arg1 << 1);
    reg = (reg & ~0x10U) | ((arg2 & 0xFF) << 4);
    __peReg[0] = (u16)reg;
}

GXBreakPtCallback fn_800B8FD8(GXBreakPtCallback callback) {
    GXBreakPtCallback old = lbl_8047A9C0;
    BOOL enabled = OSDisableInterrupts();

    lbl_8047A9C0 = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

void fn_800B901C(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;
    u16 token = __peReg[7];

    if (lbl_8047A9C0 != NULL) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        lbl_8047A9C0(token);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }

    __peReg[5] = (__peReg[5] & ~4U) | 4;
}

GXDrawDoneCallback fn_800B90A4(GXDrawDoneCallback callback) {
    GXDrawDoneCallback old = lbl_8047A9C4;
    BOOL enabled = OSDisableInterrupts();

    lbl_8047A9C4 = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

void fn_800B90E8(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;

    __peReg[5] = (__peReg[5] & ~8U) | 8;
    lbl_8047A9C8 = 1;

    if (lbl_8047A9C4 != NULL) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        lbl_8047A9C4();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }

    OSWakeupThread(&lbl_8047A9CC);
}

void __GXPEInit(void) {
    volatile u16* regp;
    u32 reg;

    __OSSetInterruptHandler(0x12, fn_800B901C);
    __OSSetInterruptHandler(0x13, fn_800B90E8);
    OSInitThreadQueue(&lbl_8047A9CC);
    __OSUnmaskInterrupts(0x2000);
    __OSUnmaskInterrupts(0x1000);

    regp = &__peReg[5];
    reg = *regp;
    reg = (reg & ~4U) | 4;
    reg = (reg & ~8U) | 8;
    reg = (reg & ~1U) | 1;
    reg = (reg & ~2U) | 2;
    *regp = (u16)reg;
}

void fn_800B91EC(void) {
    if (gx->dirtyState & 1) {
        __GXSetSUTexRegs();
    }
    if (gx->dirtyState & 2) {
        fn_800BC024();
    }
    if (gx->dirtyState & 4) {
        fn_800B9578();
    }
    if (gx->dirtyState & 8) {
        fn_800B7BC4();
    }
    if (gx->dirtyState & 0x10) {
        fn_800B8444();
    }
    if (gx->dirtyState & 0x18) {
        __GXCalculateVLim();
    }
    gx->dirtyState = 0;
}

void fn_800B928C(u32 primitive, u32 vertexFormat, u16 vertexCount) {
    if (gx->dirtyState != 0) {
        fn_800B91EC();
    }

    if (*(u32*)&gx->field_000 == 0) {
        __GXSendFlushPrim();
    }

    GX_FIFO_U8 = vertexFormat | primitive;
    GX_FIFO_U16 = vertexCount;
}

void __GXSendFlushPrim(void) {
    GXData_800B857C* p = gx;
    u16 nverts = p->field_004;
    u16 stride = p->field_006;
    u32 bytes = nverts * stride;
    u32 i;

    GX_FIFO_U8 = 0x98;
    GX_FIFO_U16 = nverts;

    for (i = 0; i < bytes; i += 4) {
        GX_FIFO_U32 = 0;
    }

    gx->field_002 = 1;
}

void fn_800B9404(u32 left, u32 top) {
    GXData_800B857C* p = gx;

    p->field_07C = (p->field_07C & ~0xFFU) | (left & 0xFF);
    p->field_07C = (p->field_07C & ~0x70000U) | (top << 16);
    GX_BP_REG(p->field_07C);
    p->field_002 = 0;
}

void fn_800B944C(u32 right, u32 bottom) {
    GXData_800B857C* p = gx;

    p->field_07C = (p->field_07C & ~0xFF00U) | ((right & 0xFF) << 8);
    p->field_07C = (p->field_07C & ~0x380000U) | (bottom << 19);
    GX_BP_REG(p->field_07C);
    p->field_002 = 0;
}

void fn_800B9494(u32 chan, u32 enable0, u32 enable1) {
    gx->field_0B8[chan] = (gx->field_0B8[chan] & ~0x40000U) | ((enable0 & 0xFF) << 18);
    gx->field_0B8[chan] = (gx->field_0B8[chan] & ~0x80000U) | ((enable1 & 0xFF) << 19);
    GX_BP_REG(gx->field_0B8[chan]);
    gx->field_002 = 0;
}

void fn_800B94F0(s32 value) {
    switch (value) {
    case 1:
        value = 2;
        break;
    case 2:
        value = 1;
        break;
    }

    gx->field_204 = (gx->field_204 & ~0xC000U) | (value << 14);
    gx->dirtyState |= 4;
}

void fn_800B953C(u32 value) {
    GXData_800B857C* p = gx;

    p->field_204 = (p->field_204 & ~0x80000U) | ((value & 0xFF) << 19);
    GX_BP_REG(0xFE080000);
    GX_BP_REG(p->field_204);
}

void fn_800B9578(void) {
    GXData_800B857C* p = gx;

    GX_FIFO_U8 = 0x61;
    GX_FIFO_U32 = p->field_204;
    p->field_002 = 0;
}

void fn_800B959C(u16 left, u16 top, u16 width, u16 height) {
    gx->field_1E0 = 0;
    gx->field_1E0 = (gx->field_1E0 & ~0x3FFU) | left;
    gx->field_1E0 = (gx->field_1E0 & ~0xFFC00U) | (top << 10);
    gx->field_1E0 = (gx->field_1E0 & 0xFFFFFFU) | 0x49000000;

    gx->field_1E4 = 0;
    gx->field_1E4 = (gx->field_1E4 & ~0x3FFU) | (width - 1);
    gx->field_1E4 = (gx->field_1E4 & ~0xFFC00U) | ((height - 1) << 10);
    gx->field_1E4 = (gx->field_1E4 & 0xFFFFFFU) | 0x4A000000;
}

void fn_800B962C(u16 left, u16 top, u16 width, u16 height) {
    gx->field_1F0 = 0;
    gx->field_1F0 = (gx->field_1F0 & ~0x3FFU) | left;
    gx->field_1F0 = (gx->field_1F0 & ~0xFFC00U) | (top << 10);
    gx->field_1F0 = (gx->field_1F0 & 0xFFFFFFU) | 0x49000000;

    gx->field_1F4 = 0;
    gx->field_1F4 = (gx->field_1F4 & ~0x3FFU) | (width - 1);
    gx->field_1F4 = (gx->field_1F4 & ~0xFFC00U) | ((height - 1) << 10);
    gx->field_1F4 = (gx->field_1F4 & 0xFFFFFFU) | 0x4A000000;
}

void fn_800B96BC(u32 value) {
    GXData_800B857C* p;
    u32* regp;

    p = *(GXData_800B857C**)&gx;
    p->field_1E8 = 0;
    regp = &p->field_1E8;
    *regp = (*regp & ~0x3FFU) |
            (((s32)((value & 0x7FFFU) << 1) << 16) >> 21);
    *regp = (*regp & 0xFFFFFFU) | 0x4D000000U;
}

void fn_800B984C(u32 value) {
    gx->field_1EC = (gx->field_1EC & ~0x3000U) | (value << 12);
    gx->field_1FC = gx->field_1FC & ~0x3000U;
}

void fn_800B9874(u32 value) {
    GXData_800B857C* p = gx;
    u32 bit0;
    u32 bit1;
    u8 out0;
    u8 out1;

    bit0 = value & 1;
    bit1 = value & 2;
    out0 = (bit0 == 1);
    out1 = (bit1 == 2);
    p->field_1EC = (p->field_1EC & ~1U) | out0;
    p->field_1EC = (p->field_1EC & ~2U) | (out1 << 1);
    p->field_1FC = (p->field_1FC & ~1U) | out0;
    p->field_1FC = (p->field_1FC & ~2U) | (out1 << 1);
}

u32 fn_800B9B14(f32 scale) {
    u32 yScale;
    u32 height;
    u32 result;
    u32 divisor;
    u8 nonUnityScale;
    GXData_800B857C* p;

    yScale = __cvt_fp2unsigned(256.0F / scale) & 0x1FF;
    p = gx;
    GX_BP_REG(0x4E000000 | yScale);
    p->field_002 = 0;
    nonUnityScale = (yScale != 0x100);
    p->field_1EC = (p->field_1EC & ~0x400U) | (nonUnityScale << 10);

    height = ((p->field_1E4 >> 10) & 0x3FF) + 1;
    result = (((height - 1) << 8) / yScale) + 1;
    if (yScale > 0x80 && yScale < 0x100) {
        divisor = yScale;
        while ((divisor & 1) == 0) {
            divisor >>= 1;
        }
        if ((height % divisor) == 0) {
            result++;
        }
    }
    if (result > 0x400) {
        result = 0x400;
    }
    return result;
}

typedef struct GXColor_800B9BDC {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} GXColor_800B9BDC;

void fn_800B9BDC(GXColor_800B9BDC color, u32 clearZ) {
    u32 reg;
    GXData_800B857C* p = gx;

    reg = 0;
    reg = (reg & ~0xFFU) | color.r;
    reg = (reg & ~0xFF00U) | (color.a << 8);
    reg = (reg & 0xFFFFFFU) | 0x4F000000U;
    GX_BP_REG(reg);
    reg = 0;
    reg = (reg & ~0xFFU) | color.b;
    reg = (reg & ~0xFF00U) | (color.g << 8);
    reg = (reg & 0xFFFFFFU) | 0x50000000U;
    GX_BP_REG(reg);
    reg = 0;
    reg = (reg & ~0xFFFFFFU) | (clearZ & 0xFFFFFFU);
    reg = (reg & 0xFFFFFFU) | 0x51000000U;
    GX_BP_REG(reg);
    p->field_002 = 0;
}

void fn_800B9E6C(u32 value) {
    GXData_800B857C* p = gx;

    p->field_1EC = (p->field_1EC & ~0x180U) | (value << 7);
}

void GXClearBoundingBox(void) {
    GXData_800B857C* p = gx;

    GX_FIFO_U8 = 0x61;
    GX_FIFO_U32 = 0x550003FF;
    GX_FIFO_U8 = 0x61;
    GX_FIFO_U32 = 0x560003FF;
    p->field_002 = 0;
}
