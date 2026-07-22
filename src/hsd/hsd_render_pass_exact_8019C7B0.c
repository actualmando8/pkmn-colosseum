#include "hsd/hsd_initialize.h"

extern volatile s32 lbl_8047B294;

HSD_RenderPass HSD_GetCurrentRenderPass(void)
{
    return (HSD_RenderPass) lbl_8047B294;
}
