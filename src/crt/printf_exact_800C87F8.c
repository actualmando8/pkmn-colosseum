#include "dolphin/types.h"

typedef struct StringWriteState {
    char* buffer;
    u32 limit;
    u32 position;
} StringWriteState;

extern void* memcpy(void* destination, const void* source, u32 count);

void* __StringWrite(void* context, const char* buffer, u32 count)
{
    StringWriteState* state;
    u32 remaining;
    u32 copied;

    state = context;
    remaining = state->limit - state->position;
    copied = remaining;
    if (state->position + count <= state->limit) {
        copied = count;
    }
    memcpy(state->buffer + state->position, buffer, copied);
    state->position += copied;
    return (void*)1;
}
