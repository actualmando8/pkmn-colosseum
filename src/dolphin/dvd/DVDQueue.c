#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"

typedef struct DVDQueueNode {
    struct DVDQueueNode* next;
    struct DVDQueueNode* prev;
} DVDQueueNode;

extern DVDQueueNode WaitingQueue[4];

BOOL __DVDCheckWaitingQueue(void) {
    BOOL enabled;
    DVDQueueNode* sentinel;
    s32 i;

    enabled = OSDisableInterrupts();

    for (i = 0; i < 4; i++) {
        sentinel = &WaitingQueue[i];

        if (sentinel->next != sentinel) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}
