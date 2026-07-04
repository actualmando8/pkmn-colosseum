#include "dolphin/types.h"

#pragma section ".data"

extern u8 lbl_8036BFC0[];
extern void* jumptable_8036BFE0[];
extern void* jumptable_8036C1E0[];
extern void* jumptable_8036C204[];
extern void* jumptable_8036C224[];
extern u8 lbl_8036C248[];
extern void* jumptable_8036C254[];
extern void* jumptable_8036C278[];

extern u8 fn_80177A64[];
extern u8 generateParticle_8017424C[];
extern u8 psCreateGeneratorID[];
extern u8 psInterpretParticle0[];

/* Auto-carved .data unit 0x8036BFC0..0x8036C29C (8 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 lbl_8036BFC0[32] = {
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00,
};

void* jumptable_8036BFE0[128] = {
    (void*)((u8*)psInterpretParticle0 + 0x3D4),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x45C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x4FC),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x584),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x68C),
    (void*)((u8*)psInterpretParticle0 + 0x6BC),
    (void*)((u8*)psInterpretParticle0 + 0x6CC),
    (void*)((u8*)psInterpretParticle0 + 0x74C),
    (void*)((u8*)psInterpretParticle0 + 0x7CC),
    (void*)((u8*)psInterpretParticle0 + 0x99C),
    (void*)((u8*)psInterpretParticle0 + 0x10F8),
    (void*)((u8*)psInterpretParticle0 + 0x1158),
    (void*)((u8*)psInterpretParticle0 + 0x118C),
    (void*)((u8*)psInterpretParticle0 + 0x1244),
    (void*)((u8*)psInterpretParticle0 + 0x1344),
    (void*)((u8*)psInterpretParticle0 + 0x1488),
    (void*)((u8*)psInterpretParticle0 + 0x14CC),
    (void*)((u8*)psInterpretParticle0 + 0x1518),
    (void*)((u8*)psInterpretParticle0 + 0x1528),
    (void*)((u8*)psInterpretParticle0 + 0x1538),
    (void*)((u8*)psInterpretParticle0 + 0x1554),
    (void*)((u8*)psInterpretParticle0 + 0x1570),
    (void*)((u8*)psInterpretParticle0 + 0x1580),
    (void*)((u8*)psInterpretParticle0 + 0x1644),
    (void*)((u8*)psInterpretParticle0 + 0x16F8),
    (void*)((u8*)psInterpretParticle0 + 0x1708),
    (void*)((u8*)psInterpretParticle0 + 0x1718),
    (void*)((u8*)psInterpretParticle0 + 0x1758),
    (void*)((u8*)psInterpretParticle0 + 0x1784),
    (void*)((u8*)psInterpretParticle0 + 0x182C),
    (void*)((u8*)psInterpretParticle0 + 0x1A34),
    (void*)((u8*)psInterpretParticle0 + 0x1BF0),
    (void*)((u8*)psInterpretParticle0 + 0x1DAC),
    (void*)((u8*)psInterpretParticle0 + 0x1E68),
    (void*)((u8*)psInterpretParticle0 + 0x1F54),
    (void*)((u8*)psInterpretParticle0 + 0x1FA8),
    (void*)((u8*)psInterpretParticle0 + 0x1FD0),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x20F8),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2220),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2904),
    (void*)((u8*)psInterpretParticle0 + 0x2914),
    (void*)((u8*)psInterpretParticle0 + 0x2924),
    (void*)((u8*)psInterpretParticle0 + 0x29B8),
    (void*)((u8*)psInterpretParticle0 + 0x2A4C),
    (void*)((u8*)psInterpretParticle0 + 0x2A5C),
    (void*)((u8*)psInterpretParticle0 + 0x2A6C),
    (void*)((u8*)psInterpretParticle0 + 0x24D0),
    (void*)((u8*)psInterpretParticle0 + 0x2AB4),
    (void*)((u8*)psInterpretParticle0 + 0x2B78),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2C3C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0xBF0),
    (void*)((u8*)psInterpretParticle0 + 0xE64),
    (void*)((u8*)psInterpretParticle0 + 0x8A4),
    (void*)((u8*)psInterpretParticle0 + 0x1920),
    (void*)((u8*)psInterpretParticle0 + 0x2D0C),
    (void*)((u8*)psInterpretParticle0 + 0x1264),
    (void*)((u8*)psInterpretParticle0 + 0x12B4),
    (void*)((u8*)psInterpretParticle0 + 0x12FC),
    (void*)((u8*)psInterpretParticle0 + 0x1334),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2E0C),
    (void*)((u8*)psInterpretParticle0 + 0x2DA0),
    (void*)((u8*)psInterpretParticle0 + 0x2DBC),
    (void*)((u8*)psInterpretParticle0 + 0x2DE0),
    (void*)((u8*)psInterpretParticle0 + 0x2DF0),
    (void*)((u8*)psInterpretParticle0 + 0x2E00),
    (void*)((u8*)psInterpretParticle0 + 0x2E00),
};

void* jumptable_8036C1E0[9] = {
    (void*)((u8*)psCreateGeneratorID + 0x360),
    (void*)((u8*)psCreateGeneratorID + 0x3AC),
    (void*)((u8*)psCreateGeneratorID + 0x7EC),
    (void*)((u8*)psCreateGeneratorID + 0x360),
    (void*)((u8*)psCreateGeneratorID + 0x360),
    (void*)((u8*)psCreateGeneratorID + 0x43C),
    (void*)((u8*)psCreateGeneratorID + 0x3E0),
    (void*)((u8*)psCreateGeneratorID + 0x3E0),
    (void*)((u8*)psCreateGeneratorID + 0x504),
};

void* jumptable_8036C204[8] = {
    (void*)((u8*)generateParticle_8017424C + 0x1024),
    (void*)((u8*)generateParticle_8017424C + 0xDEC),
    (void*)((u8*)generateParticle_8017424C + 0xE10),
    (void*)((u8*)generateParticle_8017424C + 0xE34),
    (void*)((u8*)generateParticle_8017424C + 0xE98),
    (void*)((u8*)generateParticle_8017424C + 0xEBC),
    (void*)((u8*)generateParticle_8017424C + 0xF20),
    (void*)((u8*)generateParticle_8017424C + 0xF84),
};

void* jumptable_8036C224[9] = {
    (void*)((u8*)generateParticle_8017424C + 0x7A4),
    (void*)((u8*)generateParticle_8017424C + 0xC30),
    (void*)((u8*)generateParticle_8017424C + 0xD08),
    (void*)((u8*)generateParticle_8017424C + 0x7A4),
    (void*)((u8*)generateParticle_8017424C + 0x7A4),
    (void*)((u8*)generateParticle_8017424C + 0xDB0),
    (void*)((u8*)generateParticle_8017424C + 0x7A4),
    (void*)((u8*)generateParticle_8017424C + 0x7A4),
    (void*)((u8*)generateParticle_8017424C + 0x1270),
};

u8 lbl_8036C248[12] = {
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void* jumptable_8036C254[9] = {
    (void*)((u8*)fn_80177A64 + 0x7C4),
    (void*)((u8*)fn_80177A64 + 0x89C),
    (void*)((u8*)fn_80177A64 + 0x89C),
    (void*)((u8*)fn_80177A64 + 0xA6C),
    (void*)((u8*)fn_80177A64 + 0xBC8),
    (void*)((u8*)fn_80177A64 + 0xA60),
    (void*)((u8*)fn_80177A64 + 0xA54),
    (void*)((u8*)fn_80177A64 + 0xAF4),
    (void*)((u8*)fn_80177A64 + 0xAE4),
};

void* jumptable_8036C278[9] = {
    (void*)((u8*)fn_80177A64 + 0x6A0),
    (void*)((u8*)fn_80177A64 + 0x6A0),
    (void*)((u8*)fn_80177A64 + 0x79C),
    (void*)((u8*)fn_80177A64 + 0x79C),
    (void*)((u8*)fn_80177A64 + 0x714),
    (void*)((u8*)fn_80177A64 + 0x6A0),
    (void*)((u8*)fn_80177A64 + 0x79C),
    (void*)((u8*)fn_80177A64 + 0x6A0),
    (void*)((u8*)fn_80177A64 + 0x79C),
};

