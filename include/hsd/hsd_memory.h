/**
 * @file hsd_memory.h
 * @brief HSD memory allocation interface.
 *
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_MEMORY_H
#define HSD_MEMORY_H

#include "dolphin/types.h"

void HSD_Free(void* ptr);
void* HSD_MemAlloc(s32 size);

#endif /* HSD_MEMORY_H */
