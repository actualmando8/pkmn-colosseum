#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSCache.h"

/*
 * DVDFsExtras.c - DVD filesystem helper (tiny unit).
 *
 * Per config/GC6E01/splits.txt this unit's real address range is only
 * 0x800A7F80 - 0x800A7FE0 (0x60 bytes), i.e. __DVDDequeueWaitingQueue
 * below, which is a byte-exact match. The "Matches: 0x800A8178 -
 * 0x800AA430" range previously claimed here (with fn_800A8178,
 * VIConfigure, VIWaitForRetrace, VISetNextFrameBuffer, VISetBlack,
 * getCurrentFieldEvenOdd, VIGetCurrentLine, etc. relabeled as DVD
 * functions) actually belongs to dolphin/vi/VI_range_800A8178.c and
 * dolphin/vi/VI_fn_800AA280.c - i.e. the VI (video interface) library,
 * not DVD filesystem code at all. That block was invented fiction from
 * a prior transplant pass.
 *
 * 2026-07-02 reconciliation: removed the fictional definitions of
 * DVDFastOpen, DVDClose, DVDChangeDir, __DVDCheckDevice, __DVDCheckDisk,
 * __DVDFsStub and __DVDGetCoverStatus, and the fictional body of
 * __DVDPrepareReset (kept as an extern declaration; __DVDPrepareResetAsync
 * below still calls it). DVDConvertPathToEntrynum, DVDOpen,
 * DVDGetCurrentDir, cb and __DVDPrepareResetAsync were left untouched:
 * their names DO exist in symbols.txt (so they are not "orphans" by this
 * pass's definition), just at addresses in other units - a separate,
 * out-of-scope wrong-unit-placement problem.
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
 * DVDFastOpen - orphan removed (see file header). Not present in
 * symbols.txt; this file's claimed address (0x800A85DC) actually
 * belongs to dolphin/vi/VI_range_800A8178.c per splits.txt, not this
 * unit. A second, differently-sized invented copy also lived in
 * DVD.c (also removed). Declaration remains in dolphin/dvd/dvd.h
 * because DVDOpen() below still calls it (declared via dolphin/dvd/dvd.h,
 * included at the top of this file).
 */

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
 * DVDClose - orphan removed (see file header). Not present in
 * symbols.txt; claimed address (0x800A880C) belongs to
 * dolphin/vi/VI_range_800A8178.c per splits.txt, not this unit. A
 * second, differently-sized invented copy also lived in DVD.c (also
 * removed). Declaration remains in dolphin/dvd/dvd.h because DVDOpen()
 * below still calls it.
 */

/*
 * DVDGetCurrentDir - Get the current directory entry number.
 * 0x800A82FC | size: 0x70
 */
u32 DVDGetCurrentDir(void) {
    return __DVDCurrentDir;
}

/*
 * DVDChangeDir, __DVDCheckDevice, __DVDCheckDisk, __DVDFsStub,
 * __DVDGetCoverStatus, __DVDPrepareReset - orphans removed (see file
 * header). None of these names are present in symbols.txt; all claimed
 * addresses (0x800A836C, 0x800AA198-0x800AA398) actually belong to
 * dolphin/vi/VI_range_800A8178.c and dolphin/vi/VI_fn_800AA280.c per
 * splits.txt (this unit's real range is only 0x800A7F80-0x800A7FE0,
 * i.e. __DVDDequeueWaitingQueue below). The DVDChangeDir prototype in
 * dolphin/dvd/dvd.h had no remaining callers and was removed too.
 * DVDChangeDir's real body called DVDConvertPathToEntrynum, which is
 * unrelated and untouched.
 *
 * __DVDPrepareResetAsync below still calls __DVDPrepareReset; kept as
 * a bare extern declaration so this TU still compiles.
 */
extern void __DVDPrepareReset(void);

/*
 * __DVDPrepareResetAsync - NOT removed (out of scope for this pass:
 * not in the orphan worklist). The name IS present in symbols.txt, but
 * at 0x800A7CCC - a different unit than this file, so it is a
 * wrong-unit placement rather than a pure orphan; left untouched.
 * Only its __DVDPrepareReset() callee (a listed orphan) was removed
 * above.
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

