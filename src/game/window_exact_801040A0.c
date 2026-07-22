#include "dolphin/types.h"

typedef struct WindowWork {
    u8 padding[0x60];
    u32 params[15];
    u8 freeWork[1];
} WindowWork;

void* windowGetFreeWork(WindowWork* window)
{
    if (window != 0) {
        return window->freeWork;
    }
    return 0;
}

void windowSetParam(WindowWork* window, u32 index, u32 value)
{
    if (window == 0) {
        return;
    }
    window->params[index] = value;
}

u32 windowGetParam(WindowWork* window, u32 index)
{
    if (window != 0) {
        return window->params[index];
    }
    return 0;
}
