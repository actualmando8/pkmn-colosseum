#include "dolphin/types.h"

extern u32 SIGetType(s32 channel);
extern u32 fn_800D0DF8(u32 type);

u32 fn_800D0F44(s32 channel) {
    return fn_800D0DF8(SIGetType(channel));
}
