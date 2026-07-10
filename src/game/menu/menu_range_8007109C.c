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
extern u32 GBAGetStatus(s32 chan, u32 statusPtr);

typedef struct GbaIdleCallback {
    void (*func)(s32 chan, void* arg);
    void* arg;
} GbaIdleCallback;

extern GbaIdleCallback lbl_803B6E18[5];
extern s32 lbl_803B6E08[4];
extern u8 lbl_803B6D88[0x58];
extern void floorLink(s32, s32);

/*
 * Small helpers ported from the previous campaign's archive
 * (archive/previous_campaign/src/game/menu/menu_common_ext.c). Each is
 * a trivial, self-contained accessor/no-op operating on the GBA-link
 * FightSeq-style call stack at lbl_803B6D88; re-verified against this
 * unit's own compiler flags rather than copied wholesale.
 */

/* fn_8007162C (0x8007162C): peek the current call-stack depth slot. */
s32 fn_8007162C(void) {
    u32 depth;

    depth = *(u32*)(lbl_803B6D88 + 0x40);
    return *(s32*)(lbl_803B6D88 + depth * 8);
}

/* fn_800716C8 (0x800716C8): register a per-channel idle callback
 * (archive's "return 0" stub was incomplete -- disassembly shows it
 * actually populates lbl_803B6E18[chan]). */
s32 fn_800716C8(s32 chan, void* arg, void (*func)(s32 chan, void* arg)) {
    lbl_803B6E18[chan].func = func;
    lbl_803B6E18[chan].arg = arg;
    return 0;
}

/* fn_800716E8 (0x800716E8): store a per-channel value, always returns 0. */
s32 fn_800716E8(s32 channel, s32 value) {
    lbl_803B6E08[channel] = value;
    return 0;
}

/* fn_8007169C (0x8007169C): fixed diagnostic-log call, always returns 0. */
#pragma push
#pragma peephole off
s32 fn_8007169C(void) {
    floorLink(0x385, 0);
    return 0;
}
#pragma pop

/*
 * Trivial single-call tail-wrappers ported from the previous campaign's
 * archive (archive/previous_campaign/src/game/menu/menu_tool2.c);
 * re-verified against this unit's own compiler flags.
 */
extern s32 fn_80190528(s32);
extern s32 fn_801902E0(s32);
extern s32 fadeCheck(s32);
extern s32 menuClose(s32);
extern s32 fn_801906A0(s32);
extern void _flagSet();

#pragma push
#pragma scheduling off
s32 fn_80075A9C(void) { return fn_80190528(0xab5); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075AC0(void) { return fn_801902E0(0xab5); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075AE4(void) { return fn_80190528(0xab4); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075B08(void) { return fn_801902E0(0xab4); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075B2C(void) { return fn_80190528(0xab3); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075B50(void) { return fn_801902E0(0xab3); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075BFC(void) { return fn_80190528(0xab1); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075C20(void) { return fn_801902E0(0xab1); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075C44(void) { return fn_801902E0(0xa14); }
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80075C68(void) {
    fadeCheck(1);
    return menuClose(0xe0);
}
#pragma pop

/* fn_80075B74 (0x80075B74) and fn_80075BC4 (0x80075BC4) -- REJECTED,
 * could not reach 100% (best attempts 76.7% / 37.8%; MWCC's exact
 * branchless-clamp codegen for these two counter helpers did not fall
 * out of several equivalent C phrasings tried). Left unimplemented
 * (still asm-only) rather than landed at <100%. */

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
        if (GBAGetStatus(chan, (u32)&statusA) != 0) {
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
        if (GBAGetStatus(chan, (u32)&statusB) != 0) {
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
        if (GBAGetStatus(chan, (u32)&statusA) != 0) {
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
        if (GBAGetStatus(chan, (u32)&statusB) != 0) {
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

/*
 * More small helpers ported from the previous campaign's archive
 * (menu_common_ext.c, menu_tool.c, menu_tool2.c, menu_exdisc.c);
 * re-verified against this unit's own compiler flags.
 */
extern void OSResumeThread(u32);
extern u32 lbl_8047A600;

/* fn_80072684 (0x80072684): resume the thread stashed at lbl_8047A600. */
void fn_80072684(void) {
    OSResumeThread(lbl_8047A600);
}

/* fn_80073E84 (0x80073E84): constant-1 accessor. */
s32 fn_80073E84(void) {
    return 1;
}

extern s32 menuIsCheck(s32);
/* fn_80075638 (0x80075638): tail-call wrapper. */
#pragma push
#pragma scheduling off
s32 fn_80075638(void) { return menuIsCheck(0xd8); }
#pragma pop

extern void fn_801CB9D8(u32);
extern u8 lbl_8047A5D0;
/* fn_800757F0 (0x800757F0): release and clear the handle at lbl_8047A5D0. */
void fn_800757F0(void) {
    fn_801CB9D8(*(u32*)&lbl_8047A5D0);
    *(u32*)&lbl_8047A5D0 = 0;
}

/* fn_80075D98 (0x80075D98): no-op. */
void fn_80075D98(void) {
}

extern s32 fadeCheck(s32);
extern s32 menuClose(s32);
/* fn_80075D9C (0x80075D9C): tail-call wrapper. */
#pragma push
#pragma scheduling off
s32 fn_80075D9C(void) {
    fadeCheck(1);
    return menuClose(0xe2);
}
#pragma pop

extern s32 fn_80165A20(s32, s32, s32);
/* fn_80075F4C (0x80075F4C): tail-call wrapper. */
#pragma push
#pragma scheduling off
s32 fn_80075F4C(void) { return fn_80165A20(0x46a, 0, 0x7f); }
#pragma pop

extern u8* fn_8006B420(void);

/* fn_80077AAC..fn_80077B60 (0x80077AAC-0x80077B60): fixed-index byte
 * accessors into the fn_8006B420() record. */
#pragma push
#pragma scheduling off
u8 fn_80077AAC(void) { return fn_8006B420()[0x13]; }
#pragma pop

#pragma push
#pragma scheduling off
u8 fn_80077AD0(void) { return fn_8006B420()[0x12]; }
#pragma pop

#pragma push
#pragma scheduling off
u8 fn_80077AF4(void) { return fn_8006B420()[0x11]; }
#pragma pop

#pragma push
#pragma scheduling off
u8 fn_80077B18(void) { return fn_8006B420()[0x10]; }
#pragma pop

#pragma push
#pragma scheduling off
u8 fn_80077B3C(void) { return fn_8006B420()[0xf]; }
#pragma pop

#pragma push
#pragma scheduling off
u8 fn_80077B60(void) { return fn_8006B420()[0xe]; }
#pragma pop

/* fn_80077B84 (0x80077B84): fixed-index s16 accessor into the same record. */
#pragma push
#pragma scheduling off
s16 fn_80077B84(void) { return ((s16*)fn_8006B420())[0xb]; }
#pragma pop

/* menuCBRule_GetBattleTimeLimit (0x80077BA8): same shape, scaled by 0x3c. */
s32 menuCBRule_GetBattleTimeLimit(void) {
    return ((s16*)fn_8006B420())[0xa] * 0x3c;
}

extern u32 lbl_80478928;
extern u16 lbl_802EE458[];

/* fn_80077D88 (0x80077D88): bounds-checked table lookup. */
u16 fn_80077D88(s32 index) {
    if (index < 0 || lbl_80478928 <= (u32)index) {
        return 0;
    }
    return lbl_802EE458[index];
}

extern u8 lbl_80268940[];

/* menuCBRule_ConstantRule (0x80077E50): fixed-slot table lookup, NULL out of range. */
void* menuCBRule_ConstantRule(s32 index) {
    switch (index) {
    case 0:
    case 1:
    case 2:
        return lbl_80268940 + (index * 0x54);
    }
    return (void*)0;
}

extern void* memcpy(void* dst, const void* src, u32 size);

/* fn_80077E80 (0x80077E80): fixed-size record copy. */
#pragma scheduling off
void fn_80077E80(void* dst, void* src) {
    memcpy(dst, src, 0x54);
}
#pragma scheduling on

extern s32 memcmp(const void* s1, const void* s2, u32 size);

/* fn_80077EA4 (0x80077EA4): fixed-size record equality check. */
#pragma push
#pragma peephole off
u8 fn_80077EA4(u16* s1, u16* s2) {
    return memcmp(s1, s2, 0x54) == 0;
}
#pragma pop

extern void _threadSwitch(void);
extern s32 fn_800D37CC(void);
extern u32 fn_800D3088(void);
extern s32 menuOpen(s32, s32);
extern void winMsgOpenField(s32, s32, s32);
extern void winMsgOpen(s32, s32, s32, s32);
extern void winMsgClose(s32);
extern s32 fn_8001E184(void);

extern u32 lbl_804788F0;
extern u32 lbl_802E61D8[];
extern u8 lbl_8047A630;
extern u8 lbl_8047A631;
extern u8 lbl_8047A632;
extern u8 lbl_8047A633;
extern u8 lbl_8047A634;
extern u8 lbl_8047A635;
extern u32 lbl_8047A638;
extern f32 lbl_8047C108;
extern f32 lbl_8047C114;
extern f64 lbl_8047C118;
extern f64 lbl_8047C120;
extern f32 lbl_8047C128;

#define WAIT_MENU_TIME(limit_)           \
    do {                                 \
        f32 elapsed_ = lbl_8047C114;     \
        while (elapsed_ < (limit_)) {    \
            s32 frames_;                 \
            u32 ticks_;                  \
            _threadSwitch();             \
            frames_ = fn_800D37CC();     \
            ticks_ = fn_800D3088();      \
            elapsed_ += (f32)ticks_ / (f32)frames_; \
        }                                \
    } while (0)

#define SHOW_CANCEL_MESSAGE()    \
    do {                         \
        if (arg0 == 0) {         \
            winMsgOpen(2, 0x44cf, 1, 0); \
            winMsgClose(1);      \
        }                        \
    } while (0)

#define CLOSE_AND_ABORT()        \
    do {                         \
        menuClose(0xef);         \
        WAIT_MENU_TIME(lbl_8047C108); \
        lbl_8047A638 = 1;        \
        return 0;                \
    } while (0)

#define SHOW_BLOCKING_MESSAGE(msg_) \
    do {                            \
        winMsgOpenField((msg_), 1, 0); \
        winMsgClose(1);             \
        SHOW_CANCEL_MESSAGE();      \
        CLOSE_AND_ABORT();          \
    } while (0)

u8 fn_80079EF4(s32 arg0, u32 value) {
    s32 rank;
    s8 choice;

    lbl_8047A630 = 0;
    lbl_8047A631 = 0;
    lbl_8047A632 = 0;

    rank = lbl_804788F0 - 1;
    while (rank >= 0 && lbl_802E61D8[rank] > value) {
        rank--;
    }
    if (rank < 0) {
        rank = 0;
    }

    menuClose(0xe1);
    WAIT_MENU_TIME(lbl_8047C128);
    menuOpen(0xef, 0);
    WAIT_MENU_TIME(lbl_8047C108);

    if (rank < 1) {
        SHOW_BLOCKING_MESSAGE(0x43a7);
    }

    switch (rank) {
    case 1:
        if (lbl_8047A635 != 0) {
            SHOW_BLOCKING_MESSAGE(0x43ae);
        }
        winMsgOpenField(0x43b4, 1, 0);
        lbl_8047A632 = 1;
        break;
    case 2:
        if (lbl_8047A635 != 0 && lbl_8047A634 != 0) {
            SHOW_BLOCKING_MESSAGE(0x43ab);
        }
        if (lbl_8047A635 != 0) {
            winMsgOpenField(0x43b3, 1, 0);
            lbl_8047A631 = 1;
        } else {
            winMsgOpenField(0x43b6, 1, 0);
            lbl_8047A632 = 1;
            lbl_8047A631 = 1;
        }
        break;
    case 3:
        if (lbl_8047A635 != 0 && lbl_8047A634 != 0 && lbl_8047A633 != 0) {
            SHOW_BLOCKING_MESSAGE(0x43a9);
        }
        if (lbl_8047A634 != 0) {
            winMsgOpenField(0x43b1, 1, 0);
            lbl_8047A630 = 1;
        } else if (lbl_8047A635 != 0) {
            winMsgOpenField(0x43b5, 1, 0);
            lbl_8047A630 = 1;
            lbl_8047A631 = 1;
        } else {
            winMsgOpenField(0x43c2, 1, 0);
            lbl_8047A630 = 1;
            lbl_8047A631 = 1;
            lbl_8047A632 = 1;
        }
        break;
    default:
        menuClose(0xef);
        winMsgClose(1);
        WAIT_MENU_TIME(lbl_8047C108);
        lbl_8047A638 = 1;
        return 0;
    }

    winMsgOpenField(0x43d1, 1, 0);
    choice = (s8)fn_8001E184();
    winMsgClose(1);
    if (choice == 0 || choice < -1 || choice >= 2) {
        return 1;
    }

    SHOW_CANCEL_MESSAGE();
    CLOSE_AND_ABORT();
}

#undef SHOW_BLOCKING_MESSAGE
#undef CLOSE_AND_ABORT
#undef SHOW_CANCEL_MESSAGE
#undef WAIT_MENU_TIME
