#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

/*
 * .sdata2 constants continuing directly after sdata2_8047E390. Includes a
 * "THP" (movie/video codec) tag string and a handful of opaque 4byte/2byte
 * constants whose exact producing struct is not yet identified; values are
 * reproduced verbatim from the shipped binary.
 */
SDATA2 const u32 lbl_8047E490 = 0xFF00FF80;
SDATA2 const f32 lbl_8047E494 = 0.0f;
SDATA2 const f32 lbl_8047E498 = -1.0f;
SDATA2 const f32 lbl_8047E49C = 1.0f;
SDATA2 const f64 lbl_8047E4A0 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047E4A8 = 100.0f;
SDATA2 const u8 lbl_8047E4AC[4] = "THP";
SDATA2 const f64 lbl_8047E4B0 = 4.503599627370496e+15;
SDATA2 const f32 lbl_8047E4B8 = 1.4142135381698608f;
SDATA2 const f32 lbl_8047E4BC = 1.8477590084075928f;
SDATA2 const f32 lbl_8047E4C0 = 1.0823922157287598f;
SDATA2 const f32 lbl_8047E4C4 = -2.613126039505005f;
SDATA2 const f32 lbl_8047E4C8[2] = { 1024.0f, 0.0f };
SDATA2 const f32 lbl_8047E4D0 = 320.0f;
SDATA2 const f32 lbl_8047E4D4 = 240.0f;
SDATA2 const f32 lbl_8047E4D8 = 0.0f;
SDATA2 const f32 lbl_8047E4DC = 0.0010000000474974513f;
SDATA2 const f32 lbl_8047E4E0 = 640.0f;
SDATA2 const f32 lbl_8047E4E4 = 480.0f;
SDATA2 const f32 lbl_8047E4E8 = 1.0f;
SDATA2 const f32 lbl_8047E4EC = 255.0f;
SDATA2 const f32 lbl_8047E4F0 = 0.3333333432674408f;
#pragma push
#pragma force_active on
SDATA2 const u32 sdata2_padding_8047E4F4 = 0;
#pragma pop
SDATA2 const f64 lbl_8047E4F8 = 4.503599627370496e+15;
SDATA2 const f64 lbl_8047E500 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047E508[2] = { 1.0f, 0.0f };
SDATA2 const u32 lbl_8047E510 = 0x001F0020;
SDATA2 const u16 lbl_8047E514 = 0x0021;
SDATA2 const u32 lbl_8047E518 = 0x001F0020;
SDATA2 const u16 lbl_8047E51C = 0x0021;
#pragma push
#pragma force_active on
SDATA2 const u16 sdata2_padding_8047E51E = 0;
#pragma pop
SDATA2 const f32 lbl_8047E520[2] = { 0.5f, 0.0f };
SDATA2 const f32 lbl_8047E528 = 1.0f;
SDATA2 const f32 lbl_8047E52C = 0.5f;
SDATA2 const f32 lbl_8047E530[2] = { 0.0f, 0.0f };
