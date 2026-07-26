#include "dolphin/types.h"

extern void EnableEXI2Interrupts(void); /* EnableEXI2Interrupts */

extern u8 gTRKCPUState[];
extern u8 gTRKState[];
extern u8 lbl_803FED58[]; /* exception table base */
extern u32 __TRK_get_MSR(void);
u32 TRKTargetTranslate(u32 addr);
extern u8 gTRKInterruptVectorTable[];
extern u32 lbl_80313848[];
extern void* fn_80003488(void* dst, const void* src, u32 length);
extern void TRK_flush_cache(void* address, u32 length);

static inline void TRK_copy_vector(u32 offset) {
    void* destination = (void*)TRKTargetTranslate(offset);

    fn_80003488(destination, gTRKInterruptVectorTable + offset, 0x100);
    TRK_flush_cache(destination, 0x100);
}

s32 TRKInitializeTarget(void) {
    *(s32*)&gTRKState[0x98] = 1;
    *(u32*)&gTRKState[0x8C] = __TRK_get_MSR();
    *(u32*)lbl_803FED58 = 0xE0000000;
    return 0;
}

void __TRK_copy_vectors(void) {
    u32 base = *(u32*)lbl_803FED58;
    u32* offset;
    s32 i;
    u32 mask;

    if (base <= 0x44 && base + 0x4000 > 0x44 &&
        (*(u32*)(gTRKCPUState + 0x238) & 3) != 0)
    {
        base = 0x44;
    } else {
        base = 0x80000044;
    }

    mask = *(u32*)base;
    offset = lbl_80313848;
    i = 0;
    do {
        if ((mask & (1 << i)) && i != 4) {
            TRK_copy_vector(offset[i]);
        }
        i++;
    } while (i <= 14);
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
