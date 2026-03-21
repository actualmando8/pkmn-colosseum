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

/** fn_8017572C */ void   GSscene_ProcessFreeList(void);
/** fn_80175B94 */ void*  GSscene_SpawnObject(u32 type, u32 param);
/** fn_80175F44 */ u32    GSscene_GetObjectCount(void);
/** fn_80177A64 */ void   GSscene_XFBCapture(u32 captureIndex);
/** fn_80178AA8 */ void   GSscene_CameraUpdate(u32 sceneObj);
/** fn_80179020 */ void   GSscene_CameraInterpolate(u32 camera);
/** fn_80179FA4 */ void   GSscene_Init(void);
/** fn_80176948 */ f32    GSscene_GetPositionX(u32 obj);
/** fn_8017697C */ f32    GSscene_GetPositionY(void* obj);
/** fn_801769B0 */ f32    GSscene_GetPositionZ(void* obj);

#endif /* GS_SCENE_H */
