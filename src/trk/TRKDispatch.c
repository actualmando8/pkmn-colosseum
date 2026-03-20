#include "dolphin/types.h"

/*
 * TRKDispatch.c - TRK message dispatcher.
 *
 * Routes incoming debugger messages to the appropriate command handler
 * based on the command byte in the message buffer.
 */

extern void MWTRACE(s32 level, const char* fmt, ...);
extern void fn_800BEE44(void);  /* TRKResetBuffer */

/* Command handler functions - each takes a buffer pointer and returns error */
extern s32 fn_800BF53C(void* buffer);  /* cmd 0x00: Ping */
extern s32 fn_800BF5E4(void* buffer);  /* cmd 0x01: Connect */
extern s32 fn_800BF68C(void* buffer);  /* cmd 0x02: Disconnect */
extern s32 fn_800BF8AC(void* buffer);  /* cmd 0x03: Reset */
extern s32 fn_800BF95C(void* buffer);  /* cmd 0x04: Versions */
extern s32 fn_800BFBEC(void* buffer);  /* cmd 0x05: SupportMask */
extern s32 fn_800BFECC(void* buffer);  /* cmd 0x06: CPUType */
extern s32 fn_800C0108(void* buffer);  /* cmd 0x07: ReadMemory */
extern s32 fn_800C034C(void* buffer);  /* cmd 0x08: WriteMemory */
extern s32 fn_800C0354(void* buffer);  /* cmd 0x09: ReadRegisters */
extern s32 fn_800C035C(void* buffer);  /* cmd 0x0A: WriteRegisters */
extern s32 fn_800C03B4(void* buffer);  /* cmd 0x10: Continue */
extern s32 fn_800C040C(void* buffer);  /* cmd 0x11: Step */
extern s32 fn_800C0484(void* buffer);  /* cmd 0x12: Stop */

/* Jump table at jumptable_80313770 - handled by switch in compiler */

/*
 * TRKDispatchMessage - Dispatch a message to its command handler.
 *
 * Reads the command byte from offset 0x14 in the buffer, then
 * uses a jump table to call the appropriate handler function.
 * Commands 0x00 through 0x1A are valid; others return error 0x500.
 */
s32 TRKDispatchMessage(void* buffer) {
    s32 result = 0x500;
    u8 cmd;

    fn_800BEE44(); /* reset buffer position */

    cmd = ((u8*)buffer)[0x14];
    MWTRACE(1, "TRKDispatchMessage: cmd=0x%02x\n", (u32)cmd);

    switch (cmd) {
        case 0x00: /* Ping */
            result = fn_800BF53C(buffer);
            break;
        case 0x01: /* Connect */
            result = fn_800BF5E4(buffer);
            break;
        case 0x02: /* Disconnect */
            result = fn_800BF68C(buffer);
            break;
        case 0x03: /* Reset */
            result = fn_800BF8AC(buffer);
            break;
        case 0x04: /* Versions */
            result = fn_800BF95C(buffer);
            break;
        case 0x05: /* SupportMask */
            result = fn_800BFBEC(buffer);
            break;
        case 0x06: /* CPUType */
            result = fn_800BFECC(buffer);
            break;
        case 0x07: /* ReadMemory */
            result = fn_800C0108(buffer);
            break;
        case 0x08: /* WriteMemory */
            result = fn_800C034C(buffer);
            break;
        case 0x09: /* ReadRegisters */
            result = fn_800C0354(buffer);
            break;
        case 0x0A: /* WriteRegisters */
            result = fn_800C035C(buffer);
            break;
        case 0x10: /* Continue */
            result = fn_800C03B4(buffer);
            break;
        case 0x11: /* Step */
            result = fn_800C040C(buffer);
            break;
        case 0x12: /* Stop */
            result = fn_800C0484(buffer);
            break;
        default:
            break;
    }

    MWTRACE(1, "TRKDispatchMessage: result=%d\n", result);
    return result;
}

/*
 * TRKInitializeDispatcher - Initialize the message dispatcher.
 * Currently a no-op; returns success.
 */
s32 TRKInitializeDispatcher(void) {
    return 0;
}
