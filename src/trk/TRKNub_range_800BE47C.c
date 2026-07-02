#include "dolphin/types.h"

extern void MWTRACE(s32 level, const char* fmt, ...);

extern void fn_80003488(void* dst, const void* src, u32 size);
extern void fn_800BF080(void);        /* TRKTerminateSerialHandler */
extern s32  fn_800C3588(void* dst, u32 val);

extern void TRK_board_display(const char* msg);

/* Endianness flag (1=big, 0=little) */
extern u8 gTRKBigEndian[];

/* "MessageSend : cc_write returned %ld\n" */
extern char lbl_8026F640[];

typedef struct TRKMessageBuffer {
    s32 mutex;    /* 0x00 */
    s32 inUse;    /* 0x04 */
    u32 length;   /* 0x08 */
    u32 position; /* 0x0C */
    u8 data[0x880]; /* 0x10 */
} TRKMessageBuffer;

/* TRKNubWelcome - 0x800BE668 | size 0x28 | scope global */
void TRKNubWelcome(void) {
    TRK_board_display("MetroTRK for GAMECUBE v2.6");
}

/* TRKTerminateNub - 0x800BE690 | size 0x24 | scope global */
s32 TRKTerminateNub(void) {
    fn_800BF080();
    return 0;
}

/* MessageSend - 0x800BE800 | size 0x44 | scope none */
s32 MessageSend(u8* p) {
    s32 r = fn_800C3588(p + 0x10, *(u32*)(p + 0x8));
    MWTRACE(1, lbl_8026F640, r);
    return 0;
}

/* TRKAppendBuffer_ui8 - 0x800BEBB0 | size 0x68 | scope none */
s32 TRKAppendBuffer_ui8(u8* buf, u8* src, s32 count) {
    u8 b;
    s32 i = 0;
    s32 err = 0;

    while (err == 0 && i < count) {
        u32 r = *(u32*)(buf + 0xC);
        b = *src;
        if (r >= 0x880) {
            r = 0x301;
        } else {
            *(u32*)(buf + 0xC) = r + 1;
            *(buf + 0x10 + r) = b;
            r = 0x0;
            *(u32*)(buf + 0x8) = *(u32*)(buf + 0x8) + 1;
        }
        err = r;
        i = i + 1;
        src = src + 1;
    }

    return err;
}

/* TRKAppendBuffer1_ui64 - 0x800BEC18 | size 0xFC | scope none */
s32 TRKAppendBuffer1_ui64(u8* buf, u32 unused, u32 w0, u32 w1) {
    u8 swap[8];
    u8 raw[8];
    u8* src;
    u32 writepos;
    s32 err;
    u32 n;

    *(u32*)&raw[0] = w0;
    *(u32*)&raw[4] = w1;

    if (*(s32*)gTRKBigEndian != 0) {
        src = raw;
    } else {
        swap[0] = raw[7];
        swap[1] = raw[6];
        swap[2] = raw[5];
        swap[3] = raw[4];
        swap[4] = raw[3];
        swap[5] = raw[2];
        swap[6] = raw[1];
        swap[7] = raw[0];
        src = swap;
    }

    writepos = *(u32*)(buf + 0xC);
    n = 8;
    err = 0;
    if (0x880 - writepos < 8) {
        err = 0x301;
        n = 0x880 - writepos;
    }
    if (n == 1) {
        buf[writepos + 0x10] = *src;
    } else {
        fn_80003488(buf + writepos + 0x10, src, n);
    }
    *(u32*)(buf + 0xC) = *(u32*)(buf + 0xC) + n;
    *(u32*)(buf + 0x8) = *(u32*)(buf + 0xC);
    return err;
}

/* TRKReadBuffer - 0x800BED14 | size 0x8C | scope none */
s32 TRKReadBuffer(TRKMessageBuffer* buf, u8* dst, u32 n) {
    s32 err = 0;
    if (n == 0) {
        return 0;
    }
    {
        u32 position = buf->position;
        u32 length = buf->length;
        u32 space = length - position;
        if (n > space) {
            err = 0x302;
            n = space;
        }
        fn_80003488(dst, &buf->data[position], n);
        buf->position += n;
    }
    return err;
}

/* TRKAppendBuffer - 0x800BEDA0 | size 0xA4 | scope none */
s32 TRKAppendBuffer(TRKMessageBuffer* buf, u8* src, u32 n) {
    s32 err = 0;
    if (n == 0) {
        return 0;
    }
    {
        u32 position = buf->position;
        u32 space = sizeof(buf->data) - position;
        if (space < n) {
            err = 0x301;
            n = space;
        }
        if (n == 1) {
            buf->data[position] = src[0];
        } else {
            fn_80003488(&buf->data[position], src, n);
        }
        buf->position += n;
        buf->length = buf->position;
    }
    return err;
}
