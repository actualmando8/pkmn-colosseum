#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_80375628[];

extern u8 fn_801F75F8[];

/* Auto-carved .data unit 0x80375628..0x8037564C (1 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_80375628[9] = {
    (void*)((u8*)fn_801F75F8 + 0xAC),
    (void*)((u8*)fn_801F75F8 + 0x6C),
    (void*)((u8*)fn_801F75F8 + 0x78),
    (void*)((u8*)fn_801F75F8 + 0x88),
    (void*)((u8*)fn_801F75F8 + 0xAC),
    (void*)((u8*)fn_801F75F8 + 0x98),
    (void*)((u8*)fn_801F75F8 + 0xAC),
    (void*)((u8*)fn_801F75F8 + 0xAC),
    (void*)((u8*)fn_801F75F8 + 0xA4),
};

