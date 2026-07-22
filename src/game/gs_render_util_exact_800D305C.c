#include "dolphin/types.h"
#include "game/gs_render_util.h"

extern GSRenderState* lbl_8047AA80;

void fn_800D305C(u8 level)
{
    lbl_8047AA80->frameLevel = level;
}

u32 fn_800D3068(void)
{
    return lbl_8047AA80->renderWidth;
}
