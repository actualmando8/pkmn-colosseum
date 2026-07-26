/**
 * Residual GX/SDK suffix, 0x800BC618 - 0x800BE464.
 */
#define SDK_800BC618_SUFFIX_ACTIVE
#include "src/dolphin/sdk_range_800BB30C.c"

extern u32 lbl_803135E0[];

void fn_800BC6F0(u32 stage, u32 coord, u32 map, u32 color)
{
    u32* ptref;
    u32 tmap;
    u32 tcoord;
    u32 raster;
    u32 enabled;

    ptref = &gx->tref[stage / 2];
    gx->texmapId[stage] = map;

    tmap = map & ~0x100U;
    if (tmap >= 8) {
        tmap = 0;
    }

    if (coord >= 8) {
        tcoord = 0;
        gx->tevTcEnab &= ~(1 << stage);
    } else {
        tcoord = coord;
        gx->tevTcEnab |= 1 << stage;
    }

    raster = color == 0xFF ? 7 : lbl_803135E0[color];
    enabled = map != 0xFF && (map & 0x100) == 0;
    if (stage & 1) {
        *ptref = (*ptref & ~0x7000U) | (tmap << 12);
        *ptref = (*ptref & ~0x38000U) | (tcoord << 15);
        *ptref = (*ptref & ~0x380000U) | (raster << 19);
        *ptref = (*ptref & ~0x40000U) | (enabled << 18);
    } else {
        *ptref = (*ptref & ~0x7U) | tmap;
        *ptref = (*ptref & ~0x38U) | (tcoord << 3);
        *ptref = (*ptref & ~0x380U) | (raster << 7);
        *ptref = (*ptref & ~0x40U) | (enabled << 6);
    }

    GX_BP_REG(*ptref);
    gx->field_002 = 0;
    gx->dirtyState |= 1;
}
