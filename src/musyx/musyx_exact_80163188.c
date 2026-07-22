#include "dolphin/types.h"

extern void fn_80163490(void);
typedef struct MusyXAllocatorCallbacks {
    u32 (*allocate)(u32 size);
    void (*release)(u32 allocation);
} MusyXAllocatorCallbacks;

extern MusyXAllocatorCallbacks lbl_8047B054;

void fn_80163188(void)
{
    fn_80163490();
}

void fn_801631A8(void)
{
}

void fn_801631AC(MusyXAllocatorCallbacks* callbacks)
{
    lbl_8047B054 = *callbacks;
}
