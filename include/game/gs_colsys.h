/**
 * @file gs_colsys.h
 * @brief GScolsys2 -- surface-type interaction table + matched leaf helpers.
 *
 * Backs src/game/gs_colsys.c, unit 0x8010C364-0x8010CBD0 (16 fns, 9
 * matched). Declares only what that unit's real, compiled code actually
 * needs: the collision-state struct (lbl_80404C68), the surface-type
 * table entry layout (lbl_8035B500), and the handful of size/count
 * constants those two rely on.
 *
 * A prior campaign-generation transplant had attached a full invented
 * "GScolsys2_*" public API to this header (WZX mesh loading/relocation,
 * transform building, debug-draw display lists, ground raycasting --
 * ~30 prototypes plus supporting mesh/triangle structs), matching a
 * fiction block that was removed from gs_colsys.c as dead, unreferenced
 * code (see that file's header comment). None of those names appear in
 * symbols.txt and nothing outside gs_colsys.c ever included this header,
 * so the fictional declarations have been removed along with it.
 *
 * Address range: 0x8010C364 - 0x8010CBD0
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

/** Number of surface types in the type table */
#define GSCOLSYS_NUM_SURFACE_TYPES  0x12

/* ===================================================================
 * Structures
 * =================================================================== */

/** 3D vector (12 bytes) -- parameter type of the matched
 *  GScolsy2UtilGetSidePlanePoint. */
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

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

#endif /* GS_COLSYS_H */
