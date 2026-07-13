/**
 * @file menu_range_8007109C.c
 * @brief menu (GBA-link/comm UI), 0x8007109C - 0x8007C260.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) -- mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

/* GBA link timing: OS_TIMER_CLOCK / OSMillisecondsToTicks, see include/dolphin/si/SI.h */
#define OS_BUS_CLOCK   (*(u32*)0x800000F8)
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)
#define OSMillisecondsToTicks(msec) ((msec) * (OS_TIMER_CLOCK / 1000))

extern u8 lbl_803F7A30[];
extern u32 OSGetTick(void);
extern void gbaCommandSetKeyState(s32 mode, s32 flag);
extern s32 fn_80073C38(s32 chan);
extern s32 GBAWrite();
extern s32 GBARead();
extern s32 GBAGetStatus();

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
 * menuCB_Common.c block (0x8007109C-0x80071698): menu call-stack at
 * lbl_803B6D88 ({id, flag} pairs + depth word at +0x40) plus two heap
 * handle helpers. Assert strings: lbl_80268708 = "menuCB_Common.c",
 * lbl_80268718 = _menuPop underflow, lbl_80268750 = _menuPush overflow,
 * lbl_8047C090 = "handle".
 */
extern void __assert(const char* file, u32 line, const char* msg);
extern const u8 lbl_80268708[];
extern const u8 lbl_80268718[];
extern const u8 lbl_80268750[];
extern char lbl_8047C090;

/* fn_8007109C (0x8007109C): release the current heap handle, if any. */
#pragma push
#pragma peephole off
void fn_8007109C(void) {
    extern u16 fn_800E202C(void);
    extern void fn_800E24B0(u16 handle);
    extern void fn_800E209C(u16 handle);
    u16 handle;

    handle = fn_800E202C();
    if (!(handle != 0)) {
        __assert((const char*)lbl_80268708, 0xDE, (const char*)&lbl_8047C090);
    }
    if (handle != 0) {
        fn_800E24B0(handle);
        fn_800E209C(handle);
    }
}
#pragma pop

/* fn_80071104 (0x80071104): allocate a 32-byte-aligned handle and lock it. */
#pragma push
#pragma peephole off
s32 fn_80071104(s32 size) {
    extern u16 fn_800E2C04(s32 size, s32 align);
    extern s32 fn_800E27B0(u16 handle);
    u16 handle;

    handle = fn_800E2C04((size + 0x1F) & ~0x1F, 0x20);
    if (handle != 0) {
        return fn_800E27B0(handle);
    }
    if (!(handle != 0)) {
        __assert((const char*)lbl_80268708, 0xD5, (const char*)&lbl_8047C090);
    }
    return 0;
}
#pragma pop

/* fn_80071160 (0x80071160): scan the 4 save slots for a live, valid entry. */
#pragma push
#pragma peephole off
s32 fn_80071160(void) {
    extern u8* savedataGetStatus(s32 side, s32 slotType);
    extern s32 fn_8006A7E8(u8* p);
    extern u8 fn_8008ABA0(s32 v);
    s32 off;
    u32 i;
    s32 v;

    for (i = 0, off = 0; i < 4; off += 0x1660, i++) {
        v = *(s32*)(savedataGetStatus(0, 0xE) + off + 0x59CC);
        if (v != 0) {
            if (fn_8006A7E8(savedataGetStatus(0, 0xE) + off + 0x59A8) != 0) {
                if (fn_8008ABA0(v) == 0) {
                    return v;
                }
            }
        }
    }
    return 0;
}
#pragma pop

/* fn_80071344 (0x80071344): reopen the menu on top of the call stack. */
#pragma push
#pragma peephole off
void fn_80071344(void) {
    extern s32 menuOpenCustom(s32 slot, ...);
    u32 depth;

    depth = *(u32*)(lbl_803B6D88 + 0x40);
    menuOpenCustom(*(s32*)(lbl_803B6D88 + depth * 8), 0,
                   lbl_803B6D88 + depth * 8 + 4, 0x10, 1, 0, 0);
}
#pragma pop

/* _menuPop (0x800714C8): close the top menu and pop the call stack. */
#pragma push
#pragma peephole off
s32 _menuPop(void) {
    extern s32 windowGetActiveID(void);
    extern u32 windowSearchID(s32 id);
    extern void menuCloseCustom(s32 slot, s32 p1, s32 p2);
    s32 top;
    s32 active;
    u32 d;

    top = *(s32*)(lbl_803B6D88 + *(u32*)(lbl_803B6D88 + 0x40) * 8);
    active = windowGetActiveID();
    if (active == top) {
        menuCloseCustom(*(s32*)(lbl_803B6D88 + *(u32*)(lbl_803B6D88 + 0x40) * 8), 0, 0);
    }
    if (windowSearchID(0xBE) != 0) {
        menuCloseCustom(0xBE, 0, 1);
    }
    *(s32*)(lbl_803B6D88 + *(u32*)(lbl_803B6D88 + 0x40) * 8 + 4) = 0;
    if (*(s32*)(lbl_803B6D88 + 0x40) == 0) {
        return -1;
    }
    if (!(0 < *(s32*)(lbl_803B6D88 + 0x40))) {
        __assert((const char*)lbl_80268708, 0x5C, (const char*)lbl_80268718);
    }
    d = *(u32*)(lbl_803B6D88 + 0x40) - 1;
    *(u32*)(lbl_803B6D88 + 0x40) = d;
    return *(s32*)(lbl_803B6D88 + d * 8);
}
#pragma pop

/* _menuPush (0x800715BC): push a menu id onto the call stack. */
#pragma push
#pragma peephole off
void _menuPush(s32 id) {
    u32 depth;

    depth = *(u32*)(lbl_803B6D88 + 0x40);
    if (depth >= 8) {
        __assert((const char*)lbl_80268708, 0x41, (const char*)lbl_80268750);
    } else {
        *(u32*)(lbl_803B6D88 + 0x40) = depth + 1;
        *(s32*)(lbl_803B6D88 + (depth + 1) * 8) = id;
        *(s32*)(lbl_803B6D88 + *(u32*)(lbl_803B6D88 + 0x40) * 8 + 4) = 0;
    }
}
#pragma pop

/* menuCB_InitMenu (0x80071644): clear the call stack and seed slot 0. */
#pragma push
#pragma peephole off
void menuCB_InitMenu(s32 id) {
    u32 i;

    for (i = 0; i < 8; i++) {
        *(u32*)(lbl_803B6D88 + i * 8) = 0;
        *(u32*)(lbl_803B6D88 + i * 8 + 4) = 0;
    }
    *(s32*)(lbl_803B6D88 + 0x0) = id;
    *(u32*)(lbl_803B6D88 + 0x40) = 0;
}
#pragma pop

#pragma push
#pragma scheduling off
s32 fn_80071E34(s32 chan, void* data) {
    extern s32 fn_80071EA4(s32 chan, void* data);
    s32 keyChannel;
    s32 result;

    keyChannel = chan + 1;
    gbaCommandSetKeyState(keyChannel, 2);
    result = fn_80071EA4(chan, data);
    gbaCommandSetKeyState(keyChannel, 1);
    return result;
}
#pragma pop

/* fn_80072C74 (0x80072C74): poll one GBA channel and read its response. */
#pragma push
#pragma peephole off
s32 fn_80072C74(s32 chan, u32* response) {
    extern u32 fn_800D0F44(s32 chan);
    u8 readBuf[4];
    u8 writeBuf[4];
    u8 status[4];
    s32 result;

    if (fn_800D0F44(chan) != 0x40000) {
        result = 1;
    } else if (GBAGetStatus(chan, (u32)status) != 0) {
        result = 2;
    } else if ((status[0] & 8) == 0) {
        *(u32*)writeBuf = 0x11;
        GBAWrite(chan, (u32)writeBuf, (u32)status);
        result = -1;
    } else if (GBARead(chan, (u32)readBuf, (u32)status) != 0) {
        result = 3;
    } else {
        result = 0;
        *response = *(u32*)readBuf;
    }
    if (result >= 0) {
        gbaCommandSetKeyState(chan + 1, 1);
    }
    return result;
}
#pragma pop

/* _AGB_EntryGetStatus (0x80073034): poll one GBA channel and read its status. */
#pragma push
#pragma peephole off
s32 _AGB_EntryGetStatus__FlPUl(s32 chan, u32* response) {
    extern u32 fn_800D0F44(s32 chan);
    extern s32 GBAGetStatus(s32 chan, void* status);
    extern s32 GBAWrite(s32 chan, void* source, void* status);
    extern s32 GBARead(s32 chan, void* destination, void* status);
    u8 readBuf[4];
    u8 writeBuf[4];
    u8 status[4];

    if (fn_800D0F44(chan) != 0x40000) {
        return 1;
    }
    if (GBAGetStatus(chan, status) != 0) {
        return 2;
    }
    if ((status[0] & 8) == 0) {
        *(u32*)writeBuf = 0x11;
        GBAWrite(chan, writeBuf, status);
        return -1;
    }
    if (GBARead(chan, readBuf, status) != 0) {
        return 3;
    }
    *response = *(u32*)readBuf;
    return 0;
}
#pragma pop

/* fn_80073690 (0x80073690): bracket a channel operation with key-state updates. */
#pragma push
#pragma peephole off
s32 fn_80073690(s32 chan, s32 data) {
    extern s32 fn_80073700(s32 chan, s32 data);
    s32 keyChannel;
    s32 result;

    keyChannel = chan + 1;
    gbaCommandSetKeyState(keyChannel, 2);
    result = fn_80073700(chan, data);
    gbaCommandSetKeyState(keyChannel, 1);
    return result;
}
#pragma pop

/* fn_80073990 (0x80073990): wait one millisecond, then send command 0x11. */
s32 fn_80073990(s32 chan) {
    extern u32 fn_800D0F44(s32 chan);
    u32 command;
    u32 length;
    u32 delay;
    u32 start;
    s32 result;

    delay = OSMillisecondsToTicks(1);
    start = OSGetTick();
    while (OSGetTick() - start < delay) {
    }
    if (fn_800D0F44(chan) != 0x40000) {
        result = 1;
    } else {
        command = 0x11;
        if (GBAWrite(chan, (u32)&command, (u32)&length) != 0) {
            result = 2;
        } else {
            result = 0;
        }
    }
    return result;
}

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

/* fn_80074324 (0x80074324): prep key-state then perform per-channel reset path. */
#pragma push
#pragma peephole off
s32 fn_80074324(s32 arg0) {
    extern s32 fn_80074360(s32);

    gbaCommandSetKeyState(arg0 + 1, 0);
    return fn_80074360(arg0);
}
#pragma pop

/* fn_80075A34 (0x80075A34): load the battle scene and start its camera. */
void fn_80075A34(void) {
    extern u32 fn_80113F48(void);
    extern u32 fn_801CBA0C(u32 resourceId);
    extern void GSresGetResource(u32 archive, u32 resource);
    extern void cameraPlayAnime(s32 cameraId, u32 animationId, s32 frame, s32 loop);
    extern void GSscene_SetMode(u32 mode);
    extern u8 lbl_8047A5D0;
    u32 archive;

    archive = fn_80113F48();
    GSresGetResource(archive, *(u32*)&lbl_8047A5D0 = fn_801CBA0C(0x10801000));
    cameraPlayAnime(0x5E0, 0x10821800, 0, 1);
    GSscene_SetMode(4);
}

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

#pragma push
#pragma peephole off
s32 fn_80075390(void) {
    extern u8 fn_80075638(void);
    extern void fn_8007565C(void);
    extern void fn_800756C8(s32);

    if (fn_80075638() == 0) {
        fn_8007565C();
    } else {
        fn_800756C8(3);
    }
    return 0;
}
#pragma pop

/* fn_8007565C (0x8007565C): close and release the active menu resource. */
#pragma push
#pragma peephole off
void fn_8007565C(void) {
    extern u8 lbl_8047A610;
    extern void fn_8010A420(u32);
    extern void menuCloseCustom(s32, s32, s32);
    extern u16 fn_800E202C(u32);
    extern void fn_800E24B0(void);
    extern void fn_800E209C(u16);
    u16 handle;

    fn_8010A420(*(u32*)&lbl_8047A610 + 0x144);
    menuCloseCustom(0xD8, 0, 1);
    handle = fn_800E202C(*(u32*)&lbl_8047A610);
    if (handle != 0) {
        fn_800E24B0();
        fn_800E209C(handle);
    }
    *(u32*)&lbl_8047A610 = 0;
}
#pragma pop

/* fn_80075BC4 (0x80075BC4): helper counter clamp from 0 to 0x30. */
#pragma push
#pragma peephole off
s32 fn_80075BC4(void) {
    u32 value;

    value = fn_801906A0(0xab2);
    if (value > 0x30) {
        return 0;
    } else {
        return 0x30 - value;
    }
}
#pragma pop

/* fn_80075B74 (0x80075B74): increment and clamp the helper counter. */
#pragma push
#pragma peephole off
s32 fn_80075B74(void) {
    s32 result;
    u32 value;

    value = fn_801906A0(0xab2) + 1;
    result = 1;
    if (value > 0x30) {
        value = 0x30;
        result = 0;
    }
    _flagSet(0xab2, value);
    return result;
}
#pragma pop

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

/* fn_80075EE0 (0x80075EE0): start the menu worker and fade in. */
void fn_80075EE0(void) {
    extern s32 lbl_8047A618;
    extern f32 lbl_8047C0C8;
    extern u32 fn_800FF560(void);
    extern void* GSthreadCreate(s32, u32, u32, s32, s32, void*);
    extern void fadeSet(s32, f32);
    extern s32 fn_80075F4C(void);

    if (lbl_8047A618 == 0) {
        GSthreadCreate(1, fn_800FF560(), 0x4000, 1, 1, fn_80075F4C);
    } else {
        fn_80165A20(0x46a, 0, 0x7f);
    }
    fadeSet(2, lbl_8047C0C8);
}

/* fn_80075F4C (0x80075F4C): tail-call wrapper. */
#pragma push
#pragma scheduling off
s32 fn_80075F4C(void) { return fn_80165A20(0x46a, 0, 0x7f); }
#pragma pop

/* fn_80075F78 (0x80075F78): set the message value for the selected rule. */
void fn_80075F78(void* rule) {
    extern u32 GSmsgGetGSchar(s32 messageId);
    extern void msgctrlSetValue(s32 id, u32 value);
    s32 messageId;

    switch (*(s8*)((u8*)rule + 0x95)) {
    case 0:
        messageId = 0x43bc;
        break;
    case 1:
        messageId = 0x43ba;
        break;
    case 2:
        messageId = 0x43be;
        break;
    default:
        messageId = 1;
        break;
    }
    msgctrlSetValue(0x37, GSmsgGetGSchar(messageId));
}

/* menuCBRule_CheckPokemonEventFlag (0x80075FEC): require the event flag for
 * the two special Pokemon data IDs. */
#pragma push
#pragma peephole off
u8 menuCBRule_CheckPokemonEventFlag(u8* pokemon) {
    extern u16 pokemonBiosGetPokemonDataId(u8* pokemon);
    extern u8 pokemonBiosGetEventGetFlag(u8* pokemon);

    switch (pokemonBiosGetPokemonDataId(pokemon)) {
    case 0x97:
    case 0x19A:
        if (pokemonBiosGetEventGetFlag(pokemon) == 0) {
            return 0;
        }
        break;
    }
    return 1;
}
#pragma pop

/* menuCBRule_CheckPokemonErrorAll (0x80076334): require every party member to
 * pass the per-slot error check. */
#pragma push
#pragma scheduling off
#pragma peephole off
u8 menuCBRule_CheckPokemonErrorAll(void* pokemon) {
    extern u8 fn_80076398(void* pokemon, s32 index);
    s32 i;

    for (i = 0; i < 6; i++) {
        if (fn_80076398(pokemon, i) == 0) {
            return 0;
        }
    }
#pragma scheduling on
    return 1;
}
#pragma pop

/* fn_80077A5C (0x80077A5C): accept an empty slot or a zero species value. */
#pragma push
#pragma peephole off
u8 fn_80077A5C(void* pokemon) {
    extern s32 pokemonGetStatus(void* pokemon, s32 index, s32 field, s32 subindex);
    s32 result;

    result = 0;
    if (pokemon == 0 || pokemonGetStatus(pokemon, 0, 0x6E, 0) == 0) {
        result = 1;
    }
    return result;
}
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

/* fn_80077BD0 (0x80077BD0): accept initialized save-status values. */
#pragma push
#pragma scheduling off
u8 fn_80077BD0(void) {
    extern s32* savedataGetStatus(s32 side, s32 slotType);
    s32 value;

    value = savedataGetStatus(0, 0xE)[2];
    switch (value) {
    case 0:
    case 1:
    case 2:
        return 1;
    }
    return 0;
}
#pragma pop

/* menuCBRule_CheckValidItem (0x80077C1C): handle sentinel item ids locally. */
u8 menuCBRule_CheckValidItem(u16 item) {
    extern u8 fn_80142984(u16 item);

    switch (item) {
    case 0:
        return 1;
    case 0xAF:
        return 0;
    default:
        return fn_80142984(item);
    }
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

/* fn_80077DB8 (0x80077DB8): map the current save state to a rule value. */
#pragma push
#pragma scheduling off
#pragma peephole off
s32 fn_80077DB8(void) {
    extern s32* savedataGetStatus(s32 side, s32 slotType);
    s32* entry;
    s32 state;

    entry = savedataGetStatus(0, 0xE);
    if (entry[0] == 2) {
        entry = savedataGetStatus(0, 0xE);
        if (entry[2] == 0) {
            return 6;
        }
    }

    entry = savedataGetStatus(0, 0xE);
    state = entry[1];
    switch (state) {
    case 0:
        return 3;
    case 1:
        return 4;
    case 2:
        break;
    default:
        break;
    }
    return 2;
}
#pragma pop

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

/* fn_80078D38 (0x80078D38): reset the menu fade timer probe. */
#pragma push
#pragma scheduling off
s32 fn_80078D38(void) {
    return fadeCheck(1);
}
#pragma pop

/* fn_8007926C (0x8007926C): initialize the menu scene object. */
#pragma push
#pragma peephole off
void fn_8007926C(void) {
    extern u32 lbl_8047A620;
    extern u32 fn_801CBA0C(u32 resourceId);
    extern void fn_801CB954(u32 object, s32 visible);
    extern void fn_801CB61C(u32 object, u32 resourceId, s32 animationId);
    extern void fn_801CB834(u32 object, s32 arg1, s32 arg2, s32 arg3);
    u32 object;

    lbl_8047A620 = 1;
    object = fn_801CBA0C(0x10BD1000);
    fn_801CB954(object, 1);
    fn_801CB61C(object, 0x104F1000, 0x207);
    fn_801CB834(object, 0, 0, 1);
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

void fn_80071318(u8* dst, u8* src) {
    s16 upperX;
    s16 upperY;
    u32 entryWord;
    s16 lowerX;
    s16 lowerY;

    upperX = *(volatile s16*)(src + 0xC);
    upperY = *(s16*)(src + 0xE);
    *(s16*)(dst + 0x54) = upperX;
    entryWord = *(u32*)(src + 0x10);
    *(s16*)(dst + 0x56) = upperY;
    lowerX = *(s16*)(src + 0x8);
    *(u32*)(dst + 0x58) = entryWord;
    lowerY = *(s16*)(src + 0xA);
    *(s16*)(dst + 0x5C) = lowerX;
    *(s16*)(dst + 0x5E) = lowerY;
}

/* fn_8007A5E8 (0x8007A5E8): draw the current coupon total. */
void fn_8007A5E8(s32 unused, u8* window) {
    extern u32 lbl_8047A62C;
    extern u32 heroGetStatus(void*, s32, s32);
    extern void msgctrlSetValue(s32, u32);
    extern u32 GSmsgGetRect(s32);
    extern void fn_800FB680(s32, s32, s32, s32);
    u32 value;
    u32 rect;

    if ((s32)lbl_8047A638 == 4) {
        value = lbl_8047A62C;
    } else {
        value = heroGetStatus(0, 0xD, 0);
    }
    msgctrlSetValue(0x50, value);
    rect = GSmsgGetRect(0x153);
    fn_800FB680(*(s16*)(window + 0x54) - (rect >> 16), 0, -1, 0x153);
}

/* fn_8007A664 (0x8007A664): show the sprite selected by the current mode. */
void fn_8007A664(s32 unused, u8* sprite) {
    extern void winSpriteSetDisp(void* sprite, s32 visible);

    switch (lbl_8047A638) {
    case 4:
        if (*(s16*)(sprite + 6) == 0x10BF) {
            winSpriteSetDisp(sprite, 1);
        } else {
            winSpriteSetDisp(sprite, 0);
        }
        break;
    case 3:
        if (*(s16*)(sprite + 6) == 0x10C0) {
            winSpriteSetDisp(sprite, 1);
        } else {
            winSpriteSetDisp(sprite, 0);
        }
        break;
    }
}

#pragma push
#pragma scheduling off
s32 fn_8007A82C(void) {
    return fadeCheck(1);
}
#pragma pop

void fn_8007AAFC(void) {
    lbl_803F7A30[0x342] = 1;
}

/* fn_8007AA6C (0x8007AA6C): reset event flag + start menu animation. */
void fn_8007AA6C(void) {
    extern u32 fn_80113F48(void);
    extern void cameraPlayAnime(u32, u32, u32, u32);

    lbl_8047A638 = 1;
    cameraPlayAnime(fn_80113F48(), 0x10941800, 0, 0);
}

/* fn_8007AAA8 (0x8007AAA8): wait for the worker and release its state. */
#pragma push
#pragma peephole off
s32 fn_8007AAA8(void) {
    extern s32 fn_800A1E54(void* thread, void** result);
    extern void fn_8007B0D8(void);
    void* result;

    lbl_803F7A30[0x342] = 1;
    while (*(volatile u8*)(lbl_803F7A30 + 0x345) == 0) {
    }
    fn_800A1E54(lbl_803F7A30 + 0x28, &result);
    fn_8007B0D8();
    return 0;
}
#pragma pop

/* fn_8007B090 (0x8007B090): snapshot callback state, then start the request. */
void fn_8007B090(s32 request) {
    extern void* fn_800A7BCC(void);
    extern u32 lbl_8047A640;
    extern void fn_8007B114(s32 request);
    void* state;

    state = fn_800A7BCC();
    memcpy(&lbl_8047A640, state, 4);
    fn_8007B114(request);
}

/* fn_8007B0D8 (0x8007B0D8): free outstanding pending callback state. */
void fn_8007B0D8(void) {
    extern void* lbl_80478980;
    extern void* lbl_8047A648;
    extern void* lbl_8047A64C;
    extern void fn_8009AAD4(void*, void*);
    void* r4;

    r4 = lbl_8047A648;
    if (r4 != NULL) {
        fn_8009AAD4((void*)lbl_80478980, r4);
        lbl_8047A648 = NULL;
        lbl_8047A64C = NULL;
    }
}

void fn_8007B6A4(u8* r3) {
    extern void fn_8007B6D8(u8* p);
    u8* r31 = r3;

    fn_8007B6D8(r31);
    *(u8*)(r31 + 0x345) = 0x1;
}

#pragma push
#pragma scheduling off
void fn_8007C23C(u8* r3) {
    OSResumeThread((u32)(r3 + 0x28));
}
#pragma pop
