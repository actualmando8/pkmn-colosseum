#include "dolphin/types.h"

typedef struct FlagStateEntry {
    u32 wordCount;
    u32* buffer;
} FlagStateEntry;

extern FlagStateEntry* lbl_80478EEC;
extern const char lbl_802742B8[];
extern void GSlogWrite(const char* format, ...);

void GSflagClear(s32 level)
{
    FlagStateEntry* states;
    u32* buffer;
    u32 wordCount;
    u32 index;

    states = lbl_80478EEC;
    buffer = states[level].buffer;
    if (buffer == NULL) {
        GSlogWrite(lbl_802742B8);
    } else {
        wordCount = states[level].wordCount;
        for (index = 0; index < wordCount; index++) {
            buffer[index] = 0;
        }
    }
}
