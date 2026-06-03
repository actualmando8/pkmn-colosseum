#ifndef PCPORT_REAL_CONTENT_HOST_H
#define PCPORT_REAL_CONTENT_HOST_H

#include "dolphin/types.h"
#include "hsd/hsd_pobj.h"

typedef struct {
    u8* storage;
    u32 storageSize;
    u32 dataSize;
    u32 relocCount;
    u32 publicCount;
    u32 externCount;
    u32 dataOffset;
    u32 relocOffset;
    u32 publicOffset;
    u32 externOffset;
    u32 stringOffset;
} PCPortHSDArchive;

typedef struct {
    HSD_PObj pobj;
    HSD_VtxDescList* verts;
    u8* displayList;
    void* positionData;
    void* colorData;
    void* texcoordData;
    void* texcoord1Data;
    void* normalData;
    u32 sourceArchiveOffset;
    u32 totalSubmittedVertices;
    u32 totalPrimitiveCommands;
    f32 minPosition[3];
    f32 maxPosition[3];
} PCPortTranslatedPObj;

typedef struct {
    u32 rootArchiveOffset;
    u32 jointArchiveOffset;
    f32 modelMatrix[3][4];
} PCPortTranslatedJointTransform;

typedef struct {
    u32 cameraArchiveOffset;
    u32 eyeArchiveOffset;
    u32 interestArchiveOffset;
    u16 viewportLeft;
    u16 viewportRight;
    u16 viewportTop;
    u16 viewportBottom;
    u16 scissorLeft;
    u16 scissorRight;
    u16 scissorTop;
    u16 scissorBottom;
    f32 eye[3];
    f32 interest[3];
    f32 up[3];
    f32 nearZ;
    f32 farZ;
    f32 fov;
    f32 aspect;
    f32 viewMatrix[3][4];
    f32 projectionMatrix[4][4];
} PCPortTranslatedCamera;

typedef struct {
    u32 mobjArchiveOffset;
    u32 materialArchiveOffset;
    u32 pedescArchiveOffset;
    u32 rendermode;
    u32 ambient;
    u32 diffuse;
    u32 specular;
    f32 alpha;
    f32 shininess;
    BOOL hasPEDesc;
    u8 peFlags;
    u8 peRef0;
    u8 peRef1;
    u8 peDstAlpha;
    u8 peType;
    u8 peSrcFactor;
    u8 peDstFactor;
    u8 peLogicOp;
    u8 peZComp;
    u8 peAlphaComp0;
    u8 peAlphaOp;
    u8 peAlphaComp1;
} PCPortTranslatedMaterial;

typedef enum {
    PCPORT_TRANSLATED_TEV_NONE = 0,
    PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP = 1
} PCPortTranslatedTevKind;

typedef struct {
    u32 archiveOffset;
    u32 rawWords[16];
    u8 rawWordCount;
    u8 kind;
    u8 rampLight[4];
    u8 rampDark[4];
} PCPortTranslatedTev;

typedef struct {
    u32 tobjArchiveOffset;
    u32 imageArchiveOffset;
    u32 imageDataArchiveOffset;
    u32 tevArchiveOffset;
    u32 flags;
    u32 texCoordSrc;
    u8 coordId;
    u8 hasCoordId;
    u32 wrapS;
    u32 wrapT;
    u32 magFilter;
    u32 format;
    u16 width;
    u16 height;
    u8 repeatS;
    u8 repeatT;
    u8 mipmap;
    u8 tevMode;
    PCPortTranslatedTev tev;
    f32 blending;
    u32 tlutArchiveOffset; /* archive offset of the palette (lut) data, 0 if none */
    u32 tlutFmt;           /* GXTlutFmt of the palette entries */
    u16 tlutEntries;       /* number of palette entries */
} PCPortTranslatedTexture;

typedef enum {
    PCPORT_TEXP_STAGE_NONE = 0,
    PCPORT_TEXP_STAGE_I8_RAMP_SAMPLE = 1,
    PCPORT_TEXP_STAGE_I8_MASK_MODULATE = 2,
    PCPORT_TEXP_STAGE_DIRECT_SAMPLE = 3
} PCPortTranslatedTextureExpStageKind;

#define PCPORT_TEXP_STAGE_MAX 4

typedef struct {
    u32 headArchiveOffset;
    u8 kind;
    u8 nodeCount;
    u8 coordIds[PCPORT_TEXP_STAGE_MAX];
    u8 stageKinds[PCPORT_TEXP_STAGE_MAX];
    PCPortTranslatedTexture nodes[PCPORT_TEXP_STAGE_MAX];
} PCPortParsedTextureNodeChain;

typedef struct {
    u8 kind;
    u8 coordId;
    PCPortTranslatedTexture texture;
} PCPortTranslatedTextureExpStage;

typedef enum {
    PCPORT_TEXTURE_EXP_KIND_NONE = 0,
    PCPORT_TEXTURE_EXP_KIND_I8_RAMP = 1,
    PCPORT_TEXTURE_EXP_KIND_I8_RAMP_MASK = 2,
    PCPORT_TEXTURE_EXP_KIND_DIRECT_SAMPLE = 3
} PCPortTranslatedTextureExpKind;

typedef struct {
    u32 headArchiveOffset;
    u8 kind;
    u8 stageCount;
    PCPortTranslatedTextureExpStage stages[PCPORT_TEXP_STAGE_MAX];
} PCPortTranslatedTextureExp;

typedef enum {
    PCPORT_TEXTURE_CHAIN_NONE = 0,
    PCPORT_TEXTURE_CHAIN_I8_RAMP = 1,
    PCPORT_TEXTURE_CHAIN_I8_RAMP_MASK = 2
} PCPortTranslatedTextureChainKind;

#define PCPORT_TEXTURE_CHAIN_MAX_NODES 2

typedef struct {
    u32 headArchiveOffset;
    u8 nodeCount;
    u8 kind;
    u8 coordIds[PCPORT_TEXTURE_CHAIN_MAX_NODES];
    PCPortTranslatedTexture nodes[PCPORT_TEXTURE_CHAIN_MAX_NODES];
} PCPortTranslatedTextureChain;

BOOL PCPort_LoadFsysMember(const char* fsysPath, const char* memberName,
                           u8** outData, u32* outSize);
/* Returns the largest HSD-archive member exposing a "scene_data" public symbol
 * (for field maps whose members share names; the title uses LoadFsysMember). */
BOOL PCPort_LoadFsysSceneMember(const char* fsysPath, u8** outData, u32* outSize);
void PCPort_FreeBuffer(void* buffer);

BOOL PCPort_HSDArchiveParseBE(PCPortHSDArchive* archive,
                              const void* data, u32 size);
void PCPort_HSDArchiveDestroy(PCPortHSDArchive* archive);
const void* PCPort_HSDArchiveGetPublicAddress(const PCPortHSDArchive* archive,
                                              const char* name,
                                              u32* outArchiveOffset);
u32 PCPort_ReadBigEndianU32(const void* data);
BOOL PCPort_TranslatePObjFromArchiveBE(const PCPortHSDArchive* archive,
                                       u32 pobjArchiveOffset,
                                       PCPortTranslatedPObj* outPObj);
void PCPort_DestroyTranslatedPObj(PCPortTranslatedPObj* pobj);
BOOL PCPort_TranslateJointChainToMatrixBE(const PCPortHSDArchive* archive,
                                          u32 rootJointArchiveOffset,
                                          u32 targetJointArchiveOffset,
                                          PCPortTranslatedJointTransform* outTransform);
BOOL PCPort_TranslatePerspectiveCameraFromArchiveBE(const PCPortHSDArchive* archive,
                                                    u32 cameraArchiveOffset,
                                                    PCPortTranslatedCamera* outCamera);
BOOL PCPort_TranslateMaterialFromArchiveBE(const PCPortHSDArchive* archive,
                                           u32 mobjArchiveOffset,
                                           PCPortTranslatedMaterial* outMaterial);
BOOL PCPort_TranslateTextureFromArchiveBE(const PCPortHSDArchive* archive,
                                          u32 tobjArchiveOffset,
                                          PCPortTranslatedTexture* outTexture);
BOOL PCPort_TranslateTextureNodeFromArchiveBE(const PCPortHSDArchive* archive,
                                              u32 tobjArchiveOffset,
                                              PCPortTranslatedTexture* outTexture);
BOOL PCPort_ParseTextureNodeChainFromArchiveBE(const PCPortHSDArchive* archive,
                                               u32 tobjArchiveOffset,
                                               u32 maxNodes,
                                               PCPortParsedTextureNodeChain* outChain);
BOOL PCPort_TranslateTextureExpFromArchiveBE(const PCPortHSDArchive* archive,
                                             u32 tobjArchiveOffset,
                                             PCPortTranslatedTextureExp* outExp);
BOOL PCPort_TranslateTextureChainFromArchiveBE(const PCPortHSDArchive* archive,
                                               u32 tobjArchiveOffset,
                                               PCPortTranslatedTextureChain* outChain);
BOOL PCPort_BakeTextureRGBAFromArchiveBE(const PCPortHSDArchive* archive,
                                         const PCPortTranslatedTexture* texture,
                                         u8** outPixels,
                                         u32* outSize);
BOOL PCPort_BakeTextureExpRGBAFromArchiveBE(const PCPortHSDArchive* archive,
                                            const PCPortTranslatedTextureExp* exp,
                                            u8** outPixels,
                                            u32* outSize);
BOOL PCPort_BakeTextureChainRGBAFromArchiveBE(const PCPortHSDArchive* archive,
                                              const PCPortTranslatedTextureChain* chain,
                                              u8** outPixels,
                                              u32* outSize);

#endif
