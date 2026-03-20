/**
 * @file gs_gfx.c
 * @brief GSgfx -- Genius Sonority graphics subsystem.
 *
 * Decompiled from:
 *   fn_800D39E0 (GSgfxInit)
 *   fn_800D37D4 (GSgfxSetVideoMode)
 *   fn_800D3074 (GSgfxEnableRendering)
 *   fn_800D30F0 (GSgfxSwapBuffers)
 *   fn_800D361C (GSgfxSetDrawMode)
 *   fn_800D37CC (GSgfxGetFrameCount)
 *   fn_800D3088 (GSgfxGetTickCount)
 *
 * Debug strings:
 *   "GSgfx: unable to allocate gsgfx state!"
 *   "GSgfx: Init OK, state located at %08Xh (size=%d)"
 *   "GSgfx: invalid matrix index"
 *   "GSgfx: matrix stack underflow!"
 *   "GSgfx: matrix stack overflow!"
 *
 * The graphics subsystem wraps GX and VI, managing:
 *   - A 0x5A0-byte state structure allocated from GSmem
 *   - Video mode configuration (NTSC/PAL/progressive)
 *   - Matrix stack for model-view transforms
 *   - Framebuffer management (XFB)
 *   - Render pipeline control (enable/disable, swap, draw modes)
 *   - GX FIFO and display list setup
 *   - Light, texture, and viewport init (delegated to sub-functions)
 *
 * Address range: 0x800D3074 - 0x800D3E4C (approx.)
 */

#include "dolphin/types.h"
#include "game/gs_gfx.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);        /* OSReport */
extern u16   GSmemAllocRaw(u32 size);                  /* fn_800E3534 */
extern void* GSmemGetPtr(u16 handle);                  /* fn_800E27B0 */
extern void  fn_800E0790(void);                        /* GSmem stats/check */
extern void  fn_800EEC38(u32 slot, void* callback);    /* VI register retrace cb */
extern void  fn_8019C3C4(u32 a, ...);                  /* VIConfigure wrapper */
extern void  fn_8019CB70(void);                        /* VIFlush / apply config */
extern void  fn_800D37D4(u32 mode, u32 tvFmt, u32 a,
                          u32 b, u32 xfbMode, u32 aa); /* GSgfxSetVideoMode */
extern void  fn_801BF4C4(u32 flag);                    /* GXSetDispCopyGamma */
extern void  fn_8019C690(u32 a, u32 b);                /* VISetBlack / display ctrl */
extern void  fn_801C021C(void);                        /* GXInit or FIFO init */
extern void  fn_801C01C8(void* callback);              /* GX register draw-done cb */
extern void  fn_80196C3C(void* callback);              /* VI pre-retrace callback */
extern void  fn_800D5504(u32 memSize);                 /* GSgfx alloc render mem */
extern void  fn_800D83E4(u32 fifoSize);                /* GSgfx init matrix stack */
extern void  fn_800D7B80(u32 mtxDepth);                /* GSgfx init projection */
extern void  fn_800DB890(u32 lightCount);              /* GSgfx init lighting */
extern void  fn_800D7894(void);                        /* GSgfx init viewport */
extern void  fn_800D9D68(u32 x0, u32 y0, u32 x1, u32 y1); /* GSgfx set scissor */
extern void  fn_800D9C24(u32 x0, u32 y0, u32 x1, u32 y1); /* GSgfx set viewport rect */
extern void  fn_800D87AC(s32 mode);                    /* GSgfx set internal mode */
extern void  fn_800DA2BC(u32 a, u32 b, u32 c);        /* GSgfx configure blend mode */
extern void  fn_800DA1E8(u32 a, u32 b, u32 c);        /* GSgfx configure Z mode */
extern void  fn_800DA100(u32 a, u32 b, u32 c,
                          u32 d, u32 e, u32 f);        /* GSgfx configure alpha mode */
extern void  fn_800DA028(u32 a);                       /* GSgfx configure TEV mode */
extern void  fn_800D9F40(u32 a);                       /* GSgfx configure fog */

/* Internal callbacks */
extern void  fn_800D3E4C(void);   /* VBlank retrace callback */
extern void  fn_800D3F5C(void);   /* Frame-end callback */
extern void  fn_800D3F50(void);   /* Draw-done callback */
extern void  fn_800D3EC4(void);   /* Pre-retrace callback */

/* ===== String constants (rodata) ===== */
extern const char lbl_80270360[]; /* "GSgfx: unable to allocate gsgfx state!\n" */
extern const char lbl_80270388[]; /* "GSgfx: Init OK, state located at %08Xh..." */

/* ===== Video mode tables (data section) ===== */
extern u8 lbl_80312D30[];  /* NTSC video mode table (mode 1) */
extern u8 lbl_803130F0[];  /* PAL50 video mode table (mode 2) */
extern u8 lbl_80312F4C[];  /* PAL60 video mode table (mode 4) */
extern u8 lbl_80466BC0[];  /* current display descriptor (active VI config) */
extern u8 lbl_804001F0[];  /* GX state / FIFO state block */

/* ===== Global state (sbss) ===== */

/* lbl_8047AA80 : GSgfxState* -- pointer to the graphics state struct */
static GSgfxState* gsGfxState;  /* @sda21 lbl_8047AA80 */

/* lbl_8047AAA0 : u32 -- display list buffer size parameter */
static u32 gsGfxDLSize;         /* @sda21 lbl_8047AAA0 */

/* lbl_8047AA9C : u8 -- progressive scan flag */
static u8 gsGfxProgressive;     /* @sda21 lbl_8047AA9C */

/* =======================================================================
 *  GSgfxEnableRendering / fn_800D3074
 *  Address: 0x800D3074, Size: 0x14
 *
 *  Simple setter: stores the enable flag into gsGfxState->renderEnabled.
 *
 *  Assembly:
 *    lwz r0, lbl_8047AA80@sda21(r0)   ; load state ptr
 *    stw r3, 0x58(r0)                  ; store flag at offset 0x58
 * ======================================================================= */
void GSgfxEnableRendering(u32 enable) {
    if (gsGfxState != NULL) {
        gsGfxState->renderEnabled = enable;
    }
}

/* =======================================================================
 *  GSgfxGetTickCount / fn_800D3088
 *  Address: 0x800D3088, Size: 0x68
 *
 *  Returns an internal tick counter from the state.
 * ======================================================================= */
u32 GSgfxGetTickCount(void) {
    if (gsGfxState == NULL) {
        return 0;
    }
    return gsGfxState->field_10;
}

/* =======================================================================
 *  GSgfxSwapBuffers / fn_800D30F0
 *  Address: 0x800D30F0, Size: 0xF0
 *
 *  Signals end-of-frame, triggers copy-out of the embedded framebuffer,
 *  and handles double-buffering swap.
 *
 *  Simplified -- the full function manipulates XFB indices and calls
 *  into GX/VI to copy the colour buffer.
 * ======================================================================= */
void GSgfxSwapBuffers(u32 flag) {
    if (gsGfxState == NULL) {
        return;
    }

    /* Toggle the XFB index for double buffering */
    gsGfxState->xfbIndex ^= 1;

    /* The assembly then calls VICopyXFB, GXCopyDisp, etc.
     * to perform the actual buffer swap.  We leave these
     * as external calls since the GX wrapper code is separate. */
}

/* =======================================================================
 *  fn_800D3094 -- GSgfxGetXFBCount
 *  Address: 0x800D3094, Size: 0xC
 *
 *  Returns the XFB count field (offset 0x4C) from the state.
 * ======================================================================= */
u32 fn_800D3094(void) {
    return gsGfxState->xfbCount;
}

/* =======================================================================
 *  fn_800D36B4 -- GSgfxSetClearColor
 *  Address: 0x800D36B4, Size: 0xC8
 *
 *  Sets the clear color from a float[4] RGBA array. Each component
 *  (0.0-1.0 range) is multiplied by a scaling constant and stored
 *  as a u8 in the state at offsets 0x1C-0x1F. If all components
 *  are zero, clears the projDirty flag.
 *
 *  r3 = pointer to float[4] RGBA color
 * ======================================================================= */
void fn_800D36B4(f32* color) {
    f32 scale;
    s32 r, g, b, a;

    scale = 255.0f;

    gsGfxState->projDirty = 1;

    r = (s32)(scale * color[0]);
    g = (s32)(scale * color[1]);
    b = (s32)(scale * color[2]);
    a = (s32)(scale * color[3]);

    *(u8*)((u8*)gsGfxState + 0x1C) = (u8)r;
    *(u8*)((u8*)gsGfxState + 0x1D) = (u8)g;
    *(u8*)((u8*)gsGfxState + 0x1E) = (u8)b;
    *(u8*)((u8*)gsGfxState + 0x1F) = (u8)a;

    if (*(u8*)((u8*)gsGfxState + 0x1C) == 0 &&
        *(u8*)((u8*)gsGfxState + 0x1D) == 0 &&
        *(u8*)((u8*)gsGfxState + 0x1E) == 0 &&
        *(u8*)((u8*)gsGfxState + 0x1F) == 0) {
        gsGfxState->projDirty = 0;
    }
}

/* =======================================================================
 *  fn_800D377C -- GSgfxSetBlackScreen
 *  Address: 0x800D377C, Size: 0x50
 *
 *  Controls screen blanking based on mode parameter.
 *    mode 1: VISetBlack(0, 0) -- unblank
 *    mode 2: VISetBlack(1, 0) -- blank
 *    other:  do nothing
 * ======================================================================= */
void fn_800D377C(s32 mode) {
    switch (mode) {
    case 1:
        fn_8019C690(0, 0);
        break;
    case 2:
        fn_8019C690(1, 0);
        break;
    default:
        break;
    }
}

/* =======================================================================
 *  GSgfxSetDrawMode / fn_800D361C
 *  Address: 0x800D361C
 *
 *  Stores the per-frame draw mode byte into the state.
 * ======================================================================= */
void GSgfxSetDrawMode(u8 mode) {
    if (gsGfxState != NULL) {
        gsGfxState->matrixDirty = mode;
    }
}

/* =======================================================================
 *  GSgfxGetFrameCount / fn_800D37CC
 *  Address: 0x800D37CC
 *
 *  Returns the frame counter from the state.
 * ======================================================================= */
u32 GSgfxGetFrameCount(void) {
    if (gsGfxState == NULL) {
        return 0;
    }
    return (u32)gsGfxState->frameCounter;
}

/* =======================================================================
 *  GSgfxSetVideoMode / fn_800D37D4
 *  Address: 0x800D37D4, Size: 0x20C
 *
 *  Reconfigures the video output.  Selects a mode table pointer based
 *  on the mode parameter, then calls VIConfigure and related functions.
 *
 *  r3 = mode, r4 = tvFormat, r5 = field0, r6 = field1,
 *  r7 = xfbMode, r8 = aaMode
 *
 *  Assembly (abbreviated):
 *    switch (mode):
 *      case 1: modeTable = lbl_80312D30 (NTSC)
 *      case 2: modeTable = lbl_803130F0 (PAL50)
 *      case 4: modeTable = lbl_80312F4C (PAL60/progressive)
 *    if (mode == 2 || mode == 3):
 *      gsGfxProgressive = 1
 *    else:
 *      gsGfxProgressive = 0
 *    VIConfigure(1, 2)
 *    VIConfigure(4, modeTable)
 *    VIFlush()
 *    GSgfxSetVideoMode_internal(mode, tvFormat, field0, field1, xfbMode, aaMode)
 *    ...etc
 * ======================================================================= */
void GSgfxSetVideoMode(u32 mode, u32 tvFormat, u32 field0,
                        u32 field1, u32 xfbMode, u32 aaMode) {
    void* modeTable;

    /* Select video mode table */
    switch (mode) {
        case 1:
            modeTable = (void*)lbl_80312D30;
            break;
        case 2:
            modeTable = (void*)lbl_803130F0;
            break;
        case 4:
            modeTable = (void*)lbl_80312F4C;
            break;
        default:
            modeTable = (void*)lbl_80312F4C;
            break;
    }

    /* Set progressive scan flag */
    if (mode == 2 || mode == 3) {
        gsGfxProgressive = 1;
    } else {
        gsGfxProgressive = 0;
    }

    /* Configure the video interface */
    fn_8019C3C4(1, 2);
    fn_8019C3C4(4, modeTable);
    fn_8019CB70();

    /* Apply the mode to the internal rendering state */
    /* (calls into viewport, scissor, XFB copy setup, etc.) */
}

/* =======================================================================
 *  GSgfxInit / fn_800D39E0
 *  Address: 0x800D39E0, Size: 0x468
 *
 *  Master graphics initialisation.  This is the largest function in the
 *  GSgfx module.
 *
 *  r3 = memSize, r4 = fifoSize, r5 = mtxDepth,
 *  r6 = lightCount, r7 = numBufs, r8 = dlSize
 *
 *  Assembly sequence (heavily abbreviated):
 *
 *  1. fn_800E0790()                    -- GSmem stats snapshot
 *  2. fn_800EEC38(3, fn_800D3E4C)      -- register VBlank retrace cb slot 3
 *  3. if (state already exists) goto skip_alloc
 *     handle = GSmemAllocRaw(0x5A0)    -- allocate state struct
 *     if (handle == 0):
 *       print "GSgfx: unable to allocate gsgfx state!"
 *       return
 *     gsGfxState = GSmemGetPtr(handle)
 *  4. skip_alloc:
 *     gsGfxState->mode = 2
 *     gsGfxState->frameCounter = -1
 *     gsGfxState->fifoSize = 0x10
 *     gsGfxState->clearColor = 0xFEFFFEFE
 *     gsGfxState->field_10 = 0
 *     gsGfxState->drawFlags = 3
 *     zero out bytes 0x18-0x1B, 0x49C-0x49D
 *     zero out fields 0x20-0x44
 *     zero out fields 0x47E, 0x480, 0x484
 *     gsGfxState->prevMode = -1
 *     gsGfxState->field_49F = 0
 *  5. Clear HW state block at lbl_804001F0 (GX FIFO state)
 *  6. gsGfxDLSize = dlSize
 *  7. Select video mode table based on numBufs:
 *       1 -> NTSC (lbl_80312D30)
 *       2 -> PAL50 (lbl_803130F0)
 *       4 -> PAL60 (lbl_80312F4C)
 *  8. Set progressive flag if numBufs == 2 or 3
 *  9. VIConfigure(1, 2)
 *     VIConfigure(4, modeTable)
 *     VIFlush()
 *  10. GSgfxSetVideoMode(numBufs, 2, 0, 2, 0, 0)
 *  11. GXSetDispCopyGamma(1)
 *  12. VISetBlack(0, 0)
 *  13. Register callbacks:
 *      fn_801C021C()                 -- GXInit / FIFO init
 *      fn_801C01C8(fn_800D3F50)      -- draw-done callback
 *      fn_80196C3C(fn_800D3EC4)      -- pre-retrace callback
 *  14. Sub-system init:
 *      fn_800D5504(memSize)          -- render memory pool
 *      fn_800D83E4(fifoSize)         -- matrix stack
 *      fn_800D7B80(mtxDepth)         -- projection matrices
 *      fn_800DB890(lightCount)       -- light array
 *      fn_800D7894()                 -- viewport defaults
 *  15. Set mode to 2 (active), store previous mode
 *  16. Set viewport and scissor to full-screen using display descriptor:
 *      width  = lbl_80466BC0[4] - 1
 *      height = lbl_80466BC0[6] - 1
 *      fn_800D9D68(0, 0, width, height)
 *      fn_800D9C24(0, 0, width, height)
 *  17. fn_800D87AC(-1)               -- reset internal mode
 *  18. Configure default render states:
 *      fn_800DA2BC(1, 1, 1)          -- blend mode
 *      fn_800DA1E8(1, 2, 1)          -- Z mode
 *      fn_800DA100(0, 7, 0, 1, 7, 0) -- alpha compare
 *      fn_800DA028(2)                -- TEV mode
 *      fn_800D9F40(0)                -- fog off
 *  19. More state setup...
 *  20. Print "GSgfx: Init OK, state located at %08Xh (size=%d)"
 *
 * ======================================================================= */
void GSgfxInit(u32 memSize, u32 fifoSize, u32 mtxDepth,
               u32 lightCount, u32 numBufs, u32 dlSize) {
    u16 handle;
    void* modeTable;
    u16 width, height;
    u8* disp;

    /* Step 1: Snapshot GSmem stats */
    fn_800E0790();

    /* Step 2: Register VBlank retrace callback in slot 3 */
    fn_800EEC38(3, (void*)fn_800D3E4C);

    /* Step 3: Allocate or reuse the 0x5A0-byte state structure */
    if (gsGfxState == NULL) {
        handle = GSmemAllocRaw(0x5A0);
        if ((handle & 0xFFFF) == 0) {
            fn_800DD970(lbl_80270360);
            return;
        }
        gsGfxState = (GSgfxState*)GSmemGetPtr(handle);
    }

    /* Step 4: Initialise the state fields */
    gsGfxState->mode         = 2;
    gsGfxState->frameCounter = -1;
    gsGfxState->fifoSize     = 0x10;
    gsGfxState->clearColor   = 0xFEFFFEFE;
    gsGfxState->field_10     = 0;
    gsGfxState->drawFlags    = 3;

    gsGfxState->matrixDirty   = 0;
    gsGfxState->projDirty     = 0;
    gsGfxState->viewportDirty = 0;
    gsGfxState->scissorDirty  = 0;

    gsGfxState->interlaceMode  = 0;
    gsGfxState->progressiveFlag = 0;

    gsGfxState->lightMask = 0;
    /* Zero fields 0x24-0x44 */
    {
        u32* p = (u32*)((u32)gsGfxState + 0x24);
        u32 i;
        for (i = 0; i < 9; i++) {
            p[i] = 0;
        }
    }

    gsGfxState->gammaMode  = 0;
    gsGfxState->field_480  = 0;
    gsGfxState->field_484  = 0;
    gsGfxState->prevMode   = -1;
    gsGfxState->field_49F  = 0;

    /* Step 5: Clear GX FIFO state block (22 words at lbl_804001F0) */
    {
        u32* fifoState = (u32*)lbl_804001F0;
        u32 i;
        for (i = 0; i < 22; i++) {
            fifoState[i] = 0;
        }
    }

    /* Step 6: Store display list buffer size */
    gsGfxDLSize = dlSize;

    /* Step 7: Select video mode table based on numBufs */
    switch (numBufs) {
        case 1:
            modeTable = (void*)lbl_80312D30;
            break;
        case 2:
            modeTable = (void*)lbl_803130F0;
            break;
        case 4:
            modeTable = (void*)lbl_80312F4C;
            break;
        default:
            modeTable = (void*)lbl_80312F4C;
            break;
    }

    /* Step 8: Set progressive scan flag */
    if (numBufs == 2 || numBufs == 3) {
        gsGfxProgressive = 1;
    } else {
        gsGfxProgressive = 0;
    }

    /* Step 9: Configure the video interface */
    fn_8019C3C4(1, 2);
    fn_8019C3C4(4, modeTable);
    fn_8019CB70();

    /* Step 10: Set video mode internally */
    fn_800D37D4(numBufs, 2, 0, 2, 0, 0);

    /* Step 11: Set display copy gamma correction */
    fn_801BF4C4(1);

    /* Step 12: Ensure display is not blanked */
    fn_8019C690(0, 0);

    /* Step 13: Register callbacks and initialise GX */
    gsGfxState->xfbIndex = 0;
    gsGfxState->xfbCount = 0;
    gsGfxState->xfbAddr0 = 0;
    gsGfxState->xfbAddr1 = 0;
    gsGfxState->renderEnabled = 1;
    gsGfxState->vsyncFlag     = 1;

    fn_801C021C();                         /* GXInit / FIFO */
    fn_801C01C8((void*)fn_800D3F50);       /* draw-done callback */
    fn_80196C3C((void*)fn_800D3EC4);       /* pre-retrace callback */

    /* Step 14: Initialise sub-systems */
    fn_800D5504(memSize);    /* render memory pool */
    fn_800D83E4(fifoSize);   /* matrix stack */
    fn_800D7B80(mtxDepth);   /* projection matrices */
    fn_800DB890(lightCount); /* light array */
    fn_800D7894();           /* viewport defaults */

    /* Step 15: Set mode to active, store previous mode */
    {
        u32 prevMode = gsGfxState->mode;
        gsGfxState->mode = 2;
        gsGfxState->prevMode = (s32)prevMode;
    }

    /* Step 16: Set viewport and scissor to full screen
     * Read display descriptor at lbl_80466BC0 for width/height. */
    disp = (u8*)lbl_80466BC0;
    width  = *(u16*)(disp + 4) - 1;
    height = *(u16*)(disp + 6) - 1;

    fn_800D9D68(0, 0, (u32)width, (u32)height);  /* scissor */
    fn_800D9C24(0, 0, (u32)width, (u32)height);  /* viewport */

    /* Step 17: Reset internal mode */
    fn_800D87AC(-1);

    /* Step 18: Configure default render states */
    fn_800DA2BC(1, 1, 1);           /* blend: src=1, dst=1, op=1 */
    fn_800DA1E8(1, 2, 1);           /* Z: enable, func=LEQUAL, update=1 */
    fn_800DA100(0, 7, 0, 1, 7, 0); /* alpha compare */
    fn_800DA028(2);                 /* TEV mode 2 */
    fn_800D9F40(0);                 /* fog disabled */

    /* Print success message */
    fn_800DD970(lbl_80270388, (u32)gsGfxState, 0x5A0);
}

/* =======================================================================
 *  fn_800D30A0
 *  Address: 0x800D30A0, Size: 0xC
 * ======================================================================= */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D30A0(void) {
    /* TODO: match -- 0xC bytes */
}
#pragma pop

/* =======================================================================
 *  fn_800D30AC
 *  Address: 0x800D30AC, Size: 0x44
 * ======================================================================= */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D30AC(void) {
    /* TODO: match -- 0x44 bytes */
}
#pragma pop

/* =======================================================================
 *  fn_800D3190 -- GSgfxCopyXFB
 *  Address: 0x800D3190, Size: 0x280
 * ======================================================================= */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D3190(void) {
    /* TODO: match -- 0x280 bytes */
}
#pragma pop

/* =======================================================================
 *  fn_800D3410 -- GSgfxConfigureVideoOutput
 *  Address: 0x800D3410, Size: 0x20C
 * ======================================================================= */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D3410(void) {
    /* TODO: match -- 0x20C bytes */
}
#pragma pop

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

extern u32 lbl_8047AA94;

/* Address: 0x800D37CC | Size: 0x8 | Pattern: sda_getter */
u32 fn_800D37CC(void) {
    return lbl_8047AA94;
}
