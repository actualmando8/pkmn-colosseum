/**
 * @file hsd_mobj_ext.c
 * @brief HSD MObj extended - Material setup, render mode, PE configuration.
 *
 * Address range: 0x801A8354 - 0x801A85A4
 * Contains material setup helpers, pixel engine configuration,
 * and render mode accessor stubs. These are the tail end of the
 * MObj/material system before the matrix utility code begins.
 */

#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_mobj.h"

/* External functions */
extern void fn_801C29C4(HSD_AObj* aobj, f32 frame);    /* HSD_AObjReqAnim */
extern void fn_801BEE68(HSD_TObj* tobj, u32 flags, f32 frame); /* HSD_TObjReqAnimAll */
extern void fn_801C25E4(HSD_AObj* aobj);                /* HSD_AObjRemove */
extern HSD_AObj* fn_801C2670(void* aobjdesc);           /* HSD_AObjLoadDesc */
extern void fn_801BEEDC(HSD_TObj* tobj, void* texanim); /* HSD_TObjAddAnimAll */
extern void fn_801AA35C(void* list, u32 size, u32 alignment);
extern void fn_801AA498(void* list, void* data);
extern void* fn_801AA4CC(void* list);
extern void fn_80196E10(const char* file, s32 line, const char* msg);

/* BSS vtx desc globals */
extern u8 lbl_80465620[];
extern u8 lbl_8046564C[];

/* SDA2 string constants */
extern const char lbl_8047DC48[6];
extern const char lbl_8047DC50[4];
extern const char lbl_8047DC54[4];

/* SDA variable */
extern u32 lbl_8047B2D4;

/* ========================================================================= */
/*  Material PE and texture setup                                            */
/* ========================================================================= */

/* Address: 0x801A8354 | Size: 0x68 */
/* HSD_MObjReqAnim - request material animation at given frame.
 * r3=mobj, r4=flags, f1=frame */
void fn_801A8354(HSD_MObj* mobj, u32 flags, f32 frame) {
    if (mobj == NULL) {
        return;
    }
    if (flags & 4) {
        fn_801C29C4(mobj->aobj, frame);
    }
    fn_801BEE68(mobj->tobj, flags, frame);
}

/* Address: 0x801A83BC | Size: 0x6C */
/* HSD_MObjAddAnim - add material animation to an MObj */
void fn_801A83BC(HSD_MObj* mobj, HSD_MatAnim* matanim) {
    if (mobj == NULL) {
        return;
    }
    if (matanim == NULL) {
        return;
    }
    if (mobj->aobj != NULL) {
        fn_801C25E4(mobj->aobj);
    }
    mobj->aobj = fn_801C2670(matanim->aobjdesc);
    fn_801BEEDC(mobj->tobj, matanim->texanim);
}

/* ========================================================================= */
/*  Render mode flag operations                                              */
/* ========================================================================= */

/* Address: 0x801A8428 | Size: 0x18 */
/* HSD_MObjClearFlags - clear bits in rendermode */
void fn_801A8428(HSD_MObj* mobj, u32 flags) {
    if (mobj == NULL) {
        return;
    }
    mobj->rendermode &= ~flags;
}

/* Address: 0x801A8440 | Size: 0x18 */
/* HSD_MObjSetFlags - set bits in rendermode */
void fn_801A8440(HSD_MObj* mobj, u32 flags) {
    if (mobj == NULL) {
        return;
    }
    mobj->rendermode |= flags;
}

/* Address: 0x801A8458 | Size: 0x18 */
/* HSD_MObjGetRenderMode - return the entire rendermode value */
u32 fn_801A8458(HSD_MObj* mobj) {
    if (mobj != NULL) {
        return mobj->rendermode;
    }
    return 0;
}

/* NOTE: fn_801A8470 is an SDA setter for lbl_8047B2D4, already 100% matching,
 * but it belongs in this TU so we include it */

/* Address: 0x801A8470 | Size: 0x8 */
void fn_801A8470(u32 val) {
    lbl_8047B2D4 = val;
}

/* ========================================================================= */
/*  Vertex descriptor list management                                        */
/* ========================================================================= */

/* Address: 0x801A8478 | Size: 0x30 */
/* Initialize color vertex descriptor list */
void fn_801A8478(void) {
    fn_801AA35C(lbl_80465620, 0x30, 4);
}

/* Address: 0x801A84A8 | Size: 0xC */
/* Get pointer to color vertex descriptor list */
void* fn_801A84A8(void) {
    return lbl_80465620;
}

/* Address: 0x801A84B4 | Size: 0x30 */
/* Initialize alpha vertex descriptor list */
void fn_801A84B4(void) {
    fn_801AA35C(lbl_8046564C, 0xC, 4);
}

/* Address: 0x801A84E4 | Size: 0xC */
/* Get pointer to alpha vertex descriptor list */
void* fn_801A84E4(void) {
    return lbl_8046564C;
}

/* Address: 0x801A84F0 | Size: 0x34 */
/* Set vertex attribute format for color channel */
void fn_801A84F0(void* data) {
    if (data != NULL) {
        fn_801AA498(lbl_80465620, data);
    }
}

/* Address: 0x801A8524 | Size: 0x4C */
/* Allocate/init color channel vtx desc with assert */
void* fn_801A8524(void) {
    void* result;
    result = fn_801AA4CC(lbl_80465620);
    if (result == NULL) {
        fn_80196E10(lbl_8047DC48, 0x396, lbl_8047DC50);
    }
    return result;
}

/* Address: 0x801A8570 | Size: 0x34 */
/* Set vertex attribute format for alpha channel */
void fn_801A8570(void* data) {
    if (data != NULL) {
        fn_801AA498(lbl_8046564C, data);
    }
}

/* Address: 0x801A85A4 | Size: 0x4C */
/* Allocate/init alpha channel vtx desc with assert */
void* fn_801A85A4(void) {
    void* result;
    result = fn_801AA4CC(lbl_8046564C);
    if (result == NULL) {
        fn_80196E10(lbl_8047DC48, 0x377, lbl_8047DC54);
    }
    return result;
}
