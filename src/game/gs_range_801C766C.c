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


/* Eight-direction collision search used to place a battle actor. */
s32 fn_801C7730(s32 side, s32 slot)
{
    extern const Vec3 lbl_802758A0;
    extern const f32 lbl_8047E100, lbl_8047E104, lbl_8047E10C;
    extern const f32 lbl_8047E110, lbl_8047E118, lbl_8047E11C;
    extern u8 fn_8018BDF4();
    extern u32 fn_8018D998();
    extern void* peopleSearchID();
    extern void* peopleInfoBiosGetPtr();
    extern f32 fn_8018F5E4();
    extern f32 GSvecDistance();
    extern void GSvecAdd();
    extern void fn_800E013C();
    extern u8 fn_8018D680();
    extern s32 fn_8010F320();
    extern void qsort();
    extern void fn_80183350();
    extern void fn_8018AACC();
    extern u8 peopleMoveCheck();
    extern void fn_8018BA04();
    extern void fn_80187D48();
    extern void fn_80183018();
    extern f64 cos(f64);
    extern f64 sin(f64);

    s32 floorId, resId, floorId2, resId2;
    Vec3 origin = lbl_802758A0;
    Vec3 partner, requested, center, offset, midpoint, saved;
    DistanceSortEntry entries[8];
    f32 radius;
    f32 timeout;
    s32 i, k, hit, wanted;
    u8 done;

#define SEGMENT_HIT(out, a, b, c) do {                                    \
    if (GSvecDistance((a), (b)) < lbl_8047E104) {                         \
        (out) = fn_8018D680((a), (b), (c), lbl_8047E118);                 \
    } else {                                                               \
        GSvecAdd(&midpoint, (a), (b));                                    \
        fn_800E013C(&midpoint, &midpoint, lbl_8047E108);                  \
        (out) = fn_8018D680((a), &midpoint, (c), lbl_8047E118);           \
        if ((out) == 0)                                                    \
            (out) = fn_8018D680(&midpoint, (b), (c), lbl_8047E118);       \
    }                                                                      \
} while (0)

#define WAIT_MOVE(out) do {                                                \
    (out) = 0;                                                             \
    timeout = lbl_8047E11C;                                                \
    while (peopleMoveCheck(0, 0x65, 0) != 0) {                            \
        _threadSwitch();                                                   \
        timeout -= (f32)(u32)fn_800D3088() / (f32)fn_800D37CC();          \
        if (timeout <= lbl_8047E114) { (out) = 1; break; }                 \
    }                                                                      \
} while (0)

    if (heroMoveGetResID(&floorId, &resId, 1) == 0) return 0;
    fn_8018BDF4(resId, floorId, &origin);
    if (heroMoveGetResID(&floorId2, &resId2, 0) == 0) return 0;
    fn_8018BDF4(resId2, floorId2, &partner);
    fn_8018BDF4(side, slot, &requested);

    {
        void* person = peopleSearchID(fn_8018D998(side, slot));
        void* info;
        if (person == NULL) return 0;
        info = peopleInfoBiosGetPtr(*(u32*)((u8*)person + 0x30));
        if (info == NULL) return 0;
        radius = fn_8018F5E4(info) + lbl_8047E100;
    }

    center = origin;
    if (GSvecDistance(&partner, &requested) < lbl_8047E104) {
        hit = fn_8018D680(&partner, &requested, &center,
                          radius + lbl_8047E100);
    } else {
        GSvecAdd(&midpoint, &partner, &requested);
        fn_800E013C(&midpoint, &midpoint, lbl_8047E108);
        hit = fn_8018D680(&partner, &midpoint, &center,
                          radius + lbl_8047E100);
        if (hit == 0)
            hit = fn_8018D680(&midpoint, &requested, &center,
                              radius + lbl_8047E100);
    }
    if (hit == 0) return 0;

    for (i = 0; i < 8; i++) {
        f32 angle = (f32)i * lbl_8047E10C;
        entries[i].index = i;
        offset.x = lbl_8047E110 * (f32)cos(angle);
        offset.y = lbl_8047E114;
        offset.z = lbl_8047E110 * (f32)sin(angle);
        GSvecAdd(&entries[i].pos, &offset, &partner);
        entries[i].blocked = fn_8010F320(&partner, &entries[i].pos, 0) != 0;
        entries[i].distance = GSvecDistance(&entries[i].pos, &origin);
    }
    qsort(entries, 8, sizeof(DistanceSortEntry), _fnDistanceSortFunc__FPCvPCv);

    for (i = 0; i < 8; i++) {
        if (entries[i].blocked == 1) continue;
        center = entries[i].pos;
        SEGMENT_HIT(hit, &partner, &requested, &center);
        if (hit != 0) continue;
        center = partner;
        SEGMENT_HIT(hit, &origin, &entries[i].pos, &center);
        if (hit != 0) continue;

        fn_80183350(resId, floorId);
        fn_8018AACC(resId, floorId, 1, &entries[i].pos);
        WAIT_MOVE(done);
        if (done != 0) break;

        wanted = -2;
        if (entries[i].index == 1 || entries[i].index == 3) wanted = -1;
        if (entries[i].index == 5) wanted = 4;
        if (entries[i].index == 7) wanted = 0;
        if (wanted != -2) {
            for (k = 0; k < 8; k++) {
                if (entries[k].index == wanted && entries[k].blocked == 0) {
                    center = entries[k].pos;
                    SEGMENT_HIT(hit, &partner, &requested, &center);
                    if (hit == 0) {
                        fn_8018AACC(resId, floorId, 1, &entries[k].pos);
                        WAIT_MOVE(done);
                    }
                    break;
                }
            }
        }
        if (done == 0) {
            fn_8018BA04(resId, floorId, &saved);
            fn_80187D48(resId, floorId, saved.x, saved.y, saved.z,
                        lbl_8047E100);
            WAIT_MOVE(done);
            fn_80183018(resId, floorId);
            return 0;
        }
    }

    for (i = 0; i < 8; i++) {
        f32 angle = (f32)i * lbl_8047E10C;
        entries[i].index = i;
        offset.x = lbl_8047E110 * (f32)cos(angle);
        offset.y = lbl_8047E114;
        offset.z = lbl_8047E110 * (f32)sin(angle);
        GSvecAdd(&entries[i].pos, &offset, &origin);
        entries[i].blocked = fn_8010F320(&origin, &entries[i].pos, 0) != 0;
        entries[i].distance = GSvecDistance(&entries[i].pos, &origin);
    }
    qsort(entries, 8, sizeof(DistanceSortEntry), _fnDistanceSortFunc__FPCvPCv);

    for (i = 0; i < 8; i++) {
        if (entries[i].blocked == 1) continue;
        center = partner;
        SEGMENT_HIT(hit, &origin, &entries[i].pos, &center);
        if (hit != 0) continue;
        center = entries[i].pos;
        SEGMENT_HIT(hit, &partner, &requested, &center);
        if (hit != 0) continue;
        fn_80183350(resId, floorId);
        fn_8018AACC(resId, floorId, 1, &entries[i].pos);
        WAIT_MOVE(done);
        if (done == 0) {
            fn_8018BA04(resId, floorId, &saved);
            fn_80187D48(resId, floorId, saved.x, saved.y, saved.z,
                        lbl_8047E100);
            WAIT_MOVE(done);
            fn_80183018(resId, floorId);
            return 0;
        }
    }

#undef WAIT_MOVE
#undef SEGMENT_HIT
    return 0;
}

/* Drive one scripted field actor animation and its paired movement actors. */
s32 fn_801C8E14(s32 floorDataId, u32 actorIndex, s16 mode, u8 direction)
{
    typedef struct FieldAnimActor {
        s8 enterAnim;
        s8 exitAnim;
        u8 pad02[3];
        s8 actionAnimA;
        s8 actionAnimB;
        u8 pad07;
        u8 partIndex;
        u8 pad09[11];
        void* resource;
    } FieldAnimActor;
    typedef struct PartTransform {
        Vec3 position;
        f32 rest[4];
    } PartTransform;

    extern u32* lbl_80478EC8;
    extern FieldAnimActor* lbl_80478ECC;
    extern s32 lbl_8047B3C0;
    extern u8 lbl_8047B3C4;
    extern Vec3 lbl_80467090[];
    extern const f32 lbl_8047E100;
    extern const f32 lbl_8047E11C;
    extern const f32 lbl_8047E130;
    extern const f32 lbl_8047E134;
    extern const f32 lbl_8047E138;
    extern const f32 lbl_8047E13C;
    extern const f32 lbl_8047E140;
    extern const f32 lbl_8047E144;
    extern const f32 lbl_8047E148;
    extern void* floorGetResource();
    extern void* floorDataBiosGetPtr();
    extern s32 floorDataBiosGetGroupID();
    extern void fn_801845E4();
    extern void fn_801860F8();
    extern void fn_80188AF4();
    extern void fn_80184470();
    extern void fn_80188F78();
    extern void fn_8018805C();
    extern void fn_8018AACC();
    extern void fn_8018C0A8();
    extern u8 peopleMoveCheck();
    extern void fn_80166A28();
    extern void fn_801669BC();
    extern void GSmodelSetAnimIndex();
    extern void GSmodelSetAnimFrame();
    extern void GSmodelSetAnimRate();
    extern void GSmodelSetAnimType();
    extern void GSmodelStartAnimation();
    extern u8 GSmodelHasAnimationEnded();
    extern u8 GSmodelCanAnimate();
    extern void* GSmodelGetPart();
    extern void GSpartGetTransform();
    extern void GSpartFree();
    extern void GSmodelGetFrameCount();

    FieldAnimActor* actor;
    void* model;
    void* part;
    PartTransform transform;
    Vec3 firstPos;
    Vec3 secondPos;
    f32 frame;
    f32 timer;
    s32 groupId;
    s16 animIndex;
    u8 timedOut;
    s32 i;

#define START_MODEL_ANIM(index, startFrame)                                \
    do {                                                                    \
        if (model != 0 && (s16)(index) >= 0) {                             \
            GSmodelSetAnimIndex(model, (s16)(index));                      \
            GSmodelSetAnimFrame(model, (startFrame));                      \
            GSmodelSetAnimRate(model, lbl_8047E108);                       \
            GSmodelSetAnimType(model, 0);                                  \
            GSmodelStartAnimation(model);                                  \
        }                                                                   \
    } while (0)

#define WAIT_MODEL()                                                        \
    do {                                                                    \
        if (model != 0) {                                                   \
            while (GSmodelHasAnimationEnded(model) == 0) {                 \
                _threadSwitch();                                            \
            }                                                               \
        }                                                                   \
    } while (0)

#define WAIT_SECONDS(amount)                                                \
    do {                                                                    \
        timer = lbl_8047E114;                                               \
        while (timer < (amount)) {                                         \
            _threadSwitch();                                                \
            timer += (f32)(u32)fn_800D3088() / (f32)fn_800D37CC();         \
        }                                                                   \
    } while (0)

#define WAIT_ACTOR65(out)                                                   \
    do {                                                                    \
        (out) = 0;                                                          \
        timer = lbl_8047E11C;                                               \
        while (timer > lbl_8047E114) {                                     \
            if (peopleMoveCheck(0, 0x65, 0) == 0) {                        \
                break;                                                      \
            }                                                               \
            _threadSwitch();                                                \
            timer -= (f32)(u32)fn_800D3088() / (f32)fn_800D37CC();         \
        }                                                                   \
        if (timer <= lbl_8047E114) {                                       \
            (out) = 1;                                                      \
        }                                                                   \
    } while (0)

    frame = lbl_8047E114;
    animIndex = -1;
    timedOut = 0;

    if (actorIndex >= lbl_80478EC8[0]) {
        return -1;
    }
    actor = &lbl_80478ECC[actorIndex];
    if (actor->resource == 0) {
        return -1;
    }
    model = floorGetResource(actor->resource);
    if (model == 0) {
        return -1;
    }
    groupId = floorDataBiosGetGroupID(floorDataBiosGetPtr(floorDataId));

    if (direction >= 2 && direction <= 5) {
        fn_801845E4(0, 0x64, groupId, actor->resource, actor->partIndex);
        fn_801845E4(0, 0x65, groupId, actor->resource, actor->partIndex);
        switch (direction) {
        case 2:
            fn_801860F8(0, 0x64, lbl_8047E114, lbl_8047E114,
                        lbl_8047E130);
            fn_801860F8(0, 0x65, lbl_8047E114, lbl_8047E114,
                        lbl_8047E134);
            break;
        case 3:
            fn_801860F8(0, 0x64, lbl_8047E114, lbl_8047E114,
                        lbl_8047E134);
            fn_801860F8(0, 0x65, lbl_8047E114, lbl_8047E114,
                        lbl_8047E130);
            break;
        case 4:
            fn_801860F8(0, 0x64, lbl_8047E130, lbl_8047E114,
                        lbl_8047E114);
            fn_801860F8(0, 0x65, lbl_8047E134, lbl_8047E114,
                        lbl_8047E114);
            break;
        case 5:
            fn_801860F8(0, 0x64, lbl_8047E134, lbl_8047E114,
                        lbl_8047E114);
            fn_801860F8(0, 0x65, lbl_8047E130, lbl_8047E114,
                        lbl_8047E114);
            break;
        }
        fn_80188AF4(0, 0x65);
        START_MODEL_ANIM(mode, lbl_8047E114);
        return 0;
    }

    mode = (u16)mode;
    if (mode == 1 || mode == 2 || mode == 0x81 || mode == 0x82) {
        START_MODEL_ANIM(actor->enterAnim, frame);
        if ((mode & 0x80) != 0) {
            fn_80166A28(0x44);
        }
        WAIT_MODEL();

        part = GSmodelGetPart(model, actor->partIndex);
        GSpartGetTransform(part, &transform, 0, 0);
        GSpartFree(part);

        if ((mode & 1) != 0) {
            fn_80188AF4(0, 0x65);
            transform.position.z -= lbl_8047E130;
            fn_8018AACC(0, 0x64, 1, &transform.position);
            firstPos.x = transform.position.x;
            firstPos.y = transform.position.y;
            firstPos.z = transform.position.z + lbl_8047E138;
            WAIT_SECONDS(lbl_8047E13C);

            if (lbl_8047B3C0 != 0) {
                for (i = 0; i < lbl_8047B3C0; i++) {
                    fn_8018AACC(0, 0x65, 1, &lbl_80467090[i]);
                    WAIT_ACTOR65(timedOut);
                    if (timedOut != 0) {
                        break;
                    }
                }
            }
            if (timedOut == 0) {
                fn_8018AACC(0, 0x65, 1, &firstPos);
            }
            peopleMoveCheck(0, 0x64, 1);
            if (timedOut == 0) {
                WAIT_ACTOR65(timedOut);
            }
            fn_8018805C(0, 0x64, lbl_8047E114, lbl_8047E108);
            if (timedOut == 0) {
                fn_8018805C(0, 0x65, lbl_8047E114, lbl_8047E108);
            }
            peopleMoveCheck(0, 0x64, 1);
            if (timedOut == 0) {
                WAIT_ACTOR65(timedOut);
            }
        } else {
            fn_80184470(0, 0x64);
            fn_80184470(0, 0x65);
            fn_80188F78(0, 0x65);

            part = GSmodelGetPart(model, actor->partIndex);
            GSpartGetTransform(part, &transform, 0, 0);
            GSpartFree(part);
            firstPos = transform.position;
            secondPos = transform.position;
            firstPos.z += lbl_8047E130;
            secondPos.z -= lbl_8047E130;
            fn_8018C0A8(0, 0x64, &firstPos);
            fn_8018C0A8(0, 0x65, &secondPos);
            firstPos.z += lbl_8047E140;
            secondPos.z += lbl_8047E140;
            fn_8018AACC(0, 0x64, 1, &firstPos);
            WAIT_SECONDS(lbl_8047E13C);
            fn_8018AACC(0, 0x65, 1, &secondPos);
            peopleMoveCheck(0, 0x64, 1);
            peopleMoveCheck(0, 0x65, 1);
            lbl_8047B3C4 = 0;
        }

        START_MODEL_ANIM(actor->exitAnim, frame);
        if ((mode & 0x80) != 0) {
            fn_80166A28(0x44);
        }
        WAIT_MODEL();
    } else if (mode == 0xC0) {
        WAIT_MODEL();
        if ((mode & 0x80) != 0) {
            fn_801669BC(0x45);
            fn_80166A28(0x46);
        }
    } else if (mode != 0x100) {
        if ((mode & 4) != 0) {
            animIndex = actor->actionAnimA;
        } else if ((mode & 8) != 0) {
            animIndex = actor->actionAnimB;
        }
        if (animIndex < 0 || GSmodelCanAnimate(model) == 0) {
            return -1;
        }

        if ((mode & 0x20) != 0) {
            fn_801845E4(0, 0x64, groupId, actor->resource,
                        actor->partIndex);
            fn_801845E4(0, 0x65, groupId, actor->resource,
                        actor->partIndex);
            if (direction != 0) {
                fn_801860F8(0, 0x64, lbl_8047E114, lbl_8047E114,
                            lbl_8047E130);
                fn_801860F8(0, 0x65, lbl_8047E114, lbl_8047E114,
                            lbl_8047E134);
            } else {
                fn_801860F8(0, 0x64, lbl_8047E114, lbl_8047E114,
                            lbl_8047E134);
                fn_801860F8(0, 0x65, lbl_8047E114, lbl_8047E114,
                            lbl_8047E130);
            }
            fn_80188AF4(0, 0x65);
        }
        if ((mode & 0x10) != 0) {
            GSmodelSetAnimIndex(model, animIndex);
            GSmodelGetFrameCount(model, &frame, 0);
            frame -= lbl_8047E100;
        }
        START_MODEL_ANIM(animIndex, frame);
        if ((mode & 0x40) != 0) {
            fn_801C8E14(floorDataId, actorIndex, 0xC0, direction);
        } else if ((mode & 0x80) != 0) {
            fn_80166A28(0x45);
        }
    }

    GSmodelGetFrameCount(model, &frame, 0);
    return (s32)(lbl_8047E144 *
                 ((lbl_8047E148 * frame) / (f32)fn_800D37CC()));

#undef WAIT_ACTOR65
#undef WAIT_SECONDS
#undef WAIT_MODEL
#undef START_MODEL_ANIM
}
#pragma peephole reset
