#include "dolphin/types.h"

/* Low-level GDEV hardware interface functions */
extern void DBInitInterrupts(void); /* GDEV_InitInterrupts */
extern void fn_800CE7D8(void); /* GDEV_PostStop */
extern void fn_800CE7D4(void); /* GDEV_PreContinue */

/*
 * Partial source for the gdev_cc 0x800C41AC range: only the leaf helpers that
 * byte-match under GC/1.3. gdev_cc_read is a register-allocation
 * near-misses (fleet targets) and are intentionally left as extracted asm.
 */

/* gdev_cc_initinterrupts - Enable GDEV interrupts for async reception. */
s32 gdev_cc_initinterrupts(void) {
    DBInitInterrupts();
    return 0;
}

/* gdev_cc_peek - Pull pending GDEV data into the receive circle buffer. */
s32 gdev_cc_peek(void) {
    typedef struct CircleBuffer CircleBuffer;
    extern u32 DBQueryData(void);
    extern s32 DBRead(void* buffer, s32 size);
    extern CircleBuffer lbl_803FFA98;
    extern s32 CircleBufferWriteBytes(CircleBuffer* circle, u8* buffer, u32 size);
    u8 buffer[0x500];
    s32 size;

    size = DBQueryData();
    if (size <= 0) {
        return 0;
    }
    if (DBRead(buffer, size) == 0) {
        CircleBufferWriteBytes(&lbl_803FFA98, buffer, size);
    } else {
        return -0x2719;
    }
    return size;
}

/* gdev_cc_post_stop - Called after the target stops. */
s32 gdev_cc_post_stop(void) {
    fn_800CE7D8();
    return 0;
}

/* gdev_cc_pre_continue - Called before the target continues. */
s32 gdev_cc_pre_continue(void) {
    fn_800CE7D4();
    return 0;
}

/* gdev_cc_write - Write data to the GDEV debug port. */
s32 gdev_cc_write(u8* buffer, s32 size) {
    extern const char lbl_8026FD78[];
    extern s32 lbl_8047A9E8;
    extern void MWTRACE(s32 level, const char* format, ...);
    extern s32 DBWrite(u8* buffer, s32 size);
    const char* strings = lbl_8026FD78;
    s32 written;

    if (lbl_8047A9E8 == 0) {
        MWTRACE(8, strings);
        return -0x2711;
    }

    MWTRACE(8, strings + 0x14, buffer, size);
    while (size > 0) {
        MWTRACE(1, strings + 0x40, size);
        written = DBWrite(buffer, size);
        if (written == 0) {
            break;
        }
        buffer += written;
        size -= written;
    }
    return 0;
}

/* gdev_cc_read - Read data from the GDEV debug port. */
s32 gdev_cc_read(u8* buffer, s32 size) {
    typedef struct CircleBuffer CircleBuffer;
    extern const char lbl_8026FDD4[];
    extern const char lbl_8026FDFC[];
    extern s32 lbl_8047A9E8;
    extern CircleBuffer lbl_803FFA98;
    extern void MWTRACE(s32 level, const char* format, ...);
    extern u32 DBQueryData(void);
    extern s32 DBRead(void* buffer, s32 size);
    extern u32 fn_800C41A4(CircleBuffer* circle);
    extern s32 CircleBufferWriteBytes(CircleBuffer* circle, u8* buffer, u32 size);
    extern s32 CircleBufferReadBytes(CircleBuffer* circle, u8* buffer, u32 size);
    u8 readBuffer[0x500];
    u32 error = 0;

    if (lbl_8047A9E8 == 0) {
        return -0x2711;
    }

    MWTRACE(1, lbl_8026FDD4, size, size);
    {
        u32 readSize = size;

        while (fn_800C41A4(&lbl_803FFA98) < readSize) {
            error = 0;
            size = DBQueryData();
            if (size == 0) {
                continue;
            }
            error = DBRead(readBuffer, readSize);
            if (error != 0) {
                continue;
            }
            CircleBufferWriteBytes(&lbl_803FFA98, readBuffer, size);
        }

        if (error == 0) {
            CircleBufferReadBytes(&lbl_803FFA98, buffer, readSize);
        } else {
            MWTRACE(8, lbl_8026FDFC, error);
        }
    }
    return error;
}
