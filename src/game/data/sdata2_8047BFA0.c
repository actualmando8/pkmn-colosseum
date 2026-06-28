#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

typedef union Sdata2AlignedString2 {
    u8 text[2];
    f64 align;
} Sdata2AlignedString2;

/*
 * UI-core and menu .sdata2 constants. ui_core.c owns the first transition and
 * rotation constants; menu_middle/menu_common_ext/menu_tool own the message-id
 * tables, assert strings, and menu angle/color constants.
 */
SDATA2 const f32 lbl_8047BFA0 = 255.0f;
SDATA2 const f32 lbl_8047BFA4 = 0.100000001f;
SDATA2 const f32 lbl_8047BFA8 = 0.200000003f;
SDATA2 const f32 lbl_8047BFAC = 100.0f;
SDATA2 const f32 lbl_8047BFB0 = 0.300000012f;
SDATA2 const f32 lbl_8047BFB4 = 0.400000006f;
SDATA2 const f32 lbl_8047BFB8 = 0.600000024f;
SDATA2 const f32 lbl_8047BFBC = 640.0f;
SDATA2 const f32 lbl_8047BFC0 = -640.0f;
SDATA2 const f32 lbl_8047BFC4 = 0.699999988f;
SDATA2 const u32 lbl_8047BFC8 = 0x213A44F2;
SDATA2 const u32 lbl_8047BFCC = 0x11272BF2;
SDATA2 const f32 lbl_8047BFD0 = 1.0f;
SDATA2 const f32 lbl_8047BFD4 = 56.0f;
SDATA2 const f64 lbl_8047BFD8 = 4.503601774854144e+15;
SDATA2 const f64 lbl_8047BFE0 = 4.503599627370496e+15;
SDATA2 const f32 lbl_8047BFE8 = 0.0f;
SDATA2 const f32 lbl_8047BFEC = 0.800000012f;
SDATA2 const f64 lbl_8047BFF0 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047BFF8 = 0.959931076f;
SDATA2 const f32 lbl_8047BFFC = 2.18166161f;
SDATA2 const f32 lbl_8047C000 = 0.610865235f;
SDATA2 const f32 lbl_8047C004 = 2.53072739f;
SDATA2 const f32 lbl_8047C008 = 255.0f;
SDATA2 const f32 lbl_8047C00C = 2.0f;
SDATA2 const f32 lbl_8047C010 = 8.0f;
SDATA2 const f32 lbl_8047C014 = -8.0f;
SDATA2 const f32 lbl_8047C018 = 1.0f;
SDATA2 const f64 lbl_8047C020 = 4.503599627370496e+15;
SDATA2 const u16 lbl_8047C028[4] = { 0x2EFE, 0x2EFF, 0x2F00, 0 };
SDATA2 const Sdata2AlignedString2 lbl_8047C030 = { "0" };
SDATA2 const u16 lbl_8047C038[4] = { 4, 5, 6, 7 };
SDATA2 const Sdata2AlignedString2 lbl_8047C040 = { "0" };
SDATA2 const u32 lbl_8047C048[2] = { 0x00003D8E, 0x00003D90 };
SDATA2 const u16 lbl_8047C050[4] = { 0x0960, 0x0961, 0x095D, 0x095E };
SDATA2 const u16 lbl_8047C058[4] = { 0x02AA, 0x02A9, 0x02A8, 0x02A7 };
SDATA2 const f32 lbl_8047C060 = 1.0f;
SDATA2 const u8 lbl_8047C064[2] = "0";
SDATA2 const u8 lbl_8047C068[5] = "Lv%d";
SDATA2 const u8 lbl_8047C070[6] = "Lv???";
SDATA2 const f32 lbl_8047C078 = 0.0174532924f;
SDATA2 const f64 lbl_8047C080 = 6.2831854820251464844;
SDATA2 const f32 lbl_8047C088 = 0.0f;
SDATA2 const f32 lbl_8047C08C = 6.28318548f;
SDATA2 const u8 lbl_8047C090[7] = "handle";
SDATA2 const f32 lbl_8047C098 = 180.0f;
SDATA2 const f32 lbl_8047C09C = 40.0f;
