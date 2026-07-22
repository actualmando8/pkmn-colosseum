#include "dolphin/types.h"

extern u32 fn_80113F48(void);
extern void fn_8018DB68(u32 group, u32 resource);

void fn_801CB9D8(u32 resource)
{
    fn_8018DB68(fn_80113F48(), resource);
}
