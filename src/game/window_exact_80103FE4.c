#include "dolphin/types.h"

typedef struct Window {
    u8 padding_000[0xB0];
    void* allocation;
} Window;

void* windowGetAllocPtr(Window* window)
{
    if (window != NULL) {
        return window->allocation;
    }
    return NULL;
}
