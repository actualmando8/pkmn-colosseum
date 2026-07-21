#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"

extern volatile s32 PauseFlag_8047A7F4;
extern volatile s32 PausingFlag_8047A7F8;
extern DVDCommandBlock* executing_8047A7E8;
extern u32 lbl_8047A808;
extern DVDCBCallback lbl_8047A80C;

extern void stateReady_800A6684(void);
extern BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback);

inline void DVDPause(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    PauseFlag_8047A7F4 = TRUE;
    if (executing_8047A7E8 == NULL) {
        PausingFlag_8047A7F8 = TRUE;
    }
    OSRestoreInterrupts(enabled);
}

inline void DVDResume(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    PauseFlag_8047A7F4 = FALSE;
    if (PausingFlag_8047A7F8 != FALSE) {
        PausingFlag_8047A7F8 = FALSE;
        stateReady_800A6684();
    }
    OSRestoreInterrupts(enabled);
}

inline BOOL DVDCancelAllAsync(DVDCBCallback callback) {
    BOOL enabled;
    DVDCommandBlock* block;
    BOOL result;

    enabled = OSDisableInterrupts();
    DVDPause();

    while ((block = __DVDPopWaitingQueue()) != NULL) {
        DVDCancelAsync(block, NULL);
    }

    if (executing_8047A7E8 != NULL) {
        result = DVDCancelAsync(executing_8047A7E8, callback);
    } else {
        result = TRUE;
        if (callback != NULL) {
            callback(0, NULL);
        }
    }

    DVDResume();
    OSRestoreInterrupts(enabled);
    return result;
}

void __DVDPrepareResetAsync(DVDCBCallback callback) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    __DVDClearWaitingQueue();

    if (lbl_8047A808) {
        lbl_8047A80C = callback;
    } else {
        if (executing_8047A7E8 != NULL) {
            executing_8047A7E8->callback = NULL;
        }
        DVDCancelAllAsync(callback);
    }

    OSRestoreInterrupts(enabled);
}
