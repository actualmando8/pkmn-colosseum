/*
 * MetroWerks CodeWarrior global destructor chain for GameCube.
 *
 * Manages the linked list of C++ global/static object destructors.
 * Objects are registered via __register_global_object (called from
 * static initializers) and destroyed in reverse order during
 * program shutdown via __destroy_global_chain.
 */

#include "dolphin/types.h"

typedef void (*DestructorFunc)(void* object, int mode);

typedef struct DestructorChainEntry {
    struct DestructorChainEntry* next;
    DestructorFunc destructor;
    void* object;
} DestructorChainEntry;

/*
 * Head of the global destructor chain. Stored in .sbss (small BSS)
 * for efficient SDA-relative access. Each entry is prepended to
 * the list during static initialization, so destruction happens
 * in reverse order of construction.
 */
DestructorChainEntry* __global_destructor_chain;

/*
 * __destroy_global_chain - Walk the destructor chain and invoke
 * each registered destructor.
 *
 * Called during program shutdown (via the .dtors section reference
 * in __init_cpp_exceptions.c). Each destructor receives its associated
 * object pointer and a mode of -1 (complete object destruction).
 *
 * The chain is consumed as it is walked: each entry's next pointer
 * is stored back to __global_destructor_chain before calling the
 * destructor, ensuring proper behavior even if a destructor triggers
 * further cleanup.
 */
void __destroy_global_chain(void) {
    DestructorChainEntry* entry;

    while ((entry = __global_destructor_chain) != NULL) {
        __global_destructor_chain = entry->next;
        entry->destructor(entry->object, -1);
    }
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C46B0 - 0x800C46B0 | size: 0x5C */
void fn_800C46B0(void) {
    extern u8 lbl_8026FE58[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r4 = (u32)lbl_8026FE58;
    r4 = (u32)lbl_8026FE58;
    r3 = 0x0;
    f0 = *(f64*)((u8*)r4 + 0x0);
    f3 = *(f64*)((u8*)r4 + 0x8);
    f4 = *(f64*)((u8*)r4 + 0x10);
    if (f1 < f3) goto L_800C4704;
    if (f1 >= f3) goto L_800C4704;
    f2 = f1;
    if (f1 < f4) goto L_800C46F0;
    f2 = f1 - f4;
L_800C46F0:
    f2 = (f64)(s32)f2;
    if (f1 < f4) goto L_800C4704;
    r3 = r3 + (0x8000 << 16);
L_800C4704:
    return;
}

