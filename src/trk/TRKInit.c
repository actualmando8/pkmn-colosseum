#include "dolphin/types.h"

extern void EnableEXI2Interrupts(void); /* EnableEXI2Interrupts */

extern u8 gTRKCPUState[];
extern u8 gTRKState[];
extern u8 lbl_803FED58[]; /* exception table base */
extern u32 __TRK_get_MSR(void);

s32 TRKInitializeTarget(void) {
    *(s32*)&gTRKState[0x98] = 1;
    *(u32*)&gTRKState[0x8C] = __TRK_get_MSR();
    *(u32*)lbl_803FED58 = 0xE0000000;
    return 0;
}

/* TRKTargetTranslate - 0x800C3344 | size 0x58 | scope none */
u32 TRKTargetTranslate(u32 addr) {
    u32 stackBase = *(u32*)lbl_803FED58;

    if (addr >= stackBase && addr < stackBase + 0x4000) {
        u32 msrBits = *(u32*)((u8*)gTRKCPUState + 0x238) & 0x3;
        if (msrBits != 0) {
            return addr;
        }
    }

    if (addr >= 0x7E000000 && addr <= 0x80000000) {
        return addr;
    }

    return (addr & 0x3FFFFFFF) | 0x80000000;
}

/* EnableMetroTRKInterrupts - 0x800C339C | size 0x20 | scope global */
void EnableMetroTRKInterrupts(void) {
    EnableEXI2Interrupts();
}
