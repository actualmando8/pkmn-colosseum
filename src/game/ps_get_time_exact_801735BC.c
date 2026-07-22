#include "dolphin/types.h"

u8* getTime(u8* stream, u16* value)
{
    *value = *stream++;
    if (*value & 0x80) {
        *value = ((*value & 0x7F) << 8) + *stream++;
    }
    return stream;
}
