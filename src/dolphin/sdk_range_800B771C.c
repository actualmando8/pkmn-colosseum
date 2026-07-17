/**
 * @file sdk_range_800B771C.c
 * @brief dolphin-sdk code, 0x800B771C - 0x800B856C (9 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct GXData_800B771C {
    /* 0x000 */ u8 _000[0x14];
    /* 0x014 */ u32 vcdLo;
    /* 0x018 */ u32 vcdHi;
    /* 0x01C */ u8 _01C[0x400];
    /* 0x41C */ u8 hasNrms;
    /* 0x41D */ u8 hasBiNrms;
    /* 0x41E */ u8 _41E[0xD6];
    /* 0x4F4 */ u32 dirtyState;
} GXData_800B771C;

extern volatile GXData_800B771C* const gx;
extern void fn_800B771C(void);

#define GX_FIFO_U8  (*(volatile u8*)0xCC008000)
#define GX_FIFO_U32 (*(volatile u32*)0xCC008000)

void fn_800B7BC4(void) {
    volatile u32* gx32;

    GX_FIFO_U8 = 0x8;
    gx32 = (u32*)gx;
    GX_FIFO_U8 = 0x50;
    GX_FIFO_U32 = gx32[0x5];
    GX_FIFO_U8 = 0x8;
    GX_FIFO_U8 = 0x60;
    GX_FIFO_U32 = gx32[0x6];
    fn_800B771C();
}

#pragma optimize_for_size on
#pragma peephole off
void fn_800B7D3C(void) {
    u32 vcdLo;

    gx->vcdLo = 0;
    vcdLo = *(volatile u32*)((u32)gx + 0x14);
    gx->vcdLo = (vcdLo & 0xFFFFF9FF) | 0x200;
    gx->vcdHi = 0;
    gx->hasNrms = 0;
    gx->hasBiNrms = 0;
    gx->dirtyState |= 8;
}
#pragma peephole reset
#pragma optimize_for_size reset

#pragma optimize_for_size on
#pragma opt_common_subs off
void fn_800B8444(void) {
    u8 i;
    u32 off;
    volatile u8* gx8 = (u8*)gx;

    off = 0;
    for (i = 0; i < 8; i++) {
        if (gx8[0x4F3] & (1 << i)) {
            GX_FIFO_U8 = 0x8;
            GX_FIFO_U8 = i | 0x70;
            GX_FIFO_U32 = *(volatile u32*)(gx8 + off + 0x1C);
            GX_FIFO_U8 = 0x8;
            GX_FIFO_U8 = i | 0x80;
            GX_FIFO_U32 = *(volatile u32*)(gx8 + off + 0x3C);
            GX_FIFO_U8 = 0x8;
            GX_FIFO_U8 = i | 0x90;
            GX_FIFO_U32 = *(volatile u32*)(gx8 + off + 0x5C);
        }
        off += 4;
    }
    ((volatile u8*)gx)[0x4F3] = 0;
}
#pragma opt_common_subs reset
#pragma optimize_for_size reset

void fn_800B84E0(s32 attr, u32 value, u8 value2) {
    s32 idx;
    s32 j;
    volatile u32* gx32 = (u32*)gx;

    if (attr == 0x19) {
        attr = 0xA;
    }
    idx = attr - 9;

    GX_FIFO_U8 = 0x8;
    GX_FIFO_U8 = idx | 0xA0;
    value &= 0x3FFFFFFF;
    GX_FIFO_U32 = value;
    j = idx - 0xC;
    if (j >= 0 && j < 4) {
        gx32[0x22 + j] = value;
    }

    GX_FIFO_U8 = 0x8;
    GX_FIFO_U8 = idx | 0xB0;
    GX_FIFO_U32 = value2;
    j = idx - 0xC;
    if (j >= 0 && j < 4) {
        gx32[0x26 + j] = value2;
    }
}
