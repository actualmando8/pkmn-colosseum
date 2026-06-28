#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

/*
 * Mixed GBA and Dolphin .sdata2 constants. GBA strings and constants are
 * referenced from gba_conv/gba_comm/gba_misc/late_game; the tail constants are
 * referenced by target relocations in OSSram and the 0x800A2D38..0x800A3744
 * Dolphin math/DVD area.
 */
SDATA2 const u8 lbl_8047C1A0[5] = "pWin";
SDATA2 const f32 lbl_8047C1A8 = 0.0f;
SDATA2 const f32 lbl_8047C1AC = 1.0f;
SDATA2 const f64 lbl_8047C1B0 = 4.503601774854144e+15;
SDATA2 const f64 lbl_8047C1B8 = 4.503599627370496e+15;
SDATA2 const u32 lbl_8047C1C0 = 0x00000001;
SDATA2 const u32 lbl_8047C1C4 = 0x00020003;
SDATA2 const f32 lbl_8047C1C8 = 0.5f;
SDATA2 const f32 lbl_8047C1CC = 0.0f;
SDATA2 const f32 lbl_8047C1D0 = 0.833333313f;
SDATA2 const f32 lbl_8047C1D4 = 0.0f;
SDATA2 const f32 lbl_8047C1D8 = 1.0f;
SDATA2 const f32 lbl_8047C1DC = 83.3333282f;
SDATA2 const f32 lbl_8047C1E0[2] = { 41.6666641f, 0.0f };
SDATA2 const u8 lbl_8047C1E8[7] = "handle";
SDATA2 const f32 lbl_8047C1F0 = 5.0f;
SDATA2 const f64 lbl_8047C1F8 = 4.503599627370496e+15;
SDATA2 const u32 lbl_8047C200 = 0x000078FF;
SDATA2 const u32 lbl_8047C204 = 0x780000FF;
SDATA2 const f32 lbl_8047C208 = 1.0f;
SDATA2 const f32 lbl_8047C20C = 0.800000012f;
SDATA2 const f32 lbl_8047C210 = 0.600000024f;
SDATA2 const f32 lbl_8047C214 = 0.400000006f;
SDATA2 const f32 lbl_8047C218 = 0.200000003f;
SDATA2 const f32 lbl_8047C21C = 5.0f;
SDATA2 const f64 lbl_8047C220 = 4.503599627370496e+15;
SDATA2 const f64 lbl_8047C228 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047C230 = 0.0f;
SDATA2 const f32 lbl_8047C234 = 0.25f;
SDATA2 const f32 lbl_8047C238[2] = { 0.5f, 0.0f };
SDATA2 const f32 lbl_8047C240 = 0.5f;
SDATA2 const f32 lbl_8047C244 = 16.0f;
SDATA2 const f32 lbl_8047C248 = 0.0979999974f;
SDATA2 const f32 lbl_8047C24C = 0.256999999f;
SDATA2 const f32 lbl_8047C250 = 0.504000008f;
SDATA2 const f32 lbl_8047C254 = 128.0f;
SDATA2 const f32 lbl_8047C258 = 0.43900001f;
SDATA2 const f32 lbl_8047C25C = -0.148000002f;
SDATA2 const f32 lbl_8047C260 = 0.291000009f;
SDATA2 const f32 lbl_8047C264 = 0.368000001f;
SDATA2 const f32 lbl_8047C268 = 0.0710000023f;
SDATA2 const f32 lbl_8047C26C = 235.0f;
SDATA2 const f32 lbl_8047C270 = 240.0f;
SDATA2 const f64 lbl_8047C278 = 4.503599627370496e+15;
SDATA2 const u32 lbl_8047C280 = 0x2ABE003D;
SDATA2 const u32 lbl_8047C284 = 0x003D003D;
SDATA2 const f32 lbl_8047C288 = 1.0f;
SDATA2 const f32 lbl_8047C28C = 0.0f;
SDATA2 const f32 lbl_8047C290 = 0.5f;
SDATA2 const f32 lbl_8047C294 = 3.0f;
SDATA2 const f32 lbl_8047C298 = 2.0f;
SDATA2 const f32 lbl_8047C29C = -1.0f;
