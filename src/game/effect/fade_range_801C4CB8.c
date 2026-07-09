/**
 * @file fade_range_801C4CB8.c
 * @brief fade effect system, 0x801C4CB8 - 0x801C766C.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) -- mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 *
 * Boundary bug fix (battle_grid.c 5-way split, 2026-07-07): fn_801C4CB8
 * (0x704 bytes) was physically sitting in the old monolithic
 * game/battle/battle_grid.c despite being outside that file's own
 * declared splits.txt range (which ended at 0x801C4CB8, exclusive) --
 * i.e. it always belonged to this unit's range, just misplaced in
 * source. Relocated here so this unit scores real progress instead of
 * 0%. Registered by game/effect/fade_effect.c's
 * fadeEffectHookFunction_trainer_Init hook stub.
 */
#include "dolphin/types.h"

typedef struct GSvec {
    f32 x;
    f32 y;
    f32 z;
} GSvec;

typedef struct GSvec2 {
    f32 x;
    f32 y;
} GSvec2;

typedef struct FadeCameraWork {
    void* tex0;
    void* tex1;
    u16 frame;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    f32 step;
    f32 value;
    f32 target;
    u8 pad_1C[4];
} FadeCameraWork;

typedef struct FadeFluidWork {
    u32 columns;
    u32 rows;
    f32 accel;
    f32 damping;
    f32 neighbor;
    f32 limit;
    f32 timeStep;
    f32 cellSize;
    f32 xScale;
    f32 yScale;
    GSvec* heightPage[2];
    GSvec* velocityX;
    GSvec* velocityY;
    GSvec2* texCoord;
    u8 pad_3C[4];
} FadeFluidWork;

extern void fn_801C6688(f32 t);
extern void fn_801C63C0(void* tex, GSvec* pos, f32 scale, f32 offset, f32 t, f32 alpha);
extern void fn_800D9ED8(u32 enable);
extern void fn_800D88DC(u32 mask);
extern void fn_800D888C(u32 mask);
extern void fn_800D9B58(f32 left, f32 top, f32 right, f32 bottom);
extern void fn_800DA4C4(u32 arg0, u32 arg1, u32 arg2);
extern void fn_800DA2BC(u32 arg0, u32 arg1, u32 arg2);
extern void fn_800DA1E8(u32 arg0, u32 arg1, u32 arg2);
extern void fn_800DA100(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
extern void fn_800DA028(u32 arg0);
extern void fn_800D6A00(u32 arg0);
extern void fn_800D7820(void* ptr);
extern void fn_800D85D4(u32 arg0, void* ptr);
extern u32 _fadeEffectGetRandom__FUl(u32 range);
extern u32 fn_800E202C(void* ptr);
extern void fn_800E24B0(u32 handle);
extern void fn_800E209C(u32 handle);
extern u32 fn_800E2C04(u32 size, u32 align);
extern void* fn_800E27B0(u32 handle);

extern u8 lbl_80467030[0x20];
extern u8 lbl_80467050[0x40];
extern u8 lbl_80314AE8[];
extern u8 lbl_8047B3B0;
extern u32 lbl_8047B3B8;
extern const f32 lbl_8047DFE0;
extern const f32 lbl_8047DFE4;
extern const f32 lbl_8047DFF4;
extern const f32 lbl_8047DFF8;
extern const f32 lbl_8047E008;
extern const f32 lbl_8047E00C;
extern const f32 lbl_8047E0A4;
extern const f32 lbl_8047E0A8;
extern const f32 lbl_8047E0C4;
extern const f32 lbl_8047E0C8;
extern const f32 lbl_8047E0CC;

#pragma peephole off
u32 fn_801C54FC(u32 arg0, f32 frame, f32 duration) {
    fn_801C6688(frame / duration);
    return arg0;
}
#pragma peephole on

u32 fn_801C5530(u32 arg0, void* texture, f32 frame, f32 duration, f32 angle, f32 angleDuration) {
    f32 t = frame / duration;
    f32 rot = angle / angleDuration;

    fn_801C6688(t);
    if (texture == NULL) {
        return arg0;
    }

    {
        GSvec pos;

        pos.x = lbl_8047E008;
        pos.y = lbl_8047E00C;
        pos.z = lbl_8047DFE0;
        fn_801C63C0(texture, &pos, lbl_8047DFE4, lbl_8047DFE0, t, rot);
    }
    return arg0;
}

u32 fn_801C63B8(void) {
    return 1;
}

#pragma scheduling off
void fn_801C673C(void) {
    fn_800D9ED8(0);
}
#pragma scheduling on

#pragma peephole off
void fn_801C6760(void) {
    fn_800D9ED8(1);
    fn_800D88DC(1);
    fn_800D888C(6);
    fn_800D9B58(lbl_8047DFE0, lbl_8047DFE0, lbl_8047DFF4, lbl_8047DFF8);
    fn_800DA4C4(1, 6, 7);
    fn_800DA2BC(1, 1, 0);
    fn_800DA1E8(0, 1, 1);
    fn_800DA100(0, 7, 0, 1, 7, 0);
    fn_800DA028(0);
    fn_800D6A00(7);
    fn_800D7820(NULL);
}
#pragma peephole on

void fn_801C680C(void* texture) {
    fn_800D9ED8(1);
    fn_800D88DC(3);
    fn_800D888C(4);
    fn_800D9B58(lbl_8047DFE0, lbl_8047DFE0, lbl_8047DFF4, lbl_8047DFF8);
    fn_800DA4C4(1, 6, 7);
    fn_800DA2BC(1, 1, 0);
    fn_800DA1E8(0, 1, 1);
    fn_800DA100(0, 7, 0, 1, 7, 0);
    fn_800DA028(0);
    fn_800D6A00(4);
    fn_800D7820(lbl_80314AE8);
    fn_800D85D4(0, texture);
}

void _fadeEffect_AdjustParms__Fv(void) {
    FadeCameraWork* cam = (FadeCameraWork*)lbl_80467030;
    f32 scale = lbl_8047E0A4;

    cam->step = cam->step / scale;
    cam->value = cam->value / scale;
    cam->target = cam->target / scale;
}

u32 fn_801C6908(u32 range) {
    return _fadeEffectGetRandom__FUl(range);
}

void fn_801C6928(void) {
    lbl_8047B3B0 = 1;
}

void _fadeFluidSetShockSub__FUlUlf(u32 x, u32 y, f32 strength) {
    FadeFluidWork* fluid = (FadeFluidWork*)lbl_80467050;
    u32 columns = fluid->columns;

    if (x > columns) {
        return;
    }
    if (y > fluid->rows) {
        return;
    }

    fluid->heightPage[lbl_8047B3B8][x + (y * (columns + 1))].z -= strength;
}

void fadeFluidCalcParms(f32 dt) {
    FadeFluidWork* fluid = (FadeFluidWork*)lbl_80467050;
    f32 c = fluid->limit;
    f32 dx = fluid->cellSize;
    f32 c2 = c * c;
    f32 dx2 = dx * dx;
    f32 dtHeight = fluid->timeStep * dt;
    f32 dtLimit2 = dt * c2;
    f32 denom = lbl_8047E0C4 + dtHeight;
    f32 diff = dtHeight - lbl_8047E0C4;
    f32 ratio = lbl_8047E0A8 / denom;
    f32 wave = (dt * dtLimit2) / dx2;
    f32 accel = lbl_8047E0C8 - (lbl_8047E0CC * wave);
    f32 damping = ratio * diff;
    f32 accelOut = accel * ratio;
    f32 neighbor = (lbl_8047E0C4 * wave) * ratio;

    fluid->damping = damping;
    fluid->accel = accelOut;
    fluid->neighbor = neighbor;
}

#pragma peephole off
void fn_801C75EC(void* ptr) {
    u32 handle = fn_800E202C(ptr);
    u32 masked = handle & 0xFFFF;

    if (masked != 0) {
        fn_800E24B0(handle);
        fn_800E209C(handle);
    }
}
#pragma peephole on

#pragma scheduling off
#pragma peephole off
void* fn_801C7630(u32 size) {
    u32 handle = fn_800E2C04(size, 0x20);
    u32 masked = handle & 0xFFFF;

    if (masked != 0) {
        return fn_800E27B0(handle);
    }
    return NULL;
}
#pragma peephole on
#pragma scheduling on
