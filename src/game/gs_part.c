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

GSpart *GSpartCreate(void)
{
    GSpart *part;
    u32 i;

    part = lbl_8047ABBC;
    for (i = 0; i < lbl_8047ABC0; i++, part++) {
        if (part->inUse == 0) {
            part->inUse = 1;
            return part;
        }
    }
    return NULL;
}

extern u16 lbl_8047ABB8;
extern u32 _toolentryAlloc__FUl(u32 size);
extern void* fn_800E27B0(u16 handle);

void GSpartInit(u32 count)
{
    u32 handle;
    u32 i;

    lbl_8047ABC0 = count;
    handle = _toolentryAlloc__FUl(count * sizeof(GSpart));
    lbl_8047ABB8 = handle;
    if ((u16)handle != 0) {
        lbl_8047ABBC = fn_800E27B0((u16)handle);
        for (i = 0; i < lbl_8047ABC0; i++) {
            ((u8*)lbl_8047ABBC)[i * sizeof(GSpart)] = 0;
        }
    }
}
