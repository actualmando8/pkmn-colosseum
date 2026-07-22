#include "dolphin/types.h"

typedef struct MemoData {
    u16 count;
} MemoData;

extern void* savedataGetStatus(s32 side, s32 slotType);

u16 memoDataGetCount(MemoData* data)
{
    if (data == NULL) {
        data = (MemoData*)savedataGetStatus(0, 0xC);
    }
    return data->count;
}
