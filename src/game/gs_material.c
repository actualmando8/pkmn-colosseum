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
extern void  fn_800ECA78(void* entry);                 /* env-map alpha apply */
extern void  fn_800EC134(void* entry);                 /* update MObj color */

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
void fn_800E3604(void) {
    /* GSmaterialApplyAll (0x15C bytes) */
}

/* fn_800E3760 -- GSmaterialApplySingle | Size: 0x124 */
void fn_800E3760(void) {
    /* GSmaterialApplySingle (0x124 bytes) */
}

/* fn_800E3884 -- GSmaterialLerpColors | Size: 0xA4 */
void fn_800E3884(void) {
    /* GSmaterialLerpColors (0xA4 bytes) */
}

/* fn_800E3928 -- GSmaterialUpdateColors | Size: 0x1E0 */
void fn_800E3928(void) {
    /* GSmaterialUpdateColors (0x1E0 bytes) */
}

/* fn_800E3B08 | Size: 0x34 */
void fn_800E3B08(void) {
    /* GSmaterial internal (0x34 bytes) */
}

/* fn_800E3BC0 -- GetGXTexGenSrc | Size: 0x30 */
void fn_800E3BC0(void) {
    /* GetGXTexGenSrc (0x30 bytes) */
}

/* fn_800E3DC4 -- GSmaterialUpdateAlpha | Size: 0x250 */
void fn_800E3DC4(void) {
    /* GSmaterialUpdateAlpha (0x250 bytes) */
}

/* fn_800E4014 | Size: 0x68 */
void fn_800E4014(void) {
    /* GSmaterial internal (0x68 bytes) */
}

/* fn_800E407C | Size: 0xF4 */
void fn_800E407C(void) {
    /* GSmaterial internal (0xF4 bytes) */
}

/* fn_800E4170 -- SetupAlphaBlend | Size: 0x234 */
void fn_800E4170(void) {
    /* SetupAlphaBlend (0x234 bytes) */
}

/* fn_800E43A4 -- SetupZMode | Size: 0x170 */
void fn_800E43A4(void) {
    /* SetupZMode (0x170 bytes) */
}

/* fn_800E4514 | Size: 0x84 */
void fn_800E4514(void) {
    /* GSmaterial internal (0x84 bytes) */
}

/* fn_800E4598 | Size: 0xC4 */
void fn_800E4598(void) {
    /* GSmaterial internal (0xC4 bytes) */
}

/* fn_800E465C -- TEVStageSetup | Size: 0x464 */
void fn_800E465C(void) {
    /* TEVStageSetup (0x464 bytes) */
}

/* fn_800E4AC0 -- TEVColorRegister | Size: 0x134 */
void fn_800E4AC0(void) {
    /* TEVColorRegister (0x134 bytes) */
}

/* fn_800E4BF4 -- TEVSwapMode | Size: 0xA4 */
void fn_800E4BF4(void) {
    /* TEVSwapMode (0xA4 bytes) */
}

/* fn_800E4C98 | Size: 0x80 */
void fn_800E4C98(void) {
    /* GSmaterial internal (0x80 bytes) */
}

/* fn_800E4D18 | Size: 0x24 */
void fn_800E4D18(void) {
    /* GSmaterial internal (0x24 bytes) */
}

/* fn_800E4D3C | Size: 0x74 */
void fn_800E4D3C(void) {
    /* GSmaterial internal (0x74 bytes) */
}

/* fn_800E4DB0 | Size: 0xDC */
void fn_800E4DB0(void) {
    /* GSmaterial internal (0xDC bytes) */
}

/* fn_800E4E8C | Size: 0x21C */
void fn_800E4E8C(void) {
    /* GSmaterial internal (0x21C bytes) */
}

/* fn_800E50A8 | Size: 0xE0 */
void fn_800E50A8(void) {
    /* GSmaterial internal (0xE0 bytes) */
}

/* fn_800E5188 | Size: 0x1C */
void fn_800E5188(void) {
    /* GSmaterial internal (0x1C bytes) */
}

/* fn_800E51A4 -- TEVMultiStageSetup | Size: 0x3AC */
void fn_800E51A4(void) {
    /* TEVMultiStageSetup (0x3AC bytes) */
}

/* fn_800E5550 | Size: 0xEC */
void fn_800E5550(void) {
    /* GSmaterial internal (0xEC bytes) */
}

/* fn_800E563C | Size: 0x154 */
void fn_800E563C(void) {
    /* GSmaterial internal (0x154 bytes) */
}

/* fn_800E5790 | Size: 0xBC */
void fn_800E5790(void) {
    /* GSmaterial internal (0xBC bytes) */
}

/* fn_800E584C | Size: 0x12C */
void fn_800E584C(void) {
    /* GSmaterial internal (0x12C bytes) */
}

/* fn_800E5978 | Size: 0x50 */
void fn_800E5978(void) {
    /* GSmaterial internal (0x50 bytes) */
}

/* fn_800E59C8 | Size: 0xAC */
void fn_800E59C8(void) {
    /* GSmaterial internal (0xAC bytes) */
}

/* fn_800E5A74 | Size: 0xF4 */
void fn_800E5A74(void) {
    /* GSmaterial internal (0xF4 bytes) */
}

/* fn_800E5B68 | Size: 0x78 */
void fn_800E5B68(void) {
    /* GSmaterial internal (0x78 bytes) */
}

/* fn_800E5BE0 | Size: 0x160 */
void fn_800E5BE0(void) {
    /* GSmaterial internal (0x160 bytes) */
}

/* fn_800E5D40 | Size: 0xF4 */
void fn_800E5D40(void) {
    /* GSmaterial internal (0xF4 bytes) */
}

/* fn_800E5E34 | Size: 0x178 */
void fn_800E5E34(void) {
    /* GSmaterial internal (0x178 bytes) */
}

/* fn_800E5FAC | Size: 0x50 */
void fn_800E5FAC(void) {
    /* GSmaterial internal (0x50 bytes) */
}

/* fn_800E5FFC | Size: 0xF4 */
void fn_800E5FFC(void) {
    /* GSmaterial internal (0xF4 bytes) */
}

/* fn_800E60F0 | Size: 0xCC */
void fn_800E60F0(void) {
    /* GSmaterial internal (0xCC bytes) */
}

/* fn_800E61BC | Size: 0x1D0 */
void fn_800E61BC(void) {
    /* GSmaterial internal (0x1D0 bytes) */
}

/* fn_800E638C | Size: 0xEC */
void fn_800E638C(void) {
    /* GSmaterial internal (0xEC bytes) */
}

/* fn_800E6478 | Size: 0x154 */
void fn_800E6478(void) {
    /* GSmaterial internal (0x154 bytes) */
}

/* fn_800E65CC | Size: 0xEC */
void fn_800E65CC(void) {
    /* GSmaterial internal (0xEC bytes) */
}

/* fn_800E66B8 | Size: 0x14C */
void fn_800E66B8(void) {
    /* GSmaterial internal (0x14C bytes) */
}

/* fn_800E6804 | Size: 0xD4 */
void fn_800E6804(void) {
    /* GSmaterial internal (0xD4 bytes) */
}

/* fn_800E68D8 | Size: 0xEC */
void fn_800E68D8(void) {
    /* GSmaterial internal (0xEC bytes) */
}

/* fn_800E69C4 | Size: 0x15C */
void fn_800E69C4(void) {
    /* GSmaterial internal (0x15C bytes) */
}

/* fn_800E6B20 | Size: 0xA8 */
void fn_800E6B20(void) {
    /* GSmaterial internal (0xA8 bytes) */
}

/* fn_800E6BC8 | Size: 0x1F8 */
void fn_800E6BC8(void) {
    /* GSmaterial internal (0x1F8 bytes) */
}

/* fn_800E6DC0 | Size: 0xC */
void fn_800E6DC0(void) {
    /* GSmaterial internal (0xC bytes) */
}

/* fn_800E6DCC -- TEV descriptor builder | Size: 0x4C4 */
void fn_800E6DCC(void) {
    /* TEV descriptor builder (0x4C4 bytes) */
}

/* fn_800E7290 -- TEV stage count | Size: 0x9C */
void fn_800E7290(void) {
    /* TEV stage count (0x9C bytes) */
}

/* fn_800E732C -- FULL TEV PIPELINE | Size: 0x12BC */
void fn_800E732C(void) {
    /* FULL TEV PIPELINE (0x12BC bytes) */
}

/* fn_800E85E8 -- TEV post-config | Size: 0x9C */
void fn_800E85E8(void) {
    /* TEV post-config (0x9C bytes) */
}

/* fn_800E8684 -- LIGHTING SETUP | Size: 0x878 */
void fn_800E8684(void) {
    /* LIGHTING SETUP (0x878 bytes) */
}

/* fn_800E8EFC -- RenderStateReset | Size: 0x6C */
void fn_800E8EFC(void) {
    /* RenderStateReset (0x6C bytes) */
}

/* fn_800E8F74 -- SetDistanceThreshold | Size: 0xC */
void fn_800E8F74(void) {
    /* SetDistanceThreshold (0xC bytes) */
}

/* fn_800E8F80 | Size: 0x20 */
void fn_800E8F80(void) {
    /* GSmaterial internal (0x20 bytes) */
}

/* fn_800E8FA0 | Size: 0x48 */
void fn_800E8FA0(void) {
    /* GSmaterial internal (0x48 bytes) */
}

/* fn_800E8FE8 | Size: 0x24 */
void fn_800E8FE8(void) {
    /* GSmaterial internal (0x24 bytes) */
}

/* fn_800E900C -- Distance check | Size: 0xBC */
void fn_800E900C(void) {
    /* Distance check (0xBC bytes) */
}

/* fn_800E90C8 | Size: 0x40 */
void fn_800E90C8(void) {
    /* GSmaterial internal (0x40 bytes) */
}

/* fn_800E9108 | Size: 0x40 */
void fn_800E9108(void) {
    /* GSmaterial internal (0x40 bytes) */
}

/* fn_800E9148 -- CheckRenderSlot | Size: 0x140 */
void fn_800E9148(void) {
    /* CheckRenderSlot (0x140 bytes) */
}

/* fn_800E9288 | Size: 0x50 */
void fn_800E9288(void) {
    /* GSmaterial internal (0x50 bytes) */
}

/* fn_800E92D8 | Size: 0x80 */
void fn_800E92D8(void) {
    /* GSmaterial internal (0x80 bytes) */
}

/* fn_800E9358 | Size: 0x60 */
void fn_800E9358(void) {
    /* GSmaterial internal (0x60 bytes) */
}

/* fn_800E93B8 -- SetupEnvMap | Size: 0x5E0 */
void fn_800E93B8(void) {
    /* SetupEnvMap (0x5E0 bytes) */
}

/* fn_800E9998 -- UpdateEnvMap | Size: 0x194 */
void fn_800E9998(void) {
    /* UpdateEnvMap (0x194 bytes) */
}

/* fn_800E9B2C | Size: 0x140 */
void fn_800E9B2C(void) {
    /* GSmaterial internal (0x140 bytes) */
}

/* fn_800E9C6C | Size: 0x1C8 */
void fn_800E9C6C(void) {
    /* GSmaterial internal (0x1C8 bytes) */
}

/* fn_800E9E34 | Size: 0x5C */
void fn_800E9E34(void) {
    /* GSmaterial internal (0x5C bytes) */
}

/* fn_800E9E90 -- EnvMap pipeline | Size: 0x77C */
void fn_800E9E90(void) {
    /* EnvMap pipeline (0x77C bytes) */
}

/* fn_800EA60C | Size: 0x58 */
void fn_800EA60C(void) {
    /* GSmaterial internal (0x58 bytes) */
}

/* fn_800EA664 | Size: 0x70 */
void fn_800EA664(void) {
    /* GSmaterial internal (0x70 bytes) */
}

/* fn_800EA6D4 -- BindTextureToStage | Size: 0x110 */
void fn_800EA6D4(void) {
    /* BindTextureToStage (0x110 bytes) */
}

/* fn_800EA7E4 | Size: 0x3C */
void fn_800EA7E4(void) {
    /* GSmaterial internal (0x3C bytes) */
}

/* fn_800EA820 | Size: 0x140 */
void fn_800EA820(void) {
    /* GSmaterial internal (0x140 bytes) */
}

/* fn_800EA960 -- ConfigureTEVStage | Size: 0x370 */
void fn_800EA960(void) {
    /* ConfigureTEVStage (0x370 bytes) */
}

/* fn_800EACD0 -- ConfigureBlend | Size: 0x314 */
void fn_800EACD0(void) {
    /* ConfigureBlend (0x314 bytes) */
}

/* fn_800EAFE4 -- ConfigureZMode | Size: 0x284 */
void fn_800EAFE4(void) {
    /* ConfigureZMode (0x284 bytes) */
}

/* fn_800EB268 -- ConfigureAlpha | Size: 0xD8 */
void fn_800EB268(void) {
    /* ConfigureAlpha (0xD8 bytes) */
}

/* fn_800EB340 | Size: 0xD4 */
void fn_800EB340(void) {
    /* GSmaterial internal (0xD4 bytes) */
}

/* fn_800EB414 | Size: 0x50 */
void fn_800EB414(void) {
    /* GSmaterial internal (0x50 bytes) */
}

/* fn_800EB464 | Size: 0xBC */
void fn_800EB464(void) {
    /* GSmaterial internal (0xBC bytes) */
}

/* fn_800EB520 | Size: 0x8 */
void fn_800EB520(void) {
    /* GSmaterial internal (0x8 bytes) */
}

/* fn_800EB528 | Size: 0x78 */
void fn_800EB528(void) {
    /* GSmaterial internal (0x78 bytes) */
}

/* fn_800EB5A0 | Size: 0x140 */
void fn_800EB5A0(void) {
    /* GSmaterial internal (0x140 bytes) */
}

/* fn_800EB6E0 | Size: 0x224 */
void fn_800EB6E0(void) {
    /* GSmaterial internal (0x224 bytes) */
}

/* fn_800EB904 -- Large render setup | Size: 0x5E8 */
void fn_800EB904(void) {
    /* Large render setup (0x5E8 bytes) */
}

/* fn_800EBEEC -- Render config | Size: 0x1FC */
void fn_800EBEEC(void) {
    /* Render config (0x1FC bytes) */
}

/* fn_800EC0E8 -- SetAlpha | Size: 0x4C */
void fn_800EC0E8(void) {
    /* SetAlpha (0x4C bytes) */
}

/* fn_800EC134 -- UpdateMObjColor | Size: 0x20 */
void fn_800EC134_impl(void) {
    /* GSmaterialUpdateMObjColor: Push colors to HSD MObj (0x20 bytes) */
}

/* fn_800EC154 -- GetMObjPtr | Size: 0xC */
void fn_800EC154(void) {
    /* GetMObjPtr (0xC bytes) */
}

/* fn_800EC160 -- SetDiffuseRGBA | Size: 0x28 */
void fn_800EC160(void) {
    /* SetDiffuseRGBA (0x28 bytes) */
}

/* fn_800EC188 -- SetAmbientRGBA | Size: 0x28 */
void fn_800EC188(void) {
    /* SetAmbientRGBA (0x28 bytes) */
}

/* fn_800EC1B0 -- GetDiffuseR | Size: 0xC */
void fn_800EC1B0(void) {
    /* GetDiffuseR (0xC bytes) */
}

/* fn_800EC1BC -- GetDiffuseG | Size: 0xC */
void fn_800EC1BC(void) {
    /* GetDiffuseG (0xC bytes) */
}

/* fn_800EC1C8 -- GetDiffuseB | Size: 0xC */
void fn_800EC1C8(void) {
    /* GetDiffuseB (0xC bytes) */
}

/* fn_800EC1D4 -- GetDiffuseA | Size: 0x10 */
void fn_800EC1D4(void) {
    /* GetDiffuseA (0x10 bytes) */
}

/* fn_800EC1E4 | Size: 0x24 */
void fn_800EC1E4(void) {
    /* GSmaterial internal (0x24 bytes) */
}

/* fn_800EC208 | Size: 0x9C */
void fn_800EC208(void) {
    /* GSmaterial internal (0x9C bytes) */
}

/* fn_800EC2A4 | Size: 0x64 */
void fn_800EC2A4(void) {
    /* GSmaterial internal (0x64 bytes) */
}

/* fn_800EC308 | Size: 0x54 */
void fn_800EC308(void) {
    /* GSmaterial internal (0x54 bytes) */
}

/* fn_800EC35C -- PE descriptor setup | Size: 0x174 */
void fn_800EC35C(void) {
    /* PE descriptor setup (0x174 bytes) */
}

/* fn_800EC4D0 | Size: 0x6C */
void fn_800EC4D0(void) {
    /* GSmaterial internal (0x6C bytes) */
}

/* fn_800EC53C | Size: 0x2C */
void fn_800EC53C(void) {
    /* GSmaterial internal (0x2C bytes) */
}

/* fn_800EC568 | Size: 0x8 */
void fn_800EC568(void) {
    /* GSmaterial internal (0x8 bytes) */
}

/* fn_800EC570 | Size: 0x8 */
void fn_800EC570(void) {
    /* GSmaterial internal (0x8 bytes) */
}

/* fn_800EC578 | Size: 0x34 */
void fn_800EC578(void) {
    /* GSmaterial internal (0x34 bytes) */
}

/* fn_800EC5AC | Size: 0xC */
void fn_800EC5AC(void) {
    /* GSmaterial internal (0xC bytes) */
}

/* fn_800EC5B8 | Size: 0x44 */
void fn_800EC5B8(void) {
    /* GSmaterial internal (0x44 bytes) */
}

/* fn_800EC5FC -- RenderSetup | Size: 0x2CC */
void fn_800EC5FC(void) {
    /* RenderSetup (0x2CC bytes) */
}

/* fn_800EC8C8 | Size: 0x14 */
void fn_800EC8C8(void) {
    /* GSmaterial internal (0x14 bytes) */
}

/* fn_800EC8DC | Size: 0x3C */
void fn_800EC8DC(void) {
    /* GSmaterial internal (0x3C bytes) */
}

/* fn_800EC918 | Size: 0x3C */
void fn_800EC918(void) {
    /* GSmaterial internal (0x3C bytes) */
}

/* fn_800EC954 | Size: 0xC */
void fn_800EC954(void) {
    /* GSmaterial internal (0xC bytes) */
}

/* fn_800EC960 | Size: 0xC */
void fn_800EC960(void) {
    /* GSmaterial internal (0xC bytes) */
}

/* fn_800EC96C | Size: 0x24 */
void fn_800EC96C(void) {
    /* GSmaterial internal (0x24 bytes) */
}

/* fn_800EC990 | Size: 0x4C */
void fn_800EC990(void) {
    /* GSmaterial internal (0x4C bytes) */
}

/* fn_800EC9DC | Size: 0x9C */
void fn_800EC9DC(void) {
    /* GSmaterial internal (0x9C bytes) */
}

/* fn_800ECA78 -- EnvMap alpha apply | Size: 0xFC */
void fn_800ECA78_impl(void) {
    /* GSmaterialEnvMapAlphaApply: Env-map alpha apply (0xFC bytes) */
}

/* fn_800ECB74 | Size: 0x134 */
void fn_800ECB74(void) {
    /* GSmaterial internal (0x134 bytes) */
}

/* fn_800ECCA8 -- ShadowSetup | Size: 0x428 */
void fn_800ECCA8(void) {
    /* ShadowSetup (0x428 bytes) */
}

/* fn_800ED0D0 -- PostRender | Size: 0xFC */
void fn_800ED0D0(void) {
    /* PostRender (0xFC bytes) */
}

/* fn_800ED1CC | Size: 0x308 */
void fn_800ED1CC(void) {
    /* GSmaterial internal (0x308 bytes) */
}

/* fn_800ED4D4 | Size: 0x1D8 */
void fn_800ED4D4(void) {
    /* GSmaterial internal (0x1D8 bytes) */
}

/* fn_800ED6AC | Size: 0x38 */
void fn_800ED6AC(void) {
    /* GSmaterial internal (0x38 bytes) */
}

/* fn_800ED6E4 | Size: 0x100 */
void fn_800ED6E4(void) {
    /* GSmaterial internal (0x100 bytes) */
}

/* fn_800ED7E4 | Size: 0xE0 */
void fn_800ED7E4(void) {
    /* GSmaterial internal (0xE0 bytes) */
}

/* fn_800ED8C4 | Size: 0x1D4 */
void fn_800ED8C4(void) {
    /* GSmaterial internal (0x1D4 bytes) */
}

/* fn_800EDA98 -- FullPipeline | Size: 0x5AC */
void fn_800EDA98(void) {
    /* FullPipeline (0x5AC bytes) */
}

/* fn_800EE044 -- StoreResult | Size: 0x10 */
void fn_800EE044(void* result, void** dest) {
    if (result != NULL) {
        *dest = result;
    }
}

/* fn_800EE054 -- SetRenderMode | Size: 0x38 */
void fn_800EE054(void) {
    /* SetRenderMode (0x38 bytes) */
}

/* fn_800EE08C -- ConfigureFog | Size: 0x5C */
void fn_800EE08C(void) {
    /* ConfigureFog (0x5C bytes) */
}

/* fn_800EE0E8 -- ConfigureScissor | Size: 0x68 */
void fn_800EE0E8(void) {
    /* ConfigureScissor (0x68 bytes) */
}

/* fn_800EE150 -- ApplyPEDescr | Size: 0xBC */
void fn_800EE150(void) {
    /* ApplyPEDescr (0xBC bytes) */
}

/* fn_800EE20C -- GetPEParam | Size: 0x20 */
void fn_800EE20C(void) {
    /* GetPEParam (0x20 bytes) */
}

/* fn_800EE22C -- ResetBlendMode | Size: 0x5C */
void fn_800EE22C(void) {
    /* ResetBlendMode (0x5C bytes) */
}

/* fn_800EE288 -- Finalize | Size: 0x40 */
void fn_800EE288(void) {
    /* Finalize (0x40 bytes) */
}
