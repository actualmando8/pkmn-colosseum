#include "dolphin/types.h"

extern void MWTRACE(s32 level, const char* fmt, ...);
extern void* memset(void* dst, int val, u32 len);

extern s32  TRKTargetReadInstruction(u8* buf, u32 pc);
extern void TRKAppendBuffer_ui8(s32 a, void* buf, u32 len);

/* TRK state and CPU state structures */
extern u8 gTRKState[];
extern u8 gTRKCPUState[];

/* Breakpoint info @ 0x80313834 */
extern u8 lbl_80313834[];

/* TRK exception status structure @ 0x80313824 */
extern u8 gTRKExceptionStatus[];

/* "TargetDoStep()\n" */
extern u8 lbl_8026FB70[];

/* TRKTargetGetPC - 0x800C1548 | size 0x10 | scope none */
s32 TRKTargetGetPC(void) {
    return *(s32*)&gTRKCPUState[0x80];
}

/* TRKTargetStepOutOfRange - 0x800C1558 | size 0xB8 | scope none */
s32 TRKTargetStepOutOfRange(u32 rangeStart, u32 rangeEnd, s32 c) {
    u8* bpInfo;
    u8* cpuState;
    s32 bpType;
    u32 msr;

    if (c != 0) {
        return 0x703;
    }
    bpInfo = lbl_80313834;
    *(u32*)&bpInfo[0xC] = rangeStart;
    *(u32*)&bpInfo[0x10] = rangeEnd;
    *(s32*)&bpInfo[0x4] = 1;
    *(s32*)&bpInfo[0x0] = 1;

    MWTRACE(1, (const char*)lbl_8026FB70);

    cpuState = gTRKCPUState;
    bpType = *(s32*)&bpInfo[0x4];
    msr = *(u32*)&cpuState[0x1F8];
    msr |= 0x400;
    *(u32*)&cpuState[0x1F8] = msr;

    if (bpType == 0 || bpType == 0x10) {
        *(u32*)&lbl_80313834[0x8] = *(u32*)&lbl_80313834[0x8] - 1;
    }
    *(s32*)&gTRKState[0x98] = 0;
    return 0;
}

/* TRKTargetSingleStep - 0x800C1610 | size 0xAC | scope none */
s32 TRKTargetSingleStep(u32 count, s32 c) {
    u8* bpInfo;
    u8* cpuState;
    s32 bpType;
    u32 msr;

    if (c != 0) {
        return 0x703;
    }
    bpInfo = lbl_80313834;
    *(u32*)&bpInfo[0x8] = count;
    *(s32*)&bpInfo[0x4] = 0;
    *(s32*)&bpInfo[0x0] = 1;

    MWTRACE(1, (const char*)lbl_8026FB70);

    cpuState = gTRKCPUState;
    bpType = *(s32*)&bpInfo[0x4];
    msr = *(u32*)&cpuState[0x1F8];
    msr |= 0x400;
    *(u32*)&cpuState[0x1F8] = msr;

    if (bpType == 0 || bpType == 0x10) {
        *(u32*)&bpInfo[0x8] = *(u32*)&bpInfo[0x8] - 1;
    }
    *(s32*)&gTRKState[0x98] = 0;
    return 0;
}

/* TRKTargetAddExceptionInfo - 0x800C16BC | size 0x84 | scope none */
void TRKTargetAddExceptionInfo(s32 arg) {
    u8 buf[0x40];
    s32 result;
    u32 dataword;

    memset(buf, 0, 0x40);
    dataword = *(u32*)gTRKExceptionStatus;
    *(u32*)&buf[0x0] = 0x40;
    buf[0x4] = 0x91;
    *(u32*)&buf[0x8] = dataword;
    TRKTargetReadInstruction((u8*)&result, dataword);
    *(u32*)&buf[0xC] = result;
    *(u32*)&buf[0x10] = *(u16*)&gTRKExceptionStatus[0x8];
    TRKAppendBuffer_ui8(arg, buf, 0x40);
}

/* TRKTargetAddStopInfo - 0x800C1740 | size 0x8C | scope none */
void TRKTargetAddStopInfo(s32 arg) {
    u8 buf[0x40];
    s32 result;
    u32 dataword;

    memset(buf, 0, 0x40);
    dataword = *(u32*)&gTRKCPUState[0x80];
    *(u32*)&buf[0x0] = 0x40;
    buf[0x4] = 0x90;
    *(u32*)&buf[0x8] = dataword;
    TRKTargetReadInstruction((u8*)&result, dataword);
    *(u32*)&buf[0xC] = result;
    *(u32*)&buf[0x10] = *(u32*)&gTRKCPUState[0x2F8] & 0xFFFF;
    TRKAppendBuffer_ui8(arg, buf, 0x40);
}
