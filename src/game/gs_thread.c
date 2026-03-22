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
