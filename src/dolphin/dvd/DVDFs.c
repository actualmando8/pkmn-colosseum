#include "dolphin/dvd/dvd.h"

typedef struct DVDFstEntry {
    u32 typeAndNameOffset;
    u32 parentOrStart;
    u32 nextOrLength;
} DVDFstEntry;

extern u32* BootInfo;
extern DVDFstEntry* FstStart;
extern char* FstStringStart;
extern u32 MaxEntryNum;

void __DVDFSInit(void) {
    BootInfo = (u32*)0x80000000;
    FstStart = (DVDFstEntry*)BootInfo[0x38 / 4];

    if (FstStart != NULL) {
        MaxEntryNum = FstStart[0].nextOrLength;
        FstStringStart = (char*)&FstStart[MaxEntryNum];
    }
}
