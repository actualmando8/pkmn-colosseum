/**
 * @file gs_field_colquery.c
 * @brief GSfield collision queries -- ray casts, sphere sweeps, plane tests.
 *
 * Decompiled from:
 *   fn_8010E138 (GSfield_RayCast)
 *   fn_8010E53C (GSfield_SphereSweep)
 *   fn_8010EB28 (GSfield_SweepAgainstMesh)
 *   fn_8010EFE4 (GSfield_LinePlaneTest)
 *   fn_8010F188 (GSfield_ArcTest)
 *   fn_8010F320 (GSfield_ArcTestReverse)
 *   fn_8010F4B8 (GSfield_PointInBounds)
 *   fn_8010F5A4 (GSfield_TriangleBarycentric)
 *   fn_8010F6A0 (GSfield_GetNearestTriangle)
 *   fn_8010F71C (GSfield_FindGroundHeight)
 *   fn_8010FA54 (GSfield_QueryGroundType)
 *   fn_8010FAF4 (GSfield_MultiRayCast)
 *   fn_8010FDF8 (GSfield_RayTriangleIntersect)
 *   fn_8010FFC4 (GSfield_SegmentTriTest)
 *   fn_80110084 (GSfield_TriNormalFromVerts)
 *   fn_801101B4 (GSfield_BuildCollisionGrid)
 *   fn_8011069C (GSfield_GridLookup)
 *   fn_80110E64 (GSfield_WalkableSurfaceQuery)
 *   fn_80111470 (GSfield_FloorHeightAtPoint)
 *   fn_8011163C (GSfield_HeightmapSample)
 *   fn_80111864 (GSfield_TerrainProbe)
 *   GScolsys2CheckGetEventID (GSfield_IsPointOnFloor)
 *   fn_80111C24 (GSfield_RegionBoundsTest)
 *   fn_80111DF8 (GSfield_ClipToFloorBounds)
 *   fn_80111F2C (GSfield_ProjectPointToFloor)
 *   fn_8011207C (GSfield_FindSpawnPoint)
 *   fn_80112260 (GSfield_GetSpawnPosition)
 *   floorCheckFightKind (GSfield_CheckReady)
 *   fn_801123D4 (GSfield_ResourceInit)
 *   fn_80112700 (GSfield_GetActiveFloorId)
 *   floorCheckFade (GSfield_FinalizeLoad)
 *   fn_80112780 (GSfield_BeginTransition)
 *   fn_801127BC (GSfield_SetTransitionCallback)
 *   fn_80112844 (GSfield_GetTransitionState)
 *   fn_8011288C (GSfield_IsTransitioning)
 *   fn_801128A0 (GSfield_ProcessTransition)
 *   fn_801129AC (GSfield_GetFieldState)
 *   fn_801129CC (GSfield_UpdateObjects)
 *   fn_80112F8C (GSfield_ClearObjectList)
 *   fn_80112FEC (GSfield_AddObject)
 *   _floorInitCharacters__FP11GSfloor_dd_ (GSfield_RemoveObject)
 *   floorInitMap (GSfield_MoveObject)
 *   fn_80113778 (GSfield_SetObjectVisible)
 *   fn_80113828 (GSfield_GetObjectPosition)
 *   fn_8011388C (GSfield_SetObjectPosition)
 *   fn_8011392C (GSfield_GetObjectRotX)
 *   fn_8011393C (GSfield_GetObjectRotY)
 *   fn_8011394C (GSfield_GetObjectRotZ)
 *   fn_8011395C (GSfield_GetObjectScale)
 *   fn_8011396C (GSfield_SetObjectRotation)
 *   fn_801139BC (GSfield_SetObjectScale)
 *   fn_80113A0C (GSfield_SpawnFieldModel)
 *   fn_80113B84 (GSfield_DespawnFieldModel)
 *   fn_80113D10 (GSfield_GetModelGroup)
 *   fn_80113D34 (GSfield_GetModelIndex)
 *   floorOpenObject (GSfield_LoadFieldModel)
 *   fn_80113F48 (GSfield_UnloadFieldModel)
 *   floorGetResource (GSfield_SetModelAnimation)
 *   fn_80113FB4 (GSfield_GetModelAnimFrame)
 *   fn_80113FE8 (GSfield_PlayModelAnimation)
 *   fn_801140C8 (GSfield_StopModelAnimation)
 *   fn_801140DC (GSfield_SetModelMaterial)
 *   fn_8011416C (GSfield_GetModelMaterial)
 *   fn_8011418C (GSfield_SetModelTexture)
 *   fn_801141D8 (GSfield_GetModelTexture)
 *   EvlogSet__FScUl (GSfield_AttachToJoint)
 *   fn_80114254 (GSfield_DetachFromJoint)
 *   fn_801142B4 (GSfield_GetJointPosition)
 *   floorReadGFLPostFunc (table 0x11 PostFunc; old GSfield_GetJointCount attribution was fiction)
 *
 * This module wraps GScolsys2 collision queries for use by the field/world
 * system. It transforms world-space coordinates through the collision mesh
 * matrix, performs triangle intersection tests, and returns hit results
 * sorted by distance.
 *
 * Key patterns observed in the disassembly:
 *   - Heavy use of PSMTXMultVec (MTXMultVec3) for coordinate transforms
 *   - Calls to GScolsys2GetObjEnable (GScolsys2_QueryTriVisible) for per-tri checks
 *   - Calls to fn_8010CA30 / fn_8010C8D0 for forward/inverse transforms
 *   - Calls to fn_8010DEF0 (GScolsys2_TriangleBoundsCheck)
 *   - Float comparisons with parametric t values (ray intersection)
 *   - Iteration over triangle vertex arrays (stride 0x34 per triangle)
 *   - Result arrays capped at 8 entries (GSFIELD_MAX_QUERY_RESULTS)
 *
 * Address range: 0x8010E138 - 0x80114300
 */

#include "dolphin/types.h"
#include "game/world/gs_field.h"

/* ===== External SDK / engine functions ===== */
extern void  GSlogWrite(const char* fmt, ...);        /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);

/* Matrix / vector math helpers */
extern void  PSMTXMultVec(void* mtxDst, void* vecSrc, void* vecDst);  /* MTXMultVec3 */
extern void  PSVECAdd(void* vecA, void* scale, void* vecOut);     /* VEC scale */
extern void  PSVECSubtract(void* a, void* b, void* out);             /* VEC diff/setup */
extern f32   PSVECMag(void* vec);                                /* VEC magnitude */
extern void  PSVECScale(void* curve, void* paramOut, f32 t);       /* VEC lerp */
f32   PSVECDotProduct(void* a, void* b);
f32   PSVECSquareDistance(void* a, void* b);

/* GScolsys2 functions */
extern void* fn_8010CBC0(void);                         /* GScolsys2_GetWZXData */
extern s32   GScolsys2GetObjEnable(u32 triIdx, void* outFlag);    /* GScolsys2_QueryTriVisible */
extern void  fn_8010CA30(void* mtxOut, u32 layerIdx);   /* GScolsys2_BuildInverseTransform */
extern void  fn_8010C8D0(void* mtxOut, u32 layerIdx);   /* GScolsys2_BuildTransform */
extern s32   fn_8010DEF0(void* result, void* origin,
                          void* verts, void* normal);    /* GScolsys2_TriangleBoundsCheck */
f32   GScolsy2UtilGetSidePlanePoint(void* normal, void* p1, void* p2);
void  GScolsy2UtilGetCpPlanePoint(void* out, void* normal, void* verts, void* point);
f32   GScolsys2UtilGetCpLinePoint(void* out, void* start, void* end, void* point);
s32   GScolsy2UtilChkInTri(void* point, void* verts, void* normal);

/* Collision triangle/query record layouts used by this module. */
typedef struct GScolsys2Vec3 {
    f32 x;
    f32 y;
    f32 z;
} GScolsys2Vec3;

typedef struct GScolsys2Triangle {
    GScolsys2Vec3 verts[3];      /* 0x00 */
    GScolsys2Vec3 normal;        /* 0x24 */
    u16 flags;                   /* 0x30 */
    u16 id;                      /* 0x32 */
} GScolsys2Triangle;

typedef struct GScolsys2TriangleList {
    GScolsys2Triangle* triangles; /* 0x00 */
    u32 count;                    /* 0x04 */
} GScolsys2TriangleList;

typedef struct GSfieldQueryTriangle {
    GScolsys2Vec3 verts[3];      /* 0x00 */
    GScolsys2Vec3 normal;        /* 0x24 */
    u16 id;                      /* 0x30 */
    u16 pad_32;                  /* 0x32 */
} GSfieldQueryTriangle;

typedef struct GSfieldEdgeMasks {
    u16 values[3];
} GSfieldEdgeMasks;

extern const GSfieldEdgeMasks lbl_8047CF48;
extern const GSfieldEdgeMasks lbl_8047CF50;

/* ===== String constants (rodata) ===== */
extern const char lbl_802720B0[]; /* "scene_data" */

/* ===== BSS / global state ===== */
extern u8 lbl_80404C68[];  /* GScolsys2 collision state */
extern u8 lbl_80408378[];

#define GSFIELD_COL_RESERVATION_COUNT 0x30
#define GSFIELD_COL_RESERVATION_IN_USE 0x0001
#define GSFIELD_COL_RESERVATION_DISABLED 0x0002

typedef struct GSFieldVec3f {
    f32 x;
    f32 y;
    f32 z;
} GSFieldVec3f;

typedef struct GSFieldColReservation {
    /* 0x00 */ s32 field_00;
    /* 0x04 */ s32 field_04;
    /* 0x08 */ f32 field_08;
    /* 0x0C */ f32 field_0C;
    /* 0x10 */ u16 flags;
    /* 0x12 */ u16 pad_12;
} GSFieldColReservation;

typedef struct GSFieldColLayerView {
    /* 0x000 */ u8 pad_000[0xA00];
    /* 0xA00 */ GSFieldColReservation reservations[GSFIELD_COL_RESERVATION_COUNT];
} GSFieldColLayerView;

typedef struct GSFieldWzxTriangleList {
    /* 0x00 */ void* triangles;
    /* 0x04 */ u32 triangleCount;
} GSFieldWzxTriangleList;

typedef struct GSFieldWzxRegion {
    /* 0x00 */ u8 pad_00[0x30];
    /* 0x30 */ GSFieldWzxTriangleList* floorTriangles;
    /* 0x34 */ u8 pad_34[0x04];
    /* 0x38 */ GSFieldWzxTriangleList* boundaryTriangles;
    /* 0x3C */ u8 pad_3C[0x04];
} GSFieldWzxRegion;

typedef struct GSFieldWzxData {
    /* 0x00 */ GSFieldWzxRegion* regions;
    /* 0x04 */ u32 regionCount;
} GSFieldWzxData;

typedef struct GSFieldWzxCompactTriangle {
    /* 0x00 */ GSFieldVec3f vertices[3];
    /* 0x24 */ GSFieldVec3f normal;
} GSFieldWzxCompactTriangle;

typedef struct GSFieldColqueryState {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u8 field_08;
    /* 0x09 */ u8 pad_09[0x03];
    /* 0x0C */ u32 field_0C;
    /* 0x10 */ u8 pad_10[0x18];
    /* 0x28 */ u32 transitionPollCallback;
    /* 0x2C */ u32 transitionBeginCallback;
    /* 0x30 */ u8 pad_30[0x0C];
    /* 0x3C */ u32 texture;
    /* 0x40 */ u32 texturePalette;
    /* 0x44 */ u8 textureSlot;
    /* 0x45 */ u8 pad_45[0x03];
    /* 0x48 */ u32 material;
    /* 0x4C */ u32 materialPalette;
    /* 0x50 */ u8 materialSlot;
    /* 0x51 */ u8 usePendingMaterial;
} GSFieldColqueryState;

/* ==================================================================
 * fn_8010E138 -- GSfield_RayCast
 *
 * Cast a ray against the active collision mesh. For each visible
 * triangle (checked via GScolsys2_QueryTriVisible), transforms
 * the triangle vertices through the inverse model-view matrix,
 * then tests ray-triangle intersection.
 *
 * Results are stored in a sorted array of GSFieldHitResult structs,
 * capped at 8 entries. The function returns the number of hits.
 *
 * Register usage (from disasm):
 *   r3 = origin (Vec3f*)
 *   r4 = direction (Vec3f* -- also used as extent)
 *   r25 = WZX data pointer (from fn_8010CBC0)
 *   r26 = triangle vertex stride 0x34
 *   r24 = hit count
 *   r28 = current triangle index
 *   f1  = parametric t for current hit
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSfield_RayCast(void* origin, void* direction) {
    /* TODO: match -- 824 bytes at 0x8010E138 */
    /* Calls: fn_8010CBC0, GScolsys2GetObjEnable, fn_8010CA30, fn_8010C8D0,
     *        PSMTXMultVec, fn_8010DEF0 */
}
#pragma pop

/* ==================================================================
 * fn_8010E53C -- GSfield_SphereSweep
 *
 * Swept-sphere collision test (1516 bytes). Larger than RayCast
 * because it expands each triangle by the sphere radius before
 * performing the intersection test.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSfield_SphereSweep(void* origin, void* direction, f32 radius) {
    /* TODO: match -- 1516 bytes at 0x8010E53C */
}
#pragma pop

/* ==================================================================
 * fn_8010EB28 -- GSfield_SweepAgainstMesh
 *
 * Lower-level sweep test against a specific collision mesh subset.
 * 1212 bytes, heavy float math.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSfield_SweepAgainstMesh(void* meshData, void* sweep, void* result) {
    /* TODO: match -- 1212 bytes at 0x8010EB28 */
}
#pragma pop

/* ==================================================================
 * fn_8010EFE4 -- GSfield_LinePlaneTest
 *
 * Test a line segment against a collision plane.
 * 420 bytes. Used by both RayCast and arc tests.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSfield_LinePlaneTest(void* segStart, void* segEnd,
                           void* planeNormal, s32 doubleSided) {
    /* TODO: match -- 420 bytes at 0x8010EFE4 */
}
#pragma pop

/* ==================================================================
 * fn_8010F188 -- GSfield_ArcTest
 *
 * Sweep test along a circular arc. Used for character movement on
 * curved surfaces. 408 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSfield_ArcTest(void* center, f32 radius, void* result) {
    /* TODO: match -- 408 bytes at 0x8010F188 */
}
#pragma pop

/* ==================================================================
 * fn_8010F71C -- GSfield_FindGroundHeight
 *
 * Find the ground height at a given XZ position by casting a
 * vertical ray downward. 824 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
f32 GSfield_FindGroundHeight(f32 x, f32 z, void* resultInfo) {
    /* TODO: match -- 824 bytes at 0x8010F71C */
}
#pragma pop

/* ==================================================================
 * fn_801101B4 -- GSfield_BuildCollisionGrid
 *
 * Build a spatial acceleration grid over the collision mesh for
 * faster point queries. 1256 bytes -- one of the larger functions.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSfield_BuildCollisionGrid(void* meshData) {
    /* TODO: match -- 1256 bytes at 0x801101B4 */
}
#pragma pop

/* ==================================================================
 * fn_8011069C -- GSfield_GridLookup
 *
 * Look up collision triangles in the spatial grid for a given
 * world-space position. 1992 bytes -- very large function.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSfield_GridLookup(f32 x, f32 z, void* outTriangles) {
    /* TODO: match -- 1992 bytes at 0x8011069C */
}
#pragma pop

/* ==================================================================
 * fn_801123D4 -- GSfield_ResourceInit
 *
 * Initialize the field resource system for a new floor.
 * Sets up resource slot table, callback pointers, and allocates
 * working memory. 812 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSfield_ResourceInit(u32 floorDataEntry, u32 loadMode) {
    /* TODO: match -- 812 bytes at 0x801123D4 */
}
#pragma pop

/* ==================================================================
 * fn_801129CC -- GSfield_UpdateObjects
 *
 * Per-frame update for all field objects (NPCs, models, triggers).
 * 1472 bytes -- iterates a linked list of active objects.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSfield_UpdateObjects(void) {
    /* TODO: match -- 1472 bytes at 0x801129CC */
}
#pragma pop

/* ===== Small accessor functions (0x8011392C-0x801142F8) ===== */

/* fn_8011392C */ f32 GSfield_GetObjectRotX(void* obj)    { /* TODO: match */ }
/* fn_8011393C */ f32 GSfield_GetObjectRotY(void* obj)    { /* TODO: match */ }
/* fn_8011394C */ f32 GSfield_GetObjectRotZ(void* obj)    { /* TODO: match */ }
/* fn_8011395C */ f32 GSfield_GetObjectScale(void* obj)   { /* TODO: match */ }

/* ===================================================================
 * Generated: 0 pattern-matched + 59 stubs
 * Range: 0x8010E138 - 0x8011432C
 * =================================================================== */

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
void fn_8010F4B8(void) {
    /* TODO: match -- 236 bytes at 0x8010F4B8 */
}
#pragma pop

/* 0x8010F5A4 | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010F5A4(void) {
    /* TODO: match -- 252 bytes at 0x8010F5A4 */
}
#pragma pop

/* 0x8010F6A0 | 0x7C */
void fn_8010F6A0(void* arg0, void* arg1, void* arg2, f32 t) {
    f32 v[3];

    PSVECSubtract(arg2, arg1, v);
    PSVECScale(v, v, t / PSVECMag(v));
    PSVECAdd(v, arg1, arg0);
}

/* 0x8010FA54 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010FA54(void) {
    /* TODO: match -- 160 bytes at 0x8010FA54 */
}
#pragma pop

/* 0x8010FAF4 | 0x304 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010FAF4(void) {
    /* TODO: match -- 772 bytes at 0x8010FAF4 */
}
#pragma pop

/* 0x8010FDF8 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010FDF8(void) {
    /* TODO: match -- 460 bytes at 0x8010FDF8 */
}
#pragma pop

/* 0x8010FFC4 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_8010FFC4(s32 index, s32 flag) {
#pragma optimization_level 4
    extern u8* GScolsys2GetCurFloor(void);
    u8* table;
    u8* entry;
    u8* p;
    s32 result;

    if (index < 0 || index >= 0x30) {
        result = 4;
    } else {
        table = GScolsys2GetCurFloor();
        if (table == NULL) {
            result = 1;
        } else {
            p = table + index * 0x14 + 0xA00;
            if ((*(u16*)(p + 0x10) & 1) == 0) {
                result = 4;
            } else {
                entry = p;
                result = 0;
            }
        }
    }
    if (result != 0) {
        return result;
    }
    if (flag != 0) {
        *(u16*)(entry + 0x10) &= ~0x2;
    } else {
        *(u16*)(entry + 0x10) |= 0x2;
    }
    return 0;
}
#pragma pop

/* 0x80110084 | 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80110084(s32* outIndex, void* data) {
#pragma optimization_level 4
    extern void* GScolsys2GetCurFloor(void);
    u8* base;
    s32 index;
    u8* entry;
    s32 i;

    base = (u8*)GScolsys2GetCurFloor();
    if (base == NULL) {
        return 1;
    }
    entry = base;
    index = 0;
    for (i = 0; i < 6; i++) {
        if ((*(u16*)(entry + 0xa10) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa24) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa38) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa4c) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa60) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa74) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa88) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa9c) & 1) == 0) goto found;
        entry += 0xa0;
        index++;
    }
found:
    if (index >= 0x30) {
        return 5;
    }
    {
        u8* slot = base + index * 0x14;
        *(s32*)(slot + 0xa00) = *(s32*)((u8*)data + 0x0);
        *(s32*)(slot + 0xa04) = *(s32*)((u8*)data + 0x4);
        *(f32*)(slot + 0xa08) = *(f32*)((u8*)data + 0x8);
        *(f32*)(slot + 0xa0c) = *(f32*)((u8*)data + 0xc);
        *(u16*)(slot + 0xa10) = 0;
        *(u16*)(slot + 0xa10) |= 1;
        *outIndex = index;
    }
    return 0;
}
#pragma pop

/* 0x80110E64 | 0x60C */
#pragma push
#pragma optimizewithasm off
s32 fn_80110E64(GScolsys2Vec3* point, GScolsys2Vec3* dirVec, f32 radius,
                GScolsys2TriangleList* triList, void* mtxInv, void* mtxFwd,
                GSfieldQueryTriangle* outTris) {
    GScolsys2Triangle* tri;
    GSfieldQueryTriangle* out;
    s32 outCount;
    s32 triIdx;
    GSfieldQueryTriangle* scan;
    s32 scanIdx;
    s32 vertIdx;
    GScolsys2Vec3* vdst;
    GScolsys2Vec3* vsrc;
    s32 hit;
    f32 radiusSq;
    GSfieldEdgeMasks edgeMasksA;
    GSfieldEdgeMasks edgeMasksB;
    GScolsys2Vec3 planePoint;
    GScolsys2Vec3 cp;
    GScolsys2Vec3 lineCp;
    GScolsys2Vec3 verts[3];
    f32 lineT;
    u16 flags;

    radiusSq = radius * radius;
    tri = triList->triangles;
    out = outTris;
    outCount = 0;
    triIdx = 0;
    while ((u32)triIdx < triList->count && outCount < 4) {
        scan = outTris;
        for (scanIdx = 0; scanIdx < outCount; scanIdx++, scan++) {
            if (tri->id == scan->id) {
                break;
            }
        }
        if (scanIdx >= outCount) {
            PSMTXMultVec(mtxFwd, &tri->normal, &planePoint);
            if (PSVECDotProduct(&planePoint, dirVec) < 0.0f) {
                vdst = verts;
                vsrc = tri->verts;
                vertIdx = 0;
                do {
                    PSMTXMultVec(mtxInv, vsrc, vdst);
                    vertIdx++;
                    vsrc++;
                    vdst++;
                } while (vertIdx < 3);
                if (GScolsy2UtilGetSidePlanePoint(&planePoint, verts, point) < 0.0f) {
                    hit = 0;
                } else {
                    GScolsy2UtilGetCpPlanePoint(&cp, &planePoint, verts, point);
                    if (PSVECSquareDistance(&cp, point) >= radiusSq) {
                        hit = 0;
                    } else if (GScolsy2UtilChkInTri(&cp, verts, &planePoint) == 0) {
                        hit = 0;
                    } else {
                        hit = 1;
                    }
                }
                if (hit != 0) {
                    out->id = tri->id;
                    out->verts[0] = verts[0];
                    out->verts[1] = verts[1];
                    out->verts[2] = verts[2];
                    out->normal = planePoint;
                    outCount++;
                    out++;
                }
            }
        }
        triIdx++;
        tri++;
    }

    tri = triList->triangles;
    triIdx = 0;
    out = outTris + outCount;
    while ((u32)triIdx < triList->count && outCount < 4) {
        if ((tri->flags & 7) != 0) {
            scan = outTris;
            for (scanIdx = 0; scanIdx < outCount; scanIdx++, scan++) {
                if (tri->id == scan->id) {
                    break;
                }
            }
            if (scanIdx >= outCount) {
                PSMTXMultVec(mtxFwd, &tri->normal, &planePoint);
                if (PSVECDotProduct(&planePoint, dirVec) < 0.0f) {
                    vdst = verts;
                    vsrc = tri->verts;
                    vertIdx = 0;
                    do {
                        PSMTXMultVec(mtxInv, vsrc, vdst);
                        vertIdx++;
                        vsrc++;
                        vdst++;
                    } while (vertIdx < 3);
                    edgeMasksA = lbl_8047CF48;
                    flags = tri->flags;
                    if (GScolsy2UtilGetSidePlanePoint(&planePoint, verts, point) < 0.0f) {
                        hit = 0;
                    } else {
                        vsrc = verts;
                        hit = 0;
                        for (vertIdx = 0; vertIdx < 3; vertIdx++, vsrc++) {
                            if ((flags & edgeMasksA.values[vertIdx]) != 0) {
                                s32 next = vertIdx + 1;
                                if (next >= 3) {
                                    next = 0;
                                }
                                lineT = GScolsys2UtilGetCpLinePoint(&lineCp, vsrc, &verts[next], point);
                                if (lineT >= 0.0f && lineT <= 1.0f
                                    && PSVECSquareDistance(&lineCp, point) < radiusSq) {
                                    hit = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if (hit != 0) {
                        out->id = tri->id;
                        out->verts[0] = verts[0];
                        out->verts[1] = verts[1];
                        out->verts[2] = verts[2];
                        out->normal = planePoint;
                        outCount++;
                        out++;
                    }
                }
            }
        }
        triIdx++;
        tri++;
    }

    tri = triList->triangles;
    triIdx = 0;
    out = outTris + outCount;
    while ((u32)triIdx < triList->count && outCount < 4) {
        if ((tri->flags & 7) != 0) {
            scan = outTris;
            for (scanIdx = 0; scanIdx < outCount; scanIdx++, scan++) {
                if (tri->id == scan->id) {
                    break;
                }
            }
            if (scanIdx >= outCount) {
                PSMTXMultVec(mtxFwd, &tri->normal, &planePoint);
                if (PSVECDotProduct(&planePoint, dirVec) < 0.0f) {
                    vdst = verts;
                    vsrc = tri->verts;
                    vertIdx = 0;
                    do {
                        PSMTXMultVec(mtxInv, vsrc, vdst);
                        vertIdx++;
                        vsrc++;
                        vdst++;
                    } while (vertIdx < 3);
                    edgeMasksB = lbl_8047CF50;
                    flags = tri->flags;
                    if (GScolsy2UtilGetSidePlanePoint(&planePoint, verts, point) < 0.0f) {
                        hit = 0;
                    } else {
                        vsrc = verts;
                        hit = 0;
                        for (vertIdx = 0; vertIdx < 3; vertIdx++, vsrc++) {
                            s32 next = vertIdx + 2;
                            if (next >= 3) {
                                next -= 3;
                            }
                            if ((flags & edgeMasksB.values[vertIdx]) != 0 && (flags & edgeMasksB.values[next]) != 0) {
                                if (PSVECSquareDistance(vsrc, point) < radiusSq) {
                                    hit = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if (hit != 0) {
                        out->id = tri->id;
                        out->verts[0] = verts[0];
                        out->verts[1] = verts[1];
                        out->verts[2] = verts[2];
                        out->normal = planePoint;
                        outCount++;
                        out++;
                    }
                }
            }
        }
        triIdx++;
        tri++;
    }
    return outCount;
}
#pragma pop

/* 0x80111470 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80111470(void) {
    /* TODO: match -- 460 bytes at 0x80111470 */
}
#pragma pop

/* 0x8011163C | 0x228 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011163C(void) {
    /* TODO: match -- 552 bytes at 0x8011163C */
}
#pragma pop

/* 0x80111864 | 0x338 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80111864(void* a, void* b, void* c) {
#pragma optimization_level 4
    extern f32 PSVECDotProduct(void* a, void* b);
    extern s32 GScolsys2UtilGetCpPlaneLine(void* a, void* b, void* c, void* d, void* e, void* f);
    extern s32 GScolsy2UtilChkInTri(void* a, void* b, void* c);
    extern f32 lbl_8047CF60;
    extern f32 lbl_8047CF64;
    u8* wzx;
    u8* region;
    u8* triList;
    u8* tri;
    u8* tempWrite;
    u8* tempRead;
    u8* out;
    u8* scan;
    s32 outCount;
    s32 outOffset;
    s32 regionIdx;
    s32 tempCount;
    s32 triIdx;
    s32 scanIdx;
    s32 vertIdx;
    s32 visible;
    s32 hit;
    f32 resultT;
    u8 temp[0xD0];
    f32 mtxInv[12];
    f32 mtxFwd[12];
    f32 verts[9];
    f32 dirVec[3];
    f32 planePoint[3];
    f32 hitPoint[3];

    outCount = 0;
    outOffset = 0;
    wzx = (u8*)fn_8010CBC0();
    PSVECSubtract(b, a, dirVec);
    region = *(u8**)wzx;
    regionIdx = 0;
    while ((u32)regionIdx < *(u32*)(wzx + 4)) {
        GScolsys2GetObjEnable(regionIdx, &visible);
        if (visible != 0) {
            triList = *(u8**)(region + 0x30);
            if (triList != NULL) {
                fn_8010CA30(mtxInv, regionIdx);
                fn_8010C8D0(mtxFwd, regionIdx);
                tempWrite = temp;
                tempCount = 0;
                tri = *(u8**)triList;
                triIdx = 0;
                while ((u32)triIdx < *(u32*)(triList + 4)) {
                    scan = temp;
                    scanIdx = 0;
                    if (tempCount > 0) {
                        do {
                            if (*(u16*)(tri + 0x30) == *(u16*)(scan + 0x30)) {
                                break;
                            }
                            scan += 0x34;
                            scanIdx++;
                        } while (scanIdx < tempCount);
                    }
                    if (scanIdx < tempCount) {
                        goto next_triangle;
                    }
                    PSMTXMultVec(mtxFwd, tri + 0x24, planePoint);
                    if (PSVECDotProduct(planePoint, dirVec) >= lbl_8047CF60) {
                        goto next_triangle;
                    }
                    scan = (u8*)verts;
                    vertIdx = 0;
                    do {
                        PSMTXMultVec(mtxInv, tri + (vertIdx * 0xC), scan);
                        vertIdx++;
                        scan += 0xC;
                    } while (vertIdx < 3);
                    if (GScolsys2UtilGetCpPlaneLine(hitPoint, &resultT, planePoint, verts, a, b) == 0) {
                        hit = 0;
                    } else if ((resultT < lbl_8047CF60) || (resultT > lbl_8047CF64)) {
                        hit = 0;
                    } else if (GScolsy2UtilChkInTri(hitPoint, verts, planePoint) == 0) {
                        hit = 0;
                    } else {
                        hit = 1;
                    }
                    if (hit != 0) {
                        *(u32*)(tempWrite + 0x00) = *(u32*)((u8*)verts + 0x00);
                        *(u32*)(tempWrite + 0x04) = *(u32*)((u8*)verts + 0x04);
                        *(u32*)(tempWrite + 0x08) = *(u32*)((u8*)verts + 0x08);
                        *(u32*)(tempWrite + 0x0C) = *(u32*)((u8*)verts + 0x0C);
                        *(u32*)(tempWrite + 0x10) = *(u32*)((u8*)verts + 0x10);
                        *(u32*)(tempWrite + 0x14) = *(u32*)((u8*)verts + 0x14);
                        *(u32*)(tempWrite + 0x18) = *(u32*)((u8*)verts + 0x18);
                        *(u32*)(tempWrite + 0x1C) = *(u32*)((u8*)verts + 0x1C);
                        *(u32*)(tempWrite + 0x20) = *(u32*)((u8*)verts + 0x20);
                        *(u32*)(tempWrite + 0x24) = *(u32*)((u8*)planePoint + 0x00);
                        *(u32*)(tempWrite + 0x28) = *(u32*)((u8*)planePoint + 0x04);
                        *(u32*)(tempWrite + 0x2C) = *(u32*)((u8*)planePoint + 0x08);
                        *(u16*)(tempWrite + 0x30) = *(u16*)(tri + 0x30);
                        tempWrite += 0x34;
                        tempCount++;
                    }
                next_triangle:
                    triIdx++;
                    tri += 0x34;
                    if (tempCount >= 4) {
                        break;
                    }
                }
                tempRead = temp;
                triIdx = 0;
                while (triIdx < tempCount) {
                    scan = (u8*)c;
                    scanIdx = 0;
                    if (outCount > 0) {
                        do {
                            if (*(u16*)(scan + 0x30) == *(u16*)(tempRead + 0x30)) {
                                break;
                            }
                            scan += 0x34;
                            scanIdx++;
                        } while (scanIdx < outCount);
                    }
                    if (scanIdx < outCount) {
                        goto next_temp;
                    }
                    out = (u8*)c + outOffset;
                    *(u32*)(out + 0x00) = *(u32*)(tempRead + 0x00);
                    *(u32*)(out + 0x04) = *(u32*)(tempRead + 0x04);
                    *(u32*)(out + 0x08) = *(u32*)(tempRead + 0x08);
                    *(u32*)(out + 0x0C) = *(u32*)(tempRead + 0x0C);
                    *(u32*)(out + 0x10) = *(u32*)(tempRead + 0x10);
                    *(u32*)(out + 0x14) = *(u32*)(tempRead + 0x14);
                    *(u32*)(out + 0x18) = *(u32*)(tempRead + 0x18);
                    *(u32*)(out + 0x1C) = *(u32*)(tempRead + 0x1C);
                    *(u32*)(out + 0x20) = *(u32*)(tempRead + 0x20);
                    *(u32*)(out + 0x24) = *(u32*)(tempRead + 0x24);
                    *(u32*)(out + 0x28) = *(u32*)(tempRead + 0x28);
                    *(u32*)(out + 0x2C) = *(u32*)(tempRead + 0x2C);
                    *(u32*)(out + 0x30) = *(u32*)(tempRead + 0x30);
                    outCount++;
                    outOffset += 0x34;
                next_temp:
                    tempRead += 0x34;
                    triIdx++;
                    if (outCount >= 4) {
                        break;
                    }
                }
            }
        }
        regionIdx++;
        region += 0x40;
        if (outCount >= 4) {
            break;
        }
    }
    return outCount;
}
#pragma pop

/* 0x80111B9C | 0x88 */
s32 GScolsys2CheckGetEventID(void* arg0, void* arg1, void* arg2) {
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 lbl_8047CF60;

    if (fn_8010CBC0() == 0) {
        return 0;
    }
    if (PSVECDistance(arg1, arg0) <= lbl_8047CF60) {
        return 0;
    }
    return fn_80111864(arg0, arg1, arg2);
}

/* 0x80111C24 | 0x1D4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
s32 fn_80111C24(void* origin, void* dir) {
#pragma optimization_level 4
    extern f32 PSVECDistance(void* a, void* b);
    extern s32 GScolsys2UtilGetCpPlaneLine(void* a, void* b, void* c, void* d, void* e, void* f);
    extern s32 GScolsy2UtilChkInTri(void* a, void* b, void* c);
    extern f32 lbl_8047CF68;
    extern f32 lbl_8047CF6C;
    GSFieldWzxData* wzx;
    u32 regIdx;
    GSFieldWzxTriangleList* triList;
    GSFieldWzxRegion* region;
    s32 k;
    u32 vertIdx;
    GSFieldWzxCompactTriangle* tri;
    GSFieldVec3f* vdst;
    GSFieldVec3f* vsrc;
    s32 visFlag;
    f32 resultT;
    GSFieldVec3f dirVec;
    GSFieldVec3f pt;
    GSFieldVec3f out;
    f32 mtxFwd[12];
    f32 mtxInv[12];
    GSFieldVec3f verts[3];
    s32 found;
    s32 hit;

    if (fn_8010CBC0() == NULL) {
        return 0;
    }
    if (PSVECDistance(dir, origin) <= lbl_8047CF68) {
        return 0;
    }
    wzx = (GSFieldWzxData*)fn_8010CBC0();
    PSVECSubtract(dir, origin, &dirVec);
    region = wzx->regions;
    regIdx = 0;
    while (regIdx < wzx->regionCount) {
        GScolsys2GetObjEnable(regIdx, &visFlag);
        if (visFlag != 0) {
            triList = region->boundaryTriangles;
            if (triList != NULL) {
                fn_8010CA30(mtxInv, regIdx);
                fn_8010C8D0(mtxFwd, regIdx);
                tri = (GSFieldWzxCompactTriangle*)triList->triangles;
                vertIdx = 0;
                while (vertIdx < triList->triangleCount) {
                    PSMTXMultVec(mtxFwd, &tri->normal, &pt);
                    vsrc = tri->vertices;
                    vdst = verts;
                    k = 0;
                    do {
                        PSMTXMultVec(mtxInv, vsrc, vdst);
                        k++;
                        vsrc++;
                        vdst++;
                    } while (k < 3);
                    if (GScolsys2UtilGetCpPlaneLine(&out, &resultT, &pt, verts, origin, dir) == 0) {
                        hit = 0;
                    } else if (resultT < lbl_8047CF68 || resultT > lbl_8047CF6C) {
                        hit = 0;
                    } else if (GScolsy2UtilChkInTri(&out, verts, &pt) == 0) {
                        hit = 0;
                    } else {
                        hit = 1;
                    }
                    if (hit != 0) {
                        found = 1;
                        goto inner_done;
                    }
                    vertIdx++;
                    tri++;
                }
                found = 0;
            inner_done:
                if (found != 0) {
                    return 1;
                }
            }
        }
        regIdx++;
        region++;
    }
    return 0;
}
#pragma peephole on
#pragma pop

/* 0x80111DF8 | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80111DF8(void) {
    /* TODO: match -- 308 bytes at 0x80111DF8 */
}
#pragma pop

/* 0x80111F2C | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80111F2C(void) {
    /* TODO: match -- 336 bytes at 0x80111F2C */
}
#pragma pop

/* 0x8011207C | 0x1E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011207C(void) {
    /* TODO: match -- 484 bytes at 0x8011207C */
}
#pragma pop

/* 0x80112260 | 0x120 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80112260(void) {
    /* TODO: match -- 288 bytes at 0x80112260 */
}
#pragma pop

/* 0x80112380 | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
s32 floorCheckFightKind(u32 id) {
#pragma optimization_level 4
    extern void fn_80115C48(void);
    extern s32 fn_80115AC8(void);
    s32 ready = 0;

    if (id != 0xFFFFFFFF) {
        fn_80115C48();
        switch (fn_80115AC8() & 0xFF) {
        case 2:
            ready = 1;
            break;
        }
    }
    return ready;
}
#pragma peephole on
#pragma pop

/* 0x80112700 | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80112700(void) {
#pragma optimization_level 4
    extern void fn_80115BD8(void);
    extern u32 fn_801159A8(void);
    extern void fn_800F7318(s32, u32, s32, s32, s32, ...);
    u32 id;

    fn_80115BD8();
    if ((id = fn_801159A8()) != 0) {
        fn_800F7318(0xF, id, 0x1000, 1, 0, 0);
    }
}
#pragma pop

/* 0x8011274C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void floorCheckFade(void) {
    extern void fadeCheck(s32);
    extern void fn_800D3074(s32);
    extern u8 lbl_80478DD0;

    fadeCheck(1);
    lbl_80478DD0 = 0;
    fn_800D3074(1);
}
#pragma peephole on
#pragma pop

/* 0x80112780 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_80112780(void) {
#pragma optimization_level 4
    extern void fn_800F7434(void* callback, s32 arg, ...);
    GSFieldColqueryState* state = (GSFieldColqueryState*)lbl_80408378;
    void* callback;

    callback = (void*)state->transitionBeginCallback;
    if (callback != NULL) {
        fn_800F7434(callback, 0);
    }
}
#pragma peephole on
#pragma pop

/* 0x801127BC | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801127BC(void) {
    /* TODO: match -- 136 bytes at 0x801127BC */
}
#pragma pop

/* 0x80112844 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_80112844(void) {
#pragma optimization_level 4
    extern void fn_800F7434(void* callback, s32 arg, ...);
    extern void fn_800FF0A0(void (*callback)(void));
    GSFieldColqueryState* state = (GSFieldColqueryState*)lbl_80408378;
    void* callback;

    callback = (void*)state->transitionPollCallback;
    if (callback != NULL) {
        fn_800F7434(callback, 0);
        fn_800FF0A0(fn_80112844);
    }
}
#pragma peephole on
#pragma pop

/* 0x8011288C | 0x14 */
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
void fn_8011288C(u32 a, u32 b) {
    GSFieldColqueryState* state = (GSFieldColqueryState*)lbl_80408378;

    state->transitionPollCallback = b;
    state->transitionBeginCallback = a;
}
#pragma pop

/* 0x801128A0 | 0x10C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_801128A0(void) {
#pragma optimization_level 4
    extern u32 lbl_80272094[];
    extern u8 lbl_80408378[];
    extern void fn_800FF3C0(s32, s32, void*);
    extern void fn_800FF178(s32, s32, void*);
    extern void fn_800FF4D4(void*, u32);
    extern s32 fn_800057A8(void);
    extern void fn_800FF788(u32);
    extern void fn_800FF784(u32);
    extern void fn_80118020(void);
    extern void fn_801129CC(void);
    extern void fn_801129AC(void);
    u32 sp8[3];
    u32 r;

    sp8[0] = lbl_80272094[0];
    sp8[1] = lbl_80272094[1];
    sp8[2] = lbl_80272094[2];
    fn_800FF3C0(0, 0x5000, (void*)fn_801129CC);
    fn_800FF178(0xFF, 0x5000, (void*)fn_801129AC);
    fn_800FF4D4(sp8, 1);
    fn_800FF4D4(sp8, 2);
    switch (fn_800057A8()) {
    case 5:
        r = 0x3E4;
        break;
    case 4:
        r = 0x320;
        break;
    case 1:
        r = 0x3E6;
        break;
    case 2:
        r = 0x3E7;
        break;
    case 3:
    default:
        r = 0x399;
        break;
    }
    *(u32*)(lbl_80408378 + 0x0) = 0;
    *(u32*)(lbl_80408378 + 0x4) = r;
    *(u8*)(lbl_80408378 + 0x8) = 1;
    *(u32*)(lbl_80408378 + 0xC) = 0;
    fn_800FF788(r);
    fn_800FF784(r);
    fn_80118020();
}
#pragma peephole on
#pragma pop

/* 0x801129AC | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801129AC(void) {
    extern void mailMainReceiveTerminate(void);
    mailMainReceiveTerminate();
}
#pragma pop

/* 0x80112F8C | 0x60 */
#pragma push
#pragma peephole off
void fn_80112F8C(void) {
    extern void* fn_801157B0(void);
    extern u32 fn_800FF560(void);
    extern void GSthreadCreate(s32 a, u32 b, u32 c, u32 d, u32 e, void* f);
    extern void fn_800FF0A0(void (*callback)(void));
    void* obj;

    obj = fn_801157B0();
    if (obj != NULL) {
        GSthreadCreate(1, fn_800FF560(), 0x4000, 1, 1, obj);
    }
    fn_800FF0A0(fn_80112F8C);
}
#pragma pop

/* 0x80112FEC | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80112FEC(void) {
    /* TODO: match -- 604 bytes at 0x80112FEC */
}
#pragma pop

/* 0x80113248 | 0x29C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void _floorInitCharacters__FP11GSfloor_dd_(void* a) {
#pragma optimization_level 4
    extern u32 fn_80115A80(void);
    extern u32 fn_80115704(void* a);
    extern u8* fn_8011711C(u32 i);
    extern void floorCharacterBiosGetPeopleInfoPtr(void);
    extern s32 fn_8018F6B4(void);
    extern u8 fn_800FF548(void);
    extern u32 fn_8018E050(u32 model, u32 i, s32 x);
    extern u32 fn_8018D998(u32 model, u32 i);
    extern s32 fn_80183958(u32 model, u32 i);
    extern void fn_801837D8(u32 model, u32 i, s32 a, u32 b, s32 c);
    extern void fn_8018C7C8(u32 model, u32 i, s32 flag);
    extern void floorCharacterBiosGetPos(u8* obj, void* out);
    extern void fn_8018C0A8(u32 model, u32 i, void* p);
    extern void floorCharacterBiosGetRot(u8* obj, void* out);
    extern void fn_8018BF24(u32 model, u32 i, void* p);
    extern void fn_8018CB5C(u32 model, u32 i);
    extern s32 fn_80117054(u8* obj);
    extern void fn_8018C1E8(u32 model, u32 i, s32 x);
    extern s32 fn_80117038(u8* obj);
    extern void fn_8018CA20(u32 model, u32 i, s32 x);
    extern u8 fn_80116F4C(u8* obj);
    extern void fn_80183B44(u32 model, u32 i, f32 x);
    extern void fn_801839A0(u32 model, u32 i, f32 x, f32 y);
    extern u8 fn_80116F30(u8* obj);
    extern u8 fn_80116F14(u8* obj);
    extern void fn_8018C69C(u32 model, u32 i, s32 flag);
    extern void fn_80188F78(u32 model, u32 i);
    extern const char lbl_802720CC[];
    extern const char lbl_8035B888[];
    extern f32 lbl_8047CF88;
    extern f32 lbl_8047CF8C;
    extern f32 lbl_8047CF90;
    u32 model;
    u32 count;
    u32 i;
    u32 result;
    u8* obj;
    f32 v14[3];
    f32 v8[3];

    model = fn_80115A80();
    count = fn_80115704(a);
    for (i = 0; i < count; i++) {
        obj = fn_8011711C(i);
        floorCharacterBiosGetPeopleInfoPtr();
        result = fn_8018F6B4();
        if (fn_800FF548() == 0) {
            result = fn_8018E050(model, i, result);
        } else {
            result = fn_8018D998(model, i);
        }
        if (result == 0) {
            GSlogWrite(lbl_802720CC, lbl_8035B888);
        } else if (fn_800FF548() != 1) {
            {
                s32 emitId;

                emitId = fn_80183958(model, i);
                fn_801837D8(model, i, emitId, result, 0);
            }
            fn_8018C7C8(model, i, 4);
            fn_8018C7C8(model, i, 8);
            floorCharacterBiosGetPos(obj, v14);
            fn_8018C0A8(model, i, v14);
            floorCharacterBiosGetRot(obj, v8);
            fn_8018BF24(model, i, v8);
            fn_8018CB5C(model, i);
            fn_8018C1E8(model, i, fn_80117054(obj));
            fn_8018CA20(model, i, fn_80117038(obj));
            switch (fn_80116F4C(obj)) {
            case 0:
            case 1:
                break;
            case 2:
                fn_80183B44(model, i, lbl_8047CF88);
                break;
            case 3:
                fn_801839A0(model, i, lbl_8047CF8C, lbl_8047CF90);
                break;
            }
            switch (fn_80116F30(obj)) {
            case 0:
                break;
            case 1:
                fn_8018C7C8(model, i, 0x10);
                break;
            case 2:
                fn_8018C7C8(model, i, 0x20);
                break;
            }
            switch (fn_80116F14(obj)) {
            case 0:
                fn_8018C69C(model, i, 0x40);
                break;
            case 1:
                fn_8018C7C8(model, i, 0x40);
                break;
            }
            if (((u32)((*obj >> 5) & 1)) != 0U) {
                fn_80188F78(model, i);
            }
        }
    }
}
#pragma peephole on
#pragma pop

/* 0x801134E4 | 0x294 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void floorInitMap(void) {
    /* TODO: match -- 660 bytes at 0x801134E4 */
}
#pragma pop

/* 0x80113778 | 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113778(void) {
    /* TODO: match -- 176 bytes at 0x80113778 */
}
#pragma pop

/* 0x80113828 | 0x64 */
#pragma push
#pragma peephole off
void fn_80113828(u32 arg0, s32 arg1) {
    extern u32 fn_800FF56C(void);
    extern void fn_800FF58C(s32);

    if (arg0 != 0) {
        *(u32*)(lbl_80408378 + 0x0) = fn_800FF56C();
        *(s32*)(lbl_80408378 + 0x4) = arg0;
        *(u8*)(lbl_80408378 + 0x8) = 1;
        *(s32*)(lbl_80408378 + 0xC) = arg1;
        fn_800FF58C(arg0);
    }
}
#pragma peephole on
#pragma pop

/* 0x8011388C | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_8011388C(void* a, void* b, s32 c) {
#pragma optimization_level 4
    extern u8 lbl_80408378[];
    extern u32 fn_800FF56C(void);
    extern void fn_80166A28(u32);
    extern void fn_800FF58C(void*);
    u32 sndId;

    if (a == NULL) {
        return;
    }
    *(u32*)(lbl_80408378 + 0x0) = fn_800FF56C();
    *(void**)(lbl_80408378 + 0x4) = a;
    *(u8*)(lbl_80408378 + 0x8) = 1;
    *(void**)(lbl_80408378 + 0xC) = b;
    switch (c) {
    case 0:
        sndId = 0;
        break;
    case 1:
    default:
        sndId = 0x3F9;
        break;
    }
    if (sndId != 0) {
        fn_80166A28(sndId);
    }
    fn_800FF58C(a);
}
#pragma peephole on
#pragma pop

/* 0x8011392C | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8011392C(void) {
    return *(u32*)(lbl_80408378 + 0xC);
}
#pragma pop

/* 0x8011393C | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8011393C(void) {
    return *(u32*)(lbl_80408378 + 0x4);
}
#pragma pop

/* 0x8011394C | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_8011394C(void) {
    return *(u32*)(lbl_80408378 + 0x0);
}
#pragma pop

/* 0x8011395C | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_8011395C(u32 value) {
    asm {
        lis r4, lbl_80408378@ha
        addi r4, r4, lbl_80408378@l
        stw r3, 0(r4)
    }
}
#pragma pop

/* 0x8011396C | 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma scheduling off
s32 fn_8011396C(s32 param) {
#pragma optimization_level 4
    extern u32 fn_80115C48(void);
    extern s32 fn_80115840(void);

    switch (param) {
    case 0xFD:
    case 0xFE:
    case 0xFF:
        return 0;
    }
    if (fn_80115C48() == 0) {
        return 0;
    }
    return fn_80115840();
}
#pragma scheduling on
#pragma pop

/* 0x801139BC | 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fn_801139BC(void) {
    extern void fn_8018B76C(s32, s32, s32, s32, s32);
    extern void fn_80117154(void);

    fn_8018B76C(0, 0x64, 1, 0, 1);
    fn_8018B76C(0, 0x65, 1, 0, 1);
    fn_80117154();
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma pop

/* 0x80113A0C | 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113A0C(void) {
    /* TODO: match -- 376 bytes at 0x80113A0C */
}
#pragma pop

/* 0x80113B84 | 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113B84(void) {
    /* TODO: match -- 396 bytes at 0x80113B84 */
}
#pragma pop

/* 0x80113D10 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
u32 fn_80113D10(u32 group) {
#pragma optimization_level 4
    extern void fn_800E4BF4();

    fn_800E4BF4(group);
    return 1;
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma pop

/* 0x80113D34 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void fn_80113D34(u32 unused, u32 modelIndex) {
#pragma optimization_level 4
    extern void floorOpenObject();

    floorOpenObject(modelIndex);
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma pop

/* 0x80113D58 | 0x1F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void floorOpenObject(u32 modelIndex) {
#pragma optimization_level 4
    extern const char lbl_80272088[];
    extern const char lbl_8035B868[];
    extern f32 lbl_8047CF98;
    extern void* fn_800F92D4(u32);
    extern void* HSD_ArchiveGetPublicAddress(void*, const char*);
    extern void* fn_800E4D18(void*);
    extern void GSmodelSetVisibility(void*, u32);
    extern u32 fn_800EC1BC(void*);
    extern void fn_800ECCA8(void*, u32);
    extern void fn_800EC9DC(void*, f32);
    extern void fn_800EC990(void*);
    extern u32 fn_800EC1B0(void*);
    extern void fn_800EC35C(void*, u32);
    extern void fn_800EC308(void*, f32);
    extern void fn_800EC1E4(void*);
    const char* strings;
    u32 archiveMode;
    void* archive;
    void* pub;
    void* model;
    u32 special;

    strings = lbl_80272088;
    special = 0;
    archiveMode = modelIndex;
    if ((modelIndex == 3) || (modelIndex == 100)) {
        archiveMode = 0x00F71000;
    } else if ((modelIndex == 4) || (modelIndex == 101)) {
        archiveMode = 0x00F31000;
    } else if (((modelIndex >> 9) & 0x3F) == 2) {
        special = 1;
    }

    if (archiveMode == 0) {
        GSlogWrite(strings + 0xDC, lbl_8035B868);
        return;
    }

    archive = fn_800F92D4(archiveMode);
    if (special != 0) {
        if (archive == NULL) {
            GSlogWrite(strings + 0xF8, lbl_8035B868);
            return;
        }
        pub = HSD_ArchiveGetPublicAddress(archive, strings + 0x28);
        if (pub == NULL) {
            GSlogWrite(strings + 0x118, lbl_8035B868);
            return;
        }
        archive = *(void**)pub;
        if (*(void**)archive == NULL) {
            GSlogWrite(strings + 0x140, lbl_8035B868);
            return;
        }
        archive = *(void**)archive;
    }

    model = fn_800E4D18(archive);
    if (model == NULL) {
        GSlogWrite(strings + 0x64, lbl_8035B868);
        GSlogWrite(strings + 0x15C);
        return;
    }

    GSmodelSetVisibility(model, 1);
    if (fn_800EC1BC(model) != 0) {
        fn_800ECCA8(model, 0);
        fn_800EC9DC(model, lbl_8047CF98);
        fn_800EC990(model);
    }
    if (fn_800EC1B0(model) != 0) {
        fn_800EC35C(model, 0);
        fn_800EC308(model, lbl_8047CF98);
        fn_800EC1E4(model);
    }
}
#pragma pop

/* 0x80113F48 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113F48(void) {
    extern void fn_80115BD8(void);
    extern void fn_80115A80(void);

    fn_80115BD8();
    fn_80115A80();
}
#pragma pop

/* 0x80113F6C | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorGetResource(u32 key, u32 arg) {
#pragma optimization_level 4
    extern void* fn_80115C48(u32);
    extern u32 fn_80115A80(void*);
    extern void* GSresGetResource(u32, u32);
    void* resource;

    resource = fn_80115C48(key);
    if (resource == NULL) {
        return NULL;
    }
    return GSresGetResource(fn_80115A80(resource), arg);
}
#pragma pop

/* 0x80113FB4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_80113FB4(u32 key) {
#pragma optimization_level 4
    extern void* fn_80115C48(u32);
    extern u32 fn_80115A80(void*);
    void* resource;

    resource = fn_80115C48(key);
    if (resource == NULL) {
        return 0;
    }
    return fn_80115A80(resource);
}
#pragma pop

/* 0x80113FE8 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_80113FE8(void) {
#pragma optimization_level 4
    extern u8 lbl_80408378[];
    extern u32 gamedatasaveGetStatus(s32, s32);
    extern u32 fn_800FF56C(void);
    extern void fn_800FF58C(u32);
    u8* state = lbl_80408378;
    u32 a;
    u32 b;
    u8 c;

    if (*(u8*)(state + 0x51) != 0) {
        a = *(u32*)(state + 0x48);
        b = *(u32*)(state + 0x4C);
        c = *(u8*)(state + 0x50);
    } else {
        a = gamedatasaveGetStatus(0, 5);
        b = gamedatasaveGetStatus(0, 7);
        c = (u8)gamedatasaveGetStatus(0, 8);
    }
    if (a != 0) {
        *(u32*)(lbl_80408378 + 0x0) = fn_800FF56C();
        *(u32*)(lbl_80408378 + 0x4) = a;
        *(u8*)(lbl_80408378 + 0x8) = 1;
        *(u32*)(lbl_80408378 + 0xC) = c;
        fn_800FF58C(a);
    }
    *(u8*)(state + 0x51) = 0;
    *(u32*)(lbl_80408378 + 0x0) = b;
}
#pragma peephole on
#pragma pop

/* 0x801140C8 | 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801140C8(void) {
    *(u8*)(lbl_80408378 + 0x51) = 0;
}
#pragma pop

/* 0x801140DC | 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_801140DC(u32 material, u32 palette, u8 slot) {
#pragma optimization_level 4
    extern u32 fn_80128E24(void);
    extern void* fn_80128E04(void);
    extern void gamedatasaveSetStatus(void*, u32, u32);
    void* obj;

    if (fn_80128E24() != 0) {
        obj = fn_80128E04();
        if (obj != 0) {
            gamedatasaveSetStatus(obj, 5, material);
            gamedatasaveSetStatus(obj, 7, palette);
            gamedatasaveSetStatus(obj, 8, slot);
        }
    }
}
#pragma peephole on
#pragma pop

/* 0x8011416C | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011416C(u32 material, u32 palette, u8 materialSlot) {
#pragma optimization_level 4
    u8* state = lbl_80408378;

    *(u32*)(state + 0x48) = material;
    *(u32*)(state + 0x4C) = palette;
    *(u8*)(state + 0x50) = materialSlot;
    *(u8*)(state + 0x51) = 1;
}
#pragma pop

/* 0x8011418C | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011418C(u32* texture, u32* palette, u8* textureSlot) {
#pragma optimization_level 4
    if (texture != NULL) {
        *texture = *(u32*)(lbl_80408378 + 0x3C);
    }
    if (palette != NULL) {
        *palette = *(u32*)(lbl_80408378 + 0x40);
    }
    if (textureSlot != NULL) {
        *textureSlot = *(u8*)(lbl_80408378 + 0x44);
    }
}
#pragma pop

/* 0x801141D8 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801141D8(u32 texture, u32 palette, u8 textureSlot) {
#pragma optimization_level 4
    u8* state = lbl_80408378;

    *(u32*)(state + 0x3C) = texture;
    *(u32*)(state + 0x40) = palette;
    *(u8*)(state + 0x44) = textureSlot;
    *(u8*)(state + 0x51) = 0;
}
#pragma pop

/* 0x801141F8 | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void EvlogSet__FScUl(void) {
    /* TODO: match -- 92 bytes at 0x801141F8 */
}
#pragma pop

/* 0x80114254 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
s32 fn_80114254(s32 param1, s32 param2, s32 param3) {
#pragma optimization_level 4
    extern s32 GSresGetResource(s32);
    extern void fn_8017F484(s32, s32, s32);
    s32 result = GSresGetResource(param1);

    fn_8017F484(param1, param2, param3);
    return result;
}
#pragma peephole on
#pragma pop

/* 0x801142B4 | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void* fn_801142B4(void* group, void* model, u32 size) {
#pragma optimization_level 4
    extern void* GSresAllocResourceAlign(u32 size, u32 alignment, void* group, void* model, u32 flags);
    void* result;

    result = GSresAllocResourceAlign((size + 0x1F) & ~0x1F, 0x20, group, model, 0);
    if (result == NULL) {
        result = NULL;
    }
    return result;
}
#pragma peephole on
#pragma pop

/* 0x801142F8 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void* floorReadGFLPostFunc(u32 group, u32 modelId) {
#pragma optimization_level 4
    extern void* GSresGetResource(u32, u32);
    extern void fn_801ED680(void*);
    void* model;

    model = GSresGetResource(group, modelId);
    fn_801ED680(model);
    return model;
}
#pragma peephole on
#pragma pop
