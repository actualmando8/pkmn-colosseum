/**
 * @file hsd_id.h
 * @brief HSD ID table (u32 id -> void* data hash map).
 *
 * A fixed 101-bucket separate-chaining hash table. Entries are allocated
 * from a dedicated HSD_ObjAllocData pool.
 *
 * Colosseum address range: 0x8019C0F8 - 0x8019C3C4
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_ID_H
#define HSD_ID_H

#include "dolphin/types.h"

/* ========================================================================= */
/*  Structures                                                               */
/* ========================================================================= */

typedef struct _IDEntry {
    /* 0x00 */ struct _IDEntry* next;
    /* 0x04 */ u32 id;
    /* 0x08 */ void* data;
} IDEntry;

typedef struct _HSD_IDTable {
    /* 0x000 */ IDEntry* table[101];
} HSD_IDTable;

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void* HSD_IDGetAllocData(void);
void HSD_IDInitAllocData(void);
void HSD_IDSetup(void);
void HSD_IDInsertToTable(HSD_IDTable* table, u32 id, void* data);
void HSD_IDRemoveByIDFromTable(HSD_IDTable* table, u32 id);
void* HSD_IDGetDataFromTable(HSD_IDTable* table, u32 id, s32* success);
void _HSD_IDForgetMemory(void);

#endif /* HSD_ID_H */
