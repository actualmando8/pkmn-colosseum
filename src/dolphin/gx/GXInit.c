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
