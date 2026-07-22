/**
 * @file musyx_midi_exact_801609C8.c
 * @brief Exact natural-C MusyX MIDI controller island.
 */

#include "dolphin/types.h"

typedef struct CtrlSource {
    u8 midiCtrl;
    u8 combine;
    s32 scale;
} CtrlSource;

typedef struct CtrlDest {
    CtrlSource source[4];
    u16 oldValue;
    u8 numSource;
} CtrlDest;

extern u8 lbl_80449590[8][16][134];
extern u8 lbl_8044D910[64][134];
extern u8 lbl_8044FAD0[8][16];
extern u8 lbl_8044FB50[64];
extern const u8 lbl_80273338[134];
extern const u8 lbl_802733C0[134];

extern void* memcpy(void* dst, const void* src, u32 size);
extern void fn_801603C0(u8 ctrl, u8 channel, u8 set, u8 value);
extern u8 inpTranslateExCtrl(u8 ctrl);
void inpSetMidiLastNote(u8 midi, u8 midiSet, u8 key);

void inpSetMidiCtrl14(u8 ctrl, u8 channel, u8 set, u16 value)
{
    if (channel == 0xFF) {
        return;
    }

    if (ctrl < 64) {
        fn_801603C0(ctrl & 31, channel, set, value >> 7);
        fn_801603C0((ctrl & 31) + 32, channel, set, value & 0x7f);
    } else if (ctrl == 128 || ctrl == 129) {
        fn_801603C0(ctrl & 254, channel, set, value >> 7);
        fn_801603C0((ctrl & 254) + 1, channel, set, value & 0x7f);
    } else if (ctrl == 132 || ctrl == 133) {
        fn_801603C0(ctrl & 254, channel, set, value >> 7);
        fn_801603C0((ctrl & 254) + 1, channel, set, value & 0x7f);
    } else {
        fn_801603C0(ctrl, channel, set, value >> 7);
    }
}

void inpResetMidiCtrl(u8 ch, u8 set, u32 coldReset)
{
    const u8* values;
    u8* dest;
    u32 i;

    values = coldReset ? lbl_80273338 : lbl_802733C0;
    dest = set != 0xFF ? lbl_80449590[set][ch] : lbl_8044D910[ch];

    if (coldReset) {
        memcpy(dest, values, 134);
    } else {
        for (i = 0; i < 134; i++) {
            if (values[i] != 0xFF) {
                dest[i] = values[i];
            }
        }
    }

    inpSetMidiLastNote(ch, set, 0xFF);
}

u16 inpGetMidiCtrl(u8 ctrl, u8 channel, u8 set)
{
    if (channel != 0xff) {
        if (set != 0xff) {
            if (ctrl < 0x40) {
                return lbl_80449590[set][channel][ctrl & 0x1f] << 7 |
                       lbl_80449590[set][channel][(ctrl & 0x1f) + 0x20];
            }
            if (ctrl < 0x46) {
                return lbl_80449590[set][channel][ctrl] < 0x40 ? 0 : 0x3fff;
            }
            if (ctrl >= 0x60 && ctrl < 0x66) {
                return 0;
            }
            if (ctrl == 0x80 || ctrl == 0x81) {
                return lbl_80449590[set][channel][ctrl & 0xfe] << 7 |
                       lbl_80449590[set][channel][(ctrl & 0xfe) + 1];
            }
            if (ctrl == 0x84 || ctrl == 0x85) {
                return lbl_80449590[set][channel][ctrl & 0xfe] << 7 |
                       lbl_80449590[set][channel][(ctrl & 0xfe) + 1];
            }
            return lbl_80449590[set][channel][ctrl] << 7;
        }
        if (ctrl < 0x40) {
            return lbl_8044D910[channel][ctrl & 0x1f] << 7 |
                   lbl_8044D910[channel][(ctrl & 0x1f) + 0x20];
        }
        if (ctrl < 0x46) {
            return lbl_8044D910[channel][ctrl] < 0x40 ? 0 : 0x3fff;
        }
        if (ctrl >= 0x60 && ctrl < 0x66) {
            return 0;
        }
        if (ctrl == 0x80 || ctrl == 0x81) {
            return lbl_8044D910[channel][ctrl & 0xfe] << 7 |
                   lbl_8044D910[channel][(ctrl & 0xfe) + 1];
        }
        if (ctrl == 0x84 || ctrl == 0x85) {
            return lbl_8044D910[channel][ctrl & 0xfe] << 7 |
                   lbl_8044D910[channel][(ctrl & 0xfe) + 1];
        }
        return lbl_8044D910[channel][ctrl] << 7;
    }
    return 0;
}

u8* fn_80160EA0(u8 midi, u8 midiSet)
{
    extern u8 lbl_8044D890[8][16];
    extern u8 lbl_8044FA90[64];

    if (midiSet == 0xFF) {
        return &lbl_8044FA90[midi];
    }
    return &lbl_8044D890[midiSet][midi];
}

void fn_80160ED4(u8 midi, u8 midiSet)
{
    extern u8 lbl_8044D890[8][16];
    extern u8 lbl_8044FA90[64];
    u8* channelDefaults;

    channelDefaults =
        midiSet != 0xFF ? &lbl_8044D890[midiSet][midi] : &lbl_8044FA90[midi];
    *channelDefaults = 2;
}

void inpAddCtrl(CtrlDest* dest, u8 ctrl, s32 scale, u8 comb, u32 isVar)
{
    u8 n;

    if (comb == 0) {
        dest->numSource = 0;
    }
    if (dest->numSource < 4) {
        n = dest->numSource++;
        if (isVar == 0) {
            ctrl = inpTranslateExCtrl(ctrl);
        } else {
            comb |= 0x10;
        }
        dest->source[n].midiCtrl = ctrl;
        dest->source[n].combine = comb;
        dest->source[n].scale = scale;
    }
}

void inpFXCopyCtrl(u8 ctrl, u8* dvoice, u8* svoice)
{
    u8 di;
    u8 si;

    di = *(u32*)(dvoice + 0xF4);
    si = *(u32*)(svoice + 0xF4);

    if (ctrl < 64) {
        lbl_8044D910[di][ctrl & 31] = lbl_8044D910[si][ctrl & 31];
        lbl_8044D910[di][(ctrl & 31) + 32] =
            lbl_8044D910[si][(ctrl & 31) + 32];
    } else if (ctrl == 128 || ctrl == 129) {
        lbl_8044D910[di][ctrl & 254] = lbl_8044D910[si][ctrl & 254];
        lbl_8044D910[di][(ctrl & 254) + 1] =
            lbl_8044D910[si][(ctrl & 254) + 1];
    } else if (ctrl == 132 || ctrl == 133) {
        lbl_8044D910[di][ctrl & 254] = lbl_8044D910[si][ctrl & 254];
        lbl_8044D910[di][(ctrl & 254) + 1] =
            lbl_8044D910[si][(ctrl & 254) + 1];
    } else {
        lbl_8044D910[di][ctrl] = lbl_8044D910[si][ctrl];
    }
}

void inpSetMidiLastNote(u8 midi, u8 midiSet, u8 key)
{
    if (midiSet != 0xFF) {
        lbl_8044FAD0[midiSet][midi] = key;
    } else {
        lbl_8044FB50[midi] = key;
    }
}

u8 inpGetMidiLastNote(u8 midi, u8 midiSet)
{
    if (midiSet != 0xFF) {
        return lbl_8044FAD0[midiSet][midi];
    }
    return lbl_8044FB50[midi];
}
