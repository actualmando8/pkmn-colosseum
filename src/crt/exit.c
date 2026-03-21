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
    FuncPtr* p;

    for (p = _ctors; *p != NULL; p++) {
        (*p)();
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

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C4F34 - 0x800C4F34 | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4F34(void) {
    extern u8 lbl_803FFBB8[];
    extern u8 lbl_8047AA08[];
    extern void fn_800C4FA4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = 0x1;
    __begin_critical_region(1);
    r0 = *(u8*)lbl_8047AA08;
    if ((u32)r0 != (u32)0x0) goto L_800C4F78;
    r3 = (u32)lbl_803FFBB8;
    r4 = 0x0;
    r3 = (u32)lbl_803FFBB8;
    r5 = 0x34;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = 0x1;
    *(u8*)lbl_8047AA08 = r0;
L_800C4F78: ;
    r3 = (u32)lbl_803FFBB8;
    r4 = r31;
    r3 = (u32)lbl_803FFBB8;
    fn_800C4FA4();
    r3 = 0x1;
    __end_critical_region(1);
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* fn_800C4FA4 - 0x800C4FA4 | size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4FA4(void) {
    extern void fn_800C4FFC();
    extern void fn_800C5154();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r4 == (u32)0x0) goto L_800C4FEC;
    r5 = *(u32*)((u8*)r4 + (-4));
    r0 = r5 & 0x1;
    if ((u32)r4 != (u32)0x0) goto L_800C4FCC;
    r5 = *(u32*)((u8*)r5 + 0x8);
    goto L_800C4FD8;
L_800C4FCC: ;
    r0 = *(u32*)((u8*)r4 + (-8));
    /* clrrwi r5, r0, 3 */;
    /* subi r5, r5, 0x8 */;
L_800C4FD8: ;
    if ((u32)r5 > (u32)0x44) goto L_800C4FE8;
    fn_800C4FFC();
    goto L_800C4FEC;
L_800C4FE8: ;
    fn_800C5154();
L_800C4FEC: ;
    return;
}
#pragma pop

/* fn_800C4FFC - 0x800C4FFC | size: 0x158 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4FFC(void) {
    extern u8 lbl_8026FEE8[];
    extern void fn_800C5154();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    r6 = (u32)lbl_8026FEE8;
    r7 = 0x0;
    r6 = (u32)lbl_8026FEE8;
    goto L_800C5020;
L_800C5018: ;
    r6 = r6 + 0x4;
    r7 = r7 + 0x1;
L_800C5020: ;
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((u32)r5 > (u32)r0) goto L_800C5018;
    /* subi r8, r4, 0x4 */;
    r5 = r7 << 3;
    r4 = *(u32*)((u8*)r4 + (-4));
    r5 = r5 + 0x4;
    r5 = r3 + r5;
    r0 = *(u32*)((u8*)r4 + 0xC);
    if ((u32)r0 != (u32)0x0) goto L_800C50BC;
    r6 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r6 == (u32)r4) goto L_800C50BC;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r0 != (u32)r4) goto L_800C507C;
    r0 = *(u32*)((u8*)r6 + 0x0);
    *(u32*)((u8*)r5 + 0x4) = r0;
    r6 = *(u32*)((u8*)r5 + 0x0);
    r0 = *(u32*)((u8*)r6 + 0x0);
    *(u32*)((u8*)r5 + 0x0) = r0;
    goto L_800C50BC;
L_800C507C: ;
    r0 = *(u32*)((u8*)r4 + 0x4);
    r6 = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r6 + 0x4) = r0;
    r0 = *(u32*)((u8*)r4 + 0x0);
    r6 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r6 + 0x0) = r0;
    r0 = *(u32*)((u8*)r5 + 0x4);
    *(u32*)((u8*)r4 + 0x4) = r0;
    r6 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r6 + 0x0);
    *(u32*)((u8*)r4 + 0x0) = r0;
    r6 = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r6 + 0x4) = r4;
    r6 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r6 + 0x0) = r4;
    *(u32*)((u8*)r5 + 0x4) = r4;
L_800C50BC: ;
    r0 = *(u32*)((u8*)r4 + 0xC);
    *(u32*)((u8*)r8 + 0x4) = r0;
    *(u32*)((u8*)r4 + 0xC) = r8;
    r6 = *(u32*)((u8*)r4 + 0x10);
    /* subic. r0, r6, 0x1 */;
    *(u32*)((u8*)r4 + 0x10) = r0;
    if ((u32)r0 != (u32)r4) goto L_800C5144;
    r0 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_800C50EC;
    r0 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r5 + 0x4) = r0;
L_800C50EC: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r0 != (u32)r4) goto L_800C5100;
    r0 = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r5 + 0x0) = r0;
L_800C5100: ;
    r0 = *(u32*)((u8*)r4 + 0x4);
    r6 = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r6 + 0x4) = r0;
    r0 = *(u32*)((u8*)r4 + 0x0);
    r6 = *(u32*)((u8*)r4 + 0x4);
    *(u32*)((u8*)r6 + 0x0) = r0;
    r0 = *(u32*)((u8*)r5 + 0x4);
    if ((u32)r0 != (u32)r4) goto L_800C512C;
    r0 = 0x0;
    *(u32*)((u8*)r5 + 0x4) = r0;
L_800C512C: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r0 != (u32)r4) goto L_800C5140;
    r0 = 0x0;
    *(u32*)((u8*)r5 + 0x0) = r0;
L_800C5140: ;
    fn_800C5154();
L_800C5144: ;
    return;
}
#pragma pop

/* fn_800C5154 - 0x800C5154 | size: 0x294 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C5154(void) {
    extern void fn_800C4D8C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    /* subi r8, r4, 0x8 */;
    r4 = *(u32*)((u8*)r4 + (-8));
    r5 = *(u32*)((u8*)r8 + 0x4);
    r0 = r4 & 0xFFFFFFFD;
    /* clrrwi r6, r4, 3 */;
    *(u32*)((u8*)r8 + 0x0) = r0;
    r7 = r8 + r6;
    /* clrrwi r4, r5, 1 */;
    r0 = *(u32*)((u8*)r7 + 0x0);
    r0 = r0 & 0xFFFFFFFB;
    *(u32*)((u8*)r7 + 0x0) = r0;
    *(u32*)((u8*)r7 + (-4)) = r6;
    r0 = *(u32*)((u8*)r4 + 0xC);
    /* clrrwi r5, r0, 3 */;
    /* subi r0, r5, 0x4 */;
    r5 = *(u32*)(r4 + r0);
    if ((u32)r5 == (u32)0x0) goto L_800C5330;
    r5 = *(u32*)((u8*)r5 + 0x8);
    *(u32*)((u8*)r8 + 0x8) = r5;
    r5 = *(u32*)((u8*)r8 + 0x8);
    *(u32*)((u8*)r5 + 0xC) = r8;
    r5 = *(u32*)(r4 + r0);
    *(u32*)((u8*)r8 + 0xC) = r5;
    r5 = *(u32*)(r4 + r0);
    *(u32*)((u8*)r5 + 0x8) = r8;
    *(u32*)(r4 + r0) = r8;
    r9 = *(u32*)(r4 + r0);
    r5 = *(u32*)((u8*)r9 + 0x0);
    r5 = r5 & 0x00000004;
    if ((u32)r5 != (u32)0x0) goto L_800C526C;
    r8 = *(u32*)((u8*)r9 + (-4));
    r5 = r8 & 0x00000002;
    if ((u32)r5 == (u32)0x0) goto L_800C51F0;
    r7 = r9;
    goto L_800C5270;
L_800C51F0: ;
    r7 = r9 - r8;
    r5 = *(u32*)((u8*)r7 + 0x0);
    r5 = r5 & 0x7;
    *(u32*)((u8*)r7 + 0x0) = r5;
    r5 = *(u32*)((u8*)r9 + 0x0);
    r6 = *(u32*)((u8*)r7 + 0x0);
    /* clrrwi r5, r5, 3 */;
    r5 = r8 + r5;
    /* clrrwi r5, r5, 3 */;
    r5 = r6 | r5;
    *(u32*)((u8*)r7 + 0x0) = r5;
    r5 = *(u32*)((u8*)r7 + 0x0);
    r5 = r5 & 0x00000002;
    if ((u32)r5 != (u32)0x0) goto L_800C523C;
    r5 = *(u32*)((u8*)r9 + 0x0);
    /* clrrwi r5, r5, 3 */;
    r6 = r8 + r5;
    /* subi r5, r6, 0x4 */;
    *(u32*)(r7 + r5) = r6;
L_800C523C: ;
    r5 = *(u32*)(r4 + r0);
    if ((u32)r5 != (u32)r9) goto L_800C5250;
    r5 = *(u32*)((u8*)r5 + 0xC);
    *(u32*)(r4 + r0) = r5;
L_800C5250: ;
    r6 = *(u32*)((u8*)r9 + 0x8);
    r5 = *(u32*)((u8*)r9 + 0xC);
    *(u32*)((u8*)r5 + 0x8) = r6;
    r6 = *(u32*)((u8*)r9 + 0xC);
    r5 = *(u32*)((u8*)r6 + 0x8);
    *(u32*)((u8*)r5 + 0xC) = r6;
    goto L_800C5270;
L_800C526C: ;
    r7 = r9;
L_800C5270: ;
    *(u32*)(r4 + r0) = r7;
    r9 = *(u32*)(r4 + r0);
    r6 = *(u32*)((u8*)r9 + 0x0);
    /* clrrwi r10, r6, 3 */;
    r8 = r9 + r10;
    r7 = *(u32*)((u8*)r8 + 0x0);
    r5 = r7 & 0x00000002;
    if ((u32)r5 != (u32)r9) goto L_800C533C;
    r5 = r6 & 0x7;
    /* clrrwi r6, r7, 3 */;
    *(u32*)((u8*)r9 + 0x0) = r5;
    r7 = r10 + r6;
    /* clrrwi r5, r7, 3 */;
    r6 = *(u32*)((u8*)r9 + 0x0);
    r5 = r6 | r5;
    *(u32*)((u8*)r9 + 0x0) = r5;
    r5 = *(u32*)((u8*)r9 + 0x0);
    r5 = r5 & 0x00000002;
    if ((u32)r5 != (u32)r9) goto L_800C52C4;
    /* subi r5, r7, 0x4 */;
    *(u32*)(r9 + r5) = r7;
L_800C52C4: ;
    r5 = *(u32*)((u8*)r9 + 0x0);
    r5 = r5 & 0x00000002;
    if ((u32)r5 != (u32)r9) goto L_800C52E0;
    r5 = *(u32*)(r9 + r7);
    r5 = r5 & 0xFFFFFFFB;
    *(u32*)(r9 + r7) = r5;
    goto L_800C52EC;
L_800C52E0: ;
    r5 = *(u32*)(r9 + r7);
    r5 = r5 | 0x4;
    *(u32*)(r9 + r7) = r5;
L_800C52EC: ;
    r5 = *(u32*)(r4 + r0);
    if ((u32)r5 != (u32)r8) goto L_800C5300;
    r5 = *(u32*)((u8*)r5 + 0xC);
    *(u32*)(r4 + r0) = r5;
L_800C5300: ;
    r5 = *(u32*)(r4 + r0);
    if ((u32)r5 != (u32)r8) goto L_800C5314;
    r5 = 0x0;
    *(u32*)(r4 + r0) = r5;
L_800C5314: ;
    r6 = *(u32*)((u8*)r8 + 0x8);
    r5 = *(u32*)((u8*)r8 + 0xC);
    *(u32*)((u8*)r5 + 0x8) = r6;
    r6 = *(u32*)((u8*)r8 + 0xC);
    r5 = *(u32*)((u8*)r8 + 0x8);
    *(u32*)((u8*)r5 + 0xC) = r6;
    goto L_800C533C;
L_800C5330: ;
    *(u32*)(r4 + r0) = r8;
    *(u32*)((u8*)r8 + 0x8) = r8;
    *(u32*)((u8*)r8 + 0xC) = r8;
L_800C533C: ;
    r5 = *(u32*)(r4 + r0);
    r6 = *(u32*)((u8*)r4 + 0x8);
    r0 = *(u32*)((u8*)r5 + 0x0);
    /* clrrwi r0, r0, 3 */;
    if ((u32)r6 >= (u32)r0) goto L_800C5358;
    *(u32*)((u8*)r4 + 0x8) = r0;
L_800C5358: ;
    r5 = *(u32*)((u8*)r4 + 0x10);
    r7 = 0x0;
    r0 = r5 & 0x00000002;
    if ((u32)r6 != (u32)r0) goto L_800C5384;
    r0 = *(u32*)((u8*)r4 + 0xC);
    /* clrrwi r6, r5, 3 */;
    /* clrrwi r5, r0, 3 */;
    /* subi r0, r5, 0x18 */;
    if ((u32)r6 != (u32)r0) goto L_800C5384;
    r7 = 0x1;
L_800C5384: ;
    if ((s32)r7 == (s32)0x0) goto L_800C53D8;
    r5 = *(u32*)((u8*)r4 + 0x4);
    if ((u32)r5 != (u32)r4) goto L_800C539C;
    r5 = 0x0;
L_800C539C: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)r4) goto L_800C53AC;
    *(u32*)((u8*)r3 + 0x0) = r5;
L_800C53AC: ;
    if ((u32)r5 == (u32)0x0) goto L_800C53C4;
    r0 = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r5 + 0x0) = r0;
    r3 = *(u32*)((u8*)r5 + 0x0);
    *(u32*)((u8*)r3 + 0x4) = r5;
L_800C53C4: ;
    r0 = 0x0;
    r3 = r4;
    *(u32*)((u8*)r4 + 0x4) = r0;
    *(u32*)((u8*)r4 + 0x0) = r0;
    fn_800C4D8C();
L_800C53D8: ;
    return;
}
#pragma pop

/* fn_800C53E8 - 0x800C53E8 | size: 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C53E8(void) {
    extern void fn_800C7904();
    extern u8 __files[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r3 = (u32)__files;
    r0 = (u32)__files;
    r31 = 0x0;
    r30 = r0;
    goto L_800C5434;
L_800C5410: ;
    r0 = *(u16*)((u8*)r30 + 0x4);
    /* extrwi. r0, r0, 3, 23 */;
    if ((s32)r0 == (s32)0) goto L_800C5430;
    r3 = r30;
    fn_800C7904();
    if ((s32)r3 == (s32)0x0) goto L_800C5430;
    r31 = -0x1;
L_800C5430: ;
    r30 = *(u32*)((u8*)r30 + 0x4C);
L_800C5434: ;
    if ((u32)r30 != (u32)0x0) goto L_800C5410;
    r3 = r31;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

