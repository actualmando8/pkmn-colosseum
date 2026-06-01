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

static u32 read_be32(const u8* p) {
    return ((u32)p[0] << 24) |
           ((u32)p[1] << 16) |
           ((u32)p[2] << 8) |
           (u32)p[3];
}

static u8 expand_5_to_8(u32 value) {
    return (u8)((value << 3) | (value >> 2));
}

static u8 expand_6_to_8(u32 value) {
    return (u8)((value << 2) | (value >> 4));
}

static void decode_rgb565(u16 value, u8 outRgba[4]) {
    outRgba[0] = expand_5_to_8((value >> 11) & 0x1Fu);
    outRgba[1] = expand_6_to_8((value >> 5) & 0x3Fu);
    outRgba[2] = expand_5_to_8(value & 0x1Fu);
    outRgba[3] = 0xFF;
}

static void decode_dxt1_block(const u8* srcBlock,
                              u8* dstRgba,
                              u32 dstStride) {
    u16 color0 = read_be16(srcBlock + 0);
    u16 color1 = read_be16(srcBlock + 2);
    u32 indices = read_be32(srcBlock + 4);
    u8 palette[4][4];
    u32 y;
    u32 x;

    decode_rgb565(color0, palette[0]);
    decode_rgb565(color1, palette[1]);

    if (color0 > color1) {
        for (x = 0; x < 3u; ++x) {
            palette[2][x] =
                (u8)((((u32)palette[0][x] * 2u) + (u32)palette[1][x]) / 3u);
            palette[3][x] =
                (u8)(((u32)palette[0][x] + ((u32)palette[1][x] * 2u)) / 3u);
        }
        palette[2][3] = 0xFF;
        palette[3][3] = 0xFF;
    } else {
        for (x = 0; x < 3u; ++x) {
            palette[2][x] =
                (u8)((((u32)palette[0][x]) + (u32)palette[1][x]) / 2u);
            palette[3][x] = 0u;
        }
        palette[2][3] = 0xFF;
        palette[3][3] = 0x00;
    }

    for (y = 0; y < 4u; ++y) {
        for (x = 0; x < 4u; ++x) {
            u32 code = (indices >> (2u * ((y * 4u) + x))) & 0x3u;
            u8* dst = dstRgba + (y * dstStride) + (x * 4u);

            dst[0] = palette[code][0];
            dst[1] = palette[code][1];
            dst[2] = palette[code][2];
            dst[3] = palette[code][3];
        }
    }
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
    const u8* srcBytes = (const u8*)src;
    u32 tilesX = ((u32)w + 7u) / 8u;
    u32 tilesY = ((u32)h + 3u) / 4u;
    u32 tileY;
    u32 tileX;

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * (u32)h * 4u;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);

    for (tileY = 0; tileY < tilesY; ++tileY) {
        for (tileX = 0; tileX < tilesX; ++tileX) {
            const u8* tileSrc =
                srcBytes + (((tileY * tilesX) + tileX) * 32u);
            u32 row;

            for (row = 0; row < 4u; ++row) {
                u32 dstY = (tileY * 4u) + row;
                u32 col;

                if (dstY >= h) {
                    continue;
                }

                for (col = 0; col < 8u; ++col) {
                    u32 dstX = (tileX * 8u) + col;
                    u8 intensity;
                    u8* dstPixel;

                    if (dstX >= w) {
                        continue;
                    }

                    intensity = tileSrc[(row * 8u) + col];
                    dstPixel = out->data + ((((u32)dstY * (u32)w) + dstX) * 4u);
                    dstPixel[0] = intensity;
                    dstPixel[1] = intensity;
                    dstPixel[2] = intensity;
                    dstPixel[3] = 0xFF;
                }
            }
        }
    }

    out->glInternalFormat = GL_RGBA8;
    out->glFormat = GL_RGBA;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
    out->swizzleMode = GX_TEX_SWIZZLE_RGBA;
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
    /* RGBA8 ("RGBA32"): 4x4 texels per tile, 64 bytes per tile. The tile is
     * two 32-byte groups: first 32 bytes are AR pairs (A,R per texel), next 32
     * are GB pairs (G,B per texel). Texel index k = row*4 + col, stride 2. */
    const u8* srcBytes = (const u8*)src;
    u32 tilesX = ((u32)w + 3u) / 4u;
    u32 tilesY = ((u32)h + 3u) / 4u;
    u32 tileY;
    u32 tileX;

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * (u32)h * 4u;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);

    for (tileY = 0; tileY < tilesY; ++tileY) {
        for (tileX = 0; tileX < tilesX; ++tileX) {
            const u8* arData = srcBytes + (((tileY * tilesX) + tileX) * 64u);
            const u8* gbData = arData + 32u;
            u32 row;

            for (row = 0; row < 4u; ++row) {
                u32 dstY = (tileY * 4u) + row;
                u32 col;

                if (dstY >= h) {
                    continue;
                }

                for (col = 0; col < 4u; ++col) {
                    u32 dstX = (tileX * 4u) + col;
                    u32 k = (row * 4u) + col;
                    u8* dstPixel;

                    if (dstX >= w) {
                        continue;
                    }

                    dstPixel = out->data + ((((u32)dstY * (u32)w) + dstX) * 4u);
                    dstPixel[0] = arData[(k * 2u) + 1u]; /* R */
                    dstPixel[1] = gbData[(k * 2u) + 0u]; /* G */
                    dstPixel[2] = gbData[(k * 2u) + 1u]; /* B */
                    dstPixel[3] = arData[(k * 2u) + 0u]; /* A */
                }
            }
        }
    }

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
    const u8* srcBytes = (const u8*)src;
    u32 macroTilesX = (w + 7u) / 8u;
    u32 macroTilesY = (h + 7u) / 8u;
    u32 macroY;
    u32 macroX;
    u32 subBlock;

    out->width = w;
    out->height = h;
    out->dataSize = (u32)w * (u32)h * 4u;
    out->data = (u8*)malloc(out->dataSize);
    if (!out->data) return -1;
    memset(out->data, 0, out->dataSize);

    for (macroY = 0; macroY < macroTilesY; ++macroY) {
        for (macroX = 0; macroX < macroTilesX; ++macroX) {
            const u8* macroSrc =
                srcBytes + (((macroY * macroTilesX) + macroX) * 32u);

            for (subBlock = 0; subBlock < 4u; ++subBlock) {
                u32 blockX = (macroX * 8u) + ((subBlock & 1u) * 4u);
                u32 blockY = (macroY * 8u) + ((subBlock >> 1u) * 4u);
                u8 blockPixels[4 * 4 * 4];
                u32 row;

                decode_dxt1_block(macroSrc + (subBlock * 8u),
                                  blockPixels,
                                  4u * 4u);

                for (row = 0; row < 4u; ++row) {
                    u32 dstY = blockY + row;

                    if (dstY >= h || blockX >= w) {
                        continue;
                    }

                    memcpy(out->data + (((dstY * (u32)w) + blockX) * 4u),
                           blockPixels + (row * 4u * 4u),
                           ((blockX + 4u) <= w ? 4u : (u32)w - blockX) * 4u);
                }
            }
        }
    }

    out->glInternalFormat = GL_RGBA8;
    out->glFormat = GL_RGBA;
    out->glType = GL_UNSIGNED_BYTE;
    out->isCompressed = 0;
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
