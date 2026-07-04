#include "dolphin/types.h"

#pragma section ".data"

extern u8 lbl_80313590[];
extern u8 lbl_803135E0[];
extern u8 lbl_80313608[];
extern void* jumptable_80313628[];
extern void* jumptable_80313684[];
extern void* jumptable_80313714[];
extern void* jumptable_80313770[];

extern u8 TRKDispatchMessage[];
extern u8 fn_800BD91C[];
extern u8 fn_800BE164[];

/* Auto-carved .data unit 0x80313590..0x803137DC (7 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 lbl_80313590[80] = {
    0xC0, 0x08, 0xF8, 0xAF, 0xC0, 0x08, 0xA8, 0x9F, 0xC0, 0x08, 0xAC, 0x8F,
    0xC0, 0x08, 0xFF, 0xF8, 0xC0, 0x08, 0xFF, 0xFA, 0xC0, 0x08, 0xF8, 0x0F,
    0xC0, 0x08, 0x08, 0x9F, 0xC0, 0x08, 0x0C, 0x8F, 0xC0, 0x08, 0xFF, 0xF8,
    0xC0, 0x08, 0xFF, 0xF0, 0xC1, 0x08, 0xF2, 0xF0, 0xC1, 0x08, 0xFF, 0xD0,
    0xC1, 0x08, 0xF2, 0xF0, 0xC1, 0x08, 0xFF, 0xC0, 0xC1, 0x08, 0xFF, 0xD0,
    0xC1, 0x08, 0xF0, 0x70, 0xC1, 0x08, 0xFF, 0x80, 0xC1, 0x08, 0xF0, 0x70,
    0xC1, 0x08, 0xFF, 0xC0, 0xC1, 0x08, 0xFF, 0x80,
};

u8 lbl_803135E0[40] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x00,
};

u8 lbl_80313608[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05,
};

void* jumptable_80313628[23] = {
    (void*)((u8*)fn_800BD91C + 0x564),
    (void*)((u8*)fn_800BD91C + 0x580),
    (void*)((u8*)fn_800BD91C + 0x59C),
    (void*)((u8*)fn_800BD91C + 0x5B8),
    (void*)((u8*)fn_800BD91C + 0x60C),
    (void*)((u8*)fn_800BD91C + 0x628),
    (void*)((u8*)fn_800BD91C + 0x644),
    (void*)((u8*)fn_800BD91C + 0x660),
    (void*)((u8*)fn_800BD91C + 0x5D4),
    (void*)((u8*)fn_800BD91C + 0x67C),
    (void*)((u8*)fn_800BD91C + 0x6AC),
    (void*)((u8*)fn_800BD91C + 0x6DC),
    (void*)((u8*)fn_800BD91C + 0x70C),
    (void*)((u8*)fn_800BD91C + 0x73C),
    (void*)((u8*)fn_800BD91C + 0x76C),
    (void*)((u8*)fn_800BD91C + 0x79C),
    (void*)((u8*)fn_800BD91C + 0x7CC),
    (void*)((u8*)fn_800BD91C + 0x7FC),
    (void*)((u8*)fn_800BD91C + 0x80C),
    (void*)((u8*)fn_800BD91C + 0x81C),
    (void*)((u8*)fn_800BD91C + 0x82C),
    (void*)((u8*)fn_800BD91C + 0x5F0),
    (void*)((u8*)fn_800BD91C + 0x838),
};

void* jumptable_80313684[36] = {
    (void*)((u8*)fn_800BD91C + 0x138),
    (void*)((u8*)fn_800BD91C + 0x158),
    (void*)((u8*)fn_800BD91C + 0x178),
    (void*)((u8*)fn_800BD91C + 0x198),
    (void*)((u8*)fn_800BD91C + 0x1B8),
    (void*)((u8*)fn_800BD91C + 0x1D8),
    (void*)((u8*)fn_800BD91C + 0x1F8),
    (void*)((u8*)fn_800BD91C + 0x218),
    (void*)((u8*)fn_800BD91C + 0x238),
    (void*)((u8*)fn_800BD91C + 0x258),
    (void*)((u8*)fn_800BD91C + 0x298),
    (void*)((u8*)fn_800BD91C + 0x2B8),
    (void*)((u8*)fn_800BD91C + 0x2D4),
    (void*)((u8*)fn_800BD91C + 0x2F0),
    (void*)((u8*)fn_800BD91C + 0x30C),
    (void*)((u8*)fn_800BD91C + 0x328),
    (void*)((u8*)fn_800BD91C + 0x344),
    (void*)((u8*)fn_800BD91C + 0x360),
    (void*)((u8*)fn_800BD91C + 0x37C),
    (void*)((u8*)fn_800BD91C + 0x398),
    (void*)((u8*)fn_800BD91C + 0x3B4),
    (void*)((u8*)fn_800BD91C + 0x3D0),
    (void*)((u8*)fn_800BD91C + 0x3EC),
    (void*)((u8*)fn_800BD91C + 0x408),
    (void*)((u8*)fn_800BD91C + 0x424),
    (void*)((u8*)fn_800BD91C + 0x440),
    (void*)((u8*)fn_800BD91C + 0x45C),
    (void*)((u8*)fn_800BD91C + 0x478),
    (void*)((u8*)fn_800BD91C + 0x494),
    (void*)((u8*)fn_800BD91C + 0x4B0),
    (void*)((u8*)fn_800BD91C + 0x4CC),
    (void*)((u8*)fn_800BD91C + 0x4E8),
    (void*)((u8*)fn_800BD91C + 0x504),
    (void*)((u8*)fn_800BD91C + 0x520),
    (void*)((u8*)fn_800BD91C + 0x278),
    (void*)((u8*)fn_800BD91C + 0x538),
};

void* jumptable_80313714[23] = {
    (void*)((u8*)fn_800BE164 + 0x128),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x134),
    (void*)((u8*)fn_800BE164 + 0x144),
    (void*)((u8*)fn_800BE164 + 0x158),
    (void*)((u8*)fn_800BE164 + 0x16C),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x188),
    (void*)((u8*)fn_800BE164 + 0x188),
    (void*)((u8*)fn_800BE164 + 0x188),
    (void*)((u8*)fn_800BE164 + 0x188),
    (void*)((u8*)fn_800BE164 + 0x180),
    (void*)((u8*)fn_800BE164 + 0x190),
};

void* jumptable_80313770[27] = {
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x60),
    (void*)((u8*)TRKDispatchMessage + 0x70),
    (void*)((u8*)TRKDispatchMessage + 0x80),
    (void*)((u8*)TRKDispatchMessage + 0xA0),
    (void*)((u8*)TRKDispatchMessage + 0xB0),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x90),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0xC0),
    (void*)((u8*)TRKDispatchMessage + 0xD0),
    (void*)((u8*)TRKDispatchMessage + 0xE0),
    (void*)((u8*)TRKDispatchMessage + 0xF0),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x13C),
    (void*)((u8*)TRKDispatchMessage + 0x130),
    (void*)((u8*)TRKDispatchMessage + 0x100),
    (void*)((u8*)TRKDispatchMessage + 0x110),
    (void*)((u8*)TRKDispatchMessage + 0x120),
};

