#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_robj.h"

extern void* HSD_IDGetDataFromTable(void* table, u32 id, s32* success);
extern void HSD_JObjSetupMatrix(HSD_JObj* jobj);
extern void HSD_JObjMakeMatrix(HSD_JObj* jobj);
extern void fn_801AE008(HSD_Exp* exp, u32 type, void* object,
                        HSD_ObjUpdateFunc update);
extern void fn_801AF224(HSD_RObj* robj, HSD_JObj* jobj,
                        HSD_ObjUpdateFunc update);
extern void fn_801AF560(HSD_RObj* robj, HSD_JObj* jobj,
                        HSD_ObjUpdateFunc update);
void fn_801AEFF0(HSD_RObj* robj, HSD_JObj* jobj);
extern s32 fn_801AFCAC(HSD_RObj* robj, u32 type, Vec* out);

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

void fn_801AED88(HSD_RObj* robj, HSD_JObj* jobj,
                 HSD_ObjUpdateFunc update)
{
    HSD_RObj* current;
    Vec position;

    if (robj == NULL) {
        return;
    }

    if (fn_801AFCAC(robj, 1, &position) != 0) {
        update(jobj, 0x35, (HSD_ObjData*) &position);
        update(jobj, 0x38, NULL);
    }
    fn_801AF560(robj, jobj, update);
    fn_801AF224(robj, jobj, update);
    fn_801AEFF0(robj, jobj);

    for (current = robj; current != NULL; current = current->next) {
        if ((current->flags & ROBJ_TYPE_MASK) == REFTYPE_EXP &&
            (current->flags & 0x80000000) != 0)
        {
            fn_801AE008(&current->u.exp, current->flags & 0x0FFFFFFF,
                        jobj, update);
        }
    }
}

void fn_801AEFF0(HSD_RObj* robj, HSD_JObj* jobj)
{
    HSD_RObj* current;
    BOOL changed = FALSE;

    if (jobj == NULL) {
        __assert("robj.c", 0x29E, "jobj");
    }

    for (current = robj; current != NULL; current = current->next) {
        if ((current->flags & ROBJ_TYPE_MASK) != REFTYPE_LIMIT) {
            continue;
        }

        switch (current->flags & 0x0FFFFFFF) {
        case 1:
            if (jobj->rotate_x < current->u.limit) {
                jobj->rotate_x = current->u.limit;
            }
            break;
        case 2:
            if (jobj->rotate_x > current->u.limit) {
                jobj->rotate_x = current->u.limit;
            }
            break;
        case 3:
            if (jobj->rotate_y < current->u.limit) {
                jobj->rotate_y = current->u.limit;
            }
            break;
        case 4:
            if (jobj->rotate_y > current->u.limit) {
                jobj->rotate_y = current->u.limit;
            }
            break;
        case 5:
            if (jobj->rotate_z < current->u.limit) {
                jobj->rotate_z = current->u.limit;
            }
            break;
        case 6:
            if (jobj->rotate_z > current->u.limit) {
                jobj->rotate_z = current->u.limit;
            }
            break;
        case 7:
            if (jobj->translate_x < current->u.limit) {
                jobj->translate_x = current->u.limit;
            }
            break;
        case 8:
            if (jobj->translate_x > current->u.limit) {
                jobj->translate_x = current->u.limit;
            }
            break;
        case 9:
            if (jobj->translate_y < current->u.limit) {
                jobj->translate_y = current->u.limit;
            }
            break;
        case 10:
            if (jobj->translate_y > current->u.limit) {
                jobj->translate_y = current->u.limit;
            }
            break;
        case 11:
            if (jobj->translate_y < current->u.limit) {
                jobj->translate_y = current->u.limit;
            }
            break;
        case 12:
            if (jobj->translate_y > current->u.limit) {
                jobj->translate_y = current->u.limit;
            }
            break;
        default:
            continue;
        }
        changed = TRUE;
    }

    if (changed) {
        HSD_JObjMakeMatrix(jobj);
    }
}

s32 fn_801AFCAC(HSD_RObj* robj, u32 type, Vec* out)
{
    HSD_RObj* current;
    Vec position = { 0.0f, 0.0f, 0.0f };
    s32 count = 0;

    if (robj == NULL) {
        return 0;
    }

    for (current = robj; current != NULL; current = current->next) {
        if ((current->flags & ROBJ_TYPE_MASK) == REFTYPE_JOBJ &&
            (current->flags & 0x80000000) != 0 &&
            type == (current->flags & 0x0FFFFFFF))
        {
            if (current->u.jobj == NULL) {
                __assert("robj.c", 0x1F2, "rp->u.jobj");
            }
            HSD_JObjSetupMatrix(current->u.jobj);
            count++;
            position.x += current->u.jobj->mtx[0][3];
            position.y += current->u.jobj->mtx[1][3];
            position.z += current->u.jobj->mtx[2][3];
        }
    }

    if (count != 0) {
        f32 reciprocal = 1.0f / count;

        out->x = reciprocal * position.x;
        out->y = reciprocal * position.y;
        out->z = reciprocal * position.z;
    }
    return count;
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
