#include "dolphin/types.h"

typedef struct GSnpcEventQueue {
    void* items[13];
    u32 index;
} GSnpcEventQueue;

void* fn_80030170(GSnpcEventQueue* queue)
{
    u32 index;

    if (queue == NULL) {
        return NULL;
    }

    index = queue->index;
    if (index >= 13) {
        return NULL;
    }

    queue->index = index + 1;
    return queue->items[index];
}

void fn_800301A8(void)
{
}

void fn_800301AC(void)
{
}
