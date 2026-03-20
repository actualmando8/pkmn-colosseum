#include "dolphin/types.h"

/*
 * runtime.c - CRT library functions.
 *
 * Stub implementations for function coverage.
 */

/* __save_fpr - 0x800C470C | size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void __save_fpr(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* __restore_fpr - 0x800C4758 | size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void __restore_fpr(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* __save_gpr - 0x800C47A4 | size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void __save_gpr(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* __restore_gpr - 0x800C47F0 | size: 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void __restore_gpr(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

