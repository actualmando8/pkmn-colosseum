#include "dolphin/types.h"

char* strchr(const char* string, s32 character)
{
    const u8* current = (const u8*) string - 1;
    u32 wanted = character & 0xFF;
    u32 value;

    while ((value = *++current) != 0) {
        if (value == wanted) {
            return (char*) current;
        }
    }
    return wanted != 0 ? NULL : (char*) current;
}

int strncmp(const char* leftString, const char* rightString, u32 length)
{
    const u8* left = (const u8*) leftString - 1;
    const u8* right = (const u8*) rightString - 1;
    u32 leftValue;
    u32 rightValue;

    length++;
    while (--length != 0) {
        leftValue = *++left;
        rightValue = *++right;
        if (leftValue != rightValue) {
            return leftValue - rightValue;
        }
        if (leftValue == 0) {
            break;
        }
    }
    return 0;
}
