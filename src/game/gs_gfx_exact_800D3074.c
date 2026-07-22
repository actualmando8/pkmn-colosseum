#include "dolphin/types.h"
#include "game/gs_gfx.h"

extern void fn_800D4F98(s32 arg0, ...);
extern void GXFlush(void);
extern void GSgfxBackFBDoFrame(void);
extern void fn_801BF8A0(s32 value);
extern void fn_801E16F0(void);
extern void fn_801BF6AC(void);
extern void GStextureConvertFromHW(void* texture, s32 mode);
extern void fn_800B8E74(void);
extern void GXInvalidateTexAll(void);

void fn_800D3074(u32 flag)
{
    if (flag == 0) {
        return;
    }

    lbl_8047AA80->renderEnabled = flag;
}

u32 fn_800D3088(void)
{
    return lbl_8047AA80->frameDelta;
}

u32 fn_800D3094(void)
{
    return lbl_8047AA80->xfbCount;
}

void fn_800D30A0(u32 value)
{
    lbl_8047AA80->xfbIndex = value;
}

void fn_800D30AC(void)
{
    if (lbl_8047AA80->mode == 1) {
        fn_800D4F98(4, 0);
    } else {
        GXFlush();
    }
}

void fn_800D30F0(u32 flag)
{
    void* renderTarget = lbl_8047AA80->renderTarget;

    if (renderTarget == (void*)0xFEFEFEFEU) {
        return;
    }

    if (lbl_8047AA80->progressiveFlag == 0) {
        GSgfxBackFBDoFrame();
        fn_801BF8A0(0);
        fn_801E16F0();
        fn_801BF6AC();
    } else if (renderTarget != NULL) {
        GStextureConvertFromHW(renderTarget, 1);
        flag = 0;
    }

    lbl_8047AA80->progressiveFlag = 1;

    if ((u8)flag != 0) {
        fn_800B8E74();
        if (lbl_8047AA80->renderTarget != NULL) {
            GXInvalidateTexAll();
        }
    }
}
