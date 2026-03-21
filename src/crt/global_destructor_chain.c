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

/*
 * __cvt_fp2unsigned - Convert a floating-point value to unsigned integer.
 *
 * Used by the CRT for float-to-unsigned conversions. Handles the case
 * where the value exceeds the signed 32-bit range by subtracting 2^31
 * (stored as a double constant) and adding 0x80000000 to the result.
 *
 * The double constants at lbl_8026FE58 are:
 *   +0x00: 0.0 (lower bound)
 *   +0x08: 2^31 as double (4503599627370496.0 / boundary)
 *   +0x10: 2^32 as double (upper bound)
 *
 * 0x800C46B0 | size: 0x5C
 */
u32 fn_800C46B0(f64 val) {
    extern f64 lbl_8026FE58[];
    f64 boundary = lbl_8026FE58[1];
    f64 upperBound = lbl_8026FE58[2];
    u32 result = 0;

    if (val < boundary || val >= boundary) {
        return result;
    }

    if (val >= upperBound) {
        val = val - upperBound;
    }

    result = (u32)(s32)val;

    if (val >= upperBound) {
        result += 0x80000000u;
    }

    return result;
}

