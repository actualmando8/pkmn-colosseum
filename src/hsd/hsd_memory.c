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
extern HSD_ClassInfo lbl_8036C638;
extern u8 lbl_80465608[];
extern void* lbl_8047B2D0;
extern HSD_TObj* lbl_8047B2D8;
extern HSD_TObj* lbl_8047B2DC;
extern char lbl_80274E10[];
extern char lbl_80274E1C[];
extern char lbl_80274E38[];
extern char lbl_80274E50[];

extern void HSD_TExpFreeTevDesc(HSD_TExpTevDesc* tevdesc);
extern void fn_801B7178(HSD_TExp* texp, u32 type, int flag);
extern void fn_80193AF0(void* mem, u32 size);
extern void HSD_MObjSetup(HSD_MObj* mobj, u32 rendermode);
extern void HSD_MObjUnset(HSD_MObj* mobj, u32 rendermode);
extern int MObjLoad(HSD_MObj* mobj, HSD_MObjDesc* desc);
extern void MObjUpdateFunc(void* obj, u32 type, HSD_ObjData* val);
void MObjAmnesia(HSD_ClassInfo* info);
void MObjRelease(HSD_Class* obj);

typedef struct HSD_MemCallbacks {
    void* alloc;
    void* free;
    void* clear;
    void* get_remain;
    void* check_own;
} HSD_MemCallbacks;

void _HSD_MemSetCallbacks(HSD_MemCallbacks* callbacks, u32 size)
{
    if (size != sizeof(HSD_MemCallbacks)) {
        __assert(lbl_80274E10, sizeof(HSD_MemCallbacks), lbl_80274E1C);
    }
    *(HSD_MemCallbacks*) lbl_80465608 = *callbacks;
}

void MObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&lbl_8036CB30),
                     HSD_CLASS_INFO(&lbl_8036C638), lbl_80274E38,
                     lbl_80274E50, sizeof(HSD_MObjInfo), sizeof(HSD_MObj));

    HSD_CLASS_INFO(&lbl_8036CB30)->release = MObjRelease;
    HSD_CLASS_INFO(&lbl_8036CB30)->amnesia = MObjAmnesia;
    HSD_MOBJ_INFO(&lbl_8036CB30)->setup = HSD_MObjSetup;
    HSD_MOBJ_INFO(&lbl_8036CB30)->unset = HSD_MObjUnset;
    HSD_MOBJ_INFO(&lbl_8036CB30)->load = MObjLoad;
    HSD_MOBJ_INFO(&lbl_8036CB30)->make_texp = MObjMakeTExp;
    HSD_MOBJ_INFO(&lbl_8036CB30)->setup_tev = MObjSetupTev;
    HSD_MOBJ_INFO(&lbl_8036CB30)->update = MObjUpdateFunc;
}

void MObjRelease(HSD_Class* obj)
{
    HSD_MObj* mobj = HSD_MOBJ(obj);

    HSD_AObjRemove(mobj->aobj);
    fn_80193AF0(mobj->mat, sizeof(HSD_Material));
    HSD_TObjRemoveAll(mobj->tobj);
    if (mobj->tevdesc != NULL) {
        HSD_TExpFreeTevDesc(mobj->tevdesc);
    }
    if (mobj->texp != NULL) {
        fn_801B7178(mobj->texp, 7, 1);
    }
    if (mobj->pe != NULL) {
        fn_80193AF0(mobj->pe, sizeof(HSD_PEDesc));
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

void MObjAmnesia(HSD_ClassInfo* info)
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
