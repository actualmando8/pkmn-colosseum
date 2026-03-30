/**
 * @file gs_material.c
 * @brief GSmaterial -- Material creation, TEV setup, texture binding.
 *
 * This module sits between GSmem and GStexture in the link order and
 * implements the bridge between the Genius Sonority engine and HSD's
 * MObj (material object) rendering layer.  It is the core rendering
 * state manager for the game.
 *
 * Decompiled from ~160 functions in range 0x800E3604 - 0x800EE2C8.
 *
 * Sub-modules identified within this range:
 *
 * 1. Material pool iteration / apply (0x800E3604 - 0x800E3B08)
 *    - fn_800E3604: GSmaterialApplyAll (0x15C bytes)
 *      Iterates the material pool at stride 0x170, checking flags for
 *      active+valid+texture-stage match. For each matching entry:
 *        - Acquires HSD render context (fn_800D2584)
 *        - Calls fn_800E9148 (render slot check)
 *        - Selects MObj pointer from +0x08 or +0x0C based on render type
 *        - Applies animation (fn_801A13CC with params 0,1,0)
 *        - Applies env map (fn_801A13CC with params 0,4,0)
 *        - Applies shadow (fn_801A13CC with params 0,2,0)
 *        - Calls callback at +0x168/+0x16C via fn_800D6A5C
 *        - Releases render context (fn_800E9148 with param 0)
 *        - Calls fn_80195A48 (HSD render end)
 *      Then calls fn_800D87AC(-1) to reset internal render mode.
 *
 *    - fn_800E3760: GSmaterialApplySingle (0x124 bytes)
 *      Same as above but for a single entry pointer. Also checks bit 10
 *      (shadow) in the entry flags and calls fn_80190E60 with +0x4C offset
 *      (the transform matrix) if the shadow flag is set.
 *
 * 2. Color interpolation and update (0x800E3884 - 0x800E3B08)
 *    - fn_800E3884: GSmaterialLerpColors (0xA4 bytes)
 *      Float-based color interpolation using paired-singles.
 *    - fn_800E3928: GSmaterialUpdateColors (0x1E0 bytes)
 *      Full color/alpha update pipeline to HSD MObj.
 *    - fn_800E3B08: Helper (0x34 bytes) -- reads sdata2 float constants.
 *
 * 3. Accessor functions (0x800E3B3C - 0x800E3D08)
 *    Small getters/setters (mostly 0x08-0x54 bytes each):
 *    - fn_800E3B3C: GetPoolCount       -- lwz lbl_8047AB78
 *    - fn_800E3B44: SetShadowFlag      -- bit 10 (0x400) set/clear
 *    - fn_800E3B6C: FindByMObj         -- pool search, stride 0x170
 *    - fn_800E3BC0: GetGXTexGenSrc     -- loads +0x144, calls fn_80118874
 *    - fn_800E3BF0: GetUserData        -- lwz +0x148
 *    - fn_800E3BF8: GetTexture         -- lwz +0x144
 *    - fn_800E3C00: SetUserData        -- stw +0x148
 *    - fn_800E3C08: SetTexture         -- stw +0x144, env-map update
 *    - fn_800E3C5C: GetTransformPtr    -- addi +0x4C
 *    - fn_800E3C64: HasTransform       -- MTXIsIdentity on +0x4C
 *    - fn_800E3C94: SetCustomPEFlag    -- bit 20 set/clear (oris 0x10)
 *    - fn_800E3CBC: GetTexStage        -- extrwi bit 22
 *    - fn_800E3CC8: SetEnvMapFlag      -- bit 9 (0x200) set/clear
 *    - fn_800E3CF0: GetSpecularPtr     -- addi +0x30
 *    - fn_800E3CF8: GetAmbientPtr      -- addi +0x24
 *    - fn_800E3D00: GetDiffusePtr      -- addi +0x18
 *    - fn_800E3D08: IsActive           -- extrwi bit 0
 *
 * 4. Color lerp helpers (0x800E3D14 - 0x800E3D98)
 *    Four functions that pass sub-structure offsets to fn_800E01D0:
 *    - fn_800E3D14: LerpPEColor    -- offset +0x3C
 *    - fn_800E3D40: LerpSpecular   -- offset +0x30
 *    - fn_800E3D6C: LerpAmbient    -- offset +0x24
 *    - fn_800E3D98: LerpDiffuse    -- offset +0x18
 *
 * 5. Alpha/color update pipeline (0x800E3DC4 - 0x800E4598)
 *    - fn_800E3DC4: UpdateAlpha (0x250 bytes)
 *      Reads alpha at +0x98, applies to HSD MObj, handles texture alpha.
 *    - fn_800E4014: Helper (0x68 bytes) -- extracts MObj flags.
 *    - fn_800E407C: Helper (0xF4 bytes) -- alpha compare setup.
 *    - fn_800E4170: SetupAlphaBlend (0x234 bytes) -- GXSetBlendMode.
 *    - fn_800E43A4: SetupZMode (0x170 bytes) -- GXSetZMode.
 *    - fn_800E4514: Helper (0x84 bytes) -- Z compare function.
 *    - fn_800E4598: Helper (0xC4 bytes) -- blend factor selection.
 *
 * 6. TEV configuration core (0x800E465C - 0x800E8EFC)
 *    - fn_800E465C: TEVStageSetup (0x464 bytes)
 *      Configures individual TEV stage color/alpha inputs and ops.
 *    - fn_800E4AC0: TEVColorRegister (0x134 bytes)
 *      Sets GX TEV constant color registers.
 *    - fn_800E4BF4: TEVSwapMode (0xA4 bytes)
 *      Configures TEV swap mode table entries.
 *    - fn_800E4C98-fn_800E5188: TEV helpers (various sizes)
 *      Small functions for individual TEV parameters.
 *    - fn_800E51A4: TEVMultiStageSetup (0x3AC bytes)
 *      Configures multi-stage TEV for complex shading (bump, detail).
 *    - fn_800E5550-fn_800E60F0: Texture coordinate generation
 *      Functions for setting up GXTexCoordGen, GXSetTexCoordGen2.
 *    - fn_800E61BC-fn_800E6BC8: Material color / light channel config
 *      Functions that configure GXSetChanCtrl, GXSetChanMatColor.
 *    - fn_800E6DC0: Bridge (0xC bytes) -- simple forwarding function.
 *    - fn_800E6DCC: Large TEV descriptor builder (0x4C4 bytes)
 *      Builds a complete TEV descriptor from material properties.
 *    - fn_800E7290: TEV stage count calculator (0x9C bytes)
 *      Determines how many TEV stages are needed.
 *    - fn_800E732C: FULL TEV PIPELINE (0x12BC bytes)
 *      The largest function in the module. Configures the complete
 *      TEV pipeline: stage count, all combiners, texcoord gen, indirect
 *      texture, kolor registers, and swap mode. THIS IS THE CRITICAL
 *      FUNCTION for the PC port shader pipeline translation.
 *    - fn_800E85E8: TEV post-config (0x9C bytes)
 *      Final TEV state after the main pipeline setup.
 *    - fn_800E8684: LIGHTING SETUP (0x878 bytes)
 *      Second-largest function. Full GX lighting pipeline: light channels,
 *      attenuation, diffuse/specular material color sources.
 *    - fn_800E8EFC: RenderStateReset (0x6C bytes)
 *      Cleanup after material rendering.
 *
 * 7. Callback / distance / render-slot system (0x800E8F68 - 0x800E9358)
 *    - fn_800E8F68: SetCallback (0xC bytes) -- stores lbl_8047AB84/80
 *    - fn_800E8F74: SetDistanceThreshold (0xC bytes) -- fmuls f1,f1
 *    - fn_800E8F80: Helper (0x20 bytes)
 *    - fn_800E8FA0: Helper (0x48 bytes)
 *    - fn_800E8FE8: Helper (0x24 bytes)
 *    - fn_800E900C: Distance check (0xBC bytes)
 *    - fn_800E90C8: Slot helper (0x40 bytes)
 *    - fn_800E9108: Slot helper (0x40 bytes)
 *    - fn_800E9148: CheckRenderSlot (0x140 bytes)
 *      Uses lbl_80401490 table with stride 0x58 to track active renders.
 *    - fn_800E9288: Slot release (0x50 bytes)
 *    - fn_800E92D8: Slot config (0x80 bytes)
 *    - fn_800E9358: Slot cleanup (0x60 bytes)
 *
 * 8. Environment map system (0x800E93B8 - 0x800EA60C)
 *    - fn_800E93B8: SetupEnvMap (0x5E0 bytes)
 *      Full env-map creation pipeline. Validates texture, checks format,
 *      creates reflection texture, configures indirect TEV stages.
 *    - fn_800E9998: UpdateEnvMap (0x194 bytes)
 *      Per-frame env-map matrix update from camera view.
 *    - fn_800E9B2C: EnvMap texcoord (0x140 bytes)
 *    - fn_800E9C6C: EnvMap TEV stage (0x1C8 bytes)
 *    - fn_800E9E34: EnvMap helper (0x5C bytes)
 *    - fn_800E9E90: EnvMap pipeline (0x77C bytes)
 *      Large environment map rendering pipeline.
 *
 * 9. GX state emission / render pipeline (0x800EA60C - 0x800EE2C8)
 *    - fn_800EA60C: GX state query (0x58 bytes)
 *    - fn_800EA664: GX state helper (0x70 bytes)
 *    - fn_800EA6D4: BindTextureToStage (0x110 bytes)
 *    - fn_800EA7E4: Helper (0x3C bytes)
 *    - fn_800EA820: Texture load (0x140 bytes)
 *    - fn_800EA960: ConfigureTEVStage (0x370 bytes)
 *    - fn_800EACD0: ConfigureBlend (0x314 bytes)
 *    - fn_800EAFE4: ConfigureZMode (0x284 bytes)
 *    - fn_800EB268: ConfigureAlpha (0xD8 bytes)
 *    - fn_800EB340: ConfigureAlphaTest (0xD4 bytes)
 *    - fn_800EB414: Helper (0x50 bytes)
 *    - fn_800EB464: Helper (0xBC bytes)
 *    - fn_800EB520: Simple getter (0x8 bytes)
 *    - fn_800EB528: Helper (0x78 bytes)
 *    - fn_800EB5A0: GX command batch (0x140 bytes)
 *    - fn_800EB6E0: Texture setup (0x224 bytes)
 *    - fn_800EB904: Large render setup (0x5E8 bytes)
 *    - fn_800EBEEC: Render config (0x1FC bytes)
 *    - fn_800EC0E8: SetAlpha (0x4C bytes)
 *    - fn_800EC134: UpdateMObjColor (0x20 bytes)
 *    - fn_800EC154: GetMObjPtr (0xC bytes)
 *    - fn_800EC160: SetDiffuseRGBA (0x28 bytes)
 *    - fn_800EC188: SetAmbientRGBA (0x28 bytes)
 *    - fn_800EC1B0: GetDiffuseR (0xC bytes)
 *    - fn_800EC1BC: GetDiffuseG (0xC bytes)
 *    - fn_800EC1C8: GetDiffuseB (0xC bytes)
 *    - fn_800EC1D4: GetDiffuseA (0x10 bytes)
 *    - fn_800EC1E4: Color helper (0x24 bytes)
 *    - fn_800EC208: Color config (0x9C bytes)
 *    - fn_800EC2A4: Helper (0x64 bytes)
 *    - fn_800EC308: Helper (0x54 bytes)
 *    - fn_800EC35C: PE descriptor setup (0x174 bytes)
 *    - fn_800EC4D0: PE helper (0x6C bytes)
 *    - fn_800EC53C: PE helper (0x2C bytes)
 *    - fn_800EC568: Simple getter (0x8 bytes)
 *    - fn_800EC570: Simple getter (0x8 bytes)
 *    - fn_800EC578: PE config (0x34 bytes)
 *    - fn_800EC5AC: PE helper (0xC bytes)
 *    - fn_800EC5B8: Pre-render setup (0x44 bytes)
 *    - fn_800EC5FC: RenderSetup (0x2CC bytes) -- primary entry point
 *    - fn_800EC8C8: Helper (0x14 bytes)
 *    - fn_800EC8DC: Helper (0x3C bytes)
 *    - fn_800EC918: Helper (0x3C bytes)
 *    - fn_800EC954: Simple (0xC bytes)
 *    - fn_800EC960: Simple (0xC bytes)
 *    - fn_800EC96C: Helper (0x24 bytes)
 *    - fn_800EC990: Helper (0x4C bytes)
 *    - fn_800EC9DC: Helper (0x9C bytes)
 *    - fn_800ECA78: EnvMap alpha apply (0xFC bytes)
 *    - fn_800ECB74: EnvMap texture update (0x134 bytes)
 *    - fn_800ECCA8: ShadowSetup (0x428 bytes) -- shadow pass pipeline
 *    - fn_800ED0D0: PostRender (0xFC bytes) -- state restore
 *    - fn_800ED1CC: Post-render chain (0x308 bytes)
 *    - fn_800ED4D4: Post-render chain (0x1D8 bytes)
 *    - fn_800ED6AC: Helper (0x38 bytes)
 *    - fn_800ED6E4: Helper (0x100 bytes)
 *    - fn_800ED7E4: Helper (0xE0 bytes)
 *    - fn_800ED8C4: Pipeline stage (0x1D4 bytes)
 *    - fn_800EDA98: FullPipeline (0x5AC bytes) -- complete draw sequence
 *    - fn_800EE044: StoreResult (0x10 bytes)
 *    - fn_800EE054: SetRenderMode (0x38 bytes)
 *    - fn_800EE08C: ConfigureFog (0x5C bytes)
 *    - fn_800EE0E8: ConfigureScissor (0x68 bytes)
 *    - fn_800EE150: ApplyPEDescr (0xBC bytes)
 *    - fn_800EE20C: GetPEParam (0x20 bytes)
 *    - fn_800EE22C: ResetBlendMode (0x5C bytes)
 *    - fn_800EE288: Finalize (0x40 bytes)
 *
 * Architecture notes for PC port:
 *   The TEV pipeline (fn_800E732C) is the most critical function to
 *   understand for shader translation. On GCN, TEV stages are configured
 *   per-draw-call through GX register writes. For a PC port, these must
 *   be translated into GLSL/HLSL shader programs.
 *
 *   Key mapping:
 *     GX TEV stage -> GLSL fragment shader combiner operation
 *     GX TexCoordGen -> GLSL vertex shader texture coordinate output
 *     GX IndirectTex -> GLSL bump/normal map sampling
 *     GX BlendMode -> OpenGL/Vulkan blend state
 *     GX ZMode -> OpenGL/Vulkan depth state
 *     GX AlphaCompare -> GLSL discard / alpha test
 *     GX LitChannel -> GLSL per-vertex lighting calculation
 *
 *   The environment map system (fn_800E93B8) uses GCN-specific
 *   indirect texture hardware, which must be emulated in the fragment
 *   shader for the PC port.
 *
 * Address range: 0x800E3604 - 0x800EE2C8 (~44KB, ~160 functions)
 */

#include "dolphin/types.h"
#include "game/gs_material.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);        /* OSReport / GSlog */
extern void  fn_800D2584(void);                        /* HSD_StartRender (acquire context) */
extern void  fn_800D87AC(s32 mode);                    /* GSgfx_SetInternalMode */
extern void  fn_800DD174(void* renderObj);              /* HSD render dispatch */
extern void  fn_800D6A5C(void* callbackA, void* callbackB); /* callback dispatch */

/* GSmem */
extern u16   fn_800E3534(u32 size);                    /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                  /* GSmemGetPtr */
extern void* fn_800E24B0(u16 handle);                  /* GSmemLock */
extern void  fn_800E209C(u16 handle);                  /* GSmemFree */

/* HSD object system */
extern void  fn_80193B30(void* classDesc, void* parent, void* name,
                          u32 entrySize, u32 alignment, void* nameDup); /* HSD_ClassInit */
extern void  fn_801A13CC(void* obj, u32 a, u32 b, u32 c); /* HSD_MObjAnim */
extern void  fn_80195A6C(void* renderCtx);             /* HSD render context check */
extern void  fn_80195A48(void);                        /* HSD render end */
extern void  fn_80190E60(void* mtx);                   /* MTX operation (shadow) */
extern s32   fn_80191118(void* mtx);                    /* MTXIsIdentity */
extern void  fn_80118874(void* tex, u32 param);        /* GX texgen source */
extern void  fn_80196E10(void* a, u32 line, void* b);  /* HSD assert */
extern void  fn_8019D620(void* mobj);                  /* HSD MObj color update */
extern void  fn_801A6FF0(void* obj);                   /* HSD object update */
extern void  fn_801A7CFC(void* classDesc);             /* HSD class register */
extern void  fn_801A8458(void* mobj);                  /* HSD MObj flag query */
extern void  fn_801B25C4(u32 priority);                /* HSD render priority */
extern void  fn_801B6DC0(void* obj, u32 a, u32 b, u32 c, u32 d); /* HSD TEV config */
extern u32   fn_801B6F5C(void* desc, u32 a, u32 b, u32 c); /* HSD TEV stage */
extern void* fn_801B707C(void* stage);                 /* HSD TEV resolve */
extern void  fn_801B7C60(void* obj);                   /* HSD TEV type check */

/* GX functions (via SDK wrappers) */
extern void  fn_801C2A74(void* a, u32 flags);          /* GX render mode A */
extern void  fn_801C2A90(void* a, u32 flags);          /* GX render mode B */

/* Color interpolation */
extern void  fn_800E01D0(void* dst, void* src);        /* GXColor copy/lerp */
extern void  fn_800E019C(void* a, void* b, void* c);   /* GXColor multiply */

/* Internal material functions (used by SetTexture and others) */
extern void  fn_800ECA78();                            /* env-map alpha apply */
extern void  fn_800EC134();                            /* update MObj color */

/* ===== String constants (rodata) ===== */
extern const char lbl_80270528[]; /* "GSmaterialSetPEdescr: Warning: already using a custom description!\n" */
extern const char lbl_8027056C[]; /* "GSmaterialCreate: Run out of materials. Increase materialcount at initialisation" */
extern const char lbl_802705C0[]; /* "GSmaterial MObj" */
extern const char lbl_802705D0[]; /* "GSmaterial: Unsupported texture format for environment map!\n" */
extern const char lbl_80270610[]; /* "GSmaterial: Error creating environment map: no texture defined!\n" */
extern const char lbl_80270E28[]; /* "!(jobj->flags & JOBJ_USE_QUATERNION)" */

/* ===== Assert file/line strings (sdata2) ===== */
extern u8 lbl_8047CB58[];  /* sdata2: f64 constant */
extern u8 lbl_8047CB60[];  /* sdata2: assert file string */
extern u8 lbl_8047CB68[];  /* sdata2: assert tag string */

/* ===== BSS tables ===== */
extern u8 lbl_80401490[];  /* render slot table (stride 0x58) */

/* ===== HSD class descriptor ===== */
extern u8 lbl_80315490[];  /* GSmaterial class descriptor */
extern u8 lbl_8036CB30[];  /* GSmaterial parent class descriptor */

/* ===== Global state (sbss via SDA) ===== */
/* Material pool (used by GSmaterialApplyAll and accessor functions) */
/* lbl_8047AB74 : GSmaterialEntry* -- pool base pointer */
static GSmaterialEntry* gsMatPool;          /* @sda21 lbl_8047AB74 */
/* lbl_8047AB78 : u32 -- pool capacity (max count) */
static u32 gsMatPoolCount;                  /* @sda21 lbl_8047AB78 */
/* lbl_8047AB80 : u32 -- active callback state */
static u32 gsMatCallbackState;              /* @sda21 lbl_8047AB80 */
/* lbl_8047AB84 : u32 -- callback parameter */
static u32 gsMatCallbackParam;              /* @sda21 lbl_8047AB84 */
/* lbl_8047AB88 : f32 -- squared distance threshold */
static f32 gsMatDistThresholdSq;            /* @sda21 lbl_8047AB88 */

/* Material creation pool (separate from main pool) */
/* lbl_8047AB18 : u16 -- GSmem handle for material creation pool */
static u16  gsMatCreatePoolHandle;          /* @sda21 lbl_8047AB18 */
/* lbl_8047AB1C : void* -- creation pool pointer */
static void* gsMatCreatePool;               /* @sda21 lbl_8047AB1C */
/* lbl_8047AB20 : u32 -- creation pool max count */
static u32  gsMatCreatePoolCount;           /* @sda21 lbl_8047AB20 */

/* =======================================================================
 *  Forward declarations for internal callbacks
 * ======================================================================= */
extern void fn_800DF930(void* a, void* b, void* c, void* d, void* e);
extern void fn_800DFE98(void* a);

/* Forward declarations for converted functions */
void fn_800E9148(void);


/* =======================================================================
 *  GSmaterialGetPoolCount / fn_800E3B3C
 *  Address: 0x800E3B3C, Size: 0x8
 *
 *  Assembly:
 *    lwz r3, lbl_8047AB78@sda21(r0)
 *    blr
 * ======================================================================= */
u32 GSmaterialGetPoolCount(void) {
    return gsMatPoolCount;
}

/* =======================================================================
 *  GSmaterialSetShadowFlag / fn_800E3B44
 *  Address: 0x800E3B44, Size: 0x28
 *
 *  Assembly:
 *    clrlwi. r0, r4, 24         ; mask enable to byte
 *    beq clear
 *    lwz r0, 0x0(r3)           ; load flags
 *    ori r0, r0, 0x400          ; set bit 10
 *    stw r0, 0x0(r3)
 *    blr
 *  clear:
 *    lwz r0, 0x0(r3)
 *    rlwinm r0, r0, 0, 22, 20  ; clear bit 10
 *    stw r0, 0x0(r3)
 *    blr
 * ======================================================================= */
void GSmaterialSetShadowFlag(GSmaterialEntry* entry, u8 enable) {
    if (enable != 0) {
        entry->flags |= GSMAT_FLAG_SHADOW;
    } else {
        entry->flags &= ~GSMAT_FLAG_SHADOW;
    }
}

/* =======================================================================
 *  GSmaterialFindByMObj / fn_800E3B6C
 *  Address: 0x800E3B6C, Size: 0x54
 *
 *  Assembly:
 *    lwz r0, lbl_8047AB78@sda21(r0)    ; pool count
 *    lwz r4, lbl_8047AB74@sda21(r0)    ; pool base
 *    mtctr r0
 *    cmplwi r0, 0
 *    ble notFound
 *  loop:
 *    lwz r5, 0x0(r4)                   ; load flags
 *    clrlwi. r0, r5, 31                ; test bit 0 (active)
 *    beq next
 *    rlwinm. r0, r5, 0, 24, 24         ; test bit 7 (renderType)
 *    beq usePrimary
 *    lwz r0, 0xC(r4)                   ; mobjSecondary
 *    b compare
 *  usePrimary:
 *    lwz r0, 0x8(r4)                   ; mobjPrimary
 *  compare:
 *    cmplw r0, r3                      ; compare with target MObj
 *    bne next
 *    mr r3, r4                          ; found it
 *    blr
 *  next:
 *    addi r4, r4, 0x170                ; stride to next entry
 *    bdnz loop
 *  notFound:
 *    li r3, 0
 *    blr
 * ======================================================================= */
GSmaterialEntry* GSmaterialFindByMObj(void* mobj) {
    u32 count = gsMatPoolCount;
    GSmaterialEntry* entry = gsMatPool;
    u32 i;

    for (i = 0; i < count; i++) {
        u32 flags = entry->flags;

        if (flags & GSMAT_FLAG_ACTIVE) {
            void* entryMObj;

            if (flags & GSMAT_FLAG_RENDERTYPE) {
                entryMObj = entry->mobjSecondary;
            } else {
                entryMObj = entry->mobjPrimary;
            }

            if (entryMObj == mobj) {
                return entry;
            }
        }

        entry = (GSmaterialEntry*)((u8*)entry + 0x170);
    }

    return NULL;
}

/* =======================================================================
 *  GSmaterialGetUserData / fn_800E3BF0
 *  Address: 0x800E3BF0, Size: 0x8
 *
 *  Assembly:
 *    lwz r3, 0x148(r3)
 *    blr
 * ======================================================================= */
void* GSmaterialGetUserData(GSmaterialEntry* entry) {
    return entry->userData;
}

/* =======================================================================
 *  GSmaterialGetTexture / fn_800E3BF8
 *  Address: 0x800E3BF8, Size: 0x8
 *
 *  Assembly:
 *    lwz r3, 0x144(r3)
 *    blr
 * ======================================================================= */
void* GSmaterialGetTexture(GSmaterialEntry* entry) {
    return entry->texture;
}

/* =======================================================================
 *  GSmaterialSetUserData / fn_800E3C00
 *  Address: 0x800E3C00, Size: 0x8
 *
 *  Assembly:
 *    stw r4, 0x148(r3)
 *    blr
 * ======================================================================= */
void GSmaterialSetUserData(GSmaterialEntry* entry, void* data) {
    entry->userData = data;
}

/* =======================================================================
 *  GSmaterialSetTexture / fn_800E3C08
 *  Address: 0x800E3C08, Size: 0x54
 *
 *  Assembly:
 *    lwz r0, 0x144(r3)         ; old texture
 *    cmplw r0, r4              ; same?
 *    beq done
 *    stw r4, 0x144(r31)        ; store new texture
 *    lwz r0, 0x144(r31)        ; reload
 *    cmplwi r0, 0              ; NULL?
 *    beq done
 *    lfs f1, 0x98(r31)         ; load alpha
 *    bl fn_800ECA78            ; env-map alpha apply
 *    mr r3, r31
 *    bl fn_800EC134            ; update MObj color
 *  done:
 * ======================================================================= */
void GSmaterialSetTexture(GSmaterialEntry* entry, void* tex) {
    if (entry->texture == tex) {
        return;
    }

    entry->texture = tex;

    if (entry->texture != NULL) {
        fn_800ECA78(entry);  /* env-map alpha apply -- uses alpha at +0x98 */
        fn_800EC134(entry);  /* update MObj color */
    }
}

/* =======================================================================
 *  GSmaterialGetTransformPtr / fn_800E3C5C
 *  Address: 0x800E3C5C, Size: 0x8
 *
 *  Assembly:
 *    addi r3, r3, 0x4C
 *    blr
 * ======================================================================= */
f32* GSmaterialGetTransformPtr(GSmaterialEntry* entry) {
    return entry->matrix;
}

/* =======================================================================
 *  GSmaterialHasTransform / fn_800E3C64
 *  Address: 0x800E3C64, Size: 0x30
 *
 *  Assembly:
 *    addi r3, r3, 0x4C         ; matrix pointer
 *    bl fn_80191118            ; MTXIsIdentity
 *    neg r0, r3
 *    or r0, r0, r3
 *    srwi r3, r0, 31           ; convert to boolean (0 or 1)
 *    blr
 * ======================================================================= */
u32 GSmaterialHasTransform(GSmaterialEntry* entry) {
    s32 result = (s32)fn_80191118(entry->matrix);
    /* Convert non-zero to 1 */
    return (u32)((-result | result) >> 31) & 1;
}

/* =======================================================================
 *  GSmaterialSetCustomPEFlag / fn_800E3C94
 *  Address: 0x800E3C94, Size: 0x28
 *
 *  Assembly:
 *    clrlwi. r0, r4, 24
 *    beq clear
 *    lwz r0, 0x0(r3)
 *    oris r0, r0, 0x10         ; set bit 20
 *    stw r0, 0x0(r3)
 *    blr
 *  clear:
 *    lwz r0, 0x0(r3)
 *    rlwinm r0, r0, 0, 12, 10  ; clear bit 20
 *    stw r0, 0x0(r3)
 *    blr
 * ======================================================================= */
void GSmaterialSetCustomPEFlag(GSmaterialEntry* entry, u8 enable) {
    if (enable != 0) {
        entry->flags |= GSMAT_FLAG_CUSTOM_PE;
    } else {
        entry->flags &= ~GSMAT_FLAG_CUSTOM_PE;
    }
}

/* =======================================================================
 *  GSmaterialGetTexStage / fn_800E3CBC
 *  Address: 0x800E3CBC, Size: 0xC
 *
 *  Assembly:
 *    lwz r0, 0x0(r3)
 *    extrwi r3, r0, 1, 22      ; extract bit 22
 *    blr
 * ======================================================================= */
u32 GSmaterialGetTexStage(GSmaterialEntry* entry) {
    return (entry->flags >> 9) & 1;  /* bit 22 from MSB = bit 9 from LSB in extrwi encoding */
}

/* =======================================================================
 *  GSmaterialSetEnvMapFlag / fn_800E3CC8
 *  Address: 0x800E3CC8, Size: 0x28
 *
 *  Assembly:
 *    clrlwi. r0, r4, 24
 *    beq clear
 *    lwz r0, 0x0(r3)
 *    ori r0, r0, 0x200          ; set bit 9
 *    stw r0, 0x0(r3)
 *    blr
 *  clear:
 *    lwz r0, 0x0(r3)
 *    rlwinm r0, r0, 0, 23, 21  ; clear bit 9
 *    stw r0, 0x0(r3)
 *    blr
 * ======================================================================= */
void GSmaterialSetEnvMapFlag(GSmaterialEntry* entry, u8 enable) {
    if (enable != 0) {
        entry->flags |= GSMAT_FLAG_ENVMAP;
    } else {
        entry->flags &= ~GSMAT_FLAG_ENVMAP;
    }
}

/* =======================================================================
 *  GSmaterialGetSpecularPtr / fn_800E3CF0
 *  Address: 0x800E3CF0, Size: 0x8
 *
 *  Assembly:
 *    addi r3, r3, 0x30
 *    blr
 * ======================================================================= */
void* GSmaterialGetSpecularPtr(GSmaterialEntry* entry) {
    return (void*)entry->specular;
}

/* =======================================================================
 *  GSmaterialGetAmbientPtr / fn_800E3CF8
 *  Address: 0x800E3CF8, Size: 0x8
 *
 *  Assembly:
 *    addi r3, r3, 0x24
 *    blr
 * ======================================================================= */
void* GSmaterialGetAmbientPtr(GSmaterialEntry* entry) {
    return (void*)entry->ambient;
}

/* =======================================================================
 *  GSmaterialGetDiffusePtr / fn_800E3D00
 *  Address: 0x800E3D00, Size: 0x8
 *
 *  Assembly:
 *    addi r3, r3, 0x18
 *    blr
 * ======================================================================= */
void* GSmaterialGetDiffusePtr(GSmaterialEntry* entry) {
    return (void*)entry->diffuse;
}

/* =======================================================================
 *  GSmaterialIsActive / fn_800E3D08
 *  Address: 0x800E3D08, Size: 0xC
 *
 *  Assembly:
 *    lwz r0, 0x0(r3)
 *    extrwi r3, r0, 1, 30      ; extract bit 0 (LSB)
 *    blr
 * ======================================================================= */
u32 GSmaterialIsActive(GSmaterialEntry* entry) {
    return entry->flags & GSMAT_FLAG_ACTIVE;
}

/* =======================================================================
 *  GSmaterialLerpPEColor / fn_800E3D14
 *  Address: 0x800E3D14, Size: 0x2C
 *
 *  Assembly:
 *    mr r5, r3                  ; entry
 *    mr r3, r4                  ; param
 *    addi r4, r5, 0x3C          ; &entry->peDescriptor
 *    bl fn_800E01D0             ; GXColor copy/lerp
 * ======================================================================= */
void GSmaterialLerpPEColor(GSmaterialEntry* entry, void* param) {
    fn_800E01D0(param, (void*)&entry->peDescriptor);
}

/* =======================================================================
 *  GSmaterialLerpSpecular / fn_800E3D40
 *  Address: 0x800E3D40, Size: 0x2C
 *
 *  Assembly:
 *    mr r5, r3
 *    mr r3, r4
 *    addi r4, r5, 0x30          ; &entry->specular
 *    bl fn_800E01D0
 * ======================================================================= */
void GSmaterialLerpSpecular(GSmaterialEntry* entry, void* param) {
    fn_800E01D0(param, (void*)entry->specular);
}

/* =======================================================================
 *  GSmaterialLerpAmbient / fn_800E3D6C
 *  Address: 0x800E3D6C, Size: 0x2C
 *
 *  Assembly:
 *    mr r5, r3
 *    mr r3, r4
 *    addi r4, r5, 0x24          ; &entry->ambient
 *    bl fn_800E01D0
 * ======================================================================= */
void GSmaterialLerpAmbient(GSmaterialEntry* entry, void* param) {
    fn_800E01D0(param, (void*)entry->ambient);
}

/* =======================================================================
 *  GSmaterialLerpDiffuse / fn_800E3D98
 *  Address: 0x800E3D98, Size: 0x2C
 *
 *  Assembly:
 *    mr r5, r3
 *    mr r3, r4
 *    addi r4, r5, 0x18          ; &entry->diffuse
 *    bl fn_800E01D0
 * ======================================================================= */
void GSmaterialLerpDiffuse(GSmaterialEntry* entry, void* param) {
    fn_800E01D0(param, (void*)entry->diffuse);
}

/* =======================================================================
 *  GSmaterialSetCallback / fn_800E8F68
 *  Address: 0x800E8F68, Size: 0xC
 *
 *  Assembly:
 *    stw r3, lbl_8047AB84@sda21(r0)
 *    stw r4, lbl_8047AB80@sda21(r0)
 *    blr
 * ======================================================================= */
void GSmaterialSetCallback(void* callback, void* state) {
    gsMatCallbackParam = (u32)callback;
    gsMatCallbackState = (u32)state;
}

/* ===================================================================
 * STUB FUNCTIONS -- All remaining functions in 0x800E3604-0x800EE2C8
 * =================================================================== */

/* fn_800E3604 -- GSmaterialApplyAll | Size: 0x15C */
extern u32 lbl_8047AB74;
extern u32 lbl_8047AB78;
/* Forward declarations for self-referencing asm blocks */
extern void fn_800EE044(void* result, void** dest);
extern void fn_800E51A4(void);
extern void fn_800E6B20(void);
extern void fn_800E6DCC(void);
extern void fn_800E85E8(void);
extern void fn_800E9288();
extern void fn_800E92D8(void);
extern void fn_800E9358(void);
extern void fn_800E93B8(void);
extern void fn_800E9998(void);
extern void fn_800E9E90(void);
extern s32 fn_800EA60C();
extern void fn_800EA664(void);
extern void fn_800EA6D4(void);
extern void fn_800EA7E4();
extern void fn_800EA820(void);
extern void fn_800EA960(void);
extern void fn_800EACD0(void);
extern void fn_800EAFE4(void);
extern void fn_800EB340(void);
extern void fn_800EB414();
extern void fn_800EB6E0(void);
extern void fn_800EB904(void);
extern void fn_800EBEEC(void);
extern void fn_800EC1E4();
extern void fn_800EC208(void);
extern void fn_800EC2A4();
extern void fn_800EC308();
extern void fn_800EC35C(void);
extern void fn_800EC990();
extern void fn_800EC9DC(void);
extern void fn_800ECB74(void);
extern void fn_800ECCA8(void);
extern void fn_800ED0D0(void);
extern void fn_800ED1CC(void);
extern void fn_800ED4D4(void);
extern s32 fn_800ED6AC();
extern void fn_800ED6E4(void);
extern void fn_800ED7E4(void);
extern void fn_800ED8C4(void);
extern void fn_800EDA98(void);
extern void fn_800EE054();
extern void fn_800EE08C();
extern void fn_800EE0E8();
extern void fn_800EE150(void);
extern void fn_800EE20C();

#if 1
asm void fn_800E3604(void) {
#include "src/game/gs_material_fn_800E3604.inc"
}
#else
void fn_800E3604(void) {
    /* GSmaterialApplyAll (0x15C bytes) */
}
#endif

/* fn_800E3760 -- GSmaterialApplySingle | Size: 0x124 */
#if 1
asm void fn_800E3760(void) {
#include "src/game/gs_material_fn_800E3760.inc"
}
#else
void fn_800E3760(void) {
    /* GSmaterialApplySingle (0x124 bytes) */
}
#endif

/* fn_800E3884 -- GSmaterialLerpColors | Size: 0xA4 */
extern void fn_800ED0D0(void);
extern u32 lbl_8047AB74;
extern u32 lbl_8047AB78;
#if 1
asm void fn_800E3884(void) {
#include "src/game/gs_material_fn_800E3884.inc"
}
#else
void fn_800E3884(void) {
    /* GSmaterialLerpColors (0xA4 bytes) */
}
#endif

/* fn_800E3928 -- GSmaterialUpdateColors | Size: 0x1E0 */
extern void fn_800EE3BC(void);
extern void fn_800EE828(void);
extern void* memcpy(void* dst, const void* src, u32 n);
extern u32 lbl_8047AB74;
extern u32 lbl_8047AB78;
#if 1
asm void fn_800E3928(void) {
#include "src/game/gs_material_fn_800E3928.inc"
}
#else
void fn_800E3928(void) {
    /* GSmaterialUpdateColors (0x1E0 bytes) */
}
#endif

/* fn_800E3B08 | Size: 0x34 */
extern u32 lbl_8047AB78;
extern u32 lbl_8047AB74;
#if 0
asm void fn_800E3B08(void) {
#include "src/game/gs_material_fn_800E3B08.inc"
}
#else
GSmaterialEntry* fn_800E3B08(u32 index) {
    GSmaterialEntry* entry;
    if (index >= gsMatPoolCount) {
        return NULL;
    }
    entry = (GSmaterialEntry*)((u8*)gsMatPool + index * 0x170);
    if (entry->flags & GSMAT_FLAG_ACTIVE) {
        return entry;
    }
    return NULL;
}
#endif

/* fn_800E3BC0 -- GetGXTexGenSrc | Size: 0x30 */
#if 0
asm void fn_800E3BC0(void) {
#include "src/game/gs_material_fn_800E3BC0.inc"
}
#else
void fn_800E3BC0(GSmaterialEntry* entry) {
    if (entry->texture != NULL) {
        fn_80118874(entry->texture, 1);
    }
}
#endif

/* fn_800E3DC4 -- GSmaterialUpdateAlpha | Size: 0x250 */
#if 1
asm void fn_800E3DC4(void) {
#include "src/game/gs_material_fn_800E3DC4.inc"
}
#else
void fn_800E3DC4(void) {
    /* GSmaterialUpdateAlpha (0x250 bytes) */
}
#endif

/* fn_800E4014 | Size: 0x68 */
extern void fn_80118C20(void);
#if 0
asm void fn_800E4014(void) {
#include "src/game/gs_material_fn_800E4014.inc"
}
#else
void fn_800E4014(GSmaterialEntry* entry, u8 enable) {
    if (enable) {
        entry->flags |= GSMAT_FLAG_VALID;
        if (entry->texture != NULL) {
            ((void(*)(void*, u32))fn_80118C20)(entry->texture, 1);
        }
    } else {
        entry->flags &= ~GSMAT_FLAG_VALID;
        if (entry->texture != NULL) {
            ((void(*)(void*, u32))fn_80118C20)(entry->texture, 0);
        }
    }
}
#endif

/* fn_800E407C | Size: 0xF4 */
extern u8 lbl_8047CB70[];
#if 1
asm void fn_800E407C(void) {
#include "src/game/gs_material_fn_800E407C.inc"
}
#else
void fn_800E407C(void) {
    /* GSmaterial internal (0xF4 bytes) */
}
#endif

/* fn_800E4170 -- SetupAlphaBlend | Size: 0x234 */
#if 1
asm void fn_800E4170(void) {
#include "src/game/gs_material_fn_800E4170.inc"
}
#else
void fn_800E4170(void) {
    /* SetupAlphaBlend (0x234 bytes) */
}
#endif

/* fn_800E43A4 -- SetupZMode | Size: 0x170 */
extern u8 lbl_80270E50[];
#if 1
asm void fn_800E43A4(void) {
#include "src/game/gs_material_fn_800E43A4.inc"
}
#else
void fn_800E43A4(void) {
    /* SetupZMode (0x170 bytes) */
}
#endif

/* fn_800E4514 | Size: 0x84 */
extern void fn_8019D9DC(void);
#if 1
asm void fn_800E4514(void) {
#include "src/game/gs_material_fn_800E4514.inc"
}
#else
void fn_800E4514(void) {
    /* GSmaterial internal (0x84 bytes) */
}
#endif

/* fn_800E4598 | Size: 0xC4 */
extern void fn_800A2D64(void);
extern u8 lbl_8047CB78[];
#if 1
asm void fn_800E4598(void) {
#include "src/game/gs_material_fn_800E4598.inc"
}
#else
void fn_800E4598(void) {
    /* GSmaterial internal (0xC4 bytes) */
}
#endif

/* fn_800E465C -- TEVStageSetup | Size: 0x464 */
#if 1
asm void fn_800E465C(void) {
#include "src/game/gs_material_fn_800E465C.inc"
}
#else
void fn_800E465C(void) {
    /* TEVStageSetup (0x464 bytes) */
}
#endif

/* fn_800E4AC0 -- TEVColorRegister | Size: 0x134 */
extern void fn_800E01F4(void);
extern u32 lbl_8047CB7C;
extern u32 lbl_8047CB80;
#if 1
asm void fn_800E4AC0(void) {
#include "src/game/gs_material_fn_800E4AC0.inc"
}
#else
void fn_800E4AC0(void) {
    /* TEVColorRegister (0x134 bytes) */
}
#endif

/* fn_800E4BF4 -- TEVSwapMode | Size: 0xA4 */
extern void fn_801A05EC(void);
extern void* memset(void* dst, int val, u32 n);
#if 1
asm void fn_800E4BF4(void) {
#include "src/game/gs_material_fn_800E4BF4.inc"
}
#else
void fn_800E4BF4(void) {
    /* TEVSwapMode (0xA4 bytes) */
}
#endif

/* fn_800E4C98 | Size: 0x80 */
extern f32 lbl_8047CB7C;
extern f32 lbl_8047CB80;
#if 0
asm void fn_800E4C98(void) {
#include "src/game/gs_material_fn_800E4C98.inc"
}
#else
void fn_800E4C98(void* entry) {
    struct {
        u32 pad0;
        u32 flags;
        void* ptr;
        u32 pad1;
        u32 pad2;
        f32 f0, f1, f2;
        f32 f3, f4, f5;
        f32 f6, f7, f8;
        u32 pad3;
        u32 pad4;
    } desc;
    void* inner = *(void**)((u8*)entry + 0x4);
    void* obj = *(void**)inner;
    desc.pad0  = 0;
    desc.flags = *(u32*)((u8*)obj + 0x4) | 0x1000;
    desc.ptr   = obj;
    desc.pad1  = 0;
    desc.pad2  = 0;
    desc.f0 = desc.f1 = desc.f2 = lbl_8047CB7C;
    desc.f3 = desc.f4 = desc.f5 = lbl_8047CB80;
    desc.f6 = desc.f7 = desc.f8 = lbl_8047CB7C;
    desc.pad3  = 0;
    desc.pad4  = 0;
    ((void(*)(void*, void*))fn_800E51A4)(inner, &desc);
}
#endif

/* fn_800E4D18 | Size: 0x24 */
#if 0
asm void fn_800E4D18(void) {
#include "src/game/gs_material_fn_800E4D18.inc"
}
#else
void fn_800E4D18(GSmaterialEntry* entry) {
    ((void(*)(GSmaterialEntry*, u32))fn_800E51A4)(entry, entry->flags);
}
#endif

/* fn_800E4D3C | Size: 0x74 */
extern u32 lbl_8047AB78;
extern u16 lbl_8047AB70;
extern u32 lbl_8047AB74;
#if 0
asm void fn_800E4D3C(void) {
#include "src/game/gs_material_fn_800E4D3C.inc"
}
#else
void fn_800E4D3C(u32 count) {
    u16 handle;
    u32 i;
    gsMatPoolCount = count;
    handle = fn_800E3534(count * 0x170);
    lbl_8047AB70 = handle;
    if (handle == 0) {
        return;
    }
    gsMatPool = fn_800E27B0(handle);
    for (i = 0; i < gsMatPoolCount; i++) {
        *(u32*)((u8*)gsMatPool + i * 0x170) = 0;
    }
    fn_800E92D8();
}
#endif

/* fn_800E4DB0 | Size: 0xDC */
#if 1
asm void fn_800E4DB0(void) {
#include "src/game/gs_material_fn_800E4DB0.inc"
}
#else
void fn_800E4DB0(void) {
    /* GSmaterial internal (0xDC bytes) */
}
#endif

/* fn_800E4E8C | Size: 0x21C */
#if 1
asm void fn_800E4E8C(void) {
#include "src/game/gs_material_fn_800E4E8C.inc"
}
#else
void fn_800E4E8C(void) {
    /* GSmaterial internal (0x21C bytes) */
}
#endif

/* fn_800E50A8 | Size: 0xE0 */
#if 1
asm void fn_800E50A8(void) {
#include "src/game/gs_material_fn_800E50A8.inc"
}
#else
void fn_800E50A8(void) {
    /* GSmaterial internal (0xE0 bytes) */
}
#endif

/* fn_800E5188 | Size: 0x1C */
#if 0
asm void fn_800E5188(void) {
#include "src/game/gs_material_fn_800E5188.inc"
}
#else
void* fn_800E5188(GSmaterialEntry* entry) {
    if (entry->flags & GSMAT_FLAG_RENDERTYPE) {
        return entry->mobjSecondary;
    }
    return entry->mobjPrimary;
}
#endif

/* fn_800E51A4 -- TEVMultiStageSetup | Size: 0x3AC */
extern void fn_801A0FBC(void);
extern void fn_8019147C(void);
extern void fn_80191474(void);
extern void fn_8019146C(void);
extern void fn_8019F1C4(void);
extern u32 lbl_8047AB78;
extern u32 lbl_8047AB74;
extern u32 lbl_8047CB84;
extern u32 lbl_8047CB7C;
#if 1
asm void fn_800E51A4(void) {
#include "src/game/gs_material_fn_800E51A4.inc"
}
#else
void fn_800E51A4(void) {
    /* TEVMultiStageSetup (0x3AC bytes) */
}
#endif

/* fn_800E5550 | Size: 0xEC */
extern void fn_800DEFC8(void);
extern void fn_800DF608(void);
#if 1
asm void fn_800E5550(void) {
#include "src/game/gs_material_fn_800E5550.inc"
}
#else
void fn_800E5550(void) {
    /* GSmaterial internal (0xEC bytes) */
}
#endif

/* fn_800E563C | Size: 0x154 */
extern void fn_800EE758(void);
extern void fn_800EE6B4(void);
extern void fn_800DF028(void);
#if 1
asm void fn_800E563C(void) {
#include "src/game/gs_material_fn_800E563C.inc"
}
#else
void fn_800E563C(void) {
    /* GSmaterial internal (0x154 bytes) */
}
#endif

/* fn_800E5790 | Size: 0xBC */
#if 1
asm void fn_800E5790(void) {
#include "src/game/gs_material_fn_800E5790.inc"
}
#else
void fn_800E5790(void) {
    /* GSmaterial internal (0xBC bytes) */
}
#endif

/* fn_800E584C | Size: 0x12C */
#if 1
asm void fn_800E584C(void) {
#include "src/game/gs_material_fn_800E584C.inc"
}
#else
void fn_800E584C(void) {
    /* GSmaterial internal (0x12C bytes) */
}
#endif

/* fn_800E5978 | Size: 0x50 */
extern void fn_800DF240(void);
#if 0
asm void fn_800E5978(void) {
#include "src/game/gs_material_fn_800E5978.inc"
}
#else
u32 fn_800E5978(void* p) {
    u16 handle = *(u16*)((u8*)p + 0x150);
    void* ptr;
    if (handle == 0) {
        return 0;
    }
    ptr = *(void**)((u8*)*(void**)((u8*)p + 0x14c));
    if (ptr == NULL) {
        return 0;
    }
    return ((u32(*)(void*))fn_800DF240)(ptr) & 1;
}
#endif

/* fn_800E59C8 | Size: 0xAC */
extern void fn_800DF11C(void);
#if 1
asm void fn_800E59C8(void) {
#include "src/game/gs_material_fn_800E59C8.inc"
}
#else
void fn_800E59C8(void) {
    /* GSmaterial internal (0xAC bytes) */
}
#endif

/* fn_800E5A74 | Size: 0xF4 */
extern void fn_800DF248(void);
#if 1
asm void fn_800E5A74(void) {
#include "src/game/gs_material_fn_800E5A74.inc"
}
#else
void fn_800E5A74(void) {
    /* GSmaterial internal (0xF4 bytes) */
}
#endif

/* fn_800E5B68 | Size: 0x78 */
extern void fn_800DF1E4(void);
#if 1
asm void fn_800E5B68(void) {
#include "src/game/gs_material_fn_800E5B68.inc"
}
#else
void fn_800E5B68(void) {
    /* GSmaterial internal (0x78 bytes) */
}
#endif

/* fn_800E5BE0 | Size: 0x160 */
extern void fn_800DF384(void);
#if 1
asm void fn_800E5BE0(void) {
#include "src/game/gs_material_fn_800E5BE0.inc"
}
#else
void fn_800E5BE0(void) {
    /* GSmaterial internal (0x160 bytes) */
}
#endif

/* fn_800E5D40 | Size: 0xF4 */
#if 1
asm void fn_800E5D40(void) {
#include "src/game/gs_material_fn_800E5D40.inc"
}
#else
void fn_800E5D40(void) {
    /* GSmaterial internal (0xF4 bytes) */
}
#endif

/* fn_800E5E34 | Size: 0x178 */
extern void fn_800DF208(void);
#if 1
asm void fn_800E5E34(void) {
#include "src/game/gs_material_fn_800E5E34.inc"
}
#else
void fn_800E5E34(void) {
    /* GSmaterial internal (0x178 bytes) */
}
#endif

/* fn_800E5FAC | Size: 0x50 */
#if 0
asm void fn_800E5FAC(void) {
#include "src/game/gs_material_fn_800E5FAC.inc"
}
#else
u32 fn_800E5FAC(void* p) {
    u16 handle = *(u16*)((u8*)p + 0x150);
    void* ptr;
    if (handle == 0) {
        return 0;
    }
    ptr = *(void**)((u8*)*(void**)((u8*)p + 0x14c));
    if (ptr == NULL) {
        return 0;
    }
    return (((u32(*)(void*))fn_800DF240)(ptr) >> 2) & 1;
}
#endif

/* fn_800E5FFC | Size: 0xF4 */
#if 1
asm void fn_800E5FFC(void) {
#include "src/game/gs_material_fn_800E5FFC.inc"
}
#else
void fn_800E5FFC(void) {
    /* GSmaterial internal (0xF4 bytes) */
}
#endif

/* fn_800E60F0 | Size: 0xCC */
extern void fn_800DF1B8(void);
#if 1
asm void fn_800E60F0(void) {
#include "src/game/gs_material_fn_800E60F0.inc"
}
#else
void fn_800E60F0(void) {
    /* GSmaterial internal (0xCC bytes) */
}
#endif

/* fn_800E61BC | Size: 0x1D0 */
extern void fn_800DF1D0(void);
#if 1
asm void fn_800E61BC(void) {
#include "src/game/gs_material_fn_800E61BC.inc"
}
#else
void fn_800E61BC(void) {
    /* GSmaterial internal (0x1D0 bytes) */
}
#endif

/* fn_800E638C | Size: 0xEC */
extern void fn_800DF470(void);
#if 1
asm void fn_800E638C(void) {
#include "src/game/gs_material_fn_800E638C.inc"
}
#else
void fn_800E638C(void) {
    /* GSmaterial internal (0xEC bytes) */
}
#endif

/* fn_800E6478 | Size: 0x154 */
extern void fn_800DF498(void);
#if 1
asm void fn_800E6478(void) {
#include "src/game/gs_material_fn_800E6478.inc"
}
#else
void fn_800E6478(void) {
    /* GSmaterial internal (0x154 bytes) */
}
#endif

/* fn_800E65CC | Size: 0xEC */
extern void fn_800DF140(void);
#if 1
asm void fn_800E65CC(void) {
#include "src/game/gs_material_fn_800E65CC.inc"
}
#else
void fn_800E65CC(void) {
    /* GSmaterial internal (0xEC bytes) */
}
#endif

/* fn_800E66B8 | Size: 0x14C */
extern void fn_800DF188(void);
#if 1
asm void fn_800E66B8(void) {
#include "src/game/gs_material_fn_800E66B8.inc"
}
#else
void fn_800E66B8(void) {
    /* GSmaterial internal (0x14C bytes) */
}
#endif

/* fn_800E6804 | Size: 0xD4 */
extern void fn_800DF21C(void);
extern u32 lbl_8047CB90;
extern u32 lbl_8047CB88;
#if 1
asm void fn_800E6804(void) {
#include "src/game/gs_material_fn_800E6804.inc"
}
#else
void fn_800E6804(void) {
    /* GSmaterial internal (0xD4 bytes) */
}
#endif

/* fn_800E68D8 | Size: 0xEC */
extern void fn_800DF504(void);
#if 1
asm void fn_800E68D8(void) {
#include "src/game/gs_material_fn_800E68D8.inc"
}
#else
void fn_800E68D8(void) {
    /* GSmaterial internal (0xEC bytes) */
}
#endif

/* fn_800E69C4 | Size: 0x15C */
extern void fn_800DF3F0(void);
extern void fn_800DF550(void);
#if 1
asm void fn_800E69C4(void) {
#include "src/game/gs_material_fn_800E69C4.inc"
}
#else
void fn_800E69C4(void) {
    /* GSmaterial internal (0x15C bytes) */
}
#endif

/* fn_800E6B20 | Size: 0xA8 */
extern void fn_801A02B0(void);
#if 1
asm void fn_800E6B20(void) {
#include "src/game/gs_material_fn_800E6B20.inc"
}
#else
void fn_800E6B20(void) {
    /* GSmaterial internal (0xA8 bytes) */
}
#endif

/* fn_800E6BC8 | Size: 0x1F8 */
extern void fn_8019F718(void);
extern void fn_801A015C(void);
extern void fn_8019FF74(void);
extern void fn_8019FF30(void);
extern void fn_8019FE8C(void);
extern void fn_800E090C(void);
extern u32 lbl_8047CB98;
extern u8 lbl_8047CB9C[];
extern u8 lbl_8047CBA4[];
extern u8 lbl_80270E60[];
#if 1
asm void fn_800E6BC8(void) {
#include "src/game/gs_material_fn_800E6BC8.inc"
}
#else
void fn_800E6BC8(void) {
    /* GSmaterial internal (0x1F8 bytes) */
}
#endif

/* fn_800E6DC0 | Size: 0xC */
#if 0
asm void fn_800E6DC0(void) {
#include "src/game/gs_material_fn_800E6DC0.inc"
}
#else
u32 fn_800E6DC0(void* p) {
    return (*(u32*)p >> 17) & 1;
}
#endif

/* fn_800E6DCC -- TEV descriptor builder | Size: 0x4C4 */
extern u8 lbl_8047CBAC[];
extern u8 lbl_80270E6C[];
#if 1
asm void fn_800E6DCC(void) {
#include "src/game/gs_material_fn_800E6DCC.inc"
}
#else
void fn_800E6DCC(void) {
    /* TEV descriptor builder (0x4C4 bytes) */
}
#endif

/* fn_800E7290 -- TEV stage count | Size: 0x9C */
extern void fn_800E0204(void);
#if 1
asm void fn_800E7290(void) {
#include "src/game/gs_material_fn_800E7290.inc"
}
#else
void fn_800E7290(void) {
    /* TEV stage count (0x9C bytes) */
}
#endif

/* fn_800E732C -- FULL TEV PIPELINE | Size: 0x12BC */
extern u32 lbl_8047CBB4;
extern u32 lbl_8047CBB8;
#if 1
asm void fn_800E732C(void) {
#include "src/game/gs_material_fn_800E732C.inc"
}
#else
void fn_800E732C(void) {
    /* FULL TEV PIPELINE (0x12BC bytes) */
}
#endif

/* fn_800E85E8 -- TEV post-config | Size: 0x9C */
#if 1
asm void fn_800E85E8(void) {
#include "src/game/gs_material_fn_800E85E8.inc"
}
#else
void fn_800E85E8(void) {
    /* TEV post-config (0x9C bytes) */
}
#endif

/* fn_800E8684 -- LIGHTING SETUP | Size: 0x878 */
extern void fn_80190E34(void);
extern void fn_800CDA74(void);
extern void fn_800C46B0(void);
extern void fn_801B06DC(void);
extern void fn_801B07D4(void);
extern void fn_800E3D14(void);
extern void fn_801943BC(void);
extern void fn_800E00AC(void);
extern void fn_80195970(void);
extern void fn_801B0A98(void);
extern void fn_801944E8(void);
extern void fn_801944C0(void);
extern void fn_80195794(void);
extern void fn_801B073C(void);
extern void fn_80195904(void);
extern void fn_801950D0(void);
extern void fn_801B0408(void);
extern void fn_800E3B6C(void);
extern void fn_801B019C(void);
extern void fn_801B03A0(void);
extern void fn_801B04E0(void);
extern void fn_801B1524(void);
extern void fn_8019C708(void);
extern void fn_801B16C0(void);
extern void fn_801B0EB8(void);
extern void fn_801B0BD8(void);
extern void fn_8019C6FC(void);
extern u32 lbl_8047AB74;
extern u32 lbl_8047CBC0;
extern u32 lbl_8047CBC4;
extern u32 lbl_8047AB78;
extern u32 lbl_8047CBC8;
extern u32 lbl_8047CBE0;
extern u32 lbl_8047CBCC;
extern u32 lbl_8047CBD0;
extern u32 lbl_8047CBD4;
extern u8 lbl_80478AC0[];
extern u32 lbl_8047AB80;
extern u32 lbl_8047AB84;
extern u32 lbl_8047AB94;
extern u8 lbl_80270E98[];
extern u8 lbl_8047CBD8[];
extern u32 lbl_8047AB90;
extern u32 lbl_8047AB8C;
#if 1
asm void fn_800E8684(void) {
#include "src/game/gs_material_fn_800E8684.inc"
}
#else
void fn_800E8684(void) {
    /* LIGHTING SETUP (0x878 bytes) */
}
#endif

/* fn_800E8EFC -- RenderStateReset | Size: 0x6C */
extern void fn_801B06DC(void);
extern void fn_801B0880();
#if 0
asm void fn_800E8EFC(void) {
#include "src/game/gs_material_fn_800E8EFC.inc"
}
#else
void fn_800E8EFC(void) {
    u8* slot = lbl_80401490;
    s32 i;
    for (i = 0; i < 6; i++) {
        fn_801B06DC(*(u32*)(slot + 0x54));
        fn_801B0880(*(u32*)(slot + 0x54), 0);
        *(u8*)(slot + 0x50) = 0;
        slot += 0x58;
    }
}
#endif

/* fn_800E8F74 -- SetDistanceThreshold | Size: 0xC */
extern u32 lbl_8047AB88;
#if 0
asm void fn_800E8F74(void) {
#include "src/game/gs_material_fn_800E8F74.inc"
}
#else
void fn_800E8F74(f32 dist) {
    gsMatDistThresholdSq = dist * dist;
}
#endif

/* fn_800E8F80 | Size: 0x20 */
extern void fn_801B06D4(void);
#if 0
asm void fn_800E8F80(void) {
#include "src/game/gs_material_fn_800E8F80.inc"
}
#else
void fn_800E8F80(void) {
    fn_801B06D4();
}
#endif

/* fn_800E8FA0 | Size: 0x48 */
extern u32 lbl_8047AB90;
extern u32 lbl_8047AB8C;
#if 0
asm void fn_800E8FA0(void) {
#include "src/game/gs_material_fn_800E8FA0.inc"
}
#else
void fn_800E8FA0(s32 w, s32 h) {
    if (w & 1) w++;
    if (h & 1) h++;
    if (w < 2) return;
    if (h < 2) return;
    if (w > 0x280) return;
    if (h > 0x1e0) return;
    lbl_8047AB90 = (u32)w;
    lbl_8047AB8C = (u32)h;
}
#endif

/* fn_800E8FE8 | Size: 0x24 */
#if 0
asm void fn_800E8FE8(void) {
#include "src/game/gs_material_fn_800E8FE8.inc"
}
#else
void fn_800E8FE8(void* p, void* obj) {
    if (obj != NULL) {
        void* inner = *(void**)((u8*)obj + 0xc);
        u16 val = *(u16*)((u8*)inner + 0x8);
        if (val & 3) {
            obj = NULL;
        }
    }
    *(void**)((u8*)p + 0x160) = obj;
}
#endif

/* fn_800E900C -- Distance check | Size: 0xBC */
#if 1
asm void fn_800E900C(void) {
#include "src/game/gs_material_fn_800E900C.inc"
}
#else
void fn_800E900C(void) {
    /* Distance check (0xBC bytes) */
}
#endif

/* fn_800E90C8 | Size: 0x40 */
#if 0
asm void fn_800E90C8(void) {
#include "src/game/gs_material_fn_800E90C8.inc"
}
#else
void fn_800E90C8(void* p, u32 mask) {
    if (mask & 1) {
        *(u32*)p &= ~0x10000000;
    }
    if (mask & 2) {
        *(u32*)p &= ~0x20000000;
    }
    if (mask & 4) {
        *(u32*)p &= ~0x40000000;
    }
}
#endif

/* fn_800E9108 | Size: 0x40 */
#if 0
asm void fn_800E9108(void) {
#include "src/game/gs_material_fn_800E9108.inc"
}
#else
void fn_800E9108(void* p, u32 mask) {
    if (mask & 1) {
        *(u32*)p |= 0x10000000;
    }
    if (mask & 2) {
        *(u32*)p |= 0x20000000;
    }
    if (mask & 4) {
        *(u32*)p |= 0x40000000;
    }
}
#endif

/* fn_800E9148 -- CheckRenderSlot | Size: 0x140 */
extern void fn_801A3918();
#if 0
asm void fn_800E9148(void) {
#include "src/game/gs_material_fn_800E9148.inc"
}
#else
void fn_800E9148(void* entry, u8 enable) {
    u8* slot = lbl_80401490;
    u8* s;
    u32 found = 0;
    s32 i;
    /* Unrolled 6-slot search */
    if (*(void**)slot == entry) {
        found = 1;
    } else {
        s = slot + 0x58;
        if (*(void**)s == entry) {
            found = 1;
        } else if (*(void**)(s + 0x58) == entry) {
            found = 1;
        } else if (*(void**)(s + 0xB0) == entry) {
            found = 1;
        } else if (*(void**)(s + 0x108) == entry) {
            found = 1;
        } else if (*(void**)(s + 0x160) == entry) {
            found = 1;
        }
    }
    if (found) {
        void* mobj = ((void*(*)(void*))fn_800E5188)(entry);
        if (enable == 0) {
            fn_801A3918(mobj, fn_800E9358, 0);
        } else {
            fn_801A3918(mobj, fn_800E9358, 1);
        }
        for (i = 0; i < 6; i++, slot += 0x58) {
            if (*(u8*)(slot + 0x50) != 0 && *(void**)slot == entry) {
                fn_801B0880(*(u32*)(slot + 0x54), (u32)enable);
            }
        }
    }
}
#endif

/* fn_800E9288 | Size: 0x50 */
#if 0
asm void fn_800E9288(void) {
#include "src/game/gs_material_fn_800E9288.inc"
}
#else
void fn_800E9288(GSmaterialEntry* entry) {
    u16 handle = *(u16*)((u8*)entry + 0x164);
    if (handle != 0) {
        ((void*(*)(u16))fn_800E24B0)(handle);
        handle = *(u16*)((u8*)entry + 0x164);
        ((void(*)(u16))fn_800E209C)(handle);
        *(u16*)((u8*)entry + 0x164) = 0;
        *(u32*)((u8*)entry + 0x158) = 0;
        *(u32*)((u8*)entry + 0x15c) = 0;
    }
}
#endif

/* fn_800E92D8 | Size: 0x80 */
extern u32 fn_801B1730(void);
extern f32 lbl_8047CBC8;
extern u8  lbl_8047AB94;
extern u32 lbl_8047AB90;
extern u32 lbl_8047AB8C;
extern f32 lbl_8047AB88;
extern u32 lbl_8047AB84;
#if 0
asm void fn_800E92D8(void) {
#include "src/game/gs_material_fn_800E92D8.inc"
}
#else
void fn_800E92D8(void) {
    u8* slot = lbl_80401490;
    s32 i;
    lbl_8047AB94 = 0x80;
    lbl_8047AB90 = 0x180;
    lbl_8047AB8C = 0x180;
    lbl_8047AB88 = lbl_8047CBC8;
    lbl_8047AB84 = 0;
    for (i = 0; i < 6; i++) {
        u32 val = fn_801B1730();
        *(u32*)(slot + 0x54) = val;
        fn_801B0880(*(u32*)(slot + 0x54), 0);
        slot += 0x58;
    }
}
#endif

/* fn_800E9358 | Size: 0x60 */
#if 0
asm void fn_800E9358(void) {
#include "src/game/gs_material_fn_800E9358.inc"
}
#else
void fn_800E9358(void* entry, u8 enable) {
    void* node;
    if (*(u32*)((u8*)entry + 0x14) & 0x4020) {
        return;
    }
    node = *(void**)((u8*)entry + 0x18);
    while (node != NULL) {
        void* child = *(void**)((u8*)node + 0x8);
        if (child != NULL) {
            u32* flags = (u32*)((u8*)child + 0x4);
            if (enable) {
                *flags |= 0x04000000;
            } else {
                *flags &= ~0x04000000;
            }
        }
        node = *(void**)((u8*)node + 0x4);
    }
}
#endif

/* fn_800E93B8 -- SetupEnvMap | Size: 0x5E0 */
extern u32 lbl_8047CBC0;
#if 1
asm void fn_800E93B8(void) {
#include "src/game/gs_material_fn_800E93B8.inc"
}
#else
void fn_800E93B8(void) {
    /* SetupEnvMap (0x5E0 bytes) */
}
#endif

/* fn_800E9998 -- UpdateEnvMap | Size: 0x194 */
extern void fn_800E0020(void);
extern u32 lbl_8047CBC0;
extern u32 lbl_8047AB88;
extern u32 lbl_8047CBC8;
#if 1
asm void fn_800E9998(void) {
#include "src/game/gs_material_fn_800E9998.inc"
}
#else
void fn_800E9998(void) {
    /* UpdateEnvMap (0x194 bytes) */
}
#endif

/* fn_800E9B2C | Size: 0x140 */
#if 1
asm void fn_800E9B2C(void) {
#include "src/game/gs_material_fn_800E9B2C.inc"
}
#else
void fn_800E9B2C(void) {
    /* GSmaterial internal (0x140 bytes) */
}
#endif

/* fn_800E9C6C | Size: 0x1C8 */
extern void fn_800E3D00(void);
extern void fn_800E3CF8(void);
extern void fn_800E3CF0(void);
extern u8 lbl_8047CBE8[];
extern u8 lbl_8047CBF0[];
extern u8 lbl_80270EA8[];
extern u8 lbl_8047CBF8[];
#if 1
asm void fn_800E9C6C(void) {
#include "src/game/gs_material_fn_800E9C6C.inc"
}
#else
void fn_800E9C6C(void) {
    /* GSmaterial internal (0x1C8 bytes) */
}
#endif

/* fn_800E9E34 | Size: 0x5C */
#if 0
asm void fn_800E9E34(void) {
#include "src/game/gs_material_fn_800E9E34.inc"
}
#else
void fn_800E9E34(GSmaterialEntry* entry, void* a, void* b, void* c) {
    void* mobj = fn_800E5188(entry);
    ((void(*)(void*, u32, u32, void*, void*, void*))fn_800E9E90)(mobj, 0, 7, a, b, c);
}
#endif

/* fn_800E9E90 -- EnvMap pipeline | Size: 0x77C */
extern void fn_800A2EB4(void);
extern void fn_800A2D98(void);
extern void fn_800E064C(void);
extern void fn_80197B6C(void);
extern void fn_8019F024(void);
extern void fn_801AB63C(void);
extern void fn_80199704(void);
extern u8 lbl_8047CC00[];
extern u8 lbl_8047CC08[];
extern u8 lbl_804016A0[];
#if 1
asm void fn_800E9E90(void) {
#include "src/game/gs_material_fn_800E9E90.inc"
}
#else
void fn_800E9E90(void) {
    /* EnvMap pipeline (0x77C bytes) */
}
#endif

/* fn_800EA60C | Size: 0x58 */
#if 0
asm void fn_800EA60C(void) {
#include "src/game/gs_material_fn_800EA60C.inc"
}
#else
s32 fn_800EA60C(void* obj) {
    u32 flags;
    if (obj == NULL) {
        fn_80196E10(lbl_8047CC00, 0x25d, lbl_8047CC08);
    }
    flags = *(u32*)((u8*)obj + 0x14);
    if (flags & 0x800000) {
        return 0;
    }
    if (flags & 0x40) {
        return 1;
    }
    return 0;
}
#endif

/* fn_800EA664 | Size: 0x70 */
#if 1
asm void fn_800EA664(void) {
#include "src/game/gs_material_fn_800EA664.inc"
}
#else
void fn_800EA664(void) {
    /* GSmaterial internal (0x70 bytes) */
}
#endif

/* fn_800EA6D4 -- BindTextureToStage | Size: 0x110 */
#if 1
asm void fn_800EA6D4(void) {
#include "src/game/gs_material_fn_800EA6D4.inc"
}
#else
void fn_800EA6D4(void) {
    /* BindTextureToStage (0x110 bytes) */
}
#endif

/* fn_800EA7E4 | Size: 0x3C */
#if 0
asm void fn_800EA7E4(void) {
#include "src/game/gs_material_fn_800EA7E4.inc"
}
#else
void fn_800EA7E4(void* obj) {
    if (obj != NULL && !(*(u32*)((u8*)obj + 0x14) & 0x4020)) {
        ((void(*)(void*))fn_800EA820)(obj);
    }
}
#endif

/* fn_800EA820 | Size: 0x140 */
#if 1
asm void fn_800EA820(void) {
#include "src/game/gs_material_fn_800EA820.inc"
}
#else
void fn_800EA820(void) {
    /* GSmaterial internal (0x140 bytes) */
}
#endif

/* fn_800EA960 -- ConfigureTEVStage | Size: 0x370 */
extern void fn_8019F01C(void);
extern void fn_801AB538(void);
extern void fn_801AB5F8(void);
extern void fn_800E0628(void);
extern u8 lbl_804016D0[];
#if 1
asm void fn_800EA960(void) {
#include "src/game/gs_material_fn_800EA960.inc"
}
#else
void fn_800EA960(void) {
    /* ConfigureTEVStage (0x370 bytes) */
}
#endif

/* fn_800EACD0 -- ConfigureBlend | Size: 0x314 */
#if 1
asm void fn_800EACD0(void) {
#include "src/game/gs_material_fn_800EACD0.inc"
}
#else
void fn_800EACD0(void) {
    /* ConfigureBlend (0x314 bytes) */
}
#endif

/* fn_800EAFE4 -- ConfigureZMode | Size: 0x284 */
extern void fn_80197A64(void);
extern void fn_801A85F0(void);
extern u8 lbl_80270EB8[];
extern u32 lbl_8047CC18;
extern u8 lbl_8047CC10[];
extern u32 lbl_8047CC1C;
extern u8 lbl_8047CC20[];
#if 1
asm void fn_800EAFE4(void) {
#include "src/game/gs_material_fn_800EAFE4.inc"
}
#else
void fn_800EAFE4(void) {
    /* ConfigureZMode (0x284 bytes) */
}
#endif

/* fn_800EB268 -- ConfigureAlpha | Size: 0xD8 */
extern void fn_80191460(void);
extern u32 lbl_8047CC28;
extern u8 lbl_8047CC2C[];
extern u8 lbl_8047CC34[];
#if 1
asm void fn_800EB268(void) {
#include "src/game/gs_material_fn_800EB268.inc"
}
#else
void fn_800EB268(void) {
    /* ConfigureAlpha (0xD8 bytes) */
}
#endif

/* fn_800EB340 | Size: 0xD4 */
extern u32 lbl_8047CC28;
#if 1
asm void fn_800EB340(void) {
#include "src/game/gs_material_fn_800EB340.inc"
}
#else
void fn_800EB340(void) {
    /* GSmaterial internal (0xD4 bytes) */
}
#endif

/* fn_800EB414 | Size: 0x50 */
extern void fn_800DA578(void);
extern u8 lbl_80315598[];
#if 0
asm void fn_800EB414(void) {
#include "src/game/gs_material_fn_800EB414.inc"
}
#else
void fn_800EB414(void* p, void* a2, void* a3, void* a4, void* a5, void* a6) {
    void* args[3];
    args[0] = a6;
    args[1] = a3;
    args[2] = a4;
    ((void(*)(void*, void*, u32, u8*, void*, void*))fn_800DA578)(
        *(void**)((u8*)p + 0x8),
        *(void**)((u8*)p + 0x10),
        (u32)(*(u16*)((u8*)p + 0xe)) << 5,
        lbl_80315598,
        args,
        p
    );
}
#endif

/* fn_800EB464 | Size: 0xBC */
extern void fn_800DFF98(void);
extern void fn_800E3C5C(void);
extern void fn_80191358(void);
extern u32 lbl_8047AB98;
#if 1
asm void fn_800EB464(void) {
#include "src/game/gs_material_fn_800EB464.inc"
}
#else
void fn_800EB464(void) {
    /* GSmaterial internal (0xBC bytes) */
}
#endif

/* fn_800EB520 | Size: 0x8 */
#if 0
asm void fn_800EB520(void) {
#include "src/game/gs_material_fn_800EB520.inc"
}
#else
void fn_800EB520(void* a, void* b, u32 c, void* d) {
    *(u32*)((u8*)d + 0xc) = c;
}
#endif

/* fn_800EB528 | Size: 0x78 */
#if 0
asm void fn_800EB528(void) {
#include "src/game/gs_material_fn_800EB528.inc"
}
#else
void fn_800EB528(void* entry) {
    void* src;
    if (*(void**)((u8*)entry + 0xc) != NULL) {
        return;
    }
    if (*(u32*)entry & 0x20000) {
        return;
    }
    src = *(void**)*(void**)((u8*)entry + 0x4);
    *(u32*)((u8*)entry + 0xc) = (u32)((void*(*)(void*))fn_801A0FBC)(src);
    src = *(void**)*(void**)((u8*)entry + 0x4);
    *(u32*)((u8*)entry + 0x10) = (u32)((void*(*)(void*))fn_801A0FBC)(src);
    src = *(void**)*(void**)((u8*)entry + 0x4);
    *(u32*)((u8*)entry + 0x14) = (u32)((void*(*)(void*))fn_801A0FBC)(src);
    ((void(*)(void*))fn_800EBEEC)(*(void**)((u8*)entry + 0xc));
}
#endif

/* fn_800EB5A0 | Size: 0x140 */
extern void fn_800E0560(void);
extern void fn_800E053C(void);
extern void fn_800E0518(void);
extern void fn_800E04F4(void);
extern void fn_800E042C(void);
extern void fn_800E0290(void);
extern u32 lbl_8047ABA0;
extern u8 lbl_8047CC40[];
extern u8 lbl_8047CC48[];
#if 1
asm void fn_800EB5A0(void) {
#include "src/game/gs_material_fn_800EB5A0.inc"
}
#else
void fn_800EB5A0(void) {
    /* GSmaterial internal (0x140 bytes) */
}
#endif

/* fn_800EB6E0 | Size: 0x224 */
#if 1
asm void fn_800EB6E0(void) {
#include "src/game/gs_material_fn_800EB6E0.inc"
}
#else
void fn_800EB6E0(void) {
    /* GSmaterial internal (0x224 bytes) */
}
#endif

/* fn_800EB904 -- Large render setup | Size: 0x5E8 */
extern void fn_801ADAAC(void);
extern void fn_801AD7CC(void);
extern u32 lbl_8047CC50;
extern u32 lbl_8047ABA0;
#if 1
asm void fn_800EB904(void) {
#include "src/game/gs_material_fn_800EB904.inc"
}
#else
void fn_800EB904(void) {
    /* Large render setup (0x5E8 bytes) */
}
#endif

/* fn_800EBEEC -- Render config | Size: 0x1FC */
#if 1
asm void fn_800EBEEC(void) {
#include "src/game/gs_material_fn_800EBEEC.inc"
}
#else
void fn_800EBEEC(void) {
    /* Render config (0x1FC bytes) */
}
#endif

/* fn_800EC0E8 -- SetAlpha | Size: 0x4C */
#if 0
asm void fn_800EC0E8(void) {
#include "src/game/gs_material_fn_800EC0E8.inc"
}
#else
void fn_800EC0E8(GSmaterialEntry* entry) {
    u32 flags = entry->flags;
    if (!(flags & 0x10000) || !(flags & 0x1000)) {
        ((void(*)(GSmaterialEntry*))fn_800ED1CC)(entry);
        ((void(*)(GSmaterialEntry*))fn_800ED4D4)(entry);
    }
}
#endif

/* fn_800EC134 -- UpdateMObjColor | Size: 0x20 */
void fn_800EC134_impl(void) {
    /* GSmaterialUpdateMObjColor: Push colors to HSD MObj (0x20 bytes) */
}

/* fn_800EC154 -- GetMObjPtr | Size: 0xC */
#if 0
asm void fn_800EC154(void) {
#include "src/game/gs_material_fn_800EC154.inc"
}
#else
void fn_800EC154(void* p, void* a, void* b) {
    *(u32*)((u8*)p + 0xdc) = (u32)a;
    *(u32*)((u8*)p + 0xe0) = (u32)b;
}
#endif

/* fn_800EC160 -- SetDiffuseRGBA | Size: 0x28 */
#if 0
asm void fn_800EC160(void) {
#include "src/game/gs_material_fn_800EC160.inc"
}
#else
void fn_800EC160(void* p, u8 enable) {
    if (enable) {
        *(u32*)p |= 0x800;
    } else {
        *(u32*)p &= ~0x800;
    }
}
#endif

/* fn_800EC188 -- SetAmbientRGBA | Size: 0x28 */
#if 0
asm void fn_800EC188(void) {
#include "src/game/gs_material_fn_800EC188.inc"
}
#else
void fn_800EC188(void* p, u8 enable) {
    if (enable) {
        *(u32*)p |= 0x2000;
    } else {
        *(u32*)p &= ~0x2000;
    }
}
#endif

/* fn_800EC1B0 -- GetDiffuseR | Size: 0xC */
#if 0
asm void fn_800EC1B0(void) {
#include "src/game/gs_material_fn_800EC1B0.inc"
}
#else
u32 fn_800EC1B0(void* p) {
    return (*(u32*)p >> 3) & 1;
}
#endif

/* fn_800EC1BC -- GetDiffuseG | Size: 0xC */
#if 0
asm void fn_800EC1BC(void) {
#include "src/game/gs_material_fn_800EC1BC.inc"
}
#else
u32 fn_800EC1BC(void* p) {
    return (*(u32*)p >> 2) & 1;
}
#endif

/* fn_800EC1C8 -- GetDiffuseB | Size: 0xC */
#if 0
asm void fn_800EC1C8(void) {
#include "src/game/gs_material_fn_800EC1C8.inc"
}
#else
u32 fn_800EC1C8(void* p) {
    return (*(u32*)p >> 15) & 1;
}
#endif

/* fn_800EC1D4 -- GetDiffuseA | Size: 0x10 */
#if 0
asm void fn_800EC1D4(void) {
#include "src/game/gs_material_fn_800EC1D4.inc"
}
#else
void fn_800EC1D4(void* p) {
    *(u32*)p &= ~0x40;
}
#endif

/* fn_800EC1E4 | Size: 0x24 */
#if 0
asm void fn_800EC1E4(void) {
#include "src/game/gs_material_fn_800EC1E4.inc"
}
#else
void fn_800EC1E4(void* p) {
    u32 flags = *(u32*)p;
    if (!(flags & 0x8)) {
        return;
    }
    *(u32*)p = flags | 0x40;
    *(u32*)p &= ~0x8000;
}
#endif

/* fn_800EC208 | Size: 0x9C */
extern void fn_801C028C(void);
#if 1
asm void fn_800EC208(void) {
#include "src/game/gs_material_fn_800EC208.inc"
}
#else
void fn_800EC208(void) {
    /* GSmaterial internal (0x9C bytes) */
}
#endif

/* fn_800EC2A4 | Size: 0x64 */
extern void fn_801A32A0();
#if 0
asm void fn_800EC2A4(void) {
#include "src/game/gs_material_fn_800EC2A4.inc"
}
#else
void fn_800EC2A4(void* entry, f32 val) {
    u32 flags = *(u32*)entry;
    void* mobj = *(void**)((u8*)entry + 0x8);
    if (!(flags & 0x8)) {
        return;
    }
    if (flags & 0x20000) {
        mobj = *(void**)((u8*)mobj + 0x10);
    }
    *(f32*)((u8*)entry + 0xb0) = val;
    *(f32*)((u8*)entry + 0xb4) = val;
    fn_801A32A0(mobj, 0x634, *(f32*)((u8*)entry + 0xb4));
    flags = *(u32*)entry;
    *(u32*)entry = flags & ~0x8000;
}
#endif

/* fn_800EC308 | Size: 0x54 */
extern s32 fn_800D37CC(void);
extern f32 lbl_8047CC58;
#if 0
asm void fn_800EC308(void) {
#include "src/game/gs_material_fn_800EC308.inc"
}
#else
void fn_800EC308(void* entry, f32 val) {
    u32 flags = *(u32*)entry;
    if (!(flags & 0x8)) {
        return;
    }
    *(f32*)((u8*)entry + 0xac) = val;
    if (fn_800D37CC() == 0x32) {
        f32 stored = *(f32*)((u8*)entry + 0xac);
        *(f32*)((u8*)entry + 0xac) = stored * lbl_8047CC58;
    }
}
#endif

/* fn_800EC35C -- PE descriptor setup | Size: 0x174 */
extern void fn_801A2B5C();
extern f32 lbl_8047CC5C;
#if 0
asm void fn_800EC35C(void) {
#include "src/game/gs_material_fn_800EC35C.inc"
}
#else
void fn_800EC35C(void* entry, u32 idx) {
    u32 flags = *(u32*)entry;
    void* mobj2 = *(void**)((u8*)entry + 0x8);
    s32 mode;
    if (!(flags & 0x8)) {
        return;
    }
    if (idx >= *(u32*)((u8*)entry + 0x88)) {
        return;
    }
    if (idx != *(u32*)((u8*)entry + 0xa8)) {
        if (flags & 0x20000) {
            mobj2 = *(void**)((u8*)mobj2 + 0x10);
        }
        *(u32*)((u8*)entry + 0xa8) = idx;
        {
            void* tbl = *(void**)((u8*)*(void**)((u8*)entry + 0x4) + 0x8);
            void* pe = *(void**)((u8*)tbl + idx * 4);
            fn_801A2B5C(mobj2, 0, pe);
        }
        *(f32*)((u8*)entry + 0xb8) = lbl_8047CC5C;
        fn_801C028C(mobj2, 6, 0x64db, fn_800EE08C, 2, (u8*)entry + 0xb8);
    }
    flags = *(u32*)entry;
    mobj2 = *(void**)((u8*)entry + 0x8);
    if (flags & 0x8) {
        if (flags & 0x20000) {
            mobj2 = *(void**)((u8*)mobj2 + 0x10);
        }
        *(f32*)((u8*)entry + 0xb0) = lbl_8047CC5C;
        *(f32*)((u8*)entry + 0xb4) = lbl_8047CC5C;
        fn_801A32A0(mobj2, 0x634, *(f32*)((u8*)entry + 0xb4));
        *(u32*)entry = flags & ~0x8000;
    }
    flags = *(u32*)entry;
    mode = *(s32*)((u8*)entry + 0xa4);
    mobj2 = *(void**)((u8*)entry + 0x8);
    if (flags & 0x20000) {
        mobj2 = *(void**)((u8*)mobj2 + 0x10);
    }
    *(s32*)((u8*)entry + 0xa4) = mode;
    if (mode == 0) {
        fn_801C028C(mobj2, 6, 0x64db, fn_800EE054, 3, 0);
    } else if (mode == 1) {
        fn_801C028C(mobj2, 6, 0x64db, fn_800EE054, 3, 1);
    }
}
#endif

/* fn_800EC4D0 | Size: 0x6C */
extern u32 lbl_8047CC60;
extern u32 lbl_8047CC64;
#if 1
asm void fn_800EC4D0(void) {
#include "src/game/gs_material_fn_800EC4D0.inc"
}
#else
void fn_800EC4D0(void) {
    /* GSmaterial internal (0x6C bytes) */
}
#endif

/* fn_800EC53C | Size: 0x2C */
extern u32 lbl_8047CC5C;
#if 0
asm void fn_800EC53C(void) {
#include "src/game/gs_material_fn_800EC53C.inc"
}
#else
f32 fn_800EC53C(void* p) {
    u32 flags = *(u32*)p;
    if (!(flags & 0x4)) {
        return *(f32*)&lbl_8047CC5C;
    }
    if (flags & GSMAT_FLAG_RENDERTYPE) {
        return *(f32*)((u8*)p + 0xc8);
    }
    return *(f32*)((u8*)p + 0x98);
}
#endif

/* fn_800EC568 | Size: 0x8 */
#if 0
asm void fn_800EC568(void) {
#include "src/game/gs_material_fn_800EC568.inc"
}
#else
u32 fn_800EC568(void* p) {
    return *(u32*)((u8*)p + 0x8c);
}
#endif

/* fn_800EC570 | Size: 0x8 */
#if 0
asm void fn_800EC570(void) {
#include "src/game/gs_material_fn_800EC570.inc"
}
#else
f32 fn_800EC570(void* p) {
    return *(f32*)((u8*)p + 0x94);
}
#endif

/* fn_800EC578 | Size: 0x34 */
#if 0
asm void fn_800EC578(void) {
#include "src/game/gs_material_fn_800EC578.inc"
}
#else
void fn_800EC578(void* p, u32* outA, u32* outB) {
    if (!(*(u32*)p & GSMAT_FLAG_RENDERTYPE)) {
        *outA = *(u32*)((u8*)p + 0x90);
        *outB = (u32)-1;
    } else {
        *outA = *(u32*)((u8*)p + 0xbc);
        *outB = *(u32*)((u8*)p + 0xc0);
    }
}
#endif

/* fn_800EC5AC | Size: 0xC */
#if 0
asm void fn_800EC5AC(void) {
#include "src/game/gs_material_fn_800EC5AC.inc"
}
#else
u32 fn_800EC5AC(void* p) {
    return (*(u32*)p >> 7) & 1;
}
#endif

/* fn_800EC5B8 | Size: 0x44 */
extern u32 lbl_8047CC5C;
extern u32 lbl_8047CC60;
#if 0
asm void fn_800EC5B8(void) {
#include "src/game/gs_material_fn_800EC5B8.inc"
}
#else
void fn_800EC5B8(void* p, f32 val) {
    u32 flags = *(u32*)p;
    if (!(flags & 0x4)) {
        return;
    }
    if (!(flags & GSMAT_FLAG_RENDERTYPE)) {
        return;
    }
    {
        f32 lo = *(f32*)&lbl_8047CC5C;
        f32 hi = *(f32*)&lbl_8047CC60;
        if (val < lo) {
            val = lo;
        } else if (val > hi) {
            val = hi;
        }
    }
    *(f32*)((u8*)p + 0xd4) = val;
}
#endif

/* fn_800EC5FC -- RenderSetup | Size: 0x2CC */
extern void fn_801A1B40(void);
extern u32 lbl_8047CC5C;
extern u32 lbl_8047CC60;
#if 1
asm void fn_800EC5FC(void) {
#include "src/game/gs_material_fn_800EC5FC.inc"
}
#else
void fn_800EC5FC(void) {
    /* RenderSetup (0x2CC bytes) */
}
#endif

/* fn_800EC8C8 | Size: 0x14 */
extern u32 lbl_8047CC5C;
#if 0
asm void fn_800EC8C8(void) {
#include "src/game/gs_material_fn_800EC8C8.inc"
}
#else
void fn_800EC8C8(void* p, f32 a, f32 b) {
    *(f32*)((u8*)p + 0xd8) = *(f32*)&lbl_8047CC5C;
    *(f32*)((u8*)p + 0xc4) = a;
    *(f32*)((u8*)p + 0xc8) = b;
}
#endif

/* fn_800EC8DC | Size: 0x3C */
extern u32 lbl_8047AB74;
extern u32 lbl_8047AB78;
#if 0
asm void fn_800EC8DC(void) {
#include "src/game/gs_material_fn_800EC8DC.inc"
}
#else
void fn_800EC8DC(void) {
    u32 i;
    u32 count = gsMatPoolCount;
    for (i = 0; i < count; i++) {
        GSmaterialEntry* entry = (GSmaterialEntry*)((u8*)gsMatPool + i * 0x170);
        if (entry->flags & GSMAT_FLAG_ACTIVE) {
            entry->flags &= ~GSMAT_FLAG_ALPHATEST;
        }
    }
}
#endif

/* fn_800EC918 | Size: 0x3C */
extern u32 lbl_8047AB74;
extern u32 lbl_8047AB78;
#if 0
asm void fn_800EC918(void) {
#include "src/game/gs_material_fn_800EC918.inc"
}
#else
void fn_800EC918(void) {
    u32 i;
    u32 count = gsMatPoolCount;
    for (i = 0; i < count; i++) {
        GSmaterialEntry* entry = (GSmaterialEntry*)((u8*)gsMatPool + i * 0x170);
        if (entry->flags & GSMAT_FLAG_ACTIVE) {
            entry->flags |= GSMAT_FLAG_ALPHATEST;
        }
    }
}
#endif

/* fn_800EC954 | Size: 0xC */
#if 0
asm void fn_800EC954(void) {
#include "src/game/gs_material_fn_800EC954.inc"
}
#else
u32 fn_800EC954(void* p) {
    return (*(u32*)p >> 14) & 1;
}
#endif

/* fn_800EC960 | Size: 0xC */
#if 0
asm void fn_800EC960(void) {
#include "src/game/gs_material_fn_800EC960.inc"
}
#else
u32 fn_800EC960(void* p) {
    return (*(u32*)p >> 5) & 1;
}
#endif

/* fn_800EC96C | Size: 0x24 */
#if 0
asm void fn_800EC96C(void) {
#include "src/game/gs_material_fn_800EC96C.inc"
}
#else
void fn_800EC96C(void* p) {
    u32 flags;
    *(u32*)p &= ~0x20;
    flags = *(u32*)p;
    if (!(flags & GSMAT_FLAG_TWOSIDED)) {
        return;
    }
    *(u32*)p = flags & ~0x40;
}
#endif

/* fn_800EC990 | Size: 0x4C */
#if 0
asm void fn_800EC990(void) {
#include "src/game/gs_material_fn_800EC990.inc"
}
#else
void fn_800EC990(void* p) {
    u32 flags = *(u32*)p;
    if (!(flags & 0x4)) {
        return;
    }
    flags |= 0x20;
    *(u32*)p = flags;
    *(u32*)p = flags & ~0x4000;
    flags = *(u32*)p;
    if (!(flags & GSMAT_FLAG_TWOSIDED)) {
        return;
    }
    if (!(flags & 0x8)) {
        return;
    }
    flags |= 0x40;
    *(u32*)p = flags;
    *(u32*)p = flags & ~0x8000;
}
#endif

/* fn_800EC9DC | Size: 0x9C */
extern u32 lbl_8047CC58;
#if 0
asm void fn_800EC9DC(void) {
#include "src/game/gs_material_fn_800EC9DC.inc"
}
#else
void fn_800EC9DC(void* entry, f32 val) {
    u32 flags;
    if (*(u32*)entry & 0x4) {
        *(f32*)((u8*)entry + 0x94) = val;
        if (fn_800D37CC() == 0x32) {
            f32 stored = *(f32*)((u8*)entry + 0x94);
            *(f32*)((u8*)entry + 0x94) = stored * *(f32*)&lbl_8047CC58;
        }
    }
    flags = *(u32*)entry;
    if (!(flags & 0x2000)) {
        return;
    }
    if (!(flags & 0x8)) {
        return;
    }
    *(f32*)((u8*)entry + 0xac) = val;
    if (fn_800D37CC() == 0x32) {
        f32 stored = *(f32*)((u8*)entry + 0xac);
        *(f32*)((u8*)entry + 0xac) = stored * *(f32*)&lbl_8047CC58;
    }
}
#endif

/* fn_800ECA78 -- EnvMap alpha apply | Size: 0xFC */
void fn_800ECA78_impl(void) {
    /* GSmaterialEnvMapAlphaApply: Env-map alpha apply (0xFC bytes) */
}

/* fn_800ECB74 | Size: 0x134 */
#if 0
asm void fn_800ECB74(void) {
#include "src/game/gs_material_fn_800ECB74.inc"
}
#else
void fn_800ECB74(void* entry, s32 mode) {
    void* mobj = *(void**)((u8*)entry + 0x8);
    if (*(u32*)entry & 0x20000) {
        mobj = *(void**)((u8*)mobj + 0x10);
    }
    *(s32*)((u8*)entry + 0x8c) = mode;
    if (mode == 0) {
        fn_801C028C(mobj, 6, 0x9b2f, fn_800EE054, 3, 0);
    } else if (mode == 1) {
        fn_801C028C(mobj, 6, 0x9b2f, fn_800EE054, 3, 1);
    }
    {
        u32 flags = *(u32*)entry;
        if (flags & 0x2000) {
            void* mobj2 = *(void**)((u8*)entry + 0x8);
            if (flags & 0x20000) {
                mobj2 = *(void**)((u8*)mobj2 + 0x10);
            }
            *(s32*)((u8*)entry + 0xa4) = mode;
            if (mode == 0) {
                fn_801C028C(mobj2, 6, 0x64db, fn_800EE054, 3, 0);
            } else if (mode == 1) {
                fn_801C028C(mobj2, 6, 0x64db, fn_800EE054, 3, 1);
            }
        }
    }
}
#endif

/* fn_800ECCA8 -- ShadowSetup | Size: 0x428 */
extern u32 lbl_8047CC5C;
#if 1
asm void fn_800ECCA8(void) {
#include "src/game/gs_material_fn_800ECCA8.inc"
}
#else
void fn_800ECCA8(void) {
    /* ShadowSetup (0x428 bytes) */
}
#endif

/* fn_800ED0D0 -- PostRender | Size: 0xFC */
#if 0
asm void fn_800ED0D0(void) {
#include "src/game/gs_material_fn_800ED0D0.inc"
}
#else
void fn_800ED0D0(void* entry, f32 scale) {
    u32 flags = *(u32*)entry;
    if (flags & 0x100) {
        return;
    }
    if (flags & 0x4000) {
        ((void(*)(void*, u8))fn_800ED6E4)(entry, 0);
    }
    flags = *(u32*)entry;
    if (flags & 0x8000) {
        ((void(*)(void*, u8))fn_800ED6E4)(entry, 1);
    }
    flags = *(u32*)entry;
    if (flags & GSMAT_FLAG_RENDERTYPE) {
        if (flags & 0x20) {
            f32 val = *(f32*)((u8*)entry + 0x94) * scale;
            ((void(*)(void*, f32, u8))fn_800ED7E4)(entry, val, 0);
            *(f32*)((u8*)entry + 0xc4) = *(f32*)((u8*)entry + 0xc8) * *(f32*)((u8*)entry + 0xd8);
        }
    } else {
        if (flags & 0x20) {
            f32 val = *(f32*)((u8*)entry + 0x94) * scale;
            ((void(*)(void*, f32, u8))fn_800ED7E4)(entry, val, 0);
        }
        flags = *(u32*)entry;
        if (flags & 0x40) {
            f32 val = *(f32*)((u8*)entry + 0xac) * scale;
            ((void(*)(void*, f32, u8))fn_800ED7E4)(entry, val, 1);
        }
    }
    *(u32*)entry &= ~0x10000;
}
#endif

/* fn_800ED1CC | Size: 0x308 */
extern void fn_801C027C(void);
extern u8 lbl_8047CC68[];
extern u8 lbl_8047CC70[];
#if 0
asm void fn_800ED1CC(void) {
#include "src/game/gs_material_fn_800ED1CC.inc"
}
#else
void fn_800ED1CC(void* entry) {
    u32 flags = *(u32*)entry;
    void* r31;
    f32 f31, f30;
    if (flags & 0x1000) {
        return;
    }
    if (flags & GSMAT_FLAG_RENDERTYPE) {
        fn_801A32A0(*(void**)((u8*)entry + 0x10), 0x1cb, *(f32*)((u8*)entry + 0xc4));
        fn_801A32A0(*(void**)((u8*)entry + 0x14), 0x1cb, *(f32*)((u8*)entry + 0xc8));
        ((void(*)(void*))fn_801A1B40)(*(void**)((u8*)entry + 0x10));
        ((void(*)(void*))fn_801A1B40)(*(void**)((u8*)entry + 0x14));
        ((void(*)(void*))fn_800EB5A0)(entry);
    } else {
        r31 = *(void**)((u8*)entry + 0x8);
        if (flags & 0x20000) {
            r31 = *(void**)((u8*)r31 + 0x10);
        }
        f31 = lbl_8047CC5C;
        f30 = f31;
        if (flags & 0x20) {
            u8 r4;
            if ((flags & 0x800) || (*(u32*)((u8*)entry + 0x114) != 0 && *(u32*)((u8*)entry + 0x118) != 0)) {
                r4 = 1;
            } else {
                r4 = 0;
            }
            f31 = ((f32(*)(s32, u8, f32, f32, f32, f32))fn_800ED8C4)(
                *(s32*)((u8*)entry + 0x8c), r4,
                *(f32*)((u8*)entry + 0x98), *(f32*)((u8*)entry + 0x9c),
                *(f32*)((u8*)entry + 0xa0), *(f32*)((u8*)entry + 0x94));
        }
        if (flags & 0x40) {
            u8 r4;
            flags = *(u32*)entry;
            if ((flags & 0x800) || (*(u32*)((u8*)entry + 0x114) != 0 && *(u32*)((u8*)entry + 0x118) != 0)) {
                r4 = 1;
            } else {
                r4 = 0;
            }
            f30 = ((f32(*)(s32, u8, f32, f32, f32, f32))fn_800ED8C4)(
                *(s32*)((u8*)entry + 0xa4), r4,
                *(f32*)((u8*)entry + 0xb0), *(f32*)((u8*)entry + 0xb4),
                *(f32*)((u8*)entry + 0xb8), *(f32*)((u8*)entry + 0xac));
        }
        fn_801C028C(r31, 6, 0x9b2f, fn_801C027C, 1, f31);
        fn_801C028C(r31, 6, 0x64db, fn_801C027C, 1, f30);
        ((void(*)(void*))fn_801A1B40)(r31);
        if (*(u32*)entry & 0x20) {
            void* result = NULL;
            fn_801C028C(r31, 6, 0x20, fn_800EE044, 2, &result);
            if (result != NULL) {
                *(f32*)((u8*)entry + 0x9c) = *(f32*)((u8*)result + 0x4);
            }
        }
        if (*(u32*)entry & 0x40) {
            void* result = NULL;
            fn_801C028C(r31, 6, 0x480, fn_800EE044, 2, &result);
            if (result != NULL) {
                *(f32*)((u8*)entry + 0xb4) = *(f32*)((u8*)result + 0x4);
            }
        }
        if (*(u32*)entry & 0x20) {
            ((void(*)(void*))fn_800EDA98)(entry);
        }
        flags = *(u32*)entry;
        if (flags & 0x20000) {
            void* mobj = *(void**)((u8*)entry + 0x8);
            void* tev = *(void**)((u8*)mobj + 0x10);
            if (tev != NULL) {
                if (fn_800EA60C(tev) != 0) {
                    ((void(*)(void*))fn_8019D9DC)(tev);
                }
            }
        }
        {
            void* mobj = *(void**)((u8*)entry + 0x8);
            if (mobj != NULL) {
                if (fn_800EA60C(mobj) != 0) {
                    ((void(*)(void*))fn_8019D9DC)(mobj);
                }
            }
        }
    }
    *(u32*)entry |= 0x1000;
}
#endif

/* fn_800ED4D4 | Size: 0x1D8 */
extern void fn_800E06EC(void);
extern void fn_800DFEEC(void);
extern void fn_800E0108(void);
extern void jumptable_803155B0();
#if 1
asm void fn_800ED4D4(void) {
#include "src/game/gs_material_fn_800ED4D4.inc"
}
#else
void fn_800ED4D4(void) {
    /* GSmaterial internal (0x1D8 bytes) */
}
#endif

/* fn_800ED6AC | Size: 0x38 */
#if 0
asm void fn_800ED6AC(void) {
#include "src/game/gs_material_fn_800ED6AC.inc"
}
#else
s32 fn_800ED6AC(GSmaterialEntry* entry) {
    if (entry->flags & 0x800) {
        return 1;
    }
    if (entry->updateState != 0 && *(u32*)((u8*)entry + 0x118) != 0) {
        return 1;
    }
    return 0;
}
#endif

/* fn_800ED6E4 | Size: 0x100 */
#if 0
asm void fn_800ED6E4(void) {
#include "src/game/gs_material_fn_800ED6E4.inc"
}
#else
void fn_800ED6E4(void* entry, u8 enable) {
    u32 r5, r6, r7, r8;
    void (*cb)(void*, void*);
    if (*(u32*)entry & GSMAT_FLAG_RENDERTYPE) {
        if (enable == 0) {
            r8 = *(u32*)((u8*)entry + 0xc0);
            r5 = 0x4000;
            r7 = *(u32*)((u8*)entry + 0x8c);
            r6 = 0x20;
        } else {
            r8 = *(u32*)((u8*)entry + 0xa8);
            r5 = 0x8000;
            r7 = *(u32*)((u8*)entry + 0xa4);
            r6 = 0x40;
        }
    } else {
        if (enable == 0) {
            r8 = *(u32*)((u8*)entry + 0x90);
            r5 = 0x4000;
            r7 = *(u32*)((u8*)entry + 0x8c);
            r6 = 0x20;
        } else {
            r8 = *(u32*)((u8*)entry + 0xa8);
            r5 = 0x8000;
            r7 = *(u32*)((u8*)entry + 0xa4);
            r6 = 0x40;
        }
    }
    if (r7 == 0) {
        *(u32*)entry &= ~r6;
    } else if (r7 == 1) {
        *(u32*)entry &= ~r5;
    }
    cb = (void(*)(void*, void*))*(void**)((u8*)entry + 0xdc);
    if (cb != NULL) {
        struct {
            u32 flags;
            u32 val;
            u32 e0;
        } args;
        args.flags = enable ? 2 : 1;
        if (r7 == 1) {
            args.flags |= 4;
        }
        args.val = r8;
        args.e0 = *(u32*)((u8*)entry + 0xe0);
        cb(entry, &args);
    }
}
#endif

/* fn_800ED7E4 | Size: 0xE0 */
extern f32 lbl_8047CC78;
#if 0
asm void fn_800ED7E4(void) {
#include "src/game/gs_material_fn_800ED7E4.inc"
}
#else
void fn_800ED7E4(void* entry, f32 delta) {
    f32* p;
    f32* q;
    f32 target;
    u32 r4;
    u32 r7;
    f32 threshold;
    if (*(u32*)entry & GSMAT_FLAG_RENDERTYPE) {
        if (delta == 0.0f) {
            p      = (f32*)((u8*)entry + 0xc8);
            target = *(f32*)((u8*)entry + 0xd0);
            r7     = *(u32*)((u8*)entry + 0x8c);
            q      = p;
            r4     = 0x4000;
        } else {
            p      = (f32*)((u8*)entry + 0xb0);
            target = *(f32*)((u8*)entry + 0xb8);
            r7     = *(u32*)((u8*)entry + 0xa4);
            q      = (f32*)((u8*)entry + 0xb4);
            r4     = 0x8000;
        }
    } else {
        if (delta == 0.0f) {
            p      = (f32*)((u8*)entry + 0x98);
            target = *(f32*)((u8*)entry + 0xa0);
            r7     = *(u32*)((u8*)entry + 0x8c);
            q      = (f32*)((u8*)entry + 0x9c);
            r4     = 0x4000;
        } else {
            p      = (f32*)((u8*)entry + 0xb0);
            target = *(f32*)((u8*)entry + 0xb8);
            r7     = *(u32*)((u8*)entry + 0xa4);
            q      = (f32*)((u8*)entry + 0xb4);
            r4     = 0x8000;
        }
    }
    threshold = target - lbl_8047CC78;
    *p = *p + delta;
    if (*p >= threshold) {
        if (r7 == 0) {
            *p = target;
        } else if (r7 == 1) {
            *p = *p - target;
        }
    }
    if (*q >= threshold) {
        if (r7 != 0) return;
        *(u32*)entry |= r4;
    }
}
#endif

/* fn_800ED8C4 | Size: 0x1D4 */
extern s32 fn_800D3088(void);
extern f32 fn_800CE318(f64 val);
extern u8  lbl_80478AF8;
extern f32 lbl_8047CC5C;
extern f64 lbl_8047CC80;
extern f32 lbl_8047CC78;
extern f64 lbl_8047CC88;
extern f32 lbl_8047CC60;
#if 0
asm void fn_800ED8C4(void) {
#include "src/game/gs_material_fn_800ED8C4.inc"
}
#else
f32 fn_800ED8C4(s32 mode, u8 enable, f32 cur, f32 threshold, f32 target, f32 step) {
    f32 f1 = cur;
    if (lbl_80478AF8 == 1 && fn_800D37CC() == 0x3c) {
        if (enable == 0) {
            /* Convert cur to int and back, check delta */
            f64 fconv = (f64)(s32)cur;
            f64 fbase = lbl_8047CC80;
            f32 delta = lbl_8047CC78;
            f32 diff = (f32)(cur - (f32)(fconv - fbase));
            if (diff > delta) {
                s32 frames = fn_800D3088() - 1;
                f64 fframes = (f64)frames;
                f64 fbase2 = lbl_8047CC88;
                f32 fstep = (f32)((fframes - fbase2) * step);
                if (fstep >= lbl_8047CC60) {
                    /* ok */
                } else {
                    fstep = lbl_8047CC5C;
                }
                f1 = cur + fstep;
            } else {
                f1 = cur;
            }
        } else {
            f1 = cur;
        }
    }
    if (target > lbl_8047CC5C) {
        f32 limit = target - lbl_8047CC78;
        if (f1 >= limit) {
            if (mode == 0) {
                f1 = target;
            } else if (mode == 1) {
                f1 = (f32)fn_800CE318((f64)target);
            }
        }
    }
    if (f1 != threshold) {
        if (f1 < threshold) {
            if (mode == 1) {
                f32 f31 = f1 + (target - threshold);
                if (f31 < lbl_8047CC5C) f31 = lbl_8047CC5C;
                return f31;
            } else {
                f32 f31 = lbl_8047CC5C;
                if (f31 < lbl_8047CC5C) f31 = lbl_8047CC5C;
                return f31;
            }
        } else {
            f32 f31 = f1 - threshold;
            if (f31 < lbl_8047CC5C) f31 = lbl_8047CC5C;
            return f31;
        }
    }
    {
        f32 f31 = f1 - threshold;
        if (f31 < lbl_8047CC5C) f31 = lbl_8047CC5C;
        return f31;
    }
}
#endif

/* fn_800EDA98 -- FullPipeline | Size: 0x5AC */
extern u8 lbl_80270EE8[];
#if 1
asm void fn_800EDA98(void) {
#include "src/game/gs_material_fn_800EDA98.inc"
}
#else
void fn_800EDA98(void) {
    /* FullPipeline (0x5AC bytes) */
}
#endif

/* fn_800EE044 -- StoreResult | Size: 0x10 */
void fn_800EE044(void* result, void** dest) {
    if (result != NULL) {
        *dest = result;
    }
}

/* fn_800EE054 -- SetRenderMode | Size: 0x38 */
#if 0
asm void fn_800EE054(void) {
#include "src/game/gs_material_fn_800EE054.inc"
}
#else
void fn_800EE054(void* obj, u32 mode) {
    if (mode == 0) {
        ((void(*)(void*, u32))fn_801C2A74)(obj, 0x20000000);
    } else {
        ((void(*)(void*, u32))fn_801C2A90)(obj, 0x20000000);
    }
}
#endif

/* fn_800EE08C -- ConfigureFog | Size: 0x5C */
extern u8 lbl_8047CC90[];
extern u8 lbl_8047CC98[];
#if 0
asm void fn_800EE08C(void) {
#include "src/game/gs_material_fn_800EE08C.inc"
}
#else
void fn_800EE08C(void* obj, f32* ptr) {
    f32 val;
    if (obj == NULL) {
        fn_80196E10(lbl_8047CC90, 0xab, lbl_8047CC98);
    }
    val = *(f32*)((u8*)obj + 0xc);
    if (val > *ptr) {
        *ptr = val;
    }
}
#endif

/* fn_800EE0E8 -- ConfigureScissor | Size: 0x68 */
extern u32 lbl_8047ABB0;
extern u32 lbl_8047ABAC;
#if 0
asm void fn_800EE0E8(void) {
#include "src/game/gs_material_fn_800EE0E8.inc"
}
#else
u32 fn_800EE0E8(void* entry) {
    lbl_8047ABB0 = 0;
    lbl_8047ABAC = (u32)-1;
    fn_801A3918(*(void**)((u8*)entry + 0x8), fn_800EE20C, 0);
    if (*(u32*)entry & 0x20000) {
        lbl_8047ABB0 -= 1;
    }
    return lbl_8047ABB0;
}
#endif

/* fn_800EE150 -- ApplyPEDescr | Size: 0xBC */
extern void* fn_800EE834(void);
extern u32 lbl_8047ABAC;
extern u32 lbl_8047ABB0;
extern u32 lbl_8047ABA8;
#if 0
asm void fn_800EE150(void) {
#include "src/game/gs_material_fn_800EE150.inc"
}
#else
void* fn_800EE150(void* entry, u32 param) {
    void* mobj;
    u32 max = param;
    void* result;
    if (*(u32*)entry & 0x20000) {
        max = param + 1;
    }
    lbl_8047ABAC = max;
    lbl_8047ABB0 = 0;
    lbl_8047ABA8 = 0;
    mobj = ((void*(*)(void*))fn_800E5188)(entry);
    if (max == 0) {
        lbl_8047ABA8 = (u32)mobj;
    } else {
        fn_801A3918(mobj, fn_800EE20C, 0);
        if (lbl_8047ABA8 == 0) {
            return NULL;
        }
    }
    result = fn_800EE834();
    if (result == NULL) {
        return NULL;
    }
    *(void**)((u8*)result + 0x4) = entry;
    *(u32*)((u8*)result + 0x8) = lbl_8047ABA8;
    *(u16*)((u8*)result + 0x2) = (u16)param;
    return result;
}
#endif

/* fn_800EE20C -- GetPEParam | Size: 0x20 */
extern u32 lbl_8047ABB0;
extern u32 lbl_8047ABAC;
extern u32 lbl_8047ABA8;
#if 0
asm void fn_800EE20C(void) {
#include "src/game/gs_material_fn_800EE20C.inc"
}
#else
void fn_800EE20C(u32 val) {
    u32 counter = lbl_8047ABB0;
    u32 max = lbl_8047ABAC;
    lbl_8047ABB0 = counter + 1;
    if (counter == max) {
        lbl_8047ABA8 = val;
    }
}
#endif

/* fn_800EE22C -- ResetBlendMode | Size: 0x5C */
extern void fn_800EE8F0(void);
extern u8  lbl_8047ABC4;
extern u32 lbl_8047ABD0;
extern u32 lbl_8047ABC8;
extern u32 lbl_8047ABCC;
#if 0
asm void fn_800EE22C(void) {
#include "src/game/gs_material_fn_800EE22C.inc"
}
#else
s32 fn_800EE22C(void* a, void* b) {
    if (a == b) {
        return 0;
    }
    lbl_8047ABC4 = 0;
    lbl_8047ABD0 = (u32)b;
    lbl_8047ABC8 = 0;
    lbl_8047ABCC = (u32)-1;
    fn_801A3918(a, fn_800EE8F0, 0);
    return (s32)lbl_8047ABCC;
}
#endif

/* fn_800EE288 -- Finalize | Size: 0x40 */
#if 0
asm void fn_800EE288(void) {
#include "src/game/gs_material_fn_800EE288.inc"
}
#else
void fn_800EE288(void* p) {
    void* tbl = *(void**)((u8*)p + 4);
    s32 i = 4;
    do {
        u32 val = *(u32*)((u8*)tbl + 0xe8);
        u16 key = *(u16*)((u8*)p + 2);
        if (val == (u32)key) {
            *(u32*)((u8*)tbl + 0xe4) = 0;
            *(u32*)((u8*)tbl + 0xe8) = (u32)-1;
            return;
        }
        i--;
    } while (i != 0);
}
#endif
#if 0
asm void fn_800EC134(void) {
#include "src/game/gs_material_fn_800EC134.inc"
}
#else
void fn_800EC134(void* entry) {
    ((void(*)(void*))fn_800ED1CC)(entry);
}
#endif
#if 1
asm void fn_800ECA78(void) {
#include "src/game/gs_material_fn_800ECA78.inc"
}
#else
void fn_800ECA78(void) { /* TODO */ }
#endif
