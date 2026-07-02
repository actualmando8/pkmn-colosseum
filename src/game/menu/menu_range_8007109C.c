/**
 * @file menu_range_8007109C.c
 * @brief menu (GBA-link/comm UI), 0x8007109C - 0x8007C260.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

/* GBA link timing: OS_TIMER_CLOCK / OSMillisecondsToTicks, see include/dolphin/si/SI.h */
#define OS_BUS_CLOCK   (*(u32*)0x800000F8)
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)
#define OSMillisecondsToTicks(msec) ((msec) * (OS_TIMER_CLOCK / 1000))

extern u32 OSGetTick(void);
extern void gbaCommandSetKeyState(s32 mode, s32 flag);
extern s32 fn_80073C38(s32 chan);
extern u32 GBAWrite(s32 chan, u32 srcPtr, u32 lenPtr);
extern u32 GBARead(s32 chan, u32 destPtr, u32 lenPtr);
extern u32 fn_8025F3F4(s32 chan, u32 statusPtr);

typedef struct GbaIdleCallback {
    void (*func)(s32 chan, void* arg);
    void* arg;
} GbaIdleCallback;

extern GbaIdleCallback lbl_803B6E18[5];
extern s32 lbl_803B6E08[4];

/* Address: 0x80071700 | Size: 0x2A8 */
#pragma peephole off
s32 fn_80071700(s32 chan) {
    s32 mode;
    s32 result;
    u32 timeout;
    u32 start;
    u32 cmdBuf;
    u32 respBuf;
    u8 statusA;
    u8 statusB;
    u8 lenA;
    u8 lenB;

    mode = chan + 1;
    gbaCommandSetKeyState(mode, 2);
    result = fn_80073C38(chan);
    if (result != 0) {
        goto done;
    }

    cmdBuf = 0x44;
    if (GBAWrite(chan, (u32)&cmdBuf, (u32)&lenA) != 0) {
        result = 0xB;
        goto done;
    }

    timeout = OSMillisecondsToTicks(100);
    start = OSGetTick();
    for (;;) {
        if ((OSGetTick() - start) > timeout) {
            result = 1;
            break;
        }
        if (fn_8025F3F4(chan, (u32)&statusA) != 0) {
            result = 2;
            break;
        }
        if ((statusA & 0xA) == 8) {
            result = 0;
            break;
        }
        if (lbl_803B6E18[chan].func != NULL) {
            lbl_803B6E18[chan].func(chan, lbl_803B6E18[chan].arg);
        }
        if (lbl_803B6E08[chan] != 0) {
            result = 0x3E8;
            break;
        }
    }

    if (result == 0) {
        if (GBARead(chan, (u32)&respBuf, (u32)&lenA) != 0) {
            result = 3;
        }
    }
    if (result != 0) {
        result = result + 0xB;
        goto done;
    }

    if ((respBuf >> 24) != 0x44) {
        result = 0xF;
        goto done;
    }

    timeout = OSMillisecondsToTicks(30000);
    start = OSGetTick();
    for (;;) {
        if ((OSGetTick() - start) > timeout) {
            result = 1;
            break;
        }
        if (fn_8025F3F4(chan, (u32)&statusB) != 0) {
            result = 2;
            break;
        }
        if ((statusB & 0xA) == 8) {
            result = 0;
            break;
        }
        if (lbl_803B6E18[chan].func != NULL) {
            lbl_803B6E18[chan].func(chan, lbl_803B6E18[chan].arg);
        }
        if (lbl_803B6E08[chan] != 0) {
            result = 0x3E8;
            break;
        }
    }

    if (result == 0) {
        if (GBARead(chan, (u32)&respBuf, (u32)&lenB) != 0) {
            result = 3;
        }
    }
    if (result != 0) {
        result = result + 0xF;
        goto done;
    }

    result = (respBuf != 0) ? 0x13 : 0;

done:
    gbaCommandSetKeyState(mode, 1);
    return result;
}
#pragma peephole reset

/* Address: 0x800722A0 | Size: 0x2A8 -- byte-identical body to fn_80071700 */
#pragma peephole off
s32 fn_800722A0(s32 chan) {
    s32 mode;
    s32 result;
    u32 timeout;
    u32 start;
    u32 cmdBuf;
    u32 respBuf;
    u8 statusA;
    u8 statusB;
    u8 lenA;
    u8 lenB;

    mode = chan + 1;
    gbaCommandSetKeyState(mode, 2);
    result = fn_80073C38(chan);
    if (result != 0) {
        goto done;
    }

    cmdBuf = 0x44;
    if (GBAWrite(chan, (u32)&cmdBuf, (u32)&lenA) != 0) {
        result = 0xB;
        goto done;
    }

    timeout = OSMillisecondsToTicks(100);
    start = OSGetTick();
    for (;;) {
        if ((OSGetTick() - start) > timeout) {
            result = 1;
            break;
        }
        if (fn_8025F3F4(chan, (u32)&statusA) != 0) {
            result = 2;
            break;
        }
        if ((statusA & 0xA) == 8) {
            result = 0;
            break;
        }
        if (lbl_803B6E18[chan].func != NULL) {
            lbl_803B6E18[chan].func(chan, lbl_803B6E18[chan].arg);
        }
        if (lbl_803B6E08[chan] != 0) {
            result = 0x3E8;
            break;
        }
    }

    if (result == 0) {
        if (GBARead(chan, (u32)&respBuf, (u32)&lenA) != 0) {
            result = 3;
        }
    }
    if (result != 0) {
        result = result + 0xB;
        goto done;
    }

    if ((respBuf >> 24) != 0x44) {
        result = 0xF;
        goto done;
    }

    timeout = OSMillisecondsToTicks(30000);
    start = OSGetTick();
    for (;;) {
        if ((OSGetTick() - start) > timeout) {
            result = 1;
            break;
        }
        if (fn_8025F3F4(chan, (u32)&statusB) != 0) {
            result = 2;
            break;
        }
        if ((statusB & 0xA) == 8) {
            result = 0;
            break;
        }
        if (lbl_803B6E18[chan].func != NULL) {
            lbl_803B6E18[chan].func(chan, lbl_803B6E18[chan].arg);
        }
        if (lbl_803B6E08[chan] != 0) {
            result = 0x3E8;
            break;
        }
    }

    if (result == 0) {
        if (GBARead(chan, (u32)&respBuf, (u32)&lenB) != 0) {
            result = 3;
        }
    }
    if (result != 0) {
        result = result + 0xF;
        goto done;
    }

    result = (respBuf != 0) ? 0x13 : 0;

done:
    gbaCommandSetKeyState(mode, 1);
    return result;
}
#pragma peephole reset
