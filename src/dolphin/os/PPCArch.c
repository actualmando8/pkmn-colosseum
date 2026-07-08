#include "dolphin/os/PPCArch.h"

extern u32 PPCMfhid0(void);
extern void PPCMthid0(u32 hid0);

void PPCDisableSpeculation(void) {
    u32 hid0 = PPCMfhid0();
    hid0 |= 0x200;
    PPCMthid0(hid0);
}
