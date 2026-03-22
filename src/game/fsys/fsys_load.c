/**
 * @file fsys_load.c
 * @brief FSYS archive loading, slot management, and subsystem initialization.
 *
 * Contains the master FSYS init (FSYSInit), the slot allocator (FSYSFindSlot),
 * the archive open/mount entry points (FSYSLoadArchive, FSYSLoadArchiveEx),
 * and the load manager init (FSYSInitLoadManager).
 *
 * Address range: 0x8017AC40 - 0x8017B07C, 0x8017D410 - 0x8017D56C,
 *                0x801800F8 - 0x80180320
 *
 * Key structures:
 *   FSYSManager (lbl_80453FEC)  -- singleton managing FSYS state
 *   FSYSSlot[]  (via lbl_8047B1B4) -- per-archive load slots, 0x140 each
 *   File handles (lbl_8047B1B8) -- 100-entry handle table
 */

#include "game/fsys/fsys.h"

/* ===================================================================
 * External SDK / engine functions
 * =================================================================== */

extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* Memory allocation: fn_800E2C04 allocates aligned, returns handle;
 * fn_800E27B0 resolves handle to pointer. */
extern u16  fn_800E2C04(u32 size, u32 alignment);     /* heap alloc */
extern void* fn_800E27B0(u16 handle);                  /* handle -> ptr */

/* DVD / file operations */
extern void fn_800CA968(void* dst, const void* src);   /* string copy (path) */
extern u32  fn_80167F28(const char* path);              /* DVDOpen */
extern u32  fn_80167E5C(u32 fileInfo);                  /* DVDGetLength */
extern void fn_80167E64(u32 fileInfo);                  /* DVDClose */
extern void fn_80167ED0(u32 fileInfo, void* buf, u32 len, u32 offset); /* DVDReadPrio */
extern void fn_80167FA8(u32 param);                     /* DVD subsystem init */
extern void fn_800A7BCC(void);                          /* callback registration */

/* sprintf-like: fn_800C8520 */
extern void fn_800C8520(char* dst, const char* fmt, ...);

/* Load manager internal calls */
extern void fn_8017FDB0(u32 param);                    /* init request queue */
extern void fn_8017FB08(void);                          /* start load thread */
extern void fn_80180B94(u32 count);                     /* init file table */

/* Processing / loading calls used by the load entry points */
extern s32  FSYSProcessEntry(FSYSSlot* slot);
extern void FSYSBeginLoad(FSYSSlot* slot, u32 fileHandle,
                          u32 callbackA, u32 callbackB, u32 callbackC,
                          u32 loadMode);
extern void fn_8017E1D8(FSYSSlot* slot, u32 fileHandle,
                         u32 callbackA, u32 callbackB, u32 callbackC);

/* ===================================================================
 * Global data (BSS / SBSS)
 * =================================================================== */

/* FSYSManager singleton -- lbl_80453FEC, .bss, 0x2C bytes */
FSYSManager gFSYSManager;

/* Pointer to pre-allocated FSYSSlot array -- lbl_8047B1B4, .sbss */
FSYSSlot* gFSYSSlots;

/* Pointer to file handle table (100 entries x 8 bytes) -- lbl_8047B1B8 */
FSYSFileHandle* gFSYSHandleTable;

/* Active handle count -- lbl_8047B1BC */
u32 gFSYSHandleCount;

/* Pointer to loaded TOC data (gsfsys.toc contents) -- lbl_8047B1B0 */
void* gFSYSTocData;

/* LZSS decompression context counts -- lbl_8047B1C8 */
u32 gFSYSDecompCount[2];

/* DVD read buffer pool (2 entries) -- lbl_8047B1C0 */
void* gFSYSDVDBuffers[2];

/* ===================================================================
 * fn_8017AC40: FSYSInit
 *
 * Master initialization of the FSYS subsystem.
 *
 * 1. Stores parameters into the FSYSManager singleton.
 * 2. Allocates the FSYSSlot array (numSlots * 0x140, 32-byte aligned).
 * 3. Allocates the file handle table (0x320 = 100 * 8 bytes).
 * 4. Initializes all handle entries to {-1, 0}.
 * 5. Calls DVD init and callback registration.
 * 6. Zeroes out all slots and sets default field values.
 * 7. Allocates two 0x20000-byte DVD read buffers.
 * 8. Opens "gsfsys.toc", reads the table of contents into memory.
 * 9. Calls FSYSInitLoadManager and init file table.
 * 10. Returns 1.
 * =================================================================== */
s32 FSYSInit(u32 numSlots, u32 param2, u32 param3, u32 param4) {
    FSYSManager* mgr = &gFSYSManager;
    u32 slotArraySize;
    u16 handle;
    u32 i;
    u32 tocFile;
    u32 tocSize;
    u32 tocBufSize;
    u16 tocHandle;

    /* Store configuration into the manager */
    mgr->maxSlots  = numSlots;
    mgr->numEntries = 0;
    mgr->field_24  = 0;
    mgr->field_28  = 1;

    gFSYSDecompCount[0] = 0;

    mgr->field_04  = 0;
    mgr->tocDataPtr = param2;
    mgr->field_08  = 0;
    mgr->field_14  = param3;
    mgr->field_18  = param4;
    mgr->activeSlot = NULL;

    /* Allocate slot array: numSlots * 0x140, rounded up to 32 bytes */
    slotArraySize = (numSlots * FSYS_SLOT_SIZE + 0x1F) & ~0x1F;
    handle = fn_800E2C04(slotArraySize, 0x20);
    if (handle != 0) {
        gFSYSSlots = (FSYSSlot*)fn_800E27B0(handle);
    } else {
        gFSYSSlots = NULL;
    }

    /* Allocate file handle table: 100 entries * 8 bytes = 0x320 */
    handle = fn_800E2C04(0x320, 0x20);
    if (handle != 0) {
        gFSYSHandleTable = (FSYSFileHandle*)fn_800E27B0(handle);
    } else {
        gFSYSHandleTable = NULL;
    }

    /* Initialize all 100 handle entries to unused (-1, 0) */
    gFSYSHandleCount = 0;
    for (i = 0; i < FSYS_MAX_HANDLES; i++) {
        gFSYSHandleTable[i].handleID = -1;
        gFSYSHandleTable[i].userData = 0;
    }

    /* Initialize DVD subsystem and register callbacks */
    fn_80167FA8(numSlots);
    fn_800A7BCC();

    /* Initialize all FSYSSlots to zero and set default field values */
    {
        FSYSSlot* slot = gFSYSSlots;
        for (i = 0; i < mgr->maxSlots; i++) {
            memset(slot, 0, FSYS_SLOT_SIZE);
            slot->archiveHandle = 0;
            slot->archiveData   = NULL;
            slot->padding054    = 0;
            slot->padding068    = 0;
            slot->padding05C    = 0;
            slot->padding058    = 0;
            slot->refCount      = 0;
            slot->padding064    = 0;
            slot->tocBuffer     = NULL;
            slot->padding100    = 0;
            slot++;
        }
    }

    /* Allocate two DVD read buffers of 0x20000 bytes each */
    for (i = 0; i < 2; i++) {
        handle = fn_800E2C04(0x20000, 0x20);
        if (handle != 0) {
            gFSYSDVDBuffers[i] = fn_800E27B0(handle);
        } else {
            gFSYSDVDBuffers[i] = NULL;
        }
    }

    /* Open and read gsfsys.toc */
    {
        char tocPath[0x80];
        fn_800CA968(tocPath, "gsfsys.toc");
        tocFile = fn_80167F28(tocPath);
        tocSize = fn_80167E5C(tocFile);

        /* Allocate buffer for TOC, 32-byte aligned */
        tocBufSize = (tocSize + 0x1F) & ~0x1F;
        tocBufSize = (tocBufSize + 0x1F) & ~0x1F;  /* double-aligned */
        tocHandle = fn_800E2C04(tocBufSize, 0x20);
        if (tocHandle != 0) {
            gFSYSTocData = fn_800E27B0(tocHandle);
        } else {
            gFSYSTocData = NULL;
        }

        /* Read the TOC from disc */
        fn_80167ED0(tocFile, gFSYSTocData, (tocSize + 0x1F) & ~0x1F, 0);
        fn_80167E64(tocFile);
    }

    /* Store TOC data pointer and initialize load systems */
    gFSYSTocData = gFSYSTocData;  /* redundant store in original */
    FSYSInitLoadManager(8, 0x00A00000, 0x00600000);
    fn_80180B94(FSYS_MAX_HANDLES);

    return 1;
}

/* ===================================================================
 * fn_8017D410: FSYSFindSlot
 *
 * Two-pass search of the FSYSSlot array:
 *
 * Pass 1: Search for an active slot that matches the given fileHandle.
 *   - If found and status == LOADED (0x3E8), apply mode-specific logic
 *     (ref counting, etc.) and return.
 *   - If found and status != LOADED, still return based on mode.
 *
 * Pass 2: If no matching active slot, find the first FREE slot
 *   (status == 0), reset its refCount, increment it, and return.
 *
 * Returns NULL if all slots are busy and no match found.
 * =================================================================== */
FSYSSlot* FSYSFindSlot(u32 fileHandle, u32 mode) {
    FSYSSlot* slot;
    u32 i;

    /* Pass 1: look for active slot matching fileHandle */
    slot = gFSYSSlots;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        if (slot->status != FSYS_STATUS_FREE) {
            if (slot->fileHandle == fileHandle) {
                /* Found a matching active slot */
                if (slot->status == FSYS_STATUS_LOADED) {
                    /* Slot is fully loaded */
                    if (mode == 2 || mode == 7) {
                        /* Modes 2 and 7: return NULL (don't reuse) */
                        return NULL;
                    }
                    if (slot->loadMode == 3) {
                        return slot;
                    }
                    slot->reloadFlag = 1;
                    return slot;
                }
                /* Not yet loaded */
                if (mode == 2) {

                } else {
                    if (mode >= 3) {
                        if (mode == 7) {
                            goto found_loaded;
                        }
                        goto unmatched;
                    }
                    /* mode == 0 */
                    if (slot->loadMode == 2) {
                        goto unmatched;
                    }
                    if (slot->loadMode == 7) {
                        goto unmatched;
                    }
                }
                found_loaded:
                slot->refCount++;

            }
        }
        unmatched:
        /* Check if we decided to return this slot */
        /* (The assembly is a bit tangled here; simplified) */
        if (slot->status != FSYS_STATUS_FREE &&
            slot->fileHandle == fileHandle) {
            return slot;
        }
        slot = (FSYSSlot*)((u8*)slot + FSYS_SLOT_SIZE);
    }

    /* Pass 2: find first free slot */
    slot = gFSYSSlots;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        if (slot->status == FSYS_STATUS_FREE) {
            slot->refCount = 0;
            slot->refCount++;
            return slot;
        }
        slot = (FSYSSlot*)((u8*)slot + FSYS_SLOT_SIZE);
    }

    return NULL;
}

/* ===================================================================
 * fn_8017AF6C: FSYSLoadArchive
 *
 * Simple archive load: finds or allocates a slot, stores the request ID,
 * and calls FSYSProcessEntry to begin loading.
 *
 * @param fileHandle  DVD file handle / resource identifier
 * @param requestID   Name hash of the resource to load
 * @return            1 if successfully started, 0 on failure
 * =================================================================== */
s32 FSYSLoadArchive(u32 fileHandle, u32 requestID) {
    FSYSSlot* slot;

    slot = FSYSFindSlot(fileHandle, 3);

    /* Check that the slot's file handle matches */
    if (slot->fileHandle != fileHandle) {
        return 0;
    }

    /* Store request ID and clear callbacks */
    slot->requestID = requestID;
    slot->callbackA = 0;
    slot->callbackB = 0;
    slot->callbackC = 0;

    /* Process the entry */
    if (FSYSProcessEntry(slot)) {
        return 1;
    }
    return 0;
}

/* ===================================================================
 * fn_8017B000: FSYSLoadArchiveEx
 *
 * Extended archive load with user-specified callbacks. Finds a slot,
 * stores the request ID and callbacks, then calls fn_8017E1D8 to
 * begin the load with callback support.
 *
 * @return 1 on success, 0 if no slot available
 * =================================================================== */
s32 FSYSLoadArchiveEx(u32 fileHandle, u32 requestID,
                      u32 callbackA, u32 callbackB, u32 callbackC) {
    FSYSSlot* slot;

    slot = FSYSFindSlot(fileHandle, 3);
    if (slot == NULL) {
        return 0;
    }

    slot->requestID = requestID;
    fn_8017E1D8(slot, fileHandle, callbackA, callbackB, callbackC);

    return 1;
}

/* ===================================================================
 * fn_801800F8: FSYSInitLoadManager
 *
 * Initializes the DVD info pool, decompression pool, and the
 * request queue for asynchronous archive loading.
 *
 * Structure at lbl_80454018:
 *   +0x00: state
 *   +0x04: field_04
 *   +0x08: field_08
 *   +0x0C: field_0C
 *   +0x10: field_10
 *   +0x14: field_14
 *   +0x18: field_18
 *   +0x1C: (padding)
 *   +0x20: pointer to DVDPoolEntry array (0x400 entries x 0x20)
 *   +0x24: DVDPoolEntry.field_04 of first entry (linked list)
 *   +0x28: DVDPoolEntry.field_08 (pool size = 0x400)
 *
 * Also allocates per-request structures (0x44 bytes each).
 * =================================================================== */
/* This structure is at lbl_80454018 */
typedef struct LoadManager {
    u32  state;              /* 0x00 */
    u32  field_04;           /* 0x04 */
    u32  field_08;           /* 0x08 */
    u32  field_0C;           /* 0x0C */
    u32  field_10;           /* 0x10 */
    u32  field_14;           /* 0x14 */
    u32  field_18;           /* 0x18 */
    u32  field_1C;           /* 0x1C */
    void* dvdPool;           /* 0x20: pointer to allocated DVDPoolEntry array */
    void* dvdPoolHead;       /* 0x24: head of free list */
    u32  dvdPoolSize;        /* 0x28: number of pool entries (0x400) */
    u32  dvdPoolFreeCount;   /* 0x2C */
    /* ... followed by request queue at +0x1030 ... */
} LoadManager;

/* lbl_80454018 */
extern LoadManager gLoadManager;

/* lbl_80455070 -- decompression pool */
extern DecompPoolEntry gDecompPool[];

/* lbl_8047B1D4 -- request queue array pointer */
extern void* gFSYSRequestQueue;
/* lbl_8047B1D8 -- max request count */
extern u32 gFSYSMaxRequests;
/* lbl_8047B1D0 -- active request count */
extern u32 gFSYSActiveRequests;

void FSYSInitLoadManager(u32 maxRequests, u32 alignment, u32 poolSize) {
    LoadManager* lm = &gLoadManager;
    u16 handle;
    DVDPoolEntry* pool;
    DVDPoolEntry* entry;
    DecompPoolEntry* dentry;
    u32 i;
    u32 allocSize;
    void* reqArray;

    /* Clear the load manager's active flag */
    lm->state = 0;

    /* Allocate the DVDPoolEntry array: 0x8000 bytes (0x400 * 0x20), 32-aligned */
    allocSize = 0x8000;  /* 0x400 * 0x20 = 0x8000 */
    handle = fn_800E2C04(allocSize, 0x20);
    if (handle != 0) {
        pool = (DVDPoolEntry*)fn_800E27B0(handle);
    } else {
        pool = NULL;
    }

    /* Initialize the load manager fields */
    lm->dvdPool          = pool;
    lm->field_04         = 0;
    lm->field_08         = 0;
    lm->state            = 0;  /* re-store */
    lm->dvdPoolHead      = NULL;
    lm->dvdPoolSize      = FSYS_DVDINFO_COUNT;  /* 0x400 */
    lm->field_10         = 0;
    lm->field_14         = 0;
    lm->field_0C         = 0;

    /* Initialize all pool entries */
    entry = pool;
    entry->callback = NULL;
    entry->next = (void*)((u8*)pool + 0x20);
    lm->dvdPoolHead = pool;

    for (i = 0; i < FSYS_DVDINFO_COUNT; i++) {
        entry->callback = NULL;
        entry->next     = NULL;
        entry->buffer   = NULL;
        entry->field_00 = 0;
        entry->field_1C = 0;
        entry = (DVDPoolEntry*)((u8*)entry + FSYS_DVDINFO_SIZE);
    }

    /* Set up the free list linkage for the pool */
    pool = (DVDPoolEntry*)lm->dvdPool;
    pool->callback = NULL;
    pool->next = (void*)((u8*)pool + FSYS_DVDINFO_SIZE);
    lm->dvdPoolHead = pool;

    /* Initialize decompression pool (0x1000 entries at lbl_80455070) */
    dentry = gDecompPool;
    for (i = 0; i < FSYS_DECOMP_POOL_COUNT; i++) {
        dentry->callback = NULL;
        dentry = (DecompPoolEntry*)((u8*)dentry + FSYS_DECOMP_ENTRY_SIZE);
    }

    /* Allocate per-request structures: maxRequests * 0x44 each, 32-aligned */
    gFSYSMaxRequests = maxRequests;
    allocSize = (maxRequests * FSYS_QUEUE_ENTRY_SIZE + 0x1F) & ~0x1F;
    handle = fn_800E2C04(allocSize, 0x20);
    if (handle != 0) {
        reqArray = fn_800E27B0(handle);
    } else {
        reqArray = NULL;
    }
    gFSYSRequestQueue = reqArray;

    /* Initialize each request entry */
    {
        u8* req = (u8*)gFSYSRequestQueue;
        for (i = 0; i < gFSYSMaxRequests; i++) {
            *(u32*)(req + 0x20) = 0;
            *(u32*)(req + 0x24) = 0;
            *(u32*)(req + 0x38) = 0;
            *(u32*)(req + 0x3C) = 0;
            *(u32*)(req + 0x40) = i;   /* store index */
            req += FSYS_QUEUE_ENTRY_SIZE;
        }
    }

    /* Initialize the request queue state */
    gFSYSActiveRequests = 0;

    /* Start the load processing threads */
    fn_8017FDB0(poolSize);
    fn_8017FB08();
}
