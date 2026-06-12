/**
 * @file gs_thread.h
 * @brief GSthread -- Genius Sonority cooperative task / thread system.
 *
 * GSthread provides two layers:
 *   1. "Tasks" (lightweight callbacks) -- stored in a priority-sorted linked
 *      list.  Each frame, GStaskRun iterates the list and invokes every
 *      active task whose state == 1 (ACTIVE) and pause flag == 0.
 *   2. "Threads" (heavier cooperative fibres) -- each thread owns a GSmem
 *      handle for its stack and a context block.  The scheduler walks a
 *      priority-sorted linked list and calls each thread's entry function.
 *
 * Neither layer uses the Dolphin OS thread API for scheduling; they are
 * entirely cooperative (no preemption).
 *
 * Debug string: "GSthread: Init OK, maximum of %d threads"
 * Address range: 0x800F07A8 - 0x800FE9B0 (approx.)
 */
#ifndef GS_THREAD_H
#define GS_THREAD_H

#include "dolphin/types.h"

/* -----------------------------------------------------------------------
 * Task structure -- 0x18 bytes (24 bytes).
 * Kept in a singly-linked list sorted by priority (field 0x0C).
 * ----------------------------------------------------------------------- */
typedef struct GSTask {
    /* 0x00 */ struct GSTask* prev;      /* previous task in list   */
    /* 0x04 */ struct GSTask* next;      /* next task in list       */
    /* 0x08 */ u32            state;     /* 0=free, 1=active, 2=deferred */
    /* 0x0C */ u8             priority;  /* sort key (lower = first) */
    /* 0x0D */ u8             paused;    /* if nonzero, skip execution */
    /* 0x0E */ u8             pad[2];
    /* 0x10 */ void*          param;     /* user parameter           */
    /* 0x14 */ void          (*func)(u32 taskId, void* param);
                                         /* callback function        */
} GSTask;

/* -----------------------------------------------------------------------
 * Thread structure -- 0x24 bytes (36 bytes).
 * Kept in a priority-sorted doubly-linked list.
 * ----------------------------------------------------------------------- */
typedef struct GSThread {
    /* 0x00 */ struct GSThread* prev;        /* previous thread          */
    /* 0x04 */ struct GSThread* next;        /* next thread              */
    /* 0x08 */ u8               active;      /* 0=free, 1=active         */
    /* 0x09 */ u8               suspended;   /* paused flag              */
    /* 0x0A */ u8               sleeping;    /* sleeping flag            */
    /* 0x0B */ u8               affinity;    /* affinity / sort key      */
    /* 0x0C */ u32              priority;    /* scheduling priority      */
    /* 0x10 */ u32              stackSize;   /* stack size               */
    /* 0x14 */ u8               pad0;        /* unused                   */
    /* 0x15 */ u8               pad1;
    /* 0x16 */ u8               usesFPU;     /* needs FPU context save   */
    /* 0x17 */ u8               autoStart;   /* start immediately        */
    /* 0x18 */ u32              unused;
    /* 0x1C */ void*            entryFunc;   /* thread entry point       */
    /* 0x20 */ u16              stackHandle; /* GSmem handle for stack   */
    /* 0x22 */ u16              ctxHandle;   /* GSmem handle for context */
} GSThread;

/* -----------------------------------------------------------------------
 * Task states
 * ----------------------------------------------------------------------- */
#define GSTASK_FREE     0
#define GSTASK_ACTIVE   1
#define GSTASK_DEFERRED 2

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

/**
 * GStaskInit -- Initialise the task system.
 *
 * @param numTasks   Maximum number of tasks.
 * @param numQueues  Number of deferred-task queues.
 *
 * Allocates the task array from GSmem and zeroes all entries.
 * Prints "GSthread: Init OK, maximum of %d threads".
 *
 * Corresponds to fn_800FE9B0.
 */
void GStaskInit(u32 numTasks, u32 numQueues);

/**
 * GStaskCreate -- Create and register a new task.
 *
 * @param state      Initial state (1 = ACTIVE, 2 = DEFERRED).
 * @param priority   Task priority (lower = runs first).
 * @param param      User parameter passed to callback.
 * @param func       Callback function pointer.
 * @return           1-based task ID, or 0 on failure.
 *
 * Tasks are inserted into the active list in priority order.
 * Corresponds to fn_800FE834.
 */
u32 GStaskCreate(u32 state, u8 priority, void* param, void* func);

/**
 * GStaskRun -- Execute all active tasks once.
 *
 * Walks the task linked list; for each task with state==1 and paused==0,
 * calls the callback with (taskId, param).  This is the cooperative
 * "yield" point called from the main loop.
 *
 * Corresponds to fn_800FE7A0.
 */
void GStaskRun(void);

/**
 * GSthreadInit -- Initialise the cooperative thread system.
 *
 * @param maxThreads  Maximum number of threads.
 *
 * Allocates the thread array and zeroes all entries.
 * Corresponds to GSthread.
 */
void GSthreadInit(u32 maxThreads);

/**
 * GSthreadCreate -- Create and start a cooperative thread.
 *
 * @param affinity   Thread affinity / scheduling class.
 * @param priority   Priority value (lower = runs first).
 * @param stackSize  Stack size in bytes.
 * @param usesFPU    If non-zero, save/restore FPU registers on context switch.
 * @param autoStart  If non-zero, thread begins executing immediately.
 * @param entryFunc  Thread entry point.
 * @return           Pointer to the GSThread, or NULL on failure.
 *
 * Corresponds to GSthreadCreate.
 */
GSThread* GSthreadCreate(u32 affinity, u32 priority, u32 stackSize,
                         u32 usesFPU, u32 autoStart, void* entryFunc);

/**
 * GSthreadSetStepLimit -- Set the maximum number of steps per frame.
 *
 * @param maxSteps   Step count limit.
 *
 * Corresponds to fn_800F9670.
 */
void GSthreadSetStepLimit(u32 maxSteps);

/**
 * GSthreadPoolConfig -- Configure the thread pool.
 *
 * @param a, b, c, d  Pool parameters.
 *
 * Corresponds to fn_800FF828.
 * NOTE: This function is more accurately GSfloorInit() -- it initializes
 * the floor/scene system's resource pools and creates the floor management
 * thread. See include/game/gs_floor.h for the updated declaration.
 */
void GSthreadPoolConfig(u32 a, u32 b, u32 c, u32 d);

/**
 * GSthreadSchedulerInit -- Initialise the cooperative scheduler.
 *
 * Corresponds to fn_8010D170.
 */
void GSthreadSchedulerInit(void);

#endif /* GS_THREAD_H */
