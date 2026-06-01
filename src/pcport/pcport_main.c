#include "audio_shim.h"
#include "dvd_shim.h"
#include "gx_shim.h"
#include "gx_texture.h"
#include "hsd/hsd_pobj.h"
#include "os_shim.h"
#include "pad_shim.h"
#include "pcport_window.h"
#include "real_content_host.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * Minimal bridge to one decompiled Dolphin SDK TU. This avoids pulling in
 * the full SDK header stack into the bootstrap main file.
 */
extern unsigned long CurrTvMode;
extern unsigned long VIGetTvFormat(void);
extern unsigned long __OSGetAppType(void);
extern void __OSSetAppType(unsigned long type);
extern void* TRK_memcpy(void* dst, const void* src, unsigned long n);
extern char* TRK_strcat(char* dst, const char* src);
extern unsigned long TRK_strlen(const char* s);
extern void GSgfxInit(unsigned int memSize, unsigned int fifoSize,
                      unsigned int mtxDepth, unsigned int lightCount,
                      unsigned int numBufs, unsigned int dlSize);
extern void GSgfx_BeginFrame(void);
extern void GSgfxSwapBuffers(unsigned int flag);
extern unsigned int GSgfxGetFrameCount(void);
extern unsigned int GSgfxHostGetPreRetraceCount(void);
extern unsigned char GSgfxHostGetDrawDoneFlag(void);
extern void fn_800D9D68(unsigned int x1, unsigned int y1,
                        unsigned int x2, unsigned int y2);
extern void fn_800DAD10(void* obj);
extern void fn_801AA568(HSD_PObj* pobj);
extern void GSgfxHostClearPipelineState(unsigned int pipelineId);
extern void GSgfxHostSetPipelineBlend(unsigned int pipelineId,
                                      unsigned int type,
                                      unsigned int src_factor,
                                      unsigned int dst_factor,
                                      unsigned int op);
extern void GSgfxHostSetPipelineZ(unsigned int pipelineId,
                                  unsigned char compare_enable,
                                  unsigned int func,
                                  unsigned char update_enable);
extern void GSgfxHostSetPipelineAlphaCompare(unsigned int pipelineId,
                                             unsigned int comp0,
                                             unsigned char ref0,
                                             unsigned int op,
                                             unsigned int comp1,
                                             unsigned char ref1);
extern void GSgfxHostSetPipelineAlphaScale(unsigned int pipelineId,
                                           float alphaScale);
extern void GSgfxHostSetPipelineTexture(unsigned int pipelineId,
                                        const void* textureObject,
                                        unsigned char numTexGens,
                                        unsigned char tevMode,
                                        unsigned char textureCoordId,
                                        unsigned char textureMapId);
extern unsigned int GXHostGetLastSubmittedVertexCount(void);
extern unsigned int GXHostGetLastExpandedVertexCount(void);
extern unsigned int GXHostGetLastSubmittedPrimitive(void);

unsigned long CurrTvMode = 0;

#define PCPORT_WINDOW_WIDTH  640
#define PCPORT_WINDOW_HEIGHT 480
#define PCPORT_WINDOW_FRAMES 120
#define PCPORT_GSGFX_SWAPS   3
#define PCPORT_GX_SMOKE_SAMPLE_X 320
#define PCPORT_GX_SMOKE_SAMPLE_Y 240
#define PCPORT_GX_SCISSOR_X   160
#define PCPORT_GX_SCISSOR_Y   120
#define PCPORT_GX_SCISSOR_W   320
#define PCPORT_GX_SCISSOR_H   240
#define PCPORT_GX_SCISSOR_X2  (PCPORT_GX_SCISSOR_X + PCPORT_GX_SCISSOR_W - 1)
#define PCPORT_GX_SCISSOR_Y2  (PCPORT_GX_SCISSOR_Y + PCPORT_GX_SCISSOR_H - 1)
#define PCPORT_GX_OUTSIDE_X   40
#define PCPORT_GX_OUTSIDE_Y   40
#define PCPORT_REAL_CONTENT_ARCHIVE "orig/GC6E01/disc/files/topmenu.fsys"
#define PCPORT_REAL_CONTENT_MEMBER  "menu_bg00"

/* Static "Pokemon Colosseum" title wordmark: title.fsys -> member logo_demo
 * (HSD archive), texture index 01 = RGBA8 540x224 at decompressed-archive
 * offset 0xAC2E0. Drawn as an alpha-blended 2D overlay over menu_bg00. */
#define PCPORT_LOGO_ARCHIVE      "orig/GC6E01/disc/files/title.fsys"
#define PCPORT_LOGO_MEMBER       "logo_demo"
#define PCPORT_LOGO_IMAGE_OFFSET 0xAC2E0u
#define PCPORT_LOGO_WIDTH        540
#define PCPORT_LOGO_HEIGHT       224

/* Title-screen 2D sprite sheet in topmenu.fsys: copyright lines + PRESS START
 * (RGB5A3 428x122, a raw 0x80-header texture, not an HSD archive). UV bands:
 * copyright block v0.016..0.549, PRESS START (teal) v0.574..0.721. */
#define PCPORT_TITLE_PRESS_MEMBER "menu_018"

/* The title-screen 3D scene (desert/ruins environment + the logo, all one HSD
 * archive) is title.fsys:logo_demo. Its scene_data layout is byte-compatible
 * with menu_bg00 and it carries a real perspective camera, so RunMenuScene
 * renders it with just this archive/member swap. */
#define PCPORT_TITLE_SCENE_ARCHIVE "orig/GC6E01/disc/files/title.fsys"
#define PCPORT_TITLE_SCENE_MEMBER  "logo_demo"

/* Sky/sand horizon backdrop texture inside title.fsys:logo_demo (CMPR 512x256
 * at archive offset 0x14A8E0): blue sky + clouds fading to tan sand. Drawn as a
 * full-screen 2D backdrop -- the reliable stand-in for the title scene's 3D
 * environment, whose animated-demo geometry does not render statically yet. */
#define PCPORT_TITLE_SKY_OFFSET 0x14A8E0u
#define PCPORT_TITLE_SKY_WIDTH  512
#define PCPORT_TITLE_SKY_HEIGHT 256
#define PCPORT_PDA_MENU_ARCHIVE     "orig/GC6E01/disc/files/pda_menu.fsys"
#define PCPORT_PDA2_BG_MEMBER       "pda2_bg"
#define PCPORT_SERIALIZED_JOINT_SIZE 0x40u
#define PCPORT_SERIALIZED_DOBJ_SIZE  0x10u
#define PCPORT_SERIALIZED_POBJ_SIZE  0x18u
#define PCPORT_REAL_MATERIAL_PIPELINE 1u
#define PCPORT_REAL_TEXTURED_PIPELINE 2u
#define PCPORT_REAL_SIBLING_TEXTURED_PIPELINE 3u
#define PCPORT_RENDER_NO_ZUPDATE 0x20000000u
#define PCPORT_RENDER_XLU        0x40000000u
#define PCPORT_TEXTURED_JOINT_OFFSET 0x6FE8u
#define PCPORT_TEXTURED_DOBJ_OFFSET  0x3B98u
#define PCPORT_TEXTURED_MOBJ_OFFSET  0x3900u
#define PCPORT_TEXTURED_POBJ_OFFSET  0x3B80u
#define PCPORT_SIBLING_JOINT_OFFSET  0x70E8u
#define PCPORT_SIBLING_DOBJ_OFFSET   0x6F98u
#define PCPORT_SIBLING_MOBJ_OFFSET   0x39CCu
#define PCPORT_SIBLING_POBJ_OFFSET   0x6F80u
#define PCPORT_PDA2_BG_OBJECT0_JOINT_OFFSET 0x22E8u
#define PCPORT_PDA2_BG_OBJECT0_DOBJ_OFFSET  0x1FB8u
#define PCPORT_PDA2_BG_OBJECT0_MOBJ_OFFSET  0x1A2Cu
#define PCPORT_PDA2_BG_OBJECT0_POBJ_OFFSET  0x1FA0u
#define PCPORT_PDA2_BG_OBJECT1_JOINT_OFFSET 0x2328u
#define PCPORT_PDA2_BG_OBJECT1_DOBJ_OFFSET  0x2298u
#define PCPORT_PDA2_BG_OBJECT1_MOBJ_OFFSET  0x1AECu
#define PCPORT_PDA2_BG_OBJECT1_POBJ_OFFSET  0x2280u

typedef struct {
    unsigned int reserved;
    const void* displayList;
    unsigned int displayListSize;
    unsigned int pipelineId;
    unsigned int totalVerts;
    unsigned int totalPrims;
} PCPortGSDrawObject;

static const float g_sceneLikePositions[][3] = {
    { -0.78f, -0.58f, 0.0f },
    {  0.78f, -0.58f, 0.0f },
    {  0.78f,  0.58f, 0.0f },
    { -0.78f,  0.58f, 0.0f },
    { -0.68f, -0.48f, 0.0f },
    {  0.68f, -0.48f, 0.0f },
    {  0.68f,  0.48f, 0.0f },
    { -0.68f,  0.48f, 0.0f },
    { -0.68f,  0.20f, 0.0f },
    {  0.68f,  0.20f, 0.0f },
    {  0.68f,  0.48f, 0.0f },
    { -0.68f,  0.48f, 0.0f }
};

static const unsigned char g_sceneLikeColors[][4] = {
    { 0x1B, 0x28, 0x3A, 0xFF },
    { 0x1B, 0x28, 0x3A, 0xFF },
    { 0x1B, 0x28, 0x3A, 0xFF },
    { 0x1B, 0x28, 0x3A, 0xFF },
    { 0xE6, 0xDC, 0xBC, 0xFF },
    { 0xE6, 0xDC, 0xBC, 0xFF },
    { 0xE6, 0xDC, 0xBC, 0xFF },
    { 0xE6, 0xDC, 0xBC, 0xFF },
    { 0xD0, 0x7A, 0x20, 0xFF },
    { 0xD0, 0x7A, 0x20, 0xFF },
    { 0xD0, 0x7A, 0x20, 0xFF },
    { 0xD0, 0x7A, 0x20, 0xFF }
};

static const float g_sceneLikeTexcoords[][2] = {
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f }
};

static const unsigned char g_sceneLikeDisplayList[] = {
    0x80, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x03, 0x03, 0x03,
    0x80, 0x00, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x07, 0x07, 0x07,
    0x80, 0x00, 0x04, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09,
    0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B
};

static const PCPortGSDrawObject g_sceneLikeDrawObject = {
    0,
    g_sceneLikeDisplayList,
    sizeof(g_sceneLikeDisplayList),
    0,
    (unsigned int)(sizeof(g_sceneLikePositions) /
                   sizeof(g_sceneLikePositions[0])),
    3
};

static HSD_VtxDescList g_sceneLikePObjVerts[] = {
    { GX_VA_POS,  GX_INDEX8, GX_POS_XYZ, GX_F32,   0,
      sizeof(g_sceneLikePositions[0]), (void*)g_sceneLikePositions },
    { GX_VA_CLR0, GX_INDEX8, GX_CLR_RGBA, GX_RGBA8, 0,
      sizeof(g_sceneLikeColors[0]), (void*)g_sceneLikeColors },
    { GX_VA_TEX0, GX_INDEX8, GX_TEX_ST,  GX_F32,   0,
      sizeof(g_sceneLikeTexcoords[0]), (void*)g_sceneLikeTexcoords },
    { GX_VA_NULL, GX_NONE,   0,          0,        0, 0, NULL }
};

static void ConfigureSceneLikeDisplayListState(void) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_POS, (void*)g_sceneLikePositions,
               sizeof(g_sceneLikePositions[0]));
    GXSetArray(GX_VA_CLR0, (void*)g_sceneLikeColors,
               sizeof(g_sceneLikeColors[0]));
    GXSetArray(GX_VA_TEX0, (void*)g_sceneLikeTexcoords,
               sizeof(g_sceneLikeTexcoords[0]));
}

static void InitSceneLikePObj(HSD_PObj* pobj) {
    memset(pobj, 0, sizeof(*pobj));
    pobj->verts = g_sceneLikePObjVerts;
    pobj->n_display = (u16)sizeof(g_sceneLikeDisplayList);
    pobj->display = (u8*)g_sceneLikeDisplayList;
}

static int ReadBackbufferPixelAt(int x, int y, unsigned char pixel[4]) {
    glReadBuffer(GL_BACK);
    glReadPixels(x,
                 y,
                 1,
                 1,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixel);
    return pixel[0] != 0 || pixel[1] != 0 || pixel[2] != 0;
}

/* Dump an RGBA framebuffer to a 24-bit BMP when env PCPORT_DUMP is set. GL's
 * bottom-up origin matches BMP's, so rows are written as-is. (PCPORT-only.) */
static void DumpFramebufferBMP(const unsigned char* rgba, int w, int h) {
    const char* path = getenv("PCPORT_DUMP");
    FILE* f;
    int rowsize, imgsize, x, y;
    unsigned char hdr[54];
    unsigned char* row;
    unsigned int filesize;
    if (path == NULL || rgba == NULL) return;
    f = fopen(path, "wb");
    if (f == NULL) return;
    rowsize = (w * 3 + 3) & ~3;
    imgsize = rowsize * h;
    filesize = 54u + (unsigned int)imgsize;
    memset(hdr, 0, sizeof(hdr));
    hdr[0] = 'B'; hdr[1] = 'M';
    hdr[2] = filesize & 0xff; hdr[3] = (filesize >> 8) & 0xff;
    hdr[4] = (filesize >> 16) & 0xff; hdr[5] = (filesize >> 24) & 0xff;
    hdr[10] = 54; hdr[14] = 40;
    hdr[18] = w & 0xff; hdr[19] = (w >> 8) & 0xff;
    hdr[22] = h & 0xff; hdr[23] = (h >> 8) & 0xff;
    hdr[26] = 1; hdr[28] = 24;
    fwrite(hdr, 1, 54, f);
    row = (unsigned char*)malloc(rowsize);
    for (y = 0; y < h; y++) {
        memset(row, 0, rowsize);
        for (x = 0; x < w; x++) {
            const unsigned char* p = rgba + ((size_t)y * w + x) * 4;
            row[x * 3 + 0] = p[2]; row[x * 3 + 1] = p[1]; row[x * 3 + 2] = p[0];
        }
        fwrite(row, 1, rowsize, f);
    }
    free(row);
    fclose(f);
}

static unsigned char* ReadBackbufferImage(void) {
    size_t pixelCount = (size_t)PCPORT_WINDOW_WIDTH * (size_t)PCPORT_WINDOW_HEIGHT;
    size_t bufferSize = pixelCount * 4u;
    unsigned char* pixels = (unsigned char*)malloc(bufferSize);

    if (pixels == NULL) {
        return NULL;
    }

    glReadBuffer(GL_BACK);
    glReadPixels(0,
                 0,
                 PCPORT_WINDOW_WIDTH,
                 PCPORT_WINDOW_HEIGHT,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixels);
    DumpFramebufferBMP(pixels, PCPORT_WINDOW_WIDTH, PCPORT_WINDOW_HEIGHT);
    return pixels;
}

static unsigned int CountFramebufferDiffPixels(const unsigned char* before,
                                               const unsigned char* after) {
    size_t pixelCount = (size_t)PCPORT_WINDOW_WIDTH * (size_t)PCPORT_WINDOW_HEIGHT;
    unsigned int diffCount = 0;
    size_t i;

    if (before == NULL || after == NULL) {
        return 0;
    }

    for (i = 0; i < pixelCount; ++i) {
        size_t base = i * 4u;

        if (before[base + 0] != after[base + 0] ||
            before[base + 1] != after[base + 1] ||
            before[base + 2] != after[base + 2] ||
            before[base + 3] != after[base + 3]) {
            ++diffCount;
        }
    }

    return diffCount;
}

static void TransformBoundsByMtx(const f32 minBounds[3],
                                 const f32 maxBounds[3],
                                 const f32 modelMatrix[3][4],
                                 f32 outMin[3],
                                 f32 outMax[3]) {
    u32 corner;

    for (corner = 0; corner < 8u; ++corner) {
        f32 x = (corner & 1u) != 0u ? maxBounds[0] : minBounds[0];
        f32 y = (corner & 2u) != 0u ? maxBounds[1] : minBounds[1];
        f32 z = (corner & 4u) != 0u ? maxBounds[2] : minBounds[2];
        f32 transformed[3];
        u32 axis;

        transformed[0] = (modelMatrix[0][0] * x) +
                         (modelMatrix[0][1] * y) +
                         (modelMatrix[0][2] * z) +
                         modelMatrix[0][3];
        transformed[1] = (modelMatrix[1][0] * x) +
                         (modelMatrix[1][1] * y) +
                         (modelMatrix[1][2] * z) +
                         modelMatrix[1][3];
        transformed[2] = (modelMatrix[2][0] * x) +
                         (modelMatrix[2][1] * y) +
                         (modelMatrix[2][2] * z) +
                         modelMatrix[2][3];

        for (axis = 0; axis < 3u; ++axis) {
            if (corner == 0u || transformed[axis] < outMin[axis]) {
                outMin[axis] = transformed[axis];
            }
            if (corner == 0u || transformed[axis] > outMax[axis]) {
                outMax[axis] = transformed[axis];
            }
        }
    }
}

static void ConcatAffineMtx(const f32 a[3][4],
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

static int ReadBackbufferPixel(unsigned char pixel[4]) {
    return ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                                 PCPORT_GX_SMOKE_SAMPLE_Y,
                                 pixel);
}

static void ClearBackbuffer(float r, float g, float b) {
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glDisable(GL_SCISSOR_TEST);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void LoadIdentityGXState(void) {
    static Mtx44 projection = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };
    static Mtx modelView = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f }
    };

    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(modelView, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
}

static void SubmitFullScreenGXQuad(unsigned char r,
                                   unsigned char g,
                                   unsigned char b,
                                   unsigned char a) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXColor4u8(r, g, b, a);
    GXPosition3f32(-1.0f, -1.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);

    GXColor4u8(r, g, b, a);
    GXPosition3f32(1.0f, -1.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);

    GXColor4u8(r, g, b, a);
    GXPosition3f32(1.0f, 1.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);

    GXColor4u8(r, g, b, a);
    GXPosition3f32(-1.0f, 1.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);

    GXEnd();
}

/* Draw the static title logo as an alpha-blended 2D overlay quad, composited
 * on top of whatever 3D scene was just rendered. Resets all the per-frame 3D
 * camera state (viewport/scissor/projection/modelview/depth) to a full-screen
 * orthographic identity so the quad is neither clipped to the camera scissor
 * nor depth-killed against the background. */
/* Set up the full-screen 2D orthographic overlay state once per frame: reset
 * the per-frame 3D camera viewport/scissor/projection/modelview, disable depth,
 * enable alpha blend, and configure a single textured (MODULATE) TEV stage so
 * the immediate path samples the bound texture. */
static void BeginMenuOverlay(void) {
    static Mtx44 orthoProjection = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };
    static Mtx identityModelView = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f }
    };

    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0u, 0u, 640u, 480u);
    GXSetProjection(orthoProjection, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(identityModelView, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetNumTexGens(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXHostSetVertexAlphaScale(1.0f);
}

/* Draw a textured quad at a screen-space rectangle (origin top-left, 640x480),
 * sampling the texture sub-rect [u0,v0]-[u1,v1]. v increases downward so the
 * image renders upright. Call BeginMenuOverlay() first. White vertex colour
 * passes the (MODULATE) texel through unchanged. */
static void DrawTexturedScreenRect(GXTexObj* tex,
                                   f32 sx, f32 sy, f32 sw, f32 sh,
                                   f32 u0, f32 v0, f32 u1, f32 v1) {
    f32 ndcL = ((sx) / 640.0f) * 2.0f - 1.0f;
    f32 ndcR = ((sx + sw) / 640.0f) * 2.0f - 1.0f;
    f32 ndcT = 1.0f - ((sy) / 480.0f) * 2.0f;
    f32 ndcB = 1.0f - ((sy + sh) / 480.0f) * 2.0f;

    GXLoadTexObj(tex, GX_TEXMAP0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXColor4u8(255, 255, 255, 255);
    GXPosition3f32(ndcL, ndcB, 0.0f);
    GXTexCoord2f32(u0, v1);

    GXColor4u8(255, 255, 255, 255);
    GXPosition3f32(ndcR, ndcB, 0.0f);
    GXTexCoord2f32(u1, v1);

    GXColor4u8(255, 255, 255, 255);
    GXPosition3f32(ndcR, ndcT, 0.0f);
    GXTexCoord2f32(u1, v0);

    GXColor4u8(255, 255, 255, 255);
    GXPosition3f32(ndcL, ndcT, 0.0f);
    GXTexCoord2f32(u0, v0);

    GXEnd();
}

/* Load a raw topmenu sprite member (0x80-byte header: width@0, height@2 as
 * big-endian u16, bpp marker@4 with 0x20=RGBA8 else RGB5A3; texels at +0x80),
 * decode to RGBA, and upload as a host texture. Returns 1 on success. */
static int LoadRawMenuTexObj(const char* member, GXTexObj* outTex) {
    u8* data = NULL;
    u32 size = 0;
    GXDecodedTexture decoded;
    u16 w;
    u16 h;
    u32 format;

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE, member,
                               &data, &size)) {
        return 0;
    }
    if (size < 0x80u) {
        PCPort_FreeBuffer(data);
        return 0;
    }

    w = (u16)(((u16)data[0] << 8) | data[1]);
    h = (u16)(((u16)data[2] << 8) | data[3]);
    format = (data[4] == 0x20u) ? (u32)GX_TF_RGBA8 : (u32)GX_TF_RGB5A3;

    memset(&decoded, 0, sizeof(decoded));
    if (gx_texture_decode(data + 0x80, w, h, (GXTexFmt)format,
                          NULL, GX_TL_IA8, 0, &decoded) != 0 ||
        decoded.data == NULL) {
        PCPort_FreeBuffer(data);
        return 0;
    }

    memset(outTex, 0, sizeof(*outTex));
    GXHostInitTexObjRGBA8(outTex, decoded.data, w, h, GX_CLAMP, GX_CLAMP);
    gx_texture_free(&decoded);
    PCPort_FreeBuffer(data);
    return 1;
}

static int RunRawPrimitiveControl(void) {
    unsigned char pixel[4];

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glBegin(GL_TRIANGLES);
    glColor3ub(0xFF, 0x00, 0xFF);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glColor3ub(0xFF, 0x00, 0xFF);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glColor3ub(0xFF, 0x00, 0xFF);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glFlush();

    return ReadBackbufferPixel(pixel);
}

static int RunRawScissorControl(void) {
    unsigned char inside[4];
    unsigned char outside[4];

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glScissor(PCPORT_GX_SCISSOR_X,
              PCPORT_GX_SCISSOR_Y,
              PCPORT_GX_SCISSOR_W,
              PCPORT_GX_SCISSOR_H);

    glBegin(GL_TRIANGLES);
    glColor3ub(0xFF, 0x80, 0x00);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glColor3ub(0xFF, 0x80, 0x00);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glColor3ub(0xFF, 0x80, 0x00);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glFlush();

    ReadBackbufferPixel(inside);
    ReadBackbufferPixelAt(PCPORT_GX_OUTSIDE_X, PCPORT_GX_OUTSIDE_Y, outside);
    glDisable(GL_SCISSOR_TEST);

    return (inside[0] != 0 || inside[1] != 0 || inside[2] != 0) &&
           outside[0] == 0 && outside[1] == 0 && outside[2] == 0;
}

static int RunGXPrimitiveSmoke(void) {
    unsigned char pixel[4];

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    LoadIdentityGXState();
    SubmitFullScreenGXQuad(0x20, 0xD0, 0x40, 0xFF);
    glFlush();

    if (!ReadBackbufferPixel(pixel)) {
        int rawWorked = RunRawPrimitiveControl();
        fprintf(stderr,
                "[pcport_bootstrap] GX primitive smoke failed (rgba=%u,%u,%u,%u rawControl=%d submitted=%u expanded=%u prim=0x%X)\n",
                (unsigned int)pixel[0],
                (unsigned int)pixel[1],
                (unsigned int)pixel[2],
                (unsigned int)pixel[3],
                rawWorked,
                GXHostGetLastSubmittedVertexCount(),
                GXHostGetLastExpandedVertexCount(),
                GXHostGetLastSubmittedPrimitive());
        return 0;
    }

    printf("[pcport_bootstrap] Host GX primitive smoke passed (rgba=%u,%u,%u,%u submitted=%u expanded=%u prim=0x%X)\n",
           (unsigned int)pixel[0],
           (unsigned int)pixel[1],
           (unsigned int)pixel[2],
           (unsigned int)pixel[3],
           GXHostGetLastSubmittedVertexCount(),
           GXHostGetLastExpandedVertexCount(),
           GXHostGetLastSubmittedPrimitive());
    return 1;
}

static int RunGXScissorSmoke(void) {
    unsigned char inside[4];
    unsigned char outside[4];

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    LoadIdentityGXState();
    GXSetScissor(PCPORT_GX_SCISSOR_X,
                 PCPORT_GX_SCISSOR_Y,
                 PCPORT_GX_SCISSOR_W,
                 PCPORT_GX_SCISSOR_H);
    SubmitFullScreenGXQuad(0x20, 0xA0, 0xF0, 0xFF);
    glFlush();

    ReadBackbufferPixel(inside);
    ReadBackbufferPixelAt(PCPORT_GX_OUTSIDE_X, PCPORT_GX_OUTSIDE_Y, outside);
    glDisable(GL_SCISSOR_TEST);

    if ((inside[0] == 0 && inside[1] == 0 && inside[2] == 0) ||
        outside[0] != 0 || outside[1] != 0 || outside[2] != 0) {
        int rawWorked = RunRawScissorControl();
        fprintf(stderr,
                "[pcport_bootstrap] GX scissor smoke failed (inside=%u,%u,%u,%u outside=%u,%u,%u,%u rawControl=%d submitted=%u expanded=%u prim=0x%X)\n",
                (unsigned int)inside[0],
                (unsigned int)inside[1],
                (unsigned int)inside[2],
                (unsigned int)inside[3],
                (unsigned int)outside[0],
                (unsigned int)outside[1],
                (unsigned int)outside[2],
                (unsigned int)outside[3],
                rawWorked,
                GXHostGetLastSubmittedVertexCount(),
                GXHostGetLastExpandedVertexCount(),
                GXHostGetLastSubmittedPrimitive());
        return 0;
    }

    printf("[pcport_bootstrap] Host GX scissor smoke passed (inside=%u,%u,%u,%u outside=%u,%u,%u,%u)\n",
           (unsigned int)inside[0],
           (unsigned int)inside[1],
           (unsigned int)inside[2],
           (unsigned int)inside[3],
           (unsigned int)outside[0],
           (unsigned int)outside[1],
           (unsigned int)outside[2],
           (unsigned int)outside[3]);
    return 1;
}

static int RunGSgfxVisibleAttempt(void) {
    unsigned char pixel[4];

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();
    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    glFlush();

    if (!ReadBackbufferPixel(pixel)) {
        fprintf(stderr,
                "[pcport_bootstrap] GSgfx_BeginFrame visible attempt failed (rgba=%u,%u,%u,%u submitted=%u expanded=%u prim=0x%X)\n",
                (unsigned int)pixel[0],
                (unsigned int)pixel[1],
                (unsigned int)pixel[2],
                (unsigned int)pixel[3],
                GXHostGetLastSubmittedVertexCount(),
                GXHostGetLastExpandedVertexCount(),
                GXHostGetLastSubmittedPrimitive());
        return 0;
    }

    printf("[pcport_bootstrap] GSgfx_BeginFrame visible attempt passed (rgba=%u,%u,%u,%u)\n",
           (unsigned int)pixel[0],
           (unsigned int)pixel[1],
           (unsigned int)pixel[2],
           (unsigned int)pixel[3]);
    return 1;
}

static int RunGSgfxScissorRetry(void) {
    unsigned char inside[4];
    unsigned char outside[4];

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();
    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    fn_800D9D68(PCPORT_GX_SCISSOR_X,
                PCPORT_GX_SCISSOR_Y,
                PCPORT_GX_SCISSOR_X2,
                PCPORT_GX_SCISSOR_Y2);
    GSgfx_BeginFrame();
    glFlush();

    ReadBackbufferPixel(inside);
    ReadBackbufferPixelAt(PCPORT_GX_OUTSIDE_X, PCPORT_GX_OUTSIDE_Y, outside);
    glDisable(GL_SCISSOR_TEST);

    if ((inside[0] == 0 && inside[1] == 0 && inside[2] == 0) ||
        outside[0] != 0 || outside[1] != 0 || outside[2] != 0) {
        fprintf(stderr,
                "[pcport_bootstrap] Game-owned fn_800D9D68 retry failed (inside=%u,%u,%u,%u outside=%u,%u,%u,%u submitted=%u expanded=%u prim=0x%X)\n",
                (unsigned int)inside[0],
                (unsigned int)inside[1],
                (unsigned int)inside[2],
                (unsigned int)inside[3],
                (unsigned int)outside[0],
                (unsigned int)outside[1],
                (unsigned int)outside[2],
                (unsigned int)outside[3],
                GXHostGetLastSubmittedVertexCount(),
                GXHostGetLastExpandedVertexCount(),
                GXHostGetLastSubmittedPrimitive());
        return 0;
    }

    printf("[pcport_bootstrap] Game-owned fn_800D9D68 retry passed (inside=%u,%u,%u,%u outside=%u,%u,%u,%u)\n",
           (unsigned int)inside[0],
           (unsigned int)inside[1],
           (unsigned int)inside[2],
           (unsigned int)inside[3],
           (unsigned int)outside[0],
           (unsigned int)outside[1],
           (unsigned int)outside[2],
           (unsigned int)outside[3]);
    return 1;
}

static int RunGSgfxSceneLikeSmoke(void) {
    unsigned char panel[4];
    unsigned char header[4];
    unsigned char background[4];

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();
    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    ConfigureSceneLikeDisplayListState();
    fn_800DAD10((void*)&g_sceneLikeDrawObject);
    glFlush();

    ReadBackbufferPixelAt(320, 240, panel);
    ReadBackbufferPixelAt(320, 320, header);
    ReadBackbufferPixelAt(PCPORT_GX_OUTSIDE_X, PCPORT_GX_OUTSIDE_Y, background);

    if (panel[0] != 0xE6 || panel[1] != 0xDC || panel[2] != 0xBC ||
        header[0] != 0xD0 || header[1] != 0x7A || header[2] != 0x20 ||
        background[0] != 0x30 || background[1] != 0xD5 ||
        background[2] != 0x5E) {
        fprintf(stderr,
                "[pcport_bootstrap] GSgfx scene-like display-list smoke failed (panel=%u,%u,%u,%u header=%u,%u,%u,%u bg=%u,%u,%u,%u submitted=%u expanded=%u prim=0x%X)\n",
                (unsigned int)panel[0],
                (unsigned int)panel[1],
                (unsigned int)panel[2],
                (unsigned int)panel[3],
                (unsigned int)header[0],
                (unsigned int)header[1],
                (unsigned int)header[2],
                (unsigned int)header[3],
                (unsigned int)background[0],
                (unsigned int)background[1],
                (unsigned int)background[2],
                (unsigned int)background[3],
                GXHostGetLastSubmittedVertexCount(),
                GXHostGetLastExpandedVertexCount(),
                GXHostGetLastSubmittedPrimitive());
        return 0;
    }

    printf("[pcport_bootstrap] GSgfx scene-like display-list smoke passed (panel=%u,%u,%u,%u header=%u,%u,%u,%u bg=%u,%u,%u,%u)\n",
           (unsigned int)panel[0],
           (unsigned int)panel[1],
           (unsigned int)panel[2],
           (unsigned int)panel[3],
           (unsigned int)header[0],
           (unsigned int)header[1],
           (unsigned int)header[2],
           (unsigned int)header[3],
           (unsigned int)background[0],
           (unsigned int)background[1],
           (unsigned int)background[2],
           (unsigned int)background[3]);
    return 1;
}

static int RunGSgfxPObjSmoke(void) {
    HSD_PObj pobj;
    PCPortGSDrawObject drawObject;
    unsigned char panel[4];
    unsigned char header[4];
    unsigned char background[4];

    InitSceneLikePObj(&pobj);
    memset(&drawObject, 0, sizeof(drawObject));
    drawObject.displayList = pobj.display;
    drawObject.displayListSize = pobj.n_display;
    drawObject.pipelineId = 0;
    drawObject.totalVerts = (unsigned int)(sizeof(g_sceneLikePositions) /
                                           sizeof(g_sceneLikePositions[0]));
    drawObject.totalPrims = 3;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();
    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    fn_801AA568(&pobj);
    fn_800DAD10((void*)&drawObject);
    glFlush();

    ReadBackbufferPixelAt(320, 240, panel);
    ReadBackbufferPixelAt(320, 320, header);
    ReadBackbufferPixelAt(PCPORT_GX_OUTSIDE_X, PCPORT_GX_OUTSIDE_Y, background);

    if (panel[0] != 0xE6 || panel[1] != 0xDC || panel[2] != 0xBC ||
        header[0] != 0xD0 || header[1] != 0x7A || header[2] != 0x20 ||
        background[0] != 0x30 || background[1] != 0xD5 ||
        background[2] != 0x5E) {
        fprintf(stderr,
                "[pcport_bootstrap] GSgfx HSD_PObj smoke failed (panel=%u,%u,%u,%u header=%u,%u,%u,%u bg=%u,%u,%u,%u submitted=%u expanded=%u prim=0x%X)\n",
                (unsigned int)panel[0],
                (unsigned int)panel[1],
                (unsigned int)panel[2],
                (unsigned int)panel[3],
                (unsigned int)header[0],
                (unsigned int)header[1],
                (unsigned int)header[2],
                (unsigned int)header[3],
                (unsigned int)background[0],
                (unsigned int)background[1],
                (unsigned int)background[2],
                (unsigned int)background[3],
                GXHostGetLastSubmittedVertexCount(),
                GXHostGetLastExpandedVertexCount(),
                GXHostGetLastSubmittedPrimitive());
        return 0;
    }

    printf("[pcport_bootstrap] GSgfx HSD_PObj smoke passed (panel=%u,%u,%u,%u header=%u,%u,%u,%u bg=%u,%u,%u,%u)\n",
           (unsigned int)panel[0],
           (unsigned int)panel[1],
           (unsigned int)panel[2],
           (unsigned int)panel[3],
           (unsigned int)header[0],
           (unsigned int)header[1],
           (unsigned int)header[2],
           (unsigned int)header[3],
           (unsigned int)background[0],
           (unsigned int)background[1],
           (unsigned int)background[2],
           (unsigned int)background[3]);
    return 1;
}

static int RunRealContentParserSmoke(void) {
    PCPortHSDArchive archive;
    u8* memberData = NULL;
    u32 memberSize = 0;
    const u8* sceneData;
    u32 sceneOffset = 0;
    u32 sceneWord0;
    u32 sceneWord1;
    u32 sceneWord2;
    u32 sceneWord3;
    unsigned int hostPObjDescSize;
    int ok = 0;

    memset(&archive, 0, sizeof(archive));

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE,
                               PCPORT_REAL_CONTENT_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real content load failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] HSD archive parse failed (%s:%s size=0x%X head=%08X,%08X,%08X,%08X)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER,
                memberSize,
                memberSize >= 0x04u ? PCPort_ReadBigEndianU32(memberData + 0x00) : 0u,
                memberSize >= 0x08u ? PCPort_ReadBigEndianU32(memberData + 0x04) : 0u,
                memberSize >= 0x0Cu ? PCPort_ReadBigEndianU32(memberData + 0x08) : 0u,
                memberSize >= 0x10u ? PCPort_ReadBigEndianU32(memberData + 0x0C) : 0u);
        goto cleanup;
    }

    if (archive.publicCount != 1u || archive.externCount != 0u) {
        fprintf(stderr,
                "[pcport_bootstrap] Unexpected HSD archive table counts (public=%u extern=%u)\n",
                archive.publicCount,
                archive.externCount);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to resolve public symbol scene_data\n");
        goto cleanup;
    }

    sceneWord0 = PCPort_ReadBigEndianU32(sceneData + 0x00);
    sceneWord1 = PCPort_ReadBigEndianU32(sceneData + 0x04);
    sceneWord2 = PCPort_ReadBigEndianU32(sceneData + 0x08);
    sceneWord3 = PCPort_ReadBigEndianU32(sceneData + 0x0C);
    if (sceneWord0 < archive.dataOffset || sceneWord0 >= archive.storageSize ||
        sceneWord1 < archive.dataOffset || sceneWord1 >= archive.storageSize ||
        sceneWord2 < archive.dataOffset || sceneWord2 >= archive.storageSize ||
        sceneWord3 != 0u) {
        fprintf(stderr,
                "[pcport_bootstrap] scene_data root validation failed (w0=0x%X w1=0x%X w2=0x%X w3=0x%X size=0x%X)\n",
                sceneWord0,
                sceneWord1,
                sceneWord2,
                sceneWord3,
                archive.storageSize);
        goto cleanup;
    }

    hostPObjDescSize = (unsigned int)sizeof(HSD_PObjDesc);
    if (hostPObjDescSize <= 0x18u) {
        fprintf(stderr,
                "[pcport_bootstrap] Host HSD_PObjDesc size unexpectedly small (0x%X)\n",
                hostPObjDescSize);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real HSD archive parsed (%s:%s public=scene_data offset=0x%X root=%08X,%08X,%08X,%08X)\n",
           PCPORT_REAL_CONTENT_ARCHIVE,
           PCPORT_REAL_CONTENT_MEMBER,
           sceneOffset,
           sceneWord0,
           sceneWord1,
           sceneWord2,
           sceneWord3);
    printf("[pcport_bootstrap] Direct host HSD consumption still blocked (sizeof(HSD_PObjDesc)=0x%X, serialized GC descriptor layout is 0x18 bytes)\n",
           hostPObjDescSize);
    ok = 1;

cleanup:
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int ArchiveRangeValid(const PCPortHSDArchive* archive,
                             u32 offset, u32 size) {
    if (archive == NULL || archive->storage == NULL || offset < archive->dataOffset ||
        offset > archive->storageSize) {
        return 0;
    }

    return size <= archive->storageSize - offset;
}

static int ResolveFirstRenderablePObjDescFromJoint(const PCPortHSDArchive* archive,
                                                   u32 jointOffset,
                                                   u32* outJointOffset,
                                                   u32* outDObjOffset,
                                                   u32* outPObjOffset) {
    u32 dobjOffset;
    u32 childOffset;
    u32 nextOffset;

    if (!ArchiveRangeValid(archive, jointOffset, PCPORT_SERIALIZED_JOINT_SIZE)) {
        return 0;
    }

    dobjOffset = PCPort_ReadBigEndianU32(archive->storage + jointOffset + 0x10);
    if (ArchiveRangeValid(archive, dobjOffset, PCPORT_SERIALIZED_DOBJ_SIZE)) {
        u32 pobjOffset = PCPort_ReadBigEndianU32(archive->storage + dobjOffset + 0x0C);

        if (ArchiveRangeValid(archive, pobjOffset, PCPORT_SERIALIZED_POBJ_SIZE)) {
            u32 flagsAndDisplayCount = PCPort_ReadBigEndianU32(archive->storage + pobjOffset + 0x0C);
            u32 displayCount = flagsAndDisplayCount & 0xFFFFu;

            if (displayCount > 1u) {
                *outJointOffset = jointOffset;
                *outDObjOffset = dobjOffset;
                *outPObjOffset = pobjOffset;
                return 1;
            }
        }
    }

    childOffset = PCPort_ReadBigEndianU32(archive->storage + jointOffset + 0x08);
    if (childOffset != 0u &&
        ResolveFirstRenderablePObjDescFromJoint(archive, childOffset,
                                                outJointOffset, outDObjOffset,
                                                outPObjOffset)) {
        return 1;
    }

    nextOffset = PCPort_ReadBigEndianU32(archive->storage + jointOffset + 0x0C);
    if (nextOffset != 0u &&
        ResolveFirstRenderablePObjDescFromJoint(archive, nextOffset,
                                                outJointOffset, outDObjOffset,
                                                outPObjOffset)) {
        return 1;
    }

    return 0;
}

static void ConfigureTranslatedMaterialPipeline(unsigned int pipelineId,
                                                const PCPortTranslatedMaterial* material) {
    unsigned char zUpdate;

    GSgfxHostClearPipelineState(pipelineId);
    if (material == NULL) {
        return;
    }

    GSgfxHostSetPipelineAlphaScale(pipelineId, material->alpha);
    zUpdate = (unsigned char)((material->rendermode & PCPORT_RENDER_NO_ZUPDATE) == 0u);

    if ((material->rendermode & PCPORT_RENDER_XLU) != 0u &&
        material->hasPEDesc) {
        GSgfxHostSetPipelineBlend(pipelineId,
                                  material->peType,
                                  material->peSrcFactor,
                                  material->peDstFactor,
                                  material->peLogicOp);
    }

    GSgfxHostSetPipelineZ(pipelineId,
                          1,
                          material->hasPEDesc ? material->peZComp : GX_LEQUAL,
                          zUpdate);

    if (material->hasPEDesc) {
        GSgfxHostSetPipelineAlphaCompare(pipelineId,
                                         material->peAlphaComp0,
                                         material->peRef0,
                                         material->peAlphaOp,
                                         material->peAlphaComp1,
                                         material->peRef1);
    }
}

static void ConfigureTranslatedTexturedPipeline(unsigned int pipelineId,
                                                const PCPortTranslatedMaterial* material,
                                                const PCPortTranslatedTexture* texture,
                                                GXTexObj* textureObject,
                                                unsigned char textureMapId) {
    ConfigureTranslatedMaterialPipeline(pipelineId, material);
    if (texture != NULL && textureObject != NULL) {
        GSgfxHostSetPipelineTexture(pipelineId,
                                    textureObject,
                                    1,
                                    texture->tevMode,
                                    texture->hasCoordId ? texture->coordId : 0u,
                                    textureMapId);
    }
}

static int RunRealContentTranslationSmoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedPObj;
    PCPortTranslatedJointTransform translatedJoint;
    PCPortGSDrawObject drawObject;
    u8* memberData = NULL;
    u32 memberSize = 0;
    const u8* sceneData;
    unsigned char* baselinePixels = NULL;
    unsigned char* drawnPixels = NULL;
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 jointOffset = 0;
    u32 dobjOffset = 0;
    u32 pobjOffset = 0;
    unsigned int diffPixels = 0;
    f32 modelViewMatrix[3][4];
    f32 transformedMin[3];
    f32 transformedMax[3];
    f32 viewMin[3];
    f32 viewMax[3];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedPObj, 0, sizeof(translatedPObj));
    memset(&translatedJoint, 0, sizeof(translatedJoint));
    memset(&drawObject, 0, sizeof(drawObject));

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE,
                               PCPORT_REAL_CONTENT_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real content translation load failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real content translation archive parse failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real content translation failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] scene_data branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real scene camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);
    if (!ResolveFirstRenderablePObjDescFromJoint(&archive,
                                                 rootJointOffset,
                                                 &jointOffset,
                                                 &dobjOffset,
                                                 &pobjOffset)) {
        fprintf(stderr,
                "[pcport_bootstrap] No renderable PObjDesc was found under scene_data (rootJoint=0x%X)\n",
                rootJointOffset);
        goto cleanup;
    }

    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           pobjOffset,
                                           &translatedPObj)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real PObjDesc (joint=0x%X dobj=0x%X pobj=0x%X)\n",
                jointOffset,
                dobjOffset,
                pobjOffset);
        goto cleanup;
    }

    if (!PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              jointOffset,
                                              &translatedJoint)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real joint chain (root=0x%X target=0x%X)\n",
                rootJointOffset,
                jointOffset);
        goto cleanup;
    }

    drawObject.displayList = translatedPObj.pobj.display;
    drawObject.displayListSize = translatedPObj.pobj.n_display;
    drawObject.pipelineId = 0;
    drawObject.totalVerts = translatedPObj.totalSubmittedVertices;
    drawObject.totalPrims = translatedPObj.totalPrimitiveCommands;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();
    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();

    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedJoint.modelMatrix,
                    modelViewMatrix);
    GXLoadPosMtxImm(modelViewMatrix, 0);
    GXSetCurrentMtx(0);

    baselinePixels = ReadBackbufferImage();
    if (baselinePixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture baseline framebuffer for real content translation\n");
        goto cleanup;
    }

    fn_801AA568(&translatedPObj.pobj);
    fn_800DAD10((void*)&drawObject);
    glFlush();

    drawnPixels = ReadBackbufferImage();
    if (drawnPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture translated real-content framebuffer\n");
        goto cleanup;
    }

    TransformBoundsByMtx(translatedPObj.minPosition,
                         translatedPObj.maxPosition,
                         translatedJoint.modelMatrix,
                         transformedMin,
                         transformedMax);
    TransformBoundsByMtx(translatedPObj.minPosition,
                         translatedPObj.maxPosition,
                         modelViewMatrix,
                         viewMin,
                         viewMax);
    diffPixels = CountFramebufferDiffPixels(baselinePixels, drawnPixels);
    if (diffPixels == 0u) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene camera bridge reached fn_800DAD10 but changed no framebuffer pixels (scene=0x%X camera=0x%X eye=0x%X interest=0x%X joint=0x%X dobj=0x%X pobj=0x%X submitted=%u expanded=%u prim=0x%X local=[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f] world=[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f] view=[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f] eye=(%.3f,%.3f,%.3f) at=(%.3f,%.3f,%.3f) fov=%.3f aspect=%.3f near=%.3f far=%.3f)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                translatedCamera.eyeArchiveOffset,
                translatedCamera.interestArchiveOffset,
                jointOffset,
                dobjOffset,
                pobjOffset,
                GXHostGetLastSubmittedVertexCount(),
                GXHostGetLastExpandedVertexCount(),
                GXHostGetLastSubmittedPrimitive(),
                translatedPObj.minPosition[0],
                translatedPObj.minPosition[1],
                translatedPObj.minPosition[2],
                translatedPObj.maxPosition[0],
                translatedPObj.maxPosition[1],
                translatedPObj.maxPosition[2],
                transformedMin[0],
                transformedMin[1],
                transformedMin[2],
                transformedMax[0],
                transformedMax[1],
                transformedMax[2],
                viewMin[0],
                viewMin[1],
                viewMin[2],
                viewMax[0],
                viewMax[1],
                viewMax[2],
                translatedCamera.eye[0],
                translatedCamera.eye[1],
                translatedCamera.eye[2],
                translatedCamera.interest[0],
                translatedCamera.interest[1],
                translatedCamera.interest[2],
                translatedCamera.fov,
                translatedCamera.aspect,
                translatedCamera.nearZ,
                translatedCamera.farZ);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real scene camera bridge smoke passed (scene=0x%X camera=0x%X joint=0x%X dobj=0x%X pobj=0x%X changedPixels=%u submitted=%u expanded=%u prim=0x%X local=[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f] world=[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f] view=[%.3f,%.3f,%.3f]-[%.3f,%.3f,%.3f])\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           jointOffset,
           dobjOffset,
           pobjOffset,
           diffPixels,
           GXHostGetLastSubmittedVertexCount(),
           GXHostGetLastExpandedVertexCount(),
           GXHostGetLastSubmittedPrimitive(),
           translatedPObj.minPosition[0],
           translatedPObj.minPosition[1],
           translatedPObj.minPosition[2],
           translatedPObj.maxPosition[0],
           translatedPObj.maxPosition[1],
           translatedPObj.maxPosition[2],
           transformedMin[0],
           transformedMin[1],
           transformedMin[2],
           transformedMax[0],
           transformedMax[1],
           transformedMax[2],
           viewMin[0],
           viewMin[1],
           viewMin[2],
           viewMax[0],
           viewMax[1],
           viewMax[2]);
    ok = 1;

cleanup:
    free(drawnPixels);
    free(baselinePixels);
    PCPort_DestroyTranslatedPObj(&translatedPObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int RunRealSceneSlice2Smoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedPObj;
    PCPortTranslatedJointTransform translatedJoint;
    PCPortTranslatedMaterial translatedMaterial;
    PCPortGSDrawObject opaqueDrawObject;
    PCPortGSDrawObject materialDrawObject;
    u8* memberData = NULL;
    u32 memberSize = 0;
    const u8* sceneData;
    unsigned char* opaquePixels = NULL;
    unsigned char* materialPixels = NULL;
    unsigned char opaqueSample[4] = { 0 };
    unsigned char materialSample[4] = { 0 };
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 jointOffset = 0;
    u32 dobjOffset = 0;
    u32 pobjOffset = 0;
    u32 mobjOffset = 0;
    unsigned int diffPixels = 0;
    f32 modelViewMatrix[3][4];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedPObj, 0, sizeof(translatedPObj));
    memset(&translatedJoint, 0, sizeof(translatedJoint));
    memset(&translatedMaterial, 0, sizeof(translatedMaterial));
    memset(&opaqueDrawObject, 0, sizeof(opaqueDrawObject));
    memset(&materialDrawObject, 0, sizeof(materialDrawObject));

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE,
                               PCPORT_REAL_CONTENT_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 load failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 archive parse failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 failed to translate camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);
    if (!ResolveFirstRenderablePObjDescFromJoint(&archive,
                                                 rootJointOffset,
                                                 &jointOffset,
                                                 &dobjOffset,
                                                 &pobjOffset)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 found no renderable PObjDesc (rootJoint=0x%X)\n",
                rootJointOffset);
        goto cleanup;
    }

    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           pobjOffset,
                                           &translatedPObj)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 failed to translate PObjDesc (joint=0x%X dobj=0x%X pobj=0x%X)\n",
                jointOffset,
                dobjOffset,
                pobjOffset);
        goto cleanup;
    }

    if (!PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              jointOffset,
                                              &translatedJoint)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 failed to translate joint chain (root=0x%X target=0x%X)\n",
                rootJointOffset,
                jointOffset);
        goto cleanup;
    }

    mobjOffset = PCPort_ReadBigEndianU32(archive.storage + dobjOffset + 0x08);
    if (!PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               mobjOffset,
                                               &translatedMaterial)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 failed to translate MObjDesc (dobj=0x%X mobj=0x%X)\n",
                dobjOffset,
                mobjOffset);
        goto cleanup;
    }

    opaqueDrawObject.displayList = translatedPObj.pobj.display;
    opaqueDrawObject.displayListSize = translatedPObj.pobj.n_display;
    opaqueDrawObject.pipelineId = 0;
    opaqueDrawObject.totalVerts = translatedPObj.totalSubmittedVertices;
    opaqueDrawObject.totalPrims = translatedPObj.totalPrimitiveCommands;
    materialDrawObject = opaqueDrawObject;
    materialDrawObject.pipelineId = PCPORT_REAL_MATERIAL_PIPELINE;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);

    GXHostSetVertexAlphaScale(1.0f);
    VIWaitForRetrace_PC();
    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();

    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedJoint.modelMatrix,
                    modelViewMatrix);
    GXLoadPosMtxImm(modelViewMatrix, 0);
    GXSetCurrentMtx(0);

    fn_801AA568(&translatedPObj.pobj);
    fn_800DAD10((void*)&opaqueDrawObject);
    glFlush();

    opaquePixels = ReadBackbufferImage();
    if (opaquePixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 failed to capture opaque framebuffer\n");
        goto cleanup;
    }
    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          opaqueSample);

    ConfigureTranslatedMaterialPipeline(PCPORT_REAL_MATERIAL_PIPELINE,
                                        &translatedMaterial);
    GXHostSetVertexAlphaScale(1.0f);
    VIWaitForRetrace_PC();
    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();

    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);
    GXLoadPosMtxImm(modelViewMatrix, 0);
    GXSetCurrentMtx(0);

    fn_801AA568(&translatedPObj.pobj);
    fn_800DAD10((void*)&materialDrawObject);
    glFlush();

    materialPixels = ReadBackbufferImage();
    if (materialPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 failed to capture material framebuffer\n");
        goto cleanup;
    }
    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          materialSample);

    diffPixels = CountFramebufferDiffPixels(opaquePixels, materialPixels);
    if (diffPixels == 0u) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 2 material path produced no framebuffer delta (scene=0x%X camera=0x%X joint=0x%X dobj=0x%X mobj=0x%X pobj=0x%X opaque=%u,%u,%u,%u material=%u,%u,%u,%u alpha=%.3f blend=%u/%u/%u z=%u update=%u)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                jointOffset,
                dobjOffset,
                mobjOffset,
                pobjOffset,
                opaqueSample[0],
                opaqueSample[1],
                opaqueSample[2],
                opaqueSample[3],
                materialSample[0],
                materialSample[1],
                materialSample[2],
                materialSample[3],
                translatedMaterial.alpha,
                translatedMaterial.peType,
                translatedMaterial.peSrcFactor,
                translatedMaterial.peDstFactor,
                translatedMaterial.peZComp,
                (translatedMaterial.rendermode & PCPORT_RENDER_NO_ZUPDATE) == 0u);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real scene slice 2 smoke passed (scene=0x%X camera=0x%X joint=0x%X dobj=0x%X mobj=0x%X pobj=0x%X diffPixels=%u opaque=%u,%u,%u,%u material=%u,%u,%u,%u alpha=%.3f blend=%u/%u/%u z=%u update=%u submitted=%u expanded=%u prim=0x%X)\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           jointOffset,
           dobjOffset,
           mobjOffset,
           pobjOffset,
           diffPixels,
           opaqueSample[0],
           opaqueSample[1],
           opaqueSample[2],
           opaqueSample[3],
           materialSample[0],
           materialSample[1],
           materialSample[2],
           materialSample[3],
           translatedMaterial.alpha,
           translatedMaterial.peType,
           translatedMaterial.peSrcFactor,
           translatedMaterial.peDstFactor,
           translatedMaterial.peZComp,
           (translatedMaterial.rendermode & PCPORT_RENDER_NO_ZUPDATE) == 0u,
           GXHostGetLastSubmittedVertexCount(),
           GXHostGetLastExpandedVertexCount(),
           GXHostGetLastSubmittedPrimitive());
    ok = 1;

cleanup:
    GSgfxHostClearPipelineState(PCPORT_REAL_MATERIAL_PIPELINE);
    GXHostSetVertexAlphaScale(1.0f);
    free(materialPixels);
    free(opaquePixels);
    PCPort_DestroyTranslatedPObj(&translatedPObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int RunRealTexturedSceneSliceSmoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedBasePObj;
    PCPortTranslatedJointTransform translatedBaseJoint;
    PCPortTranslatedMaterial translatedBaseMaterial;
    PCPortTranslatedPObj translatedTexturedPObj;
    PCPortTranslatedJointTransform translatedTexturedJoint;
    PCPortTranslatedMaterial translatedTexturedMaterial;
    PCPortTranslatedTextureExp translatedTextureExp;
    PCPortGSDrawObject baseDrawObject;
    PCPortGSDrawObject texturedDrawObject;
    GXTexObj textureObject;
    u8* memberData = NULL;
    u8* bakedTexturePixels = NULL;
    u32 memberSize = 0;
    u32 bakedTextureSize = 0;
    const u8* sceneData;
    unsigned char* materialPixels = NULL;
    unsigned char* texturedPixels = NULL;
    unsigned char materialOutside[4] = { 0 };
    unsigned char texturedOutside[4] = { 0 };
    unsigned char texturedCenter[4] = { 0 };
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 baseJointOffset = 0;
    u32 baseDObjOffset = 0;
    u32 basePObjOffset = 0;
    u32 baseMObjOffset = 0;
    u32 texturedTObjOffset = 0;
    u32 textureMapId = 0;
    unsigned int diffPixels = 0;
    unsigned int texturedSubmitted = 0;
    unsigned int texturedExpanded = 0;
    unsigned int texturedPrimitive = 0;
    f32 baseModelViewMatrix[3][4];
    f32 texturedModelViewMatrix[3][4];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedBasePObj, 0, sizeof(translatedBasePObj));
    memset(&translatedBaseJoint, 0, sizeof(translatedBaseJoint));
    memset(&translatedBaseMaterial, 0, sizeof(translatedBaseMaterial));
    memset(&translatedTexturedPObj, 0, sizeof(translatedTexturedPObj));
    memset(&translatedTexturedJoint, 0, sizeof(translatedTexturedJoint));
    memset(&translatedTexturedMaterial, 0, sizeof(translatedTexturedMaterial));
    memset(&translatedTextureExp, 0, sizeof(translatedTextureExp));
    memset(&baseDrawObject, 0, sizeof(baseDrawObject));
    memset(&texturedDrawObject, 0, sizeof(texturedDrawObject));
    memset(&textureObject, 0, sizeof(textureObject));

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE,
                               PCPORT_REAL_CONTENT_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real textured scene slice load failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real textured scene slice archive parse failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real textured scene slice failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real textured scene slice branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real textured scene camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);
    if (!ResolveFirstRenderablePObjDescFromJoint(&archive,
                                                 rootJointOffset,
                                                 &baseJointOffset,
                                                 &baseDObjOffset,
                                                 &basePObjOffset)) {
        fprintf(stderr,
                "[pcport_bootstrap] No baseline renderable PObjDesc was found under scene_data (rootJoint=0x%X)\n",
                rootJointOffset);
        goto cleanup;
    }

    baseMObjOffset = PCPort_ReadBigEndianU32(archive.storage + baseDObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           basePObjOffset,
                                           &translatedBasePObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              baseJointOffset,
                                              &translatedBaseJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               baseMObjOffset,
                                               &translatedBaseMaterial)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate baseline real-content path (joint=0x%X dobj=0x%X mobj=0x%X pobj=0x%X)\n",
                baseJointOffset,
                baseDObjOffset,
                baseMObjOffset,
                basePObjOffset);
        goto cleanup;
    }

    texturedTObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_TEXTURED_MOBJ_OFFSET + 0x08);
    textureMapId = PCPort_ReadBigEndianU32(archive.storage + texturedTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_TEXTURED_POBJ_OFFSET,
                                           &translatedTexturedPObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_TEXTURED_JOINT_OFFSET,
                                              &translatedTexturedJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_TEXTURED_MOBJ_OFFSET,
                                               &translatedTexturedMaterial) ||
        !PCPort_TranslateTextureExpFromArchiveBE(&archive,
                                                 texturedTObjOffset,
                                                 &translatedTextureExp)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate textured front branch (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_TEXTURED_JOINT_OFFSET,
                PCPORT_TEXTURED_DOBJ_OFFSET,
                PCPORT_TEXTURED_MOBJ_OFFSET,
                texturedTObjOffset,
                PCPORT_TEXTURED_POBJ_OFFSET);
        goto cleanup;
    }

    if (translatedTextureExp.kind != PCPORT_TEXTURE_EXP_KIND_DIRECT_SAMPLE ||
        translatedTextureExp.stageCount != 1u ||
        translatedTextureExp.stages[0].kind != PCPORT_TEXP_STAGE_DIRECT_SAMPLE) {
        fprintf(stderr,
                "[pcport_bootstrap] Real textured scene slice did not reach the expected shared direct-sample TExp boundary (texTObj=0x%X kind=%u stage0=%u stages=%u)\n",
                texturedTObjOffset,
                translatedTextureExp.kind,
                translatedTextureExp.stageCount != 0u ? translatedTextureExp.stages[0].kind : 0u,
                translatedTextureExp.stageCount);
        goto cleanup;
    }

    if (!PCPort_BakeTextureExpRGBAFromArchiveBE(&archive,
                                                &translatedTextureExp,
                                                &bakedTexturePixels,
                                                &bakedTextureSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to bake shared direct-sample TExp payload (texTObj=0x%X stage0=%u stages=%u)\n",
                texturedTObjOffset,
                translatedTextureExp.stages[0].kind,
                translatedTextureExp.stageCount);
        goto cleanup;
    }

    GXHostInitTexObjRGBA8(&textureObject,
                          bakedTexturePixels,
                          translatedTextureExp.stages[0].texture.width,
                          translatedTextureExp.stages[0].texture.height,
                          (GXTexWrapMode)translatedTextureExp.stages[0].texture.wrapS,
                          (GXTexWrapMode)translatedTextureExp.stages[0].texture.wrapT);

    baseDrawObject.displayList = translatedBasePObj.pobj.display;
    baseDrawObject.displayListSize = translatedBasePObj.pobj.n_display;
    baseDrawObject.pipelineId = PCPORT_REAL_MATERIAL_PIPELINE;
    baseDrawObject.totalVerts = translatedBasePObj.totalSubmittedVertices;
    baseDrawObject.totalPrims = translatedBasePObj.totalPrimitiveCommands;

    texturedDrawObject.displayList = translatedTexturedPObj.pobj.display;
    texturedDrawObject.displayListSize = translatedTexturedPObj.pobj.n_display;
    texturedDrawObject.pipelineId = PCPORT_REAL_TEXTURED_PIPELINE;
    texturedDrawObject.totalVerts = translatedTexturedPObj.totalSubmittedVertices;
    texturedDrawObject.totalPrims = translatedTexturedPObj.totalPrimitiveCommands;

    ConfigureTranslatedMaterialPipeline(PCPORT_REAL_MATERIAL_PIPELINE,
                                        &translatedBaseMaterial);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedTexturedMaterial,
                                        &translatedTextureExp.stages[0].texture,
                                        &textureObject,
                                        (unsigned char)textureMapId);

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();

    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedBaseJoint.modelMatrix,
                    baseModelViewMatrix);
    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);

    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    materialPixels = ReadBackbufferImage();
    if (materialPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture baseline framebuffer for textured scene slice\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_OUTSIDE_X,
                          PCPORT_GX_OUTSIDE_Y,
                          materialOutside);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();

    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedTexturedJoint.modelMatrix,
                    texturedModelViewMatrix);
    GXLoadPosMtxImm(texturedModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedTexturedPObj.pobj);
    fn_800DAD10((void*)&texturedDrawObject);
    glFlush();

    texturedSubmitted = GXHostGetLastSubmittedVertexCount();
    texturedExpanded = GXHostGetLastExpandedVertexCount();
    texturedPrimitive = GXHostGetLastSubmittedPrimitive();

    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    texturedPixels = ReadBackbufferImage();
    if (texturedPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture textured framebuffer for scene slice\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_OUTSIDE_X,
                          PCPORT_GX_OUTSIDE_Y,
                          texturedOutside);
    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          texturedCenter);

    diffPixels = CountFramebufferDiffPixels(materialPixels, texturedPixels);
    if (diffPixels == 0u ||
        (materialOutside[0] == texturedOutside[0] &&
         materialOutside[1] == texturedOutside[1] &&
         materialOutside[2] == texturedOutside[2] &&
         materialOutside[3] == texturedOutside[3])) {
        fprintf(stderr,
                "[pcport_bootstrap] Real textured scene slice reached the draw bridge but did not produce a distinct textured result (scene=0x%X camera=0x%X baseJoint=0x%X baseDObj=0x%X baseMObj=0x%X basePObj=0x%X texJoint=0x%X texDObj=0x%X texMObj=0x%X texTObj=0x%X texImage=0x%X texData=0x%X texPObj=0x%X stage0=%u stages=%u diffPixels=%u baseOutside=%u,%u,%u,%u texturedOutside=%u,%u,%u,%u center=%u,%u,%u,%u tev=%u size=%ux%u format=%u baked=%u submitted=%u expanded=%u prim=0x%X)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                baseJointOffset,
                baseDObjOffset,
                baseMObjOffset,
                basePObjOffset,
                PCPORT_TEXTURED_JOINT_OFFSET,
                PCPORT_TEXTURED_DOBJ_OFFSET,
                PCPORT_TEXTURED_MOBJ_OFFSET,
                translatedTextureExp.stages[0].texture.tobjArchiveOffset,
                translatedTextureExp.stages[0].texture.imageArchiveOffset,
                translatedTextureExp.stages[0].texture.imageDataArchiveOffset,
                PCPORT_TEXTURED_POBJ_OFFSET,
                translatedTextureExp.stages[0].kind,
                translatedTextureExp.stageCount,
                diffPixels,
                materialOutside[0],
                materialOutside[1],
                materialOutside[2],
                materialOutside[3],
                texturedOutside[0],
                texturedOutside[1],
                texturedOutside[2],
                texturedOutside[3],
                texturedCenter[0],
                texturedCenter[1],
                texturedCenter[2],
                texturedCenter[3],
                translatedTextureExp.stages[0].texture.tevMode,
                translatedTextureExp.stages[0].texture.width,
                translatedTextureExp.stages[0].texture.height,
                translatedTextureExp.stages[0].texture.format,
                bakedTextureSize,
                texturedSubmitted,
                texturedExpanded,
                texturedPrimitive);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real textured scene slice smoke passed (scene=0x%X camera=0x%X baseJoint=0x%X baseDObj=0x%X baseMObj=0x%X basePObj=0x%X texJoint=0x%X texDObj=0x%X texMObj=0x%X texTObj=0x%X texImage=0x%X texData=0x%X texPObj=0x%X kind=%u stage0=%u stages=%u diffPixels=%u baseOutside=%u,%u,%u,%u texturedOutside=%u,%u,%u,%u center=%u,%u,%u,%u tev=%u size=%ux%u format=%u baked=%u submitted=%u expanded=%u prim=0x%X)\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           baseJointOffset,
           baseDObjOffset,
           baseMObjOffset,
           basePObjOffset,
           PCPORT_TEXTURED_JOINT_OFFSET,
           PCPORT_TEXTURED_DOBJ_OFFSET,
           PCPORT_TEXTURED_MOBJ_OFFSET,
           translatedTextureExp.stages[0].texture.tobjArchiveOffset,
           translatedTextureExp.stages[0].texture.imageArchiveOffset,
           translatedTextureExp.stages[0].texture.imageDataArchiveOffset,
           PCPORT_TEXTURED_POBJ_OFFSET,
           translatedTextureExp.kind,
           translatedTextureExp.stages[0].kind,
           translatedTextureExp.stageCount,
           diffPixels,
           materialOutside[0],
           materialOutside[1],
           materialOutside[2],
           materialOutside[3],
           texturedOutside[0],
           texturedOutside[1],
           texturedOutside[2],
           texturedOutside[3],
           texturedCenter[0],
           texturedCenter[1],
           texturedCenter[2],
           texturedCenter[3],
           translatedTextureExp.stages[0].texture.tevMode,
           translatedTextureExp.stages[0].texture.width,
           translatedTextureExp.stages[0].texture.height,
           translatedTextureExp.stages[0].texture.format,
           bakedTextureSize,
           texturedSubmitted,
           texturedExpanded,
           texturedPrimitive);
    ok = 1;

cleanup:
    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_MATERIAL_PIPELINE);
    GXHostClearTextureBinding();
    GXHostSetVertexAlphaScale(1.0f);
    PCPort_FreeBuffer(bakedTexturePixels);
    free(texturedPixels);
    free(materialPixels);
    PCPort_DestroyTranslatedPObj(&translatedTexturedPObj);
    PCPort_DestroyTranslatedPObj(&translatedBasePObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int RunRealSceneSlice3Smoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedBasePObj;
    PCPortTranslatedJointTransform translatedBaseJoint;
    PCPortTranslatedMaterial translatedBaseMaterial;
    PCPortTranslatedPObj translatedFrontPObj;
    PCPortTranslatedJointTransform translatedFrontJoint;
    PCPortTranslatedMaterial translatedFrontMaterial;
    PCPortTranslatedTexture translatedFrontTexture;
    PCPortTranslatedPObj translatedSiblingPObj;
    PCPortTranslatedJointTransform translatedSiblingJoint;
    PCPortTranslatedMaterial translatedSiblingMaterial;
    PCPortTranslatedTexture translatedSiblingTexture;
    PCPortGSDrawObject baseDrawObject;
    PCPortGSDrawObject frontDrawObject;
    PCPortGSDrawObject siblingDrawObject;
    GXTexObj frontTextureObject;
    GXTexObj siblingTextureObject;
    u8* memberData = NULL;
    u32 memberSize = 0;
    const u8* sceneData;
    unsigned char* baselinePixels = NULL;
    unsigned char* multiPixels = NULL;
    unsigned char baselineCenter[4] = { 0 };
    unsigned char multiCenter[4] = { 0 };
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 baseJointOffset = 0;
    u32 baseDObjOffset = 0;
    u32 basePObjOffset = 0;
    u32 baseMObjOffset = 0;
    u32 frontTObjOffset = 0;
    u32 frontTextureMapId = 0;
    u32 siblingTObjOffset = 0;
    u32 siblingTextureMapId = 0;
    unsigned int diffPixels = 0;
    unsigned int siblingSubmitted = 0;
    unsigned int siblingExpanded = 0;
    unsigned int siblingPrimitive = 0;
    f32 baseModelViewMatrix[3][4];
    f32 frontModelViewMatrix[3][4];
    f32 siblingModelViewMatrix[3][4];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedBasePObj, 0, sizeof(translatedBasePObj));
    memset(&translatedBaseJoint, 0, sizeof(translatedBaseJoint));
    memset(&translatedBaseMaterial, 0, sizeof(translatedBaseMaterial));
    memset(&translatedFrontPObj, 0, sizeof(translatedFrontPObj));
    memset(&translatedFrontJoint, 0, sizeof(translatedFrontJoint));
    memset(&translatedFrontMaterial, 0, sizeof(translatedFrontMaterial));
    memset(&translatedFrontTexture, 0, sizeof(translatedFrontTexture));
    memset(&translatedSiblingPObj, 0, sizeof(translatedSiblingPObj));
    memset(&translatedSiblingJoint, 0, sizeof(translatedSiblingJoint));
    memset(&translatedSiblingMaterial, 0, sizeof(translatedSiblingMaterial));
    memset(&translatedSiblingTexture, 0, sizeof(translatedSiblingTexture));
    memset(&baseDrawObject, 0, sizeof(baseDrawObject));
    memset(&frontDrawObject, 0, sizeof(frontDrawObject));
    memset(&siblingDrawObject, 0, sizeof(siblingDrawObject));
    memset(&frontTextureObject, 0, sizeof(frontTextureObject));
    memset(&siblingTextureObject, 0, sizeof(siblingTextureObject));

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE,
                               PCPORT_REAL_CONTENT_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 3 load failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 3 archive parse failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 3 failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 3 branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real scene slice 3 camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);
    if (!ResolveFirstRenderablePObjDescFromJoint(&archive,
                                                 rootJointOffset,
                                                 &baseJointOffset,
                                                 &baseDObjOffset,
                                                 &basePObjOffset)) {
        fprintf(stderr,
                "[pcport_bootstrap] No baseline renderable PObjDesc was found for scene slice 3 (rootJoint=0x%X)\n",
                rootJointOffset);
        goto cleanup;
    }

    baseMObjOffset = PCPort_ReadBigEndianU32(archive.storage + baseDObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           basePObjOffset,
                                           &translatedBasePObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              baseJointOffset,
                                              &translatedBaseJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               baseMObjOffset,
                                               &translatedBaseMaterial)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate baseline branch for scene slice 3 (joint=0x%X dobj=0x%X mobj=0x%X pobj=0x%X)\n",
                baseJointOffset,
                baseDObjOffset,
                baseMObjOffset,
                basePObjOffset);
        goto cleanup;
    }

    frontTObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_TEXTURED_MOBJ_OFFSET + 0x08);
    frontTextureMapId = PCPort_ReadBigEndianU32(archive.storage + frontTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_TEXTURED_POBJ_OFFSET,
                                           &translatedFrontPObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_TEXTURED_JOINT_OFFSET,
                                              &translatedFrontJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_TEXTURED_MOBJ_OFFSET,
                                               &translatedFrontMaterial) ||
        !PCPort_TranslateTextureFromArchiveBE(&archive,
                                              frontTObjOffset,
                                              &translatedFrontTexture)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate front textured branch for scene slice 3 (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_TEXTURED_JOINT_OFFSET,
                PCPORT_TEXTURED_DOBJ_OFFSET,
                PCPORT_TEXTURED_MOBJ_OFFSET,
                frontTObjOffset,
                PCPORT_TEXTURED_POBJ_OFFSET);
        goto cleanup;
    }

    siblingTObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_SIBLING_MOBJ_OFFSET + 0x08);
    siblingTextureMapId = PCPort_ReadBigEndianU32(archive.storage + siblingTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_SIBLING_POBJ_OFFSET,
                                           &translatedSiblingPObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_SIBLING_JOINT_OFFSET,
                                              &translatedSiblingJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_SIBLING_MOBJ_OFFSET,
                                               &translatedSiblingMaterial) ||
        !PCPort_TranslateTextureFromArchiveBE(&archive,
                                              siblingTObjOffset,
                                              &translatedSiblingTexture)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate sibling branch for scene slice 3 (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_SIBLING_JOINT_OFFSET,
                PCPORT_SIBLING_DOBJ_OFFSET,
                PCPORT_SIBLING_MOBJ_OFFSET,
                siblingTObjOffset,
                PCPORT_SIBLING_POBJ_OFFSET);
        goto cleanup;
    }

    GXInitTexObj(&frontTextureObject,
                 archive.storage + translatedFrontTexture.imageDataArchiveOffset,
                 translatedFrontTexture.width,
                 translatedFrontTexture.height,
                 (GXTexFmt)translatedFrontTexture.format,
                 (GXTexWrapMode)translatedFrontTexture.wrapS,
                 (GXTexWrapMode)translatedFrontTexture.wrapT,
                 translatedFrontTexture.mipmap);

    GXInitTexObj(&siblingTextureObject,
                 archive.storage + translatedSiblingTexture.imageDataArchiveOffset,
                 translatedSiblingTexture.width,
                 translatedSiblingTexture.height,
                 (GXTexFmt)translatedSiblingTexture.format,
                 (GXTexWrapMode)translatedSiblingTexture.wrapS,
                 (GXTexWrapMode)translatedSiblingTexture.wrapT,
                 translatedSiblingTexture.mipmap);

    baseDrawObject.displayList = translatedBasePObj.pobj.display;
    baseDrawObject.displayListSize = translatedBasePObj.pobj.n_display;
    baseDrawObject.pipelineId = PCPORT_REAL_MATERIAL_PIPELINE;
    baseDrawObject.totalVerts = translatedBasePObj.totalSubmittedVertices;
    baseDrawObject.totalPrims = translatedBasePObj.totalPrimitiveCommands;

    frontDrawObject.displayList = translatedFrontPObj.pobj.display;
    frontDrawObject.displayListSize = translatedFrontPObj.pobj.n_display;
    frontDrawObject.pipelineId = PCPORT_REAL_TEXTURED_PIPELINE;
    frontDrawObject.totalVerts = translatedFrontPObj.totalSubmittedVertices;
    frontDrawObject.totalPrims = translatedFrontPObj.totalPrimitiveCommands;

    siblingDrawObject.displayList = translatedSiblingPObj.pobj.display;
    siblingDrawObject.displayListSize = translatedSiblingPObj.pobj.n_display;
    siblingDrawObject.pipelineId = PCPORT_REAL_SIBLING_TEXTURED_PIPELINE;
    siblingDrawObject.totalVerts = translatedSiblingPObj.totalSubmittedVertices;
    siblingDrawObject.totalPrims = translatedSiblingPObj.totalPrimitiveCommands;

    ConfigureTranslatedMaterialPipeline(PCPORT_REAL_MATERIAL_PIPELINE,
                                        &translatedBaseMaterial);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedFrontMaterial,
                                        &translatedFrontTexture,
                                        &frontTextureObject,
                                        (unsigned char)frontTextureMapId);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE,
                                        &translatedSiblingMaterial,
                                        &translatedSiblingTexture,
                                        &siblingTextureObject,
                                        (unsigned char)siblingTextureMapId);

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();

    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedFrontJoint.modelMatrix,
                    frontModelViewMatrix);
    GXLoadPosMtxImm(frontModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedFrontPObj.pobj);
    fn_800DAD10((void*)&frontDrawObject);
    glFlush();

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedBaseJoint.modelMatrix,
                    baseModelViewMatrix);
    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    baselinePixels = ReadBackbufferImage();
    if (baselinePixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture baseline framebuffer for scene slice 3\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          baselineCenter);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();

    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(frontModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedFrontPObj.pobj);
    fn_800DAD10((void*)&frontDrawObject);
    glFlush();

    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedSiblingJoint.modelMatrix,
                    siblingModelViewMatrix);
    GXLoadPosMtxImm(siblingModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedSiblingPObj.pobj);
    fn_800DAD10((void*)&siblingDrawObject);
    glFlush();

    siblingSubmitted = GXHostGetLastSubmittedVertexCount();
    siblingExpanded = GXHostGetLastExpandedVertexCount();
    siblingPrimitive = GXHostGetLastSubmittedPrimitive();

    multiPixels = ReadBackbufferImage();
    if (multiPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture multi-object framebuffer for scene slice 3\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          multiCenter);

    diffPixels = CountFramebufferDiffPixels(baselinePixels, multiPixels);
    if (diffPixels == 0u &&
        baselineCenter[0] == multiCenter[0] &&
        baselineCenter[1] == multiCenter[1] &&
        baselineCenter[2] == multiCenter[2] &&
        baselineCenter[3] == multiCenter[3]) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 3 reached the sibling draw path but did not change the current textured scene (scene=0x%X camera=0x%X baseJoint=0x%X frontJoint=0x%X siblingJoint=0x%X siblingDObj=0x%X siblingMObj=0x%X siblingTObj=0x%X siblingImage=0x%X siblingData=0x%X siblingPObj=0x%X baselineCenter=%u,%u,%u,%u multiCenter=%u,%u,%u,%u format=%u diffPixels=%u submitted=%u expanded=%u prim=0x%X)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                baseJointOffset,
                PCPORT_TEXTURED_JOINT_OFFSET,
                PCPORT_SIBLING_JOINT_OFFSET,
                PCPORT_SIBLING_DOBJ_OFFSET,
                PCPORT_SIBLING_MOBJ_OFFSET,
                translatedSiblingTexture.tobjArchiveOffset,
                translatedSiblingTexture.imageArchiveOffset,
                translatedSiblingTexture.imageDataArchiveOffset,
                PCPORT_SIBLING_POBJ_OFFSET,
                baselineCenter[0],
                baselineCenter[1],
                baselineCenter[2],
                baselineCenter[3],
                multiCenter[0],
                multiCenter[1],
                multiCenter[2],
                multiCenter[3],
                translatedSiblingTexture.format,
                diffPixels,
                siblingSubmitted,
                siblingExpanded,
                siblingPrimitive);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real scene slice 3 smoke passed (scene=0x%X camera=0x%X frontJoint=0x%X frontDObj=0x%X frontMObj=0x%X frontTObj=0x%X frontPObj=0x%X baseJoint=0x%X baseDObj=0x%X baseMObj=0x%X basePObj=0x%X siblingJoint=0x%X siblingDObj=0x%X siblingMObj=0x%X siblingTObj=0x%X siblingImage=0x%X siblingData=0x%X siblingPObj=0x%X diffPixels=%u baselineCenter=%u,%u,%u,%u multiCenter=%u,%u,%u,%u siblingFmt=%u siblingSize=%ux%u submitted=%u expanded=%u prim=0x%X)\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           PCPORT_TEXTURED_JOINT_OFFSET,
           PCPORT_TEXTURED_DOBJ_OFFSET,
           PCPORT_TEXTURED_MOBJ_OFFSET,
           translatedFrontTexture.tobjArchiveOffset,
           PCPORT_TEXTURED_POBJ_OFFSET,
           baseJointOffset,
           baseDObjOffset,
           baseMObjOffset,
           basePObjOffset,
           PCPORT_SIBLING_JOINT_OFFSET,
           PCPORT_SIBLING_DOBJ_OFFSET,
           PCPORT_SIBLING_MOBJ_OFFSET,
           translatedSiblingTexture.tobjArchiveOffset,
           translatedSiblingTexture.imageArchiveOffset,
           translatedSiblingTexture.imageDataArchiveOffset,
           PCPORT_SIBLING_POBJ_OFFSET,
           diffPixels,
           baselineCenter[0],
           baselineCenter[1],
           baselineCenter[2],
           baselineCenter[3],
           multiCenter[0],
           multiCenter[1],
           multiCenter[2],
           multiCenter[3],
           translatedSiblingTexture.format,
           translatedSiblingTexture.width,
           translatedSiblingTexture.height,
           siblingSubmitted,
           siblingExpanded,
           siblingPrimitive);
    ok = 1;

cleanup:
    GSgfxHostClearPipelineState(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_MATERIAL_PIPELINE);
    GXHostClearTextureBinding();
    GXHostSetVertexAlphaScale(1.0f);
    free(multiPixels);
    free(baselinePixels);
    PCPort_DestroyTranslatedPObj(&translatedSiblingPObj);
    PCPort_DestroyTranslatedPObj(&translatedFrontPObj);
    PCPort_DestroyTranslatedPObj(&translatedBasePObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int RunRealTevSceneSliceSmoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedBasePObj;
    PCPortTranslatedJointTransform translatedBaseJoint;
    PCPortTranslatedMaterial translatedBaseMaterial;
    PCPortTranslatedPObj translatedFrontPObj;
    PCPortTranslatedJointTransform translatedFrontJoint;
    PCPortTranslatedMaterial translatedFrontMaterial;
    PCPortTranslatedTexture translatedFrontTexture;
    PCPortTranslatedPObj translatedSiblingPObj;
    PCPortTranslatedJointTransform translatedSiblingJoint;
    PCPortTranslatedMaterial translatedSiblingMaterial;
    PCPortTranslatedTexture translatedSiblingTexture;
    PCPortGSDrawObject baseDrawObject;
    PCPortGSDrawObject frontDrawObject;
    PCPortGSDrawObject siblingDrawObject;
    GXTexObj frontTextureObject;
    GXTexObj siblingRawTextureObject;
    GXTexObj siblingTevTextureObject;
    u8* memberData = NULL;
    u8* bakedSiblingPixels = NULL;
    u32 memberSize = 0;
    u32 bakedSiblingSize = 0;
    const u8* sceneData;
    unsigned char* baselinePixels = NULL;
    unsigned char* tevPixels = NULL;
    unsigned char baselineCenter[4] = { 0 };
    unsigned char tevCenter[4] = { 0 };
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 baseJointOffset = 0;
    u32 baseDObjOffset = 0;
    u32 basePObjOffset = 0;
    u32 baseMObjOffset = 0;
    u32 frontTObjOffset = 0;
    u32 frontTextureMapId = 0;
    u32 siblingTObjOffset = 0;
    u32 siblingTextureMapId = 0;
    unsigned int diffPixels = 0;
    unsigned int tevSubmitted = 0;
    unsigned int tevExpanded = 0;
    unsigned int tevPrimitive = 0;
    f32 baseModelViewMatrix[3][4];
    f32 frontModelViewMatrix[3][4];
    f32 siblingModelViewMatrix[3][4];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedBasePObj, 0, sizeof(translatedBasePObj));
    memset(&translatedBaseJoint, 0, sizeof(translatedBaseJoint));
    memset(&translatedBaseMaterial, 0, sizeof(translatedBaseMaterial));
    memset(&translatedFrontPObj, 0, sizeof(translatedFrontPObj));
    memset(&translatedFrontJoint, 0, sizeof(translatedFrontJoint));
    memset(&translatedFrontMaterial, 0, sizeof(translatedFrontMaterial));
    memset(&translatedFrontTexture, 0, sizeof(translatedFrontTexture));
    memset(&translatedSiblingPObj, 0, sizeof(translatedSiblingPObj));
    memset(&translatedSiblingJoint, 0, sizeof(translatedSiblingJoint));
    memset(&translatedSiblingMaterial, 0, sizeof(translatedSiblingMaterial));
    memset(&translatedSiblingTexture, 0, sizeof(translatedSiblingTexture));
    memset(&baseDrawObject, 0, sizeof(baseDrawObject));
    memset(&frontDrawObject, 0, sizeof(frontDrawObject));
    memset(&siblingDrawObject, 0, sizeof(siblingDrawObject));
    memset(&frontTextureObject, 0, sizeof(frontTextureObject));
    memset(&siblingRawTextureObject, 0, sizeof(siblingRawTextureObject));
    memset(&siblingTevTextureObject, 0, sizeof(siblingTevTextureObject));

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE,
                               PCPORT_REAL_CONTENT_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice load failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice archive parse failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real TEV scene camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);
    if (!ResolveFirstRenderablePObjDescFromJoint(&archive,
                                                 rootJointOffset,
                                                 &baseJointOffset,
                                                 &baseDObjOffset,
                                                 &basePObjOffset)) {
        fprintf(stderr,
                "[pcport_bootstrap] No baseline renderable PObjDesc was found for real TEV scene slice (rootJoint=0x%X)\n",
                rootJointOffset);
        goto cleanup;
    }

    baseMObjOffset = PCPort_ReadBigEndianU32(archive.storage + baseDObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           basePObjOffset,
                                           &translatedBasePObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              baseJointOffset,
                                              &translatedBaseJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               baseMObjOffset,
                                               &translatedBaseMaterial)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate baseline branch for real TEV scene slice (joint=0x%X dobj=0x%X mobj=0x%X pobj=0x%X)\n",
                baseJointOffset,
                baseDObjOffset,
                baseMObjOffset,
                basePObjOffset);
        goto cleanup;
    }

    frontTObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_TEXTURED_MOBJ_OFFSET + 0x08);
    frontTextureMapId = PCPort_ReadBigEndianU32(archive.storage + frontTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_TEXTURED_POBJ_OFFSET,
                                           &translatedFrontPObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_TEXTURED_JOINT_OFFSET,
                                              &translatedFrontJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_TEXTURED_MOBJ_OFFSET,
                                               &translatedFrontMaterial) ||
        !PCPort_TranslateTextureFromArchiveBE(&archive,
                                              frontTObjOffset,
                                              &translatedFrontTexture)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate front textured branch for real TEV scene slice (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_TEXTURED_JOINT_OFFSET,
                PCPORT_TEXTURED_DOBJ_OFFSET,
                PCPORT_TEXTURED_MOBJ_OFFSET,
                frontTObjOffset,
                PCPORT_TEXTURED_POBJ_OFFSET);
        goto cleanup;
    }

    siblingTObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_SIBLING_MOBJ_OFFSET + 0x08);
    siblingTextureMapId = PCPort_ReadBigEndianU32(archive.storage + siblingTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_SIBLING_POBJ_OFFSET,
                                           &translatedSiblingPObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_SIBLING_JOINT_OFFSET,
                                              &translatedSiblingJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_SIBLING_MOBJ_OFFSET,
                                               &translatedSiblingMaterial) ||
        !PCPort_TranslateTextureFromArchiveBE(&archive,
                                              siblingTObjOffset,
                                              &translatedSiblingTexture)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate sibling branch for real TEV scene slice (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_SIBLING_JOINT_OFFSET,
                PCPORT_SIBLING_DOBJ_OFFSET,
                PCPORT_SIBLING_MOBJ_OFFSET,
                siblingTObjOffset,
                PCPORT_SIBLING_POBJ_OFFSET);
        goto cleanup;
    }

    if (translatedSiblingTexture.tev.kind != PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice reached the visible sibling branch but found no supported narrow TEV payload (siblingTObj=0x%X tev=0x%X format=%u)\n",
                translatedSiblingTexture.tobjArchiveOffset,
                translatedSiblingTexture.tev.archiveOffset,
                translatedSiblingTexture.format);
        goto cleanup;
    }

    if (!PCPort_BakeTextureRGBAFromArchiveBE(&archive,
                                             &translatedSiblingTexture,
                                             &bakedSiblingPixels,
                                             &bakedSiblingSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to bake narrow real TEV texture payload (siblingTObj=0x%X tev=0x%X)\n",
                translatedSiblingTexture.tobjArchiveOffset,
                translatedSiblingTexture.tev.archiveOffset);
        goto cleanup;
    }

    GXInitTexObj(&frontTextureObject,
                 archive.storage + translatedFrontTexture.imageDataArchiveOffset,
                 translatedFrontTexture.width,
                 translatedFrontTexture.height,
                 (GXTexFmt)translatedFrontTexture.format,
                 (GXTexWrapMode)translatedFrontTexture.wrapS,
                 (GXTexWrapMode)translatedFrontTexture.wrapT,
                 translatedFrontTexture.mipmap);

    GXInitTexObj(&siblingRawTextureObject,
                 archive.storage + translatedSiblingTexture.imageDataArchiveOffset,
                 translatedSiblingTexture.width,
                 translatedSiblingTexture.height,
                 (GXTexFmt)translatedSiblingTexture.format,
                 (GXTexWrapMode)translatedSiblingTexture.wrapS,
                 (GXTexWrapMode)translatedSiblingTexture.wrapT,
                 translatedSiblingTexture.mipmap);

    GXHostInitTexObjRGBA8(&siblingTevTextureObject,
                          bakedSiblingPixels,
                          translatedSiblingTexture.width,
                          translatedSiblingTexture.height,
                          (GXTexWrapMode)translatedSiblingTexture.wrapS,
                          (GXTexWrapMode)translatedSiblingTexture.wrapT);

    baseDrawObject.displayList = translatedBasePObj.pobj.display;
    baseDrawObject.displayListSize = translatedBasePObj.pobj.n_display;
    baseDrawObject.pipelineId = PCPORT_REAL_MATERIAL_PIPELINE;
    baseDrawObject.totalVerts = translatedBasePObj.totalSubmittedVertices;
    baseDrawObject.totalPrims = translatedBasePObj.totalPrimitiveCommands;

    frontDrawObject.displayList = translatedFrontPObj.pobj.display;
    frontDrawObject.displayListSize = translatedFrontPObj.pobj.n_display;
    frontDrawObject.pipelineId = PCPORT_REAL_TEXTURED_PIPELINE;
    frontDrawObject.totalVerts = translatedFrontPObj.totalSubmittedVertices;
    frontDrawObject.totalPrims = translatedFrontPObj.totalPrimitiveCommands;

    siblingDrawObject.displayList = translatedSiblingPObj.pobj.display;
    siblingDrawObject.displayListSize = translatedSiblingPObj.pobj.n_display;
    siblingDrawObject.pipelineId = PCPORT_REAL_SIBLING_TEXTURED_PIPELINE;
    siblingDrawObject.totalVerts = translatedSiblingPObj.totalSubmittedVertices;
    siblingDrawObject.totalPrims = translatedSiblingPObj.totalPrimitiveCommands;

    ConfigureTranslatedMaterialPipeline(PCPORT_REAL_MATERIAL_PIPELINE,
                                        &translatedBaseMaterial);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedFrontMaterial,
                                        &translatedFrontTexture,
                                        &frontTextureObject,
                                        (unsigned char)frontTextureMapId);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE,
                                        &translatedSiblingMaterial,
                                        &translatedSiblingTexture,
                                        &siblingRawTextureObject,
                                        (unsigned char)siblingTextureMapId);

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedFrontJoint.modelMatrix,
                    frontModelViewMatrix);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedBaseJoint.modelMatrix,
                    baseModelViewMatrix);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedSiblingJoint.modelMatrix,
                    siblingModelViewMatrix);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(frontModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedFrontPObj.pobj);
    fn_800DAD10((void*)&frontDrawObject);
    glFlush();

    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    GXLoadPosMtxImm(siblingModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedSiblingPObj.pobj);
    fn_800DAD10((void*)&siblingDrawObject);
    glFlush();

    baselinePixels = ReadBackbufferImage();
    if (baselinePixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture baseline framebuffer for real TEV scene slice\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          baselineCenter);

    ConfigureTranslatedMaterialPipeline(PCPORT_REAL_MATERIAL_PIPELINE,
                                        &translatedBaseMaterial);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedFrontMaterial,
                                        &translatedFrontTexture,
                                        &frontTextureObject,
                                        (unsigned char)frontTextureMapId);
    ConfigureTranslatedMaterialPipeline(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE,
                                        &translatedSiblingMaterial);
    GSgfxHostSetPipelineTexture(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE,
                                &siblingTevTextureObject,
                                1,
                                translatedSiblingTexture.tevMode,
                                translatedSiblingTexture.hasCoordId
                                    ? translatedSiblingTexture.coordId
                                    : 0u,
                                (unsigned char)siblingTextureMapId);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(frontModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedFrontPObj.pobj);
    fn_800DAD10((void*)&frontDrawObject);
    glFlush();

    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    GXLoadPosMtxImm(siblingModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedSiblingPObj.pobj);
    fn_800DAD10((void*)&siblingDrawObject);
    glFlush();

    tevSubmitted = GXHostGetLastSubmittedVertexCount();
    tevExpanded = GXHostGetLastExpandedVertexCount();
    tevPrimitive = GXHostGetLastSubmittedPrimitive();

    tevPixels = ReadBackbufferImage();
    if (tevPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture TEV framebuffer for real TEV scene slice\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          tevCenter);
    diffPixels = CountFramebufferDiffPixels(baselinePixels, tevPixels);
    if (diffPixels == 0u &&
        baselineCenter[0] == tevCenter[0] &&
        baselineCenter[1] == tevCenter[1] &&
        baselineCenter[2] == tevCenter[2] &&
        baselineCenter[3] == tevCenter[3]) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice reached the sibling draw path but did not change the framebuffer (scene=0x%X camera=0x%X siblingJoint=0x%X siblingDObj=0x%X siblingMObj=0x%X siblingTObj=0x%X siblingTev=0x%X baselineCenter=%u,%u,%u,%u tevCenter=%u,%u,%u,%u light=%u,%u,%u,%u dark=%u,%u,%u,%u diffPixels=%u submitted=%u expanded=%u prim=0x%X)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                PCPORT_SIBLING_JOINT_OFFSET,
                PCPORT_SIBLING_DOBJ_OFFSET,
                PCPORT_SIBLING_MOBJ_OFFSET,
                translatedSiblingTexture.tobjArchiveOffset,
                translatedSiblingTexture.tev.archiveOffset,
                baselineCenter[0],
                baselineCenter[1],
                baselineCenter[2],
                baselineCenter[3],
                tevCenter[0],
                tevCenter[1],
                tevCenter[2],
                tevCenter[3],
                translatedSiblingTexture.tev.rampLight[0],
                translatedSiblingTexture.tev.rampLight[1],
                translatedSiblingTexture.tev.rampLight[2],
                translatedSiblingTexture.tev.rampLight[3],
                translatedSiblingTexture.tev.rampDark[0],
                translatedSiblingTexture.tev.rampDark[1],
                translatedSiblingTexture.tev.rampDark[2],
                translatedSiblingTexture.tev.rampDark[3],
                diffPixels,
                tevSubmitted,
                tevExpanded,
                tevPrimitive);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real TEV scene slice smoke passed (scene=0x%X camera=0x%X siblingJoint=0x%X siblingDObj=0x%X siblingMObj=0x%X siblingTObj=0x%X siblingTev=0x%X siblingImage=0x%X siblingData=0x%X diffPixels=%u baselineCenter=%u,%u,%u,%u tevCenter=%u,%u,%u,%u light=%u,%u,%u,%u dark=%u,%u,%u,%u bakedBytes=%u submitted=%u expanded=%u prim=0x%X)\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           PCPORT_SIBLING_JOINT_OFFSET,
           PCPORT_SIBLING_DOBJ_OFFSET,
           PCPORT_SIBLING_MOBJ_OFFSET,
           translatedSiblingTexture.tobjArchiveOffset,
           translatedSiblingTexture.tev.archiveOffset,
           translatedSiblingTexture.imageArchiveOffset,
           translatedSiblingTexture.imageDataArchiveOffset,
           diffPixels,
           baselineCenter[0],
           baselineCenter[1],
           baselineCenter[2],
           baselineCenter[3],
           tevCenter[0],
           tevCenter[1],
           tevCenter[2],
           tevCenter[3],
           translatedSiblingTexture.tev.rampLight[0],
           translatedSiblingTexture.tev.rampLight[1],
           translatedSiblingTexture.tev.rampLight[2],
           translatedSiblingTexture.tev.rampLight[3],
           translatedSiblingTexture.tev.rampDark[0],
           translatedSiblingTexture.tev.rampDark[1],
           translatedSiblingTexture.tev.rampDark[2],
           translatedSiblingTexture.tev.rampDark[3],
           bakedSiblingSize,
           tevSubmitted,
           tevExpanded,
           tevPrimitive);
    ok = 1;

cleanup:
    GSgfxHostClearPipelineState(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_MATERIAL_PIPELINE);
    GXHostClearTextureBinding();
    GXHostSetVertexAlphaScale(1.0f);
    free(tevPixels);
    free(baselinePixels);
    PCPort_FreeBuffer(bakedSiblingPixels);
    PCPort_DestroyTranslatedPObj(&translatedSiblingPObj);
    PCPort_DestroyTranslatedPObj(&translatedFrontPObj);
    PCPort_DestroyTranslatedPObj(&translatedBasePObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int RunRealTevSceneSlice2Smoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedBasePObj;
    PCPortTranslatedJointTransform translatedBaseJoint;
    PCPortTranslatedMaterial translatedBaseMaterial;
    PCPortTranslatedTexture translatedBaseTexture;
    PCPortTranslatedPObj translatedFrontPObj;
    PCPortTranslatedJointTransform translatedFrontJoint;
    PCPortTranslatedMaterial translatedFrontMaterial;
    PCPortTranslatedTexture translatedFrontTexture;
    PCPortTranslatedPObj translatedSiblingPObj;
    PCPortTranslatedJointTransform translatedSiblingJoint;
    PCPortTranslatedMaterial translatedSiblingMaterial;
    PCPortTranslatedTexture translatedSiblingTexture;
    PCPortGSDrawObject baseDrawObject;
    PCPortGSDrawObject frontDrawObject;
    PCPortGSDrawObject siblingDrawObject;
    GXTexObj frontTextureObject;
    GXTexObj baseTevTextureObject;
    GXTexObj siblingTevTextureObject;
    u8* memberData = NULL;
    u8* bakedBasePixels = NULL;
    u8* bakedSiblingPixels = NULL;
    u32 memberSize = 0;
    u32 bakedBaseSize = 0;
    u32 bakedSiblingSize = 0;
    const u8* sceneData;
    unsigned char* baselinePixels = NULL;
    unsigned char* broaderPixels = NULL;
    unsigned char baselineCenter[4] = { 0 };
    unsigned char broaderCenter[4] = { 0 };
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 baseJointOffset = 0;
    u32 baseDObjOffset = 0;
    u32 basePObjOffset = 0;
    u32 baseMObjOffset = 0;
    u32 baseTObjOffset = 0;
    u32 baseTextureMapId = 0;
    u32 frontTObjOffset = 0;
    u32 frontTextureMapId = 0;
    u32 siblingTObjOffset = 0;
    u32 siblingTextureMapId = 0;
    unsigned int diffPixels = 0;
    unsigned int broaderSubmitted = 0;
    unsigned int broaderExpanded = 0;
    unsigned int broaderPrimitive = 0;
    f32 baseModelViewMatrix[3][4];
    f32 frontModelViewMatrix[3][4];
    f32 siblingModelViewMatrix[3][4];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedBasePObj, 0, sizeof(translatedBasePObj));
    memset(&translatedBaseJoint, 0, sizeof(translatedBaseJoint));
    memset(&translatedBaseMaterial, 0, sizeof(translatedBaseMaterial));
    memset(&translatedBaseTexture, 0, sizeof(translatedBaseTexture));
    memset(&translatedFrontPObj, 0, sizeof(translatedFrontPObj));
    memset(&translatedFrontJoint, 0, sizeof(translatedFrontJoint));
    memset(&translatedFrontMaterial, 0, sizeof(translatedFrontMaterial));
    memset(&translatedFrontTexture, 0, sizeof(translatedFrontTexture));
    memset(&translatedSiblingPObj, 0, sizeof(translatedSiblingPObj));
    memset(&translatedSiblingJoint, 0, sizeof(translatedSiblingJoint));
    memset(&translatedSiblingMaterial, 0, sizeof(translatedSiblingMaterial));
    memset(&translatedSiblingTexture, 0, sizeof(translatedSiblingTexture));
    memset(&baseDrawObject, 0, sizeof(baseDrawObject));
    memset(&frontDrawObject, 0, sizeof(frontDrawObject));
    memset(&siblingDrawObject, 0, sizeof(siblingDrawObject));
    memset(&frontTextureObject, 0, sizeof(frontTextureObject));
    memset(&baseTevTextureObject, 0, sizeof(baseTevTextureObject));
    memset(&siblingTevTextureObject, 0, sizeof(siblingTevTextureObject));

    if (!PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE,
                               PCPORT_REAL_CONTENT_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 2 load failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 2 archive parse failed (%s:%s)\n",
                PCPORT_REAL_CONTENT_ARCHIVE,
                PCPORT_REAL_CONTENT_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 2 failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 2 branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real TEV scene 2 camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);
    if (!ResolveFirstRenderablePObjDescFromJoint(&archive,
                                                 rootJointOffset,
                                                 &baseJointOffset,
                                                 &baseDObjOffset,
                                                 &basePObjOffset)) {
        fprintf(stderr,
                "[pcport_bootstrap] No baseline renderable PObjDesc was found for real TEV scene slice 2 (rootJoint=0x%X)\n",
                rootJointOffset);
        goto cleanup;
    }

    baseMObjOffset = PCPort_ReadBigEndianU32(archive.storage + baseDObjOffset + 0x08);
    baseTObjOffset = PCPort_ReadBigEndianU32(archive.storage + baseMObjOffset + 0x08);
    baseTextureMapId = PCPort_ReadBigEndianU32(archive.storage + baseTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           basePObjOffset,
                                           &translatedBasePObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              baseJointOffset,
                                              &translatedBaseJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               baseMObjOffset,
                                               &translatedBaseMaterial) ||
        !PCPort_TranslateTextureFromArchiveBE(&archive,
                                              baseTObjOffset,
                                              &translatedBaseTexture)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate base branch for real TEV scene slice 2 (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                baseJointOffset,
                baseDObjOffset,
                baseMObjOffset,
                baseTObjOffset,
                basePObjOffset);
        goto cleanup;
    }

    frontTObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_TEXTURED_MOBJ_OFFSET + 0x08);
    frontTextureMapId = PCPort_ReadBigEndianU32(archive.storage + frontTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_TEXTURED_POBJ_OFFSET,
                                           &translatedFrontPObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_TEXTURED_JOINT_OFFSET,
                                              &translatedFrontJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_TEXTURED_MOBJ_OFFSET,
                                               &translatedFrontMaterial) ||
        !PCPort_TranslateTextureFromArchiveBE(&archive,
                                              frontTObjOffset,
                                              &translatedFrontTexture)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate front textured branch for real TEV scene slice 2 (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_TEXTURED_JOINT_OFFSET,
                PCPORT_TEXTURED_DOBJ_OFFSET,
                PCPORT_TEXTURED_MOBJ_OFFSET,
                frontTObjOffset,
                PCPORT_TEXTURED_POBJ_OFFSET);
        goto cleanup;
    }

    siblingTObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_SIBLING_MOBJ_OFFSET + 0x08);
    siblingTextureMapId = PCPort_ReadBigEndianU32(archive.storage + siblingTObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_SIBLING_POBJ_OFFSET,
                                           &translatedSiblingPObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_SIBLING_JOINT_OFFSET,
                                              &translatedSiblingJoint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_SIBLING_MOBJ_OFFSET,
                                               &translatedSiblingMaterial) ||
        !PCPort_TranslateTextureFromArchiveBE(&archive,
                                              siblingTObjOffset,
                                              &translatedSiblingTexture)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate sibling branch for real TEV scene slice 2 (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_SIBLING_JOINT_OFFSET,
                PCPORT_SIBLING_DOBJ_OFFSET,
                PCPORT_SIBLING_MOBJ_OFFSET,
                siblingTObjOffset,
                PCPORT_SIBLING_POBJ_OFFSET);
        goto cleanup;
    }

    if (translatedBaseTexture.tev.kind != PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP ||
        translatedSiblingTexture.tev.kind != PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 2 found no shared translated TEV payload boundary (baseTev=0x%X baseKind=%u siblingTev=0x%X siblingKind=%u)\n",
                translatedBaseTexture.tev.archiveOffset,
                translatedBaseTexture.tev.kind,
                translatedSiblingTexture.tev.archiveOffset,
                translatedSiblingTexture.tev.kind);
        goto cleanup;
    }

    if (!PCPort_BakeTextureRGBAFromArchiveBE(&archive,
                                             &translatedBaseTexture,
                                             &bakedBasePixels,
                                             &bakedBaseSize) ||
        !PCPort_BakeTextureRGBAFromArchiveBE(&archive,
                                             &translatedSiblingTexture,
                                             &bakedSiblingPixels,
                                             &bakedSiblingSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to bake shared translated TEV payloads (baseTev=0x%X siblingTev=0x%X)\n",
                translatedBaseTexture.tev.archiveOffset,
                translatedSiblingTexture.tev.archiveOffset);
        goto cleanup;
    }

    GXInitTexObj(&frontTextureObject,
                 archive.storage + translatedFrontTexture.imageDataArchiveOffset,
                 translatedFrontTexture.width,
                 translatedFrontTexture.height,
                 (GXTexFmt)translatedFrontTexture.format,
                 (GXTexWrapMode)translatedFrontTexture.wrapS,
                 (GXTexWrapMode)translatedFrontTexture.wrapT,
                 translatedFrontTexture.mipmap);

    GXHostInitTexObjRGBA8(&baseTevTextureObject,
                          bakedBasePixels,
                          translatedBaseTexture.width,
                          translatedBaseTexture.height,
                          (GXTexWrapMode)translatedBaseTexture.wrapS,
                          (GXTexWrapMode)translatedBaseTexture.wrapT);

    GXHostInitTexObjRGBA8(&siblingTevTextureObject,
                          bakedSiblingPixels,
                          translatedSiblingTexture.width,
                          translatedSiblingTexture.height,
                          (GXTexWrapMode)translatedSiblingTexture.wrapS,
                          (GXTexWrapMode)translatedSiblingTexture.wrapT);

    baseDrawObject.displayList = translatedBasePObj.pobj.display;
    baseDrawObject.displayListSize = translatedBasePObj.pobj.n_display;
    baseDrawObject.pipelineId = PCPORT_REAL_MATERIAL_PIPELINE;
    baseDrawObject.totalVerts = translatedBasePObj.totalSubmittedVertices;
    baseDrawObject.totalPrims = translatedBasePObj.totalPrimitiveCommands;

    frontDrawObject.displayList = translatedFrontPObj.pobj.display;
    frontDrawObject.displayListSize = translatedFrontPObj.pobj.n_display;
    frontDrawObject.pipelineId = PCPORT_REAL_TEXTURED_PIPELINE;
    frontDrawObject.totalVerts = translatedFrontPObj.totalSubmittedVertices;
    frontDrawObject.totalPrims = translatedFrontPObj.totalPrimitiveCommands;

    siblingDrawObject.displayList = translatedSiblingPObj.pobj.display;
    siblingDrawObject.displayListSize = translatedSiblingPObj.pobj.n_display;
    siblingDrawObject.pipelineId = PCPORT_REAL_SIBLING_TEXTURED_PIPELINE;
    siblingDrawObject.totalVerts = translatedSiblingPObj.totalSubmittedVertices;
    siblingDrawObject.totalPrims = translatedSiblingPObj.totalPrimitiveCommands;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedFrontJoint.modelMatrix,
                    frontModelViewMatrix);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedBaseJoint.modelMatrix,
                    baseModelViewMatrix);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedSiblingJoint.modelMatrix,
                    siblingModelViewMatrix);

    ConfigureTranslatedMaterialPipeline(PCPORT_REAL_MATERIAL_PIPELINE,
                                        &translatedBaseMaterial);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedFrontMaterial,
                                        &translatedFrontTexture,
                                        &frontTextureObject,
                                        (unsigned char)frontTextureMapId);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE,
                                        &translatedSiblingMaterial,
                                        &translatedSiblingTexture,
                                        &siblingTevTextureObject,
                                        (unsigned char)siblingTextureMapId);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(frontModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedFrontPObj.pobj);
    fn_800DAD10((void*)&frontDrawObject);
    glFlush();

    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    GXLoadPosMtxImm(siblingModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedSiblingPObj.pobj);
    fn_800DAD10((void*)&siblingDrawObject);
    glFlush();

    baselinePixels = ReadBackbufferImage();
    if (baselinePixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture baseline framebuffer for real TEV scene slice 2\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          baselineCenter);

    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_MATERIAL_PIPELINE,
                                        &translatedBaseMaterial,
                                        &translatedBaseTexture,
                                        &baseTevTextureObject,
                                        (unsigned char)baseTextureMapId);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedFrontMaterial,
                                        &translatedFrontTexture,
                                        &frontTextureObject,
                                        (unsigned char)frontTextureMapId);
    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE,
                                        &translatedSiblingMaterial,
                                        &translatedSiblingTexture,
                                        &siblingTevTextureObject,
                                        (unsigned char)siblingTextureMapId);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(frontModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedFrontPObj.pobj);
    fn_800DAD10((void*)&frontDrawObject);
    glFlush();

    GXLoadPosMtxImm(baseModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedBasePObj.pobj);
    fn_800DAD10((void*)&baseDrawObject);
    glFlush();

    GXLoadPosMtxImm(siblingModelViewMatrix, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedSiblingPObj.pobj);
    fn_800DAD10((void*)&siblingDrawObject);
    glFlush();

    broaderSubmitted = GXHostGetLastSubmittedVertexCount();
    broaderExpanded = GXHostGetLastExpandedVertexCount();
    broaderPrimitive = GXHostGetLastSubmittedPrimitive();

    broaderPixels = ReadBackbufferImage();
    if (broaderPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture broader TEV framebuffer for real TEV scene slice 2\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          broaderCenter);
    diffPixels = CountFramebufferDiffPixels(baselinePixels, broaderPixels);
    if (diffPixels == 0u &&
        baselineCenter[0] == broaderCenter[0] &&
        baselineCenter[1] == broaderCenter[1] &&
        baselineCenter[2] == broaderCenter[2] &&
        baselineCenter[3] == broaderCenter[3]) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 2 reached the shared base+sibling TEV draw path but did not change the framebuffer (scene=0x%X camera=0x%X baseTObj=0x%X baseTev=0x%X siblingTObj=0x%X siblingTev=0x%X baselineCenter=%u,%u,%u,%u broaderCenter=%u,%u,%u,%u baseLight=%u,%u,%u,%u baseDark=%u,%u,%u,%u diffPixels=%u submitted=%u expanded=%u prim=0x%X)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                translatedBaseTexture.tobjArchiveOffset,
                translatedBaseTexture.tev.archiveOffset,
                translatedSiblingTexture.tobjArchiveOffset,
                translatedSiblingTexture.tev.archiveOffset,
                baselineCenter[0],
                baselineCenter[1],
                baselineCenter[2],
                baselineCenter[3],
                broaderCenter[0],
                broaderCenter[1],
                broaderCenter[2],
                broaderCenter[3],
                translatedBaseTexture.tev.rampLight[0],
                translatedBaseTexture.tev.rampLight[1],
                translatedBaseTexture.tev.rampLight[2],
                translatedBaseTexture.tev.rampLight[3],
                translatedBaseTexture.tev.rampDark[0],
                translatedBaseTexture.tev.rampDark[1],
                translatedBaseTexture.tev.rampDark[2],
                translatedBaseTexture.tev.rampDark[3],
                diffPixels,
                broaderSubmitted,
                broaderExpanded,
                broaderPrimitive);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real TEV scene slice 2 smoke passed (scene=0x%X camera=0x%X baseJoint=0x%X baseDObj=0x%X baseMObj=0x%X baseTObj=0x%X baseTev=0x%X siblingJoint=0x%X siblingDObj=0x%X siblingMObj=0x%X siblingTObj=0x%X siblingTev=0x%X diffPixels=%u baselineCenter=%u,%u,%u,%u broaderCenter=%u,%u,%u,%u baseLight=%u,%u,%u,%u baseDark=%u,%u,%u,%u baseBakedBytes=%u siblingBakedBytes=%u submitted=%u expanded=%u prim=0x%X)\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           baseJointOffset,
           baseDObjOffset,
           baseMObjOffset,
           translatedBaseTexture.tobjArchiveOffset,
           translatedBaseTexture.tev.archiveOffset,
           PCPORT_SIBLING_JOINT_OFFSET,
           PCPORT_SIBLING_DOBJ_OFFSET,
           PCPORT_SIBLING_MOBJ_OFFSET,
           translatedSiblingTexture.tobjArchiveOffset,
           translatedSiblingTexture.tev.archiveOffset,
           diffPixels,
           baselineCenter[0],
           baselineCenter[1],
           baselineCenter[2],
           baselineCenter[3],
           broaderCenter[0],
           broaderCenter[1],
           broaderCenter[2],
           broaderCenter[3],
           translatedBaseTexture.tev.rampLight[0],
           translatedBaseTexture.tev.rampLight[1],
           translatedBaseTexture.tev.rampLight[2],
           translatedBaseTexture.tev.rampLight[3],
           translatedBaseTexture.tev.rampDark[0],
           translatedBaseTexture.tev.rampDark[1],
           translatedBaseTexture.tev.rampDark[2],
           translatedBaseTexture.tev.rampDark[3],
           bakedBaseSize,
           bakedSiblingSize,
           broaderSubmitted,
           broaderExpanded,
           broaderPrimitive);
    ok = 1;

cleanup:
    GSgfxHostClearPipelineState(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_MATERIAL_PIPELINE);
    GXHostClearTextureBinding();
    GXHostSetVertexAlphaScale(1.0f);
    free(broaderPixels);
    free(baselinePixels);
    PCPort_FreeBuffer(bakedSiblingPixels);
    PCPort_FreeBuffer(bakedBasePixels);
    PCPort_DestroyTranslatedPObj(&translatedSiblingPObj);
    PCPort_DestroyTranslatedPObj(&translatedFrontPObj);
    PCPort_DestroyTranslatedPObj(&translatedBasePObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int RunRealTevSceneSlice3Smoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedObject1PObj;
    PCPortTranslatedJointTransform translatedObject1Joint;
    PCPortTranslatedMaterial translatedObject1Material;
    PCPortTranslatedTextureExp translatedObject1TextureExp;
    PCPortGSDrawObject drawObject1;
    GXTexObj rawTextureObject1;
    GXTexObj tevTextureObject1;
    u8* memberData = NULL;
    u8* bakedPixels1 = NULL;
    u32 memberSize = 0;
    u32 bakedSize1 = 0;
    const u8* sceneData;
    unsigned char* baselinePixels = NULL;
    unsigned char* tevPixels = NULL;
    unsigned char baselineCenter[4] = { 0 };
    unsigned char tevCenter[4] = { 0 };
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 object1TObjOffset = 0;
    u32 object1TextureMapId = 0;
    unsigned int diffPixels = 0;
    unsigned int tevSubmitted = 0;
    unsigned int tevExpanded = 0;
    unsigned int tevPrimitive = 0;
    f32 modelViewMatrix1[3][4];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedObject1PObj, 0, sizeof(translatedObject1PObj));
    memset(&translatedObject1Joint, 0, sizeof(translatedObject1Joint));
    memset(&translatedObject1Material, 0, sizeof(translatedObject1Material));
    memset(&translatedObject1TextureExp, 0, sizeof(translatedObject1TextureExp));
    memset(&drawObject1, 0, sizeof(drawObject1));
    memset(&rawTextureObject1, 0, sizeof(rawTextureObject1));
    memset(&tevTextureObject1, 0, sizeof(tevTextureObject1));

    if (!PCPort_LoadFsysMember(PCPORT_PDA_MENU_ARCHIVE,
                               PCPORT_PDA2_BG_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 3 load failed (%s:%s)\n",
                PCPORT_PDA_MENU_ARCHIVE,
                PCPORT_PDA2_BG_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 3 archive parse failed (%s:%s)\n",
                PCPORT_PDA_MENU_ARCHIVE,
                PCPORT_PDA2_BG_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 3 failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 3 branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real TEV scene 3 camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);

    object1TObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_PDA2_BG_OBJECT1_MOBJ_OFFSET + 0x08);
    object1TextureMapId = PCPort_ReadBigEndianU32(archive.storage + object1TObjOffset + 0x08);
    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_PDA2_BG_OBJECT1_POBJ_OFFSET,
                                           &translatedObject1PObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_PDA2_BG_OBJECT1_JOINT_OFFSET,
                                              &translatedObject1Joint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_PDA2_BG_OBJECT1_MOBJ_OFFSET,
                                               &translatedObject1Material) ||
        !PCPort_TranslateTextureExpFromArchiveBE(&archive,
                                                 object1TObjOffset,
                                                 &translatedObject1TextureExp)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate pda2_bg object 1 (joint=0x%X dobj=0x%X mobj=0x%X tobj=0x%X pobj=0x%X)\n",
                PCPORT_PDA2_BG_OBJECT1_JOINT_OFFSET,
                PCPORT_PDA2_BG_OBJECT1_DOBJ_OFFSET,
                PCPORT_PDA2_BG_OBJECT1_MOBJ_OFFSET,
                object1TObjOffset,
                PCPORT_PDA2_BG_OBJECT1_POBJ_OFFSET);
        goto cleanup;
    }

    if (translatedObject1TextureExp.kind != PCPORT_TEXTURE_EXP_KIND_I8_RAMP ||
        translatedObject1TextureExp.stageCount != 1u ||
        translatedObject1TextureExp.stages[0].kind != PCPORT_TEXP_STAGE_I8_RAMP_SAMPLE) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 3 did not reach the expected shared TExp boundary (obj1TObj=0x%X kind=%u stage0=%u stages=%u obj1Tev=0x%X)\n",
                translatedObject1TextureExp.stages[0].texture.tobjArchiveOffset,
                translatedObject1TextureExp.kind,
                translatedObject1TextureExp.stages[0].kind,
                translatedObject1TextureExp.stageCount,
                translatedObject1TextureExp.stages[0].texture.tev.archiveOffset);
        goto cleanup;
    }

    if (!PCPort_BakeTextureExpRGBAFromArchiveBE(&archive,
                                                &translatedObject1TextureExp,
                                                &bakedPixels1,
                                                &bakedSize1)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to bake pda2_bg translated TEV payload (obj1Tev=0x%X)\n",
                translatedObject1TextureExp.stages[0].texture.tev.archiveOffset);
        goto cleanup;
    }

    GXInitTexObj(&rawTextureObject1,
                 archive.storage + translatedObject1TextureExp.stages[0].texture.imageDataArchiveOffset,
                 translatedObject1TextureExp.stages[0].texture.width,
                 translatedObject1TextureExp.stages[0].texture.height,
                 (GXTexFmt)translatedObject1TextureExp.stages[0].texture.format,
                 (GXTexWrapMode)translatedObject1TextureExp.stages[0].texture.wrapS,
                 (GXTexWrapMode)translatedObject1TextureExp.stages[0].texture.wrapT,
                 translatedObject1TextureExp.stages[0].texture.mipmap);

    GXHostInitTexObjRGBA8(&tevTextureObject1,
                          bakedPixels1,
                          translatedObject1TextureExp.stages[0].texture.width,
                          translatedObject1TextureExp.stages[0].texture.height,
                          (GXTexWrapMode)translatedObject1TextureExp.stages[0].texture.wrapS,
                          (GXTexWrapMode)translatedObject1TextureExp.stages[0].texture.wrapT);

    drawObject1.displayList = translatedObject1PObj.pobj.display;
    drawObject1.displayListSize = translatedObject1PObj.pobj.n_display;
    drawObject1.pipelineId = PCPORT_REAL_TEXTURED_PIPELINE;
    drawObject1.totalVerts = translatedObject1PObj.totalSubmittedVertices;
    drawObject1.totalPrims = translatedObject1PObj.totalPrimitiveCommands;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedObject1Joint.modelMatrix,
                    modelViewMatrix1);

    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedObject1Material,
                                        &translatedObject1TextureExp.stages[0].texture,
                                        &rawTextureObject1,
                                        (unsigned char)object1TextureMapId);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(modelViewMatrix1, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedObject1PObj.pobj);
    fn_800DAD10((void*)&drawObject1);
    glFlush();

    baselinePixels = ReadBackbufferImage();
    if (baselinePixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture baseline framebuffer for real TEV scene slice 3\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          baselineCenter);

    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedObject1Material,
                                        &translatedObject1TextureExp.stages[0].texture,
                                        &tevTextureObject1,
                                        (unsigned char)object1TextureMapId);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(modelViewMatrix1, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedObject1PObj.pobj);
    fn_800DAD10((void*)&drawObject1);
    glFlush();

    tevSubmitted = GXHostGetLastSubmittedVertexCount();
    tevExpanded = GXHostGetLastExpandedVertexCount();
    tevPrimitive = GXHostGetLastSubmittedPrimitive();

    tevPixels = ReadBackbufferImage();
    if (tevPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture TEV framebuffer for real TEV scene slice 3\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          tevCenter);
    diffPixels = CountFramebufferDiffPixels(baselinePixels, tevPixels);
    if (diffPixels == 0u &&
        baselineCenter[0] == tevCenter[0] &&
        baselineCenter[1] == tevCenter[1] &&
        baselineCenter[2] == tevCenter[2] &&
        baselineCenter[3] == tevCenter[3]) {
        fprintf(stderr,
                "[pcport_bootstrap] Real TEV scene slice 3 reached the distinct pda2_bg TEV draw path but did not change the framebuffer (scene=0x%X camera=0x%X obj1Tev=0x%X baselineCenter=%u,%u,%u,%u tevCenter=%u,%u,%u,%u obj1Light=%u,%u,%u,%u obj1Dark=%u,%u,%u,%u diffPixels=%u submitted=%u expanded=%u prim=0x%X)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                translatedObject1TextureExp.stages[0].texture.tev.archiveOffset,
                baselineCenter[0],
                baselineCenter[1],
                baselineCenter[2],
                baselineCenter[3],
                tevCenter[0],
                tevCenter[1],
                tevCenter[2],
                tevCenter[3],
                translatedObject1TextureExp.stages[0].texture.tev.rampLight[0],
                translatedObject1TextureExp.stages[0].texture.tev.rampLight[1],
                translatedObject1TextureExp.stages[0].texture.tev.rampLight[2],
                translatedObject1TextureExp.stages[0].texture.tev.rampLight[3],
                translatedObject1TextureExp.stages[0].texture.tev.rampDark[0],
                translatedObject1TextureExp.stages[0].texture.tev.rampDark[1],
                translatedObject1TextureExp.stages[0].texture.tev.rampDark[2],
                translatedObject1TextureExp.stages[0].texture.tev.rampDark[3],
                diffPixels,
                tevSubmitted,
                tevExpanded,
                tevPrimitive);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real TEV scene slice 3 smoke passed (scene=0x%X camera=0x%X obj1Joint=0x%X obj1DObj=0x%X obj1MObj=0x%X obj1TObj=0x%X kind=%u stage0=%u stages=%u obj1Tev=0x%X diffPixels=%u baselineCenter=%u,%u,%u,%u tevCenter=%u,%u,%u,%u obj1Light=%u,%u,%u,%u obj1Dark=%u,%u,%u,%u baked1=%u submitted=%u expanded=%u prim=0x%X)\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           PCPORT_PDA2_BG_OBJECT1_JOINT_OFFSET,
           PCPORT_PDA2_BG_OBJECT1_DOBJ_OFFSET,
           PCPORT_PDA2_BG_OBJECT1_MOBJ_OFFSET,
           translatedObject1TextureExp.stages[0].texture.tobjArchiveOffset,
           translatedObject1TextureExp.kind,
           translatedObject1TextureExp.stages[0].kind,
           translatedObject1TextureExp.stageCount,
           translatedObject1TextureExp.stages[0].texture.tev.archiveOffset,
           diffPixels,
           baselineCenter[0],
           baselineCenter[1],
           baselineCenter[2],
           baselineCenter[3],
           tevCenter[0],
           tevCenter[1],
           tevCenter[2],
           tevCenter[3],
           translatedObject1TextureExp.stages[0].texture.tev.rampLight[0],
           translatedObject1TextureExp.stages[0].texture.tev.rampLight[1],
           translatedObject1TextureExp.stages[0].texture.tev.rampLight[2],
           translatedObject1TextureExp.stages[0].texture.tev.rampLight[3],
           translatedObject1TextureExp.stages[0].texture.tev.rampDark[0],
           translatedObject1TextureExp.stages[0].texture.tev.rampDark[1],
           translatedObject1TextureExp.stages[0].texture.tev.rampDark[2],
           translatedObject1TextureExp.stages[0].texture.tev.rampDark[3],
           bakedSize1,
           tevSubmitted,
           tevExpanded,
           tevPrimitive);
    ok = 1;

cleanup:
    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
    GXHostClearTextureBinding();
    GXHostSetVertexAlphaScale(1.0f);
    free(tevPixels);
    free(baselinePixels);
    PCPort_FreeBuffer(bakedPixels1);
    PCPort_DestroyTranslatedPObj(&translatedObject1PObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int RunRealSceneSlice4Smoke(void) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    PCPortTranslatedPObj translatedObject0PObj;
    PCPortTranslatedJointTransform translatedObject0Joint;
    PCPortTranslatedMaterial translatedObject0Material;
    PCPortTranslatedTextureExp translatedObject0TextureExp;
    PCPortTranslatedPObj translatedObject1PObj;
    PCPortTranslatedJointTransform translatedObject1Joint;
    PCPortTranslatedMaterial translatedObject1Material;
    PCPortTranslatedTextureExp translatedObject1TextureExp;
    PCPortGSDrawObject drawObject0;
    PCPortGSDrawObject drawObject1;
    GXTexObj tevTextureObject0;
    GXTexObj tevTextureObject1;
    u8* memberData = NULL;
    u8* bakedPixels0 = NULL;
    u8* bakedPixels1 = NULL;
    u32 memberSize = 0;
    u32 bakedSize0 = 0;
    u32 bakedSize1 = 0;
    const u8* sceneData;
    unsigned char* baselinePixels = NULL;
    unsigned char* richerPixels = NULL;
    unsigned char baselineCenter[4] = { 0 };
    unsigned char richerCenter[4] = { 0 };
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    u32 object0TObjOffset = 0;
    u32 object1TObjOffset = 0;
    u32 object0TextureMapId = 0;
    u32 object1TextureMapId = 0;
    u32 object0NextTObjOffset = 0;
    u32 object0RawSrc0 = 0;
    u32 object0RawSrc1 = 0;
    unsigned char object0CoordId0 = 0;
    unsigned char object0CoordId1 = 0;
    unsigned int diffPixels = 0;
    unsigned int richerSubmitted = 0;
    unsigned int richerExpanded = 0;
    unsigned int richerPrimitive = 0;
    f32 modelViewMatrix0[3][4];
    f32 modelViewMatrix1[3][4];
    int ok = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&translatedObject0PObj, 0, sizeof(translatedObject0PObj));
    memset(&translatedObject0Joint, 0, sizeof(translatedObject0Joint));
    memset(&translatedObject0Material, 0, sizeof(translatedObject0Material));
    memset(&translatedObject0TextureExp, 0, sizeof(translatedObject0TextureExp));
    memset(&translatedObject1PObj, 0, sizeof(translatedObject1PObj));
    memset(&translatedObject1Joint, 0, sizeof(translatedObject1Joint));
    memset(&translatedObject1Material, 0, sizeof(translatedObject1Material));
    memset(&translatedObject1TextureExp, 0, sizeof(translatedObject1TextureExp));
    memset(&drawObject0, 0, sizeof(drawObject0));
    memset(&drawObject1, 0, sizeof(drawObject1));
    memset(&tevTextureObject0, 0, sizeof(tevTextureObject0));
    memset(&tevTextureObject1, 0, sizeof(tevTextureObject1));

    if (!PCPort_LoadFsysMember(PCPORT_PDA_MENU_ARCHIVE,
                               PCPORT_PDA2_BG_MEMBER,
                               &memberData,
                               &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 load failed (%s:%s)\n",
                PCPORT_PDA_MENU_ARCHIVE,
                PCPORT_PDA2_BG_MEMBER);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 archive parse failed (%s:%s)\n",
                PCPORT_PDA_MENU_ARCHIVE,
                PCPORT_PDA2_BG_MEMBER);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate real scene slice 4 camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);

    object0TObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_PDA2_BG_OBJECT0_MOBJ_OFFSET + 0x08);
    object1TObjOffset = PCPort_ReadBigEndianU32(archive.storage + PCPORT_PDA2_BG_OBJECT1_MOBJ_OFFSET + 0x08);
    object0NextTObjOffset = PCPort_ReadBigEndianU32(archive.storage + object0TObjOffset + 0x04);
    object0RawSrc0 = PCPort_ReadBigEndianU32(archive.storage + object0TObjOffset + 0x0C);
    object0RawSrc1 = PCPort_ReadBigEndianU32(archive.storage + object0NextTObjOffset + 0x0C);
    object0TextureMapId = PCPort_ReadBigEndianU32(archive.storage + object0TObjOffset + 0x08);
    object1TextureMapId = PCPort_ReadBigEndianU32(archive.storage + object1TObjOffset + 0x08);

    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_PDA2_BG_OBJECT0_POBJ_OFFSET,
                                           &translatedObject0PObj)) {
        fprintf(stderr,
                "[pcport_bootstrap] pda2_bg object 0 PObj translation failed after TEX1 bridge changes (pobj=0x%X)\n",
                PCPORT_PDA2_BG_OBJECT0_POBJ_OFFSET);
        goto cleanup;
    }

    if (!PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_PDA2_BG_OBJECT0_JOINT_OFFSET,
                                              &translatedObject0Joint)) {
        fprintf(stderr,
                "[pcport_bootstrap] pda2_bg object 0 joint translation failed (root=0x%X joint=0x%X)\n",
                rootJointOffset,
                PCPORT_PDA2_BG_OBJECT0_JOINT_OFFSET);
        goto cleanup;
    }

    if (!PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_PDA2_BG_OBJECT0_MOBJ_OFFSET,
                                               &translatedObject0Material)) {
        fprintf(stderr,
                "[pcport_bootstrap] pda2_bg object 0 material translation failed (mobj=0x%X)\n",
                PCPORT_PDA2_BG_OBJECT0_MOBJ_OFFSET);
        goto cleanup;
    }

    if (!PCPort_TranslateTextureExpFromArchiveBE(&archive,
                                                 object0TObjOffset,
                                                 &translatedObject0TextureExp)) {
        fprintf(stderr,
                "[pcport_bootstrap] pda2_bg object 0 texture translation failed (tobj=0x%X nextTObj=0x%X rawSrc0=%u rawSrc1=%u)\n",
                object0TObjOffset,
                object0NextTObjOffset,
                object0RawSrc0,
                object0RawSrc1);
        goto cleanup;
    }

    if (!PCPort_TranslatePObjFromArchiveBE(&archive,
                                           PCPORT_PDA2_BG_OBJECT1_POBJ_OFFSET,
                                           &translatedObject1PObj) ||
        !PCPort_TranslateJointChainToMatrixBE(&archive,
                                              rootJointOffset,
                                              PCPORT_PDA2_BG_OBJECT1_JOINT_OFFSET,
                                              &translatedObject1Joint) ||
        !PCPort_TranslateMaterialFromArchiveBE(&archive,
                                               PCPORT_PDA2_BG_OBJECT1_MOBJ_OFFSET,
                                               &translatedObject1Material) ||
        !PCPort_TranslateTextureExpFromArchiveBE(&archive,
                                                 object1TObjOffset,
                                                 &translatedObject1TextureExp)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to translate pda2_bg baseline object 1 for scene slice 4\n");
        goto cleanup;
    }

    object0CoordId0 = translatedObject0TextureExp.stages[0].coordId;
    object0CoordId1 = translatedObject0TextureExp.stages[1].coordId;
    if (translatedObject0TextureExp.kind != PCPORT_TEXTURE_EXP_KIND_I8_RAMP_MASK ||
        translatedObject0TextureExp.stageCount != 2u ||
        translatedObject0TextureExp.stages[0].kind != PCPORT_TEXP_STAGE_I8_RAMP_SAMPLE ||
        translatedObject0TextureExp.stages[1].kind != PCPORT_TEXP_STAGE_I8_MASK_MODULATE ||
        translatedObject1TextureExp.kind != PCPORT_TEXTURE_EXP_KIND_I8_RAMP ||
        translatedObject1TextureExp.stageCount != 1u ||
        translatedObject1TextureExp.stages[0].kind != PCPORT_TEXP_STAGE_I8_RAMP_SAMPLE ||
        object0CoordId0 != 0u || object0CoordId1 != 1u) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 did not reach the expected shared TExp boundary (kind0=%u stage00=%u stage01=%u stages0=%u rawSrc0=%u coord0=%u rawSrc1=%u coord1=%u kind1=%u stage10=%u stages1=%u)\n",
                translatedObject0TextureExp.kind,
                translatedObject0TextureExp.stages[0].kind,
                translatedObject0TextureExp.stages[1].kind,
                translatedObject0TextureExp.stageCount,
                object0RawSrc0,
                object0CoordId0,
                object0RawSrc1,
                object0CoordId1,
                translatedObject1TextureExp.kind,
                translatedObject1TextureExp.stages[0].kind,
                translatedObject1TextureExp.stageCount);
        goto cleanup;
    }

    if (PCPort_ReadBigEndianU32(archive.storage + object0NextTObjOffset + 0x04) != 0u ||
        (translatedObject0TextureExp.stages[0].texture.flags & 0x0Fu) != 0u ||
        (translatedObject0TextureExp.stages[1].texture.flags & 0x0Fu) != 0u ||
        (translatedObject0TextureExp.stages[1].texture.flags & 0x00F00000u) != 0x00300000u ||
        translatedObject0TextureExp.stages[1].texture.tev.rampLight[0] != 0u ||
        translatedObject0TextureExp.stages[1].texture.tev.rampLight[1] != 0u ||
        translatedObject0TextureExp.stages[1].texture.tev.rampLight[2] != 0u ||
        translatedObject0TextureExp.stages[1].texture.tev.rampDark[0] != 0u ||
        translatedObject0TextureExp.stages[1].texture.tev.rampDark[1] != 0u ||
        translatedObject0TextureExp.stages[1].texture.tev.rampDark[2] != 0u) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 did not match the expected narrow chained TObj shape (next1=0x%X flags0=0x%X flags1=0x%X light1=%u,%u,%u dark1=%u,%u,%u)\n",
                PCPort_ReadBigEndianU32(archive.storage + object0NextTObjOffset + 0x04),
                translatedObject0TextureExp.stages[0].texture.flags,
                translatedObject0TextureExp.stages[1].texture.flags,
                translatedObject0TextureExp.stages[1].texture.tev.rampLight[0],
                translatedObject0TextureExp.stages[1].texture.tev.rampLight[1],
                translatedObject0TextureExp.stages[1].texture.tev.rampLight[2],
                translatedObject0TextureExp.stages[1].texture.tev.rampDark[0],
                translatedObject0TextureExp.stages[1].texture.tev.rampDark[1],
                translatedObject0TextureExp.stages[1].texture.tev.rampDark[2]);
        goto cleanup;
    }

    if (translatedObject0TextureExp.stages[0].texture.tev.kind != PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP ||
        translatedObject1TextureExp.stages[0].texture.tev.kind != PCPORT_TRANSLATED_TEV_I8_COLOR_RAMP) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 did not reach the expected base TEV payload boundary (obj0Tev0=0x%X kind0=%u obj0Tev1=0x%X kind1=%u obj1Tev=0x%X kind2=%u)\n",
                translatedObject0TextureExp.stages[0].texture.tev.archiveOffset,
                translatedObject0TextureExp.stages[0].texture.tev.kind,
                translatedObject0TextureExp.stages[1].texture.tev.archiveOffset,
                translatedObject0TextureExp.stages[1].texture.tev.kind,
                translatedObject1TextureExp.stages[0].texture.tev.archiveOffset,
                translatedObject1TextureExp.stages[0].texture.tev.kind);
        goto cleanup;
    }

    if (!PCPort_BakeTextureExpRGBAFromArchiveBE(&archive,
                                                &translatedObject0TextureExp,
                                                &bakedPixels0,
                                                &bakedSize0) ||
        !PCPort_BakeTextureExpRGBAFromArchiveBE(&archive,
                                                &translatedObject1TextureExp,
                                                &bakedPixels1,
                                                &bakedSize1)) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to bake pda2_bg chained TObj payloads for scene slice 4 (obj0Tev0=0x%X obj0Tev1=0x%X obj1Tev=0x%X)\n",
                translatedObject0TextureExp.stages[0].texture.tev.archiveOffset,
                translatedObject0TextureExp.stages[1].texture.tev.archiveOffset,
                translatedObject1TextureExp.stages[0].texture.tev.archiveOffset);
        goto cleanup;
    }

    GXHostInitTexObjRGBA8(&tevTextureObject0,
                          bakedPixels0,
                          translatedObject0TextureExp.stages[0].texture.width,
                          translatedObject0TextureExp.stages[0].texture.height,
                          (GXTexWrapMode)translatedObject0TextureExp.stages[0].texture.wrapS,
                          (GXTexWrapMode)translatedObject0TextureExp.stages[0].texture.wrapT);
    GXHostInitTexObjRGBA8(&tevTextureObject1,
                          bakedPixels1,
                          translatedObject1TextureExp.stages[0].texture.width,
                          translatedObject1TextureExp.stages[0].texture.height,
                          (GXTexWrapMode)translatedObject1TextureExp.stages[0].texture.wrapS,
                          (GXTexWrapMode)translatedObject1TextureExp.stages[0].texture.wrapT);

    drawObject0.displayList = translatedObject0PObj.pobj.display;
    drawObject0.displayListSize = translatedObject0PObj.pobj.n_display;
    drawObject0.pipelineId = PCPORT_REAL_SIBLING_TEXTURED_PIPELINE;
    drawObject0.totalVerts = translatedObject0PObj.totalSubmittedVertices;
    drawObject0.totalPrims = translatedObject0PObj.totalPrimitiveCommands;

    drawObject1.displayList = translatedObject1PObj.pobj.display;
    drawObject1.displayListSize = translatedObject1PObj.pobj.n_display;
    drawObject1.pipelineId = PCPORT_REAL_TEXTURED_PIPELINE;
    drawObject1.totalVerts = translatedObject1PObj.totalSubmittedVertices;
    drawObject1.totalPrims = translatedObject1PObj.totalPrimitiveCommands;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    VIWaitForRetrace_PC();

    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedObject0Joint.modelMatrix,
                    modelViewMatrix0);
    ConcatAffineMtx(translatedCamera.viewMatrix,
                    translatedObject1Joint.modelMatrix,
                    modelViewMatrix1);

    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_TEXTURED_PIPELINE,
                                        &translatedObject1Material,
                                        &translatedObject1TextureExp.stages[0].texture,
                                        &tevTextureObject1,
                                        (unsigned char)object1TextureMapId);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);
    GXLoadPosMtxImm(modelViewMatrix1, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedObject1PObj.pobj);
    fn_800DAD10((void*)&drawObject1);
    glFlush();

    baselinePixels = ReadBackbufferImage();
    if (baselinePixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture baseline framebuffer for real scene slice 4\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          baselineCenter);

    ConfigureTranslatedTexturedPipeline(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE,
                                        &translatedObject0Material,
                                        &translatedObject0TextureExp.stages[0].texture,
                                        &tevTextureObject0,
                                        (unsigned char)object0TextureMapId);

    ClearBackbuffer(0.0f, 0.0f, 0.0f);
    GSgfx_BeginFrame();
    GXSetViewport((f32)translatedCamera.viewportLeft,
                  (f32)translatedCamera.viewportTop,
                  (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                  (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                  0.0f,
                  1.0f);
    GXSetScissor((u32)translatedCamera.scissorLeft,
                 (u32)translatedCamera.scissorTop,
                 (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                 (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
    GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);

    GXLoadPosMtxImm(modelViewMatrix1, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedObject1PObj.pobj);
    fn_800DAD10((void*)&drawObject1);

    GXLoadPosMtxImm(modelViewMatrix0, 0);
    GXSetCurrentMtx(0);
    fn_801AA568(&translatedObject0PObj.pobj);
    fn_800DAD10((void*)&drawObject0);
    glFlush();

    richerSubmitted = GXHostGetLastSubmittedVertexCount();
    richerExpanded = GXHostGetLastExpandedVertexCount();
    richerPrimitive = GXHostGetLastSubmittedPrimitive();

    richerPixels = ReadBackbufferImage();
    if (richerPixels == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Failed to capture richer framebuffer for real scene slice 4\n");
        goto cleanup;
    }

    ReadBackbufferPixelAt(PCPORT_GX_SMOKE_SAMPLE_X,
                          PCPORT_GX_SMOKE_SAMPLE_Y,
                          richerCenter);
    diffPixels = CountFramebufferDiffPixels(baselinePixels, richerPixels);
    if (diffPixels == 0u &&
        baselineCenter[0] == richerCenter[0] &&
        baselineCenter[1] == richerCenter[1] &&
        baselineCenter[2] == richerCenter[2] &&
        baselineCenter[3] == richerCenter[3]) {
        fprintf(stderr,
                "[pcport_bootstrap] Real scene slice 4 admitted object 0 but did not change the baseline scene (scene=0x%X camera=0x%X obj0TexCoordSrc=%u obj0Tev=0x%X baselineCenter=%u,%u,%u,%u richerCenter=%u,%u,%u,%u diffPixels=%u submitted=%u expanded=%u prim=0x%X)\n",
                sceneOffset,
                translatedCamera.cameraArchiveOffset,
                object0RawSrc0,
                translatedObject0TextureExp.stages[0].texture.tev.archiveOffset,
                baselineCenter[0],
                baselineCenter[1],
                baselineCenter[2],
                baselineCenter[3],
                richerCenter[0],
                richerCenter[1],
                richerCenter[2],
                richerCenter[3],
                diffPixels,
                richerSubmitted,
                richerExpanded,
                richerPrimitive);
        goto cleanup;
    }

    printf("[pcport_bootstrap] Real scene slice 4 smoke passed (scene=0x%X camera=0x%X obj0Joint=0x%X obj0DObj=0x%X obj0MObj=0x%X obj0TObj0=0x%X obj0TObj1=0x%X kind0=%u stage00=%u stage01=%u stages0=%u kind1=%u stage10=%u stages1=%u obj0Tev0=0x%X obj0Tev1=0x%X rawSrc0=%u coord0=%u rawSrc1=%u coord1=%u obj1Tev=0x%X diffPixels=%u baselineCenter=%u,%u,%u,%u richerCenter=%u,%u,%u,%u obj0Light=%u,%u,%u,%u obj0Dark=%u,%u,%u,%u baked0=%u baked1=%u submitted=%u expanded=%u prim=0x%X)\n",
           sceneOffset,
           translatedCamera.cameraArchiveOffset,
           PCPORT_PDA2_BG_OBJECT0_JOINT_OFFSET,
           PCPORT_PDA2_BG_OBJECT0_DOBJ_OFFSET,
           PCPORT_PDA2_BG_OBJECT0_MOBJ_OFFSET,
           translatedObject0TextureExp.stages[0].texture.tobjArchiveOffset,
           translatedObject0TextureExp.stages[1].texture.tobjArchiveOffset,
           translatedObject0TextureExp.kind,
           translatedObject0TextureExp.stages[0].kind,
           translatedObject0TextureExp.stages[1].kind,
           translatedObject0TextureExp.stageCount,
           translatedObject1TextureExp.kind,
           translatedObject1TextureExp.stages[0].kind,
           translatedObject1TextureExp.stageCount,
           translatedObject0TextureExp.stages[0].texture.tev.archiveOffset,
           translatedObject0TextureExp.stages[1].texture.tev.archiveOffset,
           object0RawSrc0,
           object0CoordId0,
           object0RawSrc1,
           object0CoordId1,
           translatedObject1TextureExp.stages[0].texture.tev.archiveOffset,
           diffPixels,
           baselineCenter[0],
           baselineCenter[1],
           baselineCenter[2],
           baselineCenter[3],
           richerCenter[0],
           richerCenter[1],
           richerCenter[2],
           richerCenter[3],
           translatedObject0TextureExp.stages[0].texture.tev.rampLight[0],
           translatedObject0TextureExp.stages[0].texture.tev.rampLight[1],
           translatedObject0TextureExp.stages[0].texture.tev.rampLight[2],
           translatedObject0TextureExp.stages[0].texture.tev.rampLight[3],
           translatedObject0TextureExp.stages[0].texture.tev.rampDark[0],
           translatedObject0TextureExp.stages[0].texture.tev.rampDark[1],
           translatedObject0TextureExp.stages[0].texture.tev.rampDark[2],
           translatedObject0TextureExp.stages[0].texture.tev.rampDark[3],
           bakedSize0,
           bakedSize1,
           richerSubmitted,
           richerExpanded,
           richerPrimitive);
    ok = 1;

cleanup:
    GSgfxHostClearPipelineState(PCPORT_REAL_SIBLING_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
    GXHostClearTextureBinding();
    GXHostSetVertexAlphaScale(1.0f);
    free(richerPixels);
    free(baselinePixels);
    PCPort_FreeBuffer(bakedPixels0);
    PCPort_FreeBuffer(bakedPixels1);
    PCPort_DestroyTranslatedPObj(&translatedObject0PObj);
    PCPort_DestroyTranslatedPObj(&translatedObject1PObj);
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    return ok;
}

static int HasArg(int argc, char** argv, const char* arg) {
    int i;

    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], arg) == 0) {
            return 1;
        }
    }

    return 0;
}

static GLFWwindow* CreateSmokeWindow(void) {
    GLFWwindow* window;

    if (!glfwInit()) {
        fprintf(stderr, "[pcport_bootstrap] glfwInit failed\n");
        return NULL;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(PCPORT_WINDOW_WIDTH,
                              PCPORT_WINDOW_HEIGHT,
                              "Pokemon Colosseum PC Bootstrap",
                              NULL,
                              NULL);
    if (window == NULL) {
        fprintf(stderr, "[pcport_bootstrap] glfwCreateWindow failed\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

static void DestroySmokeWindow(GLFWwindow* window) {
    if (window != NULL) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}

static int RunWindowSmokeLoop(GLFWwindow* window) {
    int frame;
    int framesRan = 0;

    for (frame = 0; frame < PCPORT_WINDOW_FRAMES; ++frame) {
        int fbWidth;
        int fbHeight;
        float colorBias;

        if (glfwWindowShouldClose(window)) {
            break;
        }

        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        colorBias = (float)frame / (float)PCPORT_WINDOW_FRAMES;

        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(0.08f + (0.10f * colorBias),
                     0.12f + (0.06f * colorBias),
                     0.18f + (0.08f * colorBias),
                     1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
        VIWaitForRetrace_PC();
        ++framesRan;
    }

    printf("[pcport_bootstrap] Window smoke loop completed (%d frames)\n",
           framesRan);
    return framesRan > 0;
}

static int RunGSgfxSmoke(void) {
    unsigned int frameCount;
    unsigned int preRetraceCount;
    unsigned char drawDoneFlag;
    int i;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);
    frameCount = GSgfxGetFrameCount();
    if (frameCount != 0xFFFFFFFFu) {
        fprintf(stderr,
                "[pcport_bootstrap] GSgfxInit verification failed (frameCount=%u)\n",
                frameCount);
        return 0;
    }

    for (i = 0; i < PCPORT_GSGFX_SWAPS; ++i) {
        VIWaitForRetrace_PC();
        GSgfxSwapBuffers(1);
    }

    preRetraceCount = GSgfxHostGetPreRetraceCount();
    if (preRetraceCount != PCPORT_GSGFX_SWAPS) {
        fprintf(stderr,
                "[pcport_bootstrap] GSgfx pre-retrace verification failed (count=%u)\n",
                preRetraceCount);
        return 0;
    }

    drawDoneFlag = GSgfxHostGetDrawDoneFlag();
    if (drawDoneFlag == 0) {
        fprintf(stderr,
                "[pcport_bootstrap] GSgfx draw-done verification failed\n");
        return 0;
    }

    printf("[pcport_bootstrap] GSgfx smoke path completed (%d swaps, %u pre-retrace callbacks)\n",
           PCPORT_GSGFX_SWAPS,
           preRetraceCount);
    return 1;
}

/* Counters threaded through the recursive menu-scene draw walk so the caller
 * can report coverage (PCPORT-only diagnostic state). */
typedef struct {
    unsigned int joints;
    unsigned int dobjs;
    unsigned int drawn;
    unsigned int skipped;
    unsigned int textured;
    unsigned int materialOnly;
} MenuTreeStats;

/*
 * Generalization of ResolveFirstRenderablePObjDescFromJoint: instead of
 * stopping at the first renderable PObjDesc, walk every joint (child at
 * joint+0x08, sibling at joint+0x0C), every DObj on each joint (list head at
 * joint+0x10, next at dobj+0x00), and draw each DObj's PObj (dobj+0x0C) with
 * the translate+draw template proven in RunRealSceneSlice2/3/4Smoke.
 * Every archive read is range-guarded so a malformed/missing node is skipped,
 * never dereferenced. Each PObj is drawn TEXTURED when its material (MObj at
 * dobj+0x08) carries a texture (TObj at mobj+0x08) that the host translators
 * understand: the TObj chain is translated (PCPort_TranslateTextureExpFromArchiveBE),
 * baked to RGBA (PCPort_BakeTextureExpRGBAFromArchiveBE) and uploaded
 * (GXHostInitTexObjRGBA8) then bound through ConfigureTranslatedTexturedPipeline --
 * the exact path proven by the slice-3/slice-4 textured smokes but with NO
 * byte-exact format asserts. On ANY failure (no TObj, unsupported format, range
 * fail, translate/bake fail) the node falls back to the material-only pipeline
 * so flat-shaded geometry still renders and nothing aborts. Per-node baked
 * buffers are freed and the texture binding is cleared after every draw.
 */
static void RenderJointTree(const PCPortHSDArchive* a,
                            u32 rootJoint,
                            u32 joint,
                            const PCPortTranslatedCamera* cam,
                            int pipelineId,
                            MenuTreeStats* stats) {
    u32 dobjOffset;
    u32 childOffset;
    u32 nextOffset;

    if (!ArchiveRangeValid(a, joint, PCPORT_SERIALIZED_JOINT_SIZE)) {
        return;
    }
    stats->joints++;

    dobjOffset = PCPort_ReadBigEndianU32(a->storage + joint + 0x10);
    while (dobjOffset != 0u &&
           ArchiveRangeValid(a, dobjOffset, PCPORT_SERIALIZED_DOBJ_SIZE)) {
        u32 pobjOffset = PCPort_ReadBigEndianU32(a->storage + dobjOffset + 0x0C);
        u32 mobjOffset = PCPort_ReadBigEndianU32(a->storage + dobjOffset + 0x08);

        stats->dobjs++;

        if (ArchiveRangeValid(a, pobjOffset, PCPORT_SERIALIZED_POBJ_SIZE)) {
            PCPortTranslatedPObj translatedPObj;
            PCPortTranslatedJointTransform translatedJoint;
            PCPortTranslatedMaterial translatedMaterial;
            PCPortTranslatedTextureExp translatedTextureExp;
            PCPortGSDrawObject drawObject;
            GXTexObj nodeTextureObject;
            f32 modelViewMatrix[3][4];
            u8* bakedPixels = NULL;
            u32 bakedSize = 0u;
            u32 tobjOffset = 0u;
            u32 textureMapId = 0u;
            int haveMaterial = 0;
            int haveTexture = 0;
            int isLogoTex = 0;

            memset(&translatedPObj, 0, sizeof(translatedPObj));
            memset(&translatedJoint, 0, sizeof(translatedJoint));
            memset(&translatedMaterial, 0, sizeof(translatedMaterial));
            memset(&translatedTextureExp, 0, sizeof(translatedTextureExp));
            memset(&drawObject, 0, sizeof(drawObject));
            memset(&nodeTextureObject, 0, sizeof(nodeTextureObject));

            if (PCPort_TranslatePObjFromArchiveBE(a, pobjOffset, &translatedPObj) &&
                PCPort_TranslateJointChainToMatrixBE(a, rootJoint, joint,
                                                     &translatedJoint)) {
                if (ArchiveRangeValid(a, mobjOffset, 0x4u)) {
                    haveMaterial = PCPort_TranslateMaterialFromArchiveBE(
                        a, mobjOffset, &translatedMaterial);
                }

                /* Textured path: the MObj->TObj link lives at mobj+0x08 (same
                 * read the slice-3/slice-4 smokes use) and the texture-map id at
                 * tobj+0x08. Translate the TObj chain (PCPort_TranslateTextureExpFromArchiveBE)
                 * and bake it to a linear RGBA buffer (PCPort_BakeTextureExpRGBAFromArchiveBE,
                 * the slice-4 template) which the bake function fills for the
                 * I8-ramp / I8-ramp+mask families AND, for plain non-TEV nodes,
                 * by decoding the native GX format (CMPR/RGBA8/I8/...) through
                 * DecodeTextureToRGBA. The RGBA is uploaded with GXHostInitTexObjRGBA8
                 * and bound through ConfigureTranslatedTexturedPipeline -- exactly
                 * the path proven by RunRealSceneSlice4Smoke (129-colour textured
                 * bg) but with NO byte-exact texture-shape asserts. On ANY failure
                 * (no link, range fail, unsupported format, translate/bake fail,
                 * zero extent) haveTexture stays 0 and the node falls back to the
                 * material-only pipeline so flat-shaded geometry still renders.
                 * Never aborts. */
                if (haveMaterial && ArchiveRangeValid(a, mobjOffset, 0x0Cu)) {
                    tobjOffset =
                        PCPort_ReadBigEndianU32(a->storage + mobjOffset + 0x08);
                    if (ArchiveRangeValid(a, tobjOffset, 0x0Cu) &&
                        PCPort_TranslateTextureExpFromArchiveBE(
                            a, tobjOffset, &translatedTextureExp) &&
                        translatedTextureExp.stageCount != 0u &&
                        translatedTextureExp.stages[0].texture.width != 0u &&
                        translatedTextureExp.stages[0].texture.height != 0u &&
                        PCPort_BakeTextureExpRGBAFromArchiveBE(
                            a, &translatedTextureExp, &bakedPixels, &bakedSize) &&
                        bakedPixels != NULL) {
                        const PCPortTranslatedTexture* baseTexture =
                            &translatedTextureExp.stages[0].texture;

                        /* The crisp logo is drawn by the 2D overlay; skip the
                         * scene's own logo billboard AND its two glow billboards
                         * (810x336 @ 0x693E0/0x8AB60) so they don't ghost over
                         * the crisp 2D logo. */
                        isLogoTex =
                            (baseTexture->imageDataArchiveOffset == PCPORT_LOGO_IMAGE_OFFSET ||
                             baseTexture->imageDataArchiveOffset == 0x693E0u ||
                             baseTexture->imageDataArchiveOffset == 0x8AB60u);

                        textureMapId = PCPort_ReadBigEndianU32(
                            a->storage + tobjOffset + 0x08);

                        /* Modulate the baked texture by the material diffuse
                         * colour (texture x diffuse). The desert ground/ruins
                         * texture with a near-white haze/glow; their real stone/
                         * sand tone is the material diffuse, so without this they
                         * render washed-out at full brightness. White-diffuse
                         * materials are left unchanged. */
                        if (haveMaterial) {
                            u32 dr = (translatedMaterial.diffuse >> 24) & 0xFFu;
                            u32 dg = (translatedMaterial.diffuse >> 16) & 0xFFu;
                            u32 db = (translatedMaterial.diffuse >> 8) & 0xFFu;
                            if (dr != 0xFFu || dg != 0xFFu || db != 0xFFu) {
                                u32 px = (u32)baseTexture->width *
                                         (u32)baseTexture->height;
                                u32 i;
                                for (i = 0; i < px; ++i) {
                                    bakedPixels[(i * 4u) + 0u] = (u8)(
                                        (bakedPixels[(i * 4u) + 0u] * dr) / 255u);
                                    bakedPixels[(i * 4u) + 1u] = (u8)(
                                        (bakedPixels[(i * 4u) + 1u] * dg) / 255u);
                                    bakedPixels[(i * 4u) + 2u] = (u8)(
                                        (bakedPixels[(i * 4u) + 2u] * db) / 255u);
                                }
                            }
                        }

                        GXHostInitTexObjRGBA8(
                            &nodeTextureObject,
                            bakedPixels,
                            baseTexture->width,
                            baseTexture->height,
                            (GXTexWrapMode)baseTexture->wrapS,
                            (GXTexWrapMode)baseTexture->wrapT);
                        haveTexture = 1;
                    }
                }

                drawObject.displayList = translatedPObj.pobj.display;
                drawObject.displayListSize = translatedPObj.pobj.n_display;
                drawObject.pipelineId =
                    haveTexture ? PCPORT_REAL_TEXTURED_PIPELINE
                                : (unsigned int)pipelineId;
                drawObject.totalVerts = translatedPObj.totalSubmittedVertices;
                drawObject.totalPrims = translatedPObj.totalPrimitiveCommands;

                if (haveTexture) {
                    ConfigureTranslatedTexturedPipeline(
                        PCPORT_REAL_TEXTURED_PIPELINE,
                        &translatedMaterial,
                        &translatedTextureExp.stages[0].texture,
                        &nodeTextureObject,
                        (unsigned char)textureMapId);
                    stats->textured++;
                } else {
                    ConfigureTranslatedMaterialPipeline(
                        (unsigned int)pipelineId,
                        haveMaterial ? &translatedMaterial : NULL);
                    stats->materialOnly++;
                }
                GXHostSetVertexAlphaScale(1.0f);

                ConcatAffineMtx(cam->viewMatrix,
                                translatedJoint.modelMatrix,
                                modelViewMatrix);
                GXLoadPosMtxImm(modelViewMatrix, 0);
                GXSetCurrentMtx(0);
                /* Apply this PObj's vertex descriptor + arrays to the GX shim so
                 * the indexed display-list replay (fn_800BD0FC -> GXCallDisplayList)
                 * decodes real positions, colours AND texcoords for this node.
                 * Without this the shim's g_vtxDescState stays unset and every
                 * replayed vertex collapses to the origin with degenerate (0,0)
                 * texcoords -- which is why textured nodes previously rendered as
                 * a flat material colour. The translated arrays live on the
                 * PObj's verts list (LE, host-resident); the list is GX_VA_NULL
                 * terminated. */
                if (translatedPObj.pobj.verts != NULL) {
                    HSD_VtxDescList* vtx = translatedPObj.pobj.verts;

                    GXClearVtxDesc();
                    while (vtx->attr != GX_VA_NULL) {
                        GXSetVtxDesc((GXAttr)vtx->attr, (GXAttrType)vtx->attr_type);
                        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)vtx->attr,
                                        (GXCompCnt)vtx->comp_cnt,
                                        (GXCompType)vtx->comp_type, vtx->frac);
                        GXSetArray((GXAttr)vtx->attr, vtx->vertex, (u8)vtx->stride);
                        ++vtx;
                    }
                }
                /* Skip the demo's full-screen fade/flash overlay: a material-
                 * only quad whose material alpha is ~0 (fully transparent).
                 * In-game it is an animated alpha fade that ends transparent;
                 * drawn opaque it would cover the whole title scene. Keying on
                 * alpha (not camera-space position) is camera-independent. */
                if (!((haveTexture == 0 && haveMaterial &&
                       translatedMaterial.alpha < 0.01f) ||
                      isLogoTex)) {
                    fn_801AA568(&translatedPObj.pobj);
                    fn_800DAD10((void*)&drawObject);
                    stats->drawn++;
                } else {
                    stats->skipped++;
                }

                if (haveTexture) {
                    GXHostClearTextureBinding();
                    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
                }

                {
                    static int rjtDbg = -1;
                    if (rjtDbg < 0) {
                        rjtDbg = (getenv("PCPORT_RENDER_DEBUG") != NULL) ? 1 : 0;
                    }
                    if (rjtDbg && stats->dobjs <= 40u) {
                        printf("[rjt] pobj#%u %s verts=%u cam=(%.0f,%.0f,%.0f)\n",
                               stats->dobjs, haveTexture ? "TEX" : "MAT",
                               drawObject.totalVerts,
                               modelViewMatrix[0][3], modelViewMatrix[1][3],
                               modelViewMatrix[2][3]);
                    }
                }
            } else {
                stats->skipped++;
            }

            PCPort_FreeBuffer(bakedPixels);
            PCPort_DestroyTranslatedPObj(&translatedPObj);
        } else {
            stats->skipped++;
        }

        nextOffset = PCPort_ReadBigEndianU32(a->storage + dobjOffset + 0x00);
        if (nextOffset == dobjOffset) {
            break;
        }
        dobjOffset = nextOffset;
    }

    childOffset = PCPort_ReadBigEndianU32(a->storage + joint + 0x08);
    if (childOffset != 0u && childOffset != joint) {
        RenderJointTree(a, rootJoint, childOffset, cam, pipelineId, stats);
    }

    nextOffset = PCPort_ReadBigEndianU32(a->storage + joint + 0x0C);
    if (nextOffset != 0u && nextOffset != joint) {
        RenderJointTree(a, rootJoint, nextOffset, cam, pipelineId, stats);
    }
}

/* World->camera 3x4 view matrix (GameCube convention: camera looks down -Z)
 * from eye/interest/up, used to override a scene camera with a known pose. */
static void BuildViewMatrixLookAt(const f32 eye[3], const f32 interest[3],
                                  const f32 up[3], f32 outView[3][4]) {
    f32 zx = eye[0] - interest[0];
    f32 zy = eye[1] - interest[1];
    f32 zz = eye[2] - interest[2];
    f32 zl = (f32)sqrt((double)((zx * zx) + (zy * zy) + (zz * zz)));
    f32 xx, xy, xz, xl, yx, yy, yz;

    if (zl < 1e-6f) { zl = 1.0f; }
    zx /= zl; zy /= zl; zz /= zl;

    xx = (up[1] * zz) - (up[2] * zy);
    xy = (up[2] * zx) - (up[0] * zz);
    xz = (up[0] * zy) - (up[1] * zx);
    xl = (f32)sqrt((double)((xx * xx) + (xy * xy) + (xz * xz)));
    if (xl < 1e-6f) { xl = 1.0f; }
    xx /= xl; xy /= xl; xz /= xl;

    yx = (zy * xz) - (zz * xy);
    yy = (zz * xx) - (zx * xz);
    yz = (zx * xy) - (zy * xx);

    outView[0][0] = xx; outView[0][1] = xy; outView[0][2] = xz;
    outView[0][3] = -((xx * eye[0]) + (xy * eye[1]) + (xz * eye[2]));
    outView[1][0] = yx; outView[1][1] = yy; outView[1][2] = yz;
    outView[1][3] = -((yx * eye[0]) + (yy * eye[1]) + (yz * eye[2]));
    outView[2][0] = zx; outView[2][1] = zy; outView[2][2] = zz;
    outView[2][3] = -((zx * eye[0]) + (zy * eye[1]) + (zz * eye[2]));
}

/*
 * Route B boot path: load+parse the top-menu scene once, then present its full
 * joint tree every frame in a persistent (but headless-safe, capped) window
 * loop. Reuses the resolve/camera/draw primitives proven by the slice smokes.
 */
static int RunMenuScene(GLFWwindow* window) {
    PCPortHSDArchive archive;
    PCPortTranslatedCamera translatedCamera;
    u8* memberData = NULL;
    u32 memberSize = 0;
    const u8* sceneData;
    u32 sceneOffset = 0;
    u32 sceneBranchOffset;
    u32 cameraDescOffset;
    u32 sceneJointListOffset;
    u32 rootJointOffset;
    const char* capEnv;
    int frameCap;
    int frame;
    int dumpRequested;
    int ok = 0;
    const char* menuMember;
    const char* sceneArchive;
    PCPortHSDArchive logoArchive;
    u8* logoData = NULL;
    u32 logoSize = 0;
    u8* logoPixels = NULL;
    u32 logoPxSize = 0;
    GXTexObj logoTex;
    int haveLogo = 0;
    GXTexObj menu018Tex;
    int haveMenu018 = 0;
    GXTexObj skyTex;
    int haveSky = 0;
    int render3D = 0;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&logoArchive, 0, sizeof(logoArchive));
    memset(&logoTex, 0, sizeof(logoTex));
    memset(&menu018Tex, 0, sizeof(menu018Tex));
    memset(&skyTex, 0, sizeof(skyTex));

    /* Default to the title scene (desert/ruins environment + logo) in
     * title.fsys:logo_demo. Env overrides PCPORT_MENU_ARCHIVE / PCPORT_MENU_MEMBER
     * select any other fsys scene member (e.g. topmenu.fsys / menu_bg00 for the
     * post-start main-menu background). */
    sceneArchive = getenv("PCPORT_MENU_ARCHIVE");
    if (sceneArchive == NULL || sceneArchive[0] == '\0') {
        sceneArchive = PCPORT_TITLE_SCENE_ARCHIVE;
    }
    menuMember = getenv("PCPORT_MENU_MEMBER");
    if (menuMember == NULL || menuMember[0] == '\0') {
        menuMember = PCPORT_TITLE_SCENE_MEMBER;
    }

    if (!PCPort_LoadFsysMember(sceneArchive, menuMember,
                               &memberData, &memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Menu scene load failed (%s:%s)\n",
                sceneArchive, menuMember);
        goto cleanup;
    }

    if (!PCPort_HSDArchiveParseBE(&archive, memberData, memberSize)) {
        fprintf(stderr,
                "[pcport_bootstrap] Menu scene archive parse failed (%s:%s)\n",
                sceneArchive, menuMember);
        goto cleanup;
    }

    sceneData = (const u8*)PCPort_HSDArchiveGetPublicAddress(&archive,
                                                             "scene_data",
                                                             &sceneOffset);
    if (sceneData == NULL) {
        fprintf(stderr,
                "[pcport_bootstrap] Menu scene failed to resolve scene_data\n");
        goto cleanup;
    }

    sceneBranchOffset = PCPort_ReadBigEndianU32(sceneData + 0x00);
    if (!ArchiveRangeValid(&archive, sceneBranchOffset, 0x10u)) {
        fprintf(stderr,
                "[pcport_bootstrap] Menu scene branch offset was invalid (0x%X)\n",
                sceneBranchOffset);
        goto cleanup;
    }

    cameraDescOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x08);
    if (!PCPort_TranslatePerspectiveCameraFromArchiveBE(&archive,
                                                        cameraDescOffset,
                                                        &translatedCamera)) {
        fprintf(stderr,
                "[pcport_bootstrap] Menu scene failed to translate camera (scene=0x%X camera=0x%X)\n",
                sceneOffset,
                cameraDescOffset);
        goto cleanup;
    }

    /* Replace logo_demo's embedded flythrough-START camera (interest looks down
     * and close, which frames the scene zoomed-in) with the title END pose held
     * by cam_logo_demo_stop: same eye, but the interest looks ACROSS the desert
     * at eye level. These are the end-frame values of cam_logo_demo_stop's
     * (all-constant) camera animation. The archive's projection (fov 45, near/
     * far) is kept. PCPORT_NO_TITLE_CAM disables it (use the raw scene camera). */
    if (getenv("PCPORT_NO_TITLE_CAM") == NULL) {
        static const f32 titleEye[3] = { 0.0f, 38.905f, 409.812f };
        static const f32 titleInt[3] = { 0.0f, 39.6514f, 1.5625f };
        static const f32 titleUp[3]  = { 0.0f, 1.0f, 0.0f };

        BuildViewMatrixLookAt(titleEye, titleInt, titleUp,
                              translatedCamera.viewMatrix);
    }

    /* Experimental manual camera: PCPORT_CAM_EYE / PCPORT_CAM_INT ("x,y,z") +
     * optional PCPORT_CAM_UP override the view via look-at, for dialing in the
     * framing. */
    {
        const char* ce = getenv("PCPORT_CAM_EYE");
        const char* ci = getenv("PCPORT_CAM_INT");
        const char* cu = getenv("PCPORT_CAM_UP");
        if (ce != NULL && ci != NULL) {
            f32 e[3] = { 0.0f, 0.0f, 0.0f };
            f32 in[3] = { 0.0f, 0.0f, 0.0f };
            f32 u[3] = { 0.0f, 1.0f, 0.0f };

            sscanf(ce, "%f,%f,%f", &e[0], &e[1], &e[2]);
            sscanf(ci, "%f,%f,%f", &in[0], &in[1], &in[2]);
            if (cu != NULL) {
                sscanf(cu, "%f,%f,%f", &u[0], &u[1], &u[2]);
            }
            BuildViewMatrixLookAt(e, in, u, translatedCamera.viewMatrix);
            printf("[cam] override eye=(%.1f,%.1f,%.1f) int=(%.1f,%.1f,%.1f) up=(%.1f,%.1f,%.1f)\n",
                   e[0], e[1], e[2], in[0], in[1], in[2], u[0], u[1], u[2]);
        }
    }

    sceneJointListOffset = PCPort_ReadBigEndianU32(archive.storage + sceneBranchOffset + 0x00);
    rootJointOffset = PCPort_ReadBigEndianU32(archive.storage + sceneJointListOffset + 0x00);
    if (!ArchiveRangeValid(&archive, rootJointOffset, PCPORT_SERIALIZED_JOINT_SIZE)) {
        fprintf(stderr,
                "[pcport_bootstrap] Menu scene root joint was invalid (0x%X)\n",
                rootJointOffset);
        goto cleanup;
    }

    capEnv = getenv("PCPORT_MENU_FRAMES");
    frameCap = capEnv != NULL ? atoi(capEnv) : PCPORT_WINDOW_FRAMES;
    if (frameCap <= 0) {
        frameCap = PCPORT_WINDOW_FRAMES;
    }
    dumpRequested = getenv("PCPORT_DUMP") != NULL;
    /* Default to rendering the real 3D title scene (desert/ruins + the textured
     * geometry, texture x diffuse modulated, demo fade + logo billboards
     * skipped). Set PCPORT_NO_RENDER_3D=1 to fall back to the flat 2D sky
     * backdrop instead. */
    render3D = getenv("PCPORT_NO_RENDER_3D") == NULL;

    GSgfxInit(0x7DDD0, 0x10, 0x8, 0x20, 1, 0x1E0);

    printf("[pcport_bootstrap] Menu scene loaded (scene=0x%X camera=0x%X rootJoint=0x%X frameCap=%d dump=%d)\n",
           sceneOffset,
           cameraDescOffset,
           rootJointOffset,
           frameCap,
           dumpRequested);

    /* Load + bake + upload the static title logo once (GL context + GSgfxInit
     * are ready here). The decompressor fix in real_content_host.c is required
     * for logo_demo (a strongly-compressed v0102 member) to load at all. */
    if (PCPort_LoadFsysMember(PCPORT_LOGO_ARCHIVE, PCPORT_LOGO_MEMBER,
                              &logoData, &logoSize) &&
        PCPort_HSDArchiveParseBE(&logoArchive, logoData, logoSize)) {
        PCPortTranslatedTexture logoDesc;

        memset(&logoDesc, 0, sizeof(logoDesc));
        logoDesc.imageDataArchiveOffset = PCPORT_LOGO_IMAGE_OFFSET;
        logoDesc.format = GX_TF_RGBA8;
        logoDesc.width = PCPORT_LOGO_WIDTH;
        logoDesc.height = PCPORT_LOGO_HEIGHT;

        if (PCPort_BakeTextureRGBAFromArchiveBE(&logoArchive, &logoDesc,
                                                &logoPixels, &logoPxSize)) {
            GXHostInitTexObjRGBA8(&logoTex, logoPixels,
                                  PCPORT_LOGO_WIDTH, PCPORT_LOGO_HEIGHT,
                                  GX_CLAMP, GX_CLAMP);
            haveLogo = 1;
            printf("[pcport_bootstrap] Title logo loaded (%dx%d, %u bytes)\n",
                   PCPORT_LOGO_WIDTH, PCPORT_LOGO_HEIGHT, logoPxSize);
        }
    }
    if (!haveLogo) {
        fprintf(stderr,
                "[pcport_bootstrap] Title logo unavailable (continuing without it)\n");
    }

    /* Title-screen 2D overlay: copyright lines + PRESS START (raw RGB5A3 sprite). */
    haveMenu018 = LoadRawMenuTexObj(PCPORT_TITLE_PRESS_MEMBER, &menu018Tex);
    if (haveMenu018) {
        printf("[pcport_bootstrap] Title overlay (PRESS START + copyright) loaded\n");
    }

    /* Bake the desert sky/sand backdrop (CMPR) from the title scene archive. */
    if (!render3D) {
        PCPortTranslatedTexture skyDesc;
        u8* skyPixels = NULL;
        u32 skyPxSize = 0;

        memset(&skyDesc, 0, sizeof(skyDesc));
        skyDesc.imageDataArchiveOffset = PCPORT_TITLE_SKY_OFFSET;
        skyDesc.format = GX_TF_CMPR;
        skyDesc.width = PCPORT_TITLE_SKY_WIDTH;
        skyDesc.height = PCPORT_TITLE_SKY_HEIGHT;
        if (PCPort_BakeTextureRGBAFromArchiveBE(&archive, &skyDesc,
                                                &skyPixels, &skyPxSize)) {
            GXHostInitTexObjRGBA8(&skyTex, skyPixels,
                                  PCPORT_TITLE_SKY_WIDTH, PCPORT_TITLE_SKY_HEIGHT,
                                  GX_CLAMP, GX_CLAMP);
            PCPort_FreeBuffer(skyPixels);
            haveSky = 1;
            printf("[pcport_bootstrap] Title sky backdrop loaded (%dx%d)\n",
                   PCPORT_TITLE_SKY_WIDTH, PCPORT_TITLE_SKY_HEIGHT);
        }
    }

    for (frame = 0; frame < frameCap; ++frame) {
        MenuTreeStats stats;

        if (window != NULL && glfwWindowShouldClose(window)) {
            break;
        }

        memset(&stats, 0, sizeof(stats));

        VIWaitForRetrace_PC();
        ClearBackbuffer(0.0f, 0.0f, 0.0f);
        GSgfx_BeginFrame();

        if (render3D) {
            GXSetViewport((f32)translatedCamera.viewportLeft,
                          (f32)translatedCamera.viewportTop,
                          (f32)(translatedCamera.viewportRight - translatedCamera.viewportLeft),
                          (f32)(translatedCamera.viewportBottom - translatedCamera.viewportTop),
                          0.0f,
                          1.0f);
            GXSetScissor((u32)translatedCamera.scissorLeft,
                         (u32)translatedCamera.scissorTop,
                         (u32)(translatedCamera.scissorRight - translatedCamera.scissorLeft),
                         (u32)(translatedCamera.scissorBottom - translatedCamera.scissorTop));
            GXSetProjection(translatedCamera.projectionMatrix, GX_PERSPECTIVE);
            /* Depth-test the scene so the large ground plane does not paint over
             * the standing ruin pillars (which are drawn before it). */
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);

            RenderJointTree(&archive,
                            rootJointOffset,
                            rootJointOffset,
                            &translatedCamera,
                            (int)PCPORT_REAL_MATERIAL_PIPELINE,
                            &stats);
        }

        /* Title screen: 2D sky/sand backdrop, then logo + PRESS START +
         * copyright on top (the 3D scene is gated behind PCPORT_RENDER_3D). */
        if (haveSky || haveLogo || haveMenu018) {
            BeginMenuOverlay();
            if (haveSky) {
                DrawTexturedScreenRect(&skyTex, 0.0f, 0.0f, 640.0f, 480.0f,
                                       0.0f, 0.0f, 1.0f, 1.0f);
            }
            if (haveLogo) {
                /* logo, top-centre (540:224 aspect) */
                DrawTexturedScreenRect(&logoTex, 115.0f, 34.0f, 410.0f, 170.0f,
                                       0.0f, 0.0f, 1.0f, 1.0f);
            }
            if (haveMenu018) {
                /* PRESS START (teal band v0.574..0.721), centred below the logo */
                DrawTexturedScreenRect(&menu018Tex, 188.0f, 268.0f, 264.0f, 30.0f,
                                       0.0f, 0.574f, 1.0f, 0.721f);
                /* copyright block (v0.016..0.549), bottom-left */
                DrawTexturedScreenRect(&menu018Tex, 28.0f, 392.0f, 300.0f, 58.0f,
                                       0.0f, 0.016f, 1.0f, 0.549f);
            }
        }

        /* On the final frame, capture the framebuffer (BMP dump happens inside
         * ReadBackbufferImage via DumpFramebufferBMP when PCPORT_DUMP is set)
         * before presenting. */
        if (dumpRequested && frame == frameCap - 1) {
            unsigned char* pixels = ReadBackbufferImage();
            free(pixels);
        }

        GSgfxSwapBuffers(1);

        if (frame == 0) {
            printf("[pcport_bootstrap] Menu scene frame 0 walked (joints=%u dobjs=%u drawn=%u skipped=%u textured=%u materialOnly=%u)\n",
                   stats.joints,
                   stats.dobjs,
                   stats.drawn,
                   stats.skipped,
                   stats.textured,
                   stats.materialOnly);
        }
    }

    GSgfxHostClearPipelineState(PCPORT_REAL_TEXTURED_PIPELINE);
    GSgfxHostClearPipelineState(PCPORT_REAL_MATERIAL_PIPELINE);
    GXHostClearTextureBinding();
    GXHostSetVertexAlphaScale(1.0f);
    ok = 1;

cleanup:
    PCPort_HSDArchiveDestroy(&archive);
    PCPort_FreeBuffer(memberData);
    PCPort_HSDArchiveDestroy(&logoArchive);
    PCPort_FreeBuffer(logoData);
    PCPort_FreeBuffer(logoPixels);
    return ok;
}

int main(int argc, char** argv) {
    int audioInitialized = 0;
    int runRealContentParserSmoke;
    int runRealSceneSlice4Smoke;
    int runRealTevSceneSlice3Smoke;
    int runRealTevSceneSlice2Smoke;
    int runRealTevSceneSliceSmoke;
    int runRealSceneSlice3Smoke;
    int runRealSceneSlice2Smoke;
    int runRealTexturedSceneSliceSmoke;
    int runRealContentTranslationSmoke;
    int runGSgfxPObjSmoke;
    int runGSgfxScissorRetry;
    int runGSgfxSceneLikeSmoke;
    int runGSgfxVisibleSmoke;
    int runGXPrimitiveSmoke;
    int runGXScissorSmoke;
    int osInitialized = 0;
    int runGsGfxSmoke;
    int runWindowSmoke;
    int runMenu;
    int exitCode = 0;
    char trkBuffer[32];
    char trkSuffix[8];
    unsigned long appType;
    unsigned long trkLen;
    unsigned long tvFormat;
    GLFWwindow* window = NULL;

    runWindowSmoke = HasArg(argc, argv, "--window-smoke");
    runGsGfxSmoke = HasArg(argc, argv, "--gsgfx-smoke");
    runRealContentParserSmoke = HasArg(argc, argv, "--real-content-parser-smoke");
    runRealSceneSlice4Smoke = HasArg(argc, argv, "--real-scene-slice-4-smoke");
    runRealTevSceneSlice3Smoke = HasArg(argc, argv, "--real-tev-scene-slice-3-smoke");
    runRealTevSceneSlice2Smoke = HasArg(argc, argv, "--real-tev-scene-slice-2-smoke");
    runRealTevSceneSliceSmoke = HasArg(argc, argv, "--real-tev-scene-slice-smoke");
    runRealSceneSlice3Smoke = HasArg(argc, argv, "--real-scene-slice-3-smoke");
    runRealSceneSlice2Smoke = HasArg(argc, argv, "--real-scene-slice-2-smoke");
    runRealTexturedSceneSliceSmoke = HasArg(argc, argv, "--real-textured-scene-slice-smoke");
    runRealContentTranslationSmoke = HasArg(argc, argv, "--real-content-translation-smoke");
    runGSgfxPObjSmoke = HasArg(argc, argv, "--gsgfx-pobj-smoke");
    runGSgfxScissorRetry = HasArg(argc, argv, "--gsgfx-scissor-retry");
    runGSgfxSceneLikeSmoke = HasArg(argc, argv, "--gsgfx-scene-like-smoke");
    runGSgfxVisibleSmoke = HasArg(argc, argv, "--gsgfx-visible-smoke");
    runGXPrimitiveSmoke = HasArg(argc, argv, "--gx-primitive-smoke");
    runGXScissorSmoke = HasArg(argc, argv, "--gx-scissor-smoke");
    runMenu = HasArg(argc, argv, "--menu");

    printf("[pcport_bootstrap] Starting stub native bootstrap\n");

    if (runWindowSmoke || runGsGfxSmoke || runRealContentParserSmoke ||
        runRealSceneSlice4Smoke ||
        runRealTevSceneSlice3Smoke ||
        runRealTevSceneSlice2Smoke ||
        runRealTevSceneSliceSmoke ||
        runRealSceneSlice3Smoke ||
        runRealSceneSlice2Smoke ||
        runRealTexturedSceneSliceSmoke ||
        runRealContentTranslationSmoke ||
        runGSgfxPObjSmoke ||
        runGSgfxScissorRetry || runGSgfxSceneLikeSmoke ||
        runGSgfxVisibleSmoke ||
        runGXPrimitiveSmoke || runGXScissorSmoke ||
        runMenu || argc <= 1) {
        window = CreateSmokeWindow();
        if (window == NULL) {
            return 1;
        }

        PCPort_SetHostWindow(window);
        printf("[pcport_bootstrap] Native window + GL context created\n");
    }

    OSInit_PC();
    osInitialized = 1;
    PADInit();

    if (!JAudio_Init()) {
        fprintf(stderr, "[pcport_bootstrap] JAudio_Init failed\n");
        exitCode = 1;
        goto cleanup;
    }
    audioInitialized = 1;

    if (!DVDInit_PC()) {
        fprintf(stderr, "[pcport_bootstrap] DVDInit_PC failed\n");
        exitCode = 1;
        goto cleanup;
    }

    GXInit(NULL, 0);

    CurrTvMode = 2;
    tvFormat = VIGetTvFormat();
    if (tvFormat != 1) {
        fprintf(stderr,
                "[pcport_bootstrap] VIGetTvFormat returned %lu, expected 1\n",
                tvFormat);
        exitCode = 1;
        goto cleanup;
    }

    TRK_memcpy(trkSuffix, "trk", 4);
    TRK_memcpy(trkBuffer, "bridge:", 8);
    TRK_strcat(trkBuffer, trkSuffix);
    trkLen = TRK_strlen(trkBuffer);
    if (trkLen != 10) {
        fprintf(stderr,
                "[pcport_bootstrap] TRK utility verification failed (len=%lu)\n",
                trkLen);
        exitCode = 1;
        goto cleanup;
    }

    __OSSetAppType(0x12345678UL);
    appType = __OSGetAppType();
    if (appType != 0x12345678UL) {
        fprintf(stderr,
                "[pcport_bootstrap] OSStateFlags verification failed (appType=%lu)\n",
                appType);
        exitCode = 1;
        goto cleanup;
    }

    printf("[pcport_bootstrap] Stub subsystems initialized\n");
    printf("[pcport_bootstrap] Linked decomp TU VIGetTvFormat verified (mode=%lu)\n",
           tvFormat);
    printf("[pcport_bootstrap] Linked decomp TU TRKUtil verified (%s, len=%lu)\n",
           trkBuffer, trkLen);
    printf("[pcport_bootstrap] Linked decomp TU OSStateFlags verified (appType=0x%08lX)\n",
           appType);
    if (runRealContentParserSmoke) {
        if (!RunRealContentParserSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real FSYS member parsed through host HSD archive bridge\n");
    } else if (runRealSceneSlice4Smoke) {
        if (!RunRealSceneSlice4Smoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real GX_VA_TEX1 scene slice exercised through the existing game-owned draw bridge\n");
    } else if (runRealTevSceneSlice3Smoke) {
        if (!RunRealTevSceneSlice3Smoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Second distinct real TEV scene slice exercised through the existing game-owned draw bridge\n");
    } else if (runRealTevSceneSlice2Smoke) {
        if (!RunRealTevSceneSlice2Smoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Broader real TEV scene slice exercised through the existing game-owned draw bridge\n");
    } else if (runRealTevSceneSliceSmoke) {
        if (!RunRealTevSceneSliceSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real TEV-interpreted scene slice exercised through the existing game-owned draw bridge\n");
    } else if (runRealSceneSlice3Smoke) {
        if (!RunRealSceneSlice3Smoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real multi-object scene slice exercised through the existing game-owned draw bridge\n");
    } else if (runRealTexturedSceneSliceSmoke) {
        if (!RunRealTexturedSceneSliceSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real textured repository-content scene slice exercised through the existing game-owned draw bridge\n");
    } else if (runRealSceneSlice2Smoke) {
        if (!RunRealSceneSlice2Smoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real scene slice 2 exercised through the existing game-owned draw bridge\n");
    } else if (runRealContentTranslationSmoke) {
        if (!RunRealContentTranslationSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real FSYS member translated into host HSD_PObj state\n");
    } else if (runGSgfxPObjSmoke) {
        if (!RunGSgfxPObjSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Game-owned fn_800DAD10 path exercised via HSD_PObj-backed state\n");
    } else if (runGSgfxScissorRetry) {
        if (!RunGSgfxScissorRetry()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Game-owned fn_800D9D68 path exercised\n");
    } else if (runGSgfxSceneLikeSmoke) {
        if (!RunGSgfxSceneLikeSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Game-owned fn_800DAD10 display-list path exercised\n");
    } else if (runGSgfxVisibleSmoke) {
        if (!RunGSgfxVisibleAttempt()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Visible GSgfx_BeginFrame path exercised directly\n");
    } else if (runGXPrimitiveSmoke) {
        if (!RunGXPrimitiveSmoke()) {
            exitCode = 1;
            goto cleanup;
        }
        if (!RunGSgfxVisibleAttempt()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Host GX primitive path and unchanged GSgfx_BeginFrame attempt both executed\n");
    } else if (runGXScissorSmoke) {
        if (!RunGXScissorSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Host GX scissor path exercised\n");
    } else if (runGsGfxSmoke) {
        if (!RunGSgfxSmoke()) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Real game-owned GSgfx callback/render path exercised\n");
    } else if (runWindowSmoke) {
        if (!RunWindowSmokeLoop(window)) {
            fprintf(stderr,
                    "[pcport_bootstrap] Window smoke loop failed to present a frame\n");
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] No game code, assets, or decompiled frame path started\n");
    } else if (runMenu || window != NULL) {
        if (window == NULL) {
            fprintf(stderr,
                    "[pcport_bootstrap] Menu scene requested but no window/GL context available\n");
            exitCode = 1;
            goto cleanup;
        }

        if (!RunMenuScene(window)) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Top-menu scene graph rendered through the existing game-owned draw bridge\n");
    } else {
        printf("[pcport_bootstrap] No game code, assets, or render loop started\n");
    }

cleanup:
    if (audioInitialized) {
        JAudio_Shutdown();
    }

    if (osInitialized) {
        OSShutdown_PC();
    }

    if (window != NULL) {
        PCPort_SetHostWindow(NULL);
        DestroySmokeWindow(window);
    }

    if (exitCode == 0) {
        printf("[pcport_bootstrap] Shutdown complete\n");
    }

    return exitCode;
}
