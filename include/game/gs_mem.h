/**
 * @file gs_mem.h
 * @brief GSmem -- Genius Sonority custom memory allocator for Pokemon Colosseum.
 *
 * GSmem is a slab/block allocator layered on top of the Dolphin OS arena.
 * The game carves a ~14.5 MB region from OSGetArenaLo() and manages it with
 * a free-list of variably-sized blocks.  Each allocation is tracked through a
 * 16-byte "alloc entry" (GSmemEntry) that stores a handle, a pointer to the
 * allocated data, and the block size.  Handles are 16-bit indices (1-based)
 * returned to callers instead of raw pointers.
 *
 * Source file string: "GCN_Mem_Alloc.c"
 * Debug string:       "GSmem: Init OK, using area %08Xh -> %08Xh"
 * Address range:      0x800E202C - 0x800E3604 (approx.)
 */
#ifndef GS_MEM_H
#define GS_MEM_H

#include "dolphin/types.h"

/* -----------------------------------------------------------------------
 * Block header -- sits at the start of every free block in the heap.
 * Size: 12 bytes (0x0C) minimum.
 * ----------------------------------------------------------------------- */
typedef struct GSmemBlock {
    /* 0x00 */ struct GSmemBlock* prev;  /* previous free block (or NULL) */
    /* 0x04 */ struct GSmemBlock* next;  /* next free block (or NULL)     */
    /* 0x08 */ u32                size;  /* usable size of this block     */
} GSmemBlock;

/* -----------------------------------------------------------------------
 * Allocation entry -- 16-byte descriptor in the entry table.
 * The entry table grows downward from the end of the heap.
 * ----------------------------------------------------------------------- */
typedef struct GSmemEntry {
    /* 0x00 */ u16 handle;    /* self-referencing handle (1-based), 0 = free */
    /* 0x02 */ u16 refCount;  /* reference count / lock flag                 */
    /* 0x04 */ void* data;    /* pointer to the allocated memory region      */
    /* 0x08 */ u32   size;    /* size of the allocation in bytes             */
    /* 0x0C */ u16   align;   /* alignment that was requested                */
    /* 0x0E */ u16   pad;
} GSmemEntry;

/* -----------------------------------------------------------------------
 * Allocation strategy constants (stored in gsMemDefaultHeap / lbl_8047AB2C)
 * ----------------------------------------------------------------------- */
#define GSMEM_FIT_FIRST  0   /* first-fit (default) */
#define GSMEM_FIT_BEST   1   /* best-fit (smallest block that fits) */
#define GSMEM_FIT_WORST  2   /* worst-fit (largest block) */

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

/**
 * GSmemInit -- Initialise the GSmem heap.
 *
 * @param heapId  Heap index (always 0 for the main game heap).
 * @param start   Start address of the memory region.
 * @param end     End address of the memory region (exclusive).
 *
 * Aligns start up and end down to 32-byte boundaries, creates a single
 * free block covering the full region, and initialises the entry table
 * at the top of the region.
 *
 * Corresponds to fn_800E3568.
 */
void GSmemInit(u32 heapId, void* start, void* end);

/**
 * GSmemSetDefaultHeap -- Set the active allocation heap.
 *
 * @param heapId  Heap index to make current.
 *
 * Corresponds to fn_800E3560.
 */
void GSmemSetDefaultHeap(u32 heapId);

/**
 * GSmemAlloc -- Allocate a block of memory (first-fit / address-ordered).
 *
 * @param alignment  Required alignment (power-of-two, rounded to 32).
 * @param size       Number of bytes to allocate.
 * @return           16-bit handle (1-based), or 0 on failure.
 *
 * Corresponds to fn_800E2C04.
 */
u16 GSmemAlloc(u32 alignment, u32 size);

/**
 * GSmemAllocTail -- Allocate from the end of a free block.
 *
 * @param alignment  Required alignment.
 * @param size       Number of bytes.
 * @return           16-bit handle, or 0 on failure.
 *
 * Corresponds to fn_800E2B00.
 */
u16 GSmemAllocTail(u32 alignment, u32 size);

/**
 * GSmemAllocRaw -- Low-level alloc used by both GSmemAlloc and GSmemAllocTail.
 *
 * @param size       Rounded allocation size (including header padding).
 * @return           Handle, or 0 on failure.
 *
 * Corresponds to fn_800E3534.
 */
u16 GSmemAllocRaw(u32 size);

/**
 * GSmemFree -- Release a previously allocated handle.
 *
 * @param handle     16-bit handle returned by GSmemAlloc/AllocTail.
 *
 * Merges the freed block with adjacent free blocks when possible.
 * Validates guard bytes in debug builds.
 *
 * Corresponds to fn_800E209C.
 */
void GSmemFree(u16 handle);

/**
 * GSmemLock -- Resolve a handle to a raw pointer.
 *
 * @param handle     16-bit handle.
 * @return           Pointer to the usable data, or NULL on invalid handle.
 *
 * Increments the entry's reference count.
 *
 * Corresponds to fn_800E24B0.
 */
void* GSmemLock(u16 handle);

/**
 * GSmemGetPtr -- Resolve a handle to a pointer without locking.
 *
 * @param handle     16-bit handle.
 * @return           Pointer to the usable data, or NULL if invalid.
 *
 * Corresponds to fn_800E27B0.
 */
void* GSmemGetPtr(u16 handle);

/**
 * GSmemFindHandle -- Look up the handle for a raw pointer.
 *
 * @param ptr        Pointer previously returned by GSmemLock/GetPtr.
 * @return           The owning handle, or 0 if not found.
 *
 * Corresponds to fn_800E202C.
 */
u16 GSmemFindHandle(void* ptr);

/**
 * GSmemGetFreeSize -- Return the total number of free bytes in the heap.
 *
 * Corresponds to fn_800E0DDC.
 */
u32 GSmemGetFreeSize(void);

/**
 * GSmemIsInited -- Returns 1 if the heap has been initialised.
 *
 * Corresponds to fn_800E2AF8.
 */
u32 GSmemIsInited(void);

#endif /* GS_MEM_H */
