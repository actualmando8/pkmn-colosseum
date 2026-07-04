#include "dolphin/types.h"

#pragma section ".data"

extern u8 pad_05_8036C8BC_data[];
extern void* jumptable_8036C8C0[];
extern void* lbl_8036C8E0[];
extern void* jumptable_8036C934[];

extern u8 fn_8019C3C4[];
extern u8 fn_8019CE50[];
extern u8 fn_801A20C8[];

/* Auto-carved .data unit 0x8036C8BC..0x8036CA1C (4 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 pad_05_8036C8BC_data[4] = {
    0x00, 0x00, 0x00, 0x00,
};

void* jumptable_8036C8C0[8] = {
    (void*)((u8*)fn_8019C3C4 + 0xD0),
    (void*)((u8*)fn_8019C3C4 + 0xF4),
    (void*)((u8*)fn_8019C3C4 + 0x260),
    (void*)((u8*)fn_8019C3C4 + 0x298),
    (void*)((u8*)fn_8019C3C4 + 0x23C),
    (void*)((u8*)fn_8019C3C4 + 0x118),
    (void*)((u8*)fn_8019C3C4 + 0x140),
    (void*)((u8*)fn_8019C3C4 + 0x174),
};

void* lbl_8036C8E0[21] = {
    (void*)((u8*)fn_8019CE50),
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

void* jumptable_8036C934[58] = {
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0x260),
    (void*)((u8*)fn_801A20C8 + 0x328),
    (void*)((u8*)fn_801A20C8 + 0x3C4),
    (void*)((u8*)fn_801A20C8 + 0x50),
    (void*)((u8*)fn_801A20C8 + 0x460),
    (void*)((u8*)fn_801A20C8 + 0x4E0),
    (void*)((u8*)fn_801A20C8 + 0x560),
    (void*)((u8*)fn_801A20C8 + 0x5E0),
    (void*)((u8*)fn_801A20C8 + 0x660),
    (void*)((u8*)fn_801A20C8 + 0x6E0),
    (void*)((u8*)fn_801A20C8 + 0x790),
    (void*)((u8*)fn_801A20C8 + 0x760),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x890),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x8DC),
    (void*)((u8*)fn_801A20C8 + 0x90C),
    (void*)((u8*)fn_801A20C8 + 0x940),
    (void*)((u8*)fn_801A20C8 + 0x960),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0xA78),
    (void*)((u8*)fn_801A20C8 + 0x984),
    (void*)((u8*)fn_801A20C8 + 0x9A0),
    (void*)((u8*)fn_801A20C8 + 0x9BC),
    (void*)((u8*)fn_801A20C8 + 0x9D8),
    (void*)((u8*)fn_801A20C8 + 0x9F4),
    (void*)((u8*)fn_801A20C8 + 0x9F4),
    (void*)((u8*)fn_801A20C8 + 0x9F4),
    (void*)((u8*)fn_801A20C8 + 0x9F4),
};

