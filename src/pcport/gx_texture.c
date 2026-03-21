/**
 * @file gx_texture.c
 * @brief GCN texture format decoder -- stub implementations.
 *
 * Decodes GCN tiled/swizzled texture data into linear RGBA8 suitable for
 * upload to OpenGL via glTexImage2D.
 *
 * References:
 *   - docs/pc_port_design.md Section 5 (Texture Format Translation)
 *   - YAGCD -- GCN texture format documentation
 *   - Dolphin Emulator TextureDecoder_Common.cpp for reference
 *
 * Phase 3 PC port scaffolding -- skeleton only.
 */

#ifdef __MWERKS__
/* GCN build: pcport shim not applicable */
#else

#include "gx_texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* OpenGL constants (used in result structs before GL headers available) */
#define GL_R8                     0x8229
#define GL_RG8                    0x822B
#define GL_RGBA8                  0x8058
#define GL_RED                    0x1903
#define GL_RG                     0x8227
#define GL_RGBA                   0x1908
#define GL_UNSIGNED_BYTE          0x1401
#define GL_UNSIGNED_SHORT_5_6_5   0x8363
#define GL_RGB565_CONST           0x8D62
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1

/* =========================================================================
 * Helper: big-endian 16-bit read
 * ========================================================================= */
static u16 read_be16(const u8* p) {
    return (u16)((p[0] << 8) | p[1]);
}

/* =========================================================================
 * TLUT (palette) decode
 * ========================================================================= */

void gx_tlut_decode_entry(u16 entry, GXTlutFmt fmt,
                          u8* outR, u8* outG, u8* outB, u8* outA) {
    /* TODO: Phase 3d -- Decode a single TLUT palette entry
     *
     * GX_TL_IA8:
     *   High byte = intensity (I), low byte = alpha (A)
     *   *outR = *outG = *outB = (entry >> 8) & 0xFF;
     *   *outA = entry & 0xFF;
     *
     * GX_TL_RGB565:
     *   5 bits red, 6 bits green, 5 bits blue
     *   *outR = ((entry >> 11) & 0x1F) * 255 / 31;
     *   *outG = ((entry >> 5) & 0x3F) * 255 / 63;
     *   *outB = (entry & 0x1F) * 255 / 31;
     *   *outA = 255;
     *
     * GX_TL_RGB5A3:
     *   If MSB=1: 5 bits R, 5 bits G, 5 bits B, A=255
     *   If MSB=0: 4 bits R, 4 bits G, 4 bits B, 3 bits A
     *   (same decode as RGB5A3 texture format)
     */

    (void)entry; (void)fmt;
    *outR = 0; *outG = 0; *outB = 0; *outA = 255;
}

u8 gx_texture_get_swizzle_mode(GXTexFmt format) {
    switch (format) {
        case GX_TF_I4:
        case GX_TF_I8:
            return GX_TEX_SWIZZLE_RRRR;
        case GX_TF_IA4:
        case GX_TF_IA8:
            return GX_TEX_SWIZZLE_RRRA;
        default:
            return GX_TEX_SWIZZLE_RGBA;
    }
}

u32 gx_texture_compute_size(u16 width, u16 height, GXTexFmt format) {
    /* TODO: Phase 3d -- Compute tiled texture size
     *
     * Round width and height up to tile boundaries, then compute:
     *   numTiles = ceilDiv(width, tileW) * ceilDiv(height, tileH)
     *   size = numTiles * bytesPerTile
     *
     * Tile dimensions by format:
     *   I4:     8x8, 32 bytes/tile  -> 4 bpp
     *   I8:     8x4, 32 bytes/tile  -> 8 bpp
     *   IA4:    8x4, 32 bytes/tile  -> 8 bpp
     *   IA8:    4x4, 32 bytes/tile  -> 16 bpp
     *   RGB565: 4x4, 32 bytes/tile  -> 16 bpp
     *   RGB5A3: 4x4, 32 bytes/tile  -> 16 bpp
     *   RGBA8:  4x4, 64 bytes/tile  -> 32 bpp
     *   CI4:    8x8, 32 bytes/tile  -> 4 bpp
     *   CI8:    8x4, 32 bytes/tile  -> 8 bpp
     *   CMPR:   8x8, 32 bytes/tile  -> 4 bpp (compressed)
     */

    u32 tileW = 0, tileH = 0, bytesPerTile = 0;

    switch (format) {
        case GX_TF_I4:     tileW = 8; tileH = 8; bytesPerTile = 32; break;
        case GX_TF_I8:     tileW = 8; tileH = 4; bytesPerTile = 32; break;
        case GX_TF_IA4:    tileW = 8; tileH = 4; bytesPerTile = 32; break;
        case GX_TF_IA8:    tileW = 4; tileH = 4; bytesPerTile = 32; break;
        case GX_TF_RGB565: tileW = 4; tileH = 4; bytesPerTile = 32; break;
        case GX_TF_RGB5A3: tileW = 4; tileH = 4; bytesPerTile = 32; break;
        case GX_TF_RGBA8:  tileW = 4; tileH = 4; bytesPerTile = 64; break;
        case GX_TF_C4:     tileW = 8; tileH = 8; bytesPerTile = 32; break;
        case GX_TF_C8:     tileW = 8; tileH = 4; bytesPerTile = 32; break;
        case GX_TF_CMPR:   tileW = 8; tileH = 8; bytesPerTile = 32; break;
        default: return 0;
    }

    u32 tilesX = (width + tileW - 1) / tileW;
    u32 tilesY = (height + tileH - 1) / tileH;
    return tilesX * tilesY * bytesPerTile;
}

/* =========================================================================
 * Per-format decode stubs
 * ========================================================================= */

s32 gx_texture_decode_I4(const void* src, u16 w, u16 h,
                         GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode I4 (4-bit intensity)
     *
     * Tile layout: 8x8 texels per tile, 4 bits per texel
     * Each tile is 32 bytes (8*8/2 = 32)
     *
     * Algorithm:
     * for each tile (x, y):
     *   for each row r in [0,7]:
     *     for each column c in [0,7] step 2:
     *       byte = src[tileOffset + r*4 + c/2]
     *       pixel0 = (byte >> 4) & 0xF    // high nibble
     *       pixel1 = byte & 0xF           // low nibble
     *       out[(tileY*8+r)*w + (tileX*8+c)]   = pixel0 * 17  // scale 0-15 to 0-255
     *       out[(tileY*8+r)*w + (tileX*8+c+1)] = pixel1 * 17
     *
     * Output: GL_R8 format, swizzle mode RRRR
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);
    out->glInternalFormat = GL_R8;
    out->glFormat = GL_RED;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RRRR;
    return 0;
}

s32 gx_texture_decode_I8(const void* src, u16 w, u16 h,
                         GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode I8 (8-bit intensity)
     *
     * Tile layout: 8x4 texels per tile, 8 bits per texel
     * Each tile is 32 bytes (8*4 = 32)
     *
     * Algorithm:
     * for each tile (x, y):
     *   for each row r in [0,3]:
     *     for each column c in [0,7]:
     *       out[(tileY*4+r)*w + (tileX*8+c)] = src[tileOffset + r*8 + c]
     *
     * Output: GL_R8 format, swizzle mode RRRR
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);
    out->glInternalFormat = GL_R8;
    out->glFormat = GL_RED;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RRRR;
    return 0;
}

s32 gx_texture_decode_IA4(const void* src, u16 w, u16 h,
                          GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode IA4 (4-bit intensity + 4-bit alpha)
     *
     * Tile layout: 8x4 texels per tile, 8 bits per texel (4I + 4A)
     * Each tile is 32 bytes
     *
     * For each texel:
     *   intensity = (byte >> 4) & 0xF -> scale to [0,255]
     *   alpha = byte & 0xF -> scale to [0,255]
     *   out[pixel*2+0] = intensity * 17
     *   out[pixel*2+1] = alpha * 17
     *
     * Output: GL_RG8 format (R=intensity, G=alpha), swizzle RRRA
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h * 2;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);
    out->glInternalFormat = GL_RG8;
    out->glFormat = GL_RG;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RRRA;
    return 0;
}

s32 gx_texture_decode_IA8(const void* src, u16 w, u16 h,
                          GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode IA8 (8-bit intensity + 8-bit alpha)
     *
     * Tile layout: 4x4 texels per tile, 16 bits per texel
     * Each tile is 32 bytes
     *
     * For each texel (big-endian):
     *   alpha = byte0
     *   intensity = byte1
     *   out[pixel*2+0] = intensity
     *   out[pixel*2+1] = alpha
     *
     * Output: GL_RG8, swizzle RRRA
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h * 2;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);
    out->glInternalFormat = GL_RG8;
    out->glFormat = GL_RG;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RRRA;
    return 0;
}

s32 gx_texture_decode_RGB565(const void* src, u16 w, u16 h,
                             GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode RGB565 (16-bit RGB)
     *
     * Tile layout: 4x4 texels per tile, 16 bits per texel
     * Each tile is 32 bytes
     *
     * For each texel (big-endian u16):
     *   r = ((val >> 11) & 0x1F) * 255 / 31
     *   g = ((val >> 5) & 0x3F) * 255 / 63
     *   b = (val & 0x1F) * 255 / 31
     *   a = 255
     *
     * Output: GL_RGBA8 (expand to full RGBA for simplicity)
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h * 4;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0xFF, out->dataSize);
    out->glInternalFormat = GL_RGBA8;
    out->glFormat = GL_RGBA;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RGBA;
    return 0;
}

s32 gx_texture_decode_RGB5A3(const void* src, u16 w, u16 h,
                             GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode RGB5A3 (16-bit, two modes)
     *
     * Tile layout: 4x4 texels per tile, 16 bits per texel
     *
     * For each texel (big-endian u16):
     *   if (val & 0x8000) {  // MSB = 1 -> RGB555, opaque
     *       r = ((val >> 10) & 0x1F) * 255 / 31
     *       g = ((val >> 5) & 0x1F) * 255 / 31
     *       b = (val & 0x1F) * 255 / 31
     *       a = 255
     *   } else {              // MSB = 0 -> RGBA4443
     *       a = ((val >> 12) & 0x07) * 255 / 7
     *       r = ((val >> 8) & 0x0F) * 255 / 15
     *       g = ((val >> 4) & 0x0F) * 255 / 15
     *       b = (val & 0x0F) * 255 / 15
     *   }
     *
     * Output: GL_RGBA8
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h * 4;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0xFF, out->dataSize);
    out->glInternalFormat = GL_RGBA8;
    out->glFormat = GL_RGBA;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RGBA;
    return 0;
}

s32 gx_texture_decode_RGBA8(const void* src, u16 w, u16 h,
                            GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode RGBA8 (32-bit, interleaved AR/GB tiles)
     *
     * Tile layout: 4x4 texels per tile, 64 bytes per tile
     * Each tile stores two 32-byte cache lines:
     *   Cache line 0: AR values (16 texels * 2 bytes = 32)
     *   Cache line 1: GB values (16 texels * 2 bytes = 32)
     *
     * For each texel within a tile:
     *   a = arData[row*4 + col]   (from cache line 0)
     *   r = arData[row*4 + col + 1]
     *   g = gbData[row*4 + col]   (from cache line 1)
     *   b = gbData[row*4 + col + 1]
     *
     * De-interleave and write to linear RGBA output.
     *
     * Output: GL_RGBA8
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h * 4;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0xFF, out->dataSize);
    out->glInternalFormat = GL_RGBA8;
    out->glFormat = GL_RGBA;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RGBA;
    return 0;
}

s32 gx_texture_decode_CI4(const void* src, u16 w, u16 h,
                          const void* tlut, GXTlutFmt tlutFmt,
                          GXDecodedTexture* out) {
    (void)src; (void)tlut; (void)tlutFmt;

    /* TODO: Phase 3d -- Decode CI4 (4-bit color index via palette)
     *
     * Tile layout: 8x8 texels per tile, 4 bits per texel
     * Same tile format as I4, but each value is a palette index.
     *
     * Algorithm:
     * for each texel:
     *   u8 index = (byte >> nibbleShift) & 0x0F
     *   u16 paletteEntry = read_be16(&tlut[index * 2])
     *   gx_tlut_decode_entry(paletteEntry, tlutFmt, &r, &g, &b, &a)
     *   out[pixel*4+0] = r
     *   out[pixel*4+1] = g
     *   out[pixel*4+2] = b
     *   out[pixel*4+3] = a
     *
     * Output: GL_RGBA8
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h * 4;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0xFF, out->dataSize);
    out->glInternalFormat = GL_RGBA8;
    out->glFormat = GL_RGBA;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RGBA;
    return 0;
}

s32 gx_texture_decode_CI8(const void* src, u16 w, u16 h,
                          const void* tlut, GXTlutFmt tlutFmt,
                          GXDecodedTexture* out) {
    (void)src; (void)tlut; (void)tlutFmt;

    /* TODO: Phase 3d -- Decode CI8 (8-bit color index via palette)
     *
     * Tile layout: 8x4 texels per tile, 8 bits per texel
     * Same tile format as I8, but each byte is a palette index.
     *
     * Algorithm:
     * for each texel:
     *   u8 index = src[tileOffset + r*8 + c]
     *   u16 paletteEntry = read_be16(&tlut[index * 2])
     *   gx_tlut_decode_entry(paletteEntry, tlutFmt, &r, &g, &b, &a)
     *
     * Output: GL_RGBA8
     */

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * h * 4;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0xFF, out->dataSize);
    out->glInternalFormat = GL_RGBA8;
    out->glFormat = GL_RGBA;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RGBA;
    return 0;
}

s32 gx_texture_decode_CMPR(const void* src, u16 w, u16 h,
                           GXDecodedTexture* out) {
    (void)src;

    /* TODO: Phase 3d -- Decode CMPR (DXT1-like compressed)
     *
     * GCN CMPR = DXT1 with two differences:
     * 1. Sub-block order: within each 8x8 macro tile, the four 4x4
     *    DXT1 blocks are stored in a Z-order (top-left, top-right,
     *    bottom-left, bottom-right).
     * 2. Byte order: GCN is big-endian; DXT1 on PC is little-endian.
     *    The two 16-bit color endpoints need byte-swapping.
     *    The 32-bit index block also needs byte-swapping.
     *
     * Algorithm:
     * for each 8x8 macro tile:
     *   for each of the 4 sub-blocks (in Z-order):
     *     Read 8 bytes (DXT1 block)
     *     Byte-swap the two 16-bit color values
     *     Byte-swap the 32-bit index value
     *     Write to output in linear DXT1 block order
     *
     * Output: GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
     * Upload via glCompressedTexImage2D
     *
     * Compressed size = (width/4) * (height/4) * 8 bytes per block
     */

    u32 blocksX = (w + 3) / 4;
    u32 blocksY = (h + 3) / 4;
    out->width = w;
    out->height = h;
    out->dataSize = blocksX * blocksY * 8;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);
    out->glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    out->glFormat = 0; /* not used for compressed */
    out->glType = 0;   /* not used for compressed */
    out->isCompressed = 1;
    out->swizzleMode = GX_TEX_SWIZZLE_RGBA;
    return 0;
}

/* =========================================================================
 * Main decode dispatch
 * ========================================================================= */

s32 gx_texture_decode(const void* srcData, u16 width, u16 height,
                      GXTexFmt format,
                      const void* tlutData, GXTlutFmt tlutFmt,
                      u16 tlutEntries,
                      GXDecodedTexture* outResult) {
    (void)tlutEntries;

    if (!srcData || !outResult || width == 0 || height == 0) return -1;

    memset(outResult, 0, sizeof(*outResult));

    switch (format) {
        case GX_TF_I4:
            return gx_texture_decode_I4(srcData, width, height, outResult);
        case GX_TF_I8:
            return gx_texture_decode_I8(srcData, width, height, outResult);
        case GX_TF_IA4:
            return gx_texture_decode_IA4(srcData, width, height, outResult);
        case GX_TF_IA8:
            return gx_texture_decode_IA8(srcData, width, height, outResult);
        case GX_TF_RGB565:
            return gx_texture_decode_RGB565(srcData, width, height, outResult);
        case GX_TF_RGB5A3:
            return gx_texture_decode_RGB5A3(srcData, width, height, outResult);
        case GX_TF_RGBA8:
            return gx_texture_decode_RGBA8(srcData, width, height, outResult);
        case GX_TF_C4:
            return gx_texture_decode_CI4(srcData, width, height,
                                         tlutData, tlutFmt, outResult);
        case GX_TF_C8:
            return gx_texture_decode_CI8(srcData, width, height,
                                         tlutData, tlutFmt, outResult);
        case GX_TF_CMPR:
            return gx_texture_decode_CMPR(srcData, width, height, outResult);
        default:
            printf("[gx_texture] Unsupported texture format: 0x%02X\n", format);
            return -1;
    }
}

s32 gx_texture_decode_mipmap(const void* srcData, u16 mipWidth, u16 mipHeight,
                             GXTexFmt format,
                             const void* tlutData, GXTlutFmt tlutFmt,
                             u16 tlutEntries,
                             GXDecodedTexture* outResult) {
    /* TODO: Phase 3d -- Mipmap decode
     *
     * Same as gx_texture_decode but for a single mip level.
     * The caller provides the mip-level dimensions and data pointer
     * (offset from the base texture data using GStextureHandle::mipOffsets).
     *
     * Upload with: glTexImage2D(GL_TEXTURE_2D, mipLevel, ...)
     */

    return gx_texture_decode(srcData, mipWidth, mipHeight, format,
                             tlutData, tlutFmt, tlutEntries, outResult);
}

void gx_texture_free(GXDecodedTexture* decoded) {
    if (decoded && decoded->data) {
        free(decoded->data);
        decoded->data = (u8*)0;
        decoded->dataSize = 0;
    }
}


#endif /* __MWERKS__ */