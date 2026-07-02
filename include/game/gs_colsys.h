/**
 * @file gs_colsys.h
 * @brief GScolsys2 -- Genius Sonority collision system for Pokemon Colosseum.
 *
 * GScolsys2 manages collision mesh data loaded from WZX resources within
 * FSYS floor archives. The system supports:
 *   - Loading and relocating collision mesh data (WZX format)
 *   - Per-triangle collision queries (ground height, surface type)
 *   - Type advantage lookup between collision surface types
 *   - Debug visualization of collision meshes (wireframe triangles)
 *   - Up to 4 collision mesh "layers" (double-buffered active/pending)
 *
 * The collision data is organized around a central BSS structure at
 * lbl_80404C68 (0x3710 bytes), which contains:
 *   - A pointer to the active WZX collision mesh data
 *   - Up to 4 collision layers, each 0xDC0 bytes, containing per-triangle
 *     collision state (0x28 bytes per triangle entry, 16 flag halfwords
 *     spaced 0x14 apart starting at offset 0xA10 within each layer)
 *   - A GSgfx render handle for debug drawing
 *   - A display list handle for cached collision debug visualization
 *
 * The WZX collision mesh format stores triangles as:
 *   - 3 vertices (Vec3f each = 0x0C bytes, total 0x24 per tri)
 *   - Triangle attributes at offsets 0x24..0x3F (surface type, normal,
 *     color encoding for debug vis, child mesh pointers)
 *   - Each complete triangle record is 0x40 bytes
 *
 * Mesh groups referenced at offsets 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38
 * within each triangle record contain sub-mesh pointers with their own
 * vertex arrays (used for walls, ceiling, ramps, etc.).
 *
 * The type effectiveness system uses 0x12 (18) surface types stored in a
 * static table at lbl_8035B500, with each entry being 0x2C bytes. Type
 * codes 0x41='A' (advantage), 0x42='B' (disadvantage), 0x43='C' (immune)
 * and 0x3F='?' (neutral) are used for surface interaction lookups.
 *
 * Address range: 0x8010C220 - 0x8010E138 (core collision)
 *
 * Debug strings:
 *   "GScolsys2Draw : can't alloc display list memory."
 *   "[%s]..." (Japanese assert string at lbl_80272000)
 */
#ifndef GS_COLSYS_H
#define GS_COLSYS_H

#include "dolphin/types.h"

/* ===================================================================
 * Constants
 * =================================================================== */

/** Maximum number of collision layers (double-buffered) */
#define GSCOLSYS_MAX_LAYERS         4

/** Size of one collision layer in bytes */
#define GSCOLSYS_LAYER_SIZE         0xDC0

/** Size of one triangle collision entry */
#define GSCOLSYS_TRI_ENTRY_SIZE     0x28

/** Size of one WZX triangle record (vertices + attributes) */
#define GSCOLSYS_WZX_TRI_SIZE       0x40

/** Number of surface types in the type table */
#define GSCOLSYS_NUM_SURFACE_TYPES  0x12

/** Size of one surface type table entry */
#define GSCOLSYS_TYPE_ENTRY_SIZE    0x2C

/** Number of collision flag halfwords per layer iteration */
#define GSCOLSYS_FLAGS_PER_ITER     16

/** Spacing between collision flag halfwords */
#define GSCOLSYS_FLAG_STRIDE        0x14

/** Base offset of collision flags within a layer */
#define GSCOLSYS_FLAG_BASE_OFFSET   0xA10

/* Surface type interaction codes */
#define GSCOLSYS_TYPE_NEUTRAL       0x3F  /* '?' - no interaction */
#define GSCOLSYS_TYPE_ADVANTAGE     0x41  /* 'A' - surface advantage */
#define GSCOLSYS_TYPE_DISADVANTAGE  0x42  /* 'B' - surface disadvantage */
#define GSCOLSYS_TYPE_IMMUNE        0x43  /* 'C' - immune / blocked */

/* Debug draw colors (packed RGBA) */
#define GSCOLSYS_COLOR_DEFAULT      0xFFFFFFFF  /* white - walkable */
#define GSCOLSYS_COLOR_WALL         0xFF00FFC0  /* pink - wall collision */
#define GSCOLSYS_COLOR_SLOPE        0xFFFF00C0  /* yellow - slope */
#define GSCOLSYS_COLOR_BOUNDARY     0x00FFFFC0  /* cyan - boundary */

/* ===================================================================
 * Structures
 * =================================================================== */

/** 3D vector (12 bytes) */
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

/** 3x3 rotation matrix (36 bytes) -- identity matrix at lbl_80272020 */
typedef struct Mtx33f {
    f32 m[3][3];
} Mtx33f;

/**
 * WZX collision mesh header.
 *
 * The first word of the WZX data is a self-relative offset to the
 * vertex/triangle data. During relocation (fn_8010CE04), this is
 * converted to an absolute pointer by adding the base address.
 *
 * Fields:
 *   0x00: u32  vertexDataOffset / vertexDataPtr (relocated)
 *   0x04: u32  triangleCount
 */
typedef struct GSColTriangle GSColTriangle;
typedef struct GSColSubMesh GSColSubMesh;

typedef struct GSColMeshHeader {
    /* 0x00 */ GSColTriangle* vertexData; /**< Pointer to triangle array (relocated) */
    /* 0x04 */ u32    triangleCount;  /**< Number of triangles in this mesh */
} GSColMeshHeader;

/**
 * WZX triangle record -- 0x40 bytes per triangle.
 *
 * Contains 3 vertices (forming the triangle), surface attributes,
 * and pointers to sub-mesh groups for different collision layers
 * (walls, ceilings, ramps, boundaries).
 *
 * Vertex layout: v[0] at 0x00, v[1] at 0x0C, v[2] at 0x18
 * Each vertex is a Vec3f (x, y, z).
 *
 * Attribute fields at 0x24..0x3F contain sub-mesh pointers
 * that are relocated during load.
 */
struct GSColTriangle {
    /* 0x00 */ Vec3f   v[3];          /**< Triangle vertices */
    /* 0x24 */ GSColSubMesh* meshGroupA; /**< Sub-mesh group A (e.g. walkable floor) */
    /* 0x28 */ GSColSubMesh* meshGroupB; /**< Sub-mesh group B (e.g. wall surfaces) */
    /* 0x2C */ GSColSubMesh* meshGroupC; /**< Sub-mesh group C (e.g. slopes) */
    /* 0x30 */ GSColSubMesh* meshGroupD; /**< Sub-mesh group D (e.g. ceiling) */
    /* 0x34 */ GSColSubMesh* meshGroupE; /**< Sub-mesh group E (e.g. ramps) */
    /* 0x38 */ GSColSubMesh* meshGroupF; /**< Sub-mesh group F (e.g. boundary) */
    /* 0x3C */ u16     flags;         /**< Triangle flags (bit 0 = disabled) */
    /* 0x3E */ u16     pad3E;
};

/**
 * Collision sub-mesh group header.
 *
 * Each sub-mesh group (meshGroupA-F in GSColTriangle) points to one
 * of these headers, which in turn contains a vertex data pointer,
 * triangle count, and relocation offsets.
 *
 * The vertex data contains triangles in a compact format:
 *   - For "simple" groups (meshGroupC, meshGroupF): 0x30 bytes per tri
 *     (3 vertices only, no sub-mesh pointers)
 *   - For "full" groups (meshGroupA, meshGroupB, meshGroupD, meshGroupE):
 *     0x34 bytes per tri (3 vertices + color byte at 0x30)
 *
 * Each group stores a 4-byte color constant for debug visualization.
 */
struct GSColSubMesh {
    /* 0x00 */ void*  vertexData;    /**< Pointer to triangle vertex array */
    /* 0x04 */ u32    triangleCount; /**< Number of triangles */
    /* 0x08 */ void*  normalData;    /**< Optional normal data pointer */
    /* 0x0C */ void*  extraData;     /**< Optional extra data pointer */
};

/**
 * Per-triangle collision state entry -- 0x28 bytes.
 *
 * Stored within each collision layer. Contains the transformed
 * triangle position, normal, and status flags.
 *
 * Fields recovered from fn_8010CD6C (cleanup copy loop):
 *   0x00-0x1F: Copied from WZX triangle record (first 0x24 bytes
 *              of vertex/normal data, 8 words)
 *   0x20:      Additional state word
 *   0x24:      u16 flags (bit 0 = active/visible flag)
 */
typedef struct GSColTriState {
    /* 0x00 */ u32  data[9];         /**< Triangle data (position, normal, etc.) */
    /* 0x24 */ u16  flags;           /**< Status flags (bit 0 = active) */
    /* 0x26 */ u16  pad26;
} GSColTriState;

/**
 * Surface type table entry -- 0x2C bytes.
 *
 * Stored in the static table at lbl_8035B500. Contains surface
 * type attributes and a type-vs-type interaction matrix.
 *
 * Fields recovered from fn_8010C46C..fn_8010C508:
 *   0x00: u8   surfaceFlags
 *   0x02: u16  surfaceId
 *   0x04: u32  surfaceParam
 *   0x08: u16[18] typeInteraction  -- interaction codes vs other types
 */
typedef struct GSColSurfaceType {
    /* 0x00 */ u8   surfaceFlags;
    /* 0x01 */ u8   pad01;
    /* 0x02 */ u16  surfaceId;
    /* 0x04 */ u32  surfaceParam;
    /* 0x08 */ u16  typeInteraction[GSCOLSYS_NUM_SURFACE_TYPES];
} GSColSurfaceType;

/**
 * Main collision system state -- 0x3710 bytes (BSS at lbl_80404C68).
 *
 * Layout:
 *   0x0000: void*  wzxDataPtr      -- pointer to active WZX collision data
 *   0x0004: collision layers[4], each 0xDC0 bytes
 *           (layer N at offset 0x04 + N * 0xDC0)
 *   0x3704: s32    activeLayer     -- current active layer index (-1 = none)
 *   0x3708: u32    gfxRenderHandle -- GSgfx render object handle for debug draw
 *   0x370C: void*  displayList     -- cached display list for debug collision vis
 */
typedef struct GSColSysState {
    /* 0x0000 */ void*  wzxDataPtr;
    /* 0x0004 */ u8     layers[GSCOLSYS_MAX_LAYERS][GSCOLSYS_LAYER_SIZE];
    /* 0x3704 */ s32    activeLayer;
    /* 0x3708 */ u32    gfxRenderHandle;
    /* 0x370C */ void*  displayList;
} GSColSysState;

/* ===================================================================
 * Public API
 * =================================================================== */

/**
 * GScolsys2_Nop -- No-operation stub (possibly a removed debug function).
 *
 * Simply returns (blr). Called during early system init.
 *
 * Corresponds to fn_8010C220.
 */
void GScolsys2_Nop(void);

/**
 * GScolsys2_AllocBuffers -- Allocate collision system buffers.
 *
 * Allocates two buffer pools from GSmem:
 *   1. Triangle record pool: count * 0x10 bytes (indexed by triangle ID)
 *   2. Sub-mesh pool: count * 0x08 bytes per entry, each containing
 *      a 0x6EC0-byte collision data block
 *
 * The allocated buffers are stored in SDA globals (lbl_8047AD48..58).
 * Zeroes all buffer contents via memset.
 *
 * @param count  Number of collision mesh entries to allocate for.
 *
 * Corresponds to fn_8010C224.
 */
void GScolsys2_AllocBuffers(u32 count);

/**
 * GScolsys2_GetSurfaceEnabled -- Check if a surface type is enabled.
 *
 * Returns bit 0 of the byte at offset 0x00 of the surface type's
 * collision buffer entry.
 *
 * @param surfaceIndex  Surface type index (u16).
 * @return              1 if enabled, 0 if disabled or out of range.
 *
 * Corresponds to fn_8010C388.
 */
u32 GScolsys2_GetSurfaceEnabled(u16 surfaceIndex);

/**
 * GScolsys2_GetSurfaceData -- Get the data pointer for a surface type.
 *
 * Returns the 32-bit word at offset 0x04 of the surface type's
 * collision buffer entry.
 *
 * @param surfaceIndex  Surface type index (u16).
 * @return              Data pointer, or 0 if out of range.
 *
 * Corresponds to fn_8010C3FC.
 */
void* GScolsys2_GetSurfaceData(u16 surfaceIndex);

/**
 * GScolsys2_GetTypeId -- Get the surface ID for a type index.
 *
 * Looks up the surfaceId field (offset 0x02) in the type table entry
 * at index typeIndex (each entry is 0x2C bytes).
 *
 * @param typeIndex  Type table index.
 * @return           Surface ID (u16), or 0 if out of range.
 *
 * Corresponds to fn_8010C46C.
 */
u16 GScolsys2_GetTypeId(u16 typeIndex);

/**
 * GScolsys2_GetTypeFlags -- Get the surface flags for a type index.
 *
 * Looks up the surfaceFlags field (offset 0x00) in the type table.
 *
 * @param typeIndex  Type table index.
 * @return           Surface flags byte, or 0 if out of range.
 *
 * Corresponds to fn_8010C4A0.
 */
u8 GScolsys2_GetTypeFlags(u16 typeIndex);

/**
 * GScolsys2_GetTypeParam -- Get the surface parameter for a type index.
 *
 * Looks up the surfaceParam field (offset 0x04) in the type table.
 *
 * @param typeIndex  Type table index.
 * @return           Surface parameter (u32), or 0 if out of range.
 *
 * Corresponds to fn_8010C4D4.
 */
u32 GScolsys2_GetTypeParam(u16 typeIndex);

/**
 * GScolsys2_GetTypeInteraction -- Look up type interaction between two surfaces.
 *
 * Returns the interaction code (0x41='A', 0x42='B', 0x43='C', 0x3F='?')
 * for the interaction of surface type A vs surface type B.
 *
 * @param typeA      First surface type index.
 * @param typeB      Second surface type index (column in interaction table).
 * @return           Interaction code (u16), or 0 if out of range.
 *
 * Corresponds to fn_8010C508.
 */
u16 GScolsys2_GetTypeInteraction(u32 typeA, u32 typeB);

/**
 * GScolsys2_CalcAdvantage -- Calculate net advantage score between two types.
 *
 * Iterates all surface types and sums interaction scores:
 *   - Type 'C' (immune): -10 for one side, +10 for the other
 *   - Type 'B' (disadvantage): -10 for one side, +10 for the other
 *   - Type 'A' (advantage): +10 for one side
 *   - Type 9 is skipped (neutral marker)
 *
 * @param typeA      First surface type index (u16).
 * @param isTypeA    Direction flag: 1 = typeA attacks, 0 = typeA defends.
 * @return           Net advantage score (positive = advantage for typeA).
 *
 * Corresponds to fn_8010C54C.
 */
s32 GScolsys2_CalcAdvantage(u16 typeA, u8 isTypeA);

/**
 * GScolsys2_CalcGroupResult -- Determine result type for a group of interactions.
 *
 * Given a group of type indices, calculates the combined interaction
 * result against a target type.
 *
 * @param targetType   Target surface type (u16).
 * @param typeArray    Array of type indices to check.
 * @param arrayCount   Number of entries in typeArray.
 * @return             Result code: 0x41='A', 0x42='B', 0x43='C', or 0x3F='?'.
 *
 * Corresponds to fn_8010C650.
 */
u16 GScolsys2_CalcGroupResult(u16 targetType, u16* typeArray, u16 arrayCount);

/**
 * GScolsys2_DotPlaneEdge -- Compute dot product of normal with edge.
 *
 * Calculates: normal.x * (p2.x - p1.x) + normal.y * (p2.y - p1.y)
 *           + normal.z * (p2.z - p1.z)
 *
 * Used for determining which side of a collision edge a point is on.
 *
 * @param normal  Pointer to Vec3f normal vector.
 * @param p1      Pointer to Vec3f edge point 1.
 * @param p2      Pointer to Vec3f edge point 2.
 * @return        Dot product result (f32).
 *
 * Corresponds to GScolsy2UtilGetSidePlanePoint.
 */
f32 GScolsys2_DotPlaneEdge(Vec3f* normal, Vec3f* p1, Vec3f* p2);

/**
 * GScolsys2_QueryTriVisible -- Check if a triangle is visible/active.
 *
 * Looks up the triangle at the given index in the active collision layer
 * and checks its visibility flag (bit 0 of the flags halfword at offset 0x24).
 *
 * @param triIndex  Triangle index within the collision mesh.
 * @param outResult Pointer to u32 to receive result (0=visible, 1=hidden).
 * @return          0 on success, 1 if no data loaded, 2 if index out of range.
 *
 * Corresponds to fn_8010C7BC.
 */
s32 GScolsys2_QueryTriVisible(s32 triIndex, u32* outResult);

/**
 * GScolsys2_SetTriVisible -- Set or clear a triangle's visibility flag.
 *
 * Modifies bit 0 of the flags halfword at offset 0x24 in the triangle's
 * collision state entry within the active layer.
 *
 * @param triIndex  Triangle index within the collision mesh.
 * @param visible   0 = set visible (set bit), nonzero = hide (clear bit).
 * @return          0 on success, nonzero on error.
 *
 * Corresponds to fn_8010C844.
 */
s32 GScolsys2_SetTriVisible(s32 triIndex, s32 visible);

/**
 * GScolsys2_BuildTransform -- Build a transform matrix for a triangle.
 *
 * Given a triangle index, constructs a 4x3 transformation matrix from
 * the triangle's position, rotation, and scale data in the active
 * collision layer. The resulting matrix is stored in outMtx.
 *
 * @param outMtx    Destination for the 4x3 transform matrix (12 floats).
 * @param triIndex  Triangle index in the active collision layer.
 * @return          1 on success, 0 on error.
 *
 * Corresponds to fn_8010C8D0.
 */
s32 GScolsys2_BuildTransform(void* outMtx, u32 triIndex);

/**
 * GScolsys2_BuildInverseTransform -- Build an inverse transform for a triangle.
 *
 * Similar to GScolsys2_BuildTransform but produces the inverse matrix,
 * used for transforming world-space queries into collision-local space.
 *
 * @param outMtx    Destination for the inverse transform matrix.
 * @param triIndex  Triangle index in the active collision layer.
 * @return          1 on success, 0 on error.
 *
 * Corresponds to fn_8010CA30.
 */
s32 GScolsys2_BuildInverseTransform(void* outMtx, u32 triIndex);

/**
 * GScolsys2_GetWZXData -- Return the active WZX collision data pointer.
 *
 * @return  Pointer to the WZX collision mesh data, or NULL if none loaded.
 *
 * Corresponds to fn_8010CBC0.
 */
void* GScolsys2_GetWZXData(void);

/**
 * GScolsys2_GetActiveLayerPtr -- Return pointer to the active collision layer.
 *
 * @return  Pointer to the active layer data, or NULL if no valid layer.
 *
 * Corresponds to GScolsys2GetCurFloor.
 */
void* GScolsys2_GetActiveLayerPtr(void);

/**
 * GScolsys2_Reset -- Reset the collision system and free display list.
 *
 * Clears the WZX data pointer and frees the cached debug display list
 * if one exists.
 *
 * Corresponds to GScolsys2UnloadCCD.
 */
void GScolsys2_Reset(void);

/**
 * GScolsys2_Finalize -- Finalize and deactivate collision layers.
 *
 * Clears the active flag (bit 0) on all collision flag halfwords across
 * all 3 iterations within the active collision layer, then sets the
 * WZX data pointer to NULL.
 *
 * Called during floor unloading (fn_8010CC54 from gs_floor.c).
 *
 * Corresponds to fn_8010CC54.
 */
void GScolsys2_Finalize(void);

/**
 * GScolsys2_Cleanup -- Copy collision data from WZX source into active layer.
 *
 * Iterates all triangles in the WZX mesh and copies vertex/attribute data
 * from the source records (0x40 bytes each) into the active layer's
 * collision state entries (0x28 bytes each), clearing the flags halfword.
 *
 * Called during floor cleanup (fn_8010CD6C from gs_floor.c).
 *
 * Corresponds to fn_8010CD6C.
 */
void GScolsys2_Cleanup(void);

/**
 * GScolsys2_RelocateWZX -- Relocate WZX collision mesh data in place.
 *
 * Converts all relative offsets in the WZX data to absolute pointers
 * by adding the base address. Processes:
 *   - The main vertex data offset at +0x00
 *   - Per-triangle sub-mesh pointers at offsets 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38
 *   - Within each sub-mesh: vertex data, normal data, and extra data pointers
 *
 * @param wzxData  Pointer to the raw WZX data buffer.
 *
 * Corresponds to fn_8010CE04.
 */
void GScolsys2_RelocateWZX(void* wzxData);

/**
 * GScolsys2_LoadWZX -- Load and activate WZX collision data.
 *
 * Relocates the WZX data and stores it as the active collision mesh.
 * Returns 0 if no valid layer is available, 1 on success.
 *
 * @param wzxData  Pointer to the raw WZX collision data.
 * @return         1 on success, 0 on failure.
 *
 * Corresponds to fn_8010CFE4.
 */
s32 GScolsys2_LoadWZX(void* wzxData);

/**
 * GScolsys2_PopLayer -- Pop (deactivate) the current collision layer.
 *
 * Decrements the active layer index. Returns 0 if already at layer -1,
 * 1 on success.
 *
 * Corresponds to fn_8010D038.
 */
s32 GScolsys2_PopLayer(void);

/**
 * GScolsys2_Init -- Initialize a new collision layer.
 *
 * Pushes a new collision layer. Clears the active flag (bit 0) on all
 * collision flag halfwords within the new layer, then increments the
 * active layer index. Returns 0 if all 4 layers are in use.
 *
 * Called during floor init (fn_8010D064 from gs_floor.c).
 *
 * Corresponds to fn_8010D064.
 */
s32 GScolsys2_Init(void);

/**
 * GScolsys2_InitRenderer -- Initialize the collision debug renderer.
 *
 * Sets the active layer to 0, clears the display list, creates a GSgfx
 * render object, and registers two render passes for debug drawing.
 *
 * Corresponds to fn_8010D170.
 */
void GScolsys2_InitRenderer(void);

/**
 * GScolsys2_DrawTriGroup -- Draw a group of triangles for debug visualization.
 *
 * Iterates all triangles in a collision mesh group, transforms vertices
 * through the 4x3 matrix, computes vertex colors from surface type
 * attributes, and submits them as GX triangle primitives.
 *
 * @param meshData  Pointer to the GSColSubMesh for this group.
 * @param mtx       Pointer to the 4x3 transform matrix (12 floats).
 *
 * Corresponds to fn_8010D20C.
 */
void GScolsys2_DrawTriGroup(GSColSubMesh* meshData, void* mtx);

/**
 * GScolsys2_Draw -- Build a debug display list for all collision meshes.
 *
 * Sets up GX render state (TEV, Z-mode, alpha blending), allocates a
 * display list buffer, then iterates all active triangles in the
 * collision mesh. For each triangle, draws:
 *   - meshGroupA triangles (walkable surfaces) in white (0xFFFFFFFF)
 *   - meshGroupB triangles (walls) in pink (0xFF00FFC0)
 *   - meshGroupC triangles (slopes) in yellow (0xFFFF00C0)
 *   - meshGroupD triangles (boundaries) in cyan (0x00FFFFC0)
 *   - meshGroupE triangles (ramps) -- same as meshGroupA
 *   - meshGroupF triangles (ceilings) -- same as meshGroupB
 *
 * If display list allocation fails, prints:
 *   "GScolsys2Draw : can't alloc display list memory."
 *
 * Corresponds to fn_8010D3C8.
 */
void* GScolsys2_Draw(void);

/**
 * GScolsys2_DrawActive -- Draw collision for active layer with per-tri checks.
 *
 * Similar to GScolsys2_Draw but checks per-triangle visibility flags in
 * the active collision layer. Only draws triangles that are flagged as
 * active (flag bit 0 set in the WZX triangle and cleared in the layer
 * state entry). Also manages display list caching -- on first call,
 * invokes GScolsys2_Draw to build the display list, then replays it
 * on subsequent frames.
 *
 * Corresponds to fn_8010D8D4.
 */
void GScolsys2_DrawActive(void);

/**
 * GScolsys2_FindNearestGround -- Find the nearest ground triangle below a point.
 *
 * Performs a vertical ray cast downward from the given position.
 * Returns the triangle index whose Y-intersection is closest to
 * (but not above) the query point's Y coordinate.
 *
 * @param pos         Pointer to Vec3f query position.
 * @param outFloorType Pointer to u8 to receive the floor surface type.
 * @param outAttribute Pointer to u8 to receive the floor attribute.
 * @return            1 if a ground triangle was found, 0 if no hit.
 *
 * Corresponds to fn_8010DE00.
 */
s32 GScolsys2_FindNearestGround(Vec3f* pos, u8* outFloorType, u8* outAttribute);

/**
 * GScolsys2_TriangleBoundsCheck -- Check if a point is within a triangle's XZ bounds.
 *
 * Tests whether a 2D point (X, Z) falls within the bounding box and
 * edges of a collision triangle, using cross-product edge tests.
 * Also used for ray-triangle intersection culling.
 *
 * @param pos         Pointer to Vec3f with the query X and Z coordinates.
 * @param triVerts    Pointer to the triangle's 3 vertices (9 floats).
 * @return            1 if inside the triangle, 0 if outside.
 *
 * Corresponds to fn_8010DEF0.
 */
s32 GScolsys2_TriangleBoundsCheck(Vec3f* pos, f32* triVerts);

#endif /* GS_COLSYS_H */
