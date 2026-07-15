/**
 * @file gs_range_800C45A0.c
 * @brief gs-engine code, 0x800C45A0 - 0x800C4668 (1 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct VaList {
    signed char gpr;
    signed char fpr;
    char reserved[2];
    char* inputArgArea;
    char* regSaveArea;
} VaList;

#define ALIGN(address, size)                                                   \
    (((u32)(address) + ((size) - 1)) & ~((size) - 1))

void* __va_arg(VaList* list, int type) {
    char* address;
    signed char* reg = &list->gpr;
    int gpr = list->gpr;
    int maxSize = 8;
    int size = 4;
    int increment = 1;
    int even = 0;
    int fprOffset = 0;
    int regSize = 4;

    if (type == 3) {
        reg = &list->fpr;
        gpr = list->fpr;
        size = 8;
        fprOffset = 0x20;
        regSize = 8;
    }

    if (type == 2) {
        size = 8;
        maxSize--;
        if (gpr & 1) {
            even = 1;
        }
        increment = 2;
    }

    if (gpr < maxSize) {
        gpr += even;
        address = list->regSaveArea + fprOffset + gpr * regSize;
        *reg = gpr + increment;
    } else {
        *reg = 8;
        address = list->inputArgArea;
        address = (char*)ALIGN(address, size);
        list->inputArgArea = address + size;
    }

    if (type == 0) {
        address = *(char**)address;
    }

    return address;
}
