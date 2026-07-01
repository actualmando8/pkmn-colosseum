#include "dolphin/types.h"

/* Low-level AMC hardware interface functions */
extern void fn_800CE7A0(void); /* AMC_InitInterrupts */
extern void fn_800CE7BC(void); /* AMC_PostStop */
extern void fn_800CE7C0(void); /* AMC_PreContinue */

/*
 * Partial source for the ddh_cc 0x800C3C00 range: only the leaf helpers that
 * byte-match under GC/1.3. ddh_cc_peek/write/read are register-allocation
 * near-misses (fleet targets) and are intentionally left as extracted asm.
 */

/* ddh_cc_initinterrupts - Enable AMC interrupts for async reception. */
s32 ddh_cc_initinterrupts(void) {
    fn_800CE7A0();
    return 0;
}

/* ddh_cc_post_stop - Called after the target stops. */
s32 ddh_cc_post_stop(void) {
    fn_800CE7BC();
    return 0;
}

/* ddh_cc_pre_continue - Called before the target continues. */
s32 ddh_cc_pre_continue(void) {
    fn_800CE7C0();
    return 0;
}
