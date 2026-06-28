#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

typedef union Sdata2AlignedString3 {
    u8 text[3];
    f64 align;
} Sdata2AlignedString3;

typedef union Sdata2AlignedString7 {
    u8 text[7];
    f64 align;
} Sdata2AlignedString7;

/*
 * Mixed menu, Card-E, save, and GBA .sdata2 constants. The D.D.D0 byte string
 * and following u16 table are consumed as halfword lookup data in menu_tool2.c;
 * the aligned strings are assert labels referenced from save and GBA code.
 */
SDATA2 const f32 lbl_8047C0A0 = 255.0f;
SDATA2 const f32 lbl_8047C0A4 = 0.125663713f;
SDATA2 const f32 lbl_8047C0A8 = 0.0f;
SDATA2 const f32 lbl_8047C0AC = 1.0f;
SDATA2 const f64 lbl_8047C0B0 = 4.503601774854144e+15;
SDATA2 const f64 lbl_8047C0B8 = 4.503599627370496e+15;
SDATA2 const f32 lbl_8047C0C0 = 0.833333313f;
SDATA2 const f32 lbl_8047C0C4 = 0.5f;
SDATA2 const f32 lbl_8047C0C8[2] = { 1.0f, 0.0f };
SDATA2 const u8 lbl_8047C0D0[7] = "D.D.D0";
SDATA2 const u16 lbl_8047C0D8[4] = { 0x442F, 0x442C, 0x442D, 0x4426 };
SDATA2 const f32 lbl_8047C0E0 = 0.0f;
SDATA2 const f32 lbl_8047C0E4 = 1.0f;
SDATA2 const u8 lbl_8047C0E8[7] = "celebi";
SDATA2 const f64 lbl_8047C0F0 = 4.503601774854144e+15;
SDATA2 const f64 lbl_8047C0F8 = 4.503599627370496e+15;
SDATA2 const f32 lbl_8047C100 = 0.5f;
SDATA2 const f32 lbl_8047C104 = 0.300000012f;
SDATA2 const f32 lbl_8047C108 = 0.300000012f;
SDATA2 const u8 lbl_8047C10C[8] = "pikachu";
SDATA2 const f32 lbl_8047C114 = 0.0f;
SDATA2 const f64 lbl_8047C118 = 4.503601774854144e+15;
SDATA2 const f64 lbl_8047C120 = 4.503599627370496e+15;
SDATA2 const f32 lbl_8047C128[2] = { 0.100000001f, 0.0f };
SDATA2 const u32 lbl_8047C130[2] = { 0xFFFFFFFFu, 0xFFFFFF00u };
SDATA2 const u32 lbl_8047C138[2] = { 0xFFFFFF00u, 0xFFFFFFFFu };
SDATA2 const u8 lbl_8047C140[7] = "handle";
SDATA2 const f32 lbl_8047C148 = 1.0f;
SDATA2 const f32 lbl_8047C14C = 0.0625f;
SDATA2 const f32 lbl_8047C150 = 0.0f;
SDATA2 const f32 lbl_8047C154 = 48.0f;
SDATA2 const f32 lbl_8047C158 = 32.0f;
SDATA2 const f32 lbl_8047C15C = 10.0f;
SDATA2 const f32 lbl_8047C160 = 0.03125f;
SDATA2 const f64 lbl_8047C168 = 4.503601774854144e+15;
SDATA2 const f64 lbl_8047C170 = 4.503599627370496e+15;
SDATA2 const u8 lbl_8047C178[2] = "0";
SDATA2 const Sdata2AlignedString7 lbl_8047C180 = { "series" };
SDATA2 const Sdata2AlignedString3 lbl_8047C188 = { "lv" };
SDATA2 const u16 lbl_8047C190[4] = { 0x01A8, 0x01B1, 0x01B1, 0x01A8 };
SDATA2 const u8 lbl_8047C198[7] = "handle";
