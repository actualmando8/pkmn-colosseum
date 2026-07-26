/**
 * @file hsd_robj_range_801ADD48.c
 * @brief sysdolphin TU proven by __FILE__ literals / melee alignment
 *        (naming phase). Boundary interpolated between evidence edges.
 */
#include "dolphin/types.h"
#include "hsd/hsd_robj.h"

#pragma push
#pragma optimization_level 2
HSD_Rvalue* fn_801ADF54(HSD_RvalueList* list)
{
    HSD_Rvalue* rvalue;
    HSD_Rvalue* result = NULL;
    HSD_Rvalue** cursor = &result;

    if (list == NULL) {
        return NULL;
    }

    while (list->joint != NULL) {
        struct ObjAllocData {
            u8 data[0x2C];
        };
        extern struct ObjAllocData lbl_80465688;
        extern const u8 lbl_8047DD50[7];
        extern const u8 lbl_8047DD58[7];
        extern void* HSD_ObjAlloc(void* data);
        extern void __assert(const u8* file, int line, const u8* condition);
        extern void* memset(void* dest, int value, u32 size);

        rvalue = HSD_ObjAlloc(&lbl_80465688);
        if (rvalue == NULL) {
            __assert(lbl_8047DD50, 0x485, lbl_8047DD58);
        }
        memset(rvalue, 0, sizeof(HSD_Rvalue));

        *cursor = rvalue;
        (*cursor)->flags = list->flags;
        list++;
        cursor = &(*cursor)->next;
    }

    return result;
}

extern f32 fn_801AE000(void*);

void fn_801ADE50(HSD_Exp* exp, HSD_ExpDesc* desc)
{
    extern void* memset(void* dest, int value, u32 size);

    memset(exp, 0, sizeof(HSD_Exp));
    if (desc != NULL) {
        if (desc->func != NULL) {
            exp->expr.func = desc->func;
        } else {
            exp->expr.func = fn_801AE000;
        }
        exp->rvalue = fn_801ADF54(desc->rvalue);
        exp->nb_args = -1;
    }
}

void fn_801ADD48(HSD_Exp* exp, HSD_ByteCodeExpDesc* desc)
{
    extern void* memset(void* dest, int value, u32 size);

    memset(exp, 0, sizeof(HSD_Exp));
    if (desc != NULL) {
        if (desc->bytecode != NULL) {
            exp->expr.bytecode = desc->bytecode;
        } else {
            exp->expr.bytecode = NULL;
        }
        exp->rvalue = fn_801ADF54(desc->rvalue);
        exp->nb_args = -1;
        exp->is_bytecode = 1;
    }
}
#pragma pop
