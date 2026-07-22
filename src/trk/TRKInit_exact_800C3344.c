#include "trk/trk.h"

extern u8 gTRKCPUState[];
extern u8 lbl_803FED58[];

u32 TRKTargetTranslate(u32 address)
{
    u32 stackBase;

    stackBase = *(u32*)lbl_803FED58;
    if (address >= stackBase && address < stackBase + 0x4000) {
        u32 msrBits;

        msrBits = *(u32*)(gTRKCPUState + 0x238) & 3;
        if (msrBits != 0) {
            return address;
        }
    }

    if (address >= 0x7E000000 && address <= 0x80000000) {
        return address;
    }

    return (address & 0x3FFFFFFF) | 0x80000000;
}

void EnableMetroTRKInterrupts(void)
{
    EnableEXI2Interrupts();
}
