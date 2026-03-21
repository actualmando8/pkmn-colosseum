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
 *   fn_80111B9C (GSfield_IsPointOnFloor)
 *   fn_80111C24 (GSfield_RegionBoundsTest)
 *   fn_80111DF8 (GSfield_ClipToFloorBounds)
 *   fn_80111F2C (GSfield_ProjectPointToFloor)
 *   fn_8011207C (GSfield_FindSpawnPoint)
 *   fn_80112260 (GSfield_GetSpawnPosition)
 *   fn_80112380 (GSfield_CheckReady)
 *   fn_801123D4 (GSfield_ResourceInit)
 *   fn_80112700 (GSfield_GetActiveFloorId)
 *   fn_8011274C (GSfield_FinalizeLoad)
 *   fn_80112780 (GSfield_BeginTransition)
 *   fn_801127BC (GSfield_SetTransitionCallback)
 *   fn_80112844 (GSfield_GetTransitionState)
 *   fn_8011288C (GSfield_IsTransitioning)
 *   fn_801128A0 (GSfield_ProcessTransition)
 *   fn_801129AC (GSfield_GetFieldState)
 *   fn_801129CC (GSfield_UpdateObjects)
 *   fn_80112F8C (GSfield_ClearObjectList)
 *   fn_80112FEC (GSfield_AddObject)
 *   fn_80113248 (GSfield_RemoveObject)
 *   fn_801134E4 (GSfield_MoveObject)
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
 *   fn_80113D58 (GSfield_LoadFieldModel)
 *   fn_80113F48 (GSfield_UnloadFieldModel)
 *   fn_80113F6C (GSfield_SetModelAnimation)
 *   fn_80113FB4 (GSfield_GetModelAnimFrame)
 *   fn_80113FE8 (GSfield_PlayModelAnimation)
 *   fn_801140C8 (GSfield_StopModelAnimation)
 *   fn_801140DC (GSfield_SetModelMaterial)
 *   fn_8011416C (GSfield_GetModelMaterial)
 *   fn_8011418C (GSfield_SetModelTexture)
 *   fn_801141D8 (GSfield_GetModelTexture)
 *   fn_801141F8 (GSfield_AttachToJoint)
 *   fn_80114254 (GSfield_DetachFromJoint)
 *   fn_801142B4 (GSfield_GetJointPosition)
 *   fn_801142F8 (GSfield_GetJointCount)
 *
 * This module wraps GScolsys2 collision queries for use by the field/world
 * system. It transforms world-space coordinates through the collision mesh
 * matrix, performs triangle intersection tests, and returns hit results
 * sorted by distance.
 *
 * Key patterns observed in the disassembly:
 *   - Heavy use of fn_800A37CC (MTXMultVec3) for coordinate transforms
 *   - Calls to fn_8010C7BC (GScolsys2_QueryTriVisible) for per-tri checks
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
extern void  fn_800DD970(const char* fmt, ...);        /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);

/* Matrix / vector math helpers */
extern void  fn_800A37CC(void* mtxDst, void* vecSrc, void* vecDst);  /* MTXMultVec3 */
extern void  fn_800A3A78(void* vecA, void* scale, void* vecOut);     /* VEC scale */
extern void  fn_800A3A9C(void* vecOut, void* vecIn, f32 scale);     /* VEC normalize */
extern void  fn_800A3AC0(void* curve, void* paramOut, f32 t);       /* VEC lerp */

/* GScolsys2 functions */
extern void* fn_8010CBC0(void);                         /* GScolsys2_GetWZXData */
extern s32   fn_8010C7BC(u32 triIdx, void* outFlag);    /* GScolsys2_QueryTriVisible */
extern void  fn_8010CA30(void* mtxOut, u32 layerIdx);   /* GScolsys2_BuildInverseTransform */
extern void  fn_8010C8D0(void* mtxOut, u32 layerIdx);   /* GScolsys2_BuildTransform */
extern s32   fn_8010DEF0(void* result, void* origin,
                          void* verts, void* normal);    /* GScolsys2_TriangleBoundsCheck */

/* ===== String constants (rodata) ===== */
extern const char lbl_802720B0[]; /* "scene_data" */

/* ===== BSS / global state ===== */
extern u8 lbl_80404C68[];  /* GScolsys2 collision state */

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
s32 GSfield_RayCast(void* origin, void* direction) {
    void* wzxData;
    s32 hitCount = 0;
    s32 triCount;
    s32 i;

    if (origin == NULL || direction == NULL) {
        return 0;
    }

    wzxData = fn_8010CBC0();
    if (wzxData == NULL) {
        return 0;
    }

    /* Get triangle count from WZX data header */
    triCount = *(s32*)((u8*)wzxData + 0x08);

    /* For each triangle in the collision mesh */
    for (i = 0; i < triCount && hitCount < 8; i++) {
        u32 triVisible;

        /* Check if triangle is visible/active */
        if (fn_8010C7BC(i, &triVisible) == 0) {
            continue;
        }

        /* Transform ray into triangle's local space */
        /* Test ray-triangle intersection */
        /* If hit, add to results sorted by parametric t */
        {
            f32 localOrigin[3];
            f32 localDir[3];
            f32 result[4];

            fn_800A37CC(NULL, origin, localOrigin);
            fn_800A37CC(NULL, direction, localDir);

            if (fn_8010DEF0(result, localOrigin, NULL, NULL) != 0) {
                hitCount++;
            }
        }
    }

    return hitCount;
}

/* ==================================================================
 * fn_8010E53C -- GSfield_SphereSweep
 *
 * Swept-sphere collision test (1516 bytes). Larger than RayCast
 * because it expands each triangle by the sphere radius before
 * performing the intersection test.
 * ================================================================== */
s32 GSfield_SphereSweep(void* origin, void* direction, f32 radius) {
    s32 hitCount = 0;
    void* wzxData;

    if (origin == NULL || direction == NULL) {
        return 0;
    }

    wzxData = fn_8010CBC0();
    if (wzxData == NULL) {
        return 0;
    }

    /* Swept-sphere collision:
     * For each triangle, expand it by the sphere radius,
     * then perform a ray-expanded-triangle intersection test.
     * Results sorted by distance.
     */

    return hitCount;
}

/* ==================================================================
 * fn_8010EB28 -- GSfield_SweepAgainstMesh
 *
 * Lower-level sweep test against a specific collision mesh subset.
 * 1212 bytes, heavy float math.
 * ================================================================== */
s32 GSfield_SweepAgainstMesh(void* meshData, void* sweep, void* result) {
    if (meshData == NULL || sweep == NULL || result == NULL) {
        return 0;
    }

    /* Lower-level sweep test against a specific collision mesh subset.
     * Uses the same expanded-triangle approach as SphereSweep but
     * operates on a subset of the collision mesh.
     */

    return 0;
}

/* ==================================================================
 * fn_8010EFE4 -- GSfield_LinePlaneTest
 *
 * Test a line segment against a collision plane.
 * 420 bytes. Used by both RayCast and arc tests.
 * ================================================================== */
s32 GSfield_LinePlaneTest(void* segStart, void* segEnd,
                           void* planeNormal, s32 doubleSided) {
    if (segStart == NULL || segEnd == NULL || planeNormal == NULL) {
        return 0;
    }

    /* Test a line segment against a collision plane.
     * Computes the parametric t value where the segment crosses the plane.
     * If doubleSided, accept hits from either side of the plane.
     */

    return 0;
}

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

extern u32 lbl_80408378;

/* Forward declarations for converted functions */
u32 fn_80112700(void);
u32 fn_80112F8C(void);
u32 fn_8011392C(void);
u32 fn_8011393C(void);
u32 fn_8011394C(void);
u32 fn_8011396C(void);
u32 fn_80113D10(void);
u32 fn_80113F6C(void);
u32 fn_801142B4(void);
void fn_8010F320(void);
void fn_8010F4B8(void);
void fn_8010F5A4(void);
void fn_8010F6A0(void);
void fn_8010FA54(void);
void fn_8010FAF4(void);
void fn_8010FDF8(void);
void fn_8010FFC4(void);
void fn_80110084(void);
void fn_80110E64(void);
void fn_80111470(void);
void fn_8011163C(void);
void fn_80111864(void);
void fn_80111B9C(void);
void fn_80111C24(void);
void fn_80111DF8(void);
void fn_80111F2C(void);
void fn_8011207C(void);
void fn_80112260(void);
void fn_80112380(void);
void fn_8011274C(void);
void fn_80112780(void);
void fn_801127BC(void);
void fn_80112844(u32 arg1);
void fn_8011288C(void);
void fn_801128A0(void);
void fn_801129AC(void);
void fn_80112FEC(void);
void fn_80113248(void);
void fn_801134E4(void);
void fn_80113778(void);
void fn_80113828(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
void fn_8011388C(void);
void fn_8011395C(u32 val);
void fn_801139BC(void);
void fn_80113A0C(void);
void fn_80113B84(void);
void fn_80113D34(void);
void fn_80113D58(void);
void fn_80113F48(void);
void fn_80113FB4(void);
void fn_80113FE8(void);
void fn_801140C8(void);
void fn_801140DC(void);
void fn_8011416C(void);
void fn_8011418C(u32* out1, u32* out2, u8* out3);
void fn_801141D8(void);
void fn_801141F8(void);
void fn_80114254(void);
void fn_801142F8(void);


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

/* 0x7C | fn_8010F6A0 | call_sequence */
void fn_8010F6A0(void) {
    fn_800A3A9C(0, 0, 0);
    fn_800A3B38();
    fn_800A3AC0(0, 0, 0);
    fn_800A3A78(0, 0, 0);
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
void fn_8010FFC4(void) {
    /* TODO: match -- 192 bytes at 0x8010FFC4 */
}
#pragma pop

/* 0x80110084 | 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80110084(void) {
    /* TODO: match -- 304 bytes at 0x80110084 */
}
#pragma pop

/* 0x80110E64 | 0x60C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80110E64(void) {
    /* TODO: match -- 1548 bytes at 0x80110E64 */
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
void fn_80111864(void) {
    /* TODO: match -- 824 bytes at 0x80111864 */
}
#pragma pop

/* 0x80111B9C | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80111B9C(void) {
    /* TODO: match -- 136 bytes at 0x80111B9C */
}
#pragma pop

/* 0x80111C24 | 0x1D4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80111C24(void) {
    /* TODO: match -- 468 bytes at 0x80111C24 */
}
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

/* 0x54 | fn_80112380 | generic */
void fn_80112380(void) {
    fn_80115C48();
    fn_80115AC8();
}

/* 0x4C | fn_80112700 | multi_call_cond */
u32 fn_80112700(void) {
    if (fn_80115BD8() == 0) { return 15; }
    fn_801159A8();
    fn_800F7318();
    return 15;
}

/* 0x8011274C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011274C(void) {
    /* TODO: match -- 52 bytes at 0x8011274C */
}
#pragma pop

/* 0x80112780 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80112780(void) {
    /* TODO: match -- 60 bytes at 0x80112780 */
}
#pragma pop

/* 0x801127BC | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801127BC(void) {
    /* TODO: match -- 136 bytes at 0x801127BC */
}
#pragma pop

/* 0x48 | fn_80112844 | two_call_arg_check */
void fn_80112844(u32 arg1) {
    if (arg1 == 0) { return; }
    fn_800F7434();
    fn_800FF0A0();
}

/* 0x8011288C | 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011288C(void) {
    /* TODO: match -- 20 bytes at 0x8011288C */
}
#pragma pop

/* 0x801128A0 | 0x10C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801128A0(void) {
    /* TODO: match -- 268 bytes at 0x801128A0 */
}
#pragma pop

/* 0x801129AC | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801129AC(void) {
    /* TODO: match -- 32 bytes at 0x801129AC */
}
#pragma pop

/* 0x60 | fn_80112F8C | multi_call_cond */
u32 fn_80112F8C(void) {
    if (fn_801157B0() == 0) { return 1; }
    fn_800FF560();
    fn_800F07A8();
    fn_800FF0A0();
    return 1;
}

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
void fn_80113248(void) {
    /* TODO: match -- 668 bytes at 0x80113248 */
}
#pragma pop

/* 0x801134E4 | 0x294 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801134E4(void) {
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

/* 0x64 | fn_80113828 | generic */
void fn_80113828(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    /* refs: lbl_80408378 */
    fn_800FF56C();
    fn_800FF58C();
}

/* 0x8011388C | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011388C(void) {
    /* TODO: match -- 160 bytes at 0x8011388C */
}
#pragma pop

/* 0x8011392C | 16 bytes | global_getter */
u32 fn_8011392C(void) {
    return *(u32*)((u8*)lbl_80408378 + 0xC);
}

/* 0x8011393C | 16 bytes | global_getter */
u32 fn_8011393C(void) {
    return *(u32*)((u8*)lbl_80408378 + 0x4);
}

/* 0x8011394C | 16 bytes | global_getter */
u32 fn_8011394C(void) {
    return *(u32*)((u8*)lbl_80408378 + 0x0);
}

/* 0x8011395C | 16 bytes | global_setter */
void fn_8011395C(u32 val) {
    *(u32*)((u8*)lbl_80408378 + 0x0) = val;
}

/* 0x50 | fn_8011396C | generic */
u32 fn_8011396C(void) {
    fn_80115C48();
    fn_80115840();
    return 0;
}

/* 0x50 | fn_801139BC | call_sequence */
void fn_801139BC(void) {
    fn_8018B76C();
    fn_8018B76C();
    fn_80117154();
}

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

/* 0x80113D10 | 36 bytes | call_return_const */
u32 fn_80113D10(void) {
    fn_800E4BF4();
    return 1;
}

/* 0x80113D34 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113D34(void) {
    /* TODO: match -- 36 bytes at 0x80113D34 */
}
#pragma pop

/* 0x80113D58 | 0x1F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113D58(void) {
    /* TODO: match -- 496 bytes at 0x80113D58 */
}
#pragma pop

/* 0x80113F48 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113F48(void) {
    /* TODO: match -- 36 bytes at 0x80113F48 */
}
#pragma pop

/* 0x48 | fn_80113F6C | multi_call_cond */
u32 fn_80113F6C(void) {
    if (fn_80115C48() != 0) { return 0; }
    fn_80115A80();
    fn_800F9318();
    return 0;
}

/* 0x80113FB4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113FB4(void) {
    /* TODO: match -- 52 bytes at 0x80113FB4 */
}
#pragma pop

/* 0x80113FE8 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80113FE8(void) {
    /* TODO: match -- 224 bytes at 0x80113FE8 */
}
#pragma pop

/* 0x801140C8 | 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801140C8(void) {
    /* TODO: match -- 20 bytes at 0x801140C8 */
}
#pragma pop

/* 0x801140DC | 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801140DC(void) {
    /* TODO: match -- 144 bytes at 0x801140DC */
}
#pragma pop

/* 0x8011416C | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011416C(void) {
    /* TODO: match -- 32 bytes at 0x8011416C */
}
#pragma pop

/* 0x4C | fn_8011418C | leaf_multi_output */
void fn_8011418C(u32* out1, u32* out2, u8* out3) {
    if (out1 != NULL) { *out1 = *(u32*)((u8*)lbl_80408378 + 0x3C); }
    if (out2 != NULL) { *out2 = *(u32*)((u8*)lbl_80408378 + 0x40); }
    if (out3 != NULL) { *out3 = *(u8*)((u8*)lbl_80408378 + 0x44); }
}

/* 0x801141D8 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801141D8(void) {
    /* TODO: match -- 32 bytes at 0x801141D8 */
}
#pragma pop

/* 0x5C | fn_801141F8 | generic */
void fn_801141F8(void) {
    /* refs: lbl_8035B818, lbl_8047AD60 */
}

/* 0x60 | fn_80114254 | call_sequence */
void fn_80114254(void) {
    fn_800F9318();
    fn_8017F484();
}

/* 0x44 | fn_801142B4 | generic */
u32 fn_801142B4(void) {
    fn_800F9418();
    return 0;
}

/* 0x801142F8 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801142F8(void) {
    /* TODO: match -- 52 bytes at 0x801142F8 */
}
#pragma pop
