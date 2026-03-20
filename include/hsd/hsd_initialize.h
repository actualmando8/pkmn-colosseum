/**
 * @file hsd_initialize.h
 * @brief HSD initialization and render pass management.
 *
 * Colosseum address range: 0x8019C690 (HSD_InitAssert1)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_INITIALIZE_H
#define HSD_INITIALIZE_H

#include "dolphin/types.h"

/* ========================================================================= */
/*  Constants                                                                */
/* ========================================================================= */

#define HSD_DEFAULT_FIFO_SIZE   (256 * 1024)
#define HSD_DEFAULT_XFB_MAX_NUM 2
#define HSD_DEFAULT_AUDIO_SIZE  (512 * 1024)

/* ========================================================================= */
/*  Render pass type                                                         */
/* ========================================================================= */

typedef enum _HSD_RenderPass {
    HSD_RP_SCREEN = 0,
    HSD_RP_TOPHALF,
    HSD_RP_BOTTOMHALF,
    HSD_RP_OFFSCREEN,
} HSD_RenderPass;

/* ========================================================================= */
/*  Init parameter types                                                     */
/* ========================================================================= */

typedef enum _HSD_InitParam {
    HSD_INIT_FIFO_SIZE,
    HSD_INIT_XFB_MAX_NUM,
    HSD_INIT_HEAP_MAX_NUM,
    HSD_INIT_AUDIO_HEAP_SIZE,
    HSD_INIT_RENDER_MODE_OBJ
} HSD_InitParam;

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_InitComponent(void);
s32 HSD_GetHeap(void);
void HSD_SetHeap(s32 handle);
HSD_RenderPass HSD_GetCurrentRenderPass(void);
void HSD_StartRender(HSD_RenderPass pass);
BOOL HSD_SetInitParameter(HSD_InitParam param, ...);

#endif /* HSD_INITIALIZE_H */
