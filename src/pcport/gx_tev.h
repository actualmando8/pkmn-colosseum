/**
 * @file gx_tev.h
 * @brief TEV combiner to GLSL shader translation.
 *
 * The GCN TEV (Texture Environment) is a fixed-function, multi-stage texture
 * combiner with up to 16 stages. Each stage has separate color and alpha
 * equations of the form:
 *
 *   output = (d + lerp(a, b, c)) * scale + bias
 *
 * This module takes a snapshot of the current TEV state and compiles it into
 * a GLSL 330 fragment shader string. Compiled shaders are cached by a hash
 * of the TEV state so that identical configurations reuse the same program.
 *
 * References:
 *   - docs/pc_port_design.md Section 2 (TEV Combiner Translation)
 *   - docs/pc_port_design.md Section 2.3 (GLSL Translation Strategy)
 *   - include/hsd/hsd_tobj.h (TEX_COLORMAP_*, TEX_ALPHAMAP_* modes)
 *   - include/hsd/hsd_mobj.h (HSD_MObj TEV setup)
 *
 * Phase 3 PC port scaffolding -- skeleton only.
 */
#ifndef PCPORT_GX_TEV_H
#define PCPORT_GX_TEV_H

#include "gx_shim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================================
 * Constants
 * ========================================================================= */

/** Maximum number of TEV stages (GCN hardware limit) */
#define GX_TEV_MAX_STAGES   16

/** Maximum number of cached shader programs */
#define TEV_SHADER_CACHE_MAX 256

/** Maximum length of a generated GLSL fragment shader source string */
#define TEV_SHADER_SRC_MAX  8192

/* =========================================================================
 * TEV stage state -- per-stage configuration
 * ========================================================================= */

typedef struct GXTevStageState {
    /* Color combiner inputs (a, b, c, d) */
    u8 colorIn[4];      /* GXTevColorArg values */
    /* Alpha combiner inputs (a, b, c, d) */
    u8 alphaIn[4];      /* GXTevAlphaArg values */

    /* Color combiner operation */
    u8 colorOp;          /* GXTevOp */
    u8 colorBias;        /* GXTevBias */
    u8 colorScale;       /* GXTevScale */
    u8 colorClamp;       /* GXBool */
    u8 colorOutReg;      /* GXTevRegID */

    /* Alpha combiner operation */
    u8 alphaOp;          /* GXTevOp */
    u8 alphaBias;        /* GXTevBias */
    u8 alphaScale;       /* GXTevScale */
    u8 alphaClamp;       /* GXBool */
    u8 alphaOutReg;      /* GXTevRegID */

    /* TEV stage binding */
    u8 texCoordId;       /* GXTexCoordID */
    u8 texMapId;         /* GXTexMapID */
    u8 channelId;        /* GXChannelID */

    /* Convenience: GXSetTevOp preset mode (if set) */
    u8 tevMode;          /* GXTevMode (0xFF = custom, not preset) */
} GXTevStageState;

/* =========================================================================
 * Full TEV state snapshot -- used as shader cache key
 * ========================================================================= */

typedef struct GXTevState {
    /** Per-stage configuration */
    GXTevStageState stages[GX_TEV_MAX_STAGES];

    /** Number of active TEV stages */
    u8 numStages;

    /** Dirty flag: set when any TEV state changes, cleared after compile */
    u8 dirty;

    /** Number of active texture generators (for texcoord varying count) */
    u8 numTexGens;

    /** Number of active color channels */
    u8 numChans;

    /** Alpha compare state (for discard in fragment shader) */
    u8 alphaComp0;       /* GXCompare */
    u8 alphaComp1;       /* GXCompare */
    u8 alphaRef0;
    u8 alphaRef1;
    u8 alphaOp;          /* GXAlphaOp */

    /** Fog enabled flag */
    u8 fogEnable;

    /** Z compare location (before/after texture) */
    u8 zCompLocBeforeTex;
} GXTevState;

/* =========================================================================
 * Shader cache entry
 * ========================================================================= */

typedef struct GXTevShaderEntry {
    /** Hash of the GXTevState that produced this shader */
    u32 stateHash;

    /** OpenGL shader program ID */
    u32 glProgram;

    /** Uniform locations (cached after linking) */
    s32 loc_projMatrix;
    s32 loc_modelViewMatrix;
    s32 loc_normalMatrix;
    s32 loc_tex[8];
    s32 loc_tevColor[4];
    s32 loc_tevKonst[4];
    s32 loc_matAmbient;
    s32 loc_matDiffuse;
    s32 loc_alphaComp0;
    s32 loc_alphaRef0;
    s32 loc_alphaComp1;
    s32 loc_alphaRef1;
    s32 loc_alphaOp;
    s32 loc_fogEnable;
    s32 loc_fogType;
    s32 loc_fogStart;
    s32 loc_fogEnd;
    s32 loc_fogColor;
    s32 loc_texSwizzle[8];

    /** Validity flag */
    u8 valid;
} GXTevShaderEntry;

/* =========================================================================
 * Public API
 * ========================================================================= */

/**
 * gx_tev_init -- Initialize the TEV shader cache.
 *
 * Allocates the cache array and compiles the default "passthrough" shader
 * (single TEV stage, PASSCLR mode).
 *
 * Call once during GXInit, before any rendering.
 */
void gx_tev_init(void);

/**
 * gx_tev_shutdown -- Free all cached shaders and release resources.
 *
 * Call during shutdown to clean up GL shader programs.
 */
void gx_tev_shutdown(void);

/**
 * gx_tev_compile -- Compile or look up a GLSL shader for the current TEV state.
 *
 * @param state  Pointer to the current TEV state snapshot.
 * @return       Pointer to the cached shader entry, or NULL on compile failure.
 *
 * If a shader for this state hash already exists in the cache, return it.
 * Otherwise, generate a new GLSL fragment shader from the TEV stage
 * configuration, compile and link it with the standard vertex shader,
 * and store it in the cache.
 *
 * The generated fragment shader follows the pattern in pc_port_design.md S2.3:
 *
 *   #version 330 core
 *   uniform sampler2D u_tex[8];
 *   uniform vec4 u_tevColor[4];
 *   uniform vec4 u_tevKonst[4];
 *   in vec4 v_color0;
 *   in vec2 v_texcoord[8];
 *   out vec4 fragColor;
 *
 *   void main() {
 *       vec4 prev = v_color0;
 *       // --- TEV Stage 0 ---
 *       // ... generated from stage[0] colorIn/alphaIn/colorOp/alphaOp
 *       // --- TEV Stage 1..N ---
 *       // ... generated similarly
 *       // --- Alpha test ---
 *       // if (!alphaTest(prev.a)) discard;
 *       fragColor = prev;
 *   }
 */
GXTevShaderEntry* gx_tev_compile(const GXTevState* state);

/**
 * gx_tev_bind -- Bind the shader for the current TEV state and set uniforms.
 *
 * @param entry  Shader cache entry from gx_tev_compile.
 *
 * Calls glUseProgram and sets all uniform values from the current GX state.
 */
void gx_tev_bind(const GXTevShaderEntry* entry);

/**
 * gx_tev_hash -- Compute a hash of the TEV state for cache lookup.
 *
 * @param state  Pointer to the TEV state to hash.
 * @return       32-bit hash value.
 *
 * Uses FNV-1a or similar fast hash over the state bytes.
 */
u32 gx_tev_hash(const GXTevState* state);

/**
 * gx_tev_generate_fragment_shader -- Generate GLSL source from TEV state.
 *
 * @param state    Pointer to the TEV state.
 * @param outBuf   Output buffer for the GLSL source string.
 * @param bufSize  Size of the output buffer.
 * @return         Length of the generated source, or -1 on error.
 *
 * Emits a complete GLSL 330 fragment shader that implements the TEV
 * combiner logic described by the state.
 */
s32 gx_tev_generate_fragment_shader(const GXTevState* state,
                                    char* outBuf, u32 bufSize);

/**
 * gx_tev_generate_vertex_shader -- Generate the standard vertex shader.
 *
 * @param numTexGens  Number of active texture coordinate generators.
 * @param numChans    Number of active color channels.
 * @param outBuf      Output buffer for the GLSL source string.
 * @param bufSize     Size of the output buffer.
 * @return            Length of the generated source, or -1 on error.
 *
 * The vertex shader is mostly static (same for all TEV configurations),
 * but the number of texcoord varyings and color channel computations
 * varies.
 */
s32 gx_tev_generate_vertex_shader(u8 numTexGens, u8 numChans,
                                  char* outBuf, u32 bufSize);

/**
 * gx_tev_get_cache_stats -- Get shader cache hit/miss statistics.
 *
 * @param outHits    Pointer to receive hit count.
 * @param outMisses  Pointer to receive miss count.
 * @param outTotal   Pointer to receive total cached shaders.
 */
void gx_tev_get_cache_stats(u32* outHits, u32* outMisses, u32* outTotal);

#ifdef __cplusplus
}
#endif

#endif /* PCPORT_GX_TEV_H */
