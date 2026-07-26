/**
 * Residual source island 0x80055E38-0x80056A80 from the shared menuCB unit.
 */
#define MENUCB_PREFIX_80055E38_ONLY
#include "src/game/menu/menuCB_range_80055E38.c"

extern f32 lbl_8047A57C;
extern f32 lbl_8047A574;
extern u32 lbl_8047A580;
extern u8 lbl_8026768C[];
extern f32 lbl_8047BED0;
extern f32 lbl_8047BED4;
extern f32 lbl_8047BED8;
extern f32 lbl_8047BEB4;
extern void* fn_80104704(u32);

void fn_80056854(void) {
    f32 old578;
    u32 tbl1[3];
    u32 tbl2[3];

    old578 = lbl_8047A578;
    if (old578 > lbl_8047BEC0) {
        lbl_8047A57C = lbl_8047A57C + old578;
        if (lbl_8047A57C >= lbl_8047BEB4) {
            lbl_8047A57C = lbl_8047BEB4;
            lbl_8047A578 = lbl_8047BEC0;
        }
    }
    if (lbl_8047A578 < lbl_8047BEC0) {
        lbl_8047A57C = lbl_8047A57C + lbl_8047A578;
        if (lbl_8047A57C <= lbl_8047BED0) {
            lbl_8047A57C = lbl_8047BED0;
            lbl_8047A578 = lbl_8047BEC0;
        }
    }
    if (lbl_8047BEC0 != old578) {
        s32 val;
        u32 idx = lbl_8047A580;
        tbl1[0] = ((u32*)lbl_8026768C)[0];
        tbl1[1] = ((u32*)lbl_8026768C)[1];
        tbl1[2] = ((u32*)lbl_8026768C)[2];
        if ((s32)idx < 0 || (s32)idx >= 3) {
            val = -1;
        } else {
            val = (s32)tbl1[idx];
        }
        if (val >= 0) {
            if (lbl_8047BEC0 == lbl_8047A578) {
                fn_80102254((u32)val, 0);
            } else {
                void* obj = fn_80104704((u32)val);
                if (obj != (void*)0) {
                    *(s16*)((u8*)obj + 0x84) = (s16)(s32)(lbl_8047BED4 * lbl_8047A57C);
                }
            }
        }
        idx = lbl_8047A584;
        {
            u32 *src = (u32*)&lbl_8026768C;
            tbl2[0] = src[0];
            tbl2[1] = src[1];
            tbl2[2] = src[2];
        }
        if ((s32)idx < 0 || (s32)idx >= 3) {
            val = -1;
        } else {
            val = (s32)tbl2[idx];
        }
        if (val >= 0) {
            void* obj = fn_80104704((u32)val);
            if (obj != (void*)0) {
                *(s16*)((u8*)obj + 0x84) = (s16)(s32)(lbl_8047BED4 * lbl_8047A57C);
                if (old578 > lbl_8047BEC0) {
                    *(s16*)((u8*)obj + 0x84) = *(s16*)((u8*)obj + 0x84) + 0x1a6;
                } else {
                    *(s16*)((u8*)obj + 0x84) = *(s16*)((u8*)obj + 0x84) - 0x1a6;
                }
            }
        }
    }
    lbl_8047A574 = lbl_8047A574 + lbl_8047BED8;
    if (lbl_8047A574 > lbl_8047BEB4) {
        lbl_8047A574 = lbl_8047A574 - lbl_8047BEB4;
    }
    if (lbl_8047A570 < lbl_8047BEB4) {
        lbl_8047A570 = lbl_8047A570 + lbl_8047BED8;
        if (lbl_8047A570 >= lbl_8047BEB4) {
            lbl_8047A570 = lbl_8047BEB4;
        }
    }
}
