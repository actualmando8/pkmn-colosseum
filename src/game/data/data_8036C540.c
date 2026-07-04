#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_8036C540[];

extern u8 fn_8018F4C8[];

/* Auto-carved .data unit 0x8036C540..0x8036C564 (1 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_8036C540[9] = {
    (void*)((u8*)fn_8018F4C8 + 0xE8),
    (void*)((u8*)fn_8018F4C8 + 0x2C),
    (void*)((u8*)fn_8018F4C8 + 0x44),
    (void*)((u8*)fn_8018F4C8 + 0x5C),
    (void*)((u8*)fn_8018F4C8 + 0x74),
    (void*)((u8*)fn_8018F4C8 + 0x8C),
    (void*)((u8*)fn_8018F4C8 + 0xA4),
    (void*)((u8*)fn_8018F4C8 + 0xBC),
    (void*)((u8*)fn_8018F4C8 + 0xD4),
};

