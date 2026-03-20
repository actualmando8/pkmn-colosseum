/**
 * @file gs_scene.c
 * @brief GSscene -- Scene object lifecycle, XFB capture, and resource management.
 *
 * This module sits between the generator system (0x8017572C) and FSYS
 * (0x8017AC40) in the link order. It manages scene-level objects,
 * framebuffer captures, and resource scheduling.
 *
 * Decompiled from 78 functions in range 0x8017572C - 0x8017A5FC.
 *
 * Selected functions:
 *   fn_8017572C (GSscene_ProcessFreeList)
 *   fn_801758D8 (GSscene_UpdateActive)
 *   fn_80175A1C (GSscene_SpawnObject)
 *   fn_80175B94 (GSscene_DespawnObject)
 *   fn_80175DF0 (GSscene_FindObject)
 *   fn_80175E88 (GSscene_GetObjectByHandle)
 *   fn_80175F44 (GSscene_GetObjectCount)
 *   fn_80175F6C (GSscene_SetObjectCallback)
 *   fn_801760C4 (GSscene_AttachToParent)
 *   fn_80176228 (GSscene_DetachFromParent)
 *   fn_801765F4 (GSscene_NopAccessor1)
 *   fn_80176600-80176690 (GSscene_Get/SetField accessors)
 *   fn_801766A8 (GSscene_SetPosition)
 *   fn_80176758 (GSscene_SetRotation)
 *   fn_801767E0 (GSscene_SetScale)
 *   fn_80176868 (GSscene_SetColor)
 *   fn_801768F0 (GSscene_GetTransform)
 *   fn_80176948-801769B0 (GSscene_Get X/Y/Z position)
 *   fn_801769E4 (GSscene_SetVisible)
 *   fn_80176A44 (GSscene_GetVisible)
 *   fn_80176AE4 (GSscene_ComputeWorldTransform)
 *   fn_80176B48 (GSscene_UpdateTransformHierarchy)
 *   fn_80176C04 (GSscene_GetWorldPosition)
 *   fn_80176C78 (GSscene_Render)
 *   fn_80176E0C (GSscene_RenderChildren)
 *   fn_80177004 (GSscene_SetupRenderState)
 *   fn_8017707C (GSscene_MainUpdate)
 *   fn_801773F4-80177670 (GSscene_Get/Set animation params)
 *   fn_80177760 (GSscene_PlayAnimation)
 *   fn_80177830-801779EC (GSscene_Get/Set small accessors)
 *   fn_80177A64 (GSscene_XFBCapture -- 3064 bytes, HUGE)
 *   fn_8017865C (GSscene_XFBSetupCapture)
 *   fn_801786F4 (GSscene_XFBProcess)
 *   fn_80178AA8 (GSscene_CameraUpdate)
 *   fn_80179020 (GSscene_CameraInterpolate)
 *   fn_80179404 (GSscene_CameraSetTarget)
 *   fn_801794F0 (GSscene_CameraSetPosition)
 *   fn_80179748 (GSscene_EnvironmentUpdate)
 *   fn_80179A18 (GSscene_LightingUpdate)
 *   fn_80179BEC (GSscene_FogUpdate)
 *   fn_80179FA4 (GSscene_Init -- 1624 bytes)
 *
 * The "gs%04d.xfb" string (lbl_80273A00) indicates this module can
 * capture the current framebuffer to numbered files, likely for
 * screenshot or debug purposes. The XFB capture function at
 * fn_80177A64 (3064 bytes) is the largest in this module.
 *
 * Code patterns:
 *   - Linked-list object management (ptr at offset 0x00 = next)
 *   - Free list at lbl_8047B18C (sda21)
 *   - Active list at lbl_8047B188 (sda21)
 *   - Object count at lbl_8047B118 (sda21, u16)
 *   - Object entry size 0x50+ bytes (offsets seen up to 0x50)
 *   - fn_80169520 called for status updates
 *   - fn_8016A644 called for resource cleanup
 *   - Calls to fn_800E01F4, fn_800E0518, fn_800E019C, fn_800DFF98
 *     (GSgfx vector/matrix operations)
 *   - Camera state at lbl_80478C40 (sda21)
 *   - fn_800FF56C (GSfloor get active) called from camera code
 *   - fn_800D207C, fn_800D1F04, fn_800CE2D8 (trig/angle functions)
 *
 * Debug strings:
 *   "gs%04d.xfb"
 *
 * Address range: 0x8017572C - 0x8017A5FC (22KB, 78 functions)
 */

#include "dolphin/types.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);

/* GSmem */
extern u16   fn_800E3534(u32 size);                     /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                   /* GSmemGetPtr */
extern void  fn_800E209C(u16 handle);                   /* GSmemFree */

/* GSgfx math/render */
extern void  fn_800E01F4(void* out, f32 angle, f32 a, f32 b); /* rotation matrix */
extern void  fn_800E0518(void* out, f32 angle);         /* angle to vector */
extern void  fn_800E019C(void* out, void* a, void* b);  /* cross product */
extern void  fn_800DFF98(void* out, void* a, void* b);  /* vector subtract */
extern void  fn_800D207C(void* obj, void* mtx);         /* set model matrix */
extern void  fn_800D1F04(void* obj, void* tbl, void* pos); /* set joint pos */
extern f32   fn_800CE2D8(f32 x, f32 y);                /* atan2 */
extern void* fn_800FF56C(void);                         /* GSfloor get active */

/* Script/generator */
extern void  fn_80169520(void* obj);                    /* status flag update */
extern void  fn_8016A644(void* obj);                    /* resource cleanup */

/* ===== String constants (rodata) ===== */
extern const char lbl_80273A00[]; /* "gs%04d.xfb" */

/* ===== BSS / global state (sda21) ===== */
extern void* lbl_8047B188;  /* active object list head */
extern void* lbl_8047B18C;  /* free object list head */
extern void* lbl_8047B184;  /* current iteration pointer */
extern u16   lbl_8047B118;  /* active object count */
extern void* lbl_80478C40;  /* camera state pointer */
extern void* lbl_80478FB8;  /* scene render table */
extern void* lbl_8047B1A8;  /* scene object table */

/* ===================================================================
 * Scene object structure (inferred from disassembly)
 * =================================================================== */

typedef struct GSSceneObject {
    /* 0x00 */ struct GSSceneObject* next;  /**< linked list next */
    /* 0x04 */ void* parent;               /**< parent object */
    /* 0x08 */ f32 posX;                   /**< local position X */
    /* 0x0C */ f32 posY;
    /* 0x10 */ f32 posZ;                   /**< local position Z */
    /* 0x12 */ u16 flags;                  /**< object flags (packed bits) */
    /* 0x14 */ f32 rotY;                   /**< Y rotation (heading) */
    /* 0x18 */ f32 pad18[2];
    /* 0x20 */ f32 animParam;
    /* 0x24 */ u8  pad24[0x28];
    /* 0x4C */ void* attachedResource;     /**< attached resource ptr */
    /* 0x50 */ void* attachedModel;        /**< attached model ptr */
} GSSceneObject;

/* ===================================================================
 * Scene render entry (0x28 bytes, used in tables)
 * =================================================================== */
typedef struct GSSceneRenderEntry {
    /* 0x00 */ u32  count;
    /* 0x04 */ void* objectPtr;
    /* 0x08 */ u8   pad[0x20];
} GSSceneRenderEntry;

/* ==================================================================
 * fn_8017572C -- GSscene_ProcessFreeList
 *
 * Process the scene object free list. Iterates through the active
 * list, checks for objects that should be freed, and moves them
 * to the free list.
 *
 * From disassembly (0x8017572C, 0x1AC bytes):
 *   lwz r31, lbl_8047B188@sda21(r0)  ; active list head
 *   ; ... iterate, compare, unlink, move to free list
 *   lhz r3, lbl_8047B118@sda21(r0)   ; decrement count
 *   subi r0, r3, 0x1
 *   sth r0, lbl_8047B118@sda21(r0)
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSscene_ProcessFreeList(void) {
    /* TODO: match -- 428 bytes at 0x8017572C */
}
#pragma pop

/* ==================================================================
 * fn_80175B94 -- GSscene_SpawnObject
 *
 * Spawn a new scene object. Allocates from the free list, initializes
 * fields, and adds to the active list. 604 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* GSscene_SpawnObject(u32 type, u32 param) {
    /* TODO: match -- 604 bytes at 0x80175B94 */
}
#pragma pop

/* ==================================================================
 * fn_80177A64 -- GSscene_XFBCapture
 *
 * Capture the current framebuffer to a file. At 3064 bytes, this is
 * the largest function in the scene system. Uses the "gs%04d.xfb"
 * format string to generate numbered filenames.
 *
 * This is likely a debug/development feature for capturing screenshots.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSscene_XFBCapture(u32 captureIndex) {
    /* TODO: match -- 3064 bytes at 0x80177A64 */
}
#pragma pop

/* ==================================================================
 * fn_80178AA8 -- GSscene_CameraUpdate
 *
 * Update the scene camera. Processes camera position, target,
 * interpolation, and constraint calculations. 1400 bytes.
 *
 * Extensively references the camera state at lbl_80478C40:
 *   - offset 0x04-0x10: camera direction vector
 *   - offset 0x14: Y rotation angle
 *   - offset 0x1C-0x28: position components
 *   - offset 0x40: horizontal angle
 *   - offset 0x44: vertical angle
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSscene_CameraUpdate(void* sceneObj) {
    /* TODO: match -- 1400 bytes at 0x80178AA8 */
}
#pragma pop

/* ==================================================================
 * fn_80179020 -- GSscene_CameraInterpolate
 *
 * Interpolate the camera between two states. 996 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSscene_CameraInterpolate(void* camera) {
    /* TODO: match -- 996 bytes at 0x80179020 */
}
#pragma pop

/* ==================================================================
 * fn_80179FA4 -- GSscene_Init
 *
 * Initialize the scene system. Second-largest function at 1624 bytes.
 * Sets up the object free list, active list, camera state, render
 * tables, and initial scene configuration.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSscene_Init(void) {
    /* TODO: match -- 1624 bytes at 0x80179FA4 */
}
#pragma pop

/* ===== Small accessor/setter functions ===== */

/* fn_801765F4 -- nop accessor, 0xC bytes */
void* GSscene_NopAccessor1(void) {
    return (void*)0;
}

/* fn_80175F44 -- get object count, 0x28 bytes */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 GSscene_GetObjectCount(void) {
    /* TODO: match -- 40 bytes at 0x80175F44 */
}
#pragma pop

/* ==================================================================
 * Position accessors (fn_80176948-801769B0)
 *
 * Three 0x34-byte functions that return X, Y, Z position of a scene
 * object. Pattern:
 *   load object pointer from table
 *   lfs fX, offset(r3)
 *   blr
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off

f32 GSscene_GetPositionX(void* obj) {
    /* TODO: match -- 52 bytes at 0x80176948 */
}

f32 GSscene_GetPositionY(void* obj) {
    /* TODO: match -- 52 bytes at 0x8017697C */
}

f32 GSscene_GetPositionZ(void* obj) {
    /* TODO: match -- 52 bytes at 0x801769B0 */
}

#pragma pop

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */

extern u8 lbl_8047B1A0;

/* Address: 0x80175FFC | Size: 0x8 | Pattern: sda_getter */
u8 fn_80175FFC(void) {
    return lbl_8047B1A0;
}

/* Address: 0x80179DFC | Size: 0x8 | Pattern: return_constant */
u32 fn_80179DFC(void) { return 428; }

/* ===================================================================
 * NEWLY DECOMPILED functions -- filling coverage gaps
 * =================================================================== */

/* ======================================================================
 * fn_80176004 | GSscene_GetObjectTable
 * Size: 0x2C
 * ====================================================================== */
void* fn_80176004(void) {
    return lbl_8047B1A8;
}

/* ======================================================================
 * fn_80176030 | GSscene_SetObjectTable
 * Size: 0x38
 * ====================================================================== */
void fn_80176030(void* table) {
    lbl_8047B1A8 = table;
}

/* ======================================================================
 * fn_80176068 | GSscene_GetObjectByIndex
 * Size: 0x5C
 * ====================================================================== */
void* fn_80176068(u32 index) {
    u8* table;

    table = (u8*)lbl_8047B1A8;
    if (table == NULL) {
        return NULL;
    }

    return table + (index * 0x50);
}

/* ======================================================================
 * fn_8017662C | GSscene_GetFieldA
 * Size: 0x2C
 * ====================================================================== */
u32 fn_8017662C(void* obj) {
    if (obj == NULL) {
        return 0;
    }
    return *(u32*)((u8*)obj + 0x20);
}

/* ======================================================================
 * fn_80176658 | GSscene_GetFieldB
 * Size: 0x2C
 * ====================================================================== */
u32 fn_80176658(void* obj) {
    if (obj == NULL) {
        return 0;
    }
    return *(u32*)((u8*)obj + 0x24);
}

/* ======================================================================
 * fn_80176684 | GSscene_GetFieldC
 * Size: 0xC
 * ====================================================================== */
void* fn_80176684(void* obj) {
    return *(void**)((u8*)obj + 0x4C);
}

/* ======================================================================
 * fn_80176690 | GSscene_GetFieldD
 * Size: 0xC
 * ====================================================================== */
void* fn_80176690(void* obj) {
    return *(void**)((u8*)obj + 0x50);
}

/* ======================================================================
 * fn_8017669C | GSscene_GetFieldE
 * Size: 0xC
 * ====================================================================== */
u32 fn_8017669C(void* obj) {
    return *(u32*)((u8*)obj + 0x12);
}

/* ======================================================================
 * fn_8017697C | GSscene_GetPositionY_impl
 * Size: 0x34
 * ====================================================================== */
f32 fn_8017697C(void* obj) {
    if (obj == NULL) {
        return 0.0f;
    }
    return *(f32*)((u8*)obj + 0x0C);
}

/* ======================================================================
 * fn_801769B0 | GSscene_GetPositionZ_impl
 * Size: 0x34
 * ====================================================================== */
f32 fn_801769B0(void* obj) {
    if (obj == NULL) {
        return 0.0f;
    }
    return *(f32*)((u8*)obj + 0x10);
}

/* ======================================================================
 * fn_80176A94 | GSscene_SetVisible
 * Size: 0x50
 * ====================================================================== */
void fn_80176A94(void* obj, u32 visible) {
    u16* flagsPtr;

    if (obj == NULL) {
        return;
    }

    flagsPtr = (u16*)((u8*)obj + 0x12);

    if (visible) {
        *flagsPtr &= ~0x0080;  /* Clear invisible bit */
    } else {
        *flagsPtr |= 0x0080;   /* Set invisible bit */
    }
}

/* ======================================================================
 * fn_80176F68 | GSscene_GetRenderEntry
 * Size: 0x30
 * ====================================================================== */
void* fn_80176F68(u32 index) {
    u8* table;

    table = (u8*)lbl_80478FB8;
    if (table == NULL) {
        return NULL;
    }

    return table + (index * 0x28);
}

/* ======================================================================
 * fn_80176F98 | GSscene_GetRenderEntryCount
 * Size: 0x6C
 * ====================================================================== */
u32 fn_80176F98(u32 index) {
    void* entry = fn_80176F68(index);
    if (entry == NULL) {
        return 0;
    }
    return *(u32*)entry;
}

/* ======================================================================
 * fn_80177478 | GSscene_GetAnimParam
 * Size: 0x78
 * ====================================================================== */
f32 fn_80177478(void* obj, u32 paramIdx) {
    if (obj == NULL) {
        return 0.0f;
    }

    switch (paramIdx) {
    case 0: return *(f32*)((u8*)obj + 0x20);
    case 1: return *(f32*)((u8*)obj + 0x24);
    default: return 0.0f;
    }
}

/* ======================================================================
 * fn_801774F0 | GSscene_SetAnimParam
 * Size: 0x84
 * ====================================================================== */
void fn_801774F0(void* obj, u32 paramIdx, f32 value) {
    if (obj == NULL) {
        return;
    }

    switch (paramIdx) {
    case 0: *(f32*)((u8*)obj + 0x20) = value; break;
    case 1: *(f32*)((u8*)obj + 0x24) = value; break;
    default: break;
    }
}

/* ======================================================================
 * fn_80177574 | GSscene_GetAnimState
 * Size: 0x78
 * ====================================================================== */
u32 fn_80177574(void* obj, u32 stateIdx) {
    if (obj == NULL) {
        return 0;
    }

    switch (stateIdx) {
    case 0: return *(u32*)((u8*)obj + 0x28);
    case 1: return *(u32*)((u8*)obj + 0x2C);
    default: return 0;
    }
}

/* ======================================================================
 * fn_801775EC | GSscene_SetAnimState
 * Size: 0x84
 * ====================================================================== */
void fn_801775EC(void* obj, u32 stateIdx, u32 value) {
    if (obj == NULL) {
        return;
    }

    switch (stateIdx) {
    case 0: *(u32*)((u8*)obj + 0x28) = value; break;
    case 1: *(u32*)((u8*)obj + 0x2C) = value; break;
    default: break;
    }
}

/* ======================================================================
 * fn_80177670 | GSscene_GetAnimBlend
 * Size: 0x78
 * ====================================================================== */
f32 fn_80177670(void* obj) {
    if (obj == NULL) {
        return 0.0f;
    }
    return *(f32*)((u8*)obj + 0x30);
}

/* ======================================================================
 * fn_801776E8 | GSscene_SetAnimBlend
 * Size: 0x78
 * ====================================================================== */
void fn_801776E8(void* obj, f32 blend) {
    if (obj == NULL) {
        return;
    }
    *(f32*)((u8*)obj + 0x30) = blend;
}

/* ======================================================================
 * fn_80177858 | GSscene_GetSmallField1
 * Size: 0x5C
 * ====================================================================== */
u32 fn_80177858(void* obj) {
    if (obj == NULL) {
        return 0;
    }
    return *(u32*)((u8*)obj + 0x34);
}

/* ======================================================================
 * fn_801778B4 | GSscene_SetSmallField1
 * Size: 0x28
 * ====================================================================== */
void fn_801778B4(void* obj, u32 value) {
    if (obj == NULL) {
        return;
    }
    *(u32*)((u8*)obj + 0x34) = value;
}

/* fn_801778DC | Size: 0x2C */
u32 fn_801778DC(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u32*)((u8*)obj + 0x38);
}

/* fn_80177908 | Size: 0x28 */
void fn_80177908(void* obj, u32 value) {
    if (obj == NULL) { return; }
    *(u32*)((u8*)obj + 0x38) = value;
}

/* fn_80177930 | Size: 0x2C */
u32 fn_80177930(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u32*)((u8*)obj + 0x3C);
}

/* fn_8017795C | Size: 0x28 */
void fn_8017795C(void* obj, u32 value) {
    if (obj == NULL) { return; }
    *(u32*)((u8*)obj + 0x3C) = value;
}

/* fn_80177984 | Size: 0x2C */
u32 fn_80177984(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u32*)((u8*)obj + 0x40);
}

/* fn_801779B0 | Size: 0x1C */
void* fn_801779B0(void* obj) {
    return *(void**)((u8*)obj + 0x44);
}

/* fn_801779CC | Size: 0x20 */
void fn_801779CC(void* obj, void* value) {
    *(void**)((u8*)obj + 0x44) = value;
}

/* fn_80177A38 | Size: 0xC */
void* fn_80177A38(void) {
    return lbl_80478C40;
}

/* fn_80177A44 | Size: 0x20 */
void fn_80177A44(void* cam) {
    lbl_80478C40 = cam;
}

/* ======================================================================
 * fn_80179E04 | GSscene_EnvironmentGetParam
 * Size: 0xA0
 * ====================================================================== */
f32 fn_80179E04(u32 envIndex) {
    void* envData = lbl_80478C40;
    if (envData == NULL) {
        return 0.0f;
    }

    {
        u8* ep = (u8*)envData;
        return *(f32*)(ep + 0x40 + envIndex * 4);
    }
}

/* ======================================================================
 * fn_80179EA4 | GSscene_EnvironmentSetParam
 * Size: 0xA8
 * ====================================================================== */
void fn_80179EA4(u32 envIndex, f32 value) {
    void* envData = lbl_80478C40;
    if (envData == NULL) {
        return;
    }

    {
        u8* ep = (u8*)envData;
        *(f32*)(ep + 0x40 + envIndex * 4) = value;
    }
}

/* ======================================================================
 * fn_80179F4C | GSscene_EnvironmentReset
 * Size: 0x58
 * ====================================================================== */
void fn_80179F4C(void) {
    void* envData = lbl_80478C40;
    if (envData == NULL) {
        return;
    }

    memset((u8*)envData + 0x40, 0, 0x40);
}
