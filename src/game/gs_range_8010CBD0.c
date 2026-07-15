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

/* 0x8010CC04 | 0x50 */
s32 GScolsys2UnloadCCD(void) {
    extern void GSgfxDLFree(void* displayList);

    COL_STATE->wzxDataPtr = NULL;
    if (COL_STATE->displayList != NULL) {
        GSgfxDLFree(COL_STATE->displayList);
        COL_STATE->displayList = NULL;
    }
    return 1;
}

/* 0x8010CD6C | 0x98 */
void fn_8010CD6C(void) {
    typedef struct TriData {
        Vec3f vectors[3];
    } TriData;
    void* wzx = COL_STATE->wzxDataPtr;
    u32* wzxHeader;
    u32 i;
    u32* srcTri;
    u8* dstEntry;

    if (wzx == NULL) {
        return;
    }

    wzxHeader = (u32*)wzx;
    i = 0;
    srcTri = (u32*)wzxHeader[0];
    dstEntry = (u8*)COL_STATE + COL_LAYER_IDX * GSCOLSYS_LAYER_SIZE + 4;

    for (; i < wzxHeader[1]; i++) {
        ((TriData*)dstEntry)->vectors[0] = ((TriData*)srcTri)->vectors[0];
        ((TriData*)dstEntry)->vectors[1] = ((TriData*)srcTri)->vectors[1];
        ((TriData*)dstEntry)->vectors[2] = ((TriData*)srcTri)->vectors[2];
        *(u16*)(dstEntry + 0x24) = 0;

        srcTri = (u32*)((u8*)srcTri + 0x40);
        dstEntry += GSCOLSYS_TRI_ENTRY_SIZE;
    }
}

/* 0x8010CFE4 | 0x54 */
s32 fn_8010CFE4(void* ccdFile) {
    extern void _offsetCCD__FP12CCD_FILEHEAD(void* ccdFile);

    if (COL_LAYER_IDX < 0) {
        return 0;
    }

    _offsetCCD__FP12CCD_FILEHEAD(ccdFile);
    COL_STATE->wzxDataPtr = ccdFile;
    return 1;
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

/* 0x8010D170 | 0x9C */
void fn_8010D170(void) {
    extern void* fn_800D7894(void);
    extern void fn_800D7868(void* handle, u32 a, u32 b, u32 c,
                            u32 d, u32 e, u32 f, u32 g);

    COL_STATE->activeLayer = 0;
    COL_STATE->displayList = NULL;
    COL_STATE->gfxRenderHandle = (u32)fn_800D7894();
    fn_800D7868((void*)COL_STATE->gfxRenderHandle, 1, 0, 1, 4, 0, 0, 0);
    fn_800D7868((void*)COL_STATE->gfxRenderHandle, 4, 0, 6, 10, 0, 0, 0);
    COL_STATE->displayList = NULL;
}

/* 0x8010DE00 | 0xF0 */
s32 GScolsys2WalkGetLayer(Vec3f* position, u8* layer, u8* material) {
    typedef struct WalkHit {
        f32 height;
        u32 unk4;
        u8 layer;
        u8 material;
        u8 pad[2];
    } WalkHit;
    WalkHit hits[8];
    s32 count;
    s32 i;
    s32 nearest;
    f32 distance;
    f32 nearestDistance;
    extern s32 fn_8010E138(Vec3f* position, WalkHit* hits);
    extern const f32 lbl_8047CEE0;

    count = fn_8010E138(position, hits);
    if (count <= 0) {
        return 0;
    }

    distance = position->y - hits[0].height;
    distance = distance > lbl_8047CEE0 ? distance : -distance;
    nearestDistance = distance;
    nearest = 0;

    for (i = 1; i < count; i++) {
        distance = position->y - hits[i].height;
        distance = distance > lbl_8047CEE0 ? distance : -distance;
        if (nearestDistance > distance) {
            nearest = i;
            nearestDistance = distance;
        }
    }

    *layer = hits[nearest].layer;
    *material = hits[nearest].material;
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
s32 GScolsys2UtilGetCpPlaneLine(Vec3f* out, f32* tOut, Vec3f* normal,
                               Vec3f* planePoint, Vec3f* lineStart,
                               Vec3f* lineEnd) {
    Vec3f direction;
    f32 denominator;
    f32 t;
    extern void PSVECSubtract(void*, void*, void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);

    PSVECSubtract(lineEnd, lineStart, &direction);
    denominator = normal->x * direction.x + normal->y * direction.y
                + normal->z * direction.z;
    if (denominator == 0.0f) {
        return 0;
    }

    t = (normal->x * (planePoint->x - lineStart->x)
       + normal->y * (planePoint->y - lineStart->y)
       + normal->z * (planePoint->z - lineStart->z)) / denominator;
    PSVECScale(&direction, &direction, t);
    PSVECAdd(&direction, lineStart, out);
    *tOut = t;
    return 1;
}

/* 0x8010F5A4 | 0xFC */
f32 GScolsys2UtilGetCpLinePoint(Vec3f* out, Vec3f* start, Vec3f* end,
                               Vec3f* point) {
    Vec3f direction;
    f32 lengthSquared;
    f32 t;
    extern void PSVECSubtract(void*, void*, void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);
    extern const f32 lbl_8047CF10;

    PSVECSubtract(end, start, &direction);
    lengthSquared = direction.x * direction.x + direction.y * direction.y
                  + direction.z * direction.z;
    if (lbl_8047CF10 == lengthSquared) {
        out->x = start->x;
        out->y = start->y;
        out->z = start->z;
        return lbl_8047CF10;
    }

    t = (direction.x * (point->x - start->x)
       + direction.y * (point->y - start->y)
       + direction.z * (point->z - start->z)) / lengthSquared;
    PSVECScale(&direction, &direction, t);
    PSVECAdd(&direction, start, out);
    return t;
}
