#include "dolphin/types.h"

typedef struct WindowWork WindowWork;

struct WindowWork {
    u8 pad_00[4];
    s32 id;
    u8 pad_08[8];
    WindowWork* next;
};

typedef struct WindowSystemWork {
    u32 field_00;
    u32 activeID;
    u8 pad_08[4];
    WindowWork* windows;
    u8 keyInfo;
} WindowSystemWork;

extern WindowSystemWork lbl_80404ACC;

u32 windowGetActiveID(void)
{
    return lbl_80404ACC.activeID;
}
