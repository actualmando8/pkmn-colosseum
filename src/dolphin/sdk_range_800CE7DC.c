#include "dolphin/types.h"

#define FALSE 0
#define TRUE 1
#define IS_TRUE(value) ((value) != FALSE)
#define IS_FALSE(value) (!IS_TRUE(value))
#define ROUND_UP(value, align) (((value) + (align) - 1) & -(align))

volatile u32 __EXIRegs[0x40] : 0xCC006800;

extern u8 lbl_80478AD0;
extern BOOL OSDisableInterrupts(void);
extern void OSRestoreInterrupts(BOOL interrupts);
extern BOOL DBGEXIImm(void* buffer, s32 length, u32 write);
extern BOOL DBGWrite(u32 address, void* buffer, s32 length);

static inline BOOL DBGEXISelect(u32 value)
{
    u32 regs = __EXIRegs[10];
    regs &= 0x405;
    regs |= 0x80 | (value << 4);
    __EXIRegs[10] = regs;
    return TRUE;
}

static inline BOOL DBGEXIDeselect(void)
{
    __EXIRegs[10] &= 0x405;
    return TRUE;
}

static inline BOOL DBGEXISync(void)
{
    while (__EXIRegs[13] & 1) {
    }
    return TRUE;
}

static inline BOOL DBGWriteMailbox(u32 value)
{
    BOOL total = FALSE;
    u32 packet;

    if (!DBGEXISelect(4)) {
        return FALSE;
    }

    packet = (value & 0x1FFFFFFF) | 0xC0000000;
    total |= IS_FALSE(DBGEXIImm(&packet, sizeof(packet), 1));
    total |= IS_FALSE(DBGEXISync());
    total |= IS_FALSE(DBGEXIDeselect());
    return IS_FALSE(total);
}

static inline BOOL DBGReadStatus(u32* status)
{
    BOOL total = FALSE;
    u32 command;

    if (!DBGEXISelect(4)) {
        return FALSE;
    }

    command = 1 << 30;
    total |= IS_FALSE(DBGEXIImm(&command, 2, 1));
    total |= IS_FALSE(DBGEXISync());
    total |= IS_FALSE(DBGEXIImm(status, 4, 0));
    total |= IS_FALSE(DBGEXISync());
    total |= IS_FALSE(DBGEXIDeselect());
    return IS_FALSE(total);
}

BOOL DBWrite(const void* source, u32 size)
{
    u32 address;
    u32 busy;
    BOOL interrupts;

    interrupts = OSDisableInterrupts();

    do {
        DBGReadStatus(&busy);
    } while (busy & 2);

    lbl_80478AD0++;
    address = (lbl_80478AD0 & 1) ? 0x1000 : 0;
    while (!DBGWrite(address | 0x1C000, (void*)source, ROUND_UP(size, 4))) {
    }

    do {
        DBGReadStatus(&busy);
    } while (busy & 2);

    address = (lbl_80478AD0 << 16) | 0x1F000000 | size;
    while (!DBGWriteMailbox(address)) {
    }

    do {
        while (!DBGReadStatus(&busy)) {
        }
    } while (busy & 2);

    OSRestoreInterrupts(interrupts);
    return FALSE;
}
