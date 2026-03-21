/**
 * @file gs_thread.c
 * @brief GSthread -- Genius Sonority cooperative task / thread system.
 *
 * Decompiled from:
 *   fn_800FE9B0 (GStaskInit)
 *   fn_800FE834 (GStaskCreate)
 *   fn_800FE7A0 (GStaskRun)
 *   fn_800F09D8 (GSthreadInit)
 *   fn_800F07A8 (GSthreadCreate)
 *   fn_800FEA74 (GStaskSchedulerThread -- internal)
 *   fn_800FEBA0 (GStaskSwapCallback -- internal)
 *
 * Debug strings:
 *   "GSthread: Init OK, maximum of %d threads"
 *   "GSthreadCreate. Warning: 'usesFPU==FALSE' OK?"
 *
 * The task system and thread system are two separate but related layers:
 *
 * 1. TASKS (lightweight):
 *    - Array of GSTask structs, each 0x18 (24) bytes.
 *    - Kept in a priority-sorted singly-linked list.
 *    - GStaskRun walks the list each frame and calls active callbacks.
 *    - Used for per-frame work: VBlank, pad polling, audio, reset.
 *
 * 2. THREADS (heavier cooperative fibres):
 *    - Array of GSThread structs, each 0x24 (36) bytes.
 *    - Own a GSmem-allocated stack and context block.
 *    - Managed in a priority-sorted doubly-linked list.
 *    - Used for the main game loop and long-running subsystems.
 *
 * Address range: 0x800F07A8 - 0x800FEBA0 (approx.)
 */

#include "dolphin/types.h"
#include "game/gs_thread.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);          /* OSReport */
extern u16   GSmemAllocRaw(u32 size);                    /* fn_800E3534 */
extern void* GSmemGetPtr(u16 handle);                    /* fn_800E27B0 */
extern void* GSmemLock(u16 handle);                      /* fn_800E24B0 */
extern void  GSmemFree(u16 handle);                      /* fn_800E209C */
extern u16   GSmemAlloc(u32 alignment, u32 size);        /* fn_800E2C04 */
extern void  fn_800A263C(void* func, void* arg,
                          void* stackTop, u32 stackSize); /* OSCreateFiber-like */
extern void  OSDisableInterrupts(void);
extern void  OSRestoreInterrupts(void);
extern void  fn_800D30A0(void* callback);                 /* GSgfx register swap callback */
extern void  fn_800F015C(void);                           /* GSthread context init */
extern void  fn_800F01F0(void);                           /* GSthread FPU context init */

/* ===== String constants (rodata references) ===== */
extern const char lbl_80271008[]; /* "GSthreadCreate. Warning: 'usesFPU==FALE' OK?\n" */
extern const char lbl_80271038[]; /* "GSthread: Init OK, maximum of %d threads\n" */

/* ===== Forward declarations for internal functions ===== */
static void GStaskSchedulerThread(void);  /* fn_800FEA74 */
extern void fn_800FEBA0(void);            /* GStaskSwapCallback */
extern void fn_800F0F4C(void);            /* GSthread trampoline / entry wrapper */

/* Forward declarations for converted functions */
u32 fn_800FE010(void);
void fn_800FE7A0(void);
void fn_800FE834(void);
void fn_800FE9B0(void);
void fn_800FEA74(void);


/* ===== Global state (sbss) ===== */

/* --- Task system globals --- */
static u32     gsTaskMaxNormal;     /* lbl_8047AC80 : max normal-priority tasks */
static u32     gsTaskMaxDeferred;   /* lbl_8047AC84 : max deferred-queue tasks  */
static u32     gsTaskTotal;         /* lbl_8047AC88 : total task slots           */
static GSTask* gsTaskCurrentRun;    /* lbl_8047AC94 : task currently executing   */
static GSTask* gsTaskListHead;      /* lbl_8047AC98 : head of active task list   */
static GSTask* gsTaskDeferredHead;  /* lbl_8047AC9C : head of deferred list      */
static u16     gsTaskArrayHandle;   /* lbl_8047AC78 : GSmem handle for task array */
static GSTask* gsTaskArray;         /* lbl_8047AC7C : resolved pointer to tasks  */
static u16     gsTaskStackHandle;   /* lbl_8047AC8C : GSmem handle for scheduler stack */
static void*   gsTaskStackPtr;      /* lbl_8047AC90 : scheduler stack base       */

/* --- Thread system globals --- */
static u32       gsThreadMaxCount;    /* lbl_8047AC30 : maximum thread count */
static u16       gsThreadArrayHandle; /* lbl_8047AC2C : GSmem handle for array */
static GSThread* gsThreadArray;       /* lbl_8047AC28 : resolved pointer */
static GSThread* gsThreadListHead;    /* lbl_8047AC08 : head of active thread list */
static u32       gsThreadFrameCount;  /* lbl_8047AC00 : frame counter */
static u8        gsThreadActive;      /* lbl_8047AC0C : flag indicating threads running */
static void*     gsThreadCurrentCtx;  /* lbl_8047AC1C : current thread context pointer */

/* =======================================================================
 *  GStaskInit / fn_800FE9B0
 *  Address: 0x800FE9B0, Size: 0xC4
 *
 *  Allocates the task array and scheduler stack from GSmem.
 *  Sets up the internal scheduler thread using fn_800A263C.
 *
 *  r3 = numTasks (normal-priority), r4 = numQueues (deferred)
 *
 *  Assembly:
 *    total = r3 + r4
 *    allocSize = total * 0x18 (sizeof GSTask)
 *    gsTaskMaxNormal = r3
 *    gsTaskMaxDeferred = r4
 *    gsTaskTotal = total
 *    gsTaskCurrentRun = NULL
 *    handle = GSmemAllocRaw(allocSize)
 *    gsTaskArrayHandle = handle
 *    gsTaskArray = GSmemGetPtr(handle)
 *    // Zero all task slots: store 0 at offset 0x08 of each 0x18-byte entry
 *    // Allocate 0x2000-byte scheduler stack
 *    stackHandle = GSmemAllocRaw(0x2000)
 *    gsTaskStackHandle = stackHandle
 *    gsTaskStackPtr = GSmemGetPtr(stackHandle)
 *    fn_800A263C(GStaskSchedulerThread, NULL, stackTop, 0x1FFC)
 *    fn_800D30A0(GStaskSwapCallback)
 * ======================================================================= */
void GStaskInit(u32 numTasks, u32 numQueues) {
    u32 total;
    u32 allocSize;
    u16 handle;
    u32 i;
    u32 offset;

    total = numTasks + numQueues;

    gsTaskMaxNormal   = numTasks;
    gsTaskMaxDeferred = numQueues;
    gsTaskTotal       = total;
    gsTaskCurrentRun  = NULL;

    /* Allocate task array: total * 24 bytes */
    allocSize = total * sizeof(GSTask);
    handle = GSmemAllocRaw(allocSize);
    gsTaskArrayHandle = handle;

    if ((handle & 0xFFFF) == 0) {
        return; /* allocation failed */
    }

    gsTaskArray = (GSTask*)GSmemGetPtr(handle & 0xFFFF);

    /* Zero the state field of every task slot */
    offset = 0;
    for (i = 0; i < total; i++) {
        /* Store 0 at offset 0x08 (state field) of each task */
        GSTask* task = (GSTask*)((u32)gsTaskArray + offset);
        task->state = GSTASK_FREE;
        offset += sizeof(GSTask);
    }

    /* Allocate a 0x2000-byte stack for the scheduler co-routine */
    handle = GSmemAllocRaw(0x2000);
    gsTaskStackHandle = handle;
    gsTaskStackPtr = GSmemGetPtr(handle & 0xFFFF);

    /* Create the scheduler fibre:
     * entry = GStaskSchedulerThread
     * arg   = NULL
     * stack = gsTaskStackPtr + 0x1FFC (top of 8KB stack)
     * size  = 0x1FFC */
    fn_800A263C((void*)GStaskSchedulerThread, NULL,
                (void*)((u32)gsTaskStackPtr + 0x1FFC), 0x1FFC);

    /* Register the swap-buffer callback with GSgfx */
    fn_800D30A0((void*)fn_800FEBA0);
}

/* =======================================================================
 *  GStaskCreate / fn_800FE834
 *  Address: 0x800FE834, Size: 0x17C
 *
 *  Creates a task and inserts it into the appropriate list.
 *
 *  r3 = state, r4 = priority, r5 = param, r6 = func
 *
 *  If state == 2 (DEFERRED), search starts from the end of the array
 *  (deferred slots); otherwise search from the beginning (normal slots).
 *
 *  The function finds the first free slot (state == 0), initialises it,
 *  then inserts it into the linked list in priority order.
 *
 *  Returns a 1-based task ID (index = (task - gsTaskArray) / 24 + 1).
 * ======================================================================= */
u32 GStaskCreate(u32 state, u8 priority, void* param, void* func) {
    GSTask* task;
    GSTask* search;
    u32 count;
    u32 i;

    /* Choose search range based on state */
    if (state == GSTASK_DEFERRED) {
        /* Deferred: start from the end of the normal region */
        task = (GSTask*)((u32)gsTaskArray + gsTaskMaxNormal * sizeof(GSTask));
        count = gsTaskMaxDeferred;
    } else {
        /* Normal: start from the beginning */
        task = gsTaskArray;
        count = gsTaskMaxNormal;
    }

    /* Find a free slot */
    for (i = 0; i < count; i++) {
        if (task->state == GSTASK_FREE) {
            goto found;
        }
        task = (GSTask*)((u32)task + sizeof(GSTask));
    }
    /* No free slot */
    return 0;

found:
    /* Initialise the task */
    task->prev     = NULL;
    task->next     = NULL;
    task->state    = state;
    task->priority = priority;
    task->paused   = 0;
    task->param    = param;
    task->func     = (void (*)(u32, void*))func;

    /* Insert into the appropriate linked list */
    if (gsTaskListHead == NULL) {
        /* First task in the list */
        gsTaskListHead = task;
    } else {
        OSDisableInterrupts();

        if (state == GSTASK_DEFERRED) {
            /* Insert into deferred list */
            task->next = gsTaskDeferredHead;
            gsTaskDeferredHead = task;
        } else {
            /* Insert into active list in priority order */
            GSTask* prev = NULL;
            GSTask* curr = gsTaskListHead;

            /* Walk until we find a task with priority >= ours */
            while (curr->next != NULL) {
                if (curr->priority >= task->priority) {
                    break;
                }
                prev = curr;
                curr = curr->next;
            }

            if (curr->next == NULL && curr->priority < task->priority) {
                /* Append at the end */
                task->prev = curr;
                task->next = NULL;
                curr->next = task;
            } else {
                /* Insert before curr */
                GSTask* prevOfCurr = curr->prev;
                if (prevOfCurr != NULL) {
                    prevOfCurr->next = task;
                }
                task->prev = curr->prev;
                task->next = curr;
                curr->prev = task;

                /* Update head if needed */
                if (gsTaskListHead == curr) {
                    gsTaskListHead = task;
                }
            }
        }

        OSRestoreInterrupts();
    }

    /* Compute 1-based task ID:
     * id = ((task - gsTaskArray) / sizeof(GSTask)) + 1
     * Assembly uses mulhwu with magic constant 0xAAAAAAAB for /24 */
    {
        u32 offset = (u32)task - (u32)gsTaskArray;
        u32 id = (offset / sizeof(GSTask)) + 1;
        return id;
    }
}

/* =======================================================================
 *  GStaskRun / fn_800FE7A0
 *  Address: 0x800FE7A0, Size: 0x94
 *
 *  Iterates the active task list and invokes each active, non-paused
 *  task's callback function.  This is the main cooperative yield point.
 *
 *  For each task where state == 1 (ACTIVE) and paused == 0:
 *    1. Store current task in gsTaskCurrentRun.
 *    2. Compute taskId = ((task - gsTaskArray) / 24) + 1
 *    3. Call task->func(taskId, task->param)
 *  After all tasks, clear gsTaskCurrentRun to NULL.
 * ======================================================================= */
void GStaskRun(void) {
    GSTask* task;
    GSTask* nextTask;

    task = gsTaskListHead;
    while (task != NULL) {
        nextTask = task->next;

        if (task->state == GSTASK_ACTIVE && task->paused == 0) {
            u32 taskId;
            u32 offset;

            gsTaskCurrentRun = task;

            /* Compute 1-based task ID */
            offset = (u32)task - (u32)gsTaskArray;
            taskId = (offset / sizeof(GSTask)) + 1;

            /* Invoke the callback via function pointer */
            task->func(taskId, task->param);
        }

        task = nextTask;
    }

    gsTaskCurrentRun = NULL;
}

/* =======================================================================
 *  GSthreadInit / fn_800F09D8
 *  Address: 0x800F09D8, Size: 0x9C
 *
 *  Allocates the thread array from GSmem and zeroes all entries.
 *
 *  r3 = maxThreads
 *
 *  Assembly:
 *    allocSize = maxThreads * 0x24 (sizeof GSThread)
 *    gsThreadMaxCount = maxThreads
 *    handle = GSmemAllocRaw(allocSize)
 *    gsThreadArrayHandle = handle
 *    gsThreadArray = GSmemGetPtr(handle)
 *    // Zero the 'active' byte at offset 0x08 of each 0x24-byte entry
 *    gsThreadFrameCount = 0
 *    gsThreadListHead = NULL
 *    Print "GSthread: Init OK, maximum of %d threads\n"
 * ======================================================================= */
void GSthreadInit(u32 maxThreads) {
    u32 allocSize;
    u16 handle;
    u32 i;
    u32 offset;

    gsThreadMaxCount = maxThreads;

    /* Allocate thread array: maxThreads * 36 bytes */
    allocSize = maxThreads * sizeof(GSThread);
    handle = GSmemAllocRaw(allocSize);
    gsThreadArrayHandle = handle;

    if ((handle & 0xFFFF) == 0) {
        return; /* allocation failed */
    }

    gsThreadArray = (GSThread*)GSmemGetPtr(handle & 0xFFFF);

    /* Zero the 'active' field of every thread slot */
    offset = 0;
    for (i = 0; i < maxThreads; i++) {
        GSThread* thr = (GSThread*)((u32)gsThreadArray + offset);
        thr->active = 0;
        offset += sizeof(GSThread);
    }

    /* Reset frame counter and thread list */
    gsThreadFrameCount = 0;
    gsThreadListHead   = NULL;

    /* Print init message */
    fn_800DD970(lbl_80271038, maxThreads);
}

/* =======================================================================
 *  GSthreadCreate / fn_800F07A8
 *  Address: 0x800F07A8, Size: 0x228
 *
 *  Creates a cooperative thread with its own GSmem-allocated stack.
 *
 *  r3 = affinity, r4 = priority, r5 = stackSize,
 *  r6 = usesFPU, r7 = autoStart, r8 = entryFunc
 *
 *  Assembly:
 *    // Warn if usesFPU == 0
 *    if (usesFPU == 0) fn_800DD970(lbl_80271008);
 *    // Find a free thread slot (active == 0)
 *    thread = NULL;
 *    for each slot in gsThreadArray:
 *      if slot->active == 0: thread = slot; break;
 *    if (thread == NULL) return NULL;
 *    // Allocate stack memory
 *    stackHandle = GSmemAlloc(stackSize, 0x20)  [32-byte aligned]
 *    thread->stackHandle = stackHandle
 *    // Allocate context block: 0x88 or 0x188 depending on usesFPU
 *    ctxSize = usesFPU ? 0x188 : 0x88
 *    ctxHandle = GSmemAllocRaw(ctxSize)
 *    thread->ctxHandle = ctxHandle
 *    // Initialise thread fields
 *    thread->active = 1
 *    thread->priority = priority
 *    thread->stackSize = stackSize
 *    thread->usesFPU = usesFPU
 *    thread->autoStart = autoStart
 *    thread->affinity = affinity
 *    thread->entryFunc = entryFunc
 *    // Resolve pointers
 *    ctx = GSmemGetPtr(ctxHandle)
 *    stack = GSmemGetPtr(stackHandle)
 *    gsThreadCurrentCtx = ctx
 *    // Init context
 *    fn_800F015C()
 *    if (usesFPU) fn_800F01F0()
 *    // Set up stack frame
 *    ctx->stackPtr = stackSize - 8
 *    ctx->entryFunc = entryFunc
 *    ctx->trampoline = fn_800F0F4C
 *    stack->sentinel = -1  (stack guard)
 *    // Lock handles (increment refcount)
 *    GSmemLock(ctxHandle)
 *    GSmemLock(stackHandle)
 *    // Insert into priority-sorted thread list
 *    ... (same linked-list logic as tasks)
 *    gsThreadActive = 1
 *    return thread
 * ======================================================================= */
GSThread* GSthreadCreate(u32 affinity, u32 priority, u32 stackSize,
                          u32 usesFPU, u32 autoStart, void* entryFunc) {
    GSThread* thread;
    u16 stackHandle;
    u16 ctxHandle;
    u32 ctxSize;
    void* ctx;
    void* stack;
    u32 i;

    /* Warn if FPU context saving is disabled */
    if (usesFPU == 0) {
        fn_800DD970(lbl_80271008);
    }

    /* Find a free thread slot */
    thread = gsThreadArray;
    for (i = 0; i < gsThreadMaxCount; i++) {
        if (thread->active == 0) {
            goto found;
        }
        thread = (GSThread*)((u32)thread + sizeof(GSThread));
    }
    /* No free slots */
    return NULL;

found:
    /* Allocate stack from GSmem (32-byte aligned) */
    stackHandle = GSmemAlloc(stackSize, 0x20);
    thread->stackHandle = stackHandle;

    if ((stackHandle & 0xFFFF) == 0) {
        return NULL;
    }

    /* Allocate context block.
     * 0x88 bytes for base context, 0x188 if FPU state is included. */
    ctxSize = (usesFPU != 0) ? 0x188 : 0x88;
    ctxHandle = GSmemAllocRaw(ctxSize);
    thread->ctxHandle = ctxHandle;

    if ((ctxHandle & 0xFFFF) == 0) {
        GSmemFree(stackHandle);
        return NULL;
    }

    /* Initialise thread fields */
    thread->active    = 1;
    thread->priority  = priority;
    thread->stackSize = stackSize;
    thread->suspended = 0;
    thread->sleeping  = 0;
    thread->usesFPU   = (u8)usesFPU;
    thread->entryFunc = entryFunc;
    thread->prev      = NULL;
    thread->next      = NULL;
    thread->affinity  = (u8)affinity;
    thread->autoStart = (u8)autoStart;

    /* Resolve GSmem handles to raw pointers */
    ctx   = GSmemGetPtr(ctxHandle & 0xFFFF);
    stack = GSmemGetPtr(stackHandle & 0xFFFF);

    gsThreadCurrentCtx = ctx;

    /* Initialise the thread's execution context */
    fn_800F015C();
    if (usesFPU != 0) {
        fn_800F01F0(); /* set up FPU save area */
    }

    /* Set up the context's stack pointer and entry point:
     * ctx->stackPtr = stackSize - 8
     * ctx->entry    = entryFunc
     * ctx->trampoline = fn_800F0F4C  (common thread wrapper) */
    {
        u32* ctxWords = (u32*)ctx;
        ctxWords[1] = stackSize - 8;               /* offset 0x04 = stack ptr */
        ctxWords[0x20] = (u32)entryFunc;            /* offset 0x80 = entry    */
        ctxWords[0x21] = (u32)fn_800F0F4C;          /* offset 0x84 = trampoline */
    }

    /* Place a stack sentinel (-1) at the base of the stack */
    *(s32*)stack = -1;

    /* Lock the handles to prevent accidental free */
    GSmemLock(ctxHandle);
    GSmemLock(stackHandle);

    /* Insert into the priority-sorted thread list */
    {
        GSThread* prev = NULL;
        GSThread* curr = gsThreadListHead;

        if (curr == NULL) {
            gsThreadListHead = thread;
        } else {
            /* Walk until we find a thread with affinity >= ours */
            while (curr->next != NULL) {
                if (curr->affinity >= thread->affinity) {
                    break;
                }
                prev = curr;
                curr = curr->next;
            }

            if (curr->next == NULL && curr->affinity < thread->affinity) {
                /* Append at end */
                thread->prev = curr;
                thread->next = NULL;
                curr->next = thread;
            } else {
                /* Insert before curr */
                GSThread* prevOfCurr = curr->prev;
                if (prevOfCurr != NULL) {
                    prevOfCurr->next = thread;
                }
                thread->prev = curr->prev;
                thread->next = curr;
                curr->prev = thread;

                if (gsThreadListHead == curr) {
                    gsThreadListHead = thread;
                }
            }
        }
    }

    gsThreadActive = 1;
    return thread;
}

/* =======================================================================
 *  GStaskSchedulerThread / fn_800FEA74  (INTERNAL)
 *  Address: 0x800FEA74, Size: 0x12C
 *
 *  The scheduler co-routine that runs inside the 8 KB scheduler stack.
 *  It loops forever, executing all active tasks each iteration (same
 *  logic as GStaskRun, but running as a fibre on its own stack).
 * ======================================================================= */
static void GStaskSchedulerThread(void) {
    GSTask* task;
    GSTask* nextTask;

    for (;;) {
        task = gsTaskListHead;
        while (task != NULL) {
            nextTask = task->next;

            if (task->state == GSTASK_DEFERRED && task->paused == 0) {
                u32 taskId;
                u32 offset;

                gsTaskCurrentRun = task;
                offset = (u32)task - (u32)gsTaskArray;
                taskId = (offset / sizeof(GSTask)) + 1;

                task->func(taskId, task->param);
            }

            task = nextTask;
        }

        /* Yield back to the main fibre (implementation is via
         * the cooperative switch in fn_800A263C -- effectively
         * a longjmp back to the caller's context). */
    }
}


/* ===================================================================
 * Stub functions for coverage -- TODO: decompile
 * 43 function(s)
 * =================================================================== */

/* fn_800F0030 - 0x800F0030 | size: 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F0030(void) {
    /* TODO: decompile -- 144 bytes at 0x800F0030 */
}
#pragma pop

/* fn_800F00C0 - 0x800F00C0 | size: 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F00C0(void) {
    /* TODO: decompile -- 156 bytes at 0x800F00C0 */
}
#pragma pop

/* fn_800F028C - 0x800F028C | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F028C(void) {
    /* TODO: decompile -- 104 bytes at 0x800F028C */
}
#pragma pop

/* fn_800F02F4 - 0x800F02F4 | size: 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F02F4(void) {
    /* TODO: decompile -- 20 bytes at 0x800F02F4 */
}
#pragma pop

/* fn_800F0654 - 0x800F0654 | size: 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F0654(void) {
    /* TODO: decompile -- 340 bytes at 0x800F0654 */
}
#pragma pop

/* fn_800F0A74 - 0x800F0A74 | size: 0x4D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F0A74(void) {
    /* TODO: decompile -- 1240 bytes at 0x800F0A74 */
}
#pragma pop

/* fn_800F106C - 0x800F106C | size: 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F106C(void) {
    /* TODO: decompile -- 124 bytes at 0x800F106C */
}
#pragma pop

/* fn_800F10E8 - 0x800F10E8 | size: 0x2E8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F10E8(void) {
    /* TODO: decompile -- 744 bytes at 0x800F10E8 */
}
#pragma pop

/* fn_800F13D0 - 0x800F13D0 | size: 0x2F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F13D0(void) {
    /* TODO: decompile -- 752 bytes at 0x800F13D0 */
}
#pragma pop

/* fn_800F16C0 - 0x800F16C0 | size: 0x34C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F16C0(void) {
    /* TODO: decompile -- 844 bytes at 0x800F16C0 */
}
#pragma pop

/* fn_800F1A0C - 0x800F1A0C | size: 0x42C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F1A0C(void) {
    /* TODO: decompile -- 1068 bytes at 0x800F1A0C */
}
#pragma pop

/* fn_800F1E38 - 0x800F1E38 | size: 0x42C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F1E38(void) {
    /* TODO: decompile -- 1068 bytes at 0x800F1E38 */
}
#pragma pop

/* fn_800F2264 - 0x800F2264 | size: 0x290 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F2264(void) {
    /* TODO: decompile -- 656 bytes at 0x800F2264 */
}
#pragma pop

/* fn_800F24F4 - 0x800F24F4 | size: 0x2E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F24F4(void) {
    /* TODO: decompile -- 736 bytes at 0x800F24F4 */
}
#pragma pop

/* fn_800F27D4 - 0x800F27D4 | size: 0x414 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F27D4(void) {
    /* TODO: decompile -- 1044 bytes at 0x800F27D4 */
}
#pragma pop

/* fn_800F2BE8 - 0x800F2BE8 | size: 0x410 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F2BE8(void) {
    /* TODO: decompile -- 1040 bytes at 0x800F2BE8 */
}
#pragma pop

/* fn_800F2FF8 - 0x800F2FF8 | size: 0x420 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F2FF8(void) {
    /* TODO: decompile -- 1056 bytes at 0x800F2FF8 */
}
#pragma pop

/* fn_800F3418 - 0x800F3418 | size: 0x418 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F3418(void) {
    /* TODO: decompile -- 1048 bytes at 0x800F3418 */
}
#pragma pop

/* fn_800F3830 - 0x800F3830 | size: 0x420 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F3830(void) {
    /* TODO: decompile -- 1056 bytes at 0x800F3830 */
}
#pragma pop

/* fn_800F3C50 - 0x800F3C50 | size: 0x418 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F3C50(void) {
    /* TODO: decompile -- 1048 bytes at 0x800F3C50 */
}
#pragma pop

/* fn_800F4068 - 0x800F4068 | size: 0x3D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F4068(void) {
    /* TODO: decompile -- 984 bytes at 0x800F4068 */
}
#pragma pop

/* fn_800F4440 - 0x800F4440 | size: 0x3D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F4440(void) {
    /* TODO: decompile -- 984 bytes at 0x800F4440 */
}
#pragma pop

/* fn_800F4818 - 0x800F4818 | size: 0x420 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F4818(void) {
    /* TODO: decompile -- 1056 bytes at 0x800F4818 */
}
#pragma pop

/* fn_800F4C38 - 0x800F4C38 | size: 0x3F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F4C38(void) {
    /* TODO: decompile -- 1012 bytes at 0x800F4C38 */
}
#pragma pop

/* fn_800F502C - 0x800F502C | size: 0x3D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F502C(void) {
    /* TODO: decompile -- 984 bytes at 0x800F502C */
}
#pragma pop

/* fn_800F5404 - 0x800F5404 | size: 0x1D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F5404(void) {
    /* TODO: decompile -- 472 bytes at 0x800F5404 */
}
#pragma pop

/* fn_800F55DC - 0x800F55DC | size: 0x214 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F55DC(void) {
    /* TODO: decompile -- 532 bytes at 0x800F55DC */
}
#pragma pop

/* fn_800F57F0 - 0x800F57F0 | size: 0x24C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F57F0(void) {
    /* TODO: decompile -- 588 bytes at 0x800F57F0 */
}
#pragma pop

/* fn_800F5A3C - 0x800F5A3C | size: 0x264 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F5A3C(void) {
    /* TODO: decompile -- 612 bytes at 0x800F5A3C */
}
#pragma pop

/* fn_800F5CA0 - 0x800F5CA0 | size: 0x24C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F5CA0(void) {
    /* TODO: decompile -- 588 bytes at 0x800F5CA0 */
}
#pragma pop

/* fn_800F5EEC - 0x800F5EEC | size: 0x3D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F5EEC(void) {
    /* TODO: decompile -- 976 bytes at 0x800F5EEC */
}
#pragma pop

/* fn_800F62BC - 0x800F62BC | size: 0x3D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F62BC(void) {
    /* TODO: decompile -- 976 bytes at 0x800F62BC */
}
#pragma pop

/* fn_800F668C - 0x800F668C | size: 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F668C(void) {
    /* TODO: decompile -- 128 bytes at 0x800F668C */
}
#pragma pop

/* fn_800F670C - 0x800F670C | size: 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F670C(void) {
    /* TODO: decompile -- 160 bytes at 0x800F670C */
}
#pragma pop

/* fn_800F67AC - 0x800F67AC | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F67AC(void) {
    /* TODO: decompile -- 28 bytes at 0x800F67AC */
}
#pragma pop

/* fn_800F67C8 - 0x800F67C8 | size: 0x184 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F67C8(void) {
    /* TODO: decompile -- 388 bytes at 0x800F67C8 */
}
#pragma pop

/* fn_800F694C - 0x800F694C | size: 0x168 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F694C(void) {
    /* TODO: decompile -- 360 bytes at 0x800F694C */
}
#pragma pop

/* fn_800F6AB4 - 0x800F6AB4 | size: 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F6AB4(void) {
    /* TODO: decompile -- 160 bytes at 0x800F6AB4 */
}
#pragma pop

/* fn_800F6B54 - 0x800F6B54 | size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F6B54(void) {
    /* TODO: decompile -- 88 bytes at 0x800F6B54 */
}
#pragma pop

/* fn_800F6BAC - 0x800F6BAC | size: 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F6BAC(void) {
    /* TODO: decompile -- 16 bytes at 0x800F6BAC */
}
#pragma pop

/* fn_800F6BC4 - 0x800F6BC4 | size: 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F6BC4(void) {
    /* TODO: decompile -- 340 bytes at 0x800F6BC4 */
}
#pragma pop

/* fn_800F6D18 - 0x800F6D18 | size: 0x350 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F6D18(void) {
    /* TODO: decompile -- 848 bytes at 0x800F6D18 */
}
#pragma pop

/* fn_800F7068 - 0x800F7068 | size: 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F7068(void) {
    /* TODO: decompile -- 160 bytes at 0x800F7068 */
}
#pragma pop

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

/* Address: 0x800F6BBC | Size: 0x8 | Pattern: return_constant */
u32 fn_800F6BBC(void) { return 1; }

/* ===================================================================
 * Generated: 1 pattern-matched + 61 stubs
 * Range: 0x800F8268 - 0x800FEBA0
 * =================================================================== */

/* 0x800F8268 | 0x1C0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F8268(void) {
    /* TODO: match -- 448 bytes at 0x800F8268 */
}
#pragma pop

/* 0x800F8428 | 0x22C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F8428(void) {
    /* TODO: match -- 556 bytes at 0x800F8428 */
}
#pragma pop

/* 0x800F8654 | 0x400 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F8654(void) {
    /* TODO: match -- 1024 bytes at 0x800F8654 */
}
#pragma pop

/* 0x800F8A54 | 0x708 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F8A54(void) {
    /* TODO: match -- 1800 bytes at 0x800F8A54 */
}
#pragma pop

/* 0x800F915C | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F915C(void) {
    /* TODO: match -- 180 bytes at 0x800F915C */
}
#pragma pop

/* 0x800F9210 | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9210(void) {
    /* TODO: match -- 196 bytes at 0x800F9210 */
}
#pragma pop

/* 0x44 | fn_800F92D4 | generic */
u32 fn_800F92D4(u32 arg1, u32 arg2) {
    /* refs: lbl_8047AC5C, lbl_8047AC60 */
    return 0;
}

/* 0x60 | fn_800F9318 | generic */
u32 fn_800F9318(void) {
    /* refs: lbl_8047AC5C, lbl_8047AC60 */
    return 0;
}

/* 0x800F9378 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9378(void) {
    /* TODO: match -- 160 bytes at 0x800F9378 */
}
#pragma pop

/* 0x800F9418 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9418(void) {
    /* TODO: match -- 300 bytes at 0x800F9418 */
}
#pragma pop

/* 0x800F9544 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9544(void) {
    /* TODO: match -- 300 bytes at 0x800F9544 */
}
#pragma pop

/* 0x74 | fn_800F9670 | generic */
void fn_800F9670(void) {
    /* refs: lbl_8047AC58, lbl_8047AC5C, lbl_8047AC60 */
    fn_800E3534();
    fn_800E27B0();
}

/* 0x800F96E4 | 0x408 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F96E4(void) {
    /* TODO: match -- 1032 bytes at 0x800F96E4 */
}
#pragma pop

/* 0x800F9AEC | 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9AEC(void) {
    /* TODO: match -- 280 bytes at 0x800F9AEC */
}
#pragma pop

/* 0x800F9C04 | 0x100 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9C04(void) {
    /* TODO: match -- 256 bytes at 0x800F9C04 */
}
#pragma pop

/* 0x800F9D04 | 0x20 -- calls fn_80080ED8 */
extern void fn_80080ED8(void);
void fn_800F9D04(void) {
    fn_80080ED8();
}

/* 0x800F9D24 | 0x14C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9D24(void) {
    /* TODO: match -- 332 bytes at 0x800F9D24 */
}
#pragma pop

/* 0x74 | fn_800F9E70 | generic */
u32 fn_800F9E70(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_800FE010();
    memcpy();
    return 0;
}

/* 0x800F9EE4 | 0x180 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800F9EE4(void) {
    /* TODO: match -- 384 bytes at 0x800F9EE4 */
}
#pragma pop

/* 0x800FA064 | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FA064(void) {
    /* TODO: match -- 252 bytes at 0x800FA064 */
}
#pragma pop

/* 0x5C | fn_800FA160 | single_call_straight */
void fn_800FA160(void) {
    fn_800DBEB4();
}

/* 0x800FA1BC | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FA1BC(void) {
    /* TODO: match -- 196 bytes at 0x800FA1BC */
}
#pragma pop

/* 0x800FA280 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FA280(void) {
    /* TODO: match -- 148 bytes at 0x800FA280 */
}
#pragma pop

/* 0x800FA314 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FA314(void) {
    /* TODO: match -- 188 bytes at 0x800FA314 */
}
#pragma pop

/* 0x74 | fn_800FA3D0 | generic */
u32 fn_800FA3D0(u32 arg1, u32 arg2) {
    /* refs: lbl_80478B08 */
    return 0;
}

/* 0x800FA444 | 0x654 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FA444(void) {
    /* TODO: match -- 1620 bytes at 0x800FA444 */
}
#pragma pop

/* 0x800FAA98 | 0x460 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FAA98(void) {
    /* TODO: match -- 1120 bytes at 0x800FAA98 */
}
#pragma pop

/* 0x800FAEF8 | 0x544 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FAEF8(void) {
    /* TODO: match -- 1348 bytes at 0x800FAEF8 */
}
#pragma pop

/* 0x800FB43C | 0x244 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FB43C(void) {
    /* TODO: match -- 580 bytes at 0x800FB43C */
}
#pragma pop

/* 0x800FB680 | 0x248 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FB680(void) {
    /* TODO: match -- 584 bytes at 0x800FB680 */
}
#pragma pop

/* 0x800FB8C8 | 0x26C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FB8C8(void) {
    /* TODO: match -- 620 bytes at 0x800FB8C8 */
}
#pragma pop

/* 0x800FBB34 | 0x254 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FBB34(void) {
    /* TODO: match -- 596 bytes at 0x800FBB34 */
}
#pragma pop

/* 0x800FBD88 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FBD88(void) {
    /* TODO: match -- 244 bytes at 0x800FBD88 */
}
#pragma pop

/* 0x800FBE7C | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FBE7C(void) {
    /* TODO: match -- 148 bytes at 0x800FBE7C */
}
#pragma pop

/* 0x64 | fn_800FBF10 | single_call_straight */
void fn_800FBF10(void) {
    fn_800EF504();
}

/* 0x800FBF74 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FBF74(void) {
    /* TODO: match -- 604 bytes at 0x800FBF74 */
}
#pragma pop

/* 0x74 | fn_800FC1D0 | generic */
u32 fn_800FC1D0(u32 arg1) {
    /* refs: lbl_80478B08 */
    return 0;
}

/* 0x60 | fn_800FC244 | generic */
u32 fn_800FC244(void) {
    /* refs: lbl_80478B08 */
    return 0;
}

/* 0x800FC2A4 | 0x4 | void_stub */
void fn_800FC2A4(void) {
}

/* 0x800FC2A8 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FC2A8(void) {
    /* TODO: match -- 244 bytes at 0x800FC2A8 */
}
#pragma pop

/* 0x800FC39C | 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FC39C(void) {
    /* TODO: match -- 380 bytes at 0x800FC39C */
}
#pragma pop

/* 0x800FC518 | 0x10 -- store r3 to lbl_80478B08->0x28, return 0 */
extern u32 lbl_80478B08;
u32 fn_800FC518(u32 val) {
    *(u32*)((u8*)lbl_80478B08 + 0x28) = val;
    return 0;
}

/* 0x800FC528 | 0x2B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FC528(void) {
    /* TODO: match -- 696 bytes at 0x800FC528 */
}
#pragma pop

/* 0x800FC7E0 | 0xB68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FC7E0(void) {
    /* TODO: match -- 2920 bytes at 0x800FC7E0 */
}
#pragma pop

/* 0x800FD348 | 0x354 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FD348(void) {
    /* TODO: match -- 852 bytes at 0x800FD348 */
}
#pragma pop

/* 0x800FD69C | 0x880 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FD69C(void) {
    /* TODO: match -- 2176 bytes at 0x800FD69C */
}
#pragma pop

/* 0x800FDF1C | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FDF1C(void) {
    /* TODO: match -- 200 bytes at 0x800FDF1C */
}
#pragma pop

/* 0x800FDFE4 | 0x2C -- (fn_800FE010() + 1) >> 1 - 1 */
u32 fn_800FDFE4(void) {
    u32 result = (u32)fn_800FE010();
    return ((result + 1) >> 1) - 1;
}

/* 0x800FE010 | 0x34C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 fn_800FE010(void) {
    /* TODO: match -- 844 bytes at 0x800FE010 */
}
#pragma pop

/* 0x800FE35C | 0x30 -- fn_800D9D68(0, 0, 0x27F, 0x1DF) -- set viewport 640x480 */
extern void fn_800D9D68(u32 x, u32 y, u32 w, u32 h);
void fn_800FE35C(void) {
    fn_800D9D68(0, 0, 0x27F, 0x1DF);
}

/* 0x800FE38C | 0x148 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FE38C(void) {
    /* TODO: match -- 328 bytes at 0x800FE38C */
}
#pragma pop

/* 0x800FE4D4 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FE4D4(void) {
    /* TODO: match -- 460 bytes at 0x800FE4D4 */
}
#pragma pop

/* 0x800FE6A0 | 0xC -- store two floats to SDA globals */
extern f32 lbl_80478B10;
extern f32 lbl_80478B14;
void fn_800FE6A0(f32 f1, f32 f2) {
    lbl_80478B10 = f1;
    lbl_80478B14 = f2;
}

/* 0x800FE6AC | 0x24 -- output lbl_8047AC70 and lbl_8047AC72 to pointers */
extern s16 lbl_8047AC70;
extern s16 lbl_8047AC72;
void fn_800FE6AC(s16* outX, s16* outY) {
    if (outX != NULL) { *outX = lbl_8047AC70; }
    if (outY != NULL) { *outY = lbl_8047AC72; }
}

/* 0x800FE6D0 | 0xC -- set lbl_8047AC70 and lbl_8047AC72 */
void fn_800FE6D0(s16 x, s16 y) {
    lbl_8047AC70 = x;
    lbl_8047AC72 = y;
}

/* 0x800FE6DC | 0x1C -- task unpause by 1-based ID: gsTaskArray[(id-1)*0x18 + 0xD] = 0 */
void fn_800FE6DC(u32 taskId) {
    u32 idx = taskId - 1;
    u8* ptr = (u8*)gsTaskArray + idx * sizeof(GSTask) + 0x0D;
    *ptr = 0;
}

/* 0x800FE6F8 | 0x1C -- task pause by 1-based ID: gsTaskArray[(id-1)*0x18 + 0xD] = 1 */
void fn_800FE6F8(u32 taskId) {
    u32 idx = taskId - 1;
    u8* ptr = (u8*)gsTaskArray + idx * sizeof(GSTask) + 0x0D;
    *ptr = 1;
}

/* 0x800FE714 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FE714(void) {
    /* TODO: match -- 140 bytes at 0x800FE714 */
}
#pragma pop

/* 0x800FE7A0 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FE7A0(void) {
    /* TODO: match -- 148 bytes at 0x800FE7A0 */
}
#pragma pop

/* 0x800FE834 | 0x17C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FE834(void) {
    /* TODO: match -- 380 bytes at 0x800FE834 */
}
#pragma pop

/* 0x800FE9B0 | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FE9B0(void) {
    /* TODO: match -- 196 bytes at 0x800FE9B0 */
}
#pragma pop

/* 0x800FEA74 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800FEA74(void) {
    /* TODO: match -- 300 bytes at 0x800FEA74 */
}
#pragma pop
