/**
 * @file gs_model.h
 * @brief GSmodel -- 3D model management system for Pokemon Colosseum.
 *
 * GSmodel manages the lifecycle of 3D models in the engine:
 *   - Loading model data from FSYS archives into GSmem
 *   - Joint/skeletal hierarchy setup
 *   - Animation attachment and playback
 *   - Vertex skinning
 *   - Display list generation and caching
 *   - Model rendering through GSgfx
 *
 * The model resource table is a BSS array at lbl_80402518 (0x2400 bytes)
 * containing up to 0x60 slots of 0x48 bytes each.
 *
 * Address range: 0x80101910 - 0x8010C220 (44KB, 145 functions)
 */
#ifndef GS_MODEL_H
#define GS_MODEL_H

#include "dolphin/types.h"

/* ===================================================================
 * Constants
 * =================================================================== */

#define GSMODEL_TABLE_SIZE      0x2400
#define GSMODEL_SLOT_SIZE       0x48
#define GSMODEL_MAX_SLOTS       0x60

/* Animation playback flags */
#define GSMODEL_ANIM_LOOP       0x01
#define GSMODEL_ANIM_PINGPONG   0x02
#define GSMODEL_ANIM_REVERSE    0x04
#define GSMODEL_ANIM_PAUSED     0x08

/* ===================================================================
 * Structures
 * =================================================================== */

typedef struct GSModelSlot {
    /* 0x00 */ u32  flags;
    /* 0x04 */ u16  memHandle;
    /* 0x06 */ u16  animHandle;
    /* 0x08 */ void* dataPtr;
    /* 0x0C */ void* animPtr;
    /* 0x10 */ u32  jointCount;
    /* 0x14 */ void* jointTable;
    /* 0x18 */ f32  animFrame;
    /* 0x1C */ f32  animSpeed;
    /* 0x20 */ f32  animEnd;
    /* 0x24 */ u32  animFlags;
    /* 0x28 */ void* displayList;
    /* 0x2C */ u32  dlSize;
    /* 0x30 */ void* skinWeights;
    /* 0x34 */ u32  vertexCount;
    /* 0x38 */ void* materialPtr;
    /* 0x3C */ void* texturePtr;
    /* 0x40 */ u32  resourceId;
    /* 0x44 */ u32  refCount;
} GSModelSlot;

/* ===================================================================
 * Public API
 * =================================================================== */

/** fn_80101910 */ s32   GSmodel_FindLoadedResource(void* query);
/** fn_801019F8 */ void  GSmodel_ClearResourceTable(void);
/** fn_80101A28 */ u32   GSmodel_GetResourceCount(void);
/** fn_80101A4C */ void* GSmodel_GetResourceByIndex(u32 index);
/** fn_80102004 */ u32   GSmodel_GetJointCount(void* model);
/** fn_801028B0 */ void  GSmodel_AttachAnimation(void* model, void* animData, f32 startFrame);
/** fn_80102F38 */ void  GSmodel_UpdateAnimation(void* model);
/** fn_801074D4 */ void  GSmodel_RenderModel(void* model);
/** fn_80108580 */ void  GSmodel_BuildDisplayList(void* model);
/** fn_80108C14 */ void  GSmodel_ProcessSkinning(void* model);

#endif /* GS_MODEL_H */
