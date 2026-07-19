#include "dolphin/types.h"

extern void fn_800210F0(void);

/* Retained in the original gs_title translation unit despite no direct callsite. */
#pragma force_active on

void fn_80021624(void)
{
    fn_800210F0();
}
