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
extern void  fn_800DD970(const char* fmt, ...);        /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);

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
extern u8 lbl_8047AA80[];  /* GSgfx state pointer (sda21) */
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

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 6 functions matched
 * =================================================================== */

extern u32 lbl_8047AB08;

/* Address: 0x800DCA9C | Size: 0x8 | Pattern: simple_getter */
u8 fn_800DCA9C(u8* obj) {
    return *(u8*)((u8*)obj + 0x70);
}

/* Address: 0x800DCAD4 | Size: 0x8 | Pattern: simple_setter */
void fn_800DCAD4(u8* obj, u32 val) {
    *(u32*)((u8*)obj + 0x5C) = val;
}

/* Address: 0x800DCC2C | Size: 0x8 | Pattern: simple_getter */
u8 fn_800DCC2C(u8* obj) {
    return *(u8*)((u8*)obj + 0x2);
}

/* Address: 0x800DCC34 | Size: 0x8 | Pattern: simple_setter */
void fn_800DCC34(u8* obj, u8 val) {
    *(u8*)((u8*)obj + 0x1) = val;
}

/* Address: 0x800DD384 | Size: 0x8 | Pattern: sda_getter */
u32 fn_800DD384(void) {
    return lbl_8047AB08;
}

/* Address: 0x800DF240 | Size: 0x8 | Pattern: simple_getter */
u16 fn_800DF240(u8* obj) {
    return *(u16*)((u8*)obj + 0x2);
}

/* ===================================================================
 * STUB FUNCTIONS -- All remaining functions in 0x800D3E4C-0x800E202C
 * that were not previously decompiled.
 * =================================================================== */

/* fn_800D45F8 | Size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D45F8(void) { /* TODO */ }
#pragma pop

/* fn_800D4604 | Size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D4604(void) { /* TODO */ }
#pragma pop

/* fn_800D4610 | Size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D4610(void) { /* TODO */ }
#pragma pop

/* fn_800D5504 -- GSlog_Init | Size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5504(void) { /* TODO */ }
#pragma pop

/* fn_800D55D0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D55D0(void) { /* TODO */ }
#pragma pop

/* fn_800D5648 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5648(void) { /* TODO */ }
#pragma pop

/* fn_800D56C0 | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D56C0(void) { /* TODO */ }
#pragma pop

/* fn_800D5724 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5724(void) { /* TODO */ }
#pragma pop

/* fn_800D579C | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D579C(void) { /* TODO */ }
#pragma pop

/* fn_800D5814 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5814(void) { /* TODO */ }
#pragma pop

/* fn_800D58A0 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D58A0(void) { /* TODO */ }
#pragma pop

/* fn_800D592C | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D592C(void) { /* TODO */ }
#pragma pop

/* fn_800D59B8 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D59B8(void) { /* TODO */ }
#pragma pop

/* fn_800D5A38 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5A38(void) { /* TODO */ }
#pragma pop

/* fn_800D5AB0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5AB0(void) { /* TODO */ }
#pragma pop

/* fn_800D5B28 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5B28(void) { /* TODO */ }
#pragma pop

/* fn_800D5BA0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5BA0(void) { /* TODO */ }
#pragma pop

/* fn_800D5C18 | Size: 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5C18(void) { /* TODO */ }
#pragma pop

/* fn_800D5CB8 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5CB8(void) { /* TODO */ }
#pragma pop

/* fn_800D5D6C | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5D6C(void) { /* TODO */ }
#pragma pop

/* fn_800D5DD0 | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5DD0(void) { /* TODO */ }
#pragma pop

/* fn_800D5E34 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5E34(void) { /* TODO */ }
#pragma pop

/* fn_800D5EB4 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5EB4(void) { /* TODO */ }
#pragma pop

/* fn_800D5F34 | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5F34(void) { /* TODO */ }
#pragma pop

/* fn_800D5FA4 | Size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5FA4(void) { /* TODO */ }
#pragma pop

/* fn_800D6028 | Size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6028(void) { /* TODO */ }
#pragma pop

/* fn_800D60AC | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D60AC(void) { /* TODO */ }
#pragma pop

/* fn_800D6148 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6148(void) { /* TODO */ }
#pragma pop

/* fn_800D61E4 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D61E4(void) { /* TODO */ }
#pragma pop

/* fn_800D6280 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6280(void) { /* TODO */ }
#pragma pop

/* fn_800D631C | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D631C(void) { /* TODO */ }
#pragma pop

/* fn_800D63B0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D63B0(void) { /* TODO */ }
#pragma pop

/* fn_800D6464 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6464(void) { /* TODO */ }
#pragma pop

/* fn_800D6518 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6518(void) { /* TODO */ }
#pragma pop

/* fn_800D65CC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D65CC(void) { /* TODO */ }
#pragma pop

/* fn_800D6680 | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6680(void) { /* TODO */ }
#pragma pop

/* fn_800D6728 | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6728(void) { /* TODO */ }
#pragma pop

/* fn_800D67BC -- MTX operations | Size: 0x244 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D67BC(void) { /* TODO */ }
#pragma pop

/* fn_800D6A00 | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6A00(void) { /* TODO */ }
#pragma pop

/* fn_800D6A5C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6A5C(void) { /* TODO */ }
#pragma pop

/* fn_800D6A80 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6A80(void) { /* TODO */ }
#pragma pop

/* fn_800D6B00 -- Large matrix setup | Size: 0x730 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6B00(void) { /* TODO */ }
#pragma pop

/* fn_800D7230 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7230(void) { /* TODO */ }
#pragma pop

/* fn_800D724C | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D724C(void) { /* TODO */ }
#pragma pop

/* fn_800D7268 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7268(void) { /* TODO */ }
#pragma pop

/* fn_800D7284 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7284(void) { /* TODO */ }
#pragma pop

/* fn_800D72A4 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D72A4(void) { /* TODO */ }
#pragma pop

/* fn_800D72C4 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D72C4(void) { /* TODO */ }
#pragma pop

/* fn_800D72E4 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D72E4(void) { /* TODO */ }
#pragma pop

/* fn_800D7304 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7304(void) { /* TODO */ }
#pragma pop

/* fn_800D7328 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7328(void) { /* TODO */ }
#pragma pop

/* fn_800D7344 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7344(void) { /* TODO */ }
#pragma pop

/* fn_800D7360 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7360(void) { /* TODO */ }
#pragma pop

/* fn_800D737C | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D737C(void) { /* TODO */ }
#pragma pop

/* fn_800D7398 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7398(void) { /* TODO */ }
#pragma pop

/* fn_800D73C4 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D73C4(void) { /* TODO */ }
#pragma pop

/* fn_800D73F8 | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D73F8(void) { /* TODO */ }
#pragma pop

/* fn_800D740C | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D740C(void) { /* TODO */ }
#pragma pop

/* fn_800D7420 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7420(void) { /* TODO */ }
#pragma pop

/* fn_800D7444 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7444(void) { /* TODO */ }
#pragma pop

/* fn_800D7468 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7468(void) { /* TODO */ }
#pragma pop

/* fn_800D748C | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D748C(void) { /* TODO */ }
#pragma pop

/* fn_800D74A0 | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D74A0(void) { /* TODO */ }
#pragma pop

/* fn_800D74B4 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D74B4(void) { /* TODO */ }
#pragma pop

/* fn_800D74D0 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D74D0(void) { /* TODO */ }
#pragma pop

/* fn_800D74EC | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D74EC(void) { /* TODO */ }
#pragma pop

/* fn_800D7508 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7508(void) { /* TODO */ }
#pragma pop

/* fn_800D7524 | Size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7524(void) { /* TODO */ }
#pragma pop

/* fn_800D7540 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7540(void) { /* TODO */ }
#pragma pop

/* fn_800D7564 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7564(void) { /* TODO */ }
#pragma pop

/* fn_800D7588 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7588(void) { /* TODO */ }
#pragma pop

/* fn_800D75AC | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D75AC(void) { /* TODO */ }
#pragma pop

/* fn_800D75D0 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D75D0(void) { /* TODO */ }
#pragma pop

/* fn_800D75F4 | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D75F4(void) { /* TODO */ }
#pragma pop

/* fn_800D7650 | Size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7650(void) { /* TODO */ }
#pragma pop

/* fn_800D76A8 | Size: 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D76A8(void) { /* TODO */ }
#pragma pop

/* fn_800D7820 | Size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7820(void) { /* TODO */ }
#pragma pop

/* fn_800D7868 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7868(void) { /* TODO */ }
#pragma pop

/* fn_800D7894 -- GSgfx_InitViewport | Size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7894(void) { /* TODO */ }
#pragma pop

/* fn_800D7940 | Size: 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7940(void) { /* TODO */ }
#pragma pop

/* fn_800D7A70 | Size: 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7A70(void) { /* TODO */ }
#pragma pop

/* fn_800D7B80 -- GSgfx_InitProjection | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7B80(void) { /* TODO */ }
#pragma pop

/* fn_800D7BF8 | Size: 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7BF8(void) { /* TODO */ }
#pragma pop

/* fn_800D7C74 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7C74(void) { /* TODO */ }
#pragma pop

/* fn_800D7D10 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7D10(void) { /* TODO */ }
#pragma pop

/* fn_800D7D90 | Size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7D90(void) { /* TODO */ }
#pragma pop

/* fn_800D7E5C | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7E5C(void) { /* TODO */ }
#pragma pop

/* fn_800D7F14 | Size: 0xD0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7F14(void) { /* TODO */ }
#pragma pop

/* fn_800D7FE4 | Size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7FE4(void) { /* TODO */ }
#pragma pop

/* fn_800D8088 | Size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D8088(void) { /* TODO */ }
#pragma pop

/* fn_800D8154 | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D8154(void) { /* TODO */ }
#pragma pop

/* fn_800D81EC | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D81EC(void) { /* TODO */ }
#pragma pop

/* fn_800D8284 | Size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D8284(void) { /* TODO */ }
#pragma pop

/* fn_800D834C | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D834C(void) { /* TODO */ }
#pragma pop

/* fn_800D83E4 -- GSgfx_InitMatrixStack | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D83E4(void) { /* TODO */ }
#pragma pop

/* fn_800D848C | Size: 0x148 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D848C(void) { /* TODO */ }
#pragma pop

/* fn_800D85D4 | Size: 0x1D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D85D4(void) { /* TODO */ }
#pragma pop

/* fn_800D87AC -- GSgfx_SetInternalMode | Size: 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D87AC(void) { /* TODO */ }
#pragma pop

/* fn_800D888C | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D888C(void) { /* TODO */ }
#pragma pop

/* fn_800D88DC | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D88DC(void) { /* TODO */ }
#pragma pop

/* fn_800D923C | Size: 0x400 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D923C(void) { /* TODO */ }
#pragma pop

/* fn_800D963C | Size: 0x4B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D963C(void) { /* TODO */ }
#pragma pop

/* fn_800D9AF0 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9AF0(void) { /* TODO */ }
#pragma pop

/* fn_800D9B24 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9B24(void) { /* TODO */ }
#pragma pop

/* fn_800D9B58 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9B58(void) { /* TODO */ }
#pragma pop

/* fn_800D9BD0 | Size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9BD0(void) { /* TODO */ }
#pragma pop

/* fn_800D9C24 -- GSgfx_SetViewportRect | Size: 0x144 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9C24(void) { /* TODO */ }
#pragma pop

/* fn_800D9D68 -- GSgfx_SetScissor | Size: 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9D68(void) { /* TODO */ }
#pragma pop

/* fn_800D9E4C | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9E4C(void) { /* TODO */ }
#pragma pop

/* fn_800D9ED8 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9ED8(void) { /* TODO */ }
#pragma pop

/* fn_800D9F40 -- GSgfx_ConfigureFog | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9F40(void) { /* TODO */ }
#pragma pop

/* fn_800D9FB4 | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9FB4(void) { /* TODO */ }
#pragma pop

/* fn_800DA028 -- GSgfx_ConfigureTEV | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA028(void) { /* TODO */ }
#pragma pop

/* fn_800DA08C | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA08C(void) { /* TODO */ }
#pragma pop

/* fn_800DA100 -- GSgfx_ConfigureAlpha | Size: 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA100(void) { /* TODO */ }
#pragma pop

/* fn_800DA1E8 -- GSgfx_ConfigureZ | Size: 0xD4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA1E8(void) { /* TODO */ }
#pragma pop

/* fn_800DA2BC -- GSgfx_ConfigureBlend | Size: 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA2BC(void) { /* TODO */ }
#pragma pop

/* fn_800DA3B0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA3B0(void) { /* TODO */ }
#pragma pop

/* fn_800DA428 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA428(void) { /* TODO */ }
#pragma pop

/* fn_800DA4C4 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA4C4(void) { /* TODO */ }
#pragma pop

/* fn_800DA578 | Size: 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA578(void) { /* TODO */ }
#pragma pop

/* fn_800DA6F0 | Size: 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA6F0(void) { /* TODO */ }
#pragma pop

/* fn_800DA880 | Size: 0x440 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA880(void) { /* TODO */ }
#pragma pop

/* fn_800DACC0 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DACC0(void) { /* TODO */ }
#pragma pop

/* fn_800DAD10 | Size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DAD10(void) { /* TODO */ }
#pragma pop

/* fn_800DADB4 | Size: 0x1AC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DADB4(void) { /* TODO */ }
#pragma pop

/* fn_800DAF60 | Size: 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DAF60(void) { /* TODO */ }
#pragma pop

/* fn_800DB098 | Size: 0x6C0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB098(void) { /* TODO */ }
#pragma pop

/* fn_800DB758 | Size: 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB758(void) { /* TODO */ }
#pragma pop

/* fn_800DB890 -- GSgfx_InitLighting | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB890(void) { /* TODO */ }
#pragma pop

/* fn_800DB900 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB900(void) { /* TODO */ }
#pragma pop

/* fn_800DB988 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB988(void) { /* TODO */ }
#pragma pop

/* fn_800DB9F0 | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB9F0(void) { /* TODO */ }
#pragma pop

/* fn_800DBA54 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBA54(void) { /* TODO */ }
#pragma pop

/* fn_800DBAA4 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBAA4(void) { /* TODO */ }
#pragma pop

/* fn_800DBB0C | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBB0C(void) { /* TODO */ }
#pragma pop

/* fn_800DBB84 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBB84(void) { /* TODO */ }
#pragma pop

/* fn_800DBBFC | Size: 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBBFC(void) { /* TODO */ }
#pragma pop

/* fn_800DBCE4 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBCE4(void) { /* TODO */ }
#pragma pop

/* fn_800DBD70 | Size: 0xEC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBD70(void) { /* TODO */ }
#pragma pop

/* fn_800DBE5C | Size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBE5C(void) { /* TODO */ }
#pragma pop

/* fn_800DBEB4 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBEB4(void) { /* TODO */ }
#pragma pop

/* fn_800DBF1C | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBF1C(void) { /* TODO */ }
#pragma pop

/* fn_800DBF78 | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBF78(void) { /* TODO */ }
#pragma pop

/* fn_800DBFD4 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBFD4(void) { /* TODO */ }
#pragma pop

/* fn_800DC04C | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC04C(void) { /* TODO */ }
#pragma pop

/* fn_800DC0D4 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC0D4(void) { /* TODO */ }
#pragma pop

/* fn_800DC14C | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC14C(void) { /* TODO */ }
#pragma pop

/* fn_800DC1D4 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC1D4(void) { /* TODO */ }
#pragma pop

/* fn_800DC224 | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC224(void) { /* TODO */ }
#pragma pop

/* fn_800DC298 | Size: 0xF8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC298(void) { /* TODO */ }
#pragma pop

/* fn_800DC390 | Size: 0x1B0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC390(void) { /* TODO */ }
#pragma pop

/* fn_800DC540 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC540(void) { /* TODO */ }
#pragma pop

/* fn_800DC560 | Size: 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC560(void) { /* TODO */ }
#pragma pop

/* fn_800DC6D8 | Size: 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC6D8(void) { /* TODO */ }
#pragma pop

/* fn_800DC874 | Size: 0x4 */
void fn_800DC874(void) {
    /* 4 bytes -- blr (empty function) */
}

/* fn_800DC878 | Size: 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC878(void) { /* TODO */ }
#pragma pop

/* fn_800DCA10 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCA10(void) { /* TODO */ }
#pragma pop

/* fn_800DCAA4 | Size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCAA4(void) { /* TODO */ }
#pragma pop

/* fn_800DCAB0 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCAB0(void) { /* TODO */ }
#pragma pop

/* fn_800DCADC | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCADC(void) { /* TODO */ }
#pragma pop

/* fn_800DCAF0 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCAF0(void) { /* TODO */ }
#pragma pop

/* fn_800DCB78 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCB78(void) { /* TODO */ }
#pragma pop

/* fn_800DCC3C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCC3C(void) { /* TODO */ }
#pragma pop

/* fn_800DCC60 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCC60(void) { /* TODO */ }
#pragma pop

/* fn_800DCC84 | Size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCC84(void) { /* TODO */ }
#pragma pop

/* fn_800DCCF0 | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCCF0(void) { /* TODO */ }
#pragma pop

/* fn_800DCD98 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCD98(void) { /* TODO */ }
#pragma pop

/* fn_800DCE4C | Size: 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCE4C(void) { /* TODO */ }
#pragma pop

/* fn_800DCFBC | Size: 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCFBC(void) { /* TODO */ }
#pragma pop

/* fn_800DD0B8 | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD0B8(void) { /* TODO */ }
#pragma pop

/* fn_800DD128 | Size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD128(void) { /* TODO */ }
#pragma pop

/* fn_800DD174 | Size: 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD174(void) { /* TODO */ }
#pragma pop

/* fn_800DD270 | Size: 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD270(void) { /* TODO */ }
#pragma pop

/* fn_800DD38C | Size: 0x5E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD38C(void) { /* TODO */ }
#pragma pop

/* fn_800DD970 -- OSReport/GSlog | Size: 0x5E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD970_impl(void) { /* TODO */ }
#pragma pop

/* fn_800DDF54 | Size: 0x148 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DDF54(void) { /* TODO */ }
#pragma pop

/* fn_800DE09C | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DE09C(void) { /* TODO */ }
#pragma pop

/* fn_800DE128 | Size: 0x558 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DE128(void) { /* TODO */ }
#pragma pop

/* fn_800DEFC8 | Size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DEFC8(void) { /* TODO */ }
#pragma pop

/* fn_800DF028 | Size: 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF028(void) { /* TODO */ }
#pragma pop

/* fn_800DF11C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF11C(void) { /* TODO */ }
#pragma pop

/* fn_800DF140 | Size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF140(void) { /* TODO */ }
#pragma pop

/* fn_800DF188 | Size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF188(void) { /* TODO */ }
#pragma pop

/* fn_800DF1B8 | Size: 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF1B8(void) { /* TODO */ }
#pragma pop

/* fn_800DF1D0 | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF1D0(void) { /* TODO */ }
#pragma pop

/* fn_800DF1E4 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF1E4(void) { /* TODO */ }
#pragma pop

/* fn_800DF208 | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF208(void) { /* TODO */ }
#pragma pop

/* fn_800DF21C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF21C(void) { /* TODO */ }
#pragma pop

/* fn_800DF248 | Size: 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF248(void) { /* TODO */ }
#pragma pop

/* fn_800DF384 | Size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF384(void) { /* TODO */ }
#pragma pop

/* fn_800DF3F0 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF3F0(void) { /* TODO */ }
#pragma pop

/* fn_800DF470 | Size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF470(void) { /* TODO */ }
#pragma pop

/* fn_800DF498 | Size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF498(void) { /* TODO */ }
#pragma pop

/* fn_800DF504 | Size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF504(void) { /* TODO */ }
#pragma pop

/* fn_800DF550 | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF550(void) { /* TODO */ }
#pragma pop

/* fn_800DF608 | Size: 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF608(void) { /* TODO */ }
#pragma pop

/* fn_800DF7A4 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF7A4(void) { /* TODO */ }
#pragma pop

/* fn_800DF854 -- GSmaterialInit | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF854(void) { /* TODO */ }
#pragma pop

/* fn_800DF8CC -- GSmaterialClassInit | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF8CC(void) { /* TODO */ }
#pragma pop

/* fn_800DF930 | Size: 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF930(void) { /* TODO */ }
#pragma pop

/* fn_800DFABC | Size: 0x3DC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFABC(void) { /* TODO */ }
#pragma pop

/* fn_800DFE98 | Size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFE98(void) { /* TODO */ }
#pragma pop

/* fn_800DFEEC | Size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFEEC(void) { /* TODO */ }
#pragma pop

/* fn_800DFF98 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFF98(void) { /* TODO */ }
#pragma pop

/* fn_800DFFCC | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFFCC(void) { /* TODO */ }
#pragma pop

/* fn_800E0000 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0000(void) { /* TODO */ }
#pragma pop

/* fn_800E0020 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0020(void) { /* TODO */ }
#pragma pop

/* fn_800E0040 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0040(void) { /* TODO */ }
#pragma pop

/* fn_800E0060 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0060(void) { /* TODO */ }
#pragma pop

/* fn_800E008C | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E008C(void) { /* TODO */ }
#pragma pop

/* fn_800E00AC | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E00AC(void) { /* TODO */ }
#pragma pop

/* fn_800E00E0 | Size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E00E0(void) { /* TODO */ }
#pragma pop

/* fn_800E0108 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0108(void) { /* TODO */ }
#pragma pop

/* fn_800E013C | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E013C(void) { /* TODO */ }
#pragma pop

/* fn_800E0168 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0168(void) { /* TODO */ }
#pragma pop

/* fn_800E019C | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E019C(void) { /* TODO */ }
#pragma pop

/* fn_800E01D0 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E01D0(void) { /* TODO */ }
#pragma pop

/* fn_800E01F4 | Size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E01F4(void) { /* TODO */ }
#pragma pop

/* fn_800E0204 | Size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0204(void) { /* TODO */ }
#pragma pop

/* fn_800E0218 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0218(void) { /* TODO */ }
#pragma pop

/* fn_800E0238 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0238(void) { /* TODO */ }
#pragma pop

/* fn_800E0264 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0264(void) { /* TODO */ }
#pragma pop

/* fn_800E0290 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0290(void) { /* TODO */ }
#pragma pop

/* fn_800E02C4 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E02C4(void) { /* TODO */ }
#pragma pop

/* fn_800E02E8 | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E02E8(void) { /* TODO */ }
#pragma pop

/* fn_800E032C | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E032C(void) { /* TODO */ }
#pragma pop

/* fn_800E0370 | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0370(void) { /* TODO */ }
#pragma pop

/* fn_800E03B4 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E03B4(void) { /* TODO */ }
#pragma pop

/* fn_800E03E8 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E03E8(void) { /* TODO */ }
#pragma pop

/* fn_800E040C | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E040C(void) { /* TODO */ }
#pragma pop

/* fn_800E042C | Size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E042C(void) { /* TODO */ }
#pragma pop

/* fn_800E048C | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E048C(void) { /* TODO */ }
#pragma pop

/* fn_800E04F4 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E04F4(void) { /* TODO */ }
#pragma pop

/* fn_800E0518 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0518(void) { /* TODO */ }
#pragma pop

/* fn_800E053C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E053C(void) { /* TODO */ }
#pragma pop

/* fn_800E0560 | Size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0560(void) { /* TODO */ }
#pragma pop

/* fn_800E05C0 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E05C0(void) { /* TODO */ }
#pragma pop

/* fn_800E0628 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0628(void) { /* TODO */ }
#pragma pop

/* fn_800E064C | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E064C(void) { /* TODO */ }
#pragma pop

/* fn_800E0678 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0678(void) { /* TODO */ }
#pragma pop

/* fn_800E0698 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0698(void) { /* TODO */ }
#pragma pop

/* fn_800E06B8 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E06B8(void) { /* TODO */ }
#pragma pop

/* fn_800E06EC | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E06EC(void) { /* TODO */ }
#pragma pop

/* fn_800E0718 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0718(void) { /* TODO */ }
#pragma pop

/* fn_800E0738 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0738(void) { /* TODO */ }
#pragma pop

/* fn_800E076C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E076C(void) { /* TODO */ }
#pragma pop

/* fn_800E0790 | Size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0790(void) { /* TODO */ }
#pragma pop

/* fn_800E07E4 | Size: 0x128 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E07E4(void) { /* TODO */ }
#pragma pop

/* fn_800E090C | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E090C(void) { /* TODO */ }
#pragma pop

/* fn_800E09B4 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E09B4(void) { /* TODO */ }
#pragma pop

/* fn_800E09E8 | Size: 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E09E8(void) { /* TODO */ }
#pragma pop

/* fn_800E0BA0 | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0BA0(void) { /* TODO */ }
#pragma pop

/* fn_800E0BE4 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0BE4(void) { /* TODO */ }
#pragma pop

/* fn_800E0C04 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0C04(void) { /* TODO */ }
#pragma pop

/* fn_800E0C54 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0C54(void) { /* TODO */ }
#pragma pop

/* fn_800E0C78 | Size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0C78(void) { /* TODO */ }
#pragma pop

/* fn_800E0CA0 | Size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0CA0(void) { /* TODO */ }
#pragma pop

/* fn_800E0D24 | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0D24(void) { /* TODO */ }
#pragma pop

/* fn_800E0DDC | Size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0DDC(void) { /* TODO */ }
#pragma pop

/* fn_800E0E14 | Size: 0x730 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0E14(void) { /* TODO */ }
#pragma pop

/* fn_800E202C | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E202C(void) { /* TODO */ }
#pragma pop
