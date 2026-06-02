#include "audio_shim.h"
#include "dvd_shim.h"
#include "gx_shim.h"
#include "gx_texture.h"
#include "hsd/hsd_pobj.h"
#include "os_shim.h"
#include "pad_shim.h"
#include "pcport_font.h"
#include "pcport_window.h"
#include "real_content_host.h"
#include "thp_player.h"

#include <GLFW/glfw3.h>
#include <direct.h>   /* _chdir (host-only: locate the asset root at startup) */
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
/* P-A engine-fibre spike (engine_spike.c). */
extern int RunFibreSelfTest(void);
extern int RunEngineSpike(GLFWwindow* window);
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

/* Main-menu panel sprite in topmenu.fsys: STORY MODE (Continue / New Game),
 * BATTLE MODE (Colosseum Battle / Battle Now), OPTIONS. Raw 0x80-header RGBA8
 * 276x574 sprite (same format as menu_018). Shown after START is pressed on the
 * title screen -- the first interactive screen transition in the port. */
#define PCPORT_MAIN_MENU_MEMBER   "menu_033"

/* Main-menu chrome sprite sheet in topmenu.fsys (raw 0x80-header RGBA8 774x139):
 * the pointing-hand cursor, the blue Quit button, grey buttons + the description
 * box top edge. The hand cursor (UV u 0.19..0.28, v 0.66..0.99) is drawn to the
 * left of the selected item; the Quit button is UV u 0.0..0.16, v 0.40..0.74. */
#define PCPORT_TOPMENU_CHROME_MEMBER "menu_032"

/* Main-menu blue-swirl background: topmenu.fsys member menu_bg00 (HSD archive),
 * texture index 00 = CMPR 640x480 at decompressed-archive offset 0x73C0. Baked
 * once and drawn full-screen behind the cards (covers the green GSgfx EFB clear
 * with the real artwork instead of the flat-blue stand-in). */
#define PCPORT_MENU_BG_MEMBER  "menu_bg00"
#define PCPORT_MENU_BG_OFFSET  0x73C0u
#define PCPORT_MENU_BG_WIDTH   640
#define PCPORT_MENU_BG_HEIGHT  480

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

/* Dump an RGBA framebuffer to a 24-bit BMP at an explicit path. GL's bottom-up
 * origin matches BMP's, so rows are written as-is. (PCPORT-only.) */
static void DumpFramebufferBMPTo(const unsigned char* rgba, int w, int h,
                                 const char* path) {
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

/* Dump to the path in env PCPORT_DUMP (the single end-of-run screenshot). */
static void DumpFramebufferBMP(const unsigned char* rgba, int w, int h) {
    DumpFramebufferBMPTo(rgba, w, h, getenv("PCPORT_DUMP"));
}

/* Read the back buffer and write it straight to `path` (within-run sequence
 * capture, used to verify the title's drifting animations headlessly). */
static void DumpBackbufferTo(const char* path) {
    unsigned char* px;
    if (path == NULL) return;
    px = (unsigned char*)malloc((size_t)PCPORT_WINDOW_WIDTH *
                                (size_t)PCPORT_WINDOW_HEIGHT * 4u);
    if (px == NULL) return;
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, PCPORT_WINDOW_WIDTH, PCPORT_WINDOW_HEIGHT,
                 GL_RGBA, GL_UNSIGNED_BYTE, px);
    DumpFramebufferBMPTo(px, PCPORT_WINDOW_WIDTH, PCPORT_WINDOW_HEIGHT, path);
    free(px);
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
    /* 2D overlays (sky/logo/PRESS START/copyright) are unlit and full-bright. */
    GXHostSetLightingEnabled(GX_FALSE);
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

/* Like DrawTexturedScreenRect but with a vertical alpha gradient: the top edge
 * is modulated by aTop, the bottom edge by aBottom (linearly interpolated
 * between). Lets a scrolling textured band (drifting clouds / sand-wind) feather
 * into whatever was drawn beneath it instead of ending on a hard seam. The
 * sampled texture's own alpha is multiplied by this gradient (MODULATE), so an
 * opaque sky texture takes the gradient directly and a low-alpha wisp texture
 * stays subtle. Call BeginMenuOverlay() first. */
static void DrawTexturedScreenRectA(GXTexObj* tex,
                                    f32 sx, f32 sy, f32 sw, f32 sh,
                                    f32 u0, f32 v0, f32 u1, f32 v1,
                                    u8 aTop, u8 aBottom) {
    f32 ndcL = ((sx) / 640.0f) * 2.0f - 1.0f;
    f32 ndcR = ((sx + sw) / 640.0f) * 2.0f - 1.0f;
    f32 ndcT = 1.0f - ((sy) / 480.0f) * 2.0f;
    f32 ndcB = 1.0f - ((sy + sh) / 480.0f) * 2.0f;

    GXLoadTexObj(tex, GX_TEXMAP0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXColor4u8(255, 255, 255, aBottom);
    GXPosition3f32(ndcL, ndcB, 0.0f);
    GXTexCoord2f32(u0, v1);
    GXColor4u8(255, 255, 255, aBottom);
    GXPosition3f32(ndcR, ndcB, 0.0f);
    GXTexCoord2f32(u1, v1);
    GXColor4u8(255, 255, 255, aTop);
    GXPosition3f32(ndcR, ndcT, 0.0f);
    GXTexCoord2f32(u1, v0);
    GXColor4u8(255, 255, 255, aTop);
    GXPosition3f32(ndcL, ndcT, 0.0f);
    GXTexCoord2f32(u0, v0);
    GXEnd();
}

/* Synthesise a tileable "sand-wind" wisp texture: faint sandy horizontal streaks
 * on a transparent ground, for the drifting desert-wind overlay on the title.
 * The real game's wind is a particle/haze effect with no single source sprite,
 * so this is a self-contained procedural stand-in. Horizontal wavenumbers are
 * integers so the pattern tiles seamlessly under a GX_REPEAT scroll; a sin
 * vertical envelope fades it out at the top/bottom edges. Returns 1 on success. */
static int BuildSandWindTexture(GXTexObj* tex) {
    enum { WIND_W = 256, WIND_H = 64 };
    const f32 kPi = 3.14159265358979323846f;
    u8* px;
    int x;
    int y;

    px = (u8*)malloc((size_t)WIND_W * WIND_H * 4u);
    if (px == NULL) {
        return 0;
    }
    for (y = 0; y < WIND_H; ++y) {
        f32 fy = (f32)y / (f32)(WIND_H - 1);
        f32 env = sinf(kPi * fy);          /* 0 at edges, 1 mid -> soft band */
        for (x = 0; x < WIND_W; ++x) {
            f32 fx = (f32)x / (f32)WIND_W;  /* 0..1, period = full width */
            f32 n;
            f32 a;
            int o = (y * WIND_W + x) * 4;

            /* A few integer-wavenumber sines, phase-shifted by height, give long
             * thin diagonal wisps that vary along their length. */
            n  = sinf(2.0f * kPi * (2.0f * fx) + 1.3f + 2.0f * fy);
            n += 0.6f * sinf(2.0f * kPi * (5.0f * fx) + 4.1f - 1.5f * fy);
            n += 0.4f * sinf(2.0f * kPi * (9.0f * fx) + 0.7f + 3.0f * fy);
            a = n * 0.25f + 0.5f;           /* ~0..1 */
            if (a < 0.0f) { a = 0.0f; }
            if (a > 1.0f) { a = 1.0f; }
            a = a * a;                      /* soft wispy gaps (not too sharp) */
            a *= env;

            px[o + 0] = 240;                /* warm pale sand */
            px[o + 1] = 228;
            px[o + 2] = 198;
            px[o + 3] = (u8)(a * 30.0f);    /* max ~30/255 -> very subtle haze */
        }
    }
    memset(tex, 0, sizeof(*tex));
    GXHostInitTexObjRGBA8(tex, px, WIND_W, WIND_H, GX_REPEAT, GX_CLAMP);
    free(px);
    return 1;
}

/* Make an RGBA image tile seamlessly left-to-right so it can be GX_REPEAT
 * scrolled without a wrap seam. The title sky texture wraps a sky cylinder in
 * the real game and is only ~seamless (its cloud edges don't quite line up), so
 * a naive horizontal scroll shows a moving vertical seam. Fix: roll the columns
 * by W/2 (the original left|right seam moves to the centre and the new outer
 * edges, being interior-adjacent columns, join seamlessly), then linearly heal
 * the now-central seam over a narrow band. Edits pixels in place. */
static void MakeSeamlessHoriz(u8* px, int w, int h) {
    int half = w / 2;
    int band = 28;            /* heal half-width (px) around the central seam */
    int x;
    int y;
    int c;
    u8* tmp;

    if (px == NULL || w < 4 || h < 1) {
        return;
    }
    if (band > half - 1) { band = half - 1; }
    tmp = (u8*)malloc((size_t)w * h * 4u);
    if (tmp == NULL) {
        return;
    }
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            int sx = (x + half) % w;
            for (c = 0; c < 4; ++c) {
                tmp[(y * w + x) * 4 + c] = px[(y * w + sx) * 4 + c];
            }
        }
    }
    /* Linear-blend the central band [half-band, half+band) between its two
     * endpoints, replacing the hard seam with a smooth ramp. */
    for (y = 0; y < h; ++y) {
        for (c = 0; c < 4; ++c) {
            int lo = tmp[(y * w + (half - band)) * 4 + c];
            int hi = tmp[(y * w + (half + band - 1)) * 4 + c];
            for (x = half - band; x < half + band; ++x) {
                f32 t = (f32)(x - (half - band)) / (f32)(2 * band - 1);
                tmp[(y * w + x) * 4 + c] = (u8)((f32)lo + ((f32)hi - (f32)lo) * t);
            }
        }
    }
    memcpy(px, tmp, (size_t)w * h * 4u);
    free(tmp);
}

/* Draw a solid-colour screen-space rect with no texture (GX_PASSCLR passes the
 * vertex colour straight through). Used for the menu backdrop that covers the
 * green GSgfx EFB clear, and as a cursor/highlight primitive. Restores the
 * textured MODULATE TEV op afterwards so following textured draws are unaffected.
 * Call BeginMenuOverlay() first (alpha blend + ortho state). */
static void DrawSolidScreenRect(f32 sx, f32 sy, f32 sw, f32 sh,
                                u8 r, u8 g, u8 b, u8 a) {
    f32 ndcL = ((sx) / 640.0f) * 2.0f - 1.0f;
    f32 ndcR = ((sx + sw) / 640.0f) * 2.0f - 1.0f;
    f32 ndcT = 1.0f - ((sy) / 480.0f) * 2.0f;
    f32 ndcB = 1.0f - ((sy + sh) / 480.0f) * 2.0f;

    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXColor4u8(r, g, b, a);
    GXPosition3f32(ndcL, ndcB, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXColor4u8(r, g, b, a);
    GXPosition3f32(ndcR, ndcB, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXColor4u8(r, g, b, a);
    GXPosition3f32(ndcR, ndcT, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXColor4u8(r, g, b, a);
    GXPosition3f32(ndcL, ndcT, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXEnd();

    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
}

/* Lazily-built ASCII font atlas: white glyphs with coverage alpha from
 * pcport_font.h, uploaded once as an RGBA texture. The port has no native glyph
 * renderer, so this gives reusable text for the menu/save-prompt/etc. */
static GXTexObj g_fontTex;
static int g_fontReady = 0;

static void EnsureFontAtlas(void) {
    int n;
    int i;
    u8* rgba;

    if (g_fontReady) {
        return;
    }
    n = PCPORT_FONT_ATLAS_W * PCPORT_FONT_ATLAS_H;
    rgba = (u8*)malloc((size_t)n * 4u);
    if (rgba == NULL) {
        return;
    }
    for (i = 0; i < n; ++i) {
        rgba[i * 4 + 0] = 255;
        rgba[i * 4 + 1] = 255;
        rgba[i * 4 + 2] = 255;
        rgba[i * 4 + 3] = kPcportFontAlpha[i];
    }
    memset(&g_fontTex, 0, sizeof(g_fontTex));
    GXHostInitTexObjRGBA8(&g_fontTex, rgba,
                          PCPORT_FONT_ATLAS_W, PCPORT_FONT_ATLAS_H,
                          GX_CLAMP, GX_CLAMP);
    free(rgba);
    g_fontReady = 1;
}

/* Draw one line of ASCII text at screen (x,y), each glyph gw x gh px, tinted
 * (r,g,b,a) via MODULATE against the white atlas (monospace advance = gw).
 * Call BeginMenuOverlay() + EnsureFontAtlas() first. */
static void DrawTextScreen(f32 x, f32 y, f32 gw, f32 gh,
                           u8 r, u8 g, u8 b, u8 a, const char* s) {
    f32 cx = x;
    f32 du = (f32)PCPORT_FONT_CELL_W / (f32)PCPORT_FONT_ATLAS_W;
    f32 dv = (f32)PCPORT_FONT_CELL_H / (f32)PCPORT_FONT_ATLAS_H;

    if (!g_fontReady || s == NULL) {
        return;
    }
    GXLoadTexObj(&g_fontTex, GX_TEXMAP0);
    for (; *s != '\0'; ++s) {
        unsigned char ch = (unsigned char)*s;
        int idx;
        f32 u0, v0, u1, v1, ndcL, ndcR, ndcT, ndcB;

        if (ch == ' ' || ch < (unsigned char)PCPORT_FONT_FIRST ||
            ch > (unsigned char)PCPORT_FONT_LAST) {
            cx += gw;
            continue;
        }
        idx = (int)ch - PCPORT_FONT_FIRST;
        u0 = (f32)(idx % PCPORT_FONT_COLS) * du;
        v0 = (f32)(idx / PCPORT_FONT_COLS) * dv;
        u1 = u0 + du;
        v1 = v0 + dv;
        ndcL = (cx / 640.0f) * 2.0f - 1.0f;
        ndcR = ((cx + gw) / 640.0f) * 2.0f - 1.0f;
        ndcT = 1.0f - (y / 480.0f) * 2.0f;
        ndcB = 1.0f - ((y + gh) / 480.0f) * 2.0f;

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXColor4u8(r, g, b, a); GXPosition3f32(ndcL, ndcB, 0.0f); GXTexCoord2f32(u0, v1);
        GXColor4u8(r, g, b, a); GXPosition3f32(ndcR, ndcB, 0.0f); GXTexCoord2f32(u1, v1);
        GXColor4u8(r, g, b, a); GXPosition3f32(ndcR, ndcT, 0.0f); GXTexCoord2f32(u1, v0);
        GXColor4u8(r, g, b, a); GXPosition3f32(ndcL, ndcT, 0.0f); GXTexCoord2f32(u0, v0);
        GXEnd();
        cx += gw;
    }
}

/* Greedy word-wrap: draw `s` as up to maxLines lines of <= maxChars glyphs,
 * starting at (x,y) with 1.2*gh line pitch. */
static void DrawTextWrapped(f32 x, f32 y, f32 gw, f32 gh, int maxChars, int maxLines,
                            u8 r, u8 g, u8 b, u8 a, const char* s) {
    char line[128];
    char word[80];
    int lineLen = 0;
    int wordLen = 0;
    int lines = 0;
    const char* p = s;
    int done = 0;

    if (s == NULL) {
        return;
    }
    while (!done && lines < maxLines) {
        char c = *p;
        int flushWord = 0;
        int atEnd = 0;

        if (c == '\0') {
            atEnd = 1;
            flushWord = (wordLen > 0);
        } else if (c == ' ') {
            flushWord = 1;
        } else if (wordLen < (int)sizeof(word) - 1) {
            word[wordLen++] = c;
        }

        if (flushWord) {
            int need = wordLen + (lineLen > 0 ? 1 : 0);
            int i;

            if (lineLen + need > maxChars && lineLen > 0) {
                line[lineLen] = '\0';
                DrawTextScreen(x, y + (f32)lines * gh * 1.2f, gw, gh, r, g, b, a, line);
                lines++;
                lineLen = 0;
            }
            if (lines < maxLines) {
                if (lineLen > 0 && lineLen < (int)sizeof(line) - 1) {
                    line[lineLen++] = ' ';
                }
                for (i = 0; i < wordLen && lineLen < (int)sizeof(line) - 1; ++i) {
                    line[lineLen++] = word[i];
                }
            }
            wordLen = 0;
        }
        if (atEnd) {
            if (lineLen > 0 && lines < maxLines) {
                line[lineLen] = '\0';
                DrawTextScreen(x, y + (f32)lines * gh * 1.2f, gw, gh, r, g, b, a, line);
            }
            done = 1;
        } else {
            ++p;
        }
    }
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

/* Load a raw RGBA8 blob (BE u32 width, BE u32 height, then w*h*4 RGBA bytes) from
 * a repo-relative file and upload it as a texture. Used for the title's Espeon
 * cutout, whose on-disc source member isn't identifiable (loaded by numeric ID
 * via still-ASM sprite code; texels not found in any fsys member or raw file by
 * an exhaustive scan) -- so the authentic texture, extracted via a Dolphin
 * texture dump and bundled at tools/pcport_assets/, stands in until/if the disc
 * member is located. Returns 1 on success. */
static int LoadRawRGBABlobTexObj(const char* path, GXTexObj* outTex) {
    FILE* f = fopen(path, "rb");
    unsigned char hdr[8];
    u32 w, h, px;
    u8* pixels;

    if (f == NULL) {
        return 0;
    }
    if (fread(hdr, 1, 8, f) != 8) {
        fclose(f);
        return 0;
    }
    w = ((u32)hdr[0] << 24) | ((u32)hdr[1] << 16) | ((u32)hdr[2] << 8) | hdr[3];
    h = ((u32)hdr[4] << 24) | ((u32)hdr[5] << 16) | ((u32)hdr[6] << 8) | hdr[7];
    if (w == 0u || h == 0u || w > 4096u || h > 4096u) {
        fclose(f);
        return 0;
    }
    px = w * h * 4u;
    pixels = (u8*)malloc(px);
    if (pixels == NULL) {
        fclose(f);
        return 0;
    }
    if (fread(pixels, 1, px, f) != px) {
        free(pixels);
        fclose(f);
        return 0;
    }
    fclose(f);

    memset(outTex, 0, sizeof(*outTex));
    GXHostInitTexObjRGBA8(outTex, pixels, (u16)w, (u16)h, GX_CLAMP, GX_CLAMP);
    free(pixels);
    return 1;
}

/* Load a raw 0x80-header sprite from an arbitrary fsys archive, like
 * LoadRawMenuTexObj but with a caller-chosen archive and CMPR support. The
 * header byte at +4 marks the format: 0x20=RGBA8, 0x04=CMPR, else RGB5A3. Used
 * for the boot logos (nintendo_logo.fsys:logo_nintendo etc. are CMPR 640x480). */
static int LoadFsysSpriteTexObj(const char* archive, const char* member,
                                GXTexObj* outTex) {
    u8* data = NULL;
    u32 size = 0;
    GXDecodedTexture decoded;
    u16 w;
    u16 h;
    u32 format;

    if (!PCPort_LoadFsysMember(archive, member, &data, &size)) {
        return 0;
    }
    if (size < 0x80u) {
        PCPort_FreeBuffer(data);
        return 0;
    }

    w = (u16)(((u16)data[0] << 8) | data[1]);
    h = (u16)(((u16)data[2] << 8) | data[3]);
    if (data[4] == 0x20u) {
        format = (u32)GX_TF_RGBA8;
    } else if (data[4] == 0x04u) {
        format = (u32)GX_TF_CMPR;
    } else {
        format = (u32)GX_TF_RGB5A3;
    }

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
                int debugFlatChar = 0;
                {
                    static int isoChars = -1;
                    static int isoLo = 6, isoHi = 9;
                    if (isoChars < 0) {
                        const char* e = getenv("PCPORT_ISOLATE_CHARS");
                        isoChars = (e != NULL) ? 1 : 0;
                        if (e != NULL && e[0] != '\0' && e[0] != '1') {
                            /* "lo-hi" range, e.g. "6-9" or single "7" */
                            int a = 0, b = 0;
                            if (sscanf(e, "%d-%d", &a, &b) == 2) { isoLo = a; isoHi = b; }
                            else if (sscanf(e, "%d", &a) == 1) { isoLo = isoHi = a; }
                        }
                    }
                    if (isoChars) {
                        if (stats->dobjs < (u32)isoLo || stats->dobjs > (u32)isoHi) {
                            isLogoTex = 1; /* skip everything outside the range */
                        } else if (getenv("PCPORT_ISOLATE_FLAT") != NULL) {
                            debugFlatChar = 1; /* draw bright + unlit */
                        }
                    }
                }
                if (debugFlatChar) {
                    /* Distinct bright flat colour per dobj, lighting OFF, so the
                     * isolated mesh's silhouette + screen position is unambiguous. */
                    static const u32 kDbgColors[4] = {
                        0xFF3030FFu, 0x30FF30FFu, 0x3060FFFFu, 0xFFFF30FFu };
                    PCPortTranslatedMaterial dbgMat;
                    memset(&dbgMat, 0, sizeof(dbgMat));
                    dbgMat.alpha = 1.0f;
                    dbgMat.diffuse = kDbgColors[(stats->dobjs - (u32)6) & 3u];
                    dbgMat.ambient = dbgMat.diffuse;
                    ConfigureTranslatedMaterialPipeline((unsigned int)pipelineId, &dbgMat);
                    GXHostSetVertexAlphaScale(1.0f);
                    drawObject.pipelineId = (unsigned int)pipelineId;
                    fn_801AA568(&translatedPObj.pobj);
                    GXHostSetLightingEnabled(GX_FALSE);
                    fn_800DAD10((void*)&drawObject);
                    stats->drawn++;
                } else if (!((haveTexture == 0 && haveMaterial &&
                       translatedMaterial.alpha < 0.01f) ||
                      isLogoTex)) {
                    /* Enable directional lighting for the 3D scene geometry so
                     * each pillar face is shaded by its angle to the light and
                     * the otherwise flat-tan ruins gain visible 3D form. The 2D
                     * overlays (BeginMenuOverlay) leave lighting disabled, so
                     * they stay full-bright. Disabled again right after the draw
                     * to keep the gate tightly scoped to scene geometry. */
                    fn_801AA568(&translatedPObj.pobj);
                    GXHostSetLightingEnabled(getenv("PCPORT_SCENE_NOLIGHT") != NULL
                                                 ? GX_FALSE : GX_TRUE);
                    fn_800DAD10((void*)&drawObject);
                    GXHostSetLightingEnabled(GX_FALSE);
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
                    if (rjtDbg && stats->dobjs <= 44u) {
                        const PCPortTranslatedTexture* bt =
                            haveTexture ? &translatedTextureExp.stages[0].texture : NULL;
                        printf("[rjt] pobj#%u %s verts=%u texOff=0x%X %ux%u fmt=%u diff=%08X alpha=%.2f cam=(%.0f,%.0f,%.0f)\n",
                               stats->dobjs, haveTexture ? "TEX" : "MAT",
                               drawObject.totalVerts,
                               bt ? bt->imageDataArchiveOffset : 0u,
                               bt ? bt->width : 0u, bt ? bt->height : 0u,
                               bt ? (unsigned)bt->format : 0u,
                               haveMaterial ? translatedMaterial.diffuse : 0u,
                               haveMaterial ? translatedMaterial.alpha : -1.0f,
                               modelViewMatrix[0][3], modelViewMatrix[1][3],
                               modelViewMatrix[2][3]);
                    }
                }

                {
                    static int skinDbg = -1;
                    if (skinDbg < 0) {
                        skinDbg = (getenv("PCPORT_SKIN_POSE_DEBUG") != NULL) ? 1 : 0;
                    }
                    if (skinDbg && stats->dobjs <= 40u) {
                        u16 pflags = translatedPObj.pobj.flags;
                        u32 ptype = (u32)((pflags >> 12) & 3u);
                        u32 uField = PCPort_ReadBigEndianU32(a->storage + pobjOffset + 0x14);
                        /* dobj joint world-space origin (bind) */
                        f32 djx = translatedJoint.modelMatrix[0][3];
                        f32 djy = translatedJoint.modelMatrix[1][3];
                        f32 djz = translatedJoint.modelMatrix[2][3];
                        printf("[skinpose] pobj#%u flags=0x%04X type=%u verts=%u "
                               "uField=0x%X dobjJoint@0x%X bind=(%.1f,%.1f,%.1f) "
                               "aabbMin=(%.1f,%.1f,%.1f) aabbMax=(%.1f,%.1f,%.1f)",
                               stats->dobjs, pflags, ptype,
                               translatedPObj.totalSubmittedVertices,
                               uField, joint, djx, djy, djz,
                               translatedPObj.minPosition[0],
                               translatedPObj.minPosition[1],
                               translatedPObj.minPosition[2],
                               translatedPObj.maxPosition[0],
                               translatedPObj.maxPosition[1],
                               translatedPObj.maxPosition[2]);
                        if (uField != 0u &&
                            ArchiveRangeValid(a, uField, PCPORT_SERIALIZED_JOINT_SIZE)) {
                            PCPortTranslatedJointTransform skinJoint;
                            memset(&skinJoint, 0, sizeof(skinJoint));
                            if (PCPort_TranslateJointChainToMatrixBE(a, rootJoint,
                                                                     uField, &skinJoint)) {
                                printf(" skinJoint@0x%X bind=(%.1f,%.1f,%.1f)",
                                       uField,
                                       skinJoint.modelMatrix[0][3],
                                       skinJoint.modelMatrix[1][3],
                                       skinJoint.modelMatrix[2][3]);
                            } else {
                                printf(" skinJoint@0x%X (chain-resolve FAILED)", uField);
                            }
                        }
                        printf("\n");
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

        /* Serialized HSD_DObjDesc layout: +0x00 class_name, +0x04 next,
         * +0x08 mobj, +0x0C pobj (confirmed src/hsd/hsd_dobj.c:493 reads the
         * next subdesc from +4, and the mobj/pobj reads above use +0x08/+0x0C).
         * Reading "next" from +0x00 (class_name) only ever yielded the first
         * dobj of each joint's chain -- the title's character body meshes are
         * later dobjs in those chains, so the host saw 22/43 dobjs and the
         * characters never drew. */
        nextOffset = PCPort_ReadBigEndianU32(a->storage + dobjOffset + 0x04);
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
 * Host front-end states. The seed of the RunGame() state machine: the title
 * screen reacts to START by advancing to the main-menu panel. More states
 * (save prompt, mode select) plug into this same enum + present loop.
 */
typedef enum PCPortSceneState {
    PCPORT_SCENE_TITLE = 0,
    PCPORT_SCENE_MAIN_MENU = 1,
    PCPORT_SCENE_DIALOG = 2,
    PCPORT_SCENE_SAVE_PROMPT = 3
} PCPortSceneState;

/* Draw a modal message box (dark teal panel + lighter border, white text) at the
 * bottom centre, matching the real game's dialog style. Dims the screen behind.
 * yesNo!=0 draws YES/NO with the selected option highlighted; else an [A] OK hint.
 * Call BeginMenuOverlay() + EnsureFontAtlas() first. */
static void DrawDialogBox(const char* text, int yesNo, int cursor) {
    DrawSolidScreenRect(0.0f, 0.0f, 640.0f, 480.0f, 0, 0, 0, 110);
    DrawSolidScreenRect(72.0f, 300.0f, 496.0f, 138.0f, 96, 124, 142, 250);
    DrawSolidScreenRect(78.0f, 306.0f, 484.0f, 126.0f, 24, 42, 56, 252);
    DrawTextWrapped(98.0f, 320.0f, 11.0f, 17.0f, 36, 3, 228, 236, 244, 255, text);
    if (yesNo) {
        if (cursor == 0) {
            DrawSolidScreenRect(196.0f, 398.0f, 70.0f, 28.0f, 250, 206, 92, 255);
            DrawTextScreen(212.0f, 402.0f, 14.0f, 20.0f, 28, 38, 52, 255, "YES");
            DrawTextScreen(372.0f, 402.0f, 14.0f, 20.0f, 200, 212, 224, 255, "NO");
        } else {
            DrawSolidScreenRect(360.0f, 398.0f, 56.0f, 28.0f, 250, 206, 92, 255);
            DrawTextScreen(212.0f, 402.0f, 14.0f, 20.0f, 200, 212, 224, 255, "YES");
            DrawTextScreen(372.0f, 402.0f, 14.0f, 20.0f, 28, 38, 52, 255, "NO");
        }
    } else {
        DrawTextScreen(296.0f, 404.0f, 10.0f, 15.0f, 170, 185, 205, 255, "[A] OK");
    }
}

/* What a dialog's confirm ("Yes" / A on an info box) does. */
#define PCPORT_DLG_INFO      0   /* info only: A or B dismisses back to the menu */
#define PCPORT_DLG_QUIT      1   /* Yes -> close the window */
#define PCPORT_DLG_CONTINUE  2   /* Yes -> load saved game (not yet implemented) */
#define PCPORT_DLG_NEWGAME   3   /* Yes -> start a new game (not yet implemented) */

/* Host-side save-data presence check. The GC save / memory-card subsystem has
 * no decompiled C (a black box), so save state is reimplemented host-side. For
 * now this only tests whether a save blob exists at PCPORT_SAVE_PATH (env-
 * overridable) -- no GCI container or SHA-1; matches the game's own "no save
 * data" fallback when absent. */
#define PCPORT_SAVE_PATH_DEFAULT "build_pc/colosseum.sav"
static int PCPort_SaveExists(void) {
    const char* path = getenv("PCPORT_SAVE_PATH");
    FILE* f;

    if (path == NULL || path[0] == '\0') {
        path = PCPORT_SAVE_PATH_DEFAULT;
    }
    f = fopen(path, "rb");
    if (f != NULL) {
        fclose(f);
        return 1;
    }
    return 0;
}

/* Main-menu selectable items, in D-pad up/down traversal order. handX/handY is
 * the top-left screen position (640x480) of the pointing-hand cursor sprite for
 * that item. Positions are tuned against the menu_033 card layout (STORY card
 * left, BATTLE card right, OPTIONS + Quit buttons below). */
typedef struct PCPortMenuItem {
    f32 handX;
    f32 handY;
    const char* label;
    const char* desc;
} PCPortMenuItem;

static const PCPortMenuItem kMainMenuItems[] = {
    {  74.0f, 214.0f, "CONTINUE",         "Continue the Story Mode from where it was last saved." },
    {  74.0f, 250.0f, "NEW GAME",         "Start a new Story Mode adventure from the beginning." },
    { 372.0f, 214.0f, "COLOSSEUM BATTLE", "Take on the Colosseum tournaments and challenges." },
    { 372.0f, 250.0f, "BATTLE NOW",       "Set up a quick custom battle with your own rules." },
    { 312.0f, 326.0f, "OPTIONS",          "Adjust game settings such as rumble and sound." },
    { 506.0f, 328.0f, "QUIT",             "Quit the game and return to the title screen." }
};
#define PCPORT_MENU_ITEM_COUNT ((int)(sizeof(kMainMenuItems) / sizeof(kMainMenuItems[0])))

/* Show a single static boot logo (a raw 0x80-header fsys sprite) full-screen.
 * Holds for `seconds` (paced via glfwGetTime), skippable on START/A. Returns 0 if
 * the window was closed (abort the whole sequence), else 1. With dumpFrame>=0 it
 * renders one frame, dumps, and returns (headless verification). prev carries the
 * shared input edge-state across boot items. */
static int BootShowLogo(GLFWwindow* window, const char* archive, const char* member,
                        double seconds, int dumpFrame, u16* prev) {
    GXTexObj tex;
    PADStatus pads[4];
    double start;

    memset(&tex, 0, sizeof(tex));
    memset(pads, 0, sizeof(pads));
    if (!LoadFsysSpriteTexObj(archive, member, &tex)) {
        fprintf(stderr, "[boot] logo load failed: %s:%s (skipping)\n", archive, member);
        return 1;
    }
    printf("[boot] logo %s:%s\n", archive, member);

    if (dumpFrame >= 0) {
        ClearBackbuffer(0.0f, 0.0f, 0.0f);
        GSgfx_BeginFrame();
        BeginMenuOverlay();
        DrawTexturedScreenRect(&tex, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        {
            unsigned char* px = ReadBackbufferImage();
            free(px);
        }
        GSgfxSwapBuffers(1);
        return 1;
    }

    start = glfwGetTime();
    for (;;) {
        u16 held;
        u16 pressed;

        if (window != NULL && glfwWindowShouldClose(window)) {
            return 0;
        }
        VIWaitForRetrace_PC();
        PADRead(pads);
        held = pads[0].button;
        pressed = (u16)(held & ~(*prev));
        *prev = held;
        if (pressed & (GCN_PAD_BUTTON_START | GCN_PAD_BUTTON_A)) {
            break;
        }
        if (glfwGetTime() - start >= seconds) {
            break;
        }
        ClearBackbuffer(0.0f, 0.0f, 0.0f);
        GSgfx_BeginFrame();
        BeginMenuOverlay();
        DrawTexturedScreenRect(&tex, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        GSgfxSwapBuffers(1);
    }
    return 1;
}

/* Play one THP movie full-screen, decoded by thp_player and presented via the 2D
 * quad path. Paced to the movie's fps via glfwGetTime (frames held between vsyncs);
 * START/A skips, window close returns 0. dumpFrame>=0 decodes to that frame, dumps,
 * and returns (headless verification). */
static int BootPlayTHP(GLFWwindow* window, const char* path, int dumpFrame, u16* prev) {
    PCPortTHP* thp = PCPortTHP_Open(path);
    GXTexObj frameTex;
    const unsigned char* rgba = NULL;
    PADStatus pads[4];
    int vw;
    int vh;
    int total;
    int decoded = 0;
    float fps;
    double startTime;

    if (thp == NULL) {
        fprintf(stderr, "[boot] cannot open %s (skipping)\n", path);
        return 1;
    }
    memset(&frameTex, 0, sizeof(frameTex));
    memset(pads, 0, sizeof(pads));
    vw = PCPortTHP_Width(thp);
    vh = PCPortTHP_Height(thp);
    total = PCPortTHP_FrameCount(thp);
    fps = PCPortTHP_Fps(thp);
    if (fps <= 0.0f) {
        fps = 29.97f;
    }
    printf("[boot] playing %s (%dx%d, %d frames, %.2f fps)\n", path, vw, vh, total, fps);

    if (dumpFrame >= 0) {
        int f;
        for (f = 0; f <= dumpFrame; ++f) {
            if (!PCPortTHP_NextFrameRGBA(thp, &rgba)) {
                break;
            }
        }
        if (rgba != NULL) {
            GXHostUpdateTexObjRGBA8(&frameTex, rgba, (u16)vw, (u16)vh);
            ClearBackbuffer(0.0f, 0.0f, 0.0f);
            GSgfx_BeginFrame();
            BeginMenuOverlay();
            DrawTexturedScreenRect(&frameTex, 0.0f, 0.0f, 640.0f, 480.0f,
                                   0.0f, 0.0f, 1.0f, 1.0f);
            {
                unsigned char* px = ReadBackbufferImage();
                free(px);
            }
            GSgfxSwapBuffers(1);
        }
        PCPortTHP_Close(thp);
        return 1;
    }

    startTime = glfwGetTime();
    for (;;) {
        double elapsed;
        int wantFrame;
        u16 held;
        u16 pressed;

        if (window != NULL && glfwWindowShouldClose(window)) {
            PCPortTHP_Close(thp);
            return 0;
        }
        VIWaitForRetrace_PC();
        PADRead(pads);
        held = pads[0].button;
        pressed = (u16)(held & ~(*prev));
        *prev = held;
        if (pressed & (GCN_PAD_BUTTON_START | GCN_PAD_BUTTON_A)) {
            break;
        }
        elapsed = glfwGetTime() - startTime;
        wantFrame = (int)(elapsed * fps);
        while (decoded <= wantFrame) {
            if (!PCPortTHP_NextFrameRGBA(thp, &rgba)) {
                rgba = NULL;
                break;
            }
            GXHostUpdateTexObjRGBA8(&frameTex, rgba, (u16)vw, (u16)vh);
            ++decoded;
        }
        if (rgba == NULL && decoded >= total) {
            break;
        }
        ClearBackbuffer(0.0f, 0.0f, 0.0f);
        GSgfx_BeginFrame();
        if (decoded > 0) {
            BeginMenuOverlay();
            DrawTexturedScreenRect(&frameTex, 0.0f, 0.0f, 640.0f, 480.0f,
                                   0.0f, 0.0f, 1.0f, 1.0f);
        }
        GSgfxSwapBuffers(1);
    }
    PCPortTHP_Close(thp);
    return 1;
}

/*
 * Boot sequence before the title: Nintendo logo (static) -> The Pokemon Company
 * (tpc.thp) -> Genius Sonority (gs_logo.thp) -> opening demo (openingdemo.thp).
 * Static logos come from the *_logo.fsys CMPR sprites; the rest are THP movies.
 * START/A skips the current item; window close aborts. PCPORT_NO_BOOT=1 skips the
 * whole sequence; PCPORT_BOOT_DUMP_FRAME=N dumps the first item (GL path) for
 * headless verification. Must run after GSgfxInit.
 */
static int RunBootSequence(GLFWwindow* window) {
    static const struct {
        int isThp;
        const char* archive;
        const char* member;
    } kBoot[] = {
        { 0, "orig/GC6E01/disc/files/nintendo_logo.fsys", "logo_nintendo" },
        { 1, "orig/GC6E01/disc/files/movie/tpc.thp",        NULL },
        { 1, "orig/GC6E01/disc/files/movie/gs_logo.thp",    NULL },
        { 1, "orig/GC6E01/disc/files/movie/openingdemo.thp", NULL }
    };
    int n = (int)(sizeof(kBoot) / sizeof(kBoot[0]));
    int i;
    u16 prev = 0;
    int dumpFrame = -1;
    const char* e;

    if (getenv("PCPORT_NO_BOOT") != NULL) {
        return 1;
    }
    e = getenv("PCPORT_BOOT_DUMP_FRAME");
    if (e != NULL && e[0] != '\0') {
        dumpFrame = atoi(e);
    }

    for (i = 0; i < n; ++i) {
        int rc;
        if (kBoot[i].isThp) {
            rc = BootPlayTHP(window, kBoot[i].archive, dumpFrame, &prev);
        } else {
            rc = BootShowLogo(window, kBoot[i].archive, kBoot[i].member,
                              2.6, dumpFrame, &prev);
        }
        if (rc == 0) {
            GXHostClearTextureBinding();
            return 0;  /* window closed during boot */
        }
        if (dumpFrame >= 0) {
            GXHostClearTextureBinding();
            return 1;  /* verification: rendered the first item, stop */
        }
    }

    GXHostClearTextureBinding();
    return 1;
}

/* --- Title-screen posed cast -------------------------------------------------
 * The real title composites a set of pre-rendered 2D character/Pokemon cutouts
 * over the desert scene and cycles between sets when idle. These are NOT 3D
 * models (confirmed static in Dolphin) -- the existing 2D textured-quad path
 * (DrawTexturedScreenRect) draws them directly. The cutouts live in
 * `title.fsys` as `t_vs_*` members (0x80-header, format byte@4 = 0x10 = RGB5A3
 * with a soft alpha edge); LoadFsysSpriteTexObj decodes them (non-0x20 -> RGB5A3).
 * Identified set 1 ("c" group): t_vs_c3 = Wes, t_vs_c4 = Rui, t_vs_c2 = Umbreon
 * (t_vs_c1 = Espeon is not in title.fsys -- the default-set 4th cutout is loaded
 * from elsewhere; TBD, fail-loads gracefully until located). Reference layout:
 * both humans lower-LEFT (facing right), both Pokemon lower-RIGHT (facing left).
 * hflip swaps u0/u1 to mirror a cutout so it faces inward. */
typedef struct PCPortTitleCastMember {
    const char* member;     /* title.fsys member name */
    f32 x, y, w, h;         /* screen-space rect (640x480, origin top-left) */
    int hflip;              /* mirror horizontally (face inward) */
    const char* blob;       /* repo-relative raw-RGBA fallback if member absent */
} PCPortTitleCastMember;

/* Entries are drawn IN ORDER (back-to-front), so the in-front cutout of an
 * overlapping pair comes later. The t_vs art is authored already facing the
 * correct title direction (native: Wes/Rui face right, Umbreon/Espeon face
 * left) so hflip stays 0 -- the game does NOT mirror them. Rects measured
 * against the Dolphin reference frame. Left pair: Rui behind, Wes in front;
 * right pair: Umbreon behind, Espeon in front. */
/* Rects EXTRACTED by template-matching each cutout (alpha-weighted, occlusion-
 * masked, front-to-back) against a CLEAN borderless Dolphin F9 set-1 frame --
 * build_pc/logo_probe/match_cutouts.py (CLEAN=1). Order is back-to-front. */
static const PCPortTitleCastMember kTitleCastSet1[] = {
    { "t_vs_c4",  40.0f, 254.0f, 139.0f, 221.0f, 0, NULL },  /* Rui : behind, right-of-Wes */
    { "t_vs_c3",   4.0f, 262.0f, 133.0f, 214.0f, 0, NULL },  /* Wes : front, far-left      */
    { "t_vs_c2", 456.0f, 254.0f, 163.0f, 220.0f, 0, NULL },  /* Umbreon : behind, left     */
    /* Espeon: 135x192 RGB5A3, dumped from Dolphin (not found as a disc member);
     * loaded from the bundled raw-RGBA blob. Front of the right pair. */
    { "t_vs_c1", 508.0f, 292.0f, 129.0f, 184.0f, 0,
      "tools/pcport_assets/title_espeon.rgba" },           /* Espeon : front, right */
};
#define PCPORT_TITLE_CAST_MAX 8
#define PCPORT_TITLE_CAST_ARCHIVE "orig/GC6E01/disc/files/title.fsys"

/* The real title cycles through several cast SETS while idle. Each set is a list
 * of cutouts (back-to-front). Set 1 (protagonists) is template-matched against a
 * clean F9 frame and is exact; the others are provisional placements from the
 * t_vs group IDs + the corner layout, to be refined per-set with a clean F9 shot
 * run through build_pc/logo_probe/discover_set.py. */
static const PCPortTitleCastMember kTitleCastSet2[] = {  /* legendaries */
    { "t_vs_a4",  70.0f, 288.0f, 139.0f, 140.0f, 0, NULL },  /* Kyogre  : behind, left  */
    { "t_vs_a5", -10.0f, 276.0f, 190.0f, 196.0f, 0, NULL },  /* Groudon : front, left   */
    { "t_vs_a3", 438.0f, 256.0f, 152.0f, 205.0f, 0, NULL },  /* Suicune : behind, right */
    { "t_vs_a1", 504.0f, 300.0f, 128.0f, 159.0f, 0, NULL },  /* Raikou  : front, right  */
};
static const PCPortTitleCastMember kTitleCastSet3[] = {  /* starters (partial: only b2/b3 on disc) */
    { "t_vs_b2",  20.0f, 280.0f, 170.0f, 167.0f, 0, NULL },  /* Meganium   : left  */
    { "t_vs_b3", 470.0f, 286.0f, 135.0f, 188.0f, 0, NULL },  /* Feraligatr : right */
};

typedef struct PCPortTitleSet {
    const PCPortTitleCastMember* members;
    int count;
    const char* name;
} PCPortTitleSet;

#define PCPORT_TITLE_SET_COUNT_(arr) ((int)(sizeof(arr)/sizeof((arr)[0])))
static const PCPortTitleSet kTitleSets[] = {
    { kTitleCastSet1, 4, "protagonists" },
    { kTitleCastSet2, 4, "legendaries" },
    { kTitleCastSet3, 2, "starters" },
};
#define PCPORT_TITLE_NUM_SETS ((int)(sizeof(kTitleSets)/sizeof(kTitleSets[0])))
#define PCPORT_TITLE_MAX_SETS 8   /* array dim; must be >= number of kTitleSets entries */

/*
 * Route B boot path: load+parse the top-menu scene once, then present its full
 * joint tree every frame in a persistent window loop. Reads the keyboard/pad
 * each frame (host edge-detector) so START advances title -> main menu. The
 * loop runs until the window is closed; an explicit PCPORT_MENU_FRAMES cap (or
 * PCPORT_DUMP) keeps the headless screenshot path finite. Reuses the resolve/
 * camera/draw primitives proven by the slice smokes.
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
    GXTexObj menu033Tex;
    int haveMenu033 = 0;
    GXTexObj menu032Tex;
    int haveMenu032 = 0;
    int menuCursor = 0;
    PCPortHSDArchive menuBgArchive;
    u8* menuBgData = NULL;
    u32 menuBgSize = 0;
    GXTexObj menuBgTex;
    int haveMenuBg = 0;
    GXTexObj skyTex;
    int haveSky = 0;
    GXTexObj cloudTex;             /* idx19 sky band, GX_REPEAT for the drift scroll */
    int haveCloud = 0;
    GXTexObj windTex;              /* procedural sand-wind wisps, GX_REPEAT */
    int haveWind = 0;
    f32 cloudSpeed = 0.010f;       /* texture-units/sec the clouds drift left */
    f32 cloudSpanX = 1.6f;         /* texture widths across the screen (>1 = smaller clouds) */
    f32 windSpeed = 0.060f;        /* texture-units/sec the sand-wind blows left */
    f32 cloudBandH = 190.0f;       /* sky-band height in px (clouds fade out below) */
    int cloudsEnabled = 1;
    int windEnabled = 1;
    double animTimeForced = -1.0;  /* PCPORT_ANIM_TIME pins the anim clock (headless) */
    GXTexObj titleCastTex[PCPORT_TITLE_MAX_SETS][PCPORT_TITLE_CAST_MAX];
    int titleCastOk[PCPORT_TITLE_MAX_SETS][PCPORT_TITLE_CAST_MAX];
    int titleCastIdx;
    int titleSetI;
    int titleSetIndex = 0;
    int titleSetForced = -1;       /* PCPORT_TITLE_SET=N pins a set (headless capture) */
    double titleCycleSecs = 7.0;   /* PCPORT_CYCLE_SECS overrides */
    double titleCycleStart = 0.0;
    int render3D = 0;
    PADStatus pads[4];
    u16 padHeld = 0;
    u16 padPrev = 0;
    u16 padPressed = 0;
    PCPortSceneState sceneState = PCPORT_SCENE_TITLE;
    int saveExists = 0;
    int dialogKind = PCPORT_DLG_INFO;
    int dialogYesNo = 0;
    int dialogCursor = 0;
    const char* dialogText = NULL;
    int debugStartFrame = -1;
    const char* debugStartEnv;
    int debugCursor = -1;
    const char* debugCursorEnv;
    int debugAFrame = -1;
    const char* debugAEnv;
    const char* seqBase = NULL;    /* PCPORT_DUMP_SEQ: within-run sequence capture */
    int seqEvery = 10;
    int capExplicit;

    memset(&archive, 0, sizeof(archive));
    memset(&translatedCamera, 0, sizeof(translatedCamera));
    memset(&logoArchive, 0, sizeof(logoArchive));
    memset(&logoTex, 0, sizeof(logoTex));
    memset(&menu018Tex, 0, sizeof(menu018Tex));
    memset(&menu033Tex, 0, sizeof(menu033Tex));
    memset(&menu032Tex, 0, sizeof(menu032Tex));
    memset(&menuBgArchive, 0, sizeof(menuBgArchive));
    memset(&menuBgTex, 0, sizeof(menuBgTex));
    memset(&skyTex, 0, sizeof(skyTex));
    memset(&cloudTex, 0, sizeof(cloudTex));
    memset(&windTex, 0, sizeof(windTex));
    memset(pads, 0, sizeof(pads));

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

    /* Loop cap policy: an explicit PCPORT_MENU_FRAMES (or a PCPORT_DUMP request)
     * bounds the loop so the headless screenshot path stays finite; otherwise
     * the loop is uncapped (frameCap==0) and runs until the window is closed --
     * the interactive front-end. */
    capEnv = getenv("PCPORT_MENU_FRAMES");
    capExplicit = (capEnv != NULL && capEnv[0] != '\0');
    frameCap = capExplicit ? atoi(capEnv) : 0;
    if (capExplicit && frameCap <= 0) {
        frameCap = PCPORT_WINDOW_FRAMES;
    }
    dumpRequested = getenv("PCPORT_DUMP") != NULL;

    /* Debug affordance: inject a one-frame START press at PCPORT_DEBUG_START_FRAME
     * so the headless dump can exercise the real edge-detector + title->menu
     * transition without a physical keypress. Parsed before the dump-cap fallback
     * so the fallback can guarantee the loop runs long enough to reach it. */
    debugStartEnv = getenv("PCPORT_DEBUG_START_FRAME");
    if (debugStartEnv != NULL && debugStartEnv[0] != '\0') {
        debugStartFrame = atoi(debugStartEnv);
    }
    /* Debug affordance: pin the main-menu cursor to a fixed item (overrides nav)
     * so a headless dump can verify the hand-cursor position at any item. */
    debugCursorEnv = getenv("PCPORT_DEBUG_CURSOR");
    if (debugCursorEnv != NULL && debugCursorEnv[0] != '\0') {
        debugCursor = atoi(debugCursorEnv);
    }
    /* Debug affordance: inject a one-frame A press at PCPORT_DEBUG_A_FRAME so a
     * headless dump can open/confirm a dialog without a physical keypress. */
    debugAEnv = getenv("PCPORT_DEBUG_A_FRAME");
    if (debugAEnv != NULL && debugAEnv[0] != '\0') {
        debugAFrame = atoi(debugAEnv);
    }
    saveExists = PCPort_SaveExists();

    /* Debug affordance: PCPORT_DUMP_SEQ=<base> writes <base>_<frame>.bmp every
     * PCPORT_DUMP_SEQ_EVERY frames (default 10) within a single run, so the title
     * drift animations can be verified for actual motion from one process. */
    seqBase = getenv("PCPORT_DUMP_SEQ");
    if (seqBase != NULL && seqBase[0] != '\0') {
        const char* ev = getenv("PCPORT_DUMP_SEQ_EVERY");
        if (ev != NULL && atoi(ev) > 0) { seqEvery = atoi(ev); }
    } else {
        seqBase = NULL;
    }

    if (dumpRequested && frameCap <= 0) {
        /* A dump needs a finite "last frame". Make the fallback cap late enough
         * to both reach an injected debug START and capture the frame after it,
         * else the dump would silently grab the pre-transition title. */
        frameCap = PCPORT_GSGFX_SWAPS;
        if (debugStartFrame >= 0 && debugStartFrame + 2 > frameCap) {
            frameCap = debugStartFrame + 2;
        }
    }
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

    /* Boot movies play first (skippable via PCPORT_NO_BOOT). */
    if (!RunBootSequence(window)) {
        ok = 1;  /* window closed during the boot sequence -> clean exit */
        goto cleanup;
    }
    if (getenv("PCPORT_BOOT_DUMP_FRAME") != NULL) {
        ok = 1;  /* boot frame captured for verification -> skip the title */
        goto cleanup;
    }

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

    /* Main-menu panel (shown after START). Loaded up front so the title->menu
     * transition is instant. */
    haveMenu033 = LoadRawMenuTexObj(PCPORT_MAIN_MENU_MEMBER, &menu033Tex);
    if (haveMenu033) {
        printf("[pcport_bootstrap] Main-menu panel (menu_033) loaded\n");
    } else {
        fprintf(stderr,
                "[pcport_bootstrap] Main-menu panel unavailable (START keeps the title)\n");
    }

    /* Main-menu chrome sheet (hand cursor + Quit button). */
    haveMenu032 = LoadRawMenuTexObj(PCPORT_TOPMENU_CHROME_MEMBER, &menu032Tex);
    if (haveMenu032) {
        printf("[pcport_bootstrap] Main-menu chrome (menu_032: hand cursor + Quit) loaded\n");
    }

    /* Title posed cast: load EVERY set's cutouts once (RGBA8/RGB5A3, alpha).
     * The title cycles through kTitleSets[] while idle. */
    for (titleSetI = 0; titleSetI < PCPORT_TITLE_NUM_SETS; ++titleSetI) {
        const PCPortTitleSet* set = &kTitleSets[titleSetI];
        int cnt = set->count;
        if (cnt > PCPORT_TITLE_CAST_MAX) {
            cnt = PCPORT_TITLE_CAST_MAX;
        }
        for (titleCastIdx = 0; titleCastIdx < cnt; ++titleCastIdx) {
            const PCPortTitleCastMember* cm = &set->members[titleCastIdx];
            memset(&titleCastTex[titleSetI][titleCastIdx], 0,
                   sizeof(titleCastTex[titleSetI][titleCastIdx]));
            titleCastOk[titleSetI][titleCastIdx] = LoadFsysSpriteTexObj(
                PCPORT_TITLE_CAST_ARCHIVE, cm->member,
                &titleCastTex[titleSetI][titleCastIdx]);
            if (!titleCastOk[titleSetI][titleCastIdx] && cm->blob != NULL) {
                titleCastOk[titleSetI][titleCastIdx] = LoadRawRGBABlobTexObj(
                    cm->blob, &titleCastTex[titleSetI][titleCastIdx]);
            }
            printf("[pcport_bootstrap] Title cast[%s] %s: %s\n",
                   set->name, cm->member,
                   titleCastOk[titleSetI][titleCastIdx] ? "loaded" : "FAILED");
        }
    }
    {
        const char* fs = getenv("PCPORT_TITLE_SET");
        const char* cs = getenv("PCPORT_CYCLE_SECS");
        if (fs != NULL) {
            titleSetForced = atoi(fs);
            if (titleSetForced >= 0 && titleSetForced < PCPORT_TITLE_NUM_SETS) {
                titleSetIndex = titleSetForced;
            }
        }
        if (cs != NULL) {
            double v = atof(cs);
            if (v > 0.5) { titleCycleSecs = v; }
        }
    }
    {
        /* Title ambient-animation tuning: cloud drift + sand-wind. */
        const char* e;
        if (getenv("PCPORT_NO_CLOUDS") != NULL) { cloudsEnabled = 0; }
        if (getenv("PCPORT_NO_WIND") != NULL) { windEnabled = 0; }
        e = getenv("PCPORT_CLOUD_SPEED");
        if (e != NULL) { cloudSpeed = (f32)atof(e); }
        e = getenv("PCPORT_WIND_SPEED");
        if (e != NULL) { windSpeed = (f32)atof(e); }
        e = getenv("PCPORT_CLOUD_H");
        if (e != NULL) { f32 v = (f32)atof(e); if (v > 10.0f) { cloudBandH = v; } }
        e = getenv("PCPORT_CLOUD_SPAN");
        if (e != NULL) { f32 v = (f32)atof(e); if (v > 0.2f) { cloudSpanX = v; } }
        /* When set, drive the drift off a fixed clock instead of wall-time, so the
         * headless fast-loop (which advances glfwGetTime by ~nothing per frame)
         * can capture a chosen point in the animation deterministically. */
        e = getenv("PCPORT_ANIM_TIME");
        if (e != NULL) { animTimeForced = atof(e); }
    }

    /* Bake the blue-swirl main-menu background (menu_bg00, CMPR 640x480). */
    if (PCPort_LoadFsysMember(PCPORT_REAL_CONTENT_ARCHIVE, PCPORT_MENU_BG_MEMBER,
                              &menuBgData, &menuBgSize) &&
        PCPort_HSDArchiveParseBE(&menuBgArchive, menuBgData, menuBgSize)) {
        PCPortTranslatedTexture bgDesc;
        u8* bgPixels = NULL;
        u32 bgPxSize = 0;

        memset(&bgDesc, 0, sizeof(bgDesc));
        bgDesc.imageDataArchiveOffset = PCPORT_MENU_BG_OFFSET;
        bgDesc.format = GX_TF_CMPR;
        bgDesc.width = PCPORT_MENU_BG_WIDTH;
        bgDesc.height = PCPORT_MENU_BG_HEIGHT;
        if (PCPort_BakeTextureRGBAFromArchiveBE(&menuBgArchive, &bgDesc,
                                                &bgPixels, &bgPxSize)) {
            GXHostInitTexObjRGBA8(&menuBgTex, bgPixels,
                                  PCPORT_MENU_BG_WIDTH, PCPORT_MENU_BG_HEIGHT,
                                  GX_CLAMP, GX_CLAMP);
            PCPort_FreeBuffer(bgPixels);
            haveMenuBg = 1;
            printf("[pcport_bootstrap] Main-menu background (menu_bg00 swirl) baked (%dx%d)\n",
                   PCPORT_MENU_BG_WIDTH, PCPORT_MENU_BG_HEIGHT);
        }
    }
    if (!haveMenuBg) {
        fprintf(stderr,
                "[pcport_bootstrap] Main-menu background unavailable (flat-blue stand-in)\n");
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

    /* Drifting-cloud layer: the same sky texture (blue + clouds fading to tan),
     * but uploaded GX_REPEAT on S so the title can scroll its U over time and the
     * clouds wrap seamlessly. Drawn as a 2D band over the top sky region in the
     * 3D title path (where the scene's own sky reads as flat blue), so the clouds
     * are actually visible and animate. Baked regardless of render3D. */
    if (cloudsEnabled) {
        PCPortTranslatedTexture cloudDesc;
        u8* cloudPixels = NULL;
        u32 cloudPxSize = 0;

        memset(&cloudDesc, 0, sizeof(cloudDesc));
        cloudDesc.imageDataArchiveOffset = PCPORT_TITLE_SKY_OFFSET;
        cloudDesc.format = GX_TF_CMPR;
        cloudDesc.width = PCPORT_TITLE_SKY_WIDTH;
        cloudDesc.height = PCPORT_TITLE_SKY_HEIGHT;
        if (PCPort_BakeTextureRGBAFromArchiveBE(&archive, &cloudDesc,
                                                &cloudPixels, &cloudPxSize)) {
            MakeSeamlessHoriz(cloudPixels, PCPORT_TITLE_SKY_WIDTH,
                              PCPORT_TITLE_SKY_HEIGHT);
            GXHostInitTexObjRGBA8(&cloudTex, cloudPixels,
                                  PCPORT_TITLE_SKY_WIDTH, PCPORT_TITLE_SKY_HEIGHT,
                                  GX_REPEAT, GX_CLAMP);
            PCPort_FreeBuffer(cloudPixels);
            haveCloud = 1;
            printf("[pcport_bootstrap] Title drifting-cloud layer baked (%dx%d, repeat-S)\n",
                   PCPORT_TITLE_SKY_WIDTH, PCPORT_TITLE_SKY_HEIGHT);
        }
    }

    /* Sand-wind layer: procedural tileable wisps, scrolled across the desert. */
    if (windEnabled && BuildSandWindTexture(&windTex)) {
        haveWind = 1;
        printf("[pcport_bootstrap] Title sand-wind layer built (procedural wisps)\n");
    }

    /* Build the ASCII font atlas once (GL context ready) for menu/prompt text. */
    EnsureFontAtlas();

    for (frame = 0; ; ++frame) {
        MenuTreeStats stats;

        if (window != NULL && glfwWindowShouldClose(window)) {
            break;
        }
        if (frameCap > 0 && frame >= frameCap) {
            break;
        }

        memset(&stats, 0, sizeof(stats));

        VIWaitForRetrace_PC();   /* pumps glfwPollEvents -> fresh key state */

        /* Host input + edge detection: read the pad, derive this frame's
         * newly-pressed buttons (held & ~prev), then advance the front-end. */
        PADRead(pads);
        padHeld = pads[0].button;
        if (debugStartFrame >= 0 && frame == debugStartFrame) {
            padHeld = (u16)(padHeld | GCN_PAD_BUTTON_START);
        }
        if (debugAFrame >= 0 && frame == debugAFrame) {
            padHeld = (u16)(padHeld | GCN_PAD_BUTTON_A);
        }
        padPressed = (u16)(padHeld & ~padPrev);
        padPrev = padHeld;

        if (sceneState == PCPORT_SCENE_TITLE) {
            /* Idle cast cycling: advance to the next set every titleCycleSecs of
             * no input (like the real attract title). Any input resets the timer;
             * PCPORT_TITLE_SET pins a set for headless capture. */
            double nowT = glfwGetTime();
            if (titleCycleStart == 0.0) {
                titleCycleStart = nowT;
            }
            if (padPressed != 0) {
                titleCycleStart = nowT;
            } else if (titleSetForced < 0 &&
                       (nowT - titleCycleStart) >= titleCycleSecs) {
                titleSetIndex = (titleSetIndex + 1) % PCPORT_TITLE_NUM_SETS;
                titleCycleStart = nowT;
                printf("[pcport_bootstrap] title cast -> set %d (%s)\n",
                       titleSetIndex, kTitleSets[titleSetIndex].name);
            }
            if ((padPressed & GCN_PAD_BUTTON_START) && haveMenu033) {
                /* The real game checks the memory card on START before the menu. */
                sceneState = PCPORT_SCENE_SAVE_PROMPT;
                printf("[pcport_bootstrap] START pressed (frame %d) -> save prompt\n",
                       frame);
            }
        } else if (sceneState == PCPORT_SCENE_SAVE_PROMPT) {
            if (padPressed & (GCN_PAD_BUTTON_START | GCN_PAD_BUTTON_A |
                              GCN_PAD_BUTTON_B)) {
                sceneState = PCPORT_SCENE_MAIN_MENU;
                menuCursor = 0;
                printf("[pcport_bootstrap] save prompt dismissed -> main menu\n");
            }
        } else if (sceneState == PCPORT_SCENE_MAIN_MENU) {
            if (padPressed & GCN_PAD_BUTTON_DOWN) {
                menuCursor = (menuCursor + 1) % PCPORT_MENU_ITEM_COUNT;
            }
            if (padPressed & GCN_PAD_BUTTON_UP) {
                menuCursor = (menuCursor + PCPORT_MENU_ITEM_COUNT - 1) %
                             PCPORT_MENU_ITEM_COUNT;
            }
            if (debugCursor >= 0) {
                menuCursor = debugCursor % PCPORT_MENU_ITEM_COUNT; /* pin for headless capture */
            }
            if (padPressed & GCN_PAD_BUTTON_A) {
                /* Open the dialog appropriate to the selected item. */
                dialogCursor = 0;
                switch (menuCursor) {
                case 0: /* CONTINUE */
                    if (saveExists) {
                        dialogYesNo = 1; dialogKind = PCPORT_DLG_CONTINUE;
                        dialogText = "Load the saved game and continue Story Mode?";
                    } else {
                        dialogYesNo = 0; dialogKind = PCPORT_DLG_INFO;
                        dialogText = "There is no saved game data to continue.";
                    }
                    break;
                case 1: /* NEW GAME */
                    if (saveExists) {
                        dialogYesNo = 1; dialogKind = PCPORT_DLG_NEWGAME;
                        dialogText = "A saved game already exists. Overwrite it and start a new adventure?";
                    } else {
                        dialogYesNo = 0; dialogKind = PCPORT_DLG_INFO;
                        dialogText = "A new adventure begins! (Story Mode is not yet playable in this port.)";
                    }
                    break;
                case 2: /* COLOSSEUM BATTLE */
                    dialogYesNo = 0; dialogKind = PCPORT_DLG_INFO;
                    dialogText = "Colosseum Battle is not yet available in this port.";
                    break;
                case 3: /* BATTLE NOW */
                    dialogYesNo = 0; dialogKind = PCPORT_DLG_INFO;
                    dialogText = "Battle Now is not yet available in this port.";
                    break;
                case 4: /* OPTIONS */
                    dialogYesNo = 0; dialogKind = PCPORT_DLG_INFO;
                    dialogText = "Options are not yet available in this port.";
                    break;
                default: /* QUIT */
                    dialogYesNo = 1; dialogKind = PCPORT_DLG_QUIT;
                    dialogText = "Quit the game?";
                    break;
                }
                sceneState = PCPORT_SCENE_DIALOG;
                printf("[pcport_bootstrap] Selected %s -> dialog\n",
                       kMainMenuItems[menuCursor].label);
            }
            if (padPressed & GCN_PAD_BUTTON_B) {
                sceneState = PCPORT_SCENE_TITLE;
                printf("[pcport_bootstrap] B pressed -> back to title\n");
            }
        } else { /* PCPORT_SCENE_DIALOG */
            if (dialogYesNo) {
                if (padPressed & GCN_PAD_BUTTON_LEFT) {
                    dialogCursor = 0; /* Yes */
                }
                if (padPressed & GCN_PAD_BUTTON_RIGHT) {
                    dialogCursor = 1; /* No */
                }
                if (padPressed & GCN_PAD_BUTTON_A) {
                    if (dialogCursor == 0) { /* Yes */
                        if (dialogKind == PCPORT_DLG_QUIT) {
                            if (window != NULL) {
                                glfwSetWindowShouldClose(window, GLFW_TRUE);
                            }
                            printf("[pcport_bootstrap] Quit confirmed -> closing\n");
                        } else {
                            printf("[pcport_bootstrap] Confirmed (load/new-game not yet implemented)\n");
                        }
                    }
                    sceneState = PCPORT_SCENE_MAIN_MENU;
                }
                if (padPressed & GCN_PAD_BUTTON_B) {
                    sceneState = PCPORT_SCENE_MAIN_MENU;
                }
            } else { /* info box: A or B dismisses */
                if (padPressed & (GCN_PAD_BUTTON_A | GCN_PAD_BUTTON_B)) {
                    sceneState = PCPORT_SCENE_MAIN_MENU;
                }
            }
        }

        /* Blue backdrop for the main menu (the real game's swirl background is a
         * separate topmenu.fsys member, not yet located -- placeholder for now);
         * black behind the 3D title. */
        if (sceneState == PCPORT_SCENE_MAIN_MENU) {
            ClearBackbuffer(0.16f, 0.22f, 0.45f);
        } else {
            ClearBackbuffer(0.0f, 0.0f, 0.0f);
        }
        GSgfx_BeginFrame();

        if (render3D && (sceneState == PCPORT_SCENE_TITLE ||
                         sceneState == PCPORT_SCENE_SAVE_PROMPT)) {
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
            if (frame == 0 && getenv("PCPORT_RENDER_DEBUG") != NULL) {
                printf("[cam] eye=(%.1f,%.1f,%.1f) interest=(%.1f,%.1f,%.1f) fov=%.1f aspect=%.2f near=%.2f far=%.2f\n",
                       translatedCamera.eye[0], translatedCamera.eye[1], translatedCamera.eye[2],
                       translatedCamera.interest[0], translatedCamera.interest[1], translatedCamera.interest[2],
                       translatedCamera.fov, translatedCamera.aspect,
                       translatedCamera.nearZ, translatedCamera.farZ);
            }
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

        /* 2D overlay, selected by the current front-end state. The sky/sand
         * backdrop (when 3D is off) is shared by both states; the title draws
         * logo + PRESS START + copyright, the main menu draws the menu_033
         * panel. The 3D scene is gated behind PCPORT_RENDER_3D. */
        if (haveSky || haveCloud || haveWind || haveLogo ||
            haveMenu018 || haveMenu033 || haveMenu032) {
            BeginMenuOverlay();
            if (sceneState == PCPORT_SCENE_TITLE ||
                sceneState == PCPORT_SCENE_SAVE_PROMPT) {
                if (haveSky) {
                    DrawTexturedScreenRect(&skyTex, 0.0f, 0.0f, 640.0f, 480.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f);
                }
                /* Drifting clouds: scroll the sky band's U to the left over time
                 * (GX_REPEAT wraps it). Drawn as an opaque upper band plus a lower
                 * feather strip that fades into the 3D desert, so there is no hard
                 * horizon seam. The texture's own tan-fading bottom helps the blend. */
                if (haveCloud) {
                    double animT = (animTimeForced >= 0.0)
                                       ? animTimeForced : glfwGetTime();
                    f32 cu0 = (f32)(animT * (double)cloudSpeed);
                    f32 cu1 = cu0 + cloudSpanX;  /* >1 tile = smaller, less-stretched clouds */
                    f32 bandH = cloudBandH;
                    f32 feat = 70.0f;            /* long fade into the warm-tan horizon */
                    f32 vMid;
                    /* Slightly translucent so the 3D sky's blue reads through (softer,
                     * matches the game's hazier sky than a hard texture replace). */
                    u8 aMain = 232;
                    if (feat > bandH) { feat = bandH; }
                    vMid = (bandH - feat) / bandH;
                    DrawTexturedScreenRectA(&cloudTex, 0.0f, 0.0f, 640.0f, bandH - feat,
                                            cu0, 0.0f, cu1, vMid, aMain, aMain);
                    DrawTexturedScreenRectA(&cloudTex, 0.0f, bandH - feat, 640.0f, feat,
                                            cu0, vMid, cu1, 1.0f, aMain, 0);
                }
                /* Sand-wind: faint sandy wisps drifting left over the desert,
                 * feathered top and bottom so they sit subtly on the ground. */
                if (haveWind) {
                    double animT = (animTimeForced >= 0.0)
                                       ? animTimeForced : glfwGetTime();
                    f32 wu0 = (f32)(animT * (double)windSpeed);
                    f32 wu1 = wu0 + 2.5f;        /* tile ~2.5x for finer streaks */
                    DrawTexturedScreenRectA(&windTex, 0.0f, 196.0f, 640.0f, 236.0f,
                                            wu0, 0.0f, wu1, 1.0f, 255, 255);
                }
                /* Posed cast cutouts for the active cycling set, over the desert
                 * but UNDER the logo (heads may tuck behind the centre logo). */
                {
                    const PCPortTitleSet* aset = &kTitleSets[titleSetIndex];
                    int acnt = aset->count;
                    if (acnt > PCPORT_TITLE_CAST_MAX) { acnt = PCPORT_TITLE_CAST_MAX; }
                    for (titleCastIdx = 0; titleCastIdx < acnt; ++titleCastIdx) {
                        const PCPortTitleCastMember* cm = &aset->members[titleCastIdx];
                        f32 cu0 = cm->hflip ? 1.0f : 0.0f;
                        f32 cu1 = cm->hflip ? 0.0f : 1.0f;
                        if (!titleCastOk[titleSetIndex][titleCastIdx]) {
                            continue;
                        }
                        DrawTexturedScreenRect(&titleCastTex[titleSetIndex][titleCastIdx],
                                               cm->x, cm->y, cm->w, cm->h,
                                               cu0, 0.0f, cu1, 1.0f);
                    }
                }
                if (haveLogo) {
                    /* logo, top-centre (540:224 aspect) */
                    DrawTexturedScreenRect(&logoTex, 115.0f, 34.0f, 410.0f, 170.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f);
                }
                if (haveMenu018) {
                    /* PRESS START only on the title itself (not while the save
                     * prompt is up), then the copyright block on both. */
                    if (sceneState == PCPORT_SCENE_TITLE) {
                        DrawTexturedScreenRect(&menu018Tex, 188.0f, 268.0f, 264.0f, 30.0f,
                                               0.0f, 0.574f, 1.0f, 0.721f);
                    }
                    /* copyright block (v0.016..0.549), bottom-left */
                    DrawTexturedScreenRect(&menu018Tex, 28.0f, 392.0f, 300.0f, 58.0f,
                                           0.0f, 0.016f, 1.0f, 0.549f);
                }
                /* Memory-card read prompt over the title (matches the real boot). */
                if (sceneState == PCPORT_SCENE_SAVE_PROMPT) {
                    DrawDialogBox("The Memory Card in Slot A has been read!", 0, 0);
                }
            } else { /* PCPORT_SCENE_MAIN_MENU: composite the real layout from the sheets */
                /* Opaque backdrop FIRST -- the game's draw path leaves a green EFB
                 * clear that shows through everywhere; this covers it. Use the real
                 * menu_bg00 blue-swirl artwork when baked, else a flat-blue quad. */
                if (haveMenuBg) {
                    DrawTexturedScreenRect(&menuBgTex, 0.0f, 0.0f, 640.0f, 480.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f);
                } else {
                    DrawSolidScreenRect(0.0f, 0.0f, 640.0f, 480.0f, 28, 44, 92, 255);
                }
                if (haveMenu033) {
                    /* STORY MODE card (sheet v 0.00-0.402) -> screen left (centred,
                     * narrower so it isn't clipped at the window edge) */
                    DrawTexturedScreenRect(&menu033Tex, 44.0f, 34.0f, 256.0f, 254.0f,
                                           0.0f, 0.000f, 1.0f, 0.402f);
                    /* BATTLE MODE card (sheet v 0.408-0.863) -> screen right */
                    DrawTexturedScreenRect(&menu033Tex, 344.0f, 34.0f, 256.0f, 254.0f,
                                           0.0f, 0.408f, 1.0f, 0.863f);
                    /* OPTIONS green pill (sheet full pill u 0.015-0.875, v 0.872-0.949) */
                    DrawTexturedScreenRect(&menu033Tex, 336.0f, 320.0f, 188.0f, 42.0f,
                                           0.015f, 0.872f, 0.875f, 0.949f);
                }
                if (haveMenu032) {
                    /* Quit button (chrome sheet u 0.0-0.162, v 0.40-0.74) */
                    DrawTexturedScreenRect(&menu032Tex, 528.0f, 322.0f, 92.0f, 42.0f,
                                           0.0f, 0.40f, 0.162f, 0.74f);
                    /* pointing-hand cursor at the selected item (u 0.185-0.285, v 0.66-0.99) */
                    DrawTexturedScreenRect(&menu032Tex,
                                           kMainMenuItems[menuCursor].handX,
                                           kMainMenuItems[menuCursor].handY,
                                           48.0f, 38.0f,
                                           0.185f, 0.66f, 0.285f, 0.99f);
                }
                /* Bottom description box: dark teal panel + lighter border with
                 * white text for the selected item (matches the real game). */
                DrawSolidScreenRect(46.0f, 380.0f, 548.0f, 84.0f, 96, 124, 142, 240);
                DrawSolidScreenRect(50.0f, 384.0f, 540.0f, 76.0f, 26, 44, 58, 245);
                DrawTextWrapped(68.0f, 400.0f, 12.0f, 18.0f, 40, 2,
                                228, 236, 244, 255,
                                kMainMenuItems[menuCursor].desc);

                /* Selection dialog over the (dimmed) menu. */
                if (sceneState == PCPORT_SCENE_DIALOG) {
                    DrawDialogBox(dialogText, dialogYesNo, dialogCursor);
                }
            }
        }

        /* On the final frame, capture the framebuffer (BMP dump happens inside
         * ReadBackbufferImage via DumpFramebufferBMP when PCPORT_DUMP is set)
         * before presenting. */
        if (dumpRequested && frameCap > 0 && frame == frameCap - 1) {
            unsigned char* pixels = ReadBackbufferImage();
            free(pixels);
        }
        if (seqBase != NULL && (frame % seqEvery) == 0) {
            char seqPath[1024];
            snprintf(seqPath, sizeof(seqPath), "%s_%04d.bmp", seqBase, frame);
            DumpBackbufferTo(seqPath);
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
    PCPort_HSDArchiveDestroy(&menuBgArchive);
    PCPort_FreeBuffer(menuBgData);
    return ok;
}

/* Make the working directory the asset root so the game's relative asset paths
 * (orig/GC6E01/disc/files/...) resolve no matter where the exe is launched from
 * (double-clicked, or run from build_pc/). Walks up from the exe's own directory
 * looking for orig/GC6E01/disc/files/title.fsys and chdir()s there. */
static void PCPort_ChdirToAssetRoot(const char* argv0) {
    char dir[1024];
    char probe[1152];
    size_t len;
    int i;
    int slash;

    if (argv0 == NULL) {
        return;
    }
    len = strlen(argv0);
    if (len == 0 || len >= sizeof(dir)) {
        return;
    }
    memcpy(dir, argv0, len + 1);
    slash = -1;
    for (i = (int)len - 1; i >= 0; --i) {
        if (dir[i] == '/' || dir[i] == '\\') {
            slash = i;
            break;
        }
    }
    if (slash < 0) {
        dir[0] = '.';
        dir[1] = '\0';
    } else {
        dir[slash] = '\0';   /* directory containing the exe */
    }

    for (i = 0; i < 7; ++i) {
        FILE* f;
        size_t dl;

        snprintf(probe, sizeof(probe),
                 "%s/orig/GC6E01/disc/files/title.fsys", dir);
        f = fopen(probe, "rb");
        if (f != NULL) {
            fclose(f);
            if (_chdir(dir) == 0) {
                printf("[pcport_bootstrap] asset root: %s\n", dir);
            }
            return;
        }
        dl = strlen(dir);
        if (dl + 4 >= sizeof(dir)) {
            return;
        }
        memcpy(dir + dl, "/..", 4);   /* go up one level */
    }
}

/* THP decode smoke (no GL): decode one frame of a movie to RGBA via thp_player +
 * stb_image and write it as a PPM, to verify the decode path in the host build
 * independent of the GL present path. PCPORT_THP_FILE selects the movie (default
 * gs_logo), PCPORT_THP_FRAME the frame index, PCPORT_THP_OUT the output path. */
static int RunTHPSmoke(void) {
    const char* path = getenv("PCPORT_THP_FILE");
    const char* frameEnv = getenv("PCPORT_THP_FRAME");
    const char* outPath = getenv("PCPORT_THP_OUT");
    int wantFrame = (frameEnv != NULL && frameEnv[0] != '\0') ? atoi(frameEnv) : 0;
    const unsigned char* rgba = NULL;
    PCPortTHP* thp;
    FILE* out;
    int w;
    int h;
    int f;

    if (path == NULL || path[0] == '\0') {
        path = "orig/GC6E01/disc/files/movie/gs_logo.thp";
    }
    if (outPath == NULL || outPath[0] == '\0') {
        outPath = "build_pc/thp_smoke.ppm";
    }
    thp = PCPortTHP_Open(path);
    if (thp == NULL) {
        fprintf(stderr, "[thp-smoke] failed to open %s\n", path);
        return 0;
    }
    w = PCPortTHP_Width(thp);
    h = PCPortTHP_Height(thp);
    printf("[thp-smoke] %s: %dx%d, %d frames, %.2f fps\n",
           path, w, h, PCPortTHP_FrameCount(thp), PCPortTHP_Fps(thp));
    for (f = 0; f <= wantFrame; ++f) {
        if (!PCPortTHP_NextFrameRGBA(thp, &rgba)) {
            fprintf(stderr, "[thp-smoke] decode stopped before frame %d\n", wantFrame);
            PCPortTHP_Close(thp);
            return 0;
        }
    }
    out = fopen(outPath, "wb");
    if (out == NULL) {
        fprintf(stderr, "[thp-smoke] cannot write %s\n", outPath);
        PCPortTHP_Close(thp);
        return 0;
    }
    {
        int i;
        fprintf(out, "P6\n%d %d\n255\n", w, h);
        for (i = 0; i < w * h; ++i) {
            fwrite(rgba + (size_t)i * 4, 1, 3, out); /* RGB, drop alpha */
        }
    }
    fclose(out);
    printf("[thp-smoke] wrote %s (frame %d, %dx%d)\n", outPath, wantFrame, w, h);
    PCPortTHP_Close(thp);
    return 1;
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
    int runEngine;
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
    runEngine = HasArg(argc, argv, "--engine");

    printf("[pcport_bootstrap] Starting stub native bootstrap\n");

    /* Resolve assets relative to the exe, so launching by double-click / from any
     * directory works (assets are loaded via repo-relative paths). */
    PCPort_ChdirToAssetRoot(argc > 0 ? argv[0] : NULL);

    /* THP decode smoke: pure decode -> PPM, no window/GL. Verifies thp_player +
     * stb_image in the host build. */
    if (HasArg(argc, argv, "--thp-smoke")) {
        return RunTHPSmoke() ? 0 : 1;
    }

    /* P-A spike: headless cooperative-fibre scheduler self-test. No window/GL —
     * proves the fn_800F0308 vsync-yield semantics run natively on host fibres. */
    if (HasArg(argc, argv, "--fibre-test")) {
        return RunFibreSelfTest() ? 0 : 1;
    }

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
        runMenu || runEngine || argc <= 1) {
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
    } else if (runEngine) {
        if (window == NULL) {
            fprintf(stderr,
                    "[pcport_bootstrap] --engine requested but no window/GL context available\n");
            exitCode = 1;
            goto cleanup;
        }

        if (!RunEngineSpike(window)) {
            exitCode = 1;
            goto cleanup;
        }

        printf("[pcport_bootstrap] Engine-fibre spike: host<->engine cooperative round-trip ticked frames\n");
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
