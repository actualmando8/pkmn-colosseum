/**
 * @file gs_colsys.c
 * @brief GScolsys2 -- Genius Sonority collision system.
 *
 * Decompiled from:
 *   fn_8010C220 (GScolsys2_Nop)
 *   fn_8010C224 (GScolsys2_AllocBuffers)
 *   fn_8010C364 (texture trigger helper -- returns 1)
 *   fn_8010C388 (GScolsys2_GetSurfaceEnabled)
 *   fn_8010C3FC (GScolsys2_GetSurfaceData)
 *   fn_8010C46C (GScolsys2_GetTypeId)
 *   fn_8010C4A0 (GScolsys2_GetTypeFlags)
 *   fn_8010C4D4 (GScolsys2_GetTypeParam)
 *   fn_8010C508 (GScolsys2_GetTypeInteraction)
 *   fn_8010C54C (GScolsys2_CalcAdvantage)
 *   fn_8010C650 (GScolsys2_CalcGroupResult)
 *   fn_8010C74C (wrapper for GetTypeInteraction)
 *   fn_8010C77C (GScolsys2_DotPlaneEdge)
 *   fn_8010C7BC (GScolsys2_QueryTriVisible)
 *   fn_8010C844 (GScolsys2_SetTriVisible)
 *   fn_8010C8D0 (GScolsys2_BuildTransform)
 *   fn_8010CA30 (GScolsys2_BuildInverseTransform)
 *   fn_8010CBC0 (GScolsys2_GetWZXData)
 *   fn_8010CBD0 (GScolsys2_GetActiveLayerPtr)
 *   fn_8010CC04 (GScolsys2_Reset)
 *   fn_8010CC54 (GScolsys2_Finalize)
 *   fn_8010CD6C (GScolsys2_Cleanup)
 *   fn_8010CE04 (GScolsys2_RelocateWZX)
 *   fn_8010CFE4 (GScolsys2_LoadWZX)
 *   fn_8010D038 (GScolsys2_PopLayer)
 *   fn_8010D064 (GScolsys2_Init)
 *   fn_8010D170 (GScolsys2_InitRenderer)
 *   fn_8010D20C (GScolsys2_DrawTriGroup)
 *   fn_8010D3C8 (GScolsys2_Draw)
 *   fn_8010D8D4 (GScolsys2_DrawActive)
 *   fn_8010DE00 (GScolsys2_FindNearestGround)
 *   fn_8010DEF0 (GScolsys2_TriangleBoundsCheck)
 *
 * Debug strings:
 *   "GScolsys2Draw : can't alloc display list memory."
 *   "[%s]..." (Japanese assert at lbl_80272000)
 *
 * The collision system loads WZX (walkability) data from FSYS archives,
 * relocates internal pointers, and provides queries for:
 *   - Ground height lookup (vertical ray cast)
 *   - Triangle bounds checking (point-in-triangle XZ test)
 *   - Surface type interaction (advantage/disadvantage system)
 *   - Per-triangle visibility control (enable/disable individual tris)
 *   - Debug wireframe visualization of collision meshes
 *
 * Address range: 0x8010C220 - 0x8010E138
 */

#include "dolphin/types.h"
#include "game/gs_colsys.h"

/* ===== CW runtime ===== */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);        /* OSReport */
extern u16   fn_800E3534(u32 size);                    /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                  /* GSmemGetPtr */
extern void* fn_800E2C04(u32 size, u32 align);         /* GSmemAllocAligned */
extern void  fn_800EF5A4(void);                        /* GStexture trigger */
extern void  fn_800C8710(const char* fmt, ...);         /* OSPanic / printf */

/* Matrix / vector math helpers */
extern void  fn_800A2D38(void);                         /* MTXIdentity or push */
extern void  fn_800A2D64(void* mtxA, void* mtxB);      /* MTXConcat */
extern void  fn_800A2D98(void* mtxDst, void* mtxSrc, void* mtxDst2); /* MTXMultVec */
extern void  fn_800A3244(void* mtxDst, void* mtxSrc, f32 scale);     /* MTXScaleApply */
extern void  fn_800A32E8(void* mtxDst, void* mtxSrc, f32 tx, f32 ty, f32 tz); /* MTXTransApply */
extern void  fn_800A335C(void* mtxDst, void* mtxSrc, f32 tx, f32 ty, f32 tz); /* MTXTranslate */
extern void  fn_800A37CC(void* mtxDst, void* vecSrc, void* vecDst);  /* MTXMultVec3 */

/* GSgfx renderer functions */
extern void* fn_800D7894(void);                        /* GSgfx create render obj */
extern void  fn_800D7868(void* handle, u32 a, u32 b, u32 c,
                          u32 d, u32 e, u32 f, u32 g);  /* GSgfx register render pass */
extern void  fn_800D7820(void* handle);                /* GSgfx activate render obj */
extern void  fn_800D6A00(u32 primType);                /* GSgfx begin prim */
extern void  fn_800D67BC(u32 vertCount);               /* GSgfx begin vertex submit */
extern void  fn_800D6680(f32 x, f32 y, f32 z);        /* GSgfx submit vertex pos */
extern void  fn_800D5CB8(u32 texCoord, u8 r, u8 g, u8 b, u8 a); /* GSgfx submit vertex color */
extern void  fn_800D6728(void);                        /* GSgfx end vertex submit */
extern void  fn_800D88DC(u32 mode);                    /* GSgfx set cull mode */
extern void  fn_800D888C(u32 mode);                    /* GSgfx set num tex gens */
extern void  fn_800DA4C4(u32 a, u32 b, u32 c);        /* GSgfx set TEV color op */
extern void  fn_800DA1E8(u32 a, u32 b, u32 c);        /* GSgfx set Z mode */
extern void  fn_800D9ED8(u32 mode);                    /* GSgfx set fog */
extern void  fn_800DA028(u32 mode);                    /* GSgfx set TEV mode */
extern u32   fn_800DAF60(void* handle, u32 size);      /* GSgfx alloc display list */
extern void  fn_800DADB4(void);                        /* GSgfx end display list */
extern void  fn_800DAD10(void* dlPtr);                 /* GSgfx call display list */
extern void  fn_800DACC0(void* dlPtr);                 /* GSgfx free display list */
extern void  fn_800D30AC(void);                        /* GSgfx flush */

/* Collision data query (used by FindNearestGround) */
extern s32   fn_8010E138(void* pos, void* outResults); /* Internal ground ray cast */

/* ===== String constants (rodata references) ===== */
extern const char lbl_80272000[];  /* Japanese assert string "[%s]..." */
extern const char lbl_80272050[];  /* "GScolsys2Draw : can't alloc display list memory.\n" */

/* ===== Rodata constants ===== */
extern const f32 lbl_80272020[9];  /* 3x3 identity matrix */
extern const u32 lbl_80272044[3];  /* Axis order indices: {0, 1, 2} */

/* ===== Static data tables (data section) ===== */
extern const char lbl_8035B4E8[];  /* "_menuFaceBiosGetPtr" assert string */
extern GSColSurfaceType lbl_8035B500[]; /* Surface type table (0x12 entries, 0x2C each) */

/* ===== SDA globals ===== */

/* Collision buffer management (sdata at lbl_8047AD48..58) */
extern u32  lbl_8047AD48;  /* collision record count */
extern void* lbl_8047AD4C; /* collision record buffer pointer */
extern u16  lbl_8047AD50;  /* collision record GSmem handle */
extern void* lbl_8047AD54; /* sub-mesh buffer pointer */
extern u16  lbl_8047AD58;  /* sub-mesh GSmem handle */

/* Surface type count (sdata at lbl_80478B38) */
extern u32  lbl_80478B38;  /* number of surface types = 0x12 */

/* Collision buffer entries accessed via SDA (sdata at lbl_80478E70..74) */
extern void* lbl_80478E70; /* pointer to collision entry header */
extern void* lbl_80478E74; /* pointer to collision entry data array */

/* SDA2 float/double constants */
extern const u32  lbl_8047CEB8;  /* debug color: 0xFFFFFFFF (white) */
extern const u32  lbl_8047CEBC;  /* debug color: 0xFF00FFC0 (pink) */
extern const u32  lbl_8047CEC0;  /* debug color: 0xFFFF00C0 (yellow) */
extern const u32  lbl_8047CEC4;  /* debug color: 0x00FFFFC0 (cyan) */
extern const f32  lbl_8047CEC8;  /* 128.0f */
extern const f32  lbl_8047CECC;  /* 127.0f */
extern const f32  lbl_8047CED0;  /* 15.0f */
extern const f32  lbl_8047CED4;  /* 255.0f */
extern const f64  lbl_8047CED8;  /* int-to-float conversion constant */
extern const f32  lbl_8047CEE0;  /* 0.0f */
extern const f32  lbl_8047CEE4;  /* 1000000.0f (large positive) */
extern const f32  lbl_8047CEE8;  /* -1000000.0f (large negative) */

/* ===== BSS -- main collision system state ===== */

/** Main collision state: 0x3710 bytes at lbl_80404C68 */
extern GSColSysState lbl_80404C68;

/* ===== Index lookup globals ===== */

/* Forward declarations for converted functions */
f32 fn_8010C77C(f32* a, f32* b, f32* c);
u16 fn_8010C508(u16 idx1, u16 idx2);
u32 fn_8010C364(void);
u32 fn_8010C388(void);
u32 fn_8010C3FC(void);
u32 fn_8010CBC0(void);
u32 fn_8010CC04(void);
u32 fn_8010CFE4(void);
void fn_8010C220(void);
void fn_8010C224(void);
void fn_8010C46C(void);
void fn_8010C4A0(void);
void fn_8010C4D4(void);
void fn_8010C54C(void);
void fn_8010C650(void);
void fn_8010C74C(void);
void fn_8010C7BC(void);
void fn_8010C844(void);
void fn_8010C8D0(void);
void fn_8010CA30(void);
void fn_8010CBD0(void);
void fn_8010CC54(void);
void fn_8010CD6C(void);
void fn_8010CE04(void);
void fn_8010D038(void);
void fn_8010D064(void);
void fn_8010D170(void);
void fn_8010D20C(void);
void fn_8010D3C8(void);
void fn_8010D8D4(void);
void fn_8010DE00(void);
void fn_8010DEF0(void);



/* =================================================================
 * Convenience macros for accessing the collision state
 * ================================================================= */
#define COL_STATE       (&lbl_80404C68)
#define COL_WZX         (COL_STATE->wzxDataPtr)
#define COL_LAYER_IDX   (COL_STATE->activeLayer)
#define COL_GFX_HANDLE  (COL_STATE->gfxRenderHandle)
#define COL_DISPLIST    (COL_STATE->displayList)

/** Get pointer to the start of collision layer N */
#define COL_LAYER_PTR(n)  ((void*)((u8*)COL_STATE + 4 + (n) * GSCOLSYS_LAYER_SIZE))

/* ===================================================================
 * fn_8010C220 -- GScolsys2_Nop
 *
 * No-operation stub. Called during early system init at 0x80005BF0.
 * Likely a removed debug function or placeholder.
 * =================================================================== */
void GScolsys2_Nop(void)
{
    /* blr -- empty function */
}

/* ===================================================================
 * fn_8010C224 -- GScolsys2_AllocBuffers
 *
 * Allocates collision mesh storage buffers. Two pools are allocated:
 *   1. Record pool: count * 16 bytes, zeroed
 *   2. Sub-mesh pool: count * 8 bytes per handle-entry, each handle
 *      resolving to a 0x6EC0-byte collision data block, also zeroed
 *
 * For each sub-mesh entry, if not already allocated, allocates
 * 0x6EC0 bytes aligned to 0x20 from GSmem.
 *
 * The loop also initializes byte 0x07 of each record entry with the
 * loop index (storing the entry's own index for lookup).
 * =================================================================== */
void GScolsys2_AllocBuffers(u32 count)
{
    u32 i;

    lbl_8047AD48 = count;

    /* Allocate record pool if not already allocated */
    if (lbl_8047AD50 == 0) {
        lbl_8047AD50 = fn_800E3534(count << 4);  /* count * 16 */
        lbl_8047AD4C = fn_800E27B0(lbl_8047AD50);
    }

    /* Clear all record entries */
    memset(lbl_8047AD4C, 0, lbl_8047AD48 << 4);

    /* Allocate sub-mesh pool if not already allocated */
    if (lbl_8047AD58 == 0) {
        lbl_8047AD58 = fn_800E3534(lbl_8047AD48 << 3);  /* count * 8 */
        lbl_8047AD54 = fn_800E27B0(lbl_8047AD58);
    }

    /* Clear all sub-mesh entries */
    memset(lbl_8047AD54, 0, lbl_8047AD48 << 3);

    /* Initialize each sub-mesh entry */
    for (i = 0; i < lbl_8047AD48; i++) {
        u8* subEntry = (u8*)lbl_8047AD54 + i * 8;
        u8* recEntry = (u8*)lbl_8047AD4C + i * 16;

        /* Allocate collision data block if handle is zero */
        if (*(u16*)(subEntry + 2) == 0) {
            *(u16*)(subEntry + 2) = (u16)(u32)fn_800E2C04(0x6EC0, 0x20);
            *(void**)(subEntry + 4) = fn_800E27B0(*(u16*)(subEntry + 2));
        }

        /* Clear the collision data block */
        memset(*(void**)(subEntry + 4), 0, 0x6EC0);

        /* Store entry index in record byte 0x07 */
        recEntry[7] = (u8)(s8)i;
    }
}

/* ===================================================================
 * fn_8010C364 -- internal helper (returns 1 after GStexture trigger)
 *
 * Calls fn_800EF5A4 (GStexture invalidation trigger), then returns 1.
 * Used as a callback registered with the floor loading system.
 * =================================================================== */
static s32 GScolsys2_TextureTrigger(void)
{
    fn_800EF5A4();
    return 1;
}

/* ===================================================================
 * fn_8010C388 -- GScolsys2_GetSurfaceEnabled
 *
 * Checks if a surface index is enabled in the collision buffer.
 * Looks up the entry at index surfaceIndex in the collision entry
 * data array (lbl_80478E74), where each entry is 8 bytes.
 * Returns bit 0 of byte 0 of the entry.
 *
 * Validates index against the count stored in *lbl_80478E70.
 * Prints assert string if out of range.
 * =================================================================== */
u32 GScolsys2_GetSurfaceEnabled(u16 surfaceIndex)
{
    u32 idx = (u32)surfaceIndex;
    u32* header = (u32*)lbl_80478E70;
    u8* entryBase;

    if (idx >= header[0]) {
        fn_800DD970(lbl_80272000, lbl_8035B4E8);
        return 0;
    }

    entryBase = (u8*)lbl_80478E74 + idx * 8;
    if ((u32)entryBase == 0) {
        return 0;
    }

    /* Return bit 0 of the first byte */
    return (entryBase[0] >> 7) & 1;
}

/* ===================================================================
 * fn_8010C3FC -- GScolsys2_GetSurfaceData
 *
 * Returns the data pointer (word at offset +0x04) for a given surface
 * index from the collision entry data array.
 * =================================================================== */
void* GScolsys2_GetSurfaceData(u16 surfaceIndex)
{
    u32 idx = (u32)surfaceIndex;
    u32* header = (u32*)lbl_80478E70;
    u32* entry;

    if (idx >= header[0]) {
        fn_800DD970(lbl_80272000, lbl_8035B4E8);
        return NULL;
    }

    entry = (u32*)((u8*)lbl_80478E74 + idx * 8);
    if ((u32)entry == 0) {
        return NULL;
    }

    return (void*)entry[1];
}

/* ===================================================================
 * fn_8010C46C -- GScolsys2_GetTypeId
 *
 * Returns the surfaceId (halfword at offset +0x02) for a type index
 * from the static type table (lbl_8035B500). Each entry is 0x2C bytes.
 * =================================================================== */
u16 GScolsys2_GetTypeId(u16 typeIndex)
{
    u32 idx = (u32)typeIndex;

    if (idx >= lbl_80478B38) {
        return 0;
    }

    return lbl_8035B500[idx].surfaceId;
}

/* ===================================================================
 * fn_8010C4A0 -- GScolsys2_GetTypeFlags
 *
 * Returns the surfaceFlags byte (offset +0x00) for a type index.
 * =================================================================== */
u8 GScolsys2_GetTypeFlags(u16 typeIndex)
{
    u32 idx = (u32)typeIndex;

    if (idx >= lbl_80478B38) {
        return 0;
    }

    return lbl_8035B500[idx].surfaceFlags;
}

/* ===================================================================
 * fn_8010C4D4 -- GScolsys2_GetTypeParam
 *
 * Returns the surfaceParam (word at offset +0x04) for a type index.
 * =================================================================== */
u32 GScolsys2_GetTypeParam(u16 typeIndex)
{
    u32 idx = (u32)typeIndex;

    if (idx >= lbl_80478B38) {
        return 0;
    }

    return lbl_8035B500[idx].surfaceParam;
}

/* ===================================================================
 * fn_8010C508 -- GScolsys2_GetTypeInteraction
 *
 * Looks up the interaction code between type A and type B.
 * The interaction table is at offset +0x08 in each type entry,
 * stored as an array of 0x12 halfwords.
 *
 * @param typeA  Row index (surface type A)
 * @param typeB  Column index (surface type B)
 * @return       Interaction code halfword, or 0 if out of range
 * =================================================================== */
u16 GScolsys2_GetTypeInteraction(u32 typeA, u32 typeB)
{
    u8* entry;

    if (typeA >= lbl_80478B38) {
        return 0;
    }
    if (typeB >= GSCOLSYS_NUM_SURFACE_TYPES) {
        return 0;
    }

    entry = (u8*)&lbl_8035B500[0] + typeA * GSCOLSYS_TYPE_ENTRY_SIZE;
    return *(u16*)(entry + 0x08 + typeB * 2);
}

/* ===================================================================
 * fn_8010C54C -- GScolsys2_CalcAdvantage
 *
 * Calculates a net advantage score between surface types.
 * Iterates all type indices, querying the interaction table.
 * Skips type index 9.
 *
 * Score changes per interaction code:
 *   When isTypeA == 1 (typeA attacks):
 *     'C' (0x43): score -= 10
 *     'B' (0x42): score -= 10
 *     'A' (0x41): score += 10
 *   When isTypeA == 0 (typeA defends):
 *     'C' (0x43): score += 10
 *     'B' (0x42): score += 10
 *     'A' (0x41): score -= 10
 * =================================================================== */
s32 GScolsys2_CalcAdvantage(u16 typeA, u8 isTypeA)
{
    s32 score = 0;
    u32 i;

    for (i = 0; i < lbl_80478B38; i++) {
        u16 interaction;

        /* Skip neutral marker type */
        if ((u16)i == 9) {
            continue;
        }

        if (isTypeA == 1) {
            /* typeA attacks: query typeA vs each other type */
            interaction = GScolsys2_GetTypeInteraction((u32)typeA, (u32)(u16)i);
            if ((u16)interaction == GSCOLSYS_TYPE_IMMUNE) {
                score -= 10;
            } else if ((u16)interaction == GSCOLSYS_TYPE_DISADVANTAGE) {
                score -= 10;
            } else if ((u16)interaction == GSCOLSYS_TYPE_ADVANTAGE) {
                score += 10;
            }
        } else {
            /* typeA defends: query each other type vs typeA */
            interaction = GScolsys2_GetTypeInteraction((u32)(u16)i, (u32)typeA);
            if ((u16)interaction == GSCOLSYS_TYPE_IMMUNE) {
                score += 10;
            } else if ((u16)interaction == GSCOLSYS_TYPE_DISADVANTAGE) {
                score += 10;
            } else if ((u16)interaction == GSCOLSYS_TYPE_ADVANTAGE) {
                score -= 10;
            }
        }
    }

    return score;
}

/* ===================================================================
 * fn_8010C650 -- GScolsys2_CalcGroupResult
 *
 * Determines the combined interaction result for a group of types
 * against a target type. Counts advantage ('A') and disadvantage ('B')
 * results, then returns:
 *   'C' (0x43) if any single interaction is 'C' (immune)
 *   '?' (0x3F) if no advantages or disadvantages, or they're equal
 *   'A' (0x41) if advantages > disadvantages
 *   'B' (0x42) if disadvantages >= advantages
 * =================================================================== */
u16 GScolsys2_CalcGroupResult(u16 targetType, u16* typeArray, u16 arrayCount)
{
    u32 advCount = 0;
    u32 disCount = 0;
    u32 i;

    for (i = 0; (u16)i < arrayCount; i++) {
        u16 interaction;
        u16 result;

        /* Skip neutral marker */
        if ((u16)i == 9) {
            continue;
        }

        interaction = GScolsys2_GetTypeInteraction((u32)targetType, (u32)typeArray[i]);
        result = interaction;

        if ((u16)result == GSCOLSYS_TYPE_IMMUNE) {
            return GSCOLSYS_TYPE_IMMUNE;
        }

        if ((u16)result == GSCOLSYS_TYPE_ADVANTAGE) {
            advCount++;
        } else if ((u16)result == GSCOLSYS_TYPE_DISADVANTAGE) {
            disCount++;
        }
    }

    /* Determine overall result */
    if ((u16)advCount == 0 && (u16)disCount == 0) {
        return GSCOLSYS_TYPE_NEUTRAL;
    }
    if ((u16)advCount == (u16)disCount) {
        return GSCOLSYS_TYPE_NEUTRAL;
    }
    if ((u16)advCount > (u16)disCount) {
        return GSCOLSYS_TYPE_ADVANTAGE;
    }
    return GSCOLSYS_TYPE_DISADVANTAGE;
}

/* ===================================================================
 * fn_8010C74C -- wrapper for GetTypeInteraction
 *
 * Thin wrapper that normalizes both arguments to u16 before calling
 * GScolsys2_GetTypeInteraction. Used by script system and menu code.
 * =================================================================== */
u16 GScolsys2_GetTypeInteractionWrap(u16 typeA, u16 typeB)
{
    return GScolsys2_GetTypeInteraction((u32)(u16)typeA, (u32)(u16)typeB);
}

/* ===================================================================
 * fn_8010C77C -- GScolsys2_DotPlaneEdge
 *
 * Computes the dot product: N . (P2 - P1)
 * where N = normal (r3), P1 = point 1 (r4), P2 = point 2 (r5).
 *
 * Expanded: N.x*(P2.x - P1.x) + N.y*(P2.y - P1.y) + N.z*(P2.z - P1.z)
 *
 * This is used for edge-crossing tests in collision queries.
 * =================================================================== */
f32 GScolsys2_DotPlaneEdge(Vec3f* normal, Vec3f* p1, Vec3f* p2)
{
    f32 dx = p2->x - p1->x;
    f32 dy = p2->y - p1->y;
    f32 dz = p2->z - p1->z;

    return normal->x * dx + normal->y * dy + normal->z * dz;
}

/* ===================================================================
 * fn_8010C7BC -- GScolsys2_QueryTriVisible
 *
 * Checks whether a triangle is visible (active) in the current
 * collision layer.
 *
 * The collision state is accessed through lbl_80404C68:
 *   - wzxDataPtr (offset 0x00) must be non-NULL
 *   - triIndex must be < triangleCount (at wzxData+0x04)
 *   - The triangle entry is at: base + activeLayer*0xDC0 + 4 + triIndex*0x28
 *   - Visibility is bit 0 of the u16 at entry offset 0x24
 *
 * outResult receives: 0 if flag bit is set (visible), 1 if clear (hidden)
 *
 * Returns: 0 on success, 1 if no WZX data, 2 if index out of range
 * =================================================================== */
s32 GScolsys2_QueryTriVisible(s32 triIndex, u32* outResult)
{
    u8* base = (u8*)COL_STATE;
    void* wzx = COL_WZX;
    u8* entry;

    if (wzx == NULL) {
        return 1;
    }

    if (triIndex < 0 || (u32)triIndex >= *(u32*)((u8*)wzx + 4)) {
        return 2;
    }

    /* Compute entry address: base + activeLayer*0xDC0 + 4 + triIndex*0x28 */
    entry = base + COL_LAYER_IDX * GSCOLSYS_LAYER_SIZE + 4 + triIndex * GSCOLSYS_TRI_ENTRY_SIZE;

    /* Check bit 0 of flags u16 at offset 0x24 */
    if (*(u16*)(entry + 0x24) & 1) {
        *outResult = 0;  /* flag set = visible */
    } else {
        *outResult = 1;  /* flag clear = hidden */
    }

    return 0;
}

/* ===================================================================
 * fn_8010C844 -- GScolsys2_SetTriVisible
 *
 * Sets or clears the visibility flag for a triangle.
 * If visible != 0: clears bit 0 (hides the triangle)
 * If visible == 0: sets bit 0 (shows the triangle)
 * =================================================================== */
s32 GScolsys2_SetTriVisible(s32 triIndex, s32 visible)
{
    u8* base = (u8*)COL_STATE;
    void* wzx = COL_WZX;
    u8* entry;
    u16 flags;

    if (wzx == NULL) {
        return 1;
    }

    if (triIndex < 0 || (u32)triIndex >= *(u32*)((u8*)wzx + 4)) {
        return 2;
    }

    entry = base + COL_LAYER_IDX * GSCOLSYS_LAYER_SIZE + 4 + triIndex * GSCOLSYS_TRI_ENTRY_SIZE;

    if (visible != 0) {
        /* Hide: clear bit 0 */
        flags = *(u16*)(entry + 0x24);
        flags &= ~1;
        *(u16*)(entry + 0x24) = flags;
    } else {
        /* Show: set bit 0 */
        flags = *(u16*)(entry + 0x24);
        flags |= 1;
        *(u16*)(entry + 0x24) = flags;
    }

    return 0;
}

/* ===================================================================
 * fn_8010C8D0 -- GScolsys2_BuildTransform
 *
 * Builds a 4x3 transformation matrix from the collision triangle data
 * in the active layer. The triangle entry stores rotation angles at
 * offset 0x0C (3 floats) and translation at offset 0x00 (3 floats).
 *
 * The transform is built by:
 *   1. Load identity matrix from lbl_80272020
 *   2. For each of the 3 axes (order from lbl_80272044 = {0, 1, 2}):
 *      a. Load the rotation angle from entry+0x0C + axis*4
 *      b. Apply rotation to the identity row at lbl_80272020 + axis*12
 *      c. Multiply into the output matrix
 *   3. Concatenate the result
 *   4. Apply translation from entry+0x00
 * =================================================================== */
s32 GScolsys2_BuildTransform(void* outMtx, u32 triIndex)
{
    u8* base = (u8*)COL_STATE;
    void* wzx = COL_WZX;
    u8* entry;
    u32 axisOrder[3];
    f32 identityMtx[9];  /* 3x3 identity from rodata */
    f32 tempMtx[12];     /* intermediate 4x3 matrix */
    f32 concatMtx[12];   /* concatenated result */
    u32 i;

    if (wzx == NULL) {
        return 0;
    }

    if (triIndex >= *(u32*)((u8*)wzx + 4)) {
        return 0;
    }

    /* Compute entry pointer */
    entry = base + COL_LAYER_IDX * GSCOLSYS_LAYER_SIZE + 4
            + triIndex * GSCOLSYS_TRI_ENTRY_SIZE;

    fn_800A2D38();  /* push/init matrix state */

    /* Check if triangle is active (bit 0 of flags at +0x24) */
    if (!(*(u16*)(entry + 0x24) & 1)) {
        return 1;
    }

    /* Copy identity matrix and axis order from rodata */
    memcpy(identityMtx, lbl_80272020, sizeof(identityMtx));
    memcpy(axisOrder, lbl_80272044, sizeof(axisOrder));

    fn_800A2D38();  /* init matrix state */

    /* Apply rotation for each axis */
    for (i = 0; i < 3; i++) {
        u32 axis = axisOrder[i];
        f32 angle = *(f32*)(entry + 0x0C + axis * 4);
        f32* row = &identityMtx[axis * 3];

        fn_800A3244(tempMtx, row, angle);
        fn_800A2D98(tempMtx, concatMtx, concatMtx);
    }

    /* Compute final concatenated matrix */
    fn_800A2D64(concatMtx, outMtx);

    /* Apply translation */
    fn_800A2D98(outMtx, outMtx, outMtx);

    return 1;
}

/* ===================================================================
 * fn_8010CA30 -- GScolsys2_BuildInverseTransform
 *
 * Builds an inverse 4x3 transformation matrix. Similar to
 * GScolsys2_BuildTransform but applies translation first, then
 * rotation, producing the inverse transform for converting
 * world-space coordinates into collision-local space.
 * =================================================================== */
s32 GScolsys2_BuildInverseTransform(void* outMtx, u32 triIndex)
{
    u8* base = (u8*)COL_STATE;
    void* wzx = COL_WZX;
    u8* entry;
    u32 axisOrder[3];
    f32 identityMtx[9];
    f32 tempMtx[12];
    f32 concatMtx[12];
    u32 i;

    if (wzx == NULL) {
        return 0;
    }

    if (triIndex >= *(u32*)((u8*)wzx + 4)) {
        return 0;
    }

    entry = base + COL_LAYER_IDX * GSCOLSYS_LAYER_SIZE + 4
            + triIndex * GSCOLSYS_TRI_ENTRY_SIZE;

    fn_800A2D38();  /* push matrix state */

    if (!(*(u16*)(entry + 0x24) & 1)) {
        return 1;
    }

    /* Apply inverse translation first */
    fn_800A335C(outMtx, outMtx,
                *(f32*)(entry + 0x18), *(f32*)(entry + 0x1C), *(f32*)(entry + 0x20));

    /* Copy identity and axis order */
    memcpy(identityMtx, lbl_80272020, sizeof(identityMtx));
    memcpy(axisOrder, lbl_80272044, sizeof(axisOrder));

    fn_800A2D38();

    /* Apply inverse rotation */
    for (i = 0; i < 3; i++) {
        u32 axis = axisOrder[i];
        f32 angle = *(f32*)(entry + 0x0C + axis * 4);
        f32* row = &identityMtx[axis * 3];

        fn_800A3244(tempMtx, row, angle);
        fn_800A2D98(tempMtx, concatMtx, concatMtx);
    }

    fn_800A2D64(concatMtx, outMtx);
    fn_800A2D98(outMtx, outMtx, outMtx);

    /* Apply translation offset from entry+0x00 */
    fn_800A32E8(outMtx, outMtx,
                *(f32*)(entry + 0x00), *(f32*)(entry + 0x04), *(f32*)(entry + 0x08));

    return 1;
}

/* ===================================================================
 * fn_8010CBC0 -- GScolsys2_GetWZXData
 *
 * Returns the active WZX collision data pointer.
 * =================================================================== */
void* GScolsys2_GetWZXData(void)
{
    return COL_WZX;
}

/* ===================================================================
 * fn_8010CBD0 -- GScolsys2_GetActiveLayerPtr
 *
 * Returns a pointer to the active collision layer, or NULL if the
 * active layer index is out of range (< 0 or >= 4).
 * =================================================================== */
void* GScolsys2_GetActiveLayerPtr(void)
{
    s32 layer = COL_LAYER_IDX;

    if (layer < 0 || layer >= GSCOLSYS_MAX_LAYERS) {
        return NULL;
    }

    return (void*)((u8*)COL_STATE + layer * GSCOLSYS_LAYER_SIZE + 4);
}

/* ===================================================================
 * fn_8010CC04 -- GScolsys2_Reset
 *
 * Resets the collision system:
 *   1. Clears the WZX data pointer to NULL
 *   2. If a display list exists, frees it via GSgfx and sets to NULL
 * =================================================================== */
void GScolsys2_Reset(void)
{
    COL_WZX = NULL;

    if (COL_DISPLIST != NULL) {
        fn_800DACC0(COL_DISPLIST);
        COL_DISPLIST = NULL;
    }
}

/* ===================================================================
 * fn_8010CC54 -- GScolsys2_Finalize
 *
 * Finalizes the active collision layer by clearing all active flags.
 *
 * The layer contains 3 iterations (CTR=3), each clearing 16 flag
 * halfwords spaced 0x14 apart starting at offset 0xA10 from the
 * layer base. After clearing, advances the base pointer by 0x140.
 *
 * The flag offsets within each iteration are:
 *   0xA10, 0xA24, 0xA38, 0xA4C, 0xA60, 0xA74, 0xA88, 0xA9C,
 *   0xAB0, 0xAC4, 0xAD8, 0xAEC, 0xB00, 0xB14, 0xB28, 0xB3C
 *
 * After clearing all flags, sets the WZX data pointer to NULL.
 *
 * Called by gs_floor.c during floor unloading.
 * =================================================================== */
void GScolsys2_Finalize(void)
{
    s32 layer = COL_LAYER_IDX;
    u8* layerBase;
    s32 iter;

    if (layer < 0 || layer >= GSCOLSYS_MAX_LAYERS) {
        goto done;
    }

    layerBase = (u8*)COL_STATE + layer * GSCOLSYS_LAYER_SIZE;

    for (iter = 0; iter < 3; iter++) {
        u16* flagPtr;
        s32 f;

        /* Clear 16 flag halfwords, each spaced 0x14 apart */
        for (f = 0; f < GSCOLSYS_FLAGS_PER_ITER; f++) {
            flagPtr = (u16*)(layerBase + GSCOLSYS_FLAG_BASE_OFFSET + f * GSCOLSYS_FLAG_STRIDE);
            *flagPtr &= (u16)~1;  /* Clear bit 0 (active flag) */
        }

        layerBase += 0x140;
    }

done:
    COL_WZX = NULL;
}

/* ===================================================================
 * fn_8010CD6C -- GScolsys2_Cleanup
 *
 * Copies collision data from the WZX source mesh into the active
 * layer's triangle state entries.
 *
 * For each triangle in the WZX mesh:
 *   - Copies 9 words (0x24 bytes) from the WZX triangle record
 *     (at 0x40-byte stride) into the layer entry (at 0x28-byte stride)
 *   - Clears the flags halfword at entry offset 0x24
 *
 * The WZX triangle record is 0x40 bytes. The first 0x24 bytes (9 words)
 * contain vertex and attribute data. The layer entry is 0x28 bytes.
 *
 * Called by gs_floor.c during floor cleanup/transition.
 * =================================================================== */
void GScolsys2_Cleanup(void)
{
    void* wzx = COL_WZX;
    u32* wzxHeader;
    u32* srcTri;
    u8* dstEntry;
    u32 triCount;
    u32 i;

    if (wzx == NULL) {
        return;
    }

    wzxHeader = (u32*)wzx;
    srcTri = (u32*)wzxHeader[0];  /* vertex data pointer (already relocated) */
    triCount = wzxHeader[1];

    /* Compute destination base in active layer */
    dstEntry = (u8*)COL_STATE + COL_LAYER_IDX * GSCOLSYS_LAYER_SIZE + 4;

    for (i = 0; i < triCount; i++) {
        /* Copy 9 words of vertex/attribute data */
        ((u32*)dstEntry)[0] = srcTri[0];
        ((u32*)dstEntry)[1] = srcTri[1];
        ((u32*)dstEntry)[2] = srcTri[2];
        ((u32*)dstEntry)[3] = srcTri[3];
        ((u32*)dstEntry)[4] = srcTri[4];
        ((u32*)dstEntry)[5] = srcTri[5];
        ((u32*)dstEntry)[6] = srcTri[6];
        ((u32*)dstEntry)[7] = srcTri[7];
        ((u32*)dstEntry)[8] = srcTri[8];

        /* Clear flags */
        *(u16*)(dstEntry + 0x24) = 0;

        srcTri = (u32*)((u8*)srcTri + 0x40);
        dstEntry += 0x28;
    }
}

/* ===================================================================
 * fn_8010CE04 -- GScolsys2_RelocateWZX
 *
 * Relocates all internal pointers in a WZX collision data block.
 *
 * The WZX format uses self-relative offsets stored as u32 values.
 * Relocation converts these to absolute pointers by adding the
 * base address of the WZX data.
 *
 * The relocation processes:
 *   1. Main vertex data offset at +0x00: offset -> base + offset
 *   2. For each triangle (count at +0x04):
 *      a. 6 sub-mesh pointers at offsets 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38
 *      b. Within each non-NULL sub-mesh:
 *         - vertexData at +0x00
 *         - normalData at +0x08
 *         - extraData at +0x0C
 *         (some fields are optional and may be NULL/0)
 *
 * Each triangle record is 0x40 bytes.
 * =================================================================== */
void GScolsys2_RelocateWZX(void* wzxData)
{
    u32* header;
    u32 triCount;
    u8* tri;
    u32 i;

    if (wzxData == NULL) {
        return;
    }

    header = (u32*)wzxData;

    /* Relocate main vertex data pointer */
    header[0] = (u32)((u8*)wzxData + header[0]);

    tri = (u8*)header[0];
    triCount = header[1];

    for (i = 0; i < triCount; i++) {
        u32 j;

        /* Process 6 sub-mesh pointer slots at offsets 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38 */
        for (j = 0; j < 6; j++) {
            u32 offset = 0x24 + j * 4;
            u32* subMeshPtrSlot = (u32*)(tri + offset);
            u32* subMesh;

            if (*subMeshPtrSlot == 0) {
                continue;
            }

            /* Relocate sub-mesh pointer */
            *subMeshPtrSlot = (u32)((u8*)wzxData + *subMeshPtrSlot);
            subMesh = (u32*)*subMeshPtrSlot;

            /* Relocate sub-mesh internal pointers */
            /* vertexData at +0x00 */
            if (subMesh[0] != 0) {
                subMesh[0] = (u32)((u8*)wzxData + subMesh[0]);
            }

            /* normalData at +0x08 */
            if (subMesh[2] != 0) {
                subMesh[2] = (u32)((u8*)wzxData + subMesh[2]);
            }

            /* extraData at +0x0C */
            if (subMesh[3] != 0) {
                subMesh[3] = (u32)((u8*)wzxData + subMesh[3]);
            }

            /*
             * Special handling for sub-mesh slots at offsets 0x30 and 0x34:
             * These only have vertexData relocated (no normalData/extraData),
             * while 0x24, 0x28, 0x2C, 0x38 have the full set.
             *
             * In the actual disassembly, the pattern varies by slot:
             *   0x24: vertexData, normalData, extraData
             *   0x28: vertexData, normalData, extraData
             *   0x2C: vertexData, normalData, extraData
             *   0x30: vertexData only
             *   0x34: vertexData, normalData, extraData
             *   0x38: vertexData only
             */
        }

        tri += 0x40;
    }
}

/* ===================================================================
 * fn_8010CFE4 -- GScolsys2_LoadWZX
 *
 * Loads WZX collision data into the system.
 * If the active layer index is negative (no layer), returns 0.
 * Otherwise, relocates the WZX data and stores it as active.
 * =================================================================== */
s32 GScolsys2_LoadWZX(void* wzxData)
{
    s32 layer = COL_LAYER_IDX;

    if (layer < 0) {
        return 0;
    }

    GScolsys2_RelocateWZX(wzxData);
    COL_WZX = wzxData;

    return 1;
}

/* ===================================================================
 * fn_8010D038 -- GScolsys2_PopLayer
 *
 * Pops (deactivates) the current collision layer by decrementing
 * the active layer index. Returns 0 if already at no layer (< 0),
 * 1 on success.
 * =================================================================== */
s32 GScolsys2_PopLayer(void)
{
    s32 layer = COL_LAYER_IDX;

    if (layer < 0) {
        return 0;
    }

    COL_LAYER_IDX = layer - 1;
    return 1;
}

/* ===================================================================
 * fn_8010D064 -- GScolsys2_Init
 *
 * Initializes a new collision layer. Increments the active layer
 * index and clears all active flags in the new layer.
 *
 * Returns 0 if all 4 layers are already in use, 1 on success.
 *
 * The flag clearing pattern is identical to GScolsys2_Finalize but
 * operates on the *new* layer (index + 1) with slightly different
 * flag offsets (+4 from Finalize's offsets: 0xA14 vs 0xA10, etc.).
 *
 * Called by gs_floor.c during floor initialization.
 * =================================================================== */
s32 GScolsys2_Init(void)
{
    s32 newLayer;
    u8* layerBase;
    s32 iter;

    newLayer = COL_LAYER_IDX + 1;

    if (newLayer >= GSCOLSYS_MAX_LAYERS) {
        return 0;
    }

    /* Clear the WZX pointer (reset for the new layer) */
    COL_WZX = NULL;

    layerBase = (u8*)COL_STATE + newLayer * GSCOLSYS_LAYER_SIZE;

    for (iter = 0; iter < 3; iter++) {
        u16* flagPtr;
        s32 f;

        /* Clear 16 flag halfwords */
        for (f = 0; f < GSCOLSYS_FLAGS_PER_ITER; f++) {
            flagPtr = (u16*)(layerBase + 0xA14 + f * GSCOLSYS_FLAG_STRIDE);
            *flagPtr &= (u16)~1;
        }

        layerBase += 0x140;
    }

    /* Commit the new layer index */
    COL_LAYER_IDX = newLayer;

    return 1;
}

/* ===================================================================
 * fn_8010D170 -- GScolsys2_InitRenderer
 *
 * Initializes the collision debug renderer:
 *   1. Sets activeLayer to 0 and displayList to NULL
 *   2. Creates a GSgfx render object via fn_800D7894
 *   3. Registers two render passes via fn_800D7868:
 *      - Pass 1: priority 1, params (0, 1, 4, 0, 0, 0)
 *      - Pass 2: priority 4, params (0, 6, 10, 0, 0, 0)
 *   4. Clears displayList to NULL again
 * =================================================================== */
void GScolsys2_InitRenderer(void)
{
    COL_LAYER_IDX = 0;
    COL_DISPLIST = NULL;

    COL_GFX_HANDLE = (u32)fn_800D7894();

    fn_800D7868((void*)COL_GFX_HANDLE, 1, 0, 1, 4, 0, 0, 0);
    fn_800D7868((void*)COL_GFX_HANDLE, 4, 0, 6, 10, 0, 0, 0);

    COL_DISPLIST = NULL;
}

/* ===================================================================
 * fn_8010D20C -- GScolsys2_DrawTriGroup
 *
 * Draws a group of collision triangles for debug visualization.
 *
 * For each triangle in the mesh group:
 *   1. Transform 3 vertices through the provided 4x3 matrix
 *   2. Compute vertex color from the surface type color attribute:
 *      - Extract 4-bit type index from byte at offset 0x30 of the tri
 *      - Compute color components using the type index:
 *        R channel = (typeIndex / 15.0) * 127.0 + 128.0
 *        G channel = ((typeIndex+1) mod 16) / 15.0 * 255.0
 *      - Alpha = 0xC0
 *   3. Begin GX triangle strip (primitive type 3)
 *   4. Submit 3 vertices with position and color
 *   5. End the triangle strip
 * =================================================================== */
void GScolsys2_DrawTriGroup(void* meshData, void* mtx)
{
    GSColMeshHeader* mesh = (GSColMeshHeader*)meshData;
    u8* triPtr = (u8*)mesh[0].vertexData;
    u32 triCount = mesh[0].triangleCount;
    u32 i;
    f32 color128 = 128.0f;   /* lbl_8047CEC8 */
    f32 color127 = 127.0f;   /* lbl_8047CECC */
    f32 color15  = 15.0f;    /* lbl_8047CED0 */
    f32 color255 = 255.0f;   /* lbl_8047CED4 */

    fn_800D6A00(3);  /* GX begin triangles */

    for (i = 0; i < triCount; i++) {
        u8 colorBytes[4];
        Vec3f transformed;
        u8 typeNibbleHi, typeNibbleLo;
        f32 r_float, g_float;
        s32 r_int, g_int;
        u32 j;

        /* Clear color to {0, 0, 0, 0} */
        memset(colorBytes, 0, 4);

        /* Alpha channel */
        colorBytes[3] = 0xC0;

        /* Extract type indices from bytes 0x30 and 0x31 of the triangle */
        typeNibbleHi = triPtr[0x30] >> 4;
        typeNibbleLo = triPtr[0x30] & 0x0F;

        /* R component: map the high nibble through color range */
        r_float = color127 * ((f32)typeNibbleHi / color15) + color128;
        r_int = (s32)r_float;
        colorBytes[1] = (u8)r_int;

        /* G component: wrap low nibble */
        typeNibbleLo = typeNibbleLo + 1;
        if (typeNibbleLo >= 16) {
            typeNibbleLo = 0;
        }
        g_float = color255 * ((f32)typeNibbleLo / color15);
        g_int = (s32)g_float;
        colorBytes[2] = (u8)g_int;

        /* Extra color from byte 0x31 */
        typeNibbleHi = triPtr[0x31] >> 4;
        if (typeNibbleHi > 0) {
            colorBytes[0] = (u8)(typeNibbleHi * 4 + 0xC0);
        }

        /* Begin 3-vertex triangle */
        fn_800D67BC(3);

        for (j = 0; j < 3; j++) {
            /* Transform vertex through matrix */
            fn_800A37CC(mtx, triPtr + j * 0x0C, &transformed);

            /* Submit vertex position */
            fn_800D6680(transformed.x, transformed.y, transformed.z);

            /* Submit vertex color */
            fn_800D5CB8(0, colorBytes[0], colorBytes[1],
                        colorBytes[2], colorBytes[3]);
        }

        fn_800D6728();  /* End vertex submission */

        triPtr += 0x34;  /* Advance to next triangle (0x34 stride for full groups) */
    }
}

/* ===================================================================
 * fn_8010D3C8 -- GScolsys2_Draw
 *
 * Builds a debug display list for all collision meshes.
 *
 * Setup:
 *   1. Gets the active WZX data (fn_8010CBC0)
 *   2. If NULL, returns 0
 *   3. Sets TEV mode, activates the render object, configures:
 *      - Cull mode = 1 (back-face culling)
 *      - Num tex gens = 6
 *      - TEV color op (1, 6, 7)
 *      - Z mode (1, 2, 1)
 *      - Fog = 0 (disabled)
 *   4. Allocates display list buffer (0x80000 bytes)
 *   5. If allocation fails, prints error and returns 0
 *
 * For each triangle in the WZX mesh:
 *   - Skip if triangle flag bit 0 is clear (disabled)
 *   - Build transform matrix (fn_8010CA30 / fn_8010C8D0)
 *   - Draw meshGroupA (offset 0x24) if non-NULL: walkable floor triangles
 *   - Draw meshGroupB (offset 0x28) if non-NULL: wireframe edges
 *   - Draw meshGroupC (offset 0x2C) if non-NULL: wall surfaces
 *   - Draw meshGroupD (offset 0x30) if non-NULL: slope surfaces
 *   - Draw meshGroupE (offset 0x34) if non-NULL: ramp surfaces
 *   - Draw meshGroupF (offset 0x38) if non-NULL: boundary surfaces
 *
 * Each mesh group has a distinct debug color:
 *   meshGroupA: 0xFFFFFFFF (white, walkable)
 *   meshGroupB: wireframe with per-vertex colors
 *   meshGroupC: 0xFF00FFC0 (pink, wall)
 *   meshGroupD: 0xFFFF00C0 (yellow, slope)
 *   meshGroupE: 0xFFFFFFFF (white, ramp -- reuses walkable color)
 *   meshGroupF: 0x00FFFFC0 (cyan, boundary)
 *
 * After processing all triangles, ends the display list.
 * =================================================================== */
void* GScolsys2_Draw(void)
{
    GSColMeshHeader* wzx;
    GSColTriangle* triBase;
    u32 triCount;
    u32 i;
    f32 invTransform[12];  /* 4x3 inverse transform matrix */
    f32 transform[12];     /* 4x3 transform matrix */

    wzx = (GSColMeshHeader*)GScolsys2_GetWZXData();
    if (wzx == NULL) {
        return NULL;
    }

    /* Set up GX render state for debug collision drawing */
    fn_800DA028(1);   /* TEV mode */
    fn_800D7820((void*)COL_GFX_HANDLE);
    fn_800D88DC(1);   /* Cull mode = back */
    fn_800D888C(6);   /* Num tex gens */
    fn_800DA4C4(1, 6, 7);   /* TEV color operation */
    fn_800DA1E8(1, 2, 1);   /* Z-buffer mode */
    fn_800D9ED8(0);   /* Fog disabled */

    /* Allocate display list buffer (512KB) */
    if (!fn_800DAF60((void*)COL_GFX_HANDLE, 0x80000)) {
        fn_800C8710(lbl_80272050);
        return NULL;
    }

    /* Get triangle data */
    triBase = (GSColTriangle*)wzx->vertexData;
    triCount = wzx->triangleCount;

    for (i = 0; i < triCount; i++) {
        GSColTriangle* tri = &triBase[i];

        /* Skip disabled triangles (flag bit 0 clear) */
        if (!(tri->flags & 1)) {
            continue;
        }

        /* Build inverse and forward transforms */
        GScolsys2_BuildInverseTransform(invTransform, i);
        GScolsys2_BuildTransform(transform, i);

        /* Draw meshGroupA: walkable floor surfaces (white) */
        if (tri->meshGroupA != NULL) {
            GScolsys2_DrawTriGroup(tri->meshGroupA, invTransform);
        }

        /* Draw meshGroupB: wall collision edges */
        if (tri->meshGroupB != NULL) {
            u32 color = lbl_8047CEB8;  /* 0xFFFFFFFF white */
            u32 subTriCount;
            u8* subVerts;
            u32 j;
            Vec3f xformed[3];

            subVerts = (u8*)((GSColSubMesh*)tri->meshGroupB)->vertexData;
            subTriCount = ((GSColSubMesh*)tri->meshGroupB)->triangleCount;

            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D6A00(1);  /* line strip primitive */

                for (v = 0; v < 3; v++) {
                    u32 nextV = v + 1;
                    if (nextV >= 3) nextV = 0;

                    fn_800D67BC(2);  /* 2 vertices per line segment */

                    /* Current vertex */
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xformed[0]);
                    fn_800D6680(xformed[0].x, xformed[0].y, xformed[0].z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);

                    /* Next vertex */
                    fn_800A37CC(invTransform, subVerts + nextV * 0x0C, &xformed[1]);
                    fn_800D6680(xformed[1].x, xformed[1].y, xformed[1].z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);

                    fn_800D6728();
                }

                subVerts += 0x34;
            }
        }

        /* Draw meshGroupC: wall surfaces (pink) */
        if (tri->meshGroupC != NULL) {
            u32 color = lbl_8047CEBC;  /* 0xFF00FFC0 */
            u8* subVerts = (u8*)((GSColSubMesh*)tri->meshGroupC)->vertexData;
            u32 subTriCount = ((GSColSubMesh*)tri->meshGroupC)->triangleCount;
            u32 j;

            fn_800D6A00(3);  /* triangles */
            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D67BC(3);
                for (v = 0; v < 3; v++) {
                    Vec3f xf;
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                }
                fn_800D6728();
                subVerts += 0x34;
            }
        }

        /* Draw meshGroupD: slope surfaces (yellow) */
        if (tri->meshGroupD != NULL) {
            u32 color = lbl_8047CEC0;  /* 0xFFFF00C0 */
            u8* subVerts = (u8*)((GSColSubMesh*)tri->meshGroupD)->vertexData;
            u32 subTriCount = ((GSColSubMesh*)tri->meshGroupD)->triangleCount;
            u32 j;

            fn_800D6A00(3);
            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D67BC(3);
                for (v = 0; v < 3; v++) {
                    Vec3f xf;
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                }
                fn_800D6728();
                subVerts += 0x34;
            }
        }

        /* Draw meshGroupE: ramp surfaces (same as walkable) */
        if (tri->meshGroupE != NULL) {
            GScolsys2_DrawTriGroup(tri->meshGroupE, invTransform);
        }

        /* Draw meshGroupF: boundary surfaces (cyan) */
        if (tri->meshGroupF != NULL) {
            u32 color = lbl_8047CEC4;  /* 0x00FFFFC0 */
            u8* subVerts = (u8*)((GSColSubMesh*)tri->meshGroupF)->vertexData;
            u32 subTriCount = ((GSColSubMesh*)tri->meshGroupF)->triangleCount;
            u32 j;

            fn_800D6A00(3);
            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D67BC(3);
                for (v = 0; v < 3; v++) {
                    Vec3f xf;
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                }
                fn_800D6728();
                subVerts += 0x30;  /* boundary uses 0x30 stride (no color byte) */
            }
        }
    }

    /* End display list recording */
    fn_800DADB4();

    return (void*)1;  /* success */
}

/* ===================================================================
 * fn_8010D8D4 -- GScolsys2_DrawActive
 *
 * Draws collision meshes for the active layer with per-triangle
 * visibility checks. Similar to GScolsys2_Draw but:
 *   1. Checks per-triangle state in the active layer entry
 *   2. Only draws triangles where:
 *      - The layer entry's flag bit 0 is CLEAR (not yet processed)
 *      - The WZX triangle's flag bit 0 is SET (active)
 *   3. After first pass, caches the display list via GScolsys2_Draw
 *   4. On subsequent frames, replays the cached display list
 *
 * The per-triangle state is at:
 *   layer_base + 4 + triIndex * 0x28 + 0x24  (flags halfword)
 *
 * Each frame advances through: layer entry (0x28 stride),
 * WZX triangle (0x40 stride) simultaneously.
 *
 * At the end, if no display list exists yet, calls GScolsys2_Draw
 * to build one, then replays it via fn_800DAD10 + fn_800D30AC.
 * =================================================================== */
void GScolsys2_DrawActive(void)
{
    GSColMeshHeader* wzx;
    GSColTriangle* triBase;
    u8* layerEntry;
    u32 triCount;
    u32 i;
    f32 invTransform[12];
    f32 transform[12];

    wzx = (GSColMeshHeader*)GScolsys2_GetWZXData();
    if (wzx == NULL) {
        return;
    }

    /* Set up GX render state */
    fn_800DA028(1);
    fn_800D7820((void*)(((u8*)COL_STATE)[0x3708]));  /* render handle */
    fn_800D88DC(1);
    fn_800D888C(6);
    fn_800DA4C4(1, 6, 7);
    fn_800DA1E8(1, 2, 1);
    fn_800D9ED8(0);

    triBase = (GSColTriangle*)wzx->vertexData;
    triCount = wzx->triangleCount;

    /* Compute layer entry base */
    layerEntry = (u8*)COL_STATE + COL_LAYER_IDX * GSCOLSYS_LAYER_SIZE + 4;

    for (i = 0; i < triCount; i++) {
        GSColTriangle* tri = &triBase[i];
        u16 layerFlags = *(u16*)(layerEntry + 0x24);
        u16 triFlags;

        /* Skip if layer entry already processed (bit 0 set) */
        if (layerFlags & 1) {
            goto next;
        }

        /* Skip if WZX triangle is inactive (bit 0 clear) */
        triFlags = tri->flags;
        if (!(triFlags & 1)) {
            goto next;
        }

        /* Build transforms */
        GScolsys2_BuildInverseTransform(invTransform, i);
        GScolsys2_BuildTransform(transform, i);

        /* Draw each mesh group (same structure as GScolsys2_Draw) */
        if (tri->meshGroupA != NULL) {
            GScolsys2_DrawTriGroup(tri->meshGroupA, invTransform);
        }

        if (tri->meshGroupB != NULL) {
            u32 color = lbl_8047CEB8;
            u8* subVerts = (u8*)((GSColSubMesh*)tri->meshGroupB)->vertexData;
            u32 subTriCount = ((GSColSubMesh*)tri->meshGroupB)->triangleCount;
            u32 j;
            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D6A00(1);
                for (v = 0; v < 3; v++) {
                    u32 nextV = v + 1;
                    Vec3f xf;
                    if (nextV >= 3) nextV = 0;
                    fn_800D67BC(2);
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                    fn_800A37CC(invTransform, subVerts + nextV * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                    fn_800D6728();
                }
                subVerts += 0x34;
            }
        }

        if (tri->meshGroupC != NULL) {
            u32 color = lbl_8047CEBC;
            u8* subVerts = (u8*)((GSColSubMesh*)tri->meshGroupC)->vertexData;
            u32 subTriCount = ((GSColSubMesh*)tri->meshGroupC)->triangleCount;
            u32 j;
            fn_800D6A00(3);
            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D67BC(3);
                for (v = 0; v < 3; v++) {
                    Vec3f xf;
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                }
                fn_800D6728();
                subVerts += 0x34;
            }
        }

        if (tri->meshGroupD != NULL) {
            u32 color = lbl_8047CEC0;
            u8* subVerts = (u8*)((GSColSubMesh*)tri->meshGroupD)->vertexData;
            u32 subTriCount = ((GSColSubMesh*)tri->meshGroupD)->triangleCount;
            u32 j;
            fn_800D6A00(3);
            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D67BC(3);
                for (v = 0; v < 3; v++) {
                    Vec3f xf;
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                }
                fn_800D6728();
                subVerts += 0x34;
            }
        }

        if (tri->meshGroupE != NULL) {
            GScolsys2_DrawTriGroup(tri->meshGroupE, invTransform);
        }

        if (tri->meshGroupF != NULL) {
            u32 color = lbl_8047CEC4;
            u8* subVerts = (u8*)((GSColSubMesh*)tri->meshGroupF)->vertexData;
            u32 subTriCount = ((GSColSubMesh*)tri->meshGroupF)->triangleCount;
            u32 j;
            fn_800D6A00(3);
            for (j = 0; j < subTriCount; j++) {
                u32 v;
                fn_800D67BC(3);
                for (v = 0; v < 3; v++) {
                    Vec3f xf;
                    fn_800A37CC(invTransform, subVerts + v * 0x0C, &xf);
                    fn_800D6680(xf.x, xf.y, xf.z);
                    fn_800D5CB8(0, (u8)(color >> 24), (u8)(color >> 16),
                                (u8)(color >> 8), (u8)color);
                }
                fn_800D6728();
                subVerts += 0x30;
            }
        }

    next:
        layerEntry += GSCOLSYS_TRI_ENTRY_SIZE;
    }

    /* Build and cache display list on first call */
    if (COL_DISPLIST == NULL) {
        COL_DISPLIST = GScolsys2_Draw();
    }

    /* Replay cached display list */
    if (COL_DISPLIST != NULL) {
        fn_800DAD10(COL_DISPLIST);
        fn_800D30AC();
    }
}

/* ===================================================================
 * fn_8010DE00 -- GScolsys2_FindNearestGround
 *
 * Finds the nearest ground triangle below a given position.
 *
 * Performs a vertical ray cast using fn_8010E138 (internal ground
 * ray cast routine). The ray cast returns an array of hit results,
 * each containing the Y-intersection height at offset +0x00 and
 * floor type/attribute bytes.
 *
 * This function then finds the hit whose Y-height is closest to
 * (but below) the query position's Y coordinate. The absolute
 * distance |queryY - hitY| is computed and the minimum is selected.
 *
 * Result structure (per hit, 0x0C bytes):
 *   0x00: u8  floorType
 *   0x01: u8  attribute
 *   0x02-0x07: padding
 *   0x08: f32 hitY  (Y-coordinate of ground intersection)
 *
 * @param pos         Query position (Vec3f*)
 * @param outFloorType Receives the floor type byte of nearest hit
 * @param outAttribute Receives the attribute byte of nearest hit
 * @return            1 if ground found, 0 if no intersection
 * =================================================================== */
s32 GScolsys2_FindNearestGround(Vec3f* pos, u8* outFloorType, u8* outAttribute)
{
    u8 hitBuffer[0x6C];  /* Space for up to 9 hit results (0x0C each) */
    s32 hitCount;
    f32 queryY;
    f32 bestDist;
    s32 bestIndex;
    s32 i;

    hitCount = fn_8010E138(pos, hitBuffer);

    if (hitCount <= 0) {
        return 0;
    }

    queryY = pos->y;

    /* Find nearest hit by absolute Y distance */
    bestDist = queryY - *(f32*)(hitBuffer + 0x08);
    if (bestDist <= 0.0f) {
        bestDist = -bestDist;
    }
    bestIndex = 0;

    if (hitCount > 1) {
        for (i = 1; i < hitCount; i++) {
            f32 dist = queryY - *(f32*)(hitBuffer + 0x08 + i * 0x0C);
            if (dist <= 0.0f) {
                dist = -dist;
            }
            if (bestDist > dist) {
                bestIndex = i;
                bestDist = dist;
            }
        }
    }

    /* Extract floor type and attribute from best hit */
    *outFloorType = hitBuffer[0x00 + bestIndex * 0x0C];
    *outAttribute = hitBuffer[0x01 + bestIndex * 0x0C];

    return 1;
}

/* ===================================================================
 * fn_8010DEF0 -- GScolsys2_TriangleBoundsCheck
 *
 * Tests whether a 2D point (X, Z) falls within a collision triangle's
 * bounding box and passes all three edge cross-product tests.
 *
 * Algorithm:
 *   1. Compute AABB of the triangle (min/max of all 3 vertex X and Z)
 *   2. If the query point (from the Vec3f at r4) is outside the AABB,
 *      return 0 (early reject)
 *   3. For each edge of the triangle, compute the 2D cross product:
 *        cross = (edgeZ * dx) - (edgeX * dz)
 *      where dx = queryX - v.x, dz = queryZ - v.z
 *      If cross > 0, the point is on the wrong side -> return 0
 *   4. If all 3 edge tests pass, return 1 (point is inside)
 *
 * The triangle vertices are passed as r5 (pointer to 3 Vec3f values).
 * The query point is r4 (pointer to Vec3f with X at +0x00, Z at +0x08).
 *
 * Note: Only X and Z coordinates are tested (this is a top-down 2D test).
 * Y is ignored for the bounds check -- height is handled separately.
 * =================================================================== */
s32 GScolsys2_TriangleBoundsCheck(Vec3f* queryPoint, f32* triVerts)
{
    f32 minX, maxX, minZ, maxZ;
    f32 queryX, queryZ;
    f32 v0x, v0z, v1x, v1z, v2x, v2z;
    s32 nextVert;
    f32* nextVertPtr;
    f32 edgeX, edgeZ, dx, dz, cross;

    /* Initialize min/max with first vertex */
    v0x = triVerts[0];   /* v0.x */
    v0z = triVerts[2];   /* v0.z */
    minX = maxX = v0x;
    minZ = maxZ = v0z;

    /* Expand with second vertex */
    v1x = triVerts[3];   /* v1.x */
    v1z = triVerts[5];   /* v1.z */
    if (minX > v1x) minX = v1x;
    if (minZ > v1z) minZ = v1z;
    if (maxX < v1x) maxX = v1x;
    if (maxZ < v1z) maxZ = v1z;

    /* Expand with third vertex */
    v2x = triVerts[6];   /* v2.x */
    v2z = triVerts[8];   /* v2.z */
    if (minX > v2x) minX = v2x;
    if (minZ > v2z) minZ = v2z;
    if (maxX < v2x) maxX = v2x;
    if (maxZ < v2z) maxZ = v2z;

    /* Get query point XZ */
    queryX = queryPoint->x;
    queryZ = queryPoint->z;

    /* AABB rejection test */
    if (minX > queryX || minZ > queryZ || maxX < queryX || maxZ < queryZ) {
        return 0;
    }

    /* Edge 0 -> 1 cross product test */
    nextVert = 1;
    if (nextVert >= 3) nextVert = 0;
    nextVertPtr = triVerts + nextVert * 3;
    edgeZ = nextVertPtr[2] - triVerts[2];  /* v1.z - v0.z */
    edgeX = nextVertPtr[0] - triVerts[0];  /* v1.x - v0.x */
    dx = queryX - triVerts[0];             /* query.x - v0.x */
    dz = queryZ - triVerts[2];             /* query.z - v0.z */
    cross = edgeX * dz - edgeZ * dx;
    if (cross > 0.0f) {
        return 0;
    }

    /* Edge 1 -> 2 cross product test */
    nextVert = 2;
    if (nextVert >= 3) nextVert = 0;
    nextVertPtr = triVerts + nextVert * 3;
    edgeZ = nextVertPtr[2] - triVerts[5];  /* v2.z - v1.z */
    edgeX = nextVertPtr[0] - triVerts[3];  /* v2.x - v1.x */
    dx = queryX - triVerts[3];
    dz = queryZ - triVerts[5];
    cross = edgeX * dz - edgeZ * dx;
    if (cross > 0.0f) {
        return 0;
    }

    /* Edge 2 -> 0 cross product test */
    nextVert = 3;
    if (nextVert >= 3) nextVert = 0;
    nextVertPtr = triVerts + nextVert * 3;
    edgeZ = nextVertPtr[2] - triVerts[8];  /* v0.z - v2.z */
    edgeX = nextVertPtr[0] - triVerts[6];  /* v0.x - v2.x */
    dx = queryX - triVerts[6];
    dz = queryZ - triVerts[8];
    cross = edgeX * dz - edgeZ * dx;
    if (cross > 0.0f) {
        return 0;
    }

    return 1;  /* Point is inside triangle */
}

/* ===================================================================
 * Generated: 1 pattern-matched + 31 stubs
 * Range: 0x8010C220 - 0x8010E138
 * =================================================================== */

/* 0x8010C220 | 0x4 | void_stub */
void fn_8010C220(void) {
}

/* 0x8010C224 | 0x140 */
void fn_8010C224(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    tmp = *(u16*)&lbl_8047AD50;
    *(u32*)&lbl_8047AD48 = r3;
    if (tmp != 0) goto L_8010C264;
    r3 = r3 << 4;
    ((void(*)(void))fn_800E3534)();
    *(u16*)&lbl_8047AD50 = r3;
    r3 = r3 & 0xFFFF;
    ((void(*)(void))fn_800E27B0)();
    *(u32*)&lbl_8047AD4C = r3;
L_8010C264:
    tmp = *(u32*)&lbl_8047AD48;
    r4 = 0x0;
    r3 = *(u32*)&lbl_8047AD4C;
    r5 = tmp << 4;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = *(u16*)&lbl_8047AD58;
    if (tmp != 0) goto L_8010C2A0;
    tmp = *(u32*)&lbl_8047AD48;
    r3 = tmp << 3;
    ((void(*)(void))fn_800E3534)();
    *(u16*)&lbl_8047AD58 = r3;
    r3 = r3 & 0xFFFF;
    ((void(*)(void))fn_800E27B0)();
    *(u32*)&lbl_8047AD54 = r3;
L_8010C2A0:
    tmp = *(u32*)&lbl_8047AD48;
    r4 = 0x0;
    r3 = *(u32*)&lbl_8047AD54;
    r5 = tmp << 3;
    memset((void*)r3, (int)r4, (u32)r5);
    r31 = 0x0;
    r29 = 0x0;
    r30 = r31;
    goto L_8010C33C;
L_8010C2C4:
    r3 = *(u32*)&lbl_8047AD54;
    tmp = r31 + 0x2;
    tmp = *(u16*)(r3 + tmp);
    if (tmp != 0) goto L_8010C308;
    r3 = 0x6ec0;
    r4 = 0x20;
    ((void(*)(void))fn_800E2C04)();
    r4 = *(u32*)&lbl_8047AD54;
    tmp = r31 + 0x2;
    *(u16*)(r4 + tmp) = r3;
    r3 = *(u32*)&lbl_8047AD54;
    r3 = *(u16*)(r3 + tmp);
    ((void(*)(void))fn_800E27B0)();
    r4 = *(u32*)&lbl_8047AD54;
    tmp = r31 + 0x4;
    *(u32*)(r4 + tmp) = r3;
L_8010C308:
    r3 = *(u32*)&lbl_8047AD54;
    tmp = r31 + 0x4;
    r4 = 0x0;
    r5 = 0x6ec0;
    r3 = *(u32*)(r3 + tmp);
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = *(u32*)&lbl_8047AD4C;
    r4 = (s8)r29;
    tmp = r30 + 0x7;
    r31 = r31 + 0x8;
    *(u8*)(r3 + tmp) = r4;
    r30 = r30 + 0x10;
    r29 = r29 + 0x1;
L_8010C33C:
    tmp = *(u32*)&lbl_8047AD48;
    if ((s32)r29 < (s32)tmp) goto L_8010C2C4;
    return;
}

/* 0x8010C364 | 36 bytes | call_return_const */
u32 fn_8010C364(void) {
    fn_800EF5A4();
    return 1;
}

/* 0x74 | fn_8010C388 | generic */
u32 fn_8010C388(void) {
    /* refs: lbl_80272000, lbl_8035B4E8, lbl_80478E70, lbl_80478E74 */
    fn_800DD970("");
    return 0;
}

/* 0x70 | fn_8010C3FC | generic */
u32 fn_8010C3FC(void) {
    /* refs: lbl_80272000, lbl_8035B4E8, lbl_80478E70, lbl_80478E74 */
    fn_800DD970("");
    return 0;
}

/* 0x8010C46C | 0x34 */
void fn_8010C46C(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    tmp = *(u32*)&lbl_80478B38;
    r4 = r3 & 0xFFFF;
    if (r4 < tmp) goto L_8010C484;
    r3 = 0x0;
    return;
L_8010C484:
    tmp = r3 & 0xFFFF;
    r4 = (u32)&lbl_8035B500;
    r5 = tmp * 0x2c;
    tmp = (u32)&lbl_8035B500;
    r4 = tmp + r5;
    r3 = *(u16*)((u8*)r4 + 0x2);
    return;
}

/* 0x8010C4A0 | 0x34 */
void fn_8010C4A0(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    tmp = *(u32*)&lbl_80478B38;
    r4 = r3 & 0xFFFF;
    if (r4 < tmp) goto L_8010C4B8;
    r3 = 0x0;
    return;
L_8010C4B8:
    tmp = r3 & 0xFFFF;
    r4 = (u32)&lbl_8035B500;
    r5 = tmp * 0x2c;
    tmp = (u32)&lbl_8035B500;
    r4 = tmp + r5;
    r3 = *(u8*)((u8*)r4 + 0x0);
    return;
}

/* 0x8010C4D4 | 0x34 */
void fn_8010C4D4(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    tmp = *(u32*)&lbl_80478B38;
    r4 = r3 & 0xFFFF;
    if (r4 < tmp) goto L_8010C4EC;
    r3 = 0x0;
    return;
L_8010C4EC:
    tmp = r3 & 0xFFFF;
    r4 = (u32)&lbl_8035B500;
    r5 = tmp * 0x2c;
    tmp = (u32)&lbl_8035B500;
    r4 = tmp + r5;
    r3 = *(u32*)((u8*)r4 + 0x4);
    return;
}

/* 0x44 | fn_8010C508 | leaf_2d_array_lookup */
u16 fn_8010C508(u16 idx1, u16 idx2) {
    void* entry;
    if (idx1 >= lbl_80478B38) { return 0; }
    if (idx2 >= 0x12) { return 0; }
    entry = (u8*)lbl_8035B500 + idx1 * 0x2C + idx2 * 0x2;
    return *(u16*)((u8*)entry + 0x8);
}

/* 0x8010C54C | 0x104 */
void fn_8010C54C(void) {
    extern void fn_8010C508();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r26 = r3;
    r31 = 0x0;
    r29 = 0x0;
    *(u8*)(sp + 0x8) = r4;
    goto L_8010C628;
L_8010C570:
    tmp = r29 & 0xFFFF;
    if (tmp == 9) goto L_8010C624;
    tmp = *(u8*)(sp + 0x8);
    if (tmp != 1) goto L_8010C5D8;
    r3 = r26 & 0xFFFF;
    r4 = r29 & 0xFFFF;
    fn_8010C508();
    tmp = r3 & 0xFFFF;
    r28 = r3;
    r30 = r28;
    if (tmp != 0x43) goto L_8010C5B0;
    goto L_8010C624;
L_8010C5B0:
    tmp = r30 & 0xFFFF;
    if (tmp != 0x42) goto L_8010C5C4;
    goto L_8010C624;
L_8010C5C4:
    tmp = r30 & 0xFFFF;
    if (tmp != 0x41) goto L_8010C624;
    r31 = r31 + 0xa;
    goto L_8010C624;
L_8010C5D8:
    r3 = r29 & 0xFFFF;
    r4 = r26 & 0xFFFF;
    fn_8010C508();
    tmp = r3 & 0xFFFF;
    r27 = r3;
    r30 = r27;
    if (tmp != 0x43) goto L_8010C600;
    r31 = r31 + 0xa;
    goto L_8010C624;
L_8010C600:
    tmp = r30 & 0xFFFF;
    if (tmp != 0x42) goto L_8010C614;
    r31 = r31 + 0xa;
    goto L_8010C624;
L_8010C614:
    tmp = r30 & 0xFFFF;
    if (tmp != 0x41) goto L_8010C624;
L_8010C624:
    r29 = r29 + 0x1;
L_8010C628:
    tmp = *(u32*)&lbl_80478B38;
    r3 = r29 & 0xFFFF;
    if (r3 < tmp) goto L_8010C570;
    r3 = r31;
    return;
}

/* 0x8010C650 | 0xFC */
void fn_8010C650(void) {
    extern void fn_8010C508();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r25 = r4;
    r30 = 0x0;
    r29 = 0x0;
    *(u16*)(sp + 0x8) = r3;
    r31 = 0x0;
    *(u16*)(sp + 0xA) = r5;
    goto L_8010C6DC;
L_8010C67C:
    tmp = r31 & 0xFFFF;
    if (tmp == 9) goto L_8010C6D8;
    r3 = *(u16*)(sp + 0x8);
    r4 = *(u16*)(r25 + tmp);
    fn_8010C508();
    tmp = r3 & 0xFFFF;
    r26 = r3;
    r28 = r26;
    if (tmp != 0x43) goto L_8010C6B4;
    r3 = 0x43;
    goto L_8010C738;
L_8010C6B4:
    tmp = r28 & 0xFFFF;
    if (tmp != 0x41) goto L_8010C6C8;
    r30 = r30 + 0x1;
    goto L_8010C6D8;
L_8010C6C8:
    tmp = r28 & 0xFFFF;
    if (tmp != 0x42) goto L_8010C6D8;
    r29 = r29 + 0x1;
L_8010C6D8:
    r31 = r31 + 0x1;
L_8010C6DC:
    tmp = *(u16*)(sp + 0xA);
    r3 = r31 & 0xFFFF;
    if (r3 < tmp) goto L_8010C67C;
    tmp = r30 & 0xFFFF;
    if (r3 != tmp) goto L_8010C704;
    tmp = r29 & 0xFFFF;
    if (r3 != tmp) goto L_8010C704;
    r3 = 0x3f;
    goto L_8010C738;
L_8010C704:
    r3 = r30 & 0xFFFF;
    tmp = r29 & 0xFFFF;
    if (r3 != tmp) goto L_8010C71C;
    r3 = 0x3f;
    goto L_8010C738;
L_8010C71C:
    r3 = r30 & 0xFFFF;
    tmp = r29 & 0xFFFF;
    if (r3 <= tmp) goto L_8010C734;
    r3 = 0x41;
    goto L_8010C738;
L_8010C734:
    r3 = 0x42;
L_8010C738:
    return;
}

/* 0x8010C74C | 0x30 */
void fn_8010C74C(void) {
    extern void fn_8010C508();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    *(u16*)(sp + 0x8) = r3;
    *(u16*)(sp + 0xA) = r4;
    r3 = *(u16*)(sp + 0x8);
    r4 = *(u16*)(sp + 0xA);
    fn_8010C508();
    return;
}

/* 0x40 | fn_8010C77C | dot_product_3way */
f32 fn_8010C77C(f32* a, f32* b, f32* c) {
    f32 dx = c[1] - b[1];
    f32 dy = c[0] - b[0];
    f32 dz = c[2] - b[2];
    return a[1] * dx + a[0] * dy + a[2] * dz;
}

/* 0x8010C7BC | 0x88 */
void fn_8010C7BC(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r5 = (u32)&lbl_80404C68;
    r6 = (u32)&lbl_80404C68;
    r5 = *(u32*)((u8*)r6 + 0x0);
    if (r5 != 0) goto L_8010C7D8;
    r3 = 0x1;
    goto L_8010C814;
L_8010C7D8:
    if ((s32)r3 < 0) goto L_8010C7EC;
    tmp = *(u32*)((u8*)r5 + 0x4);
    if (r3 < tmp) goto L_8010C7F4;
L_8010C7EC:
    r3 = 0x2;
    goto L_8010C814;
L_8010C7F4:
    r5 = *(u32*)((u8*)r6 + 0x3704);
    tmp = r3 * 0x28;
    r3 = 0x0;
    r5 = r5 * 0xdc0;
    r5 = r6 + r5;
    r5 = r5 + 0x4;
    r5 = r5 + tmp;
    r7 = r5;
L_8010C814:
    if ((s32)r3 != (s32)0x0) return;
    tmp = *(u16*)((u8*)r7 + 0x24);
    tmp = tmp & 0x1;
    if ((s32)r3 == 0) goto L_8010C834;
    tmp = 0x0;
    *(u32*)((u8*)r4 + 0x0) = tmp;
    goto L_8010C83C;
L_8010C834:
    tmp = 0x1;
    *(u32*)((u8*)r4 + 0x0) = tmp;
L_8010C83C:
    r3 = 0x0;
    return;
}

/* 0x8010C844 | 0x8C */
void fn_8010C844(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r5 = (u32)&lbl_80404C68;
    r6 = (u32)&lbl_80404C68;
    r5 = *(u32*)((u8*)r6 + 0x0);
    if (r5 != 0) goto L_8010C860;
    r3 = 0x1;
    goto L_8010C89C;
L_8010C860:
    if ((s32)r3 < 0) goto L_8010C874;
    tmp = *(u32*)((u8*)r5 + 0x4);
    if (r3 < tmp) goto L_8010C87C;
L_8010C874:
    r3 = 0x2;
    goto L_8010C89C;
L_8010C87C:
    r5 = *(u32*)((u8*)r6 + 0x3704);
    tmp = r3 * 0x28;
    r3 = 0x0;
    r5 = r5 * 0xdc0;
    r5 = r6 + r5;
    r5 = r5 + 0x4;
    r5 = r5 + tmp;
    r7 = r5;
L_8010C89C:
    if ((s32)r3 != (s32)0x0) return;
    if ((s32)r4 == 0) goto L_8010C8BC;
    tmp = *(u16*)((u8*)r7 + 0x24);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r7 + 0x24) = tmp;
    goto L_8010C8C8;
L_8010C8BC:
    tmp = *(u16*)((u8*)r7 + 0x24);
    tmp = tmp | 0x1;
    *(u16*)((u8*)r7 + 0x24) = tmp;
L_8010C8C8:
    r3 = 0x0;
    return;
}

/* 0x8010C8D0 | 0x160 */
void fn_8010C8D0(void) {
    u8 sp[0xE0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r5 = (u32)&lbl_80404C68;
    r6 = (u32)&lbl_80404C68;
    r30 = r3;
    r5 = *(u32*)((u8*)r6 + 0x0);
    if (r5 != 0) goto L_8010C900;
    r3 = 0x0;
    goto L_8010CA1C;
L_8010C900:
    tmp = *(u32*)((u8*)r5 + 0x4);
    if (r4 < tmp) goto L_8010C914;
    r3 = 0x0;
    goto L_8010CA1C;
L_8010C914:
    r5 = *(u32*)((u8*)r6 + 0x3704);
    tmp = r4 * 0x28;
    r4 = r5 * 0xdc0;
    r4 = r6 + r4;
    r27 = r4 + 0x4;
    r27 = r27 + tmp;
    ((void(*)(void))fn_800A2D38)();
    tmp = *(u16*)((u8*)r27 + 0x24);
    tmp = tmp & 0x1;
    if (r4 == tmp) goto L_8010CA18;
    r3 = (u32)&lbl_80272044;
    r4 = (u32)&lbl_80272020;
    r6 = (u32)&lbl_80272044;
    r31 = r27 + 0xc;
    r27 = *(u32*)&lbl_80272020;
    r3 = (u32)sp + 0x44;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r28 = *(u32*)((u8*)r4 + 0x4);
    r29 = *(u32*)((u8*)r4 + 0x8);
    r12 = *(u32*)((u8*)r4 + 0xC);
    r11 = *(u32*)((u8*)r4 + 0x10);
    r10 = *(u32*)((u8*)r4 + 0x14);
    r9 = *(u32*)((u8*)r4 + 0x18);
    r8 = *(u32*)((u8*)r4 + 0x1C);
    r7 = *(u32*)((u8*)r4 + 0x20);
    r4 = *(u32*)((u8*)r6 + 0x4);
    tmp = *(u32*)((u8*)r6 + 0x8);
    *(u32*)(sp + 0x10) = tmp;
    ((void(*)(void))fn_800A2D38)();
    r27 = 0x0;
    r28 = (u32)sp + 0x8;
L_8010C9BC:
    tmp = *(u32*)((u8*)r28 + 0x0);
    r4 = (u32)sp + 0x74;
    r3 = (u32)sp + 0x14;
    r5 = tmp * 0xc;
    tmp = tmp << 2;
    f1 = *(f32*)(r31 + tmp);
    r4 = r4 + r5;
    ((void(*)(void))fn_800A3244)();
    r4 = (u32)sp + 0x44;
    r3 = (u32)sp + 0x14;
    r5 = r4;
    ((void(*)(void))fn_800A2D98)();
    r27 = r27 + 0x1;
    r28 = r28 + 0x4;
    if ((s32)r27 < 3) goto L_8010C9BC;
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x98;
    ((void(*)(void))fn_800A2D64)();
    r3 = r30;
    r5 = r30;
    r4 = (u32)sp + 0x98;
    ((void(*)(void))fn_800A2D98)();
L_8010CA18:
    r3 = 0x1;
L_8010CA1C:
    return;
}

/* 0x8010CA30 | 0x190 */
void fn_8010CA30(void) {
    u8 sp[0xE0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r5 = (u32)&lbl_80404C68;
    r6 = (u32)&lbl_80404C68;
    r29 = r3;
    r5 = *(u32*)((u8*)r6 + 0x0);
    if (r5 != 0) goto L_8010CA60;
    r3 = 0x0;
    goto L_8010CBAC;
L_8010CA60:
    tmp = *(u32*)((u8*)r5 + 0x4);
    if (r4 < tmp) goto L_8010CA74;
    r3 = 0x0;
    goto L_8010CBAC;
L_8010CA74:
    r5 = *(u32*)((u8*)r6 + 0x3704);
    tmp = r4 * 0x28;
    r4 = r5 * 0xdc0;
    r4 = r6 + r4;
    r30 = r4 + 0x4;
    r30 = r30 + tmp;
    ((void(*)(void))fn_800A2D38)();
    tmp = *(u16*)((u8*)r30 + 0x24);
    tmp = tmp & 0x1;
    if (r4 == tmp) goto L_8010CBA8;
    f1 = *(f32*)((u8*)r30 + 0x18);
    r3 = r29;
    f2 = *(f32*)((u8*)r30 + 0x1C);
    r4 = r29;
    f3 = *(f32*)((u8*)r30 + 0x20);
    ((void(*)(void))fn_800A335C)();
    r4 = (u32)&lbl_80272020;
    r26 = *(u32*)&lbl_80272020;
    r3 = (u32)&lbl_80272044;
    r31 = r30 + 0xc;
    r6 = (u32)&lbl_80272044;
    r27 = *(u32*)((u8*)r4 + 0x4);
    r28 = *(u32*)((u8*)r4 + 0x8);
    r3 = (u32)sp + 0x44;
    r12 = *(u32*)((u8*)r4 + 0xC);
    r11 = *(u32*)((u8*)r4 + 0x10);
    r10 = *(u32*)((u8*)r4 + 0x14);
    r9 = *(u32*)((u8*)r4 + 0x18);
    r8 = *(u32*)((u8*)r4 + 0x1C);
    r7 = *(u32*)((u8*)r4 + 0x20);
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    tmp = *(u32*)((u8*)r6 + 0x8);
    *(u32*)(sp + 0x10) = tmp;
    ((void(*)(void))fn_800A2D38)();
    r26 = 0x0;
    r27 = (u32)sp + 0x8;
L_8010CB34:
    tmp = *(u32*)((u8*)r27 + 0x0);
    r4 = (u32)sp + 0x74;
    r3 = (u32)sp + 0x14;
    r5 = tmp * 0xc;
    tmp = tmp << 2;
    f1 = *(f32*)(r31 + tmp);
    r4 = r4 + r5;
    ((void(*)(void))fn_800A3244)();
    r4 = (u32)sp + 0x44;
    r3 = (u32)sp + 0x14;
    r5 = r4;
    ((void(*)(void))fn_800A2D98)();
    r26 = r26 + 0x1;
    r27 = r27 + 0x4;
    if ((s32)r26 < 3) goto L_8010CB34;
    r3 = (u32)sp + 0x44;
    r4 = (u32)sp + 0x98;
    ((void(*)(void))fn_800A2D64)();
    r3 = r29;
    r5 = r29;
    r4 = (u32)sp + 0x98;
    ((void(*)(void))fn_800A2D98)();
    f1 = *(f32*)((u8*)r30 + 0x0);
    r3 = r29;
    f2 = *(f32*)((u8*)r30 + 0x4);
    r4 = r29;
    f3 = *(f32*)((u8*)r30 + 0x8);
    ((void(*)(void))fn_800A32E8)();
L_8010CBA8:
    r3 = 0x1;
L_8010CBAC:
    return;
}

/* 0x8010CBC0 | 16 bytes | global_getter */
u32 fn_8010CBC0(void) {
    return *(u32*)((u8*)&lbl_80404C68 + 0x0);
}

/* 0x8010CBD0 | 0x34 */
void fn_8010CBD0(void) {
    u32 tmp = 0;
    u32 r3 = 0;

    r3 = (u32)&lbl_80404C68;
    r3 = (u32)&lbl_80404C68;
    tmp = *(u32*)((u8*)r3 + 0x3704);
    if ((s32)tmp < 0) goto L_8010CBEC;
    if ((s32)tmp < 4) goto L_8010CBF4;
L_8010CBEC:
    r3 = 0x0;
    return;
L_8010CBF4:
    tmp = tmp * 0xdc0;
    r3 = r3 + tmp;
    r3 = r3 + 0x4;
    return;
}

/* 0x50 | fn_8010CC04 | global_cond_call */
u32 fn_8010CC04(void) {
    /* uses lbl_80404C68 */
    if (0 /* field check */) { return 1; }
    fn_800DACC0(0 /* TODO */);
    return 1;
}

/* 0x8010CC54 | 0x118 */
void fn_8010CC54(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = (u32)&lbl_80404C68;
    r3 = (u32)&lbl_80404C68;
    tmp = *(u32*)((u8*)r3 + 0x3704);
    if ((s32)tmp < 0) goto L_8010CC70;
    if ((s32)tmp < 4) goto L_8010CC78;
L_8010CC70:
    r3 = 0x0;
    goto L_8010CC84;
L_8010CC78:
    tmp = tmp * 0xdc0;
    r3 = r3 + tmp;
    r3 = r3 + 0x4;
L_8010CC84:
    if (r3 == 0) goto L_8010CD5C;
    tmp = 0x3;
    ctr_fn = (void(*)(void))tmp;
L_8010CC94:
    tmp = *(u16*)((u8*)r3 + 0xA10);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA10) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA24);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA24) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA38);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA38) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA4C);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA4C) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA60);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA60) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA74);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA74) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA88);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA88) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA9C);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA9C) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAB0);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAB0) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAC4);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAC4) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAD8);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAD8) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAEC);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAEC) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB00);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB00) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB14);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB14) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB28);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB28) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB3C);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB3C) = tmp;
    r3 = r3 + 0x140;
    if (--ctr != 0) goto L_8010CC94;
L_8010CD5C:
    r3 = (u32)&lbl_80404C68;
    tmp = 0x0;
    *(u32*)&lbl_80404C68 = tmp;
    return;
}

/* 0x8010CD6C | 0x98 */
void fn_8010CD6C(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r3 = (u32)&lbl_80404C68;
    r5 = *(u32*)&lbl_80404C68;
    if ((u32)r5 == (u32)0x0) return;
    tmp = *(u32*)((u8*)r3 + 0x3704);
    r6 = 0x0;
    r7 = *(u32*)((u8*)r5 + 0x0);
    tmp = tmp * 0xdc0;
    r3 = r3 + tmp;
    r8 = r3 + 0x4;
    goto L_8010CDF4;
L_8010CD98:
    r4 = *(u32*)((u8*)r7 + 0x0);
    tmp = 0x0;
    r3 = *(u32*)((u8*)r7 + 0x4);
    r6 = r6 + 0x1;
    *(u32*)((u8*)r8 + 0x0) = r4;
    *(u32*)((u8*)r8 + 0x4) = r3;
    r3 = *(u32*)((u8*)r7 + 0x8);
    *(u32*)((u8*)r8 + 0x8) = r3;
    r4 = *(u32*)((u8*)r7 + 0xC);
    r3 = *(u32*)((u8*)r7 + 0x10);
    *(u32*)((u8*)r8 + 0xC) = r4;
    *(u32*)((u8*)r8 + 0x10) = r3;
    r3 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r8 + 0x14) = r3;
    r4 = *(u32*)((u8*)r7 + 0x18);
    r3 = *(u32*)((u8*)r7 + 0x1C);
    *(u32*)((u8*)r8 + 0x18) = r4;
    *(u32*)((u8*)r8 + 0x1C) = r3;
    r3 = *(u32*)((u8*)r7 + 0x20);
    r7 = r7 + 0x40;
    *(u32*)((u8*)r8 + 0x20) = r3;
    *(u16*)((u8*)r8 + 0x24) = tmp;
    r8 = r8 + 0x28;
L_8010CDF4:
    tmp = *(u32*)((u8*)r5 + 0x4);
    if (r6 < tmp) goto L_8010CD98;
    return;
}

/* 0x8010CE04 | 0x1E0 */
void fn_8010CE04(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    if ((u32)r3 == (u32)0x0) return;
    tmp = *(u32*)((u8*)r3 + 0x0);
    r5 = 0x0;
    tmp = tmp + r3;
    *(u32*)((u8*)r3 + 0x0) = tmp;
    r4 = *(u32*)((u8*)r3 + 0x0);
    goto L_8010CFD4;
L_8010CE24:
    tmp = *(u32*)((u8*)r4 + 0x24);
    if (tmp == 0) goto L_8010CE78;
    tmp = tmp + r3;
    *(u32*)((u8*)r4 + 0x24) = tmp;
    r6 = *(u32*)((u8*)r4 + 0x24);
    tmp = *(u32*)((u8*)r6 + 0x0);
    if (tmp == 0) goto L_8010CE50;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x0) = tmp;
L_8010CE50:
    tmp = *(u32*)((u8*)r6 + 0x8);
    if (tmp == 0) goto L_8010CE64;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x8) = tmp;
L_8010CE64:
    tmp = *(u32*)((u8*)r6 + 0xC);
    if (tmp == 0) goto L_8010CE78;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0xC) = tmp;
L_8010CE78:
    tmp = *(u32*)((u8*)r4 + 0x28);
    if (tmp == 0) goto L_8010CECC;
    tmp = tmp + r3;
    *(u32*)((u8*)r4 + 0x28) = tmp;
    r6 = *(u32*)((u8*)r4 + 0x28);
    tmp = *(u32*)((u8*)r6 + 0x0);
    if (tmp == 0) goto L_8010CEA4;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x0) = tmp;
L_8010CEA4:
    tmp = *(u32*)((u8*)r6 + 0x8);
    if (tmp == 0) goto L_8010CEB8;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x8) = tmp;
L_8010CEB8:
    tmp = *(u32*)((u8*)r6 + 0xC);
    if (tmp == 0) goto L_8010CECC;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0xC) = tmp;
L_8010CECC:
    tmp = *(u32*)((u8*)r4 + 0x2C);
    if (tmp == 0) goto L_8010CF20;
    tmp = tmp + r3;
    *(u32*)((u8*)r4 + 0x2C) = tmp;
    r6 = *(u32*)((u8*)r4 + 0x2C);
    tmp = *(u32*)((u8*)r6 + 0x0);
    if (tmp == 0) goto L_8010CEF8;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x0) = tmp;
L_8010CEF8:
    tmp = *(u32*)((u8*)r6 + 0x8);
    if (tmp == 0) goto L_8010CF0C;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x8) = tmp;
L_8010CF0C:
    tmp = *(u32*)((u8*)r6 + 0xC);
    if (tmp == 0) goto L_8010CF20;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0xC) = tmp;
L_8010CF20:
    tmp = *(u32*)((u8*)r4 + 0x30);
    if (tmp == 0) goto L_8010CF4C;
    tmp = tmp + r3;
    *(u32*)((u8*)r4 + 0x30) = tmp;
    r6 = *(u32*)((u8*)r4 + 0x30);
    tmp = *(u32*)((u8*)r6 + 0x0);
    if (tmp == 0) goto L_8010CF4C;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x0) = tmp;
L_8010CF4C:
    tmp = *(u32*)((u8*)r4 + 0x34);
    if (tmp == 0) goto L_8010CFA0;
    tmp = tmp + r3;
    *(u32*)((u8*)r4 + 0x34) = tmp;
    r6 = *(u32*)((u8*)r4 + 0x34);
    tmp = *(u32*)((u8*)r6 + 0x0);
    if (tmp == 0) goto L_8010CF78;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x0) = tmp;
L_8010CF78:
    tmp = *(u32*)((u8*)r6 + 0x8);
    if (tmp == 0) goto L_8010CF8C;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x8) = tmp;
L_8010CF8C:
    tmp = *(u32*)((u8*)r6 + 0xC);
    if (tmp == 0) goto L_8010CFA0;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0xC) = tmp;
L_8010CFA0:
    tmp = *(u32*)((u8*)r4 + 0x38);
    if (tmp == 0) goto L_8010CFCC;
    tmp = tmp + r3;
    *(u32*)((u8*)r4 + 0x38) = tmp;
    r6 = *(u32*)((u8*)r4 + 0x38);
    tmp = *(u32*)((u8*)r6 + 0x0);
    if (tmp == 0) goto L_8010CFCC;
    tmp = tmp + r3;
    *(u32*)((u8*)r6 + 0x0) = tmp;
L_8010CFCC:
    r5 = r5 + 0x1;
    r4 = r4 + 0x40;
L_8010CFD4:
    tmp = *(u32*)((u8*)r3 + 0x4);
    if (r5 < tmp) goto L_8010CE24;
    return;
}

/* 0x54 | fn_8010CFE4 | global_cond_call */
u32 fn_8010CFE4(void) {
    /* uses lbl_80404C68 */
    if (1 /* field check */) { return 0; }
    fn_8010CE04();
    return 1;
}

/* 0x8010D038 | 0x2C */
void fn_8010D038(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = (u32)&lbl_80404C68;
    r4 = (u32)&lbl_80404C68;
    r3 = *(u32*)((u8*)r4 + 0x3704);
    if ((s32)r3 >= 0) goto L_8010D054;
    r3 = 0x0;
    return;
L_8010D054:
    r3 = 0x1;
    *(u32*)((u8*)r4 + 0x3704) = tmp;
    return;
}

/* 0x8010D064 | 0x10C */
void fn_8010D064(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = (u32)&lbl_80404C68;
    r5 = (u32)&lbl_80404C68;
    r3 = *(u32*)((u8*)r5 + 0x3704);
    r6 = r3 + 0x1;
    if ((s32)r6 < 4) goto L_8010D084;
    r3 = 0x0;
    return;
L_8010D084:
    r3 = r6 * 0xdc0;
    r4 = 0x0;
    tmp = 0x3;
    *(u32*)((u8*)r5 + 0x0) = r4;
    r3 = r5 + r3;
    ctr_fn = (void(*)(void))tmp;
L_8010D09C:
    tmp = *(u16*)((u8*)r3 + 0xA14);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA14) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA28);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA28) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA3C);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA3C) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA50);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA50) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA64);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA64) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA78);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA78) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xA8C);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xA8C) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAA0);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAA0) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAB4);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAB4) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAC8);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAC8) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xADC);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xADC) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xAF0);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xAF0) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB04);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB04) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB18);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB18) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB2C);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB2C) = tmp;
    tmp = *(u16*)((u8*)r3 + 0xB40);
    tmp = tmp & 0x0000FFFE;
    *(u16*)((u8*)r3 + 0xB40) = tmp;
    r3 = r3 + 0x140;
    if (--ctr != 0) goto L_8010D09C;
    *(u32*)((u8*)r5 + 0x3704) = r6;
    r3 = 0x1;
    return;
}

/* 0x8010D170 | 0x9C */
void fn_8010D170(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)&lbl_80404C68;
    tmp = 0x0;
    r30 = (u32)&lbl_80404C68;
    *(u32*)((u8*)r30 + 0x3704) = tmp;
    *(u32*)((u8*)r30 + 0x370C) = tmp;
    ((void(*)(void))fn_800D7894)();
    r5 = (u32)&lbl_80404C68;
    r4 = 0x1;
    r31 = (u32)&lbl_80404C68;
    r5 = 0x0;
    *(u32*)((u8*)r31 + 0x3708) = r3;
    r6 = 0x1;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    ((void(*)(void))fn_800D7868)();
    r3 = *(u32*)((u8*)r31 + 0x3708);
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x6;
    r7 = 0xa;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    ((void(*)(void))fn_800D7868)();
    tmp = 0x0;
    *(u32*)((u8*)r30 + 0x370C) = tmp;
    return;
}

/* 0x8010D20C | 0x1BC */
void fn_8010D20C(void) {
    u8 sp[0xB0];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r29 = r3;
    r30 = r4;
    r3 = 0x3;
    ((void(*)(void))fn_800D6A00)();
    r31 = *(u32*)((u8*)r29 + 0x0);
    r26 = 0x0;
    f27 = *(f32*)&lbl_8047CECC;
    r28 = 0x43300000;
    f28 = *(f64*)&lbl_8047CED8;
    f29 = *(f32*)&lbl_8047CED0;
    f30 = *(f32*)&lbl_8047CEC8;
    f31 = *(f32*)&lbl_8047CED4;
    goto L_8010D380;
L_8010D278:
    r3 = (u32)sp + 0x8;
    r4 = 0x0;
    r5 = 0x4;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = 0xc0;
    *(u8*)(sp + 0xB) = tmp;
    r3 = *(u8*)((u8*)r31 + 0x30);
    /* extrwi tmp, r3, 4, 24 */;
    r3 = r3 & 0xF;
    *(u32*)(sp + 0x1C) = tmp;
    r3 = r3 + 0x1;
    f0 = f0 - f28;
    f0 = f0 / f29;
    f0 = f27 * f0 + f30;
    f0 = (f64)(s32)f0;
    *(u8*)(sp + 0x9) = tmp;
    if ((s32)r3 < 0x10) goto L_8010D2D8;
    r3 = 0x0;
L_8010D2D8:
    *(u32*)(sp + 0x2C) = tmp;
    f0 = f0 - f28;
    f0 = f0 / f29;
    f0 = f31 * f0;
    f0 = (f64)(s32)f0;
    *(u8*)(sp + 0xA) = tmp;
    tmp = *(u8*)((u8*)r31 + 0x31);
    /* extrwi. tmp, tmp, 4, 24 */;
    if ((s32)r3 <= 0x10) goto L_8010D31C;
    r3 = tmp << 2;
    tmp = r3 + 0xc0;
    *(u8*)(sp + 0x8) = tmp;
L_8010D31C:
    r3 = 0x3;
    ((void(*)(void))fn_800D67BC)();
    r25 = 0x0;
    r27 = r31;
L_8010D32C:
    r3 = r30;
    r4 = r27;
    r5 = (u32)sp + 0xc;
    ((void(*)(void))fn_800A37CC)();
    f1 = *(f32*)(sp + 0xC);
    f2 = *(f32*)(sp + 0x10);
    f3 = *(f32*)(sp + 0x14);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0x8);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0x9);
    r6 = *(u8*)(sp + 0xA);
    r7 = *(u8*)(sp + 0xB);
    ((void(*)(void))fn_800D5CB8)();
    r25 = r25 + 0x1;
    r27 = r27 + 0xc;
    if ((s32)r25 < 3) goto L_8010D32C;
    ((void(*)(void))fn_800D6728)();
    r26 = r26 + 0x1;
    r31 = r31 + 0x34;
L_8010D380:
    tmp = *(u32*)((u8*)r29 + 0x4);
    if (r26 < tmp) goto L_8010D278;
    return;
}

/* 0x8010D3C8 | 0x50C */
void fn_8010D3C8(void) {
    extern void fn_8010C8D0();
    extern void fn_8010CA30();
    extern void fn_8010CBC0();
    extern void fn_8010D20C();
    u8 sp[0x120];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    fn_8010CBC0();
    /* mr. r29, r3 */;
    if ((s32)tmp != 0) goto L_8010D3EC;
    r3 = 0x0;
    goto L_8010D8C0;
L_8010D3EC:
    r3 = 0x1;
    ((void(*)(void))fn_800DA028)();
    r3 = (u32)&lbl_80404C68;
    r28 = (u32)&lbl_80404C68;
    r3 = *(u32*)((u8*)r28 + 0x3708);
    ((void(*)(void))fn_800D7820)();
    r3 = 0x1;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x6;
    ((void(*)(void))fn_800D888C)();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    ((void(*)(void))fn_800DA4C4)();
    r3 = 0x1;
    r4 = 0x2;
    r5 = 0x1;
    ((void(*)(void))fn_800DA1E8)();
    r3 = 0x0;
    ((void(*)(void))fn_800D9ED8)();
    r3 = *(u32*)((u8*)r28 + 0x3708);
    r4 = 0x80000;
    ((void(*)(void))fn_800DAF60)();
    tmp = r3 & 0xFF;
    if ((s32)tmp != 0) goto L_8010D468;
    r3 = (u32)&lbl_80272050;
    r3 = (u32)&lbl_80272050;
    ((void(*)(void))fn_800C8710)();
    r3 = 0x0;
    goto L_8010D8C0;
L_8010D468:
    r30 = *(u32*)((u8*)r29 + 0x0);
    r31 = 0x0;
    goto L_8010D8B0;
L_8010D474:
    tmp = *(u16*)((u8*)r30 + 0x3C);
    tmp = tmp & 0x1;
    if ((s32)tmp != 0) goto L_8010D8A8;
    r4 = r31;
    r3 = (u32)sp + 0xb8;
    fn_8010CA30();
    r4 = r31;
    r3 = (u32)sp + 0x88;
    fn_8010C8D0();
    r3 = *(u32*)((u8*)r30 + 0x24);
    if (r3 == 0) goto L_8010D4B0;
    r4 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x88;
    fn_8010D20C();
L_8010D4B0:
    r20 = *(u32*)((u8*)r30 + 0x28);
    if (r20 == 0) goto L_8010D5B0;
    tmp = *(u32*)&lbl_8047CEB8;
    r21 = (u32)sp + 0x64;
    r28 = *(u32*)((u8*)r20 + 0x0);
    r26 = 0x0;
    *(u32*)(sp + 0x18) = tmp;
    goto L_8010D5A4;
L_8010D4D4:
    r25 = r21;
    r24 = r28;
    r27 = 0x0;
L_8010D4E0:
    r4 = r24;
    r5 = r25;
    r3 = (u32)sp + 0xb8;
    ((void(*)(void))fn_800A37CC)();
    r27 = r27 + 0x1;
    r24 = r24 + 0xc;
    r25 = r25 + 0xc;
    if ((s32)r27 < 3) goto L_8010D4E0;
    r3 = 0x1;
    ((void(*)(void))fn_800D6A00)();
    r22 = 0x0;
    r24 = r21;
L_8010D514:
    r27 = r22 + 0x1;
    if ((s32)r27 < 3) goto L_8010D524;
    r27 = 0x0;
L_8010D524:
    r3 = 0x2;
    ((void(*)(void))fn_800D67BC)();
    f1 = *(f32*)((u8*)r24 + 0x0);
    f2 = *(f32*)((u8*)r24 + 0x4);
    f3 = *(f32*)((u8*)r24 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0x18);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0x19);
    r6 = *(u8*)(sp + 0x1A);
    r7 = *(u8*)(sp + 0x1B);
    ((void(*)(void))fn_800D5CB8)();
    tmp = r27 * 0xc;
    r3 = (u32)sp + 0x64;
    r3 = r3 + tmp;
    f1 = *(f32*)((u8*)r3 + 0x0);
    f2 = *(f32*)((u8*)r3 + 0x4);
    f3 = *(f32*)((u8*)r3 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0x18);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0x19);
    r6 = *(u8*)(sp + 0x1A);
    r7 = *(u8*)(sp + 0x1B);
    ((void(*)(void))fn_800D5CB8)();
    ((void(*)(void))fn_800D6728)();
    r22 = r22 + 0x1;
    r24 = r24 + 0xc;
    if ((s32)r22 < 3) goto L_8010D514;
    r26 = r26 + 0x1;
    r28 = r28 + 0x34;
L_8010D5A4:
    tmp = *(u32*)((u8*)r20 + 0x4);
    if (r26 < tmp) goto L_8010D4D4;
L_8010D5B0:
    r28 = *(u32*)((u8*)r30 + 0x2C);
    if (r28 == 0) goto L_8010D658;
    tmp = *(u32*)&lbl_8047CEBC;
    r3 = 0x3;
    r26 = *(u32*)((u8*)r28 + 0x0);
    *(u32*)(sp + 0x14) = tmp;
    ((void(*)(void))fn_800D6A00)();
    r23 = *(u8*)(sp + 0x17);
    r25 = 0x0;
    r22 = *(u8*)(sp + 0x16);
    r21 = *(u8*)(sp + 0x15);
    r20 = *(u8*)(sp + 0x14);
    goto L_8010D64C;
L_8010D5E8:
    r3 = 0x3;
    ((void(*)(void))fn_800D67BC)();
    r27 = 0x0;
    r24 = r26;
L_8010D5F8:
    r4 = r24;
    r3 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x34;
    ((void(*)(void))fn_800A37CC)();
    f1 = *(f32*)(sp + 0x34);
    f2 = *(f32*)(sp + 0x38);
    f3 = *(f32*)(sp + 0x3C);
    ((void(*)(void))fn_800D6680)();
    r4 = r20;
    r5 = r21;
    r6 = r22;
    r7 = r23;
    r3 = 0x0;
    ((void(*)(void))fn_800D5CB8)();
    r27 = r27 + 0x1;
    r24 = r24 + 0xc;
    if ((s32)r27 < 3) goto L_8010D5F8;
    ((void(*)(void))fn_800D6728)();
    r25 = r25 + 0x1;
    r26 = r26 + 0x34;
L_8010D64C:
    tmp = *(u32*)((u8*)r28 + 0x4);
    if (r25 < tmp) goto L_8010D5E8;
L_8010D658:
    r28 = *(u32*)((u8*)r30 + 0x30);
    if (r28 == 0) goto L_8010D700;
    tmp = *(u32*)&lbl_8047CEC0;
    r3 = 0x3;
    r25 = *(u32*)((u8*)r28 + 0x0);
    *(u32*)(sp + 0x10) = tmp;
    ((void(*)(void))fn_800D6A00)();
    r21 = *(u8*)(sp + 0x13);
    r27 = 0x0;
    r22 = *(u8*)(sp + 0x12);
    r23 = *(u8*)(sp + 0x11);
    r24 = *(u8*)(sp + 0x10);
    goto L_8010D6F4;
L_8010D690:
    r3 = 0x3;
    ((void(*)(void))fn_800D67BC)();
    r26 = 0x0;
    r20 = r25;
L_8010D6A0:
    r4 = r20;
    r3 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x28;
    ((void(*)(void))fn_800A37CC)();
    f1 = *(f32*)(sp + 0x28);
    f2 = *(f32*)(sp + 0x2C);
    f3 = *(f32*)(sp + 0x30);
    ((void(*)(void))fn_800D6680)();
    r4 = r24;
    r5 = r23;
    r6 = r22;
    r7 = r21;
    r3 = 0x0;
    ((void(*)(void))fn_800D5CB8)();
    r26 = r26 + 0x1;
    r20 = r20 + 0xc;
    if ((s32)r26 < 3) goto L_8010D6A0;
    ((void(*)(void))fn_800D6728)();
    r27 = r27 + 0x1;
    r25 = r25 + 0x34;
L_8010D6F4:
    tmp = *(u32*)((u8*)r28 + 0x4);
    if (r27 < tmp) goto L_8010D690;
L_8010D700:
    r22 = *(u32*)((u8*)r30 + 0x34);
    if (r22 == 0) goto L_8010D800;
    tmp = *(u32*)&lbl_8047CEB8;
    r23 = (u32)sp + 0x40;
    r25 = *(u32*)((u8*)r22 + 0x0);
    r27 = 0x0;
    *(u32*)(sp + 0xC) = tmp;
    goto L_8010D7F4;
L_8010D724:
    r20 = r23;
    r21 = r25;
    r26 = 0x0;
L_8010D730:
    r4 = r21;
    r5 = r20;
    r3 = (u32)sp + 0xb8;
    ((void(*)(void))fn_800A37CC)();
    r26 = r26 + 0x1;
    r21 = r21 + 0xc;
    r20 = r20 + 0xc;
    if ((s32)r26 < 3) goto L_8010D730;
    r3 = 0x1;
    ((void(*)(void))fn_800D6A00)();
    r21 = 0x0;
    r20 = r23;
L_8010D764:
    r26 = r21 + 0x1;
    if ((s32)r26 < 3) goto L_8010D774;
    r26 = 0x0;
L_8010D774:
    r3 = 0x2;
    ((void(*)(void))fn_800D67BC)();
    f1 = *(f32*)((u8*)r20 + 0x0);
    f2 = *(f32*)((u8*)r20 + 0x4);
    f3 = *(f32*)((u8*)r20 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0xC);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0xD);
    r6 = *(u8*)(sp + 0xE);
    r7 = *(u8*)(sp + 0xF);
    ((void(*)(void))fn_800D5CB8)();
    tmp = r26 * 0xc;
    r3 = (u32)sp + 0x40;
    r3 = r3 + tmp;
    f1 = *(f32*)((u8*)r3 + 0x0);
    f2 = *(f32*)((u8*)r3 + 0x4);
    f3 = *(f32*)((u8*)r3 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0xC);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0xD);
    r6 = *(u8*)(sp + 0xE);
    r7 = *(u8*)(sp + 0xF);
    ((void(*)(void))fn_800D5CB8)();
    ((void(*)(void))fn_800D6728)();
    r21 = r21 + 0x1;
    r20 = r20 + 0xc;
    if ((s32)r21 < 3) goto L_8010D764;
    r27 = r27 + 0x1;
    r25 = r25 + 0x34;
L_8010D7F4:
    tmp = *(u32*)((u8*)r22 + 0x4);
    if (r27 < tmp) goto L_8010D724;
L_8010D800:
    r28 = *(u32*)((u8*)r30 + 0x38);
    if (r28 == 0) goto L_8010D8A8;
    tmp = *(u32*)&lbl_8047CEC4;
    r3 = 0x3;
    r25 = *(u32*)((u8*)r28 + 0x0);
    *(u32*)(sp + 0x8) = tmp;
    ((void(*)(void))fn_800D6A00)();
    r21 = *(u8*)(sp + 0xB);
    r27 = 0x0;
    r22 = *(u8*)(sp + 0xA);
    r23 = *(u8*)(sp + 0x9);
    r24 = *(u8*)(sp + 0x8);
    goto L_8010D89C;
L_8010D838:
    r3 = 0x3;
    ((void(*)(void))fn_800D67BC)();
    r26 = 0x0;
    r20 = r25;
L_8010D848:
    r4 = r20;
    r3 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x1c;
    ((void(*)(void))fn_800A37CC)();
    f1 = *(f32*)(sp + 0x1C);
    f2 = *(f32*)(sp + 0x20);
    f3 = *(f32*)(sp + 0x24);
    ((void(*)(void))fn_800D6680)();
    r4 = r24;
    r5 = r23;
    r6 = r22;
    r7 = r21;
    r3 = 0x0;
    ((void(*)(void))fn_800D5CB8)();
    r26 = r26 + 0x1;
    r20 = r20 + 0xc;
    if ((s32)r26 < 3) goto L_8010D848;
    ((void(*)(void))fn_800D6728)();
    r27 = r27 + 0x1;
    r25 = r25 + 0x30;
L_8010D89C:
    tmp = *(u32*)((u8*)r28 + 0x4);
    if (r27 < tmp) goto L_8010D838;
L_8010D8A8:
    r31 = r31 + 0x1;
    r30 = r30 + 0x40;
L_8010D8B0:
    tmp = *(u32*)((u8*)r29 + 0x4);
    if (r31 < tmp) goto L_8010D474;
    ((void(*)(void))fn_800DADB4)();
L_8010D8C0:
    return;
}

/* 0x8010D8D4 | 0x52C */
void fn_8010D8D4(void) {
    extern void fn_8010C8D0();
    extern void fn_8010CA30();
    extern void fn_8010CBC0();
    extern void fn_8010D20C();
    extern void fn_8010D3C8();
    u8 sp[0x120];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    fn_8010CBC0();
    /* mr. r28, r3 */;
    if ((s32)tmp == 0) goto L_8010DDEC;
    r3 = 0x1;
    ((void(*)(void))fn_800DA028)();
    r3 = (u32)&lbl_80404C68;
    r3 = (u32)&lbl_80404C68;
    r3 = *(u32*)((u8*)r3 + 0x3708);
    ((void(*)(void))fn_800D7820)();
    r3 = 0x1;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x6;
    ((void(*)(void))fn_800D888C)();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    ((void(*)(void))fn_800DA4C4)();
    r3 = 0x1;
    r4 = 0x2;
    r5 = 0x1;
    ((void(*)(void))fn_800DA1E8)();
    r3 = 0x0;
    ((void(*)(void))fn_800D9ED8)();
    r3 = (u32)&lbl_80404C68;
    r30 = *(u32*)((u8*)r28 + 0x0);
    r3 = (u32)&lbl_80404C68;
    r31 = 0x0;
    tmp = *(u32*)((u8*)r3 + 0x3704);
    tmp = tmp * 0xdc0;
    r3 = r3 + tmp;
    r29 = r3 + 0x4;
    goto L_8010DDB0;
L_8010D964:
    tmp = *(u16*)((u8*)r29 + 0x24);
    tmp = tmp & 0x1;
    if ((s32)tmp != 0) goto L_8010DDA4;
    tmp = *(u16*)((u8*)r30 + 0x3C);
    tmp = tmp & 0x1;
    if ((s32)tmp == 0) goto L_8010DDA4;
    r4 = r31;
    r3 = (u32)sp + 0xb8;
    fn_8010CA30();
    r4 = r31;
    r3 = (u32)sp + 0x88;
    fn_8010C8D0();
    r3 = *(u32*)((u8*)r30 + 0x24);
    if (r3 == 0) goto L_8010D9AC;
    r4 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x88;
    fn_8010D20C();
L_8010D9AC:
    r19 = *(u32*)((u8*)r30 + 0x28);
    if (r19 == 0) goto L_8010DAAC;
    tmp = *(u32*)&lbl_8047CEB8;
    r20 = (u32)sp + 0x64;
    r27 = *(u32*)((u8*)r19 + 0x0);
    r25 = 0x0;
    *(u32*)(sp + 0x18) = tmp;
    goto L_8010DAA0;
L_8010D9D0:
    r24 = r20;
    r23 = r27;
    r26 = 0x0;
L_8010D9DC:
    r4 = r23;
    r5 = r24;
    r3 = (u32)sp + 0xb8;
    ((void(*)(void))fn_800A37CC)();
    r26 = r26 + 0x1;
    r23 = r23 + 0xc;
    r24 = r24 + 0xc;
    if ((s32)r26 < 3) goto L_8010D9DC;
    r3 = 0x1;
    ((void(*)(void))fn_800D6A00)();
    r21 = 0x0;
    r23 = r20;
L_8010DA10:
    r26 = r21 + 0x1;
    if ((s32)r26 < 3) goto L_8010DA20;
    r26 = 0x0;
L_8010DA20:
    r3 = 0x2;
    ((void(*)(void))fn_800D67BC)();
    f1 = *(f32*)((u8*)r23 + 0x0);
    f2 = *(f32*)((u8*)r23 + 0x4);
    f3 = *(f32*)((u8*)r23 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0x18);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0x19);
    r6 = *(u8*)(sp + 0x1A);
    r7 = *(u8*)(sp + 0x1B);
    ((void(*)(void))fn_800D5CB8)();
    tmp = r26 * 0xc;
    r3 = (u32)sp + 0x64;
    r3 = r3 + tmp;
    f1 = *(f32*)((u8*)r3 + 0x0);
    f2 = *(f32*)((u8*)r3 + 0x4);
    f3 = *(f32*)((u8*)r3 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0x18);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0x19);
    r6 = *(u8*)(sp + 0x1A);
    r7 = *(u8*)(sp + 0x1B);
    ((void(*)(void))fn_800D5CB8)();
    ((void(*)(void))fn_800D6728)();
    r21 = r21 + 0x1;
    r23 = r23 + 0xc;
    if ((s32)r21 < 3) goto L_8010DA10;
    r25 = r25 + 0x1;
    r27 = r27 + 0x34;
L_8010DAA0:
    tmp = *(u32*)((u8*)r19 + 0x4);
    if (r25 < tmp) goto L_8010D9D0;
L_8010DAAC:
    r27 = *(u32*)((u8*)r30 + 0x2C);
    if (r27 == 0) goto L_8010DB54;
    tmp = *(u32*)&lbl_8047CEBC;
    r3 = 0x3;
    r25 = *(u32*)((u8*)r27 + 0x0);
    *(u32*)(sp + 0x14) = tmp;
    ((void(*)(void))fn_800D6A00)();
    r22 = *(u8*)(sp + 0x17);
    r24 = 0x0;
    r21 = *(u8*)(sp + 0x16);
    r20 = *(u8*)(sp + 0x15);
    r19 = *(u8*)(sp + 0x14);
    goto L_8010DB48;
L_8010DAE4:
    r3 = 0x3;
    ((void(*)(void))fn_800D67BC)();
    r26 = 0x0;
    r23 = r25;
L_8010DAF4:
    r4 = r23;
    r3 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x34;
    ((void(*)(void))fn_800A37CC)();
    f1 = *(f32*)(sp + 0x34);
    f2 = *(f32*)(sp + 0x38);
    f3 = *(f32*)(sp + 0x3C);
    ((void(*)(void))fn_800D6680)();
    r4 = r19;
    r5 = r20;
    r6 = r21;
    r7 = r22;
    r3 = 0x0;
    ((void(*)(void))fn_800D5CB8)();
    r26 = r26 + 0x1;
    r23 = r23 + 0xc;
    if ((s32)r26 < 3) goto L_8010DAF4;
    ((void(*)(void))fn_800D6728)();
    r24 = r24 + 0x1;
    r25 = r25 + 0x34;
L_8010DB48:
    tmp = *(u32*)((u8*)r27 + 0x4);
    if (r24 < tmp) goto L_8010DAE4;
L_8010DB54:
    r27 = *(u32*)((u8*)r30 + 0x30);
    if (r27 == 0) goto L_8010DBFC;
    tmp = *(u32*)&lbl_8047CEC0;
    r3 = 0x3;
    r24 = *(u32*)((u8*)r27 + 0x0);
    *(u32*)(sp + 0x10) = tmp;
    ((void(*)(void))fn_800D6A00)();
    r20 = *(u8*)(sp + 0x13);
    r26 = 0x0;
    r21 = *(u8*)(sp + 0x12);
    r22 = *(u8*)(sp + 0x11);
    r23 = *(u8*)(sp + 0x10);
    goto L_8010DBF0;
L_8010DB8C:
    r3 = 0x3;
    ((void(*)(void))fn_800D67BC)();
    r25 = 0x0;
    r19 = r24;
L_8010DB9C:
    r4 = r19;
    r3 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x28;
    ((void(*)(void))fn_800A37CC)();
    f1 = *(f32*)(sp + 0x28);
    f2 = *(f32*)(sp + 0x2C);
    f3 = *(f32*)(sp + 0x30);
    ((void(*)(void))fn_800D6680)();
    r4 = r23;
    r5 = r22;
    r6 = r21;
    r7 = r20;
    r3 = 0x0;
    ((void(*)(void))fn_800D5CB8)();
    r25 = r25 + 0x1;
    r19 = r19 + 0xc;
    if ((s32)r25 < 3) goto L_8010DB9C;
    ((void(*)(void))fn_800D6728)();
    r26 = r26 + 0x1;
    r24 = r24 + 0x34;
L_8010DBF0:
    tmp = *(u32*)((u8*)r27 + 0x4);
    if (r26 < tmp) goto L_8010DB8C;
L_8010DBFC:
    r21 = *(u32*)((u8*)r30 + 0x34);
    if (r21 == 0) goto L_8010DCFC;
    tmp = *(u32*)&lbl_8047CEB8;
    r22 = (u32)sp + 0x40;
    r24 = *(u32*)((u8*)r21 + 0x0);
    r26 = 0x0;
    *(u32*)(sp + 0xC) = tmp;
    goto L_8010DCF0;
L_8010DC20:
    r19 = r22;
    r20 = r24;
    r25 = 0x0;
L_8010DC2C:
    r4 = r20;
    r5 = r19;
    r3 = (u32)sp + 0xb8;
    ((void(*)(void))fn_800A37CC)();
    r25 = r25 + 0x1;
    r20 = r20 + 0xc;
    r19 = r19 + 0xc;
    if ((s32)r25 < 3) goto L_8010DC2C;
    r3 = 0x1;
    ((void(*)(void))fn_800D6A00)();
    r20 = 0x0;
    r19 = r22;
L_8010DC60:
    r25 = r20 + 0x1;
    if ((s32)r25 < 3) goto L_8010DC70;
    r25 = 0x0;
L_8010DC70:
    r3 = 0x2;
    ((void(*)(void))fn_800D67BC)();
    f1 = *(f32*)((u8*)r19 + 0x0);
    f2 = *(f32*)((u8*)r19 + 0x4);
    f3 = *(f32*)((u8*)r19 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0xC);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0xD);
    r6 = *(u8*)(sp + 0xE);
    r7 = *(u8*)(sp + 0xF);
    ((void(*)(void))fn_800D5CB8)();
    tmp = r25 * 0xc;
    r3 = (u32)sp + 0x40;
    r3 = r3 + tmp;
    f1 = *(f32*)((u8*)r3 + 0x0);
    f2 = *(f32*)((u8*)r3 + 0x4);
    f3 = *(f32*)((u8*)r3 + 0x8);
    ((void(*)(void))fn_800D6680)();
    r4 = *(u8*)(sp + 0xC);
    r3 = 0x0;
    r5 = *(u8*)(sp + 0xD);
    r6 = *(u8*)(sp + 0xE);
    r7 = *(u8*)(sp + 0xF);
    ((void(*)(void))fn_800D5CB8)();
    ((void(*)(void))fn_800D6728)();
    r20 = r20 + 0x1;
    r19 = r19 + 0xc;
    if ((s32)r20 < 3) goto L_8010DC60;
    r26 = r26 + 0x1;
    r24 = r24 + 0x34;
L_8010DCF0:
    tmp = *(u32*)((u8*)r21 + 0x4);
    if (r26 < tmp) goto L_8010DC20;
L_8010DCFC:
    r27 = *(u32*)((u8*)r30 + 0x38);
    if (r27 == 0) goto L_8010DDA4;
    tmp = *(u32*)&lbl_8047CEC4;
    r3 = 0x3;
    r24 = *(u32*)((u8*)r27 + 0x0);
    *(u32*)(sp + 0x8) = tmp;
    ((void(*)(void))fn_800D6A00)();
    r20 = *(u8*)(sp + 0xB);
    r26 = 0x0;
    r21 = *(u8*)(sp + 0xA);
    r22 = *(u8*)(sp + 0x9);
    r23 = *(u8*)(sp + 0x8);
    goto L_8010DD98;
L_8010DD34:
    r3 = 0x3;
    ((void(*)(void))fn_800D67BC)();
    r25 = 0x0;
    r19 = r24;
L_8010DD44:
    r4 = r19;
    r3 = (u32)sp + 0xb8;
    r5 = (u32)sp + 0x1c;
    ((void(*)(void))fn_800A37CC)();
    f1 = *(f32*)(sp + 0x1C);
    f2 = *(f32*)(sp + 0x20);
    f3 = *(f32*)(sp + 0x24);
    ((void(*)(void))fn_800D6680)();
    r4 = r23;
    r5 = r22;
    r6 = r21;
    r7 = r20;
    r3 = 0x0;
    ((void(*)(void))fn_800D5CB8)();
    r25 = r25 + 0x1;
    r19 = r19 + 0xc;
    if ((s32)r25 < 3) goto L_8010DD44;
    ((void(*)(void))fn_800D6728)();
    r26 = r26 + 0x1;
    r24 = r24 + 0x30;
L_8010DD98:
    tmp = *(u32*)((u8*)r27 + 0x4);
    if (r26 < tmp) goto L_8010DD34;
L_8010DDA4:
    r31 = r31 + 0x1;
    r30 = r30 + 0x40;
    r29 = r29 + 0x28;
L_8010DDB0:
    tmp = *(u32*)((u8*)r28 + 0x4);
    if (r31 < tmp) goto L_8010D964;
    r3 = (u32)&lbl_80404C68;
    r27 = (u32)&lbl_80404C68;
    tmp = *(u32*)((u8*)r27 + 0x370C);
    if (tmp != 0) goto L_8010DDD8;
    fn_8010D3C8();
    *(u32*)((u8*)r27 + 0x370C) = r3;
L_8010DDD8:
    r3 = *(u32*)((u8*)r27 + 0x370C);
    if (r3 == 0) goto L_8010DDEC;
    ((void(*)(void))fn_800DAD10)();
    ((void(*)(void))fn_800D30AC)();
L_8010DDEC:
    return;
}

/* 0x8010DE00 | 0xF0 */
void fn_8010DE00(void) {
    u8 sp[0x80];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r5;
    r30 = r4;
    r4 = (u32)sp + 0x8;
    r29 = r3;
    ((void(*)(void))fn_8010E138)();
    if ((s32)r3 > 0) goto L_8010DE3C;
    r3 = 0x0;
    goto L_8010DED4;
L_8010DE3C:
    f2 = *(f32*)((u8*)r29 + 0x4);
    f1 = *(f32*)(sp + 0x8);
    f0 = *(f32*)&lbl_8047CEE0;
    f1 = f2 - f1;
    if (f1 <= f0) goto L_8010DE58;
    goto L_8010DE5C;
L_8010DE58:
    f1 = -f1;
L_8010DE5C:
    f3 = f1;
    r4 = (u32)sp + 0x14;
    f0 = *(f32*)&lbl_8047CEE0;
    r6 = 0x0;
    r5 = 0x1;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r3 <= 1) goto L_8010DEB4;
L_8010DE80:
    f1 = *(f32*)((u8*)r4 + 0x0);
    f1 = f2 - f1;
    if (f1 <= f0) goto L_8010DE94;
    goto L_8010DE98;
L_8010DE94:
    f1 = -f1;
L_8010DE98:
    if (f3 <= f1) goto L_8010DEA8;
    r6 = r5;
    f3 = f1;
L_8010DEA8:
    r4 = r4 + 0xc;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto L_8010DE80;
L_8010DEB4:
    r6 = r6 * 0xc;
    r5 = (u32)sp + 0x10;
    r4 = (u32)sp + 0x11;
    r3 = 0x1;
    tmp = *(u8*)(r5 + r6);
    *(u8*)((u8*)r30 + 0x0) = tmp;
    tmp = *(u8*)(r4 + r6);
    *(u8*)((u8*)r31 + 0x0) = tmp;
L_8010DED4:
    return;
}

/* 0x8010DEF0 | 0x248 */
void fn_8010DEF0(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;

    f0 = *(f32*)&lbl_8047CEE4;
    f2 = *(f32*)&lbl_8047CEE8;
    f4 = *(f32*)((u8*)r5 + 0x0);
    f1 = f0;
    f3 = f2;
    if (f0 <= f4) goto L_8010DF10;
    f1 = f4;
L_8010DF10:
    f5 = *(f32*)((u8*)r5 + 0x8);
    if (f0 <= f5) goto L_8010DF20;
    f0 = f5;
L_8010DF20:
    if (f3 >= f4) goto L_8010DF2C;
    f3 = f4;
L_8010DF2C:
    if (f2 >= f5) goto L_8010DF38;
    f2 = f5;
L_8010DF38:
    f4 = *(f32*)((u8*)r5 + 0xC);
    if (f1 <= f4) goto L_8010DF48;
    f1 = f4;
L_8010DF48:
    f5 = *(f32*)((u8*)r5 + 0x14);
    if (f0 <= f5) goto L_8010DF58;
    f0 = f5;
L_8010DF58:
    if (f3 >= f4) goto L_8010DF64;
    f3 = f4;
L_8010DF64:
    if (f2 >= f5) goto L_8010DF70;
    f2 = f5;
L_8010DF70:
    f4 = *(f32*)((u8*)r5 + 0x18);
    if (f1 <= f4) goto L_8010DF80;
    f1 = f4;
L_8010DF80:
    f5 = *(f32*)((u8*)r5 + 0x20);
    if (f0 <= f5) goto L_8010DF90;
    f0 = f5;
L_8010DF90:
    if (f3 >= f4) goto L_8010DF9C;
    f3 = f4;
L_8010DF9C:
    if (f2 >= f5) goto L_8010DFA8;
    f2 = f5;
L_8010DFA8:
    f6 = *(f32*)((u8*)r4 + 0x0);
    if (f1 > f6) goto L_8010DFD0;
    f7 = *(f32*)((u8*)r4 + 0x8);
    if (f0 > f7) goto L_8010DFD0;
    if (f3 < f6) goto L_8010DFD0;
    if (f2 >= f7) goto L_8010DFD8;
L_8010DFD0:
    r3 = 0x0;
    return;
L_8010DFD8:
    tmp = 0x1;
    f0 = *(f32*)&lbl_8047CEE0;
    if ((s32)tmp < 3) goto L_8010DFEC;
    tmp = 0x0;
L_8010DFEC:
    tmp = tmp * 0xc;
    f5 = *(f32*)((u8*)r5 + 0x0);
    f8 = *(f32*)((u8*)r5 + 0x8);
    f1 = f6 - f5;
    r7 = r5 + tmp;
    f2 = f7 - f8;
    f3 = *(f32*)((u8*)r7 + 0x8);
    f4 = *(f32*)((u8*)r7 + 0x0);
    f3 = f3 - f8;
    f4 = f4 - f5;
    f1 = f3 * f1;
    f1 = f4 * f2 - f1;
    if (f1 <= f0) goto L_8010E02C;
    r3 = 0x0;
    return;
L_8010E02C:
    tmp = 0x2;
    if ((s32)tmp < 3) goto L_8010E03C;
    tmp = 0x0;
L_8010E03C:
    tmp = tmp * 0xc;
    f5 = *(f32*)((u8*)r5 + 0xC);
    f8 = *(f32*)((u8*)r5 + 0x14);
    f1 = f6 - f5;
    r7 = r5 + tmp;
    f2 = f7 - f8;
    f3 = *(f32*)((u8*)r7 + 0x8);
    f4 = *(f32*)((u8*)r7 + 0x0);
    f3 = f3 - f8;
    f4 = f4 - f5;
    f1 = f3 * f1;
    f1 = f4 * f2 - f1;
    if (f1 <= f0) goto L_8010E07C;
    r3 = 0x0;
    return;
L_8010E07C:
    tmp = 0x3;
    if ((s32)tmp < 3) goto L_8010E08C;
    tmp = 0x0;
L_8010E08C:
    tmp = tmp * 0xc;
    f5 = *(f32*)((u8*)r5 + 0x18);
    f8 = *(f32*)((u8*)r5 + 0x20);
    f1 = f6 - f5;
    r7 = r5 + tmp;
    f2 = f7 - f8;
    f3 = *(f32*)((u8*)r7 + 0x8);
    f4 = *(f32*)((u8*)r7 + 0x0);
    f3 = f3 - f8;
    f4 = f4 - f5;
    f1 = f3 * f1;
    f1 = f4 * f2 - f1;
    if (f1 <= f0) goto L_8010E0CC;
    r3 = 0x0;
    return;
L_8010E0CC:
    f0 = *(f32*)&lbl_8047CEE0;
    f8 = *(f32*)((u8*)r6 + 0x4);
    if (f0 != f8) goto L_8010E0E4;
    r3 = 0x0;
    return;
L_8010E0E4:
    f1 = *(f32*)((u8*)r4 + 0x4);
    f0 = *(f32*)((u8*)r5 + 0x4);
    f2 = *(f32*)((u8*)r5 + 0x0);
    f0 = f1 - f0;
    f1 = *(f32*)((u8*)r5 + 0x8);
    f2 = f6 - f2;
    f3 = *(f32*)((u8*)r6 + 0x0);
    f5 = *(f32*)((u8*)r6 + 0x8);
    f4 = f7 - f1;
    f1 = f8 * f0;
    *(f32*)((u8*)r3 + 0x0) = f6;
    f0 = *(f32*)((u8*)r4 + 0x4);
    f1 = f3 * f2 + f1;
    f1 = -(f5 * f4 + f1);
    f1 = f1 / f8;
    f0 = f0 + f1;
    *(f32*)((u8*)r3 + 0x4) = f0;
    f0 = *(f32*)((u8*)r4 + 0x8);
    *(f32*)((u8*)r3 + 0x8) = f0;
    r3 = 0x1;
    return;
}
