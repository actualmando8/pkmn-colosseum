/**
 * @file hsd_hash.h
 * @brief HSD generic hash table.
 *
 * A class-driven hash table: the bucket index and the key comparison are
 * supplied by the concrete class through two extra virtual slots appended to
 * HSD_ClassInfo (getidx @0x3C, keycheck @0x40).
 *
 * Colosseum address range: 0x8019BFE8 (HSD_HashSearch)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_HASH_H
#define HSD_HASH_H

#include "dolphin/types.h"
#include "hsd/hsd_class.h"

typedef struct _HSD_Hash HSD_Hash;

/* ========================================================================= */
/*  Structures                                                               */
/* ========================================================================= */

typedef struct _HSD_HashEntry {
    /* 0x00 */ struct _HSD_HashEntry* next;
    /* 0x04 */ void* key;
    /* 0x08 */ void* value;
} HSD_HashEntry;

typedef struct _HSD_HashClass {
    /* 0x00 */ struct _HSD_HashClassInfo* class_info;
} HSD_HashClass;

typedef struct _HSD_HashClassInfo {
    /* 0x00 */ HSD_ClassInfo parent;
    /* 0x3C */ u32 (*getidx)(HSD_Hash* hash);
    /* 0x40 */ s32 (*keycheck)(HSD_Hash* hash, void* table_key, void* key);
} HSD_HashClassInfo;

struct _HSD_Hash {
    /* 0x00 */ HSD_HashClass parent;
    /* 0x04 */ HSD_HashEntry** table;
    /* 0x08 */ u32 table_size;
};

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void* HSD_HashSearch(HSD_Hash* hash, void* key, s32* success);

#endif /* HSD_HASH_H */
