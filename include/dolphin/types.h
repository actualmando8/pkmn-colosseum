#ifndef DOLPHIN_TYPES_H
#define DOLPHIN_TYPES_H

/*
 * Standard GameCube/Dolphin SDK type definitions.
 * These match the MetroWerks CodeWarrior / Nintendo SDK conventions.
 */

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef int BOOL;

#define TRUE 1
#define FALSE 0
#define NULL ((void*)0)

typedef s64 OSTime;
typedef s32 OSHeapHandle;

/* Size type for memory operations */
typedef u32 size_t;

#endif /* DOLPHIN_TYPES_H */
