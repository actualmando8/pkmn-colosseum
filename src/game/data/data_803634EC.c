#include "dolphin/types.h"

#pragma section ".data"

extern u8 pad_05_803634EC_data[];
extern void* jumptable_803634F0[];
extern void* jumptable_80363558[];
extern u8 lbl_803635C0[];
extern u8 lbl_803635D8[];
extern u8 lbl_803635F0[];
extern u8 lbl_80363610[];

extern u8 heroGetStatus[];
extern u8 heroSetStatus[];

/* Auto-carved .data unit 0x803634EC..0x80363630 (7 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 pad_05_803634EC_data[4] = {
    0x00, 0x00, 0x00, 0x00,
};

void* jumptable_803634F0[26] = {
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x80),
    (void*)((u8*)heroSetStatus + 0x8C),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x148),
    (void*)((u8*)heroSetStatus + 0x98),
    (void*)((u8*)heroSetStatus + 0xA4),
    (void*)((u8*)heroSetStatus + 0xB0),
    (void*)((u8*)heroSetStatus + 0xBC),
    (void*)((u8*)heroSetStatus + 0xC8),
    (void*)((u8*)heroSetStatus + 0xD4),
    (void*)((u8*)heroSetStatus + 0xE0),
    (void*)((u8*)heroSetStatus + 0xEC),
    (void*)((u8*)heroSetStatus + 0xF8),
    (void*)((u8*)heroSetStatus + 0x104),
    (void*)((u8*)heroSetStatus + 0x110),
    (void*)((u8*)heroSetStatus + 0x11C),
    (void*)((u8*)heroSetStatus + 0x128),
    (void*)((u8*)heroSetStatus + 0x134),
    (void*)((u8*)heroSetStatus + 0x140),
};

void* jumptable_80363558[26] = {
    (void*)((u8*)heroGetStatus + 0x1A8),
    (void*)((u8*)heroGetStatus + 0x94),
    (void*)((u8*)heroGetStatus + 0x9C),
    (void*)((u8*)heroGetStatus + 0xA4),
    (void*)((u8*)heroGetStatus + 0xB0),
    (void*)((u8*)heroGetStatus + 0xBC),
    (void*)((u8*)heroGetStatus + 0xC8),
    (void*)((u8*)heroGetStatus + 0xD4),
    (void*)((u8*)heroGetStatus + 0xE0),
    (void*)((u8*)heroGetStatus + 0xEC),
    (void*)((u8*)heroGetStatus + 0xF8),
    (void*)((u8*)heroGetStatus + 0x104),
    (void*)((u8*)heroGetStatus + 0x110),
    (void*)((u8*)heroGetStatus + 0x118),
    (void*)((u8*)heroGetStatus + 0x120),
    (void*)((u8*)heroGetStatus + 0x128),
    (void*)((u8*)heroGetStatus + 0x134),
    (void*)((u8*)heroGetStatus + 0x140),
    (void*)((u8*)heroGetStatus + 0x14C),
    (void*)((u8*)heroGetStatus + 0x158),
    (void*)((u8*)heroGetStatus + 0x164),
    (void*)((u8*)heroGetStatus + 0x170),
    (void*)((u8*)heroGetStatus + 0x17C),
    (void*)((u8*)heroGetStatus + 0x188),
    (void*)((u8*)heroGetStatus + 0x190),
    (void*)((u8*)heroGetStatus + 0x19C),
};

u8 lbl_803635C0[24] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 lbl_803635D8[24] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x67, 0xFE, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0D, 0x68, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x69,
};

u8 lbl_803635F0[32] = {
    0x00, 0x30, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x35,
    0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 0x41, 0x00, 0x42,
    0x00, 0x43, 0x00, 0x44, 0x00, 0x45, 0x00, 0x46,
};

u8 lbl_80363610[32] = {
    0xFF, 0x10, 0xFF, 0x11, 0xFF, 0x12, 0xFF, 0x13, 0xFF, 0x14, 0xFF, 0x15,
    0xFF, 0x16, 0xFF, 0x17, 0xFF, 0x18, 0xFF, 0x19, 0xFF, 0x21, 0xFF, 0x22,
    0xFF, 0x23, 0xFF, 0x24, 0xFF, 0x25, 0xFF, 0x26,
};

