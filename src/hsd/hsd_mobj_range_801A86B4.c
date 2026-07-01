/**
 * Partial source for the hsd_mtx 0x801A86B4 range: only the two trivial helpers
 * that byte-match under GC/1.3. The five large matrix routines in this range
 * (fn_801A8884/8B94/8D1C/98CC/9DF0 = HSD_MtxSRT/MkRotationMtx/GetScale/
 * GetRotation/InverseConcat) are float-math near-misses (best ~16-71%, source
 * refinement fleet targets) and the two axis-dispatch stubs (fn_801A86B4,
 * fn_801A958C) have no recovered C; all are left as extracted asm.
 */

#include "dolphin/types.h"

/* Address: 0x801A9570 | Size: 0x1C */
/* Extract translation vector from 3x4 matrix (last column) */
void HSD_MtxGetTranslate(f32 mtx[3][4], f32* vec) {
    vec[0] = mtx[0][3];
    vec[1] = mtx[1][3];
    vec[2] = mtx[2][3];
}

/* Address: 0x801AA350 | Size: 0xC */
/* Clear vtx desc list head pointer */
extern u32 lbl_8047B2E0;
void fn_801AA350(void) {
    lbl_8047B2E0 = 0;
}
