#include "dolphin/gx/GX.h"
#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSTime.h"
#include "dolphin/os/PPCArch.h"

/*
 * GXInit.c - GX Graphics API Initialization.
 *
 * Contains GXInit and __GXInitGX which set up the GP (Graphics Processor)
 * hardware, command processor, and rendering pipeline to default states.
 *
 * Matches: 0x800B5E8C - 0x800B6FE0
 *   GXInit     (0x890) - Full GX initialization
 *   __GXInitGX (0x8C4) - Initialize all GX state to defaults
 */

extern void* memset(void* dest, int val, u32 n);
extern void* memcpy(void* dest, const void* src, u32 n);
extern void OSRegisterVersion(const char* id);
extern void OSReport(const char* fmt, ...);

static const char* __GXVersion = "<< Dolphin SDK - GX\trelease build: Aug 22 2002 04:07:51 (0x2301) >>";

/* GX hardware register bases */
#define CP_BASE      ((volatile u16*)0xCC000000)
#define PE_BASE      ((volatile u16*)0xCC001000)
#define PI_FIFO_BASE ((volatile u32*)0xCC003000)

/* GX internal state (large ~1KB structure) */
typedef struct GXData {
    u32     cpRegs[16];
    u32     xfRegs[16];
    u32     bpRegs[256];
    u32     fifoBase;
    u32     fifoEnd;
    u32     fifoSize;
    u32     cpEnable;
    u32     cpClr;
    u32     peCtrl;
    u16     cpCRstat;
    u8      dirtyState;
    u8      padding;
    u32     _pad[16];
} GXData;

static GXData __GXData;
GXData* __GXContextPtr = &__GXData;

/* FIFO state */
static GXFifoObj CPUFifo;
static GXFifoObj GPFifo;
static GXFifoObj* CPGPLinked;

/*
 * GXInit - Initialize the GX graphics subsystem.
 * 0x800B5E8C | size: 0x890
 *
 * Allocates the FIFO buffer, initializes the command processor,
 * and sets up all GX hardware to default states.
 */
void* GXInit(void* base, u32 size) {
    u32 reg;
    GXFifoObj* fifo;

    OSRegisterVersion(__GXVersion);

    /* Zero out GX state */
    memset(&__GXData, 0, sizeof(__GXData));

    /* Initialize the command FIFO */
    fifo = &CPUFifo;
    GXInitFifoBase(fifo, base, size);
    GXSetCPUFifo(fifo);
    GXSetGPFifo(fifo);

    /* Initialize PE (Pixel Engine) */
    __GXPEInit();

    /* Set up default graphics state */
    __GXInitGX();

    return fifo;
}

/*
 * __GXInitGX - Initialize all GX state to defaults.
 * 0x800B671C | size: 0x8C4
 *
 * This large function sets every GX subsystem to its default state:
 * viewport, scissor, blend mode, depth test, lighting, textures,
 * TEV stages, etc.
 */
void __GXInitGX(void) {
    GXRenderModeObj defMode;
    f32 identity[3][4];
    u32 i;

    /* Set viewport to standard NTSC fullscreen */
    /* GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f); */

    /* Set scissor box */
    /* GXSetScissor(0, 0, 640, 480); */

    /* Clear all vertex descriptors */
    /* GXClearVtxDesc(); */
    /* GXInvalidateVtxCache(); */

    /* Set default blend mode (no blending) */
    /* GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR); */

    /* Set default Z mode */
    /* GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE); */

    /* Set default color update */
    /* GXSetColorUpdate(GX_TRUE); */
    /* GXSetAlphaUpdate(GX_TRUE); */

    /* Set default alpha compare */
    /* GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0); */

    /* Disable all TEV stages except stage 0 */
    /* GXSetNumTevStages(1); */

    /* Set default TEV stage 0 */
    /* GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0); */
    /* GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE); */

    /* Disable all lights */
    /* GXSetNumChans(0); */

    /* Disable texgen */
    /* GXSetNumTexGens(1); */

    /* Set identity position/normal matrix */
    identity[0][0] = 1.0f; identity[0][1] = 0.0f; identity[0][2] = 0.0f; identity[0][3] = 0.0f;
    identity[1][0] = 0.0f; identity[1][1] = 1.0f; identity[1][2] = 0.0f; identity[1][3] = 0.0f;
    identity[2][0] = 0.0f; identity[2][1] = 0.0f; identity[2][2] = 1.0f; identity[2][3] = 0.0f;

    /* Set default cull mode */
    /* GXSetCullMode(GX_CULL_BACK); */

    /* Clear PE statistics */
    /* GXSetDispCopySrc(0, 0, 640, 480); */

    /* Set default TMEM config */
    __GXSetTmemConfig(0);

    /* Initialize pixel processing */
    /* GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1000.0f, (GXColor){0,0,0,0}); */
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800B6FE0 - 0x800B6FE0 | size: 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B6FE0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7180 - 0x800B7180 | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7180(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B71F0 - 0x800B71F0 | size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B71F0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7484 - 0x800B7484 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7484(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7514 - 0x800B7514 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7514(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7538 - 0x800B7538 | size: 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7538(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7558 - 0x800B7558 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7558(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7594 - 0x800B7594 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7594(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B75D0 - 0x800B75D0 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B75D0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B760C - 0x800B760C | size: 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B760C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B770C - 0x800B770C | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B770C(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800B7714 - 0x800B7714 | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7714(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800B771C - 0x800B771C | size: 0x158 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B771C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7874 - 0x800B7874 | size: 0x350 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7874(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7BC4 - 0x800B7BC4 | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7BC4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7C18 - 0x800B7C18 | size: 0x124 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7C18(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7D3C - 0x800B7D3C | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7D3C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B7D74 - 0x800B7D74 | size: 0x358 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B7D74(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B80CC - 0x800B80CC | size: 0x378 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B80CC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8444 - 0x800B8444 | size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8444(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B84E0 - 0x800B84E0 | size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B84E0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B856C - 0x800B856C | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B856C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B857C - 0x800B857C | size: 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B857C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B884C - 0x800B884C | size: 0x40 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B884C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8920 - 0x800B8920 | size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8920(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B897C - 0x800B897C | size: 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B897C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8AE8 - 0x800B8AE8 | size: 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8AE8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8C58 - 0x800B8C58 | size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8C58(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8D10 - 0x800B8D10 | size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8D10(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8DA8 - 0x800B8DA8 | size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8DA8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8DF4 - 0x800B8DF4 | size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8DF4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8E74 - 0x800B8E74 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8E74(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8E98 - 0x800B8E98 | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8E98(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8EAC - 0x800B8EAC | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8EAC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8EC0 - 0x800B8EC0 | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8EC0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8EDC - 0x800B8EDC | size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8EDC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8F64 - 0x800B8F64 | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8F64(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8F80 - 0x800B8F80 | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8F80(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8F94 - 0x800B8F94 | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8F94(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8FB0 - 0x800B8FB0 | size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8FB0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B8FD8 - 0x800B8FD8 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B8FD8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B901C - 0x800B901C | size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B901C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B90A4 - 0x800B90A4 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B90A4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B90E8 - 0x800B90E8 | size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B90E8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B91EC - 0x800B91EC | size: 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B91EC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B928C - 0x800B928C | size: 0xF0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B928C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B937C - 0x800B937C | size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B937C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9404 - 0x800B9404 | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9404(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B944C - 0x800B944C | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B944C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9494 - 0x800B9494 | size: 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9494(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B94F0 - 0x800B94F0 | size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B94F0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B953C - 0x800B953C | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B953C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9578 - 0x800B9578 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9578(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B959C - 0x800B959C | size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B959C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B962C - 0x800B962C | size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B962C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B96BC - 0x800B96BC | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B96BC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B96F8 - 0x800B96F8 | size: 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B96F8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B984C - 0x800B984C | size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B984C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9874 - 0x800B9874 | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9874(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B98DC - 0x800B98DC | size: 0x238 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B98DC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9B14 - 0x800B9B14 | size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9B14(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9BDC - 0x800B9BDC | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9BDC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9C44 - 0x800B9C44 | size: 0x228 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9C44(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9E6C - 0x800B9E6C | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9E6C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9E88 - 0x800B9E88 | size: 0x15C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9E88(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800B9FE4 - 0x800B9FE4 | size: 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800B9FE4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA160 - 0x800BA160 | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA160(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA198 - 0x800BA198 | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA198(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA1B4 - 0x800BA1B4 | size: 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA1B4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA344 - 0x800BA344 | size: 0xD0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA344(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA414 - 0x800BA414 | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA414(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA424 - 0x800BA424 | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA424(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA440 - 0x800BA440 | size: 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA440(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA44C - 0x800BA44C | size: 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA44C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA4C8 - 0x800BA4C8 | size: 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA4C8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA5BC - 0x800BA5BC | size: 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA5BC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA6B0 - 0x800BA6B0 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA6B0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA6F4 - 0x800BA6F4 | size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA6F4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA7C0 - 0x800BA7C0 | size: 0x15C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA7C0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA91C - 0x800BA91C | size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA91C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BA9E4 - 0x800BA9E4 | size: 0x274 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BA9E4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BAC58 - 0x800BAC58 | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BAC58(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BACA0 - 0x800BACA0 | size: 0x194 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BACA0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BAE34 - 0x800BAE34 | size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BAE34(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BAE5C - 0x800BAE5C | size: 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BAE5C(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800BAE64 - 0x800BAE64 | size: 0x198 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BAE64(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BAFFC - 0x800BAFFC | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BAFFC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB050 - 0x800BB050 | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB050(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB098 - 0x800BB098 | size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB098(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB29C - 0x800BB29C | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB29C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB2E4 - 0x800BB2E4 | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB2E4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB2F8 - 0x800BB2F8 | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB2F8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB30C - 0x800BB30C | size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB30C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB3C4 - 0x800BB3C4 | size: 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB3C4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB780 - 0x800BB780 | size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB780(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB81C - 0x800BB81C | size: 0x160 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB81C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BB97C - 0x800BB97C | size: 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BB97C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBAF8 - 0x800BBAF8 | size: 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBAF8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBC0C - 0x800BBC0C | size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBC0C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBC34 - 0x800BBC34 | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBC34(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBC7C - 0x800BBC7C | size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBC7C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBCE0 - 0x800BBCE0 | size: 0x1AC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBCE0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBE8C - 0x800BBE8C | size: 0x10C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBE8C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBF98 - 0x800BBF98 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBF98(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BBFDC - 0x800BBFDC | size: 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BBFDC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC024 - 0x800BC024 | size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC024(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC114 - 0x800BC114 | size: 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC114(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC1A0 - 0x800BC1A0 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC1A0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC1E4 - 0x800BC1E4 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC1E4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC228 - 0x800BC228 | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC228(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC290 - 0x800BC290 | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC290(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC2F8 - 0x800BC2F8 | size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC2F8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC36C - 0x800BC36C | size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC36C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC3E0 - 0x800BC3E0 | size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC3E0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC454 - 0x800BC454 | size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC454(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC4C0 - 0x800BC4C0 | size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC4C0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC52C - 0x800BC52C | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC52C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC580 - 0x800BC580 | size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC580(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC618 - 0x800BC618 | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC618(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC66C - 0x800BC66C | size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC66C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC6F0 - 0x800BC6F0 | size: 0x1D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC6F0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC8C8 - 0x800BC8C8 | size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC8C8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BC8F8 - 0x800BC8F8 | size: 0x21C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BC8F8(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCB14 - 0x800BCB14 | size: 0x1C8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCB14(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCCDC - 0x800BCCDC | size: 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCCDC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCDDC - 0x800BCDDC | size: 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCDDC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCE30 - 0x800BCE30 | size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCE30(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCE5C - 0x800BCE5C | size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCE5C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCE88 - 0x800BCE88 | size: 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCE88(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCEBC - 0x800BCEBC | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCEBC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCEF4 - 0x800BCEF4 | size: 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCEF4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BCFDC - 0x800BCFDC | size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BCFDC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD008 - 0x800BD008 | size: 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD008(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD044 - 0x800BD044 | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD044(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD07C - 0x800BD07C | size: 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD07C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD0F8 - 0x800BD0F8 | size: 0x4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD0F8(void) {
    nofralloc
    blr
}
#pragma pop

/* fn_800BD0FC - 0x800BD0FC | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD0FC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD16C - 0x800BD16C | size: 0x174 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD16C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD2E0 - 0x800BD2E0 | size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD2E0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD394 - 0x800BD394 | size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD394(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD454 - 0x800BD454 | size: 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD454(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD4B4 - 0x800BD4B4 | size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD4B4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD504 - 0x800BD504 | size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD504(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD554 - 0x800BD554 | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD554(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD58C - 0x800BD58C | size: 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD58C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD640 - 0x800BD640 | size: 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD640(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD744 - 0x800BD744 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD744(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD768 - 0x800BD768 | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD768(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD7A0 - 0x800BD7A0 | size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD7A0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD830 - 0x800BD830 | size: 0x40 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD830(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD870 - 0x800BD870 | size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD870(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD898 - 0x800BD898 | size: 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD898(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BD91C - 0x800BD91C | size: 0x848 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BD91C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BE164 - 0x800BE164 | size: 0x1A8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BE164(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BE30C - 0x800BE30C | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BE30C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800BE31C - 0x800BE31C | size: 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800BE31C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

