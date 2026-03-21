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
extern u32 lbl_8047AA80;   /* GSgfx state pointer (sbss, sda21) */
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
void GSgfx_VBlankCallback(s32 mode, void* buffer) {
    if (mode == 0) {
        memcpy((void*)lbl_80400248, buffer, 0x5A0);
        *(u32*)lbl_8047AA80 = (u32)lbl_80400248;
    } else if (mode == 1) {
        memcpy(buffer, (const void*)lbl_80400248, 0x5A0);
        *(u32*)lbl_8047AA80 = (u32)buffer;
    }
}

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
void GSgfx_PreRetraceCallback(s32 flag, f32 p1, f32 p2, f32 p3, f32 p4, f32 p5, f32 p6) {
    extern u8 lbl_8047AA88[];
    extern void fn_800AA2F0();
    extern void fn_800BD640();
    extern void fn_800BD744();
    typedef void (*RetraceFunc)(s32, f32, f32, f32, f32, f32, f32);
    RetraceFunc callback;
    callback = (RetraceFunc)(*(u32*)lbl_8047AA88);
    if (callback != NULL) {
        callback(flag, p1, p2, p3, p4, p5, p6);
    } else if (flag != 0) {
        fn_800AA2F0();
        fn_800BD640(p1, p2, p3, p4, p5, p6);
    } else {
        fn_800BD744();
    }
}

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
void GSgfx_FrameEndCallback(void) {
    u8* state = (u8*)*(u32*)lbl_8047AA80;
    void (*callback)(void);

    *(u32*)(state + 0x4C) = *(u32*)(state + 0x4C) + 1;
    state = (u8*)*(u32*)lbl_8047AA80;
    callback = (void(*)(void))*(u32*)(state + 0x48);
    if (callback != 0) {
        callback();
    }
}

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
void GSgfx_BeginFrame(void) {
    extern u8 lbl_803147C8[];
    extern u8 lbl_804001F0[];
    extern u8 lbl_8047AA8C[];
    extern u8 lbl_8047CA20[];
    extern u8 lbl_8047CA24[];
    extern u8 lbl_8047CA28[];
    extern void fn_800D2584();
    extern void fn_800D461C();
    extern void fn_800D5CB8();
    extern void fn_800D65CC();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D87AC();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800D9B58();
    extern void fn_800D9FB4();
    extern void fn_800DA028();
    extern void fn_800DA100();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DD174();
    extern void fn_800E3604();
    extern void fn_80118104();
    extern void fn_80195A48();
    extern void fn_80195A6C();
    extern void fn_8019BD18();
    extern void fn_801B25C4();
    extern void fn_801E17A8();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r6 = (u32)lbl_804001F0;
    r30 = r3;
    r26 = r4;
    r27 = r5;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x28);
    r7 = *(u32*)((u8*)r8 + 0x490);
    r3 = *(u32*)((u8*)r8 + 0x494);
    r3 = r3 - r7;
    if ((u32)r3 <= (u32)r0) goto L_800D3FE8;
    *(u32*)((u8*)r6 + 0x28) = r3;
L_800D3FE8: ;
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
    if ((u32)r3 == (u32)r0) goto L_800D402C;
    fn_801E17A8();
L_800D402C: ;
    if ((u32)r3 == (u32)r0) goto L_800D40D4;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x10;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if ((u32)r3 == (u32)r0) goto L_800D40A8;
    fn_800D2584();
    if ((u32)r3 == (u32)r0) goto L_800D40A8;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    if ((s32)r3 == (s32)0x0) goto L_800D40A0;
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
L_800D40A0: ;
    r3 = -0x1;
    fn_800D87AC();
L_800D40A8: ;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto L_800D40B8;
L_800D40B4: ;
    fn_800D461C();
L_800D40B8: ;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    if ((u32)r3 < (u32)r0) goto L_800D40B4;
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x40) = r0;
L_800D40D4: ;
    if ((u32)r3 == (u32)r0) goto L_800D415C;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x1000;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if ((u32)r3 == (u32)r0) goto L_800D4150;
    fn_800D2584();
    if ((u32)r3 == (u32)r0) goto L_800D4150;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    if ((s32)r3 == (s32)0x0) goto L_800D4148;
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
L_800D4148: ;
    r3 = -0x1;
    fn_800D87AC();
L_800D4150: ;
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x44) = r0;
L_800D415C: ;
    if ((s32)r3 == (s32)0x0) goto L_800D4204;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x2000;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if ((s32)r3 == (s32)0x0) goto L_800D41D8;
    fn_800D2584();
    if ((s32)r3 == (s32)0x0) goto L_800D41D8;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    if ((s32)r3 == (s32)0x0) goto L_800D41D0;
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
L_800D41D0: ;
    r3 = -0x1;
    fn_800D87AC();
L_800D41D8: ;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto L_800D41E8;
L_800D41E4: ;
    fn_800D461C();
L_800D41E8: ;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    if ((u32)r3 < (u32)r0) goto L_800D41E4;
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x48) = r0;
L_800D4204: ;
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
    if ((u32)r3 == (u32)r0) goto L_800D43BC;
    fn_801E17A8();
L_800D43BC: ;
    if ((u32)r3 == (u32)r0) goto L_800D4464;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x10;
    r0 = r26 & 0xFF;
    r25 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if ((u32)r3 == (u32)r0) goto L_800D4438;
    fn_800D2584();
    if ((u32)r3 == (u32)r0) goto L_800D4438;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r24 + 0xC);
    fn_80195A6C();
    if ((s32)r3 == (s32)0x0) goto L_800D4430;
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
L_800D4430: ;
    r3 = -0x1;
    fn_800D87AC();
L_800D4438: ;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto L_800D4448;
L_800D4444: ;
    fn_800D461C();
L_800D4448: ;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    if ((u32)r3 < (u32)r0) goto L_800D4444;
    OSGetTick();
    r0 = r3 - r25;
    *(u32*)((u8*)r31 + 0x4C) = r0;
L_800D4464: ;
    if ((u32)r3 == (u32)r0) goto L_800D44EC;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x1000;
    r0 = r26 & 0xFF;
    r24 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if ((u32)r3 == (u32)r0) goto L_800D44E0;
    fn_800D2584();
    if ((u32)r3 == (u32)r0) goto L_800D44E0;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r25 + 0xC);
    fn_80195A6C();
    if ((s32)r3 == (s32)0x0) goto L_800D44D8;
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
L_800D44D8: ;
    r3 = -0x1;
    fn_800D87AC();
L_800D44E0: ;
    OSGetTick();
    r0 = r3 - r24;
    *(u32*)((u8*)r31 + 0x50) = r0;
L_800D44EC: ;
    if ((s32)r3 == (s32)0x0) goto L_800D4594;
    OSGetTick();
    r4 = *(u32*)lbl_8047AA80;
    r5 = 0x2000;
    r0 = r26 & 0xFF;
    r30 = r3;
    *(u32*)((u8*)r4 + 0x4) = r5;
    if ((s32)r3 == (s32)0x0) goto L_800D4568;
    fn_800D2584();
    if ((s32)r3 == (s32)0x0) goto L_800D4568;
    r3 = 0x7f;
    fn_801B25C4();
    r3 = *(u32*)((u8*)r26 + 0xC);
    fn_80195A6C();
    if ((s32)r3 == (s32)0x0) goto L_800D4560;
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
L_800D4560: ;
    r3 = -0x1;
    fn_800D87AC();
L_800D4568: ;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x490);
    goto L_800D4578;
L_800D4574: ;
    fn_800D461C();
L_800D4578: ;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x494);
    if ((u32)r3 < (u32)r0) goto L_800D4574;
    OSGetTick();
    r0 = r3 - r30;
    *(u32*)((u8*)r31 + 0x54) = r0;
L_800D4594: ;
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
    if ((u32)r3 == (u32)r0) goto L_800D45E4;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x490);
    *(u32*)((u8*)r3 + 0x494) = r0;
L_800D45E4: ;
    return;
}

/* ==================================================================
 * fn_800D461C -- GSlog_PrintFormatted
 *
 * Varargs printf-like function for the GS debug logging system.
 * At 0x97C (2428) bytes, this is a substantial printf implementation.
 * Uses "0123456789ABCDEF" for hex digit lookup.
 * ================================================================== */
void GSlog_PrintFormatted(u32 channel, u32 paramCount, ...) {
    extern void fn_800D30AC();
    extern void fn_800D55D0();
    extern void fn_800D5648();
    extern void fn_800D56C0();
    extern void fn_800D5724();
    extern void fn_800D579C();
    extern void fn_800D5814();
    extern void fn_800D58A0();
    extern void fn_800D592C();
    extern void fn_800D59B8();
    extern void fn_800D5A38();
    extern void fn_800D5AB0();
    extern void fn_800D5B28();
    extern void fn_800D5BA0();
    extern void fn_800D5C18();
    extern void fn_800D5CB8();
    extern void fn_800D5D6C();
    extern void fn_800D5DD0();
    extern void fn_800D5E34();
    extern void fn_800D5EB4();
    extern void fn_800D5F34();
    extern void fn_800D5FA4();
    extern void fn_800D6028();
    extern void fn_800D60AC();
    extern void fn_800D6148();
    extern void fn_800D61E4();
    extern void fn_800D6280();
    extern void fn_800D631C();
    extern void fn_800D63B0();
    extern void fn_800D6464();
    extern void fn_800D6518();
    extern void fn_800D65CC();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D76A8();
    extern void fn_800D7820();
    extern void fn_800D7C74();
    extern void fn_800D7D10();
    extern void fn_800D7D90();
    extern void fn_800D7E5C();
    extern void fn_800D7F14();
    extern void fn_800D7FE4();
    extern void fn_800D8088();
    extern void fn_800D8154();
    extern void fn_800D81EC();
    extern void fn_800D8284();
    extern void fn_800D834C();
    extern void fn_800D848C();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800D9B58();
    extern void fn_800D9BD0();
    extern void fn_800D9C24();
    extern void fn_800D9D68();
    extern void fn_800D9E4C();
    extern void fn_800D9ED8();
    extern void fn_800D9F40();
    extern void fn_800D9FB4();
    extern void fn_800DA028();
    extern void fn_800DA08C();
    extern void fn_800DA100();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA3B0();
    extern void fn_800DA428();
    extern void fn_800DA4C4();
    extern void fn_800DAD10();
    extern void fn_800DB900();
    extern void fn_800DB988();
    extern void fn_800DB9F0();
    extern void fn_800DBA54();
    extern void fn_800DBAA4();
    extern void fn_800DBB0C();
    extern void fn_800DBB84();
    extern void fn_800DBBFC();
    extern void fn_800DBCE4();
    extern void fn_800DBD70();
    extern void fn_800DBE5C();
    extern void fn_800DBEB4();
    extern void fn_800DBF1C();
    extern void fn_800DBF78();
    extern void fn_800DBFD4();
    extern void fn_800DC04C();
    extern void fn_800DC0D4();
    extern void fn_800DC14C();
    extern void fn_800DC1D4();
    extern void fn_800DC224();
    extern u8 jumptable_80314188[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r31 = r3 + 0x4;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 > (u32)0x5b) goto L_800D4F7C;
    r3 = (u32)jumptable_80314188;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80314188;
    /* lwzx r0, r3, r0 */;
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D6A00();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFFFF;
    fn_800D67BC();
    goto L_800D4F7C;
    fn_800D6728();
    goto L_800D4F7C;
    fn_800D30AC();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D6680();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = r3 & 0xFFFF;
    r4 = r4 & 0xFFFF;
    r31 = r31 + 0xc;
    r5 = r0 & 0xFFFF;
    fn_800D65CC();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s16)r3;
    r4 = (s16)r4;
    r31 = r31 + 0xc;
    r5 = (s16)r0;
    fn_800D6518();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = r3 & 0xFF;
    r4 = r4 & 0xFF;
    r31 = r31 + 0xc;
    r5 = r0 & 0xFF;
    fn_800D6464();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s8)r3;
    r4 = (s8)r4;
    r31 = r31 + 0xc;
    r5 = (s8)r0;
    fn_800D63B0();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800D631C();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = r3 & 0xFFFF;
    r4 = r0 & 0xFFFF;
    fn_800D6280();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = (s16)r3;
    r4 = (s16)r0;
    fn_800D61E4();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = r3 & 0xFF;
    r4 = r0 & 0xFF;
    fn_800D6148();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = (s8)r3;
    r4 = (s8)r0;
    fn_800D60AC();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFFFF;
    fn_800D6028();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800D5FA4();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D5F34();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s16)r3;
    r4 = (s16)r4;
    r31 = r31 + 0xc;
    r5 = (s16)r0;
    fn_800D5EB4();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = (s8)r3;
    r4 = (s8)r4;
    r31 = r31 + 0xc;
    r5 = (s8)r0;
    fn_800D5E34();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFFFF;
    fn_800D5DD0();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800D5D6C();
    goto L_800D4F7C;
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
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0xC);
    r4 = r3 & 0xFF;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = r5 & 0xFF;
    r6 = r0 & 0xFF;
    r31 = r31 + 0x10;
    fn_800D5C18();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800D5BA0();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D5B28();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D5AB0();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFF;
    fn_800D5A38();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    f1 = *(f32*)((u8*)r31 + 0x4);
    f2 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D59B8();
    goto L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = r4 & 0xFFFF;
    r5 = r0 & 0xFFFF;
    r31 = r31 + 0xc;
    fn_800D592C();
    goto L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = (s16)r4;
    r5 = (s16)r0;
    r31 = r31 + 0xc;
    fn_800D58A0();
    goto L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = r4 & 0xFF;
    r5 = r0 & 0xFF;
    r31 = r31 + 0xc;
    fn_800D5814();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r4 = (s8)r3;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = (s8)r0;
    r31 = r31 + 0xc;
    fn_800D58A0();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D579C();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFF;
    fn_800D5724();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800D56C0();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D5648();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D55D0();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800D85D4();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    r6 = r31;
    fn_800D848C();
    r31 = r31 + 0x30;
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D88DC();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D888C();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DAD10();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DA4C4();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DA428();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFF;
    fn_800DA3B0();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DA2BC();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DA1E8();
    goto L_800D4F7C;
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
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DA08C();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DA028();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9FB4();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9F40();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9ED8();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D9E4C();
    goto L_800D4F7C;
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
    goto L_800D4F7C;
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
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    f4 = *(f32*)((u8*)r31 + 0xC);
    r31 = r31 + 0x10;
    fn_800D9BD0();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    f4 = *(f32*)((u8*)r31 + 0xC);
    r31 = r31 + 0x10;
    fn_800D9B58();
    goto L_800D4F7C;
    fn_800D834C();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D8284();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D81EC();
    goto L_800D4F7C;
    f1 = *(f32*)((u8*)r31 + 0x0);
    f2 = *(f32*)((u8*)r31 + 0x4);
    f3 = *(f32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800D8154();
    goto L_800D4F7C;
    r3 = r31;
    fn_800D8088();
    r31 = r31 + 0x30;
    goto L_800D4F7C;
    r3 = r31;
    fn_800D7FE4();
    r31 = r31 + 0x30;
    goto L_800D4F7C;
    r3 = r31;
    fn_800D7F14();
    r31 = r31 + 0x30;
    goto L_800D4F7C;
    fn_800D7E5C();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r4 = r31;
    r3 = r0 & 0xFF;
    fn_800D7D90();
    r31 = r31 + 0x30;
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    r3 = r0 & 0xFF;
    fn_800D7D10();
    goto L_800D4F7C;
    fn_800D7C74();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800D7820();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x4);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x8;
    r4 = r0 & 0xFFFF;
    fn_800D76A8();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DC224();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800DC1D4();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r7 = r0 & 0xFF;
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r8 = *(u32*)((u8*)r31 + 0x14);
    r31 = r31 + 0x18;
    fn_800DC14C();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DC0D4();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r7 = r0 & 0xFF;
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r8 = *(u32*)((u8*)r31 + 0x14);
    r31 = r31 + 0x18;
    fn_800DC04C();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r6 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DBFD4();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800DBF78();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r31 = r31 + 0x8;
    fn_800DBF1C();
    goto L_800D4F7C;
    r30 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r4 = r31;
    r3 = (u32)sp + 0x14;
    r5 = 0x4;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)(sp + 0x14);
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r31 = r31 + 0x4;
    *(u32*)(sp + 0x10) = r0;
    fn_800DBEB4();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DBE5C();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r0 = r0 & 0xFF;
    *(u32*)(sp + 0x8) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)(sp + 0xC) = r0;
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
    goto L_800D4F7C;
    r4 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0xC);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = r4 & 0xFF;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r6 = r0 & 0xFF;
    r7 = *(u32*)((u8*)r31 + 0x10);
    r31 = r31 + 0x14;
    fn_800DBCE4();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    *(u32*)(sp + 0x8) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)(sp + 0xC) = r0;
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
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DBB84();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DBB0C();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    fn_800DBAA4();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r3 = r0 & 0xFF;
    fn_800DBA54();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DB9F0();
    goto L_800D4F7C;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u32*)((u8*)r31 + 0x8);
    r31 = r31 + 0xc;
    fn_800DB988();
    goto L_800D4F7C;
    r0 = *(u32*)((u8*)r31 + 0x64);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r31 = r31 + 0x4;
    r4 = r31;
    r5 = (s8)r0;
    r31 = r31 + 0x64;
    fn_800DB900();
L_800D4F7C: ;
    r3 = r31;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}

/* ==================================================================
 * fn_800D4F98 -- GSlog_QueueCommand
 *
 * Queue a rendering command via the GSlog debug/command system.
 * Used by lighting, material, and draw functions to batch commands.
 * 1388 bytes.
 * ================================================================== */
void GSlog_QueueCommand(u32 opcode, u32 paramCount, ...) {
    extern u8 jumptable_803142F8[];
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    if ((s32)r0 != (s32)0) goto L_800D4FD0;
    *(f64*)(sp + 0x28) = f1;
    *(f64*)(sp + 0x30) = f2;
    *(f64*)(sp + 0x38) = f3;
    *(f64*)(sp + 0x40) = f4;
    *(f64*)(sp + 0x48) = f5;
    *(f64*)(sp + 0x50) = f6;
    *(f64*)(sp + 0x58) = f7;
    *(f64*)(sp + 0x60) = f8;
L_800D4FD0: ;
    r30 = *(u32*)lbl_8047AA80;
    r11 = (u32)sp + 0x98;
    r0 = (u32)sp + 0x8;
    r12 = (0x200 << 16);
    r31 = (u32)sp + 0x70;
    r6 = *(u32*)((u8*)r30 + 0x494);
    r5 = r6 + 0x4;
    *(u32*)((u8*)r30 + 0x494) = r5;
    *(u32*)((u8*)r6 + 0x0) = r3;
    *(u32*)(sp + 0x78) = r0;
    if ((u32)r4 > (u32)0x14) goto L_800D54EC;
    r3 = (u32)jumptable_803142F8;
    r0 = r4 << 2;
    r3 = (u32)jumptable_803142F8;
    /* lwzx r0, r3, r0 */;
    ctr_fn = (void(*)(void))r0;
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
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    r3 = (u32)sp + 0x70;
    r4 = 0x1;
    __va_arg();
    r4 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    *(u32*)((u8*)r3 + 0x0) = r5;
    goto L_800D54EC;
    r3 = r31;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)(sp + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)(sp + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    r3 = (u32)sp + 0x70;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)(sp + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)(sp + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    r3 = (u32)sp + 0x70;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)(sp + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)(sp + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    r3 = (u32)sp + 0x70;
    r4 = 0x3;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)(sp + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)(sp + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    goto L_800D54EC;
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
    *(f32*)(sp + 0x6C) = f0;
    r0 = r5 + 0x4;
    *(u32*)((u8*)r6 + 0x494) = r0;
    r0 = *(u32*)(sp + 0x6C);
    *(u32*)((u8*)r5 + 0x0) = r0;
    __va_arg();
    f0 = *(f64*)((u8*)r3 + 0x0);
    r4 = *(u32*)lbl_8047AA80;
    f0 = (f32)f0;
    r3 = *(u32*)((u8*)r4 + 0x494);
    *(f32*)(sp + 0x6C) = f0;
    r0 = r3 + 0x4;
    *(u32*)((u8*)r4 + 0x494) = r0;
    r0 = *(u32*)(sp + 0x6C);
    *(u32*)((u8*)r3 + 0x0) = r0;
    goto L_800D54EC;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r6 = *(u32*)lbl_8047AA80;
    r5 = 0x30;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r3 = *(u32*)((u8*)r6 + 0x494);
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x30;
    *(u32*)((u8*)r4 + 0x494) = r0;
    goto L_800D54EC;
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
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x30;
    *(u32*)((u8*)r4 + 0x494) = r0;
    goto L_800D54EC;
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
    memcpy((void*)r3, (const void*)r4, (u32)r5);
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
    goto L_800D54EC;
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
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x494);
    r0 = r3 + 0x30;
    *(u32*)((u8*)r4 + 0x494) = r0;
    goto L_800D54EC;
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
L_800D54EC: ;
    r31 = *(u32*)(sp + 0x8C);
    r30 = *(u32*)(sp + 0x88);
    return;
}

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
void GSgfx_ConfigurePipeline(void) {
    extern u8 lbl_80314404[];
    extern u8 lbl_803144F0[];
    extern u8 lbl_80478AE0[];
    extern void fn_800D923C();
    extern void fn_800D963C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x10);
    if ((s32)r0 != (s32)0) goto L_800D8C48;
    r4 = *(u8*)((u8*)r3 + 0x94);
    r0 = 0x1;
    if ((u32)r4 == (u32)0x0) goto L_800D8964;
    r0 = 0x2;
    goto L_800D8978;
L_800D8964: ;
    if ((u32)r4 != (u32)0x0) goto L_800D8978;
    r4 = r5 & 0x1;
    if ((u32)r4 != (u32)0x0) goto L_800D8978;
    r0 = 0x0;
L_800D8978: ;
    *(u8*)((u8*)r6 + 0x60) = r0;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
    if ((s32)r0 <= (s32)0x0) goto L_800D8C48;
    r7 = *(u32*)lbl_8047AA80;
    r8 = *(u32*)((u8*)r7 + 0x10);
    if ((s32)r0 == (s32)0x0) goto L_800D8B1C;
    r5 = 0x7f1;
    r4 = 0x0;
    r9 = 0x10;
    r6 = 0x1;
    ctr_fn = (void(*)(void))r5;
L_800D89B8: ;
    r5 = *(u32*)((u8*)r7 + 0x10);
    if ((s32)r0 == (s32)0x0) goto L_800D89D0;
    r5 = r6 << r5;
    r4 = r4 | r5;
L_800D89D0: ;
    r9 = r9 + 0x1;
    if (--ctr != 0) goto L_800D89B8;
    r5 = r8 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_800D8A7C;
    r5 = 0x4;
    r18 = 0x0;
    if ((s32)r5 != (s32)0x4) goto L_800D89FC;
    r5 = r18;
    r18 = 0x1;
    goto L_800D8A0C;
L_800D89FC: ;
    if ((s32)r5 != (s32)0x5) goto L_800D8A0C;
    r5 = 0x1;
    r18 = r5;
L_800D8A0C: ;
    r5 = r5 * 0x6;
    r12 = 0x0;
    r7 = 0x2;
    r8 = r12;
    r9 = 0x1;
    goto L_800D8A60;
L_800D8A24: ;
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
    if ((s32)r5 == (s32)0x5) goto L_800D8A5C;
    r18 = 0x0;
    r5 = r5 + 0x6;
    goto L_800D8A60;
L_800D8A5C: ;
    r12 = 0x1;
L_800D8A60: ;
    r6 = r12 & 0xFF;
    if ((s32)r5 == (s32)0x5) goto L_800D8A24;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
    goto L_800D8BB4;
L_800D8A7C: ;
    r5 = 0x4;
    r19 = 0x0;
    if ((s32)r5 != (s32)0x4) goto L_800D8A98;
    r5 = r19;
    r19 = 0x1;
    goto L_800D8AA8;
L_800D8A98: ;
    if ((s32)r5 != (s32)0x5) goto L_800D8AA8;
    r5 = 0x1;
    r19 = r5;
L_800D8AA8: ;
    r5 = r5 * 0x6;
    r18 = 0x0;
    r7 = 0x2;
    r9 = r18;
    r8 = r18;
    r10 = 0x1;
    goto L_800D8B00;
L_800D8AC4: ;
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
    if ((s32)r5 == (s32)0x5) goto L_800D8AFC;
    r19 = 0x0;
    r5 = r5 + 0x6;
    goto L_800D8B00;
L_800D8AFC: ;
    r18 = 0x1;
L_800D8B00: ;
    r6 = r18 & 0xFF;
    if ((s32)r5 == (s32)0x5) goto L_800D8AC4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
    goto L_800D8BB4;
L_800D8B1C: ;
    r4 = 0x4;
    r12 = 0x0;
    if ((s32)r4 != (s32)0x4) goto L_800D8B38;
    r4 = r12;
    r12 = 0x1;
    goto L_800D8B48;
L_800D8B38: ;
    if ((s32)r4 != (s32)0x5) goto L_800D8B48;
    r4 = 0x1;
    r12 = r4;
L_800D8B48: ;
    r4 = r4 * 0x6;
    r11 = 0x0;
    r7 = 0x1;
    r8 = r11;
    r6 = 0x2;
    goto L_800D8B9C;
L_800D8B60: ;
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
    if ((s32)r4 == (s32)0x5) goto L_800D8B98;
    r12 = r8;
    r4 = r4 + 0x6;
    goto L_800D8B9C;
L_800D8B98: ;
    r11 = r7;
L_800D8B9C: ;
    r5 = r11 & 0xFF;
    if ((s32)r4 == (s32)0x5) goto L_800D8B60;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
L_800D8BB4: ;
    if ((s32)r0 != (s32)0x2) goto L_800D8C48;
    r4 = 0x5;
    r12 = 0x0;
    goto L_800D8BCC;
    goto L_800D8BDC;
L_800D8BCC: ;
    if ((s32)r4 != (s32)0x5) goto L_800D8BDC;
    r4 = 0x1;
    r12 = r4;
L_800D8BDC: ;
    r4 = r4 * 0x6;
    r11 = 0x0;
    r7 = 0x1;
    r8 = r11;
    r6 = 0x2;
    goto L_800D8C30;
L_800D8BF4: ;
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
    if ((s32)r4 == (s32)0x5) goto L_800D8C2C;
    r12 = r8;
    r4 = r4 + 0x6;
    goto L_800D8C30;
L_800D8C2C: ;
    r11 = r7;
L_800D8C30: ;
    r5 = r11 & 0xFF;
    if ((s32)r4 == (s32)0x5) goto L_800D8BF4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x414);
    r4 = r4 | 0x1;
    *(u32*)((u8*)r5 + 0x414) = r4;
L_800D8C48: ;
    r6 = *(u32*)lbl_8047AA80;
    r5 = *(u32*)((u8*)r6 + 0x10);
    if ((s32)r4 != (s32)0x5) goto L_800D8DC4;
    r3 = 0x0;
    *(u8*)((u8*)r6 + 0x79) = r3;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x2;
    *(u32*)((u8*)r3 + 0x414) = r0;
    if ((s32)r0 != (s32)0x1) goto L_800D8CCC;
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
    goto L_800D8DA4;
L_800D8CCC: ;
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
L_800D8DA4: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x3AC) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 | 0x4;
    *(u32*)((u8*)r3 + 0x414) = r0;
    goto L_800D9224;
L_800D8DC4: ;
    /* clrrwi. r0, r5, 31 */;
    if ((s32)r0 == (s32)0x1) goto L_800D90B4;
    r4 = (u32)lbl_80400B28;
    r5 = (u32)lbl_80400B28;
    r4 = *(u8*)((u8*)r5 + 0x34C);
    if ((u32)r4 == (u32)0x0) goto L_800D8DF4;
    r0 = r4 + 0x1;
    r4 = r0 & 0xFF;
    *(u8*)((u8*)r5 + 0x34C) = r0;
    *(u8*)((u8*)r5 + 0x34C) = r0;
L_800D8DF4: ;
    r0 = *(u8*)((u8*)r3 + 0xB0);
    r4 = 0x0;
    if ((u32)r0 != (u32)0x1) goto L_800D8E08;
    r4 = 0x0;
L_800D8E08: ;
    r0 = *(u8*)((u8*)r3 + 0xCC);
    if ((u32)r0 != (u32)0x1) goto L_800D8E18;
    r4 = 0x1;
L_800D8E18: ;
    r0 = *(u8*)((u8*)r3 + 0xE8);
    if ((u32)r0 != (u32)0x1) goto L_800D8E28;
    r4 = 0x2;
L_800D8E28: ;
    r0 = *(u8*)((u8*)r3 + 0x104);
    if ((u32)r0 != (u32)0x1) goto L_800D8E38;
    r4 = 0x3;
L_800D8E38: ;
    r0 = *(u8*)((u8*)r3 + 0x120);
    if ((u32)r0 != (u32)0x1) goto L_800D8E48;
    r4 = 0x4;
L_800D8E48: ;
    r0 = *(u8*)((u8*)r3 + 0x13C);
    if ((u32)r0 != (u32)0x1) goto L_800D8E58;
    r4 = 0x5;
L_800D8E58: ;
    r0 = *(u8*)((u8*)r3 + 0x158);
    if ((u32)r0 != (u32)0x1) goto L_800D8E68;
    r4 = 0x6;
L_800D8E68: ;
    r0 = *(u8*)((u8*)r3 + 0x174);
    if ((u32)r0 != (u32)0x1) goto L_800D8E78;
    r4 = 0x7;
L_800D8E78: ;
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
    goto L_800D904C;
L_800D8F04: ;
    r0 = *(u8*)((u8*)r28 + 0x1FC);
    r6 = r29 + 0x42e;
    r3 = *(u32*)lbl_8047AA80;
    r4 = 0x0;
    r6 = r3 + r6;
    if ((u32)r0 == (u32)0x0) goto L_800D8F2C;
    if ((s32)r20 <= (s32)0x0) goto L_800D8F2C;
    r4 = 0x1;
L_800D8F2C: ;
    r0 = r21 + 0x25c;
    r8 = (u32)lbl_80478AE0;
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
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)lbl_8047AA80;
    r3 = r25 + 0xfb;
    r4 = r26 + 0x9b;
    r5 = 0x5;
    r3 = r0 + r3;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)lbl_8047AA80;
    r3 = r29 + 0x14b;
    r4 = r24 + 0xeb;
    r5 = 0x4;
    r3 = r0 + r3;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)lbl_8047AA80;
    r3 = r29 + 0x18b;
    r4 = r24 + 0x12b;
    r5 = 0x4;
    r3 = r0 + r3;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r5 = *(u32*)((u8*)r24 + 0x16C);
    r3 = r29 + 0x1cc;
    r4 = *(u32*)lbl_8047AA80;
    r0 = r29 + 0x20c;
    /* stwx r5, r4, r3 */;
    r4 = *(u32*)((u8*)r24 + 0x1AC);
    r3 = *(u32*)lbl_8047AA80;
    /* stwx r4, r3, r0 */;
    if ((u32)r18 == (u32)0x0) goto L_800D9028;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r22 + 0x26c;
    r4 = r23 + 0x20c;
    r5 = 0x14;
    r3 = r0 + r3;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
L_800D9028: ;
    r29 = r29 + 0x4;
    r28 = r28 + 0x1;
    r27 = r27 + 0x3;
    r26 = r26 + 0x5;
    r25 = r25 + 0x5;
    r24 = r24 + 0x4;
    r23 = r23 + 0x14;
    r22 = r22 + 0x14;
    r21 = r21 + 0x1;
L_800D904C: ;
    if ((s32)r21 < (s32)r19) goto L_800D8F04;
    r6 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    r5 = 0x10;
    r3 = r6 + 0x24c;
    r4 = r4 + 0x1ec;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    if ((s32)r20 == (s32)0x0) goto L_800D9224;
    r6 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    r5 = r20 << 2;
    r3 = r6 + 0x3ad;
    r4 = r4 + 0x34d;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r6 = *(u32*)lbl_8047AA80;
    r3 = (u32)lbl_80400B28;
    r4 = (u32)lbl_80400B28;
    r5 = 0x54;
    r3 = r6 + 0x3c0;
    r4 = r4 + 0x360;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    goto L_800D9224;
L_800D90B4: ;
    r0 = *(u8*)((u8*)r3 + 0xB0);
    r21 = 0x0;
    r0 = *(u8*)((u8*)r3 + 0xCC);
    if ((u32)r0 != (u32)0x1) goto L_800D90D0;
    r21 = 0x1;
L_800D90D0: ;
    r0 = *(u8*)((u8*)r3 + 0xE8);
    if ((u32)r0 != (u32)0x1) goto L_800D90E0;
    r21 = 0x2;
L_800D90E0: ;
    r0 = *(u8*)((u8*)r3 + 0x104);
    if ((u32)r0 != (u32)0x1) goto L_800D90F0;
    r21 = 0x3;
L_800D90F0: ;
    r0 = *(u8*)((u8*)r3 + 0x120);
    if ((u32)r0 != (u32)0x1) goto L_800D9100;
    r21 = 0x4;
L_800D9100: ;
    r0 = *(u8*)((u8*)r3 + 0x13C);
    if ((u32)r0 != (u32)0x1) goto L_800D9110;
    r21 = 0x5;
L_800D9110: ;
    r0 = *(u8*)((u8*)r3 + 0x158);
    if ((u32)r0 != (u32)0x1) goto L_800D9120;
    r21 = 0x6;
L_800D9120: ;
    r0 = *(u8*)((u8*)r3 + 0x174);
    if ((u32)r0 != (u32)0x1) goto L_800D9130;
    r21 = 0x7;
L_800D9130: ;
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
    goto L_800D921C;
L_800D91AC: ;
    r0 = *(u32*)lbl_8047AA80;
    r8 = r22 + 0x42e;
    r7 = r23 + 0x7b;
    r5 = (u32)lbl_80478AE0;
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
L_800D921C: ;
    if ((s32)r24 <= (s32)r21) goto L_800D91AC;
L_800D9224: ;
    fn_800D923C();
    return;
}

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
void GSmaterial_Create(void* params) {
    extern u8 lbl_80400F30[];
    extern u8 lbl_80478AE8[];
    extern u8 lbl_8047CAB0[];
    extern u8 lbl_8047CAB4[];
    extern u8 lbl_8047CAB8[];
    extern u8 lbl_8047CABC[];
    extern u8 lbl_8047CAC0[];
    extern u8 jumptable_8031540C[];
    u8 sp[0x60];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    *(f64*)(sp + 0x50) = f31;
    /* psq_st f31, 0x58((u32)sp), 0, qr0 */;
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
    goto L_800DEF9C;
L_800DE6D0: ;
    r3 = r21 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_800DE71C;
    r4 = *(u8*)((u8*)r25 + 0x0);
    if ((s32)r4 != (s32)0x25) goto L_800DE710;
    r3 = *(u8*)((u8*)r25 + 0x1);
    if ((s32)r3 != (s32)0x25) goto L_800DE704;
    r3 = 0x25;
    r25 = r25 + 0x1;
    *(u8*)((u8*)r24 + 0x0) = r3;
    r24 = r24 + 0x1;
    goto L_800DEF7C;
L_800DE704: ;
    r23 = r28 + 0x278;
    r21 = 0x1;
    goto L_800DEF7C;
L_800DE710: ;
    *(u8*)((u8*)r24 + 0x0) = r4;
    r24 = r24 + 0x1;
    goto L_800DEF7C;
L_800DE71C: ;
    r5 = *(u8*)((u8*)r25 + 0x0);
    r3 = (s8)r5;
    if ((u32)r4 > (u32)0x20) goto L_800DED58;
    r3 = (u32)jumptable_8031540C;
    r4 = r4 << 2;
    r3 = (u32)jumptable_8031540C;
    /* lwzx r3, r3, r4 */;
    ctr_fn = (void(*)(void))r3;
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
    goto L_800DED60;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r0 = *(u32*)((u8*)r3 + 0x0);
    r3 = r28 + 0x268;
    r8 = 0x0;
    r9 = r0;
    if ((s32)r0 >= (s32)0x0) goto L_800DE79C;
    r8 = 0x1;
    r9 = -r0;
L_800DE79C: ;
    r4 = (0x6666 << 16);
    r7 = *(u32*)lbl_80478AE8;
    r6 = r4 + 0x6667;
L_800DE7A8: ;
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
    if ((s32)r0 != (s32)0x0) goto L_800DE7A8;
    r0 = r8 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800DE7F4;
    r0 = 0x2d;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
L_800DE7F4: ;
    r0 = 0x0;
    r18 = r28 + 0x268;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + 0x267;
    r4 = r4 + r28;
    goto L_800DE84C;
L_800DE814: ;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
L_800DE84C: ;
    if ((u32)r18 < (u32)r4) goto L_800DE814;
    r27 = r28 + 0x268;
    r0 = 0x1;
    goto L_800DED60;
    r3 = r31;
    r4 = 0x3;
    __va_arg();
    f1 = *(f64*)((u8*)r3 + 0x0);
    r27 = r28 + 0x268;
    f0 = *(f32*)lbl_8047CAB0;
    f1 = (f32)f1;
    f31 = f1;
    if (f1 >= f0) goto L_800DE898;
    r0 = 0x2d;
    f31 = -f1;
    *(u8*)((u8*)r27 + 0x0) = r0;
    r27 = r27 + 0x1;
L_800DE898: ;
    f0 = (f64)(s32)f31;
    r4 = r27;
    r0 = 0x0;
    *(f64*)(sp + 0x8) = f0;
    r18 = *(u32*)(sp + 0xC);
    r5 = r18;
    if ((s32)r18 >= (s32)0x0) goto L_800DE8C0;
    r0 = 0x1;
    r5 = -r18;
L_800DE8C0: ;
    r6 = (0x6666 << 16);
    r3 = *(u32*)lbl_80478AE8;
    r9 = r6 + 0x6667;
L_800DE8CC: ;
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
    if ((s32)r18 != (s32)0x0) goto L_800DE8CC;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800DE918;
    r0 = 0x2d;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r4 = r4 + 0x1;
L_800DE918: ;
    r0 = 0x0;
    r3 = r27;
    *(u8*)((u8*)r4 + 0x0) = r0;
    strlen();
    r4 = r3 + r27;
    goto L_800DE96C;
L_800DE934: ;
    r3 = *(u8*)((u8*)r27 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r27 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r27 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r27 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r27 + 0x0) = r0;
    r27 = r27 + 0x1;
L_800DE96C: ;
    if ((u32)r27 < (u32)r4) goto L_800DE934;
    /* xoris r3, r18, 0x8000 */;
    r0 = (0x4330 << 16);
    r4 = 0x0;
    f2 = *(f64*)lbl_8047CAC0;
    *(u32*)(sp + 0x8) = r0;
    f3 = *(f32*)lbl_8047CAB4;
    f1 = *(f64*)(sp + 0x8);
    f0 = *(f32*)lbl_8047CAB8;
    f1 = f1 - f2;
    f31 = f31 - f1;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x1;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x2;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x3;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x4;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x5;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x6;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x7;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x8;
    if (f3 < f31) goto L_800DEA3C;
    f3 = f3 / f0;
    r4 = 0x9;
    if (f3 < f31) goto L_800DEA3C;
    r4 = 0xa;
L_800DEA3C: ;
    r18 = r28 + 0x258;
    r3 = 0x30;
    if ((s32)r4 == (s32)0x0) goto L_800DEA98;
    /* srwi. r0, r4, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r4 == (s32)0x0) goto L_800DEA88;
L_800DEA58: ;
    *(u8*)((u8*)r18 + 0x0) = r3;
    *(u8*)((u8*)r18 + 0x1) = r3;
    *(u8*)((u8*)r18 + 0x2) = r3;
    *(u8*)((u8*)r18 + 0x3) = r3;
    *(u8*)((u8*)r18 + 0x4) = r3;
    *(u8*)((u8*)r18 + 0x5) = r3;
    *(u8*)((u8*)r18 + 0x6) = r3;
    *(u8*)((u8*)r18 + 0x7) = r3;
    r18 = r18 + 0x8;
    if (--ctr != 0) goto L_800DEA58;
    r4 = r4 & 0x7;
    if ((s32)r4 == (s32)0x0) goto L_800DEA98;
L_800DEA88: ;
    ctr_fn = (void(*)(void))r4;
L_800DEA8C: ;
    *(u8*)((u8*)r18 + 0x0) = r3;
    r18 = r18 + 0x1;
    if (--ctr != 0) goto L_800DEA8C;
L_800DEA98: ;
    f0 = *(f32*)lbl_8047CABC;
    r3 = r18;
    r8 = 0x0;
    f31 = f31 * f0;
    f0 = (f64)(s32)f31;
    *(f64*)(sp + 0x8) = f0;
    r9 = *(u32*)(sp + 0xC);
    if ((s32)r9 >= (s32)0x0) goto L_800DEAC4;
    r8 = 0x1;
    r9 = -r9;
L_800DEAC4: ;
    r4 = (0x6666 << 16);
    r7 = *(u32*)lbl_80478AE8;
    r6 = r4 + 0x6667;
L_800DEAD0: ;
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
    if ((s32)r9 != (s32)0x0) goto L_800DEAD0;
    r0 = r8 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800DEB1C;
    r0 = 0x2d;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
L_800DEB1C: ;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + r18;
    goto L_800DEB70;
L_800DEB38: ;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
L_800DEB70: ;
    if ((u32)r18 < (u32)r4) goto L_800DEB38;
    r3 = r28 + 0x258;
    strlen();
    r5 = r28 + 0x258;
    r5 = r5 + r3;
    r3 = 0xa - r3;
    r4 = 0x30;
    if ((s32)r3 >= (s32)0xa) goto L_800DEBE4;
    /* srwi. r0, r3, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r3 == (s32)0xa) goto L_800DEBD4;
L_800DEBA4: ;
    *(u8*)((u8*)r5 + 0x0) = r4;
    *(u8*)((u8*)r5 + 0x1) = r4;
    *(u8*)((u8*)r5 + 0x2) = r4;
    *(u8*)((u8*)r5 + 0x3) = r4;
    *(u8*)((u8*)r5 + 0x4) = r4;
    *(u8*)((u8*)r5 + 0x5) = r4;
    *(u8*)((u8*)r5 + 0x6) = r4;
    *(u8*)((u8*)r5 + 0x7) = r4;
    r5 = r5 + 0x8;
    if (--ctr != 0) goto L_800DEBA4;
    r3 = r3 & 0x7;
    if ((s32)r3 == (s32)0xa) goto L_800DEBE4;
L_800DEBD4: ;
    ctr_fn = (void(*)(void))r3;
L_800DEBD8: ;
    *(u8*)((u8*)r5 + 0x0) = r4;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto L_800DEBD8;
L_800DEBE4: ;
    r0 = 0x0;
    r27 = r28 + 0x268;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r0 = 0x1;
    goto L_800DED60;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r27 = *(u32*)((u8*)r3 + 0x0);
    r0 = 0x1;
    goto L_800DED60;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r0 = *(u8*)((u8*)r25 + 0x0);
    r5 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x58) goto L_800DECB0;
    r3 = *(u32*)lbl_80478AE8;
    r4 = r28 + 0x268;
L_800DEC34: ;
    r0 = r5 & 0xF;
    /* srwi. r5, r5, 4 */;
    /* lbzx r0, r3, r0 */;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r4 = r4 + 0x1;
    if ((s32)r0 != (s32)0x58) goto L_800DEC34;
    r0 = 0x0;
    r18 = r28 + 0x268;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + 0x267;
    r4 = r4 + r28;
    goto L_800DECA4;
L_800DEC6C: ;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
L_800DECA4: ;
    if ((u32)r18 < (u32)r4) goto L_800DEC6C;
    goto L_800DED4C;
L_800DECB0: ;
    r4 = *(u32*)lbl_80478AE8;
    r6 = r5;
    r5 = r28 + 0x268;
L_800DECBC: ;
    r0 = r6 & 0xF;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = *(u8*)((u8*)r5 + 0x0);
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0x41) goto L_800DECE0;
    r0 = r3 + 0x20;
    *(u8*)((u8*)r5 + 0x0) = r0;
L_800DECE0: ;
    /* srwi. r6, r6, 4 */;
    r5 = r5 + 0x1;
    if ((s32)r0 != (s32)0x41) goto L_800DECBC;
    r0 = 0x0;
    r18 = r28 + 0x268;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = r18;
    strlen();
    r4 = r3 + 0x267;
    r4 = r4 + r28;
    goto L_800DED44;
L_800DED0C: ;
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r18 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r18 + 0x0) = r0;
    r18 = r18 + 0x1;
L_800DED44: ;
    if ((u32)r18 < (u32)r4) goto L_800DED0C;
L_800DED4C: ;
    r27 = r28 + 0x268;
    r0 = 0x1;
    goto L_800DED60;
L_800DED58: ;
    *(u8*)((u8*)r23 + 0x0) = r5;
    r23 = r23 + 0x1;
L_800DED60: ;
    r3 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x1) goto L_800DEF7C;
    r0 = 0x0;
    *(u8*)((u8*)r23 + 0x0) = r0;
    r23 = r28 + 0x278;
    r0 = *(u8*)((u8*)r28 + 0x278);
    if ((s32)r0 != (s32)0x2d) goto L_800DED8C;
    r20 = 0x1;
    r23 = r23 + 0x1;
L_800DED8C: ;
    r0 = *(u8*)((u8*)r23 + 0x0);
    if ((s32)r0 != (s32)0x30) goto L_800DEDA0;
    r19 = 0x1;
    r23 = r23 + 0x1;
L_800DEDA0: ;
    r3 = r23;
    r18 = 0x0;
    goto L_800DEDD8;
L_800DEDAC: ;
    r0 = (s8)r4;
    if ((s32)r0 < (s32)0x30) goto L_800DEDE4;
    if ((s32)r0 > (s32)0x39) goto L_800DEDE4;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r18 = r18 * 0xa;
    r3 = r3 + 0x1;
    r0 = (s8)r0;
    r18 = r0 + r18;
L_800DEDD8: ;
    r4 = *(u8*)((u8*)r3 + 0x0);
    r0 = (s8)r4;
    if ((s32)r0 != (s32)0x39) goto L_800DEDAC;
L_800DEDE4: ;
    r3 = r27;
    strlen();
    if ((s32)r18 <= (s32)r3) goto L_800DEE58;
    r0 = r20 & 0xFF;
    r18 = r18 - r3;
    if ((s32)r18 != (s32)r3) goto L_800DEE58;
    r5 = r19 & 0xFF;
    r4 = 0x30;
    r3 = 0x20;
    goto L_800DEE2C;
L_800DEE10: ;
    if ((u32)r5 == (u32)0x0) goto L_800DEE24;
    *(u8*)((u8*)r24 + 0x0) = r4;
    r24 = r24 + 0x1;
    goto L_800DEE2C;
L_800DEE24: ;
    *(u8*)((u8*)r24 + 0x0) = r3;
    r24 = r24 + 0x1;
L_800DEE2C: ;
    if ((s32)r18 == (s32)0x0) goto L_800DEE58;
    r0 = r24 - r29;
    if ((u32)r0 < (u32)r30) goto L_800DEE10;
    goto L_800DEE58;
L_800DEE48: ;
    r0 = *(u8*)((u8*)r27 + 0x0);
    r27 = r27 + 0x1;
    *(u8*)((u8*)r24 + 0x0) = r0;
    r24 = r24 + 0x1;
L_800DEE58: ;
    r0 = *(u8*)((u8*)r27 + 0x0);
    r0 = (s8)r0;
    if ((u32)r0 == (u32)r30) goto L_800DEE70;
    r0 = r24 - r29;
    if ((u32)r0 < (u32)r30) goto L_800DEE48;
L_800DEE70: ;
    r0 = *(u8*)((u8*)r25 + 0x0);
    if ((s32)r0 != (s32)0x66) goto L_800DEF38;
    r27 = r28 + 0x258;
    r3 = r27;
    strlen();
    r19 = r3;
    r3 = r23;
    r4 = 0x2e;
    strchr();
    if ((u32)r3 == (u32)0x0) goto L_800DEF00;
    r4 = r3 + 0x1;
    r3 = 0x0;
    goto L_800DEED8;
L_800DEEAC: ;
    r0 = (s8)r5;
    if ((s32)r0 < (s32)0x30) goto L_800DEEE4;
    if ((s32)r0 > (s32)0x39) goto L_800DEEE4;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = r3 * 0xa;
    r4 = r4 + 0x1;
    r0 = (s8)r0;
    r3 = r0 + r3;
L_800DEED8: ;
    r5 = *(u8*)((u8*)r4 + 0x0);
    r0 = (s8)r5;
    if ((s32)r0 != (s32)0x39) goto L_800DEEAC;
L_800DEEE4: ;
    r18 = r3;
    if ((s32)r3 <= (s32)0x0) goto L_800DEF00;
    if ((s32)r19 <= (s32)r3) goto L_800DEF00;
    r0 = 0x0;
    /* stbx r0, r27, r3 */;
L_800DEF00: ;
    r0 = 0x2e;
    *(u8*)((u8*)r24 + 0x0) = r0;
    r24 = r24 + 0x1;
    goto L_800DEF20;
L_800DEF10: ;
    r0 = *(u8*)((u8*)r27 + 0x0);
    r27 = r27 + 0x1;
    *(u8*)((u8*)r24 + 0x0) = r0;
    r24 = r24 + 0x1;
L_800DEF20: ;
    r0 = *(u8*)((u8*)r27 + 0x0);
    r0 = (s8)r0;
    if ((s32)r19 == (s32)r3) goto L_800DEF38;
    r0 = r24 - r29;
    if ((u32)r0 < (u32)r30) goto L_800DEF10;
L_800DEF38: ;
    r0 = r20 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800DEF6C;
    r3 = 0x20;
    goto L_800DEF54;
L_800DEF4C: ;
    *(u8*)((u8*)r24 + 0x0) = r3;
    r24 = r24 + 0x1;
L_800DEF54: ;
    if ((s32)r18 == (s32)0x0) goto L_800DEF6C;
    r0 = r24 - r29;
    if ((u32)r0 < (u32)r30) goto L_800DEF4C;
L_800DEF6C: ;
    r20 = 0x0;
    r19 = 0x0;
    r21 = 0x0;
    r0 = 0x0;
L_800DEF7C: ;
    r3 = *(u8*)((u8*)r25 + 0x0);
    r3 = (s8)r3;
    if ((u32)r0 == (u32)r30) goto L_800DEF94;
    r3 = r24 - r29;
    if ((u32)r3 < (u32)r30) goto L_800DEF98;
L_800DEF94: ;
    r22 = 0x1;
L_800DEF98: ;
    r25 = r25 + 0x1;
L_800DEF9C: ;
    r3 = r22 & 0xFF;
    if ((u32)r3 == (u32)r30) goto L_800DE6D0;
    r0 = 0x0;
    *(u8*)((u8*)r24 + 0x0) = r0;
    /* psq_l f31, 0x58((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x50);
    return;
}

/* ==================================================================
 * fn_800E1544 -- GSgfx_DrawDispatch
 *
 * Main draw command dispatch function. At 2792 bytes, this is the
 * largest function in the rendering pipeline. It interprets draw
 * commands and submits vertices/primitives to GX.
 * ================================================================== */
void GSgfx_DrawDispatch(void* drawList) {
    extern u8 lbl_80270BB8[];
    extern u8 lbl_8047AB28[];
    extern u8 lbl_8047AB30[];
    extern u8 lbl_8047AB34[];
    extern u8 lbl_8047AB38[];
    extern void fn_800DD970();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = 0x0;
    r5 = *(u32*)lbl_8047AB30;
    r3 = r5;
    goto L_800E1578;
L_800E1564: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 <= (u32)r4) goto L_800E1574;
    r4 = r0;
L_800E1574: ;
    r3 = *(u32*)((u8*)r3 + 0x4);
L_800E1578: ;
    if ((u32)r3 != (u32)0x0) goto L_800E1564;
    r28 = r5;
    if ((u32)r5 == (u32)0x0) goto L_800E1FEC;
    r0 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r0 == (u32)0x0) goto L_800E1FEC;
    goto L_800E1FE4;
L_800E159C: ;
    r7 = *(u32*)((u8*)r28 + 0x8);
    r6 = *(u32*)lbl_8047AB38;
    r8 = r28 + r7;
    if ((u32)r8 == (u32)r6) goto L_800E1FE0;
    r5 = *(u32*)lbl_8047AB34;
    r0 = r5 + 0x10;
    r3 = r5;
    r0 = r0 - r6;
    r0 = (u32)r0 >> 4;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r5 < (u32)r6) goto L_800E15F4;
L_800E15D0: ;
    r0 = *(u16*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E15EC;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r8) goto L_800E15EC;
    goto L_800E15F8;
L_800E15EC: ;
    if (--ctr != 0) goto L_800E15D0;
L_800E15F4: ;
    r3 = 0x0;
L_800E15F8: ;
    if ((u32)r3 != (u32)0x0) goto L_800E1618;
    r3 = (u32)lbl_80270BB8;
    fn_800DD970();
    r3 = 0x0;
    goto L_800E2018;
L_800E1618: ;
    r0 = *(u16*)((u8*)r3 + 0x2);
    if ((u32)r0 != (u32)0x0) goto L_800E1898;
    r0 = *(u16*)((u8*)r3 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_800E1898;
    r5 = *(u32*)lbl_8047AB30;
    r0 = 0x0;
    if ((u32)r5 != (u32)r28) goto L_800E1644;
    r0 = 0x1;
L_800E1644: ;
    r6 = *(u32*)((u8*)r28 + 0x0);
    r5 = *(u32*)((u8*)r3 + 0x8);
    r12 = *(u32*)((u8*)r28 + 0x4);
    r5 = r28 + r5;
    if ((u32)r6 == (u32)0x0) goto L_800E1660;
    *(u32*)((u8*)r6 + 0x4) = r5;
L_800E1660: ;
    r8 = *(u32*)((u8*)r28 + 0x4);
    if ((u32)r8 == (u32)0x0) goto L_800E1670;
    *(u32*)((u8*)r8 + 0x0) = r5;
L_800E1670: ;
    r22 = *(u32*)((u8*)r3 + 0x4);
    r10 = *(u32*)((u8*)r3 + 0x8);
    /* subf. r9, r28, r22 */;
    r8 = -r9;
    if ((u32)r8 == (u32)0x0) goto L_800E1688;
    r8 = r9;
L_800E1688: ;
    if ((u32)r8 < (u32)0x4) goto L_800E17A4;
    /* srwi. r8, r10, 2 */;
    r11 = r28;
    r10 = r10 & 0x3;
    r9 = r8;
    if ((u32)r8 == (u32)0x4) goto L_800E171C;
    /* srwi. r8, r8, 3 */;
    ctr_fn = (void(*)(void))r8;
    if ((u32)r8 == (u32)0x4) goto L_800E1704;
L_800E16B0: ;
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
    if (--ctr != 0) goto L_800E16B0;
    r9 = r9 & 0x7;
    if ((u32)r8 == (u32)0x4) goto L_800E171C;
L_800E1704: ;
    ctr_fn = (void(*)(void))r9;
L_800E1708: ;
    r8 = *(u32*)((u8*)r22 + 0x0);
    r22 = r22 + 0x4;
    *(u32*)((u8*)r11 + 0x0) = r8;
    r11 = r11 + 0x4;
    if (--ctr != 0) goto L_800E1708;
L_800E171C: ;
    r9 = r10;
    if ((u32)r10 == (u32)0x0) goto L_800E182C;
    /* srwi. r8, r10, 3 */;
    ctr_fn = (void(*)(void))r8;
    if ((u32)r10 == (u32)0x0) goto L_800E1788;
L_800E1734: ;
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
    if (--ctr != 0) goto L_800E1734;
    r9 = r9 & 0x7;
    if ((u32)r10 == (u32)0x0) goto L_800E182C;
L_800E1788: ;
    ctr_fn = (void(*)(void))r9;
L_800E178C: ;
    r8 = *(u8*)((u8*)r22 + 0x0);
    r22 = r22 + 0x1;
    *(u8*)((u8*)r11 + 0x0) = r8;
    r11 = r11 + 0x1;
    if (--ctr != 0) goto L_800E178C;
    goto L_800E182C;
L_800E17A4: ;
    r11 = r28;
    r9 = r10;
    if ((u32)r10 == (u32)0x0) goto L_800E182C;
    /* srwi. r8, r10, 3 */;
    ctr_fn = (void(*)(void))r8;
    if ((u32)r10 == (u32)0x0) goto L_800E1814;
L_800E17C0: ;
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
    if (--ctr != 0) goto L_800E17C0;
    r9 = r9 & 0x7;
    if ((u32)r10 == (u32)0x0) goto L_800E182C;
L_800E1814: ;
    ctr_fn = (void(*)(void))r9;
L_800E1818: ;
    r8 = *(u8*)((u8*)r22 + 0x0);
    r22 = r22 + 0x1;
    *(u8*)((u8*)r11 + 0x0) = r8;
    r11 = r11 + 0x1;
    if (--ctr != 0) goto L_800E1818;
L_800E182C: ;
    *(u32*)((u8*)r3 + 0x4) = r28;
    *(u32*)((u8*)r5 + 0x0) = r6;
    *(u32*)((u8*)r5 + 0x4) = r12;
    *(u32*)((u8*)r5 + 0x8) = r7;
    r7 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r7 == (u32)0x0) goto L_800E1884;
    r6 = *(u32*)((u8*)r5 + 0x8);
    r3 = r5 + r6;
    if ((u32)r7 != (u32)r3) goto L_800E1884;
    r3 = *(u32*)((u8*)r7 + 0x8);
    r3 = r6 + r3;
    *(u32*)((u8*)r5 + 0x8) = r3;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r3 == (u32)0x0) goto L_800E1878;
    *(u32*)((u8*)r3 + 0x0) = r5;
L_800E1878: ;
    r3 = *(u32*)((u8*)r5 + 0x4);
    r3 = *(u32*)((u8*)r3 + 0x4);
    *(u32*)((u8*)r5 + 0x4) = r3;
L_800E1884: ;
    r0 = r0 & 0xFF;
    if ((u32)r3 == (u32)0x0) goto L_800E1890;
    *(u32*)lbl_8047AB30 = r5;
L_800E1890: ;
    r28 = *(u32*)lbl_8047AB30;
    goto L_800E1FE4;
L_800E1898: ;
    r3 = 0x0;
    r11 = r5;
    r8 = (u32)sp + 0x8;
    r29 = 0x0;
    r10 = 0x0;
    goto L_800E1A50;
L_800E18C0: ;
    r0 = *(u16*)((u8*)r11 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E1A4C;
    r0 = *(u16*)((u8*)r11 + 0x2);
    if ((u32)r0 != (u32)0x0) goto L_800E1A4C;
    r5 = *(u32*)((u8*)r11 + 0x8);
    r7 = *(u32*)((u8*)r28 + 0x8);
    if ((u32)r5 > (u32)r7) goto L_800E1A4C;
    r0 = *(u32*)((u8*)r11 + 0x4);
    if ((u32)r0 <= (u32)r28) goto L_800E1A4C;
    r0 = *(u16*)((u8*)r11 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_800E1A4C;
    if ((s32)r29 >= (s32)0x4) goto L_800E1930;
    r0 = r10 + r5;
    if ((u32)r0 > (u32)r7) goto L_800E1930;
    r5 = (u32)sp + 0x8;
    r29 = r29 + 0x1;
    /* stwx r11, r5, r3 */;
    r3 = r3 + 0x4;
    r0 = *(u32*)((u8*)r11 + 0x8);
    r10 = r10 + r0;
    goto L_800E1A4C;
L_800E1930: ;
    r7 = (u32)sp + 0x8;
    r31 = 0x0;
    goto L_800E1A44;
L_800E193C: ;
    r0 = r31 << 2;
    /* lwzx r12, r7, r0 */;
    r9 = 0x0;
    r30 = 0x0;
    /* stwx r11, r7, r0 */;
    if ((s32)r29 <= (s32)0x0) goto L_800E1A1C;
    if ((s32)r29 <= (s32)0x8) goto L_800E19E8;
    r27 = r22 + 0x7;
    r5 = r8;
    r27 = (u32)r27 >> 3;
    ctr_fn = (void(*)(void))r27;
    if ((s32)r22 <= (s32)0x0) goto L_800E19E8;
L_800E197C: ;
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
    if (--ctr != 0) goto L_800E197C;
L_800E19E8: ;
    r27 = r30 << 2;
    r22 = (u32)sp + 0x8;
    r5 = r29 - r30;
    r22 = r22 + r27;
    ctr_fn = (void(*)(void))r5;
    if ((s32)r30 >= (s32)r29) goto L_800E1A1C;
L_800E1A04: ;
    r5 = *(u32*)((u8*)r22 + 0x0);
    r22 = r22 + 0x4;
    r30 = r30 + 0x1;
    r5 = *(u32*)((u8*)r5 + 0x8);
    r9 = r9 + r5;
    if (--ctr != 0) goto L_800E1A04;
L_800E1A1C: ;
    if ((u32)r9 <= (u32)r10) goto L_800E1A3C;
    r5 = *(u32*)((u8*)r28 + 0x8);
    if ((u32)r9 > (u32)r5) goto L_800E1A3C;
    r10 = r9;
    r31 = r29;
    goto L_800E1A40;
L_800E1A3C: ;
    /* stwx r12, r7, r0 */;
L_800E1A40: ;
    r31 = r31 + 0x1;
L_800E1A44: ;
    if ((s32)r31 < (s32)r29) goto L_800E193C;
L_800E1A4C: ;
L_800E1A50: ;
    if ((u32)r11 >= (u32)r6) goto L_800E18C0;
    if ((s32)r29 <= (s32)0x0) goto L_800E1FE0;
    r0 = *(u32*)lbl_8047AB30;
    r6 = *(u32*)((u8*)r28 + 0x0);
    r7 = *(u32*)((u8*)r28 + 0x4);
    r0 = *(u32*)((u8*)r28 + 0x8);
    if ((u32)r28 != (u32)r0) goto L_800E1A7C;
    *(u32*)lbl_8047AB30 = r7;
L_800E1A7C: ;
    if ((u32)r6 == (u32)0x0) goto L_800E1A88;
    *(u32*)((u8*)r6 + 0x4) = r7;
L_800E1A88: ;
    if ((u32)r7 == (u32)0x0) goto L_800E1A94;
    *(u32*)((u8*)r7 + 0x0) = r6;
L_800E1A94: ;
    r3 = r28;
    r5 = 0x0;
    goto L_800E1D94;
L_800E1AA0: ;
    r10 = *(u32*)((u8*)r8 + 0x0);
    r9 = *(u32*)((u8*)r10 + 0x4);
    r12 = *(u32*)((u8*)r10 + 0x8);
    /* subf. r11, r3, r9 */;
    r10 = -r11;
    if ((u32)r7 == (u32)0x0) goto L_800E1ABC;
    r10 = r11;
L_800E1ABC: ;
    if ((u32)r10 < (u32)0x4) goto L_800E1BDC;
    /* srwi. r10, r12, 2 */;
    r23 = r9;
    r22 = r3;
    r12 = r12 & 0x3;
    r11 = r10;
    if ((u32)r10 == (u32)0x4) goto L_800E1B54;
    /* srwi. r10, r10, 3 */;
    ctr_fn = (void(*)(void))r10;
    if ((u32)r10 == (u32)0x4) goto L_800E1B3C;
L_800E1AE8: ;
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
    if (--ctr != 0) goto L_800E1AE8;
    r11 = r11 & 0x7;
    if ((u32)r10 == (u32)0x4) goto L_800E1B54;
L_800E1B3C: ;
    ctr_fn = (void(*)(void))r11;
L_800E1B40: ;
    r10 = *(u32*)((u8*)r23 + 0x0);
    r23 = r23 + 0x4;
    *(u32*)((u8*)r22 + 0x0) = r10;
    r22 = r22 + 0x4;
    if (--ctr != 0) goto L_800E1B40;
L_800E1B54: ;
    r11 = r12;
    if ((u32)r12 == (u32)0x0) goto L_800E1C68;
    /* srwi. r10, r12, 3 */;
    ctr_fn = (void(*)(void))r10;
    if ((u32)r12 == (u32)0x0) goto L_800E1BC0;
L_800E1B6C: ;
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
    if (--ctr != 0) goto L_800E1B6C;
    r11 = r11 & 0x7;
    if ((u32)r12 == (u32)0x0) goto L_800E1C68;
L_800E1BC0: ;
    ctr_fn = (void(*)(void))r11;
L_800E1BC4: ;
    r10 = *(u8*)((u8*)r23 + 0x0);
    r23 = r23 + 0x1;
    *(u8*)((u8*)r22 + 0x0) = r10;
    r22 = r22 + 0x1;
    if (--ctr != 0) goto L_800E1BC4;
    goto L_800E1C68;
L_800E1BDC: ;
    r22 = r9;
    r23 = r3;
    r11 = r12;
    if ((u32)r12 == (u32)0x0) goto L_800E1C68;
    /* srwi. r10, r12, 3 */;
    ctr_fn = (void(*)(void))r10;
    if ((u32)r12 == (u32)0x0) goto L_800E1C50;
L_800E1BFC: ;
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
    if (--ctr != 0) goto L_800E1BFC;
    r11 = r11 & 0x7;
    if ((u32)r12 == (u32)0x0) goto L_800E1C68;
L_800E1C50: ;
    ctr_fn = (void(*)(void))r11;
L_800E1C54: ;
    r10 = *(u8*)((u8*)r22 + 0x0);
    r22 = r22 + 0x1;
    *(u8*)((u8*)r23 + 0x0) = r10;
    r23 = r23 + 0x1;
    if (--ctr != 0) goto L_800E1C54;
L_800E1C68: ;
    r10 = *(u32*)((u8*)r8 + 0x0);
    r11 = 0x0;
    *(u32*)((u8*)r10 + 0x4) = r3;
    r10 = *(u32*)((u8*)r8 + 0x0);
    r12 = *(u32*)lbl_8047AB30;
    r10 = *(u32*)((u8*)r10 + 0x8);
    r3 = r3 + r10;
    r0 = r0 - r10;
    goto L_800E1C94;
L_800E1C8C: ;
    r11 = r12;
    r12 = *(u32*)((u8*)r12 + 0x4);
L_800E1C94: ;
    if ((u32)r12 == (u32)0x0) goto L_800E1CA4;
    if ((u32)r12 < (u32)r9) goto L_800E1C8C;
L_800E1CA4: ;
    if ((u32)r11 == (u32)0x0) goto L_800E1CBC;
    *(u32*)((u8*)r9 + 0x0) = r11;
    r10 = *(u32*)((u8*)r11 + 0x4);
    *(u32*)((u8*)r9 + 0x4) = r10;
    goto L_800E1CD0;
L_800E1CBC: ;
    r10 = 0x0;
    *(u32*)((u8*)r9 + 0x0) = r10;
    r10 = *(u32*)lbl_8047AB30;
    *(u32*)((u8*)r9 + 0x4) = r10;
    *(u32*)lbl_8047AB30 = r9;
L_800E1CD0: ;
    r10 = *(u32*)((u8*)r8 + 0x0);
    r10 = *(u32*)((u8*)r10 + 0x8);
    *(u32*)((u8*)r9 + 0x8) = r10;
    r10 = *(u32*)((u8*)r9 + 0x0);
    if ((u32)r10 == (u32)0x0) goto L_800E1CEC;
    *(u32*)((u8*)r10 + 0x4) = r9;
L_800E1CEC: ;
    r10 = *(u32*)((u8*)r9 + 0x4);
    if ((u32)r10 == (u32)0x0) goto L_800E1CFC;
    *(u32*)((u8*)r10 + 0x0) = r9;
L_800E1CFC: ;
    r12 = *(u32*)((u8*)r9 + 0x4);
    if ((u32)r12 == (u32)0x0) goto L_800E1D44;
    r11 = *(u32*)((u8*)r9 + 0x8);
    r10 = r9 + r11;
    if ((u32)r12 != (u32)r10) goto L_800E1D44;
    r10 = *(u32*)((u8*)r12 + 0x8);
    r10 = r11 + r10;
    *(u32*)((u8*)r9 + 0x8) = r10;
    r10 = *(u32*)((u8*)r9 + 0x4);
    r10 = *(u32*)((u8*)r10 + 0x4);
    if ((u32)r10 == (u32)0x0) goto L_800E1D38;
    *(u32*)((u8*)r10 + 0x0) = r9;
L_800E1D38: ;
    r10 = *(u32*)((u8*)r9 + 0x4);
    r10 = *(u32*)((u8*)r10 + 0x4);
    *(u32*)((u8*)r9 + 0x4) = r10;
L_800E1D44: ;
    r12 = *(u32*)((u8*)r9 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_800E1D8C;
    r11 = *(u32*)((u8*)r12 + 0x8);
    r10 = r12 + r11;
    if ((u32)r9 != (u32)r10) goto L_800E1D8C;
    r10 = *(u32*)((u8*)r9 + 0x8);
    r10 = r11 + r10;
    *(u32*)((u8*)r12 + 0x8) = r10;
    r11 = *(u32*)((u8*)r9 + 0x4);
    if ((u32)r11 == (u32)0x0) goto L_800E1D80;
    r10 = *(u32*)((u8*)r9 + 0x0);
    *(u32*)((u8*)r11 + 0x0) = r10;
L_800E1D80: ;
    r10 = *(u32*)((u8*)r9 + 0x4);
    r9 = *(u32*)((u8*)r9 + 0x0);
    *(u32*)((u8*)r9 + 0x4) = r10;
L_800E1D8C: ;
    r8 = r8 + 0x4;
    r5 = r5 + 0x1;
L_800E1D94: ;
    if ((s32)r5 < (s32)r29) goto L_800E1AA0;
    if ((u32)r0 < (u32)0xc) goto L_800E1E6C;
    *(u32*)((u8*)r3 + 0x0) = r6;
    *(u32*)((u8*)r3 + 0x4) = r7;
    *(u32*)((u8*)r3 + 0x8) = r0;
    r5 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r5 == (u32)0x0) goto L_800E1DC4;
    *(u32*)((u8*)r5 + 0x4) = r3;
    goto L_800E1DC8;
L_800E1DC4: ;
    *(u32*)lbl_8047AB30 = r3;
L_800E1DC8: ;
    r5 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r5 == (u32)0x0) goto L_800E1DD8;
    *(u32*)((u8*)r5 + 0x0) = r3;
L_800E1DD8: ;
    r6 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r6 == (u32)0x0) goto L_800E1E20;
    r5 = *(u32*)((u8*)r3 + 0x8);
    r0 = r3 + r5;
    if ((u32)r6 != (u32)r0) goto L_800E1E20;
    r0 = *(u32*)((u8*)r6 + 0x8);
    r0 = r5 + r0;
    *(u32*)((u8*)r3 + 0x8) = r0;
    r5 = *(u32*)((u8*)r3 + 0x4);
    r5 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r5 == (u32)0x0) goto L_800E1E14;
    *(u32*)((u8*)r5 + 0x0) = r3;
L_800E1E14: ;
    r5 = *(u32*)((u8*)r3 + 0x4);
    r0 = *(u32*)((u8*)r5 + 0x4);
    *(u32*)((u8*)r3 + 0x4) = r0;
L_800E1E20: ;
    r6 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r6 == (u32)0x0) goto L_800E1FD8;
    r5 = *(u32*)((u8*)r6 + 0x8);
    r0 = r6 + r5;
    if ((u32)r3 != (u32)r0) goto L_800E1FD8;
    r0 = *(u32*)((u8*)r3 + 0x8);
    r0 = r5 + r0;
    *(u32*)((u8*)r6 + 0x8) = r0;
    r5 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r5 == (u32)0x0) goto L_800E1E5C;
    r0 = *(u32*)((u8*)r3 + 0x0);
    *(u32*)((u8*)r5 + 0x0) = r0;
L_800E1E5C: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r3 = *(u32*)((u8*)r3 + 0x0);
    *(u32*)((u8*)r3 + 0x4) = r0;
    goto L_800E1FD8;
L_800E1E6C: ;
    r3 = r29 << 2;
    r5 = (u32)sp + 0x8;
    /* lwzx r8, r5, r3 */;
    r3 = *(u32*)((u8*)r8 + 0x8);
    r0 = r3 + r0;
    *(u32*)((u8*)r8 + 0x8) = r0;
    r0 = *(u8*)lbl_8047AB28;
    if ((u32)r0 == (u32)0x0) goto L_800E1FD8;
    r3 = *(u32*)((u8*)r8 + 0x4);
    r5 = 0x0;
    r7 = 0x3d94;
    *(u8*)((u8*)r3 + 0x0) = r5;
    *(u8*)((u8*)r3 + 0x1) = r5;
    *(u8*)((u8*)r3 + 0x2) = r5;
    *(u8*)((u8*)r3 + 0x3) = r5;
    r3 = *(u32*)((u8*)r8 + 0x8);
    r0 = *(u32*)((u8*)r8 + 0x4);
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
    if ((u32)r0 == (u32)0x0) goto L_800E1F58;
    /* srwi. r0, r0, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 == (u32)0x0) goto L_800E1F44;
L_800E1EF4: ;
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
    if (--ctr != 0) goto L_800E1EF4;
    r3 = r3 & 0x7;
    if ((u32)r0 == (u32)0x0) goto L_800E1F58;
L_800E1F44: ;
    ctr_fn = (void(*)(void))r3;
L_800E1F48: ;
    r0 = *(u16*)((u8*)r6 + 0x0);
    r6 = r6 + 0x2;
    r7 = r7 + r0;
    if (--ctr != 0) goto L_800E1F48;
L_800E1F58: ;
    r3 = r5;
    if ((u32)r5 == (u32)0x0) goto L_800E1FD4;
    /* srwi. r0, r5, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r5 == (u32)0x0) goto L_800E1FC0;
L_800E1F70: ;
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
    if (--ctr != 0) goto L_800E1F70;
    r3 = r3 & 0x7;
    if ((u32)r5 == (u32)0x0) goto L_800E1FD4;
L_800E1FC0: ;
    ctr_fn = (void(*)(void))r3;
L_800E1FC4: ;
    r0 = *(u8*)((u8*)r6 + 0x0);
    r6 = r6 + 0x1;
    r7 = r7 + r0;
    if (--ctr != 0) goto L_800E1FC4;
L_800E1FD4: ;
    *(u16*)((u8*)r8 + 0xE) = r7;
L_800E1FD8: ;
    r28 = *(u32*)lbl_8047AB30;
    goto L_800E1FE4;
L_800E1FE0: ;
    r28 = *(u32*)((u8*)r28 + 0x4);
L_800E1FE4: ;
    if ((u32)r28 != (u32)0x0) goto L_800E159C;
L_800E1FEC: ;
    r3 = *(u32*)lbl_8047AB30;
    r0 = 0x0;
    goto L_800E200C;
L_800E1FF8: ;
    r5 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r5 <= (u32)r0) goto L_800E2008;
    r0 = r5;
L_800E2008: ;
    r3 = *(u32*)((u8*)r3 + 0x4);
L_800E200C: ;
    if ((u32)r3 != (u32)0x0) goto L_800E1FF8;
    r3 = r0 - r4;
L_800E2018: ;
    return;
}

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
u32 fn_800D45F8(void) {
    return *(u32*)lbl_8047AA80;
}

/* fn_800D4604 | Size: 0xC */
void fn_800D4604(u32 val) {
    *(u32*)lbl_8047AA80 = val;
}

/* fn_800D4610 | Size: 0xC */
void fn_800D4610(u8 val) {
    *((u8*)lbl_8047AA80 + 0x49C) = val;
}

/* fn_800D5504 -- GSlog_Init | Size: 0xCC */
void fn_800D5504(void) {
    extern u8 lbl_802703C0[];
    extern u8 lbl_804001F0[];
    extern void fn_800DD970();
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u32 r3 = 0;
    u8* state;
    u32 size;
    u16 handle;
    u32 ptr;

    size = r3;
    state = (u8*)*(u32*)lbl_8047AA80;
    if (*(u32*)(state + 0x490) != 0) {
        fn_800DD970((u32)lbl_802703C0);
        return;
    }
    fn_800E3534();
    state = (u8*)*(u32*)lbl_8047AA80;
    *(u16*)(state + 0x48C) = (u16)r3;
    r3 = *(u16*)((u8*)*(u32*)lbl_8047AA80 + 0x48C);
    if (r3 == 0) {
        fn_800DD970((u32)lbl_802703C0 + 0x28);
        return;
    }
    fn_800E27B0();
    ptr = r3;
    state = (u8*)*(u32*)lbl_8047AA80;
    *(u32*)(state + 0x490) = ptr;
    *(u32*)(state + 0x494) = *(u32*)(state + 0x490);
    state = (u8*)*(u32*)lbl_8047AA80;
    *(u32*)(state + 0x498) = size;
    *(u32*)(lbl_804001F0 + 0x28) = 0;
    state = (u8*)*(u32*)lbl_8047AA80;
    fn_800DD970((u32)lbl_802703C0 + 0x4C, *(u32*)(state + 0x498), *(u32*)(state + 0x490));
}

/* fn_800D55D0 | Size: 0x78 */
void fn_800D55D0(f32 val) {
    extern f32 lbl_8047CA30;
    extern f32 lbl_8047CA34;
    extern f32 lbl_8047CA38;
    extern void fn_800B944C();
    extern void fn_800D4F98();
    extern u32 lbl_8047AA80;
    u8* state;
    s32 scaled;
    if (val < lbl_8047CA30 || val > lbl_8047CA34) {
        return;
    }
    state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x25, 0xB);
        return;
    }
    scaled = (s32)(lbl_8047CA38 * val);
    fn_800B944C(scaled, 0);
}

/* fn_800D5648 | Size: 0x78 */
void fn_800D5648(f32 val) {
    extern f32 lbl_8047CA30;
    extern f32 lbl_8047CA34;
    extern f32 lbl_8047CA38;
    extern void fn_800B9404();
    extern void fn_800D4F98();
    extern u32 lbl_8047AA80;
    u8* state;
    s32 scaled;
    if (val < lbl_8047CA30 || val > lbl_8047CA34) {
        return;
    }
    state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x24, 0xB);
        return;
    }
    scaled = (s32)(lbl_8047CA38 * val);
    fn_800B9404(scaled, 0);
}

/* fn_800D56C0 | Size: 0x64 */
void fn_800D56C0(u8 param) {
    extern void fn_800D4F98();
    extern void fn_800D7230();
    extern u32 lbl_8047AA80;
    u8* state;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x23, 0x1, param);
    } else {
        *(u32*)(state + 0x4A0) = (u32)fn_800D7230;
        state[0x4A4] = param;
    }
}

/* fn_800D5724 | Size: 0x78 */
void fn_800D5724(u32 index, u8 param) {
    extern void fn_800D4F98();
    extern void fn_800D724C();
    extern u32 lbl_8047AA80;
    u8* state;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x22, 0x2, index, param);
    } else {
        *(u32*)(state + (index << 2) + 0x500) = (u32)fn_800D724C;
        *(u8*)(state + (index << 4) + 0x520) = param;
    }
}

/* fn_800D579C | Size: 0x78 */
void fn_800D579C(u32 param1, u16 param2) {
    extern void fn_800D4F98();
    extern void fn_800D7268();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x21, 0x2, param1, (u16)param2, state);
    } else {
        *(u32*)((state + (0x21 << 2)) + 0x500) = (u32)fn_800D7268;
        *(u16*)((state + (0x21 << 4)) + 0x522) = 0x2;
    }
}


/* fn_800D5814 | Size: 0x8C */
void fn_800D5814(u32 index, u8 param1, u8 param2) {
    extern void fn_800D4F98();
    extern void fn_800D72A4();
    extern u32 lbl_8047AA80;
    u8* state;
    u32 off4;
    u32 off16;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x1F, 0x3, index, (u32)param1, (u32)param2);
    } else {
        off4 = index << 2;
        off16 = index << 4;
        *(u32*)(state + off4 + 0x500) = (u32)fn_800D72A4;
        *(u8*)(state + off16 + 0x520) = param1;
        *(u8*)(state + off16 + 0x521) = param2;
    }
}

/* fn_800D58A0 | Size: 0x8C */
void fn_800D58A0(u32 index, s16 param1, s16 param2) {
    extern void fn_800D4F98();
    extern void fn_800D72C4();
    extern u32 lbl_8047AA80;
    u8* state;
    u32 off4;
    u32 off16;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x1E, 0x3, index, (s32)param1, (s32)param2);
    } else {
        off4 = index << 2;
        off16 = index << 4;
        *(u32*)(state + off4 + 0x500) = (u32)fn_800D72C4;
        *(u16*)(state + off16 + 0x522) = (u16)param1;
        *(u16*)(state + off16 + 0x524) = (u16)param2;
    }
}

/* fn_800D592C | Size: 0x8C */
void fn_800D592C(u32 index, u16 param1, u16 param2) {
    extern void fn_800D4F98();
    extern void fn_800D72E4();
    extern u32 lbl_8047AA80;
    u8* state;
    u32 off4;
    u32 off16;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x1D, 0x3, index, (u32)param1, (u32)param2);
    } else {
        off4 = index << 2;
        off16 = index << 4;
        *(u32*)(state + off4 + 0x500) = (u32)fn_800D72E4;
        *(u16*)(state + off16 + 0x522) = param1;
        *(u16*)(state + off16 + 0x524) = param2;
    }
}

/* fn_800D59B8 | Size: 0x80 */
void fn_800D59B8(u32 index, f32 param1, f32 param2) {
    extern void fn_800D4F98();
    extern void fn_800D7304();
    extern u32 lbl_8047AA80;
    u8* state;
    u32 off4;
    u32 off16;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x1C, 0xF, index, param1, param2);
    } else {
        off4 = index << 2;
        off16 = index << 4;
        *(u32*)(state + off4 + 0x500) = (u32)fn_800D7304;
        *(f32*)(state + off16 + 0x528) = param1;
        *(f32*)(state + off16 + 0x52C) = param2;
    }
}

/* fn_800D5A38 | Size: 0x78 */
void fn_800D5A38(u32 index, u8 param) {
    extern void fn_800D4F98();
    extern void fn_800D7328();
    extern u32 lbl_8047AA80;
    u8* state;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x1B, 2, index, (u32)param);
    } else {
        u32 off4 = index << 2;
        u32 offC = index * 0xC;
        *(u32*)(state + off4 + 0x4E0) = (u32)fn_800D7328;
        *(u8*)(state + offC + 0x4E8) = param;
    }
}

/* fn_800D5AB0 | Size: 0x78 */
void fn_800D5AB0(u32 index, u16 param) {
    extern void fn_800D4F98();
    extern void fn_800D7344();
    extern u32 lbl_8047AA80;
    u8* state;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x1A, 2, index, (u32)param);
    } else {
        u32 off4 = index << 2;
        u32 offC = index * 0xC;
        *(u32*)(state + off4 + 0x4E0) = (u32)fn_800D7344;
        *(u16*)(state + offC + 0x4EC) = param;
    }
}

/* fn_800D5B28 | Size: 0x78 */
void fn_800D5B28(u32 index, u16 param) {
    extern void fn_800D4F98();
    extern void fn_800D7360();
    extern u32 lbl_8047AA80;
    u8* state;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x19, 2, index, (u32)param);
    } else {
        u32 off4 = index << 2;
        u32 offC = index * 0xC;
        *(u32*)(state + off4 + 0x4E0) = (u32)fn_800D7360;
        *(u16*)(state + offC + 0x4EC) = param;
    }
}

/* fn_800D5BA0 | Size: 0x78 */
void fn_800D5BA0(u32 index, u32 param) {
    extern void fn_800D4F98();
    extern void fn_800D737C();
    extern u32 lbl_8047AA80;
    u8* state;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x18, 2, index, param);
    } else {
        u32 off4 = index << 2;
        u32 offC = index * 0xC;
        *(u32*)(state + off4 + 0x4E0) = (u32)fn_800D737C;
        *(u32*)(state + offC + 0x4F0) = param;
    }
}

/* fn_800D5C18 | Size: 0xA0 */
void fn_800D5C18(u32 index, u8 param1, u8 param2, u8 param3) {
    extern void fn_800D4F98();
    extern void fn_800D7398();
    extern u32 lbl_8047AA80;
    u8* state;
    state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x17, 4, index, (u32)param1, (u32)param2, (u32)param3);
    } else {
        u32 off4 = index << 2;
        u32 offC = index * 0xC;
        *(u32*)(state + off4 + 0x4E0) = (u32)fn_800D7398;
        *(u8*)(state + offC + 0x4E8) = param1;
        *(u8*)(state + offC + 0x4E9) = param2;
        *(u8*)(state + offC + 0x4EA) = param3;
    }
}

/* fn_800D5CB8 | Size: 0xB4 */
void fn_800D5CB8(u32 param1, u8 param2, u32 param3, u32 param4, u32 param5) {
    extern void fn_800D4F98();
    extern void fn_800D73C4();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x16, 0x5, param1, (u8)param2, (u8)param3, (u8)param4, (u8)param5, param4);
    } else {
        *(u32*)(((u8)param4 + (0x16 << 2)) + 0x4E0) = (u32)fn_800D73C4;
        *(u8*)((state + (0x16 * 0xc)) + 0x4E8) = 0x5;
        *(u8*)((state + (0x16 * 0xc)) + 0x4E9) = param3;
        *(u8*)((state + (0x16 * 0xc)) + 0x4EA) = param4;
        *(u8*)((state + (0x16 * 0xc)) + 0x4EB) = (u8)param5;
    }
}


/* fn_800D5D6C | Size: 0x64 */
void fn_800D5D6C(u8 param1) {
    extern void fn_800D4F98();
    extern void fn_800D73F8();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x15, 0x1, (u8)param1);
    } else {
        *(u32*)((u8)param1 + 0x4C4) = (u32)fn_800D73F8;
        *(u8*)(state + 0x4C8) = 0x15;
    }
}


/* fn_800D5DD0 | Size: 0x64 */
void fn_800D5DD0(u16 param1) {
    extern void fn_800D4F98();
    extern void fn_800D740C();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x14, 0x1, (u16)param1);
    } else {
        *(u32*)((u16)param1 + 0x4C4) = (u32)fn_800D740C;
        *(u16*)(state + 0x4CC) = 0x14;
    }
}


/* fn_800D5E34 | Size: 0x80 */
void fn_800D5E34(s8 param1, s8 param2, u32 param3) {
    extern void fn_800D4F98();
    extern void fn_800D7420();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x13, 0x3, (s8)param1, (s8)param2, (s8)param3);
    } else {
        *(u32*)((s8)param2 + 0x4C4) = (u32)fn_800D7420;
        *(u8*)(state + 0x4C8) = 0x13;
        *(u8*)(state + 0x4C9) = 0x3;
        *(u8*)(state + 0x4CA) = (s8)param3;
    }
}


/* fn_800D5EB4 | Size: 0x80 */
void fn_800D5EB4(s16 param1, s16 param2, u32 param3) {
    extern void fn_800D4F98();
    extern void fn_800D7444();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x12, 0x3, (s16)param1, (s16)param2, (s16)param3);
    } else {
        *(u32*)((s16)param2 + 0x4C4) = (u32)fn_800D7444;
        *(u16*)(state + 0x4CC) = 0x12;
        *(u16*)(state + 0x4CE) = 0x3;
        *(u16*)(state + 0x4D0) = (s16)param3;
    }
}


/* fn_800D5F34 | Size: 0x70 */
void fn_800D5F34(f32 fp1, f32 fp2, f32 fp3) {
    extern void fn_800D4F98();
    extern void fn_800D7468();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x11, 0xd);
    } else {
        *(u32*)(0xd + 0x4C4) = (u32)fn_800D7468;
        *(f32*)(state + 0x4D4) = fp1;
        *(f32*)(state + 0x4D8) = fp2;
        *(f32*)(state + 0x4DC) = fp3;
    }
}


/* fn_800D5FA4 | Size: 0x84 */
void fn_800D5FA4(u32 param1) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D748C();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x10, 0x1, (u8)param1);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D748C;
        *(u8*)(state + 0x4AC) = param1;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D6028 | Size: 0x84 */
void fn_800D6028(u32 param1) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D74A0();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0xf, 0x1, (u16)param1);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D74A0;
        *(u16*)(state + 0x4B0) = param1;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D60AC | Size: 0x9C */
void fn_800D60AC(u32 param1, u32 param2) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D74B4();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0xe, 0x2, (s8)param1, (s8)param2);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D74B4;
        *(u8*)(state + 0x4AC) = param1;
        *(u8*)(state + 0x4AD) = param2;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D6148 | Size: 0x9C */
void fn_800D6148(u32 param1, u32 param2) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D74D0();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0xd, 0x2, (u8)param1, (u8)param2);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D74D0;
        *(u8*)(state + 0x4AC) = param1;
        *(u8*)(state + 0x4AD) = param2;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D61E4 | Size: 0x9C */
void fn_800D61E4(u32 param1, u32 param2) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D74EC();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0xc, 0x2, (s16)param1, (s16)param2);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D74EC;
        *(u16*)(state + 0x4B0) = param1;
        *(u16*)(state + 0x4B2) = param2;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D6280 | Size: 0x9C */
void fn_800D6280(u32 param1, u32 param2) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D7508();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0xb, 0x2, (u16)param1, (u16)param2);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D7508;
        *(u16*)(state + 0x4B0) = param1;
        *(u16*)(state + 0x4B2) = param2;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D631C | Size: 0x94 */
void fn_800D631C(f32 fp1, f32 fp2) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D7524();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0xa, 0xc);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D7524;
        *(f32*)(state + 0x4B8) = fp1;
        *(f32*)(state + 0x4BC) = fp2;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D63B0 | Size: 0xB4 */
void fn_800D63B0(u32 param1, u32 param2, u32 param3) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D7540();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x9, 0x3, (s8)param1, (s8)param2, (s8)param3);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D7540;
        *(u8*)(state + 0x4AC) = param1;
        *(u8*)(state + 0x4AD) = param2;
        *(u8*)(state + 0x4AE) = param3;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D6464 | Size: 0xB4 */
void fn_800D6464(u32 param1, u32 param2, u32 param3) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D7564();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x8, 0x3, (u8)param1, (u8)param2, (u8)param3);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D7564;
        *(u8*)(state + 0x4AC) = param1;
        *(u8*)(state + 0x4AD) = param2;
        *(u8*)(state + 0x4AE) = param3;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D6518 | Size: 0xB4 */
void fn_800D6518(u32 param1, u32 param2, u32 param3) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D7588();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x7, 0x3, (s16)param1, (s16)param2, (s16)param3);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D7588;
        *(u16*)(state + 0x4B0) = param1;
        *(u16*)(state + 0x4B2) = param2;
        *(u16*)(state + 0x4B4) = param3;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D65CC | Size: 0xB4 */
void fn_800D65CC(u32 param1, u32 param2, u32 param3) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D75AC();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x6, 0x3, (u16)param1, (u16)param2, (u16)param3);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D75AC;
        *(u16*)(state + 0x4B0) = param1;
        *(u16*)(state + 0x4B2) = param2;
        *(u16*)(state + 0x4B4) = param3;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D6680 | Size: 0xA8 */
void fn_800D6680(f32 fp1, f32 fp2, f32 fp3) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    extern void fn_800D75D0();
    u8* state = (u8*)lbl_8047AA80;
    if (state[0x47E] == 0 && *(s32*)state == 1) {
        fn_800D4F98(0x5, 0xd);
    } else {
        fn_800D6B00();
        *(u32*)(state + 0x4A8) = (u32)fn_800D75D0;
        *(f32*)(state + 0x4B8) = fp1;
        *(f32*)(state + 0x4BC) = fp2;
        *(f32*)(state + 0x4C0) = fp3;
        *(u8*)(state + 0x49F) = 0x1;
    }
}


/* fn_800D6728 | Size: 0x94 */
void fn_800D6728(void) {
    extern void fn_800D4F98();
    extern void fn_800D6B00();
    u8* state = (u8*)*(u32*)lbl_8047AA80;

    if (state[0x47E] == 1) {
        fn_800D6B00();
    } else if (*(s32*)state == 1) {
        fn_800D4F98(3, 0);
    } else {
        fn_800D6B00();
        state = (u8*)*(u32*)lbl_8047AA80;
        if (state[0x1B] == state[0x1A] &&
            *(u32*)(state + 0x4) != *(u32*)(state + 0x8) &&
            *(u32*)(state + 0x24) == *(u32*)(state + 0x20)) {
            *(u32*)(state + 0x24) = 0;
        }
    }
}

/* fn_800D67BC -- MTX operations | Size: 0x244 */
void fn_800D67BC(void) {
    extern u8 lbl_80314350[];
    extern u8 lbl_804001F0[];
    extern void fn_800B928C();
    extern void fn_800D4F98();
    extern void fn_800D7650();
    extern void fn_800D7868();
    extern void fn_800D7A70();
    extern void fn_800D892C();
    extern void fn_800DB758();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r31 = 0;

    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    if ((u32)r0 != (u32)0x1) goto L_800D67E8;
    fn_800DB758();
    goto L_800D69EC;
L_800D67E8: ;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D680C;
    r5 = r31 & 0xFFFF;
    r3 = 0x2;
    r4 = 0x1;
    fn_800D4F98();
    goto L_800D69EC;
L_800D680C: ;
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    if ((u32)r3 != (u32)r0) goto L_800D69EC;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r3 == (u32)r0) goto L_800D69EC;
    r0 = *(u32*)((u8*)r4 + 0x24);
    if ((u32)r0 != (u32)0x0) goto L_800D6910;
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
    if ((u32)r0 == (u32)0x0) goto L_800D689C;
    r3 = *(u32*)((u8*)r3 + 0x20);
    r4 = 0x2;
    r5 = 0x0;
    r6 = 0x2;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
L_800D689C: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x10);
    r0 = r0 & 0x1;
    if ((u32)r0 == (u32)0x0) goto L_800D68D0;
    r3 = *(u32*)((u8*)r3 + 0x20);
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x6;
    r7 = 0xa;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
L_800D68D0: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r0 == (u32)0x0) goto L_800D6904;
    r3 = *(u32*)((u8*)r3 + 0x20);
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x8;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
L_800D6904: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x20);
    *(u32*)((u8*)r3 + 0x24) = r0;
L_800D6910: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x14);
    if ((s32)r0 != (s32)0x7) goto L_800D6924;
    /* clrlslwi r31, r31, 17, 1 */;
L_800D6924: ;
    r3 = *(u32*)((u8*)r3 + 0x24);
    fn_800D7A70();
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r3 + 0x24);
    fn_800D892C();
    r4 = *(u32*)lbl_8047AA80;
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
    r0 = r0 + r5;
    *(u32*)((u8*)r6 + 0xC) = r0;
    if ((s32)r3 >= (s32)0x6) goto L_800D6998;
    if ((s32)r3 == (s32)0x3) goto L_800D69A4;
    if ((s32)r3 >= (s32)0x3) goto L_800D69C8;
    goto L_800D69EC;
L_800D6998: ;
    if ((s32)r3 >= (s32)0x8) goto L_800D69EC;
    goto L_800D69DC;
L_800D69A4: ;
    r3 = (0x5555 << 16);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r0 = r3 + 0x5556;
    r3 = (s32)((s64)r0 * (s64)r5 >> 32);
    r0 = (u32)r3 >> 31;
    r0 = r3 + r0;
    r0 = r4 + r0;
    *(u32*)((u8*)r6 + 0x4) = r0;
    goto L_800D69EC;
L_800D69C8: ;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r3 = r5 + r0;
    *(u32*)((u8*)r6 + 0x4) = r0;
    goto L_800D69EC;
L_800D69DC: ;
    r3 = *(u32*)((u8*)r6 + 0x4);
    r0 = r3 + r0;
    *(u32*)((u8*)r6 + 0x4) = r0;
L_800D69EC: ;
    return;
}

/* fn_800D6A00 | Size: 0x5C */
void fn_800D6A00(u32 val) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(u8*)(state + 0x47E) == 1) {
        *(u32*)(state + 0x488) = val;
    } else if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x1, 0x1, val);
    } else {
        *(u32*)(state + 0x14) = val;
    }
}

/* fn_800D6A5C | Size: 0x24 */
void fn_800D6A5C(u32 dx, u32 dy) {
    extern u8 lbl_804001F0[];
    u8* base = lbl_804001F0;
    *(u32*)(base + 0xC) = *(u32*)(base + 0xC) + dx;
    *(u32*)(base + 0x4) = *(u32*)(base + 0x4) + dy;
}

/* fn_800D6A80 | Size: 0x80 */
void fn_800D6A80(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r0 = *(u32*)((u8*)r5 + 0x0);
    r7 = r3 & 0xFFFF;
    r0 = r0 + r7;
    *(u32*)((u8*)r5 + 0x0) = r0;
    if ((s32)r4 >= (s32)0x6) goto L_800D6AA8;
    if ((s32)r4 == (s32)0x3) goto L_800D6AB4;
    if ((s32)r4 >= (s32)0x3) goto L_800D6AD8;
    return;
L_800D6AA8: ;
    if ((s32)r4 >= (s32)0x8) return;
    goto L_800D6AEC;
L_800D6AB4: ;
    r3 = (0x5555 << 16);
    r4 = *(u32*)((u8*)r6 + 0x0);
    r0 = r3 + 0x5556;
    r3 = (s32)((s64)r0 * (s64)r7 >> 32);
    r0 = (u32)r3 >> 31;
    r0 = r3 + r0;
    r0 = r4 + r0;
    *(u32*)((u8*)r6 + 0x0) = r0;
    return;
L_800D6AD8: ;
    r0 = *(u32*)((u8*)r6 + 0x0);
    r3 = r7 + r0;
    *(u32*)((u8*)r6 + 0x0) = r0;
    return;
L_800D6AEC: ;
    r4 = *(u32*)((u8*)r6 + 0x0);
    r0 = r4 + r0;
    *(u32*)((u8*)r6 + 0x0) = r0;
    return;
}

/* fn_800D6B00 -- Large matrix setup | Size: 0x730 */
void fn_800D6B00(void) {
    extern u8 lbl_804007E8[];
    extern void fn_800DB098();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r3 = (u32)lbl_804007E8;
    r31 = (u32)lbl_804007E8;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x49F);
    if ((u32)r0 == (u32)0x0) goto L_800D7210;
    r3 = *(u8*)((u8*)r4 + 0x47E);
    if ((u32)r3 != (u32)0x0) goto L_800D6B4C;
    r0 = *(u32*)((u8*)r4 + 0x14);
    if ((s32)r0 == (s32)0x7) goto L_800D6B60;
L_800D6B4C: ;
    if ((u32)r3 != (u32)0x1) goto L_800D70F8;
    r0 = *(u32*)((u8*)r4 + 0x488);
    if ((s32)r0 != (s32)0x7) goto L_800D70F8;
L_800D6B60: ;
    r0 = *(u8*)((u8*)r4 + 0x18);
    if ((u32)r0 != (u32)0x1) goto L_800D70B4;
    r3 = r31 + 0x98;
    r4 = r4 + 0x4ac;
    r5 = 0x18;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0x80;
    r5 = 0x18;
    r4 = r4 + 0x4e8;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0x0;
    r5 = 0x80;
    r4 = r4 + 0x520;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
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
    memcpy((void*)r3, (const void*)r4, (u32)r5);
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
    if ((u32)r0 != (u32)0x1) goto L_800D6D1C;
    fn_800DB098();
    goto L_800D6E10;
L_800D6D1C: ;
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    if ((u32)r3 != (u32)r0) goto L_800D6E10;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r3 == (u32)r0) goto L_800D6E10;
    r29 = *(u32*)((u8*)r4 + 0x24);
    r0 = *(u8*)((u8*)r29 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D6D5C;
    r12 = *(u32*)((u8*)r4 + 0x4A0);
    r3 = 0x0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6D5C: ;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4A8);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u8*)((u8*)r29 + 0x40);
    if ((u32)r0 == (u32)0x0) goto L_800D6D90;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4C4);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6D90: ;
    r30 = 0x4;
    r28 = r29 + 0x70;
L_800D6D98: ;
    r0 = *(u8*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D6DC0;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x4e0;
    /* lwzx r12, r5, r0 */;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6DC0: ;
    r30 = r30 + 0x1;
    r28 = r28 + 0x1c;
    if ((s32)r30 <= (s32)0x5) goto L_800D6D98;
    r30 = 0x6;
    r28 = r29 + 0xa8;
L_800D6DD8: ;
    r0 = *(u8*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D6E00;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x500;
    /* lwzx r12, r5, r0 */;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6E00: ;
    r30 = r30 + 0x1;
    r28 = r28 + 0x1c;
    if ((s32)r30 <= (s32)0xd) goto L_800D6DD8;
L_800D6E10: ;
    r3 = *(u32*)lbl_8047AA80;
    r4 = r31 + 0x98;
    r5 = 0x18;
    r3 = r3 + 0x4ac;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = *(u32*)lbl_8047AA80;
    r4 = r31 + 0x80;
    r5 = 0x18;
    r3 = r3 + 0x4e8;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = *(u32*)lbl_8047AA80;
    r4 = r31 + 0x0;
    r5 = 0x80;
    r3 = r3 + 0x520;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    if ((u32)r0 != (u32)0x1) goto L_800D6E64;
    fn_800DB098();
    goto L_800D6F58;
L_800D6E64: ;
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    if ((u32)r3 != (u32)r0) goto L_800D6F58;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r3 == (u32)r0) goto L_800D6F58;
    r30 = *(u32*)((u8*)r4 + 0x24);
    r0 = *(u8*)((u8*)r30 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D6EA4;
    r12 = *(u32*)((u8*)r4 + 0x4A0);
    r3 = 0x0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6EA4: ;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4A8);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u8*)((u8*)r30 + 0x40);
    if ((u32)r0 == (u32)0x0) goto L_800D6ED8;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4C4);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6ED8: ;
    r29 = 0x4;
    r28 = r30 + 0x70;
L_800D6EE0: ;
    r0 = *(u8*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D6F08;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x4e0;
    /* lwzx r12, r5, r0 */;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6F08: ;
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    if ((s32)r29 <= (s32)0x5) goto L_800D6EE0;
    r29 = 0x6;
    r28 = r30 + 0xa8;
L_800D6F20: ;
    r0 = *(u8*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D6F48;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x500;
    /* lwzx r12, r5, r0 */;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D6F48: ;
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    if ((s32)r29 <= (s32)0xd) goto L_800D6F20;
L_800D6F58: ;
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
    goto L_800D70F8;
L_800D70B4: ;
    r3 = r31 + 0x148;
    r4 = r4 + 0x4ac;
    r5 = 0x18;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0x130;
    r5 = 0x18;
    r4 = r4 + 0x4e8;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AA80;
    r3 = r31 + 0xb0;
    r5 = 0x80;
    r4 = r4 + 0x520;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x18) = r0;
L_800D70F8: ;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x47E);
    if ((u32)r0 != (u32)0x1) goto L_800D7110;
    fn_800DB098();
    goto L_800D7204;
L_800D7110: ;
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    if ((u32)r3 != (u32)r0) goto L_800D7204;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r3 == (u32)r0) goto L_800D7204;
    r30 = *(u32*)((u8*)r4 + 0x24);
    r0 = *(u8*)((u8*)r30 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D7150;
    r12 = *(u32*)((u8*)r4 + 0x4A0);
    r3 = 0x0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D7150: ;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4A8);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = *(u8*)((u8*)r30 + 0x40);
    if ((u32)r0 == (u32)0x0) goto L_800D7184;
    r4 = *(u32*)lbl_8047AA80;
    r3 = 0x0;
    r12 = *(u32*)((u8*)r4 + 0x4C4);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D7184: ;
    r29 = 0x4;
    r28 = r30 + 0x70;
L_800D718C: ;
    r0 = *(u8*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D71B4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x4e0;
    /* lwzx r12, r5, r0 */;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D71B4: ;
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    if ((s32)r29 <= (s32)0x5) goto L_800D718C;
    r29 = 0x6;
    r28 = r30 + 0xa8;
L_800D71CC: ;
    r0 = *(u8*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D71F4;
    r5 = *(u32*)lbl_8047AA80;
    r4 = r3 << 2;
    r0 = r4 + 0x500;
    /* lwzx r12, r5, r0 */;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800D71F4: ;
    r29 = r29 + 0x1;
    r28 = r28 + 0x1c;
    if ((s32)r29 <= (s32)0xd) goto L_800D71CC;
L_800D7204: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x49F) = r0;
L_800D7210: ;
    return;
}

/* fn_800D7230 | Size: 0x1C */
void fn_800D7230(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    u8 val = *(u8*)(state + 0x4A4);
    *fifo = (u8)(val + (val << 1));
}

/* fn_800D724C | Size: 0x1C */
void fn_800D724C(u32 idx) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    state += idx << 4;
    *fifo = *(u8*)(state + 0x520);
}

/* fn_800D7268 | Size: 0x1C */
void fn_800D7268(u32 idx) {
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    state += idx << 4;
    *fifo = *(u16*)(state + 0x522);
}

/* fn_800D7284 | Size: 0x20 */
void fn_800D7284(u32 idx) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    u8 val;
    state += idx << 4;
    val = *(u8*)(state + 0x520);
    *fifo = val;
    *fifo = val;
}

/* fn_800D72A4 | Size: 0x20 */
void fn_800D72A4(u32 idx) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    u8 val;
    state += idx << 4;
    val = *(u8*)(state + 0x520);
    *fifo = val;
    *fifo = val;
}

/* fn_800D72C4 | Size: 0x20 */
void fn_800D72C4(u32 idx) {
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    u16 val;
    state += idx << 4;
    val = *(u16*)(state + 0x522);
    *fifo = val;
    *fifo = val;
}

/* fn_800D72E4 | Size: 0x20 */
void fn_800D72E4(u32 idx) {
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    u16 val;
    state += idx << 4;
    val = *(u16*)(state + 0x522);
    *fifo = val;
    *fifo = val;
}

/* fn_800D7304 | Size: 0x24 */
void fn_800D7304(u32 idx) {
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    f32 v0, v1;
    state += idx << 4;
    v0 = *(f32*)(state + 0x528);
    v1 = *(f32*)(state + 0x52C);
    *fifo = v0;
    *fifo = v1;
}

/* fn_800D7328 | Size: 0x1C */
void fn_800D7328(u32 idx) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    state += idx * 12;
    *fifo = *(u8*)(state + 0x4E8);
}

/* fn_800D7344 | Size: 0x1C */
void fn_800D7344(u32 idx) {
    u32 off = idx * 12;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80 + off;
    *fifo = *(u16*)(state + 0x4EC);
}

/* fn_800D7360 | Size: 0x1C */
void fn_800D7360(u32 idx) {
    u32 off = idx * 12;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80 + off;
    *fifo = *(u16*)(state + 0x4EC);
}

/* fn_800D737C | Size: 0x1C */
void fn_800D737C(u32 idx) {
    u32 off = idx * 12;
    volatile u32* fifo = (volatile u32*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80 + off;
    *fifo = *(u32*)(state + 0x4F0);
}

/* fn_800D7398 | Size: 0x2C */
void fn_800D7398(u32 idx) {
    u32 off = idx * 12;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80 + off;
    u8 b0 = *(u8*)(state + 0x4E8);
    u8 b1 = *(u8*)(state + 0x4E9);
    u8 b2 = *(u8*)(state + 0x4EA);
    *fifo = b0;
    *fifo = b1;
    *fifo = b2;
}

/* fn_800D73C4 | Size: 0x34 */
void fn_800D73C4(u32 idx) {
    u32 off = idx * 12;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80 + off;
    u8 b0 = *(u8*)(state + 0x4E8);
    u8 b1 = *(u8*)(state + 0x4E9);
    u8 b2 = *(u8*)(state + 0x4EA);
    u8 b3 = *(u8*)(state + 0x4EB);
    *fifo = b0;
    *fifo = b1;
    *fifo = b2;
    *fifo = b3;
}

/* fn_800D73F8 | Size: 0x14 */
void fn_800D73F8(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *fifo = *(u8*)(state + 0x4C8);
}

/* fn_800D740C | Size: 0x14 */
void fn_800D740C(void) {
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *fifo = *(u16*)(state + 0x4CC);
}

/* fn_800D7420 | Size: 0x24 */
void fn_800D7420(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8 b2 = *(u8*)(state + 0x4CA);
    u8 b1 = *(u8*)(state + 0x4C9);
    u8 b0 = *(u8*)(state + 0x4C8);
    *fifo = b0;
    *fifo = b1;
    *fifo = b2;
}

/* fn_800D7444 | Size: 0x24 */
void fn_800D7444(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u16 h2 = *(u16*)(state + 0x4D0);
    u16 h1 = *(u16*)(state + 0x4CE);
    u16 h0 = *(u16*)(state + 0x4CC);
    *fifo = h0;
    *fifo = h1;
    *fifo = h2;
}

/* fn_800D7468 | Size: 0x24 */
void fn_800D7468(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 f2 = *(f32*)(state + 0x4DC);
    f32 f1 = *(f32*)(state + 0x4D8);
    f32 f0 = *(f32*)(state + 0x4D4);
    *fifo = f0;
    *fifo = f1;
    *fifo = f2;
}

/* fn_800D748C | Size: 0x14 */
void fn_800D748C(void) {
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *fifo = *(u8*)(state + 0x4AC);
}

/* fn_800D74A0 | Size: 0x14 */
void fn_800D74A0(void) {
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    *fifo = *(u16*)(state + 0x4B0);
}

/* fn_800D74B4 | Size: 0x1C */
void fn_800D74B4(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8 b1 = *(u8*)(state + 0x4AD);
    u8 b0 = *(u8*)(state + 0x4AC);
    *fifo = b0;
    *fifo = b1;
}

/* fn_800D74D0 | Size: 0x1C */
void fn_800D74D0(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8 b1 = *(u8*)(state + 0x4AD);
    u8 b0 = *(u8*)(state + 0x4AC);
    *fifo = b0;
    *fifo = b1;
}

/* fn_800D74EC | Size: 0x1C */
void fn_800D74EC(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u16 h1 = *(u16*)(state + 0x4B2);
    u16 h0 = *(u16*)(state + 0x4B0);
    *fifo = h0;
    *fifo = h1;
}

/* fn_800D7508 | Size: 0x1C */
void fn_800D7508(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u16 h1 = *(u16*)(state + 0x4B2);
    u16 h0 = *(u16*)(state + 0x4B0);
    *fifo = h0;
    *fifo = h1;
}

/* fn_800D7524 | Size: 0x1C */
void fn_800D7524(void) {
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u8* state = (u8*)lbl_8047AA80;
    f32 f0 = *(f32*)(state + 0x4B8);
    f32 f1 = *(f32*)(state + 0x4BC);
    *fifo = f0;
    *fifo = f1;
}

/* fn_800D7540 | Size: 0x24 */
void fn_800D7540(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8 b2 = *(u8*)(state + 0x4AE);
    u8 b1 = *(u8*)(state + 0x4AD);
    u8 b0 = *(u8*)(state + 0x4AC);
    *fifo = b0;
    *fifo = b1;
    *fifo = b2;
}

/* fn_800D7564 | Size: 0x24 */
void fn_800D7564(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u8* fifo = (volatile u8*)0xCC008000;
    u8 b2 = *(u8*)(state + 0x4AE);
    u8 b1 = *(u8*)(state + 0x4AD);
    u8 b0 = *(u8*)(state + 0x4AC);
    *fifo = b0;
    *fifo = b1;
    *fifo = b2;
}

/* fn_800D7588 | Size: 0x24 */
void fn_800D7588(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u16 h2 = *(u16*)(state + 0x4B4);
    u16 h1 = *(u16*)(state + 0x4B2);
    u16 h0 = *(u16*)(state + 0x4B0);
    *fifo = h0;
    *fifo = h1;
    *fifo = h2;
}

/* fn_800D75AC | Size: 0x24 */
void fn_800D75AC(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    u16 h2 = *(u16*)(state + 0x4B4);
    u16 h1 = *(u16*)(state + 0x4B2);
    u16 h0 = *(u16*)(state + 0x4B0);
    *fifo = h0;
    *fifo = h1;
    *fifo = h2;
}

/* fn_800D75D0 | Size: 0x24 */
void fn_800D75D0(void) {
    u8* state = (u8*)lbl_8047AA80;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 f2 = *(f32*)(state + 0x4C0);
    f32 f1 = *(f32*)(state + 0x4BC);
    f32 f0 = *(f32*)(state + 0x4B8);
    *fifo = f0;
    *fifo = f1;
    *fifo = f2;
}

/* fn_800D75F4 | Size: 0x5C */
void fn_800D75F4(u8* obj) {
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x24) == (u32)obj) {
        *(u32*)(state + 0x24) = 0;
    }
    *(u8*)(obj + 0x8) = 0;
    *(u8*)(obj + 0x24) = 0;
    *(u8*)(obj + 0x40) = 0;
    *(u8*)(obj + 0x5C) = 0;
    *(u8*)(obj + 0x78) = 0;
    *(u8*)(obj + 0x94) = 0;
    *(u8*)(obj + 0xB0) = 0;
    *(u8*)(obj + 0xCC) = 0;
    *(u8*)(obj + 0xE8) = 0;
    *(u8*)(obj + 0x104) = 0;
    *(u8*)(obj + 0x120) = 0;
    *(u8*)(obj + 0x13C) = 0;
    *(u8*)(obj + 0x158) = 0;
    *(u8*)(obj + 0x174) = 0;
    *(u8*)(obj + 0x0) = 0;
}

/* fn_800D7650 | Size: 0x58 */
void fn_800D7650(u8* obj) {
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x24) == (u32)obj) {
        *(u32*)(state + 0x24) = 0;
    }
    *(u8*)(obj + 0x8) = 0;
    *(u8*)(obj + 0x24) = 0;
    *(u8*)(obj + 0x40) = 0;
    *(u8*)(obj + 0x5C) = 0;
    *(u8*)(obj + 0x78) = 0;
    *(u8*)(obj + 0x94) = 0;
    *(u8*)(obj + 0xB0) = 0;
    *(u8*)(obj + 0xCC) = 0;
    *(u8*)(obj + 0xE8) = 0;
    *(u8*)(obj + 0x104) = 0;
    *(u8*)(obj + 0x120) = 0;
    *(u8*)(obj + 0x13C) = 0;
    *(u8*)(obj + 0x158) = 0;
    *(u8*)(obj + 0x174) = 0;
}

/* fn_800D76A8 | Size: 0x178 */
void fn_800D76A8(void) {
    extern u8 lbl_80314370[];
    extern u8 lbl_803143A8[];
    extern u8 lbl_803143B4[];
    extern u8 lbl_803143D8[];
    extern u8 lbl_804001F0[];
    extern void fn_800B7874();
    extern void fn_800B7D3C();
    extern void fn_800B7D74();
    extern void fn_800B84E0();
    extern void fn_800D4F98();
    extern void fn_800D7940();
    extern void fn_800D892C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    if ((u32)r0 != (u32)0x1) goto L_800D76E8;
    r24 = *(u32*)((u8*)r5 + 0x24);
    *(u32*)((u8*)r5 + 0x24) = r29;
    fn_800D7940();
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x24) = r24;
    goto L_800D780C;
L_800D76E8: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D7710;
    r5 = r29;
    r6 = r30 & 0xFFFF;
    r3 = 0x47;
    r4 = 0x2;
    fn_800D4F98();
    goto L_800D780C;
L_800D7710: ;
    r3 = (u32)lbl_80314370;
    r4 = (u32)lbl_803143B4;
    r31 = (u32)lbl_80314370;
    r25 = r29;
    r3 = (u32)lbl_803143D8;
    r27 = (u32)lbl_803143B4;
    r24 = r31;
    r26 = 0x0;
    r28 = (u32)lbl_803143D8;
L_800D7734: ;
    r0 = *(u8*)((u8*)r25 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D7770;
    if ((s32)r26 == (s32)0x0) goto L_800D7770;
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
L_800D7770: ;
    r26 = r26 + 0x1;
    r24 = r24 + 0x4;
    r25 = r25 + 0x1c;
    if ((s32)r26 < (s32)0xe) goto L_800D7734;
    fn_800B7D3C();
    r3 = (u32)lbl_803143A8;
    r24 = r29;
    r25 = 0x0;
    r28 = (u32)lbl_803143A8;
L_800D7798: ;
    r0 = *(u8*)((u8*)r24 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D77D0;
    r0 = *(u32*)((u8*)r24 + 0xC);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = r0 << 2;
    /* lwzx r4, r28, r0 */;
    fn_800B7874();
    r4 = *(u32*)((u8*)r24 + 0x1C);
    if ((u32)r4 == (u32)0x0) goto L_800D77D0;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = *(u8*)((u8*)r24 + 0x20);
    fn_800B84E0();
L_800D77D0: ;
    r25 = r25 + 0x1;
    r31 = r31 + 0x4;
    r24 = r24 + 0x1c;
    if ((s32)r25 < (s32)0xe) goto L_800D7798;
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
L_800D780C: ;
    return;
}

/* fn_800D7820 | Size: 0x48 */
void fn_800D7820(u32 val) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x46, 0x1, val);
    } else {
        *(u32*)(state + 0x24) = val;
    }
}

/* fn_800D7868 | Size: 0x2C */
void fn_800D7868(u8* base, u32 idx, u32 p5, u32 p6, u32 p7, u8 p8, u32 p9, u8 p10) {
    u8* slot = base + idx * 0x1C;
    *(u8*)(slot + 0x8) = 1;
    *(u32*)(slot + 0xC) = p5;
    *(u32*)(slot + 0x10) = p6;
    *(u32*)(slot + 0x14) = p7;
    *(u8*)(slot + 0x18) = p8;
    *(u32*)(slot + 0x1C) = p9;
    *(u8*)(slot + 0x20) = p10;
}

/* fn_800D7894 -- GSgfx_InitViewport | Size: 0xAC */
void fn_800D7894(void) {
    extern u8 lbl_803144D0[];
    extern u8 lbl_8047AAAC[];
    extern u8 lbl_8047AAB0[];
    extern u8 lbl_8047AAB4[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = *(u32*)lbl_8047AAB0;
    r3 = *(u32*)lbl_8047AAAC;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_800D7938;
L_800D78A8: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_800D7930;
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
    *(u32*)lbl_8047AAB4 = r0;
    if ((u32)r0 < (u32)0x8) goto L_800D78F0;
    r0 = 0x0;
    *(u32*)lbl_8047AAB4 = r0;
L_800D78F0: ;
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
L_800D7930: ;
    r3 = r3 + 0x190;
    if (--ctr != 0) goto L_800D78A8;
L_800D7938: ;
    r3 = 0x0;
    return;
}

/* fn_800D7940 | Size: 0x130 */
void fn_800D7940(void) {
    extern void fn_800D5724();
    extern void fn_800D579C();
    extern void fn_800D5A38();
    extern void fn_800D5AB0();
    extern void fn_800D5D6C();
    extern void fn_800D5DD0();
    extern void fn_800D5FA4();
    extern void fn_800D6028();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r4;
    r27 = r3;
    r3 = r28;
    fn_800D67BC();
    r31 = r28 & 0xFFFF;
    r28 = 0x0;
    goto L_800D7A4C;
L_800D796C: ;
    r0 = *(u32*)((u8*)r27 + 0x28);
    if ((s32)r0 != (s32)0x2) goto L_800D7984;
    r3 = r28;
    fn_800D6028();
    goto L_800D798C;
L_800D7984: ;
    r3 = r28 & 0xFF;
    fn_800D5FA4();
L_800D798C: ;
    r0 = *(u8*)((u8*)r27 + 0x40);
    if ((u32)r0 != (u32)0x1) goto L_800D79B8;
    r0 = *(u32*)((u8*)r27 + 0x44);
    if ((s32)r0 != (s32)0x2) goto L_800D79B0;
    r3 = r28;
    fn_800D5DD0();
    goto L_800D79B8;
L_800D79B0: ;
    r3 = r28 & 0xFF;
    fn_800D5D6C();
L_800D79B8: ;
    r29 = 0x4;
    r30 = r27 + 0x70;
L_800D79C0: ;
    r0 = *(u8*)((u8*)r30 + 0x8);
    if ((u32)r0 != (u32)0x1) goto L_800D79F0;
    r0 = *(u32*)((u8*)r30 + 0xC);
    if ((s32)r0 != (s32)0x2) goto L_800D79E8;
    r4 = r28;
    fn_800D5AB0();
    goto L_800D79F0;
L_800D79E8: ;
    r4 = r28 & 0xFF;
    fn_800D5A38();
L_800D79F0: ;
    r29 = r29 + 0x1;
    r30 = r30 + 0x1c;
    if ((s32)r29 <= (s32)0x5) goto L_800D79C0;
    r29 = 0x6;
    r30 = r27 + 0xa8;
L_800D7A08: ;
    r0 = *(u8*)((u8*)r30 + 0x8);
    if ((u32)r0 != (u32)0x1) goto L_800D7A38;
    r0 = *(u32*)((u8*)r30 + 0xC);
    if ((s32)r0 != (s32)0x2) goto L_800D7A30;
    r4 = r28;
    fn_800D579C();
    goto L_800D7A38;
L_800D7A30: ;
    r4 = r28 & 0xFF;
    fn_800D5724();
L_800D7A38: ;
    r29 = r29 + 0x1;
    r30 = r30 + 0x1c;
    if ((s32)r29 <= (s32)0xd) goto L_800D7A08;
    r28 = r28 + 0x1;
L_800D7A4C: ;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 < (u32)r31) goto L_800D796C;
    fn_800D6728();
    return;
}

/* fn_800D7A70 | Size: 0x110 */
void fn_800D7A70(void) {
    extern u8 lbl_80314370[];
    extern u8 lbl_803143A8[];
    extern u8 lbl_803143B4[];
    extern u8 lbl_803143D8[];
    extern u8 lbl_804001F0[];
    extern void fn_800B7874();
    extern void fn_800B7D3C();
    extern void fn_800B7D74();
    extern void fn_800B84E0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_80314370;
    r25 = r3;
    r31 = (u32)lbl_80314370;
    r4 = (u32)lbl_803143B4;
    r3 = (u32)lbl_803143D8;
    r28 = r25;
    r27 = r31;
    r29 = (u32)lbl_803143B4;
    r30 = (u32)lbl_803143D8;
    r26 = 0x0;
L_800D7AA8: ;
    r0 = *(u8*)((u8*)r28 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D7AE4;
    if ((s32)r26 == (s32)0x0) goto L_800D7AE4;
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
L_800D7AE4: ;
    r26 = r26 + 0x1;
    r27 = r27 + 0x4;
    r28 = r28 + 0x1c;
    if ((s32)r26 < (s32)0xe) goto L_800D7AA8;
    fn_800B7D3C();
    r3 = (u32)lbl_803143A8;
    r27 = r25;
    r28 = 0x0;
    r30 = (u32)lbl_803143A8;
L_800D7B0C: ;
    r0 = *(u8*)((u8*)r27 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800D7B44;
    r0 = *(u32*)((u8*)r27 + 0xC);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = r0 << 2;
    /* lwzx r4, r30, r0 */;
    fn_800B7874();
    r4 = *(u32*)((u8*)r27 + 0x1C);
    if ((u32)r4 == (u32)0x0) goto L_800D7B44;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r5 = *(u8*)((u8*)r27 + 0x20);
    fn_800B84E0();
L_800D7B44: ;
    r28 = r28 + 0x1;
    r31 = r31 + 0x4;
    r27 = r27 + 0x1c;
    if ((s32)r28 < (s32)0xe) goto L_800D7B0C;
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x14);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x14) = r0;
    return;
}

/* fn_800D7B80 -- GSgfx_InitProjection | Size: 0x78 */
void fn_800D7B80(void) {
    extern u8 lbl_8047AAA8[];
    extern u8 lbl_8047AAAC[];
    extern u8 lbl_8047AAB0[];
    extern u8 lbl_8047AAB4[];
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r0 = r3;
    r3 = r0 * 0x190;
    *(u32*)lbl_8047AAB0 = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAA8 = r3;
    if ((s32)r0 == (s32)0) goto L_800D7BE8;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AAAC = r3;
    r4 = r5;
    r6 = 0x0;
    goto L_800D7BD4;
L_800D7BC4: ;
    r3 = *(u32*)lbl_8047AAAC;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x190;
L_800D7BD4: ;
    r0 = *(u32*)lbl_8047AAB0;
    if ((u32)r6 < (u32)r0) goto L_800D7BC4;
    r0 = 0x0;
    *(u32*)lbl_8047AAB4 = r0;
L_800D7BE8: ;
    return;
}

/* fn_800D7BF8 | Size: 0x7C */
void fn_800D7BF8(void) {
    extern void fn_800D1A70();
    extern void fn_800D1B3C();
    extern void fn_800D1D00();
    extern void fn_800D2584();
    u32 r3 = 0;
    u32 mode = 0;

    mode = r3;
    fn_800D2584();
    if ((u32)r3 == 0) {
        r3 = 0;
        return;
    }
    switch ((s32)mode) {
    case 0:
        fn_800D1D00();
        return;
    case 1:
        fn_800D1B3C();
        return;
    case 2:
        fn_800D1A70();
        return;
    default:
        r3 = 0;
        return;
    }
}

/* fn_800D7C74 | Size: 0x9C */
void fn_800D7C74(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D7CA8;
    r3 = 0x45;
    r4 = 0x0;
    fn_800D4F98();
    goto L_800D7CFC;
L_800D7CA8: ;
    r0 = *(u8*)lbl_8047AAC8;
    if ((u32)r0 == (u32)0x0) goto L_800D7CFC;
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
L_800D7CFC: ;
    return;
}

/* fn_800D7D10 | Size: 0x80 */
void fn_800D7D10(void) {
    extern u8 lbl_80400948[];
    extern void fn_800D4F98();
    extern void fn_800DD970();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D7D48;
    r6 = r4;
    r5 = r3 & 0xFF;
    r3 = 0x44;
    r4 = 0x2;
    fn_800D4F98();
    goto L_800D7D80;
L_800D7D48: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 <= (u32)0x9) goto L_800D7D68;
    r3 = (u32)lbl_80270440;
    fn_800DD970();
    goto L_800D7D80;
L_800D7D68: ;
    r6 = r0 * 0x30;
    r5 = (u32)lbl_80400948;
    r3 = r4;
    r0 = (u32)lbl_80400948;
    r4 = r0 + r6;
    fn_800E0628();
L_800D7D80: ;
    return;
}

/* fn_800D7D90 | Size: 0xCC */
void fn_800D7D90(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800D4F98();
    extern void fn_800DD970();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r4;
    r29 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D7DDC;
    r6 = r30;
    r5 = r29 & 0xFF;
    r3 = 0x43;
    r4 = 0x11;
    fn_800D4F98();
    goto L_800D7E40;
L_800D7DDC: ;
    r31 = r29 & 0xFF;
    if ((u32)r31 <= (u32)0x9) goto L_800D7DFC;
    r3 = (u32)lbl_80270440;
    fn_800DD970();
    goto L_800D7E40;
L_800D7DFC: ;
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
    if ((u32)r0 != (u32)0x9) goto L_800D7E40;
    r0 = 0x1;
    *(u8*)lbl_8047AAC8 = r0;
L_800D7E40: ;
    return;
}

/* fn_800D7E5C | Size: 0xB8 */
void fn_800D7E5C(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC4[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800DD970();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D7E90;
    r3 = 0x42;
    r4 = 0x0;
    fn_800D4F98();
    goto L_800D7F00;
L_800D7E90: ;
    r3 = *(u32*)lbl_8047AAC0;
    r0 = *(u32*)lbl_8047AAC4;
    if ((u32)r3 < (u32)r0) goto L_800D7EB4;
    r3 = (u32)lbl_80270460;
    fn_800DD970();
    goto L_800D7F00;
L_800D7EB4: ;
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
L_800D7F00: ;
    return;
}

/* fn_800D7F14 | Size: 0xD0 */
void fn_800D7F14(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AABC[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800DD970();
    extern void fn_800E0290();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D7F4C;
    r5 = r3;
    r3 = 0x41;
    r4 = 0x10;
    fn_800D4F98();
    goto L_800D7FD0;
L_800D7F4C: ;
    r4 = *(u32*)lbl_8047AAC0;
    r0 = *(u32*)lbl_8047AABC;
    if ((u32)r4 > (u32)r0) goto L_800D7F70;
    r3 = (u32)lbl_80270480;
    fn_800DD970();
    goto L_800D7FD0;
L_800D7F70: ;
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
L_800D7FD0: ;
    return;
}

/* fn_800D7FE4 | Size: 0xA4 */
void fn_800D7FE4(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800E0290();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D801C;
    r5 = r3;
    r3 = 0x40;
    r4 = 0x10;
    fn_800D4F98();
    goto L_800D8074;
L_800D801C: ;
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
L_800D8074: ;
    return;
}

/* fn_800D8088 | Size: 0xCC */
void fn_800D8088(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AABC[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800DD970();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D80C0;
    r5 = r3;
    r3 = 0x3f;
    r4 = 0x10;
    fn_800D4F98();
    goto L_800D8140;
L_800D80C0: ;
    r4 = *(u32*)lbl_8047AAC0;
    r0 = *(u32*)lbl_8047AABC;
    if ((u32)r4 > (u32)r0) goto L_800D80E4;
    r3 = (u32)lbl_80270480;
    fn_800DD970();
    goto L_800D8140;
L_800D80E4: ;
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
L_800D8140: ;
    return;
}

/* fn_800D8154 | Size: 0x98 */
void fn_800D8154(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800E02C4();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D8188;
    r3 = 0x3e;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto L_800D81D8;
L_800D8188: ;
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
L_800D81D8: ;
    return;
}

/* fn_800D81EC | Size: 0x98 */
void fn_800D81EC(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800E03E8();
    extern void fn_800E0628();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D8220;
    r3 = 0x3d;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto L_800D8270;
L_800D8220: ;
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
L_800D8270: ;
    return;
}

/* fn_800D8284 | Size: 0xC8 */
void fn_800D8284(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800E02E8();
    extern void fn_800E032C();
    extern void fn_800E0370();
    extern void fn_800E0628();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x18) = f31;
    f31 = f3;
    *(f64*)(sp + 0x10) = f30;
    f30 = f2;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D82C8;
    r3 = 0x3c;
    r4 = 0xd;
    /* crset cr1eq */;
    fn_800D4F98();
    goto L_800D8330;
L_800D82C8: ;
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
L_800D8330: ;
    f31 = *(f64*)(sp + 0x18);
    f30 = *(f64*)(sp + 0x10);
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* fn_800D834C | Size: 0x98 */
void fn_800D834C(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800D4F98();
    extern void fn_800E0628();
    extern void fn_800E064C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D8380;
    r3 = 0x3b;
    r4 = 0x0;
    fn_800D4F98();
    goto L_800D83D0;
L_800D8380: ;
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
L_800D83D0: ;
    return;
}

/* fn_800D83E4 -- GSgfx_InitMatrixStack | Size: 0xA8 */
void fn_800D83E4(void) {
    extern u8 lbl_80314610[];
    extern u8 lbl_80400948[];
    extern u8 lbl_8047AAB8[];
    extern u8 lbl_8047AABC[];
    extern u8 lbl_8047AAC0[];
    extern u8 lbl_8047AAC4[];
    extern u8 lbl_8047AAC8[];
    extern void fn_800BD4B4();
    extern void fn_800BD504();
    extern void fn_800BD554();
    extern void fn_800E0628();
    extern void fn_800E064C();
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = r31 * 0x30;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAB8 = r3;
    if ((s32)r0 == (s32)0) goto L_800D8478;
    r3 = r0;
    fn_800E27B0();
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
L_800D8478: ;
    return;
}

/* fn_800D848C | Size: 0x148 */
void fn_800D848C(void) {
    extern u8 lbl_80314404[];
    extern u8 lbl_80314424[];
    extern u8 lbl_80314454[];
    extern u8 lbl_803144A8[];
    extern void fn_800B857C();
    extern void fn_800BD58C();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r8 = r6;
    r31 = r5;
    r30 = r4;
    r29 = r3;
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r7 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D84E4;
    r5 = r29;
    r6 = r30;
    r7 = r31;
    r3 = 0x27;
    r4 = 0x13;
    fn_800D4F98();
    goto L_800D85B8;
L_800D84E4: ;
    if ((s32)r30 != (s32)0x0) goto L_800D8524;
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
    goto L_800D85B8;
L_800D8524: ;
    if ((s32)r30 != (s32)0x1) goto L_800D854C;
    r3 = (u32)lbl_803144A8;
    r0 = r29 << 2;
    r4 = (u32)lbl_803144A8;
    r3 = r8;
    /* lwzx r4, r4, r0 */;
    r5 = 0x0;
    fn_800BD58C();
    goto L_800D8570;
L_800D854C: ;
    if ((s32)r30 != (s32)0x2) goto L_800D8570;
    r3 = (u32)lbl_803144A8;
    r0 = r29 << 2;
    r4 = (u32)lbl_803144A8;
    r3 = r8;
    /* lwzx r4, r4, r0 */;
    r5 = 0x1;
    fn_800BD58C();
L_800D8570: ;
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
L_800D85B8: ;
    return;
}

/* fn_800D85D4 | Size: 0x1D8 */
void fn_800D85D4(void) {
    extern u8 lbl_803144F0[];
    extern u8 lbl_80314510[];
    extern u8 lbl_80314530[];
    extern u8 lbl_804001F0[];
    extern u8 lbl_8047CA40[];
    extern u8 lbl_8047CA48[];
    extern void fn_800BACA0();
    extern void fn_800BAE34();
    extern void fn_800BAFFC();
    extern void fn_800BB098();
    extern void fn_800D4F98();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    if ((s32)r0 == (s32)0) goto L_800D8790;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D8620;
    r5 = r3;
    r6 = r29;
    r3 = 0x26;
    r4 = 0x2;
    fn_800D4F98();
    goto L_800D8790;
L_800D8620: ;
    r31 = r3 << 2;
    r3 = r4 + r31;
    r0 = *(u32*)((u8*)r3 + 0x28);
    if ((u32)r0 == (u32)r29) goto L_800D8790;
    r0 = *(u8*)((u8*)r29 + 0x7);
    if ((u32)r0 == (u32)0x0) goto L_800D873C;
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
    if ((s32)r0 == (s32)0x1) goto L_800D86AC;
    if ((s32)r0 >= (s32)0x1) goto L_800D8684;
    if ((s32)r0 >= (s32)0x0) goto L_800D8690;
    goto L_800D86E0;
L_800D8684: ;
    if ((s32)r0 >= (s32)0x3) goto L_800D86E0;
    goto L_800D86C8;
L_800D8690: ;
    r0 = *(u32*)((u8*)r29 + 0x18);
    if ((s32)r0 != (s32)0x2) goto L_800D86A4;
    r30 = 0x1;
    goto L_800D86E0;
L_800D86A4: ;
    r30 = 0x0;
    goto L_800D86E0;
L_800D86AC: ;
    r0 = *(u32*)((u8*)r29 + 0x18);
    if ((s32)r0 != (s32)0x2) goto L_800D86C0;
    r30 = 0x3;
    goto L_800D86E0;
L_800D86C0: ;
    r30 = 0x2;
    goto L_800D86E0;
L_800D86C8: ;
    r0 = *(u32*)((u8*)r29 + 0x18);
    if ((s32)r0 != (s32)0x2) goto L_800D86DC;
    r30 = 0x5;
    goto L_800D86E0;
L_800D86DC: ;
    r30 = 0x4;
L_800D86E0: ;
    r3 = *(u8*)((u8*)r29 + 0x5);
    r0 = (0x4330 << 16);
    r5 = *(u32*)((u8*)r29 + 0x1C);
    r4 = r30;
    f1 = *(f32*)lbl_8047CA40;
    /* xoris r3, r3, 0x8000 */;
    r5 = 0x2 - r5;
    /* cntlzw r5, r5 */;
    f2 = *(f64*)lbl_8047CA48;
    f3 = f1;
    *(u32*)(sp + 0x8) = r0;
    r3 = r29 + 0x54;
    r5 = (u32)r5 >> 5;
    r6 = 0x0;
    f0 = *(f64*)(sp + 0x8);
    r7 = 0x0;
    r8 = 0x0;
    f2 = f0 - f2;
    fn_800BACA0();
    r0 = 0x0;
    *(u8*)((u8*)r29 + 0x7) = r0;
L_800D873C: ;
    r0 = *(u32*)lbl_8047AA80;
    r3 = r0 + r31;
    *(u32*)((u8*)r3 + 0x28) = r29;
    r0 = *(u32*)((u8*)r29 + 0x48);
    if ((u32)r0 == (u32)0x0) goto L_800D8768;
    r4 = (u32)lbl_80314510;
    r3 = r29 + 0x74;
    r4 = (u32)lbl_80314510;
    /* lwzx r4, r4, r31 */;
    fn_800BB098();
L_800D8768: ;
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
L_800D8790: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* fn_800D87AC -- GSgfx_SetInternalMode | Size: 0xE0 */
void fn_800D87AC(void) {
    extern u8 lbl_80314404[];
    extern void fn_800B857C();
    extern void fn_800BBC0C();
    extern void fn_800BBC34();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r27 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x414);
    r0 = r3 | r27;
    *(u32*)((u8*)r4 + 0x414) = r0;
    if ((s32)r0 == (s32)0) goto L_800D8828;
    r3 = (u32)lbl_80314404;
    r30 = 0x0;
    r29 = (u32)lbl_80314404;
    r28 = 0x0;
    r31 = r30;
L_800D87EC: ;
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
    r30 = r30 + 0x4;
    if ((s32)r28 < (s32)0x8) goto L_800D87EC;
L_800D8828: ;
    if ((s32)r28 == (s32)0x8) goto L_800D8878;
    r29 = 0x0;
    r31 = r29;
L_800D8838: ;
    r3 = *(u32*)lbl_8047AA80;
    r4 = r29 + 0x25c;
    /* lbzx r0, r3, r4 */;
    if ((u32)r0 == (u32)0x0) goto L_800D8858;
    /* stbx r31, r3, r4 */;
    r3 = r29;
    fn_800BBC34();
L_800D8858: ;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x10) goto L_800D8838;
    r4 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    r3 = 0x0;
    *(u8*)((u8*)r4 + 0x3AC) = r0;
    fn_800BBC0C();
L_800D8878: ;
    return;
}

/* fn_800D888C | Size: 0x50 */
void fn_800D888C(u32 mask) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x29, 0x1, mask);
    } else {
        *(u32*)(state + 0x10) = *(u32*)(state + 0x10) & ~mask;
    }
}

/* fn_800D88DC | Size: 0x50 */
void fn_800D88DC(u32 mask) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x28, 0x1, mask);
    } else {
        *(u32*)(state + 0x10) = *(u32*)(state + 0x10) | mask;
    }
}

/* fn_800D923C | Size: 0x400 */
void fn_800D923C(void) {
    extern u8 lbl_804001F0[];
    extern void fn_800B884C();
    extern void fn_800BA6B0();
    extern void fn_800BA6F4();
    extern void fn_800BB780();
    extern void fn_800BB81C();
    extern void fn_800BB97C();
    extern void fn_800BBAF8();
    extern void fn_800BBC0C();
    extern void fn_800BBC34();
    extern void fn_800BBC7C();
    extern void fn_800BBCE0();
    extern void fn_800BBE8C();
    extern void fn_800BBF98();
    extern void fn_800BBFDC();
    extern void fn_800BC1A0();
    extern void fn_800BC1E4();
    extern void fn_800BC228();
    extern void fn_800BC290();
    extern void fn_800BC3E0();
    extern void fn_800BC454();
    extern void fn_800BC4C0();
    extern void fn_800BC580();
    extern void fn_800BC6F0();
    extern void fn_800BC8C8();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    r0 = r0 & 0x1;
    if ((s32)r0 == (s32)0) goto L_800D92E4;
    r3 = *(u8*)((u8*)r3 + 0x60);
    fn_800BA6B0();
    r23 = 0x0;
    r22 = 0x0;
    goto L_800D92C0;
L_800D9270: ;
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
L_800D92C0: ;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r4 + 0x60);
    if ((s32)r23 < (s32)r0) goto L_800D9270;
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x18);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x18) = r0;
L_800D92E4: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    if ((s32)r23 == (s32)r0) goto L_800D9310;
    r3 = *(u8*)((u8*)r3 + 0x79);
    fn_800B884C();
    r3 = (u32)lbl_804001F0;
    r4 = (u32)lbl_804001F0;
    r3 = *(u32*)((u8*)r4 + 0x1C);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r4 + 0x1C) = r0;
L_800D9310: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x414);
    if ((s32)r23 == (s32)r0) goto L_800D961C;
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
    if ((u32)r23 == (u32)0x0) goto L_800D93C4;
    r3 = *(u32*)lbl_8047AA80;
    r30 = 0x0;
    r31 = r3 + 0x3ad;
    r21 = r3 + 0x3c0;
    goto L_800D9398;
L_800D9370: ;
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
L_800D9398: ;
    if ((s32)r30 < (s32)r23) goto L_800D9370;
    r30 = 0x0;
L_800D93A4: ;
    r5 = *(u8*)((u8*)r21 + 0x18);
    r4 = r21;
    r3 = r30 + 0x1;
    fn_800BB81C();
    r30 = r30 + 0x1;
    r21 = r21 + 0x1c;
    if ((s32)r30 < (s32)0x3) goto L_800D93A4;
L_800D93C4: ;
    r30 = 0x0;
    r31 = r30;
    goto L_800D95AC;
L_800D93D0: ;
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
    if ((u32)r0 == (u32)0x0) goto L_800D9580;
    if ((u32)r23 == (u32)0x0) goto L_800D9580;
    r0 = *(u8*)((u8*)r22 + 0x0);
    if ((s32)r0 == (s32)0x3) goto L_800D954C;
    if ((s32)r0 >= (s32)0x3) goto L_800D94B0;
    if ((s32)r0 == (s32)0x1) goto L_800D94F8;
    if ((s32)r0 >= (s32)0x1) goto L_800D9514;
    if ((s32)r0 >= (s32)0x0) goto L_800D94C0;
    goto L_800D9588;
L_800D94B0: ;
    if ((s32)r0 == (s32)0x5) goto L_800D9574;
    if ((s32)r0 >= (s32)0x5) goto L_800D9588;
    goto L_800D9560;
L_800D94C0: ;
    r0 = *(u8*)((u8*)r22 + 0x9);
    r3 = r30;
    *(u32*)(sp + 0x8) = r0;
    r0 = *(u8*)((u8*)r22 + 0x5);
    *(u32*)(sp + 0xC) = r0;
    r4 = *(u8*)((u8*)r22 + 0x1);
    r5 = *(u8*)((u8*)r22 + 0x3);
    r6 = *(u8*)((u8*)r22 + 0x4);
    r7 = *(u8*)((u8*)r22 + 0x2);
    r8 = *(u8*)((u8*)r22 + 0x6);
    r9 = *(u8*)((u8*)r22 + 0x7);
    r10 = *(u8*)((u8*)r22 + 0x8);
    fn_800BB780();
    goto L_800D9588;
L_800D94F8: ;
    r4 = *(u8*)((u8*)r22 + 0x1);
    r3 = r30;
    r5 = *(u8*)((u8*)r22 + 0xA);
    r6 = *(u8*)((u8*)r22 + 0xB);
    r7 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBC7C();
    goto L_800D9588;
L_800D9514: ;
    r0 = *(u8*)((u8*)r22 + 0x4);
    r3 = r30;
    *(u32*)(sp + 0x8) = r0;
    r0 = *(u8*)((u8*)r22 + 0x5);
    *(u32*)(sp + 0xC) = r0;
    r4 = *(u8*)((u8*)r22 + 0x1);
    r5 = *(u16*)((u8*)r22 + 0xC);
    r6 = *(u16*)((u8*)r22 + 0xE);
    r7 = *(u16*)((u8*)r22 + 0x10);
    r8 = *(u16*)((u8*)r22 + 0x12);
    r9 = *(u8*)((u8*)r22 + 0x3);
    r10 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBCE0();
    goto L_800D9588;
L_800D954C: ;
    r4 = *(u8*)((u8*)r22 + 0x1);
    r3 = r30;
    r5 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBE8C();
    goto L_800D9588;
L_800D9560: ;
    r4 = *(u8*)((u8*)r22 + 0x1);
    r3 = r30;
    r5 = *(u8*)((u8*)r22 + 0x2);
    fn_800BBF98();
    goto L_800D9588;
L_800D9574: ;
    r3 = r30;
    fn_800BBFDC();
    goto L_800D9588;
L_800D9580: ;
    r3 = r30;
    fn_800BBC34();
L_800D9588: ;
    r29 = r29 + 0x3;
    r28 = r28 + 0x5;
    r27 = r27 + 0x5;
    r26 = r26 + 0x4;
    r25 = r25 + 0x4;
    r24 = r24 + 0x1;
    r22 = r22 + 0x14;
    r31 = r31 + 0x4;
    r30 = r30 + 0x1;
L_800D95AC: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x7A);
    if ((s32)r30 < (s32)r0) goto L_800D93D0;
    r23 = 0x0;
    r22 = r23;
L_800D95C4: ;
    r5 = *(u32*)lbl_8047AA80;
    r0 = r22 + 0x24c;
    r3 = r23;
    r4 = (u32)sp + 0x10;
    /* lwzx r0, r5, r0 */;
    *(u32*)(sp + 0x10) = r0;
    fn_800BC3E0();
    r23 = r23 + 0x1;
    r22 = r22 + 0x4;
    if ((s32)r23 < (s32)0x4) goto L_800D95C4;
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
L_800D961C: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x414) = r0;
    return;
}

/* fn_800D963C | Size: 0x4B4 */
void fn_800D963C(void) {
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    if ((s32)r4 == (s32)0x2) goto L_800D9928;
    if ((s32)r4 >= (s32)0x2) goto L_800D9668;
    if ((s32)r4 == (s32)0x0) goto L_800D9678;
    if ((s32)r4 >= (s32)0x0) goto L_800D975C;
    goto L_800D9ADC;
L_800D9668: ;
    if ((s32)r4 == (s32)0x4) goto L_800D9A04;
    if ((s32)r4 >= (s32)0x4) goto L_800D9ADC;
    goto L_800D9840;
L_800D9678: ;
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
    goto L_800D9ADC;
L_800D975C: ;
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
    goto L_800D9ADC;
L_800D9840: ;
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
    goto L_800D9ADC;
L_800D9928: ;
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
    goto L_800D9ADC;
L_800D9A04: ;
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
L_800D9ADC: ;
    return;
}

/* fn_800D9AF0 | Size: 0x34 */
void fn_800D9AF0(u16* out_x, u16* out_y, u16* out_w, u16* out_h) {
    u8* state = (u8*)lbl_8047AA80;
    *out_x = *(u16*)(state + 0x476);
    *out_y = *(u16*)(state + 0x478);
    *out_w = *(u16*)(state + 0x47A);
    *out_h = *(u16*)(state + 0x47C);
}

/* fn_800D9B24 | Size: 0x34 */
void fn_800D9B24(u16* out_x, u16* out_y, u16* out_w, u16* out_h) {
    u8* state = (u8*)lbl_8047AA80;
    *out_x = *(u16*)(state + 0x46E);
    *out_y = *(u16*)(state + 0x470);
    *out_w = *(u16*)(state + 0x472);
    *out_h = *(u16*)(state + 0x474);
}

/* fn_800D9B58 | Size: 0x78 */
void fn_800D9B58(f32 fp1, f32 fp2, f32 fp3, f32 fp4) {
    extern u8 lbl_8047CA50[];
    extern u8 lbl_8047CA54[];
    extern void fn_800BD2E0();
    extern void fn_800D4F98();
    extern void fn_800D834C();
    extern void fn_800E0698();
    u8* state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x3a, 0xe);
    } else {
        fn_800E0698();
        fn_800BD2E0();
        fn_800D834C();
    }
}


/* fn_800D9BD0 | Size: 0x54 */
void fn_800D9BD0(void) {
    extern void fn_800BD2E0();
    extern void fn_800D4F98();
    extern void fn_800E0678();
    u8 local[0x48];
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x39, 0xE);
    } else {
        fn_800E0678(local);
        fn_800BD2E0(local, 0);
    }
}

/* fn_800D9C24 -- GSgfx_SetViewportRect | Size: 0x144 */
void fn_800D9C24(void) {
    extern u8 lbl_8047CA50[];
    extern u8 lbl_8047CA58[];
    extern u8 lbl_8047CA60[];
    extern u8 lbl_8047CA68[];
    extern void fn_800D21C8();
    extern void fn_800D2584();
    extern void fn_800D3EC4();
    extern void fn_800D4F98();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;

    r31 = r6;
    r30 = r5;
    r29 = r4;
    r28 = r3;
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r7 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D9C84;
    r5 = r28 & 0xFFFF;
    r6 = r29 & 0xFFFF;
    r7 = r30 & 0xFFFF;
    r8 = r31 & 0xFFFF;
    r3 = 0x38;
    r4 = 0x4;
    fn_800D4F98();
    goto L_800D9D48;
L_800D9C84: ;
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
    f0 = *(f64*)(sp + 0x8);
    r3 = 0x0;
    f1 = f0 - f2;
    f5 = *(f32*)lbl_8047CA50;
    f6 = *(f32*)lbl_8047CA58;
    f0 = *(f64*)(sp + 0x10);
    f2 = f0 - f2;
    f0 = *(f64*)(sp + 0x18);
    *(u32*)(sp + 0x24) = r0;
    f3 = f0 - f4;
    f0 = *(f64*)(sp + 0x20);
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
    if ((u32)r3 == (u32)0x0) goto L_800D9D48;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = r31;
    fn_800D21C8();
L_800D9D48: ;
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    r29 = *(u32*)(sp + 0x34);
    r28 = *(u32*)(sp + 0x30);
    return;
}

/* fn_800D9D68 -- GSgfx_SetScissor | Size: 0xE4 */
void fn_800D9D68(void) {
    extern void fn_800BD7A0();
    extern void fn_800D2150();
    extern void fn_800D2584();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r6;
    r30 = r5;
    r29 = r4;
    r28 = r3;
    r7 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r7 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800D9DC8;
    r5 = r28 & 0xFFFF;
    r6 = r29 & 0xFFFF;
    r7 = r30 & 0xFFFF;
    r8 = r31 & 0xFFFF;
    r3 = 0x37;
    r4 = 0x4;
    fn_800D4F98();
    goto L_800D9E2C;
L_800D9DC8: ;
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
    if ((u32)r3 == (u32)0x0) goto L_800D9E2C;
    r4 = r28;
    r5 = r29;
    r6 = r30;
    r7 = r31;
    fn_800D2150();
L_800D9E2C: ;
    return;
}

/* fn_800D9E4C | Size: 0x8C */
void fn_800D9E4C(void) {
    extern u8 lbl_8047AA8C[];
    extern void fn_800D4F98();
    extern void fn_8019BD18();
    u32 r3 = 0;
    u8* state;

    state = (u8*)*(u32*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x36, 1, r3);
        return;
    }
    if ((s32)r3 == 1) {
        state[0x5D] = 1;
    } else if ((s32)r3 == 0) {
        state[0x5D] = 0;
    }
    state = (u8*)*(u32*)lbl_8047AA80;
    if (state[0x5D] == 1) {
        fn_8019BD18(*(u32*)lbl_8047AA8C);
    } else {
        fn_8019BD18(0);
    }
    return;
}

/* fn_800D9ED8 | Size: 0x68 */
void fn_800D9ED8(void) {
    extern void fn_800D4F98();
    u32 r3 = 0;
    u8* state = (u8*)*(u32*)lbl_8047AA80;

    if (*(s32*)state == 1) {
        fn_800D4F98(0x35, 1, r3);
        return;
    }
    if ((s32)r3 == 1) {
        state[0x1A] = 1;
    } else if ((s32)r3 == 0) {
        state[0x1A] = 0;
    }
}

/* fn_800D9F40 -- GSgfx_ConfigureFog | Size: 0x74 */
void fn_800D9F40(void) {
    extern void fn_800B953C();
    extern void fn_800D4F98();
    u32 r3 = 0;
    u8* state = (u8*)*(u32*)lbl_8047AA80;

    if (*(s32*)state == 1) {
        fn_800D4F98(0x34, 1, r3);
        return;
    }
    if ((s32)r3 == 1) {
        state[0x42D] = 1;
    } else if ((s32)r3 == 0) {
        state[0x42D] = 0;
    }
    fn_800B953C(*(u8*)((u8*)*(u32*)lbl_8047AA80 + 0x42D));
}

/* fn_800D9FB4 | Size: 0x74 */
void fn_800D9FB4(void) {
    extern void fn_800BD870();
    extern void fn_800D4F98();
    u32 r3 = 0;
    u8* state = (u8*)*(u32*)lbl_8047AA80;

    if (*(s32*)state == 1) {
        fn_800D4F98(0x33, 1, r3);
        return;
    }
    if ((s32)r3 == 1) {
        state[0x42C] = 0;
    } else if ((s32)r3 == 0) {
        state[0x42C] = 1;
    }
    fn_800BD870(*(u8*)((u8*)*(u32*)lbl_8047AA80 + 0x42C));
}

/* fn_800DA028 -- GSgfx_ConfigureTEV | Size: 0x64 */
void fn_800DA028(u32 param1) {
    extern u8 lbl_8031453C[];
    extern void fn_800B94F0();
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x32, 0x1, param1);
    } else {
        *(u32*)(param1 + 0x428) = (0x32 << 2);
        fn_800B94F0();
    }
}


/* fn_800DA08C | Size: 0x74 */
void fn_800DA08C(void) {
    extern void fn_800BCFDC();
    extern void fn_800D4F98();
    u32 r3 = 0;
    u8* state = (u8*)*(u32*)lbl_8047AA80;

    if (*(s32*)state == 1) {
        fn_800D4F98(0x31, 1, r3);
        return;
    }
    if ((s32)r3 == 1) {
        state[0x426] = 1;
    } else if ((s32)r3 == 0) {
        state[0x426] = 0;
    }
    fn_800BCFDC(*(u8*)((u8*)*(u32*)lbl_8047AA80 + 0x426));
}

/* fn_800DA100 -- GSgfx_ConfigureAlpha | Size: 0xE8 */
void fn_800DA100(void) {
    extern u8 lbl_8031456C[];
    extern u8 lbl_8031457C[];
    extern void fn_800BC618();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;

    r12 = r5;
    r11 = r6;
    r10 = r8;
    r9 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r9 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DA154;
    r5 = r3;
    r6 = r4;
    r8 = r11;
    r9 = r7;
    r7 = r12 & 0xFF;
    r10 = r10 & 0xFF;
    r3 = 0x30;
    r4 = 0x6;
    fn_800D4F98();
    goto L_800DA1D8;
L_800DA154: ;
    if ((s32)r3 != (s32)0x1) goto L_800DA168;
    r0 = 0x1;
    *(u8*)((u8*)r9 + 0x425) = r0;
    goto L_800DA178;
L_800DA168: ;
    if ((s32)r3 != (s32)0x0) goto L_800DA178;
    r0 = 0x0;
    *(u8*)((u8*)r9 + 0x425) = r0;
L_800DA178: ;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x425);
    if ((u32)r0 != (u32)0x0) goto L_800DA1A4;
    r3 = 0x7;
    r4 = 0x0;
    r5 = 0x1;
    r6 = 0x7;
    r7 = 0x0;
    fn_800BC618();
    goto L_800DA1D8;
L_800DA1A4: ;
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
L_800DA1D8: ;
    return;
}

/* fn_800DA1E8 -- GSgfx_ConfigureZ | Size: 0xD4 */
void fn_800DA1E8(void) {
    extern u8 lbl_8031454C[];
    extern void fn_800BCE88();
    extern void fn_800BCEBC();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DA224;
    r5 = r3;
    r6 = r4;
    r3 = 0x2f;
    r4 = 0x3;
    fn_800D4F98();
    goto L_800DA2AC;
L_800DA224: ;
    if ((s32)r3 != (s32)0x1) goto L_800DA238;
    r0 = 0x1;
    *(u8*)((u8*)r6 + 0x41C) = r0;
    goto L_800DA248;
L_800DA238: ;
    if ((s32)r3 != (s32)0x0) goto L_800DA248;
    r0 = 0x0;
    *(u8*)((u8*)r6 + 0x41C) = r0;
L_800DA248: ;
    if ((s32)r7 != (s32)0x1) goto L_800DA260;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x424) = r0;
    goto L_800DA274;
L_800DA260: ;
    if ((s32)r7 != (s32)0x0) goto L_800DA274;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x424) = r0;
L_800DA274: ;
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
L_800DA2AC: ;
    return;
}

/* fn_800DA2BC -- GSgfx_ConfigureBlend | Size: 0xF4 */
void fn_800DA2BC(void) {
    extern void fn_800BCE30();
    extern void fn_800BCE5C();
    extern void fn_800BCE88();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DA2F8;
    r5 = r3;
    r6 = r4;
    r3 = 0x2e;
    r4 = 0x3;
    fn_800D4F98();
    goto L_800DA3A0;
L_800DA2F8: ;
    if ((s32)r3 != (s32)0x1) goto L_800DA30C;
    r0 = 0x1;
    *(u8*)((u8*)r6 + 0x419) = r0;
    goto L_800DA31C;
L_800DA30C: ;
    if ((s32)r3 != (s32)0x0) goto L_800DA31C;
    r0 = 0x0;
    *(u8*)((u8*)r6 + 0x419) = r0;
L_800DA31C: ;
    if ((s32)r4 != (s32)0x1) goto L_800DA334;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x41A) = r0;
    goto L_800DA348;
L_800DA334: ;
    if ((s32)r4 != (s32)0x0) goto L_800DA348;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x41A) = r0;
L_800DA348: ;
    if ((s32)r7 != (s32)0x1) goto L_800DA360;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x1;
    *(u8*)((u8*)r3 + 0x41B) = r0;
    goto L_800DA374;
L_800DA360: ;
    if ((s32)r7 != (s32)0x0) goto L_800DA374;
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x0;
    *(u8*)((u8*)r3 + 0x41B) = r0;
L_800DA374: ;
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
L_800DA3A0: ;
    return;
}

/* fn_800DA3B0 | Size: 0x78 */
void fn_800DA3B0(void) {
    extern void fn_800BD008();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DA3E8;
    r5 = r3;
    r6 = r4 & 0xFF;
    r3 = 0x2d;
    r4 = 0x2;
    fn_800D4F98();
    goto L_800DA418;
L_800DA3E8: ;
    if ((s32)r3 != (s32)0x1) goto L_800DA3FC;
    r0 = 0x1;
    *(u8*)((u8*)r5 + 0x418) = r0;
    goto L_800DA40C;
L_800DA3FC: ;
    if ((s32)r3 != (s32)0x0) goto L_800DA40C;
    r0 = 0x0;
    *(u8*)((u8*)r5 + 0x418) = r0;
L_800DA40C: ;
    r3 = *(u32*)lbl_8047AA80;
    r3 = *(u8*)((u8*)r3 + 0x418);
    fn_800BD008();
L_800DA418: ;
    return;
}

/* fn_800DA428 | Size: 0x9C */
void fn_800DA428(void) {
    extern u8 lbl_803145D0[];
    extern void fn_800BCDDC();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DA45C;
    r5 = r3;
    r3 = 0x2c;
    r4 = 0x1;
    fn_800D4F98();
    goto L_800DA4B4;
L_800DA45C: ;
    if ((s32)r3 != (s32)0x0) goto L_800DA488;
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0x5;
    fn_800BCDDC();
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x10;
    *(u32*)((u8*)r3 + 0x8) = r0;
    goto L_800DA4B4;
L_800DA488: ;
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
L_800DA4B4: ;
    return;
}

/* fn_800DA4C4 | Size: 0xB4 */
void fn_800DA4C4(void) {
    extern u8 lbl_8031459C[];
    extern u8 lbl_803145A8[];
    extern void fn_800BCDDC();
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DA500;
    r5 = r3;
    r6 = r4;
    r3 = 0x2b;
    r4 = 0x3;
    fn_800D4F98();
    goto L_800DA568;
L_800DA500: ;
    if ((s32)r3 != (s32)0x0) goto L_800DA52C;
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0x5;
    fn_800BCDDC();
    r3 = *(u32*)lbl_8047AA80;
    r0 = 0x10;
    *(u32*)((u8*)r3 + 0x8) = r0;
    goto L_800DA568;
L_800DA52C: ;
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
L_800DA568: ;
    return;
}

/* fn_800DA578 | Size: 0x178 */
void fn_800DA578(void) {
    extern void fn_800DA6F0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r8 = r4;
    r27 = r3;
    r28 = r6;
    r29 = r7;
    r30 = r8 + r5;
    goto L_800DA6D4;
L_800DA5A0: ;
    r0 = *(u8*)((u8*)r8 + 0x0);
    r8 = r8 + 0x1;
    if ((s32)r0 == (s32)0x98) goto L_800DA618;
    if ((s32)r0 >= (s32)0x98) goto L_800DA5E8;
    if ((s32)r0 == (s32)0x80) goto L_800DA618;
    if ((s32)r0 >= (s32)0x80) goto L_800DA5DC;
    if ((s32)r0 == (s32)0x61) goto L_800DA6C8;
    if ((s32)r0 >= (s32)0x61) goto L_800DA6DC;
    if ((s32)r0 == (s32)0x0) goto L_800DA6D4;
    goto L_800DA6DC;
L_800DA5DC: ;
    if ((s32)r0 == (s32)0x90) goto L_800DA618;
    goto L_800DA6DC;
L_800DA5E8: ;
    if ((s32)r0 == (s32)0xb0) goto L_800DA618;
    if ((s32)r0 >= (s32)0xb0) goto L_800DA60C;
    if ((s32)r0 == (s32)0xa8) goto L_800DA618;
    if ((s32)r0 >= (s32)0xa8) goto L_800DA6DC;
    if ((s32)r0 == (s32)0xa0) goto L_800DA618;
    goto L_800DA6DC;
L_800DA60C: ;
    if ((s32)r0 == (s32)0xb8) goto L_800DA618;
    goto L_800DA6DC;
L_800DA618: ;
    r6 = *(u16*)((u8*)r8 + 0x0);
    r8 = r8 + 0x2;
    if ((s32)r0 == (s32)0xa0) goto L_800DA69C;
    if ((s32)r0 >= (s32)0xa0) goto L_800DA650;
    if ((s32)r0 == (s32)0x90) goto L_800DA68C;
    if ((s32)r0 >= (s32)0x90) goto L_800DA644;
    if ((s32)r0 == (s32)0x80) goto L_800DA6A4;
    goto L_800DA6A8;
L_800DA644: ;
    if ((s32)r0 == (s32)0x98) goto L_800DA694;
    goto L_800DA6A8;
L_800DA650: ;
    if ((s32)r0 == (s32)0xb0) goto L_800DA684;
    if ((s32)r0 >= (s32)0xb0) goto L_800DA668;
    if ((s32)r0 == (s32)0xa8) goto L_800DA67C;
    goto L_800DA6A8;
L_800DA668: ;
    if ((s32)r0 == (s32)0xb8) goto L_800DA674;
    goto L_800DA6A8;
L_800DA674: ;
    r31 = 0x0;
    goto L_800DA6A8;
L_800DA67C: ;
    r31 = 0x1;
    goto L_800DA6A8;
L_800DA684: ;
    r31 = 0x2;
    goto L_800DA6A8;
L_800DA68C: ;
    r31 = 0x3;
    goto L_800DA6A8;
L_800DA694: ;
    r31 = 0x4;
    goto L_800DA6A8;
L_800DA69C: ;
    r31 = 0x5;
    goto L_800DA6A8;
L_800DA6A4: ;
    r31 = 0x6;
L_800DA6A8: ;
    r5 = r8;
    r3 = r31;
    r4 = r27;
    r7 = r28;
    r8 = r29;
    fn_800DA6F0();
    r8 = r3;
    goto L_800DA6D4;
L_800DA6C8: ;
    r8 = r8 + 0x4;
    goto L_800DA6D4;
    goto L_800DA6DC;
L_800DA6D4: ;
    if ((u32)r8 < (u32)r30) goto L_800DA5A0;
L_800DA6DC: ;
    return;
}

/* fn_800DA6F0 | Size: 0x190 */
void fn_800DA6F0(void) {
    extern void fn_800DA880();
    extern u8 jumptable_803152B8[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r28 = r5;
    r27 = r4;
    r29 = r6;
    r30 = r7;
    r31 = r8;
    r6 = r27;
    r5 = 0x0;
    goto L_800DA7B8;
L_800DA720: ;
    if ((u32)r0 > (u32)0x19) goto L_800DA7B4;
    r4 = (u32)jumptable_803152B8;
    r0 = r0 << 2;
    r4 = (u32)jumptable_803152B8;
    /* lwzx r0, r4, r0 */;
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r5 = r5 | 0x1;
    goto L_800DA7B4;
    r5 = r5 | 0x40;
    goto L_800DA7B4;
    r5 = r5 | 0x2;
    goto L_800DA7B4;
    r5 = r5 | 0x4;
    goto L_800DA7B4;
    r5 = r5 | 0x8;
    goto L_800DA7B4;
    r5 = r5 | 0x10;
    goto L_800DA7B4;
    r5 = r5 | 0x20;
    goto L_800DA7B4;
    r5 = r5 | 0x80;
    goto L_800DA7B4;
    r5 = r5 | 0x100;
    goto L_800DA7B4;
    r5 = r5 | 0x200;
    goto L_800DA7B4;
    r5 = r5 | 0x400;
    goto L_800DA7B4;
    r5 = r5 | 0x800;
    goto L_800DA7B4;
    r5 = r5 | 0x1000;
    goto L_800DA7B4;
    r5 = r5 | 0x2000;
    goto L_800DA7B4;
    r5 = r5 | 0x4000;
L_800DA7B4: ;
    r6 = r6 + 0x18;
L_800DA7B8: ;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r0 != (s32)0xff) goto L_800DA720;
    r12 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_800DA844;
    r4 = r29;
    r6 = r31;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    goto L_800DA844;
L_800DA7E4: ;
    r12 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r12 == (u32)0x0) goto L_800DA7FC;
    r3 = r31;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800DA7FC: ;
    r26 = r27;
    goto L_800DA820;
L_800DA804: ;
    r3 = r26;
    r4 = r28;
    r5 = r30;
    r6 = r31;
    fn_800DA880();
    r28 = r3;
    r26 = r26 + 0x18;
L_800DA820: ;
    r0 = *(u32*)((u8*)r26 + 0x0);
    if ((s32)r0 != (s32)0xff) goto L_800DA804;
    r12 = *(u32*)((u8*)r30 + 0xC);
    if ((u32)r12 == (u32)0x0) goto L_800DA844;
    r3 = r31;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800DA844: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r12 != (u32)0x0) goto L_800DA7E4;
    r12 = *(u32*)((u8*)r30 + 0x10);
    if ((u32)r12 == (u32)0x0) goto L_800DA868;
    r3 = r31;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800DA868: ;
    r3 = r28;
    return;
}

/* fn_800DA880 | Size: 0x440 */
void fn_800DA880(void) {
    extern u8 jumptable_80315320[];
    extern u8 jumptable_80315340[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r4;
    r10 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r10 >= (s32)0x15) goto L_800DA8B4;
    if ((s32)r10 >= (s32)0x9) goto L_800DA934;
    if ((s32)r10 >= (s32)0x0) goto L_800DA8C0;
    goto L_800DAC88;
L_800DA8B4: ;
    if ((s32)r10 == (s32)0x19) goto L_800DA934;
    goto L_800DAC88;
L_800DA8C0: ;
    r8 = (0x1 << 16);
    r9 = (0x1000 << 16);
    if ((u32)r10 > (u32)0x8) goto L_800DA92C;
    r3 = (u32)jumptable_80315340;
    r0 = r10 << 2;
    r3 = (u32)jumptable_80315340;
    /* lwzx r0, r3, r0 */;
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r7 = 0x1;
    goto L_800DA92C;
    r7 = 0x40;
    goto L_800DA92C;
    r7 = 0x40;
    goto L_800DA92C;
    r7 = 0x40;
    goto L_800DA92C;
    r7 = 0x40;
    goto L_800DA92C;
    r7 = 0x40;
    goto L_800DA92C;
    r7 = 0x40;
    goto L_800DA92C;
    r7 = 0x40;
    goto L_800DA92C;
    r7 = 0x40;
L_800DA92C: ;
    r31 = r31 + 0x1;
    goto L_800DAC88;
L_800DA934: ;
    if ((s32)r10 == (s32)0xb) goto L_800DA944;
    if ((s32)r10 != (s32)0xc) goto L_800DA9C4;
L_800DA944: ;
    r4 = 0x20;
    if ((s32)r10 != (s32)0xb) goto L_800DA954;
    r4 = 0x10;
L_800DA954: ;
    r10 = *(u32*)((u8*)r3 + 0xC);
    r7 = r4;
    r9 = (0x1000 << 16);
    if ((s32)r10 == (s32)0x3) goto L_800DA9AC;
    if ((s32)r10 >= (s32)0x3) goto L_800DA984;
    if ((s32)r10 == (s32)0x1) goto L_800DA99C;
    if ((s32)r10 >= (s32)0x1) goto L_800DA9A4;
    if ((s32)r10 >= (s32)0x0) goto L_800DA994;
    goto L_800DAB1C;
L_800DA984: ;
    if ((s32)r10 == (s32)0x5) goto L_800DA9BC;
    if ((s32)r10 >= (s32)0x5) goto L_800DAB1C;
    goto L_800DA9B4;
L_800DA994: ;
    r8 = (0x20 << 16);
    goto L_800DAB1C;
L_800DA99C: ;
    r8 = (0x40 << 16);
    goto L_800DAB1C;
L_800DA9A4: ;
    r8 = (0x80 << 16);
    goto L_800DAB1C;
L_800DA9AC: ;
    r8 = (0x100 << 16);
    goto L_800DAB1C;
L_800DA9B4: ;
    r8 = (0x200 << 16);
    goto L_800DAB1C;
L_800DA9BC: ;
    r8 = (0x400 << 16);
    goto L_800DAB1C;
L_800DA9C4: ;
    r8 = *(u32*)((u8*)r3 + 0xC);
    if ((s32)r8 == (s32)0x2) goto L_800DAA04;
    if ((s32)r8 >= (s32)0x2) goto L_800DA9E4;
    if ((s32)r8 == (s32)0x0) goto L_800DA9F4;
    if ((s32)r8 >= (s32)0x0) goto L_800DA9FC;
    goto L_800DAA18;
L_800DA9E4: ;
    if ((s32)r8 == (s32)0x4) goto L_800DAA14;
    if ((s32)r8 >= (s32)0x4) goto L_800DAA18;
    goto L_800DAA0C;
L_800DA9F4: ;
    r4 = (0x1 << 16);
    goto L_800DAA18;
L_800DA9FC: ;
    r4 = (0x2 << 16);
    goto L_800DAA18;
L_800DAA04: ;
    r4 = (0x4 << 16);
    goto L_800DAA18;
L_800DAA0C: ;
    r4 = (0x8 << 16);
    goto L_800DAA18;
L_800DAA14: ;
    r4 = (0x10 << 16);
L_800DAA18: ;
    r8 = r4;
    if ((s32)r10 >= (s32)0xd) goto L_800DAA3C;
    if ((s32)r10 == (s32)0xa) goto L_800DAA74;
    if ((s32)r10 >= (s32)0xa) goto L_800DAB1C;
    if ((s32)r10 >= (s32)0x9) goto L_800DAA54;
    goto L_800DAB1C;
L_800DAA3C: ;
    if ((s32)r10 == (s32)0x19) goto L_800DAA8C;
    if ((s32)r10 >= (s32)0x19) goto L_800DAB1C;
    if ((s32)r10 >= (s32)0x15) goto L_800DAB1C;
    goto L_800DAAA4;
L_800DAA54: ;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r7 = 0x2;
    if ((s32)r4 != (s32)0x0) goto L_800DAA6C;
    r9 = (0x2000 << 16);
    goto L_800DAB1C;
L_800DAA6C: ;
    r9 = (0x4000 << 16);
    goto L_800DAB1C;
L_800DAA74: ;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r7 = 0x4;
    if ((s32)r4 != (s32)0x0) goto L_800DAB1C;
    r9 = (0x4000 << 16);
    goto L_800DAB1C;
L_800DAA8C: ;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r7 = 0x8;
    if ((s32)r4 != (s32)0x1) goto L_800DAB1C;
    r9 = (0x4000 << 16);
    goto L_800DAB1C;
L_800DAAA4: ;
    if ((u32)r9 > (u32)0x7) goto L_800DAB04;
    r4 = (u32)jumptable_80315320;
    r9 = r9 << 2;
    r4 = (u32)jumptable_80315320;
    /* lwzx r4, r4, r9 */;
    ctr_fn = (void(*)(void))r4;
    /* indirect jump via ctr */;
    r7 = 0x80;
    goto L_800DAB04;
    r7 = 0x100;
    goto L_800DAB04;
    r7 = 0x200;
    goto L_800DAB04;
    r7 = 0x400;
    goto L_800DAB04;
    r7 = 0x800;
    goto L_800DAB04;
    r7 = 0x1000;
    goto L_800DAB04;
    r7 = 0x2000;
    goto L_800DAB04;
    r7 = 0x4000;
L_800DAB04: ;
    r4 = *(u32*)((u8*)r3 + 0x8);
    if ((s32)r4 != (s32)0x0) goto L_800DAB18;
    r9 = (0x1000 << 16);
    goto L_800DAB1C;
L_800DAB18: ;
    r9 = (0x2000 << 16);
L_800DAB1C: ;
    r4 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r4 != (s32)0x1) goto L_800DAC58;
    r3 = (0x20 << 16);
    r4 = r31;
    if ((s32)r8 == (s32)r3) goto L_800DABE4;
    if ((s32)r8 >= (s32)r3) goto L_800DAB8C;
    r3 = (0x4 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABE4;
    if ((s32)r8 >= (s32)r3) goto L_800DAB6C;
    r3 = (0x2 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABDC;
    if ((s32)r8 >= (s32)r3) goto L_800DABF8;
    r3 = (0x1 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABDC;
    goto L_800DABF8;
L_800DAB6C: ;
    r3 = (0x10 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABF4;
    if ((s32)r8 >= (s32)r3) goto L_800DABF8;
    r3 = (0x8 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABE4;
    goto L_800DABF8;
L_800DAB8C: ;
    r3 = (0x100 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABE4;
    if ((s32)r8 >= (s32)r3) goto L_800DABBC;
    r3 = (0x80 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABF4;
    if ((s32)r8 >= (s32)r3) goto L_800DABF8;
    r3 = (0x40 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABEC;
    goto L_800DABF8;
L_800DABBC: ;
    r3 = (0x400 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABF4;
    if ((s32)r8 >= (s32)r3) goto L_800DABF8;
    r3 = (0x200 << 16);
    if ((s32)r8 == (s32)r3) goto L_800DABEC;
    goto L_800DABF8;
L_800DABDC: ;
    r0 = 0x1;
    goto L_800DABF8;
L_800DABE4: ;
    r0 = 0x2;
    goto L_800DABF8;
L_800DABEC: ;
    r0 = 0x3;
    goto L_800DABF8;
L_800DABF4: ;
    r0 = 0x4;
L_800DABF8: ;
    r3 = (0x2000 << 16);
    if ((s32)r9 == (s32)r3) goto L_800DAC3C;
    if ((s32)r9 >= (s32)r3) goto L_800DAC2C;
    r3 = (0x1000 << 16);
    if ((s32)r9 == (s32)r3) goto L_800DAC50;
    if ((s32)r9 >= (s32)r3) goto L_800DAC50;
    r3 = (0x8000 << 16);
    r3 = r3 + 0x1;
    if ((s32)r9 >= (s32)r3) goto L_800DAC50;
    goto L_800DAC4C;
L_800DAC2C: ;
    r3 = (0x4000 << 16);
    if ((s32)r9 == (s32)r3) goto L_800DAC44;
    goto L_800DAC50;
L_800DAC3C: ;
    r0 = r0 << 1;
    goto L_800DAC50;
L_800DAC44: ;
    r0 = r0 * 0x3;
    goto L_800DAC50;
L_800DAC4C: ;
    r0 = r0 << 2;
L_800DAC50: ;
    r31 = r31 + r0;
    goto L_800DAC88;
L_800DAC58: ;
    if ((s32)r4 != (s32)0x2) goto L_800DAC6C;
    r4 = *(u8*)((u8*)r31 + 0x0);
    r31 = r31 + 0x1;
    goto L_800DAC74;
L_800DAC6C: ;
    r4 = *(u16*)((u8*)r31 + 0x0);
    r31 = r31 + 0x2;
L_800DAC74: ;
    r0 = *(u16*)((u8*)r3 + 0x12);
    r4 = r4 & 0xFFFF;
    r3 = *(u32*)((u8*)r3 + 0x14);
    r0 = r4 * r0;
    r4 = r3 + r0;
L_800DAC88: ;
    r12 = *(u32*)((u8*)r5 + 0x8);
    if ((u32)r12 == (u32)0x0) goto L_800DACA8;
    r0 = r7 | r8;
    r5 = r6;
    r3 = r9 | r0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800DACA8: ;
    r3 = r31;
    return;
}

/* fn_800DACC0 | Size: 0x50 */
void fn_800DACC0(u8* obj) {
    extern void fn_800E209C();
    extern void fn_800E24B0();
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x480) != (u32)obj) {
        fn_800E24B0(*(u16*)(obj + 0x2));
        fn_800E209C(*(u16*)(obj + 0x2));
        *(u8*)(obj + 0x0) = 0;
    }
}

/* fn_800DAD10 | Size: 0xA4 */
void fn_800DAD10(void) {
    extern void fn_800BD0FC();
    extern void fn_800D4F98();
    extern void fn_800D6A5C();
    extern void fn_800D7A70();
    extern void fn_800D892C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DAD4C;
    r5 = r31;
    r3 = 0x2a;
    r4 = 0x1;
    fn_800D4F98();
    goto L_800DADA0;
L_800DAD4C: ;
    r3 = *(u8*)((u8*)r4 + 0x1B);
    r0 = *(u8*)((u8*)r4 + 0x1A);
    if ((u32)r3 != (u32)r0) goto L_800DADA0;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r3 == (u32)r0) goto L_800DADA0;
    r0 = *(u32*)((u8*)r31 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_800DADA0;
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
L_800DADA0: ;
    return;
}

/* fn_800DADB4 | Size: 0x1AC */
void fn_800DADB4(void) {
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E2AF8();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x47E);
    if ((u32)r0 != (u32)0x0) goto L_800DADDC;
    r3 = 0x0;
    goto L_800DAF4C;
L_800DADDC: ;
    r31 = *(u32*)((u8*)r3 + 0x480);
    r5 = *(u32*)((u8*)r3 + 0x484);
    r3 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    r4 = r5 + 0x1f;
    /* clrrwi r7, r4, 5 */;
    r0 = r3 + r0;
    if ((u32)r7 > (u32)r0) goto L_800DAE0C;
    r0 = *(u8*)((u8*)r31 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_800DAE24;
L_800DAE0C: ;
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E24B0();
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E209C();
    r3 = 0x0;
    goto L_800DAF4C;
L_800DAE24: ;
    /* subf. r0, r5, r7 */;
    r6 = 0x0;
    r3 = r0;
    if ((u32)r0 == (u32)0x0) goto L_800DAF08;
    /* srwi. r0, r0, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 == (u32)0x0) goto L_800DAEEC;
L_800DAE40: ;
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
    if (--ctr != 0) goto L_800DAE40;
    r3 = r3 & 0x7;
    if ((u32)r0 == (u32)0x0) goto L_800DAF08;
L_800DAEEC: ;
    ctr_fn = (void(*)(void))r3;
L_800DAEF0: ;
    r5 = *(u32*)lbl_8047AA80;
    r4 = *(u32*)((u8*)r5 + 0x484);
    r0 = r4 + 0x1;
    *(u32*)((u8*)r5 + 0x484) = r0;
    *(u8*)((u8*)r4 + 0x0) = r6;
    if (--ctr != 0) goto L_800DAEF0;
L_800DAF08: ;
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
L_800DAF4C: ;
    return;
}

/* fn_800DAF60 | Size: 0x138 */
void fn_800DAF60(void) {
    extern u8 lbl_8047AAD4[];
    extern u8 lbl_8047AAD8[];
    extern void fn_800E209C();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r4;
    r29 = r3;
    r5 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r5 + 0x47E);
    if ((u32)r0 != (u32)0x1) goto L_800DAF98;
    r3 = 0x0;
    goto L_800DB07C;
L_800DAF98: ;
    r0 = *(u8*)((u8*)r5 + 0x49F);
    if ((u32)r0 != (u32)0x1) goto L_800DAFAC;
    r3 = 0x0;
    goto L_800DB07C;
L_800DAFAC: ;
    r0 = *(u32*)lbl_8047AAD8;
    r31 = *(u32*)lbl_8047AAD4;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_800DAFD8;
L_800DAFC0: ;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_800DAFD0;
    goto L_800DAFDC;
L_800DAFD0: ;
    r31 = r31 + 0x18;
    if (--ctr != 0) goto L_800DAFC0;
L_800DAFD8: ;
    r31 = 0x0;
L_800DAFDC: ;
    if ((u32)r31 != (u32)0x0) goto L_800DAFEC;
    r3 = 0x0;
    goto L_800DB07C;
L_800DAFEC: ;
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
    if ((u32)r0 != (u32)0x0) goto L_800DB028;
    r3 = 0x0;
    goto L_800DB07C;
L_800DB028: ;
    *(u32*)((u8*)r31 + 0x8) = r30;
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E27B0();
    *(u32*)((u8*)r31 + 0x4) = r3;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)0x0) goto L_800DB054;
    r3 = *(u16*)((u8*)r31 + 0x2);
    fn_800E209C();
    r3 = 0x0;
    goto L_800DB07C;
L_800DB054: ;
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
L_800DB07C: ;
    return;
}

/* fn_800DB098 | Size: 0x6C0 */
void fn_800DB098(void) {
    extern void fn_800D724C();
    extern void fn_800D7268();
    extern void fn_800D7284();
    extern void fn_800D72A4();
    extern void fn_800D72C4();
    extern void fn_800D72E4();
    extern void fn_800D7304();
    extern void fn_800D7328();
    extern void fn_800D7344();
    extern void fn_800D7360();
    extern void fn_800D737C();
    extern void fn_800D7398();
    extern void fn_800D73C4();
    extern void fn_800D73F8();
    extern void fn_800D740C();
    extern void fn_800D7420();
    extern void fn_800D7444();
    extern void fn_800D7468();
    extern void fn_800D748C();
    extern void fn_800D74A0();
    extern void fn_800D74B4();
    extern void fn_800D74D0();
    extern void fn_800D74EC();
    extern void fn_800D7508();
    extern void fn_800D7524();
    extern void fn_800D7540();
    extern void fn_800D7564();
    extern void fn_800D7588();
    extern void fn_800D75AC();
    extern void fn_800D75D0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = *(u32*)lbl_8047AA80;
    r7 = *(u32*)((u8*)r3 + 0x480);
    r4 = *(u32*)((u8*)r3 + 0x484);
    r5 = *(u32*)((u8*)r7 + 0x4);
    r0 = *(u32*)((u8*)r7 + 0x8);
    r6 = r4 + 0x68;
    r0 = r5 + r0;
    if ((u32)r6 <= (u32)r0) goto L_800DB0C8;
    r0 = 0x1;
    *(u8*)((u8*)r7 + 0x1) = r0;
    return;
L_800DB0C8: ;
    r5 = (u32)fn_800D75D0;
    r6 = *(u32*)((u8*)r3 + 0x4A8);
    r0 = (u32)fn_800D75D0;
    if ((u32)r6 != (u32)r0) goto L_800DB10C;
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
    goto L_800DB2B4;
L_800DB10C: ;
    r5 = (u32)fn_800D75AC;
    r0 = (u32)fn_800D75AC;
    if ((u32)r6 == (u32)r0) goto L_800DB12C;
    r5 = (u32)fn_800D7588;
    r0 = (u32)fn_800D7588;
    if ((u32)r6 != (u32)r0) goto L_800DB15C;
L_800DB12C: ;
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
    goto L_800DB2B4;
L_800DB15C: ;
    r5 = (u32)fn_800D7564;
    r0 = (u32)fn_800D7564;
    if ((u32)r6 == (u32)r0) goto L_800DB17C;
    r5 = (u32)fn_800D7540;
    r0 = (u32)fn_800D7540;
    if ((u32)r6 != (u32)r0) goto L_800DB1AC;
L_800DB17C: ;
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
    goto L_800DB2B4;
L_800DB1AC: ;
    r5 = (u32)fn_800D7524;
    r0 = (u32)fn_800D7524;
    if ((u32)r6 != (u32)r0) goto L_800DB1E0;
    r0 = *(u32*)((u8*)r3 + 0x4B8);
    r5 = r4 + 0x8;
    *(u32*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r3 + 0x4BC);
    *(u32*)((u8*)r4 + 0x4) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto L_800DB2B4;
L_800DB1E0: ;
    r5 = (u32)fn_800D7508;
    r0 = (u32)fn_800D7508;
    if ((u32)r6 == (u32)r0) goto L_800DB200;
    r5 = (u32)fn_800D74EC;
    r0 = (u32)fn_800D74EC;
    if ((u32)r6 != (u32)r0) goto L_800DB224;
L_800DB200: ;
    r0 = *(u16*)((u8*)r3 + 0x4B0);
    r5 = r4 + 0x4;
    *(u16*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u16*)((u8*)r3 + 0x4B2);
    *(u16*)((u8*)r4 + 0x2) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto L_800DB2B4;
L_800DB224: ;
    r5 = (u32)fn_800D74D0;
    r0 = (u32)fn_800D74D0;
    if ((u32)r6 == (u32)r0) goto L_800DB244;
    r5 = (u32)fn_800D74B4;
    r0 = (u32)fn_800D74B4;
    if ((u32)r6 != (u32)r0) goto L_800DB268;
L_800DB244: ;
    r0 = *(u8*)((u8*)r3 + 0x4AC);
    r5 = r4 + 0x2;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    r0 = *(u8*)((u8*)r3 + 0x4AD);
    *(u8*)((u8*)r4 + 0x1) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto L_800DB2B4;
L_800DB268: ;
    r5 = (u32)fn_800D74A0;
    r0 = (u32)fn_800D74A0;
    if ((u32)r6 != (u32)r0) goto L_800DB290;
    r0 = *(u16*)((u8*)r3 + 0x4B0);
    r5 = r4 + 0x2;
    *(u16*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto L_800DB2B4;
L_800DB290: ;
    r5 = (u32)fn_800D748C;
    r0 = (u32)fn_800D748C;
    if ((u32)r6 != (u32)r0) goto L_800DB2B4;
    r0 = *(u8*)((u8*)r3 + 0x4AC);
    r5 = r4 + 0x1;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
L_800DB2B4: ;
    r4 = *(u32*)lbl_8047AA80;
    r3 = *(u32*)((u8*)r4 + 0x480);
    r5 = *(u32*)((u8*)r4 + 0x4C4);
    r3 = *(u32*)((u8*)r3 + 0xC);
    r0 = *(u8*)((u8*)r3 + 0x40);
    if ((u32)r0 != (u32)0x1) goto L_800DB3F0;
    r3 = (u32)fn_800D7468;
    r0 = (u32)fn_800D7468;
    if ((u32)r5 != (u32)r0) goto L_800DB314;
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
    goto L_800DB3F0;
L_800DB314: ;
    r3 = (u32)fn_800D7444;
    r0 = (u32)fn_800D7444;
    if ((u32)r5 != (u32)r0) goto L_800DB358;
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
    goto L_800DB3F0;
L_800DB358: ;
    r3 = (u32)fn_800D7420;
    r0 = (u32)fn_800D7420;
    if ((u32)r5 != (u32)r0) goto L_800DB39C;
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
    goto L_800DB3F0;
L_800DB39C: ;
    r3 = (u32)fn_800D740C;
    r0 = (u32)fn_800D740C;
    if ((u32)r5 != (u32)r0) goto L_800DB3C8;
    r5 = *(u32*)((u8*)r4 + 0x484);
    r0 = *(u16*)((u8*)r4 + 0x4CC);
    *(u16*)((u8*)r5 + 0x0) = r0;
    r5 = r5 + 0x2;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
    goto L_800DB3F0;
L_800DB3C8: ;
    r3 = (u32)fn_800D73F8;
    r0 = (u32)fn_800D73F8;
    if ((u32)r5 != (u32)r0) goto L_800DB3F0;
    r5 = *(u32*)((u8*)r4 + 0x484);
    r0 = *(u8*)((u8*)r4 + 0x4C8);
    *(u8*)((u8*)r5 + 0x0) = r0;
    r5 = r5 + 0x1;
    r3 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r3 + 0x484) = r5;
L_800DB3F0: ;
    r0 = 0x2;
    r5 = 0x4;
    r3 = 0x70;
    ctr_fn = (void(*)(void))r0;
L_800DB400: ;
    r4 = *(u32*)lbl_8047AA80;
    r7 = r8 << 2;
    r0 = r3 + 0x8;
    r6 = *(u32*)((u8*)r4 + 0x480);
    r7 = r7 + 0x4e0;
    /* lwzx r7, r4, r7 */;
    r6 = *(u32*)((u8*)r6 + 0xC);
    /* lbzx r0, r6, r0 */;
    if ((u32)r0 != (u32)0x1) goto L_800DB5B0;
    r6 = (u32)fn_800D73C4;
    r0 = (u32)fn_800D73C4;
    if ((u32)r7 != (u32)r0) goto L_800DB490;
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
    goto L_800DB5B0;
L_800DB490: ;
    r6 = (u32)fn_800D7398;
    r0 = (u32)fn_800D7398;
    if ((u32)r7 != (u32)r0) goto L_800DB4E4;
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
    goto L_800DB5B0;
L_800DB4E4: ;
    r6 = (u32)fn_800D737C;
    r0 = (u32)fn_800D737C;
    if ((u32)r7 != (u32)r0) goto L_800DB518;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4f0;
    /* lwzx r0, r4, r0 */;
    *(u32*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x4;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto L_800DB5B0;
L_800DB518: ;
    r6 = (u32)fn_800D7360;
    r0 = (u32)fn_800D7360;
    if ((u32)r7 != (u32)r0) goto L_800DB54C;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4ec;
    /* lhzx r0, r4, r0 */;
    *(u16*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x2;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto L_800DB5B0;
L_800DB54C: ;
    r6 = (u32)fn_800D7344;
    r0 = (u32)fn_800D7344;
    if ((u32)r7 != (u32)r0) goto L_800DB580;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4ec;
    /* lhzx r0, r4, r0 */;
    *(u16*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x2;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto L_800DB5B0;
L_800DB580: ;
    r6 = (u32)fn_800D7328;
    r0 = (u32)fn_800D7328;
    if ((u32)r7 != (u32)r0) goto L_800DB5B0;
    r6 = r8 * 0xc;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x4e8;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x1;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
L_800DB5B0: ;
    r3 = r3 + 0x1c;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto L_800DB400;
    r0 = 0x8;
    r5 = 0x6;
    r3 = 0xa8;
    ctr_fn = (void(*)(void))r0;
L_800DB5CC: ;
    r4 = *(u32*)lbl_8047AA80;
    r7 = r8 << 2;
    r0 = r3 + 0x8;
    r6 = *(u32*)((u8*)r4 + 0x480);
    r7 = r7 + 0x500;
    /* lwzx r7, r4, r7 */;
    r6 = *(u32*)((u8*)r6 + 0xC);
    /* lbzx r0, r6, r0 */;
    if ((u32)r0 != (u32)0x1) goto L_800DB748;
    r6 = (u32)fn_800D7304;
    r0 = (u32)fn_800D7304;
    if ((u32)r7 != (u32)r0) goto L_800DB63C;
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
    goto L_800DB748;
L_800DB63C: ;
    r6 = (u32)fn_800D72E4;
    r0 = (u32)fn_800D72E4;
    if ((u32)r7 == (u32)r0) goto L_800DB65C;
    r6 = (u32)fn_800D72C4;
    r0 = (u32)fn_800D72C4;
    if ((u32)r7 != (u32)r0) goto L_800DB690;
L_800DB65C: ;
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
    goto L_800DB748;
L_800DB690: ;
    r6 = (u32)fn_800D72A4;
    r0 = (u32)fn_800D72A4;
    if ((u32)r7 == (u32)r0) goto L_800DB6B0;
    r6 = (u32)fn_800D7284;
    r0 = (u32)fn_800D7284;
    if ((u32)r7 != (u32)r0) goto L_800DB6E4;
L_800DB6B0: ;
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
    goto L_800DB748;
L_800DB6E4: ;
    r6 = (u32)fn_800D7268;
    r0 = (u32)fn_800D7268;
    if ((u32)r7 != (u32)r0) goto L_800DB718;
    r6 = r8 << 4;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x522;
    /* lhzx r0, r4, r0 */;
    *(u16*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x2;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
    goto L_800DB748;
L_800DB718: ;
    r6 = (u32)fn_800D724C;
    r0 = (u32)fn_800D724C;
    if ((u32)r7 != (u32)r0) goto L_800DB748;
    r6 = r8 << 4;
    r7 = *(u32*)((u8*)r4 + 0x484);
    r0 = r6 + 0x520;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r7 + 0x0) = r0;
    r7 = r7 + 0x1;
    r4 = *(u32*)lbl_8047AA80;
    *(u32*)((u8*)r4 + 0x484) = r7;
L_800DB748: ;
    r3 = r3 + 0x1c;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto L_800DB5CC;
    return;
}

/* fn_800DB758 | Size: 0x138 */
void fn_800DB758(void) {
    extern void fn_800D6A80();
    extern u8 jumptable_80315364[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x488);
    if ((s32)r0 != (s32)0x7) goto L_800DB780;
    /* clrlslwi r31, r31, 17, 1 */;
L_800DB780: ;
    r6 = *(u32*)((u8*)r4 + 0x480);
    r3 = r31;
    r4 = *(u32*)((u8*)r4 + 0x488);
    r5 = r6 + 0x10;
    r6 = r6 + 0x14;
    fn_800D6A80();
    r4 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r4 + 0x488);
    if ((u32)r0 > (u32)0x7) goto L_800DB83C;
    r3 = (u32)jumptable_80315364;
    r0 = r0 << 2;
    r3 = (u32)jumptable_80315364;
    /* lwzx r0, r3, r0 */;
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xb8;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xa8;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xb0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x90;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x98;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0xa0;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x80;
    *(u8*)((u8*)r3 + 0x0) = r0;
    goto L_800DB83C;
    r3 = *(u32*)((u8*)r4 + 0x484);
    r0 = 0x80;
    *(u8*)((u8*)r3 + 0x0) = r0;
L_800DB83C: ;
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
    return;
}

/* fn_800DB890 -- GSgfx_InitLighting | Size: 0x70 */
void fn_800DB890(void) {
    extern u8 lbl_8047AAD0[];
    extern u8 lbl_8047AAD4[];
    extern u8 lbl_8047AAD8[];
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r0 = r3;
    r3 = r0 * 0x18;
    *(u32*)lbl_8047AAD8 = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAD0 = r3;
    if ((s32)r0 == (s32)0) goto L_800DB8F0;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AAD4 = r3;
    r4 = r5;
    r6 = 0x0;
    goto L_800DB8E4;
L_800DB8D4: ;
    r3 = *(u32*)lbl_8047AAD4;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x18;
L_800DB8E4: ;
    r0 = *(u32*)lbl_8047AAD8;
    if ((u32)r6 < (u32)r0) goto L_800DB8D4;
L_800DB8F0: ;
    return;
}

/* fn_800DB900 | Size: 0x88 */
void fn_800DB900(u32 param1, u32 param2, u32 param3) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x5b, 0x12, param1, param2, (s8)param3);
    } else {
        memcpy((void*)(((u32)lbl_80400B28 + (*(u32*)((u8*)state + 0x0) * 0x1c)) + 0x360), (const void*)0x12, (u32)0x18);
        *(u8*)((((u32)lbl_80400B28 + (*(u32*)((u8*)state + 0x0) * 0x1c)) + 0x360) + 0x18) = param3;
    }
}


/* fn_800DB988 | Size: 0x68 */
void fn_800DB988(u32 param1, u8 param2, u32 param3) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x5a, 0x3, param1, param2, param3);
    } else {
        *(u8*)((((u32)lbl_80400B28 + (0x5a << 2)) + 0x34d) + 0x2) = 0x3;
        *(u8*)((((u32)lbl_80400B28 + (0x5a << 2)) + 0x34d) + 0x3) = param3;
    }
}


/* fn_800DB9F0 | Size: 0x64 */
void fn_800DB9F0(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DBA2C;
    r5 = r3;
    r6 = r4;
    r3 = 0x59;
    r4 = 0x3;
    fn_800D4F98();
    goto L_800DBA44;
L_800DBA2C: ;
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    r3 += 845; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r7;
L_800DBA44: ;
    return;
}

/* fn_800DBA54 | Size: 0x50 */
void fn_800DBA54(u8 val) {
    extern void fn_800D4F98();
    extern u8 lbl_80400B28[];
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x58, 0x1, (u32)(val & 0xFF));
    } else {
        *(u8*)(lbl_80400B28 + 0x34C) = val;
    }
}

/* fn_800DBAA4 | Size: 0x68 */
void fn_800DBAA4(u32 param1) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x57, 0x1, param1);
    } else {
        *(u8*)(((u32)lbl_80400B28 + 0x57) + 0x1FC) = 0x1;
        *(u8*)(((u32)lbl_80400B28 + (0x57 * 0x14)) + 0x20C) = 0x5;
    }
}


/* fn_800DBB0C | Size: 0x78 */
void fn_800DBB0C(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DBB48;
    r5 = r3;
    r6 = r4;
    r3 = 0x56;
    r4 = 0x3;
    fn_800D4F98();
    goto L_800DBB74;
L_800DBB48: ;
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
L_800DBB74: ;
    return;
}

/* fn_800DBB84 | Size: 0x78 */
void fn_800DBB84(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r7 = r5;
    r6 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DBBC0;
    r5 = r3;
    r6 = r4;
    r3 = 0x55;
    r4 = 0x3;
    fn_800D4F98();
    goto L_800DBBEC;
L_800DBBC0: ;
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
L_800DBBEC: ;
    return;
}

/* fn_800DBBFC | Size: 0xE8 */
void fn_800DBBFC(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r8;
    r30 = r7;
    r29 = r6;
    r28 = r5;
    r11 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r11 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DBC7C;
    r5 = r3;
    r6 = r4;
    r7 = r28 & 0xFFFF;
    r8 = r29 & 0xFFFF;
    r9 = r30 & 0xFFFF;
    r10 = r31 & 0xFFFF;
    r3 = 0x54;
    r4 = 0xa;
    fn_800D4F98();
    goto L_800DBCC4;
L_800DBC7C: ;
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
L_800DBCC4: ;
    return;
}

/* fn_800DBCE4 | Size: 0x8C */
void fn_800DBCE4(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r10 = r5;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r8 + 0x0);
    r8 = r6;
    if ((s32)r0 != (s32)0x1) goto L_800DBD30;
    r5 = r3;
    r6 = r4;
    r9 = r7;
    r7 = r10 & 0xFF;
    r8 = r8 & 0xFF;
    r3 = 0x53;
    r4 = 0x5;
    fn_800D4F98();
    goto L_800DBD60;
L_800DBD30: ;
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
L_800DBD60: ;
    return;
}

/* fn_800DBD70 | Size: 0xEC */
void fn_800DBD70(void) {
    extern void fn_800D4F98();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r12 = *(u8*)((u8*)(u32)sp + 0x3B);
    r31 = r8;
    r30 = r7;
    r29 = r6;
    r28 = r5;
    r11 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r11 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_800DBDF4;
    r0 = r10 & 0xFF;
    r5 = r3;
    r6 = r4;
    *(u32*)(sp + 0xC) = r0;
    r7 = r28;
    r8 = r29;
    r9 = r30;
    r10 = r31;
    r3 = 0x52;
    r4 = 0xa;
    fn_800D4F98();
    goto L_800DBE3C;
L_800DBDF4: ;
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
L_800DBE3C: ;
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    r28 = *(u32*)(sp + 0x20);
    return;
}

/* fn_800DBE5C | Size: 0x58 */
void fn_800DBE5C(u32 idx) {
    extern void fn_800D4F98();
    extern u8 lbl_80400B28[];
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x51, 0x1, idx);
    } else {
        *(u8*)(lbl_80400B28 + idx + 0x1FC) = 0;
    }
}

/* fn_800DBEB4 | Size: 0x68 */
void fn_800DBEB4(u32 param1, u32 param2) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x50, 0x14, param1);
    } else {
        *(u32*)(((u32)lbl_80400B28 + (0x50 << 2)) + 0x1EC) = *(u32*)(0x14 + 0x0);
    }
}


/* fn_800DBF1C | Size: 0x5C */
void fn_800DBF1C(u32 idx, u32 val) {
    extern void fn_800D4F98();
    extern u8 lbl_80400B28[];
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x4F, 0x2, idx, val);
    } else {
        *(u32*)(lbl_80400B28 + (idx << 2) + 0x1AC) = val;
    }
}

/* fn_800DBF78 | Size: 0x5C */
void fn_800DBF78(u32 idx, u32 val) {
    extern void fn_800D4F98();
    extern u8 lbl_80400B28[];
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x4E, 0x2, idx, val);
    } else {
        *(u32*)(lbl_80400B28 + (idx << 2) + 0x16C) = val;
    }
}

/* fn_800DBFD4 | Size: 0x78 */
void fn_800DBFD4(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r10 = r5;
    r9 = r7;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r8 + 0x0);
    r8 = r6;
    if ((s32)r0 != (s32)0x1) goto L_800DC01C;
    r5 = r3;
    r6 = r4;
    r7 = r10;
    r3 = 0x4d;
    r4 = 0x5;
    fn_800D4F98();
    goto L_800DC03C;
L_800DC01C: ;
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    r3 += 299; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r10;
    *(u8*)((u8*)r3 + 0x2) = r8;
    *(u8*)((u8*)r3 + 0x3) = r9;
L_800DC03C: ;
    return;
}

/* fn_800DC04C | Size: 0x88 */
void fn_800DC04C(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;

    r12 = r5;
    r11 = r6;
    r10 = r8;
    r9 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r9 + 0x0);
    r9 = r7;
    if ((s32)r0 != (s32)0x1) goto L_800DC0A0;
    r5 = r3;
    r6 = r4;
    r7 = r12;
    r8 = r11;
    r9 = r9 & 0xFF;
    r3 = 0x4c;
    r4 = 0x6;
    fn_800D4F98();
    goto L_800DC0C4;
L_800DC0A0: ;
    r5 = r3 * 0x5;
    r3 = (u32)lbl_80400B28;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r5;
    r3 += 155; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r12;
    *(u8*)((u8*)r3 + 0x2) = r11;
    *(u8*)((u8*)r3 + 0x3) = r9;
    *(u8*)((u8*)r3 + 0x4) = r10;
L_800DC0C4: ;
    return;
}

/* fn_800DC0D4 | Size: 0x78 */
void fn_800DC0D4(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r10 = r5;
    r9 = r7;
    r8 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r8 + 0x0);
    r8 = r6;
    if ((s32)r0 != (s32)0x1) goto L_800DC11C;
    r5 = r3;
    r6 = r4;
    r7 = r10;
    r3 = 0x4b;
    r4 = 0x5;
    fn_800D4F98();
    goto L_800DC13C;
L_800DC11C: ;
    r5 = (u32)lbl_80400B28;
    r3 = r3 << 2;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r3;
    r3 += 235; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r10;
    *(u8*)((u8*)r3 + 0x2) = r8;
    *(u8*)((u8*)r3 + 0x3) = r9;
L_800DC13C: ;
    return;
}

/* fn_800DC14C | Size: 0x88 */
void fn_800DC14C(void) {
    extern void fn_800D4F98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;

    r12 = r5;
    r11 = r6;
    r10 = r8;
    r9 = *(u32*)lbl_8047AA80;
    r0 = *(u32*)((u8*)r9 + 0x0);
    r9 = r7;
    if ((s32)r0 != (s32)0x1) goto L_800DC1A0;
    r5 = r3;
    r6 = r4;
    r7 = r12;
    r8 = r11;
    r9 = r9 & 0xFF;
    r3 = 0x4a;
    r4 = 0x6;
    fn_800D4F98();
    goto L_800DC1C4;
L_800DC1A0: ;
    r5 = r3 * 0x5;
    r3 = (u32)lbl_80400B28;
    r0 = (u32)lbl_80400B28;
    r3 = r0 + r5;
    r3 += 75; *(u8*)r3 = r4;
    *(u8*)((u8*)r3 + 0x1) = r12;
    *(u8*)((u8*)r3 + 0x2) = r11;
    *(u8*)((u8*)r3 + 0x3) = r9;
    *(u8*)((u8*)r3 + 0x4) = r10;
L_800DC1C4: ;
    return;
}

/* fn_800DC1D4 | Size: 0x50 */
void fn_800DC1D4(u8 val) {
    extern void fn_800D4F98();
    extern u8 lbl_80400B28[];
    u8* state = (u8*)lbl_8047AA80;
    if (*(u32*)(state + 0x0) == 1) {
        fn_800D4F98(0x49, 0x1, (u32)(val & 0xFF));
    } else {
        *(u8*)(lbl_80400B28 + 0x1A) = val;
    }
}

/* fn_800DC224 | Size: 0x74 */
void fn_800DC224(u32 param1, u8 param2, u32 param3, u32 param4, u32 param5) {
    extern void fn_800D4F98();
    u8* state = (u8*)lbl_8047AA80;
    if (*(s32*)state == 1) {
        fn_800D4F98(0x48, 0x5, param1, param2, param3, param4, param5, state);
    } else {
        *(u8*)((state + ((0x48 << 2) + 0x42e)) + 0x0) = 0x5;
        *(u8*)((state + ((0x48 << 2) + 0x42e)) + 0x1) = param3;
        *(u8*)((state + ((0x48 << 2) + 0x42e)) + 0x2) = param4;
        *(u8*)((state + ((0x48 << 2) + 0x42e)) + 0x3) = param5;
    }
}


/* fn_800DC298 | Size: 0xF8 */
void fn_800DC298(void) {
    extern u8 lbl_80400EE0[];
    extern u8 lbl_8047AAE0[];
    extern void fn_800EF504();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)lbl_80400EE0;
    r31 = (u32)lbl_80400EE0;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DC2CC;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)r3) goto L_800DC2CC;
    goto L_800DC324;
L_800DC2CC: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC2E8;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)r3) goto L_800DC2E8;
    goto L_800DC324;
L_800DC2E8: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC304;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)r3) goto L_800DC304;
    goto L_800DC324;
L_800DC304: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC320;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)r3) goto L_800DC320;
    goto L_800DC324;
L_800DC320: ;
    r31 = 0x0;
L_800DC324: ;
    if ((u32)r31 != (u32)0x0) goto L_800DC334;
    r3 = 0x0;
    goto L_800DC37C;
L_800DC334: ;
    r3 = *(u32*)((u8*)r31 + 0x4);
    fn_800EF504();
    r4 = 0x0;
    r3 = (u32)lbl_80400EE0;
    *(u8*)((u8*)r31 + 0x0) = r4;
    r0 = 0x4;
    r3 = (u32)lbl_80400EE0;
    *(u8*)lbl_8047AAE0 = r4;
    ctr_fn = (void(*)(void))r0;
L_800DC358: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DC370;
    r0 = 0x1;
    *(u8*)lbl_8047AAE0 = r0;
    goto L_800DC378;
L_800DC370: ;
    r3 = r3 + 0x14;
    if (--ctr != 0) goto L_800DC358;
L_800DC378: ;
    r3 = 0x1;
L_800DC37C: ;
    return;
}

/* fn_800DC390 | Size: 0x1B0 */
void fn_800DC390(void) {
    extern u8 lbl_80400EE0[];
    extern u8 lbl_8047AAE0[];
    extern void fn_800EF4DC();
    extern void fn_800EF548();
    extern void fn_800EF578();
    extern void fn_800EF590();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r5;
    r29 = r4;
    r28 = r3;
    fn_800EF4DC();
    if ((s32)r3 == (s32)0x90) goto L_800DC3E0;
    if ((s32)r3 >= (s32)0x90) goto L_800DC3D8;
    if ((s32)r3 >= (s32)0x46) goto L_800DC3D8;
    if ((s32)r3 >= (s32)0x40) goto L_800DC3E0;
L_800DC3D8: ;
    r3 = 0x0;
    goto L_800DC520;
L_800DC3E0: ;
    r3 = (u32)lbl_80400EE0;
    r0 = *(u8*)lbl_80400EE0;
    if ((u32)r0 != (u32)0x1) goto L_800DC400;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r28) goto L_800DC400;
    goto L_800DC458;
L_800DC400: ;
    r0 = *(u8*)((u8*)r3 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC41C;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r28) goto L_800DC41C;
    goto L_800DC458;
L_800DC41C: ;
    r0 = *(u8*)((u8*)r3 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC438;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r28) goto L_800DC438;
    goto L_800DC458;
L_800DC438: ;
    r0 = *(u8*)((u8*)r3 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC454;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((u32)r0 != (u32)r28) goto L_800DC454;
    goto L_800DC458;
L_800DC454: ;
    r3 = 0x0;
L_800DC458: ;
    if ((u32)r3 == (u32)0x0) goto L_800DC468;
    r3 = 0x0;
    goto L_800DC520;
L_800DC468: ;
    r3 = (u32)lbl_80400EE0;
    r4 = 0x0;
    r31 = (u32)lbl_80400EE0;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_800DC484;
    goto L_800DC4B8;
L_800DC484: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)0x0) goto L_800DC494;
    goto L_800DC4B8;
L_800DC494: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)0x0) goto L_800DC4A4;
    goto L_800DC4B8;
L_800DC4A4: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)0x0) goto L_800DC4B4;
    goto L_800DC4B8;
L_800DC4B4: ;
    r31 = r4;
L_800DC4B8: ;
    if ((u32)r31 != (u32)0x0) goto L_800DC4C8;
    r3 = 0x0;
    goto L_800DC520;
L_800DC4C8: ;
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
L_800DC520: ;
    return;
}

/* fn_800DC540 | Size: 0x20 */
void fn_800DC540(void) {
    extern u8 lbl_80400EE0[];
    extern u8 lbl_8047AAE0[];
    u8* base = lbl_80400EE0;
    base[0] = 0;
    *(u8*)lbl_8047AAE0 = 0;
    base[0x14] = 0;
    base[0x28] = 0;
    base[0x3C] = 0;
}

/* fn_800DC560 | Size: 0x178 */
void fn_800DC560(void) {
    extern u8 lbl_80400EE0[];
    extern u8 lbl_8047AAE0[];
    extern void fn_800EF1E8();
    extern void fn_800EF504();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = *(u8*)lbl_8047AAE0;
    if ((u32)r0 == (u32)0x0) goto L_800DC6BC;
    r3 = (u32)lbl_80400EE0;
    r30 = 0x0;
    r0 = (u32)lbl_80400EE0;
    r31 = r0;
L_800DC594: ;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DC6AC;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x0;
    fn_800EF1E8();
    r12 = *(u32*)((u8*)r31 + 0x8);
    if ((u32)r12 == (u32)0x0) goto L_800DC6A0;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r4 = *(u32*)((u8*)r31 + 0x10);
    r5 = *(u32*)((u8*)r31 + 0xC);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = r3 & 0xFF;
    if ((u32)r12 != (u32)0x0) goto L_800DC6A0;
    r3 = (u32)lbl_80400EE0;
    r4 = *(u32*)((u8*)r31 + 0x4);
    r29 = (u32)lbl_80400EE0;
    r0 = *(u8*)((u8*)r29 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DC5FC;
    r0 = *(u32*)((u8*)r29 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_800DC5FC;
    goto L_800DC654;
L_800DC5FC: ;
    r0 = *(u8*)((u8*)r29 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC618;
    r0 = *(u32*)((u8*)r29 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_800DC618;
    goto L_800DC654;
L_800DC618: ;
    r0 = *(u8*)((u8*)r29 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC634;
    r0 = *(u32*)((u8*)r29 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_800DC634;
    goto L_800DC654;
L_800DC634: ;
    r0 = *(u8*)((u8*)r29 + 0x14);
    if ((u32)r0 != (u32)0x1) goto L_800DC650;
    r0 = *(u32*)((u8*)r29 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_800DC650;
    goto L_800DC654;
L_800DC650: ;
    r29 = 0x0;
L_800DC654: ;
    if ((u32)r29 == (u32)0x0) goto L_800DC6A0;
    r3 = *(u32*)((u8*)r29 + 0x4);
    fn_800EF504();
    r4 = 0x0;
    r3 = (u32)lbl_80400EE0;
    *(u8*)((u8*)r29 + 0x0) = r4;
    r0 = 0x4;
    r3 = (u32)lbl_80400EE0;
    *(u8*)lbl_8047AAE0 = r4;
    ctr_fn = (void(*)(void))r0;
L_800DC680: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DC698;
    r0 = 0x1;
    *(u8*)lbl_8047AAE0 = r0;
    goto L_800DC6A0;
L_800DC698: ;
    r3 = r3 + 0x14;
    if (--ctr != 0) goto L_800DC680;
L_800DC6A0: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x10) = r0;
L_800DC6AC: ;
    r30 = r30 + 0x1;
    r31 = r31 + 0x14;
    if ((u32)r30 < (u32)0x4) goto L_800DC594;
L_800DC6BC: ;
    return;
}

/* fn_800DC6D8 | Size: 0x19C */
void fn_800DC6D8(void) {
    extern u8 lbl_8047AAEC[];
    extern u8 lbl_8047AAF0[];
    extern u8 lbl_8047CA70[];
    extern u8 lbl_8047CA74[];
    extern u8 lbl_8047CA78[];
    extern u8 lbl_8047CA80[];
    extern void fn_801A6370();
    extern void fn_801A6408();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r31 = 0x0;
    r30 = 0x0;
    r28 = r3;
    goto L_800DC848;
L_800DC704: ;
    r0 = *(u32*)lbl_8047AAEC;
    r29 = r0 + r31;
    r0 = *(u8*)((u8*)r29 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DC840;
    r0 = *(u8*)((u8*)r29 + 0x3);
    if ((u32)r0 != (u32)0x1) goto L_800DC840;
    r3 = *(u32*)((u8*)r29 + 0xC);
    f1 = *(f32*)((u8*)r29 + 0x68);
    fn_801A6370();
    r3 = *(u32*)((u8*)r29 + 0xC);
    fn_801A6408();
    r3 = (0x4330 << 16);
    r0 = *(u8*)((u8*)r29 + 0x71);
    r0 = (s8)r0;
    f2 = *(f64*)lbl_8047CA80;
    f3 = *(f32*)((u8*)r29 + 0x64);
    f0 = *(f64*)(sp + 0x8);
    f1 = *(f32*)((u8*)r29 + 0x6C);
    f2 = f0 - f2;
    f0 = *(f32*)lbl_8047CA70;
    r3 = *(u32*)((u8*)r29 + 0x5C);
    f4 = f1 - f0;
    f1 = f3 * f2;
    if ((s32)r0 != (s32)-0x1) goto L_800DC788;
    f0 = *(f32*)((u8*)r29 + 0x68);
    f0 = f0 - f1;
    *(f32*)((u8*)r29 + 0x68) = f0;
    goto L_800DC79C;
L_800DC788: ;
    if ((s32)r0 != (s32)0x1) goto L_800DC79C;
    f0 = *(f32*)((u8*)r29 + 0x68);
    f0 = f0 + f1;
    *(f32*)((u8*)r29 + 0x68) = f0;
L_800DC79C: ;
    if ((s32)r3 == (s32)0x1) goto L_800DC7F0;
    if ((s32)r3 >= (s32)0x1) goto L_800DC7B4;
    if ((s32)r3 >= (s32)0x0) goto L_800DC7C0;
    goto L_800DC840;
L_800DC7B4: ;
    if ((s32)r3 >= (s32)0x3) goto L_800DC840;
    goto L_800DC80C;
L_800DC7C0: ;
    f0 = *(f32*)lbl_8047CA74;
    f1 = *(f32*)((u8*)r29 + 0x68);
    f0 = f4 - f0;
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_800DC840;
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r29 + 0x70) = r3;
    *(u8*)((u8*)r29 + 0x71) = r0;
    *(f32*)((u8*)r29 + 0x68) = f0;
    goto L_800DC840;
L_800DC7F0: ;
    f0 = *(f32*)((u8*)r29 + 0x68);
    /* cror eq, gt, eq */;
    if (f0 != f4) goto L_800DC840;
    f0 = f0 - f4;
    *(f32*)((u8*)r29 + 0x68) = f0;
    goto L_800DC840;
L_800DC80C: ;
    f1 = *(f32*)((u8*)r29 + 0x68);
    /* cror eq, gt, eq */;
    if (f1 != f4) goto L_800DC828;
    r0 = -0x1;
    *(u8*)((u8*)r29 + 0x71) = r0;
    goto L_800DC840;
L_800DC828: ;
    f0 = *(f32*)lbl_8047CA78;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_800DC840;
    r0 = 0x1;
    *(u8*)((u8*)r29 + 0x71) = r0;
L_800DC840: ;
    r31 = r31 + 0x74;
    r30 = r30 + 0x1;
L_800DC848: ;
    r0 = *(u32*)lbl_8047AAF0;
    if ((u32)r30 < (u32)r0) goto L_800DC704;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}

/* fn_800DC874 | Size: 0x4 */
void fn_800DC874(void) {
    /* 4 bytes -- blr (empty function) */
}

/* fn_800DC878 | Size: 0x198 */
void fn_800DC878(void) {
    extern u8 lbl_8047AAF4[];
    extern u8 lbl_8047CA78[];
    extern u8 lbl_8047CA88[];
    extern void fn_800D37CC();
    extern void fn_801A426C();
    extern void fn_801A48F4();
    extern void fn_801A49C0();
    extern void fn_801A6370();
    extern void fn_801A66E0();
    extern void fn_801C028C();
    extern void fn_800DD128();
    extern void fn_801C027C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28((u32)sp), 0, qr0 */;
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
    if ((u32)r0 == (u32)0x0) goto L_800DC944;
    r3 = *(u32*)((u8*)r30 + 0xC);
    fn_801A66E0();
    r0 = *(u32*)((u8*)r30 + 0x58);
    if ((u32)r29 > (u32)r0) goto L_800DC944;
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
    r7 = 0x0;
    r3 = *(u32*)((u8*)r30 + 0xC);
    fn_801C028C();
    f0 = *(f32*)lbl_8047AAF4;
    *(f32*)((u8*)r30 + 0x6C) = f0;
L_800DC944: ;
    r0 = *(u8*)((u8*)r30 + 0x2);
    f0 = *(f32*)((u8*)r31 + 0x20);
    if ((u32)r0 == (u32)0x0) goto L_800DC958;
    *(f32*)((u8*)r30 + 0x68) = f0;
L_800DC958: ;
    r0 = *(u8*)((u8*)r30 + 0x2);
    f31 = *(f32*)((u8*)r31 + 0x24);
    if ((u32)r0 == (u32)0x0) goto L_800DC9A8;
    fn_800D37CC();
    if ((s32)r3 != (s32)0x32) goto L_800DC97C;
    f0 = *(f32*)lbl_8047CA88;
    f31 = f31 * f0;
L_800DC97C: ;
    *(f32*)((u8*)r30 + 0x64) = f31;
    r3 = (u32)fn_801C027C;
    r6 = (u32)fn_801C027C;
    r4 = (0x1 << 16);
    r3 = *(u32*)((u8*)r30 + 0xC);
    f1 = *(f32*)((u8*)r30 + 0x64);
    r4 = 0x7;
    r7 = 0x1;
    /* crset cr1eq */;
    fn_801C028C();
L_800DC9A8: ;
    r0 = *(u32*)((u8*)r31 + 0x28);
    *(u32*)((u8*)r30 + 0x5C) = r0;
    r0 = *(u8*)((u8*)r31 + 0x2);
    *(u8*)((u8*)r30 + 0x70) = r0;
    r0 = *(u8*)((u8*)r31 + 0x3);
    *(u8*)((u8*)r30 + 0x71) = r0;
    r0 = *(u8*)((u8*)r30 + 0x3);
    if ((u32)r0 == (u32)0x0) goto L_800DC9EC;
    r0 = *(u8*)((u8*)r30 + 0x2);
    if ((u32)r0 == (u32)0x0) goto L_800DC9EC;
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x3) = r3;
    *(u8*)((u8*)r30 + 0x70) = r0;
    *(u8*)((u8*)r30 + 0x71) = r3;
L_800DC9EC: ;
    /* psq_l f31, 0x28((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* fn_800DCA10 | Size: 0x8C */
void fn_800DCA10(void) {
    extern void fn_801A48B0();
    extern void fn_801A497C();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

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
    return;
}

/* fn_800DCAA4 | Size: 0xC */
void fn_800DCAA4(void* self) {
    *(u8*)((u8*)self + 0x3) = 0;
}

/* fn_800DCAB0 | Size: 0x24 */
void fn_800DCAB0(void* self) {
    u8* ptr = (u8*)self;
    if (ptr[0x2] == 0) return;
    ptr[0x3] = 1;
    ptr[0x70] = 0;
    ptr[0x71] = 1;
}

/* fn_800DCADC | Size: 0x14 */
void fn_800DCADC(void* self, f32 value) {
    u8* ptr = (u8*)self;
    if (ptr[0x2] == 0) return;
    *(f32*)(ptr + 0x68) = value;
}

/* fn_800DCAF0 | Size: 0x88 */
void fn_800DCAF0(void) {
    extern u8 lbl_8047CA88[];
    extern void fn_800D37CC();
    extern void fn_801C028C();
    extern void fn_801C027C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x10) = f31;
    /* psq_st f31, 0x18((u32)sp), 0, qr0 */;
    r31 = r3;
    f31 = f1;
    r0 = *(u8*)((u8*)r3 + 0x2);
    if ((u32)r0 == (u32)0x0) goto L_800DCB5C;
    fn_800D37CC();
    if ((s32)r3 != (s32)0x32) goto L_800DCB30;
    f0 = *(f32*)lbl_8047CA88;
    f31 = f31 * f0;
L_800DCB30: ;
    *(f32*)((u8*)r31 + 0x64) = f31;
    r3 = (u32)fn_801C027C;
    r6 = (u32)fn_801C027C;
    r4 = (0x1 << 16);
    r3 = *(u32*)((u8*)r31 + 0xC);
    f1 = *(f32*)((u8*)r31 + 0x64);
    r4 = 0x7;
    r7 = 0x1;
    /* crset cr1eq */;
    fn_801C028C();
L_800DCB5C: ;
    /* psq_l f31, 0x18((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x10);
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* fn_800DCB78 | Size: 0xB4 */
void fn_800DCB78(void) {
    extern u8 lbl_8047AAF4[];
    extern u8 lbl_8047CA78[];
    extern void fn_801A426C();
    extern void fn_801A6370();
    extern void fn_801A66E0();
    extern void fn_801C028C();
    extern void fn_800DD128();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r4;
    r30 = r3;
    r0 = *(u8*)((u8*)r3 + 0x2);
    if ((u32)r0 == (u32)0x0) goto L_800DCC14;
    r3 = *(u32*)((u8*)r30 + 0xC);
    fn_801A66E0();
    r0 = *(u32*)((u8*)r30 + 0x58);
    if ((u32)r31 > (u32)r0) goto L_800DCC14;
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
    r7 = 0x0;
    r3 = *(u32*)((u8*)r30 + 0xC);
    fn_801C028C();
    f0 = *(f32*)lbl_8047AAF4;
    *(f32*)((u8*)r30 + 0x6C) = f0;
L_800DCC14: ;
    return;
}

/* fn_800DCC3C | Size: 0x24 */
void fn_800DCC3C(void* self) {
    extern void fn_801A48F4();
    fn_801A48F4(*(u32*)((u8*)self + 0xC));
}

/* fn_800DCC60 | Size: 0x24 */
void fn_800DCC60(void* self) {
    extern void fn_801A49C0();
    fn_801A49C0(*(u32*)((u8*)self + 0xC));
}

/* fn_800DCC84 | Size: 0x6C */
void fn_800DCC84(void) {
    extern void fn_801A4A48();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    f2 = *(f32*)((u8*)r4 + 0x0);
    f1 = *(f32*)((u8*)r4 + 0x4);
    f0 = *(f32*)((u8*)r4 + 0x8);
    f2 = (f64)(s32)f2;
    f1 = (f64)(s32)f1;
    r4 = (u32)sp + 0x8;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x10) = f2;
    *(f64*)(sp + 0x18) = f1;
    r6 = *(u32*)(sp + 0x14);
    *(f64*)(sp + 0x20) = f0;
    r5 = *(u32*)(sp + 0x1C);
    r0 = *(u32*)(sp + 0x24);
    *(u8*)(sp + 0xC) = r6;
    *(u8*)(sp + 0xD) = r5;
    *(u8*)(sp + 0xE) = r0;
    r0 = *(u32*)(sp + 0xC);
    *(u32*)(sp + 0x8) = r0;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A4A48();
    return;
}

/* fn_800DCCF0 | Size: 0xA8 */
void fn_800DCCF0(void) {
    extern void fn_801A68F8();
    extern void fn_801A6910();
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    r4 = 0x3;
    r30 = r3;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A68F8();
    if ((s32)r31 == (s32)0x2) goto L_800DCD60;
    if ((s32)r31 >= (s32)0x2) goto L_800DCD34;
    if ((s32)r31 == (s32)0x0) goto L_800DCD40;
    if ((s32)r31 >= (s32)0x0) goto L_800DCD50;
    goto L_800DCD7C;
L_800DCD34: ;
    if ((s32)r31 >= (s32)0x4) goto L_800DCD7C;
    goto L_800DCD70;
L_800DCD40: ;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x0;
    fn_801A6910();
    goto L_800DCD7C;
L_800DCD50: ;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x1;
    fn_801A6910();
    goto L_800DCD7C;
L_800DCD60: ;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x2;
    fn_801A6910();
    goto L_800DCD7C;
L_800DCD70: ;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = 0x3;
    fn_801A6910();
L_800DCD7C: ;
    *(u32*)((u8*)r30 + 0x4) = r31;
    return;
}

/* fn_800DCD98 | Size: 0xB4 */
void fn_800DCD98(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r31 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r31 == (u32)0x0) goto L_800DCE28;
    r3 = (0x1 << 16);
    r4 = *(u16*)((u8*)r31 + 0x4);
    r0 = r0 & 0xFFFF;
    r0 = r0 - r4;
    /* cntlzw r0, r0 */;
    /* srwi. r0, r0, 5 */;
    if ((u32)r31 == (u32)0x0) goto L_800DCDE0;
    goto L_800DCDF0;
L_800DCDE0: ;
    /* cntlzw r0, r4 */;
    *(u16*)((u8*)r31 + 0x4) = r3;
    r0 = (u32)r0 >> 5;
L_800DCDF0: ;
    if ((s32)r0 == (s32)0x0) goto L_800DCE28;
    if ((u32)r31 == (u32)0x0) goto L_800DCE28;
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x30);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r4 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r12 = *(u32*)((u8*)r4 + 0x34);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800DCE28: ;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x1) = r0;
    *(u8*)((u8*)r30 + 0x0) = r0;
    return;
}

/* fn_800DCE4C | Size: 0x170 */
void fn_800DCE4C(void) {
    extern u8 lbl_8047AAEC[];
    extern u8 lbl_8047AAF0[];
    extern u8 lbl_8047AAF4[];
    extern u8 lbl_8047CA70[];
    extern u8 lbl_8047CA78[];
    extern void fn_801A426C();
    extern void fn_801A4344();
    extern void fn_801A6370();
    extern void fn_801A66E0();
    extern void fn_801C028C();
    extern void fn_800DD128();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = *(u32*)lbl_8047AAF0;
    r31 = *(u32*)lbl_8047AAEC;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_800DCE88;
L_800DCE70: ;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_800DCE80;
    goto L_800DCE8C;
L_800DCE80: ;
    r31 = r31 + 0x74;
    if (--ctr != 0) goto L_800DCE70;
L_800DCE88: ;
    r31 = 0x0;
L_800DCE8C: ;
    if ((u32)r31 != (u32)0x0) goto L_800DCE9C;
    r3 = 0x0;
    goto L_800DCFA8;
L_800DCE9C: ;
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
    if ((u32)r0 == (u32)0x0) goto L_800DCFA0;
    *(u8*)((u8*)r31 + 0x2) = r5;
    f0 = *(f32*)lbl_8047CA70;
    *(f32*)((u8*)r31 + 0x64) = f0;
    *(u32*)((u8*)r31 + 0x5C) = r5;
    *(u8*)((u8*)r31 + 0x70) = r4;
    *(u32*)((u8*)r31 + 0x58) = r4;
    goto L_800DCEFC;
L_800DCEF0: ;
    r3 = *(u32*)((u8*)r31 + 0x58);
    r0 = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x58) = r0;
L_800DCEFC: ;
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)((u8*)r31 + 0x58);
    r3 = *(u32*)((u8*)r3 + 0x4);
    r0 = r0 << 2;
    /* lwzx r0, r3, r0 */;
    if ((u32)r0 != (u32)0x0) goto L_800DCEF0;
    r0 = *(u8*)((u8*)r31 + 0x2);
    if ((u32)r0 == (u32)0x0) goto L_800DCFA4;
    r3 = *(u32*)((u8*)r31 + 0xC);
    fn_801A66E0();
    r0 = *(u32*)((u8*)r31 + 0x58);
    if ((u32)r0 < (u32)0x0) goto L_800DCFA4;
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
    r7 = 0x0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    fn_801C028C();
    f0 = *(f32*)lbl_8047AAF4;
    *(f32*)((u8*)r31 + 0x6C) = f0;
    goto L_800DCFA4;
L_800DCFA0: ;
    *(u8*)((u8*)r31 + 0x2) = r4;
L_800DCFA4: ;
    r3 = r31;
L_800DCFA8: ;
    return;
}

/* fn_800DCFBC | Size: 0xFC */
void fn_800DCFBC(void) {
    extern u8 lbl_8047AAEC[];
    extern u8 lbl_8047AAF0[];
    extern u8 lbl_8047CA78[];
    extern u8 lbl_8047CA8C[];
    extern void fn_801A4344();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = *(u32*)lbl_8047AAF0;
    r31 = *(u32*)lbl_8047AAEC;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_800DCFF8;
L_800DCFE0: ;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_800DCFF0;
    goto L_800DCFFC;
L_800DCFF0: ;
    r31 = r31 + 0x74;
    if (--ctr != 0) goto L_800DCFE0;
L_800DCFF8: ;
    r31 = 0x0;
L_800DCFFC: ;
    if ((u32)r31 != (u32)0x0) goto L_800DD00C;
    r3 = 0x0;
    goto L_800DD0A4;
L_800DD00C: ;
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
L_800DD0A4: ;
    return;
}

/* fn_800DD0B8 | Size: 0x70 */
void fn_800DD0B8(void) {
    extern u8 lbl_8047AAE8[];
    extern u8 lbl_8047AAEC[];
    extern u8 lbl_8047AAF0[];
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r0 = r3;
    r3 = r0 * 0x74;
    *(u32*)lbl_8047AAF0 = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAE8 = r3;
    if ((s32)r0 == (s32)0) goto L_800DD118;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AAEC = r3;
    r4 = r5;
    r6 = 0x0;
    goto L_800DD10C;
L_800DD0FC: ;
    r3 = *(u32*)lbl_8047AAEC;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x74;
L_800DD10C: ;
    r0 = *(u32*)lbl_8047AAF0;
    if ((u32)r6 < (u32)r0) goto L_800DD0FC;
L_800DD118: ;
    return;
}

/* fn_800DD128 | Size: 0x4C */
void fn_800DD128(u8* obj) {
    extern u8 lbl_8047AAF4[];
    extern u8 lbl_8047CA70[];
    extern u8 lbl_8047CA90[];
    extern u8 lbl_8047CA98[];
    extern void fn_80196E10();
    if (obj == 0) {
        fn_80196E10(lbl_8047CA90, 0xAB, lbl_8047CA98);
    }
    *(f32*)lbl_8047AAF4 = *(f32*)lbl_8047CA70 + *(f32*)((u8*)obj + 0xC);
}

/* fn_800DD174 | Size: 0xFC */
void fn_800DD174(void) {
    extern u8 lbl_8047AAEC[];
    extern u8 lbl_8047AAF0[];
    extern void fn_801A4B00();
    extern void fn_801A4D20();
    extern void fn_801A4F54();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    r3 = 0x0;
    fn_801A4B00();
    r5 = *(u32*)lbl_8047AAEC;
    r3 = 0x0;
    r0 = *(u32*)lbl_8047AAF0;
    r4 = r5;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_800DD1D4;
L_800DD1AC: ;
    r0 = *(u8*)((u8*)r4 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DD1C8;
    r0 = *(u8*)((u8*)r4 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_800DD1C8;
    goto L_800DD1D8;
L_800DD1C8: ;
    r4 = r4 + 0x74;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_800DD1AC;
L_800DD1D4: ;
    r3 = -0x1;
L_800DD1D8: ;
    if ((s32)r3 == (s32)-0x1) goto L_800DD254;
    r6 = r3 * 0x74;
    r7 = r3 + 0x1;
    r4 = r7 * 0x74;
    r5 = r5 + r6;
    goto L_800DD22C;
L_800DD1F4: ;
    r0 = *(u32*)lbl_8047AAEC;
    r8 = r0 + r4;
    r0 = *(u8*)((u8*)r8 + 0x0);
    if ((u32)r0 != (u32)0x1) goto L_800DD224;
    r0 = *(u8*)((u8*)r8 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_800DD224;
    r3 = *(u32*)((u8*)r5 + 0xC);
    r5 = r8;
    r0 = *(u32*)((u8*)r8 + 0xC);
    *(u32*)((u8*)r3 + 0xC) = r0;
L_800DD224: ;
    r4 = r4 + 0x74;
    r7 = r7 + 0x1;
L_800DD22C: ;
    r0 = *(u32*)lbl_8047AAF0;
    if ((u32)r7 < (u32)r0) goto L_800DD1F4;
    r3 = *(u32*)((u8*)r5 + 0xC);
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0xC) = r0;
    r0 = *(u32*)lbl_8047AAEC;
    r3 = r0 + r6;
    r3 = *(u32*)((u8*)r3 + 0xC);
    fn_801A4D20();
L_800DD254: ;
    r3 = r31;
    fn_801A4F54();
    return;
}

/* fn_800DD270 | Size: 0x114 */
void fn_800DD270(void) {
    extern u8 lbl_8047AAF8[];
    extern u8 lbl_8047AAFA[];
    extern u8 lbl_8047AAFC[];
    extern u32 lbl_8047AB08;
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    r0 = *(u16*)lbl_8047AAF8;
    if ((u32)r0 != (u32)0x0) goto L_800DD298;
    r3 = 0x0;
    goto L_800DD370;
L_800DD298: ;
    r0 = *(u32*)lbl_8047AB08;
    if ((u32)r31 <= (u32)r0) goto L_800DD2AC;
    r3 = 0x0;
    goto L_800DD370;
L_800DD2AC: ;
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E27B0();
    r6 = 0x0;
    r7 = 0x0;
    if ((u32)r31 <= (u32)0x0) goto L_800DD35C;
    if ((u32)r31 <= (u32)0x8) goto L_800DD334;
    r0 = r4 + 0x7;
    r5 = r3;
    r0 = (u32)r0 >> 3;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r4 <= (u32)0x0) goto L_800DD334;
L_800DD2E8: ;
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
    if (--ctr != 0) goto L_800DD2E8;
L_800DD334: ;
    r4 = r7 << 1;
    r0 = r31 - r7;
    r3 = r3 + r4;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r7 >= (u32)r31) goto L_800DD35C;
L_800DD34C: ;
    r0 = *(u16*)((u8*)r3 + 0x0);
    r3 = r3 + 0x2;
    r6 = r6 + r0;
    if (--ctr != 0) goto L_800DD34C;
L_800DD35C: ;
    r0 = *(u32*)lbl_8047AAFC;
    r3 = *(u16*)lbl_8047AAFA;
    r31 = r0 + r6;
    fn_800E24B0();
    r3 = r31;
L_800DD370: ;
    return;
}

/* fn_800DD38C | Size: 0x5E4 */
void fn_800DD38C(void) {
    extern u8 lbl_802704B4[];
    extern u8 lbl_80400F30[];
    extern u8 lbl_80400F44[];
    extern u8 lbl_8047AAFC[];
    extern u8 lbl_8047AB00[];
    extern u8 lbl_8047AB04[];
    extern u32 lbl_8047AB08;
    extern u8 lbl_8047AB0C[];
    extern u8 lbl_8047AB11[];
    extern void fn_800A2998();
    extern void fn_800DE09C();
    extern void fn_800DE680();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    if ((s32)r0 != (s32)0) goto L_800DD3C8;
    *(f64*)(sp + 0x28) = f1;
    *(f64*)(sp + 0x30) = f2;
    *(f64*)(sp + 0x38) = f3;
    *(f64*)(sp + 0x40) = f4;
    *(f64*)(sp + 0x48) = f5;
    *(f64*)(sp + 0x50) = f6;
    *(f64*)(sp + 0x58) = f7;
    *(f64*)(sp + 0x60) = f8;
L_800DD3C8: ;
    r0 = *(u8*)lbl_8047AB11;
    r12 = (u32)sp + 0xb8;
    r11 = (u32)sp + 0x8;
    r31 = (0x100 << 16);
    if ((u32)r0 == (u32)0x0) goto L_800DD44C;
    OSGetTime();
    r5 = (u32)sp + 0x74;
    fn_800A2998();
    r4 = *(u32*)(sp + 0x84);
    r3 = (u32)lbl_80400F30;
    r5 = (u32)lbl_802704B4;
    r7 = *(u32*)(sp + 0x80);
    r6 = r4 + 0x1;
    r8 = *(u32*)(sp + 0x7C);
    r9 = *(u32*)(sp + 0x78);
    r3 = (u32)lbl_80400F30;
    r10 = *(u32*)(sp + 0x74);
    r5 = (u32)lbl_802704B4;
    r4 = 0x14;
    fn_800DE09C();
L_800DD44C: ;
    r3 = (u32)lbl_80400F44;
    r5 = r30;
    r3 = (u32)lbl_80400F44;
    r6 = (u32)sp + 0x68;
    r4 = 0xff;
    fn_800DE680();
    r0 = *(u32*)lbl_8047AAFC;
    r3 = (u32)lbl_80400F44;
    r4 = 0x0;
    *(u8*)((u8*)r3 + 0xFF) = r4;
    if ((u32)r0 == (u32)0x0) goto L_800DD934;
    r0 = *(u8*)lbl_8047AB11;
    if ((u32)r0 == (u32)0x0) goto L_800DD4B0;
    strlen();
    r4 = (u32)lbl_80400F30;
    r31 = r3;
    r3 = (u32)lbl_80400F30;
    strlen();
    r0 = r31 + 0x1;
    r0 = r3 + r0;
    r31 = r0 & 0xFFFF;
    goto L_800DD4BC;
L_800DD4B0: ;
    strlen();
    r0 = r3 + 0x1;
    r31 = r0 & 0xFFFF;
L_800DD4BC: ;
    r3 = r31 & 0xFFFF;
L_800DD4C0: ;
    r9 = *(u32*)lbl_8047AB08;
    r0 = 0x1;
    r4 = *(u32*)lbl_8047AB0C;
    if ((u32)r9 < (u32)r4) goto L_800DD4D8;
    r0 = 0x0;
L_800DD4D8: ;
    r7 = 0x0;
    r10 = *(u32*)lbl_8047AB00;
    r6 = r7;
    if ((u32)r9 <= (u32)0x0) goto L_800DD588;
    if ((u32)r9 <= (u32)0x8) goto L_800DD55C;
    r4 = r5 + 0x7;
    r8 = r10;
    r4 = (u32)r4 >> 3;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r5 <= (u32)0x0) goto L_800DD55C;
L_800DD510: ;
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
    if (--ctr != 0) goto L_800DD510;
L_800DD55C: ;
    r5 = r6 << 1;
    r4 = r9 - r6;
    r5 = r10 + r5;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 >= (u32)r9) goto L_800DD588;
L_800DD574: ;
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    r6 = r6 + 0x1;
    r7 = r7 + r4;
    if (--ctr != 0) goto L_800DD574;
L_800DD588: ;
    r5 = *(u32*)lbl_8047AB04;
    r4 = r3 + r7;
    if ((u32)r4 < (u32)r5) goto L_800DD59C;
    r0 = 0x0;
L_800DD59C: ;
    r4 = r0 & 0xFF;
    if ((u32)r4 != (u32)r5) goto L_800DD7FC;
    r4 = *(u16*)((u8*)r10 + 0x0);
    r8 = *(u32*)lbl_8047AAFC;
    r6 = r5 - r4;
    r7 = r8 + r4;
    /* subf. r5, r8, r7 */;
    r4 = r7 - r8;
    r5 = -r4;
    if ((u32)r4 == (u32)r5) goto L_800DD5C8;
    r5 = r4;
L_800DD5C8: ;
    if ((u32)r5 < (u32)0x4) goto L_800DD6E0;
    /* srwi. r4, r6, 2 */;
    r6 = r6 & 0x3;
    r5 = r4;
    if ((u32)r5 == (u32)0x4) goto L_800DD658;
    /* srwi. r4, r4, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r5 == (u32)0x4) goto L_800DD640;
L_800DD5EC: ;
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
    if (--ctr != 0) goto L_800DD5EC;
    r5 = r5 & 0x7;
    if ((u32)r5 == (u32)0x4) goto L_800DD658;
L_800DD640: ;
    ctr_fn = (void(*)(void))r5;
L_800DD644: ;
    r4 = *(u32*)((u8*)r7 + 0x0);
    r7 = r7 + 0x4;
    *(u32*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x4;
    if (--ctr != 0) goto L_800DD644;
L_800DD658: ;
    r5 = r6;
    if ((u32)r6 == (u32)0x0) goto L_800DD764;
    /* srwi. r4, r6, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 == (u32)0x0) goto L_800DD6C4;
L_800DD670: ;
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
    if (--ctr != 0) goto L_800DD670;
    r5 = r5 & 0x7;
    if ((u32)r6 == (u32)0x0) goto L_800DD764;
L_800DD6C4: ;
    ctr_fn = (void(*)(void))r5;
L_800DD6C8: ;
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto L_800DD6C8;
    goto L_800DD764;
L_800DD6E0: ;
    r5 = r6;
    if ((u32)r6 == (u32)0x0) goto L_800DD764;
    /* srwi. r4, r6, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 == (u32)0x0) goto L_800DD74C;
L_800DD6F8: ;
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
    if (--ctr != 0) goto L_800DD6F8;
    r5 = r5 & 0x7;
    if ((u32)r6 == (u32)0x0) goto L_800DD764;
L_800DD74C: ;
    ctr_fn = (void(*)(void))r5;
L_800DD750: ;
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto L_800DD750;
L_800DD764: ;
    r4 = *(u32*)lbl_8047AB08;
    r7 = *(u32*)lbl_8047AB00;
    /* subic. r6, r4, 0x1 */;
    r5 = r7 + 0x2;
    if ((u32)r6 == (u32)0x0) goto L_800DD7F0;
    /* srwi. r4, r6, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 == (u32)0x0) goto L_800DD7D8;
L_800DD784: ;
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
    if (--ctr != 0) goto L_800DD784;
    r6 = r6 & 0x7;
    if ((u32)r6 == (u32)0x0) goto L_800DD7F0;
L_800DD7D8: ;
    ctr_fn = (void(*)(void))r6;
L_800DD7DC: ;
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    *(u16*)((u8*)r7 + 0x0) = r4;
    r7 = r7 + 0x2;
    if (--ctr != 0) goto L_800DD7DC;
L_800DD7F0: ;
    r4 = *(u32*)lbl_8047AB08;
    *(u32*)lbl_8047AB08 = r4;
L_800DD7FC: ;
    r0 = r0 & 0xFF;
    if ((u32)r6 == (u32)0x0) goto L_800DD4C0;
    r8 = *(u32*)lbl_8047AB08;
    r5 = 0x0;
    r6 = *(u32*)lbl_8047AB00;
    r4 = r5;
    if ((u32)r8 <= (u32)0x0) goto L_800DD8B4;
    if ((u32)r8 <= (u32)0x8) goto L_800DD88C;
    r0 = r3 + 0x7;
    r7 = r6;
    r0 = (u32)r0 >> 3;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r3 <= (u32)0x0) goto L_800DD88C;
L_800DD840: ;
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
    if (--ctr != 0) goto L_800DD840;
L_800DD88C: ;
    r3 = r4 << 1;
    r0 = r8 - r4;
    r3 = r6 + r3;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r4 >= (u32)r8) goto L_800DD8B4;
L_800DD8A4: ;
    r0 = *(u16*)((u8*)r3 + 0x0);
    r3 = r3 + 0x2;
    r5 = r5 + r0;
    if (--ctr != 0) goto L_800DD8A4;
L_800DD8B4: ;
    r0 = *(u8*)lbl_8047AB11;
    r3 = *(u32*)lbl_8047AAFC;
    r30 = r3 + r5;
    if ((u32)r0 == (u32)0x0) goto L_800DD8F8;
    r3 = (u32)lbl_80400F30;
    strlen();
    r4 = (u32)lbl_80400F30;
    r5 = r3;
    r4 = (u32)lbl_80400F30;
    r3 = r30;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = (u32)lbl_80400F30;
    strlen();
    r30 = r30 + r3;
L_800DD8F8: ;
    r3 = (u32)lbl_80400F44;
    strlen();
    r5 = r3;
    r4 = (u32)lbl_80400F44;
    r3 = r30;
    r5 = r5 + 0x1;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AB08;
    r5 = *(u32*)lbl_8047AB00;
    r3 = r4 + 0x1;
    r0 = r4 << 1;
    *(u32*)lbl_8047AB08 = r3;
    /* sthx r31, r5, r0 */;
L_800DD934: ;
    r0 = *(u8*)lbl_8047AB11;
    if ((u32)r0 == (u32)0x0) goto L_800DD94C;
    r3 = (u32)lbl_80400F30;
    strlen();
L_800DD94C: ;
    r3 = (u32)lbl_80400F44;
    strlen();
    r31 = *(u32*)(sp + 0xAC);
    r30 = *(u32*)(sp + 0xA8);
    return;
}

/* fn_800DD970 -- OSReport/GSlog | Size: 0x5E4 */
void fn_800DD970_impl(void) {
    extern u8 lbl_802704B4[];
    extern u8 lbl_80401044[];
    extern u8 lbl_80401058[];
    extern u8 lbl_8047AAFC[];
    extern u8 lbl_8047AB00[];
    extern u8 lbl_8047AB04[];
    extern u32 lbl_8047AB08;
    extern u8 lbl_8047AB0C[];
    extern u8 lbl_8047AB11[];
    extern void fn_800A2998();
    extern void fn_800DE09C();
    extern void fn_800DE128();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    if ((s32)r0 != (s32)0) goto L_800DD9AC;
    *(f64*)(sp + 0x28) = f1;
    *(f64*)(sp + 0x30) = f2;
    *(f64*)(sp + 0x38) = f3;
    *(f64*)(sp + 0x40) = f4;
    *(f64*)(sp + 0x48) = f5;
    *(f64*)(sp + 0x50) = f6;
    *(f64*)(sp + 0x58) = f7;
    *(f64*)(sp + 0x60) = f8;
L_800DD9AC: ;
    r0 = *(u8*)lbl_8047AB11;
    r12 = (u32)sp + 0xb8;
    r11 = (u32)sp + 0x8;
    r31 = (0x100 << 16);
    if ((u32)r0 == (u32)0x0) goto L_800DDA30;
    OSGetTime();
    r5 = (u32)sp + 0x74;
    fn_800A2998();
    r4 = *(u32*)(sp + 0x84);
    r3 = (u32)lbl_80401044;
    r5 = (u32)lbl_802704B4;
    r7 = *(u32*)(sp + 0x80);
    r6 = r4 + 0x1;
    r8 = *(u32*)(sp + 0x7C);
    r9 = *(u32*)(sp + 0x78);
    r3 = (u32)lbl_80401044;
    r10 = *(u32*)(sp + 0x74);
    r5 = (u32)lbl_802704B4;
    r4 = 0x14;
    fn_800DE09C();
L_800DDA30: ;
    r3 = (u32)lbl_80401058;
    r5 = r30;
    r3 = (u32)lbl_80401058;
    r6 = (u32)sp + 0x68;
    r4 = 0xff;
    fn_800DE128();
    r0 = *(u32*)lbl_8047AAFC;
    r3 = (u32)lbl_80401058;
    r4 = 0x0;
    *(u8*)((u8*)r3 + 0xFF) = r4;
    if ((u32)r0 == (u32)0x0) goto L_800DDF18;
    r0 = *(u8*)lbl_8047AB11;
    if ((u32)r0 == (u32)0x0) goto L_800DDA94;
    strlen();
    r4 = (u32)lbl_80401044;
    r31 = r3;
    r3 = (u32)lbl_80401044;
    strlen();
    r0 = r31 + 0x1;
    r0 = r3 + r0;
    r31 = r0 & 0xFFFF;
    goto L_800DDAA0;
L_800DDA94: ;
    strlen();
    r0 = r3 + 0x1;
    r31 = r0 & 0xFFFF;
L_800DDAA0: ;
    r3 = r31 & 0xFFFF;
L_800DDAA4: ;
    r9 = *(u32*)lbl_8047AB08;
    r0 = 0x1;
    r4 = *(u32*)lbl_8047AB0C;
    if ((u32)r9 < (u32)r4) goto L_800DDABC;
    r0 = 0x0;
L_800DDABC: ;
    r7 = 0x0;
    r10 = *(u32*)lbl_8047AB00;
    r6 = r7;
    if ((u32)r9 <= (u32)0x0) goto L_800DDB6C;
    if ((u32)r9 <= (u32)0x8) goto L_800DDB40;
    r4 = r5 + 0x7;
    r8 = r10;
    r4 = (u32)r4 >> 3;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r5 <= (u32)0x0) goto L_800DDB40;
L_800DDAF4: ;
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
    if (--ctr != 0) goto L_800DDAF4;
L_800DDB40: ;
    r5 = r6 << 1;
    r4 = r9 - r6;
    r5 = r10 + r5;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 >= (u32)r9) goto L_800DDB6C;
L_800DDB58: ;
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    r6 = r6 + 0x1;
    r7 = r7 + r4;
    if (--ctr != 0) goto L_800DDB58;
L_800DDB6C: ;
    r5 = *(u32*)lbl_8047AB04;
    r4 = r3 + r7;
    if ((u32)r4 < (u32)r5) goto L_800DDB80;
    r0 = 0x0;
L_800DDB80: ;
    r4 = r0 & 0xFF;
    if ((u32)r4 != (u32)r5) goto L_800DDDE0;
    r4 = *(u16*)((u8*)r10 + 0x0);
    r8 = *(u32*)lbl_8047AAFC;
    r6 = r5 - r4;
    r7 = r8 + r4;
    /* subf. r5, r8, r7 */;
    r4 = r7 - r8;
    r5 = -r4;
    if ((u32)r4 == (u32)r5) goto L_800DDBAC;
    r5 = r4;
L_800DDBAC: ;
    if ((u32)r5 < (u32)0x4) goto L_800DDCC4;
    /* srwi. r4, r6, 2 */;
    r6 = r6 & 0x3;
    r5 = r4;
    if ((u32)r5 == (u32)0x4) goto L_800DDC3C;
    /* srwi. r4, r4, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r5 == (u32)0x4) goto L_800DDC24;
L_800DDBD0: ;
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
    if (--ctr != 0) goto L_800DDBD0;
    r5 = r5 & 0x7;
    if ((u32)r5 == (u32)0x4) goto L_800DDC3C;
L_800DDC24: ;
    ctr_fn = (void(*)(void))r5;
L_800DDC28: ;
    r4 = *(u32*)((u8*)r7 + 0x0);
    r7 = r7 + 0x4;
    *(u32*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x4;
    if (--ctr != 0) goto L_800DDC28;
L_800DDC3C: ;
    r5 = r6;
    if ((u32)r6 == (u32)0x0) goto L_800DDD48;
    /* srwi. r4, r6, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 == (u32)0x0) goto L_800DDCA8;
L_800DDC54: ;
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
    if (--ctr != 0) goto L_800DDC54;
    r5 = r5 & 0x7;
    if ((u32)r6 == (u32)0x0) goto L_800DDD48;
L_800DDCA8: ;
    ctr_fn = (void(*)(void))r5;
L_800DDCAC: ;
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto L_800DDCAC;
    goto L_800DDD48;
L_800DDCC4: ;
    r5 = r6;
    if ((u32)r6 == (u32)0x0) goto L_800DDD48;
    /* srwi. r4, r6, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 == (u32)0x0) goto L_800DDD30;
L_800DDCDC: ;
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
    if (--ctr != 0) goto L_800DDCDC;
    r5 = r5 & 0x7;
    if ((u32)r6 == (u32)0x0) goto L_800DDD48;
L_800DDD30: ;
    ctr_fn = (void(*)(void))r5;
L_800DDD34: ;
    r4 = *(u8*)((u8*)r7 + 0x0);
    r7 = r7 + 0x1;
    *(u8*)((u8*)r8 + 0x0) = r4;
    r8 = r8 + 0x1;
    if (--ctr != 0) goto L_800DDD34;
L_800DDD48: ;
    r4 = *(u32*)lbl_8047AB08;
    r7 = *(u32*)lbl_8047AB00;
    /* subic. r6, r4, 0x1 */;
    r5 = r7 + 0x2;
    if ((u32)r6 == (u32)0x0) goto L_800DDDD4;
    /* srwi. r4, r6, 3 */;
    ctr_fn = (void(*)(void))r4;
    if ((u32)r6 == (u32)0x0) goto L_800DDDBC;
L_800DDD68: ;
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
    if (--ctr != 0) goto L_800DDD68;
    r6 = r6 & 0x7;
    if ((u32)r6 == (u32)0x0) goto L_800DDDD4;
L_800DDDBC: ;
    ctr_fn = (void(*)(void))r6;
L_800DDDC0: ;
    r4 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    *(u16*)((u8*)r7 + 0x0) = r4;
    r7 = r7 + 0x2;
    if (--ctr != 0) goto L_800DDDC0;
L_800DDDD4: ;
    r4 = *(u32*)lbl_8047AB08;
    *(u32*)lbl_8047AB08 = r4;
L_800DDDE0: ;
    r0 = r0 & 0xFF;
    if ((u32)r6 == (u32)0x0) goto L_800DDAA4;
    r8 = *(u32*)lbl_8047AB08;
    r5 = 0x0;
    r6 = *(u32*)lbl_8047AB00;
    r4 = r5;
    if ((u32)r8 <= (u32)0x0) goto L_800DDE98;
    if ((u32)r8 <= (u32)0x8) goto L_800DDE70;
    r0 = r3 + 0x7;
    r7 = r6;
    r0 = (u32)r0 >> 3;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r3 <= (u32)0x0) goto L_800DDE70;
L_800DDE24: ;
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
    if (--ctr != 0) goto L_800DDE24;
L_800DDE70: ;
    r3 = r4 << 1;
    r0 = r8 - r4;
    r3 = r6 + r3;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r4 >= (u32)r8) goto L_800DDE98;
L_800DDE88: ;
    r0 = *(u16*)((u8*)r3 + 0x0);
    r3 = r3 + 0x2;
    r5 = r5 + r0;
    if (--ctr != 0) goto L_800DDE88;
L_800DDE98: ;
    r0 = *(u8*)lbl_8047AB11;
    r3 = *(u32*)lbl_8047AAFC;
    r30 = r3 + r5;
    if ((u32)r0 == (u32)0x0) goto L_800DDEDC;
    r3 = (u32)lbl_80401044;
    strlen();
    r4 = (u32)lbl_80401044;
    r5 = r3;
    r4 = (u32)lbl_80401044;
    r3 = r30;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = (u32)lbl_80401044;
    strlen();
    r30 = r30 + r3;
L_800DDEDC: ;
    r3 = (u32)lbl_80401058;
    strlen();
    r5 = r3;
    r4 = (u32)lbl_80401058;
    r3 = r30;
    r5 = r5 + 0x1;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = *(u32*)lbl_8047AB08;
    r5 = *(u32*)lbl_8047AB00;
    r3 = r4 + 0x1;
    r0 = r4 << 1;
    *(u32*)lbl_8047AB08 = r3;
    /* sthx r31, r5, r0 */;
L_800DDF18: ;
    r0 = *(u8*)lbl_8047AB11;
    if ((u32)r0 == (u32)0x0) goto L_800DDF30;
    r3 = (u32)lbl_80401044;
    strlen();
L_800DDF30: ;
    r3 = (u32)lbl_80401058;
    strlen();
    r31 = *(u32*)(sp + 0xAC);
    r30 = *(u32*)(sp + 0xA8);
    return;
}

/* fn_800DDF54 | Size: 0x148 */
void fn_800DDF54(void) {
    extern u8 lbl_8047AAF8[];
    extern u8 lbl_8047AAFA[];
    extern u8 lbl_8047AAFC[];
    extern u8 lbl_8047AB00[];
    extern u8 lbl_8047AB04[];
    extern u8 lbl_8047AB0C[];
    extern u8 lbl_8047AB10[];
    extern u8 lbl_8047AB11[];
    extern void fn_800DD970();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = (u32)lbl_802704A0;
    r0 = 0x0;
    r31 = (u32)lbl_802704A0;
    *(u8*)lbl_8047AB10 = r0;
    *(u32*)lbl_8047AB04 = r30;
    *(u8*)lbl_8047AB11 = r4;
    if ((s32)r0 != (s32)0) goto L_800DDF9C;
    r3 = r31 + 0x2c;
    fn_800DD970();
    r3 = 0x1;
    goto L_800DE084;
L_800DDF9C: ;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAF8 = r3;
    if ((s32)r0 != (s32)0) goto L_800DDFC0;
    r3 = r31 + 0x48;
    fn_800DD970();
    r3 = 0x0;
    goto L_800DE084;
L_800DDFC0: ;
    r0 = (u32)r30 >> 7;
    *(u32*)lbl_8047AB0C = r0;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AAFA = r3;
    if ((s32)r0 != (s32)0) goto L_800DDFF8;
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E209C();
    r3 = r31 + 0x48;
    fn_800DD970();
    r3 = 0x0;
    goto L_800DE084;
L_800DDFF8: ;
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E27B0();
    *(u32*)lbl_8047AAFC = r3;
    if ((u32)r3 != (u32)0x0) goto L_800DE030;
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E209C();
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E209C();
    r3 = r31 + 0x48;
    fn_800DD970();
    r3 = 0x0;
    goto L_800DE084;
L_800DE030: ;
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E27B0();
    *(u32*)lbl_8047AB00 = r3;
    if ((u32)r3 != (u32)0x0) goto L_800DE070;
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E24B0();
    r3 = *(u16*)lbl_8047AAF8;
    fn_800E209C();
    r3 = *(u16*)lbl_8047AAFA;
    fn_800E209C();
    r3 = r31 + 0x48;
    fn_800DD970();
    r3 = 0x0;
    goto L_800DE084;
L_800DE070: ;
    r4 = *(u32*)lbl_8047AB04;
    r3 = r31 + 0x5c;
    fn_800DD970();
    r3 = 0x1;
L_800DE084: ;
    return;
}

/* fn_800DE09C | Size: 0x8C */
void fn_800DE09C(void) {
    extern void fn_800DE128();
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r6 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;

    if ((s32)r0 != (s32)0) goto L_800DE0D0;
    *(f64*)(sp + 0x28) = f1;
    *(f64*)(sp + 0x30) = f2;
    *(f64*)(sp + 0x38) = f3;
    *(f64*)(sp + 0x40) = f4;
    *(f64*)(sp + 0x48) = f5;
    *(f64*)(sp + 0x50) = f6;
    *(f64*)(sp + 0x58) = f7;
    *(f64*)(sp + 0x60) = f8;
L_800DE0D0: ;
    r11 = (u32)sp + 0x88;
    r0 = (u32)sp + 0x8;
    r12 = (0x300 << 16);
    r31 = (u32)sp + 0x68;
    r6 = r31;
    *(u32*)(sp + 0x70) = r0;
    fn_800DE128();
    r31 = *(u32*)(sp + 0x7C);
    return;
}

/* fn_800DE128 | Size: 0x558 */
void fn_800DE128(void) {
    extern u8 lbl_80401168[];
    extern u8 lbl_80401178[];
    extern u8 lbl_80478AE8[];
    extern u8 lbl_8047CAA0[];
    extern u8 lbl_8047CAA8[];
    extern u8 jumptable_80315388[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r0 = 0x0;
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
    goto L_800DE65C;
L_800DE16C: ;
    r3 = r27 & 0xFF;
    if ((s32)r0 != (s32)0) goto L_800DE1C0;
    r4 = *(u8*)((u8*)r31 + 0x0);
    if ((s32)r4 != (s32)0x25) goto L_800DE1B4;
    r3 = *(u8*)((u8*)r31 + 0x1);
    if ((s32)r3 != (s32)0x25) goto L_800DE1A0;
    r3 = 0x25;
    r31 = r31 + 0x1;
    *(u8*)((u8*)r30 + 0x0) = r3;
    r30 = r30 + 0x1;
    goto L_800DE63C;
L_800DE1A0: ;
    r3 = (u32)lbl_80401178;
    r27 = 0x1;
    r3 = (u32)lbl_80401178;
    r29 = r3;
    goto L_800DE63C;
L_800DE1B4: ;
    *(u8*)((u8*)r30 + 0x0) = r4;
    r30 = r30 + 0x1;
    goto L_800DE63C;
L_800DE1C0: ;
    r5 = *(u8*)((u8*)r31 + 0x0);
    r3 = (s8)r5;
    if ((u32)r4 > (u32)0x20) goto L_800DE4D8;
    r3 = (u32)jumptable_80315388;
    r4 = r4 << 2;
    r3 = (u32)jumptable_80315388;
    /* lwzx r3, r3, r4 */;
    ctr_fn = (void(*)(void))r3;
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
    goto L_800DE4E0;
    r3 = r24;
    r4 = 0x1;
    __va_arg();
    r0 = *(u32*)((u8*)r3 + 0x0);
    r3 = (u32)lbl_80401168;
    r8 = 0x0;
    r9 = r0;
    if ((s32)r0 >= (s32)0x0) goto L_800DE254;
    r8 = 0x1;
    r9 = -r0;
L_800DE254: ;
    r4 = (0x6666 << 16);
    r7 = *(u32*)lbl_80478AE8;
    r6 = r4 + 0x6667;
L_800DE260: ;
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
    if ((s32)r0 != (s32)0x0) goto L_800DE260;
    r0 = r8 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800DE2AC;
    r0 = 0x2d;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r3 + 0x1;
L_800DE2AC: ;
    r0 = 0x0;
    r4 = (u32)lbl_80401168;
    r20 = (u32)lbl_80401168;
    *(u8*)((u8*)r3 + 0x0) = r0;
    r3 = r20;
    strlen();
    r4 = (u32)lbl_80401168;
    r0 = (u32)lbl_80401168;
    r4 = r3 + r0;
    goto L_800DE310;
L_800DE2D8: ;
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r20 = r20 + 0x1;
L_800DE310: ;
    if ((u32)r20 < (u32)r4) goto L_800DE2D8;
    r3 = (u32)lbl_80401168;
    r0 = 0x1;
    r3 = (u32)lbl_80401168;
    r20 = r3;
    goto L_800DE4E0;
    r3 = r24;
    r4 = 0x3;
    __va_arg();
    r20 = (u32)lbl_8047CAA0;
    r0 = 0x1;
    goto L_800DE4E0;
    r3 = r24;
    r4 = 0x1;
    __va_arg();
    r20 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r20 != (u32)0x0) goto L_800DE360;
    r20 = (u32)lbl_8047CAA8;
L_800DE360: ;
    r0 = 0x1;
    goto L_800DE4E0;
    r3 = r24;
    r4 = 0x1;
    __va_arg();
    r0 = *(u8*)((u8*)r31 + 0x0);
    r6 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x58) goto L_800DE41C;
    r4 = (u32)lbl_80401168;
    r3 = *(u32*)lbl_80478AE8;
    r4 = (u32)lbl_80401168;
    r5 = r6;
L_800DE394: ;
    r0 = r5 & 0xF;
    /* srwi. r5, r5, 4 */;
    /* lbzx r0, r3, r0 */;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r4 = r4 + 0x1;
    if ((s32)r0 != (s32)0x58) goto L_800DE394;
    r0 = 0x0;
    r3 = (u32)lbl_80401168;
    r20 = (u32)lbl_80401168;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r3 = r20;
    strlen();
    r4 = (u32)lbl_80401168;
    r0 = (u32)lbl_80401168;
    r4 = r3 + r0;
    goto L_800DE410;
L_800DE3D8: ;
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r20 = r20 + 0x1;
L_800DE410: ;
    if ((u32)r20 < (u32)r4) goto L_800DE3D8;
    goto L_800DE4C4;
L_800DE41C: ;
    r3 = (u32)lbl_80401168;
    r4 = *(u32*)lbl_80478AE8;
    r5 = (u32)lbl_80401168;
L_800DE428: ;
    r0 = r6 & 0xF;
    /* lbzx r0, r4, r0 */;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = *(u8*)((u8*)r5 + 0x0);
    r0 = (s8)r3;
    if ((s32)r0 < (s32)0x41) goto L_800DE44C;
    r0 = r3 + 0x20;
    *(u8*)((u8*)r5 + 0x0) = r0;
L_800DE44C: ;
    /* srwi. r6, r6, 4 */;
    r5 = r5 + 0x1;
    if ((s32)r0 != (s32)0x41) goto L_800DE428;
    r0 = 0x0;
    r3 = (u32)lbl_80401168;
    r20 = (u32)lbl_80401168;
    *(u8*)((u8*)r5 + 0x0) = r0;
    r3 = r20;
    strlen();
    r4 = (u32)lbl_80401168;
    r0 = (u32)lbl_80401168;
    r4 = r3 + r0;
    goto L_800DE4BC;
L_800DE484: ;
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r3 = *(u8*)((u8*)r4 + 0x0);
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r4 + 0x0) = r0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    r3 = *(u8*)((u8*)r20 + 0x0);
    r0 = r3 ^ r0;
    *(u8*)((u8*)r20 + 0x0) = r0;
    r20 = r20 + 0x1;
L_800DE4BC: ;
    if ((u32)r20 < (u32)r4) goto L_800DE484;
L_800DE4C4: ;
    r3 = (u32)lbl_80401168;
    r0 = 0x1;
    r3 = (u32)lbl_80401168;
    r20 = r3;
    goto L_800DE4E0;
L_800DE4D8: ;
    *(u8*)((u8*)r29 + 0x0) = r5;
    r29 = r29 + 0x1;
L_800DE4E0: ;
    r3 = r0 & 0xFF;
    if ((u32)r3 != (u32)0x1) goto L_800DE63C;
    r0 = 0x0;
    r3 = (u32)lbl_80401178;
    *(u8*)((u8*)r29 + 0x0) = r0;
    r3 = (u32)lbl_80401178;
    r29 = r3;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x2d) goto L_800DE514;
    r26 = 0x1;
    r29 = r3 + 0x1;
L_800DE514: ;
    r0 = *(u8*)((u8*)r29 + 0x0);
    if ((s32)r0 != (s32)0x30) goto L_800DE528;
    r25 = 0x1;
    r29 = r29 + 0x1;
L_800DE528: ;
    r3 = r29;
    r27 = 0x0;
    goto L_800DE560;
L_800DE534: ;
    r0 = (s8)r4;
    if ((s32)r0 < (s32)0x30) goto L_800DE56C;
    if ((s32)r0 > (s32)0x39) goto L_800DE56C;
    r0 = *(u8*)((u8*)r3 + 0x0);
    r27 = r27 * 0xa;
    r3 = r3 + 0x1;
    r0 = (s8)r0;
    r27 = r0 + r27;
L_800DE560: ;
    r4 = *(u8*)((u8*)r3 + 0x0);
    r0 = (s8)r4;
    if ((s32)r0 != (s32)0x39) goto L_800DE534;
L_800DE56C: ;
    r3 = r20;
    strlen();
    if ((s32)r27 <= (s32)r3) goto L_800DE5E0;
    r0 = r26 & 0xFF;
    r27 = r27 - r3;
    if ((s32)r27 != (s32)r3) goto L_800DE5E0;
    r5 = r25 & 0xFF;
    r4 = 0x30;
    r3 = 0x20;
    goto L_800DE5B4;
L_800DE598: ;
    if ((u32)r5 == (u32)0x0) goto L_800DE5AC;
    *(u8*)((u8*)r30 + 0x0) = r4;
    r30 = r30 + 0x1;
    goto L_800DE5B4;
L_800DE5AC: ;
    *(u8*)((u8*)r30 + 0x0) = r3;
    r30 = r30 + 0x1;
L_800DE5B4: ;
    if ((s32)r27 == (s32)0x0) goto L_800DE5E0;
    r0 = r30 - r22;
    if ((u32)r0 < (u32)r23) goto L_800DE598;
    goto L_800DE5E0;
L_800DE5D0: ;
    r0 = *(u8*)((u8*)r20 + 0x0);
    r20 = r20 + 0x1;
    *(u8*)((u8*)r30 + 0x0) = r0;
    r30 = r30 + 0x1;
L_800DE5E0: ;
    r0 = *(u8*)((u8*)r20 + 0x0);
    r0 = (s8)r0;
    if ((u32)r0 == (u32)r23) goto L_800DE5F8;
    r0 = r30 - r22;
    if ((u32)r0 < (u32)r23) goto L_800DE5D0;
L_800DE5F8: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_800DE62C;
    r3 = 0x20;
    goto L_800DE614;
L_800DE60C: ;
    *(u8*)((u8*)r30 + 0x0) = r3;
    r30 = r30 + 0x1;
L_800DE614: ;
    if ((s32)r27 == (s32)0x0) goto L_800DE62C;
    r0 = r30 - r22;
    if ((u32)r0 < (u32)r23) goto L_800DE60C;
L_800DE62C: ;
    r26 = 0x0;
    r25 = 0x0;
    r27 = 0x0;
    r0 = 0x0;
L_800DE63C: ;
    r3 = *(u8*)((u8*)r31 + 0x0);
    r3 = (s8)r3;
    if ((u32)r0 == (u32)r23) goto L_800DE654;
    r3 = r30 - r22;
    if ((u32)r3 < (u32)r23) goto L_800DE658;
L_800DE654: ;
    r28 = 0x1;
L_800DE658: ;
    r31 = r31 + 0x1;
L_800DE65C: ;
    r3 = r28 & 0xFF;
    if ((u32)r3 == (u32)r23) goto L_800DE16C;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x0) = r0;
    return;
}

/* fn_800DEFC8 | Size: 0x60 */
void fn_800DEFC8(u8* self) {
    extern void fn_800B8DF4();
    extern void fn_801BBD3C();
    u32 handle;
    u8* sub;
    u8* sub2;
    u32 old;
    handle = *(u32*)(self + 0x38);
    if ((handle + 0x01020000) != 0xFEFE) {
        fn_800B8DF4(handle);
        sub = (u8*)*(u32*)(self + 0x8);
        sub2 = (u8*)*(u32*)(sub + 0x8);
        old = *(u32*)(sub2 + 0x58);
        *(u32*)(sub2 + 0x58) = *(u32*)(self + 0x38);
        fn_801BBD3C(old);
        *(u32*)(self + 0x38) = 0xFEFEFEFE;
    }
}

/* fn_800DF028 | Size: 0xF4 */
void fn_800DF028(void) {
    extern u8 lbl_8047CAC8[];
    extern void fn_800B8DF4();
    extern void fn_800EF3E0();
    extern void fn_800EF4E4();
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_800EF504();
    extern void fn_800EF548();
    extern void fn_801BBD84();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r30 = r4;
    r29 = r3;
    r3 = *(u32*)((u8*)r3 + 0x38);
    r0 = r3 + (0x102 << 16);
    if ((u32)r0 != (u32)0xfefe) goto L_800DF074;
    r3 = *(u32*)((u8*)r29 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x8);
    r0 = *(u32*)((u8*)r3 + 0x58);
    *(u32*)((u8*)r29 + 0x38) = r0;
    fn_801BBD84();
    r31 = r3;
    goto L_800DF080;
L_800DF074: ;
    r3 = *(u32*)((u8*)r29 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x8);
    r31 = *(u32*)((u8*)r3 + 0x58);
L_800DF080: ;
    if ((u32)r31 == (u32)0x0) goto L_800DF100;
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
L_800DF100: ;
    return;
}

/* fn_800DF11C | Size: 0x24 */
void fn_800DF11C(void* self, void* out) {
    u8* src = (u8*)self;
    u8* dst = (u8*)out;
    dst[0] = src[0xC];
    dst[1] = src[0xD];
    dst[2] = src[0xE];
    dst[3] = src[0xF];
}

/* fn_800DF140 | Size: 0x48 */
void fn_800DF140(u8* self) {
    extern u8 lbl_8047CACC[];
    extern u8 lbl_8047CAD0[];
    extern void fn_801A6DDC();
    f32 scale = *(f32*)lbl_8047CACC;
    u8 alpha = *(u8*)(self + 0x1);
    u32 sub = *(u32*)(self + 0x8);
    f32 val = (f32)alpha / scale;
    fn_801A6DDC(sub, val);
}

/* fn_800DF188 | Size: 0x30 */
void fn_800DF188(void* self) {
    extern u8 lbl_8047CACC[];
    u8* ptr = (u8*)self;
    f32 scale = *(f32*)lbl_8047CACC;
    u8* sub = (u8*)*(u32*)(ptr + 0x8);
    u8* sub2 = (u8*)*(u32*)(sub + 0xC);
    f32 val = scale * *(f32*)(sub2 + 0xC);
    ptr[0x1] = (u8)(s32)val;
}

/* fn_800DF1B8 | Size: 0x18 */
void fn_800DF1B8(void* self, f32 value) {
    u8* ptr = (u8*)self;
    u32 sub = *(u32*)(ptr + 0x20);
    *(f32*)(ptr + 0x34) = value;
    if (sub == 0) return;
    *(f32*)((u8*)sub + 0x50) = value;
}

/* fn_800DF1D0 | Size: 0x14 */
void fn_800DF1D0(void* self, u32 p1, u32 p2, u32 p3, f32 fval) {
    u8* ptr = (u8*)self;
    *(u32*)(ptr + 0x2C) = p1;
    *(u32*)(ptr + 0x30) = p2;
    *(f32*)(ptr + 0x34) = fval;
    *(u32*)(ptr + 0x28) = p3;
}

/* fn_800DF1E4 | Size: 0x24 */
void fn_800DF1E4(void* self, void* color) {
    u8* ptr = (u8*)self;
    u8* c = (u8*)color;
    ptr[0xC] = c[0];
    ptr[0xD] = c[1];
    ptr[0xE] = c[2];
    ptr[0xF] = c[3];
}

/* fn_800DF208 | Size: 0x14 */
void fn_800DF208(void* self, u32 a, u32 b, u32 c, u32 d) {
    u8* ptr = (u8*)self;
    *(u32*)(ptr + 0x10) = a;
    *(u32*)(ptr + 0x14) = b;
    *(u32*)(ptr + 0x18) = c;
    *(u32*)(ptr + 0x1C) = d;
}

/* fn_800DF21C | Size: 0x24 */
void fn_800DF21C(void* self) {
    extern void fn_801A6DDC();
    fn_801A6DDC(*(u32*)((u8*)self + 0x8));
}

/* fn_800DF248 | Size: 0x13C */
void fn_800DF248(void) {
    extern void fn_801A6FF0();
    extern void fn_801BBD3C();
    extern void fn_801BBED4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r0 = *(u16*)((u8*)r3 + 0x2);
    r30 = *(u32*)((u8*)r3 + 0x8);
    r31 = r4 & r0;
    r0 = r31 & 0x1;
    if ((s32)r0 == (s32)0) goto L_800DF290;
    r0 = 0x7f;
    *(u8*)((u8*)r29 + 0xF) = r0;
    *(u8*)((u8*)r29 + 0xE) = r0;
    *(u8*)((u8*)r29 + 0xD) = r0;
    *(u8*)((u8*)r29 + 0xC) = r0;
L_800DF290: ;
    if ((s32)r0 == (s32)0) goto L_800DF2B8;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r29 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r29 + 0x14) = r4;
    *(u32*)((u8*)r29 + 0x18) = r3;
    *(u32*)((u8*)r29 + 0x1C) = r0;
L_800DF2B8: ;
    if ((s32)r0 == (s32)0) goto L_800DF33C;
    r5 = *(u32*)((u8*)r29 + 0x8);
    r28 = *(u32*)((u8*)r29 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r29 + 0x24);
    if ((u32)r4 == (u32)0x0) goto L_800DF334;
    if ((u32)r28 == (u32)0x0) goto L_800DF334;
    if ((u32)r4 != (u32)r28) goto L_800DF310;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto L_800DF318;
    goto L_800DF310;
L_800DF2F8: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 != (u32)r28) goto L_800DF30C;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
L_800DF30C: ;
    r4 = *(u32*)((u8*)r4 + 0x8);
L_800DF310: ;
    if ((u32)r4 != (u32)0x0) goto L_800DF2F8;
L_800DF318: ;
    if ((u32)r3 == (u32)0x0) goto L_800DF324;
    fn_801BBD3C();
L_800DF324: ;
    if ((u32)r28 == (u32)0x0) goto L_800DF334;
    r3 = r28;
    fn_801BBED4();
L_800DF334: ;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0x28) = r0;
L_800DF33C: ;
    r0 = *(u16*)((u8*)r29 + 0x2);
    r0 = r0 & ~r31;
    *(u16*)((u8*)r29 + 0x2) = r0;
    r0 = *(u16*)((u8*)r29 + 0x2);
    if ((u32)r0 != (u32)0x0) goto L_800DF35C;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x20) = r0;
L_800DF35C: ;
    r3 = r30;
    fn_801A6FF0();
    return;
}

/* fn_800DF384 | Size: 0x6C */
void fn_800DF384(void) {
    extern void fn_800DFABC();
    extern void fn_801A6FF0();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r0 = *(u16*)((u8*)r3 + 0x2);
    r30 = *(u32*)((u8*)r3 + 0x8);
    /* andc. r31, r4, r0 */;
    if ((s32)r0 == (s32)0) goto L_800DF3D4;
    *(u32*)((u8*)r30 + 0x20) = r29;
    if ((s32)r0 == (s32)0) goto L_800DF3C0;
    fn_800DFABC();
L_800DF3C0: ;
    r0 = *(u16*)((u8*)r29 + 0x2);
    r3 = r30;
    r0 = r0 | r31;
    *(u16*)((u8*)r29 + 0x2) = r0;
    fn_801A6FF0();
L_800DF3D4: ;
    return;
}

/* fn_800DF3F0 | Size: 0x80 */
void fn_800DF3F0(void) {
    extern void fn_801A8458();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = *(u32*)((u8*)r3 + 0x8);
    fn_801A8458();
    r0 = r3 & 0x1;
    r4 = 0x0;
    if ((s32)r0 == (s32)0) goto L_800DF414;
    r4 = r4 | 0x1;
L_800DF414: ;
    if ((s32)r0 == (s32)0) goto L_800DF420;
    r4 = r4 | 0x2;
L_800DF420: ;
    if ((s32)r0 == (s32)0) goto L_800DF42C;
    r4 = r4 | 0x4;
L_800DF42C: ;
    if ((s32)r0 == (s32)0) goto L_800DF438;
    r4 = r4 | 0x8;
L_800DF438: ;
    if ((s32)r0 == (s32)0) goto L_800DF444;
    r4 = r4 | 0x10;
L_800DF444: ;
    if ((s32)r0 == (s32)0) goto L_800DF450;
    r4 = r4 | 0x20;
L_800DF450: ;
    if ((s32)r0 == (s32)0) goto L_800DF45C;
    r4 = r4 | 0x40;
L_800DF45C: ;
    r3 = r4;
    return;
}

/* fn_800DF470 | Size: 0x28 */
void fn_800DF470(void* self) {
    u8* ptr = (u8*)self;
    u32 cur;
    u32 check;
    u8* sub;
    cur = *(u32*)(ptr + 0x3C);
    check = cur + (0x102 << 16);
    if (check == 0xfefe) return;
    sub = (u8*)*(u32*)(ptr + 0x8);
    *(u32*)(sub + 0x10) = cur;
    *(u32*)(ptr + 0x3C) = (0xfeff << 16) - 0x102;
}

/* fn_800DF498 | Size: 0x6C */
void fn_800DF498(void) {
    extern void fn_800DD970();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    r30 = r3;
    r3 = *(u32*)((u8*)r3 + 0x3C);
    r0 = r3 + (0x102 << 16);
    if ((u32)r0 != (u32)0xfefe) goto L_800DF4D4;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)((u8*)r3 + 0x10);
    *(u32*)((u8*)r30 + 0x3C) = r0;
    goto L_800DF4E4;
L_800DF4D4: ;
    r3 = (u32)lbl_80270528;
    fn_800DD970();
L_800DF4E4: ;
    r3 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r3 + 0x10) = r31;
    return;
}

/* fn_800DF504 | Size: 0x4C */
void fn_800DF504(u8* self) {
    extern void fn_801A6FF0();
    extern void fn_801A8428();
    extern void fn_801A8440();
    u32 gobj = *(u32*)(self + 0x8);
    fn_801A8428(gobj, 0x4000600F);
    fn_801A8440(*(u32*)(self + 0x8), *(u32*)(self + 0x4));
    fn_801A6FF0(*(u32*)(self + 0x8));
}

/* fn_800DF550 | Size: 0xB8 */
void fn_800DF550(void) {
    extern void fn_801A6FF0();
    extern void fn_801A8428();
    extern void fn_801A8440();
    extern void fn_801A8458();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

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
    if ((s32)r0 == (s32)0) goto L_800DF598;
    r4 = r4 | 0x1;
L_800DF598: ;
    if ((s32)r0 == (s32)0) goto L_800DF5A4;
    r4 = r4 | 0x2;
L_800DF5A4: ;
    if ((s32)r0 == (s32)0) goto L_800DF5B0;
    r4 = r4 | 0x4;
L_800DF5B0: ;
    if ((s32)r0 == (s32)0) goto L_800DF5BC;
    r4 = r4 | 0x8;
L_800DF5BC: ;
    if ((s32)r0 == (s32)0) goto L_800DF5C8;
    r4 = r4 | (0x4000 << 16);
L_800DF5C8: ;
    if ((s32)r0 == (s32)0) goto L_800DF5D4;
    r4 = r4 | 0x2000;
L_800DF5D4: ;
    if ((s32)r0 == (s32)0) goto L_800DF5E0;
    r4 = r4 | 0x4000;
L_800DF5E0: ;
    r3 = *(u32*)((u8*)r30 + 0x8);
    fn_801A8440();
    r3 = *(u32*)((u8*)r30 + 0x8);
    fn_801A6FF0();
    return;
}

/* fn_800DF608 | Size: 0x19C */
void fn_800DF608(void) {
    extern void fn_800B8DF4();
    extern void fn_801A6FF0();
    extern void fn_801BBD3C();
    extern void fn_801BBED4();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r5 = *(u32*)((u8*)r3 + 0x3C);
    r0 = r5 + (0x102 << 16);
    if ((u32)r0 == (u32)0xfefe) goto L_800DF64C;
    r4 = *(u32*)((u8*)r30 + 0x8);
    r3 = (0xfeff << 16);
    *(u32*)((u8*)r4 + 0x10) = r5;
    *(u32*)((u8*)r30 + 0x3C) = r0;
L_800DF64C: ;
    r3 = *(u32*)((u8*)r30 + 0x38);
    r0 = r3 + (0x102 << 16);
    if ((u32)r0 == (u32)0xfefe) goto L_800DF684;
    fn_800B8DF4();
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)((u8*)r30 + 0x38);
    r4 = *(u32*)((u8*)r3 + 0x8);
    r3 = *(u32*)((u8*)r4 + 0x58);
    *(u32*)((u8*)r4 + 0x58) = r0;
    fn_801BBD3C();
    r3 = (0xfeff << 16);
    *(u32*)((u8*)r30 + 0x38) = r0;
L_800DF684: ;
    r29 = *(u16*)((u8*)r30 + 0x2);
    r31 = *(u32*)((u8*)r30 + 0x8);
    r0 = r29 & 0x1;
    if ((u32)r0 == (u32)0xfefe) goto L_800DF6A8;
    r0 = 0x7f;
    *(u8*)((u8*)r30 + 0xF) = r0;
    *(u8*)((u8*)r30 + 0xE) = r0;
    *(u8*)((u8*)r30 + 0xD) = r0;
    *(u8*)((u8*)r30 + 0xC) = r0;
L_800DF6A8: ;
    if ((u32)r0 == (u32)0xfefe) goto L_800DF6D0;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r30 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r30 + 0x14) = r4;
    *(u32*)((u8*)r30 + 0x18) = r3;
    *(u32*)((u8*)r30 + 0x1C) = r0;
L_800DF6D0: ;
    if ((u32)r0 == (u32)0xfefe) goto L_800DF754;
    r5 = *(u32*)((u8*)r30 + 0x8);
    r28 = *(u32*)((u8*)r30 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r30 + 0x24);
    if ((u32)r4 == (u32)0x0) goto L_800DF74C;
    if ((u32)r28 == (u32)0x0) goto L_800DF74C;
    if ((u32)r4 != (u32)r28) goto L_800DF728;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto L_800DF730;
    goto L_800DF728;
L_800DF710: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 != (u32)r28) goto L_800DF724;
    r0 = *(u32*)((u8*)r28 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
L_800DF724: ;
    r4 = *(u32*)((u8*)r4 + 0x8);
L_800DF728: ;
    if ((u32)r4 != (u32)0x0) goto L_800DF710;
L_800DF730: ;
    if ((u32)r3 == (u32)0x0) goto L_800DF73C;
    fn_801BBD3C();
L_800DF73C: ;
    if ((u32)r28 == (u32)0x0) goto L_800DF74C;
    r3 = r28;
    fn_801BBED4();
L_800DF74C: ;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x28) = r0;
L_800DF754: ;
    r0 = *(u16*)((u8*)r30 + 0x2);
    r0 = r0 & ~r29;
    *(u16*)((u8*)r30 + 0x2) = r0;
    r0 = *(u16*)((u8*)r30 + 0x2);
    if ((u32)r0 != (u32)0x0) goto L_800DF774;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
L_800DF774: ;
    r3 = r31;
    fn_801A6FF0();
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x0) = r0;
    return;
}

/* fn_800DF7A4 | Size: 0xB0 */
void fn_800DF7A4(void) {
    extern u8 lbl_8047AB1C[];
    extern u8 lbl_8047AB20[];
    extern void fn_800DD970();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = *(u32*)lbl_8047AB20;
    r3 = *(u32*)lbl_8047AB1C;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_800DF7DC;
L_800DF7C4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_800DF7D4;
    goto L_800DF7E0;
L_800DF7D4: ;
    r3 = r3 + 0x40;
    if (--ctr != 0) goto L_800DF7C4;
L_800DF7DC: ;
    r3 = 0x0;
L_800DF7E0: ;
    if ((u32)r3 != (u32)0x0) goto L_800DF800;
    r3 = (u32)lbl_8027056C;
    fn_800DD970();
    r3 = 0x0;
    goto L_800DF844;
L_800DF800: ;
    r7 = 0x1;
    r4 = (0xfeff << 16);
    *(u8*)((u8*)r3 + 0x0) = r7;
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
L_800DF844: ;
    return;
}

/* fn_800DF854 -- GSmaterialInit | Size: 0x78 */
void fn_800DF854(void) {
    extern u8 lbl_80315490[];
    extern u8 lbl_8047AB18[];
    extern u8 lbl_8047AB1C[];
    extern u8 lbl_8047AB20[];
    extern void fn_800E27B0();
    extern void fn_800E3534();
    extern void fn_801A7CFC();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    *(u32*)lbl_8047AB20 = r3;
    r3 = r3 << 6;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    *(u16*)lbl_8047AB18 = r3;
    if ((s32)r0 == (s32)0) goto L_800DF8BC;
    r3 = r0;
    fn_800E27B0();
    r5 = 0x0;
    *(u32*)lbl_8047AB1C = r3;
    r4 = r5;
    r6 = 0x0;
    goto L_800DF8A4;
L_800DF894: ;
    r3 = *(u32*)lbl_8047AB1C;
    r6 = r6 + 0x1;
    /* stbx r4, r3, r5 */;
    r5 = r5 + 0x40;
L_800DF8A4: ;
    r0 = *(u32*)lbl_8047AB20;
    if ((u32)r6 < (u32)r0) goto L_800DF894;
    r3 = (u32)lbl_80315490;
    fn_801A7CFC();
L_800DF8BC: ;
    return;
}

/* fn_800DF8CC -- GSmaterialClassInit | Size: 0x64 */
void fn_800DF8CC(void) {
    extern u8 lbl_80315490[];
    extern u8 lbl_8036CB30[];
    extern void fn_80193B30();
    extern void fn_800DF930();
    extern void fn_800DFE98();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

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
    return;
}

/* fn_800DF930 | Size: 0x18C */
void fn_800DF930(void) {
    extern u8 lbl_8036CB30[];
    extern void fn_801B5F08();
    extern void fn_801B64EC();
    extern void fn_801B6CD8();
    extern void fn_801B6DC0();
    extern void fn_801B6E74();
    extern void fn_801B6F5C();
    extern void fn_801B707C();
    extern void fn_801B7C60();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r6 = (u32)lbl_8036CB30;
    r29 = r3;
    r28 = r5;
    r12 = *(u32*)((u8*)r6 + 0x44);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r29 = *(u32*)((u8*)r29 + 0x20);
    r30 = r3;
    if ((u32)r29 != (u32)0x0) goto L_800DF97C;
    goto L_800DFA9C;
L_800DF97C: ;
    r0 = *(u16*)((u8*)r29 + 0x2);
    if ((u32)r29 == (u32)0x0) goto L_800DF9C4;
    r31 = r30;
    goto L_800DF9BC;
L_800DF990: ;
    r3 = r31;
    fn_801B7C60();
    if ((s32)r3 != (s32)0x1) goto L_800DF9B8;
    r4 = *(u32*)((u8*)r29 + 0x10);
    r3 = r31;
    r5 = *(u32*)((u8*)r29 + 0x14);
    r6 = *(u32*)((u8*)r29 + 0x18);
    r7 = *(u32*)((u8*)r29 + 0x1C);
    fn_801B6DC0();
L_800DF9B8: ;
    r31 = *(u32*)((u8*)r31 + 0x4);
L_800DF9BC: ;
    if ((u32)r31 != (u32)0x0) goto L_800DF990;
L_800DF9C4: ;
    r0 = *(u16*)((u8*)r29 + 0x2);
    r0 = r0 & 0x1;
    if ((u32)r31 == (u32)0x0) goto L_800DFA98;
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
    *(u32*)(sp + 0x8) = r0;
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
    *(u32*)(sp + 0x8) = r0;
    r7 = r28;
    r9 = r30;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x5;
    r8 = 0x5;
    r10 = 0x0;
    fn_801B5F08();
    r30 = r29;
L_800DFA98: ;
    r3 = r30;
L_800DFA9C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}

/* fn_800DFABC | Size: 0x3DC */
void fn_800DFABC(void) {
    extern u8 lbl_803154E4[];
    extern u8 lbl_8047CAC8[];
    extern void fn_800DD970();
    extern void fn_800EF3E0();
    extern void fn_800EF4E4();
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_800EF504();
    extern void fn_800EF548();
    extern void fn_801A6D5C();
    extern void fn_801A6DA0();
    extern void fn_801A6DC4();
    extern void fn_801A6FF0();
    extern void fn_801BBD3C();
    extern void fn_801BBD60();
    extern void fn_801BBD84();
    extern void fn_801BBED4();
    extern void fn_801BE4CC();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r28 = r3;
    r31 = *(u32*)((u8*)r3 + 0x28);
    if ((u32)r31 == (u32)0x0) goto L_800DFD78;
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
    if ((s32)r0 == (s32)0x1) goto L_800DFB20;
    if ((s32)r0 >= (s32)0x1) goto L_800DFB38;
    if ((s32)r0 >= (s32)0x0) goto L_800DFB2C;
    goto L_800DFB38;
    goto L_800DFB38;
L_800DFB20: ;
    r0 = r5 | 0x5;
    *(u32*)((u8*)r4 + 0x40) = r0;
    goto L_800DFB44;
L_800DFB2C: ;
    r0 = r5 | 0x1;
    *(u32*)((u8*)r4 + 0x40) = r0;
    goto L_800DFB44;
L_800DFB38: ;
    r0 = *(u32*)((u8*)r4 + 0x40);
    r0 = r0 | 0x6;
    *(u32*)((u8*)r4 + 0x40) = r0;
L_800DFB44: ;
    r0 = *(u32*)((u8*)r28 + 0x30);
    if ((s32)r0 != (s32)0x0) goto L_800DFB60;
    r0 = *(u32*)((u8*)r4 + 0x40);
    r0 = r0 | (0x4 << 16);
    *(u32*)((u8*)r4 + 0x40) = r0;
    goto L_800DFB6C;
L_800DFB60: ;
    r0 = *(u32*)((u8*)r4 + 0x40);
    r0 = r0 | (0x3 << 16);
    *(u32*)((u8*)r4 + 0x40) = r0;
L_800DFB6C: ;
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
    if ((s32)r0 == (s32)0xe) goto L_800DFCE4;
    if ((s32)r0 >= (s32)0xe) goto L_800DFBC4;
    if ((s32)r0 >= (s32)0x7) goto L_800DFBC4;
    if ((s32)r0 >= (s32)0x0) goto L_800DFCE4;
L_800DFBC4: ;
    r3 = (u32)lbl_802705D0;
    fn_800DD970();
    r3 = r29;
    fn_801BBD3C();
    r3 = r29;
    fn_801BBD60();
    r0 = *(u16*)((u8*)r28 + 0x2);
    r31 = *(u32*)((u8*)r28 + 0x8);
    r0 = r29 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_800DFC0C;
    r0 = 0x7f;
    *(u8*)((u8*)r28 + 0xF) = r0;
    *(u8*)((u8*)r28 + 0xE) = r0;
    *(u8*)((u8*)r28 + 0xD) = r0;
    *(u8*)((u8*)r28 + 0xC) = r0;
L_800DFC0C: ;
    if ((s32)r0 == (s32)0x0) goto L_800DFC34;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r28 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r28 + 0x14) = r4;
    *(u32*)((u8*)r28 + 0x18) = r3;
    *(u32*)((u8*)r28 + 0x1C) = r0;
L_800DFC34: ;
    if ((s32)r0 == (s32)0x0) goto L_800DFCB8;
    r5 = *(u32*)((u8*)r28 + 0x8);
    r30 = *(u32*)((u8*)r28 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r28 + 0x24);
    if ((u32)r4 == (u32)0x0) goto L_800DFCB0;
    if ((u32)r30 == (u32)0x0) goto L_800DFCB0;
    if ((u32)r4 != (u32)r30) goto L_800DFC8C;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto L_800DFC94;
    goto L_800DFC8C;
L_800DFC74: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 != (u32)r30) goto L_800DFC88;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
L_800DFC88: ;
    r4 = *(u32*)((u8*)r4 + 0x8);
L_800DFC8C: ;
    if ((u32)r4 != (u32)0x0) goto L_800DFC74;
L_800DFC94: ;
    if ((u32)r3 == (u32)0x0) goto L_800DFCA0;
    fn_801BBD3C();
L_800DFCA0: ;
    if ((u32)r30 == (u32)0x0) goto L_800DFCB0;
    r3 = r30;
    fn_801BBED4();
L_800DFCB0: ;
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x28) = r0;
L_800DFCB8: ;
    r0 = *(u16*)((u8*)r28 + 0x2);
    r0 = r0 & ~r29;
    *(u16*)((u8*)r28 + 0x2) = r0;
    r0 = *(u16*)((u8*)r28 + 0x2);
    if ((u32)r0 != (u32)0x0) goto L_800DFCD8;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
L_800DFCD8: ;
    r3 = r31;
    fn_801A6FF0();
    goto L_800DFE84;
L_800DFCE4: ;
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
    if ((u32)r0 == (u32)0x0) goto L_800DFD58;
    goto L_800DFD3C;
L_800DFD38: ;
    r4 = r0;
L_800DFD3C: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 != (u32)0x0) goto L_800DFD38;
    r3 = r30;
    r5 = r27;
    fn_801A6D5C();
    goto L_800DFD64;
L_800DFD58: ;
    r3 = r30;
    r4 = r27;
    fn_801A6DA0();
L_800DFD64: ;
    *(u32*)((u8*)r28 + 0x20) = r27;
    r3 = r31;
    *(u32*)((u8*)r28 + 0x24) = r29;
    fn_800EF504();
    goto L_800DFE84;
L_800DFD78: ;
    r3 = (u32)lbl_80270610;
    fn_800DD970();
    r0 = *(u16*)((u8*)r28 + 0x2);
    r31 = *(u32*)((u8*)r28 + 0x8);
    r0 = r29 & 0x1;
    if ((u32)r0 == (u32)0x0) goto L_800DFDB0;
    r0 = 0x7f;
    *(u8*)((u8*)r28 + 0xF) = r0;
    *(u8*)((u8*)r28 + 0xE) = r0;
    *(u8*)((u8*)r28 + 0xD) = r0;
    *(u8*)((u8*)r28 + 0xC) = r0;
L_800DFDB0: ;
    if ((u32)r0 == (u32)0x0) goto L_800DFDD8;
    r0 = 0x0;
    r4 = 0x1;
    *(u32*)((u8*)r28 + 0x10) = r0;
    r3 = 0x2;
    r0 = 0x3;
    *(u32*)((u8*)r28 + 0x14) = r4;
    *(u32*)((u8*)r28 + 0x18) = r3;
    *(u32*)((u8*)r28 + 0x1C) = r0;
L_800DFDD8: ;
    if ((u32)r0 == (u32)0x0) goto L_800DFE5C;
    r5 = *(u32*)((u8*)r28 + 0x8);
    r30 = *(u32*)((u8*)r28 + 0x20);
    r4 = *(u32*)((u8*)r5 + 0x8);
    r3 = *(u32*)((u8*)r28 + 0x24);
    if ((u32)r4 == (u32)0x0) goto L_800DFE54;
    if ((u32)r30 == (u32)0x0) goto L_800DFE54;
    if ((u32)r4 != (u32)r30) goto L_800DFE30;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r5 + 0x8) = r0;
    goto L_800DFE38;
    goto L_800DFE30;
L_800DFE18: ;
    r0 = *(u32*)((u8*)r4 + 0x8);
    if ((u32)r0 != (u32)r30) goto L_800DFE2C;
    r0 = *(u32*)((u8*)r30 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = r0;
L_800DFE2C: ;
    r4 = *(u32*)((u8*)r4 + 0x8);
L_800DFE30: ;
    if ((u32)r4 != (u32)0x0) goto L_800DFE18;
L_800DFE38: ;
    if ((u32)r3 == (u32)0x0) goto L_800DFE44;
    fn_801BBD3C();
L_800DFE44: ;
    if ((u32)r30 == (u32)0x0) goto L_800DFE54;
    r3 = r30;
    fn_801BBED4();
L_800DFE54: ;
    r0 = 0x0;
    *(u32*)((u8*)r28 + 0x28) = r0;
L_800DFE5C: ;
    r0 = *(u16*)((u8*)r28 + 0x2);
    r0 = r0 & ~r29;
    *(u16*)((u8*)r28 + 0x2) = r0;
    r0 = *(u16*)((u8*)r28 + 0x2);
    if ((u32)r0 != (u32)0x0) goto L_800DFE7C;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x20) = r0;
L_800DFE7C: ;
    r3 = r31;
    fn_801A6FF0();
L_800DFE84: ;
    return;
}

/* fn_800DFE98 | Size: 0x54 */
u32 fn_800DFE98(u8* self) {
    extern u8 lbl_8036CB30[];
    typedef u32 (*VtblFunc)(u8*);
    VtblFunc func = (VtblFunc)*(u32*)(lbl_8036CB30 + 0x40);
    u32 result = func(self);
    if (result == 0) {
        *(u32*)(self + 0x20) = 0;
    }
    return result;
}

/* fn_800DFEEC | Size: 0xAC */
void fn_800DFEEC(void) {
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;

    f6 = *(f32*)((u8*)r5 + 0x4);
    f12 = *(f32*)((u8*)r4 + 0x4);
    f10 = *(f32*)((u8*)r5 + 0x8);
    f11 = *(f32*)((u8*)r4 + 0x0);
    f0 = f6 * f12;
    f5 = *(f32*)((u8*)r5 + 0x0);
    f2 = f10 * f12;
    f8 = *(f32*)((u8*)r4 + 0x8);
    f1 = f6 * f11;
    f9 = *(f32*)((u8*)r4 + 0xC);
    f0 = f5 * f11 + f0;
    f3 = f5 * f9 + f2;
    f2 = f5 * f8;
    f7 = f10 * f8 + f0;
    f1 = f10 * f9 + f1;
    f4 = -(f6 * f8 - f3);
    f0 = f11 * f7;
    f2 = f6 * f9 + f2;
    f6 = -(f5 * f12 - f1);
    f0 = f9 * f4 + f0;
    f5 = -(f10 * f11 - f2);
    f0 = f12 * f6 + f0;
    f0 = -(f8 * f5 - f0);
    *(f32*)((u8*)r3 + 0x0) = f0;
    f0 = *(f32*)((u8*)r4 + 0x4);
    f1 = *(f32*)((u8*)r4 + 0xC);
    f0 = f0 * f7;
    f2 = *(f32*)((u8*)r4 + 0x8);
    f3 = *(f32*)((u8*)r4 + 0x0);
    f0 = f1 * f5 + f0;
    f0 = f2 * f4 + f0;
    f0 = -(f3 * f6 - f0);
    *(f32*)((u8*)r3 + 0x4) = f0;
    f0 = *(f32*)((u8*)r4 + 0x8);
    f1 = *(f32*)((u8*)r4 + 0xC);
    f0 = f0 * f7;
    f2 = *(f32*)((u8*)r4 + 0x0);
    f3 = *(f32*)((u8*)r4 + 0x4);
    f0 = f1 * f6 + f0;
    f0 = f2 * f5 + f0;
    f0 = -(f3 * f4 - f0);
    *(f32*)((u8*)r3 + 0x8) = f0;
    return;
}

/* fn_800DFF98 | Size: 0x34 */
void fn_800DFF98(void* a, void* b, void* c) {
    extern void fn_800A37CC();
    fn_800A37CC(b, c, a);
}

/* fn_800DFFCC | Size: 0x34 */
void fn_800DFFCC(void* a, void* b, void* c) {
    extern void fn_800A3B9C();
    fn_800A3B9C(b, c, a);
}

/* fn_800E0000 | Size: 0x20 */
void fn_800E0000(void) {
    extern void fn_800A3B7C();
    fn_800A3B7C();
}

/* fn_800E0020 | Size: 0x20 */
void fn_800E0020(void) {
    extern void fn_800A3BD8();
    fn_800A3BD8();
}

/* fn_800E0040 | Size: 0x20 */
void fn_800E0040(void) {
    extern void fn_800A3C00();
    fn_800A3C00();
}

/* fn_800E0060 | Size: 0x2C */
void fn_800E0060(void* a, void* b) {
    extern void fn_800A3ADC();
    fn_800A3ADC(b, a);
}

/* fn_800E008C | Size: 0x20 */
void fn_800E008C(void) {
    extern void fn_800A3B38();
    fn_800A3B38();
}

/* fn_800E00AC | Size: 0x34 */
void fn_800E00AC(void* mtx, void* vec, f32 scale) {
    extern u8 lbl_8047CAD8[];
    extern void fn_800A3AC0();
    f32 inv_scale = *(f32*)lbl_8047CAD8 / scale;
    fn_800A3AC0(vec, mtx, inv_scale);
}

/* fn_800E00E0 | Size: 0x28 */
void fn_800E00E0(void* dst, void* src) {
    f32* d = (f32*)dst;
    f32* s = (f32*)src;
    d[0] = -s[0];
    d[1] = -s[1];
    d[2] = -s[2];
}

/* fn_800E0108 | Size: 0x34 */
void fn_800E0108(void* dst, void* a, void* b) {
    f32* d = (f32*)dst;
    f32* va = (f32*)a;
    f32* vb = (f32*)b;
    d[0] = va[0] * vb[0];
    d[1] = va[1] * vb[1];
    d[2] = va[2] * vb[2];
}

/* fn_800E013C | Size: 0x2C */
void fn_800E013C(void* a, void* b) {
    extern void fn_800A3AC0();
    fn_800A3AC0(b, a);
}

/* fn_800E0168 | Size: 0x34 */
void fn_800E0168(void* a, void* b, void* c) {
    extern void fn_800A3A9C();
    fn_800A3A9C(b, c, a);
}

/* fn_800E019C | Size: 0x34 */
void fn_800E019C(void* a, void* b, void* c) {
    extern void fn_800A3A78();
    fn_800A3A78(b, c, a);
}

/* fn_800E01D0 | Size: 0x24 */
void fn_800E01D0(void* dst, void* src) {
    memcpy(dst, src, 0xc);
}

/* fn_800E01F4 | Size: 0x10 */
void fn_800E01F4(void* dst, f32 x, f32 y, f32 z) {
    f32* d = (f32*)dst;
    d[0] = x;
    d[1] = y;
    d[2] = z;
}

/* fn_800E0204 | Size: 0x14 */
void fn_800E0204(void* dst) {
    extern u8 lbl_8047CADC[];
    f32 v = *(f32*)lbl_8047CADC;
    f32* d = (f32*)dst;
    d[0] = v;
    d[1] = v;
    d[2] = v;
}

/* fn_800E0218 | Size: 0x20 */
void fn_800E0218(void) {
    extern void fn_800A3458();
    fn_800A3458();
}

/* fn_800E0238 | Size: 0x2C */
void fn_800E0238(void* a, void* b) {
    extern void fn_800A2E64();
    fn_800A2E64(b, a);
}

/* fn_800E0264 | Size: 0x2C */
void fn_800E0264(void* a, void* b) {
    extern void fn_800A2EB4();
    fn_800A2EB4(b, a);
}

/* fn_800E0290 | Size: 0x34 */
void fn_800E0290(void* a, void* b, void* c) {
    extern void fn_800A2D98();
    fn_800A2D98(b, c, a);
}

/* fn_800E02C4 | Size: 0x24 */
void fn_800E02C4(void* a) {
    extern void fn_800A335C();
    fn_800A335C(0, a);
}

/* fn_800E02E8 | Size: 0x44 */
void fn_800E02E8(void* mtx) {
    extern void fn_800A2D98();
    extern void fn_800A3074();
    u8 temp[0x30];
    fn_800A3074(temp, 0x5A);
    fn_800A2D98(mtx, temp, mtx);
}

/* fn_800E032C | Size: 0x44 */
void fn_800E032C(void* mtx) {
    extern void fn_800A2D98();
    extern void fn_800A3074();
    u8 temp[0x30];
    fn_800A3074(temp, 0x59);
    fn_800A2D98(mtx, temp, mtx);
}

/* fn_800E0370 | Size: 0x44 */
void fn_800E0370(void* mtx) {
    extern void fn_800A2D98();
    extern void fn_800A3074();
    u8 temp[0x30];
    fn_800A3074(temp, 0x58);
    fn_800A2D98(mtx, temp, mtx);
}

/* fn_800E03B4 | Size: 0x34 */
void fn_800E03B4(void* mtx, f32* vec) {
    extern void fn_800A32E8();
    fn_800A32E8(0, mtx, vec[0], vec[1], vec[2]);
}

/* fn_800E03E8 | Size: 0x24 */
void fn_800E03E8(void* a) {
    extern void fn_800A32E8();
    fn_800A32E8(0, a);
}

/* fn_800E040C | Size: 0x20 */
void fn_800E040C(void) {
    extern void fn_800A33B4();
    fn_800A33B4();
}

/* fn_800E042C | Size: 0x60 */
void fn_800E042C(void* dst, f32* scale) {
    extern u8 lbl_80315568[];
    u8* d = (u8*)dst;
    memcpy(dst, lbl_80315568, 0x30);
    *(f32*)(d + 0x00) = scale[0];
    *(f32*)(d + 0x14) = scale[1];
    *(f32*)(d + 0x28) = scale[2];
}

/* fn_800E048C | Size: 0x68 */
void fn_800E048C(void) {
    extern u8 lbl_80315568[];
    u8 sp[0x30];
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r4 = (u32)lbl_80315568;
    r5 = 0x30;
    r4 = (u32)lbl_80315568;
    *(f64*)(sp + 0x28) = f31;
    f31 = f3;
    *(f64*)(sp + 0x20) = f30;
    f30 = f2;
    *(f64*)(sp + 0x18) = f29;
    f29 = f1;
    r31 = r3;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    *(f32*)((u8*)r31 + 0x0) = f29;
    *(f32*)((u8*)r31 + 0x14) = f30;
    *(f32*)((u8*)r31 + 0x28) = f31;
    f31 = *(f64*)(sp + 0x28);
    f30 = *(f64*)(sp + 0x20);
    f29 = *(f64*)(sp + 0x18);
    r31 = *(u32*)(sp + 0x14);
    return;
}

/* fn_800E04F4 | Size: 0x24 */
void fn_800E04F4(void* dst) {
    extern void fn_800A3074();
    fn_800A3074(dst, 0x5a);
}

/* fn_800E0518 | Size: 0x24 */
void fn_800E0518(void* dst) {
    extern void fn_800A3074();
    fn_800A3074(dst, 0x59);
}

/* fn_800E053C | Size: 0x24 */
void fn_800E053C(void* dst) {
    extern void fn_800A3074();
    fn_800A3074(dst, 0x58);
}

/* fn_800E0560 | Size: 0x60 */
void fn_800E0560(void* dst, f32* trans) {
    extern u8 lbl_80315568[];
    u8* d = (u8*)dst;
    memcpy(dst, lbl_80315568, 0x30);
    *(f32*)(d + 0x0C) = trans[0];
    *(f32*)(d + 0x1C) = trans[1];
    *(f32*)(d + 0x2C) = trans[2];
}

/* fn_800E05C0 | Size: 0x68 */
void fn_800E05C0(void) {
    extern u8 lbl_80315568[];
    u8 sp[0x30];
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r4 = (u32)lbl_80315568;
    r5 = 0x30;
    r4 = (u32)lbl_80315568;
    *(f64*)(sp + 0x28) = f31;
    f31 = f3;
    *(f64*)(sp + 0x20) = f30;
    f30 = f2;
    *(f64*)(sp + 0x18) = f29;
    f29 = f1;
    r31 = r3;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    *(f32*)((u8*)r31 + 0xC) = f29;
    *(f32*)((u8*)r31 + 0x1C) = f30;
    *(f32*)((u8*)r31 + 0x2C) = f31;
    f31 = *(f64*)(sp + 0x28);
    f30 = *(f64*)(sp + 0x20);
    f29 = *(f64*)(sp + 0x18);
    r31 = *(u32*)(sp + 0x14);
    return;
}

/* fn_800E0628 | Size: 0x24 */
void fn_800E0628(void* dst, void* src) {
    memcpy(dst, src, 0x30);
}

/* fn_800E064C | Size: 0x2C */
void fn_800E064C(void* dst) {
    extern u8 lbl_80315568[];
    memcpy(dst, lbl_80315568, 0x30);
}

/* fn_800E0678 | Size: 0x20 */
void fn_800E0678(void) {
    extern void fn_800A3910();
    fn_800A3910();
}

/* fn_800E0698 | Size: 0x20 */
void fn_800E0698(void) {
    extern void fn_800A39E0();
    fn_800A39E0();
}

/* fn_800E06B8 | Size: 0x34 */
void fn_800E06B8(void* a, void* b, void* c) {
    extern void fn_800A3D3C();
    fn_800A3D3C(b, c, a);
}

/* fn_800E06EC | Size: 0x2C */
void fn_800E06EC(void* a, void* b) {
    extern void fn_801ADAAC();
    fn_801ADAAC(b, a);
}

/* fn_800E0718 | Size: 0x20 */
void fn_800E0718(void) {
    extern void fn_800A3CB0();
    fn_800A3CB0();
}

/* fn_800E0738 | Size: 0x34 */
void fn_800E0738(void* a, void* b, void* c) {
    extern void fn_800A3C54();
    fn_800A3C54(b, c, a);
}

/* fn_800E076C | Size: 0x24 */
void fn_800E076C(void* dst, void* src) {
    f32* d = (f32*)dst;
    f32* s = (f32*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

/* fn_800E0790 | Size: 0x54 -- uses GQR SPRs, keep as asm */
void fn_800E0790(void) {
    extern void fn_800E0C78();
    extern void fn_800E0D24();
    asm {
        li r3, 0x4
        oris r3, r3, 0x4
        mtspr 918, r3
        li r3, 0x5
        oris r3, r3, 0x5
        mtspr 919, r3
        li r3, 0x6
        oris r3, r3, 0x6
        mtspr 920, r3
        li r3, 0x7
        oris r3, r3, 0x7
        mtspr 921, r3
    }
    fn_800E0C78();
    fn_800E0D24();
}

/* fn_800E07E4 | Size: 0x128 */
void fn_800E07E4(void) {
    extern u8 lbl_8047CAE0[];
    extern u8 lbl_8047CAE4[];
    extern u8 lbl_8047CAE8[];
    extern void fn_800E013C();
    extern void fn_800E019C();
    u8 sp[0x60];
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x50) = f31;
    /* psq_st f31, 0x58(r1), 0, qr0 */;
    *(f64*)(sp + 0x40) = f30;
    /* psq_st f30, 0x48(r1), 0, qr0 */;
    *(f64*)(sp + 0x30) = f29;
    /* psq_st f29, 0x38(r1), 0, qr0 */;
    *(f64*)(sp + 0x20) = f28;
    /* psq_st f28, 0x28(r1), 0, qr0 */;
    f28 = f1;
    f0 = *(f32*)lbl_8047CAE4;
    r30 = r3;
    r31 = r4;
    if (f28 >= f0) goto L_800E0834;
    f28 = f0;
L_800E0834: ;
    f0 = *(f32*)lbl_8047CAE0;
    if (f28 <= f0) goto L_800E0844;
    f28 = f0;
L_800E0844: ;
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
    f31 = *(f64*)(sp + 0x50);
    /* psq_l f30, 0x48(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x40);
    /* psq_l f29, 0x38(r1), 0, qr0 */;
    f29 = *(f64*)(sp + 0x30);
    /* psq_l f28, 0x28(r1), 0, qr0 */;
    f28 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}

/* fn_800E090C | Size: 0xA8 */
void fn_800E090C(void) {
    extern u8 lbl_8047CAF0[];
    extern u8 lbl_8047CAF4[];
    extern void fn_800E013C();
    extern void fn_800E0168();
    extern void fn_800E019C();
    extern void fn_800E01D0();
    u8 sp[0x20];
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x10) = f31;
    /* psq_st f31, 0x18(r1), 0, qr0 */;
    f31 = f1;
    f0 = *(f32*)lbl_8047CAF0;
    r30 = r3;
    r31 = r4;
    /* cror eq, lt, eq */;
    if (f31 != f0) goto L_800E094C;
    fn_800E01D0();
    goto L_800E0994;
L_800E094C: ;
    f0 = *(f32*)lbl_8047CAF4;
    /* cror eq, gt, eq */;
    if (f31 != f0) goto L_800E0968;
    r4 = r5;
    fn_800E01D0();
    goto L_800E0994;
L_800E0968: ;
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
L_800E0994: ;
    /* psq_l f31, 0x18(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x10);
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}

/* fn_800E09B4 | Size: 0x34 */
f32 fn_800E09B4(f32 a, f32 b, f32 t) {
    extern u8 lbl_8047CAF0[];
    extern u8 lbl_8047CAF4[];
    if (t <= *(f32*)lbl_8047CAF0) {
        return a;
    }
    if (t >= *(f32*)lbl_8047CAF4) {
        return b;
    }
    return a + t * (b - a);
}

/* fn_800E09E8 | Size: 0x1B8 */
void fn_800E09E8(void) {
    extern u8 lbl_8047CAF8[];
    extern u8 lbl_8047CAFC[];
    extern u8 lbl_8047CB00[];
    extern u8 lbl_8047CB08[];
    extern void fn_800E013C();
    extern void fn_800E019C();
    extern void fn_800E01D0();
    u8 sp[0xB0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f25 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0xA0) = f31;
    /* psq_st f31, 0xa8((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x90) = f30;
    /* psq_st f30, 0x98((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x80) = f29;
    /* psq_st f29, 0x88((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x70) = f28;
    /* psq_st f28, 0x78((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x60) = f27;
    /* psq_st f27, 0x68((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x50) = f26;
    /* psq_st f26, 0x58((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x40) = f25;
    /* psq_st f25, 0x48((u32)sp), 0, qr0 */;
    r28 = r5;
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x18) = r0;
    f1 = *(f64*)lbl_8047CB00;
    r29 = r3;
    r27 = r4;
    f2 = *(f32*)lbl_8047CAF8;
    f0 = *(f64*)(sp + 0x18);
    f0 = f0 - f1;
    f25 = f2 / f0;
    fn_800E01D0();
    f29 = *(f64*)lbl_8047CB08;
    r29 = r29 + 0xc;
    f30 = *(f32*)lbl_8047CAFC;
    f31 = *(f32*)lbl_8047CAF8;
    r28 = 0x1;
    r30 = (0x4330 << 16);
    goto L_800E0B40;
L_800E0A84: ;
    /* xoris r0, r28, 0x8000 */;
    *(u32*)(sp + 0x1C) = r0;
    f0 = *(f64*)(sp + 0x18);
    f0 = f0 - f29;
    f27 = f0 * f25;
    if (f27 >= f30) goto L_800E0AA8;
    f27 = f30;
L_800E0AA8: ;
    if (f27 <= f31) goto L_800E0AB4;
    f27 = f31;
L_800E0AB4: ;
    f28 = f27 * f27;
    r3 = r29;
    r4 = r27 + 0x24;
    f0 = f28 * f27;
    f26 = f0 - f28;
    f1 = f26;
    fn_800E013C();
    f28 = f26 - f28;
    r3 = (u32)sp + 0x8;
    r4 = r27 + 0x18;
    f1 = f28 + f27;
    fn_800E013C();
    r3 = r29;
    r4 = r29;
    r5 = (u32)sp + 0x8;
    fn_800E019C();
    f28 = f28 + f26;
    r4 = r27;
    r3 = (u32)sp + 0x8;
    f1 = f31 + f28;
    fn_800E013C();
    r3 = r29;
    r4 = r29;
    r5 = (u32)sp + 0x8;
    fn_800E019C();
    f1 = -f28;
    r3 = (u32)sp + 0x8;
    r4 = r27 + 0xc;
    fn_800E013C();
    r3 = r29;
    r4 = r29;
    r5 = (u32)sp + 0x8;
    fn_800E019C();
    r28 = r28 + 0x1;
    r29 = r29 + 0xc;
L_800E0B40: ;
    if ((u32)r28 < (u32)r31) goto L_800E0A84;
    r3 = r29;
    r4 = r27 + 0xc;
    fn_800E01D0();
    /* psq_l f31, 0xa8((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0xA0);
    /* psq_l f30, 0x98((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x90);
    /* psq_l f29, 0x88((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0x80);
    /* psq_l f28, 0x78((u32)sp), 0, qr0 */;
    f28 = *(f64*)(sp + 0x70);
    /* psq_l f27, 0x68((u32)sp), 0, qr0 */;
    f27 = *(f64*)(sp + 0x60);
    /* psq_l f26, 0x58((u32)sp), 0, qr0 */;
    f26 = *(f64*)(sp + 0x50);
    /* psq_l f25, 0x48((u32)sp), 0, qr0 */;
    f25 = *(f64*)(sp + 0x40);
    return;
}

/* fn_800E0BA0 | Size: 0x44 */
f32 fn_800E0BA0(void) {
    extern u8 lbl_8047CB10[];
    extern f32 fn_801ADC7C();
    f32 a = fn_801ADC7C();
    f32 b = fn_801ADC7C();
    return a + b - *(f32*)lbl_8047CB10;
}

/* fn_800E0BE4 | Size: 0x20 */
void fn_800E0BE4(void) {
    extern void fn_801ADC7C();
    fn_801ADC7C();
}

/* fn_800E0C04 | Size: 0x50 */
u32 fn_800E0C04(u32 modulus) {
    extern u32 fn_801ADCD8();
    u32 lo = fn_801ADCD8();
    u32 hi = fn_801ADCD8();
    u32 combined = (hi << 16) | lo;
    return combined % modulus;
}

/* fn_800E0C54 | Size: 0x24 */
u16 fn_800E0C54(void) {
    extern u32 fn_801ADCD8();
    return (u16)(fn_801ADCD8() & 0xFFFF);
}

/* fn_800E0C78 | Size: 0x28 */
void fn_800E0C78(void) {
    extern u8 lbl_80478C94[];
    u32 time_lo = (u32)OSGetTime();
    u32* ptr = *(u32**)lbl_80478C94;
    *ptr = time_lo;
}

/* fn_800E0CA0 | Size: 0x84 */
void fn_800E0CA0(void) {
    extern u8 lbl_804011B8[];
    extern u8 lbl_8047CB18[];
    extern u8 lbl_8047CB1C[];
    extern u8 lbl_8047CB20[];
    extern u8 lbl_8047CB24[];
    extern u8 lbl_8047CB28[];
    extern u8 lbl_8047CB30[];
    extern u8 lbl_8047CB34[];
    extern void fn_800C46B0();
    extern void fn_800CE318();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x10) = f31;
    /* psq_st f31, 0x18((u32)sp), 0, qr0 */;
    f0 = *(f32*)lbl_8047CB20;
    f31 = *(f32*)lbl_8047CB1C;
    if (f1 <= f0) goto L_800E0CD4;
    f31 = *(f32*)lbl_8047CB24;
    f2 = *(f64*)lbl_8047CB28;
    fn_800CE318();
    f1 = (f32)f1;
L_800E0CD4: ;
    f0 = *(f32*)lbl_8047CB18;
    if (f1 <= f0) goto L_800E0CE8;
    f0 = *(f32*)lbl_8047CB20;
    f1 = f0 - f1;
L_800E0CE8: ;
    f2 = *(f32*)lbl_8047CB34;
    f0 = *(f32*)lbl_8047CB30;
    f1 = f2 * f1 + f0;
    fn_800C46B0();
    r4 = (u32)lbl_804011B8;
    r0 = r3 << 2;
    r3 = (u32)lbl_804011B8;
    /* lfsx f0, r3, r0 */;
    f1 = f31 * f0;
    /* psq_l f31, 0x18((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x10);
    return;
}

/* fn_800E0D24 | Size: 0xB8 */
void fn_800E0D24(void) {
    extern u8 lbl_804011B8[];
    extern u8 lbl_8047CB30[];
    extern u8 lbl_8047CB38[];
    extern u8 lbl_8047CB40[];
    extern void fn_800CDBE0();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x40) = f31;
    /* psq_st f31, 0x48((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x30) = f30;
    /* psq_st f30, 0x38((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x20) = f29;
    /* psq_st f29, 0x28((u32)sp), 0, qr0 */;
    r3 = (u32)lbl_804011B8;
    f29 = *(f32*)lbl_8047CB38;
    f30 = *(f32*)lbl_8047CB30;
    r30 = (u32)lbl_804011B8;
    f31 = *(f64*)lbl_8047CB40;
    r29 = 0x0;
    r31 = (0x4330 << 16);
L_800E0D70: ;
    /* xoris r0, r29, 0x8000 */;
    *(u32*)(sp + 0xC) = r0;
    f0 = *(f64*)(sp + 0x8);
    f0 = f0 - f31;
    f0 = f30 * f0;
    f1 = f29 * f0;
    fn_800CDBE0();
    f0 = (f32)f1;
    r29 = r29 + 0x1;
    *(f32*)((u8*)r30 + 0x0) = f0;
    r30 = r30 + 0x4;
    if ((s32)r29 < (s32)0xb5) goto L_800E0D70;
    /* psq_l f31, 0x48((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x40);
    /* psq_l f30, 0x38((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x30);
    /* psq_l f29, 0x28((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}

/* fn_800E0DDC | Size: 0x38 */
u32 fn_800E0DDC(void) {
    extern u8 lbl_8047AB40[];
    extern u8 lbl_8047AB44[];
    extern u8 __OSStartTime[];
    u8* timer;
    u32 head;
    u32 total;
    u32 node;
    timer = __OSStartTime;
    head = *(u32*)(timer + 0x10);
    *(u32*)lbl_8047AB44 = head;
    *(u32*)lbl_8047AB40 = *(u32*)(timer + 0x14);
    total = 0;
    node = *(u32*)((u8*)head + 0x4);
    while (node != 0) {
        total += *(u32*)((u8*)node + 0x8);
        node = *(u32*)((u8*)node + 0x4);
    }
    return total;
}

/* fn_800E0E14 | Size: 0x730 */
void fn_800E0E14(void) {
    extern u8 lbl_80270658[];
    extern u8 lbl_8047AB28[];
    extern u8 lbl_8047AB30[];
    extern u8 lbl_8047AB34[];
    extern u8 lbl_8047AB38[];
    extern u8 lbl_8047AB3C[];
    extern u8 lbl_8047AB48[];
    extern u8 lbl_8047AB4C[];
    extern u8 lbl_8047AB50[];
    extern u8 lbl_8047AB54[];
    extern u8 lbl_8047AB58[];
    extern u8 lbl_8047AB5C[];
    extern u8 lbl_8047AB60[];
    extern u8 lbl_8047AB64[];
    extern u8 lbl_8047AB68[];
    extern u8 lbl_8047CB48[];
    extern u8 lbl_8047CB50[];
    extern void fn_800DD38C();
    extern void fn_800DD970();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r25 = r3;
    r3 = (u32)lbl_80270658;
    r26 = r4;
    r0 = r25 & 0xFF;
    r29 = 0x1;
    r31 = (u32)lbl_80270658;
    r28 = 0x0;
    r27 = 0x0;
    if ((s32)r0 == (s32)0) goto L_800E0E54;
    r3 = r31 + 0x0;
    fn_800DD970();
L_800E0E54: ;
    r3 = *(u32*)lbl_8047AB30;
    if ((u32)r3 == (u32)0x0) goto L_800E0E7C;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E0E7C;
    r3 = r31 + 0x34;
    fn_800DD970();
    r29 = 0x0;
L_800E0E7C: ;
    r24 = *(u32*)lbl_8047AB30;
    goto L_800E0F54;
L_800E0E84: ;
    r0 = *(u32*)lbl_8047AB68;
    r27 = r27 + 0x1;
    if ((u32)r24 < (u32)r0) goto L_800E0EA0;
    r0 = *(u32*)lbl_8047AB64;
    if ((u32)r24 <= (u32)r0) goto L_800E0EB0;
L_800E0EA0: ;
    r3 = r31 + 0x64;
    fn_800DD970();
    r29 = 0x0;
L_800E0EB0: ;
    r0 = *(u32*)lbl_8047AB38;
    if ((u32)r24 < (u32)r0) goto L_800E0ECC;
    r3 = r31 + 0x90;
    fn_800DD970();
    r29 = 0x0;
L_800E0ECC: ;
    r3 = *(u32*)((u8*)r24 + 0x8);
    r0 = *(u32*)lbl_8047AB38;
    r3 = r24 + r3;
    if ((u32)r3 <= (u32)r0) goto L_800E0EF0;
    r3 = r31 + 0xc0;
    fn_800DD970();
    r29 = 0x0;
L_800E0EF0: ;
    r30 = *(u32*)((u8*)r24 + 0x4);
    if ((u32)r30 == (u32)0x0) goto L_800E0F50;
    r0 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)r24) goto L_800E0F18;
    r3 = r31 + 0xf8;
    fn_800DD970();
    r29 = 0x0;
L_800E0F18: ;
    r0 = *(u32*)((u8*)r24 + 0x8);
    r0 = r24 + r0;
    if ((u32)r30 != (u32)r0) goto L_800E0F38;
    r3 = r31 + 0x124;
    fn_800DD970();
    r29 = 0x0;
L_800E0F38: ;
    if ((u32)r24 <= (u32)r30) goto L_800E0F50;
    r3 = r31 + 0x154;
    fn_800DD970();
    r29 = 0x0;
L_800E0F50: ;
    r24 = *(u32*)((u8*)r24 + 0x4);
L_800E0F54: ;
    if ((u32)r24 != (u32)0x0) goto L_800E0E84;
    r30 = *(u32*)lbl_8047AB34;
    goto L_800E11EC;
L_800E0F64: ;
    r0 = *(u16*)((u8*)r30 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E11E8;
    r3 = *(u32*)((u8*)r30 + 0x4);
    r28 = r28 + 0x1;
    r0 = *(u32*)lbl_8047AB68;
    if ((u32)r3 < (u32)r0) goto L_800E0F94;
    r0 = *(u32*)((u8*)r30 + 0x8);
    r0 = r3 + r0;
    if ((u32)r0 <= (u32)r4) goto L_800E0FA4;
L_800E0F94: ;
    r3 = r31 + 0x180;
    fn_800DD970();
    r29 = 0x0;
L_800E0FA4: ;
    r0 = *(u8*)lbl_8047AB28;
    if ((u32)r0 == (u32)0x0) goto L_800E11E8;
    r4 = *(u32*)((u8*)r30 + 0x4);
    r0 = *(u8*)((u8*)r4 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E0FC8;
    r0 = 0x0;
    goto L_800E1064;
L_800E0FC8: ;
    r0 = *(u8*)((u8*)r4 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_800E0FDC;
    r0 = 0x0;
    goto L_800E1064;
L_800E0FDC: ;
    r0 = *(u8*)((u8*)r4 + 0x2);
    if ((u32)r0 == (u32)0x0) goto L_800E0FF0;
    r0 = 0x0;
    goto L_800E1064;
L_800E0FF0: ;
    r0 = *(u8*)((u8*)r4 + 0x3);
    if ((u32)r0 == (u32)0x0) goto L_800E1004;
    r0 = 0x0;
    goto L_800E1064;
L_800E1004: ;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r3 = r4 + r3;
    r0 = *(u8*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E1024;
    r0 = 0x0;
    goto L_800E1064;
L_800E1024: ;
    r0 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_800E1038;
    r0 = 0x0;
    goto L_800E1064;
L_800E1038: ;
    r0 = *(u8*)((u8*)r3 + 0x2);
    if ((u32)r0 == (u32)0x0) goto L_800E104C;
    r0 = 0x0;
    goto L_800E1064;
L_800E104C: ;
    r0 = *(u8*)((u8*)r3 + 0x3);
    if ((u32)r0 == (u32)0x0) goto L_800E1060;
    r0 = 0x0;
    goto L_800E1064;
L_800E1060: ;
    r0 = 0x1;
L_800E1064: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_800E10B8;
    r4 = *(u16*)((u8*)r30 + 0x0);
    r3 = r31 + 0x1ac;
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
    r3 = r0 + r3;
    *(u8*)((u8*)r3 + 0x0) = r4;
    *(u8*)((u8*)r3 + 0x1) = r4;
    *(u8*)((u8*)r3 + 0x2) = r4;
    *(u8*)((u8*)r3 + 0x3) = r4;
L_800E10B8: ;
    r0 = *(u16*)((u8*)r30 + 0x2);
    if ((u32)r0 != (u32)0x0) goto L_800E11E8;
    r0 = *(u32*)((u8*)r30 + 0x8);
    r6 = 0x3d94;
    r5 = *(u32*)((u8*)r30 + 0x4);
    /* srwi. r4, r0, 1 */;
    r3 = r0 & 0x1;
    if ((u32)r0 == (u32)0x0) goto L_800E114C;
    /* srwi. r0, r4, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 == (u32)0x0) goto L_800E1138;
L_800E10E8: ;
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
    if (--ctr != 0) goto L_800E10E8;
    r4 = r4 & 0x7;
    if ((u32)r0 == (u32)0x0) goto L_800E114C;
L_800E1138: ;
    ctr_fn = (void(*)(void))r4;
L_800E113C: ;
    r0 = *(u16*)((u8*)r5 + 0x0);
    r5 = r5 + 0x2;
    r6 = r6 + r0;
    if (--ctr != 0) goto L_800E113C;
L_800E114C: ;
    if ((u32)r3 == (u32)0x0) goto L_800E11C4;
    /* srwi. r0, r3, 3 */;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r3 == (u32)0x0) goto L_800E11B0;
L_800E1160: ;
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
    if (--ctr != 0) goto L_800E1160;
    r3 = r3 & 0x7;
    if ((u32)r3 == (u32)0x0) goto L_800E11C4;
L_800E11B0: ;
    ctr_fn = (void(*)(void))r3;
L_800E11B4: ;
    r0 = *(u8*)((u8*)r5 + 0x0);
    r5 = r5 + 0x1;
    r6 = r6 + r0;
    if (--ctr != 0) goto L_800E11B4;
L_800E11C4: ;
    r3 = *(u16*)((u8*)r30 + 0xE);
    r0 = r6 & 0xFFFF;
    if ((u32)r3 == (u32)r0) goto L_800E11E8;
    r4 = *(u16*)((u8*)r30 + 0x0);
    r3 = r31 + 0x1d8;
    fn_800DD970();
    r29 = 0x0;
L_800E11E8: ;
L_800E11EC: ;
    r4 = *(u32*)lbl_8047AB38;
    if ((u32)r30 >= (u32)r4) goto L_800E0F64;
    r0 = r26 & 0xFF;
    if ((u32)r30 == (u32)r4) goto L_800E120C;
    r3 = r31 + 0x208;
    fn_800DD970();
L_800E120C: ;
    r30 = *(u32*)lbl_8047AB68;
    goto L_800E1368;
L_800E1214: ;
    r24 = *(u32*)lbl_8047AB34;
    r0 = r24 + 0x10;
    r0 = r0 - r3;
    r0 = (u32)r0 >> 4;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r24 < (u32)r3) goto L_800E1254;
L_800E1230: ;
    r0 = *(u16*)((u8*)r24 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E124C;
    r0 = *(u32*)((u8*)r24 + 0x4);
    if ((u32)r0 != (u32)r30) goto L_800E124C;
    goto L_800E1258;
L_800E124C: ;
    if (--ctr != 0) goto L_800E1230;
L_800E1254: ;
    r24 = 0x0;
L_800E1258: ;
    if ((u32)r24 == (u32)0x0) goto L_800E12A4;
    r7 = *(u16*)((u8*)r24 + 0x0);
    if ((u32)r7 == (u32)0x0) goto L_800E12A4;
    r0 = r26 & 0xFF;
    if ((u32)r7 == (u32)0x0) goto L_800E1298;
    r6 = *(u32*)((u8*)r24 + 0x8);
    r4 = r30;
    r8 = *(u16*)((u8*)r24 + 0x2);
    r3 = r31 + 0x21c;
    r5 = r6 + r30;
    r9 = *(u16*)((u8*)r24 + 0xC);
    fn_800DD970();
L_800E1298: ;
    r0 = *(u32*)((u8*)r24 + 0x8);
    r30 = r30 + r0;
    goto L_800E1368;
L_800E12A4: ;
    r0 = r26 & 0xFF;
    if ((u32)r7 == (u32)0x0) goto L_800E12D0;
    r6 = *(u32*)((u8*)r30 + 0x8);
    r4 = r30;
    r7 = *(u32*)((u8*)r30 + 0x0);
    r3 = r31 + 0x268;
    r5 = r6 + r30;
    r8 = *(u32*)((u8*)r30 + 0x4);
    fn_800DD970();
L_800E12D0: ;
    r3 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_800E12F4;
    r0 = *(u32*)lbl_8047AB68;
    if ((u32)r3 < (u32)r0) goto L_800E1318;
    r0 = *(u32*)lbl_8047AB64;
    if ((u32)r3 > (u32)r0) goto L_800E1318;
L_800E12F4: ;
    r3 = *(u32*)((u8*)r30 + 0x4);
    if ((u32)r3 == (u32)0x0) goto L_800E1330;
    r0 = *(u32*)lbl_8047AB68;
    if ((u32)r3 < (u32)r0) goto L_800E1318;
    r0 = *(u32*)lbl_8047AB64;
    if ((u32)r3 <= (u32)r0) goto L_800E1330;
L_800E1318: ;
    r4 = r30;
    r3 = r31 + 0x2a8;
    fn_800DD970();
    r29 = 0x0;
    goto L_800E1374;
L_800E1330: ;
    r5 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)lbl_8047AB38;
    r3 = r30 + r5;
    if ((u32)r3 > (u32)r0) goto L_800E134C;
    if ((u32)r5 != (u32)0x0) goto L_800E1364;
L_800E134C: ;
    r4 = r30;
    r3 = r31 + 0x2e8;
    fn_800DD970();
    r29 = 0x0;
    goto L_800E1374;
L_800E1364: ;
    r30 = r30 + r5;
L_800E1368: ;
    r3 = *(u32*)lbl_8047AB38;
    if ((u32)r30 < (u32)r3) goto L_800E1214;
L_800E1374: ;
    r5 = *(u32*)lbl_8047AB38;
    if ((u32)r30 == (u32)r5) goto L_800E1394;
    r4 = r30;
    r3 = r31 + 0x318;
    fn_800DD970();
    r29 = 0x0;
L_800E1394: ;
    r0 = *(u32*)lbl_8047AB4C;
    if ((u32)r28 == (u32)r0) goto L_800E13B0;
    r3 = r31 + 0x36c;
    fn_800DD970();
    r29 = 0x0;
L_800E13B0: ;
    r0 = r25 & 0xFF;
    if ((u32)r28 == (u32)r0) goto L_800E151C;
    r4 = *(u32*)lbl_8047AB68;
    r3 = r31 + 0x3a4;
    r5 = *(u32*)lbl_8047AB64;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB4C;
    r3 = r31 + 0x3cc;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB48;
    r3 = r31 + 0x3e8;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB38;
    r3 = r31 + 0x404;
    r0 = *(u32*)lbl_8047AB34;
    r4 = r0 - r4;
    r4 = r4 + 0x10;
    fn_800DD970();
    r3 = *(u32*)lbl_8047AB30;
    r4 = 0x0;
    goto L_800E1428;
L_800E1414: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 <= (u32)r4) goto L_800E1424;
    r4 = r0;
L_800E1424: ;
    r3 = *(u32*)((u8*)r3 + 0x4);
L_800E1428: ;
    if ((u32)r3 != (u32)0x0) goto L_800E1414;
    r3 = r31 + 0x424;
    fn_800DD970();
    r3 = *(u32*)lbl_8047AB30;
    r4 = 0x0;
    goto L_800E1454;
L_800E1448: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x4);
    r4 = r4 + r0;
L_800E1454: ;
    if ((u32)r3 != (u32)0x0) goto L_800E1448;
    r3 = r31 + 0x444;
    fn_800DD970();
    r4 = r27;
    r3 = r31 + 0x464;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB60;
    r3 = r31 + 0x480;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB5C;
    r3 = r31 + 0x49c;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB58;
    r3 = r31 + 0x4b8;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB54;
    r3 = r31 + 0x4d4;
    fn_800DD970();
    r4 = *(u32*)lbl_8047AB50;
    r3 = r31 + 0x4f0;
    fn_800DD970();
    r4 = (0x4330 << 16);
    r0 = r27 + r28;
    f2 = *(f64*)lbl_8047CB50;
    r3 = r31 + 0x50c;
    f3 = *(f32*)lbl_8047CB48;
    f0 = *(f64*)(sp + 0x8);
    *(u32*)(sp + 0x14) = r0;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f2;
    f0 = f1 / f0;
    f1 = f3 * f0;
    /* crset cr1eq */;
    fn_800DD38C();
    r3 = r31 + 0x52c;
    fn_800DD970();
L_800E151C: ;
    r0 = r29 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_800E152C;
    r0 = 0x0;
    *(u32*)lbl_8047AB3C = r0;
L_800E152C: ;
    r3 = r29;
    return;
}

/* fn_800E202C | Size: 0x70 */
void fn_800E202C(void) {
    extern u8 lbl_8047AB28[];
    extern u8 lbl_8047AB34[];
    extern u8 lbl_8047AB38[];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = *(u8*)lbl_8047AB28;
    if ((u32)r0 != (u32)0x1) goto L_800E203C;
L_800E203C: ;
    r4 = *(u32*)lbl_8047AB34;
    r5 = *(u32*)lbl_8047AB38;
    r0 = r4 + 0x10;
    r0 = r0 - r5;
    r0 = (u32)r0 >> 4;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r4 < (u32)r5) goto L_800E2080;
L_800E205C: ;
    r0 = *(u16*)((u8*)r4 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_800E2078;
    r0 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r0 != (u32)r3) goto L_800E2078;
    goto L_800E2084;
L_800E2078: ;
    if (--ctr != 0) goto L_800E205C;
L_800E2080: ;
    r4 = 0x0;
L_800E2084: ;
    if ((u32)r4 != (u32)0x0) goto L_800E2094;
    r3 = 0x0;
    return;
L_800E2094: ;
    r3 = *(u16*)((u8*)r4 + 0x0);
    return;
}
