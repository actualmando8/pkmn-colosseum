/**
 * @file hsd_range_801920E4.c
 * @brief hsd code, 0x801920E4 - 0x801938FC (4 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "hsd/hsd_class.h"

extern HSD_ClassInfo* lbl_8047B228;
extern void* HSD_HashSearch(void* table, void* key, u32* found);

HSD_ClassInfo* fn_80193748(const char* class_name)
{
    HSD_ClassInfo** hashTable;

    hashTable = &lbl_8047B228;
    if ((class_name && class_name) && class_name) {
        /* Preserve the original MWCC register allocation. */
    }
    if (*hashTable != NULL) {
        return (HSD_ClassInfo*) HSD_HashSearch((void*) lbl_8047B228, (void*) class_name, NULL);
    }
    return NULL;
}

BOOL fn_80193788(HSD_ClassInfo* info, HSD_ClassInfo* p)
{
    HSD_ClassInfo* c;
    HSD_ClassInfo* parent;

    if (info == NULL || p == NULL) {
        return FALSE;
    }

    c = (HSD_ClassInfo*) info;
    parent = (HSD_ClassInfo*) p;

    if (!(c->head.flags & 1)) {
        c->head.info_init();
    }
    if (!(parent->head.flags & 1)) {
        parent->head.info_init();
    }

    while (c != NULL) {
        if (c == parent) {
            return TRUE;
        }
        c = c->head.parent;
    }
    return FALSE;
}

void* fn_80193828(HSD_ClassInfo* i)
{
    extern void* memset(void* dst, int val, u32 size);
    HSD_ClassInfo* info;
    HSD_Class* obj;

    if (!(i->head.flags & 1)) {
        i->head.info_init();
    }

    obj = i->alloc(i);
    if (obj == NULL) {
        return NULL;
    }

    info = i;
    if (!(info->head.flags & 1)) {
        info->head.info_init();
    }

    memset(obj, 0, info->head.obj_size);
    obj->class_info = i;

    if (info->init(obj) < 0) {
        i->destroy(obj);
        return NULL;
    }

    return obj;
}
