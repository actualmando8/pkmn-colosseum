/**
 * @file hsd_initialize.c
 * @brief HSD initialization and render pass management.
 *
 * Colosseum address: 0x8019C690 (HSD_InitAssert1)
 * The initialize module sets up the HSD subsystem: GX FIFO,
 * framebuffers, heaps, and pixel format validation.
 *
 * Adapted from doldecomp/melee src/sysdolphin/baselib/initialize.c
 */

#include "hsd/hsd_initialize.h"
#include "hsd/hsd_debug.h"

static HSD_RenderPass current_render_pass = HSD_RP_SCREEN;
static s32 current_heap = -1;

/* ========================================================================= */
/*  Heap management                                                          */
/* ========================================================================= */

s32 HSD_GetHeap(void)
{
    return current_heap;
}

void HSD_SetHeap(s32 handle)
{
    current_heap = handle;
}

/* ========================================================================= */
/*  Render pass                                                              */
/* ========================================================================= */

HSD_RenderPass HSD_GetCurrentRenderPass(void)
{
    return current_render_pass;
}

void HSD_StartRender(HSD_RenderPass pass)
{
    current_render_pass = pass;
}

/* ===================================================================
 * Generated: 0 pattern-matched + 10 stubs
 * Range: 0x8019C690 - 0x8019CE50
 * =================================================================== */

/* 0x8019C6EC | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047B290;
#if 0
asm void fn_8019C6EC(void) {
#include "src/hsd/hsd_initialize_fn_8019C6EC.inc"
}
#else
void fn_8019C6EC(u32 flags) {
    lbl_8047B290 |= flags;
}
#endif
#pragma pop

/* 0x8019C6FC | 0xC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047B294;
#if 1
asm void fn_8019C6FC(void) {
#include "src/hsd/hsd_initialize_fn_8019C6FC.inc"
}
#else
void fn_8019C6FC(void) {
    /* TODO: match -- 12 bytes at 0x8019C6FC */
}
#endif
#pragma pop

/* 0x8019C708 | 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800BCEF4(void);
extern void fn_800BD07C(void);
extern void fn_800B856C(void);
extern void fn_800BB29C(void);
extern u8 lbl_80466BC0[];
extern u32 lbl_8047B294;
extern u32 lbl_80478C78;
extern u32 lbl_8047B27C;
extern u32 lbl_8047B290;
#if 1
asm void fn_8019C708(void) {
#include "src/hsd/hsd_initialize_fn_8019C708.inc"
}
#else
void fn_8019C708(void) {
    /* TODO: match -- 168 bytes at 0x8019C708 */
}
#endif
#pragma pop

/* 0x8019C7B0 | 0x8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047B294;
#if 0
asm void fn_8019C7B0(void) {
#include "src/hsd/hsd_initialize_fn_8019C7B0.inc"
}
#else
u32 fn_8019C7B0(void) {
    return lbl_8047B294;
}
#endif
#pragma pop

/* 0x8019C7B8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u32 lbl_8047B268;
extern u32 lbl_8047B26C;
#if 0
asm void fn_8019C7B8(void) {
#include "src/hsd/hsd_initialize_fn_8019C7B8.inc"
}
#else
#pragma optimization_level 4
#pragma optimizewithasm on
s32 fn_8019C7B8(u32 addr) {
    if (lbl_8047B268 > addr) {
        return 0;
    }
    if (addr >= lbl_8047B26C) {
        return 0;
    }
    return 1;
}
#endif
#pragma pop

/* 0x8019C7E0 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8009AC3C(void);
extern void fn_8009ABD0(void);
extern void fn_8009AB50(void);
extern u32 lbl_80478C70;
extern u32 lbl_8047B270;
extern u32 lbl_8047B274;
extern u32 lbl_8047B268;
extern u32 lbl_8047B26C;
#if 1
asm void fn_8019C7E0(void) {
#include "src/hsd/hsd_initialize_fn_8019C7E0.inc"
}
#else
void fn_8019C7E0(void) {
    /* TODO: match -- 116 bytes at 0x8019C7E0 */
}
#endif
#pragma pop

/* 0x8019C854 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_80196E10(void);
extern void fn_8009AC50(void);
extern u32 lbl_8047B288;
extern u8 lbl_802749E4[];
extern u8 lbl_80274A28[];
extern u32 lbl_80478C70;
#if 1
asm void fn_8019C854(void) {
#include "src/hsd/hsd_initialize_fn_8019C854.inc"
}
#else
void fn_8019C854(void) {
    /* TODO: match -- 72 bytes at 0x8019C854 */
}
#endif
#pragma pop

/* 0x8019C89C | 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8009AAD4(void);
extern u32 lbl_8047B288;
extern u32 lbl_80478C70;
#if 1
asm void fn_8019C89C(void) {
#include "src/hsd/hsd_initialize_fn_8019C89C.inc"
}
#else
void fn_8019C89C(void) {
    /* TODO: match -- 88 bytes at 0x8019C89C */
}
#endif
#pragma pop

/* 0x8019C8F4 | 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8009A9D8(void);
extern u32 lbl_8047B288;
extern u32 lbl_80478C70;
extern u8 lbl_8047DB0C[];
#if 1
asm void fn_8019C8F4(void) {
#include "src/hsd/hsd_initialize_fn_8019C8F4.inc"
}
#else
void fn_8019C8F4(void) {
    /* TODO: match -- 132 bytes at 0x8019C8F4 */
}
#endif
#pragma pop

/* 0x8019C978 | 0x1F8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_8009F3D4(void);
extern void fn_801A69C0(void);
extern void fn_801AA538(void);
extern void OSGetArenaLo();
extern void OSGetArenaHi();
extern void OSSetArenaLo();
extern u8 lbl_80465568[];
extern u32 lbl_8047B288;
extern u32 lbl_8047B270;
extern u32 lbl_8047B274;
extern u32 lbl_8047B268;
extern u32 lbl_8047B26C;
extern u32 lbl_80478C70;
extern u32 lbl_8047B284;
#if 1
asm void fn_8019C978(void) {
#include "src/hsd/hsd_initialize_fn_8019C978.inc"
}
#else
void fn_8019C978(void) {
    /* TODO: match -- 504 bytes at 0x8019C978 */
}
#endif
#pragma pop
