/**
 * @file gx_shim.c
 * @brief GX-to-OpenGL 3.3 translation layer -- stub implementations.
 *
 * Each function is a stub that will be filled in during Phase 3 to translate
 * the GCN GX call into the equivalent OpenGL 3.3 operation.
 *
 * References:
 *   - docs/pc_port_design.md Section 8 (Core API Mapping Table)
 *   - gx_shim.h for full function documentation
 *   - gx_tev.h for TEV-to-GLSL shader generation
 *   - gx_texture.h for GCN texture format decoding
 *
 * Phase 3 PC port scaffolding -- skeleton only.
 */

#include "gx_shim.h"
#include "gx_tev.h"
#include "gx_texture.h"

#include <stdio.h>
#include <string.h>

/* TODO: Include OpenGL headers when build system is ready
 * #include <glad/glad.h>
 * #include <GLFW/glfw3.h>
 */

/* =========================================================================
 * Internal state tracking
 *
 * The GX shim maintains a shadow of the GX state machine so that
 * redundant GL state changes can be avoided.
 * ========================================================================= */

/** Current TEV stage configuration (fed to shader generator) */
static GXTevState g_tevState;

/** Number of active TEV stages */
static u8 g_numTevStages = 1;

/** Current projection matrix */
static f32 g_projMatrix[4][4];
static GXProjectionType g_projType;

/** Current modelview matrix slots (GX supports 10 position matrix slots) */
static f32 g_posMtx[10][3][4];
static f32 g_nrmMtx[10][3][4];

/** Current viewport parameters */
static f32 g_viewportX, g_viewportY, g_viewportW, g_viewportH;
static f32 g_viewportNear, g_viewportFar;

/** Scissor state */
static u32 g_scissorX, g_scissorY, g_scissorW, g_scissorH;

/** Blend state */
static GXBlendMode g_blendType;
static GXBlendFactor g_blendSrc, g_blendDst;
static GXLogicOp g_blendLogicOp;

/** Alpha compare state */
static GXCompare g_alphaComp0, g_alphaComp1;
static u8 g_alphaRef0, g_alphaRef1;
static GXAlphaOp g_alphaOp;

/** Depth state */
static GXBool g_zEnable, g_zUpdate;
static GXCompare g_zFunc;

/** Fog state */
static GXFogType g_fogType;
static f32 g_fogStart, g_fogEnd, g_fogNear, g_fogFar;
static GXColor g_fogColor;

/** Cull mode */
static GXCullMode g_cullMode;

/** Channel state (lighting) */
static GXColor g_chanAmbColor[2];
static GXColor g_chanMatColor[2];

/** TEV color/konst registers */
static GXColor g_tevColorRegs[4];
static GXColor g_tevKonstRegs[4];

/** Light objects */
static GXLightObj g_lightObjs[8];

/** Immediate-mode vertex accumulation buffer */
#define GX_IMM_VTX_MAX 65536

typedef struct {
    f32 pos[3];
    u8  color[4];
    f32 texcoord[2];
} GXImmVertex;

static GXImmVertex g_immVertices[GX_IMM_VTX_MAX];
static u32 g_immVertexCount = 0;
static GXPrimitive g_immPrimType;
static u16 g_immExpectedVerts = 0;

/* =========================================================================
 * 1. Initialization and FIFO
 * ========================================================================= */

void GXInit(void* base, u32 size) {
    (void)base; (void)size;
    /* TODO: Phase 3a -- Initialize OpenGL 3.3 context
     *
     * 1. Verify that GLFW has already created a window + GL context
     *    (this should be done in pcport_main.c before GXInit is called)
     * 2. Call gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)
     * 3. Set default GL state:
     *    - glEnable(GL_DEPTH_TEST)
     *    - glDepthFunc(GL_LEQUAL)
     *    - glEnable(GL_BLEND)
     *    - glBlendFunc(GL_ONE, GL_ONE) -- default additive blend
     *    - glEnable(GL_CULL_FACE)
     *    - glFrontFace(GL_CW) -- GCN uses clockwise winding
     * 4. Initialize the TEV shader cache (gx_tev_init())
     * 5. Create the immediate-mode VBO/VAO for GXBegin/GXEnd
     * 6. Initialize texture decode tables
     */

    memset(&g_tevState, 0, sizeof(g_tevState));
    g_numTevStages = 1;
    g_immVertexCount = 0;

    printf("[gx_shim] GXInit stub -- OpenGL init goes here\n");
}

GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb) {
    (void)cb;
    /* TODO: No-op on PC -- OpenGL is synchronous.
     * The game's GSgfx_DrawDoneCallback just sets a flag;
     * on PC we can set it immediately or ignore it.
     */
    return (GXDrawDoneCallback)0;
}

void GXSetDispCopyGamma(GXGamma gamma) {
    (void)gamma;
    /* TODO: Phase 3h -- Gamma correction
     * If using an sRGB framebuffer: glEnable(GL_FRAMEBUFFER_SRGB)
     * Otherwise: apply gamma in a post-processing pass
     * The game always sets gamma=GX_GM_1_0 (linear), so this is
     * usually a no-op.
     */
}

/* =========================================================================
 * 2. Viewport, Scissor, Projection
 * ========================================================================= */

void GXSetViewport(f32 xOrig, f32 yOrig, f32 wd, f32 ht,
                   f32 nearZ, f32 farZ) {
    g_viewportX = xOrig;
    g_viewportY = yOrig;
    g_viewportW = wd;
    g_viewportH = ht;
    g_viewportNear = nearZ;
    g_viewportFar = farZ;

    /* TODO: Phase 3a -- Translate to OpenGL
     *
     * GCN viewport origin is top-left; OpenGL origin is bottom-left.
     * Convert: glViewport((GLint)xOrig,
     *                     (GLint)(framebufferHeight - yOrig - ht),
     *                     (GLsizei)wd, (GLsizei)ht);
     * glDepthRange(nearZ, farZ);
     *
     * Note: GCN depth range is [0,1], same as GL default.
     * For widescreen support, scale wd by the aspect ratio override.
     */
}

void GXSetProjection(Mtx44 mtx, GXProjectionType type) {
    g_projType = type;
    memcpy(g_projMatrix, mtx, sizeof(g_projMatrix));

    /* TODO: Phase 3b -- Upload projection matrix
     *
     * GCN uses a different projection matrix convention than OpenGL:
     * - GCN clip space Z: [-1, 0] (near = -1, far = 0)
     * - OpenGL clip space Z: [-1, 1] (near = -1, far = 1)
     *
     * Apply the correction: projMatrix[2][2] and projMatrix[2][3]
     * need to be adjusted to map from GCN Z range to GL Z range.
     *
     * Upload via: glUniformMatrix4fv(u_projMatrix_loc, 1, GL_TRUE,
     *                                 (GLfloat*)correctedMtx);
     * (GL_TRUE for row-major -> GL column-major transpose)
     */
}

void GXSetScissor(u32 xOrig, u32 yOrig, u32 wd, u32 ht) {
    g_scissorX = xOrig;
    g_scissorY = yOrig;
    g_scissorW = wd;
    g_scissorH = ht;

    /* TODO: Phase 3a -- Translate to OpenGL
     *
     * glEnable(GL_SCISSOR_TEST);
     * glScissor(xOrig, framebufferHeight - yOrig - ht, wd, ht);
     *
     * Same Y-flip as viewport.
     */
}

/* =========================================================================
 * 3. Matrix Operations
 * ========================================================================= */

void GXLoadPosMtxImm(Mtx mtx, u32 id) {
    if (id >= 10) return;
    memcpy(g_posMtx[id], mtx, sizeof(Mtx));

    /* TODO: Phase 3b -- Upload modelview matrix
     *
     * If id == 0 (the default matrix slot used for most rendering):
     *   glUniformMatrix4x3fv(u_modelViewMatrix_loc, 1, GL_TRUE,
     *                         (GLfloat*)mtx);
     * Or expand to 4x4 and use glUniformMatrix4fv.
     *
     * For skinning (envelope mode), matrix slots 1-9 hold bone matrices:
     *   glUniformMatrix4fv(u_boneMatrix_loc + id, 1, GL_TRUE, ...)
     */
}

void GXLoadNrmMtxImm(Mtx mtx, u32 id) {
    if (id >= 10) return;
    memcpy(g_nrmMtx[id], mtx, sizeof(Mtx));

    /* TODO: Phase 3b -- Upload normal matrix
     *
     * The normal matrix should be the inverse transpose of the
     * modelview matrix (upper 3x3). GX provides it pre-computed.
     *   glUniformMatrix3fv(u_normalMatrix_loc, 1, GL_TRUE,
     *                       (GLfloat*)mtx);
     *
     * Only the upper 3x3 is used for normals.
     */
}

/* =========================================================================
 * 4. TEV / Blend / Alpha / Z / Fog State
 * ========================================================================= */

void GXSetTevOp(GXTevStageID stage, GXTevMode mode) {
    if ((u32)stage >= GX_MAX_TEVSTAGE) return;

    /* TODO: Phase 3c -- TEV preset to shader variant
     *
     * GXSetTevOp is a convenience function that sets both colorIn and
     * alphaIn for a TEV stage based on the preset mode:
     *
     * GX_MODULATE:
     *   color = tex * ras
     *   alpha = tex_a * ras_a
     *
     * GX_DECAL:
     *   color = lerp(ras, tex, tex_a)
     *   alpha = ras_a
     *
     * GX_BLEND:
     *   color = lerp(ras, tex, ras_a)  [approximation]
     *   alpha = tex_a * ras_a
     *
     * GX_REPLACE:
     *   color = tex
     *   alpha = tex_a
     *
     * GX_PASSCLR:
     *   color = ras
     *   alpha = ras_a
     *
     * Store the mode in g_tevState for the shader generator,
     * then mark the shader as dirty to trigger recompilation.
     */

    g_tevState.stages[stage].tevMode = mode;
    g_tevState.dirty = 1;
}

void GXSetTevColorIn(GXTevStageID stage,
                     GXTevColorArg a, GXTevColorArg b,
                     GXTevColorArg c, GXTevColorArg d) {
    if ((u32)stage >= GX_MAX_TEVSTAGE) return;

    /* TODO: Phase 3c -- Store TEV color input configuration
     *
     * Record the four color input sources for this TEV stage.
     * These will be read by the shader generator to emit GLSL code.
     */

    g_tevState.stages[stage].colorIn[0] = a;
    g_tevState.stages[stage].colorIn[1] = b;
    g_tevState.stages[stage].colorIn[2] = c;
    g_tevState.stages[stage].colorIn[3] = d;
    g_tevState.dirty = 1;
}

void GXSetTevAlphaIn(GXTevStageID stage,
                     GXTevAlphaArg a, GXTevAlphaArg b,
                     GXTevAlphaArg c, GXTevAlphaArg d) {
    if ((u32)stage >= GX_MAX_TEVSTAGE) return;

    /* TODO: Phase 3c -- Store TEV alpha input configuration */

    g_tevState.stages[stage].alphaIn[0] = a;
    g_tevState.stages[stage].alphaIn[1] = b;
    g_tevState.stages[stage].alphaIn[2] = c;
    g_tevState.stages[stage].alphaIn[3] = d;
    g_tevState.dirty = 1;
}

void GXSetTevColorOp(GXTevStageID stage, GXTevOp op,
                     GXTevBias bias, GXTevScale scale,
                     GXBool clamp, GXTevRegID out_reg) {
    if ((u32)stage >= GX_MAX_TEVSTAGE) return;

    /* TODO: Phase 3c -- Store TEV color operation
     *
     * The TEV color combiner computes:
     *   result = (d + mix(a, b, c)) * scale + bias
     * Where op selects ADD or SUB, and scale/bias are modifiers.
     *
     * clamp: if true, clamp result to [0,1]
     * out_reg: which register receives the result (PREV, REG0-2)
     */

    g_tevState.stages[stage].colorOp = op;
    g_tevState.stages[stage].colorBias = bias;
    g_tevState.stages[stage].colorScale = scale;
    g_tevState.stages[stage].colorClamp = clamp;
    g_tevState.stages[stage].colorOutReg = out_reg;
    g_tevState.dirty = 1;
}

void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op,
                     GXTevBias bias, GXTevScale scale,
                     GXBool clamp, GXTevRegID out_reg) {
    if ((u32)stage >= GX_MAX_TEVSTAGE) return;

    /* TODO: Phase 3c -- Store TEV alpha operation (same as colorOp) */

    g_tevState.stages[stage].alphaOp = op;
    g_tevState.stages[stage].alphaBias = bias;
    g_tevState.stages[stage].alphaScale = scale;
    g_tevState.stages[stage].alphaClamp = clamp;
    g_tevState.stages[stage].alphaOutReg = out_reg;
    g_tevState.dirty = 1;
}

void GXSetTevColor(GXTevRegID id, GXColor color) {
    if ((u32)id > 3) return;
    g_tevColorRegs[id] = color;

    /* TODO: Phase 3c -- Upload TEV color register to shader
     *
     * glUniform4f(u_tevColor_loc[id],
     *             color.r / 255.0f, color.g / 255.0f,
     *             color.b / 255.0f, color.a / 255.0f);
     */
}

void GXSetTevKColor(GXTevRegID id, GXColor color) {
    if ((u32)id > 3) return;
    g_tevKonstRegs[id] = color;

    /* TODO: Phase 3c -- Upload TEV konst color register to shader
     *
     * glUniform4f(u_tevKonst_loc[id],
     *             color.r / 255.0f, color.g / 255.0f,
     *             color.b / 255.0f, color.a / 255.0f);
     */
}

void GXSetNumTevStages(u8 nStages) {
    g_numTevStages = nStages;
    g_tevState.numStages = nStages;
    g_tevState.dirty = 1;

    /* TODO: Phase 3c -- This value is part of the shader cache key.
     * When the TEV state is flushed before a draw call, the number
     * of stages determines how many TEV stage loops to emit in GLSL.
     */
}

void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord,
                   GXTexMapID map, GXChannelID color) {
    if ((u32)stage >= GX_MAX_TEVSTAGE) return;

    /* TODO: Phase 3c -- Store TEV order for shader generation
     *
     * This tells each TEV stage:
     *   - Which texcoord to use for sampling (coord)
     *   - Which texture map slot to sample (map)
     *   - Which rasterized color channel to use (color)
     */

    g_tevState.stages[stage].texCoordId = coord;
    g_tevState.stages[stage].texMapId = map;
    g_tevState.stages[stage].channelId = color;
    g_tevState.dirty = 1;
}

void GXSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp op) {
    g_blendType = type;
    g_blendSrc = src_factor;
    g_blendDst = dst_factor;
    g_blendLogicOp = op;

    /* TODO: Phase 3e -- Translate to OpenGL blend state
     *
     * if (type == GX_BM_NONE) {
     *     glDisable(GL_BLEND);
     * } else if (type == GX_BM_BLEND) {
     *     glEnable(GL_BLEND);
     *     glBlendFunc(translateBlendFactor(src_factor),
     *                 translateBlendFactor(dst_factor));
     *     glBlendEquation(GL_FUNC_ADD);
     * } else if (type == GX_BM_SUBTRACT) {
     *     glEnable(GL_BLEND);
     *     glBlendFunc(GL_ONE, GL_ONE);
     *     glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
     * } else if (type == GX_BM_LOGIC) {
     *     // Logic ops are not available in GL core profile.
     *     // Approximate with blend or implement in fragment shader.
     * }
     *
     * GX blend factor -> GL blend factor mapping:
     *   GX_BL_ZERO        -> GL_ZERO
     *   GX_BL_ONE         -> GL_ONE
     *   GX_BL_SRCCLR      -> GL_SRC_COLOR
     *   GX_BL_INVSRCCLR   -> GL_ONE_MINUS_SRC_COLOR
     *   GX_BL_SRCALPHA    -> GL_SRC_ALPHA
     *   GX_BL_INVSRCALPHA -> GL_ONE_MINUS_SRC_ALPHA
     *   GX_BL_DSTALPHA    -> GL_DST_ALPHA
     *   GX_BL_INVDSTALPHA -> GL_ONE_MINUS_DST_ALPHA
     */
}

void GXSetAlphaCompare(GXCompare comp0, u8 ref0,
                       GXAlphaOp op,
                       GXCompare comp1, u8 ref1) {
    g_alphaComp0 = comp0;
    g_alphaRef0 = ref0;
    g_alphaOp = op;
    g_alphaComp1 = comp1;
    g_alphaRef1 = ref1;

    /* TODO: Phase 3e -- Alpha test in fragment shader
     *
     * OpenGL 3.3 core has no fixed-function alpha test.
     * Set uniforms for the fragment shader's alpha test logic:
     *
     * glUniform1i(u_alphaComp0_loc, comp0);
     * glUniform1f(u_alphaRef0_loc, ref0 / 255.0f);
     * glUniform1i(u_alphaOp_loc, op);
     * glUniform1i(u_alphaComp1_loc, comp1);
     * glUniform1f(u_alphaRef1_loc, ref1 / 255.0f);
     *
     * The fragment shader uses:
     *   if (!alphaTest(fragColor.a)) discard;
     */
}

void GXSetZMode(GXBool compare_enable, GXCompare func,
                GXBool update_enable) {
    g_zEnable = compare_enable;
    g_zFunc = func;
    g_zUpdate = update_enable;

    /* TODO: Phase 3e -- Translate to OpenGL depth state
     *
     * if (compare_enable) {
     *     glEnable(GL_DEPTH_TEST);
     *     glDepthFunc(translateCompare(func));
     *     // GX_NEVER->GL_NEVER, GX_LESS->GL_LESS, GX_LEQUAL->GL_LEQUAL, etc.
     * } else {
     *     glDisable(GL_DEPTH_TEST);
     * }
     * glDepthMask(update_enable ? GL_TRUE : GL_FALSE);
     */
}

void GXSetZCompLoc(GXBool before_tex) {
    (void)before_tex;

    /* TODO: Phase 3e -- Z comparison location
     *
     * before_tex=1: Depth test happens before texture lookup (early Z).
     *   In the fragment shader, discard based on alpha AFTER depth write.
     * before_tex=0: Depth test happens after texture lookup.
     *   In the fragment shader, discard based on alpha BEFORE depth write.
     *
     * This affects the ordering of the discard statement relative to
     * the depth write in the generated fragment shader.
     */
}

void GXSetFog(GXFogType type, f32 startz, f32 endz,
              f32 nearz, f32 farz, GXColor color) {
    g_fogType = type;
    g_fogStart = startz;
    g_fogEnd = endz;
    g_fogNear = nearz;
    g_fogFar = farz;
    g_fogColor = color;

    /* TODO: Phase 3e -- Set fog uniforms in fragment shader
     *
     * if (type == GX_FOG_NONE) {
     *     glUniform1i(u_fogEnable_loc, 0);
     * } else {
     *     glUniform1i(u_fogEnable_loc, 1);
     *     glUniform1i(u_fogType_loc, type);
     *     glUniform1f(u_fogStart_loc, startz);
     *     glUniform1f(u_fogEnd_loc, endz);
     *     glUniform4f(u_fogColor_loc,
     *                 color.r/255.0f, color.g/255.0f,
     *                 color.b/255.0f, color.a/255.0f);
     * }
     *
     * Fragment shader fog calculation:
     *   float fogFactor = clamp((fogEnd - viewZ) / (fogEnd - fogStart), 0, 1);
     *   fragColor.rgb = mix(fogColor.rgb, fragColor.rgb, fogFactor);
     */
}

void GXSetCullMode(GXCullMode mode) {
    g_cullMode = mode;

    /* TODO: Phase 3a -- Translate to OpenGL cull state
     *
     * if (mode == GX_CULL_NONE) {
     *     glDisable(GL_CULL_FACE);
     * } else {
     *     glEnable(GL_CULL_FACE);
     *     // GCN front face is CW; GL default is CCW.
     *     // With glFrontFace(GL_CW):
     *     if (mode == GX_CULL_FRONT)
     *         glCullFace(GL_FRONT);
     *     else if (mode == GX_CULL_BACK)
     *         glCullFace(GL_BACK);
     *     else // GX_CULL_ALL
     *         glCullFace(GL_FRONT_AND_BACK);
     * }
     */
}

void GXSetChanCtrl(GXChannelID chan, GXBool enable,
                   GXColorSrc amb_src, GXColorSrc mat_src,
                   u32 light_mask, GXDiffuseFn diff_fn,
                   GXAttnFn attn_fn) {
    (void)chan; (void)enable; (void)amb_src; (void)mat_src;
    (void)light_mask; (void)diff_fn; (void)attn_fn;

    /* TODO: Phase 3e -- Configure per-channel lighting in shader
     *
     * Store the channel control state. This determines whether the
     * vertex shader computes lighting for COLOR0/COLOR1 or passes
     * through vertex colors / material colors.
     *
     * Uniforms to set:
     *   u_chanCtrlEnable[chan] = enable
     *   u_chanAmbSrc[chan] = amb_src  (REG or VTX)
     *   u_chanMatSrc[chan] = mat_src  (REG or VTX)
     *   u_chanLightMask[chan] = light_mask
     *   u_chanDiffFn[chan] = diff_fn
     *   u_chanAttnFn[chan] = attn_fn
     */
}

void GXSetChanAmbColor(GXChannelID chan, GXColor color) {
    if (chan < 2) g_chanAmbColor[chan] = color;

    /* TODO: Phase 3e -- Upload ambient color uniform
     *
     * glUniform4f(u_ambientColor_loc[chan],
     *             color.r/255.0f, color.g/255.0f,
     *             color.b/255.0f, color.a/255.0f);
     */
}

void GXSetChanMatColor(GXChannelID chan, GXColor color) {
    if (chan < 2) g_chanMatColor[chan] = color;

    /* TODO: Phase 3e -- Upload material color uniform
     *
     * glUniform4f(u_matColor_loc[chan],
     *             color.r/255.0f, color.g/255.0f,
     *             color.b/255.0f, color.a/255.0f);
     */
}

/* =========================================================================
 * 5. Texture State
 * ========================================================================= */

void GXInitTexObj(GXTexObj* obj, void* image,
                  u16 width, u16 height, GXTexFmt format,
                  GXTexWrapMode wrap_s, GXTexWrapMode wrap_t,
                  GXBool mipmap) {
    (void)obj; (void)image; (void)width; (void)height;
    (void)format; (void)wrap_s; (void)wrap_t; (void)mipmap;

    /* TODO: Phase 3d -- Create OpenGL texture
     *
     * 1. Generate a GL texture name: glGenTextures(1, &texId)
     * 2. Store texId inside the GXTexObj (reuse the opaque 32 bytes)
     * 3. Decode the GCN texture data from 'image':
     *    - De-tile/de-swizzle from GCN tiled format to linear
     *    - Convert from GXTexFmt to RGBA8 using gx_texture_decode()
     * 4. Upload to GL:
     *    glBindTexture(GL_TEXTURE_2D, texId);
     *    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
     *                 GL_RGBA, GL_UNSIGNED_BYTE, decodedData);
     * 5. Set wrap modes:
     *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
     *                    translateWrapMode(wrap_s));
     *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
     *                    translateWrapMode(wrap_t));
     * 6. Set filter modes (from mipmap flag):
     *    if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);
     */
}

void GXInitTlutObj(GXTlutObj* obj, void* lut,
                   GXTlutFmt fmt, u16 n_entries) {
    (void)obj; (void)lut; (void)fmt; (void)n_entries;

    /* TODO: Phase 3d -- Decode TLUT on CPU
     *
     * There is no GL equivalent for hardware palette lookups.
     * Store the palette data pointer inside the GXTlutObj so that
     * GXInitTexObj can use it for CI4/CI8/CI14x2 texture decoding.
     *
     * The palette entries are in GXTlutFmt format:
     *   GX_TL_IA8    -> 16-bit intensity+alpha
     *   GX_TL_RGB565 -> 16-bit RGB
     *   GX_TL_RGB5A3 -> 16-bit RGB with 1-bit alpha mode flag
     */
}

void GXLoadTexObj(GXTexObj* obj, GXTexMapID id) {
    (void)obj; (void)id;

    /* TODO: Phase 3d -- Bind texture to a TEV stage
     *
     * u32 texId = extractGLTexId(obj);
     * glActiveTexture(GL_TEXTURE0 + id);
     * glBindTexture(GL_TEXTURE_2D, texId);
     * glUniform1i(u_tex_loc[id], id);
     */
}

void GXInvalidateTexAll(void) {
    /* No-op on PC -- texture cache is always coherent. */
}

void GXCopyTex(void* dest, GXBool clear) {
    (void)dest; (void)clear;

    /* TODO: Phase 3f -- EFB copy to texture (render-to-texture)
     *
     * This is used by GStextureUploadFromBuffer for render-to-texture
     * effects (blur, distortion, aura).
     *
     * Implementation:
     * 1. Bind the FBO that was set up as the render target
     * 2. Use glCopyTexSubImage2D or glBlitFramebuffer to copy
     *    the FBO color attachment to the destination texture
     * 3. If clear==GX_TRUE, clear the FBO after copy
     *
     * The 'dest' pointer on GCN points to main RAM; on PC, map it
     * to the corresponding GL texture ID.
     */
}

void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
    (void)left; (void)top; (void)wd; (void)ht;

    /* TODO: Phase 3f -- Configure FBO blit source region
     *
     * Store the source rectangle for the next GXCopyTex call.
     * Used by GStextureUploadFromBuffer to specify which part
     * of the EFB to copy.
     */
}

void GXPixModeSync(void) {
    /* TODO: Phase 3f -- Pipeline synchronization
     *
     * glFinish(); // or glMemoryBarrier if using compute shaders
     *
     * This is rarely needed on PC since GL driver handles sync.
     * Only call glFinish() if we see rendering artifacts from
     * missing synchronization.
     */
}

/* =========================================================================
 * 6. Lighting
 * ========================================================================= */

void GXInitLightObj(GXLightObj* obj, GXColor color,
                    f32 px, f32 py, f32 pz) {
    (void)obj; (void)color; (void)px; (void)py; (void)pz;

    /* TODO: Phase 3e -- Initialize light parameters
     *
     * Store position and color in the GXLightObj structure.
     * The 64-byte opaque struct can be repurposed to hold:
     *   - vec3 position (12 bytes)
     *   - vec3 direction (12 bytes)
     *   - vec4 color (16 bytes)
     *   - attenuation params (12 bytes)
     *   - spot params (12 bytes)
     */
}

void GXInitLightDir(GXLightObj* obj, f32 nx, f32 ny, f32 nz) {
    (void)obj; (void)nx; (void)ny; (void)nz;
    /* TODO: Phase 3e -- Store light direction in GXLightObj */
}

void GXInitLightDistAttn(GXLightObj* obj, f32 ref_dist,
                         f32 ref_brightness, GXDiffuseFn fn) {
    (void)obj; (void)ref_dist; (void)ref_brightness; (void)fn;
    /* TODO: Phase 3e -- Store distance attenuation parameters */
}

void GXInitLightSpot(GXLightObj* obj, f32 cutoff, u32 spot_fn) {
    (void)obj; (void)cutoff; (void)spot_fn;
    /* TODO: Phase 3e -- Store spotlight parameters */
}

void GXInitSpecularDir(GXLightObj* obj, f32 nx, f32 ny, f32 nz) {
    (void)obj; (void)nx; (void)ny; (void)nz;
    /* TODO: Phase 3e -- Store specular direction */
}

void GXInitLightShininess(GXLightObj* obj, f32 shininess) {
    (void)obj; (void)shininess;
    /* TODO: Phase 3e -- Store shininess exponent */
}

void GXLoadLightObj(GXLightObj* obj, GXLightID id) {
    (void)obj; (void)id;

    /* TODO: Phase 3e -- Upload light to shader
     *
     * Determine the light index from id (log2 of the bitmask):
     *   int idx = __builtin_ctz(id);
     *
     * Upload the light struct from GXLightObj to the uniform:
     *   glUniform3f(u_lights_pos_loc[idx], pos.x, pos.y, pos.z);
     *   glUniform3f(u_lights_dir_loc[idx], dir.x, dir.y, dir.z);
     *   glUniform4f(u_lights_color_loc[idx], r, g, b, a);
     *   glUniform1f(u_lights_cosAtten_loc[idx], cosAtten);
     *   glUniform1f(u_lights_distAtten_loc[idx], distAtten);
     */
}

/* =========================================================================
 * 7. Draw Commands
 * ========================================================================= */

void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts) {
    (void)vtxfmt;
    g_immPrimType = type;
    g_immExpectedVerts = nverts;
    g_immVertexCount = 0;

    /* TODO: Phase 3b -- Begin immediate-mode vertex accumulation
     *
     * Before drawing, flush the current TEV/blend/Z state:
     * 1. If g_tevState.dirty, compile or look up the GLSL shader
     *    from the TEV state cache (gx_tev_compile)
     * 2. Bind the shader program
     * 3. Set all uniforms (matrices, TEV regs, blend/alpha/Z/fog)
     * 4. Begin writing vertices into the dynamic VBO
     *
     * Reset the vertex write pointer to the start of the buffer.
     */
}

void GXEnd(void) {
    /* TODO: Phase 3b -- Flush immediate-mode vertices to OpenGL
     *
     * 1. Upload g_immVertices[0..g_immVertexCount-1] to the dynamic VBO:
     *    glBindBuffer(GL_ARRAY_BUFFER, g_immVBO);
     *    glBufferSubData(GL_ARRAY_BUFFER, 0,
     *                    g_immVertexCount * sizeof(GXImmVertex),
     *                    g_immVertices);
     *
     * 2. Translate the GX primitive type to GL:
     *    GLenum glPrim;
     *    switch (g_immPrimType) {
     *        case GX_TRIANGLES:     glPrim = GL_TRIANGLES; break;
     *        case GX_TRIANGLESTRIP: glPrim = GL_TRIANGLE_STRIP; break;
     *        case GX_TRIANGLEFAN:   glPrim = GL_TRIANGLE_FAN; break;
     *        case GX_LINES:         glPrim = GL_LINES; break;
     *        case GX_LINESTRIP:     glPrim = GL_LINE_STRIP; break;
     *        case GX_POINTS:        glPrim = GL_POINTS; break;
     *        case GX_QUADS:
     *            // Split each quad into 2 triangles
     *            convertQuadsToTriangles();
     *            glPrim = GL_TRIANGLES;
     *            break;
     *    }
     *
     * 3. Draw:
     *    glDrawArrays(glPrim, 0, g_immVertexCount);
     */

    g_immVertexCount = 0;
    g_immExpectedVerts = 0;
}

void GXPosition3f32(f32 x, f32 y, f32 z) {
    if (g_immVertexCount >= GX_IMM_VTX_MAX) return;

    /* TODO: Phase 3b -- Write position to immediate-mode buffer */
    g_immVertices[g_immVertexCount].pos[0] = x;
    g_immVertices[g_immVertexCount].pos[1] = y;
    g_immVertices[g_immVertexCount].pos[2] = z;
}

void GXColor4u8(u8 r, u8 g, u8 b, u8 a) {
    if (g_immVertexCount >= GX_IMM_VTX_MAX) return;

    /* TODO: Phase 3b -- Write color to immediate-mode buffer */
    g_immVertices[g_immVertexCount].color[0] = r;
    g_immVertices[g_immVertexCount].color[1] = g;
    g_immVertices[g_immVertexCount].color[2] = b;
    g_immVertices[g_immVertexCount].color[3] = a;
}

void GXTexCoord2f32(f32 s, f32 t) {
    if (g_immVertexCount >= GX_IMM_VTX_MAX) return;

    /* TODO: Phase 3b -- Write texcoord to immediate-mode buffer
     * After writing the texcoord, advance the vertex counter since
     * texcoord is typically the last attribute submitted per vertex.
     */
    g_immVertices[g_immVertexCount].texcoord[0] = s;
    g_immVertices[g_immVertexCount].texcoord[1] = t;
    g_immVertexCount++;
}

void GXCallDisplayList(void* list, u32 nbytes) {
    (void)list; (void)nbytes;

    /* TODO: Phase 3b -- Execute pre-built display list
     *
     * On GCN, display lists are pre-compiled GPU command buffers.
     * For the PC port, they have been pre-parsed at load time
     * (in HSD_PObjLoadDesc) into VBO/VAO objects.
     *
     * Look up the pre-built VBO/VAO for this display list pointer:
     *   GLDisplayList* dl = gx_displaylist_find(list);
     *   if (dl) {
     *       glBindVertexArray(dl->vao);
     *       glDrawArrays(dl->primType, 0, dl->vertexCount);
     *   }
     *
     * For display lists not yet pre-parsed, fall back to runtime
     * parsing (slower, but handles edge cases).
     */
}

/* =========================================================================
 * 8. Framebuffer / Copy
 * ========================================================================= */

void GXCopyDisp(void* dest, GXBool clear) {
    (void)dest; (void)clear;

    /* TODO: Phase 3f -- Swap framebuffers
     *
     * This is the end-of-frame operation:
     *   glfwSwapBuffers(g_window);
     *
     * If clear==GX_TRUE, clear the back buffer after swap:
     *   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     *
     * The 'dest' parameter is the XFB address on GCN; ignored on PC.
     */
}

void GXSetCopyFilter(GXBool aa, u8 sample_pattern[12][2],
                     GXBool vf, u8 vfilter[7]) {
    (void)aa; (void)sample_pattern; (void)vf; (void)vfilter;

    /* TODO: Phase 3h -- Multisampling is handled differently on PC.
     *
     * If MSAA is desired, request it in the GLFW window hints:
     *   glfwWindowHint(GLFW_SAMPLES, 4);
     * And enable:
     *   glEnable(GL_MULTISAMPLE);
     *
     * The GCN copy filter parameters are ignored.
     */
}

void GXSetDispCopyDst(u16 wd) {
    (void)wd;
    /* No-op on PC -- display copy stride is handled by the window system. */
}

void GXSetNumChans(u8 nChans) {
    (void)nChans;

    /* TODO: Phase 3e -- Configure number of active color channels
     *
     * Set a uniform or shader variant key to control how many
     * color channels are computed in the vertex shader.
     * Colosseum typically uses 1 channel (COLOR0A0).
     */
}

void GXSetNumTexGens(u8 nTexGens) {
    (void)nTexGens;

    /* TODO: Phase 3c -- Configure texture coordinate generation
     *
     * Set a uniform or shader variant key for the number of
     * active texture coordinate generators. This affects how
     * many texcoord varyings are passed to the fragment shader.
     */
}
