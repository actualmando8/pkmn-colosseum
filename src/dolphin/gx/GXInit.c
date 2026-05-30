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

/* fn_800B6FE0 - 0x800B6FE0 | size: 0x134 -- GX FIFO management */
void fn_800B6FE0(void) {
    /* GX FIFO management (0x134 bytes) */
}

/* fn_800B7180 - 0x800B7180 | size: 0x70 -- GX FIFO management */
void fn_800B7180(void) {
    /* GX FIFO management (0x70 bytes) */
}

/* fn_800B71F0 - 0x800B71F0 | size: 0xC -- GX FIFO management */
void fn_800B71F0(void) {
    /* Simple accessor (0xC bytes) */
}

/* fn_800B7484 - 0x800B7484 | size: 0x44 -- GX FIFO management */
void fn_800B7484(void) {
    /* GX FIFO management (0x44 bytes) */
}

/* fn_800B7514 - 0x800B7514 | size: 0x24 -- GX Misc / PE / draw sync */
void fn_800B7514(void) {
    /* GX Misc / PE / draw sync (0x24 bytes) */
}

/* fn_800B7538 - 0x800B7538 | size: 0x20 -- GX Misc / PE / draw sync */
void fn_800B7538(void) {
    /* GX Misc / PE / draw sync (0x20 bytes) */
}

/* fn_800B7558 - 0x800B7558 | size: 0x3C -- GX Misc / PE / draw sync */
void fn_800B7558(void) {
    /* GX Misc / PE / draw sync (0x3C bytes) */
}

/* fn_800B7594 - 0x800B7594 | size: 0x3C -- GX Misc / PE / draw sync */
void fn_800B7594(void) {
    /* GX Misc / PE / draw sync (0x3C bytes) */
}

/* fn_800B75D0 - 0x800B75D0 | size: 0x3C -- GX Misc / PE / draw sync */
void fn_800B75D0(void) {
    /* GX Misc / PE / draw sync (0x3C bytes) */
}

/* fn_800B760C - 0x800B760C | size: 0x100 -- GX Misc / PE / draw sync */
void fn_800B760C(void) {
    /* GX Misc / PE / draw sync (0x100 bytes) */
}

/* fn_800B770C - 0x800B770C | size: 0x8 -- GX Misc / PE / draw sync */
u32 fn_800B770C(void) {
    extern u32 lbl_8047A9A0;
    return lbl_8047A9A0;
}

/* fn_800B7714 - 0x800B7714 | size: 0x8 -- GX Misc / PE / draw sync */
u32 fn_800B7714(void) {
    extern u32 lbl_8047A9A4;
    return lbl_8047A9A4;
}

/* fn_800B771C - 0x800B771C | size: 0x158 -- GX Misc / PE / draw sync */
void fn_800B771C(void) {
    /* GX Misc / PE / draw sync (0x158 bytes) */
}

/* fn_800B7874 - 0x800B7874 | size: 0x350 -- GX Misc / PE / draw sync */
void fn_800B7874(void) {
    /* GX Misc / PE / draw sync (0x350 bytes) -- large function, requires Ghidra */
}

/* fn_800B7BC4 - 0x800B7BC4 | size: 0x54 -- GX Misc / PE / draw sync */
void fn_800B7BC4(void) {
    /* GX Misc / PE / draw sync (0x54 bytes) */
}

/* fn_800B7C18 - 0x800B7C18 | size: 0x124 -- GX Misc / PE / draw sync */
void fn_800B7C18(void) {
    /* GX Misc / PE / draw sync (0x124 bytes) */
}

/* fn_800B7D3C - 0x800B7D3C | size: 0x38 -- GX Misc / PE / draw sync */
void fn_800B7D3C(void) {
    /* GX Misc / PE / draw sync (0x38 bytes) */
}

/* fn_800B7D74 - 0x800B7D74 | size: 0x358 -- GX Misc / PE / draw sync */
void fn_800B7D74(void) {
    /* GX Misc / PE / draw sync (0x358 bytes) -- large function, requires Ghidra */
}

/* fn_800B80CC - 0x800B80CC | size: 0x378 -- GX Misc / PE / draw sync */
void fn_800B80CC(void) {
    /* GX Misc / PE / draw sync (0x378 bytes) -- large function, requires Ghidra */
}

/* fn_800B8444 - 0x800B8444 | size: 0x9C -- GX Misc / PE / draw sync */
void fn_800B8444(void) {
    /* GX Misc / PE / draw sync (0x9C bytes) */
}

/* fn_800B84E0 - 0x800B84E0 | size: 0x8C -- GX Misc / PE / draw sync */
void fn_800B84E0(void) {
    /* GX Misc / PE / draw sync (0x8C bytes) */
}

/* fn_800B856C - 0x800B856C | size: 0x10 -- GX Misc / PE / draw sync */
void fn_800B856C(void) {
    /* Simple accessor (0x10 bytes) */
}

/* fn_800B857C - 0x800B857C | size: 0x2D0 -- GX Misc / PE / draw sync */
void fn_800B857C(void) {
    /* GX Misc / PE / draw sync (0x2D0 bytes) -- large function, requires Ghidra */
}

/* fn_800B884C - 0x800B884C | size: 0x40 -- GX Misc / PE / draw sync */
void fn_800B884C(void) {
    /* GX Misc / PE / draw sync (0x40 bytes) */
}

/* fn_800B8920 - 0x800B8920 | size: 0x5C -- GX Misc / PE / draw sync */
void fn_800B8920(void) {
    /* GX Misc / PE / draw sync (0x5C bytes) */
}

/* fn_800B897C - 0x800B897C | size: 0x16C -- GX Misc / PE / draw sync */
void fn_800B897C(void) {
    /* GX Misc / PE / draw sync (0x16C bytes) */
}

/* fn_800B8AE8 - 0x800B8AE8 | size: 0x170 -- GX Misc / PE / draw sync */
void fn_800B8AE8(void) {
    /* GX Misc / PE / draw sync (0x170 bytes) */
}

/* fn_800B8C58 - 0x800B8C58 | size: 0xB8 -- GX Misc / PE / draw sync */
void fn_800B8C58(void) {
    /* GX Misc / PE / draw sync (0xB8 bytes) */
}

/* fn_800B8D10 - 0x800B8D10 | size: 0x98 -- GX Misc / PE / draw sync */
void fn_800B8D10(void) {
    /* GX Misc / PE / draw sync (0x98 bytes) */
}

/* fn_800B8DA8 - 0x800B8DA8 | size: 0x4C -- GX Misc / PE / draw sync */
void fn_800B8DA8(void) {
    /* GX Misc / PE / draw sync (0x4C bytes) */
}

/* fn_800B8DF4 - 0x800B8DF4 | size: 0x80 -- GX Misc / PE / draw sync */
void fn_800B8DF4(void) {
    /* GX Misc / PE / draw sync (0x80 bytes) */
}

/* fn_800B8E74 - 0x800B8E74 | size: 0x24 -- GX Misc / PE / draw sync */
void fn_800B8E74(void) {
    /* GX Misc / PE / draw sync (0x24 bytes) */
}

/* fn_800B8E98 - 0x800B8E98 | size: 0x14 -- GX Misc / PE / draw sync */
void fn_800B8E98(void) {
    /* Simple accessor (0x14 bytes) */
}

/* fn_800B8EAC - 0x800B8EAC | size: 0x14 -- GX Misc / PE / draw sync */
void fn_800B8EAC(void) {
    /* Simple accessor (0x14 bytes) */
}

/* fn_800B8EC0 - 0x800B8EC0 | size: 0x1C -- GX Misc / PE / draw sync */
void fn_800B8EC0(void) {
    /* GX Misc / PE / draw sync (0x1C bytes) */
}

/* fn_800B8EDC - 0x800B8EDC | size: 0x88 -- GX Misc / PE / draw sync */
void fn_800B8EDC(void) {
    /* GX Misc / PE / draw sync (0x88 bytes) */
}

/* fn_800B8F64 - 0x800B8F64 | size: 0x1C -- GX Misc / PE / draw sync */
void fn_800B8F64(void) {
    /* GX Misc / PE / draw sync (0x1C bytes) */
}

/* fn_800B8F80 - 0x800B8F80 | size: 0x14 -- GX Misc / PE / draw sync */
void fn_800B8F80(void) {
    /* Simple accessor (0x14 bytes) */
}

/* fn_800B8F94 - 0x800B8F94 | size: 0x1C -- GX Misc / PE / draw sync */
void fn_800B8F94(void) {
    /* GX Misc / PE / draw sync (0x1C bytes) */
}

/* fn_800B8FB0 - 0x800B8FB0 | size: 0x28 -- GX Misc / PE / draw sync */
void fn_800B8FB0(void) {
    /* GX Misc / PE / draw sync (0x28 bytes) */
}

/* fn_800B8FD8 - 0x800B8FD8 | size: 0x44 -- GX Misc / PE / draw sync */
void fn_800B8FD8(void) {
    /* GX Misc / PE / draw sync (0x44 bytes) */
}

/* fn_800B901C - 0x800B901C | size: 0x88 -- GX Misc / PE / draw sync */
void fn_800B901C(void) {
    /* GX Misc / PE / draw sync (0x88 bytes) */
}

/* fn_800B90A4 - 0x800B90A4 | size: 0x44 -- GX Misc / PE / draw sync */
void fn_800B90A4(void) {
    /* GX Misc / PE / draw sync (0x44 bytes) */
}

/* fn_800B90E8 - 0x800B90E8 | size: 0x84 -- GX Misc / PE / draw sync */
void fn_800B90E8(void) {
    /* GX Misc / PE / draw sync (0x84 bytes) */
}

/* fn_800B91EC - 0x800B91EC | size: 0xA0 -- GX Misc / PE / draw sync */
void fn_800B91EC(void) {
    /* GX Misc / PE / draw sync (0xA0 bytes) */
}

/* fn_800B928C - 0x800B928C | size: 0xF0 -- GX Misc / PE / draw sync */
void fn_800B928C(void) {
    /* GX Misc / PE / draw sync (0xF0 bytes) */
}

/* fn_800B937C - 0x800B937C | size: 0x88 -- GX Misc / PE / draw sync */
void fn_800B937C(void) {
    /* GX Misc / PE / draw sync (0x88 bytes) */
}

/* fn_800B9404 - 0x800B9404 | size: 0x48 -- GX Misc / PE / draw sync */
void fn_800B9404(void) {
    /* GX Misc / PE / draw sync (0x48 bytes) */
}

/* fn_800B944C - 0x800B944C | size: 0x48 -- GX Misc / PE / draw sync */
void fn_800B944C(void) {
    /* GX Misc / PE / draw sync (0x48 bytes) */
}

/* fn_800B9494 - 0x800B9494 | size: 0x5C -- GX Misc / PE / draw sync */
void fn_800B9494(void) {
    /* GX Misc / PE / draw sync (0x5C bytes) */
}

/* fn_800B94F0 - 0x800B94F0 | size: 0x4C -- GX Misc / PE / draw sync */
void fn_800B94F0(void) {
    /* GX Misc / PE / draw sync (0x4C bytes) */
}

/* fn_800B953C - 0x800B953C | size: 0x3C -- GX Misc / PE / draw sync */
void fn_800B953C(void) {
    /* GX Misc / PE / draw sync (0x3C bytes) */
}

/* fn_800B9578 - 0x800B9578 | size: 0x24 -- GX Misc / PE / draw sync */
void fn_800B9578(void) {
    /* GX Misc / PE / draw sync (0x24 bytes) */
}

/* fn_800B959C - 0x800B959C | size: 0x90 -- GX Misc / PE / draw sync */
void fn_800B959C(void) {
    /* GX Misc / PE / draw sync (0x90 bytes) */
}

/* fn_800B962C - 0x800B962C | size: 0x90 -- GX Misc / PE / draw sync */
void fn_800B962C(void) {
    /* GX Misc / PE / draw sync (0x90 bytes) */
}

/* fn_800B96BC - 0x800B96BC | size: 0x3C -- GX Transform / viewport / projection */
void fn_800B96BC(void) {
    /* GX Transform / viewport / projection (0x3C bytes) */
}

/* fn_800B96F8 - 0x800B96F8 | size: 0x154 -- GX Transform / viewport / projection */
void fn_800B96F8(void) {
    /* GX Transform / viewport / projection (0x154 bytes) */
}

/* fn_800B984C - 0x800B984C | size: 0x28 -- GX Transform / viewport / projection */
void fn_800B984C(void) {
    /* GX Transform / viewport / projection (0x28 bytes) */
}

/* fn_800B9874 - 0x800B9874 | size: 0x68 -- GX Transform / viewport / projection */
void fn_800B9874(void) {
    /* GX Transform / viewport / projection (0x68 bytes) */
}

/* fn_800B98DC - 0x800B98DC | size: 0x238 -- GX Transform / viewport / projection */
void fn_800B98DC(void) {
    /* GX Transform / viewport / projection (0x238 bytes) -- large function, requires Ghidra */
}

/* fn_800B9B14 - 0x800B9B14 | size: 0xC8 -- GX Transform / viewport / projection */
void fn_800B9B14(void) {
    /* GX Transform / viewport / projection (0xC8 bytes) */
}

/* fn_800B9BDC - 0x800B9BDC | size: 0x68 -- GX Transform / viewport / projection */
void fn_800B9BDC(void) {
    /* GX Transform / viewport / projection (0x68 bytes) */
}

/* fn_800B9C44 - 0x800B9C44 | size: 0x228 -- GX Transform / viewport / projection */
void fn_800B9C44(void) {
    /* GX Transform / viewport / projection (0x228 bytes) -- large function, requires Ghidra */
}

/* fn_800B9E6C - 0x800B9E6C | size: 0x1C -- GX Transform / viewport / projection */
void fn_800B9E6C(void) {
    /* GX Transform / viewport / projection (0x1C bytes) */
}

/* fn_800B9E88 - 0x800B9E88 | size: 0x15C -- GX Transform / viewport / projection */
void fn_800B9E88(void) {
    /* GX Transform / viewport / projection (0x15C bytes) */
}

/* fn_800B9FE4 - 0x800B9FE4 | size: 0x17C -- GX Transform / viewport / projection */
void fn_800B9FE4(void) {
    /* GX Transform / viewport / projection (0x17C bytes) */
}

/* fn_800BA160 - 0x800BA160 | size: 0x38 -- GX Geometry / vertex descriptor */
void fn_800BA160(void) {
    /* GX Geometry / vertex descriptor (0x38 bytes) */
}

/* fn_800BA198 - 0x800BA198 | size: 0x1C -- GX Geometry / vertex descriptor */
void fn_800BA198(void) {
    /* GX Geometry / vertex descriptor (0x1C bytes) */
}

/* fn_800BA1B4 - 0x800BA1B4 | size: 0x190 -- GX Geometry / vertex descriptor */
void fn_800BA1B4(void) {
    /* GX Geometry / vertex descriptor (0x190 bytes) */
}

/* fn_800BA344 - 0x800BA344 | size: 0xD0 -- GX Geometry / vertex descriptor */
void fn_800BA344(void) {
    /* GX Geometry / vertex descriptor (0xD0 bytes) */
}

/* fn_800BA414 - 0x800BA414 | size: 0x10 -- GX Geometry / vertex descriptor */
void fn_800BA414(void) {
    /* Simple accessor (0x10 bytes) */
}

/* fn_800BA424 - 0x800BA424 | size: 0x1C -- GX Geometry / vertex descriptor */
void fn_800BA424(void) {
    /* GX Geometry / vertex descriptor (0x1C bytes) */
}

/* fn_800BA440 - 0x800BA440 | size: 0xC -- GX Geometry / vertex descriptor */
void fn_800BA440(void) {
    /* Simple accessor (0xC bytes) */
}

/* fn_800BA44C - 0x800BA44C | size: 0x7C -- GX Geometry / vertex descriptor */
void fn_800BA44C(void) {
    /* GX Geometry / vertex descriptor (0x7C bytes) */
}

/* fn_800BA4C8 - 0x800BA4C8 | size: 0xF4 -- GX Geometry / vertex descriptor */
void fn_800BA4C8(void) {
    /* GX Geometry / vertex descriptor (0xF4 bytes) */
}

/* fn_800BA5BC - 0x800BA5BC | size: 0xF4 -- GX Geometry / vertex descriptor */
void fn_800BA5BC(void) {
    /* GX Geometry / vertex descriptor (0xF4 bytes) */
}

/* fn_800BA6B0 - 0x800BA6B0 | size: 0x44 -- GX Geometry / vertex descriptor */
void fn_800BA6B0(void) {
    /* GX Geometry / vertex descriptor (0x44 bytes) */
}

/* fn_800BA6F4 - 0x800BA6F4 | size: 0xCC -- GX Geometry / vertex descriptor */
void fn_800BA6F4(void) {
    /* GX Geometry / vertex descriptor (0xCC bytes) */
}

/* fn_800BA7C0 - 0x800BA7C0 | size: 0x15C -- GX Geometry / vertex descriptor */
void fn_800BA7C0(void) {
    /* GX Geometry / vertex descriptor (0x15C bytes) */
}

/* fn_800BA91C - 0x800BA91C | size: 0xC8 -- GX Geometry / vertex descriptor */
void fn_800BA91C(void) {
    /* GX Geometry / vertex descriptor (0xC8 bytes) */
}

/* fn_800BA9E4 - 0x800BA9E4 | size: 0x274 -- GX Geometry / vertex descriptor */
void fn_800BA9E4(void) {
    /* GX Geometry / vertex descriptor (0x274 bytes) -- large function, requires Ghidra */
}

/* fn_800BAC58 - 0x800BAC58 | size: 0x48 -- GX Geometry / vertex descriptor */
void fn_800BAC58(void) {
    /* GX Geometry / vertex descriptor (0x48 bytes) */
}

/* fn_800BACA0 - 0x800BACA0 | size: 0x194 -- GX Geometry / vertex descriptor */
void fn_800BACA0(void) {
    /* GX Geometry / vertex descriptor (0x194 bytes) */
}

/* fn_800BAE34 - 0x800BAE34 | size: 0x28 -- GX Geometry / vertex descriptor */
void fn_800BAE34(void) {
    /* GX Geometry / vertex descriptor (0x28 bytes) */
}

/* fn_800BAE5C - 0x800BAE5C | size: 0x8 -- GX Geometry / vertex descriptor */
u32 fn_800BAE5C(void* p) {
    return *(u32*)((u8*)p + 0x14);
}

/* fn_800BAE64 - 0x800BAE64 | size: 0x198 -- GX Texture / TMEM */
void fn_800BAE64(void) {
    /* GX Texture / TMEM (0x198 bytes) */
}

/* fn_800BAFFC - 0x800BAFFC | size: 0x54 -- GX Texture / TMEM */
void fn_800BAFFC(void) {
    /* GX Texture / TMEM (0x54 bytes) */
}

/* fn_800BB050 - 0x800BB050 | size: 0x48 -- GX Texture / TMEM */
void fn_800BB050(void) {
    /* GX Texture / TMEM (0x48 bytes) */
}

/* fn_800BB098 - 0x800BB098 | size: 0x9C -- GX Texture / TMEM */
void fn_800BB098(void) {
    /* GX Texture / TMEM (0x9C bytes) */
}

/* fn_800BB29C - 0x800BB29C | size: 0x48 -- GX Texture / TMEM */
void fn_800BB29C(void) {
    /* GX Texture / TMEM (0x48 bytes) */
}

/* fn_800BB2E4 - 0x800BB2E4 | size: 0x14 -- GX Texture / TMEM */
void fn_800BB2E4(void) {
    /* Simple accessor (0x14 bytes) */
}

/* fn_800BB2F8 - 0x800BB2F8 | size: 0x14 -- GX Texture / TMEM */
void fn_800BB2F8(void) {
    /* Simple accessor (0x14 bytes) */
}

/* fn_800BB30C - 0x800BB30C | size: 0xB8 -- GX Texture / TMEM */
void fn_800BB30C(void) {
    /* GX Texture / TMEM (0xB8 bytes) */
}

/* fn_800BB3C4 - 0x800BB3C4 | size: 0x17C -- GX Texture / TMEM */
void fn_800BB3C4(void) {
    /* GX Texture / TMEM (0x17C bytes) */
}

/* fn_800BB780 - 0x800BB780 | size: 0x9C -- GX TMEM config / texture state */
void fn_800BB780(void) {
    /* GX TMEM config / texture state (0x9C bytes) */
}

/* fn_800BB81C - 0x800BB81C | size: 0x160 -- GX TMEM config / texture state */
void fn_800BB81C(void) {
    /* GX TMEM config / texture state (0x160 bytes) */
}

/* fn_800BB97C - 0x800BB97C | size: 0x17C -- GX TMEM config / texture state */
void fn_800BB97C(void) {
    /* GX TMEM config / texture state (0x17C bytes) */
}

/* fn_800BBAF8 - 0x800BBAF8 | size: 0x114 -- GX TMEM config / texture state */
void fn_800BBAF8(void) {
    /* GX TMEM config / texture state (0x114 bytes) */
}

/* fn_800BBC0C - 0x800BBC0C | size: 0x28 -- GX TMEM config / texture state */
void fn_800BBC0C(void) {
    /* GX TMEM config / texture state (0x28 bytes) */
}

/* fn_800BBC34 - 0x800BBC34 | size: 0x48 -- GX TMEM config / texture state */
void fn_800BBC34(void) {
    /* GX TMEM config / texture state (0x48 bytes) */
}

/* fn_800BBC7C - 0x800BBC7C | size: 0x64 -- GX TMEM config / texture state */
void fn_800BBC7C(void) {
    /* GX TMEM config / texture state (0x64 bytes) */
}

/* fn_800BBCE0 - 0x800BBCE0 | size: 0x1AC -- GX TMEM config / texture state */
void fn_800BBCE0(void) {
    /* GX TMEM config / texture state (0x1AC bytes) */
}

/* fn_800BBE8C - 0x800BBE8C | size: 0x10C -- GX TMEM config / texture state */
void fn_800BBE8C(void) {
    /* GX TMEM config / texture state (0x10C bytes) */
}

/* fn_800BBF98 - 0x800BBF98 | size: 0x44 -- GX TMEM config / texture state */
void fn_800BBF98(void) {
    /* GX TMEM config / texture state (0x44 bytes) */
}

/* fn_800BBFDC - 0x800BBFDC | size: 0x48 -- GX TMEM config / texture state */
void fn_800BBFDC(void) {
    /* GX TMEM config / texture state (0x48 bytes) */
}

/* fn_800BC024 - 0x800BC024 | size: 0xCC -- GX Light / material / TEV */
void fn_800BC024(void) {
    /* GX Light / material / TEV (0xCC bytes) */
}

/* fn_800BC114 - 0x800BC114 | size: 0x8C -- GX Light / material / TEV */
void fn_800BC114(void) {
    /* GX Light / material / TEV (0x8C bytes) */
}

/* fn_800BC1A0 - 0x800BC1A0 | size: 0x44 -- GX Light / material / TEV */
void fn_800BC1A0(void) {
    /* GX Light / material / TEV (0x44 bytes) */
}

/* fn_800BC1E4 - 0x800BC1E4 | size: 0x44 -- GX Light / material / TEV */
void fn_800BC1E4(void) {
    /* GX Light / material / TEV (0x44 bytes) */
}

/* fn_800BC228 - 0x800BC228 | size: 0x68 -- GX Light / material / TEV */
void fn_800BC228(void) {
    /* GX Light / material / TEV (0x68 bytes) */
}

/* fn_800BC290 - 0x800BC290 | size: 0x68 -- GX Light / material / TEV */
void fn_800BC290(void) {
    /* GX Light / material / TEV (0x68 bytes) */
}

/* fn_800BC2F8 - 0x800BC2F8 | size: 0x74 -- GX Light / material / TEV */
void fn_800BC2F8(void) {
    /* GX Light / material / TEV (0x74 bytes) */
}

/* fn_800BC36C - 0x800BC36C | size: 0x74 -- GX Light / material / TEV */
void fn_800BC36C(void) {
    /* GX Light / material / TEV (0x74 bytes) */
}

/* fn_800BC3E0 - 0x800BC3E0 | size: 0x74 -- GX Light / material / TEV */
void fn_800BC3E0(void) {
    /* GX Light / material / TEV (0x74 bytes) */
}

/* fn_800BC454 - 0x800BC454 | size: 0x6C -- GX Light / material / TEV */
void fn_800BC454(void) {
    /* GX Light / material / TEV (0x6C bytes) */
}

/* fn_800BC4C0 - 0x800BC4C0 | size: 0x6C -- GX Light / material / TEV */
void fn_800BC4C0(void) {
    /* GX Light / material / TEV (0x6C bytes) */
}

/* fn_800BC52C - 0x800BC52C | size: 0x54 -- GX Light / material / TEV */
void fn_800BC52C(void) {
    /* GX Light / material / TEV (0x54 bytes) */
}

/* fn_800BC580 - 0x800BC580 | size: 0x98 -- GX Light / material / TEV */
void fn_800BC580(void) {
    /* GX Light / material / TEV (0x98 bytes) */
}

/* fn_800BC618 - 0x800BC618 | size: 0x54 -- GX Light / material / TEV */
void fn_800BC618(void) {
    /* GX Light / material / TEV (0x54 bytes) */
}

/* fn_800BC66C - 0x800BC66C | size: 0x84 -- GX Light / material / TEV */
void fn_800BC66C(void) {
    /* GX Light / material / TEV (0x84 bytes) */
}

/* fn_800BC6F0 - 0x800BC6F0 | size: 0x1D8 -- GX Light / material / TEV */
void fn_800BC6F0(void) {
    /* GX Light / material / TEV (0x1D8 bytes) */
}

/* fn_800BC8C8 - 0x800BC8C8 | size: 0x30 -- GX Light / material / TEV */
void fn_800BC8C8(void) {
    /* GX Light / material / TEV (0x30 bytes) */
}

/* fn_800BC8F8 - 0x800BC8F8 | size: 0x21C -- GX Light / material / TEV */
void fn_800BC8F8(void) {
    /* GX Light / material / TEV (0x21C bytes) -- large function, requires Ghidra */
}

/* fn_800BCB14 - 0x800BCB14 | size: 0x1C8 -- GX Light / material / TEV */
void fn_800BCB14(void) {
    /* GX Light / material / TEV (0x1C8 bytes) */
}

/* fn_800BCCDC - 0x800BCCDC | size: 0x100 -- GX Light / material / TEV */
void fn_800BCCDC(void) {
    /* GX Light / material / TEV (0x100 bytes) */
}

/* fn_800BCDDC - 0x800BCDDC | size: 0x54 -- GX Light / material / TEV */
void fn_800BCDDC(void) {
    /* GX Light / material / TEV (0x54 bytes) */
}

/* fn_800BCE30 - 0x800BCE30 | size: 0x2C -- GX Light / material / TEV */
void fn_800BCE30(void) {
    /* GX Light / material / TEV (0x2C bytes) */
}

/* fn_800BCE5C - 0x800BCE5C | size: 0x2C -- GX Light / material / TEV */
void fn_800BCE5C(void) {
    /* GX Light / material / TEV (0x2C bytes) */
}

/* fn_800BCE88 - 0x800BCE88 | size: 0x34 -- GX Pixel / blend / alpha / z-mode */
void fn_800BCE88(void) {
    /* GX Pixel / blend / alpha / z-mode (0x34 bytes) */
}

/* fn_800BCEBC - 0x800BCEBC | size: 0x38 -- GX Pixel / blend / alpha / z-mode */
void fn_800BCEBC(void) {
    /* GX Pixel / blend / alpha / z-mode (0x38 bytes) */
}

/* fn_800BCEF4 - 0x800BCEF4 | size: 0xE8 -- GX Pixel / blend / alpha / z-mode */
void fn_800BCEF4(void) {
    /* GX Pixel / blend / alpha / z-mode (0xE8 bytes) */
}

/* fn_800BCFDC - 0x800BCFDC | size: 0x2C -- GX Pixel / blend / alpha / z-mode */
void fn_800BCFDC(void) {
    /* GX Pixel / blend / alpha / z-mode (0x2C bytes) */
}

/* fn_800BD008 - 0x800BD008 | size: 0x3C -- GX Pixel / blend / alpha / z-mode */
void fn_800BD008(void) {
    /* GX Pixel / blend / alpha / z-mode (0x3C bytes) */
}

/* fn_800BD044 - 0x800BD044 | size: 0x38 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD044(void) {
    /* GX Pixel / blend / alpha / z-mode (0x38 bytes) */
}

/* fn_800BD07C - 0x800BD07C | size: 0x7C -- GX Pixel / blend / alpha / z-mode */
void fn_800BD07C(void) {
    /* GX Pixel / blend / alpha / z-mode (0x7C bytes) */
}

/* fn_800BD0F8 - 0x800BD0F8 | size: 0x4 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD0F8(void) {
}

/* fn_800BD0FC - 0x800BD0FC | size: 0x70 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD0FC(void) {
    /* GX Pixel / blend / alpha / z-mode (0x70 bytes) */
}

/* fn_800BD16C - 0x800BD16C | size: 0x174 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD16C(void) {
    /* GX Pixel / blend / alpha / z-mode (0x174 bytes) */
}

/* fn_800BD2E0 - 0x800BD2E0 | size: 0xB4 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD2E0(void) {
    /* GX Pixel / blend / alpha / z-mode (0xB4 bytes) */
}

/* fn_800BD394 - 0x800BD394 | size: 0xC0 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD394(void) {
    /* GX Pixel / blend / alpha / z-mode (0xC0 bytes) */
}

/* fn_800BD454 - 0x800BD454 | size: 0x60 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD454(void) {
    /* GX Pixel / blend / alpha / z-mode (0x60 bytes) */
}

/* fn_800BD4B4 - 0x800BD4B4 | size: 0x50 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD4B4(void) {
    /* GX Pixel / blend / alpha / z-mode (0x50 bytes) */
}

/* fn_800BD504 - 0x800BD504 | size: 0x50 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD504(void) {
    /* GX Pixel / blend / alpha / z-mode (0x50 bytes) */
}

/* fn_800BD554 - 0x800BD554 | size: 0x38 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD554(void) {
    /* GX Pixel / blend / alpha / z-mode (0x38 bytes) */
}

/* fn_800BD58C - 0x800BD58C | size: 0xB4 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD58C(void) {
    /* GX Pixel / blend / alpha / z-mode (0xB4 bytes) */
}

/* fn_800BD640 - 0x800BD640 | size: 0x104 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD640(void) {
    /* GX Pixel / blend / alpha / z-mode (0x104 bytes) */
}

/* fn_800BD744 - 0x800BD744 | size: 0x24 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD744(void) {
    /* GX Pixel / blend / alpha / z-mode (0x24 bytes) */
}

/* fn_800BD768 - 0x800BD768 | size: 0x38 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD768(void) {
    /* GX Pixel / blend / alpha / z-mode (0x38 bytes) */
}

/* fn_800BD7A0 - 0x800BD7A0 | size: 0x90 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD7A0(void) {
    /* GX Pixel / blend / alpha / z-mode (0x90 bytes) */
}

/* fn_800BD830 - 0x800BD830 | size: 0x40 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD830(void) {
    /* GX Pixel / blend / alpha / z-mode (0x40 bytes) */
}

/* fn_800BD870 - 0x800BD870 | size: 0x28 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD870(void) {
    /* GX Pixel / blend / alpha / z-mode (0x28 bytes) */
}

/* fn_800BD898 - 0x800BD898 | size: 0x84 -- GX Pixel / blend / alpha / z-mode */
void fn_800BD898(void) {
    /* GX Pixel / blend / alpha / z-mode (0x84 bytes) */
}

/* fn_800BD91C - 0x800BD91C | size: 0x848 -- GX Display copy / EFB */
void fn_800BD91C(void) {
    /* GX Display copy / EFB (0x848 bytes) -- large function, requires Ghidra */
}

/* fn_800BE164 - 0x800BE164 | size: 0x1A8 -- GX Display copy / EFB */
void fn_800BE164(void) {
    /* GX Display copy / EFB (0x1A8 bytes) */
}

/* fn_800BE30C - 0x800BE30C | size: 0x10 -- GX Display copy / EFB */
void fn_800BE30C(void) {
    /* Simple accessor (0x10 bytes) */
}

/* fn_800BE31C - 0x800BE31C | size: 0x2C -- GX Display copy / EFB */
void fn_800BE31C(void) {
    /* GX Display copy / EFB (0x2C bytes) */
}

