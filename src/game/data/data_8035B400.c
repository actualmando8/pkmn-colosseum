#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_8035B400[];
extern u8 lbl_8035B430[];
extern u8 lbl_8035B43C[];

extern u8 fn_801074D4[];

/* Auto-carved .data unit 0x8035B400..0x8035B448 (3 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_8035B400[12] = {
    (void*)((u8*)fn_801074D4 + 0x5C4),
    (void*)((u8*)fn_801074D4 + 0x5D8),
    (void*)((u8*)fn_801074D4 + 0x878),
    (void*)((u8*)fn_801074D4 + 0x5E4),
    (void*)((u8*)fn_801074D4 + 0x964),
    (void*)((u8*)fn_801074D4 + 0x964),
    (void*)((u8*)fn_801074D4 + 0x7E0),
    (void*)((u8*)fn_801074D4 + 0x744),
    (void*)((u8*)fn_801074D4 + 0x934),
    (void*)((u8*)fn_801074D4 + 0x5E4),
    (void*)((u8*)fn_801074D4 + 0x8D0),
    (void*)((u8*)fn_801074D4 + 0x8AC),
};

u8 lbl_8035B430[12] = {
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 lbl_8035B43C[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

