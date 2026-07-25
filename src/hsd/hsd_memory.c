/**
 * @file hsd_range_801A69C0.c
 * @brief hsd code, 0x801A69C0 - 0x801A8428 (23 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_tobj.h"

extern HSD_MObjInfo lbl_8036CB30; /* hsdMObj class info */
extern void* lbl_8047B2D0;
extern HSD_TObj* lbl_8047B2D8;
extern HSD_TObj* lbl_8047B2DC;

extern void HSD_TExpFreeTevDesc(HSD_TExpTevDesc* tevdesc);
extern void fn_801B7178(HSD_TExp* texp, u32 type, int flag);

void MObjRelease(HSD_Class* obj)
{
    HSD_MObj* mobj = HSD_MOBJ(obj);

    HSD_AObjRemove(mobj->aobj);
    hsdFreeMemPiece(mobj->mat, sizeof(HSD_Material));
    HSD_TObjRemoveAll(mobj->tobj);
    if (mobj->tevdesc != NULL) {
        HSD_TExpFreeTevDesc(mobj->tevdesc);
    }
    if (mobj->texp != NULL) {
        fn_801B7178(mobj->texp, 7, 1);
    }
    if (mobj->pe != NULL) {
        hsdFreeMemPiece(mobj->pe, sizeof(HSD_PEDesc));
    }
    lbl_8036CB30.parent.head.parent->release(obj);
}

void HSD_MObjDeleteShadowTexture(HSD_TObj* tobj)
{
    if (tobj != NULL) {
        HSD_TObj** cur = &lbl_8047B2DC;
        while (*cur != NULL) {
            if (*cur == tobj) {
                *cur = tobj->next;
                tobj->next = NULL;
                return;
            }
            cur = &(*cur)->next;
        }
    } else {
        HSD_TObj* next;
        for (next = NULL; lbl_8047B2DC != NULL; lbl_8047B2DC = next) {
            next = lbl_8047B2DC->next;
            lbl_8047B2DC->next = NULL;
        }
    }
}

void MObjAmnesia(void* info)
{
    if (info == lbl_8047B2D0) {
        lbl_8047B2D0 = 0;
    }
    if (info == (void*) &lbl_8036CB30) {
        lbl_8047B2D8 = 0;
        lbl_8047B2DC = 0;
    }
    lbl_8036CB30.parent.head.parent->amnesia(info);
}
