#ifndef CRT_STRING_H
#define CRT_STRING_H

#include "dolphin/types.h"

s32 memcmp(const void* lhs, const void* rhs, u32 len);
void* memcpy(void* dst, const void* src, u32 len);

#endif /* CRT_STRING_H */
