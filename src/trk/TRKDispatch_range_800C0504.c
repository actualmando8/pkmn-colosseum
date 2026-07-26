#include "dolphin/types.h"

extern char lbl_8026FA34[];
extern char lbl_8026FA3C[];
extern void MWTRACE(s32 level, const char* format, ...);

void OutputData(void* data, s32 length)
{
    u8* current = data;
    s32 i;

    for (i = 0; i < length; i++, current++) {
        MWTRACE(8, lbl_8026FA34, *current);
        if (i % 16 == 15) {
            MWTRACE(8, lbl_8026FA3C);
        }
    }
    MWTRACE(8, lbl_8026FA3C);
}
