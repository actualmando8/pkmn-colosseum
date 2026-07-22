#include "dolphin/types.h"

s32 fightSeqCondChgActParaIdToValue(u8 para_id)
{
    switch (para_id) {
    case 0x10:
        return 1;
    case 0x20:
        return 2;
    case 0x90:
        return -1;
    case 0xA0:
        return -2;
    default:
        return 0;
    }
}

s32 fightSeqCondChgActTypeToPokemonStatusId(u8 type)
{
    switch (type) {
    case 1:
        return 0xE6;
    case 2:
        return 0xE7;
    case 3:
        return 0xEA;
    case 4:
        return 0xE8;
    case 5:
        return 0xE9;
    case 6:
        return 0xEB;
    case 7:
        return 0xEC;
    default:
        return 0;
    }
}
