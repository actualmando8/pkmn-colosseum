#include "dolphin/types.h"

extern void* __DBInterface;
extern s32 DBVerbose;
extern void __DBExceptionDestination(void);

void DBInit(void) {
    __DBInterface = (void*)0x80000040;
    *(u32*)0x80000048 = (u32)__DBExceptionDestination + 0x80000000;
    DBVerbose = 1;
}
