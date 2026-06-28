#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

/*
 * Mixed early game .sdata2 constants. main.c owns the error strings and initial
 * volume/conversion constants; gs_task, gs_party_access, gs_npc_interact,
 * gs_event_exec, and gs_pokemon_summary reference the remaining UI/camera
 * constants in this run.
 */
SDATA2 const f32 lbl_8047B6A0 = 0.0f;
SDATA2 const f32 lbl_8047B6A4 = 359940.0f;
SDATA2 const f64 lbl_8047B6A8 = 4.503601774854144e+15;
SDATA2 const f64 lbl_8047B6B0 = 4.503599627370496e+15;
SDATA2 const u8 lbl_8047B6B8[8] = "error.c";
SDATA2 const u8 lbl_8047B6C0[5] = "%s:\n";
SDATA2 const u8 lbl_8047B6C8[3] = "%d";
#pragma push
#pragma force_active on
SDATA2 const u32 sdata2_padding_8047B6CC = 0;
#pragma pop
SDATA2 const f32 lbl_8047B6D0 = 25500.0f;
SDATA2 const f64 lbl_8047B6D8 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047B6E0[2] = { 0.5f, 0.0f };
SDATA2 const f32 lbl_8047B6E8[2] = { 0.5f, 0.0f };
SDATA2 const f32 lbl_8047B6F0 = 20.0f;
SDATA2 const f64 lbl_8047B6F8 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047B700 = 0.0f;
SDATA2 const f32 lbl_8047B704 = 0.785398185f;
SDATA2 const f32 lbl_8047B708 = 2.3561945f;
SDATA2 const f64 lbl_8047B710 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047B718 = 0.0f;
SDATA2 const f32 lbl_8047B71C = 100.0f;
SDATA2 const f32 lbl_8047B720 = 1200.0f;
SDATA2 const f32 lbl_8047B724 = 20.0f;
SDATA2 const f32 lbl_8047B728 = 50.0f;
SDATA2 const f32 lbl_8047B72C = 1.0f;
SDATA2 const f64 lbl_8047B730 = 4.503599627370496e+15;
SDATA2 const f64 lbl_8047B738 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047B740 = 255.0f;
SDATA2 const f32 lbl_8047B744 = 1.0f;
SDATA2 const f32 lbl_8047B748 = 0.0f;
SDATA2 const f32 lbl_8047B74C = 0.0333333351f;
SDATA2 const f32 lbl_8047B750 = 338.0f;
SDATA2 const f32 lbl_8047B754 = 0.0506708547f;
SDATA2 const f32 lbl_8047B758 = 6.28318548f;
SDATA2 const f32 lbl_8047B75C = 3.14159274f;
SDATA2 const f32 lbl_8047B760 = -3.14159274f;
SDATA2 const f64 lbl_8047B768 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047B770 = 0.5f;
SDATA2 const f32 lbl_8047B774 = 2.0f;
SDATA2 const f32 lbl_8047B778 = 1.57079637f;
SDATA2 const f32 lbl_8047B77C = 45.0f;
SDATA2 const f32 lbl_8047B780 = 0.25f;
SDATA2 const f32 lbl_8047B784 = 0.75f;
SDATA2 const f32 lbl_8047B788 = -31.0f;
SDATA2 const f32 lbl_8047B78C = 31.0f;
SDATA2 const f32 lbl_8047B790 = -338.0f;
SDATA2 const f32 lbl_8047B794 = 22.0f;
SDATA2 const f32 lbl_8047B798 = 7.0f;
SDATA2 const f32 lbl_8047B79C = 0.00800000038f;
