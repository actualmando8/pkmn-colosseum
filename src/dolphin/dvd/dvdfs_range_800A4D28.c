/*
 * dolphin/dvd/dvd.h declares "u32 DVDGetCurrentDir(void);" which does not
 * match this unit's real two-argument DVDGetCurrentDir (buf, maxlen). That
 * header is shared and out of scope for this file, so shadow the name only
 * for the duration of the include and restore it before defining the real
 * function below.
 */
#define DVDGetCurrentDir DVDGetCurrentDir_stale_header_prototype
#include "dolphin/dvd/dvd.h"
#undef DVDGetCurrentDir
#include "dolphin/os/OSThread.h"

/* SDA-relative global used by the callback wait path in this unit */
extern OSThreadQueue __DVDThreadQueue;

/* Forward declarations for callbacks defined later in this file. */
void cbForReadAsync(s32 result, DVDFileInfo* fileInfo);
void cbForSeekAsync(s32 result, DVDFileInfo* fileInfo);
BOOL DVDGetCurrentDir(char* path, u32 maxlen);

/* FST entry layout - mirrors DVDFs.c's DVDFstEntry. Duplicated locally
 * since the type isn't shared through a header; FstStart_8047A7CC is the
 * same global object defined (non-static) in DVDFs.c. */
typedef struct DVDFstEntry {
    u32 typeAndNameOffset;
    u32 parentOrStart;
    u32 nextOrLength;
} DVDFstEntry;

#define DVD_FST_TYPE_MASK 0xFF000000u

extern DVDFstEntry* FstStart_8047A7CC;

/* Current-directory entry number (sda-relative global at 0x8047A7D8). */
extern u32 lbl_8047A7D8;

extern u32 entryToPath(u32 entrynum, char* path, u32 maxlen);

BOOL DVDClose(DVDFileInfo* fileInfo) {
    DVDCancel(&fileInfo->cb);
    return TRUE;
}

BOOL DVDOpen(const char* path, DVDFileInfo* fileInfo) {
    extern char lbl_803119B8[];
    extern void OSReport(const char* fmt, ...);
    char cwd[0x80];
    s32 entrynum = DVDConvertPathToEntrynum(path);

    if (entrynum < 0) {
        DVDGetCurrentDir(cwd, sizeof(cwd));
        OSReport(lbl_803119B8, path, cwd);
        return FALSE;
    }

    {
        s32 isDir;

        if ((FstStart_8047A7CC[entrynum].typeAndNameOffset & DVD_FST_TYPE_MASK) == 0) {
            isDir = FALSE;
        } else {
            isDir = TRUE;
        }

        if (isDir != FALSE) {
            return FALSE;
        }
    }

    fileInfo->startAddr = FstStart_8047A7CC[entrynum].parentOrStart;
    fileInfo->length = FstStart_8047A7CC[entrynum].nextOrLength;
    fileInfo->callback = NULL;
    fileInfo->cb.state = 0;

    return TRUE;
}

BOOL DVDGetCurrentDir(char* path, u32 maxlen) {
    u32 currentDir = lbl_8047A7D8;
    u32 len = entryToPath(currentDir, path, maxlen);
    s32 result;

    if (len == maxlen) {
        path[maxlen - 1] = '\0';
        result = FALSE;
        goto done;
    }

    if ((FstStart_8047A7CC[currentDir].typeAndNameOffset & DVD_FST_TYPE_MASK) == 0) {
        result = FALSE;
    } else {
        result = TRUE;
    }

    if (result != FALSE) {
        if (len == maxlen - 1) {
            path[len] = '\0';
            result = FALSE;
            goto done;
        }
        path[len] = '/';
        len++;
    }

    path[len] = '\0';
    result = TRUE;

done:
    return result;
}

BOOL DVDReadAsync(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                  DVDCBCallback callback, s32 prio) {
    extern char lbl_803119F0[];
    extern char lbl_804789C0;
    extern void fn_800060F0(const char* file, u32 line, const char* expr, ...);
    s32 end;

    if (offset < 0 || (u32)offset >= fileInfo->length) {
        fn_800060F0(&lbl_804789C0, 0x2E6, lbl_803119F0);
    }

    end = offset + length;
    if (end < 0 || (u32)end >= fileInfo->length + 0x20) {
        fn_800060F0(&lbl_804789C0, 0x2EC, lbl_803119F0);
    }

    fileInfo->callback = callback;
    DVDReadAbsAsyncPrio(&fileInfo->cb, addr, length, fileInfo->startAddr + offset,
                        (DVDCBCallback)cbForReadAsync, prio);
    return TRUE;
}

void cbForReadAsync(s32 result, DVDFileInfo* fileInfo) {
    if (fileInfo->callback != NULL) {
        fileInfo->callback(result, &fileInfo->cb);
    }
}

void cbForReadSync(s32 result, DVDFileInfo* fileInfo) {
    OSWakeupThread(&__DVDThreadQueue);
}

BOOL DVDSeekAsyncPrio(DVDFileInfo* fileInfo, s32 offset, DVDCBCallback callback,
                      s32 prio) {
    extern char lbl_80311A54[];
    extern char lbl_804789C0;
    extern void fn_800060F0(const char* file, u32 line, const char* expr, ...);
    extern BOOL DVDSeekAbsAsyncPrio(DVDCommandBlock* block, s32 offset,
                                    DVDCBCallback callback, s32 prio);

    if (offset < 0 || (u32)offset >= fileInfo->length) {
        fn_800060F0(&lbl_804789C0, 0x383, lbl_80311A54);
    }

    fileInfo->callback = callback;
    DVDSeekAbsAsyncPrio(&fileInfo->cb, fileInfo->startAddr + offset,
                        (DVDCBCallback)cbForSeekAsync, prio);
    return TRUE;
}

void cbForSeekAsync(s32 result, DVDFileInfo* fileInfo) {
    if (fileInfo->callback != NULL) {
        fileInfo->callback(result, &fileInfo->cb);
    }
}

void defaultOptionalCommandChecker(DVDCommandBlock* command) {
    (void)command;
}
