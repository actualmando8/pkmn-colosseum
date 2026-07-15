/**
 * @file sdk_range_800CEB64.c
 * @brief dolphin-sdk code, 0x800CEB64 - 0x800CF708 (13 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "dolphin/os/OSInterrupt.h"

typedef void (*DBGInterruptHandler)(__OSInterrupt interrupt, OSContext* context);

extern DBGInterruptHandler lbl_8047AA2C;
extern void (*lbl_8047AA28)(s32);
extern u8* lbl_8047AA38;
extern u8 lbl_8047AA3C;

void DBGHandler(s32 interrupt, OSContext* context);
void MWCallback(void);

volatile u32 EXI_REGS[16] : 0xCC006800;

void DBInitInterrupts(void) {
    __OSMaskInterrupts(0x18000);
    __OSMaskInterrupts(0x40);
    lbl_8047AA2C = (__OSInterruptHandler)MWCallback;
    __OSSetInterruptHandler(0x19, (__OSInterruptHandler)DBGHandler);
    __OSUnmaskInterrupts(0x40);
}

void DBInitComm(u8** comm, void (*callback)(s32)) {
    BOOL interrupts = OSDisableInterrupts();

    lbl_8047AA38 = &lbl_8047AA3C;
    *comm = lbl_8047AA38;
    lbl_8047AA28 = callback;
    __OSMaskInterrupts(0x18000);
    EXI_REGS[10] = 0;
    OSRestoreInterrupts(interrupts);
}

extern u8 lbl_8047AA3C;

volatile u32 lbl_CC003000 : 0xCC003000;

void DBGHandler(s32 interrupt, OSContext* context) {
    lbl_CC003000 = 0x1000;
    if (lbl_8047AA2C != NULL) {
        lbl_8047AA2C((__OSInterrupt)interrupt, context);
    }
}

void MWCallback(void) {
    lbl_8047AA3C = 1;
    if (lbl_8047AA28 != NULL) {
        lbl_8047AA28(0);
    }
}

#define IS_TRUE(value) ((value) != FALSE)
#define IS_FALSE(value) !IS_TRUE(value)

inline static u32 DBGEXISelect(u32 value) {
    u32 regs = EXI_REGS[10];

    regs &= 0x405;
    regs |= 0x80 | (value << 4);
    EXI_REGS[10] = regs;
    return TRUE;
}

inline static BOOL DBGEXISync(void) {
    while (EXI_REGS[13] & 1) {
    }
    return TRUE;
}

inline static BOOL DBGEXIDeselect(void) {
    EXI_REGS[10] &= 0x405;
    return TRUE;
}

s32 fn_800CECAC(u32* data) {
    extern s32 DBGEXIImm(void* buffer, s32 length, s32 write);
    BOOL total = FALSE;
    u32 value;

    DBGEXISelect(4);

    value = 1 << 30;
    total |= IS_FALSE(DBGEXIImm(&value, 2, 1));
    total |= IS_FALSE(DBGEXISync());

    total |= IS_FALSE(DBGEXIImm(data, 4, 0));
    total |= IS_FALSE(DBGEXISync());

    total |= IS_FALSE(DBGEXIDeselect());

    return IS_FALSE(total);
}

s32 DBGWrite(u32 type, u32* data, s32 length) {
    extern s32 DBGEXIImm(void* buffer, s32 length, s32 write);
    BOOL total;
    u32* current = data;
    u32 command;
    u32 value;
    volatile u32 scratch[3];

    DBGEXISelect(4);
    command = ((type << 8) & 0x01FFFC00) | 0xA0000000;

    total = IS_FALSE(DBGEXIImm(&command, 4, 1));
    DBGEXISync();

    while (length != 0) {
        value = *current++;
        total |= IS_FALSE(DBGEXIImm(&value, 4, 1));
        DBGEXISync();

        length -= 4;
        if (length < 0) {
            length = 0;
        }
    }

    DBGEXIDeselect();
    return IS_FALSE(total);
}

s32 DBGRead(u32 type, u32* data, s32 length) {
    extern s32 DBGEXIImm(void* buffer, s32 length, s32 write);
    BOOL total;
    u32* current = data;
    u32 command;
    u32 value;
    volatile u32 scratch[3];

    DBGEXISelect(4);
    command = ((type << 8) & 0x01FFFC00) | 0x20000000;

    total = IS_FALSE(DBGEXIImm(&command, 4, 1));
    DBGEXISync();

    while (length != 0) {
        total |= IS_FALSE(DBGEXIImm(&value, 4, 0));
        DBGEXISync();
        *current++ = value;

        length -= 4;
        if (length < 0) {
            length = 0;
        }
    }

    DBGEXIDeselect();
    return IS_FALSE(total);
}

s32 fn_800CEF10(u32* data) {
    extern s32 DBGEXIImm(void* buffer, s32 length, s32 write);
    BOOL total = FALSE;
    u32 value;

    DBGEXISelect(4);

    value = 3 << 29;
    total |= IS_FALSE(DBGEXIImm(&value, 2, 1));
    total |= IS_FALSE(DBGEXISync());

    total |= IS_FALSE(DBGEXIImm(data, 4, 0));
    total |= IS_FALSE(DBGEXISync());

    total |= IS_FALSE(DBGEXIDeselect());

    return IS_FALSE(total);
}

typedef struct EXIProbeControl {
    u8 pad_00[0x20];
    s32 idTime;
    u8 pad_24[0x1C];
} EXIProbeControl;

volatile s32 EXIProbeStartTime[2] : 0x800030C0;

#pragma push
#pragma scheduling off
s32 EXIProbeEx(s32 chan) {
    extern BOOL __EXIProbe(s32 chan);
    extern BOOL fn_80099400(s32 chan, u32 dev, u32* id);
    extern EXIProbeControl lbl_803FFEF0[];
    EXIProbeControl* exi = &lbl_803FFEF0[chan];
    u32 id;
    BOOL probe;

    probe = __EXIProbe(chan);
    if (probe && exi->idTime == 0) {
        probe = fn_80099400(chan, 0, &id) ? TRUE : FALSE;
    }
    if (probe) {
        return 1;
    }
    if (EXIProbeStartTime[chan] != 0) {
        return 0;
    }
    return -1;
}
#pragma scheduling reset
#pragma pop

s32 InitializeUART(u32 baud) {
    extern u32 OSGetConsoleType(void);
    extern u32 lbl_8047AA40;
    extern u32 lbl_8047AA44;
    extern u32 lbl_8047AA48;
    extern u32 lbl_8047AA4C;

    if (lbl_8047AA4C == 0xA5FF005A) {
        return 0;
    }
    if ((OSGetConsoleType() & 0x10000000) == 0) {
        lbl_8047AA48 = 0;
        return 2;
    }
    lbl_8047AA48 = 0xA5FF005A;
    lbl_8047AA40 = 0;
    lbl_8047AA44 = 1;
    return 0;
}
