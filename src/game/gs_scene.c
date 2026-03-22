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
void GSscene_ProcessFreeList(void) {
    extern u8 lbl_8047B180[];
    extern u8 lbl_8047D6B0[];
    extern void fn_801A3E64();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r31 = *(u32*)&lbl_8047B188;
    while (1) {
        if (r31 == 0) break;
        r30 = *(u32*)((u8*)r31 + 0x0);
        tmp = 0x0;
        r3 = *(u32*)&lbl_8047B188;
        *(u32*)&lbl_8047B184 = tmp;
        while (1) {
            if (r3 == 0) break;
            if (r3 == r31) {
                tmp = *(u16*)((u8*)r31 + 0x12);
                r29 = *(u32*)&lbl_8047B184;
                tmp = tmp & 0x00000080;
                if (r3 != r31) {
                    r3 = r31;
                    ((void(*)(void))fn_80169520)();
                }
                tmp = *(u32*)((u8*)r31 + 0x4C);
                if (tmp != 0) {
                    f0 = *(f32*)lbl_8047D6B0;
                    tmp = 0x1;
                    r29 = r31;
                    *(f32*)((u8*)r31 + 0x8) = f0;
                    *(u16*)((u8*)r31 + 0x10) = tmp;
                    goto L_80175834;
                }
                tmp = *(u16*)((u8*)r31 + 0x12);
                tmp = tmp & 0x00003800;
                do {
                    if (tmp == 0) break;
                    r3 = *(u32*)((u8*)r31 + 0x50);
                    if (r3 == 0) break;
                    tmp = *(u32*)((u8*)r3 + 0x4);
                    if (tmp != r31) break;
                    tmp = *(u16*)((u8*)r3 + 0x32);
                    if (tmp == 1) break;
                    f0 = *(f32*)lbl_8047D6B0;
                    tmp = 0x1;
                    r29 = r31;
                    *(f32*)((u8*)r31 + 0x8) = f0;
                    *(u16*)((u8*)r31 + 0x10) = tmp;
                    goto L_80175834;
                } while (0);
                if (r29 == 0) {
                    tmp = *(u32*)((u8*)r31 + 0x0);
                    *(u32*)&lbl_8047B188 = tmp;
                } else {

                    tmp = *(u32*)((u8*)r31 + 0x0);
                    *(u32*)((u8*)r29 + 0x0) = tmp;
                }
                tmp = *(u32*)((u8*)r31 + 0x50);
                if (tmp != 0) {
                    r3 = r31;
                    ((void(*)(void))fn_8016A644)();
                }
                tmp = *(u32*)&lbl_8047B18C;
                *(u32*)((u8*)r31 + 0x0) = tmp;
                r3 = *(u16*)&lbl_8047B118;
                *(u32*)&lbl_8047B18C = r31;
                *(u16*)&lbl_8047B118 = tmp;
            L_80175834:
                *(u32*)&lbl_8047B184 = r29;
                if (r29 != 0) {
                    while (1) {
                        r3 = *(u32*)&lbl_8047B184;
                        tmp = *(u32*)((u8*)r3 + 0x0);
                        if (tmp == 0) break;
                        *(u32*)&lbl_8047B184 = tmp;

                    }
                    break;
                }
                tmp = *(u32*)&lbl_8047B188;
                if (tmp == 0) break;
                *(u32*)&lbl_8047B184 = tmp;
                while (1) {
                    r3 = *(u32*)&lbl_8047B184;
                    tmp = *(u32*)((u8*)r3 + 0x0);
                    if (tmp == 0) break;
                    *(u32*)&lbl_8047B184 = tmp;

                }
                break;
            }
            *(u32*)&lbl_8047B184 = r3;
            r3 = *(u32*)((u8*)r3 + 0x0);

        }

        r31 = r30;

    }
    while (1) {
        r3 = *(u32*)lbl_8047B180;
        if (r3 == 0) break;
        fn_801A3E64();
        *(u32*)lbl_8047B180 = r3;

    }
    return;
}

/* ==================================================================
 * fn_80175B94 -- GSscene_SpawnObject
 *
 * Spawn a new scene object. Allocates from the free list, initializes
 * fields, and adds to the active list. 604 bytes.
 * ================================================================== */
void* GSscene_SpawnObject(u32 type, u32 param) {
    extern u8 lbl_8047B180[];
    extern u8 lbl_8047B190[];
    extern u8 lbl_8047D6B0[];
    extern void fn_801698F8();
    extern void fn_801A3E64();
    extern void fn_801A6960();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;

    fn_801698F8();
    r30 = *(u32*)&lbl_8047B188;
    while (1) {
        if (r30 == 0) break;
        r31 = *(u32*)((u8*)r30 + 0x0);
        tmp = 0x0;
        type = *(u32*)&lbl_8047B188;
        *(u32*)&lbl_8047B184 = tmp;
        while (1) {
            if (type == 0) break;
            if (type == r30) {
                tmp = *(u16*)((u8*)r30 + 0x12);
                r29 = *(u32*)&lbl_8047B184;
                tmp = tmp & 0x00000080;
                if (type != r30) {
                    type = r30;
                    ((void(*)(void))fn_80169520)();
                }
                tmp = *(u32*)((u8*)r30 + 0x4C);
                if (tmp != 0) {
                    f0 = *(f32*)lbl_8047D6B0;
                    tmp = 0x1;
                    r29 = r30;
                    *(f32*)((u8*)r30 + 0x8) = f0;
                    *(u16*)((u8*)r30 + 0x10) = tmp;
                    goto L_80175CA0;
                }
                tmp = *(u16*)((u8*)r30 + 0x12);
                tmp = tmp & 0x00003800;
                do {
                    if (tmp == 0) break;
                    type = *(u32*)((u8*)r30 + 0x50);
                    if (type == 0) break;
                    tmp = *(u32*)((u8*)type + 0x4);
                    if (tmp != r30) break;
                    tmp = *(u16*)((u8*)type + 0x32);
                    if (tmp == 1) break;
                    f0 = *(f32*)lbl_8047D6B0;
                    tmp = 0x1;
                    r29 = r30;
                    *(f32*)((u8*)r30 + 0x8) = f0;
                    *(u16*)((u8*)r30 + 0x10) = tmp;
                    goto L_80175CA0;
                } while (0);
                if (r29 == 0) {
                    tmp = *(u32*)((u8*)r30 + 0x0);
                    *(u32*)&lbl_8047B188 = tmp;
                } else {

                    tmp = *(u32*)((u8*)r30 + 0x0);
                    *(u32*)((u8*)r29 + 0x0) = tmp;
                }
                tmp = *(u32*)((u8*)r30 + 0x50);
                if (tmp != 0) {
                    type = r30;
                    ((void(*)(void))fn_8016A644)();
                }
                tmp = *(u32*)&lbl_8047B18C;
                *(u32*)((u8*)r30 + 0x0) = tmp;
                type = *(u16*)&lbl_8047B118;
                *(u32*)&lbl_8047B18C = r30;
                *(u16*)&lbl_8047B118 = tmp;
            L_80175CA0:
                *(u32*)&lbl_8047B184 = r29;
                if (r29 != 0) {
                    while (1) {
                        type = *(u32*)&lbl_8047B184;
                        tmp = *(u32*)((u8*)type + 0x0);
                        if (tmp == 0) break;
                        *(u32*)&lbl_8047B184 = tmp;

                    }
                    break;
                }
                tmp = *(u32*)&lbl_8047B188;
                if (tmp == 0) break;
                *(u32*)&lbl_8047B184 = tmp;
                while (1) {
                    type = *(u32*)&lbl_8047B184;
                    tmp = *(u32*)((u8*)type + 0x0);
                    if (tmp == 0) break;
                    *(u32*)&lbl_8047B184 = tmp;

                }
                break;
            }
            *(u32*)&lbl_8047B184 = type;
            type = *(u32*)((u8*)type + 0x0);

        }

        r30 = r31;

    }
    while (1) {
        type = *(u32*)lbl_8047B180;
        if (type == 0) break;
        fn_801A3E64();
        *(u32*)lbl_8047B180 = type;

    }
    type = *(u32*)&lbl_8047B18C;
    while (type != 0) {

        r29 = *(u32*)((u8*)type + 0x0);
        fn_801A6960();
        type = r29;

    }
    r29 = *(u32*)lbl_8047B190;
    tmp = 0x0;
    *(u32*)&lbl_8047B18C = tmp;
    if (r29 != 0) {
        if (r29 != 0) {
            if (r29 != 0) {
                type = 0x10000;
                param = *(u16*)((u8*)r29 + 0x4);
                tmp = tmp & 0xFFFF;
                tmp = tmp - param;
                tmp = __cntlzw(tmp);
                /* srwi. tmp, tmp, 5 */;
                if (r29 != 0) {
                } else {

                    tmp = __cntlzw(param);
                    *(u16*)((u8*)r29 + 0x4) = type;
                    tmp = (u32)tmp >> 5;
                }
                if ((s32)tmp != 0) {
                    if (r29 != 0) {
                        param = *(u32*)((u8*)r29 + 0x0);
                        type = r29;
                        r12 = *(u32*)((u8*)param + 0x30);
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
                        param = *(u32*)((u8*)r29 + 0x0);
                        type = r29;
                        r12 = *(u32*)((u8*)param + 0x34);
                        ctr_fn = (void(*)(void))r12;
                        ctr_fn();
        }
        }
        }
        }
        tmp = 0x0;
        *(u32*)lbl_8047B190 = tmp;
    }
    return;
}

/* ==================================================================
 * fn_80177A64 -- GSscene_XFBCapture
 *
 * Capture the current framebuffer to a file. At 3064 bytes, this is
 * the largest function in the scene system. Uses the "gs%04d.xfb"
 * format string to generate numbered filenames.
 *
 * This is likely a debug/development feature for capturing screenshots.
 * ================================================================== */
void GSscene_XFBCapture(u32 captureIndex) {
    extern u8 lbl_8036C248[];
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D738[];
    extern u8 lbl_8047D740[];
    extern u8 lbl_8047D748[];
    extern u8 lbl_8047D750[];
    extern u8 lbl_8047D758[];
    extern u8 lbl_8047D760[];
    extern void fn_800D1734();
    extern void fn_800D1A38();
    extern void fn_800D1EB8();
    extern void fn_800D1FDC();
    extern void fn_800D203C();
    extern void fn_800D20CC();
    extern void fn_800D2248();
    extern void fn_800D258C();
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_800E0168();
    extern void fn_800E01D0();
    extern void fn_800E090C();
    extern void fn_800E3D98();
    extern void fn_800EE150();
    extern void fn_800EE3BC();
    extern void fn_800EE828();
    extern void fn_800F92D4();
    extern void fn_800F9318();
    extern void fn_801174C4();
    extern void fn_801174EC();
    extern void fn_801786F4();
    extern void fn_80178AA8();
    extern void fn_80179020();
    extern u8 jumptable_8036C254[];
    extern u8 jumptable_8036C278[];
    u8 sp[0x150];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    captureIndex = 0x0;
    r4 = 0x0;
    fn_800F9318();
    r30 = *(u32*)&lbl_80478C40;
    captureIndex = captureIndex;
    tmp = *(u8*)((u8*)r30 + 0x1);
    if (tmp == 1) {
        tmp = *(u8*)((u8*)r30 + 0x4C);
        do {
            if (tmp == 0) break;
            fn_800D37CC();
            tmp = 0x43300000;
            f1 = *(f64*)lbl_8047D738;
            *(u32*)(sp + 0x120) = tmp;
            f31 = f0 - f1;
            fn_800D3088();
            tmp = 0x43300000;
            r4 = *(u32*)&lbl_80478C40;
            f2 = *(f64*)lbl_8047D760;
            *(u32*)(sp + 0x128) = tmp;
            f0 = *(f32*)((u8*)r4 + 0x6C);
            f1 = f1 - f2;
            f1 = f1 / f31;
            f0 = f0 + f1;
            *(f32*)((u8*)r4 + 0x6C) = f0;
            r5 = *(u32*)&lbl_80478C40;
            f1 = *(f32*)((u8*)r5 + 0x6C);
            f0 = *(f32*)((u8*)r5 + 0x68);
            /* cror eq, gt, eq */;
            if (f1 == f0) {
                captureIndex = r5 + 0x1c;
                r4 = r5 + 0x50;
                fn_800E01D0();
                f0 = *(f32*)lbl_8047D740;
                tmp = 0x0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(f32*)((u8*)captureIndex + 0x6C) = f0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(u8*)((u8*)captureIndex + 0x4C) = tmp;
                break;
            }
            f1 = f1 / f0;
            captureIndex = r5 + 0x1c;
            r4 = r5 + 0x5c;
            r5 = r5 + 0x50;
            fn_800E090C();
        } while (0);
        captureIndex = *(u32*)&lbl_80478C40;
        tmp = *(u8*)((u8*)captureIndex + 0x4D);
        do {
            if (tmp == 0) break;
            fn_800D37CC();
            tmp = 0x43300000;
            f1 = *(f64*)lbl_8047D738;
            *(u32*)(sp + 0x128) = tmp;
            f31 = f0 - f1;
            fn_800D3088();
            tmp = 0x43300000;
            r4 = *(u32*)&lbl_80478C40;
            f2 = *(f64*)lbl_8047D760;
            *(u32*)(sp + 0x120) = tmp;
            f0 = *(f32*)((u8*)r4 + 0x8C);
            f1 = f1 - f2;
            f1 = f1 / f31;
            f0 = f0 + f1;
            *(f32*)((u8*)r4 + 0x8C) = f0;
            r5 = *(u32*)&lbl_80478C40;
            f1 = *(f32*)((u8*)r5 + 0x8C);
            f0 = *(f32*)((u8*)r5 + 0x88);
            /* cror eq, gt, eq */;
            if (f1 == f0) {
                captureIndex = r5 + 0x28;
                r4 = r5 + 0x70;
                fn_800E01D0();
                f0 = *(f32*)lbl_8047D740;
                tmp = 0x0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(f32*)((u8*)captureIndex + 0x8C) = f0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(u8*)((u8*)captureIndex + 0x4D) = tmp;
                break;
            }
            f1 = f1 / f0;
            captureIndex = r5 + 0x28;
            r4 = r5 + 0x7c;
            r5 = r5 + 0x70;
            fn_800E090C();
        } while (0);
        captureIndex = *(u32*)&lbl_80478C40;
        tmp = *(u8*)((u8*)captureIndex + 0x4E);
        do {
            if (tmp == 0) break;
            fn_800D37CC();
            tmp = 0x43300000;
            f1 = *(f64*)lbl_8047D738;
            *(u32*)(sp + 0x128) = tmp;
            f31 = f0 - f1;
            fn_800D3088();
            tmp = 0x43300000;
            r4 = *(u32*)&lbl_80478C40;
            f2 = *(f64*)lbl_8047D760;
            *(u32*)(sp + 0x120) = tmp;
            f0 = *(f32*)((u8*)r4 + 0xAC);
            f1 = f1 - f2;
            f1 = f1 / f31;
            f0 = f0 + f1;
            *(f32*)((u8*)r4 + 0xAC) = f0;
            r5 = *(u32*)&lbl_80478C40;
            f1 = *(f32*)((u8*)r5 + 0xAC);
            f0 = *(f32*)((u8*)r5 + 0xA8);
            /* cror eq, gt, eq */;
            if (f1 == f0) {
                captureIndex = r5 + 0x4;
                r4 = r5 + 0x90;
                fn_800E01D0();
                f0 = *(f32*)lbl_8047D740;
                tmp = 0x0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(f32*)((u8*)captureIndex + 0xAC) = f0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(u8*)((u8*)captureIndex + 0x4E) = tmp;
                break;
            }
            f1 = f1 / f0;
            captureIndex = r5 + 0x4;
            r4 = r5 + 0x9c;
            r5 = r5 + 0x90;
            fn_800E090C();
        } while (0);
        captureIndex = *(u32*)&lbl_80478C40;
        tmp = *(u8*)((u8*)captureIndex + 0x4F);
        do {
            if (tmp == 0) break;
            fn_800D37CC();
            tmp = 0x43300000;
            f1 = *(f64*)lbl_8047D738;
            *(u32*)(sp + 0x128) = tmp;
            f31 = f0 - f1;
            fn_800D3088();
            tmp = 0x43300000;
            r4 = *(u32*)&lbl_80478C40;
            f2 = *(f64*)lbl_8047D760;
            *(u32*)(sp + 0x120) = tmp;
            f0 = *(f32*)((u8*)r4 + 0xCC);
            f1 = f1 - f2;
            f1 = f1 / f31;
            f0 = f0 + f1;
            *(f32*)((u8*)r4 + 0xCC) = f0;
            r5 = *(u32*)&lbl_80478C40;
            f1 = *(f32*)((u8*)r5 + 0xCC);
            f0 = *(f32*)((u8*)r5 + 0xC8);
            /* cror eq, gt, eq */;
            if (f1 == f0) {
                captureIndex = r5 + 0x10;
                r4 = r5 + 0xb0;
                fn_800E01D0();
                f0 = *(f32*)lbl_8047D740;
                tmp = 0x0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(f32*)((u8*)captureIndex + 0xCC) = f0;
                captureIndex = *(u32*)&lbl_80478C40;
                *(u8*)((u8*)captureIndex + 0x4F) = tmp;
                break;
            }
            f1 = f1 / f0;
            captureIndex = r5 + 0x10;
            r4 = r5 + 0xbc;
            r5 = r5 + 0xb0;
            fn_800E090C();
        } while (0);
        r29 = *(u32*)&lbl_80478C40;
        tmp = *(u8*)((u8*)r29 + 0x0);
        do {
            if ((s32)tmp == 6) break;
            if ((s32)tmp < 6) {
                if ((s32)tmp < 2) {
                    if ((s32)tmp >= 0) goto L_80177DB4;
                    break;
                }
                if ((s32)tmp >= 5) goto L_80177DB4;
                break;
            }
            if ((s32)tmp == 8) goto L_80178040;
            if ((s32)tmp >= 8) break;
            goto L_80177EF8;
        L_80177DB4:
            fn_801174C4();
            tmp = captureIndex & 0xFF;
            if ((s32)tmp == 8) break;
            fn_801174EC();
            tmp = captureIndex & 0xFF;
            if ((s32)tmp == 8) break;
            r5 = *(u32*)&lbl_80478C40;
            captureIndex = (u32)sp + 0xb4;
            r4 = r5 + 0x1c;
            r5 = r5 + 0x28;
            ((void(*)(void))fn_800E019C)();
            r4 = *(u32*)&lbl_80478C40;
            captureIndex = (u32)sp + 0xb4;
            r5 = captureIndex;
            r4 = r4 + 0x4;
            fn_800E0168();
            f1 = *(f32*)(sp + 0xB8);
            captureIndex = *(u32*)&lbl_80478C40;
            f0 = *(f32*)lbl_8047D740;
            *(f32*)((u8*)captureIndex + 0x40) = f1;
            f2 = *(f32*)(sp + 0xB4);
            f1 = *(f32*)(sp + 0xBC);
            f2 = f2 * f2;
            f1 = f1 * f1;
            f4 = f2 + f1;
            if (f4 > f0) {
                /* frsqrte f1, f4 */;
                f3 = *(f64*)lbl_8047D748;
                f2 = *(f64*)lbl_8047D750;
                f0 = f1 * f1;
                f1 = f3 * f1;
                f0 = -(f4 * f0 - f2);
                f1 = f1 * f0;
                f0 = f1 * f1;
                f1 = f3 * f1;
                f0 = -(f4 * f0 - f2);
                f1 = f1 * f0;
                f0 = f1 * f1;
                f1 = f3 * f1;
                f0 = -(f4 * f0 - f2);
                f0 = f1 * f0;
                f4 = f4 * f0;
                f4 = (f32)f4;
                goto L_80177EEC;
            }
            f0 = *(f64*)lbl_8047D758;
            if (f4 < f0) {
                captureIndex = (u32)lbl_80478AC0;
                f4 = *(f32*)lbl_80478AC0;
                goto L_80177EEC;
            }
            *(f32*)(sp + 0x50) = f4;
            tmp = 0x7F800000;
            captureIndex = r4 & 0x7F800000;
            if ((s32)captureIndex != (s32)tmp) {
                if ((s32)captureIndex >= (s32)tmp) goto L_80177ED8;
                if ((s32)captureIndex != 0) {
                    goto L_80177ED8;
                }
                tmp = r4 & 0x7FFFFF;
                if ((s32)captureIndex != 0) {
                    tmp = 0x1;
                    goto L_80177EDC;
                }
                tmp = 0x2;
                goto L_80177EDC;
                }
            tmp = r4 & 0x7FFFFF;
            if ((s32)captureIndex != 0) {
                tmp = 0x5;
                goto L_80177EDC;
            }
            tmp = 0x3;
            goto L_80177EDC;
        L_80177ED8:
            tmp = 0x4;
        L_80177EDC:
            if ((s32)tmp != 1) goto L_80177EEC;
            captureIndex = (u32)lbl_80478AC0;
            f4 = *(f32*)lbl_80478AC0;
        L_80177EEC:
            captureIndex = *(u32*)&lbl_80478C40;
            *(f32*)((u8*)captureIndex + 0x44) = f4;
            break;
        L_80177EF8:
            captureIndex = (u32)sp + 0xb4;
            r4 = r29 + 0x4;
            r5 = r29 + 0x1c;
            fn_800E0168();
            f1 = *(f32*)(sp + 0xB8);
            captureIndex = *(u32*)&lbl_80478C40;
            f0 = *(f32*)lbl_8047D740;
            *(f32*)((u8*)captureIndex + 0x40) = f1;
            f2 = *(f32*)(sp + 0xB4);
            f1 = *(f32*)(sp + 0xBC);
            f2 = f2 * f2;
            f1 = f1 * f1;
            f4 = f2 + f1;
            if (f4 > f0) {
                /* frsqrte f1, f4 */;
                f3 = *(f64*)lbl_8047D748;
                f2 = *(f64*)lbl_8047D750;
                f0 = f1 * f1;
                f1 = f3 * f1;
                f0 = -(f4 * f0 - f2);
                f1 = f1 * f0;
                f0 = f1 * f1;
                f1 = f3 * f1;
                f0 = -(f4 * f0 - f2);
                f1 = f1 * f0;
                f0 = f1 * f1;
                f1 = f3 * f1;
                f0 = -(f4 * f0 - f2);
                f0 = f1 * f0;
                f4 = f4 * f0;
                f4 = (f32)f4;
                goto L_80178000;
            }
            f0 = *(f64*)lbl_8047D758;
            if (f4 < f0) {
                captureIndex = (u32)lbl_80478AC0;
                f4 = *(f32*)lbl_80478AC0;
                goto L_80178000;
            }
            *(f32*)(sp + 0x4C) = f4;
            tmp = 0x7F800000;
            captureIndex = r4 & 0x7F800000;
            if ((s32)captureIndex != (s32)tmp) {
                if ((s32)captureIndex >= (s32)tmp) goto L_80177FEC;
                if ((s32)captureIndex != 0) {
                    goto L_80177FEC;
                }
                tmp = r4 & 0x7FFFFF;
                if ((s32)captureIndex != 0) {
                    tmp = 0x1;
                    goto L_80177FF0;
                }
                tmp = 0x2;
                goto L_80177FF0;
                }
            tmp = r4 & 0x7FFFFF;
            if ((s32)captureIndex != 0) {
                tmp = 0x5;
                goto L_80177FF0;
            }
            tmp = 0x3;
            goto L_80177FF0;
        L_80177FEC:
            tmp = 0x4;
        L_80177FF0:
            if ((s32)tmp != 1) goto L_80178000;
            captureIndex = (u32)lbl_80478AC0;
            f4 = *(f32*)lbl_80478AC0;
        L_80178000:
            captureIndex = *(u32*)&lbl_80478C40;
            *(f32*)((u8*)captureIndex + 0x44) = f4;
            captureIndex = *(u32*)&lbl_80478C40;
            tmp = *(u8*)((u8*)captureIndex + 0x4F);
            if (tmp != 0) break;
            tmp = *(u8*)((u8*)captureIndex + 0x4E);
            if (tmp == 0) break;
            f1 = *(f32*)(sp + 0xB4);
            f2 = *(f32*)(sp + 0xBC);
            ((void(*)(void))fn_800CE2D8)();
            f0 = (f32)f1;
            captureIndex = *(u32*)&lbl_80478C40;
            *(f32*)((u8*)captureIndex + 0x14) = f0;
            break;
        L_80178040:
            captureIndex = *(u32*)((u8*)r29 + 0xD0);
            r4 = *(u32*)((u8*)r29 + 0xD4);
            fn_800F9318();
            /* mr. r30, captureIndex */;
            if (tmp == 0) {
                captureIndex = *(u32*)((u8*)r29 + 0xD4);
                fn_800F92D4();
                r30 = captureIndex;
            }
            if (r30 == 0) return;
            captureIndex = r30;
            fn_800D1A38();
            tmp = captureIndex & 0xFF;
            if (r30 == 0) return;
            captureIndex = r30;
            fn_800D1734();
            tmp = captureIndex & 0xFF;
            if (r30 == 0) break;
            return;
        } while (0);
        captureIndex = *(u32*)&lbl_80478C40;
        tmp = *(u8*)((u8*)captureIndex + 0x4C);
        do {
            if (tmp != 0) break;
            tmp = *(u8*)((u8*)captureIndex + 0x4D);
            if (tmp != 0) break;
            tmp = *(u8*)((u8*)captureIndex + 0x4E);
            if (tmp != 0) break;
            tmp = *(u8*)((u8*)captureIndex + 0x4F);
            if (tmp != 0) break;
            tmp = 0x0;
            goto L_801780CC;
        } while (0);
        tmp = 0x1;
    L_801780CC:
        tmp = tmp & 0xFF;
        if (tmp != 0) goto L_80178200;
        tmp = 0x0;
        *(u8*)((u8*)captureIndex + 0x1) = tmp;
        goto L_80178200;
    }
    tmp = *(u8*)((u8*)r30 + 0x0);
    if (tmp > 8) goto L_80178200;
    captureIndex = (u32)jumptable_8036C278;
    tmp = tmp << 2;
    captureIndex = (u32)jumptable_8036C278;
    tmp = *(u32*)(captureIndex + tmp);
    ctr_fn = (void(*)(void))tmp;
    captureIndex = *(u32*)((u8*)r30 + 0x34);
    r4 = *(u32*)((u8*)r30 + 0x38);
    fn_800F9318();
    /* mr. r30, captureIndex */;
    if (tmp == 8) goto L_80178200;
    r4 = *(u32*)&lbl_80478C40;
    r4 = r4 + 0x1c;
    fn_800E3D98();
    captureIndex = *(u32*)&lbl_80478C40;
    r4 = *(u32*)((u8*)captureIndex + 0x3C);
    if ((s32)r4 < 0) goto L_80178200;
    captureIndex = r30;
    fn_800EE150();
    /* mr. r30, captureIndex */;
    if ((s32)r4 == 0) goto L_80178200;
    r4 = *(u32*)&lbl_80478C40;
    r5 = 0x0;
    r6 = 0x0;
    r4 = r4 + 0x28;
    fn_800EE3BC();
    r4 = *(u32*)&lbl_80478C40;
    captureIndex = r4 + 0x28;
    r5 = r4 + 0x1c;
    r4 = captureIndex;
    fn_800E0168();
    captureIndex = r30;
    fn_800EE828();
    goto L_80178200;
    captureIndex = *(u32*)((u8*)r30 + 0xD0);
    r4 = *(u32*)((u8*)r30 + 0xD4);
    fn_800F9318();
    if (captureIndex == 0) {
        captureIndex = *(u32*)((u8*)r30 + 0xD4);
        fn_800F92D4();
    }
    captureIndex = captureIndex;
    if (captureIndex != 0) goto L_80178200;
    captureIndex = *(u32*)&lbl_80478C40;
    tmp = 0x0;
    *(u32*)((u8*)captureIndex + 0xD0) = tmp;
    captureIndex = *(u32*)&lbl_80478C40;
    *(u32*)((u8*)captureIndex + 0xD4) = tmp;
    r4 = *(u32*)&lbl_80478C40;
    captureIndex = *(u8*)((u8*)r4 + 0x2);
    tmp = *(u8*)((u8*)r4 + 0x0);
    if (tmp != captureIndex) {
        *(u8*)((u8*)r4 + 0x0) = captureIndex;
    }
    captureIndex = *(u32*)&lbl_80478C40;
    r4 = 0x0;
    r6 = 0x64;
    tmp = -0x1;
    *(u32*)((u8*)captureIndex + 0x34) = r4;
    captureIndex = 0x0;
    r4 = 0x0;
    r5 = *(u32*)&lbl_80478C40;
    *(u32*)((u8*)r5 + 0x38) = r6;
    r5 = *(u32*)&lbl_80478C40;
    *(u32*)((u8*)r5 + 0x3C) = tmp;
    fn_800F9318();
    captureIndex = captureIndex;
L_80178200:
    r5 = *(u32*)&lbl_80478C40;
    tmp = *(u8*)((u8*)r5 + 0x0);
    do {
        if (tmp > 8) break;
        captureIndex = (u32)jumptable_8036C254;
        tmp = tmp << 2;
        captureIndex = (u32)jumptable_8036C254;
        tmp = *(u32*)(captureIndex + tmp);
        ctr_fn = (void(*)(void))tmp;
        f1 = *(f32*)lbl_8047D740;
        captureIndex = (u32)sp + 0xa8;
        f2 = *(f32*)((u8*)r5 + 0x40);
        f3 = *(f32*)((u8*)r5 + 0x44);
        ((void(*)(void))fn_800E01F4)();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = (u32)sp + 0xf0;
        f1 = *(f32*)((u8*)r4 + 0x14);
        ((void(*)(void))fn_800E0518)();
        captureIndex = (u32)sp + 0xa8;
        r4 = (u32)sp + 0xf0;
        r5 = captureIndex;
        ((void(*)(void))fn_800DFF98)();
        r4 = *(u32*)&lbl_80478C40;
        r5 = (u32)sp + 0xa8;
        captureIndex = r4 + 0x4;
        r4 = r4 + 0x1c;
        ((void(*)(void))fn_800E019C)();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        r4 = r4 + 0x4;
        ((void(*)(void))fn_800D207C)();
        r5 = *(u32*)&lbl_80478C40;
        captureIndex = (u32)sp + 0x9c;
        r4 = r5 + 0x1c;
        r5 = r5 + 0x28;
        ((void(*)(void))fn_800E019C)();
        r4 = (u32)lbl_8036C248;
        captureIndex = captureIndex;
        r4 = (u32)lbl_8036C248;
        r5 = (u32)sp + 0x9c;
        ((void(*)(void))fn_800D1F04)();
        captureIndex = *(u32*)&lbl_80478C40;
        f1 = *(f32*)((u8*)captureIndex + 0x40);
        f2 = *(f32*)((u8*)captureIndex + 0x44);
        ((void(*)(void))fn_800CE2D8)();
        f0 = (f32)f1;
        r8 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        r4 = (u32)sp + 0x3c;
        r5 = (u32)sp + 0x40;
        r6 = (u32)sp + 0x44;
        f0 = -f0;
        r7 = (u32)sp + 0x48;
        *(f32*)((u8*)r8 + 0x10) = f0;
        fn_800D1FDC();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        f2 = *(f32*)(sp + 0x40);
        f1 = *(f32*)((u8*)r4 + 0x48);
        f3 = *(f32*)(sp + 0x44);
        f4 = *(f32*)(sp + 0x48);
        fn_800D20CC();
        break;
        captureIndex = (u32)sp + 0x84;
        r4 = r5 + 0x1c;
        r5 = r5 + 0x28;
        ((void(*)(void))fn_800E019C)();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        r4 = r4 + 0x4;
        ((void(*)(void))fn_800D207C)();
        r4 = (u32)lbl_8036C248;
        captureIndex = captureIndex;
        r4 = (u32)lbl_8036C248;
        r5 = (u32)sp + 0x84;
        ((void(*)(void))fn_800D1F04)();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = (u32)sp + 0x90;
        r5 = (u32)sp + 0x84;
        r4 = r4 + 0x4;
        fn_800E0168();
        f1 = *(f32*)(sp + 0x94);
        captureIndex = *(u32*)&lbl_80478C40;
        f0 = *(f32*)lbl_8047D740;
        *(f32*)((u8*)captureIndex + 0x40) = f1;
        f2 = *(f32*)(sp + 0x90);
        f1 = *(f32*)(sp + 0x98);
        f2 = f2 * f2;
        f1 = f1 * f1;
        f4 = f2 + f1;
        if (f4 > f0) {
            /* frsqrte f1, f4 */;
            f3 = *(f64*)lbl_8047D748;
            f2 = *(f64*)lbl_8047D750;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f1 = f1 * f0;
            f0 = f1 * f1;
            f1 = f3 * f1;
            f0 = -(f4 * f0 - f2);
            f0 = f1 * f0;
            f4 = f4 * f0;
            f4 = (f32)f4;
            goto L_80178440;
        }
        f0 = *(f64*)lbl_8047D758;
        if (f4 < f0) {
            captureIndex = (u32)lbl_80478AC0;
            f4 = *(f32*)lbl_80478AC0;
            goto L_80178440;
        }
        *(f32*)(sp + 0x8) = f4;
        tmp = 0x7F800000;
        captureIndex = r4 & 0x7F800000;
        if ((s32)captureIndex != (s32)tmp) {
            if ((s32)captureIndex >= (s32)tmp) goto L_8017842C;
            if ((s32)captureIndex != 0) {
                goto L_8017842C;
            }
            tmp = r4 & 0x7FFFFF;
            if ((s32)captureIndex != 0) {
                tmp = 0x1;
                goto L_80178430;
            }
            tmp = 0x2;
            goto L_80178430;
            }
        tmp = r4 & 0x7FFFFF;
        if ((s32)captureIndex != 0) {
            tmp = 0x5;
            goto L_80178430;
        }
        tmp = 0x3;
        goto L_80178430;
    L_8017842C:
        tmp = 0x4;
    L_80178430:
        if ((s32)tmp != 1) goto L_80178440;
        captureIndex = (u32)lbl_80478AC0;
        f4 = *(f32*)lbl_80478AC0;
    L_80178440:
        captureIndex = *(u32*)&lbl_80478C40;
        *(f32*)((u8*)captureIndex + 0x44) = f4;
        f1 = *(f32*)(sp + 0x90);
        f2 = *(f32*)(sp + 0x98);
        ((void(*)(void))fn_800CE2D8)();
        f0 = (f32)f1;
        captureIndex = *(u32*)&lbl_80478C40;
        *(f32*)((u8*)captureIndex + 0x14) = f0;
        captureIndex = *(u32*)&lbl_80478C40;
        f1 = *(f32*)((u8*)captureIndex + 0x40);
        f2 = *(f32*)((u8*)captureIndex + 0x44);
        ((void(*)(void))fn_800CE2D8)();
        f0 = (f32)f1;
        r8 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        r4 = (u32)sp + 0x2c;
        r5 = (u32)sp + 0x30;
        r6 = (u32)sp + 0x34;
        f0 = -f0;
        r7 = (u32)sp + 0x38;
        *(f32*)((u8*)r8 + 0x10) = f0;
        fn_800D1FDC();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        f2 = *(f32*)(sp + 0x30);
        f1 = *(f32*)((u8*)r4 + 0x48);
        f3 = *(f32*)(sp + 0x34);
        f4 = *(f32*)(sp + 0x38);
        fn_800D20CC();
        break;
        captureIndex = captureIndex;
        fn_80179020();
        break;
        captureIndex = captureIndex;
        fn_80178AA8();
        break;
        captureIndex = captureIndex;
        r4 = r5 + 0x4;
        ((void(*)(void))fn_800D207C)();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        r4 = r4 + 0x10;
        fn_800D203C();
        captureIndex = captureIndex;
        r4 = (u32)sp + 0x78;
        r5 = (u32)sp + 0x6c;
        fn_800D1EB8();
        r5 = *(u32*)&lbl_80478C40;
        r4 = (u32)sp + 0x6c;
        captureIndex = r5 + 0x1c;
        r5 = r5 + 0x28;
        fn_800E0168();
        captureIndex = captureIndex;
        r4 = (u32)sp + 0x1c;
        r5 = (u32)sp + 0x20;
        r6 = (u32)sp + 0x24;
        r7 = (u32)sp + 0x28;
        fn_800D1FDC();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        f2 = *(f32*)(sp + 0x20);
        f1 = *(f32*)((u8*)r4 + 0x48);
        f3 = *(f32*)(sp + 0x24);
        f4 = *(f32*)(sp + 0x28);
        fn_800D20CC();
        break;
        captureIndex = captureIndex;
        fn_801786F4();
        fn_800D2248();
        return;
        f1 = *(f32*)lbl_8047D740;
        captureIndex = (u32)sp + 0x60;
        f2 = *(f32*)((u8*)r5 + 0x40);
        f3 = *(f32*)((u8*)r5 + 0x44);
        ((void(*)(void))fn_800E01F4)();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = (u32)sp + 0xc0;
        f1 = *(f32*)((u8*)r4 + 0x14);
        ((void(*)(void))fn_800E0518)();
        captureIndex = (u32)sp + 0x60;
        r4 = (u32)sp + 0xc0;
        r5 = captureIndex;
        ((void(*)(void))fn_800DFF98)();
        r4 = *(u32*)&lbl_80478C40;
        r5 = (u32)sp + 0x60;
        captureIndex = r4 + 0x4;
        r4 = r4 + 0x1c;
        ((void(*)(void))fn_800E019C)();
        r5 = *(u32*)&lbl_80478C40;
        captureIndex = (u32)sp + 0x54;
        r4 = r5 + 0x1c;
        r5 = r5 + 0x28;
        ((void(*)(void))fn_800E019C)();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        r4 = r4 + 0x4;
        ((void(*)(void))fn_800D207C)();
        r4 = (u32)lbl_8036C248;
        captureIndex = captureIndex;
        r4 = (u32)lbl_8036C248;
        r5 = (u32)sp + 0x54;
        ((void(*)(void))fn_800D1F04)();
        captureIndex = *(u32*)&lbl_80478C40;
        f1 = *(f32*)((u8*)captureIndex + 0x40);
        f2 = *(f32*)((u8*)captureIndex + 0x44);
        ((void(*)(void))fn_800CE2D8)();
        f0 = (f32)f1;
        r8 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        r4 = (u32)sp + 0xc;
        r5 = (u32)sp + 0x10;
        r6 = (u32)sp + 0x14;
        f0 = -f0;
        r7 = (u32)sp + 0x18;
        *(f32*)((u8*)r8 + 0x10) = f0;
        fn_800D1FDC();
        r4 = *(u32*)&lbl_80478C40;
        captureIndex = captureIndex;
        f2 = *(f32*)(sp + 0x10);
        f1 = *(f32*)((u8*)r4 + 0x48);
        f3 = *(f32*)(sp + 0x14);
        f4 = *(f32*)(sp + 0x18);
        fn_800D20CC();
    } while (0);
    captureIndex = captureIndex;
    fn_800D258C();
    fn_800D2248();

    return;
}

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
void GSscene_CameraUpdate(u32 sceneObj) {
    extern u8 lbl_8036C248[];
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047D728[];
    extern u8 lbl_8047D72C[];
    extern u8 lbl_8047D738[];
    extern u8 lbl_8047D740[];
    extern u8 lbl_8047D748[];
    extern u8 lbl_8047D750[];
    extern u8 lbl_8047D758[];
    extern u8 lbl_8047D76C[];
    extern u8 lbl_8047D770[];
    extern u8 lbl_8047D774[];
    extern u8 lbl_8047D778[];
    extern u8 lbl_8047D77C[];
    extern void fn_800CDBE0();
    extern void fn_800CE148();
    extern void fn_800D1FDC();
    extern void fn_800D20CC();
    extern void fn_800D3088();
    extern void fn_800F7920();
    extern void fn_800F7994();
    extern void fn_800F7BC4();
    extern void fn_801337B0();
    u8 sp[0xB0];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    sceneObj = sceneObj;
    fn_801337B0();
tmp = (u32)sceneObj & 0xFF;
    if ((s32)tmp != 0) return;
    fn_800D3088();
    sceneObj = sceneObj;
sceneObj = (u32)0x1;
    r4 = 0x1;
    fn_800F7994();
    tmp = (s8)sceneObj;
    r31 = tmp * sceneObj;
    fn_800D3088();
    sceneObj = sceneObj;
sceneObj = (u32)0x1;
    r4 = 0x1;
    fn_800F7920();
    tmp = (s8)sceneObj;
sceneObj = *(u32*)&lbl_80478C40;
sceneObj = tmp * sceneObj;
    f1 = *(f32*)((u8*)sceneObj + 0x40);
    f2 = *(f32*)((u8*)sceneObj + 0x44);
    ((void(*)(void))fn_800CE2D8)();
sceneObj = *(u32*)&lbl_80478C40;
    f1 = (f32)f1;
    f0 = *(f32*)lbl_8047D740;
    f2 = *(f32*)((u8*)sceneObj + 0x40);
    f3 = *(f32*)((u8*)sceneObj + 0x44);
    f30 = f1;
    f2 = f2 * f2;
    f1 = f3 * f3;
    f31 = f2 + f1;
    if (f31 > f0) {
        /* frsqrte f1, f31 */;
        f3 = *(f64*)lbl_8047D748;
        f2 = *(f64*)lbl_8047D750;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f1 = f1 * f0;
        f0 = f1 * f1;
        f1 = f3 * f1;
        f0 = -(f31 * f0 - f2);
        f0 = f1 * f0;
        f31 = f31 * f0;
        f31 = (f32)f31;
        goto L_80178C28;
    }
    f0 = *(f64*)lbl_8047D758;
    if (f31 < f0) {
    sceneObj = (u32)lbl_80478AC0;
        f31 = *(f32*)lbl_80478AC0;
        goto L_80178C28;
    }
    *(f32*)(sp + 0x8) = f31;
    tmp = 0x7F800000;
sceneObj = (r4 & 0x7F800000);
    if ((s32)sceneObj != (s32)tmp) {
        if ((s32)sceneObj >= (s32)tmp) goto L_80178C14;
        if ((s32)sceneObj != 0) {
            goto L_80178C14;
        }
        tmp = r4 & 0x7FFFFF;
        if ((s32)sceneObj != 0) {
            tmp = 0x1;
            goto L_80178C18;
        }
        tmp = 0x2;
        goto L_80178C18;
        }
    tmp = r4 & 0x7FFFFF;
    if ((s32)sceneObj != 0) {
        tmp = 0x5;
        goto L_80178C18;
    }
    tmp = 0x3;
    goto L_80178C18;
L_80178C14:
    tmp = 0x4;
L_80178C18:
    if ((s32)tmp != 1) goto L_80178C28;
sceneObj = (u32)lbl_80478AC0;
    f31 = *(f32*)lbl_80478AC0;
L_80178C28:
sceneObj = (u32)0x1;
    fn_800F7BC4();
tmp = (u32)sceneObj & 0x00000020;
    if ((s32)tmp != 1) {
        tmp = 0x43300000;
    sceneObj = *(u32*)&lbl_80478C40;
        *(u32*)(sp + 0x68) = tmp;
        f1 = *(f64*)lbl_8047D738;
        f2 = *(f32*)lbl_8047D76C;
        f3 = f0 - f1;
        f1 = *(f32*)((u8*)sceneObj + 0x48);
        f0 = *(f32*)lbl_8047D728;
        f1 = f3 * f2 + f1;
        f29 = f1;
        if (f1 < f0) {
            f29 = f0;
        }
        f0 = *(f32*)lbl_8047D72C;
        if (f29 > f0) {
            f29 = f0;
        }
        ((void(*)(void))fn_800FF56C)();
        r4 = *(u32*)&lbl_80478FB8;
        r5 = 0x0;
        r6 = *(u32*)&lbl_8047B1A8;
        tmp = *(u32*)((u8*)r4 + 0x0);
        r4 = r6;
        ctr_fn = (void(*)(void))tmp;
        if (tmp > 0) {
            do {
                tmp = *(u32*)((u8*)r4 + 0x4);
            if ((u32)sceneObj == tmp) {
                    tmp = r5 * 0x28;
                    sceneObj = r6 + tmp;
                    goto L_80178CD4;
            }
                r4 = r4 + 0x28;
                r5 = r5 + 0x1;
            } while (--ctr != 0);
        }
    sceneObj = (u32)0x0;
    L_80178CD4:
    if ((u32)sceneObj != 0) {
        *(f32*)((u8*)sceneObj + 0x24) = f29;
    }
    sceneObj = *(u32*)&lbl_80478C40;
        *(f32*)((u8*)sceneObj + 0x48) = f29;
        goto L_80178D4C;
    }
sceneObj = (u32)0x1;
    fn_800F7BC4();
tmp = (u32)sceneObj & 0x00000040;
if ((u32)sceneObj != 0) {
        tmp = 0x43300000;
        f2 = *(f64*)lbl_8047D738;
        *(u32*)(sp + 0x68) = tmp;
        f0 = *(f32*)lbl_8047D76C;
        f1 = f1 - f2;
        f31 = f1 * f0 + f31;
        goto L_80178D4C;
}
    tmp = 0x43300000;
    f2 = *(f64*)lbl_8047D738;
    *(u32*)(sp + 0x68) = tmp;
    f0 = *(f32*)lbl_8047D770;
    f1 = f1 - f2;
    f0 = f1 / f0;
    f30 = f30 - f0;
L_80178D4C:
    f1 = f30;
    fn_800CE148();
    f0 = (f32)f1;
sceneObj = *(u32*)&lbl_80478C40;
    f1 = f30;
    f0 = f31 * f0;
    *(f32*)((u8*)sceneObj + 0x40) = f0;
    fn_800CDBE0();
    f1 = (f32)f1;
sceneObj = *(u32*)&lbl_80478C40;
    f0 = *(f32*)lbl_8047D774;
    f1 = f31 * f1;
    *(f32*)((u8*)sceneObj + 0x44) = f1;
sceneObj = *(u32*)&lbl_80478C40;
    f1 = *(f32*)((u8*)sceneObj + 0x44);
    if (f1 < f0) {
        *(f32*)((u8*)sceneObj + 0x44) = f0;
        goto L_80178DA8;
    }
    f0 = *(f32*)lbl_8047D778;
    if (f1 <= f0) goto L_80178DA8;
    *(f32*)((u8*)sceneObj + 0x44) = f0;
L_80178DA8:
sceneObj = *(u32*)&lbl_80478C40;
    f29 = *(f32*)((u8*)sceneObj + 0x40);
    ((void(*)(void))fn_800FF56C)();
    r4 = *(u32*)&lbl_80478FB8;
    r5 = 0x0;
    r6 = *(u32*)&lbl_8047B1A8;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = r6;
    ctr_fn = (void(*)(void))tmp;
    if (tmp > 0) {
        do {
            tmp = *(u32*)((u8*)r4 + 0x4);
        if ((u32)sceneObj == tmp) {
                tmp = r5 * 0x28;
                sceneObj = r6 + tmp;
                goto L_80178DFC;
        }
            r4 = r4 + 0x28;
            r5 = r5 + 0x1;
        } while (--ctr != 0);
    }
sceneObj = (u32)0x0;
L_80178DFC:
if ((u32)sceneObj != 0) {
    *(f32*)((u8*)sceneObj + 0x18) = f29;
}
sceneObj = *(u32*)&lbl_80478C40;
    *(f32*)((u8*)sceneObj + 0x40) = f29;
sceneObj = *(u32*)&lbl_80478C40;
    f29 = *(f32*)((u8*)sceneObj + 0x44);
    ((void(*)(void))fn_800FF56C)();
    r4 = *(u32*)&lbl_80478FB8;
    r5 = 0x0;
    r6 = *(u32*)&lbl_8047B1A8;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = r6;
    ctr_fn = (void(*)(void))tmp;
    if (tmp > 0) {
        do {
            tmp = *(u32*)((u8*)r4 + 0x4);
        if ((u32)sceneObj == tmp) {
                tmp = r5 * 0x28;
                sceneObj = r6 + tmp;
                goto L_80178E64;
        }
            r4 = r4 + 0x28;
            r5 = r5 + 0x1;
        } while (--ctr != 0);
    }
sceneObj = (u32)0x0;
L_80178E64:
if ((u32)sceneObj != 0) {
    *(f32*)((u8*)sceneObj + 0x1C) = f29;
}
    r4 = *(u32*)&lbl_80478C40;
sceneObj = (u32)sp + 0x1c;
    f1 = *(f32*)lbl_8047D740;
    *(f32*)((u8*)r4 + 0x44) = f29;
    r4 = *(u32*)&lbl_80478C40;
    f2 = *(f32*)((u8*)r4 + 0x40);
    f3 = *(f32*)((u8*)r4 + 0x44);
    ((void(*)(void))fn_800E01F4)();
    tmp = 0x43300000;
sceneObj = *(u32*)&lbl_80478C40;
    *(u32*)(sp + 0x68) = tmp;
    f2 = *(f64*)lbl_8047D738;
    f0 = *(f32*)lbl_8047D77C;
    f1 = f1 - f2;
    f2 = *(f32*)((u8*)sceneObj + 0x14);
    f0 = f1 / f0;
    f0 = f2 + f0;
    *(f32*)((u8*)sceneObj + 0x14) = f0;
sceneObj = *(u32*)&lbl_80478C40;
    f29 = *(f32*)((u8*)sceneObj + 0x14);
    ((void(*)(void))fn_800FF56C)();
    r4 = *(u32*)&lbl_80478FB8;
    r5 = 0x0;
    r6 = *(u32*)&lbl_8047B1A8;
    tmp = *(u32*)((u8*)r4 + 0x0);
    r4 = r6;
    ctr_fn = (void(*)(void))tmp;
    if (tmp > 0) {
        do {
            tmp = *(u32*)((u8*)r4 + 0x4);
        if ((u32)sceneObj == tmp) {
                tmp = r5 * 0x28;
                sceneObj = r6 + tmp;
                goto L_80178F18;
        }
            r4 = r4 + 0x28;
            r5 = r5 + 0x1;
        } while (--ctr != 0);
    }
sceneObj = (u32)0x0;
L_80178F18:
if ((u32)sceneObj != 0) {
    *(f32*)((u8*)sceneObj + 0x20) = f29;
}
    r4 = *(u32*)&lbl_80478C40;
sceneObj = (u32)sp + 0x34;
    *(f32*)((u8*)r4 + 0x14) = f29;
    r4 = *(u32*)&lbl_80478C40;
    f1 = *(f32*)((u8*)r4 + 0x14);
    ((void(*)(void))fn_800E0518)();
sceneObj = (u32)sp + 0x1c;
    r4 = (u32)sp + 0x34;
r5 = (u32)sceneObj;
    ((void(*)(void))fn_800DFF98)();
    r5 = *(u32*)&lbl_80478C40;
sceneObj = (u32)sp + 0x28;
    r4 = r5 + 0x1c;
    r5 = r5 + 0x28;
    ((void(*)(void))fn_800E019C)();
sceneObj = *(u32*)&lbl_80478C40;
    r4 = (u32)sp + 0x28;
    r5 = (u32)sp + 0x1c;
sceneObj = sceneObj + 0x4;
    ((void(*)(void))fn_800E019C)();
    r4 = *(u32*)&lbl_80478C40;
    sceneObj = sceneObj;
    r4 = r4 + 0x4;
    ((void(*)(void))fn_800D207C)();
    r4 = (u32)lbl_8036C248;
    sceneObj = sceneObj;
    r4 = (u32)lbl_8036C248;
    r5 = (u32)sp + 0x28;
    ((void(*)(void))fn_800D1F04)();
sceneObj = *(u32*)&lbl_80478C40;
    f1 = *(f32*)((u8*)sceneObj + 0x40);
    f2 = *(f32*)((u8*)sceneObj + 0x44);
    ((void(*)(void))fn_800CE2D8)();
    f0 = (f32)f1;
    r8 = *(u32*)&lbl_80478C40;
    sceneObj = sceneObj;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x10;
    r6 = (u32)sp + 0x14;
    f0 = -f0;
    r7 = (u32)sp + 0x18;
    *(f32*)((u8*)r8 + 0x10) = f0;
    fn_800D1FDC();
    r4 = *(u32*)&lbl_80478C40;
    sceneObj = sceneObj;
    f2 = *(f32*)(sp + 0x10);
    f1 = *(f32*)((u8*)r4 + 0x48);
    f3 = *(f32*)(sp + 0x14);
    f4 = *(f32*)(sp + 0x18);
    fn_800D20CC();

    return;
}

/* ==================================================================
 * fn_80179020 -- GSscene_CameraInterpolate
 *
 * Interpolate the camera between two states. 996 bytes.
 * ================================================================== */
void GSscene_CameraInterpolate(u32 camera) {
    extern u8 lbl_80315540[];
    extern u8 lbl_8031554C[];
    extern u8 lbl_80315558[];
    extern u8 lbl_8047D728[];
    extern u8 lbl_8047D72C[];
    extern u8 lbl_8047D738[];
    extern u8 lbl_8047D740[];
    extern u8 lbl_8047D780[];
    extern u8 lbl_8047D784[];
    extern u8 lbl_8047D788[];
    extern void fn_800D1FDC();
    extern void fn_800D203C();
    extern void fn_800D20CC();
    extern void fn_800D3088();
    extern void fn_800DFEEC();
    extern void fn_800E0718();
    extern void fn_800E0738();
    extern void fn_800F7920();
    extern void fn_800F7994();
    extern void fn_800F7A08();
    extern void fn_800F7A7C();
    extern void fn_800F7AF0();
    extern void fn_800F7BC4();
    extern void fn_80102510();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_80102868();
    extern void fn_801337B0();
    u8 sp[0xF0];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    camera = camera;
    fn_800D3088();
    f30 = *(f32*)lbl_8047D780;
    camera = camera;
    fn_801337B0();
tmp = (u32)camera & 0xFF;
    if ((s32)tmp != 0) return;
camera = (u32)0x1;
    fn_800F7AF0();
    camera = camera;
camera = (u32)0x1;
    fn_800F7BC4();
    tmp = camera & camera;
    tmp = tmp & 0x00000400;
    do {
        if ((s32)tmp == 0) break;
    camera = (u32)0x6;
        fn_80102620();
    tmp = (u32)camera & 0xFF;
        if ((s32)tmp != 0) {
        camera = (u32)0x6;
            fn_80102510();
            break;
        }
    camera = (u32)0x6;
        r4 = 0x0;
        r5 = 0x0;
        r6 = 0x0;
        r7 = 0x1;
        r8 = 0x0;
        fn_801026A4();
    camera = (u32)0x6;
        r4 = 0x14;
        r5 = 0x104;
        fn_80102868();
    } while (0);
    r5 = *(u32*)&lbl_80478C40;
camera = (u32)lbl_80315540;
    r4 = (u32)lbl_80315540;
    f1 = *(f32*)((u8*)r5 + 0x10);
camera = (u32)sp + 0x54;
    fn_800E0718();
    r5 = *(u32*)&lbl_80478C40;
camera = (u32)lbl_8031554C;
    r4 = (u32)lbl_8031554C;
    f1 = *(f32*)((u8*)r5 + 0x14);
camera = (u32)sp + 0x44;
    fn_800E0718();
    r5 = *(u32*)&lbl_80478C40;
camera = (u32)lbl_80315558;
    r4 = (u32)lbl_80315558;
    f1 = *(f32*)((u8*)r5 + 0x18);
camera = (u32)sp + 0x34;
    fn_800E0718();
camera = (u32)sp + 0x24;
    r4 = (u32)sp + 0x54;
    r5 = (u32)sp + 0x34;
    fn_800E0738();
camera = (u32)sp + 0x24;
    r4 = (u32)sp + 0x44;
r5 = (u32)camera;
    fn_800E0738();
camera = (u32)0x1;
    r4 = 0x0;
    fn_800F7A7C();
    camera = (s8)camera;
    tmp = 0x43300000;
    r5 = camera * camera;
    *(u32*)(sp + 0x70) = tmp;
    f1 = *(f64*)lbl_8047D738;
camera = (u32)0x1;
    r4 = 0x0;
    *(u32*)(sp + 0x74) = tmp;
    f29 = f0 - f1;
    fn_800F7A08();
    camera = (s8)camera;
    tmp = 0x43300000;
    r5 = camera * camera;
    *(u32*)(sp + 0x78) = tmp;
    f1 = *(f64*)lbl_8047D738;
camera = (u32)0x1;
    r4 = 0x0;
    *(u32*)(sp + 0x7C) = tmp;
    f28 = f0 - f1;
    fn_800F7994();
    camera = (s8)camera;
    tmp = 0x43300000;
    r5 = camera * camera;
    *(u32*)(sp + 0x80) = tmp;
    f1 = *(f64*)lbl_8047D738;
camera = (u32)0x1;
    r4 = 0x0;
    *(u32*)(sp + 0x84) = tmp;
    f31 = f0 - f1;
    fn_800F7920();
    camera = (s8)camera;
    tmp = 0x43300000;
    r4 = camera * camera;
    *(u32*)(sp + 0x88) = tmp;
    f1 = *(f64*)lbl_8047D738;
camera = (u32)0x1;
    *(u32*)(sp + 0x8C) = tmp;
    f27 = f0 - f1;
    fn_800F7BC4();
tmp = (u32)camera & 0x00000040;
    if ((s32)tmp != 0) {
        f0 = *(f32*)lbl_8047D784;
        f30 = f30 * f0;
    }
camera = (u32)0x1;
    fn_800F7BC4();
tmp = (u32)camera & 0x00000800;
    if ((s32)tmp != 0) {
        f0 = -f28;
        f3 = *(f32*)lbl_8047D740;
        f1 = f29 / f30;
    camera = (u32)sp + 0x18;
        f2 = f0 / f30;
        ((void(*)(void))fn_800E01F4)();
    } else {

        f1 = f29 / f30;
        f2 = *(f32*)lbl_8047D740;
    camera = (u32)sp + 0x18;
        f3 = f28 / f30;
        ((void(*)(void))fn_800E01F4)();
    }
camera = (u32)sp + 0x64;
    r4 = (u32)sp + 0x24;
    r5 = (u32)sp + 0x18;
    fn_800DFEEC();
camera = *(u32*)&lbl_80478C40;
    r5 = (u32)sp + 0x64;
camera = camera + 0x4;
r4 = (u32)camera;
    ((void(*)(void))fn_800E019C)();
camera = (u32)0x1;
    fn_800F7BC4();
tmp = (u32)camera & 0x00000020;
    if ((s32)tmp != 0) {
        f2 = f27 / f30;
    camera = *(u32*)&lbl_80478C40;
        f0 = *(f32*)lbl_8047D728;
        f1 = *(f32*)((u8*)camera + 0x48);
        f1 = f2 + f1;
        f29 = f1;
        if (f1 < f0) {
            f29 = f0;
        }
        f0 = *(f32*)lbl_8047D72C;
        if (f29 > f0) {
            f29 = f0;
        }
        ((void(*)(void))fn_800FF56C)();
        r4 = *(u32*)&lbl_80478FB8;
        r5 = 0x0;
        r6 = *(u32*)&lbl_8047B1A8;
        tmp = *(u32*)((u8*)r4 + 0x0);
        r4 = r6;
        ctr_fn = (void(*)(void))tmp;
        if (tmp > 0) {
            do {
                tmp = *(u32*)((u8*)r4 + 0x4);
            if ((u32)camera == tmp) {
                    tmp = r5 * 0x28;
                    camera = r6 + tmp;
                    goto L_8017931C;
            }
                r4 = r4 + 0x28;
                r5 = r5 + 0x1;
            } while (--ctr != 0);
        }
    camera = (u32)0x0;
    L_8017931C:
    if ((u32)camera != 0) {
        *(f32*)((u8*)camera + 0x24) = f29;
    }
    camera = *(u32*)&lbl_80478C40;
        *(f32*)((u8*)camera + 0x48) = f29;
        goto L_80179350;
    }
    f0 = *(f32*)lbl_8047D788;
camera = *(u32*)&lbl_80478C40;
    f0 = f0 * f30;
    f1 = *(f32*)((u8*)camera + 0x10);
    f0 = f27 / f0;
    f0 = f1 - f0;
    *(f32*)((u8*)camera + 0x10) = f0;
L_80179350:
    f0 = *(f32*)lbl_8047D788;
    camera = camera;
    r4 = *(u32*)&lbl_80478C40;
    f0 = f0 * f30;
    f1 = *(f32*)((u8*)r4 + 0x14);
    f0 = f31 / f0;
    f0 = f1 - f0;
    *(f32*)((u8*)r4 + 0x14) = f0;
    r4 = *(u32*)&lbl_80478C40;
    r4 = r4 + 0x4;
    ((void(*)(void))fn_800D207C)();
    r4 = *(u32*)&lbl_80478C40;
    camera = camera;
    r4 = r4 + 0x10;
    fn_800D203C();
    camera = camera;
    r4 = (u32)sp + 0x8;
    r5 = (u32)sp + 0xc;
    r6 = (u32)sp + 0x10;
    r7 = (u32)sp + 0x14;
    fn_800D1FDC();
    r4 = *(u32*)&lbl_80478C40;
    camera = camera;
    f2 = *(f32*)(sp + 0xC);
    f1 = *(f32*)((u8*)r4 + 0x48);
    f3 = *(f32*)(sp + 0x10);
    f4 = *(f32*)(sp + 0x14);
    fn_800D20CC();

    return;
}

/* ==================================================================
 * fn_80179FA4 -- GSscene_Init
 *
 * Initialize the scene system. Second-largest function at 1624 bytes.
 * Sets up the object free list, active list, camera state, render
 * tables, and initial scene configuration.
 * ================================================================== */
void GSscene_Init(void) {
    extern u8 lbl_80453FEC[];
    extern u8 lbl_8047B1B8[];
    extern u8 lbl_8047B1BC[];
    extern u8 lbl_8047B1C0[];
    extern void fn_80167E5C();
    extern void fn_80167E98();
    extern void fn_80167EF8();
    extern void fn_80167F28();
    extern void fn_8017F800();
    extern void fn_8017F928();
    extern void fn_8017FA5C();
    extern void fn_8017A5FC();
    u8 sp[0xF0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f4 = 0.0f;
    f32 f8 = 0.0f;

    r31 = r3;
    r26 = r10;
    r3 = *(u32*)((u8*)r10 + 0x14);
    tmp = *(u32*)((u8*)r10 + 0x14);
    r17 = r3 & 0x1FFFF;
    r3 = *(u32*)((u8*)r31 + 0xFC);
    r4 = r17 + 0x1f;
    r18 = (u32)tmp >> 17;
    /* clrrwi r17, r4, 5 */;
    /* clrrwi tmp, tmp, 17 */;
    r18 = r17 + tmp;
    fn_8017FA5C();
    r4 = *(u32*)((u8*)r31 + 0x20);
    do {
        if (r4 <= tmp) break;
        tmp = 0x0;
        r25 = *(u32*)lbl_8047B1B8;
        *(u32*)(sp + 0x74) = tmp;
        while (1) {
            tmp = *(u32*)lbl_8047B1BC;
            if ((s32)r3 >= (s32)tmp) break;
            r3 = *(u32*)((u8*)r25 + 0x0);
            tmp = *(u32*)((u8*)r31 + 0x8);
            if ((s32)r3 == (s32)tmp) {
                tmp = *(u32*)((u8*)r25 + 0x0);
                *(u32*)(sp + 0x78) = tmp;
                goto L_8017A068;
            }
            r25 = r25 + 0x8;
            tmp = r3 + 0x1;
            *(u32*)(sp + 0x74) = tmp;

        }
        tmp = -0x1;
        *(u32*)(sp + 0x78) = tmp;
    L_8017A068:
        if ((s32)tmp >= 0) break;
        tmp = 0x0;
        *(u32*)(sp + 0x94) = tmp;
        do {
            tmp = *(u32*)lbl_8047B1B8;
            *(u32*)(sp + 0x6C) = tmp;
            tmp = *(u32*)((u8*)r3 + 0x0);
            *(u32*)(sp + 0x68) = tmp;
            *(u32*)(sp + 0x70) = tmp;
            *(u32*)(sp + 0x98) = tmp;
            if ((s32)tmp < 0) break;
            fn_8017F800();
            tmp = 0x0;
            *(u32*)(sp + 0x5C) = tmp;
            r28 = 0x0;
            r30 = *(u32*)lbl_8047B1B8;
            while (1) {
                tmp = *(u32*)lbl_8047B1BC;
                if ((s32)r28 >= (s32)tmp) break;
                r3 = *(u32*)((u8*)r30 + 0x0);
                if ((s32)r3 == (s32)tmp) {
                    r3 = -0x1;
                    tmp = 0x1;
                    *(u32*)((u8*)r30 + 0x0) = r3;
                    *(u32*)(sp + 0x5C) = tmp;
                    break;
                }
                r30 = r30 + 0x8;
                r28 = r28 + 0x1;

            }

            if ((s32)tmp != 0) {
                r30 = *(u32*)lbl_8047B1B8;
                r28 = 0x0;
                while (1) {
                    r3 = *(u32*)lbl_8047B1BC;
                    if ((s32)r28 >= (s32)tmp) break;
                    if ((s32)r28 >= (s32)tmp) {
                        r3 = r28 + 0x1;
                        tmp = r28 << 3;
                        r3 = r3 << 3;
                        r4 = r30 + r3;
                        r5 = r30 + tmp;
                        r3 = *(u32*)((u8*)r4 + 0x0);
                        tmp = *(u32*)((u8*)r4 + 0x4);
                        *(u32*)((u8*)r5 + 0x0) = r3;
                        *(u32*)((u8*)r5 + 0x4) = tmp;
                    }
                    r28 = r28 + 0x1;

                }
                r4 = *(u32*)lbl_8047B1BC;
                tmp = 0x0;
                r3 = -0x1;
                *(u32*)(sp + 0x64) = tmp;
                *(u32*)lbl_8047B1BC = tmp;
                tmp = *(u32*)lbl_8047B1BC;
                tmp = tmp << 3;
                *(u32*)(r30 + tmp) = r3;
            } else {

                tmp = -0x1;
                *(u32*)(sp + 0x64) = tmp;
            }
            if ((s32)tmp < 0) break;
            fn_8017FA5C();
            r4 = *(u32*)((u8*)r31 + 0x20);
            if (r4 <= tmp) break;
            tmp = r3 + 0x1;
            *(u32*)(sp + 0x94) = tmp;
        } while (1);
    } while (0);
    r4 = *(u32*)((u8*)r31 + 0xF8);
    r3 = r18;
    r5 = *(u32*)((u8*)r26 + 0x20);
    r6 = *(u32*)((u8*)r26 + 0x0);
    fn_8017F928();
    fn_8017FA5C();
    r4 = *(u32*)((u8*)r31 + 0x20);
    do {
        if (r4 <= tmp) break;
        tmp = 0x0;
        r24 = *(u32*)lbl_8047B1B8;
        *(u32*)(sp + 0x50) = tmp;
        while (1) {
            tmp = *(u32*)lbl_8047B1BC;
            if ((s32)r3 >= (s32)tmp) break;
            r3 = *(u32*)((u8*)r24 + 0x0);
            tmp = *(u32*)((u8*)r31 + 0x8);
            if ((s32)r3 == (s32)tmp) {
                tmp = *(u32*)((u8*)r24 + 0x0);
                *(u32*)(sp + 0x54) = tmp;
                goto L_8017A250;
            }
            r24 = r24 + 0x8;
            tmp = r3 + 0x1;
            *(u32*)(sp + 0x50) = tmp;

        }
        tmp = -0x1;
        *(u32*)(sp + 0x54) = tmp;
    L_8017A250:
        if ((s32)tmp >= 0) break;
        tmp = 0x0;
        *(u32*)(sp + 0x88) = tmp;
        do {
            tmp = *(u32*)lbl_8047B1B8;
            *(u32*)(sp + 0x48) = tmp;
            tmp = *(u32*)((u8*)r3 + 0x0);
            *(u32*)(sp + 0x44) = tmp;
            *(u32*)(sp + 0x4C) = tmp;
            *(u32*)(sp + 0x8C) = tmp;
            if ((s32)tmp < 0) break;
            fn_8017F800();
            tmp = 0x0;
            *(u32*)(sp + 0x38) = tmp;
            r27 = 0x0;
            r29 = *(u32*)lbl_8047B1B8;
            while (1) {
                tmp = *(u32*)lbl_8047B1BC;
                if ((s32)r27 >= (s32)tmp) break;
                r3 = *(u32*)((u8*)r29 + 0x0);
                if ((s32)r3 == (s32)tmp) {
                    r3 = -0x1;
                    tmp = 0x1;
                    *(u32*)((u8*)r29 + 0x0) = r3;
                    *(u32*)(sp + 0x38) = tmp;
                    break;
                }
                r29 = r29 + 0x8;
                r27 = r27 + 0x1;

            }

            if ((s32)tmp != 0) {
                r29 = *(u32*)lbl_8047B1B8;
                r27 = 0x0;
                while (1) {
                    r3 = *(u32*)lbl_8047B1BC;
                    if ((s32)r27 >= (s32)tmp) break;
                    if ((s32)r27 >= (s32)tmp) {
                        r3 = r27 + 0x1;
                        tmp = r27 << 3;
                        r3 = r3 << 3;
                        r4 = r29 + r3;
                        r5 = r29 + tmp;
                        r3 = *(u32*)((u8*)r4 + 0x0);
                        tmp = *(u32*)((u8*)r4 + 0x4);
                        *(u32*)((u8*)r5 + 0x0) = r3;
                        *(u32*)((u8*)r5 + 0x4) = tmp;
                    }
                    r27 = r27 + 0x1;

                }
                r4 = *(u32*)lbl_8047B1BC;
                tmp = 0x0;
                r3 = -0x1;
                *(u32*)(sp + 0x40) = tmp;
                *(u32*)lbl_8047B1BC = tmp;
                tmp = *(u32*)lbl_8047B1BC;
                tmp = tmp << 3;
                *(u32*)(r29 + tmp) = r3;
            } else {

                tmp = -0x1;
                *(u32*)(sp + 0x40) = tmp;
            }
            if ((s32)tmp < 0) break;
            fn_8017FA5C();
            r4 = *(u32*)((u8*)r31 + 0x20);
            if (r4 <= tmp) break;
            tmp = r3 + 0x1;
            *(u32*)(sp + 0x88) = tmp;
        } while (1);
    } while (0);
    tmp = 0x20000;
    if (r3 < tmp) {
        tmp = r3 + 0x1f;
        /* clrrwi tmp, tmp, 5 */;
        *(u32*)((u8*)r31 + 0x110) = tmp;
    } else {

        tmp = 0x20000;
        *(u32*)((u8*)r31 + 0x110) = tmp;
    }
    r4 = (u32)lbl_80453FEC;
    r3 = (u32)lbl_80453FEC;
    r7 = 0x0;
    *(u32*)((u8*)r31 + 0x114) = tmp;
    tmp = 0x0;
    r5 = (u32)lbl_80453FEC;
    *(u32*)((u8*)r31 + 0x118) = r7;
    r4 = (u32)lbl_80453FEC;
    r3 = (u32)lbl_80453FEC;
    *(u32*)((u8*)r31 + 0x11C) = r6;
    r6 = (u32)lbl_80453FEC;
    r5 = (u32)lbl_8047B1C0;
    *(u32*)((u8*)r31 + 0x134) = r8;
    *(u32*)((u8*)r31 + 0x138) = r7;
    *(u32*)((u8*)r31 + 0x13C) = r8;
    *(u32*)((u8*)r31 + 0x128) = r7;
    *(u32*)((u8*)r31 + 0x124) = tmp;
    *(u32*)((u8*)r4 + 0x1C) = r31;
    tmp = *(u32*)((u8*)r6 + 0x24);
    *(u32*)((u8*)r3 + 0x20) = r31;
    r3 = tmp << 2;
    tmp = *(u32*)((u8*)r31 + 0x40);
    r19 = *(u32*)(r5 + r3);
    *(u32*)(sp + 0xA4) = tmp;
    tmp = *(u32*)((u8*)r3 + 0x10);
    tmp = tmp & 0x1;
    if (r3 != tmp) {
        r3 = 0x0;
        *(u32*)((u8*)r31 + 0x6C) = r3;
        if (tmp != 0) {
            fn_80167EF8();
            tmp = r3 & 0xFF;
            if (tmp != 0) {
                fn_80167F28();
                *(u32*)((u8*)r31 + 0x6C) = r3;
        }
        }
        tmp = *(u32*)((u8*)r31 + 0x6C);
        if (tmp == 0) {
            r4 = (u32)fn_8017A5FC;
            r3 = *(u32*)((u8*)r31 + 0x68);
            r7 = (u32)fn_8017A5FC;
            r5 = *(u32*)((u8*)r31 + 0x110);
            r4 = r19;
            fn_80167E98();
            return;
        }
        r3 = *(u32*)((u8*)r31 + 0x6C);
        fn_80167E5C();
        tmp = 0x20000;
        r21 = r3;
        if (r21 < tmp) {
            tmp = r21 + 0x1f;
            /* clrrwi tmp, tmp, 5 */;
            *(u32*)((u8*)r31 + 0x110) = tmp;
        } else {

            tmp = 0x20000;
            *(u32*)((u8*)r31 + 0x110) = tmp;
        }
        *(u32*)((u8*)r26 + 0x14) = r21;
        tmp = 0x0;
        r23 = 0x0;
        r20 = 0x0;
        *(u32*)((u8*)r31 + 0x114) = r21;
        *(u32*)((u8*)r31 + 0x11C) = tmp;
        while (1) {
            tmp = *(u32*)((u8*)r31 + 0xC);
            if (r20 >= tmp) break;
            r22 = *(u32*)((u8*)r31 + 0x40);
            if (r22 != 0) {
                tmp = tmp << 2;
                r3 = *(u32*)((u8*)r3 + 0x18);
                r3 = r22 + r3;
                r3 = *(u32*)((u8*)r3 + 0x0);
                r3 = r22 + r3;
                tmp = *(u32*)(r3 + tmp);
                tmp = r22 + tmp;
                *(u32*)(sp + 0x30) = tmp;
            } else {

                tmp = 0x0;
                *(u32*)(sp + 0x30) = tmp;
            }
            r20 = r20 + 0x1;
            *(u32*)(sp + 0x7C) = tmp;
            tmp = *(u32*)((u8*)r3 + 0x14);
            r23 = r23 + tmp;

        }
        r5 = *(u32*)((u8*)r31 + 0x40);
        r3 = (u32)fn_8017A5FC;
        tmp = *(u32*)((u8*)r31 + 0x18);
        r7 = (u32)fn_8017A5FC;
        r4 = r19;
        r6 = 0x0;
        tmp = r5 + tmp;
        *(u32*)(sp + 0x80) = tmp;
        tmp = *(u32*)((u8*)r3 + 0x8);
        r23 = r23 + tmp;
        *(u32*)((u8*)r31 + 0x20) = r23;
        r3 = *(u32*)((u8*)r31 + 0x6C);
        r5 = *(u32*)((u8*)r31 + 0x110);
        fn_80167E98();
        return;
    }
    r4 = (u32)fn_8017A5FC;
    r3 = *(u32*)((u8*)r31 + 0x68);
    r7 = (u32)fn_8017A5FC;
    r5 = *(u32*)((u8*)r31 + 0x110);
    r4 = r19;
    fn_80167E98();

    return;
}

/* ===== Small accessor/setter functions ===== */

/* fn_801765F4 -- nop accessor, 0xC bytes */
void* GSscene_NopAccessor1(void) {
    return (void*)0;
}

/* fn_80175F44 -- get object count, 0x28 bytes */
u32 GSscene_GetObjectCount(void) {
    extern u8 lbl_80478C38[];
    u32 tmp = 0;
    u32 r3 = 0;

    r3 = *(u16*)lbl_80478C38;
    r3 = r3 + 0x1;
    tmp = r3 & 0xFFFF;
    *(u16*)lbl_80478C38 = r3;
    if (tmp < 0x100) {
        tmp = 0x100;
        *(u16*)lbl_80478C38 = tmp;
    }
    r3 = *(u16*)lbl_80478C38;
    return;
}

/* ==================================================================
 * Position accessors (fn_80176948-801769B0)
 *
 * Three 0x34-byte functions that return X, Y, Z position of a scene
 * object. Pattern:
 *   load object pointer from table
 *   lfs fX, offset(r3)
 *   blr
 * ================================================================== */
f32 GSscene_GetPositionX(u32 obj) {
    extern void fn_800E01D0();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r4 = 0;

obj = (u32)sp + 0x8;
    ((void(*)(void))fn_800E01F4)();
obj = *(u32*)&lbl_80478C40;
    r4 = (u32)sp + 0x8;
obj = obj + 0x4;
    fn_800E01D0();
    return;
}

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
