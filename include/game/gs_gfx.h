/**
 * @file gs_gfx.h
 * @brief GSgfx -- Genius Sonority graphics subsystem for Pokemon Colosseum.
 *
 * GSgfx wraps the GX (Graphics eXtension) hardware layer and provides:
 *   - Framebuffer / XFB management
 *   - Matrix stack (model-view, projection)
 *   - Render-state machine (draw modes, swap-buffer control)
 *   - Video-mode configuration (NTSC / PAL / progressive)
 *   - Light and texture management hooks
 *
 * The global state is kept in a 0x5A0-byte structure allocated from GSmem
 * and stored in lbl_8047AA80.
 *
 * Debug strings:
 *   "GSgfx: unable to allocate gsgfx state!"
 *   "GSgfx: Init OK, state located at %08Xh (size=%d)"
 *   "GSgfx: invalid matrix index"
 *   "GSgfx: matrix stack underflow!"
 *   "GSgfx: matrix stack overflow!"
 *
 * Address range: 0x800D3074 - 0x800D3E4C (approx.)
 */
#ifndef GS_GFX_H
#define GS_GFX_H

#include "dolphin/types.h"

/* -----------------------------------------------------------------------
 * GSgfx global state structure -- 0x5A0 bytes.
 * Allocated from GSmem, pointer stored at lbl_8047AA80.
 * ----------------------------------------------------------------------- */
typedef struct GSgfxState {
    /* 0x000 */ u32  mode;           /* rendering mode (0=off, 1=init, 2=active) */
    /* 0x004 */ s32  frameCounter;   /* current frame number (-1 = uninit) */
    /* 0x008 */ u32  fifoSize;       /* GX command FIFO entry count */
    /* 0x00C */ u32  clearColor;     /* packed RGBA clear colour */
    /* 0x010 */ u32  field_10;
    /* 0x014 */ u32  drawFlags;      /* GX draw control flags */
    /* 0x018 */ u8   matrixDirty;    /* set when model-view needs reload */
    /* 0x019 */ u8   projDirty;
    /* 0x01A */ u8   viewportDirty;
    /* 0x01B */ u8   scissorDirty;
    /* 0x01C */ u32  pad_01C;
    /* 0x020 */ u32  lightMask;      /* bitmask of active lights */
    /* 0x024-0x044 */ u32 reserved[9]; /* matrix state, counters, etc. */
    /* 0x048 */ u32  xfbIndex;       /* current XFB being displayed */
    /* 0x04C */ u32  xfbCount;
    /* 0x050 */ u32  xfbAddr0;
    /* 0x054 */ u32  xfbAddr1;
    /* 0x058 */ u32  renderEnabled;  /* 1 = rendering pipeline active */
    /* 0x05C */ u8   vsyncFlag;
    /* 0x05D-0x47D */ u8 pad[0x422];
    /* 0x47E */ u8   gammaMode;
    /* 0x47F */ u8   pad_47F;
    /* 0x480 */ u32  field_480;
    /* 0x484 */ u32  field_484;
    /* 0x488 */ s32  prevMode;       /* previous rendering mode before switch */
    /* 0x48C-0x49B */ u8 pad2[0x10];
    /* 0x49C */ u8   interlaceMode;
    /* 0x49D */ u8   progressiveFlag;
    /* 0x49E */ u8   pad_49E;
    /* 0x49F */ u8   field_49F;
    /* 0x4A0-0x59F */ u8 pad3[0x100];
} GSgfxState;

/* -----------------------------------------------------------------------
 * Video mode constants (for GSgfxSetVideoMode)
 * ----------------------------------------------------------------------- */
#define GSGFX_VMODE_NTSC       1
#define GSGFX_VMODE_PAL50      2
#define GSGFX_VMODE_PAL60      3
#define GSGFX_VMODE_PROGRESSIVE 4

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

/**
 * GSgfxInit -- Initialise the graphics state machine.
 *
 * @param memSize    Size of the internal rendering memory pool.
 * @param fifoSize   Number of GX command FIFO entries.
 * @param mtxDepth   Matrix stack depth.
 * @param lightCount Maximum number of active lights.
 * @param numBufs    Number of framebuffers (1 or 2).
 * @param dlSize     Display-list buffer size.
 *
 * Allocates a 0x5A0-byte GSgfxState from GSmem, initialises the GX
 * hardware via VIConfigure / GXInit, sets up the matrix stack and
 * viewport, and registers a VBlank callback.
 *
 * Corresponds to fn_800D39E0.
 */
void GSgfxInit(u32 memSize, u32 fifoSize, u32 mtxDepth,
               u32 lightCount, u32 numBufs, u32 dlSize);

/**
 * GSgfxSetVideoMode -- Reconfigure the video output mode.
 *
 * @param mode       Video mode (see GSGFX_VMODE_* constants).
 * @param tvFormat   TV format parameter.
 * @param field0     Interlace sub-parameter.
 * @param field1     Interlace sub-parameter.
 * @param xfbMode    XFB copy mode (0=single, 1=double).
 * @param aaMode     Anti-aliasing mode.
 *
 * Corresponds to fn_800D37D4.
 */
void GSgfxSetVideoMode(u32 mode, u32 tvFormat, u32 field0,
                        u32 field1, u32 xfbMode, u32 aaMode);

/**
 * GSgfxEnableRendering -- Turn the rendering pipeline on or off.
 *
 * @param enable     1 = enable, 0 = disable.
 *
 * Corresponds to fn_800D3074.
 */
void GSgfxEnableRendering(u32 enable);

/**
 * GSgfxSwapBuffers -- Signal end-of-frame and swap framebuffers.
 *
 * @param flag       Swap control flag.
 *
 * Corresponds to fn_800D30F0.
 */
void GSgfxSwapBuffers(u32 flag);

/**
 * GSgfxSetDrawMode -- Set the per-frame draw mode.
 *
 * @param mode       Draw mode byte.
 *
 * Corresponds to fn_800D361C.
 */
void GSgfxSetDrawMode(u8 mode);

/**
 * GSgfxGetFrameCount -- Return the current frame counter value.
 *
 * Corresponds to fn_800D37CC.
 */
u32 GSgfxGetFrameCount(void);

/**
 * GSgfxGetTickCount -- Return the internal tick counter.
 *
 * Corresponds to fn_800D3088.
 */
u32 GSgfxGetTickCount(void);

#endif /* GS_GFX_H */
