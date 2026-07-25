#include "dolphin/types.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_robj.h"

extern u8 lbl_80465688[];
extern u8 lbl_804656B4[];
extern void* HSD_ObjAlloc(void* alloc_data);
extern void HSD_ObjFree(void* alloc_data, void* object);
extern void* memset(void* dst, int value, u32 size);

HSD_RObj* fn_801AE4B0(void)
{
    HSD_RObj* robj = HSD_ObjAlloc(lbl_804656B4);

    if (robj == NULL) {
        __assert("robj.c", 0x3C5, "new");
    }
    memset(robj, 0, sizeof(HSD_RObj));
    return robj;
}

void fn_801AE50C(HSD_RObj* robj)
{
    HSD_RObj* next;

    for (; robj != NULL; robj = next) {
        HSD_Rvalue* rvalue;
        HSD_Rvalue* rvalue_next;

        next = robj->next;
        if ((robj->flags & ROBJ_TYPE_MASK) == REFTYPE_JOBJ) {
            HSD_JObjUnrefThis(robj->u.jobj);
        } else if ((robj->flags & ROBJ_TYPE_MASK) == REFTYPE_EXP) {
            for (rvalue = robj->u.exp.rvalue; rvalue != NULL;
                 rvalue = rvalue_next)
            {
                rvalue_next = rvalue->next;
                HSD_JObjUnrefThis(rvalue->jobj);
                HSD_ObjFree(lbl_80465688, rvalue);
            }
        }
        HSD_AObjRemove(robj->aobj);
        HSD_ObjFree(lbl_804656B4, robj);
    }
}
