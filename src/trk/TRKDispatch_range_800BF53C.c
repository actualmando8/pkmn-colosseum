#include "dolphin/types.h"

typedef struct TRKBuffer {
    s32 mutex;
    s32 inUse;
    u32 length;
    u32 position;
    u8 data[0x880];
} TRKBuffer;

typedef struct TRKEvent {
    s32 type;
    s32 id;
    s32 bufferIndex;
} TRKEvent;

typedef struct TRKReply {
    u32 length;
    u8 command;
    u8 pad_05[3];
    u8 error;
    u8 pad_09[0x37];
} TRKReply;

extern s32 lbl_803FE7D0[3];
extern char lbl_8026F88C[];

extern void* memset(void* destination, s32 value, u32 size);
extern s32 fn_800C3588(void* message, u32 length);
extern void MWTRACE(s32 level, const char* format, ...);
extern s32 fn_800C1310(void);
extern s32 TRKTargetStopped(void);
extern s32 TRKTargetContinue(void);
extern void __TRK_copy_vectors(void);
extern void fn_800BE464(TRKEvent* event, s32 type);
extern s32 TRKPostEvent(TRKEvent* event);
extern s32 usr_puts_serial(char* text);
extern void fn_800C39B0(u8 enabled);
extern char lbl_8026F858[];

static inline s32 TRKStandardACK(TRKBuffer* buffer, u8 error)
{
    TRKReply reply;

    memset(&reply, 0, sizeof(reply));
    reply.command = 0x80;
    reply.length = sizeof(reply);
    reply.error = error;
    fn_800C3588(&reply, sizeof(reply));
    return 0;
}

s32 TRKDoSetOption(TRKBuffer* buffer)
{
    u8 enabled = buffer->data[12];

    if (buffer->data[8] == 1) {
        usr_puts_serial(lbl_8026F858);
        if (enabled != 0) {
            usr_puts_serial(lbl_8026F858 + 0x20);
        } else {
            usr_puts_serial(lbl_8026F858 + 0x28);
        }
        fn_800C39B0(enabled);
    }
    TRKStandardACK(buffer, 0);
    return 0;
}

s32 TRKDoStop(TRKBuffer* buffer)
{
    u8 error;

    switch (fn_800C1310()) {
    case 0:
        error = 0;
        break;
    case 0x704:
        error = 0x21;
        break;
    case 0x705:
        error = 0x22;
        break;
    case 0x706:
        error = 0x20;
        break;
    default:
        error = 1;
        break;
    }
    return TRKStandardACK(buffer, error);
}

s32 TRKDoContinue(TRKBuffer* buffer)
{
    MWTRACE(1, lbl_8026F88C);
    if (TRKTargetStopped() == 0) {
        TRKStandardACK(buffer, 0x16);
        return 0;
    }

    TRKStandardACK(buffer, 0);
    return TRKTargetContinue();
}

s32 fn_800C034C(TRKBuffer* buffer)
{
    return 0;
}

s32 fn_800C0354(TRKBuffer* buffer)
{
    return 0;
}

s32 fn_800C035C(TRKBuffer* buffer)
{
    TRKStandardACK(buffer, 0);
    __TRK_copy_vectors();
    return 0;
}

s32 TRKDoDisconnect(TRKBuffer* buffer)
{
    TRKEvent event;

    lbl_803FE7D0[0] = 0;
    TRKStandardACK(buffer, 0);
    fn_800BE464(&event, 1);
    TRKPostEvent(&event);
    return 0;
}

s32 TRKDoConnect(TRKBuffer* buffer)
{
    lbl_803FE7D0[0] = 1;
    return TRKStandardACK(buffer, 0);
}
