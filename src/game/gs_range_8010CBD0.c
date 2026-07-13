/**
 * @file gs_range_8010CBD0.c
 * @brief gs-engine code, 0x8010CBD0 - 0x8010F6A0 (22 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 *
 * Boundary bug fix: the 8 functions below (0x8010E138-0x8010F6A0)
 * were physically sitting in game/gs_field_colquery.c -- outside that
 * unit's own splits.txt range (0x8010F6A0-0x801140DC) -- while this
 * unit's declared range already covers them. Relocated here during
 * the gs_field_colquery.c 6-way split so this unit scores real
 * progress instead of 0%. fn_8010E138/E53C/EB28/EFE4/F188 previously
 * carried invented "GSfield_RayCast/SphereSweep/SweepAgainstMesh/
 * LinePlaneTest/ArcTest" names/signatures from an earlier bad
 * campaign pass (same class of issue documented in
 * include/game/gs_colsys.h); reverted to the standard fn_<addr>
 * placeholder since no confirmed symbols.txt name exists yet for any
 * of them. fn_8010F4B8 and fn_8010F5A4 likewise carried invented
 * names ("GSfield_..." helpers that were never actually called under
 * those names); renamed to their confirmed symbols.txt names below.
 */
#include "dolphin/types.h"
#include "game/gs_colsys.h"

/* ===================================================================
 * External state and convenience aliases shared with the other colsys units
 * =================================================================== */
extern GSColSysState lbl_80404C68;

#define COL_STATE (&lbl_80404C68)
#define COL_LAYER_IDX (COL_STATE->activeLayer)
#define COL_LAYER_PTR(n) ((void*)((u8*)COL_STATE + 4 + (n) * GSCOLSYS_LAYER_SIZE))

/* 0x8010CBD0 | 0x34 */
void* GScolsys2GetCurFloor(void) {
    s32 layer;

    layer = COL_LAYER_IDX;
    if (layer < 0 || layer >= GSCOLSYS_MAX_LAYERS) {
        return NULL;
    }
    return COL_LAYER_PTR(layer);
}

/* 0x8010D038 | 0x2C */
s32 fn_8010D038(void) {
    s32 layer;

    layer = COL_LAYER_IDX;
    if (layer < 0) {
        return 0;
    }
    COL_LAYER_IDX = layer - 1;
    return 1;
}

typedef f32 ColVec3[3];
typedef f32 ColMtx[3][4];

typedef struct ColDrawGroup {
    u8* data;
    u32 count;
} ColDrawGroup;

typedef struct ColDrawObject {
    u8 pad_00[0x24];
    void* model;
    ColDrawGroup* edgeGroup0;
    ColDrawGroup* faceGroup0;
    ColDrawGroup* faceGroup1;
    ColDrawGroup* edgeGroup1;
    ColDrawGroup* faceGroup2;
    u16 flags;
    u8 pad_3E[2];
} ColDrawObject;

typedef struct ColDrawScene {
    ColDrawObject* objects;
    u32 count;
} ColDrawScene;

typedef union ColDrawColor {
    u32 packed;
    struct {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } channel;
} ColDrawColor;

extern ColDrawScene* fn_8010CBC0(void);
extern void fn_800DA028(s32);
extern void fn_800D7820(void*);
extern void fn_800D88DC(s32);
extern void fn_800D888C(s32);
extern void fn_800DA4C4(s32, s32, s32);
extern void fn_800DA1E8(s32, s32, s32);
extern void fn_800D9ED8(s32);
extern void fn_8010CA30(ColMtx out, u32 index);
extern void fn_8010C8D0(ColMtx out, u32 index);
extern void fn_8010D20C(void*, ColMtx, ColMtx);
extern void PSMTXMultVec(ColMtx, const ColVec3, ColVec3);
extern void fn_800D6A00(s32);
extern void fn_800D67BC(s32);
extern void fn_800D6680(f32, f32, f32);
extern void fn_800D5CB8(s32, u8, u8, u8, u8);
extern void fn_800D6728(void);
extern void* GScolsys2Draw(void);
extern void GSgfxDLDraw(void*);
extern void fn_800D30AC(void);
extern u32 lbl_8047CEB8;
extern u32 lbl_8047CEBC;
extern u32 lbl_8047CEC0;
extern u32 lbl_8047CEC4;

static inline void ColDrawSetColor(ColDrawColor color)
{
    fn_800D5CB8(0, color.channel.r, color.channel.g,
                color.channel.b, color.channel.a);
}

static inline void ColDrawEdges(ColMtx matrix, ColDrawGroup* group,
                                ColDrawColor color, u32 stride)
{
    ColVec3 transformed[3];
    u8* element;
    u32 i;
    s32 vertex;
    s32 next;

    if (group == NULL) {
        return;
    }
    element = group->data;
    for (i = 0; i < group->count; i++, element += stride) {
        for (vertex = 0; vertex < 3; vertex++) {
            PSMTXMultVec(matrix, *(ColVec3*)(element + vertex * 12),
                         transformed[vertex]);
        }
        fn_800D6A00(1);
        for (vertex = 0; vertex < 3; vertex++) {
            next = vertex + 1;
            if (next >= 3) {
                next = 0;
            }
            fn_800D67BC(2);
            fn_800D6680(transformed[vertex][0], transformed[vertex][1],
                        transformed[vertex][2]);
            ColDrawSetColor(color);
            fn_800D6680(transformed[next][0], transformed[next][1],
                        transformed[next][2]);
            ColDrawSetColor(color);
            fn_800D6728();
        }
    }
}

static inline void ColDrawFaces(ColMtx matrix, ColDrawGroup* group,
                                ColDrawColor color, u32 stride)
{
    ColVec3 transformed;
    u8* element;
    u32 i;
    s32 vertex;

    if (group == NULL) {
        return;
    }
    element = group->data;
    fn_800D6A00(3);
    for (i = 0; i < group->count; i++, element += stride) {
        fn_800D67BC(3);
        for (vertex = 0; vertex < 3; vertex++) {
            PSMTXMultVec(matrix, *(ColVec3*)(element + vertex * 12),
                         transformed);
            fn_800D6680(transformed[0], transformed[1], transformed[2]);
            ColDrawSetColor(color);
        }
        fn_800D6728();
    }
}

/* Record the collision-debug geometry into a display list. */
#pragma push
#pragma inline_depth(8)
#pragma inline_max_size(10000)
void* GScolsys2Draw(void)
{
    extern u8 GSgfxDLBegin(void* buffer, u32 size);
    extern void* GSgfxDLEnd(void);
    extern s32 printf(const char*, ...);
    extern const char lbl_80272050[];
    ColDrawScene* scene;
    ColDrawObject* object;
    ColMtx matrix;
    ColMtx normalMatrix;
    ColDrawColor color;
    u32 i;

    scene = fn_8010CBC0();
    if (scene == NULL) {
        return NULL;
    }

    fn_800DA028(1);
    fn_800D7820(*(void**)((u8*)&lbl_80404C68 + 0x3708));
    fn_800D88DC(1);
    fn_800D888C(6);
    fn_800DA4C4(1, 6, 7);
    fn_800DA1E8(1, 2, 1);
    fn_800D9ED8(0);

    if (GSgfxDLBegin(*(void**)((u8*)&lbl_80404C68 + 0x3708), 0x80000) == 0) {
        printf(lbl_80272050);
        return NULL;
    }

    object = scene->objects;
    for (i = 0; i < scene->count; i++, object++) {
        if ((object->flags & 1) != 0) {
            continue;
        }

        fn_8010CA30(matrix, i);
        fn_8010C8D0(normalMatrix, i);
        if (object->model != NULL) {
            fn_8010D20C(object->model, matrix, normalMatrix);
        }

        color.packed = lbl_8047CEB8;
        ColDrawEdges(matrix, object->edgeGroup0, color, 0x34);
        color.packed = lbl_8047CEBC;
        ColDrawFaces(matrix, object->faceGroup0, color, 0x34);
        color.packed = lbl_8047CEC0;
        ColDrawFaces(matrix, object->faceGroup1, color, 0x34);
        color.packed = lbl_8047CEB8;
        ColDrawEdges(matrix, object->edgeGroup1, color, 0x34);
        color.packed = lbl_8047CEC4;
        ColDrawFaces(matrix, object->faceGroup2, color, 0x30);
    }

    return GSgfxDLEnd();
}
#pragma pop

#pragma push
#pragma inline_depth(8)
#pragma inline_max_size(10000)
void fn_8010D8D4(void)
{
    ColDrawScene* scene;
    ColDrawObject* object;
    ColMtx matrix;
    ColMtx normalMatrix;
    ColDrawColor color;
    u8* state;
    u8* layer;
    void* displayList;
    u32 activeLayer;
    u32 i;

    scene = fn_8010CBC0();
    if (scene == NULL) {
        return;
    }

    fn_800DA028(1);
    state = (u8*)&lbl_80404C68;
    fn_800D7820(*(void**)(state + 0x3708));
    fn_800D88DC(1);
    fn_800D888C(6);
    fn_800DA4C4(1, 6, 7);
    fn_800DA1E8(1, 2, 1);
    fn_800D9ED8(0);

    object = scene->objects;
    activeLayer = *(u32*)(state + 0x3704);
    layer = state + activeLayer * 0xDC0 + 4;
    for (i = 0; i < scene->count; i++, object++, layer += 0x28) {
        if ((*(u16*)(layer + 0x24) & 1) != 0 ||
            (object->flags & 1) == 0) {
            continue;
        }

        fn_8010CA30(matrix, i);
        fn_8010C8D0(normalMatrix, i);
        if (object->model != NULL) {
            fn_8010D20C(object->model, matrix, normalMatrix);
        }

        color.packed = lbl_8047CEB8;
        ColDrawEdges(matrix, object->edgeGroup0, color, 0x34);
        color.packed = lbl_8047CEBC;
        ColDrawFaces(matrix, object->faceGroup0, color, 0x34);
        color.packed = lbl_8047CEC0;
        ColDrawFaces(matrix, object->faceGroup1, color, 0x34);
        color.packed = lbl_8047CEB8;
        ColDrawEdges(matrix, object->edgeGroup1, color, 0x34);
        color.packed = lbl_8047CEC4;
        ColDrawFaces(matrix, object->faceGroup2, color, 0x30);
    }

    displayList = *(void**)(state + 0x370C);
    if (displayList == NULL) {
        displayList = GScolsys2Draw();
        *(void**)(state + 0x370C) = displayList;
    }
    if (displayList != NULL) {
        GSgfxDLDraw(displayList);
        fn_800D30AC();
    }
}
#pragma pop

/* 0x8010E138 | 0x404 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_8010E138(void* origin, void* direction) {
    /* TODO: match -- 824 bytes at 0x8010E138 */
}
#pragma pop

/* 0x8010E53C | 0x5EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_8010E53C(void* origin, void* direction, f32 radius) {
    /* TODO: match -- 1516 bytes at 0x8010E53C */
}
#pragma pop

/* 0x8010EB28 | 0x4BC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_8010EB28(void* meshData, void* sweep, void* result) {
    /* TODO: match -- 1212 bytes at 0x8010EB28 */
}
#pragma pop

/* 0x8010EFE4 | 0x1A4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_8010EFE4(void* segStart, void* segEnd,
                 void* planeNormal, s32 doubleSided) {
    /* TODO: match -- 420 bytes at 0x8010EFE4 */
}
#pragma pop

/* 0x8010F188 | 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_8010F188(void* center, f32 radius, void* result) {
    /* TODO: match -- 408 bytes at 0x8010F188 */
}
#pragma pop

/* 0x8010F320 | 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010F320(void) {
    /* TODO: match -- 408 bytes at 0x8010F320 */
}
#pragma pop

/* 0x8010F4B8 | 0xEC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GScolsys2UtilGetCpPlaneLine(void* a, void* b, void* c, void* d, void* e, void* f) {
    /* TODO: match -- 236 bytes at 0x8010F4B8 */
}
#pragma pop

/* 0x8010F5A4 | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
f32 GScolsys2UtilGetCpLinePoint(void* out, void* start, void* end, void* point) {
    /* TODO: match -- 252 bytes at 0x8010F5A4 */
}
#pragma pop
