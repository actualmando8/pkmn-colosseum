#include "dolphin/types.h"

#pragma section ".rodata"
#define RODATA __declspec(section ".rodata")

typedef struct PokemonSummaryLayoutEntry {
    s32 messageId;
    s32 x;
    s32 y;
    s32 field_0C;
    s32 field_10;
    u32 flags;
} PokemonSummaryLayoutEntry;

/*
 * Pokemon summary/menu .rodata tables referenced from scene_init.c,
 * colosseum_ui.c, and menu/menu_pokemon.c.
 */
RODATA const PokemonSummaryLayoutEntry lbl_80267060[8] = {
    { 0x36D, -158, 179, 42, 179, 0 },
    { 0x36E, -168, 234, 52, 234, 0 },
    { 0x36F, -178, 289, 62, 289, 0 },
    { 0x370, -188, 344, 72, 344, 0 },
    { 0x373, -142, 186, 58, 186, 0x01000000u },
    { 0x374, -152, 241, 68, 241, 0x01000000u },
    { 0x375, -162, 296, 78, 296, 0x01000000u },
    { 0x376, -172, 351, 88, 351, 0x01000000u },
};

RODATA const u32 lbl_80267120[4] = { 0, 1, 2, 3 };
RODATA const u32 lbl_80267130[4] = { 0x395, 0x396, 0x397, 0 };
RODATA const u32 lbl_80267140[4] = { 0x39A, 0x39C, 0x39B, 0x39D };

RODATA const f32 lbl_80267150[12] = {
    40.0f, 0.0f, 0.0f, 40.0f,
    200.0f, 200.0f, 40.0f, 0.0f,
    0.0f, 0.0f, 100.0f, 100.0f,
};

RODATA const f32 lbl_80267180[4] = { 255.0f, 255.0f, 255.0f, 0.0f };
RODATA const f32 lbl_80267190[3] = { 14.0f, -12.0f, 10.0f };

RODATA const u16 lbl_8026719C[12] = {
    150, 145, 140, 206, 201, 196,
    262, 257, 252, 318, 313, 308,
};

RODATA const u16 lbl_802671B4[14] = {
    150, 145, 140, 318, 313, 308, 206,
    201, 196, 262, 257, 252, 0, 0,
};

RODATA const u32 lbl_802671D0[12] = {
    0x462, 0x463, 0x464, 0x465, 0x466, 0x467,
    0x468, 0x469, 0x46A, 0x46B, 0x471, 0x472,
};

RODATA const u32 lbl_80267200[10] = {
    0x47C, 0x47B, 0x47A, 0x479, 0x478,
    0x477, 0x476, 0x475, 0x474, 0x473,
};

RODATA const u32 lbl_80267228[10] = {
    0x47D, 0x47E, 0x47F, 0x480, 0x481,
    0x482, 0x483, 0x484, 0x485, 0x486,
};
