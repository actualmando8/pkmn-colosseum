/**
 * @file gs_render.c
 * @brief GSrender -- Extended graphics pipeline / GX wrapper / GSmaterial.
 *
 * This module covers the extended GSgfx rendering pipeline between the
 * SDK code and GSmem. It contains the GX wrapper layer, matrix operations,
 * lighting setup, material system, and draw command dispatch.
 *
 * Decompiled from 278 functions in range 0x800D3E4C - 0x800E202C.
 *
 * Sub-modules identified within this range:
 *
 * 1. GSgfx callbacks / VBlank (0x800D3E4C - 0x800D4610)
 *    - VBlank retrace handler, frame-end callback, draw-done
 *    - State save/restore for GSgfx state structure
 *    - fn_800D3E4C: VBlank callback (copies 0x5A0 bytes of state)
 *    - fn_800D3EC4: Pre-retrace callback (6 float params)
 *    - fn_800D3F5C: Frame counter increment + dispatch
 *    - fn_800D3FA4: GSgfx_BeginFrame (0x654 bytes -- large setup)
 *
 * 2. GSlog / debug output (0x800D461C - 0x800D5504)
 *    - fn_800D461C: GSlog_PrintFormatted (0x97C bytes -- varargs printf)
 *    - fn_800D4F98: GSlog_QueueCommand (referenced by GSmaterial)
 *    - fn_800D5504: GSlog_Init (0xCC bytes)
 *    - Uses "0123456789ABCDEF" for hex formatting
 *
 * 3. GSgfx matrix / transform (0x800D5504 - 0x800D7900)
 *    - ~80 small functions for matrix stack push/pop, load, multiply
 *    - Many are 0x14-0x24 bytes (simple wrappers around GX calls)
 *    - fn_800D67BC: MTX operations (0x244 bytes)
 *    - fn_800D6B00: Large matrix setup (0x730 bytes)
 *    - Getters/setters for matrix indices (fn_800D7230-fn_800D75D0)
 *    - Functions at 0x14-0x24 bytes follow a pattern:
 *      lwz r3, offset(rN) / blr (simple field accessors)
 *
 * 4. GSgfx viewport / projection (0x800D7900 - 0x800D9F40)
 *    - fn_800D7894: GSgfx_InitViewport
 *    - fn_800D7B80: GSgfx_InitProjection
 *    - fn_800D83E4: GSgfx_InitMatrixStack
 *    - fn_800D87AC: GSgfx_SetInternalMode
 *    - fn_800D892C: GSgfx_ConfigurePipeline (0x910 bytes)
 *    - fn_800D9C24: GSgfx_SetViewportRect
 *    - fn_800D9D68: GSgfx_SetScissor
 *
 * 5. GSgfx blend / TEV / fog (0x800D9F40 - 0x800DA2BC)
 *    - fn_800D9F40: GSgfx_ConfigureFog
 *    - fn_800DA028: GSgfx_ConfigureTEV
 *    - fn_800DA100: GSgfx_ConfigureAlpha
 *    - fn_800DA1E8: GSgfx_ConfigureZ
 *    - fn_800DA2BC: GSgfx_ConfigureBlend
 *
 * 6. GSgfx lighting (0x800DB890 - 0x800DC224)
 *    - fn_800DB890: GSgfx_InitLighting
 *    - Functions for setting light position, color, attenuation
 *    - Tight integration with lbl_8047AA80 (GSgfx state)
 *    - fn_800DC0D4: Light command via GSlog_QueueCommand
 *
 * 7. GSmaterial system (0x800DC224 - 0x800DE680)
 *    - fn_800DE680: GSmaterial_Create (0x948 bytes -- very large)
 *    - Material setup, PE descriptor config, texture binding
 *    - References:
 *      "GSmaterialSetPEdescr: Warning: already using a custom description!"
 *      "GSmaterialCreate: Run out of materials..."
 *      "GSmaterial MObj"
 *      "GSmaterial: Unsupported texture format for environment map!"
 *      "GSmaterial: Error creating environment map: no texture defined!"
 *
 * 8. GSgfx draw commands (0x800DE680 - 0x800E202C)
 *    - fn_800E1544: Large draw dispatch (0xAE8 = 2792 bytes)
 *    - Vertex submission, display list execution
 *    - Color/normal/texcoord attribute setup
 *
 * Debug strings:
 *   "GSgfx: invalid matrix index"
 *   "GSgfx: matrix stack underflow!"
 *   "GSgfx: matrix stack overflow!"
 *   "0123456789ABCDEF"
 *   "GSmaterialSetPEdescr: Warning: already using a custom description!"
 *   "GSmaterialCreate: Run out of materials. Increase materialcount..."
 *   "GSmaterial MObj"
 *   "GSmaterial: Unsupported texture format for environment map!"
 *   "GSmaterial: Error creating environment map: no texture defined!"
 *
 * Address range: 0x800D3E4C - 0x800E202C (56KB, 278 functions)
 */

#include "dolphin/types.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970();                            /* OSReport / GSlog (K&R: variadic, asm-wrapped in this TU) */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);

/* External functions referenced from asm wrappers */
extern void DCFlushRange(void* addr, u32 size);
extern u64 OSGetTime(void);
extern void fn_800D3EC4(void);
extern void fn_800D4F98(void);
extern void fn_800D67BC(void);
extern void fn_800D892C(void);

/* GSmem */
extern u16   fn_800E3534(u32 size);                    /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                  /* GSmemGetPtr */

/* SDK GX functions */
extern void  fn_800AA2F0(void);                        /* GXSetViewport */
extern void  fn_800BD640(void);                        /* GXSetProjection */
extern void  fn_800BD744(void);                        /* GXLoadPosMtxImm */
extern void  fn_800BB29C(void);                        /* GXInvalidateTexAll */

/* ===== String constants (rodata) ===== */
extern const char lbl_80270440[]; /* "GSgfx: invalid matrix index" */
extern const char lbl_80270460[]; /* "GSgfx: matrix stack underflow!" */
extern const char lbl_80270480[]; /* "GSgfx: matrix stack overflow!" */
extern const char lbl_802704A0[]; /* "0123456789ABCDEF" */
extern const char lbl_80270528[]; /* "GSmaterialSetPEdescr: Warning..." */
extern const char lbl_8027056C[]; /* "GSmaterialCreate: Run out of materials..." */
extern const char lbl_802705C0[]; /* "GSmaterial MObj" */
extern const char lbl_802705D0[]; /* "GSmaterial: Unsupported texture format..." */
extern const char lbl_80270610[]; /* "GSmaterial: Error creating environment map..." */

/* ===== BSS / global state ===== */
extern u32 lbl_8047AA80;   /* GSgfx state pointer (sda21) */
extern u8 lbl_80400248[];  /* GSgfx state backup buffer (0x5A0 bytes) */
extern u8 lbl_80400B28[];  /* light/material command buffer */

/* ==================================================================
 * fn_800D3E4C -- GSgfx VBlank callback
 *
 * Called on vertical blank interrupt. Saves or restores the 0x5A0-byte
 * GSgfx state structure depending on mode:
 *   mode 0: Copy from state -> backup (save)
 *   mode 1: Copy from backup -> caller buffer (restore)
 *
 * From disassembly (0x800D3E4C, 0x78 bytes):
 *   cmpwi r3, 0x1
 *   beq .restore
 *   ; mode 0: memcpy(lbl_80400248, r4, 0x5A0)
 *   ; mode 1: memcpy(r4=r31, lbl_80400248, 0x5A0)
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSgfx_VBlankCallback(s32 mode, void* buffer) {
    /* TODO: match -- 120 bytes at 0x800D3E4C */
}
#pragma pop

/* ==================================================================
 * fn_800D3EC4 -- GSgfx pre-retrace callback
 *
 * Called before VBlank retrace. Takes 6 float parameters for viewport
 * setup, dispatches through function pointer if set.
 *
 * From disassembly (0x800D3EC4, 0x8C bytes):
 *   fmr f26, f1 through fmr f31, f6  ; save 6 float params
 *   lwz r12, lbl_8047AA88            ; callback pointer
 *   cmplwi r12, 0x0
 *   beq .default
 *   bctrl                            ; call custom handler
 *   .default:
 *   bl fn_800AA2F0                   ; GXSetViewport
 *   bl fn_800BD640                   ; GXSetProjection
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSgfx_PreRetraceCallback(s32 flag, f32 p1, f32 p2,
                                f32 p3, f32 p4, f32 p5, f32 p6) {
    /* TODO: match -- 140 bytes at 0x800D3EC4 */
}
#pragma pop

/* ==================================================================
 * fn_800D3F50 -- GSgfx draw-done callback
 *
 * Set a flag indicating the GX pipeline has finished drawing.
 * 12 bytes.
 *
 * From disassembly (0x800D3F50, 0xC bytes):
 *   li r0, 0x1
 *   stb r0, lbl_8047AA91@sda21(r0)
 *   blr
 * ================================================================== */
void GSgfx_DrawDoneCallback(void) {
    /* Set draw-done flag */
    extern u8 lbl_8047AA91;
    lbl_8047AA91 = 1;
}

/* ==================================================================
 * fn_800D3F5C -- GSgfx frame end callback
 *
 * Increment frame counter and dispatch queued operations.
 * 72 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSgfx_FrameEndCallback(void) {
    /* TODO: match -- 72 bytes at 0x800D3F5C */
}
#pragma pop

/* ==================================================================
 * fn_800D3FA4 -- GSgfx_BeginFrame
 *
 * Set up the rendering pipeline for a new frame. This is a large
 * function (0x654 = 1620 bytes) that configures:
 *   - GX FIFO
 *   - Clear color and Z buffer
 *   - Viewport and scissor
 *   - Default blend modes
 *   - TEV stages
 *   - Projection matrix
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSgfx_BeginFrame(void) {
    /* TODO: match -- 1620 bytes at 0x800D3FA4 */
}
#pragma pop

/* ==================================================================
 * fn_800D461C -- GSlog_PrintFormatted
 *
 * Varargs printf-like function for the GS debug logging system.
 * At 0x97C (2428) bytes, this is a substantial printf implementation.
 * Uses "0123456789ABCDEF" for hex digit lookup.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSlog_PrintFormatted(u32 channel, u32 paramCount, ...) {
    /* TODO: match -- 2428 bytes at 0x800D461C */
}
#pragma pop

/* ==================================================================
 * fn_800D4F98 -- GSlog_QueueCommand
 *
 * Queue a rendering command via the GSlog debug/command system.
 * Used by lighting, material, and draw functions to batch commands.
 * 1388 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSlog_QueueCommand(u32 opcode, u32 paramCount, ...) {
    /* TODO: match -- 1388 bytes at 0x800D4F98 */
}
#pragma pop

/* ==================================================================
 * Matrix accessor functions (0x800D7230 - 0x800D75D0)
 *
 * A block of ~40 tiny functions (0x14-0x24 bytes each) that get/set
 * individual matrix elements, indices, and stack pointers.
 *
 * These follow two patterns:
 *   Pattern A (getter, 0x1C bytes):
 *     lwz r3, offset(r13/r0)   ; load from sda
 *     lwz r3, field(r3)        ; read struct field
 *     blr
 *
 *   Pattern B (setter, 0x24 bytes):
 *     lwz r4, offset(r13/r0)   ; load state ptr
 *     stw r3, field(r4)        ; store value
 *     blr
 * ================================================================== */

/* ==================================================================
 * fn_800D892C -- GSgfx_ConfigurePipeline
 *
 * Configure the full GX rendering pipeline. At 2320 bytes, this is
 * a major setup function. Called once per render mode change.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSgfx_ConfigurePipeline(void) {
    /* TODO: match -- 2320 bytes at 0x800D892C */
}
#pragma pop

/* ==================================================================
 * fn_800DE680 -- GSmaterial_Create
 *
 * Create and configure a new material. At 2376 bytes, this function
 * handles the full material setup including:
 *   - HSD MObj allocation (references "GSmaterial MObj")
 *   - PE descriptor configuration
 *   - Texture format validation
 *   - Environment map setup
 *   - Material capacity checking
 *
 * This function produces the material-related error strings.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSmaterial_Create(void* params) {
    /* TODO: match -- 2376 bytes at 0x800DE680 */
}
#pragma pop

/* ==================================================================
 * fn_800E1544 -- GSgfx_DrawDispatch
 *
 * Main draw command dispatch function. At 2792 bytes, this is the
 * largest function in the rendering pipeline. It interprets draw
 * commands and submits vertices/primitives to GX.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSgfx_DrawDispatch(void* drawList) {
    /* TODO: match -- 2792 bytes at 0x800E1544 */
}
#pragma pop

/* No-op functions (1) */
/* Address: 0x800DC874 | Size: 0x4 */
/* Forward declarations for self-referencing asm blocks */
extern void fn_800D6B00(void);
extern void fn_800D724C(u32 idx);
extern void fn_800D7268(u32 idx);
extern void fn_800D72A4(u32 idx);
extern void fn_800D72C4(u32 idx);
extern void fn_800D72E4(u32 idx);
extern void fn_800D7304(u32 idx);
extern void fn_800D7328(u32 idx);
extern void fn_800D7344(u32 idx);
extern void fn_800D7360(u32 idx);
extern void fn_800D737C(u32 idx);
extern void fn_800D7398(u32 idx);
extern void fn_800D73C4(u32 idx);
extern void fn_800D73F8(void);
extern void fn_800D740C(void);
extern void fn_800D7420(void);
extern void fn_800D7444(void);
extern void fn_800D7468(void);
extern void fn_800D748C(void);
extern void fn_800D74A0(void);
extern void fn_800D74B4(void);
extern void fn_800D74D0(void);
extern void fn_800D74EC(void);
extern void fn_800D7508(void);
extern void fn_800D7524(void);
extern void fn_800D7540(void);
extern void fn_800D7564(void);
extern void fn_800D7588(void);
extern void fn_800D75AC(void);
extern void fn_800D7650(void);
extern void fn_800D7868(void);
extern void fn_800D7940(void);
extern void fn_800D7A70(void);
extern void fn_800DA6F0(void);
extern void fn_800DA880(void);
extern void fn_800DB098(void);
extern void fn_800DB758(void);
extern void fn_800DD128(void);
extern void fn_800DE09C(void);
extern void fn_800DE128(void);
extern void fn_800DF930(void);
extern void fn_800DFABC(void);
extern void fn_800DFE98(void);
extern void fn_800E0290(void);
extern void fn_800E02C4(void*);
extern void fn_800E02E8(void);
extern void fn_800E032C(void);
extern void fn_800E0370(void);
extern void fn_800E03E8(void*);
extern void fn_800E0628(void*, void*);
extern void fn_800E064C(void*);
extern void fn_800E0678(void);
extern void fn_800E0698(void);
extern void fn_800E0C78(void);
extern void fn_800E0D24(void);

#if 0
asm void fn_800DC874(void) {
#include "src/game/gs_render_fn_800DC874.inc"
}
#else
void fn_800DC874(void) {}
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D45F8(void) {
#include "src/game/gs_render_fn_800D45F8.inc"
}
#else
u32 fn_800D45F8(void) { return *(u32*)lbl_8047AA80; }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D4604(void) {
#include "src/game/gs_render_fn_800D4604.inc"
}
#else
void fn_800D4604(u32 val) { *(u32*)lbl_8047AA80 = val; }
#endif
extern void fn_800B944C(void);
extern u32 lbl_8047CA30;
extern u32 lbl_8047CA34;
extern u32 lbl_8047CA38;
#if 1
asm void fn_800D55D0(void) {
#include "src/game/gs_render_fn_800D55D0.inc"
}
#else
void fn_800D55D0(void) { /* TODO */ }
#endif
extern void fn_800B9404(void);
extern u32 lbl_8047CA30;
extern u32 lbl_8047CA34;
extern u32 lbl_8047CA38;
#if 1
asm void fn_800D5648(void) {
#include "src/game/gs_render_fn_800D5648.inc"
}
#else
void fn_800D5648(void) { /* TODO */ }
#endif
extern void fn_800D7230(void);
#if 1
asm void fn_800D56C0(void) {
#include "src/game/gs_render_fn_800D56C0.inc"
}
#else
void fn_800D56C0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5724(void) {
#include "src/game/gs_render_fn_800D5724.inc"
}
#else
void fn_800D5724(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D579C(void) {
#include "src/game/gs_render_fn_800D579C.inc"
}
#else
void fn_800D579C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5814(void) {
#include "src/game/gs_render_fn_800D5814.inc"
}
#else
void fn_800D5814(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D58A0(void) {
#include "src/game/gs_render_fn_800D58A0.inc"
}
#else
void fn_800D58A0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D592C(void) {
#include "src/game/gs_render_fn_800D592C.inc"
}
#else
void fn_800D592C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D59B8(void) {
#include "src/game/gs_render_fn_800D59B8.inc"
}
#else
void fn_800D59B8(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5A38(void) {
#include "src/game/gs_render_fn_800D5A38.inc"
}
#else
void fn_800D5A38(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5AB0(void) {
#include "src/game/gs_render_fn_800D5AB0.inc"
}
#else
void fn_800D5AB0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5B28(void) {
#include "src/game/gs_render_fn_800D5B28.inc"
}
#else
void fn_800D5B28(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5BA0(void) {
#include "src/game/gs_render_fn_800D5BA0.inc"
}
#else
void fn_800D5BA0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5C18(void) {
#include "src/game/gs_render_fn_800D5C18.inc"
}
#else
void fn_800D5C18(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5CB8(void) {
#include "src/game/gs_render_fn_800D5CB8.inc"
}
#else
void fn_800D5CB8(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5D6C(void) {
#include "src/game/gs_render_fn_800D5D6C.inc"
}
#else
void fn_800D5D6C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5DD0(void) {
#include "src/game/gs_render_fn_800D5DD0.inc"
}
#else
void fn_800D5DD0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5E34(void) {
#include "src/game/gs_render_fn_800D5E34.inc"
}
#else
void fn_800D5E34(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5EB4(void) {
#include "src/game/gs_render_fn_800D5EB4.inc"
}
#else
void fn_800D5EB4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5F34(void) {
#include "src/game/gs_render_fn_800D5F34.inc"
}
#else
void fn_800D5F34(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D5FA4(void) {
#include "src/game/gs_render_fn_800D5FA4.inc"
}
#else
void fn_800D5FA4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D6028(void) {
#include "src/game/gs_render_fn_800D6028.inc"
}
#else
void fn_800D6028(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D60AC(void) {
#include "src/game/gs_render_fn_800D60AC.inc"
}
#else
void fn_800D60AC(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D6148(void) {
#include "src/game/gs_render_fn_800D6148.inc"
}
#else
void fn_800D6148(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D61E4(void) {
#include "src/game/gs_render_fn_800D61E4.inc"
}
#else
void fn_800D61E4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D6280(void) {
#include "src/game/gs_render_fn_800D6280.inc"
}
#else
void fn_800D6280(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D631C(void) {
#include "src/game/gs_render_fn_800D631C.inc"
}
#else
void fn_800D631C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D63B0(void) {
#include "src/game/gs_render_fn_800D63B0.inc"
}
#else
void fn_800D63B0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D6464(void) {
#include "src/game/gs_render_fn_800D6464.inc"
}
#else
void fn_800D6464(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D6518(void) {
#include "src/game/gs_render_fn_800D6518.inc"
}
#else
void fn_800D6518(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D65CC(void) {
#include "src/game/gs_render_fn_800D65CC.inc"
}
#else
void fn_800D65CC(void) { /* TODO */ }
#endif
extern void fn_800D75D0(void);
#if 1
asm void fn_800D6680(void) {
#include "src/game/gs_render_fn_800D6680.inc"
}
#else
void fn_800D6680(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D6728(void) {
#include "src/game/gs_render_fn_800D6728.inc"
}
#else
void fn_800D6728(void) { /* TODO */ }
#endif
extern void fn_800B928C(void);
extern u8 lbl_80314350[];
extern u8 lbl_804001F0[];
#if 1
asm void fn_800D67BC(void) {
#include "src/game/gs_render_fn_800D67BC.inc"
}
#else
void fn_800D67BC(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D6A00(void) {
#include "src/game/gs_render_fn_800D6A00.inc"
}
#else
void fn_800D6A00(void) { /* TODO */ }
#endif
#if 0
asm void fn_800D6A5C(void) {
#include "src/game/gs_render_fn_800D6A5C.inc"
}
#else
void fn_800D6A5C(u32 a, u32 b) { *(u32*)(lbl_804001F0 + 0xc) += a; *(u32*)(lbl_804001F0 + 0x4) += b; }
#endif
#if 1
asm void fn_800D6A80(void) {
#include "src/game/gs_render_fn_800D6A80.inc"
}
#else
void fn_800D6A80(void) { /* TODO */ }
#endif
extern u8 lbl_804007E8[];
#if 1
asm void fn_800D6B00(void) {
#include "src/game/gs_render_fn_800D6B00.inc"
}
#else
void fn_800D6B00(void) { /* TODO */ }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D724C(void) {
#include "src/game/gs_render_fn_800D724C.inc"
}
#else
void fn_800D724C(u32 idx) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x520 + idx*16); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7268(void) {
#include "src/game/gs_render_fn_800D7268.inc"
}
#else
void fn_800D7268(u32 idx) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x522 + idx*16); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7284(void) {
#include "src/game/gs_render_fn_800D7284.inc"
}
#else
void fn_800D7284(u32 idx) { u8 v = *(u8*)(lbl_8047AA80 + 0x520 + idx*16); *(volatile u8*)0xCC008000 = v; *(volatile u8*)0xCC008000 = v; }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D72A4(void) {
#include "src/game/gs_render_fn_800D72A4.inc"
}
#else
void fn_800D72A4(u32 idx) { u8 v = *(u8*)(lbl_8047AA80 + 0x520 + idx*16); *(volatile u8*)0xCC008000 = v; *(volatile u8*)0xCC008000 = v; }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D72C4(void) {
#include "src/game/gs_render_fn_800D72C4.inc"
}
#else
void fn_800D72C4(u32 idx) { u16 v = *(u16*)(lbl_8047AA80 + 0x522 + idx*16); *(volatile u16*)0xCC008000 = v; *(volatile u16*)0xCC008000 = v; }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D72E4(void) {
#include "src/game/gs_render_fn_800D72E4.inc"
}
#else
void fn_800D72E4(u32 idx) { u16 v = *(u16*)(lbl_8047AA80 + 0x522 + idx*16); *(volatile u16*)0xCC008000 = v; *(volatile u16*)0xCC008000 = v; }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7304(void) {
#include "src/game/gs_render_fn_800D7304.inc"
}
#else
void fn_800D7304(u32 idx) { u32 base = lbl_8047AA80 + idx*16; *(volatile f32*)0xCC008000 = *(f32*)(base + 0x528); *(volatile f32*)0xCC008000 = *(f32*)(base + 0x52c); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7328(void) {
#include "src/game/gs_render_fn_800D7328.inc"
}
#else
void fn_800D7328(u32 idx) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4e8 + idx*12); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7344(void) {
#include "src/game/gs_render_fn_800D7344.inc"
}
#else
void fn_800D7344(u32 idx) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4ec + idx*12); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7360(void) {
#include "src/game/gs_render_fn_800D7360.inc"
}
#else
void fn_800D7360(u32 idx) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4ec + idx*12); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D737C(void) {
#include "src/game/gs_render_fn_800D737C.inc"
}
#else
void fn_800D737C(u32 idx) { *(volatile u32*)0xCC008000 = *(u32*)(lbl_8047AA80 + 0x4f0 + idx*12); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7398(void) {
#include "src/game/gs_render_fn_800D7398.inc"
}
#else
void fn_800D7398(u32 idx) { u32 base = lbl_8047AA80 + idx*12; *(volatile u8*)0xCC008000 = *(u8*)(base + 0x4e8); *(volatile u8*)0xCC008000 = *(u8*)(base + 0x4e9); *(volatile u8*)0xCC008000 = *(u8*)(base + 0x4ea); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D73C4(void) {
#include "src/game/gs_render_fn_800D73C4.inc"
}
#else
void fn_800D73C4(u32 idx) { u32 base = lbl_8047AA80 + idx*12; *(volatile u8*)0xCC008000 = *(u8*)(base + 0x4e8); *(volatile u8*)0xCC008000 = *(u8*)(base + 0x4e9); *(volatile u8*)0xCC008000 = *(u8*)(base + 0x4ea); *(volatile u8*)0xCC008000 = *(u8*)(base + 0x4eb); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D73F8(void) {
#include "src/game/gs_render_fn_800D73F8.inc"
}
#else
void fn_800D73F8(void) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4c8); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D740C(void) {
#include "src/game/gs_render_fn_800D740C.inc"
}
#else
void fn_800D740C(void) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4cc); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7420(void) {
#include "src/game/gs_render_fn_800D7420.inc"
}
#else
void fn_800D7420(void) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4c8); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4c9); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ca); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7444(void) {
#include "src/game/gs_render_fn_800D7444.inc"
}
#else
void fn_800D7444(void) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4cc); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4ce); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4d0); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7468(void) {
#include "src/game/gs_render_fn_800D7468.inc"
}
#else
void fn_800D7468(void) { *(volatile f32*)0xCC008000 = *(f32*)(lbl_8047AA80 + 0x4d4); *(volatile f32*)0xCC008000 = *(f32*)(lbl_8047AA80 + 0x4d8); *(volatile f32*)0xCC008000 = *(f32*)(lbl_8047AA80 + 0x4dc); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D748C(void) {
#include "src/game/gs_render_fn_800D748C.inc"
}
#else
void fn_800D748C(void) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ac); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D74A0(void) {
#include "src/game/gs_render_fn_800D74A0.inc"
}
#else
void fn_800D74A0(void) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b0); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D74B4(void) {
#include "src/game/gs_render_fn_800D74B4.inc"
}
#else
void fn_800D74B4(void) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ac); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ad); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D74D0(void) {
#include "src/game/gs_render_fn_800D74D0.inc"
}
#else
void fn_800D74D0(void) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ac); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ad); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D74EC(void) {
#include "src/game/gs_render_fn_800D74EC.inc"
}
#else
void fn_800D74EC(void) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b0); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b2); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7508(void) {
#include "src/game/gs_render_fn_800D7508.inc"
}
#else
void fn_800D7508(void) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b0); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b2); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7524(void) {
#include "src/game/gs_render_fn_800D7524.inc"
}
#else
void fn_800D7524(void) { *(volatile f32*)0xCC008000 = *(f32*)(lbl_8047AA80 + 0x4b8); *(volatile f32*)0xCC008000 = *(f32*)(lbl_8047AA80 + 0x4bc); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7540(void) {
#include "src/game/gs_render_fn_800D7540.inc"
}
#else
void fn_800D7540(void) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ac); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ad); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ae); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7564(void) {
#include "src/game/gs_render_fn_800D7564.inc"
}
#else
void fn_800D7564(void) { *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ac); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ad); *(volatile u8*)0xCC008000 = *(u8*)(lbl_8047AA80 + 0x4ae); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D7588(void) {
#include "src/game/gs_render_fn_800D7588.inc"
}
#else
void fn_800D7588(void) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b0); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b2); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b4); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D75AC(void) {
#include "src/game/gs_render_fn_800D75AC.inc"
}
#else
void fn_800D75AC(void) { *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b0); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b2); *(volatile u16*)0xCC008000 = *(u16*)(lbl_8047AA80 + 0x4b4); }
#endif
#if 1
asm void fn_800D75F4(void) {
#include "src/game/gs_render_fn_800D75F4.inc"
}
#else
void fn_800D75F4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D7650(void) {
#include "src/game/gs_render_fn_800D7650.inc"
}
#else
void fn_800D7650(void) { /* TODO */ }
#endif
extern void fn_800B7D74(void);
extern void fn_800B7D3C(void);
extern void fn_800B7874(void);
extern void fn_800B84E0(void);
extern u8 lbl_80314370[];
extern u8 lbl_803143B4[];
extern u8 lbl_803143D8[];
extern u8 lbl_803143A8[];
#if 1
asm void fn_800D76A8(void) {
#include "src/game/gs_render_fn_800D76A8.inc"
}
#else
void fn_800D76A8(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D7820(void) {
#include "src/game/gs_render_fn_800D7820.inc"
}
#else
void fn_800D7820(void) { /* TODO */ }
#endif
#if 0
asm void fn_800D7868(void) {
#include "src/game/gs_render_fn_800D7868.inc"
}
#else
void fn_800D7868(u8* arr, u32 idx, u32 a, u32 b, u32 c, u8 d, u32 e, u8 f) {
    u8* p = arr + idx * 0x1c;
    p[0x8] = 1;
    *(u32*)(p + 0xc) = a;
    *(u32*)(p + 0x10) = b;
    *(u32*)(p + 0x14) = c;
    p[0x18] = d;
    *(u32*)(p + 0x1c) = e;
    p[0x20] = f;
}
#endif
extern u32 lbl_8047AAB0;
extern u32 lbl_8047AAAC;
extern u8 lbl_803144D0[];
extern u32 lbl_8047AAB4;
#if 1
asm void fn_800D7894(void) {
#include "src/game/gs_render_fn_800D7894.inc"
}
#else
void fn_800D7894(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D7940(void) {
#include "src/game/gs_render_fn_800D7940.inc"
}
#else
void fn_800D7940(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D7A70(void) {
#include "src/game/gs_render_fn_800D7A70.inc"
}
#else
void fn_800D7A70(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAB0;
extern u32 lbl_8047AAA8;
extern u32 lbl_8047AAAC;
extern u32 lbl_8047AAB4;
#if 1
asm void fn_800D7B80(void) {
#include "src/game/gs_render_fn_800D7B80.inc"
}
#else
void fn_800D7B80(void) { /* TODO */ }
#endif
extern void fn_800D2584(void);
extern void fn_800D1D00(void);
extern void fn_800D1B3C(void);
extern void fn_800D1A70(void);
#if 1
asm void fn_800D7BF8(void) {
#include "src/game/gs_render_fn_800D7BF8.inc"
}
#else
void fn_800D7BF8(void) { /* TODO */ }
#endif
extern void fn_800BD4B4(void);
extern void fn_800BD504(void);
extern void fn_800BD554(void);
extern u32 lbl_8047AAC8;
extern u8 lbl_80314610[];
extern u32 lbl_8047AAC0;
extern u8 lbl_80400948[];
#if 1
asm void fn_800D7C74(void) {
#include "src/game/gs_render_fn_800D7C74.inc"
}
#else
void fn_800D7C74(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D7D10(void) {
#include "src/game/gs_render_fn_800D7D10.inc"
}
#else
void fn_800D7D10(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D7D90(void) {
#include "src/game/gs_render_fn_800D7D90.inc"
}
#else
void fn_800D7D90(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC4;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D7E5C(void) {
#include "src/game/gs_render_fn_800D7E5C.inc"
}
#else
void fn_800D7E5C(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AABC;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D7F14(void) {
#include "src/game/gs_render_fn_800D7F14.inc"
}
#else
void fn_800D7F14(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D7FE4(void) {
#include "src/game/gs_render_fn_800D7FE4.inc"
}
#else
void fn_800D7FE4(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AABC;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D8088(void) {
#include "src/game/gs_render_fn_800D8088.inc"
}
#else
void fn_800D8088(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D8154(void) {
#include "src/game/gs_render_fn_800D8154.inc"
}
#else
void fn_800D8154(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D81EC(void) {
#include "src/game/gs_render_fn_800D81EC.inc"
}
#else
void fn_800D81EC(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D8284(void) {
#include "src/game/gs_render_fn_800D8284.inc"
}
#else
void fn_800D8284(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D834C(void) {
#include "src/game/gs_render_fn_800D834C.inc"
}
#else
void fn_800D834C(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAB8;
extern u32 lbl_8047AABC;
extern u32 lbl_8047AAC4;
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC8;
#if 1
asm void fn_800D83E4(void) {
#include "src/game/gs_render_fn_800D83E4.inc"
}
#else
void fn_800D83E4(void) { /* TODO */ }
#endif
extern void fn_800B857C(void);
extern void fn_800BD58C(void);
extern u8 lbl_80314404[];
extern u8 lbl_80314454[];
extern u8 lbl_803144A8[];
extern u8 lbl_80314424[];
#if 1
asm void fn_800D848C(void) {
#include "src/game/gs_render_fn_800D848C.inc"
}
#else
void fn_800D848C(void) { /* TODO */ }
#endif
extern void fn_800BAE34(void);
extern void fn_800BACA0(void);
extern void fn_800BB098(void);
extern void fn_800BAFFC(void);
extern u8 lbl_80314530[];
extern u32 lbl_8047CA40;
extern u32 lbl_8047CA48;
extern u8 lbl_80314510[];
extern u8 lbl_803144F0[];
#if 1
asm void fn_800D85D4(void) {
#include "src/game/gs_render_fn_800D85D4.inc"
}
#else
void fn_800D85D4(void) { /* TODO */ }
#endif
extern void fn_800BBC34(void);
extern void fn_800BBC0C(void);
#if 1
asm void fn_800D87AC(void) {
#include "src/game/gs_render_fn_800D87AC.inc"
}
#else
void fn_800D87AC(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D888C(void) {
#include "src/game/gs_render_fn_800D888C.inc"
}
#else
void fn_800D888C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D88DC(void) {
#include "src/game/gs_render_fn_800D88DC.inc"
}
#else
void fn_800D88DC(void) { /* TODO */ }
#endif
extern void fn_800BA6B0(void);
extern void fn_800BA6F4(void);
extern void fn_800B884C(void);
extern void fn_800BC8C8(void);
extern void fn_800BBAF8(void);
extern void fn_800BB97C(void);
extern void fn_800BB81C(void);
extern void fn_800BC6F0(void);
extern void fn_800BC228(void);
extern void fn_800BC290(void);
extern void fn_800BC1A0(void);
extern void fn_800BC1E4(void);
extern void fn_800BC454(void);
extern void fn_800BC4C0(void);
extern void fn_800BB780(void);
extern void fn_800BBC7C(void);
extern void fn_800BBCE0(void);
extern void fn_800BBE8C(void);
extern void fn_800BBF98(void);
extern void fn_800BBFDC(void);
extern void fn_800BC3E0(void);
extern void fn_800BC580(void);
#if 1
asm void fn_800D923C(void) {
#include "src/game/gs_render_fn_800D923C.inc"
}
#else
void fn_800D923C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D963C(void) {
#include "src/game/gs_render_fn_800D963C.inc"
}
#else
void fn_800D963C(void) { /* TODO */ }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D9AF0(void) {
#include "src/game/gs_render_fn_800D9AF0.inc"
}
#else
void fn_800D9AF0(u16* a, u16* b, u16* c, u16* d) { *a = *(u16*)(lbl_8047AA80 + 0x476); *b = *(u16*)(lbl_8047AA80 + 0x478); *c = *(u16*)(lbl_8047AA80 + 0x47a); *d = *(u16*)(lbl_8047AA80 + 0x47c); }
#endif
extern u32 lbl_8047AA80;
#if 0
asm void fn_800D9B24(void) {
#include "src/game/gs_render_fn_800D9B24.inc"
}
#else
void fn_800D9B24(u16* a, u16* b, u16* c, u16* d) { *a = *(u16*)(lbl_8047AA80 + 0x46e); *b = *(u16*)(lbl_8047AA80 + 0x470); *c = *(u16*)(lbl_8047AA80 + 0x472); *d = *(u16*)(lbl_8047AA80 + 0x474); }
#endif
extern void fn_800BD2E0(void);
extern u32 lbl_8047CA50;
extern u32 lbl_8047CA54;
#if 1
asm void fn_800D9B58(void) {
#include "src/game/gs_render_fn_800D9B58.inc"
}
#else
void fn_800D9B58(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D9BD0(void) {
#include "src/game/gs_render_fn_800D9BD0.inc"
}
#else
void fn_800D9BD0(void) { /* TODO */ }
#endif
extern void fn_800D21C8(void);
extern u32 lbl_8047CA60;
extern u32 lbl_8047CA68;
extern u32 lbl_8047CA50;
extern u32 lbl_8047CA58;
#if 1
asm void fn_800D9C24(void) {
#include "src/game/gs_render_fn_800D9C24.inc"
}
#else
void fn_800D9C24(void) { /* TODO */ }
#endif
extern void fn_800BD7A0(void);
extern void fn_800D2150(void);
#if 1
asm void fn_800D9D68(void) {
#include "src/game/gs_render_fn_800D9D68.inc"
}
#else
void fn_800D9D68(void) { /* TODO */ }
#endif
extern void fn_8019BD18(void);
extern u32 lbl_8047AA8C;
#if 1
asm void fn_800D9E4C(void) {
#include "src/game/gs_render_fn_800D9E4C.inc"
}
#else
void fn_800D9E4C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800D9ED8(void) {
#include "src/game/gs_render_fn_800D9ED8.inc"
}
#else
void fn_800D9ED8(void) { /* TODO */ }
#endif
extern void fn_800BD870(void);
#if 1
asm void fn_800D9FB4(void) {
#include "src/game/gs_render_fn_800D9FB4.inc"
}
#else
void fn_800D9FB4(void) { /* TODO */ }
#endif
extern void fn_800B94F0(void);
extern u8 lbl_8031453C[];
#if 1
asm void fn_800DA028(void) {
#include "src/game/gs_render_fn_800DA028.inc"
}
#else
void fn_800DA028(void) { /* TODO */ }
#endif
extern void fn_800BCFDC(void);
#if 1
asm void fn_800DA08C(void) {
#include "src/game/gs_render_fn_800DA08C.inc"
}
#else
void fn_800DA08C(void) { /* TODO */ }
#endif
extern void fn_800BC618(void);
extern u8 lbl_8031457C[];
extern u8 lbl_8031456C[];
#if 1
asm void fn_800DA100(void) {
#include "src/game/gs_render_fn_800DA100.inc"
}
#else
void fn_800DA100(void) { /* TODO */ }
#endif
extern void fn_800BCE88(void);
extern void fn_800BCEBC(void);
extern u8 lbl_8031454C[];
#if 1
asm void fn_800DA1E8(void) {
#include "src/game/gs_render_fn_800DA1E8.inc"
}
#else
void fn_800DA1E8(void) { /* TODO */ }
#endif
extern void fn_800BD008(void);
#if 1
asm void fn_800DA3B0(void) {
#include "src/game/gs_render_fn_800DA3B0.inc"
}
#else
void fn_800DA3B0(void) { /* TODO */ }
#endif
extern void fn_800BCDDC(void);
extern u8 lbl_803145D0[];
#if 1
asm void fn_800DA428(void) {
#include "src/game/gs_render_fn_800DA428.inc"
}
#else
void fn_800DA428(void) { /* TODO */ }
#endif
extern u8 lbl_8031459C[];
extern u8 lbl_803145A8[];
#if 1
asm void fn_800DA4C4(void) {
#include "src/game/gs_render_fn_800DA4C4.inc"
}
#else
void fn_800DA4C4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DA578(void) {
#include "src/game/gs_render_fn_800DA578.inc"
}
#else
void fn_800DA578(void) { /* TODO */ }
#endif
extern void jumptable_803152B8();
#if 1
asm void fn_800DA6F0(void) {
#include "src/game/gs_render_fn_800DA6F0.inc"
}
#else
void fn_800DA6F0(void) { /* TODO */ }
#endif
extern void jumptable_80315340();
extern void jumptable_80315320();
#if 1
asm void fn_800DA880(void) {
#include "src/game/gs_render_fn_800DA880.inc"
}
#else
void fn_800DA880(void) { /* TODO */ }
#endif
extern void fn_800E24B0(void);
extern void fn_800E209C(void);
#if 1
asm void fn_800DACC0(void) {
#include "src/game/gs_render_fn_800DACC0.inc"
}
#else
void fn_800DACC0(void) { /* TODO */ }
#endif
extern void fn_800BD0FC(void);
#if 1
asm void fn_800DAD10(void) {
#include "src/game/gs_render_fn_800DAD10.inc"
}
#else
void fn_800DAD10(void) { /* TODO */ }
#endif
extern void fn_800E2AF8(void);
#if 1
asm void fn_800DADB4(void) {
#include "src/game/gs_render_fn_800DADB4.inc"
}
#else
void fn_800DADB4(void) { /* TODO */ }
#endif
extern void fn_800E2C04(void);
extern u32 lbl_8047AAD8;
extern u32 lbl_8047AAD4;
#if 1
asm void fn_800DAF60(void) {
#include "src/game/gs_render_fn_800DAF60.inc"
}
#else
void fn_800DAF60(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DB098(void) {
#include "src/game/gs_render_fn_800DB098.inc"
}
#else
void fn_800DB098(void) { /* TODO */ }
#endif
extern void jumptable_80315364();
#if 1
asm void fn_800DB758(void) {
#include "src/game/gs_render_fn_800DB758.inc"
}
#else
void fn_800DB758(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DB900(void) {
#include "src/game/gs_render_fn_800DB900.inc"
}
#else
void fn_800DB900(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DB988(void) {
#include "src/game/gs_render_fn_800DB988.inc"
}
#else
void fn_800DB988(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DB9F0(void) {
#include "src/game/gs_render_fn_800DB9F0.inc"
}
#else
void fn_800DB9F0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBA54(void) {
#include "src/game/gs_render_fn_800DBA54.inc"
}
#else
void fn_800DBA54(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBAA4(void) {
#include "src/game/gs_render_fn_800DBAA4.inc"
}
#else
void fn_800DBAA4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBB0C(void) {
#include "src/game/gs_render_fn_800DBB0C.inc"
}
#else
void fn_800DBB0C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBB84(void) {
#include "src/game/gs_render_fn_800DBB84.inc"
}
#else
void fn_800DBB84(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBBFC(void) {
#include "src/game/gs_render_fn_800DBBFC.inc"
}
#else
void fn_800DBBFC(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBCE4(void) {
#include "src/game/gs_render_fn_800DBCE4.inc"
}
#else
void fn_800DBCE4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBD70(void) {
#include "src/game/gs_render_fn_800DBD70.inc"
}
#else
void fn_800DBD70(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBE5C(void) {
#include "src/game/gs_render_fn_800DBE5C.inc"
}
#else
void fn_800DBE5C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBEB4(void) {
#include "src/game/gs_render_fn_800DBEB4.inc"
}
#else
void fn_800DBEB4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBF1C(void) {
#include "src/game/gs_render_fn_800DBF1C.inc"
}
#else
void fn_800DBF1C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBF78(void) {
#include "src/game/gs_render_fn_800DBF78.inc"
}
#else
void fn_800DBF78(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DBFD4(void) {
#include "src/game/gs_render_fn_800DBFD4.inc"
}
#else
void fn_800DBFD4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DC04C(void) {
#include "src/game/gs_render_fn_800DC04C.inc"
}
#else
void fn_800DC04C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DC0D4(void) {
#include "src/game/gs_render_fn_800DC0D4.inc"
}
#else
void fn_800DC0D4(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DC14C(void) {
#include "src/game/gs_render_fn_800DC14C.inc"
}
#else
void fn_800DC14C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DC1D4(void) {
#include "src/game/gs_render_fn_800DC1D4.inc"
}
#else
void fn_800DC1D4(void) { /* TODO */ }
#endif
extern void fn_800EF504(void);
extern u8 lbl_80400EE0[];
extern u8 lbl_8047AAE0;
#if 1
asm void fn_800DC298(void) {
#include "src/game/gs_render_fn_800DC298.inc"
}
#else
void fn_800DC298(void) { /* TODO */ }
#endif
extern void fn_800EF4DC(void);
extern void fn_800EF590(void);
extern void fn_800EF578(void);
extern void fn_800EF548(void);
extern u8 lbl_8047AAE0;
#if 1
asm void fn_800DC390(void) {
#include "src/game/gs_render_fn_800DC390.inc"
}
#else
void fn_800DC390(void) { /* TODO */ }
#endif
extern u8 lbl_8047AAE0;
#if 0
asm void fn_800DC540(void) {
#include "src/game/gs_render_fn_800DC540.inc"
}
#else
void fn_800DC540(void) {
    lbl_80400EE0[0] = 0;
    lbl_8047AAE0 = 0;
    lbl_80400EE0[0x14] = 0;
    lbl_80400EE0[0x28] = 0;
    lbl_80400EE0[0x3c] = 0;
}
#endif
extern void fn_800EF1E8(void);
extern u8 lbl_8047AAE0;
#if 1
asm void fn_800DC560(void) {
#include "src/game/gs_render_fn_800DC560.inc"
}
#else
void fn_800DC560(void) { /* TODO */ }
#endif
extern void fn_801A6370(void);
extern void fn_801A6408(void);
extern u32 lbl_8047AAEC;
extern u32 lbl_8047CA80;
extern u32 lbl_8047CA70;
extern u32 lbl_8047CA74;
extern u32 lbl_8047CA78;
extern u32 lbl_8047AAF0;
#if 1
asm void fn_800DC6D8(void) {
#include "src/game/gs_render_fn_800DC6D8.inc"
}
#else
void fn_800DC6D8(void) { /* TODO */ }
#endif
extern void fn_801A49C0(u32);
extern void fn_801A48F4(u32);
extern void fn_801A66E0(void);
extern void fn_801A426C(void);
extern void fn_801C028C(void);
extern void fn_800D37CC(void);
extern void fn_801C027C(void);
extern u32 lbl_8047CA78;
extern u32 lbl_8047AAF4;
extern u32 lbl_8047CA88;
#if 1
asm void fn_800DC878(void) {
#include "src/game/gs_render_fn_800DC878.inc"
}
#else
void fn_800DC878(void) { /* TODO */ }
#endif
extern void fn_801A497C(void);
extern void fn_801A48B0(void);
#if 1
asm void fn_800DCA10(void) {
#include "src/game/gs_render_fn_800DCA10.inc"
}
#else
void fn_800DCA10(void) { /* TODO */ }
#endif
#if 0
asm void fn_800DCA9C(void) {
#include "src/game/gs_render_fn_800DCA9C.inc"
}
#else
u8 fn_800DCA9C(u8* obj) { return obj[0x70]; }
#endif
#if 0
asm void fn_800DCAA4(void) {
#include "src/game/gs_render_fn_800DCAA4.inc"
}
#else
void fn_800DCAA4(u8* obj) { obj[0x3] = 0; }
#endif
#if 0
asm void fn_800DCAB0(void) {
#include "src/game/gs_render_fn_800DCAB0.inc"
}
#else
void fn_800DCAB0(u8* obj) { if (!obj[0x2]) return; obj[0x3] = 1; obj[0x70] = 0; obj[0x71] = 1; }
#endif
#if 0
asm void fn_800DCAD4(void) {
#include "src/game/gs_render_fn_800DCAD4.inc"
}
#else
void fn_800DCAD4(u8* obj, u32 val) { *(u32*)((u8*)obj + 0x5c) = val; }
#endif
#if 0
asm void fn_800DCADC(void) {
#include "src/game/gs_render_fn_800DCADC.inc"
}
#else
void fn_800DCADC(u8* obj, f32 val) { if (obj[0x2]) *(f32*)((u8*)obj + 0x68) = val; }
#endif
extern u32 lbl_8047CA88;
#if 1
asm void fn_800DCAF0(void) {
#include "src/game/gs_render_fn_800DCAF0.inc"
}
#else
void fn_800DCAF0(void) { /* TODO */ }
#endif
extern u32 lbl_8047CA78;
extern u32 lbl_8047AAF4;
#if 1
asm void fn_800DCB78(void) {
#include "src/game/gs_render_fn_800DCB78.inc"
}
#else
void fn_800DCB78(void) { /* TODO */ }
#endif
#if 0
asm void fn_800DCC2C(void) {
#include "src/game/gs_render_fn_800DCC2C.inc"
}
#else
u8 fn_800DCC2C(u8* obj) { return obj[0x2]; }
#endif
#if 0
asm void fn_800DCC34(void) {
#include "src/game/gs_render_fn_800DCC34.inc"
}
#else
void fn_800DCC34(u8* obj, u8 val) { obj[0x1] = val; }
#endif
#if 0
asm void fn_800DCC3C(void) {
#include "src/game/gs_render_fn_800DCC3C.inc"
}
#else
void fn_800DCC3C(u8* obj) { fn_801A48F4(*(u32*)((u8*)obj + 0xc)); }
#endif
#if 0
asm void fn_800DCC60(void) {
#include "src/game/gs_render_fn_800DCC60.inc"
}
#else
void fn_800DCC60(u8* obj) { fn_801A49C0(*(u32*)((u8*)obj + 0xc)); }
#endif
extern void fn_801A4A48(void);
#if 1
asm void fn_800DCC84(void) {
#include "src/game/gs_render_fn_800DCC84.inc"
}
#else
void fn_800DCC84(void) { /* TODO */ }
#endif
extern void fn_801A68F8(void);
extern void fn_801A6910(void);
#if 1
asm void fn_800DCCF0(void) {
#include "src/game/gs_render_fn_800DCCF0.inc"
}
#else
void fn_800DCCF0(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DCD98(void) {
#include "src/game/gs_render_fn_800DCD98.inc"
}
#else
void fn_800DCD98(void) { /* TODO */ }
#endif
extern void fn_801A4344(void);
extern u32 lbl_8047AAF0;
extern u32 lbl_8047AAEC;
extern u32 lbl_8047CA70;
extern u32 lbl_8047CA78;
extern u32 lbl_8047AAF4;
#if 1
asm void fn_800DCE4C(void) {
#include "src/game/gs_render_fn_800DCE4C.inc"
}
#else
void fn_800DCE4C(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAF0;
extern u32 lbl_8047AAEC;
extern u32 lbl_8047CA78;
extern u32 lbl_8047CA8C;
#if 1
asm void fn_800DCFBC(void) {
#include "src/game/gs_render_fn_800DCFBC.inc"
}
#else
void fn_800DCFBC(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAF0;
extern u32 lbl_8047AAE8;
extern u32 lbl_8047AAEC;
#if 1
asm void fn_800DD0B8(void) {
#include "src/game/gs_render_fn_800DD0B8.inc"
}
#else
void fn_800DD0B8(void) { /* TODO */ }
#endif
extern void fn_80196E10(void);
extern u8 lbl_8047CA90[];
extern u8 lbl_8047CA98[];
extern u32 lbl_8047CA70;
extern u32 lbl_8047AAF4;
#if 1
asm void fn_800DD128(void) {
#include "src/game/gs_render_fn_800DD128.inc"
}
#else
void fn_800DD128(void) { /* TODO */ }
#endif
extern void fn_801A4B00(void);
extern void fn_801A4D20(void);
extern void fn_801A4F54(void);
extern u32 lbl_8047AAEC;
extern u32 lbl_8047AAF0;
#if 1
asm void fn_800DD174(void) {
#include "src/game/gs_render_fn_800DD174.inc"
}
#else
void fn_800DD174(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAF8;
extern u32 lbl_8047AB08;
extern u32 lbl_8047AAFA;
extern u32 lbl_8047AAFC;
#if 1
asm void fn_800DD270(void) {
#include "src/game/gs_render_fn_800DD270.inc"
}
#else
void fn_800DD270(void) { /* TODO */ }
#endif
extern u32 lbl_8047AB08;
#if 0
asm void fn_800DD384(void) {
#include "src/game/gs_render_fn_800DD384.inc"
}
#else
u32 fn_800DD384(void) { return lbl_8047AB08; }
#endif
extern void fn_800A2998(void);
extern void fn_800DE680(void);
extern u32 strlen(const char* s);
extern u32 lbl_8047AB11;
extern u8 lbl_80400F30[];
extern u8 lbl_802704B4[];
extern u8 lbl_80400F44[];
extern u32 lbl_8047AAFC;
extern u32 lbl_8047AB08;
extern u32 lbl_8047AB0C;
extern u32 lbl_8047AB00;
extern u32 lbl_8047AB04;
#if 1
asm void fn_800DD38C(void) {
#include "src/game/gs_render_fn_800DD38C.inc"
}
#else
void fn_800DD38C(void) { /* TODO */ }
#endif
extern u32 lbl_8047AB11;
extern u8 lbl_80401044[];
extern u8 lbl_80401058[];
extern u32 lbl_8047AAFC;
extern u32 lbl_8047AB08;
extern u32 lbl_8047AB0C;
extern u32 lbl_8047AB00;
extern u32 lbl_8047AB04;
#if 1
asm void fn_800DD970(void) {
#include "src/game/gs_render_fn_800DD970.inc"
}
#else
void fn_800DD970(void) { /* TODO */ }
#endif
extern u32 lbl_8047AB10;
extern u32 lbl_8047AB04;
extern u32 lbl_8047AB11;
extern u32 lbl_8047AAF8;
extern u32 lbl_8047AB0C;
extern u32 lbl_8047AAFA;
extern u32 lbl_8047AAFC;
extern u32 lbl_8047AB00;
#if 1
asm void fn_800DDF54(void) {
#include "src/game/gs_render_fn_800DDF54.inc"
}
#else
void fn_800DDF54(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DE09C(void) {
#include "src/game/gs_render_fn_800DE09C.inc"
}
#else
void fn_800DE09C(void) { /* TODO */ }
#endif
extern void jumptable_80315388();
extern void __va_arg();
extern u8 lbl_80401168[];
extern u8 lbl_80401178[];
extern u32 lbl_80478AE8;
extern u8 lbl_8047CAA0[];
extern u8 lbl_8047CAA8[];
#if 1
asm void fn_800DE128(void) {
#include "src/game/gs_render_fn_800DE128.inc"
}
#else
void fn_800DE128(void) { /* TODO */ }
#endif
extern void fn_800B8DF4(void);
extern void fn_801BBD3C(void);
#if 1
asm void fn_800DEFC8(void) {
#include "src/game/gs_render_fn_800DEFC8.inc"
}
#else
void fn_800DEFC8(void) { /* TODO */ }
#endif
extern void fn_801BBD84(void);
extern void fn_800EF4FC(void);
extern void fn_800EF4F4(void);
extern void fn_800EF3E0(void);
extern void fn_800EF4E4(void);
extern u32 lbl_8047CAC8;
#if 1
asm void fn_800DF028(void) {
#include "src/game/gs_render_fn_800DF028.inc"
}
#else
void fn_800DF028(void) { /* TODO */ }
#endif
#if 0
asm void fn_800DF11C(void) {
#include "src/game/gs_render_fn_800DF11C.inc"
}
#else
void fn_800DF11C(u8* src, u8* dst) { dst[0] = src[0xc]; dst[1] = src[0xd]; dst[2] = src[0xe]; dst[3] = src[0xf]; }
#endif
extern void fn_801A6DDC(u32);
extern u32 lbl_8047CAD0;
extern f32 lbl_8047CACC;
#if 1
asm void fn_800DF140(void) {
#include "src/game/gs_render_fn_800DF140.inc"
}
#else
void fn_800DF140(void) { /* TODO */ }
#endif
extern f32 lbl_8047CACC;
#if 0
asm void fn_800DF188(void) {
#include "src/game/gs_render_fn_800DF188.inc"
}
#else
void fn_800DF188(u8* obj) { obj[0x1] = (u8)(s32)(lbl_8047CACC * *(f32*)(*(u32*)((u8*)obj + 0x8) + 0xc)); }
#endif
#if 0
asm void fn_800DF1B8(void) {
#include "src/game/gs_render_fn_800DF1B8.inc"
}
#else
void fn_800DF1B8(u8* obj, f32 val) { u32 ptr = *(u32*)(obj + 0x20); *(f32*)(obj + 0x34) = val; if (ptr) *(f32*)(ptr + 0x50) = val; }
#endif
#if 0
asm void fn_800DF1D0(void) {
#include "src/game/gs_render_fn_800DF1D0.inc"
}
#else
void fn_800DF1D0(u8* obj, u32 a, u32 b, f32 c, u32 d) { *(u32*)(obj+0x2c) = a; *(u32*)(obj+0x30) = b; *(f32*)(obj+0x34) = c; *(u32*)(obj+0x28) = d; }
#endif
#if 0
asm void fn_800DF1E4(void) {
#include "src/game/gs_render_fn_800DF1E4.inc"
}
#else
void fn_800DF1E4(u8* dst, u8* src) { dst[0xc] = src[0]; dst[0xd] = src[1]; dst[0xe] = src[2]; dst[0xf] = src[3]; }
#endif
#if 0
asm void fn_800DF208(void) {
#include "src/game/gs_render_fn_800DF208.inc"
}
#else
void fn_800DF208(u8* obj, u32 a, u32 b, u32 c, u32 d) { *(u32*)(obj+0x10) = a; *(u32*)(obj+0x14) = b; *(u32*)(obj+0x18) = c; *(u32*)(obj+0x1c) = d; }
#endif
#if 0
asm void fn_800DF21C(void) {
#include "src/game/gs_render_fn_800DF21C.inc"
}
#else
void fn_800DF21C(u8* obj) { fn_801A6DDC(*(u32*)((u8*)obj + 0x8)); }
#endif
#if 0
asm void fn_800DF240(void) {
#include "src/game/gs_render_fn_800DF240.inc"
}
#else
u16 fn_800DF240(u8* obj) { return *(u16*)((u8*)obj + 0x2); }
#endif
extern void fn_801BBED4(void);
extern void fn_801A6FF0(void);
#if 1
asm void fn_800DF248(void) {
#include "src/game/gs_render_fn_800DF248.inc"
}
#else
void fn_800DF248(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DF384(void) {
#include "src/game/gs_render_fn_800DF384.inc"
}
#else
void fn_800DF384(void) { /* TODO */ }
#endif
extern void fn_801A8458(void);
#if 1
asm void fn_800DF3F0(void) {
#include "src/game/gs_render_fn_800DF3F0.inc"
}
#else
void fn_800DF3F0(void) { /* TODO */ }
#endif
#if 0
asm void fn_800DF470(void) {
#include "src/game/gs_render_fn_800DF470.inc"
}
#else
void fn_800DF470(u8* obj) {
    u32 v = *(u32*)(obj + 0x3c);
    if ((u32)(v + 0x01020000) == 0xfefe) return;
    *(u32*)(*(u32*)(obj + 0x8) + 0x10) = v;
    *(u32*)(obj + 0x3c) = 0xfefefefe;
}
#endif
#if 1
asm void fn_800DF498(void) {
#include "src/game/gs_render_fn_800DF498.inc"
}
#else
void fn_800DF498(void) { /* TODO */ }
#endif
extern void fn_801A8428(void);
extern void fn_801A8440(void);
#if 1
asm void fn_800DF504(void) {
#include "src/game/gs_render_fn_800DF504.inc"
}
#else
void fn_800DF504(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DF550(void) {
#include "src/game/gs_render_fn_800DF550.inc"
}
#else
void fn_800DF550(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DF608(void) {
#include "src/game/gs_render_fn_800DF608.inc"
}
#else
void fn_800DF608(void) { /* TODO */ }
#endif
extern u32 lbl_8047AB20;
extern u32 lbl_8047AB1C;
#if 1
asm void fn_800DF7A4(void) {
#include "src/game/gs_render_fn_800DF7A4.inc"
}
#else
void fn_800DF7A4(void) { /* TODO */ }
#endif
extern void fn_801A7CFC(void);
extern u32 lbl_8047AB20;
extern u32 lbl_8047AB18;
extern u32 lbl_8047AB1C;
extern u8 lbl_80315490[];
#if 1
asm void fn_800DF854(void) {
#include "src/game/gs_render_fn_800DF854.inc"
}
#else
void fn_800DF854(void) { /* TODO */ }
#endif
extern void fn_80193B30(void);
extern u8 lbl_8036CB30[];
#if 1
asm void fn_800DF8CC(void) {
#include "src/game/gs_render_fn_800DF8CC.inc"
}
#else
void fn_800DF8CC(void) { /* TODO */ }
#endif
extern void fn_801B7C60(void);
extern void fn_801B6DC0(void);
extern void fn_801B6F5C(void);
extern void fn_801B707C(void);
extern void fn_801B6E74(void);
extern void fn_801B64EC(void);
extern void fn_801B6CD8(void);
extern void fn_801B5F08(void);
#if 1
asm void fn_800DF930(void) {
#include "src/game/gs_render_fn_800DF930.inc"
}
#else
void fn_800DF930(void) { /* TODO */ }
#endif
extern void fn_801BBD60(void);
extern void fn_801BE4CC(void);
extern void fn_801A6DC4(void);
extern void fn_801A6D5C(void);
extern void fn_801A6DA0(void);
extern u8 lbl_803154E4[];
extern u32 lbl_8047CAC8;
#if 1
asm void fn_800DFABC(void) {
#include "src/game/gs_render_fn_800DFABC.inc"
}
#else
void fn_800DFABC(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DFE98(void) {
#include "src/game/gs_render_fn_800DFE98.inc"
}
#else
void fn_800DFE98(void) { /* TODO */ }
#endif
#if 1
asm void fn_800DFEEC(void) {
#include "src/game/gs_render_fn_800DFEEC.inc"
}
#else
void fn_800DFEEC(void) { /* TODO */ }
#endif
extern void fn_800A37CC(void*, void*, void*);
#if 0
asm void fn_800DFF98(void) {
#include "src/game/gs_render_fn_800DFF98.inc"
}
#else
void fn_800DFF98(void* a, void* b, void* c) { fn_800A37CC(b, c, a); }
#endif
extern void fn_800A3B9C(void*, void*, void*);
#if 0
asm void fn_800DFFCC(void) {
#include "src/game/gs_render_fn_800DFFCC.inc"
}
#else
void fn_800DFFCC(void* a, void* b, void* c) { fn_800A3B9C(b, c, a); }
#endif
extern void fn_800A3B7C(void);
#if 0
asm void fn_800E0000(void) {
#include "src/game/gs_render_fn_800E0000.inc"
}
#else
void fn_800E0000(void) { fn_800A3B7C(); }
#endif
extern void fn_800A3BD8(void);
#if 0
asm void fn_800E0020(void) {
#include "src/game/gs_render_fn_800E0020.inc"
}
#else
void fn_800E0020(void) { fn_800A3BD8(); }
#endif
extern void fn_800A3C00(void);
#if 0
asm void fn_800E0040(void) {
#include "src/game/gs_render_fn_800E0040.inc"
}
#else
void fn_800E0040(void) { fn_800A3C00(); }
#endif
extern void fn_800A3ADC(void*, void*);
#if 0
asm void fn_800E0060(void) {
#include "src/game/gs_render_fn_800E0060.inc"
}
#else
void fn_800E0060(void* a, void* b) { fn_800A3ADC(b, a); }
#endif
extern void fn_800A3B38(void);
#if 0
asm void fn_800E008C(void) {
#include "src/game/gs_render_fn_800E008C.inc"
}
#else
void fn_800E008C(void) { fn_800A3B38(); }
#endif
extern void fn_800A3AC0(void*, void*, f32);
extern f32 lbl_8047CAD8;
#if 0
asm void fn_800E00AC(void) {
#include "src/game/gs_render_fn_800E00AC.inc"
}
#else
void fn_800E00AC(void* a, void* b, f32 scale) {
    fn_800A3AC0(b, a, lbl_8047CAD8 / scale);
}
#endif
#if 0
asm void fn_800E00E0(void) {
#include "src/game/gs_render_fn_800E00E0.inc"
}
#else
void fn_800E00E0(f32* dst, f32* src) { dst[0] = -src[0]; dst[1] = -src[1]; dst[2] = -src[2]; }
#endif
#if 0
asm void fn_800E0108(void) {
#include "src/game/gs_render_fn_800E0108.inc"
}
#else
void fn_800E0108(f32* dst, f32* a, f32* b) {
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
    dst[2] = a[2] * b[2];
}
#endif
#if 0
asm void fn_800E013C(void) {
#include "src/game/gs_render_fn_800E013C.inc"
}
#else
void fn_800E013C(void* a, void* b) { fn_800A3AC0(b, a); }
#endif
extern void fn_800A3A9C(void*, void*, void*);
#if 0
asm void fn_800E0168(void) {
#include "src/game/gs_render_fn_800E0168.inc"
}
#else
void fn_800E0168(void* a, void* b, void* c) { fn_800A3A9C(b, c, a); }
#endif
extern void fn_800A3A78(void*, void*, void*);
#if 0
asm void fn_800E019C(void) {
#include "src/game/gs_render_fn_800E019C.inc"
}
#else
void fn_800E019C(void* a, void* b, void* c) { fn_800A3A78(b, c, a); }
#endif
#if 0
asm void fn_800E01D0(void) {
#include "src/game/gs_render_fn_800E01D0.inc"
}
#else
void fn_800E01D0(void* dst, void* src) { memcpy(dst, src, 0xc); }
#endif
#if 0
asm void fn_800E01F4(void) {
#include "src/game/gs_render_fn_800E01F4.inc"
}
#else
void fn_800E01F4(f32* arr, f32 x, f32 y, f32 z) { arr[0] = x; arr[1] = y; arr[2] = z; }
#endif
extern f32 lbl_8047CADC;
#if 0
asm void fn_800E0204(void) {
#include "src/game/gs_render_fn_800E0204.inc"
}
#else
void fn_800E0204(f32* arr) { arr[0] = lbl_8047CADC; arr[1] = lbl_8047CADC; arr[2] = lbl_8047CADC; }
#endif
extern void fn_800A3458(void);
#if 0
asm void fn_800E0218(void) {
#include "src/game/gs_render_fn_800E0218.inc"
}
#else
void fn_800E0218(void) { fn_800A3458(); }
#endif
extern void fn_800A2E64(void*, void*);
#if 0
asm void fn_800E0238(void) {
#include "src/game/gs_render_fn_800E0238.inc"
}
#else
void fn_800E0238(void* a, void* b) { fn_800A2E64(b, a); }
#endif
extern void fn_800A2EB4(void*, void*);
#if 0
asm void fn_800E0264(void) {
#include "src/game/gs_render_fn_800E0264.inc"
}
#else
void fn_800E0264(void* a, void* b) { fn_800A2EB4(b, a); }
#endif
extern void fn_800A2D98(void*, void*, void*);
#if 0
asm void fn_800E0290(void) {
#include "src/game/gs_render_fn_800E0290.inc"
}
#else
void fn_800E0290(void* a, void* b, void* c) { fn_800A2D98(b, c, a); }
#endif
extern void fn_800A335C(void*, void*);
#if 0
asm void fn_800E02C4(void) {
#include "src/game/gs_render_fn_800E02C4.inc"
}
#else
void fn_800E02C4(void* a) { fn_800A335C(a, a); }
#endif
extern void fn_800A3074(void*, u32);
#if 1
asm void fn_800E02E8(void) {
#include "src/game/gs_render_fn_800E02E8.inc"
}
#else
void fn_800E02E8(void) { /* TODO */ }
#endif
#if 1
asm void fn_800E032C(void) {
#include "src/game/gs_render_fn_800E032C.inc"
}
#else
void fn_800E032C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800E0370(void) {
#include "src/game/gs_render_fn_800E0370.inc"
}
#else
void fn_800E0370(void) { /* TODO */ }
#endif
extern void fn_800A32E8(void*, void*, f32, f32, f32);
#if 0
asm void fn_800E03B4(void) {
#include "src/game/gs_render_fn_800E03B4.inc"
}
#else
void fn_800E03B4(void* dst, f32* src) {
    fn_800A32E8(dst, dst, src[0], src[1], src[2]);
}
#endif
#if 0
asm void fn_800E03E8(void) {
#include "src/game/gs_render_fn_800E03E8.inc"
}
#else
void fn_800E03E8(void* a) { fn_800A32E8(a, a); }
#endif
extern void fn_800A33B4(void);
#if 0
asm void fn_800E040C(void) {
#include "src/game/gs_render_fn_800E040C.inc"
}
#else
void fn_800E040C(void) { fn_800A33B4(); }
#endif
extern u8 lbl_80315568[];
#if 1
asm void fn_800E042C(void) {
#include "src/game/gs_render_fn_800E042C.inc"
}
#else
void fn_800E042C(void) { /* TODO */ }
#endif
#if 1
asm void fn_800E048C(void) {
#include "src/game/gs_render_fn_800E048C.inc"
}
#else
void fn_800E048C(void) { /* TODO */ }
#endif
#if 0
asm void fn_800E04F4(void) {
#include "src/game/gs_render_fn_800E04F4.inc"
}
#else
void fn_800E04F4(void* a) { fn_800A3074(a, 0x5a); }
#endif
#if 0
asm void fn_800E0518(void) {
#include "src/game/gs_render_fn_800E0518.inc"
}
#else
void fn_800E0518(void* a) { fn_800A3074(a, 0x59); }
#endif
#if 0
asm void fn_800E053C(void) {
#include "src/game/gs_render_fn_800E053C.inc"
}
#else
void fn_800E053C(void* a) { fn_800A3074(a, 0x58); }
#endif
#if 1
asm void fn_800E0560(void) {
#include "src/game/gs_render_fn_800E0560.inc"
}
#else
void fn_800E0560(void) { /* TODO */ }
#endif
#if 1
asm void fn_800E05C0(void) {
#include "src/game/gs_render_fn_800E05C0.inc"
}
#else
void fn_800E05C0(void) { /* TODO */ }
#endif
#if 0
asm void fn_800E0628(void) {
#include "src/game/gs_render_fn_800E0628.inc"
}
#else
void fn_800E0628(void* dst, void* src) { memcpy(dst, src, 0x30); }
#endif
#if 0
asm void fn_800E064C(void) {
#include "src/game/gs_render_fn_800E064C.inc"
}
#else
void fn_800E064C(void* dst) { memcpy(dst, lbl_80315568, 0x30); }
#endif
extern void fn_800A3910(void);
#if 0
asm void fn_800E0678(void) {
#include "src/game/gs_render_fn_800E0678.inc"
}
#else
void fn_800E0678(void) { fn_800A3910(); }
#endif
extern void fn_800A39E0(void);
#if 0
asm void fn_800E0698(void) {
#include "src/game/gs_render_fn_800E0698.inc"
}
#else
void fn_800E0698(void) { fn_800A39E0(); }
#endif
extern void fn_800A3D3C(void*, void*, void*);
#if 0
asm void fn_800E06B8(void) {
#include "src/game/gs_render_fn_800E06B8.inc"
}
#else
void fn_800E06B8(void* a, void* b, void* c) { fn_800A3D3C(b, c, a); }
#endif
extern void fn_801ADAAC(void*, void*);
#if 0
asm void fn_800E06EC(void) {
#include "src/game/gs_render_fn_800E06EC.inc"
}
#else
void fn_800E06EC(void* a, void* b) { fn_801ADAAC(b, a); }
#endif
extern void fn_800A3CB0(void);
#if 0
asm void fn_800E0718(void) {
#include "src/game/gs_render_fn_800E0718.inc"
}
#else
void fn_800E0718(void) { fn_800A3CB0(); }
#endif
extern void fn_800A3C54(void*, void*, void*);
#if 0
asm void fn_800E0738(void) {
#include "src/game/gs_render_fn_800E0738.inc"
}
#else
void fn_800E0738(void* a, void* b, void* c) { fn_800A3C54(b, c, a); }
#endif
#if 0
asm void fn_800E076C(void) {
#include "src/game/gs_render_fn_800E076C.inc"
}
#else
void fn_800E076C(f32* dst, f32* src) { dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3]; }
#endif
#if 1
asm void fn_800E0790(void) {
#include "src/game/gs_render_fn_800E0790.inc"
}
#else
void fn_800E0790(void) { /* TODO */ }
#endif
extern u32 lbl_8047CAE4;
extern u32 lbl_8047CAE0;
extern u32 lbl_8047CAE8;
#if 1
asm void fn_800E07E4(void) {
#include "src/game/gs_render_fn_800E07E4.inc"
}
#else
void fn_800E07E4(void) { /* TODO */ }
#endif
extern u32 lbl_8047CAF0;
extern u32 lbl_8047CAF4;
#if 1
asm void fn_800E090C(void) {
#include "src/game/gs_render_fn_800E090C.inc"
}
#else
void fn_800E090C(void) { /* TODO */ }
#endif
extern u32 lbl_8047CAF0;
extern u32 lbl_8047CAF4;
#if 1
asm void fn_800E09B4(void) {
#include "src/game/gs_render_fn_800E09B4.inc"
}
#else
void fn_800E09B4(void) { /* TODO */ }
#endif
extern u32 lbl_8047CB00;
extern u32 lbl_8047CAF8;
extern u32 lbl_8047CB08;
extern u32 lbl_8047CAFC;
#if 1
asm void fn_800E09E8(void) {
#include "src/game/gs_render_fn_800E09E8.inc"
}
#else
void fn_800E09E8(void) { /* TODO */ }
#endif
extern void fn_801ADC7C(void);
extern u32 lbl_8047CB10;
#if 1
asm void fn_800E0BA0(void) {
#include "src/game/gs_render_fn_800E0BA0.inc"
}
#else
void fn_800E0BA0(void) { /* TODO */ }
#endif
#if 0
asm void fn_800E0BE4(void) {
#include "src/game/gs_render_fn_800E0BE4.inc"
}
#else
void fn_800E0BE4(void) { fn_801ADC7C(); }
#endif
extern u16 fn_801ADCD8(void);
#if 1
asm void fn_800E0C04(void) {
#include "src/game/gs_render_fn_800E0C04.inc"
}
#else
void fn_800E0C04(void) { /* TODO */ }
#endif
#if 0
asm void fn_800E0C54(void) {
#include "src/game/gs_render_fn_800E0C54.inc"
}
#else
u16 fn_800E0C54(void) { return fn_801ADCD8(); }
#endif
extern u32 lbl_80478C94;
#if 0
asm void fn_800E0C78(void) {
#include "src/game/gs_render_fn_800E0C78.inc"
}
#else
void fn_800E0C78(void) {
    u64 t = OSGetTime();
    *(u32*)lbl_80478C94 = (u32)t;
}
#endif
extern void fn_800CE318(void);
extern void fn_800C46B0(void);
extern u32 lbl_8047CB20;
extern u32 lbl_8047CB1C;
extern u32 lbl_8047CB24;
extern u32 lbl_8047CB28;
extern u32 lbl_8047CB18;
extern u32 lbl_8047CB34;
extern u32 lbl_8047CB30;
extern u8 lbl_804011B8[];
#if 1
asm void fn_800E0CA0(void) {
#include "src/game/gs_render_fn_800E0CA0.inc"
}
#else
void fn_800E0CA0(void) { /* TODO */ }
#endif
extern void fn_800CDBE0(void);
extern u32 lbl_8047CB38;
extern u32 lbl_8047CB30;
extern u32 lbl_8047CB40;
#if 1
asm void fn_800E0D24(void) {
#include "src/game/gs_render_fn_800E0D24.inc"
}
#else
void fn_800E0D24(void) { /* TODO */ }
#endif
extern u8 lbl_80270658[];
extern u32 lbl_8047AB30;
extern u32 lbl_8047AB68;
extern u32 lbl_8047AB64;
extern u32 lbl_8047AB38;
extern u32 lbl_8047AB34;
extern u32 lbl_8047AB28;
extern u32 lbl_8047AB4C;
extern u32 lbl_8047AB48;
extern u32 lbl_8047AB60;
extern u32 lbl_8047AB5C;
extern u32 lbl_8047AB58;
extern u32 lbl_8047AB54;
extern u32 lbl_8047AB50;
extern u32 lbl_8047CB50;
extern u32 lbl_8047CB48;
extern u32 lbl_8047AB3C;
#if 1
asm void fn_800E0E14(void) {
#include "src/game/gs_render_fn_800E0E14.inc"
}
#else
void fn_800E0E14(void) { /* TODO */ }
#endif
