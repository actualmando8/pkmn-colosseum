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
    if (lbl_8047B228 != NULL) {
        return (HSD_ClassInfo*) HSD_HashSearch((void*) lbl_8047B228, (void*) class_name, NULL);
    }
    return NULL;
}

BOOL fn_80193788(void* info, void* parent)
{
    HSD_ClassInfo* cur;
    HSD_ClassInfo* cls;

    if (info == NULL || parent == NULL) {
        return FALSE;
    }

    cur = info;
    if (!(HSD_CLASS_INFO(info)->head.flags & 1)) {
        cur->head.info_init();
    }
    cls = parent;
    if (!(cls->head.flags & 1)) {
        cls->head.info_init();
    }
    while (cur != NULL) {
        if (cur == cls) {
            return TRUE;
        }
        cur = cur->head.parent;
    }
    return FALSE;
}

void* fn_80193828(HSD_ClassInfo* i)
{
    HSD_ClassInfo* info;
    HSD_Class* cls;

    if (!(i->head.flags & 1)) {
        i->head.info_init();
    }
    cls = i->alloc(i);
    if (cls == NULL) {
        return NULL;
    }
    info = i;
    if (!(info->head.flags & 1)) {
        info->head.info_init();
    }
    memset(cls, 0, info->head.obj_size);
    cls->class_info = i;
    if (info->init(cls) < 0) {
        i->destroy(cls);
        return NULL;
    }
    return cls;
}
