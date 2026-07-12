#ifndef DOLPHIN_GX_GX_H
#define DOLPHIN_GX_GX_H

#include "dolphin/types.h"

/* GX FIFO structure */
typedef struct GXFifoObj {
    u8* base;
    u8* top;
    u32 size;
    u32 hiWatermark;
    u32 loWatermark;
    void* rdPtr;
    void* wrPtr;
    s32 count;
    u8  wrap;
    u8  pad[3];
} GXFifoObj;

/* Forward declarations for common GX types */
typedef u8 GXBool;
typedef u8 GXTexFmt;
typedef u8 GXTlutFmt;

/* Texture cache region */
typedef struct GXTexRegion {
    u32 unk[4];
} GXTexRegion;

/* TLUT region */
typedef struct GXTlutRegion {
    u32 unk[4];
} GXTlutRegion;

/* Render mode */
typedef struct GXRenderModeObj {
    u32 viTVmode;
    u16 fbWidth;
    u16 efbHeight;
    u16 xfbHeight;
    u16 viXOrigin;
    u16 viYOrigin;
    u16 viWidth;
    u16 viHeight;
    u32 xfbMode;
    u8  field_rendering;
    u8  aa;
    u8  sample_pattern[12][2];
    u8  vfilter[7];
} GXRenderModeObj;

/* Internal GX context data (opaque) */
typedef struct GXData GXData;

/* Function declarations */
void* GXInit(void* base, u32 size);
void __GXInitGX(void);
void GXInitFifoBase(GXFifoObj* fifo, void* base, u32 size);
void GXSetCPUFifo(GXFifoObj* fifo);
void GXSetGPFifo(GXFifoObj* fifo);
void __GXFifoInit(void);
void __GXPEInit(void);
void GXSetMisc(u32 token, u32 val);
void GXInitTexCacheRegion(GXTexRegion* region, GXBool is32bMipmap,
                          u32 tmemEven, u32 sizeEven,
                          u32 tmemOdd, u32 sizeOdd);
void GXInitTlutRegion(GXTlutRegion* region, u32 tmemAddr, u8 tlutSize);
void __GXSetTmemConfig(u32 config);
void __GXFlushTextureState(void);

/* ========================================================================= */
/*  GXColor                                                                  */
/* ========================================================================= */

typedef struct _GXColor {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} GXColor;

#endif /* DOLPHIN_GX_GX_H */
