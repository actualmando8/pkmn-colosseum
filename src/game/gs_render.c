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
    /* cmpwi r3, 0x1 */;
    r31 = r4;
    if (/* eq */) goto .L_800D3E98;
    if (/* ge */) goto .L_800D3EB0;
    /* cmpwi r3, 0x0 */;
    if (/* ge */) goto .L_800D3E78;
    goto .L_800D3EB0;
.L_800D3E78:
    r3 = (u32)lbl_80400248;
    r5 = 0x5a0;
    r3 = (u32)lbl_80400248;
    memcpy();
    r3 = (u32)lbl_80400248;
    r0 = (u32)lbl_80400248;
    *(u32*)lbl_8047AA80 = r0;
    goto .L_800D3EB0;
.L_800D3E98:
    r4 = (u32)lbl_80400248;
    r3 = r31;
    r4 = (u32)lbl_80400248;
    r5 = 0x5a0;
    memcpy();
    *(u32*)lbl_8047AA80 = r31;
.L_800D3EB0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
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
void GSgfx_PreRetraceCallback(s32 flag, f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6) {
    r11 = r1 + 0x40;
    r12 = *(u32*)lbl_8047AA88;
    f26 = f1;
    f27 = f2;
    f28 = f3;
    /* cmplwi r12, 0x0 */;
    f29 = f4;
    f30 = f5;
    f31 = f6;
    if (/* eq */) goto .L_800D3F08;
    /* mtctr r12 */;
    /* indirect call via ctr */;
    goto .L_800D3F38;
.L_800D3F08:
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800D3F34;
    fn_800AA2F0();
    f1 = f26;
    f2 = f27;
    f3 = f28;
    f4 = f29;
    f5 = f30;
    f6 = f31;
    fn_800BD640();
    goto .L_800D3F38;
.L_800D3F34:
    fn_800BD744();
.L_800D3F38:
    r11 = r1 + 0x40;
    r0 = *(u32*)((u8*)r1 + 0x44);
    return;
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
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x4C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x4C) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r12 = *(u32*)((u8*)r3 + 0x48);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800D3F94;
    r3 = *(u32*)((u8*)r3 + 0x4C);
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D3F94:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
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
    r6 = (u32)lbl_804001F0;
    r6 = (u32)lbl_804001F0;
    /* stmw r24, 0x10(r1) */;
    r30 = r3;
    r26 = r4;
    r27 = r5;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x28);
    r7 = *(u32*)((u8*)r8 + 0x490);
    r3 = *(u32*)((u8*)r8 + 0x494);
    r3 = r3 - r7;
    /* cmplw r3, r0 */;
    if (/* le */) goto .L_800D3FE8;
    *(u32*)((u8*)r6 + 0x28) = r3;
.L_800D3FE8:
    r29 = *(u32*)((u8*)r8 + 0x0);
    r0 = 0x2;
    r28 = *(u32*)((u8*)r8 + 0x4);
    *(u32*)((u8*)r8 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x0;
    r3 = (u32)lbl_804001F0;
    r0 = r30 & 0x1;
    *(u8*)((u8*)r4 + 0x1B) = r5;
    r31 = (u32)lbl_804001F0;
    *(u32*)((u8*)r31 + 0x40) = r5;
    *(u32*)((u8*)r31 + 0x44) = r5;
    *(u32*)((u8*)r31 + 0x48) = r5;
    if (/* eq */) goto .L_800D402C;
    fn_801E17A8();
.L_800D402C:
    /* rlwinm r0, r30, 0, 27, 27 */;
    if (/* eq */) goto .L_800D40D4;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x10;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if (/* eq */) goto .L_800D40A8;
    fn_800D2584();
    /* mr. r24, r3 */;
    if (/* eq */) goto .L_800D40A8;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800D40A0;
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_800DD174();
    r3 = 0x10;
    r4 = 0x0;
    fn_800E3604();
    r3 = 0x10;
    r4 = 0x0;
    fn_80118104();
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    fn_80195A48();
.L_800D40A0:
    r3 = -0x1;
    fn_800D87AC();
.L_800D40A8:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto .L_800D40B8;
.L_800D40B4:
    fn_800D461C();
.L_800D40B8:
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800D40B4;
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x40) = r0;
.L_800D40D4:
    /* rlwinm r0, r30, 0, 19, 19 */;
    if (/* eq */) goto .L_800D415C;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x1000;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if (/* eq */) goto .L_800D4150;
    fn_800D2584();
    /* mr. r24, r3 */;
    if (/* eq */) goto .L_800D4150;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800D4148;
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_800DD174();
    r3 = 0x1000;
    r4 = 0x0;
    fn_800E3604();
    r3 = 0x1000;
    r4 = 0x0;
    fn_80118104();
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    fn_80195A48();
.L_800D4148:
    r3 = -0x1;
    fn_800D87AC();
.L_800D4150:
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x44) = r0;
.L_800D415C:
    /* rlwinm r0, r30, 0, 18, 18 */;
    if (/* eq */) goto .L_800D4204;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x2000;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if (/* eq */) goto .L_800D41D8;
    fn_800D2584();
    /* mr. r24, r3 */;
    if (/* eq */) goto .L_800D41D8;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800D41D0;
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_800DD174();
    r3 = 0x2000;
    r4 = 0x0;
    fn_800E3604();
    r3 = 0x2000;
    r4 = 0x0;
    fn_80118104();
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    fn_80195A48();
.L_800D41D0:
    r3 = -0x1;
    fn_800D87AC();
.L_800D41D8:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto .L_800D41E8;
.L_800D41E4:
    fn_800D461C();
.L_800D41E8:
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800D41E4;
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x48) = r0;
.L_800D4204:
    r3 = *(u32*)lbl_8047AA80;
    r0 = -0x1;
    r4 = 0x0;
    r5 = 0x1;
    *(u32*)((u8*)r3 + 0x4) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r24 = *(u8*)((u8*)r3 + 0x1A);
    r0 = *(u8*)((u8*)r3 + 0x1B);
    *(u8*)((u8*)r3 + 0x1A) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x49C);
    fn_800DA2BC();
    r3 = 0x1;
    r4 = 0x7;
    r5 = 0x2;
    fn_800DA1E8();
    r3 = 0x0;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x7;
    r8 = 0x0;
    fn_800DA100();
    r3 = 0x1;
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    f1 = *(f32*)lbl_8047CA20;
    f3 = *(f32*)lbl_8047CA24;
    f2 = f1;
    f4 = *(f32*)lbl_8047CA28;
    fn_800D9B58();
    r3 = 0x0;
    fn_800D9FB4();
    r3 = 0x0;
    fn_800DA028();
    r3 = (u32)lbl_803147C8;
    r3 = (u32)lbl_803147C8;
    fn_800D7820();
    r3 = 0x4;
    fn_800D6A00();
    r3 = 0x4;
    fn_800D67BC();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x752f;
    fn_800D65CC();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0xc4;
    fn_800D5CB8();
    r3 = 0x280;
    r4 = 0x0;
    r5 = 0x752f;
    fn_800D65CC();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0xc4;
    fn_800D5CB8();
    r3 = 0x0;
    r4 = 0x1e0;
    r5 = 0x752f;
    fn_800D65CC();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0xc4;
    fn_800D5CB8();
    r3 = 0x280;
    r4 = 0x1e0;
    r5 = 0x752f;
    fn_800D65CC();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0xc4;
    fn_800D5CB8();
    fn_800D6728();
    r6 = *(u32*)lbl_8047AA80;
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x1;
    *(u8*)((u8*)r6 + 0x1A) = r24;
    fn_800DA2BC();
    r3 = 0x1;
    r4 = 0x7;
    r5 = 0x2;
    fn_800DA1E8();
    r3 = 0x1;
    fn_800D9FB4();
    r0 = -0x102;
    r3 = (u32)lbl_804001F0;
    r30 = r30 & r0;
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x1;
    r31 = (u32)lbl_804001F0;
    r0 = r30 & 0x1;
    *(u8*)((u8*)r4 + 0x1B) = r5;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x4C) = r0;
    *(u32*)((u8*)r31 + 0x50) = r0;
    *(u32*)((u8*)r31 + 0x54) = r0;
    if (/* eq */) goto .L_800D43BC;
    fn_801E17A8();
.L_800D43BC:
    /* rlwinm r0, r30, 0, 27, 27 */;
    if (/* eq */) goto .L_800D4464;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x10;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if (/* eq */) goto .L_800D4438;
    fn_800D2584();
    /* mr. r24, r3 */;
    if (/* eq */) goto .L_800D4438;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800D4430;
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_800DD174();
    r3 = 0x10;
    r4 = 0x1;
    fn_800E3604();
    r3 = 0x10;
    r4 = 0x1;
    fn_80118104();
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    fn_80195A48();
.L_800D4430:
    r3 = -0x1;
    fn_800D87AC();
.L_800D4438:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto .L_800D4448;
.L_800D4444:
    fn_800D461C();
.L_800D4448:
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800D4444;
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x4C) = r0;
.L_800D4464:
    /* rlwinm r0, r30, 0, 19, 19 */;
    if (/* eq */) goto .L_800D44EC;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x1000;
    r0 = r26 & 0xFF;
    r24 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if (/* eq */) goto .L_800D44E0;
    fn_800D2584();
    /* mr. r25, r3 */;
    if (/* eq */) goto .L_800D44E0;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r25 + 0xC);
    fn_80195A6C();
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800D44D8;
    r3 = *(u32*)((u8*)r25 + 0xC);
    fn_800DD174();
    r3 = 0x1000;
    r4 = 0x1;
    fn_800E3604();
    r3 = 0x1000;
    r4 = 0x1;
    fn_80118104();
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    fn_80195A48();
.L_800D44D8:
    r3 = -0x1;
    fn_800D87AC();
.L_800D44E0:
    OSGetTick();
    r0 = r3 - r24;
    *(u32*)((u8*)r31 + 0x50) = r0;
.L_800D44EC:
    /* rlwinm r0, r30, 0, 18, 18 */;
    if (/* eq */) goto .L_800D4594;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x2000;
    r0 = r26 & 0xFF;
    r30 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if (/* eq */) goto .L_800D4568;
    fn_800D2584();
    /* mr. r26, r3 */;
    if (/* eq */) goto .L_800D4568;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r26 + 0xC);
    fn_80195A6C();
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800D4560;
    r3 = *(u32*)((u8*)r26 + 0xC);
    fn_800DD174();
    r3 = 0x2000;
    r4 = 0x1;
    fn_800E3604();
    r3 = 0x2000;
    r4 = 0x1;
    fn_80118104();
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    fn_80195A48();
.L_800D4560:
    r3 = -0x1;
    fn_800D87AC();
.L_800D4568:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto .L_800D4578;
.L_800D4574:
    fn_800D461C();
.L_800D4578:
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800D4574;
    OSGetTick();
    r0 = r3 - r30;
    *(u32*)((u8*)r31 + 0x54) = r0;
.L_800D4594:
    r5 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    r3 = 0x1;
    r4 = 0x1;
    *(u8*)((u8*)r5 + 0x1B) = r0;
    r5 = 0x1;
    fn_800DA2BC();
    r3 = 0x1;
    r4 = 0x2;
    r5 = 0x2;
    fn_800DA1E8();
    r3 = *(u32*)lbl_8047AA80;
    r0 = r27 & 0xFF;
    *(u32*)((u8*)r3 + 0x0) = r29;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x4) = r28;
    if (/* eq */) goto .L_800D45E4;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x490);
    *(u32*)((u8*)r3 + 0x494) = r0;
.L_800D45E4:
    /* lmw r24, 0x10(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x34);
    return;
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
    r31 = r3 + 0x4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x5b */;
    if (/* gt */) goto .L_800D4F7C;
    r3 = (u32)jumptable_80314188;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80314188;
    /* lwzx r0, r3, r0 */;
    /* mtctr r0 */;
    /* indirect jump via ctr */;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D6A00();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFFFF;
    fn_800D67BC();
    goto .L_800D4F7C;
    fn_800D6728();
    goto .L_800D4F7C;
    fn_800D30AC();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D6680();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = r3 & 0xFFFF;
    r4 = r4 & 0xFFFF;
    r31 = r31 + 0xc;
    r5 = r0 & 0xFFFF;
    fn_800D65CC();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s16)r3;
    r4 = (s16)r4;
    r31 = r31 + 0xc;
    r5 = (s16)r0;
    fn_800D6518();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = r3 & 0xFF;
    r4 = r4 & 0xFF;
    r31 = r31 + 0xc;
    r5 = r0 & 0xFF;
    fn_800D6464();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s8)r3;
    r4 = (s8)r4;
    r31 = r31 + 0xc;
    r5 = (s8)r0;
    fn_800D63B0();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800D631C();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = r3 & 0xFFFF;
    r4 = r0 & 0xFFFF;
    fn_800D6280();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = (s16)r3;
    r4 = (s16)r0;
    fn_800D61E4();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = r3 & 0xFF;
    r4 = r0 & 0xFF;
    fn_800D6148();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = (s8)r3;
    r4 = (s8)r0;
    fn_800D60AC();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFFFF;
    fn_800D6028();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800D5FA4();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D5F34();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s16)r3;
    r4 = (s16)r4;
    r31 = r31 + 0xc;
    r5 = (s16)r0;
    fn_800D5EB4();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s8)r3;
    r4 = (s8)r4;
    r31 = r31 + 0xc;
    r5 = (s8)r0;
    fn_800D5E34();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFFFF;
    fn_800D5DD0();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800D5D6C();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r4 = r0 & 0xFF;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r5 = r3 & 0xFF;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r6 = r6 & 0xFF;
    r7 = r0 & 0xFF;
    r31 = r31 + 0x14;
    fn_800D5CB8();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0xC);
    r4 = r3 & 0xFF;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = r5 & 0xFF;
    r6 = r0 & 0xFF;
    r31 = r31 + 0x10;
    fn_800D5C18();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800D5BA0();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D5B28();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D5AB0();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFF;
    fn_800D5A38();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    f1 = *(f32*)((u8*)r31 + 0x4);
    f2 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D59B8();
    goto .L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = r4 & 0xFFFF;
    r5 = r0 & 0xFFFF;
    r31 = r31 + 0xc;
    fn_800D592C();
    goto .L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = (s16)r4;
    r5 = (s16)r0;
    r31 = r31 + 0xc;
    fn_800D58A0();
    goto .L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = r4 & 0xFF;
    r5 = r0 & 0xFF;
    r31 = r31 + 0xc;
    fn_800D5814();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r4 = (s8)r3;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = (s8)r0;
    r31 = r31 + 0xc;
    fn_800D58A0();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D579C();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFF;
    fn_800D5724();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800D56C0();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D5648();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D55D0();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800D85D4();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    r6 = r31;
    fn_800D848C();
    r31 = r31 + 0x30;
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D88DC();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D888C();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DAD10();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DA4C4();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DA428();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFF;
    fn_800DA3B0();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DA2BC();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DA1E8();
    goto .L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0x14);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = r4 & 0xFF;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r8 = r0 & 0xFF;
    r6 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x18;
    fn_800DA100();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DA08C();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DA028();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9FB4();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9F40();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9ED8();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9E4C();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r3 = r0 & 0xFFFF;
    r0 = *(u32*)((u8*)r31 + 0xC);
    r4 = r4 & 0xFFFF;
    r5 = r5 & 0xFFFF;
    r31 = r31 + 0x10;
    r6 = r0 & 0xFFFF;
    fn_800D9D68();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r3 = r0 & 0xFFFF;
    r0 = *(u32*)((u8*)r31 + 0xC);
    r4 = r4 & 0xFFFF;
    r5 = r5 & 0xFFFF;
    r31 = r31 + 0x10;
    r6 = r0 & 0xFFFF;
    fn_800D9C24();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    f4 = *(f32*)((u8*)r31 + 0xC);
    r31 = r31 + 0x10;
    fn_800D9BD0();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    f4 = *(f32*)((u8*)r31 + 0xC);
    r31 = r31 + 0x10;
    fn_800D9B58();
    goto .L_800D4F7C;
    fn_800D834C();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D8284();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D81EC();
    goto .L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D8154();
    goto .L_800D4F7C;
    r3 = r31;
    fn_800D8088();
    r31 = r31 + 0x30;
    goto .L_800D4F7C;
    r3 = r31;
    fn_800D7FE4();
    r31 = r31 + 0x30;
    goto .L_800D4F7C;
    r3 = r31;
    fn_800D7F14();
    r31 = r31 + 0x30;
    goto .L_800D4F7C;
    fn_800D7E5C();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r4 = r31;
    r3 = r0 & 0xFF;
    fn_800D7D90();
    r31 = r31 + 0x30;
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = r0 & 0xFF;
    fn_800D7D10();
    goto .L_800D4F7C;
    fn_800D7C74();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D7820();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D76A8();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DC224();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800DC1D4();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r7 = r0 & 0xFF;
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r8 = *(u32*)((u8*)r31 + 0x14);
    r31 = r31 + 0x18;
    fn_800DC14C();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DC0D4();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r7 = r0 & 0xFF;
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r8 = *(u32*)((u8*)r31 + 0x14);
    r31 = r31 + 0x18;
    fn_800DC04C();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DBFD4();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800DBF78();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800DBF1C();
    goto .L_800D4F7C;
    r30 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r4 = r31;
    r3 = r1 + 0x14;
    r5 = 0x4;
    memcpy();
    r0 = *(u32*)((u8*)r1 + 0x14);
    r3 = r30;
    r4 = r1 + 0x10;
    r31 = r31 + 0x4;
    fn_800DBEB4();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DBE5C();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r0 = r0 & 0xFF;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x24);
    r3 = *(u32*)((u8*)r31 + 0x8);
    r4 = *(u32*)((u8*)r31 + 0xC);
    r10 = r0 & 0xFF;
    r5 = *(u32*)((u8*)r31 + 0x10);
    r6 = *(u32*)((u8*)r31 + 0x14);
    r7 = *(u32*)((u8*)r31 + 0x18);
    r8 = *(u32*)((u8*)r31 + 0x1C);
    r9 = *(u32*)((u8*)r31 + 0x20);
    r31 = r31 + 0x28;
    fn_800DBD70();
    goto .L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0xC);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = r4 & 0xFF;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r6 = r0 & 0xFF;
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DBCE4();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x10);
    r3 = *(u32*)((u8*)r31 + 0x14);
    r4 = *(u32*)((u8*)r31 + 0x18);
    r5 = r0 & 0xFFFF;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    r6 = r3 & 0xFFFF;
    r3 = *(u32*)((u8*)r31 + 0x8);
    r7 = r4 & 0xFFFF;
    r4 = *(u32*)((u8*)r31 + 0xC);
    r8 = r0 & 0xFFFF;
    r9 = *(u32*)((u8*)r31 + 0x20);
    r10 = *(u32*)((u8*)r31 + 0x24);
    r31 = r31 + 0x28;
    fn_800DBBFC();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DBB84();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DBB0C();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DBAA4();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800DBA54();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DB9F0();
    goto .L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DB988();
    goto .L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x64);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r4 = r31;
    r5 = (s8)r0;
    r31 = r31 + 0x64;
    fn_800DB900();
.L_800D4F7C:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r3 = r31;
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    return;
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
    if (/* ne */) goto .L_800D4FD0;
    *(f64*)((u8*)r1 + 0x28) = f1;
    *(f64*)((u8*)r1 + 0x30) = f2;
    *(f64*)((u8*)r1 + 0x38) = f3;
    *(f64*)((u8*)r1 + 0x40) = f4;
    *(f64*)((u8*)r1 + 0x48) = f5;
    *(f64*)((u8*)r1 + 0x50) = f6;
    *(f64*)((u8*)r1 + 0x58) = f7;
    *(f64*)((u8*)r1 + 0x60) = f8;
.L_800D4FD0:
    /* cmplwi r4, 0x14 */;
    r30 = *(u32*)lbl_8047AA80;
    r11 = r1 + 0x98;
    r0 = r1 + 0x8;
    r12 = (0x200 << 16);
    r31 = r1 + 0x70;
    r6 = *(u32*)((u8*)r30 + 0x494);
    r5 = r6 + 0x4;
    *(u32*)((u8*)r30 + 0x494) = r5;
    *(u32*)((u8*)r6 + 0x0) = r3;
    if (/* gt */) goto .L_800D54EC;
    r3 = (u32)jumptable_803142F8;
    r0 = r4 << 2;
    r3 = (u32)jumptable_803142F8;
    /* lwzx r0, r3, r0 */;
    /* mtctr r0 */;
    /* indirect jump via ctr */;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = r1 + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    goto .L_800D54EC;
    r3 = r31;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)((u8*)r1 + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)((u8*)r1 + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    r3 = r1 + 0x70;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)((u8*)r1 + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)((u8*)r1 + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    r3 = r1 + 0x70;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)((u8*)r1 + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)((u8*)r1 + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    r3 = r1 + 0x70;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)((u8*)r1 + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)((u8*)r1 + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    goto .L_800D54EC;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x3;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r3 = r31;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    *(u32*)((u8*)r5 + 0x0) = r7;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r3 = r31;
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x3;
    f0 = (f32)f0;
    r5 = *(u32*)((u8*)r6 + 0x494);
    *(f32*)((u8*)r1 + 0x6C) = f0;
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    r0 = *(u32*)((u8*)r1 + 0x6C);
    *(u32*)((u8*)r5 + 0x0) = r0;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)((u8*)r1 + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)((u8*)r1 + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    goto .L_800D54EC;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r5 = 0x30;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r6 + 0x494);
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x30;
    *(u32*)((u8*)r4 + 0x494) = r0;
    goto .L_800D54EC;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x1;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r3 = r31;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    *(u32*)((u8*)r5 + 0x0) = r7;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r5 = 0x30;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r6 + 0x494);
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x30;
    *(u32*)((u8*)r4 + 0x494) = r0;
    goto .L_800D54EC;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x1;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r3 = r31;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    *(u32*)((u8*)r5 + 0x0) = r7;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r5 = 0x18;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r6 + 0x494);
    memcpy();
    r6 = *(u32*)lbl_8047AA80;
    r3 = r31;
    r4 = 0x1;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x60;
    *(u32*)((u8*)r6 + 0x494) = r0;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    goto .L_800D54EC;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x1;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r3 = r31;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    *(u32*)((u8*)r5 + 0x0) = r7;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x1;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r3 = r31;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    *(u32*)((u8*)r5 + 0x0) = r7;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x1;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r3 = r31;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    *(u32*)((u8*)r5 + 0x0) = r7;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r5 = 0x30;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r6 + 0x494);
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x30;
    *(u32*)((u8*)r4 + 0x494) = r0;
    goto .L_800D54EC;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r4 = 0x0;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r3 = r31;
    r5 = *(u32*)((u8*)r6 + 0x494);
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    *(u32*)((u8*)r5 + 0x0) = r7;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
.L_800D54EC:
    r0 = *(u32*)((u8*)r1 + 0x94);
    r31 = *(u32*)((u8*)r1 + 0x8C);
    r30 = *(u32*)((u8*)r1 + 0x88);
    return;
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
    /* stmw r18, 0x8(r1) */;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x10);
    /* rlwinm r4, r5, 0, 1, 1 */;
    if (/* ne */) goto .L_800D8C48;
    r4 = *(u8*)((u8*)r3 + 0x94);
    r0 = 0x1;
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800D8964;
    r0 = 0x2;
    goto .L_800D8978;
.L_800D8964:
    /* rlwinm r4, r5, 0, 29, 29 */;
    if (/* ne */) goto .L_800D8978;
    r4 = r5 & 0x1;
    if (/* ne */) goto .L_800D8978;
    r0 = 0x0;
.L_800D8978:
    *(u8*)((u8*)r6 + 0x60) = r0;
    /* cmpwi r0, 0x0 */;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
    if (/* le */) goto .L_800D8C48;
    r7 = *(u32*)lbl_8047AA80;
    r8 = *(u32*)((u8*)r7 + 0x10);
    /* rlwinm r4, r8, 0, 29, 29 */;
    if (/* eq */) goto .L_800D8B1C;
    r5 = 0x7f1;
    r4 = 0x0;
    r9 = 0x10;
    r6 = 0x1;
    /* mtctr r5 */;
.L_800D89B8:
    r5 = *(u32*)((u8*)r7 + 0x10);
    /* and. r5, r5, r9 */;
    if (/* eq */) goto .L_800D89D0;
    /* subi r5, r9, 0x10 */;
    r5 = r6 << r5;
    r4 = r4 | r5;
.L_800D89D0:
    r9 = r9 + 0x1;
    if (--ctr != 0) goto .L_800D89B8;
    r5 = r8 & 0x1;
    if (/* eq */) goto .L_800D8A7C;
    r5 = 0x4;
    r18 = 0x0;
    /* cmpwi r5, 0x4 */;
    if (/* ne */) goto .L_800D89FC;
    r5 = r18;
    r18 = 0x1;
    goto .L_800D8A0C;
.L_800D89FC:
    /* cmpwi r5, 0x5 */;
    if (/* ne */) goto .L_800D8A0C;
    r5 = 0x1;
    r18 = r5;
.L_800D8A0C:
    r5 = r5 * 0x6;
    r12 = 0x0;
    r7 = 0x2;
    r8 = r12;
    r9 = 0x1;
    goto .L_800D8A60;
.L_800D8A24:
    r10 = *(u32*)lbl_8047AA80;
    r11 = r5 + 0x61;
    r6 = r18 & 0xFF;
    r11 = r10 + r11;
    *(u8*)((u8*)r11 + 0x0) = r9;
    *(u8*)((u8*)r11 + 0x1) = r8;
    *(u8*)((u8*)r11 + 0x2) = r9;
    *(u8*)((u8*)r11 + 0x3) = r4;
    *(u8*)((u8*)r11 + 0x4) = r7;
    *(u8*)((u8*)r11 + 0x5) = r7;
    if (/* eq */) goto .L_800D8A5C;
    r18 = 0x0;
    r5 = r5 + 0x6;
    goto .L_800D8A60;
.L_800D8A5C:
    r12 = 0x1;
.L_800D8A60:
    r6 = r12 & 0xFF;
    if (/* eq */) goto .L_800D8A24;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
    goto .L_800D8BB4;
.L_800D8A7C:
    r5 = 0x4;
    r19 = 0x0;
    /* cmpwi r5, 0x4 */;
    if (/* ne */) goto .L_800D8A98;
    r5 = r19;
    r19 = 0x1;
    goto .L_800D8AA8;
.L_800D8A98:
    /* cmpwi r5, 0x5 */;
    if (/* ne */) goto .L_800D8AA8;
    r5 = 0x1;
    r19 = r5;
.L_800D8AA8:
    r5 = r5 * 0x6;
    r18 = 0x0;
    r7 = 0x2;
    r9 = r18;
    r8 = r18;
    r10 = 0x1;
    goto .L_800D8B00;
.L_800D8AC4:
    r11 = *(u32*)lbl_8047AA80;
    r12 = r5 + 0x61;
    r6 = r19 & 0xFF;
    r12 = r11 + r12;
    *(u8*)((u8*)r12 + 0x0) = r10;
    *(u8*)((u8*)r12 + 0x1) = r9;
    *(u8*)((u8*)r12 + 0x2) = r8;
    *(u8*)((u8*)r12 + 0x3) = r4;
    *(u8*)((u8*)r12 + 0x4) = r7;
    *(u8*)((u8*)r12 + 0x5) = r7;
    if (/* eq */) goto .L_800D8AFC;
    r19 = 0x0;
    r5 = r5 + 0x6;
    goto .L_800D8B00;
.L_800D8AFC:
    r18 = 0x1;
.L_800D8B00:
    r6 = r18 & 0xFF;
    if (/* eq */) goto .L_800D8AC4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
    goto .L_800D8BB4;
.L_800D8B1C:
    r4 = 0x4;
    r12 = 0x0;
    /* cmpwi r4, 0x4 */;
    if (/* ne */) goto .L_800D8B38;
    r4 = r12;
    r12 = 0x1;
    goto .L_800D8B48;
.L_800D8B38:
    /* cmpwi r4, 0x5 */;
    if (/* ne */) goto .L_800D8B48;
    r4 = 0x1;
    r12 = r4;
.L_800D8B48:
    r4 = r4 * 0x6;
    r11 = 0x0;
    r7 = 0x1;
    r8 = r11;
    r6 = 0x2;
    goto .L_800D8B9C;
.L_800D8B60:
    r9 = *(u32*)lbl_8047AA80;
    r10 = r4 + 0x61;
    r5 = r12 & 0xFF;
    r10 = r9 + r10;
    *(u8*)((u8*)r10 + 0x0) = r8;
    *(u8*)((u8*)r10 + 0x1) = r7;
    *(u8*)((u8*)r10 + 0x2) = r7;
    *(u8*)((u8*)r10 + 0x3) = r8;
    *(u8*)((u8*)r10 + 0x4) = r8;
    *(u8*)((u8*)r10 + 0x5) = r6;
    if (/* eq */) goto .L_800D8B98;
    r12 = r8;
    r4 = r4 + 0x6;
    goto .L_800D8B9C;
.L_800D8B98:
    r11 = r7;
.L_800D8B9C:
    r5 = r11 & 0xFF;
    if (/* eq */) goto .L_800D8B60;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
.L_800D8BB4:
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D8C48;
    r4 = 0x5;
    r12 = 0x0;
    goto .L_800D8BCC;
    goto .L_800D8BDC;
.L_800D8BCC:
    /* cmpwi r4, 0x5 */;
    if (/* ne */) goto .L_800D8BDC;
    r4 = 0x1;
    r12 = r4;
.L_800D8BDC:
    r4 = r4 * 0x6;
    r11 = 0x0;
    r7 = 0x1;
    r8 = r11;
    r6 = 0x2;
    goto .L_800D8C30;
.L_800D8BF4:
    r9 = *(u32*)lbl_8047AA80;
    r10 = r4 + 0x61;
    r5 = r12 & 0xFF;
    r10 = r9 + r10;
    *(u8*)((u8*)r10 + 0x0) = r8;
    *(u8*)((u8*)r10 + 0x1) = r7;
    *(u8*)((u8*)r10 + 0x2) = r7;
    *(u8*)((u8*)r10 + 0x3) = r8;
    *(u8*)((u8*)r10 + 0x4) = r8;
    *(u8*)((u8*)r10 + 0x5) = r6;
    if (/* eq */) goto .L_800D8C2C;
    r12 = r8;
    r4 = r4 + 0x6;
    goto .L_800D8C30;
.L_800D8C2C:
    r11 = r7;
.L_800D8C30:
    r5 = r11 & 0xFF;
    if (/* eq */) goto .L_800D8BF4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
.L_800D8C48:
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x10);
    /* rlwinm r4, r5, 0, 30, 30 */;
    if (/* ne */) goto .L_800D8DC4;
    r3 = 0x0;
    /* cmpwi r0, 0x1 */;
    *(u8*)((u8*)r6 + 0x79) = r3;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r3 + 0x414) = r0;
    if (/* ne */) goto .L_800D8CCC;
    r3 = *(u32*)lbl_8047AA80;
    r4 = 0x1;
    r5 = 0xff;
    r0 = 0x4;
    *(u8*)((u8*)r3 + 0x7A) = r4;
    r3 = 0x0;
    r4 = 0x4;
    r7 = *(u32*)lbl_8047AA80;
    r6 = *(u32*)((u8*)r7 + 0x414);
    r6 = r6 | 0x4;
    *(u32*)((u8*)r7 + 0x414) = r6;
    r6 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r6 + 0x7B) = r5;
    *(u8*)((u8*)r6 + 0x7C) = r5;
    *(u8*)((u8*)r6 + 0x7D) = r0;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r0;
    fn_800D963C();
    goto .L_800D8DA4;
.L_800D8CCC:
    r3 = *(u32*)lbl_8047AA80;
    r4 = 0x2;
    r5 = 0xff;
    r0 = 0x4;
    *(u8*)((u8*)r3 + 0x7A) = r4;
    r3 = 0x0;
    r4 = 0x4;
    r7 = *(u32*)lbl_8047AA80;
    r6 = *(u32*)((u8*)r7 + 0x414);
    r6 = r6 | 0x4;
    *(u32*)((u8*)r7 + 0x414) = r6;
    r6 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r6 + 0x7B) = r5;
    *(u8*)((u8*)r6 + 0x7C) = r5;
    *(u8*)((u8*)r6 + 0x7D) = r0;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r0;
    fn_800D963C();
    r8 = *(u32*)lbl_8047AA80;
    r0 = 0xff;
    r6 = 0x5;
    r7 = 0x0;
    *(u8*)((u8*)r8 + 0x7E) = r0;
    r5 = 0x1;
    r4 = 0xf;
    r3 = 0xa;
    *(u8*)((u8*)r8 + 0x7F) = r0;
    r0 = 0xc;
    *(u8*)((u8*)r8 + 0x80) = r6;
    r8 = *(u32*)lbl_8047AA80;
    r6 = *(u32*)((u8*)r8 + 0x414);
    r6 = r6 | 0x4;
    *(u32*)((u8*)r8 + 0x414) = r6;
    r6 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r6 + 0xB0) = r7;
    *(u8*)((u8*)r6 + 0xB1) = r7;
    *(u8*)((u8*)r6 + 0xB2) = r7;
    *(u8*)((u8*)r6 + 0xB3) = r5;
    *(u8*)((u8*)r6 + 0xB4) = r7;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x414);
    r5 = r5 | 0x4;
    *(u32*)((u8*)r6 + 0x414) = r5;
    r5 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r5 + 0x14F) = r4;
    *(u8*)((u8*)r5 + 0x150) = r3;
    *(u8*)((u8*)r5 + 0x151) = r0;
    *(u8*)((u8*)r5 + 0x152) = r7;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
.L_800D8DA4:
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x3AC) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    goto .L_800D9224;
.L_800D8DC4:
    /* clrrwi. r0, r5, 31 */;
    if (/* eq */) goto .L_800D90B4;
    r4 = (u32)lbl_80400B28;
    r5 = (u32)lbl_80400B28;
    r4 = *(u8*)((u8*)r5 + 0x34C);
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800D8DF4;
    r0 = r4 + 0x1;
    r4 = r0 & 0xFF;
    *(u8*)((u8*)r5 + 0x34C) = r0;
    /* subi r0, r4, 0x1 */;
    *(u8*)((u8*)r5 + 0x34C) = r0;
.L_800D8DF4:
    r0 = *(u8*)((u8*)r3 + 0xB0);
    r4 = 0x0;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E08;
    r4 = 0x0;
.L_800D8E08:
    r0 = *(u8*)((u8*)r3 + 0xCC);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E18;
    r4 = 0x1;
.L_800D8E18:
    r0 = *(u8*)((u8*)r3 + 0xE8);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E28;
    r4 = 0x2;
.L_800D8E28:
    r0 = *(u8*)((u8*)r3 + 0x104);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E38;
    r4 = 0x3;
.L_800D8E38:
    r0 = *(u8*)((u8*)r3 + 0x120);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E48;
    r4 = 0x4;
.L_800D8E48:
    r0 = *(u8*)((u8*)r3 + 0x13C);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E58;
    r4 = 0x5;
.L_800D8E58:
    r0 = *(u8*)((u8*)r3 + 0x158);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E68;
    r4 = 0x6;
.L_800D8E68:
    r0 = *(u8*)((u8*)r3 + 0x174);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8E78;
    r4 = 0x7;
.L_800D8E78:
    r3 = (u32)lbl_80400B28;
    r21 = 0x0;
    r28 = (u32)lbl_80400B28;
    r0 = r4 + 0x1;
    r19 = *(u8*)((u8*)r28 + 0x1A);
    r3 = (u32)lbl_80314404;
    r20 = *(u8*)((u8*)r5 + 0x34C);
    r4 = (u32)lbl_803144F0;
    r31 = (u32)lbl_80314404;
    r29 = r21;
    *(u8*)((u8*)r6 + 0x79) = r0;
    r27 = r21;
    r26 = r28;
    r25 = r21;
    r5 = *(u32*)lbl_8047AA80;
    r24 = r28;
    r23 = r28;
    r22 = r21;
    r0 = *(u32*)((u8*)r5 + 0x414);
    r30 = (u32)lbl_803144F0;
    r0 = r0 | 0x2;
    *(u32*)((u8*)r5 + 0x414) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x7A) = r19;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x3AC) = r20;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    goto .L_800D904C;
.L_800D8F04:
    r0 = *(u8*)((u8*)r28 + 0x1FC);
    r6 = r29 + 0x42e;
    r3 = *(u32*)lbl_8047AA80;
    r4 = 0x0;
    /* cmplwi r0, 0x0 */;
    r6 = r3 + r6;
    if (/* eq */) goto .L_800D8F2C;
    /* cmpwi r20, 0x0 */;
    if (/* le */) goto .L_800D8F2C;
    r4 = 0x1;
.L_800D8F2C:
    r0 = r21 + 0x25c;
    r8 = lbl_80478AE0@sda21;
    /* stbx r4, r3, r0 */;
    r18 = r4 & 0xFF;
    r10 = r27 + 0x7b;
    r3 = r25 + 0xab;
    r7 = *(u8*)((u8*)r6 + 0x0);
    r4 = r26 + 0x4b;
    r0 = *(u8*)((u8*)r6 + 0x2);
    r5 = 0x5;
    r9 = r7 << 2;
    r6 = *(u8*)((u8*)r6 + 0x1);
    r7 = r0 << 2;
    r0 = *(u32*)lbl_8047AA80;
    r6 = r6 << 2;
    /* lwzx r8, r8, r9 */;
    r10 = r0 + r10;
    /* lwzx r7, r30, r7 */;
    /* lwzx r0, r31, r6 */;
    *(u8*)((u8*)r10 + 0x0) = r0;
    *(u8*)((u8*)r10 + 0x1) = r7;
    *(u8*)((u8*)r10 + 0x2) = r8;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r6 + 0x414) = r0;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r3;
    memcpy();
    r0 = *(u32*)lbl_8047AA80;
    r3 = r25 + 0xfb;
    r4 = r26 + 0x9b;
    r5 = 0x5;
    r3 = r0 + r3;
    memcpy();
    r0 = *(u32*)lbl_8047AA80;
    r3 = r29 + 0x14b;
    r4 = r24 + 0xeb;
    r5 = 0x4;
    r3 = r0 + r3;
    memcpy();
    r0 = *(u32*)lbl_8047AA80;
    r3 = r29 + 0x18b;
    r4 = r24 + 0x12b;
    r5 = 0x4;
    r3 = r0 + r3;
    memcpy();
    r5 = *(u32*)((u8*)r24 + 0x16C);
    r3 = r29 + 0x1cc;
    r4 = *(u32*)lbl_8047AA80;
    /* cmplwi r18, 0x0 */;
    r0 = r29 + 0x20c;
    /* stwx r5, r4, r3 */;
    r4 = *(u32*)((u8*)r24 + 0x1AC);
    r3 = *(u32*)lbl_8047AA80;
    /* stwx r4, r3, r0 */;
    if (/* eq */) goto .L_800D9028;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r22 + 0x26c;
    r4 = r23 + 0x20c;
    r5 = 0x14;
    r3 = r0 + r3;
    memcpy();
.L_800D9028:
    r29 = r29 + 0x4;
    r28 = r28 + 0x1;
    r27 = r27 + 0x3;
    r26 = r26 + 0x5;
    r25 = r25 + 0x5;
    r24 = r24 + 0x4;
    r23 = r23 + 0x14;
    r22 = r22 + 0x14;
    r21 = r21 + 0x1;
.L_800D904C:
    /* cmpw r21, r19 */;
    if (/* lt */) goto .L_800D8F04;
    r6 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    r5 = 0x10;
    r3 = r6 + 0x24c;
    r4 = r4 + 0x1ec;
    memcpy();
    /* cmpwi r20, 0x0 */;
    if (/* eq */) goto .L_800D9224;
    r6 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    r5 = r20 << 2;
    r3 = r6 + 0x3ad;
    r4 = r4 + 0x34d;
    memcpy();
    r6 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    r5 = 0x54;
    r3 = r6 + 0x3c0;
    r4 = r4 + 0x360;
    memcpy();
    goto .L_800D9224;
.L_800D90B4:
    r0 = *(u8*)((u8*)r3 + 0xB0);
    r21 = 0x0;
    /* cmplwi r0, 0x1 */;
    r0 = *(u8*)((u8*)r3 + 0xCC);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D90D0;
    r21 = 0x1;
.L_800D90D0:
    r0 = *(u8*)((u8*)r3 + 0xE8);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D90E0;
    r21 = 0x2;
.L_800D90E0:
    r0 = *(u8*)((u8*)r3 + 0x104);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D90F0;
    r21 = 0x3;
.L_800D90F0:
    r0 = *(u8*)((u8*)r3 + 0x120);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9100;
    r21 = 0x4;
.L_800D9100:
    r0 = *(u8*)((u8*)r3 + 0x13C);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9110;
    r21 = 0x5;
.L_800D9110:
    r0 = *(u8*)((u8*)r3 + 0x158);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9120;
    r21 = 0x6;
.L_800D9120:
    r0 = *(u8*)((u8*)r3 + 0x174);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9130;
    r21 = 0x7;
.L_800D9130:
    r7 = r21 + 0x1;
    r24 = 0x0;
    *(u8*)((u8*)r6 + 0x79) = r7;
    r3 = (u32)lbl_80314404;
    r5 = (u32)lbl_80400B28;
    r4 = (u32)lbl_803144F0;
    r6 = *(u32*)lbl_8047AA80;
    r18 = (u32)lbl_80314404;
    r5 = (u32)lbl_80400B28;
    r22 = r24;
    r0 = *(u32*)((u8*)r6 + 0x414);
    r23 = r24;
    r19 = (u32)lbl_803144F0;
    r0 = r0 | 0x2;
    *(u32*)((u8*)r6 + 0x414) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x7A) = r7;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x3AC) = r24;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    r0 = *(u8*)((u8*)r5 + 0x34C);
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x3AC) = r0;
    goto .L_800D921C;
.L_800D91AC:
    r0 = *(u32*)lbl_8047AA80;
    r8 = r22 + 0x42e;
    r7 = r23 + 0x7b;
    r5 = lbl_80478AE0@sda21;
    r8 = r0 + r8;
    r3 = r24;
    r6 = *(u8*)((u8*)r8 + 0x0);
    r7 = r0 + r7;
    r4 = *(u8*)((u8*)r8 + 0x2);
    r0 = *(u8*)((u8*)r8 + 0x1);
    r6 = r6 << 2;
    r4 = r4 << 2;
    /* lwzx r5, r5, r6 */;
    r0 = r0 << 2;
    /* lwzx r4, r19, r4 */;
    /* lwzx r0, r18, r0 */;
    *(u8*)((u8*)r7 + 0x0) = r0;
    *(u8*)((u8*)r7 + 0x1) = r4;
    *(u8*)((u8*)r7 + 0x2) = r5;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r4 + 0x414) = r0;
    r4 = *(u8*)((u8*)r8 + 0x3);
    fn_800D963C();
    r22 = r22 + 0x4;
    r23 = r23 + 0x3;
    r24 = r24 + 0x1;
.L_800D921C:
    /* cmpw r24, r21 */;
    if (/* le */) goto .L_800D91AC;
.L_800D9224:
    fn_800D923C();
    /* lmw r18, 0x8(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x44);
    return;
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
    *(f64*)((u8*)r1 + 0x50) = f31;
    /* psq_st f31, 0x58(r1), 0, qr0 */;
    /* stmw r18, 0x18(r1) */;
    r7 = (u32)lbl_80400F30;
    r29 = r3;
    r28 = (u32)lbl_80400F30;
    r30 = r4;
    r31 = r6;
    r25 = r5;
    r24 = r29;
    r26 = r28 + 0x268;
    r22 = 0x0;
    r21 = 0x0;
    r0 = 0x0;
    r20 = 0x0;
    r19 = 0x0;
    goto .L_800DEF9C;
.L_800DE6D0:
    r3 = r21 & 0xFF;
    if (/* ne */) goto .L_800DE71C;
    r4 = *(u8*)((u8*)r25 + 0x0);
    /* cmpwi r4, 0x25 */;
    if (/* ne */) goto .L_800DE710;
    r3 = *(u8*)((u8*)r25 + 0x1);
    /* cmpwi r3, 0x25 */;
    if (/* ne */) goto .L_800DE704;
    r3 = 0x25;
    r25 = r25 + 0x1;
    *(u8*)((u8*)r24 + 0x0) = r3;
    r24 = r24 + 0x1;
    goto .L_800DEF7C;
.L_800DE704:
    r23 = r28 + 0x278;
    r21 = 0x1;
    goto .L_800DEF7C;
.L_800DE710:
    *(u8*)((u8*)r24 + 0x0) = r4;
    r24 = r24 + 0x1;
    goto .L_800DEF7C;
.L_800DE71C:
    r5 = *(u8*)((u8*)r25 + 0x0);
    r3 = (s8)r5;
    /* subi r4, r3, 0x58 */;
    /* cmplwi r4, 0x20 */;
    if (/* gt */) goto .L_800DED58;
    r3 = (u32)jumptable_8031540C;
    r4 = r4 << 2;
    r3 = (u32)jumptable_8031540C;
    /* lwzx r3, r3, r4 */;
    /* mtctr r3 */;
    /* indirect jump via ctr */;
    r3 = r31;
    r27 = r28 + 0x268;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)((u8*)r3 + 0x0);
    r3 = 0x0;
    r0 = 0x1;
    *(u8*)((u8*)r28 + 0x268) = r4;
    *(u8*)((u8*)r26 + 0x1) = r3;
    goto .L_800DED60;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r0 = *(u32*)((u8*)r3 + 0x0);
    r3 = r28 + 0x268;
    r8 = 0x0;
    /* cmpwi r0, 0x0 */;
    r9 = r0;
    if (/* ge */) goto .L_800DE79C;
    r8 = 0x1;
    r9 = -r0;
.L_800DE79C:
    r4 = (0x6666 << 16);
    r7 = *(u32*)lbl_80478AE8;
    r6 = r4 + 0x6667;
.L_800DE7A8:
    r0 = (s32)((s64)r6 * (s64)r9 >> 32);
    r4 = (s32)r0 >> 2;
    r5 = (u32)r4 >> 31;
    r0 = (s32)r0 >> 2;
    r4 = r4 + r5;
    r5 = r4 * 0xa;
    r4 = (u32)r0 >> 31;
    r5 = r9 - r5;
    /* add. r9, r0, r4 */;
    /* lbzx r0, r7, r5 */;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
    if (/* ne */) goto .L_800DE7A8;
    r0 = r8 & 0xFF;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DE7F4;
    r0 = 0x2d;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
.L_800DE7F4:
    r0 = 0x0;
    r18 = r28 + 0x268;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + 0x267;
    r4 = r4 + r28;
    goto .L_800DE84C;
.L_800DE814:
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
.L_800DE84C:
    /* cmplw r18, r4 */;
    if (/* lt */) goto .L_800DE814;
    r27 = r28 + 0x268;
    r0 = 0x1;
    goto .L_800DED60;
    r3 = r31;
    r4 = 0x3;
    __va_arg();
    f1 = *(f64*)((u8*)r3 + 0x0);
    r27 = r28 + 0x268;
    f0 = *(f32*)lbl_8047CAB0;
    f1 = (f32)f1;
    f31 = f1;
    /* fcmpo cr0, f1, f0 */;
    if (/* ge */) goto .L_800DE898;
    r0 = 0x2d;
    f31 = -f1;
    *(u8*)((u8*)r27 + 0x0) = r0;
    r27 = r27 + 0x1;
.L_800DE898:
    f0 = (f64)(s32)f31;
    r4 = r27;
    r0 = 0x0;
    *(f64*)((u8*)r1 + 0x8) = f0;
    r18 = *(u32*)((u8*)r1 + 0xC);
    /* cmpwi r18, 0x0 */;
    r5 = r18;
    if (/* ge */) goto .L_800DE8C0;
    r0 = 0x1;
    r5 = -r18;
.L_800DE8C0:
    r6 = (0x6666 << 16);
    r3 = *(u32*)lbl_80478AE8;
    r9 = r6 + 0x6667;
.L_800DE8CC:
    r6 = (s32)((s64)r9 * (s64)r5 >> 32);
    r7 = (s32)r6 >> 2;
    r8 = (u32)r7 >> 31;
    r6 = (s32)r6 >> 2;
    r7 = r7 + r8;
    r8 = r7 * 0xa;
    r7 = (u32)r6 >> 31;
    r8 = r5 - r8;
    /* add. r5, r6, r7 */;
    /* lbzx r6, r3, r8 */;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r4 = r4 + 0x1;
    if (/* ne */) goto .L_800DE8CC;
    r0 = r0 & 0xFF;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DE918;
    r0 = 0x2d;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r4 = r4 + 0x1;
.L_800DE918:
    r0 = 0x0;
    r3 = r27;
    *(u8*)((u8*)r4 + 0x0) = r0;
    strlen();
    r4 = r3 + r27;
    /* subi r4, r4, 0x1 */;
    goto .L_800DE96C;
.L_800DE934:
    r3 = *(u8*)((u8*)r27 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r27 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r27 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r27 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r27 + 0x0) = r0;
    r27 = r27 + 0x1;
.L_800DE96C:
    /* cmplw r27, r4 */;
    if (/* lt */) goto .L_800DE934;
    /* xoris r3, r18, 0x8000 */;
    r0 = (0x4330 << 16);
    r4 = 0x0;
    f2 = *(f64*)lbl_8047CAC0;
    f3 = *(f32*)lbl_8047CAB4;
    f1 = *(f64*)((u8*)r1 + 0x8);
    f0 = *(f32*)lbl_8047CAB8;
    f1 = f1 - f2;
    f31 = f31 - f1;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x1;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x2;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x3;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x4;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x5;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x6;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x7;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x8;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x9;
    /* fcmpo cr0, f3, f31 */;
    if (/* lt */) goto .L_800DEA3C;
    r4 = 0xa;
.L_800DEA3C:
    /* cmpwi r4, 0x0 */;
    r18 = r28 + 0x258;
    r3 = 0x30;
    if (/* eq */) goto .L_800DEA98;
    /* srwi. r0, r4, 3 */;
    /* mtctr r0 */;
    if (/* eq */) goto .L_800DEA88;
.L_800DEA58:
    *(u8*)((u8*)r18 + 0x0) = r3;
    *(u8*)((u8*)r18 + 0x1) = r3;
    *(u8*)((u8*)r18 + 0x2) = r3;
    *(u8*)((u8*)r18 + 0x3) = r3;
    *(u8*)((u8*)r18 + 0x4) = r3;
    *(u8*)((u8*)r18 + 0x5) = r3;
    *(u8*)((u8*)r18 + 0x6) = r3;
    *(u8*)((u8*)r18 + 0x7) = r3;
    r18 = r18 + 0x8;
    if (--ctr != 0) goto .L_800DEA58;
    r4 = r4 & 0x7;
    if (/* eq */) goto .L_800DEA98;
.L_800DEA88:
    /* mtctr r4 */;
.L_800DEA8C:
    *(u8*)((u8*)r18 + 0x0) = r3;
    r18 = r18 + 0x1;
    if (--ctr != 0) goto .L_800DEA8C;
.L_800DEA98:
    f0 = *(f32*)lbl_8047CABC;
    r3 = r18;
    r8 = 0x0;
    f31 = f31 * f0;
    f0 = (f64)(s32)f31;
    *(f64*)((u8*)r1 + 0x8) = f0;
    r9 = *(u32*)((u8*)r1 + 0xC);
    /* cmpwi r9, 0x0 */;
    if (/* ge */) goto .L_800DEAC4;
    r8 = 0x1;
    r9 = -r9;
.L_800DEAC4:
    r4 = (0x6666 << 16);
    r7 = *(u32*)lbl_80478AE8;
    r6 = r4 + 0x6667;
.L_800DEAD0:
    r0 = (s32)((s64)r6 * (s64)r9 >> 32);
    r4 = (s32)r0 >> 2;
    r5 = (u32)r4 >> 31;
    r0 = (s32)r0 >> 2;
    r4 = r4 + r5;
    r5 = r4 * 0xa;
    r4 = (u32)r0 >> 31;
    r5 = r9 - r5;
    /* add. r9, r0, r4 */;
    /* lbzx r0, r7, r5 */;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
    if (/* ne */) goto .L_800DEAD0;
    r0 = r8 & 0xFF;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DEB1C;
    r0 = 0x2d;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
.L_800DEB1C:
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + r18;
    /* subi r4, r4, 0x1 */;
    goto .L_800DEB70;
.L_800DEB38:
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
.L_800DEB70:
    /* cmplw r18, r4 */;
    if (/* lt */) goto .L_800DEB38;
    r3 = r28 + 0x258;
    strlen();
    /* cmpwi r3, 0xa */;
    r5 = r28 + 0x258;
    r5 = r5 + r3;
    r3 = 0xa - r3;
    r4 = 0x30;
    if (/* ge */) goto .L_800DEBE4;
    /* srwi. r0, r3, 3 */;
    /* mtctr r0 */;
    if (/* eq */) goto .L_800DEBD4;
.L_800DEBA4:
    *(u8*)((u8*)r5 + 0x0) = r4;
    *(u8*)((u8*)r5 + 0x1) = r4;
    *(u8*)((u8*)r5 + 0x2) = r4;
    *(u8*)((u8*)r5 + 0x3) = r4;
    *(u8*)((u8*)r5 + 0x4) = r4;
    *(u8*)((u8*)r5 + 0x5) = r4;
    *(u8*)((u8*)r5 + 0x6) = r4;
    *(u8*)((u8*)r5 + 0x7) = r4;
    r5 = r5 + 0x8;
    if (--ctr != 0) goto .L_800DEBA4;
    r3 = r3 & 0x7;
    if (/* eq */) goto .L_800DEBE4;
.L_800DEBD4:
    /* mtctr r3 */;
.L_800DEBD8:
    *(u8*)((u8*)r5 + 0x0) = r4;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto .L_800DEBD8;
.L_800DEBE4:
    r0 = 0x0;
    r27 = r28 + 0x268;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r0 = 0x1;
    goto .L_800DED60;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r27 = *(u32*)((u8*)r3 + 0x0);
    r0 = 0x1;
    goto .L_800DED60;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r0 = *(u8*)((u8*)r25 + 0x0);
    r5 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x58 */;
    if (/* ne */) goto .L_800DECB0;
    r3 = *(u32*)lbl_80478AE8;
    r4 = r28 + 0x268;
.L_800DEC34:
    r0 = r5 & 0xF;
    /* srwi. r5, r5, 4 */;
    /* lbzx r0, r3, r0 */;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r4 = r4 + 0x1;
    if (/* ne */) goto .L_800DEC34;
    r0 = 0x0;
    r18 = r28 + 0x268;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + 0x267;
    r4 = r4 + r28;
    goto .L_800DECA4;
.L_800DEC6C:
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
.L_800DECA4:
    /* cmplw r18, r4 */;
    if (/* lt */) goto .L_800DEC6C;
    goto .L_800DED4C;
.L_800DECB0:
    r4 = *(u32*)lbl_80478AE8;
    r6 = r5;
    r5 = r28 + 0x268;
.L_800DECBC:
    r0 = r6 & 0xF;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = *(u8*)((u8*)r5 + 0x0);
    r0 = (s8)r3;
    /* cmpwi r0, 0x41 */;
    if (/* lt */) goto .L_800DECE0;
    r0 = r3 + 0x20;
    *(u8*)((u8*)r5 + 0x0) = r0;
.L_800DECE0:
    /* srwi. r6, r6, 4 */;
    r5 = r5 + 0x1;
    if (/* ne */) goto .L_800DECBC;
    r0 = 0x0;
    r18 = r28 + 0x268;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + 0x267;
    r4 = r4 + r28;
    goto .L_800DED44;
.L_800DED0C:
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
.L_800DED44:
    /* cmplw r18, r4 */;
    if (/* lt */) goto .L_800DED0C;
.L_800DED4C:
    r27 = r28 + 0x268;
    r0 = 0x1;
    goto .L_800DED60;
.L_800DED58:
    *(u8*)((u8*)r23 + 0x0) = r5;
    r23 = r23 + 0x1;
.L_800DED60:
    r3 = r0 & 0xFF;
    /* cmplwi r3, 0x1 */;
    if (/* ne */) goto .L_800DEF7C;
    r0 = 0x0;
    *(u8*)((u8*)r23 + 0x0) = r0;
    r23 = r28 + 0x278;
    r0 = *(u8*)((u8*)r28 + 0x278);
    /* cmpwi r0, 0x2d */;
    if (/* ne */) goto .L_800DED8C;
    r20 = 0x1;
    r23 = r23 + 0x1;
.L_800DED8C:
    r0 = *(u8*)((u8*)r23 + 0x0);
    /* cmpwi r0, 0x30 */;
    if (/* ne */) goto .L_800DEDA0;
    r19 = 0x1;
    r23 = r23 + 0x1;
.L_800DEDA0:
    r3 = r23;
    r18 = 0x0;
    goto .L_800DEDD8;
.L_800DEDAC:
    r0 = (s8)r4;
    /* cmpwi r0, 0x30 */;
    if (/* lt */) goto .L_800DEDE4;
    /* cmpwi r0, 0x39 */;
    if (/* gt */) goto .L_800DEDE4;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r18 = r18 * 0xa;
    r3 = r3 + 0x1;
    r0 = (s8)r0;
    r18 = r0 + r18;
    /* subi r18, r18, 0x30 */;
.L_800DEDD8:
    r4 = *(u8*)((u8*)r3 + 0x0);
    r0 = (s8)r4;
    if (/* ne */) goto .L_800DEDAC;
.L_800DEDE4:
    r3 = r27;
    strlen();
    /* cmpw r18, r3 */;
    if (/* le */) goto .L_800DEE58;
    r0 = r20 & 0xFF;
    r18 = r18 - r3;
    if (/* ne */) goto .L_800DEE58;
    r5 = r19 & 0xFF;
    r4 = 0x30;
    r3 = 0x20;
    goto .L_800DEE2C;
.L_800DEE10:
    /* cmplwi r5, 0x0 */;
    if (/* eq */) goto .L_800DEE24;
    *(u8*)((u8*)r24 + 0x0) = r4;
    r24 = r24 + 0x1;
    goto .L_800DEE2C;
.L_800DEE24:
    *(u8*)((u8*)r24 + 0x0) = r3;
    r24 = r24 + 0x1;
.L_800DEE2C:
    /* cmpwi r18, 0x0 */;
    /* subi r18, r18, 0x1 */;
    if (/* eq */) goto .L_800DEE58;
    r0 = r24 - r29;
    /* cmplw r0, r30 */;
    if (/* lt */) goto .L_800DEE10;
    goto .L_800DEE58;
.L_800DEE48:
    r0 = *(u8*)((u8*)r27 + 0x0);
    r27 = r27 + 0x1;
    *(u8*)((u8*)r24 + 0x0) = r0;
    r24 = r24 + 0x1;
.L_800DEE58:
    r0 = *(u8*)((u8*)r27 + 0x0);
    r0 = (s8)r0;
    if (/* eq */) goto .L_800DEE70;
    r0 = r24 - r29;
    /* cmplw r0, r30 */;
    if (/* lt */) goto .L_800DEE48;
.L_800DEE70:
    r0 = *(u8*)((u8*)r25 + 0x0);
    /* cmpwi r0, 0x66 */;
    if (/* ne */) goto .L_800DEF38;
    r27 = r28 + 0x258;
    r3 = r27;
    strlen();
    r19 = r3;
    r3 = r23;
    r4 = 0x2e;
    strchr();
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800DEF00;
    r4 = r3 + 0x1;
    r3 = 0x0;
    goto .L_800DEED8;
.L_800DEEAC:
    r0 = (s8)r5;
    /* cmpwi r0, 0x30 */;
    if (/* lt */) goto .L_800DEEE4;
    /* cmpwi r0, 0x39 */;
    if (/* gt */) goto .L_800DEEE4;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = r3 * 0xa;
    r4 = r4 + 0x1;
    r0 = (s8)r0;
    r3 = r0 + r3;
    /* subi r3, r3, 0x30 */;
.L_800DEED8:
    r5 = *(u8*)((u8*)r4 + 0x0);
    r0 = (s8)r5;
    if (/* ne */) goto .L_800DEEAC;
.L_800DEEE4:
    /* cmpwi r3, 0x0 */;
    r18 = r3;
    if (/* le */) goto .L_800DEF00;
    /* cmpw r19, r3 */;
    if (/* le */) goto .L_800DEF00;
    r0 = 0x0;
    /* stbx r0, r27, r3 */;
.L_800DEF00:
    r0 = 0x2e;
    *(u8*)((u8*)r24 + 0x0) = r0;
    r24 = r24 + 0x1;
    goto .L_800DEF20;
.L_800DEF10:
    r0 = *(u8*)((u8*)r27 + 0x0);
    r27 = r27 + 0x1;
    *(u8*)((u8*)r24 + 0x0) = r0;
    r24 = r24 + 0x1;
.L_800DEF20:
    r0 = *(u8*)((u8*)r27 + 0x0);
    r0 = (s8)r0;
    if (/* eq */) goto .L_800DEF38;
    r0 = r24 - r29;
    /* cmplw r0, r30 */;
    if (/* lt */) goto .L_800DEF10;
.L_800DEF38:
    r0 = r20 & 0xFF;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DEF6C;
    r3 = 0x20;
    goto .L_800DEF54;
.L_800DEF4C:
    *(u8*)((u8*)r24 + 0x0) = r3;
    r24 = r24 + 0x1;
.L_800DEF54:
    /* cmpwi r18, 0x0 */;
    /* subi r18, r18, 0x1 */;
    if (/* eq */) goto .L_800DEF6C;
    r0 = r24 - r29;
    /* cmplw r0, r30 */;
    if (/* lt */) goto .L_800DEF4C;
.L_800DEF6C:
    r20 = 0x0;
    r19 = 0x0;
    r21 = 0x0;
    r0 = 0x0;
.L_800DEF7C:
    r3 = *(u8*)((u8*)r25 + 0x0);
    r3 = (s8)r3;
    if (/* eq */) goto .L_800DEF94;
    r3 = r24 - r29;
    /* cmplw r3, r30 */;
    if (/* lt */) goto .L_800DEF98;
.L_800DEF94:
    r22 = 0x1;
.L_800DEF98:
    r25 = r25 + 0x1;
.L_800DEF9C:
    r3 = r22 & 0xFF;
    if (/* eq */) goto .L_800DE6D0;
    r0 = 0x0;
    *(u8*)((u8*)r24 + 0x0) = r0;
    /* psq_l f31, 0x58(r1), 0, qr0 */;
    f31 = *(f64*)((u8*)r1 + 0x50);
    /* lmw r18, 0x18(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x64);
    return;
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
    r4 = 0x0;
    /* stmw r22, 0x18(r1) */;
    r5 = *(u32*)lbl_8047AB30;
    r3 = r5;
    goto .L_800E1578;
.L_800E1564:
    r0 = *(u32*)((u8*)r3 + 0x8);
    /* cmplw r0, r4 */;
    if (/* le */) goto .L_800E1574;
    r4 = r0;
.L_800E1574:
    r3 = *(u32*)((u8*)r3 + 0x4);
.L_800E1578:
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800E1564;
    /* cmplwi r5, 0x0 */;
    r28 = r5;
    if (/* eq */) goto .L_800E1FEC;
    r0 = *(u32*)((u8*)r5 + 0x4);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E1FEC;
    goto .L_800E1FE4;
.L_800E159C:
    r7 = *(u32*)((u8*)r28 + 0x8);
    r6 = *(u32*)lbl_8047AB38;
    r8 = r28 + r7;
    /* cmplw r8, r6 */;
    if (/* eq */) goto .L_800E1FE0;
    r5 = *(u32*)lbl_8047AB34;
    r0 = r5 + 0x10;
    r3 = r5;
    r0 = r0 - r6;
    r0 = (u32)r0 >> 4;
    /* mtctr r0 */;
    /* cmplw r5, r6 */;
    if (/* lt */) goto .L_800E15F4;
.L_800E15D0:
    r0 = *(u16*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E15EC;
    r0 = *(u32*)((u8*)r3 + 0x4);
    /* cmplw r0, r8 */;
    if (/* ne */) goto .L_800E15EC;
    goto .L_800E15F8;
.L_800E15EC:
    /* subi r3, r3, 0x10 */;
    if (--ctr != 0) goto .L_800E15D0;
.L_800E15F4:
    r3 = 0x0;
.L_800E15F8:
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800E1618;
    r3 = (u32)lbl_80270BB8;
    r3 = (u32)lbl_80270BB8;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto .L_800E2018;
.L_800E1618:
    r0 = *(u16*)((u8*)r3 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800E1898;
    r0 = *(u16*)((u8*)r3 + 0xC);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800E1898;
    r5 = *(u32*)lbl_8047AB30;
    r0 = 0x0;
    /* cmplw r5, r28 */;
    if (/* ne */) goto .L_800E1644;
    r0 = 0x1;
.L_800E1644:
    r6 = *(u32*)((u8*)r28 + 0x0);
    r5 = *(u32*)((u8*)r3 + 0x8);
    /* cmplwi r6, 0x0 */;
    r12 = *(u32*)((u8*)r28 + 0x4);
    r5 = r28 + r5;
    if (/* eq */) goto .L_800E1660;
    *(u32*)((u8*)r6 + 0x4) = r5;
.L_800E1660:
    r8 = *(u32*)((u8*)r28 + 0x4);
    /* cmplwi r8, 0x0 */;
    if (/* eq */) goto .L_800E1670;
    *(u32*)((u8*)r8 + 0x0) = r5;
.L_800E1670:
    r22 = *(u32*)((u8*)r3 + 0x4);
    r10 = *(u32*)((u8*)r3 + 0x8);
    /* subf. r9, r28, r22 */;
    r8 = -r9;
    if (/* eq */) goto .L_800E1688;
    r8 = r9;
.L_800E1688:
    /* cmplwi r8, 0x4 */;
    if (/* lt */) goto .L_800E17A4;
    /* srwi. r8, r10, 2 */;
    r11 = r28;
    r10 = r10 & 0x3;
    r9 = r8;
    if (/* eq */) goto .L_800E171C;
    /* srwi. r8, r8, 3 */;
    /* mtctr r8 */;
    if (/* eq */) goto .L_800E1704;
.L_800E16B0:
    r8 = *(u32*)((u8*)r22 + 0x0);
    *(u32*)((u8*)r11 + 0x0) = r8;
    r8 = *(u32*)((u8*)r22 + 0x4);
    *(u32*)((u8*)r11 + 0x4) = r8;
    r8 = *(u32*)((u8*)r22 + 0x8);
    *(u32*)((u8*)r11 + 0x8) = r8;
    r8 = *(u32*)((u8*)r22 + 0xC);
    *(u32*)((u8*)r11 + 0xC) = r8;
    r8 = *(u32*)((u8*)r22 + 0x10);
    *(u32*)((u8*)r11 + 0x10) = r8;
    r8 = *(u32*)((u8*)r22 + 0x14);
    *(u32*)((u8*)r11 + 0x14) = r8;
    r8 = *(u32*)((u8*)r22 + 0x18);
    *(u32*)((u8*)r11 + 0x18) = r8;
    r8 = *(u32*)((u8*)r22 + 0x1C);
    r22 = r22 + 0x20;
    *(u32*)((u8*)r11 + 0x1C) = r8;
    r11 = r11 + 0x20;
    if (--ctr != 0) goto .L_800E16B0;
    r9 = r9 & 0x7;
    if (/* eq */) goto .L_800E171C;
.L_800E1704:
    /* mtctr r9 */;
.L_800E1708:
    r8 = *(u32*)((u8*)r22 + 0x0);
    r22 = r22 + 0x4;
    *(u32*)((u8*)r11 + 0x0) = r8;
    r11 = r11 + 0x4;
    if (--ctr != 0) goto .L_800E1708;
.L_800E171C:
    /* cmplwi r10, 0x0 */;
    r9 = r10;
    if (/* eq */) goto .L_800E182C;
    /* srwi. r8, r10, 3 */;
    /* mtctr r8 */;
    if (/* eq */) goto .L_800E1788;
.L_800E1734:
    r8 = *(u8*)((u8*)r22 + 0x0);
    *(u8*)((u8*)r11 + 0x0) = r8;
    r8 = *(u8*)((u8*)r22 + 0x1);
    *(u8*)((u8*)r11 + 0x1) = r8;
    r8 = *(u8*)((u8*)r22 + 0x2);
    *(u8*)((u8*)r11 + 0x2) = r8;
    r8 = *(u8*)((u8*)r22 + 0x3);
    *(u8*)((u8*)r11 + 0x3) = r8;
    r8 = *(u8*)((u8*)r22 + 0x4);
    *(u8*)((u8*)r11 + 0x4) = r8;
    r8 = *(u8*)((u8*)r22 + 0x5);
    *(u8*)((u8*)r11 + 0x5) = r8;
    r8 = *(u8*)((u8*)r22 + 0x6);
    *(u8*)((u8*)r11 + 0x6) = r8;
    r8 = *(u8*)((u8*)r22 + 0x7);
    r22 = r22 + 0x8;
    *(u8*)((u8*)r11 + 0x7) = r8;
    r11 = r11 + 0x8;
    if (--ctr != 0) goto .L_800E1734;
    r9 = r9 & 0x7;
    if (/* eq */) goto .L_800E182C;
.L_800E1788:
    /* mtctr r9 */;
.L_800E178C:
    r8 = *(u8*)((u8*)r22 + 0x0);
    r22 = r22 + 0x1;
    *(u8*)((u8*)r11 + 0x0) = r8;
    r11 = r11 + 0x1;
    if (--ctr != 0) goto .L_800E178C;
    goto .L_800E182C;
.L_800E17A4:
    /* cmplwi r10, 0x0 */;
    r11 = r28;
    r9 = r10;
    if (/* eq */) goto .L_800E182C;
    /* srwi. r8, r10, 3 */;
    /* mtctr r8 */;
    if (/* eq */) goto .L_800E1814;
.L_800E17C0:
    r8 = *(u8*)((u8*)r22 + 0x0);
    *(u8*)((u8*)r11 + 0x0) = r8;
    r8 = *(u8*)((u8*)r22 + 0x1);
    *(u8*)((u8*)r11 + 0x1) = r8;
    r8 = *(u8*)((u8*)r22 + 0x2);
    *(u8*)((u8*)r11 + 0x2) = r8;
    r8 = *(u8*)((u8*)r22 + 0x3);
    *(u8*)((u8*)r11 + 0x3) = r8;
    r8 = *(u8*)((u8*)r22 + 0x4);
    *(u8*)((u8*)r11 + 0x4) = r8;
    r8 = *(u8*)((u8*)r22 + 0x5);
    *(u8*)((u8*)r11 + 0x5) = r8;
    r8 = *(u8*)((u8*)r22 + 0x6);
    *(u8*)((u8*)r11 + 0x6) = r8;
    r8 = *(u8*)((u8*)r22 + 0x7);
    r22 = r22 + 0x8;
    *(u8*)((u8*)r11 + 0x7) = r8;
    r11 = r11 + 0x8;
    if (--ctr != 0) goto .L_800E17C0;
    r9 = r9 & 0x7;
    if (/* eq */) goto .L_800E182C;
.L_800E1814:
    /* mtctr r9 */;
.L_800E1818:
    r8 = *(u8*)((u8*)r22 + 0x0);
    r22 = r22 + 0x1;
    *(u8*)((u8*)r11 + 0x0) = r8;
    r11 = r11 + 0x1;
    if (--ctr != 0) goto .L_800E1818;
.L_800E182C:
    *(u32*)((u8*)r3 + 0x4) = r28;
    *(u32*)((u8*)r5 + 0x0) = r6;
    *(u32*)((u8*)r5 + 0x4) = r12;
    *(u32*)((u8*)r5 + 0x8) = r7;
    r7 = *(u32*)((u8*)r5 + 0x4);
    /* cmplwi r7, 0x0 */;
    if (/* eq */) goto .L_800E1884;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r3 = r5 + r6;
    /* cmplw r7, r3 */;
    if (/* ne */) goto .L_800E1884;
    r3 = *(u32*)((u8*)r7 + 0x8);
    r3 = r6 + r3;
    *(u32*)((u8*)r5 + 0x8) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = *(u32*)((u8*)r3 + 0x4);
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800E1878;
    *(u32*)((u8*)r3 + 0x0) = r5;
.L_800E1878:
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = *(u32*)((u8*)r3 + 0x4);
    *(u32*)((u8*)r5 + 0x4) = r3;
.L_800E1884:
    r0 = r0 & 0xFF;
    if (/* eq */) goto .L_800E1890;
    *(u32*)lbl_8047AB30 = r5;
.L_800E1890:
    r28 = *(u32*)lbl_8047AB30;
    goto .L_800E1FE4;
.L_800E1898:
    r3 = 0x0;
    r11 = r5;
    r8 = r1 + 0x8;
    r29 = 0x0;
    r10 = 0x0;
    goto .L_800E1A50;
.L_800E18C0:
    r0 = *(u16*)((u8*)r11 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E1A4C;
    r0 = *(u16*)((u8*)r11 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800E1A4C;
    r5 = *(u32*)((u8*)r11 + 0x8);
    r7 = *(u32*)((u8*)r28 + 0x8);
    /* cmplw r5, r7 */;
    if (/* gt */) goto .L_800E1A4C;
    r0 = *(u32*)((u8*)r11 + 0x4);
    /* cmplw r0, r28 */;
    if (/* le */) goto .L_800E1A4C;
    r0 = *(u16*)((u8*)r11 + 0xC);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800E1A4C;
    /* cmpwi r29, 0x4 */;
    if (/* ge */) goto .L_800E1930;
    r0 = r10 + r5;
    /* cmplw r0, r7 */;
    if (/* gt */) goto .L_800E1930;
    r5 = r1 + 0x8;
    r29 = r29 + 0x1;
    /* stwx r11, r5, r3 */;
    r3 = r3 + 0x4;
    r0 = *(u32*)((u8*)r11 + 0x8);
    r10 = r10 + r0;
    goto .L_800E1A4C;
.L_800E1930:
    r7 = r1 + 0x8;
    r31 = 0x0;
    goto .L_800E1A44;
.L_800E193C:
    r0 = r31 << 2;
    /* cmpwi r29, 0x0 */;
    /* lwzx r12, r7, r0 */;
    r9 = 0x0;
    r30 = 0x0;
    /* stwx r11, r7, r0 */;
    if (/* le */) goto .L_800E1A1C;
    /* cmpwi r29, 0x8 */;
    /* subi r22, r29, 0x8 */;
    if (/* le */) goto .L_800E19E8;
    r27 = r22 + 0x7;
    r5 = r8;
    r27 = (u32)r27 >> 3;
    /* mtctr r27 */;
    /* cmpwi r22, 0x0 */;
    if (/* le */) goto .L_800E19E8;
.L_800E197C:
    r22 = *(u32*)((u8*)r5 + 0x0);
    r30 = r30 + 0x8;
    r24 = *(u32*)((u8*)r5 + 0x4);
    r25 = *(u32*)((u8*)r22 + 0x8);
    r22 = *(u32*)((u8*)r5 + 0x8);
    r23 = *(u32*)((u8*)r5 + 0xC);
    r9 = r9 + r25;
    r26 = *(u32*)((u8*)r24 + 0x8);
    r24 = *(u32*)((u8*)r5 + 0x10);
    r25 = *(u32*)((u8*)r5 + 0x14);
    r9 = r9 + r26;
    r22 = *(u32*)((u8*)r22 + 0x8);
    r26 = *(u32*)((u8*)r5 + 0x18);
    r27 = *(u32*)((u8*)r5 + 0x1C);
    r9 = r9 + r22;
    r23 = *(u32*)((u8*)r23 + 0x8);
    r5 = r5 + 0x20;
    r24 = *(u32*)((u8*)r24 + 0x8);
    r9 = r9 + r23;
    r25 = *(u32*)((u8*)r25 + 0x8);
    r9 = r9 + r24;
    r26 = *(u32*)((u8*)r26 + 0x8);
    r9 = r9 + r25;
    r27 = *(u32*)((u8*)r27 + 0x8);
    r9 = r9 + r26;
    r9 = r9 + r27;
    if (--ctr != 0) goto .L_800E197C;
.L_800E19E8:
    r27 = r30 << 2;
    r22 = r1 + 0x8;
    r5 = r29 - r30;
    r22 = r22 + r27;
    /* mtctr r5 */;
    /* cmpw r30, r29 */;
    if (/* ge */) goto .L_800E1A1C;
.L_800E1A04:
    r5 = *(u32*)((u8*)r22 + 0x0);
    r22 = r22 + 0x4;
    r30 = r30 + 0x1;
    r5 = *(u32*)((u8*)r5 + 0x8);
    r9 = r9 + r5;
    if (--ctr != 0) goto .L_800E1A04;
.L_800E1A1C:
    /* cmplw r9, r10 */;
    if (/* le */) goto .L_800E1A3C;
    r5 = *(u32*)((u8*)r28 + 0x8);
    /* cmplw r9, r5 */;
    if (/* gt */) goto .L_800E1A3C;
    r10 = r9;
    r31 = r29;
    goto .L_800E1A40;
.L_800E1A3C:
    /* stwx r12, r7, r0 */;
.L_800E1A40:
    r31 = r31 + 0x1;
.L_800E1A44:
    /* cmpw r31, r29 */;
    if (/* lt */) goto .L_800E193C;
.L_800E1A4C:
    /* subi r11, r11, 0x10 */;
.L_800E1A50:
    /* cmplw r11, r6 */;
    if (/* ge */) goto .L_800E18C0;
    /* cmpwi r29, 0x0 */;
    if (/* le */) goto .L_800E1FE0;
    r0 = *(u32*)lbl_8047AB30;
    r6 = *(u32*)((u8*)r28 + 0x0);
    /* cmplw r28, r0 */;
    r7 = *(u32*)((u8*)r28 + 0x4);
    r0 = *(u32*)((u8*)r28 + 0x8);
    if (/* ne */) goto .L_800E1A7C;
    *(u32*)lbl_8047AB30 = r7;
.L_800E1A7C:
    /* cmplwi r6, 0x0 */;
    if (/* eq */) goto .L_800E1A88;
    *(u32*)((u8*)r6 + 0x4) = r7;
.L_800E1A88:
    /* cmplwi r7, 0x0 */;
    if (/* eq */) goto .L_800E1A94;
    *(u32*)((u8*)r7 + 0x0) = r6;
.L_800E1A94:
    r3 = r28;
    r5 = 0x0;
    goto .L_800E1D94;
.L_800E1AA0:
    r10 = *(u32*)((u8*)r8 + 0x0);
    r9 = *(u32*)((u8*)r10 + 0x4);
    r12 = *(u32*)((u8*)r10 + 0x8);
    /* subf. r11, r3, r9 */;
    r10 = -r11;
    if (/* eq */) goto .L_800E1ABC;
    r10 = r11;
.L_800E1ABC:
    /* cmplwi r10, 0x4 */;
    if (/* lt */) goto .L_800E1BDC;
    /* srwi. r10, r12, 2 */;
    r23 = r9;
    r22 = r3;
    r12 = r12 & 0x3;
    r11 = r10;
    if (/* eq */) goto .L_800E1B54;
    /* srwi. r10, r10, 3 */;
    /* mtctr r10 */;
    if (/* eq */) goto .L_800E1B3C;
.L_800E1AE8:
    r10 = *(u32*)((u8*)r23 + 0x0);
    *(u32*)((u8*)r22 + 0x0) = r10;
    r10 = *(u32*)((u8*)r23 + 0x4);
    *(u32*)((u8*)r22 + 0x4) = r10;
    r10 = *(u32*)((u8*)r23 + 0x8);
    *(u32*)((u8*)r22 + 0x8) = r10;
    r10 = *(u32*)((u8*)r23 + 0xC);
    *(u32*)((u8*)r22 + 0xC) = r10;
    r10 = *(u32*)((u8*)r23 + 0x10);
    *(u32*)((u8*)r22 + 0x10) = r10;
    r10 = *(u32*)((u8*)r23 + 0x14);
    *(u32*)((u8*)r22 + 0x14) = r10;
    r10 = *(u32*)((u8*)r23 + 0x18);
    *(u32*)((u8*)r22 + 0x18) = r10;
    r10 = *(u32*)((u8*)r23 + 0x1C);
    r23 = r23 + 0x20;
    *(u32*)((u8*)r22 + 0x1C) = r10;
    r22 = r22 + 0x20;
    if (--ctr != 0) goto .L_800E1AE8;
    r11 = r11 & 0x7;
    if (/* eq */) goto .L_800E1B54;
.L_800E1B3C:
    /* mtctr r11 */;
.L_800E1B40:
    r10 = *(u32*)((u8*)r23 + 0x0);
    r23 = r23 + 0x4;
    *(u32*)((u8*)r22 + 0x0) = r10;
    r22 = r22 + 0x4;
    if (--ctr != 0) goto .L_800E1B40;
.L_800E1B54:
    /* cmplwi r12, 0x0 */;
    r11 = r12;
    if (/* eq */) goto .L_800E1C68;
    /* srwi. r10, r12, 3 */;
    /* mtctr r10 */;
    if (/* eq */) goto .L_800E1BC0;
.L_800E1B6C:
    r10 = *(u8*)((u8*)r23 + 0x0);
    *(u8*)((u8*)r22 + 0x0) = r10;
    r10 = *(u8*)((u8*)r23 + 0x1);
    *(u8*)((u8*)r22 + 0x1) = r10;
    r10 = *(u8*)((u8*)r23 + 0x2);
    *(u8*)((u8*)r22 + 0x2) = r10;
    r10 = *(u8*)((u8*)r23 + 0x3);
    *(u8*)((u8*)r22 + 0x3) = r10;
    r10 = *(u8*)((u8*)r23 + 0x4);
    *(u8*)((u8*)r22 + 0x4) = r10;
    r10 = *(u8*)((u8*)r23 + 0x5);
    *(u8*)((u8*)r22 + 0x5) = r10;
    r10 = *(u8*)((u8*)r23 + 0x6);
    *(u8*)((u8*)r22 + 0x6) = r10;
    r10 = *(u8*)((u8*)r23 + 0x7);
    r23 = r23 + 0x8;
    *(u8*)((u8*)r22 + 0x7) = r10;
    r22 = r22 + 0x8;
    if (--ctr != 0) goto .L_800E1B6C;
    r11 = r11 & 0x7;
    if (/* eq */) goto .L_800E1C68;
.L_800E1BC0:
    /* mtctr r11 */;
.L_800E1BC4:
    r10 = *(u8*)((u8*)r23 + 0x0);
    r23 = r23 + 0x1;
    *(u8*)((u8*)r22 + 0x0) = r10;
    r22 = r22 + 0x1;
    if (--ctr != 0) goto .L_800E1BC4;
    goto .L_800E1C68;
.L_800E1BDC:
    /* cmplwi r12, 0x0 */;
    r22 = r9;
    r23 = r3;
    r11 = r12;
    if (/* eq */) goto .L_800E1C68;
    /* srwi. r10, r12, 3 */;
    /* mtctr r10 */;
    if (/* eq */) goto .L_800E1C50;
.L_800E1BFC:
    r10 = *(u8*)((u8*)r22 + 0x0);
    *(u8*)((u8*)r23 + 0x0) = r10;
    r10 = *(u8*)((u8*)r22 + 0x1);
    *(u8*)((u8*)r23 + 0x1) = r10;
    r10 = *(u8*)((u8*)r22 + 0x2);
    *(u8*)((u8*)r23 + 0x2) = r10;
    r10 = *(u8*)((u8*)r22 + 0x3);
    *(u8*)((u8*)r23 + 0x3) = r10;
    r10 = *(u8*)((u8*)r22 + 0x4);
    *(u8*)((u8*)r23 + 0x4) = r10;
    r10 = *(u8*)((u8*)r22 + 0x5);
    *(u8*)((u8*)r23 + 0x5) = r10;
    r10 = *(u8*)((u8*)r22 + 0x6);
    *(u8*)((u8*)r23 + 0x6) = r10;
    r10 = *(u8*)((u8*)r22 + 0x7);
    r22 = r22 + 0x8;
    *(u8*)((u8*)r23 + 0x7) = r10;
    r23 = r23 + 0x8;
    if (--ctr != 0) goto .L_800E1BFC;
    r11 = r11 & 0x7;
    if (/* eq */) goto .L_800E1C68;
.L_800E1C50:
    /* mtctr r11 */;
.L_800E1C54:
    r10 = *(u8*)((u8*)r22 + 0x0);
    r22 = r22 + 0x1;
    *(u8*)((u8*)r23 + 0x0) = r10;
    r23 = r23 + 0x1;
    if (--ctr != 0) goto .L_800E1C54;
.L_800E1C68:
    r10 = *(u32*)((u8*)r8 + 0x0);
    r11 = 0x0;
    *(u32*)((u8*)r10 + 0x4) = r3;
    r10 = *(u32*)((u8*)r8 + 0x0);
    r12 = *(u32*)lbl_8047AB30;
    r10 = *(u32*)((u8*)r10 + 0x8);
    r3 = r3 + r10;
    r0 = r0 - r10;
    goto .L_800E1C94;
.L_800E1C8C:
    r11 = r12;
    r12 = *(u32*)((u8*)r12 + 0x4);
.L_800E1C94:
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800E1CA4;
    /* cmplw r12, r9 */;
    if (/* lt */) goto .L_800E1C8C;
.L_800E1CA4:
    /* cmplwi r11, 0x0 */;
    if (/* eq */) goto .L_800E1CBC;
    *(u32*)((u8*)r9 + 0x0) = r11;
    r10 = *(u32*)((u8*)r11 + 0x4);
    *(u32*)((u8*)r9 + 0x4) = r10;
    goto .L_800E1CD0;
.L_800E1CBC:
    r10 = 0x0;
    *(u32*)((u8*)r9 + 0x0) = r10;
    r10 = *(u32*)lbl_8047AB30;
    *(u32*)((u8*)r9 + 0x4) = r10;
    *(u32*)lbl_8047AB30 = r9;
.L_800E1CD0:
    r10 = *(u32*)((u8*)r8 + 0x0);
    r10 = *(u32*)((u8*)r10 + 0x8);
    *(u32*)((u8*)r9 + 0x8) = r10;
    r10 = *(u32*)((u8*)r9 + 0x0);
    /* cmplwi r10, 0x0 */;
    if (/* eq */) goto .L_800E1CEC;
    *(u32*)((u8*)r10 + 0x4) = r9;
.L_800E1CEC:
    r10 = *(u32*)((u8*)r9 + 0x4);
    /* cmplwi r10, 0x0 */;
    if (/* eq */) goto .L_800E1CFC;
    *(u32*)((u8*)r10 + 0x0) = r9;
.L_800E1CFC:
    r12 = *(u32*)((u8*)r9 + 0x4);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800E1D44;
    r11 = *(u32*)((u8*)r9 + 0x8);
    r10 = r9 + r11;
    /* cmplw r12, r10 */;
    if (/* ne */) goto .L_800E1D44;
    r10 = *(u32*)((u8*)r12 + 0x8);
    r10 = r11 + r10;
    *(u32*)((u8*)r9 + 0x8) = r10;
    r10 = *(u32*)((u8*)r9 + 0x4);
    r10 = *(u32*)((u8*)r10 + 0x4);
    /* cmplwi r10, 0x0 */;
    if (/* eq */) goto .L_800E1D38;
    *(u32*)((u8*)r10 + 0x0) = r9;
.L_800E1D38:
    r10 = *(u32*)((u8*)r9 + 0x4);
    r10 = *(u32*)((u8*)r10 + 0x4);
    *(u32*)((u8*)r9 + 0x4) = r10;
.L_800E1D44:
    r12 = *(u32*)((u8*)r9 + 0x0);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800E1D8C;
    r11 = *(u32*)((u8*)r12 + 0x8);
    r10 = r12 + r11;
    /* cmplw r9, r10 */;
    if (/* ne */) goto .L_800E1D8C;
    r10 = *(u32*)((u8*)r9 + 0x8);
    r10 = r11 + r10;
    *(u32*)((u8*)r12 + 0x8) = r10;
    r11 = *(u32*)((u8*)r9 + 0x4);
    /* cmplwi r11, 0x0 */;
    if (/* eq */) goto .L_800E1D80;
    r10 = *(u32*)((u8*)r9 + 0x0);
    *(u32*)((u8*)r11 + 0x0) = r10;
.L_800E1D80:
    r10 = *(u32*)((u8*)r9 + 0x4);
    r9 = *(u32*)((u8*)r9 + 0x0);
    *(u32*)((u8*)r9 + 0x4) = r10;
.L_800E1D8C:
    r8 = r8 + 0x4;
    r5 = r5 + 0x1;
.L_800E1D94:
    /* cmpw r5, r29 */;
    if (/* lt */) goto .L_800E1AA0;
    /* cmplwi r0, 0xc */;
    if (/* lt */) goto .L_800E1E6C;
    *(u32*)((u8*)r3 + 0x0) = r6;
    *(u32*)((u8*)r3 + 0x4) = r7;
    *(u32*)((u8*)r3 + 0x8) = r0;
    r5 = *(u32*)((u8*)r3 + 0x0);
    /* cmplwi r5, 0x0 */;
    if (/* eq */) goto .L_800E1DC4;
    *(u32*)((u8*)r5 + 0x4) = r3;
    goto .L_800E1DC8;
.L_800E1DC4:
    *(u32*)lbl_8047AB30 = r3;
.L_800E1DC8:
    r5 = *(u32*)((u8*)r3 + 0x4);
    /* cmplwi r5, 0x0 */;
    if (/* eq */) goto .L_800E1DD8;
    *(u32*)((u8*)r5 + 0x0) = r3;
.L_800E1DD8:
    r6 = *(u32*)((u8*)r3 + 0x4);
    /* cmplwi r6, 0x0 */;
    if (/* eq */) goto .L_800E1E20;
    r5 = *(u32*)((u8*)r3 + 0x8);
    r0 = r3 + r5;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800E1E20;
    r0 = *(u32*)((u8*)r6 + 0x8);
    r0 = r5 + r0;
    *(u32*)((u8*)r3 + 0x8) = r0;
    r5 = *(u32*)((u8*)r3 + 0x4);
    r5 = *(u32*)((u8*)r5 + 0x4);
    /* cmplwi r5, 0x0 */;
    if (/* eq */) goto .L_800E1E14;
    *(u32*)((u8*)r5 + 0x0) = r3;
.L_800E1E14:
    r5 = *(u32*)((u8*)r3 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x4);
    *(u32*)((u8*)r3 + 0x4) = r0;
.L_800E1E20:
    r6 = *(u32*)((u8*)r3 + 0x0);
    /* cmplwi r6, 0x0 */;
    if (/* eq */) goto .L_800E1FD8;
    r5 = *(u32*)((u8*)r6 + 0x8);
    r0 = r6 + r5;
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800E1FD8;
    r0 = *(u32*)((u8*)r3 + 0x8);
    r0 = r5 + r0;
    *(u32*)((u8*)r6 + 0x8) = r0;
    r5 = *(u32*)((u8*)r3 + 0x4);
    /* cmplwi r5, 0x0 */;
    if (/* eq */) goto .L_800E1E5C;
    r0 = *(u32*)((u8*)r3 + 0x0);
    *(u32*)((u8*)r5 + 0x0) = r0;
.L_800E1E5C:
    r0 = *(u32*)((u8*)r3 + 0x4);
    r3 = *(u32*)((u8*)r3 + 0x0);
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto .L_800E1FD8;
.L_800E1E6C:
    r3 = r29 << 2;
    r5 = r1 + 0x8;
    /* subi r3, r3, 0x4 */;
    /* lwzx r8, r5, r3 */;
    r3 = *(u32*)((u8*)r8 + 0x8);
    r0 = r3 + r0;
    *(u32*)((u8*)r8 + 0x8) = r0;
    r0 = *(u8*)lbl_8047AB28;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E1FD8;
    r3 = *(u32*)((u8*)r8 + 0x4);
    r5 = 0x0;
    r7 = 0x3d94;
    *(u8*)((u8*)r3 + 0x0) = r5;
    *(u8*)((u8*)r3 + 0x1) = r5;
    *(u8*)((u8*)r3 + 0x2) = r5;
    *(u8*)((u8*)r3 + 0x3) = r5;
    r3 = *(u32*)((u8*)r8 + 0x8);
    r0 = *(u32*)((u8*)r8 + 0x4);
    /* subi r3, r3, 0x4 */;
    r3 = r0 + r3;
    *(u8*)((u8*)r3 + 0x0) = r5;
    *(u8*)((u8*)r3 + 0x1) = r5;
    *(u8*)((u8*)r3 + 0x2) = r5;
    *(u8*)((u8*)r3 + 0x3) = r5;
    r3 = *(u32*)((u8*)r8 + 0x8);
    r6 = *(u32*)((u8*)r8 + 0x4);
    /* srwi. r0, r3, 1 */;
    r5 = r3 & 0x1;
    r3 = r0;
    if (/* eq */) goto .L_800E1F58;
    /* srwi. r0, r0, 3 */;
    /* mtctr r0 */;
    if (/* eq */) goto .L_800E1F44;
.L_800E1EF4:
    r0 = *(u16*)((u8*)r6 + 0x0);
    r7 = r7 + r0;
    r0 = *(u16*)((u8*)r6 + 0x2);
    r7 = r7 + r0;
    r0 = *(u16*)((u8*)r6 + 0x4);
    r7 = r7 + r0;
    r0 = *(u16*)((u8*)r6 + 0x6);
    r7 = r7 + r0;
    r0 = *(u16*)((u8*)r6 + 0x8);
    r7 = r7 + r0;
    r0 = *(u16*)((u8*)r6 + 0xA);
    r7 = r7 + r0;
    r0 = *(u16*)((u8*)r6 + 0xC);
    r7 = r7 + r0;
    r0 = *(u16*)((u8*)r6 + 0xE);
    r6 = r6 + 0x10;
    r7 = r7 + r0;
    if (--ctr != 0) goto .L_800E1EF4;
    r3 = r3 & 0x7;
    if (/* eq */) goto .L_800E1F58;
.L_800E1F44:
    /* mtctr r3 */;
.L_800E1F48:
    r0 = *(u16*)((u8*)r6 + 0x0);
    r6 = r6 + 0x2;
    r7 = r7 + r0;
    if (--ctr != 0) goto .L_800E1F48;
.L_800E1F58:
    /* cmplwi r5, 0x0 */;
    r3 = r5;
    if (/* eq */) goto .L_800E1FD4;
    /* srwi. r0, r5, 3 */;
    /* mtctr r0 */;
    if (/* eq */) goto .L_800E1FC0;
.L_800E1F70:
    r0 = *(u8*)((u8*)r6 + 0x0);
    r7 = r7 + r0;
    r0 = *(u8*)((u8*)r6 + 0x1);
    r7 = r7 + r0;
    r0 = *(u8*)((u8*)r6 + 0x2);
    r7 = r7 + r0;
    r0 = *(u8*)((u8*)r6 + 0x3);
    r7 = r7 + r0;
    r0 = *(u8*)((u8*)r6 + 0x4);
    r7 = r7 + r0;
    r0 = *(u8*)((u8*)r6 + 0x5);
    r7 = r7 + r0;
    r0 = *(u8*)((u8*)r6 + 0x6);
    r7 = r7 + r0;
    r0 = *(u8*)((u8*)r6 + 0x7);
    r6 = r6 + 0x8;
    r7 = r7 + r0;
    if (--ctr != 0) goto .L_800E1F70;
    r3 = r3 & 0x7;
    if (/* eq */) goto .L_800E1FD4;
.L_800E1FC0:
    /* mtctr r3 */;
.L_800E1FC4:
    r0 = *(u8*)((u8*)r6 + 0x0);
    r6 = r6 + 0x1;
    r7 = r7 + r0;
    if (--ctr != 0) goto .L_800E1FC4;
.L_800E1FD4:
    *(u16*)((u8*)r8 + 0xE) = r7;
.L_800E1FD8:
    r28 = *(u32*)lbl_8047AB30;
    goto .L_800E1FE4;
.L_800E1FE0:
    r28 = *(u32*)((u8*)r28 + 0x4);
.L_800E1FE4:
    /* cmplwi r28, 0x0 */;
    if (/* ne */) goto .L_800E159C;
.L_800E1FEC:
    r3 = *(u32*)lbl_8047AB30;
    r0 = 0x0;
    goto .L_800E200C;
.L_800E1FF8:
    r5 = *(u32*)((u8*)r3 + 0x8);
    /* cmplw r5, r0 */;
    if (/* le */) goto .L_800E2008;
    r0 = r5;
.L_800E2008:
    r3 = *(u32*)((u8*)r3 + 0x4);
.L_800E200C:
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800E1FF8;
    r3 = r0 - r4;
.L_800E2018:
    /* lmw r22, 0x18(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x44);
    return;
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
void fn_800D45F8(void) {
    return *(u32*)((u8*)lbl_8047AA80 + 0x0);
}

/* fn_800D4604 | Size: 0xC */
void fn_800D4604(void) {
    *(u32*)((u8*)lbl_8047AA80 + 0x0) = val;
}

/* fn_800D4610 | Size: 0xC */
void fn_800D4610(void) {
    *(u8*)((u8*)lbl_8047AA80 + 0x49C) = val;
}

/* fn_800D5504 -- GSlog_Init | Size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5504(void) {
    r5 = (u32)lbl_802703C0;
    r31 = (u32)lbl_802703C0;
    r30 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x490);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D5544;
    r3 = r31 + 0x0;
    /* crclr cr1eq */;
    fn_800DD970();
    goto .L_800D55B8;
.L_800D5544:
    fn_800E3534();
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x48C) = r3;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u16*)((u8*)r3 + 0x48C);
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800D5570;
    r3 = r31 + 0x28;
    /* crclr cr1eq */;
    fn_800DD970();
    goto .L_800D55B8;
.L_800D5570:
    fn_800E27B0();
    r5 = *(u32*)lbl_8047AA80;
    r4 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r0 = 0x0;
    *(u32*)((u8*)r5 + 0x490) = r3;
    r3 = r31 + 0x4c;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x490);
    *(u32*)((u8*)r6 + 0x494) = r5;
    r5 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r5 + 0x498) = r30;
    *(u32*)((u8*)r4 + 0x28) = r0;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x498);
    r5 = *(u32*)((u8*)r5 + 0x490);
    /* crclr cr1eq */;
    fn_800DD970();
.L_800D55B8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800D55D0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D55D0(void) {
    f0 = *(f32*)lbl_8047CA30;
    /* fcmpo cr0, f1, f0 */;
    if (/* lt */) goto .L_800D5638;
    f0 = *(f32*)lbl_8047CA34;
    /* fcmpo cr0, f1, f0 */;
    if (/* le */) goto .L_800D55F8;
    goto .L_800D5638;
.L_800D55F8:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D561C;
    r3 = 0x25;
    r4 = 0xb;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D5638;
.L_800D561C:
    f0 = *(f32*)lbl_8047CA38;
    r4 = 0x0;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)((u8*)r1 + 0x8) = f0;
    r3 = *(u32*)((u8*)r1 + 0xC);
    fn_800B944C();
.L_800D5638:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5648 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5648(void) {
    f0 = *(f32*)lbl_8047CA30;
    /* fcmpo cr0, f1, f0 */;
    if (/* lt */) goto .L_800D56B0;
    f0 = *(f32*)lbl_8047CA34;
    /* fcmpo cr0, f1, f0 */;
    if (/* le */) goto .L_800D5670;
    goto .L_800D56B0;
.L_800D5670:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5694;
    r3 = 0x24;
    r4 = 0xb;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D56B0;
.L_800D5694:
    f0 = *(f32*)lbl_8047CA38;
    r4 = 0x0;
    f0 = f0 * f1;
    f0 = (f64)(s32)f0;
    *(f64*)((u8*)r1 + 0x8) = f0;
    r3 = *(u32*)((u8*)r1 + 0xC);
    fn_800B9404();
.L_800D56B0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D56C0 | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D56C0(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5700;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5700;
    r5 = r3 & 0xFF;
    r3 = 0x23;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5714;
.L_800D5700:
    r4 = (u32)fn_800D7230;
    r0 = (u32)fn_800D7230;
    *(u32*)((u8*)r5 + 0x4A0) = r0;
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x4A4) = r3;
.L_800D5714:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5724 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5724(void) {
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r7 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5768;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5768;
    r5 = r3;
    r6 = r4 & 0xFF;
    r3 = 0x22;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D578C;
.L_800D5768:
    r5 = r3 << 2;
    r6 = (u32)fn_800D724C;
    r6 = (u32)fn_800D724C;
    r0 = r3 << 4;
    r3 = r7 + r5;
    *(u32*)((u8*)r3 + 0x500) = r6;
    r3 = *(u32*)lbl_8047AA80;
    r3 = r3 + r0;
    *(u8*)((u8*)r3 + 0x520) = r4;
.L_800D578C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D579C | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D579C(void) {
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r7 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D57E0;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D57E0;
    r5 = r3;
    r6 = r4 & 0xFFFF;
    r3 = 0x21;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5804;
.L_800D57E0:
    r5 = r3 << 2;
    r6 = (u32)fn_800D7268;
    r6 = (u32)fn_800D7268;
    r0 = r3 << 4;
    r3 = r7 + r5;
    *(u32*)((u8*)r3 + 0x500) = r6;
    r3 = *(u32*)lbl_8047AA80;
    r3 = r3 + r0;
    *(u16*)((u8*)r3 + 0x522) = r4;
.L_800D5804:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5814 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5814(void) {
    r8 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5860;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5860;
    r5 = r3;
    r6 = r4 & 0xFF;
    r7 = r8 & 0xFF;
    r3 = 0x1f;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5890;
.L_800D5860:
    r0 = r3 << 2;
    r5 = (u32)fn_800D72A4;
    r5 = (u32)fn_800D72A4;
    r7 = r3 << 4;
    r3 = r6 + r0;
    *(u32*)((u8*)r3 + 0x500) = r5;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r7;
    *(u8*)((u8*)r3 + 0x520) = r4;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r7;
    *(u8*)((u8*)r3 + 0x521) = r8;
.L_800D5890:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D58A0 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D58A0(void) {
    r8 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D58EC;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D58EC;
    r5 = r3;
    r6 = (s16)r4;
    r7 = (s16)r8;
    r3 = 0x1e;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D591C;
.L_800D58EC:
    r5 = (u32)fn_800D72C4;
    r0 = r3 << 2;
    r7 = r3 << 4;
    r5 = (u32)fn_800D72C4;
    r3 = r6 + r0;
    *(u32*)((u8*)r3 + 0x500) = r5;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r7;
    *(u16*)((u8*)r3 + 0x522) = r4;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r7;
    *(u16*)((u8*)r3 + 0x524) = r8;
.L_800D591C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D592C | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D592C(void) {
    r8 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5978;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5978;
    r5 = r3;
    r6 = r4 & 0xFFFF;
    r7 = r8 & 0xFFFF;
    r3 = 0x1d;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D59A8;
.L_800D5978:
    r0 = r3 << 2;
    r5 = (u32)fn_800D72E4;
    r5 = (u32)fn_800D72E4;
    r7 = r3 << 4;
    r3 = r6 + r0;
    *(u32*)((u8*)r3 + 0x500) = r5;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r7;
    *(u16*)((u8*)r3 + 0x522) = r4;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r7;
    *(u16*)((u8*)r3 + 0x524) = r8;
.L_800D59A8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D59B8 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D59B8(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D59F8;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D59F8;
    r5 = r3;
    r3 = 0x1c;
    r4 = 0xf;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D5A28;
.L_800D59F8:
    r0 = r3 << 2;
    r4 = (u32)fn_800D7304;
    r4 = (u32)fn_800D7304;
    r6 = r3 << 4;
    r3 = r5 + r0;
    *(u32*)((u8*)r3 + 0x500) = r4;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r6;
    *(f32*)((u8*)r3 + 0x528) = f1;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r6;
    *(f32*)((u8*)r3 + 0x52C) = f2;
.L_800D5A28:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5A38 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5A38(void) {
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r7 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5A7C;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5A7C;
    r5 = r3;
    r6 = r4 & 0xFF;
    r3 = 0x1b;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5AA0;
.L_800D5A7C:
    r0 = r3 << 2;
    r5 = (u32)fn_800D7328;
    r6 = (u32)fn_800D7328;
    r5 = r7 + r0;
    *(u32*)((u8*)r5 + 0x4E0) = r6;
    r0 = r3 * 0xc;
    r3 = *(u32*)lbl_8047AA80;
    r3 = r3 + r0;
    *(u8*)((u8*)r3 + 0x4E8) = r4;
.L_800D5AA0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5AB0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5AB0(void) {
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r7 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5AF4;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5AF4;
    r5 = r3;
    r6 = r4 & 0xFFFF;
    r3 = 0x1a;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5B18;
.L_800D5AF4:
    r0 = r3 << 2;
    r5 = (u32)fn_800D7344;
    r6 = (u32)fn_800D7344;
    r5 = r7 + r0;
    *(u32*)((u8*)r5 + 0x4E0) = r6;
    r0 = r3 * 0xc;
    r3 = *(u32*)lbl_8047AA80;
    r3 = r3 + r0;
    *(u16*)((u8*)r3 + 0x4EC) = r4;
.L_800D5B18:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5B28 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5B28(void) {
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r7 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5B6C;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5B6C;
    r5 = r3;
    r6 = r4 & 0xFFFF;
    r3 = 0x19;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5B90;
.L_800D5B6C:
    r0 = r3 << 2;
    r5 = (u32)fn_800D7360;
    r6 = (u32)fn_800D7360;
    r5 = r7 + r0;
    *(u32*)((u8*)r5 + 0x4E0) = r6;
    r0 = r3 * 0xc;
    r3 = *(u32*)lbl_8047AA80;
    r3 = r3 + r0;
    *(u16*)((u8*)r3 + 0x4EC) = r4;
.L_800D5B90:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5BA0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5BA0(void) {
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r7 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5BE4;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5BE4;
    r5 = r3;
    r6 = r4;
    r3 = 0x18;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5C08;
.L_800D5BE4:
    r0 = r3 << 2;
    r5 = (u32)fn_800D737C;
    r6 = (u32)fn_800D737C;
    r5 = r7 + r0;
    *(u32*)((u8*)r5 + 0x4E0) = r6;
    r0 = r3 * 0xc;
    r3 = *(u32*)lbl_8047AA80;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x4F0) = r4;
.L_800D5C08:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5C18 | Size: 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5C18(void) {
    r9 = r5;
    r8 = r6;
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r7 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5C6C;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5C6C;
    r5 = r3;
    r6 = r4 & 0xFF;
    r7 = r9 & 0xFF;
    r8 = r8 & 0xFF;
    r3 = 0x17;
    r4 = 0x4;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5CA8;
.L_800D5C6C:
    r0 = r3 << 2;
    r5 = (u32)fn_800D7398;
    r6 = (u32)fn_800D7398;
    r5 = r7 + r0;
    *(u32*)((u8*)r5 + 0x4E0) = r6;
    r5 = r3 * 0xc;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r5;
    *(u8*)((u8*)r3 + 0x4E8) = r4;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r5;
    *(u8*)((u8*)r3 + 0x4E9) = r9;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r5;
    *(u8*)((u8*)r3 + 0x4EA) = r8;
.L_800D5CA8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5CB8 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5CB8(void) {
    r11 = r5;
    r10 = r6;
    r9 = r7;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r8 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5D14;
    r0 = *(u32*)((u8*)r8 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5D14;
    r5 = r3;
    r6 = r4 & 0xFF;
    r7 = r11 & 0xFF;
    r8 = r10 & 0xFF;
    r9 = r9 & 0xFF;
    r3 = 0x16;
    r4 = 0x5;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5D5C;
.L_800D5D14:
    r0 = r3 << 2;
    r5 = (u32)fn_800D73C4;
    r6 = (u32)fn_800D73C4;
    r5 = r8 + r0;
    *(u32*)((u8*)r5 + 0x4E0) = r6;
    r5 = r3 * 0xc;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r5;
    *(u8*)((u8*)r3 + 0x4E8) = r4;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r5;
    *(u8*)((u8*)r3 + 0x4E9) = r11;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r5;
    *(u8*)((u8*)r3 + 0x4EA) = r10;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r5;
    *(u8*)((u8*)r3 + 0x4EB) = r9;
.L_800D5D5C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5D6C | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5D6C(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5DAC;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5DAC;
    r5 = r3 & 0xFF;
    r3 = 0x15;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5DC0;
.L_800D5DAC:
    r4 = (u32)fn_800D73F8;
    r0 = (u32)fn_800D73F8;
    *(u32*)((u8*)r5 + 0x4C4) = r0;
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x4C8) = r3;
.L_800D5DC0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5DD0 | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5DD0(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5E10;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5E10;
    r5 = r3 & 0xFFFF;
    r3 = 0x14;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5E24;
.L_800D5E10:
    r4 = (u32)fn_800D740C;
    r0 = (u32)fn_800D740C;
    *(u32*)((u8*)r5 + 0x4C4) = r0;
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x4CC) = r3;
.L_800D5E24:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5E34 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5E34(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5E80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5E80;
    r5 = (s8)r3;
    r6 = (s8)r4;
    r7 = (s8)r7;
    r3 = 0x13;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5EA4;
.L_800D5E80:
    r5 = (u32)fn_800D7420;
    r0 = (u32)fn_800D7420;
    *(u32*)((u8*)r6 + 0x4C4) = r0;
    r5 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r5 + 0x4C8) = r3;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4C9) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4CA) = r7;
.L_800D5EA4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5EB4 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5EB4(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5F00;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5F00;
    r5 = (s16)r3;
    r6 = (s16)r4;
    r7 = (s16)r7;
    r3 = 0x12;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D5F24;
.L_800D5F00:
    r5 = (u32)fn_800D7444;
    r0 = (u32)fn_800D7444;
    *(u32*)((u8*)r6 + 0x4C4) = r0;
    r5 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r5 + 0x4CC) = r3;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4CE) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4D0) = r7;
.L_800D5F24:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5F34 | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5F34(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5F70;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5F70;
    r3 = 0x11;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D5F94;
.L_800D5F70:
    r3 = (u32)fn_800D7468;
    r0 = (u32)fn_800D7468;
    *(u32*)((u8*)r4 + 0x4C4) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4D4) = f1;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4D8) = f2;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4DC) = f3;
.L_800D5F94:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D5FA4 | Size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D5FA4(void) {
    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D5FEC;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D5FEC;
    r5 = r31 & 0xFF;
    r3 = 0x10;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6014;
.L_800D5FEC:
    fn_800D6B00();
    r4 = (u32)fn_800D748C;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D748C;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AC) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6014:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D6028 | Size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6028(void) {
    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D6070;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6070;
    r5 = r31 & 0xFFFF;
    r3 = 0xf;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6098;
.L_800D6070:
    fn_800D6B00();
    r4 = (u32)fn_800D74A0;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D74A0;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B0) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6098:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D60AC | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D60AC(void) {
    r31 = r4;
    r30 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D6100;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6100;
    r5 = (s8)r30;
    r6 = (s8)r31;
    r3 = 0xe;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6130;
.L_800D6100:
    fn_800D6B00();
    r4 = (u32)fn_800D74B4;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D74B4;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AC) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AD) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6130:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800D6148 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6148(void) {
    r31 = r4;
    r30 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D619C;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D619C;
    r5 = r30 & 0xFF;
    r6 = r31 & 0xFF;
    r3 = 0xd;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D61CC;
.L_800D619C:
    fn_800D6B00();
    r4 = (u32)fn_800D74D0;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D74D0;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AC) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AD) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D61CC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800D61E4 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D61E4(void) {
    r31 = r4;
    r30 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D6238;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6238;
    r5 = (s16)r30;
    r6 = (s16)r31;
    r3 = 0xc;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6268;
.L_800D6238:
    fn_800D6B00();
    r4 = (u32)fn_800D74EC;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D74EC;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B0) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B2) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6268:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800D6280 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6280(void) {
    r31 = r4;
    r30 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D62D4;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D62D4;
    r5 = r30 & 0xFFFF;
    r6 = r31 & 0xFFFF;
    r3 = 0xb;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6304;
.L_800D62D4:
    fn_800D6B00();
    r4 = (u32)fn_800D7508;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D7508;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B0) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B2) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6304:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800D631C | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D631C(void) {
    *(f64*)((u8*)r1 + 0x18) = f31;
    f31 = f2;
    *(f64*)((u8*)r1 + 0x10) = f30;
    f30 = f1;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D6368;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6368;
    r3 = 0xa;
    r4 = 0xc;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D6398;
.L_800D6368:
    fn_800D6B00();
    r4 = (u32)fn_800D7524;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D7524;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4B8) = f30;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4BC) = f31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6398:
    r0 = *(u32*)((u8*)r1 + 0x24);
    f31 = *(f64*)((u8*)r1 + 0x18);
    f30 = *(f64*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800D63B0 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D63B0(void) {
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D6410;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6410;
    r5 = (s8)r29;
    r6 = (s8)r30;
    r7 = (s8)r31;
    r3 = 0x9;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6448;
.L_800D6410:
    fn_800D6B00();
    r4 = (u32)fn_800D7540;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D7540;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AC) = r29;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AD) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AE) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6448:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D6464 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6464(void) {
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D64C4;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D64C4;
    r5 = r29 & 0xFF;
    r6 = r30 & 0xFF;
    r7 = r31 & 0xFF;
    r3 = 0x8;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D64FC;
.L_800D64C4:
    fn_800D6B00();
    r4 = (u32)fn_800D7564;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D7564;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AC) = r29;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AD) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x4AE) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D64FC:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D6518 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6518(void) {
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D6578;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6578;
    r5 = (s16)r29;
    r6 = (s16)r30;
    r7 = (s16)r31;
    r3 = 0x7;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D65B0;
.L_800D6578:
    fn_800D6B00();
    r4 = (u32)fn_800D7588;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D7588;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B0) = r29;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B2) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B4) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D65B0:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D65CC | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D65CC(void) {
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r6 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D662C;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D662C;
    r5 = r29 & 0xFFFF;
    r6 = r30 & 0xFFFF;
    r7 = r31 & 0xFFFF;
    r3 = 0x6;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6664;
.L_800D662C:
    fn_800D6B00();
    r4 = (u32)fn_800D75AC;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D75AC;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B0) = r29;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B2) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B4) = r31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D6664:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D6680 | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6680(void) {
    *(f64*)((u8*)r1 + 0x18) = f31;
    f31 = f3;
    *(f64*)((u8*)r1 + 0x10) = f30;
    f30 = f2;
    *(f64*)((u8*)r1 + 0x8) = f29;
    f29 = f1;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D66D4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D66D4;
    r3 = 0x5;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D670C;
.L_800D66D4:
    fn_800D6B00();
    r4 = (u32)fn_800D75D0;
    r3 = *(u32*)lbl_8047AA80;
    r4 = (u32)fn_800D75D0;
    r0 = 0x1;
    *(u32*)((u8*)r3 + 0x4A8) = r4;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4B8) = f29;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4BC) = f30;
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4C0) = f31;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D670C:
    r0 = *(u32*)((u8*)r1 + 0x24);
    f31 = *(f64*)((u8*)r1 + 0x18);
    f30 = *(f64*)((u8*)r1 + 0x10);
    f29 = *(f64*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800D6728 | Size: 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6728(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D674C;
    fn_800D6B00();
    goto .L_800D67AC;
.L_800D674C:
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D676C;
    r3 = 0x3;
    r4 = 0x0;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D67AC;
.L_800D676C:
    fn_800D6B00();
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800D67AC;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* and. r0, r3, r0 */;
    if (/* eq */) goto .L_800D67AC;
    r3 = *(u32*)((u8*)r4 + 0x24);
    r0 = *(u32*)((u8*)r4 + 0x20);
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800D67AC;
    r0 = 0x0;
    *(u32*)((u8*)r4 + 0x24) = r0;
.L_800D67AC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D67BC -- MTX operations | Size: 0x244 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D67BC(void) {
    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D67E8;
    fn_800DB758();
    goto .L_800D69EC;
.L_800D67E8:
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D680C;
    r5 = r31 & 0xFFFF;
    r3 = 0x2;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D69EC;
.L_800D680C:
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800D69EC;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* and. r0, r3, r0 */;
    if (/* eq */) goto .L_800D69EC;
    r0 = *(u32*)((u8*)r4 + 0x24);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D6910;
    r3 = *(u32*)((u8*)r4 + 0x20);
    fn_800D7650();
    r3 = *(u32*)lbl_8047AA80;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x1;
    r3 = *(u32*)((u8*)r3 + 0x20);
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x10);
    /* rlwinm r0, r0, 0, 29, 29 */;
    if (/* eq */) goto .L_800D689C;
    r3 = *(u32*)((u8*)r3 + 0x20);
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x2;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
.L_800D689C:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x10);
    r0 = r0 & 0x1;
    if (/* eq */) goto .L_800D68D0;
    r3 = *(u32*)((u8*)r3 + 0x20);
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x6;
    r7 = 0xa;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
.L_800D68D0:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x10);
    /* rlwinm r0, r0, 0, 30, 30 */;
    if (/* eq */) goto .L_800D6904;
    r3 = *(u32*)((u8*)r3 + 0x20);
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x8;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
.L_800D6904:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x20);
    *(u32*)((u8*)r3 + 0x24) = r0;
.L_800D6910:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x14);
    /* cmpwi r0, 0x7 */;
    if (/* ne */) goto .L_800D6924;
    /* clrlslwi r31, r31, 17, 1 */;
.L_800D6924:
    r3 = *(u32*)((u8*)r3 + 0x24);
    fn_800D7A70();
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x24);
    fn_800D892C();
    r4 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_80314350;
    r3 = (u32)lbl_80314350;
    r5 = r31;
    r0 = *(u32*)((u8*)r4 + 0x14);
    r4 = *(u32*)((u8*)r4 + 0x24);
    r0 = r0 << 2;
    /* lwzx r3, r3, r0 */;
    r4 = *(u32*)((u8*)r4 + 0x4);
    fn_800B928C();
    r4 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_804001F0;
    r6 = (u32)lbl_804001F0;
    r5 = r31 & 0xFFFF;
    r3 = *(u32*)((u8*)r4 + 0x14);
    r0 = *(u32*)((u8*)r6 + 0xC);
    /* cmpwi r3, 0x6 */;
    r0 = r0 + r5;
    *(u32*)((u8*)r6 + 0xC) = r0;
    if (/* ge */) goto .L_800D6998;
    /* cmpwi r3, 0x3 */;
    if (/* eq */) goto .L_800D69A4;
    if (/* ge */) goto .L_800D69C8;
    goto .L_800D69EC;
.L_800D6998:
    /* cmpwi r3, 0x8 */;
    if (/* ge */) goto .L_800D69EC;
    goto .L_800D69DC;
.L_800D69A4:
    r3 = (0x5555 << 16);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r0 = r3 + 0x5556;
    r3 = (s32)((s64)r0 * (s64)r5 >> 32);
    r0 = (u32)r3 >> 31;
    r0 = r3 + r0;
    r0 = r4 + r0;
    *(u32*)((u8*)r6 + 0x4) = r0;
    goto .L_800D69EC;
.L_800D69C8:
    r0 = *(u32*)((u8*)r6 + 0x4);
    r3 = r5 + r0;
    /* subi r0, r3, 0x2 */;
    *(u32*)((u8*)r6 + 0x4) = r0;
    goto .L_800D69EC;
.L_800D69DC:
    r3 = *(u32*)((u8*)r6 + 0x4);
    /* extrwi r0, r31, 15, 16 */;
    r0 = r3 + r0;
    *(u32*)((u8*)r6 + 0x4) = r0;
.L_800D69EC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D6A00 | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6A00(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6A24;
    *(u32*)((u8*)r4 + 0x488) = r3;
    goto .L_800D6A4C;
.L_800D6A24:
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6A48;
    r5 = r3;
    r3 = 0x1;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D6A4C;
.L_800D6A48:
    *(u32*)((u8*)r4 + 0x14) = r3;
.L_800D6A4C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D6A5C | Size: 0x24 */
void fn_800D6A5C(void) {
    *(u32*)((u8*)r6 + 0xC) = r3;
    *(u32*)((u8*)r6 + 0x4) = r0;
}

/* fn_800D6A80 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6A80(void) {
    r0 = *(u32*)((u8*)r5 + 0x0);
    r7 = r3 & 0xFFFF;
    /* cmpwi r4, 0x6 */;
    r0 = r0 + r7;
    *(u32*)((u8*)r5 + 0x0) = r0;
    if (/* ge */) goto .L_800D6AA8;
    /* cmpwi r4, 0x3 */;
    if (/* eq */) goto .L_800D6AB4;
    if (/* ge */) goto .L_800D6AD8;
    return;
.L_800D6AA8:
    /* cmpwi r4, 0x8 */;
    if (/* ge */) return;
    goto .L_800D6AEC;
.L_800D6AB4:
    r3 = (0x5555 << 16);
    r4 = *(u32*)((u8*)r6 + 0x0);
    r0 = r3 + 0x5556;
    r3 = (s32)((s64)r0 * (s64)r7 >> 32);
    r0 = (u32)r3 >> 31;
    r0 = r3 + r0;
    r0 = r4 + r0;
    *(u32*)((u8*)r6 + 0x0) = r0;
    return;
.L_800D6AD8:
    r0 = *(u32*)((u8*)r6 + 0x0);
    r3 = r7 + r0;
    /* subi r0, r3, 0x2 */;
    *(u32*)((u8*)r6 + 0x0) = r0;
    return;
.L_800D6AEC:
    r4 = *(u32*)((u8*)r6 + 0x0);
    /* extrwi r0, r3, 15, 16 */;
    r0 = r4 + r0;
    *(u32*)((u8*)r6 + 0x0) = r0;
    return;
}
#pragma pop

/* fn_800D6B00 -- Large matrix setup | Size: 0x730 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D6B00(void) {
    r3 = (u32)lbl_804007E8;
    r31 = (u32)lbl_804007E8;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x49F);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D7210;
    r3 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800D6B4C;
    r0 = *(u32*)((u8*)r4 + 0x14);
    /* cmpwi r0, 0x7 */;
    if (/* eq */) goto .L_800D6B60;
.L_800D6B4C:
    /* cmplwi r3, 0x1 */;
    if (/* ne */) goto .L_800D70F8;
    r0 = *(u32*)((u8*)r4 + 0x488);
    /* cmpwi r0, 0x7 */;
    if (/* ne */) goto .L_800D70F8;
.L_800D6B60:
    r0 = *(u8*)((u8*)r4 + 0x18);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D70B4;
    r3 = r31 + 0x98;
    r4 = r4 + 0x4ac;
    r5 = 0x18;
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0x80;
    r5 = 0x18;
    r4 = r4 + 0x4e8;
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0x0;
    r5 = 0x80;
    r4 = r4 + 0x520;
    memcpy();
    r0 = *(u8*)((u8*)r31 + 0x148);
    r6 = r31 + 0x148;
    r3 = *(u32*)lbl_8047AA80;
    r4 = r31 + 0x130;
    r5 = 0x18;
    *(u8*)((u8*)r3 + 0x4AC) = r0;
    r0 = *(u16*)((u8*)r6 + 0x4);
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x4B0) = r0;
    f0 = *(f32*)((u8*)r6 + 0xC);
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x4B8) = f0;
    r3 = *(u32*)lbl_8047AA80;
    r3 = r3 + 0x4e8;
    memcpy();
    r0 = *(u8*)((u8*)r31 + 0xB0);
    r3 = r31 + 0xb0;
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x520) = r0;
    r0 = *(u16*)((u8*)r3 + 0x2);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x522) = r0;
    f0 = *(f32*)((u8*)r3 + 0x8);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x528) = f0;
    r0 = *(u8*)((u8*)r3 + 0x10);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x530) = r0;
    r0 = *(u16*)((u8*)r3 + 0x12);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x532) = r0;
    f0 = *(f32*)((u8*)r3 + 0x18);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x538) = f0;
    r0 = *(u8*)((u8*)r3 + 0x20);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x540) = r0;
    r0 = *(u16*)((u8*)r3 + 0x22);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x542) = r0;
    f0 = *(f32*)((u8*)r3 + 0x28);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x548) = f0;
    r0 = *(u8*)((u8*)r3 + 0x30);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x550) = r0;
    r0 = *(u16*)((u8*)r3 + 0x32);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x552) = r0;
    f0 = *(f32*)((u8*)r3 + 0x38);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x558) = f0;
    r0 = *(u8*)((u8*)r3 + 0x40);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x560) = r0;
    r0 = *(u16*)((u8*)r3 + 0x42);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x562) = r0;
    f0 = *(f32*)((u8*)r3 + 0x48);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x568) = f0;
    r0 = *(u8*)((u8*)r3 + 0x50);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x570) = r0;
    r0 = *(u16*)((u8*)r3 + 0x52);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x572) = r0;
    f0 = *(f32*)((u8*)r3 + 0x58);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x578) = f0;
    r0 = *(u8*)((u8*)r3 + 0x60);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x580) = r0;
    r0 = *(u16*)((u8*)r3 + 0x62);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x582) = r0;
    f0 = *(f32*)((u8*)r3 + 0x68);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x588) = f0;
    r0 = *(u8*)((u8*)r3 + 0x70);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x590) = r0;
    r0 = *(u16*)((u8*)r3 + 0x72);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x592) = r0;
    f0 = *(f32*)((u8*)r3 + 0x78);
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x598) = f0;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6D1C;
    fn_800DB098();
    goto .L_800D6E10;
.L_800D6D1C:
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800D6E10;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* and. r0, r3, r0 */;
    if (/* eq */) goto .L_800D6E10;
    r29 = *(u32*)((u8*)r4 + 0x24);
    r0 = *(u8*)((u8*)r29 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6D5C;
    r12 = *(u32*)((u8*)r4 + 0x4A0);
    r3 = 0x0;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6D5C:
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4A8);
    /* mtctr r12 */;
    /* indirect call via ctr */;
    r0 = *(u8*)((u8*)r29 + 0x40);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6D90;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4C4);
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6D90:
    r30 = 0x4;
    r28 = r29 + 0x70;
.L_800D6D98:
    r0 = *(u8*)((u8*)r28 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6DC0;
    /* subi r3, r30, 0x4 */;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x4e0;
    /* lwzx r12, r5, r0 */;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6DC0:
    r30 = r30 + 0x1;
    r28 = r28 + 0x1c;
    /* cmpwi r30, 0x5 */;
    if (/* le */) goto .L_800D6D98;
    r30 = 0x6;
    r28 = r29 + 0xa8;
.L_800D6DD8:
    r0 = *(u8*)((u8*)r28 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6E00;
    /* subi r3, r30, 0x6 */;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x500;
    /* lwzx r12, r5, r0 */;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6E00:
    r30 = r30 + 0x1;
    r28 = r28 + 0x1c;
    /* cmpwi r30, 0xd */;
    if (/* le */) goto .L_800D6DD8;
.L_800D6E10:
    r3 = *(u32*)lbl_8047AA80;
    r4 = r31 + 0x98;
    r5 = 0x18;
    r3 = r3 + 0x4ac;
    memcpy();
    r3 = *(u32*)lbl_8047AA80;
    r4 = r31 + 0x80;
    r5 = 0x18;
    r3 = r3 + 0x4e8;
    memcpy();
    r3 = *(u32*)lbl_8047AA80;
    r4 = r31 + 0x0;
    r5 = 0x80;
    r3 = r3 + 0x520;
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D6E64;
    fn_800DB098();
    goto .L_800D6F58;
.L_800D6E64:
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800D6F58;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* and. r0, r3, r0 */;
    if (/* eq */) goto .L_800D6F58;
    r30 = *(u32*)((u8*)r4 + 0x24);
    r0 = *(u8*)((u8*)r30 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6EA4;
    r12 = *(u32*)((u8*)r4 + 0x4A0);
    r3 = 0x0;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6EA4:
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4A8);
    /* mtctr r12 */;
    /* indirect call via ctr */;
    r0 = *(u8*)((u8*)r30 + 0x40);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6ED8;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4C4);
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6ED8:
    r29 = 0x4;
    r28 = r30 + 0x70;
.L_800D6EE0:
    r0 = *(u8*)((u8*)r28 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6F08;
    /* subi r3, r29, 0x4 */;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x4e0;
    /* lwzx r12, r5, r0 */;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6F08:
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    /* cmpwi r29, 0x5 */;
    if (/* le */) goto .L_800D6EE0;
    r29 = 0x6;
    r28 = r30 + 0xa8;
.L_800D6F20:
    r0 = *(u8*)((u8*)r28 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D6F48;
    /* subi r3, r29, 0x6 */;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x500;
    /* lwzx r12, r5, r0 */;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D6F48:
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    /* cmpwi r29, 0xd */;
    if (/* le */) goto .L_800D6F20;
.L_800D6F58:
    r6 = r31 + 0x148;
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u8*)((u8*)r6 + 0x1);
    r3 = r31 + 0xb0;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0x4AD) = r5;
    r5 = *(u16*)((u8*)r6 + 0x6);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x4B2) = r5;
    f0 = *(f32*)((u8*)r6 + 0x10);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x4BC) = f0;
    r5 = *(u8*)((u8*)r3 + 0x1);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x521) = r5;
    r5 = *(u16*)((u8*)r3 + 0x4);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x524) = r5;
    f0 = *(f32*)((u8*)r3 + 0xC);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x52C) = f0;
    r5 = *(u8*)((u8*)r3 + 0x11);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x531) = r5;
    r5 = *(u16*)((u8*)r3 + 0x14);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x534) = r5;
    f0 = *(f32*)((u8*)r3 + 0x1C);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x53C) = f0;
    r5 = *(u8*)((u8*)r3 + 0x21);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x541) = r5;
    r5 = *(u16*)((u8*)r3 + 0x24);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x544) = r5;
    f0 = *(f32*)((u8*)r3 + 0x2C);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x54C) = f0;
    r5 = *(u8*)((u8*)r3 + 0x31);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x551) = r5;
    r5 = *(u16*)((u8*)r3 + 0x34);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x554) = r5;
    f0 = *(f32*)((u8*)r3 + 0x3C);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x55C) = f0;
    r5 = *(u8*)((u8*)r3 + 0x41);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x561) = r5;
    r5 = *(u16*)((u8*)r3 + 0x44);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x564) = r5;
    f0 = *(f32*)((u8*)r3 + 0x4C);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x56C) = f0;
    r5 = *(u8*)((u8*)r3 + 0x51);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x571) = r5;
    r5 = *(u16*)((u8*)r3 + 0x54);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x574) = r5;
    f0 = *(f32*)((u8*)r3 + 0x5C);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x57C) = f0;
    r5 = *(u8*)((u8*)r3 + 0x61);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x581) = r5;
    r5 = *(u16*)((u8*)r3 + 0x64);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x584) = r5;
    f0 = *(f32*)((u8*)r3 + 0x6C);
    r4 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r4 + 0x58C) = f0;
    r5 = *(u8*)((u8*)r3 + 0x71);
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x591) = r5;
    r5 = *(u16*)((u8*)r3 + 0x74);
    r4 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r4 + 0x594) = r5;
    f0 = *(f32*)((u8*)r3 + 0x7C);
    r3 = *(u32*)lbl_8047AA80;
    *(f32*)((u8*)r3 + 0x59C) = f0;
    r3 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r3 + 0x18) = r0;
    goto .L_800D70F8;
.L_800D70B4:
    r3 = r31 + 0x148;
    r4 = r4 + 0x4ac;
    r5 = 0x18;
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0x130;
    r5 = 0x18;
    r4 = r4 + 0x4e8;
    memcpy();
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0xb0;
    r5 = 0x80;
    r4 = r4 + 0x520;
    memcpy();
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x18) = r0;
.L_800D70F8:
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7110;
    fn_800DB098();
    goto .L_800D7204;
.L_800D7110:
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800D7204;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* and. r0, r3, r0 */;
    if (/* eq */) goto .L_800D7204;
    r30 = *(u32*)((u8*)r4 + 0x24);
    r0 = *(u8*)((u8*)r30 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D7150;
    r12 = *(u32*)((u8*)r4 + 0x4A0);
    r3 = 0x0;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D7150:
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4A8);
    /* mtctr r12 */;
    /* indirect call via ctr */;
    r0 = *(u8*)((u8*)r30 + 0x40);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D7184;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4C4);
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D7184:
    r29 = 0x4;
    r28 = r30 + 0x70;
.L_800D718C:
    r0 = *(u8*)((u8*)r28 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D71B4;
    /* subi r3, r29, 0x4 */;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x4e0;
    /* lwzx r12, r5, r0 */;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D71B4:
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    /* cmpwi r29, 0x5 */;
    if (/* le */) goto .L_800D718C;
    r29 = 0x6;
    r28 = r30 + 0xa8;
.L_800D71CC:
    r0 = *(u8*)((u8*)r28 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D71F4;
    /* subi r3, r29, 0x6 */;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x500;
    /* lwzx r12, r5, r0 */;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800D71F4:
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    /* cmpwi r29, 0xd */;
    if (/* le */) goto .L_800D71CC;
.L_800D7204:
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x49F) = r0;
.L_800D7210:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r28 = *(u32*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800D7230 | Size: 0x1C */
void fn_800D7230(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1188, False);
    u8* base = state + (idx << 1);
    *(volatile u8*)fifo = *(u8*)(state + 0x0);
}

/* fn_800D724C | Size: 0x1C */
void fn_800D724C(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx << 4);
    *(volatile u8*)fifo = *(u8*)(base + 0x520);
}

/* fn_800D7268 | Size: 0x1C */
void fn_800D7268(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx << 4);
    *(volatile u16*)fifo = *(u16*)(base + 0x522);
}

/* fn_800D7284 | Size: 0x20 */
void fn_800D7284(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx << 4);
    *(volatile u8*)fifo = *(u8*)(base + 0x520);
    *(volatile u8*)fifo = *(u8*)(base + 0x520);
}

/* fn_800D72A4 | Size: 0x20 */
void fn_800D72A4(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx << 4);
    *(volatile u8*)fifo = *(u8*)(base + 0x520);
    *(volatile u8*)fifo = *(u8*)(base + 0x520);
}

/* fn_800D72C4 | Size: 0x20 */
void fn_800D72C4(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx << 4);
    *(volatile u16*)fifo = *(u16*)(base + 0x522);
    *(volatile u16*)fifo = *(u16*)(base + 0x522);
}

/* fn_800D72E4 | Size: 0x20 */
void fn_800D72E4(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx << 4);
    *(volatile u16*)fifo = *(u16*)(base + 0x522);
    *(volatile u16*)fifo = *(u16*)(base + 0x522);
}

/* fn_800D7304 | Size: 0x24 */
void fn_800D7304(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx << 4);
    *(volatile f32*)fifo = *(f32*)(base + 0x528);
    *(volatile f32*)fifo = *(f32*)(base + 0x52C);
}

/* fn_800D7328 | Size: 0x1C */
void fn_800D7328(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx * 12);
    *(volatile u8*)fifo = *(u8*)(base + 0x4E8);
}

/* fn_800D7344 | Size: 0x1C */
void fn_800D7344(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx * 12);
    *(volatile u16*)fifo = *(u16*)(base + 0x4EC);
}

/* fn_800D7360 | Size: 0x1C */
void fn_800D7360(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx * 12);
    *(volatile u16*)fifo = *(u16*)(base + 0x4EC);
}

/* fn_800D737C | Size: 0x1C */
void fn_800D737C(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)base;
    u8* base = state + (idx * 12);
    *(volatile u32*)fifo = *(u32*)(base + 0x4F0);
}

/* fn_800D7398 | Size: 0x2C */
void fn_800D7398(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1257, True);
    u8* base = state + (idx * 12);
    *(volatile u8*)fifo = *(u8*)(base + 0x4E8);
    *(volatile u8*)fifo = *(u8*)(base + 0x4E9);
    *(volatile u8*)fifo = *(u8*)(base + 0x4EA);
}

/* fn_800D73C4 | Size: 0x34 */
void fn_800D73C4(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1257, True);
    u8* base = state + (idx * 12);
    *(volatile u8*)fifo = *(u8*)(base + 0x4E8);
    *(volatile u8*)fifo = *(u8*)(base + 0x4E9);
    *(volatile u8*)fifo = *(u8*)(base + 0x4EA);
    *(volatile u8*)fifo = *(u8*)(base + 0x4EB);
}

/* fn_800D73F8 | Size: 0x14 */
void fn_800D73F8(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *(volatile u8*)fifo = *(u8*)(state + 0x4C8);
}

/* fn_800D740C | Size: 0x14 */
void fn_800D740C(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *(volatile u16*)fifo = *(u16*)(state + 0x4CC);
}

/* fn_800D7420 | Size: 0x24 */
void fn_800D7420(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1225, False);
    *(volatile u8*)fifo = *(u8*)(state + 0x4C8);
    *(volatile u8*)fifo = *(u8*)(state + 0x4C9);
    *(volatile u8*)fifo = *(u8*)(state + 0x4CA);
}

/* fn_800D7444 | Size: 0x24 */
void fn_800D7444(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u16', 1230, False);
    *(volatile u16*)fifo = *(u16*)(state + 0x4CC);
    *(volatile u16*)fifo = *(u16*)(state + 0x4CE);
    *(volatile u16*)fifo = *(u16*)(state + 0x4D0);
}

/* fn_800D7468 | Size: 0x24 */
void fn_800D7468(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *(volatile f32*)fifo = *(f32*)(state + 0x4D4);
    *(volatile f32*)fifo = *(f32*)(state + 0x4D8);
    *(volatile f32*)fifo = *(f32*)(state + 0x4DC);
}

/* fn_800D748C | Size: 0x14 */
void fn_800D748C(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *(volatile u8*)fifo = *(u8*)(state + 0x4AC);
}

/* fn_800D74A0 | Size: 0x14 */
void fn_800D74A0(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *(volatile u16*)fifo = *(u16*)(state + 0x4B0);
}

/* fn_800D74B4 | Size: 0x1C */
void fn_800D74B4(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1197, False);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AC);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AD);
}

/* fn_800D74D0 | Size: 0x1C */
void fn_800D74D0(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1197, False);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AC);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AD);
}

/* fn_800D74EC | Size: 0x1C */
void fn_800D74EC(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u16', 1202, False);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B0);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B2);
}

/* fn_800D7508 | Size: 0x1C */
void fn_800D7508(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u16', 1202, False);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B0);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B2);
}

/* fn_800D7524 | Size: 0x1C */
void fn_800D7524(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *(volatile f32*)fifo = *(f32*)(state + 0x4B8);
    *(volatile f32*)fifo = *(f32*)(state + 0x4BC);
}

/* fn_800D7540 | Size: 0x24 */
void fn_800D7540(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1197, False);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AC);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AD);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AE);
}

/* fn_800D7564 | Size: 0x24 */
void fn_800D7564(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u8', 1197, False);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AC);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AD);
    *(volatile u8*)fifo = *(u8*)(state + 0x4AE);
}

/* fn_800D7588 | Size: 0x24 */
void fn_800D7588(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u16', 1202, False);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B0);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B2);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B4);
}

/* fn_800D75AC | Size: 0x24 */
void fn_800D75AC(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)('u16', 1202, False);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B0);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B2);
    *(volatile u16*)fifo = *(u16*)(state + 0x4B4);
}

/* fn_800D75D0 | Size: 0x24 */
void fn_800D75D0(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *(volatile f32*)fifo = *(f32*)(state + 0x4B8);
    *(volatile f32*)fifo = *(f32*)(state + 0x4BC);
    *(volatile f32*)fifo = *(f32*)(state + 0x4C0);
}

/* fn_800D75F4 | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D75F4(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x24);
    /* cmplw r0, r3 */;
    if (/* ne */) goto .L_800D760C;
    r0 = 0x0;
    *(u32*)((u8*)r4 + 0x24) = r0;
.L_800D760C:
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x8) = r0;
    *(u8*)((u8*)r3 + 0x24) = r0;
    *(u8*)((u8*)r3 + 0x40) = r0;
    *(u8*)((u8*)r3 + 0x5C) = r0;
    *(u8*)((u8*)r3 + 0x78) = r0;
    *(u8*)((u8*)r3 + 0x94) = r0;
    *(u8*)((u8*)r3 + 0xB0) = r0;
    *(u8*)((u8*)r3 + 0xCC) = r0;
    *(u8*)((u8*)r3 + 0xE8) = r0;
    *(u8*)((u8*)r3 + 0x104) = r0;
    *(u8*)((u8*)r3 + 0x120) = r0;
    *(u8*)((u8*)r3 + 0x13C) = r0;
    *(u8*)((u8*)r3 + 0x158) = r0;
    *(u8*)((u8*)r3 + 0x174) = r0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    return;
}
#pragma pop

/* fn_800D7650 | Size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7650(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x24);
    /* cmplw r0, r3 */;
    if (/* ne */) goto .L_800D7668;
    r0 = 0x0;
    *(u32*)((u8*)r4 + 0x24) = r0;
.L_800D7668:
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x8) = r0;
    *(u8*)((u8*)r3 + 0x24) = r0;
    *(u8*)((u8*)r3 + 0x40) = r0;
    *(u8*)((u8*)r3 + 0x5C) = r0;
    *(u8*)((u8*)r3 + 0x78) = r0;
    *(u8*)((u8*)r3 + 0x94) = r0;
    *(u8*)((u8*)r3 + 0xB0) = r0;
    *(u8*)((u8*)r3 + 0xCC) = r0;
    *(u8*)((u8*)r3 + 0xE8) = r0;
    *(u8*)((u8*)r3 + 0x104) = r0;
    *(u8*)((u8*)r3 + 0x120) = r0;
    *(u8*)((u8*)r3 + 0x13C) = r0;
    *(u8*)((u8*)r3 + 0x158) = r0;
    *(u8*)((u8*)r3 + 0x174) = r0;
    return;
}
#pragma pop

/* fn_800D76A8 | Size: 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D76A8(void) {
    /* stmw r24, 0x10(r1) */;
    r29 = r3;
    r30 = r4;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D76E8;
    r24 = *(u32*)((u8*)r5 + 0x24);
    *(u32*)((u8*)r5 + 0x24) = r29;
    fn_800D7940();
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x24) = r24;
    goto .L_800D780C;
.L_800D76E8:
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7710;
    r5 = r29;
    r6 = r30 & 0xFFFF;
    r3 = 0x47;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D780C;
.L_800D7710:
    r3 = (u32)lbl_80314370;
    r4 = (u32)lbl_803143B4;
    r31 = (u32)lbl_80314370;
    r25 = r29;
    r3 = (u32)lbl_803143D8;
    r27 = (u32)lbl_803143B4;
    r24 = r31;
    r26 = 0x0;
    r28 = (u32)lbl_803143D8;
.L_800D7734:
    r0 = *(u8*)((u8*)r25 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D7770;
    /* cmpwi r26, 0x0 */;
    if (/* eq */) goto .L_800D7770;
    r3 = *(u32*)((u8*)r25 + 0x10);
    r0 = *(u32*)((u8*)r25 + 0x14);
    r5 = r3 << 2;
    r3 = *(u32*)((u8*)r29 + 0x4);
    r0 = r0 << 2;
    r4 = *(u32*)((u8*)r24 + 0x0);
    /* lwzx r5, r27, r5 */;
    /* lwzx r6, r28, r0 */;
    r7 = *(u8*)((u8*)r25 + 0x18);
    fn_800B7D74();
.L_800D7770:
    r26 = r26 + 0x1;
    r24 = r24 + 0x4;
    /* cmpwi r26, 0xe */;
    r25 = r25 + 0x1c;
    if (/* lt */) goto .L_800D7734;
    fn_800B7D3C();
    r3 = (u32)lbl_803143A8;
    r24 = r29;
    r25 = 0x0;
    r28 = (u32)lbl_803143A8;
.L_800D7798:
    r0 = *(u8*)((u8*)r24 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D77D0;
    r0 = *(u32*)((u8*)r24 + 0xC);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = r0 << 2;
    /* lwzx r4, r28, r0 */;
    fn_800B7874();
    r4 = *(u32*)((u8*)r24 + 0x1C);
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800D77D0;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = *(u8*)((u8*)r24 + 0x20);
    fn_800B84E0();
.L_800D77D0:
    r25 = r25 + 0x1;
    r31 = r31 + 0x4;
    /* cmpwi r25, 0xe */;
    r24 = r24 + 0x1c;
    if (/* lt */) goto .L_800D7798;
    r4 = (u32)lbl_804001F0;
    r3 = r29;
    r5 = (u32)lbl_804001F0;
    r4 = *(u32*)((u8*)r5 + 0x14);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x14) = r0;
    fn_800D892C();
    r3 = r29;
    r4 = r30;
    fn_800D7940();
.L_800D780C:
    /* lmw r24, 0x10(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x34);
    return;
}
#pragma pop

/* fn_800D7820 | Size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7820(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7854;
    r5 = r3;
    r3 = 0x46;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D7858;
.L_800D7854:
    *(u32*)((u8*)r4 + 0x24) = r3;
.L_800D7858:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D7868 | Size: 0x2C */
void fn_800D7868(void) {
    *(u8*)((u8*)r3 + 0x8) = r0;
    *(u32*)((u8*)r3 + 0xC) = r5;
    *(u32*)((u8*)r3 + 0x10) = r6;
    *(u32*)((u8*)r3 + 0x14) = r7;
    *(u8*)((u8*)r3 + 0x18) = r8;
    *(u32*)((u8*)r3 + 0x1C) = r9;
    *(u8*)((u8*)r3 + 0x20) = r10;
}

/* fn_800D7894 -- GSgfx_InitViewport | Size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7894(void) {
    r0 = *(u32*)lbl_8047AAB0;
    r3 = *(u32*)lbl_8047AAAC;
    /* mtctr r0 */;
    /* cmplwi r0, 0x0 */;
    if (/* le */) goto .L_800D7938;
.L_800D78A8:
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800D7930;
    r0 = 0x1;
    r4 = (u32)lbl_803144D0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r4 = (u32)lbl_803144D0;
    r0 = *(u32*)lbl_8047AAB4;
    r0 = r0 << 2;
    /* lwzx r0, r4, r0 */;
    *(u32*)((u8*)r3 + 0x4) = r0;
    r4 = *(u32*)lbl_8047AAB4;
    r0 = r4 + 0x1;
    /* cmplwi r0, 0x8 */;
    *(u32*)lbl_8047AAB4 = r0;
    if (/* lt */) goto .L_800D78F0;
    r0 = 0x0;
    *(u32*)lbl_8047AAB4 = r0;
.L_800D78F0:
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x8) = r0;
    *(u8*)((u8*)r3 + 0x24) = r0;
    *(u8*)((u8*)r3 + 0x40) = r0;
    *(u8*)((u8*)r3 + 0x5C) = r0;
    *(u8*)((u8*)r3 + 0x78) = r0;
    *(u8*)((u8*)r3 + 0x94) = r0;
    *(u8*)((u8*)r3 + 0xB0) = r0;
    *(u8*)((u8*)r3 + 0xCC) = r0;
    *(u8*)((u8*)r3 + 0xE8) = r0;
    *(u8*)((u8*)r3 + 0x104) = r0;
    *(u8*)((u8*)r3 + 0x120) = r0;
    *(u8*)((u8*)r3 + 0x13C) = r0;
    *(u8*)((u8*)r3 + 0x158) = r0;
    *(u8*)((u8*)r3 + 0x174) = r0;
    return;
.L_800D7930:
    r3 = r3 + 0x190;
    if (--ctr != 0) goto .L_800D78A8;
.L_800D7938:
    r3 = 0x0;
    return;
}
#pragma pop

/* fn_800D7940 | Size: 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7940(void) {
    /* stmw r27, 0xc(r1) */;
    r28 = r4;
    r27 = r3;
    r3 = r28;
    fn_800D67BC();
    r31 = r28 & 0xFFFF;
    r28 = 0x0;
    goto .L_800D7A4C;
.L_800D796C:
    r0 = *(u32*)((u8*)r27 + 0x28);
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D7984;
    r3 = r28;
    fn_800D6028();
    goto .L_800D798C;
.L_800D7984:
    r3 = r28 & 0xFF;
    fn_800D5FA4();
.L_800D798C:
    r0 = *(u8*)((u8*)r27 + 0x40);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D79B8;
    r0 = *(u32*)((u8*)r27 + 0x44);
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D79B0;
    r3 = r28;
    fn_800D5DD0();
    goto .L_800D79B8;
.L_800D79B0:
    r3 = r28 & 0xFF;
    fn_800D5D6C();
.L_800D79B8:
    r29 = 0x4;
    r30 = r27 + 0x70;
.L_800D79C0:
    r0 = *(u8*)((u8*)r30 + 0x8);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D79F0;
    r0 = *(u32*)((u8*)r30 + 0xC);
    /* subi r3, r29, 0x4 */;
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D79E8;
    r4 = r28;
    fn_800D5AB0();
    goto .L_800D79F0;
.L_800D79E8:
    r4 = r28 & 0xFF;
    fn_800D5A38();
.L_800D79F0:
    r29 = r29 + 0x1;
    r30 = r30 + 0x1c;
    /* cmpwi r29, 0x5 */;
    if (/* le */) goto .L_800D79C0;
    r29 = 0x6;
    r30 = r27 + 0xa8;
.L_800D7A08:
    r0 = *(u8*)((u8*)r30 + 0x8);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7A38;
    r0 = *(u32*)((u8*)r30 + 0xC);
    /* subi r3, r29, 0x6 */;
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D7A30;
    r4 = r28;
    fn_800D579C();
    goto .L_800D7A38;
.L_800D7A30:
    r4 = r28 & 0xFF;
    fn_800D5724();
.L_800D7A38:
    r29 = r29 + 0x1;
    r30 = r30 + 0x1c;
    /* cmpwi r29, 0xd */;
    if (/* le */) goto .L_800D7A08;
    r28 = r28 + 0x1;
.L_800D7A4C:
    r0 = r28 & 0xFFFF;
    /* cmplw r0, r31 */;
    if (/* lt */) goto .L_800D796C;
    fn_800D6728();
    /* lmw r27, 0xc(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    return;
}
#pragma pop

/* fn_800D7A70 | Size: 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7A70(void) {
    r4 = (u32)lbl_80314370;
    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r31 = (u32)lbl_80314370;
    r4 = (u32)lbl_803143B4;
    r3 = (u32)lbl_803143D8;
    r28 = r25;
    r27 = r31;
    r29 = (u32)lbl_803143B4;
    r30 = (u32)lbl_803143D8;
    r26 = 0x0;
.L_800D7AA8:
    r0 = *(u8*)((u8*)r28 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D7AE4;
    /* cmpwi r26, 0x0 */;
    if (/* eq */) goto .L_800D7AE4;
    r3 = *(u32*)((u8*)r28 + 0x10);
    r0 = *(u32*)((u8*)r28 + 0x14);
    r5 = r3 << 2;
    r3 = *(u32*)((u8*)r25 + 0x4);
    r0 = r0 << 2;
    r4 = *(u32*)((u8*)r27 + 0x0);
    /* lwzx r5, r29, r5 */;
    /* lwzx r6, r30, r0 */;
    r7 = *(u8*)((u8*)r28 + 0x18);
    fn_800B7D74();
.L_800D7AE4:
    r26 = r26 + 0x1;
    r27 = r27 + 0x4;
    /* cmpwi r26, 0xe */;
    r28 = r28 + 0x1c;
    if (/* lt */) goto .L_800D7AA8;
    fn_800B7D3C();
    r3 = (u32)lbl_803143A8;
    r27 = r25;
    r28 = 0x0;
    r30 = (u32)lbl_803143A8;
.L_800D7B0C:
    r0 = *(u8*)((u8*)r27 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D7B44;
    r0 = *(u32*)((u8*)r27 + 0xC);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = r0 << 2;
    /* lwzx r4, r30, r0 */;
    fn_800B7874();
    r4 = *(u32*)((u8*)r27 + 0x1C);
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800D7B44;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = *(u8*)((u8*)r27 + 0x20);
    fn_800B84E0();
.L_800D7B44:
    r28 = r28 + 0x1;
    r31 = r31 + 0x4;
    /* cmpwi r28, 0xe */;
    r27 = r27 + 0x1c;
    if (/* lt */) goto .L_800D7B0C;
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x14);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x14) = r0;
    /* lmw r25, 0x14(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x34);
    return;
}
#pragma pop

/* fn_800D7B80 -- GSgfx_InitProjection | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7B80(void) {
    r0 = r3;
    r3 = r0 * 0x190;
    *(u32*)lbl_8047AAB0 = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAA8 = r3;
    if (/* eq */) goto .L_800D7BE8;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AAAC = r3;
    r4 = r5;
    r6 = 0x0;
    goto .L_800D7BD4;
.L_800D7BC4:
    r3 = *(u32*)lbl_8047AAAC;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x190;
.L_800D7BD4:
    r0 = *(u32*)lbl_8047AAB0;
    /* cmplw r6, r0 */;
    if (/* lt */) goto .L_800D7BC4;
    r0 = 0x0;
    *(u32*)lbl_8047AAB4 = r0;
.L_800D7BE8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D7BF8 | Size: 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7BF8(void) {
    r31 = r3;
    fn_800D2584();
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800D7C20;
    r3 = 0x0;
    goto .L_800D7C60;
.L_800D7C20:
    /* cmpwi r31, 0x1 */;
    if (/* eq */) goto .L_800D7C4C;
    if (/* ge */) goto .L_800D7C38;
    /* cmpwi r31, 0x0 */;
    if (/* ge */) goto .L_800D7C44;
    goto .L_800D7C5C;
.L_800D7C38:
    /* cmpwi r31, 0x3 */;
    if (/* ge */) goto .L_800D7C5C;
    goto .L_800D7C54;
.L_800D7C44:
    fn_800D1D00();
    goto .L_800D7C60;
.L_800D7C4C:
    fn_800D1B3C();
    goto .L_800D7C60;
.L_800D7C54:
    fn_800D1A70();
    goto .L_800D7C60;
.L_800D7C5C:
    r3 = 0x0;
.L_800D7C60:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D7C74 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7C74(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7CA8;
    r3 = 0x45;
    r4 = 0x0;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D7CFC;
.L_800D7CA8:
    r0 = *(u8*)lbl_8047AAC8;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D7CFC;
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D7CFC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D7D10 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7D10(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7D48;
    r6 = r4;
    r5 = r3 & 0xFF;
    r3 = 0x44;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D7D80;
.L_800D7D48:
    r0 = r3 & 0xFF;
    /* cmplwi r0, 0x9 */;
    if (/* le */) goto .L_800D7D68;
    r3 = (u32)lbl_80270440;
    r3 = (u32)lbl_80270440;
    /* crclr cr1eq */;
    fn_800DD970();
    goto .L_800D7D80;
.L_800D7D68:
    r6 = r0 * 0x30;
    r5 = (u32)lbl_80400948;
    r3 = r4;
    r0 = (u32)lbl_80400948;
    r4 = r0 + r6;
    fn_800E0628();
.L_800D7D80:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D7D90 | Size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7D90(void) {
    r30 = r4;
    r29 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7DDC;
    r6 = r30;
    r5 = r29 & 0xFF;
    r3 = 0x43;
    r4 = 0x11;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D7E40;
.L_800D7DDC:
    r31 = r29 & 0xFF;
    /* cmplwi r31, 0x9 */;
    if (/* le */) goto .L_800D7DFC;
    r3 = (u32)lbl_80270440;
    r3 = (u32)lbl_80270440;
    /* crclr cr1eq */;
    fn_800DD970();
    goto .L_800D7E40;
.L_800D7DFC:
    r3 = (u32)lbl_80314610;
    /* clrlslwi r0, r29, 24, 2 */;
    r4 = (u32)lbl_80314610;
    r3 = r30;
    /* lwzx r4, r4, r0 */;
    fn_800BD4B4();
    r5 = r31 * 0x30;
    r3 = (u32)lbl_80400948;
    r4 = r30;
    r0 = (u32)lbl_80400948;
    r3 = r0 + r5;
    fn_800E0628();
    r0 = r31;
    /* cmplwi r0, 0x9 */;
    if (/* ne */) goto .L_800D7E40;
    r0 = 0x1;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D7E40:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D7E5C | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7E5C(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7E90;
    r3 = 0x42;
    r4 = 0x0;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D7F00;
.L_800D7E90:
    r3 = *(u32*)lbl_8047AAC0;
    r0 = *(u32*)lbl_8047AAC4;
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800D7EB4;
    r3 = (u32)lbl_80270460;
    r3 = (u32)lbl_80270460;
    /* crclr cr1eq */;
    fn_800DD970();
    goto .L_800D7F00;
.L_800D7EB4:
    r4 = (u32)lbl_80314610;
    r3 = r3 + 0x30;
    r4 = (u32)lbl_80314610;
    *(u32*)lbl_8047AAC0 = r3;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D7F00:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D7F14 | Size: 0xD0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7F14(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D7F4C;
    r5 = r3;
    r3 = 0x41;
    r4 = 0x10;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D7FD0;
.L_800D7F4C:
    r4 = *(u32*)lbl_8047AAC0;
    r0 = *(u32*)lbl_8047AABC;
    /* cmplw r4, r0 */;
    if (/* gt */) goto .L_800D7F70;
    r3 = (u32)lbl_80270480;
    r3 = (u32)lbl_80270480;
    /* crclr cr1eq */;
    fn_800DD970();
    goto .L_800D7FD0;
.L_800D7F70:
    /* subi r4, r4, 0x30 */;
    r5 = r3;
    *(u32*)lbl_8047AAC0 = r4;
    r3 = r4;
    r4 = r4 + 0x30;
    fn_800E0290();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D7FD0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D7FE4 | Size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D7FE4(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D801C;
    r5 = r3;
    r3 = 0x40;
    r4 = 0x10;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D8074;
.L_800D801C:
    r4 = *(u32*)lbl_8047AAC0;
    r5 = r3;
    r3 = r4;
    fn_800E0290();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D8074:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D8088 | Size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D8088(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D80C0;
    r5 = r3;
    r3 = 0x3f;
    r4 = 0x10;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D8140;
.L_800D80C0:
    r4 = *(u32*)lbl_8047AAC0;
    r0 = *(u32*)lbl_8047AABC;
    /* cmplw r4, r0 */;
    if (/* gt */) goto .L_800D80E4;
    r3 = (u32)lbl_80270480;
    r3 = (u32)lbl_80270480;
    /* crclr cr1eq */;
    fn_800DD970();
    goto .L_800D8140;
.L_800D80E4:
    /* subi r0, r4, 0x30 */;
    r4 = r3;
    *(u32*)lbl_8047AAC0 = r0;
    r3 = r0;
    fn_800E0628();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D8140:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D8154 | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D8154(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8188;
    r3 = 0x3e;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D81D8;
.L_800D8188:
    r3 = *(u32*)lbl_8047AAC0;
    fn_800E02C4();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D81D8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D81EC | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D81EC(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8220;
    r3 = 0x3d;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D8270;
.L_800D8220:
    r3 = *(u32*)lbl_8047AAC0;
    fn_800E03E8();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D8270:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D8284 | Size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D8284(void) {
    *(f64*)((u8*)r1 + 0x18) = f31;
    f31 = f3;
    *(f64*)((u8*)r1 + 0x10) = f30;
    f30 = f2;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D82C8;
    r3 = 0x3c;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D8330;
.L_800D82C8:
    r3 = *(u32*)lbl_8047AAC0;
    fn_800E0370();
    f1 = f30;
    r3 = *(u32*)lbl_8047AAC0;
    fn_800E032C();
    f1 = f31;
    r3 = *(u32*)lbl_8047AAC0;
    fn_800E02E8();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D8330:
    r0 = *(u32*)((u8*)r1 + 0x24);
    f31 = *(f64*)((u8*)r1 + 0x18);
    f30 = *(f64*)((u8*)r1 + 0x10);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D834C | Size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D834C(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8380;
    r3 = 0x3b;
    r4 = 0x0;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D83D0;
.L_800D8380:
    r3 = *(u32*)lbl_8047AAC0;
    fn_800E064C();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D83D0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D83E4 -- GSgfx_InitMatrixStack | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D83E4(void) {
    r31 = r3;
    r3 = r31 * 0x30;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAB8 = r3;
    if (/* eq */) goto .L_800D8478;
    r3 = r0;
    fn_800E27B0();
    /* subi r0, r31, 0x1 */;
    *(u32*)lbl_8047AABC = r3;
    r0 = r0 * 0x30;
    r3 = r3 + r0;
    *(u32*)lbl_8047AAC4 = r3;
    *(u32*)lbl_8047AAC0 = r3;
    fn_800E064C();
    r4 = (u32)lbl_80314610;
    r3 = *(u32*)lbl_8047AAC0;
    r4 = (u32)lbl_80314610;
    r31 = *(u32*)((u8*)r4 + 0x24);
    r4 = r31;
    fn_800BD4B4();
    r3 = *(u32*)lbl_8047AAC0;
    r4 = r31;
    fn_800BD504();
    r3 = r31;
    fn_800BD554();
    r3 = (u32)lbl_80400948;
    r4 = *(u32*)lbl_8047AAC0;
    r3 = (u32)lbl_80400948;
    r3 = r3 + 0x1b0;
    fn_800E0628();
    r0 = 0x0;
    *(u8*)lbl_8047AAC8 = r0;
.L_800D8478:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800D848C | Size: 0x148 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D848C(void) {
    r8 = r6;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D84E4;
    r5 = r29;
    r6 = r30;
    r7 = r31;
    r3 = 0x27;
    r4 = 0x13;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D85B8;
.L_800D84E4:
    /* cmpwi r30, 0x0 */;
    if (/* ne */) goto .L_800D8524;
    r4 = (u32)lbl_80314404;
    r3 = (u32)lbl_80314454;
    r6 = r29 << 2;
    r0 = r31 << 2;
    r4 = (u32)lbl_80314404;
    r5 = (u32)lbl_80314454;
    /* lwzx r3, r4, r6 */;
    r4 = 0x1;
    /* lwzx r5, r5, r0 */;
    r6 = 0x3c;
    r7 = 0x0;
    r8 = 0x7d;
    fn_800B857C();
    goto .L_800D85B8;
.L_800D8524:
    /* cmpwi r30, 0x1 */;
    if (/* ne */) goto .L_800D854C;
    r3 = (u32)lbl_803144A8;
    r0 = r29 << 2;
    r4 = (u32)lbl_803144A8;
    r3 = r8;
    /* lwzx r4, r4, r0 */;
    r5 = 0x0;
    fn_800BD58C();
    goto .L_800D8570;
.L_800D854C:
    /* cmpwi r30, 0x2 */;
    if (/* ne */) goto .L_800D8570;
    r3 = (u32)lbl_803144A8;
    r0 = r29 << 2;
    r4 = (u32)lbl_803144A8;
    r3 = r8;
    /* lwzx r4, r4, r0 */;
    r5 = 0x1;
    fn_800BD58C();
.L_800D8570:
    r6 = (u32)lbl_80314404;
    r5 = (u32)lbl_80314424;
    r4 = (u32)lbl_80314454;
    r3 = (u32)lbl_803144A8;
    r10 = r29 << 2;
    r9 = (u32)lbl_80314404;
    r6 = (u32)lbl_803144A8;
    r8 = r30 << 2;
    r7 = (u32)lbl_80314424;
    r0 = r31 << 2;
    r5 = (u32)lbl_80314454;
    /* lwzx r3, r9, r10 */;
    /* lwzx r4, r7, r8 */;
    r7 = 0x0;
    /* lwzx r5, r5, r0 */;
    r8 = 0x7d;
    /* lwzx r6, r6, r10 */;
    fn_800B857C();
.L_800D85B8:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D85D4 | Size: 0x1D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D85D4(void) {
    /* mr. r29, r4 */;
    if (/* eq */) goto .L_800D8790;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8620;
    r5 = r3;
    r6 = r29;
    r3 = 0x26;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D8790;
.L_800D8620:
    r31 = r3 << 2;
    r3 = r4 + r31;
    r0 = *(u32*)((u8*)r3 + 0x28);
    /* cmplw r0, r29 */;
    if (/* eq */) goto .L_800D8790;
    r0 = *(u8*)((u8*)r29 + 0x7);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D873C;
    r4 = *(u32*)((u8*)r29 + 0x10);
    r3 = (u32)lbl_80314530;
    r0 = *(u32*)((u8*)r29 + 0x14);
    r5 = (u32)lbl_80314530;
    r4 = r4 << 2;
    r3 = r29 + 0x54;
    r0 = r0 << 2;
    /* lwzx r4, r5, r4 */;
    /* lwzx r5, r5, r0 */;
    fn_800BAE34();
    r0 = *(u32*)((u8*)r29 + 0x20);
    /* cmpwi r0, 0x1 */;
    if (/* eq */) goto .L_800D86AC;
    if (/* ge */) goto .L_800D8684;
    /* cmpwi r0, 0x0 */;
    if (/* ge */) goto .L_800D8690;
    goto .L_800D86E0;
.L_800D8684:
    /* cmpwi r0, 0x3 */;
    if (/* ge */) goto .L_800D86E0;
    goto .L_800D86C8;
.L_800D8690:
    r0 = *(u32*)((u8*)r29 + 0x18);
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D86A4;
    r30 = 0x1;
    goto .L_800D86E0;
.L_800D86A4:
    r30 = 0x0;
    goto .L_800D86E0;
.L_800D86AC:
    r0 = *(u32*)((u8*)r29 + 0x18);
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D86C0;
    r30 = 0x3;
    goto .L_800D86E0;
.L_800D86C0:
    r30 = 0x2;
    goto .L_800D86E0;
.L_800D86C8:
    r0 = *(u32*)((u8*)r29 + 0x18);
    /* cmpwi r0, 0x2 */;
    if (/* ne */) goto .L_800D86DC;
    r30 = 0x5;
    goto .L_800D86E0;
.L_800D86DC:
    r30 = 0x4;
.L_800D86E0:
    r3 = *(u8*)((u8*)r29 + 0x5);
    r0 = (0x4330 << 16);
    r5 = *(u32*)((u8*)r29 + 0x1C);
    r4 = r30;
    /* subi r3, r3, 0x1 */;
    f1 = *(f32*)lbl_8047CA40;
    /* xoris r3, r3, 0x8000 */;
    r5 = 0x2 - r5;
    /* cntlzw r5, r5 */;
    f2 = *(f64*)lbl_8047CA48;
    f3 = f1;
    r3 = r29 + 0x54;
    r5 = (u32)r5 >> 5;
    r6 = 0x0;
    f0 = *(f64*)((u8*)r1 + 0x8);
    r7 = 0x0;
    r8 = 0x0;
    f2 = f0 - f2;
    fn_800BACA0();
    r0 = 0x0;
    *(u8*)((u8*)r29 + 0x7) = r0;
.L_800D873C:
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r31;
    *(u32*)((u8*)r3 + 0x28) = r29;
    r0 = *(u32*)((u8*)r29 + 0x48);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D8768;
    r4 = (u32)lbl_80314510;
    r3 = r29 + 0x74;
    r4 = (u32)lbl_80314510;
    /* lwzx r4, r4, r31 */;
    fn_800BB098();
.L_800D8768:
    r4 = (u32)lbl_803144F0;
    r3 = r29 + 0x54;
    r4 = (u32)lbl_803144F0;
    /* lwzx r4, r4, r31 */;
    fn_800BAFFC();
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x24);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x24) = r0;
.L_800D8790:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D87AC -- GSgfx_SetInternalMode | Size: 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D87AC(void) {
    /* rlwinm r0, r3, 0, 30, 30 */;
    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x414);
    r0 = r3 | r27;
    *(u32*)((u8*)r4 + 0x414) = r0;
    if (/* eq */) goto .L_800D8828;
    r3 = (u32)lbl_80314404;
    r30 = 0x0;
    r29 = (u32)lbl_80314404;
    r28 = 0x0;
    r31 = r30;
.L_800D87EC:
    r3 = *(u32*)lbl_8047AA80;
    r0 = r30 + 0x28;
    r5 = r28 + 0x4;
    r4 = 0x1;
    /* stwx r31, r3, r0 */;
    r6 = 0x3c;
    r7 = 0x0;
    r8 = 0x7d;
    r3 = *(u32*)((u8*)r29 + 0x0);
    fn_800B857C();
    r28 = r28 + 0x1;
    r29 = r29 + 0x4;
    /* cmpwi r28, 0x8 */;
    r30 = r30 + 0x4;
    if (/* lt */) goto .L_800D87EC;
.L_800D8828:
    /* rlwinm r0, r27, 0, 29, 29 */;
    if (/* eq */) goto .L_800D8878;
    r29 = 0x0;
    r31 = r29;
.L_800D8838:
    r3 = *(u32*)lbl_8047AA80;
    r4 = r29 + 0x25c;
    /* lbzx r0, r3, r4 */;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D8858;
    /* stbx r31, r3, r4 */;
    r3 = r29;
    fn_800BBC34();
.L_800D8858:
    r29 = r29 + 0x1;
    /* cmpwi r29, 0x10 */;
    if (/* lt */) goto .L_800D8838;
    r4 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    r3 = 0x0;
    *(u8*)((u8*)r4 + 0x3AC) = r0;
    fn_800BBC0C();
.L_800D8878:
    /* lmw r27, 0xc(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    return;
}
#pragma pop

/* fn_800D888C | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D888C(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D88C0;
    r5 = r3;
    r3 = 0x29;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D88CC;
.L_800D88C0:
    r0 = *(u32*)((u8*)r4 + 0x10);
    r0 = r0 & ~r3;
    *(u32*)((u8*)r4 + 0x10) = r0;
.L_800D88CC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D88DC | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D88DC(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D8910;
    r5 = r3;
    r3 = 0x28;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D891C;
.L_800D8910:
    r0 = *(u32*)((u8*)r4 + 0x10);
    r0 = r0 | r3;
    *(u32*)((u8*)r4 + 0x10) = r0;
.L_800D891C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D923C | Size: 0x400 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D923C(void) {
    /* stmw r21, 0x24(r1) */;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 & 0x1;
    if (/* eq */) goto .L_800D92E4;
    r3 = *(u8*)((u8*)r3 + 0x60);
    fn_800BA6B0();
    r23 = 0x0;
    r22 = 0x0;
    goto .L_800D92C0;
.L_800D9270:
    r24 = r22 + 0x61;
    r3 = r23;
    r24 = r4 + r24;
    r4 = *(u8*)((u8*)r24 + 0x0);
    r5 = *(u8*)((u8*)r24 + 0x1);
    r6 = *(u8*)((u8*)r24 + 0x2);
    r7 = *(u8*)((u8*)r24 + 0x3);
    r8 = *(u8*)((u8*)r24 + 0x4);
    r9 = *(u8*)((u8*)r24 + 0x5);
    fn_800BA6F4();
    r4 = *(u8*)((u8*)r24 + 0x6);
    r3 = r23 + 0x2;
    r5 = *(u8*)((u8*)r24 + 0x7);
    r6 = *(u8*)((u8*)r24 + 0x8);
    r7 = *(u8*)((u8*)r24 + 0x9);
    r8 = *(u8*)((u8*)r24 + 0xA);
    r9 = *(u8*)((u8*)r24 + 0xB);
    fn_800BA6F4();
    r22 = r22 + 0xc;
    r23 = r23 + 0x1;
.L_800D92C0:
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x60);
    /* cmpw r23, r0 */;
    if (/* lt */) goto .L_800D9270;
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x18);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x18) = r0;
.L_800D92E4:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    /* rlwinm r0, r0, 0, 30, 30 */;
    if (/* eq */) goto .L_800D9310;
    r3 = *(u8*)((u8*)r3 + 0x79);
    fn_800B884C();
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x1C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x1C) = r0;
.L_800D9310:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    /* rlwinm r0, r0, 0, 29, 29 */;
    if (/* eq */) goto .L_800D961C;
    r3 = *(u8*)((u8*)r3 + 0x7A);
    fn_800BC8C8();
    r4 = *(u32*)lbl_8047AA80;
    r23 = *(u8*)((u8*)r4 + 0x3AC);
    r29 = r4 + 0x7b;
    r28 = r4 + 0xab;
    r27 = r4 + 0xfb;
    r3 = r23;
    r26 = r4 + 0x14b;
    r25 = r4 + 0x18b;
    r24 = r4 + 0x25c;
    r22 = r4 + 0x26c;
    fn_800BBC0C();
    /* cmplwi r23, 0x0 */;
    if (/* eq */) goto .L_800D93C4;
    r3 = *(u32*)lbl_8047AA80;
    r30 = 0x0;
    r31 = r3 + 0x3ad;
    r21 = r3 + 0x3c0;
    goto .L_800D9398;
.L_800D9370:
    r4 = *(u8*)((u8*)r31 + 0x0);
    r3 = r30;
    r5 = *(u8*)((u8*)r31 + 0x1);
    fn_800BBAF8();
    r4 = *(u8*)((u8*)r31 + 0x2);
    r3 = r30;
    r5 = *(u8*)((u8*)r31 + 0x3);
    fn_800BB97C();
    r31 = r31 + 0x4;
    r30 = r30 + 0x1;
.L_800D9398:
    /* cmpw r30, r23 */;
    if (/* lt */) goto .L_800D9370;
    r30 = 0x0;
.L_800D93A4:
    r5 = *(u8*)((u8*)r21 + 0x18);
    r4 = r21;
    r3 = r30 + 0x1;
    fn_800BB81C();
    r30 = r30 + 0x1;
    r21 = r21 + 0x1c;
    /* cmpwi r30, 0x3 */;
    if (/* lt */) goto .L_800D93A4;
.L_800D93C4:
    r30 = 0x0;
    r31 = r30;
    goto .L_800D95AC;
.L_800D93D0:
    r4 = *(u8*)((u8*)r29 + 0x0);
    r3 = r30;
    r5 = *(u8*)((u8*)r29 + 0x1);
    r6 = *(u8*)((u8*)r29 + 0x2);
    fn_800BC6F0();
    r4 = *(u8*)((u8*)r28 + 0x0);
    r3 = r30;
    r5 = *(u8*)((u8*)r28 + 0x1);
    r6 = *(u8*)((u8*)r28 + 0x2);
    r7 = *(u8*)((u8*)r28 + 0x3);
    r8 = *(u8*)((u8*)r28 + 0x4);
    fn_800BC228();
    r4 = *(u8*)((u8*)r27 + 0x0);
    r3 = r30;
    r5 = *(u8*)((u8*)r27 + 0x1);
    r6 = *(u8*)((u8*)r27 + 0x2);
    r7 = *(u8*)((u8*)r27 + 0x3);
    r8 = *(u8*)((u8*)r27 + 0x4);
    fn_800BC290();
    r4 = *(u8*)((u8*)r26 + 0x0);
    r3 = r30;
    r5 = *(u8*)((u8*)r26 + 0x1);
    r6 = *(u8*)((u8*)r26 + 0x2);
    r7 = *(u8*)((u8*)r26 + 0x3);
    fn_800BC1A0();
    r4 = *(u8*)((u8*)r25 + 0x0);
    r3 = r30;
    r5 = *(u8*)((u8*)r25 + 0x1);
    r6 = *(u8*)((u8*)r25 + 0x2);
    r7 = *(u8*)((u8*)r25 + 0x3);
    fn_800BC1E4();
    r4 = *(u32*)lbl_8047AA80;
    r0 = r31 + 0x1cc;
    r3 = r30;
    /* lwzx r4, r4, r0 */;
    fn_800BC454();
    r4 = *(u32*)lbl_8047AA80;
    r0 = r31 + 0x20c;
    r3 = r30;
    /* lwzx r4, r4, r0 */;
    fn_800BC4C0();
    r0 = *(u8*)((u8*)r24 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800D9580;
    /* cmplwi r23, 0x0 */;
    if (/* eq */) goto .L_800D9580;
    r0 = *(u8*)((u8*)r22 + 0x0);
    /* cmpwi r0, 0x3 */;
    if (/* eq */) goto .L_800D954C;
    if (/* ge */) goto .L_800D94B0;
    /* cmpwi r0, 0x1 */;
    if (/* eq */) goto .L_800D94F8;
    if (/* ge */) goto .L_800D9514;
    /* cmpwi r0, 0x0 */;
    if (/* ge */) goto .L_800D94C0;
    goto .L_800D9588;
.L_800D94B0:
    /* cmpwi r0, 0x5 */;
    if (/* eq */) goto .L_800D9574;
    if (/* ge */) goto .L_800D9588;
    goto .L_800D9560;
.L_800D94C0:
    r0 = *(u8*)((u8*)r22 + 0x9);
    r3 = r30;
    r0 = *(u8*)((u8*)r22 + 0x5);
    r4 = *(u8*)((u8*)r22 + 0x1);
    r5 = *(u8*)((u8*)r22 + 0x3);
    r6 = *(u8*)((u8*)r22 + 0x4);
    r7 = *(u8*)((u8*)r22 + 0x2);
    r8 = *(u8*)((u8*)r22 + 0x6);
    r9 = *(u8*)((u8*)r22 + 0x7);
    r10 = *(u8*)((u8*)r22 + 0x8);
    fn_800BB780();
    goto .L_800D9588;
.L_800D94F8:
    r4 = *(u8*)((u8*)r22 + 0x1);
    r3 = r30;
    r5 = *(u8*)((u8*)r22 + 0xA);
    r6 = *(u8*)((u8*)r22 + 0xB);
    r7 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBC7C();
    goto .L_800D9588;
.L_800D9514:
    r0 = *(u8*)((u8*)r22 + 0x4);
    r3 = r30;
    r0 = *(u8*)((u8*)r22 + 0x5);
    r4 = *(u8*)((u8*)r22 + 0x1);
    r5 = *(u16*)((u8*)r22 + 0xC);
    r6 = *(u16*)((u8*)r22 + 0xE);
    r7 = *(u16*)((u8*)r22 + 0x10);
    r8 = *(u16*)((u8*)r22 + 0x12);
    r9 = *(u8*)((u8*)r22 + 0x3);
    r10 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBCE0();
    goto .L_800D9588;
.L_800D954C:
    r4 = *(u8*)((u8*)r22 + 0x1);
    r3 = r30;
    r5 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBE8C();
    goto .L_800D9588;
.L_800D9560:
    r4 = *(u8*)((u8*)r22 + 0x1);
    r3 = r30;
    r5 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBF98();
    goto .L_800D9588;
.L_800D9574:
    r3 = r30;
    fn_800BBFDC();
    goto .L_800D9588;
.L_800D9580:
    r3 = r30;
    fn_800BBC34();
.L_800D9588:
    r29 = r29 + 0x3;
    r28 = r28 + 0x5;
    r27 = r27 + 0x5;
    r26 = r26 + 0x4;
    r25 = r25 + 0x4;
    r24 = r24 + 0x1;
    r22 = r22 + 0x14;
    r31 = r31 + 0x4;
    r30 = r30 + 0x1;
.L_800D95AC:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x7A);
    /* cmpw r30, r0 */;
    if (/* lt */) goto .L_800D93D0;
    r23 = 0x0;
    r22 = r23;
.L_800D95C4:
    r5 = *(u32*)lbl_8047AA80;
    r0 = r22 + 0x24c;
    r3 = r23;
    r4 = r1 + 0x10;
    /* lwzx r0, r5, r0 */;
    fn_800BC3E0();
    r23 = r23 + 0x1;
    r22 = r22 + 0x4;
    /* cmpwi r23, 0x4 */;
    if (/* lt */) goto .L_800D95C4;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x1;
    r6 = 0x2;
    r7 = 0x3;
    fn_800BC580();
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x20);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x20) = r0;
.L_800D961C:
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x414) = r0;
    /* lmw r21, 0x24(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x54);
    return;
}
#pragma pop

/* fn_800D963C | Size: 0x4B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D963C(void) {
    /* cmpwi r4, 0x2 */;
    if (/* eq */) goto .L_800D9928;
    if (/* ge */) goto .L_800D9668;
    /* cmpwi r4, 0x0 */;
    if (/* eq */) goto .L_800D9678;
    if (/* ge */) goto .L_800D975C;
    goto .L_800D9ADC;
.L_800D9668:
    /* cmpwi r4, 0x4 */;
    if (/* eq */) goto .L_800D9A04;
    if (/* ge */) goto .L_800D9ADC;
    goto .L_800D9840;
.L_800D9678:
    r4 = r3 * 0x5;
    r3 = r3 << 2;
    r0 = *(u32*)lbl_8047AA80;
    r11 = 0x0;
    r10 = 0x1;
    r8 = r4 + 0xab;
    r8 = r0 + r8;
    r12 = r3 + 0x14b;
    *(u8*)((u8*)r8 + 0x0) = r11;
    r7 = 0xf;
    r6 = 0xa;
    r5 = 0x8;
    *(u8*)((u8*)r8 + 0x1) = r11;
    r31 = r4 + 0xfb;
    r30 = r3 + 0x18b;
    r4 = 0x7;
    *(u8*)((u8*)r8 + 0x2) = r11;
    r3 = 0x4;
    r0 = 0x5;
    *(u8*)((u8*)r8 + 0x3) = r10;
    *(u8*)((u8*)r8 + 0x4) = r11;
    r9 = *(u32*)lbl_8047AA80;
    r8 = *(u32*)((u8*)r9 + 0x414);
    r8 = r8 | 0x4;
    *(u32*)((u8*)r9 + 0x414) = r8;
    r8 = *(u32*)lbl_8047AA80;
    r12 = r8 + r12;
    *(u8*)((u8*)r12 + 0x0) = r7;
    *(u8*)((u8*)r12 + 0x1) = r6;
    *(u8*)((u8*)r12 + 0x2) = r5;
    *(u8*)((u8*)r12 + 0x3) = r7;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x414);
    r5 = r5 | 0x4;
    *(u32*)((u8*)r6 + 0x414) = r5;
    r5 = *(u32*)lbl_8047AA80;
    r31 = r5 + r31;
    *(u8*)((u8*)r31 + 0x0) = r11;
    *(u8*)((u8*)r31 + 0x1) = r11;
    *(u8*)((u8*)r31 + 0x2) = r11;
    *(u8*)((u8*)r31 + 0x3) = r10;
    *(u8*)((u8*)r31 + 0x4) = r11;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x414);
    r5 = r5 | 0x4;
    *(u32*)((u8*)r6 + 0x414) = r5;
    r5 = *(u32*)lbl_8047AA80;
    r30 = r5 + r30;
    *(u8*)((u8*)r30 + 0x0) = r4;
    *(u8*)((u8*)r30 + 0x1) = r3;
    *(u8*)((u8*)r30 + 0x2) = r0;
    *(u8*)((u8*)r30 + 0x3) = r4;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    goto .L_800D9ADC;
.L_800D975C:
    r9 = r3 * 0x5;
    r3 = r3 << 2;
    r0 = *(u32*)lbl_8047AA80;
    r11 = 0x0;
    r10 = 0x1;
    r8 = r9 + 0xab;
    r8 = r0 + r8;
    r12 = r3 + 0x14b;
    *(u8*)((u8*)r8 + 0x0) = r11;
    r7 = 0xa;
    r6 = 0x8;
    r5 = 0x9;
    *(u8*)((u8*)r8 + 0x1) = r11;
    r4 = 0xf;
    r30 = r9 + 0xfb;
    r31 = r3 + 0x18b;
    *(u8*)((u8*)r8 + 0x2) = r11;
    r3 = 0x7;
    r0 = 0x5;
    *(u8*)((u8*)r8 + 0x3) = r10;
    *(u8*)((u8*)r8 + 0x4) = r11;
    r9 = *(u32*)lbl_8047AA80;
    r8 = *(u32*)((u8*)r9 + 0x414);
    r8 = r8 | 0x4;
    *(u32*)((u8*)r9 + 0x414) = r8;
    r8 = *(u32*)lbl_8047AA80;
    r12 = r8 + r12;
    *(u8*)((u8*)r12 + 0x0) = r7;
    *(u8*)((u8*)r12 + 0x1) = r6;
    *(u8*)((u8*)r12 + 0x2) = r5;
    *(u8*)((u8*)r12 + 0x3) = r4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r4;
    r4 = *(u32*)lbl_8047AA80;
    r30 = r4 + r30;
    *(u8*)((u8*)r30 + 0x0) = r11;
    *(u8*)((u8*)r30 + 0x1) = r11;
    *(u8*)((u8*)r30 + 0x2) = r11;
    *(u8*)((u8*)r30 + 0x3) = r10;
    *(u8*)((u8*)r30 + 0x4) = r11;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r4;
    r4 = *(u32*)lbl_8047AA80;
    r31 = r4 + r31;
    *(u8*)((u8*)r31 + 0x0) = r3;
    *(u8*)((u8*)r31 + 0x1) = r3;
    *(u8*)((u8*)r31 + 0x2) = r3;
    *(u8*)((u8*)r31 + 0x3) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    goto .L_800D9ADC;
.L_800D9840:
    r4 = r3 * 0x5;
    r3 = r3 << 2;
    r0 = *(u32*)lbl_8047AA80;
    r12 = 0x0;
    r11 = 0x1;
    r9 = r4 + 0xab;
    r9 = r0 + r9;
    r30 = r3 + 0x14b;
    *(u8*)((u8*)r9 + 0x0) = r12;
    r8 = 0xa;
    r7 = 0xc;
    r6 = 0x8;
    *(u8*)((u8*)r9 + 0x1) = r12;
    r5 = 0xf;
    r31 = r4 + 0xfb;
    r29 = r3 + 0x18b;
    *(u8*)((u8*)r9 + 0x2) = r12;
    r4 = 0x7;
    r3 = 0x5;
    r0 = 0x4;
    *(u8*)((u8*)r9 + 0x3) = r11;
    *(u8*)((u8*)r9 + 0x4) = r12;
    r10 = *(u32*)lbl_8047AA80;
    r9 = *(u32*)((u8*)r10 + 0x414);
    r9 = r9 | 0x4;
    *(u32*)((u8*)r10 + 0x414) = r9;
    r9 = *(u32*)lbl_8047AA80;
    r30 = r9 + r30;
    *(u8*)((u8*)r30 + 0x0) = r8;
    *(u8*)((u8*)r30 + 0x1) = r7;
    *(u8*)((u8*)r30 + 0x2) = r6;
    *(u8*)((u8*)r30 + 0x3) = r5;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x414);
    r5 = r5 | 0x4;
    *(u32*)((u8*)r6 + 0x414) = r5;
    r5 = *(u32*)lbl_8047AA80;
    r31 = r5 + r31;
    *(u8*)((u8*)r31 + 0x0) = r12;
    *(u8*)((u8*)r31 + 0x1) = r12;
    *(u8*)((u8*)r31 + 0x2) = r12;
    *(u8*)((u8*)r31 + 0x3) = r11;
    *(u8*)((u8*)r31 + 0x4) = r12;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x414);
    r5 = r5 | 0x4;
    *(u32*)((u8*)r6 + 0x414) = r5;
    r5 = *(u32*)lbl_8047AA80;
    r29 = r5 + r29;
    *(u8*)((u8*)r29 + 0x0) = r4;
    *(u8*)((u8*)r29 + 0x1) = r3;
    *(u8*)((u8*)r29 + 0x2) = r0;
    *(u8*)((u8*)r29 + 0x3) = r4;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    goto .L_800D9ADC;
.L_800D9928:
    r7 = r3 * 0x5;
    r3 = r3 << 2;
    r0 = *(u32*)lbl_8047AA80;
    r9 = 0x0;
    r8 = 0x1;
    r6 = r7 + 0xab;
    r6 = r0 + r6;
    r10 = r3 + 0x14b;
    *(u8*)((u8*)r6 + 0x0) = r9;
    r5 = 0xf;
    r4 = 0x8;
    r11 = r7 + 0xfb;
    *(u8*)((u8*)r6 + 0x1) = r9;
    r12 = r3 + 0x18b;
    r3 = 0x7;
    r0 = 0x4;
    *(u8*)((u8*)r6 + 0x2) = r9;
    *(u8*)((u8*)r6 + 0x3) = r8;
    *(u8*)((u8*)r6 + 0x4) = r9;
    r7 = *(u32*)lbl_8047AA80;
    r6 = *(u32*)((u8*)r7 + 0x414);
    r6 = r6 | 0x4;
    *(u32*)((u8*)r7 + 0x414) = r6;
    r6 = *(u32*)lbl_8047AA80;
    r10 = r6 + r10;
    *(u8*)((u8*)r10 + 0x0) = r5;
    *(u8*)((u8*)r10 + 0x1) = r5;
    *(u8*)((u8*)r10 + 0x2) = r5;
    *(u8*)((u8*)r10 + 0x3) = r4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r4;
    r4 = *(u32*)lbl_8047AA80;
    r11 = r4 + r11;
    *(u8*)((u8*)r11 + 0x0) = r9;
    *(u8*)((u8*)r11 + 0x1) = r9;
    *(u8*)((u8*)r11 + 0x2) = r9;
    *(u8*)((u8*)r11 + 0x3) = r8;
    *(u8*)((u8*)r11 + 0x4) = r9;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r4;
    r4 = *(u32*)lbl_8047AA80;
    r12 = r4 + r12;
    *(u8*)((u8*)r12 + 0x0) = r3;
    *(u8*)((u8*)r12 + 0x1) = r3;
    *(u8*)((u8*)r12 + 0x2) = r3;
    *(u8*)((u8*)r12 + 0x3) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    goto .L_800D9ADC;
.L_800D9A04:
    r7 = r3 * 0x5;
    r3 = r3 << 2;
    r0 = *(u32*)lbl_8047AA80;
    r9 = 0x0;
    r8 = 0x1;
    r6 = r7 + 0xab;
    r6 = r0 + r6;
    r10 = r3 + 0x14b;
    *(u8*)((u8*)r6 + 0x0) = r9;
    r5 = 0xf;
    r4 = 0xa;
    r11 = r7 + 0xfb;
    *(u8*)((u8*)r6 + 0x1) = r9;
    r12 = r3 + 0x18b;
    r3 = 0x7;
    r0 = 0x5;
    *(u8*)((u8*)r6 + 0x2) = r9;
    *(u8*)((u8*)r6 + 0x3) = r8;
    *(u8*)((u8*)r6 + 0x4) = r9;
    r7 = *(u32*)lbl_8047AA80;
    r6 = *(u32*)((u8*)r7 + 0x414);
    r6 = r6 | 0x4;
    *(u32*)((u8*)r7 + 0x414) = r6;
    r6 = *(u32*)lbl_8047AA80;
    r10 = r6 + r10;
    *(u8*)((u8*)r10 + 0x0) = r5;
    *(u8*)((u8*)r10 + 0x1) = r5;
    *(u8*)((u8*)r10 + 0x2) = r5;
    *(u8*)((u8*)r10 + 0x3) = r4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r4;
    r4 = *(u32*)lbl_8047AA80;
    r11 = r4 + r11;
    *(u8*)((u8*)r11 + 0x0) = r9;
    *(u8*)((u8*)r11 + 0x1) = r9;
    *(u8*)((u8*)r11 + 0x2) = r9;
    *(u8*)((u8*)r11 + 0x3) = r8;
    *(u8*)((u8*)r11 + 0x4) = r9;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x4;
    *(u32*)((u8*)r5 + 0x414) = r4;
    r4 = *(u32*)lbl_8047AA80;
    r12 = r4 + r12;
    *(u8*)((u8*)r12 + 0x0) = r3;
    *(u8*)((u8*)r12 + 0x1) = r3;
    *(u8*)((u8*)r12 + 0x2) = r3;
    *(u8*)((u8*)r12 + 0x3) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
.L_800D9ADC:
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r1 = r1 + 0x20;
    return;
}
#pragma pop

/* fn_800D9AF0 | Size: 0x34 */
void fn_800D9AF0(void) {
    *(u16*)((u8*)r3 + 0x0) = r0;
    *(u16*)((u8*)r4 + 0x0) = r0;
    *(u16*)((u8*)r5 + 0x0) = r0;
    *(u16*)((u8*)r6 + 0x0) = r0;
}

/* fn_800D9B24 | Size: 0x34 */
void fn_800D9B24(void) {
    *(u16*)((u8*)r3 + 0x0) = r0;
    *(u16*)((u8*)r4 + 0x0) = r0;
    *(u16*)((u8*)r5 + 0x0) = r0;
    *(u16*)((u8*)r6 + 0x0) = r0;
}

/* fn_800D9B58 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9B58(void) {
    f7 = f1;
    f0 = f3;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9B90;
    r3 = 0x3a;
    r4 = 0xe;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D9BC0;
.L_800D9B90:
    f1 = f2;
    f5 = *(f32*)lbl_8047CA50;
    f2 = f4;
    f6 = *(f32*)lbl_8047CA54;
    f3 = f7;
    r3 = r1 + 0x8;
    f4 = f0;
    fn_800E0698();
    r3 = r1 + 0x8;
    r4 = 0x1;
    fn_800BD2E0();
    fn_800D834C();
.L_800D9BC0:
    r0 = *(u32*)((u8*)r1 + 0x54);
    return;
}
#pragma pop

/* fn_800D9BD0 | Size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9BD0(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9C00;
    r3 = 0x39;
    r4 = 0xe;
    /* crset cr1eq */;
    fn_800D4F98();
    goto .L_800D9C14;
.L_800D9C00:
    r3 = r1 + 0x8;
    fn_800E0678();
    r3 = r1 + 0x8;
    r4 = 0x0;
    fn_800BD2E0();
.L_800D9C14:
    r0 = *(u32*)((u8*)r1 + 0x54);
    return;
}
#pragma pop

/* fn_800D9C24 -- GSgfx_SetViewportRect | Size: 0x144 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9C24(void) {
    r31 = r6;
    r30 = r5;
    r29 = r4;
    r28 = r3;
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9C84;
    r5 = r28 & 0xFFFF;
    r6 = r29 & 0xFFFF;
    r7 = r30 & 0xFFFF;
    r8 = r31 & 0xFFFF;
    r3 = 0x38;
    r4 = 0x4;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D9D48;
.L_800D9C84:
    r7 = r28 & 0xFFFF;
    r5 = r29 & 0xFFFF;
    r3 = r30 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    r4 = r3 - r7;
    r6 = (0x4330 << 16);
    r3 = r0 - r5;
    r4 = r4 + 0x1;
    f2 = *(f64*)lbl_8047CA60;
    r0 = r3 + 0x1;
    /* xoris r4, r4, 0x8000 */;
    f4 = *(f64*)lbl_8047CA68;
    /* xoris r0, r0, 0x8000 */;
    f0 = *(f64*)((u8*)r1 + 0x8);
    r3 = 0x0;
    f1 = f0 - f2;
    f5 = *(f32*)lbl_8047CA50;
    f6 = *(f32*)lbl_8047CA58;
    f0 = *(f64*)((u8*)r1 + 0x10);
    f2 = f0 - f2;
    f0 = *(f64*)((u8*)r1 + 0x18);
    f3 = f0 - f4;
    f0 = *(f64*)((u8*)r1 + 0x20);
    f4 = f0 - f4;
    fn_800D3EC4();
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x476) = r28;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x478) = r29;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x47A) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x47C) = r31;
    fn_800D2584();
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800D9D48;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = r31;
    fn_800D21C8();
.L_800D9D48:
    r0 = *(u32*)((u8*)r1 + 0x44);
    r31 = *(u32*)((u8*)r1 + 0x3C);
    r30 = *(u32*)((u8*)r1 + 0x38);
    r29 = *(u32*)((u8*)r1 + 0x34);
    r28 = *(u32*)((u8*)r1 + 0x30);
    return;
}
#pragma pop

/* fn_800D9D68 -- GSgfx_SetScissor | Size: 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9D68(void) {
    r31 = r6;
    r30 = r5;
    r29 = r4;
    r28 = r3;
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r7 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9DC8;
    r5 = r28 & 0xFFFF;
    r6 = r29 & 0xFFFF;
    r7 = r30 & 0xFFFF;
    r8 = r31 & 0xFFFF;
    r3 = 0x37;
    r4 = 0x4;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D9E2C;
.L_800D9DC8:
    r3 = r28 & 0xFFFF;
    r0 = r30 & 0xFFFF;
    r5 = r0 - r3;
    r4 = r29 & 0xFFFF;
    r0 = r31 & 0xFFFF;
    r6 = r0 - r4;
    r5 = r5 + 0x1;
    r6 = r6 + 0x1;
    fn_800BD7A0();
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x46E) = r28;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x470) = r29;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x472) = r30;
    r3 = *(u32*)lbl_8047AA80;
    *(u16*)((u8*)r3 + 0x474) = r31;
    fn_800D2584();
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800D9E2C;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = r31;
    fn_800D2150();
.L_800D9E2C:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r28 = *(u32*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800D9E4C | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9E4C(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9E80;
    r5 = r3;
    r3 = 0x36;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D9EC8;
.L_800D9E80:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800D9E94;
    r0 = 0x1;
    *(u8*)((u8*)r4 + 0x5D) = r0;
    goto .L_800D9EA4;
.L_800D9E94:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800D9EA4;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0x5D) = r0;
.L_800D9EA4:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x5D);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9EC0;
    r3 = *(u32*)lbl_8047AA8C;
    fn_8019BD18();
    goto .L_800D9EC8;
.L_800D9EC0:
    r3 = 0x0;
    fn_8019BD18();
.L_800D9EC8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D9ED8 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9ED8(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9F0C;
    r5 = r3;
    r3 = 0x35;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D9F30;
.L_800D9F0C:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800D9F20;
    r0 = 0x1;
    *(u8*)((u8*)r4 + 0x1A) = r0;
    goto .L_800D9F30;
.L_800D9F20:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800D9F30;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0x1A) = r0;
.L_800D9F30:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D9F40 -- GSgfx_ConfigureFog | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9F40(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9F74;
    r5 = r3;
    r3 = 0x34;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800D9FA4;
.L_800D9F74:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800D9F88;
    r0 = 0x1;
    *(u8*)((u8*)r4 + 0x42D) = r0;
    goto .L_800D9F98;
.L_800D9F88:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800D9F98;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0x42D) = r0;
.L_800D9F98:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x42D);
    fn_800B953C();
.L_800D9FA4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800D9FB4 | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D9FB4(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800D9FE8;
    r5 = r3;
    r3 = 0x33;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA018;
.L_800D9FE8:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800D9FFC;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0x42C) = r0;
    goto .L_800DA00C;
.L_800D9FFC:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA00C;
    r0 = 0x1;
    *(u8*)((u8*)r4 + 0x42C) = r0;
.L_800DA00C:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x42C);
    fn_800BD870();
.L_800DA018:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA028 -- GSgfx_ConfigureTEV | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA028(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA05C;
    r5 = r3;
    r3 = 0x32;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA07C;
.L_800DA05C:
    r4 = (u32)lbl_8031453C;
    r0 = r3 << 2;
    r3 = (u32)lbl_8031453C;
    /* lwzx r0, r3, r0 */;
    *(u32*)((u8*)r5 + 0x428) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x428);
    fn_800B94F0();
.L_800DA07C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA08C | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA08C(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA0C0;
    r5 = r3;
    r3 = 0x31;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA0F0;
.L_800DA0C0:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800DA0D4;
    r0 = 0x1;
    *(u8*)((u8*)r4 + 0x426) = r0;
    goto .L_800DA0E4;
.L_800DA0D4:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA0E4;
    r0 = 0x0;
    *(u8*)((u8*)r4 + 0x426) = r0;
.L_800DA0E4:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x426);
    fn_800BCFDC();
.L_800DA0F0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA100 -- GSgfx_ConfigureAlpha | Size: 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA100(void) {
    r12 = r5;
    r11 = r6;
    r10 = r8;
    r9 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r9 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA154;
    r5 = r3;
    r6 = r4;
    r8 = r11;
    r9 = r7;
    r7 = r12 & 0xFF;
    r10 = r10 & 0xFF;
    r3 = 0x30;
    r4 = 0x6;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA1D8;
.L_800DA154:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800DA168;
    r0 = 0x1;
    *(u8*)((u8*)r9 + 0x425) = r0;
    goto .L_800DA178;
.L_800DA168:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA178;
    r0 = 0x0;
    *(u8*)((u8*)r9 + 0x425) = r0;
.L_800DA178:
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x425);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DA1A4;
    r3 = 0x7;
    r4 = 0x0;
    r5 = 0x1;
    r6 = 0x7;
    r7 = 0x0;
    fn_800BC618();
    goto .L_800DA1D8;
.L_800DA1A4:
    r6 = (u32)lbl_8031457C;
    r3 = (u32)lbl_8031456C;
    r8 = r4 << 2;
    r5 = r11 << 2;
    r6 = (u32)lbl_8031457C;
    r4 = (u32)lbl_8031456C;
    r0 = r7 << 2;
    /* lwzx r3, r6, r8 */;
    /* lwzx r5, r4, r5 */;
    r4 = r12;
    /* lwzx r6, r6, r0 */;
    r7 = r10;
    fn_800BC618();
.L_800DA1D8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA1E8 -- GSgfx_ConfigureZ | Size: 0xD4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA1E8(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA224;
    r5 = r3;
    r6 = r4;
    r3 = 0x2f;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA2AC;
.L_800DA224:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800DA238;
    r0 = 0x1;
    *(u8*)((u8*)r6 + 0x41C) = r0;
    goto .L_800DA248;
.L_800DA238:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA248;
    r0 = 0x0;
    *(u8*)((u8*)r6 + 0x41C) = r0;
.L_800DA248:
    /* cmpwi r7, 0x1 */;
    if (/* ne */) goto .L_800DA260;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x424) = r0;
    goto .L_800DA274;
.L_800DA260:
    /* cmpwi r7, 0x0 */;
    if (/* ne */) goto .L_800DA274;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x424) = r0;
.L_800DA274:
    r3 = (u32)lbl_8031454C;
    r0 = r4 << 2;
    r4 = (u32)lbl_8031454C;
    r3 = *(u32*)lbl_8047AA80;
    /* lwzx r0, r4, r0 */;
    *(u32*)((u8*)r3 + 0x420) = r0;
    r5 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r5 + 0x41C);
    r4 = *(u32*)((u8*)r5 + 0x420);
    r5 = *(u8*)((u8*)r5 + 0x41B);
    fn_800BCE88();
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x424);
    fn_800BCEBC();
.L_800DA2AC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA2BC -- GSgfx_ConfigureBlend | Size: 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA2BC(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA2F8;
    r5 = r3;
    r6 = r4;
    r3 = 0x2e;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA3A0;
.L_800DA2F8:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800DA30C;
    r0 = 0x1;
    *(u8*)((u8*)r6 + 0x419) = r0;
    goto .L_800DA31C;
.L_800DA30C:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA31C;
    r0 = 0x0;
    *(u8*)((u8*)r6 + 0x419) = r0;
.L_800DA31C:
    /* cmpwi r4, 0x1 */;
    if (/* ne */) goto .L_800DA334;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x41A) = r0;
    goto .L_800DA348;
.L_800DA334:
    /* cmpwi r4, 0x0 */;
    if (/* ne */) goto .L_800DA348;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x41A) = r0;
.L_800DA348:
    /* cmpwi r7, 0x1 */;
    if (/* ne */) goto .L_800DA360;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x41B) = r0;
    goto .L_800DA374;
.L_800DA360:
    /* cmpwi r7, 0x0 */;
    if (/* ne */) goto .L_800DA374;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x41B) = r0;
.L_800DA374:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x419);
    fn_800BCE30();
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x41A);
    fn_800BCE5C();
    r5 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r5 + 0x41C);
    r4 = *(u32*)((u8*)r5 + 0x420);
    r5 = *(u8*)((u8*)r5 + 0x41B);
    fn_800BCE88();
.L_800DA3A0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA3B0 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA3B0(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA3E8;
    r5 = r3;
    r6 = r4 & 0xFF;
    r3 = 0x2d;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA418;
.L_800DA3E8:
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800DA3FC;
    r0 = 0x1;
    *(u8*)((u8*)r5 + 0x418) = r0;
    goto .L_800DA40C;
.L_800DA3FC:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA40C;
    r0 = 0x0;
    *(u8*)((u8*)r5 + 0x418) = r0;
.L_800DA40C:
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x418);
    fn_800BD008();
.L_800DA418:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA428 | Size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA428(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA45C;
    r5 = r3;
    r3 = 0x2c;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA4B4;
.L_800DA45C:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA488;
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0x5;
    fn_800BCDDC();
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x10;
    *(u32*)((u8*)r3 + 0x8) = r0;
    goto .L_800DA4B4;
.L_800DA488:
    r4 = (u32)lbl_803145D0;
    r0 = r3 << 2;
    r4 = (u32)lbl_803145D0;
    r3 = 0x2;
    /* lwzx r6, r4, r0 */;
    r4 = 0x1;
    r5 = 0x1;
    fn_800BCDDC();
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x2000;
    *(u32*)((u8*)r3 + 0x8) = r0;
.L_800DA4B4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA4C4 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA4C4(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DA500;
    r5 = r3;
    r6 = r4;
    r3 = 0x2b;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DA568;
.L_800DA500:
    /* cmpwi r3, 0x0 */;
    if (/* ne */) goto .L_800DA52C;
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0x5;
    fn_800BCDDC();
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x10;
    *(u32*)((u8*)r3 + 0x8) = r0;
    goto .L_800DA568;
.L_800DA52C:
    r6 = (u32)lbl_8031459C;
    r5 = (u32)lbl_803145A8;
    r8 = r3 << 2;
    r4 = r4 << 2;
    r3 = (u32)lbl_8031459C;
    r5 = (u32)lbl_803145A8;
    r0 = r7 << 2;
    /* lwzx r3, r3, r8 */;
    /* lwzx r4, r5, r4 */;
    r6 = 0x5;
    /* lwzx r5, r5, r0 */;
    fn_800BCDDC();
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x2000;
    *(u32*)((u8*)r3 + 0x8) = r0;
.L_800DA568:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DA578 | Size: 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA578(void) {
    r8 = r4;
    /* stmw r27, 0xc(r1) */;
    r27 = r3;
    r28 = r6;
    r29 = r7;
    r30 = r8 + r5;
    goto .L_800DA6D4;
.L_800DA5A0:
    r0 = *(u8*)((u8*)r8 + 0x0);
    r8 = r8 + 0x1;
    /* rlwinm r0, r0, 0, 24, 28 */;
    /* cmpwi r0, 0x98 */;
    if (/* eq */) goto .L_800DA618;
    if (/* ge */) goto .L_800DA5E8;
    /* cmpwi r0, 0x80 */;
    if (/* eq */) goto .L_800DA618;
    if (/* ge */) goto .L_800DA5DC;
    /* cmpwi r0, 0x61 */;
    if (/* eq */) goto .L_800DA6C8;
    if (/* ge */) goto .L_800DA6DC;
    /* cmpwi r0, 0x0 */;
    if (/* eq */) goto .L_800DA6D4;
    goto .L_800DA6DC;
.L_800DA5DC:
    /* cmpwi r0, 0x90 */;
    if (/* eq */) goto .L_800DA618;
    goto .L_800DA6DC;
.L_800DA5E8:
    /* cmpwi r0, 0xb0 */;
    if (/* eq */) goto .L_800DA618;
    if (/* ge */) goto .L_800DA60C;
    /* cmpwi r0, 0xa8 */;
    if (/* eq */) goto .L_800DA618;
    if (/* ge */) goto .L_800DA6DC;
    /* cmpwi r0, 0xa0 */;
    if (/* eq */) goto .L_800DA618;
    goto .L_800DA6DC;
.L_800DA60C:
    /* cmpwi r0, 0xb8 */;
    if (/* eq */) goto .L_800DA618;
    goto .L_800DA6DC;
.L_800DA618:
    /* cmpwi r0, 0xa0 */;
    r6 = *(u16*)((u8*)r8 + 0x0);
    r8 = r8 + 0x2;
    if (/* eq */) goto .L_800DA69C;
    if (/* ge */) goto .L_800DA650;
    /* cmpwi r0, 0x90 */;
    if (/* eq */) goto .L_800DA68C;
    if (/* ge */) goto .L_800DA644;
    /* cmpwi r0, 0x80 */;
    if (/* eq */) goto .L_800DA6A4;
    goto .L_800DA6A8;
.L_800DA644:
    /* cmpwi r0, 0x98 */;
    if (/* eq */) goto .L_800DA694;
    goto .L_800DA6A8;
.L_800DA650:
    /* cmpwi r0, 0xb0 */;
    if (/* eq */) goto .L_800DA684;
    if (/* ge */) goto .L_800DA668;
    /* cmpwi r0, 0xa8 */;
    if (/* eq */) goto .L_800DA67C;
    goto .L_800DA6A8;
.L_800DA668:
    /* cmpwi r0, 0xb8 */;
    if (/* eq */) goto .L_800DA674;
    goto .L_800DA6A8;
.L_800DA674:
    r31 = 0x0;
    goto .L_800DA6A8;
.L_800DA67C:
    r31 = 0x1;
    goto .L_800DA6A8;
.L_800DA684:
    r31 = 0x2;
    goto .L_800DA6A8;
.L_800DA68C:
    r31 = 0x3;
    goto .L_800DA6A8;
.L_800DA694:
    r31 = 0x4;
    goto .L_800DA6A8;
.L_800DA69C:
    r31 = 0x5;
    goto .L_800DA6A8;
.L_800DA6A4:
    r31 = 0x6;
.L_800DA6A8:
    r5 = r8;
    r3 = r31;
    r4 = r27;
    r7 = r28;
    r8 = r29;
    fn_800DA6F0();
    r8 = r3;
    goto .L_800DA6D4;
.L_800DA6C8:
    r8 = r8 + 0x4;
    goto .L_800DA6D4;
    goto .L_800DA6DC;
.L_800DA6D4:
    /* cmplw r8, r30 */;
    if (/* lt */) goto .L_800DA5A0;
.L_800DA6DC:
    /* lmw r27, 0xc(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    return;
}
#pragma pop

/* fn_800DA6F0 | Size: 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA6F0(void) {
    /* stmw r26, 0x8(r1) */;
    r28 = r5;
    r27 = r4;
    r29 = r6;
    r30 = r7;
    r31 = r8;
    r6 = r27;
    r5 = 0x0;
    goto .L_800DA7B8;
.L_800DA720:
    /* cmplwi r0, 0x19 */;
    if (/* gt */) goto .L_800DA7B4;
    r4 = (u32)jumptable_803152B8;
    r0 = r0 << 2;
    r4 = (u32)jumptable_803152B8;
    /* lwzx r0, r4, r0 */;
    /* mtctr r0 */;
    /* indirect jump via ctr */;
    r5 = r5 | 0x1;
    goto .L_800DA7B4;
    r5 = r5 | 0x40;
    goto .L_800DA7B4;
    r5 = r5 | 0x2;
    goto .L_800DA7B4;
    r5 = r5 | 0x4;
    goto .L_800DA7B4;
    r5 = r5 | 0x8;
    goto .L_800DA7B4;
    r5 = r5 | 0x10;
    goto .L_800DA7B4;
    r5 = r5 | 0x20;
    goto .L_800DA7B4;
    r5 = r5 | 0x80;
    goto .L_800DA7B4;
    r5 = r5 | 0x100;
    goto .L_800DA7B4;
    r5 = r5 | 0x200;
    goto .L_800DA7B4;
    r5 = r5 | 0x400;
    goto .L_800DA7B4;
    r5 = r5 | 0x800;
    goto .L_800DA7B4;
    r5 = r5 | 0x1000;
    goto .L_800DA7B4;
    r5 = r5 | 0x2000;
    goto .L_800DA7B4;
    r5 = r5 | 0x4000;
.L_800DA7B4:
    r6 = r6 + 0x18;
.L_800DA7B8:
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0xff */;
    if (/* ne */) goto .L_800DA720;
    r12 = *(u32*)((u8*)r30 + 0x0);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800DA844;
    r4 = r29;
    r6 = r31;
    /* mtctr r12 */;
    /* indirect call via ctr */;
    goto .L_800DA844;
.L_800DA7E4:
    r12 = *(u32*)((u8*)r30 + 0x4);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800DA7FC;
    r3 = r31;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800DA7FC:
    r26 = r27;
    goto .L_800DA820;
.L_800DA804:
    r3 = r26;
    r4 = r28;
    r5 = r30;
    r6 = r31;
    fn_800DA880();
    r28 = r3;
    r26 = r26 + 0x18;
.L_800DA820:
    r0 = *(u32*)((u8*)r26 + 0x0);
    /* cmpwi r0, 0xff */;
    if (/* ne */) goto .L_800DA804;
    r12 = *(u32*)((u8*)r30 + 0xC);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800DA844;
    r3 = r31;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800DA844:
    r0 = r29 & 0xFFFF;
    /* subi r29, r29, 0x1 */;
    if (/* ne */) goto .L_800DA7E4;
    r12 = *(u32*)((u8*)r30 + 0x10);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800DA868;
    r3 = r31;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800DA868:
    r3 = r28;
    /* lmw r26, 0x8(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    return;
}
#pragma pop

/* fn_800DA880 | Size: 0x440 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DA880(void) {
    r31 = r4;
    r10 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r10, 0x15 */;
    if (/* ge */) goto .L_800DA8B4;
    /* cmpwi r10, 0x9 */;
    if (/* ge */) goto .L_800DA934;
    /* cmpwi r10, 0x0 */;
    if (/* ge */) goto .L_800DA8C0;
    goto .L_800DAC88;
.L_800DA8B4:
    /* cmpwi r10, 0x19 */;
    if (/* eq */) goto .L_800DA934;
    goto .L_800DAC88;
.L_800DA8C0:
    /* cmplwi r10, 0x8 */;
    r8 = (0x1 << 16);
    r9 = (0x1000 << 16);
    if (/* gt */) goto .L_800DA92C;
    r3 = (u32)jumptable_80315340;
    r0 = r10 << 2;
    r3 = (u32)jumptable_80315340;
    /* lwzx r0, r3, r0 */;
    /* mtctr r0 */;
    /* indirect jump via ctr */;
    r7 = 0x1;
    goto .L_800DA92C;
    r7 = 0x40;
    goto .L_800DA92C;
    r7 = 0x40;
    goto .L_800DA92C;
    r7 = 0x40;
    goto .L_800DA92C;
    r7 = 0x40;
    goto .L_800DA92C;
    r7 = 0x40;
    goto .L_800DA92C;
    r7 = 0x40;
    goto .L_800DA92C;
    r7 = 0x40;
    goto .L_800DA92C;
    r7 = 0x40;
.L_800DA92C:
    r31 = r31 + 0x1;
    goto .L_800DAC88;
.L_800DA934:
    /* cmpwi r10, 0xb */;
    if (/* eq */) goto .L_800DA944;
    /* cmpwi r10, 0xc */;
    if (/* ne */) goto .L_800DA9C4;
.L_800DA944:
    /* cmpwi r10, 0xb */;
    r4 = 0x20;
    if (/* ne */) goto .L_800DA954;
    r4 = 0x10;
.L_800DA954:
    r10 = *(u32*)((u8*)r3 + 0xC);
    r7 = r4;
    r9 = (0x1000 << 16);
    /* cmpwi r10, 0x3 */;
    if (/* eq */) goto .L_800DA9AC;
    if (/* ge */) goto .L_800DA984;
    /* cmpwi r10, 0x1 */;
    if (/* eq */) goto .L_800DA99C;
    if (/* ge */) goto .L_800DA9A4;
    /* cmpwi r10, 0x0 */;
    if (/* ge */) goto .L_800DA994;
    goto .L_800DAB1C;
.L_800DA984:
    /* cmpwi r10, 0x5 */;
    if (/* eq */) goto .L_800DA9BC;
    if (/* ge */) goto .L_800DAB1C;
    goto .L_800DA9B4;
.L_800DA994:
    r8 = (0x20 << 16);
    goto .L_800DAB1C;
.L_800DA99C:
    r8 = (0x40 << 16);
    goto .L_800DAB1C;
.L_800DA9A4:
    r8 = (0x80 << 16);
    goto .L_800DAB1C;
.L_800DA9AC:
    r8 = (0x100 << 16);
    goto .L_800DAB1C;
.L_800DA9B4:
    r8 = (0x200 << 16);
    goto .L_800DAB1C;
.L_800DA9BC:
    r8 = (0x400 << 16);
    goto .L_800DAB1C;
.L_800DA9C4:
    r8 = *(u32*)((u8*)r3 + 0xC);
    /* cmpwi r8, 0x2 */;
    if (/* eq */) goto .L_800DAA04;
    if (/* ge */) goto .L_800DA9E4;
    /* cmpwi r8, 0x0 */;
    if (/* eq */) goto .L_800DA9F4;
    if (/* ge */) goto .L_800DA9FC;
    goto .L_800DAA18;
.L_800DA9E4:
    /* cmpwi r8, 0x4 */;
    if (/* eq */) goto .L_800DAA14;
    if (/* ge */) goto .L_800DAA18;
    goto .L_800DAA0C;
.L_800DA9F4:
    r4 = (0x1 << 16);
    goto .L_800DAA18;
.L_800DA9FC:
    r4 = (0x2 << 16);
    goto .L_800DAA18;
.L_800DAA04:
    r4 = (0x4 << 16);
    goto .L_800DAA18;
.L_800DAA0C:
    r4 = (0x8 << 16);
    goto .L_800DAA18;
.L_800DAA14:
    r4 = (0x10 << 16);
.L_800DAA18:
    /* cmpwi r10, 0xd */;
    r8 = r4;
    if (/* ge */) goto .L_800DAA3C;
    /* cmpwi r10, 0xa */;
    if (/* eq */) goto .L_800DAA74;
    if (/* ge */) goto .L_800DAB1C;
    /* cmpwi r10, 0x9 */;
    if (/* ge */) goto .L_800DAA54;
    goto .L_800DAB1C;
.L_800DAA3C:
    /* cmpwi r10, 0x19 */;
    if (/* eq */) goto .L_800DAA8C;
    if (/* ge */) goto .L_800DAB1C;
    /* cmpwi r10, 0x15 */;
    if (/* ge */) goto .L_800DAB1C;
    goto .L_800DAAA4;
.L_800DAA54:
    r4 = *(u32*)((u8*)r3 + 0x8);
    r7 = 0x2;
    /* cmpwi r4, 0x0 */;
    if (/* ne */) goto .L_800DAA6C;
    r9 = (0x2000 << 16);
    goto .L_800DAB1C;
.L_800DAA6C:
    r9 = (0x4000 << 16);
    goto .L_800DAB1C;
.L_800DAA74:
    r4 = *(u32*)((u8*)r3 + 0x8);
    r7 = 0x4;
    /* cmpwi r4, 0x0 */;
    if (/* ne */) goto .L_800DAB1C;
    r9 = (0x4000 << 16);
    goto .L_800DAB1C;
.L_800DAA8C:
    r4 = *(u32*)((u8*)r3 + 0x8);
    r7 = 0x8;
    /* cmpwi r4, 0x1 */;
    if (/* ne */) goto .L_800DAB1C;
    r9 = (0x4000 << 16);
    goto .L_800DAB1C;
.L_800DAAA4:
    /* subi r9, r10, 0xd */;
    /* cmplwi r9, 0x7 */;
    if (/* gt */) goto .L_800DAB04;
    r4 = (u32)jumptable_80315320;
    r9 = r9 << 2;
    r4 = (u32)jumptable_80315320;
    /* lwzx r4, r4, r9 */;
    /* mtctr r4 */;
    /* indirect jump via ctr */;
    r7 = 0x80;
    goto .L_800DAB04;
    r7 = 0x100;
    goto .L_800DAB04;
    r7 = 0x200;
    goto .L_800DAB04;
    r7 = 0x400;
    goto .L_800DAB04;
    r7 = 0x800;
    goto .L_800DAB04;
    r7 = 0x1000;
    goto .L_800DAB04;
    r7 = 0x2000;
    goto .L_800DAB04;
    r7 = 0x4000;
.L_800DAB04:
    r4 = *(u32*)((u8*)r3 + 0x8);
    /* cmpwi r4, 0x0 */;
    if (/* ne */) goto .L_800DAB18;
    r9 = (0x1000 << 16);
    goto .L_800DAB1C;
.L_800DAB18:
    r9 = (0x2000 << 16);
.L_800DAB1C:
    r4 = *(u32*)((u8*)r3 + 0x4);
    /* cmpwi r4, 0x1 */;
    if (/* ne */) goto .L_800DAC58;
    r3 = (0x20 << 16);
    r4 = r31;
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABE4;
    if (/* ge */) goto .L_800DAB8C;
    r3 = (0x4 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABE4;
    if (/* ge */) goto .L_800DAB6C;
    r3 = (0x2 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABDC;
    if (/* ge */) goto .L_800DABF8;
    r3 = (0x1 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABDC;
    goto .L_800DABF8;
.L_800DAB6C:
    r3 = (0x10 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABF4;
    if (/* ge */) goto .L_800DABF8;
    r3 = (0x8 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABE4;
    goto .L_800DABF8;
.L_800DAB8C:
    r3 = (0x100 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABE4;
    if (/* ge */) goto .L_800DABBC;
    r3 = (0x80 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABF4;
    if (/* ge */) goto .L_800DABF8;
    r3 = (0x40 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABEC;
    goto .L_800DABF8;
.L_800DABBC:
    r3 = (0x400 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABF4;
    if (/* ge */) goto .L_800DABF8;
    r3 = (0x200 << 16);
    /* cmpw r8, r3 */;
    if (/* eq */) goto .L_800DABEC;
    goto .L_800DABF8;
.L_800DABDC:
    r0 = 0x1;
    goto .L_800DABF8;
.L_800DABE4:
    r0 = 0x2;
    goto .L_800DABF8;
.L_800DABEC:
    r0 = 0x3;
    goto .L_800DABF8;
.L_800DABF4:
    r0 = 0x4;
.L_800DABF8:
    r3 = (0x2000 << 16);
    /* cmpw r9, r3 */;
    if (/* eq */) goto .L_800DAC3C;
    if (/* ge */) goto .L_800DAC2C;
    r3 = (0x1000 << 16);
    /* cmpw r9, r3 */;
    if (/* eq */) goto .L_800DAC50;
    if (/* ge */) goto .L_800DAC50;
    r3 = (0x8000 << 16);
    r3 = r3 + 0x1;
    /* cmpw r9, r3 */;
    if (/* ge */) goto .L_800DAC50;
    goto .L_800DAC4C;
.L_800DAC2C:
    r3 = (0x4000 << 16);
    /* cmpw r9, r3 */;
    if (/* eq */) goto .L_800DAC44;
    goto .L_800DAC50;
.L_800DAC3C:
    r0 = r0 << 1;
    goto .L_800DAC50;
.L_800DAC44:
    r0 = r0 * 0x3;
    goto .L_800DAC50;
.L_800DAC4C:
    r0 = r0 << 2;
.L_800DAC50:
    r31 = r31 + r0;
    goto .L_800DAC88;
.L_800DAC58:
    /* cmpwi r4, 0x2 */;
    if (/* ne */) goto .L_800DAC6C;
    r4 = *(u8*)((u8*)r31 + 0x0);
    r31 = r31 + 0x1;
    goto .L_800DAC74;
.L_800DAC6C:
    r4 = *(u16*)((u8*)r31 + 0x0);
    r31 = r31 + 0x2;
.L_800DAC74:
    r0 = *(u16*)((u8*)r3 + 0x12);
    r4 = r4 & 0xFFFF;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r4 * r0;
    r4 = r3 + r0;
.L_800DAC88:
    r12 = *(u32*)((u8*)r5 + 0x8);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800DACA8;
    r0 = r7 | r8;
    r5 = r6;
    r3 = r9 | r0;
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800DACA8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r3 = r31;
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DACC0 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DACC0(void) {
    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x480);
    /* cmplw r0, r31 */;
    if (/* eq */) goto .L_800DACFC;
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E24B0();
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E209C();
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
.L_800DACFC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DAD10 | Size: 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DAD10(void) {
    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DAD4C;
    r5 = r31;
    r3 = 0x2a;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DADA0;
.L_800DAD4C:
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    /* cmplw r3, r0 */;
    if (/* ne */) goto .L_800DADA0;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* and. r0, r3, r0 */;
    if (/* eq */) goto .L_800DADA0;
    r0 = *(u32*)((u8*)r31 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DADA0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    fn_800D7A70();
    r3 = *(u32*)((u8*)r31 + 0xC);
    fn_800D892C();
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = *(u32*)((u8*)r31 + 0x8);
    fn_800BD0FC();
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = *(u32*)((u8*)r31 + 0x14);
    fn_800D6A5C();
.L_800DADA0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DADB4 | Size: 0x1AC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DADB4(void) {
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x47E);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DADDC;
    r3 = 0x0;
    goto .L_800DAF4C;
.L_800DADDC:
    r31 = *(u32*)((u8*)r3 + 0x480);
    r5 = *(u32*)((u8*)r3 + 0x484);
    r3 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r4 = r5 + 0x1f;
    /* clrrwi r7, r4, 5 */;
    r0 = r3 + r0;
    /* cmplw r7, r0 */;
    if (/* gt */) goto .L_800DAE0C;
    r0 = *(u8*)((u8*)r31 + 0x1);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DAE24;
.L_800DAE0C:
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E24B0();
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E209C();
    r3 = 0x0;
    goto .L_800DAF4C;
.L_800DAE24:
    /* subf. r0, r5, r7 */;
    r6 = 0x0;
    r3 = r0;
    if (/* eq */) goto .L_800DAF08;
    /* srwi. r0, r0, 3 */;
    /* mtctr r0 */;
    if (/* eq */) goto .L_800DAEEC;
.L_800DAE40:
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    if (--ctr != 0) goto .L_800DAE40;
    r3 = r3 & 0x7;
    if (/* eq */) goto .L_800DAF08;
.L_800DAEEC:
    /* mtctr r3 */;
.L_800DAEF0:
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    if (--ctr != 0) goto .L_800DAEF0;
.L_800DAF08:
    r0 = *(u32*)((u8*)r31 + 0x4);
    r0 = r7 - r0;
    *(u32*)((u8*)r31 + 0x8) = r0;
    r3 = *(u16*)((u8*)r31 + 0x2);
    r4 = *(u32*)((u8*)r31 + 0x8);
    fn_800E2AF8();
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = *(u32*)((u8*)r31 + 0x8);
    DCFlushRange();
    r4 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    r3 = r31;
    *(u8*)((u8*)r4 + 0x47E) = r0;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x480) = r0;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r0;
.L_800DAF4C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DAF60 | Size: 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DAF60(void) {
    r30 = r4;
    r29 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DAF98;
    r3 = 0x0;
    goto .L_800DB07C;
.L_800DAF98:
    r0 = *(u8*)((u8*)r5 + 0x49F);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DAFAC;
    r3 = 0x0;
    goto .L_800DB07C;
.L_800DAFAC:
    r0 = *(u32*)lbl_8047AAD8;
    r31 = *(u32*)lbl_8047AAD4;
    /* mtctr r0 */;
    /* cmplwi r0, 0x0 */;
    if (/* le */) goto .L_800DAFD8;
.L_800DAFC0:
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DAFD0;
    goto .L_800DAFDC;
.L_800DAFD0:
    r31 = r31 + 0x18;
    if (--ctr != 0) goto .L_800DAFC0;
.L_800DAFD8:
    r31 = 0x0;
.L_800DAFDC:
    /* cmplwi r31, 0x0 */;
    if (/* ne */) goto .L_800DAFEC;
    r3 = 0x0;
    goto .L_800DB07C;
.L_800DAFEC:
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r3;
    r3 = r30;
    r4 = 0x20;
    *(u8*)((u8*)r31 + 0x1) = r0;
    *(u32*)((u8*)r31 + 0x10) = r0;
    *(u32*)((u8*)r31 + 0x14) = r0;
    fn_800E2C04();
    *(u16*)((u8*)r31 + 0x2) = r3;
    r0 = *(u16*)((u8*)r31 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DB028;
    r3 = 0x0;
    goto .L_800DB07C;
.L_800DB028:
    *(u32*)((u8*)r31 + 0x8) = r30;
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E27B0();
    *(u32*)((u8*)r31 + 0x4) = r3;
    r0 = *(u32*)((u8*)r31 + 0x4);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DB054;
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E209C();
    r3 = 0x0;
    goto .L_800DB07C;
.L_800DB054:
    *(u32*)((u8*)r31 + 0xC) = r29;
    r0 = 0x1;
    r3 = 0x1;
    r4 = *(u32*)lbl_8047AA80;
    *(u8*)((u8*)r4 + 0x47E) = r0;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x480) = r31;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r0;
.L_800DB07C:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DB098 | Size: 0x6C0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB098(void) {
    r3 = *(u32*)lbl_8047AA80;
    r7 = *(u32*)((u8*)r3 + 0x480);
    r4 = *(u32*)((u8*)r3 + 0x484);
    r5 = *(u32*)((u8*)r7 + 0x4);
    r0 = *(u32*)((u8*)r7 + 0x8);
    r6 = r4 + 0x68;
    r0 = r5 + r0;
    /* cmplw r6, r0 */;
    if (/* le */) goto .L_800DB0C8;
    r0 = 0x1;
    *(u8*)((u8*)r7 + 0x1) = r0;
    return;
.L_800DB0C8:
    r5 = (u32)fn_800D75D0;
    r6 = *(u32*)((u8*)r3 + 0x4A8);
    r0 = (u32)fn_800D75D0;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB10C;
    r0 = *(u32*)((u8*)r3 + 0x4B8);
    r5 = r4 + 0xc;
    *(u32*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x4BC);
    *(u32*)((u8*)r4 + 0x4) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x4C0);
    *(u32*)((u8*)r4 + 0x8) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB2B4;
.L_800DB10C:
    r5 = (u32)fn_800D75AC;
    r0 = (u32)fn_800D75AC;
    /* cmplw r6, r0 */;
    if (/* eq */) goto .L_800DB12C;
    r5 = (u32)fn_800D7588;
    r0 = (u32)fn_800D7588;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB15C;
.L_800DB12C:
    r0 = *(u16*)((u8*)r3 + 0x4B0);
    r5 = r4 + 0x6;
    *(u16*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u16*)((u8*)r3 + 0x4B2);
    *(u16*)((u8*)r4 + 0x2) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u16*)((u8*)r3 + 0x4B4);
    *(u16*)((u8*)r4 + 0x4) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB2B4;
.L_800DB15C:
    r5 = (u32)fn_800D7564;
    r0 = (u32)fn_800D7564;
    /* cmplw r6, r0 */;
    if (/* eq */) goto .L_800DB17C;
    r5 = (u32)fn_800D7540;
    r0 = (u32)fn_800D7540;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB1AC;
.L_800DB17C:
    r0 = *(u8*)((u8*)r3 + 0x4AC);
    r5 = r4 + 0x3;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x4AD);
    *(u8*)((u8*)r4 + 0x1) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x4AE);
    *(u8*)((u8*)r4 + 0x2) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB2B4;
.L_800DB1AC:
    r5 = (u32)fn_800D7524;
    r0 = (u32)fn_800D7524;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB1E0;
    r0 = *(u32*)((u8*)r3 + 0x4B8);
    r5 = r4 + 0x8;
    *(u32*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x4BC);
    *(u32*)((u8*)r4 + 0x4) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB2B4;
.L_800DB1E0:
    r5 = (u32)fn_800D7508;
    r0 = (u32)fn_800D7508;
    /* cmplw r6, r0 */;
    if (/* eq */) goto .L_800DB200;
    r5 = (u32)fn_800D74EC;
    r0 = (u32)fn_800D74EC;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB224;
.L_800DB200:
    r0 = *(u16*)((u8*)r3 + 0x4B0);
    r5 = r4 + 0x4;
    *(u16*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u16*)((u8*)r3 + 0x4B2);
    *(u16*)((u8*)r4 + 0x2) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB2B4;
.L_800DB224:
    r5 = (u32)fn_800D74D0;
    r0 = (u32)fn_800D74D0;
    /* cmplw r6, r0 */;
    if (/* eq */) goto .L_800DB244;
    r5 = (u32)fn_800D74B4;
    r0 = (u32)fn_800D74B4;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB268;
.L_800DB244:
    r0 = *(u8*)((u8*)r3 + 0x4AC);
    r5 = r4 + 0x2;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x4AD);
    *(u8*)((u8*)r4 + 0x1) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB2B4;
.L_800DB268:
    r5 = (u32)fn_800D74A0;
    r0 = (u32)fn_800D74A0;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB290;
    r0 = *(u16*)((u8*)r3 + 0x4B0);
    r5 = r4 + 0x2;
    *(u16*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB2B4;
.L_800DB290:
    r5 = (u32)fn_800D748C;
    r0 = (u32)fn_800D748C;
    /* cmplw r6, r0 */;
    if (/* ne */) goto .L_800DB2B4;
    r0 = *(u8*)((u8*)r3 + 0x4AC);
    r5 = r4 + 0x1;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
.L_800DB2B4:
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x480);
    r5 = *(u32*)((u8*)r4 + 0x4C4);
    r3 = *(u32*)((u8*)r3 + 0xC);
    r0 = *(u8*)((u8*)r3 + 0x40);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DB3F0;
    r3 = (u32)fn_800D7468;
    r0 = (u32)fn_800D7468;
    /* cmplw r5, r0 */;
    if (/* ne */) goto .L_800DB314;
    r5 = *(u32*)((u8*)r4 + 0x484);
    r0 = *(u32*)((u8*)r4 + 0x4D4);
    *(u32*)((u8*)r5 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x4D8);
    *(u32*)((u8*)r5 + 0x4) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x4DC);
    *(u32*)((u8*)r5 + 0x8) = r0;
    r5 = r5 + 0xc;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB3F0;
.L_800DB314:
    r3 = (u32)fn_800D7444;
    r0 = (u32)fn_800D7444;
    /* cmplw r5, r0 */;
    if (/* ne */) goto .L_800DB358;
    r5 = *(u32*)((u8*)r4 + 0x484);
    r0 = *(u16*)((u8*)r4 + 0x4CC);
    *(u16*)((u8*)r5 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u16*)((u8*)r3 + 0x4CE);
    *(u16*)((u8*)r5 + 0x2) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u16*)((u8*)r3 + 0x4D0);
    *(u16*)((u8*)r5 + 0x4) = r0;
    r5 = r5 + 0x6;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB3F0;
.L_800DB358:
    r3 = (u32)fn_800D7420;
    r0 = (u32)fn_800D7420;
    /* cmplw r5, r0 */;
    if (/* ne */) goto .L_800DB39C;
    r5 = *(u32*)((u8*)r4 + 0x484);
    r0 = *(u8*)((u8*)r4 + 0x4C8);
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x4C9);
    *(u8*)((u8*)r5 + 0x1) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x4CA);
    *(u8*)((u8*)r5 + 0x2) = r0;
    r5 = r5 + 0x3;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB3F0;
.L_800DB39C:
    r3 = (u32)fn_800D740C;
    r0 = (u32)fn_800D740C;
    /* cmplw r5, r0 */;
    if (/* ne */) goto .L_800DB3C8;
    r5 = *(u32*)((u8*)r4 + 0x484);
    r0 = *(u16*)((u8*)r4 + 0x4CC);
    *(u16*)((u8*)r5 + 0x0) = r0;
    r5 = r5 + 0x2;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto .L_800DB3F0;
.L_800DB3C8:
    r3 = (u32)fn_800D73F8;
    r0 = (u32)fn_800D73F8;
    /* cmplw r5, r0 */;
    if (/* ne */) goto .L_800DB3F0;
    r5 = *(u32*)((u8*)r4 + 0x484);
    r0 = *(u8*)((u8*)r4 + 0x4C8);
    *(u8*)((u8*)r5 + 0x0) = r0;
    r5 = r5 + 0x1;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
.L_800DB3F0:
    r0 = 0x2;
    r5 = 0x4;
    r3 = 0x70;
    /* mtctr r0 */;
.L_800DB400:
    r4 = *(u32*)lbl_8047AA80;
    /* subi r8, r5, 0x4 */;
    r7 = r8 << 2;
    r0 = r3 + 0x8;
    r6 = *(u32*)((u8*)r4 + 0x480);
    r7 = r7 + 0x4e0;
    /* lwzx r7, r4, r7 */;
    r6 = *(u32*)((u8*)r6 + 0xC);
    /* lbzx r0, r6, r0 */;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DB5B0;
    r6 = (u32)fn_800D73C4;
    r0 = (u32)fn_800D73C4;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB490;
    r8 = r8 * 0xc;
    r9 = *(u32*)((u8*)r4 + 0x484);
    r0 = r8 + 0x4e8;
    /* lbzx r7, r4, r0 */;
    r6 = r8 + 0x4e9;
    r4 = r8 + 0x4ea;
    r0 = r8 + 0x4eb;
    *(u8*)((u8*)r9 + 0x0) = r7;
    r7 = *(u32*)lbl_8047AA80;
    /* lbzx r6, r7, r6 */;
    *(u8*)((u8*)r9 + 0x1) = r6;
    r6 = *(u32*)lbl_8047AA80;
    /* lbzx r4, r6, r4 */;
    *(u8*)((u8*)r9 + 0x2) = r4;
    r4 = *(u32*)lbl_8047AA80;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r9 + 0x3) = r0;
    r9 = r9 + 0x4;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r9;
    goto .L_800DB5B0;
.L_800DB490:
    r6 = (u32)fn_800D7398;
    r0 = (u32)fn_800D7398;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB4E4;
    r7 = r8 * 0xc;
    r8 = *(u32*)((u8*)r4 + 0x484);
    r0 = r7 + 0x4e8;
    /* lbzx r6, r4, r0 */;
    r4 = r7 + 0x4e9;
    r0 = r7 + 0x4ea;
    *(u8*)((u8*)r8 + 0x0) = r6;
    r6 = *(u32*)lbl_8047AA80;
    /* lbzx r4, r6, r4 */;
    *(u8*)((u8*)r8 + 0x1) = r4;
    r4 = *(u32*)lbl_8047AA80;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r8 + 0x2) = r0;
    r8 = r8 + 0x3;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r8;
    goto .L_800DB5B0;
.L_800DB4E4:
    r6 = (u32)fn_800D737C;
    r0 = (u32)fn_800D737C;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB518;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4f0;
    /* lwzx r0, r4, r0 */;
    *(u32*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x4;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto .L_800DB5B0;
.L_800DB518:
    r6 = (u32)fn_800D7360;
    r0 = (u32)fn_800D7360;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB54C;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4ec;
    /* lhzx r0, r4, r0 */;
    *(u16*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x2;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto .L_800DB5B0;
.L_800DB54C:
    r6 = (u32)fn_800D7344;
    r0 = (u32)fn_800D7344;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB580;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4ec;
    /* lhzx r0, r4, r0 */;
    *(u16*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x2;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto .L_800DB5B0;
.L_800DB580:
    r6 = (u32)fn_800D7328;
    r0 = (u32)fn_800D7328;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB5B0;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4e8;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x1;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
.L_800DB5B0:
    r3 = r3 + 0x1c;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto .L_800DB400;
    r0 = 0x8;
    r5 = 0x6;
    r3 = 0xa8;
    /* mtctr r0 */;
.L_800DB5CC:
    r4 = *(u32*)lbl_8047AA80;
    /* subi r8, r5, 0x6 */;
    r7 = r8 << 2;
    r0 = r3 + 0x8;
    r6 = *(u32*)((u8*)r4 + 0x480);
    r7 = r7 + 0x500;
    /* lwzx r7, r4, r7 */;
    r6 = *(u32*)((u8*)r6 + 0xC);
    /* lbzx r0, r6, r0 */;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DB748;
    r6 = (u32)fn_800D7304;
    r0 = (u32)fn_800D7304;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB63C;
    r6 = r8 << 4;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x528;
    /* lwzx r4, r4, r0 */;
    r0 = r6 + 0x52c;
    *(u32*)((u8*)r7 + 0x0) = r4;
    r4 = *(u32*)lbl_8047AA80;
    /* lwzx r0, r4, r0 */;
    *(u32*)((u8*)r7 + 0x4) = r0;
    r7 = r7 + 0x8;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto .L_800DB748;
.L_800DB63C:
    r6 = (u32)fn_800D72E4;
    r0 = (u32)fn_800D72E4;
    /* cmplw r7, r0 */;
    if (/* eq */) goto .L_800DB65C;
    r6 = (u32)fn_800D72C4;
    r0 = (u32)fn_800D72C4;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB690;
.L_800DB65C:
    r6 = r8 << 4;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x522;
    /* lhzx r4, r4, r0 */;
    r0 = r6 + 0x524;
    *(u16*)((u8*)r7 + 0x0) = r4;
    r4 = *(u32*)lbl_8047AA80;
    /* lhzx r0, r4, r0 */;
    *(u16*)((u8*)r7 + 0x2) = r0;
    r7 = r7 + 0x4;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto .L_800DB748;
.L_800DB690:
    r6 = (u32)fn_800D72A4;
    r0 = (u32)fn_800D72A4;
    /* cmplw r7, r0 */;
    if (/* eq */) goto .L_800DB6B0;
    r6 = (u32)fn_800D7284;
    r0 = (u32)fn_800D7284;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB6E4;
.L_800DB6B0:
    r6 = r8 << 4;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x520;
    /* lbzx r4, r4, r0 */;
    r0 = r6 + 0x521;
    *(u8*)((u8*)r7 + 0x0) = r4;
    r4 = *(u32*)lbl_8047AA80;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r7 + 0x1) = r0;
    r7 = r7 + 0x2;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto .L_800DB748;
.L_800DB6E4:
    r6 = (u32)fn_800D7268;
    r0 = (u32)fn_800D7268;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB718;
    r6 = r8 << 4;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x522;
    /* lhzx r0, r4, r0 */;
    *(u16*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x2;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto .L_800DB748;
.L_800DB718:
    r6 = (u32)fn_800D724C;
    r0 = (u32)fn_800D724C;
    /* cmplw r7, r0 */;
    if (/* ne */) goto .L_800DB748;
    r6 = r8 << 4;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x520;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x1;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
.L_800DB748:
    r3 = r3 + 0x1c;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto .L_800DB5CC;
    return;
}
#pragma pop

/* fn_800DB758 | Size: 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB758(void) {
    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x488);
    /* cmpwi r0, 0x7 */;
    if (/* ne */) goto .L_800DB780;
    /* clrlslwi r31, r31, 17, 1 */;
.L_800DB780:
    r6 = *(u32*)((u8*)r4 + 0x480);
    r3 = r31;
    r4 = *(u32*)((u8*)r4 + 0x488);
    r5 = r6 + 0x10;
    r6 = r6 + 0x14;
    fn_800D6A80();
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x488);
    /* cmplwi r0, 0x7 */;
    if (/* gt */) goto .L_800DB83C;
    r3 = (u32)jumptable_80315364;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80315364;
    /* lwzx r0, r3, r0 */;
    /* mtctr r0 */;
    /* indirect jump via ctr */;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xb8;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto .L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xa8;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto .L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xb0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto .L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x90;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto .L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x98;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto .L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xa0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto .L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x80;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto .L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x80;
    *(u8*)((u8*)r3 + 0x0) = r0;
.L_800DB83C:
    r3 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x484);
    r0 = r5 + 0x1;
    *(u32*)((u8*)r3 + 0x484) = r0;
    r3 = *(u32*)((u8*)r3 + 0x480);
    r4 = *(u8*)((u8*)r5 + 0x0);
    r3 = *(u32*)((u8*)r3 + 0xC);
    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = r4 | r0;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r3 + 0x484);
    *(u16*)((u8*)r4 + 0x0) = r31;
    r4 = r4 + 0x2;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r4;
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DB890 -- GSgfx_InitLighting | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB890(void) {
    r0 = r3;
    r3 = r0 * 0x18;
    *(u32*)lbl_8047AAD8 = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAD0 = r3;
    if (/* eq */) goto .L_800DB8F0;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AAD4 = r3;
    r4 = r5;
    r6 = 0x0;
    goto .L_800DB8E4;
.L_800DB8D4:
    r3 = *(u32*)lbl_8047AAD4;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x18;
.L_800DB8E4:
    r0 = *(u32*)lbl_8047AAD8;
    /* cmplw r6, r0 */;
    if (/* lt */) goto .L_800DB8D4;
.L_800DB8F0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DB900 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB900(void) {
    r30 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DB948;
    r5 = r3;
    r6 = r4;
    r7 = (s8)r30;
    r3 = 0x5b;
    r4 = 0x12;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DB970;
.L_800DB948:
    /* subi r0, r3, 0x1 */;
    r3 = (u32)lbl_80400B28;
    r6 = r0 * 0x1c;
    r5 = 0x18;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r6;
    r31 = r3 + 0x360;
    r3 = r31;
    memcpy();
    *(u8*)((u8*)r31 + 0x18) = r30;
.L_800DB970:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800DB988 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB988(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DB9C4;
    r5 = r3;
    r6 = r4;
    r3 = 0x5a;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DB9E0;
.L_800DB9C4:
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    r3 = r3 + 0x34d;
    *(u8*)((u8*)r3 + 0x2) = r4;
    *(u8*)((u8*)r3 + 0x3) = r7;
.L_800DB9E0:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DB9F0 | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DB9F0(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBA2C;
    r5 = r3;
    r6 = r4;
    r3 = 0x59;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBA44;
.L_800DBA2C:
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    r3 += 845; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r7;
.L_800DBA44:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBA54 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBA54(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBA88;
    r5 = r3 & 0xFF;
    r3 = 0x58;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBA94;
.L_800DBA88:
    r4 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    *(u8*)((u8*)r4 + 0x34C) = r3;
.L_800DBA94:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBAA4 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBAA4(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBAD8;
    r5 = r3;
    r3 = 0x57;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBAFC;
.L_800DBAD8:
    r4 = (u32)lbl_80400B28;
    r6 = 0x1;
    r5 = (u32)lbl_80400B28;
    r4 = 0x5;
    r0 = r3 * 0x14;
    r3 = r5 + r3;
    *(u8*)((u8*)r3 + 0x1FC) = r6;
    r3 = r5 + r0;
    *(u8*)((u8*)r3 + 0x20C) = r4;
.L_800DBAFC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBB0C | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBB0C(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBB48;
    r5 = r3;
    r6 = r4;
    r3 = 0x56;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBB74;
.L_800DBB48:
    r5 = (u32)lbl_80400B28;
    r8 = 0x1;
    r6 = (u32)lbl_80400B28;
    r0 = 0x4;
    r5 = r3 * 0x14;
    r3 = r6 + r3;
    *(u8*)((u8*)r3 + 0x1FC) = r8;
    r3 = r6 + r5;
    r3 += 524; *(u8*)r3 = r0;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u8*)((u8*)r3 + 0x2) = r7;
.L_800DBB74:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBB84 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBB84(void) {
    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBBC0;
    r5 = r3;
    r6 = r4;
    r3 = 0x55;
    r4 = 0x3;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBBEC;
.L_800DBBC0:
    r5 = (u32)lbl_80400B28;
    r8 = 0x1;
    r6 = (u32)lbl_80400B28;
    r0 = 0x3;
    r5 = r3 * 0x14;
    r3 = r6 + r3;
    *(u8*)((u8*)r3 + 0x1FC) = r8;
    r3 = r6 + r5;
    r3 += 524; *(u8*)r3 = r0;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u8*)((u8*)r3 + 0x2) = r7;
.L_800DBBEC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBBFC | Size: 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBBFC(void) {
    r12 = *(u32*)((u8*)r1 + 0x38);
    r31 = r8;
    r30 = r7;
    r29 = r6;
    r28 = r5;
    r11 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r11 + 0x0);
    r11 = *(u32*)((u8*)r1 + 0x3C);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBC7C;
    r5 = r3;
    r6 = r4;
    r7 = r28 & 0xFFFF;
    r8 = r29 & 0xFFFF;
    r9 = r30 & 0xFFFF;
    r10 = r31 & 0xFFFF;
    r3 = 0x54;
    r4 = 0xa;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBCC4;
.L_800DBC7C:
    r5 = (u32)lbl_80400B28;
    r7 = 0x1;
    r6 = (u32)lbl_80400B28;
    r0 = 0x2;
    r5 = r3 * 0x14;
    r3 = r6 + r3;
    *(u8*)((u8*)r3 + 0x1FC) = r7;
    r3 = r6 + r5;
    r3 += 524; *(u8*)r3 = r0;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u16*)((u8*)r3 + 0xC) = r28;
    *(u16*)((u8*)r3 + 0xE) = r29;
    *(u16*)((u8*)r3 + 0x10) = r30;
    *(u16*)((u8*)r3 + 0x12) = r31;
    *(u8*)((u8*)r3 + 0x3) = r9;
    *(u8*)((u8*)r3 + 0x2) = r10;
    *(u8*)((u8*)r3 + 0x4) = r12;
    *(u8*)((u8*)r3 + 0x5) = r11;
.L_800DBCC4:
    r0 = *(u32*)((u8*)r1 + 0x34);
    r31 = *(u32*)((u8*)r1 + 0x2C);
    r30 = *(u32*)((u8*)r1 + 0x28);
    r29 = *(u32*)((u8*)r1 + 0x24);
    r28 = *(u32*)((u8*)r1 + 0x20);
    return;
}
#pragma pop

/* fn_800DBCE4 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBCE4(void) {
    r10 = r5;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r8 + 0x0);
    r8 = r6;
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBD30;
    r5 = r3;
    r6 = r4;
    r9 = r7;
    r7 = r10 & 0xFF;
    r8 = r8 & 0xFF;
    r3 = 0x53;
    r4 = 0x5;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBD60;
.L_800DBD30:
    r5 = (u32)lbl_80400B28;
    r6 = 0x1;
    r5 = (u32)lbl_80400B28;
    r0 = r3 * 0x14;
    r3 = r5 + r3;
    *(u8*)((u8*)r3 + 0x1FC) = r6;
    r3 = r5 + r0;
    r3 += 524; *(u8*)r3 = r6;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u8*)((u8*)r3 + 0xA) = r10;
    *(u8*)((u8*)r3 + 0xB) = r8;
    *(u8*)((u8*)r3 + 0x2) = r7;
.L_800DBD60:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBD70 | Size: 0xEC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBD70(void) {
    r12 = *(u8*)((u8*)r1 + 0x3B);
    r31 = r8;
    r30 = r7;
    r29 = r6;
    r28 = r5;
    r11 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r11 + 0x0);
    r11 = *(u32*)((u8*)r1 + 0x3C);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBDF4;
    r0 = r10 & 0xFF;
    r5 = r3;
    r6 = r4;
    r7 = r28;
    r8 = r29;
    r9 = r30;
    r10 = r31;
    r3 = 0x52;
    r4 = 0xa;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBE3C;
.L_800DBDF4:
    r5 = (u32)lbl_80400B28;
    r7 = 0x1;
    r6 = (u32)lbl_80400B28;
    r0 = 0x0;
    r5 = r3 * 0x14;
    r3 = r6 + r3;
    *(u8*)((u8*)r3 + 0x1FC) = r7;
    r3 = r6 + r5;
    r3 += 524; *(u8*)r3 = r0;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u8*)((u8*)r3 + 0x3) = r28;
    *(u8*)((u8*)r3 + 0x4) = r29;
    *(u8*)((u8*)r3 + 0x2) = r30;
    *(u8*)((u8*)r3 + 0x6) = r31;
    *(u8*)((u8*)r3 + 0x7) = r9;
    *(u8*)((u8*)r3 + 0x8) = r10;
    *(u8*)((u8*)r3 + 0x9) = r12;
    *(u8*)((u8*)r3 + 0x5) = r11;
.L_800DBE3C:
    r0 = *(u32*)((u8*)r1 + 0x34);
    r31 = *(u32*)((u8*)r1 + 0x2C);
    r30 = *(u32*)((u8*)r1 + 0x28);
    r29 = *(u32*)((u8*)r1 + 0x24);
    r28 = *(u32*)((u8*)r1 + 0x20);
    return;
}
#pragma pop

/* fn_800DBE5C | Size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBE5C(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBE90;
    r5 = r3;
    r3 = 0x51;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBEA4;
.L_800DBE90:
    r4 = (u32)lbl_80400B28;
    r5 = 0x0;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    *(u8*)((u8*)r3 + 0x1FC) = r5;
.L_800DBEA4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBEB4 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBEB4(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBEF4;
    r0 = *(u32*)((u8*)r4 + 0x0);
    r5 = r3;
    r6 = r1 + 0x8;
    r3 = 0x50;
    r4 = 0x14;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBF0C;
.L_800DBEF4:
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r4 = *(u32*)((u8*)r4 + 0x0);
    r3 = r0 + r3;
    *(u32*)((u8*)r3 + 0x1EC) = r4;
.L_800DBF0C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBF1C | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBF1C(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBF54;
    r5 = r3;
    r6 = r4;
    r3 = 0x4f;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBF68;
.L_800DBF54:
    r5 = (u32)lbl_80400B28;
    r0 = r3 << 2;
    r3 = (u32)lbl_80400B28;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x1AC) = r4;
.L_800DBF68:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBF78 | Size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBF78(void) {
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DBFB0;
    r5 = r3;
    r6 = r4;
    r3 = 0x4e;
    r4 = 0x2;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DBFC4;
.L_800DBFB0:
    r5 = (u32)lbl_80400B28;
    r0 = r3 << 2;
    r3 = (u32)lbl_80400B28;
    r3 = r3 + r0;
    *(u32*)((u8*)r3 + 0x16C) = r4;
.L_800DBFC4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DBFD4 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DBFD4(void) {
    r10 = r5;
    r9 = r7;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r8 + 0x0);
    r8 = r6;
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC01C;
    r5 = r3;
    r6 = r4;
    r7 = r10;
    r3 = 0x4d;
    r4 = 0x5;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DC03C;
.L_800DC01C:
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    r3 += 299; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r10;
    *(u8*)((u8*)r3 + 0x2) = r8;
    *(u8*)((u8*)r3 + 0x3) = r9;
.L_800DC03C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DC04C | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC04C(void) {
    r12 = r5;
    r11 = r6;
    r10 = r8;
    r9 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r9 + 0x0);
    r9 = r7;
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC0A0;
    r5 = r3;
    r6 = r4;
    r7 = r12;
    r8 = r11;
    r9 = r9 & 0xFF;
    r3 = 0x4c;
    r4 = 0x6;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DC0C4;
.L_800DC0A0:
    r5 = r3 * 0x5;
    r3 = (u32)lbl_80400B28;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r5;
    r3 += 155; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r12;
    *(u8*)((u8*)r3 + 0x2) = r11;
    *(u8*)((u8*)r3 + 0x3) = r9;
    *(u8*)((u8*)r3 + 0x4) = r10;
.L_800DC0C4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DC0D4 | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC0D4(void) {
    r10 = r5;
    r9 = r7;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r8 + 0x0);
    r8 = r6;
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC11C;
    r5 = r3;
    r6 = r4;
    r7 = r10;
    r3 = 0x4b;
    r4 = 0x5;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DC13C;
.L_800DC11C:
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    r3 += 235; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r10;
    *(u8*)((u8*)r3 + 0x2) = r8;
    *(u8*)((u8*)r3 + 0x3) = r9;
.L_800DC13C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DC14C | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC14C(void) {
    r12 = r5;
    r11 = r6;
    r10 = r8;
    r9 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r9 + 0x0);
    r9 = r7;
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC1A0;
    r5 = r3;
    r6 = r4;
    r7 = r12;
    r8 = r11;
    r9 = r9 & 0xFF;
    r3 = 0x4a;
    r4 = 0x6;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DC1C4;
.L_800DC1A0:
    r5 = r3 * 0x5;
    r3 = (u32)lbl_80400B28;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r5;
    r3 += 75; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r12;
    *(u8*)((u8*)r3 + 0x2) = r11;
    *(u8*)((u8*)r3 + 0x3) = r9;
    *(u8*)((u8*)r3 + 0x4) = r10;
.L_800DC1C4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DC1D4 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC1D4(void) {
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC208;
    r5 = r3 & 0xFF;
    r3 = 0x49;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DC214;
.L_800DC208:
    r4 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    *(u8*)((u8*)r4 + 0x1A) = r3;
.L_800DC214:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DC224 | Size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC224(void) {
    r11 = r5;
    r8 = r6;
    r9 = r7;
    r10 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r10 + 0x0);
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC26C;
    r5 = r3;
    r6 = r4;
    r7 = r11;
    r3 = 0x48;
    r4 = 0x5;
    /* crclr cr1eq */;
    fn_800D4F98();
    goto .L_800DC288;
.L_800DC26C:
    r3 = r3 << 2;
    r3 = r3 + 0x42e;
    r3 = r10 + r3;
    *(u8*)((u8*)r3 + 0x0) = r4;
    *(u8*)((u8*)r3 + 0x1) = r11;
    *(u8*)((u8*)r3 + 0x2) = r8;
    *(u8*)((u8*)r3 + 0x3) = r9;
.L_800DC288:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DC298 | Size: 0xF8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC298(void) {
    r4 = (u32)lbl_80400EE0;
    r31 = (u32)lbl_80400EE0;
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC2CC;
    r0 = *(u32*)((u8*)r31 + 0x4);
    /* cmplw r0, r3 */;
    if (/* ne */) goto .L_800DC2CC;
    goto .L_800DC324;
.L_800DC2CC:
    r0 = *(u8*)((u8*)r31 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC2E8;
    r0 = *(u32*)((u8*)r31 + 0x4);
    /* cmplw r0, r3 */;
    if (/* ne */) goto .L_800DC2E8;
    goto .L_800DC324;
.L_800DC2E8:
    r0 = *(u8*)((u8*)r31 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC304;
    r0 = *(u32*)((u8*)r31 + 0x4);
    /* cmplw r0, r3 */;
    if (/* ne */) goto .L_800DC304;
    goto .L_800DC324;
.L_800DC304:
    r0 = *(u8*)((u8*)r31 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC320;
    r0 = *(u32*)((u8*)r31 + 0x4);
    /* cmplw r0, r3 */;
    if (/* ne */) goto .L_800DC320;
    goto .L_800DC324;
.L_800DC320:
    r31 = 0x0;
.L_800DC324:
    /* cmplwi r31, 0x0 */;
    if (/* ne */) goto .L_800DC334;
    r3 = 0x0;
    goto .L_800DC37C;
.L_800DC334:
    r3 = *(u32*)((u8*)r31 + 0x4);
    fn_800EF504();
    r4 = 0x0;
    r3 = (u32)lbl_80400EE0;
    *(u8*)((u8*)r31 + 0x0) = r4;
    r0 = 0x4;
    r3 = (u32)lbl_80400EE0;
    *(u8*)lbl_8047AAE0 = r4;
    /* mtctr r0 */;
.L_800DC358:
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC370;
    r0 = 0x1;
    *(u8*)lbl_8047AAE0 = r0;
    goto .L_800DC378;
.L_800DC370:
    r3 = r3 + 0x14;
    if (--ctr != 0) goto .L_800DC358;
.L_800DC378:
    r3 = 0x1;
.L_800DC37C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DC390 | Size: 0x1B0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC390(void) {
    r30 = r5;
    r29 = r4;
    r28 = r3;
    fn_800EF4DC();
    /* cmpwi r3, 0x90 */;
    if (/* eq */) goto .L_800DC3E0;
    if (/* ge */) goto .L_800DC3D8;
    /* cmpwi r3, 0x46 */;
    if (/* ge */) goto .L_800DC3D8;
    /* cmpwi r3, 0x40 */;
    if (/* ge */) goto .L_800DC3E0;
.L_800DC3D8:
    r3 = 0x0;
    goto .L_800DC520;
.L_800DC3E0:
    r3 = (u32)lbl_80400EE0;
    r0 = *(u8*)lbl_80400EE0;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC400;
    r0 = *(u32*)((u8*)r3 + 0x4);
    /* cmplw r0, r28 */;
    if (/* ne */) goto .L_800DC400;
    goto .L_800DC458;
.L_800DC400:
    r0 = *(u8*)((u8*)r3 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC41C;
    r0 = *(u32*)((u8*)r3 + 0x4);
    /* cmplw r0, r28 */;
    if (/* ne */) goto .L_800DC41C;
    goto .L_800DC458;
.L_800DC41C:
    r0 = *(u8*)((u8*)r3 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC438;
    r0 = *(u32*)((u8*)r3 + 0x4);
    /* cmplw r0, r28 */;
    if (/* ne */) goto .L_800DC438;
    goto .L_800DC458;
.L_800DC438:
    r0 = *(u8*)((u8*)r3 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC454;
    r0 = *(u32*)((u8*)r3 + 0x4);
    /* cmplw r0, r28 */;
    if (/* ne */) goto .L_800DC454;
    goto .L_800DC458;
.L_800DC454:
    r3 = 0x0;
.L_800DC458:
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800DC468;
    r3 = 0x0;
    goto .L_800DC520;
.L_800DC468:
    r3 = (u32)lbl_80400EE0;
    r4 = 0x0;
    r31 = (u32)lbl_80400EE0;
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DC484;
    goto .L_800DC4B8;
.L_800DC484:
    r0 = *(u8*)((u8*)r31 + 0x14);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DC494;
    goto .L_800DC4B8;
.L_800DC494:
    r0 = *(u8*)((u8*)r31 + 0x14);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DC4A4;
    goto .L_800DC4B8;
.L_800DC4A4:
    r0 = *(u8*)((u8*)r31 + 0x14);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DC4B4;
    goto .L_800DC4B8;
.L_800DC4B4:
    r31 = r4;
.L_800DC4B8:
    /* cmplwi r31, 0x0 */;
    if (/* ne */) goto .L_800DC4C8;
    r3 = 0x0;
    goto .L_800DC520;
.L_800DC4C8:
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r3;
    r4 = 0x0;
    r5 = 0x0;
    *(u32*)((u8*)r31 + 0x4) = r28;
    *(u32*)((u8*)r31 + 0x8) = r29;
    *(u32*)((u8*)r31 + 0xC) = r30;
    *(u32*)((u8*)r31 + 0x10) = r0;
    r3 = *(u32*)((u8*)r31 + 0x4);
    fn_800EF590();
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x2;
    r5 = 0x2;
    r6 = 0x0;
    fn_800EF578();
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x0;
    fn_800EF548();
    r0 = 0x1;
    r3 = 0x1;
    *(u8*)lbl_8047AAE0 = r0;
.L_800DC520:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r28 = *(u32*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800DC540 | Size: 0x20 */
void fn_800DC540(void) {
    *(u8*)((u8*)r3 + 0x14) = r0;
    *(u8*)((u8*)r3 + 0x28) = r0;
    *(u8*)((u8*)r3 + 0x3C) = r0;
}

/* fn_800DC560 | Size: 0x178 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC560(void) {
    r0 = *(u8*)lbl_8047AAE0;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DC6BC;
    r3 = (u32)lbl_80400EE0;
    r30 = 0x0;
    r0 = (u32)lbl_80400EE0;
    r31 = r0;
.L_800DC594:
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC6AC;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x0;
    fn_800EF1E8();
    r12 = *(u32*)((u8*)r31 + 0x8);
    /* cmplwi r12, 0x0 */;
    if (/* eq */) goto .L_800DC6A0;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = *(u32*)((u8*)r31 + 0x10);
    r5 = *(u32*)((u8*)r31 + 0xC);
    /* mtctr r12 */;
    /* indirect call via ctr */;
    r0 = r3 & 0xFF;
    if (/* ne */) goto .L_800DC6A0;
    r3 = (u32)lbl_80400EE0;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r29 = (u32)lbl_80400EE0;
    r0 = *(u8*)((u8*)r29 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC5FC;
    r0 = *(u32*)((u8*)r29 + 0x4);
    /* cmplw r0, r4 */;
    if (/* ne */) goto .L_800DC5FC;
    goto .L_800DC654;
.L_800DC5FC:
    r0 = *(u8*)((u8*)r29 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC618;
    r0 = *(u32*)((u8*)r29 + 0x4);
    /* cmplw r0, r4 */;
    if (/* ne */) goto .L_800DC618;
    goto .L_800DC654;
.L_800DC618:
    r0 = *(u8*)((u8*)r29 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC634;
    r0 = *(u32*)((u8*)r29 + 0x4);
    /* cmplw r0, r4 */;
    if (/* ne */) goto .L_800DC634;
    goto .L_800DC654;
.L_800DC634:
    r0 = *(u8*)((u8*)r29 + 0x14);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC650;
    r0 = *(u32*)((u8*)r29 + 0x4);
    /* cmplw r0, r4 */;
    if (/* ne */) goto .L_800DC650;
    goto .L_800DC654;
.L_800DC650:
    r29 = 0x0;
.L_800DC654:
    /* cmplwi r29, 0x0 */;
    if (/* eq */) goto .L_800DC6A0;
    r3 = *(u32*)((u8*)r29 + 0x4);
    fn_800EF504();
    r4 = 0x0;
    r3 = (u32)lbl_80400EE0;
    *(u8*)((u8*)r29 + 0x0) = r4;
    r0 = 0x4;
    r3 = (u32)lbl_80400EE0;
    *(u8*)lbl_8047AAE0 = r4;
    /* mtctr r0 */;
.L_800DC680:
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC698;
    r0 = 0x1;
    *(u8*)lbl_8047AAE0 = r0;
    goto .L_800DC6A0;
.L_800DC698:
    r3 = r3 + 0x14;
    if (--ctr != 0) goto .L_800DC680;
.L_800DC6A0:
    r3 = *(u32*)((u8*)r31 + 0x10);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x10) = r0;
.L_800DC6AC:
    r30 = r30 + 0x1;
    r31 = r31 + 0x14;
    /* cmplwi r30, 0x4 */;
    if (/* lt */) goto .L_800DC594;
.L_800DC6BC:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DC6D8 | Size: 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC6D8(void) {
    r31 = 0x0;
    r30 = 0x0;
    r28 = r3;
    goto .L_800DC848;
.L_800DC704:
    r0 = *(u32*)lbl_8047AAEC;
    r29 = r0 + r31;
    r0 = *(u8*)((u8*)r29 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC840;
    r0 = *(u8*)((u8*)r29 + 0x3);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC840;
    r3 = *(u32*)((u8*)r29 + 0xC);
    f1 = *(f32*)((u8*)r29 + 0x68);
    fn_801A6370();
    r3 = *(u32*)((u8*)r29 + 0xC);
    fn_801A6408();
    r3 = (0x4330 << 16);
    r0 = *(u8*)((u8*)r29 + 0x71);
    r0 = (s8)r0;
    f2 = *(f64*)lbl_8047CA80;
    /* cmpwi r0, -0x1 */;
    f3 = *(f32*)((u8*)r29 + 0x64);
    f0 = *(f64*)((u8*)r1 + 0x8);
    f1 = *(f32*)((u8*)r29 + 0x6C);
    f2 = f0 - f2;
    f0 = *(f32*)lbl_8047CA70;
    r3 = *(u32*)((u8*)r29 + 0x5C);
    f4 = f1 - f0;
    f1 = f3 * f2;
    if (/* ne */) goto .L_800DC788;
    f0 = *(f32*)((u8*)r29 + 0x68);
    f0 = f0 - f1;
    *(f32*)((u8*)r29 + 0x68) = f0;
    goto .L_800DC79C;
.L_800DC788:
    /* cmpwi r0, 0x1 */;
    if (/* ne */) goto .L_800DC79C;
    f0 = *(f32*)((u8*)r29 + 0x68);
    f0 = f0 + f1;
    *(f32*)((u8*)r29 + 0x68) = f0;
.L_800DC79C:
    /* cmpwi r3, 0x1 */;
    if (/* eq */) goto .L_800DC7F0;
    if (/* ge */) goto .L_800DC7B4;
    /* cmpwi r3, 0x0 */;
    if (/* ge */) goto .L_800DC7C0;
    goto .L_800DC840;
.L_800DC7B4:
    /* cmpwi r3, 0x3 */;
    if (/* ge */) goto .L_800DC840;
    goto .L_800DC80C;
.L_800DC7C0:
    f0 = *(f32*)lbl_8047CA74;
    f1 = *(f32*)((u8*)r29 + 0x68);
    f0 = f4 - f0;
    /* fcmpo cr0, f1, f0 */;
    /* cror eq, gt, eq */;
    if (/* ne */) goto .L_800DC840;
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r29 + 0x70) = r3;
    *(u8*)((u8*)r29 + 0x71) = r0;
    *(f32*)((u8*)r29 + 0x68) = f0;
    goto .L_800DC840;
.L_800DC7F0:
    f0 = *(f32*)((u8*)r29 + 0x68);
    /* fcmpo cr0, f0, f4 */;
    /* cror eq, gt, eq */;
    if (/* ne */) goto .L_800DC840;
    f0 = f0 - f4;
    *(f32*)((u8*)r29 + 0x68) = f0;
    goto .L_800DC840;
.L_800DC80C:
    f1 = *(f32*)((u8*)r29 + 0x68);
    /* fcmpo cr0, f1, f4 */;
    /* cror eq, gt, eq */;
    if (/* ne */) goto .L_800DC828;
    r0 = -0x1;
    *(u8*)((u8*)r29 + 0x71) = r0;
    goto .L_800DC840;
.L_800DC828:
    f0 = *(f32*)lbl_8047CA78;
    /* fcmpo cr0, f1, f0 */;
    /* cror eq, lt, eq */;
    if (/* ne */) goto .L_800DC840;
    r0 = 0x1;
    *(u8*)((u8*)r29 + 0x71) = r0;
.L_800DC840:
    r31 = r31 + 0x74;
    r30 = r30 + 0x1;
.L_800DC848:
    r0 = *(u32*)lbl_8047AAF0;
    /* cmplw r30, r0 */;
    if (/* lt */) goto .L_800DC704;
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r28 = *(u32*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800DC874 | Size: 0x4 */
void fn_800DC874(void) {
    /* 4 bytes -- blr (empty function) */
}

/* fn_800DC878 | Size: 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DC878(void) {
    *(f64*)((u8*)r1 + 0x20) = f31;
    /* psq_st f31, 0x28(r1), 0, qr0 */;
    r30 = r3;
    r31 = r4;
    r0 = *(u8*)((u8*)r3 + 0x1);
    r4 = r31 + 0x4;
    *(u8*)((u8*)r31 + 0x0) = r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A49C0();
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = r31 + 0x10;
    fn_801A48F4();
    r0 = *(u8*)((u8*)r30 + 0x2);
    r29 = *(u32*)((u8*)r31 + 0x1C);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DC944;
    r3 = *(u32*)((u8*)r30 + 0xC);
    fn_801A66E0();
    r0 = *(u32*)((u8*)r30 + 0x58);
    /* cmplw r29, r0 */;
    if (/* gt */) goto .L_800DC944;
    *(u32*)((u8*)r30 + 0x60) = r29;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)((u8*)r30 + 0x60);
    r4 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 << 2;
    r3 = *(u32*)((u8*)r30 + 0xC);
    /* lwzx r4, r4, r0 */;
    fn_801A426C();
    r3 = *(u32*)((u8*)r30 + 0xC);
    f1 = *(f32*)lbl_8047CA78;
    fn_801A6370();
    f0 = *(f32*)lbl_8047CA78;
    r3 = (u32)fn_800DD128;
    r6 = (u32)fn_800DD128;
    r4 = 0x7;
    *(f32*)lbl_8047AAF4 = f0;
    r3 = (0x1 << 16);
    /* subi r5, r3, 0x1 */;
    r7 = 0x0;
    r3 = *(u32*)((u8*)r30 + 0xC);
    /* crclr cr1eq */;
    fn_801C028C();
    f0 = *(f32*)lbl_8047AAF4;
    *(f32*)((u8*)r30 + 0x6C) = f0;
.L_800DC944:
    r0 = *(u8*)((u8*)r30 + 0x2);
    f0 = *(f32*)((u8*)r31 + 0x20);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DC958;
    *(f32*)((u8*)r30 + 0x68) = f0;
.L_800DC958:
    r0 = *(u8*)((u8*)r30 + 0x2);
    f31 = *(f32*)((u8*)r31 + 0x24);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DC9A8;
    fn_800D37CC();
    /* cmpwi r3, 0x32 */;
    if (/* ne */) goto .L_800DC97C;
    f0 = *(f32*)lbl_8047CA88;
    f31 = f31 * f0;
.L_800DC97C:
    *(f32*)((u8*)r30 + 0x64) = f31;
    r3 = (u32)fn_801C027C;
    r6 = (u32)fn_801C027C;
    r4 = (0x1 << 16);
    r3 = *(u32*)((u8*)r30 + 0xC);
    /* subi r5, r4, 0x1 */;
    f1 = *(f32*)((u8*)r30 + 0x64);
    r4 = 0x7;
    r7 = 0x1;
    /* crset cr1eq */;
    fn_801C028C();
.L_800DC9A8:
    r0 = *(u32*)((u8*)r31 + 0x28);
    *(u32*)((u8*)r30 + 0x5C) = r0;
    r0 = *(u8*)((u8*)r31 + 0x2);
    *(u8*)((u8*)r30 + 0x70) = r0;
    r0 = *(u8*)((u8*)r31 + 0x3);
    *(u8*)((u8*)r30 + 0x71) = r0;
    r0 = *(u8*)((u8*)r30 + 0x3);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DC9EC;
    r0 = *(u8*)((u8*)r30 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DC9EC;
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x3) = r3;
    *(u8*)((u8*)r30 + 0x70) = r0;
    *(u8*)((u8*)r30 + 0x71) = r3;
.L_800DC9EC:
    /* psq_l f31, 0x28(r1), 0, qr0 */;
    r0 = *(u32*)((u8*)r1 + 0x34);
    f31 = *(f64*)((u8*)r1 + 0x20);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DCA10 | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCA10(void) {
    r31 = r4;
    r4 = r31 + 0x4;
    r30 = r3;
    r0 = *(u8*)((u8*)r3 + 0x1);
    *(u8*)((u8*)r31 + 0x0) = r0;
    r0 = *(u8*)((u8*)r3 + 0x3);
    *(u8*)((u8*)r31 + 0x1) = r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A497C();
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = r31 + 0x10;
    fn_801A48B0();
    r0 = *(u32*)((u8*)r30 + 0x60);
    *(u32*)((u8*)r31 + 0x1C) = r0;
    f0 = *(f32*)((u8*)r30 + 0x68);
    *(f32*)((u8*)r31 + 0x20) = f0;
    f0 = *(f32*)((u8*)r30 + 0x64);
    *(f32*)((u8*)r31 + 0x24) = f0;
    r0 = *(u32*)((u8*)r30 + 0x5C);
    *(u32*)((u8*)r31 + 0x28) = r0;
    r0 = *(u8*)((u8*)r30 + 0x70);
    *(u8*)((u8*)r31 + 0x2) = r0;
    r0 = *(u8*)((u8*)r30 + 0x71);
    *(u8*)((u8*)r31 + 0x3) = r0;
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DCAA4 | Size: 0xC */
void fn_800DCAA4(void) {
    *(u8*)((u8*)obj + 0x3) = 0;
}

/* fn_800DCAB0 | Size: 0x24 */
void fn_800DCAB0(void) {
    *(u8*)((u8*)r3 + 0x3) = r4;
    *(u8*)((u8*)r3 + 0x70) = r0;
    *(u8*)((u8*)r3 + 0x71) = r4;
}

/* fn_800DCADC | Size: 0x14 */
void fn_800DCADC(void) {
    *(f32*)((u8*)r3 + 0x68) = f1;
}

/* fn_800DCAF0 | Size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCAF0(void) {
    *(f64*)((u8*)r1 + 0x10) = f31;
    /* psq_st f31, 0x18(r1), 0, qr0 */;
    r31 = r3;
    f31 = f1;
    r0 = *(u8*)((u8*)r3 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DCB5C;
    fn_800D37CC();
    /* cmpwi r3, 0x32 */;
    if (/* ne */) goto .L_800DCB30;
    f0 = *(f32*)lbl_8047CA88;
    f31 = f31 * f0;
.L_800DCB30:
    *(f32*)((u8*)r31 + 0x64) = f31;
    r3 = (u32)fn_801C027C;
    r6 = (u32)fn_801C027C;
    r4 = (0x1 << 16);
    r3 = *(u32*)((u8*)r31 + 0xC);
    /* subi r5, r4, 0x1 */;
    f1 = *(f32*)((u8*)r31 + 0x64);
    r4 = 0x7;
    r7 = 0x1;
    /* crset cr1eq */;
    fn_801C028C();
.L_800DCB5C:
    /* psq_l f31, 0x18(r1), 0, qr0 */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    f31 = *(f64*)((u8*)r1 + 0x10);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DCB78 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCB78(void) {
    r31 = r4;
    r30 = r3;
    r0 = *(u8*)((u8*)r3 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DCC14;
    r3 = *(u32*)((u8*)r30 + 0xC);
    fn_801A66E0();
    r0 = *(u32*)((u8*)r30 + 0x58);
    /* cmplw r31, r0 */;
    if (/* gt */) goto .L_800DCC14;
    *(u32*)((u8*)r30 + 0x60) = r31;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)((u8*)r30 + 0x60);
    r4 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 << 2;
    r3 = *(u32*)((u8*)r30 + 0xC);
    /* lwzx r4, r4, r0 */;
    fn_801A426C();
    r3 = *(u32*)((u8*)r30 + 0xC);
    f1 = *(f32*)lbl_8047CA78;
    fn_801A6370();
    f0 = *(f32*)lbl_8047CA78;
    r3 = (u32)fn_800DD128;
    r6 = (u32)fn_800DD128;
    r4 = 0x7;
    *(f32*)lbl_8047AAF4 = f0;
    r3 = (0x1 << 16);
    /* subi r5, r3, 0x1 */;
    r7 = 0x0;
    r3 = *(u32*)((u8*)r30 + 0xC);
    /* crclr cr1eq */;
    fn_801C028C();
    f0 = *(f32*)lbl_8047AAF4;
    *(f32*)((u8*)r30 + 0x6C) = f0;
.L_800DCC14:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800DCC3C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCC3C(void) {
    /* lwz r3, 0xc(r3) */;
    fn_801A48F4();
}
#pragma pop

/* fn_800DCC60 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCC60(void) {
    /* lwz r3, 0xc(r3) */;
    fn_801A49C0();
}
#pragma pop

/* fn_800DCC84 | Size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCC84(void) {
    f2 = *(f32*)((u8*)r4 + 0x0);
    f1 = *(f32*)((u8*)r4 + 0x4);
    f0 = *(f32*)((u8*)r4 + 0x8);
    f2 = (f64)(s32)f2;
    f1 = (f64)(s32)f1;
    r4 = r1 + 0x8;
    f0 = (f64)(s32)f0;
    *(f64*)((u8*)r1 + 0x10) = f2;
    *(f64*)((u8*)r1 + 0x18) = f1;
    r6 = *(u32*)((u8*)r1 + 0x14);
    *(f64*)((u8*)r1 + 0x20) = f0;
    r5 = *(u32*)((u8*)r1 + 0x1C);
    r0 = *(u32*)((u8*)r1 + 0x24);
    *(u8*)((u8*)r1 + 0xC) = r6;
    *(u8*)((u8*)r1 + 0xD) = r5;
    *(u8*)((u8*)r1 + 0xE) = r0;
    r0 = *(u32*)((u8*)r1 + 0xC);
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A4A48();
    r0 = *(u32*)((u8*)r1 + 0x34);
    return;
}
#pragma pop

/* fn_800DCCF0 | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCCF0(void) {
    r31 = r4;
    r4 = 0x3;
    r30 = r3;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A68F8();
    /* cmpwi r31, 0x2 */;
    if (/* eq */) goto .L_800DCD60;
    if (/* ge */) goto .L_800DCD34;
    /* cmpwi r31, 0x0 */;
    if (/* eq */) goto .L_800DCD40;
    if (/* ge */) goto .L_800DCD50;
    goto .L_800DCD7C;
.L_800DCD34:
    /* cmpwi r31, 0x4 */;
    if (/* ge */) goto .L_800DCD7C;
    goto .L_800DCD70;
.L_800DCD40:
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x0;
    fn_801A6910();
    goto .L_800DCD7C;
.L_800DCD50:
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x1;
    fn_801A6910();
    goto .L_800DCD7C;
.L_800DCD60:
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x2;
    fn_801A6910();
    goto .L_800DCD7C;
.L_800DCD70:
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x3;
    fn_801A6910();
.L_800DCD7C:
    *(u32*)((u8*)r30 + 0x4) = r31;
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800DCD98 | Size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCD98(void) {
    r30 = r3;
    r31 = *(u32*)((u8*)r3 + 0xC);
    /* cmplwi r31, 0x0 */;
    if (/* eq */) goto .L_800DCE28;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r31 + 0x4);
    /* subi r0, r3, 0x1 */;
    r0 = r0 & 0xFFFF;
    r0 = r0 - r4;
    /* cntlzw r0, r0 */;
    /* srwi. r0, r0, 5 */;
    if (/* eq */) goto .L_800DCDE0;
    goto .L_800DCDF0;
.L_800DCDE0:
    /* subi r3, r4, 0x1 */;
    /* cntlzw r0, r4 */;
    *(u16*)((u8*)r31 + 0x4) = r3;
    r0 = (u32)r0 >> 5;
.L_800DCDF0:
    /* cmpwi r0, 0x0 */;
    if (/* eq */) goto .L_800DCE28;
    /* cmplwi r31, 0x0 */;
    if (/* eq */) goto .L_800DCE28;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x30);
    /* mtctr r12 */;
    /* indirect call via ctr */;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x34);
    /* mtctr r12 */;
    /* indirect call via ctr */;
.L_800DCE28:
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x1) = r0;
    *(u8*)((u8*)r30 + 0x0) = r0;
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DCE4C | Size: 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCE4C(void) {
    r0 = *(u32*)lbl_8047AAF0;
    r31 = *(u32*)lbl_8047AAEC;
    /* mtctr r0 */;
    /* cmplwi r0, 0x0 */;
    if (/* le */) goto .L_800DCE88;
.L_800DCE70:
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DCE80;
    goto .L_800DCE8C;
.L_800DCE80:
    r31 = r31 + 0x74;
    if (--ctr != 0) goto .L_800DCE70;
.L_800DCE88:
    r31 = 0x0;
.L_800DCE8C:
    /* cmplwi r31, 0x0 */;
    if (/* ne */) goto .L_800DCE9C;
    r3 = 0x0;
    goto .L_800DCFA8;
.L_800DCE9C:
    *(u32*)((u8*)r31 + 0x8) = r3;
    r3 = *(u32*)((u8*)r31 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x0);
    fn_801A4344();
    *(u32*)((u8*)r31 + 0xC) = r3;
    r5 = 0x1;
    r4 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r5;
    *(u8*)((u8*)r31 + 0x1) = r4;
    *(u8*)((u8*)r31 + 0x3) = r4;
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r3 + 0x4);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DCFA0;
    *(u8*)((u8*)r31 + 0x2) = r5;
    f0 = *(f32*)lbl_8047CA70;
    *(f32*)((u8*)r31 + 0x64) = f0;
    *(u32*)((u8*)r31 + 0x5C) = r5;
    *(u8*)((u8*)r31 + 0x70) = r4;
    *(u32*)((u8*)r31 + 0x58) = r4;
    goto .L_800DCEFC;
.L_800DCEF0:
    r3 = *(u32*)((u8*)r31 + 0x58);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x58) = r0;
.L_800DCEFC:
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0x58);
    r3 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 << 2;
    /* lwzx r0, r3, r0 */;
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DCEF0;
    r0 = *(u8*)((u8*)r31 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DCFA4;
    r3 = *(u32*)((u8*)r31 + 0xC);
    fn_801A66E0();
    r0 = *(u32*)((u8*)r31 + 0x58);
    /* cmplwi r0, 0x0 */;
    if (/* lt */) goto .L_800DCFA4;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x60) = r0;
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0x60);
    r4 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 << 2;
    r3 = *(u32*)((u8*)r31 + 0xC);
    /* lwzx r4, r4, r0 */;
    fn_801A426C();
    r3 = *(u32*)((u8*)r31 + 0xC);
    f1 = *(f32*)lbl_8047CA78;
    fn_801A6370();
    f0 = *(f32*)lbl_8047CA78;
    r3 = (u32)fn_800DD128;
    r6 = (u32)fn_800DD128;
    r4 = 0x7;
    *(f32*)lbl_8047AAF4 = f0;
    r3 = (0x1 << 16);
    /* subi r5, r3, 0x1 */;
    r7 = 0x0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    /* crclr cr1eq */;
    fn_801C028C();
    f0 = *(f32*)lbl_8047AAF4;
    *(f32*)((u8*)r31 + 0x6C) = f0;
    goto .L_800DCFA4;
.L_800DCFA0:
    *(u8*)((u8*)r31 + 0x2) = r4;
.L_800DCFA4:
    r3 = r31;
.L_800DCFA8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DCFBC | Size: 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DCFBC(void) {
    r0 = *(u32*)lbl_8047AAF0;
    r31 = *(u32*)lbl_8047AAEC;
    /* mtctr r0 */;
    /* cmplwi r0, 0x0 */;
    if (/* le */) goto .L_800DCFF8;
.L_800DCFE0:
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DCFF0;
    goto .L_800DCFFC;
.L_800DCFF0:
    r31 = r31 + 0x74;
    if (--ctr != 0) goto .L_800DCFE0;
.L_800DCFF8:
    r31 = 0x0;
.L_800DCFFC:
    /* cmplwi r31, 0x0 */;
    if (/* ne */) goto .L_800DD00C;
    r3 = 0x0;
    goto .L_800DD0A4;
.L_800DD00C:
    r8 = 0x0;
    f1 = *(f32*)lbl_8047CA78;
    *(u32*)((u8*)r31 + 0x10) = r8;
    r7 = 0x4;
    f0 = *(f32*)lbl_8047CA8C;
    r6 = 0x80;
    *(f32*)((u8*)r31 + 0x14) = f1;
    r5 = r31 + 0x10;
    r4 = r31 + 0x24;
    r0 = r31 + 0x54;
    *(f32*)((u8*)r31 + 0x18) = f1;
    r3 = r31 + 0x38;
    *(f32*)((u8*)r31 + 0x1C) = f1;
    *(u32*)((u8*)r31 + 0x20) = r8;
    *(u32*)((u8*)r31 + 0x24) = r8;
    *(f32*)((u8*)r31 + 0x28) = f1;
    *(f32*)((u8*)r31 + 0x2C) = f1;
    *(f32*)((u8*)r31 + 0x30) = f1;
    *(u32*)((u8*)r31 + 0x34) = r8;
    *(f32*)((u8*)r31 + 0x54) = f0;
    *(u32*)((u8*)r31 + 0x38) = r8;
    *(u32*)((u8*)r31 + 0x3C) = r8;
    *(u16*)((u8*)r31 + 0x40) = r7;
    *(u16*)((u8*)r31 + 0x42) = r8;
    *(u8*)((u8*)r31 + 0x44) = r6;
    *(u8*)((u8*)r31 + 0x45) = r6;
    *(u8*)((u8*)r31 + 0x46) = r6;
    *(u8*)((u8*)r31 + 0x47) = r8;
    *(u32*)((u8*)r31 + 0x48) = r5;
    *(u32*)((u8*)r31 + 0x4C) = r4;
    *(u32*)((u8*)r31 + 0x50) = r0;
    fn_801A4344();
    *(u32*)((u8*)r31 + 0xC) = r3;
    r4 = 0x1;
    r0 = 0x0;
    r3 = r31;
    *(u8*)((u8*)r31 + 0x0) = r4;
    *(u8*)((u8*)r31 + 0x1) = r0;
.L_800DD0A4:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DD0B8 | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD0B8(void) {
    r0 = r3;
    r3 = r0 * 0x74;
    *(u32*)lbl_8047AAF0 = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAE8 = r3;
    if (/* eq */) goto .L_800DD118;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AAEC = r3;
    r4 = r5;
    r6 = 0x0;
    goto .L_800DD10C;
.L_800DD0FC:
    r3 = *(u32*)lbl_8047AAEC;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x74;
.L_800DD10C:
    r0 = *(u32*)lbl_8047AAF0;
    /* cmplw r6, r0 */;
    if (/* lt */) goto .L_800DD0FC;
.L_800DD118:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DD128 | Size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD128(void) {
    /* mr. r31, r3 */;
    if (/* ne */) goto .L_800DD150;
    r3 = lbl_8047CA90@sda21;
    r4 = 0xab;
    r5 = lbl_8047CA98@sda21;
    fn_80196E10();
.L_800DD150:
    f1 = *(f32*)lbl_8047CA70;
    f0 = *(f32*)((u8*)r31 + 0xC);
    f0 = f1 + f0;
    *(f32*)lbl_8047AAF4 = f0;
    r31 = *(u32*)((u8*)r1 + 0xC);
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DD174 | Size: 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD174(void) {
    r31 = r3;
    r3 = 0x0;
    fn_801A4B00();
    r5 = *(u32*)lbl_8047AAEC;
    r3 = 0x0;
    r0 = *(u32*)lbl_8047AAF0;
    r4 = r5;
    /* mtctr r0 */;
    /* cmplwi r0, 0x0 */;
    if (/* le */) goto .L_800DD1D4;
.L_800DD1AC:
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DD1C8;
    r0 = *(u8*)((u8*)r4 + 0x1);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DD1C8;
    goto .L_800DD1D8;
.L_800DD1C8:
    r4 = r4 + 0x74;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto .L_800DD1AC;
.L_800DD1D4:
    r3 = -0x1;
.L_800DD1D8:
    /* cmpwi r3, -0x1 */;
    if (/* eq */) goto .L_800DD254;
    r6 = r3 * 0x74;
    r7 = r3 + 0x1;
    r4 = r7 * 0x74;
    r5 = r5 + r6;
    goto .L_800DD22C;
.L_800DD1F4:
    r0 = *(u32*)lbl_8047AAEC;
    r8 = r0 + r4;
    r0 = *(u8*)((u8*)r8 + 0x0);
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DD224;
    r0 = *(u8*)((u8*)r8 + 0x1);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DD224;
    r3 = *(u32*)((u8*)r5 + 0xC);
    r5 = r8;
    r0 = *(u32*)((u8*)r8 + 0xC);
    *(u32*)((u8*)r3 + 0xC) = r0;
.L_800DD224:
    r4 = r4 + 0x74;
    r7 = r7 + 0x1;
.L_800DD22C:
    r0 = *(u32*)lbl_8047AAF0;
    /* cmplw r7, r0 */;
    if (/* lt */) goto .L_800DD1F4;
    r3 = *(u32*)((u8*)r5 + 0xC);
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0xC) = r0;
    r0 = *(u32*)lbl_8047AAEC;
    r3 = r0 + r6;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A4D20();
.L_800DD254:
    r3 = r31;
    fn_801A4F54();
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DD270 | Size: 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD270(void) {
    r31 = r3;
    r0 = *(u16*)lbl_8047AAF8;
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DD298;
    r3 = 0x0;
    goto .L_800DD370;
.L_800DD298:
    r0 = *(u32*)lbl_8047AB08;
    /* cmplw r31, r0 */;
    if (/* le */) goto .L_800DD2AC;
    r3 = 0x0;
    goto .L_800DD370;
.L_800DD2AC:
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E27B0();
    /* cmplwi r31, 0x0 */;
    r6 = 0x0;
    r7 = 0x0;
    if (/* le */) goto .L_800DD35C;
    /* cmplwi r31, 0x8 */;
    /* subi r4, r31, 0x8 */;
    if (/* le */) goto .L_800DD334;
    r0 = r4 + 0x7;
    r5 = r3;
    r0 = (u32)r0 >> 3;
    /* mtctr r0 */;
    /* cmplwi r4, 0x0 */;
    if (/* le */) goto .L_800DD334;
.L_800DD2E8:
    r4 = *(u16*)((u8*)r5 + 0x0);
    r7 = r7 + 0x8;
    r0 = *(u16*)((u8*)r5 + 0x2);
    r6 = r6 + r4;
    r4 = *(u16*)((u8*)r5 + 0x4);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0x6);
    r6 = r6 + r4;
    r4 = *(u16*)((u8*)r5 + 0x8);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0xA);
    r6 = r6 + r4;
    r4 = *(u16*)((u8*)r5 + 0xC);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0xE);
    r6 = r6 + r4;
    r5 = r5 + 0x10;
    r6 = r6 + r0;
    if (--ctr != 0) goto .L_800DD2E8;
.L_800DD334:
    r4 = r7 << 1;
    r0 = r31 - r7;
    r3 = r3 + r4;
    /* mtctr r0 */;
    /* cmplw r7, r31 */;
    if (/* ge */) goto .L_800DD35C;
.L_800DD34C:
    r0 = *(u16*)((u8*)r3 + 0x0);
    r3 = r3 + 0x2;
    r6 = r6 + r0;
    if (--ctr != 0) goto .L_800DD34C;
.L_800DD35C:
    r0 = *(u32*)lbl_8047AAFC;
    r3 = *(u16*)lbl_8047AAFA;
    r31 = r0 + r6;
    fn_800E24B0();
    r3 = r31;
.L_800DD370:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DD38C | Size: 0x5E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD38C(void) {
    r30 = r3;
    if (/* ne */) goto .L_800DD3C8;
    *(f64*)((u8*)r1 + 0x28) = f1;
    *(f64*)((u8*)r1 + 0x30) = f2;
    *(f64*)((u8*)r1 + 0x38) = f3;
    *(f64*)((u8*)r1 + 0x40) = f4;
    *(f64*)((u8*)r1 + 0x48) = f5;
    *(f64*)((u8*)r1 + 0x50) = f6;
    *(f64*)((u8*)r1 + 0x58) = f7;
    *(f64*)((u8*)r1 + 0x60) = f8;
.L_800DD3C8:
    r0 = *(u8*)lbl_8047AB11;
    r12 = r1 + 0xb8;
    r11 = r1 + 0x8;
    r31 = (0x100 << 16);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DD44C;
    OSGetTime();
    r5 = r1 + 0x74;
    fn_800A2998();
    r4 = *(u32*)((u8*)r1 + 0x84);
    r3 = (u32)lbl_80400F30;
    r5 = (u32)lbl_802704B4;
    r7 = *(u32*)((u8*)r1 + 0x80);
    r6 = r4 + 0x1;
    r8 = *(u32*)((u8*)r1 + 0x7C);
    r9 = *(u32*)((u8*)r1 + 0x78);
    r3 = (u32)lbl_80400F30;
    r10 = *(u32*)((u8*)r1 + 0x74);
    r5 = (u32)lbl_802704B4;
    r4 = 0x14;
    /* crclr cr1eq */;
    fn_800DE09C();
.L_800DD44C:
    r3 = (u32)lbl_80400F44;
    r5 = r30;
    r3 = (u32)lbl_80400F44;
    r6 = r1 + 0x68;
    r4 = 0xff;
    fn_800DE680();
    r0 = *(u32*)lbl_8047AAFC;
    r3 = (u32)lbl_80400F44;
    r3 = (u32)lbl_80400F44;
    r4 = 0x0;
    /* cmplwi r0, 0x0 */;
    *(u8*)((u8*)r3 + 0xFF) = r4;
    if (/* eq */) goto .L_800DD934;
    r0 = *(u8*)lbl_8047AB11;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DD4B0;
    strlen();
    r4 = (u32)lbl_80400F30;
    r31 = r3;
    r3 = (u32)lbl_80400F30;
    strlen();
    r0 = r31 + 0x1;
    r0 = r3 + r0;
    r31 = r0 & 0xFFFF;
    goto .L_800DD4BC;
.L_800DD4B0:
    strlen();
    r0 = r3 + 0x1;
    r31 = r0 & 0xFFFF;
.L_800DD4BC:
    r3 = r31 & 0xFFFF;
.L_800DD4C0:
    r9 = *(u32*)lbl_8047AB08;
    r0 = 0x1;
    r4 = *(u32*)lbl_8047AB0C;
    /* cmplw r9, r4 */;
    if (/* lt */) goto .L_800DD4D8;
    r0 = 0x0;
.L_800DD4D8:
    /* cmplwi r9, 0x0 */;
    r7 = 0x0;
    r10 = *(u32*)lbl_8047AB00;
    r6 = r7;
    if (/* le */) goto .L_800DD588;
    /* cmplwi r9, 0x8 */;
    /* subi r5, r9, 0x8 */;
    if (/* le */) goto .L_800DD55C;
    r4 = r5 + 0x7;
    r8 = r10;
    r4 = (u32)r4 >> 3;
    /* mtctr r4 */;
    /* cmplwi r5, 0x0 */;
    if (/* le */) goto .L_800DD55C;
.L_800DD510:
    r5 = *(u16*)((u8*)r8 + 0x0);
    r6 = r6 + 0x8;
    r4 = *(u16*)((u8*)r8 + 0x2);
    r7 = r7 + r5;
    r5 = *(u16*)((u8*)r8 + 0x4);
    r7 = r7 + r4;
    r4 = *(u16*)((u8*)r8 + 0x6);
    r7 = r7 + r5;
    r5 = *(u16*)((u8*)r8 + 0x8);
    r7 = r7 + r4;
    r4 = *(u16*)((u8*)r8 + 0xA);
    r7 = r7 + r5;
    r5 = *(u16*)((u8*)r8 + 0xC);
    r7 = r7 + r4;
    r4 = *(u16*)((u8*)r8 + 0xE);
    r7 = r7 + r5;
    r8 = r8 + 0x10;
    r7 = r7 + r4;
    if (--ctr != 0) goto .L_800DD510;
.L_800DD55C:
    r5 = r6 << 1;
    r4 = r9 - r6;
    r5 = r10 + r5;
    /* mtctr r4 */;
    /* cmplw r6, r9 */;
    if (/* ge */) goto .L_800DD588;
.L_800DD574:
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    r6 = r6 + 0x1;
    r7 = r7 + r4;
    if (--ctr != 0) goto .L_800DD574;
.L_800DD588:
    r5 = *(u32*)lbl_8047AB04;
    r4 = r3 + r7;
    /* cmplw r4, r5 */;
    if (/* lt */) goto .L_800DD59C;
    r0 = 0x0;
.L_800DD59C:
    r4 = r0 & 0xFF;
    if (/* ne */) goto .L_800DD7FC;
    r4 = *(u16*)((u8*)r10 + 0x0);
    r8 = *(u32*)lbl_8047AAFC;
    r6 = r5 - r4;
    r7 = r8 + r4;
    /* subf. r5, r8, r7 */;
    r4 = r7 - r8;
    r5 = -r4;
    if (/* eq */) goto .L_800DD5C8;
    r5 = r4;
.L_800DD5C8:
    /* cmplwi r5, 0x4 */;
    if (/* lt */) goto .L_800DD6E0;
    /* srwi. r4, r6, 2 */;
    r6 = r6 & 0x3;
    r5 = r4;
    if (/* eq */) goto .L_800DD658;
    /* srwi. r4, r4, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DD640;
.L_800DD5EC:
    r4 = *(u32*)((u8*)r7 + 0x0);
    *(u32*)((u8*)r8 + 0x0) = r4;
    r4 = *(u32*)((u8*)r7 + 0x4);
    *(u32*)((u8*)r8 + 0x4) = r4;
    r4 = *(u32*)((u8*)r7 + 0x8);
    *(u32*)((u8*)r8 + 0x8) = r4;
    r4 = *(u32*)((u8*)r7 + 0xC);
    *(u32*)((u8*)r8 + 0xC) = r4;
    r4 = *(u32*)((u8*)r7 + 0x10);
    *(u32*)((u8*)r8 + 0x10) = r4;
    r4 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r8 + 0x14) = r4;
    r4 = *(u32*)((u8*)r7 + 0x18);
    *(u32*)((u8*)r8 + 0x18) = r4;
    r4 = *(u32*)((u8*)r7 + 0x1C);
    r7 = r7 + 0x20;
    *(u32*)((u8*)r8 + 0x1C) = r4;
    r8 = r8 + 0x20;
    if (--ctr != 0) goto .L_800DD5EC;
    r5 = r5 & 0x7;
    if (/* eq */) goto .L_800DD658;
.L_800DD640:
    /* mtctr r5 */;
.L_800DD644:
    r4 = *(u32*)((u8*)r7 + 0x0);
    r7 = r7 + 0x4;
    *(u32*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x4;
    if (--ctr != 0) goto .L_800DD644;
.L_800DD658:
    /* cmplwi r6, 0x0 */;
    r5 = r6;
    if (/* eq */) goto .L_800DD764;
    /* srwi. r4, r6, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DD6C4;
.L_800DD670:
    r4 = *(u8*)((u8*)r7 + 0x0);
    *(u8*)((u8*)r8 + 0x0) = r4;
    r4 = *(u8*)((u8*)r7 + 0x1);
    *(u8*)((u8*)r8 + 0x1) = r4;
    r4 = *(u8*)((u8*)r7 + 0x2);
    *(u8*)((u8*)r8 + 0x2) = r4;
    r4 = *(u8*)((u8*)r7 + 0x3);
    *(u8*)((u8*)r8 + 0x3) = r4;
    r4 = *(u8*)((u8*)r7 + 0x4);
    *(u8*)((u8*)r8 + 0x4) = r4;
    r4 = *(u8*)((u8*)r7 + 0x5);
    *(u8*)((u8*)r8 + 0x5) = r4;
    r4 = *(u8*)((u8*)r7 + 0x6);
    *(u8*)((u8*)r8 + 0x6) = r4;
    r4 = *(u8*)((u8*)r7 + 0x7);
    r7 = r7 + 0x8;
    *(u8*)((u8*)r8 + 0x7) = r4;
    r8 = r8 + 0x8;
    if (--ctr != 0) goto .L_800DD670;
    r5 = r5 & 0x7;
    if (/* eq */) goto .L_800DD764;
.L_800DD6C4:
    /* mtctr r5 */;
.L_800DD6C8:
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto .L_800DD6C8;
    goto .L_800DD764;
.L_800DD6E0:
    /* cmplwi r6, 0x0 */;
    r5 = r6;
    if (/* eq */) goto .L_800DD764;
    /* srwi. r4, r6, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DD74C;
.L_800DD6F8:
    r4 = *(u8*)((u8*)r7 + 0x0);
    *(u8*)((u8*)r8 + 0x0) = r4;
    r4 = *(u8*)((u8*)r7 + 0x1);
    *(u8*)((u8*)r8 + 0x1) = r4;
    r4 = *(u8*)((u8*)r7 + 0x2);
    *(u8*)((u8*)r8 + 0x2) = r4;
    r4 = *(u8*)((u8*)r7 + 0x3);
    *(u8*)((u8*)r8 + 0x3) = r4;
    r4 = *(u8*)((u8*)r7 + 0x4);
    *(u8*)((u8*)r8 + 0x4) = r4;
    r4 = *(u8*)((u8*)r7 + 0x5);
    *(u8*)((u8*)r8 + 0x5) = r4;
    r4 = *(u8*)((u8*)r7 + 0x6);
    *(u8*)((u8*)r8 + 0x6) = r4;
    r4 = *(u8*)((u8*)r7 + 0x7);
    r7 = r7 + 0x8;
    *(u8*)((u8*)r8 + 0x7) = r4;
    r8 = r8 + 0x8;
    if (--ctr != 0) goto .L_800DD6F8;
    r5 = r5 & 0x7;
    if (/* eq */) goto .L_800DD764;
.L_800DD74C:
    /* mtctr r5 */;
.L_800DD750:
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto .L_800DD750;
.L_800DD764:
    r4 = *(u32*)lbl_8047AB08;
    r7 = *(u32*)lbl_8047AB00;
    /* subic. r6, r4, 0x1 */;
    r5 = r7 + 0x2;
    if (/* eq */) goto .L_800DD7F0;
    /* srwi. r4, r6, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DD7D8;
.L_800DD784:
    r4 = *(u16*)((u8*)r5 + 0x0);
    *(u16*)((u8*)r7 + 0x0) = r4;
    r4 = *(u16*)((u8*)r5 + 0x2);
    *(u16*)((u8*)r7 + 0x2) = r4;
    r4 = *(u16*)((u8*)r5 + 0x4);
    *(u16*)((u8*)r7 + 0x4) = r4;
    r4 = *(u16*)((u8*)r5 + 0x6);
    *(u16*)((u8*)r7 + 0x6) = r4;
    r4 = *(u16*)((u8*)r5 + 0x8);
    *(u16*)((u8*)r7 + 0x8) = r4;
    r4 = *(u16*)((u8*)r5 + 0xA);
    *(u16*)((u8*)r7 + 0xA) = r4;
    r4 = *(u16*)((u8*)r5 + 0xC);
    *(u16*)((u8*)r7 + 0xC) = r4;
    r4 = *(u16*)((u8*)r5 + 0xE);
    r5 = r5 + 0x10;
    *(u16*)((u8*)r7 + 0xE) = r4;
    r7 = r7 + 0x10;
    if (--ctr != 0) goto .L_800DD784;
    r6 = r6 & 0x7;
    if (/* eq */) goto .L_800DD7F0;
.L_800DD7D8:
    /* mtctr r6 */;
.L_800DD7DC:
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    *(u16*)((u8*)r7 + 0x0) = r4;
    r7 = r7 + 0x2;
    if (--ctr != 0) goto .L_800DD7DC;
.L_800DD7F0:
    r4 = *(u32*)lbl_8047AB08;
    /* subi r4, r4, 0x1 */;
    *(u32*)lbl_8047AB08 = r4;
.L_800DD7FC:
    r0 = r0 & 0xFF;
    if (/* eq */) goto .L_800DD4C0;
    r8 = *(u32*)lbl_8047AB08;
    r5 = 0x0;
    r6 = *(u32*)lbl_8047AB00;
    r4 = r5;
    /* cmplwi r8, 0x0 */;
    if (/* le */) goto .L_800DD8B4;
    /* cmplwi r8, 0x8 */;
    /* subi r3, r8, 0x8 */;
    if (/* le */) goto .L_800DD88C;
    r0 = r3 + 0x7;
    r7 = r6;
    r0 = (u32)r0 >> 3;
    /* mtctr r0 */;
    /* cmplwi r3, 0x0 */;
    if (/* le */) goto .L_800DD88C;
.L_800DD840:
    r3 = *(u16*)((u8*)r7 + 0x0);
    r4 = r4 + 0x8;
    r0 = *(u16*)((u8*)r7 + 0x2);
    r5 = r5 + r3;
    r3 = *(u16*)((u8*)r7 + 0x4);
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r7 + 0x6);
    r5 = r5 + r3;
    r3 = *(u16*)((u8*)r7 + 0x8);
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r7 + 0xA);
    r5 = r5 + r3;
    r3 = *(u16*)((u8*)r7 + 0xC);
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r7 + 0xE);
    r5 = r5 + r3;
    r7 = r7 + 0x10;
    r5 = r5 + r0;
    if (--ctr != 0) goto .L_800DD840;
.L_800DD88C:
    r3 = r4 << 1;
    r0 = r8 - r4;
    r3 = r6 + r3;
    /* mtctr r0 */;
    /* cmplw r4, r8 */;
    if (/* ge */) goto .L_800DD8B4;
.L_800DD8A4:
    r0 = *(u16*)((u8*)r3 + 0x0);
    r3 = r3 + 0x2;
    r5 = r5 + r0;
    if (--ctr != 0) goto .L_800DD8A4;
.L_800DD8B4:
    r0 = *(u8*)lbl_8047AB11;
    r3 = *(u32*)lbl_8047AAFC;
    /* cmplwi r0, 0x0 */;
    r30 = r3 + r5;
    if (/* eq */) goto .L_800DD8F8;
    r3 = (u32)lbl_80400F30;
    r3 = (u32)lbl_80400F30;
    strlen();
    r4 = (u32)lbl_80400F30;
    r5 = r3;
    r4 = (u32)lbl_80400F30;
    r3 = r30;
    memcpy();
    r3 = (u32)lbl_80400F30;
    r3 = (u32)lbl_80400F30;
    strlen();
    r30 = r30 + r3;
.L_800DD8F8:
    r3 = (u32)lbl_80400F44;
    r3 = (u32)lbl_80400F44;
    strlen();
    r5 = r3;
    r4 = (u32)lbl_80400F44;
    r4 = (u32)lbl_80400F44;
    r3 = r30;
    r5 = r5 + 0x1;
    memcpy();
    r4 = *(u32*)lbl_8047AB08;
    r5 = *(u32*)lbl_8047AB00;
    r3 = r4 + 0x1;
    r0 = r4 << 1;
    *(u32*)lbl_8047AB08 = r3;
    /* sthx r31, r5, r0 */;
.L_800DD934:
    r0 = *(u8*)lbl_8047AB11;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DD94C;
    r3 = (u32)lbl_80400F30;
    r3 = (u32)lbl_80400F30;
    strlen();
.L_800DD94C:
    r3 = (u32)lbl_80400F44;
    r3 = (u32)lbl_80400F44;
    strlen();
    r0 = *(u32*)((u8*)r1 + 0xB4);
    r31 = *(u32*)((u8*)r1 + 0xAC);
    r30 = *(u32*)((u8*)r1 + 0xA8);
    return;
}
#pragma pop

/* fn_800DD970 -- OSReport/GSlog | Size: 0x5E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DD970_impl(void) {
    r30 = r3;
    if (/* ne */) goto .L_800DD9AC;
    *(f64*)((u8*)r1 + 0x28) = f1;
    *(f64*)((u8*)r1 + 0x30) = f2;
    *(f64*)((u8*)r1 + 0x38) = f3;
    *(f64*)((u8*)r1 + 0x40) = f4;
    *(f64*)((u8*)r1 + 0x48) = f5;
    *(f64*)((u8*)r1 + 0x50) = f6;
    *(f64*)((u8*)r1 + 0x58) = f7;
    *(f64*)((u8*)r1 + 0x60) = f8;
.L_800DD9AC:
    r0 = *(u8*)lbl_8047AB11;
    r12 = r1 + 0xb8;
    r11 = r1 + 0x8;
    r31 = (0x100 << 16);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DDA30;
    OSGetTime();
    r5 = r1 + 0x74;
    fn_800A2998();
    r4 = *(u32*)((u8*)r1 + 0x84);
    r3 = (u32)lbl_80401044;
    r5 = (u32)lbl_802704B4;
    r7 = *(u32*)((u8*)r1 + 0x80);
    r6 = r4 + 0x1;
    r8 = *(u32*)((u8*)r1 + 0x7C);
    r9 = *(u32*)((u8*)r1 + 0x78);
    r3 = (u32)lbl_80401044;
    r10 = *(u32*)((u8*)r1 + 0x74);
    r5 = (u32)lbl_802704B4;
    r4 = 0x14;
    /* crclr cr1eq */;
    fn_800DE09C();
.L_800DDA30:
    r3 = (u32)lbl_80401058;
    r5 = r30;
    r3 = (u32)lbl_80401058;
    r6 = r1 + 0x68;
    r4 = 0xff;
    fn_800DE128();
    r0 = *(u32*)lbl_8047AAFC;
    r3 = (u32)lbl_80401058;
    r3 = (u32)lbl_80401058;
    r4 = 0x0;
    /* cmplwi r0, 0x0 */;
    *(u8*)((u8*)r3 + 0xFF) = r4;
    if (/* eq */) goto .L_800DDF18;
    r0 = *(u8*)lbl_8047AB11;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DDA94;
    strlen();
    r4 = (u32)lbl_80401044;
    r31 = r3;
    r3 = (u32)lbl_80401044;
    strlen();
    r0 = r31 + 0x1;
    r0 = r3 + r0;
    r31 = r0 & 0xFFFF;
    goto .L_800DDAA0;
.L_800DDA94:
    strlen();
    r0 = r3 + 0x1;
    r31 = r0 & 0xFFFF;
.L_800DDAA0:
    r3 = r31 & 0xFFFF;
.L_800DDAA4:
    r9 = *(u32*)lbl_8047AB08;
    r0 = 0x1;
    r4 = *(u32*)lbl_8047AB0C;
    /* cmplw r9, r4 */;
    if (/* lt */) goto .L_800DDABC;
    r0 = 0x0;
.L_800DDABC:
    /* cmplwi r9, 0x0 */;
    r7 = 0x0;
    r10 = *(u32*)lbl_8047AB00;
    r6 = r7;
    if (/* le */) goto .L_800DDB6C;
    /* cmplwi r9, 0x8 */;
    /* subi r5, r9, 0x8 */;
    if (/* le */) goto .L_800DDB40;
    r4 = r5 + 0x7;
    r8 = r10;
    r4 = (u32)r4 >> 3;
    /* mtctr r4 */;
    /* cmplwi r5, 0x0 */;
    if (/* le */) goto .L_800DDB40;
.L_800DDAF4:
    r5 = *(u16*)((u8*)r8 + 0x0);
    r6 = r6 + 0x8;
    r4 = *(u16*)((u8*)r8 + 0x2);
    r7 = r7 + r5;
    r5 = *(u16*)((u8*)r8 + 0x4);
    r7 = r7 + r4;
    r4 = *(u16*)((u8*)r8 + 0x6);
    r7 = r7 + r5;
    r5 = *(u16*)((u8*)r8 + 0x8);
    r7 = r7 + r4;
    r4 = *(u16*)((u8*)r8 + 0xA);
    r7 = r7 + r5;
    r5 = *(u16*)((u8*)r8 + 0xC);
    r7 = r7 + r4;
    r4 = *(u16*)((u8*)r8 + 0xE);
    r7 = r7 + r5;
    r8 = r8 + 0x10;
    r7 = r7 + r4;
    if (--ctr != 0) goto .L_800DDAF4;
.L_800DDB40:
    r5 = r6 << 1;
    r4 = r9 - r6;
    r5 = r10 + r5;
    /* mtctr r4 */;
    /* cmplw r6, r9 */;
    if (/* ge */) goto .L_800DDB6C;
.L_800DDB58:
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    r6 = r6 + 0x1;
    r7 = r7 + r4;
    if (--ctr != 0) goto .L_800DDB58;
.L_800DDB6C:
    r5 = *(u32*)lbl_8047AB04;
    r4 = r3 + r7;
    /* cmplw r4, r5 */;
    if (/* lt */) goto .L_800DDB80;
    r0 = 0x0;
.L_800DDB80:
    r4 = r0 & 0xFF;
    if (/* ne */) goto .L_800DDDE0;
    r4 = *(u16*)((u8*)r10 + 0x0);
    r8 = *(u32*)lbl_8047AAFC;
    r6 = r5 - r4;
    r7 = r8 + r4;
    /* subf. r5, r8, r7 */;
    r4 = r7 - r8;
    r5 = -r4;
    if (/* eq */) goto .L_800DDBAC;
    r5 = r4;
.L_800DDBAC:
    /* cmplwi r5, 0x4 */;
    if (/* lt */) goto .L_800DDCC4;
    /* srwi. r4, r6, 2 */;
    r6 = r6 & 0x3;
    r5 = r4;
    if (/* eq */) goto .L_800DDC3C;
    /* srwi. r4, r4, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DDC24;
.L_800DDBD0:
    r4 = *(u32*)((u8*)r7 + 0x0);
    *(u32*)((u8*)r8 + 0x0) = r4;
    r4 = *(u32*)((u8*)r7 + 0x4);
    *(u32*)((u8*)r8 + 0x4) = r4;
    r4 = *(u32*)((u8*)r7 + 0x8);
    *(u32*)((u8*)r8 + 0x8) = r4;
    r4 = *(u32*)((u8*)r7 + 0xC);
    *(u32*)((u8*)r8 + 0xC) = r4;
    r4 = *(u32*)((u8*)r7 + 0x10);
    *(u32*)((u8*)r8 + 0x10) = r4;
    r4 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r8 + 0x14) = r4;
    r4 = *(u32*)((u8*)r7 + 0x18);
    *(u32*)((u8*)r8 + 0x18) = r4;
    r4 = *(u32*)((u8*)r7 + 0x1C);
    r7 = r7 + 0x20;
    *(u32*)((u8*)r8 + 0x1C) = r4;
    r8 = r8 + 0x20;
    if (--ctr != 0) goto .L_800DDBD0;
    r5 = r5 & 0x7;
    if (/* eq */) goto .L_800DDC3C;
.L_800DDC24:
    /* mtctr r5 */;
.L_800DDC28:
    r4 = *(u32*)((u8*)r7 + 0x0);
    r7 = r7 + 0x4;
    *(u32*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x4;
    if (--ctr != 0) goto .L_800DDC28;
.L_800DDC3C:
    /* cmplwi r6, 0x0 */;
    r5 = r6;
    if (/* eq */) goto .L_800DDD48;
    /* srwi. r4, r6, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DDCA8;
.L_800DDC54:
    r4 = *(u8*)((u8*)r7 + 0x0);
    *(u8*)((u8*)r8 + 0x0) = r4;
    r4 = *(u8*)((u8*)r7 + 0x1);
    *(u8*)((u8*)r8 + 0x1) = r4;
    r4 = *(u8*)((u8*)r7 + 0x2);
    *(u8*)((u8*)r8 + 0x2) = r4;
    r4 = *(u8*)((u8*)r7 + 0x3);
    *(u8*)((u8*)r8 + 0x3) = r4;
    r4 = *(u8*)((u8*)r7 + 0x4);
    *(u8*)((u8*)r8 + 0x4) = r4;
    r4 = *(u8*)((u8*)r7 + 0x5);
    *(u8*)((u8*)r8 + 0x5) = r4;
    r4 = *(u8*)((u8*)r7 + 0x6);
    *(u8*)((u8*)r8 + 0x6) = r4;
    r4 = *(u8*)((u8*)r7 + 0x7);
    r7 = r7 + 0x8;
    *(u8*)((u8*)r8 + 0x7) = r4;
    r8 = r8 + 0x8;
    if (--ctr != 0) goto .L_800DDC54;
    r5 = r5 & 0x7;
    if (/* eq */) goto .L_800DDD48;
.L_800DDCA8:
    /* mtctr r5 */;
.L_800DDCAC:
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto .L_800DDCAC;
    goto .L_800DDD48;
.L_800DDCC4:
    /* cmplwi r6, 0x0 */;
    r5 = r6;
    if (/* eq */) goto .L_800DDD48;
    /* srwi. r4, r6, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DDD30;
.L_800DDCDC:
    r4 = *(u8*)((u8*)r7 + 0x0);
    *(u8*)((u8*)r8 + 0x0) = r4;
    r4 = *(u8*)((u8*)r7 + 0x1);
    *(u8*)((u8*)r8 + 0x1) = r4;
    r4 = *(u8*)((u8*)r7 + 0x2);
    *(u8*)((u8*)r8 + 0x2) = r4;
    r4 = *(u8*)((u8*)r7 + 0x3);
    *(u8*)((u8*)r8 + 0x3) = r4;
    r4 = *(u8*)((u8*)r7 + 0x4);
    *(u8*)((u8*)r8 + 0x4) = r4;
    r4 = *(u8*)((u8*)r7 + 0x5);
    *(u8*)((u8*)r8 + 0x5) = r4;
    r4 = *(u8*)((u8*)r7 + 0x6);
    *(u8*)((u8*)r8 + 0x6) = r4;
    r4 = *(u8*)((u8*)r7 + 0x7);
    r7 = r7 + 0x8;
    *(u8*)((u8*)r8 + 0x7) = r4;
    r8 = r8 + 0x8;
    if (--ctr != 0) goto .L_800DDCDC;
    r5 = r5 & 0x7;
    if (/* eq */) goto .L_800DDD48;
.L_800DDD30:
    /* mtctr r5 */;
.L_800DDD34:
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto .L_800DDD34;
.L_800DDD48:
    r4 = *(u32*)lbl_8047AB08;
    r7 = *(u32*)lbl_8047AB00;
    /* subic. r6, r4, 0x1 */;
    r5 = r7 + 0x2;
    if (/* eq */) goto .L_800DDDD4;
    /* srwi. r4, r6, 3 */;
    /* mtctr r4 */;
    if (/* eq */) goto .L_800DDDBC;
.L_800DDD68:
    r4 = *(u16*)((u8*)r5 + 0x0);
    *(u16*)((u8*)r7 + 0x0) = r4;
    r4 = *(u16*)((u8*)r5 + 0x2);
    *(u16*)((u8*)r7 + 0x2) = r4;
    r4 = *(u16*)((u8*)r5 + 0x4);
    *(u16*)((u8*)r7 + 0x4) = r4;
    r4 = *(u16*)((u8*)r5 + 0x6);
    *(u16*)((u8*)r7 + 0x6) = r4;
    r4 = *(u16*)((u8*)r5 + 0x8);
    *(u16*)((u8*)r7 + 0x8) = r4;
    r4 = *(u16*)((u8*)r5 + 0xA);
    *(u16*)((u8*)r7 + 0xA) = r4;
    r4 = *(u16*)((u8*)r5 + 0xC);
    *(u16*)((u8*)r7 + 0xC) = r4;
    r4 = *(u16*)((u8*)r5 + 0xE);
    r5 = r5 + 0x10;
    *(u16*)((u8*)r7 + 0xE) = r4;
    r7 = r7 + 0x10;
    if (--ctr != 0) goto .L_800DDD68;
    r6 = r6 & 0x7;
    if (/* eq */) goto .L_800DDDD4;
.L_800DDDBC:
    /* mtctr r6 */;
.L_800DDDC0:
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    *(u16*)((u8*)r7 + 0x0) = r4;
    r7 = r7 + 0x2;
    if (--ctr != 0) goto .L_800DDDC0;
.L_800DDDD4:
    r4 = *(u32*)lbl_8047AB08;
    /* subi r4, r4, 0x1 */;
    *(u32*)lbl_8047AB08 = r4;
.L_800DDDE0:
    r0 = r0 & 0xFF;
    if (/* eq */) goto .L_800DDAA4;
    r8 = *(u32*)lbl_8047AB08;
    r5 = 0x0;
    r6 = *(u32*)lbl_8047AB00;
    r4 = r5;
    /* cmplwi r8, 0x0 */;
    if (/* le */) goto .L_800DDE98;
    /* cmplwi r8, 0x8 */;
    /* subi r3, r8, 0x8 */;
    if (/* le */) goto .L_800DDE70;
    r0 = r3 + 0x7;
    r7 = r6;
    r0 = (u32)r0 >> 3;
    /* mtctr r0 */;
    /* cmplwi r3, 0x0 */;
    if (/* le */) goto .L_800DDE70;
.L_800DDE24:
    r3 = *(u16*)((u8*)r7 + 0x0);
    r4 = r4 + 0x8;
    r0 = *(u16*)((u8*)r7 + 0x2);
    r5 = r5 + r3;
    r3 = *(u16*)((u8*)r7 + 0x4);
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r7 + 0x6);
    r5 = r5 + r3;
    r3 = *(u16*)((u8*)r7 + 0x8);
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r7 + 0xA);
    r5 = r5 + r3;
    r3 = *(u16*)((u8*)r7 + 0xC);
    r5 = r5 + r0;
    r0 = *(u16*)((u8*)r7 + 0xE);
    r5 = r5 + r3;
    r7 = r7 + 0x10;
    r5 = r5 + r0;
    if (--ctr != 0) goto .L_800DDE24;
.L_800DDE70:
    r3 = r4 << 1;
    r0 = r8 - r4;
    r3 = r6 + r3;
    /* mtctr r0 */;
    /* cmplw r4, r8 */;
    if (/* ge */) goto .L_800DDE98;
.L_800DDE88:
    r0 = *(u16*)((u8*)r3 + 0x0);
    r3 = r3 + 0x2;
    r5 = r5 + r0;
    if (--ctr != 0) goto .L_800DDE88;
.L_800DDE98:
    r0 = *(u8*)lbl_8047AB11;
    r3 = *(u32*)lbl_8047AAFC;
    /* cmplwi r0, 0x0 */;
    r30 = r3 + r5;
    if (/* eq */) goto .L_800DDEDC;
    r3 = (u32)lbl_80401044;
    r3 = (u32)lbl_80401044;
    strlen();
    r4 = (u32)lbl_80401044;
    r5 = r3;
    r4 = (u32)lbl_80401044;
    r3 = r30;
    memcpy();
    r3 = (u32)lbl_80401044;
    r3 = (u32)lbl_80401044;
    strlen();
    r30 = r30 + r3;
.L_800DDEDC:
    r3 = (u32)lbl_80401058;
    r3 = (u32)lbl_80401058;
    strlen();
    r5 = r3;
    r4 = (u32)lbl_80401058;
    r4 = (u32)lbl_80401058;
    r3 = r30;
    r5 = r5 + 0x1;
    memcpy();
    r4 = *(u32*)lbl_8047AB08;
    r5 = *(u32*)lbl_8047AB00;
    r3 = r4 + 0x1;
    r0 = r4 << 1;
    *(u32*)lbl_8047AB08 = r3;
    /* sthx r31, r5, r0 */;
.L_800DDF18:
    r0 = *(u8*)lbl_8047AB11;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800DDF30;
    r3 = (u32)lbl_80401044;
    r3 = (u32)lbl_80401044;
    strlen();
.L_800DDF30:
    r3 = (u32)lbl_80401058;
    r3 = (u32)lbl_80401058;
    strlen();
    r0 = *(u32*)((u8*)r1 + 0xB4);
    r31 = *(u32*)((u8*)r1 + 0xAC);
    r30 = *(u32*)((u8*)r1 + 0xA8);
    return;
}
#pragma pop

/* fn_800DDF54 | Size: 0x148 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DDF54(void) {
    r5 = (u32)lbl_802704A0;
    r0 = 0x0;
    r31 = (u32)lbl_802704A0;
    /* mr. r30, r3 */;
    *(u8*)lbl_8047AB10 = r0;
    *(u32*)lbl_8047AB04 = r30;
    *(u8*)lbl_8047AB11 = r4;
    if (/* ne */) goto .L_800DDF9C;
    r3 = r31 + 0x2c;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x1;
    goto .L_800DE084;
.L_800DDF9C:
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAF8 = r3;
    if (/* ne */) goto .L_800DDFC0;
    r3 = r31 + 0x48;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto .L_800DE084;
.L_800DDFC0:
    r0 = (u32)r30 >> 7;
    /* rlwinm r3, r30, 26, 6, 30 */;
    *(u32*)lbl_8047AB0C = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAFA = r3;
    if (/* ne */) goto .L_800DDFF8;
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E209C();
    r3 = r31 + 0x48;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto .L_800DE084;
.L_800DDFF8:
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E27B0();
    /* cmplwi r3, 0x0 */;
    *(u32*)lbl_8047AAFC = r3;
    if (/* ne */) goto .L_800DE030;
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E209C();
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E209C();
    r3 = r31 + 0x48;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto .L_800DE084;
.L_800DE030:
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E27B0();
    /* cmplwi r3, 0x0 */;
    *(u32*)lbl_8047AB00 = r3;
    if (/* ne */) goto .L_800DE070;
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E24B0();
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E209C();
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E209C();
    r3 = r31 + 0x48;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto .L_800DE084;
.L_800DE070:
    r4 = *(u32*)lbl_8047AB04;
    r3 = r31 + 0x5c;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x1;
.L_800DE084:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800DE09C | Size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DE09C(void) {
    if (/* ne */) goto .L_800DE0D0;
    *(f64*)((u8*)r1 + 0x28) = f1;
    *(f64*)((u8*)r1 + 0x30) = f2;
    *(f64*)((u8*)r1 + 0x38) = f3;
    *(f64*)((u8*)r1 + 0x40) = f4;
    *(f64*)((u8*)r1 + 0x48) = f5;
    *(f64*)((u8*)r1 + 0x50) = f6;
    *(f64*)((u8*)r1 + 0x58) = f7;
    *(f64*)((u8*)r1 + 0x60) = f8;
.L_800DE0D0:
    r11 = r1 + 0x88;
    r0 = r1 + 0x8;
    r12 = (0x300 << 16);
    r31 = r1 + 0x68;
    r6 = r31;
    fn_800DE128();
    r0 = *(u32*)((u8*)r1 + 0x84);
    r31 = *(u32*)((u8*)r1 + 0x7C);
    return;
}
#pragma pop

/* fn_800DE128 | Size: 0x558 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DE128(void) {
    r0 = 0x0;
    /* stmw r20, 0x10(r1) */;
    r22 = r3;
    r3 = (u32)lbl_80401168;
    r23 = r4;
    r24 = r6;
    r31 = r5;
    r30 = r22;
    r21 = (u32)lbl_80401168;
    r28 = 0x0;
    r27 = 0x0;
    r26 = 0x0;
    r25 = 0x0;
    goto .L_800DE65C;
.L_800DE16C:
    r3 = r27 & 0xFF;
    if (/* ne */) goto .L_800DE1C0;
    r4 = *(u8*)((u8*)r31 + 0x0);
    /* cmpwi r4, 0x25 */;
    if (/* ne */) goto .L_800DE1B4;
    r3 = *(u8*)((u8*)r31 + 0x1);
    /* cmpwi r3, 0x25 */;
    if (/* ne */) goto .L_800DE1A0;
    r3 = 0x25;
    r31 = r31 + 0x1;
    *(u8*)((u8*)r30 + 0x0) = r3;
    r30 = r30 + 0x1;
    goto .L_800DE63C;
.L_800DE1A0:
    r3 = (u32)lbl_80401178;
    r27 = 0x1;
    r3 = (u32)lbl_80401178;
    r29 = r3;
    goto .L_800DE63C;
.L_800DE1B4:
    *(u8*)((u8*)r30 + 0x0) = r4;
    r30 = r30 + 0x1;
    goto .L_800DE63C;
.L_800DE1C0:
    r5 = *(u8*)((u8*)r31 + 0x0);
    r3 = (s8)r5;
    /* subi r4, r3, 0x58 */;
    /* cmplwi r4, 0x20 */;
    if (/* gt */) goto .L_800DE4D8;
    r3 = (u32)jumptable_80315388;
    r4 = r4 << 2;
    r3 = (u32)jumptable_80315388;
    /* lwzx r3, r3, r4 */;
    /* mtctr r3 */;
    /* indirect jump via ctr */;
    r4 = (u32)lbl_80401168;
    r3 = r24;
    r0 = (u32)lbl_80401168;
    r4 = 0x1;
    r20 = r0;
    __va_arg();
    r0 = *(u32*)((u8*)r3 + 0x0);
    r3 = (u32)lbl_80401168;
    r4 = (u32)lbl_80401168;
    r3 = 0x0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = 0x1;
    *(u8*)((u8*)r21 + 0x1) = r3;
    goto .L_800DE4E0;
    r3 = r24;
    r4 = 0x1;
    __va_arg();
    r0 = *(u32*)((u8*)r3 + 0x0);
    r3 = (u32)lbl_80401168;
    r3 = (u32)lbl_80401168;
    r8 = 0x0;
    /* cmpwi r0, 0x0 */;
    r9 = r0;
    if (/* ge */) goto .L_800DE254;
    r8 = 0x1;
    r9 = -r0;
.L_800DE254:
    r4 = (0x6666 << 16);
    r7 = *(u32*)lbl_80478AE8;
    r6 = r4 + 0x6667;
.L_800DE260:
    r0 = (s32)((s64)r6 * (s64)r9 >> 32);
    r4 = (s32)r0 >> 2;
    r5 = (u32)r4 >> 31;
    r0 = (s32)r0 >> 2;
    r4 = r4 + r5;
    r5 = r4 * 0xa;
    r4 = (u32)r0 >> 31;
    r5 = r9 - r5;
    /* add. r9, r0, r4 */;
    /* lbzx r0, r7, r5 */;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
    if (/* ne */) goto .L_800DE260;
    r0 = r8 & 0xFF;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DE2AC;
    r0 = 0x2d;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
.L_800DE2AC:
    r0 = 0x0;
    r4 = (u32)lbl_80401168;
    r20 = (u32)lbl_80401168;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r20;
    strlen();
    r4 = (u32)lbl_80401168;
    r0 = (u32)lbl_80401168;
    r4 = r3 + r0;
    /* subi r4, r4, 0x1 */;
    goto .L_800DE310;
.L_800DE2D8:
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r20 = r20 + 0x1;
.L_800DE310:
    /* cmplw r20, r4 */;
    if (/* lt */) goto .L_800DE2D8;
    r3 = (u32)lbl_80401168;
    r0 = 0x1;
    r3 = (u32)lbl_80401168;
    r20 = r3;
    goto .L_800DE4E0;
    r3 = r24;
    r4 = 0x3;
    __va_arg();
    r20 = lbl_8047CAA0@sda21;
    r0 = 0x1;
    goto .L_800DE4E0;
    r3 = r24;
    r4 = 0x1;
    __va_arg();
    r20 = *(u32*)((u8*)r3 + 0x0);
    /* cmplwi r20, 0x0 */;
    if (/* ne */) goto .L_800DE360;
    r20 = lbl_8047CAA8@sda21;
.L_800DE360:
    r0 = 0x1;
    goto .L_800DE4E0;
    r3 = r24;
    r4 = 0x1;
    __va_arg();
    r0 = *(u8*)((u8*)r31 + 0x0);
    r6 = *(u32*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x58 */;
    if (/* ne */) goto .L_800DE41C;
    r4 = (u32)lbl_80401168;
    r3 = *(u32*)lbl_80478AE8;
    r4 = (u32)lbl_80401168;
    r5 = r6;
.L_800DE394:
    r0 = r5 & 0xF;
    /* srwi. r5, r5, 4 */;
    /* lbzx r0, r3, r0 */;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r4 = r4 + 0x1;
    if (/* ne */) goto .L_800DE394;
    r0 = 0x0;
    r3 = (u32)lbl_80401168;
    r20 = (u32)lbl_80401168;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = r20;
    strlen();
    r4 = (u32)lbl_80401168;
    r0 = (u32)lbl_80401168;
    r4 = r3 + r0;
    /* subi r4, r4, 0x1 */;
    goto .L_800DE410;
.L_800DE3D8:
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r20 = r20 + 0x1;
.L_800DE410:
    /* cmplw r20, r4 */;
    if (/* lt */) goto .L_800DE3D8;
    goto .L_800DE4C4;
.L_800DE41C:
    r3 = (u32)lbl_80401168;
    r4 = *(u32*)lbl_80478AE8;
    r5 = (u32)lbl_80401168;
.L_800DE428:
    r0 = r6 & 0xF;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = *(u8*)((u8*)r5 + 0x0);
    r0 = (s8)r3;
    /* cmpwi r0, 0x41 */;
    if (/* lt */) goto .L_800DE44C;
    r0 = r3 + 0x20;
    *(u8*)((u8*)r5 + 0x0) = r0;
.L_800DE44C:
    /* srwi. r6, r6, 4 */;
    r5 = r5 + 0x1;
    if (/* ne */) goto .L_800DE428;
    r0 = 0x0;
    r3 = (u32)lbl_80401168;
    r20 = (u32)lbl_80401168;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = r20;
    strlen();
    r4 = (u32)lbl_80401168;
    r0 = (u32)lbl_80401168;
    r4 = r3 + r0;
    /* subi r4, r4, 0x1 */;
    goto .L_800DE4BC;
.L_800DE484:
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subi r4, r4, 0x1 */;
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r20 = r20 + 0x1;
.L_800DE4BC:
    /* cmplw r20, r4 */;
    if (/* lt */) goto .L_800DE484;
.L_800DE4C4:
    r3 = (u32)lbl_80401168;
    r0 = 0x1;
    r3 = (u32)lbl_80401168;
    r20 = r3;
    goto .L_800DE4E0;
.L_800DE4D8:
    *(u8*)((u8*)r29 + 0x0) = r5;
    r29 = r29 + 0x1;
.L_800DE4E0:
    r3 = r0 & 0xFF;
    /* cmplwi r3, 0x1 */;
    if (/* ne */) goto .L_800DE63C;
    r0 = 0x0;
    r3 = (u32)lbl_80401178;
    *(u8*)((u8*)r29 + 0x0) = r0;
    r3 = (u32)lbl_80401178;
    r29 = r3;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* cmpwi r0, 0x2d */;
    if (/* ne */) goto .L_800DE514;
    r26 = 0x1;
    r29 = r3 + 0x1;
.L_800DE514:
    r0 = *(u8*)((u8*)r29 + 0x0);
    /* cmpwi r0, 0x30 */;
    if (/* ne */) goto .L_800DE528;
    r25 = 0x1;
    r29 = r29 + 0x1;
.L_800DE528:
    r3 = r29;
    r27 = 0x0;
    goto .L_800DE560;
.L_800DE534:
    r0 = (s8)r4;
    /* cmpwi r0, 0x30 */;
    if (/* lt */) goto .L_800DE56C;
    /* cmpwi r0, 0x39 */;
    if (/* gt */) goto .L_800DE56C;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r27 = r27 * 0xa;
    r3 = r3 + 0x1;
    r0 = (s8)r0;
    r27 = r0 + r27;
    /* subi r27, r27, 0x30 */;
.L_800DE560:
    r4 = *(u8*)((u8*)r3 + 0x0);
    r0 = (s8)r4;
    if (/* ne */) goto .L_800DE534;
.L_800DE56C:
    r3 = r20;
    strlen();
    /* cmpw r27, r3 */;
    if (/* le */) goto .L_800DE5E0;
    r0 = r26 & 0xFF;
    r27 = r27 - r3;
    if (/* ne */) goto .L_800DE5E0;
    r5 = r25 & 0xFF;
    r4 = 0x30;
    r3 = 0x20;
    goto .L_800DE5B4;
.L_800DE598:
    /* cmplwi r5, 0x0 */;
    if (/* eq */) goto .L_800DE5AC;
    *(u8*)((u8*)r30 + 0x0) = r4;
    r30 = r30 + 0x1;
    goto .L_800DE5B4;
.L_800DE5AC:
    *(u8*)((u8*)r30 + 0x0) = r3;
    r30 = r30 + 0x1;
.L_800DE5B4:
    /* cmpwi r27, 0x0 */;
    /* subi r27, r27, 0x1 */;
    if (/* eq */) goto .L_800DE5E0;
    r0 = r30 - r22;
    /* cmplw r0, r23 */;
    if (/* lt */) goto .L_800DE598;
    goto .L_800DE5E0;
.L_800DE5D0:
    r0 = *(u8*)((u8*)r20 + 0x0);
    r20 = r20 + 0x1;
    *(u8*)((u8*)r30 + 0x0) = r0;
    r30 = r30 + 0x1;
.L_800DE5E0:
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = (s8)r0;
    if (/* eq */) goto .L_800DE5F8;
    r0 = r30 - r22;
    /* cmplw r0, r23 */;
    if (/* lt */) goto .L_800DE5D0;
.L_800DE5F8:
    r0 = r26 & 0xFF;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800DE62C;
    r3 = 0x20;
    goto .L_800DE614;
.L_800DE60C:
    *(u8*)((u8*)r30 + 0x0) = r3;
    r30 = r30 + 0x1;
.L_800DE614:
    /* cmpwi r27, 0x0 */;
    /* subi r27, r27, 0x1 */;
    if (/* eq */) goto .L_800DE62C;
    r0 = r30 - r22;
    /* cmplw r0, r23 */;
    if (/* lt */) goto .L_800DE60C;
.L_800DE62C:
    r26 = 0x0;
    r25 = 0x0;
    r27 = 0x0;
    r0 = 0x0;
.L_800DE63C:
    r3 = *(u8*)((u8*)r31 + 0x0);
    r3 = (s8)r3;
    if (/* eq */) goto .L_800DE654;
    r3 = r30 - r22;
    /* cmplw r3, r23 */;
    if (/* lt */) goto .L_800DE658;
.L_800DE654:
    r28 = 0x1;
.L_800DE658:
    r31 = r31 + 0x1;
.L_800DE65C:
    r3 = r28 & 0xFF;
    if (/* eq */) goto .L_800DE16C;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x0) = r0;
    /* lmw r20, 0x10(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x44);
    return;
}
#pragma pop

/* fn_800DEFC8 | Size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DEFC8(void) {
    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x38);
    r0 = r3 + (0x102 << 16);
    /* cmplwi r0, 0xfefe */;
    if (/* eq */) goto .L_800DF014;
    fn_800B8DF4();
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0x38);
    r4 = *(u32*)((u8*)r3 + 0x8);
    r3 = *(u32*)((u8*)r4 + 0x58);
    *(u32*)((u8*)r4 + 0x58) = r0;
    fn_801BBD3C();
    r3 = (0xfeff << 16);
    /* subi r0, r3, 0x102 */;
    *(u32*)((u8*)r31 + 0x38) = r0;
.L_800DF014:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DF028 | Size: 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF028(void) {
    r30 = r4;
    r29 = r3;
    r3 = *(u32*)((u8*)r3 + 0x38);
    r0 = r3 + (0x102 << 16);
    /* cmplwi r0, 0xfefe */;
    if (/* ne */) goto .L_800DF074;
    r3 = *(u32*)((u8*)r29 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x8);
    r0 = *(u32*)((u8*)r3 + 0x58);
    *(u32*)((u8*)r29 + 0x38) = r0;
    fn_801BBD84();
    r31 = r3;
    goto .L_800DF080;
.L_800DF074:
    r3 = *(u32*)((u8*)r29 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x8);
    r31 = *(u32*)((u8*)r3 + 0x58);
.L_800DF080:
    /* cmplwi r31, 0x0 */;
    if (/* eq */) goto .L_800DF100;
    r3 = r30;
    r4 = 0x0;
    fn_800EF548();
    *(u32*)((u8*)r31 + 0x0) = r3;
    r3 = r30;
    fn_800EF4FC();
    *(u16*)((u8*)r31 + 0x4) = r3;
    r3 = r30;
    fn_800EF4F4();
    *(u16*)((u8*)r31 + 0x6) = r3;
    r3 = r30;
    r4 = 0x1;
    fn_800EF3E0();
    *(u32*)((u8*)r31 + 0x8) = r3;
    r3 = r30;
    fn_800EF4E4();
    r4 = r3 & 0xFF;
    f0 = *(f32*)lbl_8047CAC8;
    r0 = -r4;
    r3 = r30;
    r0 = r0 | r4;
    r0 = (u32)r0 >> 31;
    *(u32*)((u8*)r31 + 0xC) = r0;
    *(f32*)((u8*)r31 + 0x10) = f0;
    *(f32*)((u8*)r31 + 0x14) = f0;
    fn_800EF504();
    fn_800B8DF4();
    r3 = *(u32*)((u8*)r29 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x8);
    *(u32*)((u8*)r3 + 0x58) = r31;
.L_800DF100:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DF11C | Size: 0x24 */
void fn_800DF11C(void) {
    *(u8*)((u8*)r4 + 0x0) = r0;
    *(u8*)((u8*)r4 + 0x1) = r0;
    *(u8*)((u8*)r4 + 0x2) = r0;
    *(u8*)((u8*)r4 + 0x3) = r0;
}

/* fn_800DF140 | Size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF140(void) {
    /* lfd f2, lbl_8047CAD0@sda21(r0) */;
    /* lis r0, 0x4330 */;
    /* lfs f0, lbl_8047CACC@sda21(r0) */;
    /* lbz r4, 0x1(r3) */;
    /* lwz r3, 0x8(r3) */;
    /* lfd f1, 0x8(r1) */;
    /* fsubs f1, f1, f2 */;
    /* fdivs f1, f1, f0 */;
    fn_801A6DDC();
}
#pragma pop

/* fn_800DF188 | Size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF188(void) {
    f1 = *(f32*)lbl_8047CACC;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r4 = *(u32*)((u8*)r4 + 0xC);
    f0 = *(f32*)((u8*)r4 + 0xC);
    f0 = f1 * f0;
    f0 = (f64)(s32)f0;
    *(f64*)((u8*)r1 + 0x8) = f0;
    r0 = *(u32*)((u8*)r1 + 0xC);
    *(u8*)((u8*)r3 + 0x1) = r0;
    r1 = r1 + 0x10;
    return;
}
#pragma pop

/* fn_800DF1B8 | Size: 0x18 */
void fn_800DF1B8(void) {
    *(f32*)((u8*)r3 + 0x34) = f1;
    *(f32*)((u8*)r4 + 0x50) = f1;
}

/* fn_800DF1D0 | Size: 0x14 */
void fn_800DF1D0(void) {
    *(u32*)((u8*)r3 + 0x2C) = r4;
    *(u32*)((u8*)r3 + 0x30) = r5;
    *(f32*)((u8*)r3 + 0x34) = f1;
    *(u32*)((u8*)r3 + 0x28) = r6;
}

/* fn_800DF1E4 | Size: 0x24 */
void fn_800DF1E4(void) {
    *(u8*)((u8*)r3 + 0xC) = r0;
    *(u8*)((u8*)r3 + 0xD) = r0;
    *(u8*)((u8*)r3 + 0xE) = r0;
    *(u8*)((u8*)r3 + 0xF) = r0;
}

/* fn_800DF208 | Size: 0x14 */
void fn_800DF208(void) {
    *(u32*)((u8*)r3 + 0x10) = r4;
    *(u32*)((u8*)r3 + 0x14) = r5;
    *(u32*)((u8*)r3 + 0x18) = r6;
    *(u32*)((u8*)r3 + 0x1C) = r7;
}

/* fn_800DF21C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF21C(void) {
    /* lwz r3, 0x8(r3) */;
    fn_801A6DDC();
}
#pragma pop

/* fn_800DF248 | Size: 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF248(void) {
    r29 = r3;
    r0 = *(u16*)((u8*)r3 + 0x2);
    r30 = *(u32*)((u8*)r3 + 0x8);
    r31 = r4 & r0;
    r0 = r31 & 0x1;
    if (/* eq */) goto .L_800DF290;
    r0 = 0x7f;
    *(u8*)((u8*)r29 + 0xF) = r0;
    *(u8*)((u8*)r29 + 0xE) = r0;
    *(u8*)((u8*)r29 + 0xD) = r0;
    *(u8*)((u8*)r29 + 0xC) = r0;
.L_800DF290:
    /* rlwinm r0, r31, 0, 30, 30 */;
    if (/* eq */) goto .L_800DF2B8;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r29 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r29 + 0x14) = r4;
    *(u32*)((u8*)r29 + 0x18) = r3;
    *(u32*)((u8*)r29 + 0x1C) = r0;
.L_800DF2B8:
    /* rlwinm r0, r31, 0, 29, 29 */;
    if (/* eq */) goto .L_800DF33C;
    r5 = *(u32*)((u8*)r29 + 0x8);
    r28 = *(u32*)((u8*)r29 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r29 + 0x24);
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800DF334;
    /* cmplwi r28, 0x0 */;
    if (/* eq */) goto .L_800DF334;
    /* cmplw r4, r28 */;
    if (/* ne */) goto .L_800DF310;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto .L_800DF318;
    goto .L_800DF310;
.L_800DF2F8:
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* cmplw r0, r28 */;
    if (/* ne */) goto .L_800DF30C;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
.L_800DF30C:
    r4 = *(u32*)((u8*)r4 + 0x8);
.L_800DF310:
    /* cmplwi r4, 0x0 */;
    if (/* ne */) goto .L_800DF2F8;
.L_800DF318:
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800DF324;
    fn_801BBD3C();
.L_800DF324:
    /* cmplwi r28, 0x0 */;
    if (/* eq */) goto .L_800DF334;
    r3 = r28;
    fn_801BBED4();
.L_800DF334:
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x28) = r0;
.L_800DF33C:
    r0 = *(u16*)((u8*)r29 + 0x2);
    r0 = r0 & ~r31;
    *(u16*)((u8*)r29 + 0x2) = r0;
    r0 = *(u16*)((u8*)r29 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DF35C;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x20) = r0;
.L_800DF35C:
    r3 = r30;
    fn_801A6FF0();
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r28 = *(u32*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800DF384 | Size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF384(void) {
    r29 = r3;
    r0 = *(u16*)((u8*)r3 + 0x2);
    r30 = *(u32*)((u8*)r3 + 0x8);
    /* andc. r31, r4, r0 */;
    if (/* eq */) goto .L_800DF3D4;
    /* rlwinm r0, r31, 0, 29, 29 */;
    *(u32*)((u8*)r30 + 0x20) = r29;
    if (/* eq */) goto .L_800DF3C0;
    fn_800DFABC();
.L_800DF3C0:
    r0 = *(u16*)((u8*)r29 + 0x2);
    r3 = r30;
    r0 = r0 | r31;
    *(u16*)((u8*)r29 + 0x2) = r0;
    fn_801A6FF0();
.L_800DF3D4:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DF3F0 | Size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF3F0(void) {
    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_801A8458();
    r0 = r3 & 0x1;
    r4 = 0x0;
    if (/* eq */) goto .L_800DF414;
    r4 = r4 | 0x1;
.L_800DF414:
    /* rlwinm r0, r3, 0, 30, 30 */;
    if (/* eq */) goto .L_800DF420;
    r4 = r4 | 0x2;
.L_800DF420:
    /* rlwinm r0, r3, 0, 29, 29 */;
    if (/* eq */) goto .L_800DF42C;
    r4 = r4 | 0x4;
.L_800DF42C:
    /* rlwinm r0, r3, 0, 28, 28 */;
    if (/* eq */) goto .L_800DF438;
    r4 = r4 | 0x8;
.L_800DF438:
    /* rlwinm r0, r3, 0, 1, 1 */;
    if (/* eq */) goto .L_800DF444;
    r4 = r4 | 0x10;
.L_800DF444:
    /* rlwinm r0, r3, 0, 18, 18 */;
    if (/* eq */) goto .L_800DF450;
    r4 = r4 | 0x20;
.L_800DF450:
    /* rlwinm r0, r3, 0, 17, 17 */;
    if (/* eq */) goto .L_800DF45C;
    r4 = r4 | 0x40;
.L_800DF45C:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r3 = r4;
    return;
}
#pragma pop

/* fn_800DF470 | Size: 0x28 */
void fn_800DF470(void) {
    *(u32*)((u8*)r5 + 0x10) = r6;
    *(u32*)((u8*)r3 + 0x3C) = r0;
}

/* fn_800DF498 | Size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF498(void) {
    r31 = r4;
    r30 = r3;
    r3 = *(u32*)((u8*)r3 + 0x3C);
    r0 = r3 + (0x102 << 16);
    /* cmplwi r0, 0xfefe */;
    if (/* ne */) goto .L_800DF4D4;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)((u8*)r3 + 0x10);
    *(u32*)((u8*)r30 + 0x3C) = r0;
    goto .L_800DF4E4;
.L_800DF4D4:
    r3 = (u32)lbl_80270528;
    r3 = (u32)lbl_80270528;
    /* crclr cr1eq */;
    fn_800DD970();
.L_800DF4E4:
    r3 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r3 + 0x10) = r31;
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800DF504 | Size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF504(void) {
    r4 = (0x4000 << 16);
    r4 = r4 + 0x600f;
    r31 = r3;
    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_801A8428();
    r3 = *(u32*)((u8*)r31 + 0x8);
    r4 = *(u32*)((u8*)r31 + 0x4);
    fn_801A8440();
    r3 = *(u32*)((u8*)r31 + 0x8);
    fn_801A6FF0();
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DF550 | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF550(void) {
    r31 = r4;
    r30 = r3;
    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_801A8458();
    *(u32*)((u8*)r30 + 0x4) = r3;
    r3 = (0x4000 << 16);
    r4 = r3 + 0x600f;
    r3 = *(u32*)((u8*)r30 + 0x8);
    fn_801A8428();
    r0 = r31 & 0x1;
    r4 = 0x0;
    if (/* eq */) goto .L_800DF598;
    r4 = r4 | 0x1;
.L_800DF598:
    /* rlwinm r0, r31, 0, 30, 30 */;
    if (/* eq */) goto .L_800DF5A4;
    r4 = r4 | 0x2;
.L_800DF5A4:
    /* rlwinm r0, r31, 0, 29, 29 */;
    if (/* eq */) goto .L_800DF5B0;
    r4 = r4 | 0x4;
.L_800DF5B0:
    /* rlwinm r0, r31, 0, 28, 28 */;
    if (/* eq */) goto .L_800DF5BC;
    r4 = r4 | 0x8;
.L_800DF5BC:
    /* rlwinm r0, r31, 0, 27, 27 */;
    if (/* eq */) goto .L_800DF5C8;
    r4 = r4 | (0x4000 << 16);
.L_800DF5C8:
    /* rlwinm r0, r31, 0, 26, 26 */;
    if (/* eq */) goto .L_800DF5D4;
    r4 = r4 | 0x2000;
.L_800DF5D4:
    /* rlwinm r0, r31, 0, 25, 25 */;
    if (/* eq */) goto .L_800DF5E0;
    r4 = r4 | 0x4000;
.L_800DF5E0:
    r3 = *(u32*)((u8*)r30 + 0x8);
    fn_801A8440();
    r3 = *(u32*)((u8*)r30 + 0x8);
    fn_801A6FF0();
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800DF608 | Size: 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF608(void) {
    r30 = r3;
    r5 = *(u32*)((u8*)r3 + 0x3C);
    r0 = r5 + (0x102 << 16);
    /* cmplwi r0, 0xfefe */;
    if (/* eq */) goto .L_800DF64C;
    r4 = *(u32*)((u8*)r30 + 0x8);
    r3 = (0xfeff << 16);
    /* subi r0, r3, 0x102 */;
    *(u32*)((u8*)r4 + 0x10) = r5;
    *(u32*)((u8*)r30 + 0x3C) = r0;
.L_800DF64C:
    r3 = *(u32*)((u8*)r30 + 0x38);
    r0 = r3 + (0x102 << 16);
    /* cmplwi r0, 0xfefe */;
    if (/* eq */) goto .L_800DF684;
    fn_800B8DF4();
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)((u8*)r30 + 0x38);
    r4 = *(u32*)((u8*)r3 + 0x8);
    r3 = *(u32*)((u8*)r4 + 0x58);
    *(u32*)((u8*)r4 + 0x58) = r0;
    fn_801BBD3C();
    r3 = (0xfeff << 16);
    /* subi r0, r3, 0x102 */;
    *(u32*)((u8*)r30 + 0x38) = r0;
.L_800DF684:
    r29 = *(u16*)((u8*)r30 + 0x2);
    r31 = *(u32*)((u8*)r30 + 0x8);
    r0 = r29 & 0x1;
    if (/* eq */) goto .L_800DF6A8;
    r0 = 0x7f;
    *(u8*)((u8*)r30 + 0xF) = r0;
    *(u8*)((u8*)r30 + 0xE) = r0;
    *(u8*)((u8*)r30 + 0xD) = r0;
    *(u8*)((u8*)r30 + 0xC) = r0;
.L_800DF6A8:
    /* rlwinm r0, r29, 0, 30, 30 */;
    if (/* eq */) goto .L_800DF6D0;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r30 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r30 + 0x14) = r4;
    *(u32*)((u8*)r30 + 0x18) = r3;
    *(u32*)((u8*)r30 + 0x1C) = r0;
.L_800DF6D0:
    /* rlwinm r0, r29, 0, 29, 29 */;
    if (/* eq */) goto .L_800DF754;
    r5 = *(u32*)((u8*)r30 + 0x8);
    r28 = *(u32*)((u8*)r30 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r30 + 0x24);
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800DF74C;
    /* cmplwi r28, 0x0 */;
    if (/* eq */) goto .L_800DF74C;
    /* cmplw r4, r28 */;
    if (/* ne */) goto .L_800DF728;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto .L_800DF730;
    goto .L_800DF728;
.L_800DF710:
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* cmplw r0, r28 */;
    if (/* ne */) goto .L_800DF724;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
.L_800DF724:
    r4 = *(u32*)((u8*)r4 + 0x8);
.L_800DF728:
    /* cmplwi r4, 0x0 */;
    if (/* ne */) goto .L_800DF710;
.L_800DF730:
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800DF73C;
    fn_801BBD3C();
.L_800DF73C:
    /* cmplwi r28, 0x0 */;
    if (/* eq */) goto .L_800DF74C;
    r3 = r28;
    fn_801BBED4();
.L_800DF74C:
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x28) = r0;
.L_800DF754:
    r0 = *(u16*)((u8*)r30 + 0x2);
    r0 = r0 & ~r29;
    *(u16*)((u8*)r30 + 0x2) = r0;
    r0 = *(u16*)((u8*)r30 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DF774;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
.L_800DF774:
    r3 = r31;
    fn_801A6FF0();
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x0) = r0;
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r28 = *(u32*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800DF7A4 | Size: 0xB0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF7A4(void) {
    r0 = *(u32*)lbl_8047AB20;
    r3 = *(u32*)lbl_8047AB1C;
    /* mtctr r0 */;
    /* cmplwi r0, 0x0 */;
    if (/* le */) goto .L_800DF7DC;
.L_800DF7C4:
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DF7D4;
    goto .L_800DF7E0;
.L_800DF7D4:
    r3 = r3 + 0x40;
    if (--ctr != 0) goto .L_800DF7C4;
.L_800DF7DC:
    r3 = 0x0;
.L_800DF7E0:
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800DF800;
    r3 = (u32)lbl_8027056C;
    r3 = (u32)lbl_8027056C;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = 0x0;
    goto .L_800DF844;
.L_800DF800:
    r7 = 0x1;
    r4 = (0xfeff << 16);
    *(u8*)((u8*)r3 + 0x0) = r7;
    /* subi r6, r4, 0x102 */;
    r5 = 0x0;
    r4 = 0x2;
    *(u32*)((u8*)r3 + 0x3C) = r6;
    r0 = 0x3;
    *(u16*)((u8*)r3 + 0x2) = r5;
    *(u32*)((u8*)r3 + 0x10) = r5;
    *(u32*)((u8*)r3 + 0x14) = r7;
    *(u32*)((u8*)r3 + 0x18) = r4;
    *(u32*)((u8*)r3 + 0x1C) = r0;
    *(u32*)((u8*)r3 + 0x28) = r5;
    *(u32*)((u8*)r3 + 0x20) = r5;
    *(u32*)((u8*)r3 + 0x24) = r5;
    *(u32*)((u8*)r3 + 0x38) = r6;
.L_800DF844:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DF854 -- GSmaterialInit | Size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF854(void) {
    *(u32*)lbl_8047AB20 = r3;
    r3 = r3 << 6;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AB18 = r3;
    if (/* eq */) goto .L_800DF8BC;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AB1C = r3;
    r4 = r5;
    r6 = 0x0;
    goto .L_800DF8A4;
.L_800DF894:
    r3 = *(u32*)lbl_8047AB1C;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x40;
.L_800DF8A4:
    r0 = *(u32*)lbl_8047AB20;
    /* cmplw r6, r0 */;
    if (/* lt */) goto .L_800DF894;
    r3 = (u32)lbl_80315490;
    r3 = (u32)lbl_80315490;
    fn_801A7CFC();
.L_800DF8BC:
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DF8CC -- GSmaterialClassInit | Size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF8CC(void) {
    r3 = (u32)lbl_802705C0;
    r4 = (u32)lbl_8036CB30;
    r6 = (u32)lbl_80315490;
    r5 = (u32)lbl_802705C0;
    r7 = 0x54;
    r3 = (u32)lbl_80315490;
    r4 = (u32)lbl_8036CB30;
    r8 = 0x24;
    r6 = r5;
    fn_80193B30();
    r5 = (u32)fn_800DFE98;
    r4 = (u32)lbl_80315490;
    r3 = (u32)fn_800DF930;
    r5 = (u32)fn_800DFE98;
    r4 = (u32)lbl_80315490;
    r0 = (u32)fn_800DF930;
    *(u32*)((u8*)r4 + 0x40) = r5;
    *(u32*)((u8*)r4 + 0x44) = r0;
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800DF930 | Size: 0x18C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DF930(void) {
    r6 = (u32)lbl_8036CB30;
    r6 = (u32)lbl_8036CB30;
    r29 = r3;
    r28 = r5;
    r12 = *(u32*)((u8*)r6 + 0x44);
    /* mtctr r12 */;
    /* indirect call via ctr */;
    r29 = *(u32*)((u8*)r29 + 0x20);
    r30 = r3;
    /* cmplwi r29, 0x0 */;
    if (/* ne */) goto .L_800DF97C;
    goto .L_800DFA9C;
.L_800DF97C:
    r0 = *(u16*)((u8*)r29 + 0x2);
    /* rlwinm r0, r0, 0, 30, 30 */;
    if (/* eq */) goto .L_800DF9C4;
    r31 = r30;
    goto .L_800DF9BC;
.L_800DF990:
    r3 = r31;
    fn_801B7C60();
    /* cmpwi r3, 0x1 */;
    if (/* ne */) goto .L_800DF9B8;
    r4 = *(u32*)((u8*)r29 + 0x10);
    r3 = r31;
    r5 = *(u32*)((u8*)r29 + 0x14);
    r6 = *(u32*)((u8*)r29 + 0x18);
    r7 = *(u32*)((u8*)r29 + 0x1C);
    fn_801B6DC0();
.L_800DF9B8:
    r31 = *(u32*)((u8*)r31 + 0x4);
.L_800DF9BC:
    /* cmplwi r31, 0x0 */;
    if (/* ne */) goto .L_800DF990;
.L_800DF9C4:
    r0 = *(u16*)((u8*)r29 + 0x2);
    r0 = r0 & 0x1;
    if (/* eq */) goto .L_800DFA98;
    r6 = r28;
    r3 = r29 + 0xc;
    r4 = 0x1;
    r5 = 0x0;
    fn_801B6F5C();
    r31 = r3;
    r6 = r28;
    r3 = r29 + 0xf;
    r4 = 0x6;
    r5 = 0x0;
    fn_801B6F5C();
    r0 = r3;
    r3 = r28;
    r28 = r0;
    fn_801B707C();
    r4 = 0x0;
    r29 = r3;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x1;
    fn_801B6E74();
    r0 = 0x0;
    r3 = r29;
    r7 = r31;
    r9 = r30;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x1;
    r8 = 0x1;
    r10 = 0x0;
    fn_801B64EC();
    r3 = r29;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x1;
    fn_801B6CD8();
    r0 = 0x0;
    r3 = r29;
    r7 = r28;
    r9 = r30;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x5;
    r8 = 0x5;
    r10 = 0x0;
    fn_801B5F08();
    r30 = r29;
.L_800DFA98:
    r3 = r30;
.L_800DFA9C:
    r0 = *(u32*)((u8*)r1 + 0x24);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r29 = *(u32*)((u8*)r1 + 0x14);
    r28 = *(u32*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800DFABC | Size: 0x3DC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFABC(void) {
    /* stmw r27, 0xc(r1) */;
    r28 = r3;
    r31 = *(u32*)((u8*)r3 + 0x28);
    /* cmplwi r31, 0x0 */;
    if (/* eq */) goto .L_800DFD78;
    r30 = *(u32*)((u8*)r28 + 0x8);
    fn_801BBD84();
    r4 = (u32)lbl_803154E4;
    r5 = 0x80;
    r4 = (u32)lbl_803154E4;
    r29 = r3;
    *(u32*)((u8*)r4 + 0x40) = r5;
    f0 = *(f32*)((u8*)r28 + 0x34);
    *(f32*)((u8*)r4 + 0x44) = f0;
    r0 = *(u32*)((u8*)r28 + 0x2C);
    /* cmpwi r0, 0x1 */;
    if (/* eq */) goto .L_800DFB20;
    if (/* ge */) goto .L_800DFB38;
    /* cmpwi r0, 0x0 */;
    if (/* ge */) goto .L_800DFB2C;
    goto .L_800DFB38;
    goto .L_800DFB38;
.L_800DFB20:
    r0 = r5 | 0x5;
    *(u32*)((u8*)r4 + 0x40) = r0;
    goto .L_800DFB44;
.L_800DFB2C:
    r0 = r5 | 0x1;
    *(u32*)((u8*)r4 + 0x40) = r0;
    goto .L_800DFB44;
.L_800DFB38:
    r0 = *(u32*)((u8*)r4 + 0x40);
    r0 = r0 | 0x6;
    *(u32*)((u8*)r4 + 0x40) = r0;
.L_800DFB44:
    r0 = *(u32*)((u8*)r28 + 0x30);
    /* cmpwi r0, 0x0 */;
    if (/* ne */) goto .L_800DFB60;
    r0 = *(u32*)((u8*)r4 + 0x40);
    r0 = r0 | (0x4 << 16);
    *(u32*)((u8*)r4 + 0x40) = r0;
    goto .L_800DFB6C;
.L_800DFB60:
    r0 = *(u32*)((u8*)r4 + 0x40);
    r0 = r0 | (0x3 << 16);
    *(u32*)((u8*)r4 + 0x40) = r0;
.L_800DFB6C:
    r3 = r31;
    r4 = 0x0;
    fn_800EF548();
    *(u32*)((u8*)r29 + 0x0) = r3;
    r3 = r31;
    fn_800EF4FC();
    *(u16*)((u8*)r29 + 0x4) = r3;
    r3 = r31;
    fn_800EF4F4();
    *(u16*)((u8*)r29 + 0x6) = r3;
    r3 = r31;
    r4 = 0x1;
    fn_800EF3E0();
    *(u32*)((u8*)r29 + 0x8) = r3;
    r0 = *(u32*)((u8*)r29 + 0x8);
    /* cmpwi r0, 0xe */;
    if (/* eq */) goto .L_800DFCE4;
    if (/* ge */) goto .L_800DFBC4;
    /* cmpwi r0, 0x7 */;
    if (/* ge */) goto .L_800DFBC4;
    /* cmpwi r0, 0x0 */;
    if (/* ge */) goto .L_800DFCE4;
.L_800DFBC4:
    r3 = (u32)lbl_802705D0;
    r3 = (u32)lbl_802705D0;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = r29;
    fn_801BBD3C();
    r3 = r29;
    fn_801BBD60();
    r0 = *(u16*)((u8*)r28 + 0x2);
    r31 = *(u32*)((u8*)r28 + 0x8);
    /* rlwinm r29, r0, 0, 29, 29 */;
    r0 = r29 & 0x1;
    if (/* eq */) goto .L_800DFC0C;
    r0 = 0x7f;
    *(u8*)((u8*)r28 + 0xF) = r0;
    *(u8*)((u8*)r28 + 0xE) = r0;
    *(u8*)((u8*)r28 + 0xD) = r0;
    *(u8*)((u8*)r28 + 0xC) = r0;
.L_800DFC0C:
    /* rlwinm r0, r29, 0, 30, 30 */;
    if (/* eq */) goto .L_800DFC34;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r28 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r28 + 0x14) = r4;
    *(u32*)((u8*)r28 + 0x18) = r3;
    *(u32*)((u8*)r28 + 0x1C) = r0;
.L_800DFC34:
    /* rlwinm r0, r29, 0, 29, 29 */;
    if (/* eq */) goto .L_800DFCB8;
    r5 = *(u32*)((u8*)r28 + 0x8);
    r30 = *(u32*)((u8*)r28 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r28 + 0x24);
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800DFCB0;
    /* cmplwi r30, 0x0 */;
    if (/* eq */) goto .L_800DFCB0;
    /* cmplw r4, r30 */;
    if (/* ne */) goto .L_800DFC8C;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto .L_800DFC94;
    goto .L_800DFC8C;
.L_800DFC74:
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* cmplw r0, r30 */;
    if (/* ne */) goto .L_800DFC88;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
.L_800DFC88:
    r4 = *(u32*)((u8*)r4 + 0x8);
.L_800DFC8C:
    /* cmplwi r4, 0x0 */;
    if (/* ne */) goto .L_800DFC74;
.L_800DFC94:
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800DFCA0;
    fn_801BBD3C();
.L_800DFCA0:
    /* cmplwi r30, 0x0 */;
    if (/* eq */) goto .L_800DFCB0;
    r3 = r30;
    fn_801BBED4();
.L_800DFCB0:
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x28) = r0;
.L_800DFCB8:
    r0 = *(u16*)((u8*)r28 + 0x2);
    r0 = r0 & ~r29;
    *(u16*)((u8*)r28 + 0x2) = r0;
    r0 = *(u16*)((u8*)r28 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DFCD8;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
.L_800DFCD8:
    r3 = r31;
    fn_801A6FF0();
    goto .L_800DFE84;
.L_800DFCE4:
    r3 = r31;
    fn_800EF4E4();
    r4 = r3 & 0xFF;
    r3 = (u32)lbl_803154E4;
    r0 = -r4;
    f0 = *(f32*)lbl_8047CAC8;
    r0 = r0 | r4;
    r3 = (u32)lbl_803154E4;
    r0 = (u32)r0 >> 31;
    *(u32*)((u8*)r29 + 0xC) = r0;
    *(f32*)((u8*)r29 + 0x10) = f0;
    *(f32*)((u8*)r29 + 0x14) = f0;
    *(u32*)((u8*)r3 + 0x4C) = r29;
    fn_801BE4CC();
    r0 = r3;
    r3 = r30;
    r27 = r0;
    fn_801A6DC4();
    /* mr. r4, r3 */;
    if (/* eq */) goto .L_800DFD58;
    goto .L_800DFD3C;
.L_800DFD38:
    r4 = r0;
.L_800DFD3C:
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DFD38;
    r3 = r30;
    r5 = r27;
    fn_801A6D5C();
    goto .L_800DFD64;
.L_800DFD58:
    r3 = r30;
    r4 = r27;
    fn_801A6DA0();
.L_800DFD64:
    *(u32*)((u8*)r28 + 0x20) = r27;
    r3 = r31;
    *(u32*)((u8*)r28 + 0x24) = r29;
    fn_800EF504();
    goto .L_800DFE84;
.L_800DFD78:
    r3 = (u32)lbl_80270610;
    r3 = (u32)lbl_80270610;
    /* crclr cr1eq */;
    fn_800DD970();
    r0 = *(u16*)((u8*)r28 + 0x2);
    r31 = *(u32*)((u8*)r28 + 0x8);
    /* rlwinm r29, r0, 0, 29, 29 */;
    r0 = r29 & 0x1;
    if (/* eq */) goto .L_800DFDB0;
    r0 = 0x7f;
    *(u8*)((u8*)r28 + 0xF) = r0;
    *(u8*)((u8*)r28 + 0xE) = r0;
    *(u8*)((u8*)r28 + 0xD) = r0;
    *(u8*)((u8*)r28 + 0xC) = r0;
.L_800DFDB0:
    /* rlwinm r0, r29, 0, 30, 30 */;
    if (/* eq */) goto .L_800DFDD8;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r28 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r28 + 0x14) = r4;
    *(u32*)((u8*)r28 + 0x18) = r3;
    *(u32*)((u8*)r28 + 0x1C) = r0;
.L_800DFDD8:
    /* rlwinm r0, r29, 0, 29, 29 */;
    if (/* eq */) goto .L_800DFE5C;
    r5 = *(u32*)((u8*)r28 + 0x8);
    r30 = *(u32*)((u8*)r28 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r28 + 0x24);
    /* cmplwi r4, 0x0 */;
    if (/* eq */) goto .L_800DFE54;
    /* cmplwi r30, 0x0 */;
    if (/* eq */) goto .L_800DFE54;
    /* cmplw r4, r30 */;
    if (/* ne */) goto .L_800DFE30;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto .L_800DFE38;
    goto .L_800DFE30;
.L_800DFE18:
    r0 = *(u32*)((u8*)r4 + 0x8);
    /* cmplw r0, r30 */;
    if (/* ne */) goto .L_800DFE2C;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
.L_800DFE2C:
    r4 = *(u32*)((u8*)r4 + 0x8);
.L_800DFE30:
    /* cmplwi r4, 0x0 */;
    if (/* ne */) goto .L_800DFE18;
.L_800DFE38:
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800DFE44;
    fn_801BBD3C();
.L_800DFE44:
    /* cmplwi r30, 0x0 */;
    if (/* eq */) goto .L_800DFE54;
    r3 = r30;
    fn_801BBED4();
.L_800DFE54:
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x28) = r0;
.L_800DFE5C:
    r0 = *(u16*)((u8*)r28 + 0x2);
    r0 = r0 & ~r29;
    *(u16*)((u8*)r28 + 0x2) = r0;
    r0 = *(u16*)((u8*)r28 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800DFE7C;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
.L_800DFE7C:
    r3 = r31;
    fn_801A6FF0();
.L_800DFE84:
    /* lmw r27, 0xc(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    return;
}
#pragma pop

/* fn_800DFE98 | Size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFE98(void) {
    r5 = (u32)lbl_8036CB30;
    r5 = (u32)lbl_8036CB30;
    r31 = r3;
    r12 = *(u32*)((u8*)r5 + 0x40);
    /* mtctr r12 */;
    /* indirect call via ctr */;
    /* cmpwi r3, 0x0 */;
    if (/* eq */) goto .L_800DFECC;
    goto .L_800DFED8;
.L_800DFECC:
    r0 = 0x0;
    r3 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
.L_800DFED8:
    r0 = *(u32*)((u8*)r1 + 0x14);
    r31 = *(u32*)((u8*)r1 + 0xC);
    return;
}
#pragma pop

/* fn_800DFEEC | Size: 0xAC */
void fn_800DFEEC(void) {
    *(f32*)((u8*)r3 + 0x0) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
}

/* fn_800DFF98 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFF98(void) {
    /* mr r6, r3 */;
    /* mr r3, r4 */;
    /* mr r0, r5 */;
    /* mr r5, r6 */;
    /* mr r4, r0 */;
    fn_800A37CC();
}
#pragma pop

/* fn_800DFFCC | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800DFFCC(void) {
    /* mr r6, r3 */;
    /* mr r3, r4 */;
    /* mr r0, r5 */;
    /* mr r5, r6 */;
    /* mr r4, r0 */;
    fn_800A3B9C();
}
#pragma pop

/* fn_800E0000 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0000(void) {
    fn_800A3B7C();
}
#pragma pop

/* fn_800E0020 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0020(void) {
    fn_800A3BD8();
}
#pragma pop

/* fn_800E0040 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0040(void) {
    fn_800A3C00();
}
#pragma pop

/* fn_800E0060 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0060(void) {
    /* mr r0, r3 */;
    /* mr r3, r4 */;
    /* mr r4, r0 */;
    fn_800A3ADC();
}
#pragma pop

/* fn_800E008C | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E008C(void) {
    fn_800A3B38();
}
#pragma pop

/* fn_800E00AC | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E00AC(void) {
    /* lfs f0, lbl_8047CAD8@sda21(r0) */;
    /* mr r0, r3 */;
    /* fdivs f1, f0, f1 */;
    /* mr r3, r4 */;
    /* mr r4, r0 */;
    fn_800A3AC0();
}
#pragma pop

/* fn_800E00E0 | Size: 0x28 */
void fn_800E00E0(void) {
    *(f32*)((u8*)r3 + 0x0) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
}

/* fn_800E0108 | Size: 0x34 */
void fn_800E0108(void) {
    *(f32*)((u8*)r3 + 0x0) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
}

/* fn_800E013C | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E013C(void) {
    /* mr r0, r3 */;
    /* mr r3, r4 */;
    /* mr r4, r0 */;
    fn_800A3AC0();
}
#pragma pop

/* fn_800E0168 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0168(void) {
    /* mr r6, r3 */;
    /* mr r3, r4 */;
    /* mr r0, r5 */;
    /* mr r5, r6 */;
    /* mr r4, r0 */;
    fn_800A3A9C();
}
#pragma pop

/* fn_800E019C | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E019C(void) {
    /* mr r6, r3 */;
    /* mr r3, r4 */;
    /* mr r0, r5 */;
    /* mr r5, r6 */;
    /* mr r4, r0 */;
    fn_800A3A78();
}
#pragma pop

/* fn_800E01D0 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E01D0(void) {
    /* li r5, 0xc */;
    memcpy();
}
#pragma pop

/* fn_800E01F4 | Size: 0x10 */
void fn_800E01F4(void) {
    *(f32*)((u8*)r3 + 0x0) = f1;
    *(f32*)((u8*)r3 + 0x4) = f2;
    *(f32*)((u8*)r3 + 0x8) = f3;
}

/* fn_800E0204 | Size: 0x14 */
void fn_800E0204(void) {
    *(f32*)((u8*)r3 + 0x0) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
}

/* fn_800E0218 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0218(void) {
    fn_800A3458();
}
#pragma pop

/* fn_800E0238 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0238(void) {
    /* mr r0, r3 */;
    /* mr r3, r4 */;
    /* mr r4, r0 */;
    fn_800A2E64();
}
#pragma pop

/* fn_800E0264 | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0264(void) {
    /* mr r0, r3 */;
    /* mr r3, r4 */;
    /* mr r4, r0 */;
    fn_800A2EB4();
}
#pragma pop

/* fn_800E0290 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0290(void) {
    /* mr r6, r3 */;
    /* mr r3, r4 */;
    /* mr r0, r5 */;
    /* mr r5, r6 */;
    /* mr r4, r0 */;
    fn_800A2D98();
}
#pragma pop

/* fn_800E02C4 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E02C4(void) {
    /* mr r4, r3 */;
    fn_800A335C();
}
#pragma pop

/* fn_800E02E8 | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E02E8(void) {
    r4 = 0x5a;
    r31 = r3;
    r3 = r1 + 0x8;
    fn_800A3074();
    r3 = r31;
    r5 = r31;
    r4 = r1 + 0x8;
    fn_800A2D98();
    r0 = *(u32*)((u8*)r1 + 0x44);
    r31 = *(u32*)((u8*)r1 + 0x3C);
    return;
}
#pragma pop

/* fn_800E032C | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E032C(void) {
    r4 = 0x59;
    r31 = r3;
    r3 = r1 + 0x8;
    fn_800A3074();
    r3 = r31;
    r5 = r31;
    r4 = r1 + 0x8;
    fn_800A2D98();
    r0 = *(u32*)((u8*)r1 + 0x44);
    r31 = *(u32*)((u8*)r1 + 0x3C);
    return;
}
#pragma pop

/* fn_800E0370 | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0370(void) {
    r4 = 0x58;
    r31 = r3;
    r3 = r1 + 0x8;
    fn_800A3074();
    r3 = r31;
    r5 = r31;
    r4 = r1 + 0x8;
    fn_800A2D98();
    r0 = *(u32*)((u8*)r1 + 0x44);
    r31 = *(u32*)((u8*)r1 + 0x3C);
    return;
}
#pragma pop

/* fn_800E03B4 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E03B4(void) {
    /* mr r5, r4 */;
    /* lfs f1, 0x0(r4) */;
    /* mr r4, r3 */;
    /* lfs f2, 0x4(r5) */;
    /* lfs f3, 0x8(r5) */;
    fn_800A32E8();
}
#pragma pop

/* fn_800E03E8 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E03E8(void) {
    /* mr r4, r3 */;
    fn_800A32E8();
}
#pragma pop

/* fn_800E040C | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E040C(void) {
    fn_800A33B4();
}
#pragma pop

/* fn_800E042C | Size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E042C(void) {
    r5 = (u32)lbl_80315568;
    r0 = (u32)lbl_80315568;
    r5 = 0x30;
    r31 = r4;
    r4 = r0;
    r30 = r3;
    memcpy();
    f0 = *(f32*)((u8*)r31 + 0x0);
    *(f32*)((u8*)r30 + 0x0) = f0;
    f0 = *(f32*)((u8*)r31 + 0x4);
    *(f32*)((u8*)r30 + 0x14) = f0;
    f0 = *(f32*)((u8*)r31 + 0x8);
    *(f32*)((u8*)r30 + 0x28) = f0;
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800E048C | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E048C(void) {
    r4 = (u32)lbl_80315568;
    r5 = 0x30;
    r4 = (u32)lbl_80315568;
    *(f64*)((u8*)r1 + 0x28) = f31;
    f31 = f3;
    *(f64*)((u8*)r1 + 0x20) = f30;
    f30 = f2;
    *(f64*)((u8*)r1 + 0x18) = f29;
    f29 = f1;
    r31 = r3;
    memcpy();
    *(f32*)((u8*)r31 + 0x0) = f29;
    *(f32*)((u8*)r31 + 0x14) = f30;
    *(f32*)((u8*)r31 + 0x28) = f31;
    f31 = *(f64*)((u8*)r1 + 0x28);
    f30 = *(f64*)((u8*)r1 + 0x20);
    f29 = *(f64*)((u8*)r1 + 0x18);
    r31 = *(u32*)((u8*)r1 + 0x14);
    r0 = *(u32*)((u8*)r1 + 0x34);
    return;
}
#pragma pop

/* fn_800E04F4 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E04F4(void) {
    /* li r4, 0x5a */;
    fn_800A3074();
}
#pragma pop

/* fn_800E0518 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0518(void) {
    /* li r4, 0x59 */;
    fn_800A3074();
}
#pragma pop

/* fn_800E053C | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E053C(void) {
    /* li r4, 0x58 */;
    fn_800A3074();
}
#pragma pop

/* fn_800E0560 | Size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0560(void) {
    r5 = (u32)lbl_80315568;
    r0 = (u32)lbl_80315568;
    r5 = 0x30;
    r31 = r4;
    r4 = r0;
    r30 = r3;
    memcpy();
    f0 = *(f32*)((u8*)r31 + 0x0);
    *(f32*)((u8*)r30 + 0xC) = f0;
    f0 = *(f32*)((u8*)r31 + 0x4);
    *(f32*)((u8*)r30 + 0x1C) = f0;
    f0 = *(f32*)((u8*)r31 + 0x8);
    *(f32*)((u8*)r30 + 0x2C) = f0;
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800E05C0 | Size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E05C0(void) {
    r4 = (u32)lbl_80315568;
    r5 = 0x30;
    r4 = (u32)lbl_80315568;
    *(f64*)((u8*)r1 + 0x28) = f31;
    f31 = f3;
    *(f64*)((u8*)r1 + 0x20) = f30;
    f30 = f2;
    *(f64*)((u8*)r1 + 0x18) = f29;
    f29 = f1;
    r31 = r3;
    memcpy();
    *(f32*)((u8*)r31 + 0xC) = f29;
    *(f32*)((u8*)r31 + 0x1C) = f30;
    *(f32*)((u8*)r31 + 0x2C) = f31;
    f31 = *(f64*)((u8*)r1 + 0x28);
    f30 = *(f64*)((u8*)r1 + 0x20);
    f29 = *(f64*)((u8*)r1 + 0x18);
    r31 = *(u32*)((u8*)r1 + 0x14);
    r0 = *(u32*)((u8*)r1 + 0x34);
    return;
}
#pragma pop

/* fn_800E0628 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0628(void) {
    /* li r5, 0x30 */;
    memcpy();
}
#pragma pop

/* fn_800E064C | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E064C(void) {
    /* lis r4, lbl_80315568@ha */;
    /* li r5, 0x30 */;
    /* addi r4, r4, lbl_80315568@l */;
    memcpy();
}
#pragma pop

/* fn_800E0678 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0678(void) {
    fn_800A3910();
}
#pragma pop

/* fn_800E0698 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0698(void) {
    fn_800A39E0();
}
#pragma pop

/* fn_800E06B8 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E06B8(void) {
    /* mr r6, r3 */;
    /* mr r3, r4 */;
    /* mr r0, r5 */;
    /* mr r5, r6 */;
    /* mr r4, r0 */;
    fn_800A3D3C();
}
#pragma pop

/* fn_800E06EC | Size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E06EC(void) {
    /* mr r0, r3 */;
    /* mr r3, r4 */;
    /* mr r4, r0 */;
    fn_801ADAAC();
}
#pragma pop

/* fn_800E0718 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0718(void) {
    fn_800A3CB0();
}
#pragma pop

/* fn_800E0738 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0738(void) {
    /* mr r6, r3 */;
    /* mr r3, r4 */;
    /* mr r0, r5 */;
    /* mr r5, r6 */;
    /* mr r4, r0 */;
    fn_800A3C54();
}
#pragma pop

/* fn_800E076C | Size: 0x24 */
void fn_800E076C(void) {
    *(f32*)((u8*)r3 + 0x0) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(f32*)((u8*)r3 + 0xC) = f0;
}

/* fn_800E0790 | Size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0790(void) {
    r3 = 0x4;
    r3 = r3 | (0x4 << 16);
    /* mtspr GQR2, r3 */;
    r3 = 0x5;
    r3 = r3 | (0x5 << 16);
    /* mtspr GQR3, r3 */;
    r3 = 0x6;
    r3 = r3 | (0x6 << 16);
    /* mtspr GQR4, r3 */;
    r3 = 0x7;
    r3 = r3 | (0x7 << 16);
    /* mtspr GQR5, r3 */;
    fn_800E0C78();
    fn_800E0D24();
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800E07E4 | Size: 0x128 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E07E4(void) {
    *(f64*)((u8*)r1 + 0x50) = f31;
    /* psq_st f31, 0x58(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x40) = f30;
    /* psq_st f30, 0x48(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x30) = f29;
    /* psq_st f29, 0x38(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x20) = f28;
    /* psq_st f28, 0x28(r1), 0, qr0 */;
    f28 = f1;
    f0 = *(f32*)lbl_8047CAE4;
    r30 = r3;
    r31 = r4;
    /* fcmpo cr0, f28, f0 */;
    if (/* ge */) goto .L_800E0834;
    f28 = f0;
.L_800E0834:
    f0 = *(f32*)lbl_8047CAE0;
    /* fcmpo cr0, f28, f0 */;
    if (/* le */) goto .L_800E0844;
    f28 = f0;
.L_800E0844:
    f0 = *(f32*)lbl_8047CAE0;
    f31 = f28 * f28;
    r3 = r30;
    r4 = r31;
    f30 = f0 - f28;
    f29 = f30 * f30;
    f1 = f29 * f30;
    fn_800E013C();
    f0 = *(f32*)lbl_8047CAE8;
    r3 = r1 + 0x8;
    r4 = r31 + 0x18;
    f0 = f0 * f28;
    f1 = f0 * f29;
    fn_800E013C();
    r3 = r30;
    r4 = r30;
    r5 = r1 + 0x8;
    fn_800E019C();
    f0 = *(f32*)lbl_8047CAE8;
    r3 = r1 + 0x8;
    r4 = r31 + 0x24;
    f0 = f0 * f31;
    f1 = f0 * f30;
    fn_800E013C();
    r3 = r30;
    r4 = r30;
    r5 = r1 + 0x8;
    fn_800E019C();
    f1 = f31 * f28;
    r3 = r1 + 0x8;
    r4 = r31 + 0xc;
    fn_800E013C();
    r3 = r30;
    r4 = r30;
    r5 = r1 + 0x8;
    fn_800E019C();
    /* psq_l f31, 0x58(r1), 0, qr0 */;
    f31 = *(f64*)((u8*)r1 + 0x50);
    /* psq_l f30, 0x48(r1), 0, qr0 */;
    f30 = *(f64*)((u8*)r1 + 0x40);
    /* psq_l f29, 0x38(r1), 0, qr0 */;
    f29 = *(f64*)((u8*)r1 + 0x30);
    /* psq_l f28, 0x28(r1), 0, qr0 */;
    f28 = *(f64*)((u8*)r1 + 0x20);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r0 = *(u32*)((u8*)r1 + 0x64);
    r30 = *(u32*)((u8*)r1 + 0x18);
    return;
}
#pragma pop

/* fn_800E090C | Size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E090C(void) {
    *(f64*)((u8*)r1 + 0x10) = f31;
    /* psq_st f31, 0x18(r1), 0, qr0 */;
    f31 = f1;
    f0 = *(f32*)lbl_8047CAF0;
    r30 = r3;
    r31 = r4;
    /* fcmpo cr0, f31, f0 */;
    /* cror eq, lt, eq */;
    if (/* ne */) goto .L_800E094C;
    fn_800E01D0();
    goto .L_800E0994;
.L_800E094C:
    f0 = *(f32*)lbl_8047CAF4;
    /* fcmpo cr0, f31, f0 */;
    /* cror eq, gt, eq */;
    if (/* ne */) goto .L_800E0968;
    r4 = r5;
    fn_800E01D0();
    goto .L_800E0994;
.L_800E0968:
    r4 = r5;
    r5 = r31;
    fn_800E0168();
    f1 = f31;
    r3 = r30;
    r4 = r30;
    fn_800E013C();
    r3 = r30;
    r4 = r30;
    r5 = r31;
    fn_800E019C();
.L_800E0994:
    /* psq_l f31, 0x18(r1), 0, qr0 */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    f31 = *(f64*)((u8*)r1 + 0x10);
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    return;
}
#pragma pop

/* fn_800E09B4 | Size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E09B4(void) {
    f0 = *(f32*)lbl_8047CAF0;
    /* fcmpo cr0, f3, f0 */;
    /* cror eq, lt, eq */;
    if (/* eq */) return;
    f0 = *(f32*)lbl_8047CAF4;
    /* fcmpo cr0, f3, f0 */;
    /* cror eq, gt, eq */;
    if (/* ne */) goto .L_800E09DC;
    f1 = f2;
    return;
.L_800E09DC:
    f0 = f2 - f1;
    f1 = f3 * f0 + f1;
    return;
}
#pragma pop

/* fn_800E09E8 | Size: 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E09E8(void) {
    *(f64*)((u8*)r1 + 0xA0) = f31;
    /* psq_st f31, 0xa8(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x90) = f30;
    /* psq_st f30, 0x98(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x80) = f29;
    /* psq_st f29, 0x88(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x70) = f28;
    /* psq_st f28, 0x78(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x60) = f27;
    /* psq_st f27, 0x68(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x50) = f26;
    /* psq_st f26, 0x58(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x40) = f25;
    /* psq_st f25, 0x48(r1), 0, qr0 */;
    /* stmw r27, 0x2c(r1) */;
    r28 = r5;
    r0 = (0x4330 << 16);
    /* subi r5, r28, 0x1 */;
    f1 = *(f64*)lbl_8047CB00;
    r29 = r3;
    r27 = r4;
    f2 = *(f32*)lbl_8047CAF8;
    f0 = *(f64*)((u8*)r1 + 0x18);
    f0 = f0 - f1;
    f25 = f2 / f0;
    fn_800E01D0();
    f29 = *(f64*)lbl_8047CB08;
    r29 = r29 + 0xc;
    f30 = *(f32*)lbl_8047CAFC;
    /* subi r31, r28, 0x1 */;
    f31 = *(f32*)lbl_8047CAF8;
    r28 = 0x1;
    r30 = (0x4330 << 16);
    goto .L_800E0B40;
.L_800E0A84:
    /* xoris r0, r28, 0x8000 */;
    f0 = *(f64*)((u8*)r1 + 0x18);
    f0 = f0 - f29;
    f27 = f0 * f25;
    /* fcmpo cr0, f27, f30 */;
    if (/* ge */) goto .L_800E0AA8;
    f27 = f30;
.L_800E0AA8:
    /* fcmpo cr0, f27, f31 */;
    if (/* le */) goto .L_800E0AB4;
    f27 = f31;
.L_800E0AB4:
    f28 = f27 * f27;
    r3 = r29;
    r4 = r27 + 0x24;
    f0 = f28 * f27;
    f26 = f0 - f28;
    f1 = f26;
    fn_800E013C();
    f28 = f26 - f28;
    r3 = r1 + 0x8;
    r4 = r27 + 0x18;
    f1 = f28 + f27;
    fn_800E013C();
    r3 = r29;
    r4 = r29;
    r5 = r1 + 0x8;
    fn_800E019C();
    f28 = f28 + f26;
    r4 = r27;
    r3 = r1 + 0x8;
    f1 = f31 + f28;
    fn_800E013C();
    r3 = r29;
    r4 = r29;
    r5 = r1 + 0x8;
    fn_800E019C();
    f1 = -f28;
    r3 = r1 + 0x8;
    r4 = r27 + 0xc;
    fn_800E013C();
    r3 = r29;
    r4 = r29;
    r5 = r1 + 0x8;
    fn_800E019C();
    r28 = r28 + 0x1;
    r29 = r29 + 0xc;
.L_800E0B40:
    /* cmplw r28, r31 */;
    if (/* lt */) goto .L_800E0A84;
    r3 = r29;
    r4 = r27 + 0xc;
    fn_800E01D0();
    /* psq_l f31, 0xa8(r1), 0, qr0 */;
    f31 = *(f64*)((u8*)r1 + 0xA0);
    /* psq_l f30, 0x98(r1), 0, qr0 */;
    f30 = *(f64*)((u8*)r1 + 0x90);
    /* psq_l f29, 0x88(r1), 0, qr0 */;
    f29 = *(f64*)((u8*)r1 + 0x80);
    /* psq_l f28, 0x78(r1), 0, qr0 */;
    f28 = *(f64*)((u8*)r1 + 0x70);
    /* psq_l f27, 0x68(r1), 0, qr0 */;
    f27 = *(f64*)((u8*)r1 + 0x60);
    /* psq_l f26, 0x58(r1), 0, qr0 */;
    f26 = *(f64*)((u8*)r1 + 0x50);
    /* psq_l f25, 0x48(r1), 0, qr0 */;
    f25 = *(f64*)((u8*)r1 + 0x40);
    /* lmw r27, 0x2c(r1) */;
    r0 = *(u32*)((u8*)r1 + 0xB4);
    return;
}
#pragma pop

/* fn_800E0BA0 | Size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0BA0(void) {
    *(f64*)((u8*)r1 + 0x10) = f31;
    /* psq_st f31, 0x18(r1), 0, qr0 */;
    fn_801ADC7C();
    f31 = f1;
    fn_801ADC7C();
    f1 = f1 + f31;
    f0 = *(f32*)lbl_8047CB10;
    f1 = f1 - f0;
    /* psq_l f31, 0x18(r1), 0, qr0 */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    f31 = *(f64*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800E0BE4 | Size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0BE4(void) {
    fn_801ADC7C();
}
#pragma pop

/* fn_800E0C04 | Size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0C04(void) {
    r30 = r3;
    fn_801ADCD8();
    r31 = r3;
    fn_801ADCD8();
    r0 = r3 << 16;
    r3 = r0 | r31;
    r0 = (u32)r3 / (u32)r30;
    r0 = r0 * r30;
    r3 = r3 - r0;
    r31 = *(u32*)((u8*)r1 + 0xC);
    r30 = *(u32*)((u8*)r1 + 0x8);
    r0 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800E0C54 | Size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0C54(void) {
    fn_801ADCD8();
}
#pragma pop

/* fn_800E0C78 | Size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0C78(void) {
    OSGetTime();
}
#pragma pop

/* fn_800E0CA0 | Size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0CA0(void) {
    *(f64*)((u8*)r1 + 0x10) = f31;
    /* psq_st f31, 0x18(r1), 0, qr0 */;
    f0 = *(f32*)lbl_8047CB20;
    f31 = *(f32*)lbl_8047CB1C;
    /* fcmpo cr0, f1, f0 */;
    if (/* le */) goto .L_800E0CD4;
    f31 = *(f32*)lbl_8047CB24;
    f2 = *(f64*)lbl_8047CB28;
    fn_800CE318();
    f1 = (f32)f1;
.L_800E0CD4:
    f0 = *(f32*)lbl_8047CB18;
    /* fcmpo cr0, f1, f0 */;
    if (/* le */) goto .L_800E0CE8;
    f0 = *(f32*)lbl_8047CB20;
    f1 = f0 - f1;
.L_800E0CE8:
    f2 = *(f32*)lbl_8047CB34;
    f0 = *(f32*)lbl_8047CB30;
    f1 = f2 * f1 + f0;
    fn_800C46B0();
    r4 = (u32)lbl_804011B8;
    r0 = r3 << 2;
    r3 = (u32)lbl_804011B8;
    /* lfsx f0, r3, r0 */;
    f1 = f31 * f0;
    /* psq_l f31, 0x18(r1), 0, qr0 */;
    r0 = *(u32*)((u8*)r1 + 0x24);
    f31 = *(f64*)((u8*)r1 + 0x10);
    return;
}
#pragma pop

/* fn_800E0D24 | Size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0D24(void) {
    *(f64*)((u8*)r1 + 0x40) = f31;
    /* psq_st f31, 0x48(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x30) = f30;
    /* psq_st f30, 0x38(r1), 0, qr0 */;
    *(f64*)((u8*)r1 + 0x20) = f29;
    /* psq_st f29, 0x28(r1), 0, qr0 */;
    r3 = (u32)lbl_804011B8;
    f29 = *(f32*)lbl_8047CB38;
    f30 = *(f32*)lbl_8047CB30;
    r30 = (u32)lbl_804011B8;
    f31 = *(f64*)lbl_8047CB40;
    r29 = 0x0;
    r31 = (0x4330 << 16);
.L_800E0D70:
    /* xoris r0, r29, 0x8000 */;
    f0 = *(f64*)((u8*)r1 + 0x8);
    f0 = f0 - f31;
    f0 = f30 * f0;
    f1 = f29 * f0;
    fn_800CDBE0();
    f0 = (f32)f1;
    r29 = r29 + 0x1;
    /* cmpwi r29, 0xb5 */;
    *(f32*)((u8*)r30 + 0x0) = f0;
    r30 = r30 + 0x4;
    if (/* lt */) goto .L_800E0D70;
    /* psq_l f31, 0x48(r1), 0, qr0 */;
    f31 = *(f64*)((u8*)r1 + 0x40);
    /* psq_l f30, 0x38(r1), 0, qr0 */;
    f30 = *(f64*)((u8*)r1 + 0x30);
    /* psq_l f29, 0x28(r1), 0, qr0 */;
    f29 = *(f64*)((u8*)r1 + 0x20);
    r31 = *(u32*)((u8*)r1 + 0x1C);
    r30 = *(u32*)((u8*)r1 + 0x18);
    r0 = *(u32*)((u8*)r1 + 0x54);
    r29 = *(u32*)((u8*)r1 + 0x14);
    return;
}
#pragma pop

/* fn_800E0DDC | Size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0DDC(void) {
    r5 = __OSStartTime@sda21;
    r3 = 0x0;
    r4 = *(u32*)((u8*)r5 + 0x10);
    *(u32*)lbl_8047AB44 = r4;
    r0 = *(u32*)((u8*)r5 + 0x14);
    *(u32*)lbl_8047AB40 = r0;
    r4 = *(u32*)((u8*)r4 + 0x4);
    goto .L_800E0E08;
.L_800E0DFC:
    r0 = *(u32*)((u8*)r4 + 0x8);
    r4 = *(u32*)((u8*)r4 + 0x4);
    r3 = r3 + r0;
.L_800E0E08:
    /* cmplwi r4, 0x0 */;
    if (/* ne */) goto .L_800E0DFC;
    return;
}
#pragma pop

/* fn_800E0E14 | Size: 0x730 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E0E14(void) {
    /* stmw r24, 0x20(r1) */;
    r25 = r3;
    r3 = (u32)lbl_80270658;
    r26 = r4;
    r0 = r25 & 0xFF;
    r29 = 0x1;
    r31 = (u32)lbl_80270658;
    r28 = 0x0;
    r27 = 0x0;
    if (/* eq */) goto .L_800E0E54;
    r3 = r31 + 0x0;
    /* crclr cr1eq */;
    fn_800DD970();
.L_800E0E54:
    r3 = *(u32*)lbl_8047AB30;
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800E0E7C;
    r0 = *(u32*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E0E7C;
    r3 = r31 + 0x34;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0E7C:
    r24 = *(u32*)lbl_8047AB30;
    goto .L_800E0F54;
.L_800E0E84:
    r0 = *(u32*)lbl_8047AB68;
    r27 = r27 + 0x1;
    /* cmplw r24, r0 */;
    if (/* lt */) goto .L_800E0EA0;
    r0 = *(u32*)lbl_8047AB64;
    /* cmplw r24, r0 */;
    if (/* le */) goto .L_800E0EB0;
.L_800E0EA0:
    r3 = r31 + 0x64;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0EB0:
    r0 = *(u32*)lbl_8047AB38;
    /* cmplw r24, r0 */;
    if (/* lt */) goto .L_800E0ECC;
    r3 = r31 + 0x90;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0ECC:
    r3 = *(u32*)((u8*)r24 + 0x8);
    r0 = *(u32*)lbl_8047AB38;
    r3 = r24 + r3;
    /* cmplw r3, r0 */;
    if (/* le */) goto .L_800E0EF0;
    r3 = r31 + 0xc0;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0EF0:
    r30 = *(u32*)((u8*)r24 + 0x4);
    /* cmplwi r30, 0x0 */;
    if (/* eq */) goto .L_800E0F50;
    r0 = *(u32*)((u8*)r30 + 0x0);
    /* cmplw r0, r24 */;
    if (/* eq */) goto .L_800E0F18;
    r3 = r31 + 0xf8;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0F18:
    r0 = *(u32*)((u8*)r24 + 0x8);
    r0 = r24 + r0;
    /* cmplw r30, r0 */;
    if (/* ne */) goto .L_800E0F38;
    r3 = r31 + 0x124;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0F38:
    /* cmplw r24, r30 */;
    if (/* le */) goto .L_800E0F50;
    r3 = r31 + 0x154;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0F50:
    r24 = *(u32*)((u8*)r24 + 0x4);
.L_800E0F54:
    /* cmplwi r24, 0x0 */;
    if (/* ne */) goto .L_800E0E84;
    r30 = *(u32*)lbl_8047AB34;
    goto .L_800E11EC;
.L_800E0F64:
    r0 = *(u16*)((u8*)r30 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E11E8;
    r3 = *(u32*)((u8*)r30 + 0x4);
    r28 = r28 + 0x1;
    r0 = *(u32*)lbl_8047AB68;
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800E0F94;
    r0 = *(u32*)((u8*)r30 + 0x8);
    r0 = r3 + r0;
    /* cmplw r0, r4 */;
    if (/* le */) goto .L_800E0FA4;
.L_800E0F94:
    r3 = r31 + 0x180;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E0FA4:
    r0 = *(u8*)lbl_8047AB28;
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E11E8;
    r4 = *(u32*)((u8*)r30 + 0x4);
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E0FC8;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E0FC8:
    r0 = *(u8*)((u8*)r4 + 0x1);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E0FDC;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E0FDC:
    r0 = *(u8*)((u8*)r4 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E0FF0;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E0FF0:
    r0 = *(u8*)((u8*)r4 + 0x3);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E1004;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E1004:
    r3 = *(u32*)((u8*)r30 + 0x8);
    /* subi r3, r3, 0x4 */;
    r3 = r4 + r3;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E1024;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E1024:
    r0 = *(u8*)((u8*)r3 + 0x1);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E1038;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E1038:
    r0 = *(u8*)((u8*)r3 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E104C;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E104C:
    r0 = *(u8*)((u8*)r3 + 0x3);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E1060;
    r0 = 0x0;
    goto .L_800E1064;
.L_800E1060:
    r0 = 0x1;
.L_800E1064:
    r0 = r0 & 0xFF;
    if (/* ne */) goto .L_800E10B8;
    r4 = *(u16*)((u8*)r30 + 0x0);
    r3 = r31 + 0x1ac;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = *(u32*)((u8*)r30 + 0x4);
    r4 = 0x0;
    r29 = 0x0;
    *(u8*)((u8*)r3 + 0x0) = r4;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u8*)((u8*)r3 + 0x2) = r4;
    *(u8*)((u8*)r3 + 0x3) = r4;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)((u8*)r30 + 0x4);
    /* subi r3, r3, 0x4 */;
    r3 = r0 + r3;
    *(u8*)((u8*)r3 + 0x0) = r4;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u8*)((u8*)r3 + 0x2) = r4;
    *(u8*)((u8*)r3 + 0x3) = r4;
.L_800E10B8:
    r0 = *(u16*)((u8*)r30 + 0x2);
    /* cmplwi r0, 0x0 */;
    if (/* ne */) goto .L_800E11E8;
    r0 = *(u32*)((u8*)r30 + 0x8);
    r6 = 0x3d94;
    r5 = *(u32*)((u8*)r30 + 0x4);
    /* srwi. r4, r0, 1 */;
    r3 = r0 & 0x1;
    if (/* eq */) goto .L_800E114C;
    /* srwi. r0, r4, 3 */;
    /* mtctr r0 */;
    if (/* eq */) goto .L_800E1138;
.L_800E10E8:
    r0 = *(u16*)((u8*)r5 + 0x0);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0x2);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0x4);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0x6);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0x8);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0xA);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0xC);
    r6 = r6 + r0;
    r0 = *(u16*)((u8*)r5 + 0xE);
    r5 = r5 + 0x10;
    r6 = r6 + r0;
    if (--ctr != 0) goto .L_800E10E8;
    r4 = r4 & 0x7;
    if (/* eq */) goto .L_800E114C;
.L_800E1138:
    /* mtctr r4 */;
.L_800E113C:
    r0 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    r6 = r6 + r0;
    if (--ctr != 0) goto .L_800E113C;
.L_800E114C:
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800E11C4;
    /* srwi. r0, r3, 3 */;
    /* mtctr r0 */;
    if (/* eq */) goto .L_800E11B0;
.L_800E1160:
    r0 = *(u8*)((u8*)r5 + 0x0);
    r6 = r6 + r0;
    r0 = *(u8*)((u8*)r5 + 0x1);
    r6 = r6 + r0;
    r0 = *(u8*)((u8*)r5 + 0x2);
    r6 = r6 + r0;
    r0 = *(u8*)((u8*)r5 + 0x3);
    r6 = r6 + r0;
    r0 = *(u8*)((u8*)r5 + 0x4);
    r6 = r6 + r0;
    r0 = *(u8*)((u8*)r5 + 0x5);
    r6 = r6 + r0;
    r0 = *(u8*)((u8*)r5 + 0x6);
    r6 = r6 + r0;
    r0 = *(u8*)((u8*)r5 + 0x7);
    r5 = r5 + 0x8;
    r6 = r6 + r0;
    if (--ctr != 0) goto .L_800E1160;
    r3 = r3 & 0x7;
    if (/* eq */) goto .L_800E11C4;
.L_800E11B0:
    /* mtctr r3 */;
.L_800E11B4:
    r0 = *(u8*)((u8*)r5 + 0x0);
    r5 = r5 + 0x1;
    r6 = r6 + r0;
    if (--ctr != 0) goto .L_800E11B4;
.L_800E11C4:
    r3 = *(u16*)((u8*)r30 + 0xE);
    r0 = r6 & 0xFFFF;
    /* cmplw r3, r0 */;
    if (/* eq */) goto .L_800E11E8;
    r4 = *(u16*)((u8*)r30 + 0x0);
    r3 = r31 + 0x1d8;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E11E8:
    /* subi r30, r30, 0x10 */;
.L_800E11EC:
    r4 = *(u32*)lbl_8047AB38;
    /* cmplw r30, r4 */;
    if (/* ge */) goto .L_800E0F64;
    r0 = r26 & 0xFF;
    if (/* eq */) goto .L_800E120C;
    r3 = r31 + 0x208;
    /* crclr cr1eq */;
    fn_800DD970();
.L_800E120C:
    r30 = *(u32*)lbl_8047AB68;
    goto .L_800E1368;
.L_800E1214:
    r24 = *(u32*)lbl_8047AB34;
    r0 = r24 + 0x10;
    r0 = r0 - r3;
    r0 = (u32)r0 >> 4;
    /* mtctr r0 */;
    /* cmplw r24, r3 */;
    if (/* lt */) goto .L_800E1254;
.L_800E1230:
    r0 = *(u16*)((u8*)r24 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E124C;
    r0 = *(u32*)((u8*)r24 + 0x4);
    /* cmplw r0, r30 */;
    if (/* ne */) goto .L_800E124C;
    goto .L_800E1258;
.L_800E124C:
    /* subi r24, r24, 0x10 */;
    if (--ctr != 0) goto .L_800E1230;
.L_800E1254:
    r24 = 0x0;
.L_800E1258:
    /* cmplwi r24, 0x0 */;
    if (/* eq */) goto .L_800E12A4;
    r7 = *(u16*)((u8*)r24 + 0x0);
    /* cmplwi r7, 0x0 */;
    if (/* eq */) goto .L_800E12A4;
    r0 = r26 & 0xFF;
    if (/* eq */) goto .L_800E1298;
    r6 = *(u32*)((u8*)r24 + 0x8);
    r4 = r30;
    r8 = *(u16*)((u8*)r24 + 0x2);
    r3 = r31 + 0x21c;
    r5 = r6 + r30;
    r9 = *(u16*)((u8*)r24 + 0xC);
    /* crclr cr1eq */;
    /* subi r5, r5, 0x1 */;
    fn_800DD970();
.L_800E1298:
    r0 = *(u32*)((u8*)r24 + 0x8);
    r30 = r30 + r0;
    goto .L_800E1368;
.L_800E12A4:
    r0 = r26 & 0xFF;
    if (/* eq */) goto .L_800E12D0;
    r6 = *(u32*)((u8*)r30 + 0x8);
    r4 = r30;
    r7 = *(u32*)((u8*)r30 + 0x0);
    r3 = r31 + 0x268;
    r5 = r6 + r30;
    r8 = *(u32*)((u8*)r30 + 0x4);
    /* crclr cr1eq */;
    /* subi r5, r5, 0x1 */;
    fn_800DD970();
.L_800E12D0:
    r3 = *(u32*)((u8*)r30 + 0x0);
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800E12F4;
    r0 = *(u32*)lbl_8047AB68;
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800E1318;
    r0 = *(u32*)lbl_8047AB64;
    /* cmplw r3, r0 */;
    if (/* gt */) goto .L_800E1318;
.L_800E12F4:
    r3 = *(u32*)((u8*)r30 + 0x4);
    /* cmplwi r3, 0x0 */;
    if (/* eq */) goto .L_800E1330;
    r0 = *(u32*)lbl_8047AB68;
    /* cmplw r3, r0 */;
    if (/* lt */) goto .L_800E1318;
    r0 = *(u32*)lbl_8047AB64;
    /* cmplw r3, r0 */;
    if (/* le */) goto .L_800E1330;
.L_800E1318:
    r4 = r30;
    r3 = r31 + 0x2a8;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
    goto .L_800E1374;
.L_800E1330:
    r5 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)lbl_8047AB38;
    r3 = r30 + r5;
    /* cmplw r3, r0 */;
    if (/* gt */) goto .L_800E134C;
    /* cmplwi r5, 0x0 */;
    if (/* ne */) goto .L_800E1364;
.L_800E134C:
    r4 = r30;
    r3 = r31 + 0x2e8;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
    goto .L_800E1374;
.L_800E1364:
    r30 = r30 + r5;
.L_800E1368:
    r3 = *(u32*)lbl_8047AB38;
    /* cmplw r30, r3 */;
    if (/* lt */) goto .L_800E1214;
.L_800E1374:
    r5 = *(u32*)lbl_8047AB38;
    /* cmplw r30, r5 */;
    if (/* eq */) goto .L_800E1394;
    r4 = r30;
    r3 = r31 + 0x318;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E1394:
    r0 = *(u32*)lbl_8047AB4C;
    /* cmplw r28, r0 */;
    if (/* eq */) goto .L_800E13B0;
    r3 = r31 + 0x36c;
    /* crclr cr1eq */;
    fn_800DD970();
    r29 = 0x0;
.L_800E13B0:
    r0 = r25 & 0xFF;
    if (/* eq */) goto .L_800E151C;
    r4 = *(u32*)lbl_8047AB68;
    r3 = r31 + 0x3a4;
    r5 = *(u32*)lbl_8047AB64;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB4C;
    r3 = r31 + 0x3cc;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB48;
    r3 = r31 + 0x3e8;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB38;
    r3 = r31 + 0x404;
    r0 = *(u32*)lbl_8047AB34;
    r4 = r0 - r4;
    r4 = r4 + 0x10;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = *(u32*)lbl_8047AB30;
    r4 = 0x0;
    goto .L_800E1428;
.L_800E1414:
    r0 = *(u32*)((u8*)r3 + 0x8);
    /* cmplw r0, r4 */;
    if (/* le */) goto .L_800E1424;
    r4 = r0;
.L_800E1424:
    r3 = *(u32*)((u8*)r3 + 0x4);
.L_800E1428:
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800E1414;
    r3 = r31 + 0x424;
    /* crclr cr1eq */;
    fn_800DD970();
    r3 = *(u32*)lbl_8047AB30;
    r4 = 0x0;
    goto .L_800E1454;
.L_800E1448:
    r0 = *(u32*)((u8*)r3 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x4);
    r4 = r4 + r0;
.L_800E1454:
    /* cmplwi r3, 0x0 */;
    if (/* ne */) goto .L_800E1448;
    r3 = r31 + 0x444;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = r27;
    r3 = r31 + 0x464;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB60;
    r3 = r31 + 0x480;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB5C;
    r3 = r31 + 0x49c;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB58;
    r3 = r31 + 0x4b8;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB54;
    r3 = r31 + 0x4d4;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB50;
    r3 = r31 + 0x4f0;
    /* crclr cr1eq */;
    fn_800DD970();
    r4 = (0x4330 << 16);
    /* subi r3, r27, 0x1 */;
    r0 = r27 + r28;
    f2 = *(f64*)lbl_8047CB50;
    r3 = r31 + 0x50c;
    f3 = *(f32*)lbl_8047CB48;
    f0 = *(f64*)((u8*)r1 + 0x8);
    f1 = f0 - f2;
    f0 = *(f64*)((u8*)r1 + 0x10);
    f0 = f0 - f2;
    f0 = f1 / f0;
    f1 = f3 * f0;
    /* crset cr1eq */;
    fn_800DD38C();
    r3 = r31 + 0x52c;
    /* crclr cr1eq */;
    fn_800DD970();
.L_800E151C:
    r0 = r29 & 0xFF;
    if (/* ne */) goto .L_800E152C;
    r0 = 0x0;
    *(u32*)lbl_8047AB3C = r0;
.L_800E152C:
    r3 = r29;
    /* lmw r24, 0x20(r1) */;
    r0 = *(u32*)((u8*)r1 + 0x44);
    return;
}
#pragma pop

/* fn_800E202C | Size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800E202C(void) {
    r0 = *(u8*)lbl_8047AB28;
    /* cmplwi r0, 0x1 */;
    if (/* ne */) goto .L_800E203C;
    /* subi r3, r3, 0x4 */;
.L_800E203C:
    r4 = *(u32*)lbl_8047AB34;
    r5 = *(u32*)lbl_8047AB38;
    r0 = r4 + 0x10;
    r0 = r0 - r5;
    r0 = (u32)r0 >> 4;
    /* mtctr r0 */;
    /* cmplw r4, r5 */;
    if (/* lt */) goto .L_800E2080;
.L_800E205C:
    r0 = *(u16*)((u8*)r4 + 0x0);
    /* cmplwi r0, 0x0 */;
    if (/* eq */) goto .L_800E2078;
    r0 = *(u32*)((u8*)r4 + 0x4);
    /* cmplw r0, r3 */;
    if (/* ne */) goto .L_800E2078;
    goto .L_800E2084;
.L_800E2078:
    /* subi r4, r4, 0x10 */;
    if (--ctr != 0) goto .L_800E205C;
.L_800E2080:
    r4 = 0x0;
.L_800E2084:
    /* cmplwi r4, 0x0 */;
    if (/* ne */) goto .L_800E2094;
    r3 = 0x0;
    return;
.L_800E2094:
    r3 = *(u16*)((u8*)r4 + 0x0);
    return;
}
#pragma pop
