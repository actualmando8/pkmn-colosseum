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

typedef struct FadeTrailPoint {
    f32 x;
    f32 y;
    f32 z;
    f32 angle;
    s32 alpha;
} FadeTrailPoint;

typedef struct FadeTrailWork {
    FadeTrailPoint point[12];
} FadeTrailWork;

extern void fn_801C6688(f32 t);
extern void fn_801C63C0(void* tex, GSvec* pos, f32 scale, f32 offset, f32 t, f32 alpha);
extern void fn_801C5B60(FadeTrailPoint* point, s32 alpha, f32 scale, f32 angle);
extern void fn_801C5D60(void);
extern void fn_801C673C(void);
extern void fn_801C680C(void* texture);
extern void fn_801C53BC(void* texture);
extern void fn_800D75F4(void* ptr);
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
extern void fn_800D848C(u32 arg0, u32 arg1, u32 arg2, void* ptr);
extern void fn_800DC1D4(u32 arg0);
extern void fn_800DC224(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern void fn_800DC14C(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
extern void fn_800DC0D4(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern void fn_800DC04C(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
extern void fn_800DBFD4(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern void fn_800E042C(void* mtx, GSvec* scale);
extern void fn_800E02E8(void* mtx, f32 angle);
extern void fn_800E03B4(void* mtx, GSvec* trans);
extern void GSvecTransform(GSvec* dst, void* mtx, GSvec* src);
extern void fn_800D67BC(u32 arg0);
extern void fn_800D6680(f32 x, f32 y, f32 z);
extern void fn_800D5CB8(u32 arg0, u8 r, u8 g, u8 b, u8 a);
extern void fn_800D59B8(u32 arg0, f32 s, f32 t);
extern void fn_800D6728(void);
extern void GSgfxEndBackFBCapture(void* texture);
extern u32 _fadeEffectGetRandom__FUl(u32 range);
extern u32 fn_800E202C(void* ptr);
extern void fn_800E24B0(u32 handle);
extern void fn_800E209C(u32 handle);
extern u32 fn_800E2C04(u32 size, u32 align);
extern void* fn_800E27B0(u32 handle);

extern u8 lbl_80467030[0x20];
extern u8 lbl_80467050[0x40];
extern u8 lbl_80466E50[0x1E0];
extern u8 lbl_80314AE8[];
extern u8 lbl_8047B3B0;
extern void* lbl_8047B3B4;
extern u32 lbl_8047B3B8;
extern const f32 lbl_8047DFDC;
extern const f32 lbl_8047DFE0;
extern const f32 lbl_8047DFE4;
extern const f32 lbl_8047DFF0;
extern const f32 lbl_8047DFF4;
extern const f32 lbl_8047DFF8;
extern const f64 lbl_8047E000;
extern const f32 lbl_8047E008;
extern const f32 lbl_8047E00C;
extern const f32 lbl_8047E010;
extern const f32 lbl_8047E014;
extern const f32 lbl_8047E018;
extern const f32 lbl_8047E04C;
extern const f32 lbl_8047E050;
extern const f32 lbl_8047E054;
extern const f32 lbl_8047E058;
extern const f32 lbl_8047E0A4;
extern const f32 lbl_8047E0A8;
extern const f32 lbl_8047E0C4;
extern const f32 lbl_8047E0C8;
extern const f32 lbl_8047E0CC;

#pragma peephole off
u32 fn_801C4CB8(u32 finish, void* texture, f32 frame, f32 duration)
{
    FadeCameraWork* camera;
    f32 textureMatrix[3][4];
    f32 matrix[3][4];
    GSvec position;
    GSvec point;
    GSvec transformed;
    f32 scaleValue;
    f32 progress;
    f32 left;
    f32 right;
    f32 top;
    f32 bottom;

    if (texture == NULL) {
        return finish;
    }

    if (lbl_8047B3B0 == 1) {
        lbl_8047B3B0 = 0;
        fn_801C53BC(texture);
    }

    if (lbl_8047B3B0 == 0 && (u8)finish == 0 && lbl_8047B3B4 != NULL) {
        fn_800D75F4(lbl_8047B3B4);
        lbl_8047B3B4 = NULL;
    }

    progress = frame / duration;
    camera = (FadeCameraWork*)lbl_80467030;
    camera->frame++;
    camera->value += camera->step;
    if (camera->step >= lbl_8047DFE0) {
        camera->step += lbl_8047DFDC * progress;
        if (camera->value >= camera->target) {
            camera->value = camera->target;
        }
    } else {
        camera->step -= lbl_8047DFDC * progress;
        if (camera->value <= camera->target) {
            camera->value = camera->target;
        }
    }

    position.x = lbl_8047E008;
    position.y = lbl_8047E00C;
    position.z = lbl_8047DFE0;
    fn_801C63C0(texture, &position, lbl_8047DFE4, lbl_8047DFE0, progress,
                lbl_8047DFE0);

    position.x = lbl_8047E008 + camera->value;
    position.y = lbl_8047E00C;
    position.z = lbl_8047DFE0;
    fn_801C63C0(texture, &position, lbl_8047DFE4, lbl_8047DFE0, progress,
                lbl_8047DFDC);

    position.x = lbl_8047E008 - camera->value;
    position.y = lbl_8047E00C;
    position.z = lbl_8047DFE0;
    fn_801C63C0(texture, &position, lbl_8047DFE4, lbl_8047DFE0, progress,
                lbl_8047DFDC);

    if (lbl_8047B3B4 != NULL) {
        fn_800D9ED8(1);
        fn_800D88DC(0x80000003);
        fn_800D888C(4);
        fn_800D9B58(lbl_8047DFE0, lbl_8047DFE0, lbl_8047DFF4, lbl_8047DFF8);
        fn_800DA4C4(1, 6, 7);
        fn_800DA2BC(1, 1, 0);
        fn_800DA1E8(0, 1, 1);
        fn_800DA100(0, 7, 0, 1, 7, 0);
        fn_800DA028(0);
        fn_800D848C(0, 0, 4, textureMatrix);
        fn_800D848C(1, 0, 5, textureMatrix);
        fn_800DC1D4(2);
        fn_800D85D4(0, ((FadeCameraWork*)lbl_80467030)->tex1);
        fn_800DC224(0, 0, 0, 0, 0);
        fn_800DC14C(0, 8, 0, 0, 0, 1);
        fn_800DC0D4(0, 9, 13, 12, 15);
        fn_800DC04C(0, 0, 0, 0, 0, 1);
        fn_800DBFD4(0, 7, 4, 5, 7);
        fn_800D85D4(1, texture);
        fn_800DC224(1, 0, 1, 1, 0);
        fn_800DC14C(1, 0, 0, 0, 0, 0);
        fn_800DC0D4(1, 15, 8, 2, 15);
        fn_800DC04C(1, 0, 0, 0, 0, 0);
        fn_800DBFD4(1, 7, 7, 7, 1);
        fn_800D7820(lbl_8047B3B4);
        fn_800D6A00(4);

        position.x = lbl_8047E008;
        position.y = lbl_8047E00C;
        position.z = lbl_8047DFE0;
        scaleValue = lbl_8047DFE4 + ((f32)camera->frame / lbl_8047E010);
        point.x = scaleValue;
        point.y = scaleValue;
        point.z = scaleValue;
        progress = lbl_8047DFF0 * (lbl_8047DFE4 - progress);
        fn_800E042C(matrix, &point);
        fn_800E02E8(matrix, lbl_8047DFE0);
        fn_800E03B4(matrix, &position);

        point.x = lbl_8047E014;
        point.y = lbl_8047E014;
        point.z = lbl_8047DFE0;
        GSvecTransform(&transformed, matrix, &point);
        transformed.x -= lbl_8047E008;
        transformed.y -= lbl_8047E00C;
        left = (lbl_8047E008 - transformed.x) / lbl_8047DFF4;
        right = (lbl_8047E008 + transformed.x) / lbl_8047DFF4;
        top = (lbl_8047E00C - transformed.y) / lbl_8047DFF8;
        bottom = (lbl_8047E00C + transformed.y) / lbl_8047DFF8;
        fn_800D67BC(4);

        point.x = lbl_8047E018;
        point.y = lbl_8047E018;
        point.z = lbl_8047DFE0;
        GSvecTransform(&transformed, matrix, &point);
        fn_800D6680(transformed.x, transformed.y, transformed.z);
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, (u8)progress);
        fn_800D59B8(0, lbl_8047DFE0, lbl_8047DFE0);
        fn_800D59B8(1, left, top);

        point.x = lbl_8047E014;
        point.y = lbl_8047E018;
        point.z = lbl_8047DFE0;
        GSvecTransform(&transformed, matrix, &point);
        fn_800D6680(transformed.x, transformed.y, transformed.z);
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, (u8)progress);
        fn_800D59B8(0, lbl_8047DFE4, lbl_8047DFE0);
        fn_800D59B8(1, right, top);

        point.x = lbl_8047E018;
        point.y = lbl_8047E014;
        point.z = lbl_8047DFE0;
        GSvecTransform(&transformed, matrix, &point);
        fn_800D6680(transformed.x, transformed.y, transformed.z);
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, (u8)progress);
        fn_800D59B8(0, lbl_8047DFE0, lbl_8047DFE4);
        fn_800D59B8(1, left, bottom);

        point.x = lbl_8047E014;
        point.y = lbl_8047E014;
        point.z = lbl_8047DFE0;
        GSvecTransform(&transformed, matrix, &point);
        fn_800D6680(transformed.x, transformed.y, transformed.z);
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, (u8)progress);
        fn_800D59B8(0, lbl_8047DFE4, lbl_8047DFE4);
        fn_800D59B8(1, right, bottom);
        fn_800D6728();
        fn_800DC1D4(1);
        fn_800D888C(0x80000000);
        fn_800D9ED8(0);
    }

    if (lbl_8047B3B0 == 0 && (u8)finish == 0) {
        GSgfxEndBackFBCapture(texture);
    }
    return finish;
}
#pragma peephole on

#pragma peephole off
u32 fn_801C54FC(u32 arg0, f32 frame, f32 duration) {
    fn_801C6688(frame / duration);
    return arg0;
}
#pragma peephole on

#pragma peephole off
u32 fn_801C5530(u32 arg0, void* texture, f32 frame, f32 duration, f32 angle, f32 angleDuration) {
    u32 result;
    void* tex;
    f32 t;
    f32 rot;

    result = arg0;
    tex = texture;
    t = frame / duration;
    rot = angle / angleDuration;

    fn_801C6688(t);
    if (tex == NULL) {
        return result;
    }

    {
        GSvec pos;

        pos.x = lbl_8047E008;
        pos.y = lbl_8047E00C;
        pos.z = lbl_8047DFE0;
        fn_801C63C0(tex, &pos, lbl_8047DFE4, lbl_8047DFE0, t, rot);
    }
    return result;
}
#pragma peephole on

#pragma peephole off
u32 fn_801C5898(u32 arg0, void* texture, f32 frame, f32 duration,
                f32 angle, f32 angleDuration)
{
    FadeCameraWork* camera;
    FadeTrailWork* trail;
    GSvec position;
    f32 positionStep[2];
    f32 angleStep[2];
    f32 alphaScale;
    f32 maxPosition;
    f32 progress;
    f32 angleProgress;
    f32 value;
    s32 i;
    s32 j;

    if (lbl_8047B3B0 == 1) {
        lbl_8047B3B0 = 0;
        fn_801C5D60();
    }

    progress = frame / duration;
    angleProgress = angle / angleDuration;
    camera = (FadeCameraWork*)lbl_80467030;
    camera->frame++;
    camera->value += camera->step;
    if (camera->value >= camera->target) {
        camera->value = camera->target;
    }
    fn_801C6688(progress);

    if (texture == NULL) {
        return arg0;
    }

    position.x = lbl_8047E008;
    position.y = lbl_8047E00C;
    position.z = lbl_8047DFE0;
    fn_801C63C0(texture, &position, lbl_8047DFE4, lbl_8047DFE0,
                progress, angleProgress);
    fn_801C680C(((FadeCameraWork*)lbl_80467030)->tex0);

    value = camera->value;
    positionStep[0] = value;
    positionStep[1] = -value;
    angleStep[0] = lbl_8047E04C * (value / lbl_8047E050);
    angleStep[1] = -angleStep[0];
    alphaScale = lbl_8047DFE4 - progress;

    trail = (FadeTrailWork*)lbl_80466E50;
    for (i = 0; i < 2; i++) {
        for (j = 11; j > 0; j--) {
            FadeTrailPoint* point = &trail[i].point[j];

            if ((camera->frame & 3) == 0) {
                point->x = point[-1].x;
                point->y = point[-1].y;
                point->z = point[-1].z;
                point->angle = point[-1].angle;
            }
            fn_801C5B60(point,
                        (s32)(alphaScale * (f32)point->alpha),
                        lbl_8047DFE4, point->angle);
        }

        value = trail[i].point[0].angle + angleStep[i];
        if (value <= lbl_8047DFE0) {
            value += lbl_8047E04C;
        } else if (value >= lbl_8047E04C) {
            value -= lbl_8047E04C;
        }
        trail[i].point[0].angle = value;

        maxPosition = lbl_8047E054;
        value = trail[i].point[0].x + positionStep[i];
        if (value > maxPosition) {
            value = maxPosition;
        } else if (value < lbl_8047E058) {
            value = lbl_8047E058;
        }
        trail[i].point[0].x = value;

        fn_801C5B60(&trail[i].point[0],
                    (s32)(alphaScale * (f32)trail[i].point[0].alpha),
                    lbl_8047DFE4, trail[i].point[0].angle);
    }
    fn_801C673C();
    return arg0;
}
#pragma peephole on

#pragma peephole off
u32 fn_801C5ED0(u32 arg0, void* texture, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    extern void _fadeEffectFunction_UDLR_FirstInit__FP9GStextureUs(void* texture, u16 mode);
    extern u32 fn_801C6008(u32 arg0, void* texture, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
    f32 arg2Local;
    u32 arg0Local;
    f32 arg3Local;
    f32 arg4Local;
    f32 arg5Local;
    void* tex;

    arg2Local = arg2;
    arg0Local = arg0;
    arg3Local = arg3;
    arg4Local = arg4;
    arg5Local = arg5;
    tex = texture;

    if (tex == NULL) {
        return arg0Local;
    }

    if (lbl_8047B3B0 == 1) {
        lbl_8047B3B0 = 0;
        _fadeEffectFunction_UDLR_FirstInit__FP9GStextureUs(tex, 8);
    }

    return fn_801C6008(arg0Local, tex, arg2Local, arg3Local, arg4Local, arg5Local);
}
#pragma peephole on

#pragma peephole off
void fn_801C5F6C(u32 arg0, void* texture, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    extern void _fadeEffectFunction_UDLR_FirstInit__FP9GStextureUs(void* texture, u16 mode);
    extern void fn_801C6008(u32 arg0, void* texture, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
    f32 arg2Local;
    u32 arg0Local;
    f32 arg3Local;
    f32 arg4Local;
    f32 arg5Local;
    void* tex;

    arg2Local = arg2;
    arg0Local = arg0;
    arg3Local = arg3;
    arg4Local = arg4;
    arg5Local = arg5;
    tex = texture;

    if (tex != NULL) {
        if (lbl_8047B3B0 == 1) {
            lbl_8047B3B0 = 0;
            _fadeEffectFunction_UDLR_FirstInit__FP9GStextureUs(tex, 2);
        }

        fn_801C6008(arg0Local, tex, arg2Local, arg3Local, arg4Local, arg5Local);
    }
}
#pragma peephole on

u32 fn_801C63B8(void) {
    return 1;
}

void fn_801C6688(f32 t) {
    extern void fn_801C673C(void);
    extern void fn_801C6760(void);
    s32 alpha;

    fn_801C6760();
    alpha = 0xFF - (s32)(lbl_8047DFF0 * t);
    fn_800D67BC(2);
    fn_800D6680(lbl_8047DFE0, lbl_8047DFE0, lbl_8047DFE0);
    fn_800D5CB8(0, 0, 0, 0, alpha);
    fn_800D6680(lbl_8047DFF4, lbl_8047DFF8, lbl_8047DFE0);
    fn_800D5CB8(0, 0, 0, 0, alpha);
    fn_800D6728();
    fn_801C673C();
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

    ((f32*)fluid->heightPage[lbl_8047B3B8])
        [3 * (x + (y * (columns + 1))) + 2] -= strength;
}

void fadeFluidCalcParms(f32 dt) {
    FadeFluidWork* fluid = (FadeFluidWork*)lbl_80467050;
    f32 c = fluid->limit;
    f32 dx = fluid->cellSize;
    f32 c2 = c * c;
    f32 dx2 = dx * dx;
    f32 dtHeight = fluid->timeStep * dt;
    f32 dtLimit2 = dt * c2;
    f32 wave = (dt * dtLimit2) / dx2;
    f32 denom = lbl_8047E0C4 + dtHeight;
    f32 diff = dtHeight - lbl_8047E0C4;
    f32 ratio = lbl_8047E0A8 / denom;
    f32 accel = lbl_8047E0C8 - (lbl_8047E0CC * wave);
    f32 damping = ratio * diff;
    f32 accelOut = accel * ratio;
    f32 neighbor = (lbl_8047E0C4 * wave) * ratio;

    fluid->damping = damping;
    fluid->accel = accelOut;
    fluid->neighbor = neighbor;
}


void fadeFluidInit(u32 columns, u32 rows, f32 cellSize, f32 calcStep,
                   f32 waveLimit, f32 timeStep)
{
    extern void* fn_801C7630(u32 size);
    extern f32 sqrtf(f32);
    extern void set__5GSvecFfff(GSvec*, f32, f32, f32);
    extern void OSReport(const char*, ...);
    extern char lbl_80275860[];
    extern const f32 lbl_8047E0AC;
    extern const f32 lbl_8047E0D0;
    extern const f32 lbl_8047E0D4;
    extern const f32 lbl_8047E0F0;

    FadeFluidWork* fluid = (FadeFluidWork*)lbl_80467050;
    u32 pointCount;
    u32 vectorBytes;
    u32 x;
    u32 y;
    u32 index;
    f32 maximumLimit;
    f32 rowPosition;
    f32 columnPosition;
    f32 texStepX;
    f32 texStepY;

    pointCount = (columns + 1) * (rows + 1);
    vectorBytes = pointCount * sizeof(GSvec);
    fluid->columns = columns;
    fluid->rows = rows;
    fluid->xScale = lbl_8047E0D0 / (f32)columns;
    fluid->yScale = lbl_8047E0D4 / (f32)rows;
    fluid->heightPage[0] = fn_801C7630(vectorBytes);
    fluid->heightPage[1] = fn_801C7630(vectorBytes);
    lbl_8047B3B8 = 0;
    fluid->velocityX = fn_801C7630(vectorBytes);
    fluid->velocityY = fn_801C7630(vectorBytes);
    fluid->texCoord = fn_801C7630(pointCount * sizeof(GSvec2));

    maximumLimit = (cellSize / (lbl_8047E0C4 * calcStep)) *
                   sqrtf(lbl_8047E0C4 + timeStep);
    if (waveLimit > lbl_8047E0AC && waveLimit >= maximumLimit) {
        OSReport(lbl_80275860, waveLimit, maximumLimit);
        waveLimit = maximumLimit - lbl_8047E0F0;
    }
    fluid->cellSize = cellSize;
    fluid->limit = waveLimit;
    fluid->timeStep = timeStep;
    fadeFluidCalcParms(calcStep);

    rowPosition = lbl_8047E0AC;
    texStepX = lbl_8047E0A8 / (f32)columns;
    texStepY = lbl_8047E0A8 / (f32)rows;
    index = 0;
    for (y = 0; y <= rows; y++) {
        columnPosition = lbl_8047E0AC;
        for (x = 0; x <= columns; x++) {
            set__5GSvecFfff(&fluid->heightPage[0][index],
                            columnPosition, rowPosition, lbl_8047E0AC);
            GSvecCopy(&fluid->heightPage[1][index],
                      &fluid->heightPage[0][index]);
            set__5GSvecFfff(&fluid->velocityX[index],
                            lbl_8047E0AC, lbl_8047E0AC, waveLimit);
            set__5GSvecFfff(&fluid->velocityY[index],
                            waveLimit, lbl_8047E0AC, lbl_8047E0AC);
            fluid->texCoord[index].x = texStepX * (f32)x;
            fluid->texCoord[index].y = texStepY * (f32)y;
            columnPosition += cellSize;
            index++;
        }
        rowPosition += cellSize;
    }
}

#pragma scheduling off
void fadeFluidQuit(void) {
    extern void fn_801C75EC(void* ptr);
    FadeFluidWork* fluid0 = (FadeFluidWork*)lbl_80467050;
    FadeFluidWork* fluid1;
    FadeFluidWork* fluid2;
    FadeFluidWork* fluid3;
    FadeFluidWork* fluid4;

    if (fluid0->heightPage[0] != NULL) {
        fn_801C75EC(fluid0->heightPage[0]);
    }

    fluid1 = (FadeFluidWork*)lbl_80467050;
    if (fluid1->heightPage[1] != NULL) {
        fn_801C75EC(fluid1->heightPage[1]);
    }

    fluid2 = (FadeFluidWork*)lbl_80467050;
    if (fluid2->velocityX != NULL) {
        fn_801C75EC(fluid2->velocityX);
    }

    fluid3 = (FadeFluidWork*)lbl_80467050;
    if (fluid3->velocityY != NULL) {
        fn_801C75EC(fluid3->velocityY);
    }

    fluid4 = (FadeFluidWork*)lbl_80467050;
    if (fluid4->texCoord != NULL) {
        fn_801C75EC(fluid4->texCoord);
    }

    fluid0->heightPage[0] = NULL;
    fluid1->heightPage[1] = NULL;
    fluid2->velocityX = NULL;
    fluid3->velocityY = NULL;
    fluid4->texCoord = NULL;
}
#pragma scheduling on

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
