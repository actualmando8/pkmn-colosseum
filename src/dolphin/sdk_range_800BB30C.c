/**
 * @file sdk_range_800BB30C.c
 * @brief dolphin-sdk code, 0x800BB30C - 0x800BE464 (68 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef union GXStatus_800BB30C {
    /* 0x000 */ u32 word;
    struct {
        /* 0x000 */ u16 field_000;
        /* 0x002 */ u16 field_002;
    } half;
} GXStatus_800BB30C;

typedef struct GXData_800BB30C {
    /* 0x000 */ GXStatus_800BB30C status;
    /* 0x004 */ u8 pad_004[0x7C];
    /* 0x080 */ u32 mtxIdx0;
    /* 0x084 */ u32 mtxIdx1;
    /* 0x088 */ u8 pad_088[0x70];
    /* 0x0F8 */ u32 scissorTL;
    /* 0x0FC */ u32 scissorBR;
    /* 0x100 */ u8 pad_100[0x20];
    /* 0x120 */ u32 field_120;
    /* 0x124 */ u32 field_124;
    /* 0x128 */ u8 pad_128[0x8];
    /* 0x130 */ u32 tevColorEnv[16];
    /* 0x170 */ u32 tevAlphaEnv[16];
    /* 0x1B0 */ u8 pad_1B0[0x20];
    /* 0x1D0 */ u32 field_1D0;
    /* 0x1D4 */ u32 dstAlpha;
    /* 0x1D8 */ u32 zMode;
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
    /* 0x204 */ u32 genMode;
    /* 0x208 */ u8 pad_208[0x218];
    /* 0x420 */ u32 field_420;
    /* 0x424 */ f32 field_424;
    /* 0x428 */ f32 field_428;
    /* 0x42C */ f32 field_42C;
    /* 0x430 */ f32 field_430;
    /* 0x434 */ f32 field_434;
    /* 0x438 */ f32 field_438;
    /* 0x43C */ f32 projection[6];
    /* 0x454 */ u8 pad_454[0x88];
    /* 0x4DC */ u32 field_4DC;
    /* 0x4E0 */ u32 field_4E0;
    /* 0x4E4 */ u8 pad_4E4[0x10];
    /* 0x4F4 */ u32 dirtyState;
} GXData_800BB30C;

#define field_002 status.half.field_002

extern GXData_800BB30C* gx;
extern volatile u16* __cpReg;

extern void fn_800BB780(u32 dstCoord, u32 func, u32 srcParam, u32 mtx,
                        u32 normalize, u32 postMtx, u32 normalizeColor,
                        u32 bias, u32 arg8, u32 arg9);
extern void fn_800BD640(u32 value);
extern void __GXSetMatrixIndex(u32 value);
extern void fn_800BE164(u32* hi, u32* lo);
extern void fn_800B91EC(void);
extern void __GXSendFlushPrim(void);
extern u32 __cvt_fp2unsigned(f32 value);
extern s32 TRKReleaseBuffer(s32 bufferIndex);

typedef struct TRKEvent {
    /* 0x00 */ s32 type;
    /* 0x04 */ s32 unused;
    /* 0x08 */ s32 bufferIndex;
} TRKEvent;

#define GX_FIFO_U8  (*(volatile u8*)0xCC008000)
#define GX_FIFO_U16 (*(volatile u16*)0xCC008000)
#define GX_FIFO_U32 (*(volatile u32*)0xCC008000)
#define GX_FIFO_F32 (*(volatile f32*)0xCC008000)

#define GX_BP_REG(reg)       \
    do {                     \
        GX_FIFO_U8 = 0x61;   \
        GX_FIFO_U32 = (reg); \
    } while (0)

void fn_800BBC0C(u32 nChans) {
    GXData_800BB30C* p = gx;

    p->genMode = (p->genMode & ~0x70000U) | ((nChans & 0xFF) << 16);
    p->dirtyState |= 6;
}

void fn_800BBC34(u32 dstCoord) {
    fn_800BB780(dstCoord, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void fn_800BBC7C(u32 dstCoord, u32 func, u8 normalize, u8 color, u32 postMtx) {
    u32 colorSel = color != 0 ? 6 : 0;

    fn_800BB780(dstCoord, func, 0, normalize != 0 ? 7 : 0, postMtx, colorSel, colorSel, 0, 0, 0);
}

void fn_800BBF98(u32 dstCoord, u32 func, u32 normalize) {
    fn_800BB780(dstCoord, func, 0, 7, normalize, 0, 0, 0, 0, 0);
}

void fn_800BBFDC(u32 dstCoord) {
    fn_800BB780(dstCoord, 0, 0, 0, 0, 6, 6, 1, 0, 0);
}

#pragma peephole off
void __GXFlushTextureState(void) {
    GXData_800BB30C* p = gx;

    GX_BP_REG(p->field_124);
    p->field_002 = 0;
}
#pragma peephole on

void fn_800BC8C8(u32 nStages) {
    GXData_800BB30C* p = gx;

    p->genMode = (p->genMode & ~0x3C00U) | (((nStages & 0xFF) - 1) << 10);
    p->dirtyState |= 4;
}

void fn_800BCEBC(u32 value) {
    GXData_800BB30C* p = gx;

    p->field_1DC = (p->field_1DC & ~0x40U) | ((value & 0xFF) << 6);
    GX_BP_REG(p->field_1DC);
    p->field_002 = 0;
}

void fn_800BD0F8(void) {}

void GXCallDisplayList(void* list, u32 nbytes) {
    if (gx->dirtyState != 0) {
        fn_800B91EC();
    }

    if (gx->status.word == 0) {
        __GXSendFlushPrim();
    }

    GX_FIFO_U8 = 0x40;
    GX_FIFO_U32 = (u32)list;
    GX_FIFO_U32 = nbytes;
}

void fn_800BD394(f32* projection) {
    GXData_800BB30C* p;
    u32 type;

    type = __cvt_fp2unsigned(projection[0]);
    p = gx;
    p->field_420 = type;
    p->field_424 = projection[1];
    p->field_428 = projection[2];
    p->field_42C = projection[3];
    p->field_430 = projection[4];
    p->field_434 = projection[5];
    p->field_438 = projection[6];

    GX_FIFO_U8 = 0x10;
    GX_FIFO_U32 = 0x00061020;
    GX_FIFO_F32 = p->field_424;
    GX_FIFO_F32 = p->field_428;
    GX_FIFO_F32 = p->field_42C;
    GX_FIFO_F32 = p->field_430;
    GX_FIFO_F32 = p->field_434;
    GX_FIFO_F32 = p->field_438;
    GX_FIFO_U32 = p->field_420;
    p->field_002 = 1;
}

void fn_800BD454(f32* projection) {
    GXData_800BB30C* p = gx;

    projection[0] = p->field_420;
    projection[1] = p->field_424;
    projection[2] = p->field_428;
    projection[3] = p->field_42C;
    projection[4] = p->field_430;
    projection[5] = p->field_434;
    projection[6] = p->field_438;
}

void fn_800BD554(u32 index) {
    GXData_800BB30C* p = gx;

    p->mtxIdx0 = (p->mtxIdx0 & ~0x3FU) | index;
    __GXSetMatrixIndex(0);
}

void fn_800BD744(void) {
    fn_800BD640(1);
}

void fn_800BD768(f32* projection) {
    GXData_800BB30C* p = gx;

    projection[0] = p->projection[0];
    projection[1] = p->projection[1];
    projection[2] = p->projection[2];
    projection[3] = p->projection[3];
    projection[4] = p->projection[4];
    projection[5] = p->projection[5];
}

void fn_800BE30C(void) {
    __cpReg[2] = 4;
}

u32 fn_800BE31C(void) {
    u32 hi;
    u32 lo;

    fn_800BE164(&hi, &lo);
    return hi;
}

void fn_800BCE30(u32 zCompLoc) {
    GXData_800BB30C* p = gx;
    u32 value = p->field_1D0;

    value = (value & 0x0FFFFFFF) | ((zCompLoc << 3) & 0x10000000);
    GX_BP_REG(value);
    p->field_1D0 = value;
    p->field_002 = 0;
}

void fn_800BCE5C(u32 zCompLoc) {
    GXData_800BB30C* p = gx;
    u32 value = p->field_1D0;

    value = (value & 0xF7FFFFFF) | (((s32)zCompLoc << 4) & 0x08000000);
    GX_BP_REG(value);
    p->field_1D0 = value;
    p->field_002 = 0;
}

void GXSetZMode(u32 compareEnable, u32 func, u32 updateEnable) {
    GXData_800BB30C* p = gx;
    u32 value = p->zMode;

    value &= 0x87FFFFFFU;
    value |= compareEnable << 31;
    value |= func << 28;
    value |= updateEnable << 27;
    GX_BP_REG(value);
    p->zMode = value;
    p->field_002 = 0;
}

void fn_800BCFDC(u32 zCompLoc) {
    GXData_800BB30C* p = gx;
    u32 value = p->field_1D0;

    value = (value & ~0x20000000U) | ((zCompLoc << 29) & 0x20000000);
    GX_BP_REG(value);
    p->field_1D0 = value;
    p->field_002 = 0;
}

void GXSetDstAlpha(u32 enable, u32 alpha) {
    GXData_800BB30C* p = gx;
    u32 value = p->dstAlpha;

    value &= 0x00FFFFFFU;
    value |= (alpha << 24);

    value &= 0xFF7FFFFFU;
    value |= ((enable << 23) & 0x00800000U);

    GX_BP_REG(value);
    p->dstAlpha = value;
    p->field_002 = 0;
}

#pragma optimize_for_size off
void GXSetClipMode(u32 clipMode) {
    GX_FIFO_U8 = 0x10;
    GX_FIFO_U32 = 0x1005;
    GX_FIFO_U32 = clipMode;
    gx->field_002 = 1;
}

#pragma optimize_for_size reset

void fn_800BD044(u32 arg0, u32 arg1) {
    GXData_800BB30C* p = gx;
    u32 value = arg1;

    value &= 0xFFU;
    value |= (arg0 & 0xFFU) << 1U;
    value &= 0xFFU;
    GX_BP_REG(0x44000000U | value);
    p->field_002 = 0;
}

void fn_800BD830(u32 arg0, u32 arg1) {
    GXData_800BB30C* p = gx;
    u32 value = arg0 + 0x156U;
    u32 cmd = arg1 + 0x156U;

    value = (value >> 1U) & 0xFFC00FFFU;
    cmd = ((cmd << 9U) | (cmd >> 23U)) & 0x003FFFFFU;
    value |= cmd;
    value &= 0x00FFFFFFU;

    GX_BP_REG(0x59000000U | value);
    p->field_002 = 0;
}

void TRKDestructEvent(TRKEvent* event) {
    TRKReleaseBuffer(event->bufferIndex);
}
