#include "dolphin/types.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_robj.h"

extern void* HSD_IDGetDataFromTable(void* table, u32 id, s32* success);

void fn_801AEBE4(HSD_RObj* robj, HSD_RObjDesc* desc)
{
    for (; robj != NULL && desc != NULL;
         robj = robj->next, desc = desc->next)
    {
        if ((robj->flags & ROBJ_TYPE_MASK) == REFTYPE_JOBJ) {
            HSD_JObjUnrefThis(robj->u.jobj);
            robj->u.jobj =
                HSD_IDGetDataFromTable(NULL, (u32) desc->u.joint, NULL);
            if (robj->u.jobj == NULL) {
                __assert("robj.c", 0x330, "robj->u.jobj");
            }
            HSD_JObjRefThis(robj->u.jobj);
        } else if ((robj->flags & ROBJ_TYPE_MASK) == REFTYPE_EXP) {
            HSD_Rvalue* rvalue = robj->u.exp.rvalue;
            HSD_RvalueList* list = desc->u.exp->rvalue;

            while (rvalue != NULL && list->joint != NULL) {
                HSD_JObjUnrefThis(rvalue->jobj);
                rvalue->jobj =
                    HSD_IDGetDataFromTable(NULL, (u32) list->joint, NULL);
                if (rvalue->jobj == NULL) {
                    __assert("robj.c", 0x4F2, "rvalue->jobj");
                }
                HSD_JObjRefThis(rvalue->jobj);
                rvalue = rvalue->next;
                list++;
            }
        }
    }
}

static inline void RObjAddAnim(HSD_RObj* robj, HSD_RObjAnimJoint* anim)
{
    if (robj == NULL || anim == NULL) {
        return;
    }
    if (robj->aobj != NULL) {
        HSD_AObjRemove(robj->aobj);
    }
    robj->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
}

void fn_801AFE68(HSD_RObj* robj, HSD_RObjAnimJoint* anim)
{
    HSD_RObj* i;
    HSD_RObjAnimJoint* j;

    if (robj == NULL || anim == NULL) {
        return;
    }
    for (i = robj, j = anim; i != NULL && j != NULL;
         i = i->next, j = j->next)
    {
        RObjAddAnim(i, j);
    }
}

static inline void RObjReqAnim(HSD_RObj* robj, f32 frame)
{
    if (robj != NULL && robj->aobj != NULL) {
        HSD_AObjReqAnim(robj->aobj, frame);
    }
}

void fn_801AFEFC(HSD_RObj* robj, f32 frame)
{
    for (; robj != NULL; robj = robj->next) {
        RObjReqAnim(robj, frame);
    }
}

static inline void RObjReqAnimByFlags(HSD_RObj* robj, f32 frame, u32 flags)
{
    if (robj != NULL && robj->aobj != NULL && (flags & 0x80) != 0) {
        HSD_AObjReqAnim(robj->aobj, frame);
    }
}

void fn_801AFF64(HSD_RObj* robj, f32 frame, u32 flags)
{
    for (; robj != NULL; robj = robj->next) {
        RObjReqAnimByFlags(robj, frame, flags);
    }
}

static inline void RObjRemoveAnim(HSD_RObj* robj)
{
    if (robj != NULL && robj->aobj != NULL) {
        HSD_AObjRemove(robj->aobj);
        robj->aobj = NULL;
    }
}

void fn_801AFFE0(HSD_RObj* robj)
{
    for (; robj != NULL; robj = robj->next) {
        RObjRemoveAnim(robj);
    }
}

extern void RObjUpdateFunc(void* obj, u32 type, HSD_ObjData* val);

static inline void RObjAnim(HSD_RObj* robj)
{
    if (robj != NULL) {
        HSD_AObjInterpretAnim(robj->aobj, robj, RObjUpdateFunc);
    }
}

void fn_801B0040(HSD_RObj* robj)
{
    if (robj != NULL) {
        for (; robj != NULL; robj = robj->next) {
            RObjAnim(robj);
        }
    }
}
