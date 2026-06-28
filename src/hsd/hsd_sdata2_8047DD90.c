#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

typedef union Sdata2AlignedString2 {
    u8 text[2];
    f64 align;
} Sdata2AlignedString2;

typedef union Sdata2AlignedString7 {
    u8 text[7];
    f64 align;
} Sdata2AlignedString7;

/*
 * Mixed HSD .sdata2 constants and assert strings used by pobj, shadow, tev,
 * and texp code. The aligned string wrappers preserve compiler-emitted zero
 * padding before later string labels.
 */
SDATA2 const u8 lbl_8047DD90[4] = "obj";
SDATA2 const f32 lbl_8047DD94 = 1.000000013351432e-10f;
SDATA2 const f64 lbl_8047DD98 = 0.5;
SDATA2 const f64 lbl_8047DDA0 = 3.0;
SDATA2 const f64 lbl_8047DDA8 = 0.0;
SDATA2 const f64 lbl_8047DDB0 = 1.000000013351432e-10;
SDATA2 const u8 lbl_8047DDB8[5] = "rect";
SDATA2 const f32 lbl_8047DDC0 = 0.0f;
SDATA2 const f32 lbl_8047DDC4 = -3.4028234663852886e+38f;
SDATA2 const f32 lbl_8047DDC8 = 3.4028234663852886e+38f;
SDATA2 const u8 lbl_8047DDCC[7] = "shadow";
SDATA2 const u8 lbl_8047DDD4[2] = "0";
SDATA2 const f64 lbl_8047DDD8 = 4.503599627370496e+15;
SDATA2 const u8 lbl_8047DDE0[5] = "lobj";
SDATA2 const f32 lbl_8047DDE8 = 1.2000000476837158f;
SDATA2 const f32 lbl_8047DDEC = -1.100000023841858f;
SDATA2 const f32 lbl_8047DDF0 = 0.5f;
SDATA2 const f32 lbl_8047DDF4 = -0.5f;
SDATA2 const f32 lbl_8047DDF8[2] = { 256.0f, 0.0f };
SDATA2 const f32 lbl_8047DE00 = 0.0f;
SDATA2 const f32 lbl_8047DE04 = 1.0f;
SDATA2 const f32 lbl_8047DE08 = 0.5f;
SDATA2 const f32 lbl_8047DE0C = 9.999999747378752e-06f;
SDATA2 const f64 lbl_8047DE10 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047DE18 = 0.125f;
SDATA2 const f32 lbl_8047DE1C = -0.0010000000474974513f;
SDATA2 const f64 lbl_8047DE20 = 0.5;
SDATA2 const f64 lbl_8047DE28 = 3.0;
SDATA2 const f64 lbl_8047DE30 = 0.0;
SDATA2 const f32 lbl_8047DE38 = 4.0f;
SDATA2 const f32 lbl_8047DE3C = 2.0f;
SDATA2 const f32 lbl_8047DE40 = 3.0f;
SDATA2 const f32 lbl_8047DE44 = 0.1666666716337204f;
SDATA2 const f32 lbl_8047DE48[2] = { 6.0f, 0.0f };
SDATA2 const f32 lbl_8047DE50 = 0.0f;
SDATA2 const f32 lbl_8047DE54 = 1.0f;
SDATA2 const f32 lbl_8047DE58[2] = { 255.0f, 0.0f };
SDATA2 const u8 lbl_8047DE60[6] = "tev.c";
SDATA2 const Sdata2AlignedString2 lbl_8047DE68 = { "0" };
SDATA2 const Sdata2AlignedString7 lbl_8047DE70 = { "texp.c" };
SDATA2 const u8 lbl_8047DE78[8] = "tevdesc";
SDATA2 const f32 lbl_8047DE80 = 255.0f;
SDATA2 const f64 lbl_8047DE88 = 255.0;
