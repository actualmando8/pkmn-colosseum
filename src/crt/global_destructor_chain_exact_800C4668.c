/* Canonical CodeWarrior global destructor chain. */
#include "dolphin/types.h"

typedef void (*DestructorFunc)(void* object, int mode);

typedef struct DestructorChainEntry {
    struct DestructorChainEntry* next;
    DestructorFunc destructor;
    void* object;
} DestructorChainEntry;

DestructorChainEntry* __global_destructor_chain;

void __destroy_global_chain(void)
{
    DestructorChainEntry* entry;

    while ((entry = __global_destructor_chain) != NULL) {
        __global_destructor_chain = entry->next;
        entry->destructor(entry->object, -1);
    }
}
