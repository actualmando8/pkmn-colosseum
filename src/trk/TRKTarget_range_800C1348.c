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

/* TRKPostInterruptEvent - 0x800C195C | size 0xAC | scope global */
void TRKPostInterruptEvent(void) {
    u32 instruction;
    u8 event[0xC];
    s32 eventType;

    extern void fn_800BE464(void* event, s32 type);
    extern s32 TRKPostEvent(void* event);

    if (*(s32*)&gTRKState[0x9C] != 0) {
        *(s32*)&gTRKState[0x9C] = 0;
    } else {
        s32 exceptionID = *(s32*)&gTRKCPUState[0x2F8] & 0xFFFF;

        switch (exceptionID) {
        case 0x700:
        case 0xD00:
            TRKTargetReadInstruction((u8*)&instruction, *(u32*)&gTRKCPUState[0x80]);
            if (instruction == 0x0FE00000) {
                eventType = 5;
            } else {
                eventType = 3;
            }
            break;
        default:
            eventType = 4;
            break;
        }
        fn_800BE464(event, eventType);
        TRKPostEvent(event);
    }
}

/* TRKTargetAccessDefault - 0x800C24BC | size 0xF4 | scope global */
s32 TRKTargetAccessDefault(u32 firstRegister, u32 lastRegister, s32 buffer,
                           u32* transferSize, s32 read) {
    typedef struct TRKExceptionState {
        u32 words[4];
    } TRKExceptionState;

    extern s32 TRKAppendBuffer_ui32(s32 buffer, u32* data, u32 count);
    extern s32 TRKReadBuffer_ui32(s32 buffer, u32* data, u32 count);
    extern u8 gTRKExceptionStatus_80313824[];

    s32 result;
    u32 registerCount;
    u32* registers;
    TRKExceptionState savedState;

    if (lastRegister > 0x24) {
        return 0x701;
    }

    registerCount = lastRegister - firstRegister + 1;
    savedState = *(TRKExceptionState*)gTRKExceptionStatus_80313824;
    gTRKExceptionStatus_80313824[0xD] = 0;
    registers = (u32*)gTRKCPUState + firstRegister;
    *transferSize = registerCount * sizeof(u32);

    if (read != 0) {
        result = TRKAppendBuffer_ui32(buffer, registers, registerCount);
    } else {
        result = TRKReadBuffer_ui32(buffer, registers, registerCount);
    }

    if (gTRKExceptionStatus_80313824[0xD] != 0) {
        *transferSize = 0;
        result = 0x702;
    }

    *(TRKExceptionState*)gTRKExceptionStatus_80313824 = savedState;
    return result;
}

/* TRKTargetReadInstruction - 0x800C25B0 | size 0x4C | scope global */
s32 TRKTargetReadInstruction(u8* buf, u32 pc) {
    extern s32 TRKTargetAccessMemory(u8* buf, u32 address, u32* length,
                                     s32 memorySpace, s32 read);

    u32 length = sizeof(u32);
    s32 result = TRKTargetAccessMemory(buf, pc, &length, 0, 1);

    if (result == 0 && length != sizeof(u32)) {
        result = 0x700;
    }
    return result;
}

/* TRKAccessFile - 0x800C29F0 | size: 0x8 | scope global */
u32 TRKAccessFile(u32 cmd, u32 dir, u32* addrBuf, u32 len) {
    (void)cmd;
    (void)dir;
    (void)addrBuf;
    (void)len;
    return 0;
}

/* TRKOpenFile - 0x800C29F8 | size: 0x8 | scope global */
u32 TRKOpenFile(u32 cmd, u32 dir, u32* addrBuf, u32 len) {
    (void)cmd;
    return TRKAccessFile(0xD2, dir, addrBuf, len);
}

/* TRKCloseFile - 0x800C2A00 | size: 0x8 | scope global */
u32 TRKCloseFile(u32 cmd, u32 param) {
    (void)cmd;
    return TRKAccessFile(cmd, param, NULL, 0);
}

/* TRKPositionFile - 0x800C2A08 | size: 0x8 | scope global */
u32 TRKPositionFile(u32 cmd, u32 dir, u32* addrBuf, u32 len) {
    (void)cmd;
    return TRKAccessFile(0xD4, dir, addrBuf, len);
}
