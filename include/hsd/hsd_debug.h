/**
 * @file hsd_debug.h
 * @brief HSD debug/assertion macros.
 *
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_DEBUG_H
#define HSD_DEBUG_H

#include "dolphin/types.h"

/* Assertion function - prints file/line/expression and halts */
void __assert(char* file, u32 line, char* expr);

/**
 * HSD_ASSERT - runtime assertion macro.
 *
 * In Colosseum (as in Melee), assertions embed a hardcoded line number
 * rather than using __LINE__. This is used to match the original binary
 * where assert strings contain specific line numbers from HAL's source.
 */
#define HSD_ASSERT(line, cond) \
    ((cond) ? ((void) 0) : __assert(__FILE__, line, #cond))

#define HSD_ASSERTMSG(line, cond, msg) \
    ((cond) ? ((void) 0) : __assert(__FILE__, line, msg))

#endif /* HSD_DEBUG_H */
