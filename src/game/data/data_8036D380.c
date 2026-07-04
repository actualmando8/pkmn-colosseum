#include "dolphin/types.h"

#pragma section ".data"

extern u8 lbl_8036D380[];
extern void* jumptable_8036D3B0[];
extern void* lbl_8036D3F0[];
extern u8 lbl_8036D43C[];
extern u8 lbl_8036D46C[];
extern u8 lbl_8036D48C[];
extern u8 lbl_8036D510[];
extern u8 lbl_8036D594[];
extern void* jumptable_8036D5A4[];
extern void* jumptable_8036D5D0[];
extern void* jumptable_8036D5F0[];
extern void* jumptable_8036D610[];
extern void* jumptable_8036D630[];
extern void* jumptable_8036D650[];
extern void* jumptable_8036D670[];
extern void* jumptable_8036D690[];
extern void* jumptable_8036D6B0[];
extern void* jumptable_8036D6D0[];
extern void* jumptable_8036D6F0[];
extern void* jumptable_8036D714[];
extern void* jumptable_8036D734[];
extern void* jumptable_8036D798[];
extern void* jumptable_8036D7C0[];
extern void* jumptable_8036D7F0[];
extern void* jumptable_8036D820[];
extern void* jumptable_8036D850[];
extern void* jumptable_8036D880[];
extern void* jumptable_8036D8B0[];
extern void* jumptable_8036D8E0[];
extern void* jumptable_8036D910[];
extern void* jumptable_8036D940[];
extern void* jumptable_8036D974[];
extern void* jumptable_8036D9A4[];
extern void* jumptable_8036D9D4[];
extern void* jumptable_8036DA04[];
extern void* jumptable_8036DA34[];
extern void* jumptable_8036DA64[];
extern void* jumptable_8036DA94[];
extern void* jumptable_8036DAC4[];
extern void* jumptable_8036DAF4[];
extern void* jumptable_8036DB24[];
extern void* jumptable_8036DB54[];
extern void* jumptable_8036DB84[];
extern void* jumptable_8036DBB4[];
extern void* jumptable_8036DBE4[];
extern void* jumptable_8036DC14[];
extern void* jumptable_8036DC44[];
extern void* jumptable_8036DC74[];

extern u8 CObjForeachAnim[];
extern u8 DObjForeachAnim[];
extern u8 HSD_ForeachAnim[];
extern u8 HSD_Index2PosNrmMtx[];
extern u8 HSD_Index2TexMtx[];
extern u8 HSD_TObjAssignResources[];
extern u8 HSD_TObjSetup[];
extern u8 JObjForeachAnim[];
extern u8 LObjForeachAnim[];
extern u8 MakeColorGenTExp[];
extern u8 TObjInfoInit[];
extern u8 fn_801BAC8C[];
extern u8 fn_801BDA58[];
extern u8 fn_801BE85C[];

/* Auto-carved .data unit 0x8036D380..0x8036DCA4 (48 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 lbl_8036D380[48] = {
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
};

void* jumptable_8036D3B0[16] = {
    (void*)((u8*)fn_801BAC8C + 0x1B4),
    (void*)((u8*)fn_801BAC8C + 0x1B4),
    (void*)((u8*)fn_801BAC8C + 0x49C),
    (void*)((u8*)fn_801BAC8C + 0x49C),
    (void*)((u8*)fn_801BAC8C + 0x49C),
    (void*)((u8*)fn_801BAC8C + 0x49C),
    (void*)((u8*)fn_801BAC8C + 0x49C),
    (void*)((u8*)fn_801BAC8C + 0x49C),
    (void*)((u8*)fn_801BAC8C + 0x35C),
    (void*)((u8*)fn_801BAC8C + 0x404),
    (void*)((u8*)fn_801BAC8C + 0x35C),
    (void*)((u8*)fn_801BAC8C + 0x404),
    (void*)((u8*)fn_801BAC8C + 0x35C),
    (void*)((u8*)fn_801BAC8C + 0x404),
    (void*)((u8*)fn_801BAC8C + 0x35C),
    (void*)((u8*)fn_801BAC8C + 0x404),
};

void* lbl_8036D3F0[19] = {
    (void*)((u8*)TObjInfoInit),
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

u8 lbl_8036D43C[48] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 lbl_8036D46C[32] = {
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09,
};

u8 lbl_8036D48C[132] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
};

u8 lbl_8036D510[132] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
};

u8 lbl_8036D594[16] = {
    0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

void* jumptable_8036D5A4[11] = {
    (void*)((u8*)HSD_Index2TexMtx + 0x2C),
    (void*)((u8*)HSD_Index2TexMtx + 0x34),
    (void*)((u8*)HSD_Index2TexMtx + 0x3C),
    (void*)((u8*)HSD_Index2TexMtx + 0x44),
    (void*)((u8*)HSD_Index2TexMtx + 0x4C),
    (void*)((u8*)HSD_Index2TexMtx + 0x54),
    (void*)((u8*)HSD_Index2TexMtx + 0x5C),
    (void*)((u8*)HSD_Index2TexMtx + 0x64),
    (void*)((u8*)HSD_Index2TexMtx + 0x6C),
    (void*)((u8*)HSD_Index2TexMtx + 0x74),
    (void*)((u8*)HSD_Index2TexMtx + 0x7C),
};

void* jumptable_8036D5D0[8] = {
    (void*)((u8*)HSD_TObjSetup + 0x5C),
    (void*)((u8*)HSD_TObjSetup + 0x64),
    (void*)((u8*)HSD_TObjSetup + 0x6C),
    (void*)((u8*)HSD_TObjSetup + 0x74),
    (void*)((u8*)HSD_TObjSetup + 0x7C),
    (void*)((u8*)HSD_TObjSetup + 0x84),
    (void*)((u8*)HSD_TObjSetup + 0x8C),
    (void*)((u8*)HSD_TObjSetup + 0x94),
};

void* jumptable_8036D5F0[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x510),
    (void*)((u8*)HSD_TObjAssignResources + 0x518),
    (void*)((u8*)HSD_TObjAssignResources + 0x520),
    (void*)((u8*)HSD_TObjAssignResources + 0x528),
    (void*)((u8*)HSD_TObjAssignResources + 0x530),
    (void*)((u8*)HSD_TObjAssignResources + 0x538),
    (void*)((u8*)HSD_TObjAssignResources + 0x540),
    (void*)((u8*)HSD_TObjAssignResources + 0x548),
};

void* jumptable_8036D610[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x490),
    (void*)((u8*)HSD_TObjAssignResources + 0x498),
    (void*)((u8*)HSD_TObjAssignResources + 0x4A0),
    (void*)((u8*)HSD_TObjAssignResources + 0x4A8),
    (void*)((u8*)HSD_TObjAssignResources + 0x4B0),
    (void*)((u8*)HSD_TObjAssignResources + 0x4B8),
    (void*)((u8*)HSD_TObjAssignResources + 0x4C0),
    (void*)((u8*)HSD_TObjAssignResources + 0x4C8),
};

void* jumptable_8036D630[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x408),
    (void*)((u8*)HSD_TObjAssignResources + 0x410),
    (void*)((u8*)HSD_TObjAssignResources + 0x418),
    (void*)((u8*)HSD_TObjAssignResources + 0x420),
    (void*)((u8*)HSD_TObjAssignResources + 0x428),
    (void*)((u8*)HSD_TObjAssignResources + 0x430),
    (void*)((u8*)HSD_TObjAssignResources + 0x438),
    (void*)((u8*)HSD_TObjAssignResources + 0x440),
};

void* jumptable_8036D650[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x388),
    (void*)((u8*)HSD_TObjAssignResources + 0x390),
    (void*)((u8*)HSD_TObjAssignResources + 0x398),
    (void*)((u8*)HSD_TObjAssignResources + 0x3A0),
    (void*)((u8*)HSD_TObjAssignResources + 0x3A8),
    (void*)((u8*)HSD_TObjAssignResources + 0x3B0),
    (void*)((u8*)HSD_TObjAssignResources + 0x3B8),
    (void*)((u8*)HSD_TObjAssignResources + 0x3C0),
};

void* jumptable_8036D670[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x2F4),
    (void*)((u8*)HSD_TObjAssignResources + 0x2FC),
    (void*)((u8*)HSD_TObjAssignResources + 0x304),
    (void*)((u8*)HSD_TObjAssignResources + 0x30C),
    (void*)((u8*)HSD_TObjAssignResources + 0x314),
    (void*)((u8*)HSD_TObjAssignResources + 0x31C),
    (void*)((u8*)HSD_TObjAssignResources + 0x324),
    (void*)((u8*)HSD_TObjAssignResources + 0x32C),
};

void* jumptable_8036D690[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x22C),
    (void*)((u8*)HSD_TObjAssignResources + 0x234),
    (void*)((u8*)HSD_TObjAssignResources + 0x23C),
    (void*)((u8*)HSD_TObjAssignResources + 0x244),
    (void*)((u8*)HSD_TObjAssignResources + 0x24C),
    (void*)((u8*)HSD_TObjAssignResources + 0x254),
    (void*)((u8*)HSD_TObjAssignResources + 0x25C),
    (void*)((u8*)HSD_TObjAssignResources + 0x264),
};

void* jumptable_8036D6B0[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x180),
    (void*)((u8*)HSD_TObjAssignResources + 0x188),
    (void*)((u8*)HSD_TObjAssignResources + 0x190),
    (void*)((u8*)HSD_TObjAssignResources + 0x198),
    (void*)((u8*)HSD_TObjAssignResources + 0x1A0),
    (void*)((u8*)HSD_TObjAssignResources + 0x1A8),
    (void*)((u8*)HSD_TObjAssignResources + 0x1B0),
    (void*)((u8*)HSD_TObjAssignResources + 0x1B8),
};

void* jumptable_8036D6D0[8] = {
    (void*)((u8*)HSD_TObjAssignResources + 0x104),
    (void*)((u8*)HSD_TObjAssignResources + 0x10C),
    (void*)((u8*)HSD_TObjAssignResources + 0x114),
    (void*)((u8*)HSD_TObjAssignResources + 0x11C),
    (void*)((u8*)HSD_TObjAssignResources + 0x124),
    (void*)((u8*)HSD_TObjAssignResources + 0x12C),
    (void*)((u8*)HSD_TObjAssignResources + 0x134),
    (void*)((u8*)HSD_TObjAssignResources + 0x13C),
};

void* jumptable_8036D6F0[9] = {
    (void*)((u8*)MakeColorGenTExp + 0x84),
    (void*)((u8*)MakeColorGenTExp + 0x8C),
    (void*)((u8*)MakeColorGenTExp + 0x94),
    (void*)((u8*)MakeColorGenTExp + 0x9C),
    (void*)((u8*)MakeColorGenTExp + 0xA4),
    (void*)((u8*)MakeColorGenTExp + 0xAC),
    (void*)((u8*)MakeColorGenTExp + 0xB8),
    (void*)((u8*)MakeColorGenTExp + 0xC0),
    (void*)((u8*)MakeColorGenTExp + 0xCC),
};

void* jumptable_8036D714[8] = {
    (void*)((u8*)fn_801BDA58 + 0x16C),
    (void*)((u8*)fn_801BDA58 + 0x174),
    (void*)((u8*)fn_801BDA58 + 0x17C),
    (void*)((u8*)fn_801BDA58 + 0x184),
    (void*)((u8*)fn_801BDA58 + 0x18C),
    (void*)((u8*)fn_801BDA58 + 0x194),
    (void*)((u8*)fn_801BDA58 + 0x19C),
    (void*)((u8*)fn_801BDA58 + 0x1A4),
};

void* jumptable_8036D734[25] = {
    (void*)((u8*)fn_801BE85C + 0x5F4),
    (void*)((u8*)fn_801BE85C + 0x40),
    (void*)((u8*)fn_801BE85C + 0x118),
    (void*)((u8*)fn_801BE85C + 0x124),
    (void*)((u8*)fn_801BE85C + 0x130),
    (void*)((u8*)fn_801BE85C + 0x13C),
    (void*)((u8*)fn_801BE85C + 0xF4),
    (void*)((u8*)fn_801BE85C + 0x100),
    (void*)((u8*)fn_801BE85C + 0x10C),
    (void*)((u8*)fn_801BE85C + 0xBC),
    (void*)((u8*)fn_801BE85C + 0x98),
    (void*)((u8*)fn_801BE85C + 0x154),
    (void*)((u8*)fn_801BE85C + 0x170),
    (void*)((u8*)fn_801BE85C + 0x1CC),
    (void*)((u8*)fn_801BE85C + 0x228),
    (void*)((u8*)fn_801BE85C + 0x284),
    (void*)((u8*)fn_801BE85C + 0x2E0),
    (void*)((u8*)fn_801BE85C + 0x33C),
    (void*)((u8*)fn_801BE85C + 0x398),
    (void*)((u8*)fn_801BE85C + 0x3F4),
    (void*)((u8*)fn_801BE85C + 0x450),
    (void*)((u8*)fn_801BE85C + 0x4AC),
    (void*)((u8*)fn_801BE85C + 0x508),
    (void*)((u8*)fn_801BE85C + 0x564),
    (void*)((u8*)fn_801BE85C + 0x5C0),
};

void* jumptable_8036D798[10] = {
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x2C),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x34),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x3C),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x44),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x4C),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x54),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x5C),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x64),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x6C),
    (void*)((u8*)HSD_Index2PosNrmMtx + 0x74),
};

void* jumptable_8036D7C0[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0xB58),
    (void*)((u8*)HSD_ForeachAnim + 0xB68),
    (void*)((u8*)HSD_ForeachAnim + 0xB7C),
    (void*)((u8*)HSD_ForeachAnim + 0xB90),
    (void*)((u8*)HSD_ForeachAnim + 0xBA4),
    (void*)((u8*)HSD_ForeachAnim + 0xBD0),
    (void*)((u8*)HSD_ForeachAnim + 0xBE8),
    (void*)((u8*)HSD_ForeachAnim + 0xC00),
    (void*)((u8*)HSD_ForeachAnim + 0xBB8),
    (void*)((u8*)HSD_ForeachAnim + 0xC18),
    (void*)((u8*)HSD_ForeachAnim + 0xC34),
    (void*)((u8*)HSD_ForeachAnim + 0xC50),
};

void* jumptable_8036D7F0[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x9F8),
    (void*)((u8*)HSD_ForeachAnim + 0xA08),
    (void*)((u8*)HSD_ForeachAnim + 0xA1C),
    (void*)((u8*)HSD_ForeachAnim + 0xA30),
    (void*)((u8*)HSD_ForeachAnim + 0xA44),
    (void*)((u8*)HSD_ForeachAnim + 0xA70),
    (void*)((u8*)HSD_ForeachAnim + 0xA88),
    (void*)((u8*)HSD_ForeachAnim + 0xAA0),
    (void*)((u8*)HSD_ForeachAnim + 0xA58),
    (void*)((u8*)HSD_ForeachAnim + 0xAB8),
    (void*)((u8*)HSD_ForeachAnim + 0xAD4),
    (void*)((u8*)HSD_ForeachAnim + 0xAF0),
};

void* jumptable_8036D820[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x8A8),
    (void*)((u8*)HSD_ForeachAnim + 0x8B8),
    (void*)((u8*)HSD_ForeachAnim + 0x8CC),
    (void*)((u8*)HSD_ForeachAnim + 0x8E0),
    (void*)((u8*)HSD_ForeachAnim + 0x8F4),
    (void*)((u8*)HSD_ForeachAnim + 0x920),
    (void*)((u8*)HSD_ForeachAnim + 0x938),
    (void*)((u8*)HSD_ForeachAnim + 0x950),
    (void*)((u8*)HSD_ForeachAnim + 0x908),
    (void*)((u8*)HSD_ForeachAnim + 0x968),
    (void*)((u8*)HSD_ForeachAnim + 0x984),
    (void*)((u8*)HSD_ForeachAnim + 0x9A0),
};

void* jumptable_8036D850[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x748),
    (void*)((u8*)HSD_ForeachAnim + 0x758),
    (void*)((u8*)HSD_ForeachAnim + 0x76C),
    (void*)((u8*)HSD_ForeachAnim + 0x780),
    (void*)((u8*)HSD_ForeachAnim + 0x794),
    (void*)((u8*)HSD_ForeachAnim + 0x7C0),
    (void*)((u8*)HSD_ForeachAnim + 0x7D8),
    (void*)((u8*)HSD_ForeachAnim + 0x7F0),
    (void*)((u8*)HSD_ForeachAnim + 0x7A8),
    (void*)((u8*)HSD_ForeachAnim + 0x808),
    (void*)((u8*)HSD_ForeachAnim + 0x824),
    (void*)((u8*)HSD_ForeachAnim + 0x840),
};

void* jumptable_8036D880[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x5B0),
    (void*)((u8*)HSD_ForeachAnim + 0x5C0),
    (void*)((u8*)HSD_ForeachAnim + 0x5D4),
    (void*)((u8*)HSD_ForeachAnim + 0x5E8),
    (void*)((u8*)HSD_ForeachAnim + 0x5FC),
    (void*)((u8*)HSD_ForeachAnim + 0x628),
    (void*)((u8*)HSD_ForeachAnim + 0x640),
    (void*)((u8*)HSD_ForeachAnim + 0x658),
    (void*)((u8*)HSD_ForeachAnim + 0x610),
    (void*)((u8*)HSD_ForeachAnim + 0x670),
    (void*)((u8*)HSD_ForeachAnim + 0x68C),
    (void*)((u8*)HSD_ForeachAnim + 0x6A8),
};

void* jumptable_8036D8B0[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x450),
    (void*)((u8*)HSD_ForeachAnim + 0x460),
    (void*)((u8*)HSD_ForeachAnim + 0x474),
    (void*)((u8*)HSD_ForeachAnim + 0x488),
    (void*)((u8*)HSD_ForeachAnim + 0x49C),
    (void*)((u8*)HSD_ForeachAnim + 0x4C8),
    (void*)((u8*)HSD_ForeachAnim + 0x4E0),
    (void*)((u8*)HSD_ForeachAnim + 0x4F8),
    (void*)((u8*)HSD_ForeachAnim + 0x4B0),
    (void*)((u8*)HSD_ForeachAnim + 0x510),
    (void*)((u8*)HSD_ForeachAnim + 0x52C),
    (void*)((u8*)HSD_ForeachAnim + 0x548),
};

void* jumptable_8036D8E0[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x2F0),
    (void*)((u8*)HSD_ForeachAnim + 0x300),
    (void*)((u8*)HSD_ForeachAnim + 0x314),
    (void*)((u8*)HSD_ForeachAnim + 0x328),
    (void*)((u8*)HSD_ForeachAnim + 0x33C),
    (void*)((u8*)HSD_ForeachAnim + 0x368),
    (void*)((u8*)HSD_ForeachAnim + 0x380),
    (void*)((u8*)HSD_ForeachAnim + 0x398),
    (void*)((u8*)HSD_ForeachAnim + 0x350),
    (void*)((u8*)HSD_ForeachAnim + 0x3B0),
    (void*)((u8*)HSD_ForeachAnim + 0x3CC),
    (void*)((u8*)HSD_ForeachAnim + 0x3E8),
};

void* jumptable_8036D910[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x1A0),
    (void*)((u8*)HSD_ForeachAnim + 0x1B0),
    (void*)((u8*)HSD_ForeachAnim + 0x1C4),
    (void*)((u8*)HSD_ForeachAnim + 0x1D8),
    (void*)((u8*)HSD_ForeachAnim + 0x1EC),
    (void*)((u8*)HSD_ForeachAnim + 0x218),
    (void*)((u8*)HSD_ForeachAnim + 0x230),
    (void*)((u8*)HSD_ForeachAnim + 0x248),
    (void*)((u8*)HSD_ForeachAnim + 0x200),
    (void*)((u8*)HSD_ForeachAnim + 0x260),
    (void*)((u8*)HSD_ForeachAnim + 0x27C),
    (void*)((u8*)HSD_ForeachAnim + 0x298),
};

void* jumptable_8036D940[13] = {
    (void*)((u8*)HSD_ForeachAnim + 0xC6C),
    (void*)((u8*)HSD_ForeachAnim + 0xC6C),
    (void*)((u8*)HSD_ForeachAnim + 0x6EC),
    (void*)((u8*)HSD_ForeachAnim + 0x144),
    (void*)((u8*)HSD_ForeachAnim + 0xC6C),
    (void*)((u8*)HSD_ForeachAnim + 0xB18),
    (void*)((u8*)HSD_ForeachAnim + 0x128),
    (void*)((u8*)HSD_ForeachAnim + 0x6D0),
    (void*)((u8*)HSD_ForeachAnim + 0x160),
    (void*)((u8*)HSD_ForeachAnim + 0x410),
    (void*)((u8*)HSD_ForeachAnim + 0x708),
    (void*)((u8*)HSD_ForeachAnim + 0x570),
    (void*)((u8*)HSD_ForeachAnim + 0x868),
};

void* jumptable_8036D974[12] = {
    (void*)((u8*)HSD_ForeachAnim + 0x108),
    (void*)((u8*)HSD_ForeachAnim + 0xA8),
    (void*)((u8*)HSD_ForeachAnim + 0xC4),
    (void*)((u8*)HSD_ForeachAnim + 0xDC),
    (void*)((u8*)HSD_ForeachAnim + 0x108),
    (void*)((u8*)HSD_ForeachAnim + 0xA8),
    (void*)((u8*)HSD_ForeachAnim + 0xC4),
    (void*)((u8*)HSD_ForeachAnim + 0xDC),
    (void*)((u8*)HSD_ForeachAnim + 0x108),
    (void*)((u8*)HSD_ForeachAnim + 0xA8),
    (void*)((u8*)HSD_ForeachAnim + 0xC4),
    (void*)((u8*)HSD_ForeachAnim + 0xDC),
};

void* jumptable_8036D9A4[12] = {
    (void*)((u8*)JObjForeachAnim + 0x1EC),
    (void*)((u8*)JObjForeachAnim + 0x1FC),
    (void*)((u8*)JObjForeachAnim + 0x210),
    (void*)((u8*)JObjForeachAnim + 0x224),
    (void*)((u8*)JObjForeachAnim + 0x238),
    (void*)((u8*)JObjForeachAnim + 0x264),
    (void*)((u8*)JObjForeachAnim + 0x27C),
    (void*)((u8*)JObjForeachAnim + 0x294),
    (void*)((u8*)JObjForeachAnim + 0x24C),
    (void*)((u8*)JObjForeachAnim + 0x2AC),
    (void*)((u8*)JObjForeachAnim + 0x2C8),
    (void*)((u8*)JObjForeachAnim + 0x2E4),
};

void* jumptable_8036D9D4[12] = {
    (void*)((u8*)JObjForeachAnim + 0x70),
    (void*)((u8*)JObjForeachAnim + 0x80),
    (void*)((u8*)JObjForeachAnim + 0x94),
    (void*)((u8*)JObjForeachAnim + 0xA8),
    (void*)((u8*)JObjForeachAnim + 0xBC),
    (void*)((u8*)JObjForeachAnim + 0xE8),
    (void*)((u8*)JObjForeachAnim + 0x100),
    (void*)((u8*)JObjForeachAnim + 0x118),
    (void*)((u8*)JObjForeachAnim + 0xD0),
    (void*)((u8*)JObjForeachAnim + 0x130),
    (void*)((u8*)JObjForeachAnim + 0x14C),
    (void*)((u8*)JObjForeachAnim + 0x168),
};

void* jumptable_8036DA04[12] = {
    (void*)((u8*)DObjForeachAnim + 0x460),
    (void*)((u8*)DObjForeachAnim + 0x470),
    (void*)((u8*)DObjForeachAnim + 0x484),
    (void*)((u8*)DObjForeachAnim + 0x498),
    (void*)((u8*)DObjForeachAnim + 0x4AC),
    (void*)((u8*)DObjForeachAnim + 0x4D8),
    (void*)((u8*)DObjForeachAnim + 0x4F0),
    (void*)((u8*)DObjForeachAnim + 0x508),
    (void*)((u8*)DObjForeachAnim + 0x4C0),
    (void*)((u8*)DObjForeachAnim + 0x520),
    (void*)((u8*)DObjForeachAnim + 0x53C),
    (void*)((u8*)DObjForeachAnim + 0x558),
};

void* jumptable_8036DA34[12] = {
    (void*)((u8*)DObjForeachAnim + 0x304),
    (void*)((u8*)DObjForeachAnim + 0x314),
    (void*)((u8*)DObjForeachAnim + 0x328),
    (void*)((u8*)DObjForeachAnim + 0x33C),
    (void*)((u8*)DObjForeachAnim + 0x350),
    (void*)((u8*)DObjForeachAnim + 0x37C),
    (void*)((u8*)DObjForeachAnim + 0x394),
    (void*)((u8*)DObjForeachAnim + 0x3AC),
    (void*)((u8*)DObjForeachAnim + 0x364),
    (void*)((u8*)DObjForeachAnim + 0x3C4),
    (void*)((u8*)DObjForeachAnim + 0x3E0),
    (void*)((u8*)DObjForeachAnim + 0x3FC),
};

void* jumptable_8036DA64[12] = {
    (void*)((u8*)DObjForeachAnim + 0x1B4),
    (void*)((u8*)DObjForeachAnim + 0x1C4),
    (void*)((u8*)DObjForeachAnim + 0x1D8),
    (void*)((u8*)DObjForeachAnim + 0x1EC),
    (void*)((u8*)DObjForeachAnim + 0x200),
    (void*)((u8*)DObjForeachAnim + 0x22C),
    (void*)((u8*)DObjForeachAnim + 0x244),
    (void*)((u8*)DObjForeachAnim + 0x25C),
    (void*)((u8*)DObjForeachAnim + 0x214),
    (void*)((u8*)DObjForeachAnim + 0x274),
    (void*)((u8*)DObjForeachAnim + 0x290),
    (void*)((u8*)DObjForeachAnim + 0x2AC),
};

void* jumptable_8036DA94[12] = {
    (void*)((u8*)DObjForeachAnim + 0x60),
    (void*)((u8*)DObjForeachAnim + 0x70),
    (void*)((u8*)DObjForeachAnim + 0x84),
    (void*)((u8*)DObjForeachAnim + 0x98),
    (void*)((u8*)DObjForeachAnim + 0xAC),
    (void*)((u8*)DObjForeachAnim + 0xD8),
    (void*)((u8*)DObjForeachAnim + 0xF0),
    (void*)((u8*)DObjForeachAnim + 0x108),
    (void*)((u8*)DObjForeachAnim + 0xC0),
    (void*)((u8*)DObjForeachAnim + 0x120),
    (void*)((u8*)DObjForeachAnim + 0x13C),
    (void*)((u8*)DObjForeachAnim + 0x158),
};

void* jumptable_8036DAC4[12] = {
    (void*)((u8*)LObjForeachAnim + 0x5B4),
    (void*)((u8*)LObjForeachAnim + 0x5C4),
    (void*)((u8*)LObjForeachAnim + 0x5D8),
    (void*)((u8*)LObjForeachAnim + 0x5EC),
    (void*)((u8*)LObjForeachAnim + 0x600),
    (void*)((u8*)LObjForeachAnim + 0x62C),
    (void*)((u8*)LObjForeachAnim + 0x644),
    (void*)((u8*)LObjForeachAnim + 0x65C),
    (void*)((u8*)LObjForeachAnim + 0x614),
    (void*)((u8*)LObjForeachAnim + 0x674),
    (void*)((u8*)LObjForeachAnim + 0x690),
    (void*)((u8*)LObjForeachAnim + 0x6AC),
};

void* jumptable_8036DAF4[12] = {
    (void*)((u8*)LObjForeachAnim + 0x464),
    (void*)((u8*)LObjForeachAnim + 0x474),
    (void*)((u8*)LObjForeachAnim + 0x488),
    (void*)((u8*)LObjForeachAnim + 0x49C),
    (void*)((u8*)LObjForeachAnim + 0x4B0),
    (void*)((u8*)LObjForeachAnim + 0x4DC),
    (void*)((u8*)LObjForeachAnim + 0x4F4),
    (void*)((u8*)LObjForeachAnim + 0x50C),
    (void*)((u8*)LObjForeachAnim + 0x4C4),
    (void*)((u8*)LObjForeachAnim + 0x524),
    (void*)((u8*)LObjForeachAnim + 0x540),
    (void*)((u8*)LObjForeachAnim + 0x55C),
};

void* jumptable_8036DB24[12] = {
    (void*)((u8*)LObjForeachAnim + 0x304),
    (void*)((u8*)LObjForeachAnim + 0x314),
    (void*)((u8*)LObjForeachAnim + 0x328),
    (void*)((u8*)LObjForeachAnim + 0x33C),
    (void*)((u8*)LObjForeachAnim + 0x350),
    (void*)((u8*)LObjForeachAnim + 0x37C),
    (void*)((u8*)LObjForeachAnim + 0x394),
    (void*)((u8*)LObjForeachAnim + 0x3AC),
    (void*)((u8*)LObjForeachAnim + 0x364),
    (void*)((u8*)LObjForeachAnim + 0x3C4),
    (void*)((u8*)LObjForeachAnim + 0x3E0),
    (void*)((u8*)LObjForeachAnim + 0x3FC),
};

void* jumptable_8036DB54[12] = {
    (void*)((u8*)LObjForeachAnim + 0x1B4),
    (void*)((u8*)LObjForeachAnim + 0x1C4),
    (void*)((u8*)LObjForeachAnim + 0x1D8),
    (void*)((u8*)LObjForeachAnim + 0x1EC),
    (void*)((u8*)LObjForeachAnim + 0x200),
    (void*)((u8*)LObjForeachAnim + 0x22C),
    (void*)((u8*)LObjForeachAnim + 0x244),
    (void*)((u8*)LObjForeachAnim + 0x25C),
    (void*)((u8*)LObjForeachAnim + 0x214),
    (void*)((u8*)LObjForeachAnim + 0x274),
    (void*)((u8*)LObjForeachAnim + 0x290),
    (void*)((u8*)LObjForeachAnim + 0x2AC),
};

void* jumptable_8036DB84[12] = {
    (void*)((u8*)LObjForeachAnim + 0x60),
    (void*)((u8*)LObjForeachAnim + 0x70),
    (void*)((u8*)LObjForeachAnim + 0x84),
    (void*)((u8*)LObjForeachAnim + 0x98),
    (void*)((u8*)LObjForeachAnim + 0xAC),
    (void*)((u8*)LObjForeachAnim + 0xD8),
    (void*)((u8*)LObjForeachAnim + 0xF0),
    (void*)((u8*)LObjForeachAnim + 0x108),
    (void*)((u8*)LObjForeachAnim + 0xC0),
    (void*)((u8*)LObjForeachAnim + 0x120),
    (void*)((u8*)LObjForeachAnim + 0x13C),
    (void*)((u8*)LObjForeachAnim + 0x158),
};

void* jumptable_8036DBB4[12] = {
    (void*)((u8*)CObjForeachAnim + 0x5B4),
    (void*)((u8*)CObjForeachAnim + 0x5C4),
    (void*)((u8*)CObjForeachAnim + 0x5D8),
    (void*)((u8*)CObjForeachAnim + 0x5EC),
    (void*)((u8*)CObjForeachAnim + 0x600),
    (void*)((u8*)CObjForeachAnim + 0x62C),
    (void*)((u8*)CObjForeachAnim + 0x644),
    (void*)((u8*)CObjForeachAnim + 0x65C),
    (void*)((u8*)CObjForeachAnim + 0x614),
    (void*)((u8*)CObjForeachAnim + 0x674),
    (void*)((u8*)CObjForeachAnim + 0x690),
    (void*)((u8*)CObjForeachAnim + 0x6AC),
};

void* jumptable_8036DBE4[12] = {
    (void*)((u8*)CObjForeachAnim + 0x464),
    (void*)((u8*)CObjForeachAnim + 0x474),
    (void*)((u8*)CObjForeachAnim + 0x488),
    (void*)((u8*)CObjForeachAnim + 0x49C),
    (void*)((u8*)CObjForeachAnim + 0x4B0),
    (void*)((u8*)CObjForeachAnim + 0x4DC),
    (void*)((u8*)CObjForeachAnim + 0x4F4),
    (void*)((u8*)CObjForeachAnim + 0x50C),
    (void*)((u8*)CObjForeachAnim + 0x4C4),
    (void*)((u8*)CObjForeachAnim + 0x524),
    (void*)((u8*)CObjForeachAnim + 0x540),
    (void*)((u8*)CObjForeachAnim + 0x55C),
};

void* jumptable_8036DC14[12] = {
    (void*)((u8*)CObjForeachAnim + 0x304),
    (void*)((u8*)CObjForeachAnim + 0x314),
    (void*)((u8*)CObjForeachAnim + 0x328),
    (void*)((u8*)CObjForeachAnim + 0x33C),
    (void*)((u8*)CObjForeachAnim + 0x350),
    (void*)((u8*)CObjForeachAnim + 0x37C),
    (void*)((u8*)CObjForeachAnim + 0x394),
    (void*)((u8*)CObjForeachAnim + 0x3AC),
    (void*)((u8*)CObjForeachAnim + 0x364),
    (void*)((u8*)CObjForeachAnim + 0x3C4),
    (void*)((u8*)CObjForeachAnim + 0x3E0),
    (void*)((u8*)CObjForeachAnim + 0x3FC),
};

void* jumptable_8036DC44[12] = {
    (void*)((u8*)CObjForeachAnim + 0x1B4),
    (void*)((u8*)CObjForeachAnim + 0x1C4),
    (void*)((u8*)CObjForeachAnim + 0x1D8),
    (void*)((u8*)CObjForeachAnim + 0x1EC),
    (void*)((u8*)CObjForeachAnim + 0x200),
    (void*)((u8*)CObjForeachAnim + 0x22C),
    (void*)((u8*)CObjForeachAnim + 0x244),
    (void*)((u8*)CObjForeachAnim + 0x25C),
    (void*)((u8*)CObjForeachAnim + 0x214),
    (void*)((u8*)CObjForeachAnim + 0x274),
    (void*)((u8*)CObjForeachAnim + 0x290),
    (void*)((u8*)CObjForeachAnim + 0x2AC),
};

void* jumptable_8036DC74[12] = {
    (void*)((u8*)CObjForeachAnim + 0x60),
    (void*)((u8*)CObjForeachAnim + 0x70),
    (void*)((u8*)CObjForeachAnim + 0x84),
    (void*)((u8*)CObjForeachAnim + 0x98),
    (void*)((u8*)CObjForeachAnim + 0xAC),
    (void*)((u8*)CObjForeachAnim + 0xD8),
    (void*)((u8*)CObjForeachAnim + 0xF0),
    (void*)((u8*)CObjForeachAnim + 0x108),
    (void*)((u8*)CObjForeachAnim + 0xC0),
    (void*)((u8*)CObjForeachAnim + 0x120),
    (void*)((u8*)CObjForeachAnim + 0x13C),
    (void*)((u8*)CObjForeachAnim + 0x158),
};

