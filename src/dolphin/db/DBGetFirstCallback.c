#include "dolphin/types.h"

extern void* __DBInterface;

u32 fn_800A2C58(void) {
    if (__DBInterface == NULL) {
        return 0;
    }

    return *(u32*)__DBInterface;
}
