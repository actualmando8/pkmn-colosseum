#include "dolphin/types.h"

extern void* windowGetFreeWork(void* window);
extern void* windowGetKeyInfo(void);

void winMsgButton(void* window)
{
    void* work = windowGetFreeWork(window);
    void* keyInfo = windowGetKeyInfo();

    *(u32*)((u8*)work + 0x8) = *(u16*)((u8*)keyInfo + 0x4);
}
