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

extern u32* const gx;
extern void fn_800B771C(void);

typedef union GXFifo_800B7BC4 {
    u8 u8;
    u32 u32;
} GXFifo_800B7BC4;

volatile GXFifo_800B7BC4 GXWGFifo_800B7BC4 : 0xCC008000;

void fn_800B7BC4(void) {
    GXWGFifo_800B7BC4.u8 = 8;
    GXWGFifo_800B7BC4.u8 = 0x50;
    GXWGFifo_800B7BC4.u32 = gx[5];
    GXWGFifo_800B7BC4.u8 = 8;
    GXWGFifo_800B7BC4.u8 = 0x60;
    GXWGFifo_800B7BC4.u32 = gx[6];
    fn_800B771C();
}

#pragma optimize_for_size on
#pragma peephole off
void fn_800B7D3C(void) {
    volatile u32* gx32;
    volatile u8* gx8;

    gx32 = (u32*)gx;
    gx8 = (u8*)gx;

    gx32[0x5] = 0;
    gx32[0x5] = (gx32[0x5] & 0xFF9FFFFF) | 0x200;
    gx32[0x6] = 0;
    gx8[0x41C] = 0;
    gx8[0x41D] = 0;
    gx32[0x4F4 >> 2] |= 8;
}
#pragma peephole reset
#pragma optimize_for_size reset

typedef struct GXData_800B8444 {
    u8 _pad_000[0x1C];
    u32 vatA[8];
    u32 vatB[8];
    u32 vatC[8];
    u8 _pad_07C[0x477];
    u8 dirtyVAT;
} GXData_800B8444;

void fn_800B8444(void) {
    u8 i;

    for (i = 0; i < 8; i++) {
        if (((GXData_800B8444*)gx)->dirtyVAT & (1 << (u8)i)) {
            GXWGFifo_800B7BC4.u8 = 8;
            GXWGFifo_800B7BC4.u8 = i | 0x70;
            GXWGFifo_800B7BC4.u32 = ((GXData_800B8444*)gx)->vatA[i];
            GXWGFifo_800B7BC4.u8 = 8;
            GXWGFifo_800B7BC4.u8 = i | 0x80;
            GXWGFifo_800B7BC4.u32 = ((GXData_800B8444*)gx)->vatB[i];
            GXWGFifo_800B7BC4.u8 = 8;
            GXWGFifo_800B7BC4.u8 = i | 0x90;
            GXWGFifo_800B7BC4.u32 = ((GXData_800B8444*)gx)->vatC[i];
        }
    }
    ((GXData_800B8444*)gx)->dirtyVAT = 0;
}

typedef struct GXData_800B84E0 {
    u8 _pad_000[0x88];
    u32 arrayBase[4];
    u32 arrayStride[4];
} GXData_800B84E0;

void fn_800B84E0(s32 attr, void* base, u8 stride) {
    s32 index;

    if (attr == 25) {
        attr = 10;
    }

    index = attr - 9;
    GXWGFifo_800B7BC4.u8 = 8;
    GXWGFifo_800B7BC4.u8 = index | 0xA0;
    GXWGFifo_800B7BC4.u32 = (u32)base & 0x3FFFFFFF;

    if (index - 12 >= 0 && index - 12 < 4) {
        ((GXData_800B84E0*)gx)->arrayBase[index - 12] =
            (u32)base & 0x3FFFFFFF;
    }

    GXWGFifo_800B7BC4.u8 = 8;
    GXWGFifo_800B7BC4.u8 = index | 0xB0;
    GXWGFifo_800B7BC4.u32 = stride;

    if (index - 12 >= 0 && index - 12 < 4) {
        ((GXData_800B84E0*)gx)->arrayStride[index - 12] = stride;
    }
}
