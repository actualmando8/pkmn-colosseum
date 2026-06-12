/**
 * @file hsd_fog.c
 * @brief HSD Fog and FogAdj implementation.
 *
 * Colosseum address: 0x8019B7C0 (HSD_FogAdjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/fog.c
 */

#include "hsd/hsd_fog.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_object.h"

extern void* memcpy(void* dst, const void* src, u32 size);

static void FogInfoInit(void);
static void FogAdjInfoInit(void);

static HSD_FogInfo hsdFog = { FogInfoInit };
static HSD_FogAdjInfo hsdFogAdj = { FogAdjInfoInit };

/* ========================================================================= */
/*  FogAdj                                                                   */
/* ========================================================================= */

void HSD_FogAdjInit(HSD_FogAdj* fogadj, HSD_FogAdjDesc* desc)
{
    if (fogadj == NULL || desc == NULL) {
        return;
    }
    fogadj->center = (s16) desc->center;
    fogadj->width = desc->width;
    memcpy(fogadj->mtx, desc->mtx, sizeof(f32) * 16);
}

HSD_FogAdj* HSD_FogAdjLoadDesc(HSD_FogAdjDesc* desc)
{
    HSD_FogAdj* fogadj;

    if (desc == NULL) {
        return NULL;
    }

    fogadj = HSD_FogAdjAlloc();
    HSD_FogAdjInit(fogadj, desc);
    return fogadj;
}

HSD_FogAdj* HSD_FogAdjAlloc(void)
{
    HSD_FogAdj* fogadj;
    fogadj = (HSD_FogAdj*) hsdNew(&hsdFogAdj.parent.parent);
    HSD_ASSERT(0, fogadj);
    return fogadj;
}

/* ========================================================================= */
/*  Fog                                                                      */
/* ========================================================================= */

void HSD_FogInit(HSD_Fog* fog, HSD_FogDesc* desc)
{
    if (fog == NULL || desc == NULL) {
        return;
    }
    fog->type = desc->type;
    fog->start = desc->start;
    fog->end = desc->end;
    fog->color = desc->color;
    fog->fog_adj = HSD_FogAdjLoadDesc(desc->fogadjdesc);
}

HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc)
{
    HSD_Fog* fog;

    if (desc == NULL) {
        return NULL;
    }

    fog = HSD_FogAlloc();
    HSD_FogInit(fog, desc);
    return fog;
}

HSD_Fog* HSD_FogAlloc(void)
{
    HSD_Fog* fog;
    fog = (HSD_Fog*) hsdNew(&hsdFog.parent.parent);
    HSD_ASSERT(0, fog);
    return fog;
}

void HSD_FogReqAnim(HSD_Fog* fog, f32 frame)
{
    if (fog != NULL) {
        HSD_AObjReqAnim(fog->aobj, frame);
    }
}

void HSD_FogInterpretAnim(HSD_Fog* fog)
{
    if (fog == NULL) {
        return;
    }
    /* Interpret AObj -> update fog parameters */
}

/* ========================================================================= */
/*  Class init                                                               */
/* ========================================================================= */

static void FogRelease(HSD_Class* o)
{
    HSD_Fog* fog = (HSD_Fog*) o;
    HSD_AObjRemove(fog->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFog)->release(o);
}

static void FogInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFog), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_fog",
                     sizeof(HSD_FogInfo), sizeof(HSD_Fog));
    HSD_CLASS_INFO(&hsdFog)->release = FogRelease;
}

static void FogAdjRelease(HSD_Class* o)
{
    HSD_FogAdj* fogadj = (HSD_FogAdj*) o;
    HSD_AObjRemove(fogadj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFogAdj)->release(o);
}

static void FogAdjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFogAdj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_fogadj",
                     sizeof(HSD_FogAdjInfo), sizeof(HSD_FogAdj));
    HSD_CLASS_INFO(&hsdFogAdj)->release = FogAdjRelease;
}

/* ===================================================================
 * Generated: 0 pattern-matched + 13 stubs
 * Range: 0x8019B7C0 - 0x8019C690
 * =================================================================== */

/* 0x8019B808 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8019B874(void);
extern void fn_8019B948(void);
extern u8 lbl_8036C7E8[];
extern u8 lbl_8036CC00[];
extern u8 lbl_802747B8[];
extern u8 lbl_8047DA60;
void fn_8019B808(void) {
    hsdInitClassInfo((HSD_ClassInfo*) lbl_8036C7E8,
                     (HSD_ClassInfo*) lbl_8036CC00, (char*) lbl_802747B8,
                     (char*) &lbl_8047DA60, 0x40, 0x20);
    ((void**)lbl_8036C7E8)[0x30/4] = (void*)fn_8019B874;
    ((void**)lbl_8036C7E8)[0x3c/4] = (void*)fn_8019B948;
}

/* 0x8019B874 | 0xD4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801C25E4(void);
#if 1
asm void fn_8019B874(void) {
#include "src/hsd/hsd_fog_fn_8019B874.inc"
}
#else
void fn_8019B874(void) {
    /* TODO: match -- 212 bytes at 0x8019B874 */
}
#endif
#pragma pop

/* 0x8019B948 | 0x230 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void jumptable_8036C864();
extern u32 lbl_8047DA68;
extern u32 lbl_8047DA6C;
extern u32 lbl_8047DA70;
#if 1
asm void fn_8019B948(void) {
#include "src/hsd/hsd_fog_fn_8019B948.inc"
}
#else
void fn_8019B948(void) {
    /* TODO: match -- 560 bytes at 0x8019B948 */
}
#endif
#pragma pop

/* 0x8019BB78 | 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80193828(void);
extern void fn_800BD768(void);
extern void* memset(void* dst, int val, u32 n);
extern u8 lbl_8047DA74[];
extern u8 lbl_8047DA80[];
extern u8 lbl_8036C828[];
extern u8 lbl_8047DA7C[];
#if 1
asm void fn_8019BB78(void) {
#include "src/hsd/hsd_fog_fn_8019BB78.inc"
}
#else
void fn_8019BB78(void) {
    /* TODO: match -- 416 bytes at 0x8019BB78 */
}
#endif
#pragma pop

/* 0x8019BD18 | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800BC8F8(void);
extern void HSD_CObjGetCurrent(void);
extern void HSD_Panic(void);
extern void HSD_CObjGetFar(void);
extern void HSD_CObjGetNear(void);
extern void fn_800BD454(void);
extern void fn_800BCB14(void);
extern void fn_800BCCDC(void);
extern u32 lbl_8047E720;
extern u32 lbl_8047DA68;
extern u8 lbl_802747DC[];
extern u32 lbl_8047DA90;
extern u32 lbl_8047DA84;
extern u32 lbl_8047DA88;
extern u32 lbl_8047DA8C;
extern u32 lbl_8047DA6C;
#if 1
asm void fn_8019BD18(void) {
#include "src/hsd/hsd_fog_fn_8019BD18.inc"
}
#else
void fn_8019BD18(void) {
    /* TODO: match -- 720 bytes at 0x8019BD18 */
}
#endif
#pragma pop

/* 0x8019BFE8 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u8 lbl_80274800[];
extern u8 lbl_8047DA98[];
#if 1
asm void HSD_HashSearch(void) {
#include "src/hsd/hsd_fog_HSD_HashSearch.inc"
}
#else
void HSD_HashSearch(void) {
    /* TODO: match -- 272 bytes at 0x8019BFE8 */
}
#endif
#pragma pop

/* 0x8019C0F8 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u8 lbl_804653A8[];
#if 0
asm void _HSD_IDForgetMemory(void) {
#include "src/hsd/hsd_fog__HSD_IDForgetMemory.inc"
}
#else
void _HSD_IDForgetMemory(void) {
    memset(lbl_804653A8, 0, 0x194);
}
#endif
#pragma pop

/* 0x8019C128 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void HSD_IDGetDataFromTable(void) {
#include "src/hsd/hsd_fog_HSD_IDGetDataFromTable.inc"
}
#else
#pragma optimization_level 4
void* HSD_IDGetDataFromTable(u32* table, u32 key, u32* found) {
    u32** bucket;
    u32** node;

    if (table == NULL) {
        table = (u32*)lbl_804653A8;
    }
    bucket = (u32**)(table + key % 101);
    node = (u32**)*bucket;
    while (node != NULL) {
        if ((u32)node[1] == key) {
            if (found != NULL) {
                *found = 1;
            }
            return node[2];
        }
        node = (u32**)node[0];
    }
    if (found != NULL) {
        *found = 0;
    }
    return NULL;
}
#endif
#pragma pop

/* 0x8019C1B0 | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void HSD_ObjFree(void* list, void* data);
extern u8 lbl_8046553C[];
#if 1
asm void fn_8019C1B0(void) {
#include "src/hsd/hsd_fog_fn_8019C1B0.inc"
}
#else
#pragma optimization_level 4
void fn_8019C1B0(u32* table, u32 key) {
    u32** bucket;
    u32** node;
    u32** prev;
    u32 hash;

    if (table == NULL) {
        table = (u32*)lbl_804653A8;
    }
    hash = key % 101;
    bucket = (u32**)(table + hash);
    node = (u32**)*bucket;
    prev = NULL;
    while (node != NULL) {
        if ((u32)node[1] == key) {
            if (prev != NULL) {
                prev[0] = (u32*)node[0];
            } else {
                *bucket = (u32*)node[0];
            }
            HSD_ObjFree(lbl_8046553C, node);
            return;
        }
        prev = node;
        node = (u32**)node[0];
    }
}
#endif
#pragma pop

/* 0x8019C264 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void* HSD_ObjAlloc(void* list);
extern u8 lbl_8047DAA0[];
extern u8 lbl_8047DAA8[];
#if 1
asm void HSD_IDInsertToTable(void) {
#include "src/hsd/hsd_fog_HSD_IDInsertToTable.inc"
}
#else
#pragma optimization_level 4
void HSD_IDInsertToTable(u32* table, u32 key, u32 value) {
    u32** node;
    u32** newnode;
    u32 hash;

    if (table == NULL) {
        table = (u32*)lbl_804653A8;
    }
    hash = key % 101;
    node = (u32**)*((u32**)(table + hash));
    while (node != NULL) {
        if ((u32)node[1] == key) {
            break;
        }
        node = (u32**)node[0];
    }
    if (node != NULL) {
        node[1] = (u32*)key;
        node[2] = (u32*)value;
    } else {
        newnode = (u32**)HSD_ObjAlloc(lbl_8046553C);
        if (newnode == NULL) {
            __assert((char*)lbl_8047DAA0, 0x43, (char*)lbl_8047DAA8);
        }
        memset(newnode, 0, 0xc);
        newnode[1] = (u32*)key;
        newnode[2] = (u32*)value;
        newnode[0] = (u32*)*((u32**)(table + hash));
        *((u32**)(table + hash)) = (u32*)newnode;
    }
}
#endif
#pragma pop

/* 0x8019C358 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void HSD_IDSetup(void) {
#include "src/hsd/hsd_fog_HSD_IDSetup.inc"
}
#else
void HSD_IDSetup(void) {
    memset(lbl_804653A8, 0, 0x194);
}
#endif
#pragma pop

/* 0x8019C388 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AA35C(void* list, u32 size, u32 alignment);
#if 0
asm void HSD_IDInitAllocData(void) {
#include "src/hsd/hsd_fog_HSD_IDInitAllocData.inc"
}
#else
void HSD_IDInitAllocData(void) {
    fn_801AA35C(lbl_8046553C, 0xC, 4);
}
#endif
#pragma pop

/* 0x8019C3B8 | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void HSD_IDGetAllocData(void) {
#include "src/hsd/hsd_fog_HSD_IDGetAllocData.inc"
}
#else
void* HSD_IDGetAllocData(void) {
    return lbl_8046553C;
}
#endif
#pragma pop
