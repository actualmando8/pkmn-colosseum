/**
 * @file gs_texture.h
 * @brief GStexture -- Genius Sonority texture management system.
 *
 * GStexture manages runtime texture allocation, creation, binding, and
 * caching for the GS engine. Textures are stored in a fixed-size pool
 * of GStextureHandle entries (each 0x80 bytes), allocated from GSmem.
 *
 * The system supports all GCN texture formats (I4, I8, IA4, IA8, RGB565,
 * RGB5A3, RGBA8, CI4, CI8, CI14x2, CMPR) and handles mipmap chain
 * generation and GX texture object setup.
 *
 * The floor system creates/frees texture handles via GStexture when
 * loading and unloading area resources.
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
 * GStextureInit -- Initialise the texture pool.
 *
 * @param maxTextures  Maximum number of texture slots.
 *
 * Allocates maxTextures * 0x80 bytes from GSmem and zeroes all entries,
 * marking each slot's inUse field to 0.
 *
 * Corresponds to fn_800EFFC0.
 */
void GStextureInit(u32 maxTextures);

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
 * GStextureConvertCI -- Convert a CI (colour-indexed) texture to direct colour.
 *
 * For CI (0x44) format textures, allocates a temporary buffer, unswizzles
 * the CI data into direct-colour pixels using the palette, copies back,
 * and frees the temporary allocation.
 *
 * @param tex  Texture handle to convert.
 *
 * Corresponds to fn_800EF098.
 */
void GStextureConvertCI(GStextureHandle* tex);

/**
 * GStextureUploadFromBuffer -- Copy pixel data from an external buffer into
 * the texture, performing format conversion and GX upload.
 *
 * @param tex         Texture handle.
 * @param srcBuffer   Source pixel data to upload.
 * @return            1 on success, 0 on failure / unsupported format.
 *
 * Handles format-specific setup: computes GXTexFmt, sets up GXTexObj
 * with proper dimensions, flushes dcache, and invalidates the texture.
 *
 * Supports paletted formats (CI4/CI8/CI14x2/CMPR) with TLUT setup.
 *
 * Corresponds to fn_800EF1E8.
 */
u32 GStextureUploadFromBuffer(GStextureHandle* tex, void* srcBuffer);

/**
 * GStextureGetGXFormat -- Return the GXTexFmt for a texture.
 *
 * Maps the GS internal format ID to the GX hardware format constant.
 *
 * @param tex      Texture handle.
 * @param alpha    Alpha variant flag (for A0 format with/without alpha).
 * @return         GXTexFmt value, or -1 for unsupported formats.
 *
 * Corresponds to fn_800EF3E0.
 */
s32 GStextureGetGXFormat(GStextureHandle* tex, u8 alpha);

/**
 * GStextureGetTLUTFormat -- Return the TLUT format for a texture.
 *
 * @param tex  Texture handle.
 * @return     TLUT format value from the handle.
 *
 * Corresponds to fn_800EF4D4.
 */
u32 GStextureGetTLUTFormat(GStextureHandle* tex);

/**
 * GStextureGetFormat -- Return the internal GS format for a texture.
 *
 * @param tex  Texture handle.
 * @return     GS format ID.
 *
 * Corresponds to fn_800EF4DC.
 */
u32 GStextureGetFormat(GStextureHandle* tex);

/**
 * GStextureGetMipCount -- Return the number of extra mipmap levels.
 *
 * @param tex  Texture handle.
 * @return     Mip level count minus 1 (0 = base only).
 *
 * Corresponds to fn_800EF4E4.
 */
u8 GStextureGetMipCount(GStextureHandle* tex);

/**
 * GStextureGetHeight -- Return the texture height.
 *
 * @param tex  Texture handle.
 * @return     Height in texels.
 *
 * Corresponds to fn_800EF4F4.
 */
u16 GStextureGetHeight(GStextureHandle* tex);

/**
 * GStextureGetWidth -- Return the texture width.
 *
 * @param tex  Texture handle.
 * @return     Width in texels.
 *
 * Corresponds to fn_800EF4FC.
 */
u16 GStextureGetWidth(GStextureHandle* tex);

/**
 * GStextureFlush -- Flush the texture's pixel data from the data cache.
 *
 * Calls DCFlushRange on the pixel buffer, invalidates the GX texture
 * cache, and decrements the reference count.
 *
 * @param tex  Texture handle.
 *
 * Corresponds to GStextureUnlockImage.
 */
void GStextureFlush(GStextureHandle* tex);

/**
 * GStextureGetMipData -- Get a pointer to a specific mipmap level's data.
 *
 * @param tex      Texture handle.
 * @param level    Mipmap level index (0 = base).
 * @return         Pointer to the mip data, or NULL if level >= 8.
 *
 * Increments the reference count on the texture.
 *
 * Corresponds to GStextureLockImage.
 */
void* GStextureGetMipData(GStextureHandle* tex, u8 level);

/**
 * GStextureSetWrapMode -- Set wrap and filter modes on a texture.
 *
 * @param tex       Texture handle.
 * @param wrapS     GX wrap mode for S coordinate.
 * @param wrapT     GX wrap mode for T coordinate.
 * @param lodClamp  LOD clamp value.
 *
 * Marks the texture as dirty.
 *
 * Corresponds to fn_800EF578.
 */
void GStextureSetWrapMode(GStextureHandle* tex, u32 wrapS, u32 wrapT,
                           u32 lodClamp);

/**
 * GStextureSetFilterMode -- Set min/mag filter modes on a texture.
 *
 * @param tex       Texture handle.
 * @param minFilt   GX min filter mode.
 * @param magFilt   GX mag filter mode.
 *
 * Marks the texture as dirty.
 *
 * Corresponds to fn_800EF590.
 */
void GStextureSetFilterMode(GStextureHandle* tex, u32 minFilt, u32 magFilt);

/**
 * GStextureFree -- Free a texture handle and release its GSmem allocation.
 *
 * Locks the GSmem handle, then frees it. Marks the slot as not in use.
 *
 * @param tex  Texture handle to free.
 *
 * Corresponds to fn_800EF5A4.
 */
void GStextureFree(GStextureHandle* tex);

/**
 * GStextureBind -- Bind an external GSmem allocation as texture data.
 *
 * If the slot is already marked as not-in-use, stores the given GSmem
 * handle and marks the slot as in-use. Used when the floor system
 * provides pre-loaded texture data.
 *
 * @param tex       Texture handle.
 * @param memHandle GSmem handle for the texture pixel data.
 *
 * Corresponds to fn_800EFD14.
 */
void GStextureBind(GStextureHandle* tex, u16 memHandle);

/**
 * GStextureSetupFromTPL -- Set up a texture from a TPL (Texture Palette Library)
 * data block. Resolves relative offsets in the TPL data to absolute pointers,
 * sets up the GXTexObj and GXTlutObj, and marks the texture dirty.
 *
 * This is used when loading textures from FSYS archives where the texture
 * data is stored in TPL format.
 *
 * @param tex  Texture handle (must already have format/size configured).
 *
 * Returns the texture handle on success.
 *
 * Corresponds to fn_800EFD3C.
 */
GStextureHandle* GStextureSetupFromTPL(GStextureHandle* tex);

#endif /* GS_TEXTURE_H */
