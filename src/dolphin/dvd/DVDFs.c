#include "dolphin/dvd/dvd.h"

typedef struct DVDFstEntry {
    u32 typeAndNameOffset;
    u32 parentOrStart;
    u32 nextOrLength;
} DVDFstEntry;

u32 MaxEntryNum;
char* FstStringStart_8047A7D0;
DVDFstEntry* FstStart_8047A7CC;
u32* BootInfo;

void __DVDFSInit(void) {
    BootInfo = (u32*)0x80000000;
    FstStart_8047A7CC = (DVDFstEntry*)BootInfo[0x38 / 4];

    if (FstStart_8047A7CC != NULL) {
        MaxEntryNum = FstStart_8047A7CC[0].nextOrLength;
        FstStringStart_8047A7D0 = (char*)&FstStart_8047A7CC[MaxEntryNum];
    }
}
