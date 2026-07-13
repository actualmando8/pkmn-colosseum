/**
 * @file gs_range_801C766C.c
 * @brief gs-engine, 0x801C766C - 0x801CA7EC.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"
#pragma peephole off

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct DistanceSortEntry {
    s32 index;
    s32 blocked;
    Vec3 pos;
    f32 distance;
} DistanceSortEntry;

typedef struct CursorPos {
    u8 row;
    s8 col;
} CursorPos;


extern u8 heroMoveIsMember(s32 member);
extern void heroSetStatus(s32 hero, s32 status, u8 value);
extern void* savedataGetStatus(s32 save, s32 status);
extern void* gamedatasaveBiosGetPtr(void);
extern void fn_8011418C(s32* floorId, s32* prevFloorId, u8* floorPosIndex);
extern void gamedatasaveBiosSetFloorid(void* save, s32 floorId);
extern void gamedatasaveBiosSetPrevfloorid(void* save, s32 floorId);
extern void gamedatasaveBiosSetFloorposindex(void* save, u8 floorPosIndex);
extern s32 fn_801D0748(s32, s32, s32);

extern u32 fn_801906A0(u32 flag);
extern void fn_801D0AFC(s32);
extern void* heroBiosGetPokemonPtr(void* status, u16 index);
extern u8 pokemonCheckValid(void* pokemon);
extern u8 fn_80121ADC(void* pokemon, s32 ribbon);
extern void fn_80121B4C(void* pokemon, s32 ribbon);
extern s32 menuGetCursorFromItemID(s32 menu, s32 itemId);
extern void cursorBiosSetPos(s32 cursor, const CursorPos* pos);
extern void fn_8000D710(s32);
extern s32 fn_80075638(void);
extern void fn_8007565C(void);
extern void menuSubOpenSelect(s32, s32, s32, s32, s32, s32);
extern u8 heroMoveGetResID(s32* floorId, s32* resId, s32 member);
extern void heroMoveChkHinderClear(s32 member);
extern void* fn_800F92D4(s32 lightId);
extern void GSlogWrite(const char* fmt, ...);
extern u8 GSlightHasAnimationEnded(void* light);
extern void GSlightStopAnimation(void* light);
extern void GSlightSetAnimIndex(void* light, s32 index);
extern void GSlightSetAnimRate(void* light, f32 rate);
extern void GSlightSetAnimFrame(void* light, f32 frame);
extern void GSlightSetAnimType(void* light, s32 type);
extern void GSlightSetActive(void* light, u8 active);
extern void _threadSwitch(void);
extern void memoDataSetMemoFlag(s32 flag);
extern void fn_8012F1FC(s32 member);
extern void heroMoveDismissMember(s32 member);
extern u32 _fadeEffectGetRandom__FUl(u32 limit);
extern s32 fn_800D37CC(void);
extern u32 fn_800D3088(void);
extern void fn_8025D164(void);
extern s32 fn_8006ADEC(void);
extern f32 fn_8025D0A8(void* status);
extern void fn_8006ADB4(s32 value);

extern const char lbl_802758AC[];
extern const f32 lbl_8047E108;
extern const f32 lbl_8047E114;
extern const f64 lbl_8047E120;
extern const f64 lbl_8047E128;

u8 fn_801C766C(void)
{
    void* save;
    s32 floorId;
    s32 prevFloorId;
    u8 floorPosIndex;
    s32 isMember;

    if (heroMoveIsMember(1) != 0) {
        isMember = 1;
    } else {
        isMember = 0;
    }

    heroSetStatus(0, 0x18, isMember);
    savedataGetStatus(0, 1);
    save = gamedatasaveBiosGetPtr();
    fn_8011418C(&floorId, &prevFloorId, &floorPosIndex);
    gamedatasaveBiosSetFloorid(save, floorId);
    gamedatasaveBiosSetPrevfloorid(save, prevFloorId);
    gamedatasaveBiosSetFloorposindex(save, floorPosIndex);

    if (fn_801D0748(4, 2, 0) == 4) {
        return 1;
    }

    return 0;
}

s32 _fnDistanceSortFunc__FPCvPCv(const void* lhs, const void* rhs)
{
    const DistanceSortEntry* left = (const DistanceSortEntry*)lhs;
    const DistanceSortEntry* right = (const DistanceSortEntry*)rhs;

    if (left->distance == right->distance) {
        return 0;
    }

    if (left->distance > right->distance) {
        return 1;
    }

    return -1;
}

void fn_801C852C(s32 mode)
{
    void* status;
    void* pokemon;
    u16 i;

    if (fn_801906A0(0x8AE) == 0) {
        switch (mode) {
        case 0:
            fn_801D0AFC(0);
            break;
        case 1:
            status = savedataGetStatus(0, 2);
            i = 0;
            while (i < 6) {
                pokemon = heroBiosGetPokemonPtr(status, i);
                if (pokemonCheckValid(pokemon) != 0) {
                    if (fn_80121ADC(pokemon, 3) != 0) {
                        fn_80121B4C(pokemon, 3);
                    }
                    if (fn_80121ADC(pokemon, 4) != 0) {
                        fn_80121B4C(pokemon, 4);
                    }
                }
                i++;
            }
            break;
        }
    }
}

void fn_801C8628(void)
{
    CursorPos initial;
    CursorPos pos;
    s32 cursor;

    cursor = menuGetCursorFromItemID(0x59, 0x20C);
    if (cursor < 0) {
        cursor = 0;
    }

    initial.row = 0;
    initial.col = cursor;
    pos = initial;
    cursorBiosSetPos(2, &pos);
    fn_8000D710(1);
}

void fn_801C8804(void)
{
    u32 ready = (u8)fn_80075638();

    if (ready != 0) {
        fn_8007565C();
    }
}

void fn_801C8DD0(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    menuSubOpenSelect(1, a, b, c, d, e);
}

void fn_801C9C9C(void)
{
    s32 floorId;
    s32 resId;
    u32 found;

    found = (u8)heroMoveGetResID(&floorId, &resId, 1);
    if (found != 0) {
        heroMoveChkHinderClear(1);
    }
}

u8 fn_801C9CDC(s32 lightId, s32 wait)
{
    void* light = fn_800F92D4(lightId);

    if (light == NULL) {
        GSlogWrite(lbl_802758AC, lightId);
        return 0;
    }

    for (;;) {
        if (GSlightHasAnimationEnded(light) != 0) {
            return 0;
        }
        if (wait != 0) {
            _threadSwitch();
        } else {
            return 0;
        }
    }

    return 0;
}

void scriptLightStopMotion(s32 lightId)
{
    void* light = fn_800F92D4(lightId);

    if (light == NULL) {
        GSlogWrite(lbl_802758AC, lightId);
    } else {
        GSlightStopAnimation(light);
    }
}

void fn_801C9DC4(s32 lightId, s32 animIndex, s32 frame, s32 loop)
{
    void* light = fn_800F92D4(lightId);

    if (light == NULL) {
        GSlogWrite(lbl_802758AC, lightId);
    } else {
        GSlightSetAnimIndex(light, animIndex);
        GSlightSetAnimRate(light, lbl_8047E108);
        GSlightSetAnimFrame(light, (f32)frame);
        if (loop != 0) {
            GSlightSetAnimType(light, 1);
        } else {
            GSlightSetAnimType(light, 0);
        }
    }
}

void fn_801C9E7C(s32 lightId, s32 active)
{
    void* light = fn_800F92D4(lightId);

    if (light == NULL) {
        GSlogWrite(lbl_802758AC, lightId);
    } else {
        GSlightSetActive(light, (u8)active);
    }
}

void scriptSetMemoFlag(void)
{
    memoDataSetMemoFlag(0);
}

void fn_801C9F00(s32 active)
{
    u32 enabled = (u8)active;

    if (enabled != 0) {
        heroSetStatus(0, 0x18, 1);
        fn_8012F1FC(1);
    } else {
        heroSetStatus(0, 0x18, 0);
        heroMoveDismissMember(1);
    }
}

u32 fadeEffectGetRandom(u32 limit)
{
    return _fadeEffectGetRandom__FUl(limit);
}

void fn_801CA4F8(f32 frames)
{
    f32 elapsed = lbl_8047E114;

    while (elapsed < frames) {
        _threadSwitch();
        elapsed += (f32)fn_800D3088() / (f32)fn_800D37CC();
    }
}

void fn_801CA708(void)
{
    fn_8025D164();
}

s32 fn_801CA728(s32 count)
{
    s32 base;
    s32 value;
    f32 multiplier;

    base = fn_8006ADEC();
    multiplier = fn_8025D0A8(savedataGetStatus(0, 2));
    value = base + (s32)((f32)count * multiplier);
    fn_8006ADB4(value);
    return value;
}

void fn_801CA7AC(s32 value)
{
    fn_8006ADB4(value);
}

s32 fn_801CA7CC(void)
{
    return fn_8006ADEC();
}

#pragma peephole reset
