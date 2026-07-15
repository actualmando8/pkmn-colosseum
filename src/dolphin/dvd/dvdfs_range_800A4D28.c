#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSThread.h"

/* SDA-relative global used by the callback wait path in this unit */
extern OSThreadQueue __DVDThreadQueue;

BOOL DVDOpen(const char* path, DVDFileInfo* fileInfo) {
    struct DVDFstEntry {
        u32 typeAndNameOffset;
        u32 parentOrStart;
        u32 nextOrLength;
    };
    extern struct DVDFstEntry* FstStart_8047A7CC;
    extern char lbl_803119B8[];
    extern BOOL DVDGetCurrentDir(char* path, u32 maxlen);
    extern void OSReport(const char* format, ...);
    s32 entrynum;
    BOOL isDir;
    char currentDir[128];

    entrynum = DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        DVDGetCurrentDir(currentDir, sizeof(currentDir));
        OSReport(lbl_803119B8, path, currentDir);
        return FALSE;
    }

    if ((FstStart_8047A7CC[entrynum].typeAndNameOffset & 0xFF000000) == 0) {
        isDir = FALSE;
    } else {
        isDir = TRUE;
    }
    if (isDir) {
        return FALSE;
    }

    fileInfo->startAddr = FstStart_8047A7CC[entrynum].parentOrStart;
    fileInfo->length = FstStart_8047A7CC[entrynum].nextOrLength;
    fileInfo->callback = NULL;
    fileInfo->cb.state = 0;
    return TRUE;
}

BOOL DVDClose(DVDFileInfo* fileInfo) {
    DVDCancel(&fileInfo->cb);
    return TRUE;
}

BOOL DVDGetCurrentDir(char* path, u32 maxlen) {
    struct DVDFstEntry {
        u32 typeAndNameOffset;
        u32 parentOrStart;
        u32 nextOrLength;
    };
    extern struct DVDFstEntry* FstStart_8047A7CC;
    extern u32 lbl_8047A7D8;
    extern u32 entryToPath(u32 entrynum, char* path, u32 maxlen);
    u32 entrynum;
    u32 length;
    BOOL isDir;

    entrynum = lbl_8047A7D8;
    length = entryToPath(entrynum, path, maxlen);
    if (length == maxlen) {
        path[maxlen - 1] = '\0';
        return FALSE;
    }

    if ((FstStart_8047A7CC[entrynum].typeAndNameOffset & 0xFF000000) == 0) {
        isDir = FALSE;
    } else {
        isDir = TRUE;
    }
    if (isDir) {
        if (length == maxlen - 1) {
            path[length] = '\0';
            return FALSE;
        }
        path[length++] = '/';
    }
    path[length] = '\0';
    return TRUE;
}

BOOL DVDReadAsync(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                  DVDCBCallback callback, s32 prio) {
    extern char lbl_804789C0[8];
    extern char lbl_803119F0[];
    extern void fn_800060F0(const char* file, s32 line, const char* format, ...);
    extern void cbForReadAsync(s32 result, DVDFileInfo* fileInfo);
    s32 end;

    if (offset < 0 || (u32)offset >= fileInfo->length) {
        fn_800060F0(lbl_804789C0, 0x2E6, lbl_803119F0);
    }
    end = offset + length;
    if (end < 0 || (u32)end >= fileInfo->length + 0x20) {
        fn_800060F0(lbl_804789C0, 0x2EC, lbl_803119F0);
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

BOOL DVDSeekAsyncPrio(DVDFileInfo* fileInfo, s32 offset,
                      DVDCBCallback callback, s32 prio) {
    extern char lbl_804789C0[8];
    extern char lbl_80311A54[];
    extern void fn_800060F0(const char* file, s32 line, const char* format, ...);
    extern BOOL DVDSeekAbsAsyncPrio(DVDCommandBlock* block, s32 offset,
                                     DVDCBCallback callback, s32 prio);
    extern void cbForSeekAsync(s32 result, DVDFileInfo* fileInfo);

    if (offset < 0 || (u32)offset >= fileInfo->length) {
        fn_800060F0(lbl_804789C0, 0x383, lbl_80311A54);
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
