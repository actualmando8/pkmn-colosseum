#include "dolphin/types.h"

typedef void (*FuncPtr)(void);

extern FuncPtr __stdio_exit;
extern void __close_all(void);

void __stdio_atexit(void) {
    __stdio_exit = (FuncPtr)__close_all;
}
