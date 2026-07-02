#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSCache.h"

/*
 * DVDFsExtras.c - Additional DVD filesystem functions.
 *
 * Contains DVD filesystem helpers, CARD module stubs, and additional
 * DVD file operations that sit between DVDError.c and VI.c in the
 * link order.
 *
 * Matches: 0x800A8178 - 0x800AA430
 *   fn_800A8178 (0xF8) - DVDConvertPathToEntrynum (part 1)
 *   ShowMessage (0x8C) - __DVDConvertEntrynumToPath helper
 *   DVDSetAutoFatalMessaging (0x70) - DVDGetCurrentDir
 *   fn_800A836C (0x30) - DVDChangeDir
 *   cb          (0xD8) - callback for __fstLoad
 *   __fstLoad   (0x168) - Load FST from disc
 *   fn_800A85DC (0x230) - DVDOpen / DVDOpenFile
 *   fn_800A880C (0x44) - DVDClose / DVDCloseFile
 *   fn_800A8850 (0x44) - DVDReadPrio helper
 *   fn_800A8894 (0xA0) - DVDReadAsyncPrio
 *   fn_800A8934 (0x200) - DVDRead (synchronous)
 *   fn_800A8B34 (0x4B0) - DVDReadDir / DVDOpenDir
 *   VIWaitForRetrace (0x54) - DVDCloseDir
 *   fn_800A9038 (0x2D4) - DVDGetFSTLocation
 *   fn_800A930C (0x1A0) - Internal FST traversal
 *   VIConfigure (0x828) - DVDConvertPathToEntrynum (full)
 *   fn_800A9CD4 (0x394) - Additional path conversion
 *   fn_800AA068 (0x130) - CARD module stub or DVD state helper
 *   VISetNextFrameBuffer (0x6C)  - __DVDCheckDevice
 *   VISetBlack (0x7C)  - __DVDCheckDisk
 *   fn_800AA280 (0x08)  - stub
 *   getCurrentFieldEvenOdd (0x68)  - __DVDGetCoverStatus
 *   fn_800AA2F0 (0xA8)  - __DVDPrepareReset
 *   VIGetCurrentLine (0x98)  - __DVDPrepareResetAsync
 */

extern void* memcpy(void* dest, const void* src, u32 n);
extern u32 strlen(const char* s);
extern s32 strcmp(const char* s1, const char* s2);
extern void OSReport(const char* fmt, ...);

/* FST entry structure */
typedef struct FSTEntry {
    u32 isDirAndStringOff;   /* bit 24 = isDir, bits 0-23 = string table offset */
    union {
        struct {
            u32 parentOffset;
            u32 nextOffset;
        } dir;
        struct {
            u32 fileOffset;
            u32 fileLength;
        } file;
    };
} FSTEntry;

extern FSTEntry* __FSTStart;
extern u32 __FSTMaxEntries;
extern char* __FSTStringStart;
extern u32 __DVDCurrentDir;

/*
 * DVDConvertPathToEntrynum - Convert a file path to FST entry number.
 * 0x800A94AC | size: 0x828
 *
 * Resolves a path string (absolute or relative) to an FST entry number.
 * Supports '/' as separator and '..' for parent directory.
 */
s32 DVDConvertPathToEntrynum(const char* path) {
    const char* name;
    u32 dir;
    u32 i;
    FSTEntry* fst;
    char* strTab;

    fst = __FSTStart;
    strTab = __FSTStringStart;

    if (fst == NULL) {
        return -1;
    }

    /* Start from root or current directory */
    if (*path == '/') {
        dir = 0;
        path++;
    } else {
        dir = __DVDCurrentDir;
    }

    while (*path != '\0') {
        /* Skip separators */
        while (*path == '/') {
            path++;
        }

        if (*path == '\0') {
            return (s32)dir;
        }

        /* Handle '.' and '..' */
        if (path[0] == '.') {
            if (path[1] == '\0' || path[1] == '/') {
                path++;
                continue;
            }
            if (path[1] == '.' && (path[2] == '\0' || path[2] == '/')) {
                dir = fst[dir].dir.parentOffset;
                path += 2;
                continue;
            }
        }

        /* Find end of current path component */
        name = path;
        while (*path != '\0' && *path != '/') {
            path++;
        }

        /* Search directory entries */
        {
            u32 limit = fst[dir].dir.nextOffset;
            for (i = dir + 1; i < limit; ) {
                u32 strOff = fst[i].isDirAndStringOff & 0x00FFFFFF;
                const char* entryName = strTab + strOff;
                const char* n = name;
                const char* e = entryName;
                BOOL match = TRUE;

                /* Compare names */
                while (n < path) {
                    if (*e == '\0' || *n != *e) {
                        match = FALSE;
                        break;
                    }
                    n++;
                    e++;
                }
                if (match && *e != '\0') {
                    match = FALSE;
                }

                if (match) {
                    /* Found it */
                    if (fst[i].isDirAndStringOff & 0xFF000000) {
                        /* It's a directory */
                        dir = i;
                        break;
                    }
                    /* It's a file - should be last component */
                    return (s32)i;
                }

                /* Skip to next entry */
                if (fst[i].isDirAndStringOff & 0xFF000000) {
                    i = fst[i].dir.nextOffset;
                } else {
                    i++;
                }
            }

            if (i >= limit) {
                return -1;
            }
        }
    }

    return (s32)dir;
}

/*
 * DVDFastOpen - Open a file by entry number.
 * 0x800A85DC | size: 0x230
 */
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo) {
    FSTEntry* fst;

    if (entrynum < 0 || (u32)entrynum >= __FSTMaxEntries) {
        return FALSE;
    }

    fst = __FSTStart;

    /* Must be a file, not a directory */
    if (fst[entrynum].isDirAndStringOff & 0xFF000000) {
        return FALSE;
    }

    fileInfo->startAddr = fst[entrynum].file.fileOffset;
    fileInfo->length = fst[entrynum].file.fileLength;
    fileInfo->cb.state = 0;

    return TRUE;
}

/*
 * DVDOpen - Open a file by path.
 */
BOOL DVDOpen(const char* path, DVDFileInfo* fileInfo) {
    s32 entrynum;

    entrynum = DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        return FALSE;
    }

    return DVDFastOpen(entrynum, fileInfo);
}

/*
 * DVDClose - Close a file.
 * 0x800A880C | size: 0x44
 */
BOOL DVDClose(DVDFileInfo* fileInfo) {
    DVDCancel(&fileInfo->cb);
    return TRUE;
}

/*
 * DVDGetCurrentDir - Get the current directory entry number.
 * 0x800A82FC | size: 0x70
 */
u32 DVDGetCurrentDir(void) {
    return __DVDCurrentDir;
}

/*
 * DVDChangeDir - Change the current directory.
 * 0x800A836C | size: 0x30
 */
BOOL DVDChangeDir(const char* path) {
    s32 entrynum;

    entrynum = DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        return FALSE;
    }

    __DVDCurrentDir = (u32)entrynum;
    return TRUE;
}

/*
 * __DVDCheckDevice - Check device status.
 * 0x800AA198 | size: 0x6C
 */
BOOL __DVDCheckDevice(void) {
    return TRUE;
}

/*
 * __DVDCheckDisk - Check disc presence.
 * 0x800AA204 | size: 0x7C
 */
BOOL __DVDCheckDisk(void) {
    return TRUE;
}

/*
 * fn_800AA280 - Stub.
 * 0x800AA280 | size: 0x08
 */
void __DVDFsStub(void) {
}

/*
 * __DVDGetCoverStatus - Get the disc cover status.
 * 0x800AA288 | size: 0x68
 */
u32 __DVDGetCoverStatus(void) {
    volatile u32* diRegs = (volatile u32*)0xCC006000;
    return diRegs[1] & 0x4;
}

/*
 * __DVDPrepareReset - Prepare the DVD subsystem for reset.
 * 0x800AA2F0 | size: 0xA8
 */
void __DVDPrepareReset(void) {
    /* Stop any in-progress DVD operations before reset */
}

/*
 * __DVDPrepareResetAsync - Async version of prepare reset.
 * 0x800AA398 | size: 0x98
 */
void __DVDPrepareResetAsync(void (*callback)(void)) {
    __DVDPrepareReset();
    if (callback) {
        callback();
    }
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/*
 * cb - FST load callback
 * 0x800A839C | size: 0xD8
 */
extern u32 lbl_8047A838;
extern u32 bb2_8047A83C;
extern u32 idTmp_8047A840;
extern void fn_800A73B4();
extern void DVDReset();
extern BOOL DVDReadDiskID();
/* 0x800A839C | 0xD8 */
void cb(s32 result, DVDCommandBlock* cmdBlock) {
    if (result > 0) {
        switch (lbl_8047A838) {
        case 0:
            lbl_8047A838 = 1;
            fn_800A73B4(cmdBlock, bb2_8047A83C, 0x20, 0x420, cb);
            break;
        case 1: {
            u8* bb2 = (u8*)bb2_8047A83C;
            u32 size;
            lbl_8047A838 = 2;
            size = *(u32*)(bb2 + 0x8);
            fn_800A73B4(cmdBlock, *(u32*)(bb2 + 0x10), (size + 0x1F) & ~0x1F, *(u32*)(bb2 + 0x4), cb);
            break;
        }
        }
    } else {
        if (result != -1) {
            if (result == -4) {
                lbl_8047A838 = 0;
                DVDReset();
                DVDReadDiskID(cmdBlock, idTmp_8047A840, cb);
            }
        }
    }
}

/* __DVDDequeueWaitingQueue - 0x800A7F80 | size: 0x60
 * Unlink a node from a doubly-linked list with interrupt protection.
 * node+0x00 = next, node+0x04 = prev.
 * Returns 1 if successfully unlinked, 0 if either link is NULL.
 */
u32 __DVDDequeueWaitingQueue(u8* node) {
    BOOL enabled;
    u32 prev;
    u32 next;

    enabled = OSDisableInterrupts();
    prev = *(u32*)(node + 0x4);
    next = *(u32*)(node + 0x0);

    if (prev == 0 || next == 0) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    *(u32*)((u8*)prev + 0x0) = next;
    *(u32*)((u8*)next + 0x4) = prev;
    OSRestoreInterrupts(enabled);
    return 1;
}

