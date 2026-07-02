/**
 * @file gs_texture.h
 * @brief GStexture -- Genius Sonority texture management system.
 *
 * Only GStextureCreate (WIP), GStextureUnlockImage and GStextureLockImage
 * have confirmed names/content in this unit (see gs_texture.c's header
 * comment for the reconciliation that removed a fourteen-function
 * fiction block of invented GStexture* wrappers with no symbols.txt
 * entry, no external callers under those names, and -- for several --
 * directly contradicted signatures/semantics at their real call sites).
 *
 * The GStextureHandle layout below is inferred from GStextureCreate.
 *
 * Debug strings:
 *   "GStexture: invalid texture format"
 *   "GStexture: warning -- texture size adjusted from [%d,%d] to [%d,%d]"
 *
 * Address range: 0x800EF098 - 0x800F07A8 (approx.)
 */
#ifndef GS_TEXTURE_H
#define GS_TEXTURE_H

#include "dolphin/types.h"

/* ===================================================================
 * GX texture format constants (matching GXTexFmt enum)
 * =================================================================== */
#define GS_TEXFMT_I4       0x00
#define GS_TEXFMT_I8       0x01
#define GS_TEXFMT_IA4      0x02  /* unused in Colosseum? */
#define GS_TEXFMT_IA8      0x03  /* unused in Colosseum? */
#define GS_TEXFMT_RGB565   0x04  /* unused in Colosseum? */
#define GS_TEXFMT_RGB5A3   0x05  /* unused in Colosseum? */
#define GS_TEXFMT_RGBA8    0x06  /* unused in Colosseum? */
#define GS_TEXFMT_CI4      0x08  /* unused in Colosseum? */
#define GS_TEXFMT_CI8      0x09  /* unused in Colosseum? */
#define GS_TEXFMT_CI14x2   0x0A  /* unused in Colosseum? */
#define GS_TEXFMT_CMPR     0x0E

/* Internal GS texture format IDs (from format switch in GStexture_Create) */
#define GS_TEXFMT_GS_I4       0x00   /* 4bpp -> bitsPerPixel = 8 */
#define GS_TEXFMT_GS_I8       0x01   /* 8bpp -> bitsPerPixel = 4 */
#define GS_TEXFMT_GS_IA4      0x30   /* 4bpp -> bitsPerPixel = 4 */
#define GS_TEXFMT_GS_IA8      0x40   /* 8bpp -> bitsPerPixel = 4 */
#define GS_TEXFMT_GS_RGB565   0x41   /* 16bpp -> bitsPerPixel = 8 */
#define GS_TEXFMT_GS_RGB5A3   0x42   /* 16bpp -> bitsPerPixel = 8 */
#define GS_TEXFMT_GS_RGBA8    0x43   /* 32bpp -> bitsPerPixel = 16 */
#define GS_TEXFMT_GS_CI4      0x44   /* 4bpp -> bitsPerPixel = 8 */
#define GS_TEXFMT_GS_CI8      0x45   /* 8bpp -> bitsPerPixel = 32 */
#define GS_TEXFMT_GS_CI14x2   0x46   /* unused */
#define GS_TEXFMT_GS_CMPR     0x90   /* compressed -> bitsPerPixel = 16 */
#define GS_TEXFMT_GS_A8       0xA0   /* alpha-only -> bitsPerPixel = 4 */
#define GS_TEXFMT_GS_B0       0xB0   /* 4bpp -> bitsPerPixel = 4 */

/* ===================================================================
 * TLUT (texture look-up table / palette) format constants
 * =================================================================== */
#define GS_TLUT_NONE       0   /* no palette */
#define GS_TLUT_IA8        1   /* 16 entries */
#define GS_TLUT_RGB565     2   /* 256 entries */
#define GS_TLUT_RGB5A3     3   /* 1024 entries */

/* ===================================================================
 * GStextureHandle -- one entry in the texture pool.
 *
 * Size: 0x80 bytes (128 bytes).
 * Stored in the pool at lbl_8047ABF4.
 * Pool count stored at lbl_8047ABF8.
 * Pool GSmem handle stored at lbl_8047ABF0.
 *
 * Fields recovered from GStextureCreate (GStexture_Create):
 * =================================================================== */
typedef struct GStextureHandle {
    /* 0x00 */ u16    width;          /* texture width in texels */
    /* 0x02 */ u16    height;         /* texture height in texels */
    /* 0x04 */ u8     bitsPerPixel;   /* bits per pixel for this format */
    /* 0x05 */ u8     mipLevels;      /* number of mipmap levels */
    /* 0x06 */ u8     inUse;          /* 1 = slot occupied */
    /* 0x07 */ u8     dirty;          /* 1 = needs re-upload to GX */
    /* 0x08 */ u32    format;         /* GS texture format ID */
    /* 0x0C */ u32    tlutFormat;     /* TLUT palette format */
    /* 0x10 */ u32    wrapS;          /* GX wrap mode S */
    /* 0x14 */ u32    wrapT;          /* GX wrap mode T */
    /* 0x18 */ u32    minFilter;      /* GX min filter mode */
    /* 0x1C */ u32    magFilter;      /* GX mag filter mode */
    /* 0x20 */ u32    lodClamp;       /* LOD clamp / flag */
    /* 0x24 */ u16    memHandle;      /* GSmem handle for pixel data */
    /* 0x26 */ u16    pad26;
    /* 0x28 */ void*  data;           /* pointer to pixel data (from GSmemGetPtr) */
    /* 0x2C-0x48 */ u32 mipOffsets[8]; /* byte offset for each mip level */
    /* 0x48 */ u32    tlutOffset;     /* byte offset of TLUT data */
    /* 0x4C */ u32    totalSize;      /* total texture data size (all mips) */
    /* 0x50 */ u16    refCount;       /* reference count / lock count */
    /* 0x52 */ u16    pad52;
    /* 0x54 */ u8     gxTexObj[0x20]; /* GXTexObj embedded struct */
    /* 0x74 */ u8     gxTlutObj[0x0C]; /* GXTlutObj embedded struct */
} GStextureHandle;

/* ===================================================================
 * Public API
 * =================================================================== */

/**
 * GStextureCreate -- Create a new texture handle.
 *
 * @param width       Texture width in texels (4-1024).
 * @param height      Texture height in texels (4-1024).
 * @param format      GS texture format ID (GS_TEXFMT_GS_*).
 * @param tlutFormat  TLUT palette format (0-3).
 * @param mipLevels   Number of mipmap levels (0 = base only).
 * @return            Pointer to GStextureHandle, or NULL on failure.
 *
 * Validates format, computes total size including all mip levels,
 * allocates pixel data from GSmem, sets up the GXTexObj, and marks
 * the slot as in use.
 *
 * If width/height are both 0, uses the current display dimensions.
 * Prints "GStexture: invalid texture format" for unknown formats.
 * Prints "GStexture: warning -- texture size adjusted ..." if the
 * dimensions are rounded to the next power of two.
 *
 * Corresponds to GStextureCreate.
 */
GStextureHandle* GStextureCreate(u16 width, u16 height, u32 format,
                                  u32 tlutFormat, u8 mipLevels);

/**
 * GStextureUnlockImage -- Flush the texture's pixel data from the data
 * cache and invalidate the GX texture cache.
 *
 * @param tex  Texture handle.
 * @return     The texture's refCount after decrementing (used by
 *             gs_render.c's `GXDrawDone(GStextureUnlockImage(image))`).
 */
u32 GStextureUnlockImage(GStextureHandle* tex);

/**
 * GStextureLockImage -- Get a pointer to a specific mipmap level's data.
 *
 * @param tex      Texture handle.
 * @param level    Mipmap level index (0 = base).
 * @return         Pointer to the mip data, or NULL if level >= 8.
 *
 * Increments the reference count on the texture.
 */
void* GStextureLockImage(GStextureHandle* tex, u8 level);

#endif /* GS_TEXTURE_H */
