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
extern void  fn_800DD970(const char*, ...);             /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);

/* External functions referenced from asm wrappers */
extern void DCFlushRange(void* addr, u32 size);
extern u64 OSGetTime(void);
extern void fn_800D3EC4(void);
extern void fn_800D4F98(u32, ...);
extern void fn_800D67BC(void);
extern void fn_800D892C(u32);

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
extern void fn_800D7650(u8*);
extern void fn_800D7868(u8*, u32, u32, u32, u32, u8, u32, u8);
extern void fn_800D7940(void);
extern void fn_800D7A70(u32);
extern void fn_800DA6F0(void);
extern void fn_800DA880(void);
extern void fn_800DB098(void);
extern void fn_800DB758(void);
extern void fn_800DD128(u8*);
extern void fn_800DE09C(void);
extern void fn_800DE128(void);
extern void fn_800DF930(void);
extern void fn_800DFABC(void);
extern s32 fn_800DFE98(u8*);
extern void fn_800E0290(void*, void*, void*);
extern void fn_800E02C4(void*);
extern void fn_800E02E8(void*);
extern void fn_800E032C(void*);
extern void fn_800E0370(void*);
extern void fn_800E03E8(void*, f32, f32, f32);
extern void fn_800E0628(void*, void*);
extern void fn_800E064C(void*);
extern void fn_800E0678(u8*);
extern void fn_800E0698(void*, f32, f32, f32, f32, f32, f32);
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
extern void fn_800B944C(u32, u32);
extern f32 lbl_8047CA30;
extern f32 lbl_8047CA34;
extern f32 lbl_8047CA38;
#if 0
asm void fn_800D55D0(void) {
#include "src/game/gs_render_fn_800D55D0.inc"
}
#else
void fn_800D55D0(f32 val) {
    if (val < lbl_8047CA30 || val > lbl_8047CA34) return;
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x25, 0xb); }
    else {
        u32 tmp;
        tmp = (u32)(s32)(lbl_8047CA38 * val);
        fn_800B944C(tmp, 0);
    }
}
#endif
extern void fn_800B9404(u32, u32);
extern f32 lbl_8047CA30;
extern f32 lbl_8047CA34;
extern f32 lbl_8047CA38;
#if 0
asm void fn_800D5648(void) {
#include "src/game/gs_render_fn_800D5648.inc"
}
#else
void fn_800D5648(f32 val) {
    if (val < lbl_8047CA30 || val > lbl_8047CA34) return;
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x24, 0xb); }
    else {
        u32 tmp;
        tmp = (u32)(s32)(lbl_8047CA38 * val);
        fn_800B9404(tmp, 0);
    }
}
#endif
extern void fn_800D7230(void);
#if 0
asm void fn_800D56C0(void) {
#include "src/game/gs_render_fn_800D56C0.inc"
}
#else
void fn_800D56C0(u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x23, 1, (u8)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4a0) = (u32)fn_800D7230;
        *(u8*)(lbl_8047AA80 + 0x4a4) = val;
    }
}
#endif
#if 0
asm void fn_800D5724(void) {
#include "src/game/gs_render_fn_800D5724.inc"
}
#else
void fn_800D5724(u32 idx, u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x22, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x500 + idx * 4) = (u32)fn_800D724C;
        *(u8*)(lbl_8047AA80 + 0x520 + idx * 16) = val;
    }
}
#endif
#if 0
asm void fn_800D579C(void) {
#include "src/game/gs_render_fn_800D579C.inc"
}
#else
void fn_800D579C(u32 idx, u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x21, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x500 + idx * 4) = (u32)fn_800D7268;
        *(u16*)(lbl_8047AA80 + 0x522 + idx * 16) = val;
    }
}
#endif
#if 0
asm void fn_800D5814(void) {
#include "src/game/gs_render_fn_800D5814.inc"
}
#else
void fn_800D5814(u32 idx, u8 a, u8 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0x1f, 3, idx, (u32)a, (u32)b); }
    else {
        *(u32*)(state + 0x500 + idx * 4) = (u32)fn_800D72A4;
        *(u8*)(state + 0x520 + idx * 16) = a;
        *(u8*)(state + 0x521 + idx * 16) = b;
    }
}
#endif
#if 0
asm void fn_800D58A0(void) {
#include "src/game/gs_render_fn_800D58A0.inc"
}
#else
void fn_800D58A0(u32 idx, s16 a, s16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0x1e, 3, idx, (s32)a, (s32)b); }
    else {
        *(u32*)(state + 0x500 + idx * 4) = (u32)fn_800D72C4;
        *(u16*)(state + 0x522 + idx * 16) = a;
        *(u16*)(state + 0x524 + idx * 16) = b;
    }
}
#endif
#if 0
asm void fn_800D592C(void) {
#include "src/game/gs_render_fn_800D592C.inc"
}
#else
void fn_800D592C(u32 idx, u16 a, u16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0x1d, 3, idx, (u32)a, (u32)b); }
    else {
        *(u32*)(state + 0x500 + idx * 4) = (u32)fn_800D72E4;
        *(u16*)(state + 0x522 + idx * 16) = a;
        *(u16*)(state + 0x524 + idx * 16) = b;
    }
}
#endif
#if 0
asm void fn_800D59B8(void) {
#include "src/game/gs_render_fn_800D59B8.inc"
}
#else
void fn_800D59B8(u32 idx, f32 x, f32 y) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0x1c, 0xf); }
    else {
        *(u32*)(state + 0x500 + idx * 4) = (u32)fn_800D7304;
        *(f32*)(state + 0x528 + idx * 16) = x;
        *(f32*)(state + 0x52c + idx * 16) = y;
    }
}
#endif
#if 0
asm void fn_800D5A38(void) {
#include "src/game/gs_render_fn_800D5A38.inc"
}
#else
void fn_800D5A38(u32 idx, u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x1b, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D7328;
        *(u8*)(lbl_8047AA80 + 0x4e8 + idx * 12) = val;
    }
}
#endif
#if 0
asm void fn_800D5AB0(void) {
#include "src/game/gs_render_fn_800D5AB0.inc"
}
#else
void fn_800D5AB0(u32 idx, u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x1a, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D7344;
        *(u16*)(lbl_8047AA80 + 0x4ec + idx * 12) = val;
    }
}
#endif
#if 0
asm void fn_800D5B28(void) {
#include "src/game/gs_render_fn_800D5B28.inc"
}
#else
void fn_800D5B28(u32 idx, u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x19, 2, idx, (u32)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D7360;
        *(u16*)(lbl_8047AA80 + 0x4ec + idx * 12) = val;
    }
}
#endif
#if 0
asm void fn_800D5BA0(void) {
#include "src/game/gs_render_fn_800D5BA0.inc"
}
#else
void fn_800D5BA0(u32 idx, u32 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x18, 2, idx, val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4e0 + idx * 4) = (u32)fn_800D737C;
        *(u32*)(lbl_8047AA80 + 0x4f0 + idx * 12) = val;
    }
}
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
void fn_800D5CB8(void) { /* TODO - similar to fn_800D5C18 */ }
#endif
#if 0
asm void fn_800D5D6C(void) {
#include "src/game/gs_render_fn_800D5D6C.inc"
}
#else
void fn_800D5D6C(u8 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x15, 1, (u8)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4c4) = (u32)fn_800D73F8;
        *(u8*)(lbl_8047AA80 + 0x4c8) = val;
    }
}
#endif
#if 0
asm void fn_800D5DD0(void) {
#include "src/game/gs_render_fn_800D5DD0.inc"
}
#else
void fn_800D5DD0(u16 val) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x14, 1, (u16)val);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4c4) = (u32)fn_800D740C;
        *(u16*)(lbl_8047AA80 + 0x4cc) = val;
    }
}
#endif
#if 0
asm void fn_800D5E34(void) {
#include "src/game/gs_render_fn_800D5E34.inc"
}
#else
void fn_800D5E34(s8 a, s8 b, s8 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0x13, 3, (s32)a, (s32)b, (s32)c); }
    else {
        *(u32*)(state + 0x4c4) = (u32)fn_800D7420;
        *(u8*)(state + 0x4c8) = a;
        *(u8*)(state + 0x4c9) = b;
        *(u8*)(state + 0x4ca) = c;
    }
}
#endif
#if 0
asm void fn_800D5EB4(void) {
#include "src/game/gs_render_fn_800D5EB4.inc"
}
#else
void fn_800D5EB4(s16 a, s16 b, s16 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0x12, 3, (s32)a, (s32)b, (s32)c); }
    else {
        *(u32*)(state + 0x4c4) = (u32)fn_800D7444;
        *(u16*)(state + 0x4cc) = a;
        *(u16*)(state + 0x4ce) = b;
        *(u16*)(state + 0x4d0) = c;
    }
}
#endif
#if 0
asm void fn_800D5F34(void) {
#include "src/game/gs_render_fn_800D5F34.inc"
}
#else
void fn_800D5F34(f32 x, f32 y, f32 z) {
    if (!*(u8*)(lbl_8047AA80 + 0x47e) && *(u32*)lbl_8047AA80 == 1) {
        fn_800D4F98(0x11, 0xd);
    } else {
        *(u32*)(lbl_8047AA80 + 0x4c4) = (u32)fn_800D7468;
        *(f32*)(lbl_8047AA80 + 0x4d4) = x;
        *(f32*)(lbl_8047AA80 + 0x4d8) = y;
        *(f32*)(lbl_8047AA80 + 0x4dc) = z;
    }
}
#endif
#if 0
asm void fn_800D5FA4(void) {
#include "src/game/gs_render_fn_800D5FA4.inc"
}
#else
void fn_800D5FA4(u8 val) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0x10, 1, (u32)val); }
    else {
        fn_800D6B00();
        *(u32*)(state + 0x4a8) = (u32)fn_800D748C;
        *(u8*)(state + 0x4ac) = val;
        *(u8*)(state + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D6028(void) {
#include "src/game/gs_render_fn_800D6028.inc"
}
#else
void fn_800D6028(u16 val) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(u32*)state == 1) { fn_800D4F98(0xf, 1, (u32)val); }
    else {
        fn_800D6B00();
        *(u32*)(state + 0x4a8) = (u32)fn_800D74A0;
        *(u16*)(state + 0x4b0) = val;
        *(u8*)(state + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D60AC(void) {
#include "src/game/gs_render_fn_800D60AC.inc"
}
#else
void fn_800D60AC(s8 a, s8 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xe, 2, (s32)a, (s32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D74B4;
        *(u8*)(lbl_8047AA80 + 0x4ac) = a;
        *(u8*)(lbl_8047AA80 + 0x4ad) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D6148(void) {
#include "src/game/gs_render_fn_800D6148.inc"
}
#else
void fn_800D6148(u8 a, u8 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xd, 2, (u32)a, (u32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D74D0;
        *(u8*)(lbl_8047AA80 + 0x4ac) = a;
        *(u8*)(lbl_8047AA80 + 0x4ad) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D61E4(void) {
#include "src/game/gs_render_fn_800D61E4.inc"
}
#else
void fn_800D61E4(s16 a, s16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xc, 2, (s32)a, (s32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D74EC;
        *(u16*)(lbl_8047AA80 + 0x4b0) = a;
        *(u16*)(lbl_8047AA80 + 0x4b2) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D6280(void) {
#include "src/game/gs_render_fn_800D6280.inc"
}
#else
void fn_800D6280(u16 a, u16 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xb, 2, (u32)a, (u32)b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7508;
        *(u16*)(lbl_8047AA80 + 0x4b0) = a;
        *(u16*)(lbl_8047AA80 + 0x4b2) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D631C(void) {
#include "src/game/gs_render_fn_800D631C.inc"
}
#else
void fn_800D631C(f32 a, f32 b) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(0xa, 0xc, a, b);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7524;
        *(f32*)(lbl_8047AA80 + 0x4b8) = a;
        *(f32*)(lbl_8047AA80 + 0x4bc) = b;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D63B0(void) {
#include "src/game/gs_render_fn_800D63B0.inc"
}
#else
void fn_800D63B0(s8 a, s8 b, s8 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(9, 3, (s32)a, (s32)b, (s32)c);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D7540;
        *(u8*)(lbl_8047AA80 + 0x4ac) = a;
        *(u8*)(lbl_8047AA80 + 0x4ad) = b;
        *(u8*)(lbl_8047AA80 + 0x4ae) = c;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
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
#if 0
asm void fn_800D6680(void) {
#include "src/game/gs_render_fn_800D6680.inc"
}
#else
void fn_800D6680(f32 a, f32 b, f32 c) {
    u32 state = lbl_8047AA80;
    if (!*(u8*)(state + 0x47e) && *(s32*)state == 1) {
        fn_800D4F98(5, 0xd, a, b, c);
    } else {
        fn_800D6B00();
        *(u32*)(lbl_8047AA80 + 0x4a8) = (u32)fn_800D75D0;
        *(f32*)(lbl_8047AA80 + 0x4b8) = a;
        *(f32*)(lbl_8047AA80 + 0x4bc) = b;
        *(f32*)(lbl_8047AA80 + 0x4c0) = c;
        *(u8*)(lbl_8047AA80 + 0x49f) = 1;
    }
}
#endif
#if 0
asm void fn_800D6728(void) {
#include "src/game/gs_render_fn_800D6728.inc"
}
#else
void fn_800D6728(void) {
    u32 state = lbl_8047AA80;
    if (*(u8*)(state + 0x47e) == 1) {
        fn_800D6B00();
        return;
    }
    if (*(s32*)state == 1) {
        fn_800D4F98(3, 0);
        return;
    }
    fn_800D6B00();
    {
        u32 s = lbl_8047AA80;
        if (*(u8*)(s + 0x1b) == *(u8*)(s + 0x1a) &&
            (*(u32*)(s + 0x4) & *(u32*)(s + 0x8)) &&
            *(u32*)(s + 0x24) == *(u32*)(s + 0x20)) {
            *(u32*)(s + 0x24) = 0;
        }
    }
}
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
#if 0
asm void fn_800D6A00(void) {
#include "src/game/gs_render_fn_800D6A00.inc"
}
#else
void fn_800D6A00(u32 val) {
    if (*(u8*)(lbl_8047AA80 + 0x47e) == 1) { *(u32*)(lbl_8047AA80 + 0x488) = val; }
    else if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x1, 1, val); }
    else { *(u32*)(lbl_8047AA80 + 0x14) = val; }
}
#endif
#if 0
asm void fn_800D6A5C(void) {
#include "src/game/gs_render_fn_800D6A5C.inc"
}
#else
void fn_800D6A5C(u32 a, u32 b) { *(u32*)(lbl_804001F0 + 0xc) += a; *(u32*)(lbl_804001F0 + 0x4) += b; }
#endif
#if 0
asm void fn_800D6A80(void) {
#include "src/game/gs_render_fn_800D6A80.inc"
}
#else
void fn_800D6A80(u16 vertCount, s32 type, u32* totalVerts, u32* totalPrims) {
    *totalVerts += vertCount;
    switch (type) {
        case 3: *totalPrims += vertCount / 3; break;
        case 4:
        case 5: *totalPrims += vertCount - 2; break;
        case 6:
        case 7: *totalPrims += (vertCount >> 1); break;
    }
}
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
#if 0
asm void fn_800D75F4(void) {
#include "src/game/gs_render_fn_800D75F4.inc"
}
#else
void fn_800D75F4(u8* obj) {
    if (*(u32*)(lbl_8047AA80 + 0x24) == (u32)obj) *(u32*)(lbl_8047AA80 + 0x24) = 0;
    obj[0x008] = 0; obj[0x024] = 0; obj[0x040] = 0; obj[0x05c] = 0;
    obj[0x078] = 0; obj[0x094] = 0; obj[0x0b0] = 0; obj[0x0cc] = 0;
    obj[0x0e8] = 0; obj[0x104] = 0; obj[0x120] = 0; obj[0x13c] = 0;
    obj[0x158] = 0; obj[0x174] = 0; obj[0x000] = 0;
}
#endif
#if 0
asm void fn_800D7650(void) {
#include "src/game/gs_render_fn_800D7650.inc"
}
#else
void fn_800D7650(u8* obj) {
    if (*(u32*)(lbl_8047AA80 + 0x24) == (u32)obj) *(u32*)(lbl_8047AA80 + 0x24) = 0;
    obj[0x008] = 0; obj[0x024] = 0; obj[0x040] = 0; obj[0x05c] = 0;
    obj[0x078] = 0; obj[0x094] = 0; obj[0x0b0] = 0; obj[0x0cc] = 0;
    obj[0x0e8] = 0; obj[0x104] = 0; obj[0x120] = 0; obj[0x13c] = 0;
    obj[0x158] = 0; obj[0x174] = 0;
}
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
#if 0
asm void fn_800D7820(void) {
#include "src/game/gs_render_fn_800D7820.inc"
}
#else
void fn_800D7820(u32 val) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x46, 1, val); }
    else { *(u32*)(lbl_8047AA80 + 0x24) = val; }
}
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
asm void fn_800D7A70(u32) {
#include "src/game/gs_render_fn_800D7A70.inc"
}
#else
void fn_800D7A70(u32 a) { /* TODO */ }
#endif
extern u32 lbl_8047AAB0;
extern u16 lbl_8047AAA8;
extern u32 lbl_8047AAAC;
extern u32 lbl_8047AAB4;
#if 1
asm void fn_800D7B80(void) {
#include "src/game/gs_render_fn_800D7B80.inc"
}
#else
void fn_800D7B80(u32 count) {
    u16 handle;
    u32 off;
    u32 i;
    lbl_8047AAB0 = count;
    handle = fn_800E3534(count * 0x190);
    lbl_8047AAA8 = handle;
    if (handle != 0) {
        lbl_8047AAAC = (u32)fn_800E27B0(handle);
        for (off = 0, i = 0; i < lbl_8047AAB0; i++, off += 0x190) {
            *(u8*)(lbl_8047AAAC + off) = 0;
        }
        lbl_8047AAB4 = 0;
    }
}
#endif
extern u32 fn_800D2584(void);
extern u32 fn_800D1D00(void);
extern u32 fn_800D1B3C(void);
extern u32 fn_800D1A70(void);
#if 0
asm void fn_800D7BF8(void) {
#include "src/game/gs_render_fn_800D7BF8.inc"
}
#else
u32 fn_800D7BF8(s32 mode) {
    if (!fn_800D2584()) return 0;
    switch (mode) {
        case 0: return fn_800D1D00();
        case 1: return fn_800D1B3C();
        case 2: return fn_800D1A70();
        default: return 0;
    }
}
#endif
extern void fn_800BD4B4(u32, u32);
extern void fn_800BD504(u32, u32);
extern void fn_800BD554(u32);
extern u8 lbl_8047AAC8;
extern u8 lbl_80314610[];
extern u32 lbl_8047AAC0;
extern u8 lbl_80400948[];
#if 0
asm void fn_800D7C74(void) {
#include "src/game/gs_render_fn_800D7C74.inc"
}
#else
void fn_800D7C74(void) {
    u32 state = lbl_8047AA80;
    u32 mtx;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x45, 0);
    } else if (lbl_8047AAC8) {
        mtx = *(u32*)(lbl_80314610 + 0x24);
        fn_800BD4B4(lbl_8047AAC0, mtx);
        fn_800BD504(lbl_8047AAC0, mtx);
        fn_800BD554(mtx);
        fn_800E0628((void*)(lbl_80400948 + 0x1b0), (void*)lbl_8047AAC0);
        lbl_8047AAC8 = 0;
    }
}
#endif
#if 0
asm void fn_800D7D10(void) {
#include "src/game/gs_render_fn_800D7D10.inc"
}
#else
void fn_800D7D10(u8 idx, void* src) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x44, 2, (u32)idx, src); }
    else if (idx <= 9) { fn_800E0628(src, &lbl_80400948[idx * 0x30]); }
    else { fn_800DD970(lbl_80270440); }
}
#endif
extern u8 lbl_8047AAC8;
#if 1
asm void fn_800D7D90(void) {
#include "src/game/gs_render_fn_800D7D90.inc"
}
#else
void fn_800D7D90(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AAC4;
extern u8 lbl_8047AAC8;
#if 1
asm void fn_800D7E5C(void) {
#include "src/game/gs_render_fn_800D7E5C.inc"
}
#else
void fn_800D7E5C(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AABC;
extern u8 lbl_8047AAC8;
#if 1
asm void fn_800D7F14(void) {
#include "src/game/gs_render_fn_800D7F14.inc"
}
#else
void fn_800D7F14(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u8 lbl_8047AAC8;
#if 0
asm void fn_800D7FE4(void) {
#include "src/game/gs_render_fn_800D7FE4.inc"
}
#else
void fn_800D7FE4(void* param) {
    u32 state = lbl_8047AA80;
    u32 mtx;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x40, 0x10, param);
    } else {
        fn_800E0290((void*)lbl_8047AAC0, (void*)lbl_8047AAC0, param);
        mtx = *(u32*)(lbl_80314610 + 0x24);
        fn_800BD4B4(lbl_8047AAC0, mtx);
        fn_800BD504(lbl_8047AAC0, mtx);
        fn_800BD554(mtx);
        fn_800E0628((void*)(lbl_80400948 + 0x1b0), (void*)lbl_8047AAC0);
        lbl_8047AAC8 = 0;
    }
}
#endif
extern u32 lbl_8047AAC0;
extern u32 lbl_8047AABC;
extern u8 lbl_8047AAC8;
#if 1
asm void fn_800D8088(void) {
#include "src/game/gs_render_fn_800D8088.inc"
}
#else
void fn_800D8088(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u8 lbl_8047AAC8;
#if 0
asm void fn_800D8154(void) {
#include "src/game/gs_render_fn_800D8154.inc"
}
#else
void fn_800D8154(f32 a, f32 b, f32 c) {
    u32 state = lbl_8047AA80;
    u32 mtx;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x3e, 0xd, a, b, c);
    } else {
        fn_800E02C4((void*)lbl_8047AAC0);
        mtx = *(u32*)(lbl_80314610 + 0x24);
        fn_800BD4B4(lbl_8047AAC0, mtx);
        fn_800BD504(lbl_8047AAC0, mtx);
        fn_800BD554(mtx);
        fn_800E0628((void*)(lbl_80400948 + 0x1b0), (void*)lbl_8047AAC0);
        lbl_8047AAC8 = 0;
    }
}
#endif
extern u32 lbl_8047AAC0;
extern u8 lbl_8047AAC8;
#if 0
asm void fn_800D81EC(void) {
#include "src/game/gs_render_fn_800D81EC.inc"
}
#else
void fn_800D81EC(f32 a, f32 b, f32 c) {
    u32 state = lbl_8047AA80;
    u32 mtx;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x3d, 0xd, a, b, c);
    } else {
        fn_800E03E8((void*)lbl_8047AAC0, a, b, c);
        mtx = *(u32*)(lbl_80314610 + 0x24);
        fn_800BD4B4(lbl_8047AAC0, mtx);
        fn_800BD504(lbl_8047AAC0, mtx);
        fn_800BD554(mtx);
        fn_800E0628((void*)(lbl_80400948 + 0x1b0), (void*)lbl_8047AAC0);
        lbl_8047AAC8 = 0;
    }
}
#endif
extern u32 lbl_8047AAC0;
extern u8 lbl_8047AAC8;
#if 1
asm void fn_800D8284(void) {
#include "src/game/gs_render_fn_800D8284.inc"
}
#else
void fn_800D8284(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAC0;
extern u8 lbl_8047AAC8;
#if 0
asm void fn_800D834C(void) {
#include "src/game/gs_render_fn_800D834C.inc"
}
#else
void fn_800D834C(void) {
    u32 state = lbl_8047AA80;
    u32 mtx;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x3b, 0);
    } else {
        fn_800E064C((void*)lbl_8047AAC0);
        mtx = *(u32*)(lbl_80314610 + 0x24);
        fn_800BD4B4(lbl_8047AAC0, mtx);
        fn_800BD504(lbl_8047AAC0, mtx);
        fn_800BD554(mtx);
        fn_800E0628((void*)(lbl_80400948 + 0x1b0), (void*)lbl_8047AAC0);
        lbl_8047AAC8 = 0;
    }
}
#endif
extern u16 lbl_8047AAB8;
extern u32 lbl_8047AABC;
extern u32 lbl_8047AAC4;
extern u32 lbl_8047AAC0;
extern u8 lbl_8047AAC8;
#if 0
asm void fn_800D83E4(void) {
#include "src/game/gs_render_fn_800D83E4.inc"
}
#else
void fn_800D83E4(u32 count) {
    u16 handle;
    u32 mtx;
    u32 ptr;
    u32 val;
    handle = fn_800E3534(count * 0x30);
    val = (u16)handle;
    lbl_8047AAB8 = handle;
    if (val != 0) {
        ptr = (u32)fn_800E27B0((u16)val);
        lbl_8047AABC = ptr;
        ptr = ptr + (count - 1) * 0x30;
        lbl_8047AAC4 = ptr;
        lbl_8047AAC0 = ptr;
        fn_800E064C((void*)ptr);
        mtx = *(u32*)(lbl_80314610 + 0x24);
        fn_800BD4B4(lbl_8047AAC0, mtx);
        fn_800BD504(lbl_8047AAC0, mtx);
        fn_800BD554(mtx);
        fn_800E0628((void*)(lbl_80400948 + 0x1b0), (void*)lbl_8047AAC0);
        lbl_8047AAC8 = 0;
    }
}
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
#if 0
asm void fn_800D888C(void) {
#include "src/game/gs_render_fn_800D888C.inc"
}
#else
void fn_800D888C(u32 mask) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x29, 1, mask); }
    else { *(u32*)(lbl_8047AA80 + 0x10) &= ~mask; }
}
#endif
#if 0
asm void fn_800D88DC(void) {
#include "src/game/gs_render_fn_800D88DC.inc"
}
#else
void fn_800D88DC(u32 mask) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x28, 1, mask); }
    else { *(u32*)(lbl_8047AA80 + 0x10) |= mask; }
}
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
extern void fn_800BD2E0(void*, u32);
extern f32 lbl_8047CA50;
extern f32 lbl_8047CA54;
#if 0
asm void fn_800D9B58(void) {
#include "src/game/gs_render_fn_800D9B58.inc"
}
#else
void fn_800D9B58(f32 a, f32 b, f32 c, f32 d) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x3a, 0xe); }
    else {
        u8 tmp[0x48];
        fn_800E0698(tmp, b, d, a, c, lbl_8047CA50, lbl_8047CA54);
        fn_800BD2E0(tmp, 1);
        fn_800D834C();
    }
}
#endif
#if 0
asm void fn_800D9BD0(void) {
#include "src/game/gs_render_fn_800D9BD0.inc"
}
#else
void fn_800D9BD0(void) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x39, 0xe); }
    else { u8 tmp[0x48]; fn_800E0678(tmp); fn_800BD2E0(tmp, 0); }
}
#endif
extern void fn_800D21C8(void);
extern u32 lbl_8047CA60;
extern u32 lbl_8047CA68;
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
extern void fn_8019BD18(u32);
extern u32 lbl_8047AA8C;
#if 0
asm void fn_800D9E4C(void) {
#include "src/game/gs_render_fn_800D9E4C.inc"
}
#else
void fn_800D9E4C(s32 val) {
    u32 state = lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x36, 1, val);
    } else {
        if (val == 1) {
            *(u8*)(state + 0x5d) = 1;
        } else if (val == 0) {
            *(u8*)(state + 0x5d) = 0;
        }
        if (*(u8*)(lbl_8047AA80 + 0x5d) == 1) {
            fn_8019BD18(lbl_8047AA8C);
        } else {
            fn_8019BD18(0);
        }
    }
}
#endif
#if 0
asm void fn_800D9ED8(void) {
#include "src/game/gs_render_fn_800D9ED8.inc"
}
#else
void fn_800D9ED8(s32 val) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x35, 1, val); }
    else if (val == 1) { *(u8*)(lbl_8047AA80 + 0x1a) = 1; }
    else if (val == 0) { *(u8*)(lbl_8047AA80 + 0x1a) = 0; }
}
#endif
extern void fn_800BD870(u32);
#if 0
asm void fn_800D9FB4(void) {
#include "src/game/gs_render_fn_800D9FB4.inc"
}
#else
void fn_800D9FB4(u32 val) {
    u32 state = lbl_8047AA80;
    if (*(u32*)state == 1) { fn_800D4F98(0x33, 1, val); }
    else {
        if (val == 1) { *(u8*)(state + 0x42c) = 0; }
        else if (val == 0) { *(u8*)(state + 0x42c) = 1; }
        fn_800BD870(*(u8*)(lbl_8047AA80 + 0x42c));
    }
}
#endif
extern void fn_800B94F0(u32);
extern u8 lbl_8031453C[];
#if 0
asm void fn_800DA028(void) {
#include "src/game/gs_render_fn_800DA028.inc"
}
#else
void fn_800DA028(u32 idx) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x32, 1, idx); }
    else {
        *(u32*)(lbl_8047AA80 + 0x428) = ((u32*)lbl_8031453C)[idx];
        fn_800B94F0(*(u32*)(lbl_8047AA80 + 0x428));
    }
}
#endif
extern void fn_800BCFDC(u32);
#if 0
asm void fn_800DA08C(void) {
#include "src/game/gs_render_fn_800DA08C.inc"
}
#else
void fn_800DA08C(u32 val) {
    u32 state = lbl_8047AA80;
    if (*(u32*)state == 1) { fn_800D4F98(0x31, 1, val); }
    else {
        if (val == 1) { *(u8*)(state + 0x426) = 1; }
        else if (val == 0) { *(u8*)(state + 0x426) = 0; }
        fn_800BCFDC(*(u8*)(lbl_8047AA80 + 0x426));
    }
}
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
extern void fn_800BD008(u32);
#if 0
asm void fn_800DA3B0(void) {
#include "src/game/gs_render_fn_800DA3B0.inc"
}
#else
void fn_800DA3B0(u32 val, u8 b) {
    u32 state = lbl_8047AA80;
    if (*(u32*)state == 1) { fn_800D4F98(0x2d, 2, val, (u32)b); }
    else {
        if (val == 1) { *(u8*)(state + 0x418) = 1; }
        else if (val == 0) { *(u8*)(state + 0x418) = 0; }
        fn_800BD008(*(u8*)(lbl_8047AA80 + 0x418));
    }
}
#endif
extern void fn_800BCDDC(u32, u32, u32, u32);
extern u8 lbl_803145D0[];
#if 0
asm void fn_800DA428(void) {
#include "src/game/gs_render_fn_800DA428.inc"
}
#else
void fn_800DA428(s32 val) {
    u32 state = lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x2c, 1, val);
    } else if (val == 0) {
        fn_800BCDDC(0, 1, 1, 5);
        *(u32*)(lbl_8047AA80 + 0x8) = 0x10;
    } else {
        fn_800BCDDC(2, 1, 1, *(u32*)(lbl_803145D0 + val * 4));
        *(u32*)(lbl_8047AA80 + 0x8) = 0x2000;
    }
}
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
extern void fn_800E24B0(u16);
extern void fn_800E209C(u16);
#if 0
asm void fn_800DACC0(void) {
#include "src/game/gs_render_fn_800DACC0.inc"
}
#else
void fn_800DACC0(u8* obj) {
    if (*(u8**)(lbl_8047AA80 + 0x480) != obj) {
        fn_800E24B0(*(u16*)(obj + 0x2));
        fn_800E209C(*(u16*)(obj + 0x2));
        *(u8*)(obj + 0x0) = 0;
    }
}
#endif
extern void fn_800BD0FC(u32, u32);
#if 0
asm void fn_800DAD10(void) {
#include "src/game/gs_render_fn_800DAD10.inc"
}
#else
void fn_800DAD10(u32 obj) {
    u32 state = lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x2a, 1, obj);
    } else if (*(u8*)(state + 0x1b) == *(u8*)(state + 0x1a) &&
               (*(u32*)(state + 0x4) & *(u32*)(state + 0x8)) &&
               *(u32*)(obj + 0x8) != 0) {
        fn_800D7A70(*(u32*)(obj + 0xc));
        fn_800D892C(*(u32*)(obj + 0xc));
        fn_800BD0FC(*(u32*)(obj + 0x4), *(u32*)(obj + 0x8));
        fn_800D6A5C(*(u32*)(obj + 0x10), *(u32*)(obj + 0x14));
    }
}
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
#if 0
asm void fn_800DB900(void) {
#include "src/game/gs_render_fn_800DB900.inc"
}
#else
void fn_800DB900(u32 idx, void* src, s8 val) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x5b, 0x12, idx, src, (s32)val); }
    else {
        u8* dst = &lbl_80400B28[(idx - 1) * 0x1c + 0x360];
        memcpy(dst, src, 0x18);
        dst[0x18] = val;
    }
}
#endif
#if 0
asm void fn_800DB988(void) {
#include "src/game/gs_render_fn_800DB988.inc"
}
#else
void fn_800DB988(u32 idx, u8 a, u8 b) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x5a, 3, idx, (u32)a, (u32)b); }
    else {
        lbl_80400B28[idx * 4 + 0x34f] = a;
        lbl_80400B28[idx * 4 + 0x350] = b;
    }
}
#endif
#if 0
asm void fn_800DB9F0(void) {
#include "src/game/gs_render_fn_800DB9F0.inc"
}
#else
void fn_800DB9F0(u32 idx, u8 a, u8 b) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x59, 3, idx, (u32)a, (u32)b); }
    else {
        lbl_80400B28[idx * 4 + 0x34d] = a;
        lbl_80400B28[idx * 4 + 0x34e] = b;
    }
}
#endif
#if 0
asm void fn_800DBA54(void) {
#include "src/game/gs_render_fn_800DBA54.inc"
}
#else
void fn_800DBA54(u8 val) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x58, 1, (u8)val); }
    else { lbl_80400B28[0x34c] = val; }
}
#endif
#if 0
asm void fn_800DBAA4(void) {
#include "src/game/gs_render_fn_800DBAA4.inc"
}
#else
void fn_800DBAA4(u32 idx) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x57, 1, idx); }
    else {
        lbl_80400B28[idx + 0x1fc] = 1;
        lbl_80400B28[idx * 0x14 + 0x20c] = 5;
    }
}
#endif
#if 0
asm void fn_800DBB0C(void) {
#include "src/game/gs_render_fn_800DBB0C.inc"
}
#else
void fn_800DBB0C(u32 idx, u8 a, u8 b) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x56, 3, idx, (u32)a, (u32)b); }
    else {
        lbl_80400B28[idx + 0x1fc] = 1;
        lbl_80400B28[idx * 0x14 + 0x20c] = 4;
        lbl_80400B28[idx * 0x14 + 0x20d] = a;
        lbl_80400B28[idx * 0x14 + 0x20e] = b;
    }
}
#endif
#if 0
asm void fn_800DBB84(void) {
#include "src/game/gs_render_fn_800DBB84.inc"
}
#else
void fn_800DBB84(u32 idx, u8 a, u8 b) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x55, 3, idx, (u32)a, (u32)b); }
    else {
        lbl_80400B28[idx + 0x1fc] = 1;
        lbl_80400B28[idx * 0x14 + 0x20c] = 3;
        lbl_80400B28[idx * 0x14 + 0x20d] = a;
        lbl_80400B28[idx * 0x14 + 0x20e] = b;
    }
}
#endif
#if 1
asm void fn_800DBBFC(void) {
#include "src/game/gs_render_fn_800DBBFC.inc"
}
#else
void fn_800DBBFC(void) { /* TODO */ }
#endif
#if 0
asm void fn_800DBCE4(void) {
#include "src/game/gs_render_fn_800DBCE4.inc"
}
#else
void fn_800DBCE4(u32 idx, u8 a, u8 b, u8 c, u32 d) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x53, 5, idx, (u32)a, (u32)b, (u32)c, d); }
    else {
        lbl_80400B28[idx + 0x1fc] = 1;
        lbl_80400B28[idx * 0x14 + 0x20c] = 1;
        lbl_80400B28[idx * 0x14 + 0x20d] = a;
        lbl_80400B28[idx * 0x14 + 0x216] = b;
        lbl_80400B28[idx * 0x14 + 0x217] = c;
        lbl_80400B28[idx * 0x14 + 0x20e] = d;
    }
}
#endif
#if 1
asm void fn_800DBD70(void) {
#include "src/game/gs_render_fn_800DBD70.inc"
}
#else
void fn_800DBD70(void) { /* TODO */ }
#endif
#if 0
asm void fn_800DBE5C(void) {
#include "src/game/gs_render_fn_800DBE5C.inc"
}
#else
void fn_800DBE5C(u32 idx) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x51, 1, idx); }
    else { lbl_80400B28[idx + 0x1fc] = 0; }
}
#endif
#if 0
asm void fn_800DBEB4(void) {
#include "src/game/gs_render_fn_800DBEB4.inc"
}
#else
void fn_800DBEB4(u32 idx, u32* src) {
    if (*(u32*)lbl_8047AA80 == 1) {
        u32 tmp = *src;
        fn_800D4F98(0x50, 0x14, idx, &tmp);
    } else {
        *(u32*)&lbl_80400B28[idx * 4 + 0x1ec] = *src;
    }
}
#endif
#if 0
asm void fn_800DBF1C(void) {
#include "src/game/gs_render_fn_800DBF1C.inc"
}
#else
void fn_800DBF1C(u32 idx, u32 val) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x4f, 2, idx, val); }
    else { *(u32*)&lbl_80400B28[idx * 4 + 0x1ac] = val; }
}
#endif
#if 0
asm void fn_800DBF78(void) {
#include "src/game/gs_render_fn_800DBF78.inc"
}
#else
void fn_800DBF78(u32 idx, u32 val) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x4e, 2, idx, val); }
    else { *(u32*)&lbl_80400B28[idx * 4 + 0x16c] = val; }
}
#endif
#if 0
asm void fn_800DBFD4(void) {
#include "src/game/gs_render_fn_800DBFD4.inc"
}
#else
void fn_800DBFD4(u32 idx, u8 a, u8 b, u8 c, u8 d) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x4d, 5, idx, (u32)a, (u32)b, (u32)c, (u32)d); }
    else {
        lbl_80400B28[idx * 4 + 0x12b] = a;
        lbl_80400B28[idx * 4 + 0x12c] = b;
        lbl_80400B28[idx * 4 + 0x12d] = c;
        lbl_80400B28[idx * 4 + 0x12e] = d;
    }
}
#endif
#if 0
asm void fn_800DC04C(void) {
#include "src/game/gs_render_fn_800DC04C.inc"
}
#else
void fn_800DC04C(u32 idx, u8 a, u8 b, u8 c, u8 d, u8 e) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x4c, 6, idx, (u32)a, (u32)b, (u32)c, (u32)d, (u32)e); }
    else {
        lbl_80400B28[idx * 5 + 0x9b] = a;
        lbl_80400B28[idx * 5 + 0x9c] = b;
        lbl_80400B28[idx * 5 + 0x9d] = c;
        lbl_80400B28[idx * 5 + 0x9e] = d;
        lbl_80400B28[idx * 5 + 0x9f] = e;
    }
}
#endif
#if 0
asm void fn_800DC0D4(void) {
#include "src/game/gs_render_fn_800DC0D4.inc"
}
#else
void fn_800DC0D4(u32 idx, u8 a, u8 b, u8 c, u8 d) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x4b, 5, idx, (u32)a, (u32)b, (u32)c, (u32)d); }
    else {
        lbl_80400B28[idx * 4 + 0xeb] = a;
        lbl_80400B28[idx * 4 + 0xec] = b;
        lbl_80400B28[idx * 4 + 0xed] = c;
        lbl_80400B28[idx * 4 + 0xee] = d;
    }
}
#endif
#if 0
asm void fn_800DC14C(void) {
#include "src/game/gs_render_fn_800DC14C.inc"
}
#else
void fn_800DC14C(u32 idx, u8 a, u8 b, u8 c, u8 d, u8 e) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x4a, 6, idx, (u32)a, (u32)b, (u32)c, (u32)d, (u32)e); }
    else {
        lbl_80400B28[idx * 5 + 0x4b] = a;
        lbl_80400B28[idx * 5 + 0x4c] = b;
        lbl_80400B28[idx * 5 + 0x4d] = c;
        lbl_80400B28[idx * 5 + 0x4e] = d;
        lbl_80400B28[idx * 5 + 0x4f] = e;
    }
}
#endif
#if 0
asm void fn_800DC1D4(void) {
#include "src/game/gs_render_fn_800DC1D4.inc"
}
#else
void fn_800DC1D4(u8 val) {
    if (*(u32*)lbl_8047AA80 == 1) { fn_800D4F98(0x49, 1, (u8)val); }
    else { lbl_80400B28[0x1a] = val; }
}
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
extern void fn_801A6370(u32, f32);
extern void fn_801A6408(void);
extern u32 lbl_8047AAEC;
extern u32 lbl_8047CA80;
extern f32 lbl_8047CA70;
extern u32 lbl_8047CA74;
extern f32 lbl_8047CA78;
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
extern void fn_801A66E0(u32);
extern void fn_801A426C(u32, u32);
extern void fn_801C028C(u32, u32, u32, void*, u32, ...);
extern s32 fn_800D37CC(void);
extern void fn_801C027C(void);
extern f32 lbl_8047CA78;
extern f32 lbl_8047AAF4;
extern f32 lbl_8047CA88;
#if 1
asm void fn_800DC878(void) {
#include "src/game/gs_render_fn_800DC878.inc"
}
#else
void fn_800DC878(void) { /* TODO */ }
#endif
extern void fn_801A497C(void*, void*);
extern void fn_801A48B0(void*, void*);
#if 0
asm void fn_800DCA10(void) {
#include "src/game/gs_render_fn_800DCA10.inc"
}
#else
void fn_800DCA10(u8* src, u8* dst) {
    dst[0] = src[1];
    dst[1] = src[3];
    fn_801A497C(*(void**)(src + 0xc), dst + 4);
    fn_801A48B0(*(void**)(src + 0xc), dst + 0x10);
    *(u32*)(dst + 0x1c) = *(u32*)(src + 0x60);
    *(f32*)(dst + 0x20) = *(f32*)(src + 0x68);
    *(f32*)(dst + 0x24) = *(f32*)(src + 0x64);
    *(u32*)(dst + 0x28) = *(u32*)(src + 0x5c);
    dst[2] = src[0x70];
    dst[3] = src[0x71];
}
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
extern f32 lbl_8047CA88;
#if 0
asm void fn_800DCAF0(void) {
#include "src/game/gs_render_fn_800DCAF0.inc"
}
#else
void fn_800DCAF0(u8* obj, f32 speed) {
    if (!obj[2]) return;
    if (fn_800D37CC() == 0x32) {
        speed *= lbl_8047CA88;
    }
    *(f32*)(obj + 0x64) = speed;
    fn_801C028C(*(u32*)(obj + 0xc), 7, 0xFFFF, (void*)fn_801C027C, 1, *(f32*)(obj + 0x64));
}
#endif
extern f32 lbl_8047CA78;
extern f32 lbl_8047AAF4;
#if 0
asm void fn_800DCB78(void) {
#include "src/game/gs_render_fn_800DCB78.inc"
}
#else
void fn_800DCB78(u8* obj, u32 frame) {
    if (!obj[2]) return;
    fn_801A66E0(*(u32*)(obj + 0xc));
    if (frame > *(u32*)(obj + 0x58)) return;
    *(u32*)(obj + 0x60) = frame;
    {
        u32 data = *(u32*)(obj + 0x8);
        u32 curFrame = *(u32*)(obj + 0x60);
        u32 frames = *(u32*)(data + 0x4);
        fn_801A426C(*(u32*)(obj + 0xc), *(u32*)(frames + curFrame * 4));
    }
    fn_801A6370(*(u32*)(obj + 0xc), lbl_8047CA78);
    lbl_8047AAF4 = lbl_8047CA78;
    fn_801C028C(*(u32*)(obj + 0xc), 7, 0xFFFF, (void*)fn_800DD128, 0);
    *(f32*)(obj + 0x6c) = lbl_8047AAF4;
}
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
extern void fn_801A4A48(u32, u8*);
#if 0
asm void fn_800DCC84(void) {
#include "src/game/gs_render_fn_800DCC84.inc"
}
#else
void fn_800DCC84(u8* obj, f32* rgb) {
    u8 tmp[4];
    tmp[0] = (u8)(s32)rgb[0];
    tmp[1] = (u8)(s32)rgb[1];
    tmp[2] = (u8)(s32)rgb[2];
    fn_801A4A48(*(u32*)(obj + 0xc), tmp);
}
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
extern f32 lbl_8047CA70;
extern f32 lbl_8047CA78;
extern f32 lbl_8047AAF4;
#if 1
asm void fn_800DCE4C(void) {
#include "src/game/gs_render_fn_800DCE4C.inc"
}
#else
void fn_800DCE4C(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAF0;
extern u32 lbl_8047AAEC;
extern f32 lbl_8047CA78;
extern u32 lbl_8047CA8C;
#if 1
asm void fn_800DCFBC(void) {
#include "src/game/gs_render_fn_800DCFBC.inc"
}
#else
void fn_800DCFBC(void) { /* TODO */ }
#endif
extern u32 lbl_8047AAF0;
extern u16 lbl_8047AAE8;
extern u32 lbl_8047AAEC;
#if 0
asm void fn_800DD0B8(void) {
#include "src/game/gs_render_fn_800DD0B8.inc"
}
#else
void fn_800DD0B8(u32 count) {
    u16 handle;
    lbl_8047AAF0 = count;
    handle = fn_800E3534(count * 0x74);
    lbl_8047AAE8 = handle;
    if (handle) {
        u32 i = 0, off = 0;
        lbl_8047AAEC = (u32)fn_800E27B0(handle);
        do {
            ((u8*)lbl_8047AAEC)[off] = 0;
            off += 0x74;
            i++;
        } while (i < lbl_8047AAF0);
    }
}
#endif
extern void fn_80196E10(u8*, u32, u8*);
extern u8 lbl_8047CA90[];
extern u8 lbl_8047CA98[];
extern f32 lbl_8047CA70;
extern f32 lbl_8047AAF4;
#if 0
asm void fn_800DD128(u8* obj) {
#include "src/game/gs_render_fn_800DD128.inc"
}
#else
void fn_800DD128(u8* obj) {
    if (!obj) fn_80196E10(lbl_8047CA90, 0xab, lbl_8047CA98);
    lbl_8047AAF4 = lbl_8047CA70 + *(f32*)(obj + 0xc);
}
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
asm void fn_800DD970(const char* fmt, ...) {
#include "src/game/gs_render_fn_800DD970.inc"
}
#else
void fn_800DD970(const char* fmt, ...) { /* TODO */ }
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
extern void fn_800B8DF4(u32);
extern void fn_801BBD3C(u32);
#if 0
asm void fn_800DEFC8(void) {
#include "src/game/gs_render_fn_800DEFC8.inc"
}
#else
void fn_800DEFC8(u8* obj) {
    u32 sentinel = *(u32*)(obj + 0x38);
    u8* ptr1;
    u32 old;
    u32 new_val;
    u8* target;
    u32 prev;
    if ((u32)(sentinel + 0x01020000) == 0xfefe) return;
    fn_800B8DF4(sentinel);
    ptr1 = *(u8**)(obj + 0x8);
    old = *(u32*)(ptr1 + 0x8 + 0x58 - 0x8);
    new_val = sentinel;
    target = *(u8**)(ptr1 + 0x8);
    prev = *(u32*)(target + 0x58);
    *(u32*)(target + 0x58) = new_val;
    fn_801BBD3C(prev);
    *(u32*)(obj + 0x38) = 0xfefefefe;
}
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
extern void fn_801A6DDC(u32, ...);
extern u32 lbl_8047CAD0;
extern f32 lbl_8047CACC;
#if 0
asm void fn_800DF140(void) {
#include "src/game/gs_render_fn_800DF140.inc"
}
#else
void fn_800DF140(u8* obj) {
    fn_801A6DDC(*(u32*)(obj + 0x8), (f32)obj[0x1] / lbl_8047CACC);
}
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
extern void fn_801A6FF0(void*);
#if 1
asm void fn_800DF248(void) {
#include "src/game/gs_render_fn_800DF248.inc"
}
#else
void fn_800DF248(void) { /* TODO */ }
#endif
#if 0
asm void fn_800DF384(void) {
#include "src/game/gs_render_fn_800DF384.inc"
}
#else
void fn_800DF384(u8* obj, u32 flags) {
    u16 cur_flags = *(u16*)(obj + 0x2);
    u8* ptr = *(u8**)(obj + 0x8);
    u32 new_bits = flags & ~cur_flags;
    if (!new_bits) return;
    *(u8**)(ptr + 0x20) = obj;
    if (new_bits & (1 << 29)) fn_800DFABC();
    *(u16*)(obj + 0x2) = cur_flags | new_bits;
    fn_801A6FF0(ptr);
}
#endif
extern u32 fn_801A8458(void*);
#if 0
asm void fn_800DF3F0(void) {
#include "src/game/gs_render_fn_800DF3F0.inc"
}
#else
u32 fn_800DF3F0(void* obj) {
    u32 v = fn_801A8458(*(void**)((u8*)obj + 8));
    u32 flags = 0;
    if (v & 0x1) flags |= 0x1;
    if (v & 0x2) flags |= 0x2;
    if (v & 0x4) flags |= 0x4;
    if (v & 0x8) flags |= 0x8;
    if (v & 0x40000000) flags |= 0x10;
    if (v & 0x2000) flags |= 0x20;
    if (v & 0x4000) flags |= 0x40;
    return flags;
}
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
#if 0
asm void fn_800DF498(void) {
#include "src/game/gs_render_fn_800DF498.inc"
}
#else
void fn_800DF498(u8* obj, u32 new_val) {
    u32 sentinel = *(u32*)(obj + 0x3c);
    if ((u32)(sentinel + 0x01020000) == 0xfefe) {
        *(u32*)(obj + 0x3c) = *(u32*)(*(u8**)(obj + 0x8) + 0x10);
    } else {
        fn_800DD970(lbl_80270528);
    }
    *(u32*)(*(u8**)(obj + 0x8) + 0x10) = new_val;
}
#endif
extern void fn_801A8428(void*, u32);
extern void fn_801A8440(void*, void*);
#if 0
asm void fn_800DF504(void) {
#include "src/game/gs_render_fn_800DF504.inc"
}
#else
void fn_800DF504(u8* obj) {
    fn_801A8428(*(void**)(obj + 0x8), 0x4000600f);
    fn_801A8440(*(void**)(obj + 0x8), *(void**)(obj + 0x4));
    fn_801A6FF0(*(void**)(obj + 0x8));
}
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
extern void fn_801A7CFC(void*);
extern u32 lbl_8047AB20;
extern u16 lbl_8047AB18;
extern u32 lbl_8047AB1C;
extern u8 lbl_80315490[];
#if 0
asm void fn_800DF854(void) {
#include "src/game/gs_render_fn_800DF854.inc"
}
#else
void fn_800DF854(u32 count) {
    u16 handle;
    lbl_8047AB20 = count;
    handle = fn_800E3534(count * 0x40);
    lbl_8047AB18 = handle;
    if (handle) {
        u32 i = 0, off = 0;
        lbl_8047AB1C = (u32)fn_800E27B0(handle);
        do { ((u8*)lbl_8047AB1C)[off] = 0; off += 0x40; i++; } while (i < lbl_8047AB20);
        fn_801A7CFC(lbl_80315490);
    }
}
#endif
extern void fn_80193B30(void*, void*, void*, void*, u32, u32);
extern u8 lbl_8036CB30[];
#if 0
asm void fn_800DF8CC(void) {
#include "src/game/gs_render_fn_800DF8CC.inc"
}
#else
void fn_800DF8CC(void) {
    fn_80193B30(lbl_80315490, lbl_8036CB30, (void*)lbl_802705C0, (void*)lbl_802705C0, 0x54, 0x24);
    *(void**)&lbl_80315490[0x40] = (void*)fn_800DFE98;
    *(void**)&lbl_80315490[0x44] = (void*)fn_800DF930;
}
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
#if 0
asm s32 fn_800DFE98(u8* obj) {
#include "src/game/gs_render_fn_800DFE98.inc"
}
#else
s32 fn_800DFE98(u8* obj) {
    s32 r = ((s32(*)(u8*))*(u32*)((u8*)lbl_8036CB30 + 0x40))(obj);
    if (!r) { *(u32*)(obj + 0x20) = 0; return 0; }
    return r;
}
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
asm void fn_800E013C(void* a, void* b, f32 c) {
#include "src/game/gs_render_fn_800E013C.inc"
}
#else
void fn_800E013C(void* a, void* b, f32 c) { fn_800A3AC0(b, a, c); }
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
asm void fn_800E0290(void* a, void* b, void* c) {
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
#if 0
asm void fn_800E02E8(void* obj) {
#include "src/game/gs_render_fn_800E02E8.inc"
}
#else
void fn_800E02E8(void* obj) { u8 tmp[0x38]; fn_800A3074(tmp, 0x5a); fn_800A2D98(obj, tmp, obj); }
#endif
#if 0
asm void fn_800E032C(void* obj) {
#include "src/game/gs_render_fn_800E032C.inc"
}
#else
void fn_800E032C(void* obj) { u8 tmp[0x38]; fn_800A3074(tmp, 0x59); fn_800A2D98(obj, tmp, obj); }
#endif
#if 0
asm void fn_800E0370(void* obj) {
#include "src/game/gs_render_fn_800E0370.inc"
}
#else
void fn_800E0370(void* obj) { u8 tmp[0x38]; fn_800A3074(tmp, 0x58); fn_800A2D98(obj, tmp, obj); }
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
asm void fn_800E03E8(void* a, f32 b, f32 c, f32 d) {
#include "src/game/gs_render_fn_800E03E8.inc"
}
#else
void fn_800E03E8(void* a, f32 b, f32 c, f32 d) { fn_800A32E8(a, a, b, c, d); }
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
#if 0
asm void fn_800E042C(void) {
#include "src/game/gs_render_fn_800E042C.inc"
}
#else
void fn_800E042C(u8* dst, f32* src) {
    memcpy(dst, lbl_80315568, 0x30);
    *(f32*)(dst + 0x00) = src[0];
    *(f32*)(dst + 0x14) = src[1];
    *(f32*)(dst + 0x28) = src[2];
}
#endif
#if 0
asm void fn_800E048C(void) {
#include "src/game/gs_render_fn_800E048C.inc"
}
#else
void fn_800E048C(u8* dst, f32 x, f32 y, f32 z) {
    memcpy(dst, lbl_80315568, 0x30);
    *(f32*)(dst + 0x00) = x;
    *(f32*)(dst + 0x14) = y;
    *(f32*)(dst + 0x28) = z;
}
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
#if 0
asm void fn_800E0560(void) {
#include "src/game/gs_render_fn_800E0560.inc"
}
#else
void fn_800E0560(u8* dst, f32* src) {
    memcpy(dst, lbl_80315568, 0x30);
    *(f32*)(dst + 0x0c) = src[0];
    *(f32*)(dst + 0x1c) = src[1];
    *(f32*)(dst + 0x2c) = src[2];
}
#endif
#if 0
asm void fn_800E05C0(void) {
#include "src/game/gs_render_fn_800E05C0.inc"
}
#else
void fn_800E05C0(u8* dst, f32 x, f32 y, f32 z) {
    memcpy(dst, lbl_80315568, 0x30);
    *(f32*)(dst + 0x0c) = x;
    *(f32*)(dst + 0x1c) = y;
    *(f32*)(dst + 0x2c) = z;
}
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
asm void fn_800E0678(u8* p) {
#include "src/game/gs_render_fn_800E0678.inc"
}
#else
void fn_800E0678(u8* p) { fn_800A3910(); }
#endif
extern void fn_800A39E0(void);
#if 0
asm void fn_800E0698(void) {
#include "src/game/gs_render_fn_800E0698.inc"
}
#else
void fn_800E0698(void* p, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f) { fn_800A39E0(); }
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
extern f32 lbl_8047CAF0;
extern f32 lbl_8047CAF4;
#if 0
asm void fn_800E090C(void) {
#include "src/game/gs_render_fn_800E090C.inc"
}
#else
void fn_800E090C(void* dst, void* src, void* target, f32 t) {
    if (t <= lbl_8047CAF0) {
        fn_800E01D0(dst, src);
    } else if (t >= lbl_8047CAF4) {
        fn_800E01D0(dst, target);
    } else {
        fn_800E0168(dst, target, src);
        fn_800E013C(dst, dst, t);
        fn_800E019C(dst, dst, src);
    }
}
#endif
extern f32 lbl_8047CAF0;
extern f32 lbl_8047CAF4;
#if 0
asm void fn_800E09B4(void) {
#include "src/game/gs_render_fn_800E09B4.inc"
}
#else
f32 fn_800E09B4(f32 start, f32 end, f32 t) {
    if (t <= lbl_8047CAF0) return start;
    if (t >= lbl_8047CAF4) return end;
    return t * (end - start) + start;
}
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
extern f32 fn_801ADC7C(void);
extern f32 lbl_8047CB10;
#if 0
asm void fn_800E0BA0(void) {
#include "src/game/gs_render_fn_800E0BA0.inc"
}
#else
f32 fn_800E0BA0(void) {
    f32 a = fn_801ADC7C();
    f32 b = fn_801ADC7C();
    return b + a - lbl_8047CB10;
}
#endif
#if 0
asm void fn_800E0BE4(void) {
#include "src/game/gs_render_fn_800E0BE4.inc"
}
#else
void fn_800E0BE4(void) { fn_801ADC7C(); }
#endif
extern u16 fn_801ADCD8(void);
#if 0
asm void fn_800E0C04(void) {
#include "src/game/gs_render_fn_800E0C04.inc"
}
#else
u32 fn_800E0C04(u32 mod) {
    u32 a = fn_801ADCD8();
    u32 b = fn_801ADCD8();
    u32 combined = (b << 16) | a;
    return combined % mod;
}
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
extern f64 fn_800CE318(f64 x, f64 y);
extern s32 fn_800C46B0(f32 x);
extern f32 lbl_8047CB20;
extern f32 lbl_8047CB1C;
extern f32 lbl_8047CB24;
extern f64 lbl_8047CB28;
extern f32 lbl_8047CB18;
extern f32 lbl_8047CB34;
extern f32 lbl_8047CB30;
extern f32 lbl_804011B8[];
#if 1
asm void fn_800E0CA0(void) {
#include "src/game/gs_render_fn_800E0CA0.inc"
}
#else
f32 fn_800E0CA0(f32 x) {
    f32 scale;
    f32 tmp;
    scale = lbl_8047CB1C;
    if (x > lbl_8047CB20) {
        scale = lbl_8047CB24;
        tmp = (f32)fn_800CE318(x, lbl_8047CB28);
        x = tmp;
    }
    if (x > lbl_8047CB18) {
        x = lbl_8047CB20 - x;
    }
    x = lbl_8047CB34 * x + lbl_8047CB30;
    return scale * lbl_804011B8[fn_800C46B0(x)];
}
#endif
extern void fn_800CDBE0(void);
extern f32 lbl_8047CB38;
extern f64 lbl_8047CB40;
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
