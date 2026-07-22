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
} WindowSystemWork;

extern WindowSystemWork lbl_80404ACC;

void* windowSearchID(s32 id)
{
    WindowWork* window;

    if (id <= 0) {
        return NULL;
    }

    window = lbl_80404ACC.windows;
    while (window != NULL) {
        if (window->id == id) {
            return window;
        }
        window = window->next;
    }
    return NULL;
}
