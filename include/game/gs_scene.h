/**
 * @file gs_scene.h
 * @brief GSscene -- Scene object lifecycle, XFB capture, and camera management.
 *
 * GSscene manages the scene-level object hierarchy:
 *   - Linked-list object pools (active/free lists)
 *   - Object spawn/despawn lifecycle
 *   - Parent-child attachment
 *   - Transform hierarchy (position, rotation, scale)
 *   - Scene camera with interpolation
 *   - XFB (framebuffer) capture for debug screenshots
 *   - Scene rendering dispatch
 *
 * Object lists stored in BSS:
 *   - Active list head: lbl_8047B188
 *   - Free list head:   lbl_8047B18C
 *   - Object count:     lbl_8047B118 (u16)
 *   - Camera state:     lbl_80478C40
 *
 * Debug strings:
 *   "gs%04d.xfb"
 *
 * Address range: 0x8017572C - 0x8017A5FC (22KB, 78 functions)
 *
 * Eleven addresses previously declared here under invented GSscene_*
 * names collided with real, differently-named symbols.txt entries
 * (psKillAllGenerator, psRemoveGenerator, psGetNewIDNum) or are still
 * unnamed fn_ scaffolds; those declarations have been corrected/removed
 * below (see gs_scene.c's header comment for the full mapping). The
 * genuinely-matched GSscene_* declarations (camera vectors, mode) are
 * real and unchanged.
 */
#ifndef GS_SCENE_H
#define GS_SCENE_H

#include "dolphin/types.h"

/* ===================================================================
 * Structures
 * =================================================================== */

typedef struct GSSceneObject {
    /* 0x00 */ struct GSSceneObject* next;
    /* 0x04 */ void* parent;
    /* 0x08 */ f32 posX;
    /* 0x0C */ f32 posY;
    /* 0x10 */ f32 posZ;
    /* 0x12 */ u16 flags;
    /* 0x14 */ f32 rotY;
    /* 0x18 */ f32 pad18[2];
    /* 0x20 */ f32 animParam;
    /* 0x24 */ u8  pad24[0x28];
    /* 0x4C */ void* attachedResource;
    /* 0x50 */ void* attachedModel;
} GSSceneObject;

typedef struct GSSceneRenderEntry {
    /* 0x00 */ u32  count;
    /* 0x04 */ void* objectPtr;
    /* 0x08 */ u8   pad[0x20];
} GSSceneRenderEntry;

/* ===================================================================
 * Public API
 * =================================================================== */

/** real name (symbols.txt) */ void   psKillAllGenerator(void);
/** real name (symbols.txt) */ void*  psRemoveGenerator(u32 type, u32 param);
/** real name (symbols.txt) */ u32    psGetNewIDNum(void);
/** unnamed scaffold */ void   fn_80177A64(u32 captureIndex);
/** unnamed scaffold */ void   fn_80178AA8(void* sceneObj);
/** unnamed scaffold */ void   fn_80179020(void* camera);
/** 0x801778B4 */ void   GSscene_GetCameraDirectionVector(void* dst);
/** 0x801778DC */ void   GSscene_SetCameraDirectionVector(void* src);
/** 0x80177830 */ void   GSscene_GetCameraRotationVector(void* dst);
/** 0x80177858 */ void   GSscene_SetCameraRotationVector(void* src);
/** 0x80177908 */ void   GSscene_GetCameraPositionVector(void* dst);
/** 0x80177930 */ void   GSscene_SetCameraPositionVector(void* src);
/** 0x8017795C */ void   GSscene_GetCameraViewVector(void* dst);
/** 0x80177984 */ void   GSscene_SetCameraViewVector(void* src);
/** 0x80177A38 */ u32    GSscene_GetMode(void);
/** 0x80177A44 */ u32    GSscene_SetMode(u32 mode);
/** unnamed scaffold */ void   fn_80179FA4(void);
/** unnamed scaffold */ f32    fn_80176948(void* obj);
/** unnamed scaffold */ f32    fn_8017697C(void* obj);
/** unnamed scaffold */ f32    fn_801769B0(void* obj);

#endif /* GS_SCENE_H */
