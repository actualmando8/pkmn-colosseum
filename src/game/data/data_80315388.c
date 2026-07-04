#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_80315388[];
extern void* jumptable_8031540C[];
extern void* lbl_80315490[];
extern u8 lbl_803154E4[];
extern u8 lbl_80315540[];
extern u8 lbl_8031554C[];
extern u8 lbl_80315558[];
extern u8 lbl_80315568[];
extern void* lbl_80315598[];
extern void* jumptable_803155B0[];
extern void* lbl_803155D0[];
extern u8 lbl_80315668[];

extern u8 _GSmaterialObjInit_800EF33C[];
extern u8 fn_800DE128[];
extern u8 fn_800DE680[];
extern u8 fn_800EB464[];
extern u8 fn_800EB520[];
extern u8 fn_800ED4D4[];
extern u8 fn_800F10E8[];
extern u8 fn_800F13D0[];
extern u8 fn_800F16C0[];
extern u8 fn_800F1A0C[];
extern u8 fn_800F1E38[];
extern u8 fn_800F2264[];
extern u8 fn_800F24F4[];
extern u8 fn_800F27D4[];
extern u8 fn_800F2BE8[];
extern u8 fn_800F2FF8[];
extern u8 fn_800F3418[];
extern u8 fn_800F3830[];
extern u8 fn_800F3C50[];
extern u8 fn_800F4068[];
extern u8 fn_800F4440[];
extern u8 fn_800F4818[];
extern u8 fn_800F4C38[];
extern u8 fn_800F502C[];
extern u8 fn_800F5404[];
extern u8 fn_800F55DC[];
extern u8 fn_800F57F0[];
extern u8 fn_800F5A3C[];
extern u8 fn_800F5CA0[];
extern u8 fn_800F5EEC[];
extern u8 fn_800F62BC[];
extern u8 fn_800F668C[];
extern u8 fn_800F670C[];
extern u8 fn_800F67AC[];
extern u8 fn_800F67C8[];
extern u8 fn_800F694C[];
extern u8 fn_800F6AB4[];
extern u8 fn_800F6B54[];
extern u8 fn_800F6BAC[];
extern u8 fn_800F6BBC[];

/* Auto-carved .data unit 0x80315388..0x80315678 (12 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_80315388[33] = {
    (void*)((u8*)fn_800DE128 + 0x240),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0xC4),
    (void*)((u8*)fn_800DE128 + 0xFC),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x204),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x21C),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x3B0),
    (void*)((u8*)fn_800DE128 + 0x240),
};

void* jumptable_8031540C[33] = {
    (void*)((u8*)fn_800DE680 + 0x590),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0xC8),
    (void*)((u8*)fn_800DE680 + 0xF0),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x1E0),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x578),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x6D8),
    (void*)((u8*)fn_800DE680 + 0x590),
};

void* lbl_80315490[21] = {
    (void*)((u8*)_GSmaterialObjInit_800EF33C),
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
};

u8 lbl_803154E4[92] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 lbl_80315540[12] = {
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 lbl_8031554C[12] = {
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 lbl_80315558[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 lbl_80315568[48] = {
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void* lbl_80315598[6] = {
    (void*)((u8*)fn_800EB520),
    (void*)0x00000000,
    (void*)((u8*)fn_800EB464),
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
};

void* jumptable_803155B0[8] = {
    (void*)((u8*)fn_800ED4D4 + 0x194),
    (void*)((u8*)fn_800ED4D4 + 0xEC),
    (void*)((u8*)fn_800ED4D4 + 0xFC),
    (void*)((u8*)fn_800ED4D4 + 0x10C),
    (void*)((u8*)fn_800ED4D4 + 0x11C),
    (void*)((u8*)fn_800ED4D4 + 0x138),
    (void*)((u8*)fn_800ED4D4 + 0x154),
    (void*)((u8*)fn_800ED4D4 + 0x170),
};

void* lbl_803155D0[38] = {
    (void*)((u8*)fn_800F6BBC),
    (void*)((u8*)fn_800F6BAC),
    (void*)((u8*)fn_800F6B54),
    (void*)((u8*)fn_800F6AB4),
    (void*)((u8*)fn_800F694C),
    (void*)((u8*)fn_800F67C8),
    (void*)((u8*)fn_800F67AC),
    (void*)((u8*)fn_800F670C),
    (void*)((u8*)fn_800F668C),
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)((u8*)fn_800F62BC),
    (void*)((u8*)fn_800F5EEC),
    (void*)((u8*)fn_800F5CA0),
    (void*)((u8*)fn_800F5A3C),
    (void*)((u8*)fn_800F57F0),
    (void*)((u8*)fn_800F55DC),
    (void*)((u8*)fn_800F5404),
    (void*)((u8*)fn_800F502C),
    (void*)((u8*)fn_800F4C38),
    (void*)((u8*)fn_800F4818),
    (void*)((u8*)fn_800F4440),
    (void*)((u8*)fn_800F4068),
    (void*)((u8*)fn_800F3C50),
    (void*)((u8*)fn_800F3830),
    (void*)((u8*)fn_800F3418),
    (void*)((u8*)fn_800F2FF8),
    (void*)((u8*)fn_800F2BE8),
    (void*)((u8*)fn_800F27D4),
    (void*)((u8*)fn_800F24F4),
    (void*)((u8*)fn_800F2264),
    (void*)((u8*)fn_800F1E38),
    (void*)((u8*)fn_800F1A0C),
    (void*)((u8*)fn_800F16C0),
    (void*)((u8*)fn_800F13D0),
    (void*)((u8*)fn_800F10E8),
};

u8 lbl_80315668[16] = {
    0x5F, 0x76, 0x6D, 0x54, 0x68, 0x72, 0x65, 0x61, 0x64, 0x43, 0x72, 0x65,
    0x61, 0x74, 0x65, 0x00,
};

