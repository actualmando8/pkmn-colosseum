#include "dolphin/types.h"

s32 fightSeqIsEncoreNgWazaDataId(u16 move)
{
    if (move == 0xa5 || move == 0xe3 || move == 0x77 || move == 0xffff) {
        return 1;
    }
    return 0;
}
