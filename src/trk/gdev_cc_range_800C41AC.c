#include "dolphin/types.h"

/* Low-level GDEV hardware interface functions */
extern void DBInitInterrupts(void); /* GDEV_InitInterrupts */
extern void fn_800CE7D8(void); /* GDEV_PostStop */
extern void fn_800CE7D4(void); /* GDEV_PreContinue */

/*
 * Partial source for the gdev_cc 0x800C41AC range: only the leaf helpers that
 * byte-match under GC/1.3. gdev_cc_peek/write/read are register-allocation
 * near-misses (fleet targets) and are intentionally left as extracted asm.
 */

/* gdev_cc_initinterrupts - Enable GDEV interrupts for async reception. */
s32 gdev_cc_initinterrupts(void) {
    DBInitInterrupts();
    return 0;
}

/* gdev_cc_post_stop - Called after the target stops. */
s32 gdev_cc_post_stop(void) {
    fn_800CE7D8();
    return 0;
}

/* gdev_cc_pre_continue - Called before the target continues. */
s32 gdev_cc_pre_continue(void) {
    fn_800CE7D4();
    return 0;
}
