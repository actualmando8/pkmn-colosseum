#include "dolphin/types.h"

typedef struct WindowSystemWork {
    u8 pad_00[0x10];
    u8 keyInfo;
} WindowSystemWork;

extern WindowSystemWork lbl_80404ACC;

void* windowGetKeyInfo(void)
{
    return &lbl_80404ACC.keyInfo;
}
