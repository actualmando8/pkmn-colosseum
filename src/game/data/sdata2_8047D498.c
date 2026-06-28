#include "dolphin/types.h"

#pragma section ".sdata2"
#define SDATA2 __declspec(section ".sdata2")

typedef void (*Sdata2FuncPtr)(void);

extern void fn_80167B70(void);
extern void fn_80167BB0(void);

/*
 * Mixed audio/DVD .sdata2 constants. Target relocations tie the run to JAudio
 * parameter helpers, SAL vector/matrix helpers, people-field volume scaling,
 * reverb/sound listener setup, GSDVD version/assert strings, and the adjacent
 * particle script/display constants.
 */
SDATA2 const f32 lbl_8047D498 = 0.016666668f;
SDATA2 const f32 lbl_8047D49C = 64.0f;
SDATA2 const f32 lbl_8047D4A0 = 8192.0f;
SDATA2 const f32 lbl_8047D4A4 = 0.0078740157f;
SDATA2 const f32 lbl_8047D4A8 = 0.08f;
SDATA2 const f32 lbl_8047D4AC = 0.15f;
SDATA2 const f32 lbl_8047D4B0[2] = { 0.3f, 0.0f };
SDATA2 const f32 lbl_8047D4B8 = 0.0f;
SDATA2 const f64 lbl_8047D4C0 = 0.5;
SDATA2 const f64 lbl_8047D4C8 = 3.0;
SDATA2 const f32 lbl_8047D4D0[2] = { 1.0f, 0.0f };
SDATA2 const f32 lbl_8047D4D8 = 1.0f;
SDATA2 const f32 lbl_8047D4DC = 32767.0f;
SDATA2 const f32 lbl_8047D4E0 = 4096.0f;
SDATA2 const f64 lbl_8047D4E8 = 4.503599627370496e+15;
SDATA2 const f32 lbl_8047D4F0 = 0.0f;
SDATA2 const f32 lbl_8047D4F4 = 1.0f;
SDATA2 const f32 lbl_8047D4F8 = 0.01f;
SDATA2 const f32 lbl_8047D4FC = 10.0f;
SDATA2 const f32 lbl_8047D500 = 0.1f;
SDATA2 const f32 lbl_8047D504 = 32000.0f;
SDATA2 const f64 lbl_8047D508 = 10.0;
SDATA2 const f32 lbl_8047D510 = 0.05f;
SDATA2 const f32 lbl_8047D514 = 0.8f;
SDATA2 const f64 lbl_8047D518 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047D520 = 100.0f;
SDATA2 const f64 lbl_8047D528 = 4.503601774854144e+15;
SDATA2 const f32 lbl_8047D530 = 0.6f;
SDATA2 const f32 lbl_8047D534 = 0.3f;
SDATA2 const f32 lbl_8047D538[2] = { 0.5f, 0.0f };
SDATA2 const f32 lbl_8047D540 = 0.0f;
SDATA2 const f32 lbl_8047D544 = 1000.0f;
SDATA2 const f64 lbl_8047D548 = 4.503599627370496e+15;
SDATA2 const f64 lbl_8047D550 = 4.503601774854144e+15;
SDATA2 Sdata2FuncPtr const lbl_8047D558 = fn_80167BB0;
SDATA2 Sdata2FuncPtr const lbl_8047D55C = fn_80167B70;
SDATA2 const f32 lbl_8047D560 = 0.0f;
SDATA2 const f32 lbl_8047D564 = 10000.0f;
SDATA2 const f32 lbl_8047D568 = -1.0f;
SDATA2 const f32 lbl_8047D56C = 1.0f;
SDATA2 const f32 lbl_8047D570 = 250.0f;
SDATA2 const f32 lbl_8047D574 = 1000.0f;
SDATA2 const u8 lbl_8047D578[5] = "GC6J";
SDATA2 const u8 lbl_8047D580[3] = "01";
SDATA2 const u8 lbl_8047D584[8] = "GSdvd.c";
SDATA2 const u8 lbl_8047D58C[6] = "FALSE";
SDATA2 const u8 lbl_8047D594[12] = "GC6E";
SDATA2 const f32 lbl_8047D5A0 = 0.0f;
SDATA2 const f32 lbl_8047D5A4 = 640.0f;
SDATA2 const f32 lbl_8047D5A8[2] = { 480.0f, 0.0f };
SDATA2 const f32 lbl_8047D5B0 = 3.0f;
SDATA2 const f32 lbl_8047D5B4 = 0.0f;
SDATA2 const f32 lbl_8047D5B8[2] = { 1.0f, 0.0f };
SDATA2 const f32 lbl_8047D5C0 = 0.0f;
SDATA2 const f32 lbl_8047D5C4 = 1.0f;
SDATA2 const f32 lbl_8047D5C8 = 0.0f;
SDATA2 const f32 lbl_8047D5CC = 2.0f;
SDATA2 const f32 lbl_8047D5D0 = 1.0f;
SDATA2 const u8 lbl_8047D5D4[2] = "0";
SDATA2 const f32 lbl_8047D5D8 = 255.0f;
SDATA2 const f32 lbl_8047D5DC = 6.0f;
SDATA2 const f64 lbl_8047D5E0 = 42.5;
SDATA2 const f32 lbl_8047D5E8 = -1.0f;
SDATA2 const f32 lbl_8047D5EC = 1.5707963705062866f;
SDATA2 const f32 lbl_8047D5F0 = -1.5707963705062866f;
SDATA2 const f64 lbl_8047D5F8 = 0.01;
SDATA2 const f64 lbl_8047D600 = 0.5;
SDATA2 const f64 lbl_8047D608 = 3.0;
SDATA2 const f64 lbl_8047D610 = 0.0;
SDATA2 const f32 lbl_8047D618 = 0.5f;
SDATA2 const f64 lbl_8047D620 = 4.503599627370496e+15;
SDATA2 const u8 lbl_8047D628[7] = "lastPP";
SDATA2 const f32 lbl_8047D630 = 0.0f;
SDATA2 const f32 lbl_8047D634 = 3.0f;
SDATA2 const f32 lbl_8047D638 = 1.0f;
SDATA2 const f32 lbl_8047D63C = 100.0f;
SDATA2 const f32 lbl_8047D640 = 2.0f;
SDATA2 const f32 lbl_8047D644 = 1.0e-10f;
SDATA2 const f32 lbl_8047D648 = 0.5f;
SDATA2 const f64 lbl_8047D650 = 0.5;
SDATA2 const f32 lbl_8047D658 = -1.0f;
SDATA2 const f64 lbl_8047D660 = 4.503599627370496e+15;
SDATA2 const f64 lbl_8047D668 = 4.503601774854144e+15;
SDATA2 const u8 lbl_8047D670[7] = "jobj.h";
SDATA2 const u8 lbl_8047D678[5] = "jobj";
SDATA2 const f64 lbl_8047D680 = 3.0;
SDATA2 const f64 lbl_8047D688 = 0.0;
