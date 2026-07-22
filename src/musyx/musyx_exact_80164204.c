#include "dolphin/types.h"

extern void fn_800AE8EC(void);
extern u32 fn_800AE92C(void);
extern void fn_800AE8A4(void);

u32 fn_80164204(void)
{
    fn_800AE8EC();
    while (fn_800AE92C() != 0) {
    }
    fn_800AE8A4();
    return 1;
}
