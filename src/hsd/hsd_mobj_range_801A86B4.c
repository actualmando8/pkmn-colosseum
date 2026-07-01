#include "dolphin/types.h"

/* =========================================================================
 * Partial banked source for reserved split unit 0x801A86B4 - 0x801AA608.
 * Only the functions that byte-match under GC/1.3 are provided; the rest of
 * the range (including the five float-heavy matrix routines fn_801A8884 /
 * 8B94 / 8D1C / 98CC / 9DF0, which are 16-62% source-refinement fleet
 * targets, and the empty-stub dispatchers) stay as extracted asm.
 * ========================================================================= */

/* Object / class helpers (DTK names). */
extern void  fn_801A6960(void* ptr);   /* HSD_MemFree  */
extern void* fn_801A6928(s32 size);    /* HSD_MemAlloc */
extern void  hsdInitClassInfo(void* info, void* parent, const char* library,
                              const char* name, u32 size, u32 flags);

/* Data / global symbols (DTK names). */
extern void* lbl_8047B2E0;           /* free-list pool chain head (sbss) */
extern u8    lbl_8036CBF0[];         /* data heap descriptor             */
extern u8    lbl_8036CC00[];         /* data class info                  */
extern u8    lbl_8036C638[];         /* data parent class info           */
extern const char lbl_80274EC8[];    /* rodata string                    */
extern const char lbl_8047DCA0;      /* sdata2 string                    */

/* Address: 0x801A9570 | Size: 0x1C  -- already-banked (GC/1.3, calibration) */
void HSD_MtxGetTranslate(f32 mtx[3][4], f32* vec) {
    vec[0] = mtx[0][3];
    vec[1] = mtx[1][3];
    vec[2] = mtx[2][3];
}

/* Address: 0x801AA350 | Size: 0xC  -- already-banked (GC/1.3, calibration) */
void fn_801AA350(void) {
    lbl_8047B2E0 = 0;
}

/* Address: 0x801AA498 | Size: 0x34  -- HSD_ObjFree (from backup hsd_pobj_disp.c) */
#pragma push
#pragma optimization_level 1
void HSD_ObjFree(void* list, void* data)
{
    void* l = list;
    *(u32*)((u8*)l + 0x8) = *(u32*)((u8*)l + 0x8) - 1;
    fn_801A6960(data);
}
#pragma pop

/* Address: 0x801AA4CC | Size: 0x6C  -- HSD_ObjAlloc (from backup hsd_pobj_disp.c) */
#pragma push
#pragma optimization_level 1
void* HSD_ObjAlloc(void* list)
{
    void* l = list;

    if ((*(u8*)l & 0x80) >> 7) {
        if (*(u32*)((u8*)l + 0x8) >= *(u32*)((u8*)l + 0x14)) {
            return NULL;
        }
    }

    *(u32*)((u8*)l + 0x8) += 1;
    if (*(u32*)((u8*)l + 0x8) > *(u32*)((u8*)l + 0x10)) {
        *(u32*)((u8*)l + 0x10) = *(u32*)((u8*)l + 0x8);
    }

    return fn_801A6928(*(u32*)((u8*)l + 0x20));
}
#pragma pop

/* Address: 0x801AA538 | Size: 0x30  -- HSD_ObjSetHeap (from backup hsd_pobj_disp.c) */
#pragma push
#pragma optimization_level 0
void HSD_ObjSetHeap(void* a, void* b)
{
    *(volatile void**)((u8*)lbl_8036CBF0 + 0x4) = b;
    *(volatile void**)((u8*)lbl_8036CBF0 + 0x0) = b;
    *(volatile void**)((u8*)lbl_8036CBF0 + 0xc) = a;
    *(volatile void**)((u8*)lbl_8036CBF0 + 0x8) = a;
}
#pragma pop

/* Address: 0x801AA568 | Size: 0x44  -- PObj class info init (small)
 * (from backup hsd_pobj_disp.c) */
void ObjInfoInit_802596A4(void)
{
    hsdInitClassInfo((void*) lbl_8036CC00,
                     (void*) lbl_8036C638, (char*) lbl_80274EC8,
                     (char*) &lbl_8047DCA0, 0x3c, 0x8);
}
