#include "game/battle/battle_waza_types.h"

s32 mailGetReceiveNumber(s32 mailId)
{
    WazaPartyScratch* party;
    s32 index;
    BOOL found;
    s32 currentId;

    found = FALSE;
    index = 0;

    while (index < (s32)((WazaPartyScratch*)savedataGetStatus(0, 0x0A))->count &&
           !found) {
        party = (WazaPartyScratch*)savedataGetStatus(0, 0x0A);
        if (index < 0 ||
            index >=
                (s32)((WazaPartyScratch*)savedataGetStatus(0, 0x0A))->count) {
            currentId = -1;
        } else {
            currentId = party->seqIds[index];
        }

        if (mailId == currentId) {
            found = TRUE;
        }

        index++;
    }

    if (!found) {
        return -1;
    }
    return index;
}
