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
