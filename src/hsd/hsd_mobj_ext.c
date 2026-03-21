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

/* External SDA variables */
extern u32 lbl_8047B2D4;

/* ========================================================================= */
/*  Pragma stubs for complex functions                                       */
/* ========================================================================= */

/* Address: 0x801A8354 | Size: 0x68 */
/* Material PE (pixel engine) setup helper */
void fn_801A8354(void) {
    /* Pixel engine blend mode configuration */
}

/* Address: 0x801A83BC | Size: 0x6C */
void fn_801A83BC(void) {
    /* Pixel engine alpha compare setup */
}

/* ========================================================================= */
/*  Small accessor / utility functions                                       */
/* ========================================================================= */

/* Address: 0x801A8428 | Size: 0x18 */
/* Render mode flag check - checks if XLU bit set */
u32 fn_801A8428(HSD_MObj* mobj) {
    if (mobj == NULL) {
        return 0;
    }
    return mobj->rendermode & RENDER_XLU;
}

/* Address: 0x801A8440 | Size: 0x18 */
/* Render mode flag check - checks render textures */
u32 fn_801A8440(HSD_MObj* mobj) {
    if (mobj == NULL) {
        return 0;
    }
    return mobj->rendermode & RENDER_TEXTURES;
}

/* Address: 0x801A8458 | Size: 0x18 */
/* Render mode flag check - checks channel field */
u32 fn_801A8458(HSD_MObj* mobj) {
    if (mobj == NULL) {
        return 0;
    }
    return mobj->rendermode & CHANNEL_FIELD;
}

/* NOTE: fn_801A8470 is already in hsd_mobj.c */

/* Address: 0x801A8478 | Size: 0x30 */
void fn_801A8478(void) {
    /* Material color channel setup */
}

/* Address: 0x801A84A8 | Size: 0xC */
void fn_801A84A8(u8* obj) {
    if (obj != NULL) {
        *(u32*)(obj + 0x4) = 0;
    }
}

/* Address: 0x801A84B4 | Size: 0x30 */
void fn_801A84B4(void) {
    /* Material alpha channel setup */
}

/* Address: 0x801A84E4 | Size: 0xC */
void fn_801A84E4(u8* obj) {
    if (obj != NULL) {
        *(u32*)(obj + 0x4) = 0;
    }
}

/* Address: 0x801A84F0 | Size: 0x34 */
void fn_801A84F0(void) {
    /* GX channel control setup (diffuse) */
}

/* Address: 0x801A8524 | Size: 0x4C */
void fn_801A8524(void) {
    /* GX channel control setup (specular) */
}

/* Address: 0x801A8570 | Size: 0x34 */
void fn_801A8570(void) {
    /* GX channel ambient color setup */
}

/* Address: 0x801A85A4 | Size: 0x4C */
void fn_801A85A4(void) {
    /* GX channel material color setup */
}
