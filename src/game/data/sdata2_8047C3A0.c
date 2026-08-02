#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

/*
 * Mixed Dolphin SDK and CRT .sdata2 constants. Target relocations tie the
 * first run to GXProject/fog/viewport code, then to stdio, wcstombs,
 * float2str, and fdlibm-style acos constants.
 */
SDATA2 const f32 lbl_8047C3A0 = 2.0f;
SDATA2 const f64 lbl_8047C3A8 = 0.5;
SDATA2 const f32 lbl_8047C3B0 = 8388638.0f;
SDATA2 const f64 lbl_8047C3B8 = 4503601774854144.0;
SDATA2 const f64 lbl_8047C3C0 = 0.0;
SDATA2 const f64 lbl_8047C3C8 = 3.0;
SDATA2 const f32 lbl_8047C3D0 = 256.0f;
SDATA2 const f64 lbl_8047C3D8 = 4503599627370496.0;
SDATA2 const f32 lbl_8047C3E0 = 0.0f;
SDATA2 const f32 lbl_8047C3E4 = 1.0f;
SDATA2 const f32 lbl_8047C3E8 = 0.5f;
SDATA2 const f64 lbl_8047C3F0 = 4503599627370496.0;
SDATA2 const f32 lbl_8047C3F8 = 342.0f;
SDATA2 const f32 lbl_8047C3FC = 16777215.0f;
SDATA2 const f64 lbl_8047C400 = 0.0;
SDATA2 const u32 lbl_8047C408 = 0x0000C0E0;
