#include "dolphin/types.h"

extern u32 lbl_8047C408;
extern char* strncpy(char* dst, const char* src, u32 n);

static inline int unicode_to_UTF8(char* dst, u16 wide)
{
    int count;
    char* out;
    u32 first_byte_mark = lbl_8047C408;

    if (wide < 0x80) {
        count = 1;
    } else if (wide < 0x800) {
        count = 2;
    } else {
        count = 3;
    }

    out = dst + count;
    switch (count) {
    case 3:
        *--out = (wide & 0x3F) | 0x80;
        wide >>= 6;
    case 2:
        *--out = (wide & 0x3F) | 0x80;
        wide >>= 6;
    case 1:
        *--out = wide | ((char*)&first_byte_mark)[count];
    }

    return count;
}

u32 wcstombs(char* dst, const u16* source, u32 limit)
{
    int written = 0;
    int count;
    char encoded[3];
    u16* current;

    if (dst == 0 || source == 0) {
        return 0;
    }

    current = (u16*)source;
    while (written <= limit) {
        if (*current == 0) {
            dst[written] = '\0';
            break;
        }

        count = unicode_to_UTF8(encoded, *current++);
        if (written + count <= limit) {
            strncpy(dst + written, encoded, count);
            written += count;
        } else {
            break;
        }
    }

    return written;
}
