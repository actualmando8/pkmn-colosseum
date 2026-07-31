/**
 * @file snd_midictrl_exact_80161D20.c
 * @brief MusyX extended MIDI controller helpers.
 */

#include "dolphin/types.h"

extern u16 inpGetMidiCtrl(u8 ctrl, u8 channel, u8 set);
extern void fn_801603C0(u8 ctrl, u8 channel, u8 set, u8 value);

u8 inpTranslateExCtrl(u8 ctrl)
{
    switch (ctrl) {
    case 0x80:
        ctrl = 0x80;
        break;
    case 0x81:
        ctrl = 0x82;
        break;
    case 0x82:
        ctrl = 0xA0;
        break;
    case 0x83:
        ctrl = 0xA1;
        break;
    case 0x84:
        ctrl = 0x83;
        break;
    case 0x85:
        ctrl = 0x84;
        break;
    case 0x86:
        ctrl = 0xA2;
        break;
    case 0x87:
        ctrl = 0xA3;
        break;
    case 0x88:
        ctrl = 0xA4;
        break;
    }
    return ctrl;
}

static inline void inpSetMidiCtrl14(u8 ctrl, u8 channel, u8 set, u16 value)
{
    if (channel == 0xFF) {
        return;
    }

    if (ctrl < 64) {
        fn_801603C0(ctrl & 31, channel, set, value >> 7);
        fn_801603C0((ctrl & 31) + 32, channel, set, value & 0x7F);
    } else if (ctrl == 128 || ctrl == 129) {
        fn_801603C0(ctrl & 254, channel, set, value >> 7);
        fn_801603C0((ctrl & 254) + 1, channel, set, value & 0x7F);
    } else if (ctrl == 132 || ctrl == 133) {
        fn_801603C0(ctrl & 254, channel, set, value >> 7);
        fn_801603C0((ctrl & 254) + 1, channel, set, value & 0x7F);
    } else {
        fn_801603C0(ctrl, channel, set, value >> 7);
    }
}

u16 inpGetExCtrl(u8* obj, u8 ctrl)
{
    u16 value;

    switch (inpTranslateExCtrl(ctrl)) {
    case 0xA0:
        value = (*(s16*)(obj + 0x1C4) << 1) + 0x2000;
        break;
    case 0xA1:
        value = (*(s16*)(obj + 0x1D0) << 1) + 0x2000;
        break;
    default:
        value =
            obj[0x121] != 0xFF
                ? inpGetMidiCtrl(ctrl, obj[0x121], obj[0x122])
                : 0;
        break;
    }
    return value;
}

void inpSetExCtrl(u8* obj, u8 ctrl, s16 value)
{
    value = value < 0 ? 0 : value > 0x3FFF ? 0x3FFF : value;

    switch (inpTranslateExCtrl(ctrl)) {
    case 0xA0:
    case 0xA1:
        break;
    default:
        if (obj[0x121] != 0xFF) {
            inpSetMidiCtrl14(ctrl, obj[0x121], obj[0x122], value);
        }
        break;
    }
}
