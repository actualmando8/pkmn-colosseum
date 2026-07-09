/**
 * @file sdk_range_800A2D38.c
 * @brief dolphin-sdk code, 0x800A2D38 - 0x800A3EB0 (37 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). Functions not present here remain asm-only until
 * matched; the range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef f32 Mtx44[4][4];

extern const f32 lbl_8047C2A8;
extern const f32 lbl_8047C2AC;
extern const f32 lbl_8047C2B0;
extern const f32 lbl_8047C2B4;
extern const f32 lbl_8047C2B8;
extern const f32 lbl_8047C2BC;

extern f32 tanf(f32 x);

void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 near, f32 far)
{
    f32 cot;
    f32 tmp;

    fovY = lbl_8047C2B8 * fovY;
    fovY = lbl_8047C2BC * fovY;
    cot = lbl_8047C2A8 / tanf(fovY);
    tmp = lbl_8047C2A8 / (far - near);

    m[0][0] = cot / aspect;
    m[0][1] = lbl_8047C2B0;
    m[0][2] = lbl_8047C2B0;
    m[0][3] = lbl_8047C2B0;

    m[1][0] = lbl_8047C2B0;
    m[1][1] = cot;
    m[1][2] = lbl_8047C2B0;
    m[1][3] = lbl_8047C2B0;

    m[2][0] = lbl_8047C2B0;
    m[2][1] = lbl_8047C2B0;
    m[2][2] = -near * tmp;
    m[2][3] = -(far * near) * tmp;

    m[3][0] = lbl_8047C2B0;
    m[3][1] = lbl_8047C2B0;
    m[3][2] = lbl_8047C2B4;
    m[3][3] = lbl_8047C2B0;
}

void C_MTXOrtho(Mtx44 m, f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far)
{
    f32 tmp;

    tmp = lbl_8047C2A8 / (right - left);
    m[0][0] = lbl_8047C2AC * tmp;
    m[0][1] = lbl_8047C2B0;
    m[0][2] = lbl_8047C2B0;
    m[0][3] = -(right + left) * tmp;

    tmp = lbl_8047C2A8 / (top - bottom);
    m[1][0] = lbl_8047C2B0;
    m[1][1] = lbl_8047C2AC * tmp;
    m[1][2] = lbl_8047C2B0;
    m[1][3] = -(top + bottom) * tmp;

    tmp = lbl_8047C2A8 / (far - near);
    m[2][0] = lbl_8047C2B0;
    m[2][1] = lbl_8047C2B0;
    m[2][2] = lbl_8047C2B4 * tmp;
    m[2][3] = -far * tmp;

    m[3][0] = lbl_8047C2B0;
    m[3][1] = lbl_8047C2B0;
    m[3][2] = lbl_8047C2B0;
    m[3][3] = lbl_8047C2A8;
}
