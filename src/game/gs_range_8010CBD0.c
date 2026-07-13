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
