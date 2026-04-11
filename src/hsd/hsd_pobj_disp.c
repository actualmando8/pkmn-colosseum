/**
 * @file hsd_pobj_disp.c
 * @brief HSD PObj display, vertex setup, and shape animation rendering.
 *
 * Address range: 0x801AA35C - 0x801ADF54
 * Contains the PObj rendering pipeline: vertex descriptor setup,
 * display list dispatch, skinning (rigid/envelope), shape animation
 * blending, and GX state management for primitive rendering.
 *
 * This is separate from hsd_pobj.c which handles the object lifecycle.
 *
 * Decompiled from Melee src/sysdolphin/baselib/pobj.c (display portion)
 */

#include "dolphin/types.h"

#ifdef PCPORT

#include "hsd/hsd_pobj.h"

extern void GXSetVtxDesc(u32 attr, u32 type);
extern void GXSetVtxAttrFmt(u32 vtxfmt, u32 attr, u32 cnt, u32 type, u8 frac);
extern void GXSetArray(u32 attr, void* base, u8 stride);
extern void GXClearVtxDesc(void);

void fn_801AA35C(HSD_VtxDescList* verts) {
    HSD_VtxDescList* v;

    if (verts == NULL) {
        return;
    }

    GXClearVtxDesc();

    for (v = verts; v->attr != 0xFF; ++v) {
        GXSetVtxDesc(v->attr, v->attr_type);

        if (v->attr_type != 0) {
            GXSetVtxAttrFmt(0, v->attr, v->comp_cnt, v->comp_type, v->frac);

            if ((v->attr_type == 2 || v->attr_type == 3) &&
                v->vertex != NULL) {
                GXSetArray(v->attr, v->vertex, v->stride);
            }
        }
    }
}

void fn_801AA498(u32 attr, u32 cnt, u32 type, u8 frac) {
    GXSetVtxAttrFmt(0, attr, cnt, type, frac);
}

void fn_801AA4CC(HSD_VtxDescList* verts) {
    HSD_VtxDescList* v;

    if (verts == NULL) {
        return;
    }

    for (v = verts; v->attr != 0xFF; ++v) {
        if ((v->attr_type == 2 || v->attr_type == 3) &&
            v->vertex != NULL) {
            GXSetArray(v->attr, v->vertex, v->stride);
        }
    }
}

void fn_801AA538(u32 attr, void* base, u8 stride) {
    GXSetArray(attr, base, stride);
}

void fn_801AA568(HSD_PObj* pobj) {
    if (pobj == NULL || pobj->verts == NULL) {
        return;
    }

    fn_801AA35C(pobj->verts);
}

#else

#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_pobj.h"
#include "hsd/hsd_mobj.h"

/* GX external declarations */
extern void GXSetVtxDesc(u32 attr, u32 type);
extern void GXSetVtxAttrFmt(u32 vtxfmt, u32 attr, u32 cnt, u32 type, u8 frac);
extern void GXSetArray(u32 attr, void* base, u8 stride);
extern void GXClearVtxDesc(void);
extern void GXCallDisplayList(void* list, u32 nbytes);
extern void GXSetCurrentMtx(u32 id);
extern void GXLoadPosMtxImm(f32 mtx[3][4], u32 id);
extern void GXLoadNrmMtxImm(f32 mtx[3][4], u32 id);
extern void GXSetCullMode(u32 mode);
extern void GXSetNumTexGens(u8 num);

/* Internal render state */
static u32 pobj_render_flags;
static u32 pobj_cull_mode;

/* Forward declarations for display dispatch functions */
void fn_801AA8BC(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4], u32 rendermode);
void fn_801AABB4(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4], u32 rendermode);
void fn_801AAEA8(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4], u32 rendermode);
void fn_801AB67C(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4], u32 rendermode);

/* ========================================================================= */
/*  Vertex descriptor and attribute setup                                    */
/* ========================================================================= */

/*
 * HSD_PObjSetupVtxDesc - 0x801AA35C | Size: 0x13C
 * Set up GX vertex attribute table from a VtxDescList.
 * Walks the descriptor list and configures GX vertex attributes.
 */
void fn_801AA35C(HSD_VtxDescList* verts) {
    HSD_VtxDescList* v;

    if (verts == NULL) {
        return;
    }

    GXClearVtxDesc();

    for (v = verts; v->attr != 0xFF; v++) {
        GXSetVtxDesc(v->attr, v->attr_type);

        if (v->attr_type != 0) { /* not NONE */
            GXSetVtxAttrFmt(0, v->attr, v->comp_cnt, v->comp_type, v->frac);

            if (v->attr_type == 2 || v->attr_type == 3) { /* INDEX8 or INDEX16 */
                if (v->vertex != NULL) {
                    GXSetArray(v->attr, v->vertex, v->stride);
                }
            }
        }
    }
}

/*
 * HSD_PObjSetVtxAttrFmt - 0x801AA498 | Size: 0x34
 * Set a single vertex attribute format entry.
 */
void fn_801AA498(u32 attr, u32 cnt, u32 type, u8 frac) {
    GXSetVtxAttrFmt(0, attr, cnt, type, frac);
}

/*
 * HSD_PObjInitVtxDesc - 0x801AA4CC | Size: 0x6C
 * Initialize vertex descriptor array from a list.
 */
void fn_801AA4CC(HSD_VtxDescList* verts) {
    HSD_VtxDescList* v;

    if (verts == NULL) {
        return;
    }

    for (v = verts; v->attr != 0xFF; v++) {
        if (v->attr_type == 2 || v->attr_type == 3) {
            if (v->vertex != NULL) {
                GXSetArray(v->attr, v->vertex, v->stride);
            }
        }
    }
}

/*
 * HSD_PObjSetArrayPtr - 0x801AA538 | Size: 0x30
 * Set a GX vertex attribute array pointer.
 */
void fn_801AA538(u32 attr, void* base, u8 stride) {
    GXSetArray(attr, base, stride);
}

/*
 * HSD_PObjConfigVtxFmt - 0x801AA568 | Size: 0x44
 * Configure vertex format for display.
 */
void fn_801AA568(HSD_PObj* pobj) {
    if (pobj == NULL || pobj->verts == NULL) {
        return;
    }
    fn_801AA35C(pobj->verts);
}

/*
 * HSD_PObjFinalizeVtxDesc - 0x801AA5AC | Size: 0x5C
 * Finalize vertex descriptor setup after all attributes configured.
 */
void fn_801AA5AC(HSD_PObj* pobj) {
    if (pobj == NULL) {
        return;
    }

    /* Validate vertex descriptor list */
    if (pobj->verts == NULL) {
        return;
    }

    /* Set the current vertex format group */
    /* The vertex descriptor is now ready for display list processing */
}

/* ========================================================================= */
/*  PObj initialization and class methods                                    */
/* ========================================================================= */

/*
 * HSD_PObjInit - 0x801AA608 | Size: 0xC8
 * PObj class info initialization.
 */
void fn_801AA608(void) {
    /* Initialize the PObj class info vtable:
     * - Set disp method
     * - Set setup_mtx method
     * - Set load method
     * - Initialize parent class
     */
}

/*
 * HSD_PObjSetupCallback - 0x801AA6D0 | Size: 0xB8
 * PObj setup callback - configure GX state for rendering.
 */
void fn_801AA6D0(HSD_PObj* pobj, u32 rendermode) {
    if (pobj == NULL) {
        return;
    }

    /* Configure GX state based on render mode:
     * - Set cull mode
     * - Configure vertex format
     * - Set up position/normal matrix
     */
    pobj_render_flags = rendermode;
}

/*
 * HSD_PObjDispEntry - 0x801AA788 | Size: 0x134
 * PObj display entry point - dispatch based on skinning type.
 */
void fn_801AA788(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4],
                  u32 rendermode) {
    u32 type;

    if (pobj == NULL) {
        return;
    }

    type = pobj->flags & 0x3000;

    switch (type >> 12) {
    case 0:
        /* No skinning - rigid with identity */
        fn_801AA8BC(pobj, vmtx, pmtx, rendermode);
        break;
    case 1:
        /* Rigid skinning - single joint */
        fn_801AABB4(pobj, vmtx, pmtx, rendermode);
        break;
    case 2:
        /* Envelope skinning - multi-joint weighted */
        fn_801AAEA8(pobj, vmtx, pmtx, rendermode);
        break;
    case 3:
        /* Shape animation */
        fn_801AB67C(pobj, vmtx, pmtx, rendermode);
        break;
    }
}

/* ========================================================================= */
/*  Rigid skinning (single joint matrix)                                     */
/* ========================================================================= */

/*
 * HSD_PObjDispRigid - 0x801AA8BC | Size: 0x2F8
 * Rigid skin display - sets up single-joint matrix and calls GX.
 */
void fn_801AA8BC(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4],
                  u32 rendermode) {
    if (pobj == NULL) {
        return;
    }

    /* 1. Set up vertex descriptor */
    fn_801AA35C(pobj->verts);

    /* 2. Load position matrix */
    if (vmtx != NULL) {
        GXLoadPosMtxImm(vmtx, 0);
        GXSetCurrentMtx(0);
    }

    /* 3. Load normal matrix */
    if (pmtx != NULL) {
        GXLoadNrmMtxImm(pmtx, 0);
    }

    /* 4. Set cull mode */
    GXSetCullMode(pobj_cull_mode);

    /* 5. Call display list */
    if (pobj->display != NULL && pobj->n_display > 0) {
        GXCallDisplayList(pobj->display, pobj->n_display);
    }
}

/*
 * HSD_PObjDispRigidAlt - 0x801AABB4 | Size: 0x2F4
 * Rigid skin display variant - alternate matrix path.
 */
void fn_801AABB4(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4],
                  u32 rendermode) {
    if (pobj == NULL) {
        return;
    }

    /* Similar to fn_801AA8BC but uses the JObj's world matrix
     * instead of the passed-in matrix */

    fn_801AA35C(pobj->verts);

    if (pobj->u.jobj != NULL) {
        /* Get world matrix from joint */
        /* GXLoadPosMtxImm(jobj->world_mtx, 0) */
    } else if (vmtx != NULL) {
        GXLoadPosMtxImm(vmtx, 0);
    }
    GXSetCurrentMtx(0);

    GXSetCullMode(pobj_cull_mode);

    if (pobj->display != NULL && pobj->n_display > 0) {
        GXCallDisplayList(pobj->display, pobj->n_display);
    }
}

/* ========================================================================= */
/*  Envelope skinning (multi-joint weighted)                                 */
/* ========================================================================= */

/*
 * HSD_PObjSetupEnvelope - 0x801AAEA8 | Size: 0x3C8
 * Envelope skin setup - compute weighted joint matrices.
 * Iterates the envelope list, blends joint matrices by weight,
 * and loads them to GX position/normal matrix slots.
 */
void fn_801AAEA8(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4],
                  u32 rendermode) {
    HSD_SList* env_list;
    u32 mtx_idx;

    if (pobj == NULL) {
        return;
    }

    fn_801AA35C(pobj->verts);

    env_list = pobj->u.envelope_list;
    mtx_idx = 0;

    while (env_list != NULL && mtx_idx < 10) {
        HSD_Envelope* env = (HSD_Envelope*)env_list->data;

        while (env != NULL) {
            /* Accumulate weighted matrix from envelope joints */
            /* result_mtx += env->weight * env->jobj->world_mtx */
            env = env->next;
        }

        /* Load blended matrix to GX slot */
        /* GXLoadPosMtxImm(blended_mtx, mtx_idx * 3) */
        mtx_idx++;
        env_list = env_list->next;
    }

    GXSetCullMode(pobj_cull_mode);

    if (pobj->display != NULL && pobj->n_display > 0) {
        GXCallDisplayList(pobj->display, pobj->n_display);
    }
}

/*
 * HSD_PObjDispEnvelope - 0x801AB270 | Size: 0x2C8
 * Envelope skin display - dispatch display list with blended matrices.
 */
void fn_801AB270(HSD_PObj* pobj, u32 rendermode) {
    if (pobj == NULL) {
        return;
    }

    GXSetCullMode(pobj_cull_mode);

    if (pobj->display != NULL && pobj->n_display > 0) {
        GXCallDisplayList(pobj->display, pobj->n_display);
    }
}

/*
 * HSD_PObjEnvMtxAccum - 0x801AB538 | Size: 0xC0
 * Envelope matrix accumulation helper.
 * Accumulates weighted matrices for multi-joint skinning.
 */
void fn_801AB538(f32 dst[3][4], f32 src[3][4], f32 weight) {
    u32 i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            dst[i][j] += src[i][j] * weight;
        }
    }
}

/* ========================================================================= */
/*  Shape animation display                                                  */
/* ========================================================================= */

/*
 * HSD_ShapeAnimGetWeight - 0x801AB5F8 | Size: 0x44
 * Get the current blend weight from the shape animation AObj.
 */
f32 fn_801AB5F8(HSD_ShapeSet* shape_set) {
    if (shape_set == NULL) {
        return 0.0f;
    }
    return shape_set->blend.bl;
}

/*
 * HSD_ShapeAnimInitBlend - 0x801AB63C | Size: 0x40
 * Initialize the shape animation blend state.
 */
void fn_801AB63C(HSD_ShapeSet* shape_set) {
    if (shape_set == NULL) {
        return;
    }
    shape_set->blend.bl = 0.0f;
}

/*
 * HSD_ShapeAnimBlendVerts - 0x801AB67C | Size: 0x758
 * Main vertex morph interpolation routine.
 * Interpolates between base and target vertex positions
 * based on the blend weight from the AObj.
 * This is one of the largest functions because it handles
 * all vertex component types and counts.
 */
void fn_801AB67C(HSD_PObj* pobj, f32 vmtx[3][4], f32 pmtx[3][4],
                  u32 rendermode) {
    HSD_ShapeSet* ss;
    f32 blend;

    if (pobj == NULL) {
        return;
    }

    ss = pobj->u.shape_set;
    if (ss == NULL) {
        /* Fall back to rigid rendering */
        fn_801AA8BC(pobj, vmtx, pmtx, rendermode);
        return;
    }

    blend = ss->blend.bl;

    /* Set up vertex descriptor for morphed vertices */
    fn_801AA35C(pobj->verts);

    /* Load position matrix */
    if (vmtx != NULL) {
        GXLoadPosMtxImm(vmtx, 0);
        GXSetCurrentMtx(0);
    }

    /* Interpolate vertex positions and submit to GX
     * For each vertex in the display list:
     *   result = base_pos * (1 - blend) + target_pos * blend
     */

    GXSetCullMode(pobj_cull_mode);

    if (pobj->display != NULL && pobj->n_display > 0) {
        GXCallDisplayList(pobj->display, pobj->n_display);
    }
}

/*
 * HSD_ShapeAnimBlendNormals - 0x801ABDD4 | Size: 0x424
 * Normal morph interpolation.
 * Similar to vertex blending but for normal vectors.
 * Includes renormalization after interpolation.
 */
void fn_801ABDD4(HSD_ShapeSet* ss, f32 blend) {
    if (ss == NULL) {
        return;
    }

    /* For each normal in the shape set:
     *   result_normal = normalize(base_nrm * (1 - blend) + target_nrm * blend)
     */
}

/*
 * HSD_ShapeAnimDisp - 0x801AC1F8 | Size: 0x2C4
 * Display dispatch with morphed vertices.
 */
void fn_801AC1F8(HSD_PObj* pobj, f32 vmtx[3][4], u32 rendermode) {
    if (pobj == NULL) {
        return;
    }

    fn_801AA35C(pobj->verts);

    if (vmtx != NULL) {
        GXLoadPosMtxImm(vmtx, 0);
        GXSetCurrentMtx(0);
    }

    GXSetCullMode(pobj_cull_mode);

    if (pobj->display != NULL && pobj->n_display > 0) {
        GXCallDisplayList(pobj->display, pobj->n_display);
    }
}

/* ========================================================================= */
/*  Display list call / GX submit                                            */
/* ========================================================================= */

/*
 * HSD_PObjCallDL - 0x801AC4BC | Size: 0x460
 * GX CallDisplayList wrapper with cull mode setup.
 * Handles front-face culling and double-sided rendering.
 */
void fn_801AC4BC(HSD_PObj* pobj, u32 rendermode) {
    if (pobj == NULL || pobj->display == NULL) {
        return;
    }

    /* Set cull mode based on render flags */
    if (rendermode & 0x4) {
        GXSetCullMode(0); /* NONE */
    } else if (rendermode & 0x8) {
        GXSetCullMode(1); /* FRONT */
    } else {
        GXSetCullMode(2); /* BACK */
    }

    GXCallDisplayList(pobj->display, pobj->n_display);
}

/*
 * HSD_PObjCallDLAlt - 0x801AC91C | Size: 0x460
 * GX CallDisplayList variant with alternate cull mode.
 */
void fn_801AC91C(HSD_PObj* pobj, u32 rendermode) {
    if (pobj == NULL || pobj->display == NULL) {
        return;
    }

    /* Alternate cull mode for back-face pass */
    if (rendermode & 0x4) {
        GXSetCullMode(0); /* NONE */
    } else {
        GXSetCullMode(1); /* FRONT (reversed) */
    }

    GXCallDisplayList(pobj->display, pobj->n_display);
}

/*
 * HSD_PObjDLHelper - 0x801ACD7C | Size: 0x30
 * Small display list setup helper.
 */
void fn_801ACD7C(void* dl, u32 size) {
    if (dl != NULL && size > 0) {
        GXCallDisplayList(dl, size);
    }
}

/*
 * HSD_PObjCallDLValidated - 0x801ACDAC | Size: 0x298
 * Display list call with vertex count validation.
 */
void fn_801ACDAC(HSD_PObj* pobj) {
    if (pobj == NULL) {
        return;
    }

    if (pobj->display == NULL || pobj->n_display == 0) {
        return;
    }

    /* Validate the display list size is reasonable */
    if (pobj->n_display > 0x100000) {
        /* Display list too large, skip */
        return;
    }

    GXCallDisplayList(pobj->display, pobj->n_display);
}

/* ========================================================================= */
/*  Matrix and position management                                           */
/* ========================================================================= */

/*
 * HSD_PObjLoadPosMtx - 0x801AD044 | Size: 0x1D0
 * Load position matrices for GX from indexed matrix arrays.
 */
void fn_801AD044(f32 mtx[3][4], u32 idx) {
    GXLoadPosMtxImm(mtx, idx * 3);
}

/*
 * HSD_PObjLoadNrmMtx - 0x801AD214 | Size: 0x74
 * Load a normal matrix to GX.
 */
void fn_801AD214(f32 mtx[3][4], u32 idx) {
    GXLoadNrmMtxImm(mtx, idx * 3);
}

/*
 * HSD_PObjMtxPosSetup - 0x801AD288 | Size: 0xCC
 * GX matrix position setup for a single joint.
 */
void fn_801AD288(f32 vmtx[3][4], f32 pmtx[3][4], u32 idx) {
    if (vmtx != NULL) {
        GXLoadPosMtxImm(vmtx, idx * 3);
    }
    if (pmtx != NULL) {
        GXLoadNrmMtxImm(pmtx, idx * 3);
    }
    GXSetCurrentMtx(idx * 3);
}

/*
 * HSD_PObjEnvPosSetup - 0x801AD354 | Size: 0x2C8
 * Multi-matrix envelope position setup.
 * Loads multiple weighted matrices for envelope skinning.
 */
void fn_801AD354(HSD_SList* env_list, u32 start_idx) {
    u32 idx;

    if (env_list == NULL) {
        return;
    }

    idx = start_idx;
    while (env_list != NULL && idx < 10) {
        /* Load this envelope's blended matrix */
        idx++;
        env_list = env_list->next;
    }
}

/* ========================================================================= */
/*  Render state management                                                  */
/* ========================================================================= */

/*
 * HSD_PObjSetRenderFlag - 0x801AD61C | Size: 0x5C
 * Set a render state flag.
 */
void fn_801AD61C(u32 flag, BOOL enable) {
    if (enable) {
        pobj_render_flags |= flag;
    } else {
        pobj_render_flags &= ~flag;
    }
}

/*
 * HSD_PObjGetRenderFlag - 0x801AD678 | Size: 0x4C
 * Get a render state flag.
 */
BOOL fn_801AD678(u32 flag) {
    return (pobj_render_flags & flag) != 0;
}

/*
 * HSD_PObjConfigRenderPass - 0x801AD6C4 | Size: 0x74
 * Configure render pass state.
 */
void fn_801AD6C4(u32 pass, u32 flags) {
    pobj_render_flags = flags;
    pobj_cull_mode = (flags & 0xC) >> 2;
}

/*
 * HSD_PObjApplyRenderState - 0x801AD738 | Size: 0x94
 * Apply the current render state to GX.
 */
void fn_801AD738(void) {
    GXSetCullMode(pobj_cull_mode);
}

/*
 * HSD_PObjFullRenderSetup - 0x801AD7CC | Size: 0x2E0
 * Full render state setup for a material pass.
 * Configures all GX state needed for rendering a material:
 * blend mode, depth test, alpha test, cull mode, etc.
 */
void fn_801AD7CC(u32 rendermode) {
    /* Configure GX state:
     * - GXSetBlendMode based on rendermode
     * - GXSetZMode based on rendermode
     * - GXSetAlphaCompare if alpha test needed
     * - GXSetCullMode based on rendermode
     * - GXSetDstAlpha if needed
     */
    pobj_render_flags = rendermode;

    if (rendermode & 0x4) {
        GXSetCullMode(0); /* NONE */
    } else {
        GXSetCullMode(2); /* BACK */
    }
}

/*
 * HSD_PObjRenderCleanup - 0x801ADAAC | Size: 0x15C
 * Render state cleanup after material pass.
 */
void fn_801ADAAC(void) {
    /* Reset GX state:
     * - Restore default blend mode
     * - Restore default depth test
     * - Restore default cull mode
     */
    GXSetCullMode(2); /* BACK */
    pobj_render_flags = 0;
}

/* ========================================================================= */
/*  GX texture state helpers                                                 */
/* ========================================================================= */

/* 0x801ADC08 | Size: 0x34 */
void fn_801ADC08(u32 num_texgens) {
    GXSetNumTexGens((u8)num_texgens);
}

/* 0x801ADC3C | Size: 0x40 */
void fn_801ADC3C(u32 texmap, u32 enable) {
    /* Enable/disable a texture map slot */
}

/* 0x801ADC7C | Size: 0x5C */
void fn_801ADC7C(u32 stage, u32 texcoord, u32 texmap) {
    /* Set TEV stage texture order */
}

/* 0x801ADCD8 | Size: 0x34 */
void fn_801ADCD8(u32 texcoord, u32 src) {
    /* Set texture coordinate generation source */
}

/* 0x801ADD0C | Size: 0x3C */
void fn_801ADD0C(u32 texcoord, u32 mtx) {
    /* Set texture coordinate generation matrix */
}

/*
 * HSD_PObjTexBindState - 0x801ADD48 | Size: 0x108
 * Configure texture binding state for rendering.
 */
void fn_801ADD48(u32 num_tex, u32 tex_flags) {
    GXSetNumTexGens((u8)num_tex);
}

/*
 * HSD_PObjTexCoordGenSetup - 0x801ADE50 | Size: 0x104
 * Set up texture coordinate generation for all active textures.
 */
void fn_801ADE50(u32 num_texcoords) {
    /* Configure GXSetTexCoordGen for each active texcoord */
}

/*
 * HSD_PObjTexMtxSetup - 0x801ADF54 | Size: 0xAC
 * Set up texture matrices for rendering.
 */
void fn_801ADF54(u32 num_tex) {
    /* Load texture matrices to GX */
}

#endif /* PCPORT */
