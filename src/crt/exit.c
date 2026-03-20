#include "dolphin/types.h"

/*
 * exit.c - MetroWerks CRT exit handling.
 *
 * Implements C library exit(), the static constructor/destructor
 * initialization, and program termination support.
 */

/* Linker-defined constructor/destructor tables */
typedef void (*FuncPtr)(void);
extern FuncPtr _ctors[];
extern FuncPtr _dtors[];

/* SDA-relative globals */
extern s32 __aborting;
extern FuncPtr __stdio_exit;
extern FuncPtr __console_exit;
extern s32 __atexit_curr_func;
extern FuncPtr __atexit_funcs[];

extern void __begin_critical_region(s32 region);
extern void __end_critical_region(s32 region);
extern void __kill_critical_regions(void);
extern void __destroy_global_chain(void);
extern void _ExitProcess(void);
extern void PPCHalt(void);

/*
 * __init_cpp - Call all static constructors.
 *
 * Iterates the _ctors table (NULL-terminated array of function
 * pointers) and calls each constructor in order.
 */
static void __init_cpp(void) {
    FuncPtr* p = _ctors;

    while (*p != NULL) {
        (*p)();
        p++;
    }
}

/*
 * __init_user - Initialize user-level CRT state.
 * Calls __init_cpp to run static constructors.
 */
void __init_user(void) {
    __init_cpp();
}

/*
 * _ExitProcess - Halt the processor.
 * Final termination point; never returns.
 */
void _ExitProcess(void) {
    PPCHalt();
}

/*
 * exit - Standard C library exit function.
 *
 * Performs orderly shutdown:
 *   1. If not aborting, call destructor chain and static destructors
 *   2. Call __stdio_exit if registered
 *   3. Call atexit-registered functions in reverse order
 *   4. Call __console_exit if registered
 *   5. Halt the processor via _ExitProcess
 */
void exit(int status) {
    if (__aborting == 0) {
        /* Call begin/end critical region pair (bookkeeping) */
        __begin_critical_region(0);
        __end_critical_region(0);

        /* Run global destructors */
        __destroy_global_chain();

        /* Call static destructors from _dtors table */
        {
            FuncPtr* p = _dtors;
            while (*p != NULL) {
                (*p)();
                p++;
            }
        }

        /* Call stdio exit handler if registered */
        if (__stdio_exit != NULL) {
            __stdio_exit();
            __stdio_exit = NULL;
        }
    }

    /* Call atexit functions in reverse order */
    __begin_critical_region(0);
    while (__atexit_curr_func > 0) {
        __atexit_curr_func--;
        __atexit_funcs[__atexit_curr_func]();
    }
    __end_critical_region(0);

    __kill_critical_regions();

    /* Call console exit handler if registered */
    if (__console_exit != NULL) {
        __console_exit();
        __console_exit = NULL;
    }

    _ExitProcess();
}

/*
 * MWTRACE - MetroWerks trace/debug output function.
 *
 * Variadic no-op stub in release builds. The register spill
 * is required for ABI compliance with varargs on PPC.
 */
asm void MWTRACE(s32 level, const char* fmt, ...) {
    nofralloc
    stwu   r1, -0x70(r1)
    bne    cr1, @skip_fp
    stfd   f1, 0x28(r1)
    stfd   f2, 0x30(r1)
    stfd   f3, 0x38(r1)
    stfd   f4, 0x40(r1)
    stfd   f5, 0x48(r1)
    stfd   f6, 0x50(r1)
    stfd   f7, 0x58(r1)
    stfd   f8, 0x60(r1)
@skip_fp:
    stw    r3, 0x08(r1)
    stw    r4, 0x0c(r1)
    stw    r5, 0x10(r1)
    stw    r6, 0x14(r1)
    stw    r7, 0x18(r1)
    stw    r8, 0x1c(r1)
    stw    r9, 0x20(r1)
    stw    r10, 0x24(r1)
    addi   r1, r1, 0x70
    blr
}

/*
 * usr_put_initialize - Initialize user put (trace output) system.
 * No-op on GameCube.
 */
void usr_put_initialize(void) {
    /* empty */
}
