#include "dolphin/types.h"

typedef struct TRKBuffer {
    s32 mutex;
    s32 inUse;
    u32 length;
    u32 position;
    u8 data[0x880];
} TRKBuffer;

extern TRKBuffer lbl_803FCE08[];

s32 TRKSetBufferPosition(TRKBuffer* buffer, u32 position) {
    s32 result = 0;

    if (position > 0x880) {
        result = 0x301;
    } else {
        buffer->position = position;
        if (position > buffer->length) {
            buffer->length = position;
        }
    }

    return result;
}
