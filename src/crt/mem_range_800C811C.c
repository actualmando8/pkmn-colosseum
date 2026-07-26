#include "dolphin/types.h"

void* __memrchr(const void* ptr, int ch, size_t count)
{
    const unsigned char* p;
    unsigned long value = ch & 0xFF;

    for (p = (const unsigned char*)ptr + count, count++; --count;) {
        if (*--p == value) {
            return (void*)p;
        }
    }
    return NULL;
}

void* memchr(const void* ptr, int ch, size_t count)
{
    const unsigned char* p;
    unsigned long value = ch & 0xFF;

    for (p = (const unsigned char*)ptr - 1, count++; --count;) {
        if ((*++p & 0xFF) == value) {
            return (void*)p;
        }
    }
    return NULL;
}
