/**
 * @file gs_part.c
 * @brief GSpart (model part/joint-subtree accessors)
 *
 * Split from gs_range_800E202C.c (0x800EE150-0x800EE928) — one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"

typedef struct GSpart {
    u8 inUse;
    u8 _pad;
    u16 index;
    void *model;
    void *jobj;
} GSpart;

void GSpartFree(GSpart *part)
{
    part->inUse = 0;
}

extern GSpart *lbl_8047ABBC;
extern u32 lbl_8047ABC0;
extern void *lbl_8047ABA8;
extern u32 lbl_8047ABAC;
extern u32 lbl_8047ABB0;

void fn_800EE20C(void *jobj)
{
    if (lbl_8047ABB0++ == lbl_8047ABAC) {
        lbl_8047ABA8 = jobj;
    }
}

static inline GSpart *GSpartFindFree(void)
{
    GSpart *part;
    u32 i;

    part = lbl_8047ABBC;
    for (i = 0; i < lbl_8047ABC0; i++, part++) {
        if (part->inUse == 0) {
            return part;
        }
    }
    return NULL;
}

GSpart *GSpartCreate(void)
{
    GSpart *part;

    part = GSpartFindFree();
    if (part == NULL) {
        return NULL;
    }
    part->inUse = 1;
    return part;
}
