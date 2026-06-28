#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

typedef union Sdata2AlignedString2 {
    u8 text[2];
    f64 align;
} Sdata2AlignedString2;

/*
 * Mixed HSD TExp/TObj/CObj/util/video/AObj .sdata2 constants and assert
 * strings. Source references and symbolmap strings tie the range to HSD code;
 * aligned string wrappers preserve compiler-emitted padding before later labels.
 */
SDATA2 const u8 lbl_8047DE90[5] = "texp";
SDATA2 const u8 lbl_8047DE98[5] = "desc";
SDATA2 const Sdata2AlignedString2 lbl_8047DEA0 = { "0" };
SDATA2 const u8 lbl_8047DEA8[8] = "l < num";
SDATA2 const u8 lbl_8047DEB0[7] = "tobj.c";
SDATA2 const u8 lbl_8047DEB8[6] = "idesc";
SDATA2 const u8 lbl_8047DEC0[4] = "tev";
SDATA2 const u8 lbl_8047DEC4[5] = "tlut";
SDATA2 const u8 lbl_8047DECC[4] = "new";
SDATA2 const u8 lbl_8047DED0[2] = "0";
SDATA2 const f32 lbl_8047DED4 = 0.0f;
SDATA2 const f32 lbl_8047DED8 = 0.5f;
SDATA2 const f32 lbl_8047DEDC = -0.5f;
SDATA2 const f32 lbl_8047DEE0 = 0.0f;
SDATA2 const f32 lbl_8047DEE4 = 1.0f;
SDATA2 const u8 lbl_8047DEE8[5] = "cobj";
SDATA2 const f32 lbl_8047DEF0 = -1.0f;
SDATA2 const f64 lbl_8047DEF8 = -0.5;
SDATA2 const f32 lbl_8047DF00 = 1.000000013351432e-10f;
SDATA2 const f64 lbl_8047DF08 = 4.503599627370496e+15;
SDATA2 const u8 lbl_8047DF10[5] = "tobj";
SDATA2 const f32 lbl_8047DF18[2] = { 255.0f, 0.0f };
SDATA2 const u8 lbl_8047DF20[7] = "util.c";
SDATA2 const Sdata2AlignedString2 lbl_8047DF28 = { "0" };
SDATA2 const u8 lbl_8047DF30[8] = "video.c";
SDATA2 const f32 lbl_8047DF38[2] = { 1.0f, 0.0f };
SDATA2 const u8 lbl_8047DF40[7] = "aobj.c";
SDATA2 const u8 lbl_8047DF48[4] = "obj";
SDATA2 const u8 lbl_8047DF4C[4] = "new";
SDATA2 const f32 lbl_8047DF50 = 1.0f;
SDATA2 const f32 lbl_8047DF54 = 0.0f;
SDATA2 const f32 lbl_8047DF58 = 0.5f;
SDATA2 const f32 lbl_8047DF5C = 1.1000000238418579f;
SDATA2 const f32 lbl_8047DF60 = 1.2000000476837158f;
SDATA2 const f32 lbl_8047DF64 = 1.3999999761581421f;
SDATA2 const f32 lbl_8047DF68 = 1.0f;
SDATA2 const f32 lbl_8047DF6C = 0.875f;
SDATA2 const f32 lbl_8047DF70 = 1.7999999523162842f;
SDATA2 const f32 lbl_8047DF74 = 2.75f;
SDATA2 const f32 lbl_8047DF78 = 65.0f;
SDATA2 const f32 lbl_8047DF7C = 40.0f;
SDATA2 const f32 lbl_8047DF80 = -30.0f;
SDATA2 const f32 lbl_8047DF84 = -8.0f;
SDATA2 const f32 lbl_8047DF88 = 30.0f;
SDATA2 const f32 lbl_8047DF8C = 1.5707963705062866f;
