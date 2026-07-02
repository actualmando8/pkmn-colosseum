#include "dolphin/types.h"

extern u32 SIGetType(s32 channel);
extern u32 SIDecodeType(u32 type);

u32 fn_800D0F44(s32 channel) {
    return SIDecodeType(SIGetType(channel));
}
