/**
 * @file hsd_lobj_tail_exact_801A68D0.c
 * @brief Strict light-state accessors and dispatch tail, 0x801A68D0 - 0x801A69C0.
 */
#include "dolphin/types.h"
#include "hsd/hsd_lobj.h"

extern s32 lbl_8047B2B8;
extern u32 lbl_8047B2BC;
extern u32 lbl_8047B2C0;
extern u32 lbl_8047B2C4;
extern u32 lbl_8047B2C8;
extern u8 lbl_80465608[];

s32 HSD_LObjGetNbActive(void)
{
    return lbl_8047B2B8;
}

u32 HSD_LObjGetLightMaskSpecular(void)
{
    return lbl_8047B2C0;
}

u32 HSD_LObjGetLightMaskAlpha(void)
{
    return lbl_8047B2C8;
}

u32 HSD_LObjGetLightMaskAttnFunc(void)
{
    return lbl_8047B2C4;
}

u32 HSD_LObjGetLightMaskDiffuse(void)
{
    return lbl_8047B2BC;
}

void HSD_LObjClearFlags(HSD_LObj* lobj, u32 flags)
{
    if (lobj == NULL) {
        return;
    }
    lobj->flags &= ~flags;
}

void HSD_LObjSetFlags(HSD_LObj* lobj, u32 flags)
{
    if (lobj == NULL) {
        return;
    }
    lobj->flags |= flags;
}

void fn_801A6928(HSD_LObj* lobj)
{
    void (*func)(HSD_LObj*, u32, u32);
    func = ((void (**)(HSD_LObj*, u32, u32))lbl_80465608)[0];
    func(lobj, 0x20, 0);
}

void fn_801A6960(HSD_LObj* lobj)
{
    void (*func)(HSD_LObj*);
    func = ((void (**)(HSD_LObj*))lbl_80465608)[1];
    func(lobj);
}

typedef void (*LObjDispatchFn)(void*);

void fn_801A6990(HSD_LObj* lobj)
{
    LObjDispatchFn func;
    func = ((LObjDispatchFn*)lbl_80465608)[4];
    func(lobj);
}
