#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

/*
 * .sdata2 constants continuing directly after sdata2_8047E490, ending at
 * the existing game/colosseum_battle_sdata2 slice. Contains several small
 * u32 id/index tables (values in the 0x7500-0x76B5 range, likely message or
 * script ids), a couple of small byte/word constants, and a run of float
 * constants used by some rendering/animation path. Values are reproduced
 * verbatim from the shipped binary; the exact producing structs are not yet
 * identified.
 */
SDATA2 const u32 lbl_8047E538[6] = {
    0x00007650, 0x00007656, 0x00007596, 0x00007597, 0x00007651, 0x00007657,
};
SDATA2 const u32 lbl_8047E550[2] = { 0x0000757F, 0x00007580 };
SDATA2 const u32 lbl_8047E558[20] = {
    0x000075A1, 0x000075A2, 0x000075A4, 0x000075A5, 0x0000759D, 0x0000759F,
    0x00007659, 0x00007577, 0x00007591, 0x00007647, 0x00007593, 0x00007647,
    0x000075AE, 0x00007647, 0x000075AF, 0x0000765D, 0x00007552, 0x00007553,
    0x00007562, 0x00007563,
};
SDATA2 const u32 lbl_8047E5A8[12] = {
    0x00007558, 0x00007559, 0x0000755C, 0x0000755D, 0x00007560, 0x00007561,
    0x0000753E, 0x000075AD, 0x00007557, 0x0000761C, 0x000075F1, 0x000075F2,
};
SDATA2 const u32 lbl_8047E5D8[8] = {
    0x000075D1, 0x000075D3, 0x000076B5, 0x000076B6,
    0x0000760C, 0x0000761A, 0x0000754D, 0x0000754E,
};
SDATA2 const u32 lbl_8047E5F8[2] = { 0xFFFF7FFF, 0x3FFF1FFF };
SDATA2 const u16 lbl_8047E600 = 0x0102;
SDATA2 const u8 lbl_8047E602 = 0x05;
SDATA2 const u32 lbl_8047E604 = 0x01020304;
SDATA2 const u8 lbl_8047E608 = 0x05;
#pragma push
#pragma force_active on
SDATA2 const u8 sdata2_padding_8047E609[7] = { 0, 0, 0, 0, 0, 0, 0 };
#pragma pop
SDATA2 const f64 lbl_8047E610 = 4.503599627370496e+15;
SDATA2 const f32 lbl_8047E618 = 1.0f;
SDATA2 const f32 lbl_8047E61C = 0.0f;
SDATA2 const f64 lbl_8047E620 = 4.503599627370496e+15;
SDATA2 const u32 lbl_8047E628 = 0x005C0105;
SDATA2 const u32 lbl_8047E62C = 0x004D008B;
SDATA2 const f32 lbl_8047E630 = 0.5f;
SDATA2 const f32 lbl_8047E634 = 0.8f;
SDATA2 const f32 lbl_8047E638 = 0.1f;
SDATA2 const f32 lbl_8047E63C = 0.3f;
SDATA2 const f32 lbl_8047E640[2] = { 0.25f, 0.0f };
SDATA2 const f32 lbl_8047E648 = 1.0f;
SDATA2 const f32 lbl_8047E64C = 0.5f;
SDATA2 const f32 lbl_8047E650[2] = { 0.25f, 0.0f };
