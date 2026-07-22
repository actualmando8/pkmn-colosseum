#include "dolphin/os/OSInterrupt.h"

u32 fn_80164398(void)
{
    return OSDisableInterrupts();
}
