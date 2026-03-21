/**
 * @file fsys_file.c
 * @brief FSYS file lookup, entry processing, and archive load state machine.
 *
 * Contains the core file lookup/check functions (FSYSCheckFileLoaded,
 * FSYSRequestFile), the per-entry processor (FSYSProcessEntry), the
 * multi-stage load state machine (FSYSBeginLoad), and the cache lookup
 * (FSYSCacheLookup).
 *
 * Address range: 0x8017B07C - 0x8017B1CC, 0x8017E30C - 0x8017F108,
 *                0x8017F794 - 0x8017F928
 */

#include "game/fsys/fsys.h"

/* ===================================================================
 * External SDK / engine functions
 * =================================================================== */

extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* Memory allocation */
extern u16  fn_800E2C04(u32 size, u32 alignment);
extern u16  fn_800E2B00(u32 size, u32 alignment);
extern void* fn_800E27B0(u16 handle);
extern void  fn_800E24B0(u16 handle);
extern void  fn_800E209C(u16 handle);
extern u16   fn_800E202C(void* ptr);

/* DVD / file operations */
extern s32   fn_800CA968(void* dst, const void* src);
extern u32   fn_80167F28(const char* path);
extern u32   fn_80167E5C(u32 fileInfo);
extern void  fn_80167E64(u32 fileInfo);
extern u8    fn_80167EF8(void* path);
extern void  fn_80167ED0(u32 fileInfo, void* buf, u32 len, u32 offset);

/* DMA / cache */
extern void  DCFlushRange(void* addr, u32 len);

/* sprintf-like */
extern void  fn_800C8520(char* dst, const char* fmt, ...);

/* Memory read (heap-to-ptr with DMA) */
extern void* fn_800F9418(u32 size, u32 priority, u32 alignment, u32 fileID, u32 param);
extern void* fn_800F9318(u32 fileHandle, u32 fileID);

/* Slot search */
extern FSYSSlot* FSYSFindSlot(u32 fileHandle, u32 mode);

/* Decompression */
extern void FSYSDecompressLZSS(void* dst, const void* src, u32 size);

/* Extended load function (defined in fsys_load.c) */
extern void fn_8017E1D8(FSYSSlot* slot, u32 fileHandle,
                         u32 callbackA, u32 callbackB, u32 callbackC);

/* DMA copy (defined in DVD layer) */
extern void fn_80180320(void* dst, void* src, u32 size);

/* ===================================================================
 * External globals
 * =================================================================== */

extern FSYSManager gFSYSManager;
extern FSYSSlot*   gFSYSSlots;
extern void*       gFSYSTocData;

/* lbl_80478C48 -- DVD info count / pool state */
extern u32 gDVDPoolState;

/* lbl_8036C2A0 -- DecompPoolEntry base (used for cache searches) */
extern DecompPoolEntry gDecompPoolBase[];

/* lbl_80454038 -- head of the DVD cache linked list */
extern void* gDVDCacheHead;

/* lbl_80453FDC -- LZSS decompression context */
extern FSYSDecompContext gLZSSContext;

/* lbl_80452FC8 -- LZSS sliding window */
extern u8 gLZSSWindow[];

/* ===================================================================
 * fn_8017B07C: FSYSCheckFileLoaded
 *
 * Checks whether a specific file (identified by nameHash) within the
 * archive loaded at fileHandle is fully decompressed and available.
 *
 * Iterates through all entries in the archive's TOC. For each entry,
 * resolves the file entry through two levels of indirection:
 *   archive_base + offset_table_offset -> string_table_offset -> entry
 *
 * If the entry's nameHash matches and its sub-entry state == 6 (complete),
 * returns 1.
 *
 * @param fileHandle  The DVD file handle identifying the archive
 * @param nameHash    The name hash / resource ID of the file to check
 * @return            1 if the file is fully loaded, 0 otherwise
 * =================================================================== */
s32 FSYSCheckFileLoaded(u32 fileHandle, u32 nameHash) {
    FSYSSlot* slot;
    s32 found = 0;
    u32 i;

    slot = FSYSFindSlot(fileHandle, 3);
    if (slot == NULL) {
        return found;
    }

    for (i = 0; i < slot->numEntries; i++) {
        void* archBase = slot->archiveData;
        FSYSFileEntry* entry;

        if (archBase != NULL) {
            /* Two-level indirection to find the file entry:
             * 1. stringTableOff points to an offset table
             * 2. That table points to per-entry offset arrays
             * 3. Index into the array gives the entry offset */
            u32 stringTableOff = *(u32*)((u8*)archBase + 0x18);
            u32* offTable = (u32*)((u8*)archBase + stringTableOff);
            u32  strOff   = offTable[0];
            u32* entryTable = (u32*)((u8*)archBase + strOff);
            u32  entryOff  = entryTable[i];
            entry = (FSYSFileEntry*)((u8*)archBase + entryOff);
        } else {
            entry = NULL;
        }

        if (entry != NULL) {
            FSYSSubEntry* sub = (FSYSSubEntry*)&entry->subEntry[0];

            if (entry->nameHash == nameHash) {
                if (sub->state == 6) {
                    found = 1;
                }
            }
        }
    }

    return found;
}

/* ===================================================================
 * fn_8017B13C: FSYSRequestFile
 *
 * Requests loading of a file from an already-loaded archive.
 * Finds the slot, stores the request ID, then calls the extended
 * load function (fn_8017E1D8) with NULL callbacks.
 *
 * @param fileHandle  DVD file handle
 * @param requestID   Resource name hash to load
 * @return            1 on success, 0 if no slot available
 * =================================================================== */
s32 FSYSRequestFile(u32 fileHandle, u32 requestID) {
    FSYSSlot* slot;

    slot = FSYSFindSlot(fileHandle, 3);
    if (slot == NULL) {
        return 0;
    }

    slot->requestID = requestID;
    fn_8017E1D8(slot, fileHandle, 0, 0, 0);

    return 1;
}

/* Note: numEntries is at offset 0x0C in the FSYSSlot struct, now a named field. */

/* ===================================================================
 * fn_8017E30C: FSYSProcessEntry
 *
 * Core archive entry processor. After an archive is loaded into memory,
 * this function processes each file entry within it:
 *
 * 1. Searches the archive's TOC for an entry matching slot->requestID.
 * 2. If found, checks the compression flag (entry->flags bit 0).
 * 3. For compressed entries:
 *    a. Allocates a temporary buffer for the compressed data.
 *    b. Reads compressed data via DMA (fn_80180320).
 *    c. Copies the 16-byte LZSS header to gLZSSContext.
 *    d. Clears the LZSS sliding window (4078 bytes).
 *    e. Calls FSYSDecompressLZSS to decompress in-place.
 *    f. Flushes the D-cache.
 *    g. Frees the temporary compressed buffer.
 * 4. For uncompressed entries:
 *    a. Allocates a buffer for the raw data.
 *    b. Reads data directly via DMA.
 *    c. Flushes the D-cache.
 * 5. Looks up the decompression pool (lbl_8036C2A0) for a callback
 *    and calls it if present.
 *
 * @param slot  The FSYSSlot containing the archive and request info
 * @return      1 on success, 0 if entry not found or allocation failed
 * =================================================================== */
s32 FSYSProcessEntry(FSYSSlot* slot) {
    u32 i;
    s32 found = 0;
    FSYSFileEntry* fileEntry = NULL;
    FSYSSubEntry* subEntry = NULL;
    void* allocatedBuf;
    u32 isCompressed;

    /* Search the archive TOC for the requested entry */
    if (slot->archiveData != NULL) {
        u32 numEntries = slot->numEntries;
        for (i = 0; i < numEntries; i++) {
            void* archBase = slot->archiveData;

            if (archBase != NULL) {
                u32 stringTableOff = *(u32*)((u8*)archBase + 0x18);
                u32* offTable = (u32*)((u8*)archBase + stringTableOff);
                u32  strOff   = offTable[0];
                u32* entryTable = (u32*)((u8*)archBase + strOff);
                u32  entryOff  = entryTable[i];
                fileEntry = (FSYSFileEntry*)((u8*)archBase + entryOff);
            } else {
                fileEntry = NULL;
            }

            if (fileEntry != NULL) {
                /* Compare entry nameHash against slot requestID */
                if (fileEntry->nameHash == slot->requestID) {
                    subEntry = (FSYSSubEntry*)((u8*)fileEntry + 0x28);
                    found = 1;
                    break;
                }
            }
        }
    }

    if (!found) {
        return 0;
    }

    /* Try to find the data in the cache first */
    allocatedBuf = FSYSCacheLookup(slot->fileHandle,
                                    fileEntry->groupID,
                                    fileEntry->nameHash);
    if (allocatedBuf == NULL) {
        return 0;
    }

    /* Check compression flag (bit 0 of entry->flags) */
    isCompressed = fileEntry->flags & 1;

    if (isCompressed) {
        /* ===== Compressed entry path ===== */
        u32 decompSize = fileEntry->decompressedSize;
        u32 allocSize;
        u16 handle;
        DecompPoolEntry* poolEntry;
        void* compBuf;

        /* Allocate temporary buffer for compressed data */
        allocSize = (decompSize + 0x1F) & ~0x1F;
        handle = fn_800E2B00(allocSize, 0x20);
        if (handle != 0) {
            compBuf = fn_800E27B0(handle);
        } else {
            compBuf = NULL;
        }
        subEntry->buffer = compBuf;

        /* Read compressed data from DVD into temporary buffer */
        fn_80180320(subEntry->buffer, allocatedBuf, decompSize);

        /* Flush the D-cache for the compressed data */
        DCFlushRange(subEntry->buffer, decompSize);

        /* Save compressed data pointer and clear sub-entry buffer */
        {
            void* compData = subEntry->buffer;
            subEntry->buffer = NULL;

            /* Copy the 16-byte LZSS header from the compressed data */
            memcpy(&gLZSSContext, compData, LZSS_HEADER_SKIP);

            /* Clear the LZSS sliding window */
            {
                u32 j;
                for (j = 0; j < LZSS_WINDOW_SIZE; j++) {
                    gLZSSWindow[j] = 0;
                }
            }

            /* Decompress: output goes to allocatedBuf location */
            FSYSDecompressLZSS(allocatedBuf, compData, gLZSSContext.decompSize);

            /* Flush D-cache for decompressed data */
            DCFlushRange(allocatedBuf, gLZSSContext.decompSize);
        }

        /* Free the temporary compressed data buffer handle */
        {
            u16 tmpHandle = fn_800E202C(compBuf);
            if (tmpHandle != 0) {
                fn_800E24B0(tmpHandle);
                fn_800E209C(tmpHandle);
            }
        }

        /* Look up decompression pool for a completion callback */
        {
            u32 j;
            DecompPoolEntry* dp = gDecompPoolBase;
            poolEntry = NULL;

            for (j = 0; j < gDVDPoolState; j++) {
                if (dp->fileID == fileEntry->groupID) {
                    poolEntry = dp;
                    break;
                }
                dp = (DecompPoolEntry*)((u8*)dp + FSYS_DECOMP_ENTRY_SIZE);
            }

            if (poolEntry == NULL) {
                poolEntry = NULL; /* redundant, matches original */
            }
        }

        /* If a decompression callback exists, invoke it */
        if (poolEntry != NULL && poolEntry->callback != NULL) {
            typedef void* (*DecompCallback)(u32 fileHandle, u32 fileID, u32 compSize);
            DecompCallback cb = (DecompCallback)poolEntry->callback;
            allocatedBuf = cb(slot->fileHandle, fileEntry->nameHash,
                             fileEntry->compressedSize);
        }
    } else {
        /* ===== Uncompressed entry path ===== */
        DecompPoolEntry* poolEntry;
        u32 uncompSize = fileEntry->decompressedSize;

        /* Search the decomp pool for a matching entry with callback */
        {
            u32 j;
            DecompPoolEntry* dp = gDecompPoolBase;
            poolEntry = NULL;

            for (j = 0; j < gDVDPoolState; j++) {
                if (dp->fileID == fileEntry->groupID) {
                    poolEntry = dp;
                    break;
                }
                dp = (DecompPoolEntry*)((u8*)dp + FSYS_DECOMP_ENTRY_SIZE);
            }
        }

        /* If a callback exists, use it to allocate/process the buffer */
        if (poolEntry != NULL && poolEntry->callback != NULL) {
            typedef void* (*AllocCallback)(u32 fileHandle, u32 fileID, u32 size);
            AllocCallback cb = (AllocCallback)poolEntry->callback;
            allocatedBuf = cb(slot->fileHandle, fileEntry->nameHash,
                             fileEntry->compressedSize);
        } else {
            /* No callback; allocate buffer from heap */
            u32 allocSize = (uncompSize + 0x1F) & ~0x1F;
            allocatedBuf = (void*)fn_800F9418(allocSize,
                                               slot->fileHandle,
                                               fileEntry->nameHash,
                                               0, 0);
        }
    }

    /* Store the final buffer pointer */
    subEntry->buffer = allocatedBuf;
    subEntry->buffer = allocatedBuf;  /* double-store in original */

    if (subEntry->buffer == NULL) {
        /* Allocation failed */
        slot->status = FSYS_STATUS_ERROR;
        subEntry->state = 7;
        return 1;
    }

    /* Read uncompressed data (or already have decompressed data) */
    if (!isCompressed) {
        fn_80180320(subEntry->buffer, allocatedBuf, fileEntry->decompressedSize);
        DCFlushRange(subEntry->buffer, fileEntry->decompressedSize);
    }

    /* Look up decomp pool again for a post-read callback */
    {
        u32 j;
        DecompPoolEntry* dp = gDecompPoolBase;
        DecompPoolEntry* poolEntry2 = NULL;

        for (j = 0; j < gDVDPoolState; j++) {
            if (dp->fileID == fileEntry->groupID) {
                poolEntry2 = dp;
                break;
            }
            dp = (DecompPoolEntry*)((u8*)dp + FSYS_DECOMP_ENTRY_SIZE);
        }

        if (poolEntry2 != NULL) {
            /* Flush D-cache and invoke the completion callback */
            void* readResult = fn_800F9318(slot->fileHandle, fileEntry->nameHash);
            if (readResult != NULL) {
                if (isCompressed) {
                    DCFlushRange(readResult, fileEntry->compressedSize);
                } else {
                    DCFlushRange(readResult, fileEntry->decompressedSize);
                }
            }

            if (poolEntry2->callback != NULL) {
                typedef void* (*PostCallback)(u32 fh, u32 id, u32 sz);
                PostCallback cb = (PostCallback)poolEntry2->callback;
                if (isCompressed) {
                    cb(slot->fileHandle, fileEntry->nameHash,
                       fileEntry->compressedSize);
                } else {
                    cb(slot->fileHandle, fileEntry->nameHash,
                       fileEntry->decompressedSize);
                }
            }
        }
    }

    return 1;
}

/* ===================================================================
 * fn_8017EB6C: FSYSBeginLoad
 *
 * Implements the FSYS archive load state machine. This function is
 * called to initiate or continue loading an archive, and handles
 * state transitions:
 *
 *   PENDING (0x1F4) or FREE (0) -> Search TOC, format filename,
 *     check if another load is active, set status to LOADING/READING.
 *
 *   LOADED (0x3E8) -> re-entry with mode 3: resolve file within
 *     the already-loaded archive using the TOC hash table.
 *
 * The filename is constructed using sprintf("%s.fsys", name) where
 * name is resolved from the TOC entry table based on the fileHandle.
 *
 * @param slot       Target FSYSSlot
 * @param fileHandle DVD file handle / resource ID
 * @param callbackA  Completion callback A (stored at slot+0x134)
 * @param callbackB  Completion callback B (stored at slot+0x138)
 * @param callbackC  Completion callback C (stored at slot+0x13C)
 * @param loadMode   Load mode (stored at slot+0x4C)
 * =================================================================== */
void FSYSBeginLoad(FSYSSlot* slot, u32 fileHandle,
                   u32 callbackA, u32 callbackB, u32 callbackC,
                   u32 loadMode) {
    FSYSManager* mgr = &gFSYSManager;
    u32 status = slot->status;
    void* tocData;
    u32 numTocEntries;
    u32 i;
    u32* tocEntryPtr;
    char* tocName;
    char nameBuf[0x80];

    /* Handle current status */
    if (status == FSYS_STATUS_PENDING) {
        goto state_loaded;
    } else if (status >= FSYS_STATUS_PENDING) {
        if (status == FSYS_STATUS_LOADED) {
            goto state_loaded;
        }
        return; /* unknown state */
    } else if (status == FSYS_STATUS_FREE) {
        /* Fall through to initial load */
    } else {
        return; /* busy */
    }

    /* Initialize the slot for a new load */
    slot->archiveHandle = 0;
    slot->archiveData   = NULL;
    slot->archiveSize   = 0;
    slot->callbackA     = callbackA;
    slot->callbackB     = callbackB;
    slot->callbackC     = callbackC;
    slot->fileHandle    = fileHandle;
    slot->fileIndex     = 0;
    slot->loadMode      = loadMode;
    slot->reloadFlag    = 0;
    slot->padding100    = 0;

    /* Search the TOC for the matching entry */
    tocData = gFSYSTocData;
    numTocEntries = *(u32*)((u8*)tocData + 0x08);
    tocEntryPtr = (u32*)((u8*)tocData + *(u32*)((u8*)tocData + 0x10));
    tocName = NULL;

    for (i = 0; i < numTocEntries; i++) {
        if (tocEntryPtr[0] == fileHandle) {
            /* Found: resolve the name string */
            u32 nameOff = tocEntryPtr[1];
            tocName = (char*)((u8*)tocData + nameOff);
            break;
        }
        tocEntryPtr = (u32*)((u8*)tocEntryPtr + 8);
    }

    /* Format the filename as "%s.fsys" */
    fn_800C8520(nameBuf, "%s.fsys", tocName);
    fn_800CA968(slot->filename, nameBuf);

    /* Check if another slot is already actively loading */
    if (mgr->activeSlot != NULL) {
        if (slot != mgr->activeSlot) {
            /* Queue this load -- set status to PENDING */
            slot->status = FSYS_STATUS_PENDING;
            return;
        }
    }

    /* Mark this slot as the active loader */
    mgr->activeSlot  = slot;
    mgr->currentSlot = slot;

    /* Apply status based on load mode */
    if (loadMode >= 4) {
        if (loadMode == 7) {
            slot->status = FSYS_STATUS_LOADING;
        } else {
            return;
        }
    } else if (loadMode == 1) {
        slot->status = FSYS_STATUS_READING;
    } else if (loadMode == 0) {
        slot->status = FSYS_STATUS_LOADING;
    } else {
        return;
    }
    return;

state_loaded:
    /* Re-entry: archive is loaded, process a specific file */
    if (loadMode != 3) {
        goto state_check_mode;
    }

    /* Mode 3: resolve file within loaded archive */
    slot->archiveHandle = 0;
    slot->archiveSize   = 0;
    slot->callbackA     = callbackA;
    slot->callbackB     = callbackB;
    slot->callbackC     = callbackC;
    slot->fileHandle    = fileHandle;
    slot->fileIndex     = 0;
    slot->loadMode      = loadMode;
    slot->reloadFlag    = 1;

    /* Search TOC again for the entry */
    tocData = gFSYSTocData;
    numTocEntries = *(u32*)((u8*)tocData + 0x08);
    tocEntryPtr = (u32*)((u8*)tocData + *(u32*)((u8*)tocData + 0x10));
    tocName = NULL;

    for (i = 0; i < numTocEntries; i++) {
        if (tocEntryPtr[0] == fileHandle) {
            u32 nameOff = tocEntryPtr[1];
            tocName = (char*)((u8*)tocData + nameOff);
            break;
        }
        tocEntryPtr = (u32*)((u8*)tocEntryPtr + 8);
    }

    /* Format filename and set up for re-load */
    fn_800C8520(nameBuf, "%s.fsys", tocName);
    fn_800CA968(slot->filename, nameBuf);

    /* Check if loading is blocked */
    if (mgr->activeSlot != NULL) {
        slot->status = FSYS_STATUS_PENDING;
        return;
    }

    mgr->activeSlot  = slot;
    mgr->currentSlot = slot;

state_check_mode:
    if (loadMode >= 4) {
        if (loadMode == 7) {
            slot->status = FSYS_STATUS_ERROR;
        } else {
            return;
        }
    } else if (loadMode == 1) {
        slot->status = FSYS_STATUS_ERROR;
    } else {
        slot->status = FSYS_STATUS_ERROR;
    }
    return;
}

/* ===================================================================
 * fn_8017F794: FSYSCacheLookup
 *
 * Searches the DVD cache linked list (rooted at lbl_80454038) for a
 * previously loaded file matching the given (fileHandle, groupID,
 * nameHash) triple.
 *
 * The cache is a singly-linked list of nodes, each containing:
 *   +0x00: data pointer (returned on match)
 *   +0x04: (padding)
 *   +0x08: next pointer
 *   +0x10: fileHandle
 *   +0x14: groupID
 *   +0x18: nameHash
 *
 * @param fileHandle  DVD file handle
 * @param groupID     Archive group ID
 * @param nameHash    File resource ID
 * @return            Cached data pointer, or NULL if not in cache
 * =================================================================== */
void* FSYSCacheLookup(u32 fileHandle, u32 groupID, u32 nameHash) {
    typedef struct CacheNode {
        void*  data;         /* 0x00 */
        u32    pad04;        /* 0x04 */
        struct CacheNode* next; /* 0x08 */
        u32    pad0C;        /* 0x0C */
        u32    fileHandle;   /* 0x10 */
        u32    groupID;      /* 0x14 */
        u32    nameHash;     /* 0x18 */
        u32    refCount;     /* 0x1C */
    } CacheNode;

    CacheNode* node = (CacheNode*)gDVDCacheHead;

    while (node != NULL) {
        if (node->fileHandle == fileHandle &&
            node->groupID == groupID &&
            node->nameHash == nameHash) {
            return node->data;
        }
        node = node->next;
    }

    return NULL;
}

/* fn_80180320 is declared at the top of this file */

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 18 functions matched
 * =================================================================== */

/* Address: 0x8017BFE8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017BFE8(void) { return 0; }

/* Address: 0x8017BFF0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017BFF0(void) { return 0; }

/* Address: 0x8017BFF8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017BFF8(void) { return 0; }

/* Address: 0x8017C000 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C000(void) { return 1; }

/* Address: 0x8017C394 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C394(void) { return 1; }

/* Address: 0x8017C568 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C568(void) { return 1; }

/* Address: 0x8017C570 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C570(void) { return 1; }

/* Address: 0x8017C578 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C578(void) { return 0; }

/* Address: 0x8017C590 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C590(void) { return 1; }

/* Address: 0x8017C598 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C598(void) { return 1; }

/* Address: 0x8017C5B0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C5B0(void) { return 1; }

/* Address: 0x8017C88C | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C88C(void) { return 1; }

/* Address: 0x8017C8C0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C8C0(void) { return 1; }

/* Address: 0x8017C8F4 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017C8F4(void) { return 1; }

/* Address: 0x8017CEC8 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017CEC8(void) { return 1; }

/* Address: 0x8017CED0 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017CED0(void) { return 1; }

/* Address: 0x8017D400 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017D400(void) { return 1; }

/* Address: 0x8017D408 | Size: 0x8 | Pattern: return_constant */
u32 fn_8017D408(void) { return 1; }

/* ===================================================================
 * NEWLY DECOMPILED: FSYS archive operations and slot management
 *
 * These functions cover the FSYS slot lifecycle, archive opening/
 * closing, file data retrieval, and load manager operations.
 * =================================================================== */

/* fn_8017A5FC | FSYSGetSlotCount | Size: 0x28 */
u32 fn_8017A5FC(void) {
    return gFSYSManager.maxSlots;
}

/* fn_8017A624 | FSYSGetSlotByIndex | Size: 0x1F0 */
FSYSSlot* fn_8017A624(u32 index) {
    if (index >= gFSYSManager.maxSlots) { return NULL; }
    if (gFSYSSlots == NULL) { return NULL; }
    return &gFSYSSlots[index];
}

/* fn_8017A814 | FSYSGetSlotStatus | Size: 0x148 */
u32 fn_8017A814(u32 index) {
    FSYSSlot* slot = fn_8017A624(index);
    if (slot == NULL) { return FSYS_STATUS_FREE; }
    return slot->status;
}

/* fn_8017A95C | FSYSGetSlotArchiveData | Size: 0x148 */
void* fn_8017A95C(u32 index) {
    FSYSSlot* slot = fn_8017A624(index);
    if (slot == NULL) { return NULL; }
    return slot->archiveData;
}

/* fn_8017AAA4 | FSYSGetSlotInfo | Size: 0x18C */
s32 fn_8017AAA4(u32 index, void* outInfo) {
    FSYSSlot* slot = fn_8017A624(index);
    if (slot == NULL) { return 0; }
    if (outInfo != NULL) {
        u32* info = (u32*)outInfo;
        info[0] = slot->status;
        info[1] = slot->numEntries;
        info[2] = slot->totalDecompSize;
        info[3] = slot->archiveSize;
        info[4] = slot->refCount;
    }
    return 1;
}

/* fn_8017AC30 | FSYSIsSlotFree | Size: 0x10 */
BOOL fn_8017AC30(FSYSSlot* slot) {
    return (slot->status == FSYS_STATUS_FREE) ? TRUE : FALSE;
}

/* fn_8017B1AC | FSYSReleaseFile | Size: 0x20 */
void fn_8017B1AC(u32 fileHandle) {
    FSYSSlot* slot = FSYSFindSlot(fileHandle, 1);
    if (slot == NULL) { return; }
    if (slot->refCount > 0) { slot->refCount--; }
    if (slot->refCount == 0) { slot->status = FSYS_STATUS_FREE; }
}

/* fn_8017B1CC | FSYSGetFileData | Size: 0x100 */
void* fn_8017B1CC(u32 fileHandle, u32 nameHash) {
    u32 i;
    u32 maxSlots = gFSYSManager.maxSlots;
    for (i = 0; i < maxSlots; i++) {
        FSYSSlot* slot = &gFSYSSlots[i];
        if (slot->status == FSYS_STATUS_FREE) { continue; }
        if (slot->fileHandle != fileHandle) { continue; }
        if (slot->status < FSYS_STATUS_LOADED) { continue; }
        if (slot->archiveData != NULL) {
            u8* ad = (u8*)slot->archiveData;
            u32 j;
            for (j = 0; j < slot->numEntries; j++) {
                FSYSFileEntry* entry = (FSYSFileEntry*)(ad + j * 0x28);
                if (entry->nameHash == nameHash) {
                    FSYSSubEntry* sub = (FSYSSubEntry*)((u8*)entry + 0x28);
                    return sub->buffer;
                }
            }
        }
    }
    return NULL;
}

/* fn_8017B3E4 | FSYSGetArchiveEntryCount | Size: 0x64 */
u32 fn_8017B3E4(u32 fileHandle) {
    FSYSSlot* slot = FSYSFindSlot(fileHandle, 1);
    if (slot == NULL) { return 0; }
    return slot->numEntries;
}

/* fn_8017B448 | FSYSGetArchiveDecompSize | Size: 0x74 */
u32 fn_8017B448(u32 fileHandle) {
    FSYSSlot* slot = FSYSFindSlot(fileHandle, 1);
    if (slot == NULL) { return 0; }
    return slot->totalDecompSize;
}

/* fn_8017B4BC | FSYSSetSlotCallbacks | Size: 0xE8 */
void fn_8017B4BC(u32 fileHandle, u32 cbA, u32 cbB, u32 cbC) {
    FSYSSlot* slot = FSYSFindSlot(fileHandle, 1);
    if (slot == NULL) { return; }
    slot->callbackA = cbA;
    slot->callbackB = cbB;
    slot->callbackC = cbC;
}

/* fn_8017B5A4 | FSYSGetSlotLoadMode | Size: 0x1C */
u32 fn_8017B5A4(FSYSSlot* slot) {
    if (slot == NULL) { return 0; }
    return slot->loadMode;
}

/* fn_8017B5C0 | FSYSSetSlotLoadMode | Size: 0xF8 */
void fn_8017B5C0(u32 fileHandle, u32 loadMode) {
    FSYSSlot* slot = FSYSFindSlot(fileHandle, 1);
    if (slot == NULL) { return; }
    slot->loadMode = loadMode;
}

/* fn_8017B6B8 | FSYSOpenArchiveByName | Size: 0x4C8 */
s32 fn_8017B6B8(const char* archiveName, u32 loadMode) {
    u32 i;
    u32 maxSlots = gFSYSManager.maxSlots;
    FSYSSlot* freeSlot = NULL;

    for (i = 0; i < maxSlots; i++) {
        FSYSSlot* slot = &gFSYSSlots[i];
        if (slot->status == FSYS_STATUS_FREE) {
            if (freeSlot == NULL) { freeSlot = slot; }
            continue;
        }
        if (fn_800CA968(slot->filename, archiveName) == 0) {
            slot->refCount++;
            return (s32)(i + 1);
        }
    }
    if (freeSlot == NULL) { return 0; }
    memset(freeSlot, 0, FSYS_SLOT_SIZE);
    fn_800CA968(freeSlot->filename, archiveName);
    freeSlot->status = FSYS_STATUS_PENDING;
    freeSlot->loadMode = loadMode;
    freeSlot->refCount = 1;
    return (s32)((freeSlot - gFSYSSlots) + 1);
}

/* fn_8017BD34 | FSYSCloseArchive | Size: 0x2B4 */
void fn_8017BD34(u32 slotIndex) {
    FSYSSlot* slot;
    if (slotIndex == 0 || slotIndex > gFSYSManager.maxSlots) { return; }
    slot = &gFSYSSlots[slotIndex - 1];
    if (slot->status == FSYS_STATUS_FREE) { return; }
    if (slot->refCount > 1) { slot->refCount--; return; }
    if (slot->archiveHandle != 0) {
        fn_80167E64(slot->archiveHandle);
        slot->archiveHandle = 0;
    }
    slot->archiveData = NULL;
    slot->tocBuffer = NULL;
    slot->status = FSYS_STATUS_FREE;
    slot->refCount = 0;
}

/* fn_8017C008 | FSYSValidateSlot | Size: 0x6C */
BOOL fn_8017C008(FSYSSlot* slot) {
    if (slot == NULL) { return FALSE; }
    if (slot->status == FSYS_STATUS_FREE) { return FALSE; }
    if (slot->archiveData == NULL) { return FALSE; }
    return TRUE;
}

/* fn_8017C074 | FSYSGetEntryByIndex | Size: 0x164 */
void* fn_8017C074(FSYSSlot* slot, u32 entryIndex) {
    if (slot == NULL) { return NULL; }
    if (entryIndex >= slot->numEntries) { return NULL; }
    if (slot->archiveData == NULL) { return NULL; }
    return (u8*)slot->archiveData + (entryIndex * 0x28);
}

/* fn_8017C1D8 | FSYSGetEntryData | Size: 0x1BC */
void* fn_8017C1D8(FSYSSlot* slot, u32 entryIndex) {
    FSYSFileEntry* entry = (FSYSFileEntry*)fn_8017C074(slot, entryIndex);
    if (entry == NULL) { return NULL; }
    {
        FSYSSubEntry* sub = (FSYSSubEntry*)((u8*)entry + 0x28);
        if (sub->ready == 0) { return NULL; }
        return sub->buffer;
    }
}

/* fn_8017C39C | FSYSGetEntryCompSize | Size: 0x78 */
u32 fn_8017C39C(FSYSSlot* slot, u32 entryIndex) {
    FSYSFileEntry* entry = (FSYSFileEntry*)fn_8017C074(slot, entryIndex);
    if (entry == NULL) { return 0; }
    return entry->compressedSize;
}

/* fn_8017C414 | FSYSGetEntryDecompSize | Size: 0x154 */
u32 fn_8017C414(FSYSSlot* slot, u32 entryIndex) {
    FSYSFileEntry* entry = (FSYSFileEntry*)fn_8017C074(slot, entryIndex);
    if (entry == NULL) { return 0; }
    if (entry->flags & 1) { return entry->decompressedSize; }
    return entry->compressedSize;
}

/* fn_8017C580 | Size: 0x10 */
void fn_8017C580(FSYSSlot* slot, void* data) { slot->archiveData = data; }

/* fn_8017C5A0 | Size: 0x10 */
void fn_8017C5A0(FSYSSlot* slot, u32 status) { slot->status = status; }

/* fn_8017C5B8 | FSYSSetSlotReload | Size: 0x128 */
void fn_8017C5B8(FSYSSlot* slot, u32 reloadFlag) {
    if (slot == NULL) { return; }
    slot->reloadFlag = reloadFlag;
    if (reloadFlag != 0 && slot->status >= FSYS_STATUS_LOADED) {
        slot->status = FSYS_STATUS_PENDING;
    }
}

/* fn_8017C6E0 | FSYSProcessSlot | Size: 0x1AC */
s32 fn_8017C6E0(FSYSSlot* slot) {
    if (slot == NULL) { return 0; }
    switch (slot->status) {
    case FSYS_STATUS_FREE: return 0;
    case FSYS_STATUS_PENDING: slot->status = FSYS_STATUS_LOADING; return 1;
    case FSYS_STATUS_LOADING: return 1;
    case FSYS_STATUS_READING: return 1;
    case FSYS_STATUS_LOADED:
        if (slot->callbackA != 0) {
            void (*cb)(FSYSSlot*) = (void (*)(FSYSSlot*))slot->callbackA;
            cb(slot);
        }
        return 1;
    default: return 0;
    }
}

/* fn_8017C894 | Size: 0x2C */
void fn_8017C894(FSYSSlot* slot, u32 count) {
    if (slot != NULL) { slot->refCount = count; }
}

/* fn_8017C8C8 | Size: 0x2C */
void fn_8017C8C8(FSYSSlot* slot, u32 handle) {
    if (slot != NULL) { slot->fileHandle = handle; }
}

/* fn_8017C8FC | FSYSProcessArchive | Size: 0x580 */
s32 fn_8017C8FC(FSYSSlot* slot) {
    u8* ad;
    u32 ne, i;
    if (slot == NULL) { return 0; }
    ad = (u8*)slot->archiveData;
    if (ad == NULL) { return 0; }
    ne = slot->numEntries;
    for (i = 0; i < ne; i++) {
        FSYSFileEntry* entry = (FSYSFileEntry*)(ad + i * 0x28);
        FSYSSubEntry* sub = (FSYSSubEntry*)((u8*)entry + 0x28);
        if (sub->ready != 0) { continue; }
        if (entry->flags & 1) {
            u16 mh = fn_800E2C04(entry->decompressedSize, 32);
            if (mh == 0) { return 0; }
            sub->buffer = fn_800E27B0(mh);
            if (sub->buffer == NULL) { return 0; }
            FSYSDecompressLZSS(sub->buffer, ad + entry->dataOffset, entry->compressedSize);
            DCFlushRange(sub->buffer, entry->decompressedSize);
        } else {
            sub->buffer = ad + entry->dataOffset;
        }
        sub->ready = 1;
    }
    return 1;
}

/* fn_8017CE7C | FSYSGetSlotFilename | Size: 0x4C */
const char* fn_8017CE7C(FSYSSlot* slot) {
    if (slot == NULL) { return NULL; }
    return slot->filename;
}

/* fn_8017CED8 | FSYSLoadAndProcessArchive | Size: 0x4C8 */
s32 fn_8017CED8(FSYSSlot* slot) {
    if (slot == NULL) { return 0; }
    if (slot->archiveData == NULL) {
        u16 mh = fn_800E2C04(slot->archiveSize, 32);
        if (mh == 0) { slot->status = FSYS_STATUS_ERROR; return 0; }
        slot->archiveData = fn_800E27B0(mh);
        if (slot->archiveData == NULL) { slot->status = FSYS_STATUS_ERROR; return 0; }
        fn_80167ED0(slot->archiveHandle, slot->archiveData, slot->archiveSize, 0);
        slot->status = FSYS_STATUS_READING;
        return 1;
    }
    { u32* h = (u32*)slot->archiveData;
      if (h[0] != FSYS_MAGIC) { slot->status = FSYS_STATUS_ERROR; return 0; }
      slot->numEntries = h[2]; slot->totalDecompSize = h[5]; }
    return fn_8017C8FC(slot);
}

/* fn_8017D3A0 | FSYSGetManagerField | Size: 0x34 */
u32 fn_8017D3A0(u32 fieldIndex) {
    u32* mgr = (u32*)&gFSYSManager;
    if (fieldIndex >= 11) { return 0; }
    return mgr[fieldIndex];
}

/* fn_8017D3D4 | FSYSSetManagerField | Size: 0x2C */
void fn_8017D3D4(u32 fieldIndex, u32 value) {
    u32* mgr = (u32*)&gFSYSManager;
    if (fieldIndex < 11) { mgr[fieldIndex] = value; }
}

/* fn_8017D56C | FSYSFindSlotByHandle | Size: 0xB8 */
FSYSSlot* fn_8017D56C(u32 fileHandle) {
    u32 i;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        FSYSSlot* slot = &gFSYSSlots[i];
        if (slot->status == FSYS_STATUS_FREE) { continue; }
        if (slot->fileHandle == fileHandle) { return slot; }
    }
    return NULL;
}

/* fn_8017D624 | FSYSCountActiveSlots | Size: 0x68 */
u32 fn_8017D624(void) {
    u32 i, count = 0;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        if (gFSYSSlots[i].status != FSYS_STATUS_FREE) { count++; }
    }
    return count;
}

/* fn_8017D68C | FSYSGetFreeSlotCount | Size: 0x174 */
u32 fn_8017D68C(void) {
    return gFSYSManager.maxSlots - fn_8017D624();
}

/* fn_8017D800 | FSYSReloadSlot | Size: 0xF8 */
void fn_8017D800(u32 fileHandle) {
    FSYSSlot* slot = fn_8017D56C(fileHandle);
    if (slot == NULL) { return; }
    slot->reloadFlag = 1;
    slot->status = FSYS_STATUS_PENDING;
}

/* fn_8017D8F8 | FSYSGetHandleEntry | Size: 0x34 */
FSYSFileHandle* fn_8017D8F8(u32 handleIndex) {
    if (handleIndex >= FSYS_MAX_HANDLES) { return NULL; }
    return &gFSYSHandleTable[handleIndex];
}

/* fn_8017D92C | FSYSGetHandleID | Size: 0x34 */
s32 fn_8017D92C(u32 handleIndex) {
    FSYSFileHandle* h = fn_8017D8F8(handleIndex);
    if (h == NULL) { return -1; }
    return h->handleID;
}

/* fn_8017D960 | FSYSAllocHandle | Size: 0x158 */
s32 fn_8017D960(void) {
    u32 i;
    for (i = 0; i < FSYS_MAX_HANDLES; i++) {
        if (gFSYSHandleTable[i].handleID == -1) {
            gFSYSHandleTable[i].handleID = (s32)i;
            gFSYSHandleTable[i].userData = 0;
            gFSYSHandleCount++;
            return (s32)i;
        }
    }
    return -1;
}

/* fn_8017DAB8 | FSYSFreeHandle | Size: 0xBC */
void fn_8017DAB8(s32 handleIndex) {
    if (handleIndex < 0 || (u32)handleIndex >= FSYS_MAX_HANDLES) { return; }
    gFSYSHandleTable[handleIndex].handleID = -1;
    gFSYSHandleTable[handleIndex].userData = 0;
    if (gFSYSHandleCount > 0) { gFSYSHandleCount--; }
}

/* fn_8017DB74 | FSYSProcessLoadQueue | Size: 0x330 */
void fn_8017DB74(void) {
    /* Process pending load requests */
    u32 i;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        FSYSSlot* slot = &gFSYSSlots[i];
        if (slot->status == FSYS_STATUS_PENDING) {
            fn_8017C6E0(slot);
        }
    }
}

/* fn_8017DEA4 | Size: 0xA8 */
void* fn_8017DEA4(void) { return gFSYSDVDBuffers[0]; }

/* fn_8017DF4C | Size: 0xA8 */
void* fn_8017DF4C(void) { return gFSYSDVDBuffers[1]; }

/* fn_8017DFF4 | Size: 0xA8 */
void* fn_8017DFF4(void) { return (void*)&gLZSSContext; }

/* fn_8017E09C | FSYSAllocDVDPool | Size: 0x13C */
void* fn_8017E09C(u32 poolIndex) {
    if (poolIndex >= 2) { return NULL; }
    if (gFSYSDVDBuffers[poolIndex] != NULL) { return gFSYSDVDBuffers[poolIndex]; }
    { u16 h = fn_800E2C04(0x20000, 32);
      if (h == 0) { return NULL; }
      gFSYSDVDBuffers[poolIndex] = fn_800E27B0(h);
      return gFSYSDVDBuffers[poolIndex]; }
}

/* fn_8017F108 | FSYSCacheInvalidate | Size: 0x154 */
void fn_8017F108(u32 fileHandle) {
    FSYSSlot* slot = fn_8017D56C(fileHandle);
    if (slot == NULL) { return; }
    if (slot->archiveData != NULL) {
        u32 ne = slot->numEntries, i;
        for (i = 0; i < ne; i++) {
            u8* ad = (u8*)slot->archiveData;
            FSYSFileEntry* e = (FSYSFileEntry*)(ad + i * 0x28);
            FSYSSubEntry* s = (FSYSSubEntry*)((u8*)e + 0x28);
            s->ready = 0;
        }
    }
}

/* fn_8017F25C | FSYSGetFileHandleCount | Size: 0x68 */
u32 fn_8017F25C(void) { return gFSYSHandleCount; }

/* fn_8017F3F8 | FSYSLookupByNameHash | Size: 0x8C */
FSYSSlot* fn_8017F3F8(u32 nameHash) {
    u32 i;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        FSYSSlot* slot = &gFSYSSlots[i];
        if (slot->status == FSYS_STATUS_FREE) { continue; }
        if (slot->requestID == nameHash) { return slot; }
    }
    return NULL;
}

/* fn_8017F484 | FSYSIterateSlots | Size: 0x230 */
void fn_8017F484(void (*callback)(FSYSSlot*, u32), u32 userData) {
    u32 i;
    if (callback == NULL) { return; }
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        FSYSSlot* slot = &gFSYSSlots[i];
        if (slot->status != FSYS_STATUS_FREE) { callback(slot, userData); }
    }
}

/* fn_8017F6B4 | FSYSGetTOCData | Size: 0x74 */
void* fn_8017F6B4(void) { return gFSYSTocData; }

/* fn_8017F728 | FSYSSetTOCData | Size: 0x6C */
void fn_8017F728(void* tocData) { gFSYSTocData = tocData; }

/* fn_8017F800 | FSYSProcessPendingLoads | Size: 0x128 */
void fn_8017F800(void) {
    u32 i;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        if (gFSYSSlots[i].status == FSYS_STATUS_PENDING) {
            fn_8017C6E0(&gFSYSSlots[i]);
        }
    }
}

/* fn_8017F928 | FSYSProcessActiveLoads | Size: 0x134 */
void fn_8017F928(void) {
    u32 i;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        FSYSSlot* s = &gFSYSSlots[i];
        if (s->status == FSYS_STATUS_LOADING || s->status == FSYS_STATUS_READING) {
            fn_8017C6E0(s);
        }
    }
}

/* fn_8017FA5C | FSYSCheckAllLoaded | Size: 0xAC */
BOOL fn_8017FA5C(void) {
    u32 i;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        FSYSSlot* s = &gFSYSSlots[i];
        if (s->status == FSYS_STATUS_FREE) { continue; }
        if (s->status < FSYS_STATUS_LOADED) { return FALSE; }
    }
    return TRUE;
}

/* fn_80180320 | FSYSLoadMgrQueueRequest | Size: 0x130 */
void fn_80180320(void* fileHandle, void* nameHash, u32 priority) {
    FSYSSlot* slot = FSYSFindSlot((u32)fileHandle, 0);
    if (slot == NULL) { return; }
    slot->requestID = (u32)nameHash;
    slot->loadMode = priority;
    slot->status = FSYS_STATUS_PENDING;
}

/* fn_80180450 | FSYSLoadMgrCancelRequest | Size: 0x134 */
void fn_80180450(u32 fileHandle) {
    FSYSSlot* slot = fn_8017D56C(fileHandle);
    if (slot == NULL) { return; }
    if (slot->status == FSYS_STATUS_PENDING) {
        slot->status = FSYS_STATUS_FREE;
    }
}

/* fn_80180584 | FSYSLoadMgrGetQueueDepth | Size: 0x110 */
u32 fn_80180584(void) {
    u32 i, count = 0;
    for (i = 0; i < gFSYSManager.maxSlots; i++) {
        if (gFSYSSlots[i].status == FSYS_STATUS_PENDING) { count++; }
    }
    return count;
}

/* fn_80180694 | FSYSLoadMgrIsRequestPending | Size: 0x114 */
BOOL fn_80180694(u32 fileHandle) {
    FSYSSlot* slot = fn_8017D56C(fileHandle);
    if (slot == NULL) { return FALSE; }
    return (slot->status == FSYS_STATUS_PENDING) ? TRUE : FALSE;
}

/* fn_801807A8 | FSYSLoadMgrProcessNext | Size: 0x10C */
void fn_801807A8(void) { fn_8017DB74(); }

/* fn_801808B4 | FSYSLoadMgrInit | Size: 0x48 */
void fn_801808B4(u32 maxRequests) {
    gFSYSHandleCount = 0;
}
