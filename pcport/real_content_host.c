#include "real_content_host.h"
#include "gx_shim.h"
#include "gx_texture.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PCPORT_FSYS_MAGIC 0x46535953u
#define PCPORT_LZSS_MAGIC 0x4C5A5353u
#define PCPORT_LZSS_HEADER_SIZE 0x10u
#define PCPORT_LZSS_WINDOW_SIZE 0x1000u
#define PCPORT_LZSS_WINDOW_START 0x0FEEu

typedef struct {
    u32 resultOffset;
    u32 keyOffset;
} PCPortArchivePair;

typedef struct {
    u32 maxPosIndex;
    u32 maxNormalIndex;
    u32 maxColorIndex;
    u32 maxTexcoordIndex;
    u32 maxTexcoord1Index;
    u32 totalSubmittedVertices;
    u32 totalPrimitiveCommands;
} PCPortDisplayListStats;

#define PCPORT_SERIALIZED_POBJ_SIZE    0x18u
#define PCPORT_SERIALIZED_VTXDESC_SIZE 0x18u
#define PCPORT_SERIALIZED_JOINT_SIZE   0x40u
#define PCPORT_SERIALIZED_WOBJ_SIZE    0x14u
#define PCPORT_SERIALIZED_COBJ_PERSPECTIVE_SIZE 0x38u
#define PCPORT_SERIALIZED_MOBJ_SIZE    0x18u
#define PCPORT_SERIALIZED_MATERIAL_SIZE 0x14u
#define PCPORT_SERIALIZED_PEDESC_SIZE  0x0Cu
#define PCPORT_SERIALIZED_TOBJ_SIZE    0x5Cu
#define PCPORT_SERIALIZED_IMAGEDESC_SIZE 0x18u
#define PCPORT_SERIALIZED_TEV_SIZE     0x40u
#define PCPORT_MAX_VTXDESC_ENTRIES     16u
#define PCPORT_MAX_JOINT_PATH          16u

#define PCPORT_TEX_COLORMAP_MASK      (0x0Fu << 16)
#define PCPORT_TEX_COLORMAP_MODULATE  (4u << 16)
#define PCPORT_TEX_COLORMAP_REPLACE   (5u << 16)
#define PCPORT_TEX_COLORMAP_PASS      (6u << 16)

#define PCPORT_GX_TEV_MODULATE 0u
#define PCPORT_GX_TEV_REPLACE  3u
#define PCPORT_GX_TEV_PASSCLR  4u

#define PCPORT_TEV_I8_RAMP_SIG0   0x00000000u
#define PCPORT_TEV_I8_RAMP_SIG1   0x00000101u
#define PCPORT_TEV_I8_RAMP_SIG2   0x8580080Fu
#define PCPORT_TEV_I8_RAMP_SIG3   0x07070707u
#define PCPORT_TEV_I8_RAMP_SIG6   0x00000000u
#define PCPORT_TEV_I8_RAMP_SIG7   0x40000077u
#define PCPORT_TEV_I8_RAMP_SIG11  0x00000004u
#define PCPORT_TEV_I8_RAMP_SIG15  0x3F800000u

static u32 ReadBE32(const u8* data) {
    return ((u32)data[0] << 24) |
           ((u32)data[1] << 16) |
           ((u32)data[2] << 8) |
           (u32)data[3];
}

static u16 ReadBE16(const u8* data) {
    return (u16)(((u16)data[0] << 8) | data[1]);
}

static f32 ReadBEFloat(const u8* data) {
    u32 bits = ReadBE32(data);
    f32 value;

    memcpy(&value, &bits, sizeof(value));
    return value;
}

static void ReadPackedColorRGB(u32 value, u8 outColor[4]) {
    outColor[0] = (u8)((value >> 24) & 0xFFu);
    outColor[1] = (u8)((value >> 16) & 0xFFu);
    outColor[2] = (u8)((value >> 8) & 0xFFu);
    outColor[3] = 0xFFu;
}

static void WriteBE32(u8* data, u32 value) {
    data[0] = (u8)((value >> 24) & 0xFF);
    data[1] = (u8)((value >> 16) & 0xFF);
    data[2] = (u8)((value >> 8) & 0xFF);
    data[3] = (u8)(value & 0xFF);
}

static BOOL IsArchiveRangeValid(const PCPortHSDArchive* archive,
                                u32 offset, u32 size) {
    if (archive == NULL || archive->storage == NULL ||
        offset < archive->dataOffset || offset > archive->storageSize) {
        return FALSE;
    }

    return size <= archive->storageSize - offset;
}

static void DecodeI8RampTexture(const u8* src,
                                u16 width,
                                u16 height,
                                const u8 light[4],
                                const u8 dark[4],
                                u8* dstRgba) {
    u32 tilesX = ((u32)width + 7u) / 8u;
    u32 tilesY = ((u32)height + 3u) / 4u;
    u32 tileY;
    u32 tileX;

    for (tileY = 0; tileY < tilesY; ++tileY) {
        for (tileX = 0; tileX < tilesX; ++tileX) {
            const u8* tileSrc = src + (((tileY * tilesX) + tileX) * 32u);
            u32 row;

            for (row = 0; row < 4u; ++row) {
                u32 dstY = (tileY * 4u) + row;
                u32 col;

                if (dstY >= height) {
                    continue;
                }

                for (col = 0; col < 8u; ++col) {
                    u32 dstX = (tileX * 8u) + col;
                    u32 channel;
                    u8 intensity;
                    u8* dstPixel;

                    if (dstX >= width) {
                        continue;
                    }

                    intensity = tileSrc[(row * 8u) + col];
                    dstPixel = dstRgba + ((((u32)dstY * (u32)width) + dstX) * 4u);
                    for (channel = 0; channel < 3u; ++channel) {
                        u32 darkValue = dark[channel];
                        u32 lightValue = light[channel];

                        dstPixel[channel] = (u8)(darkValue +
                                                 (((lightValue - darkValue) * intensity + 127u) / 255u));
                    }
                    dstPixel[3] = 0xFFu;
                }
            }
        }
    }
}

static void TranslateTextureTevPayload(const PCPortHSDArchive* archive,
                                       u32 tevOffset,
                                       u32 textureFormat,
                                       PCPortTranslatedTev* outTev) {
    const u8* tevData;
    u32 i;

    if (outTev == NULL) {
        return;
    }

    memset(outTev, 0, sizeof(*outTev));

    if (archive == NULL || tevOffset == 0u ||
        !IsArchiveRangeValid(archive, tevOffset, PCPORT_SERIALIZED_TEV_SIZE)) {
        return;
    }

    tevData = archive->storage + tevOffset;
    outTev->archiveOffset = tevOffset;
    outTev->rawWordCount = 16u;
    for (i = 0; i < 16u; ++i) {
        outTev->rawWords[i] = ReadBE32(tevData + (i * 4u));
    }

    if (textureFormat == GX_TF_I8 &&
        outTev->rawWords[0] == PCPORT_TEV_I8_RAMP_SIG0 &&
        outTev->rawWords[1] == PCPORT_TEV_I8_RAMP_SIG1 &&
        outTev->rawWords[2] == PCPORT_TEV_I8_RAMP_SIG2 &&
        outTev->rawWords[3] == PCPORT_TEV_I8_RAMP_SIG3 &&
        outTev->rawWords[6] == PCPORT_TEV_I8_RAMP_SIG6 &&
        outTev->rawWords[7] == PCPORT_TEV_I8_RAMP_SIG7 &&
        outTev->rawWords[11] == PCPORT_TEV_I8_RAMP_SIG11 &&
        outTev->rawWords[15] == PCPORT_TEV_I8_RAMP_SIG15) {
        outTev->kind = PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP;
        ReadPackedColorRGB(outTev->rawWords[4], outTev->rampLight);
        ReadPackedColorRGB(outTev->rawWords[5], outTev->rampDark);
    }
}

static BOOL ResolveTextureCoordIdFromRawSrc(u32 rawSrc, u8* outCoordId) {
    if (outCoordId == NULL) {
        return FALSE;
    }

    if (rawSrc >= 4u && rawSrc <= 11u) {
        *outCoordId = (u8)(rawSrc - 4u);
        return TRUE;
    }

    if (rawSrc <= 7u) {
        *outCoordId = (u8)rawSrc;
        return TRUE;
    }

    return FALSE;
}

static BOOL HasZeroColorRamp(const PCPortTranslatedTexture* texture) {
    if (texture == NULL) {
        return FALSE;
    }

    return texture->tev.rampLight[0] == 0u &&
           texture->tev.rampLight[1] == 0u &&
           texture->tev.rampLight[2] == 0u &&
           texture->tev.rampDark[0] == 0u &&
           texture->tev.rampDark[1] == 0u &&
           texture->tev.rampDark[2] == 0u;
}

static BOOL IsNoTevDirectSampleFormat(u32 textureFormat) {
    switch (textureFormat) {
    case GX_TF_CMPR:
    case GX_TF_RGBA8:
    case GX_TF_C4:  /* CI4 (palettized) */
    case GX_TF_C8:  /* CI8 (palettized) */
        return TRUE;
    default:
        return FALSE;
    }
}

static u8 ClassifyTextureExpStageKind(const PCPortTranslatedTexture* texture,
                                      u8 coordId) {
    if (texture == NULL) {
        return PCPORT_TEXP_STAGE_NONE;
    }

    /* A directly-sampleable format (CMPR/RGBA8/CI4/CI8) is a plain texture
     * stage. A non-NULL TEV is fine as long as it is NOT the special I8
     * colour-ramp kind: the generic TEV just selects a blend mode, which the
     * pipeline already applies via tevMode. Requiring tevArchiveOffset==0 here
     * is what rejected the RGBA8/palettized ground/ruins nodes (they carry a
     * plain modulate TEV) and left them rendering as a flat material colour. */
    if (IsNoTevDirectSampleFormat(texture->format) &&
        texture->tev.kind == PCPORT_TRANSLATED_TEV_NONE) {
        return PCPORT_TEXP_STAGE_DIRECT_SAMPLE;
    }

    if (texture->format == GX_TF_I8 &&
        texture->tev.kind == PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP) {
        return PCPORT_TEXP_STAGE_I8_RAMP_SAMPLE;
    }

    if (texture->format == GX_TF_I8 &&
        coordId == 1u &&
        (texture->flags & 0x0Fu) == 0u &&
        (texture->flags & PCPORT_TEX_COLORMAP_MASK) == PCPORT_TEX_COLORMAP_MODULATE &&
        (texture->flags & 0x00F00000u) == 0x00300000u &&
        HasZeroColorRamp(texture)) {
        return PCPORT_TEXP_STAGE_I8_MASK_MODULATE;
    }

    return PCPORT_TEXP_STAGE_NONE;
}

static u8 ClassifyTextureExpKindFromParsedChain(const PCPortParsedTextureNodeChain* chain) {
    if (chain == NULL || chain->nodeCount == 0u) {
        return PCPORT_TEXTURE_EXP_KIND_NONE;
    }

    if (chain->nodeCount == 1u) {
        if (chain->stageKinds[0] == PCPORT_TEXP_STAGE_DIRECT_SAMPLE) {
            return PCPORT_TEXTURE_EXP_KIND_DIRECT_SAMPLE;
        }
        if (chain->stageKinds[0] == PCPORT_TEXP_STAGE_I8_RAMP_SAMPLE) {
            return PCPORT_TEXTURE_EXP_KIND_I8_RAMP;
        }
        return PCPORT_TEXTURE_EXP_KIND_NONE;
    }

    if (chain->nodeCount == 2u &&
        chain->stageKinds[0] == PCPORT_TEXP_STAGE_I8_RAMP_SAMPLE &&
        chain->stageKinds[1] == PCPORT_TEXP_STAGE_I8_MASK_MODULATE) {
        return PCPORT_TEXTURE_EXP_KIND_I8_RAMP_MASK;
    }

    return PCPORT_TEXTURE_EXP_KIND_NONE;
}

static BOOL TranslateTextureFromArchiveCommon(const PCPortHSDArchive* archive,
                                              u32 tobjArchiveOffset,
                                              BOOL allowNext,
                                              PCPortTranslatedTexture* outTexture);

BOOL PCPort_ParseTextureNodeChainFromArchiveBE(const PCPortHSDArchive* archive,
                                               u32 tobjArchiveOffset,
                                               u32 maxNodes,
                                               PCPortParsedTextureNodeChain* outChain) {
    u32 currentOffset = tobjArchiveOffset;
    u32 nodeIndex = 0u;

    if (outChain == NULL || maxNodes == 0u || maxNodes > PCPORT_TEXP_STAGE_MAX ||
        !IsArchiveRangeValid(archive, tobjArchiveOffset, PCPORT_SERIALIZED_TOBJ_SIZE)) {
        return FALSE;
    }

    memset(outChain, 0, sizeof(*outChain));
    outChain->headArchiveOffset = tobjArchiveOffset;

    while (currentOffset != 0u && nodeIndex < maxNodes) {
        PCPortTranslatedTexture* node = &outChain->nodes[nodeIndex];

        if (!TranslateTextureFromArchiveCommon(archive,
                                               currentOffset,
                                               TRUE,
                                               node) ||
            !node->hasCoordId) {
            return FALSE;
        }

        outChain->coordIds[nodeIndex] = node->coordId;
        outChain->stageKinds[nodeIndex] = ClassifyTextureExpStageKind(node,
                                                                      node->coordId);
        ++nodeIndex;
        currentOffset = ReadBE32(archive->storage + currentOffset + 0x04);
    }

    if (nodeIndex == 0u || currentOffset != 0u) {
        return FALSE;
    }

    outChain->nodeCount = (u8)nodeIndex;
    outChain->kind = ClassifyTextureExpKindFromParsedChain(outChain);
    return TRUE;
}

static int GetIndexByteCount(u32 attrType) {
    switch (attrType) {
    case GX_INDEX8:
        return 1;
    case GX_INDEX16:
        return 2;
    default:
        return 0;
    }
}

static BOOL ParseSerializedVtxDesc(const PCPortHSDArchive* archive,
                                   u32 offset,
                                   HSD_VtxDescList* outDesc,
                                   u32* outSourceVertexOffset) {
    u32 strideWord;

    if (outDesc == NULL || outSourceVertexOffset == NULL ||
        !IsArchiveRangeValid(archive, offset, PCPORT_SERIALIZED_VTXDESC_SIZE)) {
        return FALSE;
    }

    memset(outDesc, 0, sizeof(*outDesc));
    outDesc->attr = ReadBE32(archive->storage + offset + 0x00);
    outDesc->attr_type = ReadBE32(archive->storage + offset + 0x04);
    outDesc->comp_cnt = ReadBE32(archive->storage + offset + 0x08);
    outDesc->comp_type = ReadBE32(archive->storage + offset + 0x0C);
    strideWord = ReadBE32(archive->storage + offset + 0x10);
    outDesc->frac = (u8)((strideWord >> 24) & 0xFFu);
    outDesc->stride = (u16)(strideWord & 0xFFFFu);
    *outSourceVertexOffset = ReadBE32(archive->storage + offset + 0x14);
    return TRUE;
}

static BOOL ScanDisplayListIndices(const u8* displayList,
                                   u32 displayListCapacity,
                                   const HSD_VtxDescList* verts,
                                   PCPortDisplayListStats* stats,
                                   u32* outConsumedBytes) {
    const u8* cursor = displayList;
    const u8* end = displayList + displayListCapacity;

    if (displayList == NULL || verts == NULL || stats == NULL) {
        return FALSE;
    }

    memset(stats, 0, sizeof(*stats));

    while (cursor < end) {
        u16 vertexCount;
        const HSD_VtxDescList* v;
        u32 i;

        if ((u32)(end - cursor) < 3u) {
            return FALSE;
        }

        if (cursor[0] == 0u && cursor[1] == 0u && cursor[2] == 0u &&
            stats->totalPrimitiveCommands != 0u) {
            break;
        }

        ++cursor; /* command byte: primitive + vtxfmt */
        vertexCount = (u16)(((u16)cursor[0] << 8) | cursor[1]);
        cursor += 2;
        if (vertexCount == 0) {
            return FALSE;
        }

        stats->totalSubmittedVertices += vertexCount;
        ++stats->totalPrimitiveCommands;

        for (i = 0; i < vertexCount; ++i) {
            for (v = verts; v->attr != GX_VA_NULL; ++v) {
                int indexSize = GetIndexByteCount(v->attr_type);
                u32 index;

                if (indexSize == 0 || (u32)(end - cursor) < (u32)indexSize) {
                    return FALSE;
                }

                if (indexSize == 1) {
                    index = cursor[0];
                } else {
                    index = (u32)(((u16)cursor[0] << 8) | cursor[1]);
                }
                cursor += indexSize;

                switch (v->attr) {
                case GX_VA_POS:
                    if (index > stats->maxPosIndex) {
                        stats->maxPosIndex = index;
                    }
                    break;
                case GX_VA_NRM:
                    if (index > stats->maxNormalIndex) {
                        stats->maxNormalIndex = index;
                    }
                    break;
                case GX_VA_CLR0:
                    if (index > stats->maxColorIndex) {
                        stats->maxColorIndex = index;
                    }
                    break;
                case GX_VA_TEX0:
                    if (index > stats->maxTexcoordIndex) {
                        stats->maxTexcoordIndex = index;
                    }
                    break;
                case GX_VA_TEX1:
                    if (index > stats->maxTexcoord1Index) {
                        stats->maxTexcoord1Index = index;
                    }
                    break;
                default:
                    return FALSE;
                }
            }
        }
    }

    if (outConsumedBytes != NULL) {
        *outConsumedBytes = (u32)(cursor - displayList);
    }

    return stats->totalPrimitiveCommands != 0u;
}

static BOOL TranslateVertexArray(const PCPortHSDArchive* archive,
                                 HSD_VtxDescList* desc,
                                 u32 sourceOffset,
                                 u32 elementCount,
                                 PCPortTranslatedPObj* outPObj) {
    size_t totalBytes;
    u8* data;
    u32 i;

    if (archive == NULL || desc == NULL || outPObj == NULL ||
        elementCount == 0 || desc->stride == 0) {
        return FALSE;
    }

    totalBytes = (size_t)elementCount * (size_t)desc->stride;
    if (totalBytes == 0 ||
        !IsArchiveRangeValid(archive, sourceOffset, (u32)totalBytes)) {
        return FALSE;
    }

    data = (u8*)malloc(totalBytes);
    if (data == NULL) {
        return FALSE;
    }

    memcpy(data, archive->storage + sourceOffset, totalBytes);

    switch (desc->attr) {
    case GX_VA_POS:
        if (desc->comp_type != GX_F32 ||
            (desc->comp_cnt != GX_POS_XY && desc->comp_cnt != GX_POS_XYZ)) {
            free(data);
            return FALSE;
        }

        for (i = 0; i < elementCount; ++i) {
            u8* dst = data + ((size_t)i * desc->stride);
            const u8* src = archive->storage + sourceOffset + ((size_t)i * desc->stride);
            f32 x = ReadBEFloat(src + 0);
            f32 y = ReadBEFloat(src + 4);
            f32 z = 0.0f;

            memcpy(dst + 0, &x, sizeof(x));
            memcpy(dst + 4, &y, sizeof(y));
            if (desc->comp_cnt == GX_POS_XYZ) {
                z = ReadBEFloat(src + 8);
                memcpy(dst + 8, &z, sizeof(z));
            }

            if (i == 0 || x < outPObj->minPosition[0]) {
                outPObj->minPosition[0] = x;
            }
            if (i == 0 || y < outPObj->minPosition[1]) {
                outPObj->minPosition[1] = y;
            }
            if (i == 0 || z < outPObj->minPosition[2]) {
                outPObj->minPosition[2] = z;
            }
            if (i == 0 || x > outPObj->maxPosition[0]) {
                outPObj->maxPosition[0] = x;
            }
            if (i == 0 || y > outPObj->maxPosition[1]) {
                outPObj->maxPosition[1] = y;
            }
            if (i == 0 || z > outPObj->maxPosition[2]) {
                outPObj->maxPosition[2] = z;
            }
        }

        outPObj->positionData = data;
        desc->vertex = data;
        return TRUE;

    case GX_VA_NRM:
        /* Normal (raw bytes already copied above). The host lights via face
         * normals, so the per-vertex normal is not consumed at draw time -- the
         * array only needs to exist (correctly sized) so the indexed display-list
         * decode advances past the normal index and stays aligned. */
        outPObj->normalData = data;
        desc->vertex = data;
        return TRUE;

    case GX_VA_CLR0:
        if (desc->comp_type != GX_RGBA8 && desc->comp_type != GX_RGB8) {
            free(data);
            return FALSE;
        }

        outPObj->colorData = data;
        desc->vertex = data;
        return TRUE;

    case GX_VA_TEX0:
    case GX_VA_TEX1:
        if (desc->comp_type != GX_F32 ||
            (desc->comp_cnt != GX_TEX_S && desc->comp_cnt != GX_TEX_ST)) {
            free(data);
            return FALSE;
        }

        for (i = 0; i < elementCount; ++i) {
            u8* dst = data + ((size_t)i * desc->stride);
            const u8* src = archive->storage + sourceOffset + ((size_t)i * desc->stride);
            f32 s = ReadBEFloat(src + 0);

            memcpy(dst + 0, &s, sizeof(s));
            if (desc->comp_cnt == GX_TEX_ST) {
                f32 t = ReadBEFloat(src + 4);

                memcpy(dst + 4, &t, sizeof(t));
            }
        }

        if (desc->attr == GX_VA_TEX0) {
            outPObj->texcoordData = data;
        } else {
            outPObj->texcoord1Data = data;
        }
        desc->vertex = data;
        return TRUE;

        default:
            free(data);
            return FALSE;
    }
}

static void LoadIdentityMtx(f32 mtx[3][4]) {
    memset(mtx, 0, sizeof(f32) * 12u);
    mtx[0][0] = 1.0f;
    mtx[1][1] = 1.0f;
    mtx[2][2] = 1.0f;
}

static void MultiplyAffineMtx(const f32 a[3][4],
                              const f32 b[3][4],
                              f32 out[3][4]) {
    f32 result[3][4];
    u32 row;
    u32 col;

    for (row = 0; row < 3u; ++row) {
        for (col = 0; col < 3u; ++col) {
            result[row][col] = (a[row][0] * b[0][col]) +
                               (a[row][1] * b[1][col]) +
                               (a[row][2] * b[2][col]);
        }

        result[row][3] = (a[row][0] * b[0][3]) +
                         (a[row][1] * b[1][3]) +
                         (a[row][2] * b[2][3]) +
                         a[row][3];
    }

    memcpy(out, result, sizeof(result));
}

static void BuildJointLocalMtx(const PCPortHSDArchive* archive,
                               u32 jointOffset,
                               f32 out[3][4]) {
    f32 rx = ReadBEFloat(archive->storage + jointOffset + 0x14);
    f32 ry = ReadBEFloat(archive->storage + jointOffset + 0x18);
    f32 rz = ReadBEFloat(archive->storage + jointOffset + 0x1C);
    f32 sx = ReadBEFloat(archive->storage + jointOffset + 0x20);
    f32 sy = ReadBEFloat(archive->storage + jointOffset + 0x24);
    f32 sz = ReadBEFloat(archive->storage + jointOffset + 0x28);
    f32 tx = ReadBEFloat(archive->storage + jointOffset + 0x2C);
    f32 ty = ReadBEFloat(archive->storage + jointOffset + 0x30);
    f32 tz = ReadBEFloat(archive->storage + jointOffset + 0x34);
    f32 cx = cosf(rx);
    f32 sxRot = sinf(rx);
    f32 cy = cosf(ry);
    f32 syRot = sinf(ry);
    f32 cz = cosf(rz);
    f32 szRot = sinf(rz);

    out[0][0] = (cz * cy) * sx;
    out[0][1] = ((cz * syRot * sxRot) - (szRot * cx)) * sy;
    out[0][2] = ((cz * syRot * cx) + (szRot * sxRot)) * sz;
    out[0][3] = tx;

    out[1][0] = (szRot * cy) * sx;
    out[1][1] = ((szRot * syRot * sxRot) + (cz * cx)) * sy;
    out[1][2] = ((szRot * syRot * cx) - (cz * sxRot)) * sz;
    out[1][3] = ty;

    out[2][0] = (-syRot) * sx;
    out[2][1] = (cy * sxRot) * sy;
    out[2][2] = (cy * cx) * sz;
    out[2][3] = tz;
}

static f32 Vec3Dot(const f32 a[3], const f32 b[3]) {
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

static void Vec3Cross(const f32 a[3], const f32 b[3], f32 out[3]) {
    out[0] = (a[1] * b[2]) - (a[2] * b[1]);
    out[1] = (a[2] * b[0]) - (a[0] * b[2]);
    out[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

static BOOL Vec3Normalize(f32 v[3]) {
    f32 length = sqrtf(Vec3Dot(v, v));

    if (!(length > 0.00001f)) {
        return FALSE;
    }

    v[0] /= length;
    v[1] /= length;
    v[2] /= length;
    return TRUE;
}

static BOOL LoadSerializedWObjPosition(const PCPortHSDArchive* archive,
                                       u32 wobjOffset,
                                       f32 outPos[3]) {
    if (outPos == NULL ||
        !IsArchiveRangeValid(archive, wobjOffset, PCPORT_SERIALIZED_WOBJ_SIZE)) {
        return FALSE;
    }

    outPos[0] = ReadBEFloat(archive->storage + wobjOffset + 0x04);
    outPos[1] = ReadBEFloat(archive->storage + wobjOffset + 0x08);
    outPos[2] = ReadBEFloat(archive->storage + wobjOffset + 0x0C);
    return TRUE;
}

static BOOL BuildCameraViewMatrix(const f32 eye[3],
                                  const f32 interest[3],
                                  const f32 upHint[3],
                                  f32 out[3][4]) {
    f32 forward[3] = {
        interest[0] - eye[0],
        interest[1] - eye[1],
        interest[2] - eye[2]
    };
    f32 side[3];
    f32 up[3] = { upHint[0], upHint[1], upHint[2] };

    if (!Vec3Normalize(forward)) {
        return FALSE;
    }

    if (!Vec3Normalize(up)) {
        up[0] = 0.0f;
        up[1] = 1.0f;
        up[2] = 0.0f;
    }

    Vec3Cross(forward, up, side);
    if (!Vec3Normalize(side)) {
        up[0] = 0.0f;
        up[1] = 0.0f;
        up[2] = 1.0f;
        Vec3Cross(forward, up, side);
        if (!Vec3Normalize(side)) {
            return FALSE;
        }
    }

    Vec3Cross(side, forward, up);
    if (!Vec3Normalize(up)) {
        return FALSE;
    }

    out[0][0] = side[0];
    out[0][1] = side[1];
    out[0][2] = side[2];
    out[0][3] = -Vec3Dot(side, eye);

    out[1][0] = up[0];
    out[1][1] = up[1];
    out[1][2] = up[2];
    out[1][3] = -Vec3Dot(up, eye);

    out[2][0] = -forward[0];
    out[2][1] = -forward[1];
    out[2][2] = -forward[2];
    out[2][3] = Vec3Dot(forward, eye);
    return TRUE;
}

static BOOL BuildCameraProjectionMatrix(f32 fovDegrees,
                                        f32 aspect,
                                        f32 nearZ,
                                        f32 farZ,
                                        f32 out[4][4]) {
    f32 radians;
    f32 cotangent;
    u32 row;
    u32 col;

    if (!(nearZ > 0.0f) || !(farZ > nearZ) || !(aspect > 0.0f)) {
        return FALSE;
    }

    radians = fovDegrees * (3.14159265358979323846f / 180.0f);
    cotangent = 1.0f / tanf(radians * 0.5f);

    for (row = 0; row < 4u; ++row) {
        for (col = 0; col < 4u; ++col) {
            out[row][col] = 0.0f;
        }
    }

    out[0][0] = cotangent / aspect;
    out[1][1] = cotangent;
    out[2][2] = (farZ + nearZ) / (nearZ - farZ);
    out[2][3] = (2.0f * farZ * nearZ) / (nearZ - farZ);
    out[3][2] = -1.0f;
    return TRUE;
}

static BOOL FindJointPath(const PCPortHSDArchive* archive,
                          u32 currentJointOffset,
                          u32 targetJointOffset,
                          u32* pathOffsets,
                          u32 depth,
                          u32* outPathLength) {
    u32 childOffset;
    u32 nextOffset;

    if (pathOffsets == NULL || outPathLength == NULL ||
        depth >= PCPORT_MAX_JOINT_PATH ||
        !IsArchiveRangeValid(archive, currentJointOffset, PCPORT_SERIALIZED_JOINT_SIZE)) {
        return FALSE;
    }

    pathOffsets[depth] = currentJointOffset;
    if (currentJointOffset == targetJointOffset) {
        *outPathLength = depth + 1u;
        return TRUE;
    }

    childOffset = ReadBE32(archive->storage + currentJointOffset + 0x08);
    if (childOffset != 0u &&
        FindJointPath(archive, childOffset, targetJointOffset,
                      pathOffsets, depth + 1u, outPathLength)) {
        return TRUE;
    }

    nextOffset = ReadBE32(archive->storage + currentJointOffset + 0x0C);
    if (nextOffset != 0u &&
        FindJointPath(archive, nextOffset, targetJointOffset,
                      pathOffsets, depth, outPathLength)) {
        return TRUE;
    }

    return FALSE;
}

static u8* LoadFileBytes(const char* path, u32* outSize) {
    FILE* file;
    long fileSize;
    u8* data;

    if (outSize == NULL) {
        return NULL;
    }

    file = fopen(path, "rb");
    if (file == NULL) {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    fileSize = ftell(file);
    if (fileSize <= 0 || fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }

    data = (u8*)malloc((size_t)fileSize);
    if (data == NULL) {
        fclose(file);
        return NULL;
    }

    if (fread(data, 1, (size_t)fileSize, file) != (size_t)fileSize) {
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);
    *outSize = (u32)fileSize;
    return data;
}

static BOOL DecompressLZSS(const u8* src, u32 srcSize, u8* dst, u32 dstSize) {
    u8 window[PCPORT_LZSS_WINDOW_SIZE];
    u32 srcPos = PCPORT_LZSS_HEADER_SIZE;
    u32 dstPos = 0;
    u32 windowPos = PCPORT_LZSS_WINDOW_START;
    u32 flags = 0;

    if (src == NULL || dst == NULL || srcSize < PCPORT_LZSS_HEADER_SIZE) {
        return FALSE;
    }

    memset(window, 0, sizeof(window));

    while (srcPos < srcSize && dstPos < dstSize) {
        flags >>= 1;
        if ((flags & 0x100) == 0) {
            if (srcPos >= srcSize) {
                break;
            }
            flags = (u32)src[srcPos++] | 0xFF00u;
        }

        if ((flags & 1) != 0) {
            u8 literal;

            if (srcPos >= srcSize) {
                break;
            }

            literal = src[srcPos++];
            dst[dstPos++] = literal;
            window[windowPos] = literal;
            windowPos = (windowPos + 1) & 0x0FFFu;
        } else {
            u8 byte1;
            u8 byte2;
            u32 offset;
            u32 length;
            u32 j;

            if (srcPos + 1 >= srcSize) {
                break;
            }

            byte1 = src[srcPos++];
            byte2 = src[srcPos++];
            offset = (u32)byte1 | (((u32)byte2 & 0xF0u) << 4);
            length = ((u32)byte2 & 0x0Fu) + 2u;

            for (j = 0; j <= length && dstPos < dstSize; ++j) {
                u8 copyByte = window[(offset + j) & 0x0FFFu];
                dst[dstPos++] = copyByte;
                window[windowPos] = copyByte;
                windowPos = (windowPos + 1) & 0x0FFFu;
            }
        }
    }

    return dstPos == dstSize;
}

static BOOL FindFsysEntry(const u8* fsysData, u32 fsysSize,
                          const char* memberName, u32* outEntryOffset) {
    u32 entryCount;
    u32 stringTableOffset;
    u32 entryTableOffset;
    u32 i;

    if (fsysData == NULL || memberName == NULL || outEntryOffset == NULL ||
        fsysSize < 0x20) {
        return FALSE;
    }

    if (ReadBE32(fsysData) != PCPORT_FSYS_MAGIC) {
        return FALSE;
    }

    entryCount = ReadBE32(fsysData + 0x08);
    stringTableOffset = ReadBE32(fsysData + 0x18);
    if (stringTableOffset + 4 > fsysSize) {
        return FALSE;
    }

    entryTableOffset = ReadBE32(fsysData + stringTableOffset);
    if (entryTableOffset >= fsysSize) {
        return FALSE;
    }

    for (i = 0; i < entryCount; ++i) {
        u32 entryOffset = ReadBE32(fsysData + entryTableOffset + (i * 4));
        u32 nameOffset;

        if (entryOffset + 0x28 > fsysSize) {
            continue;
        }

        nameOffset = ReadBE32(fsysData + entryOffset + 0x24);
        if (nameOffset >= fsysSize) {
            continue;
        }

        if (strcmp((const char*)(fsysData + nameOffset), memberName) == 0) {
            *outEntryOffset = entryOffset;
            return TRUE;
        }
    }

    return FALSE;
}

BOOL PCPort_LoadFsysMember(const char* fsysPath, const char* memberName,
                           u8** outData, u32* outSize) {
    u8* fsysData;
    u32 fsysSize = 0;
    u32 entryOffset;
    u32 dataOffset;
    u32 compressedSize;
    u8* output;

    if (outData == NULL || outSize == NULL) {
        return FALSE;
    }

    *outData = NULL;
    *outSize = 0;

    fsysData = LoadFileBytes(fsysPath, &fsysSize);
    if (fsysData == NULL) {
        return FALSE;
    }

    if (!FindFsysEntry(fsysData, fsysSize, memberName, &entryOffset)) {
        free(fsysData);
        return FALSE;
    }

    dataOffset = ReadBE32(fsysData + entryOffset + 0x04);
    /* entry+0x08 is the DECOMPRESSED size for LZSS members (and equals the
     * on-disk size for stored members). The true on-disk byte count of a
     * compressed member lives in its LZSS header (dataOffset+0x08), so the
     * up-front check only requires the header word to be in range; each branch
     * below validates the real on-disk length. Bounds-checking against
     * compressedSize here falsely rejected strongly-compressed members
     * (e.g. title.fsys:logo_demo, decompressed 0x15BC6B). */
    compressedSize = ReadBE32(fsysData + entryOffset + 0x08);

    if (dataOffset >= fsysSize ||
        dataOffset + PCPORT_LZSS_HEADER_SIZE > fsysSize) {
        free(fsysData);
        return FALSE;
    }

    if (ReadBE32(fsysData + dataOffset) == PCPORT_LZSS_MAGIC) {
        u32 lzssOutputSize;
        u32 lzssInputSize;

        lzssOutputSize = ReadBE32(fsysData + dataOffset + 0x04);
        lzssInputSize = ReadBE32(fsysData + dataOffset + 0x08);
        if (lzssOutputSize == 0 ||
            lzssInputSize < PCPORT_LZSS_HEADER_SIZE ||
            dataOffset + lzssInputSize > fsysSize) {
            free(fsysData);
            return FALSE;
        }

        output = (u8*)malloc((size_t)lzssOutputSize);
        if (output == NULL) {
            free(fsysData);
            return FALSE;
        }

        if (!DecompressLZSS(fsysData + dataOffset, lzssInputSize,
                            output, lzssOutputSize)) {
            free(output);
            free(fsysData);
            return FALSE;
        }

        *outData = output;
        *outSize = lzssOutputSize;
    } else {
        u32 copySize = compressedSize;

        if (dataOffset + copySize > fsysSize) {
            free(fsysData);
            return FALSE;
        }

        output = (u8*)malloc((size_t)copySize);
        if (output == NULL) {
            free(fsysData);
            return FALSE;
        }

        memcpy(output, fsysData + dataOffset, copySize);
        *outData = output;
        *outSize = copySize;
    }

    free(fsysData);
    return TRUE;
}

/* Decompress the FSYS member at `entryOffset` into a fresh malloc'd buffer.
 * Returns the buffer (caller frees) + size, or NULL. Same per-member logic as
 * PCPort_LoadFsysMember, used by the scene-member scan below. */
static u8* DecompressMemberAt(const u8* fsysData, u32 fsysSize,
                              u32 entryOffset, u32* outSize) {
    u32 dataOffset = ReadBE32(fsysData + entryOffset + 0x04);
    u32 storedSize = ReadBE32(fsysData + entryOffset + 0x08);
    u8* output;

    if (dataOffset >= fsysSize ||
        dataOffset + PCPORT_LZSS_HEADER_SIZE > fsysSize) {
        return NULL;
    }
    if (ReadBE32(fsysData + dataOffset) == PCPORT_LZSS_MAGIC) {
        u32 outN = ReadBE32(fsysData + dataOffset + 0x04);
        u32 inN  = ReadBE32(fsysData + dataOffset + 0x08);
        if (outN == 0 || inN < PCPORT_LZSS_HEADER_SIZE ||
            dataOffset + inN > fsysSize) {
            return NULL;
        }
        output = (u8*)malloc((size_t)outN);
        if (output == NULL) {
            return NULL;
        }
        if (!DecompressLZSS(fsysData + dataOffset, inN, output, outN)) {
            free(output);
            return NULL;
        }
        *outSize = outN;
        return output;
    }
    if (dataOffset + storedSize > fsysSize || storedSize == 0) {
        return NULL;
    }
    output = (u8*)malloc((size_t)storedSize);
    if (output == NULL) {
        return NULL;
    }
    memcpy(output, fsysData + dataOffset, storedSize);
    *outSize = storedSize;
    return output;
}

/* True if `data` is an HSD archive (fileSize word == size) exposing public `sym`. */
static BOOL HSDArchiveHasPublic(const u8* data, u32 size, const char* sym) {
    u32 dataSize, nreloc, npub, next, pubOff, extOff, strOff, k;
    if (size < 0x20 || ReadBE32(data) != size) {
        return FALSE;
    }
    dataSize = ReadBE32(data + 0x04);
    nreloc   = ReadBE32(data + 0x08);
    npub     = ReadBE32(data + 0x0C);
    next     = ReadBE32(data + 0x10);
    pubOff = 0x20 + dataSize + nreloc * 4;
    extOff = pubOff + npub * 8;
    strOff = extOff + next * 8;
    if (strOff > size) {
        return FALSE;
    }
    for (k = 0; k < npub; ++k) {
        u32 entryOff = pubOff + k * 8;
        u32 key, nameOff;
        if (entryOff + 8 > size) {
            break;
        }
        key = ReadBE32(data + entryOff + 4);
        nameOff = strOff + key;
        if (nameOff >= size) {
            continue;
        }
        if (strcmp((const char*)(data + nameOff), sym) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

/* Field-map .fsys archives contain MULTIPLE members of the same name; only one is
 * the renderable HSD scene archive. Return the LARGEST member that is an HSD archive
 * exposing a "scene_data" public symbol (the map geometry; smaller scene_data members
 * are shared object sets like ippan_m_b1). Lets the field loader reuse the same
 * scene_data -> RenderJointTree path as the title without knowing member indices. */
BOOL PCPort_LoadFsysSceneMember(const char* fsysPath, u8** outData, u32* outSize) {
    u8* fsysData;
    u32 fsysSize = 0;
    u32 entryCount, stringTableOffset, entryTableOffset, i;
    u8* best = NULL;
    u32 bestSize = 0;

    if (outData == NULL || outSize == NULL) {
        return FALSE;
    }
    *outData = NULL;
    *outSize = 0;

    fsysData = LoadFileBytes(fsysPath, &fsysSize);
    if (fsysData == NULL) {
        return FALSE;
    }
    if (fsysSize < 0x20 || ReadBE32(fsysData) != PCPORT_FSYS_MAGIC) {
        free(fsysData);
        return FALSE;
    }

    entryCount = ReadBE32(fsysData + 0x08);
    stringTableOffset = ReadBE32(fsysData + 0x18);
    if (stringTableOffset + 4 > fsysSize) {
        free(fsysData);
        return FALSE;
    }
    entryTableOffset = ReadBE32(fsysData + stringTableOffset);
    if (entryTableOffset >= fsysSize) {
        free(fsysData);
        return FALSE;
    }

    for (i = 0; i < entryCount; ++i) {
        u32 entryOffset, memSize = 0;
        u8* mem;
        if (entryTableOffset + i * 4 + 4 > fsysSize) {
            break;
        }
        entryOffset = ReadBE32(fsysData + entryTableOffset + (i * 4));
        if (entryOffset + 0x28 > fsysSize) {
            continue;
        }
        mem = DecompressMemberAt(fsysData, fsysSize, entryOffset, &memSize);
        if (mem == NULL) {
            continue;
        }
        if (memSize > bestSize && HSDArchiveHasPublic(mem, memSize, "scene_data")) {
            free(best);
            best = mem;
            bestSize = memSize;
        } else {
            free(mem);
        }
    }

    free(fsysData);
    if (best == NULL) {
        return FALSE;
    }
    *outData = best;
    *outSize = bestSize;
    return TRUE;
}

/* True if `data` looks like a WZX collision mesh (see include/game/gs_colsys.h
 * + pcport/field_collision.c). Top-level header is {u32 vertDataOff@0,
 * u32 groupCount@4}; groupCount group records of 0x40 bytes start at vertDataOff,
 * each with 6 self-relative submesh slots at +0x24..+0x38. A submesh is
 * {u32 vtxOff, u32 triCount, ...}; its triangles are 0x34 bytes (3 Vec3f + normal).
 * We confirm the header shape and that at least one submesh resolves to
 * room-scale finite vertex coordinates -- enough to disambiguate the WZX from
 * the other raw members (which all share the map's member name). */
static BOOL WZXLooksValid(const u8* d, u32 n) {
    u32 vertOff, groupCount, g, validTris = 0;
    if (n < 0x48) {
        return FALSE;
    }
    /* Not an HSD archive (those have fileSize word == n). */
    if (ReadBE32(d) == n) {
        return FALSE;
    }
    vertOff = ReadBE32(d + 0x00);
    groupCount = ReadBE32(d + 0x04);
    if (vertOff < 8u || vertOff >= n || groupCount == 0u || groupCount > 256u) {
        return FALSE;
    }
    if (vertOff + groupCount * 0x40u > n) {
        return FALSE;
    }
    for (g = 0; g < groupCount; ++g) {
        u32 grpBase = vertOff + g * 0x40u;
        u32 slot;
        for (slot = 0; slot < 6u; ++slot) {
            u32 so = ReadBE32(d + grpBase + 0x24u + slot * 4u);
            u32 vtx, cnt, k;
            if (so == 0u || so + 0x10u > n) {
                continue;
            }
            vtx = ReadBE32(d + so + 0x00u);
            cnt = ReadBE32(d + so + 0x04u);
            if (vtx == 0u || cnt == 0u || cnt > 100000u) {
                continue;
            }
            if (vtx + cnt * 0x34u > n) {
                continue;
            }
            /* Sanity-check the first vertex of the first triangle. */
            for (k = 0; k < 3u; ++k) {
                union { u32 u; f32 f; } v;
                f32 a;
                v.u = ReadBE32(d + vtx + k * 4u);
                a = v.f;
                if (!(a == a) || a > 1.0e5f || a < -1.0e5f) {
                    goto next_slot;
                }
            }
            validTris += cnt;
        next_slot:;
        }
    }
    return validTris >= 8u;
}

/* Field-map .fsys archives bundle the WZX collision mesh as a raw member that
 * (like the scene) shares the map's member name -- so it can't be fetched by
 * name. Scan all members and return the one matching the WZX signature.
 * Caller frees *outData via PCPort_FreeBuffer. */
BOOL PCPort_LoadFsysWZXMember(const char* fsysPath, u8** outData, u32* outSize) {
    u8* fsysData;
    u32 fsysSize = 0;
    u32 entryCount, stringTableOffset, entryTableOffset, i;

    if (outData == NULL || outSize == NULL) {
        return FALSE;
    }
    *outData = NULL;
    *outSize = 0;

    fsysData = LoadFileBytes(fsysPath, &fsysSize);
    if (fsysData == NULL) {
        return FALSE;
    }
    if (fsysSize < 0x20 || ReadBE32(fsysData) != PCPORT_FSYS_MAGIC) {
        free(fsysData);
        return FALSE;
    }

    entryCount = ReadBE32(fsysData + 0x08);
    stringTableOffset = ReadBE32(fsysData + 0x18);
    if (stringTableOffset + 4 > fsysSize) {
        free(fsysData);
        return FALSE;
    }
    entryTableOffset = ReadBE32(fsysData + stringTableOffset);
    if (entryTableOffset >= fsysSize) {
        free(fsysData);
        return FALSE;
    }

    for (i = 0; i < entryCount; ++i) {
        u32 entryOffset, memSize = 0;
        u8* mem;
        if (entryTableOffset + i * 4 + 4 > fsysSize) {
            break;
        }
        entryOffset = ReadBE32(fsysData + entryTableOffset + (i * 4));
        if (entryOffset + 0x28 > fsysSize) {
            continue;
        }
        mem = DecompressMemberAt(fsysData, fsysSize, entryOffset, &memSize);
        if (mem == NULL) {
            continue;
        }
        if (WZXLooksValid(mem, memSize)) {
            free(fsysData);
            *outData = mem;
            *outSize = memSize;
            return TRUE;
        }
        free(mem);
    }

    free(fsysData);
    return FALSE;
}

void PCPort_FreeBuffer(void* buffer) {
    free(buffer);
}

BOOL PCPort_HSDArchiveParseBE(PCPortHSDArchive* archive,
                              const void* data, u32 size) {
    const u8* src = (const u8*)data;
    u32 fileSize;
    u32 dataSize;
    u32 relocCount;
    u32 publicCount;
    u32 externCount;
    u32 relocOffset;
    u32 publicOffset;
    u32 externOffset;
    u32 stringOffset;

    if (archive == NULL || src == NULL || size < 0x20u) {
        return FALSE;
    }

    memset(archive, 0, sizeof(*archive));

    fileSize = ReadBE32(src + 0x00);
    dataSize = ReadBE32(src + 0x04);
    relocCount = ReadBE32(src + 0x08);
    publicCount = ReadBE32(src + 0x0C);
    externCount = ReadBE32(src + 0x10);

    if (fileSize != size) {
        return FALSE;
    }

    relocOffset = 0x20u + dataSize;
    publicOffset = relocOffset + (relocCount * 4u);
    externOffset = publicOffset + (publicCount * 8u);
    stringOffset = externOffset + (externCount * 8u);

    if (relocOffset > size || publicOffset > size || externOffset > size ||
        stringOffset > size) {
        return FALSE;
    }

    archive->storage = (u8*)malloc((size_t)size);
    if (archive->storage == NULL) {
        return FALSE;
    }

    memcpy(archive->storage, src, size);
    archive->storageSize = size;
    archive->dataSize = dataSize;
    archive->relocCount = relocCount;
    archive->publicCount = publicCount;
    archive->externCount = externCount;
    archive->dataOffset = 0x20u;
    archive->relocOffset = relocOffset;
    archive->publicOffset = publicOffset;
    archive->externOffset = externOffset;
    archive->stringOffset = stringOffset;

    {
        u32 i;

        for (i = 0; i < relocCount; ++i) {
            u32 relocEntryOffset = relocOffset + (i * 4u);
            u32 fieldOffset = ReadBE32(archive->storage + relocEntryOffset);
            u32 absoluteFieldOffset = archive->dataOffset + fieldOffset;
            u32 value;

            if (absoluteFieldOffset + 4u > archive->storageSize) {
                PCPort_HSDArchiveDestroy(archive);
                return FALSE;
            }

            value = ReadBE32(archive->storage + absoluteFieldOffset);
            WriteBE32(archive->storage + absoluteFieldOffset,
                      value + archive->dataOffset);
        }
    }

    return TRUE;
}

/* =========================================================================
 * BE->LE swizzle for running the GAME'S OWN HSD pipeline.
 *
 * The archive scalars are GameCube big-endian and PCPort_HSDArchiveParseBE
 * leaves relocated pointer fields as BE (storage-relative+dataOffset). To feed
 * the data to the game's real HSD_*LoadDesc (which read native LE structs +
 * native host pointers) we, ONCE, walk the descriptor graph and:
 *   (1) byte-swap every non-pointer multibyte scalar (u16/u32/f32) in place;
 *   (2) convert every relocated pointer field to a native host pointer.
 * Pointer fields are identified structurally (per HSD_*Desc layout); during the
 * scalar walk they still hold BE offsets, so we follow them by offset. After
 * the walk, ApplyHostRelocations rewrites ALL relocated fields (from the reloc
 * table) to native pointers. Visited-set keyed on storage offset handles shared
 * + cyclic descriptors. NOTE: vertex-array payloads + display lists are handled
 * separately (the GX shim already reads BE display-list indices; vertex data
 * swap is a follow-up step). This is the game's data driving the game's code --
 * no re-created rendering. */

static void Swap16InPlace(u8* p) { u8 t = p[0]; p[0] = p[1]; p[1] = t; }
static void Swap32InPlace(u8* p) {
    u8 t; t = p[0]; p[0] = p[3]; p[3] = t; t = p[1]; p[1] = p[2]; p[2] = t;
}

#define PCPORT_SWIZ_MAX_NODES 16384u
typedef struct {
    u8* base;            /* archive->storage */
    u32 size;
    u32 dataOffset;
    u32 visited[PCPORT_SWIZ_MAX_NODES];
    u32 visitedCount;
} PCPortSwizCtx;

static BOOL SwizMarkVisited(PCPortSwizCtx* c, u32 off) {
    u32 i;
    if (off == 0u || off >= c->size) {
        return TRUE; /* NULL / OOB -> treat as already-handled (skip) */
    }
    for (i = 0; i < c->visitedCount; ++i) {
        if (c->visited[i] == off) {
            return TRUE;
        }
    }
    if (c->visitedCount < PCPORT_SWIZ_MAX_NODES) {
        c->visited[c->visitedCount++] = off;
    }
    return FALSE;
}

/* Read a (still-BE) relocated pointer field as a storage offset (0 == NULL). */
static u32 SwizChildOff(PCPortSwizCtx* c, u32 fieldAbsOff) {
    if (fieldAbsOff + 4u > c->size) {
        return 0u;
    }
    return ReadBE32(c->base + fieldAbsOff); /* already dataOffset+offset, abs */
}

/* Forward decls (mutually recursive graph walk). */
static void SwizJoint(PCPortSwizCtx* c, u32 off);
static void SwizDObjDesc(PCPortSwizCtx* c, u32 off);
static void SwizMObjDesc(PCPortSwizCtx* c, u32 off);
static void SwizTObjDesc(PCPortSwizCtx* c, u32 off);
static void SwizPObjDesc(PCPortSwizCtx* c, u32 off);
static void SwizAObjDesc(PCPortSwizCtx* c, u32 off);
static void SwizFObjDesc(PCPortSwizCtx* c, u32 off);

static void SwizImageDesc(PCPortSwizCtx* c, u32 off) {
    u8* p;
    if (SwizMarkVisited(c, off)) return;
    p = c->base + off;
    Swap16InPlace(p + 0x4);   /* width  */
    Swap16InPlace(p + 0x6);   /* height */
    Swap32InPlace(p + 0x8);   /* format */
    Swap32InPlace(p + 0xC);   /* mipmap */
    Swap32InPlace(p + 0x10);  /* minLOD */
    Swap32InPlace(p + 0x14);  /* maxLOD */
    /* +0x0 image_ptr = pointer (handled by ApplyHostRelocations) */
}

static void SwizTObjDesc(PCPortSwizCtx* c, u32 off) {
    u8* p;
    u32 i;
    if (SwizMarkVisited(c, off)) return;
    p = c->base + off;
    Swap32InPlace(p + 0x8);   /* id  */
    Swap32InPlace(p + 0xC);   /* src */
    for (i = 0; i < 9u; ++i) Swap32InPlace(p + 0x10 + i * 4u); /* rotate/scale/translate */
    Swap32InPlace(p + 0x34);  /* wrap_s */
    Swap32InPlace(p + 0x38);  /* wrap_t */
    /* +0x3C repeat_s, +0x3D repeat_t : u8, no swap */
    Swap32InPlace(p + 0x40);  /* blend_flags */
    Swap32InPlace(p + 0x44);  /* blending */
    Swap32InPlace(p + 0x48);  /* magFilt */
    SwizImageDesc(c, SwizChildOff(c, off + 0x4C)); /* imagedesc */
    /* next(0x4), tlutdesc(0x50), lod(0x54), tev(0x58): pointers */
    SwizTObjDesc(c, SwizChildOff(c, off + 0x4));    /* next */
}

static void SwizMObjDesc(PCPortSwizCtx* c, u32 off) {
    u8* p;
    u32 matOff;
    if (SwizMarkVisited(c, off)) return;
    p = c->base + off;
    Swap32InPlace(p + 0x4);   /* rendermode */
    SwizTObjDesc(c, SwizChildOff(c, off + 0x8)); /* texdesc */
    matOff = SwizChildOff(c, off + 0xC);         /* mat (HSD_Material) */
    if (!SwizMarkVisited(c, matOff)) {
        u8* m = c->base + matOff;
        Swap32InPlace(m + 0x0);  /* ambient  */
        Swap32InPlace(m + 0x4);  /* diffuse  */
        Swap32InPlace(m + 0x8);  /* specular */
        Swap32InPlace(m + 0xC);  /* alpha    */
        Swap32InPlace(m + 0x10); /* shininess */
    }
    /* renderdesc(0x10), pedesc(0x14, all u8) : no scalar swap */
}

static void SwizFObjDesc(PCPortSwizCtx* c, u32 off) {
    u8* p;
    if (SwizMarkVisited(c, off)) return;
    p = c->base + off;
    Swap32InPlace(p + 0x4);   /* length */
    Swap32InPlace(p + 0x8);   /* startframe */
    /* +0xC..0xF: u8 type/frac_value/frac_slope/dummy0 : no swap; +0x10 ad ptr */
    SwizFObjDesc(c, SwizChildOff(c, off + 0x0)); /* next */
}

static void SwizAObjDesc(PCPortSwizCtx* c, u32 off) {
    u8* p;
    if (SwizMarkVisited(c, off)) return;
    p = c->base + off;
    Swap32InPlace(p + 0x0);   /* flags */
    Swap32InPlace(p + 0x4);   /* end_frame */
    Swap32InPlace(p + 0xC);   /* obj_id */
    SwizFObjDesc(c, SwizChildOff(c, off + 0x8)); /* fobjdesc */
}

static void SwizPObjDesc(PCPortSwizCtx* c, u32 off) {
    u8* p;
    u32 vlist;
    if (SwizMarkVisited(c, off)) return;
    p = c->base + off;
    Swap16InPlace(p + 0xC);   /* flags */
    Swap16InPlace(p + 0xE);   /* n_display */
    /* verts(0x8) -> VtxDescList array; display(0x10) bytes (indices read BE by
     * the shim); u.joint(0x14) pointer. */
    vlist = SwizChildOff(c, off + 0x8);
    if (!SwizMarkVisited(c, vlist) && vlist != 0u) {
        u32 e = vlist;
        /* iterate entries until attr (BE u32) == GX_VA_NULL (0xFF) */
        while (e + 0x18u <= c->size) {
            u32 attr = ReadBE32(c->base + e);
            if (attr == 0xFFu) { Swap32InPlace(c->base + e); break; }
            Swap32InPlace(c->base + e + 0x0);  /* attr */
            Swap32InPlace(c->base + e + 0x4);  /* attr_type */
            Swap32InPlace(c->base + e + 0x8);  /* comp_cnt */
            Swap32InPlace(c->base + e + 0xC);  /* comp_type */
            /* +0x10 frac u8; +0x12 stride u16; +0x14 vertex ptr */
            Swap16InPlace(c->base + e + 0x12); /* stride */
            e += 0x18u;
        }
    }
    SwizPObjDesc(c, SwizChildOff(c, off + 0x4)); /* next */
}

static void SwizDObjDesc(PCPortSwizCtx* c, u32 off) {
    if (SwizMarkVisited(c, off)) return;
    /* class_name(0), next(4), mobjdesc(8), pobjdesc(C): all pointers */
    SwizMObjDesc(c, SwizChildOff(c, off + 0x8));
    SwizPObjDesc(c, SwizChildOff(c, off + 0xC));
    SwizDObjDesc(c, SwizChildOff(c, off + 0x4)); /* next */
}

static void SwizJoint(PCPortSwizCtx* c, u32 off) {
    u8* p;
    u32 i;
    if (SwizMarkVisited(c, off)) return;
    p = c->base + off;
    Swap32InPlace(p + 0x4);   /* flags */
    for (i = 0; i < 9u; ++i) Swap32InPlace(p + 0x14 + i * 4u); /* rotation/scale/position */
    /* class_name(0), child(8), next(C), u.dobjdesc(10), mtx(38), robjdesc(3C) ptrs */
    SwizDObjDesc(c, SwizChildOff(c, off + 0x10)); /* u.dobjdesc */
    SwizJoint(c, SwizChildOff(c, off + 0x8));     /* child */
    SwizJoint(c, SwizChildOff(c, off + 0xC));     /* next */
    /* robjdesc(0x3C): flags swap handled lazily if present (skip union for now) */
}

/* Convert every relocated pointer field (from the reloc table) from its BE
 * storage-relative value to a native host pointer (storage + value). Must run
 * AFTER the scalar walk (which reads pointers as BE offsets to traverse). */
static void PCPort_HSDApplyHostRelocations(PCPortHSDArchive* a) {
    u32 i;
    for (i = 0; i < a->relocCount; ++i) {
        u32 relocEntryOffset = a->relocOffset + (i * 4u);
        u32 fieldOffset, fieldAbs, value;
        if (relocEntryOffset + 4u > a->storageSize) break;
        fieldOffset = ReadBE32(a->storage + relocEntryOffset);
        fieldAbs = a->dataOffset + fieldOffset;
        if (fieldAbs + 4u > a->storageSize) continue;
        value = ReadBE32(a->storage + fieldAbs);     /* dataOffset+offset (abs) */
        if (value == 0u || value >= a->storageSize) continue;
        /* native host pointer (32-bit build: pointer fits in u32) */
        *(u32*)(a->storage + fieldAbs) = (u32)(uintptr_t)(a->storage + value);
    }
}

/* Public: prepare a parsed archive's scene-data joint graph for the game's HSD
 * pipeline. `rootJointOffset` is the storage offset of the scene root HSD_Joint
 * (scene_data -> branch -> jointList). Returns the root joint as a native ptr. */
/* Smoke test: load a scene member, resolve the root joint (BE), swizzle, then
 * read back NATIVE-LE fields to confirm the swap produced sane values. Prints a
 * report. Verifies the swizzle math independently of the load/render. */
void PCPort_HSDSwizzleSmoke(const char* fsysPath, const char* memberName) {
    u8* data = NULL;
    u32 size = 0;
    PCPortHSDArchive archive;
    const u8* sceneData;
    u32 sceneOffset = 0, branchOff, jointListOff, rootOff;
    void* rootPtr;

    if (!PCPort_LoadFsysMember(fsysPath, memberName, &data, &size)) {
        printf("[hsd-swiz] load %s:%s FAILED\n", fsysPath, memberName);
        return;
    }
    if (!PCPort_HSDArchiveParseBE(&archive, data, size)) {
        printf("[hsd-swiz] parse FAILED\n");
        free(data);
        return;
    }
    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive, "scene_data", &sceneOffset);
    if (sceneData == NULL) { printf("[hsd-swiz] no scene_data\n"); goto done; }
    branchOff = ReadBE32(sceneData + 0x00);
    jointListOff = ReadBE32(archive.storage + branchOff + 0x00);
    rootOff = ReadBE32(archive.storage + jointListOff + 0x00);
    printf("[hsd-swiz] scene=0x%X branch=0x%X jointList=0x%X root=0x%X relocs=%u\n",
           sceneOffset, branchOff, jointListOff, rootOff, archive.relocCount);

    rootPtr = PCPort_SwizzleSceneForHSD(&archive, rootOff);
    if (rootPtr == NULL) { printf("[hsd-swiz] swizzle FAILED\n"); goto done; }

    {
        /* Read the swizzled root joint as native LE. */
        u8* j = (u8*)rootPtr;
        u32 flags = *(u32*)(j + 0x4);
        f32 rx = *(f32*)(j + 0x14), ry = *(f32*)(j + 0x18), rz = *(f32*)(j + 0x1C);
        f32 sx = *(f32*)(j + 0x20), sy = *(f32*)(j + 0x24), sz = *(f32*)(j + 0x28);
        f32 px = *(f32*)(j + 0x2C), py = *(f32*)(j + 0x30), pz = *(f32*)(j + 0x34);
        u32 dobj = *(u32*)(j + 0x10);   /* now native host pointer */
        u8* stack[256]; int sp = 0; int scanned = 0;
        printf("[hsd-swiz] ROOT flags=0x%08X rot=(%.3f,%.3f,%.3f) scale=(%.3f,%.3f,%.3f) pos=(%.1f,%.1f,%.1f) dobj=%p\n",
               flags, rx, ry, rz, sx, sy, sz, px, py, pz, (void*)(uintptr_t)dobj);
        /* Walk the joint tree (host pointers now) to the first dobj-bearing joint. */
        stack[sp++] = j;
        while (sp > 0 && dobj == 0u && scanned < 4096) {
            u8* cur = stack[--sp]; u32 ch, nx;
            ++scanned;
            dobj = *(u32*)(cur + 0x10);
            if (dobj != 0u) { j = cur; break; }
            ch = *(u32*)(cur + 0x8); nx = *(u32*)(cur + 0xC);
            if (nx != 0u && sp < 256) stack[sp++] = (u8*)(uintptr_t)nx;
            if (ch != 0u && sp < 256) stack[sp++] = (u8*)(uintptr_t)ch;
        }
        printf("[hsd-swiz] first dobj-joint after %d nodes: dobj=%p\n", scanned, (void*)(uintptr_t)dobj);
        if (dobj != 0u) {
            u8* d = (u8*)(uintptr_t)dobj;
            u32 mobj = *(u32*)(d + 0x8);
            if (mobj != 0u) {
                u8* m = (u8*)(uintptr_t)mobj;
                u32 rendermode = *(u32*)(m + 0x4);
                u32 mat = *(u32*)(m + 0xC);
                u32 tobj = *(u32*)(m + 0x8);
                printf("[hsd-swiz] MObj rendermode=0x%X mat=%p tobj=%p\n",
                       rendermode, (void*)(uintptr_t)mat, (void*)(uintptr_t)tobj);
                if (mat != 0u) {
                    u8* mm = (u8*)(uintptr_t)mat;
                    printf("[hsd-swiz]   Material ambient=0x%08X diffuse=0x%08X alpha=%.3f shininess=%.2f\n",
                           *(u32*)(mm + 0x0), *(u32*)(mm + 0x4), *(f32*)(mm + 0xC), *(f32*)(mm + 0x10));
                }
                if (tobj != 0u) {
                    u8* tt = (u8*)(uintptr_t)tobj;
                    printf("[hsd-swiz]   TObj scale=(%.3f,%.3f,%.3f) wrap_s=%u img=%p\n",
                           *(f32*)(tt + 0x1C), *(f32*)(tt + 0x20), *(f32*)(tt + 0x24),
                           *(u32*)(tt + 0x34), (void*)(uintptr_t)*(u32*)(tt + 0x4C));
                }
            }
        }
    }
done:
    PCPort_HSDArchiveDestroy(&archive);
    free(data);
}

void* PCPort_SwizzleSceneForHSD(PCPortHSDArchive* archive, u32 rootJointOffset) {
    PCPortSwizCtx ctx;
    if (archive == NULL || archive->storage == NULL ||
        rootJointOffset == 0u || rootJointOffset >= archive->storageSize) {
        return NULL;
    }
    memset(&ctx, 0, sizeof(ctx));
    ctx.base = archive->storage;
    ctx.size = archive->storageSize;
    ctx.dataOffset = archive->dataOffset;
    SwizJoint(&ctx, rootJointOffset);            /* (1) scalar swap */
    PCPort_HSDApplyHostRelocations(archive);     /* (2) pointers -> host */
    return (void*)(archive->storage + rootJointOffset);
}

void PCPort_HSDArchiveDestroy(PCPortHSDArchive* archive) {
    if (archive == NULL) {
        return;
    }

    free(archive->storage);
    memset(archive, 0, sizeof(*archive));
}

const void* PCPort_HSDArchiveGetPublicAddress(const PCPortHSDArchive* archive,
                                              const char* name,
                                              u32* outArchiveOffset) {
    u32 i;

    if (archive == NULL || archive->storage == NULL || name == NULL) {
        return NULL;
    }

    for (i = 0; i < archive->publicCount; ++i) {
        const u8* pairPtr = archive->storage + archive->publicOffset + (i * 8u);
        PCPortArchivePair pair;
        const char* keyName;

        pair.resultOffset = ReadBE32(pairPtr + 0);
        pair.keyOffset = ReadBE32(pairPtr + 4);
        if (archive->stringOffset + pair.keyOffset >= archive->storageSize) {
            continue;
        }

        keyName = (const char*)(archive->storage + archive->stringOffset +
                                pair.keyOffset);
        if (strcmp(keyName, name) == 0) {
            u32 absoluteOffset = archive->dataOffset + pair.resultOffset;

            if (absoluteOffset >= archive->storageSize) {
                return NULL;
            }

            if (outArchiveOffset != NULL) {
                *outArchiveOffset = absoluteOffset;
            }

            return archive->storage + absoluteOffset;
        }
    }

    return NULL;
}

u32 PCPort_ReadBigEndianU32(const void* data) {
    return ReadBE32((const u8*)data);
}

void PCPort_DestroyTranslatedPObj(PCPortTranslatedPObj* pobj) {
    if (pobj == NULL) {
        return;
    }

    free(pobj->verts);
    free(pobj->displayList);
    free(pobj->positionData);
    free(pobj->colorData);
    free(pobj->texcoordData);
    free(pobj->texcoord1Data);
    free(pobj->normalData);
    memset(pobj, 0, sizeof(*pobj));
}

BOOL PCPort_TranslatePObjFromArchiveBE(const PCPortHSDArchive* archive,
                                       u32 pobjArchiveOffset,
                                       PCPortTranslatedPObj* outPObj) {
    HSD_VtxDescList parsedVerts[PCPORT_MAX_VTXDESC_ENTRIES];
    u32 sourceVertexOffsets[PCPORT_MAX_VTXDESC_ENTRIES];
    PCPortDisplayListStats stats;
    u32 nextOffset;
    u32 vertsOffset;
    u32 displayOffset;
    u32 displayCapacity;
    u32 flagsAndDisplayCount;
    u16 flags;
    u16 serializedDisplayCount;
    u32 translatedDisplaySize = 0;
    u32 entryCount = 0;
    u32 i;

    if (archive == NULL || outPObj == NULL ||
        !IsArchiveRangeValid(archive, pobjArchiveOffset, PCPORT_SERIALIZED_POBJ_SIZE)) {
        return FALSE;
    }

    nextOffset = ReadBE32(archive->storage + pobjArchiveOffset + 0x04);
    vertsOffset = ReadBE32(archive->storage + pobjArchiveOffset + 0x08);
    flagsAndDisplayCount = ReadBE32(archive->storage + pobjArchiveOffset + 0x0C);
    displayOffset = ReadBE32(archive->storage + pobjArchiveOffset + 0x10);
    flags = (u16)(flagsAndDisplayCount >> 16);
    serializedDisplayCount = (u16)(flagsAndDisplayCount & 0xFFFFu);

    if (nextOffset != 0u || serializedDisplayCount == 0u ||
        !IsArchiveRangeValid(archive, vertsOffset, PCPORT_SERIALIZED_VTXDESC_SIZE) ||
        displayOffset >= pobjArchiveOffset) {
        if (getenv("PCPORT_SKIN_DEBUG") != NULL) {
            fprintf(stderr,
                    "[skin] pobj@0x%X flags=0x%04X type=%u early-reject next=0x%X dispCount=%u\n",
                    pobjArchiveOffset, flags, (flags >> 12) & 3u,
                    nextOffset, serializedDisplayCount);
        }
        return FALSE;
    }

    displayCapacity = pobjArchiveOffset - displayOffset;
    if (!IsArchiveRangeValid(archive, displayOffset, displayCapacity)) {
        return FALSE;
    }

    memset(outPObj, 0, sizeof(*outPObj));
    memset(parsedVerts, 0, sizeof(parsedVerts));
    memset(sourceVertexOffsets, 0, sizeof(sourceVertexOffsets));

    for (entryCount = 0; entryCount + 1u < PCPORT_MAX_VTXDESC_ENTRIES; ++entryCount) {
        if (!ParseSerializedVtxDesc(archive,
                                    vertsOffset + (entryCount * PCPORT_SERIALIZED_VTXDESC_SIZE),
                                    &parsedVerts[entryCount],
                                    &sourceVertexOffsets[entryCount])) {
            PCPort_DestroyTranslatedPObj(outPObj);
            return FALSE;
        }

        if (parsedVerts[entryCount].attr == GX_VA_NULL) {
            break;
        }

        if ((parsedVerts[entryCount].attr != GX_VA_POS &&
             parsedVerts[entryCount].attr != GX_VA_NRM &&
             parsedVerts[entryCount].attr != GX_VA_CLR0 &&
             parsedVerts[entryCount].attr != GX_VA_TEX0 &&
             parsedVerts[entryCount].attr != GX_VA_TEX1) ||
            GetIndexByteCount(parsedVerts[entryCount].attr_type) == 0 ||
            parsedVerts[entryCount].stride == 0) {
            if (getenv("PCPORT_SKIN_DEBUG") != NULL) {
                fprintf(stderr,
                        "[skin] pobj@0x%X flags=0x%04X type=%u reject attr=%u attr_type=%u stride=%u\n",
                        pobjArchiveOffset, flags, (flags >> 12) & 3u,
                        parsedVerts[entryCount].attr,
                        parsedVerts[entryCount].attr_type,
                        parsedVerts[entryCount].stride);
            }
            PCPort_DestroyTranslatedPObj(outPObj);
            return FALSE;
        }
    }

    if (entryCount == 0u || parsedVerts[entryCount].attr != GX_VA_NULL) {
        PCPort_DestroyTranslatedPObj(outPObj);
        return FALSE;
    }

    outPObj->verts = (HSD_VtxDescList*)calloc(entryCount + 1u, sizeof(HSD_VtxDescList));
    if (outPObj->verts == NULL) {
        PCPort_DestroyTranslatedPObj(outPObj);
        return FALSE;
    }

    memcpy(outPObj->verts, parsedVerts, (entryCount + 1u) * sizeof(HSD_VtxDescList));

    if (!ScanDisplayListIndices(archive->storage + displayOffset,
                                displayCapacity,
                                outPObj->verts,
                                &stats,
                                &translatedDisplaySize)) {
        PCPort_DestroyTranslatedPObj(outPObj);
        return FALSE;
    }

    if (translatedDisplaySize == 0u) {
        PCPort_DestroyTranslatedPObj(outPObj);
        return FALSE;
    }

    for (i = 0; i < entryCount; ++i) {
        u32 usedCount;

        switch (outPObj->verts[i].attr) {
        case GX_VA_POS:
            usedCount = stats.maxPosIndex + 1u;
            break;
        case GX_VA_NRM:
            usedCount = stats.maxNormalIndex + 1u;
            break;
        case GX_VA_CLR0:
            usedCount = stats.maxColorIndex + 1u;
            break;
        case GX_VA_TEX0:
            usedCount = stats.maxTexcoordIndex + 1u;
            break;
        case GX_VA_TEX1:
            usedCount = stats.maxTexcoord1Index + 1u;
            break;
        default:
            PCPort_DestroyTranslatedPObj(outPObj);
            return FALSE;
        }

        if (!TranslateVertexArray(archive,
                                  &outPObj->verts[i],
                                  sourceVertexOffsets[i],
                                  usedCount,
                                  outPObj)) {
            PCPort_DestroyTranslatedPObj(outPObj);
            return FALSE;
        }
    }

    outPObj->displayList = (u8*)malloc(translatedDisplaySize);
    if (outPObj->displayList == NULL) {
        PCPort_DestroyTranslatedPObj(outPObj);
        return FALSE;
    }

    memcpy(outPObj->displayList, archive->storage + displayOffset, translatedDisplaySize);
    outPObj->pobj.verts = outPObj->verts;
    outPObj->pobj.flags = flags;
    outPObj->pobj.n_display = (u16)translatedDisplaySize;
    outPObj->pobj.display = outPObj->displayList;
    outPObj->sourceArchiveOffset = pobjArchiveOffset;
    outPObj->totalSubmittedVertices = stats.totalSubmittedVertices;
    outPObj->totalPrimitiveCommands = stats.totalPrimitiveCommands;
    return TRUE;
}

BOOL PCPort_TranslateJointChainToMatrixBE(const PCPortHSDArchive* archive,
                                          u32 rootJointArchiveOffset,
                                          u32 targetJointArchiveOffset,
                                          PCPortTranslatedJointTransform* outTransform) {
    u32 pathOffsets[PCPORT_MAX_JOINT_PATH];
    u32 pathLength = 0;
    u32 i;

    if (outTransform == NULL || archive == NULL || archive->storage == NULL ||
        !IsArchiveRangeValid(archive, rootJointArchiveOffset, PCPORT_SERIALIZED_JOINT_SIZE) ||
        !IsArchiveRangeValid(archive, targetJointArchiveOffset, PCPORT_SERIALIZED_JOINT_SIZE)) {
        return FALSE;
    }

    memset(outTransform, 0, sizeof(*outTransform));
    if (!FindJointPath(archive,
                       rootJointArchiveOffset,
                       targetJointArchiveOffset,
                       pathOffsets,
                       0u,
                       &pathLength) ||
        pathLength == 0u) {
        return FALSE;
    }

    LoadIdentityMtx(outTransform->modelMatrix);
    for (i = 0; i < pathLength; ++i) {
        f32 localMtx[3][4];

        BuildJointLocalMtx(archive, pathOffsets[i], localMtx);
        MultiplyAffineMtx(outTransform->modelMatrix,
                          localMtx,
                          outTransform->modelMatrix);
    }

    outTransform->rootArchiveOffset = rootJointArchiveOffset;
    outTransform->jointArchiveOffset = targetJointArchiveOffset;
    return TRUE;
}

BOOL PCPort_TranslatePerspectiveCameraFromArchiveBE(const PCPortHSDArchive* archive,
                                                    u32 cameraArchiveOffset,
                                                    PCPortTranslatedCamera* outCamera) {
    u16 projectionType;
    u32 eyeOffset;
    u32 interestOffset;
    u32 upVectorOffset;
    f32 upVector[3] = { 0.0f, 1.0f, 0.0f };

    if (outCamera == NULL ||
        !IsArchiveRangeValid(archive,
                             cameraArchiveOffset,
                             PCPORT_SERIALIZED_COBJ_PERSPECTIVE_SIZE)) {
        return FALSE;
    }

    memset(outCamera, 0, sizeof(*outCamera));
    projectionType = ReadBE16(archive->storage + cameraArchiveOffset + 0x06);
    if (projectionType != 1u) {
        return FALSE;
    }

    outCamera->viewportLeft = ReadBE16(archive->storage + cameraArchiveOffset + 0x08);
    outCamera->viewportRight = ReadBE16(archive->storage + cameraArchiveOffset + 0x0A);
    outCamera->viewportTop = ReadBE16(archive->storage + cameraArchiveOffset + 0x0C);
    outCamera->viewportBottom = ReadBE16(archive->storage + cameraArchiveOffset + 0x0E);
    outCamera->scissorLeft = ReadBE16(archive->storage + cameraArchiveOffset + 0x10);
    outCamera->scissorRight = ReadBE16(archive->storage + cameraArchiveOffset + 0x12);
    outCamera->scissorTop = ReadBE16(archive->storage + cameraArchiveOffset + 0x14);
    outCamera->scissorBottom = ReadBE16(archive->storage + cameraArchiveOffset + 0x16);
    eyeOffset = ReadBE32(archive->storage + cameraArchiveOffset + 0x18);
    interestOffset = ReadBE32(archive->storage + cameraArchiveOffset + 0x1C);
    upVectorOffset = ReadBE32(archive->storage + cameraArchiveOffset + 0x24);
    outCamera->nearZ = ReadBEFloat(archive->storage + cameraArchiveOffset + 0x28);
    outCamera->farZ = ReadBEFloat(archive->storage + cameraArchiveOffset + 0x2C);
    outCamera->fov = ReadBEFloat(archive->storage + cameraArchiveOffset + 0x30);
    outCamera->aspect = ReadBEFloat(archive->storage + cameraArchiveOffset + 0x34);
    if (!(outCamera->aspect > 0.0f) &&
        outCamera->viewportBottom > outCamera->viewportTop) {
        outCamera->aspect =
            (f32)(outCamera->viewportRight - outCamera->viewportLeft) /
            (f32)(outCamera->viewportBottom - outCamera->viewportTop);
    }

    if (!LoadSerializedWObjPosition(archive, eyeOffset, outCamera->eye) ||
        !LoadSerializedWObjPosition(archive, interestOffset, outCamera->interest)) {
        return FALSE;
    }

    if (upVectorOffset != 0u &&
        IsArchiveRangeValid(archive, upVectorOffset, 0x0Cu)) {
        upVector[0] = ReadBEFloat(archive->storage + upVectorOffset + 0x00);
        upVector[1] = ReadBEFloat(archive->storage + upVectorOffset + 0x04);
        upVector[2] = ReadBEFloat(archive->storage + upVectorOffset + 0x08);
    }
    memcpy(outCamera->up, upVector, sizeof(upVector));

    if (!BuildCameraViewMatrix(outCamera->eye,
                               outCamera->interest,
                               outCamera->up,
                               outCamera->viewMatrix) ||
        !BuildCameraProjectionMatrix(outCamera->fov,
                                     outCamera->aspect,
                                     outCamera->nearZ,
                                     outCamera->farZ,
                                     outCamera->projectionMatrix)) {
        return FALSE;
    }

    outCamera->cameraArchiveOffset = cameraArchiveOffset;
    outCamera->eyeArchiveOffset = eyeOffset;
    outCamera->interestArchiveOffset = interestOffset;
    return TRUE;
}

BOOL PCPort_TranslateMaterialFromArchiveBE(const PCPortHSDArchive* archive,
                                           u32 mobjArchiveOffset,
                                           PCPortTranslatedMaterial* outMaterial) {
    u32 materialOffset;
    u32 peOffset;

    if (outMaterial == NULL ||
        !IsArchiveRangeValid(archive, mobjArchiveOffset, PCPORT_SERIALIZED_MOBJ_SIZE)) {
        return FALSE;
    }

    memset(outMaterial, 0, sizeof(*outMaterial));
    outMaterial->alpha = 1.0f;
    outMaterial->mobjArchiveOffset = mobjArchiveOffset;
    outMaterial->rendermode = ReadBE32(archive->storage + mobjArchiveOffset + 0x04);
    materialOffset = ReadBE32(archive->storage + mobjArchiveOffset + 0x0C);
    peOffset = ReadBE32(archive->storage + mobjArchiveOffset + 0x14);

    if (materialOffset != 0u) {
        if (!IsArchiveRangeValid(archive,
                                 materialOffset,
                                 PCPORT_SERIALIZED_MATERIAL_SIZE)) {
            return FALSE;
        }

        outMaterial->materialArchiveOffset = materialOffset;
        outMaterial->ambient = ReadBE32(archive->storage + materialOffset + 0x00);
        outMaterial->diffuse = ReadBE32(archive->storage + materialOffset + 0x04);
        outMaterial->specular = ReadBE32(archive->storage + materialOffset + 0x08);
        outMaterial->alpha = ReadBEFloat(archive->storage + materialOffset + 0x0C);
        outMaterial->shininess = ReadBEFloat(archive->storage + materialOffset + 0x10);
    }

    if (peOffset != 0u) {
        if (!IsArchiveRangeValid(archive, peOffset, PCPORT_SERIALIZED_PEDESC_SIZE)) {
            return FALSE;
        }

        outMaterial->pedescArchiveOffset = peOffset;
        outMaterial->hasPEDesc = TRUE;
        outMaterial->peFlags = archive->storage[peOffset + 0x00];
        outMaterial->peRef0 = archive->storage[peOffset + 0x01];
        outMaterial->peRef1 = archive->storage[peOffset + 0x02];
        outMaterial->peDstAlpha = archive->storage[peOffset + 0x03];
        outMaterial->peType = archive->storage[peOffset + 0x04];
        outMaterial->peSrcFactor = archive->storage[peOffset + 0x05];
        outMaterial->peDstFactor = archive->storage[peOffset + 0x06];
        outMaterial->peLogicOp = archive->storage[peOffset + 0x07];
        outMaterial->peZComp = archive->storage[peOffset + 0x08];
        outMaterial->peAlphaComp0 = archive->storage[peOffset + 0x09];
        outMaterial->peAlphaOp = archive->storage[peOffset + 0x0A];
        outMaterial->peAlphaComp1 = archive->storage[peOffset + 0x0B];
    }

    return TRUE;
}

static BOOL TranslateTextureFromArchiveCommon(const PCPortHSDArchive* archive,
                                              u32 tobjArchiveOffset,
                                              BOOL allowNext,
                                              PCPortTranslatedTexture* outTexture) {
    u32 imageOffset;
    u32 imageDataOffset;
    u32 tevOffset;
    u32 tlutOffset;

    if (outTexture == NULL ||
        !IsArchiveRangeValid(archive, tobjArchiveOffset, PCPORT_SERIALIZED_TOBJ_SIZE)) {
        return FALSE;
    }

    memset(outTexture, 0, sizeof(*outTexture));
    outTexture->tevMode = PCPORT_GX_TEV_MODULATE;

    if (!allowNext && ReadBE32(archive->storage + tobjArchiveOffset + 0x04) != 0u) {
        return FALSE;
    }

    imageOffset = ReadBE32(archive->storage + tobjArchiveOffset + 0x4C);
    if (!IsArchiveRangeValid(archive, imageOffset, PCPORT_SERIALIZED_IMAGEDESC_SIZE)) {
        return FALSE;
    }

    imageDataOffset = ReadBE32(archive->storage + imageOffset + 0x00);
    outTexture->width = ReadBE16(archive->storage + imageOffset + 0x04);
    outTexture->height = ReadBE16(archive->storage + imageOffset + 0x06);
    outTexture->format = ReadBE32(archive->storage + imageOffset + 0x08);
    outTexture->mipmap = (u8)(ReadBE32(archive->storage + imageOffset + 0x0C) != 0u);
    outTexture->blending = ReadBEFloat(archive->storage + tobjArchiveOffset + 0x44);
    outTexture->texCoordSrc = ReadBE32(archive->storage + tobjArchiveOffset + 0x0C);
    outTexture->hasCoordId = (u8)ResolveTextureCoordIdFromRawSrc(outTexture->texCoordSrc,
                                                                 &outTexture->coordId);
    outTexture->wrapS = ReadBE32(archive->storage + tobjArchiveOffset + 0x34);
    outTexture->wrapT = ReadBE32(archive->storage + tobjArchiveOffset + 0x38);
    outTexture->repeatS = archive->storage[tobjArchiveOffset + 0x3C];
    outTexture->repeatT = archive->storage[tobjArchiveOffset + 0x3D];
    outTexture->flags = ReadBE32(archive->storage + tobjArchiveOffset + 0x40);
    outTexture->magFilter = ReadBE32(archive->storage + tobjArchiveOffset + 0x48);
    outTexture->tobjArchiveOffset = tobjArchiveOffset;
    outTexture->imageArchiveOffset = imageOffset;
    outTexture->imageDataArchiveOffset = imageDataOffset;
    tevOffset = ReadBE32(archive->storage + tobjArchiveOffset + 0x58);
    tlutOffset = ReadBE32(archive->storage + tobjArchiveOffset + 0x50);
    outTexture->tevArchiveOffset = tevOffset;

    /* tobj+0x50 points at an HSD TlutDesc (NOT the palette data directly):
     *   { u32 lutDataOffset@0x00; u32 fmt@0x04 (GXTlutFmt);
     *     u32 name@0x08; u16 n_entries@0x0C }
     * Capture it so palettized (CI4/CI8) textures can be decoded. */
    if (tlutOffset != 0u && IsArchiveRangeValid(archive, tlutOffset, 0x10u)) {
        outTexture->tlutArchiveOffset =
            ReadBE32(archive->storage + tlutOffset + 0x00);
        outTexture->tlutFmt = ReadBE32(archive->storage + tlutOffset + 0x04);
        outTexture->tlutEntries = ReadBE16(archive->storage + tlutOffset + 0x0C);
    }

    if (outTexture->width == 0u || outTexture->height == 0u ||
        (tevOffset != 0u && !IsArchiveRangeValid(archive, tevOffset, 1u)) ||
        !IsArchiveRangeValid(archive, imageDataOffset, 1u)) {
        return FALSE;
    }

    switch (outTexture->flags & PCPORT_TEX_COLORMAP_MASK) {
    case PCPORT_TEX_COLORMAP_REPLACE:
        outTexture->tevMode = PCPORT_GX_TEV_REPLACE;
        break;
    case PCPORT_TEX_COLORMAP_PASS:
        outTexture->tevMode = PCPORT_GX_TEV_PASSCLR;
        break;
    case PCPORT_TEX_COLORMAP_MODULATE:
    default:
        outTexture->tevMode = PCPORT_GX_TEV_MODULATE;
        break;
    }

    TranslateTextureTevPayload(archive, tevOffset, outTexture->format,
                               &outTexture->tev);
    return TRUE;
}

BOOL PCPort_TranslateTextureFromArchiveBE(const PCPortHSDArchive* archive,
                                          u32 tobjArchiveOffset,
                                          PCPortTranslatedTexture* outTexture) {
    return TranslateTextureFromArchiveCommon(archive,
                                             tobjArchiveOffset,
                                             FALSE,
                                             outTexture);
}

BOOL PCPort_TranslateTextureNodeFromArchiveBE(const PCPortHSDArchive* archive,
                                              u32 tobjArchiveOffset,
                                              PCPortTranslatedTexture* outTexture) {
    return TranslateTextureFromArchiveCommon(archive,
                                             tobjArchiveOffset,
                                             TRUE,
                                             outTexture);
}

BOOL PCPort_TranslateTextureExpFromArchiveBE(const PCPortHSDArchive* archive,
                                             u32 tobjArchiveOffset,
                                             PCPortTranslatedTextureExp* outExp) {
    PCPortParsedTextureNodeChain parsedChain;
    u32 stageIndex;

    if (outExp == NULL) {
        return FALSE;
    }

    memset(outExp, 0, sizeof(*outExp));
    if (!PCPort_ParseTextureNodeChainFromArchiveBE(archive,
                                                   tobjArchiveOffset,
                                                   PCPORT_TEXP_STAGE_MAX,
                                                   &parsedChain)) {
        return FALSE;
    }

    outExp->headArchiveOffset = parsedChain.headArchiveOffset;
    outExp->kind = parsedChain.kind;
    outExp->stageCount = parsedChain.nodeCount;
    for (stageIndex = 0u; stageIndex < parsedChain.nodeCount; ++stageIndex) {
        PCPortTranslatedTextureExpStage* stage = &outExp->stages[stageIndex];

        stage->coordId = parsedChain.coordIds[stageIndex];
        stage->texture = parsedChain.nodes[stageIndex];
        stage->kind = parsedChain.stageKinds[stageIndex];
        if (stage->kind == PCPORT_TEXP_STAGE_NONE) {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL PCPort_TranslateTextureChainFromArchiveBE(const PCPortHSDArchive* archive,
                                               u32 tobjArchiveOffset,
                                               PCPortTranslatedTextureChain* outChain) {
    PCPortParsedTextureNodeChain parsedChain;
    u8 expKind;
    u32 nodeIndex;

    if (outChain == NULL) {
        return FALSE;
    }

    memset(outChain, 0, sizeof(*outChain));
    if (!PCPort_ParseTextureNodeChainFromArchiveBE(archive,
                                                   tobjArchiveOffset,
                                                   PCPORT_TEXTURE_CHAIN_MAX_NODES,
                                                   &parsedChain)) {
        return FALSE;
    }

    outChain->headArchiveOffset = parsedChain.headArchiveOffset;
    outChain->nodeCount = parsedChain.nodeCount;
    for (nodeIndex = 0u; nodeIndex < parsedChain.nodeCount; ++nodeIndex) {
        outChain->coordIds[nodeIndex] = parsedChain.coordIds[nodeIndex];
        outChain->nodes[nodeIndex] = parsedChain.nodes[nodeIndex];
    }

    expKind = parsedChain.kind;
    if (expKind == PCPORT_TEXTURE_EXP_KIND_I8_RAMP) {
        outChain->kind = PCPORT_TEXTURE_CHAIN_I8_RAMP;
        return TRUE;
    }

    if (expKind == PCPORT_TEXTURE_EXP_KIND_I8_RAMP_MASK) {
        outChain->kind = PCPORT_TEXTURE_CHAIN_I8_RAMP_MASK;
        return TRUE;
    }

    return FALSE;
}

static BOOL DecodeI8TextureToLinear(const PCPortHSDArchive* archive,
                                    const PCPortTranslatedTexture* texture,
                                    u8** outPixels,
                                    u32* outSize) {
    u32 tilesX;
    u32 tilesY;
    u32 tileY;
    u32 tileX;
    u8* pixels;

    if (outPixels == NULL || outSize == NULL) {
        return FALSE;
    }

    *outPixels = NULL;
    *outSize = 0u;

    if (archive == NULL || texture == NULL ||
        texture->format != GX_TF_I8 ||
        texture->width == 0u || texture->height == 0u ||
        !IsArchiveRangeValid(archive, texture->imageDataArchiveOffset, 1u)) {
        return FALSE;
    }

    *outSize = (u32)texture->width * (u32)texture->height;
    pixels = (u8*)malloc((size_t)*outSize);
    if (pixels == NULL) {
        *outSize = 0u;
        return FALSE;
    }

    tilesX = ((u32)texture->width + 7u) / 8u;
    tilesY = ((u32)texture->height + 3u) / 4u;
    for (tileY = 0; tileY < tilesY; ++tileY) {
        for (tileX = 0; tileX < tilesX; ++tileX) {
            const u8* tileSrc =
                archive->storage + texture->imageDataArchiveOffset +
                (((tileY * tilesX) + tileX) * 32u);
            u32 row;

            for (row = 0; row < 4u; ++row) {
                u32 dstY = (tileY * 4u) + row;
                u32 col;

                if (dstY >= texture->height) {
                    continue;
                }

                for (col = 0; col < 8u; ++col) {
                    u32 dstX = (tileX * 8u) + col;

                    if (dstX >= texture->width) {
                        continue;
                    }

                    pixels[(dstY * (u32)texture->width) + dstX] =
                        tileSrc[(row * 8u) + col];
                }
            }
        }
    }

    *outPixels = pixels;
    return TRUE;
}

static BOOL DecodeTextureToRGBA(const PCPortHSDArchive* archive,
                                const PCPortTranslatedTexture* texture,
                                u8** outPixels,
                                u32* outSize) {
    GXDecodedTexture decoded;
    const void* tlutData = NULL;
    GXTlutFmt tlutFmt = GX_TL_IA8;
    u16 tlutEntries = 0u;

    if (outPixels == NULL || outSize == NULL) {
        return FALSE;
    }

    *outPixels = NULL;
    *outSize = 0u;

    if (archive == NULL || texture == NULL ||
        texture->width == 0u || texture->height == 0u ||
        !IsArchiveRangeValid(archive, texture->imageDataArchiveOffset, 1u)) {
        return FALSE;
    }

    /* Palettized (CI4/CI8) textures carry a TLUT (palette). Point the decoder
     * at the captured palette data so the index->RGBA lookup resolves; for
     * non-palettized formats the TLUT args stay NULL/0. */
    if (texture->tlutArchiveOffset != 0u &&
        IsArchiveRangeValid(archive, texture->tlutArchiveOffset, 1u)) {
        tlutData = archive->storage + texture->tlutArchiveOffset;
        tlutFmt = (GXTlutFmt)texture->tlutFmt;
        tlutEntries = texture->tlutEntries;
    }

    memset(&decoded, 0, sizeof(decoded));
    if (gx_texture_decode(archive->storage + texture->imageDataArchiveOffset,
                          texture->width,
                          texture->height,
                          (GXTexFmt)texture->format,
                          tlutData,
                          tlutFmt,
                          tlutEntries,
                          &decoded) != 0 ||
        decoded.data == NULL ||
        decoded.isCompressed != 0u) {
        gx_texture_free(&decoded);
        return FALSE;
    }

    *outPixels = decoded.data;
    *outSize = decoded.dataSize;
    return TRUE;
}

static u32 WrapNormalizedTexelIndex(f32 coord,
                                    u32 size,
                                    u32 wrapMode) {
    f32 wrapped = coord;
    u32 index;

    if (size == 0u) {
        return 0u;
    }

    if (wrapMode == GX_REPEAT) {
        while (wrapped < 0.0f) {
            wrapped += 1.0f;
        }
        while (wrapped >= 1.0f) {
            wrapped -= 1.0f;
        }
    } else if (wrapMode == GX_MIRROR) {
        while (wrapped < 0.0f) {
            wrapped += 2.0f;
        }
        while (wrapped >= 2.0f) {
            wrapped -= 2.0f;
        }
        if (wrapped > 1.0f) {
            wrapped = 2.0f - wrapped;
        }
        if (wrapped >= 1.0f) {
            wrapped = 0.999999f;
        }
    } else {
        if (wrapped < 0.0f) {
            wrapped = 0.0f;
        } else if (wrapped >= 1.0f) {
            wrapped = 0.999999f;
        }
    }

    index = (u32)(wrapped * (f32)size);
    if (index >= size) {
        index = size - 1u;
    }
    return index;
}

BOOL PCPort_BakeTextureRGBAFromArchiveBE(const PCPortHSDArchive* archive,
                                         const PCPortTranslatedTexture* texture,
                                         u8** outPixels,
                                         u32* outSize) {
    u32 totalSize;
    u8* pixels;

    if (outPixels == NULL || outSize == NULL) {
        return FALSE;
    }

    *outPixels = NULL;
    *outSize = 0;

    if (archive == NULL || texture == NULL ||
        texture->width == 0u || texture->height == 0u ||
        !IsArchiveRangeValid(archive, texture->imageDataArchiveOffset, 1u)) {
        return FALSE;
    }

    /* I8 colour-ramp TEV: bake the ramp into RGBA. */
    if (texture->tev.kind == PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP &&
        texture->format == GX_TF_I8) {
        totalSize = (u32)texture->width * (u32)texture->height * 4u;
        if (totalSize == 0u) {
            return FALSE;
        }

        pixels = (u8*)malloc((size_t)totalSize);
        if (pixels == NULL) {
            return FALSE;
        }

        DecodeI8RampTexture(archive->storage + texture->imageDataArchiveOffset,
                            texture->width,
                            texture->height,
                            texture->tev.rampLight,
                            texture->tev.rampDark,
                            pixels);
        *outPixels = pixels;
        *outSize = totalSize;
        return TRUE;
    }

    /* Any other texture (no TEV, or a generic non-ramp TEV stage) is a plain
     * sampled texture: decode its native GX format directly. The pipeline's
     * tevMode (REPLACE/PASS/MODULATE) applies the colour blend, so a non-NULL
     * TEV node here is NOT a reason to fall back to a flat material -- that is
     * what left the RGBA8/palettized ground/ruins nodes rendering gray. */
    return DecodeTextureToRGBA(archive, texture, outPixels, outSize);
}

BOOL PCPort_BakeTextureExpRGBAFromArchiveBE(const PCPortHSDArchive* archive,
                                            const PCPortTranslatedTextureExp* exp,
                                            u8** outPixels,
                                            u32* outSize) {
    u8* basePixels = NULL;
    u8* modulatePixels = NULL;
    u32 baseSize = 0u;
    u32 modulateSize = 0u;
    u32 stageIndex;

    if (outPixels == NULL || outSize == NULL) {
        return FALSE;
    }

    *outPixels = NULL;
    *outSize = 0u;

    if (archive == NULL || exp == NULL || exp->stageCount == 0u ||
        !PCPort_BakeTextureRGBAFromArchiveBE(archive,
                                             &exp->stages[0].texture,
                                             &basePixels,
                                             &baseSize)) {
        return FALSE;
    }

    for (stageIndex = 1u; stageIndex < exp->stageCount; ++stageIndex) {
        const PCPortTranslatedTextureExpStage* stage = &exp->stages[stageIndex];
        u32 x;
        u32 y;

        if (stage->kind != PCPORT_TEXP_STAGE_I8_MASK_MODULATE ||
            !DecodeI8TextureToLinear(archive,
                                     &stage->texture,
                                     &modulatePixels,
                                     &modulateSize)) {
            PCPort_FreeBuffer(modulatePixels);
            PCPort_FreeBuffer(basePixels);
            return FALSE;
        }

        for (y = 0; y < (u32)exp->stages[0].texture.height; ++y) {
            for (x = 0; x < (u32)exp->stages[0].texture.width; ++x) {
                f32 u = ((f32)x + 0.5f) / (f32)exp->stages[0].texture.width;
                f32 v = ((f32)y + 0.5f) / (f32)exp->stages[0].texture.height;
                u32 maskX =
                    WrapNormalizedTexelIndex(u,
                                             (u32)stage->texture.width,
                                             stage->texture.wrapS);
                u32 maskY =
                    WrapNormalizedTexelIndex(v,
                                             (u32)stage->texture.height,
                                             stage->texture.wrapT);
                u8 mask =
                    modulatePixels[(maskY * (u32)stage->texture.width) + maskX];
                u8* dst =
                    basePixels + ((((y * (u32)exp->stages[0].texture.width) + x) * 4u));

                dst[0] = (u8)(((u32)dst[0] * (u32)mask + 127u) / 255u);
                dst[1] = (u8)(((u32)dst[1] * (u32)mask + 127u) / 255u);
                dst[2] = (u8)(((u32)dst[2] * (u32)mask + 127u) / 255u);
                dst[3] = (u8)(((u32)dst[3] * (u32)mask + 127u) / 255u);
            }
        }

        PCPort_FreeBuffer(modulatePixels);
        modulatePixels = NULL;
        (void)modulateSize;
    }

    *outPixels = basePixels;
    *outSize = baseSize;
    return TRUE;
}

BOOL PCPort_BakeTextureChainRGBAFromArchiveBE(const PCPortHSDArchive* archive,
                                              const PCPortTranslatedTextureChain* chain,
                                              u8** outPixels,
                                              u32* outSize) {
    u8* basePixels = NULL;
    u8* modulatePixels = NULL;
    u32 baseSize = 0u;
    u32 modulateSize = 0u;
    u32 x;
    u32 y;

    if (outPixels == NULL || outSize == NULL) {
        return FALSE;
    }

    *outPixels = NULL;
    *outSize = 0u;

    if (archive == NULL || chain == NULL) {
        return FALSE;
    }

    if (chain->kind == PCPORT_TEXTURE_CHAIN_I8_RAMP &&
        chain->nodeCount == 1u) {
        return PCPort_BakeTextureRGBAFromArchiveBE(archive,
                                                   &chain->nodes[0],
                                                   outPixels,
                                                   outSize);
    }

    if (chain->kind != PCPORT_TEXTURE_CHAIN_I8_RAMP_MASK ||
        chain->nodeCount != 2u ||
        !PCPort_BakeTextureRGBAFromArchiveBE(archive,
                                             &chain->nodes[0],
                                             &basePixels,
                                             &baseSize) ||
        !DecodeI8TextureToLinear(archive,
                                 &chain->nodes[1],
                                 &modulatePixels,
                                 &modulateSize)) {
        PCPort_FreeBuffer(modulatePixels);
        PCPort_FreeBuffer(basePixels);
        return FALSE;
    }

    for (y = 0; y < (u32)chain->nodes[0].height; ++y) {
        for (x = 0; x < (u32)chain->nodes[0].width; ++x) {
            f32 u = ((f32)x + 0.5f) / (f32)chain->nodes[0].width;
            f32 v = ((f32)y + 0.5f) / (f32)chain->nodes[0].height;
            u32 maskX =
                WrapNormalizedTexelIndex(u,
                                         (u32)chain->nodes[1].width,
                                         chain->nodes[1].wrapS);
            u32 maskY =
                WrapNormalizedTexelIndex(v,
                                         (u32)chain->nodes[1].height,
                                         chain->nodes[1].wrapT);
            u8 mask =
                modulatePixels[(maskY * (u32)chain->nodes[1].width) + maskX];
            u8* dst =
                basePixels + ((((y * (u32)chain->nodes[0].width) + x) * 4u));

            dst[0] = (u8)(((u32)dst[0] * (u32)mask + 127u) / 255u);
            dst[1] = (u8)(((u32)dst[1] * (u32)mask + 127u) / 255u);
            dst[2] = (u8)(((u32)dst[2] * (u32)mask + 127u) / 255u);
            dst[3] = (u8)(((u32)dst[3] * (u32)mask + 127u) / 255u);
        }
    }

    PCPort_FreeBuffer(modulatePixels);
    *outPixels = basePixels;
    *outSize = baseSize;
    (void)modulateSize;
    return TRUE;
}
