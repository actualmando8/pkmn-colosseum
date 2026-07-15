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
