#include "dolphin/types.h"

typedef struct OSModuleInfo {
    u8 unused_00[0x1C];
    u32 version;
} OSModuleInfo;

extern BOOL Link(OSModuleInfo* module, void* bss, BOOL fixed);

BOOL fn_8009ED4C(OSModuleInfo* module, void* bss)
{
    return Link(module, bss, FALSE);
}
