#include "dolphin/types.h"

extern u8 lbl_8044FB90[];
extern u8 lbl_8044FE14[];

typedef struct MusyXAramQueueEntry {
    u32 request;
    u32 command;
    u32 zero;
    u32 priority;
    u8* destination;
    u8* source;
    u32 size;
    void (*completion)(void*);
    void (*clientCallback)(void*);
    void* clientArgument;
} MusyXAramQueueEntry;

void aramQueueCallback(void* request)
{
    u8* queue;
    u32 i;

    if (*(u32*)((u8*)request + 0xC) == 1) {
        queue = lbl_8044FE14;
    } else {
        queue = lbl_8044FB90;
    }

    for (i = 0; i < 16; i++) {
        MusyXAramQueueEntry* entry =
            (MusyXAramQueueEntry*)(queue + i * sizeof(MusyXAramQueueEntry));
        if (request == (void*)entry && entry->clientCallback != 0) {
            entry->clientCallback(entry->clientArgument);
        }
    }
    queue[0x281]--;
}
