#ifndef DOLPHIN_OS_PPCARCH_H
#define DOLPHIN_OS_PPCARCH_H

#include "dolphin/types.h"

u32 PPCMfmsr(void);
void PPCMtmsr(u32 val);
u32 PPCMfhid0(void);
void PPCMthid0(u32 val);
u32 PPCMfl2cr(void);
void PPCMtl2cr(u32 val);
void PPCMtdec(u32 val);
void PPCSync(void);
void PPCHalt(void);
void PPCMtmmcr0(u32 val);
void PPCMtmmcr1(u32 val);
void PPCMtpmc1(u32 val);
void PPCMtpmc2(u32 val);
void PPCMtpmc3(u32 val);
void PPCMtpmc4(u32 val);
u32 PPCMffpscr(void);
void PPCMtfpscr(u32 val);
u32 PPCMfhid2(void);
void PPCMthid2(u32 val);
void PPCMtwpar(u32 val);
void PPCDisableSpeculation(void);
void PPCSetFpNonIEEEMode(void);

#endif /* DOLPHIN_OS_PPCARCH_H */
