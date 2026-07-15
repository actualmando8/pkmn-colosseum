/**
 * @file sdk_range_8009F77C.c
 * @brief dolphin-sdk code, 0x8009F77C - 0x8009FAEC (8 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct {
    void* head;
    void* tail;
} OSThreadQueue;

typedef struct {
    OSThreadQueue queue;
    void* unk_08;
    void* unk_0C;
} fn_8009F77C_Worker;

extern void OSInitThreadQueue(OSThreadQueue* queue);
extern void OSWakeupThread(void* queue);

void fn_8009F77C(fn_8009F77C_Worker* arg) {
    OSInitThreadQueue(&arg->queue);
    arg->unk_08 = 0;
    arg->unk_0C = 0;
}

void fn_8009F9C8(void* queue) {
    OSInitThreadQueue((OSThreadQueue*)queue);
}

void fn_8009FABC(void* queue) {
    OSWakeupThread(queue);
}
