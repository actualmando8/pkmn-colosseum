#include "src/dolphin/card_dsp_private.h"

extern s32 fn_800B57D0(s32 chan, s32 fileNo, CARDDirEntry* entry);
extern s32 fn_800B588C(s32 chan, s32 fileNo, CARDDirEntry* entry,
                       CARDCallback callback);

s32 CARDGetAttributes(s32 chan, s32 fileNo, u8* attr)
{
    s32 result;
    CARDDirEntry entry;

    result = fn_800B57D0(chan, fileNo, &entry);
    if (result == 0) {
        *attr = entry.permission;
    }
    return result;
}

s32 fn_800B5BE4(s32 chan, s32 fileNo, u8 attr, CARDCallback callback)
{
    s32 result;
    CARDDirEntry entry;

    result = fn_800B57D0(chan, fileNo, &entry);
    if (result < 0) {
        return result;
    }
    entry.permission = attr;
    return fn_800B588C(chan, fileNo, &entry, callback);
}
