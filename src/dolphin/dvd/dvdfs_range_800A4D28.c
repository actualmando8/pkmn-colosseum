#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSThread.h"

/* SDA-relative global used by the callback wait path in this unit */
extern OSThreadQueue __DVDThreadQueue;

BOOL DVDClose(DVDFileInfo* fileInfo) {
    DVDCancel(&fileInfo->cb);
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

void cbForSeekAsync(s32 result, DVDFileInfo* fileInfo) {
    if (fileInfo->callback != NULL) {
        fileInfo->callback(result, &fileInfo->cb);
    }
}

void defaultOptionalCommandChecker(DVDCommandBlock* command) {
    (void)command;
}
