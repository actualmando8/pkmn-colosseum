#include "dolphin/types.h"

extern u8 lbl_80404ACC[];

typedef struct MenuSystemState {
    u8 pad_00[0x94];
    s32 lastError;
} MenuSystemState;

s32 menuGetLastError(void)
{
    return ((MenuSystemState*)lbl_80404ACC)->lastError;
}
