/**
 * @file gx_tev.c
 * @brief TEV combiner to GLSL shader translation -- stub implementation.
 *
 * Translates GCN TEV combiner configurations into GLSL 330 fragment shaders.
 * Shaders are generated at runtime and cached by a hash of the TEV state.
 *
 * The approach mirrors Dolphin Emulator's shader generation and the
 * encounter/aurora library from the Metroid Prime decomp. Since Colosseum
 * uses a limited set of TEV configurations (estimated 20-40 unique
 * combinations), most shaders will be compiled on first encounter and
 * then served from cache for the rest of the session.
 *
 * References:
 *   - docs/pc_port_design.md Section 2 (TEV Combiner Translation)
 *   - docs/pc_port_design.md Section 2.4 (Preset Optimization)
 *   - gx_tev.h for data structures and API
 *
 * Phase 3 PC port scaffolding -- skeleton only.
 */

#include "gx_tev.h"

#include <stdio.h>
#include <string.h>

/* TODO: Include OpenGL headers when build system is ready
 * #include <glad/glad.h>
 */

/* =========================================================================
 * Shader cache
 * ========================================================================= */

static GXTevShaderEntry g_shaderCache[TEV_SHADER_CACHE_MAX];
static u32 g_shaderCacheCount = 0;
static u32 g_cacheHits = 0;
static u32 g_cacheMisses = 0;

/* =========================================================================
 * Standard vertex shader source (shared across all TEV configurations)
 *
 * This is the template from pc_port_design.md Section 8.4, parameterized
 * by the number of active texcoord generators and color channels.
 * ========================================================================= */

static const char* VERTEX_SHADER_TEMPLATE =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "layout(location = 2) in vec4 a_color0;\n"
    "layout(location = 3) in vec4 a_color1;\n"
    "layout(location = 4) in vec2 a_texcoord0;\n"
    "layout(location = 5) in vec2 a_texcoord1;\n"
    "layout(location = 6) in vec2 a_texcoord2;\n"
    "layout(location = 7) in vec2 a_texcoord3;\n"
    "layout(location = 8) in vec2 a_texcoord4;\n"
    "layout(location = 9) in vec2 a_texcoord5;\n"
    "layout(location = 10) in vec2 a_texcoord6;\n"
    "layout(location = 11) in vec2 a_texcoord7;\n"
    "\n"
    "uniform mat4 u_projMatrix;\n"
    "uniform mat4 u_modelViewMatrix;\n"
    "uniform mat3 u_normalMatrix;\n"
    "\n"
    "out vec4 v_color0;\n"
    "out vec4 v_color1;\n"
    "out vec2 v_texcoord[8];\n"
    "out vec3 v_normal;\n"
    "out vec3 v_viewPos;\n"
    "\n"
    "void main() {\n"
    "    vec4 viewPos = u_modelViewMatrix * vec4(a_position, 1.0);\n"
    "    gl_Position = u_projMatrix * viewPos;\n"
    "    v_viewPos = viewPos.xyz;\n"
    "    v_normal = u_normalMatrix * a_normal;\n"
    "    v_color0 = a_color0;\n"
    "    v_color1 = a_color1;\n"
    "    v_texcoord[0] = a_texcoord0;\n"
    "    v_texcoord[1] = a_texcoord1;\n"
    "    v_texcoord[2] = a_texcoord2;\n"
    "    v_texcoord[3] = a_texcoord3;\n"
    "    v_texcoord[4] = a_texcoord4;\n"
    "    v_texcoord[5] = a_texcoord5;\n"
    "    v_texcoord[6] = a_texcoord6;\n"
    "    v_texcoord[7] = a_texcoord7;\n"
    "}\n";

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

/**
 * Emit GLSL code for a TEV color input source selection.
 * Returns a GLSL expression string for the given GXTevColorArg.
 */
static const char* tev_color_input_expr(u8 arg, u32 stage) {
    (void)stage;

    /* TODO: Phase 3c -- Map GXTevColorArg to GLSL expression
     *
     * GX_CC_CPREV  -> "prev.rgb"
     * GX_CC_APREV  -> "vec3(prev.a)"
     * GX_CC_C0     -> "u_tevColor[0].rgb"
     * GX_CC_A0     -> "vec3(u_tevColor[0].a)"
     * GX_CC_C1     -> "u_tevColor[1].rgb"
     * GX_CC_A1     -> "vec3(u_tevColor[1].a)"
     * GX_CC_C2     -> "u_tevColor[2].rgb"
     * GX_CC_A2     -> "vec3(u_tevColor[2].a)"
     * GX_CC_TEXC   -> "texSampleN.rgb"  (N = stage's texture)
     * GX_CC_TEXA   -> "vec3(texSampleN.a)"
     * GX_CC_RASC   -> "v_color0.rgb" (or v_color1 based on channel)
     * GX_CC_RASA   -> "vec3(v_color0.a)"
     * GX_CC_ONE    -> "vec3(1.0)"
     * GX_CC_HALF   -> "vec3(0.5)"
     * GX_CC_KONST  -> "u_tevKonst[ksel].rgb" (ksel per-stage config)
     * GX_CC_ZERO   -> "vec3(0.0)"
     */

    switch (arg) {
        case GX_CC_ZERO: return "vec3(0.0)";
        case GX_CC_ONE:  return "vec3(1.0)";
        case GX_CC_HALF: return "vec3(0.5)";
        default:         return "vec3(0.0)"; /* placeholder */
    }
}

/**
 * Emit GLSL code for a TEV alpha input source selection.
 */
static const char* tev_alpha_input_expr(u8 arg, u32 stage) {
    (void)stage;

    /* TODO: Phase 3c -- Map GXTevAlphaArg to GLSL expression
     *
     * GX_CA_APREV  -> "prev.a"
     * GX_CA_A0     -> "u_tevColor[0].a"
     * GX_CA_A1     -> "u_tevColor[1].a"
     * GX_CA_A2     -> "u_tevColor[2].a"
     * GX_CA_TEXA   -> "texSampleN.a"
     * GX_CA_RASA   -> "v_color0.a"
     * GX_CA_KONST  -> "u_tevKonst[ksel].a"
     * GX_CA_ZERO   -> "0.0"
     */

    switch (arg) {
        case GX_CA_ZERO: return "0.0";
        default:         return "0.0"; /* placeholder */
    }
}

/**
 * Emit GLSL code for the TEV combiner math operation.
 */
static void tev_emit_combiner(char* buf, u32 bufSize,
                              const char* a, const char* b,
                              const char* c, const char* d,
                              u8 op, u8 bias, u8 scale, u8 clamp,
                              const char* swizzle) {
    (void)buf; (void)bufSize;
    (void)a; (void)b; (void)c; (void)d;
    (void)op; (void)bias; (void)scale; (void)clamp; (void)swizzle;

    /* TODO: Phase 3c -- Generate the combiner equation in GLSL
     *
     * The TEV equation is:
     *   result = (d (+/-) mix(a, b, c)) * scaleVal + biasVal
     *
     * For the standard GX_TEV_ADD operation:
     *   "result.{swizzle} = (({d}) + mix({a}, {b}, {c})) * {scaleVal} + {biasVal};\n"
     *
     * For GX_TEV_SUB:
     *   "result.{swizzle} = (({d}) - mix({a}, {b}, {c})) * {scaleVal} + {biasVal};\n"
     *
     * Scale values: 1x, 2x, 4x, /2
     * Bias values:  0, +0.5, -0.5
     *
     * If clamp: "result.{swizzle} = clamp(result.{swizzle}, 0.0, 1.0);\n"
     *
     * Comparison operations (GX_TEV_COMP_*) need separate handling:
     *   GX_TEV_COMP_R8_GT: "result.{sw} = ({d}) + (({a}.r > {b}.r) ? {c} : vec3(0.0));\n"
     *   etc.
     */
}

/* =========================================================================
 * Public API implementation
 * ========================================================================= */

void gx_tev_init(void) {
    memset(g_shaderCache, 0, sizeof(g_shaderCache));
    g_shaderCacheCount = 0;
    g_cacheHits = 0;
    g_cacheMisses = 0;

    /* TODO: Phase 3c -- Compile the default passthrough shader
     *
     * The default shader (PASSCLR mode, 1 TEV stage) just passes
     * through the rasterized vertex color:
     *
     *   fragColor = v_color0;
     *
     * Pre-compile this so that early rendering works even before
     * the full TEV generator is complete.
     */

    printf("[gx_tev] TEV shader cache initialized (max %d entries)\n",
           TEV_SHADER_CACHE_MAX);
}

void gx_tev_shutdown(void) {
    /* TODO: Phase 3c -- Delete all cached GL shader programs
     *
     * for (u32 i = 0; i < g_shaderCacheCount; i++) {
     *     if (g_shaderCache[i].valid) {
     *         glDeleteProgram(g_shaderCache[i].glProgram);
     *     }
     * }
     */

    memset(g_shaderCache, 0, sizeof(g_shaderCache));
    g_shaderCacheCount = 0;
}

u32 gx_tev_hash(const GXTevState* state) {
    /* TODO: Phase 3c -- Implement FNV-1a hash over TEV state
     *
     * Hash the relevant fields:
     *   - numStages
     *   - For each active stage: colorIn[], alphaIn[], colorOp, alphaOp,
     *     colorScale, colorBias, alphaScale, alphaBias, texMapId, channelId
     *   - alphaComp0, alphaComp1, alphaRef0, alphaRef1, alphaOp
     *   - fogEnable
     *
     * FNV-1a:
     *   u32 hash = 0x811c9dc5;
     *   for each byte b in state:
     *       hash ^= b;
     *       hash *= 0x01000193;
     *   return hash;
     */

    /* Placeholder: hash just the first few bytes */
    const u8* data = (const u8*)state;
    u32 hash = 0x811c9dc5u;
    u32 i;
    u32 len = sizeof(GXTevState);
    for (i = 0; i < len; i++) {
        hash ^= data[i];
        hash *= 0x01000193u;
    }
    return hash;
}

GXTevShaderEntry* gx_tev_compile(const GXTevState* state) {
    u32 hash = gx_tev_hash(state);
    u32 i;

    /* Look up in cache */
    for (i = 0; i < g_shaderCacheCount; i++) {
        if (g_shaderCache[i].valid && g_shaderCache[i].stateHash == hash) {
            g_cacheHits++;
            return &g_shaderCache[i];
        }
    }

    /* Cache miss -- generate and compile a new shader */
    g_cacheMisses++;

    if (g_shaderCacheCount >= TEV_SHADER_CACHE_MAX) {
        printf("[gx_tev] ERROR: Shader cache full! Cannot compile new shader.\n");
        return (GXTevShaderEntry*)0;
    }

    /* TODO: Phase 3c -- Generate and compile the shader
     *
     * 1. Generate fragment shader source:
     *    char fragSrc[TEV_SHADER_SRC_MAX];
     *    gx_tev_generate_fragment_shader(state, fragSrc, sizeof(fragSrc));
     *
     * 2. Compile fragment shader:
     *    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
     *    glShaderSource(fs, 1, &fragSrc, NULL);
     *    glCompileShader(fs);
     *    // Check compile status, print errors
     *
     * 3. Compile vertex shader (using the static template):
     *    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
     *    glShaderSource(vs, 1, &VERTEX_SHADER_TEMPLATE, NULL);
     *    glCompileShader(vs);
     *
     * 4. Link program:
     *    GLuint prog = glCreateProgram();
     *    glAttachShader(prog, vs);
     *    glAttachShader(prog, fs);
     *    glLinkProgram(prog);
     *    // Check link status
     *
     * 5. Cache uniform locations:
     *    entry->loc_projMatrix = glGetUniformLocation(prog, "u_projMatrix");
     *    entry->loc_modelViewMatrix = glGetUniformLocation(prog, "u_modelViewMatrix");
     *    entry->loc_normalMatrix = glGetUniformLocation(prog, "u_normalMatrix");
     *    for (int t = 0; t < 8; t++) {
     *        char name[16]; sprintf(name, "u_tex[%d]", t);
     *        entry->loc_tex[t] = glGetUniformLocation(prog, name);
     *    }
     *    // ... etc for all uniforms
     *
     * 6. Store in cache:
     *    entry->glProgram = prog;
     *    entry->stateHash = hash;
     *    entry->valid = 1;
     */

    GXTevShaderEntry* entry = &g_shaderCache[g_shaderCacheCount];
    memset(entry, 0, sizeof(*entry));
    entry->stateHash = hash;
    entry->glProgram = 0; /* placeholder -- filled in by GL compile */
    entry->valid = 1;

    /* Set uniform locations to -1 (invalid) until actual GL compile */
    entry->loc_projMatrix = -1;
    entry->loc_modelViewMatrix = -1;
    entry->loc_normalMatrix = -1;
    for (i = 0; i < 8; i++) {
        entry->loc_tex[i] = -1;
        entry->loc_texSwizzle[i] = -1;
    }
    for (i = 0; i < 4; i++) {
        entry->loc_tevColor[i] = -1;
        entry->loc_tevKonst[i] = -1;
    }

    g_shaderCacheCount++;
    return entry;
}

void gx_tev_bind(const GXTevShaderEntry* entry) {
    if (!entry || !entry->valid) return;

    /* TODO: Phase 3c -- Bind shader program and set uniforms
     *
     * glUseProgram(entry->glProgram);
     *
     * // Upload matrices
     * if (entry->loc_projMatrix >= 0)
     *     glUniformMatrix4fv(entry->loc_projMatrix, 1, GL_TRUE,
     *                        (GLfloat*)g_projMatrix);
     *
     * if (entry->loc_modelViewMatrix >= 0)
     *     glUniformMatrix4fv(entry->loc_modelViewMatrix, 1, GL_TRUE,
     *                        (GLfloat*)g_posMtx[0]);
     *
     * // Upload TEV color registers
     * for (int i = 0; i < 4; i++) {
     *     if (entry->loc_tevColor[i] >= 0) {
     *         glUniform4f(entry->loc_tevColor[i],
     *                     g_tevColorRegs[i].r / 255.0f, ...);
     *     }
     * }
     *
     * // Upload texture sampler indices
     * for (int i = 0; i < 8; i++) {
     *     if (entry->loc_tex[i] >= 0)
     *         glUniform1i(entry->loc_tex[i], i);
     * }
     *
     * // Upload alpha test uniforms
     * // Upload fog uniforms
     */
}

s32 gx_tev_generate_fragment_shader(const GXTevState* state,
                                    char* outBuf, u32 bufSize) {
    if (!state || !outBuf || bufSize == 0) return -1;

    /* TODO: Phase 3c -- Generate GLSL 330 fragment shader
     *
     * The generated shader follows this structure:
     *
     * #version 330 core
     *
     * // Uniforms
     * uniform sampler2D u_tex[8];
     * uniform vec4 u_tevColor[4];
     * uniform vec4 u_tevKonst[4];
     * uniform int u_texSwizzle[8];
     * uniform int u_alphaComp0, u_alphaComp1;
     * uniform float u_alphaRef0, u_alphaRef1;
     * uniform int u_alphaOp;
     * uniform int u_fogEnable;
     * uniform float u_fogStart, u_fogEnd;
     * uniform vec4 u_fogColor;
     *
     * // Inputs from vertex shader
     * in vec4 v_color0;
     * in vec4 v_color1;
     * in vec2 v_texcoord[8];
     * in vec3 v_viewPos;
     *
     * out vec4 fragColor;
     *
     * // Texture swizzle helper (for I/IA/A formats)
     * vec4 applySwizzle(vec4 raw, int mode) {
     *     if (mode == 1) return vec4(raw.r, raw.r, raw.r, raw.r); // I
     *     if (mode == 2) return vec4(raw.r, raw.r, raw.r, raw.g); // IA
     *     if (mode == 3) return vec4(1.0, 1.0, 1.0, raw.r);       // A
     *     return raw; // RGBA
     * }
     *
     * // Alpha test helper
     * bool compareFunc(int comp, float val, float ref) {
     *     if (comp == 0) return false;        // NEVER
     *     if (comp == 1) return val < ref;    // LESS
     *     if (comp == 2) return val == ref;   // EQUAL
     *     if (comp == 3) return val <= ref;   // LEQUAL
     *     if (comp == 4) return val > ref;    // GREATER
     *     if (comp == 5) return val != ref;   // NEQUAL
     *     if (comp == 6) return val >= ref;   // GEQUAL
     *     return true;                         // ALWAYS (7)
     * }
     *
     * void main() {
     *     vec4 prev = v_color0;  // CPREV starts as rasterized color
     *     vec4 reg0 = u_tevColor[0];
     *     vec4 reg1 = u_tevColor[1];
     *     vec4 reg2 = u_tevColor[2];
     *
     *     // --- TEV Stage 0 ---
     *     [emit texture sample if stage uses a texture]
     *     [emit color combiner: prev.rgb = ...]
     *     [emit alpha combiner: prev.a = ...]
     *
     *     // --- TEV Stage 1..N-1 ---
     *     [same pattern]
     *
     *     // --- Alpha test ---
     *     bool pass0 = compareFunc(u_alphaComp0, prev.a, u_alphaRef0);
     *     bool pass1 = compareFunc(u_alphaComp1, prev.a, u_alphaRef1);
     *     bool alphaPass;
     *     if (u_alphaOp == 0) alphaPass = pass0 && pass1;  // AND
     *     else if (u_alphaOp == 1) alphaPass = pass0 || pass1;  // OR
     *     else alphaPass = pass0 ^^ pass1;  // XOR
     *     if (!alphaPass) discard;
     *
     *     // --- Fog ---
     *     if (u_fogEnable != 0) {
     *         float fogFactor = clamp((u_fogEnd - length(v_viewPos))
     *                                / (u_fogEnd - u_fogStart), 0.0, 1.0);
     *         prev.rgb = mix(u_fogColor.rgb, prev.rgb, fogFactor);
     *     }
     *
     *     fragColor = prev;
     * }
     *
     * For each TEV stage, call tev_color_input_expr / tev_alpha_input_expr
     * for each input, then tev_emit_combiner for the operation.
     */

    /* Placeholder: emit a minimal passthrough shader */
    s32 len = snprintf(outBuf, bufSize,
        "#version 330 core\n"
        "\n"
        "uniform sampler2D u_tex[8];\n"
        "uniform vec4 u_tevColor[4];\n"
        "uniform vec4 u_tevKonst[4];\n"
        "\n"
        "in vec4 v_color0;\n"
        "in vec4 v_color1;\n"
        "in vec2 v_texcoord[8];\n"
        "in vec3 v_viewPos;\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main() {\n"
        "    /* TODO: Generate TEV combiner stages here (%d stages) */\n"
        "    fragColor = v_color0;\n"
        "}\n",
        state->numStages
    );

    return len;
}

s32 gx_tev_generate_vertex_shader(u8 numTexGens, u8 numChans,
                                  char* outBuf, u32 bufSize) {
    (void)numTexGens; (void)numChans;

    /* TODO: Phase 3c -- Generate parameterized vertex shader
     *
     * For now, use the static template. In the future, generate
     * vertex shaders that only pass through the texcoords and
     * color channels that are actually used (optimization).
     */

    s32 len = snprintf(outBuf, bufSize, "%s", VERTEX_SHADER_TEMPLATE);
    return len;
}

void gx_tev_get_cache_stats(u32* outHits, u32* outMisses, u32* outTotal) {
    if (outHits) *outHits = g_cacheHits;
    if (outMisses) *outMisses = g_cacheMisses;
    if (outTotal) *outTotal = g_shaderCacheCount;
}
